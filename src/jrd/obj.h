/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		obj.h
 *	DESCRIPTION:	Object types in meta-data
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

#ifndef _JRD_OBJ_H_
#define _JRD_OBJ_H_

/* Object types used in RDB$DEPENDENCIES and RDB$USER_PRIVILEGES */
/* Note: some values are hard coded in grant.gdl */

#define obj_relation            0
#define obj_view                1
#define obj_trigger             2
#define obj_computed            3
#define obj_validation          4
#define obj_procedure           5
#define obj_expression_index    6
#define obj_exception           7
#define obj_user                8
#define obj_field               9
#define obj_index               10
#define obj_count               11
#define obj_user_group          12
#define obj_sql_role            13
#define obj_generator		    14
#define obj_udf       	        15
#define obj_blob_filter		    16

#endif /* _JRD_OBJ_H_ */
