/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blb_proto.h
 *	DESCRIPTION:	Prototype header file for blb.c
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

#ifndef _JRD_BLB_PROTO_H_
#define _JRD_BLB_PROTO_H_

#include "../jrd/jrd.h"
#include "../jrd/blb.h"
#include "../jrd/exe.h"
#include "../jrd/lls.h"
#include "../jrd/val.h"

#ifdef __cplusplus
//extern "C" {
#endif

extern void BLB_cancel(TDBB, struct blb *);
extern void BLB_close(TDBB, struct blb *);
extern struct blb *BLB_create(TDBB, struct tra *, struct bid *);
extern struct blb *BLB_create2(TDBB, struct tra *, struct bid *, USHORT,
							   UCHAR *);
extern void BLB_garbage_collect(TDBB, struct lls *, struct lls *, SLONG,
								struct rel *);
extern struct blb *BLB_get_array(TDBB, struct tra *, struct bid *,
								 struct ads *);
extern SLONG BLB_get_data(TDBB, struct blb *, UCHAR *, SLONG);
extern USHORT BLB_get_segment(TDBB, struct blb *, UCHAR *, USHORT);
extern SLONG BLB_get_slice(TDBB, struct tra *, struct bid *, UCHAR *, USHORT,
						   SLONG *, SLONG, UCHAR *);
extern SLONG BLB_lseek(struct blb *, USHORT, SLONG);
extern void DLL_EXPORT BLB_map_blobs(TDBB, struct blb *, struct blb *);
extern void BLB_move(TDBB, struct dsc *, struct dsc *, struct nod *);
extern struct blb *BLB_open(TDBB, struct tra *, struct bid *);
extern struct blb *BLB_open2(TDBB, struct tra *, struct bid *, USHORT,
							 UCHAR *);
extern void BLB_put_segment(TDBB, struct blb *, UCHAR *, USHORT);
extern void BLB_put_slice(TDBB, struct tra *, struct bid *, UCHAR *, USHORT,
						  SLONG *, SLONG, UCHAR *);
extern void BLB_release_array(struct arr *);
extern void BLB_scalar(TDBB, struct tra *, struct bid *, USHORT, SLONG *,
					   struct vlu *);

#ifdef __cplusplus
//} /* extern "C" */
#endif

#endif /* _JRD_BLB_PROTO_H_ */
