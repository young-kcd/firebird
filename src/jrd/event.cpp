/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		event.cpp
 *	DESCRIPTION:	Event Manager
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
 * 23-Feb-2002 Dmitry Yemanov - Events wildcarding
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/thd.h"
#include "../jrd/event.h"
#include "../jrd/gdsassert.h"
#include "../jrd/isc_signal.h"
#include "../jrd/event_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../common/config/config.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef UNIX
#include <signal.h>
#endif

#ifdef WIN_NT
#include <process.h>
#include <windows.h>

#define MUTEX		event_mutex
#endif

#ifdef SERVER
#undef MULTI_THREAD
#endif

#ifndef AST_TYPE
#define AST_TYPE	void
#endif

#ifndef MUTEX
#define MUTEX		EVENT_header->evh_mutex
#endif

#define SRQ_BASE                  ((UCHAR *) EVENT_header)

// Not used
// #define EVENT_FLAG		64
const int SEMAPHORES		= 1;
const int MAX_EVENT_BUFFER	= 65500;



static EVH acquire(void);
static FRB alloc_global(UCHAR type, ULONG length, bool recurse);
static SLONG create_process(void);
static void delete_event(EVNT);
static void delete_process(SLONG);
static void delete_request(EVT_REQ);
static void delete_session(SLONG);
static AST_TYPE deliver(void* arg);
static void deliver_request(EVT_REQ);
static void exit_handler(void *);
static EVNT find_event(USHORT, const TEXT*, EVNT);
static void free_global(FRB);
static RINT historical_interest(SES, SLONG);
static void init(void*, SH_MEM, bool);
static void insert_tail(srq *, srq *);
static EVNT make_event(USHORT, const TEXT*, SLONG);
static void mutex_bugcheck(const TEXT*, int);
static void post_process(PRB);
static void probe_processes(void);
static void punt(const TEXT*);
static void release(void);
static void remove_que(srq *);
static bool request_completed(EVT_REQ);
static ISC_STATUS return_ok(ISC_STATUS *);
#ifdef MULTI_THREAD
static THREAD_ENTRY_DECLARE watcher_thread(THREAD_ENTRY_PARAM);
#endif

static SSHORT acquire_count;
static EVH EVENT_header = NULL;
static SLONG EVENT_process_offset, EVENT_default_size;
#ifdef SOLARIS_MT
static PRB EVENT_process;
#endif
static SH_MEM_T EVENT_data;

#if defined(WIN_NT)
static MTX_T event_mutex[1];
#endif


void EVENT_cancel(SLONG request_id)
{
/**************************************
 *
 *	E V E N T _ c a n c e l
 *
 **************************************
 *
 * Functional description
 *	Cancel an outstanding event.
 *
 **************************************/
	srq *event_srq, *que2;

	if (!EVENT_header)
		return;

	acquire();

	PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);

	SRQ_LOOP(process->prb_sessions, que2) {
		SES session = (SES) ((UCHAR *) que2 - OFFSET(SES, ses_sessions));
		SRQ_LOOP(session->ses_requests, event_srq) {
			EVT_REQ request = (EVT_REQ) ((UCHAR *) event_srq - OFFSET(EVT_REQ, req_requests));
			if (request->req_request_id == request_id) {
				delete_request(request);
				release();
				return;
			}
		}
	}

	release();
}


SLONG EVENT_create_session(ISC_STATUS * status_vector)
{
/**************************************
 *
 *	E V E N T _ c r e a t e _ s e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Create session.
 *
 **************************************/
// If we're not initialized, do so now.

	if (!EVENT_header && !EVENT_init(status_vector, true))
		return 0;

	if (!EVENT_process_offset)
		create_process();

	acquire();
	SES session = (SES) alloc_global(type_ses, (SLONG) sizeof(ses), false);
	PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
	session->ses_process = EVENT_process_offset;

#ifdef MULTI_THREAD
	session->ses_flags = 0;
#endif

	insert_tail(&process->prb_sessions, &session->ses_sessions);
	SRQ_INIT(session->ses_requests);
	const SLONG id = SRQ_REL_PTR(session);
	release();

	return id;
}


void EVENT_delete_session(SLONG session_id)
{
/**************************************
 *
 *	E V E N T _ d e l e t e _ s e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Delete a session.
 *
 **************************************/

	if (!EVENT_header)
		return;

	acquire();
	delete_session(session_id);
	release();
}


void EVENT_deliver()
{
/**************************************
 *
 *	E V E N T _ d e l i v e r
 *
 **************************************
 *
 * Functional description
 *	Post an event (step 2).
 *
 *  This code was primarily located in
 *  EVENT_post (see above). This
 *  routine is called by DFW_perform_post_commit_work
 *  once all pending events are prepared
 *  for delivery with EVENT_post.
 *
 **************************************/
	srq	*event_srq;

	/* If we're not initialized, do so now */

	if (!EVENT_header)
    	return;

	acquire();

	/* Deliver requests for posted events */

	bool flag = true;

	while (flag) {
		flag = false;
		SRQ_LOOP (EVENT_header->evh_processes, event_srq)	{
			PRB process = (PRB) ((UCHAR*) event_srq - OFFSET (PRB, prb_processes));
			if (process->prb_flags & PRB_wakeup) {
				post_process(process);
				flag = true;
				break;
			}
		}
	}

	release();
}


// CVC: Contrary to the explanation, server_flag is not used!
#pragma FB_COMPILER_MESSAGE("server_flag var is not honored: bug or deprecated?")
EVH EVENT_init(ISC_STATUS* status_vector, bool server_flag)
{
/**************************************
 *
 *	E V E N T _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for access to shared global region.  If "server_flag" is true,
 *	create region if it doesn't exist.  Return address of header if region
 *	exits, otherwise return NULL.
 *
 **************************************/
	TEXT buffer[MAXPATHLEN];

/* If we're already initialized, there's nothing to do */

	if (EVENT_header)
		return EVENT_header;

	EVENT_default_size = Config::getEventMemSize();

#ifdef SERVER
	EVENT_data.sh_mem_address = EVENT_header =
		(EVH) gds__alloc((SLONG) EVENT_default_size);
/* FREE: apparently only freed at process exit */
	if (!EVENT_header) {		/* NOMEM: */
		status_vector[0] = isc_arg_gds;
		status_vector[1] = isc_virmemexh;
		status_vector[2] = isc_arg_end;
		return NULL;
	}
#ifdef DEBUG_GDS_ALLOC
/* This structure is apparently only freed when the process exits */
/* 1994-October-25 David Schnepper */
	gds_alloc_flag_unfreed((void *) EVENT_header);
#endif /* DEBUG_GDS_ALLOC */

	EVENT_data.sh_mem_length_mapped = EVENT_default_size;
	EVENT_data.sh_mem_mutex_arg = 0;
	init((SLONG) 0, &EVENT_data, true);
#else

#if (defined UNIX)
	EVENT_data.sh_mem_semaphores = SEMAPHORES;
#endif

	gds__prefix_lock(buffer, EVENT_FILE);
	const TEXT* event_file = buffer;

	if (!(EVENT_header = (EVH) ISC_map_file(status_vector,
											event_file,
											init, 0, EVENT_default_size,
											&EVENT_data))) 
	{
#ifdef SERVER
		gds__free(EVENT_data.sh_mem_address);
#endif /* SERVER */
		return NULL;
	}

#endif

	gds__register_cleanup(exit_handler, 0);

#ifdef UNIX
#ifndef MULTI_THREAD
	ISC_signal(EVENT_SIGNAL, deliver, 0);
#endif
#endif

	return EVENT_header;
}


int EVENT_post(ISC_STATUS * status_vector,
			   USHORT major_length,
			   const TEXT * major_code,
			   USHORT minor_length,
			   const TEXT * minor_code,
			   USHORT count)
{
/**************************************
 *
 *	E V E N T _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Post an event.
 *
 **************************************/
	srq *event_srq;

/* If we're not initialized, do so now */

	if (!EVENT_header && !EVENT_init(status_vector, false))
		return status_vector[1];

	acquire();

	EVNT event;
	EVNT parent = find_event(major_length, major_code, 0);
	if (parent &&
		(event = find_event(minor_length, minor_code, parent)))
	{
		event->evnt_count += count;
		SRQ_LOOP(event->evnt_interests, event_srq) {
			RINT interest = (RINT) ((UCHAR *) event_srq - OFFSET(RINT, rint_interests));
			if (interest->rint_request) {
				EVT_REQ request = (EVT_REQ) SRQ_ABS_PTR(interest->rint_request);

				if (interest->rint_count <= event->evnt_count) {
					PRB process = (PRB) SRQ_ABS_PTR(request->req_process);
					process->prb_flags |= PRB_wakeup;
				}
			}
		}
	}

	release();

	return return_ok(status_vector);
}


SLONG EVENT_que(ISC_STATUS* status_vector,
				SLONG session_id,
				USHORT string_length,
				const TEXT* string,
				USHORT events_length,
				const UCHAR* events,
				FPTR_EVENT_CALLBACK ast_routine, void* ast_arg)
{
/**************************************
 *
 *	E V E N T _ q u e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
// Allocate request block

	acquire();
	EVT_REQ request = (EVT_REQ) alloc_global(type_reqb, sizeof(evt_req), false);
	SES session = (SES) SRQ_ABS_PTR(session_id);
	insert_tail(&session->ses_requests, &request->req_requests);
	request->req_session = session_id;
	request->req_process = EVENT_process_offset;
	request->req_ast = ast_routine;
	request->req_ast_arg = ast_arg;
	const SLONG id = ++EVENT_header->evh_request_id;
	request->req_request_id = id;

	const SLONG request_offset = SRQ_REL_PTR(request);

/* Find parent block */
	EVNT parent = find_event(string_length, string, 0);
	if (!parent) {
		parent = make_event(string_length, string, 0);
		request = (EVT_REQ) SRQ_ABS_PTR(request_offset);
		session = (SES) SRQ_ABS_PTR(session_id);
	}

	const SLONG parent_offset = SRQ_REL_PTR(parent);

/* Process event block */

	SRQ_PTR* ptr = &request->req_interests;
	SLONG ptr_offset = SRQ_REL_PTR(ptr);
	const UCHAR* p = events + 1;
	const UCHAR* const end = events + events_length;
	bool flag = false;

	while (p < end) {

		const USHORT count = *p++;

		/* The data in the event block may have trailing blanks.  Strip them off. */

        const UCHAR* find_end = p + count;
		while (--find_end >= p && *find_end == ' ')
			; // nothing to do.
		const USHORT len = find_end - p + 1;
		EVNT event = find_event(len, reinterpret_cast<const char*>(p), parent);
		if (!event) {
			event =
				make_event(len, reinterpret_cast<const char*>(p), parent_offset);
			parent = (EVNT) SRQ_ABS_PTR(parent_offset);
			session = (SES) SRQ_ABS_PTR(session_id);
			request = (EVT_REQ) SRQ_ABS_PTR(request_offset);
			ptr = (SRQ_PTR *) SRQ_ABS_PTR(ptr_offset);
		}
		p += count;
		const SLONG event_offset = SRQ_REL_PTR(event);
		RINT interest, prior;
		if (interest = historical_interest(session, event_offset)) {
			for (SRQ_PTR* ptr2 = &session->ses_interests;
				 *ptr2 && (prior = (RINT) SRQ_ABS_PTR(*ptr2));
				 ptr2 = &prior->rint_next)
			{
				if (prior == interest) {
					*ptr2 = interest->rint_next;
					interest->rint_next = 0;
					break;
				}
			}
		}
		else {
			interest =
				(RINT) alloc_global(type_rint, (SLONG) sizeof(req_int), false);
			event = (EVNT) SRQ_ABS_PTR(event_offset);
			insert_tail(&event->evnt_interests, &interest->rint_interests);
			interest->rint_event = event_offset;

			parent = (EVNT) SRQ_ABS_PTR(parent_offset);
			request = (EVT_REQ) SRQ_ABS_PTR(request_offset);
			ptr = (SRQ_PTR *) SRQ_ABS_PTR(ptr_offset);
			session = (SES) SRQ_ABS_PTR(session_id);
		}
		*ptr = SRQ_REL_PTR(interest);
		ptr = &interest->rint_next;
		ptr_offset = SRQ_REL_PTR(ptr);
		interest->rint_request = request_offset;
		interest->rint_count = gds__vax_integer(p, 4);
		p += 4;
		if (interest->rint_count <= event->evnt_count)
			flag = true;
	}

	if (flag)
		post_process((PRB) SRQ_ABS_PTR(EVENT_process_offset));

	release();
	return_ok(status_vector);

	return id;
}


static EVH acquire(void)
{
/**************************************
 *
 *	a c q u i r e
 *
 **************************************
 *
 * Functional description
 *	Acquire exclusive access to shared global region.
 *
 **************************************/

	int mutex_state;
#ifdef MULTI_THREAD
	if (mutex_state = ISC_mutex_lock(MUTEX))
		mutex_bugcheck("mutex lock", mutex_state);
	EVENT_header->evh_current_process = EVENT_process_offset;
#else
	if (++acquire_count == 1) {
#ifndef SERVER
		if (mutex_state = ISC_mutex_lock(MUTEX))
			mutex_bugcheck("mutex lock", mutex_state);
#endif
		EVENT_header->evh_current_process = EVENT_process_offset;
	}
#endif

	if (EVENT_header->evh_length > EVENT_data.sh_mem_length_mapped) {
		const SLONG length = EVENT_header->evh_length;

#ifdef WIN_NT
		/* Before remapping the memory, wakeup the watcher thread.
		 * Then remap the shared memory and allow the watcher thread
		 * to remap.
		 */
		/* Need to make the following code generic to all SUPERSERVER
		 * platforms. Postponed for now. B.Sriram, 10-Jul-1997
		 */

		PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		process->prb_flags |= PRB_remap;
		event_t* event = process->prb_event;

		post_process(process);

		while (true) {
			release();
			Sleep(3);
			acquire();

			process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
			if (!(process->prb_flags & PRB_remap))
				break;
		}
#endif /* WIN_NT */

	EVH header = NULL;
#if (!(defined SUPERSERVER) && (defined HAVE_MMAP))
		ISC_STATUS_ARRAY status_vector;
		header = (evh*) ISC_remap_file(status_vector, &EVENT_data, length, false);
#endif
		if (!header) {
			release();
			gds__log("acquire: Event table remap failed");
			exit(FINI_ERROR);
		}
		EVENT_header = header;

#ifdef WIN_NT
		process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		process->prb_flags &= ~PRB_remap_over;
#endif /* WIN_NT */
	}

	return EVENT_header;
}


static FRB alloc_global(UCHAR type, ULONG length, bool recurse)
{
/**************************************
 *
 *	a l l o c _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Allocate a block in shared global region.
 *
 **************************************/
	SRQ_PTR *ptr;
	FRB free;
	SLONG best_tail = MAX_SLONG;

	length = ROUNDUP(length, sizeof(IPTR));
	SRQ_PTR* best = NULL;

	for (ptr = &EVENT_header->evh_free; (free = (FRB) SRQ_ABS_PTR(*ptr)) && *ptr;
		 ptr = &free->frb_next) 
	{
		const SLONG tail = free->frb_header.hdr_length - length;
		if (tail >= 0 && (!best || tail < best_tail)) {
			best = ptr;
			best_tail = tail;
		}
	}

	if (!best && !recurse) {
		const SLONG old_length = EVENT_data.sh_mem_length_mapped;
		const SLONG ev_length = old_length + EVENT_EXTEND_SIZE;

#ifdef WIN_NT

		/* Before remapping the memory, wakeup the watcher thread.
		 * Then remap the shared memory and allow the watcher thread
		 * to remap.
		 */

		PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		process->prb_flags |= PRB_remap;
		event_t* event = process->prb_event;
		post_process(process);

		while (true) {
			release();
			THREAD_EXIT();
			Sleep(3);
			THREAD_ENTER();
			acquire();

			process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
			if (!(process->prb_flags & PRB_remap))
				break;
		}
#endif /* WIN_NT */

		EVH header = 0;
#if !((defined SUPERSERVER) && (defined HAVE_MMAP))
		ISC_STATUS_ARRAY status_vector;
		header =
			reinterpret_cast<EVH>
			(ISC_remap_file(status_vector, &EVENT_data, ev_length, true));
#endif
		if (header) {
			free = (FRB) ((UCHAR *) header + old_length);
/**
	free->frb_header.hdr_length = EVENT_EXTEND_SIZE - sizeof (struct evh);
**/
			free->frb_header.hdr_length =
				EVENT_data.sh_mem_length_mapped - old_length;
			free->frb_header.hdr_type = type_frb;
			free->frb_next = 0;

			EVENT_header = header;
			EVENT_header->evh_length = EVENT_data.sh_mem_length_mapped;

			free_global(free);

#ifdef WIN_NT
			process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
			process->prb_flags &= ~PRB_remap_over;
#endif /* WIN_NT */

			return alloc_global(type, length, true);
		}
	}

	if (!best) {
		release();
		gds__log("alloc_global: Event table space exhausted");
		exit(FINI_ERROR);
	}

	free = (FRB) SRQ_ABS_PTR(*best);

	if (best_tail < (SLONG) sizeof(frb))
		*best = free->frb_next;
	else {
		free->frb_header.hdr_length -= length;
		free = (FRB) ((UCHAR *) free + free->frb_header.hdr_length);
		free->frb_header.hdr_length = length;
	}

	memset((UCHAR*) free + sizeof(event_hdr), 0,
		   free->frb_header.hdr_length - sizeof(event_hdr));
	free->frb_header.hdr_type = type;

	return free;
}


static SLONG create_process(void)
{
/**************************************
 *
 *	c r e a t e _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Create process block unless it already exists.
 *
 **************************************/
	if (EVENT_process_offset)
		return EVENT_process_offset;

	acquire();
	PRB process = (PRB) alloc_global(type_prb, (SLONG) sizeof(prb), false);
	insert_tail(&EVENT_header->evh_processes, &process->prb_processes);
	SRQ_INIT(process->prb_sessions);
	EVENT_process_offset = SRQ_REL_PTR(process);

#ifdef MULTI_THREAD

#ifdef SOLARIS_MT
	ISC_STATUS_ARRAY local_status;
	ISC_event_init(process->prb_event, 0, EVENT_SIGNAL);
	EVENT_process = (PRB) ISC_map_object(local_status, &EVENT_data,
										 EVENT_process_offset,
										 sizeof(prb));
#endif

#if !(defined SOLARIS_MT)
	ISC_event_init(process->prb_event, EVENT_SIGNAL, 0);
#endif
#endif

	process->prb_process_id = getpid();

	probe_processes();
	release();

#ifdef MULTI_THREAD
	if (gds__thread_start
		(watcher_thread, NULL,
		 THREAD_medium, THREAD_blast, 0))
		ERR_bugcheck_msg("cannot start thread");
#endif

	return EVENT_process_offset;
}


static void delete_event(EVNT event)
{
/**************************************
 *
 *	d e l e t e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Delete an unused and unloved event.
 *
 **************************************/
	remove_que(&event->evnt_events);

	if (event->evnt_parent) {
		EVNT parent = (EVNT) SRQ_ABS_PTR(event->evnt_parent);
		if (!--parent->evnt_count)
			delete_event(parent);
	}

	free_global((FRB) event);
}


static void delete_process(SLONG process_offset)
{
/**************************************
 *
 *	d e l e t e _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Delete a process block including friends and relations.
 *
 **************************************/
	PRB process = (PRB) SRQ_ABS_PTR(process_offset);

/* Delete any open sessions */

	while (!SRQ_EMPTY(process->prb_sessions)) {
		SES session =
			(SES) ((UCHAR *) SRQ_NEXT(process->prb_sessions) -
				   OFFSET(SES, ses_sessions));
		delete_session(SRQ_REL_PTR(session));
	}

/* Untangle and release process block */

	remove_que(&process->prb_processes);
	free_global((FRB) process);

	if (EVENT_process_offset == process_offset) {
#ifdef MULTI_THREAD
		/* Terminate the event watcher thread */
		/* When we come through the exit handler, the event semaphore might
		   have already been released by another exit handler.  So we cannot
		   use that semaphore to post the event.  Besides, the watcher thread 
		   would be terminated anyway because the whole NLM is being unloaded. */
		// CVC: NLM??? is this Novell Netware specific code???

		process->prb_flags |= PRB_exiting;
		BOOLEAN timeout = FALSE;
		while (process->prb_flags & PRB_exiting && !timeout) {
			ISC_event_post(process->prb_event);
			SLONG value = ISC_event_clear(process->prb_event);
			release();
#ifdef SOLARIS_MT
			event_t* events = EVENT_process->prb_event;
#else
			event_t* events = process->prb_event;
#endif
			timeout = ISC_event_wait(1, &events, &value, 5 * 1000000, 0, 0);
			acquire();
		}
		EVENT_process_offset = 0;
#endif
	}
}


static void delete_request(EVT_REQ request)
{
/**************************************
 *
 *	d e l e t e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release an unwanted and unloved request.
 *
 **************************************/
	SES session = (SES) SRQ_ABS_PTR(request->req_session);

	while (request->req_interests) {
		RINT interest = (RINT) SRQ_ABS_PTR(request->req_interests);

		request->req_interests = interest->rint_next;
		if (historical_interest(session, interest->rint_event)) {
			remove_que(&interest->rint_interests);
			free_global((FRB) interest);
		}
		else {
			interest->rint_next = session->ses_interests;
			session->ses_interests = SRQ_REL_PTR(interest);
			interest->rint_request = (SRQ_PTR)0;
		}
	}

	remove_que(&request->req_requests);
	free_global((FRB) request);
}


static void delete_session(SLONG session_id)
{
/**************************************
 *
 *	d e l e t e _ s e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Delete a session.
 *
 **************************************/
#ifdef MULTI_THREAD
/*  This code is very Netware specific, so for now I've #ifdef'ed this
 *  variable out.  In the future, other platforms may find a need to
 *  use this as well.  --  Morgan Schweers, November 16, 1994
 */
	USHORT kill_anyway = 0;		/*  Kill session despite session delivering. */
	/*  (Generally means session's deliver is hung. */
#endif

	SES session = (SES) SRQ_ABS_PTR(session_id);

#ifdef MULTI_THREAD
/*  delay gives up control for 250ms, so a 40 iteration timeout is a
 *  up to 10 second wait for session to finish delivering its message.
 */
	while (session->ses_flags & SES_delivering && (++kill_anyway != 40)) {
		release();
		THREAD_EXIT();
#ifdef WIN_NT
		Sleep(250);
#endif
#if (defined SOLARIS_MT || defined LINUX)
		sleep(1);
#endif
		THREAD_ENTER();
		acquire();
		session = (SES) SRQ_ABS_PTR(session_id);
	}
#endif

/* Delete all requests */

	while (!SRQ_EMPTY(session->ses_requests)) {
		srq requests = session->ses_requests;
		EVT_REQ request =
			(EVT_REQ) ((UCHAR *) SRQ_NEXT(requests) - OFFSET(EVT_REQ, req_requests));
		delete_request(request);
	}

/* Delete any historical interests */

	while (session->ses_interests) {
		RINT interest = (RINT) SRQ_ABS_PTR(session->ses_interests);
		EVNT event = (EVNT) SRQ_ABS_PTR(interest->rint_event);
		session->ses_interests = interest->rint_next;
		remove_que(&interest->rint_interests);
		free_global((FRB) interest);
		if (SRQ_EMPTY(event->evnt_interests))
			delete_event(event);
	}

	remove_que(&session->ses_sessions);
	free_global((FRB) session);
}


static AST_TYPE deliver(void* arg)
{
/**************************************
 *
 *	d e l i v e r
 *
 **************************************
 *
 * Functional description
 *	We've been poked -- deliver any satisfying requests.
 *
 **************************************/
	srq *event_srq, *que2;

#ifdef UNIX
	if (acquire_count)
		return;
#endif

	acquire();
	PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
	process->prb_flags &= ~PRB_pending;

	SRQ_LOOP(process->prb_sessions, que2) {
		SES session = (SES) ((UCHAR *) que2 - OFFSET(SES, ses_sessions));
#ifdef MULTI_THREAD
		session->ses_flags |= SES_delivering;
#endif
		const SLONG session_offset = SRQ_REL_PTR(session);
		const SLONG que2_offset = SRQ_REL_PTR(que2);
		for (bool flag = true; flag;) {
			flag = false;
			SRQ_LOOP(session->ses_requests, event_srq) {
				EVT_REQ request = (EVT_REQ) ((UCHAR *) event_srq - OFFSET(EVT_REQ, req_requests));
				if (request_completed(request)) {
					deliver_request(request);
					process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
					session = (SES) SRQ_ABS_PTR(session_offset);
					que2 = (srq *) SRQ_ABS_PTR(que2_offset);
					flag = true;
					break;
				}
			}
		}
#ifdef MULTI_THREAD
		session->ses_flags &= ~SES_delivering;
#endif
	}

	release();
}


static void deliver_request(EVT_REQ request)
{
/**************************************
 *
 *	d e l i v e r _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Request has been satisfied, send updated event block to user, then
 *	Clean up request.
 *
 **************************************/
	UCHAR buffer[512];

	FPTR_EVENT_CALLBACK ast = request->req_ast;
	void* arg = request->req_ast_arg;
	UCHAR* event_buffer = buffer;
	UCHAR* p = buffer;
	const UCHAR* end = buffer + sizeof(buffer);
	*p++ = EPB_version1;

/* Loop thru interest block picking up event name, counts, and unlinking
   stuff */

	RINT interest;
	for (SRQ_PTR next = request->req_interests;
		 next && (interest = (RINT) SRQ_ABS_PTR(next));
		 next = interest->rint_next)
	{
		interest = (RINT) SRQ_ABS_PTR(next);
		EVNT event = (EVNT) SRQ_ABS_PTR(interest->rint_event);
		if (end < p + event->evnt_length + 5)
		{
			/* Running out of space - allocate some more and copy it over */
			fb_assert(event_buffer == buffer);	/* we're in this block only once */
			// CVC: We don't check at run-time in the release version, it's
			// likely to produce a buffer overrun if we reach MAX_EVENT_BUFFER,
			// since new_buffer will be again and again this number. FIXED.
			UCHAR* new_buffer = 0;
			if (event_buffer == buffer)
				new_buffer = (UCHAR*)gds__alloc((SLONG) MAX_EVENT_BUFFER);
			/* FREE: at procedure exit */
			if (!new_buffer)
			{	/* NOMEM: */
				gds__log("failed to post all events");
				break;			/* exit loop and send what we have */
			}
			event_buffer = new_buffer;
			memcpy(event_buffer, buffer, p - buffer);
			p = event_buffer + (p - buffer);
			end = event_buffer + MAX_EVENT_BUFFER;
		}
		*p++ = event->evnt_length;
		memcpy(p, event->evnt_name, event->evnt_length);
		p += event->evnt_length;
		const SLONG count = event->evnt_count + 1;
		*p++ = (UCHAR) (count);
		*p++ = (UCHAR) (count >> 8);
		*p++ = (UCHAR) (count >> 16);
		*p++ = (UCHAR) (count >> 24);
	}

	delete_request(request);
	release();
	(*ast)(arg, p - event_buffer, event_buffer);
	if (event_buffer != buffer)
	{
		gds__free(event_buffer);
	}
	acquire();
}


static void exit_handler(void* arg)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Cleanup on exit.
 *
 **************************************/
	if (EVENT_process_offset) {
		if (EVENT_header->evh_current_process != EVENT_process_offset)
			acquire();
		delete_process(EVENT_process_offset);
		release();
	}

	while (acquire_count > 0)
		release();

	ISC_STATUS_ARRAY local_status;

#ifndef SERVER
#ifdef SOLARIS_MT
	ISC_unmap_object(local_status, &EVENT_data, (UCHAR**)&EVENT_process,
					 sizeof(prb));
#endif
	ISC_unmap_file(local_status, &EVENT_data, 0);
#endif

	EVENT_header = NULL;
}


static EVNT find_event(USHORT length, const TEXT* string, EVNT parent)
{
/**************************************
 *
 *	f i n d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Lookup an event.
 *
 **************************************/
	srq* event_srq;

	SRQ_PTR parent_offset = (parent) ? SRQ_REL_PTR(parent) : 0;

	SRQ_LOOP(EVENT_header->evh_events, event_srq) {
		EVNT event = (EVNT) ((UCHAR *) event_srq - OFFSET(EVNT, evnt_events));
		if (event->evnt_parent == parent_offset &&
			event->evnt_length == length &&
			!memcmp(string, event->evnt_name, length))
		{
			return event;
		}
	}

	return NULL;
}


static void free_global(FRB block)
{
/**************************************
 *
 *	f r e e _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Free a previous allocated block.
 *
 **************************************/
	SRQ_PTR *ptr;
	FRB free;

	FRB prior = NULL;
	SRQ_PTR offset = SRQ_REL_PTR(block);
	block->frb_header.hdr_type = type_frb;

	for (ptr = &EVENT_header->evh_free; (free = (FRB) SRQ_ABS_PTR(*ptr)) && *ptr;
		 prior = free, ptr = &free->frb_next)
	{
		if ((SCHAR *) block < (SCHAR *) free)
			break;
	}

	if (offset <= 0 || offset > EVENT_header->evh_length ||
		(prior
		 && (UCHAR*) block < (UCHAR*) prior + prior->frb_header.hdr_length))
	{
		punt("free_global: bad block");
		return;
	}

/* Start by linking block into chain */

	block->frb_next = *ptr;
	*ptr = offset;

/* Try to merge free block with next block */

	if (free && (SCHAR *) block + block->frb_header.hdr_length == (SCHAR *) free)
	{
		block->frb_header.hdr_length += free->frb_header.hdr_length;
		block->frb_next = free->frb_next;
	}

/* Next, try to merge the free block with the prior block */

	if (prior && (SCHAR *) prior + prior->frb_header.hdr_length ==	(SCHAR *) block)
	{
		prior->frb_header.hdr_length += block->frb_header.hdr_length;
		prior->frb_next = block->frb_next;
	}
}


static RINT historical_interest(SES session, SRQ_PTR event)
{
/**************************************
 *
 *	h i s t o r i c a l _ i n t e r e s t
 *
 **************************************
 *
 * Functional description
 *	Find a historical interest, if any, of an event with a session.
 *
 **************************************/
	RINT interest;

	for (SRQ_PTR ptr = session->ses_interests;
		 ptr && (interest = (RINT) SRQ_ABS_PTR(ptr)); ptr = interest->rint_next)
	{
		if (interest->rint_event == event)
			return interest;
	}

	return NULL;
}


static void init(void* arg, SH_MEM shmem_data, bool initialize)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize global region header.
 *
 **************************************/
	int mutex_state;

#if defined(WIN_NT)
	if (mutex_state = ISC_mutex_init(MUTEX, EVENT_FILE))
		mutex_bugcheck("mutex init", mutex_state);
#endif

	if (!initialize)
		return;

	EVENT_header = (EVH) shmem_data->sh_mem_address;
	EVENT_header->evh_length = EVENT_data.sh_mem_length_mapped;
	EVENT_header->evh_version = EVENT_VERSION;
	EVENT_header->evh_request_id = 0;
	SRQ_INIT(EVENT_header->evh_processes);
	SRQ_INIT(EVENT_header->evh_events);

#ifndef SERVER
#if !defined(WIN_NT)
	if (mutex_state = ISC_mutex_init(MUTEX, shmem_data->sh_mem_mutex_arg))
		mutex_bugcheck("mutex init", mutex_state);
#endif
#endif

	FRB free = (FRB) ((UCHAR*) EVENT_header + sizeof(evh));
	free->frb_header.hdr_length =
		EVENT_data.sh_mem_length_mapped - sizeof(evh);
	free->frb_header.hdr_type = type_frb;
	free->frb_next = 0;

	EVENT_header->evh_free = (UCHAR *) free - (UCHAR *) EVENT_header;
}


static void insert_tail(srq * event_srq, srq * node)
{
/**************************************
 *
 *	i n s e r t _ t a i l
 *
 **************************************
 *
 * Functional description
 *	Insert a node at the tail of a event_srq.
 *
 **************************************/
	node->srq_forward = SRQ_REL_PTR(event_srq);
	node->srq_backward = event_srq->srq_backward;

	srq* prior = (srq *) SRQ_ABS_PTR(event_srq->srq_backward);
	prior->srq_forward = SRQ_REL_PTR(node);
	event_srq->srq_backward = SRQ_REL_PTR(node);
}


static EVNT make_event(USHORT length, const TEXT* string, SLONG parent_offset)
{
/**************************************
 *
 *	m a k e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate an link in an event.
 *
 **************************************/
	EVNT event =
		(EVNT) alloc_global(type_evnt, (SLONG) (sizeof(evnt) + length), false);
	insert_tail(&EVENT_header->evh_events, &event->evnt_events);
	SRQ_INIT(event->evnt_interests);

	if (parent_offset) {
		event->evnt_parent = parent_offset;
		EVNT parent = (EVNT) SRQ_ABS_PTR(parent_offset);
		++parent->evnt_count;
	}

	event->evnt_length = length;
	memcpy(event->evnt_name, string, length);

	return event;
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

	sprintf(msg, "EVENT: %s error, status = %d", string, mutex_state);
	gds__log(msg);

	fprintf(stderr, "%s\n", msg);
	exit(FINI_ERROR);
}


static void post_process(PRB process)
{
/**************************************
 *
 *	p o s t _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Wakeup process.
 *
 **************************************/

	process->prb_flags &= ~PRB_wakeup;
	process->prb_flags |= PRB_pending;
	release();

#ifdef SERVER
	deliver();
#else

#ifdef MULTI_THREAD
	ISC_event_post(process->prb_event);
#else

#ifdef UNIX
	if (SRQ_REL_PTR(process) != EVENT_process_offset)
		ISC_kill(process->prb_process_id, EVENT_SIGNAL);
#endif

#endif
	acquire();
#endif
}


static void probe_processes(void)
{
/**************************************
 *
 *	p r o b e _ p r o c e s s e s
 *
 **************************************
 *
 * Functional description
 *	Probe a process to see if it still exists.  If it doesn't, get
 *	rid of it.
 *
 **************************************/
	srq* event_srq;

	SRQ_LOOP(EVENT_header->evh_processes, event_srq) {
		PRB process = (PRB) ((UCHAR *) event_srq - OFFSET(PRB, prb_processes));
		const SLONG process_offset = SRQ_REL_PTR(process);
		if (process_offset != EVENT_process_offset &&
			!ISC_check_process_existence(process->prb_process_id,
										 process->prb_process_uid[0], false))
		{
			event_srq = (srq *) SRQ_ABS_PTR(event_srq->srq_backward);
			delete_process(process_offset);
		}
	}
}


static void punt(const TEXT* string)
{
/**************************************
 *
 *	p u n t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	printf("(EVENT) punt: global region corrupt -- %s\n", string);
}


static void release(void)
{
/**************************************
 *
 *	r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release exclusive control of shared global region.
 *
 **************************************/
	int mutex_state;

#ifdef DEBUG_EVENT
	validate();
#endif

#ifdef MULTI_THREAD
	EVENT_header->evh_current_process = 0;
	if (mutex_state = ISC_mutex_unlock(MUTEX))
		mutex_bugcheck("mutex lock", mutex_state);
#else
	if (!--acquire_count) {
		EVENT_header->evh_current_process = 0;
#ifndef SERVER
		if (mutex_state = ISC_mutex_unlock(MUTEX))
			mutex_bugcheck("mutex lock", mutex_state);
#endif
#ifdef UNIX
		if (EVENT_process_offset) {
			PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
			if (process->prb_flags & PRB_pending)
				ISC_kill(process->prb_process_id, EVENT_SIGNAL);
		}
#endif
	}
#endif
}


static void remove_que(srq* node)
{
/**************************************
 *
 *	r e m o v e _ q u e
 *
 **************************************
 *
 * Functional description
 *	Remove a node from a self-relative event_srq.
 *
 **************************************/
	srq* event_srq = (srq *) SRQ_ABS_PTR(node->srq_forward);
	event_srq->srq_backward = node->srq_backward;

	event_srq = (srq *) SRQ_ABS_PTR(node->srq_backward);
	event_srq->srq_forward = node->srq_forward;
	node->srq_forward = node->srq_backward = 0;
}


static bool request_completed(EVT_REQ request)
{
/**************************************
 *
 *	r e q u e s t _ c o m p l e t e d
 *
 **************************************
 *
 * Functional description
 *	See if request is completed.
 *
 **************************************/
	RINT interest;
	for (SRQ_PTR next = request->req_interests; next; next = interest->rint_next) {
		interest = (RINT) SRQ_ABS_PTR(next);
		EVNT event = (EVNT) SRQ_ABS_PTR(interest->rint_event);
		if (interest->rint_count <= event->evnt_count)
			return true;
	}

	return false;
}


static ISC_STATUS return_ok(ISC_STATUS * status_vector)
{
/**************************************
 *
 *	r e t u r n _ o k
 *
 **************************************
 *
 * Functional description
 *	Everything is ducky -- return success.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = 0;
	*status_vector = isc_arg_end;

	return 0;
}


#ifdef DEBUG_EVENT
static int validate(void)
{
/**************************************
 *
 *	v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Make sure everything looks ok.
 *
 **************************************/
// Check consistency of global region (debugging only)

	SRQ_PTR next_free = 0;
	SLONG offset;

	for (offset = sizeof(evh); offset < EVENT_header->evh_length;
		offset += block->frb_header.hdr_length)
	{
		event_hdr* block = (event_hdr*) SRQ_ABS_PTR(offset);
		if (!block->frb_header.hdr_length || !block->frb_header.hdr_type
			|| block->frb_header.hdr_type >= type_max)
		{
			punt("bad block length or type");
			break;
		}
		if (next_free)
			if (offset == next_free)
				next_free = 0;
			else if (offset > next_free)
				punt("bad free chain");
		if (block->frb_header.hdr_type == type_frb) {
			next_free = ((FRB) block)->frb_next;
			if (next_free >= EVENT_header->evh_length)
				punt("bad frb_next");
		}
	}

	if (offset != EVENT_header->evh_length)
		punt("bad block length");
}
#endif


#ifdef MULTI_THREAD
static THREAD_ENTRY_DECLARE watcher_thread(THREAD_ENTRY_PARAM)
{
/**************************************
 *
 *	w a t c h e r _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Wait for something to happen.
 *
 **************************************/
	while (EVENT_process_offset) {
		acquire();
		PRB process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		process->prb_flags &= ~PRB_wakeup;

		if (process->prb_flags & PRB_exiting) {
			process->prb_flags &= ~PRB_exiting;
			ISC_event_post(process->prb_event);
			release();
			break;
		}
#ifdef WIN_NT
		if (process->prb_flags & PRB_remap) {
			process->prb_flags |= PRB_remap_over;
			process->prb_flags &= ~PRB_remap;
			release();
			while (true) {
				Sleep(3);
				acquire();
				process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
				release();
				if (!(process->prb_flags & PRB_remap_over))
					break;
			}
			acquire();
			process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		}
#endif

		SLONG value = ISC_event_clear(process->prb_event);
		release();
		deliver(NULL);
		acquire();
		process = (PRB) SRQ_ABS_PTR(EVENT_process_offset);
		release();
#ifdef SOLARIS_MT
		event_t* events = EVENT_process->prb_event;
#else
		event_t* events = process->prb_event;
#endif
		ISC_event_wait(1, &events, &value, 0, 0, 0);
	}
	return 0;
}
#endif

