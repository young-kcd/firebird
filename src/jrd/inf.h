/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		inf.h
 *	DESCRIPTION:	Information codes definitions
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
 * 2001.07.28: John Bellardo: Added gds__info_rsb_skip to support LIMIT.
 */

#ifndef _JRD_INF_H_
#define _JRD_INF_H_

/* Common, structural codes */

#define gds__info_end			1
#define gds__info_truncated		2
#define gds__info_error			3

/* Database information items */

#define gds__info_db_id			4
#define gds__info_reads			5
#define gds__info_writes		6
#define gds__info_fetches		7
#define gds__info_marks			8
#define gds__info_implementation 	11
#define gds__info_version		12
#define gds__info_base_level		13
#define gds__info_page_size		14
#define gds__info_num_buffers		15
#define gds__info_limbo			16
#define gds__info_current_memory	17
#define gds__info_max_memory		18
#define gds__info_window_turns		19
#define gds__info_license		20
#define gds__info_allocation		21
#define gds__info_attachment_id	 	22
#define gds__info_read_seq_count	23
#define gds__info_read_idx_count	24
#define gds__info_insert_count		25
#define gds__info_update_count		26
#define gds__info_delete_count		27
#define gds__info_backout_count	 	28
#define gds__info_purge_count		29
#define gds__info_expunge_count		30
#define gds__info_sweep_interval	31
#define gds__info_ods_version		32
#define gds__info_ods_minor_version	33
#define gds__info_no_reserve		34
#define gds__info_logfile		35
#define gds__info_cur_logfile_name	36
#define gds__info_cur_log_part_offset	37
#define gds__info_num_wal_buffers	38
#define gds__info_wal_buffer_size	39
#define gds__info_wal_ckpt_length	40
#define gds__info_wal_cur_ckpt_interval 41
#define gds__info_wal_prv_ckpt_fname	42
#define gds__info_wal_prv_ckpt_poffset	43
#define gds__info_wal_recv_ckpt_fname	44
#define gds__info_wal_recv_ckpt_poffset 45
#define gds__info_wal_grpc_wait_usecs	47
#define gds__info_wal_num_io		48
#define gds__info_wal_avg_io_size	49
#define gds__info_wal_num_commits	50
#define gds__info_wal_avg_grpc_size	51
#define gds__info_forced_writes		52


/* Request information items */

#define gds__info_number_messages	4
#define gds__info_max_message		5
#define gds__info_max_send		6
#define gds__info_max_receive		7
#define gds__info_state			8
#define gds__info_message_number	9
#define gds__info_message_size		10
#define gds__info_request_cost		11
#define gds__info_access_path		12
#define gds__info_req_select_count	13
#define gds__info_req_insert_count	14
#define gds__info_req_update_count	15
#define gds__info_req_delete_count	16

#define gds__info_req_active		2
#define gds__info_req_inactive		3
#define gds__info_req_send		4
#define gds__info_req_receive		5
#define gds__info_req_select		6

/* access path items */

#define gds__info_rsb_end		0
#define gds__info_rsb_begin		1
#define gds__info_rsb_type		2
#define gds__info_rsb_relation		3

/* rsb types */

#define gds__info_rsb_unknown		1
#define gds__info_rsb_indexed		2
#define gds__info_rsb_navigate		3
#define gds__info_rsb_sequential	4
#define gds__info_rsb_cross		5
#define gds__info_rsb_sort		6
#define gds__info_rsb_first		7
#define gds__info_rsb_boolean		8
#define gds__info_rsb_union		9
#define gds__info_rsb_aggregate		10
#define gds__info_rsb_merge		11
#define gds__info_rsb_ext_sequential	12
#define gds__info_rsb_ext_indexed	13
#define gds__info_rsb_ext_dbkey		14
#define gds__info_rsb_left_cross	15
#define gds__info_rsb_select		16
#define gds__info_rsb_sql_join		17
#define gds__info_rsb_simulate		18
#define gds__info_rsb_sim_cross		19
#define gds__info_rsb_once		20
#define gds__info_rsb_procedure		21
#define gds__info_rsb_skip		22

/* bitmap expressions */

#define gds__info_rsb_and		1
#define gds__info_rsb_or		2
#define gds__info_rsb_dbkey		3
#define gds__info_rsb_index		4


/* Blob information items */

#define gds__info_blob_num_segments	4
#define gds__info_blob_max_segment	5
#define gds__info_blob_total_length	6
#define gds__info_blob_type		7


/* Transaction information items */

#define gds__info_tra_id		4


/* Service information items */

#define gds__info_svc_version		4
#define gds__info_svc_message		5
#define gds__info_svc_total_length	6
#define gds__info_svc_response		7
#define gds__info_svc_response_more	8
#define gds__info_svc_line		9
#define gds__info_svc_to_eof		10
#define gds__info_svc_timeout		11


/* SQL information items */

#define gds__info_sql_select		4
#define gds__info_sql_bind		5
#define gds__info_sql_num_variables	6
#define gds__info_sql_describe_vars	7
#define gds__info_sql_describe_end	8
#define gds__info_sql_sqlda_seq		9
#define gds__info_sql_message_seq	10
#define gds__info_sql_type		11
#define gds__info_sql_sub_type		12
#define gds__info_sql_scale		13
#define gds__info_sql_length		14
#define gds__info_sql_null_ind		15
#define gds__info_sql_field		16
#define gds__info_sql_relation		17
#define gds__info_sql_owner		18
#define gds__info_sql_alias		19
#define gds__info_sql_sqlda_start	20
#define gds__info_sql_stmt_type		21

#endif /* _JRD_INF_H_ */
