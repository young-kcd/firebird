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

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include "../jrd/gdsassert.h"
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/thd.h"
#include "../common/classes/locks.h"

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

//#define LOCAL_SEMAPHORES 4

struct sig {
	struct sig* sig_next;
	int sig_signal;
	union {
		FPTR_VOID_PTR user;
		void (*client1) (int);
		void (*client3) (int, siginfo_t *, void *);
		FPTR_INT_VOID_PTR informs;
		FPTR_VOID untyped;
	} sig_routine;
	void* sig_arg;
	USHORT sig_flags;
	USHORT sig_w_siginfo;
};

typedef sig *SIG;

// flags
const USHORT SIG_user		= 0;		/* Our routine */
const USHORT SIG_client		= 1;		/* Not our routine */
const USHORT SIG_informs	= 2;		/* routine tells us whether to chain */

const SLONG SIG_informs_continue	= 0;	/* continue on signal processing */
const SLONG SIG_informs_stop		= 1;	/* stop signal processing */


static bool initialized_signals = false;
static SIG volatile signals = NULL;
static SLONG volatile overflow_count = 0;

static Firebird::Mutex sig_mutex;

static int process_id = 0;


const char* GDS_RELAY	= "/bin/gds_relay";

static int volatile relay_pipe = 0;


static void cleanup(void* arg);
static bool isc_signal2(int signal, FPTR_VOID handler, void* arg, ULONG);
static SLONG overflow_handler(void* arg);
static SIG que_signal(int signal, FPTR_VOID handler, void* arg, int flags, bool w_siginfo);

static void CLIB_ROUTINE signal_action(int number, siginfo_t *siginfo, void *context);

#ifndef SIG_HOLD
#define SIG_HOLD	SIG_DFL
#endif

// Not thread-safe 

ULONG isc_enter_count = 0;

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

namespace {
	volatile int inhibitCounter = 0;
	Firebird::Mutex inhibitMutex;
	volatile bool inSignalHandler = false;
	volatile FB_UINT64 pendingSignals = 0;
}

SignalInhibit::SignalInhibit() throw()
	: locked(!inSignalHandler)	// When called from signal handler, no need
								// to care - signals are already inhibited.
{
	if (!locked)
		return;

	Firebird::MutexLockGuard lock(inhibitMutex);

	++inhibitCounter;
}

void SignalInhibit::enable() throw()
{
	if (!locked)
		return;

	locked = false;

	Firebird::MutexLockGuard lock(inhibitMutex);

	fb_assert(inhibitCounter > 0);
	if (--inhibitCounter == 0) {
		while (pendingSignals)
		{
			for (int n = 0; pendingSignals && n < 64; n++)
			{
				if (pendingSignals & (QUADCONST(1) << n)) 
				{
					pendingSignals &= ~(QUADCONST(1) << n);
					ISC_kill(process_id, n + 1);
				}
			}
		}
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
	if (! pid)
		pid = getpid();

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
		if (access(process, X_OK) != 0) {
			// we don't have relay, therefore simply give meaningful diagnostic
			gds__log("ISC_kill: process %d couldn't deliver signal %d "
				"to process %d: permission denied", getpid(), signal_number, pid);
			return -1;
		}

		if (pipe(pipes)) {
			gds__log("ISC_kill: error %d creating pipe to gds_relay", errno);
			return -1;
		}
		sprintf(arg, "%d", pipes[0]);
		if (!vfork()) {
			execl(process, process, arg, NULL);
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


bool ISC_signal(int signal_number, FPTR_VOID_PTR handler, void* arg)
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
	return isc_signal2(signal_number, reinterpret_cast<FPTR_VOID>(handler), arg, SIG_user);
}


static bool isc_signal2(
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

	sig_mutex.enter();

/* See if this signal has ever been cared about before */

	for (sig = signals; sig; sig = sig->sig_next)
		if (sig->sig_signal == signal_number)
			break;

/* If it hasn't been attach our chain handler to the signal,
   and queue up whatever used to handle it as a non-ISC
   routine (they are invoked differently).  Note that if
   the old action was SIG_DFL, SIG_HOLD, SIG_IGN or our
   multiplexor, there is no need to save it. */

	bool old_sig_w_siginfo = false;
	bool rc = false;
	if (!sig) {
		struct sigaction act, oact;

		act.sa_sigaction = signal_action;
		act.sa_flags = SA_RESTART | SA_SIGINFO;
		sigemptyset(&act.sa_mask);
		sigaddset(&act.sa_mask, signal_number);
		sigaction(signal_number, &act, &oact);
		old_sig_w_siginfo = oact.sa_flags & SA_SIGINFO;
		
		if (oact.sa_sigaction != signal_action &&
			oact.sa_handler != SIG_DFL &&
			oact.sa_handler != SIG_HOLD &&
			oact.sa_handler != SIG_IGN)
		{
			que_signal(signal_number, old_sig_w_siginfo ?
				reinterpret_cast<FPTR_VOID>(oact.sa_sigaction) :
				reinterpret_cast<FPTR_VOID>(oact.sa_handler),
				NULL, SIG_client, old_sig_w_siginfo);
			rc = true;
		}
	}

	/* Que up the new ISC signal handler routine */

	que_signal(signal_number, handler, arg, flags, old_sig_w_siginfo);

	sig_mutex.leave();
	
	return rc;
}


void ISC_signal_cancel(
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

	sig_mutex.enter();

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

	sig_mutex.leave();

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

	pendingSignals = 0;

	inhibitCounter = 0;

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
	fprintf(stderr, "overflow_handler (%x)\n", arg);
#endif

/* If we're within ISC world (inside why-value) when the FPE occurs
 * we handle it (basically by ignoring it).  If it occurs outside of
 * ISC world, return back a code that tells signal_action to call any
 * customer provided handler.
 */
	if (isc_enter_count) {
		++overflow_count;
#ifdef DEBUG_FPE_HANDLING
		fprintf(stderr, "SIGFPE in isc code ignored %d\n",
				   overflow_count);
#endif
		/* We've "handled" the FPE - let signal_action know not to chain
		   the signal to other handlers */
		return SIG_informs_stop;
	}
	else {
		/* We've NOT "handled" the FPE - let signal_action know to chain
		   the signal to other handlers */
		return SIG_informs_continue;
	}
}

static SIG que_signal(int signal_number,
					  FPTR_VOID handler, 
					  void* arg, 
					  int flags,
					  bool sig_w_siginfo)
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
	sig->sig_w_siginfo = sig_w_siginfo;

	sig->sig_next = signals;
	signals = sig;

	return sig;
}


static void CLIB_ROUTINE signal_action(int number, siginfo_t *siginfo, void *context)
{
/**************************************
 *
 *	s i g n a l _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Checkin with various signal handlers.
 *
 **************************************/

	if (inhibitCounter > 0 && number != SIGALRM)
	{
		pendingSignals |= QUADCONST(1) << (number - 1);
		return;
	}

#ifndef SUPERSERVER
	// Save signal delivery status.
	const bool restoreState = inSignalHandler;
	inSignalHandler = true;
	sigset_t set, localSavedSigmask;
	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, &localSavedSigmask);
#endif

	// Invoke everybody who may have expressed an interest.
	for (SIG sig = signals; sig; sig = sig->sig_next)
	{
		if (sig->sig_signal == number)
		{
			if (sig->sig_flags & SIG_client)
			{
				if (sig->sig_w_siginfo)
				{
					(*sig->sig_routine.client3)(number, siginfo, context);
				}
				else
				{
					(*sig->sig_routine.client1)(number);
				}
			}
			else if (sig->sig_flags & SIG_informs) 
			{
				/* Routine will tell us whether to chain the signal to other handlers */
				if ((*sig->sig_routine.informs)(sig->sig_arg) == SIG_informs_stop)
				{
					break;
				}
			}
			else
			{
				(*sig->sig_routine.user) (sig->sig_arg);
			}
		}
	}

#ifndef SUPERSERVER
	sigprocmask(SIG_SETMASK, &localSavedSigmask, NULL);
	inSignalHandler = restoreState;
#endif
}

