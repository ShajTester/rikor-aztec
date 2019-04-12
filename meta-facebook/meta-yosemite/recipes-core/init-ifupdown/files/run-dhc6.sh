#!/bin/bash

if [ "$(/usr/bin/rikor-fru-util -b /tmp/rikor-fru-address -g'dhcp1')" = "dhcp" ]
then

	logger "run-dhc6.sh: dhclient -6 Started.."
	echo "run-dhc6.sh: dhclient -6 Started.."
	pid="/var/run/dhclient6.eth0.pid"
	exec dhclient -6 -d -D LL -pf ${pid} eth0 "$@"

else

	logger "dhcp is switched off"
	echo "dhcp is switched off"

	read -a Q <<< `/usr/bin/rikor-fru-util -b /tmp/rikor-fru-address -g'ip1 netmask1 gate1'`
	ifconfig eth0 ${Q[0]} netmask ${Q[1]} up
	ip route add default via ${Q[2]} dev eth0

fi
