/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		misc.c
 *	DESCRIPTION:	Miscellaneous useful routines
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
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../burp/burp.h"
#include "../burp/burp_proto.h"
#include "../burp/misc_proto.h"

#ifdef SUPERSERVER
#include "../jrd/thd_proto.h"
#endif


UCHAR *MISC_alloc_burp(ULONG size)
{
/**************************************
 *
 *	M I S C _ a l l o c _ b u r p
 *
 **************************************
 *
 * Functional description
 *	Allocate block of memory. Note that it always zeros out memory.
 *  This could be optimized.
 *
 **************************************/

	TGBL tdgbl = GET_THREAD_DATA;

/* Add some header space to store a list of blocks allocated for this gbak */
	size += ROUNDUP(sizeof(UCHAR *), ALIGNMENT);

	UCHAR* block = (UCHAR*)gds__alloc(size);

	if (!block)
		/* NOMEM: message & abort FREE: all items freed at gbak exit */
	{
		BURP_error(238, NULL, NULL, NULL, NULL, NULL);	/* msg 238: System memory exhaused */
		return NULL;
	}

	memset(block, 0, size);

/* FREE: We keep a linked list of all gbak memory allocations, which
 *       are then freed when gbak exits.  This is important for 
 *       NETWARE in particular.
 */
	*((UCHAR **) block) = tdgbl->head_of_mem_list;
	tdgbl->head_of_mem_list = block;

	return (block + ROUNDUP(sizeof(UCHAR *), ALIGNMENT));
}


void MISC_free_burp( void *free)
{
/**************************************
 *
 *	M I S C _ f r e e _ b u r p
 *
 **************************************
 *
 * Functional description
 *	Release an unwanted block.
 *
 **************************************/
	UCHAR **block;
	TGBL tdgbl;
	UCHAR **ptr;

	tdgbl = GET_THREAD_DATA;

	if (free != NULL) {
		/* Point at the head of the allocated block */
		block =
			(UCHAR **) ((UCHAR *) free - ROUNDUP(sizeof(UCHAR *), ALIGNMENT));

		/* Scan for this block in the list of blocks */
		for (ptr = &tdgbl->head_of_mem_list; *ptr; ptr = (UCHAR **) * ptr) {
			if (*ptr == (UCHAR *) block) {
				/* Found it - remove it from the list */
				*ptr = *block;

				/* and free it */
				gds__free((SLONG *) block);
				return;
			}
		}

		/* We should always find the block in the list */
		BURP_error(238, NULL, NULL, NULL, NULL, NULL);	/* msg 238: System memory exhausted */
		/* (too lazy to add a better message) */
	}
}


void MISC_terminate(UCHAR* from, UCHAR* to, ULONG length, ULONG max_length)
{
/**************************************
 *
 *	M I S C _ t e r m i n a t e
 *
 **************************************
 *
 * Functional description
 *	Null-terminate a possibly non-
 *	null-terminated string with max
 *	buffer room.
 *
 **************************************/

	if (length) {
		length = MIN(length, max_length - 1);
		do
			*to++ = *from++;
		while (--length);
		*to++ = '\0';
	}
	else {
		while (max_length-- && (*to++ = *from++));
		*--to = '\0';
	}
}

