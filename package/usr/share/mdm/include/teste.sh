#! /bin/bash

INCLUDE=./

source ${INCLUDE}/mdm-path.inc

source ${INCLUDE}/mdm-debug.inc
source ${INCLUDE}/mdm-util.inc
source ${INCLUDE}/mdm-hardware.inc

echo `IsRoot`

if [[ ! IsRoot -eq 1 ]]; then
    echo "Precisa ser root para executar este script."
    exit 1
fi
Keyboards
Keyboards event
echo
echo
Mice
Mice event
