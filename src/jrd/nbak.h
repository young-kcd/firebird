/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nbak.h
 *	DESCRIPTION:	New backup interface definitions
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
 */

#ifndef _JRD_NBAK_H_
#define _JRD_NBAK_H_

#include "../common/classes/tree.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/alloc.h"

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
	void shutdown_locks();
	// Subsystem finalization
	~BackupManager();
	// Set difference file name in header. 
	// State must be locked and equal to nbak_state_normal to call this method
	void set_difference(const char* filename);
	// Prevent backup state from modification by others
	// You may or may not call unlock_state in case this function fails
	bool lock_state(bool thread_exit);
	// Remove our interest in consistent backup state
	void unlock_state();
	// Return current backup state
	int get_state() const {
		return backup_state;
	}
	// Return current SCN for database
	ULONG get_current_scn() const {
		return current_scn;
	}
	
	// Return the amount of pages in locked database files
	SLONG get_backup_pages() const {
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
	bool lock_alloc(bool thread_exit);
	bool lock_alloc_write(bool thread_exit);
	// Remove our interest in static allocation table
	void unlock_alloc();
	void unlock_alloc_write();
	// Return page index in difference file that can be used in 
	// write_difference call later. 
	ULONG get_page_index(ULONG db_page) const;
	// Return next page index in the difference file to be allocated
	ULONG get_next_page() const {
		return last_allocated_page+1;
	}
	// Mark next difference page as used by some database page
	bool mark_alloc(ISC_STATUS* status, ULONG db_page);
	
	bool write_difference(ISC_STATUS* status, ULONG diff_page, struct pag* page);
	
	bool read_difference(ISC_STATUS* status, ULONG diff_page, struct pag* page);
	
	bool actualize_state(ISC_STATUS* status);

	// Ensure that cached allocation table contains the same information as database
	// Return false in case of error
	bool actualize_alloc(ISC_STATUS* status);
private:
	class dbb* database;
	class fil* diff_file;
	AllocItemTree* alloc_table; // Cached allocation table of pages in difference file
	int backup_state;	
	ULONG last_allocated_page; // Last physical page allocated in the difference file
	ULONG *alloc_buffer;
	ULONG current_scn;
	ULONG alloc_scn; // SCN of cached allocation table
	SLONG backup_pages; // Number of allocated pages as it was at point of backup lock
	char diff_name[MAXPATHLEN];
#ifdef SUPERSERVER
	Firebird::RWLock* alloc_lock; // Lock to protect allocation table
	Firebird::RWLock* state_lock; // Lock to protect backup lock	
	// Lock to protect initialization of alloc_table and diff_file 
	// outside of exclusive status lock to prevent possible race condition
	// in case of errors.
	Firebird::Spinlock* adjust_state_lock; 
#else
	class lck* alloc_lock; // Lock to protect allocation table
	class lck* state_lock; // Lock to protect backup lock	
	ULONG ast_flags;
	static int backup_state_ast(void *ast_object);
	static int alloc_table_ast(void *ast_object);
#endif
	bool try_lock_state_write();
	void lock_state_write(bool thread_exit);
	void unlock_state_write();
	void generate_filename();
};

/* Bit values for backup subsystem flags */
#define NBAK_state_blocking 1
#define NBAK_state_in_use 2
#define NBAK_alloc_blocking 4
#define NBAK_alloc_in_use 8
#define NBAK_alloc_dirty 16

/* Node this flags MUST correspond with backup mask in ods.h */
#define nbak_state_normal   0x0     /* Normal mode. Changes are simply written to main files */
#define nbak_state_stalled  0x400   /* Main files are locked. Changes are written to diff file */
#define nbak_state_merge    0x800   /* Merging changes from diff file into main files */
#define nbak_state_unknown  -1      /* State is unknown. Needs to be read from disk */

#endif /* _JRD_NBAK_PROTO_H_ */
