/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dpm_proto.h
 *	DESCRIPTION:	Prototype header file for dpm.c
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

#ifndef _JRD_DPM_PROTO_H_
#define _JRD_DPM_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

extern struct pag *DPM_allocate(TDBB, struct win *);
extern void DPM_backout(TDBB, struct rpb *);
extern int DPM_chain(TDBB, struct rpb *, struct rpb *);
extern int DPM_compress(TDBB, struct dpg *);
extern void DPM_create_relation(TDBB, struct rel *);
extern SLONG DPM_data_pages(TDBB, struct rel *);
extern void DPM_delete(TDBB, struct rpb *, SLONG);
extern void DPM_delete_relation(TDBB, struct rel *);
extern BOOLEAN DPM_fetch(TDBB, register struct rpb *, USHORT);
extern SSHORT DPM_fetch_back(TDBB, register struct rpb *, USHORT, SSHORT);
extern void DPM_fetch_fragment(TDBB, register struct rpb *, USHORT);
extern SINT64 DPM_gen_id(TDBB, SLONG, USHORT, SINT64);
extern int DPM_get(TDBB, register struct rpb *, SSHORT);
extern ULONG DPM_get_blob(TDBB, struct blb *, ULONG, USHORT, SLONG);
extern BOOLEAN DPM_next(TDBB, register struct rpb *, USHORT, BOOLEAN,
						BOOLEAN);
extern void DPM_pages(TDBB, SSHORT, int, ULONG, SLONG);
extern SLONG DPM_prefetch_bitmap(struct tdbb *, struct rel *, struct sbm *,
								 SLONG);
extern void DPM_scan_pages(TDBB);
extern void DPM_store(TDBB, register struct rpb *, struct lls **, USHORT);
extern SLONG DPM_store_blob(TDBB, struct blb *, struct rec *);
extern void DPM_rewrite_header(TDBB, struct rpb *);
extern void DPM_update(TDBB, register struct rpb *, struct lls **,
					   struct tra *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_DPM_PROTO_H_ */
