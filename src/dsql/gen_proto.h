/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		gen_proto.h
 *	DESCRIPTION:	Prototype Header file for gen.cpp
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

#ifndef DSQL_GEN_PROTO_H
#define DSQL_GEN_PROTO_H

UCHAR	GEN_expand_buffer(dsql_req*, UCHAR);
void	GEN_expr(dsql_req*, dsql_nod*);
void	GEN_port(dsql_req*, dsql_msg*);
void	GEN_request(dsql_req*, dsql_nod*);
void	GEN_return(DSQL_REQ, const dsql_nod*, bool);
void	GEN_start_transaction(dsql_req*, const dsql_nod*);
void	GEN_statement(dsql_req*, dsql_nod*);

inline UCHAR stuff(dsql_req* request, const UCHAR byte)
{
	return ((BLOB_PTR*)request->req_blr < (BLOB_PTR*)request->req_blr_yellow) ?
		(*request->req_blr++ = byte) : GEN_expand_buffer(request, byte);
}

#endif //  DSQL_GEN_PROTO_H

