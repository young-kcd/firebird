/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbak.cpp
 *	DESCRIPTION:	New backup technology
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 */

#include "firebird.h"
#include "jrd.h"
#include "ods.h"
#include "lck.h"
#include "cch.h"
#include "lck_proto.h"
#include "pag_proto.h"
#include "err_proto.h"
#include "cch_proto.h"
#include "isc_proto.h"
#include "thd_proto.h"
#include "os/pio_proto.h"
#include "gen/codes.h"
#include "gds_proto.h"
#include "os/guid.h"

void BackupManager::generate_filename() {
	strncpy(diff_name, (char*)database->dbb_filename->str_data, sizeof(diff_name));
	strncat(diff_name, ".delta", sizeof(diff_name)-strlen(diff_name)-1);
}

void BackupManager::lock_state_write() {
#ifdef SUPERSERVER
	state_lock->beginWrite();
#else
	TDBB tdbb = GET_THREAD_DATA;
	if (!LCK_lock(tdbb, state_lock, LCK_EX, LCK_WAIT))
		// This is OK because state changing code expect it
		ERR_post(gds_lock_conflict, 0); 
#endif
}

bool BackupManager::try_lock_state_write() {
#ifdef SUPERSERVER
	return state_lock->tryBeginWrite();
#else
	TDBB tdbb = GET_THREAD_DATA;
	return LCK_lock(tdbb, state_lock, LCK_EX, LCK_NO_WAIT);
#endif
}

void BackupManager::unlock_state_write() {
#ifdef SUPERSERVER
	state_lock->endWrite();
#else
	TDBB tdbb = GET_THREAD_DATA;
	LCK_release(tdbb, state_lock);
	backup_state = nbak_state_unknown; // We know state only as long we lock it
#endif
}

bool BackupManager::lock_alloc_write() {
#ifdef SUPERSERVER
	alloc_lock->beginWrite();
	return true;
#else
	TDBB tdbb = GET_THREAD_DATA;
	return LCK_lock(tdbb, alloc_lock, LCK_EX, LCK_WAIT);
#endif
}

void BackupManager::unlock_alloc_write() {
#ifdef SUPERSERVER
	alloc_lock->endWrite();
#else
	TDBB tdbb = GET_THREAD_DATA;
	if (ast_flags & NBAK_alloc_blocking) {
		LCK_release(tdbb, alloc_lock);
		ast_flags &= ~NBAK_alloc_blocking;
		ast_flags |= NBAK_alloc_dirty;
	} else
		LCK_convert(tdbb, alloc_lock, LCK_SR, LCK_WAIT);
#endif
}

// Initialize and open difference file for writing
bool BackupManager::begin_backup() {	
	TRACE("begin_backup");
	TDBB tdbb = GET_THREAD_DATA;
	lock_state_write();
	TRACE("state locked");
	try {
		if (!actualize_state(tdbb->tdbb_status_vector))
			ERR_punt();
		// Check state
		if (backup_state != nbak_state_normal) {
			TRACE1("end backup - invalid state %d", backup_state);
			unlock_state_write();
			return false;
		}
		// Create file
		TRACE1("Creating difference file %s", diff_name);
		diff_file = PIO_create(database, diff_name, strlen(diff_name), TRUE);
		// Zero out first page (empty allocation table)
		if (!alloc_buffer)
			alloc_buffer = reinterpret_cast<ULONG*> (
					FB_ALIGN((U_IPTR)FB_NEW(*database->dbb_permanent) 
						char[database->dbb_page_size + MIN_PAGE_SIZE], MIN_PAGE_SIZE));
		bdb temp_bdb;
		temp_bdb.bdb_page = 0;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<PAG>(alloc_buffer);
		memset(alloc_buffer, 0, database->dbb_page_size);
		if (!PIO_write(diff_file, &temp_bdb, (PAG)alloc_buffer, tdbb->tdbb_status_vector))
			ERR_punt();
		TRACE("Set backup state in header");
		FB_GUID guid;
		GenerateGuid(&guid);
		// Flush buffers to prevent the amount of allocated pages from increasing
		// Before lock we'll write only header then. This may not change the number of
		// pages in file.
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		CCH_flush(tdbb, FLUSH_ALL, 0);
		// Set state in database header page. All changes are written to main database file yet.
		WIN window;
		HDR header;
		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		int newState = nbak_state_stalled;
		header->hdr_flags = (header->hdr_flags & ~hdr_backup_mask) | newState;
		backup_pages = header->hdr_backup_pages = PIO_act_alloc(database);
		ULONG adjusted_scn = ++header->hdr_header.pag_scn(); // Generate new SCN
		PAG_replace_entry_first(header, HDR_backup_guid, sizeof(guid), (UCHAR*)&guid);
		CCH_RELEASE(tdbb, &window);
		CCH_flush(tdbb, FLUSH_ALL, 0);
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
	
		backup_state = newState;
		current_scn = adjusted_scn;
		// This is essential for SuperServer to prevent multi-threading issues
		// We already modified state in header, error here is not very important
		actualize_alloc(tdbb->tdbb_status_vector); 
		unlock_state_write();
		return true;
	} catch (const std::exception&) {
		unlock_state_write();
		backup_state = nbak_state_unknown;
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		throw;
	}
}

// Merge difference file to main files (if needed) and unlink() difference 
// file then. If merge is already in progress method silently returns and 
// does nothing (so it can be used for recovery on database startup). 
bool BackupManager::end_backup(bool recover) {
	TRACE("end_backup");
	TDBB tdbb = GET_THREAD_DATA;
	ULONG adjusted_scn; // We use this value to prevent race conditions.
						// They are possible because we release state lock
						// for some instants and anything is possible at
						// that times.
	if (recover) {
		if (!try_lock_state_write())
			return false;		
	} else
		lock_state_write();
	TRACE("state locked");
	try {
		// Check state
		if (!actualize_state(tdbb->tdbb_status_vector))
			ERR_punt();
		TRACE1("Backup state at start of end_backup is %d", backup_state);
		if (backup_state == nbak_state_normal || (recover && backup_state != nbak_state_merge)) {
			TRACE1("invalid state %d", backup_state);
			unlock_state_write();
			return false;
		}
		TRACE1("difference file %s", diff_name);
		// Set state in database header
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		TRACE1("Current backup state is %d", backup_state);
		backup_state = nbak_state_merge;
		adjusted_scn = ++current_scn;
		WIN window;
		HDR header;
		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		TRACE1("New state is getting to become %d", backup_state);
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		TRACE1("New state is getting to become after fetches %d", backup_state);
		// Generate new SCN
		header->hdr_header.pag_scn() = current_scn;
		TRACE1("new SCN=%d is getting written to header", adjusted_scn);
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~hdr_backup_mask) | backup_state;
		CCH_RELEASE(tdbb, &window);
		CCH_flush(tdbb, FLUSH_ALL, 0); // This is to adjust header in main database file
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		TRACE1("Set state %d in header page", backup_state);
	} catch (const std::exception&) {
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		unlock_state_write();
		backup_state = nbak_state_unknown;
		throw;
	}	
	
	
	// Here comes the dirty work. We need to reapply all changes from difference file to database
	
	// Release write state lock and get read lock. 
	// Merge process should not inhibit normal operations.
	tdbb->tdbb_flags &= ~TDBB_set_backup_state;
	unlock_state_write();
	if (!lock_state())
		ERR_post(gds_lock_conflict, 0);
	try {
		if (!actualize_state(tdbb->tdbb_status_vector))
			ERR_punt();
		TRACE3("Merge. State=%d, current_scn=%d, adjusted_scn=%d", backup_state, current_scn, adjusted_scn);	
		if (backup_state != nbak_state_merge || current_scn != adjusted_scn) {
			/* Handle the case when somebody finalized merge for us */
			unlock_state();
			return false;
		}
		TRACE("Status OK.");
		if (!actualize_alloc(tdbb->tdbb_status_vector))
			ERR_punt();
		TRACE1("Allocation table %p is current.", alloc_table);
		AllocItemTree::Accessor all(alloc_table);
		
		tdbb->tdbb_flags |= TDBB_set_backup_state | TDBB_backup_merge;
		
		if (all.getFirst()) do {
			WIN window;
			window.win_page = all.current().db_page;
			window.win_flags = 0;
			PAG page = CCH_FETCH(tdbb, &window, LCK_write, pag_undefined);
			if (page->pag_scn() != current_scn)
				CCH_MARK_MUST_WRITE(tdbb, &window);
			CCH_RELEASE(tdbb, &window);
		} while(all.getNext());
		CCH_flush(tdbb, FLUSH_ALL, 0); // Really write changes to main database file
		
		tdbb->tdbb_flags &= ~(TDBB_set_backup_state | TDBB_backup_merge);
		unlock_state();
		
	} catch(const std::exception&) {
		tdbb->tdbb_flags &= ~(TDBB_set_backup_state | TDBB_backup_merge);
		unlock_state();
		throw;
	}
	
	// We finished. We need to reflect it our database header page
	lock_state_write();
	try {
		// Check state
		if (!actualize_state(tdbb->tdbb_status_vector))
			ERR_punt();
		if (backup_state != nbak_state_merge || current_scn != adjusted_scn) {
			/* Handle the case when somebody finalized merge for us */
			unlock_state_write();
			return false;
		}
		// Set state in database header
		tdbb->tdbb_flags |= TDBB_set_backup_state;
		backup_state = nbak_state_normal;
		WIN window;
		HDR header;
		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		// Adjust state
		header->hdr_flags = (header->hdr_flags & ~hdr_backup_mask) | backup_state;
		TRACE1("Set state %d in header page", backup_state);
		CCH_RELEASE(tdbb, &window);
		CCH_flush(tdbb, FLUSH_ALL, 0); // This is to adjust header in main database file
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		
		// Page allocation table cache is no longer valid
		delete alloc_table;
		alloc_table = NULL;
		last_allocated_page = 0;
		
		if (diff_file) {
			PIO_close(diff_file);
			diff_file = NULL;
		}
		unlink(diff_name);
		
		unlock_state_write();
		TRACE("backup ended");
	} catch (const std::exception&) {
		tdbb->tdbb_flags &= ~TDBB_set_backup_state;
		unlock_state_write();
		backup_state = nbak_state_unknown;
		throw;
	}
	return true;
}
	
bool BackupManager::actualize_alloc(ISC_STATUS* status) {
	if (alloc_table &&
		// During merge cached allocation table contains pages for
		// SCN number current_scn-1, not for current SCN
		((backup_state!=nbak_state_merge && alloc_scn == current_scn) || 
		(backup_state==nbak_state_merge && alloc_scn+1 == current_scn))
#ifndef SUPERSERVER
		&& !(ast_flags & NBAK_alloc_dirty)
#endif
	) return true;
	TRACE("actualize_alloc");
	// For SuperServer this routine is really executed only at database startup when 
	// it has exlock or when exclusive access to database is enabled
	if ((backup_state!=nbak_state_merge && alloc_scn == current_scn) || 
		(backup_state==nbak_state_merge && alloc_scn+1 == current_scn)) 
	{
		delete alloc_table;
		alloc_table = NULL;
		last_allocated_page = 0;
		alloc_scn = backup_state==nbak_state_merge ? current_scn-1 : current_scn;
	}
	if (!alloc_table) 
		alloc_table = FB_NEW(*database->dbb_permanent) AllocItemTree(database->dbb_permanent);
	if (!alloc_buffer)
		alloc_buffer = reinterpret_cast<ULONG*> (
				FB_ALIGN((U_IPTR)FB_NEW(*database->dbb_permanent) char[database->dbb_page_size + MIN_PAGE_SIZE], 
						 MIN_PAGE_SIZE));
	while (true) {
		bdb temp_bdb;
		// Difference file pointer pages have one ULONG as number of pages allocated on the page and
		// then go physical numbers of pages from main database file. Offsets of numbers correspond
		// to difference file pages.
		
		// Get offset of pointer page. We can do so because page sizes are powers of 2
		temp_bdb.bdb_page = last_allocated_page & ~(database->dbb_page_size/sizeof(ULONG)-1);
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<PAG>(alloc_buffer);
		
		if (!PIO_read(diff_file, &temp_bdb, (PAG)alloc_buffer, status))
			return false;
		for (ULONG i=last_allocated_page-temp_bdb.bdb_page; i < alloc_buffer[0]; i++)
			alloc_table->add(AllocItem(alloc_buffer[i+1], temp_bdb.bdb_page+i+1));
		last_allocated_page = temp_bdb.bdb_page + alloc_buffer[0];
		if (alloc_buffer[0] == database->dbb_page_size/sizeof(ULONG)-1)
			// if page is full adjust position for next pointer page
			last_allocated_page++;
		else
			// We finished reading allocation table
			break;		
	}
#ifndef SUPERSERVER
	ast_flags &= ~NBAK_alloc_dirty;
#endif
	return true;
}

// Return page index in difference file that can be used in 
// write_difference call later. 
ULONG BackupManager::get_page_index(ULONG db_page) const {
	AllocItemTree::Accessor a(alloc_table);
	if (a.locate(db_page))
		return a.current().diff_page;
	else
		return 0;
}

// Mark next difference page as used by some database page
bool BackupManager::mark_alloc(ISC_STATUS* status, ULONG db_page) {
	// Grow file first. This is done in such order to keep difference
	// file consistent in case of write error. We should always be able 
	// to read next alloc page when previous one is full.
	if (alloc_buffer[0] == database->dbb_page_size/sizeof(ULONG)-2) {
		// Pointer page is full. Its time to create new one.
		char *spare_buffer = FB_NEW(*database->dbb_permanent) char[database->dbb_page_size + MIN_PAGE_SIZE];
		PAG spare_page = (PAG)FB_ALIGN((U_IPTR)spare_buffer, MIN_PAGE_SIZE);
		
		bdb temp_bdb;
		temp_bdb.bdb_page = last_allocated_page+2;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<PAG>(alloc_buffer);
		memset(spare_page, 0, database->dbb_page_size);
		if (!PIO_write(diff_file, &temp_bdb, spare_page, status)) {
			delete spare_buffer;
			return false;
		}
		delete spare_buffer;
	}

	// Write new item to the allocation table
	bdb temp_bdb;
	temp_bdb.bdb_page = last_allocated_page & ~(database->dbb_page_size/sizeof(ULONG)-1);
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = reinterpret_cast<PAG>(alloc_buffer);
	alloc_buffer[++alloc_buffer[0]] = db_page;
	if (!PIO_write(diff_file, &temp_bdb, (PAG)alloc_buffer, status))
		return false;
	last_allocated_page++;
	// Register new page in the alloc table
	alloc_table->add(AllocItem(db_page, last_allocated_page));
	// Adjust buffer and counters if we allocated new alloc page earlier
	if (alloc_buffer[0] == database->dbb_page_size/sizeof(ULONG)-1) {		
		last_allocated_page++;
		memset(alloc_buffer, 0, database->dbb_page_size);			
	}
	
	return true;
}

bool BackupManager::write_difference(ISC_STATUS* status, ULONG diff_page, struct pag* page) {
	bdb temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	if (!PIO_write(diff_file, &temp_bdb, page, status))
		return false;
	return true;
}
	
bool BackupManager::read_difference(ISC_STATUS* status, ULONG diff_page, struct pag* page) {
	bdb temp_bdb;
	temp_bdb.bdb_page = diff_page;
	temp_bdb.bdb_dbb = database;
	temp_bdb.bdb_buffer = page;
	if (!PIO_read(diff_file, &temp_bdb, page, status))		
		return false;
	return true;
}
	
BackupManager::BackupManager(DBB _database, int ini_state) : 
	database(_database), diff_file(NULL), alloc_table(NULL), 
	backup_state(ini_state), last_allocated_page(0), 
	alloc_buffer(NULL), current_scn(0), alloc_scn(0)
{
	TDBB tdbb = GET_THREAD_DATA;
	diff_name[0] = 0;
		
#ifdef SUPERSERVER
	alloc_lock = FB_NEW(*_database->dbb_permanent) Firebird::RWLock();
	state_lock = FB_NEW(*_database->dbb_permanent) Firebird::RWLock();	
	adjust_state_lock = FB_NEW(*_database->dbb_permanent) Firebird::Spinlock();	
#else
	ast_flags = 0;
	
	state_lock = FB_NEW_RPT(*database->dbb_permanent, 0) lck();
	state_lock->lck_type = LCK_backup_state;
	state_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, state_lock->lck_type);
	state_lock->lck_parent = database->dbb_lock;
	state_lock->lck_length = 0;
	state_lock->lck_dbb = database;
	state_lock->lck_object = reinterpret_cast<blk*>(database);
	state_lock->lck_ast = backup_state_ast;

	alloc_lock = FB_NEW_RPT(*database->dbb_permanent, 0) lck();
	alloc_lock->lck_type = LCK_backup_merge;
	alloc_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, alloc_lock->lck_type);
	alloc_lock->lck_parent = database->dbb_lock;
	alloc_lock->lck_length = 0;
	alloc_lock->lck_dbb = database;
	alloc_lock->lck_object = reinterpret_cast<blk*>(database);
	alloc_lock->lck_ast = alloc_table_ast;
#endif
}

void BackupManager::shutdown_locks() {
#ifndef SUPERSERVER
	TDBB tdbb = GET_THREAD_DATA;
	if (state_lock)
		LCK_release(tdbb, state_lock);
	if (alloc_lock)
		LCK_release(tdbb, alloc_lock);
#endif
}


BackupManager::~BackupManager() {
	if (diff_file)
		PIO_close(diff_file);
	shutdown_locks();
	// Note ! We do not free memory for any allocated objects.
	// It was allocated from database pool and will be freed automatically
#ifdef SUPERSERVER
	delete alloc_lock;
	delete state_lock;
	delete adjust_state_lock;
#endif
}

void BackupManager::set_difference(const char* filename) {
	TDBB tdbb = GET_THREAD_DATA;
	
	if (filename) {
		WIN window;
		HDR header;
		window.win_page = HEADER_PAGE;
		window.win_flags = 0;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK_MUST_WRITE(tdbb, &window);
		PAG_replace_entry_first(header, HDR_difference_file, 
			strlen(filename), (UCHAR*)filename);
		CCH_RELEASE(tdbb, &window);
		strncpy(diff_name, filename, sizeof(diff_name));
	} else {
		PAG_delete_clump_entry(HEADER_PAGE, HDR_difference_file);
		generate_filename();
	}
}

bool BackupManager::lock_state() {
	TDBB tdbb = GET_THREAD_DATA;
	// If we own exlock here no need to do anything else
	if (tdbb->tdbb_flags & TDBB_set_backup_state) return true; 
#ifdef SUPERSERVER
	state_lock->beginRead();
	return true;
#else
	ast_flags |= NBAK_state_in_use; // Prevent ASTs from releasing the lock
	if  (state_lock->lck_physical < LCK_SR &&
		!LCK_lock(tdbb, state_lock, LCK_SR, LCK_WAIT))
	{
		ast_flags &= ~NBAK_state_in_use; // Prevent ASTs from releasing the lock
		return false;
	} else
		return true;
#endif
}

void BackupManager::unlock_state() {
	TDBB tdbb = GET_THREAD_DATA;
	// If we own exlock here no need to do anything else
	if (tdbb->tdbb_flags & TDBB_set_backup_state) return;
#ifdef SUPERSERVER
	state_lock->endRead();
#else
	if (ast_flags & NBAK_state_blocking) {
		LCK_release(tdbb, state_lock);
		ast_flags &= ~NBAK_state_blocking;
		backup_state = nbak_state_unknown; // We know state only as long we lock it
	}
	ast_flags &= ~NBAK_state_in_use;
#endif
}

bool BackupManager::lock_alloc() {
	TDBB tdbb = GET_THREAD_DATA;
#ifdef SUPERSERVER
	alloc_lock->beginRead();
	return true;
#else
	ast_flags |= NBAK_alloc_in_use; // Prevent ASTs from releasing the lock
	if (alloc_lock->lck_physical < LCK_SR &&
		!LCK_lock(tdbb, alloc_lock, LCK_SR, LCK_WAIT))
	{
		ast_flags &= ~NBAK_alloc_in_use;
		return false;
	} else
		return true;
#endif
}

void BackupManager::unlock_alloc() {
	TDBB tdbb = GET_THREAD_DATA;
#ifdef SUPERSERVER
	alloc_lock->endRead();
#else
	if (ast_flags & NBAK_alloc_blocking) {
		LCK_release(tdbb, alloc_lock);
		ast_flags &= ~NBAK_alloc_blocking;
		ast_flags |= NBAK_alloc_dirty;
	}
	ast_flags &= ~NBAK_alloc_in_use;
#endif
}

bool BackupManager::actualize_state(ISC_STATUS* status) {
	if (backup_state == nbak_state_unknown) {
		// State is unknown. We need to read it from the disk.
		// We cannot use CCH for this because of likely recursion.
		TRACE("actualize_state");
		
		// Read original page from database file or shadows.
		SSHORT retryCount = 0;
		char *spare_buffer = FB_NEW(*database->dbb_permanent) char[database->dbb_page_size + MIN_PAGE_SIZE];
		HDR header = (HDR)FB_ALIGN((U_IPTR)spare_buffer, MIN_PAGE_SIZE);
		struct bdb temp_bdb;
		temp_bdb.bdb_page = HEADER_PAGE;
		temp_bdb.bdb_dbb = database;
		temp_bdb.bdb_buffer = reinterpret_cast<PAG>(header);
		FIL file = database->dbb_file;
		while (!PIO_read(file, &temp_bdb, temp_bdb.bdb_buffer, status)) {
			if (!CCH_rollover_to_shadow(database, file, FALSE)) {
				TRACE("Shadow change error");
				delete spare_buffer;
				return false;
			}
			if (file != database->dbb_file)
				file = database->dbb_file;
			else {
				if (retryCount++ == 3) {
					TRACE("IO error");
					delete spare_buffer;
					return false;
				}
			}
		}

		backup_state = header->hdr_flags & hdr_backup_mask;
		TRACE1("backup state read from header is %d", backup_state);
		current_scn = header->hdr_header.pag_scn();
		backup_pages = header->hdr_backup_pages;

		// Read difference file name from header clumplets
		bool fname_found = false;
		UCHAR *p = header->hdr_data;
		while (true) {
			switch(*p) {
			case HDR_backup_guid:
				p += p[1]+2;
				continue;
			case HDR_difference_file:
				fname_found = true;
				memcpy(diff_name, p+2, p[1]);
				diff_name[p[1]] = 0;				
			}
			break;
		}
		if (!fname_found) generate_filename();
		
		if (backup_state == nbak_state_normal) {
			if (diff_file) {
#ifdef SUPERSERVER
				adjust_state_lock->enter();
#endif
				PIO_close(diff_file);
				diff_file = NULL;
#ifdef SUPERSERVER
				adjust_state_lock->leave();
#endif
			}
			// Page allocation table cache is no longer valid.
			if (alloc_table) {
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
		if (backup_state != nbak_state_normal && !diff_file) {
#ifdef SUPERSERVER
			adjust_state_lock->enter();
#endif
			try {
				diff_file = PIO_open(database, diff_name, strlen(diff_name), 0, 
									 NULL, diff_name, strlen(diff_name));
			} catch(const std::exception&) {
				// This routine should not throw exceptions. This is almost correct
				// way to handle errors except that we do not fill passed status vector.
				// It is going to be tdbb->tdbb_status_vector anyway.
				return false;
			}
#ifdef SUPERSERVER
			adjust_state_lock->leave();
#endif
		}
		
	}
	return true;
}

#ifndef SUPERSERVER

int BackupManager::backup_state_ast(void *ast_object)
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
	DBB new_dbb = reinterpret_cast<DBB>(ast_object);
	LCK lock;
	struct tdbb thd_context, *tdbb;


	lock = new_dbb->backup_manager->state_lock;
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

	TRACE("backup_state_ast");
	
	if (new_dbb->backup_manager->ast_flags & NBAK_state_in_use)
		new_dbb->backup_manager->ast_flags |= NBAK_state_blocking;
	else {
		// We know state only as long we lock it
		new_dbb->backup_manager->backup_state = nbak_state_unknown; 
		LCK_release(tdbb, lock);
	}

/* Restore the prior thread context */

	RESTORE_THREAD_DATA;

	ISC_ast_exit();
	return 0;
}

int BackupManager::alloc_table_ast(void *ast_object)
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
	DBB new_dbb = reinterpret_cast<DBB>(ast_object);
	LCK lock;
	struct tdbb thd_context, *tdbb;


	lock = new_dbb->backup_manager->alloc_lock;

	ISC_ast_enter();

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	SET_THREAD_DATA;

	tdbb->tdbb_database = new_dbb;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	
	TRACE("alloc_table_ast");

	if (new_dbb->backup_manager->ast_flags & NBAK_alloc_in_use || lock->lck_physical != LCK_SR)
		new_dbb->backup_manager->ast_flags |= NBAK_alloc_blocking;
	else {
		new_dbb->backup_manager->ast_flags |= NBAK_alloc_dirty;
		LCK_release(tdbb, lock);
	}

/* Restore the prior thread context */

	RESTORE_THREAD_DATA;

	ISC_ast_exit();
	return 0;
}

#endif
