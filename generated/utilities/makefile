!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/utilities

#
# rstore.cpp - disabled
#
CPP_FILES=dba.cpp security.cpp rmet.cpp

all: $(CPP_FILES)

clean :
	-$(DELETE_COMMAND) *.cpp

dba.cpp : $(REL_SRC_DIR)/dba.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

security.cpp : $(REL_SRC_DIR)/security.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

rmet.cpp : $(REL_SRC_DIR)/rmet.epp
	$(GPRE_PGM) -r -m -n $? -o >$@

# Disabled - unused
#rstore.cpp : $(REL_SRC_DIR)/rstore.epp
#	$(GPRE_PGM) -r -m -n $? -o >$@

