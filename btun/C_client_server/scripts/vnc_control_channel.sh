#!/bin/sh

#
# Author: ng <nicolas@guillaume-fr.net>
# 
# VNC control channel
#

if [ "$5" = "" ]
then	
    echo "usage: $0 username passfile btunserver vnc_host vnc_port machine" >&2
    exit 1
fi

username="$1"
passfile="$2"
server="$3"
vhost="$4"
vport="$5"
machine="$6"

lshell="bash"

lastid=100
line=''

while read cmd arg0 arg1
do

  case "$cmd" in
   '?'|'help'|'h'|'Help')
       echo 'Ok'
       echo ' > GetNewChannelName [base]: return an available channel name, base is optional'
       echo ' > ConnectToVNC channel: connect local VNC to channel'
       echo ' > ConnectToShell channel: connect a local shell to channel'
       echo ' > Help: show this help'
          ;;
   'GetNewChannelName')
      (( lastid++ ))
      base="$arg0"
      [ "$base" = "" ] && base="MSC"
      echo "${username}_${machine}_$base${lastid}_$RANDOM$RANDOM"
	;;
   'ConnectToVNC')
      if [ "$arg0" = "" ]
      then
	echo "Error: missing channel name argument"
      else
	btun -f $passfile "${username}@${server}" $arg0 nc $vhost $vport & 
	echo "Ok"
      fi
	;;
   'ConnectToShell')
      if [ "$arg0" = "" ]
      then
	echo "Error: missing channel name argument"
      else
	btun -f $passfile  -t $lshell "${username}@${server}" $arg0 & 
	echo "Ok"
      fi
	;;
   '')
	  ;;
   *)
      echo 'Error: command `'$cmd"'"' not understood' ;;

  esac

  
done
