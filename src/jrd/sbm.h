/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm.h
 *	DESCRIPTION:	Sparse bit map block definitions
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef JRD_SBM_H
#define JRD_SBM_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../include/fb_vector.h"

#define BUNCH_BITS	LOG2_BITS_PER_LONG
#define BUNCH		SLONG
#define SEGMENT_BITS	10
#define BITS_BUNCH		(1 << BUNCH_BITS)
#define BITS_SEGMENT	(1 << SEGMENT_BITS)
#define BUNCH_SEGMENT	BITS_SEGMENT / BITS_BUNCH

#define BUCKET_BITS		15
#define BUNCH_BUCKET	(1 << (BUCKET_BITS - SEGMENT_BITS))

// Sparse bit map

class sbm : public pool_alloc<type_sbm>
{
    public:
	sbm(MemoryPool& p, int len)
	:	sbm_segments(len, p, type_sbm)
	{
	}

	sbm*			sbm_next;
	JrdMemoryPool*	sbm_pool;
	UCHAR			sbm_state;			// State of bitmap
	UCHAR			sbm_type;			// Root or bucket
	USHORT			sbm_count;			// Slots allocated
	USHORT			sbm_used;			// Slots used
	USHORT			sbm_high_water;		// Maximum slot used
	SLONG			sbm_number;			// Value when singular
	typedef			Firebird::vector<bms*>	vector_type;
	typedef			vector_type::iterator	iterator;

	vector_type				sbm_segments;
};
typedef sbm* SBM;

/* States */

#define	SBM_EMPTY	0
#define SBM_SINGULAR	1
#define SBM_PLURAL	2

/* Types */

#define SBM_BUCKET	0
#define SBM_ROOT	1

/* Bit map segment */

class bms : public pool_alloc<type_bms>
{
public:
	bms*			bms_next;
	JrdMemoryPool*	bms_pool;
	SSHORT			bms_min;			// Minimum bit set in segment
	SSHORT			bms_max;			// Maximum bit set in segment
	BUNCH			bms_bits[BUNCH_SEGMENT];
};
typedef bms* BMS;

#endif	// JRD_SBM_H
