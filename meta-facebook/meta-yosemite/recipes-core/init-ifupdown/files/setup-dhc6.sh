#!/bin/bash
#
# Copyright 2015-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#
if [ "$(/usr/bin/rikor-fru-util -b /tmp/rikor-fru-address -g'dhcp1')" = "dhcp" ] ; then

	echo -n "Setup dhclient for IPv6... "
	runsv /etc/sv/dhc6 > /dev/null 2>&1 &
	echo "done."

else

	logger "dhcp is switched off"
	echo "dhcp is switched off"

	read -a Q <<< `/usr/bin/rikor-fru-util -b /tmp/rikor-fru-address -g'ip1 netmask1 gate1'`
	ifconfig eth0 ${Q[0]} netmask ${Q[1]} up
	ip route add default via ${Q[2]} dev eth0

fi