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
#--------------------------- PC INTL MAKEFILE --------------------------- 
ROOT=..
.path.c=$(ROOT)\intl

#-g0 -4 -p- -N -a -tWD -ml -dc -Ff=512
LCFLAGS=-Ff=255

!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS ----------------------------- 
WINSRCS = dllshell.c

S1 =  ld.c ld2.c cv_narro.c cs_narro.c lc_narro.c lc_ascii.c lc_latin.c 
S2 =  lc_dos.c cs_unico.c lc_unico.c cs_utffs.c cv_jis.c lc_jis.c cs_jis.c
S3 =  intlwep.c cs_jis_2.c 
SRCS = $(S1) $(S2) $(S3)

# NOTE:  c0dx must be the first obj in the list!
OBJS = $(TOOLPATH)\lib\c0d$(MODEL).obj $(SRCS:.c=.obj) $(WINSRCS:.c=.obj)

# import libraries
INTLLIBS = $(ROOT)\jrd\$(.path.obj)\iutls.lib

LIBS = mathw$(MODEL) import cw$(MODEL)

XFILES = intl.def
#---------------------------- TARGET LIST --------------------------------

ALLBIN = intl.dll
ALLLIBS = intl.lib

all: makefile.tmp $(ALLBIN) $(ALLLIBS)
allbin: makefile.tmp $(ALLBIN)
alllibs: makefile.tmp $(ALLLIBS)

#------------------------------ RULES -------------------------------------


intl.dll: $(OBJS) $(INTLLIBS) $(ROOT)\jrd\version.rc intl.rsp intl.def
	$(TLINK) $(LFLAGS) @$*.rsp, @&&|
	$<
	$*.map
	$(LIBS) $(INTLLIBS)
	$&.def
|
	$(RC)  -i $(TOOLPATH)\include $(ROOT)\jrd\version.rc $<

intl.rsp: $(OBJS)
	copy &&|
	$**
| $<

intl.lib: intl.def
	$(IMPLIB) -o $< $**


# ---- Rules for source files which require special handling ----
dllshell.obj: $(ROOT)\jrd\dllshell.c
	$(CC) -c @&&<
	$(CFLAGS)
< $**



#---------------------------- UTILITIES ---------------------------------
# Copy targets to a known installation directory
install:  $(ALLBIN) $(ALLLIBS)
	for %i in ($(ALLBIN)) do copy $(.path.dll)\%i $(INSTALLBIN)
	for %i in ($(ALLLIBS)) do copy $(.path.dll)\%i $(INSTALLLIB)

# Refresh all the source & header files from the DEVSRC directory
srcs::
   copy $(DEVSRC)\intl\*.h
   for %i in ($(S1)) do copy $(DEVSRC)\intl\%i
   for %i in ($(S2)) do copy $(DEVSRC)\intl\%i
   for %i in ($(S3)) do copy $(DEVSRC)\intl\%i
   for %i in ($(XFILES)) do copy $(DEVSRC)\intl\%i

