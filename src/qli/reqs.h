/*
 *	PROGRAM:	Interactive Query Language Interpreter
 *	MODULE:		reqs.h
 *	DESCRIPTION:	Internal request number definitions.
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

#ifndef _QLI_REQS_H_
#define _QLI_REQS_H_

#define REQ_relation_def	0
#define REQ_store_relation	1
#define REQ_relation_id		2
#define REQ_rfr_def		3
#define REQ_field_def		4
#define REQ_store_field		5
#define REQ_store_rfr		6
#define REQ_modify_fld		7
#define REQ_update_fld		8
#define REQ_modify_rel		9
#define REQ_show_indices	10
#define REQ_show_index		11
#define REQ_check_fld		12
#define REQ_erase_fld		13
#define REQ_erase_index		14
#define REQ_erase_segments	15
#define REQ_erase_relation	16
#define REQ_erase_rfr		17
#define REQ_erase_view		18 
#define REQ_show_files		19
#define REQ_show_view		20
#define REQ_show_field		21
#define REQ_show_view_field	22
#define REQ_show_dbb		23
#define REQ_show_rel_detail	24
#define REQ_show_global_field	25
#define REQ_show_field_instance 26
#define REQ_show_global_fields	27
#define REQ_show_trigger	28
#define REQ_show_triggers	29
#define REQ_show_rel_secur	30
#define REQ_show_rel_extern	31
#define REQ_def_index1		32
#define REQ_def_index2		33
#define REQ_def_index3		34
#define REQ_mdf_index		35
#define REQ_mdf_rfr		36
#define REQ_show_forms1		37
#define REQ_show_forms2		38
/* #define REQ_trig_exists		39	OBSOLETE */
#define REQ_show_views		40
#define REQ_show_view_rel	41
#define REQ_show_secur_class	42
#define REQ_show_secur		43
#define REQ_scan_index		44
#define REQ_show_new_trigger	45
#define REQ_show_new_triggers	46
#define REQ_show_sys_triggers	47
#define REQ_new_trig_exists	48
#define REQ_sql_grant		49
#define REQ_sql_revoke		51
#define REQ_sql_cr_view		52
#define REQ_sql_alt_table	53
#define REQ_fld_positions       54
#define REQ_fld_subtype		55
#define REQ_show_functions	56
#define REQ_show_func_detail	57
#define REQ_show_func_args	58
#define REQ_fld_dimensions	59
#define REQ_show_filters	60
#define REQ_show_filter_detail	61 
#define REQ_show_index_type	62
#define REQ_show_trig_message   63
#define REQ_show_dimensions	64
#define REQ_max			65

#endif /* _QLI_REQS_H_ */
