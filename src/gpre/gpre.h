/*
 *	PROGRAM:	Language Preprocessor
 *	MODULE:		gpre.h
 *	DESCRIPTION:	Common header modules
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
 * $Id: gpre.h,v 1.33.2.1 2005-02-10 10:25:25 kkuznetsov Exp $
 * Revision 1.3  2000/11/27 09:26:13  fsg
 * Fixed bugs in gpre to handle PYXIS forms
 * and allow edit.e and fred.e to go through
 * gpre without errors (and correct result).
 *
 * This is a partial fix until all
 * PYXIS datatypes are adjusted in frm_trn.c
 *
 * removed some compiler warnings too
 *
 * Revision 1.2  2000/11/16 15:54:29  fsg
 * Added new switch -verbose to gpre that will dump
 * parsed lines to stderr
 *
 * Fixed gpre bug in handling row names in WHERE clauses
 * that are reserved words now (DATE etc)
 * (this caused gpre to dump core when parsing tan.e)
 *
 * Fixed gpre bug in handling lower case table aliases
 * in WHERE clauses for sql dialect 2 and 3.
 * (cause a core dump in a test case from C.R. Zamana)
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                         - "IMP" and "HP9000 s300"
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "SGI" port
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "HP700" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _GPRE_GPRE_H_
#define _GPRE_GPRE_H_

#include <stddef.h>				// offsetof

#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include "../jrd/y_ref.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GPRE_FORTRAN
#if defined AIX || defined AIX_PPC || defined sun
#ifndef BOOT_BUILD 
#define FTN_BLK_DATA    1
#endif
#endif
#endif

#ifdef GPRE_ADA
#if (defined hpux && defined HP10) || defined SCO_UNIX
#define ALSYS_ADA       1
#endif
#endif

#ifdef VMS
#define EITHER_CASE	1
#endif

/* If the C compiler supports the ANSI const directive, we emit it before
   BLR string definitions */

#if (defined (SOLARIS) || defined (AIX) || defined (WIN_NT))
#define CONST_STR	"const "
#endif

#ifndef CONST_STR
#define CONST_STR	""
#endif

#define MAXSYMLEN 	512			/* max length of symbol + terminator */
#define NAME_SIZE 	32
#define SQL_DIALECT_V5 	1		/* pre 6.0 dialect */

#define MAKE_ACTION(request,type) 	MSC_action(request,type)
#define ALLOC(size) 			MSC_alloc (size)
#define MAKE_CONTEXT(request)		MSC_context (request)
#define COPY(from, length, to) 		MSC_copy (from, length, to)
#define COPY_CAT(from1, length1, from2, length2,to) 	MSC_copy_cat(from1, length1, from2, length2,to)
#define FREE(block)			MSC_free (block)
#define MAKE_NODE(type, count)		MSC_node (type, count)
#define POP(stack)			MSC_pop (stack)
#define PUSH(object, stack)		MSC_push (object, stack)
#define REALLOC(block, size)		MSC_realloc (block, size)
#define MAKE_REFERENCE(link)		MSC_reference (link);
#define MAKE_REQUEST(type)		MSC_request (type)
#define MAKE_STRING(string)		MSC_string (string)
#define MAKE_PRIVILEGE_BLOCK()		MSC_privilege_block ()
#define MAKE_USERNAME(string, dyn)	MSC_username (string, dyn)
#define IBERROR				CPR_error

#define RANGE_SHORT_INTEGER(X)	((X) < 32768  &&  (X) >= -32768)
#define RANGE_POSITIVE_SHORT_INTEGER(X) ((X) < 32768 &&  (X) >= 0)

#define SINGLE_QUOTED(typ)	(typ == tok_quoted)
#define DOUBLE_QUOTED(typ)	(typ == tok_dblquoted)
#define QUOTED(typ)		(SINGLE_QUOTED(typ) || DOUBLE_QUOTED(typ))
#define DOUBLE_QUOTES_ON(str)	(str[0] == '\"')
#define REMOVE_DOUBLE_QUOTES(str)	{int ij,ii; \
				for (ii=0, ij=1; str[ij] != '\"'; ij++) \
				    str[ii++] = str[ij];\
				str[ii] = 0;                \
				}

#define STRIP_QUOTES(tkn)	{int ij; \
				for (ij=1; ij<tkn.tok_length-1; ij++) \
				    tkn.tok_string[ij-1] = tkn.tok_string[ij];\
				--ij; \
				tkn.tok_string[ij] = 0;                \
				tkn.tok_length = ij;                \
				}

/* Language options */

typedef enum lang_t
{
	lang_undef,
	lang_internal,
	lang_pascal,
	lang_fortran,
	lang_epascal,
	lang_cobol,
	lang_c,
	lang_ada,
	lang_cxx,
	lang_scxx,
	lang_cplusplus,
	lang_cpp,
	lang_internal_cxx
} LANG_T;

//___________________________________________________________________
// Test if input language is cpp based.
//
// The lang_internal is used to compile some internal stuff without
// reference to a database.  Previously it was based on lang_c (but even then
// if you look carefully there are some areas where lang_c is specifically
// treated and lang_internal would be ignored).
// Now the base language used for lang_internal is cpp, so we have this
// inline function to tell if language is cpp.
// Internally the sw_language variable is set to lang_cxx for the
// c++ source files, cxx, cpp, cplusplus, so we only need to test lang_cxx
// and lang_internal.
//

bool isLangCpp(LANG_T lang);

typedef enum act_t {
	ACT_any,
	ACT_alter_database,
	ACT_alter_domain,
	ACT_alter_index,
	ACT_alter_table,
	ACT_at_end,
	ACT_average,
	ACT_b_declare,
	ACT_basedon,
	ACT_blob_cancel,
	ACT_blob_close,
	ACT_blob_create,
	ACT_blob_for,
	ACT_blob_handle,
	ACT_blob_open,
	ACT_block_data,
	ACT_close,
	ACT_commit,
	ACT_commit_retain_context,
	ACT_create_database,
	ACT_create_domain,
	ACT_create_generator,
	ACT_create_index,
	ACT_create_shadow,
	ACT_create_table,
	ACT_create_view,
	ACT_cursor,
	ACT_database,
	ACT_declare_filter,
	ACT_declare_udf,
	ACT_delete,
	ACT_disconnect,
	ACT_drop_database,
	ACT_drop_domain,
	ACT_drop_filter,
	ACT_drop_index,
	ACT_drop_shadow,
	ACT_drop_table,
	ACT_drop_udf,
	ACT_drop_view,

	ACT_dyn_close,
	ACT_dyn_cursor,
	ACT_dyn_declare,
	ACT_dyn_describe,
	ACT_dyn_describe_input,
	ACT_dyn_execute,
	ACT_dyn_fetch,
	ACT_dyn_grant,
	ACT_dyn_immediate,
	ACT_dyn_insert,
	ACT_dyn_open,
	ACT_dyn_prepare,
	ACT_dyn_revoke,
	ACT_dyn_statement,

	ACT_endblob,
	ACT_e_declare,
	ACT_enderror,
	ACT_endfor,
	ACT_endmodify,
	ACT_endstore,
	ACT_endstore_special,
	ACT_erase,
	ACT_event_init,
	ACT_event_wait,
	ACT_fetch,
	ACT_finish,
	ACT_function,
	ACT_for,
	ACT_get_segment,
	ACT_hctef,
	ACT_insert,
	ACT_loop,
	ACT_max,
	ACT_min,
	ACT_modify,
	ACT_on_error,
	ACT_open,
	ACT_prepare,
	ACT_procedure,
	ACT_put_segment,
	ACT_ready,
	ACT_release,
	ACT_rfinish,
	ACT_rollback,
	ACT_routine,
	ACT_segment,
	ACT_segment_length,
	ACT_s_end,
	ACT_s_fetch,
	ACT_s_start,
	ACT_select,
	ACT_start,
	ACT_statistics,
	ACT_store,
	ACT_store2,
	ACT_total,
	ACT_update,
	ACT_variable,
#ifdef PYXIS
	ACT_form_for,
	ACT_form_display,
	ACT_form_end,
	ACT_form_field,
	ACT_menu,
	ACT_menu_display,
	ACT_menu_entree,
	ACT_menu_end,
	ACT_menu_for,
	ACT_title_text,
	ACT_title_length,
	ACT_terminator,
	ACT_entree_text,
	ACT_entree_length,
	ACT_entree_value,
	ACT_window_create,
	ACT_window_delete,
	ACT_window_suspend,
	ACT_item_put,
	ACT_item_for,
	ACT_item_end,
#endif
	ACT_clear_handles,
	ACT_type,
	ACT_noop,
	ACT_get_slice,
	ACT_put_slice,
	ACT_sql_dialect,

	ACT_LASTACT					/* leave this debugging ACT last */
} ACT_T;

/* Action block, used to make action points in source */

typedef struct act {
	SLONG act_position;			/* position in input stream */
	SLONG act_length;			/* length to be commented out */
	enum act_t act_type;		/* type of action */
	struct act *act_next;		/* next action in request */
	struct act *act_rest;		/* remaining actions in module */
	struct act *act_error;		/* on-error action (maybe) */
	struct act *act_pair;		/* begin/end action (maybe) */
	struct gpre_req *act_request;	/* parent request */
	struct ref *act_object;		/* dependent on action type */
	struct swe *act_whenever;	/* SQL whenever blocks */
	USHORT act_flags;			/* flags that affect the action */
	USHORT act_count;			/* used to hold begin/end count for routines */
} *ACT;

#define ACT_mark	1			/* action is mark only - no comment */
#define ACT_first	2			/* action is first on line.  No FTN continuation */
#define ACT_break	4			/* module boundary break */
#define ACT_sql		8			/* action is SQL statement */
#define ACT_decl	16			/* action is a PASCAL forward or extern routine */
#define ACT_main	32			/* action is the main routine in the program/module */
#ifdef PYXIS
#define ACT_first_entree 64		/* first entree in menu */
#endif
#define ACT_back_token	128		/* end of action marked by prior token */

#define ACT_LEN sizeof (struct act)


/* Structure used by Fortran and Basic to determine whether or not
   an array has been declared in a subroutine. */

typedef struct adl {
	USHORT adl_gds_ident;		/* Identifier of array for which Gpre has
								   generated a declaration in main or a
								   subroutine */
	struct adl *adl_next;		/* Next declared array identifier */
} *ADL;

#define ADL_LEN sizeof (struct adl)


/* Array information block.  Used to hold info about an array field.
   Note: the dimension (DIM) block used to hold dimension information.
   The preferred mechanism is the repeating tail on the array block. */

typedef struct ary {
	USHORT ary_dtype;			/* data type of array */
	int ary_dimension_count;	/* Number of dimensions in this array */
	struct dim *ary_dimension;	/* Linked list of range info for each dimension */
	SLONG ary_size;				/* Size of the array */
	USHORT ary_ident;			/* Array identifier */
	BOOLEAN ary_declared;		/* True if a declaration already was generated */
	struct ary_repeat {
		SLONG ary_lower;
		SLONG ary_upper;
	} ary_rpt[16];
} *ARY;

#define ARY_LEN(count) (sizeof (struct ary))
#define MAX_ARRAY_DIMENSIONS 16


/* Based on block.  Used for based on clause */

typedef struct bas {
	struct gpre_fld *bas_field;		/* database field referenced */
	struct lls *bas_variables;	/* list of variables based on above */
	struct str *bas_db_name;	/* database name if present and required */
	struct str *bas_rel_name;	/* relation name if no db statement */
	struct str *bas_fld_name;	/* field if no db statement */
	USHORT bas_flags;
	char bas_terminator[2];		/* terminating character */
} *BAS;

#define BAS_LEN		(sizeof (struct bas))

#define BAS_segment	1			/* Based on a blob segment length */
#define BAS_ambiguous	2		/* Ambiguous reference to segment */


/* Blob block.  Used for blob calls */

class blb
{
public:
	struct gpre_req *blb_request;	/* parent request */
	blb*		blb_next;		/* next blob in request */
	struct ref *blb_reference;	/* field reference for blob field */
	struct sym *blb_symbol;		/* Blob context variable */
	USHORT blb_ident;			/* Blob handle */
	USHORT blb_buff_ident;		/* Ident of segment buffer */
	USHORT blb_len_ident;		/* Ident of segment length */
	USHORT blb_seg_length;		/* Segment length of blob */
	USHORT blb_flags;			/* Misc and various blobs */
	USHORT blb_top_label;		/* fortran label for top of request */
	USHORT blb_btm_label;		/* fortran label for request exit */
	USHORT blb_bpb_length;		/* Length of blob parameter block */
	USHORT blb_bpb_ident;		/* Ident for blob parameter block */
	USHORT blb_type;			/* Blob type (0 = default segmented) */
	SSHORT blb_const_from_type;
	/* Constant value for subtype from
	   which this blob is to be filtered */
	TEXT *blb_var_from_type;
	/* Variable whose value is the subtype
	   from which this blob is to be filtered */
	SSHORT blb_const_to_type;
	/* Constant value for subtype to
	   which this blob is to be filtered */
	TEXT *blb_var_to_type;
	/* Variable whose value is the subtype
	   to which this blob is to be filtered */
	USHORT blb_from_charset;	/* charset to translate from */
	USHORT blb_to_charset;		/* charset to translate to */
	UCHAR blb_bpb[24];
};
typedef blb* BLB;

#define BLB_LEN sizeof(blb)

#define BLB_create		1
#define BLB_symbol_released	2


/* Constraint block, used to hold information about integrity constraints */

typedef struct cnstrt {
	struct str *cnstrt_name;	/* Name of constraint */
	USHORT cnstrt_type;			/* Type of constraint */
	struct lls *cnstrt_fields;	/* list of fields */
	USHORT cnstrt_fkey_def_type;	/* extended foreign key definition */
	struct str *cnstrt_referred_rel;	/* referred relation, if foreign key */
	struct lls *cnstrt_referred_fields;	/* optional list of fields from
										   referred relation */
	struct cnstrt *cnstrt_next;	/* next contraint for field or relation */
	struct txt *cnstrt_text;	/* source for CHECK constraints */
	struct gpre_nod *cnstrt_boolean;	/* boolean expression, for CHECK
								   constraints */
	USHORT cnstrt_flags;		/* see below */
} *CNSTRT;

#define CNSTRT_LEN sizeof (struct cnstrt)

/* Values for cnstrt_fkey_def_type */

#define REF_UPDATE_ACTION	0x0001
#define REF_UPD_CASCADE		0x0002
#define REF_UPD_SET_DEFAULT	0x0004
#define REF_UPD_SET_NULL	0x0008
#define REF_UPD_NONE		0x0010
#define REF_UPDATE_MASK		0x001E

#define REF_DELETE_ACTION	0x0020
#define REF_DEL_CASCADE		0x0040
#define REF_DEL_SET_DEFAULT	0x0080
#define REF_DEL_SET_NULL	0x0100
#define REF_DEL_NONE		0x0200
#define REF_DELETE_MASK		0x03C0

/* Values for cnstrt_type */

#define CNSTRT_NOT_NULL		1
#define CNSTRT_PRIMARY_KEY	2
#define CNSTRT_UNIQUE		3
#define CNSTRT_FOREIGN_KEY	4
#define CNSTRT_CHECK		5

/* Values for cnstrt_flags */

#define CNSTRT_DEFERRABLE		1
#define CNSTRT_INITIALLY_DEFERRED	2
#define CNSTRT_delete			4


/* Context block, used to define context symbols, etc. */

typedef struct gpre_ctx {
	USHORT ctx_internal;		/* internal context number */
	USHORT ctx_scope_level;		/* scope level for SQL alias subquery scoping */
	struct gpre_req *ctx_request;	/* parent request */
	struct gpre_ctx *ctx_next;		/* next context in request */
	struct sym *ctx_symbol;		/* symbol for context */
	struct gpre_rel *ctx_relation;	/* relation for context */
	TEXT *ctx_alias;			/* holds SQL alias for passing to engine */
	struct gpre_prc *ctx_procedure;	/* procedure for context */
	struct gpre_nod *ctx_prc_inputs;	/* procedure input parameters */
	struct rse *ctx_stream;		/* stream for context */
} *GPRE_CTX;

#define CTX_LEN sizeof (struct gpre_ctx)


typedef struct sdt {
	USHORT sdt_dialect;			/* Dialect  value as specified by SET stmt */
} *SDT;

#define SDT_LEN sizeof (struct sdt)


typedef struct fil {
	SLONG fil_length;			/* File length in pages */
	SLONG fil_start;			/* Starting page */
	TEXT *fil_name;				/* File name */
	struct fil *fil_next;		/* next file */
	USHORT fil_shadow_number;	/* shadow number if part of shadow */
	USHORT fil_partitions;		/* number of log file partitions */
	USHORT fil_flags;
} *FIL;

#define FIL_manual		1		/* Manual shadow */
#define FIL_raw			2		/* On raw device */
#define FIL_conditional		4	/* Conditional shadow */

#define FIL_LEN sizeof (struct fil)


/* Database block, more or less the granddaddy */

typedef struct dbb {
	struct dbb *dbb_next;		/* next database in program */
	struct gpre_rel *dbb_relations;	/* relations in database */
	struct gpre_rel *dbb_procedures;	/* procedures in database */
#ifdef PYXIS
	struct form *dbb_forms;		/* forms in database */
#endif
	USHORT dbb_id;				/* database id in program */
	USHORT dbb_flags;			/* Misc flag bytes */
	struct sym *dbb_name;		/* database name */
	FRBRD *dbb_handle;			/* OUR db handle */
	FRBRD *dbb_transaction;		/* default transaction */
	struct rrl *dbb_rrls;		/* temporary list of relation locks */
	struct tpb *dbb_tpbs;		/* real tpbs for this db */
	TEXT *dbb_filename;
	TEXT *dbb_runtime;
	TEXT *dbb_c_user;			/* compiletime user name */
	TEXT *dbb_c_password;		/* compiletime password */
	TEXT *dbb_c_sql_role;		/* compiletime SQL role */
	TEXT *dbb_r_user;			/* runtime user name */
	TEXT *dbb_r_password;		/* runtime password */
	TEXT *dbb_r_sql_role;		/* runtime SQL role */
	TEXT *dbb_c_lc_messages;	/* compiletime user natural language */
	TEXT *dbb_c_lc_ctype;		/* compiletime user character set */
	TEXT *dbb_r_lc_messages;	/* runtime user natural language */
	TEXT *dbb_r_lc_ctype;		/* runtime user character set */
	TEXT *dbb_def_charset;		/* charset for CREATE DATABASE */
	SSHORT dbb_know_subtype;	/* Use an charset subtype id on messages */
	SSHORT dbb_char_subtype;	/* subtype to use for all SCHAR messages */
	FRBRD *dbb_field_request;
	FRBRD *dbb_flds_request;
	FRBRD *dbb_relation_request;
	FRBRD *dbb_procedure_request;
	FRBRD *dbb_udf_request;
	FRBRD *dbb_trigger_request;
	FRBRD *dbb_proc_prms_request;
	FRBRD *dbb_proc_prm_fld_request;
	FRBRD *dbb_index_request;
	FRBRD *dbb_type_request;
	FRBRD *dbb_array_request;
	FRBRD *dbb_dimension_request;
	FRBRD *dbb_domain_request;
	FRBRD *dbb_generator_request;
	FRBRD *dbb_view_request;
	FRBRD *dbb_primary_key_request;
	int dbb_scope;				/* scope of the database handle */
	int dbb_allocation;
	int dbb_pagesize;
	int dbb_buffercount;
	int dbb_buffersize;
	int dbb_users;
	ULONG dbb_length;			/* Length of database in pages, if known */
	struct fil *dbb_logfiles;
	struct fil *dbb_overflow;	/* overflow log files */
	SLONG dbb_chkptlen;
	SSHORT dbb_numbufs;			/* log buffers */
	SSHORT dbb_bufsize;			/* log buffer size */
#ifdef SCROLLABLE_CURSORS
	SSHORT dbb_base_level;		/* code level of the engine we are talking to */
#endif
	SLONG dbb_grp_cmt_wait;
	struct fil *dbb_cache_file;
	struct fil *dbb_files;
} *DBB;

#define DBB_LEN 	sizeof (struct dbb)

#define DBB_no_arrays		1
#define DBB_sqlca		2		/* Created as default for a sqlca */
#define DBB_in_trans		4	/* included in this transaction */
#define DBB_drop_log		8
#define DBB_log_serial		16
#define DBB_log_default		32
#define DBB_cascade		64
#define DBB_drop_cache		128
#define DBB_create_database	256
#define DBB_v3			512		/* Database is V3 */

#define DBB_GLOBAL 	0
#define DBB_EXTERN 	1
#define DBB_STATIC 	2

/* database block data, for generating block data section in fussy fortran */

typedef struct dbd {
	TEXT dbb_name[128];			/* database name */
} *DBD;


/* Computed field block */

typedef struct cmpf {
	struct txt *cmpf_text;		/* source for computed field */
	struct gpre_nod *cmpf_boolean;	/* expression, for computed field */
} *CMPF;

#define CMPF_LEN sizeof (struct cmpf)


/* Dimension block, used for arrays
 *
 * Note: this structure is being phased out.  Please use the
 * repeating tail on the ARY structure instead
 *
 */

typedef struct dim {
	int dim_number;				/* Dimension number i (i=1 to n) */
	SLONG dim_lower;			/* Lower bound */
	SLONG dim_upper;			/* Upper bound */
	struct dim *dim_next;		/* Information for dimension i+1 */
	struct dim *dim_previous;	/* Information for dimension i-1 */
} *DIM;

#define DIM_LEN sizeof (struct dim)


/* Dynamic statement block, used for dynamic SQL */

typedef struct dyn {
	struct dbb *dyn_database;	/* Database involved */
	struct sym *dyn_statement_name;	/* Name of dynamic statement */
	struct sym *dyn_cursor_name;	/* Cursor name */
	TEXT *dyn_trans;			/* Transaction handle */
	TEXT *dyn_string;			/* Dynamic string or variable name */
	TEXT *dyn_sqlda;			/* Name of SQLDA structure, if any */
	TEXT *dyn_sqlda2;			/* Name of second SQLDA structure, if any */
	struct gpre_nod *dyn_using;		/* dependent on action type */
} *DYN;

#define DYN_LEN sizeof (struct dyn)


/* Field block.  Fields are what farms and databases are all about */

typedef struct gpre_fld {
	USHORT fld_dtype;			/* data type of field */
	FLD_LENGTH fld_length;		/* field length in bytes */
	SSHORT fld_scale;			/* scale factor */
	USHORT fld_id;				/* field id in meta data */
	USHORT fld_flags;			/* Misc flags */
	USHORT fld_seg_length;		/* Segment length for blobs */
	USHORT fld_position;		/* Field position in relation */
	USHORT fld_precision;		/* Field precision */
	SSHORT fld_sub_type;		/* Field sub-type */
	struct gpre_fld *fld_next;		/* next field in relation */
	struct gpre_fld *fld_array;		/* array element if field is array */
	struct gpre_rel *fld_relation;	/* relation */
	struct gpre_prc *fld_procedure;	/* procedure */
	struct sym *fld_symbol;		/* symbol for field */
	struct sym *fld_global;		/* symbol for global field */
#ifdef PYXIS
	int *fld_handle;			/* form handle */
	int *fld_prototype;			/* prototype object for sub-form */
#endif
	struct ary *fld_array_info;	/* Dimension and range information about an
								   array field */
	struct gpre_nod *fld_default_value;	/* field's default value */
	struct txt *fld_default_source;	/* source for field default value */
	struct ind *fld_index;		/* If CREATE TABLE, specifies field with
								   the unique constraint */
	struct cnstrt *fld_constraints;	/* linked list of constraints defined
									   during a meta operation */
	struct intlsym *fld_character_set;
	/* character set for SQL declared field */
	struct intlsym *fld_collate;	/* collation clause for SQL declared field */
	struct cmpf *fld_computed;	/* computed field definition */
	USHORT fld_char_length;		/* field length in CHARACTERS */
	SSHORT fld_charset_id;		/* Field character set id for text */
	SSHORT fld_collate_id;		/* Field collation id for text */
	SSHORT fld_ttype;			/* ID of text type's implementation */
} *GPRE_FLD;

#define FLD_LEN		sizeof (struct gpre_fld)

#define FLD_blob	 1
#define FLD_text	 2
#define FLD_stream_blob	 4
#define FLD_dbkey	 8
#ifdef PYXIS
#define FLD_repeating	 16		/* Field is form repeating group */
#endif
#define FLD_not_null	 32		/* if CREATE TABLE specifies not null */
#define FLD_delete	 64			/* if ALTER TABLE specifies delete of field */
#define FLD_meta	 128		/* if true, created for a metadata operation */
#define FLD_national	 256	/* uses SQL "NATIONAL" character set */
#define FLD_charset      512	/* field has a specific character set */
#define FLD_computed   	 1024	/* field is computed */
#define FLD_meta_cstring 2048	/* field is being defined as cstring by the user. */


/* filter block */

typedef struct fltr {
	TEXT *fltr_name;
	TEXT *fltr_entry_point;
	TEXT *fltr_module_name;
	SSHORT fltr_input_type;
	SSHORT fltr_output_type;
} *FLTR;

#define FLTR_LEN sizeof (struct fltr)


/* declare udf block */

typedef struct decl_udf {
	TEXT *decl_udf_name;
	TEXT *decl_udf_entry_point;
	TEXT *decl_udf_module_name;
	struct gpre_fld *decl_udf_arg_list;
	struct gpre_fld *decl_udf_return_type;
	USHORT decl_udf_return_mode;	/* BY VALUE or BY REFERENCE */
	SSHORT decl_udf_return_parameter;
} *DECL_UDF;


#define DECL_UDF_LEN sizeof (struct decl_udf)

typedef ENUM {
		FUN_value,
		FUN_reference,
		FUN_descriptor, FUN_blob_struct, FUN_scalar_array} FUN_T;


/* Index block. Used for DDL INDEX commands */

typedef struct ind {
	struct sym *ind_symbol;		/* index name */
	struct gpre_rel *ind_relation;	/* relation name */
	struct gpre_fld *ind_fields;		/* list of fields */
	USHORT ind_flags;			/* Miscellaneous flags */
} *IND;

#define IND_LEN		sizeof (struct ind)
#define IND_dup_flag	1		/* if false, duplicates not allowed */
#define IND_meta	2			/* if true, created for a metadata operation */
#define IND_descend	4			/* if true, a descending-order index */
#define IND_active	8			/* activate index */
#define IND_inactive	16		/* de-activate index */



/* Symbolic names for international text types */
/* (either collation or character set name)    */

typedef struct intlsym {		/* International symbol */
	struct dbb *intlsym_database;
	struct sym *intlsym_symbol;	/* Hash symbol for intlsym */
	struct intlsym *intlsym_next;
	USHORT intlsym_type;		/* what type of name */
	USHORT intlsym_flags;
	SSHORT intlsym_ttype;		/* id of implementation */
	SSHORT intlsym_charset_id;
	SSHORT intlsym_collate_id;
	USHORT intlsym_bytes_per_char;
	TEXT intlsym_name[2];
} *INTLSYM;

#define INTLSYM_LEN		sizeof (struct intlsym)

/* values used in intlsym_type */

#define INTLSYM_collation	1
#define	INTLSYM_charset		2

/* values used in intlsym_flags */

/* Macro for detecting fields which may have embedded nulls */

#define SUBTYPE_ALLOWS_NULLS(s)	((s) == 1)
#define FIELD_ALLOWS_NULLS(f)	(SUBTYPE_ALLOWS_NULLS((f)->fld_sub_type))


/* Linked list stack stuff */

typedef struct lls {
	struct gpre_nod *lls_object;		/* object on stack */
	struct lls *lls_next;		/* next item on stack */
} *LLS;

#define LLS_LEN sizeof (struct lls)


/* Maps used by union and global aggregates */

typedef struct map {
	struct gpre_ctx *map_context;	/* Pseudo context for map */
	struct mel *map_elements;	/* Map elements */
	USHORT map_count;			/* Number of things in map */
} *MAP;

typedef struct mel {
	struct mel *mel_next;		/* Next element in map */
	struct gpre_nod *mel_expr;		/* Expression */
	struct ref *mel_reference;
	struct gpre_ctx *mel_context;
	USHORT mel_position;		/* Position in map */
} *MEL;


/* General Syntax node, produced by parser */

typedef enum nod_t {
	nod_field = 1, nod_literal, nod_value,
	nod_and, nod_or, nod_not,
	nod_eq, nod_ne, nod_ge,
	nod_le, nod_gt, nod_lt,
	nod_containing, nod_matches, nod_any,
	nod_unique, nod_plus, nod_times,
	nod_divide, nod_minus, nod_negate,
	nod_msg, nod_for, nod_send,
	nod_receive, nod_block, nod_select,
	nod_boolean, nod_projection, nod_sort,
	nod_store, nod_modify, nod_erase,
	nod_if, nod_assignment, nod_rse,
	nod_first, nod_relation, nod_end,
	nod_label, nod_leave, nod_loop,
	nod_max, nod_min, nod_count,
	nod_total, nod_average, nod_list,
	nod_defered, nod_missing, nod_between,
	nod_union, nod_map, nod_starting,
	nod_like, nod_agg_count, nod_agg_max,
	nod_agg_min, nod_agg_total, nod_agg_average,
	nod_aggregate, nod_from, nod_null,
	nod_asterisk, nod_map_ref, nod_user_name,
	nod_upcase, nod_sleuth, nod_event_init,
	nod_udf, nod_array, nod_index,
	nod_via, nod_join_inner, nod_join_left,
	nod_join_right, nod_join_full, nod_join,
	nod_concatenate, nod_cast, nod_dom_value,
	nod_ansi_any, nod_gen_id, nod_set_generator,
	nod_merge, nod_plan_expr, nod_plan_item,
	nod_natural, nod_index_order, nod_ansi_all,
	nod_extract, nod_current_date,
	nod_current_time, nod_current_timestamp,
	nod_LASTNOD					/* Leave this debugging GPRE_NOD last */
} NOD_T;

typedef struct gpre_nod {
	enum nod_t nod_type;		/* node type */
	USHORT nod_count;			/* number of sub-items */
	struct gpre_nod *nod_arg[1];		/* argument */
} *GPRE_NOD;

#define nod_arg0 nod_arg[0]
#define nod_arg1 nod_arg[1]
#define nod_arg2 nod_arg[2]

#define NOD_LEN(cnt) (sizeof (struct gpre_nod) + (cnt - 1) * sizeof (int *))


/* Open cursor block */

typedef struct opn {
	struct sym *opn_cursor;		/* Symbol block of cursor */
	TEXT *opn_trans;			/* Transaction handle */
	struct ref *opn_using;		/* Using variables */
} *OPN;

#define OPN_LEN (sizeof (struct opn))


/* Port block */

typedef struct por {
	USHORT por_msg_number;		/* message number within request */
	USHORT por_ident;			/* ident in source */
	int por_length;				/* length of port in bytes */
	struct ref *por_references;	/* linked list of field references */
	struct por *por_next;		/* next port in request */
	USHORT por_count;			/* number of items in port */
} *POR;

#define POR_LEN (sizeof (struct por))


/* Procedure structure */

typedef struct gpre_prc {
	struct sym *prc_symbol;		/* symbol for relation */
	SSHORT prc_id;				/* procedure id */
	struct sym *prc_owner;		/* owner of procedure, if any */
	struct dbb *prc_database;	/* parent database */
	struct gpre_prc *prc_next;		/* next procedure in database */
	struct gpre_fld *prc_inputs;		/* linked list of input parameters */
	struct gpre_fld *prc_outputs;	/* linked list of output parameters */
	SSHORT prc_in_count;		/* count of input parameters */
	SSHORT prc_out_count;		/* count of output parameters */
	SSHORT prc_flags;			/* procedure flags */
} *GPRE_PRC;

#define PRC_LEN sizeof (struct gpre_prc)
#define PRC_scanned	1


/* Grant/revoke block */

typedef struct prv {
	USHORT prv_privileges;		/* holds privileges being granted or revoked */
	SCHAR *prv_username;		/* user having privileges granted or revoked */
	USHORT prv_user_dyn;		/* the dyn-verb to be used with prv_username
								   i.e. gds__dyn_grant_user/proc/trig/view */
	struct str *prv_relation;	/* relation on which we're doing grant/revoke */
	USHORT prv_object_dyn;		/* the dyn-verb to be used with prv_relation
								   i.e. gds__dyn_rel/proc_name */
	struct lls *prv_fields;		/* fields on which we're doing grant/revoke */
	struct prv *prv_next;		/* next grant/revoke block (with different user) */
} *PRV;

#define PRV_LEN (sizeof (struct prv))

#define PRV_no_privs	0		/* no privileges being granted or revoked */
#define PRV_select	1			/* select privilege being granted or revoked */
#define PRV_insert	2			/* insert privilege being granted or revoked */
#define PRV_delete	4			/* delete privilege being granted or revoked */
#define PRV_update	8			/* update privilege being granted or revoked */
#define PRV_execute	16			/* execute privilege being granted or revoked */
#define PRV_references	32		/* reference privilege */
#define PRV_grant_option 64		/* privilege to grant privileges being granted */
#define PRV_all		128			/* all privileges being granted/revoked */


/* Ready block */

typedef struct rdy {
	struct gpre_req *rdy_request;	/* dpb message & info */
	struct rdy *rdy_next;
	struct dbb *rdy_database;
	USHORT rdy_id;				/* id for unique string variable- MPEXL COB */
	TEXT *rdy_filename;
} *RDY;

#define RDY_LEN sizeof (struct rdy)


/* Field reference */

typedef struct ref {
	USHORT ref_ident;			/* identifier */
	USHORT ref_level;			/* highest level of access */
	USHORT ref_parameter;		/* parameter in port */
	USHORT ref_id;				/* id of reference in union */
	struct gpre_fld *ref_field;		/* field in question */
	struct gpre_ctx *ref_context;	/* context for reference */
	struct ref *ref_next;		/* next reference in context */
	struct por *ref_port;		/* associated port */
	struct ref *ref_source;		/* source reference for modified field */
	struct ref *ref_null;		/* reference for null value */
	struct ref *ref_master;		/* master field for null value */
	struct ref *ref_friend;		/* value for variable */
	struct gpre_nod *ref_expr;		/* expression, if node is expression */
	TEXT *ref_value;			/* value string if host language value */
	struct val *ref_values;		/* linked list of values */
	TEXT *ref_null_value;		/* value string if host language value */
	TEXT *ref_sdl;				/* Raw slice description language for an array */
	TEXT *ref_sdl_base;			/* base of sdl string during generation */
	int ref_sdl_length;			/* sdl length for this reference */
	struct slc *ref_slice;		/* Slice, if field referenced is sliced */
	USHORT ref_sdl_ident;		/* identifier of sdl structure */
	USHORT ref_offset;			/* offset of field in port */
	USHORT ref_flags;
	SSHORT ref_ttype;			/* Character set type for literals */
} *REF;

#define REF_union	1			/* Pseudo field for union */
#define REF_pseudo	2			/* Other pseudo field (probably for forms) */
#define REF_null	4			/* Only here cause of related null reference */
#define REF_fetch_array	8		/* Need to fetch full array */
#define REF_literal	16			/* Reference is to a constant */
#define REF_ttype	32			/* Reference contains character set spec */
#define REF_array_elem	64		/* Reference to an array element */
#define REF_sql_date	128		/* Reference is to a date constant */
#define REF_sql_time	256		/* Reference is to a time constant */
#define REF_timestamp  	512		/* Reference is to a timestamp constant */

#define REF_LEN sizeof (struct ref)


/* value block, used to store a set of values */

typedef struct val {
	struct val *val_next;		/* next value in list */
	TEXT *val_value;			/* value */
} *VAL;

#define VAL_LEN sizeof (struct val)


/* Relation block, not to be confused with siblings or in-laws */

typedef struct gpre_rel {
	USHORT rel_id;				/* relation id */
	struct gpre_fld *rel_fields;		/* linked list of known fields */
	struct gpre_fld *rel_dbkey;		/* linked list of known fields */
	struct sym *rel_symbol;		/* symbol for relation */
	struct dbb *rel_database;	/* parent database */
	struct gpre_rel *rel_next;		/* next relation in database */
	BOOLEAN rel_meta;			/* if true, created for a metadata operation */
	struct rse *rel_view_rse;
	struct txt *rel_view_text;	/* source for VIEW definition */
	struct sym *rel_owner;		/* owner of relation, if any */
	struct cnstrt *rel_constraints;	/* linked list of constraints defined
									   during a meta operation */
	TEXT *rel_ext_file;			/* external file name */
	USHORT rel_flags;
} *GPRE_REL;

#define REL_LEN sizeof (struct gpre_rel)

#define REL_view_check	1		/* View created with check option */


/* Request block, corresponds to a single JRD request */

enum req_t {
	REQ_for,
	REQ_store,
	REQ_store2,
	REQ_insert,
	REQ_cursor,
	REQ_select,
	REQ_mass_update,
	REQ_any,
	REQ_statistical,
	REQ_ddl,
#ifdef PYXIS
	REQ_form,
#endif
	REQ_create_database,
#ifdef PYXIS
	REQ_menu,
#endif
	REQ_slice,
	REQ_ready,
	REQ_procedure,
	REQ_set_generator,

	REQ_LASTREQUEST				/* Leave this debugging GPRE_REQ last */
};

typedef struct gpre_req {
	enum req_t req_type;		/* request type */
	USHORT req_ident;			/* ident for request handle */
	USHORT req_act_flag;		/* activity flag ident, if used */
	int req_length;				/* blr length of request */
	UCHAR *req_base;			/* base of blr string during generation */
	UCHAR *req_blr;				/* raw blr string */
	SCHAR *req_handle;			/* request handle */
	TEXT *req_trans;			/* transaction handle */
	SCHAR *req_request_level;	/* request level expression */
#ifdef PYXIS
	SCHAR *req_form_handle;		/* optional handle for forms */
#endif
	USHORT req_level;			/* access level */
	USHORT req_count;			/* number of ports in request */
	USHORT req_internal;		/* next internal context number */
	USHORT req_labels;			/* next available label */
	USHORT req_top_label;		/* fortran label for top of request */
	USHORT req_btm_label;		/* fortran label for request exit */
	struct gpre_nod *req_node;		/* request definition tree */
	struct dbb *req_database;	/* database */
	struct act *req_actions;	/* actions within request */
	struct gpre_ctx *req_contexts;	/* contexts within request */
	struct gpre_ctx *req_update;		/* update context for mass insert */
	struct gpre_req *req_next;		/* next request in module or metadata action */
	struct ref *req_values;		/* host values required */
#ifdef SCROLLABLE_CURSORS
	struct ref *req_avalues;	/* parameters to pass to asynchronous message */
#endif
	struct ref *req_eof;		/* eof reference for FOR */
	struct ref *req_index;		/* index variable */
	struct ref *req_references;	/* fields referenced in context */
#ifdef PYXIS
	struct ref *req_term_field;	/* terminating field for forms */
#endif
	struct map *req_map;		/* map for aggregates, etc */
	struct rse *req_rse;		/* record selection expression */
	struct por *req_ports;		/* linked list of ports */
	struct por *req_primary;	/* primary input or output port */
	struct por *req_sync;		/* synchronization port */
	struct por *req_vport;		/* port to send values in */
#ifdef SCROLLABLE_CURSORS
	struct por *req_aport;		/* port for asynchronous message */
#endif
	struct gpre_req *req_routine;	/* other requests in routine */
	blb*		req_blobs;		/* blobs in request */
#ifdef PYXIS
	struct form *req_form;		/* form for request */
#endif
	struct slc *req_slice;		/* slice for request */
	struct ref *req_array_references;	/* array fields referenced in context */
	USHORT req_scope_level;		/* scope level for SQL subquery parsing */
	USHORT req_in_aggregate;	/* now processing value expr for aggr */
	USHORT req_in_select_list;	/* processing select list */
	USHORT req_in_where_clause;	/* processing where clause */
	USHORT req_in_having_clause;	/* processing having clause */
	USHORT req_in_order_by_clause;	/* processing order by clause */
	USHORT req_in_subselect;	/* processing a subselect clause */
	ULONG req_flags;
} *GPRE_REQ;

#define REQ_exp_hand		1
#define REQ_local		2		/* defined in an included routine */
#ifdef PYXIS
#define REQ_menu_tag		4	/* tag line menu */
#define REQ_menu_pop_up		8	/* pop-up style menu */
#define REQ_exp_form_handle	16	/* Explicit handle for form */
#define REQ_transparent		32	/* Form is transparent */
#define REQ_form_tag		64	/* Put form on tag line */
#define REQ_form_nowait		128
#define REQ_menu_for		256	/* dynamic menu */
#define REQ_menu_for_item	512	/* dynamic menu for item */
#endif
#define REQ_sql_cursor		1024	/* request is an SQL cursor */
#define REQ_extend_dpb		2048	/* we need to extend dpb at runtime */
#define REQ_sql_declare_cursor	4096	/* request is declare cursor */
#define REQ_sql_blob_open	8192	/* request is SQL open blob cursor */
#define REQ_sql_blob_create	16384	/* request is SQL create blob cursor */
#define REQ_sql_database_dyn	32768	/* request is to generate DYN to add files o database */
#ifdef SCROLLABLE_CURSORS
#define REQ_scroll		65536	/* request is a scrollable cursor */
#define REQ_backwards	       131072	/* request was last scrolled backwards */
#endif
#define REQ_blr_version4       262144	/* request must generate blr_version4 */

#define REQ_LEN sizeof (struct gpre_req)


/* act_object block for SQL database commands. */

typedef struct mdbb {
	struct dbb *mdbb_database;
	struct gpre_req *mdbb_dpb_request;
	struct gpre_req *mdbb_dpb_extend_request;
} *MDBB;



/* Reserved relation lock block */

typedef struct rrl {
	struct rrl *rrl_next;		/* next locked relation */
	UCHAR rrl_lock_level;		/* lock level (SHARE, PROT, EXC */
	UCHAR rrl_lock_mode;		/* lock mode (READ/WRITE) */
	struct gpre_rel *rrl_relation;	/* relation block */
} *RRL;

#define RRL_LEN sizeof (struct rrl)


/* Record selection expresion syntax node */

typedef struct rse {
	USHORT rse_type;			/* node type */
	struct gpre_nod *rse_boolean;	/* boolean expression, if present */
	struct gpre_nod *rse_first;		/* "first n" clause, if present */
	struct gpre_nod *rse_reduced;	/* projection clause, if present */
	struct gpre_nod *rse_sort;		/* sort clause, if present */
	struct gpre_nod *rse_fields;		/* list of fields */
	struct gpre_nod *rse_into;		/* list of output variables */
	struct gpre_nod *rse_union;		/* if union, list of sub-rses */
	struct gpre_nod *rse_group_by;	/* list of grouping fields */
	struct gpre_nod *rse_plan;		/* user-specified access plan */
	struct map *rse_map;		/* map for aggregates */
	struct rse *rse_aggregate;	/* Aggregate rse */
	enum nod_t rse_join_type;	/* Join type */
	USHORT rse_flags;			/* flags */
	USHORT rse_count;			/* number of relations */
	struct gpre_ctx *rse_context[1];	/* context block */
} *RSE;


#ifdef __cplusplus

inline size_t RSE_LEN(size_t nItems)
{
	return offsetof(rse, rse_context) + nItems * sizeof(int *);
}

#else /* __cplusplus */

#define RSE_LEN(nItems) (offsetof(struct rse, rse_context) + (nItems) * sizeof(int*))

#endif /* __cplusplus */

//#define RSE_LEN(cnt) (sizeof (struct rse) + (cnt - 1) * sizeof (int *))

#define RSE_singleton 1


/* Set generator block */

typedef struct sgen {
	TEXT *sgen_name;
	USHORT sgen_dialect;
	SLONG sgen_value;
	SINT64 sgen_int64value;
} *SGEN;

#define SGEN_LEN sizeof (struct sgen)


/* Slice description block */

typedef struct slc {
	GPRE_REQ slc_parent_request;		/* request for blob id */
	GPRE_FLD slc_field;				/* database array field */
	GPRE_NOD slc_array;				/* user defined array */
	REF slc_field_ref;			/* array field reference */
	USHORT slc_dimensions;		/* dimensions */
	USHORT slc_parameters;		/* number of parameters */
	struct slc_repeat {
		GPRE_NOD slc_lower;
		GPRE_NOD slc_upper;
	} slc_rpt[1];
} *SLC;

#define SLC_LEN(count)	(sizeof (struct slc) + sizeof (slc::slc_repeat) * (count - 1))

/* SQL WHENEVER BLOCK */

typedef struct swe {
	struct swe *swe_next;		/* Next in chain */
	USHORT swe_condition;		/* Condition */
	USHORT swe_length;			/* Length of label */
	TEXT swe_label[1];			/* Label */
} *SWE;

#define SWE_error	0
#define SWE_warning	1
#define SWE_not_found	2
#define SWE_max		3



/* statistic block. Used for all statistics commands */

typedef struct sts {
	struct str *sts_name;		/* object name */
	USHORT sts_flags;			/* Miscellaneous flags */
} *STS;

#define STS_LEN		sizeof (struct sts)
#define STS_index	1			/* Object is an index */



/* STRing block - holds a null terminated string */

typedef struct str {
	TEXT str_string[1];			/* pretty simple, no? */
} *STR;

#define STR_LEN(size)	(sizeof (struct str) + size)


/* Symbol block, also used for hash table */

enum sym_t {
	SYM_keyword,
	SYM_context,
	SYM_database,
	SYM_relation,
	SYM_field,
	SYM_variable,
	SYM_stream,
	SYM_cursor,
	SYM_delimited_cursor,
	SYM_index,
	SYM_blob,
	SYM_statement,
	SYM_dyn_cursor,
#ifdef PYXIS
	SYM_form,
	SYM_form_map,
	SYM_form_field,
#endif
	SYM_type,
	SYM_udf,
#ifdef PYXIS
	SYM_menu,
	SYM_menu_map,
#endif
	SYM_username,
	SYM_procedure,
	SYM_charset,
	SYM_collate,
	SYM_generator,
	SYM_dummy,

	SYM_LASTSYM					/* Leave this debugging SYM last */
};

typedef struct sym {
	char *sym_string;			/* address of asciz string */
	enum sym_t sym_type;		/* symbol type */
	USHORT sym_keyword;			/* keyword number, if keyword */
	struct gpre_ctx *sym_object;		/* general pointer to object */
	struct sym *sym_collision;	/* collision pointer */
	struct sym *sym_homonym;	/* homonym pointer */
	SCHAR sym_name[1];			/* space for name, if necessary */
} *SYM;

#define SYM_LEN sizeof (struct sym)


/* Start transaction block */

typedef struct gpre_tra {
	struct gpre_tra *tra_next;		/* next transaction */
	TEXT *tra_handle;			/* this handle */
	USHORT tra_flags;			/* transaction options */
	struct tpb *tra_tpb;		/* TPB's for this transaction */
	int tra_db_count;			/* number of db's and TPB's */
} *GPRE_TRA;

#define TRA_LEN	sizeof (struct gpre_tra)

/* values for tra_flags */

#define TRA_ro			1
#define TRA_nw			2
#define TRA_con			4
#define TRA_rrl 		8
#define TRA_inc 		16
#define TRA_read_committed	32
#define TRA_autocommit		64
#define TRA_rec_version		128
#define TRA_no_auto_undo	256

#define MAX_TRA_OPTIONS		8


/* TPB block */

typedef struct tpb {
	struct tpb *tpb_tra_next;	/* next TPB for this transaction */
	struct tpb *tpb_dbb_next;	/* next TPB for this database */
	struct dbb *tpb_database;	/* DBB of this part of the transaction */
	USHORT tpb_length;			/* length of actual TPB */
	USHORT tpb_ident;			/* unique part of name for this TPB */
	UCHAR tpb_string[1];		/* actual TPB */
} *TPB;

#define TPB_LEN(tpb_string) (sizeof (struct tpb) + tpb_string)


/* Enumerated field type block */

typedef struct typ {
	SYM typ_symbol;				/* Actual symbol */
	GPRE_FLD typ_field;				/* Owner */
	SSHORT typ_value;			/* Value of type */
} *TYP;

#define TYP_LEN sizeof (struct typ)



/* Trigger block */
/* Note: This structure will need expansion later.  Currently its
   use is to create a trigger for CHECK constraints which always
   abort on error                                                   */

typedef struct gpre_trg {
	struct str *trg_name;
	USHORT trg_type;			/* Type of trigger */
	struct str *trg_source;		/* source for trigger */
	struct gpre_nod *trg_boolean;	/* boolean expression, for trigger */
	struct str *trg_message;	/* Message the trigger prints */
} *GPRE_TRG;

#define TRG_LEN sizeof (struct gpre_trg)

#define PRE_STORE_TRIGGER	1
#define POST_STORE_TRIGGER	2
#define PRE_MODIFY_TRIGGER	3
#define POST_MODIFY_TRIGGER	4
#define PRE_ERASE_TRIGGER	5
#define POST_ERASE_TRIGGER	6


/* Text block */

typedef struct txt {
	ULONG txt_position;
	USHORT txt_length;
} *TXT;

#define TXT_LEN sizeof (struct txt)



/* User Defined Function */

typedef struct udf {
	struct dbb *udf_database;
	SYM udf_symbol;				/* Function name or query name */
	USHORT udf_args;			/* Number of arguments */
	USHORT udf_flags;			/* UDF flags */
	USHORT udf_dtype;			/* Return data type */
	SSHORT udf_scale;			/* Return scale */
	USHORT udf_length;			/* Return length */
	USHORT udf_sub_type;		/* Return sub-type */
	USHORT udf_charset_id;		/* Return character set */
	USHORT udf_ttype;			/* Return text type */
	USHORT udf_type;			/* Function type */
	struct gpre_fld *udf_inputs;		/* List of udf input arguments */
	TEXT udf_function[1];		/* Function name */
} *UDF;

#define UDF_LEN (sizeof (struct udf))
#define	UDF_value	0
#define UDF_boolean	1

#define UDF_scanned	1


/* Update block -- used for (at least) MODIFY */

typedef struct upd {
	USHORT upd_level;			/* reference level */
	struct gpre_req *upd_request;	/* parent request */
	struct ref *upd_references;	/* references under modify */
	struct gpre_ctx *upd_source;		/* context being modified */
	struct gpre_ctx *upd_update;		/* update context */
	struct por *upd_port;		/* port for update */
	struct upd *upd_outer;		/* outer modify, if any */
	struct gpre_nod *upd_assignments;	/* assignments to port */
	struct ref *upd_array_references;	/* array references under modify */
} *UPD;

#define UPD_LEN sizeof (struct upd)


/* User name -- used for SQL GRANT/REVOKE */

typedef struct usn {
	struct usn *usn_next;
	SCHAR *usn_name;
	USHORT usn_dyn;				/* describes the type of user via a dyn-verb,
								   i.e. gds__dyn_grant_user/view/proc/trig */
} *USN;

#define USN_LEN sizeof (struct usn)

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "../jrd/dsc.h"
#ifdef __cplusplus
extern "C" {
#endif


#define dtype_float	dtype_real


/* GPRE wide globals */

#ifdef GPRE_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

/* Added sw_verbose
   FSG 14.Nov.2000 */

EXTERN USHORT sw_verbose, sw_auto, sw_sql, sw_gen_sql, sw_raw, sw_cstring,
	sw_dsql, sw_dyn_using, sw_ada, sw_standard_out, sw_ansi, sw_lines, sw_ids,
	sw_trace, sw_case, sw_external, sw_pyxis, sw_version, sw_window_scope,
	sw_alsys, sw_d_float, sw_sql_dialect, sw_know_interp, sw_server_version,
	sw_ods_version, override_case, dialect_specified;

EXTERN SSHORT sw_interp;
EXTERN USHORT compiletime_db_dialect;


EXTERN TEXT ada_package[256], *ada_null_address;
EXTERN DBB isc_databases;
EXTERN TEXT *default_user, *default_password;
EXTERN TEXT *default_lc_ctype;
EXTERN TEXT *default_lc_messages;
EXTERN GPRE_REQ requests;
EXTERN LLS events;
EXTERN IB_FILE *out_file;
EXTERN LANG_T sw_language;
EXTERN int line, errors, warnings, fatals;
EXTERN ACT functions;
EXTERN struct dbd global_db_list[32];
EXTERN USHORT global_db_count;
EXTERN INTLSYM text_subtypes;

/* ada_flags fields definition */

#define ADA_create_database	1	/* the flag is set when there is a
								   create database SQL statement in
								   user program, and is used to
								   generate additional "with" and
								   "function" declarations    */

EXTERN USHORT ada_flags;

#undef EXTERN

#ifdef __cplusplus
} /* extern "C" */
#endif
#include "../gpre/msc_proto.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef assert
#ifdef DEV_BUILD
#define _assert(ex)	{if (!(ex)){(void) CPR_assert (__FILE__, __LINE__);}}
#define assert(ex)	_assert(ex)
#else
#define _assert(ex)
#define assert(ex)
#endif
#endif

#define assert_IS_REQ(x) assert(!(x) || ((x)->req_type >= 0 && (x)->req_type < REQ_LASTREQUEST))
#define assert_IS_SYM(x) assert(!(x) || ((x)->sym_type >= 0 && (x)->sym_type < SYM_LASTSYM))
#define assert_IS_NOD(x) assert(!(x) || ((x)->nod_type >= 1 && (x)->nod_type < nod_LASTNOD))
#define assert_IS_ACT(x) assert(!(x) || ((x)->act_type >= 0 && (x)->act_type < ACT_LASTACT))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _GPRE_GPRE_H_ */

