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

void   BLB_cancel(thread_db*, blb*);
void   BLB_close(thread_db*, blb*);
blb*   BLB_create(thread_db*, jrd_tra*, bid*);
blb*   BLB_create2(thread_db*, jrd_tra*, bid*, USHORT, const UCHAR*);
void   BLB_garbage_collect(thread_db*, lls*, lls*, SLONG, jrd_rel*);
blb*   BLB_get_array(thread_db*, jrd_tra*, const bid*, internal_array_desc*);
SLONG  BLB_get_data(thread_db*, blb*, UCHAR*, SLONG);
USHORT BLB_get_segment(thread_db*, blb*, UCHAR*, USHORT);
SLONG  BLB_get_slice(thread_db*, jrd_tra*, const bid*, const UCHAR*, USHORT,
	const SLONG*, SLONG, UCHAR*);
SLONG  BLB_lseek(blb*, USHORT, SLONG);

void BLB_move(thread_db*, dsc*, dsc*, jrd_nod*);
void BLB_move_from_string(thread_db*, const dsc*, dsc*, jrd_nod*);
blb* BLB_open(thread_db*, jrd_tra*, const bid*);
blb* BLB_open2(thread_db*, jrd_tra*, const bid*, USHORT, const UCHAR*);
void BLB_put_segment(thread_db*, blb*, const UCHAR*, USHORT);
void BLB_put_slice(thread_db*, jrd_tra*, bid*, const UCHAR*, USHORT,
	const SLONG*, SLONG, UCHAR*);
void BLB_release_array(ArrayField*);
void BLB_scalar(thread_db*, jrd_tra*, const bid*, USHORT, SLONG*, vlu*);


#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
void BLB_map_blobs(thread_db*, blb*, blb*);
#endif

#endif	// JRD_BLB_PROTO_H

