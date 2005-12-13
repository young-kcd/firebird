/*
 *	PROGRAM:	JRD Access Method
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

/* In the new memory management code the frb, hnk, and plb types are
 * internal to the management implementation, and as such are not given
 * types using the general db engine typing scheme.
 **/
// BLKDEF(type_frb, frb, 0)
// BLKDEF(type_hnk, hnk, 0)
// BLKDEF(type_plb, plb, 0)

BLKDEF(type_vec, vec, sizeof(((VEC) 0)->vec_object[0]))
BLKDEF(type_dbb, dbb, 0)
BLKDEF(type_bcb, bcb, sizeof(((BCB) 0)->bcb_rpt[0]))   /* Done 2 */
BLKDEF(type_bdb, bdb, 0)
BLKDEF(type_pre, pre, 0)
BLKDEF(type_lck, lck, 1)
BLKDEF(type_fil, fil, 1)
BLKDEF(type_pgc, pgc, 0)
BLKDEF(type_rel, jrd_rel, 0)
BLKDEF(type_fmt, fmt, sizeof(((FMT) 0)->fmt_desc[0]))   /* Done */
BLKDEF(type_vcl, vcl, sizeof(((VCL) 0)->vcl_long[0]))   /* Done */
BLKDEF(type_req, jrd_req, sizeof(((JRD_REQ) 0)->req_rpb[0]))    /* Done */
BLKDEF(type_tra, jrd_tra, 1)
BLKDEF(type_nod, jrd_nod, sizeof(((JRD_NOD) 0)->nod_arg[0]))    /* Done */
BLKDEF(type_csb, Csb, sizeof(((CSB) 0)->csb_rpt[0]))    /* Done */
BLKDEF(type_lls, lls, 0)	/* linked list stack */
BLKDEF(type_rec, rec, 1)	/* record parameter */
BLKDEF(type_rsb, Rsb, sizeof(((RSB) 0)->rsb_arg[0]))	/* Done record source */
BLKDEF(type_bms, bms, 0)	/* bit map segment */
BLKDEF(type_dfw, dfw, 1)	/* deferred work block */
BLKDEF(type_tfb, tfb, 0)	/* temporary field block */
BLKDEF(type_str, str, 1)	/* random string block */
BLKDEF(type_dcc, Dcc, 0)	/* data compression control */
BLKDEF(type_sbm, sbm, sizeof(((SBM) 0)->sbm_segments[0]))	/* done sparse bit map */
BLKDEF(type_smb, smb, sizeof(((SMB) 0)->smb_rpt[0]))	/* done sort map block */
BLKDEF(type_blb, blb, 1)
BLKDEF(type_irb, irb, sizeof(((IRB) 0)->irb_value[0]))	/* Done Index retrieval */
BLKDEF(type_jrn, jrn, 1)
BLKDEF(type_scl, scl, 1)
BLKDEF(type_fld, fld, 1)
BLKDEF(type_ext, ext, 1)	/* External file */
BLKDEF(type_mfb, mfb, 0)	/* merge (equivalence) file block */
BLKDEF(type_riv, riv, 1)	/* River block -- used in optimizer */
BLKDEF(type_usr, usr, 0)	/* User identification block */
BLKDEF(type_att, att, 0)	/* Database attachment */
BLKDEF(type_sym, sym, 0)
BLKDEF(type_fun, fun, sizeof(((FUN) 0)->fun_rpt[0]))	/* Done Function definition */
BLKDEF(type_irl, irl, 0)
BLKDEF(type_acc, acc, 0)
BLKDEF(type_idl, idl, 0)
BLKDEF(type_rsc, Rsc, 0)
BLKDEF(type_sdw, sdw, 0)
BLKDEF(type_vct, vct, 0)	/* Verb actions */
BLKDEF(type_btb, btb, 0)
BLKDEF(type_blf, blf, 0)
BLKDEF(type_arr, arr, sizeof(((ADS) 0)->ads_rpt[0]))	/* Done, but funny   Array description */
BLKDEF(type_map, map, 0)
BLKDEF(type_log, log, 0)
BLKDEF(type_dls, dls, 1)
BLKDEF(type_ail, logfiles, 1)	/* wal file */
BLKDEF(type_texttype_alloc_id, texttype, 0)	/* Intl Text Impl Object */
BLKDEF(type_charset_alloc_id, charset, 0)	/* Intl Charset definition */
BLKDEF(type_csconvert_alloc_id, csconvert, 0)	/* Intl character set converter */
BLKDEF(type_prc, jrd_prc, 1)	/* procedure block */
BLKDEF(type_prm, prm, 1)	/* parameter block */
BLKDEF(type_sav, sav, 0)	/* save points */
BLKDEF(type_xcp, xcp, sizeof(((XCP) 0)->xcp_rpt[0]))	/* exception condition list */
BLKDEF(type_idb, idb, 0)	/* index block for caching index info */
BLKDEF(type_bkm, bkm, 1)	/* bookmark block for storing current location */
BLKDEF(type_tpc, tpc, 1)	/* TIP page cache block */
BLKDEF(type_rng, rng, 1)	/* refresh range */
BLKDEF(type_svc, svc, 1)	/* services */
BLKDEF(type_lwt, lwt, 0)	/* latch wait block */
BLKDEF(type_vcx, vcx, 0)	/* view context block */
BLKDEF(type_srpb, srpb, 0)	/* save rpb block */
BLKDEF(type_opt, Opt, 0)
BLKDEF(type_prf, prf, 0)
BLKDEF(type_rse, rse, 0)
BLKDEF(type_lit, lit, 0)
BLKDEF(type_asb, asb, 0)
BLKDEF(type_srl, srl, 0)
BLKDEF(type_ctl, ctl, 0)
