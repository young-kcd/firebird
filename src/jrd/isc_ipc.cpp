/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_ipc.c
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
 * Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - EPSON, DELTA, IMP, NCR3000 and M88K
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

 /* $Id: isc_ipc.cpp,v 1.37 2003-04-10 06:49:12 aafemt Exp $ */

#ifdef SHLIB_DEFS
#define LOCAL_SHLIB_DEFS
#endif

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include "../jrd/common.h"
#include "gen/codes.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/thd_proto.h"

#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#ifdef sparc
#ifdef SOLARIS
#define HANDLER_ADDR_ARG
#endif
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef SOLX86
#define HANDLER_ADDR_ARG
#endif

typedef struct sig {
	struct sig *sig_next;
	int sig_signal;
	SIG_FPTR sig_routine;
	void *sig_arg;
	SLONG sig_count;
	IPTR sig_thread_id;
	USHORT sig_flags;
} *SIG;

#define SIG_client	1			/* Not our routine */
#define SIG_informs	2			/* routine tells us whether to chain */

#define SIG_informs_continue	0	/* continue on signal processing */
#define SIG_informs_stop	1	/* stop signal processing */


#if (defined AIX || defined AIX_PPC)
#define GT_32_SIGNALS
#endif

#ifdef SCO_EV
#define HANDLER_ADDR_ARG
#endif

#ifndef REQUESTER
static USHORT initialized_signals = FALSE;
static SIG volatile signals = NULL;
static USHORT volatile inhibit_count = 0;
static SLONG volatile overflow_count = 0;

#ifdef MULTI_THREAD
static MUTX_T sig_mutex;
#endif

#ifdef GT_32_SIGNALS
static SLONG volatile pending_signals[2];
#else
static SLONG volatile pending_signals = 0;
#endif
static int process_id = 0;

#endif /* of ifndef REQUESTER */


/* VMS Specific Stuff */

#ifdef VMS
#include <signal.h>
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

#ifndef O_RDWR
#include <fcntl.h>
#endif

#define LOCAL_SEMAPHORES 4

#ifdef SYSV_SIGNALS
#define SIGVEC		FPTR_INT
#endif

#ifdef HAVE_SIGACTION
#define SIGVEC		struct sigaction
#endif

#ifndef GDS_RELAY
#define GDS_RELAY	"/bin/gds_relay"
#endif

#ifndef SHMEM_DELTA
#define SHMEM_DELTA	(1 << 22)
#endif

static int volatile relay_pipe = 0;
#endif


/* Windows NT */

#ifdef WIN_NT

#include <process.h>
#include <signal.h>
#include <windows.h>

#ifdef TEXT
#undef TEXT
#endif

#define TEXT		SCHAR

#ifndef NSIG
#define NSIG		100
#endif

#define SIGVEC		SIG_FPTR

#define MAX_OPN_EVENTS	40

typedef struct opn_event {
	SLONG opn_event_pid;
	SLONG opn_event_signal;		/* pseudo-signal number */
	HANDLE opn_event_lhandle;	/* local handle to foreign event */
	ULONG opn_event_age;
} *OPN_EVENT;

static struct opn_event opn_events[MAX_OPN_EVENTS];
static USHORT opn_event_count;
static ULONG opn_event_clock;
#endif

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
extern "C" {
#endif

static void cleanup(void *);
#ifdef NOT_USED_OR_REPLACED
#ifndef REQUESTER
static void error(ISC_STATUS *, TEXT *, ISC_STATUS);
#endif
#endif
static void isc_signal2(int, SIG_FPTR, void *, ULONG);
static SLONG overflow_handler(void *);
static SIG que_signal(int, SIG_FPTR, void *, int);

#if !(defined HANDLER_ADDR_ARG)
#ifdef SINIXZ
static void CLIB_ROUTINE signal_handler(int, int);
#else
static void CLIB_ROUTINE signal_handler(int, int, struct sigcontext *);
#endif
#endif

#ifdef HANDLER_ADDR_ARG
static void CLIB_ROUTINE signal_handler(int, int, void *, void *);
#endif

#ifdef OLD_POSIX_THREADS
static void sigwait_thread(int);
#endif

#ifndef sigvector
#ifndef hpux
#define sigvector	sigvec
#endif
#endif

#ifndef SIGVEC
#define SIGVEC		struct sigvec
#endif

#ifndef SIG_HOLD
#define SIG_HOLD	SIG_DFL
#endif

// Not thread-safe 

extern "C" {
	ULONG isc_enter_count = 0;
}

//static SIGVEC client_sigfpe;

#ifdef SHLIB_DEFS
#define sprintf		(*_libgds_sprintf)
#define strlen		(*_libgds_strlen)
#define strcpy		(*_libgds_strcpy)
#define exit		(*_libgds_exit)
#define _iob		(*_libgds__iob)
#define getpid		(*_libgds_getpid)
#define errno		(*_libgds_errno)
#define kill		(*_libgds_kill)
#define _exit           (*_libgds__exit)
#define pipe		(*_libgds_pipe)
#define fork		(*_libgds_fork)
#define write		(*_libgds_write)
#define _ctype		(*_libgds__ctype)
#define sigvector	(*_libgds_sigvec)
#define execl		(*_libgds_execl)
#define sigset		(*_libgds_sigset)
#define ib_fprintf		(*_libgds_fprintf)
#define close		(*_libgds_close)

extern int sprintf();
extern int strlen();
extern SCHAR *strcpy();
extern void exit();
extern IB_FILE _iob[];
extern pid_t getpid();
extern int errno;
extern int kill();
extern void _exit();
extern int pipe();
extern pid_t fork();
extern int write();
extern SCHAR _ctype[];
extern int sigvector();
extern int execl();
extern void (*sigset()) ();
extern int ib_fprintf();
extern int close();
#endif // SHLIB_DEFS


#if (defined __cplusplus) && (defined SOLX86)
}
#endif

extern "C" {

void DLL_EXPORT ISC_enter(void)
{
/**************************************
 *
 *	I S C _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	Enter ISC world from caller.
 *
 **************************************/
/* Cancel our handler for SIGFPE - in case it was already there */
	ISC_signal_cancel(SIGFPE, (FPTR_VOID) overflow_handler, NULL);

/* Setup overflow handler - with chaining to any user handler */
	isc_signal2(SIGFPE, (SIG_FPTR) overflow_handler, NULL, SIG_informs);

#ifdef DEBUG_FPE_HANDLING
/* Debug code to simulate an FPE occuring during DB Operation */
	if (overflow_count < 100)
		(void) kill(getpid(), SIGFPE);
#endif
}

} // extern "C"


#ifndef REQUESTER
void DLL_EXPORT ISC_enable(void)
{
/**************************************
 *
 *	I S C _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enable signal processing.  Re-post any pending signals.
 *
 **************************************/

#if defined(UNIX)
	USHORT n;
#endif

#ifdef GT_32_SIGNALS
	SLONG p;
	USHORT i;
#endif

	if (inhibit_count)
		--inhibit_count;

	if (inhibit_count)
		return;

#ifdef UNIX
#ifdef GT_32_SIGNALS
	while (pending_signals[0] || pending_signals[1])
		for (i = 0; i < 2; i++) {
			for (n = 0, p = pending_signals[i]; p && n < 32; n++)
				if (p & (1 << n)) {
					p &= ~(1 << n);
					ISC_kill(process_id, n + 1 + i * 32);
				}
			/* This looks like a danger point - if one of the bits
			 * was reset after we sent the signal then we will lose it.
			 */
			pending_signals[i] = 0;
		}
#else
	while (pending_signals)
		for (n = 0; pending_signals && n < 32; n++)
			if (pending_signals & (1 << n)) {
				pending_signals &= ~(1 << n);
				ISC_kill(process_id, n + 1);
			}
#endif
#endif

}
#endif


extern "C" {
void DLL_EXPORT ISC_exit(void)
{
/**************************************
 *
 *	I S C _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Exit ISC world, return to caller.
 *
 **************************************/

/* No longer attempt to handle overflow internally */
	ISC_signal_cancel(SIGFPE, (FPTR_VOID) overflow_handler, 0);
}
} // Extern "C"


#ifndef REQUESTER
void DLL_EXPORT ISC_inhibit(void)
{
/**************************************
 *
 *	I S C _ i n h i b i t
 *
 **************************************
 *
 * Functional description
 *	Inhibit process of signals.  Signals will be
 *	retained until signals are eventually re-enabled,
 *	then re-posted.
 *
 **************************************/

	++inhibit_count;
}
#endif


#if (defined VMS && defined __ALPHA)
int ISC_kill(SLONG pid, SLONG signal_number)
{
/**************************************
 *
 *	I S C _ k i l l		( A p o l l o  &  A l p h a / O p e n V M S )
 *
 **************************************
 *
 * Functional description
 *	Notify somebody else.
 *
 **************************************/

	return kill(pid, signal_number);
}
#endif


#ifdef UNIX
int ISC_kill(SLONG pid, SLONG signal_number)
{
/**************************************
 *
 *	I S C _ k i l l		( U N I X )
 *
 **************************************
 *
 * Functional description
 *	Notify somebody else.
 *
 **************************************/
	SLONG msg[3];
	int status, pipes[2];
	TEXT process[MAXPATHLEN], arg[10];

	for (;;) {
		status = kill(pid, signal_number);

		if (!status)
			return status;
		if (SYSCALL_INTERRUPTED(errno))
			continue;
		if (errno == EPERM)
			break;

		return status;
	}

/* Process is there, but we don't have the privilege to
   send to him.  */

	if (!relay_pipe) {
		gds__prefix(process, GDS_RELAY);
		if (pipe(pipes)) {
			gds__log("ISC_kill: error %d creating gds_relay", errno);
			return -1;
		}
		sprintf(arg, "%d", pipes[0]);
		if (!vfork()) {
			execl(process, process, arg, 0);
			gds__log("ISC_kill: error %d starting gds_relay %s", errno,
					 process);
			_exit(0);
		}
		relay_pipe = pipes[1];

		/* Don't need the READ pipe */
		close(pipes[0]);
	}

	msg[0] = pid;
	msg[1] = signal_number;
	msg[2] = msg[0] ^ msg[1];	/* XOR for a consistancy check */
	if (write(relay_pipe, msg, sizeof(msg)) != sizeof(msg)) {
		gds__log("ISC_kill: write to relay_pipe failed %d", errno);
		relay_pipe = 0;			/* try to restart next time */
		return -1;
	}

	return 0;
}
#endif


#ifdef WIN_NT
int API_ROUTINE ISC_kill(SLONG pid, SLONG signal_number, void *object_hndl)
{
/**************************************
 *
 *	I S C _ k i l l		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Notify somebody else.
 *
 **************************************/
	ULONG oldest_age;
	OPN_EVENT opn_event, end_opn_event, oldest_opn_event;

/* If we're simply trying to poke ourselves, do so directly. */
	if (!process_id)
		process_id = GetCurrentProcessId();

	if (pid == process_id) {
		SetEvent(object_hndl);
		return 0;
	}

	oldest_age = ~0;

	opn_event = opn_events;
	end_opn_event = opn_event + opn_event_count;
	for (; opn_event < end_opn_event; opn_event++) {
		if (opn_event->opn_event_pid == pid &&
			opn_event->opn_event_signal == signal_number) break;
		if (opn_event->opn_event_age < oldest_age) {
			oldest_opn_event = opn_event;
			oldest_age = opn_event->opn_event_age;
		}
	}

	if (opn_event >= end_opn_event) {
		HANDLE lhandle;

		if (!(lhandle = ISC_make_signal(FALSE, FALSE, pid, signal_number)))
			return -1;

		if (opn_event_count < MAX_OPN_EVENTS)
			opn_event_count++;
		else {
			opn_event = oldest_opn_event;
			CloseHandle(opn_event->opn_event_lhandle);
		}

		opn_event->opn_event_pid = pid;
		opn_event->opn_event_signal = signal_number;
		opn_event->opn_event_lhandle = lhandle;
	}

	opn_event->opn_event_age = ++opn_event_clock;

	return (SetEvent(opn_event->opn_event_lhandle)) ? 0 : -1;
}
#endif

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
extern "C" {
#endif

void API_ROUTINE ISC_signal(int signal_number, FPTR_VOID handler, void *arg)
{
/**************************************
 *
 *	I S C _ s i g n a l
 *
 **************************************
 *
 * Functional description
 *	Multiplex multiple handers into single signal.
 *
 **************************************/
	isc_signal2(signal_number, (SIG_FPTR)handler, arg, 0);
}


#ifdef SYSV_SIGNALS
static void isc_signal2(
						int signal_number,
						SIG_FPTR handler, void *arg, ULONG flags)
{
/**************************************
 *
 *	i s c _ s i g n a l 2		( S Y S V _ S I G N A L S )
 *
 **************************************
 *
 * Functional description
 *	Multiplex multiple handers into single signal.
 *
 **************************************/
	SIG sig;
	int n;
	FPTR_INT ptr;

/* The signal handler needs the process id */

	if (!process_id)
		process_id = getpid();

	THD_MUTEX_LOCK(&sig_mutex);

/* See if this signal has ever been cared about before */

	for (sig = signals; sig; sig = sig->sig_next)
		if (sig->sig_signal == signal_number)
			break;

/* If it hasn't been attach our chain handler to the signal,
   and queue up whatever used to handle it as a non-ISC
   routine (they are invoked differently).  Note that if
   the old action was SIG_DFL, SIG_HOLD, SIG_IGN or our
   multiplexor, there is no need to save it. */

	if (!sig) {
		ptr = sigset(signal_number, signal_handler);
		if (ptr != SIG_DFL &&
			ptr != SIG_IGN &&
			ptr != SIG_HOLD &&
			ptr != signal_handler)
		que_signal(signal_number, ptr, arg, SIG_client);
	}

/* Que up the new ISC signal handler routine */

	que_signal(signal_number, handler, arg, flags);

	THD_MUTEX_UNLOCK(&sig_mutex);
}
#endif /* SYSV */


#if (defined UNIX || defined WIN_NT)
#ifndef SYSV_SIGNALS
static void isc_signal2(
						int signal_number,
						SIG_FPTR handler, void *arg, ULONG flags)
{
/**************************************
 *
 *	i s c _ s i g n a l 2		( u n i x ,   W I N _ N T ,   O S 2 )
 *
 **************************************
 *
 * Functional description
 *	Multiplex multiple handers into single signal.
 *
 **************************************/

	SIG sig;
	SIG_FPTR ptr;

/* The signal handler needs the process id */
	if (!process_id)
		process_id = getpid();

#if defined(WIN_NT)
/* If not a UNIX signal, just queue for port watcher. */

	if (signal_number > NSIG) {
		que_signal(signal_number, handler, arg, flags);
		return;
	}
#endif

	THD_MUTEX_LOCK(&sig_mutex);

/* See if this signal has ever been cared about before */

	for (sig = signals; sig; sig = sig->sig_next)
		if (sig->sig_signal == signal_number)
			break;

/* If it hasn't been attach our chain handler to the signal,
   and queue up whatever used to handle it as a non-ISC
   routine (they are invoked differently).  Note that if
   the old action was SIG_DFL, SIG_HOLD, SIG_IGN or our
   multiplexor, there is no need to save it. */

	if (!sig) {
#if (defined WIN_NT)
		ptr = (SIG_FPTR) signal(signal_number, (SIG_FPTR) signal_handler);
#else
		SIGVEC vec, old_vec;

#ifndef HAVE_SIGACTION
#ifdef OLD_POSIX_THREADS
		if (signal_number != SIGALRM)
			vec.sv_handler = SIG_DFL;
		else
#endif
			vec.sv_handler = (SIG_FPTR) signal_handler;
		vec.sv_mask = 0;
		vec.sv_onstack = 0;
		sigvector(signal_number, &vec, &old_vec);
		ptr = old_vec.sv_handler;
#else
#ifdef OLD_POSIX_THREADS
		if (signal_number != SIGALRM)
			vec.sv_handler = SIG_DFL;
		else
#endif
			vec.sa_handler = (SIG_FPTR) signal_handler;
		memset(&vec.sa_mask, 0, sizeof(vec.sa_mask));
		vec.sa_flags = SA_RESTART;
		sigaction(signal_number, &vec, &old_vec);
		ptr = (SIG_FPTR) old_vec.sa_handler;
#endif
#endif
		if (ptr != (SIG_FPTR) SIG_DFL &&
			ptr != (SIG_FPTR) SIG_HOLD &&
			ptr != (SIG_FPTR) SIG_IGN && ptr != (SIG_FPTR) signal_handler) {
			que_signal(signal_number, (SIG_FPTR) ptr, arg, SIG_client);
		}
	}

	/* Que up the new ISC signal handler routine */

	que_signal(signal_number, handler, arg, flags);

	THD_MUTEX_UNLOCK(&sig_mutex);
}
#endif
#endif


#ifndef REQUESTER
void API_ROUTINE ISC_signal_cancel(
								   int signal_number,
								   FPTR_VOID handler, void *arg)
{
/**************************************
 *
 *	I S C _ s i g n a l _ c a n c e l
 *
 **************************************
 *
 * Functional description
 *	Cancel a signal handler.
 *	If handler == NULL, cancel all handlers for a given signal.
 *
 **************************************/
	SIG sig;
	volatile SIG *ptr;

	THD_MUTEX_LOCK(&sig_mutex);

	for (ptr = &signals; sig = *ptr;) {
		if (sig->sig_signal == signal_number &&
			(handler == NULL ||
			 ((FPTR_VOID)sig->sig_routine == handler && sig->sig_arg == arg))) {
			*ptr = sig->sig_next;
			gds__free(sig);
		}
		else
			ptr = &(*ptr)->sig_next;
	}

	THD_MUTEX_UNLOCK(&sig_mutex);

#ifdef OLD_POSIX_THREADS
	{
		IPTR thread_id;

		/* UNSAFE CODE HERE - sig has been freed - rewrite should
		 * this section ever be activated.
		 */
		deliberate_error_here_to_force_compile_error++;
		if (!sig || signal_number == SIGALRM)
			return;

		thread_id = sig->sig_thread_id;
		for (sig = signals; sig; sig = sig->sig_next)
			if (sig->sig_signal == signal_number)
				return;

		/* No more handlers exist for the signal.  Kill the thread that's
		   been listening for the signal. */

		pthread_cancel((pthread_t *) thread_id);
	}
#endif // OLD_POSIX_THREADS
}
#endif // ifndef REQUESTER


void DLL_EXPORT ISC_signal_init(void)
{
/**************************************
 *
 *	I S C _ s i g n a l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize any system signal handlers.
 *
 **************************************/

#ifndef REQUESTER
	if (initialized_signals)
		return;

	initialized_signals = TRUE;

	overflow_count = 0;
	gds__register_cleanup(cleanup, 0);

#ifndef VMS
	process_id = getpid();

	THD_MUTEX_INIT(&sig_mutex);

	isc_signal2(SIGFPE, (SIG_FPTR) overflow_handler, 0, SIG_informs);
#endif

#endif /* REQUESTER */

#ifdef WIN_NT
	ISC_get_security_desc();
#endif
}


#ifndef REQUESTER
static void cleanup(void *arg)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Module level cleanup handler.
 *
 **************************************/
	signals = NULL;

	THD_MUTEX_DESTROY(&sig_mutex);

	inhibit_count = 0;

#ifdef GT_32_SIGNALS
	pending_signals[0] = pending_signals[1] = 0;
#else
	pending_signals = 0;
#endif

	process_id = 0;

#ifdef WIN_NT
	{
		OPN_EVENT opn_event;

		opn_event = opn_events + opn_event_count;
		opn_event_count = 0;
		while (opn_event-- > opn_events)
			CloseHandle(opn_event->opn_event_lhandle);
	}
#endif

	initialized_signals = FALSE;
}
#endif

#ifdef NOT_USED_OR_REPLACED
#ifndef REQUESTER
static void error(ISC_STATUS * status_vector, TEXT * string, ISC_STATUS status)
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

	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds_sys_request;
	*status_vector++ = gds_arg_string;
	*status_vector++ = (ISC_STATUS) string;
	*status_vector++ = SYS_ARG;
	*status_vector++ = status;
	*status_vector++ = gds_arg_end;
}
#endif
#endif

#ifndef REQUESTER
static SLONG overflow_handler(void *arg)
{
/**************************************
 *
 *	o v e r f l o w _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Somebody overflowed.  Ho hum.
 *
 **************************************/

#ifdef DEBUG_FPE_HANDLING
	ib_fprintf(ib_stderr, "overflow_handler (%x)\n", arg);
#endif

/* If we're within ISC world (inside why-value) when the FPE occurs
 * we handle it (basically by ignoring it).  If it occurs outside of
 * ISC world, return back a code that tells signal_handler to call any
 * customer provided handler.
 */
	if (isc_enter_count) {
		++overflow_count;
#ifdef DEBUG_FPE_HANDLING
		ib_fprintf(ib_stderr, "SIGFPE in isc code ignored %d\n",
				   overflow_count);
#endif
		/* We've "handled" the FPE - let signal_handler know not to chain
		   the signal to other handlers */
		return SIG_informs_stop;
	}
	else {
		/* We've NOT "handled" the FPE - let signal_handler know to chain
		   the signal to other handlers */
		return SIG_informs_continue;
	}
}
#endif


#ifndef REQUESTER
static SIG que_signal(
					  int signal_number,
					  SIG_FPTR handler, void *arg, int flags)
{
/**************************************
 *
 *	q u e _ s i g n a l
 *
 **************************************
 *
 * Functional description
 *	Que signal for later action.
 *
 **************************************/
	SIG sig;
	IPTR thread_id = 0;

#ifdef OLD_POSIX_THREADS
	if (signal_number != SIGALRM) {
		for (sig = signals; sig; sig = sig->sig_next)
			if (sig->sig_signal == signal_number)
				break;

		if (!sig)
			pthread_create((pthread_t *) & thread_id, pthread_attr_default,
						   sigwait_thread, (void *) signal_number);
		else
			thread_id = sig->sig_thread_id;
	}
#endif

	sig = (SIG) gds__alloc((SLONG) sizeof(struct sig));
/* FREE: unknown */
	if (!sig) {					/* NOMEM: */
		DEV_REPORT("que_signal: out of memory");
		return NULL;			/* NOMEM: not handled, too difficult */
	}

#ifdef DEBUG_GDS_ALLOC
/* This will only be freed when a signal handler is de-registered
 * and we don't do that at process exit - so this not always
 * a freed structure.
 */
	gds_alloc_flag_unfreed((void *) sig);
#endif

	sig->sig_signal = signal_number;
	sig->sig_routine = handler;
	sig->sig_arg = arg;
	sig->sig_flags = flags;
	sig->sig_thread_id = thread_id;
	sig->sig_count = 0;

	sig->sig_next = signals;
	signals = sig;

	return sig;
}
#endif


#ifndef REQUESTER
static void CLIB_ROUTINE signal_handler(int number,
#ifdef SINIXZ
										int code)
#else
										int code,
#ifdef HANDLER_ADDR_ARG
										void *scp, void *addr)
#else
										struct sigcontext *scp)
#endif
#endif /* SINIXZ */
{
/**************************************
 *
 *	s i g n a l _ h a n d l e r	( G E N E R I C )
 *
 **************************************
 *
 * Functional description
 *	Checkin with various signal handlers.
 *
 **************************************/
	SIG sig;

/* if there are no signals, who cares? */

	if (signals == (SIG) NULL)
		return;

/* This should never happen, but if it does might as well not crash */

	if (number == 0)
		return;

/* If signals are inhibited, save the signal for later reposting.
   Otherwise, invoke everybody who may have expressed an interest. */

#ifdef SIGALRM
	if (inhibit_count && number != SIGALRM)
#else
	if (inhibit_count)
#endif
#ifdef GT_32_SIGNALS
		pending_signals[(number - 1) / 32] |= 1L << ((number - 1) % 32);
#else
		pending_signals |= 1L << (number - 1);
#endif
	else
		for (sig = signals; sig; sig = sig->sig_next)
			if (sig->sig_signal == number)
				if (sig->sig_flags & SIG_client)
#pragma FB_COMPILER_MESSAGE("Fix! Ugly function pointer cast!")
#ifdef HANDLER_ADDR_ARG
					((void (*)(...)) (*sig->sig_routine)) (number, code, scp, addr);
#elif defined(SINIXZ)
					((void (*)(...)) (*sig->sig_routine)) (number, code);
#else
					((void (*)(...)) (*sig->sig_routine)) (number, code, scp);
#endif
				else if (sig->sig_flags & SIG_informs) {
					ULONG res;
					/* Routine will tell us whether to chain the signal to other handlers */
#pragma FB_COMPILER_MESSAGE("Fix! Ugly function pointer cast!")
					res = ((*((SLONG(*)(void *)) sig->sig_routine))
						   (sig->sig_arg));
					if (res == SIG_informs_stop)
						break;
				}
				else
					((FPTR_VOID_PTR) (*sig->sig_routine)) (sig->sig_arg);

#ifdef SIG_RESTART
	scp->sc_syscall_action = (!ISC_check_restart()
							  || number ==
							  SIGALRM) ? SIG_RETURN : SIG_RESTART;
#endif
}
#endif


#ifdef OLD_POSIX_THREADS
static void sigwait_thread(int signal_number)
{
/**************************************
 *
 *	s i g w a i t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	This thread waits for a given signal
 *	and calls the all purpose signal
 *	handler whenever it arrives.
 *
 **************************************/
	sigset_t sigmask;
	SIG sig;

	sigemptyset(&sigmask);
	sigaddset(&sigmask, signal_number);

	while (TRUE) {
		sigwait(&sigmask);

		/* If signals are inhibited, save the signal for later reposting.
		   Otherwise, invoke everybody who may have expressed an interest. */

		if (inhibit_count && signal_number != SIGALRM)
#ifdef GT_32_SIGNALS
			pending_signals[(signal_number - 1) / 32] |=
				1 << ((signal_number - 1) % 32);
#else
			pending_signals |= 1 << (signal_number - 1);
#endif
		else
			for (sig = signals; sig; sig = sig->sig_next)
				if (sig->sig_signal == signal_number)
					if (sig->sig_flags & SIG_client)
#ifdef HANDLER_ADDR_ARG
						(*sig->sig_routine) (number, 0, NULL, NULL);
#else
						(*sig->sig_routine) (number, 0, NULL);
#endif
					else
						(*sig->sig_routine) (sig->sig_arg);
	}
}
#endif

#if (defined __cplusplus) && (defined SOLX86)
/* Who else got mixed c and C++ linkage error - let join me. KLK
*/
}
#endif
