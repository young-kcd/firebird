/*
 *	PROGRAM:	JRD access method
 *	MODULE:		nlm_thd.h
 *	DESCRIPTION:	NLM Thread support definitions
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

/* This include file turns certain V4 threading constructs on for NLM. */

#ifndef NETWARE_386
#define NLM_INIT
#define NLM_GLOBAL_MUTEX_LOCK
#define NLM_GLOBAL_MUTEX_UNLOCK
#define NLM_MUTEX_INIT(mutx)
#define NLM_MUTEX_LOCK(mutx)
#define NLM_MUTEX_UNLOCK(mutx)
#define NLM_MUTEX_DESTROY(mutx)
#define NLM_JRD_MUTEX_LOCK(mutx)
#define NLM_JRD_MUTEX_UNLOCK(mutx)
#else
#define NLM_INIT			THD_INIT
#define NLM_GLOBAL_MUTEX_LOCK		THD_GLOBAL_MUTEX_LOCK
#define NLM_GLOBAL_MUTEX_UNLOCK		THD_GLOBAL_MUTEX_UNLOCK
#define NLM_MUTEX_INIT(mutx)		THD_MUTEX_INIT (mutx)
#define NLM_MUTEX_LOCK(mutx)		THD_MUTEX_LOCK (mutx)
#define NLM_MUTEX_UNLOCK(mutx)		THD_MUTEX_UNLOCK (mutx)
#define NLM_MUTEX_DESTROY(mutx)		THD_MUTEX_DESTROY (mutx)
#define NLM_JRD_MUTEX_LOCK(mutx)	THD_JRD_MUTEX_LOCK (mutx)
#define NLM_JRD_MUTEX_UNLOCK(mutx)	THD_JRD_MUTEX_UNLOCK (mutx)

#ifdef V4_THREADING
#undef V4_INIT
#undef V4_GLOBAL_MUTEX_LOCK
#undef V4_GLOBAL_MUTEX_UNLOCK
#undef V4_MUTEX_INIT
#undef V4_MUTEX_LOCK
#undef V4_MUTEX_UNLOCK
#undef V4_MUTEX_DESTROY
#undef V4_JRD_MUTEX_LOCK
#undef V4_JRD_MUTEX_UNLOCK
#define V4_INIT				THD_INIT
#define V4_GLOBAL_MUTEX_LOCK		{THREAD_EXIT; THD_GLOBAL_MUTEX_LOCK; THREAD_ENTER;}
#define V4_GLOBAL_MUTEX_UNLOCK		THD_GLOBAL_MUTEX_UNLOCK
#define V4_MUTEX_INIT(mutx)		THD_MUTEX_INIT (mutx)
#define V4_MUTEX_LOCK(mutx)		{THREAD_EXIT; THD_MUTEX_LOCK (mutx); THREAD_ENTER;}
#define V4_MUTEX_UNLOCK(mutx)		THD_MUTEX_UNLOCK (mutx)
#define V4_MUTEX_DESTROY(mutx)		THD_MUTEX_DESTROY (mutx)
#define V4_JRD_MUTEX_LOCK(mutx)		{THREAD_EXIT; THD_JRD_MUTEX_LOCK (mutx); THREAD_ENTER;}
#define V4_JRD_MUTEX_UNLOCK(mutx)	THD_JRD_MUTEX_UNLOCK (mutx)
#endif
#endif
