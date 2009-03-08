#!/bin/sh

#
# Author: ng <nicolas@guillaume-fr.net>
# 
# VNC control channel
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

lastid=100
line=''

while read cmd arg0 arg1
do

  case "$cmd" in
   '?'|'help'|'h')
       echo ' > GetNewChannelName: return an available channel name'
       echo ' > ConnectToVNC channel: connect local VNC to channel'
       echo ' > help'
          ;;
   'GetNewChannelName')
      (( lastid++ ))
      echo "${username}_VNC_${lastid}_$RANDOM$RANDOM"
	;;
   'ConnectToVNC')
      btun "${username}@${server}" -f $passfile $arg0 nc $vhost $vport & 
      echo "Ok"
	;;
   '')
	  ;;
   *)
      echo 'command `'$cmd"'"' not understood' ;;

  esac

  
done
