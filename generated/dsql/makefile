!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/dsql


CPP_FILES=array.cpp blob.cpp metd.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

array.cpp : $(REL_SRC_DIR)/array.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

blob.cpp : $(REL_SRC_DIR)/blob.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

metd.cpp : $(REL_SRC_DIR)/metd.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

