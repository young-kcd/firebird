/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		types.h
 *	DESCRIPTION:	System type definitions
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
 * 2001.10.03 Claudio Valderrama: add int64, user_group, role, generator,
 *   udf, blob_filter and FB's new system flags for special triggers.
 *
 */

TYPE("TEXT", blr_text, nam_FIELD_TYPE)
	TYPE("SHORT", blr_short, nam_FIELD_TYPE)
	TYPE("LONG", blr_long, nam_FIELD_TYPE)
	TYPE("QUAD", blr_quad, nam_FIELD_TYPE)
	TYPE("FLOAT", blr_float, nam_FIELD_TYPE)
	TYPE("DOUBLE", blr_double, nam_FIELD_TYPE)
	TYPE("TIMESTAMP", blr_timestamp, nam_FIELD_TYPE)
	TYPE("VARYING", blr_varying, nam_FIELD_TYPE)
	TYPE("BLOB", blr_blob, nam_FIELD_TYPE)
	TYPE("CSTRING", blr_cstring, nam_FIELD_TYPE)
	TYPE("BLOB_ID", blr_blob_id, nam_FIELD_TYPE)
	TYPE("DATE", blr_sql_date, nam_FIELD_TYPE)
	TYPE("TIME", blr_sql_time, nam_FIELD_TYPE)
    TYPE ("INT64", blr_int64, nam_FIELD_TYPE)


	TYPE("TEXT", 1, nam_FIELD_SUB_TYPE)
	TYPE("BLR", 2, nam_FIELD_SUB_TYPE)
	TYPE("ACL", 3, nam_FIELD_SUB_TYPE)
	TYPE("RANGES", 4, nam_FIELD_SUB_TYPE)
	TYPE("SUMMARY", 5, nam_FIELD_SUB_TYPE)
	TYPE("FORMAT", 6, nam_FIELD_SUB_TYPE)
	TYPE("TRANSACTION_DESCRIPTION", 7, nam_FIELD_SUB_TYPE)
	TYPE("EXTERNAL_FILE_DESCRIPTION", 8, nam_FIELD_SUB_TYPE)

	TYPE("VALUE", 0, nam_FUNCTION_TYPE)
	TYPE("BOOLEAN", 1, nam_FUNCTION_TYPE)

	TYPE("BY_VALUE", 0, nam_MECHANISM)
	TYPE("BY_REFERENCE", 1, nam_MECHANISM)
	TYPE("BY_VMS_DESCRIPTOR", 2, nam_MECHANISM)
	TYPE("BY_ISC_DESCRIPTOR", 3, nam_MECHANISM)
	TYPE("BY_SCALAR_ARRAY_DESCRIPTOR", 4, nam_MECHANISM)

	TYPE("PRE_STORE", 1, nam_TRIGGER_TYPE)
	TYPE("POST_STORE", 2, nam_TRIGGER_TYPE)
	TYPE("PRE_MODIFY", 3, nam_TRIGGER_TYPE)
	TYPE("POST_MODIFY", 4, nam_TRIGGER_TYPE)
	TYPE("PRE_ERASE", 5, nam_TRIGGER_TYPE)
	TYPE("POST_ERASE", 6, nam_TRIGGER_TYPE)

    TYPE ("RELATION", obj_relation, nam_OBJECT_TYPE)
    TYPE ("VIEW",  obj_view, nam_OBJECT_TYPE)
    TYPE ("TRIGGER", obj_trigger, nam_OBJECT_TYPE)
    TYPE ("COMPUTED_FIELD", obj_computed, nam_OBJECT_TYPE)
    TYPE ("VALIDATION", obj_validation, nam_OBJECT_TYPE)
    TYPE ("PROCEDURE", obj_procedure, nam_OBJECT_TYPE)
    TYPE ("EXPRESSION_INDEX", obj_expression_index, nam_OBJECT_TYPE)
    TYPE ("EXCEPTION", obj_exception, nam_OBJECT_TYPE)
    TYPE ("USER",  obj_user, nam_OBJECT_TYPE)
    TYPE ("FIELD",  obj_field, nam_OBJECT_TYPE)
    TYPE ("INDEX",  obj_index, nam_OBJECT_TYPE)
    TYPE ("DEPENDENT_COUNT", obj_count, nam_OBJECT_TYPE)
    TYPE ("USER_GROUP", obj_user_group, nam_OBJECT_TYPE)
    TYPE ("ROLE",  obj_sql_role, nam_OBJECT_TYPE)
    TYPE ("GENERATOR", obj_generator, nam_OBJECT_TYPE)
    TYPE ("UDF",  obj_udf, nam_OBJECT_TYPE)
    TYPE ("BLOB_FILTER", obj_blob_filter, nam_OBJECT_TYPE)

	TYPE("LIMBO", 1, nam_TRANSACTION_STATE)
	TYPE("COMMITTED", 2, nam_TRANSACTION_STATE)
	TYPE("ROLLED_BACK", 3, nam_TRANSACTION_STATE)

    TYPE ("USER",  0, nam_SYSTEM_FLAG)
    TYPE ("SYSTEM",  1, nam_SYSTEM_FLAG)
    TYPE ("QLI",  2, nam_SYSTEM_FLAG)
    TYPE ("CHECK_CONSTRAINT", 3, nam_SYSTEM_FLAG)
    TYPE ("REFERENTIAL_CONSTRAINT", 4, nam_SYSTEM_FLAG)
    TYPE ("VIEW_CHECK", 5, nam_SYSTEM_FLAG)

