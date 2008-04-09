#
# Este arquivo contém as funcões necessárias para gerenciar uma cabeca do
# multiterminal.
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
#                26/02/2008
#
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc
#       mdm-links.inc
#       mdm-images.inc
#       mdm.conf

#
# Esta funcão é responsavel por obter os links para o teclado e o mouse
# que serão associados ao terminal atual.
#
# Parâmetros:   número do terminal para associar o mouse e teclado

function GetLinks()
{

    local TERMINAL=${1}
    
    # aguarda um teclado para o terminal
    KeyboardLink ${TERMINAL}

    # aguarda um mouse para o terminal
    MouseLink ${TERMINAL}
    
    # informa ao usuário para aguardar
    SetImage wait
    
    # apaga as imagens temporárias
    SetImage free   
     
} # GetLinks

#
# Esta funcão pergunta ao usuário se o terminal deve ser reconfigurado, ou seja,
# se o terminal deve obter novo teclado e/ou mouse a ser usado.
#
# Parâmetros:   número do terminal a ser reconfigurado

function CheckReconfig()
{

    local TERMINAL=${1}
    
    # mostra mensagem de reconfiguracão do terminal
    SetImage reconfig
    
    local PRESSED=$(${ReadDevices} 14 \
                    ${LINK_DIR}/mdmKbd${TERMINAL} | \
                    grep '^detecao' | \
                    cut -d'|' -f2)

    if [[ "${PRESSED}" = 'esc' ]]; then
        WriteDebug $"Refazendo configuracão terminal remoto."
                    
        # refaz a configuracão
        rm -f ${LINK_DIR}/mdmKbd${TERMINAL}
        rm -f ${LINK_DIR}/mdmMouse${TERMINAL}
                    
        # obtém novos mouse e teclado para o terminal
        GetLinks ${TERMINAL}
    fi
    
} # CheckReconfig
    
#
# Esta funcão é responsável por abrir um terminal do multiterminal
#
# Parâmetros:   número do terminal a ser aberto

function OpenTerminal()
{

    local TERMINAL=${1}
    
    # obtém teclado e mouse para o terminal
    GetLinks ${TERMINAL}
    
    if [[ -z "${DISPLAY_MANAGER}" ]]; then
        DISPLAY_MANAGER=gdm
    fi
    
    case ${DISPLAY_MANAGER} in
        gdm)
            WriteDebug $"O gdm é o display manager a ser usado."
            # arquivo de XAUTHORITY do gdm
            export XAUTHORITY=/var/lib/gdm/\:0.Xauth
            
            WriteDebug "${TERMINAL}=${XephyrLoader} \
                        -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                        -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                        -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                        -use-evdev -dpi 92 -audit 0 \
                        -display :0.$(( TERMINAL - 1 )) \
                        -xauthority ${XAUTHORITY} :${TERMINAL}"
            # cria a entrada para o servidor Xephyr dinamicamente
            ${GDMDynamic}   -a "${TERMINAL}=${XephyrLoader} \
                            -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                            -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                            -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                            -use-evdev -dpi 92 -audit 0 \
                            -display :0.$(( TERMINAL - 1 )) \
                            -xauthority ${XAUTHORITY} :${TERMINAL}"
         
            while (( 1 )); do
                # invoca o servidor criado anteriormente
                ${GDMDynamic} "-r ${TERMINAL}"
                WriteDebug $"Xephyr Remoto ${TERMINAL} morreu!"
                
                # pergunta se o usuário quer reconfigurar terminal
                CheckReconfig ${TERMINAL}
            done
            ;;
        remote)
            WriteDebug $"Servidor remoto como display manager."
            
            # verifica se existe nome do servidor a invocar. Se não existir,
            # usa um nome padrão. Esta configuracao deve ser feita em mdm.conf
            if [[ -z "${QUERY_COMMAND}" ]]; then
                QUERY_COMMAND="xdmcpserver"
            fi
            
            # repete indefinidamente
            while (( 1 )); do
                # invoca o Xephyr com a opcão -query para login remoto
                ${XephyrLoader} -query ${QUERY_COMMAND} \
                                -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                                -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                                -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                                -use-evdev -dpi 92 -audit 0 \
                                -display :0.$(( TERMINAL - 1 )) \
                                -xauthority ${XAUTHORITY} :${TERMINAL}
                
                WriteDebug $"Xephyr Local ${TERMINAL} morreu!"
                
                # pergunta se o usuário quer reconfigurar o terminal
                CheckReconfig ${TERMINAL}
            done
            ;;
        *)
            echo $"Display Manager Inválido. Verifique mdm.conf e corrija."
            ;;
    esac
    
} # OpenTerminal
