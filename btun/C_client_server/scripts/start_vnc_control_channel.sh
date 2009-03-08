#!/bin/sh

#
# Author: ng <nicolas@guillaume-fr.net>
# 
# Start the VNC control channel
#

if [ "$5" = "" ]
then	
    echo "usage: $0 username passfile btunserver vnc_host vnc_port" >&2
    exit 1
fi

username="$1"
passfile="$2"
server="$3"
vhost="$4"
vport="$5"

exec btun "${username}@${server}"  -m -f $passfile "${username}_VNCControlChannel" ./vnc_control_channel.sh "$username" "$passfile" "$server" "$vhost" "$vport"

