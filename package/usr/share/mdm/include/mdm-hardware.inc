#
#  Funções para detecção de hardware no mdm
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
#                31/01/2008
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

#
# Descobre os teclados instalados no sistema.
#
# Parâmetros:   1, opcional
#                   * all       mostra informacoes padrao do teclado (default)
#                   * event     mostra apenas os eventos associados ao teclado
#
# Saida:        imprime na saida padrão os teclados encontrados no sistema
#

function Keyboards()
{
    # verifica o parâmetro passado
    if [[ -z "${1}" ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
        
    # Seleciona os dispositivos físicos (endereços)
    local ALL_PHYSICAL=(`cat ${PROC_DEVICES} | \
                         grep P: | \
                         cut -d' ' -f2`)
    
    # Seleciona os controladores dos dispositivos
    local ALL_HANDLERS=(`cat ${PROC_DEVICES} | \
                         grep H: | \
                         cut -d' ' -f2`)
    
    # Seleciona os eventos dos dispositivos
    local ALL_EVENTS=(`cat ${PROC_DEVICES} | \
                       grep H: | \
                       sed 's/ /\n/g' | \
                       grep event`)
    
    # Seleciona os nomes dos dispositivos
    local ALL_NAMES=(`cat ${PROC_DEVICES} | \
                      grep N: | \
                      cut -d'=' -f2 | \
                      tr -d ' '`)

    # Regras para detecção do teclado:
    #  * Palavra kbd deve estar no controlador 
    #  * Palavra Speaker ou Button não devem estar no nome do dispositivo
    #  * O endereço físico do dispostivo deve terminar em 0.
    for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
    do
       if  echo ${ALL_HANDLERS[i]} | \
           grep "kbd" > /dev/null && \
           echo ${ALL_NAMES[i]} | \
           egrep -v "(Speaker|Button)" > /dev/null && \
           echo ${ALL_PHYSICAL[i]} | \
           grep ".*0$" > /dev/null ; then

           local AUX=`echo ${ALL_PHYSICAL[i]} | cut -f2 -d'='`
           case ${TYPE} in
                all)
                    echo -e "kbd\t${AUX}"
                    ;;
                event)
                    echo -e "kevent\t/dev/input/${ALL_EVENTS[i]}"
                    ;;
           esac
        fi
    done
} # Keyboards

#
# Descobre os mouses instalados no sistema
#
# Parâmetros:   tipo de saida (opcional)
#                   * all       mostra informacões padrão do mouse (default)
#                   * event     mostra apenas os eventos associados ao mouse
#
# Saida:        imprime na saída padrão os mouses encontrados no sistema
#

function Mice()
{
    # verifica o parâmetro passado
    if [[ -z "${1}" ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
    
    # Seleciona os dispositivos físicos (endereços)
    local ALL_PHYSICAL=(`cat $PROC_DEVICES | \
                         grep P: | \
                         cut -d' ' -f2`)
    
    # Seleciona os controladores dos dispositivos
    local ALL_HANDLERS=(`cat $PROC_DEVICES | \
                         grep H: | \
                         cut -d' ' -f2`)
    
    # Seleciona os eventos dos dispositivos
    local ALL_EVENTS=(`cat ${PROC_DEVICES} | \
                       grep H: | \
                       sed 's/ /\n/g' | \
                       grep event`)

    case ${TYPE} in
        all)
            # Regras para detectar o mouse: 
            #   * handler deve conter mouse  
            #   * Endereco fisico deve terminar em 0
            for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
            do
                if echo ${ALL_HANDLERS[i]} | \
                    grep "mouse" > /dev/null && \
                    echo ${ALL_PHYSICAL[i]} | \
                    grep ".*0$" > /dev/null; then

                    local AUX=`echo ${ALL_PHYSICAL[i]} | cut -d'=' -f2`
                    echo -e "mouse\t${AUX}"
                fi
            done
            ;;
        event)
            # eventos já utilizados
            local USADOS=""

            # Regras para detectar eventos do mouse: 
            #   * handler deve conter "mouse E"
            #   * Endereco fisico deve terminar em 0
            #   * Endereco fisico em /inputX não está em uso
            #     (é necessário quando o mouse tem input1 sem
            #      input0).
            for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
            do
                if echo ${ALL_HANDLERS[i]} | \
                    grep "mouse" > /dev/null && \
                    (echo ${ALL_PHYSICAL[i]} | \
                     grep ".*0$" > /dev/null || \
                     ! grep -q "${ALL_PHYSICAL[i]}" <<< ${USADOS}); then

                    USADOS="${USADOS} $(cut -d/ -f1 <<< ${ALL_PHYSICAL[i]})"
                    echo -e "mevent\t/dev/input/${ALL_EVENTS[i]}"
                fi
            done
            ;;
    esac 
    # Se não encontrou nenhum mouse, assume mouse serial como padrão 
    if [[ ${#ALL_PHYSICAL[@]} -eq 0 ]]; then
        echo -e "mouse\tserial0"
    fi
} # Mice

#
# Descobre as placas de video instaladas no computador
#
# Parâmetros:   1 parametro opcional.
#               podendo ser: driver, bus ou all. all é o default
# Saida:        imprime na saída padrão o endereço do BUS da placa de video
#               imprime na saída padrão o driver de video a ser usado
#
function VideoCards()
{
    # verifica o parâmetro passado
    if [[ $# -ne 1 ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
        
    # Usa o discover para encontrar os drivers para os dispositivos
    local ALL_DRIVERS=(`${Discover} --type-summary \
                                    --data-path=xfree86/server/device/driver \
                                    display 2> /dev/null`)

    # obtém lista de placas de video instaladas no computador
    local ALL_BUS_IDS=(`lspci | grep "VGA" | cut -f1 -d' '`)
    
    for (( i=0; i < ${#ALL_BUS_IDS[@]}; i=i+1 )); do
        # Verifica se foi detectado um driver para o dispositivo.
        # Se não encontrou o driver, assume o driver VESA
        if [[ "${ALL_DRIVERS[$i]}" = "" ]]; then 
            ALL_DRIVERS[$i]=vesa
        fi
        
        # os BUSID retornados pelo comando lspci estão no formato 00:00.00
        # vamos dividir o busid em 00 e 00.00
        NUMS=(`echo ${ALL_BUS_IDS[$i]} |  \
             awk 'BEGIN {FS=":"}{print toupper($1), toupper($2)}'`)

    	# Agora, vamos dividir o 00.00 em 00 e 00
        SEC_NUMS=(`echo ${NUMS[1]} |  \
                   awk 'BEGIN {FS="."}{print toupper($1), toupper($2)}'`)
                   
        # Já temos os números divididos em grupo, vamos converter de 
        # Hexadecimal para Decimal
        if [[ ${TYPE} = "all" ]]; then
            echo -e "bus\t`echo "obase=10;ibase=16;${NUMS[0]};\
                    ${SEC_NUMS[0]};${SEC_NUMS[1]};" | bc | paste -s -d":"`"
        elif [[ ${TYPE} = "bus" ]]; then
            echo "obase=10;ibase=16;${NUMS[0]};${SEC_NUMS[0]};${SEC_NUMS[1]};" \
                 | bc | paste -s -d":"
        fi
    done
    
    # Finalmente, escrevemos os drivers encontrados
    for i in ${ALL_DRIVERS[@]}
    do
        if [[ ${TYPE} = "all" ]]; then
            echo -e "driver\t${i}"
        elif [[ ${TYPE} = "driver" ]]; then
            echo ${i}
        fi
    done
} # VideoCards

#
# Seta o número de cabecas a serem usadas
#
# Parâmetros:   nenhum
# Saida:        imprime na saída padrão o número de cabecas a serem usadas
#               O numero de cabecas pode ser forcado através do arquivo de 
#               configuracao do mdm.
#

function NumHeads()
{
    # obtem o número de mouses conectados ao computador
    local MICE=`Mice | wc -l`
    
    # obtem o número de teclados conectados ao computador
    local KBD=`Keyboards | wc -l`
    
    # assume que o número de cabecas é igual ao número de placas de vídeo
    local HEADS=`VideoCards driver | wc -l`
    
    # fica com o menor entre teclados e placas de video
    if [[ ${KBD} -lt ${HEADS} ]]; then
        HEADS=${KBD}
    fi
    
    # fica com o menor entre teclados, placas de video e mouses
    if [[ ${MICE} -lt ${HEADS} ]]; then
        HEADS=${MICE}
    fi
    
    # checa se a variável HEADS contém um valor válido
    if [[ -z ${HEADS} || ${HEADS} -lt 1 ]]; then
        HEADS=1
    fi
    
    # escreve o número de cabecas detectadas
    echo "${HEADS}"
} # NumHeads
    
