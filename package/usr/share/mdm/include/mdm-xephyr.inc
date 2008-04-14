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
#                26/02/2008
#
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

trap "" usr1

function CallXephyr()
{
    local DEBUG_FILE=Xephyr.log
    local ARGS=()
    
    WriteDebug $"Iniciando CallXephyr"
    
    # analisa todos os parâmetros recebidos pela funcão
    while [[ ! -z "${1}" ]]; do
        # checa XAUTHORITY
        if [[ "${1}" == "-xauthority" ]]; then
            shift
            if [[ ! -z "${1}" ]]; then
                export XAUTHORITY="${1}"
            fi
        # checa display a ser usado pelo Xephyr
        elif [[ "${1}" == "-display" ]]; then
            shift
            if [[ ! -z "${1}" ]]; then
                export DISPLAY="${1}"
            fi
        # outros parâmetros
        else
            if ! expr match ${1} 'vt[0-9][0-9]*' > /dev/null; then
                ARGS=("${ARGS[@]}" "${1}")
            fi
        fi
        # próximo parâmetro
        shift
    done
    
    WriteDebug $"Inciando Xephyr. Parâmetros: ${ARGS[@]}"
    
    # executa o Xephyr
    ${Xephyr} "${ARGS[@]}" 2> ${LOG_DIR}/Xephyr_${DISPLAY}.log &
    
    local PID=$!
    
    echo ${PID} > ${PIDS}/Xephyr_${DISPLAY}.pid
   
    # aguarda pelo término do Xephyr
    wait ${PID}
    
    # código de término do Xephyr
    local RETURN_CODE=$?
     
    # exclui o PID
    rm -f ${PIDS}/Xephyr_${DISPLAY}.pid
    
    WriteDebug $"Xephyr terminou. Código de retorno: ${RETURN_CODE}"
} # CallXephyr
