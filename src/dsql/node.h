/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		node.h
 *	DESCRIPTION:	Definitions needed for accessing a parse tree
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
 * 2001.6.12 Claudio Valderrama: add break_* constants.
 * 2001.6.30 Claudio valderrama: Jim Starkey suggested to hold information
 * about source line in each node that's created.
 * 2001.07.28 John Bellardo: Added e_rse_limit to nod_rse and nod_limit.
 * 2001.08.03 John Bellardo: Reordered args to no_sel for new LIMIT syntax
 * 2002.07.30 Arno Brinkman:
 * 2002.07.30	Added nod_searched_case, nod_simple_case, nod_coalesce
 * 2002.07.30	and constants for arguments
 * 2002.08.04 Dmitry Yemanov: ALTER VIEW
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 */

#ifndef DSQL_NODE_H
#define DSQL_NODE_H

/* an enumeration of the possible node types in a syntax tree */

typedef enum nod_t 
{
	nod_unknown_type = 0,
	nod_commit = 1,	/* Commands, not executed. */
	nod_rollback,
	nod_trans,
	nod_def_default,
	nod_del_default,
	nod_def_database,
	nod_def_domain,	
	nod_mod_domain,
	nod_del_domain,
	nod_mod_database, /* 10 */
	nod_def_relation,
	nod_mod_relation,
	nod_del_relation,
	nod_def_field,
	nod_mod_field,
	nod_del_field,
	nod_def_index,
	nod_del_index,
	nod_def_view,
	nod_def_constraint, /* 20 */
	nod_def_trigger,
	nod_mod_trigger,
	nod_del_trigger,
	nod_def_trigger_msg,
	nod_mod_trigger_msg,
	nod_del_trigger_msg,
	nod_def_procedure,	
	nod_mod_procedure,
	nod_del_procedure,
	nod_def_exception,  /* 30 */
	nod_mod_exception,
	nod_del_exception,
	nod_def_generator,
	nod_del_generator,
	nod_def_filter,
	nod_del_filter,
	nod_def_shadow,	
	nod_del_shadow,
	nod_def_udf,
	nod_del_udf,    /* 40 */
	nod_grant,
	nod_revoke,
	nod_rel_constraint,
	nod_delete_rel_constraint,
	nod_primary,
	nod_foreign,
	nod_abort,	
	nod_references,
	nod_proc_obj,
	nod_trig_obj, /* 50 */
	nod_view_obj,
	nod_list,	/* SQL statements, mapped into GDML statements */
	nod_select,
	nod_insert,
	nod_delete,
	nod_update,	
	nod_close,
	nod_open,
	nod_all,	/* ALL privileges */
	nod_execute, /* 60 */		/* EXECUTE privilege */
	nod_store,
	nod_modify,
	nod_erase,
	nod_assign,	
	nod_exec_procedure,
	nod_return,	/* Procedure statements */
	nod_exit,
	nod_while,
	nod_if,
	nod_for_select, /* 70 */
	nod_erase_current,
	nod_modify_current,
	nod_post,
	nod_block,	
	nod_on_error,
	nod_sqlcode,
	nod_gdscode,
	nod_exception,
	nod_exception_stmt,
	nod_default, /* 80 */
	nod_start_savepoint,
	nod_end_savepoint,
	nod_cursor,	/* used to create record streams */
	nod_relation,				
	nod_relation_name,
	nod_procedure_name,
	nod_rel_proc_name,
	nod_rse,
	nod_select_expr,            
	nod_union,                  /* 90 */
	nod_aggregate,
	nod_order,
	nod_flag,	
	nod_join,
/* NOTE: when adding an expression node, be sure to
   test various combinations; in particular, think 
   about parameterization using a '?' - there is code
   in PASS1_node which must be updated to find the
   data type of the parameter based on the arguments 
   to an expression node */
	nod_eql,
	nod_neq,
	nod_gtr,
	nod_geq,
	nod_leq,
	nod_lss,         /* 100 */
	nod_between,
	nod_like,
	nod_missing,
	nod_and,
	nod_or,
	nod_any,
	nod_not,
	nod_unique,
	nod_containing,
	nod_starting,
	nod_via,
	nod_field,	/* values */
	nod_dom_value,
	nod_field_name,
	nod_parameter,
	nod_constant,
	nod_map,
	nod_alias,
	nod_user_name,
	nod_user_group,
	nod_variable,
	nod_var_name,
	nod_array,
	nod_add,	/* functions */
	nod_subtract,
	nod_multiply,
	nod_divide,
	nod_negate,
	nod_concatenate,
	nod_substr,
	nod_null,
	nod_dbkey,
	nod_udf,
	nod_cast,
	nod_upcase,
	nod_collate,
	nod_gen_id,
	nod_add2,	/* functions different for dialect >= V6_TRANSITION */
	nod_subtract2,
	nod_multiply2,
	nod_divide2,
	nod_gen_id2,
	nod_average,	/* aggregates */
	nod_from,
	nod_max,
	nod_min,
	nod_total,
	nod_count,
	nod_exists,
	nod_singular,
	nod_agg_average,
	nod_agg_max,
	nod_agg_min,
	nod_agg_total,
	nod_agg_count,
	nod_agg_average2,
	nod_agg_total2,
	nod_get_segment,	/* blobs */
	nod_put_segment,
	nod_join_inner,	/* join types */
	nod_join_left,
	nod_join_right,
	nod_join_full,
	/* sql transaction support */
	nod_access,
	nod_wait,
	nod_isolation,
	nod_version,
	nod_table_lock,
	nod_lock_mode,
	nod_reserve,
	nod_commit_retain,
	/* sql database stmts support */
	nod_page_size,
	nod_file_length,
	nod_file_desc,
	nod_log_file_desc,
	nod_cache_file_desc,
	nod_group_commit_wait,
	nod_check_point_len,
	nod_num_log_buffers,
	nod_log_buffer_size,
	nod_drop_log,
	nod_drop_cache,
	nod_dfl_charset,
	/* sql connect options support (used for create database) */
	nod_password,
	nod_lc_ctype,	/* SET NAMES */
	/* Misc nodes */
	nod_udf_return_value,
	/* computed field */
	nod_def_computed,
	/* access plan stuff */
	nod_plan_expr,
	nod_plan_item,
	nod_merge,
	nod_natural,
	nod_index,
	nod_index_order,
	nod_set_generator,
	nod_set_generator2,	/* SINT64 value for dialect > V6_TRANSITION */
	/* alter index */
	nod_mod_index,
	nod_idx_active,
	nod_idx_inactive,
		/* drop behaviour */
	nod_restrict,
	nod_cascade,
	/* set statistics */
	nod_set_statistics,
	/* record version */
	nod_rec_version,
	/* ANY keyword used */
	nod_ansi_any,
	nod_eql_any,
	nod_neq_any,
	nod_gtr_any,
	nod_geq_any,
	nod_leq_any,
	nod_lss_any,
	/* ALL keyword used */
	nod_ansi_all,
	nod_eql_all,
	nod_neq_all,
	nod_gtr_all,
	nod_geq_all,
	nod_leq_all,
	nod_lss_all,
	/*referential integrity actions */
	nod_ref_upd_del,
	nod_ref_trig_action,
	/* SQL role support */
	nod_def_role,
	nod_role_name,
	nod_grant_admin,
	nod_del_role,
	/* SQL time & date support */
	nod_current_time,
	nod_current_date,
	nod_current_timestamp,
	nod_extract,
	/* ALTER column/domain support */
	nod_mod_domain_type,
	nod_mod_field_name,
	nod_mod_field_type,
	nod_mod_field_pos,

	/* CVC: SQL requires that DROP VIEW and DROP table are independent. */
	nod_del_view,
	nod_current_role, /* nod_role_name is already taken but only for DDL. */
	nod_breakleave,
	nod_redef_relation, /* allows silent creation/overwriting of a relation. */
	nod_udf_param, /* there should be a way to signal a param by descriptor! */
	nod_limit, /* limit support */
	nod_redef_procedure, /* allows silent creation/overwriting of a procedure. */
	nod_exec_sql, /* EXECUTE STATEMENT */
	nod_internal_info, /* internal engine info */
	nod_searched_case, /* searched CASE function */
	nod_simple_case, /* simple CASE function */
	nod_coalesce, /* COALESCE function */
	nod_mod_view, /* ALTER VIEW */
	nod_replace_procedure, /* CREATE OR ALTER PROCEDURE */
	nod_replace_trigger, /* CREATE OR ALTER TRIGGER */
	nod_replace_view, /* CREATE OR ALTER VIEW */
	nod_redef_view, /* allows silent creation/overwriting of a view */
	nod_for_update, /* FOR UPDATE clause */
	nod_user_savepoint, /* savepoints support */
	nod_release_savepoint,
	nod_undo_savepoint,
	nod_label, /* label support */
	nod_exec_into, /* EXECUTE STATEMENT INTO */
	nod_difference_file,
	nod_drop_difference,
	nod_begin_backup,
	nod_end_backup,
	nod_derived_table, // Derived table support
	nod_derived_field  // Derived table support
} NOD_TYPE;



/* definition of a syntax node created both
   in parsing and in context recognition */

class dsql_nod : public pool_alloc_rpt<class dsql_nod*, dsql_type_nod>
{
public:
	NOD_TYPE nod_type;			/* Type of node */
	DSC nod_desc;				/* Descriptor */
	USHORT nod_line;			/* Source line of the statement. */
	USHORT nod_column;			/* Source column of the statement. */
	USHORT nod_count;			/* Number of arguments */
	USHORT nod_flags;
	dsql_nod* nod_arg[1];
	
	dsql_nod() : nod_type(nod_unknown_type), nod_count(0), nod_flags(0) {}
};
typedef dsql_nod* DSQL_NOD;

/* values of flags */

#define NOD_AGG_DISTINCT 	1

#define NOD_UNION_ALL	 	1

#define NOD_SINGLETON_SELECT	1

#define NOD_READ_ONLY	1
#define NOD_READ_WRITE	2

#define NOD_WAIT	1
#define NOD_NO_WAIT	2

#define NOD_VERSION	1
#define NOD_NO_VERSION	2

#define NOD_CONCURRENCY		1
#define NOD_CONSISTENCY		2
#define NOD_READ_COMMITTED	4

#define NOD_SHARED	1
#define NOD_PROTECTED	2
#define NOD_READ	4
#define NOD_WRITE	8

#define REF_ACTION_CASCADE 1
#define REF_ACTION_SET_DEFAULT 2
#define REF_ACTION_SET_NULL 4
#define REF_ACTION_NONE 8

/* Node flag indicates that this node has a different type or result
   depending on the SQL dialect. */
#define NOD_COMP_DIALECT	16

/* Parameters to MAKE_constant */

#define CONSTANT_STRING	    0	/* stored as a string */
#define CONSTANT_SLONG	    1	/* stored as a SLONG */
#define CONSTANT_DOUBLE	    2	/* stored as a string */
#define CONSTANT_DATE	    3	/* stored as a SLONG */
#define CONSTANT_TIME	    4	/* stored as a ULONG */
#define CONSTANT_TIMESTAMP  5	/* stored as a QUAD */
#define CONSTANT_SINT64     6	/* stored as a SINT64 */



/* enumerations of the arguments to a node, offsets
   within the variable tail nod_arg */

/* Note Bene:
 *	e_<nodename>_count	== count of arguments in nod_arg
 *	This is often used as the count of sub-nodes, but there
 *	are cases when non-DSQL_NOD arguments are stuffed into nod_arg
 *	entries.  These include nod_udf, nod_gen_id, nod_cast,
 *	& nod_collate.
 */

#define e_select_expr   0       /* nod_select */
#define e_select_order  1
#define e_select_update 2
#define e_select_lock   3
#define e_select_count  4

#define e_fpd_list      0       /* nod_for_update */
#define e_fpd_count     1

#define e_sav_name      0       /* nod_user_savepoint, nod_undo_savepoint */
#define e_sav_count     1

#define e_fld_context	0		/* nod_field */
#define e_fld_field	1
#define e_fld_indices	2
#define e_fld_count	3

#define e_ary_array	0			/* nod_array */
#define e_ary_indices	1
#define e_ary_count	2

#define e_xcp_name	0			/* nod_exception */
#define e_xcp_text	1
#define e_xcp_count	2

#define e_blk_action	0		/* nod_block */
#define e_blk_errs	1
#define e_blk_count	2

#define e_err_errs	0			/* nod_on_error */
#define e_err_action	1
#define e_err_count	2

#define e_var_variable	0		/* nod_variable */
#define e_var_count	1

#define e_pst_event	0			/* nod_post */
#define e_pst_argument	1
#define e_pst_count	2

#define e_exec_sql_stmnt	0	/* nod_exec_sql */
#define e_exec_sql_count	1

#define e_exec_into_stmnt	0	/* nod_exec_into */
#define e_exec_into_block	1
#define e_exec_into_list	2
#define e_exec_into_label	3
#define e_exec_into_count	4

#define e_internal_info	0		/* nod_internal_info */
#define e_internal_info_count	1

#define e_xcp_name	0			/* nod_exception_stmt */
#define e_xcp_msg	1
#define e_xcp_count	2

#define e_rtn_procedure 0		/* nod_procedure */
#define e_rtn_count	1

#define e_vrn_name	0			/* nod_variable_name */
#define e_vrn_count	1

#define e_fln_context	0		/* nod_field_name */
#define e_fln_name	1
#define e_fln_count	2

#define e_rel_context	0		/* nod_relation */
#define e_rel_count	1

#define e_agg_context	0		/* nod_aggregate */
#define e_agg_group	1
#define e_agg_rse	2
#define e_agg_count	3

#define e_rse_streams	0		/* nod_rse */
#define e_rse_boolean	1
#define e_rse_sort	2
#define e_rse_reduced	3
#define e_rse_items	4
#define e_rse_first	5
#define e_rse_singleton	6
#define e_rse_plan	7
#define e_rse_skip	8
#define e_rse_lock  9
#define e_rse_count	10

#define e_limit_skip	0	/* nod_limit */
#define e_limit_length	1
#define e_limit_count	2

#define e_par_parameter	0		/* nod_parameter */
#define e_par_count	1

#define e_flp_select	0		/* nod_for_select */
#define e_flp_into	1
#define e_flp_cursor	2
#define e_flp_action	3
#define e_flp_label	4
#define e_flp_count	5

#define e_cur_name	0			/* nod_cursor */
#define e_cur_context	1
#define e_cur_next	2
#define e_cur_count	3

#define e_prc_name      0		/* nod_procedure */
#define e_prc_inputs    1
#define e_prc_outputs   2
#define e_prc_dcls      3
#define e_prc_body      4
#define e_prc_source    5
#define e_prc_cursors   6
#define e_prc_count     7

#define e_exe_procedure 0		/* nod_exec_procedure */
#define e_exe_inputs    1
#define e_exe_outputs   2
#define e_exe_count     3

#define e_msg_number	0		/* nod_message */
#define e_msg_text	1
#define e_msg_count	2

#define e_sel_limit	0	/* nod_select_expr */
#define e_sel_distinct	1
#define e_sel_list	2
#define e_sel_from	3
#define e_sel_where	4
#define e_sel_group	5
#define e_sel_having	6
#define e_sel_plan	7
#define e_sel_order	8
#define e_sel_singleton	9
#define e_sel_count	10

#define e_ins_relation	0		/* nod_insert */
#define e_ins_fields	1
#define e_ins_values	2
#define e_ins_select	3
#define e_ins_count	4

#define e_sto_relation	0		/* nod_store */
#define e_sto_statement	1
#define e_sto_rse	2
#define e_sto_count	3

#define e_del_relation	0		/* nod_delete */
#define e_del_boolean	1
#define e_del_cursor	2
#define e_del_count	3

#define e_era_relation	0		/* nod_erase */
#define e_era_rse	1
#define e_era_count	2

#define e_erc_context	0		/* nod_erase_current */
#define e_erc_count	1

#define e_mdc_context	0		/* nod_modify_current */
#define e_mdc_update	1
#define e_mdc_statement	2
#define e_mdc_count	3

#define e_upd_relation	0		/* nod_update */
#define e_upd_statement	1
#define e_upd_boolean	2
#define e_upd_cursor	3
#define e_upd_count	4

#define e_mod_source	0		/* nod_modify */
#define e_mod_update	1
#define e_mod_statement	2
#define e_mod_rse	3
#define e_mod_count	4

#define e_map_context	0		/* nod_map */
#define e_map_map	1
#define e_map_count	2

#define e_blb_field	0			/* nod_get_segment & nod_put_segment */
#define e_blb_relation	1
#define e_blb_filter	2
#define e_blb_max_seg	3
#define e_blb_count	4

#define e_idx_unique	0		/* nod_def_index */
#define e_idx_asc_dsc	1
#define e_idx_name	2
#define e_idx_table	3
#define e_idx_fields	4
#define e_idx_count	5

#define e_rln_name	0			/* nod_relation_name */
#define e_rln_alias	1
#define e_rln_count	2

#define e_rpn_name	0			/* nod_rel_proc_name */
#define e_rpn_alias	1
#define e_rpn_inputs	2
#define e_rpn_count	3

#define e_join_left_rel	0		/* nod_join */
#define e_join_type	1
#define e_join_rght_rel	2
#define e_join_boolean	3
#define e_join_count	4

#define e_via_rse       0
#define e_via_value_1   1
#define e_via_value_2	2
#define e_via_count	3

#define e_if_condition	0
#define e_if_true	1
#define e_if_false	2
#define e_if_count	3

#define e_while_cond	0
#define e_while_action	1
#define e_while_label	2
#define e_while_count	3

#define e_drl_name	0			/* nod_def_relation */
#define e_drl_elements	1
#define e_drl_ext_file	2		/* external file */
#define e_drl_count	3

#define e_dft_default	0		/* nod_def_default  */
#define e_dft_default_source 1
#define e_dft_count     2

#define e_dom_name	0			/* nod_def_domain */
#define e_dom_default	1
#define e_dom_default_source 2
#define e_dom_constraint 3
#define e_dom_collate   4
#define e_dom_count     5

#define e_dfl_field	0			/* nod_def_field */
#define e_dfl_default	1
#define e_dfl_default_source 2
#define e_dfl_constraint 3
#define e_dfl_collate   4
#define e_dfl_domain    5
#define e_dfl_computed	6
#define e_dfl_count     7

#define e_view_name	0			/* nod_def_view */
#define e_view_fields	1
#define e_view_select	2
#define e_view_check	3
#define e_view_source   4
#define e_view_count	5

#define e_alt_name	0			/* nod_mod_relation */
#define e_alt_ops	1
#define e_alt_count	2

#define e_grant_privs	0		/* nod_grant */
#define e_grant_table	1
#define e_grant_users	2
#define e_grant_grant	3
#define e_grant_count	4

#define e_alias_value	0		/* nod_alias */
#define e_alias_alias	1
#define e_alias_count	2

#define e_rct_name	0			/* nod_rel_constraint  */
#define e_rct_type	1

#define e_pri_columns	0		/* nod_primary */
#define e_pri_index	1
#define e_pri_count	2

#define e_for_columns	0		/* nod_foreign */
#define e_for_reftable	1
#define e_for_refcolumns 2
#define e_for_action    3
#define e_for_index	4
#define e_for_count	5

#define e_ref_upd 0				/* nod_ref_upd_del_action */
#define e_ref_del 1
#define e_ref_upd_del_count 2

#define e_ref_trig_action_count 0

#define e_cnstr_name		0	/* nod_def_constraint */
#define e_cnstr_table		1	/* NOTE: IF ADDING AN ARG IT MUST BE */
#define e_cnstr_type		2	/* NULLED OUT WHEN THE NODE IS */
#define e_cnstr_position	3	/* DEFINED IN parse.y */
#define e_cnstr_condition	4
#define e_cnstr_actions		5
#define e_cnstr_source		6
#define e_cnstr_message		7
#define e_cnstr_else		8
#define e_cnstr_count		9

#define e_trg_name	0			/* nod_mod_trigger and nod_def trigger */
#define e_trg_table	1
#define e_trg_active	2
#define e_trg_type	3
#define e_trg_position	4
#define e_trg_actions	5
#define e_trg_source	6
#define e_trg_messages	7
#define e_trg_cursors	8
#define e_trg_count	9

#define e_abrt_number	0		/* nod_abort */
#define e_abrt_count	1

#define e_cast_target   0	/* Not a DSQL_NOD */	/* nod_cast */
#define e_cast_source   1
#define e_cast_count    2

#define e_coll_target   0       /* Not a DSQL_NOD */	/* nod_collate */
#define e_coll_source   1
#define e_coll_count    2

#define e_order_field   0		/* nod_order */
#define e_order_flag    1
#define e_order_nulls   2
#define e_order_count   3

#define e_lock_tables	0
#define e_lock_mode	1

#define e_database_name 0
#define e_database_initial_desc 1
#define e_database_rem_desc 2
#define e_cdb_count 3

#define e_commit_retain	0

#define e_adb_all 0
#define e_adb_count 1

#define e_gen_name  0
#define e_gen_count 1

#define e_filter_name     0
#define e_filter_in_type  1
#define e_filter_out_type 2
#define e_filter_entry_pt 3
#define e_filter_module   4
#define e_filter_count    5

#define e_gen_id_name 0		/* Not a DSQL_NOD */	/* nod_gen_id */
#define e_gen_id_value 1
#define e_gen_id_count 2


#define e_udf_name          0
#define e_udf_entry_pt      1
#define e_udf_module        2
#define e_udf_args          3
#define e_udf_return_value  4
#define e_udf_count         5

/* computed field */

#define e_cmp_expr	0
#define e_cmp_text	1

/* create shadow */

#define e_shadow_number  	0
#define e_shadow_man_auto 	1
#define e_shadow_conditional 	2
#define e_shadow_name		3
#define e_shadow_length 	4
#define e_shadow_sec_files 	5
#define e_shadow_count	        6

/* alter index */

#define e_alt_index		0
#define e_mod_idx_count		1

#define e_alt_idx_name		0
#define e_alt_idx_name_count	1

/* set statistics */

#define e_stat_name		0
#define e_stat_count		1

/* SQL extract() function */

#define e_extract_part  0	/* constant representing part to extract */
#define e_extract_value	1	/* Must be a time or date value */
#define e_extract_count	2

/* SQL CURRENT_TIME, CURRENT_DATE, CURRENT_TIMESTAMP */

#define e_current_time_count		0
#define e_current_date_count		0
#define e_current_timestamp_count	0

#define e_alt_dom_name			0	/* nod_modify_domain */
#define e_alt_dom_ops			1
#define e_alt_dom_count			2

#define e_mod_dom_new_dom_type		0	/* nod_mod_domain_type */
#define e_mod_dom_count			1

#define e_mod_fld_name_orig_name	0	/* nod_mod_field_name */
#define e_mod_fld_name_new_name		1
#define e_mod_fld_name_count		2

#define e_mod_fld_type			0	/* nod_mod_field_type */
#define e_mod_fld_type_dom_name		2
#define e_mod_fld_type_count		2

#define e_mod_fld_pos_orig_name		0	/* nod_mod_field_position */
#define e_mod_fld_pos_new_position	1
#define e_mod_fld_pos_count		2

/* CVC: blr_leave used to emulate break */
#define e_breakleave_label	0	/* nod_breakleave */
#define e_breakleave_count	1

/* SQL substring() function */

#define e_substr_value	0	/* Anything that can be treated as a string */
#define e_substr_start	1	/* Where the slice starts */
#define e_substr_length	2	/* The length of the slice */
#define e_substr_count	3

#define e_udf_param_field	0
#define e_udf_param_type	1 /* Basically, by_reference or by_descriptor */
#define e_udf_param_count	2

/* CASE <case_operand> {WHEN <when_operand> THEN <when_result>}.. [ELSE <else_result>] END 
   Node-constants for after pass1 */

#define e_simple_case_case_operand	0	/* 1 value */
#define e_simple_case_when_operands	1	/* list */
#define e_simple_case_results		2	/* list including else_result */

/* CASE {WHEN <search_condition> THEN <when_result>}.. [ELSE <else_result>] END 
   Node-constants for after pass1 */

#define e_searched_case_search_conditions	0	/* list boolean expressions */
#define e_searched_case_results				1	/* list including else_result */

#define e_label_name	0
#define e_label_number	1
#define e_label_count	2

#define e_derived_table_rse				0  // Contains select_expr
#define e_derived_table_alias			1  // Alias name for derived table
#define e_derived_table_column_alias	2  // List with alias names from derived table columns
#define e_derived_table_count			3  

#define e_derived_field_value			0  // Contains the source expression
#define e_derived_field_name			1  // Name for derived table field
#define e_derived_field_scope			2  // Scope-level
#define e_derived_field_count			4  

#endif // DSQL_NODE_H

