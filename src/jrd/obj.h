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

#ifndef JRD_OBJ_H
#define JRD_OBJ_H

// Object types used in RDB$DEPENDENCIES and RDB$USER_PRIVILEGES
// Note: some values are hard coded in grant.gdl

const int obj_relation			= 0;
const int obj_view				= 1;
const int obj_trigger			= 2;
const int obj_computed			= 3;
const int obj_validation		= 4;
const int obj_procedure			= 5;
const int obj_expression_index	= 6;
const int obj_exception			= 7;
const int obj_user				= 8;
const int obj_field				= 9;
const int obj_index				= 10;
const int obj_charset			= 11;
const int obj_user_group		= 12;
const int obj_sql_role			= 13;
const int obj_generator			= 14;
const int obj_udf				= 15;
const int obj_blob_filter		= 16;
const int obj_collation			= 17;
const int obj_package_header	= 18;
const int obj_package_body		= 19;

// objects types for ddl operations
const int obj_database			= 20;
const int obj_relations			= 21;
const int obj_views				= 22;
const int obj_procedures		= 23;
const int obj_functions			= 24;
const int obj_packages			= 25;
const int obj_generators		= 26;
const int obj_domains			= 27;
const int obj_exceptions		= 28;
const int obj_roles				= 29;
const int obj_shadows			= 30;
const int obj_charsets			= 31;
const int obj_collations		= 32;
const int obj_filters			= 33;

const int obj_type_MAX			= 34;	// keep this last!

// used in the parser only / no relation with obj_type_MAX
const int obj_user_or_role		= 35;
const int obj_schema			= 36;
const int obj_parameter			= 37;

static char* get_object_name(int object_type)
{
	switch (object_type)
	{
		case obj_relations:
			return "OBJ$TABLES";
		case obj_views:
			return "OBJ$VIEWS";
		case obj_procedures:
			return "OBJ$PROCEDURES";
		case obj_functions:
			return "OBJ$FUNCTIONS";
		case obj_packages:
			return "OBJ$PACKAGES";
		case obj_generators:
			return "OBJ$GENERATORS";
		case obj_filters:
			return "OBJ$FILTERS";
		case obj_domains:
			return "OBJ$DOMAINS";
		case obj_exceptions:
			return "OBJ$EXCEPTIONS";
		case obj_roles:
			return "OBJ$ROLES";
		case obj_shadows:
			return "OBJ$SHADOWS";
		case obj_charsets:
			return "OBJ$CHARSETS";
		case obj_collations:
			return "OBJ$COLLATIONS";
		default:
			return "";
	}
}

#endif // JRD_OBJ_H
