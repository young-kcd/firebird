/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sch.cpp
 *	DESCRIPTION:	Voluntary thread scheduler
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/isc.h"
#include "../jrd/ibase.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/os/thd_priority.h"
#include "../common/classes/locks.h"

#ifdef	WIN_NT
#include <windows.h>
#endif


/* must be careful with alignment on structures like this that are
   not run through the ALL routine */

struct thread {
	struct thread *thread_next;	/* Next thread to be scheduled */
	struct thread *thread_prior;	/* Prior thread */
	event_t thread_stall[1];	/* Generic event to stall thread */
	FB_THREAD_ID thread_id;			/* Current thread id */
	USHORT thread_count;		/* AST disable count */
	USHORT thread_flags;		/* Flags */
};

typedef thread *THREAD;

//  thread_flags

const USHORT THREAD_hiber			= 1;	/* Thread is hibernating */
const USHORT THREAD_ast_disabled	= 2;	/* Disable AST delivery */
const USHORT THREAD_ast_active		= 4;	/* Disable AST preemption while AST active */
const USHORT THREAD_ast_pending		= 8;	/* AST waiting to be delivered */

static THREAD alloc_thread(void);
static bool ast_enable(void);
static void ast_disable(void);
static void cleanup(void *);
static void mutex_bugcheck(const TEXT*, int);
static bool schedule(void);
static bool schedule_active(bool);
static void stall(THREAD);
static void stall_ast(THREAD);

static THREAD free_threads = NULL;
static THREAD active_thread = NULL;
static THREAD ast_thread = NULL;
static MUTX_T thread_mutex[1];
volatile static bool init_flag = false;
static USHORT enabled = FALSE;


#ifdef VMS
int API_ROUTINE gds__ast_active(void)
{
/**************************************
 *
 *	g d s _ $ a s t _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	An asynchronous operation has completed, wake somebody
 *	up, if necessary.
 *
 **************************************/

	return THREAD_ast_active();
}


void API_ROUTINE gds__completion_ast(int *arg)
{
/**************************************
 *
 *	g d s _ $ c o m p l e t i o n _ a s t
 *
 **************************************
 *
 * Functional description
 *	An asynchronous operation has completed, wake somebody
 *	up, if necessary.
 *
 **************************************/

	THREAD_completion_ast();
	sys$wake(0, 0);
}
#endif


int API_ROUTINE gds__thread_enable(int enable_flag)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Check-in with thread traffic cop.
 *
 **************************************/

	if (enable_flag) {
		enabled = TRUE;
		SCH_init();
	}

	return enabled;
}


void API_ROUTINE gds__thread_enter(void)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	Check-in with thread traffic cop.
 *
 **************************************/

	SCH_enter();
}


void API_ROUTINE gds__thread_exit(void)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Check-out with thread traffic cop.
 *
 **************************************/

	SCH_exit();
}


#ifdef VMS
int API_ROUTINE gds__thread_wait(FPTR_INT entrypoint, SLONG arg)
{
/**************************************
 *
 *	g d s _ $ t h r e a d _ w a i t
 *
 **************************************
 *
 * Functional description
 *	Stall a thread with a callback to determine runnability.
 *
 **************************************/

	return thread_wait(entrypoint, arg);
}
#endif


void SCH_abort(void)
{
/**************************************
 *
 *	S C H _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Try to abort the current thread.  If the thread is active,
 *	unlink it.
 *
 **************************************/

/* If threading isn't active, don't sweat it */

	if (!active_thread)
		return;

/* See if we can find thread.  If not, don't worry about it */

	const FB_THREAD_ID id = ThreadData::getId();
	THREAD thread;
	for (THREAD* ptr = &active_thread; thread = *ptr; ptr = &thread->thread_next) {
		if (thread->thread_id == id)
			break;
		if (thread->thread_next == active_thread)
			return;
	}

/* If we're the active thread, do a normal thread exit */

	if (thread == active_thread) {
		SCH_exit();
		return;
	}

/* We're on the list but not active.  Remove from list */

	int mutex_state = THD_mutex_lock(thread_mutex);
	if (mutex_state)
		mutex_bugcheck("mutex lock", mutex_state);
	thread->thread_prior->thread_next = thread->thread_next;
	thread->thread_next->thread_prior = thread->thread_prior;
	thread->thread_next = free_threads;
	free_threads = thread;
	if (mutex_state = THD_mutex_unlock(thread_mutex))
		mutex_bugcheck("mutex unlock", mutex_state);
}


void SCH_ast(enum ast_t action)
{
/**************************************
 *
 *	S C H _ a s t
 *
 **************************************
 *
 * Functional description
 *	Control AST delivery for threaded platforms.
 *	In particular, make AST delivery non-preemptible
 *	to avoid nasty race conditions.
 *
 *	In case you're wondering: AST = Asynchronous System Trap
 *
 **************************************/
	if (!ast_thread && !(action == AST_alloc || action == AST_disable ||
						 action == AST_enable))
	{
		/* Better be an AST thread before we do anything to it! */
		fb_assert(FALSE);
		return;
	}

	if (ast_thread && action == AST_check)
		if (!(ast_thread->thread_flags & THREAD_ast_pending) ||
			ast_thread->thread_count > 1)
			return;

	if (!init_flag)
		SCH_init();

	int mutex_state = THD_mutex_lock(thread_mutex);
	if (mutex_state)
		mutex_bugcheck("mutex lock", mutex_state);

	switch (action) {
		/* Check into thread scheduler as AST thread */

	case AST_alloc:
		ast_thread = alloc_thread();
		ast_thread->thread_flags = THREAD_ast_disabled;
		ast_thread->thread_prior = ast_thread->thread_next = ast_thread;
		break;

	case AST_init:
		ast_thread->thread_id = ThreadData::getId();
		break;

		/* Check out of thread scheduler as AST thread */

	case AST_fini:
		ast_thread->thread_next = free_threads;
		free_threads = ast_thread;
		ast_thread = NULL;
		break;

		/* Disable AST delivery if not an AST thread */

	case AST_disable:
		ast_disable();
		break;

		/* Reenable AST delivery if not an AST thread */

	case AST_enable:
		ast_enable();
		break;

		/* Active thread allows a pending AST to be delivered
		   and waits */

	case AST_check:
		if (ast_enable())
			stall(active_thread);
		else
			ast_disable();
		break;

		/* Request AST delivery and prevent thread scheduling */

	case AST_enter:
		if (ast_thread->thread_flags & THREAD_ast_disabled) {
			ast_thread->thread_flags |= THREAD_ast_pending;
			stall_ast(ast_thread);
		}
		ast_thread->thread_flags |= THREAD_ast_active;
		break;

		/* AST delivery complete; resume thread scheduling */

	case AST_exit:
		ast_thread->thread_flags &= ~(THREAD_ast_active | THREAD_ast_pending);
		if (active_thread)
			ISC_event_post(active_thread->thread_stall);

		/* Post non-active threads that have requested AST disabling */

		for (THREAD thread = ast_thread->thread_next; thread != ast_thread;
			 thread = thread->thread_next)
		{
				 ISC_event_post(thread->thread_stall);
		}
		break;
	}

	if (mutex_state = THD_mutex_unlock(thread_mutex))
		mutex_bugcheck("mutex unlock", mutex_state);
}


THREAD SCH_current_thread(void)
{
/**************************************
 *
 *	S C H _ c u r r e n t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Return id of current thread.  If scheduling is not active,
 *	return NULL.
 *
 **************************************/

	return active_thread;
}


void SCH_enter(void)
{
/**************************************
 *
 *	S C H _ e n t e r
 *
 **************************************
 *
 * Functional description
 *	A thread wants to enter the access method and submit to our scheduling.
 *	Humor him.
 *
 **************************************/

/* Special case single thread case */

#ifndef MULTI_THREAD
	if (free_threads) {
		THREAD thread = active_thread = free_threads;
		free_threads = NULL;
		thread->thread_next = thread->thread_prior = thread;
		thread->thread_flags = 0;
		thread->thread_id = ThreadData::getId();
		return;
	}
#endif

	if (!init_flag) {
		SCH_init();
	}

/* Get mutex on scheduler data structures to prevent tragic misunderstandings */

	int mutex_state = THD_mutex_lock(thread_mutex);
	if (mutex_state) {
		mutex_bugcheck("mutex lock", mutex_state);
	}

	THREAD thread = alloc_thread();
	thread->thread_id = ThreadData::getId();

/* Link thread block into circular list of active threads */

	if (active_thread)
	{
		/* The calling thread should NOT be the active_thread 
		   This is to prevent deadlock by the same thread */
		fb_assert(thread->thread_id != active_thread->thread_id);

		thread->thread_next = active_thread;
		THREAD prior = active_thread->thread_prior;
		thread->thread_prior = prior;
		active_thread->thread_prior = thread;
		prior->thread_next = thread;
	}
	else
	{
		thread->thread_next = thread->thread_prior = thread;
		active_thread = thread;
	}
	ThreadPriorityScheduler::enter();

	if (active_thread->thread_flags & THREAD_hiber) {
		schedule();
	}

	stall(thread);
	if (mutex_state = THD_mutex_unlock(thread_mutex)) {
		mutex_bugcheck("mutex unlock", mutex_state);
	}
}


void SCH_exit(void)
{
/**************************************
 *
 *	S C H _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Thread is done in access method, remove data structure from
 *	scheduler, and release thread block.
 *
 **************************************/
	SCH_validate();

#ifndef MULTI_THREAD
	free_threads = active_thread;
	active_thread = NULL;
	free_threads->thread_next = NULL;
#else
	int mutex_state = THD_mutex_lock(thread_mutex);
	if (mutex_state)
		mutex_bugcheck("mutex lock", mutex_state);

	ast_enable();				/* Reenable AST delivery */

	THREAD thread = active_thread;

	// This is to prevent nasty crash if error (for example, IO error) 
	// happens during attach to database in SS builds. Exception
	// handler there calls THREAD_EXIT without preceding THREAD_ENTER
	// in this case (during shutdown of CACHE_WRITER or CACHE_READER)
	if (!thread) return; 

	if (thread == thread->thread_next)
		active_thread = NULL;
	else {
		THREAD next = thread->thread_next;
		THREAD prior = thread->thread_prior;
		active_thread = prior;
		prior->thread_next = next;
		next->thread_prior = prior;
	}
	ThreadPriorityScheduler::exit();

	thread->thread_next = free_threads;
	free_threads = thread;
	schedule();

	if (mutex_state = THD_mutex_unlock(thread_mutex))
		mutex_bugcheck("mutex unlock", mutex_state);
#endif
}


void SCH_hiber(void)
{
/**************************************
 *
 *	S C H _ h i b e r 
 *
 **************************************
 *
 * Functional description
 *	Go to sleep until woken by another thread.  See also
 *	SCH_wake.
 *
 **************************************/

	schedule_active(true);
}

#ifdef MULTI_THREAD
static Firebird::Mutex scheduler_init_lock;
#endif

void SCH_init(void)
{
/**************************************
 *
 *	S C H _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the thread scheduler.
 *
 **************************************/
	if (init_flag)
		return;
		
#ifdef MULTI_THREAD
	scheduler_init_lock.enter();

	try {
		if (!init_flag) {
#endif
			gds__register_cleanup(cleanup, 0);
			init_flag = true;
#ifdef MULTI_THREAD
			THD_INIT;
		}
	} catch (const std::exception&) {
		scheduler_init_lock.leave();
		throw;
	}
	scheduler_init_lock.leave();
#endif
}


bool SCH_schedule(void)
{
/**************************************
 *
 *	S C H _ s c h e d u l e
 *
 **************************************
 *
 * Functional description
 *	Voluntarily relinquish control so that others may run.
 *	If a context switch actually happened, return true.
 *
 **************************************/
	return schedule_active(false);
}


bool SCH_thread_enter_check(void)
{
/**************************************
 *
 *	S C H _ t h r e a d _ e n t e r _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check if thread is active thread, if so return true
 * else return false.
 *
 **************************************/

/* if active thread is not null and thread_id matches the we are the
   active thread */
	if ((active_thread) && (active_thread->thread_id == ThreadData::getId()))
		return true;

	return false;

}


bool SCH_validate(void)
{
/**************************************
 *
 *	S C H _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Check integrity of thread system (assume thread is entered).
 *
 **************************************/

	if (!init_flag || !active_thread) {
		gds__log("SCH_validate -- not entered");
		if (getenv("ISC_PUNT"))
			abort();
		return false;
	}

#ifdef MULTI_THREAD
	if (active_thread->thread_id != ThreadData::getId()) {
		gds__log("SCH_validate -- wrong thread");
		return false;
	}
#endif

	return true;
}


void SCH_wake(THREAD thread)
{
/**************************************
 *
 *	S C H _ w a k e
 *
 **************************************
 *
 * Functional description
 *	Take thread out of hibernation.
 *
 **************************************/
	thread->thread_flags &= ~THREAD_hiber;
	ISC_event_post(thread->thread_stall);
}


static THREAD alloc_thread(void)
{
/**************************************
 *
 *	a l l o c _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Allocate a thread block.
 *
 **************************************/

/* Find a useable thread block.  If there isn't one, allocate one */

	THREAD thread = free_threads;
	if (thread)
		free_threads = thread->thread_next;
	else {
		thread = (THREAD) gds__alloc((SLONG) sizeof(struct thread));
		/* FREE: unknown */
		if (!thread)			/* NOMEM: bugcheck */
			mutex_bugcheck("Out of memory", 0);	/* no real error handling */
#ifdef DEBUG_GDS_ALLOC
		/* There is one thread structure allocated per thread, and this
		 * is never freed except by process exit
		 */
		gds_alloc_flag_unfreed((void *) thread);
#endif
		ISC_event_init(thread->thread_stall, 0, 0);
	}

	thread->thread_flags = thread->thread_count = 0;
	return thread;
}


static bool ast_enable(void)
{
/**************************************
 *
 *	a s t _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enables AST delivery and returns
 *	TRUE is an AST is deliverable.
 *
 **************************************/
	if (!ast_thread)
		return false;

	if (ast_thread->thread_flags & THREAD_ast_active &&
		ast_thread->thread_id == ThreadData::getId())
		return false;

	if (!ast_thread->thread_count || !--ast_thread->thread_count) {
		ast_thread->thread_flags &= ~THREAD_ast_disabled;
		if (ast_thread->thread_flags & THREAD_ast_pending) {
			ast_thread->thread_flags |= THREAD_ast_active;
			ISC_event_post(ast_thread->thread_stall);
			return true;
		}
	}

	return false;
}


static void ast_disable(void)
{
/**************************************
 *
 *	a s t _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Disables AST delivery and waits
 *	until an active AST completes
 *	before returning.
 *
 **************************************/
	if (!ast_thread)
		return;

	if (ast_thread->thread_flags & THREAD_ast_active) {
		if (ast_thread->thread_id == ThreadData::getId())
			return;
		else {
			if (active_thread
				&& active_thread->thread_id == ThreadData::getId()) 
			{
				stall(active_thread);
				return;
			}
			else {
				THREAD thread = alloc_thread();
				stall_ast(thread);
				thread->thread_next = free_threads;
				free_threads = thread;
			}
		}
	}

	ast_thread->thread_flags |= THREAD_ast_disabled;
	++ast_thread->thread_count;
}


static void cleanup(void *arg)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Exit handler for image exit.
 *
 **************************************/
	if (!init_flag)
		return;

/* this is added to make sure that we release the memory
 * we have alloacted for the thread event handler through
 * ISC_event_handle () (CreateEvent) */

#ifdef SUPERCLIENT
/* use locks */
	THD_mutex_lock(thread_mutex);

	if (!init_flag)
		return;

/* loop through the list of active threads and free the events */
	THREAD temp_thread = active_thread;
	if (temp_thread) {
		/* reach to the starting of the list */
		while (temp_thread != temp_thread->thread_prior)
			temp_thread = temp_thread->thread_prior;
		/* now we are at the begining of the list. Now start freeing the
		 * EVENT structures and move to the next */
		do {
			ISC_event_fini(temp_thread->thread_stall);
			/* the thread structures are freed as a part of the 
			 * gds_alloc cleanup, so do not worry about them here
			 */
		} while (temp_thread->thread_next != temp_thread
			   && (temp_thread = temp_thread->thread_next));

	}

/* loop through the list of free threads and free the events */
	if (temp_thread = free_threads) {
		/* reach to the starting of the list */
		while (temp_thread != temp_thread->thread_prior)
			temp_thread = temp_thread->thread_prior;
		/* now we are at the begining of the list. Now start freeing the
		 * EVENT structures and move to the next */
		do {
			ISC_event_fini(temp_thread->thread_stall);
			/* the thread structures are freed as a part of the 
			 * gds_alloc cleanup, so do not worry about them here
			 */
		}
		while (temp_thread->thread_next != temp_thread
			   && (temp_thread = temp_thread->thread_next));


	}

	THD_mutex_unlock(thread_mutex);
#endif /* SUPERCLIENT */

	init_flag = false;
}


static void mutex_bugcheck(const TEXT* string, int mutex_state)
{
/**************************************
 *
 *	m u t e x _ b u g c h e c k
 *
 **************************************
 *
 * Functional description
 *	There has been a bugcheck during a mutex operation.
 *	Post the bugcheck.
 *
 **************************************/
	TEXT msg[128];

	sprintf(msg, "SCH: %s error, status = %d", string, mutex_state);
	gds__log(msg);
	fprintf(stderr, "%s\n", msg);

	abort();
}


// CVC: Nobody checks the result from this function.
static bool schedule(void)
{
/**************************************
 *
 *	s c h e d u l e
 *
 **************************************
 *
 * Functional description
 *	Loop thru active thread to find the next runable task.  If we find one,
 *	set "active_tasks" to point to it and return true.  Otherwise simply
 *	return false.
 *
 **************************************/
	if (!active_thread)
		return false;

	THREAD thread = active_thread;

	for (;;) {
		thread = thread->thread_next;
		if (!(thread->thread_flags & THREAD_hiber))
			break;
		if (thread == active_thread)
			return false;
	}

	active_thread = thread;
	ISC_event_post(active_thread->thread_stall);

	return true;
}


static bool schedule_active(bool hiber_flag)
{
/**************************************
 *
 *	s c h e d u l e _ a c t i v e
 *
 **************************************
 *
 * Functional description
 *	Voluntarily relinquish control so that others may run.
 *	If a context switch actually happened, return true.
 *
 **************************************/
#ifndef MULTI_THREAD
	return false;
#else
	if (!active_thread)
		return false;

	int mutex_state = THD_mutex_lock(thread_mutex);
	if (mutex_state)
		mutex_bugcheck("mutex lock", mutex_state);

/* Take this opportunity to check for pending ASTs
   and deliver them. */

	if (ast_enable())
		stall(active_thread);
	else
		ast_disable();

	if (hiber_flag)
		active_thread->thread_flags |= THREAD_hiber;
	THREAD thread = active_thread;
	schedule();
	bool ret;
	if (thread == active_thread && !(thread->thread_flags & THREAD_hiber))
		ret = false;
	else {
		ast_enable();
		stall(thread);
		ret = true;
	}

	if (mutex_state = THD_mutex_unlock(thread_mutex))
		mutex_bugcheck("mutex unlock", mutex_state);

	return ret;
#endif
}


static void stall(THREAD thread)
{
/**************************************
 *
 *	s t a l l
 *
 **************************************
 *
 * Functional description
 *	Stall until our thread is made active.
 *
 **************************************/
	if (thread != active_thread || thread->thread_flags & THREAD_hiber ||
		(ast_thread && ast_thread->thread_flags & THREAD_ast_active))
		for (;;) {
			SLONG value = ISC_event_clear(thread->thread_stall);
			if (thread == active_thread
				&& !(thread->thread_flags & THREAD_hiber)
				&& (!ast_thread
					|| !(ast_thread->thread_flags & THREAD_ast_active)))
			{
				break;
			}
			int mutex_state = THD_mutex_unlock(thread_mutex);
			if (mutex_state)
				mutex_bugcheck("mutex unlock", mutex_state);
			event_t* ptr = thread->thread_stall;
			ISC_event_wait(1, &ptr, &value, 0, 0, 0);
			if (mutex_state = THD_mutex_lock(thread_mutex))
				mutex_bugcheck("mutex lock", mutex_state);
		}

/* Explicitly disable AST delivery for active thread */

	ast_disable();
}


static void stall_ast(THREAD thread)
{
/**************************************
 *
 *	s t a l l _ a s t
 *
 **************************************
 *
 * Functional description
 *	If this the AST thread then stall until AST delivery
 *	is reenabled. Otherwise, this is a normal thread (though
 *	not the active thread) and it wants to wait until
 *	AST is complete.
 *
 **************************************/
	if (thread == ast_thread) {
		if (ast_thread->thread_flags & THREAD_ast_disabled)
			for (;;) {
				SLONG value = ISC_event_clear(thread->thread_stall);
				if (!(ast_thread->thread_flags & THREAD_ast_disabled))
					break;
				int mutex_state = THD_mutex_unlock(thread_mutex);
				if (mutex_state)
					mutex_bugcheck("mutex unlock", mutex_state);
				event_t* ptr = thread->thread_stall;
				ISC_event_wait(1, &ptr, &value, 0, 0, 0);
				if (mutex_state = THD_mutex_lock(thread_mutex))
					mutex_bugcheck("mutex lock", mutex_state);
			}
	}
	else {
		/* Link thread block into ast thread queue */

		thread->thread_next = ast_thread->thread_next;
		thread->thread_prior = ast_thread;
		ast_thread->thread_next->thread_prior = thread;
		ast_thread->thread_next = thread;

		/* Wait for AST delivery to complete */

		if (ast_thread->thread_flags & THREAD_ast_active)
			for (;;) {
				SLONG value = ISC_event_clear(thread->thread_stall);
				if (!(ast_thread->thread_flags & THREAD_ast_active))
					break;
				int mutex_state = THD_mutex_unlock(thread_mutex);
				if (mutex_state)
					mutex_bugcheck("mutex unlock", mutex_state);
				event_t* ptr = thread->thread_stall;
				ISC_event_wait(1, &ptr, &value, 0, 0, 0);
				if (mutex_state = THD_mutex_lock(thread_mutex))
					mutex_bugcheck("mutex lock", mutex_state);
			}
		/* Unlink thread block from ast thread queue */

		thread->thread_prior->thread_next = thread->thread_next;
		thread->thread_next->thread_prior = thread->thread_prior;
	}
}

