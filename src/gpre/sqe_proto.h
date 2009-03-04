/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		sqe_proto.h
 *	DESCRIPTION:	Prototype header file for sqe.cpp
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

typedef GPRE_NOD (*pfn_SQE_list_cb) (gpre_req*, bool, USHORT*, bool*);

GPRE_NOD	SQE_boolean(gpre_req*, USHORT*);
gpre_ctx*	SQE_context(gpre_req*);
GPRE_NOD	SQE_field(gpre_req*, bool);
GPRE_NOD	SQE_list(pfn_SQE_list_cb, gpre_req*, bool);
REF			SQE_parameter(gpre_req*, bool);
void		SQE_post_field(GPRE_NOD, gpre_fld*);
REF			SQE_post_reference(gpre_req*, gpre_fld*, gpre_ctx*, GPRE_NOD);
bool		SQE_resolve(GPRE_NOD, gpre_req*, gpre_rse*);
gpre_rse*	SQE_select(gpre_req*, bool);
GPRE_NOD	SQE_value(gpre_req*, bool, USHORT*, bool*);
GPRE_NOD	SQE_variable(gpre_req*, bool, USHORT*, bool*);

#endif // GPRE_SQE_PROTO_H

