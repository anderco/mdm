#
#  MDM system utilities
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
#       mdm-path.inc
#       mdm-debug.inc

#
# This function checks if the user is root.
#
# Parameters:   none
# 
# Output:       writes onto standard output:
#                   0   common user
#                   1   superuser
#

function IsRoot()
{
    if [[ `id -u` -eq 0 ]]; then
        echo 1
    else
        echo 0
    fi
} # IsRoot

#
# This function checks if necessary MDM system programs are installed for the
# perfect system work.
#
# Parameters:   none
#

function CheckSystem()
{
    # Set log filename to be written
    local DEBUG_FILE=System.log
    
    # Check if Discover is installed
    if [[ -z "${Discover}" ]]; then
        WriteDebug $"Driver detection disabled."\
                   $"Discover not found."
    fi
    
    # Check if GDMDynamic is installed
    if [[ -z "${GDMDynamic}" ]]; then
        WriteDebug $"GDM Dynamic not found. Please, install it."
        exit 1
    fi
    
    # Check if Xephyr is installed
    if [[ -z "${Xephyr}" ]]; then
        WriteDebug $"Xephyr not found. Please, install it."
        exit 1
    fi
    
    # Check if X is installed
    if [[ -z "${X}" ]]; then
        WriteDebug $"X not found. Please, install it." 
        exit 1
    fi
    
    # Check if xsetbg is installed
    if [[ -z "${ShowImage}" ]]; then
        WriteDebug $"xsetbg not found. Install xloadimage package."
        exit 1
    fi
} # CheckSystem

#
# This function checks if some essential variables for MDM routines execution
# were set. In case they were not set in the configuration files, set them with
# default value.
#

function InitSystem()
{

    # initiate DEBUG mode as off
    if [[ -z "${DEBUG_ENABLED}" ]]; then
        DEBUG_ENABLED=0
    fi
    
    # initiate links creation mode
    if [[ -z "${LINK_CREATION_MODE}" ]]; then
        LINK_CREATION_MODE="once"
    fi
    
    # initiate login manager
    if [[ -z "${DISPLAY_MANAGER}" ]]; then
        DISPLAY_MANAGER="gdm"
    fi
    
    # initiate remote server
    if [[ -z "${QUERY_COMMAND}" ]]; then
        QUERY_COMMAND="xdmcpserver"
    fi
    
    # Create MDM necessary directories 
    CreatePath
    
    # Check if needed programs are correctly installed
    CheckSystem
} # InitSystem

#
# This funtion writes Copyright texts of MDM system
#
# Parameters:   none

function MDMCopyright()
{

    echo "Multiseat Display Manager. Version ${VERSION}"
    echo "Copyright (c) 2004-2008 Universidade Federal do Parana (UFPR)"
    echo "Centro de Computacao Cientifica e Software Livre (C3SL)"
    echo
    echo "This is free software."
    echo "You may redistribute copies of it under the terms of the"
    echo "GNU General Public License <http://www.gnu.org/licenses/gpl.html>."
    echo "There is NO WARRANTY, to the extent permitted by law."
    echo
    
} # MDMCopyright
