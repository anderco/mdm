#
#  This file has necessary functions to create symbolic links to keyboards
#  and mice of each seat. These symbolic links will be used by Xephyr to 
#  associate each each seat to an only keyboard and mouse.

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
# This function associates a physical keyboard to an only seat
#
# Parameters:   seat number to associate keyboard
#

function KeyboardLink()
{

    local CREATED
    local TERMINAL=${1}
    local LINK
    
    WriteDebug $"Keyboard set, seat ${1}"
    
    if [[ -e ${LINK_DIR}/mdmKbd${1} ]]; then 
        CREATED=1
    else
        CREATED=0
    fi
    
    # check link creation mode
    case ${LINK_CREATION_MODE} in
        always)
            # ignore already created link
            CREATED=0
            ;;
        once)
            # if link is already created, close
            if [[ ${CREATED} -eq 1 ]]; then
                return
            fi
            ;;
    esac
    
    # ask for fn keypress
    SetImage keyboard ${TERMINAL}
    
    # repeat until get link created
    while (( ! ${CREATED} )); do
           
        # keyboard events
        local KEYB_EVENTS=$( Keyboards event | cut -f2 )
        
        # wait until get a keyboard connected
        if [[ -z "${KEYB_EVENTS}" ]]; then
            continue
        fi
        
        # wait a keypress
        local PRESSED=$( ${ReadDevices} ${TERMINAL} ${KEYB_EVENTS} | \
                         grep '^detecao' | \
                         cut -d '|' -f2 )
                        
        # check if a key has been pressed or time out
        if [[ ${PRESSED} != 'timeout' ]]; then
            
            WriteDebug $"Creating keyboard link."
            
            # create symbolic link
            ln -s ${PRESSED} ${LINK_DIR}/mdmKbd$1
            
            # set "created link" flag
            CREATED=1
            
            # check if there isn't another link pointing to the same keyboard.
            # if there is, exclude the just created one and keep waiting for the
            # next keypress
            for LINK in `ls ${LINK_DIR} | grep "\<mdmKbd"`; do
                          
                # compare found LINK whith the just create one
                if [[ ${LINK} != "mdmKbd${1}" ]]; then
                     
                    local AUX=$( stat -c %N ${LINK_DIR}/${LINK} | \
                                 cut -d'`' -f3 | \
                                 cut -d \' -f1 )
                    if [[ ${AUX} = ${PRESSED} ]]; then
                        # exclude created link, leaving the old one untouched
                        rm -f ${LINK_DIR}/mdmKbd${1}
                    
                        # set "not created link" flag
                        CREATED=0
                        
                        WriteDebug $"Keyboard connected to another head."
                                            
                        # this keyboard is already in use. Remove it from list
                        KEYB_EVENTS=`echo ${KEYB_EVENTS} | \
                                     sed "s;\<${AUX}\>;;g"`
                    fi
                fi
            done
        fi
    done
    
    WriteDebug $"Keyboard configured! Seat: ${TERMINAL}" 
    
} # KeyboardLink

#
# This function associates a physicall mouse to an only seat
#
# Parameters:   seat number to associate mouse with
#

function MouseLink()
{

     
    local CREATED
    local TERMINAL=${1}
    local LINK
    local LOCK
    
    WriteDebug $"Configuring mouse, seat ${1}"
    
    if [[ -e ${LINK_DIR}/mdmMouse${1} ]]; then 
        CREATED=1
    else
        CREATED=0
    fi
    
    # check link creation mode
    case ${LINK_CREATION_MODE} in
        always)
            # ignore already created link
            CREATED=0
            ;;
        once)
            # if the link is already created, close
            if [[ ${CREATED} -eq 1 ]]; then
                return
            fi
            ;;
    esac

    # repeat until get a mouse to associate with this seat
    while (( ! ${CREATED} )); do
    
        WriteDebug $"Waiting connected mouse."
        
        # listagem dos eventos de mouses conectados ao terminal
        # events list of mice connected to the seat
        local MICE_EVENTS=$( Mice event | cut -f2 )
        
        # repeat until get a connected mouse
        if [[ -z "${MICE_EVENTS}" ]]; then
            continue
        fi
        
        # create a lock file to stop current mouse clicks to other seats,
        # or many mice clicking at the same seat.
        local HAS_LOCK=1
        
        # repeat until find an available mouse
        while (( ${HAS_LOCK} )); do
        
            # create a lock file to seat currently being configured
            touch ${LOCK_DIR}/lock${1}
        
            # assume that there is no mouse associated with this seat
            HAS_LOCK=0
            
            # check all existing lock files
            for LOCK in `ls ${LOCK_DIR} | grep "\<lock"`; do
                if [[ ${LOCK} != "lock${1}" ]]; then
                    HAS_LOCK=1
                fi
            done
        
            # check if there is a mouse associated with this seat
            if (( HAS_LOCK )); then
                # show wait image
                SetImage wait
            
                # erase creatd lock file
                rm -f ${LOCK_DIR}/lock${1}
            
                # wait
                sleep 1
            fi
        done
        
        # show "press left mouse button" image...
        SetImage mouse 
        
        WriteDebug $"Waiting mouse button press. Head ${1}"
        
        # wait a mouse button press
        local PRESSED=$( ${ReadDevices} 13 ${MICE_EVENTS} | \
                         grep '^detecao' | 
                         cut -d'|' -f2 )
                        
        # check if there was a timeout
        if [[ "${PRESSED}" = "timeout" ]]; then
            # ask for waiting
            SetImage wait
            
            # remove lock file
            rm -f ${LOCK_DIR}/lock${1}
            
            # wait 5 seconds
            sleep 5
        else
            WriteDebug $"Pressed: ${PRESSED}"
            
            # create link
            ln -s ${PRESSED} ${LINK_DIR}/mdmMouse${1}
            
            # set link as created
            CREATED=1
            
            # Check if there isn't another link pointing to this mouse.
            # If there is, exclude ther recently created one and repeat the
            # whole process.

            for LINK in `ls ${LINK_DIR} | grep "\<mdmMouse"`; do
               # compare found LINK with the recently created one
            
                if [[ ${LINK} != "mdmMouse${1}" ]]; then
                     
                    local AUX=$(stat -c %N ${LINK_DIR}/${LINK} | \
                                cut -d'`' -f3 | \
                                cut -d \' -f1)
                            
                     if [[ ${AUX} = ${PRESSED} ]]; then
                        # exclude created link
                        rm -f ${LINK_DIR}/mdmMouse${1}
                        CREATED=0
                    
                        WriteDebug $"Mouse connected to another heaad."
                    
                        # remove it from available mice list
                        MICE_EVENTS=`echo ${MICE_EVENTS} | \
                                     sed "s;\<${AUX}\>;;g"`
                    fi
                fi
            done
            # exclude lock file
            rm ${LOCK_DIR}/lock${1}
        fi
    done
    
    WriteDebug $"Mouse Configured! Seat: ${TERMINAL}"
} # MouseLink

