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

BLKDEF(type_vec, vec, sizeof(((vec*) NULL)->vec_object[0]))
BLKDEF(type_dbb, Database, 0)
BLKDEF(type_bcb, BufferControl, sizeof(((BufferControl*) NULL)->bcb_rpt[0]))   /* Done 2 */
BLKDEF(type_bdb, BufferDesc, 0)
BLKDEF(type_pre, Precedence, 0)
BLKDEF(type_lck, Lock, 1)
BLKDEF(type_fil, jrd_file, 1)
BLKDEF(type_pgc, PageControl, 0)
BLKDEF(type_rel, jrd_rel, 0)
BLKDEF(type_fmt, Format, sizeof(((Format*) NULL)->fmt_desc[0]))   /* Done */
BLKDEF(type_vcl, vcl, sizeof(((vcl*) NULL)->vcl_long[0]))   /* Done */
BLKDEF(type_req, jrd_req, sizeof(((jrd_req*) NULL)->req_rpb[0]))    /* Done */
BLKDEF(type_tra, jrd_tra, 1)
BLKDEF(type_nod, jrd_nod, sizeof(((jrd_nod*) NULL)->nod_arg[0]))    /* Done */
BLKDEF(type_csb, CompilerScratch, sizeof(((CompilerScratch*) NULL)->csb_rpt[0]))    /* Done */
BLKDEF(type_lls, lls, 0)	/* linked list stack */
BLKDEF(type_rec, Record, 1)	/* record parameter */
BLKDEF(type_rsb, RecordSource, sizeof(((RecordSource*) NULL)->rsb_arg[0]))	/* Done record source */
BLKDEF(type_bms, BitmapSegment, 0)	/* bit map segment */
BLKDEF(type_dfw, DeferredWork, 1)	/* deferred work block */
BLKDEF(type_tfb, TemporaryField, 0)	/* temporary field block */
BLKDEF(type_str, str, 1)	/* random string block */
BLKDEF(type_dcc, DataComprControl, 0)	/* data compression control */
BLKDEF(type_sbm, SparseBitmap, sizeof(((SparseBitmap*) NULL)->sbm_segments[0]))	/* done sparse bit map */
BLKDEF(type_smb, SortMap, sizeof(((SortMap*) NULL)->smb_rpt[0]))	/* done sort map block */
BLKDEF(type_blb, blb, 1)
BLKDEF(type_irb, IndexRetrieval, sizeof(((IndexRetrieval*) NULL)->irb_value[0]))	/* Done Index retrieval */
BLKDEF(type_scl, SecurityClass, 1)
BLKDEF(type_fld, jrd_fld, 1)
BLKDEF(type_ext, ExternalFile, 1)	/* External file */
BLKDEF(type_mfb, merge_file, 0)	/* merge (equivalence) file block */
BLKDEF(type_riv, River, 1)	/* River block -- used in optimizer */
BLKDEF(type_usr, UserId, 0)	/* User identification block */
BLKDEF(type_att, Jrd::Attachment, 0)	/* Database attachment */
BLKDEF(type_sym, Symbol, 0)
BLKDEF(type_fun, UserFunction, sizeof(((UserFunction*) NULL)->fun_rpt[0]))	/* Done Function definition */
BLKDEF(type_irl, IndexedRelationship, 0)
BLKDEF(type_acc, AccessItem, 0)
BLKDEF(type_idl, IndexLock, 0)
BLKDEF(type_rsc, Resource, 0)
BLKDEF(type_sdw, Shadow, 0)
BLKDEF(type_vct, VerbAction, 0)	/* Verb actions */
BLKDEF(type_btb, BlockingThread, 0)
BLKDEF(type_blf, BlobFilter, 0)
BLKDEF(type_arr, ArrayField, sizeof(((internal_array_desc*) NULL)->iad_rpt[0]))	/* Done, but funny   Array description */
BLKDEF(type_map, blb_map, 0)
BLKDEF(type_log, fblog, 0)
BLKDEF(type_dls, dir_list, 1)
//BLKDEF(type_ail, logfiles, 1)	// WAL file. Obsolete.
BLKDEF(type_prc, jrd_prc, 1)	/* procedure block */
BLKDEF(type_prm, Parameter, 1)	/* parameter block */
BLKDEF(type_sav, Savepoint, 0)	/* save points */
BLKDEF(type_xcp, PsqlException, sizeof(((PsqlException*) NULL)->xcp_rpt[0]))	/* exception condition list */
BLKDEF(type_idb, IndexBlock, 0)	/* index block for caching index info */
BLKDEF(type_bkm, Bookmark, 1)	/* bookmark block for storing current location */
BLKDEF(type_tpc, TxPageCache, 1)	/* TIP page cache block */
BLKDEF(type_rng, RefreshRange, 1)	// refresh range. Used by commented PC_ENGINE for ODAPI support.
BLKDEF(type_svc, Service, 1)	/* services */
BLKDEF(type_lwt, LatchWait, 0)	/* latch wait block */
BLKDEF(type_vcx, ViewContext, 0)	/* view context block */
BLKDEF(type_srpb, SaveRecordParam, 0)	/* save record_param block */
BLKDEF(type_opt, OptimizerBlk, 0)
BLKDEF(type_prf, Prefetch, 0)
BLKDEF(type_rse, RecordSelExpr, 0)
BLKDEF(type_lit, Literal, 0)
BLKDEF(type_asb, AggregateSort, 0)
//BLKDEF(type_srl, srl, 0) // Obsolete, apparently deleted by Sean's cleanup.
BLKDEF(type_ctl, BlobControl, 0)
