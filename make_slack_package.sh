#!/bin/sh


mkdir -p slack/install
mkdir -p slack/etc
mkdir -p slack/usr/bin

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
EOF

cat > slack/install/doinst.sh <<EOF
#/bin/sh
EOF

make mrproper || exit 1
make min || exit 1
cp -f daemon/daemon max_uptime/max_uptime range/range rsh/rsh xoror/xoror slack/usr/bin/
cp -f rsh/rsh.conf slack/etc/

name="ng-utils-revXX-$(uname -m).tgz"

cd slack
tar fcz "$name" *
mv "$name" ..
cd ..
rm -fr slack

echo Done package $name.
