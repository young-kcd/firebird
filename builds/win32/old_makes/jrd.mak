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
# Use the command line option -DNOSTACK to disable stack switching on JRD.DLL
#
#--------------------------- PC JRD MAKEFILE ----------------------------
ROOT=..
.path.c=$(ROOT)\jrd

# Path to devkit and clisdk
!if !$d(TPATH)
TPATH=..
!endif

#LCFLAGS (local compiler flags) -wpro -DHSDEBUG -DHSDEBUGFOREIGN -DHSDEBUGYGG
# -wpro : warns undefined function calls
# -N-   : turn stack checking off
LCFLAGS= -N-

#LLFLAGS (local link flags)
# -F    : inhibit optimization of far calls to near data
# -3    : generate 386 enhanced mode instructions
LLFLAGS= -f -3

LINCLUDEPATH=$(TPATH)\clisdk\include;$(TPATH)\wfwdev\include

!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS -----------------------------


WINSRCS = dllshell.c


JRD1 = all.c bkm.c blb.c btr.c builtin.c blf.c
JRD2 = cch.c cmp.c cvt.c cvt2.c dfw.c dpm.c dyn.c dyn_def.c dyn_del.c
JRD3 = dyn_mod.c err.c evl.c exe.c ext.c filters.c flu.c fun.c
JRD4 = grant.c iberr.c idx.c inf.c ini.c intl.c inuse.c
JRD5 = jrd.c jrdwep.c lck.c log.c met.c misc.c mov.c nav.c opt.c
JRD6 = pag.c par.c pcmet.c pcsleep.c pwd.c qatest.c rlck.c rng.c 
JRD7 = rse.c sbm.c sch.c scl.c sdw.c shut.c sort.c sqz.c svc.c sym.c
JRD8 = tpc.c tra.c val.c vio.c windows.c 
OPENSRCS = fileserv.c
JRDSRCS = $(JRD1) $(JRD2) $(JRD3) $(JRD4) $(JRD5) $(JRD6) $(JRD7) \
	  $(JRD8) $(OPENSRCS) 
JRDLOCKSRCS = lock.c
JRDOBJS = $(JRDSRCS:.c=.obj) $(WINSRCS:.c=.obj) $(JRDLOCKSRCS:.c=.obj)

GDSSRCS = alt.c flu.c gdswep.c license.c why.c perf.c utl.c 
GDSASM  = thin.asm 
GDSDSQLSRCS = preparse.c user_dsq.c blob.c array.c
GDSOBJS = $(GDSSRCS:.c=.obj) $(GDSASM:.asm=.obj) \
	  $(GDSDSQLSRCS:.c=.obj) $(WINSRCS:.c=.obj)

IUTL1 = dsc.c enc.c function.c gds.c isc.c isc_file.c isc_ipc.c
IUTL2 = isc_sync.c iutlswep.c loiter.c math.c node.c sdl.c seg.c thd.c 
IUTLSRCS = $(IUTL1) $(IUTL2) 
IUTLDSQLSRCS = utld.c 
IUTLOBJS = $(IUTLSRCS:.c=.obj) $(IUTLDSQLSRCS:.c=.obj) $(WINSRCS:.c=.obj)


# Extra files which are used to build the product
XFILE = gds.def jrd.def iutls.def makefile.mak version.rc jrd_ver.rc 
XFILES = evl_like.c sweep.cur

#---------------------------- TARGET LIST --------------------------------

# GDS import definition files
!if !$d(NOSTACK)
ENGINELIB = $(ROOT)\stack\$(.path.obj)\stack.lib
!else
ENGINELIB = jrd.lib
!endif

# GDS no longer links with ENGINELIB since it's dynamically loaded.
GDSLIBS = $(ROOT)\remote\$(.path.obj)\remote.lib \
	  $(ROOT)\dsql\$(.path.obj)\dsql.lib \
	  iutls.lib 

# JRD import definition files
JRDLIBS =iutls.lib

# IUTLS import definition files
IUTLLIBS =

LIBS = mathw$(MODEL) import cw$(MODEL)

ALLBIN = gds.dll jrd.dll iutls.dll
ALLLIBS = gds.lib jrd.lib iutls.lib 

all: makefile.tmp extern_libs gds.h $(ALLLIBS) $(ALLBIN)
alllibs: makefile.tmp $(ALLLIBS)
allbin: makefile.tmp extern_libs $(ALLBIN)

#------------------------------ RULES -------------------------------------
# NOTE:  c0dx must be the first obj on the link line!

#----------- Windows DLLs -----------

gds.dll: $(GDSOBJS) $(GDSLIBS) gds.rsp gds.def version.rc
	$(TLINK) $(LFLAGS) $(TOOLPATH)\lib\c0d$(MODEL) @$*.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(GDSLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include version.rc $<
!if !$d(NOSTACK)
	echo Stack Switching Build      > $(.path.dll)\readme.txt
!else
	echo No Stack Switching Build   > $(.path.dll)\readme.txt
!endif

jrd.dll: $(JRDOBJS) $(JRDLIBS) jrd.rsp jrd.def version.rc jrd_ver.rc
	$(TLINK) $(LFLAGS) $(TOOLPATH)\lib\c0d$(MODEL) @$*.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(JRDLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include jrd_ver.rc $<

iutls.dll: $(IUTLOBJS) $(IUTLLIBS) iutls.rsp iutls.def version.rc
	$(TLINK) $(LFLAGS) $(TOOLPATH)\lib\c0d$(MODEL) @$*.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(IUTLLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include version.rc $<

#----------- Response files -----------
gds.rsp: $(GDSOBJS)
	copy &&|
	$**
| $<

jrd.rsp: $(JRDOBJS)
	copy &&|
	$**
| $<


iutls.rsp: $(IUTLOBJS)
	copy &&|
	$**
| $<

#----------- Import Libraries -----------
jrd.lib: jrd.def
	implib -o $< $**

iutls.lib: iutls.def
	implib -o $< $**

gds.lib: gds.def
	implib -o $< $**

# Because of the circular "architecture" here, we need to build 
# required libraries within each of the makefiles.
extern_libs:
	cd $(ROOT)\dsql
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\remote
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\stack
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\jrd



# ---- Rules for source files which require special handling ----
thin.obj: thin.asm
	$(TASM) -m$(MODEL) thin.asm, $(.path.obj)\thin.obj

gds.h:  ibase.h gdsold.h
	$(RM) gds.h
	$(CAT) ibase.h > gds.h
	$(CAT) gdsold.h >> gds.h

fileserv.obj: $(ROOT)\open\fileserv.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

why.obj: why.c
	$(CC) -c @&&<
	$(CFLAGS) -DNO_LOCAL_FILTERS
< $**

utl.obj: utl.c
	$(CC) -c @&&<
	$(CFLAGS) -DREQUESTER
< $**

alt.obj: alt.c
	$(CC) -c @&&<
	$(CFLAGS) -DREQUESTER
< $**


# DSQL source files
user_dsq.obj: $(ROOT)\dsql\user_dsq.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

utld.obj: $(ROOT)\dsql\utld.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

preparse.obj: $(ROOT)\dsql\preparse.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

blob.obj: $(ROOT)\dsql\blob.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

array.obj: $(ROOT)\dsql\array.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

# Lock source files
lock.obj: $(ROOT)\lock\lock.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**

#---------------------------- UTILITIES ---------------------------------
# Copy targets to a known installation directory
install:  extern_libs gds.h $(ALLBIN) $(ALLLIBS)
	for %i in ($(ALLBIN)) do copy $(.path.dll)\%i $(INSTALLBIN)
	for %i in ($(ALLLIBS)) do copy $(.path.dll)\%i $(INSTALLLIB)
!if !$d(DEBUG)
	copy gds.h $(INSTALLINC)
	copy ibase.h $(INSTALLINC)
!endif


# Refresh all the source & header files from the DEVSRC directory
srcs::
	# Copy all the include files to make maintenance simpler
	copy $(DEVSRC)\jrd\*.h
	copy $(DEVSRC)\jrd\quad.c

	for %i in ($(GDSSRCS)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(GDSASM)) do copy $(DEVSRC)\jrd\%i

	for %i in ($(JRD1)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD2)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD3)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD4)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD5)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD6)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD7)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(JRD8)) do copy $(DEVSRC)\jrd\%i
	copy $(DEVSRC)\lock\*.h $(ROOT)\lock
	for %i in ($(JRDLOCKSRCS)) do copy $(DEVSRC)\lock\%i $(ROOT)\lock
	copy $(DEVSRC)\wal\*.h $(ROOT)\wal

	for %i in ($(IUTL1)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(IUTL2)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(IUTL3)) do copy $(DEVSRC)\jrd\%i

	for %i in ($(WINSRCS)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(XFILE)) do copy $(DEVSRC)\jrd\%i
	for %i in ($(XFILES)) do copy $(DEVSRC)\jrd\%i
