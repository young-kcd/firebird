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

#ifndef JRD_OS_THD_PRIORITY_H
#define JRD_OS_THD_PRIORITY_H

#ifdef WIN_NT
#include <windows.h>
#if defined(SUPERSERVER) && !defined(EMBEDDED)
// Comment this definition to build without priority scheduler 
//	OR:
// Uncomment this definition to build with priority scheduler
#define THREAD_PSCHED
#endif
#endif

#include "../jrd/thd.h"

#ifdef THREAD_PSCHED

#include "../common/classes/alloc.h"
#include <process.h>

// Each thread, issuing THREAD_EXIT / THREAD_ENTER, must
// have associated with it THPS class. That is
// what thread local storage points to, instead of 
// struct thdd. We keep pointer to this struct inside this 
// class.
class ThreadPriorityScheduler {
private:
	static MUTX_T mutex;			// locks modification of thps chains
	static MemoryPool* pool;		// where we should place our thps
	static ThreadPriorityScheduler* chain;	// where starts thps chain
	static ThreadPriorityScheduler* news;	// where starts new thps chain
	static bool initialized;
	static DWORD specific_key;		// for thread LS access
	static bool shutdown;		// server shutting down

	ThreadPriorityScheduler* next;			// next thread in list
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
	static bool Boosted(void);
	static void Cleanup(void);
	static void Init(void);
	static void Attach(HANDLE tHandle, DWORD thread_id, int &p);
	static void Attach(HANDLE tHandle, DWORD thread_id, UCHAR flags);
};

// thps_flags values
const UCHAR THPS_PSCHED		= 1;	// thread controlled by priority scheduler
const UCHAR THPS_UP			= 2;	// candidate for priority boost
const UCHAR THPS_LOW		= 4;	// candidate for priority decrement
const UCHAR THPS_BOOSTED	= 8;	// thread controlled by priority scheduler

inline void THPS_ENTER() {
	ThreadPriorityScheduler::Enter();
}
inline void THPS_EXIT() {
	ThreadPriorityScheduler::Exit();
}
inline thdd* THPS_GET(DWORD specific_key) {
	return (thdd*) ThreadPriorityScheduler::Get();
}
inline void THPS_SET(DWORD specific_key, thdd* new_context) {
	ThreadPriorityScheduler::Set(new_context);
}
inline void THPS_INIT() {
	ThreadPriorityScheduler::Init();
}
inline void THPS_FINI() {
	ThreadPriorityScheduler::Cleanup();
}
inline void THPS_ATTACH(HANDLE handle, DWORD thread_id, int priority) {
	ThreadPriorityScheduler::Attach(handle, thread_id, priority);
}
inline bool THPS_BOOSTDONE() {
	return ThreadPriorityScheduler::Boosted();
}

#else // THREAD_PSCHED

inline void THPS_ENTER() {
}
inline void THPS_EXIT() {
}
#ifdef WIN_NT
inline thdd* THPS_GET(DWORD specific_key) {
	return (thdd*) TlsGetValue(specific_key);
}
inline void THPS_SET(DWORD specific_key, thdd* new_context) {
	TlsSetValue(specific_key, (LPVOID) new_context);
}
inline void THPS_ATTACH(HANDLE handle, DWORD thread_id, int priority) {
}
#endif
inline void THPS_INIT() {
}
inline void THPS_FINI() {
}
inline bool THPS_BOOSTDONE() {
	return false;
}

#endif // THREAD_PSCHED

#endif // JRD_OS_THD_PRIORITY_H

