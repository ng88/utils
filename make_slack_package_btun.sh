#!/bin/sh

export PATH="/sbin/:$PATH"

rm -fr slack
mkdir -p slack/install

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
chmod a+r slack/install/slack-desc

cat > slack/install/doinst.sh <<EOF
#/bin/sh

if ! grep btund /etc/rc.d/rc.M >/dev/null 2>&1
then

   echo '# Start the btun server'             >> /etc/rc.d/rc.M
   echo 'if [ -x /etc/rc.d/rc.btund ]; then'  >> /etc/rc.d/rc.M
   echo '. /etc/rc.d/rc.btund start'          >> /etc/rc.d/rc.M
   echo 'fi'                                  >> /etc/rc.d/rc.M

fi

LDP=/usr/lib/btun/plugins/
if ! grep "\$LDP" /etc/ld.so.conf >/dev/null 2>&1
then
   echo "\$LDP" >> /etc/ld.so.conf
   /sbin/ldconfig
fi

EOF
chmod a+rx slack/install/doinst.sh

make mrproper || exit 1
make btun $* || exit 1
make tcpmux $* || exit 1
make installbtun $* prefix=slack || exit 1

install -m 755 -d slack/etc/rc.d/
install -m 644 btun/C_client_server/config/rc.btund slack/etc/rc.d/
rm -f slack/etc/btund/users

rev=$(common/get_svn_rev.sh)
name="btun-rev$rev-$(uname -m).tgz"

cd slack
makepkg -l y -p -c n  "$name"
mv "$name" ..
cd ..
rm -fr slack

echo Done package $name.
