#!/bin/bash

set -e

cd ../../mdm
make clean
make ubuntu-8.04
cp packages/*.deb ../compiled/ubuntu-8.04/

cd ../extra-modes/xephyr-gdm/
make clean
make ubuntu-8.04
cp packages/*.deb ../../compiled/ubuntu-8.04/

cd ../../dependencies/ubuntu-8.04/
