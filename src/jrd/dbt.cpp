/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbt.cpp
 *	DESCRIPTION:	Debugging routines
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
// Those includes until the END comment comes from everything.h and was moved
// here when the file was removed. 
// Most probably only a few of the includes are needed
#include "../jrd/common.h"
#include "../jrd/isc.h"
#include "../jrd/jrd.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "../jrd/all.h"
#include "../jrd/os/pio.h"
#include "../jrd/pag.h"
#include "../jrd/val.h"
#include "../jrd/exe.h"
#include "../jrd/req.h"
#include "../jrd/lls.h"
#include "../jrd/rse.h"
#include "../jrd/sbm.h"
#include "../jrd/tra.h"
#include "../jrd/sqz.h"
#include "../jrd/blb.h"
#include "../jrd/btr.h"
#include "../jrd/scl.h"
#include "../jrd/ext.h"
#include "../jrd/met.h"
#include "../jrd/sdw.h"
#include "../jrd/log.h"
#include "../jrd/intl.h"
#include "../jrd/intl_classes.h"
#include "../jrd/fil.h"
#include "../jrd/tpc.h"
#include "../jrd/rng.h"
#include "../jrd/svc.h"
#include "../jrd/blob_filter.h"
// END
#include "../jrd/dbg.h"

#define FLD(struct, string, field) string, (SCHAR*) OFFSET (struct, field), (SCHAR*) sizeof (((struct) NULL)->field)


typedef SCHAR* TEXT_PTR;

int* ptr;

TEXT_PTR dbt_window[] = {
	FLD(WIN*, "Page: %ld", win_page),
	FLD(WIN*, "Buffer: %x", win_buffer),
	FLD(WIN*, "BufferDesc: %x", win_bdb),
	FLD(WIN*, "Scans: %d", win_scans),
	FLD(WIN*, "Flags: %x", win_flags),
	0
},
dbt_record_param[] = {
	FLD(record_param*, "Relation %x", rpb_relation),
	FLD(record_param*, "Number %ld", rpb_number),
	FLD(record_param*, "Trans %ld", rpb_transaction_nr),
	FLD(record_param*, "Page %ld", rpb_page),
	FLD(record_param*, "Line %x", rpb_line),
	FLD(record_param*, "Back page %ld", rpb_b_page),
	FLD(record_param*, "Line %x", rpb_b_line),
	FLD(record_param*, "Fragment page %ld", rpb_f_page),
	FLD(record_param*, "Line %x", rpb_f_line),
	FLD(record_param*, "Format %x", rpb_format_number),
	FLD(record_param*, "Address: %x ", rpb_address),
	FLD(record_param*, "Length %x", rpb_length),
	FLD(record_param*, "Record %x", rpb_record),
	FLD(record_param*, "Flags %x", rpb_flags),
	0
};

static TEXT_PTR dbb_stuff[] = {
	"DATABASE",
	FLD(Database*, "BCB: %x", dbb_bcb),
	FLD(Database*, "Relations: %x", dbb_relations),
	FLD(Database*, "Lock: %x", dbb_lock),
	FLD(Database*, "File: %x", dbb_file),
	FLD(Database*, "Permanent: %x", dbb_permanent),
	FLD(Database*, "Pools: %x", dbb_pools),
	FLD(Database*, "Page_size: %d", dbb_page_size),
	FLD(Database*, "dp_per_pp: %d", dbb_dp_per_pp),
	0
},
vec[] = {
	"VECTOR",
// TMN: FIXFIX! John?
//	FLD(VEC, "Count %d", vec_count),
	"Count %d", (SCHAR*)4, (SCHAR*)4,
	0
},
vcl[] = {
	"VECTOR OF ULONGS",
// TMN: FIXFIX! John?
//	FLD(VCL, "Count %d", vcl_count),
	"Count %d", (SCHAR*)4, (SCHAR*)4,
	0
};
/*frb[] = {
	"FREE",
// TMN: FIXFIX! John?
//		FLD(FRB, "Next %x", frb_next),
		"Next %x", (SCHAR*)4, (SCHAR*)4,
		0
};*/
/*static TEXT_PTR hnk[] = {
	"HUNK",
		FLD(HNK, "Addr: %x", hnk_address),
		FLD(HNK, "Len: %d", hnk_length),
		FLD(HNK, "Next: %x", hnk_next),
		0
};
*/
/*static TEXT_PTR plb[] = {
	"POOL",
		FLD(PLB, "Id: %d", plb_pool_id),
		FLD(PLB, "Free: %x", plb_free),
		FLD(PLB, "Hunk: %x", plb_hunks),
		0
};*/

static TEXT_PTR BufferControl[] = {
	"BUFFER CONTROL",
		FLD(BufferControl*, "Count: %x", bcb_count),
		0
},
BufferDesc[] = {
	"BUFFER DESCRIPTOR",
		FLD(BufferDesc*, "Page: %ld", bdb_page),
		FLD(BufferDesc*, "Lock: %x", bdb_lock),
		FLD(BufferDesc*, "Buffer: %x", bdb_buffer),
		FLD(BufferDesc*, "Use count: %x", bdb_use_count),
		FLD(BufferDesc*, "Flags: %x", bdb_flags),
		0
},
Precedence[] = {
	"PRECEDENCE",
		FLD(Precedence*, "Flags: %x", pre_flags),
		FLD(Precedence*, "Low: %x", pre_low),
		FLD(Precedence*, "High: %x", pre_hi),
		0
},
Lock[] = {
	"LOCK",
		FLD(Lock*, "Parent: %x", lck_parent),
		FLD(Lock*, "Object: %x", lck_object),
		FLD(Lock*, "Type: %x", lck_type),
		FLD(Lock*, "Physical: %x", lck_physical),
		FLD(Lock*, "Logical: %x", lck_logical),
		FLD(Lock*, "Length: %x", lck_length),
		0
},
jrd_file[] = {
	"FILE",
		FLD(jrd_file*, "File desc: %x", fil_desc),
		0
},
pgc[] = {
	"PAGE CONTROL",
		FLD(PGC, "High water: %d", pgc_high_water),
		FLD(PGC, "Pages/PIP: %x", pgc_ppp),
		FLD(PGC, "First PIP: %x", pgc_pip),
		FLD(PGC, "Bytes/PIP: %x", pgc_bytes),
		0
},

jrd_rel[] = {
	"RELATION",
		FLD(jrd_rel*, "%s", rel_name),
		FLD(jrd_rel*, "Id: %d", rel_id),
		FLD(jrd_rel*, "Current format: %x", rel_current_format),
		FLD(jrd_rel*, "Formats: %x", rel_formats),
		FLD(jrd_rel*, "Pages: %x", rel_pages),
		FLD(jrd_rel*, "Root: %ld", rel_index_root),
		0
},
fmt[] = {
	"FORMAT",
		FLD(fmt*, "Count: %d", fmt_count),
		FLD(fmt*, "Length: %d", fmt_length),
		FLD(fmt*, "Version: %d", fmt_version),
		0
},
jrd_req[] = {
	"REQUEST",
		FLD(jrd_req*, "COUNT: %x", req_count),
		FLD(jrd_req*, "Impure: %x", req_impure_size),
		FLD(jrd_req*, "Incarn: %x", req_incarnation),
		FLD(jrd_req*, "Pool: %x", req_pool),
		FLD(jrd_req*, "Sub req: %x", req_sub_requests),
		FLD(jrd_req*, "Trans: %x", req_transaction),
		FLD(jrd_req*, "Next req: %x", req_request),
		FLD(jrd_req*, "Msg: %x", req_message),
		FLD(jrd_req*, "Length: %x", req_length),
		FLD(jrd_req*, "#msgs: %x", req_nmsgs),
		FLD(jrd_req*, "Max send: %x", req_msend),
		FLD(jrd_req*, "Max receive: %x", req_mreceive),
		FLD(jrd_req*, "Top: %x", req_top_node),
		FLD(jrd_req*, "Next: %x", req_next),
		FLD(jrd_req*, "Label: %x", req_label),
		FLD(jrd_req*, "Op: %x", req_operation),
		FLD(jrd_req*, "Flags: %x", req_flags),
		0
},
jrd_tra[] = {
	"TRANSACTION",
		FLD(jrd_tra*, "Number: %ld", tra_number),
		FLD(jrd_tra*, "Oldest: %ld", tra_oldest),
		FLD(jrd_tra*, "Next: %x", tra_next),
		FLD(jrd_tra*, "Pool: %x", tra_pool),
		FLD(jrd_tra*, "Lock: %x", tra_lock),
		FLD(jrd_tra*, "Locks: %x", tra_relation_locks),
		FLD(jrd_tra*, "Flags: %x", tra_flags),
		0
},
jrd_nod[] = {
	"NODE",
		FLD(jrd_nod*, "Type: %x", nod_type),
		FLD(jrd_nod*, "Impure: %x", nod_impure),
		0
},
lls[] = {
	"LINKED LIST STACK",
		FLD(LLS, "Object: %x", lls_object),
		FLD(LLS, "Next: %x", lls_next),
		0
},
VerbAction[] = {
	"RECORD",
		FLD(Record*, "Format: %x", rec_format),
		0
},
Rsb[] = {
	"RECORD SOURCE BLOCK",
		FLD(Rsb*, "Type: %x", rsb_type),
		FLD(Rsb*, "Stream: %x", rsb_stream),
		FLD(Rsb*, "Relation: %x", rsb_relation),
		FLD(Rsb*, "Next: %x", rsb_next),
		FLD(Rsb*, "Count: %x", rsb_count),
		0
},
opt[] = {
	"OPTIMIZER",
		FLD(OPT, "Csb*: %x", opt_csb),
		FLD(OPT, "Cnt: %x", opt_count),
		0
},
BitmapSegment[] = {
	"BIT MAP SEGMENT",
		FLD(BitmapSegment*, "Min: %x", bms_min),
		FLD(BitmapSegment*, "Max: %x", bms_max),
		0
},
DeferredWork[] = {
	"DEFERRED WORK BLOCK",
		FLD(DeferredWork*, "type: %d", dfw_type),
		FLD(DeferredWork*, "next: %x", dfw_next),
		FLD(DeferredWork*, "name: %s", dfw_name),
		0
},
tfb[] = {
	"TEMPORARY FIELD BLOCK",
		FLD(TFB, "id: %d", tfb_id),
		FLD(TFB, "dtype: %d", tfb_desc.dsc_dtype),
		FLD(TFB, "scale: %d", tfb_desc.dsc_scale),
		FLD(TFB, "len: %d", tfb_desc.dsc_length),
		0
},
str[] = {
	"string",
		FLD(STR, "length: %d", str_length),
		0
},
Dcc[] = {
	"DATA COMPRESSION CONTROL",
		FLD(Dcc*, "next: %x", dcc_next),
		FLD(Dcc*, "end: %x", dcc_end),
		0
},
SparseBitmap[] = {
	"SPARSE BIT MAP",
		FLD(SparseBitmap*, "state: %d", sbm_state),
		FLD(SparseBitmap*, "count: %d", sbm_count),
		FLD(SparseBitmap*, "used: %d", sbm_used),
		FLD(SparseBitmap*, "high water: %d", sbm_high_water),
		FLD(SparseBitmap*, "number: %d", sbm_number),
		0
},
SortMap[] = {
	"SORT MAP",
		FLD(SortMap*, "count: %d", smb_count),
		FLD(SortMap*, "keys: %d", smb_keys),
		FLD(SortMap*, "length: %d", smb_length),
		FLD(SortMap*, "sort key: %x", smb_key_desc),
		0
},
blb[] = {
	"BLOB",
		FLD(blb*, "Relation: %x", blb_relation),
		FLD(blb*, "Count: %d", blb_count),
		FLD(blb*, "Length: %d", blb_length),
		FLD(blb*, "Max seg: %d", blb_max_segment),
		FLD(blb*, "Flags: %x", blb_flags),
		FLD(blb*, "Trans: %x", blb_transaction),
		FLD(blb*, "Next: %x", blb_segment),
		0
},
irb[] = {

	"INDEX RETRIEVAL",
		FLD(IRB, "index: %d", irb_index),
		FLD(IRB, "relation: %x", irb_relation),
		FLD(IRB, "lower bounds: %d", irb_lower_count),
		FLD(IRB, "upper boudns: %d", irb_upper_count),
		0
},
ctl[] = {
	"BLOB CONTROL", 0
};

static TEXT_PTR scl[] = {	"SECURITY CLASS", 0};
static TEXT_PTR fld[] = {	"FIELD", 0};
static TEXT_PTR ExternalFile[] = {	"EXTERNAL FILE", 0};
static TEXT_PTR mfb[] = {	"MERGE EQUIVALENCE FILE BLOCK", 0};
static TEXT_PTR River[] = {	"SORT MERGE RIVER", 0};
static TEXT_PTR plc[] = {	"PAGE/LOCK SERVER CONNECTION", 0};
static TEXT_PTR usr[] = {	"USER IDENTIFICATION BLOCK ", 0};
static TEXT_PTR Attachment[] = {	"ATTACHMENT BLOCK", 0};
static TEXT_PTR sym[] = {	"SYMBOL", 0};
static TEXT_PTR fun[] = {	"FUNCTION", 0};
static TEXT_PTR irl[] = {	"INDEXED RELATIONSHIP", 0};
static TEXT_PTR acc[] = {	"ACCESS", 0};
static TEXT_PTR Resource[] = {	"RESOURCE", 0};
static TEXT_PTR idl[] = {	"INDEX LOCK", 0};
static TEXT_PTR Shadow[] = {	"SHADOW", 0};
static TEXT_PTR Savepoint[] = {	"SAVE POINT", 0};
static TEXT_PTR VerbAction[] = {	"VERB", 0};
static TEXT_PTR BlockingThread[] = {	"BLOCKED THREAD", 0};
static TEXT_PTR blf[] = {	"BLOB FILTER", 0};
static TEXT_PTR arr[] = {	"ARRAY DESCRIPTION", 0};
static TEXT_PTR blb_map[] = {	"MAP BLOCK", 0};
static TEXT_PTR fblog[] = {	"LOG BLOCK", 0};
static TEXT_PTR dls[] = {	"DIR LIST BLOCK", 0};
static TEXT_PTR jrd_prc[] =
{
	"PROCEDURE",
		FLD(jrd_prc*, "%s", prc_name),
		FLD(jrd_prc*, "Id: %d", prc_id), 0
};
static TEXT_PTR prm[] = {	"PARAMETER", FLD(PRM, "%s", prm_name), 0};
static TEXT_PTR idb[] = {	"INDEX BLOCK", 0};
static TEXT_PTR bkm[] = {	"BOOKMARK BLOCK", 0};
static TEXT_PTR rng[] = {	"REFRESH RANGE BLOCK", 0};
static TEXT_PTR tpc[] = {	"TIP CACHE BLOCK", 0};
static TEXT_PTR xcp[] = {	"EXCEPTION LIST BLOCK", 0};
static TEXT_PTR Opt[] = {	"OPTIMIZATION BLOCK", 0};
static TEXT_PTR Prefetch[] = {	"PRF", 0};
static TEXT_PTR rse[] = {	"RECORD SELECTION EXPRESSION", 0};
static TEXT_PTR Literal[] = {	"LITERAL", 0};
static TEXT_PTR asb[] = {	"ASB", 0};
static TEXT_PTR srl[] = {	"SRL", 0};

	/* xxx
	   x [] = {
	   "x",
	   FLD (x, "x: %x", x),
	   0},
	 */
static TEXT_PTR Csb[] = {
	"COMPILE SCRATCH BLOCK",
		FLD(Csb*, "Count: %x", csb_count),
		FLD(Csb*, "Node: %x", csb_node),
		FLD(Csb*, "Streams: %x", csb_n_stream),
		FLD(Csb*, "Running: %x", csb_running),
		FLD(Csb*, "BLR: %x", csb_blr),
		0
};

static TEXT_PTR texttype[] = {
/*	"INTL TEXT OBJECT",
		FLD(TEXTTYPE, "Name: %s", texttype_name),
		FLD(TEXTTYPE, "Vers: %d", texttype_version),
		FLD(TEXTTYPE, "ID:   %d", texttype_type),
		FLD(TEXTTYPE, "CS:   %d", texttype_character_set),
		FLD(TEXTTYPE, "Cntry:%d", texttype_country),
		FLD(TEXTTYPE, "Flags:%d", texttype_flags),*/
		0
};
static TEXT_PTR charset[] = {
/*	"INTL Character Set",
		FLD(charset*, "Name: %s", charset_name),
		FLD(charset*, "Vers: %d", charset_version),
		FLD(charset*, "ID:   %d", charset_id),
		FLD(charset*, "B/Ch: %d", charset_max_bytes_per_char),
		FLD(charset*, "B/Ch: %d", charset_min_bytes_per_char),
		FLD(charset*, "Flags:%d", charset_flags), */
		0
};
static TEXT_PTR csconvert[] = {
/*	"INTL Character set converter",
		FLD(csconvert*, "Name: %s", csconvert_name),
		FLD(csconvert*, "from: %d", csconvert_from),
		FLD(csconvert*, "to:   %d", csconvert_to), */
		0
};
static TEXT_PTR thread_db[] = {

	"THREAD DATA BLOCK",
		FLD(thread_db*, "Status vec: %x", tdbb_status_vector),
		FLD(thread_db*, "Default: %x", tdbb_default),
		0
};
static TEXT_PTR Service[] =		{	"SERVICE MANAGER BLOCK", 0};
static TEXT_PTR LatchWait[] =		{	"LATCH WAIT BLOCK", 0};
static TEXT_PTR ViewContext[] =		{	"VIEW CONTEXT BLOCK", 0};
static TEXT_PTR srpb[] =	{	"RPB BLOCK", 0};


static int (*dbg_all) (), (*dbg_block) (), (*dbg_examine) (), (*dbg_eval) (),
	(*dbg_open) (), (*dbg_close) (), (*dbg_pool) (), (*dbg_pretty) (),
	(*dbg_window) (), (*dbg_rpb) (), (*dbg_bdbs) (), (*dbg_analyze) (),
	(*dbg_check) (), (*dmp_page) (), (*dmp_active) (), (*dmp_dirty) (),
	(*dbg_verify) ();
#define SYM(struct, name)	"name", OFFSET (struct, name), 0, symb_offset, sizeof (((struct) NULL)->name),

struct symb dbt_symbols[] = {
	{"blk", &dbg_block, symb_printer, sizeof(int)},
	{"ev", &dbg_eval, symb_printer, sizeof(int)},
	{"ex", &dbg_examine, symb_printer, sizeof(int)},
	{"dump", &dmp_page, symb_printer, sizeof(SLONG)},
	{"pool", &dbg_pool, symb_printer, 0},
	{"pretty", &dbg_pretty, symb_printer, 0},
	{"analyze", &dbg_analyze, symb_printer, 0},
	{"window", &dbg_window, symb_printer, 0},
	{"rpb", &dbg_rpb, symb_printer, 0},
	{"check", &dbg_check, symb_printer, 0},

	{"dirty", &dmp_dirty, symb_routine, 0},
	{"active", &dmp_active, symb_routine, 0},
	{"all", &dbg_all, symb_routine, 0},
	{"open", &dbg_open, symb_routine, 0},
	{"close", &dbg_close, symb_routine, 0},
	{"bdbs", &dbg_bdbs, symb_routine, 0},
	{"verify", &dbg_verify, symb_routine, 0},

/*
    "Database", &Database, symb_absolute, sizeof(Database),
    SYM (Database*, dbb_bcb)
    SYM (Database*, dbb_relations)
    SYM (Database*, dbb_pools)
    SYM (Database*, dbb_requests)
    SYM (REL, rel_formats)
    SYM (REL, rel_pages)
    SYM (jrd_req*, req_top_node)
    SYM (jrd_req*, req_next)
*/
	{NULL, 0, symb_routine, 0}
};

#define BLKDEF(type, name, tail) (TEXT*) name,

#define Database dbb_stuff

TEXT* dbt_blocks[] =
{
	0,
#include "../jrd/blk.h"
	0
};
#undef BLKDEF



const char* DBT_jrd_type_map(int type)
{
#define BLKDEF(enum_c, str_c, ext) case enum_c: return str_c[0];
	switch (type)
	{
	#include "../jrd/blk.h"
	default:
		break;
	}
#undef BLKDEF
	return "Uknown Jrd Obj";
}

void MP_GDB_print(MemoryPool *p)
{
	p->print_contents(stdout, DBT_jrd_type_map);
}

