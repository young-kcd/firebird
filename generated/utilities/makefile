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
# rstore.c - disabled
#
C_FILES=dba.c security.c rmet.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

dba.c : $(REL_SRC_DIR)/dba.e
	$(GPRE_PGM) -r -m -n $? -o >$@

security.c : $(REL_SRC_DIR)/security.e
	$(GPRE_PGM) -r -m -n $? -o >$@

rmet.c : $(REL_SRC_DIR)/rmet.e
	$(GPRE_PGM) -r -m -n $? -o >$@

# Disabled - unused
#rstore.c : $(REL_SRC_DIR)/rstore.e
#	$(GPRE_PGM) -r -m -n $? -o >$@

