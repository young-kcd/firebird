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
 * 2004.01.16 Vlad Horsun: added support for default parameters and
 *   EXECUTE BLOCK statement
 * Adriano dos Santos Fernandes
 */

#ifndef DSQL_NODE_H
#define DSQL_NODE_H

#include "../dsql/dsql.h"
#include "../common/classes/Aligner.h"

// an enumeration of the possible node types in a syntax tree

namespace Dsql {

enum nod_t
{
	// NOTE: when adding an expression node, be sure to
	// test various combinations; in particular, think
	// about parameterization using a '?' - there is code
	// in PASS1_node which must be updated to find the
	// data type of the parameter based on the arguments
	// to an expression node.

	nod_unknown_type = 0,
	nod_commit,
	nod_rollback,
	nod_trans,
	nod_def_default,
	nod_del_default,
	nod_def_database,
	nod_def_domain,
	nod_mod_database,
	nod_def_field,
	nod_mod_field,
	nod_del_field,
	nod_def_index,
	nod_del_index,
	nod_def_constraint,
	nod_del_generator,
	nod_def_filter,
	nod_del_filter,
	nod_def_shadow,
	nod_del_shadow,
	nod_def_udf,
	nod_del_udf,
	nod_grant,
	nod_revoke,
	nod_rel_constraint,
	nod_delete_rel_constraint,
	nod_primary,
	nod_foreign,
	nod_references,
	nod_proc_obj,
	nod_trig_obj,
	nod_view_obj,
	nod_list,
	nod_select,
	nod_insert,
	nod_delete,
	nod_update,
	nod_all,	// ALL privileges
	nod_execute,	// EXECUTE privilege
	nod_store,
	nod_modify,
	nod_erase,
	nod_assign,
	nod_while,
	nod_erase_current,
	nod_modify_current,
	nod_block,
	nod_on_error,
	nod_sqlcode,
	nod_gdscode,
	nod_exception,
	nod_default,
	nod_start_savepoint,
	nod_end_savepoint,
	nod_cursor,
	nod_procedure_name,
	nod_select_expr,
	nod_order,
	nod_flag,
	nod_unique,
	nod_dom_value,
	nod_field_name,
	nod_alias,
	nod_user_name,
	nod_user_group,
	nod_var_name,
	nod_array,
	nod_not_null,
	nod_collate,
	nod_get_segment,	// blobs
	nod_put_segment,
	nod_access,
	nod_wait,
	nod_isolation,
	nod_version,
	nod_table_lock,
	nod_lock_mode,
	nod_reserve,
	nod_retain,
	nod_page_size,
	nod_file_length,
	nod_file_desc,
	nod_dfl_charset,
	nod_password,
	nod_lc_ctype,	// SET NAMES
	nod_udf_return_value,
	nod_def_computed,
	nod_plan_expr,
	nod_plan_item,
	nod_natural,
	nod_index,
	nod_index_order,
	nod_set_generator,
	nod_set_generator2,
	nod_mod_index,	// alter index
	nod_idx_active,
	nod_idx_inactive,
	nod_restrict,	// drop behaviour
	nod_cascade,
	nod_set_statistics,	// set statistics
	nod_ref_upd_del,	// referential integrity actions
	nod_ref_trig_action,
	nod_def_role,	// SQL role support
	nod_role_name,
	nod_grant_admin,
	nod_del_role,
	nod_mod_field_name,
	nod_mod_field_type,
	nod_mod_field_pos,
	nod_udf_param, // there should be a way to signal a param by descriptor!
	nod_for_update, // FOR UPDATE clause
	nod_label, // label support
	nod_difference_file,
	nod_drop_difference,
	nod_begin_backup,
	nod_end_backup,
	nod_rows,	// ROWS support
	nod_mod_udf,
	nod_returning,
	nod_tra_misc,
	nod_lock_timeout,
	nod_src_info,
	nod_with,
	nod_update_or_insert,
	nod_merge,
	nod_merge_when,
	nod_merge_update,
	nod_merge_delete,
	nod_merge_insert,
	nod_mod_role,
	nod_add_user,
	nod_mod_user,
	nod_del_user,
	nod_dfl_collate,
	nod_trg_act,
	nod_trg_ext,
	nod_class_stmtnode,
	nod_class_exprnode,
	nod_package_name,
	nod_package_obj,
	nod_mod_field_null_flag,
	nod_func_obj,
	nod_function_name
};

/* enumerations of the arguments to a node, offsets
   within the variable tail nod_arg */

/* Note Bene:
 *	e_<nodename>_count	== count of arguments in nod_arg
 *	This is often used as the count of sub-nodes, but there
 *	are cases when non-DSQL_NOD arguments are stuffed into nod_arg
 *	entries.  These include nod_udf and nod_collate.
 */
enum node_args {
	e_select_expr = 0,		// nod_select
	e_select_update,
	e_select_lock,
	e_select_count,

	e_fpd_list = 0,			// nod_for_update
	e_fpd_count,

	e_ary_array = 0,		// nod_array
	e_ary_indices,
	e_ary_count,

	e_blk_action = 0,		// nod_block
	e_blk_errs,
	e_blk_count,

	e_err_errs = 0,			// nod_on_error
	e_err_action,
	e_err_count,

	e_vrn_name = 0,			// nod_variable_name
	e_vrn_count,

	e_fln_context = 0,		// nod_field_name
	e_fln_name,
	e_fln_count,

	e_rows_skip = 0,		// nod_rows
	e_rows_length,
	e_rows_count,

	e_cur_name = 0,			// nod_cursor
	e_cur_scroll,
	e_cur_rse,
	e_cur_number,
	e_cur_count,

	e_sel_query_spec = 0,	// nod_select_expr
	e_sel_order,
	e_sel_rows,
	e_sel_with_list,
	e_sel_alias,			// Alias name for derived table
	e_sel_columns,			// List with alias names from derived table columns
	e_sel_count,

	e_ins_relation = 0,		// nod_insert
	e_ins_fields,
	e_ins_values,
	e_ins_select,
	e_ins_return,
	e_ins_count,

	e_mrg_relation = 0,		// nod_merge
	e_mrg_using,
	e_mrg_condition,
	e_mrg_when,
	e_mrg_return,
	e_mrg_count,

	e_mrg_when_matched = 0,	// nod_merge_when
	e_mrg_when_not_matched,
	e_mrg_when_count,

	e_mrg_update_condition = 0,	// nod_merge_update
	e_mrg_update_statement,
	e_mrg_update_count,

	e_mrg_delete_condition = 0,	// nod_merge_delete
	e_mrg_delete_count,

	e_mrg_insert_condition = 0,	// nod_merge_insert
	e_mrg_insert_fields,
	e_mrg_insert_values,
	e_mrg_insert_count,

	e_sto_relation = 0,		// nod_store
	e_sto_statement,
	e_sto_rse,
	e_sto_return,
	e_sto_count,

	e_upi_relation = 0,		// nod_update_or_insert
	e_upi_fields,
	e_upi_values,
	e_upi_matching,
	e_upi_return,
	e_upi_count,

	e_del_relation = 0,		// nod_delete
	e_del_boolean,
	e_del_plan,
	e_del_sort,
	e_del_rows,
	e_del_cursor,
	e_del_return,
	e_del_count,

	e_era_relation = 0,		// nod_erase
	e_era_rse,
	e_era_return,
	e_era_count,

	e_asgn_value = 0,       // nod_assign
	e_asgn_field,
	e_asgn_count,

	e_erc_context = 0,		// nod_erase_current
	e_erc_return,
	e_erc_count,

	e_mod_source = 0,		// nod_modify
	e_mod_update,
	e_mod_statement,
	e_mod_rse,
	e_mod_return,
	e_mod_count,

	e_mdc_context = 0,		// nod_modify_current
	e_mdc_update,
	e_mdc_statement,
	e_mdc_return,
	e_mdc_count,

	e_upd_relation = 0,		// nod_update
	e_upd_statement,
	e_upd_boolean,
	e_upd_plan,
	e_upd_sort,
	e_upd_rows,
	e_upd_cursor,
	e_upd_return,
	e_upd_rse_flags,
	e_upd_count,

	e_blb_field = 0,		// nod_get_segment & nod_put_segment
	e_blb_relation,
	e_blb_filter,
	e_blb_max_seg,
	e_blb_count,

	e_idx_unique = 0,		// nod_def_index
	e_idx_asc_dsc,
	e_idx_name,
	e_idx_table,
	e_idx_fields,
	e_idx_count,

	e_while_cond = 0,		// nod_while
	e_while_action,
	e_while_label,
	e_while_count,

	e_drl_name = 0,			// relation support
	e_drl_elements,
	e_drl_ext_file,			// external file
	e_drl_count,

	e_dft_default = 0,		// nod_def_default
	e_dft_default_source,
	e_dft_count,

	e_dom_name = 0,			// nod_def_domain
	e_dom_default,
	e_dom_constraint,
	e_dom_collate,
	e_dom_count,

	e_dfl_field = 0,		// nod_def_field
	e_dfl_default,
	e_dfl_constraint,
	e_dfl_collate,
	e_dfl_domain,
	e_dfl_computed,
	e_dfl_identity,
	e_dfl_count,

	e_grant_privs = 0,		// nod_grant
	e_grant_table,
	e_grant_users,
	e_grant_grant,
	e_grant_grantor,
	e_grant_count,

	e_alias_value = 0,		// nod_alias
	e_alias_alias,
	e_alias_imp_join,
	e_alias_count,

	e_rct_name = 0,			// nod_rel_constraint
	e_rct_type,

	e_pri_columns = 0,		// nod_primary
	e_pri_index,
	e_pri_count,

	e_for_columns = 0,		// nod_foreign
	e_for_reftable,
	e_for_refcolumns,
	e_for_action,
	e_for_index,
	e_for_count,

	e_ref_upd = 0,			// nod_ref_upd_del_action
	e_ref_del,
	e_ref_upd_del_count,

	e_ref_trig_action_count = 0,	// nod_ref_trig_action

	e_cnstr_table = 0,		// nod_def_constraint
	e_cnstr_type,
	e_cnstr_condition,
	e_cnstr_actions,
	e_cnstr_source,
	e_cnstr_count,

	e_coll_target = 0,		// Not a DSQL_NOD   nod_collate
	e_coll_source,
	e_coll_count,

	e_order_field = 0,		// nod_order
	e_order_flag,
	e_order_nulls,
	e_order_count,

	e_lock_tables = 0,		//
	e_lock_mode,

	e_database_name = 0,	//
	e_database_initial_desc,
	e_database_rem_desc,
	e_cdb_count,

	e_commit_retain = 0,	//
	e_commit_count,

	e_rollback_retain = 0,	//
	e_rollback_count,

	e_adb_all = 0,			//
	e_adb_count,

	e_filter_name = 0,		//
	e_filter_in_type,
	e_filter_out_type,
	e_filter_entry_pt,
	e_filter_module,
	e_filter_count,

	e_gen_id_name = 0,		// Not a DSQL_NOD   nod_set_generator
	e_gen_id_value,
	e_gen_id_count,


	e_udf_name = 0,			//
	e_udf_entry_pt,
	e_udf_module,
	e_udf_args,
	e_udf_return_value,
	e_udf_count,

	// computed field

	e_cmp_expr = 0,
	e_cmp_text,

	// create shadow

	e_shadow_number = 0,
	e_shadow_man_auto,
	e_shadow_conditional,
	e_shadow_name,
	e_shadow_length,
	e_shadow_sec_files,
	e_shadow_count,

	// alter index

	e_alt_index = 0,
	e_mod_idx_count,

	e_alt_idx_name = 0,
	e_alt_idx_name_count,

	// set statistics

	e_stat_name = 0,
	e_stat_count,

	e_mod_fld_name_orig_name = 0,	// nod_mod_field_name
	e_mod_fld_name_new_name,
	e_mod_fld_name_count,

	e_mod_fld_type_field = 0,				// nod_mod_field_type
	e_mod_fld_type_dom_name,
	e_mod_fld_type_default,
	e_mod_fld_type_computed,
	e_mod_fld_type_count,

	e_mod_fld_pos_orig_name = 0,	// nod_mod_field_position
	e_mod_fld_pos_new_position,
	e_mod_fld_pos_count,

	e_udf_param_field = 0,
	e_udf_param_type,		// Basically, by_reference or by_descriptor
	e_udf_param_count,

	// CASE {WHEN <search_condition> THEN <when_result>}.. [ELSE <else_result>] END
	// Node-constants for after pass1

	e_searched_case_search_conditions = 0,	// list boolean expressions
	e_searched_case_results,				// list including else_result

	e_label_name = 0,
	e_label_number,
	e_label_count,

	e_mod_udf_name = 0,				// nod_mod_udf
	e_mod_udf_entry_pt,
	e_mod_udf_module,
	e_mod_udf_count,

	e_ret_source = 0,				// nod_returning
	e_ret_target,
	e_ret_count,

	e_src_info_line = 0,			// nod_src_info
	e_src_info_column,
	e_src_info_stmt,
	e_src_info_count,

	e_mod_role_os_name = 0,			// nod_mod_role
	e_mod_role_db_name,
	e_mod_role_action,				// 0 - drop, 1 - add
	e_mod_role_count,

	e_del_user_name = 0,			// nod_del_user
	e_del_user_count,

	e_user_name = 0, 				// nod_add(mod)_user
	e_user_passwd,
	e_user_first,
	e_user_middle,
	e_user_last,
	e_user_admin,
	e_user_count,

	e_mod_fld_null_flag_field = 0,				// nod_mod_field_null_flag
	e_mod_fld_null_flag_value,
	e_mod_fld_null_flag_count
};

} // namespace

namespace Jrd {

typedef Dsql::nod_t NOD_TYPE;

// definition of a syntax node created both in parsing and in context recognition

class dsql_nod : public pool_alloc_rpt<class dsql_nod*, dsql_type_nod>
{
public:
	NOD_TYPE nod_type;			// Type of node
	DSC nod_desc;				// Descriptor
	USHORT nod_line;			// Source line of the statement.
	USHORT nod_column;			// Source column of the statement.
	USHORT nod_count;			// Number of arguments
	USHORT nod_flags;
	// In two adjacent elements (0 and 1) 64-bit value is placed sometimes
	RPT_ALIGN(dsql_nod* nod_arg[1]);

	dsql_nod() : nod_type(Dsql::nod_unknown_type), nod_count(0), nod_flags(0) {}
};

// values of flags
enum nod_flags_vals {
	NOD_UNION_ALL			= 1, // nod_list
	NOD_UNION_RECURSIVE 	= 2,
	NOD_SIMPLE_LIST			= 4,	// no need to enclose with blr_begin ... blr_end

	NOD_READ_ONLY			= 1, // nod_access
	NOD_READ_WRITE			= 2,

	NOD_WAIT				= 1, // nod_wait
	NOD_NO_WAIT				= 2,

	NOD_VERSION				= 1, // nod_version
	NOD_NO_VERSION			= 2,

	NOD_CONCURRENCY			= 1, // nod_isolation
	NOD_CONSISTENCY			= 2,
	NOD_READ_COMMITTED		= 4,

	NOD_SHARED				= 1, // nod_lock_mode
	NOD_PROTECTED			= 2,
	NOD_READ				= 4,
	NOD_WRITE				= 8,

	NOD_NO_AUTO_UNDO        = 1, // nod_tra_misc
	NOD_IGNORE_LIMBO        = 2,
	NOD_RESTART_REQUESTS    = 4,

	NOD_NULLS_FIRST			= 1, // nod_order
	NOD_NULLS_LAST			= 2,

	REF_ACTION_CASCADE		= 1, // nod_ref_trig_action
	REF_ACTION_SET_DEFAULT	= 2,
	REF_ACTION_SET_NULL		= 4,
	REF_ACTION_NONE			= 8,
	// Node flag indicates that this node has a different type or result
	// depending on the SQL dialect.
	NOD_COMP_DIALECT		= 16, // nod_...2, see MAKE_desc

	NOD_SELECT_EXPR_SINGLETON				= 1,	// nod_select_expr
	NOD_SELECT_EXPR_VALUE					= 2,
	NOD_SELECT_EXPR_RECURSIVE				= 4,	// recursive member of recursive CTE
	NOD_SELECT_EXPR_VIEW_FIELDS				= 8,	// view's field list
	NOD_SELECT_EXPR_DERIVED					= 16,
	NOD_SELECT_EXPR_DT_IGNORE_COLUMN_CHECK	= 32,
	NOD_SELECT_EXPR_DT_CTE_USED				= 64,

	NOD_CURSOR_EXPLICIT		= 1, // nod_cursor
	NOD_CURSOR_FOR			= 2,
	NOD_CURSOR_ALL			= USHORT(~0)
};

} // namespace

#endif // DSQL_NODE_H
