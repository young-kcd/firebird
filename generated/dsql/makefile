!IF "$(OS)" == "Windows_NT"

# NOTE - If you enter an absolute path to gpre.exe, it MUST use
# MS-DOS style backslash!
GPRE_PGM=..\..\bin\gpre

DELETE_COMMAND=del /q

!ELSE

!ERROR Running on unknown operating system.

!ENDIF

REL_SRC_DIR=../../src/dsql


C_FILES=array.c blob.c metd.c

all: $(C_FILES)

clean :
	-$(DELETE_COMMAND) *.c

array.c : $(REL_SRC_DIR)/array.e
	$(GPRE_PGM) -r -m -n $? -o >$@

blob.c : $(REL_SRC_DIR)/blob.e
	$(GPRE_PGM) -r -m -n $? -o >$@

metd.c : $(REL_SRC_DIR)/metd.e
	$(GPRE_PGM) -r -m -n $? -o >$@

