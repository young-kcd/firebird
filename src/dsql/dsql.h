/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql.h
 *	DESCRIPTION:	General Definitions for V4 DSQL module
 *
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
 * 2001.11.26 Claudio Valderrama: include udf_arguments and udf_flags
 *   in the udf struct, so we can load the arguments and check for
 *   collisions between dropping and redefining the udf concurrently.
 *   This closes SF Bug# 409769.
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2004.01.16 Vlad Horsun: added support for default parameters and 
 *   EXECUTE BLOCK statement
 */

#ifndef DSQL_DSQL_H
#define DSQL_DSQL_H

#include "../jrd/common.h"
#include "../dsql/all.h"
#include "../common/classes/array.h"
#include "../common/classes/stack.h"
#define REQUESTER
#include "../jrd/val.h"  // Get rid of duplicated FUN_T enum.
#undef REQUESTER

#ifdef DEV_BUILD
// This macro enables DSQL tracing code
#define DSQL_DEBUG
#endif

#ifdef DSQL_DEBUG
DEFINE_TRACE_ROUTINE(dsql_trace);
#endif

//! Dynamic SQL Error Status Block
struct dsql_err_stblock
{
	ISC_STATUS* dsql_status;
	ISC_STATUS* dsql_user_status;
};


// this table is used in data allocation to determine
// whether a block has a variable length tail
#include "../dsql/blk.h"

// generic block used as header to all allocated structures
#include "../include/fb_blk.h"

#include "../dsql/sym.h"

//! generic data type used to store strings
class dsql_str : public pool_alloc_rpt<char, dsql_type_str>
{
public:
	const char* str_charset;	//!< ASCIIZ Character set identifier for string
	USHORT      str_flags;
	ULONG       str_length;		//!< length of string in BYTES
	char        str_data[2];	//!< one for ALLOC and one for the NULL
};

// values used in str_flags

const long STR_delimited_id		= 0x1L;

// Context aliases used in triggers
const char* const OLD_CONTEXT		= "OLD";
const char* const NEW_CONTEXT		= "NEW";
const char* const TEMP_CONTEXT		= "TEMP";

//! macros and block used to implement a generic stack mechanism
#ifdef NOT_USED_OR_REPLACED
class dsql_lls : public pool_alloc<dsql_type_lls>
{
public:
	blk* lls_object;
	dsql_lls* lls_next;
};

inline void LLS_PUSH(blk* object, dsql_lls** stack)
{
	DsqlMemoryPool::ALLD_push(object, stack);
}

inline blk* LLS_POP(dsql_lls** stack)
{
	return DsqlMemoryPool::ALLD_pop(stack);
}
#endif

class dsql_ctx;
class dsql_str;
class dsql_nod;
typedef Firebird::Stack<dsql_ctx*> DsqlContextStack;
typedef Firebird::Stack<dsql_str*> DsqlStrStack;
typedef Firebird::Stack<dsql_nod*> DsqlNodStack;

//======================================================================
// remaining node definitions for local processing
//

/// Include definition of descriptor

#include "../jrd/dsc.h"

//! internal DSQL requests
enum irq_type_t {
    irq_relation    = 0,     //!< lookup a relation                     
    irq_fields      = 1,     //!< lookup a relation's fields            
    irq_dimensions  = 2,     //!< lookup a field's dimensions           
    irq_primary_key = 3,     //!< lookup a primary key                  
    irq_view        = 4,     //!< lookup a view's base relations        
    irq_function    = 5,     //!< lookup a user defined function        
    irq_func_return = 6,     //!< lookup a function's return argument   
    irq_procedure   = 7,     //!< lookup a stored procedure             
    irq_parameters  = 8,     //!< lookup a procedure's parameters       
    irq_collation   = 9,     //!< lookup a collation name               
    irq_charset     = 10,    //!< lookup a character set name           
    irq_trigger     = 11,    //!< lookup a trigger                      
    irq_domain      = 12,    //!< lookup a domain                       
    irq_type        = 13,    //!< lookup a symbolic name in RDB$TYPES   
    irq_col_default = 14,    //!< lookup default for a column           
    irq_domain_2    = 15,    //!< lookup a domain

    irq_MAX         = 16
};

// dsql_node definition
#include "../dsql/node.h"

// blocks used to cache metadata

//! Database Block
class dsql_dbb : public pool_alloc<dsql_type_dbb>
{
public:
	dsql_dbb*		dbb_next;
	class dsql_rel* dbb_relations;		//!< known relations in database
	class dsql_prc*	dbb_procedures;		//!< known procedures in database
	class dsql_udf*	dbb_functions;		//!< known functions in database
	DsqlMemoryPool*	dbb_pool;			//!< The current pool for the dbb
	FB_API_HANDLE	dbb_database_handle;
	FB_API_HANDLE	dbb_requests[irq_MAX];
	dsql_str*		dbb_dfl_charset;
	USHORT			dbb_base_level;		//!< indicates the version of the engine code itself
	USHORT			dbb_flags;
	USHORT			dbb_db_SQL_dialect;
	SSHORT			dbb_att_charset;	//!< characterset at time of attachment
};

//! values used in dbb_flags
enum dbb_flags_vals {
	DBB_no_arrays	= 0x1,
	DBB_v3			= 0x2,
	DBB_no_charset	= 0x4,
	DBB_read_only	= 0x8
};

//! Relation block
class dsql_rel : public pool_alloc_rpt<SCHAR, dsql_type_dsql_rel>
{
public:
	dsql_rel*	rel_next;			//!< Next relation in database
	dsql_sym*	rel_symbol;			//!< Hash symbol for relation
	class dsql_fld*	rel_fields;		//!< Field block
	dsql_rel*	rel_base_relation;	//!< base relation for an updatable view
	TEXT*		rel_name;			//!< Name of relation
	TEXT*		rel_owner;			//!< Owner of relation
	USHORT		rel_id;				//!< Relation id
	USHORT		rel_dbkey_length;
	USHORT		rel_flags;
	TEXT		rel_data[3];
};

// rel_flags bits
enum rel_flags_vals {
	REL_new_relation	= 1, //!< relation is newly defined, not committed yet
	REL_dropped			= 2, //!< relation has been dropped
	REL_view			= 4, //!< relation is a view 
	REL_external		= 8  //!< relation is an external table
};

class dsql_fld : public pool_alloc_rpt<SCHAR, dsql_type_fld>
{
public:
	dsql_fld*	fld_next;				//!< Next field in relation
	dsql_rel*	fld_relation;			//!< Parent relation
	class dsql_prc*	fld_procedure;			//!< Parent procedure
	dsql_nod*	fld_ranges;				//!< ranges for multi dimension array
	dsql_nod*	fld_character_set;		//!< null means not specified
	dsql_nod*	fld_sub_type_name;		//!< Subtype name for later resolution
	USHORT		fld_flags;
	USHORT		fld_id;					//!< Field in in database
	USHORT		fld_dtype;				//!< Data type of field
	FLD_LENGTH	fld_length;				//!< Length of field
	USHORT		fld_element_dtype;		//!< Data type of array element
	USHORT		fld_element_length;		//!< Length of array element
	SSHORT		fld_scale;				//!< Scale factor of field
	SSHORT		fld_sub_type;			//!< Subtype for text & blob fields
	USHORT		fld_precision;			//!< Precision for exact numeric types
	USHORT		fld_character_length;	//!< length of field in characters
	USHORT		fld_seg_length;			//!< Segment length for blobs
	SSHORT		fld_dimensions;			//!< Non-zero means array
	SSHORT		fld_character_set_id;	//!< ID of field's character set
	SSHORT		fld_collation_id;		//!< ID of field's collation
	SSHORT		fld_ttype;				//!< ID of field's language_driver
	TEXT		fld_name[2];
};

// values used in fld_flags

enum fld_flags_vals {
	FLD_computed	= 1,
	FLD_drop		= 2,
	FLD_dbkey		= 4,
	FLD_national	= 8, //!< field uses NATIONAL character set
	FLD_nullable	= 16
};

const int MAX_ARRAY_DIMENSIONS = 16; //!< max array dimensions

//! database/log/cache file block
class dsql_fil : public pool_alloc<dsql_type_fil>
{
public:
	SLONG	fil_length;			//!< File length in pages
	SLONG	fil_start;			//!< Starting page
	dsql_str*	fil_name;			//!< File name
	dsql_fil*	fil_next;			//!< next file
	SSHORT	fil_shadow_number;	//!< shadow number if part of shadow
	SSHORT	fil_manual;			//!< flag to indicate manual shadow
	SSHORT	fil_partitions;		//!< number of log file partitions
	USHORT	fil_flags;
};

//! Stored Procedure block
class dsql_prc : public pool_alloc_rpt<SCHAR, dsql_type_prc>
{
public:
	dsql_prc*	prc_next;		//!< Next relation in database
	dsql_sym*	prc_symbol;		//!< Hash symbol for procedure
	dsql_fld*	prc_inputs;		//!< Input parameters
	dsql_fld*	prc_outputs;	//!< Output parameters
	TEXT*		prc_name;		//!< Name of procedure
	TEXT*		prc_owner;		//!< Owner of procedure
	SSHORT		prc_in_count;
	SSHORT		prc_def_count;	//!< number of inputs with default values
	SSHORT		prc_out_count;
	USHORT		prc_id;			//!< Procedure id
	USHORT		prc_flags;
	TEXT		prc_data[3];
};

// prc_flags bits

enum prc_flags_vals {
	PRC_new_procedure	= 1, //!< procedure is newly defined, not committed yet
	PRC_dropped			= 2  //!< procedure has been dropped
};

//! User defined function block
class dsql_udf : public pool_alloc_rpt<SCHAR, dsql_type_udf>
{
public:
	dsql_udf*	udf_next;
	dsql_sym*	udf_symbol;		//!< Hash symbol for udf
	USHORT		udf_dtype;
	SSHORT		udf_scale;
	SSHORT		udf_sub_type;
	USHORT		udf_length;
	SSHORT		udf_character_set_id;
	USHORT		udf_character_length;
    dsql_nod*	udf_arguments;
    USHORT      udf_flags;

	TEXT		udf_name[2];
};

// udf_flags bits

enum udf_flags_vals {
	UDF_new_udf		= 1, //!< udf is newly declared, not committed yet
	UDF_dropped		= 2  //!< udf has been dropped
};

// Variables - input, output & local

//! Variable block
class dsql_var : public pool_alloc_rpt<SCHAR, dsql_type_var>
{
public:
	dsql_fld*	var_field;		//!< Field on which variable is based
	USHORT	var_flags;
	USHORT	var_msg_number;		//!< Message number containing variable
	USHORT	var_msg_item;		//!< Item number in message
	USHORT	var_variable_number;	//!< Local variable number
	TEXT	var_name[2];
};

// values used in var_flags
enum var_flags_vals {
	VAR_input	= 1,
	VAR_output	= 2,
	VAR_local	= 4
};


// Symbolic names for international text types
// (either collation or character set name)

//! International symbol
class dsql_intlsym : public pool_alloc_rpt<SCHAR, dsql_type_intlsym>
{
public:
	dsql_sym*	intlsym_symbol;		//!< Hash symbol for intlsym
	USHORT		intlsym_type;		//!< what type of name
	USHORT		intlsym_flags;
	SSHORT		intlsym_ttype;		//!< id of implementation
	SSHORT		intlsym_charset_id;
	SSHORT		intlsym_collate_id;
	USHORT		intlsym_bytes_per_char;
	TEXT		intlsym_name[2];
};

// values used in intlsym_type
enum dsql_intlsym_type_vals {
	DSQL_INTLSYM_collation	= 1,
	DSQL_INTLSYM_charset	= 2
};

// values used in intlsym_flags


// Forward declaration.
class dsql_par;
class dsql_opn;

//! Request information
enum REQ_TYPE
{
	REQ_SELECT, REQ_SELECT_UPD, REQ_INSERT, REQ_DELETE, REQ_UPDATE,
	REQ_UPDATE_CURSOR, REQ_DELETE_CURSOR,
	REQ_COMMIT, REQ_ROLLBACK, REQ_DDL, REQ_EMBED_SELECT,
	REQ_START_TRANS, REQ_GET_SEGMENT, REQ_PUT_SEGMENT, REQ_EXEC_PROCEDURE,
	REQ_COMMIT_RETAIN, REQ_SET_GENERATOR, REQ_SAVEPOINT, 
	REQ_EXEC_BLOCK, REQ_SELECT_BLOCK 
};

class dsql_req : public pool_alloc<dsql_type_req>
{
public:
	// begin - member functions that should be private
	inline void		append_uchar(UCHAR byte);
	inline void		append_ushort(USHORT val);
	inline void		append_ulong(ULONG val);
	void		append_cstring(UCHAR verb, const char* string);
	void		append_string(UCHAR verb, const char* string, USHORT len);
	void		append_number(UCHAR verb, SSHORT number);
	void		begin_blr(UCHAR verb);
	void		end_blr();
	void		append_uchars(UCHAR byte, UCHAR count);
	void		append_ushort_with_length(USHORT val);
	void		append_ulong_with_length(ULONG val);
	void		append_file_length(ULONG length);
	void		append_file_start(ULONG start);
	void		generate_unnamed_trigger_beginning(	bool		on_update_trigger,
													const char*	prim_rel_name,
													const dsql_nod* prim_columns,
													const char*	for_rel_name,
													const dsql_nod* for_columns);
	// end - member functions that should be private

	dsql_req(DsqlMemoryPool& p) 
		: req_pool(p), 
		req_main_context(p), 
		req_context(&req_main_context), 
		req_union_context(p), 
		req_dt_context(p), 
		req_dt_base_context(0), 
		req_blr_data(p),
		req_labels(p), 
		req_cursors(p) { }

	dsql_req*	req_parent;		//!< Source request, if cursor update
	dsql_req*	req_sibling;	//!< Next sibling request, if cursor update
	dsql_req*	req_offspring;	//!< Cursor update requests
	DsqlMemoryPool&	req_pool;
	DsqlContextStack	req_main_context;
	DsqlContextStack*	req_context;
    DsqlContextStack	req_union_context;	//!< Save contexts for views of unions
    DsqlContextStack	req_dt_context;		//!< Save contexts for views of derived tables
    DsqlContextStack*	req_dt_base_context;	//!< Save base context used for processing derived tables
	dsql_sym* req_name;			//!< Name of request
	dsql_sym* req_cursor;		//!< Cursor symbol, if any
	dsql_dbb*	req_dbb;			//!< Database handle
	FB_API_HANDLE	req_trans;			//!< Database transaction handle
	dsql_opn* req_open_cursor;
	dsql_nod* req_ddl_node;		//!< Store metadata request
	dsql_nod* req_blk_node;		//!< exec_block node 
	class dsql_blb* req_blob;			//!< Blob info for blob requests
	FB_API_HANDLE	req_handle;				//!< OSRI request handle
	//dsql_str*	req_blr_string;			//!< String block during BLR generation
	Firebird::HalfStaticArray<BLOB_PTR, 256> req_blr_data;
	class dsql_msg* req_send;		//!< Message to be sent to start request
	class dsql_msg* req_receive;	//!< Per record message to be received
	class dsql_msg* req_async;		//!< Message for sending scrolling information
	dsql_par* req_eof;			//!< End of file parameter
	dsql_par* req_dbkey;		//!< Database key for current of
	dsql_par* req_rec_version;	//!< Record Version for current of
	dsql_par* req_parent_rec_version;	//!< parent record version
	dsql_par* req_parent_dbkey;	//!< Parent database key for current of
	dsql_rel* req_relation;	//!< relation created by this request (for DDL)
	dsql_prc* req_procedure;	//!< procedure created by this request (for DDL)
	class dsql_ctx* req_outer_agg_context;	//!< agg context for outer ref
	//BLOB_PTR* req_blr;			//!< Running blr address
	//BLOB_PTR* req_blr_yellow;	//!< Threshold for upping blr buffer size
	ULONG	req_inserts;			//!< records processed in request
	ULONG	req_deletes;
	ULONG	req_updates;
	ULONG	req_selects;
	REQ_TYPE req_type;			//!< Type of request
	ULONG	req_base_offset;		//!< place to go back and stuff in blr length
	USHORT	req_context_number;	//!< Next available context number
	USHORT	req_scope_level;		//!< Scope level for parsing aliases in subqueries
	USHORT	req_message_number;	//!< Next available message number
	USHORT	req_loop_level;		//!< Loop level
	DsqlStrStack	req_labels;			//!< Loop labels
	USHORT	req_cursor_number;	//!< Cursor number
	DsqlNodStack	req_cursors;		//!< Cursors
	USHORT	req_in_select_list;	//!< now processing "select list"
	USHORT	req_in_where_clause;	//!< processing "where clause"
	USHORT	req_in_group_by_clause;	//!< processing "group by clause"
	USHORT	req_in_having_clause;	//!< processing "having clause"
	USHORT	req_in_order_by_clause;	//!< processing "order by clause"
	USHORT	req_error_handlers;	//!< count of active error handlers
	USHORT	req_flags;			//!< generic flag
	USHORT	req_client_dialect;	//!< dialect passed into the API call
	USHORT	req_in_outer_join;	//!< processing inside outer-join part
	dsql_str*		req_alias_relation_prefix;	//!< prefix for every relation-alias.

private:
	// Request should never be destroyed using delete.
	// It dies together with it's pool in release_request().
	~dsql_req();
	// To avoid posix warning about missing public destructor declare 
	// MemoryPool as friend class. In fact IT releases request memory!
	friend class MemoryPool;
};


// values used in req_flags
enum req_flags_vals {
	REQ_cursor_open			= 1,
	REQ_save_metadata		= 2,
	REQ_prepared			= 4,
	REQ_embedded_sql_cursor	= 8,
	REQ_procedure			= 16,
	REQ_trigger				= 32,
	REQ_orphan				= 64,
	REQ_enforce_scope		= 128,
	REQ_no_batch			= 256,
	REQ_backwards			= 512,
	REQ_blr_version4		= 1024,
	REQ_blr_version5		= 2048,
	REQ_exec_block			= 4096
};

//! Blob
class dsql_blb : public pool_alloc<dsql_type_blb>
{
public:
	dsql_nod*	blb_field;			//!< Related blob field
	dsql_par*	blb_blob_id;		//!< Parameter to hold blob id
	dsql_par*	blb_segment;		//!< Parameter for segments
	dsql_nod* blb_from;
	dsql_nod* blb_to;
	class dsql_msg*	blb_open_in_msg;	//!< Input message to open cursor
	class dsql_msg*	blb_open_out_msg;	//!< Output message from open cursor
	class dsql_msg*	blb_segment_msg;	//!< Segment message
};

//! List of open cursors
class dsql_opn : public pool_alloc<dsql_type_opn>
{
public:
	dsql_opn*	opn_next;			//!< Next open cursor
	dsql_req*	opn_request;		//!< Request owning the cursor
	FB_API_HANDLE		opn_transaction;	//!< Transaction executing request
};


//! Transaction block
class dsql_tra : public pool_alloc<dsql_type_tra>
{
public:
	dsql_tra* tra_next;		//!< Next open transaction
};


//! Context block used to create an instance of a relation reference
class dsql_ctx : public pool_alloc<dsql_type_ctx>
{
public:
	dsql_ctx(MemoryPool &p)
		: ctx_childs_derived_table(p) {}

	dsql_req*			ctx_request;		//!< Parent request
	dsql_rel*			ctx_relation;		//!< Relation for context
	dsql_prc*			ctx_procedure;		//!< Procedure for context
	dsql_nod*			ctx_proc_inputs;	//!< Procedure input parameters
	class dsql_map*		ctx_map;			//!< Map for aggregates
	dsql_nod*			ctx_rse;			//!< Sub-rse for aggregates
	dsql_ctx*			ctx_parent;			//!< Parent context for aggregates
	TEXT*				ctx_alias;			//!< Context alias
	USHORT				ctx_context;		//!< Context id
	USHORT				ctx_scope_level;	//!< Subquery level within this request
	USHORT				ctx_flags;			//!< Various flag values
	DsqlContextStack	ctx_childs_derived_table;	//!< Childs derived table context

	dsql_ctx& operator=(dsql_ctx& v)
	{
		ctx_request = v.ctx_request;
		ctx_relation = v.ctx_relation;
		ctx_procedure = v.ctx_procedure;
		ctx_proc_inputs = v.ctx_proc_inputs;
		ctx_map = v.ctx_map;
		ctx_rse = v.ctx_rse;
		ctx_parent = v.ctx_parent;
		ctx_alias = v.ctx_alias;
		ctx_context = v.ctx_context;
		ctx_scope_level = v.ctx_scope_level;
		ctx_flags = v.ctx_flags;
		ctx_childs_derived_table.assign(v.ctx_childs_derived_table);

		return *this;
	}
};

// Flag values for ctx_flags

const USHORT CTX_outer_join = 0x01;	// reference is part of an outer join

//! Aggregate/union map block to map virtual fields to their base
//! TMN: NOTE! This datatype should definitely be renamed!
class dsql_map : public pool_alloc<dsql_type_map>
{
public:
	dsql_map*	map_next;			//!< Next map in item
	dsql_nod*	map_node;			//!< Value for map item
	USHORT		map_position;		//!< Position in map
};

//! Message block used in communicating with a running request
class dsql_msg : public pool_alloc<dsql_type_msg>
{
public:
	dsql_par*	msg_parameters;	//!< Parameter list
	dsql_par*	msg_par_ordered;	//!< Ordered parameter list
	UCHAR*		msg_buffer;			//!< Message buffer
	USHORT		msg_number;			//!< Message number
	USHORT		msg_length;			//!< Message length
	USHORT		msg_parameter;		//!< Next parameter number
	USHORT		msg_index;			//!< Next index into SQLDA
};

//! Parameter block used to describe a parameter of a message
class dsql_par : public pool_alloc<dsql_type_par>
{
public:
	dsql_msg*	par_message;		//!< Parent message
	dsql_par*	par_next;			//!< Next parameter in linked list
	dsql_par*	par_ordered;		//!< Next parameter in order of index
	dsql_par*	par_null;			//!< Null parameter, if used
	dsql_nod*	par_node;			//!< Associated value node, if any
	dsql_ctx*	par_dbkey_ctx;		//!< Context of internally requested dbkey
	dsql_ctx*	par_rec_version_ctx;	//!< Context of internally requested record version
	const TEXT*	par_name;			//!< Parameter name, if any
	const TEXT*	par_rel_name;		//!< Relation name, if any
	const TEXT*	par_owner_name;		//!< Owner name, if any
	const TEXT*	par_alias;			//!< Alias, if any
	DSC			par_desc;			//!< Field data type
	DSC			par_user_desc;		//!< SQLDA data type
	USHORT		par_parameter;		//!< BLR parameter number
	USHORT		par_index;			//!< Index into SQLDA, if appropriate
};

#include "../jrd/thd.h"

// DSQL threading declarations

class tsql : public ThreadData
{
private:
	DsqlMemoryPool*		tsql_default;
	friend class Firebird::SubsystemContextPoolHolder <tsql, DsqlMemoryPool>;

	void setDefaultPool(DsqlMemoryPool* p)
	{
		tsql_default = p;
	}

public:
	tsql(ISC_STATUS* status) 
		: ThreadData(tddSQL), tsql_default(0), 
		tsql_status(status), tsql_user_status(0)
	{
	}

	ISC_STATUS*		tsql_status;
	ISC_STATUS*		tsql_user_status;

	DsqlMemoryPool* getDefaultPool()
	{
		return tsql_default;
	}
};

typedef Firebird::SubsystemContextPoolHolder <tsql, DsqlMemoryPool> 
	DsqlContextPoolHolder;

inline tsql* DSQL_get_thread_data() {
	return (tsql*) ThreadData::getSpecific();
}

inline void DSQL_set_thread_data(tsql* &tdsql, tsql* thd_context) {
	tdsql = thd_context;
	tdsql->putSpecific();
}

inline void DSQL_restore_thread_data() {
	ThreadData::restoreSpecific();
}

/*! \var unsigned DSQL_debug
    \brief Debug level 
    
    0       No output
    1       Display output tree in PASS1_statment
    2       Display input tree in PASS1_statment
    4       Display ddl BLR
    8       Display BLR
    16      Display PASS1_rse input tree
    32      Display SQL input string
    64      Display BLR in dsql/prepare
    > 256   Display yacc parser output level = DSQL_level>>8
*/

// macros for error generation

#define BLKCHK(blk, type) if (MemoryPool::blk_type(blk) != (SSHORT) type) ERRD_bugcheck("expected type")

#ifdef DSQL_DEBUG
	extern unsigned DSQL_debug;
#endif

#ifdef DEV_BUILD
// Verifies that a pointed to block matches the expected type.
// Useful to find coding errors & memory globbers.

#define DEV_BLKCHK(blk, typ)	{						\
		if ((blk) && MemoryPool::blk_type(blk) != (SSHORT)typ) {	\
			ERRD_assert_msg("Unexpected memory block type",			\
							(char*) __FILE__,			\
							(ULONG) __LINE__);			\
		}												\
	}

#undef fb_assert
void ERRD_assert_msg(const char*, const char*, ULONG);
#define fb_assert(ex)	{if (!(ex)) {ERRD_assert_msg (NULL, (char*)__FILE__, __LINE__);}}

#else // PROD_BUILD

#define DEV_BLKCHK(blk, typ)
#undef fb_assert
#define fb_assert(ex)

#endif // DEV_BUILD

#endif // DSQL_DSQL_H

