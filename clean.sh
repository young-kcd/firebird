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
rm -f src/master_msg_db 2>/dev/null
rm -f src/yachts.lnk 2>/dev/null
rm src/Makefile 2>/dev/null
rm src/make.new/config/config.guess 2>/dev/null
rm src/make.new/config/config.sub 2>/dev/null
rm src/make.new/config/ltmain.sh 2>/dev/null
rm src/v5_examples/Makefile 2>/dev/null

# those files apparently aren't removed 
rm src/burp/backup.cpp 2>/dev/null
rm src/burp/restore.cpp 2>/dev/null
rm src/gpre/gpre_meta.cpp 2>/dev/null
rm src/jrd/codes.cpp 2>/dev/null
rm src/msgs/build_file.cpp 2>/dev/null
rm src/msgs/change_msgs.cpp 2>/dev/null
rm src/msgs/check_msgs.cpp 2>/dev/null
rm src/msgs/enter_msgs.cpp 2>/dev/null
rm src/msgs/modify_msgs.cpp 2>/dev/null
