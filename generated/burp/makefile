!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre
SED_PGM=sed

COMPILER_CFLAGS=-c -DNOMSG -DWIN32_LEAN_AND_MEAN -nologo
OBJEXT=obj
DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/burp
COMMON_CFLAGS=-I../../src -I../../src/include
CFLAGS=$(COMMON_CFLAGS) $(COMPILER_CFLAGS)

#
# C_FILES could be useful as build target in case we are only
# generate the C files into this dir
#

C_FILES=backup.c restore.c

OBJ_FILES=backup.$(OBJEXT) restore.$(OBJEXT)

all: $(OBJ_FILES)

c_files : $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.$(OBJEXT)
	-$(DELETE_COMMAND) *.c

backup.c : $(REL_SRC_DIR)/backup.e
	$(GPRE_PGM) -r -m -n $? -o >$@

restore.c : $(REL_SRC_DIR)/restore.e
	$(GPRE_PGM) -r -m -n $? -o >$@

