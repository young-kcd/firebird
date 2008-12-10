/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		blk.h
 *	DESCRIPTION:	Block type definitions
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

enum blk_t {
	dsql_type_MIN = 0,
	dsql_type_ctx,
	dsql_type_par,
	dsql_type_map, // Unique
	dsql_type_req,
	dsql_type_dbb,
	dsql_type_dsql_rel,
	dsql_type_fld,
	dsql_type_fil,
	dsql_type_nod,
	dsql_type_msg,
	dsql_type_lls, // linked list stack
	dsql_type_str, // random string block
	dsql_type_sym, // symbol block
	dsql_type_err,
	dsql_type_opn,
	dsql_type_tra,
	dsql_type_udf,
	dsql_type_var,
	dsql_type_blb,
	dsql_type_prc,
	dsql_type_intlsym,
	dsql_type_vec,
	dsql_type_MAX
};

