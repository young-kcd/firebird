/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blb_proto.h
 *	DESCRIPTION:	Prototype header file for blb.cpp
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

#ifndef JRD_BLB_PROTO_H
#define JRD_BLB_PROTO_H

#include "../jrd/jrd.h"
#include "../jrd/blb.h"
#include "../jrd/exe.h"
#include "../jrd/lls.h"
#include "../jrd/val.h"

// fwd decl.
class blb;
struct jrd_tra;
struct bid;
struct lls;
struct jrd_rel;
struct dsc;
struct arr;
struct jrd_nod;
struct vlu;
struct ads;

#ifdef __cplusplus
extern "C" {
#endif

void   BLB_cancel(TDBB, blb*);
void   BLB_close(TDBB, blb*);
blb*   BLB_create(TDBB, jrd_tra*, bid*);
blb*   BLB_create2(TDBB, jrd_tra*, bid*, USHORT, UCHAR*);
void   BLB_garbage_collect(TDBB, lls*, lls*, SLONG, jrd_rel*);
blb*   BLB_get_array(TDBB, jrd_tra*, bid*, ads*);
SLONG  BLB_get_data(TDBB, blb*, UCHAR *, SLONG);
USHORT BLB_get_segment(TDBB, blb*, UCHAR*, USHORT);
SLONG  BLB_get_slice(TDBB, jrd_tra*, bid*, UCHAR*, USHORT, SLONG*, SLONG, UCHAR*);
SLONG  BLB_lseek(blb*, USHORT, SLONG);

void BLB_move(TDBB, const dsc*, dsc*, jrd_nod*);
void BLB_move_from_string(TDBB, const dsc*, dsc*, jrd_nod*);
blb* BLB_open(TDBB, jrd_tra*, bid*);
blb* BLB_open2(TDBB, jrd_tra*, bid*, USHORT,
						 UCHAR *);
void BLB_put_segment(TDBB, blb*, UCHAR *, USHORT);
void BLB_put_slice(TDBB, jrd_tra*, bid*, UCHAR*, USHORT, SLONG*, SLONG, UCHAR*);
void BLB_release_array(arr*);
void BLB_scalar(TDBB, jrd_tra*, bid*, USHORT, SLONG*, vlu*);
#ifdef __cplusplus
} // extern "C"
#endif


#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
extern "C" {
void BLB_map_blobs(TDBB, struct blb*, struct blb*);
}
#endif

#endif	// JRD_BLB_PROTO_H

