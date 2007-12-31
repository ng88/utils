#!/bin/sh

rm -fr slack
mkdir -p slack/install

cat > slack/install/slack-desc <<EOF
ng-utils: ng-utils (misc unix tools)
ng-utils: 
ng-utils: These tools are provied:
ng-utils:  rsh - a Restricted SHell
ng-utils:  xoror - an advanced xor encrypter
ng-utils:  max_uptime - an uptime logger
ng-utils:  daemon - start any program as a daemon
ng-utils:  range - an integer range producer
ng-utils: 
ng-utils: 
ng-utils: 
EOF
chmod a+r slack/install/slack-desc

cat > slack/install/doinst.sh <<EOF
#/bin/sh

EOF
chmod a+x slack/install/doinst.sh

make mrproper || exit 1
make daemon max_uptime range rsh xoror $* || exit 1
make installnobtun $* prefix=slack || exit 1

rev=$(sed -n 's/.*revision="\([^"]*\)".*/\1/gp' .svn/entries | sort -nr | head -1)
name="ng-utils-rev$rev-$(uname -m).tgz"

cd slack
makepkg -l y -p -c n  "$name"
mv "$name" ..
cd ..
rm -fr slack

echo Done package $name.
