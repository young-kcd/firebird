/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		req.h
 *	DESCRIPTION:	Request block definitions
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

#ifndef _JRD_REQ_H_
#define _JRD_REQ_H_

/* record parameter block */

typedef struct rpb {
	SLONG rpb_number;			/* record number in relation */
	SLONG rpb_transaction;		/* transaction number */
	struct rel *rpb_relation;	/* relation of record */
	struct rec *rpb_record;		/* final record block */
	struct rec *rpb_prior;		/* prior record block if this is a delta record */
	struct srpb *rpb_copy;		/* rpb copy for singleton verification */
	struct rec *rpb_undo;		/* our first version of data if this is a second modification */
	USHORT rpb_format_number;	/* format number in relation */

#ifndef GATEWAY
	SLONG rpb_page;				/* page number */
	USHORT rpb_line;			/* line number on page */

	SLONG rpb_f_page;			/* fragment page number */
	USHORT rpb_f_line;			/* fragment line number on page */

	SLONG rpb_b_page;			/* back page */
	USHORT rpb_b_line;			/* back line */
#else
	struct sbm *rpb_fields;		/* referenced fields */
	struct sbm *rpb_asgn_flds;	/* referenced assigned from fields */
	struct sql *rpb_sql_selct;	/* SQL select statement */
	struct sql *rpb_sql_other;	/* SQL non-select statement */
#endif

	UCHAR *rpb_address;			/* address of record sans header */
	USHORT rpb_length;			/* length of record */
	USHORT rpb_flags;			/* record ODS flags replica */
	USHORT rpb_stream_flags;	/* stream flags */
	SSHORT rpb_org_scans;		/* relation scan count at stream open */
#ifndef GATEWAY
	struct win rpb_window;
#endif
} RPB;

/* Record flags must be an exact replica of ODS record header flags */

#define rpb_deleted	1
#define rpb_chained	2
#define rpb_fragment	4
#define rpb_incomplete	8
#define rpb_blob	16
#define rpb_delta	32			/* prior version is a differences record */
#define rpb_damaged	128			/* record is busted */
#define rpb_gc_active	256		/* garbage collecting dead record version */

/* Stream flags */

#define RPB_s_refetch	0x1		/* re-fetch required due to sort */
#define RPB_s_update	0x2		/* input stream fetched for update */

#ifndef GATEWAY
#define SET_NULL(record,id)	record->rec_data [id >> 3] |=  (1 << (id & 7))
#define CLEAR_NULL(record,id)	record->rec_data [id >> 3] &= ~(1 << (id & 7))
#define TEST_NULL(record,id)	record->rec_data [id >> 3] &   (1 << (id & 7))
#else
#define SET_NULL(record,id)	((SSHORT*) record->rec_data) [id] = -1
#define CLEAR_NULL(record,id)	((SSHORT*) record->rec_data) [id] = 0
#define TEST_NULL(record,id)	(((SSHORT*) record->rec_data) [id] < 0)
#endif

#define MAX_DIFFERENCES		1024	/* Max length of generated Differences string 
									   between two records */

/* Store allocation policy types.  Parameter to DPM_store() */

#define DPM_primary	1			/* New primary record */
#define DPM_secondary	2		/* Chained version of primary record */
#define DPM_other	3			/* Independent (or don't care) record */

/* Record block (holds data, remember data?) */

typedef struct rec {
	struct blk rec_header;
	struct fmt *rec_format;		/* what the data looks like */
	struct lls *rec_precedence;	/* stack of higher precedence pages */
	USHORT rec_length;			/* how much there is */
	UCHAR rec_flags;			/* misc record flags */
	SLONG rec_number;			/* original rpb number - used for undoing multiple updates */
	double rec_dummy;			/* this is to force next field to a double boundary */
	UCHAR rec_data[1];			/* THIS VARIABLE MUST BE ALIGNED ON A DOUBLE BOUNDARY */
} *REC;

#define REC_same_tx	1			/* record inserted/updated and deleted by same tx */
#define REC_gc_active	2		/* relation garbage collect record block in use */
#define REC_new_version 4		/* savepoint created new record version and deleted it */

/* save rpb block */

typedef struct srpb {
	struct blk srpb_header;
	struct rpb srpb_rpb[1];		/* record parameter blocks */
} *SRPB;

/* request block */

typedef struct req {
	struct blk req_header;
	ATT req_attachment;			/* database attachment */
	USHORT req_count;			/* number of streams */
	USHORT req_incarnation;		/* incarnation number */
	ULONG req_impure_size;		/* size of impure area */
	struct plb *req_pool;
	struct vec *req_sub_requests;	/* vector of sub-requests */
	struct tra *req_transaction;
	struct req *req_request;	/* next request in dbb */
	struct acc *req_access;		/* Access items to be checked */
	struct vec *req_variables;	/* Vector of variables, if any */
	struct rsc *req_resources;	/* Resources (relations and indices) */
	struct nod *req_message;	/* Current message for send/receive */
#ifdef SCROLLABLE_CURSORS
	struct nod *req_async_message;	/* Asynchronous message (used in scrolling) */
#endif
	struct vec *req_refresh_ranges;	/* Vector of refresh_ranges */
	struct rng *req_begin_ranges;	/* Vector of refresh_ranges */
	struct prc *req_procedure;	/* procedure, if any */
	TEXT *req_trg_name;			/* name of request (trigger), if any */
	USHORT req_length;			/* message length for send/receive */
	USHORT req_nmsgs;			/* number of message types */
	USHORT req_mmsg;			/* highest message type */
	USHORT req_msend;			/* longest send message */
	USHORT req_mreceive;		/* longest receive message */

	ULONG req_records_selected;	/* count of records selected by request (meeting selection criteria) */
	ULONG req_records_inserted;	/* count of records inserted by request */
	ULONG req_records_updated;	/* count of records updated by request */
	ULONG req_records_deleted;	/* count of records deleted by request */

	struct nod *req_top_node;	/* top of execution tree */
	struct nod *req_next;		/* next node for execution */
	struct vec *req_fors;		/* Vector of for loops, if any */
	struct vec *req_invariants;	/* Vector of invariant nodes, if any */
	USHORT req_label;			/* label for leave */
	ULONG req_flags;			/* misc request flags */
	struct sav *req_proc_sav_point;	/* procedure savepoint list */
	ULONG req_timestamp;		/* Start time of request */

	ENUM req_s {
		req_evaluate,
			req_return, req_receive, req_send, req_proceed, req_sync, req_unwind} req_operation;	/* operation for next node */

	struct rpb req_rpb[1];		/* record parameter blocks */
} *REQ;

#define REQ_SIZE	(sizeof (struct req) - sizeof (((REQ) 0)->req_rpb[0]))

/* Flags for req_flags */
#define req_active				0x1L
#define req_stall				0x2L
#define req_leave				0x4L
#ifdef SCROLLABLE_CURSORS
#define req_async_processing	0x8L
#endif
#define req_null			   0x10L
#define req_broken			   0x20L
#define req_abort			   0x40L
#define req_internal		   0x80L
#define req_warning			  0x100L
#define req_in_use			  0x200L
#define req_sys_trigger		  0x400L	/* request is a system trigger */
#define req_count_records	  0x800L	/* count records accessed */
#define req_proc_fetch		 0x1000L	/* Fetch from procedure in progress */
#define req_ansi_any		 0x2000L	/* Request is processing ANSI ANY */
#define req_same_tx_upd		 0x4000L	/* record was updated by same transaction */
#define req_ansi_all		 0x8000L	/* Request is processing ANSI ANY */
#define req_ansi_not		0x10000L	/* Request is processing ANSI ANY */
#define req_reserved		0x20000L	/* Request reserved for client */
#define req_ignore_perm	        0x40000L	/* ignore permissions checks */
#define req_fetch_required	0x80000L	/* need to fetch next record */
#define req_error_handler      0x100000L	/* looper is called to handle error */
#define req_clone_data_from_default_clause \
                               0x200000L	/* This flag is marked in the
											   **   req_flags if and only if the
											   **   the column that was created or
											   **   added as "DEFAULT xx NOT NULL"
											   **   and no data has been
											   **   inserted/updated to the column.
											   **   The data of this column was
											   **   cloned from the default clause.
											 */
#define req_blr_version4      0x400000L	/* Request is of blr_version4 */
/* Mask for flags preserved in a clone of a request */
#define REQ_FLAGS_CLONE_MASK	(req_sys_trigger | req_internal | req_ignore_perm | req_blr_version4)

/* Mask for flags preserved on initialization of a request */
#define REQ_FLAGS_INIT_MASK	(req_in_use | req_internal | req_sys_trigger | req_ignore_perm | req_blr_version4)


/* Resources */

/* TMN: Had to remove this enum from bein nested in struct rsc since we now use C++,
 * but this enum is used in the C API. Can you say "legacy"? :-(
 */
enum rsc_s {
	rsc_relation,
	rsc_procedure,
	rsc_index
};

typedef struct rsc {
	struct blk rsc_header;
	struct rsc *rsc_next;		/* Next resource in request */
	struct rel *rsc_rel;		/* Relation block */
	struct prc *rsc_prc;		/* Relation block */
	USHORT rsc_id;				/* Id of parent */
	enum rsc_s rsc_type;
} *RSC;

/* Index lock block */

typedef struct idl
{
	struct blk	idl_header;
	struct idl*	idl_next;		/* Next index lock block for relation */
	struct lck*	idl_lock;		/* Lock block */
	struct rel*	idl_relation;	/* Parent relation */
	USHORT		idl_id;			/* Index id */
	USHORT		idl_count;		/* Use count */
} *IDL;


/* Access items */

typedef struct acc
{
	struct blk	acc_header;
	struct acc*	acc_next;
#ifndef GATEWAY
	TEXT*		acc_security_name;	/* WRITTEN into by SCL_get_class() */
#else
	struct scl*	acc_security_name;
#endif
	struct rel*	acc_view;
	CONST TEXT*	acc_trg_name;
	CONST TEXT*	acc_prc_name;
	CONST TEXT*	acc_name;
	CONST TEXT*	acc_type;
	USHORT		acc_mask;
} *ACC;

#endif /* _JRD_REQ_H_ */
