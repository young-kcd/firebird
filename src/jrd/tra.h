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
 * to define the types this header usus.
 */

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../common/classes/tree.h"
#include "../jrd/rpb_chain.h"

/* Transaction block */

class jrd_tra : public pool_alloc_rpt<SCHAR, type_tra>
{
    public:
	struct att *tra_attachment;	/* database attachment */
	SLONG tra_number;			/* transaction number */
	SLONG tra_top;				/* highest transaction in snapshot */
	SLONG tra_oldest;			/* oldest interesting transaction */
	SLONG tra_oldest_active;	/* record versions older than this can be
								   gargage-collected by this tx */
	struct jrd_tra *tra_next;		/* next transaction in database */
	struct jrd_tra *tra_sibling;	/* next transaction in group */
	JrdMemoryPool* tra_pool;		/* pool for transaction */
	class blb *tra_blobs;		/* Linked list of active blobs */
	struct arr *tra_arrays;		/* Linked list of active arrays */
	struct lck *tra_lock;		/* lock for transaction */
	struct vec *tra_relation_locks;	/* locks for relations */
	struct sbm *tra_commit_sub_trans;	/* commited sub-transactions */
	struct sav *tra_save_point;	/* list of savepoints  */
	struct sav *tra_save_free;	/* free savepoints */
	SLONG tra_save_point_number;	/* next save point number to use */
	ULONG tra_flags;
#ifdef PC_ENGINE
	SLONG tra_range_id;				/* unique id of cache range within transaction */
#endif
	struct dfw *tra_deferred_work;	/* work deferred to commit time */
	class Rsc *tra_resources;		/* resource existence list */
	class traRpbList *tra_rpblist;	/* active RPB's of given transaction */
	UCHAR tra_use_count;			/* use count for safe AST delivery */
	UCHAR tra_callback_count;		/* callback count for 'execute statement' */
	UCHAR tra_transactions[1];
};
typedef jrd_tra *JRD_TRA;

#define TRA_system		1L		/* system transaction */
#define TRA_update		2L		/* update is permitted */
#define TRA_prepared		4L	/* transaction is in limbo */
#define TRA_reconnected		8L	/* reconnect in progress */
#define TRA_reserving		16L	/* relations explicityly locked */
#define TRA_degree3		32L		/* serializeable transaction */
#define TRA_committing		64L	/* commit in progress */
#define TRA_write		128L	/* transaction has written */
#define TRA_readonly		256L	/* transaction is readonly */
#define TRA_nowait		512L	/* don't wait on relations, give up */
#define TRA_prepare2		1024L	/* transaction has updated RDB$TRANSACTIONS */
#define TRA_ignore_limbo	2048L	/* ignore transactions in limbo */
#define TRA_invalidated 	4096L	/* transaction invalidated by failed write */
#define TRA_deferred_meta 	8192L	/* deferred meta work posted */
#define TRA_add_log		16384L	/* write ahead log file was added */
#define TRA_delete_log		32768L	/* write ahead log file was deleted */
#define TRA_read_committed	65536L	/* can see latest committed records */
#define TRA_autocommit		131072L	/* autocommits all updates */
#define TRA_perform_autocommit	262144L	/* indicates autocommit is necessary */
#define TRA_rec_version		524288L	/* don't wait for uncommitted versions */
#define TRA_restart_requests	1048576L	/* restart all requests in attachment */
#define TRA_no_auto_undo	2097152L	/* don't start a savepoint in TRA_start */
#define TRA_sweep_at_startup    4194304L	/* sweep done at startup */
#define TRA_precommitted	8388608L	/* transaction committed at startup */

#define TRA_MASK		3
#define TRA_BITS_PER_TRANS	2
#define TRA_TRANS_PER_BYTE	4
#define TRA_SHIFT		2

#define TRANS_SHIFT(number)	(((number) & TRA_MASK) << 1)
#define TRANS_OFFSET(number)	((number) >> TRA_SHIFT)

/* Transaction cleanup. If a database is never quiescent, look
   for "dead" active transactions every so often at transaction
   startup */

#define TRA_ACTIVE_CLEANUP	100

/* Transaction states.  The first four are states found
   in the transaction inventory page; the last two are
   returned internally */

#define tra_active	    0		/* Transaction is active */
#define tra_limbo	    1
#define tra_dead	    2
#define tra_committed	    3
#define tra_us		    4		/* Transaction is us */
#define tra_precommitted    5	/* Transaction is precommitted */

/* The highest transaction number possible.  This is 0x7fffffff if SLONG is 32 bits. */
#define MAX_TRA_NUMBER		 (~(1L << (BITS_PER_LONG - 1)))
/* Savepoint block */

class sav : public pool_alloc<type_sav>
{
    public:
	struct vct *sav_verb_actions;	/* verb action list */
	struct vct *sav_verb_free;	/* free verb action block */
	USHORT sav_verb_count;		/* Active verb count */
	SLONG sav_number;			/* save point number */
	struct sav *sav_next;
	USHORT sav_flags;
	TEXT sav_name[32]; /* Savepoint name */
};
typedef sav *SAV;

/* Savepoint block flags. */

#define SAV_trans_level		1	/* savepoint was started by TRA_start */
#define SAV_event_post		2	/* event posted in the save point */
#define SAV_user            4   /* named user savepoint as opposed to system ones */

#define SAV_LARGE         500   /* Number of allocated record structs when transaction sweep
                                  becomes preferred over transaction savepoint undo */

/* Deferred work blocks are used by the meta data handler to keep track
   of work deferred to commit time.  This are usually used to perform
   meta data updates */

ENUM dfw_t {
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
	dfw_delete_log,
	dfw_create_log,
	dfw_create_procedure,
	dfw_modify_procedure,
	dfw_delete_procedure,
	dfw_delete_prm, 
	dfw_delete_exception, 
	dfw_unlink_file,
	dfw_delete_generator,
	dfw_delete_udf
};

class dfw : public pool_alloc_rpt<SCHAR, type_dfw>
{
    public:
	ENUM dfw_t dfw_type;		/* type of work deferred */
	struct dfw *dfw_next;		/* next block in transaction */
	struct lck *dfw_lock;		/* relation creation lock */
	struct dfw *dfw_args;		/* arguments */
	SLONG dfw_sav_number;		/* save point number */
	USHORT dfw_name_length;		/* length of object name */
	USHORT dfw_id;				/* object id, if appropriate */
	USHORT dfw_count;			/* count of block posts */
	SCHAR dfw_name[2];			/* name of object */
};
typedef dfw *DFW;

/* Verb actions */

class UndoItem {
public:
	SLONG rec_number;
	class rec* rec_data;
    static const SLONG& generate(void *sender, const UndoItem& item) {
		return item.rec_number;
    }
	UndoItem() {
	}
	UndoItem(SLONG rec_number, rec* rec_data) {
		this->rec_number = rec_number;
		this->rec_data = rec_data;
	}
};

typedef Firebird::BePlusTree<UndoItem, SLONG, MemoryPool, UndoItem> UndoItemTree;

class vct : public pool_alloc<type_vct>
{
    public:
	struct vct *vct_next;		/* Next action within verb */
	struct jrd_rel *vct_relation;	/* Relation involved */
	struct sbm *vct_records;	/* Record involved */
	UndoItemTree* vct_undo;     /* Data for undo records */
};
typedef vct *VCT;


#endif /* JRD_TRA_H */
