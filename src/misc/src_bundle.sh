#/bin/sh
#
# Script to checkout a clean copy of the sourcecode from CVS
# and prepare it as an official "source distribution"
#

# Determine root of local checkout
SRCROOT=`dirname $0`/../..

# Set the CVS root from the local checkout if possible
ROOT=`cat $SRCROOT/CVS/Root`
if [ "x$ROOT" == 'x' ]; then
    ROOT=":pserver:anonymous@cvs.sourceforge.net:/cvsroot/firebird"
fi

# Set the tag or branch based on the current checkout, if possible
CVSTAG='$Name: not supported by cvs2svn $'
TAG=`echo $CVSTAG | sed -e 's/\\$Name: not supported by cvs2svn $/\1/' | sed -e 's/ //'`
if [ "x$TAG" == 'x' ]; then
    TAG="B1_5_Release"
fi

MODULE=firebird2
NAME=firebird
rm -rf $MODULE

echo "Checking out $MODULE from $ROOT using $TAG"
cvs -z9 -d $ROOT export -r $TAG $MODULE

# Load version information from the new checkout
source $MODULE/src/misc/writeBuildNum.sh
DIRNAME="$NAME-$PRODUCT_VER_STRING"

echo "Creating tarball for $DIRNAME"
rm -rf $DIRNAME
mv $MODULE $DIRNAME
cd $DIRNAME

echo "Generating configure script"
NOCONFIGURE=1 . ./autogen.sh > /dev/null
cd ..
tar -cjf $DIRNAME.tar.bz2 $DIRNAME
echo "New tarball is $DIRNAME.tar.bz2"
rm -rf $DIRNAME

