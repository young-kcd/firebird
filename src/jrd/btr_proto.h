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

USHORT	BTR_all(thread_db*, jrd_rel*, idx**, idx**, str**, SLONG*);
void	BTR_create(thread_db*, jrd_rel*, idx*, USHORT, sort_context*, SelectivityList&);
void	BTR_delete_index(thread_db*, win*, USHORT);
//USHORT	BTR_delete_node(thread_db*, btree_page*, USHORT);
bool	BTR_description(jrd_rel*, index_root_page*, idx*, SSHORT);
void	BTR_evaluate(thread_db*, IndexRetrieval*, SparseBitmap**);
UCHAR*	BTR_find_leaf(btree_page*, key*, UCHAR*, USHORT*, bool, bool);
btree_page*	BTR_find_page(thread_db*, IndexRetrieval*, win*, idx*, key*, key*, bool);
void	BTR_insert(thread_db*, win*, iib*);
enum idx_e	BTR_key(thread_db*, jrd_rel*, Record*, idx*, key*, idx_null_state*);
USHORT	BTR_key_length(jrd_rel*, idx*);
UCHAR*	BTR_last_node(btree_page*, jrd_exp*, struct btx**);
btree_page*	BTR_left_handoff(thread_db*, win*, btree_page*, SSHORT);
USHORT	BTR_lookup(thread_db*, jrd_rel*, USHORT, idx*);
void	BTR_make_key(thread_db*, USHORT, jrd_nod**, idx*, key*, bool);
bool	BTR_next_index(thread_db*, jrd_rel*, jrd_tra*, idx*, win*);
void	BTR_remove(thread_db*, win*, iib*);
void	BTR_reserve_slot(thread_db*, jrd_rel*, jrd_tra*, idx*);
void	BTR_selectivity(thread_db*, jrd_rel*, USHORT, SelectivityList&);

#endif // JRD_BTR_PROTO_H

