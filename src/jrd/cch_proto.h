/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cch_proto.h
 *	DESCRIPTION:	Prototype header file for cch.cpp
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

#ifndef JRD_CCH_PROTO_H
#define JRD_CCH_PROTO_H

void		CCH_shutdown_database(Database*);

USHORT		CCH_checksum(bdb*);
int			CCH_down_grade_dbb(void* ast_argument);
bool		CCH_exclusive(TDBB, USHORT, SSHORT);
bool		CCH_exclusive_attachment(TDBB, USHORT, SSHORT);
void		CCH_expand(TDBB, ULONG);
pag*		CCH_fake(TDBB, win*, SSHORT);
pag*		CCH_fetch(TDBB, win*, USHORT, SSHORT, SSHORT, SSHORT, bool);
SSHORT		CCH_fetch_lock(TDBB, win*, USHORT, SSHORT, SSHORT, SSHORT);
void		CCH_fetch_page(TDBB, win*, SSHORT, bool);
void		CCH_fini(TDBB);
void		CCH_flush(TDBB, USHORT, SLONG);
bool		CCH_free_page(TDBB);
SLONG		CCH_get_incarnation(win*);
pag*		CCH_handoff(TDBB, win*, SLONG, SSHORT, SSHORT, SSHORT, SSHORT);
void		CCH_init(TDBB, ULONG);
void		CCH_mark(TDBB, win*, USHORT);
void		CCH_mark_must_write(TDBB, win*);
void		CCH_must_write(win*);
lck*		CCH_page_lock(TDBB);
void		CCH_precedence(TDBB, win*, SLONG);
void		CCH_prefetch(tdbb*, SLONG*, SSHORT);
bool		CCH_prefetch_pages(TDBB);
void		CCH_release(TDBB, win*, bool);
void		CCH_release_and_free(win*);
void		CCH_release_exclusive(TDBB);
bool		CCH_rollover_to_shadow(Database*, jrd_file*, const bool);
void		CCH_unwind(TDBB, bool);
bool		CCH_validate(win*);
void		CCH_flush_database(TDBB tdbb);
bool		CCH_write_all_shadows(TDBB, Shadow*, bdb*,
					 ISC_STATUS*, USHORT, const bool);

/* macros for dealing with cache pages */

#define CCH_FETCH(tdbb, window, lock, type)		  CCH_fetch (tdbb, window, lock, type, 1, 1, true)
#define CCH_FETCH_NO_SHADOW(tdbb, window, lock, type)		  CCH_fetch (tdbb, window, lock, type, 1, 1, false)
#define CCH_FETCH_NO_CHECKSUM(tdbb, window, lock, type)   CCH_fetch (tdbb, window, lock, type, 0, 1, true)
#define CCH_FETCH_TIMEOUT(tdbb, window, lock, type, latch_wait)   CCH_fetch (tdbb, window, lock, type, 0, latch_wait, true)
#define CCH_FETCH_LOCK(tdbb, window, lock, wait, latch_wait, type) CCH_fetch_lock (tdbb, window, lock, wait, latch_wait, type)
#define CCH_FETCH_PAGE(tdbb, window, checksum, read_shadow)       CCH_fetch_page (tdbb, window, checksum, read_shadow)
#define CCH_RELEASE(tdbb, window)                         CCH_release (tdbb, window, false)
#define CCH_RELEASE_TAIL(tdbb, window)                    CCH_release (tdbb, window, true)
#define CCH_MARK(tdbb, window)                            CCH_mark (tdbb, window, 0)
#define CCH_MARK_SYSTEM(tdbb, window)                     CCH_mark (tdbb, window, 1)
#define CCH_HANDOFF(tdbb, window, page, lock, type)       CCH_handoff (tdbb, window, page, lock, type, 1, 0)
#define CCH_HANDOFF_TIMEOUT(tdbb, window, page, lock, type, latch_wait)   CCH_handoff (tdbb, window, page, lock, type, latch_wait, 0)
#define CCH_HANDOFF_TAIL(tdbb, window, page, lock, type)  CCH_handoff (tdbb, window, page, lock, type, 1, 1)
#define CCH_MARK_MUST_WRITE(tdbb, window)                 CCH_mark_must_write (tdbb, window)
#define CCH_PREFETCH(tdbb, pages, count)            CCH_prefetch (tdbb, pages, count)

/* Flush flags */

#define FLUSH_ALL	1			/* flush all dirty buffers in cache */
#define FLUSH_RLSE	2			/* release page locks after flush */
#define FLUSH_TRAN	4			/* flush transaction dirty buffers from dirty btree */
#define FLUSH_SWEEP	8			/* flush dirty buffers from garbage collection */
#define FLUSH_SYSTEM	16		/* flush system transaction only from dirty btree */
#define FLUSH_FINI	(FLUSH_ALL | FLUSH_RLSE)

#endif /* JRD_CCH_PROTO_H */

