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
# Uninstall script for UNIX InterBase software
#
#          ***  FOR INTERNAL USE ONLY ***
#
# Copyright (c)1998,1999, InterBase Software Corporation
# All Rights Reserved
#
############################################################################

SHLIB_PATH=/usr/lib
export SHLIB_PATH

PATH=$PATH:/usr/ucb:/usr/bsd:/bin:/usr/bin
export PATH

DeleteSymLinks()
{
if [ -f $INTERBASE/lib/gdsmt.so.0 ]; then
    rm -f /usr/lib/libgdsmt.so
    rm -f /usr/lib/libgdsmt.so.0
fi
}
#
#
echo "\nThe InterBase Client software will be removed from your computer."
count=0
while true; do
    echo "\nDo you want to continue? (default: n) [y, n] : \c"
    read response
    case $response in
	y|Y|yes|Yes|YES )
	    echo "\nAll the files from \"$INTERBASE\" directory will be lost."
	    break
	;;
	n|N|no|NO|No|"" ) exit 1;;
	* )
	    echo "\nERROR: Invalid response, expected \"y\" or \"n\""
    esac
done

DeleteSymLinks

exit 0
