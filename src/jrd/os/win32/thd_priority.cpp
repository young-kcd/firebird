/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd_priority.cpp
 *	DESCRIPTION:	Thread priorities scheduler
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
 * 2002.10.20 Alexander Peshkoff: Created this scheduler, changing
 *   priorities of Win32 threads. to avoid side effects of Windows
 *   native priorities scheduling.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/os/thd_priority.h"
#include "../common/config/config.h"

#ifdef THREAD_PSCHED

// configurable parameters
#define THPS_TIME (Config::getPrioritySwitchDelay())	// ms between rescheds
#define THPS_TICKS (Config::getDeadThreadsCollection())	// sched loops before thread killing

#include "../jrd/ib_stdio.h"
#include <errno.h>
#include "../jrd/common.h"
#include "../jrd/thd_proto.h"

#include <process.h>
#include <windows.h>

// #define DEBUG_THREAD_PSCHED

#ifdef DEBUG_THREAD_PSCHED
#include "../jrd/gds_proto.h"
#endif

MUTX_T ThreadPriorityScheduler::mutex;
MemoryPool * ThreadPriorityScheduler::pool = 0;
ThreadPriorityScheduler * ThreadPriorityScheduler::chain = 0;
ThreadPriorityScheduler * ThreadPriorityScheduler::news = 0;
bool ThreadPriorityScheduler::initialized = false;
DWORD ThreadPriorityScheduler::specific_key = (DWORD) -1;
bool ThreadPriorityScheduler::shutdown = false;

//____________________________________________________________
//
// Shutdown 
//
void ThreadPriorityScheduler::Cleanup(void) {
	if (initialized)
	{
		initialized = false;
		shutdown = true;
	}
}

//____________________________________________________________
//
// Returns thdd for current thread
//
thdd* ThreadPriorityScheduler::Get(void) {
	return InternalGet()->context;
}

//____________________________________________________________
//
// Returns thps for current thread
//
ThreadPriorityScheduler * ThreadPriorityScheduler::InternalGet(void) {
	ThreadPriorityScheduler *tps = 
		(ThreadPriorityScheduler *)TlsGetValue(specific_key);
	if (! tps)
		tps = Attach();
	return tps;
}

//____________________________________________________________
//
// Sets thdd for current thread
//
void ThreadPriorityScheduler::Set(thdd *val) {
	InternalGet()->context = val;
}

//____________________________________________________________
//
// Initialize priority scheduler
//
void ThreadPriorityScheduler::Init(void)
{
	if (initialized)
		return;

	initialized = true;
	specific_key = TlsAlloc();

	// memory pool for thps allocation
	pool = getDefaultMemoryPool();

	// mutex to sync linked list operations
	THD_mutex_init(&mutex);

	// allocate thps for current thread
	Attach(GetCurrentThread(), GetCurrentThreadId(), (UCHAR)0);

	// start scheduler
	unsigned thread_id;
	HANDLE real_handle = (HANDLE)_beginthreadex(NULL, 0,
		Scheduler, 0, 0, &thread_id);
	if (! real_handle)
		Firebird::system_call_failed::raise();
	SetThreadPriority(real_handle, THREAD_PRIORITY_TIME_CRITICAL);
	CloseHandle(real_handle);
}


//____________________________________________________________
//
// Allocate instance of thps class from pool
//
void ThreadPriorityScheduler::Attach(HANDLE tHandle, DWORD thread_id, int &p) {
#ifdef DEV_BUILD
	if (! pool)
		Firebird::fatal_exception::raise("Missing pool in ThreadPriorityScheduler");
#endif

	UCHAR flags = 0;
	if (p == THREAD_PRIORITY_NORMAL) {
		flags = THPS_PSCHED | THPS_BOOSTED;
		p = THREAD_PRIORITY_HIGHEST;
	}
	Attach(tHandle, thread_id, flags);
}

//____________________________________________________________
//
// Allocate instance of thps class from pool
//
void ThreadPriorityScheduler::Attach(HANDLE tHandle, DWORD thread_id, UCHAR flags) {
	ThreadPriorityScheduler * m = 
				FB_NEW(*pool) ThreadPriorityScheduler;
	m->next = 0;
	m->id = thread_id;
	m->ticks = THPS_TICKS;
	m->inside = 0;
	m->goneout = 0;
	m->gonein = 0;
	m->flags = flags;

	HANDLE process = GetCurrentProcess();
	if (! DuplicateHandle(process, tHandle, 
				process, &m->handle, 0, 
				FALSE, DUPLICATE_SAME_ACCESS))
		Firebird::system_call_failed::raise();

	THD_mutex_lock(&mutex);
	m->next = news;
	news = m;
	THD_mutex_unlock(&mutex);
}

//____________________________________________________________
//
// Finds instance of thps class for current thread
//
ThreadPriorityScheduler * ThreadPriorityScheduler::Attach(void) {
	DWORD my_id = GetCurrentThreadId();
	THD_mutex_lock(&mutex);
	for (ThreadPriorityScheduler **pt = &news; *pt; pt = &(*pt)->next)
		if ((*pt)->id == my_id) {
			ThreadPriorityScheduler *m = *pt;
			*pt = m->next;
			m->next = chain;
			m->context = 0;
			chain = m;
			TlsSetValue(specific_key, m);
			THD_mutex_unlock(&mutex);
#ifdef DEBUG_THREAD_PSCHED
			gds__log("^ handle=%p priority=%d", m->handle, 
				m->flags & THPS_BOOSTED ? 
				THREAD_PRIORITY_HIGHEST : THREAD_PRIORITY_NORMAL);
#endif
			return m;
		}
	THD_mutex_unlock(&mutex);
	Firebird::fatal_exception::raise("Unknown thread tried to attach to ThreadPriorityScheduler");
	// Never get - avoid warnings
	return 0;
}

//____________________________________________________________
//
// Goes to low priority zone
//
void ThreadPriorityScheduler::Enter(void) {
	ThreadPriorityScheduler *t = InternalGet();
	t->inside = 1;
	t->gonein = 1;
}

//____________________________________________________________
//
// Goes from low priority zone
//
void ThreadPriorityScheduler::Exit(void) {
	ThreadPriorityScheduler *t = InternalGet();
	t->ticks = THPS_TICKS;
	t->inside = 0;
	t->goneout = 1;
}

//____________________________________________________________
//
// Check whether current thread has high priority
//
bool ThreadPriorityScheduler::Boosted(void) {
	return InternalGet()->flags & THPS_BOOSTED ? true : false;
}

//____________________________________________________________
//
// Scheduler Thread
//
unsigned int __stdcall ThreadPriorityScheduler::Scheduler(LPVOID) {
	static int GlobalTicks = THPS_TICKS;
	for (;;) {
		Sleep(THPS_TIME);
		UCHAR StateCloseHandles = 0;
		// we needn't lock mutex, because we don't modify
		// next here, and new thps object may be added
		// only in the beginning of the chain - even it it 
		// happens, we safely ignore it here
		for (ThreadPriorityScheduler *t = chain; t; t = t->next) {
			UCHAR p_flags = t->flags;
			if (p_flags & THPS_PSCHED) {
				UCHAR gonein = t->gonein;
//				UCHAR goneout = t->goneout;
				t->gonein = 0;
				t->goneout = 0;
				t->flags &= ~(THPS_UP | THPS_LOW); // clean them
#pragma FB_COMPILER_MESSAGE("Fix! May have problems with long running UDFs.")
				if ((! gonein) && (! (p_flags & THPS_BOOSTED))) {
					if (p_flags & THPS_UP) {
				// 1.	thread exited single thread zone and didn't 
				//		return into it since this &last cycle:
				//			increase priority
						if (! SetThreadPriority(t->handle, 
									THREAD_PRIORITY_HIGHEST))
							Firebird::system_call_failed::raise();

#ifdef DEBUG_THREAD_PSCHED
						gds__log("+ handle=%p priority=%d", t->handle, THREAD_PRIORITY_HIGHEST);
#endif
						t->flags |= THPS_BOOSTED;
						continue;
					}
				// 2.	thread exited single thread zone
				//		and never returned there during this cycle:
				//			candidate for priority increase
					t->flags |= THPS_UP;
					continue;
				}
				if ((gonein || t->inside) && (p_flags & THPS_BOOSTED)) {
					if (p_flags & THPS_LOW) {
				// 3.	thread entered single thread zone
				//		last cycle and didn't leave it completely
				//		this cycle:
				//		decrease priority
						if (! SetThreadPriority(t->handle, 
									THREAD_PRIORITY_NORMAL))
							Firebird::system_call_failed::raise();
#ifdef DEBUG_THREAD_PSCHED
						gds__log("- handle=%p priority=%d", t->handle, THREAD_PRIORITY_NORMAL);
#endif
						t->flags &= ~THPS_BOOSTED;
						continue;
					}
				// 4.	thread entered single thread zone this cycle:
				//		candidate for priority decrease
					t->flags |= THPS_LOW;
					continue;
				}
			}
			// no such events happened - should we check for dead thread ?
			if (! t->inside) {
				if (t->ticks <= 0)
					StateCloseHandles = 1;
				else
					// Safe - if this thread modifies ticks,
					// ticks may get THPS_TICKS or THPS_TICKS-1,
					// or t->ticks - 1. Worst case -
					// extra check for ended thread.
					t->ticks--;
			}
		}
		if (! shutdown) {
			if (GlobalTicks-- > 0)
				continue;
			GlobalTicks = THPS_TICKS;
		}

		if (StateCloseHandles > 0) {
			for (ThreadPriorityScheduler ** pt = &chain; *pt; pt = &(*pt)->next) {
start_label:
				if ((*pt)->inside)
					continue;
				if ((*pt)->ticks > 0)
					continue;
				DWORD ExitCode;
				if (! GetExitCodeThread((*pt)->handle, &ExitCode)) 
					Firebird::system_call_failed::raise();
				if (ExitCode == STILL_ACTIVE) {
					(*pt)->ticks = THPS_TICKS;
					continue;
				}
				// thread exited - close handle and do cleanup
				if (StateCloseHandles == 1) {
					THD_mutex_lock(&mutex);
					StateCloseHandles = 2;
				}
				ThreadPriorityScheduler *m = *pt;
				*pt = m->next;
#ifdef DEBUG_THREAD_PSCHED
				gds__log("~ handle=%p", m->handle);
#endif
				CloseHandle(m->handle);
				delete m;
				if (*pt)
					goto start_label;
				else
					break;
			}
			if (StateCloseHandles == 2)
				THD_mutex_unlock(&mutex);
		}
		if (shutdown && (! chain) && (! news))
			break;
	}
	// cleanup
	THD_mutex_destroy(&mutex);
	TlsFree(specific_key);
	return 0;
}

#endif //THREAD_PSCHED
