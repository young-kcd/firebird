REL_SRC_DIR=../src/jrd
REL_DST_DIR=./jrd

.SUFFIXES : .e

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

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# NOTE: "stats" is temporarily removed since it isn't even used.
#
# stats.c
#

C_FILES=$(REL_DST_DIR)/blob_filter.c \
        $(REL_DST_DIR)/dfw.c         \
		$(REL_DST_DIR)/dpm.c         \
		$(REL_DST_DIR)/dyn.c         \
		$(REL_DST_DIR)/dyn_def.c     \
		$(REL_DST_DIR)/dyn_del.c     \
		$(REL_DST_DIR)/dyn_mod.c     \
        $(REL_DST_DIR)/dyn_util.c    \
		$(REL_DST_DIR)/fun.c         \
		$(REL_DST_DIR)/grant.c       \
		$(REL_DST_DIR)/ini.c         \
		$(REL_DST_DIR)/met.c         \
		$(REL_DST_DIR)/pcmet.c       \
		$(REL_DST_DIR)/scl.c         \
		$(REL_DST_DIR)/stats.c


all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) $(CLEAN_DIR_PATH)*.c

$(REL_DST_DIR)/blob_filter.c :
	$(GPRE_PGM) -n -manual -raw $? -o >$@

$(REL_DST_DIR)/dyn_def.c :
	$(GPRE_PGM) -n -gds -raw -ids $? -o > dyn_def.c.tmp
	-$(SED_PGM) -f $(REL_SRC_DIR)/dyn_def.sed dyn_def.c.tmp > $(REL_DST_DIR)/dyn_def.c
	$(DELETE_COMMAND) dyn_def.c.tmp

