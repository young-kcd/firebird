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
struct record_param;
class SparseBitmap;
class Record;
class jrd_tra;

struct pag* DPM_allocate(thread_db*, struct win*);
void	DPM_backout(thread_db*, record_param*);
int		DPM_chain(thread_db*, record_param*, record_param*);
int		DPM_compress(thread_db*, struct data_page*);
void	DPM_create_relation(thread_db*, jrd_rel*);
SLONG	DPM_data_pages(thread_db*, jrd_rel*);
void	DPM_delete(thread_db*, record_param*, SLONG);
void	DPM_delete_relation(thread_db*, jrd_rel*);
bool	DPM_fetch(thread_db*, record_param*, USHORT);
SSHORT	DPM_fetch_back(thread_db*, record_param*, USHORT, SSHORT);
void	DPM_fetch_fragment(thread_db*, record_param*, USHORT);
SINT64	DPM_gen_id(thread_db*, SLONG, USHORT, SINT64);
int		DPM_get(thread_db*, record_param*, SSHORT);
ULONG	DPM_get_blob(thread_db*, blb*, ULONG, USHORT, SLONG);
bool	DPM_next(thread_db*, record_param*, USHORT, bool, bool);
void	DPM_pages(thread_db*, SSHORT, int, ULONG, SLONG);
SLONG	DPM_prefetch_bitmap(struct thread_db*, jrd_rel*, SparseBitmap*, SLONG);
void	DPM_scan_pages(thread_db*);
void	DPM_store(thread_db*, record_param*, lls**, USHORT);
SLONG	DPM_store_blob(thread_db*, blb*, Record*);
void	DPM_rewrite_header(thread_db*, record_param*);
void	DPM_update(thread_db*, record_param*, lls**, const jrd_tra*);

#endif // JRD_DPM_PROTO_H

