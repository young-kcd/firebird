!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/dudley


CPP_FILES=exe.cpp extract.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

exe.cpp : $(REL_SRC_DIR)/exe.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

extract.cpp : $(REL_SRC_DIR)/extract.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

