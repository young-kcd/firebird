REL_SRC_DIR=../../src/jrd
.SUFFIXES : .e

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

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# NOTE: "stats" is temporarily removed since it isn't even used.
#
# stats.c
#

C_FILES=blob_filter.c dfw.c dpm.c dyn.c dyn_def.c dyn_del.c dyn_mod.c \
        dyn_util.c fun.c grant.c ini.c met.c pcmet.c scl.c stats.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

blob_filter.c :
	$(GPRE_PGM) -n -manual -raw $? -o >$@

dyn_def.c :
	$(GPRE_PGM) -n -gds -raw -ids $? -o > dyn_def.c.tmp
	$(SED_PGM) -f $(REL_SRC_DIR)/dyn_def.sed dyn_def.c.tmp > dyn_def.c

