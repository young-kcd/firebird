/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		make_proto.h
 *	DESCRIPTION:	Prototype Header file for make.c
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

#ifndef _DSQL_MAKE_PROTO_H_
#define _DSQL_MAKE_PROTO_H_

#include "../dsql/sym.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nod *MAKE_constant(struct str *, int);
struct nod *MAKE_str_constant(struct str *, SSHORT);
struct str *MAKE_cstring(CONST SCHAR *);
void MAKE_desc(struct dsc *, struct nod *);
void MAKE_desc_from_field(struct dsc *, struct fld *);
struct nod *MAKE_field(struct ctx *, struct fld *, struct nod *);
struct nod *MAKE_list(struct lls *);
struct nod *MAKE_node(ENUM nod_t, int);
struct par *MAKE_parameter(struct msg *, USHORT, USHORT);
struct str *MAKE_string(CONST UCHAR *, int);
struct sym *MAKE_symbol(struct dbb *, CONST TEXT *, USHORT,
						   ENUM sym_type, struct req *);
struct str *MAKE_tagged_string(CONST UCHAR *, int, CONST TEXT *);
struct nod *MAKE_variable(struct fld *, CONST TEXT *, USHORT, USHORT,
								 USHORT, USHORT);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _DSQL_MAKE_PROTO_H_ */
