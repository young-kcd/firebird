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

#include "../jrd/ib_stdio.h"
#include "../jrd/ibase.h"
#include "../jrd/everything.h"
#include "../jrd/dbg.h"

#ifdef DELTA
#define FLD(struct, string, field) 0
#else
#define FLD(struct, string, field) string, (SCHAR*) OFFSET (struct, field), (SCHAR*) sizeof (((struct) 0)->field)
#endif


extern "C" {


typedef SCHAR *TEXT_PTR;

int
 *ptr;

TEXT_PTR dbt_window[] = {
	FLD(WIN *, "Page: %ld", win_page),
	FLD(WIN *, "Buffer: %x", win_buffer),
	FLD(WIN *, "BDB: %x", win_bdb),
	FLD(WIN *, "Scans: %d", win_scans),
	FLD(WIN *, "Flags: %x", win_flags), 0
}, dbt_rpb[] = {

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
		FLD(RPB *, "Flags %x", rpb_flags), 0};

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
}, vec[] = {
"VECTOR", FLD(VEC, "Count %d", vec_count), 0}, vcl[] = {
"VECTOR OF ULONGS", FLD(VCL, "Count %d", vcl_count), 0}, frb[] = {
"FREE", FLD(FRB, "Next %x", frb_next), 0}, hnk[] = {

	"HUNK",
		FLD(HNK, "Addr: %x", hnk_address),
		FLD(HNK, "Len: %d", hnk_length),

		FLD(HNK, "Next: %x", hnk_next), 0}, plb[] = {
	"POOL",
		FLD(PLB, "Id: %d", plb_pool_id),
		FLD(PLB, "Free: %x", plb_free),

		FLD(PLB, "Hunk: %x", plb_hunks), 0}, bcb[] = {
"BUFFER CONTROL", FLD(BCB, "Count: %x", bcb_count), 0}, bdb[] = {

	"BUFFER DESCRIPTOR",
		FLD(BDB, "Page: %ld", bdb_page),
		FLD(BDB, "Lock: %x", bdb_lock),
		FLD(BDB, "Buffer: %x", bdb_buffer),
		FLD(BDB, "Use count: %x", bdb_use_count),
		FLD(BDB, "Flags: %x", bdb_flags), 0}, pre[] = {

	"PRECEDENCE",
		FLD(PRE, "Flags: %x", pre_flags),
		FLD(PRE, "Low: %x", pre_low),

		FLD(PRE, "High: %x", pre_hi), 0}, lck[] = {
	"LOCK",
		FLD(LCK, "Parent: %x", lck_parent),
		FLD(LCK, "Object: %x", lck_object),
		FLD(LCK, "Type: %x", lck_type),
		FLD(LCK, "Physical: %x", lck_physical),
		FLD(LCK, "Logical: %x", lck_logical),
		FLD(LCK, "Length: %x", lck_length), 0}, fil[] = {
"FILE", FLD(FIL, "File desc: %x", fil_desc), 0}, pgc[] = {

	"PAGE CONTROL",
		FLD(PGC, "High water: %d", pgc_high_water),
		FLD(PGC, "Pages/PIP: %x", pgc_ppp),
		FLD(PGC, "First PIP: %x", pgc_pip),
		FLD(PGC, "Bytes/PIP: %x", pgc_bytes), 0}, rel[] = {

	"RELATION",
		FLD(REL, "%s", rel_name),
		FLD(REL, "Id: %d", rel_id),
		FLD(REL, "Current format: %x", rel_current_format),
		FLD(REL, "Formats: %x", rel_formats),
		FLD(REL, "Pages: %x", rel_pages),
		FLD(REL, "Root: %ld", rel_index_root), 0}, fmt[] = {

	"FORMAT",
		FLD(FMT, "Count: %d", fmt_count),
		FLD(FMT, "Length: %d", fmt_length),
		FLD(FMT, "Version: %d", fmt_version), 0}, req[] = {

	"REQUEST",
		FLD(REQ, "COUNT: %x", req_count),
		FLD(REQ, "Impure: %x", req_impure_size),
		FLD(REQ, "Incarn: %x", req_incarnation),
		FLD(REQ, "Pool: %x", req_pool),
		FLD(REQ, "Sub req: %x", req_sub_requests),
		FLD(REQ, "Trans: %x", req_transaction),
		FLD(REQ, "Next req: %x", req_request),
		FLD(REQ, "Msg: %x", req_message),
		FLD(REQ, "Length: %x", req_length),
		FLD(REQ, "#msgs: %x", req_nmsgs),
		FLD(REQ, "Max send: %x", req_msend),
		FLD(REQ, "Max receive: %x", req_mreceive),
		FLD(REQ, "Top: %x", req_top_node),
		FLD(REQ, "Next: %x", req_next),
		FLD(REQ, "Label: %x", req_label),
		FLD(REQ, "Op: %x", req_operation),

		FLD(REQ, "Flags: %x", req_flags), 0}, tra[] = {
	"TRANSACTION",
		FLD(TRA, "Number: %ld", tra_number),
		FLD(TRA, "Oldest: %ld", tra_oldest),
		FLD(TRA, "Next: %x", tra_next),
		FLD(TRA, "Pool: %x", tra_pool),
		FLD(TRA, "Lock: %x", tra_lock),
		FLD(TRA, "Locks: %x", tra_relation_locks),
		FLD(TRA, "Flags: %x", tra_flags), 0}, nod[] = {

	"NODE",
		FLD(NOD, "Type: %x", nod_type),

		FLD(NOD, "Impure: %x", nod_impure), 0}, lls[] = {
	"LINKED LIST STACK",
		FLD(LLS, "Object: %x", lls_object),

		FLD(LLS, "Next: %x", lls_next), 0}, rec[] = {
"RECORD", FLD(REC, "Format: %x", rec_format), 0}, rsb[] = {

	"RECORD SOURCE BLOCK",
		FLD(RSB, "Type: %x", rsb_type),
		FLD(RSB, "Stream: %x", rsb_stream),
		FLD(RSB, "Relation: %x", rsb_relation),
		FLD(RSB, "Next: %x", rsb_next),

		FLD(RSB, "Count: %x", rsb_count), 0}, opt[] = {
	"OPTIMIZER",
		FLD(OPT, "CSB: %x", opt_csb),

		FLD(OPT, "Cnt: %x", opt_count), 0}, bms[] = {
	"BIT MAP SEGMENT",
		FLD(BMS, "Min: %x", bms_min),

		FLD(BMS, "Max: %x", bms_max), 0}, dfw[] = {
	"DEFERRED WORK BLOCK",
		FLD(DFW, "type: %d", dfw_type),
		FLD(DFW, "next: %x", dfw_next),

		FLD(DFW, "name: %s", dfw_name), 0}, tfb[] = {
	"TEMPORY FIELD BLOCK",
		FLD(TFB, "id: %d", tfb_id),
		FLD(TFB, "dtype: %d", tfb_desc.dsc_dtype),
		FLD(TFB, "scale: %d", tfb_desc.dsc_scale),
		FLD(TFB, "len: %d", tfb_desc.dsc_length), 0}, str[] = {
"string", FLD(STR, "length: %d", str_length), 0}, dcc[] = {

	"DATA COMPRESSION CONTROL",
		FLD(DCC, "next: %x", dcc_next),

		FLD(DCC, "end: %x", dcc_end), 0}, sbm[] = {
	"SPARE BIT MAP",
		FLD(SBM, "state: %d", sbm_state),
		FLD(SBM, "count: %d", sbm_count),
		FLD(SBM, "used: %d", sbm_used),
		FLD(SBM, "high water: %d", sbm_high_water),
		FLD(SBM, "number: %d", sbm_number), 0}, smb[] = {

	"SORT MAP",
		FLD(SMB, "count: %d", smb_count),
		FLD(SMB, "keys: %d", smb_keys),
		FLD(SMB, "length: %d", smb_length),
		FLD(SMB, "sort key: %x", smb_key_desc), 0}, blb[] = {

	"BLOB",
		FLD(BLB, "Relation: %x", blb_relation),
		FLD(BLB, "Count: %d", blb_count),
		FLD(BLB, "Length: %d", blb_length),
		FLD(BLB, "Max seg: %d", blb_max_segment),
		FLD(BLB, "Flags: %x", blb_flags),
		FLD(BLB, "Trans: %x", blb_transaction),
		FLD(BLB, "Next: %x", blb_segment), 0}, irb[] = {

	"INDEX RETRIEVAL",
		FLD(IRB, "index: %d", irb_index),
		FLD(IRB, "relation: %x", irb_relation),
		FLD(IRB, "lower bounds: %d", irb_lower_count),
		FLD(IRB, "upper boudns: %d", irb_upper_count), 0}, jrn[] = {
"JOURNAL", 0}, scl[] = {
"SECURITY CLASS", 0}, fld[] = {
"FIELD", 0}, ext[] = {
"EXTERNAL FILE", 0}, mfb[] = {
"MERGE EQUIVALENCE FILE BLOCK", 0}, riv[] = {
"SORT MERGE RIVER", 0}, plc[] = {
"PAGE/LOCK SERVER CONNECTION", 0}, usr[] = {
"USER IDENTIFICATION BLOCK ", 0}, att[] = {
"ATTACHMENT BLOCK", 0}, sym[] = {
"SYMBOL", 0}, fun[] = {
"FUNCTION", 0}, irl[] = {
"INDEXED RELATIONSHIP", 0}, acc[] = {
"ACCESS", 0}, rsc[] = {
"RESOURCE", 0}, idl[] = {
"INDEX LOCK", 0}, sdw[] = {
"SHADOW", 0}, sav[] = {
"SAVE POINT", 0}, vct[] = {
"VERB", 0}, btb[] = {
"BLOCKED THREAD", 0}, blf[] = {
"BLOB FILTER", 0}, arr[] = {
"ARRAY DESCRIPTION", 0}, map[] = {
"MAP BLOCK", 0}, log[] = {
"LOG BLOCK", 0}, dls[] = {
"DIR LIST BLOCK", 0}, prc[] = {
"PROCEDURE", FLD(PRC, "%s", prc_name), FLD(PRC, "Id: %d", prc_id), 0}, prm[] = {
"PARAMETER", FLD(PRM, "%s", prm_name), 0}, idb[] = {
"INDEX BLOCK", 0}, bkm[] = {
"BOOKMARK BLOCK", 0}, rng[] = {
"REFRESH RANGE BLOCK", 0}, tpc[] = {
"TIP CACHE BLOCK", 0}, xcp[] = {
"EXCEPTION LIST BLOCK", 0},

	/* xxx
	   x [] = {
	   "x",
	   FLD (x, "x: %x", x),
	   0},
	 */
	csb[] = {
	"COMPILE SCRATCH BLOCK",
		FLD(CSB, "Count: %x", csb_count),
		FLD(CSB, "Node: %x", csb_node),
		FLD(CSB, "Streams: %x", csb_n_stream),
		FLD(CSB, "Running: %x", csb_running),

		FLD(CSB, "BLR: %x", csb_blr), 0}, texttype[] = {
	"INTL TEXT OBJECT",
		FLD(TEXTTYPE, "Name: %s", texttype_name),
		FLD(TEXTTYPE, "Vers: %d", texttype_version),
		FLD(TEXTTYPE, "ID:   %d", texttype_type),
		FLD(TEXTTYPE, "CS:   %d", texttype_character_set),
		FLD(TEXTTYPE, "Cntry:%d", texttype_country),
		FLD(TEXTTYPE, "Flags:%d", texttype_flags), 0}, charset[] = {

	"INTL Character Set",
		FLD(CHARSET, "Name: %s", charset_name),
		FLD(CHARSET, "Vers: %d", charset_version),
		FLD(CHARSET, "ID:   %d", charset_id),
		FLD(CHARSET, "B/Ch: %d", charset_max_bytes_per_char),
		FLD(CHARSET, "B/Ch: %d", charset_min_bytes_per_char),
		FLD(CHARSET, "Flags:%d", charset_flags), 0}, csconvert[] = {

	"INTL Character set converter",
		FLD(CSCONVERT, "Name: %s", csconvert_name),
		FLD(CSCONVERT, "from: %d", csconvert_from),
		FLD(CSCONVERT, "to:   %d", csconvert_to), 0}, tdbb[] = {

	"THREAD DATA BLOCK",
		FLD(TDBB, "Status vec: %x", tdbb_status_vector),
		FLD(TDBB, "Default: %x", tdbb_default), 0}, logfiles[] = {
"WRITE AHEAD LOG FILE", 0}, svc[] = {
"SERVICE MANAGER BLOCK", 0}, lwt[] = {
"LATCH WAIT BLOCK", 0}, vcx[] = {
"VIEW CONTEXT BLOCK", 0}, srpb[] = {
"RPB BLOCK", 0};

static int (*dbg_all) (), (*dbg_block) (), (*dbg_examine) (), (*dbg_eval) (),
	(*dbg_open) (), (*dbg_close) (), (*dbg_pool) (), (*dbg_pretty) (),
	(*dbg_window) (), (*dbg_rpb) (), (*dbg_bdbs) (), (*dbg_analyze) (),
	(*dbg_check) (), (*dmp_page) (), (*dmp_active) (), (*dmp_dirty) (),
	(*dbg_verify) ();
#define SYM(struct, name)	"name", OFFSET (struct, name), 0, symb_offset, sizeof (((struct) 0)->name),

struct symb dbt_symbols[] = {
	"blk", &dbg_block, symb_printer, sizeof(int),
	"ev", &dbg_eval, symb_printer, sizeof(int),
	"ex", &dbg_examine, symb_printer, sizeof(int),
	"dump", &dmp_page, symb_printer, sizeof(SLONG),
	"pool", &dbg_pool, symb_printer, 0,
	"pretty", &dbg_pretty, symb_printer, 0,
	"analyze", &dbg_analyze, symb_printer, 0,
	"window", &dbg_window, symb_printer, 0,
	"rpb", &dbg_rpb, symb_printer, 0,
	"check", &dbg_check, symb_printer, 0,

	"dirty", &dmp_dirty, symb_routine, 0,
	"active", &dmp_active, symb_routine, 0,
	"all", &dbg_all, symb_routine, 0,
	"open", &dbg_open, symb_routine, 0,
	"close", &dbg_close, symb_routine, 0,
	"bdbs", &dbg_bdbs, symb_routine, 0,
	"verify", &dbg_verify, symb_routine, 0,

/*
    "dbb", &dbb, symb_absolute, sizeof (dbb),
    SYM (DBB, dbb_bcb)
    SYM (DBB, dbb_relations)
    SYM (DBB, dbb_pools)
    SYM (DBB, dbb_requests)
    SYM (REL, rel_formats)
    SYM (REL, rel_pages)
    SYM (REQ, req_top_node)
    SYM (REQ, req_next)
*/
	(SCHAR *) 0, 0, symb_routine, 0
};

#define BLKDEF(type, name, tail) (TEXT*) name,

#define dbb dbb_stuff

TEXT *dbt_blocks[] = { 0,
#include "../jrd/blk.h"
	0
};


} // extern "C"
