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

#include "firebird.h"
#include <windows.h>

#include "../ipserver/ipc.h"

#define BLKDEF(type, root, tail) {sizeof (struct root), tail},
static struct
{
	SSHORT typ_root_length;
	SSHORT typ_tail_length;
} PIPE_block_sizes[] = {
	{0, 0},

#include "../ipserver/blk.h"
	{0, 0}
};

#include "../jrd/gds_proto.h"
#include "../ipserver/alli_proto.h"


//____________________________________________________________
//
// Allocate a block.
//
void* ALLI_alloc(ULONG size)
{
	return gds__alloc(size);
}


//____________________________________________________________
//
// Allocate a block from a given pool and initialize the block.
// This is the primary block allocation routine.
//
BLK ALLI_block(UCHAR type, int count)
{
/*

if (type <= (SCHAR) type_MIN || type >= (SCHAR) type_MAX)
    abort();

*/

/* Compute block length */

	const USHORT tail = PIPE_block_sizes[type].typ_tail_length;
	USHORT size = PIPE_block_sizes[type].typ_root_length + count * tail;

	BLK block = (BLK) ALLI_alloc((SLONG) size);

	if (block)
	{
		block->blk_type = type;
		block->blk_length = size;

		size -= sizeof(struct blk);
		if (size)
		{
			memset((SCHAR *) block + sizeof(struct blk), 0, size);
		}
	}

	return block;
}


//____________________________________________________________
//
// Free a block.
//
void ALLI_free(void* block)
{
	gds__free(block);
}


//____________________________________________________________
//
// Release a structured block.
//
void ALLI_release(BLK block)
{
	ALLI_free(block);
}
