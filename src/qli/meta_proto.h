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

#ifndef QLI_META_PROTO_H
#define QLI_META_PROTO_H

extern int	MET_declare(dbb*, qli_fld*, nam*);
extern void	MET_define_field(dbb*, qli_fld*);
extern void	MET_define_index(syn*);
extern void	MET_define_relation(qli_rel*, qli_rel*);
extern void	MET_define_sql_relation(qli_rel*);
extern void	MET_delete_database(dbb*);
extern void	MET_delete_field(dbb*, nam*);
extern void	MET_delete_index(dbb*, nam*);
extern void	MET_delete_relation(qli_rel*);
extern int	MET_dimensions(dbb*, TEXT*);
extern void	MET_fields(qli_rel*);
extern void	MET_finish(dbb*);
extern int	MET_get_datatype (USHORT);
extern void	MET_index_info(const TEXT*, const TEXT*, SCHAR*);
extern void	MET_meta_commit(dbb*);
extern void	MET_meta_rollback(dbb*);
extern FRBRD*	MET_meta_transaction(dbb*, int);
extern void	MET_modify_field(dbb*, qli_fld*);
extern void	MET_modify_index(syn*);
extern void	MET_modify_relation(qli_rel*, qli_fld*);
extern void	MET_ready(syn*, USHORT);
extern void	MET_shutdown (void);
extern void	MET_sql_alter_table(qli_rel*, qli_fld*);
extern void	MET_sql_cr_view(syn*);
extern void	MET_sql_grant(syn*);
extern void	MET_sql_revoke(syn*);
extern FRBRD*	MET_transaction(NOD_T, dbb*);

#endif /* QLI_META_PROTO_H */

