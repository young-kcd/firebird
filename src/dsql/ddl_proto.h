/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		ddl_proto.h
 *	DESCRIPTION:	Prototype Header file for ddl_proto.h
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

#ifndef _DSQL_DDL_PROTO_H_
#define _DSQL_DDL_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

void DDL_execute(struct req *);
void DDL_generate(struct req *, struct nod *);
int DDL_ids(struct req *);
void DDL_put_field_dtype(struct req *, struct fld *, USHORT);
void DDL_resolve_intl_type(struct req *, struct fld *, struct str *);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif /* _DSQL_DDL_PROTO_H_ */
