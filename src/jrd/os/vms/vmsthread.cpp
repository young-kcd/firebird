/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		vmsthread.c
 *	DESCRIPTION:	VMS Thread Manager
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
 */

#include "firebird.h"
#include "../jrd/common.h"

#include <pthread.h>

#define TCB_STACKSIZE	65536

/* Thread control block */

typedef struct tcb {
	struct tcb *tcb_next;
	int (*tcb_routine) ();
	long tcb_routine_arg;
	int (*tcb_callback) ();
	long tcb_callback_arg;
	pthread_cond_t tcb_cond_wait;
	pthread_t tcb_tid;
	char tcb_flags;
} *TCB;

#define TCB_active	1			/* Thread control block is active. */

/* Mutex for synchronizing condition variables. */

static pthread_once_t init_block = pthread_once_init;
static pthread_attr_t thread_attributes;
static pthread_mutex_t threads_mutex;
static TCB threads = 0;

static void thread_init();
static void thread_starter();


THREAD_ast_active()
{
/**************************************
 *
 *	T H R E A D _ a s t _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	Called as completion ast for any asynchronous
 *	system service.
 *
 **************************************/

	return FALSE;
}


THREAD_wakeup()
{
/**************************************
 *
 *	T H R E A D _ w a k e u p
 *
 **************************************
 *
 * Functional description
 *	Called as completion ast for any asynchronous
 *	system service.
 *
 **************************************/
	TCB tcb;
	int (*callback) ();

	if (pthread_once(&init_block, thread_init))
		gds__put_error("THREAD_completion: pthread_once error!\n");

	if (pthread_mutex_lock(&threads_mutex))
		gds__put_error("THREAD_completion: pthread_mutex_lock error!\n");

	for (tcb = threads; tcb; tcb = tcb->tcb_next) {
		if (tcb->tcb_flags & TCB_active &&
			(callback = tcb->tcb_callback) &&
			(*callback) (tcb->tcb_callback_arg))
				if (pthread_cond_signal(&tcb->tcb_cond_wait))
					gds__put_error
					("THREAD_completion: pthread_cond_signal error!\n");
	}

	if (pthread_mutex_unlock(&threads_mutex))
		gds__put_error("THREAD_completion: pthread_mutex_unlock error!\n");
}


THREAD_completion_ast()
{
/**************************************
 *
 *	T H R E A D _ c o m p l e t i o n _ a s t
 *
 **************************************
 *
 * Functional description
 *	Called as completion ast for any asynchronous
 *	system service.
 *
 **************************************/
	TCB tcb;
	int (*callback) ();

	for (tcb = threads; tcb; tcb = tcb->tcb_next) {
		if (tcb->tcb_flags & TCB_active &&
			(callback = tcb->tcb_callback) &&
			(*callback) (tcb->tcb_callback_arg))
				if (pthread_cond_signal_int_np(&tcb->tcb_cond_wait))
				gds__put_error
					("THREAD_completion_ast: pthread_cond_signal_int error!\n");
	}
}


THREAD_start(routine, arg, priority, flags)
	 int (*routine) ();
	 long arg;
	 int priority;
	 int flags;
{
/**************************************
 *
 *	T H R E A D _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start a new thread.  Return 0 if successful,
 *	status if not.
 *
 **************************************/
	TCB tcb, prior;

/* Make sure thread subsystem has been initialized. */

	if (pthread_once(&init_block, thread_init))
		gds__put_error("THREAD_start: pthread_once error!\n");

	if (pthread_mutex_lock(&threads_mutex))
		gds__put_error("THREAD_start: pthread_mutex_lock error!\n");

	prior = threads;
	for (tcb = threads; tcb; tcb = tcb->tcb_next) {
		if (!(tcb->tcb_flags & TCB_active))
			break;
		prior = tcb;
	}

	if (!tcb) {
		tcb = (TCB) gds__alloc((long) sizeof(struct tcb));
		memset(tcb, 0, sizeof(struct tcb));
		if (pthread_cond_init(&tcb->tcb_cond_wait, pthread_condattr_default))
			gds__put_error("THREAD_start: pthread_cond_init error!\n");
		prior->tcb_next = tcb;
	}

	tcb->tcb_callback = 0;
	tcb->tcb_routine = routine;
	tcb->tcb_routine_arg = arg;
	tcb->tcb_flags |= TCB_active;

	if (pthread_mutex_unlock(&threads_mutex))
		gds__put_error("THREAD_start: pthread_mutex_unlock error!\n");

	if (pthread_create(&tcb->tcb_tid, thread_attributes,
					   (pthread_startroutine_t) thread_starter, tcb))
		gds__put_error("THREAD_start: pthread_create error!\n");

	return 0;
}


THREAD_wait(callback, arg)
	 int (*callback) ();
	 long arg;
{
/**************************************
 *
 *	T H R E A D _ w a i t
 *
 **************************************
 *
 * Functional description
 *	Wait on thread condition variable.
 *
 **************************************/
	pthread_t self;
	TCB tcb;

	if (pthread_once(&init_block, thread_init))
		gds__put_error("THREAD_wait: pthread_once() error!\n");

	self = pthread_self();

	if (pthread_mutex_lock(&threads_mutex))
		gds__put_error("THREAD_wait: pthread_mutex_lock() error!\n");

/* Find this thread's control block. */

	for (tcb = threads; tcb; tcb = tcb->tcb_next)
		if (pthread_equal(tcb->tcb_tid, self))
			break;

	if (!tcb)
		gds__put_error("THREAD_wait: can't find thread control block.\n");

/* Catch-all thread completion AST will signal condition
   variables after testing callbacks. */

	tcb->tcb_callback_arg = arg;
	tcb->tcb_callback = callback;

/* Wait for callback predicate to evaluate to TRUE
   before returning. */

	while (!(*callback) (arg))
		if (pthread_cond_wait(&tcb->tcb_cond_wait, &threads_mutex))
			gds__put_error("THREAD_wait: pthread_cond_wait error!\n");

	tcb->tcb_callback = 0;

	if (pthread_mutex_unlock(&threads_mutex))
		gds__put_error("THREAD_wait: pthread_mutex_unlock error!\n");
}


static void thread_init()
{
/**************************************
 *
 *	t h r e a d _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize global mutex and thread characteristics
 *	once and for all.
 *
 **************************************/
	TCB tcb;

	if (pthread_mutex_init(&threads_mutex, pthread_mutexattr_default))
		gds__put_error("thread_init: pthread_mutex_init error!\n");

	if (pthread_attr_create(&thread_attributes))
		gds__put_error("thread_init: pthread_attr_create error!\n");

	if (pthread_attr_setstacksize(&thread_attributes, TCB_STACKSIZE))
		gds__put_error("thread_init: pthread_attr_setstacksize error!\n");

	tcb = (TCB) gds__alloc((long) sizeof(struct tcb));
	memset(tcb, 0, sizeof(struct tcb));
	if (pthread_cond_init(&tcb->tcb_cond_wait, pthread_condattr_default))
		gds__put_error("thread_init: pthread_cond_init error!\n");

	tcb->tcb_tid = pthread_self();
	tcb->tcb_flags |= TCB_active;
	threads = tcb;
}


static void thread_starter(tcb)
	 TCB tcb;
{
/**************************************
 *
 *	t h r e a d _ s t a r t e r
 *
 **************************************
 *
 * Functional description
 *	Wrapper to start a thread. Releases
 *	thread control block on exit.
 *
 **************************************/

	tcb->tcb_tid = pthread_self();

/* Reclaim thread storage on exit. */

/*
if (pthread_detach (&tcb->tcb_tid))
   gds__put_error ("thread_starter: pthread_detach error!\n");
*/

	(*tcb->tcb_routine) (tcb->tcb_routine_arg);

	tcb->tcb_callback = 0;
	tcb->tcb_flags &= ~TCB_active;

	if (pthread_detach(&tcb->tcb_tid))
		gds__put_error("thread_starter: pthread_detach error!\n");
}
