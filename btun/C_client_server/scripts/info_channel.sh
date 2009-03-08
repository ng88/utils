#!/bin/bash

line=''

while read  line
do

  cmd="$line"

  case "$cmd" in
   'status')
       /etc/rc.d/rc.btund status ;;
   'uptime')
      uptime ;;
   '?'|'help'|'h')
       echo ' > status'
       echo ' > uptime'
       echo ' > help'
          ;;
   '')
	  ;;
   *)
      echo 'command `'$cmd"'"' not understood' ;;

  esac


  echo    $(date '+%F %X') $(hostname) '-- information channel'
  echo -n '?> '

done
