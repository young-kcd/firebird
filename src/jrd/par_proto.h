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
	class thread_db;
	struct ItemInfo;
	class DmlNode;
}

/* blr type classes */

const int OTHER			= 0;
const int STATEMENT		= 1;
const int TYPE_BOOL		= 2;
const int VALUE			= 3;
const int TYPE_RSE		= 4;
const int RELATION		= 5;
const int ACCESS_TYPE	= 6;

struct dsc;

Jrd::jrd_nod*	PAR_blr(Jrd::thread_db*, Jrd::jrd_rel*, const UCHAR*, Jrd::CompilerScratch*,
					Jrd::CompilerScratch**, Jrd::jrd_req**, const bool, USHORT);
USHORT			PAR_desc(Jrd::thread_db*, Jrd::CompilerScratch*, dsc*, Jrd::ItemInfo* = NULL);
Jrd::jrd_nod*	PAR_gen_field(Jrd::thread_db*, USHORT, USHORT);
Jrd::jrd_nod*	PAR_make_field(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, const Firebird::MetaName&);
Jrd::jrd_nod*	PAR_make_list(Jrd::thread_db*, Jrd::NodeStack&);
Jrd::jrd_nod*	PAR_make_node(Jrd::thread_db*, int);
Jrd::CompilerScratch*	PAR_parse(Jrd::thread_db*, const UCHAR*, USHORT, USHORT = 0, const UCHAR* = NULL);
SLONG			PAR_symbol_to_gdscode(const Firebird::string&);

typedef Jrd::DmlNode* (*NodeParseFunc)(Jrd::thread_db* tdbb, MemoryPool& pool, Jrd::CompilerScratch* csb);

Jrd::jrd_nod* PAR_parse_node(Jrd::thread_db* tdbb, Jrd::CompilerScratch* csb, USHORT expected,
	USHORT expected_optional = 0);
void PAR_register(UCHAR blr, NodeParseFunc parseFunc);
void PAR_syntax_error(Jrd::CompilerScratch* csb, const TEXT* string);

#endif // JRD_PAR_PROTO_H
