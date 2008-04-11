#
# This file contains necessary functions to manage one head of multiseat.
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
# This function is responsible for getting mice and keyboard that will be 
# associated to current seat.
#
# Parameters:   seat number to associate mouse and keyboard with

function GetLinks()
{

    local TERMINAL=${1}
    
    # wait for a keyboard to the seat
    KeyboardLink ${TERMINAL}

    # wait for a mouse to the seat
    MouseLink ${TERMINAL}
    
    # tell user to wait
    SetImage wait
    
    # erase temporary images
    SetImage free   
     
} # GetLinks

#
# This function asks to user if the seat must be reconfigured, is say, if seat
# must get new keyboard and/or mice to be used.
#
# Parameters:   seat number to be reconfigured

function CheckReconfig()
{

    local TERMINAL=${1}
    
    # show seat reconfiguration image
    SetImage reconfig
    
    local PRESSED=$(${ReadDevices} 14 \
                    ${LINK_DIR}/mdmKbd${TERMINAL} | \
                    grep '^detecao' | \
                    cut -d'|' -f2)

    if [[ "${PRESSED}" = 'esc' ]]; then
        WriteDebug $"Reconfiguring remote seat."
                    
        # redo configuration
        rm -f ${LINK_DIR}/mdmKbd${TERMINAL}
        rm -f ${LINK_DIR}/mdmMouse${TERMINAL}
                    
        # get new mouse and keyboard to the seat
        GetLinks ${TERMINAL}
    fi
    
} # CheckReconfig
    
#
# This function is responsible for openning a seat of multiseat
#
# Parameters:   seat number to be openned

function OpenTerminal()
{

    local TERMINAL=${1}
    
    # obtém teclado e mouse para o terminal
    # get keyboard and mouse to the seat
    GetLinks ${TERMINAL}
    
    if [[ -z "${DISPLAY_MANAGER}" ]]; then
        DISPLAY_MANAGER=gdm
    fi
    
    case ${DISPLAY_MANAGER} in
        gdm)
            WriteDebug $"GDM is the display manager to be used."
            # XAUTHORITY gdm's file
            export XAUTHORITY=/var/lib/gdm/\:0.Xauth
            
            WriteDebug "${TERMINAL}=${XephyrLoader} \
                        -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                        -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                        -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                        -use-evdev -dpi 92 -audit 0 \
                        -display :0.$(( TERMINAL - 1 )) \
                        -xauthority ${XAUTHORITY} :${TERMINAL}"
            # creates entry to Xephyr server dinamically
            ${GDMDynamic}   -a "${TERMINAL}=${XephyrLoader} \
                            -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                            -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                            -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                            -use-evdev -dpi 92 -audit 0 \
                            -display :0.$(( TERMINAL - 1 )) \
                            -xauthority ${XAUTHORITY} :${TERMINAL}"
         
            while (( 1 )); do
                # invokes just-created server
                ${GDMDynamic} "-r ${TERMINAL}"
                WriteDebug $"Remote Xephyr ${TERMINAL} was killed!"

                # ask if the user wants to reconfigure the seat                
                CheckReconfig ${TERMINAL}
            done
            ;;
        remote)
            WriteDebug $"Remote server as display manager."
            
            # checks is there is a server name to invoke. If not, takes a 
            # default name. This option must be set at mdm.conf
            if [[ -z "${QUERY_COMMAND}" ]]; then
                QUERY_COMMAND="xdmcpserver"
            fi
            
            # repeats indefinitely
            while (( 1 )); do
                # invokes Xephyr with -query option for remote login
                ${XephyrLoader} -query ${QUERY_COMMAND} \
                                -geometry ${RESOLUTION[${TERMINAL}]}+0+0 \
                                -keyboard ${LINK_DIR}/mdmKbd${TERMINAL} \
                                -mouse ${LINK_DIR}/mdmMouse${TERMINAL},5 \
                                -use-evdev -dpi 92 -audit 0 \
                                -display :0.$(( TERMINAL - 1 )) \
                                -xauthority ${XAUTHORITY} :${TERMINAL}
                
                WriteDebug $"Local Xephyr ${TERMINAL} was killed!"
                
                # ask if the user wants to reconfigure seat
                CheckReconfig ${TERMINAL}
            done
            ;;
        *)
            echo $"Invalid Display Manager. Check mdm.conf and correct it."
            ;;
    esac
    
} # OpenTerminal
