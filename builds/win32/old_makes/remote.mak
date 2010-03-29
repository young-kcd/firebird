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
#
# 2002.10.29 Sean Leyne - Removed support for obsolete IPX/SPX Protocol
#
#--------------------------- PC REMOTE MAKEFILE ---------------------------
ROOT=..
.path.c=$(ROOT)\remote

# Path to devkit and clisdk
!if !$d(TPATH)
TPATH=..
!endif

# add local paths for winsock files before including std.mk
# These will be added to the front of INCLUDEPATH and LIBPATH respectively.
LINCLUDEPATH=$(TPATH)\clisdk\include;$(TPATH)\wfwdev\include

# CRM - temporarily disabled until warnings are cleaned up!!!
#LCFLAGS=-wpro
LCFLAGS=$(LCFLAGS) -DWINDOWS -DNWWIN -DREQUESTER

# These are C flags if you want to enable useful warnings.
#LCFLAGS=$(LCFLAGS) -w -w-ucp -w-sus -w-par -w-stu -w-aus -w-pia

!include $(ROOT)\std.mk

# Add compiler flags for router
APP_CFLAGS=$(APP_CFLAGS) -DWINDOWS_ROUTER

#--------------------------- SOURCE COMPONENTS -----------------------------

S1 = allr.c inet.c interfac.c merge.c parser.c protocol.c
S2 = remote.c xdr.c ntoh.c 
SRCS = $(S1) $(S2)
JRDSRCS = cvt.c 
TSTSRCS = server.c inet_ser.c winmain.c

WINSRCS = dllshell.c wnet.c wfwnp.c

# Extra files which are used to build the product
XFILES   = remote.def makefile.mak nlmtst.def

# NOTE:  c0dx must be the first obj in the list!
OBJS=$(TOOLPATH)\lib\c0d$(MODEL).obj $(SRCS:.c=.obj) \
     $(JRDSRCS:.c=.obj) $(WINSRCS:.c=.obj)
TSTOBJS=$(TSTSRCS:.c=.obj)

LIBS=mathw$(MODEL) import cw$(MODEL)
RMTLIBS=$(ROOT)\jrd\$(.path.obj)\gds.lib $(ROOT)\jrd\$(.path.obj)\iutls.lib
TSTLIBS=$(RMTLIBS) $(ROOT)\remote\$(.path.obj)\remote.lib $(TPATH)\devkit\netbcc3.0\winsock.lib

#---------------------------- TARGET LIST --------------------------------
ALLBIN = remote.dll nlmtst.exe
ALLLIBS = remote.lib

all: makefile.tmp extern_libs $(ALLLIBS) $(ALLBIN)
allbin: makefile.tmp extern_libs $(ALLBIN)
alllibs: makefile.tmp $(ALLLIBS)


#------------------------------ RULES -------------------------------------

#----- Windows REMOTE DLL and import library -----
remote.dll: $(OBJS) remote.def $(RMTLIBS) remote.rsp $(ROOT)\jrd\version.rc
	tlink $(LFLAGS) @$(.path.obj)\remote.rsp @&&|
	$<
	$*.map
	$(LIBS) $(RMTLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version.rc $<

	# Build a symbol file for post mortem dump analysis
	tmapsym $(.path.obj)\remote.map
	copy remote.sym $(.path.obj)

remote.rsp: $(OBJS)
	copy &&|
	$**
| $<

remote.lib: remote.def
	$(IMPLIB) -o $< $**

nlmtst.exe: $(ROOT)\jrd\version.rc nlmtst.rsp nlmtst.def $(TSTOBJS) $(TSTLIBS)
	$(TLINK) $(APP_LFLAGS) $(TOOLPATH)\lib\c0w$(MODEL).obj @$(.path.obj)\nlmtst.rsp @&&|
	$<
	$*.map
	$(LIBS) $(TSTLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version.rc $<

# The .c files for an .exe must be compiled differently than DLLs
$(TSTOBJS): $(TSTSRCS)
	$(CC) -c -w- @&&<
		$(APP_CFLAGS)
< $&.c
	

# Because of the circular "architecture" here, we need to build 
# required libraries within each of the makefiles.
extern_libs:
	cd $(ROOT)\jrd
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\remote

nlmtst.rsp: $(TSTOBJS)
	copy &&|
	$**
| $<

cvt.obj: $(ROOT)\jrd\cvt.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

# To allow windows to multitask on a blocking TCP call,
# add -DHSNOBLOCKING when compiling inet.c.
inet.obj: inet.c
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
   copy $(DEVSRC)\remote\*.h
   for %i in ($(S1)) do copy $(DEVSRC)\remote\%i
   for %i in ($(S2)) do copy $(DEVSRC)\remote\%i
   for %i in ($(TSTSRCS)) do copy $(DEVSRC)\remote\%i
   for %i in ($(WINSRCS)) do copy $(DEVSRC)\remote\%i
   for %i in ($(DOSSRCS)) do copy $(DEVSRC)\remote\%i
   for %i in ($(XFILES)) do copy $(DEVSRC)\remote\%i
