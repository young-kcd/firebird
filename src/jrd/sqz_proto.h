/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sqz_proto.h
 *	DESCRIPTION:	Prototype header file for sqz.c
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
 */

#ifndef JRD_SQZ_PROTO_H
#define JRD_SQZ_PROTO_H

#include "../jrd/req.h"
#include "../jrd/sqz.h"

#ifdef __cplusplus
extern "C" {
#endif

USHORT	SQZ_apply_differences(struct rec*, SCHAR*, SCHAR*);
USHORT	SQZ_compress(class Dcc*, const SCHAR*, SCHAR*, int);
USHORT	SQZ_compress_length(class Dcc*, SCHAR*, int);
SCHAR*	SQZ_decompress(const SCHAR*, USHORT, SCHAR*, const SCHAR*);
USHORT	SQZ_differences(SCHAR*, USHORT, SCHAR*, USHORT, SCHAR*, int);
USHORT	SQZ_no_differences(SCHAR*, int);
void	SQZ_fast(class Dcc*, SCHAR*, SCHAR*);
USHORT	SQZ_length(TDBB, SCHAR*, int, class Dcc*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_SQZ_PROTO_H
