#!/usr/bin/ksh
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
# Setup script
#
# Copyright (c)1998,1999, InterBase Software Corporation
# All Rights Reserved
#
############################################################################

PATH=$PATH:/usr/ucb:/usr/bsd:/bin:/usr/bin
export PATH

CheckStatus()
{
case $1 in
    0 )
	echo "The installation completed successfully"
	;;
    1 )
	echo "*** The installation failed"
	;;
    2 )
	echo "*** The installation is not complete"
	;;
esac
}

if [ "`whoami`" != "root" ]; then
    echo "ERROR: No permissions to perform operation."
    echo "       Log in as root and try again."
    exit 1
fi

ibserver=`ps -ef|grep ibserver|grep -c -v grep`
if [ $ibserver -ne 0 ]; then
    echo "ERROR: Detected another Interbase server running on the system."
    echo "       Shutdown the InterBase server and try again."
    exit 1
fi

CurrentDir=`pwd`
FilesDir=`dirname $0`

case "${FilesDir}" in
  /* ) ;;
  . ) FilesDir="$CurrentDir" ;;
  * ) FilesDir="$CurrentDir"/"$FilesDir" ;;
esac

CurrentOS=`uname -s`
IC="InterClient*.tar"
case $CurrentOS in
    HP-UX )
        IBCS="InterBase60_ClientServer_HUS.tar"
        IBC="InterBase60_Client_HUS.tar"
        ib_install="interbase/install.ksh"
        ;;
    SunOS )
        IBCS="IBCSpkg"
        IBC="IBCOpkg"
	NONABI_SCRIPTS=TRUE
	export NONABI_SCRIPTS
	;;
esac
Acrobat="Adobe/INSTALL"
inst=YES

while [ $inst = YES ] ; do
    echo "\n\t1. Install InterBase Client and Server software"
    echo "\t2. Install InterBase Client Only software"
    echo "\t3. Install InterClient JDBC software"
    echo "\t4. Install Adobe Acrobat(R) Reader software"
    echo "\t5. Exit\n"
    echo "Enter selection. (default 1) [1-5] : \c"
    read selection
    case $selection in
	1 | "" )
	    case $CurrentOS in
		HP-UX )
		    if [ -f $FilesDir/$IBCS ]; then
			echo "\nStarting InterBase Client and Server Install, please wait..."
			cd /tmp;
			tar xfp $FilesDir/$IBCS $ib_install;
			./$ib_install -s $FilesDir/$IBCS;
			status=$?
			CheckStatus $status
		    else
			echo "\nERROR: $FilesDir/$IBCS not found";
			echo "*** The installation failed ***";
		    fi
		    ;;
		SunOS )
		    if [ -d $FilesDir/$IBCS ]; then
		        pkgadd -d $FilesDir/$IBCS;
			status=$?
			CheckStatus $status
		    else
		        echo "\nERROR: $FilesDir/$IBCS not found";
			echo "*** The installation failed ***";
		    fi
		    ;;
		* )
		    echo "ERROR: install script does not support \"$CurrentOS\"";
		    echo "*** The installation failed ***";;
	    esac
	    ;;
	2 )
	    case $CurrentOS in
		HP-UX )
                    if [ -f $FilesDir/$IBC ]; then
			echo "\nStarting InterBase Client Install, please wait..."
			cd /tmp;
			tar xfp $FilesDir/$IBC $ib_install;
			./$ib_install -c $FilesDir/$IBC;
			status=$?
			CheckStatus $status
                    else
                        echo "\nERROR: $FilesDir/$IBC not found";
			echo "*** The installation failed ***";
                    fi
                    ;;
		SunOS )
		    if [ -d $FilesDir/$IBC ]; then
		        pkgadd -d $FilesDir/$IBC;
			status=$?
			CheckStatus $status
		    else
		        echo "\nERROR: $FilesDir/$IBC not found";
			echo "*** The installation failed ***";
		    fi
		    ;;
                * )
                    echo "ERROR: install script does not support \"$CurrentOS\"";
		    echo "*** The installation failed ***";;
            esac
            ;;
	3 )
	    echo "\nStarting InterClient JDBC Install, please wait...";
	    case $CurrentOS in
		HP-UX )
		    if [ -f $FilesDir/$IC ]; then
			cd /tmp;
			tar xpf $FilesDir/InterClient*.tar;
			cd interclient_install_temp_dir;
			./install.sh;
			status=$?
			CheckStatus $status
		    else
			echo "\nERROR: $FilesDir/$IC not found";
			echo "*** The installation failed ***";
		    fi
		    ;;
                * )
                    echo "ERROR: install script does not support \"$CurrentOS\"";
		    echo "*** The installation failed ***";;
            esac
	    ;;
	4)
	    echo "\nStarting Adobe Acrobat(R) Reader Install, please wait...";
	    if [ -f $FilesDir/$Acrobat ]; then
		$FilesDir/$Acrobat;
		status=$?
		CheckStatus $status
	    else
		echo "\nERROR: $FilesDir/$Acrobat not found";
		echo "*** The installation failed ***";
	    fi
	    ;;
	5 )
	    inst=DONE;;
	* )
	    echo "\nERROR: Invalid selection, expected 1 thru 5\n";;
    esac
done
exit 0
