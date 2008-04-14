#
#  Functions for hardware detection on mdm
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
#                31/01/2008
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

#
# Discover keyboards available on system.
#
# Parameters:   one (optional)
#                   * all       show default keyboard information (default)
#                   * event     show only keyboard associated events
#
# Output:       print on standard output keyboard and mice found on system
#

function Keyboards()
{
    # check given parameters
    if [[ -z "${1}" ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
        
    # Select physical devices (addresses)
    local ALL_PHYSICAL=(`cat ${PROC_DEVICES} | \
                         grep P: | \
                         cut -d' ' -f2`)
    
    # Select device controllers
    local ALL_HANDLERS=(`cat ${PROC_DEVICES} | \
                         grep H: | \
                         cut -d' ' -f2`)
    
    # Select device events
    local ALL_EVENTS=(`cat ${PROC_DEVICES} | \
                       grep H: | \
                       sed 's/ /\n/g' | \
                       grep event`)
    
    # Select device names
    local ALL_NAMES=(`cat ${PROC_DEVICES} | \
                      grep N: | \
                      cut -d'=' -f2 | \
                      tr -d ' '`)

    # Rules for keyboard detection:
    #  * Word kbd must be in controller
    #  * Word Speaker or Button must not be in device name
    #  * Device physical address must end at 0.
    for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
    do
       if  echo ${ALL_HANDLERS[i]} | \
           grep "kbd" > /dev/null && \
           echo ${ALL_NAMES[i]} | \
           egrep -v "(Speaker|Button)" > /dev/null && \
           echo ${ALL_PHYSICAL[i]} | \
           grep ".*0$" > /dev/null ; then

           local AUX=`echo ${ALL_PHYSICAL[i]} | cut -f2 -d'='`
           case ${TYPE} in
                all)
                    echo -e "kbd\t${AUX}"
                    ;;
                event)
                    echo -e "kevent\t/dev/input/${ALL_EVENTS[i]}"
                    ;;
           esac
        fi
    done
} # Keyboards

#
# Discover mice available on system.
#
# Parameters:   output type (opcional)
#                   * all       show default information of mouse (default) 
#                   * event     show only mouse associated events
#
# Output:       print on standard output mice found on system
#

function Mice()
{
    # check given parameter
    if [[ -z "${1}" ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
    
    # Select physical devices (addresses)
    local ALL_PHYSICAL=(`cat $PROC_DEVICES | \
                         grep P: | \
                         cut -d' ' -f2`)
    
    # Select device controllers
    local ALL_HANDLERS=(`cat $PROC_DEVICES | \
                         grep H: | \
                         cut -d' ' -f2`)
    
    # Select device events
    local ALL_EVENTS=(`cat ${PROC_DEVICES} | \
                       grep H: | \
                       sed 's/ /\n/g' | \
                       grep event`)

    case ${TYPE} in
        all)
            # Rules for mouse detection:
            #   * handler must contain mouse
            #   * Physical address must end at 0.
            for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
            do
                if echo ${ALL_HANDLERS[i]} | \
                    grep "mouse" > /dev/null && \
                    echo ${ALL_PHYSICAL[i]} | \
                    grep ".*0$" > /dev/null; then

                    local AUX=`echo ${ALL_PHYSICAL[i]} | cut -d'=' -f2`
                    echo -e "mouse\t${AUX}"
                fi
            done
            ;;
        event)
            # already used events
            local USED=""

            # Rules to detect mouse events:
            #   * handler must contain "mouse E"
            #   * Physical address must end at 0
            #   * Physical address in /inputX is not in use
            #     (it is necessary when mouse has input1 without input0).
            for (( i=0; i < ${#ALL_PHYSICAL[@]}; i=i+1 ))
            do
                if echo ${ALL_HANDLERS[i]} | \
                    grep "mouse" > /dev/null && \
                    (echo ${ALL_PHYSICAL[i]} | \
                    grep ".*0$" > /dev/null || \
                    ! grep -q "${ALL_PHYSICAL[i]}" <<< ${USED}); then

                    USED="${USED} $(cut -d/ -f1 <<< ${ALL_PHYSICAL[i]})"
                    echo -e "mevent\t/dev/input/${ALL_EVENTS[i]}"
                fi
            done
            ;;
    esac 
    # If no mice were found, takes serial as default
    if [[ ${#ALL_PHYSICAL[@]} -eq 0 ]]; then
        echo -e "mouse\tserial0"
    fi
} # Mice

#
# Discover video cards installed on computer
#
# Parameters:   1 optional.
#               that might be: driver, bus or all. all is default
# Output:       print on standard output BUS address from video card
#               print on standard output video driver to be used
#
function VideoCards()
{
    # check given parameter
    if [[ $# -ne 1 ]]; then
        local TYPE=all
    else
        local TYPE=${1}
    fi
        
    # Use discover to find drivers for devices
    local ALL_DRIVERS=(`${Discover} --type-summary \
                                    --data-path=xfree86/server/device/driver \
                                    display 2> /dev/null`)

    # obtain video cards list installed on computer
    local ALL_BUS_IDS=(`lspci | grep "VGA" | cut -f1 -d' '`)
    
    for (( i=0; i < ${#ALL_BUS_IDS[@]}; i=i+1 )); do
        # Check if a driver was detected for device
        # If did not found the driver, takes VESA driver
        if [[ "${ALL_DRIVERS[$i]}" = "" ]]; then 
            ALL_DRIVERS[$i]=vesa
        fi
        
        # BUSID returned by lspci command are in 00:00.00 format
        # lets divide busid in 00 and 00.00
        NUMS=(`echo ${ALL_BUS_IDS[$i]} |  \
             awk 'BEGIN {FS=":"}{print toupper($1), toupper($2)}'`)

        # Now, lets divide 00.00 in 00 and 00
        SEC_NUMS=(`echo ${NUMS[1]} |  \
                   awk 'BEGIN {FS="."}{print toupper($1), toupper($2)}'`)
                   
        # We already have numbers divided in group, lets convert from 
        # Hexadecimal to Decimal
        if [[ ${TYPE} = "all" ]]; then
            echo -e "bus\t`echo "obase=10;ibase=16;${NUMS[0]};\
                    ${SEC_NUMS[0]};${SEC_NUMS[1]};" | bc | paste -s -d":"`"
        elif [[ ${TYPE} = "bus" ]]; then
            echo "obase=10;ibase=16;${NUMS[0]};${SEC_NUMS[0]};${SEC_NUMS[1]};" \
                 | bc | paste -s -d":"
        fi
    done
    
    # Finally, we write found drivers
    for i in ${ALL_DRIVERS[@]}
    do
        if [[ ${TYPE} = "all" ]]; then
            echo -e "driver\t${i}"
        elif [[ ${TYPE} = "driver" ]]; then
            echo ${i}
        fi
    done
} # VideoCards

#
# Set number of heads to be used
#
# Parameters:   none
# Output:       print on standard output the head number to be used
#

function NumHeads()
{
   
    # take head number as same as video card number
    local HEADS=`VideoCards driver | wc -l`
    
    # check if HEADS variable has a true value
    if [[ -z ${HEADS} || ${HEADS} -lt 1 ]]; then
        HEADS=1
    fi
    
    # writes detected head number
    echo "${HEADS}"
} # NumHeads
    
