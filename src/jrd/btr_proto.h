/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		btr_proto.h
 *	DESCRIPTION:	Prototype header file for btr.c
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

#ifdef __cplusplus
extern "C" {
#endif

USHORT		BTR_all(TDBB, struct jrd_rel *, struct idx **, struct idx **,
					  struct str **, SLONG *);
void		BTR_create(TDBB, struct jrd_rel *, struct idx *, USHORT, struct scb *,
					   float *);
void		BTR_delete_index(TDBB, struct win *, USHORT);
bool		BTR_description(JRD_REL, struct irt *, struct idx *, SSHORT);
void		BTR_evaluate(struct tdbb *, struct irb *, struct sbm **);
struct btn*	BTR_find_leaf(struct btr *, struct key *, UCHAR *, USHORT *,
								 int, bool);
struct btr*	BTR_find_page(struct tdbb *, struct irb *, struct win *,
								 struct idx *, struct key *, struct key *, bool);
void		BTR_insert(struct tdbb *, struct win *, struct iib *);
enum idx_e	BTR_key(struct tdbb *, struct jrd_rel *, struct rec *,
						  struct idx *, struct key *, idx_null_state *);
USHORT		BTR_key_length(struct jrd_rel *, struct idx *);
struct btn*	BTR_last_node(struct btr *, struct exp *, struct btx **);
struct btr*	BTR_left_handoff(struct tdbb *, struct win *, struct btr *,
									SSHORT);
USHORT		BTR_lookup(TDBB, struct jrd_rel *, USHORT, struct idx *);
void		BTR_make_key(struct tdbb *, USHORT, struct jrd_nod **, struct idx *,
						 struct key *, USHORT);
bool		BTR_next_index(TDBB, struct jrd_rel *, struct jrd_tra *, struct idx *,
							struct win *);
struct btn*	BTR_next_node(struct btn *, struct btx **);
struct btn*	BTR_previous_node(struct btn *, struct btx **);
void		BTR_remove(struct tdbb *, struct win *, struct iib *);
void		BTR_reserve_slot(TDBB, struct jrd_rel *, struct jrd_tra *, struct idx *);
float		BTR_selectivity(TDBB, struct jrd_rel *, USHORT);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // JRD_BTR_PROTO_H

