/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		tra.h
 *	DESCRIPTION:	Transaction block definitions
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
 * 2001.6.25 Claudio Valderrama: add dfw_delete_generator and dfw_delete_udf
 *           to the dfw_t enumeration.
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 */

#ifndef JRD_TRA_H
#define JRD_TRA_H

/*
 * TMN: Fix this header! It should include any header it needs
 * to define the types this header uses.
 */

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../common/classes/tree.h"
#include "../common/classes/GenericMap.h"
#include "../jrd/rpb_chain.h"
#include "../jrd/exe.h"
#include "../jrd/blb.h" // For bid structure
#include "../jrd/sbm.h" // For bid structure

namespace Jrd {

class blb;
class Lock;
class jrd_rel;
class vec;
class Savepoint;
class Record;
class VerbAction;
class ArrayField;
class Attachment;

// Blobs active in transaction identified by bli_temp_id. Please keep this 
// structure small as there can be huge amount of them floating in memory.
struct BlobIndex {
	ULONG bli_temp_id;
	bool bli_materialized;
	jrd_req* bli_request;
	union {
		bid bli_blob_id; // ID of materialized blob
		blb* bli_blob_object; // Blob object
	};
    static const ULONG& generate(const void *sender, const BlobIndex& item) {
		return item.bli_temp_id;
    }
	// Empty default constructor to make it behave like POD structure
	BlobIndex() {}
	BlobIndex(ULONG temp_id, blb* blob_object) : 
		bli_temp_id(temp_id), bli_materialized(false), bli_request(NULL), 
		bli_blob_object(blob_object) 
	{ }
};

typedef Firebird::BePlusTree<BlobIndex, ULONG, MemoryPool, BlobIndex> BlobIndexTree;

/* Transaction block */

class jrd_tra : public pool_alloc_rpt<SCHAR, type_tra>
{
    public:
	enum wait_t {
		tra_no_wait,
		tra_probe,
		tra_wait
	};

	jrd_tra(MemoryPool& p) : tra_blobs(&p), tra_resources(p), tra_context_vars(p) {}
	Attachment* tra_attachment;	/* database attachment */
	SLONG tra_number;			/* transaction number */
	SLONG tra_top;				/* highest transaction in snapshot */
	SLONG tra_oldest;			/* oldest interesting transaction */
	SLONG tra_oldest_active;	/* record versions older than this can be
								   gargage-collected by this tx */
	jrd_tra*	tra_next;		/* next transaction in database */
	jrd_tra*	tra_sibling;	/* next transaction in group */
	JrdMemoryPool* tra_pool;		/* pool for transaction */
	BlobIndexTree tra_blobs;		/* list of active blobs */
	ArrayField*	tra_arrays;		/* Linked list of active arrays */
	Lock*		tra_lock;		/* lock for transaction */
	vec*		tra_relation_locks;	/* locks for relations */
	UInt32Bitmap*	tra_commit_sub_trans;	/* commited sub-transactions */
	Savepoint*	tra_save_point;	/* list of savepoints  */
	SLONG tra_save_point_number;	/* next save point number to use */
	ULONG tra_flags;
#ifdef PC_ENGINE
	SLONG tra_range_id;				/* unique id of cache range within transaction */
#endif
	class DeferredWork*	tra_deferred_work;	/* work deferred to commit time */
	ResourceList tra_resources;		/* resource existence list */
	Firebird::StringMap tra_context_vars; // Context variables for the transaction
	traRpbList* tra_rpblist;	/* active record_param's of given transaction */
	UCHAR tra_use_count;		/* use count for safe AST delivery */
	UCHAR tra_callback_count;	/* callback count for 'execute statement' */
	SSHORT tra_lock_timeout;	/* in seconds, -1 means infinite, 0 means NOWAIT */
	ULONG tra_next_blob_id;     // ID of the previous blob or array created in this transaction
	UCHAR tra_transactions[1];

	SSHORT getLockWait() const
	{
		return -tra_lock_timeout;
	}
};

const ULONG TRA_system			= 1L;		/* system transaction */
const ULONG TRA_update			= 2L;		/* update is permitted */
const ULONG TRA_prepared		= 4L;		/* transaction is in limbo */
const ULONG TRA_reconnected		= 8L;		/* reconnect in progress */
const ULONG TRA_reserving		= 16L;		/* relations explicityly locked */
const ULONG TRA_degree3			= 32L;		/* serializeable transaction */
const ULONG TRA_committing		= 64L;		/* commit in progress */
const ULONG TRA_write			= 128L;		/* transaction has written */
const ULONG TRA_readonly		= 256L;		/* transaction is readonly */
//const ULONG TRA_nowait			= 512L;		/* don't wait on relations, give up */
const ULONG TRA_prepare2		= 1024L;	/* transaction has updated RDB$TRANSACTIONS */
const ULONG TRA_ignore_limbo	= 2048L;	/* ignore transactions in limbo */
const ULONG TRA_invalidated 	= 4096L;	/* transaction invalidated by failed write */
const ULONG TRA_deferred_meta 	= 8192L;	/* deferred meta work posted */
//const ULONG TRA_add_log		= 16384L;	// write ahead log file was added
//const ULONG TRA_delete_log	= 32768L;	// write ahead log file was deleted
const ULONG TRA_read_committed	= 65536L;	/* can see latest committed records */
const ULONG TRA_autocommit		= 131072L;	/* autocommits all updates */
const ULONG TRA_perform_autocommit	= 262144L;	/* indicates autocommit is necessary */
const ULONG TRA_rec_version			= 524288L;	/* don't wait for uncommitted versions */
const ULONG TRA_restart_requests	= 1048576L;	/* restart all requests in attachment */
const ULONG TRA_no_auto_undo		= 2097152L;	/* don't start a savepoint in TRA_start */
const ULONG TRA_sweep_at_startup	= 4194304L;	/* sweep done at startup */
const ULONG TRA_precommitted		= 8388608L;	/* transaction committed at startup */

const int TRA_MASK				= 3;
const int TRA_BITS_PER_TRANS	= 2;
const int TRA_TRANS_PER_BYTE	= 4;
const int TRA_SHIFT				= 2;

#define TRANS_SHIFT(number)	(((number) & TRA_MASK) << 1)
#define TRANS_OFFSET(number)	((number) >> TRA_SHIFT)

/* Transaction cleanup. If a database is never quiescent, look
   for "dead" active transactions every so often at transaction
   startup */

const int TRA_ACTIVE_CLEANUP	= 100;

/* Transaction states.  The first four are states found
   in the transaction inventory page; the last two are
   returned internally */

const int tra_active		= 0;	/* Transaction is active */
const int tra_limbo			= 1;
const int tra_dead			= 2;
const int tra_committed		= 3;
const int tra_us			= 4;	/* Transaction is us */
const int tra_precommitted	= 5;	/* Transaction is precommitted */

// The highest transaction number possible.  This is 0x7fffffff if SLONG is 32 bits.
//#define MAX_TRA_NUMBER		 (~(1L << (BITS_PER_LONG - 1)))

/* Savepoint block */

class Savepoint : public pool_alloc<type_sav>
{
    public:
	VerbAction*		sav_verb_actions;	/* verb action list */
	USHORT			sav_verb_count;		/* Active verb count */
	SLONG			sav_number;			/* save point number */
	Savepoint*		sav_next;
	USHORT			sav_flags;
	TEXT			sav_name[32]; /* Savepoint name */
};

/* Savepoint block flags. */

const int SAV_trans_level	= 1;	/* savepoint was started by TRA_start */
const int SAV_event_post	= 2;	/* event posted in the save point */
const int SAV_user			= 4;	/* named user savepoint as opposed to system ones */

/* Maximum size in bytes of transaction-level savepoint data.
  When transaction-level savepoint gets past this size we drop it and use GC
  mechanisms to clean out changes done in transaction */
const IPTR SAV_LARGE			= 1024 * 1024;	// 1 MB of savepoint undo data is A LOT:
                                                // ~25K - ~4M changed records

/* Deferred work blocks are used by the meta data handler to keep track
   of work deferred to commit time.  This are usually used to perform
   meta data updates */

enum dfw_t {
	dfw_null,
	dfw_create_relation,
	dfw_delete_relation,
	dfw_update_format,
	dfw_create_index,
	dfw_delete_index,
	dfw_compute_security,
	dfw_add_file,
	dfw_add_shadow,
	dfw_delete_shadow,
	dfw_modify_file,
	dfw_erase_file,
	dfw_delete_field,
	dfw_delete_global,
	dfw_delete_rfr,
	dfw_post_event,
	dfw_create_trigger,
	dfw_delete_trigger,
	dfw_modify_trigger,
	dfw_load_triggers,
	dfw_grant,
	dfw_revoke,
	dfw_scan_relation,
	dfw_create_expression_index,
	dfw_delete_expression_index,
	dfw_create_procedure,
	dfw_modify_procedure,
	dfw_delete_procedure,
	dfw_delete_prm, 
	dfw_delete_exception, 
	dfw_unlink_file,
	dfw_delete_generator,
	dfw_modify_generator,
	dfw_delete_udf,
	dfw_add_difference,
	dfw_delete_difference,
	dfw_begin_backup,
	dfw_end_backup
};

class DeferredWork : public pool_alloc_rpt<SCHAR, type_dfw>
{
    public:
	enum dfw_t 		dfw_type;		/* type of work deferred */
	DeferredWork*	dfw_next;		/* next block in transaction */
	Lock*			dfw_lock;		/* relation creation lock */
	DeferredWork*	dfw_args;		/* arguments */
	SLONG			dfw_sav_number;	/* save point number */
	USHORT			dfw_name_length;/* length of object name */
	USHORT			dfw_id;			/* object id, if appropriate */
	USHORT			dfw_count;		/* count of block posts */
	SCHAR			dfw_name[2];	/* name of object */
};

/* Verb actions */

class UndoItem {
public:
	SINT64 rec_number;
	Record* rec_data;
    static const SINT64& generate(const void *sender, const UndoItem& item) {
		return item.rec_number;
    }
	UndoItem() {
	}
	UndoItem(SINT64 rec_numberL, Record* rec_dataL) {
		this->rec_number = rec_numberL;
		this->rec_data = rec_dataL;
	}
};

typedef Firebird::BePlusTree<UndoItem, SINT64, MemoryPool, UndoItem> UndoItemTree;

class VerbAction : public pool_alloc<type_vct>
{
    public:
	VerbAction* 	vct_next;		/* Next action within verb */
	jrd_rel*		vct_relation;	/* Relation involved */
	RecordBitmap*	vct_records;	/* Record involved */
	UndoItemTree*	vct_undo;		/* Data for undo records */
};

} //namespace Jrd

#endif // JRD_TRA_H

