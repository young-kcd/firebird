REL_SRC_DIR=../../src/jrd
.SUFFIXES : .e .epp

!IF "$(OS)" == "Windows_NT"

# NOTE - This should probably also be the path taken for other
# Win32 operating systems, like Win9x/Me.

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre
SED_PGM=sed

DELETE_COMMAND=del /q

#
# MS NMAKE specific directory-relative inference rule
#
{$(REL_SRC_DIR)}.e{}.c:
	$(GPRE_PGM) -n -gds -raw -ids $? -o >$@

{$(REL_SRC_DIR)}.epp{}.cpp:
	$(GPRE_PGM) -n -gds -raw -ids $? -o >$@

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# NOTE: "stats" is temporarily removed since it isn't even used.
#
# stats.cpp
#

CPP_FILES=blob_filter.cpp dfw.cpp dpm.cpp dyn.cpp dyn_def.cpp \
        dyn_del.cpp dyn_mod.cpp dyn_util.cpp fun.cpp grant.cpp \
        ini.cpp met.cpp pcmet.cpp scl.cpp stats.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

blob_filter.cpp :
	$(GPRE_PGM) -n -manual -raw $? -o >$@

dyn_def.cpp :
	$(GPRE_PGM) -n -gds -raw -ids $? -o > dyn_def.cpp.tmp
	$(SED_PGM) -f $(REL_SRC_DIR)/dyn_def.sed dyn_def.cpp.tmp > dyn_def.cpp

