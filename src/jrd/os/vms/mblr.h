/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		MBLR.H
 *	DESCRIPTION:	Meta-data update language
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

#ifndef JRD_MBLR_H
#define JRD_MBLR_H

#define mblr_null			0

#define mblr$k_modify			1
#define mblr$k_define			2
#define mblr$k_delete			3

#define	mblr$k_version_1		1

#define	mblr$k_cdd_path			5
#define	mblr$k_change_field		6
#define	mblr$k_constraint		7
#define	mblr$k_constraint_def		8
#define	mblr$k_constraint_end		9
#define	mblr$k_descriptor		10
#define	mblr$k_define_field		11
#define	mblr$k_delete_field		12
#define	mblr$k_dtr_def_val		13
#define	mblr$k_dtr_edit_str		14
#define	mblr$k_dtr_qry_hdr		15
#define	mblr$k_dtr_qry_hdr_line		16
#define	mblr$k_dtr_qry_hdr_end		17
#define	mblr$k_duplicates		18
#define	mblr$k_field_def		19
#define	mblr$k_field_end		20
#define	mblr$k_field_expression		21
#define	mblr$k_field_name		22
#define	mblr$k_field_noupdate		23
#define	mblr$k_field_update		24

#define	mblr$k_index_def		26
#define	mblr$k_index_end		27
#define mblr$k_missing_value		28

#define	mblr$k_relation_def		31
#define	mblr$k_relation_end		32
#define	mblr$k_relation_name		33
#define	mblr$k_rse			34
#define	mblr$k_valid_if			35
#define	mblr$k_view_def			36
#define	mblr$k_view_end			37
#define	mblr$k_change_attributes	38
#define	mblr$k_delete_attributes	39
#define	mblr$k_commit			40
#define	mblr$k_update			41
#define	mblr$k_dtr_qry_nam		42
#define	mblr$k_valid_if_source		43
#define	mblr$k_computed_by_source	44
#define	mblr$k_rse_source		45
#define	mblr$k_constraint_count		46

#define mblr$k_description_source	51
#define mblr$k_sub_type			52
#define mblr$k_segment_length		53

#define mblr$k_index_unknown1		75
#define mblr$k_index_unknown2		76

#define mblr_max			54	/* the display table only goes this high */

#define dsc$k_dtype_qu			5	/* a segmented string */
#define dsc$k_dtype_b			6	/* ? blr_text & special case to be length 1 ? */
#define dsc$k_dtype_w			blr_short
#define dsc$k_dtype_l			blr_long
#define dsc$k_dtype_q			blr_quad
#define dsc$k_dtype_f			blr_float
#define dsc$k_dtype_g			blr_double
#define dsc$k_dtype_d			blr_d_float
#define dsc$k_dtype_t			blr_text
#define dsc$k_dtype_vt			blr_varying
#define dsc$k_dtype_adt			blr_timestamp

#endif /* JRD_MBLR_H */
