/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_s_proto.h
 *	DESCRIPTION:	Prototype header file for isc_sync.cpp
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

#ifndef JRD_ISC_S_PROTO_H
#define JRD_ISC_S_PROTO_H

#include "../jrd/isc.h"
BOOLEAN	ISC_check_restart(void);
int		ISC_event_blocked(USHORT, struct event_t **, SLONG *);
SLONG	ISC_event_clear(struct event_t *);
void	ISC_event_fini(struct event_t *);
int		ISC_event_init(struct event_t *, int, int);

#if defined(WIN_NT)
int		ISC_event_init_shared(struct event_t*, int, const TEXT*, struct event_t*, bool);
#endif
int		ISC_event_post(struct event_t *);
int		ISC_event_wait(SSHORT, struct event_t **, SLONG *, SLONG, FPTR_VOID_PTR, void *);

#ifdef WIN_NT
void*	ISC_make_signal(bool, bool, int, int);
#endif /* WIN_NT */

typedef void (*FPTR_INIT_GLOBAL_REGION)(void*, struct sh_mem*, bool);
UCHAR*	ISC_map_file(ISC_STATUS*, const TEXT*, FPTR_INIT_GLOBAL_REGION,
					void*, SLONG, struct sh_mem*);
#if defined(WIN_NT)
int		ISC_mutex_init(struct mtx*, const TEXT*);
#else
int		ISC_mutex_init(struct mtx*, SLONG);
#endif

int		ISC_mutex_lock(struct mtx *);
int		ISC_mutex_lock_cond(struct mtx *);
int		ISC_mutex_unlock(struct mtx *);

#ifdef HAVE_MMAP
UCHAR*	ISC_map_object(ISC_STATUS *, SH_MEM, SLONG, SLONG);
void	ISC_unmap_object(ISC_STATUS *, SH_MEM, UCHAR **, SLONG);
#endif

#ifdef SUPERSERVER
#ifdef UNIX
void	ISC_exception_post(ULONG, const TEXT*);
void	ISC_sync_signals_set(void);
void	ISC_sync_signals_reset(void);
#endif /* UNIX */

#ifdef WIN_NT
ULONG	ISC_exception_post(ULONG, const TEXT*);
#endif /* WIN_NT */

#endif /* SUPERSERVER */

UCHAR*	ISC_remap_file(ISC_STATUS *, struct sh_mem *, SLONG, bool);
void	ISC_reset_timer(FPTR_VOID_PTR, void *, SLONG *, void **);
void	ISC_set_timer(SLONG, FPTR_VOID_PTR, void *, SLONG *, void **);
void	ISC_unmap_file(ISC_STATUS *, struct sh_mem *, USHORT);


#endif // JRD_ISC_S_PROTO_H
