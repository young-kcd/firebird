/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ThreadData.cpp
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
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <errno.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../jrd/os/thd_priority.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/gdsassert.h"
#include "../common/classes/fb_tls.h"


#ifdef WIN_NT
#include <process.h>
#include <windows.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#ifdef SOLARIS_MT
#include <thread.h>
#include <signal.h>
#endif

#ifdef NOT_USED_OR_REPLACED
#ifdef VMS
// THE SOLE PURPOSE OF THE FOLLOWING DECLARATION IS TO ALLOW THE VMS KIT TO
// COMPILE.  IT IS NOT CORRECT AND MUST BE REMOVED AT SOME POINT.
ThreadData* gdbb;
#endif
#endif

#include "../common/classes/locks.h"
#include "../common/classes/rwlock.h"
Firebird::Mutex ib_mutex;

namespace {

TLS_DECLARE (void*, tSpecific);
TLS_DECLARE (ThreadData*, tData);

}


int API_ROUTINE gds__thread_start(
				  ThreadEntryPoint* entrypoint,
				  void *arg,
				  int priority, int flags, void *thd_id)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start a thread.
 *
 **************************************/

	int rc = 0;
	try {
		ThreadData::start(entrypoint, arg, priority, flags, thd_id);
	}
	catch(const Firebird::status_exception& status) {
		rc = status.value()[1];
	}
	return rc;
}


FB_THREAD_ID ThreadData::getId(void)
{
/**************************************
 *
 *	T H D _ g e t _ t h r e a d _ i d
 *
 **************************************
 *
 * Functional description
 *	Get platform's notion of a thread ID.
 *
 **************************************/
	FB_THREAD_ID id = 1;
#ifdef WIN_NT
	id = GetCurrentThreadId();
#endif
#ifdef SOLARIS_MT
	id = thr_self();
#endif
#ifdef USE_POSIX_THREADS

/* The following is just a temp. decision.
*/
#ifdef HP10

	id = (FB_THREAD_ID) (pthread_self().field1);

#else

	id = (FB_THREAD_ID) pthread_self();

#endif /* HP10 */
#endif /* USE_POSIX_THREADS */

	return id;
}


ThreadData* ThreadData::getSpecific(void)
{
/**************************************
 *
 *	T H D _ g e t _ s p e c i f i c
 *
 **************************************
 *
 * Functional description
 * Gets thread specific data and returns
 * a pointer to it.
 *
 **************************************/
	return TLS_GET(tData);
}


void ThreadData::getSpecificData(void **t_data)
{
/**************************************
 *
 *	T H D _ g e t s p e c i f i c _ d a t a
 *
 **************************************
 *
 * Functional description
 *	return the previously stored t_data.
 *
 **************************************/

	*t_data = TLS_GET(tSpecific);
}


void ThreadData::putSpecific()
{
/**************************************
 *
 *	T H D _ p u t _ s p e c i f i c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	threadDataPriorContext = TLS_GET(tData);
	TLS_SET(tData, this);
}


void ThreadData::putSpecificData(void *t_data)
{
/**************************************
 *
 *	T H D _ p u t s p e c i f i c _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Store the passed t_data
 *
 **************************************/

	TLS_SET(tSpecific, t_data);
}


void ThreadData::restoreSpecific()
{
/**************************************
 *
 *	T H D _ r e s t o r e _ s p e c i f i c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ThreadData* current_context = getSpecific();

	TLS_SET(tData, current_context->threadDataPriorContext);
}

namespace 
{

#ifdef THREAD_PSCHED
THREAD_ENTRY_DECLARE threadStart(THREAD_ENTRY_PARAM arg) {
	fb_assert(arg);
	Firebird::ContextPoolHolder mainThreadContext(getDefaultMemoryPool());
	{
		ThreadPriorityScheduler* tps = 
			reinterpret_cast<ThreadPriorityScheduler*>(arg);
		try {
			tps->run();
		}
		catch (...) {
			tps->detach();
			throw;
		}
		tps->detach();
		return 0;
	}
}

#define THREAD_ENTRYPOINT threadStart
#define THREAD_ARG tps

#else  //THREAD_PSCHED



// due to same names of parameters for various ThreadData::start(...),
// we may use common macro for various platforms
#define THREAD_ENTRYPOINT threadStart
#define THREAD_ARG reinterpret_cast<THREAD_ENTRY_PARAM> (FB_NEW(*getDefaultMemoryPool()) \
		ThreadArgs(reinterpret_cast<THREAD_ENTRY_RETURN (THREAD_ENTRY_CALL *) (THREAD_ENTRY_PARAM)>(routine), \
		reinterpret_cast<THREAD_ENTRY_PARAM>(arg)))

class ThreadArgs
{
public:
	typedef THREAD_ENTRY_RETURN (THREAD_ENTRY_CALL *Routine) (THREAD_ENTRY_PARAM);
	typedef THREAD_ENTRY_PARAM Arg;
private:
	Routine routine;
	Arg arg;
public:
	ThreadArgs(Routine r, Arg a) : routine(r), arg(a) { }
	ThreadArgs(const ThreadArgs& t) : routine(t.routine), arg(t.arg) { }
	void run() {routine(arg);}
private:
	ThreadArgs& operator=(const ThreadArgs&);
};

THREAD_ENTRY_DECLARE threadStart(THREAD_ENTRY_PARAM arg) {
	fb_assert(arg);
	Firebird::ContextPoolHolder mainThreadContext(getDefaultMemoryPool());
	ThreadArgs localArgs(*reinterpret_cast<ThreadArgs*>(arg));
	delete reinterpret_cast<ThreadArgs*>(arg);
	localArgs.run();
	return 0;
}

#endif //THREAD_PSCHED

} // anonymous namespace 


#ifdef ANY_THREADING
#ifdef USE_POSIX_THREADS
#define START_THREAD
void ThreadData::start(ThreadEntryPoint* routine,
				void *arg, 
				int priority_arg, 
				int flags, 
				void *thd_id)
{
/**************************************
 *
 *	t h r e a d _ s t a r t		( P O S I X )
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.
 *
 **************************************/
	pthread_t thread;
	pthread_attr_t pattr;
	int state;

#if ( !defined HP10 && !defined LINUX && !defined FREEBSD )
	state = pthread_attr_init(&pattr);
	if (state)
		Firebird::system_call_failed::raise("pthread_attr_init", state);

	// Do not make thread bound for superserver/client
#if (!defined (SUPERCLIENT) && !defined (SUPERSERVER))
	pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
#endif

	pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);
	state = pthread_create(&thread, &pattr, THREAD_ENTRYPOINT, THREAD_ARG);
	pthread_attr_destroy(&pattr);
	if (state)
		Firebird::system_call_failed::raise("pthread_create", state);

#else
#if ( defined LINUX || defined FREEBSD )

	if (state = pthread_create(&thread, NULL, THREAD_ENTRYPOINT, THREAD_ARG))
		Firebird::system_call_failed::raise("pthread_create", state);
	if (state = pthread_detach(thread))
		Firebird::system_call_failed::raise("pthread_detach", state);

#else
	
	long stack_size;

	state = pthread_attr_create(&pattr);
	if (state)
		Firebird::system_call_failed::raise("pthread_attr_create", state);

/* The default HP's stack size is too small. HP's documentation
   says it is "machine specific". My test showed it was less
   than 64K. We definitly need more stack to be able to execute
   concurrently many (at least 100) copies of the same request
   (like, for example in case of recursive stored prcedure).
   The following code sets threads stack size up to 256K if the
   default stack size is less than this number
*/
	stack_size = pthread_attr_getstacksize(pattr);
	if (stack_size == -1)
		Firebird::system_call_failed::raise("pthread_attr_getstacksize");

	if (stack_size < 0x40000L) {
		state = pthread_attr_setstacksize(&pattr, 0x40000L);
		if (state)
			Firebird::system_call_failed::raise("pthread_attr_setstacksize", state);
	}

/* HP's Posix threads implementation does not support
   bound attribute. It just a user level library.
*/
	state = pthread_create(&thread, pattr, THREAD_ENTRYPOINT, THREAD_ARG);
	if (state)
		Firebird::system_call_failed::raise("pthread_create", state);

	state = pthread_detach(&thread);
	if (state)
		Firebird::system_call_failed::raise("pthread_detach", state);
	state = pthread_attr_delete(&pattr);
	if (state)
		Firebird::system_call_failed::raise("pthread_attr_delete", state);

#endif /* linux */
#endif /* HP10 */
}
#endif /* USE_POSIX_THREADS */


#ifdef SOLARIS_MT
#define START_THREAD
void ThreadData::start(ThreadEntryPoint* routine,
				void *arg, 
				int priority_arg, 
				int flags, 
				void *thd_id)
{
/**************************************
 *
 *	t h r e a d _ s t a r t		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.
 *
 **************************************/
	int rval;
	thread_t thread_id;
	sigset_t new_mask, orig_mask;

	sigfillset(&new_mask);
	sigdelset(&new_mask, SIGALRM);
	if (rval = thr_sigsetmask(SIG_SETMASK, &new_mask, &orig_mask))
		Firebird::system_call_failed::raise("thr_sigsetmask", rval);
#if (defined SUPERCLIENT || defined SUPERSERVER)
	rval = thr_create(NULL, 0, THREAD_ENTRYPOINT, THREAD_ARG, THR_DETACHED | THR_NEW_LWP,
					 &thread_id);
#else
	rval =
		thr_create(NULL, 0, THREAD_ENTRYPOINT, THREAD_ARG, THR_DETACHED | THR_BOUND,
					 &thread_id);
#endif
	thr_sigsetmask(SIG_SETMASK, &orig_mask, NULL);

	if (rval)
		Firebird::system_call_failed::raise("thr_create", rval);
}
#endif
#endif


#ifdef WIN_NT
#define START_THREAD
void ThreadData::start(ThreadEntryPoint* routine,
				void *arg, 
				int priority_arg, 
				int flags, 
				void *thd_id)
{
/**************************************
 *
 *	t h r e a d _ s t a r t		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.
 *
 **************************************/

	int priority;

	switch (priority_arg) {
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

#ifdef THREAD_PSCHED
	ThreadPriorityScheduler::Init();

	ThreadPriorityScheduler *tps = FB_NEW(*getDefaultMemoryPool()) 
		ThreadPriorityScheduler(routine, arg, 
			ThreadPriorityScheduler::adjustPriority(priority));
#endif //THREAD_PSCHED

	/* I have changed the CreateThread here to _beginthreadex() as using
	 * CreateThread() can lead to memory leaks caused by C-runtime library.
	 * Advanced Windows by Richter pg. # 109. */

	DWORD thread_id;
	unsigned long real_handle = 
		_beginthreadex(NULL, 0, THREAD_ENTRYPOINT, THREAD_ARG, CREATE_SUSPENDED,
					   reinterpret_cast<unsigned*>(&thread_id));
	if (!real_handle)
	{
		Firebird::system_call_failed::raise("_beginthreadex", GetLastError());
	}
	HANDLE handle = reinterpret_cast<HANDLE>(real_handle);

	SetThreadPriority(handle, priority);

	if (! (flags & THREAD_wait))
	{
		ResumeThread(handle);
	}
	if (thd_id)
	{
		*(HANDLE *) thd_id = handle;
	}
	else
	{
		CloseHandle(handle);
	}
}
#endif


#ifdef ANY_THREADING
#ifdef VMS
#ifndef USE_POSIX_THREADS
#define START_THREAD
/**************************************
 *
 *	t h r e a d _ s t a r t		( V M S )
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.  This routine is coded in macro.
 *
 **************************************/
#endif
#endif
#endif


#ifndef START_THREAD
void ThreadData::start(ThreadEntryPoint* routine,
				void *arg, 
				int priority_arg, 
				int flags, 
				void *thd_id)
{
/**************************************
 *
 *	t h r e a d _ s t a r t		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Wrong attempt to start a new thread.
 *
 **************************************/

}
#endif


