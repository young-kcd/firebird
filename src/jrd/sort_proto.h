/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sort_proto.h
 *	DESCRIPTION:	Prototype header file for sort.cpp
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

#ifndef JRD_SORT_PROTO_H
#define JRD_SORT_PROTO_H

namespace Jrd {
	class Attachment;
	struct sort_key_def;
	struct sort_work_file;
	struct sort_context;
}

#ifdef SCROLLABLE_CURSORS
void	SORT_diddle_key(UCHAR *, Jrd::sort_context*, bool);
void	SORT_get(ISC_STATUS*, Jrd::sort_context*, ULONG **, RSE_GET_MODE);
void	SORT_read_block(ISC_STATUS*, Jrd::sort_work_file*, ULONG, BLOB_PTR *, ULONG);
#else
void	SORT_get(ISC_STATUS*, Jrd::sort_context*, ULONG **);
ULONG	SORT_read_block(ISC_STATUS*, Jrd::sort_work_file*, ULONG, BLOB_PTR *,
							 ULONG);
#endif

void	SORT_error(ISC_STATUS*, Jrd::sort_work_file*, TEXT *, ISC_STATUS, int);
void	SORT_fini(Jrd::sort_context*, Jrd::Attachment*);
Jrd::sort_context*	SORT_init(ISC_STATUS*, USHORT, USHORT, USHORT, const Jrd::sort_key_def*,
						Jrd::FPTR_REJECT_DUP_CALLBACK, void*, Jrd::Attachment*, UINT64);
void	SORT_put(ISC_STATUS*, Jrd::sort_context*, ULONG **);
void	SORT_shutdown(Jrd::Attachment*);
bool	SORT_sort(ISC_STATUS*, Jrd::sort_context*);
ULONG	SORT_write_block(ISC_STATUS*, Jrd::sort_work_file*, ULONG, BLOB_PTR *,
							  ULONG);

#endif // JRD_SORT_PROTO_H

