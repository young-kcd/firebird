/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		event.c
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "gen/codes.h"
#include "../jrd/thd.h"
#include "../jrd/event.h"
#include "../jrd/gdsassert.h"
#include "../jrd/isc_signal.h"
#include "../jrd/event_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/isc_i_proto.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#pragma FB_COMPILER_MESSAGE("FIXFIXFIX!!! - DANGER!")
// We currently can't include jrd/err_proto.h to get the function
// declaration. :-<
extern "C" {
extern void DLL_EXPORT ERR_bugcheck_msg(CONST TEXT *);
}


#ifdef UNIX
#include <signal.h>
#define EVENT_SIGNAL	SIGUSR2
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

#define EVENT_FLAG		64
#define SEMAPHORES		1
#define MAX_EVENT_BUFFER	65500


extern "C" {


static EVH acquire(void);
static FRB alloc_global(UCHAR type, ULONG length, BOOLEAN recurse);
static SLONG create_process(void);
static void delete_event(EVNT);
static void delete_process(SLONG);
static void delete_request(REQ);
static void delete_session(SLONG);
static AST_TYPE deliver(void);
static void deliver_request(REQ);
static void exit_handler(void *);
#ifdef EVENTS_WILDCARDING
static EVNT	find_event(USHORT, TEXT *, EVNT, BOOLEAN, EVNT);
#else /* EVENTS_WILDCARDING */
static EVNT find_event(USHORT, TEXT *, EVNT);
#endif /* EVENTS_WILDCARDING */
static void free_global(FRB);
static RINT historical_interest(SES, SLONG);
static void init(void *, SH_MEM, int);
static void insert_tail(SRQ *, SRQ *);
static EVNT make_event(USHORT, TEXT *, SLONG);
static void mutex_bugcheck(TEXT *, int);
static void post_process(PRB);
static void probe_processes(void);
static void punt(TEXT *);
static void release(void);
static void remove_que(SRQ *);
static BOOLEAN request_completed(REQ);
static STATUS return_ok(STATUS *);
static void THREAD_ROUTINE watcher_thread(void *);

static SSHORT acquire_count;
static EVH EVENT_header = NULL;
static SLONG EVENT_process_offset, EVENT_default_size;
static PRB EVENT_process;
static SH_MEM_T EVENT_data;

#if defined(WIN_NT)
static MTX_T event_mutex[1];
#endif

static struct ipccfg EVENT_hdrtbl[] =
{
#if !(defined WIN_NT)
	{"V4_EVENT_MEM_SIZE", -1, &EVENT_default_size, 0, 0},
	{"ANY_EVENT_MEM_SIZE", -1, &EVENT_default_size, -1, 0},
#endif
	{NULL, -1, NULL, 0, 0}
};

/* Wildcard characters */

#define MATCH_ONE '?'
#define MATCH_ANY '*'


#ifdef EVENTS_WILDCARDING
USHORT MATCHESNAME(TEXT	* p1, SSHORT l1_bytes, TEXT	* p2, SSHORT l2_bytes)
{
/**************************************
 *
 *	MATCHESNAME
 *
 **************************************
 *
 * Functional description
 *	A simplified version of EVL_??_matches.
 *  Used for events wildcarding.
 *
 **************************************/
	TEXT c;
	SSHORT l1, l2;

	assert(p1 != NULL);
	assert(p2 != NULL);
	assert((l1_bytes % sizeof (TEXT)) == 0);
	assert((l2_bytes % sizeof (TEXT)) == 0);

	l1 = l1_bytes / sizeof (TEXT);
	l2 = l2_bytes / sizeof (TEXT);

	while (l2-- > 0) {
		c = *p2++;
		if (c == (TEXT) MATCH_ANY) {
			while ((l2 > 0) && (*p2 == (TEXT) MATCH_ANY)) {
				l2--;
				p2++;
			}
			if (l2 == 0)
				return TRUE;
			while (l1)
				if (MATCHESNAME(p1++, 
								l1-- * sizeof (TEXT),
								p2, 
								l2 * sizeof (TEXT)))
					return TRUE;
			return FALSE;
		}
		if (l1-- == 0)
			return FALSE;
		if ((c != (TEXT) MATCH_ONE && c != *p1))
			return FALSE;
		p1++;
	}

	return (l1) ? FALSE : TRUE;
}
#endif /* EVENTS_WILDCARDING */


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
	PRB process;
	REQ request;
	SES session;
	SRQ *que, *que2;

	if (!EVENT_header)
		return;

	ACQUIRE;

	process = (PRB) ABS_PTR(EVENT_process_offset);

	QUE_LOOP(process->prb_sessions, que2) {
		session = (SES) ((UCHAR *) que2 - OFFSET(SES, ses_sessions));
		QUE_LOOP(session->ses_requests, que) {
			request = (REQ) ((UCHAR *) que - OFFSET(REQ, req_requests));
			if (request->req_request_id == request_id) {
				delete_request(request);
				RELEASE;
				return;
			}
		}
	}

	RELEASE;
}


SLONG EVENT_create_session(STATUS * status_vector)
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
	PRB process;
	SES session;
	SLONG id;

/* If we're not initialized, do so now */

	if (!EVENT_header && !EVENT_init(status_vector, TRUE))
		return 0;

	if (!EVENT_process_offset)
		create_process();

	ACQUIRE;
	session = (SES) alloc_global(type_ses, (SLONG) sizeof(struct ses), FALSE);
	process = (PRB) ABS_PTR(EVENT_process_offset);
	session->ses_process = EVENT_process_offset;

#ifdef MULTI_THREAD
	session->ses_flags = 0;
#endif

	insert_tail(&process->prb_sessions, &session->ses_sessions);
	QUE_INIT(session->ses_requests);
	id = REL_PTR(session);
	RELEASE;

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

	ACQUIRE;
	delete_session(session_id);
	RELEASE;
}


EVH EVENT_init(STATUS * status_vector, USHORT server_flag)
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
	TEXT *event_file, buffer[256];

/* If we're already initialized, there's nothing to do */

	if (EVENT_header)
		return EVENT_header;

	EVENT_default_size = EVENT_DEFAULT_SIZE;
	ISC_get_config(LOCK_HEADER, EVENT_hdrtbl);

#ifdef SERVER
	EVENT_data.sh_mem_address = EVENT_header =
		(EVH) gds__alloc((SLONG) EVENT_default_size);
/* FREE: apparently only freed at process exit */
	if (!EVENT_header) {		/* NOMEM: */
		status_vector[0] = gds_arg_gds;
		status_vector[1] = gds__virmemexh;
		status_vector[2] = gds_arg_end;
		return NULL;
	}
#ifdef DEBUG_GDS_ALLOC
/* This structure is apparently only freed when the process exits */
/* 1994-October-25 David Schnepper */
	gds_alloc_flag_unfreed((void *) EVENT_header);
#endif /* DEBUG_GDS_ALLOC */

	EVENT_data.sh_mem_length_mapped = EVENT_default_size;
	EVENT_data.sh_mem_mutex_arg = 0;
	init((SLONG) 0, &EVENT_data, TRUE);
#else

#if (defined UNIX || NETWARE_386)
	EVENT_data.sh_mem_semaphores = SEMAPHORES;
#endif

	gds__prefix_lock(buffer, EVENT_FILE);
	event_file = buffer;

	if (!(EVENT_header = (EVH) ISC_map_file(status_vector,
											event_file,
											reinterpret_cast < void (*)(void *, struct sh_mem*, int) >
											(init), 0, EVENT_default_size,
											&EVENT_data))) {
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


int EVENT_post(STATUS * status_vector,
			   USHORT major_length,
			   TEXT * major_code,
			   USHORT minor_length,
			   TEXT * minor_code,
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
	REQ request;
	SRQ *que;
	PRB process;
	EVNT event, parent;
	RINT interest;
	USHORT flag;
#ifdef EVENTS_WILDCARDING
	APE act, act_new;
#endif /* EVENTS_WILDCARDING */

/* If we're not initialized, do so now */

	if (!EVENT_header && !EVENT_init(status_vector, FALSE))
		return status_vector[1];

#ifdef EVENTS_WILDCARDING
	event = NULL;
#endif /* EVENTS_WILDCARDING */

	ACQUIRE;

#ifdef EVENTS_WILDCARDING
	while ((parent = find_event (major_length, major_code, NULL, FALSE, NULL)) &&
		(event = find_event (minor_length, minor_code, parent, TRUE, event))) {
#else /* EVENTS_WILDCARDING */
	if ((parent = find_event(major_length, major_code, 0)) &&
		(event = find_event(minor_length, minor_code, parent))) {
#endif /* EVENTS_WILDCARDING */
		event->evnt_count += count;
		QUE_LOOP(event->evnt_interests, que) {
			interest = (RINT) ((UCHAR *) que - OFFSET(RINT, rint_interests));
			if (interest->rint_request) {
				request = (REQ) ABS_PTR(interest->rint_request);

#ifdef EVENTS_WILDCARDING
				/* Allocate and fill the APE block, link it to the interest */

				act_new = (APE) alloc_global (type_ape, (SLONG) sizeof (struct ape) + minor_length, FALSE);
				act_new->ape_next = NULL;
				act_new->ape_count = count;
				act_new->ape_length = minor_length;
				memcpy (act_new->ape_name, minor_code, minor_length);
				if (!interest->rint_apes)
					interest->rint_apes = (PTR) act_new;
				act = (APE) interest->rint_apes;
				while (act->ape_next)
					act = (APE) act->ape_next;
				act->ape_next = (act == act_new) ? NULL : act_new;
#endif /* EVENTS_WILDCARDING */

				if (interest->rint_count <= event->evnt_count) {
					process = (PRB) ABS_PTR(request->req_process);
					process->prb_flags |= PRB_wakeup;
				}
			}
		}
#ifndef EVENTS_WILDCARDING
		for (flag = TRUE; flag;) {
			flag = FALSE;
			QUE_LOOP(EVENT_header->evh_processes, que) {
				process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
				if (process->prb_flags & PRB_wakeup) {
					post_process(process);
					flag = TRUE;
					break;
				}
			}
		}
#endif /* EVENTS_WILDCARDING */
	}

	RELEASE;

	return return_ok(status_vector);
}


#ifdef EVENTS_WILDCARDING
void EVENT_post_finalize()
{
/**************************************
 *
 *	E V E N T _ p o s t _ f i n a l i z e
 *
 **************************************
 *
 * Functional description
 *	Post an event (step 2).
 *
 *  This code was primarily located in
 *  EVENT_post (see above), but with
 *  introduction of wildcarding it has
 *  been implemented as standalone. This
 *  routine is called by DFW_perform_post_commit_work
 *  once all pending events are prepared
 *  for delivery with EVENT_post.
 *
 **************************************/
	SRQ	*que;
	PRB	process;
	USHORT flag = TRUE;

	/* If we're not initialized, do so now */

	if (!EVENT_header)
    	return;

	ACQUIRE;

	/* Deliver requests for posted events */

	while (flag) {
		flag = FALSE;
		QUE_LOOP (EVENT_header->evh_processes, que)	{
			process = (PRB) ((UCHAR*) que - OFFSET (PRB, prb_processes));
			if (process->prb_flags & PRB_wakeup) {
				post_process (process);
				flag = TRUE;
				break;
			}
		}
	}

	RELEASE;
}
#endif /* EVENTS_WILDCARDING */


SLONG EVENT_que(STATUS * status_vector,
				SLONG session_id,
				USHORT string_length,
				TEXT * string,
				USHORT events_length,
				UCHAR * events, FPTR_VOID ast_routine, void *ast_arg)
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
	UCHAR *p, *end, *find_end;
	USHORT count, flag, len;
	SES session;
	REQ request;
	EVNT event, parent;
	RINT interest, prior;
	PTR *ptr, *ptr2;
	SLONG id, request_offset, parent_offset, event_offset, ptr_offset;

/* Allocate request block */

	ACQUIRE;
	request = (REQ) alloc_global(type_req, (SLONG) sizeof(struct req), FALSE);
	session = (SES) ABS_PTR(session_id);
	insert_tail(&session->ses_requests, &request->req_requests);
	request->req_session = session_id;
	request->req_process = EVENT_process_offset;
	request->req_ast = ast_routine;
	request->req_ast_arg = ast_arg;
	request->req_request_id = id = ++EVENT_header->evh_request_id;

	request_offset = REL_PTR(request);

/* Find parent block */

#ifdef EVENTS_WILDCARDING
	if (!(parent = find_event(string_length, string, NULL, FALSE, NULL))) {
#else /* EVENTS_WILDCARDING */
	if (!(parent = find_event(string_length, string, 0))) {
#endif /* EVENTS_WILDCARDING */
		parent = make_event(string_length, string, 0);
		request = (REQ) ABS_PTR(request_offset);
		session = (SES) ABS_PTR(session_id);
	}

	parent_offset = REL_PTR(parent);

/* Process event block */

	ptr = &request->req_interests;
	ptr_offset = REL_PTR(ptr);
	p = events + 1;
	end = events + events_length;
	flag = FALSE;

#ifdef EVENTS_WILDCARDING
	while (p < end && *p) { /* Ignore the detailed part of EPB */
#else /* EVENTS_WILDCARDING */
	while (p < end) {
#endif /* EVENTS_WILDCARDING */
		count = *p++;

		/* The data in the event block may have trailing blanks.  Strip them off. */

		for (find_end = p + count; --find_end >= p && *find_end == ' ';);
		len = find_end - p + 1;
#ifdef EVENTS_WILDCARDING
		if (!(event = find_event(len, reinterpret_cast < char *>(p), parent, FALSE, NULL)))
#else /* EVENTS_WILDCARDING */
		if (!(event = find_event(len, reinterpret_cast < char *>(p), parent))) {
#endif /* EVENTS_WILDCARDING */
			event =
				make_event(len, reinterpret_cast < char *>(p), parent_offset);
			parent = (EVNT) ABS_PTR(parent_offset);
			session = (SES) ABS_PTR(session_id);
			request = (REQ) ABS_PTR(request_offset);
			ptr = (PTR *) ABS_PTR(ptr_offset);
		}
		p += count;
		event_offset = REL_PTR(event);
		if (interest = historical_interest(session, event_offset)) {
			for (ptr2 = &session->ses_interests;
				 *ptr2 && (prior = (RINT) ABS_PTR(*ptr2));
				 ptr2 = &prior->rint_next) if (prior == interest) {
					*ptr2 = interest->rint_next;
					interest->rint_next = 0;
					break;
				}
		}
		else {
			interest =
				(RINT) alloc_global(type_rint, (SLONG) sizeof(struct rint),
									FALSE);
			event = (EVNT) ABS_PTR(event_offset);
			insert_tail(&event->evnt_interests, &interest->rint_interests);
			interest->rint_event = event_offset;

			parent = (EVNT) ABS_PTR(parent_offset);
			request = (REQ) ABS_PTR(request_offset);
			ptr = (PTR *) ABS_PTR(ptr_offset);
			session = (SES) ABS_PTR(session_id);
		}
#ifdef EVENTS_WILDCARDING
		interest->rint_apes = NULL;
#endif /* EVENTS_WILDCARDING */
		*ptr = REL_PTR(interest);
		ptr = &interest->rint_next;
		ptr_offset = REL_PTR(ptr);
		interest->rint_request = request_offset;
		interest->rint_count = gds__vax_integer(p, 4);
		p += 4;
		if (interest->rint_count <= event->evnt_count)
			flag = TRUE;
	}

	if (flag)
		post_process((PRB) ABS_PTR(EVENT_process_offset));

	RELEASE;
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
	SLONG length;
	EVH header = NULL;
#ifdef WIN_NT
	PRB process;
	EVENT event;
#endif

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
		length = EVENT_header->evh_length;

#ifdef WIN_NT
		/* Before remapping the memory, wakeup the watcher thread.
		 * Then remap the shared memory and allow the watcher thread
		 * to remap.
		 */
		/* Need to make the following code generic to all SUPERSERVER
		 * platforms. Postponed for now. B.Sriram, 10-Jul-1997
		 */

		process = (PRB) ABS_PTR(EVENT_process_offset);
		process->prb_flags |= PRB_remap;
		event = process->prb_event;

		post_process(process);

		while (1) {
			RELEASE;
			Sleep(3);
			ACQUIRE;

			process = (PRB) ABS_PTR(EVENT_process_offset);
			if (!(process->prb_flags & PRB_remap))
				break;
		}
#endif /* WIN_NT */

#if (!(defined SUPERSERVER) && (defined HAVE_MMAP))
		STATUS status_vector[20];
		header = (evh*) ISC_remap_file(status_vector, &EVENT_data, length, FALSE);
#endif
		if (!header) {
			RELEASE;
			gds__log("acquire: Event table remap failed");
			exit(FINI_ERROR);
		}
		EVENT_header = header;

#ifdef WIN_NT
		process = (PRB) ABS_PTR(EVENT_process_offset);
		process->prb_flags &= ~PRB_remap_over;
#endif /* WIN_NT */
	}

	return EVENT_header;
}


static FRB alloc_global(UCHAR type, ULONG length, BOOLEAN recurse)
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
	PTR *ptr, *best;
	FRB free;
	SLONG tail, best_tail, ev_length, old_length;
	STATUS status_vector[20];
#ifdef WIN_NT
	PRB process;
	EVENT event;
#endif

	length = ROUNDUP(length, sizeof(IPTR));
	best = NULL;

	for (ptr = &EVENT_header->evh_free; (free = (FRB) ABS_PTR(*ptr)) && *ptr;
		 ptr = &free->frb_next) {
		tail = free->frb_header.hdr_length - length;
		if (tail >= 0 && (!best || tail < best_tail)) {
			best = ptr;
			best_tail = tail;
		}
	}

	if (!best && !recurse) {
		old_length = EVENT_data.sh_mem_length_mapped;
		ev_length = old_length + EVENT_EXTEND_SIZE;

#ifdef WIN_NT

		/* Before remapping the memory, wakeup the watcher thread.
		 * Then remap the shared memory and allow the watcher thread
		 * to remap.
		 */

		process = (PRB) ABS_PTR(EVENT_process_offset);
		process->prb_flags |= PRB_remap;
		event = process->prb_event;
		post_process(process);

		while (1) {
			RELEASE;
			THREAD_EXIT;
			Sleep(3);
			THREAD_ENTER;
			ACQUIRE;

			process = (PRB) ABS_PTR(EVENT_process_offset);
			if (!(process->prb_flags & PRB_remap))
				break;
		}
#endif /* WIN_NT */

	EVH header = 0;
#if !((defined SUPERSERVER) && (defined HAVE_MMAP))
		header =
			reinterpret_cast < EVH >
			(ISC_remap_file(status_vector, &EVENT_data, ev_length, TRUE));
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
			process = (PRB) ABS_PTR(EVENT_process_offset);
			process->prb_flags &= ~PRB_remap_over;
#endif /* WIN_NT */

			return alloc_global(type, length, TRUE);
		}
	}

	if (!best) {
		RELEASE;
		gds__log("alloc_global: Event table space exhausted");
		exit(FINI_ERROR);
	}

	free = (FRB) ABS_PTR(*best);

	if (best_tail < sizeof(struct frb))
		*best = free->frb_next;
	else {
		free->frb_header.hdr_length -= length;
		free = (FRB) ((UCHAR *) free + free->frb_header.hdr_length);
		free->frb_header.hdr_length = length;
	}

	memset((UCHAR *) free + sizeof(struct hdr), 0,
		   free->frb_header.hdr_length - sizeof(struct hdr));
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
	PRB process;

	if (EVENT_process_offset)
		return EVENT_process_offset;

	ACQUIRE;
	process = (PRB) alloc_global(type_prb, (SLONG) sizeof(struct prb), FALSE);
	insert_tail(&EVENT_header->evh_processes, &process->prb_processes);
	QUE_INIT(process->prb_sessions);
	EVENT_process_offset = REL_PTR(process);

#ifdef MULTI_THREAD

#ifdef NETWARE_386
	ISC_event_init(process->prb_event, 0, 0);
#endif

#ifdef SOLARIS_MT
	STATUS local_status[20];
	ISC_event_init(process->prb_event, 0, EVENT_SIGNAL);
	EVENT_process = (PRB) ISC_map_object(local_status, &EVENT_data,
										 EVENT_process_offset,
										 sizeof(struct prb));
#endif

#if !(defined NETWARE_386 || defined SOLARIS_MT)
	ISC_event_init(process->prb_event, EVENT_SIGNAL, 0);
#endif
#endif

#ifdef NETWARE_386
	process->prb_process_id = GetThreadID();
#else
	process->prb_process_id = getpid();
#endif

	probe_processes();
	RELEASE;

#ifdef MULTI_THREAD
	if (gds__thread_start
		(reinterpret_cast < FPTR_INT_VOID_PTR > (watcher_thread), NULL,
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
	EVNT parent;

	remove_que(&event->evnt_events);

	if (event->evnt_parent) {
		parent = (EVNT) ABS_PTR(event->evnt_parent);
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
	PRB process;
	SES session;
	EVENT events;
	SLONG value;
	BOOLEAN timeout;

	process = (PRB) ABS_PTR(process_offset);

/* Delete any open sessions */

	while (!QUE_EMPTY(process->prb_sessions)) {
		session =
			(SES) ((UCHAR *) QUE_NEXT(process->prb_sessions) -
				   OFFSET(SES, ses_sessions));
		delete_session(REL_PTR(session));
	}

/* Untangle and release process block */

	remove_que(&process->prb_processes);
	free_global((FRB) process);

	if (EVENT_process_offset == process_offset) {
#ifdef MULTI_THREAD
		/* Terminate the event watcher thread */
#ifndef NETWARE_386
		/* When we come through the exit handler, the event semaphore might
		   have already been released by another exit handler.  So we cannot
		   use that semaphore to post the event.  Besides, the watcher thread 
		   would be terminated anyway because the whole NLM is being unloaded. */

		process->prb_flags |= PRB_exiting;
		timeout = FALSE;
		while (process->prb_flags & PRB_exiting && !timeout) {
			ISC_event_post(process->prb_event);
			value = ISC_event_clear(process->prb_event);
			RELEASE;
#ifdef SOLARIS_MT
			events = EVENT_process->prb_event;
#else
			events = process->prb_event;
#endif
			timeout = ISC_event_wait(1, &events, &value, 5 * 1000000, 0, 0);
			ACQUIRE;
		}
#endif
		EVENT_process_offset = NULL;
#endif
	}
}


static void delete_request(REQ request)
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
	SES session;
	RINT interest;
#ifdef EVENTS_WILDCARDING
	APE act1, act2;
#endif /* EVENTS_WILDCARDING */
	
	session = (SES) ABS_PTR(request->req_session);

	while (request->req_interests) {
		interest = (RINT) ABS_PTR(request->req_interests);

#ifdef EVENTS_WILDCARDING
		/* Free all APE blocks associated with the request */	
		act1 = (APE) interest->rint_apes;
		while (act1) {
			act2 = (APE) act1->ape_next;
			free_global((FRB) act1);
			act1 = act2;
		}
		interest->rint_apes = NULL;
#endif /* EVENTS_WILDCARDING */
	
		request->req_interests = interest->rint_next;
		if (historical_interest(session, interest->rint_event)) {
			remove_que(&interest->rint_interests);
			free_global((FRB) interest);
		}
		else {
			interest->rint_next = session->ses_interests;
			session->ses_interests = REL_PTR(interest);
			interest->rint_request = NULL;
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
	SES session;
	SRQ requests;
	REQ request;
	EVNT event;
	RINT interest;
#ifdef MULTI_THREAD
/*  This code is very Netware specific, so for now I've #ifdef'ed this
 *  variable out.  In the future, other platforms may find a need to
 *  use this as well.  --  Morgan Schweers, November 16, 1994
 */
	USHORT kill_anyway = 0;		/*  Kill session despite session delivering. */
	/*  (Generally means session's deliver is hung. */
#endif

	session = (SES) ABS_PTR(session_id);

#ifdef MULTI_THREAD
/*  delay gives up control for 250ms, so a 40 iteration timeout is a
 *  up to 10 second wait for session to finish delivering its message.
 */
	while (session->ses_flags & SES_delivering && (++kill_anyway != 40)) {
		RELEASE;
		THREAD_EXIT;
#ifdef NETWARE_386
		delay(250);
#endif
#ifdef WIN_NT
		Sleep(250);
#endif
#if (defined SOLARIS_MT || defined LINUX)
		sleep(1);
#endif
		THREAD_ENTER;
		ACQUIRE;
		session = (SES) ABS_PTR(session_id);
	}
#endif

/* Delete all requests */

	while (!QUE_EMPTY(session->ses_requests)) {
		requests = session->ses_requests;
		request =
			(REQ) ((UCHAR *) QUE_NEXT(requests) - OFFSET(REQ, req_requests));
		delete_request(request);
	}

/* Delete any historical interests */

	while (session->ses_interests) {
		interest = (RINT) ABS_PTR(session->ses_interests);
		event = (EVNT) ABS_PTR(interest->rint_event);
		session->ses_interests = interest->rint_next;
		remove_que(&interest->rint_interests);
		free_global((FRB) interest);
		if (QUE_EMPTY(event->evnt_interests))
			delete_event(event);
	}

	remove_que(&session->ses_sessions);
	free_global((FRB) session);
}


static AST_TYPE deliver(void)
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
	PRB process;
	REQ request;
	SES session;
	SRQ *que, *que2;
	USHORT flag;
	SLONG session_offset, que2_offset;

#ifdef UNIX
	if (acquire_count)
		return;
#endif

	ACQUIRE;
	process = (PRB) ABS_PTR(EVENT_process_offset);
	process->prb_flags &= ~PRB_pending;

	QUE_LOOP(process->prb_sessions, que2) {
		session = (SES) ((UCHAR *) que2 - OFFSET(SES, ses_sessions));
#ifdef MULTI_THREAD
		session->ses_flags |= SES_delivering;
#endif
		session_offset = REL_PTR(session);
		que2_offset = REL_PTR(que2);
		for (flag = TRUE; flag;) {
			flag = FALSE;
			QUE_LOOP(session->ses_requests, que) {
				request = (REQ) ((UCHAR *) que - OFFSET(REQ, req_requests));
				if (request_completed(request)) {
					deliver_request(request);
					process = (PRB) ABS_PTR(EVENT_process_offset);
					session = (SES) ABS_PTR(session_offset);
					que2 = (SRQ *) ABS_PTR(que2_offset);
					flag = TRUE;
					break;
				}
			}
		}
#ifdef MULTI_THREAD
		session->ses_flags &= ~SES_delivering;
#endif
	}

	RELEASE;
}


static void deliver_request(REQ request)
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
	void (*ast) ();
	void *arg;
#ifdef EVENTS_WILDCARDING
	SLONG count, index, length;
	APE act;
#else /* EVENTS_WILDCARDING */
	SLONG count;
#endif /* EVENTS_WILDCARDING */
	RINT interest;
	PTR next;
	EVNT event;
	UCHAR buffer[512], *p, *event_buffer, *end;

	ast = request->req_ast;
	arg = request->req_ast_arg;
	p = event_buffer = buffer;
	end = buffer + sizeof(buffer);
	*p++ = 1;

/* Loop thru interest block picking up event name, counts, and unlinking
   stuff */

	for (next = request->req_interests;
		 next && (interest = (RINT) ABS_PTR(next));
		 next = interest->rint_next)
	{
		interest = (RINT) ABS_PTR(next);
		event = (EVNT) ABS_PTR(interest->rint_event);
		if (end < p + event->evnt_length + 5)
		{
			/* Running out of space - allocate some more and copy it over */
			assert(event_buffer == buffer);	/* we're in this block only once */
			UCHAR* new_buffer = (UCHAR*)gds__alloc((SLONG) MAX_EVENT_BUFFER);
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
		count = event->evnt_count + 1;
		*p++ = (UCHAR) (count);
		*p++ = (UCHAR) (count >> 8);
		*p++ = (UCHAR) (count >> 16);
		*p++ = (UCHAR) (count >> 24);
	}

#ifdef EVENTS_WILDCARDING
	/* Collect information about actually posted events to construct
	   the detailed part of EPB */

	length = (SLONG) (p - event_buffer);
	*p++ = 0;
	end = p;
	p += sizeof(USHORT);

	for (next = request->req_interests, index = 0;
		 next && (interest = (RINT) ABS_PTR (next));
		 next = interest->rint_next, index++) {
		act = interest->rint_apes;
		while (act)	{
/*			if (!act->ape_count)
				continue; */
			*p++ = index;
			*p++ = act->ape_length;
			memcpy(p, act->ape_name, act->ape_length);
			p += act->ape_length;
			*p++ = act->ape_count;
			*p++ = act->ape_count >> 8;
			act = act->ape_next;
		}
	}
	length = p - event_buffer - length;
	*end++ = length;
	*end++ = length >> 8;
#endif /* EVENTS_WILDCARDING */

	delete_request(request);
	RELEASE;
	reinterpret_cast<void(*)(...)>(*ast)(arg, p - event_buffer, event_buffer);
	if (event_buffer != buffer)
	{
		gds__free(event_buffer);
	}
	ACQUIRE;
}


static void exit_handler(void *arg)
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
	STATUS local_status[20];

#ifndef NETWARE_386				/*  Otherwise things go away too quickly during unload */
	if (EVENT_process_offset) {
		if (EVENT_header->evh_current_process != EVENT_process_offset)
			ACQUIRE;
		delete_process(EVENT_process_offset);
		RELEASE;
	}

	while (acquire_count > 0)
		RELEASE;
#endif /*  !NETWARE  */

#ifndef SERVER
#ifdef SOLARIS_MT
	ISC_unmap_object(local_status, &EVENT_data, &EVENT_process,
					 sizeof(struct prb));
#endif
	ISC_unmap_file(local_status, &EVENT_data, 0);
#endif

	EVENT_header = NULL;
}


#ifdef EVENTS_WILDCARDING
static EVNT find_event(USHORT length,
					   TEXT * string,
					   EVNT parent,
					   BOOLEAN match,
					   EVNT start)
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
	EVNT event, prev_event;
	SRQ	*que, *prev_que;
	PTR	parent_offset;

	parent_offset = (parent) ? REL_PTR (parent) : 0;

	QUE_LOOP (EVENT_header->evh_events, que) {
		event = (EVNT) ((UCHAR*) que - OFFSET (EVNT, evnt_events));
		prev_que = (SRQ*) QUE_PREV ((*que));
		prev_event = (EVNT) ((UCHAR*) prev_que - OFFSET (EVNT, evnt_events));
		if (match && start && start != prev_event)
			continue;
		start = NULL;
		if (event->evnt_parent == parent_offset &&
			(match ? MATCHESNAME (string, length, event->evnt_name, event->evnt_length) :
			(event->evnt_length == length && !memcmp (string, event->evnt_name, length))))
		return event;
	}

	return NULL;
}
#else /* EVENTS_WILDCARDING */
static EVNT find_event(USHORT length, TEXT * string, EVNT parent)
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
	EVNT event;
	SRQ *que;
	PTR parent_offset;

	parent_offset = (parent) ? REL_PTR(parent) : 0;

	QUE_LOOP(EVENT_header->evh_events, que) {
		event = (EVNT) ((UCHAR *) que - OFFSET(EVNT, evnt_events));
		if (event->evnt_parent == parent_offset &&
			event->evnt_length == length &&
			!memcmp(string, event->evnt_name, length)) return event;
	}

	return NULL;
}
#endif /* EVENTS_WILDCARDING */


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
	PTR *ptr, offset;
	FRB free, prior;

	prior = NULL;
	offset = REL_PTR(block);
	block->frb_header.hdr_type = type_frb;

	for (ptr = &EVENT_header->evh_free; (free = (FRB) ABS_PTR(*ptr)) && *ptr;
		 prior = free, ptr = &free->frb_next)
		if ((SCHAR * HUGE_PTR) block < (SCHAR * HUGE_PTR) free)
			break;

	if (offset <= 0 || offset > EVENT_header->evh_length ||
		(prior
		 && (UCHAR HUGE_PTR *) block <
		 (UCHAR HUGE_PTR *) prior + prior->frb_header.hdr_length)) {
		punt("free_global: bad block");
		return;
	}

/* Start by linking block into chain */

	block->frb_next = *ptr;
	*ptr = offset;

/* Try to merge free block with next block */

	if (free
		&& (SCHAR HUGE_PTR *) block + block->frb_header.hdr_length ==
		(SCHAR HUGE_PTR *) free) {
		block->frb_header.hdr_length += free->frb_header.hdr_length;
		block->frb_next = free->frb_next;
	}

/* Next, try to merge the free block with the prior block */

	if (prior
		&& (SCHAR HUGE_PTR *) prior + prior->frb_header.hdr_length ==
		(SCHAR HUGE_PTR *) block) {
		prior->frb_header.hdr_length += block->frb_header.hdr_length;
		prior->frb_next = block->frb_next;
	}
}


static RINT historical_interest(SES session, PTR event)
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
	PTR ptr;
	RINT interest;

	for (ptr = session->ses_interests;
		 ptr && (interest = (RINT) ABS_PTR(ptr)); ptr = interest->rint_next)
		if (interest->rint_event == event)
			return interest;

	return NULL;
}


static void init(void *arg, SH_MEM shmem_data, int initialize)
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
	FRB free;
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
	QUE_INIT(EVENT_header->evh_processes);
	QUE_INIT(EVENT_header->evh_events);

#ifndef SERVER
#if !defined(WIN_NT)
	if (mutex_state = ISC_mutex_init(MUTEX, shmem_data->sh_mem_mutex_arg))
		mutex_bugcheck("mutex init", mutex_state);
#endif
#endif

	free = (FRB) ((UCHAR *) EVENT_header + sizeof(struct evh));
	free->frb_header.hdr_length =
		EVENT_data.sh_mem_length_mapped - sizeof(struct evh);
	free->frb_header.hdr_type = type_frb;
	free->frb_next = 0;

	EVENT_header->evh_free = (UCHAR *) free - (UCHAR *) EVENT_header;
}


static void insert_tail(SRQ * que, SRQ * node)
{
/**************************************
 *
 *	i n s e r t _ t a i l
 *
 **************************************
 *
 * Functional description
 *	Insert a node at the tail of a que.
 *
 **************************************/
	SRQ *prior;

	node->srq_forward = REL_PTR(que);
	node->srq_backward = que->srq_backward;

	prior = (SRQ *) ABS_PTR(que->srq_backward);
	prior->srq_forward = REL_PTR(node);
	que->srq_backward = REL_PTR(node);
}


static EVNT make_event(USHORT length, TEXT * string, SLONG parent_offset)
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
	EVNT event, parent;

	event =
		(EVNT) alloc_global(type_evnt, (SLONG) (sizeof(struct evnt) + length),
							FALSE);
	insert_tail(&EVENT_header->evh_events, &event->evnt_events);
	QUE_INIT(event->evnt_interests);

	if (parent_offset) {
		event->evnt_parent = parent_offset;
		parent = (EVNT) ABS_PTR(parent_offset);
		++parent->evnt_count;
	}

	event->evnt_length = length;
	memcpy(event->evnt_name, string, length);

	return event;
}


static void mutex_bugcheck(TEXT * string, int mutex_state)
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

#ifdef NETWARE_386
	ConsolePrintf("%s\n", msg);
#else
	ib_fprintf(ib_stderr, "%s\n", msg);
#endif

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
	RELEASE;

#ifdef SERVER
	deliver();
#else

#ifdef MULTI_THREAD
	ISC_event_post(process->prb_event);
#else

#ifdef UNIX
	if (REL_PTR(process) != EVENT_process_offset)
		ISC_kill(process->prb_process_id, EVENT_SIGNAL);
#endif

#endif
	ACQUIRE;
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
	SRQ *que;
	PRB process;
	SLONG process_offset;

	QUE_LOOP(EVENT_header->evh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		process_offset = REL_PTR(process);
		if (process_offset != EVENT_process_offset &&
			!ISC_check_process_existence(process->prb_process_id,
										 process->prb_process_uid[0], FALSE)) {
			que = (SRQ *) ABS_PTR(que->srq_backward);
			delete_process(process_offset);
		}
	}
}


static void punt(TEXT * string)
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

	ib_printf("(EVENT) punt: global region corrupt -- %s\n", string);
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

#ifdef EVENT_DEBUG
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
			PRB process = (PRB) ABS_PTR(EVENT_process_offset);
			if (process->prb_flags & PRB_pending)
				ISC_kill(process->prb_process_id, EVENT_SIGNAL);
		}
#endif
	}
#endif
}


static void remove_que(SRQ * node)
{
/**************************************
 *
 *	r e m o v e _ q u e
 *
 **************************************
 *
 * Functional description
 *	Remove a node from a self-relative que.
 *
 **************************************/
	SRQ *que;

	que = (SRQ *) ABS_PTR(node->srq_forward);
	que->srq_backward = node->srq_backward;

	que = (SRQ *) ABS_PTR(node->srq_backward);
	que->srq_forward = node->srq_forward;
	node->srq_forward = node->srq_backward = 0;
}


static BOOLEAN request_completed(REQ request)
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
	PTR next;
	RINT interest;
	EVNT event;

	for (next = request->req_interests; next; next = interest->rint_next) {
		interest = (RINT) ABS_PTR(next);
		event = (EVNT) ABS_PTR(interest->rint_event);
		if (interest->rint_count <= event->evnt_count)
			return TRUE;
	}

	return FALSE;
}


static STATUS return_ok(STATUS * status_vector)
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

	*status_vector++ = gds_arg_gds;
	*status_vector++ = 0;
	*status_vector = gds_arg_end;

	return 0;
}


#ifdef EVENT_DEBUG
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
	SLONG offset;
	HDR *block;
	PTR next_free;

/* Check consistency of global region (debugging only) */

	next_free = 0;

	for (offset = sizeof(struct evh); offset < EVENT_header->evh_length;
		 offset += block->hdr_length) {
		block = (HDR *) ABS_PTR(offset);
		if (!block->hdr_length || !block->hdr_type
			|| block->hdr_type >= type_max) {
			punt("bad block length or type");
			break;
		}
		if (next_free)
			if (offset == next_free)
				next_free = 0;
			else if (offset > next_free)
				punt("bad free chain");
		if (block->hdr_type == type_frb) {
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
static void THREAD_ROUTINE watcher_thread(void *dummy)
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
	PRB process;
	SLONG value;
	EVENT events;

	while (EVENT_process_offset) {
		ACQUIRE;
		process = (PRB) ABS_PTR(EVENT_process_offset);
		process->prb_flags &= ~PRB_wakeup;

		if (process->prb_flags & PRB_exiting) {
			process->prb_flags &= ~PRB_exiting;
			ISC_event_post(process->prb_event);
			RELEASE;
			break;
		}
#ifdef WIN_NT
		if (process->prb_flags & PRB_remap) {
			process->prb_flags |= PRB_remap_over;
			process->prb_flags &= ~PRB_remap;
			RELEASE;
			while (1) {
				Sleep(3);
				ACQUIRE;
				process = (PRB) ABS_PTR(EVENT_process_offset);
				RELEASE;
				if (!(process->prb_flags & PRB_remap_over))
					break;
			}
			ACQUIRE;
			process = (PRB) ABS_PTR(EVENT_process_offset);
		}
#endif

		value = ISC_event_clear(process->prb_event);
		RELEASE;
		deliver();
		ACQUIRE;
		process = (PRB) ABS_PTR(EVENT_process_offset);
		RELEASE;
#ifdef SOLARIS_MT
		events = EVENT_process->prb_event;
#else
		events = process->prb_event;
#endif
		ISC_event_wait(1, &events, &value, 0, 0, 0);
	}

}
#endif


} // extern "C"
