/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		ddl.h
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
 */

#ifndef _DUDLEY_DDL_H_
#define _DUDLEY_DDL_H_

#include "../jrd/common.h"

#ifdef VMS
#define BLOCK_SIZE 512
#else
#define BLOCK_SIZE 1024
#endif

#define MAXSYMLEN	257			/* max length of symbol + terminator */
#define MAX_PAGE_LEN	16384	/* max allowable length for a database page */

#define DDL_MSG_FAC	2

#define BUGCHECK(string)	DDL_error (string)
#define SYNTAX_ERROR(string)	PARSE_syntax_error (string)
#define IBERROR(string)		DDL_error (string)
#define BLKCHK(blk, type)	if (blk->blk_type != (SCHAR) type) BUGCHECK ("expected type")
#define LLS_PUSH(object,stack)	DDL_push ((DUDLEY_NOD) object, stack)
#define LLS_POP(stack)		DDL_pop (stack)


/* Action block.  Do something. */

/* Actions */

enum act_t {
	act_c_database,				/* create database */
	act_m_database,				/* modify database */
	act_d_database,				/* drop a database */
	act_a_relation,				/* add relation */
	act_m_relation,				/* modify existing relation */
	act_d_relation,				/* drop existing relations */
	act_a_gfield,				/* add global field */
	act_m_gfield,				/* modify existing global fields */
	act_d_gfield,				/* drop global field */
	act_a_field,				/* add field to relation */
	act_m_field,				/* modify relation specific fields */
	act_d_field,				/* drop field from relation */
	act_a_index,				/* add index */
	act_m_index,				/* modify index */
	act_d_index,				/* delete index */
	act_a_security,				/* add security class */
	act_d_security,				/* delete security class */
	act_m_security,				/* modify security class */
	act_a_trigger,				/* add new trigger */
	act_m_trigger,				/* modify (replace) trigger */
	act_d_trigger,				/* delete trigger */
	act_a_file,					/* add file */
	act_a_function,				/* add function */
	act_d_function,				/* drop function */
	act_a_function_arg,			/* add function */
	act_d_function_arg,			/* drop function */
	act_a_trigger_msg,			/* add trigger message */
	act_m_trigger_msg,			/* modify trigger message */
	act_d_trigger_msg,			/* drop trigger message */
	act_a_type,					/* add type for field */
	act_m_type,					/* modify type for field */
	act_d_type,					/* drop type for field */
	act_a_filter,				/* add filter */
	act_d_filter,				/* drop filter */
	act_grant,					/* grant user privilege */
	act_revoke,					/* revoke user privilege */
	act_a_shadow,				/* add shadow */
	act_d_shadow,				/* drop shadow */
	act_a_generator,			/* add generator */
	act_s_generator				/* reset generator value */
};

typedef struct act {
	enum act_t act_type;		/* what to do */
	struct act *act_next;		/* next action in system */
	struct dbb *act_object;		/* object in question (dudley_rel, dudley_fld, idx, etc.) */
	USHORT act_line;			/* line the action started on */
	USHORT act_flags;
} *ACT;

#define ACT_ignore	1			/* Ignore the action */

#define ACT_LEN sizeof (struct act)


/* Context block */

typedef struct dudley_ctx {
	struct sym *ctx_name;
	struct dudley_rel *ctx_relation;
	struct dudley_fld *ctx_field;
	USHORT ctx_view_rse;
	USHORT ctx_context_id;
} *DUDLEY_CTX;

#define CTX_LEN sizeof (struct dudley_ctx)


/* Database Definition Block */

typedef struct dbb {
	struct sym *dbb_name;
	struct dudley_rel *dbb_relations;
	struct gfl *dbb_fields;
	struct dbb *dbb_next;
	struct sym *dbb_security_class;
	SLONG *dbb_handle;
	SLONG *dbb_transaction;
	struct txt *dbb_description;
	USHORT dbb_flags;
	struct sym *dbb_file_name;
	struct fil *dbb_files;
	USHORT dbb_page_size;
	ULONG dbb_length;			/* Length of database in pages, if known */
	struct fil *dbb_logfiles;
	struct fil *dbb_overflow;
	SLONG dbb_chkptlen;
	SSHORT dbb_numbufs;
	SSHORT dbb_bufsize;
	SLONG dbb_grp_cmt_wait;
	struct fil *dbb_cache_file;
} *DBB;

#define DBB_null_description	1
#define DBB_null_security_class	2
#define DBB_create_database	4
#define DBB_drop_log		8
#define DBB_log_serial		16
#define DBB_log_preallocated	32
#define DBB_log_default		64
#define DBB_cascade		128
#define DBB_drop_cache		256

#define DBB_LEN	sizeof (struct dbb)

#define DB_VERSION_DDL4 4		/* ods4 db */
#define DB_VERSION_DDL6 6		/* ods6 db */
#define DB_VERSION_DDL8 8		/* ods8 db */


/* Field block.  Fields are what farms and databases are all about */

typedef struct dudley_fld {
	SSHORT fld_dtype;			/* data type of field */
	SSHORT fld_length;			/* field length in bytes */
	SSHORT fld_scale;			/* scale factor */
	SSHORT fld_position;		/* field position */
	SSHORT fld_segment_length;
	SSHORT fld_sub_type;
	SSHORT fld_has_sub_type;
	SSHORT fld_dimension;		/* size of multi-dim. array */
	SSHORT fld_system;			/* 0 if field is user defined */
	USHORT fld_flags;			/* misc trash */
	struct dudley_fld *fld_next;		/* next field in relation */
	struct dudley_rel *fld_relation;	/* relation */
	struct sym *fld_security_class;
	struct dudley_ctx *fld_context;	/* context for view */
	struct dbb *fld_database;	/* database for global fields */
	struct sym *fld_name;		/* field name */
	struct sym *fld_source;		/* name of global field */
	struct dudley_fld *fld_source_field;	/* global field for computed field */
	struct sym *fld_base;		/* base field for views */
	struct sym *fld_query_name;	/* query name */
	struct dudley_nod *fld_query_header;	/* query header */
	struct sym *fld_edit_string;	/* edit string */
	struct dudley_nod *fld_computed;	/* computed by expression */
	struct dudley_nod *fld_missing;	/* missing value */
	struct dudley_nod *fld_default;	/* default value */
	struct dudley_nod *fld_validation;	/* valid if value */
	struct txt *fld_description;	/* description of field */
	struct txt *fld_compute_src;	/* computed_by source */
	struct txt *fld_valid_src;	/* validation source */
	SLONG *fld_ranges;			/* ranges for multi-dim. array */
} *DUDLEY_FLD;

#define fld_explicit_position	1
#define fld_modify		2
#define fld_local		4
#define fld_null_description	8
#define fld_null_security_class	16
#define fld_null_validation	32
#define fld_explicit_system	64
#define fld_null_missing_value	128
#define fld_null_edit_string	256
#define fld_null_query_name	512
#define fld_null_query_header	1024

#define FLD_LEN sizeof (struct dudley_fld)


/* File description block */

typedef struct fil {
	SLONG fil_length;			/* File length in pages */
	SLONG fil_start;			/* Starting page */
	struct sym *fil_name;		/* File name */
	struct fil *fil_next;		/* next file */
	SSHORT fil_shadow_number;	/* shadow number if part of shadow */
	SSHORT fil_manual;			/* flag to indicate manual shadow */
	SSHORT fil_partitions;		/* number of log file partitions */
	SSHORT fil_raw;				/* on raw device? */
	SSHORT fil_conditional;		/* flag to indicate conditional shadow */
} *FIL;

#define FIL_LEN	sizeof (struct fil)


/* Filter block */

typedef struct filter {
	struct sym *filter_name;	/* symbol for filter name */
	struct txt *filter_description;	/* description of filter */
	struct sym *filter_module_name;	/* symbol for module name */
	struct sym *filter_entry_point;	/* symbol for entrypoint */
	SSHORT filter_input_sub_type;
	SSHORT filter_output_sub_type;
} *FILTER;

#define FILTER_LEN	sizeof (struct filter)


/* Function description block */

typedef struct func {
	struct sym *func_name;		/* symbol for function name */
	struct sym *func_query_name;	/* symbol for query name */
	struct sym *func_module_name;	/* symbol for module name */
	struct sym *func_entry_point;	/* symbol for entrypoint */
	SSHORT func_return_arg;		/* return argument position */
	struct func *func_next;		/* next function in database */
	struct dbb *func_database;	/* database for function */
	struct txt *func_description;	/* description of function */
	struct funcarg *func_args;	/* Known function arguments */
	struct funcarg *func_return;	/* Return argument */
} *FUNC;

#define FUNC_LEN sizeof (struct func)

/* Function argument block. */

typedef struct funcarg {
	struct sym *funcarg_funcname;	/* symbol for function name */
	SSHORT funcarg_position;	/* argument position */
	SSHORT funcarg_mechanism;	/* argument passed by value, or by reference */
	SSHORT funcarg_dtype;		/* data type of argument */
	SSHORT funcarg_scale;		/* scale factor */
	SSHORT funcarg_length;		/* argument length in bytes */
	SSHORT funcarg_return_arg;	/* argument is the designated return arg */
	SSHORT funcarg_sub_type;	/* sub_type of text */
	SSHORT funcarg_has_sub_type;	/* null field for sub_type field */
	struct funcarg *funcarg_next;	/* next field in function */
} *FUNCARG;

#define FUNCARG_mechanism_value		0
#define FUNCARG_mechanism_reference	1
#define FUNCARG_mechanism_descriptor	2
#define FUNCARG_mechanism_blob_struc	3
#define FUNCARG_mechanism_sc_array_desc	4

#define FUNCARG_LEN sizeof (struct funcarg)


/* Index description block */

typedef struct dudley_idx {
	USHORT idx_count;			/* Number of fields */
	UCHAR idx_unique;			/* TRUE if unique index */
	UCHAR idx_inactive;			/* FALSE if index is active */
	UCHAR idx_type;				/* 1 = descending */
	USHORT idx_flags;			/* Indicate which attributes have changed */
	struct sym *idx_name;		/* Index name */
	struct sym *idx_relation;	/* Relation in question */
	struct txt *idx_description;	/* Description pointer */
	struct sym *idx_field[1];	/* Fields */
} *DUDLEY_IDX;

#define IDX_type_none   0
#define IDX_type_descend	1

#define IDX_active_flag	1
#define IDX_unique_flag	2
#define IDX_null_description	4
#define IDX_type_flag	8
#define IDX_statistics_flag	16

#define IDX_LEN(cnt) (sizeof (struct dudley_idx) + (cnt - 1) * sizeof (((DUDLEY_IDX) 0)->idx_field[0]))


/* Linked list stack stuff */

typedef struct lls {
	struct dudley_nod *lls_object;		/* object on stack */
	struct lls *lls_next;		/* next item on stack */
} *LLS;

#define LLS_LEN sizeof (struct lls)


/* General Syntax node, produced by parser */

enum nod_t {
	nod_field = 1, nod_literal, nod_value,
	nod_and, nod_or, nod_not,
	nod_eql, nod_neq, nod_geq,
	nod_leq, nod_gtr, nod_lss,
	nod_containing, nod_matches, nod_any,
	nod_unique, nod_add, nod_multiply,
	nod_divide, nod_subtract, nod_negate,
	nod_msg, nod_for, nod_send,
	nod_receive, nod_block, nod_select,
	nod_boolean, nod_projection, nod_sort,
	nod_store, nod_modify, nod_erase,
	nod_if, nod_assignment, nod_rse,
	nod_first, nod_context, nod_end,
	nod_label, nod_leave, nod_loop,
	nod_max, nod_min, nod_count,
	nod_total, nod_average, nod_list,
	nod_defered, nod_between, nod_missing,
	nod_field_name, nod_name, nod_starts,
	nod_from, nod_fid, nod_concatenate,
	nod_abort, nod_null, nod_user_name,
	nod_post, nod_function, nod_gen_id,
	nod_uppercase, nod_sleuth, nod_over,
	nod_set_generator, nod_index
};

typedef struct dudley_nod {
	enum nod_t nod_type;		/* node type */
	UCHAR *nod_blr;				/* symbolic blr string */
	SSHORT nod_count;			/* number of sub-items */
	struct dudley_nod *nod_arg[1];		/* argument */
} *DUDLEY_NOD;

#define NOD_LEN(cnt) (sizeof (struct dudley_nod) + (cnt - 1) * sizeof (((DUDLEY_NOD) 0)->nod_arg[0]))


/* Relation block, not to be confused with siblings or in-laws */

typedef struct dudley_rel {
	struct dbb *rel_database;	/* parent database */
	struct sym *rel_filename;	/* external filename */
	struct dudley_fld *rel_fields;		/* linked list of known fields */
	struct sym *rel_name;		/* symbol for relation */
	struct sym *rel_security_class;	/* name of security class */
	struct dudley_rel *rel_next;		/* next relation in database */
	struct dudley_nod *rel_rse;		/* view rse */
	struct txt *rel_description;	/* description of relation */
	struct txt *rel_view_source;	/* source dml for view definition */
	USHORT rel_field_position;	/* highest used field position */
	SSHORT rel_system;			/* 0 if relation is user defined */
	USHORT rel_flags;
} *DUDLEY_REL;

#define rel_null_description	1
#define rel_null_security_class	2
#define rel_explicit_system	4
#define rel_marked_for_delete	8
#define rel_null_ext_file	16
#define rel_marked_for_modify	32
#define rel_marked_for_creation	64

#define REL_LEN sizeof (struct dudley_rel)


/* Security class handling */

typedef struct scl {
	struct sym *scl_name;		/* name of security class */
	struct txt *scl_description;	/* description of security class */
	struct sce *scl_entries;	/* list of entries */
} *SCL;

#define SCL_LEN  sizeof (struct scl)
#define SCL_write  2

/* Security entry */

typedef struct sce {
	struct sce *sce_next;		/* next security item in list */
	SLONG sce_privileges;		/* bitmask of privileges */
	UCHAR *sce_idents[20];		/* misc identification stuff */
	UCHAR sce_strings[1];
} *SCE;

#define SCE_LEN sizeof (struct sce)


/* String block for build DYN & BLR strings */

typedef struct str {
	SCHAR *str_start;			/* start of string buffer */
	SCHAR *str_current;			/* current position in string being built */
	USHORT str_length;			/* length of buffer */
} *STR;

#define STR_LEN sizeof (struct str)


/* Symbol block, also used for hash table */

enum sym_t {
	SYM_keyword,				/* unspecified */
	SYM_context,				/* context variable */
	SYM_database,				/* seems like a good idea */
	SYM_relation,				/* if you don't know your relations, how do you know your friends? */
	SYM_global,					/* Global field */
	SYM_field,					/* Local field */
	SYM_function,				/* UDF */
	SYM_trigger					/* any named element deserves to be hashed */
};

typedef struct sym {
	TEXT *sym_string;			/* address of asciz string */
	SSHORT sym_length;			/* length of string (exc. term.) */
	enum sym_t sym_type;		/* symbol type */
	SSHORT sym_keyword;			/* keyword number, if keyword */
	struct dudley_ctx *sym_object;		/* general pointer to object */
	struct sym *sym_collision;	/* collision pointer */
	struct sym *sym_homonym;	/* homonym pointer */
	TEXT sym_name[1];			/* space for name, if necessary */
} *SYM;

#define SYM_LEN sizeof (struct sym)


/* Trigger block */

/* these are the externally visible trigger types */

typedef enum {
	trg_type_none = 0,			/* pre store */
	trg_store = 1,				/* pre store */
	trg_post_store = 2,
	trg_modify = 3,				/* pre modify */
	trg_post_modify = 4,
	trg_pre_erase = 5,
	trg_erase = 6				/* post erase */
} TRG_T;

/* these types are used in parsing */

#define trig_pre	0
#define	trig_post 	1
#define	trig_sto	2
#define	trig_mod 	4
#define	trig_era	8			/* erase defaults to post */
#define	trig_inact	16

typedef struct dudley_trg {
	TRG_T trg_type;
	DUDLEY_REL trg_relation;
	DUDLEY_NOD trg_statement;			/* blr */
	struct sym *trg_name;		/* symbol for trigger */
	struct txt *trg_description;	/* description of relation */
	struct txt *trg_source;		/* source of trigger */
	SSHORT trg_sequence;
	SSHORT trg_inactive;		/* 0 = on, 1 = off */
	USHORT trg_mflag;			/* modify attributes */
} *DUDLEY_TRG;

/* trg_modify_flag */

#define trg_mflag_onoff         1
#define trg_mflag_type          2
#define trg_mflag_seqnum        4
#define trg_mflag_order		8

#define TRG_LEN sizeof (struct dudley_trg)

/* Trigger message block */

typedef struct trgmsg {
	struct sym *trgmsg_trg_name;	/* symbol for trigger */
	SSHORT trgmsg_number;		/* abort code */
	struct sym *trgmsg_text;
} *TRGMSG;

typedef enum {
	trgmsg_none = 0,
	trgmsg_add = 1,
	trgmsg_modify = 2,
	trgmsg_drop = 3
} TRGMSG_T;

#define TRGMSG_LEN sizeof (struct trgmsg)


/* Text block */

typedef struct txt {
	TEXT *txt_file;
	ULONG txt_position;
	USHORT txt_length;
	USHORT txt_start_line;
} *TXT;

#define TXT_LEN sizeof (struct txt)

/* Type block */

typedef struct typ {
	struct sym *typ_field_name;	/* field name */
	struct sym *typ_name;		/* type name */
	SSHORT typ_type;			/* type value */
	struct txt *typ_description;	/* description of relation */
} *TYP;

#define TYP_LEN sizeof (struct typ)


/* User privilege block */

typedef struct userpriv {
	struct sym *userpriv_relation;
	struct usre *userpriv_userlist;
	struct upfe *userpriv_upflist;
	USHORT userpriv_flags;
} *USERPRIV;

#define USERPRIV_LEN sizeof (struct userpriv)

/* user privilege flags */

#define USERPRIV_select	1
#define USERPRIV_delete	2
#define USERPRIV_insert	4
#define USERPRIV_update	8
#define USERPRIV_grant	16

/* rdb$user_privilege.rdb$privilege */

#define UPRIV_SELECT	"SELECT"
#define UPRIV_DELETE	"DELETE"
#define UPRIV_INSERT	"INSERT"
#define UPRIV_UPDATE	"UPDATE"

/* user name entry */

typedef struct usre {
	struct usre *usre_next;
	struct sym *usre_name;
} *USRE;

#define USRE_LEN sizeof (struct usre)

/* update field entry */

typedef struct upfe {
	struct upfe *upfe_next;
	struct sym *upfe_fldname;
} *UPFE;

#define UPFE_LEN sizeof (struct upfe)


/* Data types */

#include "../jrd/dsc.h"

#define DSC_LEN sizeof (struct dsc)


/* Constant block */

typedef struct con {
	struct dsc con_desc;
	UCHAR con_data[1];
} *CON;

#define CON_LEN sizeof (struct con)


/* Program globals */

typedef enum lan_t {
	lan_undef,
	lan_pascal,
	lan_fortran,
	lan_cobol,
	lan_ansi_cobol,
	lan_c,
	lan_ada,
	lan_cxx
} LAN_T;

#ifdef DDL_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

EXTERN enum lan_t language;
EXTERN USHORT DDL_eof, DDL_errors, DDL_line, DDL_interactive, DDL_quit,
	DDL_dynamic, DDL_drop_database, DDL_service;
EXTERN UCHAR DDL_replace, DDL_description, DDL_extract, DDL_trace,
	DDL_version;
EXTERN TEXT *DDL_file_name, DYN_file_name[256], *DB_file_name,
	DDL_file_string[256], DB_file_string[256];
EXTERN TEXT *DDL_default_user, *DDL_default_password;
EXTERN ACT DDL_actions;
EXTERN DBB database;

#undef EXTERN

#include "../dudley/ddl_proto.h"

#define s_rse_first	0			/* FIRST clause, if any */
#define s_rse_boolean	1		/* Boolean clause, if any */
#define s_rse_sort	2			/* Sort clause, if any */
#define s_rse_reduced	3		/* Reduced clause, if any */
#define s_rse_contexts	4		/* Relation block */
#define s_rse_count	5

#define s_stt_rse	0
#define s_stt_value	1
#define s_stt_default	2
#define s_stt_count	3

#define s_fld_field	0			/* Field block */
#define s_fld_context	1		/* Context block */
#define s_fld_name	2
#define s_fld_subs	3
#define s_fld_count	4

#define s_if_boolean	0
#define s_if_true	1
#define s_if_false	2

#define s_for_rse	0
#define s_for_action	1

#define s_store_rel	0
#define s_store_action	1

#define s_mod_old_ctx	0
#define s_mod_new_ctx	1
#define s_mod_action	2

#endif /* _DUDLEY_DDL_H_ */
