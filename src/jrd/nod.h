/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		nod.h
 *	DESCRIPTION:	Runtime node types
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
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * Adriano dos Santos Fernandes
 */

NODE(nod_asn_list, asn_list, "")
	NODE(nod_assignment, assignment, "")
	NODE(nod_dcl_variable, declare, "")
	NODE(nod_erase, erase, "")
	NODE(nod_for, for, "")
	NODE(nod_handler, handler, "")
		NODE(nod_label, label, "")
			NODE(nod_leave, leave, "")
			NODE(nod_list, list, "")
			NODE(nod_loop, loop, "")
			NODE(nod_message, message, "")
			NODE(nod_modify, modify, "")
			NODE(nod_receive, receive, "")
			NODE(nod_select, select, "")
			NODE(nod_store, store, "")

			NODE(nod_argument, argument, "")
			NODE(nod_variable, variable, "")
			NODE(nod_average, average, "AVG")
			NODE(nod_count, count, "COUNT")
			NODE(nod_dbkey, dbkey, "ROWID")
			NODE(nod_field, field, "")
			NODE(nod_from, from, "")
			NODE(nod_literal, literal, "")
			NODE(nod_max, max, "MAX")
			NODE(nod_min, min, "MIN")
			NODE(nod_scalar, scalar, "")
			NODE(nod_gen_id, recnum, "")
			NODE(nod_prot_mask, prot_mask, "")
			NODE(nod_upcase, upcase, "UPPER")
			NODE(nod_lock_state, lock_state, "")
			NODE(nod_null, null, "NULL")
			NODE(nod_substr, substr, "")
			NODE(nod_total, total, "SUM")
			NODE(nod_trim, trim, "")
			NODE(nod_validate, validate, "")

		  NODE(nod_exec_proc, exec_proc, "")
		  NODE(nod_block, block, "")
		  NODE(nod_error_handler, error_handler, "")
		  NODE(nod_cast, cast, "CAST")
		  NODE(nod_start_savepoint, start_savepoint, "")
		  NODE(nod_end_savepoint, end_savepoint, "")

		  NODE(nod_set_generator, set_generator, "")

/* Required for NULL handling             */

		  NODE(nod_rec_version, record_version, "RECORD VERSION")
		  NODE(nod_stall, stall, "SERVER STALL")

/* Improved date handling */
		  NODE(nod_extract, extract, "EXTRACT")

		  NODE(nod_average2, average2, "AVG")
		  NODE(nod_gen_id2, recnum, "")
		  NODE(nod_set_generator2, set_generator, "")
		  NODE(nod_total2, total, "SUM")

/* EXECUTE STATEMENT */
		  NODE(nod_exec_sql, exec_sql, "EXECUTE STATEMENT")

/* EXECUTE VARCHAR INTO ...*/
		  NODE(nod_exec_into, exec_sql_into, "EXECUTE VARCHAR INTO")
		  NODE(nod_exec_stmt, exec_stmt, "EXECUTE STATEMENT")

		  NODE(nod_dcl_cursor, declare_cursor, "DECLARE CURSOR")
		  NODE(nod_cursor_stmt, cursor_stmt, "CURSOR STATEMENT")
		  NODE(nod_lowcase, lowcase, "LOWER")

		  NODE(nod_strlen, strlen, "STRLEN")

		  NODE(nod_src_info, source_info, "")
		  NODE(nod_init_variable, init_variable, "")
		  NODE(nod_domain_validation, domain_validation, "")
		  NODE(nod_class_exprnode_jrd, class_exprnode_jrd, "class_exprnode_jrd")
		  NODE(nod_class_stmtnode_jrd, class_stmtnode_jrd, "class_stmtnode_jrd")
		  NODE(nod_class_recsrcnode_jrd, class_recsrcnode_jrd, "class_recsrcnode_jrd")
		  NODE(nod_stmt_expr, stmt_expr, "stmt_expr")
  		  NODE(nod_derived_expr, derived_expr, "derived_expr")
		  NODE(nod_continue_loop, continue_loop, "")
