/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         dls_proto.h
 *      DESCRIPTION:    Temporary file management
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * 26-Sept-2001 Paul Beach - External File Directory Config. Parameter
 */

#ifndef _JRD_DLS_PROTO_H_
#define _JRD_DLS_PROTO_H_

#include "../jrd/sort.h"

extern BOOLEAN DLS_get_temp_space(ULONG, SFB);
extern void DLS_put_temp_space(SFB);
extern BOOLEAN API_ROUTINE DLS_add_dir(ULONG, TEXT *);
extern MDLS *DLS_get_access(void);
extern BOOLEAN DLS_add_func_dir(TEXT *);
extern FDLS *DLS_get_func_dirs(void);

#endif /* _JRD_DLS_PROTO_H_ */
