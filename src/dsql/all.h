/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		all.h
 *	DESCRIPTION:	Allocator prototypes
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

#include "../dsql/dsql.h"

/* alloc and init a block from a pool */
BLK ALLD_alloc(PLB pool, UCHAR type, int count);

/* extend a block by given size */
BLK ALLD_extend(BLK* pointer, int size);

int		ALLD_fini();				/* get rid of everything */
int		ALLD_free(void* memory);	/* give space back to system */
int		ALLD_init();				/* initialize pool system */
void*	ALLD_malloc(int size);		/* get memory from system */
PLB		ALLD_pool();				/* allocate a new pool */
int		ALLD_push(BLK object, LLS * stack);	/* push object on LLS stack */
BLK		ALLD_pop(LLS* stack);		/* pop object off LLS stack */
int		ALLD_release(FRB block);	/* release a block to its pool */
int		ALLD_rlpool(PLB pool);		/* release a pool */
