/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		par_proto.h
 *	DESCRIPTION:	Prototype header file for par.cpp
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

#ifndef JRD_PAR_PROTO_H
#define JRD_PAR_PROTO_H

namespace Jrd {
	class jrd_nod;
	class CompilerScratch;
	class jrd_rel;
	class jrd_req;
	class JrdStatement;
	class thread_db;
	struct ItemInfo;
	class DmlNode;
	class SortNode;
}

// blr type classes

const int OTHER			= 0;
const int STATEMENT		= 1;
const int TYPE_BOOL		= 2;
const int VALUE			= 3;
const int TYPE_RSE		= 4;
const int RELATION		= 5;
const int ACCESS_TYPE	= 6;

struct dsc;

Jrd::jrd_nod*	PAR_args(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, UCHAR, USHORT);
Jrd::jrd_nod*	PAR_args(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT);
Jrd::jrd_nod*	PAR_blr(Jrd::thread_db*, Jrd::jrd_rel*, const UCHAR*, ULONG blr_length,
						Jrd::CompilerScratch*, Jrd::CompilerScratch**, Jrd::JrdStatement**, const bool,
						USHORT);
SSHORT			PAR_context(Jrd::CompilerScratch*, SSHORT*);
void			PAR_dependency(Jrd::thread_db*, Jrd::CompilerScratch*, SSHORT, SSHORT,
	const Firebird::MetaName&);
USHORT			PAR_desc(Jrd::thread_db*, Jrd::CompilerScratch*, dsc*, Jrd::ItemInfo* = NULL);
void			PAR_error(Jrd::CompilerScratch*, const Firebird::Arg::StatusVector&, bool isSyntaxError = true);
Jrd::jrd_nod*	PAR_gen_field(Jrd::thread_db*, USHORT, USHORT);
Jrd::jrd_nod*	PAR_make_field(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, const Firebird::MetaName&);
Jrd::jrd_nod*	PAR_make_list(Jrd::thread_db*, Jrd::NodeStack&);
Jrd::jrd_nod*	PAR_make_node(Jrd::thread_db*, int);
USHORT			PAR_name(Jrd::CompilerScratch*, Firebird::MetaName&);
size_t			PAR_name(Jrd::CompilerScratch* csb, Firebird::string& name);
Jrd::CompilerScratch*	PAR_parse(Jrd::thread_db*, const UCHAR* blr, ULONG blr_length,
	bool internal_flag, USHORT = 0, const UCHAR* = NULL);

void			PAR_procedure_parms(Jrd::thread_db*, Jrd::CompilerScratch*, Jrd::jrd_prc*,
	Jrd::jrd_nod**, Jrd::jrd_nod**, bool);
Jrd::jrd_nod*	PAR_rse(Jrd::thread_db*, Jrd::CompilerScratch*, SSHORT);
Jrd::SortNode* PAR_sort(Jrd::thread_db*, Jrd::CompilerScratch*, UCHAR, bool);
Jrd::SortNode* PAR_sort_internal(Jrd::thread_db*, Jrd::CompilerScratch*, UCHAR blrOp,
	USHORT);
SLONG			PAR_symbol_to_gdscode(const Firebird::string&);

typedef Jrd::DmlNode* (*NodeParseFunc)(Jrd::thread_db* tdbb, MemoryPool& pool,
	Jrd::CompilerScratch* csb, UCHAR blrOp);

Jrd::jrd_nod* PAR_parse_node(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, USHORT expected);
void PAR_register(UCHAR blr, NodeParseFunc parseFunc);
void PAR_syntax_error(Jrd::CompilerScratch* csb, const TEXT* string);
void PAR_warning(const Firebird::Arg::StatusVector& v);

#endif // JRD_PAR_PROTO_H
