/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		lck.h
 *	DESCRIPTION:	Lock hander definitions (NOT lock manager)
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
 */

#ifndef _JRD_LCK_H_
#define _JRD_LCK_H_

/* Lock types */

enum lck_t {
	LCK_database = 1,			/* Root of lock tree */
	LCK_relation,				/* Individual relation lock */
	LCK_bdb,					/* Individual buffer block */
	LCK_tra,					/* Individual transaction lock */
	LCK_rel_exist,				/* Relation existence lock */
	LCK_idx_exist,				/* Index existence lock */
	LCK_attachment,				/* Attachment lock */
	LCK_shadow,					/* Lock to synchronize addition of shadows */
	LCK_sweep,					/* Sweep lock for single sweeper */
	LCK_file_extend,			/* Lock to synchronize file extension */
	LCK_retaining,				/* Youngest commit retaining transaction */
	LCK_expression,				/* Expression index caching mechanism */
	LCK_record_locking,			/* Lock on existence of record locking for this database */
	LCK_record,					/* Record Lock */
	LCK_prc_exist,				/* Relation existence lock */
	LCK_range_relation,			/* Relation refresh range lock */
	LCK_update_shadow			/* shadow update sync lock */
};

/* Lock owner types */

enum lck_owner_t {
	LCK_OWNER_process = 1,		/* A process is the owner of the lock */
	LCK_OWNER_database,			/* A database is the owner of the lock */
	LCK_OWNER_attachment,		/* An atttachment is the owner of the lock */
	LCK_OWNER_transaction		/* A transaction is the owner of the lock */
};

/* Blocking AST routine type.  I've added this one so that all the 
   reinterpret_cast messages casting functions to store in this struct
   can use the same type. Obviously storage of function pointers wants
   to be done in a nicer way in future so the types match.  MOD 17-7-2001 */

typedef int (*lck_ast_t)();    

typedef struct lck {
	struct blk lck_header;
	struct lck *lck_parent;
	struct lck *lck_next;		/* Next lock in chain owned by dbb */
	struct lck *lck_att_next;	/* Next in chain owned by attachment */
	struct lck *lck_prior;
	struct lck *lck_collision;	/* collisions in compatibility table */
	struct lck *lck_identical;	/* identical locks in compatibility table */
	struct dbb *lck_dbb;		/* database object is contained in */
	struct blk *lck_object;		/* argument to be passed to ast */
	struct blk *lck_owner;		/* Logical owner block (transaction, etc.) */
	struct blk *lck_compatible;	/* Enter into internal_enqueue() and treat as compatible */
	struct blk *lck_compatible2;	/* Sub-level for internal compatibility */
	struct att *lck_attachment;	/* Attachment that owns lock */
	struct btb *lck_blocked_threads;	/* Threads blocked by lock */
        lck_ast_t lck_ast;	        /* Blocking AST routine */
	SLONG lck_id;				/* Lock id from lock manager */
	SLONG lck_owner_handle;		/* Lock owner handle from the lock manager's point of view */
	USHORT lck_count;			/* count of locks taken out by attachment */
	SSHORT lck_length;			/* Length of lock string */
	UCHAR lck_logical;			/* Logical lock level */
	UCHAR lck_physical;			/* Physical lock level */
#ifndef GATEWAY
	SLONG lck_data;				/* Data associated with a lock */
#else
	SCHAR lck_reserved;			/* Logical level of reserving lock */
#endif
	enum lck_t lck_type;
	union {
		SCHAR lck_string[1];
		SLONG lck_long;
	} lck_key;
	SCHAR lck_tail[1];			/* Makes the allocater happy */
} *LCK;

#endif /* _JRD_LCK_H_ */
