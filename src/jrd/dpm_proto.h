/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dpm_proto.h
 *	DESCRIPTION:	Prototype header file for dpm.cpp
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

#ifndef JRD_DPM_PROTO_H
#define JRD_DPM_PROTO_H

// fwd. decl.
class blb;

struct pag* DPM_allocate(TDBB, struct win*);
void	DPM_backout(TDBB, struct rpb *);
int		DPM_chain(TDBB, struct rpb *, struct rpb *);
int		DPM_compress(TDBB, struct dpg *);
void	DPM_create_relation(TDBB, struct jrd_rel *);
SLONG	DPM_data_pages(TDBB, struct jrd_rel *);
void	DPM_delete(TDBB, struct rpb *, SLONG);
void	DPM_delete_relation(TDBB, struct jrd_rel *);
BOOLEAN	DPM_fetch(TDBB, struct rpb *, USHORT);
SSHORT	DPM_fetch_back(TDBB, struct rpb *, USHORT, SSHORT);
void	DPM_fetch_fragment(TDBB, struct rpb *, USHORT);
SINT64	DPM_gen_id(TDBB, SLONG, USHORT, SINT64);
int		DPM_get(TDBB, struct rpb *, SSHORT);
ULONG	DPM_get_blob(TDBB, blb*, ULONG, USHORT, SLONG);
BOOLEAN	DPM_next(TDBB, struct rpb *, USHORT, BOOLEAN,
					BOOLEAN);
void	DPM_pages(TDBB, SSHORT, int, ULONG, SLONG);
SLONG	DPM_prefetch_bitmap(struct tdbb *, struct jrd_rel *, struct sbm *,
								 SLONG);
void	DPM_scan_pages(TDBB);
void	DPM_store(TDBB, struct rpb *, struct lls **, USHORT);
SLONG	DPM_store_blob(TDBB, blb*, struct rec *);
void	DPM_rewrite_header(TDBB, struct rpb *);
void	DPM_update(TDBB, struct rpb *, struct lls **,
					   struct jrd_tra *);

#endif /* JRD_DPM_PROTO_H */
