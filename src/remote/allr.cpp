/*
 *      PROGRAM:        JRD Remote Interface/Server
 *      MODULE:         allr.cpp
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
#include "gen/iberror.h"
#include "../remote/allr_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd.h"
#include "../common/classes/alloc.h"


static const struct
{
	blk_t	type;
	USHORT	typ_root_length;
	USHORT	typ_tail_length;
} REM_block_sizes[] =
{
	{type_MIN	, 0, 0},
	{type_vec	, sizeof(rem_vec)		, sizeof(((rem_vec*) NULL)->vec_object[0])},
	{type_rdb	, sizeof(rdb)		, 0},
	{type_fmt	, sizeof(rem_fmt)		, sizeof(dsc/*((rem_fmt*) NULL)->fmt_desc[0]*/)},
	{type_rrq	, sizeof(rrq)		, sizeof(((rrq*) NULL)->rrq_rpt [0])},
	{type_rtr	, sizeof(rtr)		, 0},
	{type_str	, sizeof(rem_str)		, 1},	// random string block
	{type_rbl	, sizeof(rbl)		, 1},
	{type_port	, sizeof(rem_port)		, 1},
	{type_msg	, sizeof(message)	, 1},
	{type_rsr	, sizeof(rsr)		, 0},
	{type_rvnt	, sizeof(rvnt)		, 0},
	{type_rpr	, sizeof(rpr)		, 0},
	{type_rmtque	, sizeof(rmtque)	, 0},

	{type_MAX,	0, 0}
};



#ifdef SUPERSERVER
SLONG allr_delta_alloc = 0;
#endif


//____________________________________________________________
//
//	Allocate a block.
//
#ifdef DEBUG_GDS_ALLOC
UCHAR* ALLR_alloc_debug(ULONG size, const TEXT* FileName, ULONG LineNumber)
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

	throw std::bad_alloc();
	return NULL;	/* compiler silencer */
}


//____________________________________________________________
//
//	Allocate a block from a given pool and initialize the block.
//	This is the primary block allocation routine.
//
#ifdef DEBUG_GDS_ALLOC
BLK ALLR_block_debug(UCHAR type, ULONG count, const TEXT* FileName, ULONG LineNumber)
#else
BLK ALLR_block(UCHAR type, ULONG count)
#endif
{
	if (type <= (UCHAR) type_MIN || type >= (UCHAR) type_MAX)
	{
		TRDB tdrdb = REM_get_thread_data();
		ISC_STATUS* status_vector = tdrdb->trdb_status_vector;

		if (status_vector)
		{
			TEXT errmsg[128];
			status_vector[0] = isc_arg_gds;
			status_vector[1] = isc_bug_check;
			status_vector[2] = isc_arg_string;
			status_vector[4] = isc_arg_end;
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
			    // This is suspicious. The address of a local, non-static var
				// is being passed. If REMOTE_save_status_strings() is not called,
				// then embedded srv clearly shouldn't try to read the status
				// vector where the exception is trapped, because "errmsg" would
				// vanish before that => illegal address in the stack.
				status_vector[3] = (ISC_STATUS) errmsg;
#ifndef EMBEDDED
				REMOTE_save_status_strings(tdrdb->trdb_status_vector);
#endif
			}
			Firebird::status_exception::raise();
		}
		throw std::bad_alloc();
	}

#pragma FB_COMPILER_MESSAGE("Warning: outdated assumption for 16-bit platforms")
	// Compute block length, recasting count to make sure the calculation
	// comes out right on 16-bit platforms (like MS-DOS or Win16).

	const USHORT ucount	= (USHORT)count;
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
	const UCHAR* q = (UCHAR*)block;
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
rem_vec* ALLR_vector(rem_vec** ptr, ULONG count)
{
	++count;

	rem_vec* vector = *ptr;

	if (!vector) {
		vector = *ptr = (rem_vec*) ALLR_block(type_vec, count);
		vector->vec_count = count;
		return vector;
	}

/* If it fits, do it */

	if (count <= vector->vec_count)
		return vector;

	rem_vec* new_vector = *ptr = (rem_vec*) ALLR_block(type_vec, count);
	new_vector->vec_count = count;

	blk** p = new_vector->vec_object;
	blk* const* q = vector->vec_object;
	const blk* const* const end = q + (int) vector->vec_count;
	while (q < end) {
		*p++ = *q++;
	}
	ALLR_release(vector);

	return new_vector;
}

