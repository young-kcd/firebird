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

echo TmpLogFile=$TmpLogFile
echo LogFile=$LogFile
echo INTERBASE=$INTERBASE
echo SUPERSERVER=$SUPERSERVER
echo IBSymLink=$IBSymLink
echo IBINSTALL=$IBINSTALL

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
echo "Done"
echo "----------------------------\n" >> $TmpLogFile


#
# Create /usr/interbase symbolic link
#

if [ "$INTERBASE" != "$IBSymLink" ]; then
    echo "Creating /usr/interbase symbolic link... \c"
    echo "Creating /usr/interbase symbolic link" >> $TmpLogFile

    if [ -f "$IBSymLink" -o -d "$IBSymLink" -o -h "$IBSymLink" ]; then
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
