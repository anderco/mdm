#!/bin/bash

set -e

cd ../../mdm
make clean
make debian-lenny
cp packages/*.deb ../compiled/debian-lenny/

cd ../extra-modes/xephyr-gdm/
make clean
make debian-lenny
cp packages/*.deb ../../compiled/debian-lenny/

cd ../../dependencies/debian-lenny/
