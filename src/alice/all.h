/*
 *	PROGRAM:	Alice
 *	MODULE:		all.h
 *	DESCRIPTION:	Data allocation declarations
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

#ifndef _ALICE_ALL_H_
#define _ALICE_ALL_H_

#if ALIGNMENT == 8
#define MIN_ALLOC	8
#else
#define MIN_ALLOC	4
#endif

#define MAX_BLOCK	256000
#define	FUDGE		1
#define SHIFT		2

#define MIN_ALLOCATION 1024		/* Minimin allocation from operating system */

/* This is the header to be used by all blocks to
   specify the type of block, the pool in which it has
   been allocated, and the length of its tail. */

#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif

/* Free block */

typedef struct frb
{
	struct blk frb_header;
	struct frb *frb_next;		/* Next free block in pool */
} *FRB;

/* Pool block */

typedef struct plb
{
	struct blk plb_header;
	USHORT plb_pool_id;			/* pool id */
	struct frb *plb_free;		/* first free block */
	struct hnk *plb_hunks;		/* first hunk block */
	struct lls *plb_lls;		/* avaiable linked list stack nodes */
} *PLB;

/* Hunk blocks */

typedef struct hnk
{
	struct blk hnk_header;
	SCHAR *hnk_address;			/* start of memory hunk */
	SLONG hnk_length;			/* length of memory hunk */
	struct hnk *hnk_next;		/* next memory hunk in structure */
} *HNK;

#endif /* _ALICE_ALL_H_ */
