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

# XXX This is uglier than ../../mdm/misc/make-tree.s

TARGET=tree/
BASE_DIR=/

MDM_SHARE=$BASE_DIR/usr/share/mdm/
MDM_SCRIPTS=$BASE_DIR/usr/sbin/

MDM_MODES=$MDM_SHARE/modes/

GDM_CONF=/etc/gdm/

rm -rf   $TARGET
mkdir -p $TARGET

mkdir -p $TARGET/$MDM_SHARE
mkdir -p $TARGET/$MDM_SCRIPTS

mkdir -p $TARGET/$MDM_MODES

mkdir -p $TARGET/$GDM_CONF

cp src/xephyr-gdm		$TARGET/$MDM_MODES/
cp src/xephyr-wrapper		$TARGET/$MDM_SCRIPTS/

# Yes, 2 files.
cp gdm-config/gdm.conf-custom	$TARGET/$GDM_CONF/
cp gdm-config/gdm.conf-custom	$TARGET/$GDM_CONF/gdm.conf
