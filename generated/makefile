REL_SRC_DIR=../src/jrd
REL_DST_DIR=./jrd

.SUFFIXES : .e .epp

!IF "$(OS)" == "Windows_NT"

# NOTE - This should probably also be the path taken for other
# Win32 operating systems, like Win9x/Me.

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\bin\gpre
SED_PGM=sed

CLEAN_DIR_PATH=.\jrd\
DELETE_COMMAND=del /q

#
# MS NMAKE specific directory-relative inference rule
#
{$(REL_SRC_DIR)}.e{$(REL_DST_DIR)}.c:
	$(GPRE_PGM) -n -gds -raw -ids $? -o >$@

{$(REL_SRC_DIR)}.epp{$(REL_DST_DIR)}.cpp:
	$(GPRE_PGM) -n -gds -raw -ids $? -o >$@

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# NOTE: "stats" is temporarily removed since it isn't even used.
#
# stats.cpp
#

CPP_FILES=$(REL_DST_DIR)/blob_filter.cpp \
        $(REL_DST_DIR)/dfw.cpp           \
		$(REL_DST_DIR)/dpm.cpp         \
		$(REL_DST_DIR)/dyn.cpp         \
		$(REL_DST_DIR)/dyn_def.cpp     \
		$(REL_DST_DIR)/dyn_del.cpp     \
		$(REL_DST_DIR)/dyn_mod.cpp     \
        $(REL_DST_DIR)/dyn_util.cpp    \
		$(REL_DST_DIR)/fun.cpp         \
		$(REL_DST_DIR)/grant.cpp       \
		$(REL_DST_DIR)/ini.cpp         \
		$(REL_DST_DIR)/met.cpp         \
		$(REL_DST_DIR)/pcmet.cpp       \
		$(REL_DST_DIR)/scl.cpp         \
		$(REL_DST_DIR)/stats.cpp


all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) $(CLEAN_DIR_PATH)*.cpp

$(REL_DST_DIR)/blob_filter.cpp :
	$(GPRE_PGM) -n -manual -raw $? -o >$@

$(REL_DST_DIR)/dyn_def.cpp :
	$(GPRE_PGM) -n -gds -raw -ids $? -o > dyn_def.cpp.tmp
	-$(SED_PGM) -f $(REL_SRC_DIR)/dyn_def.sed dyn_def.cpp.tmp > $(REL_DST_DIR)/dyn_def.cpp
	$(DELETE_COMMAND) dyn_def.cpp.tmp

