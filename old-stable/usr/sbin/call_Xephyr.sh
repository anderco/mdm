#!/bin/bash

# Copyright (C) 2004-2007 Centro de Computacao Cientifica e Software Livre
# Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
# USA.

# This script is called by gdm!

LOG_DIR=/var/log/mdm                    # log directory
DEBUG_LOG=$LOG_DIR/xephyr_debug.log     # call_Xephyr debug log file
ETC_INSTALL_DIR=/etc/mdm
MDM_CONF=$ETC_INSTALL_DIR/mdm.conf
PIDS=/var/run/mdm

source $MDM_CONF

#
# WRITE_DEBUG
#
# check if DEBUG_MULTISEAT is defined and is equals to 1. In this case,
# write a message in a debug file.
#
# Parameters: $1 = Text 1 to write
#             $2 = Text 2 to write
#
# Returns: nothing

function write_debug()
{
    if [[ $DEBUG_MULTISEAT -eq 1 ]]; then
	    touch $DEBUG_LOG
        # write debug text to file
        echo -e "`date "+%D %R"` $1 $2" >> $DEBUG_LOG    
    fi
} # write_debug

trap "" usr1

#******************** MAIN ********************

if [[ -z $DEBUG_MULTISEAT ]]; then
    DEBUG_MULTISEAT=0                       # no debug function
fi
#DEBUG_MULTISEAT=1 #force debug
write_debug "Starting Xephyr! Args: ${args[@]}"

XEPHYR=$(which Xephyr)    # find Xephyr

args=()

write_debug "$XEPHYR $*"

if [[ -z $XEPHYR ]]; then    # found Xephyr???
    echo -e "Error!!! Xephyr not found. Aborting...\n\n"
    write_debug "Xephyr not found. Please install Xephyr."
    exit 1
fi

while [[ ! -z "$1" ]]; do
    if [[ "$1" == "-xauthority" ]]; then
        shift
        if [[ ! -z "$1" ]]; then
            export XAUTHORITY="$1"
        fi
    elif [[ "$1" == "-display" ]]; then
        shift
        if [[ ! -z "$1" ]]; then
            export DISPLAY="$1"
        fi
    else
        if ! expr match $1 'vt[0-9][0-9]*' >/dev/null; then
            args=("${args[@]}" "$1")
        fi
    fi
    shift
done

$XEPHYR "${args[@]}"  2> $LOG_DIR/Xephyr_$DISPLAY.log & #run Xephyr
PID=$!
echo $PID > $PIDS/Xephyr$DISPLAY.pid
wait $PID
R=$?
rm -f $PID/Xephyr.$DISPLAY.pid

write_debug "Xephyr finished. Code: $R"
exit $R
