#!/bin/sh
#
#
GENDIR="../../../../gen/firebird"
OUTDIR="../../../../output"
SRCDIR="../../../../src"
DOCDIR="../../../../doc"
EXADIR="../../../../examples"
#
#
rm -Rf $OUTDIR
rmdir -p $OUTDIR
mkdir $OUTDIR
#
# Copying root
#
echo Copying root...
cp $GENDIR/misc/firebird.conf	$OUTDIR
cp $GENDIR/security.fdb			$OUTDIR
gbak $GENDIR/security.fdb		$OUTDIR/security.fbk
cp $GENDIR/firebird.msg			$OUTDIR/firebird.msg
#
# Copying bin
#
echo Copying bin...
mkdir $OUTDIR/bin
cp $GENDIR/lib/fbclient.dll			$OUTDIR/bin/
echo "to be build"	>				$OUTDIR/bin/gds32.dll
cp $GENDIR/lib/libib_util.dll		$OUTDIR/bin/ib_util.dll
cp $GENDIR/bin/fb_lock_print.exe	$OUTDIR/bin/
cp $GENDIR/bin/fbserver.exe			$OUTDIR/bin/
cp $GENDIR/bin/gbak.exe				$OUTDIR/bin/
cp $GENDIR/bin/gdef.exe				$OUTDIR/bin/
cp $GENDIR/bin/gfix.exe				$OUTDIR/bin/
cp $GENDIR/bin/gpre.exe				$OUTDIR/bin/
cp $GENDIR/bin/gsec.exe				$OUTDIR/bin/
cp $GENDIR/bin/gsplit.exe			$OUTDIR/bin/
cp $GENDIR/bin/gstat.exe			$OUTDIR/bin/
cp $GENDIR/bin/ibguard.exe			$OUTDIR/bin/fbguard.exe
cp $GENDIR/bin/instreg.exe			$OUTDIR/bin/
cp $GENDIR/bin/instsvc.exe			$OUTDIR/bin/
cp $GENDIR/bin/isql.exe				$OUTDIR/bin/
cp $GENDIR/bin/nbackup.exe			$OUTDIR/bin/
cp $GENDIR/bin/qli.exe				$OUTDIR/bin/
#
# Copying Doc
#
echo Copying doc...
cp -R $DOCDIR $OUTDIR/doc
#
# Copying Examples
#
echo Copying examples...
cp -R $EXADIR $OUTDIR/examples
#
# Copying help
#
echo Copying help...
mkdir $OUTDIR/help
cp $GENDIR/help/help.fdb			$OUTDIR/help/
#
# Copying Include
#
echo Copying include...
mkdir $OUTDIR/include
cp $GENDIR/include/blr.h			$OUTDIR/include/
cp $GENDIR/include/gds.h			$OUTDIR/include/
cp $GENDIR/include/ib_util.h		$OUTDIR/include/
cp $GENDIR/include/ibase.h			$OUTDIR/include/
cp $GENDIR/include/iberror.h		$OUTDIR/include/
cp $GENDIR/include/perf.h			$OUTDIR/include/
#
# Copying intl
#
echo Copying intl...
mkdir $OUTDIR/intl
cp $GENDIR/intl/fbintl.dll			$OUTDIR/intl/
#
# Copying lib (should create fbclient_ms.lib and gds32_ms.lib
#
# TODO the fbclient_ms is generated with all the functions exported and so the size is huge.
# a new def file with cdecl and stdcall entrypoints for only the API functions should be created
#
echo Copying lib...
mkdir $OUTDIR/lib
cp $GENDIR/lib/fbclient_ms.lib		$OUTDIR/lib/
echo "to be build"	>				$OUTDIR/lib/gds32.lib
#
# Copying UDF
#
echo Copying UDF...
mkdir $OUTDIR/UDF
cp $SRCDIR/extlib/ib_udf.sql		$OUTDIR/UDF
cp $SRCDIR/extlib/fbudf/fbudf.sql	$OUTDIR/UDF
cp $GENDIR/UDF/fbudf.dll			$OUTDIR/UDF
cp $GENDIR/UDF/ib_udf.dll			$OUTDIR/UDF

