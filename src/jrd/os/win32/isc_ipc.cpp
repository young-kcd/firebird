/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_ipc.cpp
 *	DESCRIPTION:	Handing and posting of signals (Windows)
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
 * 2002.08.27 Nickolay Samofatov - create Windows version of this module
 *
 */


#include "firebird.h"
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/thd.h"

#include <windows.h>
#include <process.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

// signals may be not defined in MINGW
#ifndef SIG_SGE
#define SIG_SGE (void (__cdecl *)(int))3           /* signal gets error */
#endif
#ifndef SIG_ACK
#define SIG_ACK (void (__cdecl *)(int))4           /* acknowledge */
#endif

#ifndef REQUESTER
static USHORT initialized_signals = FALSE;
static SLONG volatile overflow_count = 0;

static MUTX_T sig_mutex;

static int process_id = 0;

#endif /* of ifndef REQUESTER */


const USHORT MAX_OPN_EVENTS	= 40;

struct opn_event {
	SLONG opn_event_pid;
	SLONG opn_event_signal;		/* pseudo-signal number */
	HANDLE opn_event_lhandle;	/* local handle to foreign event */
	ULONG opn_event_age;
};

typedef opn_event *OPN_EVENT;

static struct opn_event opn_events[MAX_OPN_EVENTS];
static USHORT opn_event_count;
static ULONG opn_event_clock;

static void (*system_overflow_handler)(int);
static void cleanup(void *);
static void overflow_handler(int, int) throw();

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
/* Setup overflow handler - with chaining to any user handler */
	void (*temp)(int) = signal(SIGFPE,
		reinterpret_cast<void(*)(int)>(overflow_handler));
	if (temp != reinterpret_cast<void(*)(int)>(overflow_handler))
		system_overflow_handler = temp;

#ifdef DEBUG_FPE_HANDLING
/* Debug code to simulate an FPE occuring during DB Operation */
	if (overflow_count < 100)
		(void) kill(getpid(), SIGFPE);
#endif
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
	signal(SIGFPE, system_overflow_handler);
}


int ISC_kill(SLONG pid, SLONG signal_number, void *object_hndl)
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

/* If we're simply trying to poke ourselves, do so directly. */
	if (!process_id)
		process_id = GetCurrentProcessId();

	if (pid == process_id) {
		SetEvent(object_hndl);
		return 0;
	}

	OPN_EVENT oldest_opn_event;
	ULONG oldest_age = ~0;

	OPN_EVENT opn_event = opn_events;
	OPN_EVENT end_opn_event = opn_event + opn_event_count;
	for (; opn_event < end_opn_event; opn_event++) {
		if (opn_event->opn_event_pid == pid &&
			opn_event->opn_event_signal == signal_number)
		{
			break;
		}
		if (opn_event->opn_event_age < oldest_age) {
			oldest_opn_event = opn_event;
			oldest_age = opn_event->opn_event_age;
		}
	}

	if (opn_event >= end_opn_event) {
		HANDLE lhandle = ISC_make_signal(false, false, pid, signal_number);
		if (!lhandle)
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

#ifndef REQUESTER
	if (initialized_signals)
		return;

	initialized_signals = TRUE;

	overflow_count = 0;
	gds__register_cleanup(cleanup, 0);

	process_id = getpid();

	system_overflow_handler =
		signal(SIGFPE, reinterpret_cast<void(*)(int)>(overflow_handler));

#endif /* REQUESTER */

	ISC_get_security_desc();
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
	process_id = 0;

	OPN_EVENT opn_event = opn_events + opn_event_count;
	opn_event_count = 0;
	while (opn_event-- > opn_events)
		CloseHandle(opn_event->opn_event_lhandle);

	initialized_signals = FALSE;
}
#endif

#ifndef REQUESTER
static void overflow_handler(int signal, int code) throw()
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
 * ISC world, return back a code that tells signal_handler to call any
 * customer provided handler.
 */
	if (isc_enter_count) {
		++overflow_count;
#ifdef DEBUG_FPE_HANDLING
		fprintf(stderr, "SIGFPE in isc code ignored %d\n",
				   overflow_count);
#endif
		/* We've "handled" the FPE */
	}
	else {
		/* We've NOT "handled" the FPE - let's chain
		   the signal to other handlers */
		if (system_overflow_handler != SIG_DFL &&
			system_overflow_handler != SIG_IGN &&
			system_overflow_handler != SIG_SGE &&
			system_overflow_handler != SIG_ACK)
		{
			reinterpret_cast<void (*)(int, int)>(system_overflow_handler)(signal, code);
		}
	}
}
#endif

