/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		all.h
 *	DESCRIPTION:	Block allocator blocks
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

#ifndef _JRD_ALL_H_
#define _JRD_ALL_H_

#if ALIGNMENT == 8
#define MIN_ALLOC	8
#else
#define MIN_ALLOC	4
#endif

/* MAX_BLOCK for OS/2 is calculated by taking the max memory
   available in a segment (65534 bytes), subtracting 4 bytes
   for a length, 8 bytes for a free block, and 2 bytes to 
   allow rounding to a 4 byte boundary */

#if (defined PC_PLATFORM && !defined NETWARE_386)
#define MAX_BLOCK	65520
#else
#define MAX_BLOCK	(262144 - MIN_ALLOCATION - sizeof (struct hnk) - 8)
#endif

#define	FUDGE		1

#define SHIFT		SHIFTLONG

#define MIN_ALLOCATION	1024	/* Minimum allocation from operating system */

/* Prevent shared memory pools from fragmenting into very long
   free block lists. Any hunk residual larger than a free block
   structure will lengthen the free list. A larger extend size
   reduces the number of times the pool is extended and reduces
   the length of the free block list. */

#ifdef SUPERSERVER
#define PERM_EXTEND_SIZE	(16 * MIN_ALLOCATION)
#define CACH_EXTEND_SIZE	(16 * MIN_ALLOCATION)
#else
#define PERM_EXTEND_SIZE	MIN_ALLOCATION
#define CACH_EXTEND_SIZE	MIN_ALLOCATION
#endif

#define EXTEND(vector_count)	vector_count + vector_count % 10



/* Free block */

typedef struct frb {
	struct blk frb_header;
	struct frb *frb_next;		/* Next free block in pool */
} *FRB;

/* Pool block */

typedef struct plb {
	struct blk plb_header;
	USHORT plb_pool_id;			/* pool id */
	USHORT plb_extend_size;		/* pool extend size */
	struct frb *plb_free;		/* first free block */
	struct hnk *plb_hunks;		/* first hunk block */
	struct hnk *plb_huge_hunks;	/* first huge hunk block (blocks > MAX_BLOCK) */
	struct lls *plb_lls;		/* available linked list stack nodes */
	struct dcc *plb_dccs;		/* available data compression control blocks */
	struct sbm *plb_buckets;	/* available bit map buckets */
	struct bms *plb_segments;	/* available bit map segments */
	MUTX plb_mutex[1];
	SLONG *plb_blk_type_count;	/* array to keep track of block types */
} *PLB;

/* Hunk blocks */

typedef struct hnk {
	struct blk hnk_header;
	SCHAR *hnk_address;			/* start of memory hunk */
	SLONG hnk_length;			/* length of memory hunk */
	struct hnk *hnk_next;		/* next memory hunk in structure */
} *HNK;

#endif /* _JRD_ALL_H_ */
