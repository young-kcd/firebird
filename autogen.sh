#!/bin/sh
#
# Run this to generate all the initial makefiles, etc.
#
# $id: $

PKG_NAME=Firebird2
SRCDIR=`dirname $0`
DIE=0

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

# Put other tests for programs here!


# If anything failed, exit now.
if test "$DIE" -eq 1; then
  exit 1
fi

# Give a warning if no arguments to 'configure' have been supplied.
if test -z "$*"; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the"
  echo \`$0\'" command line."
  echo
fi

# Generate configure from configure.in
echo "Running autoconf ..."
autoconf || exit 1

# If NOCONFIGURE is set, skip the call to configure
if test x$NOCONFIGURE = x
then
  echo Running $SRCDIR/configure $conf_flags "$@" ...
  $SRCDIR/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PKG_NAME
else
  echo Skipping configure process.
fi

# EOF