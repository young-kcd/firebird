!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/gpre


CPP_FILES=gpre_meta.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

gpre_meta.cpp : $(REL_SRC_DIR)/gpre_meta.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

