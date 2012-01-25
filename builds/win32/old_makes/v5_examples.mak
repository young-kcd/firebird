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
#--------------------------- PC EXAMPLES MAKEFILE ---------------------------- 
ROOT=..
.path.c=$(ROOT)\example4

!include $(ROOT)\std.mk

#--------------------------- SOURCE COMPONENTS ----------------------------- 

H1 = example.h align.h
HDRS=$(H1) 

S1 = api1.c api2.c api3.c api4.c api5.c api6.c api7.c api8.c api9.c api9F
S2 = api10.c api11.c api12.c api13.c api15.c api16t.c apifull.c create2.sql
S3 = winevent.c winevent.rc winevent.def example.mak example.def readme

SRCS = $(S1) $(S2) $(S3)


# Extra files which are used to build the product
XFILES   = makefile.mak

#---------------------------- TARGET LIST --------------------------------
#---------------------------- UTILITIES ---------------------------------
# Refresh all the source & header files from the DEVSRC directory
srcs::
	for %i in ($(H1)) do copy $(DEVSRC)\example4\%i 
	for %i in ($(S1)) do copy $(DEVSRC)\example4\%i
	for %i in ($(S2)) do copy $(DEVSRC)\example4\%i
	for %i in ($(S3)) do copy $(DEVSRC)\example4\%i
	for %i in ($(XFILES)) do copy $(DEVSRC)\example4\%i


