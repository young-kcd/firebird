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
 */

NODE(nod_asn_list, asn_list, "")
	NODE(nod_assignment, assignment, "")
	NODE(nod_dcl_variable, declare, "")
	NODE(nod_erase, erase, "")
	NODE(nod_user_savepoint, user_savepoint, "SAVEPOINT")	
	NODE(nod_fetch, fetch, "")
	NODE(nod_for, for, "")
	NODE(nod_handler, handler, "")
		NODE(nod_if, if, "")
		NODE(nod_label, label, "")
			NODE(nod_leave, leave, "")
			NODE(nod_list, list, "")
			NODE(nod_loop, loop, "")
			NODE(nod_message, message, "")
			NODE(nod_modify, modify, "")
			NODE(nod_receive, receive, "")
			NODE(nod_select, select, "")
			NODE(nod_send, send, "")
			NODE(nod_store, store, "")
			NODE(nod_post, post, "")

			NODE(nod_add, add, " + ")
			NODE(nod_argument, argument, "")
			NODE(nod_variable, variable, "")
			NODE(nod_average, average, "AVG")
			NODE(nod_concatenate, concatenate, " || ")
			NODE(nod_count, count, "COUNT")
			NODE(nod_count2, count2, "COUNT")
			NODE(nod_dbkey, dbkey, "ROWID")
			NODE(nod_divide, divide, " / ")
			NODE(nod_field, field, "")
			NODE(nod_from, from, "")
			NODE(nod_index, index, "")
			NODE(nod_bit_and, bit_and, "")
			NODE(nod_bit_or, bit_or, "")
			NODE(nod_bit_in, bit_in, "")
			NODE(nod_bit_dbkey, bit_dbkey, "")
			NODE(nod_literal, literal, "")
			NODE(nod_max, max, "MAX")
			NODE(nod_min, min, "MIN")
			NODE(nod_multiply, multiply, " * ")
			NODE(nod_negate, negate, " -")
			NODE(nod_function, function, "")
			NODE(nod_scalar, scalar, "")
			NODE(nod_user_name, UserId, "")
			NODE(nod_gen_id, recnum, "")
			NODE(nod_prot_mask, prot_mask, "")
			NODE(nod_upcase, upcase, "UPPER")
			NODE(nod_lock_state, lock_state, "")
			NODE(nod_null, null, "NULL")
			NODE(nod_substr, substr, "")
			NODE(nod_subtract, subtract, " - ")
			NODE(nod_total, total, "SUM")
			NODE(nod_trim, trim, "")
			NODE(nod_value_if, value_if, "")
			NODE(nod_eql, eql, " = ")
			NODE(nod_neq, neq, " != ")
			NODE(nod_geq, geq, " >= ")
			NODE(nod_gtr, gtr, " > ")
			NODE(nod_lss, lss, " < ")
			NODE(nod_leq, leq, " <= ")
			NODE(nod_matches, matches, "")
			NODE(nod_sleuth, sleuth, "")
			NODE(nod_like, like, " LIKE ")
			NODE(nod_contains, contains, "")
			NODE(nod_missing, missing, " IS NULL")
			NODE(nod_between, between, "")
			NODE(nod_starts, starts, "")
			NODE(nod_unique, unique, "")
			NODE(nod_any, any, "")
			NODE(nod_and, and, " AND ")
			NODE(nod_or, or, " OR ")
			NODE(nod_not, not, " NOT ")
			NODE(nod_validate, validate, "")

			NODE(nod_agg_count, agg_count, "")
			NODE(nod_agg_max, agg_max, "")
			NODE(nod_agg_max_indexed, agg_max_indexed, "")
			NODE(nod_agg_min, agg_min, "")
			NODE(nod_agg_min_indexed, agg_min_indexed, "")
			NODE(nod_agg_total, agg_total, "")
			NODE(nod_agg_average, agg_average, "")
			NODE(nod_agg_count2, agg_count2, "")
			NODE(nod_agg_count_distinct, agg_count_distinct, "")
			NODE(nod_agg_total_distinct, agg_total_distinct, "")
			NODE(nod_agg_average_distinct, agg_average_distinct, "")
			NODE(nod_asb, AggregateSort, "")
			NODE(nod_relation, relation, "")
			NODE(nod_rse, RecordSelExpr, "")
			NODE(nod_stream, stream, "")
			NODE(nod_set_index, set_index, "")
			NODE(nod_seek, seek, "")
			NODE(nod_seek_no_warn, seek_no_warn, "")
			NODE(nod_find, find, "")
			NODE(nod_bookmark, bookmark, "")
			NODE(nod_get_bookmark, get_bookmark, "")
			NODE(nod_set_bookmark, set_bookmark, "")
			NODE(nod_lock_relation, lock_relation, "")
			NODE(nod_lock_record, lock_record, "")
			NODE(nod_begin_range, begin_range, "")
			NODE(nod_end_range, end_range, "")
			NODE(nod_delete_range, delete_range, "")
			NODE(nod_sort, sort, "")
			NODE(nod_union, union, "")
		  NODE(nod_aggregate, aggregate, "")
		  NODE(nod_map, map, "")
		  NODE(nod_dependency, dependency, "")
		  NODE(nod_exec_proc, exec_proc, "")
		  NODE(nod_procedure, procedure, "")
		  NODE(nod_block, block, "")
		  NODE(nod_error_handler, error_handler, "")
		  NODE(nod_abort, abort, "")
		  NODE(nod_cast, cast, "CAST")
		  NODE(nod_nop, nop, "")
		  NODE(nod_release_lock, nod_release_lock, "")
		  NODE(nod_release_locks, nod_release_locks, "")
		  NODE(nod_start_savepoint, start_savepoint, "")
		  NODE(nod_end_savepoint, end_savepoint, "")
		  NODE(nod_find_dbkey, find_dbkey, "")
		  NODE(nod_find_dbkey_version, find_dbkey_version, "")
		  NODE(nod_range_relation, range_relation, "")
		  NODE(nod_delete_ranges, delete_ranges, "")
		  NODE(nod_crack, crack, "CRACK")
		  NODE(nod_force_crack, force_crack, "FORCE CRACK")

/* access plan stuff */
		  NODE(nod_plan, plan, "PLAN")
		  NODE(nod_merge, merge, "MERGE")
		  NODE(nod_join, join, "JOIN")
		  NODE(nod_sequential, sequential, "SEQUENTIAL")
		  NODE(nod_navigational, navigational, "NAVIGATIONAL")
		  NODE(nod_indices, indices, "INDICES")
		  NODE(nod_retrieve, retrieve, "RETRIEVE")

		  NODE(nod_relation2, relation2, "RELATION2")
		  NODE(nod_rid2, rid2, "RID2")
		  NODE(nod_reset_stream, reset_stream, "RESET STREAM")
		  NODE(nod_release_bookmark, release_bookmark, "RELEASE BOOKMARK")

		  NODE(nod_set_generator, set_generator, "")

/* Required for NULL handling             */
		  NODE(nod_ansi_any, ansi_any, "ANSI ANY")
		  NODE(nod_exists, exists, "EXISTS")
		  NODE(nod_cardinality, cardinality, "CARDINALITY")

		  NODE(nod_rec_version, record_version, "RECORD VERSION")
		  NODE(nod_stall, stall, "SERVER STALL")

		  NODE(nod_ansi_all, ansi_all, "ANSI ALL")

/* Improved date handling */
		  NODE(nod_extract, extract, "EXTRACT")
		  NODE(nod_current_date, current_date, "CURRENT DATE")
		  NODE(nod_current_time, current_time, "CURRENT TIME")
		  NODE(nod_current_timestamp, current_timestamp, "CURRENT TIMESTAMP")

/* Additions for 64-bit integer support */
		  NODE(nod_add2, add2, " + ")
		  NODE(nod_subtract2, subtract2, " - ")
		  NODE(nod_multiply2, multiply2, " * ")
		  NODE(nod_divide2, divide2, " / ")
		  NODE(nod_agg_total2, agg_total, "")
		  NODE(nod_agg_average2, agg_average2, "")
		  NODE(nod_agg_total_distinct2, agg_total_distinct2, "")
		  NODE(nod_agg_average_distinct2, agg_average_distinct2, "")
		  NODE(nod_average2, average2, "AVG")
		  NODE(nod_gen_id2, recnum, "")
		  NODE(nod_set_generator2, set_generator, "")
		  NODE(nod_total2, total, "SUM")

          NODE(nod_current_role, current_role, "")
          NODE(nod_breakleave, breakleave, "")

/* EXECUTE VARCHAR */
		  NODE(nod_exec_sql, exec_sql, "EXECUTE VARCHAR")

/* Internal engine info */
		  NODE(nod_internal_info, internal_info, "INTERNAL INFO")

/* EXECUTE VARCHAR INTO ...*/
		  NODE(nod_exec_into, exec_sql_into, "EXECUTE VARCHAR INTO")

		  NODE(nod_dcl_cursor, declare_cursor, "DECLARE CURSOR")
		  NODE(nod_cursor_stmt, cursor_stmt, "CURSOR STATEMENT")
		  NODE(nod_lowcase, lowcase, "LOWER")
		  NODE(nod_equiv, equiv, " == ")

		  NODE(nod_strlen, strlen, "STRLEN")
