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

CheckInstallDir()
{
if [ ! -z "$1" ]; then
  if [ ! -d "$1" ]; then
    while true; do
      echo "\nDirectory \"$IBINSTALL\" does not exist." | tee -a $TmpLogFile
      echo "Do you want to create it now? (default: y) [y, n, q] : \c" | tee -a $TmpLogFile
      read response
      echo "$response" >> $TmpLogFile
      case $response in
        y|Y|yes|Yes|YES|"" )
	  mkdir -p $IBINSTALL
	  echo "\nmkdir -p $IBINSTALL" >> $TmpLogFile
	  break;;
	n|N|no|NO|No )
	  IBINSTALL=""
	  break;;
	q|Q|quit|Quit|QUIT )
	  Quit;;
	* )
	  echo "\nERROR: Invalid response, expected \"y\", \"n\" or \"q\"" | tee -a $TmpLogFile
	  ;;
      esac
    done
  else
    if [ -d "$1/interbase" ]; then
      while true; do
	echo "\nDirectory \"$IBINSTALL/interbase\" already exists." | tee -a $TmpLogFile
	echo "Do you want to continue? (default: n) [y, n, q] : \c" | tee -a $TmpLogFile
	read response
	echo "$response" >> $TmpLogFile
	case $response in
	  y|Y|yes|Yes|YES )
	    break;;
	  n|N|no|NO|No|"" )
	    IBINSTALL=""
	    break;;
	  q|Q|quit|Quit|QUIT )
	    Quit;;
	  * )
	    echo "\nERROR: Invalid response, expected \"y\", \"n\" or \"q\"" | tee -a $TmpLogFile
	    ;;
	esac
      done
    fi
  fi
fi
}

GetInstallDir()
{

while [ -z "$IBINSTALL" ] ; do
    echo "\nEnter the absolute path name of the install directory" | tee -a $TmpLogFile
    echo "[$DefaultInstDir] : \c" | tee -a $TmpLogFile
    read inst_dir
    echo "$inst_dir" >> $TmpLogFile
    case $inst_dir in
        "" )
            IBINSTALL="$DefaultInstDir";;
        /* )
            IBINSTALL="$inst_dir";;
	* )
	    echo "\nERROR: The specified pathname \"$inst_dir\" is not the absolute." | tee -a $TmpLogFile
	    echo "       Please try again." | tee -a $TmpLogFile
	    continue;;
    esac

    CheckInstallDir $IBINSTALL
    if [ -z "$IBINSTALL" ]; then
	continue
    fi
done
}

#
# Main entry
#

if [ -f $TmpLogFile ]; then
    mv $TmpLogFile $TmpLogFile.old
    echo "Old $TmpLogFile moved to $TmpLogFile.old\n" >> $TmpLogFile
fi
echo "***********************************************" >> $TmpLogFile
echo "InterBase Install in progress" >> $TmpLogFile
echo "`date`" >> $TmpLogFile
echo "***********************************************" >> $TmpLogFile

if [ "`whoami`" != "root" ]; then
    echo "ERROR: No permissions to perform operation." | tee -a $TmpLogFile
    echo "       Log in as root and try again." | tee -a $TmpLogFile
    ExitError
fi

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
    echo "ERROR: Detected another Interbase server running on the system." | tee -a $TmpLogFile
    echo "       Shutdown the InterBase server and try again." | tee -a $TmpLogFile
    ExitError
fi

if [ $# -gt 2 ]; then
    echo "ERROR: too many parameters." | tee -a $TmpLogFile
    echo "Usage: $0 [-c | -s] AbsolutePathOfTheTarFile" | tee -a $TmpLogFile
    ExitError
fi

for i in $1 $2 ; do
    case $i in
	-c | -C )
	    SUPERSERVER="";;
	-s | -S )
	    SUPERSERVER="SUPERSERVER";;
	* )
	    TarFile="$i";;
    esac
done

if [ -z "$TarFile" ]; then
    echo "ERROR: Syntax error, expected archive file." | tee -a $TmpLogFile
    echo "Usage: $0 [-c | -s] AbsolutePathOfTheTarFile" | tee -a $TmpLogFile
    ExitError
fi

if [ -z "$IBINSTALL" ]; then
    GetInstallDir
else
    case "$IBINSTALL" in
	/* )
	    CheckInstallDir $IBINSTALL
	    if [ -z "$IBINSTALL" ]; then
		GetInstallDir
	    fi
	    ;;
	* )
	    echo "ERROR: The specified pathname \"$IBINSTALL\" is not the absolute."
	    IBINSTALL=""
	    GetInstallDir;;
    esac
fi

cd $IBINSTALL
if [ ! -f $TarFile ]; then
    echo "ERROR: File \"$TarFile\" not found." | tee -a $TmpLogFile
    ExitError
fi

INTERBASE="$IBINSTALL/interbase"

#
# Extract files from a tar archive
#
echo "----------------------------\n" >> $TmpLogFile
echo "\nExtracting files" >> $TmpLogFile
echo "\nExtracting files... \c"
tar vxf $TarFile >> $TmpLogFile 2>&1
if [ $? -ne 0 ]; then
    echo "ERROR: Can't extract the files from the archive." | tee -a $TmpLogFile
    ExitError
fi
echo "Done"
echo "----------------------------\n" >> $TmpLogFile

#
# If no switches were specified then set SUPERSERVER depending on the
# content of the tar file
#
if [ $# -ne 2 ]; then
    if [ -f $INTERBASE/bin/ibserver ]; then
	SUPERSERVER="SUPERSERVER"
    else
	SUPERSERVER=""
    fi
fi

#
# Make sure the top level ($INTERBASE) is 777
#
echo "Updating access permissions... \c"
echo "Updating access permissions" >> $TmpLogFile
chmod 777 $INTERBASE
echo "chmod 777 $INTERBASE" >> $TmpLogFile

#
# Protect security, examples and QLI help databases
#
if [ "$SUPERSERVER" ]; then
    chmod uog=rw $INTERBASE/isc4.gdb
    chmod uog=rw $INTERBASE/help/help.gdb
    chmod uog=rw $INTERBASE/examples/employee.gdb
    chmod uog=rw $INTERBASE/examples/intlemp.gdb
    chmod 4755 $INTERBASE/bin/gstat
    echo "chmod uog=rw $INTERBASE/isc4.gdb" >> $TmpLogFile
    echo "chmod uog=rw $INTERBASE/help/help.gdb" >> $TmpLogFile
    echo "chmod uog=rw $INTERBASE/examples/employee.gdb" >> $TmpLogFile
    echo "chmod uog=rw $INTERBASE/examples/intlemp.gdb" >> $TmpLogFile
    echo "chmod 4755 $INTERBASE/bin/gstat" >> $TmpLogFile

fi
echo "Done"
echo "----------------------------\n" >> $TmpLogFile


#
# Create the symbolic links
#
echo "Creating symbolic links... \c"
echo "Creating symbolic links" >> $TmpLogFile

if [ -f $INTERBASE/include/gds.h ]; then
    rm -f /usr/include/gds.h
    echo "rm -f /usr/include/gds.h" >> $TmpLogFile
    ln -s $IBSymLink/include/gds.h /usr/include/gds.h
    echo "ln -s $IBSymLink/include/gds.h /usr/include/gds.h" >> $TmpLogFile
fi
if [ -f $INTERBASE/include/ibase.h ]; then
    rm -f /usr/include/ibase.h
    echo "rm -f /usr/include/ibase.h" >> $TmpLogFile
    ln -s $IBSymLink/include/ibase.h /usr/include/ibase.h
    echo "ln -s $IBSymLink/include/ibase.h /usr/include/ibase.h" >> $TmpLogFile
fi
if [ -f $INTERBASE/include/iberror.h ]; then
    rm -f /usr/include/iberror.h
    echo "rm -f /usr/include/iberror.h" >> $TmpLogFile
    ln -s $IBSymLink/include/iberror.h /usr/include/iberror.h
    echo "ln -s $IBSymLink/include/iberror.h /usr/include/iberror.h" >> $TmpLogFile
fi
if [ -f $INTERBASE/include/ib_util.h ]; then
    rm -f /usr/include/ib_util.h
    echo "rm -f /usr/include/ib_util.h" >> $TmpLogFile
    ln -s $IBSymLink/include/ib_util.h /usr/include/ib_util.h
    echo "ln -s $IBSymLink/include/ib_util.h /usr/include/ib_util.h" >> $TmpLogFile
fi

if [ -f $INTERBASE/lib//gds_b.a ]; then
    rm -f /usr/lib/libgds_b.a
    echo "rm -f /usr/lib/libgds_b.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds_b.a /usr/lib/libgds_b.a
    echo "ln -s $IBSymLink/lib/gds_b.a /usr/lib/libgds_b.a" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gds.a ]; then
    rm -f /usr/lib/libgds.a
    echo "rm -f /usr/lib/libgds.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds.a /usr/lib/libgds.a
    echo "ln -s $IBSymLink/lib/gds.a /usr/lib/libgds.a" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gds.sl ]; then
    rm -f /usr/lib/libgds.sl
    echo "rm -f /usr/lib/libgds.sl" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds.sl /usr/lib/libgds.sl
    echo "ln -s $IBSymLink/lib/gds.sl /usr/lib/libgds.sl" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gdsshr.a ]; then
    rm -f /usr/lib/libgdsshr.a
    echo "rm -f /usr/lib/libgdsshr.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gdsshr.a /usr/lib/libgdsshr.a
    echo "ln -s $IBSymLink/lib/gdsshr.a /usr/lib/libgdsshr.a" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gds_s.a ]; then
    rm -f /usr/lib/libgds_s
    echo "rm -f /usr/lib/libgds_s" >> $TmpLogFile
    rm -f /usr/lib/libgds_s.a
    echo "rm -f /usr/lib/libgds_s.a" >> $TmpLogFile
    rm -f /usr/lib/libgdsf_s
    echo "rm -f /usr/lib/libgdsf_s" >> $TmpLogFile
    rm -f /usr/lib/libgdsf_s.a
    echo "rm -f /usr/lib/libgdsf_s.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds_s /usr/lib/libgds_s
    echo "ln -s $IBSymLink/lib/gds_s /usr/lib/libgds_s" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds_s.a /usr/lib/libgds_s.a
    echo "ln -s $IBSymLink/lib/gds_s.a /usr/lib/libgds_s.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gdsf_s /usr/lib/libgdsf_s
    echo "ln -s $IBSymLink/lib/gdsf_s /usr/lib/libgdsf_s" >> $TmpLogFile
    ln -s $IBSymLink/lib/gdsf_s.a /usr/lib/libgdsf_s.a
    echo "ln -s $IBSymLink/lib/gdsf_s.a /usr/lib/libgdsf_s.a" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gds.so.0 ]; then
    if [ -d /usr/shlib ]; then
        rm -f /usr/shlib/libgds.so
	echo "rm -f /usr/shlib/libgds.so" >> $TmpLogFile
        rm -f /usr/shlib/libgds.so.0
	echo "rm -f /usr/shlib/libgds.so.0" >> $TmpLogFile
        ln -s $IBSymLink/lib/gds.so.0 /usr/shlib/libgds.so.0
	echo "ln -s $IBSymLink/lib/gds.so.0 /usr/shlib/libgds.so.0" >> $TmpLogFile
        ln -s /usr/shlib/libgds.so.0 /usr/shlib/libgds.so
	echo "ln -s /usr/shlib/libgds.so.0 /usr/shlib/libgds.so" >> $TmpLogFile
    else
        rm -f /usr/lib/libgds.so
	echo "rm -f /usr/lib/libgds.so" >> $TmpLogFile
        rm -f /usr/lib/libgds.so.0
	echo "rm -f /usr/lib/libgds.so.0" >> $TmpLogFile
        ln -s $IBSymLink/lib/gds.so.0 /usr/lib/libgds.so.0
	echo "ln -s $IBSymLink/lib/gds.so.0 /usr/lib/libgds.so.0" >> $TmpLogFile
        ln -s /usr/lib/libgds.so.0 /usr/lib/libgds.so
	echo "ln -s /usr/lib/libgds.so.0 /usr/lib/libgds.so" >> $TmpLogFile
    fi
fi
if [ -f $INTERBASE/lib/gdsmt.so.0 ]; then
    rm -f /usr/lib/libgdsmt.so
    echo "rm -f /usr/lib/libgdsmt.so" >> $TmpLogFile
    rm -f /usr/lib/libgdsmt.so.0
    echo "rm -f /usr/lib/libgdsmt.so.0" >> $TmpLogFile
    ln -s $IBSymLink/lib/gdsmt.so.0 /usr/lib/libgdsmt.so.0
    echo "ln -s $IBSymLink/lib/gdsmt.so.0 /usr/lib/libgdsmt.so.0" >> $TmpLogFile
    ln -s /usr/lib/libgdsmt.so.0 /usr/lib/libgdsmt.so
    echo "ln -s /usr/lib/libgdsmt.so.0 /usr/lib/libgdsmt.so" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gdsf.so ]; then
    rm -f /usr/lib/libgdsf.so
    echo "rm -f /usr/lib/libgdsf.so" >> $TmpLogFile
    ln -s $IBSymLink/lib/gdsf.so /usr/lib/libgdsf.so
    echo "ln -s $IBSymLink/lib/gdsf.so /usr/lib/libgdsf.so" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/gds_pyxis.a ]; then
    rm -f /usr/lib/libgds_pyxis.a
    echo "rm -f /usr/lib/libgds_pyxis.a" >> $TmpLogFile
    ln -s $IBSymLink/lib/gds_pyxis.a /usr/lib/libgds_pyxis.a
    echo "ln -s $IBSymLink/lib/gds_pyxis.a /usr/lib/libgds_pyxis.a" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/ib_util.sl ]; then
    rm -f /usr/lib/libib_util.sl
    echo "rm -f /usr/lib/libib_util.sl" >> $TmpLogFile
    ln -s $IBSymLink/lib/ib_util.sl /usr/lib/libib_util.sl
    echo "ln -s $IBSymLink/lib/ib_util.sl /usr/lib/libib_util.sl" >> $TmpLogFile
fi
if [ -f $INTERBASE/lib/ib_util.so ]; then
    rm -f /usr/lib/libib_util.so
    echo "rm -f /usr/lib/libib_util.so" >> $TmpLogFile
    ln -s $IBSymLink/lib/ib_util.so /usr/lib/libib_util.so
    echo "ln -s $IBSymLink/lib/ib_util.so /usr/lib/libib_util.so" >> $TmpLogFile
fi

echo "Done"
echo "----------------------------\n" >> $TmpLogFile

#
# If TCP is installed, update both services and servers databases
#
if [ "$SUPERSERVER" ]; then
    echo "Updating service name database... \c"
    echo "Updating service name database" >> $TmpLogFile
    if [ -f /etc/services ]; then
	grep -q -s gds_db /etc/services  
	if test $? != 0 ; then
	    cat /etc/services $INTERBASE/services.isc > services
	    echo "cat /etc/services $INTERBASE/services.isc > services" >> $TmpLogFile
	    mv services /etc/services
	    echo "mv services /etc/services" >> $TmpLogFile
	fi
    fi
    echo "Done"
    echo "----------------------------\n" >> $TmpLogFile
    echo "Updating configuration file for inetd... \c"
    echo "Updating configuration file for inetd" >> $TmpLogFile
    if [ -f /etc/inetd.conf ]; then
	# Remove gds_db service. We do not want gds_inet_server to be invoked
	cat /etc/inetd.conf | grep -v gds_db > inetd.conf
	echo "cat /etc/inetd.conf | grep -v gds_db > inetd.conf" >> $TmpLogFile
	mv inetd.conf /etc/inetd.conf
	echo "mv inetd.conf /etc/inetd.conf" >> $TmpLogFile
    fi
    if [ -f /usr/etc/inetd.conf ]; then
	# Remove gds_db service. We do not want gds_inet_server to be invoked
	cat /usr/etc/inetd.conf | grep -v gds_db > inetd.conf
	echo "cat /usr/etc/inetd.conf | grep -v gds_db > inetd.conf" >> $TmpLogFile
	mv inetd.conf /usr/etc/inetd.conf
	echo "mv inetd.conf /usr/etc/inetd.conf" >> $TmpLogFile
    fi
    echo "Done"
    echo "----------------------------\n" >> $TmpLogFile
fi

#
# create the ibmgr shell script
#
if [ "$SUPERSERVER" ]; then
echo "Creating the ibmgr shell script... \c"
echo "Creating the ibmgr shell script" >> $TmpLogFile
cat > $INTERBASE/bin/ibmgr << EOF
#!/bin/sh
INTERBASE=$INTERBASE
export INTERBASE
exec $INTERBASE/bin/ibmgr.bin \$@
EOF

chmod +x $INTERBASE/bin/ibmgr
echo "Done"
echo "----------------------------\n" >> $TmpLogFile
fi

#
# Create the license file
#
if [ "$SUPERSERVER" ]; then
INTERBASE="$INTERBASE"; export INTERBASE
echo "Creating the license file" >> $TmpLogFile
echo "\nPlease enter the license certificate id: \c" | tee -a $TmpLogFile
read id
echo "$id" >> $TmpLogFile
case "$id" in
  "eval" | "" )
      echo "Creating a 90-day evaluation license..." | tee -a $TmpLogFile
      $INTERBASE/bin/iblicense -eval
      chmod uog=r $INTERBASE/ib_license.dat
      ;;
  * )
      echo "Please enter the license certificate key: \c" | tee -a $TmpLogFile
      read key
      echo "$key" >> $TmpLogFile
      if [ -z "$key" ] ; then
	echo "We need the license key which matches your license id." | tee -a $TmpLogFile
	echo "Please enter the license certificate key: \c" | tee -a $TmpLogFile
	read key
	echo "$key" >> $TmpLogFile
      fi
      if [ ! -z "$key" ] ; then
	echo "Creating the license..." | tee -a $TmpLogFile
	$INTERBASE/bin/iblicense -add -id $id -key $key
	if [ $? -eq 0 ]; then
	    chmod uog=r $INTERBASE/ib_license.dat
	else
	    ExitError
	fi
      else
	echo "ERROR: No license created. Please see the release notes" | tee -a $TmpLogFile
        echo "       to create a license file." | tee -a $TmpLogFile
	ExitError
      fi
      ;;
esac
fi
#
# Create /usr/interbase symbolic link
#

if [ "$INTERBASE" != "$IBSymLink" ]; then
    echo "Creating /usr/interbase symbolic link... \c"
    echo "Creating /usr/interbase symbolic link" >> $TmpLogFile

    if [ -a "$IBSymLink" -o -L "$IBSymLink" ]; then
	mv $IBSymLink ${IBSymLink}.$$
	echo "WARNING: Old \"$IBSymLink\" moved to \"${IBSymLink}.$$\"" | tee -a $TmpLogFile
    fi
	
    ln -s $INTERBASE $IBSymLink
    echo "ln -s $INTERBASE $IBSymLink" >> $TmpLogFile
    echo "Done"
    echo "----------------------------\n" >> $TmpLogFile
fi
#
# Move the log file to the $INTERBASE
#
if [ -f $INTERBASE/$LogFile ]; then
    cp $INTERBASE/$LogFile $INTERBASE/$LogFile.old
    echo "Old $INTERBASE/$LogFile moved to $INTERBASE/$LogFile.old" >> $TmpLogFile
fi
mv $TmpLogFile $INTERBASE/$LogFile
ExitSuccess
