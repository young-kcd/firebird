REL_SRC_DIR=../../src/msgs
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
	$(GPRE_PGM) -r -m -n $? -o >$@

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# load.c - disabled
#

C_FILES=build_file.c check_msgs.c enter_msgs.c modify_msgs.c \
        change_msgs.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

