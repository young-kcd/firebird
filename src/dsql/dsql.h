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
 */

#ifndef _DSQL_DSQL_H_
#define _DSQL_DSQL_H_

#include "../jrd/ibsetjmp.h"
#include "../jrd/common.h"

/* Dynamic SQL Error Status Block */

typedef struct err {
	STATUS *dsql_status;
	STATUS *dsql_user_status;
	JMP_BUF dsql_env;			/* Error return environment */
} *ERR;

/* macros for block allocation */

#define ALL_release(blk)	 ALLD_release (blk)
#define ALLOCD(type)		 ALLD_alloc (tdsql->tsql_default, type, 0)
#define ALLOCDV(type,repeat)	 ALLD_alloc (tdsql->tsql_default, type, repeat)
#define ALLOCP(type)		 ALLD_alloc (DSQL_permanent_pool, type, 0)
#define ALLOCPV(type,repeat)	 ALLD_alloc (DSQL_permanent_pool, type, repeat)
#define ALLOC(type,pool)	 ALLD_alloc (pool, type, 0)
#define ALLOCV(type,pool,repeat) ALLD_alloc (pool, type, repeat)

/* this table is used in data allocation to determine
   whether a block has a variable length tail */

#define BLKDEF(type, root, tail) type,

ENUM blk_t {
	type_MIN = 0,
#include "../dsql/blk.h"
type_MAX};

#undef BLKDEF

/* generic block used as header to all allocated structures */

#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif

/* aggregate blocks to store vectors of objects */

typedef struct vec {
	struct blk vec_header;
	ULONG vec_count;
	struct blk *vec_object[1];
} *VEC;

typedef struct vcl {
	struct blk vcl_header;
	ULONG vcl_count;
	SLONG vcl_long[1];
} *VCL;

/* generic structure used to store strings */

typedef struct str
{
	struct blk str_header;
	TEXT*	str_charset;			/* ASCIIZ Character set identifier for string */
	USHORT	str_flags;
	USHORT	str_length;			/* length of string in BYTES */
	UCHAR	str_data[2];			/* one for ALLOC and one for the NULL */
} *STR;

/* values used in str_flags */

#define	STR_delimited_id	0x1L

/* macros and block used to implement a generic stack mechanism */

#define LLS_PUSH(object,stack)		ALLD_push ((BLK) object, stack)
#define LLS_POP(stack)			ALLD_pop (stack)

typedef struct lls {
	struct blk lls_header;
	struct blk *lls_object;
	struct lls *lls_next;
} *LLS;


/* blocks used internally to the data allocator */

/* free block used to point to an unused block */

typedef struct frb {
	struct blk frb_header;
	struct frb *frb_next;		/* Next free block in pool */
} *FRB;

/* Pool block used to store a set of blocks to be treated as an aggregate */

typedef struct plb {
	struct blk plb_header;
	USHORT plb_pool_id;			/* pool id */
	struct frb *plb_free;		/* first free block */
	struct hnk *plb_hunks;		/* first hunk block */
	struct lls *plb_lls;		/* available linked list stack nodes */
	SLONG *plb_blk_type_count;	/* array to keep track of block types */
} *PLB;

/* Hunk blocks, used to allow dynamic expansion of an allocated pool */

typedef struct hnk {
	struct blk hnk_header;
	UCHAR *hnk_address;			/* start of memory hunk */
	int hnk_length;				/* length of memory hunk */
	struct hnk *hnk_next;		/* next memory hunk in structure */
} *HNK;



/*======================================================================
   remaining node definitions for local processing
*/

/* Include definition of descriptor */

#include "../jrd/dsc.h"

/* internal DSQL requests */


#if 0

#define irq_relation	0	/* lookup a relation */
#define irq_fields		1	/* lookup a relation's fields */
#define irq_dimensions	2	/* lookup a field's dimensions */
#define irq_primary_key	3	/* lookup a primary key */
#define irq_view		4	/* lookup a view's base relations */
#define irq_function	5	/* lookup a user defined function */
#define irq_func_return	6	/* lookup a function's return argument */
#define irq_procedure	7	/* lookup a stored procedure */
#define irq_parameters	8	/* lookup a procedure's parameters */
#define irq_collation	9	/* lookup a collation name */
#define irq_charset		10	/* lookup a character set name */
#define irq_trigger		11	/* lookup a trigger */
#define irq_domain		12	/* lookup a domain */
#define irq_type		13	/* lookup a symbolic name in RDB$TYPES */
#define irq_col_default	14	/* lookup default for a column */
#define irq_domain_2	15	/* lookup a domain */

#define irq_MAX			16

#else

enum irq_type_t {
	irq_relation	= 0,
	irq_fields		= 1,
	irq_dimensions	= 2,
	irq_primary_key	= 3,
	irq_view		= 4,
	irq_function	= 5,
	irq_func_return	= 6,
	irq_procedure	= 7,
	irq_parameters	= 8,
	irq_collation	= 9,
	irq_charset		= 10,
	irq_trigger		= 11,
	irq_domain		= 12,
	irq_type		= 13,
	irq_col_default	= 14,
	irq_domain_2	= 15,

	irq_MAX			= 16
};

#endif


/* blocks used to cache metadata */

typedef struct dbb {			/* Database Block */
	struct blk dbb_header;
	struct dbb *dbb_next;
	struct dsql_rel *dbb_relations;	/* known relations in database */
	struct prc *dbb_procedures;	/* known procedures in database */
	struct udf *dbb_functions;	/* known functions in database */
	struct plb *dbb_pool;
	SLONG *dbb_database_handle;
	SLONG *dbb_requests[irq_MAX];
	struct str *dbb_dfl_charset;
	USHORT dbb_base_level;		/* indicates the version of the engine code itself */
	USHORT dbb_flags;
	USHORT dbb_db_SQL_dialect;
	SSHORT dbb_att_charset;		/* characterset at time of attachment */
} *DBB;

/* values used in dbb_flags */

#define DBB_no_arrays	0x1
#define DBB_v3		0x2
#define DBB_no_charset	0x4
#define DBB_read_only	0x8

typedef struct dsql_rel		/* Relation block */
{
	struct blk			rel_header;
	struct dsql_rel*	rel_next;			/* Next relation in database */
	struct sym*			rel_symbol;			/* Hash symbol for relation */
	struct fld*			rel_fields;			/* Field block */
	struct dsql_rel*	rel_base_relation;	/* base relation for an updatable view */
	TEXT*				rel_name;			/* Name of relation */
	TEXT*				rel_owner;			/* Owner of relation */
	USHORT				rel_id;				/* Relation id */
	USHORT				rel_dbkey_length;
	USHORT				rel_flags;
	TEXT				rel_data[3];
} *DSQL_REL;

/* rel_flags bits */

#define REL_new_relation	1	/* relation is newly defined, not committed yet */
#define REL_dropped			2	/* relation has been dropped */

typedef struct fld		/* Field block */
{
	struct blk			fld_header;
	struct fld*			fld_next;				/* Next field in relation */
	struct dsql_rel*	fld_relation;			/* Parent relation */
	struct prc*			fld_procedure;			/* Parent procedure */
	struct nod*			fld_ranges;				/* ranges for multi dimension array */
	struct nod*			fld_character_set;		/* null means not specified */
	struct nod*			fld_sub_type_name;		/* Subtype name for later resolution */
	USHORT				fld_flags;
	USHORT				fld_id;					/* Field in in database */
	USHORT				fld_dtype;				/* Data type of field */
	FLD_LENGTH			fld_length;				/* Length of field */
	USHORT				fld_element_dtype;		/* Data type of array element */
	USHORT				fld_element_length;		/* Length of array element */
	SSHORT				fld_scale;				/* Scale factor of field */
	SSHORT				fld_sub_type;			/* Subtype for text & blob fields */
	USHORT				fld_precision;			/* Precision for exact numeric types */
	USHORT				fld_character_length;	/* length of field in characters */
	USHORT				fld_seg_length;			/* Segment length for blobs */
	SSHORT				fld_dimensions;			/* Non-zero means array */
	SSHORT				fld_character_set_id;	/* ID of field's character set */
	SSHORT				fld_collation_id;		/* ID of field's collation */
	SSHORT				fld_ttype;				/* ID of field's language_driver */
	TEXT fld_name[2];
} *FLD;

/* values used in fld_flags */

#define FLD_computed	1
#define FLD_drop	2
#define FLD_dbkey	4
#define FLD_national	8		/* field uses NATIONAL character set */
#define FLD_nullable	16

#define MAX_ARRAY_DIMENSIONS 16	/* max array dimensions */

/* database/log/cache file block */

typedef struct fil {
	struct blk fil_header;
	SLONG fil_length;			/* File length in pages */
	SLONG fil_start;			/* Starting page */
	struct str *fil_name;		/* File name */
	struct fil *fil_next;		/* next file */
	SSHORT fil_shadow_number;	/* shadow number if part of shadow */
	SSHORT fil_manual;			/* flag to indicate manual shadow */
	SSHORT fil_partitions;		/* number of log file partitions */
	USHORT fil_flags;
} *FIL;

/* Stored Procedure block */

typedef struct prc {			/* Relation block */
	struct blk prc_header;
	struct prc *prc_next;		/* Next relation in database */
	struct sym *prc_symbol;		/* Hash symbol for procedure */
	struct fld *prc_inputs;		/* Input parameters */
	struct fld *prc_outputs;	/* Output parameters */
	TEXT *prc_name;				/* Name of procedure */
	TEXT *prc_owner;			/* Owner of procedure */
	SSHORT prc_in_count;
	SSHORT prc_out_count;
	USHORT prc_id;				/* Procedure id */
	USHORT prc_flags;
	TEXT prc_data[3];
} *PRC;

/* prc_flags bits */

#define PRC_new_procedure	1	/* procedure is newly defined, not committed yet */
#define PRC_dropped		2		/* procedure has been dropped */

/* User defined function block */

typedef struct udf {
	struct blk udf_header;
	struct udf *udf_next;
	struct sym *udf_symbol;		/* Hash symbol for udf */
	USHORT udf_dtype;
	SSHORT udf_scale;
	SSHORT udf_sub_type;
	USHORT udf_length;
	SSHORT udf_character_set_id;
	USHORT udf_character_length;
	TEXT udf_name[2];
} *UDF;

/* these values are multiplied by -1 to indicate that server frees them 
   on return from the udf */
typedef ENUM {
		FUN_value,
		FUN_reference,
		FUN_descriptor, FUN_blob_struct, FUN_scalar_array} FUN_T;

/* Variables - input, output & local */

typedef struct var {			/* Variable block */
	struct blk var_header;
	struct fld *var_field;		/* Field on which variable is based */
	USHORT var_flags;
	USHORT var_msg_number;		/* Message number containing variable */
	USHORT var_msg_item;		/* Item number in message */
	USHORT var_variable_number;	/* Local variable number */
	TEXT var_name[2];
} *VAR;

/* values used in var_flags */

#define VAR_input	1
#define VAR_output	2
#define VAR_local	4


/* Symbolic names for international text types */
/* (either collation or character set name)    */

typedef struct intlsym {		/* International symbol */
	struct blk intlsym_header;
	struct sym *intlsym_symbol;	/* Hash symbol for intlsym */
	USHORT intlsym_type;		/* what type of name */
	USHORT intlsym_flags;
	SSHORT intlsym_ttype;		/* id of implementation */
	SSHORT intlsym_charset_id;
	SSHORT intlsym_collate_id;
	USHORT intlsym_bytes_per_char;
	TEXT intlsym_name[2];
} *INTLSYM;

/* values used in intlsym_type */

#define INTLSYM_collation	1
#define	INTLSYM_charset		2

/* values used in intlsym_flags */



/* Request information */

typedef enum {
	REQ_SELECT, REQ_SELECT_UPD, REQ_INSERT, REQ_DELETE, REQ_UPDATE,
	REQ_UPDATE_CURSOR, REQ_DELETE_CURSOR,
	REQ_COMMIT, REQ_ROLLBACK, REQ_DDL, REQ_EMBED_SELECT,
	REQ_START_TRANS, REQ_GET_SEGMENT, REQ_PUT_SEGMENT, REQ_EXEC_PROCEDURE,
	REQ_COMMIT_RETAIN, REQ_SET_GENERATOR
} REQ_TYPE;

typedef struct req {
	struct blk req_header;
	struct req *req_parent;		/* Source request, if cursor update */
	struct req *req_sibling;	/* Next sibling request, if cursor update */
	struct req *req_offspring;	/* Cursor update requests */
	struct plb *req_pool;
	struct lls *req_context;
	struct sym *req_name;		/* Name of request */
	struct sym *req_cursor;		/* Cursor symbol. if any */
	struct dbb *req_dbb;		/* Database handle */
	int *req_trans;				/* Database transaction handle */
	struct opn *req_open_cursor;
	struct nod *req_ddl_node;	/* Store metadata request */
	struct blb *req_blob;		/* Blob info for blob requests */
	int *req_handle;			/* OSRI request handle */
	struct str *req_blr_string;	/* String block during BLR generation */
	struct msg *req_send;		/* Message to be sent to start request */
	struct msg *req_receive;	/* Per record message to be received */
	struct msg *req_async;		/* Message for sending scrolling information */
	struct par *req_eof;		/* End of file parameter */
	struct par *req_dbkey;		/* Database key for current of */
	struct par *req_rec_version;	/* Record Version for current of */
	struct par *req_parent_rec_version;	/* parent record version */
	struct par *req_parent_dbkey;	/* Parent database key for current of */
	struct dsql_rel *req_relation;	/* relation created by this request (for DDL) */
	struct prc *req_procedure;	/* procedure created by this request (for DDL) */
	struct ctx *req_outer_agg_context;	/* agg context for outer ref */
	BLOB_PTR *req_blr;			/* Running blr address */
	BLOB_PTR *req_blr_yellow;	/* Threshold for upping blr buffer size */
	ULONG req_inserts;			/* records processed in request */
	ULONG req_deletes;
	ULONG req_updates;
	ULONG req_selects;
	REQ_TYPE req_type;			/* Type of request */
	USHORT req_base_offset;		/* place to go back and stuff in blr length */
	USHORT req_context_number;	/* Next available context number */
	USHORT req_scope_level;		/* Scope level for parsing aliases in subqueries */
	USHORT req_message_number;	/* Next available message number */
	USHORT req_loop_number;		/* Next available loop number */
	SSHORT req_inhibit_map;		/* Map inhibit count */
	USHORT req_in_select_list;	/* now processing "select list" */
	USHORT req_in_where_clause;	/* processing "where clause" */
	USHORT req_in_having_clause;	/* processing "having clause" */
	USHORT req_in_order_by_clause;	/* processing "order by clause" */
	USHORT req_error_handlers;	/* count of active error handlers */
	USHORT req_flags;			/* generic flag */
	USHORT req_client_dialect;	/* dialect passed into the API call */
} *REQ;

/* values used in req_flags */

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

/* Blob */

typedef struct blb {
	struct blk blb_header;
	struct nod *blb_field;		/* Related blob field */
	struct par *blb_blob_id;	/* Parameter to hold blob id */
	struct par *blb_segment;	/* Parameter for segments */
	struct nod *blb_from;
	struct nod *blb_to;
	struct msg *blb_open_in_msg;	/* Input message to open cursor */
	struct msg *blb_open_out_msg;	/* Output message from open cursor */
	struct msg *blb_segment_msg;	/* Segment message */
} *BLB;

/* List of open cursors */

typedef struct opn {
	struct blk opn_header;
	struct opn *opn_next;		/* Next open cursor */
	struct req *opn_request;	/* Request owning the cursor */
	SLONG *opn_transaction;		/* Transaction executing request */
} *OPN;

/* Transaction block */

typedef struct tra {
	struct blk tra_header;
	struct tra *tra_next;		/* Next open transaction */
} *TRA;


/* Context block used to create an instance of a relation reference */

typedef struct ctx {
	struct blk ctx_header;
	struct req *ctx_request;	/* Parent request */
	struct dsql_rel *ctx_relation;	/* Relation for context */
	struct prc *ctx_procedure;	/* Procedure for context */
	struct nod *ctx_proc_inputs;	/* Procedure input parameters */
	struct map *ctx_map;		/* Map for aggregates */
	struct nod *ctx_rse;		/* Sub-rse for aggregates */
	struct ctx *ctx_parent;		/* Parent context for aggregates */
	TEXT *ctx_alias;			/* Context alias */
	USHORT ctx_context;			/* Context id */
	USHORT ctx_scope_level;		/* Subquery level within this request */
	USHORT ctx_flags;			/* Various flag values */
} *CTX;

/* Flag values for ctx_flags */

#define CTX_outer_join		(1<<0)	/* reference is part of an outer join */

/* Aggregate/union map block to map virtual fields to their base */

typedef struct map {
	struct blk map_header;
	struct map *map_next;		/* Next map in item */
	struct nod *map_node;		/* Value for map item */
	USHORT map_position;		/* Position in map */
} *MAP;


/* Message block used in communicating with a running request */

#ifndef GUI_TOOLS
typedef struct msg {
	struct blk msg_header;
	struct par *msg_parameters;	/* Parameter list */
	struct par *msg_par_ordered;	/* Ordered parameter list */
	UCHAR *msg_buffer;			/* Message buffer */
	USHORT msg_number;			/* Message number */
	USHORT msg_length;			/* Message length */
	USHORT msg_parameter;		/* Next parameter number */
	USHORT msg_index;			/* Next index into SQLDA */
} *MSG;
#endif

/* Parameter block used to describe a parameter of a message */

typedef struct par {
	struct blk par_header;
	struct msg *par_message;	/* Parent message */
	struct par *par_next;		/* Next parameter in linked list */
	struct par *par_ordered;	/* Next parameter in order of index */
	struct par *par_null;		/* Null parameter, if used */
	struct nod *par_node;		/* Associated value node, if any */
	struct ctx *par_dbkey_ctx;	/* Context of internally requested dbkey */
	struct ctx *par_rec_version_ctx;	/* Context of internally requested record version */
	TEXT *par_name;				/* Parameter name, if any */
	TEXT *par_rel_name;			/* Relation name, if any */
	TEXT *par_owner_name;		/* Owner name, if any */
	TEXT *par_alias;			/* Alias, if any */
	DSC par_desc;				/* Field data type */
	DSC par_user_desc;			/* SQLDA data type */
	USHORT par_parameter;		/* BLR parameter number */
	USHORT par_index;			/* Index into SQLDA, if appropriate */
} *PAR;


#include "../jrd/thd.h"

/* DSQL threading declarations */

typedef struct tsql
{
	struct thdd	tsql_thd_data;
	struct plb*	tsql_default;
	STATUS*		tsql_status;
	STATUS*		tsql_user_status;
	jmp_buf*	tsql_setjmp;
} *TSQL;

#ifdef GET_THREAD_DATA
#undef GET_THREAD_DATA
#endif

#define GET_THREAD_DATA	((TSQL) THD_get_specific())


#ifndef SHLIB_DEFS
#ifdef DSQL_MAIN
PLB DSQL_permanent_pool;
int DSQL_debug;
#else
extern PLB DSQL_permanent_pool;
extern int DSQL_debug;
#endif
#else
extern PLB DSQL_permanent_pool;
extern int DSQL_debug;
#endif



/* macros for error generation */

#define BUGCHECK(string)	ERRD_bugcheck(string)
#define IBERROR(code, string)	ERRD_error(code, string)
#define BLKCHK(blk, type) if (blk->blk_type != (SCHAR) type) BUGCHECK ("expected type")

/* macro to stuff blr */
/* this is used in both ddl.cpp and gen.cpp, and is put here for commonality */

#define STUFF(byte)     ((BLOB_PTR*)request->req_blr < (BLOB_PTR*)request->req_blr_yellow) ?\
			(*request->req_blr++ = (UCHAR)(byte)) : GEN_expand_buffer (request, (UCHAR)(byte))


/* Macros for DEV_BUILD internal consistancy checking */

#ifdef DEV_BUILD

/* Verifies that a pointed to block matches the expected type.
 * Useful to find coding errors & memory globbers.
 */
#define DEV_BLKCHK(blk, typ)	\
	{ \
	if ((blk) && (((BLK) (blk))->blk_type != (typ))) \
	    ERRD_assert_msg (assert_blkchk_msg, assert_filename, (ULONG) __LINE__); \
	}

#define _assert(ex)	{if (!(ex)){(void) ERRD_assert_msg (NULL, assert_filename, __LINE__);}}
#define assert(ex)	_assert(ex)
#define ASSERT_FAIL ERRD_assert_msg (NULL, assert_filename, __LINE__)

/* Define the assert_filename as a static variable to save on codespace */

#define	ASSERT_FILENAME	\
	static UCHAR assert_filename[] = __FILE__; \
	static UCHAR assert_blkchk_msg[] = "Unexpected memory block type";	/* NTX: dev */

#else /* PROD_BUILD */

#define DEV_BLKCHK(blk, typ)
#define _assert(ex)
#define assert(ex)
#define ASSERT_FAIL
#define	ASSERT_FILENAME

#endif /* DEV_BUILD */

#endif /* _DSQL_DSQL_H_ */
