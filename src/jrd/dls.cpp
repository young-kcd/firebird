/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         dls.cpp
 *      DESCRIPTION:    Temporary file management
 *			The file contains set of functions that
 *			are used to manage directory list.
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
 * 26-Sept-2001 Paul Beach - External File Directory Config. Parameter
 */

#include "firebird.h"
#include <string.h>
#include <stdlib.h>
#include "../jrd/sort.h"
#include "../jrd/dls_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/file_params.h"
#include "../common/config/dir_list.h"

using namespace Jrd;

static MDLS DLS_cfg_tmpdir = { NULL, FALSE };	/* directory list object */

/*  Note that the only kind of writing permitted on the fdls list is
 *  appending a new entry to the existing (possibly empty) list.
 *  Therefore, we only need a write-lock to protect against
 *  simultaneous updates: readers don't need a lock because they'll
 *  always see either the old or the new state of the list, never an
 *  in-between state.
 *
 *  Also note that for this argument to be correct, it is necessary
 *  that updaters NULL out the "next" pointer of a new element
 *  *before*, not after, they append the element to the list, so that
 *  the list is at all times terminated by a NULL pointer.
 */


bool DLS_get_temp_space(ULONG size, sort_work_file* sfb)
{
/**************************************
 *
 *      D L S _ g e t _ t e m p _ s p a c e
 *
 **************************************
 *
 * Functional description
 *	Allocate disk space with required size
 *
 **************************************/

	bool result = false;

	fb_assert(size > (ULONG) 0);
	fb_assert(sfb);

	MDLS* ptr = DLS_get_access();

#ifdef V4_THREADING
	if (!ptr->mdls_mutex_init) {
		V4_MUTEX_INIT(ptr->mdls_mutex);
		ptr->mdls_mutex_init = true;
	}

	V4_MUTEX_LOCK(ptr->mdls_mutex);
#endif
	if (!sfb->sfb_dls) {
		/* allocate temp. space starting search from the begining of the dls list */
		for (sfb->sfb_dls = ptr->mdls_dls;
			 sfb->sfb_dls;
			 sfb->sfb_dls = sfb->sfb_dls->dls_next)
		{
			if (size <= (sfb->sfb_dls->dls_size - sfb->sfb_dls->dls_inuse)) {
				sfb->sfb_dls->dls_inuse += size;
				result = true;
				break;
			}
		}
	}
	else {
		/* allocate temp. space from the current dls entry */
		if (size <= (sfb->sfb_dls->dls_size - sfb->sfb_dls->dls_inuse)) {
			sfb->sfb_dls->dls_inuse += size;
			result = true;
		}
	}
#ifdef V4_THREADING
	V4_MUTEX_UNLOCK(ptr->mdls_mutex);
#endif

	return (result);
}


void DLS_put_temp_space(sort_work_file* sfb)
{
/**************************************
 *
 *      D L S _ p u t _ t e m p _ s p a c e
 *
 **************************************
 *
 * Functional description
 *	Release disk space occupied by sort file
 *
 **************************************/
	if (sfb && sfb->sfb_dls) {
		MDLS* ptr = DLS_get_access();
#ifdef V4_THREADING
		fb_assert(ptr->mdls_mutex_init);
		V4_MUTEX_LOCK(ptr->mdls_mutex);
#endif
		fb_assert(sfb->sfb_dls->dls_inuse >= sfb->sfb_file_size);
		if (sfb->sfb_dls->dls_inuse > sfb->sfb_file_size)
			sfb->sfb_dls->dls_inuse -= sfb->sfb_file_size;
		else
			sfb->sfb_dls->dls_inuse = 0;
#ifdef V4_THREADING
		V4_MUTEX_UNLOCK(ptr->mdls_mutex);
#endif
	}
}


bool API_ROUTINE DLS_add_dir(ULONG size, const TEXT* dir_name)
{
/**************************************
 *
 *      D L S _ a d d _ d i r
 *
 **************************************
 *
 * Functional description
 *      Add new entry to the temporary directory list.
 *
 **************************************/

/* allocate dls structure */

	dls* new_dls = (DLS) gds__alloc((SLONG) (sizeof(dls) +
										sizeof(TEXT) * strlen(dir_name)));
	if (!new_dls)
		return false;

	strcpy(new_dls->dls_directory, dir_name);
	new_dls->dls_size = size;
	new_dls->dls_inuse = 0;
	new_dls->dls_next = NULL;

/* get access to directory list object */

	MDLS* mdls = DLS_get_access();

#ifdef V4_THREADING
/* lock mutex, initialize it in case of the first access */

	if (!mdls->mdls_mutex_init) {
		V4_MUTEX_INIT(mdls->mdls_mutex);
		mdls->mdls_mutex_init = true;
	}

	V4_MUTEX_LOCK(mdls->mdls_mutex);
#endif

/* add new entry to the end of list */

	if (!mdls->mdls_dls) {
		mdls->mdls_dls = new_dls;
	}
	else {
		dls* dls_iterator = mdls->mdls_dls;
		while (dls_iterator->dls_next)
			dls_iterator = dls_iterator->dls_next;
		dls_iterator->dls_next = new_dls;
	}

#ifdef V4_THREADING
/* release lock */

	V4_MUTEX_UNLOCK(mdls->mdls_mutex);
#endif

	return true;
}


MDLS* DLS_get_access(void)
{
/**************************************
 *
 *      D L S _ g e t _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *      Return pointer to the temporary file configuration
 *
 **************************************/

	static bool is_initialized = false;
	if (!is_initialized) {
		is_initialized = true;
		Firebird::TempDirectoryList dir_list;
		for (size_t i = 0; i < dir_list.Count(); i++) {
			DLS_add_dir(dir_list[i].second, dir_list[i].first.c_str());
		}
	}

	return (&DLS_cfg_tmpdir);
}

