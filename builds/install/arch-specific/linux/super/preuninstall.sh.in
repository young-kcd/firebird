#! /bin/sh



#------------------------------------------------------------------------
#  stop server if it is running


stopServerIfRunning() {

    checkString=`ps -ef| egrep "(ibserver|ibguard)" |grep -v grep`

    if [ ! -z "$checkString" ] 
      then
        /etc/rc.d/init.d/firebird stop
    fi
}



#= Main PreUn ================================================================



    IBRootDir=/opt/interbase
    IBBin=$IBRootDir/bin


    stopServerIfRunning

    /sbin/chkconfig --del firebird


    cd $IBRootDir
    if [ -f isc4.gdb ] 
      then 
        cp isc4.gdb /tmp 
        echo "Saved a copy of isc4.gdb in /tmp" 
    fi 
        

    for i in isc_init1* isc_event1* isc_lock1* isc_guard* interbase.log SYSDBA.password
      do
        if [ -f $i ]
          then
            rm -f $i
        fi
      done


    rm -f $IBBin/ibmgr






