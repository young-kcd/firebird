#!/bin/sh
#
# Run this to generate all the initial makefiles, etc.
#
# $Id: autogen.sh,v 1.12 2005-05-27 22:14:40 asfernandes Exp $

PKG_NAME=Firebird2
SRCDIR=`dirname $0`
DIE=0

if [ -z $AUTOCONF ]
then
  AUTOCONF=autoconf
fi
if [ -z $LIBTOOL ]
then
  LIBTOOL=libtool
fi
if [ -z $LIBTOOLIZE ]
then
  LIBTOOLIZE=libtoolize
fi

echo "AUTOCONF="$AUTOCONF
echo "LIBTOOL="$LIBTOOL
echo "LIBTOOLiZE="$LIBTOOLIZE
AUTOHEADER=`echo $AUTOCONF |sed 's/conf/header/'`

VER=`$AUTOCONF --version|grep '^[Aa]utoconf'|sed 's/^[^0-9]*//'`
case "$VER" in
 0* | 1\.* | 2\.[0-9] | 2\.[0-9][a-z]* | \
 2\.[1-4][0-9] | 2\.5[0-5][a-z]* )
  echo
  echo "**Error**: You must have autoconf 2.56 or later installed."
  echo "Download the appropriate package for your distribution/OS,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/autoconf/"
  DIE=1
  ;;
esac

VER=`$LIBTOOL --version|grep ' libtool)'|sed 's/.*) \([0-9][0-9.]*\) .*/\1/'`
case "$VER" in
 0* | 1\.[0-2] | 1\.[0-2][a-z]* | \
 1\.3\.[0-2] | 1\.3\.[0-2][a-z]* )
  echo
  echo "**Error**: You must have libtool 1.3.3 or later installed."
  echo "Download the appropriate package for your distribution/OS,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/libtool/"
  DIE=1
  ;;
esac

# Put other tests for programs here!


# If anything failed, exit now.
if test "$DIE" -eq 1; then
  exit 1
fi

# Give a warning if no arguments to 'configure' have been supplied.
if test -z "$*" -a x$NOCONFIGURE = x; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the"
  echo \`$0\'" command line."
  echo
fi

# Generate configure from configure.in
echo "Running libtoolize ..."
LIBTOOL_M4=`$LIBTOOLIZE --copy --force --dry-run|grep 'You should add the contents of'|sed "s,^[^/]*\(/[^']*\).*$,\1,"`
if test "x$LIBTOOL_M4" != "x"; then
 rm -f aclocal.m4
 cp $LIBTOOL_M4 aclocal.m4
fi
$LIBTOOLIZE --copy --force || exit 1

echo "Running autoheader ..."
$AUTOHEADER || exit 1

echo "Running autoconf ..."
$AUTOCONF || exit 1

# If NOCONFIGURE is set, skip the call to configure
if test "x$NOCONFIGURE" = "x"; then
  echo Running $SRCDIR/configure $conf_flags "$@" ...
  rm -f config.cache config.log
  chmod a+x $SRCDIR/configure
  $SRCDIR/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PKG_NAME
else
  echo Autogen skipping configure process.
fi

# EOF
