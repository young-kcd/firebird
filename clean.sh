echo Removing directories

rm -Rf autom4te.cache 2>/dev/null
rm -Rf gen 2>/dev/null

echo Removing files in this directory

rm aclocal.m4 2>/dev/null
rm config.log 2>/dev/null
rm config.status 2>/dev/null
rm configure 2>/dev/null
rm libtool 2>/dev/null
rm Makefile 2>/dev/null

echo Removing files in src directory

rm src/include/gen/autoconfig.h 2>/dev/null
rm src/include/gen/blrtable.h 2>/dev/null
rm src/indicator.* 2>/dev/null
rm -f src/*.fdb 2>/dev/null
rm -f src/*.FDB 2>/dev/null
rm -f src/*.lnk 2>/dev/null
rm src/Makefile 2>/dev/null
rm builds/make.new/config/config.guess 2>/dev/null
rm builds/make.new/config/config.sub 2>/dev/null
rm builds/make.new/config/ltmain.sh 2>/dev/null
rm src/v5_examples/Makefile 2>/dev/null

# Cpp from epp files
rm src/burp/backup.cpp 2>/dev/null
rm src/burp/restore.cpp 2>/dev/null
rm src/dsql/array.cpp 2>/dev/null
rm src/dsql/blob.cpp 2>/dev/null
rm src/dsql/metd.cpp 2>/dev/null
rm src/gpre/gpre_meta.cpp 2>/dev/null
rm src/jrd/codes.cpp 2>/dev/null
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
rm src/jrd/ 2>/dev/null
rm src/msgs/build_file.cpp 2>/dev/null
rm src/msgs/change_msgs.cpp 2>/dev/null
rm src/msgs/check_msgs.cpp 2>/dev/null
rm src/msgs/enter_msgs.cpp 2>/dev/null
rm src/msgs/modify_msgs.cpp 2>/dev/null
