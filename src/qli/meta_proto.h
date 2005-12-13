/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		meta_proto.h
 *	DESCRIPTION:	Prototype header file for meta.c
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

#ifndef _QLI_META_PROTO_H_
#define _QLI_META_PROTO_H_

extern int	MET_declare (struct dbb *, struct qli_fld *, struct nam *);
extern void	MET_define_field (struct dbb *, struct qli_fld *);
extern void	MET_define_index (struct syn *);
extern void	MET_define_relation (struct qli_rel *, struct qli_rel *);
extern void	MET_define_sql_relation (struct qli_rel *);
extern void	MET_delete_database (struct dbb *);
extern void	MET_delete_field (struct dbb *, struct nam *);
extern void	MET_delete_index (struct dbb *, struct nam *);
extern void	MET_delete_relation (struct qli_rel *);
extern int	MET_dimensions (struct dbb *, TEXT *);
extern void	MET_fields (struct qli_rel *);
extern void	MET_finish (struct dbb *);
extern int	MET_get_datatype (USHORT);
extern void	MET_index_info (TEXT *, TEXT *, SCHAR *);
extern void	MET_meta_commit (struct dbb *);
extern void	MET_meta_rollback (struct dbb *);
extern FRBRD	*MET_meta_transaction (struct dbb *, int);
extern void	MET_modify_field (struct dbb *, struct qli_fld *);
extern void	MET_modify_index (struct syn *);
extern void	MET_modify_relation (struct qli_rel *, struct qli_fld *);
extern void	MET_ready (struct syn *, USHORT);
extern void	MET_shutdown (void);
extern void	MET_sql_alter_table (struct qli_rel *, struct qli_fld *);
extern void	MET_sql_cr_view (struct syn *);
extern void	MET_sql_grant (struct syn *);
extern void	MET_sql_revoke (struct syn *);
extern FRBRD	*MET_transaction (NOD_T, struct dbb *);

#endif /* _QLI_META_PROTO_H_ */
