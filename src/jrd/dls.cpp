/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         dls.c
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
#include "../jrd/isc_proto.h"

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

/* external function directory list */

static FDLS first_fdls = { 0, { 0 } };
static BOOLEAN fdls_mutex_init = FALSE;
static MUTX_T fdls_mutex[1];

/* external file directory list */

static EDLS     first_edls      = { 0, { 0 } };
static BOOLEAN  edls_mutex_init = FALSE;
static MUTX_T   edls_mutex[1];

BOOLEAN DLS_get_temp_space(ULONG size, SFB sfb)
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

	MDLS *ptr;
	BOOLEAN result = FALSE;

	assert(size > (ULONG) 0);
	assert(sfb);

	/* FIXME: temporary workaround until directory lists are handled
			  properly without need in ISC_get_config. Don't know why,
			  but Nickolay's one in jrd.cpp didn't work for SINIX-Z.
			  (dimitr - 2002.12.13) */
	static bool is_initialized = false;
	if (!is_initialized) {
		ISC_get_config(LOCK_HEADER, 0);
		is_initialized = true;
	}

	ptr = DLS_get_access();

	if (!ptr->mdls_mutex_init) {
		V4_MUTEX_INIT(ptr->mdls_mutex);
		ptr->mdls_mutex_init = TRUE;
	}

	V4_MUTEX_LOCK(ptr->mdls_mutex);
	if (!sfb->sfb_dls) {
		/* allocate temp. space starting search from the begining of the dls list */
		for (sfb->sfb_dls = ptr->mdls_dls;
			 sfb->sfb_dls;
			 sfb->sfb_dls = sfb->sfb_dls->dls_next)
  if (size <= (sfb->sfb_dls->dls_size - sfb->sfb_dls->dls_inuse)) {
				sfb->sfb_dls->dls_inuse += size;
				result = TRUE;
				break;
			}
	}
	else {
		/* allocate temp. space from the current dls entry */
		if (size <= (sfb->sfb_dls->dls_size - sfb->sfb_dls->dls_inuse)) {
			sfb->sfb_dls->dls_inuse += size;
			result = TRUE;
		}
	}
	V4_MUTEX_UNLOCK(ptr->mdls_mutex);

	return (result);
}


void DLS_put_temp_space(SFB sfb)
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

	MDLS *ptr;

	if (sfb && sfb->sfb_dls) {
		ptr = DLS_get_access();
		assert(ptr->mdls_mutex_init);
		V4_MUTEX_LOCK(ptr->mdls_mutex);
		assert(sfb->sfb_dls->dls_inuse >= sfb->sfb_file_size);
		if (sfb->sfb_dls->dls_inuse > sfb->sfb_file_size)
			sfb->sfb_dls->dls_inuse -= sfb->sfb_file_size;
		else
			sfb->sfb_dls->dls_inuse = 0;
		V4_MUTEX_UNLOCK(ptr->mdls_mutex);
	}
}


BOOLEAN API_ROUTINE DLS_add_dir(ULONG size, TEXT * dir_name)
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

	MDLS *mdls;
	DLS new_dls, dls;

/* allocate dls structure */

	new_dls = (DLS) gds__alloc((SLONG) (sizeof(struct dls) +
										sizeof(TEXT) * strlen(dir_name)));
	if (!new_dls)
		return FALSE;

	strcpy(new_dls->dls_directory, dir_name);
	new_dls->dls_size = size;
	new_dls->dls_inuse = 0;
	new_dls->dls_next = (DLS) NULL;

/* get access to directory list object */

	mdls = DLS_get_access();

/* lock mutex, initialize it in case of the first access */

	if (!mdls->mdls_mutex_init) {
		V4_MUTEX_INIT(mdls->mdls_mutex);
		mdls->mdls_mutex_init = TRUE;
	}

	V4_MUTEX_LOCK(mdls->mdls_mutex);

/* add new entry to the end of list */

	if (!mdls->mdls_dls) {
		mdls->mdls_dls = new_dls;
	}
	else {
		dls = mdls->mdls_dls;
		while (dls->dls_next)
			dls = dls->dls_next;
		dls->dls_next = new_dls;
	}

/* release lock */

	V4_MUTEX_UNLOCK(mdls->mdls_mutex);

	return TRUE;
}


MDLS *DLS_get_access(void)
{
/**************************************
 *
 *      d l s _ _ g e t _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *      Return pointer to the temporary file configuration
 *
 **************************************/

	return (&DLS_cfg_tmpdir);
}

BOOLEAN DLS_add_func_dir(TEXT * directory_name)
{
/**************************************
 *
 *      D L S _ a d d _ f u n c _ d i r
 *
 **************************************
 *
 * Functional description
 *      Add an external function directory name to the list
 *      Return TRUE if successful, FALSE if no memory available.
 *
 **************************************/
	FDLS *current_fdls, *new_fdls;
	size_t name_length;

	name_length = strlen(directory_name);

	new_fdls = (FDLS *) gds__alloc((SLONG) (sizeof(FDLS) + name_length));
	if (!new_fdls)
		return FALSE;

	new_fdls->fdls_next = 0;
	strcpy(new_fdls->fdls_directory, directory_name);

	/* Lock the mutex for the directory list, initializing it if
	   this is the first access. */

	if (!fdls_mutex_init) {
		V4_MUTEX_INIT(fdls_mutex);
		fdls_mutex_init = TRUE;
	}

	/* Note that when the distinction is implemented, the
	   following should be a write lock. */
	V4_MUTEX_LOCK(fdls_mutex);

	/* Append the new fdls to the list, then release the mutex */
	current_fdls = &first_fdls;
	while (current_fdls->fdls_next)
		current_fdls = current_fdls->fdls_next;
	current_fdls->fdls_next = new_fdls;

	/* Release the (write) lock for the fdls linked list. */
	V4_MUTEX_UNLOCK(fdls_mutex);

	return TRUE;
}


FDLS *DLS_get_func_dirs(void)
{
/**************************************
 *
 *      D L S _ g e t _ f u n c _ d i r s
 *
 **************************************
 *
 * Functional description
 *      Return pointer to the first function directory entry
 *
 **************************************/

	return first_fdls.fdls_next;
}

