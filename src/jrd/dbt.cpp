/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbt.c
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
#include "../jrd/everything.h"
#include "../jrd/dbg.h"

#define FLD(struct, string, field) string, (SCHAR*) OFFSET (struct, field), (SCHAR*) sizeof (((struct) NULL)->field)

extern "C" {


typedef SCHAR *TEXT_PTR;

int
 *ptr;

TEXT_PTR dbt_window[] = {
	FLD(WIN *, "Page: %ld", win_page),
	FLD(WIN *, "Buffer: %x", win_buffer),
	FLD(WIN *, "BDB: %x", win_bdb),
	FLD(WIN *, "Scans: %d", win_scans),
	FLD(WIN *, "Flags: %x", win_flags),
	0
},
dbt_rpb[] = {

	FLD(RPB *, "Relation %x", rpb_relation),
	FLD(RPB *, "Number %ld", rpb_number),
	FLD(RPB *, "Trans %ld", rpb_transaction),
	FLD(RPB *, "Page %ld", rpb_page),
	FLD(RPB *, "Line %x", rpb_line),
	FLD(RPB *, "Back page %ld", rpb_b_page),
	FLD(RPB *, "Line %x", rpb_b_line),
	FLD(RPB *, "Fragment page %ld", rpb_f_page),
	FLD(RPB *, "Line %x", rpb_f_line),
	FLD(RPB *, "Format %x", rpb_format_number),
	FLD(RPB *, "Address: %x ", rpb_address),
	FLD(RPB *, "Length %x", rpb_length),
	FLD(RPB *, "Record %x", rpb_record),
	FLD(RPB *, "Flags %x", rpb_flags),
	0
};

static TEXT_PTR dbb_stuff[] = {
	"DATABASE",
	FLD(DBB, "BCB: %x", dbb_bcb),
	FLD(DBB, "Relations: %x", dbb_relations),
	FLD(DBB, "Lock: %x", dbb_lock),
	FLD(DBB, "File: %x", dbb_file),
	FLD(DBB, "Permanent: %x", dbb_permanent),
	FLD(DBB, "Pools: %x", dbb_pools),
	FLD(DBB, "Page_size: %d", dbb_page_size),
	FLD(DBB, "dp_per_pp: %d", dbb_dp_per_pp),
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

static TEXT_PTR bcb[] = {
	"BUFFER CONTROL",
		FLD(BCB, "Count: %x", bcb_count),
		0
},
bdb[] = {
	"BUFFER DESCRIPTOR",
		FLD(BDB, "Page: %ld", bdb_page),
		FLD(BDB, "Lock: %x", bdb_lock),
		FLD(BDB, "Buffer: %x", bdb_buffer),
		FLD(BDB, "Use count: %x", bdb_use_count),
		FLD(BDB, "Flags: %x", bdb_flags),
		0
},
pre[] = {
	"PRECEDENCE",
		FLD(PRE, "Flags: %x", pre_flags),
		FLD(PRE, "Low: %x", pre_low),
		FLD(PRE, "High: %x", pre_hi),
		0
},
lck[] = {
	"LOCK",
		FLD(LCK, "Parent: %x", lck_parent),
		FLD(LCK, "Object: %x", lck_object),
		FLD(LCK, "Type: %x", lck_type),
		FLD(LCK, "Physical: %x", lck_physical),
		FLD(LCK, "Logical: %x", lck_logical),
		FLD(LCK, "Length: %x", lck_length),
		0
},
fil[] = {
	"FILE",
		FLD(FIL, "File desc: %x", fil_desc),
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
		FLD(JRD_REL, "%s", rel_name),
		FLD(JRD_REL, "Id: %d", rel_id),
		FLD(JRD_REL, "Current format: %x", rel_current_format),
		FLD(JRD_REL, "Formats: %x", rel_formats),
		FLD(JRD_REL, "Pages: %x", rel_pages),
		FLD(JRD_REL, "Root: %ld", rel_index_root),
		0
},
fmt[] = {
	"FORMAT",
		FLD(FMT, "Count: %d", fmt_count),
		FLD(FMT, "Length: %d", fmt_length),
		FLD(FMT, "Version: %d", fmt_version),
		0
},
jrd_req[] = {
	"REQUEST",
		FLD(JRD_REQ, "COUNT: %x", req_count),
		FLD(JRD_REQ, "Impure: %x", req_impure_size),
		FLD(JRD_REQ, "Incarn: %x", req_incarnation),
		FLD(JRD_REQ, "Pool: %x", req_pool),
		FLD(JRD_REQ, "Sub req: %x", req_sub_requests),
		FLD(JRD_REQ, "Trans: %x", req_transaction),
		FLD(JRD_REQ, "Next req: %x", req_request),
		FLD(JRD_REQ, "Msg: %x", req_message),
		FLD(JRD_REQ, "Length: %x", req_length),
		FLD(JRD_REQ, "#msgs: %x", req_nmsgs),
		FLD(JRD_REQ, "Max send: %x", req_msend),
		FLD(JRD_REQ, "Max receive: %x", req_mreceive),
		FLD(JRD_REQ, "Top: %x", req_top_node),
		FLD(JRD_REQ, "Next: %x", req_next),
		FLD(JRD_REQ, "Label: %x", req_label),
		FLD(JRD_REQ, "Op: %x", req_operation),
		FLD(JRD_REQ, "Flags: %x", req_flags),
		0
},
jrd_tra[] = {
	"TRANSACTION",
		FLD(JRD_TRA, "Number: %ld", tra_number),
		FLD(JRD_TRA, "Oldest: %ld", tra_oldest),
		FLD(JRD_TRA, "Next: %x", tra_next),
		FLD(JRD_TRA, "Pool: %x", tra_pool),
		FLD(JRD_TRA, "Lock: %x", tra_lock),
		FLD(JRD_TRA, "Locks: %x", tra_relation_locks),
		FLD(JRD_TRA, "Flags: %x", tra_flags),
		0
},
jrd_nod[] = {
	"NODE",
		FLD(JRD_NOD, "Type: %x", nod_type),
		FLD(JRD_NOD, "Impure: %x", nod_impure),
		0
},
lls[] = {
	"LINKED LIST STACK",
		FLD(LLS, "Object: %x", lls_object),
		FLD(LLS, "Next: %x", lls_next),
		0
},
rec[] = {
	"RECORD",
		FLD(REC, "Format: %x", rec_format),
		0
},
Rsb[] = {
	"RECORD SOURCE BLOCK",
		FLD(RSB, "Type: %x", rsb_type),
		FLD(RSB, "Stream: %x", rsb_stream),
		FLD(RSB, "Relation: %x", rsb_relation),
		FLD(RSB, "Next: %x", rsb_next),
		FLD(RSB, "Count: %x", rsb_count),
		0
},
opt[] = {
	"OPTIMIZER",
		FLD(OPT, "CSB: %x", opt_csb),
		FLD(OPT, "Cnt: %x", opt_count),
		0
},
bms[] = {
	"BIT MAP SEGMENT",
		FLD(BMS, "Min: %x", bms_min),
		FLD(BMS, "Max: %x", bms_max),
		0
},
dfw[] = {
	"DEFERRED WORK BLOCK",
		FLD(DFW, "type: %d", dfw_type),
		FLD(DFW, "next: %x", dfw_next),
		FLD(DFW, "name: %s", dfw_name),
		0
},
tfb[] = {
	"TEMPORY FIELD BLOCK",
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
		FLD(DCC, "next: %x", dcc_next),
		FLD(DCC, "end: %x", dcc_end),
		0
},
sbm[] = {
	"SPARE BIT MAP",
		FLD(SBM, "state: %d", sbm_state),
		FLD(SBM, "count: %d", sbm_count),
		FLD(SBM, "used: %d", sbm_used),
		FLD(SBM, "high water: %d", sbm_high_water),
		FLD(SBM, "number: %d", sbm_number),
		0
},
smb[] = {
	"SORT MAP",
		FLD(SMB, "count: %d", smb_count),
		FLD(SMB, "keys: %d", smb_keys),
		FLD(SMB, "length: %d", smb_length),
		FLD(SMB, "sort key: %x", smb_key_desc),
		0
},
blb[] = {
	"BLOB",
		FLD(BLB, "Relation: %x", blb_relation),
		FLD(BLB, "Count: %d", blb_count),
		FLD(BLB, "Length: %d", blb_length),
		FLD(BLB, "Max seg: %d", blb_max_segment),
		FLD(BLB, "Flags: %x", blb_flags),
		FLD(BLB, "Trans: %x", blb_transaction),
		FLD(BLB, "Next: %x", blb_segment),
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

static TEXT_PTR jrn[] = {	"JOURNAL", 0};
static TEXT_PTR scl[] = {	"SECURITY CLASS", 0};
static TEXT_PTR fld[] = {	"FIELD", 0};
static TEXT_PTR ext[] = {	"EXTERNAL FILE", 0};
static TEXT_PTR mfb[] = {	"MERGE EQUIVALENCE FILE BLOCK", 0};
static TEXT_PTR riv[] = {	"SORT MERGE RIVER", 0};
static TEXT_PTR plc[] = {	"PAGE/LOCK SERVER CONNECTION", 0};
static TEXT_PTR usr[] = {	"USER IDENTIFICATION BLOCK ", 0};
static TEXT_PTR att[] = {	"ATTACHMENT BLOCK", 0};
static TEXT_PTR sym[] = {	"SYMBOL", 0};
static TEXT_PTR fun[] = {	"FUNCTION", 0};
static TEXT_PTR irl[] = {	"INDEXED RELATIONSHIP", 0};
static TEXT_PTR acc[] = {	"ACCESS", 0};
static TEXT_PTR Rsc[] = {	"RESOURCE", 0};
static TEXT_PTR idl[] = {	"INDEX LOCK", 0};
static TEXT_PTR sdw[] = {	"SHADOW", 0};
static TEXT_PTR sav[] = {	"SAVE POINT", 0};
static TEXT_PTR vct[] = {	"VERB", 0};
static TEXT_PTR btb[] = {	"BLOCKED THREAD", 0};
static TEXT_PTR blf[] = {	"BLOB FILTER", 0};
static TEXT_PTR arr[] = {	"ARRAY DESCRIPTION", 0};
static TEXT_PTR map[] = {	"MAP BLOCK", 0};
static TEXT_PTR log[] = {	"LOG BLOCK", 0};
static TEXT_PTR dls[] = {	"DIR LIST BLOCK", 0};
static TEXT_PTR jrd_prc[] = {
	"PROCEDURE",
		FLD(JRD_PRC, "%s", prc_name),
		FLD(JRD_PRC, "Id: %d", prc_id), 0};
static TEXT_PTR prm[] = {	"PARAMETER", FLD(PRM, "%s", prm_name), 0};
static TEXT_PTR idb[] = {	"INDEX BLOCK", 0};
static TEXT_PTR bkm[] = {	"BOOKMARK BLOCK", 0};
static TEXT_PTR rng[] = {	"REFRESH RANGE BLOCK", 0};
static TEXT_PTR tpc[] = {	"TIP CACHE BLOCK", 0};
static TEXT_PTR xcp[] = {	"EXCEPTION LIST BLOCK", 0};
static TEXT_PTR Opt[] = {	"OPTIMIZATION BLOCK", 0};
static TEXT_PTR prf[] = {	"PRF", 0};
static TEXT_PTR rse[] = {	"RECORD SELECTION EXPRESSION", 0};
static TEXT_PTR lit[] = {	"LITERAL", 0};
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
		FLD(CSB, "Count: %x", csb_count),
		FLD(CSB, "Node: %x", csb_node),
		FLD(CSB, "Streams: %x", csb_n_stream),
		FLD(CSB, "Running: %x", csb_running),
		FLD(CSB, "BLR: %x", csb_blr),
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
		FLD(CHARSET, "Name: %s", charset_name),
		FLD(CHARSET, "Vers: %d", charset_version),
		FLD(CHARSET, "ID:   %d", charset_id),
		FLD(CHARSET, "B/Ch: %d", charset_max_bytes_per_char),
		FLD(CHARSET, "B/Ch: %d", charset_min_bytes_per_char),
		FLD(CHARSET, "Flags:%d", charset_flags), */
		0
};
static TEXT_PTR csconvert[] = {
/*	"INTL Character set converter",
		FLD(CSCONVERT, "Name: %s", csconvert_name),
		FLD(CSCONVERT, "from: %d", csconvert_from),
		FLD(CSCONVERT, "to:   %d", csconvert_to), */
		0
};
static TEXT_PTR tdbb[] = {

	"THREAD DATA BLOCK",
		FLD(TDBB, "Status vec: %x", tdbb_status_vector),
		FLD(TDBB, "Default: %x", tdbb_default),
		0
};
static TEXT_PTR logfiles[] = {	"WRITE AHEAD LOG FILE", 0};
static TEXT_PTR svc[] =		{	"SERVICE MANAGER BLOCK", 0};
static TEXT_PTR lwt[] =		{	"LATCH WAIT BLOCK", 0};
static TEXT_PTR vcx[] =		{	"VIEW CONTEXT BLOCK", 0};
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
    "dbb", &dbb, symb_absolute, sizeof (dbb),
    SYM (DBB, dbb_bcb)
    SYM (DBB, dbb_relations)
    SYM (DBB, dbb_pools)
    SYM (DBB, dbb_requests)
    SYM (REL, rel_formats)
    SYM (REL, rel_pages)
    SYM (JRD_REQ, req_top_node)
    SYM (JRD_REQ, req_next)
*/
	{NULL, 0, symb_routine, 0}
};

#define BLKDEF(type, name, tail) (TEXT*) name,

#define dbb dbb_stuff

TEXT* dbt_blocks[] = { 0,
#include "../jrd/blk.h"
	0
};
#undef BLKDEF


} // extern "C"


const char* DBT_jrd_type_map(int type)
{
#define BLKDEF(enum_c, str_c, ext) case enum_c: return str_c[0];
	switch(type)
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

