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
 */


 







/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		relations.h
 *	DESCRIPTION:	System relation definitions
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


				 

				 

																												 

			 

													 

																			 

																 

				 

				 
			 

						 

					 

										 

					 

								 

										 

							 

			 

								 

				 

			 

				 

						 

					 

			 
						 

											 

							 

									 

							 

					 

		

#define f_pag_page 0  
#define f_pag_id 1  
#define f_pag_seq 2  
#define f_pag_type 3  
#define f_dat_desc 0  
#define f_dat_id 1  
#define f_dat_class 2  
#define f_dat_charset 3  
#define f_fld_name 0  
#define f_fld_qname 1  
#define f_fld_v_blr 2  
#define f_fld_v_source 3  
#define f_fld_computed 4  
#define f_fld_csource 5  
#define f_fld_default 6  
#define f_fld_dsource 7  
#define f_fld_length 8  
#define f_fld_scale 9  
#define f_fld_type 10  
#define f_fld_sub_type 11  
#define f_fld_missing 12  
#define f_fld_msource 13  
#define f_fld_desc 14  
#define f_fld_sys_flag 15  
#define f_fld_qheader 16  
#define f_fld_seg_len 17  
#define f_fld_estring 18  
#define f_fld_ext_length 19  
#define f_fld_ext_scale 20  
#define f_fld_ext_type 21  
#define f_fld_dimensions 22  
#define f_fld_null_flag 23  
#define f_fld_char_length 24  
#define f_fld_coll_id 25  
#define f_fld_charset_id 26  
#define f_fld_precision 27  
#define f_seg_name 0  
#define f_seg_field 1  
#define f_seg_position 2  
#define f_idx_name 0  
#define f_idx_relation 1  
#define f_idx_id 2  
#define f_idx_flag 3  
#define f_idx_desc 4  
#define f_idx_count 5  
#define f_idx_inactive 6  
#define f_idx_type 7  
#define f_idx_foreign 8  
#define f_idx_sys_flag 9  
#define f_idx_exp_blr 10  
#define f_idx_exp_source 11  
#define f_idx_statistics 12  
#define f_rfr_fname 0  
#define f_rfr_rname 1  
#define f_rfr_sname 2  
#define f_rfr_qname 3  
#define f_rfr_base 4  
#define f_rfr_estring 5  
#define f_rfr_position 6  
#define f_rfr_qheader 7  
#define f_rfr_flag 8  
#define f_rfr_id 9  
#define f_rfr_context 10  
#define f_rfr_desc 11  
#define f_rfr_default 12  
#define f_rfr_sys_flag 13  
#define f_rfr_class 14  
#define f_rfr_complex 15  
#define f_rfr_null_flag 16  
#define f_rfr_dsource 17  
#define f_rfr_coll_id 18  
#define f_rel_blr 0  
#define f_rel_source 1  
#define f_rel_desc 2  
#define f_rel_id 3  
#define f_rel_sys_flag 4  
#define f_rel_key_len 5  
#define f_rel_format 6  
#define f_rel_field_id 7  
#define f_rel_name 8  
#define f_rel_class 9  
#define f_rel_ext_file 10  
#define f_rel_runtime 11  
#define f_rel_ext_desc 12  
#define f_rel_owner 13  
#define f_rel_def_class 14  
#define f_rel_flags 15  
#define f_vrl_vname 0  
#define f_vrl_rname 1  
#define f_vrl_context 2  
#define f_vrl_cname 3  
#define f_fmt_rid 0  
#define f_fmt_format 1  
#define f_fmt_desc 2  
#define f_cls_class 0  
#define f_cls_acl 1  
#define f_cls_desc 2  
#define f_file_name 0  
#define f_file_seq 1  
#define f_file_start 2  
#define f_file_length 3  
#define f_file_flags 4  
#define f_file_shad_num 5  
#define f_typ_field 0  
#define f_typ_type 1  
#define f_typ_name 2  
#define f_typ_desc 3  
#define f_typ_sys_flag 4  
#define f_trg_name 0  
#define f_trg_rname 1  
#define f_trg_seq 2  
#define f_trg_type 3  
#define f_trg_source 4  
#define f_trg_blr 5  
#define f_trg_desc 6  
#define f_trg_inactive 7  
#define f_trg_sys_flag 8  
#define f_trg_flags 9  
#define f_dpd_name 0  
#define f_dpd_o_name 1  
#define f_dpd_f_name 2  
#define f_dpd_type 3  
#define f_dpd_o_type 4  
#define f_fun_name 0  
#define f_fun_type 1  
#define f_fun_qname 2  
#define f_fun_desc 3  
#define f_fun_module 4  
#define f_fun_entry 5  
#define f_fun_ret_arg 6  
#define f_fun_sys_flag 7  
#define f_arg_fun_name 0  
#define f_arg_pos 1  
#define f_arg_mech 2  
#define f_arg_type 3  
#define f_arg_scale 4  
#define f_arg_length 5  
#define f_arg_sub_type 6  
#define f_arg_charset_id 7  
#define f_arg_precision 8  
#define f_arg_char_length 9  
#define f_flt_name 0  
#define f_flt_desc 1  
#define f_flt_module 2  
#define f_flt_entry 3  
#define f_flt_input 4  
#define f_flt_output 5  
#define f_flt_sys_flag 6  
#define f_msg_trigger 0  
#define f_msg_number 1  
#define f_msg_msg 2  
#define f_prv_user 0  
#define f_prv_grantor 1  
#define f_prv_priv 2  
#define f_prv_grant 3  
#define f_prv_rname 4  
#define f_prv_fname 5  
#define f_prv_u_type 6  
#define f_prv_o_type 7  
#define f_trn_id 0  
#define f_trn_state 1  
#define f_trn_time 2  
#define f_trn_desc 3  
#define f_gen_name 0  
#define f_gen_id 1  
#define f_gen_sys_flag 2  
#define f_dims_fname 0  
#define f_dims_dim 1  
#define f_dims_lower 2  
#define f_dims_upper 3  
#define f_rcon_cname 0  
#define f_rcon_ctype 1  
#define f_rcon_rname 2  
#define f_rcon_dfr 3  
#define f_rcon_idfr 4  
#define f_rcon_iname 5  
#define f_refc_cname 0  
#define f_refc_uq 1  
#define f_refc_match 2  
#define f_refc_upd_rul 3  
#define f_refc_del_rul 4  
#define f_ccon_cname 0  
#define f_ccon_tname 1  
#define f_log_name 0  
#define f_log_seq 1  
#define f_log_length 2  
#define f_log_partitions 3  
#define f_log_p_offset 4  
#define f_log_flags 5  
#define f_prc_name 0  
#define f_prc_id 1  
#define f_prc_inputs 2  
#define f_prc_outputs 3  
#define f_prc_desc 4  
#define f_prc_source 5  
#define f_prc_blr 6  
#define f_prc_class 7  
#define f_prc_owner 8  
#define f_prc_runtime 9  
#define f_prc_sys_flag 10  
#define f_prm_name 0  
#define f_prm_procedure 1  
#define f_prm_number 2  
#define f_prm_type 3  
#define f_prm_sname 4  
#define f_prm_desc 5  
#define f_prm_sys_flag 6  
#define f_cs_cs_name 0  
#define f_cs_form_of_use 1  
#define f_cs_num_chars 2  
#define f_cs_def_collate 3  
#define f_cs_id 4  
#define f_cs_sys_flag 5  
#define f_cs_desc 6  
#define f_cs_fun_name 7  
#define f_cs_bytes_char 8  
#define f_coll_name 0  
#define f_coll_id 1  
#define f_coll_cs_id 2  
#define f_coll_attr 3  
#define f_coll_sys_flag 4  
#define f_coll_desc 5  
#define f_coll_fun_name 6  
#define f_xcp_name 0  
#define f_xcp_number 1  
#define f_xcp_msg 2  
#define f_xcp_desc 3  
#define f_xcp_sys_flag 4  
#define f_rol_name 0  
#define f_rol_owner 1  
