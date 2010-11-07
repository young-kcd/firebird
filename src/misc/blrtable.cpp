/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		blrtable.cpp
 *	DESCRIPTION:	blr to internal conversion table generator
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
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <stdio.h>
#include "../jrd/ibase.h"
#include "../common/common.h"

#define NODE(type, name, keyword) type,
enum {
#include "../jrd/nod.h"
	nodeMAX
};

/* NOTE: length == 0 implies the node will not be created
         by jrd/par.cpp.
	 length == Actual length of the nod_args array.
	 count  == How many of the nods_args array represent nodes
	 internal == Name of the internal representing node.
	 internal2 == Name of the internal representing node in blr_version5.
	 blr      == Name of the BLR to make into this node
         type     == Type of expression this Node produces
	 sub_type == Expected type of parameter Nodes
*/


#define STRINGIZER(s) #s

#define PAIR(internal, blr, length, count, type, sub_type) \
	{ blr, #internal, 0, #length, #count, \
	 #type, #sub_type }

#define PAIR2(internal, blr, length, count, type, sub_type) \
	{ blr, #internal, STRINGIZER(internal##2), #length, #count, \
	 #type, #sub_type }

static void print(const SCHAR **, int, const SCHAR *);

struct VERB
{
	UCHAR blr;
	const SCHAR* internal;
	const SCHAR* internal2;
	const SCHAR* length;
	const SCHAR* count;
	const SCHAR* type;
	const SCHAR* sub_type;
};

static const VERB verbs[] =
{
	PAIR(nod_assignment, blr_assignment, e_asgn_length, 2, STATEMENT, VALUE),
	PAIR(nod_erase, blr_erase, e_erase_length, 0, STATEMENT, OTHER),
	PAIR(nod_dcl_variable, blr_dcl_variable, e_dcl_length, 0, STATEMENT, OTHER),
	PAIR(nod_class_stmtnode_jrd, blr_fetch, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_class_stmtnode_jrd, blr_for, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_handler, blr_handler, 1, 1, STATEMENT, OTHER),
	PAIR(nod_class_stmtnode_jrd, blr_if, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_label, blr_label, e_lbl_length, 1, STATEMENT, STATEMENT),
	PAIR(nod_leave, blr_leave, 1, 0, STATEMENT, OTHER),
	PAIR(nod_list, blr_begin, 0, 0, STATEMENT, STATEMENT),
	PAIR(nod_loop, blr_loop, 1, 1, STATEMENT, STATEMENT),
	PAIR(nod_message, blr_message, 0, 0, STATEMENT, OTHER),
	PAIR(nod_modify, blr_modify, 0, 0, STATEMENT, STATEMENT),
	PAIR(nod_modify, blr_modify2, 0, 0, STATEMENT, STATEMENT),
	PAIR(nod_class_stmtnode_jrd, blr_user_savepoint, 1, 0, STATEMENT, OTHER),
	PAIR(nod_receive, blr_receive, e_send_length, 1, STATEMENT, STATEMENT),
	PAIR(nod_select, blr_select, 0, 0, STATEMENT, STATEMENT),
	PAIR(nod_class_stmtnode_jrd, blr_send, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_store, blr_store, e_sto_length, e_sto_length - 1, STATEMENT, STATEMENT),
	PAIR(nod_store, blr_store2, e_sto_length, e_sto_length - 1, STATEMENT, STATEMENT),
	PAIR(nod_class_stmtnode_jrd, blr_post, 1, 0, STATEMENT, VALUE),
	PAIR(nod_class_stmtnode_jrd, blr_post_arg, 1, 0, STATEMENT, VALUE),
	PAIR(nod_exec_sql, blr_exec_sql, 1, 1, STATEMENT, VALUE),
	PAIR(nod_exec_into, blr_exec_into, 0, 0, STATEMENT, OTHER),
	PAIR(nod_exec_stmt, blr_exec_stmt, 0, 0, STATEMENT, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_internal_info, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_add, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_count, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_count2, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_count_distinct, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_max, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_min, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_total, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_total_distinct, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_average, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_average_distinct, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_list, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_agg_list_distinct, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_parameter, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_parameter2, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_parameter3, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_variable, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_user_name, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_average, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_concatenate, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_count, 1, 0, VALUE, VALUE),
	PAIR(nod_dbkey, blr_dbkey, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_divide, 1, 0, VALUE, VALUE),
	PAIR(nod_field, blr_fid, 0, 0, VALUE, VALUE),
	PAIR(nod_field, blr_field, 0, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_via, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_from, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_function, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_literal, 1, 0, VALUE, OTHER),
	PAIR(nod_scalar, blr_index, 2, 2, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_maximum, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_minimum, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_null, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_multiply, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_negate, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_gen_id, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_upcase, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_substring, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_subtract, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_total, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_value_if, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_equiv, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_eql, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_neq, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_geq, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_gtr, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_lss, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_leq, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_matching, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_matching2, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_like, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_ansi_like, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_containing, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_missing, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_between, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_starting, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_unique, 1, 0, TYPE_BOOL, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_any, 1, 0, TYPE_BOOL, TYPE_RSE),
	PAIR(nod_class_exprnode_jrd, blr_and, 1, 0, TYPE_BOOL, TYPE_BOOL),
	PAIR(nod_class_exprnode_jrd, blr_or, 1, 0, TYPE_BOOL, TYPE_BOOL),
	PAIR(nod_class_exprnode_jrd, blr_not, 1, 0, TYPE_BOOL, TYPE_BOOL),

	PAIR(nod_class_recsrcnode_jrd, blr_rse, 0, 0, TYPE_RSE, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_union, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_recurse, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_aggregate, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_relation, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_rid, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_rs_stream, 0, 0, RELATION, OTHER),
	PAIR(nod_exec_proc, blr_exec_proc, e_esp_length, 4, STATEMENT, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_procedure, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_procedure2, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_procedure3, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_procedure4, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_pid, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_pid2, 1, 0, RELATION, OTHER),
	PAIR(nod_exec_proc, blr_exec_pid, e_esp_length, 4, STATEMENT, OTHER),
	PAIR(nod_block, blr_block, e_blk_length, e_blk_length, STATEMENT, STATEMENT),
	PAIR(nod_error_handler, blr_error_handler, e_err_length, 1, STATEMENT, OTHER),
	PAIR(nod_class_stmtnode_jrd, blr_abort, 1, 0, STATEMENT, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_cast, 1, 0, VALUE, VALUE),
	PAIR(nod_class_recsrcnode_jrd, blr_singular, 0, 0, TYPE_RSE, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_scrollable, 0, 0, TYPE_RSE, OTHER),
	PAIR(nod_start_savepoint, blr_start_savepoint, 1, 0, STATEMENT, OTHER),
	PAIR(nod_end_savepoint, blr_end_savepoint, 1, 0, STATEMENT, OTHER),

	PAIR(nod_class_recsrcnode_jrd, blr_relation2, 1, 0, RELATION, OTHER),
	PAIR(nod_class_recsrcnode_jrd, blr_rid2, 1, 0, RELATION, OTHER),
	PAIR2(nod_set_generator, blr_set_generator, e_gen_length, 1, STATEMENT, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_ansi_any, 1, 0, TYPE_BOOL, TYPE_RSE),
	PAIR(nod_class_exprnode_jrd, blr_exists, 1, 0, TYPE_BOOL, TYPE_RSE),
	PAIR(nod_rec_version, blr_record_version, 1, 0, VALUE, VALUE),
	PAIR(nod_stall, blr_stall, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_class_exprnode_jrd, blr_ansi_all, 1, 0, TYPE_BOOL, TYPE_RSE),

	/* Improved Date Support */
	PAIR(nod_class_exprnode_jrd, blr_extract, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_current_date, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_current_time, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_current_time2, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_current_timestamp, 1, 0, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_current_timestamp2, 1, 0, VALUE, OTHER),

	PAIR(nod_class_exprnode_jrd, blr_current_role, 1, 0, VALUE, VALUE),
	PAIR(nod_dcl_cursor, blr_dcl_cursor, e_dcl_cur_length, 2, STATEMENT, OTHER),
	PAIR(nod_cursor_stmt, blr_cursor_stmt, e_cursor_stmt_length, 0, STATEMENT, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_lowcase, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_strlen, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_trim, 1, 0, VALUE, VALUE),
	PAIR(nod_init_variable, blr_init_variable, e_init_var_length, 0, STATEMENT, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_sys_function, 1, 0, VALUE, VALUE),
	PAIR(nod_class_stmtnode_jrd, blr_auto_trans, 1, 0, STATEMENT, STATEMENT),
	PAIR(nod_class_exprnode_jrd, blr_similar, 1, 0, TYPE_BOOL, VALUE),
	PAIR(nod_stmt_expr, blr_stmt_expr, e_stmt_expr_length, 2, VALUE, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_derived_expr, 1, 0, VALUE, VALUE),
	PAIR(nod_class_recsrcnode_jrd, blr_window, 1, 0, RELATION, OTHER),
	PAIR(nod_continue_loop, blr_continue_loop, 1, 0, STATEMENT, OTHER),
	PAIR(nod_class_exprnode_jrd, blr_agg_function, 1, 0, VALUE, VALUE),
	PAIR(nod_class_exprnode_jrd, blr_substring_similar, 1, 0, VALUE, VALUE),
	{0, NULL, NULL, NULL, NULL, NULL, NULL}
};

const SCHAR *table[256], *table2[256], *lengths[256], *counts[256],
	*types[256], *sub_types[256];



int main(int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Spit out a conversion table.
 *
 **************************************/
	for (int blr = 0; blr < FB_NELEM(table); blr++)
	{
		table[blr] = NULL;
		table2[blr] = NULL;
		lengths[blr] = NULL;
		counts[blr] = NULL;
		counts[blr] = NULL;
		types[blr] = NULL;
		sub_types[blr] = NULL;
	}

	int max = 0;
	for (const VERB* verb = verbs; verb->internal; ++verb)
	{
		const int blr = verb->blr;
		if (table[blr])
		{
			fprintf(stderr, "BLRTABLE: duplicate blr %d\n", blr);
			exit(1);
		}
		table[blr] = verb->internal;
		table2[blr] = (verb->internal2 == 0) ? verb->internal : verb->internal2;

		lengths[blr] = verb->length;
		counts[blr] = verb->count;
		types[blr] = verb->type;
		sub_types[blr] = verb->sub_type;
		if (blr > max)
			max = blr;
	}

	printf("static const UCHAR blr_table4 [] = {\n");
	print(table, max, "(UCHAR) ");

	printf("static const UCHAR blr_table [] = {\n");
	print(table2, max, "(UCHAR) ");

	printf("static const SCHAR length_table [] = {\n");
	print(lengths, max, "");

	printf("static const SCHAR count_table [] = {\n");
	print(counts, max, "");

	printf("static const SCHAR type_table [] = {\n");
	print(types, max, "");

	printf("static const SCHAR sub_type_table [] = {\n");
	print(sub_types, max, "");

	return 0;
}


static void print(const SCHAR** tableL, int max, const SCHAR* fudge)
{
/**************************************
 *
 *	p r i n t
 *
 **************************************
 *
 * Functional description
 *	Spit out a conversion table.
 *
 **************************************/
	SCHAR buffer[100];
	char* s = buffer;

	for (int blr = 0; blr <= max; blr++, tableL++)
	{
		if (*tableL)
			sprintf(s, "%s%s, ", fudge, *tableL);
		else
			sprintf(s, " 0, ");
		while (*s)
			s++;
		if (s > buffer + 50)
		{
			printf("\t%s\n/*%3d*/", buffer, blr + 1);
			s = buffer;
			*s = 0;
		}
	}

	printf("\t%s 0};\n", buffer);
}
