/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sdw.c
 *	DESCRIPTION:	Disk Shadowing Manager
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
#include <string.h>
#include "../jrd/ib_stdio.h"

#include "../jrd/ibsetjmp.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "gen/codes.h"
#include "../jrd/jrn.h"
#include "../jrd/lls.h"
#include "../jrd/req.h"
#include "../jrd/os/pio.h"
#include "../jrd/all.h"
#include "../jrd/sdw.h"
#include "../jrd/sbm.h"
#include "../jrd/flags.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/sdw_proto.h"
#include "../jrd/thd_proto.h"


extern "C" {


static void activate_shadow(void);
static SDW allocate_shadow(struct fil *, USHORT, USHORT);
static BOOLEAN check_for_file(SCHAR *, USHORT);
#ifdef NOT_USED_OR_REPLACED
static void check_if_got_ast(struct fil *);
#endif
static void copy_header(void);
static void update_dbb_to_sdw(struct dbb *);


void SDW_add(TEXT * file_name, USHORT shadow_number, USHORT file_flags)
{
/**************************************
 *
 *	S D W _ a d d
 *
 **************************************
 *
 * Functional description
 *	Add a brand new shadowing file to the database.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	FIL shadow_file;
	SDW shadow;
	WIN window;

	tdbb = GET_THREAD_DATA;
	dbb = GET_DBB;

	shadow_file = PIO_create(dbb, file_name, strlen(file_name), FALSE);

	if (dbb->dbb_flags & DBB_force_write)
		PIO_force_write(shadow_file, TRUE);

	shadow = allocate_shadow(shadow_file, shadow_number, file_flags);

/* dump out the header page, even if it is a conditional 
   shadow--the page will be fixed up properly */

	if (shadow->sdw_flags & SDW_conditional)
		shadow->sdw_flags &= ~SDW_conditional;
	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	CCH_write_all_shadows(tdbb, 0, window.win_bdb,
						  tdbb->tdbb_status_vector, 1, FALSE);
	CCH_RELEASE(tdbb, &window);
	if (file_flags & FILE_conditional)
		shadow->sdw_flags |= SDW_conditional;
}


int SDW_add_file(TEXT * file_name, SLONG start, USHORT shadow_number)
{
/**************************************
 *
 *	S D W _ a d d _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a file to a shadow set.
 *	Return the sequence number for the new file.
 *
 **************************************/

	FIL shadow_file, next, file;
	SDW shadow;
	struct bdb temp_bdb;
	HDR header;
	SLONG sequence;
	SCHAR *spare_buffer = NULL, *spare_page;

	TDBB tdbb = GET_THREAD_DATA;
	DBB  dbb  = tdbb->tdbb_database;

/* Find the file to be extended */

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
	{
		if ((shadow->sdw_number == shadow_number) &&
			!(shadow->sdw_flags & (SDW_IGNORE | SDW_rollover)))
		{
			shadow_file = shadow->sdw_file;
			break;
		}
	}

	if (!shadow) {
		return 0;
	}

/* find the last file in the list, open the new file */

	for (file = shadow_file; file->fil_next; file = file->fil_next);
	if (!(sequence = PIO_add_file(dbb, shadow_file, file_name, start)))
		return 0;

	next = file->fil_next;

	if (dbb->dbb_flags & DBB_force_write)
		PIO_force_write(next, TRUE);

/* Always write the header page, even for a conditional
 * shadow that hasn't been activated.
 */

/* allocate a spare buffer which is large enough,
   and set up to release it in case of error. Align
   the spare page buffer for raw disk access. */

	spare_buffer = (SCHAR*)
		gds__alloc((SLONG) dbb->dbb_page_size + MIN_PAGE_SIZE);
	spare_page =
		(SCHAR *) (((U_IPTR) spare_buffer + MIN_PAGE_SIZE - 1) &
				   ~((U_IPTR) MIN_PAGE_SIZE - 1));

	try {

/* create the header using the spare_buffer */

	header = (HDR) spare_page;
	header->hdr_header.pag_type = pag_header;
	header->hdr_sequence = sequence;
	header->hdr_page_size = dbb->dbb_page_size;
	header->hdr_data[0] = HDR_end;
	header->hdr_end = HDR_SIZE;
	header->hdr_next_page = 0;

/* fool PIO_write into writing the scratch page into the correct place */

	temp_bdb.bdb_page = next->fil_min_page;
	temp_bdb.bdb_dbb = dbb;
	temp_bdb.bdb_buffer = (PAG) header;
	header->hdr_header.pag_checksum = CCH_checksum(&temp_bdb);
	if (!PIO_write(	shadow_file,
					&temp_bdb,
					reinterpret_cast<pag*>(header),
					0))
	{
		if (spare_buffer) {
			gds__free(spare_buffer);
		}
		return 0;
	}
	next->fil_fudge = 1;

/* Update the previous header page to point to new file --
   we can use the same header page, suitably modified,  
   because they all look pretty much the same at this point */

/*******************
   Fix for bug 7925. drop_gdb wan not dropping secondary file in 
   multi-shadow files. The structure was not being filled with the
   info. Commented some code so that the structure will always
   be filled.

	-Sudesh 07/06/95

   The original code :
===
if (shadow_file == file)
    copy_header();
else
===
************************/
/** Tempeorarly reverting the change ------- Sudesh 07/07/95 *******/
	if (shadow_file == file) {
		copy_header();
	} else {
		--start;
		header->hdr_data[0] = HDR_end;
		header->hdr_end = HDR_SIZE;
		header->hdr_next_page = 0;

		PAG_add_header_entry(header, HDR_file, strlen(file_name),
							 reinterpret_cast < UCHAR * >(file_name));
		PAG_add_header_entry(header, HDR_last_page, sizeof(start),
							 (UCHAR *) & start);
		file->fil_fudge = 0;
		temp_bdb.bdb_page = file->fil_min_page;
		header->hdr_header.pag_checksum = CCH_checksum(&temp_bdb);
		if (!PIO_write(	shadow_file,
						&temp_bdb,
						reinterpret_cast<pag*>(header),
						0))
		{
			if (spare_buffer) {
				gds__free(spare_buffer);
			}
			return 0;
		}
		if (file->fil_min_page) {
			file->fil_fudge = 1;
		}
	}

	if (file->fil_min_page) {
		file->fil_fudge = 1;
	}

	if (spare_buffer) {
		gds__free(spare_buffer);
	}

	}	// try
	catch (const std::exception&) {
		if (spare_buffer) {
			gds__free(spare_buffer);
		}
		ERR_punt();
	}

	return sequence;
}


void SDW_check(void)
{
/**************************************
 *
 *	S D W _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check a shadow to see if it needs to 
 *	be deleted or shut down.
 *
 **************************************/
	SDW shadow, next_shadow;
	//BOOLEAN start_conditional = TRUE;
	TDBB tdbb;
	DBB dbb;
	LCK lock;

	dbb = GET_DBB;
	tdbb = GET_THREAD_DATA;

/* first get rid of any shadows that need to be 
   deleted or shutdown; deleted shadows must also
   be shutdown 

   Check to see if there is a valid shadow in the shadow set,
   if not then it is time to start an conditional shadow (if
   one has been defined). */

	for (shadow = dbb->dbb_shadow; shadow; shadow = next_shadow) {
		next_shadow = shadow->sdw_next;

		if (shadow->sdw_flags & SDW_delete) {
			MET_delete_shadow(tdbb, shadow->sdw_number);
			gds__log
				("shadow %s deleted from database %s due to unavailability on write",
				 shadow->sdw_file->fil_string, dbb->dbb_file->fil_string);
		}

		/* note that shutting down a shadow is destructive to 
		   the shadow block */

		if (shadow->sdw_flags & SDW_shutdown)
			SDW_shutdown_shadow(shadow);
	}
	if (SDW_check_conditional()) {
		if (SDW_lck_update((SLONG) 0)) {
			lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) lck();
			lock->lck_dbb = dbb;
			lock->lck_attachment = tdbb->tdbb_attachment;
			lock->lck_length = sizeof(SLONG);
			lock->lck_key.lck_long = -1;
			lock->lck_type = LCK_update_shadow;
			lock->lck_owner_handle =
				LCK_get_owner_handle(tdbb, lock->lck_type);
			lock->lck_parent = dbb->dbb_lock;
			lock->lck_owner = (BLK) tdbb->tdbb_attachment;

			LCK_lock(tdbb, lock, LCK_EX, FALSE);
			if (lock->lck_physical == LCK_EX) {
				SDW_notify();
				SDW_dump_pages();
				LCK_release(tdbb, lock);
			}
			delete lock;
		}
	}
}


BOOLEAN SDW_check_conditional(void)
{
/**************************************
 *
 *	S D W _ c h e c k _ c o n d i t i o n a l
 *
 **************************************
 *
 * Functional description
 *	Check if a conditional shadow exists 
 *	if so update meta data and return true
 *
 **************************************/
	SDW shadow, next_shadow;
	BOOLEAN start_conditional = TRUE;
	USHORT file_flags;
	DBB dbb;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* first get rid of any shadows that need to be 
   deleted or shutdown; deleted shadows must also
   be shutdown 

   Check to see if there is a valid shadow in the shadow set,
   if not then it is time to start an conditional shadow (if
   one has been defined). */

	for (shadow = dbb->dbb_shadow; shadow; shadow = next_shadow) {
		next_shadow = shadow->sdw_next;

		if (!(shadow->sdw_flags & (SDW_delete | SDW_shutdown)))
			if (!(shadow->sdw_flags & SDW_INVALID)) {
				start_conditional = FALSE;
				break;
			}
	}

/* if there weren't any conventional shadows, now is
   the time to start the first conditional shadow in the list
   Note that allocate_shadow keeps the sdw_next list sorted */

	if (start_conditional)
		for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
			if ((shadow->sdw_flags & SDW_conditional) &&
				!(shadow->sdw_flags & (SDW_IGNORE | SDW_rollover))) {
				shadow->sdw_flags &= ~SDW_conditional;

				gds__log("conditional shadow %d %s activated for database %s",
						 shadow->sdw_number, shadow->sdw_file->fil_string,
						 dbb->dbb_file->fil_string);
				file_flags = FILE_shadow;
				if (shadow->sdw_flags & SDW_manual)
					file_flags |= FILE_manual;
				MET_update_shadow(tdbb, shadow, file_flags);
				return TRUE;
			}

	return FALSE;
}


void SDW_close(void)
{
/**************************************
 *
 *	S D W _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close all disk shadowing files associated with
 *	a database.
 *
 **************************************/
	DBB dbb;
	SDW shadow;

	dbb = GET_DBB;

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
		PIO_close(shadow->sdw_file);
}


void SDW_dump_pages(void)
{
/**************************************
 *
 *	S D W _ d u m p _ p a g e s
 *
 **************************************
 *
 * Functional description
 *	Look for any shadow files that haven't been written yet.
 *	Fetch pages from the database and write them
 *	to all unwritten shadow files.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	SDW shadow;
	WIN window;
	SLONG page_number, max;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	window.win_flags = 0;
	gds__log("conditional shadow dumped for database %s",
			 dbb->dbb_file->fil_string);
	max = PAG_last_page();

/* mark the first shadow in the list because we don't
   want to start shadowing to any files that are added
   while we are in the middle of dumping pages */

/* none of these pages should need any alteration
   since header pages for extend files are not handled at this level */

	window.win_flags = WIN_large_scan;
	window.win_scans = 1;

	for (page_number = HEADER_PAGE + 1; page_number <= max; page_number++) {
#ifdef SUPERSERVER_V2
		if (!(page_number % dbb->dbb_prefetch_sequence)) {
			SLONG i, number, pages[PREFETCH_MAX_PAGES];

			number = page_number;
			for (i = 0; i < dbb->dbb_prefetch_pages && number <= max;)
				pages[i++] = number++;

			CCH_PREFETCH(tdbb, pages, i);
		}
#endif
		for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
			if (!(shadow->sdw_flags & (SDW_INVALID | SDW_dumped))) {
				window.win_page = page_number;

				/* when copying a database, it is possible that there are some pages defined
				   in the pip that were never actually written to disk, in the case of a faked
				   page which was never written to disk because of a rollback; to prevent 
				   checksum errors on this type of page, don't check for checksum when the 
				   page type is 0 */

				CCH_FETCH_NO_CHECKSUM(tdbb, &window, LCK_read, pag_undefined);
				if (!CCH_write_all_shadows(tdbb, shadow, window.win_bdb,
										   tdbb->tdbb_status_vector, 1,
										   FALSE)) {
					CCH_RELEASE(tdbb, &window);
					ERR_punt();
				}
				if (shadow->sdw_next)
					CCH_RELEASE(tdbb, &window);
				else
					CCH_RELEASE_TAIL(tdbb, &window);
			}
	}

/* mark all shadows seen to this point as dumped */

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
		if (!(shadow->sdw_flags & SDW_INVALID))
			shadow->sdw_flags |= SDW_dumped;
}



void SDW_get_shadows(void)
{
/**************************************
 *
 *	S D W _ g e t _ s h a d o w s
 *
 **************************************
 *
 * Functional description
 *	Get any new shadows that have been
 *	defined.
 *
 **************************************/
	DBB dbb;
	LCK lock;
	HDR header;
	WIN window;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* unless we have one, get a shared lock to ensure that we don't miss any
   signals */

	dbb->dbb_ast_flags &= ~DBB_get_shadows;

	lock = dbb->dbb_shadow_lock;

	if (lock->lck_physical != LCK_SR) {
		/* assert (lock->lck_physical == LCK_none); */

		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
		lock->lck_key.lck_long = header->hdr_shadow_count;
		LCK_lock(tdbb, lock, LCK_SR, TRUE);
		CCH_RELEASE(tdbb, &window);
	}

/* get all new shadow files, marking that we looked at them first
   to prevent missing any new ones later on, although it does not
   matter for the purposes of the current page being written */

	MET_get_shadow_files(tdbb, FALSE);
}




void SDW_init(USHORT activate, USHORT delete_, SBM sbm_rec)
{
/**************************************
 *
 *	S D W _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize shadowing by opening all shadow files and
 *	getting a lock on the semaphore for disk shadowing. 
 *	When anyone tries to get an exclusive lock on this 
 *	semaphore, it is a signal to check for a new file 
 *	to use as a shadow.
 *
 **************************************/
	DBB dbb;
	LCK lock;
	HDR header;
	WIN window;
	USHORT key_length;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* set up the lock block for synchronizing addition of new shadows */

	key_length = sizeof(header->hdr_shadow_count);
	dbb->dbb_shadow_lock = lock = FB_NEW_RPT(*dbb->dbb_permanent, key_length) lck();
	lock->lck_type = LCK_shadow;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_length = key_length;
	lock->lck_dbb = dbb;
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_ast = SDW_start_shadowing;

	if (activate)
		activate_shadow();

/* get current shadow lock count from database header page */

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;

	header = (HDR) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
	lock->lck_key.lck_long = header->hdr_shadow_count;
	LCK_lock(tdbb, lock, LCK_SR, TRUE);
	CCH_RELEASE(tdbb, &window);

	MET_get_shadow_files(tdbb, delete_);
	CCH_recover_shadow(tdbb, sbm_rec);
}


BOOLEAN SDW_lck_update(SLONG sdw_update_flags)
{
/**************************************
 *
 *	 S D W _ l c k _ u p d a t e	
 *
 **************************************
 *
 * Functional description
 *  update the lck struct with the flag 
 *  The update type flag indicates the type fo corrective action
 *  to be taken by the ASTs of other procs attached to this DB.	
 *	
 *  A non zero sdw_update_flag is passed, it indicates error handling
 *  Two processes may encounter the SDW array at the same time
 *  and both will want to perform corrective action. Only one should
 *  be allowed. For that, 
 *		check if current data is zero, else return
 *  	write the pid into the lock data, read back to verify
 *      if pid is different, another process has updated behind you, so
 *		let him handle it and return
 *  	Update the data with sdw_update_flag passed to the function
 *
 **************************************/
	DBB dbb;
	LCK lock;

	dbb = GET_DBB;
	if (!(lock = dbb->dbb_shadow_lock))
		return FALSE;

	if (lock->lck_physical != LCK_SR) {
		/* assert (lock->lck_physical == LCK_none); */
		return FALSE;
	}

	if (!sdw_update_flags) {
		if (LCK_read_data(lock))
			return FALSE;
		else
			return TRUE;
	}

	if (LCK_read_data(lock))
		return FALSE;

	LCK_write_data(lock, lock->lck_id);
	if (LCK_read_data(lock) != lock->lck_id)
		return FALSE;

	LCK_write_data(lock, sdw_update_flags);
	return TRUE;
}


void SDW_notify(void)
{
/**************************************
 *
 *	S D W _ n o t i f y
 *
 **************************************
 *
 * Functional description
 *	Notify other processes that there has been
 *	a shadow added.
 *
 **************************************/
	DBB dbb;
	LCK lock;
	HDR header;
	WIN window;
	JRNDA record;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* get current shadow lock count from database header page --
   note that since other processes need the header page to issue locks
   on the shadow count, this is effectively an uninterruptible operation */

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);

/* get an exclusive lock on the current shadowing semaphore to
   notify other processes to find my shadow -- if we have a shared
   on it already, convert to exclusive */

	lock = dbb->dbb_shadow_lock;

	if (lock->lck_physical == LCK_SR) {
		if (lock->lck_key.lck_long != header->hdr_shadow_count)
			BUGCHECK(162);		/* msg 162 shadow lock not synchronized properly */
		LCK_convert(tdbb, lock, LCK_EX, TRUE);
	}
	else {
		lock->lck_key.lck_long = header->hdr_shadow_count;
		LCK_lock(tdbb, lock, LCK_EX, TRUE);
	}

	LCK_release(tdbb, lock);

/* now get a shared lock on the incremented shadow count to ensure that
   we will get notification of the next shadow add */

	lock->lck_key.lck_long = ++(header->hdr_shadow_count);
	LCK_lock(tdbb, lock, LCK_SR, TRUE);
	if (dbb->dbb_wal) {
		record.jrnda_type = JRNP_DB_HDR_SDW_COUNT;
		record.jrnda_data = header->hdr_shadow_count;
		CCH_journal_record(tdbb, &window,
						   reinterpret_cast < UCHAR * >(&record), JRNDA_SIZE,
						   0, 0);
	}

	CCH_RELEASE(tdbb, &window);
}


BOOLEAN SDW_rollover_to_shadow(FIL file, BOOLEAN inAst)
{
/**************************************
 *
 *	S D W _ r o l l o v e r _ t o _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SDW shadow;
	BOOLEAN start_conditional = FALSE;
	SLONG sdw_update_flags = SDW_rollover;
	TDBB tdbb;
	DBB dbb;
	LCK shadow_lock, update_lock;
	struct lck temp_lock;

	tdbb = GET_THREAD_DATA;
	dbb = GET_DBB;

	if (file != dbb->dbb_file)
		return TRUE;

	update_lock = &temp_lock;
	update_lock->lck_dbb = dbb;
	update_lock->lck_attachment = tdbb->tdbb_attachment;
	update_lock->lck_length = sizeof(SLONG);
	update_lock->lck_key.lck_long = -1;
	update_lock->lck_type = LCK_update_shadow;
	update_lock->lck_owner_handle =
		LCK_get_owner_handle(tdbb, update_lock->lck_type);
	update_lock->lck_parent = dbb->dbb_lock;
	update_lock->lck_owner = (BLK) tdbb->tdbb_attachment;

	LCK_lock(tdbb, update_lock, LCK_EX, LCK_NO_WAIT);
	if (update_lock->lck_physical != LCK_EX ||
		file != dbb->dbb_file || !SDW_lck_update(sdw_update_flags)) {
		LCK_release(tdbb, update_lock);
		LCK_lock(tdbb, update_lock, LCK_SR, LCK_NO_WAIT);
		while (update_lock->lck_physical != LCK_SR) {
			if (dbb->dbb_ast_flags & DBB_get_shadows)
				break;
			if ((file != dbb->dbb_file) || !dbb->dbb_shadow_lock)
				break;
			LCK_lock(tdbb, update_lock, LCK_SR, LCK_NO_WAIT);
		}

		if (update_lock->lck_physical == LCK_SR)
			LCK_release(tdbb, update_lock);
		return TRUE;
	}

/* check the various status flags to see if there
   is a valid shadow to roll over to */

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next) {
		if (!(shadow->sdw_flags & SDW_dumped))
			continue;
		if (!(shadow->sdw_flags & SDW_INVALID))
			break;
	}

	if (!shadow) {
		LCK_release(tdbb, update_lock);
		return FALSE;
	}

	if (file != dbb->dbb_file) {
		LCK_release(tdbb, update_lock);
		return TRUE;
	}

/* close the main database file if possible
   and release all file blocks */

	PIO_close(dbb->dbb_file);

	while ( (file = dbb->dbb_file) ) {
		dbb->dbb_file = file->fil_next;
		delete file;
	}

/* point the main database file at the file of the first shadow
   in the list and mark that shadow as rolled over to
   so that we won't write to it twice -- don't remove
   this shadow from the linked list of shadows because
   that would cause us to create a new shadow block for
   it the next time we do a MET_get_shadow_files () */

	dbb->dbb_file = shadow->sdw_file;
	shadow->sdw_flags |= SDW_rollover;

	shadow_lock = dbb->dbb_shadow_lock;

/* check conditional does a meta data update - since we were
   successfull updating LCK_data we will be the only one doing so */

	if (!inAst) {
		if ( (start_conditional = SDW_check_conditional()) ) {
			sdw_update_flags = (SDW_rollover | SDW_conditional);
			LCK_write_data(shadow_lock, sdw_update_flags);
		}
	}

	SDW_notify();
	LCK_write_data(shadow_lock, (SLONG) 0);
	LCK_release(tdbb, shadow_lock);
	delete shadow_lock;
	dbb->dbb_shadow_lock = 0;
	LCK_release(tdbb, update_lock);
	if (start_conditional && !inAst) {
		CCH_unwind(tdbb, FALSE);
		SDW_dump_pages();
		ERR_post(gds_deadlock, 0);
	}

	return TRUE;
}


void SDW_shutdown_shadow(SDW shadow)
{
/**************************************
 *
 *	S D W _ s h u t d o w n _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Stop shadowing to a given shadow number.
 *
 **************************************/
	DBB dbb;
	SDW *ptr;
	FIL file, free;

	dbb = GET_DBB;

/* find the shadow block and delete it from linked list */

	for (ptr = &dbb->dbb_shadow; *ptr; ptr = &(*ptr)->sdw_next)
		if (*ptr == shadow) {
			*ptr = shadow->sdw_next;
			break;
		}

/* close the shadow files and free up the associated memory */

	if (shadow) {
		PIO_close(shadow->sdw_file);
		for (free = shadow->sdw_file; (file = free->fil_next); free = file)
			delete free;
		delete free;
		delete shadow;
	}
}


void SDW_start(
			   TEXT * file_name,
			   USHORT shadow_number, USHORT file_flags, USHORT delete_)
{
/**************************************
 *
 *	S D W _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Commence shadowing on a previously created shadow file.
 *
 *	<delete_> is TRUE if we are not actually starting shadowing,
 *	but deleting inaccessible shadow files.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	SDW shadow;
	int length, expanded_length, string_length;
	SCHAR expanded_name[MAXPATHLEN];
	UCHAR *p;
	FIL dbb_file, shadow_file = 0;
	WIN window;
	HDR database_header, shadow_header;
	volatile USHORT header_fetched = 0;
	SLONG *spare_buffer = NULL, *spare_page;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* check that this shadow has not already been started,
   (unless it is marked as invalid, in which case it may
   be an old shadow of the same number) */

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
		if ((shadow->sdw_number == shadow_number) &&
			!(shadow->sdw_flags & SDW_INVALID)) return;

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
		if (shadow->sdw_number == shadow_number)
			break;

/* check to see if the shadow is the same as the current database --
   if so, a shadow file is being accessed as a database */

	length = strlen(file_name);
	expanded_length = PIO_expand(file_name, (USHORT) length, expanded_name);
	dbb_file = dbb->dbb_file;

	if (dbb_file && dbb_file->fil_string &&
		!strcmp(dbb_file->fil_string, expanded_name)) {
		if (shadow && (shadow->sdw_flags & SDW_rollover))
			return;
		else
			ERR_post(gds_shadow_accessed, 0);
	}

/* catch errors: delete the shadow file if missing,
   and deallocate the spare buffer */

	shadow = NULL;
	spare_buffer =
		(SLONG *) gds__alloc((SLONG) dbb->dbb_page_size + MIN_PAGE_SIZE);
	spare_page = reinterpret_cast < SLONG * >((SCHAR *)
											  
											  (((U_IPTR)
												spare_buffer + MIN_PAGE_SIZE -
												1) & ~((U_IPTR) MIN_PAGE_SIZE
													   - 1)));
	try {

	shadow_file =
		PIO_open(dbb, expanded_name, expanded_length, FALSE, 0, file_name,
				 length);

	if (dbb->dbb_flags & DBB_force_write) {
		PIO_force_write(shadow_file, TRUE);
	}

	if (!(file_flags & FILE_conditional))
	{
		/* make some sanity checks on the database and shadow header pages:
		   1. make sure that the proper database filename is accessing this shadow
		   2. make sure the database and shadow are in sync by checking the creation time/transaction id
		   3. make sure that the shadow has not already been activated */

		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		database_header =
			(HDR) CCH_FETCH(tdbb, &window, LCK_read, pag_header);
		header_fetched++;

		if (!PIO_read
			(shadow_file, window.win_bdb, (PAG) spare_page,
			 tdbb->tdbb_status_vector)) ERR_punt();

		shadow_header = (HDR) spare_page;

		/* 
		 *          NOTE ! NOTE! NOTE!
		 * Starting V4.0, header pages can have over flow pages.  For the shadow,
		 * we are making an assumption that the shadow header page will not
		 * overflow, as the only things written on a shadow header is the
		 * HDR_root_file_name, HDR_file, and HDR_last_page
		 */

		for (p = shadow_header->hdr_data;
			 *p != HDR_end && *p != HDR_root_file_name; p += 2 + p[1]);
		if (*p++ == HDR_end)
			BUGCHECK(163);		/* msg 163 root file name not listed for shadow */

		/* if the database file is not the same and the original file is
		   still around, then there is a possibility for shadow corruption */

		string_length = (USHORT) * p++;
		if (strncmp
			(dbb_file->fil_string, reinterpret_cast < const char *>(p),
			 string_length)
			&& check_for_file(reinterpret_cast < char *>(p),
							  string_length)) ERR_punt();

		if (
			(shadow_header->hdr_creation_date[0] !=
			 database_header->hdr_creation_date[0])
			|| (shadow_header->hdr_creation_date[1] !=
				database_header->hdr_creation_date[1])
			|| (!shadow_header->hdr_flags & hdr_active_shadow))
			ERR_punt();

		CCH_RELEASE(tdbb, &window);
		header_fetched--;
	}

/* allocate the shadow block and mark it as
   dumped (except for the cases when it isn't) */

	shadow = allocate_shadow(shadow_file, shadow_number, file_flags);
	if (!(file_flags & FILE_conditional)) {
		shadow->sdw_flags |= SDW_dumped;
	}

/* get the ancillary files and reset the error environment */

	PAG_init2(shadow_number);
	if (spare_buffer) {
		gds__free(spare_buffer);
	}

	}	// try
	catch (const std::exception&) {
		if (header_fetched) {
			CCH_RELEASE(tdbb, &window);
		}
		if (shadow_file) {
			PIO_close(shadow_file);
			delete shadow_file;
		}
		if (spare_buffer) {
			gds__free(spare_buffer);
		}
		if (file_flags & FILE_manual && !delete_) {
			ERR_post(gds_shadow_missing, gds_arg_number,
					 (SLONG) shadow_number, 0);
		}
		else
		{
			MET_delete_shadow(tdbb, shadow_number);
			gds__log
				("shadow %s deleted from database %s due to unavailability on attach",
				 expanded_name, dbb_file->fil_string);
		}
	}
}


int SDW_start_shadowing(void *ast_object)
{
/**************************************
 *
 *	S D W _ s t a r t _ s h a d o w i n g
 *
 **************************************
 *
 * Functional description
 *	A blocking AST has been issued to give up
 *	the lock on the shadowing semaphore. 
 *	Do so after flagging the need to check for
 *	new shadow files before doing the next physical write.
 *
 **************************************/
	DBB new_dbb = reinterpret_cast<DBB>(ast_object);
	LCK lock;
	struct tdbb thd_context, *tdbb;


	lock = new_dbb->dbb_shadow_lock;
	if (lock->lck_physical != LCK_SR)
		return 0;

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	SET_THREAD_DATA;

	tdbb->tdbb_database = new_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

	new_dbb->dbb_ast_flags |= DBB_get_shadows;
	if (LCK_read_data(lock) & SDW_rollover)
		update_dbb_to_sdw(new_dbb);

	LCK_release(tdbb, lock);

/* Restore the prior thread context */

	RESTORE_THREAD_DATA;

	ISC_ast_exit();
	return 0;
}


static void activate_shadow(void)
{
/**************************************
 *
 *	a c t i v a t e _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Change a shadow into a database.
 *
 **************************************/
	DBB dbb;
	WIN window;
	HDR header;
	JRNDA record;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	gds__log("activating shadow file %s", dbb->dbb_file->fil_string);

	MET_activate_shadow(tdbb);

/* clear the shadow bit on the header page */

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	header->hdr_flags &= ~hdr_active_shadow;
	if (dbb->dbb_wal) {
		record.jrnda_type = JRNP_DB_HDR_FLAGS;
		record.jrnda_data = header->hdr_flags;
		CCH_journal_record(tdbb, &window,
						   reinterpret_cast < UCHAR * >(&record), JRNDA_SIZE,
						   0, 0);
	}
	CCH_RELEASE(tdbb, &window);
}


static SDW allocate_shadow(
						   FIL shadow_file,
						   USHORT shadow_number, USHORT file_flags)
{
/**************************************
 *
 *	a l l o c a t e _ s h a d o w
 *
 **************************************
 *
 * Functional description
 *	Allocate a shadow block, setting all
 *	the fields properly.
 *
 **************************************/
	SDW shadow, *pShadow;
	DBB dbb;

	dbb = GET_DBB;

	shadow = FB_NEW(*dbb->dbb_permanent) sdw();
	shadow->sdw_file = shadow_file;
	shadow->sdw_number = shadow_number;
	if (file_flags & FILE_manual)
		shadow->sdw_flags |= SDW_manual;
	if (file_flags & FILE_conditional)
		shadow->sdw_flags |= SDW_conditional;

/* Link the new shadow into the list of shadows according to
 * shadow number position.  This is so we will activate
 * conditional shadows in the order specified by shadow number.
 * Note that the shadow number may not be unique in this list
 * - as could happen when shadow X is dropped, and then X is
 * recreated.
 */

	for (pShadow = &dbb->dbb_shadow; *pShadow;
		 pShadow =
		 &((*pShadow)->sdw_next)) if ((*pShadow)->sdw_number >=
									  shadow_number) break;

	shadow->sdw_next = *pShadow;
	*pShadow = shadow;

	return shadow;
}


static BOOLEAN check_for_file(SCHAR * name, USHORT length)
{
/**************************************
 *
 *	c h e c k _ f o r _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Check for the existence of a file.
 *	Return TRUE if it is there.
 *
 **************************************/

	TDBB tdbb = GET_THREAD_DATA;
	DBB  dbb  = tdbb->tdbb_database;

	try {
		FIL temp_file = PIO_open(dbb, name, length, FALSE, 0, name, length);
		PIO_close(temp_file);
	}	// try
	catch (const std::exception&) {
		return FALSE;
	}

	return TRUE;
}

#ifdef NOT_USED_OR_REPLACED
static void check_if_got_ast(FIL file)
{
/**************************************
 *
 *	c h e c k _ i f _ g o t _ a s t	
 *
 **************************************
 *
 * Functional description
 * 	have we got the signal indicating a 
 *	a shadow update
 *
 **************************************/
	DBB dbb;
	LCK lock;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	lock = dbb->dbb_shadow_lock;
	if (!lock || (file != dbb->dbb_file))
		return;
	while (TRUE) {
		if (dbb->dbb_ast_flags & DBB_get_shadows)
			break;
		LCK_convert(tdbb, lock, LCK_SR, TRUE);
	}
}
#endif

static void copy_header(void)
{
/**************************************
 *
 *	c o p y _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Fetch the header page from the database 
 *	and write it to the shadow file.  This is
 *	done so that if this shadow is extended,
 *	the header page will be there for writing 
 *	the name of the extend file.
 *
 **************************************/
	DBB dbb;
	WIN window;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* get the database header page and write it out --
   CCH will take care of modifying it */

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, &window);
	CCH_RELEASE(tdbb, &window);
}


static void update_dbb_to_sdw(DBB dbb)
{
/**************************************
 *
 *	 u p d a t e _ d b b _ t o _ s d w	
 *
 **************************************
 *
 * Functional description
 * 	Another process has indicated that dbb is corrupt
 *	so close dbb and initialize sdw to dbb
 *
 **************************************/

	SDW shadow;
	FIL file;

/* find shadow to rollover to */

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next) {
		if (!(shadow->sdw_flags & SDW_dumped))
			continue;
		if (!(shadow->sdw_flags & SDW_INVALID))
			break;
	}

	if (!shadow)
		return;					/* should be a BUGCHECK */

/* close the main database file if possible
   and release all file blocks */

	PIO_close(dbb->dbb_file);

	while ( (file = dbb->dbb_file) ) {
		dbb->dbb_file = file->fil_next;
		delete file;
	}

	dbb->dbb_file = shadow->sdw_file;
	shadow->sdw_flags |= SDW_rollover;
}


} // extern "C"
