/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		met.h
 *	DESCRIPTION:	Random meta-data stuff
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

#ifndef _JRD_MET_H_
#define _JRD_MET_H_

/* Record types for record summary blob records */

typedef enum rsr_t {
	RSR_field_id,
	RSR_field_name,
	RSR_view_context,
	RSR_base_field,
	RSR_computed_blr,
	RSR_missing_value,
	RSR_default_value,
	RSR_validation_blr,
	RSR_security_class,
	RSR_trigger_name,
	RSR_dimensions,
	RSR_array_desc,

	RSR_relation_id,			/* The following are Gateway specific */
	RSR_relation_name,			/* and are used to speed the acquiring */
	RSR_rel_sys_flag,			/* of relation information */
	RSR_view_blr,
	RSR_owner_name,
	RSR_field_type,				/* The following are also Gateway */
	RSR_field_scale,			/* specific and relate to field info */
	RSR_field_length,
	RSR_field_sub_type,
	RSR_field_not_null
} RSR_T;

/* Temporary field block */

class tfb : public pool_alloc<type_tfb>
{
    public:
	struct tfb *tfb_next;		/* next block in chain */
	USHORT tfb_id;				/* id of field in relation */
	USHORT tfb_flags;
	DSC tfb_desc;
#ifdef GATEWAY
	XDSC tfb_ext_desc;			/* external descriptor */
	STR tfb_name;				/* name of field in relation */
#endif
};
typedef tfb *TFB;

#define TFB_computed	1
#define TFB_array	2

#define MET_object_active	0
#define MET_object_inactive	1
#define MET_object_unknown	2

#define TRIGGER_PRE_STORE	1
#define TRIGGER_POST_STORE	2
#define TRIGGER_PRE_MODIFY	3
#define TRIGGER_POST_MODIFY	4
#define TRIGGER_PRE_ERASE	5
#define TRIGGER_POST_ERASE	6
#define TRIGGER_MAX		7

#include "../jrd/obj.h"

#endif /* _JRD_MET_H_ */
