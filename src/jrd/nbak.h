/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbak.h
 *	DESCRIPTION:	New backup interface definitions
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
 *  $Id: nbak.h,v 1.16 2004-06-30 01:38:57 skidder Exp $
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

namespace Ods {
	struct pag;
}

namespace Jrd {

class Lock;
class Record;

class AllocItem {
public:
	ULONG db_page; // Page number in the main database file
	ULONG diff_page; // Page number in the difference file
	Record* rec_data;
    static const ULONG& generate(const void *sender, const AllocItem& item) {
		return item.db_page;
    }
	AllocItem() {
	}
	AllocItem(ULONG db_pageL, ULONG diff_pageL) {
		this->db_page = db_pageL;
		this->diff_page = diff_pageL;
	}
};

typedef Firebird::BePlusTree<AllocItem, ULONG, MemoryPool, AllocItem> AllocItemTree;


class BackupManager {
public:
	// Subsystem initialization
	BackupManager(class Database* _database, int ini_state);
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
	bool write_difference(ISC_STATUS* status, ULONG diff_page, Ods::pag* page) throw();
	
	bool read_difference(ULONG diff_page, Ods::pag* page) throw();
	
	
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
	class Database* database;
	class jrd_file* diff_file;
	AllocItemTree* alloc_table; // Cached allocation table of pages in difference file
	volatile SATOM backup_state;
	ULONG last_allocated_page; // Last physical page allocated in the difference file
	BYTE *temp_buffers_space;
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
	Firebird::Mutex* adjust_state_lock; 
#else
	Lock* alloc_lock; // Lock to protect allocation table
	Lock* state_lock; // Lock to protect backup state
	Lock* database_lock; // Lock to protect writing to database

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
const UATOM NBAK_state_in_use = 1;
const UATOM NBAK_alloc_in_use = 2;

// Flags manipulated at AST level
const UATOM NBAK_state_blocking		= 1;
const UATOM NBAK_alloc_blocking		= 2;
const UATOM NBAK_database_blocking	= 4;
const UATOM NBAK_alloc_dirty		= 8;

// Note this flags MUST correspond with backup mask in ods.h
const SATOM nbak_state_normal	= 0x0;     // Normal mode. Changes are simply written to main files
const SATOM nbak_state_stalled	= 0x400;   // Main files are locked. Changes are written to diff file
const SATOM nbak_state_merge	= 0x800;   // Merging changes from diff file into main files
const SATOM nbak_state_unknown	= -1;      // State is unknown. Needs to be read from disk

} //namespace Jrd

#endif /* JRD_NBAK_PROTO_H */
