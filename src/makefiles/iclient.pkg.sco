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
PKG:SharedControl:
description        = "iclient component description file"
distTreeRoot       = /usr/gds.sco6/custom/interclient
distTreeRootSHARED = /usr/gds.sco6/custom/interclient
distTreeRootCLIENT = /usr/gds.sco6/custom/interclient

DIR:SharedControl:SHARED:cntl:
mode  = 0755
owner = root
group = sys
flags = 

FILE:SharedControl:SHARED:cntl/ccs:
mode  = 0755
owner = root
group = sys
flags = 

PKG:iclient:
description        = "InterClient"
dependencies       = 
distTreeRootSHARED = /usr/gds.sco6/custom/interclient
distTreeRootCLIENT = /usr/gds.sco6/custom/interclient

FILE:iclient:SHARED:InterClient150_SI.tar:
mode  = 0755
owner = root
group = bin
flags = 
