#
#  This file has necessary functions to create symbolic links to keyboard and
#  mice of each seat. These symlinks will be used by Xephyr to associate each
#  seat to a keyboard and mouse exclusively.

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
    
    WriteDebug $"Initiating CallXephyr"
    
    # analyze all function given parameters
    while [[ ! -z "${1}" ]]; do
        # check XAUTHORITY
        if [[ "${1}" == "-xauthority" ]]; then
            shift
            if [[ ! -z "${1}" ]]; then
                export XAUTHORITY="${1}"
            fi
        # check display to be used by Xephyr
        elif [[ "${1}" == "-display" ]]; then
            shift
            if [[ ! -z "${1}" ]]; then
                export DISPLAY="${1}"
            fi
        # other parameters
        else
            if ! expr match ${1} 'vt[0-9][0-9]*' > /dev/null; then
                ARGS=("${ARGS[@]}" "${1}")
            fi
        fi
        # next parameter
        shift
    done
    
    WriteDebug $"Initiating Xephyr. Parameters: ${ARGS[@]}"
    
    # executes Xephyr
    ${Xephyr} "${ARGS[@]}" 2> ${LOG_DIR}/Xephyr_${DISPLAY}.log &
    
    local PID=$!
    
    echo ${PID} > ${PIDS}/Xephyr_${DISPLAY}.pid
   
    # wait for Xephyr end
    wait ${PID}
    
    # Xephyr end code
    local RETURN_CODE=$?
     
    # exclude PID
    rm -f ${PIDS}/Xephyr_${DISPLAY}.pid
    
    WriteDebug $"Xephyr killed. Return code: ${RETURN_CODE}"
} # CallXephyr
