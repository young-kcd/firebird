/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		all.c
 *	DESCRIPTION:	Internal block allocator
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

#include "../pipe/pipe.h"

#define BLKDEF(type, root, tail) sizeof (struct root), tail,
static struct {
    SSHORT	typ_root_length;
    SSHORT	typ_tail_length;
    } PIPE_block_sizes[] =
    {
    0,0,

#include "../pipe/blk.h"
    0};

#include "../pipe/allp_proto.h"
#include "../jrd/gds_proto.h"


UCHAR *ALLP_alloc (
    ULONG	size)
{
/**************************************
 *
 *	A L L _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block.
 *
 **************************************/

return gds__alloc (size);
}

BLK ALLP_block (
    UCHAR	type,
    int		count)
{
/**************************************
 *
 *	A L L _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a block from a given pool and initialize the block.
 *	This is the primary block allocation routine.
 *
 **************************************/
register BLK	block;
register USHORT	size, *p;
USHORT		best_tail, tail;

if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
    abort();

/* Compute block length */

size = PIPE_block_sizes[type].typ_root_length;

if (tail = PIPE_block_sizes[type].typ_tail_length)
    size += count * tail;

block = (BLK) ALLP_alloc ((SLONG) size);
block->blk_type = type;
block->blk_length = size;

if (size -= sizeof (struct blk))
    memset ((SCHAR*) block + sizeof (struct blk), 0, size);

return block;
}

void ALLP_free (
    UCHAR	*block)
{
/**************************************
 *
 *	A L L _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Free a block.
 *
 **************************************/

gds__free (block);
}

void ALLP_release (
    BLK		block)
{
/**************************************
 *
 *	A L L _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a structured block.
 *
 **************************************/

ALLP_free (block);
}
