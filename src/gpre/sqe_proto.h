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

#ifndef GPRE_SQE_PROTO_H
#define GPRE_SQE_PROTO_H

typedef GPRE_NOD(*pfn_SQE_list_cb) (GPRE_REQ, bool, USHORT *, bool *);

extern GPRE_NOD SQE_boolean(GPRE_REQ, USHORT *);
extern GPRE_CTX SQE_context(GPRE_REQ);
extern GPRE_NOD SQE_field(GPRE_REQ, bool);
extern GPRE_NOD SQE_list(pfn_SQE_list_cb, GPRE_REQ, bool);
extern REF SQE_parameter(GPRE_REQ, bool);
extern void SQE_post_field(GPRE_NOD, GPRE_FLD);
extern REF SQE_post_reference(GPRE_REQ, GPRE_FLD, GPRE_CTX, GPRE_NOD);
extern bool SQE_resolve(GPRE_NOD, GPRE_REQ, GPRE_RSE);
extern GPRE_RSE SQE_select(GPRE_REQ, bool);
extern GPRE_NOD SQE_value(GPRE_REQ, bool, USHORT *, bool *);
extern GPRE_NOD SQE_variable(GPRE_REQ, bool, USHORT *, bool *);

#endif /* GPRE_SQE_PROTO_H */

