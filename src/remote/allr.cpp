/*
 *      PROGRAM:        JRD Remote Interface/Server
 *      MODULE:         allr.c
 *      DESCRIPTION:    Internal block allocator
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

#include "firebird.h"
#include <stdlib.h>
#include <string.h>
#include "../remote/remote.h"
#include "gen/codes.h"
#include "../remote/allr_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"


static const struct
{
	blk_t	type;
	USHORT	typ_root_length;
	USHORT	typ_tail_length;
} REM_block_sizes[] =
{
	{type_MIN	, 0, 0},
	{type_vec	, sizeof(vec)		, sizeof(((VEC) NULL)->vec_object[0])},
	{type_rdb	, sizeof(rdb)		, 0},
	{type_fmt	, sizeof(fmt)		, sizeof(((FMT) NULL)->fmt_desc[0])},
	{type_rrq	, sizeof(rrq)		, sizeof(((RRQ) NULL)->rrq_rpt [0])},
	{type_rtr	, sizeof(rtr)		, 0},
	{type_str	, sizeof(str)		, 1},	// random string block
	{type_rbl	, sizeof(rbl)		, 1},
	{type_port	, sizeof(port)		, 1},
	{type_msg	, sizeof(message)	, 1},
	{type_rsr	, sizeof(rsr)		, 0},
	{type_rvnt	, sizeof(rvnt)		, 0},
	{type_rpr	, sizeof(rpr)		, 0},
	{type_rmtque	, sizeof(rmtque)	, 0},

	{type_MIN,	0, 0}
};


extern "C" {


#ifdef SUPERSERVER
SLONG allr_delta_alloc = 0;
#endif


//____________________________________________________________
//
//	Allocate a block.
//
#ifdef DEBUG_GDS_ALLOC
UCHAR* ALLR_alloc_debug(ULONG size, TEXT* FileName, ULONG LineNumber)
#else
UCHAR* ALLR_alloc(ULONG size)
#endif
{
	UCHAR* block = (UCHAR*)
#ifdef DEBUG_GDS_ALLOC
		gds__alloc_debug((SLONG)size, FileName, LineNumber);
#else
		gds__alloc((SLONG) size);
#endif

	if (block)
	{
#ifdef SUPERSERVER
		allr_delta_alloc += size;
#endif
		return block;
	}

	// FREE: caller must free - usually using ALLR_release.
	// NOMEM: post a user level error, if we have a status vector,
	//        otherwise just an error return

	TRDB trdb = GET_THREAD_DATA;
	ISC_STATUS* status_vector = trdb->trdb_status_vector;
	if (status_vector) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_virmemexh;
		*status_vector = gds_arg_end;
	}

	Firebird::status_exception::raise(gds_virmemexh);
	return NULL;	/* compiler silencer */
}


//____________________________________________________________
//
//	Allocate a block from a given pool and initialize the block.
//	This is the primary block allocation routine.
//
#ifdef DEBUG_GDS_ALLOC
BLK ALLR_block_debug(UCHAR type, ULONG count, TEXT* FileName, ULONG LineNumber)
#else
BLK ALLR_block(UCHAR type, ULONG count)
#endif
{
	if (type <= (UCHAR) type_MIN || type >= (UCHAR) type_MAX)
	{
		TRDB	trdb			= GET_THREAD_DATA;
		ISC_STATUS*	status_vector	= trdb->trdb_status_vector;

		if (status_vector)
		{
			TEXT errmsg[128];
			status_vector[0] = gds_arg_gds;
			status_vector[1] = gds_bug_check;
			status_vector[2] = gds_arg_string;
			status_vector[4] = gds_arg_end;
			const SSHORT lookup_result =
				gds__msg_lookup(0,
								JRD_BUGCHK,
								150,
								sizeof(errmsg),
								errmsg,
								NULL);
			if (lookup_result < 1)
			{
				status_vector[3] =
					(ISC_STATUS) "request to allocate invalid block type";
			}
			else
			{
				status_vector[3] = (ISC_STATUS) errmsg;
#ifndef EMBEDDED
				REMOTE_save_status_strings(trdb->trdb_status_vector);
#endif
			}
		}

		Firebird::status_exception::raise(gds_bug_check);
	}

	// Compute block length, recasting count to make sure the calculation
	// comes out right on 16-bit platforms (like MS-DOS or Win16).

	USHORT ucount	= (USHORT)count;
	USHORT size		= REM_block_sizes[type].typ_root_length;
	USHORT tail		= REM_block_sizes[type].typ_tail_length;

	if (tail && ucount >= 1) {
		size += (ucount - 1) * tail;
	}

	BLK block = (BLK)
#ifdef DEBUG_GDS_ALLOC
		ALLR_alloc_debug((ULONG) size, FileName, LineNumber);
#else
		ALLR_alloc((ULONG) size);
#endif

	// NOMEM: callee handled
	// FREE:  caller must handle - use ALLR_release

	block->blk_type = type;
	block->blk_length = size;

	size -= sizeof(struct blk);

	if (size) {
		memset((char*)block + sizeof(struct blk), 0, size);
	}

	return block;
}


//____________________________________________________________
//
//	Clone a block.
//
//	Caller is responsible for free'ing the clone
//
BLK ALLR_clone(BLK block)
{
	USHORT l = block->blk_length;
	BLK clone = (BLK) ALLR_alloc((ULONG) l);

	// NOMEM: ALLR_alloc() handled
	// FREE:  caller must handle  - use ALLR_release

	UCHAR* p = (UCHAR*)clone;
	UCHAR* q = (UCHAR*)block;
	do {
		*p++ = *q++;
	} while (--l);

	return clone;
}


//____________________________________________________________
//
//	Free a block.
//
void ALLR_free( void *block)
{
#ifdef SUPERSERVER
	allr_delta_alloc -= gds__free(block);
#else
	gds__free(block);
#endif
}


//____________________________________________________________
//
//	Release a structured block.
//
void ALLR_release( void *block)
{
	ALLR_free(block);
}


//____________________________________________________________
//
//	Allocate a vector.
//
VEC ALLR_vector(VEC* ptr, ULONG count)
{
	++count;

	VEC vector = *ptr;

	if (!vector) {
		vector = *ptr = (VEC) ALLR_block(type_vec, count);
		vector->vec_count = count;
		return vector;
	}

/* If it fits, do it */

	if (count <= vector->vec_count)
		return vector;

	VEC new_vector = *ptr = (VEC) ALLR_block(type_vec, count);
	new_vector->vec_count = count;

	BLK* p = new_vector->vec_object;
	BLK* q = vector->vec_object;
	BLK* end = q + (int) vector->vec_count;
	while (q < end) {
		*p++ = *q++;
	}
	ALLR_release(vector);

	return new_vector;
}


} // extern "C"
