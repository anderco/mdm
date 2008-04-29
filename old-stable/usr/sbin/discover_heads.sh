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

#Esse script descobre os perifericos da maquina, como keyboard, mouse, 
#placas de video, eventos do mouse e kbd. Sem parametro, ele retorna uma 
#lista com os
#dispositivos, em duas colunas.  A primeira identifica o tipo de dispositivo,
#a segunda, o endereco fisico do dispositivo. Com parametro, retorna
#uma lista apenas dos dispositivos que sao do tipo especificado no prametro.
#parametros validos: mouse, kbd, driver, placa, mevent, kevent

PROC_DEVICES=/proc/bus/input/devices
DISCOVER=/sbin/discover
LOG_DIR=/var/log/mdm
DEBUG_LOG=$LOG_DIR/discover_head_debug.log
ETC_INSTALL_DIR=/etc/mdm/
MDM_CONF=$ETC_INSTALL_DIR/mdm.conf

source $MDM_CONF

#
# WRITE_DEBUG
#
# check if DEBUG_MULTISEAT is defined and is equals to 1. In this case,
# write a message in a debug file.
#
# Parameters: $1 = Text 1 to write
#             $2 = Text 2 to write (optional)
#
# Returns: nothing

function write_debug()
{
    if [[ $DEBUG_MULTISEAT -eq 1 ]]; then
        touch $DEBUG_LOG
        # write debug text to file
        echo -e "`date "+%D %R"` $1 $2\n" >> $DEBUG_LOG
    fi
} # write_debug

#
# MOUSE
#
# look in devices list for a mouse
#
# parameters:   none
# returns:      none

function mouse () {
    # vector with all physical address found in PROC_DEVICES
    ALL_PHYS=(`cat $PROC_DEVICES | grep P: | cut -f2 -d' '`)
    # vector with all handles found in PROC_DEVICES
    ALL_HANDLERS=(`cat $PROC_DEVICES | grep H: | cut -f2 -d' '`)
    # vector with device names found in PROC_DEVICES
    ALL_N=(`cat $PROC_DEVICES | grep N: | cut -f2 -d'=' |  tr -d ' '`)

    for ((i=0; i<${#ALL_PHYS[@]}; i=i+1))
    do
        #regra para detectar o mouse: handler contem mouse E 
        # endereco fisico terminar em 0
        if echo ${ALL_HANDLERS[i]} | grep "mouse" > /dev/null && echo ${ALL_PHYS[i]} | grep ".*0$" > /dev/null; then
            AUX=`echo ${ALL_PHYS[i]} | cut -f2 -d'='`
            echo -e "mouse\t$AUX"
        fi
    done
    # if no mouse found, put a serial mouse as default 
    if [[ ${#ALL_PHYS[@]} == 0 ]]; then
        echo -e "mouse\tserial0"
    fi
} # mouse

#
# KBD
#
# look in devices for a keyboard
#
# parameteres:  none
# returns:      none

function kbd(){

    ALL_PHYS=(`cat $PROC_DEVICES | grep P: | cut -f2 -d' '`)
    ALL_HANDLERS=(`cat $PROC_DEVICES | grep H: | cut -f2 -d' '`)
    ALL_N=(`cat $PROC_DEVICES | grep N: | cut -f2 -d'=' |  tr -d ' '`)

    for ((i=0; i<${#ALL_PHYS[@]}; i=i+1))
    do
        # rules to detect keyboard: word kbd is in handles and
        # word Speaker or Button are not in device and physical
        # address finish in 0.
        if  echo ${ALL_HANDLERS[i]} | \
            grep "kbd" > /dev/null && echo ${ALL_N[i]} | \
            egrep -v "(Speaker|Button)" > /dev/null && \
            echo ${ALL_PHYS[i]} | \
            grep ".*0$" > /dev/null ; then

	        AUX=`echo ${ALL_PHYS[i]} | cut -f2 -d'='`
	        echo -e "kbd\t$AUX"
  
        fi
    done
}

#
# PLACAS
#
# look in devices for video hardware (vga boards) 
#
# parameteres:  none
# returns:      none

function placas (){

    ALL_DRIVERS=(`$DISCOVER -t --data-path=xfree86/server/device/driver display`)

    ALL_BUS_IDS=(`lspci | grep "VGA" | cut -f1 -d' '`)

    for ((i=0; i<${#ALL_BUS_IDS[@]}; i=i+1)) ; do
	    if [[ "${ALL_DRIVERS[$i]}" = "" ]]; then 
            ALL_DRIVERS[$i]=vesa                    # VESA=default
        fi
        #busid from lspci is in format 00:00.00
        #below we divide in 00 and 00.00
        NUMS=(`echo ${ALL_BUS_IDS[$i]} |  \
             awk 'BEGIN {FS=":"}{print toupper($1), toupper($2)}'`)
		#now, we divide 00.00 in 00 and 00
        SEC_NUMS=(`echo ${NUMS[1]} |  \
                 awk 'BEGIN {FS="."}{print toupper($1), toupper($2)}'`)
		#now, we convert then numbers from hexa to decimal base 
        echo -e "bus\t`echo "obase=10;ibase=16;${NUMS[0]};${SEC_NUMS[0]};${SEC_NUMS[1]};" | bc | paste -s -d":"`"

    done
    
    for i in ${ALL_DRIVERS[@]}
    do
        echo -e "driver\t$i"
    done
} # placas

#
# MOUSE_EVENTS
#
# detect mouse events
#
# parameteres:  none
# returns:      none

function mouse_events(){

    ALL_PHYS=(`cat $PROC_DEVICES | grep P: | cut -f2 -d' '`)
    ALL_HANDLERS=(`cat $PROC_DEVICES | grep H: | cut -f2 -d' '`)
    ALL_EVENTS=(`cat $PROC_DEVICES | grep H: | sed 's/ /\n/g' | grep event`)
    usados=""
    for ((i=0; i < ${#ALL_PHYS[@]}; i++)); do
        # rules to detect mouse-events: in handler appears "mouse E"
        # or the physical address ends in 0, or physical address in
        # /inputX is not in use (it's necessary when mouse has input1
        # without input0).

        if  echo ${ALL_HANDLERS[i]} | \
            grep "mouse" > /dev/null && \
            (echo ${ALL_PHYS[i]} | grep ".*0$" > /dev/null || \
            ! grep -q "${ALL_PHYS[$i]}" <<< $usados ); then

            usados="$usados $(cut -d/ -f1 <<< ${ALL_PHYS[$i]})"
            echo -e "mevent\t/dev/input/${ALL_EVENTS[i]}"
        fi
    done
} # mouse_events

#
# KBD_EVENTS
#
# detect keyboard events
#
# parameteres:  none
# returns:      none

function kbd_events(){

    ALL_PHYS=(`cat $PROC_DEVICES | grep P: | cut -f2 -d' '`)
    ALL_HANDLERS=(`cat $PROC_DEVICES | grep H: | cut -f2 -d' '`)
    ALL_EVENTS=(`cat $PROC_DEVICES | grep H: | sed 's/ /\n/g' | grep event`)
    ALL_N=(`cat $PROC_DEVICES | grep N: | cut -f2 -d'=' |  tr -d ' '`)
    for ((i=0; i<${#ALL_PHYS[@]}; i=i+1)); do
        # rules do detect keyboard events: in handler appears "kbd E" and
        # in name has no "Speaker" and physical address ends in 0
        if  echo ${ALL_HANDLERS[i]} | grep "kbd" > /dev/null && \
            echo ${ALL_N[i]} | egrep -v "(Speaker|Button)" > /dev/null && \
            echo ${ALL_PHYS[i]} | grep ".*0$" > /dev/null ; then

            echo -e "kevent\t/dev/input/${ALL_EVENTS[i]}"
        fi
    done
} # kbd_events

# ******************** MAIN *************************

if [[ -z $DEBUG_MULTISEAT ]]; then
    DEBUG_MULTISEAT=0                       # no debug function
fi
#DEBUG_MULTISEAT=1	#force debug
if [[ "$#" = 0 ]]
then
    ARG=all  
else
    ARG=$1
fi

case $ARG in
    all)
        MOUSES=`mouse`
        KBDS=`kbd`
        PLACAS=`placas`
        KEVENT=`kbd_events`
        MEVENT=`mouse_events`
        echo "$MOUSES"
        echo "$KBDS"
        echo "$PLACAS"
        echo "$KEVENT"
        echo "$MEVENT"
        ;;
    mouse)
        MOUSES=`mouse`
        echo "$MOUSES"
        ;;
    kbd)
        KBDS=`kbd`
        echo "$KBDS"
        ;;
    placa)
        PLACAS=`placas`
        echo "$PLACAS"
        ;;
    bus)
        PLACAS=`placas`
        echo "$PLACAS" | grep bus
        ;;
    driver)
        PLACAS=`placas`
        echo "$PLACAS" | grep driver
        ;;
    kevent)
        KEVENT=`kbd_events`
        echo "$KEVENT" 
        ;;
    mevent)
        MEVENT=`mouse_events`
        echo "$MEVENT" 
        ;;
esac
