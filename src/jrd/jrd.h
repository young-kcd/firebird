/*
 *      PROGRAM:        JRD access method
 *      MODULE:         jrd.h
 *      DESCRIPTION:    Common descriptions
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
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef JRD_JRD_H
#define JRD_JRD_H

#include "../jrd/gdsassert.h"
#include "../jrd/common.h"
#include "../jrd/dsc.h"
#include "../jrd/all.h"
#include "../jrd/nbak.h"
#include "../jrd/btn.h"
#include "../jrd/all.h"
#include "../jrd/jrd_proto.h"
#if defined(UNIX) && defined(SUPERSERVER)
#include <setjmp.h>
#endif

#include "../include/fb_vector.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/stack.h"
#include "../common/classes/timestamp.h"
#include "../common/classes/GenericMap.h"
#include "../jrd/sbm.h"

#ifdef DEV_BUILD
#define DEBUG                   if (debug) DBG_supervisor(debug);
#define VIO_DEBUG				/* remove this for production build */
#else /* PROD */
#define DEBUG
#undef VIO_DEBUG
#endif

#define BUGCHECK(number)        ERR_bugcheck (number, __FILE__, __LINE__)
#define CORRUPT(number)         ERR_corrupt (number)
#define IBERROR(number)         ERR_error (number)


#define BLKCHK(blk, type)       if (MemoryPool::blk_type(blk) != (USHORT) (type)) BUGCHECK (147)

/* DEV_BLKCHK is used for internal consistency checking - where
 * the performance hit in production build isn't desired.
 * (eg: scatter this everywhere)
 *
 * This causes me a problem DEV_BLKCHK fails when the data seems valid
 * After talking to John this could be because the memory is from the local
 * stack rather than the heap.  However I found to continue I needed to 
 * turn it off by dfining the macro to be empty.  But In thinking about
 * it I think that it would be more helful for a mode where these extra 
 * DEV checks just gave warnings rather than being fatal.
 * MOD 29-July-2002
 *
 */
#ifdef DEV_BUILD
#define DEV_BLKCHK(blk,type)
//#define DEV_BLKCHK(blk, type)    if (blk) {BLKCHK (blk, type);}
#else
#define DEV_BLKCHK(blk, type)	/* nothing */
#endif


// Thread data block / IPC related data blocks
#include "../jrd/thd.h"
#include "../jrd/isc.h"

// Definition of block types for data allocation in JRD
#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

// Definition of DatabasePlugins
#include "../jrd/flu.h"

class CharSetContainer;
struct dsc;
struct thread;
struct mod;

namespace Jrd {

// The database block, the topmost block in the metadata
// cache for a database

const int HASH_SIZE		= 101;


// fwd. decl.
class vec;
class thread_db;
class Attachment;
class jrd_tra;
class jrd_req;
class Lock;
class jrd_file;
class Format;
class jrd_nod;
class BufferControl;
class SparseBitmap;
class BlockingThread;
class jrd_rel;
class ExternalFile;
class ViewContext;
class IndexBlock;
class IndexLock;
class Bookmark;
class ArrayField;
class BlobFilter;
class PageControl;
class Symbol;
class UserId;
struct sort_context;
class TxPageCache;
class RecordSelExpr;
class SecurityClass;
class vcl;
class Shadow;
class TextType;

class Database : private pool_alloc<type_dbb>
{
public:
	typedef int (*crypt_routine) (const char*, void*, int, void*);

	static Database* newDbb(MemoryPool& p) {
		return FB_NEW(p) Database(p);
	}
	
	// The deleteDbb function MUST be used to delete a Database object.
	// The function hides some tricky order of operations.  Since the
	// memory for the vectors in the Database is allocated out of the Database's
	// permanent memory pool, the entire delete() operation needs
	// to complete _before_ the permanent pool is deleted, or else
	// risk an aborted engine.
	static void deleteDbb(Database* toDelete)
	{
		if (toDelete == 0)
			return;
		JrdMemoryPool *perm = toDelete->dbb_permanent;
#ifdef SUPERSERVER
		// Memory pool destruction below decrements memory statistics for 
		// SuperServer situated in database block we are about to deallocate
		// right now.
		Firebird::MemoryStats temp_stats;
		perm->setStatsGroup(temp_stats);
#endif
		delete toDelete;
		JrdMemoryPool::noDbbDeletePool(perm);
	}
	
	Database*	dbb_next;		/* Next database block in system */
	Attachment* dbb_attachments;	/* Active attachments */
	BufferControl*	dbb_bcb;		/* Buffer control block */
	vec*		dbb_relations;	/* relation vector */
	vec*		dbb_procedures;	/* scanned procedures */
	Lock* 		dbb_lock;		/* granddaddy lock */
	jrd_tra*	dbb_sys_trans;	/* system transaction */
	jrd_file*	dbb_file;		/* files for I/O operations */
	Shadow*		dbb_shadow;		/* shadow control block */
	Lock*		dbb_shadow_lock;	/* lock for synchronizing addition of shadows */
	SLONG dbb_shadow_sync_count;	/* to synchronize changes to shadows */
	Lock*		dbb_retaining_lock;	/* lock for preserving commit retaining snapshot */
	PageControl*	dbb_pcontrol;	/* page control */
	vcl*		dbb_t_pages;	/* pages number for transactions */
	vcl*		dbb_gen_id_pages;	/* known pages for gen_id */
	BlobFilter*	dbb_blob_filters;	/* known blob filters */

	DatabaseModules	modules;	/* external function/filter modules */
	MUTX_T *dbb_mutexes;		/* Database block mutexes */
	WLCK_T *dbb_rw_locks;		/* Database block read/write locks */
	REC_MUTX_T dbb_sp_rec_mutex;	/* Recursive mutex for accessing/updating stored procedure metadata */
	SLONG dbb_sort_size;		/* Size of sort space per sort */

	UATOM dbb_ast_flags;		/* flags modified at AST level */
	ULONG dbb_flags;
	USHORT dbb_ods_version;		/* major ODS version number */
	USHORT dbb_minor_version;	/* minor ODS version number */
	USHORT dbb_minor_original;	/* minor ODS version at creation */
	USHORT dbb_page_size;		/* page size */
	USHORT dbb_dp_per_pp;		/* data pages per pointer page */
	USHORT dbb_max_records;		/* max record per data page */
	USHORT dbb_max_idx;			/* max number of indexes on a root page */
	USHORT dbb_use_count;		/* active count of threads */
	USHORT dbb_shutdown_delay;	/* seconds until forced shutdown */
	USHORT dbb_refresh_ranges;	/* active count of refresh ranges */
	USHORT dbb_prefetch_sequence;	/* sequence to pace frequency of prefetch requests */
	USHORT dbb_prefetch_pages;	/* prefetch pages per request */
	Firebird::string dbb_spare_string;	/* random buffer */
	Firebird::PathName dbb_filename;	/* filename string */
	Firebird::PathName dbb_database_name;	/* database ID (file name or alias) */
	Firebird::string dbb_encrypt_key;	/* encryption key */

	JrdMemoryPool* dbb_permanent;
	JrdMemoryPool* dbb_bufferpool;

	typedef JrdMemoryPool* pool_ptr;
	typedef Firebird::vector<pool_ptr> pool_vec_type;

	pool_vec_type dbb_pools;		/* pools */
    USHORT dbb_next_pool_id;
	vec*		dbb_internal;	/* internal requests */
	vec*		dbb_dyn_req;	/* internal dyn requests */

	SLONG dbb_oldest_active;	/* Cached "oldest active" transaction */
	SLONG dbb_oldest_transaction;	/* Cached "oldest interesting" transaction */
	SLONG dbb_oldest_snapshot;	/* Cached "oldest snapshot" of all active transactions */
	SLONG dbb_next_transaction;	/* Next transaction id used by NETWARE */
	SLONG dbb_attachment_id;	/* Next attachment id for ReadOnly DB's */
	SLONG dbb_page_incarnation;	/* Cache page incarnation counter */
	ULONG dbb_page_buffers;		/* Page buffers from header page */

	event_t dbb_writer_event[1];	/* Event to wake up cache writer */
	event_t dbb_writer_event_init[1];	/* Event for initialization cache writer */
	event_t dbb_writer_event_fini[1];	/* Event for finalization cache writer */
	event_t dbb_reader_event[1];	/* Event to wake up cache reader */
#ifdef GARBAGE_THREAD
	event_t dbb_gc_event[1];	/* Event to wake up garbage collector */
	event_t dbb_gc_event_init[1];	/* Event for initialization garbage collector */
	event_t dbb_gc_event_fini[1];	/* Event for finalization garbage collector */
#endif
	Attachment* dbb_update_attachment;	/* Attachment with update in process */
	BlockingThread*	dbb_update_que;	/* Attachments waiting for update */
	BlockingThread*	dbb_free_btbs;	/* Unused BlockingThread blocks */

	Firebird::MemoryStats dbb_memory_stats;
	
	SLONG dbb_reads;
	SLONG dbb_writes;
	SLONG dbb_fetches;
	SLONG dbb_marks;
	SLONG dbb_last_header_write;	/* Transaction id of last header page physical write */
	SLONG dbb_flush_cycle;		/* Current flush cycle */
	SLONG dbb_sweep_interval;	/* Transactions between sweep */
	SLONG dbb_lock_owner_handle;	/* Handle for the lock manager */

	USHORT unflushed_writes;	/* unflushed writes */
	time_t last_flushed_write;	/* last flushed write time */

	crypt_routine dbb_encrypt;		/* External encryption routine */
	crypt_routine dbb_decrypt;		/* External decryption routine */

	class blb_map *dbb_blob_map;	/* mapping of blobs for REPLAY */
	struct log *dbb_log;		/* log file for REPLAY */
	Firebird::vector<CharSetContainer*>		dbb_charsets;	/* intl character set descriptions */
//	struct wal *dbb_wal;		/* WAL handle for WAL API */
	TxPageCache*	dbb_tip_cache;	/* cache of latest known state of all transactions in system */
	vcl*		dbb_pc_transactions;	/* active precommitted transactions */
	class BackupManager *backup_manager; /* physical backup manager */
	Symbol*	dbb_hash_table[HASH_SIZE];	/* keep this at the end */

private:
	explicit Database(MemoryPool& p)
	:	modules(p),
		dbb_spare_string(p),
		dbb_filename(p),
		dbb_database_name(p),
		dbb_encrypt_key(p),
		dbb_pools(1, p, type_dbb),
		dbb_charsets(p)
	{
	}

	~Database()
	{
		destroyIntlObjects();

		pool_vec_type::iterator itr = dbb_pools.begin();
		while (itr != dbb_pools.end())
		{
			if (*itr && *itr == dbb_bufferpool)
				dbb_bufferpool = 0;
			if (*itr && *itr != dbb_permanent)
				JrdMemoryPool::deletePool(*itr);
			else
				itr++;
		}
		if (dbb_bufferpool)
			JrdMemoryPool::deletePool(dbb_bufferpool);
	}

	void destroyIntlObjects();	// defined in intl.cpp

	// The delete operators are no-oped because the Database memory is allocated from the
	// Database's own permanent pool.  That pool has already been released by the Database
	// destructor, so the memory has already been released.  Hence the operator
	// delete no-op.
	void operator delete(void *mem) {}
	void operator delete[](void *mem) {}

	Database(const Database&);	// no impl.
	const Database& operator =(const Database&) { return *this; }
};

//
// bit values for dbb_flags
//
const ULONG DBB_no_garbage_collect	= 0x1L;
const ULONG DBB_damaged				= 0x2L;
const ULONG DBB_exclusive			= 0x4L;	/* Database is accessed in exclusive mode */
const ULONG DBB_bugcheck			= 0x8L;	/* Bugcheck has occurred */
#ifdef GARBAGE_THREAD
const ULONG DBB_garbage_collector	= 0x10L;	/* garbage collector thread exists */
const ULONG DBB_gc_active			= 0x20L;	/* ... and is actively working. */
const ULONG DBB_gc_pending			= 0x40L;	/* garbage collection requested */
#endif
const ULONG DBB_force_write			= 0x80L;	/* Database is forced write */
const ULONG DBB_no_reserve			= 0x100L;	/* No reserve space for versions */
//const ULONG DBB_add_log				= 0x200L;	// write ahead log has been added
//const ULONG DBB_delete_log			= 0x400L;	// write ahead log has been deleted
//const ULONG DBB_cache_manager		= 0x800L;	// Shared cache manager
const ULONG DBB_DB_SQL_dialect_3	= 0x1000L;	/* database SQL dialect 3 */
const ULONG DBB_read_only			= 0x2000L;	/* DB is ReadOnly (RO). If not set, DB is RW */
const ULONG DBB_being_opened_read_only	= 0x4000L;	/* DB is being opened RO. If unset, opened as RW */
const ULONG DBB_not_in_use			= 0x8000L;	/* Database to be ignored while attaching */
const ULONG DBB_lck_init_done		= 0x10000L;	/* LCK_init() called for the database */
const ULONG DBB_sp_rec_mutex_init	= 0x20000L;	/* Stored procedure mutex initialized */
const ULONG DBB_sweep_in_progress	= 0x40000L;	/* A database sweep operation is in progress */
const ULONG DBB_security_db			= 0x80000L;	/* ISC security database */
const ULONG DBB_sweep_thread_started	= 0x100000L;	/* A database sweep thread has been started */
const ULONG DBB_suspend_bgio		= 0x200000L;	/* Suspend I/O by background threads */
const ULONG DBB_being_opened		= 0x400000L;	/* database is being attached to */
const ULONG DBB_gc_cooperative		= 0x0800000L;	/* cooperative garbage collection */
const ULONG DBB_gc_background		= 0x1000000L;	/* background garbage collection by gc_thread */
//
// dbb_ast_flags
//
const UATOM DBB_blocking			= 0x1L;		// Exclusive mode is blocking
const UATOM DBB_get_shadows			= 0x2L;		// Signal received to check for new shadows
const UATOM DBB_assert_locks		= 0x4L;		// Locks are to be asserted
const UATOM DBB_shutdown			= 0x8L;		// Database is shutdown
const UATOM DBB_shut_attach			= 0x10L;	// no new attachments accepted
const UATOM DBB_shut_tran			= 0x20L;	// no new transactions accepted
const UATOM DBB_shut_force			= 0x40L;	// forced shutdown in progress
const UATOM DBB_shutdown_locks		= 0x80L;	// Database locks release by shutdown
const UATOM DBB_shutdown_full		= 0x100L;	// Database fully shut down
const UATOM DBB_shutdown_single		= 0x200L;	// Database is in single-user maintenance mode

//
// Database attachments
//
const int DBB_read_seq_count		= 0;
const int DBB_read_idx_count		= 1;
const int DBB_update_count			= 2;
const int DBB_insert_count			= 3;
const int DBB_delete_count			= 4;
const int DBB_backout_count			= 5;
const int DBB_purge_count			= 6;
const int DBB_expunge_count			= 7;
const int DBB_max_count				= 8;

//
// Database mutexes and read/write locks
//
const int DBB_MUTX_init_fini		= 0;	// During startup and shutdown
const int DBB_MUTX_statistics		= 1;	// Memory size and counts
const int DBB_MUTX_replay			= 2;	// Replay logging
const int DBB_MUTX_dyn				= 3;	// Dynamic ddl
const int DBB_MUTX_cache			= 4;	// Process-private cache management
const int DBB_MUTX_clone			= 5;	// Request cloning
const int DBB_MUTX_cmp_clone		= 6;	// Compiled request cloning
const int DBB_MUTX_flush_count		= 7;	// flush count/time
const int DBB_MUTX_max				= 8;
const int DBB_WLCK_pools			= 0;	// Pool manipulation
const int DBB_WLCK_files			= 1;	// DB and shadow file manipulation
const int DBB_WLCK_max				= 2;

//
// Flags to indicate normal internal requests vs. dyn internal requests
//
const int IRQ_REQUESTS				= 1;
const int DYN_REQUESTS				= 2;


//
// Errors during validation - will be returned on info calls
// CVC: It seems they will be better in a header for val.cpp that's not val.h
//
const int VAL_PAG_WRONG_TYPE			= 0;
const int VAL_PAG_CHECKSUM_ERR			= 1;
const int VAL_PAG_DOUBLE_ALLOC			= 2;
const int VAL_PAG_IN_USE				= 3;
const int VAL_PAG_ORPHAN				= 4;
const int VAL_BLOB_INCONSISTENT			= 5;
const int VAL_BLOB_CORRUPT				= 6;
const int VAL_BLOB_TRUNCATED			= 7;
const int VAL_REC_CHAIN_BROKEN			= 8;
const int VAL_DATA_PAGE_CONFUSED		= 9;
const int VAL_DATA_PAGE_LINE_ERR		= 10;
const int VAL_INDEX_PAGE_CORRUPT		= 11;
const int VAL_P_PAGE_LOST				= 12;
const int VAL_P_PAGE_INCONSISTENT		= 13;
const int VAL_REC_DAMAGED				= 14;
const int VAL_REC_BAD_TID				= 15;
const int VAL_REC_FRAGMENT_CORRUPT		= 16;
const int VAL_REC_WRONG_LENGTH			= 17;
const int VAL_INDEX_ROOT_MISSING		= 18;
const int VAL_TIP_LOST					= 19;
const int VAL_TIP_LOST_SEQUENCE			= 20;
const int VAL_TIP_CONFUSED				= 21;
const int VAL_REL_CHAIN_ORPHANS			= 22;
const int VAL_INDEX_MISSING_ROWS		= 23;
const int VAL_INDEX_ORPHAN_CHILD		= 24;
const int VAL_MAX_ERROR					= 25;


//
// the attachment block; one is created for each attachment to a database
//
class Attachment : public pool_alloc<type_att>
{
public:
	explicit Attachment(Database* dbb) :
		att_database(dbb), 
		att_lc_messages(*dbb->dbb_permanent),
		att_working_directory(*dbb->dbb_permanent), 
		att_filename(*dbb->dbb_permanent),
		att_context_vars(*dbb->dbb_permanent),
		att_network_protocol(*dbb->dbb_permanent),
		att_remote_address(*dbb->dbb_permanent) { }
/*	Attachment()
	:	att_database(0),
		att_next(0),
		att_blocking(0),
		att_user(0),
		att_transactions(0),
		att_dbkey_trans(0),
		att_requests(0),
		att_active_sorts(0),
		att_id_lock(0),
		att_attachment_id(0),
		att_lock_owner_handle(0),
		att_event_session(0),
		att_security_class(0),
		att_security_classes(0),
		att_relation_locks(0),
		att_bookmarks(0),
		att_record_locks(0),
		att_bkm_quick_ref(0),
		att_lck_quick_ref(0),
		att_flags(0),
		att_charset(0),
		att_lc_messages(0),
		att_long_locks(0),
		att_compatibility_table(0),
		att_val_errors(0),
		att_working_directory(0)
	{
		att_counts[0] = 0;
	}*/

	Database*	att_database;		// Parent databasea block
	Attachment*	att_next;			// Next attachment to database
	Attachment*	att_blocking;		// Blocking attachment, if any
	UserId*		att_user;			// User identification
	jrd_tra*	att_transactions;	// Transactions belonging to attachment
	jrd_tra*	att_dbkey_trans;	// transaction to control db-key scope
	jrd_req*	att_requests;		// Requests belonging to attachment
	sort_context*	att_active_sorts;	// Active sorts
	Lock*		att_id_lock;		// Attachment lock (if any)
	SLONG		att_attachment_id;	// Attachment ID
	SLONG		att_lock_owner_handle;	// Handle for the lock manager
	SLONG		att_event_session;	// Event session id, if any
	SecurityClass*	att_security_class;	// security class for database
	SecurityClass*	att_security_classes;	// security classes
	vcl*		att_counts[DBB_max_count];
	vec*		att_relation_locks;	// explicit persistent locks for relations
	Bookmark*	att_bookmarks;		// list of bookmarks taken out using this attachment
	Lock*		att_record_locks;	// explicit or implicit record locks taken out during attachment
	vec*		att_bkm_quick_ref;	// correspondence table of bookmarks
	vec*		att_lck_quick_ref;	// correspondence table of locks
	ULONG		att_flags;			// Flags describing the state of the attachment
	SSHORT		att_charset;		// user's charset specified in dpb
	Firebird::PathName	att_lc_messages;	// attachment's preference for message natural language
	Lock*		att_long_locks;		// outstanding two phased locks
	vec*		att_compatibility_table;	// hash table of compatible locks
	vcl*		att_val_errors;
	Firebird::PathName	att_working_directory;	// Current working directory is cached
	Firebird::PathName	att_filename;			// alias used to attach the database
	Firebird::TimeStamp	att_timestamp;	// connection date and time
	Firebird::StringMap att_context_vars;  // Context variables for the connection
	Firebird::string att_network_protocol; // Network protocol used by client for connection
	Firebird::string att_remote_address; // Protocol-specific addess of remote client
};


/* Attachment flags */

const ULONG ATT_no_cleanup			= 1;	// Don't expunge, purge, or garbage collect
const ULONG ATT_shutdown			= 2;	// attachment has been shutdown
const ULONG ATT_shutdown_notify		= 4;	// attachment has notified client of shutdown
const ULONG ATT_shutdown_manager	= 8;	// attachment requesting shutdown
const ULONG ATT_lck_init_done		= 16;	// LCK_init() called for the attachment
const ULONG ATT_exclusive			= 32;	// attachment wants exclusive database access
const ULONG ATT_attach_pending		= 64;	// Indicate attachment is only pending
const ULONG ATT_exclusive_pending	= 128;	// Indicate exclusive attachment pending
const ULONG ATT_gbak_attachment		= 256;	// Indicate GBAK attachment
const ULONG ATT_security_db			= 512;	// Indicates an implicit attachment to the security db
#ifdef GARBAGE_THREAD
const ULONG ATT_notify_gc			= 1024;	// Notify garbage collector to expunge, purge ..
const ULONG ATT_disable_notify_gc	= 2048;	// Temporarily perform own garbage collection
const ULONG ATT_garbage_collector	= 4096;	// I'm a garbage collector

const ULONG ATT_NO_CLEANUP			= (ATT_no_cleanup | ATT_notify_gc);
#else
const ULONG ATT_NO_CLEANUP			= ATT_no_cleanup;
#endif

#ifdef CANCEL_OPERATION
const ULONG ATT_cancel_raise		= 8192;		// Cancel currently running operation
const ULONG ATT_cancel_disable		= 16384;	// Disable cancel operations
#endif

const ULONG ATT_gfix_attachment		= 32768;	// Indicate a GFIX attachment
const ULONG ATT_gstat_attachment	= 65536;	// Indicate a GSTAT attachment


/* Procedure block */

class jrd_prc : public pool_alloc<type_prc>
{
    public:
	USHORT prc_id;
	USHORT prc_flags;
	USHORT prc_inputs;
	USHORT prc_defaults;
	USHORT prc_outputs;
	jrd_nod*	prc_input_msg;
	jrd_nod*	prc_output_msg;
	Format*		prc_input_fmt;
	Format*		prc_output_fmt;
	Format*		prc_format;
	vec*		prc_input_fields;	/* vector of field blocks */
	vec*		prc_output_fields;	/* vector of field blocks */
	jrd_req*	prc_request;	/* compiled procedure request */
	USHORT prc_use_count;		/* requests compiled with procedure */
	SSHORT prc_int_use_count;	/* number of procedures compiled with procedure, set and 
	                               used internally in the MET_clear_cache procedure 
								   no code should rely on value of this field 
								   (it will usually be 0)
								*/
	Lock* prc_existence_lock;	/* existence lock, if any */
	Firebird::MetaName prc_security_name;	/* security class name for procedure */
	Firebird::MetaName prc_name;	/* ascic name */
	USHORT prc_alter_count;		/* No. of times the procedure was altered */

	public:
	explicit jrd_prc(MemoryPool& p) : prc_security_name(p), prc_name(p) {}
};

// prc_flags

const USHORT PRC_scanned			= 1;	/* Field expressions scanned */
const USHORT PRC_system				= 2;
const USHORT PRC_obsolete			= 4;	/* Procedure known gonzo */
const USHORT PRC_being_scanned		= 8;	/* New procedure needs dependencies during scan */
const USHORT PRC_blocking			= 16;	/* Blocking someone from dropping procedure */
const USHORT PRC_create				= 32;	/* Newly created */
const USHORT PRC_being_altered		= 64;	/* Procedure is getting altered */
									// This flag is used to make sure that MET_remove_procedure
									// does not delete and remove procedure block from cache
									// so dfw.epp:modify_procedure() can flip procedure body without 
									// invalidating procedure pointers from other parts of metadata cache

const USHORT PRC_check_existence	= 128;	/* Existence lock released */

//const USHORT MAX_PROC_ALTER			= 64;	/* No. of times an in-cache procedure can be altered */



/* Parameter block */

class Parameter : public pool_alloc<type_prm>
{
    public:
	USHORT		prm_number;
	dsc			prm_desc;
	jrd_nod*	prm_default_val;
	Firebird::MetaName prm_name;		/* asciiz name */
    public:
	explicit Parameter(MemoryPool& p) : prm_name(p) { }
};


/* Primary dependencies from all foreign references to relation's
   primary/unique keys */

struct prim {
	vec* prim_reference_ids;
	vec* prim_relations;
	vec* prim_indexes;
};

typedef prim *PRIM;

/* Foreign references to other relations' primary/unique keys */

struct frgn {
	vec* frgn_reference_ids;
	vec* frgn_relations;
	vec* frgn_indexes;
};

typedef frgn *FRGN;

// Relation trigger definition
class Trigger {
public:
	Firebird::HalfStaticArray<UCHAR, 128> blr;	// BLR code
	jrd_req*	request;		// Compiled request. Gets filled on first invocation
	bool		compile_in_progress;
	bool		sys_trigger;
	UCHAR		type;			// Trigger type
	USHORT		flags;			// Flags as they are in RDB$TRIGGERS table
	jrd_rel*	relation;		// Trigger parent relation
	Firebird::MetaName	name;	// Trigger name
	void compile(thread_db*);	// Ensure that trigger is compiled
	void release(thread_db*);	// Try to free trigger request

	explicit Trigger(MemoryPool& p) : blr(p), name(p) { }
};

typedef Firebird::ObjectsArray<Trigger> trig_vec;

#ifdef GARBAGE_THREAD

class RelationGarbage
{
private:
	class TranGarbage {
	public:
		SLONG tran;
		PageBitmap *bm;

		TranGarbage(PageBitmap *aBm, SLONG aTran) : tran(aTran), bm(aBm) {}

		static inline const SLONG generate(void const*, const TranGarbage& Item) 
		{ return Item.tran; }
	};

	typedef	Firebird::SortedArray<
				TranGarbage, 
				Firebird::EmptyStorage<TranGarbage>, 
				SLONG, 
				TranGarbage> TranGarbageArray;
	
	TranGarbageArray array;

public:
	explicit RelationGarbage(MemoryPool& p) : array(p) {}
	~RelationGarbage() { clear(); }

	void addPage(MemoryPool* pool, const SLONG pageno, const SLONG tranid);
	void clear();

	void getGarbage(const SLONG oldest_snapshot, PageBitmap **sbm);
	
	SLONG minTranID() 
	{ return (array.getCount() > 0) ? array[0].tran : MAX_SLONG; }
};

#endif //GARBAGE_THREAD


/* view context block to cache view aliases */

class ViewContext
{
public:
	Firebird::MetaName	vcx_context_name;
	Firebird::MetaName	vcx_relation_name;
	USHORT	vcx_context;
	static const USHORT& generate(const void*, const ViewContext& vc) 
	{ 
		return vc.vcx_context; 
	}
};

typedef Firebird::SortedArray<ViewContext, Firebird::EmptyStorage<ViewContext>, 
		USHORT, ViewContext> ViewContexts;


/* Relation block; one is created for each relation referenced
   in the database, though it is not really filled out until
   the relation is scanned */

class jrd_rel : public pool_alloc<type_rel>
{
public:
	USHORT	rel_id;
	USHORT	rel_flags;
	USHORT	rel_current_fmt;	/* Current format number */
	Format*	rel_current_format;	/* Current record format */
	Firebird::MetaName	rel_name;		/* ascii relation name */
	vec*	rel_formats;		/* Known record formats */
	Firebird::MetaName	rel_owner_name;	/* ascii owner */
	vcl*	rel_pages;			/* vector of pointer page numbers */
	vec*	rel_fields;			/* vector of field blocks */

	RecordSelExpr*	rel_view_rse;		/* view record select expression */
	ViewContexts	rel_view_contexts;	/* sorted array of view contexts */

	Firebird::MetaName	rel_security_name;	/* security class name for relation */
	ExternalFile* rel_file;		/* external file name */
	SLONG rel_index_root;		/* index root page number */
	SLONG rel_data_pages;		/* count of relation data pages */

	vec*	rel_gc_rec;		/* vector of records for garbage collection */
#ifdef GARBAGE_THREAD
	PageBitmap*	rel_gc_bitmap;	/* garbage collect bitmap of data page sequences */
	RelationGarbage*	rel_garbage;	/* deffered gc bitmap's by tran numbers */
#endif

	USHORT rel_slot_space;		/* lowest pointer page with slot space */
	USHORT rel_data_space;		/* lowest pointer page with data page space */
	USHORT rel_use_count;		/* requests compiled with relation */
	USHORT rel_sweep_count;		/* sweep and/or garbage collector threads active */
	SSHORT rel_scan_count;		/* concurrent sequential scan count */

	Lock*	rel_existence_lock;	/* existence lock, if any */
	Lock*	rel_interest_lock;	/* interest lock to ensure compatibility of relation and record locks */
	Lock*	rel_record_locking;	/* lock to start record locking on relation */
	Lock*	rel_partners_lock;	/* partners lock */

	ULONG rel_explicit_locks;	/* count of records explicitly locked in relation */
	ULONG rel_read_locks;		/* count of records read locked in relation (implicit or explicit) */
	ULONG rel_write_locks;		/* count of records write locked in relation (implicit or explicit) */
	ULONG rel_lock_total;		/* count of records locked since database first attached */

	IndexLock*	rel_index_locks;	/* index existence locks */
	IndexBlock*	rel_index_blocks;	/* index blocks for caching index info */
	trig_vec*	rel_pre_erase; 	/* Pre-operation erase trigger */
	trig_vec*	rel_post_erase;	/* Post-operation erase trigger */
	trig_vec*	rel_pre_modify;	/* Pre-operation modify trigger */
	trig_vec*	rel_post_modify;	/* Post-operation modify trigger */
	trig_vec*	rel_pre_store;		/* Pre-operation store trigger */
	trig_vec*	rel_post_store;	/* Post-operation store trigger */
	prim rel_primary_dpnds;	/* foreign dependencies on this relation's primary key */
	frgn rel_foreign_refs;	/* foreign references to other relations' primary keys */

public:
	explicit jrd_rel(MemoryPool& p) 
		: rel_name(p), rel_owner_name(p), rel_view_contexts(p), rel_security_name(p) { }
};

// rel_flags

const USHORT REL_scanned				= 1;		/* Field expressions scanned (or being scanned) */
const USHORT REL_system					= 2;
const USHORT REL_deleted				= 4;		/* Relation known gonzo */
const USHORT REL_get_dependencies		= 8;			/* New relation needs dependencies during scan */
const USHORT REL_force_scan				= 16;		/* system relation has been updated since ODS change, force a scan */
const USHORT REL_check_existence		= 32;		/* Existence lock released pending drop of relation */
const USHORT REL_blocking				= 64;		/* Blocking someone from dropping relation */
const USHORT REL_sys_triggers			= 128;		/* The relation has system triggers to compile */
const USHORT REL_sql_relation			= 256;		/* Relation defined as sql table */
const USHORT REL_check_partners			= 512;		/* Rescan primary dependencies and foreign references */
const USHORT REL_being_scanned			= 1024;		/* relation scan in progress */
const USHORT REL_sys_trigs_being_loaded	= 2048;		/* System triggers being loaded */
const USHORT REL_deleting				= 4096;		/* relation delete in progress */


/* Field block, one for each field in a scanned relation */

class jrd_fld : public pool_alloc<type_fld>
{
public:
	jrd_nod*	fld_validation;		/* validation clause, if any */
	jrd_nod*	fld_not_null;		/* if field cannot be NULL */
	jrd_nod*	fld_missing_value;	/* missing value, if any */
	jrd_nod*	fld_computation;	/* computation for virtual field */
	jrd_nod*	fld_source;			/* source for view fields */
	jrd_nod*	fld_default_value;	/* default value, if any */
	ArrayField*	fld_array;			/* array description, if array */
	Firebird::MetaName	fld_name;	/* Field name */
	Firebird::MetaName	fld_security_name;	/* security class name for field */

public:
	explicit jrd_fld(MemoryPool& p) 
		: fld_name(p), fld_security_name(p) { }
};



/* Index block to cache index information */

class IndexBlock : public pool_alloc<type_idb>
{
    public:
	IndexBlock*	idb_next;
	jrd_nod*	idb_expression;			/* node tree for index expression */
	jrd_req*	idb_expression_request;	/* request in which index expression is evaluated */
	dsc			idb_expression_desc;	/* descriptor for expression result */
	Lock*		idb_lock;				/* lock to synchronize changes to index */
	UCHAR idb_id;
};



/* general purpose vector */
template <class T, USHORT TYPE = type_vec>
class vec_base : protected pool_alloc<TYPE>
{
public:
	typedef typename Firebird::vector<T>::iterator iterator;
	typedef typename Firebird::vector<T>::const_iterator const_iterator;

	static vec_base* newVector(MemoryPool& p, int len)
	{
		return FB_NEW(p) vec_base<T, TYPE>(p, len);
	}
	static vec_base* newVector(MemoryPool& p, const vec_base& base)
	{
		return FB_NEW(p) vec_base<T, TYPE>(p, base);
	}
		
	// CVC: This should be size_t instead of ULONG for maximum portability.
	ULONG count() const { return vector.size(); }
	T& operator[](size_t index) { return vector[index]; }
	const T& operator[](size_t index) const { return vector[index]; }

	iterator begin() { return vector.begin(); }
	iterator end() { return vector.end(); }

	const_iterator begin() const { return vector.begin(); }
	const_iterator end() const { return vector.end(); }
	
	void clear() { vector.clear(); }
	void prepend(int n) { vector.insert(vector.begin(), n); }
	
//	T* memPtr() { return &*(vector.begin()); }
	T* memPtr() { return &vector[0]; }

	void resize(size_t n, T val = T()) { vector.resize(n, val); }

	void operator delete(void* mem) { MemoryPool::globalFree(mem); }

protected:
	vec_base(MemoryPool& p, int len)
		: vector(len, p, TYPE) {}
	vec_base(MemoryPool& p, const vec_base& base)
		: vector(p, TYPE) { vector = base.vector; }

private:
	Firebird::vector<T> vector;
};

class vec : public vec_base<BlkPtr, type_vec>
{
public:
    static vec* newVector(MemoryPool& p, int len)
	{
		return FB_NEW(p) vec(p, len);
	}
    static vec* newVector(MemoryPool& p, const vec& base)
	{
		return FB_NEW(p) vec(p, base);
	}
	static vec* newVector(MemoryPool& p, vec* base, int len)
	{
		if (!base)
			base = FB_NEW(p) vec(p, len);
		else if (len > (int) base->count())
			base->resize(len);
		return base;
	}

private:
    vec(MemoryPool& p, int len) : vec_base<BlkPtr, type_vec>(p, len) {}
    vec(MemoryPool& p, const vec& base) : vec_base<BlkPtr, type_vec>(p, base) {}
};

class vcl : public vec_base<SLONG, type_vcl>
{
public:
    static vcl* newVector(MemoryPool& p, int len)
	{
		return FB_NEW(p) vcl(p, len);
	}
    static vcl* newVector(MemoryPool& p, const vcl& base)
	{
		return FB_NEW(p) vcl(p, base);
	}
	static vcl* newVector(MemoryPool& p, vcl* base, int len)
	{
		if (!base)
			base = FB_NEW(p) vcl(p, len);
		else if (len > (int) base->count())
			base->resize(len);
		return base;
	}

private:
    vcl(MemoryPool& p, int len) : vec_base<SLONG, type_vcl>(p, len) {}
    vcl(MemoryPool& p, const vcl& base) : vec_base<SLONG, type_vcl>(p, base) {}
};
typedef vcl* VCL;

//#define TEST_VECTOR(vector, number)      ((vector && number < vector->vec_count) ?
//					  vector->vec_object [number] : NULL)


//
// Transaction element block
//
struct teb {
	Attachment** teb_database;
	int teb_tpb_length;
	UCHAR* teb_tpb;
};

typedef teb TEB;

/* Blocking Thread Block */

class BlockingThread : public pool_alloc<type_btb>
{
    public:
	BlockingThread* btb_next;
	thread* btb_thread_id;
};

/* Window block for loading cached pages into */
// CVC: Apparently, the only possible values are HEADER_PAGE==0 and LOG_PAGE==2
// and reside in ods.h, although I watched a place with 1 and others with members
// of a struct.

struct win {
	SLONG win_page;
	Ods::pag* win_buffer;
	exp_index_buf* win_expanded_buffer;
	class BufferDesc* win_bdb;
	SSHORT win_scans;
	USHORT win_flags;
	explicit win(SLONG wp) : win_page(wp), win_flags(0) {}
};

typedef win WIN;

// This is a compilation artifact: I wanted to be sure I would pick all old "win"
// declarations at the top, so "win" was built with a mandatory argument in
// the constructor. This struct satisfies a single place with an array. The
// alternative would be to initialize 16 elements of the array with 16 calls
// to the constructor: win my_array[n] = {win(-1), ... (win-1)};
// When all places are changed, this class can disappear and win's constructor
// may get the default value of -1 to "wp".
struct win_for_array: public win
{
	win_for_array() : win(-1) {}
};

// win_flags

const USHORT WIN_large_scan		= 1;	/* large sequential scan */
const USHORT WIN_secondary			= 2;	/* secondary stream */
const USHORT WIN_garbage_collector	= 4;	/* garbage collector's window */
const USHORT WIN_garbage_collect	= 8;	/* scan left a page for garbage collector */


// Thread specific database block
class thread_db : public ThreadData
{
private:
	JrdMemoryPool*	tdbb_default;
	void setDefaultPool(JrdMemoryPool* p)
	{
		tdbb_default = p;
	}
	friend class Firebird::SubsystemContextPoolHolder <Jrd::thread_db, JrdMemoryPool>;
public:
	thread_db() 
		: ThreadData(ThreadData::tddDBB)
	{
		tdbb_default = 0;
		tdbb_database = 0;
		tdbb_attachment = 0;
		tdbb_transaction = 0;
		tdbb_request = 0;
		tdbb_status_vector = 0;
		tdbb_setjmp = 0;
		tdbb_quantum = 0;
		tdbb_flags = 0;
		JRD_inuse_clear(this);
	}
	Database*	tdbb_database;
	Attachment*	tdbb_attachment;
	jrd_tra*	tdbb_transaction;
	jrd_req*	tdbb_request;
	ISC_STATUS*	tdbb_status_vector;
	void*		tdbb_setjmp;
	SSHORT		tdbb_quantum;		// Cycles remaining until voluntary schedule
	USHORT		tdbb_flags;
	struct iuo	tdbb_mutexes;
	struct iuo	tdbb_rw_locks;
	struct iuo	tdbb_pages;

#if defined(UNIX) && defined(SUPERSERVER)
    sigjmp_buf tdbb_sigsetjmp;
#endif

	JrdMemoryPool* getDefaultPool()
	{
		return tdbb_default;
	}
};

// tdbb_flags

const USHORT TDBB_sweeper				= 1;	/* Thread sweeper or garbage collector */
const USHORT TDBB_no_cache_unwind		= 2;	/* Don't unwind page buffer cache */
const USHORT TDBB_prc_being_dropped		= 4;	/* Dropping a procedure  */
const USHORT TDBB_set_backup_state		= 8;	/* Setting state for backup lock */
const USHORT TDBB_backup_merge			= 16;	/* Merging changes from difference file */
const USHORT TDBB_stack_trace_done		= 32;	/* PSQL stack trase is added into status-vector */

/* List of internal database handles */

struct ihndl
{
	ihndl*	ihndl_next;
	void*	ihndl_object;
};

// duplicate context of firebird string to store in jrd_nod::nod_arg
inline char* stringDup(MemoryPool& p, const Firebird::string& s)
{
	char* rc = (char*) p.allocate(s.length() + 1, 0
#ifdef DEBUG_GDS_ALLOC
		, __FILE__, __LINE__
#endif
		);
	strcpy(rc, s.c_str());
	return rc;
}

inline char* stringDup(MemoryPool& p, const char* s, size_t l)
{
	char* rc = (char*) p.allocate(l + 1, 0
#ifdef DEBUG_GDS_ALLOC
		, __FILE__, __LINE__
#endif
		);
	memcpy(rc, s, l);
	rc[l] = 0;
	return rc;
}

inline char* stringDup(MemoryPool& p, const char* s)
{
	if (! s)
	{
		return 0;
	}
	return stringDup(p, s, strlen(s));
}

// Used in string convsersion calls
typedef Firebird::HalfStaticArray<UCHAR, 256> MoveBuffer;

} //namespace Jrd

/* Lock levels */
#include "../lock/lock.h"

/* Random string block -- as long as impure areas don't have
 * constructors and destructors, the need this varying string */

class VaryingString : public pool_alloc_rpt<SCHAR, type_str>
{
public:
	USHORT str_length;
	UCHAR str_data[2];			// one byte for ALLOC and one for the NULL
};

/* Threading macros */

/* Define JRD_get_thread_data off the platform specific version.
 * If we're in DEV mode, also do consistancy checks on the
 * retrieved memory structure.  This was originally done to
 * track down cases of no "PUT_THREAD_DATA" on the NLM. 
 *
 * This allows for NULL thread data (which might be an error by itself)
 * If there is thread data, 
 * AND it is tagged as being a thread_db.
 * AND it has a non-NULL tdbb_database field, 
 * THEN we validate that the structure there is a database block.
 * Otherwise, we return what we got.
 * We can't always validate the database field, as during initialization
 * there is no tdbb_database set up.
 */

#include "../jrd/thd.h"

#ifdef DEV_BUILD
#include "../jrd/err_proto.h"

inline Jrd::thread_db* JRD_get_thread_data() {
	ThreadData* p1 = ThreadData::getSpecific();
	if (p1 && p1->getType() == ThreadData::tddDBB)
	{
		Jrd::thread_db* p2 = (Jrd::thread_db*)p1;
		if (p2->tdbb_database && MemoryPool::blk_type(p2->tdbb_database) != type_dbb)
		{
			BUGCHECK(147);
		}
	}
	return (Jrd::thread_db*) p1;
}
inline void CHECK_TDBB(const Jrd::thread_db* tdbb) {
	fb_assert(tdbb && (tdbb->getType() == ThreadData::tddDBB) &&
			(!tdbb->tdbb_database ||
				MemoryPool::blk_type(tdbb->tdbb_database) == type_dbb));
}
inline void CHECK_DBB(const Jrd::Database* dbb) {
	fb_assert(dbb && MemoryPool::blk_type(dbb) == type_dbb);
}

#else
/* PROD_BUILD */
inline Jrd::thread_db* JRD_get_thread_data() {
	return (Jrd::thread_db*) ThreadData::getSpecific();
}
inline void CHECK_DBB(const Jrd::Database* dbb) {
}
inline void CHECK_TDBB(const Jrd::thread_db* tdbb) {
}

#endif

inline Jrd::Database* GET_DBB() {
	return JRD_get_thread_data()->tdbb_database;
}

/*-------------------------------------------------------------------------*
 * macros used to set thread_db and Database pointers when there are not set already *
 *-------------------------------------------------------------------------*/
inline void SET_TDBB(Jrd::thread_db* &tdbb) {
	if (tdbb == NULL) {
		tdbb = JRD_get_thread_data();
	}
	CHECK_TDBB(tdbb);
}
inline void SET_DBB(Jrd::Database* &dbb) {
	if (dbb == NULL) {
		dbb = GET_DBB();
	}
	CHECK_DBB(dbb);
}

#ifdef V4_THREADING
#define V4_JRD_MUTEX_LOCK(mutx)         JRD_mutex_lock (mutx)
#define V4_JRD_MUTEX_UNLOCK(mutx)       JRD_mutex_unlock (mutx)
#define V4_JRD_RW_LOCK_LOCK(wlck, type)  JRD_wlck_lock (wlck, type)
#define V4_JRD_RW_LOCK_UNLOCK(wlck)     JRD_wlck_unlock (wlck)
// BRS. 03/23/2003
// Those empty defines was substituted with #ifdef V4_THREADING
//#else
//#define V4_JRD_MUTEX_LOCK(mutx)
//#define V4_JRD_MUTEX_UNLOCK(mutx)
//#define V4_JRD_RW_LOCK_LOCK(wlck, type)
//#define V4_JRD_RW_LOCK_UNLOCK(wlck)
#endif

#ifdef ANY_THREADING
#define THD_JRD_MUTEX_LOCK(mutx)        JRD_mutex_lock (mutx)
#define THD_JRD_MUTEX_UNLOCK(mutx)      JRD_mutex_unlock (mutx)
#else
#define THD_JRD_MUTEX_LOCK(mutx)
#define THD_JRD_MUTEX_UNLOCK(mutx)
#endif


/* global variables for engine */


#if !defined(REQUESTER)

extern int debug;

#endif /* REQUESTER */


/* Define the xxx_thread_data macros.  These are needed in the whole 
   component, but they are defined differently for use in jrd.cpp (JRD_MAIN)
   Here we have a function which sets some flags, and then calls THD_put_specific
   so in this case we define the macro as calling that function. */
// CVC: This may be obsolete now that different subsystems use different macro/function names.

inline static void JRD_set_thread_data(Jrd::thread_db* &tdbb, Jrd::thread_db& thd_context)
{
	tdbb = &thd_context;
	tdbb->putSpecific();
}

inline void JRD_restore_thread_data() {
	ThreadData::restoreSpecific();
}

namespace Jrd {
	typedef Firebird::SubsystemContextPoolHolder <Jrd::thread_db, JrdMemoryPool> 
		ContextPoolHolder;
}

#endif /* JRD_JRD_H */
