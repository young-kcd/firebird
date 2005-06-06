/*
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
 * 2001.07.28: John Bellardo: Added skip and current_role to table.
 */
static const struct
{
	const char* blr_string;
	const UCHAR* blr_operators;
} blr_table[] =
{
	{NULL, NULL},
	{"assignment", two},
	{"begin", begin},
	{"declare", declare},
	{"message", message},
	{"erase", byte},
	{"fetch", two}, {"for", two}, {"if", three}, {"loop", one}, {"modify", byte_byte_verb},	/* 10 */
 	{"handler", one},
	{"receive", byte_verb},
	{"select", begin},
    {"send", byte_verb},
	{"store", two},
	{NULL, NULL}, {"label", byte_verb}, {"leave", byte}, {"store2", three}, {"post", one},	/* 20 */
	{"literal", literal},
	{"dbkey", byte},
	{"field", field},
	{"fid", parm},
	{"parameter", parm},
	{"variable", variable},
	{"average", two}, {"count", one}, {"maximum", two}, {"minimum", two},	/* 30 */
	{"total", two},
	{"count2", two},
	{NULL, NULL},
	{"add", two},
	{"subtract", two},
	{"multiply", two},
	{"divide", two}, {"negate", one}, {"concatenate", two}, {"substring", three},	/* 40 */
	{"parameter2", parm2},
	{"from", two},
	{"via", three},
	{"user_name", zero},
	{"null", zero}, {NULL, NULL}, {"eql", two}, {"neq", two}, {"gtr", two}, {"geq", two},	/* 50 */
	{"lss", two},
	{"leq", two},
	{"containing", two},
	{"matching", two},
	{"starting", two},
	{"between", three}, {"or", two}, {"and", two}, {"not", one}, {"any", one},	/* 60 */
	{"missing", one},
	{"unique", one},
	{"like", two},
	{NULL, NULL},
	{"stream", begin},
	{"set_index", byte_verb},
	{"rse", rse}, {"first", one}, {"project", byte_args}, {"sort", byte_args},	/* 70 */
	{"boolean", one},
	{"ascending", one},
	{"descending", one},
	{"relation", relation},
	{"rid", rid},
	{"union", union_ops},
	{"map", map}, {"group_by", byte_args}, {"aggregate", aggregate}, {"join_type", join},	/* 80 */
	{NULL, NULL},
	{NULL, NULL},
	{"agg_count", zero},
	{"agg_max", one},
	{"agg_min", one},
	{"agg_total", one},
	{"agg_average", one}, {"parameter3", parm3}, {NULL, NULL}, {NULL, NULL},	/* 90 */
	{NULL, NULL},
	{NULL, NULL},
	{"agg_count2", one},
	{"agg_count_distinct", one},
	{"agg_total_distinct", one},
	{"agg_average_distinct", one},
	{NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {"function", function},	/* 100 */
	{"gen_id", gen_id},
	{"prot_mask", two},
	{"upcase", one},
	{"lock_state", one},
	{"value_if", three},
	{"matching2", three},
	{"index", indx}, {"ansi_like", three}, {"bookmark", one}, {"crack", byte},	/* 110 */
	{"force_crack", byte},
	{"seek", seek},
	{"find", find},
	{"lock_relation", lock_relation},
	{"lock_record", byte_verb},
	{"set_bookmark", byte_verb},
	{"get_bookmark", byte}, {NULL, NULL}, {"rs_stream", rse}, {"exec_proc", exec_proc},	/* 120 */
	{"begin_range", one},
	{"end_range", two},
	{"delete_range", one},
	{"procedure", procedure},
	{"pid", pid},
	{NULL, NULL},
	{"singular", one}, {"abort", set_error}, {"block", begin}, {"error_handler", error_handler},	/* 130 */
	{"cast", cast},
	{"release_lock", one},
	{"release_locks", zero},
	{"start_savepoint", zero},
	{"end_savepoint", zero},
	{"find_dbkey", byte_verb},
	{"range_relation", range_relation},
	{"delete_ranges", zero}, {"plan", one}, {"merge", byte_args},	/* 140 */
	{"join", byte_args},
	{"sequential", zero},
	{"navigational", byte_literal},
	{"indices", indices},
	{"retrieve", two},
	{"relation2", relation2},
	{"rid2", rid2},
	{"reset_stream", byte_byte}, {"release_bookmark", one}, {"set_generator", gen_id},	/* 150  */
	{"ansi_any", one},
	{"exists", one},
	{"cardinality", byte},
	{"record_version", byte},
	{"stall", zero},
	{"seek_no_warn", seek},
	{"find_dbkey_version", byte_verb_verb}, {"ansi_all", one},
/* New BLR in 6.0 */
	{"extract", extract}, {"current_date", zero},	/* 160 */
	{"current_timestamp", zero}, {"current_time", zero},
	{"post_arg", two},
	{"exec_into", exec_into},
	{"user_savepoint", user_savepoint},
	{"dcl_cursor", dcl_cursor},
	{"cursor_stmt", cursor_stmt},
    /* These verbs were added in 6.0, primarily to support 64-bit integers, now obsolete */
	{"agg_total_distinct2", one}, {"agg_average2", one}, {"agg_average_distinct2", one},	/* 170 */
	{"average2", two}, 
	{"gen_id2", gen_id}, 
    {"set_generator2", gen_id},
/* New BLR in FB1 */
    {"current_role", zero},
    {"skip", one}, 
/* New BLR in FB2 */
	{"exec_sql", one},
	{"internal_info", one},
	{"nullsfirst", zero},
	{"writelock", zero},
	{"nullslast", zero},
	{"lowcase", one}, /* 181 */
	{"strlen", strlength},
	{"trim", trim},
	{0, 0}
};

