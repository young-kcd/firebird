/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		pass1_proto.h
 *	DESCRIPTION:	Prototype Header file for pass1.c
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

#ifndef _DSQL_PASS1_PROTO_H_
#define _DSQL_PASS1_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern struct dsql_ctx *PASS1_make_context(struct dsql_req *, struct dsql_nod *);
extern struct dsql_nod *PASS1_node(struct dsql_req *, struct dsql_nod *, bool);
extern struct dsql_nod *PASS1_rse(struct dsql_req *, struct dsql_nod *, struct dsql_nod *, DSQL_NOD update_lock);
extern struct dsql_nod *PASS1_statement(struct dsql_req *, struct dsql_nod *, bool);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _DSQL_PASS1_PROTO_H_ */
