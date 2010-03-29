# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
# The contents of this file are subject to the Interbase Public
# License Version 1.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy
# of the License at http://www.Inprise.com/IPL.html
#
# Software distributed under the License is distributed on an
# "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
# or implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code was created by Inprise Corporation
# and its predecessors. Portions created by Inprise Corporation are
# Copyright (C) Inprise Corporation.
#
# All Rights Reserved.
# Contributor(s): ______________________________________.
#--------------------------- PC DSQL MAKEFILE --------------------------- 
ROOT=..
.path.c=$(ROOT)\dsql

# -wpro	: warns undefined function calls
# -N-	: turn stack checking off
LCFLAGS=-DREQUESTER -N-

# Path to devkit and clisdk
!if !$d(TPATH)
TPATH=..
!endif

!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS ----------------------------- 

WINSRCS = dllshell.c

# DSQL source files
S1 = alld.c ddl.c dsql.c dsqlwep.c errd.c gen.c hsh.c
S2 = make.c metd.c movd.c parse.c pass1.c 
SRCS = $(S1) $(S2)

# sources from the jrd component used to build dsql.dll
JRDSRCS = cvt.c 

# DSQL source files used by the jrd component for client side requests
CLIENTSRCS = array.c blob.c preparse.c user_dsq.c utld.c


# DSQL import definition files
DSQLLIBS = $(ROOT)\jrd\$(.path.obj)\gds.lib $(ROOT)\jrd\$(.path.obj)\iutls.lib

# Extra files which are used to build the product
XFILES   = dsql.def makefile.mak


#---------------------------- TARGET LIST --------------------------------
# NOTE:  c0dx must be the first obj in the list!
OBJS= $(TOOLPATH)\lib\c0d$(MODEL).obj $(SRCS:.c=.obj) \
      $(JRDSRCS:.c=.obj) $(WINSRCS:.c=.obj)

ALLBIN = dsql.dll
ALLLIBS = dsql.lib

all: makefile.tmp extern_libs $(ALLBIN) $(ALLLIBS)
alllibs : makefile.tmp $(ALLLIBS)
allbin :  makefile.tmp extern_libs $(ALLBIN)


LIBS=mathw$(MODEL) import cw$(MODEL)

#------------------------------ RULES -------------------------------------

#----- Windows DSQL DLL -----
dsql.dll: $(OBJS) $(DSQLLIBS) dsql.rsp dsql.def $(ROOT)\jrd\version.rc
	$(TLINK) $(LFLAGS) @$*.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(DSQLLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version.rc $<


#----------- Response file -----------

dsql.rsp: $(OBJS)
	copy &&|
	$**
| $<



#----------- Import Library -----------

# Because of the circular "architecture" here, we need to build 
# required libraries within each of the makefiles.
extern_libs:
	cd $(ROOT)\jrd
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\dsql


dsql.lib: dsql.def
	implib -o $< $**


# ---- Rules for source files which require special handling ----

cvt.obj: $(ROOT)\jrd\cvt.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

dllshell.obj: $(ROOT)\jrd\dllshell.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

#---------------------------- UTILITIES ---------------------------------
# Copy targets to a known installation directory
install:  extern_libs $(ALLBIN) $(ALLLIBS)
	for %i in ($(ALLBIN)) do copy $(.path.dll)\%i $(INSTALLBIN)
	for %i in ($(ALLLIBS)) do copy $(.path.dll)\%i $(INSTALLLIB)

# Refresh all the source & header files from the DEVSRC directory
srcs::
   copy $(DEVSRC)\dsql\*.h
   for %i in ($(S1)) do copy $(DEVSRC)\dsql\%i
   for %i in ($(S2)) do copy $(DEVSRC)\dsql\%i
   for %i in ($(CLIENTSRCS)) do copy $(DEVSRC)\dsql\%i
   for %i in ($(WINSRCS)) do copy $(DEVSRC)\dsql\%i
   for %i in ($(XFILES)) do copy $(DEVSRC)\dsql\%i
	copy $(DEVSRC)\wal\*.h $(ROOT)\wal
