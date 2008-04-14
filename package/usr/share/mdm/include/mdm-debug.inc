#
# System LOG functions
# Funções de LOG do sistema
#
# Copyright (C) 2004-2008 Centro de Computacao Cientifica e Software Livre
# Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR#
#
# This is free software.  You may redistribute copies of it under the terms of
# the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.
# There is NO WARRANTY, to the extent permitted by law.

#**********************************
#
#  File Version: 1.0.0
#                30/01/2008
#  Dependencies:
#       none

#
# This function is responsible for writing a debug system log.
# It uses environmental variable $DEBUG_FILE as a log filename to be written.
#
# Parameters:   text to be written (mandatory)
#               text2 to be written (optional)
#

function WriteDebug()
{
    if [[ ! -z "${DEBUG_ENABLED}" &&  "${DEBUG_ENABLED}" -eq 1 ]]; then
        # creates log file, case it does not exist yet
        local DBG=${DEBUG}/${DEBUG_FILE}
        
        # writes log on debug file
        echo -e "`date "+%D %R"` $1 $2" >> ${DBG}
        
        # writes log on standard error output
        echo -e "$1" "$2" >&2
    fi
}

#
# This function writes a generic system log inside mdm_sys.log file
#
# Parameters:   text to be written (mandatory)
#               text2 to be written (optional)
#

function WriteLog()
{
    local LOG=${DEBUG}/mdm-sys.log
    
    # writes log on logs file
    echo -e "`date "+%D %R"` $1 $2" >> ${LOG}
}
