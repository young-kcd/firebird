#!/bin/sh

# The post uninstall routine for Firebird Classic.


#------------------------------------------------------------------------
# remove line from config file if it exists in it.

removeLineFromFile() {
    FileName=$1
    oldLine=$2

    if [ ! -z "$oldLine" ] 
      then
        cat $FileName | grep -v "$oldLine" > ${FileName}.tmp
        mv ${FileName}.tmp $FileName
        echo "Updated."

    fi
}


#= Main PostUn ============================================================

    if [ -L /usr/lib/libgds.so ] 
      then 
        rm /usr/lib/libgds.so 
    fi 

    if [ "$1"=0 ] 
      then 

        # Lose the gds_db line from /etc/services 

        FileName=/etc/services
        oldLine=`grep "^gds_db" $FileName`
        removeLineFromFile "$FileName" "$oldLine"

        # Next, lose the gds_db line from /etc/inetd.conf

        FileName=/etc/inetd.conf
        oldLine=`grep "^gds_db" $FileName`
        removeLineFromFile "$FileName" "$oldLine"

        # Get inetd to reread new inetd.conf file

        if [ -f /var/run/inetd.pid ]
          then
            kill -HUP `cat /var/run/inetd.pid`
        fi
    fi
