echo Removing temporary directories

rm -Rf autom4te.cache 2>/dev/null
rm -Rf gen 2>/dev/null
rm -Rf temp 2>/dev/null

echo Removing files in this directory

rm -f aclocal.m4 2>/dev/null
rm config.log 2>/dev/null
rm config.status 2>/dev/null
rm configure 2>/dev/null
rm libtool 2>/dev/null
rm -f Makefile 2>/dev/null

echo Removing files in builds directory

rm -f builds/make.new/config/config.guess 2>/dev/null
rm -f builds/make.new/config/config.sub 2>/dev/null
rm -f builds/make.new/config/ltmain.sh 2>/dev/null
rm -f builds/make.new/config/ltconfig 2>/dev/null
rm -f builds/make.new/config/ltcf-c.sh 2>/dev/null
rm -f builds/make.new/config/ltcf-cxx.sh 2>/dev/null
rm -f builds/make.new/config/config.h.in 2>/dev/null

echo Removing files in src directory

rm src/include/gen/autoconfig.h 2>/dev/null
rm src/include/gen/blrtable.h 2>/dev/null
rm -f src/v5_examples/Makefile 2>/dev/null

# Cpp from epp files
rm src/alice/alice_meta.cpp 2>/dev/null
rm src/burp/backup.cpp 2>/dev/null
rm src/burp/restore.cpp 2>/dev/null
rm src/dsql/dsql.tab.c 2>/dev/null
rm src/dsql/array.cpp 2>/dev/null
rm src/dsql/blob.cpp 2>/dev/null
rm src/dsql/metd.cpp 2>/dev/null
rm src/dudley/exe.cpp 2>/dev/null
rm src/dudley/extract.cpp 2>/dev/null
rm src/gpre/gpre_meta.cpp 2>/dev/null
rm src/isql/extract.cpp 2>/dev/null
rm src/isql/isql.cpp 2>/dev/null
rm src/isql/show.cpp 2>/dev/null
rm src/jrd/dfw.cpp 2>/dev/null
rm src/jrd/dpm.cpp 2>/dev/null
rm src/jrd/dyn.cpp 2>/dev/null
rm src/jrd/dyn_def.cpp 2>/dev/null
rm src/jrd/dyn_del.cpp 2>/dev/null
rm src/jrd/dyn_mod.cpp 2>/dev/null
rm src/jrd/dyn_util.cpp 2>/dev/null
rm src/jrd/fun.cpp 2>/dev/null
rm src/jrd/grant.cpp 2>/dev/null
rm src/jrd/ini.cpp 2>/dev/null
rm src/jrd/met.cpp 2>/dev/null
rm src/jrd/pcmet.cpp 2>/dev/null
rm src/jrd/scl.cpp 2>/dev/null
rm src/misc/codes.cpp 2>/dev/null
rm src/misc/relations.cpp 2>/dev/null
rm src/msgs/build_file.cpp 2>/dev/null
rm src/msgs/change_msgs.cpp 2>/dev/null
rm src/msgs/check_msgs.cpp 2>/dev/null
rm src/msgs/enter_msgs.cpp 2>/dev/null
rm src/msgs/modify_msgs.cpp 2>/dev/null
rm src/qli/help.cpp 2>/dev/null
rm src/qli/meta.cpp 2>/dev/null
rm src/qli/proc.cpp 2>/dev/null
rm src/qli/show.cpp 2>/dev/null
rm src/utilities/gstat/dba.cpp 2>/dev/null
rm src/utilities/gsec/security.cpp 2>/dev/null

exit 0
