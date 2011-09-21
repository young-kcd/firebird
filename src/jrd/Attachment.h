/*
 *      PROGRAM:        JRD access method
 *      MODULE:         Attachment.h
 *      DESCRIPTION:    JRD Attachment class
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
 */

#ifndef JRD_ATTACHMENT_H
#define JRD_ATTACHMENT_H

#include "firebird.h"
#include "../common/common.h"
// Definition of block types for data allocation in JRD
#include "../include/fb_blk.h"
#include "../jrd/scl.h"
#include "../jrd/PreparedStatement.h"
#include "../jrd/RandomGenerator.h"
#include "../jrd/RuntimeStatistics.h"

#include "../common/classes/ByteChunk.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/stack.h"
#include "../common/classes/timestamp.h"

#include "../jrd/EngineInterface.h"

namespace EDS {
	class Connection;
}

class CharSetContainer;

namespace Jrd
{
	class thread_db;
	class Database;
	class jrd_tra;
	class jrd_req;
	class Lock;
	class jrd_file;
	class Format;
	class BufferControl;
	class SparseBitmap;
	class jrd_rel;
	class ExternalFile;
	class ViewContext;
	class IndexBlock;
	class IndexLock;
	class ArrayField;
	struct sort_context;
	class vcl;
	class TextType;
	class Parameter;
	class jrd_fld;
	class dsql_dbb;
	class PreparedStatement;
	class TraceManager;
	template <typename T> class vec;
	class jrd_rel;
	class jrd_prc;
	class Trigger;
	typedef Firebird::ObjectsArray<Trigger> trig_vec;
	class Function;
	class JrdStatement;

struct DSqlCacheItem
{
	Lock* lock;
	bool locked;
	bool obsolete;
};

typedef Firebird::GenericMap<Firebird::Pair<Firebird::Left<
	Firebird::string, DSqlCacheItem> > > DSqlCache;


struct DdlTriggerContext
{
	DdlTriggerContext()
		: eventType(*getDefaultMemoryPool()),
		  objectType(*getDefaultMemoryPool()),
		  objectName(*getDefaultMemoryPool()),
		  sqlText(*getDefaultMemoryPool())
	{
	}

	Firebird::string eventType;
	Firebird::string objectType;
	Firebird::MetaName objectName;
	Firebird::string sqlText;
};


//
// Database record counters.
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

struct bid;

//
// the attachment block; one is created for each attachment to a database
//
class Attachment : public pool_alloc<type_att>
{
public:
	class SyncGuard
	{
	public:
		explicit SyncGuard(Attachment* att, bool optional = false)
			: m_mutex(NULL)
		{
			if (att && att->att_interface)
				m_mutex = att->att_interface->getMutex();

			fb_assert(optional || m_mutex);

			if (m_mutex)
				m_mutex->enter();
		}

		~SyncGuard()
		{
			if (m_mutex)
				m_mutex->leave();
		}

	private:
		// copying is prohibited
		SyncGuard(const SyncGuard&);
		SyncGuard& operator=(const SyncGuard&);

		Firebird::Mutex* m_mutex;
	};

	class Checkout
	{
	public:
		explicit Checkout(Attachment* att, bool optional = false)
			: m_mutex(NULL)
		{
			if (att && att->att_interface)
			{
				m_ref = att->att_interface;
				m_mutex = att->att_interface->getMutex();
			}

			fb_assert(optional || m_mutex);

			if (m_mutex)
				m_mutex->leave();
		}

		~Checkout()
		{
			if (m_mutex)
				m_mutex->enter();
		}

	private:
		// copying is prohibited
		Checkout(const Checkout&);
		Checkout& operator=(const Checkout&);

		Firebird::Mutex* m_mutex;
		Firebird::RefPtr<JAttachment> m_ref;
	};

	class CheckoutLockGuard
	{
	public:
		CheckoutLockGuard(Attachment* att, Firebird::Mutex& mutex, bool optional = false)
			: m_mutex(mutex)
		{
			if (!m_mutex.tryEnter())
			{
				Checkout attCout(att, optional);
				m_mutex.enter();
			}
		}

		~CheckoutLockGuard()
		{
			m_mutex.leave();
		}

	private:
		// copying is prohibited
		CheckoutLockGuard(const CheckoutLockGuard&);
		CheckoutLockGuard& operator=(const CheckoutLockGuard&);

		Firebird::Mutex& m_mutex;
	};

public:
	static Attachment* create(Database* dbb);
	static void destroy(Attachment* const attachment);

	MemoryPool* const att_pool;					// Memory pool
	Firebird::MemoryStats att_memory_stats;

	Database*	att_database;				// Parent database block
	Attachment*	att_next;					// Next attachment to database
	UserId*		att_user;					// User identification
	jrd_tra*	att_transactions;			// Transactions belonging to attachment
	jrd_tra*	att_dbkey_trans;			// transaction to control db-key scope
	SLONG		att_oldest_snapshot;		// GTT's record versions older than this can be gargage-collected

private:
	jrd_tra*	att_sys_transaction;		// system transaction

public:
	Firebird::SortedArray<jrd_req*> att_requests;	// Requests belonging to attachment
	Lock*		att_id_lock;				// Attachment lock (if any)
	SLONG		att_attachment_id;			// Attachment ID
	const ULONG	att_lock_owner_id;			// ID for the lock manager
	SLONG		att_lock_owner_handle;		// Handle for the lock manager
	ULONG		att_backup_state_counter;	// Counter of backup state locks for attachment
	SLONG		att_event_session;			// Event session id, if any
	SecurityClass*	att_security_class;		// security class for database
	SecurityClassList*	att_security_classes;	// security classes
	vcl*		att_counts[DBB_max_count];
	RuntimeStatistics	att_stats;
	ULONG		att_flags;					// Flags describing the state of the attachment
	SSHORT		att_client_charset;			// user's charset specified in dpb
	SSHORT		att_charset;				// current (client or external) attachment charset
	Lock*		att_long_locks;				// outstanding two phased locks
	Lock*		att_wait_lock;				// lock at which attachment waits currently
	vec<Lock*>*	att_compatibility_table;	// hash table of compatible locks
	vcl*		att_val_errors;
	Firebird::PathName	att_working_directory;	// Current working directory is cached
	Firebird::PathName	att_filename;			// alias used to attach the database
	const Firebird::TimeStamp	att_timestamp;	// Connection date and time
	Firebird::StringMap att_context_vars;	// Context variables for the connection
	Firebird::Stack<DdlTriggerContext> ddlTriggersContext;	// Context variables for DDL trigger event
	Firebird::string att_network_protocol;	// Network protocol used by client for connection
	Firebird::string att_remote_address;	// Protocol-specific addess of remote client
	SLONG att_remote_pid;					// Process id of remote client
	Firebird::PathName att_remote_process;	// Process name of remote client
	RandomGenerator att_random_generator;	// Random bytes generator
	Lock*		att_temp_pg_lock;			// temporary pagespace ID lock
	DSqlCache att_dsql_cache;	// DSQL cache locks
	Firebird::SortedArray<void*> att_udf_pointers;
	dsql_dbb* att_dsql_instance;
	bool att_in_use;						// attachment in use (can't be detached or dropped)
	int att_use_count;						// number of API calls running except of asyncronous ones

	EDS::Connection* att_ext_connection;	// external connection executed by this attachment
	ULONG att_ext_call_depth;				// external connection call depth, 0 for user attachment
	TraceManager* att_trace_manager;		// Trace API manager

	JAttachment* att_interface;
	Firebird::IAttachment* att_public_interface;

	/// former Database members - start

	vec<jrd_rel*>*					att_relations;			// relation vector
	vec<jrd_prc*>*					att_procedures;			// scanned procedures
	trig_vec*						att_triggers[DB_TRIGGER_MAX];
	trig_vec*						att_ddl_triggers;
	Firebird::Array<Function*>		att_functions;			// User defined functions

	Firebird::Array<JrdStatement*>	att_internal;			// internal statements
	Firebird::Array<JrdStatement*>	att_dyn_req;			// internal dyn statements

	jrd_req* findSystemRequest(thread_db* tdbb, USHORT id, USHORT which);

	Firebird::Array<CharSetContainer*>	att_charsets;		// intl character set descriptions
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, USHORT> > > att_charset_ids;	// Character set ids

	void releaseIntlObjects();			// defined in intl.cpp
	void destroyIntlObjects();			// defined in intl.cpp

	// from CMP_shutdown_database and CMP_fini
	void releaseLocks(thread_db* tdbb);

	Firebird::Array<MemoryPool*>	att_pools;		// pools

	MemoryPool* createPool();
	void deletePool(MemoryPool* pool);

	/// former Database members - end

	bool locksmith() const;
	jrd_tra* getSysTransaction();
	void setSysTransaction(jrd_tra* trans);	// used only by TRA_init

	PreparedStatement* prepareStatement(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::string& text, Firebird::MemoryPool* pool = NULL);
	PreparedStatement* prepareStatement(thread_db* tdbb, jrd_tra* transaction,
		const PreparedStatement::Builder& builder, Firebird::MemoryPool* pool = NULL);

	PreparedStatement* prepareUserStatement(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::string& text, Firebird::MemoryPool* pool = NULL);

	Firebird::MetaName nameToMetaCharSet(thread_db* tdbb, const Firebird::MetaName& name);
	Firebird::MetaName nameToUserCharSet(thread_db* tdbb, const Firebird::MetaName& name);
	Firebird::string stringToMetaCharSet(thread_db* tdbb, const Firebird::string& str,
		const char* charSet = NULL);
	Firebird::string stringToUserCharSet(thread_db* tdbb, const Firebird::string& str);

	void storeMetaDataBlob(thread_db* tdbb, jrd_tra* transaction, bid* blobId,
		const Firebird::string& text)
	{
		storeMetaDataBlob(tdbb, transaction, blobId, text, att_charset);
	}

	void storeMetaDataBlob(thread_db* tdbb, jrd_tra* transaction,
		bid* blobId, const Firebird::string& text, USHORT fromCharSet);
	void storeBinaryBlob(thread_db* tdbb, jrd_tra* transaction, bid* blobId,
		const Firebird::ByteChunk& chunk);

	void cancelExternalConnection(thread_db* tdbb);
	void detachLocksFromAttachment();

	bool backupStateWriteLock(thread_db* tdbb, SSHORT wait);
	void backupStateWriteUnLock(thread_db* tdbb);
	bool backupStateReadLock(thread_db* tdbb, SSHORT wait);
	void backupStateReadUnLock(thread_db* tdbb);

private:
	Attachment(MemoryPool* pool, Database* dbb);
	~Attachment();
};


// Attachment flags

const ULONG ATT_no_cleanup			= 0x0001L;	// Don't expunge, purge, or garbage collect
const ULONG ATT_shutdown			= 0x0002L;	// attachment has been shutdown
const ULONG ATT_purge_error			= 0x0004L;	// trouble happened in purge attachment, att_mutex remains locked
const ULONG ATT_shutdown_manager	= 0x0008L;	// attachment requesting shutdown
const ULONG ATT_lck_init_done		= 0x0010L;	// LCK_init() called for the attachment
const ULONG ATT_exclusive			= 0x0020L;	// attachment wants exclusive database access
const ULONG ATT_attach_pending		= 0x0040L;	// Indicate attachment is only pending
const ULONG ATT_exclusive_pending	= 0x0080L;	// Indicate exclusive attachment pending
const ULONG ATT_gbak_attachment		= 0x0100L;	// Indicate GBAK attachment
const ULONG ATT_notify_gc			= 0x0200L;	// Notify garbage collector to expunge, purge ..
const ULONG ATT_garbage_collector	= 0x0400L;	// I'm a garbage collector
const ULONG ATT_cancel_raise		= 0x0800L;	// Cancel currently running operation
const ULONG ATT_cancel_disable		= 0x1000L;	// Disable cancel operations
const ULONG ATT_gfix_attachment		= 0x2000L;	// Indicate a GFIX attachment
const ULONG ATT_gstat_attachment	= 0x4000L;	// Indicate a GSTAT attachment
const ULONG ATT_no_db_triggers		= 0x8000L;	// Don't execute database triggers

const ULONG ATT_NO_CLEANUP			= (ATT_no_cleanup | ATT_notify_gc);


inline bool Attachment::locksmith() const
{
	return att_user && att_user->locksmith();
}

inline jrd_tra* Attachment::getSysTransaction()
{
	return att_sys_transaction;
}

inline void Attachment::setSysTransaction(jrd_tra* trans)
{
	att_sys_transaction = trans;
}

} // namespace Jrd

#endif // JRD_ATTACHMENT_H
