#!/bin/ksh
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
scriptname="$0"
step="$1"
keywords="$2"
pkglist="$3"

# Source in the standard functions library, ccsSetup.sh
. ccsSetup.sh

doPreLoad() {
if [ -e /usr/interbase ]
then
	echo
	echo
	echo "----------------***** ERROR *****-----------------"
	echo "/usr/interbase existing on the system. "
	echo
	echo "Aborting installation ...."
	echo
	echo "You must remove an active existing installation of Interbase"
	echo "from the system before attempting to install this one." 
	echo "For further details, please refer to the release notes"
	echo "included on the media."
	echo
	echo
	ccs_return_value=1
fi
}

doPostExport() {
	/usr/interbase/install
	# Extract the InterBase version and save it in .version file
	/usr/interbase/bin/gpre -z 2>/dev/null | head -1 | cut -d' ' -f3 > $SSO_CLIENT_ROOT/.version
	# Copy .version file to /usr/interbase for cross checking during removal
	cp $SSO_CLIENT_ROOT/.version /usr/interbase
	echo
	echo
	echo "----------------***** IMPORTANT *****-----------------"
	echo "Please read release notes in /usr/interbase/ReleaseNotes.txt"
	echo "or /usr/interbase/ReleaseNotes.html. They contain important"
	echo "information about this version of InterBase on SCO and"
	echo "instructions on how to access other products and"
	echo "documents on the CD-ROM"
	echo
	echo
}

doPreUnexport() {
	if [ -d /usr/interbase ]
	then
		# Check whether /usr/interbase contains exactly the
		# same version that is being removed and only then remove.
		if cmp /usr/interbase/.version $SSO_CLIENT_ROOT/.version 
		then
			touch /tmp/.RMIBDIR
		fi
	fi
}

doPostUnload() {
	if [ -f /tmp/.RMIBDIR  ]
 	then
		rm -rf /usr/interbase
		rm -f /tmp/.RMIBDIR
		rm -rf /usr/include/gds.h
		rm -rf /usr/include/ibase.h
		rm -rf /usr/lib/libgds*
	else
		# Either /usr/interbase does not contain a .version
		# or it contains a .version which is different from
		# $SSO_CLIENT_ROOT/.version. So don't remove
		# /usr/interbase
		echo
		echo 
		echo "Warning"
		echo "-------" 
		echo "The package has been removed from the system but"
		echo "/usr/interbase contains a version other than the one"
		echo "you are trying to remove."
		echo "Hence /usr/interbase has not been removed."
		echo
		echo
	fi
}

ccs_return_value=0

case "$step" in
	PRE_LOAD) doPreLoad ;;
    POST_EXPORT) doPostExport ;;
	PRE_UNEXPORT)  doPreUnexport ;;
	POST_UNLOAD) doPostUnload ;;
esac

exit $ccs_return_value
