REL_SRC_DIR=../../src/msgs
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
	$(GPRE_PGM) -r -m -n $? -o >$@

{$(REL_SRC_DIR)}.epp{}.cpp:
	$(GPRE_PGM) -r -m -n $? -o >$@

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

#
# load.c - disabled
#

CPP_FILES=build_file.cpp check_msgs.cpp enter_msgs.cpp modify_msgs.cpp \
        change_msgs.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

