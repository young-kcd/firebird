/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_ipc.cpp
 *	DESCRIPTION:	Handing and posting of signals (POSIX)
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
 * 2003.08.27 Nickolay Samofatov - create POSIX version of this module
 *
 */

 /* $Id: isc_ipc.cpp,v 1.4 2003-10-16 08:51:03 robocop Exp $ */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <assert.h>
#include "../jrd/common.h"
#include "gen/codes.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/thd_proto.h"

#ifdef HAVE_VFORK_H
#include <vfork.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_SYS_SIGNAL_H
#include <sys/signal.h>
#endif

#ifdef HAVE_SYS_SIGINFO_H
#include <sys/siginfo.h>
#endif

#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifndef O_RDWR
#include <fcntl.h>
#endif

#define LOCAL_SEMAPHORES 4

typedef struct sig {
	struct sig* sig_next;
	int sig_signal;
	union {
		FPTR_VOID_PTR user;
		void (*client) (int, siginfo_t*, void*);
		FPTR_INT_VOID_PTR informs;
		FPTR_VOID untyped;
	} sig_routine;
	void* sig_arg;
	USHORT sig_flags;
} *SIG;

#define SIG_user    0           /* Our routine */
#define SIG_client	1			/* Not our routine */
#define SIG_informs	2			/* routine tells us whether to chain */

#define SIG_informs_continue	0	/* continue on signal processing */
#define SIG_informs_stop	1	/* stop signal processing */


static bool initialized_signals = false;
static SIG volatile signals = NULL;
static SLONG volatile overflow_count = 0;

#ifdef MULTI_THREAD
static MUTX_T sig_mutex;
#endif

static int process_id = 0;


#ifndef GDS_RELAY
#define GDS_RELAY	"/bin/gds_relay"
#endif

static int volatile relay_pipe = 0;


static void cleanup(void* arg);
static void isc_signal2(int signal, FPTR_VOID handler, void* arg, ULONG);
static SLONG overflow_handler(void* arg);
static SIG que_signal(int signal, FPTR_VOID handler, void* arg, int flags);

static void CLIB_ROUTINE signal_handler(int number,
										siginfo_t* info,
										void* pointer);

#ifndef SIG_HOLD
#define SIG_HOLD	SIG_DFL
#endif

// Not thread-safe 

extern "C" {
	ULONG isc_enter_count = 0;
}

void ISC_enter(void)
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
	ISC_signal_cancel(SIGFPE, reinterpret_cast<FPTR_VOID_PTR>(overflow_handler), NULL);

/* Setup overflow handler - with chaining to any user handler */
	isc_signal2(SIGFPE, reinterpret_cast<FPTR_VOID>(overflow_handler), NULL, SIG_informs);

#ifdef DEBUG_FPE_HANDLING
/* Debug code to simulate an FPE occuring during DB Operation */
	if (overflow_count < 100)
		(void) kill(getpid(), SIGFPE);
#endif
}

volatile sig_atomic_t inhibit_counter = 0;
sigset_t saved_sigmask;

void ISC_inhibit() throw()
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
	if (inhibit_counter == 0) {
		sigset_t set, oset;
		sigfillset(&set);
		sigprocmask(SIG_BLOCK, &set, &saved_sigmask);
	}
	inhibit_counter++;
}


void ISC_enable() throw()
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
	assert(inhibit_counter > 0);
	inhibit_counter--;
	if (inhibit_counter == 0) {
		// Return to the mask as it were before the first recursive 
		// call to ISC_inhibit
		sigprocmask(SIG_SETMASK, &saved_sigmask, NULL);
	}
}


void ISC_exit(void)
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
	ISC_signal_cancel(SIGFPE, reinterpret_cast<FPTR_VOID_PTR>(overflow_handler), 0);
}


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
	for (;;) {
		const int status = kill(pid, signal_number);

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

	int pipes[2];
	
	if (!relay_pipe) {
		TEXT process[MAXPATHLEN], arg[10];
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

	SLONG msg[3];
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


void API_ROUTINE ISC_signal(int signal_number, FPTR_VOID_PTR handler, void* arg)
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
	isc_signal2(signal_number, reinterpret_cast<FPTR_VOID>(handler), arg, SIG_user);
}


static void isc_signal2(
						int signal_number,
						FPTR_VOID handler, void* arg, ULONG flags)
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
		struct sigaction act, oact;

		act.sa_sigaction = signal_handler;
		act.sa_flags = SA_RESTART | SA_SIGINFO;
		sigemptyset(&act.sa_mask);
		sigaddset(&act.sa_mask, signal_number);
		sigaction(signal_number, &act, &oact);

		if ((oact.sa_flags & SA_SIGINFO &&
			 oact.sa_sigaction != signal_handler) ||
			(!(oact.sa_flags & SA_SIGINFO) &&
			 oact.sa_handler != SIG_DFL &&
			 oact.sa_handler != SIG_HOLD &&
			 oact.sa_handler != SIG_IGN ) 
		   )
		{
			que_signal(signal_number, 
				(oact.sa_flags & SA_SIGINFO) ? 
					reinterpret_cast<FPTR_VOID>(oact.sa_sigaction) : 
					reinterpret_cast<FPTR_VOID>(oact.sa_handler), 
				NULL, SIG_client);
		}
	}

	/* Que up the new ISC signal handler routine */

	que_signal(signal_number, handler, arg, flags);

	THD_MUTEX_UNLOCK(&sig_mutex);
}


void API_ROUTINE ISC_signal_cancel(
								   int signal_number,
								   FPTR_VOID_PTR handler, void* arg)
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
	volatile SIG* ptr;

	THD_MUTEX_LOCK(&sig_mutex);

	for (ptr = &signals; sig = *ptr;) {
		if (sig->sig_signal == signal_number &&
			(handler == NULL ||
			 (sig->sig_routine.user == handler && sig->sig_arg == arg))) 
		{
			*ptr = sig->sig_next;
			gds__free(sig);
		}
		else
			ptr = &(*ptr)->sig_next;
	}

	THD_MUTEX_UNLOCK(&sig_mutex);

}


void ISC_signal_init(void)
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

	if (initialized_signals)
		return;

	initialized_signals = true;

	overflow_count = 0;
	gds__register_cleanup(cleanup, 0);

	process_id = getpid();

	THD_MUTEX_INIT(&sig_mutex);

	isc_signal2(SIGFPE, reinterpret_cast<FPTR_VOID>(overflow_handler), 0, SIG_informs);

}


static void cleanup(void* arg)
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

	process_id = 0;

	initialized_signals = false;
}

static SLONG overflow_handler(void* arg)
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

static SIG que_signal(
					  int signal_number,
					  FPTR_VOID handler, void* arg, int flags)
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
	IPTR thread_id = 0;

	SIG sig = (SIG) gds__alloc((SLONG) sizeof(struct sig));
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
	sig->sig_routine.untyped = handler;
	sig->sig_arg = arg;
	sig->sig_flags = flags;

	sig->sig_next = signals;
	signals = sig;

	return sig;
}


static void CLIB_ROUTINE signal_handler(int number,
										siginfo_t *info,
										void *pointer)
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
	/* Invoke everybody who may have expressed an interest. */

	for (SIG sig = signals; sig; sig = sig->sig_next)
		if (sig->sig_signal == number)
			if (sig->sig_flags & SIG_client)
				(*sig->sig_routine.client)(number, info, pointer);
			else if (sig->sig_flags & SIG_informs) {
				/* Routine will tell us whether to chain the signal to other handlers */
				if ((*sig->sig_routine.informs)(sig->sig_arg) == SIG_informs_stop)
					break;
			}
			else
				(*sig->sig_routine.user) (sig->sig_arg);
}

