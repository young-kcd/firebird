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

BLKDEF(dsql_type_ctx, dsql_ctx, 0)
BLKDEF(dsql_type_par, par, 0)
BLKDEF(dsql_type_map, map, 0)	/* Unique */
BLKDEF(dsql_type_req, dsql_req, 0)
BLKDEF(dsql_type_dbb, dbb, 0)
BLKDEF(dsql_type_dsql_rel, dsql_rel, 1)
BLKDEF(dsql_type_fld, dsql_fld, 1)
BLKDEF(dsql_type_fil, fil, 0)
BLKDEF(dsql_type_nod, dsql_nod, sizeof(((DSQL_NOD) 0)->nod_arg[0]))
BLKDEF(dsql_type_msg, msg, 0)
BLKDEF(dsql_type_lls, dsql_lls, 0)	/* linked list stack */
BLKDEF(dsql_type_str, str, 1)	/* random string block */
BLKDEF(dsql_type_sym, sym, 1)	/* symbol block */
BLKDEF(dsql_type_err, err, 0)
BLKDEF(dsql_type_opn, opn, 0)
BLKDEF(dsql_type_tra, dsql_tra, 0)
BLKDEF(dsql_type_udf, udf, 1)
BLKDEF(dsql_type_var, var, 1)
BLKDEF(dsql_type_blb, blb, 0)
BLKDEF(dsql_type_prc, dsql_prc, 1)
BLKDEF(dsql_type_intlsym, intlsym, 1)
BLKDEF(dsql_type_vec, vec, 0)
