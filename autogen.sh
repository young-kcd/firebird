#!/bin/sh
#
# Run this to generate all the initial makefiles, etc.
#

PKG_NAME=Firebird2
SRCDIR=`dirname $0`
DIE=0

if [ -z "$AUTORECONF" ]
then
  AUTORECONF=autoreconf
fi

echo "AUTORECONF="$AUTORECONF

# This prevents calling automake in old autotools
AUTOMAKE=true
export AUTOMAKE

VER=`$AUTORECONF --version|grep '^[Aa]utoreconf'|sed 's/^[^0-9]*//'`
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

# Some versions of autotools need it
if [ ! -d m4 ]; then
	rm -rf m4
	mkdir m4
fi

# Ensure correct utilities are called by AUTORECONF
autopath=`dirname $AUTORECONF`
if [ "$autopath" != x. ]; then
	PATH=$autopath:$PATH
	export PATH
fi

echo "Running autoreconf ..."
$AUTORECONF --install --force --verbose || exit 1

# Hack to bypass bug in autoreconf - --install switch not passed to libtoolize,
# therefore missing config.sub and confg.guess files
CONFIG_AUX_DIR=builds/make.new/config
if [ ! -f $CONFIG_AUX_DIR/config.sub -o ! -f $CONFIG_AUX_DIR/config.guess ]; then
	# re-run libtoolize with --install switch, if it does not understand that switch
	# and there are no config.sub/guess files in CONFIG_AUX_DIR, we will anyway fail
	echo "Re-running libtoolize ..."
	if [ -z "$LIBTOOLIZE" ]; then
		LIBTOOLIZE=libtoolize
	fi
	$LIBTOOLIZE --install --copy --force || exit 1
fi

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
