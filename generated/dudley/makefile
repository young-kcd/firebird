!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/dudley


C_FILES=exe.c extract.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

exe.c : $(REL_SRC_DIR)/exe.e
	$(GPRE_PGM) -r -m -n $? -o >$@

extract.c : $(REL_SRC_DIR)/extract.e
	$(GPRE_PGM) -r -m -n $? -o >$@

