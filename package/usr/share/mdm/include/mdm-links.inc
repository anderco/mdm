#
#  Este arquivo contém as funcões necessárias para criar os links simbólicos
#  para os teclados e mouses de cada terminal. Estes links simbólicos serão
#  utilizados pelo Xephyr para associar cada terminal a um teclado e mouse de 
#  forma exclusiva.
#
# Copyright (C) 2004-2008 Centro de Computacao Cientifica e Software Livre
# Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR
#
# This is free software.  You may redistribute copies of it under the terms of
# the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.
# There is NO WARRANTY, to the extent permitted by law.

#**********************************
#
#  File Version: 1.0.0
#                21/02/2008
#
#  Dependencies:
#       mdm-hardware.inc
#       mdm-path.inc
#       mdm-debug.inc

#
# Esta funcão associa um teclado físico a um terminal, de forma exclusiva
#
# Parâmetros:   número do terminal para associar o teclado
#

function KeyboardLink()
{

    local CREATED
    local TERMINAL=${1}
    local LINK
    
    WriteDebug $"Configurando teclado, terminal ${1}"
    
    if [[ -e ${LINK_DIR}/mdmKbd${1} ]]; then 
        CREATED=1
    else
        CREATED=0
    fi
    
    # verifica o modo de criacao dos links
    case ${LINK_CREATION_MODE} in
        always)
            # ignora o link que já está criado
            CREATED=0
            ;;
        once)
            # se o link já está criado, encerra
            if [[ ${CREATED} ]]; then
                return
            fi
            ;;
    esac
    
    # pede para pressionar a fn
    SetImage keyboard ${TERMINAL}
    
    # repete até ter o link criado
    while (( ! ${CREATED} )); do
           
        # eventos dos teclados
        local KEYB_EVENTS=$( Keyboards event | cut -f2 )
        
        # aguarda até ter  um teclado conectado (HOTPLUG TABAJARA)
        if [[ -z "${KEYB_EVENTS}" ]]; then
            continue
        fi
        
        # aguarda uma tecla ser pressionada no teclado
        local PRESSED=$( ${ReadDevices} ${TERMINAL} ${KEYB_EVENTS} | \
                         grep '^detecao' | \
                         cut -d '|' -f2 )
                        
        # checa se pressionou ou esgotou tempo de espera
        if [[ ${PRESSED} != 'timeout' ]]; then
            
            WriteDebug $"Criando link para o teclado."
            
            # cria o link simbólico 
            ln -s ${PRESSED} ${LINK_DIR}/mdmKbd$1
            
            # seta flag para indicar link criado
            CREATED=1
            
            # verifica se não existe outro link que aponta para o mesmo
            # teclado. Se já houver, exclui o que acabou de criar e continua
            # aguardando o próximo teclado ser pressionado
            for LINK in `ls ${LINK_DIR} | grep "\<mdmKbd"`; do
                          
                # compara o LINK encontrado com o que acabou de criar
                if [[ ${LINK} != "mdmKbd${1}" ]]; then
                    # 
                    local AUX=$( stat -c %N ${LINK_DIR}/${LINK} | \
                                 cut -d'`' -f3 | \
                                 cut -d \' -f1 )
                    if [[ ${AUX} = ${PRESSED} ]]; then
                        # exclui o link criado, deixando o antigo intacto
                        rm -f ${LINK_DIR}/mdmKbd${1}
                    
                        # seta flag para indicar que o link ainda não foi criado
                        CREATED=0
                        
                        WriteDebug $"Teclado conectado a outra cabeca."
                                            
                        # este teclado já está em uso. Remove-o da lista
                        KEYB_EVENTS=`echo ${KEYB_EVENTS} | \
                                     sed "s;\<${AUX}\>;;g"`
                    fi
                fi
            done
        fi
    done
    
    WriteDebug $"Teclado configurado! Terminal: ${TERMINAL}"
    
} # KeyboardLink

#
# Esta funcão associa um mouse físico a um terminal, de forma exclusiva
#
# Parâmetros:   número do terminal para associar o mouse
#

function MouseLink()
{

    # 
    local CREATED
    local TERMINAL=${1}
    local LINK
    local LOCK
    
    WriteDebug $"Configurando mouse, terminal ${1}"
    
    if [[ -e ${LINK_DIR}/mdmMouse${1} ]]; then 
        CREATED=1
    else
        CREATED=0
    fi
    
    # verifica o modo de criacao dos links
    case ${LINK_CREATION_MODE} in
        always)
            # ignora o link que já está criado
            CREATED=0
            ;;
        once)
            # se o link já está criado, encerra
            if [[ ${CREATED} ]]; then
                return
            fi
            ;;
    esac

    # repete até conseguir um mouse para associar a este terminal
    while (( ! ${CREATED} )); do
    
        WriteDebug $"Aguardando mouse conectado."
        
        # listagem dos eventos de mouses conectados ao terminal
        local MICE_EVENTS=$( Mice event | cut -f2 )
        
        # repete até ter um mouse conectado
        if [[ -z "${MICE_EVENTS}" ]]; then
            continue
        fi
        
        # cria um arquivo de trava para impedir clicks do mesmo mouse para 
        # outro terminal, ou vários mouses clicando no mesmo terminal.
        local HAS_LOCK=1
        
        # repete até encontrar um mouse disponível
        while (( ${HAS_LOCK} )); do
        
            # cria arquivo de lock para o terminal sendo configurado
            touch ${LOCK_DIR}/lock${1}
        
            # assume que não há mouse associado a este terminal
            HAS_LOCK=0
            
            # checa todos os arquivos de lock existentes
            for LOCK in `ls ${LOCK_DIR} | grep "\<lock"`; do
                if [[ ${LOCK} != "lock${1}" ]]; then
                    HAS_LOCK=1
                fi
            done
        
            # verifica se há um mouse associado a este terminal
            if (( HAS_LOCK )); then
                # mostra imagem para aguardar
                SetImage wait
            
                # exclui o arquivo de lock criado
                rm -f ${LOCK_DIR}/lock${1}
            
                # aguarda
                sleep 1
            fi
        done
        
        # coloca a imagem de pressione o botão esquerdo do mouse...
        SetImage mouse 
        
        WriteDebug $"Aguardando pressionamento do mouse. Cabeca ${1}"
        
        # aguarda um botão do mouse ser pressionado
        local PRESSED=$( ${ReadDevices} 13 ${MICE_EVENTS} | \
                         grep '^detecao' | 
                         cut -d'|' -f2 )
                        
        # verifica se houve time-out
        if [[ "${PRESSED}" = "timeout" ]]; then
            # pede para aguardar
            SetImage wait
            
            # remove a trava 
            rm -f ${LOCK_DIR}/lock${1}
            
            # aguarda 5 segundos
            sleep 5
        else
            WriteDebug $"Pressionado: ${PRESSED}"
            
            # cria o link
            ln -s ${PRESSED} ${LINK_DIR}/mdmMouse${1}
            
            # seta link como criado
            CREATED=1
            
            # Verifica se já não existe outro link que aponta para este mouse.
            # Se existir, exclui o que foi criado e repete todo o processo
            for LINK in `ls ${LINK_DIR} | grep "\<mdmMouse"`; do
               # compara o LINK encontrado com o que acabou de criar
                if [[ ${LINK} != "mdmMouse${1}" ]]; then
                    # 
                    local AUX=$(stat -c %N ${LINK_DIR}/${LINK} | \
                                cut -d'`' -f3 | \
                                cut -d \' -f1)
                            
                     if [[ ${AUX} = ${PRESSED} ]]; then
                        # exclui o link criado
                        rm -f ${LINK_DIR}/mdmMouse${1}
                        CREATED=0
                    
                        WriteDebug $"Mouse conectado a outra cabeca."
                    
                        # remove da lista dos mouses disponíveis
                        MICE_EVENTS=`echo ${MICE_EVENTS} | \
                                     sed "s;\<${AUX}\>;;g"`
                    fi
                fi
            done
            # exclui arquivo de lock
            rm ${LOCK_DIR}/lock${1}
        fi
    done
    
    WriteDebug $"Mouse Configurado! Terminal: ${TERMINAL}"
} # MouseLink

