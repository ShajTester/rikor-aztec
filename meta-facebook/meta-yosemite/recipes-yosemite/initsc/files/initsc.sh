#!/bin/bash
#
# Copyright
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

### BEGIN INIT INFO
# Provides:          initsc
# Required-Start:    power-on
# Required-Stop:
# Default-Start:     S
# Default-Stop:
# Short-Description: Power on micro-server
### END INIT INFO

# Eventually, this will be used to configure the various (mostly
# i2c-based) sensors, once we have a kernel version that supports
# doing this more dynamically.
#
# For now, we're using it to install the lm75 and pmbus module so that it
# can detect the fourth temperature sensor, which is located
# on the uServer, which doesn't get power until power-on executes.
#
# Similarly, the pmbus sensor seems to have an easier time of
# detecting the NCP4200 buck converters after poweron.  This has not
# been carefully explored.


echo "X" > /dev/watchdog
echo "Watchdog disabled"

/usr/local/fbpackages/rikgpio/rikgpio

# Init value for pwrbtn and rstbtn
echo 1 > /sys/class/gpio/gpio195/value
echo 1 > /sys/class/gpio/gpio142/value
echo 1 > /sys/class/gpio/gpio141/value
echo 1 > /sys/class/gpio/gpio140/value

# Try reset PCH
echo 0 > /sys/class/gpio/gpio141/value
sleep 1
echo 1 > /sys/class/gpio/gpio141/value

# /usr/local/fbpackages/netconf/netconf_script.sh

# echo "netconf_script.sh done"
