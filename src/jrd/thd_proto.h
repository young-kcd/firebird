/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd_proto.h
 *	DESCRIPTION:	Prototype header file for thd.c
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

#ifndef JRD_THD_PROTO_H
#define JRD_THD_PROTO_H

#include "../jrd/isc.h"

extern "C" {
int		API_ROUTINE gds__thread_start(FPTR_INT_VOID_PTR, void*, int, int,
										 void*);
}

struct thdd* THD_get_specific(void);
void	THD_init(void);
void	THD_cleanup(void);
int		THD_mutex_destroy(struct mutx_t*);
void	THD_put_specific(struct thdd*);
int		THD_wlck_destroy(struct wlck_t*);
int		THD_wlck_init(struct wlck_t*);
int		THD_wlck_lock(struct wlck_t*, USHORT);
int		THD_wlck_unlock(struct wlck_t*);

long	THD_get_thread_id(void);
void	THD_getspecific_data(void** t_data);

int		THD_mutex_init(struct mutx_t*);
int		THD_mutex_lock(struct mutx_t*);
int		THD_mutex_unlock(struct mutx_t*);
int		THD_mutex_destroy_n(struct mutx_t*, USHORT);
int		THD_mutex_init_n(struct mutx_t*, USHORT);
int		THD_mutex_lock_global(void);
int		THD_mutex_unlock_global(void);
void	THD_putspecific_data(void* t_data);

#ifdef SUPERSERVER
int		THD_rec_mutex_destroy(struct rec_mutx_t*);
int		THD_rec_mutex_init(struct rec_mutx_t*);
int		THD_rec_mutex_lock(struct rec_mutx_t*);
int		THD_rec_mutex_unlock(struct rec_mutx_t*);
#endif

struct thdd* THD_restore_specific(void);
int		THD_resume(THD_T);
void	THD_sleep(ULONG);
int		THD_suspend(THD_T);
void	THD_wlck_destroy_n(struct wlck_t*, USHORT);
void	THD_wlck_init_n(struct wlck_t*, USHORT);
void	THD_yield(void);

#endif // JRD_THD_PROTO_H

