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
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
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


#ifdef WIN_NT
#include <process.h>
#include <windows.h>
#include "os/thd_priority.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif


#ifdef SOLARIS
#include <thread.h>
#include <signal.h>
#endif


extern "C" {


#ifndef ANY_THREADING
THDD gdbb;
#endif
#ifdef VMS
/* THE SOLE PURPOSE OF THE FOLLOWING DECLARATION IS TO ALLOW THE VMS KIT TO
   COMPILE.  IT IS NOT CORRECT AND MUST BE REMOVED AT SOME POINT. */
THDD gdbb;
#endif

static void init(void);
static void init_tkey(void);
static void put_specific(THDD);
static int thread_start(int (*)(void *), void *, int, int, void *);

static USHORT initialized = FALSE;
static USHORT t_init = FALSE;

#ifdef ANY_THREADING
static MUTX_T ib_mutex;

#ifdef POSIX_THREADS
static pthread_key_t specific_key;
static pthread_key_t t_key;
#endif

#ifdef SOLARIS_MT
static thread_key_t specific_key;
static thread_key_t t_key;
#endif

#ifdef WIN_NT
static DWORD specific_key;
static DWORD t_key;
#endif
#endif // ANY_THREADING


int API_ROUTINE gds__thread_start(
								  FPTR_INT_VOID_PTR entrypoint,
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

	return thread_start(entrypoint, arg, priority, flags, thd_id);
}


long THD_get_thread_id(void)
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
	long id = 1;
#ifdef WIN_NT
	id = GetCurrentThreadId();
#endif
#ifdef SOLARIS_MT
	id = thr_self();
#endif
#ifdef POSIX_THREADS

/* The following is just a temp. decision.
*/
#ifdef HP10

	id = (long) (pthread_self().field1);

#else

	id = (long) pthread_self();

#endif /* HP10 */
#endif /* POSIX_THREADS */

	return id;
}


#ifdef ANY_THREADING
#ifdef POSIX_THREADS
#define GET_SPECIFIC_DEFINED
THDD THD_get_specific(void)
{
/**************************************
 *
 *	T H D _ g e t _ s p e c i f i c		( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Gets thread specific data and returns
 * a pointer to it.
 *
 **************************************/
#ifdef HP10

	THDD current_context;

	pthread_getspecific(specific_key, (pthread_addr_t *) & current_context);
	return current_context;

#else

	return ((THDD) pthread_getspecific(specific_key));

#endif /* HP10 */
}
#endif /* POSIX_THREADS */
#endif /* ANY_THREADING */


#ifdef ANY_THREADING
#ifdef SOLARIS_MT
#define GET_SPECIFIC_DEFINED
THDD THD_get_specific(void)
{
/**************************************
 *
 *	T H D _ g e t _ s p e c i f i c		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	THDD current_context;

	if (thr_getspecific(specific_key, (void **) &current_context)) {
		ib_perror("thr_getspecific");
		exit(1);
	}

	return current_context;
}
#endif
#endif


#ifdef ANY_THREADING
#ifdef WIN_NT
#define GET_SPECIFIC_DEFINED
THDD THD_get_specific(void)
{
/**************************************
 *
 *	T H D _ g e t _ s p e c i f i c		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return THPS_GET((THDD)TlsGetValue(specific_key));
}
#endif
#endif


#ifndef GET_SPECIFIC_DEFINED
THDD DLL_EXPORT THD_get_specific(void)
{
/**************************************
 *
 *	T H D _ g e t _ s p e c i f i c		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return gdbb;
}
#endif


void THD_getspecific_data(void **t_data)
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

#ifdef ANY_THREADING

/* There are some circumstances in which we do not call THD_putspecific_data(),
   such as services API, and local access on NT. As result of that, t_init
   does not get initialized. So don't use an assert in here but rather do
   the work only if t_init is initialised */
	if (t_init) {
#ifdef POSIX_THREADS
#ifdef HP10
		pthread_getspecific(t_key, t_data);
#else
		*t_data = (void *) pthread_getspecific(t_key);
#endif /* HP10 */
#endif /* POSIX_THREADS */

#ifdef SOLARIS_MT
		thr_getspecific(t_key, t_data);
#endif

#ifdef WIN_NT
		*t_data = (void *) TlsGetValue(t_key);
#endif
	}
#endif
}


void DLL_EXPORT THD_cleanup(void)
{
/**************************************
 *
 *	T H D _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 * This is the cleanup function called from the DLL
 * cleanup function. This helps to remove the allocated
 * thread specific key.
 *
 **************************************/

	if (initialized) {
		initialized = FALSE;
#ifdef POSIX_THREADS
#endif

#ifdef SOLARIS_MT
#endif

#ifdef ANY_THREADING
#ifdef WIN_NT
		TlsFree(specific_key);
#endif
		/* destroy the mutex ib_mutex which was created */
		THD_mutex_destroy(&ib_mutex);
#ifdef WIN_NT
		THPS_FINI();
#endif /* WIN_NT */
#endif /* ANY_THREADING */
	}
}


void DLL_EXPORT THD_init(void)
{
/**************************************
 *
 *	T H D _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
#ifdef POSIX_THREADS

/* In case of Posix threads we take advantage of using function
   pthread_once. This function makes sure that init() routine
   will be called only once by the first thread to call pthread_once.
*/
#ifdef HP10
	static pthread_once_t once = pthread_once_init;
#else
	static pthread_once_t once = PTHREAD_ONCE_INIT;
#endif /* HP10 */

	pthread_once(&once, init);

#else

	init();

#endif /* POSIX_THREADS */
}


void DLL_EXPORT THD_init_data(void)
{
/**************************************
 *
 *	T H D _ i n i t _ d a t a
 *
 **************************************
 *
 * Functional description
 *	init function for t_key. This is called 
 *	to ensure that the key is created.
 *
 **************************************/
#ifdef POSIX_THREADS

/* In case of Posix threads we take advantage of using function
   pthread_once. This function makes sure that init_tkey() routine
   will be called only once by the first thread to call pthread_once.
*/
#ifdef HP10
	static pthread_once_t once = pthread_once_init;
#else
	static pthread_once_t once = PTHREAD_ONCE_INIT;
#endif /* HP10 */

	pthread_once(&once, init_tkey);

#else

	init_tkey();

#endif /* POSIX_THREADS */
}


#ifdef ANY_THREADING
#ifdef POSIX_THREADS
#define THREAD_MUTEXES_DEFINED
int THD_mutex_destroy(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y 	( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Tries to destroy mutex and returns 0 if success,
 * error number in case of error
 *
 **************************************/
#ifdef HP10

	int state;

	state = pthread_mutex_destroy(mutex);
	if (!state)
		return 0;
	assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_destroy(&mutex->mutx_mutex);

#endif /* HP10 */

}


int THD_mutex_init(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t		( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Tries to initialize mutex and returns 0 if success,
 * error number in case of error
 *
 **************************************/
#ifdef HP10

	int state;

	state = pthread_mutex_init(mutex, pthread_mutexattr_default);
	if (!state)
		return 0;
	assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_init(&mutex->mutx_mutex, NULL);

#endif /* HP10 */
}


int THD_mutex_lock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k		( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Tries to lock mutex and returns 0 if success,
 * error number in case of error
 *
 **************************************/
#ifdef HP10

	int state;

	state = pthread_mutex_lock(mutex);
	if (!state)
		return 0;
	assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_lock(&mutex->mutx_mutex);

#endif /* HP10 */
}


int THD_mutex_unlock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k		( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Tries to unlock mutex and returns 0 if success,
 * error number in case of error
 *
 **************************************/
#ifdef HP10

	int state;

	state = pthread_mutex_unlock(mutex);
	if (!state)
		return 0;
	assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_unlock(&mutex->mutx_mutex);

#endif /* HP10 */
}
#endif /* POSIX_THREADS */
#endif /* ANY_THREADING */


#ifdef ANY_THREADING
#ifdef SOLARIS_MT
#define THREAD_MUTEXES_DEFINED
int THD_mutex_destroy(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y 	( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return mutex_destroy(&mutex->mutx_mutex);
}


int THD_mutex_init(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return mutex_init(&mutex->mutx_mutex, USYNC_THREAD, NULL);
}


int THD_mutex_lock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return mutex_lock(&mutex->mutx_mutex);
}


int THD_mutex_unlock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return mutex_unlock(&mutex->mutx_mutex);
}
#endif
#endif


#ifdef ANY_THREADING
#ifdef VMS
#define THREAD_MUTEXES_DEFINED
int THD_mutex_destroy(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y	( V M S )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int THD_mutex_init(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t		( V M S )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return ISC_mutex_init(mutex, 0);
}


int THD_mutex_lock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k		( V M S )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return ISC_mutex_lock(mutex);
}


int THD_mutex_unlock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k		( V M S )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return ISC_mutex_unlock(mutex);
}
#endif
#endif



#ifdef ANY_THREADING

#ifdef WIN_NT

#define THREAD_MUTEXES_DEFINED

int THD_mutex_destroy(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	DeleteCriticalSection(reinterpret_cast < LPCRITICAL_SECTION > (mutex));

	return 0;
}


int THD_mutex_init(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	InitializeCriticalSection(reinterpret_cast < LPCRITICAL_SECTION >
							  (mutex));

	return 0;
}


int THD_mutex_lock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	EnterCriticalSection(reinterpret_cast < LPCRITICAL_SECTION > (mutex));

	return 0;
}


int THD_mutex_unlock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	LeaveCriticalSection(reinterpret_cast < LPCRITICAL_SECTION > (mutex));

	return 0;
}

#endif // WIN_NT

#endif // ANY_THREADING


#ifndef THREAD_MUTEXES_DEFINED
int DLL_EXPORT THD_mutex_destroy(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y	( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int THD_mutex_init(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int THD_mutex_lock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int THD_mutex_unlock(MUTX_T * mutex)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}
#endif


int THD_mutex_destroy_n(MUTX_T * mutexes, USHORT n)
{
/**************************************
 *
 *	T H D _ m u t e x _ d e s t r o y _ n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	while (n--)
		THD_mutex_destroy(mutexes++);

	return 0;
}


int THD_mutex_init_n(MUTX_T * mutexes, USHORT n)
{
/**************************************
 *
 *	T H D _ m u t e x _ i n i t _ n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	while (n--)
		THD_mutex_init(mutexes++);

	return 0;
}


#ifdef ANY_THREADING
int THD_mutex_lock_global(void)
{
/**************************************
 *
 *	T H D _ m u t e x _ l o c k _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return THD_mutex_lock(&ib_mutex);
}


int THD_mutex_unlock_global(void)
{
/**************************************
 *
 *	T H D _ m u t e x _ u n l o c k _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return THD_mutex_unlock(&ib_mutex);
}
#endif


#ifdef V4_THREADING
#ifdef SOLARIS_MT
#ifdef THD_RWLOCK_STRUCT
#define RW_LOCK_DEFINED
int THD_wlck_destroy(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ d e s t r o y		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status;

#ifdef DEBUG_THREAD
	ib_fprintf(ib_stderr, "calling rwlock_destroy %x\n", wlock);
#endif

	status = rwlock_destroy(wlock);

#ifdef DEBUG_THREAD
	if (status)
		ib_fprintf(ib_stderr, "status = %d errno = %d\n", status, errno);
#endif

	return status;
}


int THD_wlck_init(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ i n i t		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status;

#ifdef DEBUG_THREAD
	ib_fprintf(ib_stderr, "calling rwlock_init %x\n", wlock);
#endif

	status = rwlock_init(wlock, USYNC_THREAD, NULL);

#ifdef DEBUG_THREAD
	if (status)
		ib_fprintf(ib_stderr, "status = %d errno = %d\n", status, errno);
#endif

	return status;
}


int THD_wlck_lock(WLCK_T * wlock, USHORT type)
{
/**************************************
 *
 *	T H D _ w l c k _ l o c k		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status;

#ifdef DEBUG_THREAD
	if (type == WLCK_read)
		ib_fprintf(ib_stderr, "calling rwlock_rdlock %x\n", wlock);
	else
		ib_fprintf(ib_stderr, "calling rwlock_wrlock %x\n", wlock);
#endif

	if (type == WLCK_read)
		status = rw_rdlock(wlock);
	else
		status = rw_wrlock(wlock);

#ifdef DEBUG_THREAD
	if (status)
		ib_fprintf(ib_stderr, "status = %d errno = %d\n", status, errno);
#endif

	return status;
}


int THD_wlck_unlock(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ u n l o c k	( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status;

#ifdef DEBUG_THREAD
	ib_fprintf(ib_stderr, "calling rwlock_unlock %x\n", wlock);
#endif

	status = rw_unlock(wlock);

#ifdef DEBUG_THREAD
	if (status)
		ib_fprintf(ib_stderr, "status = %d errno = %d\n", status, errno);
#endif

	return status;
}
#endif
#endif
#endif


#ifdef V4_THREADING
#ifndef THD_RWLOCK_STRUCT
#define RW_LOCK_DEFINED
int THD_wlck_destroy(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ d e s t r o y 	( s i m u l a t e )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return cond_destroy(&wlock->wlck_cond);
}


int THD_wlck_init(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ i n i t		( s i m u l a t e )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	wlock->wlck_count = 0;
	wlock->wlck_waiters = 0;

	return cond_init(&wlock->wlck_cond);
}


int THD_wlck_lock(WLCK_T * wlock, USHORT type)
{
/**************************************
 *
 *	T H D _ w l c k _ l o c k		( s i m u l a t e )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status, incr;

#ifdef DEBUG_THREAD
	if (type == WLCK_read)
		ib_fprintf(ib_stderr, "calling rwlock_rdlock %x\n", wlock);
	else
		ib_fprintf(ib_stderr, "calling rwlock_wrlock %x\n", wlock);
#endif

	if (status = THD_mutex_lock(&wlock->wlck_cond.cond_mutex))
		return status;

	if (type == WLCK_read) {
		if (wlock->wlck_count < 0)
			incr = ++wlock->wlck_waiters;
		else
			incr = 0;

		while (wlock->wlck_count < 0)
			cond_wait(&wlock->wlck_cond, NULL);

		wlock->wlck_count++;
	}
	else {
		if (wlock->wlck_count > 0)
			incr = ++wlock->wlck_waiters;
		else
			incr = 0;

		while (wlock->wlck_count > 0)
			cond_wait(&wlock->wlck_cond, NULL);

		wlock->wlck_count = -1;
	}

	if (incr)
		wlock->wlck_waiters--;

	return THD_mutex_unlock(&wlock->wlck_cond.cond_mutex);
}


int THD_wlck_unlock(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ u n l o c k	( s i m u l a t e )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int status;

	if (status = THD_mutex_lock(&wlock->wlck_cond.cond_mutex))
		return status;

	if (wlock->wlck_count > 0)
		wlock->wlck_count--;
	else
		wlock->wlck_count = 0;

	if (wlock->wlck_waiters)
		cond_broadcast(&wlock->wlck_cond);

	return THD_mutex_unlock(&wlock->wlck_cond.cond_mutex);
}
#endif
#endif


#ifndef RW_LOCK_DEFINED
int DLL_EXPORT THD_wlck_destroy(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ d e s t r o y		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int DLL_EXPORT THD_wlck_init(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ i n i t		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int DLL_EXPORT THD_wlck_lock(WLCK_T * wlock, USHORT type)
{
/**************************************
 *
 *	T H D _ w l c k _ l o c k		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}


int DLL_EXPORT THD_wlck_unlock(WLCK_T * wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ u n l o c k	( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return 0;
}
#endif


void THD_wlck_destroy_n(WLCK_T * wlocks, USHORT n)
{
/**************************************
 *
 *	T H D _ w l c k _ d e s t r o y _ n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	while (n--)
		THD_wlck_destroy(wlocks++);
}


void THD_wlck_init_n(WLCK_T * wlocks, USHORT n)
{
/**************************************
 *
 *	T H D _ w l c k _ i n i t _ n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	while (n--)
		THD_wlck_init(wlocks++);
}


void DLL_EXPORT THD_put_specific(THDD new_context)
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

	if (!initialized)
		THD_init();

/* Save the current context */

	new_context->thdd_prior_context = THD_get_specific();

	put_specific(new_context);
}


void DLL_EXPORT THD_putspecific_data(void *t_data)
{
/**************************************
 *
 *	T H D _ p u t s p e c i f i c _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Store the passed t_data using the ket t_key
 *
 **************************************/

	if (!t_init)
		THD_init_data();
#ifdef ANY_THREADING

#ifdef POSIX_THREADS
	pthread_setspecific(t_key, t_data);
#endif

#ifdef SOLARIS_MT
	thr_setspecific(t_key, t_data);
#endif

#ifdef WIN_NT
	TlsSetValue(t_key, (LPVOID) t_data);
#endif

#endif
}


THDD DLL_EXPORT THD_restore_specific(void)
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
	THDD current_context;

	current_context = THD_get_specific();

#pragma FB_COMPILER_MESSAGE("Fix! Bad struct ptr type cast.")
	put_specific(reinterpret_cast < THDD >
				 (current_context->thdd_prior_context));

	return reinterpret_cast < THDD > (current_context->thdd_prior_context);
}


#ifdef SUPERSERVER
int THD_rec_mutex_destroy(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ d e s t r o y   ( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return THD_mutex_destroy(rec_mutex->rec_mutx_mtx);
}


int THD_rec_mutex_init(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ i n i t   ( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	THD_mutex_init(rec_mutex->rec_mutx_mtx);
	rec_mutex->rec_mutx_id = 0;
	rec_mutex->rec_mutx_count = 0;
	return 0;
}


int THD_rec_mutex_lock(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ l o c k   ( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	int ret;

	if (rec_mutex->rec_mutx_id == (ULONG) THD_get_thread_id())
		rec_mutex->rec_mutx_count++;
	else {
		if (ret = THD_mutex_lock(rec_mutex->rec_mutx_mtx))
			return ret;
		rec_mutex->rec_mutx_id = THD_get_thread_id();
		rec_mutex->rec_mutx_count = 1;
	}
	return 0;
}


int THD_rec_mutex_unlock(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ u n l o c k   ( S U P E R S E R V E R )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (rec_mutex->rec_mutx_id != (ULONG) THD_get_thread_id())
		return FB_FAILURE;

	rec_mutex->rec_mutx_count--;

	if (rec_mutex->rec_mutx_count)
		return 0;
	else {
		rec_mutex->rec_mutx_id = 0;
		return THD_mutex_unlock(rec_mutex->rec_mutx_mtx);
	}
}
#endif /* SUPERSERVER */


#ifdef WIN_NT
#define THREAD_SUSPEND_DEFINED
int THD_resume(THD_T thread)
{
/**************************************
 *
 *	T H D _ r e s u m e			( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Resume execution of a thread that has been
 *	suspended.
 *
 **************************************/

	if (ResumeThread(thread) == 0xFFFFFFFF)
		return GetLastError();

	return 0;
}


int THD_suspend(THD_T thread)
{
/**************************************
 *
 *	T H D _ s u s p e n d			( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Suspend execution of a thread.
 *
 **************************************/

	if (SuspendThread(thread) == 0xFFFFFFFF)
		return GetLastError();

	return 0;
}
#endif


#ifndef THREAD_SUSPEND_DEFINED
int THD_resume(THD_T thread)
{
/**************************************
 *
 *	T H D _ r e s u m e			( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Resume execution of a thread that has been
 *	suspended.
 *
 **************************************/

	return 0;
}


int THD_suspend(THD_T thread)
{
/**************************************
 *
 *	T H D _ s u s p e n d			( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Suspend execution of a thread.
 *
 **************************************/

	return 0;
}
#endif


void THD_sleep(ULONG milliseconds)
{
/**************************************
 *
 *	T H D _ s l e e p
 *
 **************************************
 *
 * Functional description
 *	Thread sleeps for requested number
 *	of milliseconds.
 *
 **************************************/
#ifdef WIN_NT
	SleepEx(milliseconds, FALSE);
#else

#ifdef ANY_THREADING
	EVENT_T timer;
	EVENT timer_ptr = &timer;
	SLONG count;

	ISC_event_init(&timer, 0, 0);
	count = ISC_event_clear(&timer);

	(void) ISC_event_wait(1, &timer_ptr, &count, milliseconds * 1000,
						  (FPTR_VOID) 0, 0);
	ISC_event_fini(&timer);
#else /* !ANY_THREADING */
	int seconds;

/* Insure that process sleeps some amount of time. */

	if (!(seconds = milliseconds / 1000))
		++seconds;

/* Feedback unslept time due to premature wakeup from signals. */

	while (seconds = sleep(seconds));

#endif /* !ANY_THREADING */

#endif /* !WIN_NT */
}


void THD_yield(void)
{
/**************************************
 *
 *	T H D _ y i e l d
 *
 **************************************
 *
 * Functional description
 *	Thread relinquishes the processor.
 *
 **************************************/
#ifdef ANY_THREADING

#ifdef POSIX_THREADS
/* use sched_yield() instead of pthread_yield(). Because pthread_yield() 
   is not part of the (final) POSIX 1003.1c standard. Several drafts of 
   the standard contained pthread_yield(), but then the POSIX guys 
   discovered it was redundant with sched_yield() and dropped it. 
   So, just use sched_yield() instead. POSIX systems on which  
   sched_yield() is available define _POSIX_PRIORITY_SCHEDULING 
   in <unistd.h>.  Darwin defined _POSIX_THREAD_PRIORITY_SCHEDULING
   instead of _POSIX_PRIORITY_SCHEDULING.
*/
#if (defined _POSIX_PRIORITY_SCHEDULING || defined _POSIX_THREAD_PRIORITY_SCHEDULING)
	sched_yield();
#else
	pthread_yield();
#endif /* _POSIX_PRIORITY_SCHEDULING */
#endif

#ifdef SOLARIS
	thr_yield();
#endif

#ifdef WIN_NT
	SleepEx(1, FALSE);
#endif
#endif /* ANY_THREADING */
}


#ifdef V4_THREADING
#ifdef SOLARIS_MT
#define CONDITION_DEFINED
static int cond_broadcast(COND_T * cond)
{
/**************************************
 *
 *	c o n d _ b r o a d c a s t	( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return cond_broadcast(&cond->cond_cond);
}


static int cond_destroy(COND_T * cond)
{
/**************************************
 *
 *	c o n d _ d e s t r o y		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	THD_mutex_destroy(&cond->cond_mutex);

	return cond_destroy(&cond->cond_cond);
}


static int cond_init(COND_T * cond)
{
/**************************************
 *
 *	c o n d _ i n i t		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	THD_mutex_init(&cond->cond_mutex);

	return cond_init(&cond->cond_cond, USYNC_THREAD, NULL);
}


static int cond_wait(COND_T * cond, timestruc_t * time)
{
/**************************************
 *
 *	c o n d _ w a i t		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (time)
		return cond_timedwait(&cond->cond_cond, &cond->cond_mutex, time);
	else
		return cond_wait(&cond->cond_cond, &cond->cond_mutex);
}
#endif
#endif


static void init(void)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (initialized)
		return;

	initialized = TRUE;

#ifdef ANY_THREADING
	THD_mutex_init(&ib_mutex);

#ifdef POSIX_THREADS
#ifdef HP10

	pthread_keycreate(&specific_key, NULL);

#else

	pthread_key_create(&specific_key, NULL);

#endif /* HP10 */
#endif /* POSIX_THREADS */

#ifdef SOLARIS_MT
	if (thr_keycreate(&specific_key, NULL)) {
		/* This call to thr_min_stack exists simply to force a link error
		 * for a client application that neglects to include -lthread.
		 * Solaris, for unknown reasons, includes stubs for all the
		 * thread functions in libC.  Should the stubs be linked in
		 * there is no compile error, no runtime error, and InterBase
		 * will core drop.
		 * Including this call gives an undefined symbol if -lthread is
		 * omitted, looking at the man page will inform the client programmer
		 * that -lthread is needed to resolve the symbol.
		 * Note that we don't care if this thr_min_stack() is called or
		 * not, we just need to have a reference to it to force a link error.
		 */
		(void) thr_min_stack();
		ib_perror("thr_keycreate");
		exit(1);
	}
#endif /* SOLARIS _MT */

#ifdef WIN_NT
	specific_key = TlsAlloc();
#endif /* WIN_NT */

#pragma FB_COMPILER_MESSAGE("Fix! Bad function ptr type cast.")
	gds__register_cleanup(reinterpret_cast < FPTR_VOID_PTR > (THD_cleanup),
						  0);

#ifdef WIN_NT
	THPS_INIT();
#endif
#endif /* ANY_THREADING */
}


static void init_tkey(void)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Function which actually creates the key which
 *	can be used by the threads to store t_data
 *
 **************************************/

	if (t_init)
		return;

	t_init = TRUE;

#ifdef ANY_THREADING

#ifdef POSIX_THREADS
#ifdef HP10

	pthread_keycreate(&t_key, NULL);

#else

	pthread_key_create(&t_key, NULL);

#endif /* HP10 */
#endif /* POSIX_THREADS */

#ifdef SOLARIS_MT
	thr_keycreate(&t_key, NULL);
#endif

#ifdef WIN_NT
	t_key = TlsAlloc();
#endif
#endif
}


#ifdef ANY_THREADING
#ifdef POSIX_THREADS
#define PUT_SPECIFIC_DEFINED
static void put_specific(THDD new_context)
{
/**************************************
 *
 *	p u t _ s p e c i f i c		( P O S I X )
 *
 **************************************
 *
 * Functional description
 * Puts new thread specific data
 *
 **************************************/

	pthread_setspecific(specific_key, new_context);
}
#endif /* ANY_THREADING */
#endif /* POSIX_THREADS */


#ifdef ANY_THREADING
#ifdef SOLARIS_MT
#define PUT_SPECIFIC_DEFINED
static void put_specific(THDD new_context)
{
/**************************************
 *
 *	p u t _ s p e c i f i c		( S o l a r i s )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (thr_setspecific(specific_key, new_context)) {
		ib_perror("thr_setspecific");
		exit(1);
	}
}
#endif
#endif


#ifdef ANY_THREADING
#ifdef WIN_NT
#define PUT_SPECIFIC_DEFINED
static void put_specific(THDD new_context)
{
/**************************************
 *
 *	p u t _ s p e c i f i c		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	THPS_SET(TlsSetValue(specific_key, (LPVOID) new_context), new_context);
}
#endif
#endif


#ifndef PUT_SPECIFIC_DEFINED
static void put_specific(THDD new_context)
{
/**************************************
 *
 *	p u t _ s p e c i f i c		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	gdbb = new_context;
}
#endif


#ifdef ANY_THREADING
#ifdef POSIX_THREADS
#define START_THREAD
static int thread_start(
						int (*routine) (void *),
						void *arg, int priority_arg, int flags, void *thd_id)
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
		return state;

/* Do not make thread bound for superserver/client */

#if (!defined (SUPERCLIENT) && !defined (SUPERSERVER))
	pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
#endif

	pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);
	state = pthread_create(&thread, &pattr, (void*(*)(void*))routine, arg);
	pthread_attr_destroy(&pattr);
	return state;

#else
#if ( defined LINUX || defined FREEBSD )
		if (state = pthread_create(&thread, NULL, (void*(*)(void*))routine, arg))
		return state;
	return pthread_detach(thread);
#else
		long stack_size;

	state = pthread_attr_create(&pattr);
	if (state) {
		assert(state == -1);
		return errno;
	}

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
		return errno;

	if (stack_size < 0x40000L) {
		state = pthread_attr_setstacksize(&pattr, 0x40000L);
		if (state) {
			assert(state == -1);
			return errno;
		}
	}

/* HP's Posix threads implementation does not support
   bound attribute. It just a user level library.
*/
	state = pthread_create(&thread, pattr, routine, arg);
	if (state) {
		assert(state == -1);
		return errno;
	}
	state = pthread_detach(&thread);
	if (state) {
		assert(state == -1);
		return errno;
	}
	state = pthread_attr_delete(&pattr);
	if (state) {
		assert(state == -1);
		return errno;
	}
	return 0;

#endif /* linux */
#endif /* HP10 */
}
#endif /* POSIX_THREADS */
#endif /* ANY_THREADING */


#ifdef ANY_THREADING
#ifdef SOLARIS_MT
#define START_THREAD
static int thread_start(
						int (*routine) (void *),
						void *arg, int priority_arg, int flags, void *thd_id)
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

	(void) sigfillset(&new_mask);
	(void) sigdelset(&new_mask, SIGALRM);
	if (rval = thr_sigsetmask(SIG_SETMASK, &new_mask, &orig_mask))
		return rval;
#if (defined SUPERCLIENT || defined SUPERSERVER)
	rval = thr_create(NULL, 0, (void* (*)(void*) )routine, arg, THR_DETACHED, &thread_id);
#else
	rval =
		thr_create(NULL, 0, (void* (*)(void*) ) routine, arg, (THR_BOUND | THR_DETACHED),
				   &thread_id);
#endif
	(void) thr_sigsetmask(SIG_SETMASK, &orig_mask, NULL);

	return rval;
}
#endif
#endif


#ifdef WIN_NT
#define START_THREAD
static int thread_start(int (*routine) (void *),
						void *arg, int priority_arg, int flags, void *thd_id)
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

	HANDLE handle;
	DWORD thread_id;
	int priority;

	/* I have changed the CreateThread here to _beginthreadex() as using
	 * CreateThread() can lead to memory leaks caused by C-runtime library.
	 * Advanced Windows by Richter pg. # 109. */

#pragma FB_COMPILER_MESSAGE("Fix! Bad, bad, bad function ptr type cast!!!")
	unsigned long real_handle = _beginthreadex(NULL,
											   0,
											   reinterpret_cast <
											   unsigned (__stdcall *) (void *)
											   >(routine),
											   arg,
											   CREATE_SUSPENDED,
											   reinterpret_cast <
											   unsigned *>(&thread_id));
	if (!real_handle) {
		return GetLastError();
	}
	handle = reinterpret_cast < HANDLE > (real_handle);

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

	THPS_ATTACH(handle, thread_id, priority);
	SetThreadPriority(handle, priority);
	if (! (flags & THREAD_wait))
		ResumeThread(handle);
	if (thd_id)
		*(HANDLE *) thd_id = handle;
	else
		CloseHandle(handle);

	return 0;
}
#endif


#ifdef ANY_THREADING
#ifdef VMS
#ifndef POSIX_THREADS
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
static int thread_start(
						int (*routine) (void *),
						void *arg, int priority_arg, int flags, void *thd_id)
{
/**************************************
 *
 *	t h r e a d _ s t a r t		( G e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.
 *
 **************************************/

	return 1;
}
#endif


} // extern "C"
