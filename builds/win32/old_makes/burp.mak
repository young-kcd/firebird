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
#use $(Cx) to have command line options appear in DLL_CFLAGS
#NOTE: USE -DCx=-DWINBETA for BETA version string (see code in burp.c)
#--------------------------- PC BURP MAKEFILE ---------------------------
ROOT=..
.path.c=$(ROOT)\burp

# Path to devkit and clisdk
!if !$d(TPATH)
TPATH=..
!endif

# CRM - temporarily disabled until warnings are cleaned up!!!
LCFLAGS=-N-

# These are C flags if you want to enable useful warnings.
#LCFLAGS=$(LCFLAGS) -w -w-ucp -w-sus -w-par -w-stu -w-aus -w-pia

!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS -----------------------------
WINSRCS = dllshell.c

S1 = burp.c canonical.c misc.c mvol.c backup.c restore.c
S2 = burpwep.c
SRCS = $(S1) $(S2)

# sources from the remote component used to build gbak.dll
REMSRCS = xdr.c ntoh.c

# NOTE:  Object list - c0dx must be the first obj in the list!
!if $d(WIN32)
OBJS=$(TOOLPATH)\lib\c0d32.obj $(SRCS:.c=.obj) \
	 $(REMSRCS:.c=.obj) $(WINSRCS:.c=.obj)
!else
OBJS=$(TOOLPATH)\lib\c0d$(MODEL).obj $(SRCS:.c=.obj) \
	 $(REMSRCS:.c=.obj) $(WINSRCS:.c=.obj)
!endif

# import libraries
# NOTE: Client libs should now be taken from generic directories - Jeevan
#BURPLIBS = $(ROOT)\jrd\$(.path.obj)\gds.lib $(ROOT)\jrd\$(.path.obj)\iutls.lib
!if $d(WIN32)
BURPLIBS = $(SHRLIB_PATH)\gds32.lib
LIBS = import32 cw32
!else
BURPLIBS = $(INSTALLLIB)\gds.lib $(INSTALLLIB)\iutls.lib
LIBS = mathw$(MODEL) import cw$(MODEL)
!endif

# Extra files which are used to build the product
XFILES   = burp.def makefile.mak backup.e restore.e

#---------------------------- TARGET LIST --------------------------------
!if $d(WIN32)
ALLBIN = gbak32.dll
ALLLIBS = gbak32.lib
!else
ALLBIN = gbak.dll
ALLLIBS = gbak.lib
!endif

gbk_dll: makefile.tmp $(ALLBIN) $(ALLLIB)
all: makefile.tmp extern_libs $(ALLLIBS) $(ALLBIN)
allbin: makefile.tmp extern_libs $(ALLBIN)
alllibs: makefile.tmp $(ALLLIBS)


#------------------------------ RULES -------------------------------------

!if $d(WIN32)
gbak32.dll: $(OBJS) $(BURPLIBS) $(ROOT)\jrd\version_95.rc burp.rsp burp32.def
	$(TLINK) $(LFLAGS) @$(.path.obj)\burp.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(BURPLIBS)
	burp32.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version_95.rc $<
!else
gbak.dll: $(OBJS) $(BURPLIBS) $(ROOT)\jrd\version.rc burp.rsp burp.def
	$(TLINK) $(LFLAGS) @$(.path.obj)\burp.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(BURPLIBS)
	burp.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version.rc $<
!endif

#----------- Response files -----------
burp.rsp: $(OBJS)
	copy &&|
	$**
| $<

#----------- Import Libraries -----------
!if $d(WIN32)
gbak32.lib: burp32.def
	$(IMPLIB) $< $**
!else
gbak.lib: burp.def
	$(IMPLIB) -o $< $**
!endif

# Because of the circular "architecture" here, we need to build
# required libraries within each of the makefiles.
extern_libs:
	cd $(ROOT)\jrd
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\burp

# ---- Rules for source files which require special handling ----
xdr.obj: $(ROOT)\remote\xdr.c
	$(CC) -c -DBURP @&&<
	$(CFLAGS)
< $**

ntoh.obj: $(ROOT)\remote\ntoh.c
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
   copy $(DEVSRC)\burp\*.h
   for %i in ($(S1)) do copy $(DEVSRC)\burp\%i
   for %i in ($(S2)) do copy $(DEVSRC)\burp\%i
   for %i in ($(XFILES)) do copy $(DEVSRC)\burp\%i

