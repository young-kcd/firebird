#/bin/sh
#
#  The contents of this file are subject to the Initial
#  Developer's Public License Version 1.0 (the "License");
#  you may not use this file except in compliance with the
#  License. You may obtain a copy of the License at
#  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
#
#  Software distributed under the License is distributed AS IS,
#  WITHOUT WARRANTY OF ANY KIND, either express or implied.
#  See the License for the specific language governing rights
#  and limitations under the License.
#
#  The Original Code was created by Alexander Peshkov on 2-Apr-2007
#  for the Firebird Open Source RDBMS project.
#
#  Copyright (c) 2007 Alexander Peshkov
#  and all contributors signed below.
#
#  All Rights Reserved.
#  Contributor(s): ______________________________________.
#
#
#
#
# Script to prepare an "official source distribution"
# User is responsible to have clean tree with no foreign files.
# Using such tarball does not require presence of auto-tools.
#

# Determine root of local checkout
SRCROOT=`dirname $0`/../..
MODULE=$SRCROOT/temp/src
MEMBERS=`awk -F / <$SRCROOT/CVS/Entries '{print $2;}'`

# Cleanup
rm -rf $MODULE

echo "Copying to new tree"
mkdir $MODULE
pushd $SRCROOT >/dev/null 2>&1
MAKEFILES=`echo gen/[Mm]ake*`
popd >/dev/null 2>&1
tar -C $SRCROOT -cf - $MEMBERS $MAKEFILES | tar -C $MODULE -xf -

# Load version information from the tree
source $MODULE/src/misc/writeBuildNum.sh
PACKNAME="Firebird-$PRODUCT_VER_STRING-$FIREBIRD_PACKAGE_VERSION"
DIRNAME="$SRCROOT/temp/$PACKNAME"

echo "Cleaning up"
rm -rf $DIRNAME
mv $MODULE $DIRNAME
pushd $DIRNAME >/dev/null 2>&1

# Remove CVS information
rm -rf `find . -name CVS -print`

# Clean gpre-generated files and extern
cd gen
make clean_all
cd ..
rm -rf gen

# Copy configure script
cp $SRCROOT/configure .

echo "Creating tarball for $PACKNAME"
cd ..
tar cjf ../gen/$PACKNAME.tar.bz2 $PACKNAME
popd >/dev/null 2>&1
