/*
 *	PROGRAM:	C preprocessor
 *	MODULE:		blr.h
 *	DESCRIPTION:	BLR constants
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
 * Claudio Valderrama: 2001.6.18: Add blr_current_role.
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL
 */

#ifndef JRD_BLR_H
#define JRD_BLR_H

/*  WARNING: if you add a new BLR representing a data type, and the value
 *           is greater than the numerically greatest value which now
 *           represents a data type, you must change the define for
 *           DTYPE_BLR_MAX in jrd/align.h, and add the necessary entries
 *           to all the arrays in that file.
 */

#define blr_text		(unsigned char)14
#define blr_text2		(unsigned char)15	/* added in 3.2 JPN */
#define blr_short		(unsigned char)7
#define blr_long		(unsigned char)8
#define blr_quad		(unsigned char)9
#define blr_float		(unsigned char)10
#define blr_double		(unsigned char)27
#define blr_d_float		(unsigned char)11
#define blr_timestamp		(unsigned char)35
#define blr_varying		(unsigned char)37
#define blr_varying2		(unsigned char)38	/* added in 3.2 JPN */
#define blr_blob		(unsigned short)261
#define blr_cstring		(unsigned char)40     	
#define blr_cstring2    	(unsigned char)41	/* added in 3.2 JPN */
#define blr_blob_id     	(unsigned char)45	/* added from gds.h */
#define blr_sql_date		(unsigned char)12
#define blr_sql_time		(unsigned char)13
#define blr_int64               (unsigned char)16

/* Historical alias for pre V6 applications */
#define blr_date		blr_timestamp

#define blr_inner		(unsigned char)0
#define blr_left		(unsigned char)1
#define blr_right		(unsigned char)2
#define blr_full		(unsigned char)3

#define blr_gds_code		(unsigned char)0
#define blr_sql_code		(unsigned char)1
#define blr_exception		(unsigned char)2
#define blr_trigger_code 	(unsigned char)3
#define blr_default_code 	(unsigned char)4
#define blr_raise			(unsigned char)5
#define blr_exception_msg	(unsigned char)6

#define blr_version4		(unsigned char)4
#define blr_version5		(unsigned char)5
#define blr_eoc			(unsigned char)76
#define blr_end			(unsigned char)255	/* note: defined as -1 in gds.h */

#define blr_assignment		(unsigned char)1
#define blr_begin		(unsigned char)2
#define blr_dcl_variable  	(unsigned char)3	/* added from gds.h */
#define blr_message		(unsigned char)4
#define blr_erase		(unsigned char)5
#define blr_fetch		(unsigned char)6
#define blr_for			(unsigned char)7
#define blr_if			(unsigned char)8
#define blr_loop		(unsigned char)9
#define blr_modify		(unsigned char)10
#define blr_handler		(unsigned char)11
#define blr_receive		(unsigned char)12
#define blr_select		(unsigned char)13
#define blr_send		(unsigned char)14
#define blr_store		(unsigned char)15
#define blr_label		(unsigned char)17
#define blr_leave		(unsigned char)18
#define blr_store2		(unsigned char)19
#define blr_post		(unsigned char)20
#define blr_literal		(unsigned char)21
#define blr_dbkey		(unsigned char)22
#define blr_field		(unsigned char)23
#define blr_fid			(unsigned char)24
#define blr_parameter		(unsigned char)25
#define blr_variable		(unsigned char)26
#define blr_average		(unsigned char)27
#define blr_count		(unsigned char)28
#define blr_maximum		(unsigned char)29
#define blr_minimum		(unsigned char)30
#define blr_total		(unsigned char)31
/* count 2
#define blr_count2		32
*/
#define blr_add			(unsigned char)34
#define blr_subtract		(unsigned char)35
#define blr_multiply		(unsigned char)36
#define blr_divide		(unsigned char)37
#define blr_negate		(unsigned char)38
#define blr_concatenate		(unsigned char)39
#define blr_substring		(unsigned char)40
#define blr_parameter2		(unsigned char)41
#define blr_from		(unsigned char)42
#define blr_via			(unsigned char)43
#define blr_parameter2_old	(unsigned char)44	/* Confusion */
#define blr_user_name   	(unsigned char)44	/* added from gds.h */
#define blr_null		(unsigned char)45

#define blr_equiv			(unsigned char)46
#define blr_eql			(unsigned char)47
#define blr_neq			(unsigned char)48
#define blr_gtr			(unsigned char)49
#define blr_geq			(unsigned char)50
#define blr_lss			(unsigned char)51
#define blr_leq			(unsigned char)52
#define blr_containing		(unsigned char)53
#define blr_matching		(unsigned char)54
#define blr_starting		(unsigned char)55
#define blr_between		(unsigned char)56
#define blr_or			(unsigned char)57
#define blr_and			(unsigned char)58
#define blr_not			(unsigned char)59
#define blr_any			(unsigned char)60
#define blr_missing		(unsigned char)61
#define blr_unique		(unsigned char)62
#define blr_like		(unsigned char)63

#define blr_stream      	(unsigned char)65	/* added from gds.h */
#define blr_set_index   	(unsigned char)66	/* added from gds.h */

#define blr_rse			(unsigned char)67
#define blr_first		(unsigned char)68
#define blr_project		(unsigned char)69
#define blr_sort		(unsigned char)70
#define blr_boolean		(unsigned char)71
#define blr_ascending		(unsigned char)72
#define blr_descending		(unsigned char)73
#define blr_relation		(unsigned char)74
#define blr_rid			(unsigned char)75
#define blr_union		(unsigned char)76
#define blr_map			(unsigned char)77
#define blr_group_by		(unsigned char)78
#define blr_aggregate		(unsigned char)79
#define blr_join_type		(unsigned char)80

#define blr_agg_count		(unsigned char)83
#define blr_agg_max		(unsigned char)84
#define blr_agg_min		(unsigned char)85
#define blr_agg_total		(unsigned char)86
#define blr_agg_average		(unsigned char)87
#define	blr_parameter3		(unsigned char)88	/* same as Rdb definition */
#define blr_run_max		(unsigned char)89
#define blr_run_min		(unsigned char)90
#define blr_run_total		(unsigned char)91
#define blr_run_average		(unsigned char)92
#define blr_agg_count2		(unsigned char)93
#define blr_agg_count_distinct	(unsigned char)94
#define blr_agg_total_distinct	(unsigned char)95
#define blr_agg_average_distinct (unsigned char)96

#define blr_function		(unsigned char)100
#define blr_gen_id		(unsigned char)101
#define blr_prot_mask		(unsigned char)102
#define blr_upcase		(unsigned char)103
#define blr_lock_state		(unsigned char)104
#define blr_value_if		(unsigned char)105
#define blr_matching2		(unsigned char)106
#define blr_index		(unsigned char)107
#define blr_ansi_like		(unsigned char)108
#define blr_bookmark		(unsigned char)109
#define blr_crack		(unsigned char)110
#define blr_force_crack		(unsigned char)111
#define blr_seek		(unsigned char)112
#define blr_find		(unsigned char)113

/* these indicate directions for blr_seek and blr_find */

#define blr_continue		(unsigned char)0
#define blr_forward		(unsigned char)1
#define blr_backward		(unsigned char)2
#define blr_bof_forward		(unsigned char)3
#define blr_eof_backward	(unsigned char)4

#define blr_lock_relation 	(unsigned char)114
#define blr_lock_record		(unsigned char)115
#define blr_set_bookmark 	(unsigned char)116
#define blr_get_bookmark 	(unsigned char)117

#define blr_run_count		(unsigned char)118	/* changed from 88 to avoid conflict with blr_parameter3 */
#define blr_rs_stream		(unsigned char)119
#define blr_exec_proc		(unsigned char)120
#define blr_begin_range 	(unsigned char)121
#define blr_end_range 		(unsigned char)122
#define blr_delete_range 	(unsigned char)123
#define blr_procedure		(unsigned char)124
#define blr_pid			(unsigned char)125
#define blr_exec_pid		(unsigned char)126
#define blr_singular		(unsigned char)127
#define blr_abort		(unsigned char)128
#define blr_block	 	(unsigned char)129
#define blr_error_handler	(unsigned char)130

#define blr_cast		(unsigned char)131
#define blr_release_lock	(unsigned char)132
#define blr_release_locks	(unsigned char)133
#define blr_start_savepoint	(unsigned char)134
#define blr_end_savepoint	(unsigned char)135
#define blr_find_dbkey		(unsigned char)136
#define blr_range_relation	(unsigned char)137
#define blr_delete_ranges	(unsigned char)138

#define blr_plan		(unsigned char)139	/* access plan items */
#define blr_merge		(unsigned char)140
#define blr_join		(unsigned char)141
#define blr_sequential		(unsigned char)142
#define blr_navigational	(unsigned char)143
#define blr_indices		(unsigned char)144
#define blr_retrieve		(unsigned char)145

#define blr_relation2		(unsigned char)146
#define blr_rid2		(unsigned char)147
#define blr_reset_stream	(unsigned char)148
#define blr_release_bookmark	(unsigned char)149

#define blr_set_generator       (unsigned char)150

#define blr_ansi_any		(unsigned char)151   /* required for NULL handling */
#define blr_exists		(unsigned char)152   /* required for NULL handling */
#define blr_cardinality		(unsigned char)153

#define blr_record_version	(unsigned char)154	/* get tid of record */
#define blr_stall		(unsigned char)155	/* fake server stall */

#define blr_seek_no_warn	(unsigned char)156	
#define blr_find_dbkey_version	(unsigned char)157   /* find dbkey with record version */
#define blr_ansi_all		(unsigned char)158   /* required for NULL handling */

#define blr_extract		(unsigned char)159

/* sub parameters for blr_extract */

#define blr_extract_year	(unsigned char)0
#define blr_extract_month	(unsigned char)1
#define blr_extract_day		(unsigned char)2
#define blr_extract_hour	(unsigned char)3
#define blr_extract_minute	(unsigned char)4
#define blr_extract_second	(unsigned char)5
#define blr_extract_weekday	(unsigned char)6
#define blr_extract_yearday	(unsigned char)7

#define blr_current_date	(unsigned char)160
#define blr_current_timestamp	(unsigned char)161
#define blr_current_time	(unsigned char)162

/* FB 1.0 specific BLR */

#define blr_current_role	(unsigned char)174
#define blr_skip		(unsigned char)175

/* FB 1.5 specific BLR */

#define blr_exec_sql		(unsigned char)176
#define blr_internal_info	(unsigned char)177
#define blr_nullsfirst		(unsigned char)178
#define blr_writelock		(unsigned char)179
#define blr_nullslast       (unsigned char)180

/* These codes reuse BLR code space */

#define blr_post_arg		(unsigned char)163
#define blr_exec_into		(unsigned char)164
#define blr_user_savepoint	(unsigned char)165
#define blr_dcl_cursor		(unsigned char)166
#define blr_cursor_stmt		(unsigned char)167

/* These codes are actions for user-defined savepoints */

#define blr_savepoint_set	(unsigned char)0
#define blr_savepoint_release	(unsigned char)1
#define blr_savepoint_undo	(unsigned char)2
#define blr_savepoint_release_single	(unsigned char)3

/* These codes are actions for cursors */

#define blr_cursor_open			(unsigned char)0
#define blr_cursor_close		(unsigned char)1
#define blr_cursor_fetch		(unsigned char)2

#endif /* JRD_BLR_H */
