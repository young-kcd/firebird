FB_VERSION=1.5.1.4481
FB_REVISION=R1_5_1
cvs -z9 -d :ext:skidder@cvs.firebird.sourceforge.net:/cvsroot/firebird co -r $FB_REVISION firebird2
mv firebird2 firebird-$FB_VERSION
find firebird-$FB_VERSION -name CVS -exec rm -rf '{}' ';'
tar -cjf firebird-$FB_VERSION.tar.bz2 firebird-$FB_VERSION
