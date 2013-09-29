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
 *
 */
 
#ifndef JRD_NBAK_H
#define JRD_NBAK_H

#include "../common/classes/tree.h"
#include "../common/classes/rwlock.h"
#include "../common/classes/alloc.h"
#include "../common/classes/fb_string.h"
#include "GlobalRWLock.h"

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
class thread_db;
class Database;
class jrd_file;

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

// Class to synchronize access to backup state
class NBackupState: public GlobalRWLock
{
public:
	ULONG flags; 

	NBackupState(thread_db* tdbb, MemoryPool& p, BackupManager *bakMan);
	virtual ~NBackupState() { }

protected:
	BackupManager *backup_manager;
	virtual void blockingAstHandler(thread_db* tdbb);
	virtual void fetch(thread_db* tdbb);
	virtual void invalidate(thread_db* tdbb, bool ast_handler);
};

// Class to synchronize access to diff page allocation table
class NBackupAlloc: public GlobalRWLock
{
public:
	NBackupAlloc(thread_db* tdbb, MemoryPool& p, BackupManager *bakMan);
	virtual ~NBackupAlloc() { }

protected:
	BackupManager *backup_manager;
	virtual void fetch(thread_db* tdbb);
	virtual void invalidate(thread_db* tdbb, bool ast_handler);
};

const UATOM NBAK_state_blocking		= 1;	// Changing the blocking state. All pages must mark by BDB_must_write flag if this flag is set
// Note this flags MUST correspond with backup mask in ods.h
const SATOM nbak_state_normal	= 0x0;     // Normal mode. Changes are simply written to main files
const SATOM nbak_state_stalled	= 0x400;   // 1024 Main files are locked. Changes are written to diff file
const SATOM nbak_state_merge	= 0x800;   // 2048 Merging changes from diff file into main files
const SATOM nbak_state_unknown	= -1;      // State is unknown. Needs to be read from disk 

/*
 *  The functional responsibilities of NBAK are:
 *  1. to redirect writes to difference files when asked (ALTER DATABASE BEGIN
 *  BACKUP statement)
 *  2. to produce a GUID for the database snapshot and write it into the database
 *  header before the ALTER DATABASE BEGIN BACKUP statement returns
 *  3. to merge differences into the database when asked (ALTER DATABASE END BACKUP
 *  statement)
 *  4. to mark pages written by the engine with the current SCN [System Change
 *  Number] counter value for the database
 *  5. to increment SCN on each change of backup state
 *  
 *  The backup state cycle is:
 *  nbak_state_normal -> nbak_state_stalled -> nbak_state_merge -> nbak_state_normal
 *  - In normal state writes go directly to the main database files.
 *  - In stalled state writes go to the difference file only and the main files are
 *  read-only.
 *  - In merge state new pages are not allocated from difference files. Writes go to
 *  the main database files. Reads of mapped pages compare both page versions and
 *  return the version which is fresher, because we don't know if it is merged or not.
 *  
 *  For synchronization NBAK uses 3 lock types via Firebird::GlobalRWLock:
 *  LCK_backup_database, LCK_backup_alloc, LCK_backup_end.
 *  
 *  LCK_backup_database protects "clean" state of database. Database is meant to be
 *  clean when it has no dirty pages. When attachment needs to mark a page as dirty
 *  (via CCH_mark) it needs to obtain READ (LCK_PR) lock of this kind. WRITE
 *  (LCK_EX) lock forces flush of all caches leaving no dirty pages to be written to
 *  database.
 *  
 *  Modification process of a page is as follows:
 *  CCH_fetch -> CCH_mark -> CCH_release -> write_page
 *  
 *  The dirty page is owned by the ATTACHMENT between CCH_mark and CCH_release and
 *  by DATABASE until write_page happens. Each dirty page owns the logical lock on
 *  LCK_backup_database that reflects this cycle.
 *  
 *  Header page is the special case in the above logic and needs to be locked during
 *  CCH_fetch(LCK_EX) phase because it needs to be modified during state transition
 *  and taking the lock later would cause deadlocks.
 *  
 *  AST on LCK_backup_database forces all dirty pages owned by DATABASE to be
 *  written to disk via write_page. Since finalizing processing of the pages owned
 *  by attachment may require taking new locks BDB_must_write is set for them to
 *  ensure that LCK_backup_database lock is released as soon as possible.
 *  
 *  To change backup state, engine takes LCK_backup_database lock first, forcing all
 *  dirty pages to the disk, modifies the header page reflecting the state change,
 *  and releases the lock allowing transaction processing to continue.
 *  
 *  LCK_backup_alloc is used to protect mapping table between difference file
 *  (.delta) and the database. To add new page to the mapping attachment needs to
 *  take WRITE lock of this kind. READ lock is necessary to read the table.
 *  
 *  LCK_backup_end is used to ensure reliable execution of state transition from
 *  nbak_state_merge to nbak_state_normal (MERGE process). Taking of WRITE (LCK_EX)
 *  lock of this kind is needed to perform the MERGE. Every new attachment attempts
 *  to finalize incomplete merge if the database is in nbak_state_merge mode and
 *  this lock is not taken. 
 */

class BackupManager {
public:
	class SharedDatabaseHolder
	{
	public:
		explicit SharedDatabaseHolder(thread_db* atdbb, BackupManager* bm)
			: backupManager(bm), tdbb(atdbb)
		{
			backupManager->lock_shared_database(tdbb, true);
		}
		~SharedDatabaseHolder()
		{
			backupManager->unlock_shared_database(tdbb);
		}
	private:
		// copying is prohibited
		SharedDatabaseHolder(const SharedDatabaseHolder&);
		SharedDatabaseHolder& operator =(const SharedDatabaseHolder&);

		BackupManager* backupManager;
		thread_db* tdbb;
	};

	// Set when db is creating. Default = false
	bool dbCreating;

	// Subsystem initialization
	BackupManager(thread_db* tdbb, Database* _database, int ini_state);

	// Release locks in response to shutdown AST
	void shutdown_locks(thread_db* tdbb);

	// Set difference file name in header. 
	// State must be locked and equal to nbak_state_normal to call this method
	void set_difference(thread_db* tdbb, const char* filename);

	// Return current backup state
	int get_state() {
		return backup_state;
	}
	// Sets current backup state
	void set_state(int new_state) {
		backup_state = new_state;
	}

	// Return current SCN for database
	ULONG get_current_scn() const {
		return current_scn;
	}
	
	// Initialize and open difference file for writing
	void begin_backup(thread_db* tdbb);
	
	// Merge difference file to main files (if needed) and unlink() difference 
	// file then. If merge is already in progress method silently returns false and 
	// does nothing (so it can be used for recovery on database startup). 
	void end_backup(thread_db* tdbb, bool recover);
	
	// Function for force all connections to flush their caches
	// and prevent them from marking new dirty pages
	void lock_clean_database(thread_db* tdbb, SSHORT wait, WIN* window);
	void unlock_clean_database(thread_db* tdbb);

	void lock_shared_database(thread_db* tdbb, SSHORT wait);
	void unlock_shared_database(thread_db* tdbb);

	// Prevent allocation table from modification by other threads/processes
	// You may or may not call unlock function in case this functions fail
	void lock_alloc(thread_db* tdbb, SSHORT wait);
	void lock_alloc_write(thread_db* tdbb, SSHORT wait);

	// Remove our interest in static allocation table
	void unlock_alloc(thread_db* tdbb);
	void unlock_alloc_write(thread_db* tdbb);

	// Return page index in difference file that can be used in 
	// write_difference call later. 
	ULONG get_page_index(thread_db* tdbb, ULONG db_page);

	// Return next page index in the difference file to be allocated
	ULONG allocate_difference_page(thread_db* tdbb, ULONG db_page);
	
	// Must have ISC_STATUS because it is called from write_page
	bool write_difference(ISC_STATUS* status, ULONG diff_page, Ods::pag* page);
	bool read_difference(thread_db* tdbb, ULONG diff_page, Ods::pag* page);
	void flush_difference();
	void setForcedWrites(const bool forceWrite, const bool notUseFSCache);

	void openDelta();
	void closeDelta();
	
	// Routines to declare and release interest in the database file
	void checkout_dirty_page(thread_db* tdbb, SLONG owner_handle);
	void release_dirty_page(thread_db* tdbb, SLONG owner_handle);
	void change_dirty_page_owner(thread_db* tdbb, SLONG from_handle, SLONG to_handle);

	// Returns difference owner handles for locks 
	static SLONG database_lock_handle(thread_db* tdbb) {
		return LCK_get_owner_handle_by_type(tdbb, LCK_OWNER_database);
	}
	static SLONG attachment_lock_handle(thread_db* tdbb) {
		return LCK_get_owner_handle_by_type(tdbb, LCK_OWNER_attachment);
	}

	void shutdown(thread_db* tdbb);

	bool database_flush_in_progress() {
//		NBAK_TRACE(("NBAK_state_blocking=%i", database_lock->flags & NBAK_state_blocking));
		return database_lock->flags & NBAK_state_blocking;
	}

	// Make appropriate information up-to-date
	bool actualize_state(thread_db* tdbb);
	bool actualize_alloc(thread_db* tdbb);
	
	// Get size (in pages) of locked database file
	ULONG getPageCount();

	// Subsystem finalization. Called from shutdown()
	~BackupManager();
private:
	Database* database;
	jrd_file* diff_file;
	AllocItemTree* alloc_table; // Cached allocation table of pages in difference file
	volatile SATOM backup_state;
	ULONG last_allocated_page; // Last physical page allocated in the difference file
	BYTE *temp_buffers_space;
	ULONG *alloc_buffer, *empty_buffer, *spare_buffer;
	ULONG current_scn;
	Firebird::PathName diff_name;
	bool explicit_diff_name;

	// Lock to protect allocation table in this process (BTree)
	NBackupAlloc* alloc_lock;
	NBackupState* database_lock;

	void generate_filename();
};

} //namespace Jrd

#endif /* JRD_NBAK_PROTO_H */
