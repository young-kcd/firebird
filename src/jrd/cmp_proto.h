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
// req.h includes exe.h => Csb and Csb::csb_repeat.

bool CMP_clone_is_active(const jrd_req*);
jrd_nod* CMP_clone_node(thread_db*, Csb*, jrd_nod*);
jrd_req* CMP_clone_request(thread_db*, jrd_req*, USHORT, bool);
jrd_req* CMP_compile(USHORT, const UCHAR*, USHORT);
jrd_req* CMP_compile2(thread_db*, const UCHAR*, USHORT);
Csb::csb_repeat* CMP_csb_element(Csb*, USHORT);
void CMP_expunge_transaction(jrd_tra*);
void CMP_decrement_prc_use_count(thread_db*, jrd_prc*);
jrd_req* CMP_find_request(thread_db*, USHORT, USHORT);
void CMP_fini(thread_db*);
fmt* CMP_format(thread_db*, Csb*, USHORT);
void CMP_get_desc(thread_db*, Csb*, jrd_nod*, dsc*);
idl* CMP_get_index_lock(thread_db*, jrd_rel*, USHORT);
SLONG CMP_impure(Csb*, USHORT);
jrd_req* CMP_make_request(thread_db*, Csb*);
void CMP_post_access(thread_db*, Csb*, const TEXT*, SLONG,
					 const TEXT*, const TEXT*, USHORT, const TEXT*,
					 const TEXT*);
void CMP_post_resource(thread_db*, Resource**, blk*, enum Resource::rsc_s, USHORT);
void CMP_release_resource(Resource**, enum Resource::rsc_s, USHORT);
void CMP_release(thread_db*, jrd_req*);
void CMP_shutdown_database(thread_db*);

#endif // JRD_CMP_PROTO_H

