#!/bin/bash

#
#  Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>
#
# Generalization of simple_X_client/server
#
# Typical usage:
#           on your local machine
#    ./X_client.sh login@host test1 passfile
#           on the server you want to control
#    ./X_server.sh login@host test1 passfile emacs xterm
#
#

if [ "$1" = SYNC ]
then
    while true
    do

      echo $2
      v=''
      read -t1 v

      [ "$v" = "OK" ] && exit 0
    done

fi


if [ -z "$1" -o -z "$2" -o -z "$3" ]
then
    echo "usage: $0 user@host channel_prefix passfile [cmd1 cmd2 ... cmdn]" >&2
    exit 2
fi

if ! [ -r "$3" ]
then
    echo "can't read passfile" >&2
    exit 2
fi

user="$1"
channel="$2"
pass="$3"
x_port_start=1
shift 3

i=0
for c in "$@"
do
  (( i++ ))
done

#synchro avec le client et transmission du nombre d'appli
btun "$user" -f "$pass" "${channel}_X_sync" -- $0 SYNC $i


pids=''

i=$(( ${x_port_start} - 1 ))
for c in "$@"
do
  (( i++ ))

  sleep 0.3

  btun "$user" -f "$pass" "${channel}_X_n$i" -- nc -l -p $(( 6000 + $i)) &
  pids="$! $pids"
  export DISPLAY=localhost:$i.0

  sleep 0.3
  
  $c &
  pids="$! $pids"
done


for c in "$@"
do
  fg > /dev/null 2>&1
  fg > /dev/null 2>&1
done

echo Press enter to stop
read
kill -TERM $pids > /dev/null 2>&1


