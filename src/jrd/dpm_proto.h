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
class lls;
class jrd_rel;
struct rpb;

struct pag* DPM_allocate(thread_db*, struct win*);
void	DPM_backout(thread_db*, rpb*);
int		DPM_chain(thread_db*, rpb*, rpb*);
int		DPM_compress(thread_db*, struct data_page*);
void	DPM_create_relation(thread_db*, jrd_rel*);
SLONG	DPM_data_pages(thread_db*, jrd_rel*);
void	DPM_delete(thread_db*, rpb*, SLONG);
void	DPM_delete_relation(thread_db*, jrd_rel*);
bool	DPM_fetch(thread_db*, rpb*, USHORT);
SSHORT	DPM_fetch_back(thread_db*, rpb*, USHORT, SSHORT);
void	DPM_fetch_fragment(thread_db*, rpb*, USHORT);
SINT64	DPM_gen_id(thread_db*, SLONG, USHORT, SINT64);
int		DPM_get(thread_db*, rpb*, SSHORT);
ULONG	DPM_get_blob(thread_db*, blb*, ULONG, USHORT, SLONG);
bool	DPM_next(thread_db*, rpb*, USHORT, bool, bool);
void	DPM_pages(thread_db*, SSHORT, int, ULONG, SLONG);
SLONG	DPM_prefetch_bitmap(struct thread_db*, jrd_rel*, struct sbm *,
								 SLONG);
void	DPM_scan_pages(thread_db*);
void	DPM_store(thread_db*, rpb*, lls**, USHORT);
SLONG	DPM_store_blob(thread_db*, blb*, class rec*);
void	DPM_rewrite_header(thread_db*, rpb*);
void	DPM_update(thread_db*, rpb*, lls**,
					   class jrd_tra *);

#endif // JRD_DPM_PROTO_H

