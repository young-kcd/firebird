/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		btr_proto.h
 *	DESCRIPTION:	Prototype header file for btr.cpp
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

#ifndef JRD_BTR_PROTO_H
#define JRD_BTR_PROTO_H

#include "../jrd/btr.h"
#include "../jrd/ods.h"
#include "../jrd/req.h"
#include "../jrd/exe.h"

struct btree_exp;
struct exp_index_buf;

USHORT	BTR_all(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::IndexDescAlloc**);
void	BTR_create(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::index_desc*, USHORT, Jrd::sort_context*, Jrd::SelectivityList&);
void	BTR_delete_index(Jrd::thread_db*, Jrd::win*, USHORT);
//USHORT	BTR_delete_node(Jrd::thread_db*, Ods::btree_page*, USHORT);
bool	BTR_description(Jrd::thread_db*, Jrd::jrd_rel*, Ods::index_root_page*, Jrd::index_desc*, SSHORT);
void	BTR_evaluate(Jrd::thread_db*, Jrd::IndexRetrieval*, Jrd::RecordBitmap**);
UCHAR*	BTR_find_leaf(Ods::btree_page*, Jrd::temporary_key*, UCHAR*, USHORT*, bool, bool);
Ods::btree_page*	BTR_find_page(Jrd::thread_db*, Jrd::IndexRetrieval*, Jrd::win*, Jrd::index_desc*,
								 Jrd::temporary_key*, Jrd::temporary_key*, bool);
void	BTR_insert(Jrd::thread_db*, Jrd::win*, Jrd::index_insertion*);
Jrd::idx_e	BTR_key(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::Record*, Jrd::index_desc*, Jrd::temporary_key*,
					Jrd::idx_null_state*);
USHORT	BTR_key_length(Jrd::jrd_rel*, Jrd::index_desc*);
UCHAR*	BTR_last_node(Ods::btree_page*, exp_index_buf*, btree_exp**);
Ods::btree_page*	BTR_left_handoff(Jrd::thread_db*, Jrd::win*, Ods::btree_page*, SSHORT);
USHORT	BTR_lookup(Jrd::thread_db*, Jrd::jrd_rel*, USHORT, Jrd::index_desc*);
void	BTR_make_key(Jrd::thread_db*, USHORT, Jrd::jrd_nod**, Jrd::index_desc*, Jrd::temporary_key*, bool);
bool	BTR_next_index(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::jrd_tra*, Jrd::index_desc*, Jrd::win*);
void	BTR_remove(Jrd::thread_db*, Jrd::win*, Jrd::index_insertion*);
void	BTR_reserve_slot(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::jrd_tra*, Jrd::index_desc*);
void	BTR_selectivity(Jrd::thread_db*, const Jrd::jrd_rel*, USHORT, Jrd::SelectivityList&);
void	BTR_complement_key(Jrd::temporary_key*);

#endif // JRD_BTR_PROTO_H

