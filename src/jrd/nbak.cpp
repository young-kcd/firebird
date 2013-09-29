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
 */

#include "firebird.h"
#include "common.h"
#include "jrd.h"
#include "nbak.h"
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

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef NBAK_DEBUG
IMPLEMENT_TRACE_ROUTINE(nbak_trace, "NBAK")
#endif


using namespace Jrd;

/******************************** NBACKUP STATE SYNCHRONIZER ******************************/
NBackupState::NBackupState(thread_db* tdbb, MemoryPool& p, BackupManager *bakMan):
	GlobalRWLock(tdbb, p, LCK_backup_database, 0, NULL)
{
	backup_manager = bakMan;
	flags = 0;
}


void NBackupState::fetch(thread_db* tdbb)
{
	if (!backup_manager->actualize_state(tdbb))
	{
		// We can't actualize backup state and continue
		ERR_bugcheck_msg("Error: can't actualize backup state");
	}
}

void NBackupState::invalidate(thread_db* tdbb, bool ast_handler)
{
	backup_manager->set_state(nbak_state_unknown);
	NBAK_TRACE_AST( ("set state nbak_state_unknown") );
	backup_manager->closeDelta();
}

void NBackupState::blockingAstHandler(thread_db* tdbb)
{
	// SuperServer never calls CCH_flush_ast and flushs all buffers once in BackupManager::lock_clean_database
	if (!backup_manager->database_flush_in_progress() && cached_lock->lck_physical == LCK_read) {
		CCH_flush_ast(tdbb);
		NBAK_TRACE_AST(("database FLUSHED"));
	}
	GlobalRWLock::blockingAstHandler(tdbb);
}
/******************************** NBACKUP ALLOCATION TABLE SYNCHRONIZER ******************************/
NBackupAlloc::NBackupAlloc(thread_db* tdbb, MemoryPool& p, BackupManager *bakMan):
	GlobalRWLock(tdbb, p, LCK_backup_alloc, 0, NULL)
{
	backup_manager = bakMan;
}

void NBackupAlloc::fetch(thread_db* tdbb)
{
	if (!backup_manager->actualize_alloc(tdbb))
	{
		// We can't actualize backup allocation table and continue
		ERR_bugcheck_msg("Error: can't actualize alloc table");
	}
}

void NBackupAlloc::invalidate(thread_db* tdbb, bool ast_handler)
{
	NBAK_TRACE(("invalidate alloc table"));
}


/******************************** LOCK FUNCTIONS ******************************/

void BackupManager::checkout_dirty_page(thread_db* tdbb, SLONG owner_handle)
{
	// This function can not be called from AST level
	if (!(tdbb->tdbb_flags & TDBB_backup_write_locked)) {
		if (!database_lock->lock(tdbb, LCK_read, true, owner_handle)) {
			ERR_bugcheck_msg("Error: backup_database lock on checkout_dirty_page");
		}
		NBAK_TRACE(("checkout_dirty_page"));
	}
}

// NOTE: This method must be signal safe and may ba called from AST handler
void BackupManager::release_dirty_page(thread_db* tdbb, SLONG owner_handle)
{
	if (!(tdbb->tdbb_flags & TDBB_backup_write_locked)) {
		database_lock->unlock(tdbb, LCK_read, owner_handle);
		NBAK_TRACE(("release_dirty_page"));
	}
}

void BackupManager::change_dirty_page_owner(thread_db* tdbb, SLONG from_handle, SLONG to_handle)
{
	if (tdbb->tdbb_flags & TDBB_backup_write_locked || from_handle == to_handle) {
		NBAK_TRACE(("Return from change_database_lock_owner"));
		return;
	}
	database_lock->changeLockOwner(tdbb, LCK_read, from_handle, to_handle);
	NBAK_TRACE(("change lock owner old=%i, new=%i", from_handle, to_handle));
}

void BackupManager::lock_clean_database(
	thread_db* tdbb, SSHORT wait, WIN* window) 
{
	database_lock->flags |= NBAK_state_blocking;
	// Flush local cache to release all dirty pages
	CCH_flush(tdbb, FLUSH_ALL, 0);
	if (!database_lock->lock(tdbb, LCK_write, wait)) {
		database_lock->flags &= ~NBAK_state_blocking;
		ERR_bugcheck_msg("Error: can't lock clean database");
	}
	NBAK_TRACE(("database write locked"));
	database_lock->flags &= ~NBAK_state_blocking;
	tdbb->tdbb_flags |= TDBB_backup_write_locked;
	CCH_FETCH(tdbb, window, LCK_write, pag_header);
}
 
void BackupManager::unlock_clean_database(
	thread_db* tdbb) 
{
	tdbb->tdbb_flags &= ~TDBB_backup_write_locked;
	database_lock->unlock(tdbb, LCK_write);

	NBAK_TRACE(("database write unlocked"));
} 

void BackupManager::lock_shared_database(thread_db* tdbb, SSHORT wait)
{
	if (!(tdbb->tdbb_flags & TDBB_backup_write_locked))
		if (!database_lock->lock(tdbb, LCK_read, wait)) {
			ERR_bugcheck_msg("Error: can't lock database on llRead");
		}
}

void BackupManager::unlock_shared_database(thread_db* tdbb)
{
	if (!(tdbb->tdbb_flags & TDBB_backup_write_locked))
		database_lock->unlock(tdbb, LCK_read);
}

void BackupManager::lock_alloc_write(thread_db* tdbb, SSHORT wait)
{
	fb_assert(backup_state == nbak_state_stalled);
	if (!alloc_lock->lock(tdbb, LCK_write, wait)) {
		ERR_bugcheck_msg("Error: lock allocation table on write");
	}
	NBAK_TRACE(("lock_alloc_write"));
}

void BackupManager::unlock_alloc_write(thread_db* tdbb)
{
	NBAK_TRACE(("unlock_alloc_write"));
	alloc_lock->unlock(tdbb, LCK_write);
}

void BackupManager::lock_alloc(thread_db* tdbb, SSHORT wait)
{
	if (!alloc_lock->lock(tdbb, LCK_read, wait)) {
		ERR_bugcheck_msg("Error: lock allocation table on read");
	}
	NBAK_TRACE(("lock_alloc"));
}

void BackupManager::unlock_alloc(thread_db* tdbb)
{
	NBAK_TRACE(("unlock_alloc"));
	alloc_lock->unlock(tdbb, LCK_read);
}

/********************************** CORE LOGIC ********************************/

void BackupManager::generate_filename()
{
	diff_name = database->dbb_filename + ".delta";
	explicit_diff_name = false;
}

void BackupManager::openDelta()
{
    fb_assert(!diff_file);
    diff_file = PIO_open(database, diff_name, false, diff_name, false);

    if (database->dbb_flags & (DBB_force_write | DBB_no_fs_cache))
    {
        setForcedWrites(database->dbb_flags & DBB_force_write,
						database->dbb_flags & DBB_no_fs_cache);
    }
}

void BackupManager::closeDelta()
{
    if (diff_file)
    {
        PIO_close(diff_file);
        diff_file = NULL;
    }
}

// Initialize and open difference file for writing
void BackupManager::begin_backup(thread_db* tdbb)
{
	NBAK_TRACE(("begin_backup"));

	// Check for raw device
	if ((!explicit_diff_name) && database->onRawDevice()) {
		ERR_post(isc_need_difference, isc_arg_end);
	}

	WIN window(HEADER_PAGE_NUMBER);

	bool header_locked = false;
	bool database_locked = false;

	try {
		lock_clean_database(tdbb, true, &window);
		database_locked = true;
		Ods::header_page* header = (Ods::header_page*) window.win_buffer;
		header_locked = true;
		NBAK_TRACE(("header locked"));

		// Check state
		if (backup_state != nbak_state_normal) {
			NBAK_TRACE(("end backup - invalid state %d", backup_state));
			CCH_RELEASE(tdbb, &window);
			unlock_clean_database(tdbb);

			return;
		}

		// Create file	
		NBAK_TRACE(("Creating difference file %s", diff_name.c_str()));
		diff_file = PIO_create(database, diff_name, true, false, false);
		if (database->dbb_flags & (DBB_force_write | DBB_no_fs_cache))
		{
			setForcedWrites(database->dbb_flags & DBB_force_write, 
							database->dbb_flags & DBB_no_fs_cache);
		}
#ifdef UNIX 
		// adjust difference file access rights to make it match main DB ones
		if (diff_file && geteuid() == 0) {
			struct stat st;
			while (fstat(database->dbb_page_manager.findPageSpace(DB_PAGE_SPACE)->file->fil_desc, &st) != 0) {
				if (errno != EINTR) {
					Firebird::system_call_failed::raise("fstat");
				}
			}
			while (fchown(diff_file->fil_desc, st.st_uid, st.st_gid) != 0) {
				if (errno != EINTR) {
					Firebird::system_call_failed::raise("fchown");
				}
			}
			while (fchmod(diff_file->fil_desc, st.st_mode) != 0) {
				if (errno != EINTR) {
					Firebird::system_call_failed::raise("fchmod");
				}
			}
		}
#endif
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
		// Set state in database header page. All changes are written to main database file yet.
		CCH_MARK_MUST_WRITE(tdbb, &window);
		int newState = nbak_state_stalled;
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | newState;
		const ULONG adjusted_scn = ++header->hdr_header.pag_scn; // Generate new SCN
		PAG_replace_entry_first(header, Ods::HDR_backup_guid, sizeof(guid), 
			reinterpret_cast<const UCHAR*>(&guid));

		header_locked = false;
		CCH_RELEASE(tdbb, &window);
	
		backup_state = newState;
		current_scn = adjusted_scn;

		database_locked = false;
		unlock_clean_database(tdbb);

		// All changes go to the difference file now
	}
	catch (const Firebird::Exception&) {
		backup_state = nbak_state_unknown;
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		if (database_locked)
			unlock_clean_database(tdbb);
		throw;
	}
}


// Determine actual DB size (raw devices support)
ULONG BackupManager::getPageCount()
{
	if (backup_state != nbak_state_stalled)
	{
		// calculate pages only when database is locked for backup:
		// other case such service is just dangerous
		return 0;
	}
	
	class PioCount : public Jrd::PageCountCallback
	{
	private:
	    BufferDesc temp_bdb;
		PageSpace* pageSpace;

	public:
		explicit PioCount(Database* d)
		{
			fb_assert(d);
			temp_bdb.bdb_dbb = d;
			pageSpace = d->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
			fb_assert(pageSpace);
		}
		virtual void newPage(const SLONG pageNum, Ods::pag* buf)
		{
			temp_bdb.bdb_buffer = buf;
			temp_bdb.bdb_page = pageNum;
			ISC_STATUS_ARRAY status;
			if (!PIO_read(pageSpace->file, &temp_bdb, temp_bdb.bdb_buffer, status)) 
			{
				Firebird::status_exception::raise(status);
			}
		}
	};
	PioCount pioCount(database);
	
	return PAG_page_count(database, &pioCount);
}


// Merge difference file to main files (if needed) and unlink() difference 
// file then. If merge is already in progress method silently returns and 
// does nothing (so it can be used for recovery on database startup). 
void BackupManager::end_backup(thread_db* tdbb, bool recover) 
{
	NBAK_TRACE(("end_backup, recover=%i", recover));

	// Check for recover
	Database *dbb = tdbb->getDatabase();

	GlobalRWLock endLock(tdbb, *database->dbb_permanent, LCK_backup_end, 0, NULL, LCK_OWNER_attachment, LCK_OWNER_attachment, false);

	if (!endLock.lock(tdbb, LCK_write, LCK_NO_WAIT)) {
		// Someboby holds write lock on LCK_backup_end. We need not to do end_backup
		return;
	}

	// STEP 1. Change state in header to "merge"
	WIN window(HEADER_PAGE_NUMBER);
	Ods::header_page* header;

	bool database_locked = false;
	bool header_locked = false;

	ULONG adjusted_scn; // We use this value to prevent race conditions.
						// They are possible because we release state lock
						// for some instants and anything is possible at
						// that times.

	try {
		// Check state under PR lock of backup state for speed
		lock_shared_database(tdbb, true);
		// Nobody is doing end_backup but database isn't in merge state. 
		if ( (recover || backup_state != nbak_state_stalled) && (backup_state != nbak_state_merge ) ) {
			NBAK_TRACE(("invalid state %d", backup_state));
			endLock.unlock(tdbb, LCK_write);
			unlock_shared_database(tdbb);
			return;
		}
		unlock_shared_database(tdbb);
		// Here backup state can be changed. Need to check it again after lock
		lock_clean_database(tdbb, true, &window);
		database_locked = true;

		if ( (recover || backup_state != nbak_state_stalled) && (backup_state != nbak_state_merge ) ) {
			NBAK_TRACE(("invalid state %d", backup_state));
			endLock.unlock(tdbb, LCK_write);
			unlock_clean_database(tdbb);
			return;
		}
		header = (Ods::header_page*) window.win_buffer;
		header_locked = true;

		NBAK_TRACE(("difference file %s, current backup state is %d", diff_name.c_str(), backup_state));

		// Set state in database header
		backup_state = nbak_state_merge;
		adjusted_scn = ++current_scn;
		NBAK_TRACE(("New state is getting to become %d", backup_state));
		CCH_MARK_MUST_WRITE(tdbb, &window);
		// Generate new SCN
		header->hdr_header.pag_scn = current_scn;
		NBAK_TRACE(("new SCN=%d is getting written to header", header->hdr_header.pag_scn));
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | backup_state;
		CCH_RELEASE(tdbb, &window);
		header_locked = false;
		NBAK_TRACE(("Setting state %d in header page is over", backup_state));

		if (database_locked) {
			unlock_clean_database(tdbb);
			database_locked = false;
		}
	}
	catch (const Firebird::Exception&) {
		backup_state = nbak_state_unknown;
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		if (database_locked)
			unlock_clean_database(tdbb);
		endLock.unlock(tdbb, LCK_write);
		throw;
	}

	
	// STEP 2. Merging database and delta
	// Here comes the dirty work. We need to reapply all changes from difference file to database
	// Release write state lock and get read lock. 
	// Merge process should not inhibit normal operations.
	lock_shared_database(tdbb, true);
	database_locked = true;
	NBAK_TRACE(("database locked to merge"));

	try {
		NBAK_TRACE(("Merge. State=%d, current_scn=%d, adjusted_scn=%d",
			backup_state, current_scn, adjusted_scn));	

		actualize_alloc(tdbb);
		NBAK_TRACE(("Merge. Alloc table is actualized."));	
		AllocItemTree::Accessor all(alloc_table);
		
		if (all.getFirst()) {
			do {
				WIN window2(DB_PAGE_SPACE, all.current().db_page);
				NBAK_TRACE(("Merge page %d, diff=%d", all.current().db_page, all.current().diff_page));
				Ods::pag* page = CCH_FETCH(tdbb, &window2, LCK_write, pag_undefined);
				NBAK_TRACE(("Merge: page %d is fetched", all.current().db_page));
				if (page->pag_scn != current_scn) {
					CCH_MARK(tdbb, &window2);
					NBAK_TRACE(("Merge: page %d is marked", all.current().db_page));
				}
				CCH_RELEASE(tdbb, &window2);
				NBAK_TRACE(("Merge: page %d is released", all.current().db_page));
			} while (all.getNext());
		}

		if (database_locked)
			unlock_shared_database(tdbb);
		database_locked = false;

		NBAK_TRACE(("Merging is over. Database unlocked"));
		
	}
	catch (const Firebird::Exception&) {
		endLock.unlock(tdbb, LCK_write);
		if (database_locked)
			unlock_shared_database(tdbb);
		throw;
	}

	// STEP 3. Change state in header to "normal"
	// We finished. We need to reflect it in our database header page
	try {
		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		lock_clean_database(tdbb, true, &window);
		database_locked = true;
		header = (Ods::header_page*) window.win_buffer;
		header_locked = true;

		// Set state in database header
		backup_state = nbak_state_normal;
		CCH_MARK_MUST_WRITE(tdbb, &window);
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~Ods::hdr_backup_mask) | backup_state;
		NBAK_TRACE(("Set state %d in header page", backup_state));
		// Generate new SCN
		header->hdr_header.pag_scn = ++current_scn;
		NBAK_TRACE(("new SCN=%d is getting written to header", header->hdr_header.pag_scn));
		CCH_RELEASE(tdbb, &window);
		header_locked = false;
		
		// Page allocation table cache is no longer valid
		NBAK_TRACE(("Dropping alloc table"));
		delete alloc_table;
		alloc_table = NULL;
		last_allocated_page = 0;
		if (!alloc_lock->tryReleaseLock(tdbb))
			ERR_bugcheck_msg("There are holders of alloc_lock after end_backup finish");

		closeDelta();
		unlink(diff_name.c_str());
		
		if (database_locked)
			unlock_clean_database(tdbb);
		NBAK_TRACE(("backup is over"));
		endLock.unlock(tdbb, LCK_write);
	}
	catch (const Firebird::Exception&) {
		backup_state = nbak_state_unknown;
		endLock.unlock(tdbb, LCK_write);
		if (header_locked)
			CCH_RELEASE(tdbb, &window);
		if (database_locked)
			unlock_clean_database(tdbb);
		throw;
	}

	return;
}
	
bool BackupManager::actualize_alloc(thread_db* tdbb)
{
	ISC_STATUS *status_vector = tdbb->tdbb_status_vector;
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
		
			if (!PIO_read(diff_file, &temp_bdb, temp_bdb.bdb_buffer, status_vector)) {
				return false;
			}
			for (ULONG i = last_allocated_page - temp_bdb.bdb_page.getPageNum(); i < alloc_buffer[0]; i++)
			{
				NBAK_TRACE(("alloc item page=%d, diff=%d", alloc_buffer[i + 1], temp_bdb.bdb_page.getPageNum() + i + 1));
				if (!alloc_table->add(AllocItem(alloc_buffer[i + 1], temp_bdb.bdb_page.getPageNum() + i + 1)))
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
			last_allocated_page = temp_bdb.bdb_page.getPageNum() + alloc_buffer[0];
			if (alloc_buffer[0] == database->dbb_page_size / sizeof(ULONG) - 1)
				// if page is full adjust position for next pointer page
				last_allocated_page++;
			else
				// We finished reading allocation table
				break;		
		}
	}
	catch (const Firebird::Exception& ex) {
		// Handle out of memory error, etc
		delete alloc_table;
		Firebird::stuff_exception(status_vector, ex);
		alloc_table = NULL;
		last_allocated_page = 0;
		return false;
	}
	return true;
}

// Return page index in difference file that can be used in 
// write_difference call later. 
ULONG BackupManager::get_page_index(thread_db* tdbb, ULONG db_page)
{
	NBAK_TRACE(("get_page_index"));

	AllocItemTree::Accessor a(alloc_table);
	ULONG diff_page = a.locate(db_page) ? a.current().diff_page : 0;

	return diff_page;
}

// Mark next difference page as used by some database page
ULONG BackupManager::allocate_difference_page(thread_db* tdbb, ULONG db_page)
{
	// This page may be allocated by other
	if (ULONG diff_page = get_page_index(tdbb, db_page)) {
		return diff_page;
	}

	NBAK_TRACE(("allocate_difference_page"));
	fb_assert(last_allocated_page % (database->dbb_page_size / sizeof(ULONG)) == alloc_buffer[0]);

	ISC_STATUS* status_vector = tdbb->tdbb_status_vector;
	// Grow file first. This is done in such order to keep difference
	// file consistent in case of write error. We should always be able 
	// to read next alloc page when previous one is full.
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = last_allocated_page + 1;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(empty_buffer);
	if (!PIO_write(diff_file, &temp_bdb, (Ods::pag*)empty_buffer, status_vector)) {
		return 0;
	}
	
	const bool alloc_page_full = alloc_buffer[0] == database->dbb_page_size / sizeof(ULONG) - 2;
	if (alloc_page_full) {
		// Pointer page is full. Its time to create new one.
		temp_bdb.bdb_page = last_allocated_page + 2;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(empty_buffer);
		if (!PIO_write(diff_file, &temp_bdb, (Ods::pag*)empty_buffer, status_vector)) {
			return 0;
		}
	}

	// Write new item to the allocation table
	temp_bdb.bdb_page = last_allocated_page & ~(database->dbb_page_size / sizeof(ULONG) - 1);
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(alloc_buffer);
	alloc_buffer[++alloc_buffer[0]] = db_page;
	if (!PIO_write(diff_file, &temp_bdb, temp_bdb.bdb_buffer, status_vector)) {
		return 0;
	}
	last_allocated_page++;
	// Register new page in the alloc table
	try {
		alloc_table->add(AllocItem(db_page, last_allocated_page));
	}
	catch (const Firebird::Exception& ex) {
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

bool BackupManager::write_difference(ISC_STATUS* status, ULONG diff_page, Ods::pag* page)
{
	NBAK_TRACE(("write_diff"));
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	// Check that diff page is not allocation page
	fb_assert(diff_page % (database->dbb_page_size / sizeof(ULONG)));
	if (!PIO_write(diff_file, &temp_bdb, page, status))
		return false;
	return true;
}
	
bool BackupManager::read_difference(thread_db* tdbb, ULONG diff_page, Ods::pag* page)
{
	NBAK_TRACE(("read_diff"));
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	if (!PIO_read(diff_file, &temp_bdb, page, tdbb->tdbb_status_vector))		
		return false;
	return true;	
}

void BackupManager::flush_difference()
{
	if (diff_file)
		PIO_flush(diff_file);
}

void BackupManager::setForcedWrites(const bool forceWrite, const bool notUseFSCache)
{
	if (diff_file)
		PIO_force_write(diff_file, forceWrite, notUseFSCache);
}

BackupManager::BackupManager(thread_db* tdbb, Database* _database, int ini_state) :
	dbCreating(false), database(_database), diff_file(NULL), alloc_table(NULL),
	last_allocated_page(0), current_scn(0), diff_name(*_database->dbb_permanent),
	explicit_diff_name(false)
{
	// Allocate various database page buffers needed for operation
	temp_buffers_space = FB_NEW(*database->dbb_permanent) BYTE[database->dbb_page_size * 3 + MIN_PAGE_SIZE];
	// Align it at sector boundary for faster IO (also guarantees correct alignment for ULONG later)
	BYTE *temp_buffers = reinterpret_cast<BYTE*>(
		FB_ALIGN(reinterpret_cast<U_IPTR>(temp_buffers_space), MIN_PAGE_SIZE));
	memset(temp_buffers, 0, database->dbb_page_size * 3);
	
	backup_state = ini_state;

	empty_buffer = reinterpret_cast<ULONG*>(temp_buffers);
	spare_buffer = reinterpret_cast<ULONG*>(temp_buffers + database->dbb_page_size);
	alloc_buffer = reinterpret_cast<ULONG*>(temp_buffers + database->dbb_page_size * 2);

	database_lock = new NBackupState(tdbb, *database->dbb_permanent, this);
	alloc_lock = new NBackupAlloc(tdbb, *database->dbb_permanent, this);

	NBAK_TRACE(("Create BackupManager, database=%s", database->dbb_filename.c_str()));
}

void BackupManager::shutdown_locks(thread_db* tdbb)
{
	delete alloc_lock;
	alloc_lock = NULL;
	delete database_lock;
	database_lock = NULL;
}


BackupManager::~BackupManager()
{
	delete alloc_table;
	delete[] temp_buffers_space;
}

void BackupManager::set_difference(thread_db* tdbb, const char* filename) 
{
	if (filename) {
		WIN window(HEADER_PAGE_NUMBER);
		Ods::header_page* header =
			(Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		PAG_replace_entry_first(header, Ods::HDR_difference_file, 
			strlen(filename), reinterpret_cast<const UCHAR*>(filename));
		CCH_RELEASE(tdbb, &window);
		diff_name = filename;
		explicit_diff_name = true;
	}
	else {
		PAG_delete_clump_entry(HEADER_PAGE, Ods::HDR_difference_file);
		generate_filename();
	}
}

bool BackupManager::actualize_state(thread_db* tdbb)
{
	// State is unknown. We need to read it from the disk.
	// We cannot use CCH for this because of likely recursion.
	NBAK_TRACE(("actualize_state, current_state=%i", backup_state));

	if (dbCreating) {
		backup_state = nbak_state_normal;
		return true;
	}

	ISC_STATUS *status = tdbb->tdbb_status_vector;
			
	// Read original page from database file or shadows.
	SSHORT retryCount = 0;
	Ods::header_page* header = reinterpret_cast<Ods::header_page*>(spare_buffer);
	BufferDesc temp_bdb;
	temp_bdb.bdb_page = HEADER_PAGE_NUMBER;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<Ods::pag*>(header);
	PageSpace* pageSpace = 
		database->dbb_page_manager.findPageSpace(DB_PAGE_SPACE);
	fb_assert(pageSpace);
	jrd_file* file = pageSpace->file;
	while (!PIO_read(file, &temp_bdb, temp_bdb.bdb_buffer, status)) {
		if (!CCH_rollover_to_shadow(database, file, false)) {
			NBAK_TRACE(("Shadow change error"));
			return false;
		}
		if (file != pageSpace->file)
			file = pageSpace->file;
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

	// Read difference file name from header clumplets
	explicit_diff_name = false;
	const UCHAR* p = header->hdr_data;
	while (true) {
		switch (*p) {
		case Ods::HDR_backup_guid:
			p += p[1] + 2;
			continue;
		case Ods::HDR_difference_file:
			explicit_diff_name = true;
			diff_name.assign(reinterpret_cast<const char*>(p + 2), p[1]);
		}
		break;
	}
	if (!explicit_diff_name)
		generate_filename();
		
	if (new_backup_state == nbak_state_normal || missed_cycle) {
		// Page allocation table cache is no longer valid.
		if (alloc_table) {
			NBAK_TRACE(("Dropping alloc table"));
			delete alloc_table;
			alloc_table = NULL;
			last_allocated_page = 0;
			if (!alloc_lock->tryReleaseLock(tdbb))
				ERR_bugcheck_msg("There are holders of alloc_lock after end_backup finish");
		}
	}

	if (new_backup_state != nbak_state_normal && !diff_file) {
		try {
			NBAK_TRACE(("Open difference file"));
			openDelta();
		}
		catch (const Firebird::Exception& ex) {
			Firebird::stuff_exception(status, ex);
			return false;
		}
	}
	// Adjust state at the very and to ensure proper error handling
	backup_state = new_backup_state;

	return true;
}

void BackupManager::shutdown(thread_db* tdbb) 
{
    closeDelta();
    shutdown_locks(tdbb);
	delete this;
}
