##  $Revision: 1.2 $
##
##  Unix makefile for editline library.
##

##  Set your options:
##	-DANSI_ARROWS		ANSI arrows keys work like emacs.
##	-DHAVE_STDLIB		Have <stdlib.h>.
##	-DHAVE_TCGETATTR	Have tcgetattr(), tcsetattr().
##	-DHAVE_TERMIO		Have "struct termio" and <termio.h>
##	(If neither of above two, we use <sgttyb.h> and BSD ioctl's)
##	-DHIDE			Make static functions static (non debug).
##	-DHIST_SIZE=n		History size.
##	-DNEED_STRDUP		Don't have strdup().
##	-DUNIQUE_HISTORY	Don't save command if same as last one.
##	-DUSE_DIRENT		Use <dirent.h>, not <sys/dir.h>?
##	-DUSE_TERMCAP		Use the termcap library for terminal size
##				see LDFLAGS, below, if you set this.
##	-DNEED_PERROR		Don't have perror() (used in testit)
##	-DDO_SIGTSTP		Send SIGTSTP on "suspend" key
DEFS	= -DANSI_ARROWS -DHAVE_STDLIB -DHAVE_TCGETATTR -DHIDE -DUSE_DIRENT -DSYS_UNIX -DNEED_STRDUP

##  Set your C compiler:
WARN	= -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wwrite-strings \
	-Wunused -Wcomment -Wswitch
CC	= gcc -ansi $(WARN)
#CFLAGS	= $(DEFS) -O -g
#CC	= gcc
CFLAGS	= $(DEFS) -g

##  If you have -DUSE_TERMCAP, set this as appropriate:
#LDFLAGS = -ltermlib
#LDFLAGS = -ltermcap

##  Set ranlib as appropriate:
RANLIB	= ranlib
#RANLIB	= echo

##  End of configuration.

SOURCES	= editline.c complete.c sysunix.c
OBJECTS	= editline.o complete.o sysunix.o

FIREBIRD_LIB=../../../gen/firebird/lib
all:		$(FIREBIRD_LIB)/libedit.a
#all:		libedit.a

testit:		testit.c libedit.a
	$(CC) $(CFLAGS) -o testit testit.c libedit.a $(LDFLAGS)

shar:		FORCE
	shar `sed -e 1,2d -e 's/[ 	].*//' <MANIFEST` >shar

FORCE:

clean:
	rm -f *.[oa] testit foo core tags lint lint.all a.out Part01

lint:		testit
	lint -a -b -u -x $(DEFS) $(SOURCES) testit.c >lint.all
	sed -e '/warning: function prototype not in scope/d' \
		-e '/warning: old style argument declaration/'d \
		-e '/mix of old and new style function declaration/'d \
		<lint.all >lint


$(FIREBIRD_LIB)/libedit.a:	$(OBJECTS)
	@rm -f $@
	ar r $@ $(OBJECTS)
	$(RANLIB) $@

libedit.a:	$(OBJECTS)
	@rm -f $@
	ar r $@ $(OBJECTS)
	$(RANLIB) $@

$(OBJECTS):	editline.h
