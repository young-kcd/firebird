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

#ifndef _JRD_THD_PROTO_H_
#define _JRD_THD_PROTO_H_

#include "../jrd/isc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int API_ROUTINE gds__thread_start(FPTR_INT_VOID_PTR, void *, int, int,
										 void *);
extern long THD_get_thread_id(void);
extern struct thdd *DLL_EXPORT THD_get_specific(void);
extern void THD_getspecific_data(void **t_data);
extern void DLL_EXPORT THD_init(void);
extern void DLL_EXPORT THD_cleanup(void);
extern int DLL_EXPORT THD_mutex_destroy(struct mutx_t *);
extern int THD_mutex_init(struct mutx_t *);
extern int THD_mutex_lock(struct mutx_t *);
extern int THD_mutex_unlock(struct mutx_t *);
extern int THD_mutex_destroy_n(struct mutx_t *, USHORT);
extern int THD_mutex_init_n(struct mutx_t *, USHORT);
extern int THD_mutex_lock_global(void);
extern int THD_mutex_unlock_global(void);
extern void DLL_EXPORT THD_put_specific(struct thdd *);
extern void THD_putspecific_data(void *t_data);
#ifdef SUPERSERVER
extern int THD_rec_mutex_destroy(struct rec_mutx_t *);
extern int THD_rec_mutex_init(struct rec_mutx_t *);
extern int THD_rec_mutex_lock(struct rec_mutx_t *);
extern int THD_rec_mutex_unlock(struct rec_mutx_t *);
#endif
extern struct thdd *DLL_EXPORT THD_restore_specific(void);
extern int THD_resume(THD_T);
extern void THD_sleep(ULONG);
extern int THD_suspend(THD_T);
extern int DLL_EXPORT THD_wlck_destroy(struct wlck_t *);
extern int DLL_EXPORT THD_wlck_init(struct wlck_t *);
extern int DLL_EXPORT THD_wlck_lock(struct wlck_t *, USHORT);
extern int DLL_EXPORT THD_wlck_unlock(struct wlck_t *);
extern void THD_wlck_destroy_n(struct wlck_t *, USHORT);
extern void THD_wlck_init_n(struct wlck_t *, USHORT);
extern void THD_yield(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _JRD_THD_PROTO_H_ */
