#!/bin/sh


mkdir -p slack/install
mkdir -p slack/etc/btund
mkdir -p slack/etc/rc.d
mkdir -p slack/usr/bin
mkdir -p slack/bin

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

if ! grep btund /etc/rc.d/rc.M
then

   echo '# Start the btun server'             >> /etc/rc.d/rc.M
   echo 'if [ -x /etc/rc.d/rc.btund ]; then'  >> /etc/rc.d/rc.M
   echo '. /etc/rc.d/rc.btund start'          >> /etc/rc.d/rc.M
   echo 'fi'                                  >> /etc/rc.d/rc.M

fi

EOF

make mrproper || exit 1
make min $* || exit 1

install -sm 755 daemon/daemon max_uptime/max_uptime range/range xoror/xoror btun/btun slack/usr/bin/
install -sm 755 rsh/rsh slack/bin/
install -sm 755 btun/btund slack/usr/sbin/
install -m 644 rsh/rsh.conf slack/etc/
install -m 644 btun/config/users slack/etc/btund/
install -sm 755 btun/config/rc.btund  slack/etc/rc.d/

rev=$(sed -n 's/.*revision="\([^"]*\)".*/\1/gp' .svn/entries | sort -nr | head -1)
name="ng-utils-rev$rev-$(uname -m).tgz"

cd slack
tar fcz "$name" *
mv "$name" ..
cd ..
rm -fr slack

echo Done package $name.
