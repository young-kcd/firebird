/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		gen_proto.h
 *	DESCRIPTION:	Prototype Header file for gen.c
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

#ifndef _DSQL_GEN_PROTO_H_
#define _DSQL_GEN_PROTO_H_

extern UCHAR GEN_expand_buffer(struct dsql_req *, UCHAR);
extern void GEN_expr(struct dsql_req *, struct dsql_nod *);
extern void GEN_port(struct dsql_req *, struct dsql_msg *);
extern void GEN_request(struct dsql_req *, struct dsql_nod *);
extern void GEN_return(DSQL_REQ, DSQL_NOD, bool);
extern void GEN_start_transaction(struct dsql_req *, struct dsql_nod *);
extern void GEN_statement(struct dsql_req *, struct dsql_nod *);

#endif /*  _DSQL_GEN_PROTO_H_  */
