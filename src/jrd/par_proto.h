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

class jrd_nod;

jrd_nod*	PAR_blr(thread_db*, class jrd_rel*, const UCHAR*, class Csb*,
					class Csb**, class jrd_req**, const bool, USHORT);
USHORT		PAR_desc(class Csb*, struct dsc*);
jrd_nod*	PAR_gen_field(thread_db*, USHORT, USHORT);
jrd_nod*	PAR_make_field(thread_db*, class Csb*, USHORT, const TEXT*);
jrd_nod*	PAR_make_list(thread_db*, class lls*);
jrd_nod*	PAR_make_node(thread_db*, int);
class Csb*	PAR_parse(thread_db*, const UCHAR*, USHORT);
SLONG		PAR_symbol_to_gdscode(const char*);

#endif // JRD_PAR_PROTO_H

