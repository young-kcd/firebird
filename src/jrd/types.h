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
 */

TYPE("TEXT", blr_text, nam_f_type)
	TYPE("SHORT", blr_short, nam_f_type)
	TYPE("LONG", blr_long, nam_f_type)
	TYPE("QUAD", blr_quad, nam_f_type)
	TYPE("FLOAT", blr_float, nam_f_type)
	TYPE("DOUBLE", blr_double, nam_f_type)
	TYPE("TIMESTAMP", blr_timestamp, nam_f_type)
	TYPE("VARYING", blr_varying, nam_f_type)
	TYPE("BLOB", blr_blob, nam_f_type)
	TYPE("CSTRING", blr_cstring, nam_f_type)
	TYPE("BLOB_ID", blr_blob_id, nam_f_type)
	TYPE("DATE", blr_sql_date, nam_f_type)
	TYPE("TIME", blr_sql_time, nam_f_type)


	TYPE("TEXT", 1, nam_f_sub_type)
	TYPE("BLR", 2, nam_f_sub_type)
	TYPE("ACL", 3, nam_f_sub_type)
	TYPE("RANGES", 4, nam_f_sub_type)
	TYPE("SUMMARY", 5, nam_f_sub_type)
	TYPE("FORMAT", 6, nam_f_sub_type)
	TYPE("TRANSACTION_DESCRIPTION", 7, nam_f_sub_type)
	TYPE("EXTERNAL_FILE_DESCRIPTION", 8, nam_f_sub_type)

	TYPE("VALUE", 0, nam_fun_type)
	TYPE("BOOLEAN", 1, nam_fun_type)

	TYPE("BY_VALUE", 0, nam_mechanism)
	TYPE("BY_REFERENCE", 1, nam_mechanism)
	TYPE("BY_VMS_DESCRIPTOR", 2, nam_mechanism)
	TYPE("BY_ISC_DESCRIPTOR", 3, nam_mechanism)
	TYPE("BY_SCALAR_ARRAY_DESCRIPTOR", 4, nam_mechanism)

	TYPE("PRE_STORE", 1, nam_trg_type)
	TYPE("POST_STORE", 2, nam_trg_type)
	TYPE("PRE_MODIFY", 3, nam_trg_type)
	TYPE("POST_MODIFY", 4, nam_trg_type)
	TYPE("PRE_ERASE", 5, nam_trg_type)
	TYPE("POST_ERASE", 6, nam_trg_type)

	TYPE("RELATION", 0, nam_obj_type)
	TYPE("VIEW", 1, nam_obj_type)
	TYPE("TRIGGER", 2, nam_obj_type)
	TYPE("COMPUTED_FIELD", 3, nam_obj_type)
	TYPE("VALIDATION", 4, nam_obj_type)
	TYPE("PROCEDURE", 5, nam_obj_type)
	TYPE("EXPRESSION_INDEX", 6, nam_obj_type)
	TYPE("EXCEPTION", 7, nam_obj_type)
	TYPE("USER", 8, nam_obj_type)
	TYPE("FIELD", 9, nam_obj_type)
	TYPE("INDEX", 10, nam_obj_type)

	TYPE("LIMBO", 1, nam_trans_state)
	TYPE("COMMITTED", 2, nam_trans_state)
	TYPE("ROLLED_BACK", 3, nam_trans_state)
