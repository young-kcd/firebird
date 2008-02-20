/*
 *      PROGRAM:        JRD access method
 *      MODULE:         Database.h
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
 * Claudio Valderrama C.
 *
 */


#ifndef JRD_DATABASE_H
#define JRD_DATABASE_H
 
#include "firebird.h"
#include "../jrd/cch.h"
#include "../jrd/que.h"

#include "../jrd/gdsassert.h"
#include "../jrd/common.h"
#include "../jrd/dsc.h"
#include "../jrd/all.h"
#include "../jrd/btn.h"
#include "../jrd/all.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/val.h"
#if defined(UNIX) && defined(SUPERSERVER)
#include <setjmp.h>
#endif

#include "../common/classes/fb_atomic.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/array.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/stack.h"
#include "../common/classes/timestamp.h"
#include "../common/classes/GenericMap.h"
#include "../jrd/RandomGenerator.h"
#include "../jrd/os/guid.h"
#include "../jrd/sbm.h"
#include "../jrd/flu.h"
#include "../jrd/RuntimeStatistics.h"



class CharSetContainer;
 
namespace Jrd
{

	class Trigger;
	template <typename T> class vec;
	class jrd_prc;
	class jrd_rel;
	class Shadow;
	class BlobFilter;
	//class PageControl;
	class TxPageCache;
	class BackupManager;
	class vcl;
 
	typedef Firebird::ObjectsArray<Trigger> trig_vec;


class Database : public pool_alloc<type_dbb>
{
	class Sync
	{
	public:
		Sync()
			: threadId(0), isAst(false)
		{}

		~Sync()
		{
			if (threadId)
			{
				syncMutex.leave();
				threadId = 0;
			}
		}

		void lock(bool ast = false)
		{
			const FB_THREAD_ID currentId = getThreadId();

			stateMutex.enter();

			if (threadId != currentId)
			{
				stateMutex.leave();
				++waiters;
				syncMutex.enter();
				--waiters;
				stateMutex.enter();
				threadId = currentId;
				isAst = ast;
			}

			stateMutex.leave();
		}

		void unlock(bool checkout = false)
		{
			Firebird::MutexLockGuard guard(stateMutex);

			fb_assert(threadId == getThreadId());

			if (!(isAst && checkout))
			{
				threadId = 0;
				syncMutex.leave();
			}
		}

		bool hasContention() const
		{
			return (waiters.value() > 0);
		}

	private:
		// copying is prohibited
		Sync(const Sync&);
		Sync& operator=(const Sync&);

		Firebird::Mutex syncMutex;
		Firebird::Mutex stateMutex;
		Firebird::AtomicCounter waiters;
		FB_THREAD_ID threadId;
		bool isAst;
	};

public:

	class SyncGuard
	{
	public:
		SyncGuard(Database* arg, bool ast = false)
			: dbb(*arg)
		{
			dbb.dbb_sync.lock(ast);
		}

		~SyncGuard()
		{
			try
			{
				dbb.dbb_sync.unlock();
			}
			catch (const Firebird::Exception&)
			{
				Firebird::MutexLockGuard::onDtorException();
			}
		}

	private:
		// copying is prohibited
		SyncGuard(const SyncGuard&);
		SyncGuard& operator=(const SyncGuard&);

		Database& dbb;
	};

	class Checkout
	{
	public:
		explicit Checkout(Database* arg, bool flag = false)
			: dbb(*arg), io(flag)
		{
#ifndef SUPERSERVER
			if (!io)
#endif
				dbb.checkout();
		}

		~Checkout()
		{
#ifndef SUPERSERVER
			if (!io)
#endif
				dbb.checkin();
		}

	private:
		// copying is prohibited
		Checkout(const Checkout&);
		Checkout& operator=(const Checkout&);

		Database& dbb;
		const bool io;
	};
	
	class CheckoutLockGuard
	{
	public:
		CheckoutLockGuard(Database* dbb, Firebird::Mutex& m)
			: mutex(m)
		{
			Checkout dcoHolder(dbb);
			mutex.enter();
		}

		~CheckoutLockGuard()
		{
			try {
				mutex.leave();
			}
			catch (const Firebird::Exception&)
			{
				Firebird::MutexLockGuard::onDtorException();
			}
		}

	private:
		// copying is prohibited
		CheckoutLockGuard(const CheckoutLockGuard&);
		CheckoutLockGuard& operator=(const CheckoutLockGuard&);

		Firebird::Mutex& mutex;
	};

	typedef int (*crypt_routine) (const char*, void*, int, void*);

	static Database* newDbb(MemoryPool* p) {
		return FB_NEW(*p) Database(p);
	}

	// The deleteDbb function MUST be used to delete a Database object.
	// The function hides some tricky order of operations.  Since the
	// memory for the vectors in the Database is allocated out of the Database's
	// permanent memory pool, the entire delete() operation needs
	// to complete _before_ the permanent pool is deleted, or else
	// risk an aborted engine.
	static void deleteDbb(Database* const toDelete)
	{
		if (!toDelete)
			return;

		MemoryPool* const perm = toDelete->dbb_permanent;

		// Memory pool destruction below decrements memory statistics
		// situated in database block we are about to deallocate right now
		Firebird::MemoryStats temp_stats;
		perm->setStatsGroup(temp_stats);

		delete toDelete;
		MemoryPool::deletePool(perm);
	}

	static ULONG getLockOwnerId()
	{
		static Firebird::AtomicCounter counter;
		return ++counter;
	}

	mutable Sync dbb_sync;				// Database sync primitive

	Database*	dbb_next;				// Next database block in system
	Attachment* dbb_attachments;		// Active attachments
	BufferControl*	dbb_bcb;			// Buffer control block
	vec<jrd_rel*>*	dbb_relations;		// relation vector
	vec<jrd_prc*>*	dbb_procedures;		// scanned procedures
	FB_GUID		dbb_guid;				// dbb instance identifier
	Lock*		dbb_instance_lock;		// dbb instance lock
	Lock* 		dbb_lock;				// granddaddy lock
	jrd_tra*	dbb_sys_trans;			// system transaction
//	jrd_file*	dbb_file;				// files for I/O operations
	Shadow*		dbb_shadow;				// shadow control block
	Lock*		dbb_shadow_lock;		// lock for synchronizing addition of shadows
	//SLONG dbb_shadow_sync_count;		// to synchronize changes to shadows
	Lock*		dbb_retaining_lock;		// lock for preserving commit retaining snapshot
	Lock*		dbb_monitor_lock;		// lock for monitoring purposes
	PageManager dbb_page_manager;
	vcl*		dbb_t_pages;			// pages number for transactions
	vcl*		dbb_gen_id_pages;		// known pages for gen_id
	BlobFilter*	dbb_blob_filters;		// known blob filters
	trig_vec*	dbb_triggers[DB_TRIGGER_MAX];

	DatabaseModules	dbb_modules;		// external function/filter modules
	Firebird::Mutex* dbb_mutexes;		// Database block mutexes
	Firebird::Mutex dbb_sp_rec_mutex;	// Mutex for accessing/updating stored procedure metadata
	//SLONG dbb_sort_size;				// Size of sort space per sort, unused for now

	UATOM dbb_ast_flags;				// flags modified at AST level
	ULONG dbb_flags;
	USHORT dbb_ods_version;				// major ODS version number
	USHORT dbb_minor_version;			// minor ODS version number
	USHORT dbb_minor_original;			// minor ODS version at creation
	USHORT dbb_page_size;				// page size
	USHORT dbb_dp_per_pp;				// data pages per pointer page
	USHORT dbb_max_records;				// max record per data page
	USHORT dbb_max_idx;					// max number of indexes on a root page
	USHORT dbb_use_count;				// active count of threads
	USHORT dbb_shutdown_delay;			// seconds until forced shutdown.
										// Set in shut.cpp but not tested yet.
#ifdef SUPERSERVER_V2
	USHORT dbb_prefetch_sequence;		// sequence to pace frequency of prefetch requests
	USHORT dbb_prefetch_pages;			// prefetch pages per request
#endif
	Firebird::PathName dbb_filename;	// filename string
	Firebird::PathName dbb_database_name;	// database ID (file name or alias)
	Firebird::string dbb_encrypt_key;	// encryption key

	MemoryPool* dbb_permanent;
	MemoryPool* dbb_bufferpool;

	Firebird::Array<MemoryPool*> dbb_pools;		// pools

	vec<jrd_req*>*	dbb_internal;		// internal requests
	vec<jrd_req*>*	dbb_dyn_req;		// internal dyn requests

	SLONG dbb_oldest_active;			// Cached "oldest active" transaction
	SLONG dbb_oldest_transaction;		// Cached "oldest interesting" transaction
	SLONG dbb_oldest_snapshot;			// Cached "oldest snapshot" of all active transactions
	SLONG dbb_next_transaction;			// Next transaction id used by NETWARE
	SLONG dbb_attachment_id;			// Next attachment id for ReadOnly DB's
	SLONG dbb_page_incarnation;			// Cache page incarnation counter
	ULONG dbb_page_buffers;				// Page buffers from header page

	event_t dbb_writer_event[1];		// Event to wake up cache writer
	event_t dbb_writer_event_init[1];	// Event for initialization cache writer
	event_t dbb_writer_event_fini[1];	// Event for finalization cache writer
#ifdef SUPERSERVER_V2
	event_t dbb_reader_event[1];		// Event to wake up cache reader
#endif

#ifdef GARBAGE_THREAD
	event_t dbb_gc_event[1];			// Event to wake up garbage collector
	event_t dbb_gc_event_init[1];		// Event for initialization garbage collector
	event_t dbb_gc_event_fini[1];		// Event for finalization garbage collector
#endif

	ULONG dbb_current_id;				// Generator of dbb-local ids
	Firebird::MemoryStats dbb_memory_stats;

	SLONG dbb_reads;
	SLONG dbb_writes;
	SLONG dbb_fetches;
	SLONG dbb_marks;
	RuntimeStatistics dbb_stats;
	SLONG dbb_last_header_write;		// Transaction id of last header page physical write
	SLONG dbb_flush_cycle;				// Current flush cycle
	SLONG dbb_sweep_interval;			// Transactions between sweep
	const ULONG dbb_lock_owner_id;		// ID for the lock manager
	SLONG dbb_lock_owner_handle;		// Handle for the lock manager

	USHORT unflushed_writes;			// unflushed writes
	time_t last_flushed_write;			// last flushed write time

	crypt_routine dbb_encrypt;			// External encryption routine
	crypt_routine dbb_decrypt;			// External decryption routine

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	class blb_map *dbb_blob_map;		// mapping of blobs for REPLAY
	struct log *dbb_log;				// log file for REPLAY
#endif

	Firebird::Array<CharSetContainer*>		dbb_charsets;	// intl character set descriptions
	TxPageCache*	dbb_tip_cache;		// cache of latest known state of all transactions in system
	vcl*		dbb_pc_transactions;	// active precommitted transactions
	BackupManager*	dbb_backup_manager;	// physical backup manager
	Firebird::TimeStamp dbb_creation_date; // creation date
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, UserFunction*> > > dbb_functions;	// User defined functions

	ULONG generateId()
	{
		return ++dbb_current_id;
	}

	void checkin() const
	{
		dbb_sync.lock();
	}

	void checkout() const
	{
		dbb_sync.unlock(true);
	}

	// returns true if primary file is located on raw device
	bool onRawDevice() const;

	MemoryPool* createPool()
	{
#ifdef SUPERSERVER
		MemoryPool* pool = MemoryPool::createPool(dbb_permanent, dbb_memory_stats);
#else
		MemoryPool* pool = MemoryPool::createPool(dbb_permanent);
#endif
		dbb_pools.add(pool);
		return pool;
	}

	void deletePool(MemoryPool* pool);

private:
	explicit Database(MemoryPool* p)
	:	dbb_page_manager(*p),
		dbb_modules(*p),
		dbb_filename(*p),
		dbb_database_name(*p),
		dbb_encrypt_key(*p),
		dbb_permanent(p),
		dbb_pools(*p, 4),
		dbb_lock_owner_id(getLockOwnerId()),
		dbb_charsets(*p),
		dbb_functions(*p)
	{
		dbb_pools.add(p);
	}

	~Database();

public:
	// temporary measure to avoid unstable state of lock file -
	// this is anyway called in ~Database(), and in theory should be private
	void destroyIntlObjects();			// defined in intl.cpp

private:

	// The delete operators are no-oped because the Database memory is allocated from the
	// Database's own permanent pool.  That pool has already been released by the Database
	// destructor, so the memory has already been released.  Hence the operator
	// delete no-op.
	void operator delete(void *mem) {}
	void operator delete[](void *mem) {}

	Database(const Database&);			// no impl.
	const Database& operator =(const Database&) { return *this; }
};

//
// bit values for dbb_flags
//
const ULONG DBB_damaged				= 0x1L;
const ULONG DBB_exclusive			= 0x2L;		// Database is accessed in exclusive mode
const ULONG DBB_bugcheck			= 0x4L;		// Bugcheck has occurred
#ifdef GARBAGE_THREAD
const ULONG DBB_garbage_collector	= 0x8L;		// garbage collector thread exists
const ULONG DBB_gc_active			= 0x10L;	// ... and is actively working.
const ULONG DBB_gc_pending			= 0x20L;	// garbage collection requested
#endif
const ULONG DBB_force_write			= 0x40L;	// Database is forced write
const ULONG DBB_no_reserve			= 0x80L;	// No reserve space for versions
const ULONG DBB_DB_SQL_dialect_3	= 0x100L;	// database SQL dialect 3
const ULONG DBB_read_only			= 0x200L;	// DB is ReadOnly (RO). If not set, DB is RW
const ULONG DBB_being_opened_read_only	= 0x400L;	// DB is being opened RO. If unset, opened as RW
const ULONG DBB_not_in_use			= 0x800L;	// Database to be ignored while attaching
const ULONG DBB_lck_init_done		= 0x1000L;	// LCK_init() called for the database
const ULONG DBB_sweep_in_progress	= 0x2000L;	// A database sweep operation is in progress
const ULONG DBB_security_db			= 0x4000L;	// ISC security database
const ULONG DBB_suspend_bgio		= 0x8000L;	// Suspend I/O by background threads
const ULONG DBB_being_opened		= 0x10000L;	// database is being attached to
const ULONG DBB_gc_cooperative		= 0x20000L;	// cooperative garbage collection
const ULONG DBB_gc_background		= 0x40000L;	// background garbage collection by gc_thread
const ULONG DBB_no_fs_cache			= 0x80000L;	// Not using file system cache

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
const UATOM DBB_monitor_off			= 0x400L;	// Database has the monitoring lock released

} // namespace Jrd

#endif // JRD_DATABASE_H
