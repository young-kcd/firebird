/*
 *	PROGRAM:	JRD access method
 *	MODULE:		cch.h
 *	DESCRIPTION:	Cache manager definitions
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

#ifndef JRD_CCH_H
#define JRD_CCH_H

#include "../include/fb_blk.h"

/* Page buffer cache size constraints. */

#define MIN_PAGE_BUFFERS	50L
#define MAX_PAGE_BUFFERS	65535L

/* BCB -- Buffer control block -- one per system */
	struct bcb_repeat
	{
		struct bdb*	bcb_bdb;		/* Buffer descriptor block */
		struct que	bcb_page_mod;	/* Que of buffers with page mod n */
	};

class bcb : public pool_alloc_rpt<bcb_repeat, type_bcb>
{
    public:
	struct lls*	bcb_memory;			/* Large block partitioned into buffers */
	struct que	bcb_in_use;			/* Que of buffers in use */
	struct que	bcb_empty;			/* Que of empty buffers */
	struct bdb*	bcb_btree;			/* root of dirty page btree */
	struct pre*	bcb_free;			/* Free precedence blocks */
	struct que	bcb_free_lwt;		/* Free latch wait blocks */
	SSHORT		bcb_flags;			/* see below */
	SSHORT		bcb_free_minimum;	/* Threshold to activate cache writer */
	ULONG		bcb_count;			/* Number of buffers allocated */
	ULONG		bcb_checkpoint;		/* Count of buffers to checkpoint */
	struct sbm*	bcb_prefetch;		/* Bitmap of pages to prefetch */
	bcb_repeat bcb_rpt[1];
};
typedef bcb *BCB;

#define BCB_keep_pages		1	/* set during btc_flush(), pages not removed from dirty binary tree */
#define BCB_cache_writer	2	/* cache writer thread has been started */
#define BCB_checkpoint_db	4	/* WAL has requested a database checkpoint */
#define BCB_writer_active	8	/* no need to post writer event count */
#define BCB_cache_reader	16	/* cache reader thread has been started */
#define BCB_reader_active	32	/* cache reader not blocked on event */
#define BCB_free_pending	64	/* request cache writer to free pages */


/* BDB -- Buffer descriptor block */

#define BDB_max_shared		20	/* maximum number of shared latch owners per bdb */

class bdb : public pool_alloc<type_bdb>
{
    public:
	struct dbb*	bdb_dbb;				/* Database block (for ASTs) */
	struct lck*	bdb_lock;				/* Lock block for buffer */
	struct que	bdb_que;				/* Buffer que */
	struct que	bdb_in_use;				/* queue of buffers in use */
	struct pag*	bdb_buffer;				/* Actual buffer */
	struct jrd_exp*	bdb_expanded_buffer;	/* expanded index buffer */
	bdb*		bdb_jrn_bdb;			/* BDB containing journal records */
	struct btb*	bdb_blocked;			/* Blocked attachments block */
	SLONG		bdb_page;				/* Database page number in buffer */
	ULONG		bdb_sequence;
	SLONG		bdb_incarnation;
	ULONG		bdb_transactions;		/* vector of dirty flags to reduce commit overhead */
	SLONG		bdb_mark_transaction;	/* hi-water mark transaction to defer header page I/O */
	bdb*		bdb_left;				/* dirty page binary tree link */
	bdb*		bdb_right;				/* dirty page binary tree link */
	bdb*		bdb_parent;				/* dirty page binary tree link */
	struct que	bdb_lower;				/* lower precedence que */
	struct que	bdb_higher;				/* higher precedence que */
	struct que	bdb_waiters;			/* latch wait que */
	struct tdbb*bdb_exclusive;			/* thread holding exclusive latch */
	struct tdbb*bdb_io;					/* thread holding io latch */
	UATOM		bdb_ast_flags;			/* flags manipulated at AST level */
	USHORT		bdb_flags;
	USHORT		bdb_length;				/* Length of journal records */
	SSHORT		bdb_use_count;			/* Number of active users */
	SSHORT		bdb_scan_count;			/* concurrent sequential scans */
	USHORT		bdb_write_direction;    /* Where to write buffer */
	ULONG       bdb_difference_page;    /* Number of page in difference file */
	SLONG       bdb_diff_generation;    /* Number of backup/restore cycle for 
										   this database in current process.
										   Used in CS only. */
	struct tdbb*bdb_shared[BDB_max_shared];	/* threads holding shared latches */
};
typedef bdb *BDB;

/* bdb_flags */

#define BDB_dirty			1		/* page has been updated but not written yet */
#define BDB_garbage_collect	2		/* left by scan for garbage collector */
#define BDB_writer			4		/* someone is updating the page */
#define BDB_marked			8		/* page has been updated */
#define BDB_must_write		16		/* forces a write as soon as the page is released */
#define BDB_faked			32		/* page was just allocated */
#define BDB_journal			64		/* Journal buffer */
#define BDB_system_dirty 	128		/* system transaction has marked dirty */
#define BDB_io_error	 	256		/* page i/o error */
#define BDB_read_pending 	512		/* read is pending */
#define BDB_free_pending 	1024	/* buffer being freed for reuse */
#define BDB_not_valid		2048	/* i/o error invalidated buffer */
#define BDB_db_dirty 		4096	/* page must be written to database */
#define BDB_checkpoint		8192	/* page must be written by next checkpoint */
#define BDB_prefetch		16384	/* page has been prefetched but not yet referenced */
#define BDB_no_blocking_ast 32768	/* No blocking AST registered with page lock */

/* bdb_ast_flags */

#define BDB_blocking 		1	/* a blocking ast was sent while page locked */

/* bdb_write_direction values */

#define BDB_write_undefined 0
#define BDB_write_normal	1
#define BDB_write_diff		2
#define BDB_write_both		3


/* PRE -- Precedence block */

class pre : public pool_alloc<type_pre>
{
    public:
	bdb*		pre_hi;
	bdb*		pre_low;
	struct que	pre_lower;
	struct que	pre_higher;
	SSHORT		pre_flags;
};
typedef pre *PRE;

#define PRE_cleared	1

/* Compatibility matrix for latch types.

   An exclusive latch is needed to modify a page.  Before
   marking a page an 'io-prevention' latch is needed: a mark latch.
   To look at a buffer, a shared latch is needed.  To write a page,
   an io latch is needed.  

   Exclusive and shared latches interact.  Io and mark latches 
   interact.

   An mark latch is implemented as an io latch.
   
   Latches are granted in the order in which they are
   queued with one notable exception -- if buffer write
   is in-progress then shared latches are granted ahead
   of any pending exclusive latches. 

	      shared	 io	exclusive   mark
-------------------------------------------------
shared      1	     -     0         -
io          -	     0     -         0
exclusive   0	     -     0         -
mark        -        0     -         0  */

/* LATCH types */

typedef enum
{
	LATCH_none,
	LATCH_shared,
	LATCH_io,
	LATCH_exclusive,
	LATCH_mark
} LATCH;

/* LWT -- Latch wait block */

class lwt : public pool_alloc<type_lwt>
{
    public:
	struct tdbb*	lwt_tdbb;
	LATCH			lwt_latch;		/* latch type requested */
	struct que		lwt_waiters;	/* latch queue */
	struct event	lwt_event;		/* grant event to wait on */
	USHORT			lwt_flags;
};
typedef lwt *LWT;

#define LWT_pending	1			/* latch request is pending */

#include "../jrd/os/pio.h"

/* Constants used by prefetch mechanism */

#define PREFETCH_MAX_TRANSFER	16384	/* maximum block I/O transfer (bytes) */
#define PREFETCH_MAX_PAGES	(2 * PREFETCH_MAX_TRANSFER / MIN_PAGE_SIZE)	/* maximum pages allowed per prefetch request */

/* Prefetch block */

class prf : public pool_alloc<type_prf>
{
    public:
	struct tdbb*prf_tdbb;			/* thread database context */
	SLONG		prf_start_page;		/* starting page of multipage prefetch */
	USHORT		prf_max_prefetch;	/* maximum no. of pages to prefetch */
	USHORT		prf_page_count;		/* actual no. of pages being prefetched */
	struct		piob prf_piob;		/* physical I/O status block */
	SCHAR*		prf_aligned_buffer;	/* buffer address aligned for raw (OS cache bypass) I/O */
	SCHAR*		prf_io_buffer;		/* I/O buffer address */
	UCHAR		prf_flags;
	bdb*		prf_bdbs[PREFETCH_MAX_TRANSFER / MIN_PAGE_SIZE];
	SCHAR		prf_unaligned_buffer[PREFETCH_MAX_TRANSFER + MIN_PAGE_SIZE];
};
typedef prf *PRF;

#define PRF_active	1			/* prefetch block currently in use */

#endif /* JRD_CCH_H */
