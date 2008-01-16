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
#include "../jrd/ThreadStart.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../jrd/os/thd_priority.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/gdsassert.h"
#include "../common/classes/fb_tls.h"
#include "../common/classes/locks.h"


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


FB_THREAD_ID getThreadId()
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

// The following is just a temp. decision.
#ifdef HP10

	id = (FB_THREAD_ID) (pthread_self().field1);

#else

	id = (FB_THREAD_ID) pthread_self();

#endif // HP10
#endif // USE_POSIX_THREADS

	return id;
}


int THD_rec_mutex_lock(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	if (rec_mutex->rec_mutx_id == getThreadId())
	{
		rec_mutex->rec_mutx_count++;
	}
	else 
	{
	    try 
		{
			rec_mutex->rec_mutx_mtx.enter();
		}
		catch (const Firebird::system_call_failed& e)
		{
			return e.getErrorCode();
		}
		rec_mutex->rec_mutx_id = getThreadId();
		rec_mutex->rec_mutx_count = 1;
	}
	return 0;
}


int THD_rec_mutex_unlock(REC_MUTX_T * rec_mutex)
{
/**************************************
 *
 *	T H D _ r e c _ m u t e x _ u n l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (rec_mutex->rec_mutx_id != getThreadId())
		return FB_FAILURE;

	rec_mutex->rec_mutx_count--;

	if (rec_mutex->rec_mutx_count == 0)
	{
		rec_mutex->rec_mutx_id = 0;
	    try 
		{
			rec_mutex->rec_mutx_mtx.leave();
		}
		catch (const Firebird::system_call_failed& e)
		{
			return e.getErrorCode();
		}
	}
	return 0;
}


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
	event_t timer;
	event_t* timer_ptr = &timer;

	ISC_event_init(&timer, 0, 0);
	const SLONG count = ISC_event_clear(&timer);

	ISC_event_wait(1, &timer_ptr, &count, milliseconds * 1000);
	ISC_event_fini(&timer);
#endif // WIN_NT
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
#endif // _POSIX_PRIORITY_SCHEDULING
#endif

#ifdef SOLARIS_MT
	thr_yield();
#endif

#ifdef WIN_NT
	SleepEx(0, FALSE);
#endif
}
