!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/gpre


C_FILES=gpre_meta.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

gpre_meta.c : $(REL_SRC_DIR)/gpre_meta.e
	$(GPRE_PGM) -r -m -n $? -o >$@

