/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		irq.h
 *	DESCRIPTION:	Registry of persistent internal requests
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
 * 2001.6.25 Claudio Valderrama: add irq_r_gen_id_num to support
 *	new MET_lookup_generator_id().
 */

#ifndef _JRD_IRQ_H_
#define _JRD_IRQ_H_

#define REQUEST(irt) (*dbb->dbb_internal)[irt]

#define irq_s_pages	0			/* store PAGES */
#define irq_r_pages	1			/* read PAGES */
#define irq_l_field	2			/* lookup field name */
#define irq_l_relation	3		/* lookup relation name */
#define irq_c_relation	4		/* create new relation */
#define irq_format1	5			/* make a new format for a record */
#define irq_format2	6			/* make a new format for a record */
#define irq_format3	7			/* make a new format for a record */
#define irq_r_format	8		/* read a format */
#define irq_r_relation	9		/* pick up view definition */
#define irq_c_index	10			/* create index */
#define irq_m_fields	11		/* process a modification of RDB$FIELDS */
#define irq_m_index	12			/* modify index id */
#define irq_r_fields	13		/* pick up field expressions */
#define irq_l_security	14		/* lookup security_classes */
#define irq_v_security	15		/* verify security for relation */
#define irq_l_index	16			/* lookup index id */
#define irq_l_functions	17		/* lookup function */
#define irq_l_args	18			/* lookup function arguments */
#define irq_s_triggers	19		/* scan triggers */
#define irq_s_triggers2	20		/* scan triggers */
#define irq_s_msgs	21			/* search for message */
#define irq_grant1	22			/* process grant option */
#define irq_grant2	23			/* process grant option */
#define irq_grant3	24			/* process grant option */
#define irq_grant4	25			/* process grant option */
#define irq_format4	26			/* make a new format for a record */
#define irq_s_trans	27			/* store RDB$TRANSACTIONS */
#define irq_m_trans	28			/* erase RDB$TRANSACTIONS */
#define irq_e_trans	29			/* store RDB$TRANSACTIONS */
#define irq_revoke1	30			/* check revokee for residual privileges */
#define irq_revoke2	31			/* recursive revoke */
#define irq_grant5	32			/* check for "PUBLIC" in processing grant options */
#define irq_r_filters	33		/* read RDB$BLOB_FILTERS */
#define irq_c_trigger	34		/* check for dependencies when creating a trigger */
#define irq_s_deps	35			/* store RDB$DEPENDENCIES */
#define irq_d_deps	36			/* delete RDB$DEPENDENCIES */
#define irq_r_fld_dim	37		/* read RDB$FIELD_DIMENSIONS */
#define irq_r_gen_id	38		/* read RDB$GENERATORS, lookup by name. */
#define irq_s_gen_id	39		/* store RDB$GENERATORS */
#define irq_ch_f_dpd	40		/* check object field dependencies */
#define irq_ch_dpd	41			/* check object dependencies */
#define irq_ch_cmp_dpd	42		/* check computed field dependencies */
#define irq_grant6	43			/* get field-level security class */
#define irq_grant7	44			/* update field-level security classes */
#define irq_grant8	45			/* purge out field-level security classes */
#define irq_foreign1	46		/* purge out field-level security classes */
#define irq_foreign2	47		/* purge out field-level security classes */
#define irq_format5	48			/* make a new format for a record */

#define irq_c_exp_index	49		/* create expression index */
#define irq_l_exp_index	50		/* lookup expression index */

#define irq_l_rel_id	51		/* lookup relation id */
#define irq_l_procedure	52		/* lookup procedure name */
#define irq_l_proc_id	53		/* lookup procedure id */
#define irq_r_params	54		/* scan procedure parameters */

#define irq_r_procedure	55		/* scan procedure */
#define irq_p_security	56		/* verify security for procedure */
#define irq_c_prc_dpd	57		/* create procedure dependencies for delete */

#define irq_c_deps	58			/* check relation dependency */
#define irq_c_deps_f	59		/* check relation.field dependency */
#define irq_l_exception 60		/* lookup exception by number */
#define irq_l_except_no 61		/* lookup exception by name */
#define irq_grant9	62			/* process grant option */
#define irq_l_index_name 63		/* lookup index by name */
#define irq_view_context 64		/* lookup view context */
#define irq_l_cnstrt	65		/* lookup constraint for index */
#define irq_l_check	66			/* lookup check constraint for trigger */
#define irq_l_check2	67		/* lookup constraint for index */
#define irq_c_trg_perm	68		/* check if trig can ignore perm. checks */
#define irq_get_role_mem	69	/* get SQL role membership */
#define irq_get_role_name	70	/* get SQL role name */
#define irq_format6     71		/* make a new format for a record */
#define irq_r_gen_id_num	72	/* lookup generator by ID. */
#define irq_verify_role_name	73	/* ensure role exists in roles & user_privileges. */
#define irq_l_relation_defsec	74	/* check the default sec class name against rel. */

#define irq_MAX		75

#endif /* _JRD_IRQ_H_ */
