/*
 *	PROGRAM:	Preprocessor
 *	MODULE:		sql_proto.h
 *	DESCRIPTION:	Prototype header file for sql.cpp
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

#ifndef GPRE_SQL_PROTO_H
#define GPRE_SQL_PROTO_H

ACT		SQL_action(const TEXT*);
void	SQL_adjust_field_dtype(GPRE_FLD);
void	SQL_init(void);
void	SQL_par_field_collate(GPRE_REQ, GPRE_FLD);
void	SQL_par_field_dtype(GPRE_REQ, GPRE_FLD, bool);
GPRE_PRC SQL_procedure(GPRE_REQ, TEXT *, TEXT *, TEXT *, bool);
GPRE_REL SQL_relation(GPRE_REQ, TEXT *, TEXT *, TEXT *, bool);
void	SQL_relation_name(TEXT *, TEXT *, TEXT *);
void	SQL_resolve_identifier(TEXT *, TEXT *);
TEXT*	SQL_var_or_string(bool);

#endif // GPRE_SQL_PROTO_H
