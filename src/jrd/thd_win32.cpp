/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd.c
 *	DESCRIPTION:	Thread support routines
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <errno.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../jrd/thd_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/gdsassert.h"

#include <process.h>
#include <windows.h>


extern "C" {

static void init(void);
static void init_tkey(void);
static void put_specific(THDD);
static int thread_start(int (*)(void *), void *, int, int, void *);

static USHORT	initialized = FALSE;
static USHORT	t_init = FALSE;

static MUTX_T	ib_mutex;
static DWORD	specific_key;
static DWORD	t_key;


//____________________________________________________________
//
// Start a thread.
//
int API_ROUTINE gds__thread_start(FPTR_INT_VOID_PTR	entrypoint,
								  void*				arg,
								  int				priority,
								  int				flags,
								  void*				thd_id)
{
	return thread_start(entrypoint, arg, priority, flags, thd_id);
}


//____________________________________________________________
//
// Get platform's notion of a thread ID.
//
long THD_get_thread_id(void)
{
	return GetCurrentThreadId();
}


//____________________________________________________________
//
//
//
THDD THD_get_specific(void)
{
	return (THDD) TlsGetValue(specific_key);
}


//____________________________________________________________
//
// return the previously stored t_data.
//
void THD_getspecific_data(void **t_data)
{
/* There are some circumstances in which we do not call THD_putspecific_data(),
   such as services API, and local access on NT. As result of that, t_init
   does not get initialized. So don't use an assert in here but rather do
   the work only if t_init is initialised */
	if (t_init) {
		*t_data = (void *) TlsGetValue(t_key);
	}
}


//____________________________________________________________
//
// This is the cleanup function called from the DLL
// cleanup function. This helps to remove the allocated
// thread specific key.
//
void DLL_EXPORT THD_cleanup(void)
{
	if (initialized)
	{
		initialized = FALSE;
		TlsFree(specific_key);

		/* destroy the mutex ib_mutex which was created */
		THD_mutex_destroy(&ib_mutex);
	}
}


//____________________________________________________________
//
//
void DLL_EXPORT THD_init(void)
{
	init();
}


//____________________________________________________________
//
// init function for t_key. This is called 
// to ensure that the key is created.
//
void DLL_EXPORT THD_init_data(void)
{
	init_tkey();
}


//____________________________________________________________
//
//
int THD_mutex_destroy(MUTX_T* mutex)
{

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")

	DeleteCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(mutex));

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_init(MUTX_T * mutex)
{

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	InitializeCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(mutex));

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_lock(MUTX_T * mutex)
{

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	EnterCriticalSection(reinterpret_cast < LPCRITICAL_SECTION > (mutex));

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_unlock(MUTX_T * mutex)
{

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	LeaveCriticalSection(reinterpret_cast < LPCRITICAL_SECTION > (mutex));

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_destroy_n(MUTX_T* mutexes, USHORT n)
{
	while (n--)
	{
		THD_mutex_destroy(mutexes++);
	}

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_init_n(MUTX_T* mutexes, USHORT n)
{
	while (n--)
	{
		THD_mutex_init(mutexes++);
	}

	return 0;
}


//____________________________________________________________
//
//
int THD_mutex_lock_global(void)
{
	return THD_mutex_lock(&ib_mutex);
}


//____________________________________________________________
//
//
int THD_mutex_unlock_global(void)
{
	return THD_mutex_unlock(&ib_mutex);
}


//____________________________________________________________
//
//
int DLL_EXPORT THD_wlck_destroy(WLCK_T * wlock)
{
	return 0;
}


//____________________________________________________________
//
//
int DLL_EXPORT THD_wlck_init(WLCK_T * wlock)
{
	return 0;
}


//____________________________________________________________
//
//
int DLL_EXPORT THD_wlck_lock(WLCK_T * wlock, USHORT type)
{
	return 0;
}


//____________________________________________________________
//
//
int DLL_EXPORT THD_wlck_unlock(WLCK_T * wlock)
{
	return 0;
}


//____________________________________________________________
//
//
void THD_wlck_destroy_n(WLCK_T * wlocks, USHORT n)
{
	while (n--) {
		THD_wlck_destroy(wlocks++);
	}
}


//____________________________________________________________
//
//
void THD_wlck_init_n(WLCK_T * wlocks, USHORT n)
{
	while (n--) {
		THD_wlck_init(wlocks++);
	}
}


//____________________________________________________________
//
//
void DLL_EXPORT THD_put_specific(THDD new_context)
{
	if (!initialized) {
		THD_init();
	}

	// Save the current context

	new_context->thdd_prior_context = THD_get_specific();

	put_specific(new_context);
}


//____________________________________________________________
//
// Store the passed t_data using the ket t_key
//
void DLL_EXPORT THD_putspecific_data(void *t_data)
{
	if (!t_init) {
		THD_init_data();
	}

	TlsSetValue(t_key, (LPVOID) t_data);
}


//____________________________________________________________
//
//
//
THDD DLL_EXPORT THD_restore_specific(void)
{
	THDD current_context = THD_get_specific();

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	put_specific(reinterpret_cast<THDD>(current_context->thdd_prior_context));

	return reinterpret_cast<THDD>(current_context->thdd_prior_context);
}


#ifdef SUPERSERVER
//____________________________________________________________
//
//
//
int THD_rec_mutex_destroy(REC_MUTX_T * rec_mutex)
{
	return THD_mutex_destroy(rec_mutex->rec_mutx_mtx);
}


//____________________________________________________________
//
//
//
int THD_rec_mutex_init(REC_MUTX_T * rec_mutex)
{
	THD_mutex_init(rec_mutex->rec_mutx_mtx);
	rec_mutex->rec_mutx_id = 0;
	rec_mutex->rec_mutx_count = 0;
	return 0;
}


//____________________________________________________________
//
//
//
int THD_rec_mutex_lock(REC_MUTX_T * rec_mutex)
{
	if (rec_mutex->rec_mutx_id == THD_get_thread_id())
	{
		rec_mutex->rec_mutx_count++;
	} else
	{
		const int ret = THD_mutex_lock(rec_mutex->rec_mutx_mtx);
		if (ret)
		{
			return ret;
		}
		rec_mutex->rec_mutx_id = THD_get_thread_id();
		rec_mutex->rec_mutx_count = 1;
	}
	return 0;
}


//____________________________________________________________
//
//
//
int THD_rec_mutex_unlock(REC_MUTX_T * rec_mutex)
{
	if (rec_mutex->rec_mutx_id != THD_get_thread_id())
		return FAILURE;

	rec_mutex->rec_mutx_count--;

	if (rec_mutex->rec_mutx_count)
		return 0;
	else {
		rec_mutex->rec_mutx_id = 0;
		return THD_mutex_unlock(rec_mutex->rec_mutx_mtx);
	}
}
#endif /* SUPERSERVER */


//____________________________________________________________
//
// Resume execution of a thread that has been suspended.
//
int THD_resume(THD_T thread)
{
	if (ResumeThread(thread) == 0xFFFFFFFF) {
		return GetLastError();
	}

	return 0;
}


//____________________________________________________________
//
// Suspend execution of a thread.
//
int THD_suspend(THD_T thread)
{
	if (SuspendThread(thread) == 0xFFFFFFFF) {
		return GetLastError();
	}

	return 0;
}


//____________________________________________________________
//
// Thread sleeps for requested number of milliseconds.
//
void THD_sleep(ULONG milliseconds)
{
	SleepEx(milliseconds, FALSE);
}


//____________________________________________________________
//
// Thread relinquishes CPU.
//
void THD_yield(void)
{
	SleepEx(1, FALSE);
}


//____________________________________________________________
//
//
//
static void init(void)
{
	if (initialized) {
		return;
	}

	initialized = TRUE;

	THD_mutex_init(&ib_mutex);

	specific_key = TlsAlloc();

#pragma FB_COMPILER_MESSAGE("Fix! Bad function ptr type cast.")
	gds__register_cleanup(reinterpret_cast < FPTR_VOID_PTR > (THD_cleanup),
						  0);
}


//____________________________________________________________
//
// Function which actually creates the key which
// can be used by the threads to store t_data
//
static void init_tkey(void)
{
	if (t_init) {
		return;
	}

	t_init = TRUE;
	t_key = TlsAlloc();
}


//____________________________________________________________
//
//
//
static void put_specific(THDD new_context)
{
	TlsSetValue(specific_key, (LPVOID) new_context);
}


#if !defined(__BORLANDC__)
// Copyright (C) 2001 Mikael Nordell
// Need a thunking layer from stdcall to cdecl, and also to serve as
// an adapter for mismatch in return type.
//
struct thunk_thread_start_t
{
	int		(*pfn)(void*);
	void*	arg;
};

static unsigned int __stdcall win32_thread_start_thunk(void* p)
{
	const thunk_thread_start_t* pArg =
		reinterpret_cast<const thunk_thread_start_t*>(p);
	int		(*pfn)(void*)	= pArg->pfn;
	void* arg				= pArg->arg;
	free(p);
	return (unsigned int) ((*pfn)(arg));
}
#endif


//____________________________________________________________
//
// Start a new thread.  Return 0 if successful, status if not.
//
static int thread_start(FPTR_INT_VOID_PTR	routine,
						void*				arg,
						int					priority_arg,
						int					flags,
						void*				thd_id)
{
	HANDLE handle;
	DWORD thread_id;
	int priority;

	DWORD create = (flags & THREAD_wait) ? CREATE_SUSPENDED : 0;

#ifdef __BORLANDC__
	handle =
		(HANDLE) _beginthreadNT((void (_USERENTRY *) (void *)) routine, 0,
								arg, NULL, create, &thread_id);

	if (handle == (HANDLE) - 1) {
		return errno;
	}
#else
	/* I have changed the CreateThread here to _beginthreadex() as using
	 * CreateThread() can lead to memory leaks caused by C-runtime library.
	 * Advanced Windows by Richter pg. # 109. */

#if 0
#pragma FB_COMPILER_MESSAGE("Fix! Bad, bad, bad function ptr type cast!!!")

	thunk_thread_start_t* thread_start_arg =
		(thunk_thread_start_t*)malloc(sizeof(thunk_thread_start_t));
	thread_start_arg->pfn = routine;
	thread_start_arg->arg = arg;
	unsigned long real_handle = _beginthreadex(NULL,
											   0,
											   &win32_thread_start_thunk,
											   thread_start_arg,
											   create,
											   reinterpret_cast<unsigned*>(&thread_id));
#else
	unsigned long real_handle = _beginthreadex(NULL,
											   0,
											   reinterpret_cast<unsigned int(__stdcall*)(void*)>(routine),
											   arg,
											   create,
											   reinterpret_cast<unsigned*>(&thread_id));
#endif

	if (!real_handle) {
		return GetLastError();
	}
	handle = reinterpret_cast<HANDLE>(real_handle);

#endif

	switch (priority_arg)
	{
		case THREAD_critical:
			priority = THREAD_PRIORITY_TIME_CRITICAL;
			break;
		case THREAD_high:
			priority = THREAD_PRIORITY_HIGHEST;
			break;
		case THREAD_medium_high:
			priority = THREAD_PRIORITY_ABOVE_NORMAL;
			break;
		case THREAD_medium:
			priority = THREAD_PRIORITY_NORMAL;
			break;
		case THREAD_medium_low:
			priority = THREAD_PRIORITY_BELOW_NORMAL;
			break;
		case THREAD_low:
		default:
			priority = THREAD_PRIORITY_LOWEST;
			break;
	}

	SetThreadPriority(handle, priority);
	if (thd_id) {
		*(HANDLE *) thd_id = handle;
	} else {
		CloseHandle(handle);
	}

	return 0;
}


} // extern "C"
