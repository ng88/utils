#!/bin/bash

#
#  WARNING:
#
#   This script is obsolete. Although it still works, you should consider
#   the use of tcpmux that is much more powerful and convenient.
#


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


if [ "$1" = SYNC ]
then
    v=''
    read  v
    echo $v > "$2"
    exit 0
fi


if [ -z "$1" -o -z "$2"  -o -z "$3" ]
then
    echo "usage: $0 user@host channel_prefix passfile" >&2
    exit 2
fi

xdisp=$(echo $DISPLAY | sed 's/[^:]*:\([0-9][0-9]*\).*/\1/g')
user="$1"
channel="$2"
pass="$3"
tmp=/tmp/XC.$$.$RANDOM

function ctrlc_handler()
{
    echo Exiting...
    kill -TERM $pids > /dev/null 2>&1
    exit 5
}

trap ctrlc_handler 2

# synchro avec le server et recuperation du nombre d'appli
echo -n >$tmp
btun "$user" -f "$pass" "${channel}_X_sync" -- $0 SYNC $tmp

nb=$(cat $tmp)

rm -f $tmp

if [ -z "$nb" ]
then
    echo sync error >&2
    exit 3
fi
#synchro ok

xhost +localhost > /dev/null 2>&1

pids=''

for (( i=1; i <= $nb; i++ ))
do
  btun -a "$user" -f "$pass" "${channel}_X_n$i" -- nc localhost $(( $xdisp + 6000 ))  &
  pids="$! $pids"
  sleep 0.2
done

btun "$user" -f "$pass" "${channel}_X_sync" << EOF
OK
EOF

echo Hit Ctrl+C to exit.
wait
