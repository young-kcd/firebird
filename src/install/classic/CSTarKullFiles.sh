#!/bin/sh

IBRootDir=`pwd`/interbase
IBBin=$IBRootDir/bin
RunUser=root


chown -R $RunUser.$RunUser $IBRootDir

cd $IBBin

for i in gds_lock_mgr gds_drop gds_inet_server
  do
    chmod ug+s $i
  done



cd $IBRootDir

rm -f isc_init1*
rm -f isc_lock1*

chmod ugo=rw isc4.gdb

rm -f inetd.conf.isc install

