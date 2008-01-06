#!/bin/sh

rev=$(sed -n 's/.*revision="\([^"]*\)".*/\1/gp' .svn/entries | sort -nr | head -1 2>/dev/null)

[ "$rev" = "" ] && rev=$(sed -n 's/^\([0-9][0-9]*\)$/\1/gp' .svn/entries | sort -nr | head -1 2>/dev/null)
[ "$rev" = "" ] && rev='<unknow>'

echo $rev
