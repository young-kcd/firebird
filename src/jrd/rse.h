/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rse.h
 *	DESCRIPTION:	Record source block definitions
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
 * 2001.07.28: Added rsb_t.rsb_skip and IRSB_SKIP to support LIMIT functionality.
 */

#ifndef _JRD_RSE_H_
#define _JRD_RSE_H_

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

#include <vector>

#include "../jrd/dsc.h"

/* Record Source Block (RSB) types */

enum rsb_t
{
	rsb_boolean,
	rsb_cross,
	rsb_dbkey,
	rsb_first,
	rsb_skip,
	rsb_indexed,
	rsb_merge,
	rsb_multiple,
	rsb_project,
	rsb_sequential,
	rsb_sort,
	rsb_union,
	rsb_aggregate,
	rsb_ext_sequential, // External sequential access
	rsb_ext_indexed,    // External indexed access
	rsb_ext_dbkey,      // External dbkey access
	rsb_navigate,       // Walk navigational index
	rsb_bit_sieve,
	rsb_left_cross,     // Left outer join
	rsb_procedure,      // Stored procedure
	rsb_select,         // (Gateway only)
	rsb_sql_join,       // (Gateway only)
	rsb_simulate,       // (Gateway only)
	rsb_sim_cross,      // (Gateway only)
	rsb_once            // (Gateway only)
};
typedef rsb_t RSB_T;



/* Record Source Block */

class Rsb : public pool_alloc_rpt<class Rsb*, type_rsb>
{
public:
	RSB_T  rsb_type;            // type of rsb
	UCHAR  rsb_stream;          // stream, if appropriate
	USHORT rsb_count;           // number of sub arguments
	USHORT rsb_flags;
	ULONG  rsb_impure;          // offset to impure area
	ULONG  rsb_cardinality;     // estimated cardinality of stream
	ULONG  rsb_record_count;    // count of records returned from rsb (not candidate records processed)
	Rsb*   rsb_next;            // next rsb, if appropriate
	struct jrd_rel* rsb_relation;    // relation, if appropriate
	struct str*     rsb_alias;       // SQL alias for relation
	struct jrd_prc* rsb_procedure;   // procedure, if appropriate
	struct fmt*     rsb_format;      // format, if appropriate
	struct jrd_nod* rsb_any_boolean; // any/all boolean
	Rsb*   rsb_arg[1];
};
typedef Rsb *RSB;


/* bits for the rsb_flags field */

#define rsb_singular            1	/* singleton select, expect 0 or 1 records */
#define rsb_stream_type         2	/* rsb is for stream type request */
#define rsb_descending          4	/* an ascending index is being used for a descending sort or vice versa */
#define rsb_project				8	/* projection on this stream is requested */
#define rsb_writelock  		   16   /* records should be locked for writing */

/* Special argument positions within the RSB */

#define RSB_PRC_inputs		0
#define RSB_PRC_in_msg		1
#define RSB_PRC_count		2

#define RSB_NAV_index		0
#define RSB_NAV_inversion	1
#define RSB_NAV_key_length	2
#define RSB_NAV_idx_offset	3
#define RSB_NAV_count		4

#define RSB_LEFT_outer		0
#define RSB_LEFT_inner		1
#define RSB_LEFT_boolean	2
#define RSB_LEFT_streams	3
#define RSB_LEFT_rsbs		4
#define RSB_LEFT_inner_boolean	5
#define RSB_LEFT_inner_streams	6
#define RSB_LEFT_count		7



/* Merge (equivalence) file block */

typedef struct mfb {
	struct sfb *mfb_sfb;		/* merge file uses SORT I/O routines */
	ULONG mfb_equal_records;	/* equality group cardinality */
	ULONG mfb_record_size;		/* matches sort map length */
	ULONG mfb_current_block;	/* current merge block in buffer */
	ULONG mfb_block_size;		/* merge block I/O size */
	ULONG mfb_blocking_factor;	/* merge equality records per block */
	UCHAR *mfb_block_data;		/* merge block I/O buffer */
} *MFB;

#define MERGE_BLOCK_SIZE	65536

/* RSB impure area formats for the various rsb types */

typedef struct irsb {
	ULONG irsb_flags;
	USHORT irsb_count;
} *IRSB;

typedef struct irsb_first_n {
	ULONG irsb_flags;
	SLONG irsb_number;
    SINT64 irsb_count;
} *IRSB_FIRST;

typedef struct irsb_skip_n {
    ULONG irsb_flags;
    SLONG irsb_number;
    SINT64 irsb_count;
} *IRSB_SKIP;

typedef struct irsb_index {
	ULONG irsb_flags;
	SLONG irsb_number;
	SLONG irsb_prefetch_number;
	struct sbm **irsb_bitmap;
} *IRSB_INDEX;

typedef struct irsb_sort {
	ULONG irsb_flags;
	struct scb *irsb_sort_handle;
} *IRSB_SORT;

typedef struct irsb_procedure {
	ULONG irsb_flags;
	struct jrd_req *irsb_req_handle;
	struct str *irsb_message;
} *IRSB_PROCEDURE;

typedef struct irsb_mrg {
	ULONG irsb_flags;
	USHORT irsb_mrg_count;		/* Next stream in group */
	struct irsb_mrg_repeat {
		SLONG irsb_mrg_equal;	/* queue of equal records */
		SLONG irsb_mrg_equal_end;	/* end of the equal queue */
		SLONG irsb_mrg_equal_current;	/* Last fetched record from equal queue */
		SLONG irsb_mrg_last_fetched;	/* First sort merge record of next group */
		SSHORT irsb_mrg_order;	/* logical merge order by substream */
		struct mfb irsb_mrg_file;	/* merge equivalence file */
	} irsb_mrg_rpt[1];
} *IRSB_MRG;

typedef struct irsb_sim {
	ULONG irsb_flags;
	USHORT irsb_sim_rid;		/* Next relation id */
	USHORT irsb_sim_fid;		/* Next field id */
	struct jrd_req *irsb_sim_req1;	/* Request handle */
	struct jrd_req *irsb_sim_req2;	/* Request handle */
} *IRSB_SIM;

#define irsb_sim_alias	32		/* Duplicate relation but w/o user name */
#define irsb_sim_eos	64		/* Encountered end of stream */
#define irsb_sim_active	128		/* Remote simulated stream request is active */


/* impure area format for navigational rsb type,
   which holds information used to get back to 
   the current location within an index */

typedef struct irsb_nav {
	ULONG irsb_flags;
	SLONG irsb_nav_expanded_offset;	/* Page offset of current index node on expanded index page */
	SLONG irsb_nav_number;		/* last record number */
	SLONG irsb_nav_page;		/* index page number */
	SLONG irsb_nav_incarnation;	/* buffer/page incarnation counter */
	ULONG irsb_nav_count;		/* record count of last record returned */
	struct sbm **irsb_nav_bitmap;	/* Bitmap for inversion tree */
	struct sbm *irsb_nav_records_visited;	/* Bitmap of records already retrieved */
	USHORT irsb_nav_offset;		/* Page offset of current index node */
	USHORT irsb_nav_lower_length;	/* Length of lower key value */
	USHORT irsb_nav_upper_length;	/* Length of upper key value */
	USHORT irsb_nav_length;		/* Length of expanded key */
	UCHAR irsb_nav_data[1];		/* Expanded key, upper bound, and index desc */
} *IRSB_NAV;

/* flags for the irsb_flags field */

#define irsb_first                1
#define irsb_joined               2	/* Set in left join when current record has been joined to something */
#define irsb_mustread             4	/* Set in left join when must read a record from left stream */
#define irsb_open                 8	/* Indicated rsb is open */
#define irsb_backwards           16	/* Backwards navigation has been performed on this stream */
#define irsb_in_opened           32	/* Set in outer join when inner stream has been opened */
#define irsb_join_full           64	/* Set in full join when left join has completed */
#define irsb_checking_singular  128	/* fetching to verify singleton select */
#define irsb_singular_processed 256	/* singleton stream already delivered one record */
#define irsb_last_backwards     512	/* rsb was last scrolled in the backward direction */
#define irsb_bof               1024	/* rsb is at beginning of stream */
#define irsb_eof               2048	/* rsb is at end of stream */
#define irsb_crack             4096	/* the record at our current position is missing */
#define irsb_forced_crack      8192	/* the above-mentioned crack was forced by user */
#define irsb_refresh          16384	/* enter records into refresh range */
#define irsb_key_changed      32768	/* key has changed since record last returned from rsb */


/* Sort map block */
	struct smb_repeat {
		DSC smb_desc;			/* Relative descriptor */
		USHORT smb_flag_offset;	/* Offset of missing flag */
		USHORT smb_stream;		/* Stream for field id */
		SSHORT smb_field_id;	/* Id for field (-1 if dbkey) */
		struct jrd_nod *smb_node;	/* Expression node */
	};

class smb : public pool_alloc_rpt<smb_repeat, type_smb>
{
public:
	USHORT smb_keys;			/* Number of keys */
	USHORT smb_count;			/* Total number of fields */
	USHORT smb_length;			/* Sort record length */
	USHORT smb_key_length;		/* Key length in longwords */
	struct skd *smb_key_desc;	/* Address of key descriptors */
	USHORT smb_flags;			/* Misc sort flags */
    smb_repeat smb_rpt[1];
};
typedef smb *SMB;

/* values for smb_field_id */

#define SMB_DBKEY	-1			/* dbkey value */
#define SMB_TRANS_ID	-2		/* transaction id of record */

/* bits for the smb_flags field */

#define SMB_project	1			/* Sort is really a project */
#define SMB_tag		2			/* Beast is a tag sort */


/* Blocks used to compute optimal join order:
   Indexed relationships block (IRL) holds 
   information about potential join orders */

class irl : public pool_alloc<type_irl>
{
    public:
	struct irl *irl_next;		/* Next irl block for stream */
	USHORT irl_stream;			/* Stream reachable by relation */
	USHORT irl_unique;			/* Is this stream reachable by unique index? */
};
typedef irl *IRL;

/* Types of simulated joins (Gateway only) */

#define SJN_by_fld	4
#define SJN_by_rel	3
#define SJN_by_rel_prv	2
#define SJN_none	0

/* Simulated relationships block holds information about potential joins (Gateway only) */

class srl : public pool_alloc<type_srl>
{
    public:
	struct srl *srl_next;		/* Next srl block for stream */
	USHORT srl_stream;			/* Stream reachable by join */
	USHORT srl_type;			/* Simulated join type */
};
typedef srl *SRL;


/* the maximum number of opt items is the maximum
   number of streams, conjuncts, or indices which
   can be used in an rse, since this is the space
   available in the opt block */

#define MAX_OPT_ITEMS	256

#define MAX_CONJUNCTS	MAX_OPT_ITEMS
#define MAX_INDICES		MAX_OPT_ITEMS
#define	OPT_BITS		(MAX_OPT_ITEMS / 32)

// Note that MAX_STREAMS currently MUST be <= MAX_UCHAR.
// Here we should really have a compile-time assert, since this hard-coded
// limit is NOT negotiable so long as we use an array of UCHAR, where index 0
// tells how many streams are in the array (and the streams themselves are
// identified by a UCHAR).
#define MAX_STREAMS	255


/* General optimizer block */
class Opt : public pool_alloc<type_opt>
{
    public:
	class Csb *opt_csb; /* Compiler scratch block */
	SLONG opt_combinations; /* Number of partial orders considered */
	double opt_best_cost; /* Cost of best join order */
	SSHORT opt_base_conjuncts;				// number of conjuncts in our rse, next conjuncts are distributed parent
	SSHORT opt_base_parent_conjuncts;		// number of conjuncts in our rse + distributed with parent, next are parent
	SSHORT opt_base_missing_conjuncts;		// number of conjuncts in our and parent rse, but without missing
	SSHORT opt_conjuncts_count;				// total number of conjuncts in our and parent rse
	USHORT opt_best_count; /* Longest length of indexable streams */
	USHORT opt_g_flags; /* global flags */
	struct opt_repeat {
		struct jrd_nod *opt_conjunct; /* Conjunction */
		struct jrd_nod *opt_lower; /* Lower bound on index value */
		struct jrd_nod *opt_upper; /* Upper bound on index value */
		struct jrd_nod *opt_match; /* Conjunct which matches index segment */
		IRL opt_relationships; /* Streams directly reachable by index */
		ULONG opt_dependencies[MAX_OPT_ITEMS / 32];
		/* Stream dependencies to compute conjunct */
		double opt_best_stream_cost; /* best cost of retrieving first n = streams */
		USHORT opt_best_stream; /* stream in best join order seen so far */
		USHORT opt_flags;
		USHORT opt_stream; /* Stream in position of join order */
		BOOLEAN opt_idx_full_match; 
		/* Does the conjunct match all fields in index. */
	};
	struct opt_repeat opt_rpt[MAX_OPT_ITEMS];
};
typedef Opt *OPT;
/* stream-dependent bits used in opt_flags */

#define opt_used	1
#define opt_stream_used	2
#define opt_matched	4			/* conjunct matches an index segment */

/* global optimizer bits used in opt_g_flags */

#define opt_g_stream 	1		/* indicate that this is a blr_stream */


/* River block -- used to hold temporary information about a group of streams */

class riv : public pool_alloc_rpt<SCHAR, type_riv>
{
    public:
	struct Rsb *riv_rsb;		/* Record source block for river */
	USHORT riv_number;			/* Temporary number for river */
	UCHAR riv_count;			/* Count of streams */
	UCHAR riv_streams[1];		/* Actual streams */
};
typedef riv *RIV;


/* bookmark block, used to hold information about the current position 
   within an index; a pointer to this block is passed to the user as a
   handle to facilitate returning to this position */

class bkm : public pool_alloc_rpt<SCHAR, type_bkm>
{
    public:
	struct bkm *bkm_next;
	struct dsc bkm_desc;		/* bookmark descriptor describing the bookmark handle */
	ULONG bkm_handle;			/* bookmark handle containing pointer to this block */
	SLONG bkm_number;			/* current record number */
	SLONG bkm_page;				/* current btree page */
	SLONG bkm_incarnation;		/* last known incarnation number of current btree page */
	SLONG bkm_expanded_offset;	/* offset into expanded index page (if it exists) */
	USHORT bkm_offset;			/* offset into current btree page */
	USHORT bkm_flags;			/* flag values indicated below */
	struct dsc bkm_key_desc;	/* descriptor containing current key value */
	UCHAR bkm_key_data[1];		/* current key value */
};
typedef bkm *BKM;

#define bkm_bof			1
#define bkm_eof			2
#define bkm_crack		4
#define bkm_forced_crack	8

/* types for navigating through a stream */

typedef enum rse_get_mode {
	RSE_get_forward,
	RSE_get_backward,
	RSE_get_current,
	RSE_get_first,
	RSE_get_last,
	RSE_get_next
} RSE_GET_MODE;

#endif /* _JRD_RSE_H_ */
