/*
 *	PROGRAM:	Interprocess server interface
 *	MODULE:		alli.c
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

#include <windows.h>

#include "../ipserver/ipc.h"

#define BLKDEF(type, root, tail) sizeof (struct root), tail,
static struct {
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} PIPE_block_sizes[] = {
	0, 0,
#include "../ipserver/blk.h"
0};

#include "../jrd/gds_proto.h"
#include "../ipserver/alli_proto.h"



UCHAR *ALLI_alloc(ULONG size)
{
/**************************************
 *
 *	A L L I _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block.
 *
 **************************************/

	return gds__alloc(size);
}


BLK ALLI_block(UCHAR type, int count)
{
/**************************************
 *
 *	A L L I _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Allocate a block from a given pool and initialize the block.
 *	This is the primary block allocation routine.
 *
 **************************************/
	register BLK block;
	register USHORT size;
	USHORT tail;

/*

if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
    abort();

*/

/* Compute block length */

	size = PIPE_block_sizes[type].typ_root_length;

	if (tail = PIPE_block_sizes[type].typ_tail_length)
		size += count * tail;

	block = (BLK) ALLI_alloc((SLONG) size);
	if (!block)
		return block;
	block->blk_type = type;
	block->blk_length = size;

	if (size -= sizeof(struct blk))
		memset((SCHAR *) block + sizeof(struct blk), 0, size);

	return block;
}


void ALLI_free( UCHAR * block)
{
/**************************************
 *
 *	A L L I _ f r e e
 *
 **************************************
 *
 * Functional description
 *	Free a block.
 *
 **************************************/

	gds__free((ULONG *) block);
}


void ALLI_release( BLK block)
{
/**************************************
 *
 *	A L L I _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a structured block.
 *
 **************************************/

	ALLI_free((UCHAR *) block);
}
