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

#ifndef JRD_RSE_H
#define JRD_RSE_H

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"

#include "../common/classes/array.h"
#include "../jrd/constants.h"

#include "../jrd/dsc.h"

// Record source block (RSB) types

enum rsb_t
{
	rsb_boolean,						// predicate (logical condition)
	rsb_cross,							// inner join as a nested loop
	rsb_first,							// retrieve first n records
	rsb_skip,							// skip n records
	rsb_indexed,						// access via an index
	rsb_merge,							// join via a sort merge
	rsb_sequential,						// natural scan access
	rsb_sort,							// sort
	rsb_union,							// union
	rsb_aggregate,						// aggregation
	rsb_ext_sequential,					// external sequential access
	rsb_ext_indexed,					// external indexed access
	rsb_ext_dbkey,						// external DB_KEY access
	rsb_navigate,						// navigational walk on an index
	rsb_left_cross,						// left outer join as a nested loop
	rsb_procedure						// stored procedure
};
typedef rsb_t RSB_T;


// Record source block

class Rsb : public pool_alloc_rpt<class Rsb*, type_rsb>
{
public:
	RSB_T rsb_type;						// type of rsb
	UCHAR rsb_stream;					// stream, if appropriate
	USHORT rsb_count;					// number of sub arguments
	USHORT rsb_flags;
	ULONG rsb_impure;					// offset to impure area
	ULONG rsb_cardinality;				// estimated cardinality of stream
	ULONG rsb_record_count;				// count of records returned from rsb (not candidate records processed)
	Rsb* rsb_next;						// next rsb, if appropriate
	struct jrd_rel* rsb_relation;		// relation, if appropriate
	struct str*	rsb_alias;				// SQL alias for relation
	struct jrd_prc* rsb_procedure;		// procedure, if appropriate
	struct fmt* rsb_format;				// format, if appropriate
	struct jrd_nod* rsb_any_boolean;	// any/all boolean
	Rsb* rsb_arg[1];
};
typedef Rsb *RSB;

// bits for the rsb_flags field

const USHORT rsb_singular = 1;			// singleton select, expect 0 or 1 records
const USHORT rsb_stream_type = 2;		// rsb is for stream type request
const USHORT rsb_descending = 4;		// an ascending index is being used for a descending sort or vice versa
const USHORT rsb_project = 8;			// projection on this stream is requested
const USHORT rsb_writelock = 16;		// records should be locked for writing

// special argument positions within the RSB

#define RSB_PRC_inputs			0
#define RSB_PRC_in_msg			1
#define RSB_PRC_count			2

#define RSB_NAV_index			0
#define RSB_NAV_inversion		1
#define RSB_NAV_key_length		2
#define RSB_NAV_idx_offset		3
#define RSB_NAV_count			4

#define RSB_LEFT_outer			0
#define RSB_LEFT_inner			1
#define RSB_LEFT_boolean		2
#define RSB_LEFT_streams		3
#define RSB_LEFT_rsbs			4
#define RSB_LEFT_inner_boolean	5
#define RSB_LEFT_inner_streams	6
#define RSB_LEFT_count			7


// Merge (equivalence) file block

typedef struct mfb {
	struct sfb *mfb_sfb;				// merge file uses SORT I/O routines
	ULONG mfb_equal_records;			// equality group cardinality
	ULONG mfb_record_size;				// matches sort map length
	ULONG mfb_current_block;			// current merge block in buffer
	ULONG mfb_block_size;				// merge block I/O size
	ULONG mfb_blocking_factor;			// merge equality records per block
	UCHAR *mfb_block_data;				// merge block I/O buffer
} *MFB;

#define MERGE_BLOCK_SIZE	65536


// Impure area formats for the various RSB types

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
	USHORT irsb_mrg_count;				// next stream in group
	struct irsb_mrg_repeat {
		SLONG irsb_mrg_equal;			// queue of equal records
		SLONG irsb_mrg_equal_end;		// end of the equal queue
		SLONG irsb_mrg_equal_current;	// last fetched record from equal queue
		SLONG irsb_mrg_last_fetched;	// first sort merge record of next group
		SSHORT irsb_mrg_order;			// logical merge order by substream
		struct mfb irsb_mrg_file;		// merge equivalence file
	} irsb_mrg_rpt[1];
} *IRSB_MRG;

typedef struct irsb_sim {
	ULONG irsb_flags;
	USHORT irsb_sim_rid;				// next relation id
	USHORT irsb_sim_fid;				// next field id
	struct jrd_req *irsb_sim_req1;		// request handle #1
	struct jrd_req *irsb_sim_req2;		// request handle #2
} *IRSB_SIM;

const ULONG irsb_sim_alias = 32;		// duplicate relation but w/o user name
const ULONG irsb_sim_eos = 64;			// encountered end of stream
const ULONG irsb_sim_active = 128;		// remote simulated stream request is active


// impure area format for navigational rsb type,
// which holds information used to get back to 
// the current location within an index

typedef struct irsb_nav {
	ULONG irsb_flags;
	SLONG irsb_nav_expanded_offset;			// page offset of current index node on expanded index page
	SLONG irsb_nav_number;					// last record number
	SLONG irsb_nav_page;					// index page number
	SLONG irsb_nav_incarnation;				// buffer/page incarnation counter
	ULONG irsb_nav_count;					// record count of last record returned
	struct sbm **irsb_nav_bitmap;			// bitmap for inversion tree
	struct sbm *irsb_nav_records_visited;	// bitmap of records already retrieved
	USHORT irsb_nav_offset;					// page offset of current index node
	USHORT irsb_nav_lower_length;			// length of lower key value
	USHORT irsb_nav_upper_length;			// length of upper key value
	USHORT irsb_nav_length;					// length of expanded key
	UCHAR irsb_nav_data[1];					// expanded key, upper bound, and index desc
} *IRSB_NAV;

// flags for the irsb_flags field

const ULONG irsb_first = 1;
const ULONG irsb_joined = 2;				// set in left join when current record has been joined to something
const ULONG irsb_mustread = 4;				// set in left join when must read a record from left stream
const ULONG irsb_open = 8;					// indicated rsb is open
const ULONG irsb_backwards = 16;			// backwards navigation has been performed on this stream
const ULONG irsb_in_opened = 32;			// set in outer join when inner stream has been opened
const ULONG irsb_join_full = 64;			// set in full join when left join has completed
const ULONG irsb_checking_singular = 128;	// fetching to verify singleton select
const ULONG irsb_singular_processed = 256;	// singleton stream already delivered one record
const ULONG irsb_last_backwards = 512;		// rsb was last scrolled in the backward direction
const ULONG irsb_bof = 1024;				// rsb is at beginning of stream
const ULONG irsb_eof = 2048;				// rsb is at end of stream
const ULONG irsb_crack = 4096;				// the record at our current position is missing
const ULONG irsb_forced_crack = 8192;		// the above-mentioned crack was forced by user
const ULONG irsb_refresh = 16384;			// enter records into refresh range
const ULONG irsb_key_changed = 32768;		// key has changed since record last returned from rsb


// Sort map block

struct smb_repeat {
	DSC smb_desc;				// relative descriptor
	USHORT smb_flag_offset;		// offset of missing flag
	USHORT smb_stream;			// stream for field id
	SSHORT smb_field_id;		// id for field (-1 if dbkey)
	struct jrd_nod *smb_node;	// expression node
};

class smb : public pool_alloc_rpt<smb_repeat, type_smb>
{
public:
	USHORT smb_keys;			// number of keys
	USHORT smb_count;			// total number of fields
	USHORT smb_length;			// sort record length
	USHORT smb_key_length;		// key length in longwords
	struct skd *smb_key_desc;	// address of key descriptors
	USHORT smb_flags;			// misc sort flags
    smb_repeat smb_rpt[1];
};
typedef smb *SMB;

// values for smb_field_id

const SSHORT SMB_DBKEY = -1;	// dbkey value
const SSHORT SMB_TRANS_ID = -2;	// transaction id of record

// bits for the smb_flags field

const USHORT SMB_project = 1;	// sort is really a project
const USHORT SMB_tag = 2;		// beast is a tag sort


// Blocks used to compute optimal join order:
// indexed relationships block (IRL) holds 
// information about potential join orders

class irl : public pool_alloc<type_irl>
{
public:
	struct irl *irl_next;		// next irl block for stream
	USHORT irl_stream;			// stream reachable by relation
	USHORT irl_unique;			// is this stream reachable by unique index?
};
typedef irl *IRL;


// Must be less then MAX_SSHORT. Not used for static arrays.
#define MAX_CONJUNCTS	32000

// Note that MAX_STREAMS currently MUST be <= MAX_UCHAR.
// Here we should really have a compile-time assert, since this hard-coded
// limit is NOT negotiable so long as we use an array of UCHAR, where index 0
// tells how many streams are in the array (and the streams themselves are
// identified by a UCHAR).
#define MAX_STREAMS	255

// This is number of ULONG's needed to store bit-mapped flags for all streams
// OPT_STREAM_BITS = (MAX_STREAMS + 1) / sizeof(ULONG)
// This value cannot be increased simple way. Decrease is possible, but it is also
// hardcoded in several places such as TEST_DEP_ARRAYS macro
#define OPT_STREAM_BITS 8

// Number of streams, conjuncts, indices that will be statically allocated 
// in various arrays. Larger numbers will have to be allocated dynamically
#define OPT_STATIC_ITEMS 16


// General optimizer block

class Opt : public pool_alloc<type_opt>
{
public:
	class Csb *opt_csb;						// compiler scratch block
	SLONG opt_combinations;					// number of partial orders considered
	double opt_best_cost;					// cost of best join order
	SSHORT opt_base_conjuncts;				// number of conjuncts in our rse, next conjuncts are from parent
	USHORT opt_best_count;					// longest length of indexable streams
	USHORT opt_g_flags;						// global flags
	// 01 Oct 2003. Nickolay Samofatov: this static array takes as much as 256 bytes.
	// This is nothing compared to original Firebird 1.5 Opt structure size of ~180k
	// All other arrays had been converted to dynamic to preserve memory 
	// and improve performance
	struct opt_segment {
		// Index segments and their options
		struct jrd_nod* opt_lower;			// lower bound on index value
		struct jrd_nod* opt_upper;			// upper bound on index value
		struct jrd_nod* opt_match;			// conjunct which matches index segment
	} opt_segments[MAX_INDEX_SEGMENTS];
	struct opt_conjunct {
		// Conjunctions and their options
		struct jrd_nod* opt_conjunct_node;	// conjunction
		// Stream dependencies to compute conjunct
		ULONG opt_dependencies[(MAX_STREAMS + 1) / 32];
		UCHAR opt_conjunct_flags;
	};
	struct opt_stream {
		// Streams and their options
		IRL opt_relationships;				// streams directly reachable by index
		double opt_best_stream_cost;		// best cost of retrieving first n = streams
		USHORT opt_best_stream;				// stream in best join order seen so far
		USHORT opt_stream_number;			// stream in position of join order
		UCHAR opt_stream_flags;
	};
	Firebird::HalfStaticArray<opt_conjunct, OPT_STATIC_ITEMS> opt_conjuncts;
	Firebird::HalfStaticArray<opt_stream, OPT_STATIC_ITEMS> opt_streams;
	Opt(JrdMemoryPool* pool) : opt_conjuncts(pool), opt_streams(pool) {}
};
typedef Opt* OPT;

// values for opt_stream_flags

const USHORT opt_stream_used = 1;			// stream is used

// values for opt_conjunct_flags

const USHORT opt_conjunct_used = 1;			// conjunct is used
const USHORT opt_conjunct_matched = 2;		// conjunct matches an index segment

// global optimizer bits used in opt_g_flags

const USHORT opt_g_stream = 1;				// indicate that this is a blr_stream


// River block - used to hold temporary information about a group of streams

class riv : public pool_alloc_rpt<SCHAR, type_riv>
{
public:
	struct Rsb *riv_rsb;		// record source block for river
	USHORT riv_number;			// temporary number for river
	UCHAR riv_count;			// count of streams
	UCHAR riv_streams[1];		// actual streams
};
typedef riv *RIV;


// bookmark block, used to hold information about the current position 
// within an index; a pointer to this block is passed to the user as a
// handle to facilitate returning to this position

class bkm : public pool_alloc_rpt<SCHAR, type_bkm>
{
public:
	struct bkm *bkm_next;
	struct dsc bkm_desc;		// bookmark descriptor describing the bookmark handle
	ULONG bkm_handle;			// bookmark handle containing pointer to this block
	SLONG bkm_number;			// current record number
	SLONG bkm_page;				// current btree page
	SLONG bkm_incarnation;		// last known incarnation number of current btree page
	SLONG bkm_expanded_offset;	// offset into expanded index page (if it exists)
	USHORT bkm_offset;			// offset into current btree page
	USHORT bkm_flags;			// flag values indicated below
	struct dsc bkm_key_desc;	// descriptor containing current key value
	UCHAR bkm_key_data[1];		// current key value
};
typedef bkm *BKM;

const USHORT bkm_bof = 1;
const USHORT bkm_eof = 2;
const USHORT bkm_crack = 4;
const USHORT bkm_forced_crack = 8;

// types for navigating through a stream

typedef enum rse_get_mode {
	RSE_get_forward,
	RSE_get_backward,
	RSE_get_current,
	RSE_get_first,
	RSE_get_last,
	RSE_get_next
} RSE_GET_MODE;

#endif // JRD_RSE_H
