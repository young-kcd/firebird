/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		idx_proto.h
 *	DESCRIPTION:	Prototype header file for idx.cpp
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

#ifndef JRD_IDX_PROTO_H
#define JRD_IDX_PROTO_H

#include "../jrd/btr.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"


void IDX_check_access(TDBB, class Csb *, struct jrd_rel *, struct jrd_rel *,
							 struct jrd_fld *);
void IDX_create_index(TDBB, struct jrd_rel *, struct idx *, UCHAR *,
							 USHORT *, struct jrd_tra *, float *);
struct idb *IDX_create_index_block(TDBB, struct jrd_rel *, USHORT);
void IDX_delete_index(TDBB, struct jrd_rel *, USHORT);
void IDX_delete_indices(TDBB, struct jrd_rel *);
enum idx_e IDX_erase(TDBB, struct rpb *, struct jrd_tra *, struct jrd_rel **,
							USHORT *);
void IDX_garbage_collect(TDBB, struct rpb *, struct lls *,
								struct lls *);
enum idx_e IDX_modify(struct tdbb *, struct rpb *, struct rpb *,
							 struct jrd_tra *, struct jrd_rel **, USHORT *);
enum idx_e IDX_modify_check_constraints(TDBB, struct rpb *, struct rpb *,
											  struct jrd_tra *, struct jrd_rel **,
											  USHORT *);
float IDX_statistics(TDBB, struct jrd_rel *, USHORT);
enum idx_e IDX_store(struct tdbb *, struct rpb *, struct jrd_tra *,
							struct jrd_rel **, USHORT *);


#endif // JRD_IDX_PROTO_H
