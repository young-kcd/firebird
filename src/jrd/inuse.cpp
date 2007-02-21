/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		inuse.c
 *	DESCRIPTION:	Keep track of objects that are in use
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
#include "../jrd/ib_stdio.h"
#include "../jrd/jrd.h"
#include "../jrd/gds_proto.h"
#include "../jrd/inuse_proto.h"
#include "../jrd/thd_proto.h"

static void cleanup(void *);
static void init(void);

static IUO free_list = NULL;
static MUTX_T inuse_mutex[1];
static BOOLEAN initialized = FALSE;


BOOLEAN INUSE_cleanup(IUO inuse, void (*cleanup_routine) ())
{
/**************************************
 *
 *	I N U S E _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Cleanup an in-use structure.  If any
 *	objects are found to be non-NULL,
 *	call the cleanup routine.  Return TRUE
 *	if there was something to do.
 *
 **************************************/
	void **ptr, **end;
	BOOLEAN needed_cleaning;
	IUO secondary_inuse;

	needed_cleaning = FALSE;
	secondary_inuse = inuse->iuo_next;
	do {
		ptr = inuse->iuo_object;
		for (end = ptr + inuse->iuo_in_use_count; ptr < end; ptr++)
			if (*ptr) {
				reinterpret_cast <
					void (*) (void *) >(*cleanup_routine) (*ptr);
				needed_cleaning = TRUE;
			}
		inuse->iuo_in_use_count = 0;
	} while (inuse = inuse->iuo_next);

	if (secondary_inuse) {
		IUO *secondary_end_ptr;

		for (secondary_end_ptr = &secondary_inuse->iuo_next;
			 *secondary_end_ptr;)
			secondary_end_ptr = &(*secondary_end_ptr)->iuo_next;
		THD_MUTEX_LOCK(inuse_mutex);
		*secondary_end_ptr = free_list;
		free_list = secondary_inuse;
		THD_MUTEX_UNLOCK(inuse_mutex);
	}

	return needed_cleaning;
}


void INUSE_clear(IUO inuse)
{
/**************************************
 *
 *	I N U S E _ c l e a r
 *
 **************************************
 *
 * Functional description
 *	Initialize an in-use block.
 *
 **************************************/

	if (!initialized)
		init();

	inuse->iuo_in_use_count = 0;
	inuse->iuo_next = NULL;
}


BOOLEAN INUSE_insert(IUO inuse, void *new_object, BOOLEAN dup_flag)
{
/**************************************
 *
 *	I N U S E _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert an object into an in-use structure.
 *	If dup_flag is TRUE, then duplicate entries
 *	are permitted.  If it is FALSE, then a
 *	duplicate entry will not be inserted.
 *	Return TRUE if the object was inserted.
 *
 **************************************/
	void **ptr, **end;

	ptr = inuse->iuo_object;
	for (end = ptr + inuse->iuo_in_use_count; ptr < end; ptr++)
		if (!*ptr) {
			if (!dup_flag)
				while (ptr < --end)
					if (*end == new_object)
						return FALSE;
			*ptr = new_object;
			return TRUE;
		}
		else if (!dup_flag && *ptr == new_object)
			return FALSE;

	if (inuse->iuo_in_use_count >= FB_NELEM(inuse->iuo_object)) {
		IUO new_inuse;

#ifdef DEV_BUILD
		gds__log("in-use block overflow. secondary block allocated.");
#endif
		THD_MUTEX_LOCK(inuse_mutex);
		if (new_inuse = free_list) {
			free_list = new_inuse->iuo_next;
			THD_MUTEX_UNLOCK(inuse_mutex);
		}
		else {
			THD_MUTEX_UNLOCK(inuse_mutex);
			new_inuse = (IUO) gds__alloc((SLONG) sizeof(struct iuo));
			/* FREE: at process exit, by cleanup handler cleanup() in this module */
			if (!new_inuse) {	/* NOMEM: */
				DEV_REPORT("INUSE_insert: out of memory");
				return FALSE;	/* error handling too difficult */
			}
		}
		INUSE_clear(new_inuse);
		inuse->iuo_next = new_inuse;
		inuse = new_inuse;
	}

	inuse->iuo_object[inuse->iuo_in_use_count++] = new_object;

	return TRUE;
}


BOOLEAN INUSE_remove(IUO inuse, void *old_object, BOOLEAN dup_flag)
{
/**************************************
 *
 *	I N U S E _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove an object from an in-use structure.
 *	If dup_flag is TRUE, remove every occurrence
 *	of the object.  Otherwise, only remove the
 *	first.  Return TRUE if an object was removed.
 *
 **************************************/
	void **ptr, **end;
	BOOLEAN removed;

	removed = FALSE;
	do {
		ptr = inuse->iuo_object;
		for (end = ptr + inuse->iuo_in_use_count; ptr < end; ptr++)
			if (*ptr == old_object) {
				*ptr = NULL;
				if (ptr + 1 == end)
					while (!*(--end) && --inuse->iuo_in_use_count);
				if (!dup_flag)
					return TRUE;
				removed = TRUE;
			}
	} while (inuse = inuse->iuo_next);

	return removed;
}


static void cleanup(void *arg)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IUO iuo;

	THD_MUTEX_DESTROY(inuse_mutex);

	while (free_list) {
		iuo = free_list;
		free_list = iuo->iuo_next;
		gds__free(iuo);
	}

	initialized = FALSE;
}


static void init(void)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (!initialized) {
		THD_INIT;
		THD_GLOBAL_MUTEX_LOCK;
		if (!initialized) {
			THD_MUTEX_INIT(inuse_mutex);
			gds__register_cleanup(cleanup, 0);
			initialized = TRUE;
		}
		THD_GLOBAL_MUTEX_UNLOCK;
	}
}
