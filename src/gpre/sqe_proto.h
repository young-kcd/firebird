/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		sqe_proto.h
 *	DESCRIPTION:	Prototype header file for sqe.c
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

#ifndef _GPRE_SQE_PROTO_H_
#define _GPRE_SQE_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef NOD(*pfn_SQE_list_cb) (REQ, BOOLEAN, USHORT *, USHORT *);

extern NOD SQE_boolean(REQ, USHORT *);
extern CTX SQE_context(REQ);
extern NOD SQE_field(REQ, BOOLEAN);
extern NOD SQE_list(pfn_SQE_list_cb, REQ, BOOLEAN);
extern REF SQE_parameter(REQ, BOOLEAN);
extern void SQE_post_field(NOD, FLD);
extern REF SQE_post_reference(REQ, FLD, CTX, NOD);
extern BOOLEAN SQE_resolve(NOD, REQ, RSE);
extern RSE SQE_select(REQ, USHORT);
extern NOD SQE_value(REQ, BOOLEAN, USHORT *, USHORT *);
extern REF SQE_variable(REQ, BOOLEAN);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _GPRE_SQE_PROTO_H_ */
