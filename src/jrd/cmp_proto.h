/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cmp_proto.h
 *	DESCRIPTION:	Prototype header file for cmp.cpp
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

#ifndef JRD_CMP_PROTO_H
#define JRD_CMP_PROTO_H

#include "../jrd/req.h"
// req.h includes exe.h => Jrd::CompilerScratch and Jrd::CompilerScratch::csb_repeat.
#include "../jrd/scl.h"

namespace Jrd
{
	class RelationSourceNode;
}

UCHAR* CMP_alloc_map(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT);
Jrd::ValueExprNode* CMP_clone_node_opt(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::ValueExprNode*);
Jrd::BoolExprNode* CMP_clone_node_opt(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::BoolExprNode*);
Jrd::ValueExprNode* CMP_clone_node(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::ValueExprNode*);
Jrd::jrd_req* CMP_compile2(Jrd::thread_db*, const UCHAR* blr, ULONG blr_length, bool internal_flag,
	USHORT = 0, const UCHAR* = NULL);
Jrd::CompilerScratch::csb_repeat* CMP_csb_element(Jrd::CompilerScratch*, USHORT);
void CMP_decrement_prc_use_count(Jrd::thread_db*, Jrd::jrd_prc*);
void CMP_expand_view_nodes(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, Jrd::ValueExprNodeStack&,
	UCHAR, bool);
void CMP_fini(Jrd::thread_db*);
const Jrd::Format* CMP_format(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT);
void CMP_get_desc(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::jrd_nod*, dsc*);
Jrd::IndexLock* CMP_get_index_lock(Jrd::thread_db*, Jrd::jrd_rel*, USHORT);
ULONG CMP_impure(Jrd::CompilerScratch*, ULONG);
Jrd::jrd_req* CMP_make_request(Jrd::thread_db*, Jrd::CompilerScratch*, bool);
Jrd::jrd_nod* CMP_make_validation(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT);
void CMP_mark_variant(Jrd::CompilerScratch*, USHORT stream);

Jrd::ValueExprNode* CMP_pass1(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::ValueExprNode* node);
Jrd::BoolExprNode* CMP_pass1(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::BoolExprNode* node);
Jrd::jrd_nod* CMP_pass1(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::jrd_nod* node);

Jrd::jrd_nod* CMP_pass1_expand_view(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, USHORT, bool);
Jrd::RelationSourceNode* CMP_pass1_update(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::jrd_rel*,
	const Jrd::trig_vec*, USHORT, USHORT, Jrd::SecurityClass::flags_t, Jrd::jrd_rel*, USHORT);

Jrd::ExprNode* CMP_pass2(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::ExprNode* node);
Jrd::ValueExprNode* CMP_pass2(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::ValueExprNode* node);
Jrd::BoolExprNode* CMP_pass2(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::BoolExprNode* node);
Jrd::jrd_nod* CMP_pass2(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, Jrd::jrd_nod* const node,
	Jrd::jrd_nod* parent);

Jrd::ItemInfo* CMP_pass2_validation(Jrd::thread_db*, Jrd::CompilerScratch*, const Jrd::Item&);

void CMP_post_access(Jrd::thread_db*, Jrd::CompilerScratch*, const Firebird::MetaName&, SLONG,
					 Jrd::SecurityClass::flags_t, SLONG type_name, const Firebird::MetaName&,
					 const Firebird::MetaName& = "");

void CMP_post_procedure_access(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::jrd_prc*);
void CMP_post_resource(Jrd::ResourceList*, void*, Jrd::Resource::rsc_s, USHORT);
Jrd::RecordSource* CMP_post_rse(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::RseNode*);

void CMP_post_trigger_access(Jrd::CompilerScratch*, Jrd::jrd_rel*, Jrd::ExternalAccess::exa_act, Jrd::jrd_rel*);

void CMP_release(Jrd::thread_db*, Jrd::jrd_req*);
void CMP_shutdown_database(Jrd::thread_db*);

#endif // JRD_CMP_PROTO_H
