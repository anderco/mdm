#!/bin/bash

set -e

cd ../../mdm
make clean
make debian-lenny
cp packages/mdm_0.0.1_i386.deb ../compiled/debian-lenny/

cd ../extra-modes/xephyr-gdm/
make clean
make debian-lenny
cp packages/mdm-xephyr-gdm_0.0.1_i386.deb ../../compiled/debian-lenny/

cd ../../dependencies/debian-lenny/
