/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd_priority.h
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

#ifndef _JRD_OS_THD_PRIORITY_H_
#define _JRD_OS_THD_PRIORITY_H_

#ifdef WIN_NT
#if defined(SUPERSERVER) && !defined(EMBEDDED)
// Comment this definition to build without priority scheduler 
//	OR:
// Uncomment this definition to build with priority scheduler
#define THREAD_PSCHED
#endif
#endif

#ifdef THREAD_PSCHED

#include "../jrd/thd.h"
#include "../common/classes/alloc.h"
#include <process.h>
#include <windows.h>

// Each thread, issuing THREAD_EXIT / THREAD_ENTER, must
// have associated with it THPS class. That is
// what thread local storage points to, instead of 
// struct thdd. We keep pointer to this struct inside this 
// class.
class ThreadPriorityScheduler {
private:
	static MUTX_T mutex;			// locks modification of thps chains
	static MemoryPool * pool;		// where we should place our thps
	static ThreadPriorityScheduler * chain;	// where starts thps chain
	static ThreadPriorityScheduler * news;	// where starts new thps chain
	static BOOLEAN initialized;
	static DWORD specific_key;		// for thread LS access
	static BOOLEAN shutdown;		// server shutting down

	ThreadPriorityScheduler * next;			// next thread in list
	union {
	struct thdd *context;	// current context
	DWORD id;				// ID on startup
	};
    HANDLE handle;			// thread handle for SetPriority
	USHORT ticks;			// ticks left till keepalive verification
// use separate bytes for flags in order to guarantee there 
// modification independence by different threads without
// affecting each other
	UCHAR inside;			// executing between ENTER and EXIT
	UCHAR goneout;			// pass through EXIT since last scheduling
	UCHAR gonein;			// pass through ENTER since last scheduling
	UCHAR flags;			// flags that can't be modified concurrently
	static ThreadPriorityScheduler *Attach(void);
	static unsigned int __stdcall Scheduler(LPVOID);
	static ThreadPriorityScheduler *InternalGet(void);
public:
	static void Enter(void);
	static void Exit(void);
	static thdd *Get(void);
	static void Set(thdd *val);
	static BOOLEAN Boosted(void);
	static void Cleanup(void);
	static void Init(void);
	static void Attach(HANDLE tHandle, DWORD thread_id, int &p);
	static void Attach(HANDLE tHandle, DWORD thread_id, UCHAR flags);
};

// thps_flags values
#define THPS_PSCHED		1		// thread controlled by priority scheduler
#define THPS_UP			2		// candidate for priority boost
#define THPS_LOW		4		// candidate for priority decrement
#define THPS_BOOSTED	8		// thread controlled by priority scheduler

#define THPS_ENTER() ThreadPriorityScheduler::Enter()
#define THPS_EXIT() ThreadPriorityScheduler::Exit()
#define THPS_GET(WhenMissing) ThreadPriorityScheduler::Get()
#define THPS_SET(WhenMissing, val) ThreadPriorityScheduler::Set(val)
#define THPS_INIT() ThreadPriorityScheduler::Init()
#define THPS_FINI() ThreadPriorityScheduler::Cleanup()
#define THPS_ATTACH(handle, thread_id, priority) \
		ThreadPriorityScheduler::Attach(handle, thread_id, priority)
#define THPS_BOOSTDONE() ThreadPriorityScheduler::Boosted()

#else // THREAD_PSCHED

#define THPS_ENTER()
#define THPS_EXIT()
#define THPS_GET(WhenMissing) WhenMissing
#define THPS_SET(WhenMissing, val) WhenMissing
#define THPS_INIT()
#define THPS_FINI()
#define THPS_ATTACH(handle, thread_id, priority)
#define THPS_BOOSTDONE() FALSE

#endif // THREAD_PSCHED

#endif // _JRD_OS_THD_PRIORITY_H_
