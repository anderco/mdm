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

# Run this script from "mdm/extra-modes/xephyr-gdm", not
# "mdm/extra-modes/xephyr-gdm/misc"!

NEW_VERSION=$1

sed -i "s/^VERSION = .*/VERSION = $1/" Makefile
sed -i "s/^Version: .*/Version: $1/" distro/debian-lenny/DEBIAN/control
sed -i "s/^Version: .*/Version: $1/" distro/ubuntu-8.04/DEBIAN/control
