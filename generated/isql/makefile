!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/isql


CPP_FILES=extract.cpp isql.cpp show.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

extract.cpp : $(REL_SRC_DIR)/extract.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

isql.cpp : $(REL_SRC_DIR)/isql.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

show.cpp : $(REL_SRC_DIR)/show.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

