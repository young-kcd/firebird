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
 *
 */

#ifndef _DSQL_DSQL_H_
#define _DSQL_DSQL_H_

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <stdio.h>
#endif

#include "../jrd/common.h"
#include "../dsql/all.h"
#include "../jrd/y_ref.h"

//! Dynamic SQL Error Status Block
struct err
{
	ISC_STATUS* dsql_status;
	ISC_STATUS* dsql_user_status;
};
typedef err* ERR;


// this table is used in data allocation to determine
// whether a block has a variable length tail
#define BLKDEF(type, root, tail) type,

ENUM blk_t {
	dsql_type_MIN = 0,
#include "../dsql/blk.h"
dsql_type_MAX};

#undef BLKDEF

// generic block used as header to all allocated structures

#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif

//! generic data type used to store strings
class str : public pool_alloc_rpt<char, dsql_type_str>
{
public:
	const char* str_charset;	//!< ASCIIZ Character set identifier for string
	USHORT      str_flags;
	ULONG       str_length;		//!< length of string in BYTES
	char        str_data[2];	//!< one for ALLOC and one for the NULL
};
typedef str* STR;

// values used in str_flags

#define	STR_delimited_id	0x1L

//! macros and block used to implement a generic stack mechanism
class dsql_lls : public pool_alloc<dsql_type_lls>
{
public:
	blk* lls_object;
	dsql_lls* lls_next;
};
typedef dsql_lls* DLLS;

#define LLS_PUSH(object,stack)		DsqlMemoryPool::ALLD_push ((BLK) object, stack)
#define LLS_POP(stack)			DsqlMemoryPool::ALLD_pop (stack)



//======================================================================
// remaining node definitions for local processing
//

/// Include definition of descriptor

#include "../jrd/dsc.h"

#ifdef NOT_USED_OR_REPLACED

#define irq_relation    0   
#define irq_fields      1   
#define irq_dimensions  2   
#define irq_primary_key 3   
#define irq_view        4   
#define irq_function    5   
#define irq_func_return 6   
#define irq_procedure   7   
#define irq_parameters  8   
#define irq_collation   9   
#define irq_charset     10  
#define irq_trigger     11  
#define irq_domain      12  
#define irq_type        13  
#define irq_col_default 14  
#define irq_domain_2    15  

#define irq_MAX         16

#else
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

#endif


// blocks used to cache metadata

//! Database Block
class dbb : public pool_alloc<dsql_type_dbb>
{
public:
	dbb*			dbb_next;
	class dsql_rel* dbb_relations;		//!< known relations in database
	class dsql_prc*	dbb_procedures;		//!< known procedures in database
	class udf*		dbb_functions;		//!< known functions in database
	DsqlMemoryPool*	dbb_pool;			//!< The current pool for the dbb
	FRBRD*			dbb_database_handle;
	FRBRD*			dbb_requests[irq_MAX];
	str*			dbb_dfl_charset;
	USHORT			dbb_base_level;		//!< indicates the version of the engine code itself
	USHORT			dbb_flags;
	USHORT			dbb_db_SQL_dialect;
	SSHORT			dbb_att_charset;	//!< characterset at time of attachment
};
typedef dbb* DBB;

//! values used in dbb_flags
#define DBB_no_arrays	0x1
#define DBB_v3			0x2
#define DBB_no_charset	0x4
#define DBB_read_only	0x8

//! Relation block
class dsql_rel : public pool_alloc_rpt<SCHAR, dsql_type_dsql_rel>
{
public:
	dsql_rel*	rel_next;			//!< Next relation in database
	struct sym*	rel_symbol;			//!< Hash symbol for relation
	class dsql_fld*	rel_fields;		//!< Field block
	dsql_rel*	rel_base_relation;	//!< base relation for an updatable view
	TEXT*		rel_name;			//!< Name of relation
	TEXT*		rel_owner;			//!< Owner of relation
	USHORT		rel_id;				//!< Relation id
	USHORT		rel_dbkey_length;
	USHORT		rel_flags;
	TEXT		rel_data[3];
};
typedef dsql_rel* DSQL_REL;

// rel_flags bits

#define REL_new_relation	1	//!< relation is newly defined, not committed yet
#define REL_dropped			2	//!< relation has been dropped
#define REL_view            4   //!< relation is a view 
#define REL_external        8   //!< relation is an external table


class dsql_fld : public pool_alloc_rpt<SCHAR, dsql_type_fld>
{
public:
	dsql_fld*		fld_next;				//!< Next field in relation
	dsql_rel*	fld_relation;			//!< Parent relation
	class dsql_prc*	fld_procedure;			//!< Parent procedure
	struct dsql_nod*	fld_ranges;				//!< ranges for multi dimension array
	struct dsql_nod*	fld_character_set;		//!< null means not specified
	struct dsql_nod*	fld_sub_type_name;		//!< Subtype name for later resolution
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
typedef dsql_fld* DSQL_FLD;

// values used in fld_flags

#define FLD_computed	1
#define FLD_drop	2
#define FLD_dbkey	4
#define FLD_national	8		//!< field uses NATIONAL character set
#define FLD_nullable	16

#define MAX_ARRAY_DIMENSIONS 16	//!< max array dimensions

//! database/log/cache file block
class fil : public pool_alloc<dsql_type_fil>
{
public:
	SLONG	fil_length;			//!< File length in pages
	SLONG	fil_start;			//!< Starting page
	str*	fil_name;			//!< File name
	fil*	fil_next;			//!< next file
	SSHORT	fil_shadow_number;	//!< shadow number if part of shadow
	SSHORT	fil_manual;			//!< flag to indicate manual shadow
	SSHORT	fil_partitions;		//!< number of log file partitions
	USHORT	fil_flags;
};
typedef fil* FIL;

//! Stored Procedure block
class dsql_prc : public pool_alloc_rpt<SCHAR, dsql_type_prc>
{
public:
	dsql_prc*		prc_next;		//!< Next relation in database
	struct sym*	prc_symbol;		//!< Hash symbol for procedure
	dsql_fld*		prc_inputs;		//!< Input parameters
	dsql_fld*		prc_outputs;	//!< Output parameters
	TEXT*		prc_name;		//!< Name of procedure
	TEXT*		prc_owner;		//!< Owner of procedure
	SSHORT		prc_in_count;
	SSHORT		prc_out_count;
	USHORT		prc_id;			//!< Procedure id
	USHORT		prc_flags;
	TEXT		prc_data[3];
};
typedef dsql_prc* DSQL_PRC;

// prc_flags bits

#define PRC_new_procedure	1	//!< procedure is newly defined, not committed yet
#define PRC_dropped			2	//!< procedure has been dropped

//! User defined function block
class udf : public pool_alloc_rpt<SCHAR, dsql_type_udf>
{
public:
	udf*		udf_next;
	struct sym*	udf_symbol;		//!< Hash symbol for udf
	USHORT		udf_dtype;
	SSHORT		udf_scale;
	SSHORT		udf_sub_type;
	USHORT		udf_length;
	SSHORT		udf_character_set_id;
	USHORT		udf_character_length;
    struct dsql_nod  *udf_arguments;
    USHORT      udf_flags;

	TEXT		udf_name[2];
};
typedef udf* UDF;

//! these values are multiplied by -1 to indicate that server frees them
//! on return from the udf
enum FUN_T
{
	FUN_value,
	FUN_reference,
	FUN_descriptor,
	FUN_blob_struct,
	FUN_scalar_array
};

/* udf_flags bits */

#define UDF_new_udf        1   /*!< udf is newly declared, not committed yet */
#define UDF_dropped        2   /*!< udf has been dropped */

// Variables - input, output & local

//! Variable block
class var : public pool_alloc_rpt<SCHAR, dsql_type_var>
{
public:
	dsql_fld*	var_field;		//!< Field on which variable is based
	USHORT	var_flags;
	USHORT	var_msg_number;		//!< Message number containing variable
	USHORT	var_msg_item;		//!< Item number in message
	USHORT	var_variable_number;	//!< Local variable number
	TEXT	var_name[2];
};
typedef var* VAR;

// values used in var_flags

#define VAR_input	1
#define VAR_output	2
#define VAR_local	4


// Symbolic names for international text types
// (either collation or character set name)

//! International symbol
class intlsym : public pool_alloc_rpt<SCHAR, dsql_type_intlsym>
{
public:
	struct sym*	intlsym_symbol;	//!< Hash symbol for intlsym
	USHORT		intlsym_type;		//!< what type of name
	USHORT		intlsym_flags;
	SSHORT		intlsym_ttype;		//!< id of implementation
	SSHORT		intlsym_charset_id;
	SSHORT		intlsym_collate_id;
	USHORT		intlsym_bytes_per_char;
	TEXT		intlsym_name[2];
};
typedef intlsym* INTLSYM;

// values used in intlsym_type

#define INTLSYM_collation	1
#define	INTLSYM_charset		2

// values used in intlsym_flags



//! Request information
enum REQ_TYPE
{
	REQ_SELECT, REQ_SELECT_UPD, REQ_INSERT, REQ_DELETE, REQ_UPDATE,
	REQ_UPDATE_CURSOR, REQ_DELETE_CURSOR,
	REQ_COMMIT, REQ_ROLLBACK, REQ_DDL, REQ_EMBED_SELECT,
	REQ_START_TRANS, REQ_GET_SEGMENT, REQ_PUT_SEGMENT, REQ_EXEC_PROCEDURE,
	REQ_COMMIT_RETAIN, REQ_SET_GENERATOR, REQ_SAVEPOINT
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
													struct dsql_nod* prim_columns,
													const char*	for_rel_name,
													struct dsql_nod* for_columns);
	// end - member functions that should be private

	dsql_req*	req_parent;		//!< Source request, if cursor update
	dsql_req*	req_sibling;	//!< Next sibling request, if cursor update
	dsql_req*	req_offspring;	//!< Cursor update requests
	DsqlMemoryPool*	req_pool;
	DLLS	req_context;
    DLLS    req_union_context;	//!< Save contexts for views of unions
    DLLS    req_dt_context;		//!< Save contexts for views of derived tables
	struct sym* req_name;		//!< Name of request
	struct sym* req_cursor;		//!< Cursor symbol, if any
	dbb*	req_dbb;			//!< Database handle
	FRBRD*	req_trans;			//!< Database transaction handle
	class opn* req_open_cursor;
	struct dsql_nod* req_ddl_node;	//!< Store metadata request
	class blb* req_blob;			//!< Blob info for blob requests
	FRBRD*	req_handle;				//!< OSRI request handle
	str*	req_blr_string;			//!< String block during BLR generation
	class dsql_msg* req_send;		//!< Message to be sent to start request
	class dsql_msg* req_receive;	//!< Per record message to be received
	class dsql_msg* req_async;		//!< Message for sending scrolling information
	class par* req_eof;			//!< End of file parameter
	class par* req_dbkey;		//!< Database key for current of
	class par* req_rec_version;	//!< Record Version for current of
	class par* req_parent_rec_version;	//!< parent record version
	class par* req_parent_dbkey;	//!< Parent database key for current of
	dsql_rel* req_relation;	//!< relation created by this request (for DDL)
	dsql_prc* req_procedure;	//!< procedure created by this request (for DDL)
	class dsql_ctx* req_outer_agg_context;	//!< agg context for outer ref
	BLOB_PTR* req_blr;			//!< Running blr address
	BLOB_PTR* req_blr_yellow;	//!< Threshold for upping blr buffer size
	ULONG	req_inserts;			//!< records processed in request
	ULONG	req_deletes;
	ULONG	req_updates;
	ULONG	req_selects;
	REQ_TYPE req_type;			//!< Type of request
	USHORT	req_base_offset;		//!< place to go back and stuff in blr length
	USHORT	req_context_number;	//!< Next available context number
	USHORT	req_scope_level;		//!< Scope level for parsing aliases in subqueries
	USHORT	req_message_number;	//!< Next available message number
	USHORT	req_loop_level;		//!< Loop level
	USHORT	req_in_select_list;	//!< now processing "select list"
	USHORT	req_in_where_clause;	//!< processing "where clause"
	USHORT	req_in_group_by_clause;	//!< processing "group by clause"
	USHORT	req_in_having_clause;	//!< processing "having clause"
	USHORT	req_in_order_by_clause;	//!< processing "order by clause"
	USHORT	req_error_handlers;	//!< count of active error handlers
	USHORT	req_flags;			//!< generic flag
	USHORT	req_client_dialect;	//!< dialect passed into the API call
	USHORT	req_in_outer_join;	//!< processing inside outer-join part
	STR		req_alias_relation_prefix;	//!< prefix for every relation-alias.
};
typedef dsql_req* DSQL_REQ;


// values used in req_flags

#define REQ_cursor_open		1
#define REQ_save_metadata	2
#define REQ_prepared		4
#define REQ_embedded_sql_cursor	8
#define REQ_procedure		16
#define REQ_trigger		32
#define REQ_orphan		64
#define REQ_enforce_scope	128
#define REQ_no_batch		256
#define REQ_backwards		512
#define REQ_blr_version4	1024
#define REQ_blr_version5	2048

//! Blob
class blb : public pool_alloc<dsql_type_blb>
{
public:
	struct dsql_nod*	blb_field;			//!< Related blob field
	class par*	blb_blob_id;		//!< Parameter to hold blob id
	class par*	blb_segment;		//!< Parameter for segments
	struct dsql_nod* blb_from;
	struct dsql_nod* blb_to;
	class dsql_msg*	blb_open_in_msg;	//!< Input message to open cursor
	class dsql_msg*	blb_open_out_msg;	//!< Output message from open cursor
	class dsql_msg*	blb_segment_msg;	//!< Segment message
};
typedef blb* BLB;

//! List of open cursors
class opn : public pool_alloc<dsql_type_opn>
{
public:
	opn*		opn_next;			//!< Next open cursor
	dsql_req*	opn_request;		//!< Request owning the cursor
	FRBRD*		opn_transaction;	//!< Transaction executing request
};
typedef opn* OPN;

//! Transaction block
class dsql_tra : public pool_alloc<dsql_type_tra>
{
public:
	dsql_tra* tra_next;		//!< Next open transaction
};
typedef dsql_tra* DSQL_TRA;


//! Context block used to create an instance of a relation reference
class dsql_ctx : public pool_alloc<dsql_type_ctx>
{
public:
	dsql_req*			ctx_request;		//!< Parent request
	dsql_rel*			ctx_relation;		//!< Relation for context
	dsql_prc*			ctx_procedure;		//!< Procedure for context
	struct dsql_nod*	ctx_proc_inputs;	//!< Procedure input parameters
	class map*			ctx_map;			//!< Map for aggregates
	struct dsql_nod*	ctx_rse;			//!< Sub-rse for aggregates
	dsql_ctx*			ctx_parent;			//!< Parent context for aggregates
	TEXT*				ctx_alias;			//!< Context alias
	USHORT				ctx_context;		//!< Context id
	USHORT				ctx_scope_level;	//!< Subquery level within this request
	USHORT				ctx_flags;			//!< Various flag values
	DLLS				ctx_childs_derived_table;	//!< Childs derived table context
};
typedef dsql_ctx* DSQL_CTX;

// Flag values for ctx_flags

#define CTX_outer_join		(1<<0)	// reference is part of an outer join

//! Aggregate/union map block to map virtual fields to their base
//! TMN: NOTE! This datatype should definitely be renamed!
class map : public pool_alloc<dsql_type_map>
{
public:
	map*		map_next;		//!< Next map in item
	struct dsql_nod*	map_node;		//!< Value for map item
	USHORT		map_position;	//!< Position in map
};
typedef map* MAP;



//! Message block used in communicating with a running request
class dsql_msg : public pool_alloc<dsql_type_msg>
{
public:
	class par*	msg_parameters;	//!< Parameter list
	class par*	msg_par_ordered;	//!< Ordered parameter list
	UCHAR*	msg_buffer;			//!< Message buffer
	USHORT	msg_number;			//!< Message number
	USHORT	msg_length;			//!< Message length
	USHORT	msg_parameter;		//!< Next parameter number
	USHORT	msg_index;			//!< Next index into SQLDA
};
typedef dsql_msg *DSQL_MSG;

//! Parameter block used to describe a parameter of a message
class par : public pool_alloc<dsql_type_par>
{
public:
	dsql_msg*	par_message;		//!< Parent message
	class par*	par_next;			//!< Next parameter in linked list
	class par*	par_ordered;		//!< Next parameter in order of index
	class par*	par_null;			//!< Null parameter, if used
	struct dsql_nod*	par_node;			//!< Associated value node, if any
	dsql_ctx*	par_dbkey_ctx;		//!< Context of internally requested dbkey
	dsql_ctx*	par_rec_version_ctx;	//!< Context of internally requested record version
	TEXT*	par_name;			//!< Parameter name, if any
	TEXT*	par_rel_name;		//!< Relation name, if any
	TEXT*	par_owner_name;		//!< Owner name, if any
	TEXT*	par_alias;			//!< Alias, if any
	DSC		par_desc;			//!< Field data type
	DSC		par_user_desc;		//!< SQLDA data type
	USHORT	par_parameter;		//!< BLR parameter number
	USHORT	par_index;			//!< Index into SQLDA, if appropriate
};
typedef par* PAR;


#include "../jrd/thd.h"

// DSQL threading declarations

struct tsql
{
	thdd		tsql_thd_data;
	DsqlMemoryPool*		tsql_default;
	ISC_STATUS*		tsql_status;
	ISC_STATUS*		tsql_user_status;
};
typedef tsql* TSQL;


#ifdef GET_THREAD_DATA
#undef GET_THREAD_DATA
#endif

#define GET_THREAD_DATA	((TSQL) THD_get_specific())
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

#ifndef SHLIB_DEFS
#ifdef DSQL_MAIN
unsigned DSQL_debug;
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
FILE       *redirected_output;
#endif
#else
extern unsigned DSQL_debug;
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
extern FILE    *redirected_output;
#endif
#endif
#else
extern unsigned DSQL_debug;
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
extern FILE    *redirected_output;
#endif
#endif



// macros for error generation

#define BUGCHECK(string)	ERRD_bugcheck(string)
#define IBERROR(code, string)	ERRD_error(code, string)
//#define BLKCHK(blk, type) if (blk->blk_type != (SCHAR) type) BUGCHECK ("expected type")
#define BLKCHK(blk, type) if (MemoryPool::blk_type(blk) != (SSHORT) type) BUGCHECK ("expected type")

// macro to stuff blr
// this is used in both ddl.cpp and gen.cpp, and is put here for commonality

#define STUFF(byte)     ((BLOB_PTR*)request->req_blr < (BLOB_PTR*)request->req_blr_yellow) ?\
			(*request->req_blr++ = (UCHAR)(byte)) : GEN_expand_buffer (request, (UCHAR)(byte))


// Macros for DEV_BUILD internal consistancy checking

#ifdef DEV_BUILD

/* Verifies that a pointed to block matches the expected type.
 Useful to find coding errors & memory globbers.

#define DEV_BLKCHK(blk, typ)	\
	{ \
	if ((blk) && (((BLK) (blk))->blk_type != (typ))) \
	    ERRD_assert_msg (assert_blkchk_msg, assert_filename, (ULONG) __LINE__); \
	}
*/

#define DEV_BLKCHK(blk, typ)	{						\
		if ((blk) && MemoryPool::blk_type(blk) != (SSHORT)typ) {	\
			ERRD_assert_msg((char*)assert_blkchk_msg,			\
							(char*)assert_filename,			\
							(ULONG) __LINE__);			\
		}												\
	}


#define _assert(ex)	{if (!(ex)){(void) ERRD_assert_msg (NULL, (char*)assert_filename, __LINE__);}}
#undef assert
#define assert(ex)	_assert(ex)
#define ASSERT_FAIL ERRD_assert_msg (NULL, (char*)assert_filename, __LINE__)

// Define the assert_filename as a static variable to save on codespace

#define	ASSERT_FILENAME static UCHAR assert_filename[] = __FILE__; 
#define	ASSERT_BLKCHK_MSG static UCHAR assert_blkchk_msg[] = "Unexpected memory block type";	// NTX: dev

#else // PROD_BUILD

#define	ASSERT_FILENAME
#define	ASSERT_BLKCHK_MSG
#define DEV_BLKCHK(blk, typ)
#define _assert(ex)
#undef assert
#define assert(ex)
#define ASSERT_FAIL
#define	ASSERT_FILENAME

#endif // DEV_BUILD

#endif // _DSQL_DSQL_H_
