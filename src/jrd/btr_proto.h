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

USHORT	BTR_all(TDBB, jrd_rel*, idx**, idx**, str**, SLONG*);
void	BTR_create(TDBB, jrd_rel*, idx*, USHORT, scb*, SelectivityList&);
void	BTR_delete_index(TDBB, win*, USHORT);
//USHORT	BTR_delete_node(TDBB, btr*, USHORT);
bool	BTR_description(jrd_rel*, irt*, idx*, SSHORT);
void	BTR_evaluate(tdbb*, irb*, sbm**);
UCHAR*	BTR_find_leaf(btr*, key*, UCHAR*, USHORT*, int, bool);
btr*	BTR_find_page(tdbb*, irb*, win*, idx*, key*, key*, bool);
void	BTR_insert(tdbb*, win*, iib*);
enum idx_e	BTR_key(tdbb*, jrd_rel*, rec*, idx*, key*, idx_null_state*);
USHORT	BTR_key_length(jrd_rel*, idx*);
UCHAR*	BTR_last_node(btr*, jrd_exp*, struct btx**);
btr*	BTR_left_handoff(tdbb*, win*, btr*, SSHORT);
USHORT	BTR_lookup(TDBB, jrd_rel*, USHORT, idx*);
void	BTR_make_key(tdbb*, USHORT, jrd_nod**, idx*, key*, USHORT);
bool	BTR_next_index(TDBB, jrd_rel*, jrd_tra*, idx*, win*);
void	BTR_remove(tdbb*, win*, iib*);
void	BTR_reserve_slot(TDBB, jrd_rel*, jrd_tra*, idx*);
void	BTR_selectivity(TDBB, jrd_rel*, USHORT, SelectivityList&);

#endif // JRD_BTR_PROTO_H
