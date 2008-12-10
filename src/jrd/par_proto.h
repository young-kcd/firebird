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
}

struct dsc;

Jrd::jrd_nod*	PAR_blr(Jrd::thread_db*, Jrd::jrd_rel*, const UCHAR*, Jrd::CompilerScratch*,
					Jrd::CompilerScratch**, Jrd::jrd_req**, const bool, USHORT);
USHORT			PAR_desc(Jrd::CompilerScratch*, dsc*);
Jrd::jrd_nod*	PAR_gen_field(Jrd::thread_db*, USHORT, USHORT);
Jrd::jrd_nod*	PAR_make_field(Jrd::thread_db*, Jrd::CompilerScratch*, USHORT, const Firebird::MetaName&);
Jrd::jrd_nod*	PAR_make_list(Jrd::thread_db*, Jrd::NodeStack&);
Jrd::jrd_nod*	PAR_make_node(Jrd::thread_db*, int);
Jrd::CompilerScratch*	PAR_parse(Jrd::thread_db*, const UCHAR*, USHORT);
SLONG			PAR_symbol_to_gdscode(const Firebird::MetaName&);

#endif // JRD_PAR_PROTO_H

