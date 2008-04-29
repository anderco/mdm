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

# This script asks the user to press F$1 and the mouse button, so it can
# discover which device belongs to which head. After the discovery, it starts
# Xephyr using the display manager.
#
# The argument we receive is the number of the "F key" the user must press!
#
# WARNING: this script is NOT responsible for setting the "DISPLAY" environment
# variable!

SBIN_INSTALL_DIR=/usr/sbin
SHARE_INSTALL_DIR=/usr/share/mdm
ETC_INSTALL_DIR=/etc/mdm
TMP_IMAGES=/tmp/mdm
LINKS_PATH=/dev/input
IMAGES=$SHARE_INSTALL_DIR/images
LOG_DIR=/var/log/mdm
LOCK=$LINKS_PATH
PIDS=/var/run/mdm

XAUTH_FILE=$LOG_DIR/Xauth
DISCOVER_HEADS=$SBIN_INSTALL_DIR/discover_heads.sh
READ_DEVICES=$SBIN_INSTALL_DIR/read_devices
MDM_CONF=$ETC_INSTALL_DIR/mdm.conf
MESSAGES=$SBIN_INSTALL_DIR/mdm_messages.sh
MDM_CONF=$ETC_INSTALL_DIR/mdm.conf
DEBUG_LOG=$LOG_DIR/configure_head_debug.log

KEY_NUMBER=$1   # head number, keyboard number, mouse number

XEPHYR=$SBIN_INSTALL_DIR/call_Xephyr.sh
SHOW_IMAGE=$(which xsetbg)
SHOW_IMAGE_ARGS="-fullscreen"

GDMDYNAMIC=$(which gdmdynamic)

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
    if [ $DEBUG_MULTISEAT -eq 1 ]; then
        touch $DEBUG_LOG
        # write debug text to file
        echo -e "`date "+%D %R"` $1 $2" >> $DEBUG_LOG     
    fi
} # write_debug

#
# CHECK_SYSTEM
#
# Check if packages needed by mdm are present.
#
# Parameters: nothing
# Returns:    nothing

function check_system()
{
    if [[ -z $SHOW_IMAGE ]]; then
        ERR_MSG="xsetbg not found! Install xloadimage package!"
        echo $ERR_MSG
        write_debug "$ERR_MSG"
        exit 1
    fi
    if [[ -z $GDMDYNAMIC ]]; then
        ERR_MSG="gdmdynamic not found! Install gdm package!"
        echo $ERR_MSG
        write_debug "$ERR_MSG"
        exit 1
    fi
} # check_system

#
# SHOW_IMAGE
#
# Displays an image to user, asking for press Fn key ou 
# mouse's left button 
#
# Parameters: $1 action to do
#                   key_press       ask for Fn key
#                   mouse_press     ask for left button
#                   wait            display "wait..." message
#                   free            remove 'trash' (unused temp files)
# Returns:    nothing

function show_image () {
    
    case $1 in
        key_press)
            write_debug "Displaying press Fn key..."
            $SHOW_IMAGE $SHOW_IMAGE_ARGS $IMAGES/F$KEY_NUMBER.png &
            ;;
        mouse_press)
            write_debug "Displaying press left button..."
            $SHOW_IMAGE $SHOW_IMAGE_ARGS $IMAGES/config_mouse.png &
            ;;
        wait)
            write_debug "Displaying wait message..."
            $SHOW_IMAGE $SHOW_IMAGE_ARGS $IMAGES/aguarde.png &
            ;;
        free)
            write_debug "Erasing temp files..."
            # We have multiple instances of this script running, 
            # so a lot of scripts
            # will use "mouse.png" and "wait.png", but if we 
            # don't erase them, no one will!
            for i in $TMP_IMAGES/f$KEY_NUMBER.png \
                     $TMP_IMAGES/mouse.png \
                     $TMP_IMAGES/wait.png
            do
                if [[ -f $i ]]; then
                    write_debug "Erasing $i"
                    rm -f $i
                fi
            done
            ;;
        reconfig)
	    write_debug "Displaying reconfigure message ..."
            $SHOW_IMAGE $SHOW_IMAGE_ARGS $IMAGES/term_config.png &
            ;;
        *)
            echo "You bad programmer!"
            ;;
    esac
} # show_image

# ******************** MAIN *************************

if [[ -z $DEBUG_MULTISEAT ]]; then
    DEBUG_MULTISEAT=0                       # no debug function
fi
#DEBUG_MULTISEAT=1 #force debug
write_debug "Entering configure_head. Head $1"

check_system                    # verify software needed by this module

write_debug "System Ok. Continuing configure_head..."

source $MESSAGES                # include mdm messages

write_debug "Creating $TMP_IMAGES..."
mkdir -p $TMP_IMAGES            # home for temp images

write_debug "Display Manager: $DISPLAY_MANAGER"
case $DISPLAY_MANAGER in
    gdm)
        write_debug "GDM as display manager"
        export XAUTHORITY=/var/lib/gdm/\:0.Xauth
        ;;
    remote)
        write_debug "Remote display manager"
        
        ;;
    *)
        write_debug "Unknow display manager"
        echo "Error parsing the config file! Invalid DISPLAY_MANAGER."
        ;;
esac


write_debug "Calling show_image for key_press"
show_image key_press            # ask user for press Fn key

if [[ -e $LINKS_PATH/mdmKbd$1 ]]; then
    CREATED=1
    write_debug "Keyboard Link $1 created."
else
    CREATED=0
    write_debug "Keyboard Link $1 not created."
fi

# loop to set keyboard
#
write_debug "Entering keyboard loop. Head $1"
while (( ! CREATED )); do

    # Who are the keyboards?
    KEYBOARDS=$($DISCOVER_HEADS kevent | cut -f2)

    if [[ -z $KEYBOARDS ]]; then
    	continue
    fi

    # When I see that my key has been pressed:
    PRESSED=$($READ_DEVICES $1 $KEYBOARDS | grep '^detecao' | cut -d'|' -f2)
    
    if [[ -z $PRESSED ]]; then
    	continue
    fi

    if [[ $PRESSED = 'timeout' ]]; then
        write_debug "Time-Out. Head $1"
        true
    else
        # Create the link
        write_debug "Creating Keyboard Link $1..."
        export XAUTHORITY=/var/lib/gdm/\:0.Xauth

        ln -sf $PRESSED $LINKS_PATH/mdmKbd$1
        CREATED=1

        # Verify if there is already ANOTHER link in $LINKS_PATH/mdmKbd* that
        # points to the device. If there is, erase the one I created and
        # continue in this loop.
        for i in `ls $LINKS_PATH | grep "\<mdmKbd"`; do
            # XXX do this in a better way
            if [[ $i != mdmKbd$1 ]]; then
                AUX=$(stat -c %N $LINKS_PATH/$i | cut -d'`' -f3 | cut -d \' -f1)
                if [[ $AUX = $PRESSED ]]; then
                    write_debug "Keyboard link already exists. Head $1"
    	            rm -f $LINKS_PATH/mdmKbd$1
    	            CREATED=0
    	            # This keyboard is already used! Remove from the list!
    	            KEYBOARDS=`echo $KEYBOARDS | sed "s;\<$AUX\>;;g"`
                fi
            fi
        done
    fi
    # XXX Even with this policy, is it possible for 2 seats to have the same
    # device?
done
write_debug "Exiting keyboard loop. Head $1"

# Time to configure the mouse!
# Do everything just like above...

if [[ -e $LINKS_PATH/mdmMouse$1 ]]; then
    CREATED=1
    write_debug "Mouse Link $1 created."
else
    CREATED=0
    write_debug "Mouse Link $1 not created."
fi

# loop to set mouse
#
write_debug "Entering mouse loop. Head $1"
while (( ! CREATED )); do

    # Who are the mice and keyboards?
    MICE=$($DISCOVER_HEADS mevent | cut -f2)

    if [[ -z $MICE ]]; then
       continue
    fi

    # Create the lock!
    HAS_LOCK=1
    write_debug "Creating mouse lock, head $1"
    while (( HAS_LOCK ));  do
        touch $LOCK/lock$1
        HAS_LOCK=0
        for i in `ls $LOCK | grep "\<lock"`; do
            if [[ $i != lock$1 ]]; then
                HAS_LOCK=1
            fi
        done
        if (( HAS_LOCK )); then
            # Yes, we'll call this a lot of times...
            show_image wait
            write_debug "Waiting for free lock. $1"
            rm -f $LOCK/lock$1
            sleep 1
        fi
    done

    show_image mouse_press      # ask user for mouse's left button
    write_debug "Waiting user click, head $1"
    # When I see that the button was pressed:
    PRESSED=$($READ_DEVICES 13 $MICE | grep '^detecao' | cut -d'|' -f2)

    if [[ -z $PRESSED ]]; then
    	continue
    fi

    if [[ "$PRESSED" = 'timeout' ]]; then
        write_debug "Mouse TimeOut. Head $1"
        # Wait 5 seconds to give other machines the opportunity to enter the
        # lock!
        show_image wait
        rm -f $LOCK/lock$1
        sleep 5
    else
        # echo "PRESSED = $PRESSED"
        # Create the link
        write_debug "Creating Mouse link $1..."
        ln -sf $PRESSED $LINKS_PATH/mdmMouse$1
        CREATED=1

        # Verify if there is already ANOTHER link in $LINKS_PATH/mdmKbd* that
        # points to the device. If there is, erase the one I created and
        # continue in this loop.
        for i in `ls $LINKS_PATH | grep "\<mdmMouse"`; do
            # XXX do this in a better way
            if [[ $i != mdmMouse$1 ]]; then
                AUX=$(stat -c %N $LINKS_PATH/$i | cut -d'`' -f3 | cut -d \' -f1)
                if [[ $AUX = $PRESSED ]]; then
                    rm -f $LINKS_PATH/mdmMouse$1
                    CREATED=0
                    write_debug "mouse link already exists, head $1"
                    # This is already in use! Remove from the list!
                    MICE=`echo $MICE | sed "s;\<$AUX\>;;g"`
                fi
            fi
        done
        rm $LOCK/lock$1
    fi
done
write_debug "Exiting mouse loop, head $1"

show_image wait
show_image free     # delete unused images

case $DISPLAY_MANAGER in
    gdm)
        #echo "gdmdynamic -v -a $1=$SBIN_INSTALL_DIR/call_Xephyr.sh" \
        #     " -geometry ${RESOLUTION}+0+0" \
        #     " -keyboard $LINKS_PATH/mdmKbd$1"\
        #     " -mouse $LINKS_PATH/mdmMouse$1,5"\ 
        #     " -use-evdev -dpi 92 -audit 0"
        write_debug "gdm local. Calling gdm dynamic. Head $1"
        $GDMDYNAMIC -a "$1=$XEPHYR \
                    -geometry ${RESOLUTION[$1]}+0+0 \
                    -keyboard $LINKS_PATH/mdmKbd$1 \
                    -mouse $LINKS_PATH/mdmMouse$1,5 \
                    -use-evdev -dpi 92 -audit 0 \
                    -display :0.$(($1  -1)) \
                    -xauthority /var/lib/gdm/\:0.Xauth" 
        
        write_debug "-a $1=$XEPHYR $1" 
        write_debug "-geometry ${RESOLUTION[$1]}+0+0" 
        write_debug "-keyboard $LINKS_PATH/mdmKbd$1" 
        write_debug "-mouse $LINKS_PATH/mdmMouse$1,5" 
        write_debug "-use-evdev -dpi 92 -audit 0" 
        write_debug "-display :0.$(($1  -1))" 
        write_debug "-xauthority /var/lib/gdm/\:0.Xauth"
		
		write_debug "gdmdynamic run call"
        $GDMDYNAMIC "-r $1"
		write_debug "gdmdynamic called."
        ;;
    remote)
        write_debug "Remote service. Head $1"        
        while (( 1 )); do
            $XEPHYR -query $QUERY_COMMAND \
                    -geometry ${RESOLUTION[$1]}+0+0 \
                    -keyboard $LINKS_PATH/mdmKbd$1 \
                    -mouse $LINKS_PATH/mdmMouse$1,5 \
                    -use-evdev -dpi 92 -audit 0 \
                    -display :0.$(($1  -1)) \
                    -xauthority /var/lib/gdm/\:0.Xauth :$1 
            write_debug "Xephyr $1 killed!"
            show_image reconfig     # ask for reconfiguration
            PRESSED=$($READ_DEVICES 14 $LINKS_PATH/mdmKbd$1 | grep '^detecao' | cut -d'|' -f2)
            if [[ $PRESSED  = 'esc' ]]; then
                # Redo configuration
                rm -f $LINKS_PATH/mdmKbd$1
                rm -f $LINKS_PATH/mdmMouse$1
                exec $0 $1
            fi
        done
	write_debug "Loop broken."
        ;;
    *)
        echo "Error parsing the config file! Invalid DISPLAY_MANAGER."
        ;;
esac

write_debug "Exiting configure_head. Head $1"

exit 0
