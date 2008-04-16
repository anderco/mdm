#
#  This file has necessary functions to show images to keyboard and mouse
#  selection for each seat.
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
#                19/02/2008
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

#
# This function show an image according to given parameter
#
# Parameters:   image type to be shown
#                   * keyboard      show image to press Fn
#                   * mouse         show image to press mouse button
#                   * wait          show image to wait
#                   * reconfig      show image to reconfigure seat
#                   * free          exclude temporary images
#
#               seat number being configured
#

function SetImage()
{
    local FILES
    local CONVERT_ARGS="-pointsize 22 -fill #000100 -font helvetica-bold"
    local TERMINAL=${2}
    local KEYB_MSG1=$"Press F${TERMINAL} key"
    local KEYB_MSG2=$"to keyboard associated"
    local KEYB_MSG3=$"to this monitor."
    local MOUSE_MSG1=$"Press left mouse button"
    local MOUSE_MSG2=$"associated to this"
    local MOUSE_MSG3=$"monitor"
    local RECONF_MSG1=$"The seat is configured!"
    local RECONF_MSG2=$"To reconfigure it, press ESC."
    local RECONF_MSG3=$"To continue, press ENTER."
    local WAIT_MSG=$"Wait..."
    
    case ${1} in
        free)
            WriteDebug $"Erasing temporary image files..."
            
            # find temporary image files
            for FILES in ${TMP_DIR}/f*.png \
                         ${TMP_DIR}/mouse.png \
                         ${TMP_DIR}/wait.png \
                         ${TMP_DIR}/reconfig.png
            do
                # check if it is a common file
                if [[ -f ${FILES} ]]; then
                    # check if file was removed and writes on log
                    if rm ${FILES}; then
                        WriteDebug $"File ${FILES} removed."
                    else
                        WriteDebug $"Error while erasing file ${FILES}."
                    fi
                fi
            done
            ;;
        keyboard)
            WriteDebug $"Press F${TERMINAL} key ..."
            
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/F${TERMINAL}.png ]]; then
                # image does not exist. Create it
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 285,365 \"${KEYB_MSG1}\"" \
                           -draw "text 285,390 \"${KEYB_MSG2}\"" \
                           -draw "text 285,415 \"${KEYB_MSG3}\"" \
                           ${IMAGE_DIR}/keyboard.png \
                           ${TMP_DIR}/F${TERMINAL}.png
                 WriteDebug $"Image for F${TERMINAL} seat created."
            fi
            # show image on screen
            ${ShowImage} -fullscreen ${TMP_DIR}/F${TERMINAL}.png
            ;;
        mouse)
            WriteDebug $"Image: Press button ... seat ${TERMINAL}"
            
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/mouse.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 300,365 \"${MOUSE_MSG1}\"" \
                           -draw "text 300,390 \"${MOUSE_MSG2}\"" \
                           -draw "text 300,415 \"${MOUSE_MSG3}\"" \
                           ${IMAGE_DIR}/mouse.png \
                           ${TMP_DIR}/mouse.png
                WriteDebug $"mouse.png image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/mouse.png
            ;;
        wait)
            WriteDebug $"Image: Wait..."
            
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/wait.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 285,365 \"${WAIT_MSG}\"" \
                           ${IMAGE_DIR}/wait.png \
                           ${TMP_DIR}/wait.png
                WriteDebug $"wait.png image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/wait.png
            ;;
        reconfig)
            WriteDebug $"Image: Press ESC..."
            
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/reconfig.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 300,365 \"${RECONF_MSG1}\"" \
                           -draw "text 300,390 \"${RECONF_MSG2}\"" \
                           -draw "text 300,415 \"${RECONF_MSG3}\"" \
                           ${IMAGE_DIR}/reconfig.png \
                           ${TMP_DIR}/reconfig.png 
                WriteDebug $"Reconfiguration image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/reconfig.png
            ;;
        *)
            WriteDebug $"SetImage() - Invalid Option!!!"
            ;;
    esac
} # SetImage
