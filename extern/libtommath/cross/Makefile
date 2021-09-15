DIR_OBJ=.
DIR_SRC=..

include		$(DIR_SRC)/makefile

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	$(CC) -I$(DIR_SRC) $(CFLAGS) -c $(firstword $<) -o $@
