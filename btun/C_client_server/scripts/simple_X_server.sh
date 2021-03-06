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
# btun_X_tunnel_client.sh & btun_X_tunnel_server.sh enable you to execute
# X application through a btun tunnel.
#
#
#
#                      btund
#                    /       \               
#                   /         \              
#                  /           \            
#                 /             \           
#                /               \          
#               |                |         
#      /      btun              btun      \   
#      |        |                |        |         
# (1) <        nc                nc        > (2)
#      |        |                |        | 
#      \  local X server    remote X app  /                  
#      
#
#   (1) simple_X_client.sh: btun + nc + local X server connection
#   (2) simple_X_server.sh: btun + nc + X app connection on remote host
#
#


if [ -z "$1" -o -z "$2" ]
then
    echo "usage: $0 user@host channel_prefix [btun option]" >&2
    exit 2
fi

user="$1"
channel="$2"
option="$3"
x_port_start=1

btun "$user" "$3" "${channel}_X_1" -- nc -l -p $(( ${x_port_start} + 6000 )) &

export DISPLAY=localhost:${x_port_start}.0

emacs

