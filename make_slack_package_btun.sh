#!/bin/sh

rm -fr slack
mkdir -p slack/install
mkdir -p slack/etc/btund
mkdir -p slack/etc/rc.d
mkdir -p slack/usr/bin
mkdir -p slack/usr/sbin

cat > slack/install/slack-desc <<EOF
# HOW TO EDIT THIS FILE:
# The "handy ruler" below makes it easier to edit a package description.  Line
# up the first '|' above the ':' following the base package name, and the '|'
# on the right side marks the last column you can put a character in.  You must
# make exactly 11 lines for the formatting to be correct.  It's also
# customary to leave one space after the ':'.

    |-----handy-ruler------------------------------------------------------|
btun: btun (network tool)
btun: 
btun: A backward tunnel creator that enable you to go through firewalls.
btun: 
btun: 
btun: 
btun: 
btun: 
btun: 
btun: 
btun: 
EOF

cat > slack/install/doinst.sh <<EOF
#/bin/sh

if ! grep btund /etc/rc.d/rc.M >/dev/null 2>&1
then

   echo '# Start the btun server'             >> /etc/rc.d/rc.M
   echo 'if [ -x /etc/rc.d/rc.btund ]; then'  >> /etc/rc.d/rc.M
   echo '. /etc/rc.d/rc.btund start'          >> /etc/rc.d/rc.M
   echo 'fi'                                  >> /etc/rc.d/rc.M

fi

EOF

make mrproper || exit 1
make btun $* || exit 1

chmod a+x slack/install/doinst.sh

install -sm 755 btun/btun slack/usr/bin/
install -sm 755 btun/btund slack/usr/sbin/
install -m 644 btun/config/users.sample slack/etc/btund/
install -m 755 btun/config/rc.btund  slack/etc/rc.d/

rev=$(sed -n 's/.*revision="\([^"]*\)".*/\1/gp' .svn/entries | sort -nr | head -1)
name="btun-rev$rev-$(uname -m).tgz"

cd slack
tar fcz "$name" *
mv "$name" ..
cd ..
rm -fr slack

echo Done package $name.
