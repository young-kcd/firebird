#! /bin/sh


# This shell script changes both the SYSDBA user

#------------------------------------------------------------------------
# Prompt for response, store result in Answer

Answer=""

AskQuestion() {
    Test=$1
    DefaultAns=$2
    echo -n "${1}"
    Answer="$DefaultAns"
    read Answer
}


#------------------------------------------------------------------------
#  changeInitPassword


changeInitPassword() {

    NewPasswd=$1

    InitFile=/etc/rc.d/init.d/firebird
    if [ -f $InitFile ] 
      then
        ed $InitFile <<EOF
/ISC_PASSWORD/s/ISC_PASSWORD:=.*\}/ISC_PASSWORD:=$NewPasswd\}/g
w
q
EOF
    chmod u=rwx,g=rx,o= $InitFile

    fi
}

#------------------------------------------------------------------------
#  Generate new sysdba password


generateNewDBAPassword() {

    DBAPasswordFile=$IBRootDir/SYSDBA.password
    NewPasswd=`mkpasswd -l 8`
    echo "Firebird generated password " > $DBAPasswordFile
    echo "for user SYSDBA is : $NewPasswd" >> $DBAPasswordFile
    echo "generated on `hostname` at time `date`" >> $DBAPasswordFile
    echo "" >> $DBAPasswordFile
    echo "(For superserver you will also want to check the password in the" >> $DBAPasswordFile
    echo "daemon init routine in the file /etc/rc.d/init.d/firebird)" >> $DBAPasswordFile
    echo "" >> $DBAPasswordFile
    echo "Your password can be changed to a more suitable one using the" >> $DBAPasswordFile
    echo "/usr/local/firebird/bin/gsec program as show below:" >> $DBAPasswordFile
    echo "" >> $DBAPasswordFile
    echo ">cd /usr/local/firebird" >> $DBAPasswordFile
    echo ">bin/gsec -user sysdba -password <password>" >> $DBAPasswordFile
    echo "GSEC>modify sysdba -pw <newpassword>" >> $DBAPasswordFile
    echo "GSEC>quit" >> $DBAPasswordFile

    chmod u=rw,go= $DBAPasswordFile

    $IBBin/gsec -user sysdba -password masterkey <<EOF
modify sysdba -pw $NewPasswd
EOF

    changeInitPassword "$NewPasswd"


}


#------------------------------------------------------------------------
#  Change sysdba password - this routine is interactive and is only 
#  used in the install shell script not the rpm one.


askUserForNewDBAPassword() {

    OrigPasswd=""
    while [ -z "$OrigPasswd" ]
      do
          AskQuestion "Please enter current password for SYSDBA user: "
          OrigPasswd=$Answer
          
      done

    NewPasswd=""

    while [ -z "$NewPasswd" ]
      do
          AskQuestion "Please enter new password for SYSDBA user: "
          NewPasswd=$Answer
          if [ ! -z "$NewPasswd" ]
            then
             $IBBin/gsec -user sysdba -password $OrigPasswd <<EOF
modify sysdba -pw $NewPasswd
EOF
              echo ""

              changeInitPassword "$NewPasswd"

          fi
          
      done
}


#------------------------------------------------------------------------
#  Change sysdba password - this routine is interactive and is only 
#  used in the install shell script not the rpm one.


changeDBAPassword() {

    if [ -z "$InteractiveInstall" ]
      then
        generateNewDBAPassword
      else
        askUserForNewDBAPassword
    fi
}


#= Main Post ===============================================================


    IBRootDir=@install@
    IBBin=$IBRootDir/bin
    RunUser=root

    InteractiveInstall=true
    export InteractiveInstall

    changeDBAPassword
