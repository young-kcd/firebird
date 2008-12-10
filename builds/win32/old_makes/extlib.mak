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
.AUTODEPEND
ROOT = ..
.path.c=$(ROOT)\extlib

FUNCLIB_FILES=	ib_udf.c

APPBUILD=1

!include $(ROOT)\std.mk

CFLAGS=		-v -c -a4 -tWM -tWCD -DWIN32 


ib_udf.dll: ib_udf.obj
# build a small argument file and use it
	@echo $(TOOLPATH)\lib\c0d32.obj+ > link.arg
        @echo ib_udf.obj >> link.arg
        @echo .\$(.path.obj)\ib_udf.dll >> link.arg
	@echo /v /c /x /ap /Tpd >> link.arg 
        @echo $(TOOLPATH)\lib\import32.lib+ >> link.arg
	@echo $(TOOLPATH)\lib\cw32mt.lib >> link.arg
	$(TLINK) @link.arg
	$(RM) link.arg

ib_udf.obj: ib_udf.c
	$(CC) $(CFLAGS) ib_udf.c 


