#! /bin/sh


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



#= Main ProstUn ===============================================================

    
    if [ -L /usr/lib/libgds.so ]
      then
        rm -f /usr/lib/libgds.so
    fi

    # Update /etc/services


    FileName=/etc/services
    oldLine=`grep "^gds_db" $FileName`

    removeLineFromFile "$FileName" "$oldLine"

