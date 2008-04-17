#
#  This file has names and filepaths used by MDM
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
#                30/01/2008
#  Dependencies:
#       mdm-debug.inc

# Needed directories for Multiseat

PROC_DEVICES=/proc/bus/input/devices
BIN_DIR=/usr/sbin
SHARE_DIR=/usr/share/mdm
TEXT_DIR=${SHARE_DIR}/texts
TMP_DIR=/tmp/mdm
LINK_DIR=/dev/input
LOCK_DIR=${LINK_DIR}
IMAGE_DIR=${SHARE_DIR}/images
INCLUDE_DIR=${SHARE_DIR}/include
MDM_DIR=/etc/mdm
LOG_DIR=/var/log/mdm
DEBUG=/var/log/mdm
PIDS=/var/run/mdm
XORG=/etc/X11
GDM=/etc/gdm


# Arquivos do mdm
# Mdm files
CONFIG_FILE=${MDM_DIR}/mdm.conf
DEBUG_FILE=mdm.log

# Arquivos externos
# External files
GDM_CONF=${GDM}/gdm.conf
XORG_CONF=${XORG}/xorg.conf
XORG_MDM=${XORG}/xorg.conf.mdm
XAUTH_FILE=${LOG_DIR}/Xauth


# Sistemas auxiliares necessários para o funcionamento do mdm
# Auxiliary systems for mdm work
GDMDynamic=`which gdmdynamic`
Discover=`which discover`
Convert=`which convert`
Xephyr=`which Xephyr`
ShowImage=`which xsetbg`
X=`which X`
ReadDevices=${BIN_DIR}/mdm-read-devices
XephyrLoader=${BIN_DIR}/mdm-xephyr-loader
MDMService=${BIN_DIR}/mdm-service

#
# This function check the existence of system directories. Case they do not
# exist, they are created.
#
# Parameters:   none
#

function CreatePath()
{
    # Check if the log directory exists. If it does not, create it.
    if [[ ! -d ${DEBUG} ]]; then
        mkdir -p ${DEBUG}
        WriteDebug $"Log directory created!"
    fi
    
    # Check if de PIDs directory exists. If it does not, create it.
    if [[ ! -d ${PIDS} ]]; then
        mkdir -p ${PIDS}
        WriteDebug $"PIDs directory created!"
    fi
    
    # Check if the temp directory exists. If it does not, create it.
    if [[ ! -d ${TMP_DIR} ]]; then
        mkdir -p ${TMP_DIR}
        WriteDebug $"Temporary files directory created!"
    fi
} # CreatePath
