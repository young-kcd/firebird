/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		meta_proto.h
 *	DESCRIPTION:	Prototype header file for meta.epp
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

bool	MET_declare(dbb*, qli_fld*, const nam*);
void	MET_define_field(dbb*, qli_fld*);
void	MET_define_index(qli_syntax*);
void	MET_define_relation(qli_rel*, qli_rel*);
void	MET_define_sql_relation(qli_rel*);
void	MET_delete_database(dbb*);
void	MET_delete_field(dbb*, nam*);
void	MET_delete_index(dbb*, nam*);
void	MET_delete_relation(qli_rel*);
int		MET_dimensions(dbb*, const TEXT*);
void	MET_fields(qli_rel*);
void	MET_finish(dbb*);
int		MET_get_datatype (USHORT);
void	MET_index_info(const TEXT*, const TEXT*, SCHAR*);
void	MET_meta_commit(dbb*);
void	MET_meta_rollback(dbb*);
FRBRD*	MET_meta_transaction(dbb*, bool);
void	MET_modify_field(dbb*, qli_fld*);
void	MET_modify_index(qli_syntax*);
void	MET_modify_relation(qli_rel*, qli_fld*);
void	MET_ready(qli_syntax*, USHORT);
void	MET_shutdown (void);
void	MET_sql_alter_table(qli_rel*, qli_fld*);
void	MET_sql_cr_view(qli_syntax*);
void	MET_sql_grant(qli_syntax*);
void	MET_sql_revoke(qli_syntax*);
FRBRD*	MET_transaction(NOD_T, dbb*);

#endif // QLI_META_PROTO_H

