/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbak.cpp
 *	DESCRIPTION:	New backup technology
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: nbak.cpp,v 1.45 2004-12-25 09:43:50 robocop Exp $
 *
 */

#include "firebird.h"
#include "common.h"
#include "jrd.h"
#include "ods.h"
#include "lck.h"
#include "cch.h"
#include "lck_proto.h"
#include "pag_proto.h"
#include "err_proto.h"
#include "cch_proto.h"
#include "isc_proto.h"
#include "thd.h"
#include "../jrd/thread_proto.h"
#include "os/pio_proto.h"
#include "gen/iberror.h"
#include "gds_proto.h"
#include "os/guid.h"
#include "sch_proto.h"
#include "os/isc_i_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef NBAK_DEBUG
IMPLEMENT_TRACE_ROUTINE(nbak_trace, "NBAK")
#endif

/******************************** LOCK FUNCTIONS ******************************/

using namespace Jrd;

#ifndef SUPERSERVER
void BackupManager::increment_diff_use_count() throw()
{
// This functions are used in CS builds to prevent closing of 
// difference file while there are still unflushed pages from it
// and status already changed. Backup status does not change 
// externally in SS builds so we have no point to track this usage
// except for debugging purposes
	NBAK_TRACE(("increment_diff_use_count %d", diff_use_count));
	diff_use_count++;
}

void BackupManager::decrement_diff_use_count() throw()
{
	NBAK_TRACE(("decrement_diff_use_count %d", diff_use_count));
	diff_use_count--;
}
#endif

bool BackupManager::get_sw_database_lock(bool enable_signals) throw()
{
#ifdef SUPERSERVER
#ifdef WIN_NT
	NBAK_TRACE(("get_sw_database_lock %d", database_lock->getState()));
#else
	NBAK_TRACE(("get_sw_database_lock"));
#endif
	database_lock->beginRead();
	return true;
#else
	NBAK_TRACE(("get_sw_database_lock %d", database_use_count));
	thread_db* tdbb = JRD_get_thread_data();
	database_use_count++;
	if (enable_signals)
		LCK_ast_enable();
	if (database_lock->lck_physical >= LCK_SW)
		return true;
	if (!LCK_lock(tdbb, database_lock, LCK_SW, LCK_WAIT)) {
		gds__log("Cannot take SW lock on database");
		return false;
	}
	return true;
#endif
}

void BackupManager::release_sw_database_lock() throw()
{
#ifdef SUPERSERVER
#ifdef WIN_NT
	NBAK_TRACE(("release_sw_database_lock %d", database_lock->getState()));
#else
	NBAK_TRACE(("release_sw_database_lock"));
#endif
	database_lock->endRead();
#else
	NBAK_TRACE(("release_sw_database_lock %d", database_use_count));
	fb_assert(database_use_count > 0);
	thread_db* tdbb = JRD_get_thread_data();
	database_use_count--;
	if (ast_flags & NBAK_database_blocking) {
		LCK_release(tdbb, database_lock);
		ast_flags &= ~NBAK_database_blocking;
	}
#endif
}

void BackupManager::lock_state_write(bool thread_exit)
{
#ifdef SUPERSERVER
	if (thread_exit)
		THREAD_EXIT();
	state_lock->beginWrite();
	if (thread_exit)
		THREAD_ENTER();
#else
	fb_assert(!(flags & NBAK_state_in_use));
	thread_db* tdbb = JRD_get_thread_data();
	flags |= NBAK_state_in_use;
	bool locked = false;
	// Release shared lock to prevent possible deadlocks
	if (state_lock->lck_physical != LCK_none)
		if (LCK_convert(tdbb, state_lock, LCK_EX, LCK_NO_WAIT))
			locked = true;
		else {
			LCK_release(tdbb, state_lock);
			ast_flags &= ~NBAK_state_blocking;
			backup_state = nbak_state_unknown;
		}
	if (!locked && !LCK_lock(tdbb, state_lock, LCK_EX, LCK_WAIT)) {
		flags &= ~NBAK_state_in_use; // Lock should be released at this point
		gds__log("Cannot lock database backup state for writing");
		// This is OK because state changing code expect it
		ERR_post(isc_lock_conflict, 0); 
	}
	NBAK_TRACE(("backup state locked for writing"));
#endif
	if (!actualize_state()) {
		unlock_state_write();
		ERR_punt();
	}
}

bool BackupManager::try_lock_state_write()
{
#ifdef SUPERSERVER
	if (!state_lock->tryBeginWrite())
		return false;
#else
	fb_assert(!(flags & NBAK_state_in_use));
	thread_db* tdbb = JRD_get_thread_data();
	flags |= NBAK_state_in_use;
	bool result;
	if (state_lock->lck_physical == LCK_none)
		result = LCK_lock(tdbb, state_lock, LCK_EX, LCK_NO_WAIT);
	else
		result = LCK_convert(tdbb, state_lock, LCK_EX, LCK_NO_WAIT);
	if (!result) {
		flags &= ~NBAK_state_in_use;
		// This code is ok only because ASTs are delivered only once per request
		if (ast_flags & NBAK_state_blocking) {
			LCK_release(tdbb, state_lock);
			ast_flags &= ~NBAK_state_blocking;
			backup_state = nbak_state_unknown;
		}
		return false;
	} 
	NBAK_TRACE(("backup state locked for writing"));
#endif
	if (!actualize_state()) {
		unlock_state_write();
		ERR_punt();
	}
	return true;
}

void BackupManager::unlock_state_write() throw()
{
#ifdef SUPERSERVER
	state_lock->endWrite();
#else
	fb_assert(flags & NBAK_state_in_use);
	thread_db* tdbb = JRD_get_thread_data();
	// ASTs are going to be reposted after CONVERT
	ast_flags &= ~NBAK_state_blocking;
	LCK_convert(tdbb, state_lock, LCK_SR, LCK_WAIT);
	flags &= ~NBAK_state_in_use;	
	if (ast_flags & NBAK_state_blocking) {
		LCK_release(tdbb, state_lock);
		ast_flags &= ~NBAK_state_blocking;
		backup_state = nbak_state_unknown;
	}
#endif
}

bool BackupManager::lock_alloc_write(bool thread_exit) throw()
{
#ifdef SUPERSERVER
	if (thread_exit)
		THREAD_EXIT();
	alloc_lock->beginWrite();
	if (thread_exit)
		THREAD_ENTER();
#else
	fb_assert(!(flags & NBAK_alloc_in_use));
	thread_db* tdbb = JRD_get_thread_data();
	flags |= NBAK_alloc_in_use;
	// Release shared lock to prevent possible deadlocks
	bool locked = false;
	if (alloc_lock->lck_physical != LCK_none)
		if (LCK_convert(tdbb, alloc_lock, LCK_EX, LCK_NO_WAIT)) {
			locked = true;
		}
		else {
			LCK_release(tdbb, alloc_lock);
			ast_flags &= ~NBAK_alloc_blocking;
			ast_flags |= NBAK_alloc_dirty;
		}
	if (!locked && !LCK_lock(tdbb, alloc_lock, LCK_EX, LCK_WAIT)) {
		flags &= ~NBAK_alloc_in_use;
		gds__log("Cannot lock backup allocation table for writing");
		return false;
	}
#endif
	if (!actualize_alloc()) {
		unlock_alloc_write();
		return false;
	}
	return true;
}

void BackupManager::unlock_alloc_write() throw()
{
#ifdef SUPERSERVER
	alloc_lock->endWrite();
#else
	fb_assert(flags & NBAK_alloc_in_use);
	thread_db* tdbb = JRD_get_thread_data();
	// ASTs are going to be reposted after CONVERT
	ast_flags &= ~NBAK_alloc_blocking;
	LCK_convert(tdbb, alloc_lock, LCK_SR, LCK_WAIT);
	flags &= ~NBAK_alloc_in_use;
	if (ast_flags & NBAK_alloc_blocking) {
		LCK_release(tdbb, alloc_lock);
		ast_flags &= ~NBAK_alloc_blocking;
		ast_flags |= NBAK_alloc_dirty;
	} 
#endif
}

bool BackupManager::lock_state(bool thread_exit) throw()
{
	thread_db* tdbb = JRD_get_thread_data();
	// If we own exlock here no need to do anything else
	if (tdbb->tdbb_flags & TDBB_set_backup_state)
		return true;
#ifdef SUPERSERVER
	if (thread_exit)
		THREAD_EXIT();
	state_lock->beginRead();
	if (thread_exit)
		THREAD_ENTER();
#else
	fb_assert(!(flags & NBAK_state_in_use));
	flags |= NBAK_state_in_use;
	if (state_lock->lck_physical < LCK_SR) {
		if (!LCK_lock(tdbb, state_lock, LCK_SR, LCK_WAIT)) {
			flags &= ~NBAK_state_in_use;
			gds__log("Cannot lock database backup state for reading");
			return false;
		}
	}
#endif
	if (!actualize_state()) {
		unlock_state();
		return false;
	}
	return true;
}

void BackupManager::unlock_state() throw()
{
	thread_db* tdbb = JRD_get_thread_data();
	// If we own exlock here no need to do anything else
	if (tdbb->tdbb_flags & TDBB_set_backup_state)
		return;
#ifdef SUPERSERVER
	state_lock->endRead();
#else
	fb_assert(flags & NBAK_state_in_use);
	flags &= ~NBAK_state_in_use;
	if (ast_flags & NBAK_state_blocking) {
		LCK_release(tdbb, state_lock);
		ast_flags &= ~NBAK_state_blocking;
		backup_state = nbak_state_unknown;
	} 
#endif
}

bool BackupManager::lock_alloc(bool thread_exit) throw() {
#ifdef SUPERSERVER
	if (thread_exit)
		THREAD_EXIT();
	alloc_lock->beginRead();
	if (thread_exit)
		THREAD_ENTER();
#else
	fb_assert(!(flags & NBAK_alloc_in_use));
	thread_db* tdbb = JRD_get_thread_data();
	flags |= NBAK_alloc_in_use;
	if (alloc_lock->lck_physical < LCK_SR) {
		if (!LCK_lock(tdbb, alloc_lock, LCK_SR, LCK_WAIT)) {
			flags &= ~NBAK_alloc_in_use;
			gds__log("Cannot lock backup allocation table for reading");
			return false;
		}
	}
#endif
	if (!actualize_alloc()) {
		unlock_alloc();
		return false;
	}
	return true;
}

void BackupManager::unlock_alloc() throw() {
#ifdef SUPERSERVER
	alloc_lock->endRead();
#else
	fb_assert(flags & NBAK_alloc_in_use);
	thread_db* tdbb = JRD_get_thread_data();
	flags &= ~NBAK_alloc_in_use;
	if (ast_flags & NBAK_alloc_blocking) {
		LCK_release(tdbb, alloc_lock);
		ast_flags &= ~NBAK_alloc_blocking;
		ast_flags |= NBAK_alloc_dirty;
	} 
#endif
}

/********************************* AST FUNCTIONS ******************************/

#ifndef SUPERSERVER

int BackupManager::backup_state_ast(void *ast_object) throw()
{
/**************************************
 *
 *	b a c k u p _ s t a t e _ a s t
 *
 **************************************
 *
 * Functional description
 *	A blocking AST has been issued to give up
 *	the lock on the backup state semaphore. 
 *	Do so if we are not using backup state.
 *  Otherwise set blocking flag and blocking routine 
 *  will release the lock as soon it finishes
 *
 **************************************/
	Database* new_dbb = static_cast<Database*>(ast_object);
	Lock* lock = new_dbb->backup_manager->state_lock;

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	tdbb->tdbb_database = new_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

	NBAK_TRACE_AST("NBAK, backup_state_ast");
	
	if (new_dbb->backup_manager->flags & NBAK_state_in_use)
		new_dbb->backup_manager->ast_flags |= NBAK_state_blocking;
	else {
		// We know state only as long we lock it
		new_dbb->backup_manager->backup_state = nbak_state_unknown; 
		LCK_release(tdbb, lock);
	}

/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();
	return 0;
}

int BackupManager::alloc_table_ast(void *ast_object) throw()
{
/**************************************
 *
 *	a l l o c _ t a b l e _ a s t
 *
 **************************************
 *
 * Functional description
 *	A blocking AST has been issued to give up
 *	the lock on the allocate table semaphore. 
 *	Do so if we are not using allocation table.
 *  Otherwise set blocking flag and blocking routine 
 *  will release the lock as soon it finishes
 *
 **************************************/
	Database* new_dbb = static_cast<Database*>(ast_object);
	Lock* lock = new_dbb->backup_manager->alloc_lock;

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	tdbb->tdbb_database = new_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	
	NBAK_TRACE_AST("NBAK, alloc_table_ast");

	if (new_dbb->backup_manager->flags & NBAK_alloc_in_use)
		new_dbb->backup_manager->ast_flags |= NBAK_alloc_blocking;
	else {
		new_dbb->backup_manager->ast_flags |= NBAK_alloc_dirty;
		LCK_release(tdbb, lock);
	}

/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();
	return 0;
}

int BackupManager::backup_database_ast(void *ast_object) throw()
{
/**************************************
 *
 *	b a c k u p _ d a t a b a s e _ a s t
 *
 **************************************
 *
 * Functional description
 *	A blocking AST has been issued to give up
 *	the lock on the backup state semaphore. 
 *	Do so if we are not using backup state.
 *  Otherwise set blocking flag and blocking routine 
 *  will release the lock as soon it finishes
 *
 **************************************/
	Database* new_dbb = static_cast<Database*>(ast_object);
	ISC_STATUS_ARRAY ast_status;

	Lock* lock = new_dbb->backup_manager->database_lock;

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	thread_db thd_context, *tdbb;
	JRD_set_thread_data(tdbb, thd_context);

	tdbb->tdbb_database = new_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	tdbb->tdbb_status_vector = ast_status;

	NBAK_TRACE_AST("NBAK, backup_database_ast");
	
	if (new_dbb->backup_manager->database_use_count) {
		new_dbb->backup_manager->ast_flags |= NBAK_database_blocking;
		ast_status[1] = 0;
		CCH_flush_database(tdbb); // This may release database lock
		if (ast_status[1])
			gds__log_status(new_dbb->dbb_file->fil_string, ast_status);				
	} else {
		LCK_release(tdbb, lock);
	}

/* Restore the prior thread context */

	JRD_restore_thread_data();

	ISC_ast_exit();
	return 0;
}

#endif

/********************************** CORE LOGIC ********************************/

void BackupManager::generate_filename() throw()
{
	strncpy(diff_name, (char*)database->dbb_filename.c_str(), sizeof(diff_name));
	strncat(diff_name, ".delta", sizeof(diff_name) - strlen(diff_name) - 1);
}

// Initialize and open difference file for writing
void BackupManager::begin_backup()
{
	NBAK_TRACE(("begin_backup"));
	thread_db* tdbb = JRD_get_thread_data();

	// Lock header page first to prevent possible deadlock
	WIN window(HEADER_PAGE);
	Ods::header_page* header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	bool state_locked = false, header_locked = true;
	try {
		lock_state_write(true);
		state_locked = true;
		NBAK_TRACE(("state locked"));

		// Check state
		if (backup_state != nbak_state_normal) {
			NBAK_TRACE(("end backup - invalid state %d", backup_state));
			unlock_state_write();
			CCH_RELEASE(tdbb, &window);
			return;
		}
		// Create file
		NBAK_TRACE(("Creating difference file %s", diff_name));
		diff_file = PIO_create(database, diff_name, strlen(diff_name), true);
		// Zero out first page (empty allocation table)
		BufferDesc temp_bdb;
		temp_bdb.bdb_page = 0;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(alloc_buffer);
		memset(alloc_buffer, 0, database->dbb_page_size);
		if (!PIO_write(diff_file, &temp_bdb, temp_bdb.bdb_buffer, tdbb->tdbb_status_vector))
			ERR_punt();
		NBAK_TRACE(("Set backup state in header"));
		FB_GUID guid;
		GenerateGuid(&guid);
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		// Set state in database header page. All changes are written to main database file yet.
		CCH_MARK_MUST_WRITE(tdbb, &window);
		int newState = nbak_state_stalled;
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | newState;
		// This number may be smaller than actual because some pages may be not flushed to
		// disk yet. This is not a problem as it can cause only a slight performance degradation
		backup_pages = header->hdr_backup_pages = PIO_act_alloc(database);
		const ULONG adjusted_scn = ++header->hdr_header.pag_scn; // Generate new SCN
		PAG_replace_entry_first(header, Ods::HDR_backup_guid, sizeof(guid), 
			reinterpret_cast<const UCHAR*>(&guid));

		header_locked = false;
		CCH_RELEASE(tdbb, &window);
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
	
		backup_state = newState;
		current_scn = adjusted_scn;
		// This is essential for SuperServer to prevent multi-threading issues
		// We already modified state in header, error here is not very important
#ifdef SUPERSERVER
		// We cannot do this in classic because we do not have alloc_lock now
		// and we'll not get alloc_table_ast at the appropriate point
		actualize_alloc(); 
#endif
		unlock_state_write();
	} catch (const std::exception&) {
		backup_state = nbak_state_unknown;
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		if (state_locked)
			unlock_state_write();
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		throw;
	}
	// Flush local cache
	CCH_flush_database(tdbb);
#ifdef SUPERSERVER
	// Check if we can take lock on main database file
	if (!database_lock->tryBeginWrite()) {
		ERR_bugcheck_msg("Internal error. Cannot take EX lock on database");
	}
	database_lock->endWrite();
#else
	if (database_use_count) {
		ERR_bugcheck_msg("Internal error. Cannot take EX lock on database when flushing local cache");
	}
	// Send AST's to flush other caches
	database_use_count++;
	if (!LCK_lock(tdbb, database_lock, LCK_EX, LCK_WAIT)) {
		ERR_bugcheck_msg(
			"Internal error. Cannot take EX lock on database when flushing other processes cache");
	}
	database_use_count--;
	LCK_release(tdbb, database_lock);
#endif
}

// Merge difference file to main files (if needed) and unlink() difference 
// file then. If merge is already in progress method silently returns and 
// does nothing (so it can be used for recovery on database startup). 
void BackupManager::end_backup(bool recover) {
	NBAK_TRACE(("end_backup"));
	thread_db* tdbb = JRD_get_thread_data();
	ULONG adjusted_scn; // We use this value to prevent race conditions.
						// They are possible because we release state lock
						// for some instants and anything is possible at
						// that times.

	// Lock header page first to prevent possible deadlock
	WIN window(HEADER_PAGE);
	Ods::header_page* header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	bool state_locked = false, header_locked = true;

	try {
		if (recover) {
			if (!try_lock_state_write()) {
				CCH_RELEASE(tdbb, &window);
				return;
			}
		}
		else
			lock_state_write(true);
		state_locked = true;
		NBAK_TRACE(("state locked"));
		// Check state
		NBAK_TRACE(("Backup state at start of end_backup is %d", backup_state));
		if (backup_state == nbak_state_normal || (recover && backup_state != nbak_state_merge))
		{
			NBAK_TRACE(("invalid state %d", backup_state));
			unlock_state_write();
			CCH_RELEASE(tdbb, &window);
			return;
		}
		NBAK_TRACE(("difference file %s", diff_name));
		// Set state in database header
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		NBAK_TRACE(("Current backup state is %d", backup_state));
		backup_state = nbak_state_merge;
		adjusted_scn = ++current_scn;
		NBAK_TRACE(("New state is getting to become %d", backup_state));
		CCH_MARK_MUST_WRITE(tdbb, &window);
		NBAK_TRACE(("New state is getting to become after fetches %d", backup_state));
		// Generate new SCN
		header->hdr_header.pag_scn = current_scn;
		NBAK_TRACE(("new SCN=%d is getting written to header", adjusted_scn));
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | backup_state;
		header_locked = false;
		CCH_RELEASE(tdbb, &window);
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		NBAK_TRACE(("Set state %d in header page", backup_state));
	} catch (const std::exception&) {
		backup_state = nbak_state_unknown;
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		if (state_locked)
			unlock_state_write();
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		throw;
	}
	
	
	// Here comes the dirty work. We need to reapply all changes from difference file to database
	
	// Release write state lock and get read lock. 
	// Merge process should not inhibit normal operations.
	tdbb->tdbb_flags &= ~TDBB_set_backup_state;
	unlock_state_write();
	if (!lock_state(true))
		ERR_punt();
	try {
		NBAK_TRACE(("Merge. State=%d, current_scn=%d, adjusted_scn=%d",
			backup_state, current_scn, adjusted_scn));	
		if (backup_state != nbak_state_merge || current_scn != adjusted_scn) {
			/* Handle the case when somebody finalized merge for us */
			unlock_state();
			return;
		}
		NBAK_TRACE(("Status OK."));
		if (!actualize_alloc())
			ERR_punt();
		NBAK_TRACE(("Allocation table %p is current.", alloc_table));
		AllocItemTree::Accessor all(alloc_table);
		
		tdbb->tdbb_flags |= TDBB_set_backup_state | TDBB_backup_merge;
		
		if (all.getFirst()) {
			do {
				WIN window2(all.current().db_page);
				Ods::pag* page = CCH_FETCH(tdbb, &window2, LCK_write, pag_undefined);
				if (page->pag_scn != current_scn)
					CCH_MARK(tdbb, &window2);
				CCH_RELEASE(tdbb, &window2);
			} while (all.getNext());
		}
		CCH_flush(tdbb, FLUSH_ALL, 0); // Really write changes to main database file
		
		tdbb->tdbb_flags &= ~(TDBB_set_backup_state | TDBB_backup_merge);
		unlock_state();
		
	} catch(const std::exception&) {
		tdbb->tdbb_flags &= ~(TDBB_set_backup_state | TDBB_backup_merge);
		unlock_state();
		throw;
	}
	
	// We finished. We need to reflect it in our database header page
	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	state_locked = false;
	header_locked = true;
	try {
		lock_state_write(true);
		state_locked = true;
		// Check state
		if (backup_state != nbak_state_merge || current_scn != adjusted_scn) {
			/* Handle the case when somebody finalized merge for us */
			unlock_state_write();
			CCH_RELEASE(tdbb, &window);
			return;
		}
		// Set state in database header
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		backup_state = nbak_state_normal;
		CCH_MARK_MUST_WRITE(tdbb, &window);
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | backup_state;
		NBAK_TRACE(("Set state %d in header page", backup_state));
		// Generate new SCN
		header->hdr_header.pag_scn = ++current_scn;
		NBAK_TRACE(("new SCN=%d is getting written to header"));
		header_locked = false;
		CCH_RELEASE(tdbb, &window);
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		
		// Page allocation table cache is no longer valid
		delete alloc_table;
		alloc_table = NULL;
		last_allocated_page = 0;
		
		if (diff_file) {
#ifndef SUPERSERVER
			diff_generation++;
#endif
			PIO_close(diff_file);
			diff_file = NULL;
		}
		unlink(diff_name);
		
		unlock_state_write();
		NBAK_TRACE(("backup ended"));
	} catch (const std::exception&) {
		backup_state = nbak_state_unknown;
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		if (state_locked)
			unlock_state_write();
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		throw;
	}
	return;
}
	
bool BackupManager::actualize_alloc() throw()
{
	if (alloc_table
#ifndef SUPERSERVER
		&& !(ast_flags & NBAK_alloc_dirty)
#endif
	) return true;
	ISC_STATUS *status_vector = JRD_get_thread_data()->tdbb_status_vector;
	try {
		NBAK_TRACE(("actualize_alloc last_allocated_page=%d alloc_table=%p", 
			last_allocated_page, alloc_table));
		// For SuperServer this routine is really executed only at database startup when 
		// it has exlock or when exclusive access to database is enabled
		if (!alloc_table)
			alloc_table = FB_NEW(*database->dbb_permanent) AllocItemTree(database->dbb_permanent);
		while (true) {
			BufferDesc temp_bdb;
			// Difference file pointer pages have one ULONG as number of pages allocated on the page and
			// then go physical numbers of pages from main database file. Offsets of numbers correspond
			// to difference file pages.
		
			// Get offset of pointer page. We can do so because page sizes are powers of 2
			temp_bdb.bdb_page = last_allocated_page & ~(database->dbb_page_size / sizeof(ULONG) - 1);
			temp_bdb.bdb_dbb = database;
			temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(alloc_buffer);
		
			if (!PIO_read(diff_file, &temp_bdb, temp_bdb.bdb_buffer, status_vector))
				return false;
			for (ULONG i = last_allocated_page - temp_bdb.bdb_page; i < alloc_buffer[0]; i++)
			{
				NBAK_TRACE(("alloc item page=%d, diff=%d", alloc_buffer[i + 1], temp_bdb.bdb_page + i + 1));
				if (!alloc_table->add(AllocItem(alloc_buffer[i + 1], temp_bdb.bdb_page + i + 1)))
				{
					database->dbb_flags |= DBB_bugcheck;
					status_vector[0] = isc_arg_gds;
					status_vector[1] = isc_bug_check;
					status_vector[2] = isc_arg_string;
					status_vector[3] =
						(ISC_STATUS)(U_IPTR) ERR_cstring("Duplicated item in allocation table detected");
					status_vector[4] = isc_arg_end;
					return false;
				}
			}
			last_allocated_page = temp_bdb.bdb_page + alloc_buffer[0];
			if (alloc_buffer[0] == database->dbb_page_size / sizeof(ULONG) - 1)
				// if page is full adjust position for next pointer page
				last_allocated_page++;
			else
				// We finished reading allocation table
				break;		
		}
	} catch (const std::exception& ex) {
		// Handle out of memory error, etc
		delete alloc_table;
		Firebird::stuff_exception(status_vector, ex);
		alloc_table = NULL;
		last_allocated_page = 0;
		return false;				
	}
#ifndef SUPERSERVER
	ast_flags &= ~NBAK_alloc_dirty;
#endif
	return true;
}

// Return page index in difference file that can be used in 
// write_difference call later. 
ULONG BackupManager::get_page_index(ULONG db_page) const throw()
{
	AllocItemTree::Accessor a(alloc_table);
	if (a.locate(db_page))
		return a.current().diff_page;
	else
		return 0;
}

// Mark next difference page as used by some database page
ULONG BackupManager::allocate_difference_page(ULONG db_page) throw() {
	fb_assert(last_allocated_page % (database->dbb_page_size / sizeof(ULONG)) == alloc_buffer[0]);

	ISC_STATUS* status_vector = JRD_get_thread_data()->tdbb_status_vector;
	// Grow file first. This is done in such order to keep difference
	// file consistent in case of write error. We should always be able 
	// to read next alloc page when previous one is full.
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = last_allocated_page + 1;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(empty_buffer);
	if (!PIO_write(diff_file, &temp_bdb, (Ods::pag*)empty_buffer, status_vector))
		return 0;
	
	const bool alloc_page_full = alloc_buffer[0] == database->dbb_page_size / sizeof(ULONG) - 2;
	if (alloc_page_full) {
		// Pointer page is full. Its time to create new one.
		temp_bdb.bdb_page = last_allocated_page + 2;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(empty_buffer);
		if (!PIO_write(diff_file, &temp_bdb, (Ods::pag*)empty_buffer, status_vector))
			return 0;
	}

	// Write new item to the allocation table
	temp_bdb.bdb_page = last_allocated_page & ~(database->dbb_page_size / sizeof(ULONG) - 1);
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(alloc_buffer);
	alloc_buffer[++alloc_buffer[0]] = db_page;
	if (!PIO_write(diff_file, &temp_bdb, temp_bdb.bdb_buffer, status_vector))
		return 0;
	last_allocated_page++;
	// Register new page in the alloc table
	try {
		alloc_table->add(AllocItem(db_page, last_allocated_page));
	} catch(const std::exception& ex) {
		// Handle out of memory error
		delete alloc_table;
		alloc_table = NULL;
		last_allocated_page = 0;
		Firebird::stuff_exception(status_vector, ex);
		return 0;
	}
	// Adjust buffer and counters if we allocated new alloc page earlier
	if (alloc_page_full) {
		last_allocated_page++;
		memset(alloc_buffer, 0, database->dbb_page_size);
		return last_allocated_page - 1;
	}
	
	return last_allocated_page;
}

bool BackupManager::write_difference(ISC_STATUS* status, ULONG diff_page, Ods::pag* page) throw()
{
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	if (!PIO_write(diff_file, &temp_bdb, page, status))
		return false;
	return true;
}
	
bool BackupManager::read_difference(ULONG diff_page, Ods::pag* page) throw()
{
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	if (!PIO_read(diff_file, &temp_bdb, page, JRD_get_thread_data()->tdbb_status_vector))		
		return false;
	return true;
}
	
BackupManager::BackupManager(Database* _database, int ini_state) :
	database(_database), diff_file(NULL), alloc_table(NULL), 
	backup_state(ini_state), last_allocated_page(0),
	current_scn(0), backup_pages(0), diff_pending_close(false)
{
	thread_db* tdbb = JRD_get_thread_data();
	diff_name[0] = 0;
	
	// Allocate various database page buffers needed for operation
	temp_buffers_space = FB_NEW(*database->dbb_permanent) BYTE[database->dbb_page_size * 3 + MIN_PAGE_SIZE];
	// Align it at sector boundary for faster IO
	BYTE *temp_buffers = reinterpret_cast<BYTE*>(
		FB_ALIGN(reinterpret_cast<U_IPTR>(temp_buffers_space), MIN_PAGE_SIZE));
	memset(temp_buffers, 0, database->dbb_page_size * 3);
		
	empty_buffer = reinterpret_cast<ULONG*>(temp_buffers);
	spare_buffer = reinterpret_cast<ULONG*>(temp_buffers + database->dbb_page_size);
	alloc_buffer = reinterpret_cast<ULONG*>(temp_buffers + database->dbb_page_size + 2);

	
#ifdef SUPERSERVER
	alloc_lock = FB_NEW(*_database->dbb_permanent) Firebird::RWLock();
	state_lock = FB_NEW(*_database->dbb_permanent) Firebird::RWLock();	
	database_lock = FB_NEW(*_database->dbb_permanent) Firebird::RWLock();	
	adjust_state_lock = FB_NEW(*_database->dbb_permanent) Firebird::Mutex();	
#else
	flags = 0;
	ast_flags = 0;
	database_use_count = 0;
	diff_use_count = 0;
	diff_generation = 0;
	
	state_lock = FB_NEW_RPT(*database->dbb_permanent, 0) Lock();
	state_lock->lck_type = LCK_backup_state;
	state_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, state_lock->lck_type);
	state_lock->lck_parent = database->dbb_lock;
	state_lock->lck_length = 0;
	state_lock->lck_dbb = database;
	state_lock->lck_object = reinterpret_cast<blk*>(database);
	state_lock->lck_ast = backup_state_ast;

	alloc_lock = FB_NEW_RPT(*database->dbb_permanent, 0) Lock();
	alloc_lock->lck_type = LCK_backup_alloc;
	alloc_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, alloc_lock->lck_type);
	alloc_lock->lck_parent = database->dbb_lock;
	alloc_lock->lck_length = 0;
	alloc_lock->lck_dbb = database;
	alloc_lock->lck_object = reinterpret_cast<blk*>(database);
	alloc_lock->lck_ast = alloc_table_ast;

	database_lock = FB_NEW_RPT(*database->dbb_permanent, 0) Lock();
	database_lock->lck_type = LCK_backup_database;
	database_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, database_lock->lck_type);
	database_lock->lck_parent = database->dbb_lock;
	database_lock->lck_length = 0;
	database_lock->lck_dbb = database;
	database_lock->lck_object = reinterpret_cast<blk*>(database);
	database_lock->lck_ast = backup_database_ast;
#endif
}

void BackupManager::shutdown_locks() throw()
{
#ifndef SUPERSERVER
	thread_db* tdbb = JRD_get_thread_data();
	if (state_lock)
		LCK_release(tdbb, state_lock);
	if (alloc_lock)
		LCK_release(tdbb, alloc_lock);
	if (database_lock)
		LCK_release(tdbb, database_lock);
#endif
}


BackupManager::~BackupManager()
{
	if (diff_file)
		PIO_close(diff_file);
	shutdown_locks();
	delete alloc_table;
	delete[] temp_buffers_space;
#ifdef SUPERSERVER
	delete alloc_lock;
	delete state_lock;
	delete adjust_state_lock;
	delete database_lock;
#endif
}

void BackupManager::set_difference(const char* filename) {
	thread_db* tdbb = JRD_get_thread_data();
	
	if (filename) {
		WIN window(HEADER_PAGE);
		Ods::header_page* header =
			(Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		PAG_replace_entry_first(header, Ods::HDR_difference_file, 
			strlen(filename), reinterpret_cast<const UCHAR*>(filename));
		CCH_RELEASE(tdbb, &window);
		strncpy(diff_name, filename, sizeof(diff_name));
	}
	else {
		PAG_delete_clump_entry(HEADER_PAGE, Ods::HDR_difference_file);
		generate_filename();
	}
}

bool BackupManager::actualize_state() throw() {
	if (backup_state != nbak_state_unknown)
		return true;

	// State is unknown. We need to read it from the disk.
	// We cannot use CCH for this because of likely recursion.
	NBAK_TRACE(("actualize_state"));	
	
	ISC_STATUS *status = JRD_get_thread_data()->tdbb_status_vector;
			
	// Read original page from database file or shadows.
	SSHORT retryCount = 0;
	Ods::header_page* header = reinterpret_cast<Ods::header_page*>(spare_buffer);
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = HEADER_PAGE;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(header);
	jrd_file* file = database->dbb_file;
	while (!PIO_read(file, &temp_bdb, temp_bdb.bdb_buffer, status)) {
		if (!CCH_rollover_to_shadow(database, file, false)) {
			NBAK_TRACE(("Shadow change error"));
			return false;
		}
		if (file != database->dbb_file)
			file = database->dbb_file;
		else {
			if (retryCount++ == 3) {
				NBAK_TRACE(("IO error"));
				return false;
			}
		}
	}

	const int new_backup_state =
		(database->dbb_ods_version >= ODS_VERSION11) ?
		header->hdr_flags & Ods::hdr_backup_mask :
		nbak_state_normal;
	NBAK_TRACE(("backup state read from header is %d", new_backup_state));
	// Check is we missed lock/unlock cycle and need to invalidate
	// our allocation table and file handle 
	const bool missed_cycle = (header->hdr_header.pag_scn - current_scn) > 1;
	current_scn = header->hdr_header.pag_scn;
	backup_pages = header->hdr_backup_pages;

	// Read difference file name from header clumplets
	bool fname_found = false;
	const UCHAR* p = header->hdr_data;
	while (true) {
		switch (*p) {
		case Ods::HDR_backup_guid:
			p += p[1] + 2;
			continue;
		case Ods::HDR_difference_file:
			fname_found = true;
			memcpy(diff_name, p + 2, p[1]);
			diff_name[p[1]] = 0;				
		}
		break;
	}
	if (!fname_found)
		generate_filename();
		
	if (new_backup_state == nbak_state_normal || missed_cycle) {
		if (diff_file)
			diff_pending_close = true;
		// Page allocation table cache is no longer valid.
		if (alloc_table) {
			NBAK_TRACE(("Dropping alloc table"));
#ifdef SUPERSERVER
			adjust_state_lock->enter();
#endif
			delete alloc_table;
			alloc_table = NULL;
			last_allocated_page = 0;
#ifdef SUPERSERVER
			adjust_state_lock->leave();
#endif
		}
	}
#ifndef SUPERSERVER
	if (diff_use_count && new_backup_state == nbak_state_normal) {				
		database->dbb_flags |= DBB_bugcheck;
		status[0] = isc_arg_gds;
		status[1] = isc_bug_check;
		status[2] = isc_arg_string;
		status[3] =
			(ISC_STATUS)(U_IPTR) ERR_cstring("Difference file is in use while backup is already finished");
		status[4] = isc_arg_end;
		return false;
	}
#endif
	if (diff_pending_close 
#ifndef SUPERSERVER
		&& diff_use_count == 0
#endif
	) 
	{
			NBAK_TRACE(("Close difference file"));
#ifdef SUPERSERVER
			adjust_state_lock->enter();
#endif
#ifndef SUPERSERVER
			diff_generation++;
#endif
			PIO_close(diff_file);
			diff_file = NULL;
#ifdef SUPERSERVER
			adjust_state_lock->leave();
#endif			
			diff_pending_close = false;
	}
	if (new_backup_state != nbak_state_normal && !diff_file) {
#ifdef SUPERSERVER
		adjust_state_lock->enter();
#endif
		try {
			NBAK_TRACE(("Open difference file"));
			diff_file = PIO_open(database, diff_name, strlen(diff_name), false,
								 NULL, diff_name, strlen(diff_name));
		} catch(const std::exception& ex) {
#ifdef SUPERSERVER
			adjust_state_lock->leave();
#endif
			Firebird::stuff_exception(status, ex);
			gds__log_status(diff_name, status);
			return false;
		}
#ifdef SUPERSERVER
		adjust_state_lock->leave();
#endif
	}
	// Adjust state at the very and to ensure proper error handling
	backup_state = new_backup_state;
	return true;
}

