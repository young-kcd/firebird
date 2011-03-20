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
#include "../jrd/blb.h"
#include "../jrd/scl.h"
#include "../jrd/PreparedStatement.h"
#include "../jrd/RandomGenerator.h"
#include "../jrd/RuntimeStatistics.h"

#include "../common/classes/ByteChunk.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/PublicHandle.h"
#include "../common/classes/stack.h"
#include "../common/classes/timestamp.h"

#include "ProviderInterface.h"
namespace Jrd
{
	typedef Firebird::Status Status;
}

namespace EDS {
	class Connection;
}

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


//
// the attachment block; one is created for each attachment to a database
//
class Attachment : public Firebird::PublicHandle,
				   public Firebird::StdIface<Firebird::IAttachment, FB_I_ATTACHMENT_VERSION, pool_alloc<type_att> >
{
public:
	static Attachment* create(Database* dbb, FB_API_HANDLE publicHandle);
	static void destroy(Attachment* const attachment);

	MemoryPool* const att_pool;					// Memory pool
	Firebird::MemoryStats att_memory_stats;

	Database*	att_database;				// Parent database block
	FB_API_HANDLE att_public_handle;		// Public handle
	Attachment*	att_next;					// Next attachment to database
	UserId*		att_user;					// User identification
	jrd_tra*	att_transactions;			// Transactions belonging to attachment
	jrd_tra*	att_dbkey_trans;			// transaction to control db-key scope

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
	Firebird::Mutex att_mutex;				// attachment mutex
	bool att_in_use;						// attachment in use (can't be detached or dropped)

	EDS::Connection* att_ext_connection;	// external connection executed by this attachment
	ULONG att_ext_call_depth;				// external connection call depth, 0 for user attachment
	TraceManager* att_trace_manager;		// Trace API manager

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

	bool checkHandle() const;

public:
	virtual int FB_CARG release();
	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer);
//	virtual Firebird::ITransaction* startTransaction(Status* status, unsigned int tpbLength, const unsigned char* tpb);
// second form is tmp - not to rewrite external engines right now
	virtual Firebird::ITransaction* FB_CARG startTransaction(Status* status, unsigned int tpbLength, const unsigned char* tpb,
													 FB_API_HANDLE api);
	virtual Firebird::ITransaction* FB_CARG reconnectTransaction(Status* status, unsigned int length, const unsigned char* id);
	virtual Firebird::IStatement* FB_CARG allocateStatement(Status* status);
	virtual Firebird::IRequest* FB_CARG compileRequest(Status* status, unsigned int blr_length, const unsigned char* blr);
	virtual Firebird::ITransaction* FB_CARG execute(Status* status, Firebird::ITransaction* transaction,
								 unsigned int length, const char* string, unsigned int dialect,
								 unsigned int in_msg_type, const Firebird::MessageBuffer* inMsgBuffer,
								 const Firebird::MessageBuffer* outMsgBuffer);
	virtual Firebird::IEvents* FB_CARG queEvents(Status* status, Firebird::EventCallback* callback,
										 unsigned int length, const unsigned char* events);
	virtual void FB_CARG cancelOperation(Status* status, int option);
	virtual void FB_CARG ping(Status* status);
	virtual void FB_CARG detach(Status* status);
	virtual void FB_CARG drop(Status* status);

private:
	Attachment(MemoryPool* pool, Database* dbb, FB_API_HANDLE publicHandle);
	~Attachment();
};


// Attachment flags

const ULONG ATT_no_cleanup			= 1;	// Don't expunge, purge, or garbage collect
const ULONG ATT_shutdown			= 2;	// attachment has been shutdown
const ULONG ATT_purge_error			= 4;	// trouble happened in purge attachment, att_mutex remains locked
const ULONG ATT_shutdown_manager	= 8;	// attachment requesting shutdown
const ULONG ATT_lck_init_done		= 16;	// LCK_init() called for the attachment
const ULONG ATT_exclusive			= 32;	// attachment wants exclusive database access
const ULONG ATT_attach_pending		= 64;	// Indicate attachment is only pending
const ULONG ATT_exclusive_pending	= 128;	// Indicate exclusive attachment pending
const ULONG ATT_gbak_attachment		= 256;	// Indicate GBAK attachment

#ifdef GARBAGE_THREAD
const ULONG ATT_notify_gc			= 1024;	// Notify garbage collector to expunge, purge ..
const ULONG ATT_disable_notify_gc	= 2048;	// Temporarily perform own garbage collection
const ULONG ATT_garbage_collector	= 4096;	// I'm a garbage collector

const ULONG ATT_NO_CLEANUP			= (ATT_no_cleanup | ATT_notify_gc);
#else
const ULONG ATT_NO_CLEANUP			= ATT_no_cleanup;
#endif

const ULONG ATT_cancel_raise		= 8192;		// Cancel currently running operation
const ULONG ATT_cancel_disable		= 16384;	// Disable cancel operations
const ULONG ATT_gfix_attachment		= 32768;	// Indicate a GFIX attachment
const ULONG ATT_gstat_attachment	= 65536;	// Indicate a GSTAT attachment
const ULONG ATT_no_db_triggers		= 131072;	// Don't execute database triggers


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

inline bool Attachment::checkHandle() const
{
	if (!isKnownHandle())
	{
		return false;
	}

	return TypedHandle<type_att>::checkHandle();
}


} // namespace Jrd

#endif // JRD_ATTACHMENT_H
