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
#--------------------------- PC MSGS MAKEFILE ----------------------------
ROOT=..
.path.c=$(ROOT)\msgs

WIN=1
DEBUG=1
APPBUILD=1
!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS -----------------------------

SRCS = build_fi.c

# Extra files which are used to build the product
XFILES   = makefile.mak

#---------------------------- TARGET LIST --------------------------------
OBJS=$(TOOLPATH)\lib\c0wl.obj $(SRCS:.c=.obj)
BUILDLIBS=$(ROOT)\jrd\$(.path.obj)\gds.lib

all: extern_libs build.exe 

#------------------------------ RULES -------------------------------------

# Makes the build.exe application which reads the msg database and
# builds the message file.
build.exe: $(OBJS) $(BUILDLIBS) build.rsp
   tlink $(LFLAGS) @$(.path.obj)\$&.rsp @&&|
   $<
   $*.map
   $(BUILDLIBS) cw$(MODEL) mathw$(MODEL) import.lib
   $&.def
|

build.rsp: $(OBJS)
	copy &&|
	$**
| $<

# Because of the circular "architecture" here, we need to build 
# required libraries within each of the makefiles.
extern_libs:
	cd $(ROOT)\jrd
	make $(MAKEFLAGS) alllibs
	cd $(ROOT)\msgs


#---------------------------- UTILITIES ---------------------------------
# Refresh all the source & header files from the DEVSRC directory
srcs::
   for %i in ($(SRCS)) do copy $(DEVSRC)\msgs\%i
   for %i in ($(XFILES)) do copy $(DEVSRC)\msgs\%i

