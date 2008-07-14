#!/bin/bash

set -e

cd ../../mdm
make clean
make ubuntu-8.04
cp packages/mdm_0.0.1_i386.deb ../compiled/ubuntu-8.04/

cd ../extra-modes/xephyr-gdm/
make clean
make ubuntu-8.04
cp packages/mdm-xephyr-gdm_0.0.1_i386.deb ../../compiled/ubuntu-8.04/

cd ../../dependencies/ubuntu-8.04/
