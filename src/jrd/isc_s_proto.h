/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_s_proto.h
 *	DESCRIPTION:	Prototype header file for isc_sync.c
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _JRD_ISC_S_PROTO_H_
#define _JRD_ISC_S_PROTO_H_

#include "../jrd/isc.h"

#ifdef __cplusplus
extern "C" {
#endif


extern BOOLEAN ISC_check_restart(void);
extern int ISC_event_blocked(USHORT, struct event **, SLONG *);
extern SLONG DLL_EXPORT ISC_event_clear(struct event *);
extern void ISC_event_fini(struct event *);
extern int DLL_EXPORT ISC_event_init(struct event *, int, int);

#if defined(WIN_NT)
extern int ISC_event_init_shared(struct event *,
								 int, TEXT *, struct event *, USHORT);
#endif
extern int DLL_EXPORT ISC_event_post(struct event *);
extern int DLL_EXPORT ISC_event_wait(SSHORT,
									 struct event **,
									 SLONG *, SLONG, FPTR_VOID, void *);

#ifdef WIN_NT
extern void *ISC_make_signal(BOOLEAN, BOOLEAN, int, int);
#endif /* WIN_NT */

extern UCHAR *DLL_EXPORT ISC_map_file(ISC_STATUS *,
									  TEXT *,
									  void (*)(void *, struct sh_mem *, int),
									  void *, SLONG, struct sh_mem *);
#if defined(WIN_NT)
int DLL_EXPORT ISC_mutex_init(struct mtx *, TEXT*);
#else
int DLL_EXPORT ISC_mutex_init(struct mtx *, SLONG);
#endif

int DLL_EXPORT ISC_mutex_lock(struct mtx *);
int DLL_EXPORT ISC_mutex_lock_cond(struct mtx *);
int DLL_EXPORT ISC_mutex_unlock(struct mtx *);

#ifdef HAVE_MMAP
extern UCHAR *ISC_map_object(ISC_STATUS *, SH_MEM, SLONG, SLONG);
extern BOOLEAN ISC_unmap_object(ISC_STATUS *, SH_MEM, UCHAR **, SLONG);
#endif

#ifdef SUPERSERVER
#ifdef UNIX
extern void ISC_exception_post(ULONG, TEXT *);
extern void ISC_sync_signals_set(void);
extern void ISC_sync_signals_reset(void);
#endif /* UNIX */

#ifdef WIN_NT
extern ULONG ISC_exception_post(ULONG, TEXT *);
#endif /* WIN_NT */

#endif /* SUPERSERVER */

extern UCHAR *DLL_EXPORT ISC_remap_file(ISC_STATUS *,
										struct sh_mem *, SLONG, USHORT);
extern void ISC_reset_timer(FPTR_VOID, void *, SLONG *, void **);
extern void ISC_set_timer(SLONG, FPTR_VOID, void *, SLONG *, void **);
extern void DLL_EXPORT ISC_unmap_file(ISC_STATUS *, struct sh_mem *, USHORT);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* _JRD_ISC_S_PROTO_H_ */
