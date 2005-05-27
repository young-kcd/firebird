/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blb_proto.h
 *	DESCRIPTION:	Prototype header file for Jrd::blb.cpp
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
#include "../jrd/req.h"

void   BLB_cancel(Jrd::thread_db*, Jrd::blb*);
void   BLB_close(Jrd::thread_db*, Jrd::blb*);
Jrd::blb*   BLB_create(Jrd::thread_db*, Jrd::jrd_tra*, Jrd::bid*);
Jrd::blb*   BLB_create2(Jrd::thread_db*, Jrd::jrd_tra*, Jrd::bid*, USHORT, const UCHAR*);
void   BLB_garbage_collect(Jrd::thread_db*, Jrd::RecordStack&, Jrd::RecordStack&, SLONG, Jrd::jrd_rel*);
Jrd::blb*   BLB_get_array(Jrd::thread_db*, Jrd::jrd_tra*, const Jrd::bid*, Jrd::internal_array_desc*);
SLONG  BLB_get_data(Jrd::thread_db*, Jrd::blb*, UCHAR*, SLONG, bool = true);
USHORT BLB_get_segment(Jrd::thread_db*, Jrd::blb*, UCHAR*, USHORT);
SLONG  BLB_get_slice(Jrd::thread_db*, Jrd::jrd_tra*, const Jrd::bid*, const UCHAR*, USHORT,
	const SLONG*, SLONG, UCHAR*);
SLONG  BLB_lseek(Jrd::blb*, USHORT, SLONG);

void BLB_move(Jrd::thread_db*, dsc*, dsc*, Jrd::jrd_nod*);
void BLB_move_from_string(Jrd::thread_db*, const dsc*, dsc*, Jrd::jrd_nod*);
Jrd::blb* BLB_open(Jrd::thread_db*, Jrd::jrd_tra*, const Jrd::bid*);
Jrd::blb* BLB_open2(Jrd::thread_db*, Jrd::jrd_tra*, const Jrd::bid*, USHORT, const UCHAR*);
void BLB_put_segment(Jrd::thread_db*, Jrd::blb*, const UCHAR*, USHORT);
void BLB_put_slice(Jrd::thread_db*, Jrd::jrd_tra*, Jrd::bid*, const UCHAR*, USHORT,
	const SLONG*, SLONG, UCHAR*);
void BLB_release_array(Jrd::ArrayField*);
void BLB_scalar(Jrd::thread_db*, Jrd::jrd_tra*, const Jrd::bid*, USHORT, SLONG*, Jrd::impure_value*);


#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
void BLB_map_blobs(Jrd::thread_db*, Jrd::blb*, Jrd::blb*);
#endif

#endif	// JRD_BLB_PROTO_H

