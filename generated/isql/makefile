!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/isql


C_FILES=extract.c isql.c show.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

extract.c : $(REL_SRC_DIR)/extract.e
	$(GPRE_PGM) -r -m -n $? -o >$@

isql.c : $(REL_SRC_DIR)/isql.e
	$(GPRE_PGM) -r -m -n $? -o >$@

show.c : $(REL_SRC_DIR)/show.e
	$(GPRE_PGM) -r -m -n $? -o >$@

