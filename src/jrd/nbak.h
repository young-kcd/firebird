/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbak.h
 *	DESCRIPTION:	New backup interface definitions
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: nbak.h,v 1.6 2003-10-03 01:47:25 brodsom Exp $
 *
 */
 
#ifndef JRD_NBAK_H
#define JRD_NBAK_H

#include "../common/classes/tree.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/alloc.h"

// Uncomment this line if you need to trace backup-related activity
//#define NBAK_DEBUG

#ifdef NBAK_DEBUG
DEFINE_TRACE_ROUTINE(nbak_trace);
#define NBAK_TRACE(args) nbak_trace args
#define NBAK_TRACE_AST(message) gds__trace(message)
#else
#define NBAK_TRACE(args) /* nothing */
#define NBAK_TRACE_AST(message) /* nothing */
#endif

class AllocItem {
public:
	ULONG db_page; // Page number in the main database file
	ULONG diff_page; // Page number in the difference file
	class rec* rec_data;
    static const ULONG& generate(void *sender, const AllocItem& item) {
		return item.db_page;
    }
	AllocItem() {
	}
	AllocItem(ULONG db_page, ULONG diff_page) {
		this->db_page = db_page;
		this->diff_page = diff_page;
	}
};

typedef Firebird::BePlusTree<AllocItem, ULONG, MemoryPool, AllocItem> AllocItemTree;


class BackupManager {
public:
	// Subsystem initialization
	BackupManager(class dbb* _database, int ini_state);
	// Release locks in response to shutdown AST
	void shutdown_locks() throw();
	// Subsystem finalization
	~BackupManager();
	// Set difference file name in header. 
	// State must be locked and equal to nbak_state_normal to call this method
	void set_difference(const char* filename);
	// Prevent backup state from modification by others
	// You may or may not call unlock_state in case this function fails
	bool lock_state(bool thread_exit) throw();
	// Remove our interest in consistent backup state
	void unlock_state() throw();
	// Return current backup state
	int get_state() const throw() {
		return backup_state;
	}
	// Return current SCN for database
	ULONG get_current_scn() const throw() {
		return current_scn;
	}
	
	// Return the amount of pages in locked database files
	SLONG get_backup_pages() const throw() {
		return backup_pages;
	}
	
	// Initialize and open difference file for writing
	void begin_backup();
	
	// Merge difference file to main files (if needed) and unlink() difference 
	// file then. If merge is already in progress method silently returns false and 
	// does nothing (so it can be used for recovery on database startup). 
	void end_backup(bool recover);
	
	// Prevent allocation table from modification by other threads/processes
	// You may or may not call unlock function in case this functions fail
	bool lock_alloc(bool thread_exit) throw();
	bool lock_alloc_write(bool thread_exit) throw();
	// Remove our interest in static allocation table
	void unlock_alloc() throw();
	void unlock_alloc_write() throw();
	// Return page index in difference file that can be used in 
	// write_difference call later. 
	ULONG get_page_index(ULONG db_page) const throw();
	// Return next page index in the difference file to be allocated
	ULONG allocate_difference_page(ULONG db_page) throw();
	
	// Must have ISC_STATUS because it is called from write_page
	bool write_difference(ISC_STATUS* status, ULONG diff_page, struct pag* page) throw();
	
	bool read_difference(ULONG diff_page, struct pag* page) throw();
	
	
	// Routines to declare and release interest in the main database file
	bool get_sw_database_lock(bool enable_signals) throw();
	void release_sw_database_lock() throw();

#ifndef SUPERSERVER
	// Routines to declare and release deferred interest in the difference file
	void increment_diff_use_count() throw();
	void decrement_diff_use_count() throw();
	
	SLONG get_current_generation() const throw() {
		return diff_generation;
	}
#endif
private:
	class dbb* database;
	class fil* diff_file;
	AllocItemTree* alloc_table; // Cached allocation table of pages in difference file
	volatile SATOM backup_state;
	ULONG last_allocated_page; // Last physical page allocated in the difference file
	ULONG *alloc_buffer, *empty_buffer, *spare_buffer;
	ULONG current_scn;
	SLONG backup_pages; // Number of allocated pages as it was at point of backup lock
	char diff_name[MAXPATHLEN];
	// Set if we need to close difference file in the next status update cycle
	// Used in CS builds to prevent closing difference file too early while it still
	// may be used inside the signal handlers
	bool diff_pending_close; 
#ifdef SUPERSERVER
	Firebird::RWLock* alloc_lock; // Lock to protect allocation table
	Firebird::RWLock* state_lock; // Lock to protect backup lock	
	Firebird::RWLock* database_lock; // Lock to protect write to database
	// Lock to protect initialization of alloc_table and diff_file 
	// outside of exclusive status lock to prevent possible race condition
	// in case of errors.
	Firebird::Spinlock* adjust_state_lock; 
#else
	class lck* alloc_lock; // Lock to protect allocation table
	class lck* state_lock; // Lock to protect backup state
	class lck* database_lock; // Lock to protect writing to database

	// Absense of 'volatile' REALLY causes database corruption
	// with optimized build on Linux CS. Do not remove it !
	volatile UATOM flags;
	// Variables used at AST level
	volatile UATOM ast_flags;
	volatile SATOM database_use_count;
	volatile SATOM diff_use_count;
	volatile SATOM diff_generation;
	
	static int backup_state_ast(void *ast_object) throw();
	static int alloc_table_ast(void *ast_object) throw();
	static int backup_database_ast(void *ast_object) throw();
#endif
	bool try_lock_state_write();
	void lock_state_write(bool thread_exit);
	void unlock_state_write() throw();
	void generate_filename() throw();
	// Make appropriate information up-to-date
	bool actualize_state() throw();
	bool actualize_alloc() throw();
};

// Flags manipulated normally
#define NBAK_state_in_use 1
#define NBAK_alloc_in_use 2

// Flags manipulated at AST level
#define NBAK_state_blocking 1
#define NBAK_alloc_blocking 2
#define NBAK_database_blocking 4
#define NBAK_alloc_dirty  8

// Note this flags MUST correspond with backup mask in ods.h
#define nbak_state_normal   0x0     // Normal mode. Changes are simply written to main files
#define nbak_state_stalled  0x400   // Main files are locked. Changes are written to diff file
#define nbak_state_merge    0x800   // Merging changes from diff file into main files
#define nbak_state_unknown  -1      // State is unknown. Needs to be read from disk

#endif /* JRD_NBAK_PROTO_H */
