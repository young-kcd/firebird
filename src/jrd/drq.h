/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		drq.h
 *	DESCRIPTION:	Registry of persistent internal DYN requests
 *
 *
 ***************************************************************************
 *
 * Note:
 *
 *   drq_e_l_idx/109 and drq_l_idx_seg/110 are not used any more. The usage of
 *   drq_e_l_idx and drq_l_idx_seg in the past is as the following:
 *    
 *
 *      FOR (...) IDX IN RDB$INDICES
 *               WITH IDX.RDB$RELATION_NAME EQ table_name
 *                   ...
 *         FOR (...) FIRST 1 IDX_SEG IN RDB$INDEX_SEGMENTS
 *                      WITH IDX_SEG.RDB$INDEX_NAME EQ IDX.RDB$INDEX_NAME
 *                       AND IDX_SEG.RDB$FIELD_NAME = col_nm
 *                   ...
 *         END_FOR;
 *                   ...
 *      END_FOR;
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

#ifndef _JRD_DRQ_H_
#define _JRD_DRQ_H_

#define DYN_REQUEST(drt) (*dbb->dbb_dyn_req)[drt]

#define drq_l_prot_mask	0		/* lookup protection mask */
#define drq_l_user_name	1		/* lookup user name */
#define drq_s_rel_con	2		/* store relation constraints */
#define drq_s_chk_con	3		/* store check constraints */
#define drq_s_ref_con	4		/* store ref constraints */
#define drq_f_nxt_con	5		/* find next relation constraint name */
#define drq_f_nxt_fld	6		/* find next field name */
#define drq_f_nxt_idx	7		/* find next index name */
#define drq_f_nxt_trg	8		/* find next trigger name */
#define drq_c_unq_nam	9		/* check for unique field names */
#define drq_e_rel_con	10		/* erase relation constraints */
#define drq_n_idx_seg	11		/* count index segments */
#define drq_c_dup_con	12		/* check for duplicate contraint */
#define drq_l_intg_con	13		/* lookup an integrity constraint */
#define drq_s_dims	14			/* store dimensions */
#define drq_s_files	15			/* store files */
#define drq_s_filters	16		/* store filters */
#define drq_s_gens	17			/* store generators */
#define drq_s_funcs	18			/* store functions */
#define drq_s_func_args	19		/* store function arguments */
#define drq_s_gfields	20		/* store global fields */
#define drq_s_lfields	21		/* store local fields */
#define drq_s_gfields2	22		/* store global fields */
#define drq_s_rels	23			/* store relations */
#define drq_l_rel_name	24		/* lookup relation name */
#define drq_l_view_rels	25		/* lookup relations in view */
#define drq_s_usr_prvs	26		/* store user privileges */
#define drq_s_classes	27		/* store security classes */
#define drq_s_sql_lfld	28		/* store sql fields */
#define drq_s_sql_gfld	29		/* store sql fields */
#define drq_s_triggers	30		/* store triggers */
#define drq_s_trg_msgs	31		/* store trigger messages */
#define drq_s_view_rels	32		/* store view relations */
#define drq_e_dims	33			/* erase dimensions */
#define drq_e_filters	34		/* erase filters */
#define drq_e_func_args	35		/* erase functions */
#define drq_e_funcs	36			/* erase function arguments */
#define drq_l_fld_src	37		/* lookup a field source */
#define drq_e_gfields	38		/* erase global fields */
#define drq_e_indices	39		/* erase indices */
#define drq_e_idx_segs	40		/* erase index segments */
#define drq_l_dep_flds	41		/* lookup field referenced by view */
#define drq_e_lfield	42		/* erase a local field */
#define drq_e_rel_con2	43		/* erase relation constraints */
#define drq_e_rel_idxs	44		/* erase indices */
#define drq_e_rel_flds	45		/* erase relation fields */
#define drq_e_view_rels	46		/* erase view relations */
#define drq_e_relation	47		/* erase relation */
#define drq_e_rel_con3	48		/* erase relation constraints */
#define drq_e_usr_prvs	49		/* erase user privileges on relation */
#define drq_e_shadow	50		/* erase shadow */
#define drq_e_trg_msgs	51		/* erase trigger messages */
#define drq_e_trigger	52		/* erase trigger */
#define drq_l_view_rel2	53		/* lookup relations in view */
#define drq_m_rel_flds	54		/* modify relation fields */
#define drq_e_trg_msg	55		/* erase trigger message */
#define drq_e_class	56			/* erase security class */
#define drq_l_grant1	57		/* lookup grant */
#define drq_l_grant2	58		/* lookup grant */
#define drq_s_grant	59			/* store grant */
#define drq_l_fld_src2	60		/* lookup a field source */
#define drq_m_database	61		/* modify database */
#define drq_m_gfield	62		/* modify global field */
#define drq_m_index	63			/* modify index */
#define drq_m_lfield	64		/* modify local field */
#define drq_m_relation	65		/* modify relation */
#define drq_m_trigger	66		/* modify trigger */
#define drq_m_trg_msg	67		/* modify trigger message */
#define drq_e_grant1	68		/* erase grant */
#define drq_e_grant2	69		/* erase grant */
#define drq_s_indices	70		/* store indices */
#define drq_l_lfield	71		/* lookup local field */
#define drq_s_idx_segs	72		/* store index segments */
#define drq_l_unq_idx	73		/* lookup a unique index */
#define drq_l_primary	74		/* lookup a primary something */
#define drq_e_trg_msgs2	75		/* erase trigger messages */
#define drq_e_trigger2	76		/* erase trigger */
#define drq_s_prcs	77			/* store procedure */
#define drq_l_prc_name	78		/* lookup procedure name */
#define drq_s_prc_usr_prvs 79	/* store procedure priviledges */
#define drq_s_prms	80			/* store parameters */
#define drq_e_prcs	81			/* erase procedure */
#define drq_e_prms	82			/* erase all of procedure's parameters */
#define drq_s_prm_src	83		/* store parameter global field */
#define drq_s_intl_info	84		/* store RDB$CHARACTER_FIELDS */
#define drq_m_prcs	85			/* modify procedure */
#define drq_s_log_files 86		/* store log files */
#define drq_s_cache	87			/* store cache  */
#define drq_e_prm       88		/* erase a procedure parameter */
#define drq_s_xcp       89		/* store an exception */
#define drq_m_xcp       90		/* modify an exception */
#define drq_e_prc_prvs  91		/* erase user privileges on procedure */
#define drq_e_prc_prv   92		/* erase procedure's privileges  */
#define drq_e_trg_prv   93		/* erase trigger's privileges */
#define drq_d_log       94		/* drop log */
#define drq_d_cache     95		/* drop cache */
#define drq_l_log_files 96		/* lookup log files */
#define drq_l_cache     97		/* lookup cache */
#define drq_e_sec_class	98		/* delete security classes */
#define drq_l_gfield    99		/* lookup global field */
#define drq_g_nxt_con	100		/* generate next relation constraint name */
#define drq_g_nxt_fld	101		/* generate next field name */
#define drq_g_nxt_idx	102		/* generate next index name */
#define drq_g_nxt_trg	103		/* generate next trigger name */
#define drq_l_fld_pos	104		/* lookup max field position */
#define drq_e_xcp       105		/* drop an exception */
#define drq_d_gfields   106		/* drop an global field for procedure param */
#define drq_l_shadow    107		/* look up a shadow set */
#define drq_l_files	108			/* look up for defined files */
#define drq_e_l_idx     109		/* erase indices defined on a local field */
#define drq_l_idx_seg   110		/* Lookup index segments */
#define drq_e_l_gfld    111		/* erase global field for a local fields */
#define drq_gcg1	112			/* grantor_can_grant */
#define drq_gcg2	113			/* grantor_can_grant */
#define drq_gcg3	114			/* grantor_can_grant */
#define drq_gcg4	115			/* grantor_can_grant */
#define drq_gcg5	116			/* grantor_can_grant */
#define drq_l_view_idx	117		/* table is view? */
#define drq_role_gens       118	/* store SQL role */
#define drq_get_role_nm     119	/* get SQL role */
#define drq_get_role_au     120	/* get SQL role auth */
#define drq_del_role_1      121	/* delete SQL role from rdb$user_privilege */
#define drq_drop_role       122	/* delete SQL role from rdb$roles */
#define drq_get_rel_owner   123	/* get the owner of any relations */
#define drq_get_user_priv   124	/* get the grantor of user privileges or
								   the user who was granted the privileges */
#define drq_g_rel_constr_nm 125	/* get relation constraint name */
#define drq_e_rel_const     126	/* erase relation constraints */
#define drq_e_gens	    127		/* erase generators */
#define drq_s_f_class	    128 /* set the security class name for a field */
#define drq_s_u_class	    129 /* find a unique security class name for a field */
#define drq_l_difference    130 /* Look up a backup difference file */
#define drq_s_difference    131 /* Store backup difference file */
#define drq_d_difference    132 /* Delete backup difference file */
#define drq_MAX             133

#endif /* _JRD_DRQ_H_ */
