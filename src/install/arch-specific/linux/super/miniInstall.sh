#!/bin/sh

LinkLib() {
[ -f $2 ] || ln -s $1 $2
}

echo This script will register firebird in /etc/rc.d/init.d and start it.
echo Press enter to continue
read x

cp firebird.initd /etc/rc.d/init.d/firebird
chmod +x /etc/rc.d/init.d/firebird
chkconfig --add firebird

LinkLib /usr/local/firebird/lib/libfbclient.so.1.0.0 /usr/lib/libfbclient.so.1.0.0
LinkLib /usr/lib/libfbclient.so.1.0.0 /usr/lib/libfbclient.so.1
LinkLib /usr/lib/libfbclient.so.1 /usr/lib/libfbclient.so
LinkLib /usr/local/firebird/lib/libib_util.so /usr/lib/libib_util.so

/etc/rc.d/init.d/firebird start
