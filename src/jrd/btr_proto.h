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

#ifndef _JRD_BTR_PROTO_H_
#define _JRD_BTR_PROTO_H_

#include "../jrd/btr.h"
#include "../jrd/ods.h"
#include "../jrd/req.h"

USHORT	BTR_all(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::idx**, Jrd::idx**, str**, SLONG*);
void	BTR_create(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::idx*, USHORT, Jrd::sort_context*, Jrd::SelectivityList&);
void	BTR_delete_index(Jrd::thread_db*, Jrd::win*, USHORT);
//USHORT	BTR_delete_node(Jrd::thread_db*, Ods::btree_page*, USHORT);
bool	BTR_description(Jrd::jrd_rel*, Ods::index_root_page*, Jrd::idx*, SSHORT);
void	BTR_evaluate(Jrd::thread_db*, Jrd::IndexRetrieval*, Jrd::SparseBitmap**);
UCHAR*	BTR_find_leaf(Ods::btree_page*, Jrd::key*, UCHAR*, USHORT*, bool, bool);
Ods::btree_page*	BTR_find_page(Jrd::thread_db*, Jrd::IndexRetrieval*, Jrd::win*, Jrd::idx*, Jrd::key*, Jrd::key*, bool);
void	BTR_insert(Jrd::thread_db*, Jrd::win*, Jrd::iib*);
Jrd::idx_e	BTR_key(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::Record*, Jrd::idx*, Jrd::key*, Jrd::idx_null_state*);
USHORT	BTR_key_length(Jrd::jrd_rel*, Jrd::idx*);
UCHAR*	BTR_last_node(Ods::btree_page*, struct jrd_exp*, struct btx**);
Ods::btree_page*	BTR_left_handoff(Jrd::thread_db*, Jrd::win*, Ods::btree_page*, SSHORT);
USHORT	BTR_lookup(Jrd::thread_db*, Jrd::jrd_rel*, USHORT, Jrd::idx*);
void	BTR_make_key(Jrd::thread_db*, USHORT, Jrd::jrd_nod**, Jrd::idx*, Jrd::key*, bool);
bool	BTR_next_index(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::jrd_tra*, Jrd::idx*, Jrd::win*);
void	BTR_remove(Jrd::thread_db*, Jrd::win*, Jrd::iib*);
void	BTR_reserve_slot(Jrd::thread_db*, Jrd::jrd_rel*, Jrd::jrd_tra*, Jrd::idx*);
void	BTR_selectivity(Jrd::thread_db*, Jrd::jrd_rel*, USHORT, Jrd::SelectivityList&);

#endif // JRD_BTR_PROTO_H

