/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		pass1_proto.h
 *	DESCRIPTION:	Prototype Header file for pass1.cpp
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

#ifndef DSQL_PASS1_PROTO_H
#define DSQL_PASS1_PROTO_H

namespace Jrd
{
	class CompoundStmtNode;
	class DeclareCursorNode;
	class ValueExprNode;
}

void PASS1_ambiguity_check(Jrd::DsqlCompilerScratch*, const Jrd::dsql_str*, const Jrd::DsqlContextStack&);
void PASS1_check_unique_fields_names(Jrd::StrArray& names, const Jrd::CompoundStmtNode* fields);
Jrd::dsql_nod* PASS1_compose(Jrd::dsql_nod*, Jrd::dsql_nod*, UCHAR);
Jrd::DeclareCursorNode* PASS1_cursor_name(Jrd::DsqlCompilerScratch*, const Firebird::MetaName&, USHORT, bool);
void PASS1_expand_select_node(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, Jrd::DsqlNodStack&, bool);
void PASS1_field_unknown(const TEXT*, const TEXT*, const Jrd::dsql_nod*);
Jrd::dsql_nod* PASS1_label(Jrd::DsqlCompilerScratch*, bool, Jrd::dsql_nod*);
Jrd::dsql_nod* PASS1_lookup_alias(Jrd::DsqlCompilerScratch*, const Jrd::dsql_str*, Jrd::dsql_nod*, bool);
Jrd::dsql_ctx* PASS1_make_context(Jrd::DsqlCompilerScratch* statement, const Jrd::dsql_nod* relation_node);
Jrd::dsql_nod* PASS1_node(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*);
bool PASS1_node_match(const Jrd::dsql_nod*, const Jrd::dsql_nod*, bool);
Jrd::dsql_nod* PASS1_node_psql(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, bool);
Jrd::dsql_nod* PASS1_post_map(Jrd::DsqlCompilerScratch*, Jrd::ValueExprNode*, Jrd::dsql_ctx*,
	Jrd::dsql_nod*, Jrd::dsql_nod*);
Jrd::dsql_nod* PASS1_post_map(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, Jrd::dsql_ctx*,
	Jrd::dsql_nod*, Jrd::dsql_nod*);
void PASS1_put_args_on_stack(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, Jrd::DsqlNodStack&);
Jrd::dsql_nod* PASS1_rse(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, Jrd::dsql_nod*);
bool PASS1_set_parameter_type(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*, Jrd::dsql_nod*, bool);
Jrd::dsql_nod* PASS1_statement(Jrd::DsqlCompilerScratch*, Jrd::dsql_nod*);

#endif // DSQL_PASS1_PROTO_H
