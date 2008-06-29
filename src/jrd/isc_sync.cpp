/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_sync.cpp
 *	DESCRIPTION:	General purpose but non-user routines.
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "XENIX" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "DELTA" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "M88K" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SOLARIS
#ifndef DEV_BUILD
#define NDEBUG                                 // Turn off fb_assert() macros
#endif
#include "../jrd/gdsassert.h"
#endif

#ifdef HPUX
#include <sys/pstat.h>
#endif

//#include "../common/classes/timestamp.h"
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/file_params.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/err_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/jrd_pwd.h"
#include "../common/classes/fb_tls.h"
#include "../common/config/config.h"
#include "../common/utils_proto.h"

#ifdef UNIX
#include <setjmp.h>
#endif

static int process_id;
#ifdef UNIX
static UCHAR *next_shared_memory;
#endif

/* Unix specific stuff */

#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif

#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#ifndef O_RDWR
#include <fcntl.h>
#endif

#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif

#define FTOK_KEY	15
#define PRIV		0666

#ifndef SHMEM_DELTA
#define SHMEM_DELTA	(1 << 22)
#endif

#ifndef SIGURG
#define SIGURG		SIGINT
#endif

#ifndef HAVE_SEMUN
union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};
#endif
#endif /* UNIX */

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifndef HAVE_GETPAGESIZE
static size_t getpagesize(void)
{
	return PAGESIZE;
}
#endif


/* Windows NT */

#ifdef WIN_NT

#include <process.h>
#include <signal.h>
#include <windows.h>

#endif

using namespace Jrd;

static void		error(ISC_STATUS*, const TEXT*, ISC_STATUS);
static bool		event_blocked(USHORT count, const event_t* const* events, const SLONG* values);

#ifdef UNIX
static TLS_DECLARE(sigjmp_buf*, sigjmp_ptr);
static SLONG	find_key(ISC_STATUS *, TEXT *);
static void		longjmp_sig_handler(int);
#endif

#if defined(WIN_NT)
static void make_object_name(TEXT*, size_t, const TEXT*, const TEXT*);
#endif

#if defined FREEBSD || defined NETBSD || defined DARWIN || defined HPUX
#define sigset      signal
#endif


#ifdef SOLARIS_MT
static bool event_blocked(USHORT count, const event_t* const* events, const SLONG* values)
{
/**************************************
 *
 *	I S C _ e v e n t _ b l o c k e d	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	If a wait would block, return TRUE.
 *
 **************************************/

	for (; count > 0; --count, ++events, ++values)
		if ((*events)->event_count >= *values) {
#ifdef DEBUG_ISC_SYNC
			printf("event_blocked: FALSE (eg something to report)\n");
			fflush(stdout);
#endif
			return false;
		}

#ifdef DEBUG_ISC_SYNC
	printf("event_blocked: TRUE (eg nothing happened yet)\n");
	fflush(stdout);
#endif
	return true;
}


SLONG ISC_event_clear(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ c l e a r	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Clear an event preparatory to waiting on it.  The order of
 *	battle for event synchronization is:
 *
 *	    1.  Clear event.
 *	    2.  Test data structure for event already completed
 *	    3.  Wait on event.
 *
 **************************************/
	mutex_lock(event->event_mutex);
	const SLONG ret = event->event_count + 1;
	mutex_unlock(event->event_mutex);
	return ret;
}


void ISC_event_fini(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ f i n i	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Discard an event object.
 *
 **************************************/

/* Inter-Process event's are destroyed only */
	if (event->event_semid == -1) {
		mutex_destroy(event->event_mutex);
		cond_destroy(event->event_semnum);
	}
}


int ISC_event_init(event_t* event, int semid, int semnum)
{
/**************************************
 *
 *	I S C _ e v e n t _ i n i t	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Prepare an event object for use.
 *
 **************************************/
	//SLONG key, n;
	//union semun arg;

	event->event_count = 0;

	if (!semnum) {
		/* Prepare an Inter-Thread event block */
		event->event_semid = -1;
		mutex_init(event->event_mutex, USYNC_THREAD, NULL);
		cond_init(event->event_semnum, USYNC_THREAD, NULL);
	}
	else {
		/* Prepare an Inter-Process event block */
		event->event_semid = semid;
		mutex_init(event->event_mutex, USYNC_PROCESS, NULL);
		cond_init(event->event_semnum, USYNC_PROCESS, NULL);
	}

	return TRUE;
}


int ISC_event_post(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ p o s t	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Post an event to wake somebody else up.
 *
 **************************************/
/* For Solaris, we use cond_broadcast rather than cond_signal so that
   all waiters on the event are notified and awakened */

	mutex_lock(event->event_mutex);
	++event->event_count;
	const int ret = cond_broadcast(event->event_semnum);
	mutex_unlock(event->event_mutex);
	if (ret)
		gds__log("ISC_event_post: cond_broadcast failed with errno = %d", ret);
	return ret;
}


int ISC_event_wait(SSHORT	count,
				   event_t* const*	events,
				   const SLONG*	values,
				   const SLONG	micro_seconds)
{
/**************************************
 *
 *	I S C _ e v e n t _ w a i t	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Wait on an event.  If timeout limit specified, return
 *	anyway after the timeout even if no event has
 *	happened.  If returning due to timeout, return
 *	FB_FAILURE else return FB_SUCCESS.
 *
 **************************************/

/* While the API for ISC_event_wait allows for a list of events
   we never actually make use of it.  This implementation wont
   support it anyway as Solaris doesn't provide a "wait for one
   of a series of conditions" function */
	fb_assert(count == 1);

/* If we're not blocked, the rest is a gross waste of time */

	if (!event_blocked(count, events, values))
		return FB_SUCCESS;

/* Set up timers if a timeout period was specified. */

	timestruc_t timer;
	if (micro_seconds > 0) {
		timer.tv_sec = time(NULL);
		timer.tv_sec += micro_seconds / 1000000;
		timer.tv_nsec = 1000 * (micro_seconds % 1000000);
	}

	int ret = FB_SUCCESS;
	mutex_lock((*events)->event_mutex);
	for (;;) {
		if (!event_blocked(count, events, values)) {
			ret = FB_SUCCESS;
			break;
		}

		/* The Solaris cond_wait & cond_timedwait calls atomically release
		   the mutex and start a wait.  The mutex is reacquired before the
		   call returns. */

		if (micro_seconds > 0)
			ret =
				cond_timedwait((*events)->event_semnum,
							   (*events)->event_mutex, &timer);
		else
			ret = cond_wait((*events)->event_semnum, (*events)->event_mutex);
		if (micro_seconds > 0 && (ret == ETIME)) {

			/* The timer expired - see if the event occured and return
			   FB_SUCCESS or FB_FAILURE accordingly. */

			if (event_blocked(count, events, values))
				ret = FB_FAILURE;
			else
				ret = FB_SUCCESS;
			break;
		}
	}
	mutex_unlock((*events)->event_mutex);
	return ret;
}
#endif /* SOLARIS_MT */


#ifdef USE_POSIX_THREADS
static bool event_blocked(USHORT count, const event_t* const* events, const SLONG* values)
{
/**************************************
 *
 *	I S C _ e v e n t _ b l o c k e d	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	If a wait would block, return TRUE.
 *
 **************************************/

	for (; count > 0; --count, ++events, ++values)
		if ((*events)->event_count >= *values) {
#ifdef DEBUG_ISC_SYNC
			printf("event_blocked: FALSE (eg something to report)\n");
			fflush(stdout);
#endif
			return false;
		}

#ifdef DEBUG_ISC_SYNC
	printf("event_blocked: TRUE (eg nothing happened yet)\n");
	fflush(stdout);
#endif
	return true;
}


SLONG ISC_event_clear(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ c l e a r	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Clear an event preparatory to waiting on it.  The order of
 *	battle for event synchronization is:
 *
 *	    1.  Clear event.
 *	    2.  Test data structure for event already completed
 *	    3.  Wait on event.
 *
 **************************************/
	pthread_mutex_lock(event->event_mutex);
	const SLONG ret = event->event_count + 1;
	pthread_mutex_unlock(event->event_mutex);
	return ret;
}


void ISC_event_fini(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ f i n i	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Discard an event object.
 *
 **************************************/

/* Inter-Thread event's are destroyed only */
	if (event->event_semid == -1) {
		pthread_mutex_destroy(event->event_mutex);
		pthread_cond_destroy(event->event_semnum);
	}
}


int ISC_event_init(event_t* event, int semid, int semnum)
{
/**************************************
 *
 *	I S C _ e v e n t _ i n i t	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Prepare an event object for use.
 *
 **************************************/
	event->event_count = 0;

	if (!semnum) {
		/* Prepare an Inter-Thread event block */
		event->event_semid = -1;

		/* Default attribute objects initialize sync. primitives
		   to be used to sync thread within one process only.
		 */
#ifdef HP10
		pthread_mutex_init(event->event_mutex, pthread_mutexattr_default);
		pthread_cond_init(event->event_semnum, pthread_condattr_default);
#else
		pthread_mutex_init(event->event_mutex, NULL);
		pthread_cond_init(event->event_semnum, NULL);
#endif /* HP10 */
	}
	else {
		/* Prepare an Inter-Process event block */
		pthread_mutexattr_t mattr;
		pthread_condattr_t cattr;

		event->event_semid = semid;

		pthread_mutexattr_init(&mattr);
		pthread_condattr_init(&cattr);
#ifdef PTHREAD_PROCESS_SHARED
		pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
		pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
#else
#error Your system must support PTHREAD_PROCESS_SHARED to use firebird.
#endif
		pthread_mutex_init(event->event_mutex, &mattr);
		pthread_cond_init(event->event_semnum, &cattr);
		pthread_mutexattr_destroy(&mattr);
		pthread_condattr_destroy(&cattr);
	}

	return TRUE;
}


int ISC_event_post(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ p o s t	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Post an event to wake somebody else up.
 *
 **************************************/
	pthread_mutex_lock(event->event_mutex);
	++event->event_count;
	const int ret = pthread_cond_broadcast(event->event_semnum);
	pthread_mutex_unlock(event->event_mutex);
	if (ret)
#ifdef HP10

	{
		fb_assert(ret == -1);
		gds__log
			("ISC_event_post: pthread_cond_broadcast failed with errno = %d",
			 errno);
		return errno;
	}
	return 0;

#else

		gds__log
			("ISC_event_post: pthread_cond_broadcast failed with errno = %d",
			 ret);
	return ret;

#endif /* HP10 */
}


int ISC_event_wait(SSHORT count,
				   event_t* const* events,
				   const SLONG* values,
				   const SLONG micro_seconds)
{
/**************************************
 *
 *	I S C _ e v e n t _ w a i t	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Wait on an event.  If timeout limit specified, return
 *	anyway after the timeout even if no event has
 *	happened.  If returning due to timeout, return
 *	FB_FAILURE else return FB_SUCCESS.
 *
 **************************************/

/* While the API for ISC_event_wait allows for a list of events
   we never actually make use of it.  This implementation wont
   support it anyway as Solaris doesn't provide a "wait for one
   of a series of conditions" function */
	fb_assert(count == 1);

/* If we're not blocked, the rest is a gross waste of time */

	if (!event_blocked(count, events, values))
		return FB_SUCCESS;

/* Set up timers if a timeout period was specified. */

	struct timespec timer;
	if (micro_seconds > 0) {
		timer.tv_sec = time(NULL);
		timer.tv_sec += micro_seconds / 1000000;
		timer.tv_nsec = 1000 * (micro_seconds % 1000000);
	}

	int ret = FB_SUCCESS;
	pthread_mutex_lock((*events)->event_mutex);
	for (;;) {
		if (!event_blocked(count, events, values)) {
			ret = FB_SUCCESS;
			break;
		}

		/* The Posix pthread_cond_wait & pthread_cond_timedwait calls
		   atomically release the mutex and start a wait.
		   The mutex is reacquired before the call returns.
		 */
		if (micro_seconds > 0)
		{
			ret =
				pthread_cond_timedwait((*events)->event_semnum,
									   (*events)->event_mutex, &timer);

#ifdef HP10
			if ((ret == -1) && (errno == EAGAIN))
#else
#if (defined LINUX || defined DARWIN || defined HP11 || defined FREEBSD)
			if (ret == ETIMEDOUT)
#else
			if (ret == ETIME)
#endif
#endif
			{

				/* The timer expired - see if the event occured and return
				   FB_SUCCESS or FB_FAILURE accordingly. */

				if (event_blocked(count, events, values))
					ret = FB_FAILURE;
				else
					ret = FB_SUCCESS;
				break;
			}
		}
		else
			ret =
				pthread_cond_wait((*events)->event_semnum,
								  (*events)->event_mutex);
	}
	pthread_mutex_unlock((*events)->event_mutex);
	return ret;
}
#endif /* USE_POSIX_THREADS */


#ifdef WIN_NT
static bool event_blocked(USHORT count, const event_t* const* events, const SLONG* values)
{
/**************************************
 *
 *	I S C _ e v e n t _ b l o c k e d	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	If a wait would block, return TRUE.
 *
 **************************************/

	for (; count > 0; --count, ++events, ++values)
	{
		const event_t* pEvent = *events;
		if (pEvent->event_count >= *values) {
			return false;
		}
	}
	return true;
}


SLONG ISC_event_clear(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ c l e a r	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Clear an event preparatory to waiting on it.  The order of
 *	battle for event synchronization is:
 *
 *	    1.  Clear event.
 *	    2.  Test data structure for event already completed
 *	    3.  Wait on event.
 *
 **************************************/

	ResetEvent((HANDLE) event->event_handle);

	return event->event_count + 1;
}


void ISC_event_fini(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ f i n i	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Discard an event object.
 *
 **************************************/

	if (event->event_pid == process_id)
	{
		CloseHandle((HANDLE) event->event_handle);
	}
}


int ISC_event_init(event_t* event, int unused, int type)
{
/**************************************
 *
 *	I S C _ e v e n t _ i n i t	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Prepare an event object for use.
 *
 **************************************/

	static int idCounter = 0;

	const int id = type ? ++idCounter : 0;

	event->event_pid = process_id = getpid();
	event->event_count = 0;
	event->event_id = id;

	event->event_handle = ISC_make_signal(true, true, process_id, id);

	return (event->event_handle) ? TRUE : FALSE;
}


int ISC_event_post(event_t* event)
{
/**************************************
 *
 *	I S C _ e v e n t _ p o s t	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Post an event to wake somebody else up.
 *
 **************************************/

	++event->event_count;

	if (event->event_pid != process_id)
		return ISC_kill(event->event_pid, event->event_id, event->event_handle);

	return SetEvent((HANDLE) event->event_handle) ? 0 : -1;
}


int ISC_event_wait(SSHORT count,
				   event_t* const* events,
				   const SLONG* values,
				   const SLONG micro_seconds)
{
/**************************************
 *
 *	I S C _ e v e n t _ w a i t	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Wait on an event.
 *
 **************************************/
	/* If we're not blocked, the rest is a gross waste of time */

	if (!event_blocked(count, events, values)) {
		return 0;
	}

#pragma FB_COMPILER_MESSAGE("Warning: B.O. with more than 16 handles")

	HANDLE handles[16];
	HANDLE* handle_ptr = handles;
	const event_t* const* ptr = events;
	for (const event_t* const* const end = events + count; ptr < end;) {
		*handle_ptr++ = (*ptr++)->event_handle;
	}

	/* Go into wait loop */

	const DWORD timeout = (micro_seconds > 0) ? micro_seconds / 1000 : INFINITE;

	for (;;) {
		if (!event_blocked(count, events, values)) {
			return 0;
		}

		const DWORD status =
			WaitForMultipleObjects((DWORD) count, handles, TRUE, timeout);

		if (!((status >= WAIT_OBJECT_0) && (status < WAIT_OBJECT_0 + (DWORD) count)))
		{
			return status;
		}
	}
}

#endif // WIN_NT


#ifdef UNIX
void ISC_exception_post(ULONG sig_num, const TEXT* err_msg)
{
/**************************************
 *
 *	I S C _ e x c e p t i o n _ p o s t ( U N I X )
 *
 **************************************
 *
 * Functional description
 *     When we got a sync exception, fomulate the error code
 *     write it to the log file, and abort.
 *
 * 08-Mar-2004, Nickolay Samofatov.
 *   This function is dangerous and requires rewrite using signal-safe operations only.
 *   Main problem is that we call a lot of signal-unsafe functions from this signal handler,
 *   examples are gds__alloc, gds__log, etc... sprintf is safe on some BSD platforms, 
 *   but not on Linux. This may result in lock-up during signal handling.
 *
 **************************************/
	// If there's no err_msg, we asumed the switch() finds no case or we crash.
	// Too much goodwill put on the caller. Weak programming style.
	// Therefore, lifted this safety net from the NT version.
	if (!err_msg)
	{
		err_msg = "";
	}

	TEXT* log_msg = (TEXT *) gds__alloc(strlen(err_msg) + 256);
	// NOMEM: crash!
	log_msg[0] = '\0';

	switch (sig_num) {
	case SIGSEGV:
		sprintf(log_msg, "%s Segmentation Fault.\n"
				"\t\tThe code attempted to access memory\n"
				"\t\twithout privilege to do so.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case SIGBUS:
		sprintf(log_msg, "%s Bus Error.\n"
				"\t\tThe code caused a system bus error.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case SIGILL:

		sprintf(log_msg, "%s Illegal Instruction.\n"
				"\t\tThe code attempted to perfrom an\n"
				"\t\tillegal operation."
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;

	case SIGFPE:
		sprintf(log_msg, "%s Floating Point Error.\n"
				"\t\tThe code caused an arithmetic exception\n"
				"\t\tor floating point exception."
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	default:
		sprintf(log_msg, "%s Unknown Exception.\n"
				"\t\tException number %"ULONGFORMAT"."
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg, sig_num);
		break;
	}

	if (err_msg) {
		gds__log(log_msg);
		gds__free(log_msg);
	}
	abort();
}
#endif // UNIX


#ifdef WIN_NT
ULONG ISC_exception_post(ULONG except_code, const TEXT* err_msg)
{
/**************************************
 *
 *	I S C _ e x c e p t i o n _ p o s t ( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *     When we got a sync exception, fomulate the error code
 *     write it to the log file, and abort. Note: We can not
 *     actually call "abort" since in windows this will cause
 *     a dialog to appear stating the obvious!  Since on NT we
 *     would not get a core file, there is actually no difference
 *     between abort() and exit(3).
 *
 **************************************/
	ULONG result = 0;
	bool is_critical = true;

	if (!err_msg)
	{
		err_msg = "";
	}

	TEXT* log_msg = (TEXT*) gds__alloc(strlen(err_msg) + 256);
	// NOMEM: crash!
	log_msg[0] = '\0';

	switch (except_code) {
	case EXCEPTION_ACCESS_VIOLATION:
		sprintf(log_msg, "%s Access violation.\n"
				"\t\tThe code attempted to access a virtual\n"
				"\t\taddress without privilege to do so.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		sprintf(log_msg, "%s Datatype misalignment.\n"
				"\t\tThe attempted to read or write a value\n"
				"\t\tthat was not stored on a memory boundary.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		sprintf(log_msg, "%s Array bounds exceeded.\n"
				"\t\tThe code attempted to access an array\n"
				"\t\telement that is out of bounds.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		sprintf(log_msg, "%s Float denormal operand.\n"
				"\t\tOne of the floating-point operands is too\n"
				"\t\tsmall to represent as a standard floating-point\n"
				"\t\tvalue.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		sprintf(log_msg, "%s Floating-point divide by zero.\n"
				"\t\tThe code attempted to divide a floating-point\n"
				"\t\tvalue by a floating-point divisor of zero.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_INEXACT_RESULT:
		sprintf(log_msg, "%s Floating-point inexact result.\n"
				"\t\tThe result of a floating-point operation cannot\n"
				"\t\tbe represented exactly as a decimal fraction.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_INVALID_OPERATION:
		sprintf(log_msg, "%s Floating-point invalid operand.\n"
				"\t\tAn indeterminant error occurred during a\n"
				"\t\tfloating-point operation.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_OVERFLOW:
		sprintf(log_msg, "%s Floating-point overflow.\n"
				"\t\tThe exponent of a floating-point operation\n"
				"\t\tis greater than the magnitude allowed.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_STACK_CHECK:
		sprintf(log_msg, "%s Floating-point stack check.\n"
				"\t\tThe stack overflowed or underflowed as the\n"
				"result of a floating-point operation.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_FLT_UNDERFLOW:
		sprintf(log_msg, "%s Floating-point underflow.\n"
				"\t\tThe exponent of a floating-point operation\n"
				"\t\tis less than the magnitude allowed.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		sprintf(log_msg, "%s Integer divide by zero.\n"
				"\t\tThe code attempted to divide an integer value\n"
				"\t\tby an integer divisor of zero.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_INT_OVERFLOW:
		sprintf(log_msg, "%s Interger overflow.\n"
				"\t\tThe result of an integer operation caused the\n"
				"\t\tmost significant bit of the result to carry.\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg);
		break;
	case EXCEPTION_STACK_OVERFLOW:
		Firebird::status_exception::raise(isc_exception_stack_overflow, isc_arg_end);
		/* This will never be called, but to be safe it's here */
		result = (ULONG) EXCEPTION_CONTINUE_EXECUTION;
		is_critical = false;
		break;

	case EXCEPTION_BREAKPOINT:
	case EXCEPTION_SINGLE_STEP:
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
	case EXCEPTION_INVALID_DISPOSITION:
	case EXCEPTION_PRIV_INSTRUCTION:
	case EXCEPTION_IN_PAGE_ERROR:
	case EXCEPTION_ILLEGAL_INSTRUCTION:
	case EXCEPTION_GUARD_PAGE:
		/* Pass these exception on to someone else,
		   probably the OS or the debugger, since there
		   isn't a dam thing we can do with them */
		result = EXCEPTION_CONTINUE_SEARCH;
		is_critical = false;
		break;
	case 0xE06D7363: /* E == Exception. 0x6D7363 == "msc". Intel and Borland use the same code to be compatible */
		/* If we've caught our own software exception,
		   continue rewinding the stack to properly handle it
		   and deliver an error information to the client side */
		result = EXCEPTION_CONTINUE_SEARCH;
		is_critical = false;
		break;
	default:
		sprintf (log_msg, "%s An exception occurred that does\n"
				"\t\tnot have a description.  Exception number %"XLONGFORMAT".\n"
				"\tThis exception will cause the Firebird server\n"
				"\tto terminate abnormally.", err_msg, except_code);
		break; 
	}

	if (is_critical)
	{
		gds__log(log_msg);
	}

	gds__free(log_msg);

	if (is_critical)
	{
		if (Config::getBugcheckAbort()) {
			// Pass exception to outer handler in case debugger is present to collect memory dump
			return EXCEPTION_CONTINUE_SEARCH;
		}

		// Silently exit so guardian or service manager can restart the server.
		// If exception is getting out of the application Windows displays a message
		// asking if you want to send report to Microsoft or attach debugger,
		// application is not terminated until you press some button on resulting window.
		// This happens even if you run application as non-interactive service on 
		// "server" OS like Windows Server 2003.
		exit(3);
	}

	return result;
}
#endif // WIN_NT


#ifdef WIN_NT
void *ISC_make_signal(
	  bool create_flag,
	  bool manual_reset,
	  int process_idL,
	  int signal_number)
{
/**************************************
 *
 *	I S C _ m a k e _ s i g n a l		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Create or open a Windows/NT event.
 *	Use the signal number and process id
 *	in naming the object.
 *
 **************************************/

	const BOOLEAN man_rst = manual_reset ? TRUE : FALSE;

	if (!signal_number)
		return CreateEvent(NULL, man_rst, FALSE, NULL);

	TEXT event_name[BUFFER_TINY];
	sprintf(event_name, "_firebird_process%u_signal%d", process_idL, signal_number);

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, event_name);

	if (create_flag) {
		fb_assert(!hEvent);
		hEvent = CreateEvent(ISC_get_security_desc(), man_rst, FALSE, event_name);
	}

	return hEvent;
}
#endif


#ifdef UNIX
#ifdef HAVE_MMAP
UCHAR* ISC_map_file(ISC_STATUS* status_vector,
					const TEXT* filename,
					FPTR_INIT_GLOBAL_REGION init_routine,
					void* init_arg, SLONG length, SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ m a p _ f i l e		( U N I X - m m a p )
 *
 **************************************
 *
 * Functional description
 *	Try to map a given file.  If we are the first (i.e. only)
 *	process to map the file, call a given initialization
 *	routine (if given) or punt (leaving the file unmapped).
 *
 **************************************/
	TEXT expanded_filename[MAXPATHLEN], hostname[64];
	sprintf(expanded_filename, filename,
			ISC_get_host(hostname, sizeof(hostname)));

/* make the complete filename for the init file this file is to be used as a
   master lock to eliminate possible race conditions with just a single file
   locking. The race condition is caused as the conversion of a EXCLUSIVE
   lock to a SHARED lock is not atomic*/

	TEXT tmp[MAXPATHLEN];
	gds__prefix_lock(tmp, INIT_FILE);
	TEXT init_filename[MAXPATHLEN];	/* to hold the complete filename
									   of the init file. */
	sprintf(init_filename, tmp, hostname);	/* already have the hostname! */

	const int oldmask = umask(0);
	bool trunc_flag = true;
	if (length < 0) {
		length = -length;
		trunc_flag = false;
	}

/* Produce shared memory key for file */

	SLONG key;
	if (!(key = find_key(status_vector, expanded_filename))) {
		umask(oldmask);
		return NULL;
	}

/* open the init lock file */
	int fd_init;				/* filedecr. for the init file */
	fd_init = open(init_filename, O_RDWR | O_CREAT, 0666);
	if (fd_init == -1) {
		error(status_vector, "open", errno);
		return NULL;
	}

#ifndef HAVE_FLOCK
	struct flock lock;
/* get an exclusive lock on the INIT file with a block */
	lock.l_type = F_WRLCK;
	lock.l_whence = 0;
	lock.l_start = 0;
	lock.l_len = 0;
	if (fcntl(fd_init, F_SETLKW, &lock) == -1) {
		error(status_vector, "fcntl", errno);
		close(fd_init);
		return NULL;
	}
#else
/* get an flock exclusive on the INIT file with blocking */
	if (flock(fd_init, LOCK_EX)) {
		/* we failed to get an exclusive lock return back */
		error(status_vector, "flock", errno);
		close(fd_init);
		return NULL;
	}
#endif
/* open the file to be inited */
	int fd = open(expanded_filename, O_RDWR | O_CREAT, 0666);
	umask(oldmask);

	if (fd == -1) {
		error(status_vector, "open", errno);
#ifdef HAVE_FLOCK
		/* unlock init file */
		flock(fd_init, LOCK_UN);
#else
		lock.l_type = F_UNLCK;
		lock.l_whence = 0;
		lock.l_start = 0;
		lock.l_len = 0;
		fcntl(fd_init, F_SETLK, &lock);
#endif
		close(fd_init);
		return NULL;
	}

	if (length == 0) {
		/* Get and use the existing length of the shared segment */

		struct stat file_stat;
		if (fstat(fd, &file_stat) == -1) {
			error(status_vector, "fstat", errno);
			close(fd);
#ifdef HAVE_FLOCK
			/* unlock init file */
			flock(fd_init, LOCK_UN);
#else
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd_init, F_SETLK, &lock);
#endif
			close(fd_init);		/* while we are at it close the init file also */
			return NULL;
		}
		length = file_stat.st_size;
	}


	UCHAR* address =
		(UCHAR *) mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if ((U_IPTR) address == (U_IPTR) -1) {
		error(status_vector, "mmap", errno);
		close(fd);
#ifdef HAVE_FLOCK
		/* unlock init file */
		flock(fd_init, LOCK_UN);
#else
		lock.l_type = F_UNLCK;
		lock.l_whence = 0;
		lock.l_start = 0;
		lock.l_len = 0;
		fcntl(fd_init, F_SETLK, &lock);
#endif

		close(fd_init);
		return NULL;
	}

/* Get semaphore for mutex */

	shmem_data->sh_mem_address = address;
	shmem_data->sh_mem_length_mapped = length;
	shmem_data->sh_mem_handle = fd;

	if (!shmem_data->sh_mem_length_mapped)
	{
		error(status_vector, "shmem_data->sh_mem_length_mapped is 0", 0);
		return NULL;
	}

/* Try to get an exclusive lock on the lock file.  This will
   fail if somebody else has the exclusive lock */

#ifdef HAVE_FLOCK
	if (!flock(fd, LOCK_EX | LOCK_NB))
	{
		if (!init_routine) {
			/* unlock both files */
			flock(fd, LOCK_UN);
			flock(fd_init, LOCK_UN);
#else
	lock.l_type = F_WRLCK;
	lock.l_whence = 0;
	lock.l_start = 0;
	lock.l_len = 0;
	if (fcntl(fd, F_SETLK, &lock) != -1) {
		if (!init_routine) {
			/* unlock the file and the init file to release the other process */
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd, F_SETLK, &lock);

			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd_init, F_SETLK, &lock);
#endif
			munmap((char *) address, length);
			close(fd);
			close(fd_init);
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_unavailable;
			*status_vector++ = isc_arg_end;
			return NULL;
		}

		if (trunc_flag)
			ftruncate(fd, length);
		(*init_routine) (init_arg, shmem_data, true);
#ifdef HAVE_FLOCK
		if (flock(fd, LOCK_SH)) {
			error(status_vector, "flock", errno);
			flock(fd, LOCK_UN);
			flock(fd_init, LOCK_UN);
#else
		lock.l_type = F_RDLCK;
		lock.l_whence = 0;
		lock.l_start = 0;
		lock.l_len = 0;
		if (fcntl(fd, F_SETLK, &lock) == -1)
		{
			error(status_vector, "fcntl", errno);
			/* unlock the file */
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd, F_SETLK, &lock);

			/* unlock the init file to release the other process */
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd_init, F_SETLK, &lock);
#endif
			munmap((char *) address, length);

			close(fd_init);
			close(fd);
			return NULL;
		}
	}
	else {
#ifdef HAVE_FLOCK
		if (flock(fd, LOCK_SH)) {
			error(status_vector, "flock", errno);
			flock(fd, LOCK_UN);
			flock(fd_init, LOCK_UN);
#else
		lock.l_type = F_RDLCK;
		lock.l_whence = 0;
		lock.l_start = 0;
		lock.l_len = 0;
		if (fcntl(fd, F_SETLK, &lock) == -1) {
			error(status_vector, "fcntl", errno);
			/* unlock the file */
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd, F_SETLK, &lock);

			/* unlock the init file to release the other process */
			lock.l_type = F_UNLCK;
			lock.l_whence = 0;
			lock.l_start = 0;
			lock.l_len = 0;
			fcntl(fd_init, F_SETLK, &lock);
#endif
			munmap((char *) address, length);
			close(fd_init);
			close(fd);
			return NULL;
		}

		if (init_routine)
			(*init_routine) (init_arg, shmem_data, false);
	}

#ifdef HAVE_FLOCK
/* unlock the init file to release the other process */
	flock(fd_init, LOCK_UN);
#else
/* unlock the init file to release the other process */
	lock.l_type = F_UNLCK;
	lock.l_whence = 0;
	lock.l_start = 0;
	lock.l_len = 0;
	fcntl(fd_init, F_SETLK, &lock);
#endif
/* close the init file_decriptor */
	close(fd_init);
	return address;
}
#endif
#endif


#ifdef UNIX
#ifndef HAVE_MMAP
UCHAR* ISC_map_file(ISC_STATUS* status_vector,
					const TEXT* filename,
					FPTR_INIT_GLOBAL_REGION init_routine,
					void* init_arg, SLONG length, SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ m a p _ f i l e		( U N I X - s h m a t )
 *
 **************************************
 *
 * Functional description
 *	Try to map a given file.  If we are the first (i.e. only)
 *	process to map the file, call a given initialization
 *	routine (if given) or punt (leaving the file unmapped).
 *
 **************************************/
	//SSHORT count;
	TEXT expanded_filename[512];
#ifdef NOHOSTNAME
	strcpy(expanded_filename, filename);
#else
	TEXT hostname[64];
	sprintf(expanded_filename, filename,
			ISC_get_host(hostname, sizeof(hostname)));
#endif
	const int oldmask = umask(0);
	bool init_flag = false;
	if (length < 0)
		length = -length;

/* Produce shared memory key for file */

	const SLONG key = find_key(status_vector, expanded_filename);
	if (!key) {
		umask(oldmask);
		return NULL;
	}

/* Write shared memory key into expanded_filename file */

	FILE* fp = fopen(expanded_filename, "w");
	umask(oldmask);

	if (!fp) {
		error(status_vector, "fopen", errno);
		return NULL;
	}

	fprintf(fp, "%ld", key);

/* Get an exclusive lock on the file until the initialization process
   is complete.  That way potential race conditions are avoided. */

#ifndef HAVE_FLOCK
	if (lockf(fileno(fp), F_LOCK, 0)) {
		error(status_vector, "lockf", errno);
#else
	if (flock(fileno(fp), LOCK_EX)) {
		error(status_vector, "flock", errno);
#endif
		fclose(fp);
		return NULL;
	}

/* Create the shared memory region if it doesn't already exist. */

	struct shmid_ds buf;
	SLONG shmid;
	if ((shmid = shmget(key, length, IPC_CREAT | PRIV)) == -1)
#ifdef SUPERSERVER
		if (errno == EINVAL) {
			/* There are two cases when shmget() returns EINVAL error:

			   - "length" is less than the system-imposed minimum or
			   greater than the system-imposed maximum.

			   - A shared memory identifier exists for "key" but the
			   size of the segment associated with it is less
			   than "length" and "length" is not equal to zero.

			   Let's find out what the problem is by getting the
			   system-imposed limits.
			 */

#ifdef HP10
			struct pst_ipcinfo pst;

			if (pstat_getipc(&pst, sizeof(struct pst_ipcinfo), 1, 0) == -1) {
				error(status_vector, "pstat_getipc", errno);
				fclose(fp);
				return NULL;
			}

			if ((length < pst.psi_shmmin) || (length > pst.psi_shmmax)) {
				/* If pstat_getipc did not return an error "errno"
				   is still EINVAL, exactly what we want.
				 */
				error(status_vector, "shmget", errno);
				fclose(fp);
				return NULL;
			}
#endif /* HP10 */

			/* If we are here then the shared memory segment already
			   exists and the "length" we specified in shmget() is
			   bigger than the size of the existing segment.

			   Because the segment has to exist at this point the
			   following shmget() does not have IPC_CREAT flag set.

			   We need to get shmid to delete the segment. Because we
			   do not know the size of the existing segment the easiest
			   way to get shmid is to attach to the segment with zero
			   length
			 */
			if ((shmid = shmget(key, 0, PRIV)) == -1) {
				error(status_vector, "shmget", errno);
				fclose(fp);
				return NULL;
			}

			if (shmctl(shmid, IPC_RMID, &buf) == -1) {
				error(status_vector, "shmctl/IPC_RMID", errno);
				fclose(fp);
				return NULL;
			}

			/* We have just deleted shared memory segment and current
			   code fragment is an atomic operation (we are holding an
			   exclusive lock on the "isc_lock1.<machine>" file), so
			   we use IPC_EXCL flag to get an error if by some miracle
			   the sagment with the same key is already exists
			 */
			if ((shmid = shmget(key, length, IPC_CREAT | IPC_EXCL | PRIV)) ==
				-1)
			{
				error(status_vector, "shmget", errno);
				fclose(fp);
				return NULL;
			}
		}
		else					/* if errno != EINVAL) */
#endif /* SUPERSERVER */
		{
			error(status_vector, "shmget", errno);
			fclose(fp);
			return NULL;
		}

#ifdef SUPERSERVER
/* If we are here there are two possibilities:

   1. we mapped the shared memory segment of the "length" size;
   2. we mapped the segment of the size less than "length" (but
      not zero length and bigger than system-imposed minimum);

   We want shared memory segment exactly of the "length" size.
   Let's find out what the size of the segment is and if it is
   bigger than length, we remove it and create new one with the
   size "length".
   Also, if "length" is zero (that means we have already mapped
   the existing segment with the zero size) remap the segment
   with the existing size
*/
	if (shmctl(shmid, IPC_STAT, &buf) == -1) {
		error(status_vector, "shmctl/IPC_STAT", errno);
		fclose(fp);
		return NULL;
	}

	fb_assert(length <= buf.shm_segsz);
	if (length < buf.shm_segsz)
		if (length) {
			if (shmctl(shmid, IPC_RMID, &buf) == -1) {
				error(status_vector, "shmctl/IPC_RMID", errno);
				fclose(fp);
				return NULL;
			}

			if ((shmid = shmget(key, length, IPC_CREAT | IPC_EXCL | PRIV)) ==
				-1)
			{
				error(status_vector, "shmget", errno);
				fclose(fp);
				return NULL;
			}
		}
		else {
			length = buf.shm_segsz;
			if ((shmid = shmget(key, length, PRIV)) == -1) {
				error(status_vector, "shmget", errno);
				fclose(fp);
				return NULL;
			}
		}
#else /* !SUPERSERVER */

	if (length == 0) {
		/* Use the existing length.  This should not happen for the
		   very first attachment to the shared memory.  */

		if (shmctl(shmid, IPC_STAT, &buf) == -1) {
			error(status_vector, "shmctl/IPC_STAT", errno);
			fclose(fp);
			return NULL;
		}
		length = buf.shm_segsz;

		/* Now remap with the new-found length */

		if ((shmid = shmget(key, length, PRIV)) == -1) {
			error(status_vector, "shmget", errno);
			fclose(fp);
			return NULL;
		}
	}
#endif /* SUPERSERVER */


	UCHAR* address = 0;
#ifdef SHMEM_PICKY
	if (!next_shared_memory)
		next_shared_memory = (UCHAR *) sbrk(0) + SHMEM_DELTA;
	address = (UCHAR *) shmat(shmid, next_shared_memory, SHM_RND);
	if ((U_IPTR) address != -1)
#ifndef SYSV_SHMEM
		next_shared_memory = address + length;
#else
		next_shared_memory = address + length + SHMLBA;
#endif
#else
	address = (UCHAR *) shmat(shmid, NULL, 0);
#endif

	if ((U_IPTR) address == -1) {
		error(status_vector, "shmat", errno);
		fclose(fp);
		return NULL;
	}

	if (shmctl(shmid, IPC_STAT, &buf) == -1) {
		error(status_vector, "shmctl/IPC_STAT", errno);
		shmdt(address);
		next_shared_memory -= length;
		fclose(fp);
		return NULL;
	}

/* Get semaphore for mutex */


/* If we're the only one with shared memory mapped, see if
   we can initialize it.  If we can't, return failure. */

	if (buf.shm_nattch == 1) {
		if (!init_routine) {
			shmdt(address);
			next_shared_memory -= length;
			fclose(fp);
			*status_vector++ = isc_arg_gds;
			*status_vector++ = isc_unavailable;
			*status_vector++ = isc_arg_end;
			return NULL;
		}
		buf.shm_perm.mode = 0666;
		shmctl(shmid, IPC_SET, &buf);
		init_flag = true;
	}

	shmem_data->sh_mem_address = address;
	shmem_data->sh_mem_length_mapped = length;
	shmem_data->sh_mem_handle = shmid;

	if (init_routine)
		(*init_routine) (init_arg, shmem_data, init_flag);

/* When the mapped file is closed here, the lock we applied for
   synchronization will be released. */

	fclose(fp);

	return address;
}
#endif // !HAVE_MMAP

#endif // UNIX


#ifdef WIN_NT
UCHAR* ISC_map_file(
	   ISC_STATUS* status_vector,
	   const TEXT* filename,
	   FPTR_INIT_GLOBAL_REGION init_routine,
	   void* init_arg,
	   SLONG length,
	   SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ m a p _ f i l e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Try to map a given file.  If we are the first (i.e. only)
 *	process to map the file, call a given initialization
 *	routine (if given) or punt (leaving the file unmapped).
 *
 **************************************/
	TEXT expanded_filename[MAXPATHLEN], hostname[64];
	TEXT map_file[MAXPATHLEN];
	HANDLE file_handle, event_handle;
	int retry_count = 0;

	bool trunc_flag = true;
	if (length < 0) {
		length = -length;
		trunc_flag = false;
	}

/* retry to attach to mmapped file if the process initializing
 * dies during initialization.
 */

  retry:
	retry_count++;

	ISC_get_host(hostname, sizeof(hostname));
	sprintf(map_file, filename, hostname);

	file_handle = CreateFile(map_file,
				 GENERIC_READ | GENERIC_WRITE,
				 FILE_SHARE_READ | FILE_SHARE_WRITE,
				 NULL,
				 OPEN_ALWAYS,
#ifdef EMBEDDED
				 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE,
#else
				 FILE_ATTRIBUTE_NORMAL,
#endif
				 NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		error(status_vector, "CreateFile", GetLastError());
		return NULL;
	}

/* Check if file already exists */

	const bool file_exists = (GetLastError() == ERROR_ALREADY_EXISTS);

/* Create an event that can be used to determine if someone has already
   initialized shared memory. */

	make_object_name(expanded_filename, sizeof(expanded_filename), filename, "_event");
	event_handle = CreateEvent(ISC_get_security_desc(), TRUE, FALSE, expanded_filename);
	if (!event_handle) {
		error(status_vector, "CreateEvent", GetLastError());
		CloseHandle(file_handle);
		return NULL;
	}

	bool init_flag = (GetLastError() != ERROR_ALREADY_EXISTS);

	if (init_flag && !init_routine) {
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_unavailable;
		*status_vector++ = isc_arg_end;
		return NULL;
	}

	if (length == 0) {
		/* Get and use the existing length of the shared segment */

		if ((length = GetFileSize(file_handle, NULL)) == -1) {
			error(status_vector, "GetFileSize", GetLastError());
			CloseHandle(event_handle);
			CloseHandle(file_handle);
			return NULL;
		}
	}

/* All but the initializer will wait until the event is set.  That
 * is done after initialization is complete.
 * Close the file and wait for the event to be set or time out.
 * The file may be truncated.
 */

	CloseHandle(file_handle);

	if (!init_flag) {
		/* Wait for 10 seconds.  Then retry */

		const DWORD ret_event = WaitForSingleObject(event_handle, 10000);

		/* If we timed out, just retry.  It is possible that the
		 * process doing the initialization died before setting the
		 * event.
		 */

		if (ret_event == WAIT_TIMEOUT) {
			CloseHandle(event_handle);
			if (retry_count > 10) {
				error(status_vector, "WaitForSingleObject", GetLastError());
				return NULL;
			}
			goto retry;
		}
	}

	DWORD fdw_create;
	if (init_flag && file_exists && trunc_flag)
		fdw_create = TRUNCATE_EXISTING | OPEN_ALWAYS;
	else
		fdw_create = OPEN_ALWAYS;

	file_handle = CreateFile(map_file,
				 GENERIC_READ | GENERIC_WRITE,
				 FILE_SHARE_READ | FILE_SHARE_WRITE,
				 NULL,
				 fdw_create,
#ifdef EMBEDDED
				 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE,
#else
				 FILE_ATTRIBUTE_NORMAL,
#endif
				 NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		CloseHandle(event_handle);
		error(status_vector, "CreateFile", GetLastError());
		return NULL;
	}

/* Create a file mapping object that will be used to make remapping possible.
   The current length of real mapped file and its name are saved in it. */

	make_object_name(expanded_filename, sizeof(expanded_filename), filename, "_mapping");

	HANDLE header_obj = CreateFileMapping ((HANDLE) -1,
				ISC_get_security_desc(),
				PAGE_READWRITE,
				0,
				2 * sizeof (SLONG),
				expanded_filename);
	if (header_obj == NULL) 
	{
		error(status_vector, "CreateFileMapping", GetLastError());
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		return NULL;
	}

	if (!init_flag && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		// We have made header_obj but we are not initializing.
		// Previous owner is closed and clear all header_data.
		// One need to retry.
		CloseHandle(header_obj);
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		goto retry;
	}

	SLONG* header_address =
		(SLONG*) MapViewOfFile(header_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (header_address == NULL) {
		error(status_vector, "MapViewOfFile", GetLastError());
		CloseHandle(header_obj);
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		return NULL;
	}

/* Set or get the true length of the file depending on whether or not
   we are the first user. */

	if (init_flag) {
		header_address[0] = length;
		header_address[1] = 0;
	}
	else
		length = header_address[0];

/* Create the real file mapping object. */

	TEXT* p;
	for (p = expanded_filename; *p; p++);
	sprintf(p, "%"SLONGFORMAT, header_address[1]);

	HANDLE file_obj =
		CreateFileMapping(file_handle,
				  ISC_get_security_desc(),
				  PAGE_READWRITE,
				  0,
				  length,
				  expanded_filename);
	if (file_obj == NULL) {
		error(status_vector, "CreateFileMapping", GetLastError());
		UnmapViewOfFile(header_address);
		CloseHandle(header_obj);
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		return NULL;
	}

	UCHAR* address =
		(UCHAR*) MapViewOfFile(file_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (address == NULL) {
		error(status_vector, "MapViewOfFile", GetLastError());
		CloseHandle(file_obj);
		UnmapViewOfFile(header_address);
		CloseHandle(header_obj);
		CloseHandle(event_handle);
		CloseHandle(file_handle);
		return NULL;
	}

	*p = 0;

	shmem_data->sh_mem_address = address;
	shmem_data->sh_mem_length_mapped = length;

	if (!shmem_data->sh_mem_length_mapped)
	{
		error(status_vector, "shmem_data->sh_mem_length_mapped is 0", 0);
		return NULL;
	}

	shmem_data->sh_mem_handle = file_handle;
	shmem_data->sh_mem_object = file_obj;
	shmem_data->sh_mem_interest = event_handle;
	shmem_data->sh_mem_hdr_object = header_obj;
	shmem_data->sh_mem_hdr_address = header_address;
	strcpy(shmem_data->sh_mem_name, expanded_filename);

	if (init_routine)
		(*init_routine) (init_arg, shmem_data, init_flag);

	if (init_flag) {
		FlushViewOfFile(address, 0);
		SetEvent(event_handle);
		if (SetFilePointer
			(shmem_data->sh_mem_handle, length, NULL,
			 FILE_BEGIN) == 0xFFFFFFFF
			|| !SetEndOfFile(shmem_data->sh_mem_handle)
			|| !FlushViewOfFile(shmem_data->sh_mem_address, 0))
		{
			error(status_vector, "SetFilePointer", GetLastError());
			return NULL;
		}
	}

	return address;
}
#endif


#ifdef HAVE_MMAP
UCHAR *ISC_map_object(ISC_STATUS * status_vector,
					  SH_MEM shmem_data,
					  SLONG object_offset, SLONG object_length)
{
/**************************************
 *
 *	I S C _ m a p _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Try to map an object given a file mapping.
 *
 **************************************/
/* Get system page size as this is the unit of mapping. */

#ifdef SOLARIS
	const SLONG page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1) {
		error(status_vector, "sysconf", errno);
		return NULL;
	}
#else
	const SLONG page_size = (int) getpagesize();
	if (page_size == -1) {
		error(status_vector, "getpagesize", errno);
		return NULL;
	}
#endif

/* Compute the start and end page-aligned offsets which
   contain the object being mapped. */

	const SLONG start = (object_offset / page_size) * page_size;
	const SLONG end = (((object_offset + object_length) / page_size) + 1) * page_size;
	const SLONG length = end - start;
	int fd = shmem_data->sh_mem_handle;

	UCHAR* address =
		(UCHAR *) mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
					   start);

	if ((U_IPTR) address == (U_IPTR) -1) {
		error(status_vector, "mmap", errno);
		return NULL;
	}

/* Return the virtual address of the mapped object. */

	return (address + (object_offset - start));
}


void ISC_unmap_object(ISC_STATUS * status_vector,
						 SH_MEM shmem_data,
						 UCHAR ** object_pointer, SLONG object_length)
{
/**************************************
 *
 *	I S C _ u n m a p _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Try to unmap an object given a file mapping.
 *	Zero the object pointer after a successful unmap.
 *
 **************************************/
/* Get system page size as this is the unit of mapping. */

#ifdef SOLARIS
	const SLONG page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1) {
		error(status_vector, "sysconf", errno);
		return; // false;
	}
#else
	const SLONG page_size = (int) getpagesize();
	if (page_size == -1) {
		error(status_vector, "getpagesize", errno);
		return; // false;
	}
#endif

/* Compute the start and end page-aligned addresses which
   contain the mapped object. */

	UCHAR* start = (UCHAR *) ((U_IPTR) * object_pointer & ~(page_size - 1));
	const UCHAR* end =
		(UCHAR
		 *) ((U_IPTR) ((*object_pointer + object_length) +
					   (page_size - 1)) & ~(page_size - 1));
	const SLONG length = end - start;

	if (munmap((char *) start, length) == -1) {
		error(status_vector, "munmap", errno);
		return; // false;
	}

	*object_pointer = NULL;
	return; // true;
}
#endif


#ifdef WIN_NT
UCHAR* ISC_map_object(ISC_STATUS* status_vector,
					  SH_MEM shmem_data,
					  SLONG object_offset, SLONG object_length)
{
/**************************************
 *
 *	I S C _ m a p _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Try to map an object given a file mapping.
 *
 **************************************/

	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	const SLONG page_size = sys_info.dwAllocationGranularity;

	// Compute the start and end page-aligned offsets which
	// contain the object being mapped.

	const SLONG start = (object_offset / page_size) * page_size;
	const SLONG end = (((object_offset + object_length) / page_size) + 1) * page_size;
	const SLONG length = end - start;
	const HANDLE handle = shmem_data->sh_mem_object;

	UCHAR* address = (UCHAR*) MapViewOfFile(handle, FILE_MAP_WRITE, 0, start, length);

	if (address == NULL) {
		error(status_vector, "MapViewOfFile", GetLastError());
		return NULL;
	}

	// Return the virtual address of the mapped object.

	return (address + (object_offset - start));
}


void ISC_unmap_object(ISC_STATUS* status_vector,
					  SH_MEM shmem_data,
					  UCHAR** object_pointer, SLONG object_length)
{
/**************************************
 *
 *	I S C _ u n m a p _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Try to unmap an object given a file mapping.
 *	Zero the object pointer after a successful unmap.
 *
 **************************************/
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	const SLONG page_size = sys_info.dwAllocationGranularity;

	// Compute the start and end page-aligned offsets which
	// contain the object being mapped.

	const UCHAR* start = (UCHAR*) ((U_IPTR) *object_pointer & ~(page_size - 1));
	UnmapViewOfFile(start);

	*object_pointer = NULL;
}
#endif


#ifdef SOLARIS_MT
int ISC_mutex_init(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ i n i t	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Initialize a mutex.
 *
 **************************************/

	return mutex_init(mutex->mtx_mutex, USYNC_PROCESS, NULL);
}


int ISC_mutex_lock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Sieze a mutex.
 *
 **************************************/
	for (;;) {
		int state = mutex_lock(mutex->mtx_mutex);
		if (!state)
			break;
		if (!SYSCALL_INTERRUPTED(state))
			return state;
	}

	return 0;
}


int ISC_mutex_lock_cond(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k _ c o n d	( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Conditionally sieze a mutex.
 *
 **************************************/
	for (;;) {
		int state = mutex_trylock(mutex->mtx_mutex);
		if (!state)
			break;
		if (!SYSCALL_INTERRUPTED(state))
			return state;
	}

	return 0;
}


int ISC_mutex_unlock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ u n l o c k		( S O L A R I S _ M T )
 *
 **************************************
 *
 * Functional description
 *	Release a mutex.
 *
 **************************************/
	for (;;) {
		/* Note use of undocumented lwp_mutex_unlock call
		 * due to Solaris 2.4 bug */
		int state = _lwp_mutex_unlock(mutex->mtx_mutex);
		if (!state)
			break;
		if (!SYSCALL_INTERRUPTED(state))
			return state;
	}

	return 0;
}
#endif /* SOLARIS_MT */


#ifdef USE_POSIX_THREADS
int ISC_mutex_init(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ i n i t	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Initialize a mutex.
 *
 **************************************/
	pthread_mutexattr_t mattr;

	int state = pthread_mutexattr_init(&mattr);
	if (state == 0)
	{
#if _POSIX_THREAD_PROCESS_SHARED >= 200112L
		pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
#endif
		state = pthread_mutex_init(mutex->mtx_mutex, &mattr);
		pthread_mutexattr_destroy(&mattr);
	}
#ifdef HP10
	if (state != 0)
    		state = errno;
#endif
	return state;
}


int ISC_mutex_lock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Sieze a mutex.
 *
 **************************************/
#ifdef HP10
	int state = pthread_mutex_lock(mutex->mtx_mutex);
	if (!state)
		return 0;
	fb_assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_lock(mutex->mtx_mutex);

#endif /* HP10 */
}


int ISC_mutex_lock_cond(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k _ c o n d	( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Conditionally sieze a mutex.
 *
 **************************************/
#ifdef HP10
	int state = pthread_mutex_trylock(mutex->mtx_mutex);

/* HP's interpretation of return codes is different than Solaris
   (and Posix Standard?). Usually in case of error they return
   -1 and set errno to whatever error number is.
   pthread_mutex_trylock() is a special case:

	return	errno	description
	  1		Success
	  0		mutex has alreary been locked. Could not get it
	 -1	EINVAL	invalid value of mutex
*/
	if (!state)
		return -99;				/* we did not get the mutex for it had already      */
	/* been locked, let's return something which is     */
	/* not zero and negative (errno values are positive) */
	if (state == 1)
		return 0;

	fb_assert(state == -1);		/* if state is not 0 or 1, it should be -1 */
	return errno;

#else

	return pthread_mutex_trylock(mutex->mtx_mutex);

#endif /* HP10 */
}


int ISC_mutex_unlock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ u n l o c k		( P O S I X _ T H R E A D S )
 *
 **************************************
 *
 * Functional description
 *	Release a mutex.
 *
 **************************************/
#ifdef HP10
	int state = pthread_mutex_unlock(mutex->mtx_mutex);
	if (!state)
		return 0;
	fb_assert(state == -1);		/* if state is not 0, it should be -1 */
	return errno;

#else

	return pthread_mutex_unlock(mutex->mtx_mutex);

#endif /* HP10 */
}
#endif /* USE_POSIX_THREADS */


#ifdef WIN_NT

static const LPCSTR FAST_MUTEX_EVT_NAME	= "%s_FM_EVT";
static const LPCSTR FAST_MUTEX_MAP_NAME	= "%s_FM_MAP";

static const int DEFAULT_INTERLOCKED_SPIN_COUNT	= 0;
static const int DEFAULT_INTERLOCKED_SPIN_COUNT_SMP	= 200;


typedef WINBASEAPI BOOL (WINAPI *pfnSwitchToThread) ();
static inline BOOL switchToThread()
{
	static pfnSwitchToThread fnSwitchToThread = NULL;
	static bool bInit = false;

	if (!bInit)
	{
		HMODULE hLib = GetModuleHandle("kernel32.dll");
		if (hLib) {
			fnSwitchToThread = (pfnSwitchToThread) GetProcAddress(hLib, "SwitchToThread");
		}
		bInit = true;
	}

	BOOL res = FALSE;
	if (fnSwitchToThread) 
	{
#if !defined SUPERSERVER && !defined EMBEDDED
		const HANDLE hThread = GetCurrentThread();
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
#endif

		res = (*fnSwitchToThread) ();

#if !defined SUPERSERVER && !defined EMBEDDED
		SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
#endif
	}

	return res;
}


// MinGW has the wrong declaration for the operating system function.
#if defined __GNUC__
// Cast away volatile
#define FIX_TYPE(arg) const_cast<LPLONG>(arg)
#else
#define FIX_TYPE(arg) arg
#endif


static inline void lockSharedSection(volatile FAST_MUTEX_SHARED_SECTION* lpSect, ULONG SpinCount)
{
	while (InterlockedExchange(FIX_TYPE(&lpSect->lSpinLock), 1) != 0)
	{
		ULONG j = SpinCount; 
		while (j != 0)
		{
			if (lpSect->lSpinLock == 0)
				goto next;
			j--;
		}
		switchToThread();
next:;
	}
}

static inline bool tryLockSharedSection(volatile FAST_MUTEX_SHARED_SECTION* lpSect)
{
	return (InterlockedExchange(FIX_TYPE(&lpSect->lSpinLock), 1) == 0);
}

static inline void unlockSharedSection(volatile FAST_MUTEX_SHARED_SECTION* lpSect)
{
	InterlockedExchange(FIX_TYPE(&lpSect->lSpinLock), 0);
}

static DWORD enterFastMutex(FAST_MUTEX* lpMutex, DWORD dwMilliseconds)
{
	volatile FAST_MUTEX_SHARED_SECTION* lpSect = lpMutex->lpSharedInfo;

	while (true)
	{
		if (dwMilliseconds == 0) {
			if (!tryLockSharedSection(lpSect))
				return WAIT_TIMEOUT;
		}
		else {
			lockSharedSection(lpSect, lpMutex->lSpinCount);
		}

		if (lpSect->lAvailable > 0)
		{
			lpSect->lAvailable--;
#ifdef _DEBUG
			lpSect->dwThreadId = GetCurrentThreadId();
#endif
			unlockSharedSection(lpSect);
			return WAIT_OBJECT_0;
		}

#ifdef _DEBUG
		if (lpSect->dwThreadId == GetCurrentThreadId())
			DebugBreak();
#endif
		if (dwMilliseconds == 0)
		{
			unlockSharedSection(lpSect);
			return WAIT_TIMEOUT;
		}

		InterlockedIncrement(FIX_TYPE(&lpSect->lThreadsWaiting));
		unlockSharedSection(lpSect);
		
		// TODO actual timeout can be of any length
		const DWORD dwResult = WaitForSingleObject(lpMutex->hEvent, dwMilliseconds);
		InterlockedDecrement(FIX_TYPE(&lpSect->lThreadsWaiting));
		
		if (dwResult != WAIT_OBJECT_0)
			return dwResult;
	}
}

static bool leaveFastMutex(FAST_MUTEX* lpMutex)
{
	volatile FAST_MUTEX_SHARED_SECTION* lpSect = lpMutex->lpSharedInfo;

	lockSharedSection(lpSect, lpMutex->lSpinCount);
	if (lpSect->lAvailable >= 1)
	{
		unlockSharedSection(lpSect);
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
	}
	lpSect->lAvailable++;
	if (lpSect->lThreadsWaiting)
		SetEvent(lpMutex->hEvent);
	unlockSharedSection(lpSect);

	return true;
}

static inline void deleteFastMutex(FAST_MUTEX* lpMutex)
{
	UnmapViewOfFile((FAST_MUTEX_SHARED_SECTION*)lpMutex->lpSharedInfo);
	CloseHandle(lpMutex->hFileMap);
	CloseHandle(lpMutex->hEvent);
}

static inline void setupMutex(FAST_MUTEX* lpMutex)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	if (si.dwNumberOfProcessors > 1)
		lpMutex->lSpinCount = DEFAULT_INTERLOCKED_SPIN_COUNT_SMP;
	else
		lpMutex->lSpinCount = DEFAULT_INTERLOCKED_SPIN_COUNT;
}

static bool initializeFastMutex(FAST_MUTEX* lpMutex, LPSECURITY_ATTRIBUTES lpAttributes, 
								BOOL bInitialState, LPCSTR lpName)
{
	LPCSTR name = lpName;

	if (strlen(lpName) + strlen(FAST_MUTEX_EVT_NAME) - 2 >= MAXPATHLEN)
	{
		// this is the same error which CreateEvent will return for long name
		SetLastError(ERROR_FILENAME_EXCED_RANGE);
		return false;
	}

	setupMutex(lpMutex);

	char sz[MAXPATHLEN];
	if (lpName)
	{
		sprintf(sz, FAST_MUTEX_EVT_NAME, lpName);
		name = sz;
	}

	lpMutex->hEvent = CreateEvent(lpAttributes, FALSE, FALSE, name);
	DWORD dwLastError = GetLastError();

	if (lpMutex->hEvent)
	{
		if (lpName)
			sprintf(sz, FAST_MUTEX_MAP_NAME, lpName);

		lpMutex->hFileMap = CreateFileMapping(
			INVALID_HANDLE_VALUE, 
			lpAttributes, 
			PAGE_READWRITE, 
			0, 
			sizeof(FAST_MUTEX_SHARED_SECTION), 
			name);

		dwLastError = GetLastError();

		if (lpMutex->hFileMap)
		{
			lpMutex->lpSharedInfo = (FAST_MUTEX_SHARED_SECTION*) 
				MapViewOfFile(lpMutex->hFileMap, FILE_MAP_WRITE, 0, 0, 0);

			if (lpMutex->lpSharedInfo)
			{
				if (dwLastError != ERROR_ALREADY_EXISTS)
				{
					lpMutex->lpSharedInfo->lSpinLock = 0;
					lpMutex->lpSharedInfo->lThreadsWaiting = 0;
					lpMutex->lpSharedInfo->lAvailable = bInitialState ? 0 : 1;
					InterlockedExchange(FIX_TYPE(&lpMutex->lpSharedInfo->fInitialized), 1);
				}
				else
				{
					while (!lpMutex->lpSharedInfo->fInitialized) 
						switchToThread();
				}

				SetLastError(dwLastError);
				return true;
			}
			CloseHandle(lpMutex->hFileMap);
		}
		CloseHandle(lpMutex->hEvent);
	}

	SetLastError(dwLastError);
	return false;
}

static bool openFastMutex(FAST_MUTEX* lpMutex, DWORD DesiredAccess, LPCSTR lpName)
{
	LPCSTR name = lpName;

	if (strlen(lpName) + strlen(FAST_MUTEX_EVT_NAME) - 2 >= MAXPATHLEN)
	{
		SetLastError(ERROR_FILENAME_EXCED_RANGE);
		return false;
	}

	setupMutex(lpMutex);

	char sz[MAXPATHLEN];
	if (lpName)
	{
		sprintf(sz, FAST_MUTEX_EVT_NAME, lpName);
		name = sz;
	}
	
	lpMutex->hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
	
	DWORD dwLastError = GetLastError();
	
	if (lpMutex->hEvent)
	{
		if (lpName)
			sprintf(sz, FAST_MUTEX_MAP_NAME, lpName);
		
		lpMutex->hFileMap = OpenFileMapping(
			FILE_MAP_ALL_ACCESS, 
			FALSE, 
			name);
		
		dwLastError = GetLastError();
	
		if (lpMutex->hFileMap)
		{
			lpMutex->lpSharedInfo = (FAST_MUTEX_SHARED_SECTION*) 
				MapViewOfFile(lpMutex->hFileMap, FILE_MAP_WRITE, 0, 0, 0);
			
			if (lpMutex->lpSharedInfo)
				return true;

			CloseHandle(lpMutex->hFileMap);
		}
		CloseHandle(lpMutex->hEvent);
	}

	SetLastError(dwLastError);
	return false;
}

static inline void setFastMutexSpinCount(FAST_MUTEX* lpMutex, ULONG SpinCount)
{
	lpMutex->lSpinCount = SpinCount;
}


int ISC_mutex_init(MTX mutex, const TEXT* mutex_name)
{
/**************************************
 *
 *	I S C _ m u t e x _ i n i t	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Initialize a mutex.
 *
 **************************************/
	char name_buffer[MAXPATHLEN];

	make_object_name(name_buffer, sizeof(name_buffer), mutex_name, "_mutex");

	return !initializeFastMutex(&mutex->mtx_fast, 
		ISC_get_security_desc(), FALSE, name_buffer);
}


void ISC_mutex_fini(struct mtx *mutex)
{
	if (mutex->mtx_fast.lpSharedInfo)
		deleteFastMutex(&mutex->mtx_fast);
}


int ISC_mutex_lock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Sieze a mutex.
 *
 **************************************/

	const DWORD status = (mutex->mtx_fast.lpSharedInfo) ?
		enterFastMutex(&mutex->mtx_fast, INFINITE) :
		WaitForSingleObject(mutex->mtx_fast.hEvent, INFINITE);

    return (status == WAIT_OBJECT_0 || status == WAIT_ABANDONED) ? 0 : 1;
}


int ISC_mutex_lock_cond(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ l o c k _ c o n d	( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Conditionally sieze a mutex.
 *
 **************************************/

	const DWORD status = (mutex->mtx_fast.lpSharedInfo) ? 
		enterFastMutex(&mutex->mtx_fast, 0) :
		WaitForSingleObject (mutex->mtx_fast.hEvent, 0L);

    return (status == WAIT_OBJECT_0 || status == WAIT_ABANDONED) ? 0 : 1;
}


int ISC_mutex_unlock(MTX mutex)
{
/**************************************
 *
 *	I S C _ m u t e x _ u n l o c k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Release a mutex.
 *
 **************************************/

	if (mutex->mtx_fast.lpSharedInfo) {
		return !leaveFastMutex(&mutex->mtx_fast);
	}

	return !ReleaseMutex(mutex->mtx_fast.hEvent);
}


void ISC_mutex_set_spin_count (struct mtx *mutex, ULONG spins)
{
	if (mutex->mtx_fast.lpSharedInfo) 
		setFastMutexSpinCount(&mutex->mtx_fast, spins);
}

#endif // WIN_NT


#ifdef UNIX
#ifdef HAVE_MMAP
#define ISC_REMAP_FILE_DEFINED
UCHAR *ISC_remap_file(ISC_STATUS * status_vector,
					  SH_MEM shmem_data, SLONG new_length, bool flag)
{
/**************************************
 *
 *	I S C _ r e m a p _ f i l e		( U N I X - m m a p )
 *
 **************************************
 *
 * Functional description
 *	Try to re-map a given file.
 *
 **************************************/
	if (flag)
		ftruncate(shmem_data->sh_mem_handle, new_length);

	UCHAR* address =
		(UCHAR *) mmap(0, new_length, PROT_READ | PROT_WRITE, MAP_SHARED,
					   shmem_data->sh_mem_handle, 0);
	if ((U_IPTR) address == (U_IPTR) -1)
		return NULL;

	munmap((char *) shmem_data->sh_mem_address,
		   shmem_data->sh_mem_length_mapped);

	shmem_data->sh_mem_address = address;
	shmem_data->sh_mem_length_mapped = new_length;

	if (!shmem_data->sh_mem_length_mapped)
	{
		error(status_vector, "shmem_data->sh_mem_length_mapped is 0", 0);
		return NULL;
	}

	return address;
}
#endif
#endif


#ifdef WIN_NT
#define ISC_REMAP_FILE_DEFINED
UCHAR* ISC_remap_file(ISC_STATUS * status_vector,
					SH_MEM shmem_data,
					SLONG new_length,
					bool flag)
{
/**************************************
 *
 *	I S C _ r e m a p _ f i l e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Try to re-map a given file.
 *
 **************************************/

	if (flag)
		if (SetFilePointer
			(shmem_data->sh_mem_handle, new_length, NULL,
			 FILE_BEGIN) == 0xFFFFFFFF
			|| !SetEndOfFile(shmem_data->sh_mem_handle)
			|| !FlushViewOfFile(shmem_data->sh_mem_address, 0))
		{
			error(status_vector, "SetFilePointer", GetLastError());
			return NULL;
		}

/* If the remap file exists, remap does not occur correctly.
 * The file number is local to the process and when it is
 * incremented and a new filename is created, that file may
 * already exist.  In that case, the file is not expanded.
 * This will happen when the file is expanded more than once
 * by concurrently running processes.
 *
 * The problem will be fixed by making sure that a new file name
 * is generated with the mapped file is created.
 */

	HANDLE file_obj;

	while (true) {
		TEXT expanded_filename[MAXPATHLEN];
		sprintf(expanded_filename, "%s%"SLONGFORMAT, shmem_data->sh_mem_name,
				shmem_data->sh_mem_hdr_address[1] + 1);

		file_obj = CreateFileMapping(shmem_data->sh_mem_handle,
					     ISC_get_security_desc(),
					     PAGE_READWRITE,
					     0,
					     new_length,
					     expanded_filename);

		if (!((GetLastError() == ERROR_ALREADY_EXISTS) && flag))
			break;

		CloseHandle(file_obj);
		shmem_data->sh_mem_hdr_address[1]++;
	}

	if (file_obj == NULL) {
		error(status_vector, "CreateFileMapping", GetLastError());
		return NULL;
	}

	LPVOID address = MapViewOfFile(file_obj, FILE_MAP_WRITE, 0, 0, 0);

	if (address == NULL) {
		error(status_vector, "MapViewOfFile", GetLastError());
		CloseHandle(file_obj);
		return NULL;
	}

	if (flag) {
		shmem_data->sh_mem_hdr_address[0] = new_length;
		shmem_data->sh_mem_hdr_address[1]++;
	}

	UnmapViewOfFile(shmem_data->sh_mem_address);
	CloseHandle(shmem_data->sh_mem_object);

	shmem_data->sh_mem_address = static_cast<UCHAR*>(address);
	shmem_data->sh_mem_length_mapped = new_length;
	shmem_data->sh_mem_object = file_obj;

	if (!shmem_data->sh_mem_length_mapped)
	{
		error(status_vector, "shmem_data->sh_mem_length_mapped is 0", 0);
		return NULL;
	}

	return static_cast<UCHAR*>(address);
}
#endif


#ifndef ISC_REMAP_FILE_DEFINED
UCHAR* ISC_remap_file(ISC_STATUS * status_vector,
						SH_MEM shmem_data,
						SLONG new_length,
						bool flag)
{
/**************************************
 *
 *	I S C _ r e m a p _ f i l e		( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Try to re-map a given file.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_unavailable;
	*status_vector++ = isc_arg_end;

	return NULL;
}
#endif


#if (defined UNIX)
void ISC_reset_timer(
					 FPTR_VOID_PTR timeout_handler,
					 void *timeout_arg,
					 SLONG * client_timer, void **client_handler)
{
/**************************************
 *
 *	I S C _ r e s e t _ t i m e r
 *
 **************************************
 *
 * Functional description
 *	Clear a previously established timer and restore
 *	the previous context.
 *
 **************************************/
	struct itimerval internal_timer;

	ISC_signal_cancel(SIGALRM, timeout_handler, timeout_arg);

/* Cancel the timer, then restore the previous handler and alarm */

	timerclear(&internal_timer.it_interval);
	timerclear(&internal_timer.it_value);
	setitimer(ITIMER_REAL, &internal_timer, NULL);
	sigaction(SIGALRM, (struct sigaction*)client_handler, NULL);
	setitimer(ITIMER_REAL, (itimerval*)client_timer, NULL);
}
#endif


#if (defined UNIX)
void ISC_set_timer(
				   SLONG micro_seconds,
				   FPTR_VOID_PTR timeout_handler,
				   void *timeout_arg,
				   SLONG * client_timer, void **client_handler)
{
/**************************************
 *
 *	I S C _ s e t _ t i m e r
 *
 **************************************
 *
 * Functional description
 *	Set a timer for the specified amount of time.
 *
 **************************************/
	struct itimerval internal_timer;
	struct sigaction internal_handler;

/* Start by cancelling any existing timer */

	timerclear(&internal_timer.it_interval);
	timerclear(&internal_timer.it_value);
	setitimer(ITIMER_REAL, &internal_timer,
			  (struct itimerval *) client_timer);

/* Now clear the signal handler while saving the existing one */

	internal_handler.sa_handler = SIG_DFL;
	sigemptyset(&internal_handler.sa_mask);
	internal_handler.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &internal_handler,
			  (struct sigaction *) client_handler);

	if (!micro_seconds)
		return;

/* Next set the new alarm handler and finally set the new timer */

	ISC_signal(SIGALRM, timeout_handler, timeout_arg);

	if (micro_seconds < 1000000)
		internal_timer.it_value.tv_usec = micro_seconds;
	else {
		internal_timer.it_value.tv_sec = micro_seconds / 1000000;
		internal_timer.it_value.tv_usec = micro_seconds % 1000000;
	}
	setitimer(ITIMER_REAL, &internal_timer, NULL);
}
#endif


#ifdef UNIX
void ISC_sync_signals_set(void* arg)
{
/**************************************
 *
 *	I S C _ s y n c _ s i g n a l s _ s e t ( U N I X )
 *
 **************************************
 *
 * Functional description
 *	Set all the synchronous signals for a particular thread
 *
 **************************************/
	sigjmp_buf* const sigenv = static_cast<sigjmp_buf*>(arg);
	TLS_SET(sigjmp_ptr, sigenv);

	sigset(SIGILL, longjmp_sig_handler);
	sigset(SIGFPE, longjmp_sig_handler);
	sigset(SIGBUS, longjmp_sig_handler);
	sigset(SIGSEGV, longjmp_sig_handler);
}


void ISC_sync_signals_reset()
{
/**************************************
 *
 *	I S C _ s y n c _ s i g n a l s _ r e s e t ( U N I X )
 *
 **************************************
 *
 * Functional description
 *	Reset all the synchronous signals for a particular thread
 * to default.
 *
 **************************************/

	sigset(SIGILL, SIG_DFL);
	sigset(SIGFPE, SIG_DFL);
	sigset(SIGBUS, SIG_DFL);
	sigset(SIGSEGV, SIG_DFL);
}
#endif // UNIX

#ifdef UNIX
#ifdef HAVE_MMAP
void ISC_unmap_file(ISC_STATUS* status_vector, SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ u n m a p _ f i l e		( U N I X - m m a p )
 *
 **************************************
 *
 * Functional description
 *	Unmap a given file.
 *
 **************************************/
	union semun arg;

	munmap((char *) shmem_data->sh_mem_address,
		   shmem_data->sh_mem_length_mapped);

	close(shmem_data->sh_mem_handle);
}
#endif
#endif


#ifdef UNIX
#ifndef HAVE_MMAP
void ISC_unmap_file(ISC_STATUS* status_vector, SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ u n m a p _ f i l e		( U N I X - s h m a t )
 *
 **************************************
 *
 * Functional description
 *	Detach from the shared memory
 *
 **************************************/
	struct shmid_ds buf;
	union semun arg;

	shmdt(shmem_data->sh_mem_address);
}
#endif
#endif


#ifdef WIN_NT
void ISC_unmap_file(ISC_STATUS* status_vector,
					SH_MEM shmem_data)
{
/**************************************
 *
 *	I S C _ u n m a p _ f i l e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Detach from the shared memory.
 *
 **************************************/

	CloseHandle(shmem_data->sh_mem_interest);
	UnmapViewOfFile(shmem_data->sh_mem_address);
	CloseHandle(shmem_data->sh_mem_object);
	    
	CloseHandle(shmem_data->sh_mem_handle);
	UnmapViewOfFile(shmem_data->sh_mem_hdr_address);
	CloseHandle(shmem_data->sh_mem_hdr_object);
}
#endif


static void error(ISC_STATUS* status_vector, const TEXT* string, ISC_STATUS status)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	We've encountered an error, report it.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_sys_request;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS)(U_IPTR) string;
	*status_vector++ = SYS_ARG;
	*status_vector++ = status;
	*status_vector++ = isc_arg_end;
}


#ifdef UNIX
static SLONG find_key(ISC_STATUS * status_vector, TEXT * filename)
{
/**************************************
 *
 *	f i n d _ k e y
 *
 **************************************
 *
 * Functional description
 *	Find the semaphore/shared memory key for a file.
 *
 **************************************/

	// Produce shared memory key for file

	key_t key = ftok(filename, FTOK_KEY);
	if (key == -1) {
		int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, PRIV);
		if (fd == -1) {
			error(status_vector, "open", errno);
			return 0L;
		}
		close(fd);
		if ((key = ftok(filename, FTOK_KEY)) == -1) {
			error(status_vector, "ftok", errno);
			return 0L;
		}
	}

	return key;
}
#endif


#ifdef UNIX
void longjmp_sig_handler(int sig_num)
{
/**************************************
 *
 *	l o n g j m p _ s i g _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	The generic signal handler for all signals in a thread.
 *
 **************************************/

	siglongjmp(*TLS_GET(sigjmp_ptr), sig_num);
}
#endif // UNIX


#ifdef WIN_NT
static void make_object_name(
			     TEXT* buffer,
				 size_t bufsize,
			     const TEXT* object_name,
			     const TEXT* object_type)
{
/**************************************
 *
 *	m a k e _ o b j e c t _ n a m e		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Create an object name from a name and type.
 *	Also replace the file separator with "_".
 *
 **************************************/
	char hostname[64];
	_snprintf(buffer, bufsize, object_name, ISC_get_host(hostname, sizeof(hostname)));
	buffer[bufsize - 1] = 0;
	
	char* p;
	char c;
	for (p = buffer; c = *p; p++)
	{
		if (c == '/' || c == '\\' || c == ':')
			*p = '_';
	}
	strcpy(p, object_type);

	// hvlad: windows file systems use case-insensitive file names
	// while kernel objects such as events use case-sensitive names. 
	// Since we use root directory as part of kernel objects names 
	// we must use lower (or upper) register for object name to avoid
	// misunderstanding between processes
	strlwr(buffer);

#ifndef EMBEDDED
	fb_utils::prefix_kernel_object_name(buffer, bufsize);
#endif
}
#endif // WIN_NT
