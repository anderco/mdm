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

set -e

# XXX: get these from mdm-common

TARGET=tree/
BASE_DIR=/

MDM_LOGS=$BASE_DIR/var/log/mdm/
MDM_ETC=$BASE_DIR/etc/mdm/
MDM_SHARE=$BASE_DIR/usr/share/mdm/
MDM_SCRIPTS=$BASE_DIR/usr/sbin/

CONFIG_FILE=$MDM_ETC/mdm.conf

MDM_DEVICES=$MDM_ETC/devices/
MDM_MODES=$MDM_SHARE/modes/

INITD=/etc/init.d

rm -rf   $TARGET
mkdir -p $TARGET

mkdir -p $TARGET/$MDM_LOGS
mkdir -p $TARGET/$MDM_ETC
mkdir -p $TARGET/$MDM_SHARE
mkdir -p $TARGET/$MDM_SCRIPTS

mkdir -p $TARGET/$MDM_DEVICES

mkdir -p   $TARGET/$INITD
cp src/mdm $TARGET/$INITD

cp src/mdm-bin                  $TARGET/$MDM_SCRIPTS/
cp src/mdm-common               $TARGET/$MDM_SCRIPTS/
cp src/mdm-start-seat           $TARGET/$MDM_SCRIPTS/
cp src/create-xorg-conf         $TARGET/$MDM_SCRIPTS/
cp src/write-message            $TARGET/$MDM_SCRIPTS/
cp src/xephyr-wrapper           $TARGET/$MDM_SCRIPTS/
cp src/xephyr-parent-window     $TARGET/$MDM_SCRIPTS/
cp src/read-devices             $TARGET/$MDM_SCRIPTS/
cp src/discover-devices         $TARGET/$MDM_SCRIPTS/
cp misc/background.png          $TARGET/$MDM_SHARE/
cp -r modes                     $TARGET/$MDM_SHARE/
cp config/mdm.conf              $TARGET/$MDM_ETC/

