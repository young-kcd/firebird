/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd.cpp
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
#endif //VMS
#endif //NOT_USED_OR_REPLACED



int THD_wlck_lock(WLCK_T* wlock, WLCK_type type)
{
/**************************************
 *
 *	T H D _ w l c k _ l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	switch (type)
	{
	case WLCK_read:
#ifdef DEBUG_THREAD
		fprintf(stderr, "calling rwlock_rdlock %x\n", wlock);
#endif
		wlock->rwLock.beginRead();
		break;

	case WLCK_write:
#ifdef DEBUG_THREAD
		fprintf(stderr, "calling rwlock_wrlock %x\n", wlock);
#endif
		wlock->rwLock.beginWrite();
		break;

#ifdef DEV_BUILD
	default:
		fb_assert(false);
		break;
#endif
	}

	wlock->type = type;
	return 0;
}


int THD_wlck_unlock(WLCK_T* wlock)
{
/**************************************
 *
 *	T H D _ w l c k _ u n l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

#ifdef DEBUG_THREAD
	fprintf(stderr, "calling rwlock_unlock %x\n", wlock);
#endif

	switch (wlock->type)
	{
	case WLCK_read:
		wlock->rwLock.endRead();
		break;

	case WLCK_write:
		wlock->rwLock.endWrite();
		break;
	}

	return 0;
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

	return 0;
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

	if (rec_mutex->rec_mutx_id == ThreadData::getId())
		rec_mutex->rec_mutx_count++;
	else {
		if (ret = THD_mutex_lock(rec_mutex->rec_mutx_mtx))
			return ret;
		rec_mutex->rec_mutx_id = ThreadData::getId();
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

	if (rec_mutex->rec_mutx_id != ThreadData::getId())
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
	event_t timer;
	event_t* timer_ptr = &timer;

	ISC_event_init(&timer, 0, 0);
	SLONG count = ISC_event_clear(&timer);

	ISC_event_wait(1, &timer_ptr, &count, milliseconds * 1000, NULL, 0);
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

#ifdef USE_POSIX_THREADS
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

#ifdef SOLARIS_MT
	thr_yield();
#endif

#ifdef WIN_NT
	SleepEx(0, FALSE);
#endif
#endif /* ANY_THREADING */
}

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



