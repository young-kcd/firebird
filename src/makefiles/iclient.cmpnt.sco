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

COMP:InterBase:iclient:
description        = "iclient component"
version            = SI_V1.5
subpackages        = iclient
required           = iclient
dependencies       = 
distTreeRootSHARED = /usr/gds.sco6/custom/interclient
distTreeRootCLIENT = /usr/gds.sco6/custom/interclient
pkgFiles           = iclient.pkg

FILE_DEFAULT:
mode  = $DEFAULT_FILE_MODE
owner = $DEFAULT_FILE_OWNER
group = $DEFAULT_FILE_GROUP

DIR_DEFAULT:
mode  = $DEFAULT_DIR_MODE
owner = $DEFAULT_DIR_OWNER
group = $DEFAULT_DIR_GROUP

FIFO_DEFAULT:
mode  = $DEFAULT_FIFO_MODE
owner = $DEFAULT_FIFO_OWNER
group = $DEFAULT_FIFO_GROUP
