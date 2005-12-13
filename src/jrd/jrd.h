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

#include "../include/fb_vector.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef HAVE_SETJMP_H
#include <setjmp.h>
#endif

#ifdef DEV_BUILD
#define DEBUG                   if (debug) DBG_supervisor(debug);
#define VIO_DEBUG				/* remove this for production build */
#define WALW_DEBUG				/* remove this for production build */
#else /* PROD */
#define DEBUG
#undef VIO_DEBUG
#undef WALW_DEBUG
#endif

/* Moved to err_proto.h */
/*#define BUGCHECK(number)        ERR_bugcheck (number)
#define CORRUPT(number)         ERR_corrupt (number)
#define IBERROR(number)         ERR_error (number)
*/


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
//#define DEV_BLKCHK(blk,type)    if (blk) {BLKCHK (blk, type);}
#else
#define DEV_BLKCHK(blk,type)	/* nothing */
#endif


/* Thread data block / IPC related data blocks */

#ifdef __cplusplus
} /* extern "C" */
#endif

#include "../jrd/thd.h"
#include "../jrd/isc.h"

/* definition of block types for data allocation in JRD */
#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"


/* the database block, the topmost block in the metadata 
   cache for a database */

#define HASH_SIZE 101


// fwd. decl.
class vec;
struct tdbb;


class dbb : private pool_alloc<type_dbb>
{
public:
	typedef int (*crypt_routine) (char*, void*, int, void*);

	static dbb* newDbb(MemoryPool& p) {		
		return FB_NEW(p) dbb(p);
	}
	
	// The deleteDbb function MUST be used to delete a dbb object.
	// The function hides some tricky order of operations.  Since the
	// memory for the vectors in the dbb is allocated out of the dbb's
	// permanent memory pool, the entire delete() operation needs
	// to complete _before_ the permanent pool is deleted, or else
	// risk an aborted engine.
	static void deleteDbb(dbb *toDelete)
	{
		if (toDelete == 0)
			return;
		JrdMemoryPool *perm = toDelete->dbb_permanent;
		delete toDelete;
		JrdMemoryPool::noDbbDeletePool(perm);
	}
	
	class dbb *dbb_next;		/* Next database block in system */
	class att *dbb_attachments;	/* Active attachments */
	struct bcb *dbb_bcb;		/* Buffer control block */
	vec*		dbb_relations;	/* relation vector */
	vec*		dbb_procedures;	/* scanned procedures */
	struct lck *dbb_lock;		/* granddaddy lock */
	struct jrd_tra *dbb_sys_trans;	/* system transaction */
	struct fil *dbb_file;		/* files for I/O operations */
	struct sdw *dbb_shadow;		/* shadow control block */
	struct lck *dbb_shadow_lock;	/* lock for synchronizing addition of shadows */
	SLONG dbb_shadow_sync_count;	/* to synchronize changes to shadows */
	struct lck *dbb_retaining_lock;	/* lock for preserving commit retaining snapshot */
	struct plc *dbb_connection;	/* connection block */
	struct pgc *dbb_pcontrol;	/* page control */
	class vcl *dbb_t_pages;	/* pages number for transactions */
	class vcl *dbb_gen_id_pages;	/* known pages for gen_id */
	struct blf *dbb_blob_filters;	/* known blob filters */
	struct lls *dbb_modules;	/* external function/filter modules */
	MUTX_T *dbb_mutexes;		/* DBB block mutexes */
	WLCK_T *dbb_rw_locks;		/* DBB block read/write locks */
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
	class str *dbb_spare_string;	/* random buffer */
	class str *dbb_filename;	/* filename string */
	class str *dbb_encrypt_key;	/* encryption key */

	JrdMemoryPool* dbb_permanent;
	JrdMemoryPool* dbb_bufferpool;

	typedef JrdMemoryPool* pool_ptr;
	typedef Firebird::vector<pool_ptr> pool_vec_type;

	pool_vec_type dbb_pools;		/* pools */
    USHORT dbb_next_pool_id;
	vec*		dbb_internal;	/* internal requests */
	vec*		dbb_dyn_req;	/* internal dyn requests */
	struct jrn *dbb_journal;	/* journal block */

	SLONG dbb_oldest_active;	/* Cached "oldest active" transaction */
	SLONG dbb_oldest_transaction;	/* Cached "oldest interesting" transaction */
	SLONG dbb_oldest_snapshot;	/* Cached "oldest snapshot" of all active transactions */
	SLONG dbb_next_transaction;	/* Next transaction id used by NETWARE */
	SLONG dbb_attachment_id;	/* Next attachment id for ReadOnly DB's */
	SLONG dbb_page_incarnation;	/* Cache page incarnation counter */
	ULONG dbb_page_buffers;		/* Page buffers from header page */

	EVENT_T dbb_writer_event[1];	/* Event to wake up cache writer */
	EVENT_T dbb_writer_event_init[1];	/* Event for initialization cache writer */
	EVENT_T dbb_writer_event_fini[1];	/* Event for finalization cache writer */
	EVENT_T dbb_reader_event[1];	/* Event to wake up cache reader */
#ifdef GARBAGE_THREAD
	EVENT_T dbb_gc_event[1];	/* Event to wake up garbage collector */
	EVENT_T dbb_gc_event_init[1];	/* Event for initialization garbage collector */
	EVENT_T dbb_gc_event_fini[1];	/* Event for finalization garbage collector */
#endif
	class att *dbb_update_attachment;	/* Attachment with update in process */
	class btb *dbb_update_que;	/* Attachments waiting for update */
	class btb *dbb_free_btbs;	/* Unused btb blocks */

	SLONG dbb_current_memory;
	SLONG dbb_max_memory;
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

	class map *dbb_blob_map;	/* mapping of blobs for REPLAY */
	struct log *dbb_log;		/* log file for REPLAY */
	Firebird::vector<class TextType*>		dbb_text_objects;	/* intl text type descriptions */
	Firebird::vector<class CharSetContainer*>		dbb_charsets;	/* intl character set descriptions */
	struct wal *dbb_wal;		/* WAL handle for WAL API */
	struct tpc *dbb_tip_cache;	/* cache of latest known state of all transactions in system */
	class vcl *dbb_pc_transactions;	/* active precommitted transactions */
	class sym *dbb_hash_table[HASH_SIZE];	/* keep this at the end */

private:
	dbb(MemoryPool& p)
	:	dbb_pools(1, p, type_dbb),
		dbb_text_objects(p),
		dbb_charsets(p)
	{
	}

	~dbb()
	{
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

	// The delete operators are no-oped because the dbb memory is allocated from the
	// dbb's own permanent pool.  That pool has already been released by the dbb
	// destructor, so the memory has already been released.  Hence the operator
	// delete no-op.
	void operator delete(void *mem) {}
	void operator delete[](void *mem) {}

	dbb(const dbb&);	// no impl.
	const dbb& operator =(const dbb&) { return *this; }
};
typedef dbb* DBB;

//
// bit values for dbb_flags
//
#define DBB_no_garbage_collect 	0x1L
#define DBB_damaged         	0x2L
#define DBB_exclusive       	0x4L	/* Database is accessed in exclusive mode */
#define DBB_bugcheck        	0x8L	/* Bugcheck has occurred */
#ifdef GARBAGE_THREAD
#define DBB_garbage_collector	0x10L	/* garbage collector thread exists */
#define DBB_gc_active			0x20L	/* ... and is actively working. */
#define DBB_gc_pending			0x40L	/* garbage collection requested */
#endif
#define DBB_force_write     	0x80L	/* Database is forced write */
#define DBB_no_reserve     		0x100L	/* No reserve space for versions */
#define DBB_add_log         	0x200L	/* write ahead log has been added */
#define DBB_delete_log      	0x400L	/* write ahead log has been deleted */
#define DBB_cache_manager   	0x800L	/* Shared cache manager */
#define DBB_DB_SQL_dialect_3 	0x1000L	/* database SQL dialect 3 */
#define DBB_read_only    		0x2000L	/* DB is ReadOnly (RO). If not set, DB is RW */
#define DBB_being_opened_read_only 0x4000L	/* DB is being opened RO. If unset, opened as RW */
#define DBB_not_in_use      	0x8000L	/* DBB to be ignored while attaching */
#define DBB_lck_init_done   	0x10000L	/* LCK_init() called for the database */
#define DBB_sp_rec_mutex_init 	0x20000L	/* Stored procedure mutex initialized */
#define DBB_sweep_in_progress 	0x40000L	/* A database sweep operation is in progress */
#define DBB_security_db     	0x80000L	/* ISC security database */
#define DBB_sweep_thread_started 0x100000L	/* A database sweep thread has been started */
#define DBB_suspend_bgio		0x200000L	/* Suspend I/O by background threads */
#define DBB_being_opened    	0x400000L	/* database is being attached to */

//
// dbb_ast_flags
//
#define DBB_blocking		0x1L	// Exclusive mode is blocking
#define DBB_get_shadows		0x2L	// Signal received to check for new shadows
#define DBB_assert_locks	0x4L	// Locks are to be asserted
#define DBB_shutdown		0x8L	// Database is shutdown
#define DBB_shut_attach		0x10L	// no new attachments accepted
#define DBB_shut_tran		0x20L	// no new transactions accepted
#define DBB_shut_force		0x40L	// forced shutdown in progress
#define DBB_shutdown_locks	0x80L	// Database locks release by shutdown

//
// Database attachments
//
#define DBB_read_seq_count      0
#define DBB_read_idx_count      1
#define DBB_update_count        2
#define DBB_insert_count        3
#define DBB_delete_count        4
#define DBB_backout_count       5
#define DBB_purge_count         6
#define DBB_expunge_count       7
#define DBB_max_count           8

//
// Database mutexes and read/write locks
//
#define DBB_MUTX_init_fini      0	// During startup and shutdown
#define DBB_MUTX_statistics     1	// Memory size and counts
#define DBB_MUTX_replay         2	// Replay logging
#define DBB_MUTX_dyn            3	// Dynamic ddl
#define DBB_MUTX_cache          4	// Process-private cache management
#define DBB_MUTX_clone          5	// Request cloning
#define DBB_MUTX_cmp_clone      6	// Compiled request cloning
#define DBB_MUTX_flush_count    7	// flush count/time
#define DBB_MUTX_max            8
#define DBB_WLCK_pools          0	// Pool manipulation
#define DBB_WLCK_files          1	// DB and shadow file manipulation
#define DBB_WLCK_max            2

//
// Flags to indicate normal internal requests vs. dyn internal requests
//
#define IRQ_REQUESTS            1
#define DYN_REQUESTS            2


//
// Errors during validation - will be returned on info calls
//
#define VAL_PAG_WRONG_TYPE          0
#define VAL_PAG_CHECKSUM_ERR        1
#define VAL_PAG_DOUBLE_ALLOC        2
#define VAL_PAG_IN_USE              3
#define VAL_PAG_ORPHAN              4
#define VAL_BLOB_INCONSISTENT       5
#define VAL_BLOB_CORRUPT            6
#define VAL_BLOB_TRUNCATED          7
#define VAL_REC_CHAIN_BROKEN        8
#define VAL_DATA_PAGE_CONFUSED      9
#define VAL_DATA_PAGE_LINE_ERR      10
#define VAL_INDEX_PAGE_CORRUPT      11
#define VAL_P_PAGE_LOST             12
#define VAL_P_PAGE_INCONSISTENT     13
#define VAL_REC_DAMAGED             14
#define VAL_REC_BAD_TID             15
#define VAL_REC_FRAGMENT_CORRUPT    16
#define VAL_REC_WRONG_LENGTH        17
#define VAL_INDEX_ROOT_MISSING      18
#define VAL_TIP_LOST                19
#define VAL_TIP_LOST_SEQUENCE       20
#define VAL_TIP_CONFUSED            21
#define VAL_REL_CHAIN_ORPHANS       22
#define VAL_INDEX_MISSING_ROWS      23
#define VAL_INDEX_ORPHAN_CHILD      24
#define VAL_MAX_ERROR               25



//
// the attachment block; one is created for each attachment to a database
//
class att : public pool_alloc<type_att>
{
public:
/*	att()
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

	class dbb*	att_database;		// Parent databasea block
	att*		att_next;			// Next attachment to database
	att*		att_blocking;		// Blocking attachment, if any
	class usr*	att_user;			// User identification
	struct jrd_tra*	att_transactions;	// Transactions belonging to attachment
	struct jrd_tra*	att_dbkey_trans;	// transaction to control db-key scope
	struct jrd_req*	att_requests;		// Requests belonging to attachment
	struct scb*	att_active_sorts;	// Active sorts
	struct lck*	att_id_lock;		// Attachment lock (if any)
	SLONG		att_attachment_id;	// Attachment ID
	SLONG		att_lock_owner_handle;	// Handle for the lock manager
	SLONG		att_event_session;	// Event session id, if any
	class scl*	att_security_class;	// security class for database
	class scl*	att_security_classes;	// security classes
	class vcl*	att_counts[DBB_max_count];
	vec*		att_relation_locks;	// explicit persistent locks for relations
	struct bkm*	att_bookmarks;		// list of bookmarks taken out using this attachment
	struct lck*	att_record_locks;	// explicit or implicit record locks taken out during attachment
	vec*		att_bkm_quick_ref;	// correspondence table of bookmarks
	vec*		att_lck_quick_ref;	// correspondence table of locks
	ULONG		att_flags;			// Flags describing the state of the attachment
	SSHORT		att_charset;		// user's charset specified in dpb
	class str*	att_lc_messages;	// attachment's preference for message natural language
	struct lck*	att_long_locks;		// outstanding two phased locks
	vec*		att_compatibility_table;	// hash table of compatible locks
	class vcl*	att_val_errors;
	class str*	att_working_directory;	// Current working directory is cached
	class str*	att_filename;			// alias used to attach the database
};
typedef att* ATT;


/* Attachment flags */

#define ATT_no_cleanup			1	// Don't expunge, purge, or garbage collect
#define ATT_shutdown			2	// attachment has been shutdown
#define ATT_shutdown_notify		4	// attachment has notified client of shutdown
#define ATT_shutdown_manager	8	// attachment requesting shutdown
#define ATT_lck_init_done		16	// LCK_init() called for the attachment
#define ATT_exclusive			32	// attachment wants exclusive database access
#define ATT_attach_pending		64	// Indicate attachment is only pending
#define ATT_exclusive_pending   128	// Indicate exclusive attachment pending
#define ATT_gbak_attachment		256	// Indicate GBAK attachment
#define ATT_security_db			512	// Indicates an implicit attachment to the security db
#ifdef GARBAGE_THREAD
#define ATT_notify_gc			1024	// Notify garbage collector to expunge, purge ..
#define ATT_disable_notify_gc	2048	// Temporarily perform own garbage collection
#define ATT_garbage_collector	4096	// I'm a garbage collector

#define ATT_NO_CLEANUP	(ATT_no_cleanup | ATT_notify_gc)
#else
#define ATT_NO_CLEANUP	ATT_no_cleanup
#endif

#ifdef CANCEL_OPERATION
#define ATT_cancel_raise		8192	// Cancel currently running operation
#define ATT_cancel_disable		16384	// Disable cancel operations
#endif

#define ATT_gfix_attachment		32768	// Indicate a GFIX attachment
#define ATT_gstat_attachment	65536	// Indicate a GSTAT attachment


/* Procedure block */

class jrd_prc : public pool_alloc_rpt<SCHAR, type_prc>
{
    public:
	USHORT prc_id; // Should be first field because MET_remove_procedure relies on that
	USHORT prc_flags;
	USHORT prc_inputs;
	USHORT prc_outputs;
	struct jrd_nod *prc_input_msg;
	struct jrd_nod *prc_output_msg;
	struct fmt *prc_input_fmt;
	struct fmt *prc_output_fmt;
	struct fmt *prc_format;
	vec*		prc_input_fields;	/* vector of field blocks */
	vec*		prc_output_fields;	/* vector of field blocks */
	struct jrd_req *prc_request;	/* compiled procedure request */
	class str *prc_security_name;	/* pointer to security class name for procedure */
	USHORT prc_use_count;		/* requests compiled with procedure */
	SSHORT prc_int_use_count;	/* number of procedures compiled with procedure, set and 
	                               used internally in the MET_clear_cache procedure 
								   no code should rely on value of this field 
								   (it will usually be 0)
								*/
	struct lck *prc_existence_lock;	/* existence lock, if any */
	class str *prc_name;		/* pointer to ascic name */
	USHORT prc_alter_count;		/* No. of times the procedure was altered */
};
typedef jrd_prc* JRD_PRC;

#define PRC_scanned           1		/* Field expressions scanned */
#define PRC_system            2
#define PRC_obsolete          4		/* Procedure known gonzo */
#define PRC_being_scanned     8		/* New procedure needs dependencies during scan */
#define PRC_blocking          16	/* Blocking someone from dropping procedure */
#define PRC_create            32	/* Newly created */
#define PRC_being_altered     64	/* Procedure is getting altered */
#define PRC_check_existence   128	/* Existence lock released */

#define MAX_PROC_ALTER        64	/* No. of times an in-cache procedure can be altered */



/* Parameter block */

class prm : public pool_alloc_rpt<SCHAR, type_prm>
{
    public:
	USHORT prm_number;
	struct dsc prm_desc;
	TEXT *prm_name;				/* pointer to asciiz name */
	TEXT prm_string[2];			/* one byte for ALLOC and one for the terminating null */
};
typedef prm* PRM;


/* Primary dependencies from all foreign references to relation's
   primary/unique keys */

typedef struct prim {
	vec* prim_reference_ids;
	vec* prim_relations;
	vec* prim_indexes;
} *PRIM;

/* Foreign references to other relations' primary/unique keys */

typedef struct frgn {
	vec* frgn_reference_ids;
	vec* frgn_relations;
	vec* frgn_indexes;
} *FRGN;

// Relation trigger definition
typedef struct trig {
    class str* blr; // BLR code
	jrd_req* request; // Compiled request. Gets filled on first invocation
	BOOLEAN compile_in_progress;
	BOOLEAN sys_trigger;
	USHORT flags; // Flags as they are in RDB$TRIGGERS table
	class jrd_rel* relation; // Trigger parent relation
	class str* name; // Trigger name
	void compile(tdbb* _tdbb); // Ensure that trigger is compiled
	BOOLEAN release(tdbb* _tdbb); // Try to free trigger request
} *TRIG;

typedef Firebird::vector<trig> trig_vec;

typedef trig_vec* TRIG_VEC;

/* Relation block; one is created for each relation referenced
   in the database, though it is not really filled out until
   the relation is scanned */

class jrd_rel : public pool_alloc<type_rel>
{
public:
	USHORT rel_id;
	USHORT rel_flags;
	USHORT rel_current_fmt;		/* Current format number */
	UCHAR rel_length;			/* length of ascii relation name */
	struct fmt *rel_current_format;	/* Current record format */
	TEXT*	rel_name;				/* pointer to ascii relation name */
	vec*	rel_formats;	/* Known record formats */
	TEXT *rel_owner_name;		/* pointer to ascii owner */
	class vcl *rel_pages;		/* vector of pointer page numbers */
	vec*	rel_fields;		/* vector of field blocks */

	struct rse *rel_view_rse;	/* view record select expression */
	class vcx *rel_view_contexts;	/* linked list of view contexts */

	TEXT *rel_security_name;	/* pointer to security class name for relation */
	struct ext *rel_file;		/* external file name */
	SLONG rel_index_root;		/* index root page number */
	SLONG rel_data_pages;		/* count of relation data pages */

	vec*	rel_gc_rec;		/* vector of records for garbage collection */
#ifdef GARBAGE_THREAD
	struct sbm *rel_gc_bitmap;	/* garbage collect bitmap of data page sequences */
#endif

	USHORT rel_slot_space;		/* lowest pointer page with slot space */
	USHORT rel_data_space;		/* lowest pointer page with data page space */
	USHORT rel_use_count;		/* requests compiled with relation */
	USHORT rel_sweep_count;		/* sweep and/or garbage collector threads active */
	SSHORT rel_scan_count;		/* concurrent sequential scan count */

	struct lck *rel_existence_lock;	/* existence lock, if any */
	struct lck *rel_interest_lock;	/* interest lock to ensure compatibility of relation and record locks */
	struct lck *rel_record_locking;	/* lock to start record locking on relation */

	ULONG rel_explicit_locks;	/* count of records explicitly locked in relation */
	ULONG rel_read_locks;		/* count of records read locked in relation (implicit or explicit) */
	ULONG rel_write_locks;		/* count of records write locked in relation (implicit or explicit) */
	ULONG rel_lock_total;		/* count of records locked since database first attached */

	struct idl *rel_index_locks;	/* index existence locks */
	struct idb *rel_index_blocks;	/* index blocks for caching index info */
	trig_vec   *rel_pre_erase; 	/* Pre-operation erase trigger */
	trig_vec   *rel_post_erase;	/* Post-operation erase trigger */
	trig_vec   *rel_pre_modify;	/* Pre-operation modify trigger */
	trig_vec   *rel_post_modify;	/* Post-operation modify trigger */
	trig_vec   *rel_pre_store;		/* Pre-operation store trigger */
	trig_vec   *rel_post_store;	/* Post-operation store trigger */
	struct prim rel_primary_dpnds;	/* foreign dependencies on this relation's primary key */
	struct frgn rel_foreign_refs;	/* foreign references to other relations' primary keys */
};
typedef jrd_rel* JRD_REL;

#define REL_scanned					1		/* Field expressions scanned (or being scanned) */
#define REL_system					2
#define REL_deleted					4		/* Relation known gonzo */
#define REL_get_dependencies		8		/* New relation needs dependencies during scan */
#define REL_force_scan				16		/* system relation has been updated since ODS change, force a scan */
#define REL_check_existence			32		/* Existence lock released pending drop of relation */
#define REL_blocking				64		/* Blocking someone from dropping relation */
#define REL_sys_triggers			128		/* The relation has system triggers to compile */
#define REL_sql_relation			256		/* Relation defined as sql table */
#define REL_check_partners			512		/* Rescan primary dependencies and foreign references */
#define	REL_being_scanned			1024	/* relation scan in progress */
#define REL_sys_trigs_being_loaded	2048	/* System triggers being loaded */
#define REL_deleting				4096	/* relation delete in progress */


/* Field block, one for each field in a scanned relation */

class jrd_fld : public pool_alloc_rpt<SCHAR, type_fld>
{
    public:
	struct jrd_nod*	fld_validation;		/* validation clause, if any */
	struct jrd_nod*	fld_not_null;		/* if field cannot be NULL */
	struct jrd_nod*	fld_missing_value;	/* missing value, if any */
	struct jrd_nod*	fld_computation;	/* computation for virtual field */
	struct jrd_nod*	fld_source;			/* source for view fields */
	struct jrd_nod*	fld_default_value;	/* default value, if any */
	TEXT *		fld_security_name;	/* pointer to security class name for field */
	struct arr*	fld_array;			/* array description, if array */
	const TEXT*	fld_name;			/* Field name */
	UCHAR		fld_length;			/* Field name length */
	UCHAR		fld_string[2];		/* one byte for ALLOC and one for the terminating null */
};
typedef jrd_fld *JRD_FLD;



/* Index block to cache index information */

class idb : public pool_alloc<type_idb>
{
    public:
	struct idb*	idb_next;
	struct jrd_nod*	idb_expression;			/* node tree for index expression */
	struct jrd_req*	idb_expression_request;	/* request in which index expression is evaluated */
	struct dsc	idb_expression_desc;	/* descriptor for expression result */
	struct lck*	idb_lock;				/* lock to synchronize changes to index */
	UCHAR idb_id;
};
typedef idb *IDB;


/* view context block to cache view aliases */

class vcx: public pool_alloc<type_vcx>
{
    public:
	class vcx *vcx_next;
	class str *vcx_context_name;
	class str *vcx_relation_name;
	USHORT vcx_context;
};
typedef vcx *VCX;


/* general purpose vector */
template <class T, USHORT TYPE = type_vec>
class vec_base : protected pool_alloc<TYPE>
{
public:
	typedef typename Firebird::vector<T>::iterator iterator;

	static vec_base* newVector(MemoryPool& p, int len)
		{ return FB_NEW(p) vec_base<T,TYPE>(p, len); }
	static vec_base* newVector(MemoryPool& p, const vec_base& base)
		{ return FB_NEW(p) vec_base<T,TYPE>(p, base); }
		
	ULONG count() { return vector.size(); }
	T& operator[](size_t index) { return vector[index]; }

	iterator begin() { return vector.begin(); }
	iterator end() { return vector.end(); }
	
	void clear() { vector.clear(); }
	void prepend(int n) { vector.insert(vector.begin(), n); }
	
//	T* memPtr() { return &*(vector.begin()); }
	T* memPtr() { return &vector[0]; }

	void resize(size_t n, T val = T()) { vector.resize(n, val); }

	void operator delete(void *mem) { MemoryPool::globalFree(mem); }

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
        { return FB_NEW(p) vec(p, len); }
    static vec* newVector(MemoryPool& p, const vec& base)
        { return FB_NEW(p) vec(p, base); }
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
typedef vec* VEC;

class vcl : public vec_base<SLONG, type_vcl>
{
public:
    static vcl* newVector(MemoryPool& p, int len)
        { return FB_NEW(p) vcl(p, len); }
    static vcl* newVector(MemoryPool& p, const vcl& base)
        { return FB_NEW(p) vcl(p, base); }
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

#define TEST_VECTOR(vector,number)      ((vector && number < vector->vec_count) ? \
					  vector->vec_object [number] : NULL)

//
// general purpose queue
//
typedef struct que {
	struct que* que_forward;
	struct que* que_backward;
} *QUE;



/* symbol definitions */

typedef ENUM sym_t {
	SYM_rel,					/* relation block */
	SYM_fld,					/* field block */
	SYM_fun,					/* UDF function block */
	SYM_prc,					/* stored procedure block */
	SYM_sql,					/* SQL request cache block */
    SYM_blr,					/* BLR request cache block */
    SYM_label					/* CVC: I need to track labels if LEAVE is implemented. */
} SYM_T;

class sym : public pool_alloc<type_sym>
{
    public:
	TEXT *sym_string;			/* address of asciz string */
/*  USHORT	sym_length; *//* length of asciz string */
	SYM_T sym_type;				/* symbol type */
	BLK sym_object;		/* general pointer to object */
	sym *sym_collision;	/* collision pointer */
	sym *sym_homonym;	/* homonym pointer */
};
typedef sym *SYM;

/* Random string block -- jack of all kludges */

class str : public pool_alloc_rpt<SCHAR, type_str>
{
public:
	USHORT str_length;
	UCHAR str_data[2];			/* one byte for ALLOC and one for the NULL */

	static bool extend(str*& s, size_t new_len)
	{
		fb_assert(s);
		MemoryPool* pPool = MemoryPool::blk_pool(s);
		fb_assert(pPool);
		if (!pPool) {
			return false;	// runtime safety
		}
		// TMN: Note that this violates "common sense" and should be fixed.
		str* res = FB_NEW_RPT(*pPool, new_len+1) str;
		res->str_length = new_len;
		memcpy(res->str_data, s->str_data, s->str_length+1);
		str* old = s;
		s = res;
		delete old;
		return s != 0;
	}
};
typedef str *STR;


//
// Transaction element block
//
typedef struct teb {
	ATT *teb_database;
	int teb_tpb_length;
	UCHAR *teb_tpb;
} TEB;

/* Blocking Thread Block */

class btb : public pool_alloc<type_btb>
{
    public:
	btb *btb_next;
	struct thread* btb_thread_id;
};
typedef btb *BTB;

/* Lock levels */

#define LCK_none        0
#define LCK_null        1
#define LCK_SR          2
#define LCK_PR          3
#define LCK_SW          4
#define LCK_PW          5
#define LCK_EX          6

#define LCK_read        LCK_PR
#define LCK_write       LCK_EX

#define LCK_WAIT        TRUE
#define LCK_NO_WAIT     FALSE

/* Lock query data aggregates */

#define LCK_MIN		1
#define LCK_MAX		2
#define LCK_CNT		3
#define LCK_SUM		4
#define LCK_AVG		5
#define LCK_ANY		6


/* Window block for loading cached pages into */

typedef struct win {
	SLONG win_page;
	struct pag *win_buffer;
	struct exp_index_buf *win_expanded_buffer;
	struct bdb *win_bdb;
	SSHORT win_scans;
	USHORT win_flags;
} WIN;

#define	WIN_large_scan		1	/* large sequential scan */
#define WIN_secondary		2	/* secondary stream */
#define	WIN_garbage_collector	4	/* garbage collector's window */
#define WIN_garbage_collect	8	/* scan left a page for garbage collector */


/* define used for journaling start transaction */

#define MOD_START_TRAN  100


/* Thread specific database block */

typedef struct tdbb
{
	struct thdd	tdbb_thd_data;
	dbb*	tdbb_database;
	att*	tdbb_attachment;
	struct jrd_tra*	tdbb_transaction;
	struct jrd_req*	tdbb_request;
	JrdMemoryPool*	tdbb_default;
	ISC_STATUS*	tdbb_status_vector;
	void*		tdbb_setjmp;
	USHORT		tdbb_inhibit;		/* Inhibit context switch if non-zero */
	SSHORT		tdbb_quantum;		/* Cycles remaining until voluntary schedule */
	USHORT		tdbb_flags;
	struct iuo	tdbb_mutexes;
	struct iuo	tdbb_rw_locks;
	struct iuo	tdbb_pages;

    // ansi c want's sigsetjmp to be a different type. So I've used that
    // for unix implementations. MOD 12-July-2002
#ifdef WIN_NT
	void*		tdbb_sigsetjmp;
#else
    sigjmp_buf tdbb_sigsetjmp;
#endif
} *TDBB;

#define	TDBB_sweeper			1	/* Thread sweeper or garbage collector */
#define TDBB_no_cache_unwind	2	/* Don't unwind page buffer cache */
#define TDBB_prc_being_dropped	4	/* Dropping a procedure  */

/* List of internal database handles */

typedef struct ihndl
{
	struct ihndl*	ihndl_next;
	void*			ihndl_object;
} *IHNDL;


/* Threading macros */

#ifdef GET_THREAD_DATA
#undef GET_THREAD_DATA
#endif

#ifdef V4_THREADING
#define PLATFORM_GET_THREAD_DATA ((TDBB) THD_get_specific())
#endif

/* RITTER - changed HP10 to HPUX in the expression below */
#ifdef MULTI_THREAD
#if (defined SOLARIS_MT || defined WIN_NT || \
	defined HPUX || defined LINUX || defined DARWIN || defined FREEBSD )
#define PLATFORM_GET_THREAD_DATA ((TDBB) THD_get_specific())
#endif
#endif

#ifndef PLATFORM_GET_THREAD_DATA

#ifdef __cplusplus
extern "C" {
#endif
extern TDBB gdbb;
#ifdef __cplusplus
}
#endif
#define PLATFORM_GET_THREAD_DATA (gdbb)
#endif

/* Define GET_THREAD_DATA off the platform specific version.
 * If we're in DEV mode, also do consistancy checks on the
 * retrieved memory structure.  This was originally done to
 * track down cases of no "PUT_THREAD_DATA" on the NLM. 
 *
 * This allows for NULL thread data (which might be an error by itself)
 * If there is thread data, 
 * AND it is tagged as being a TDBB.
 * AND it has a non-NULL tdbb_database field, 
 * THEN we validate that the structure there is a database block.
 * Otherwise, we return what we got.
 * We can't always validate the database field, as during initialization
 * there is no tdbb_database set up.
 */
#ifdef DEV_BUILD
#define GET_THREAD_DATA (((PLATFORM_GET_THREAD_DATA) && \
                         (((THDD)(PLATFORM_GET_THREAD_DATA))->thdd_type == THDD_TYPE_TDBB) && \
			 (((TDBB)(PLATFORM_GET_THREAD_DATA))->tdbb_database)) \
			 ? ((MemoryPool::blk_type(((TDBB)(PLATFORM_GET_THREAD_DATA))->tdbb_database) == type_dbb) \
			    ? (PLATFORM_GET_THREAD_DATA) \
			    : (BUGCHECK (147), (PLATFORM_GET_THREAD_DATA))) \
			 : (PLATFORM_GET_THREAD_DATA))
#define CHECK_DBB(dbb)   assert ((dbb) && (MemoryPool::blk_type(dbb) == type_dbb) && ((dbb)->dbb_permanent->verify_pool()))
#define CHECK_TDBB(tdbb) assert ((tdbb) && \
	(((THDD)(tdbb))->thdd_type == THDD_TYPE_TDBB) && \
	((!(tdbb)->tdbb_database)||MemoryPool::blk_type((tdbb)->tdbb_database) == type_dbb))
#else
/* PROD_BUILD */
#define GET_THREAD_DATA (PLATFORM_GET_THREAD_DATA)
#define CHECK_TDBB(tdbb)		/* nothing */
#define CHECK_DBB(dbb)			/* nothing */
#endif

#define GET_DBB         (((TDBB) (GET_THREAD_DATA))->tdbb_database)

/*-------------------------------------------------------------------------*
 * macros used to set tdbb and dbb pointers when there are not set already *
 *-------------------------------------------------------------------------*/

#define	NULL_TDBB	((TDBB) NULL)
#define	NULL_DBB	((DBB) NULL)
#define	SET_TDBB(tdbb)	if ((tdbb) == NULL_TDBB) { (tdbb) = GET_THREAD_DATA; }; CHECK_TDBB (tdbb)
#define	SET_DBB(dbb)	if ((dbb)  == NULL_DBB)  { (dbb)  = GET_DBB; }; CHECK_DBB(dbb);

#ifdef V4_THREADING
#define V4_JRD_MUTEX_LOCK(mutx)         JRD_mutex_lock (mutx)
#define V4_JRD_MUTEX_UNLOCK(mutx)       JRD_mutex_unlock (mutx)
#define V4_JRD_RW_LOCK_LOCK(wlck,type)  JRD_wlck_lock (wlck, type)
#define V4_JRD_RW_LOCK_UNLOCK(wlck)     JRD_wlck_unlock (wlck)
// BRS. 03/23/2003
// Those empty defines was substituted with #ifdef V4_THREADING
//#else
//#define V4_JRD_MUTEX_LOCK(mutx)
//#define V4_JRD_MUTEX_UNLOCK(mutx)
//#define V4_JRD_RW_LOCK_LOCK(wlck,type)
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
extern IHNDL internal_db_handles;

#endif /* REQUESTER */


/* Define the xxx_THREAD_DATA macros.  These are needed in the whole 
   component, but they are defined differently for use in jrd.c (JRD_MAIN)
   Here we have a function which sets some flags, and then calls THD_put_specific
   so in this case we define the macro as calling that function. */
#ifndef JRD_MAIN

#ifdef __cplusplus

#define SET_THREAD_DATA		tdbb = &thd_context;\
				MOVE_CLEAR (tdbb, sizeof (*tdbb));\
				THD_put_specific (reinterpret_cast<struct thdd*>(tdbb));\
				tdbb->tdbb_thd_data.thdd_type = THDD_TYPE_TDBB

#else

#define SET_THREAD_DATA		tdbb = &thd_context;\
				MOVE_CLEAR (tdbb, sizeof (*tdbb));\
				THD_put_specific((struct thdd*)tdbb);\
				tdbb->tdbb_thd_data.thdd_type = THDD_TYPE_TDBB
#endif /* __cplusplus */

#define RESTORE_THREAD_DATA	THD_restore_specific()

#endif /* !JRD_MAIN */



#endif /* JRD_JRD_H */
