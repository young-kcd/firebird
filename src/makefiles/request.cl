#!/bin/sh
# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
#
# Install script for UNIX InterBase software
#
# Copyright (c)1998,1999, InterBase Software Corporation
# All Rights Reserved
#
############################################################################

PATH=$PATH:/usr/ucb:/usr/bsd:/bin:/usr/bin
export PATH
SHLIB_PATH=/usr/lib
export SHLIB_PATH

LogFile="ibinstall.log"
TmpLogFile="/tmp/$LogFile"

DefaultInstDir="/opt"
IBSymLink="/usr/interbase"
SUPERSERVER=""

#Note: TarFile must contain the absolute path name
TarFile=""

ExitError()
{
echo "\n*** Install completed with errors ***" | tee -a $TmpLogFile
echo "Install log can be found at $TmpLogFile"
exit 1
}

ExitSuccess()
{
echo "\nInstall completed successfully" | tee -a $INTERBASE/$LogFile
echo "Install log can be found at $INTERBASE/$LogFile"
exit 0
}

Quit()
{
echo "\nInstall is not complete." | tee -a $TmpLogFile
echo "You may run the install at a later time to complete the installation." | tee -a $TmpLogFile
echo "Install log can be found at $TmpLogFile"
exit 2
}



#
# Main entry
#

echo "***********************************************" >> $TmpLogFile
echo "InterBase Client Install in progress" >> $TmpLogFile
echo "`date`" >> $TmpLogFile
echo "***********************************************" >> $TmpLogFile

if [ -f /bin/ps ]; then
    PS="/bin/ps -ef"
else
    if [ -f /usr/ucb/ps ]; then
        PS="/usr/ucb/ps -ax"
    else
        PS="ps -ef"
    fi
fi

ibserver=`$PS|grep ibserver|grep -c -v grep`
gds_inet_server=`$PS|grep gds_inet_server|grep -c -v grep`

if [ $ibserver -ne 0 -o $gds_inet_server -ne 0 ]; then
    echo "ERROR: Detected Interbase server running on the system." | tee -a $TmpLogFile
    echo "       Shutdown/Uninstall the InterBase server and try again." | tee -a $TmpLogFile
    ExitError
fi

SUPERSERVER=""

if [ ! -z "$IBINSTALL" ]; then
   IDirName=$IBINSTALL
else
   IDirName=$DefaultInstDir
fi
IBINSTALL=`ckpath -d $IDirName -aoy -p "\nEnter the absolute path name of the install directory"` || exit $?

INTERBASE="$IBINSTALL/interbase"

cat >$1 << !
TmpLogFile='$TmpLogFile'
LogFile='$LogFile'
INTERBASE='$INTERBASE'
SUPERSERVER='$SUPERSERVER'
IBSymLink='$IBSymLink'
IBINSTALL='$IBINSTALL'
!
exit 0
