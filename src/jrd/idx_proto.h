/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		idx_proto.h
 *	DESCRIPTION:	Prototype header file for idx.c
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

#ifndef _JRD_IDX_PROTO_H_
#define _JRD_IDX_PROTO_H_

#include "../jrd/btr.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void IDX_check_access(TDBB, class Csb *, struct jrd_rel *, struct jrd_rel *,
							 struct jrd_fld *);
extern void IDX_create_index(TDBB, struct jrd_rel *, struct idx *, UCHAR *,
							 USHORT *, struct jrd_tra *, float *);
extern struct idb *IDX_create_index_block(TDBB, struct jrd_rel *, UCHAR);
extern void IDX_delete_index(TDBB, struct jrd_rel *, USHORT);
extern void IDX_delete_indices(TDBB, struct jrd_rel *);
extern enum idx_e IDX_erase(TDBB, struct rpb *, struct jrd_tra *, struct jrd_rel **,
							USHORT *);
extern void IDX_garbage_collect(TDBB, struct rpb *, struct lls *,
								struct lls *);
extern enum idx_e IDX_modify(struct tdbb *, struct rpb *, struct rpb *,
							 struct jrd_tra *, struct jrd_rel **, USHORT *);
extern enum idx_e IDX_modify_check_constraints(TDBB, struct rpb *, struct rpb *,
											  struct jrd_tra *, struct jrd_rel **,
											  USHORT *);
extern float IDX_statistics(TDBB, struct jrd_rel *, USHORT);
extern enum idx_e IDX_store(struct tdbb *, struct rpb *, struct jrd_tra *,
							struct jrd_rel **, USHORT *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_IDX_PROTO_H_ */
