/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		event.h
 *	DESCRIPTION:	Event manager definitions
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
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old Win3.1 port (Windows_Only)
 *
 */

#ifndef JRD_EVENT_H
#define JRD_EVENT_H

#include "../jrd/isc.h"
#include "../jrd/file_params.h"

#define PTR			SLONG
#define BASE                  ((UCHAR *) EVENT_header)
#define REL_PTR(item)         ((UCHAR *) item - BASE)
#define ABS_PTR(item)		(BASE + item)
#define ACQUIRE			acquire()
#define RELEASE			release()

#define QUE_INIT(que)	{que.srq_forward = que.srq_backward = REL_PTR (&que);}
#define QUE_EMPTY(que)	(que.srq_forward == REL_PTR (&que))
#define QUE_NEXT(que)	ABS_PTR (que.srq_forward)
#define QUE_PREV(que)	ABS_PTR (que.srq_backward)

#define QUE_LOOP(header,que)	for (que = (SRQ*) QUE_NEXT (header);\
	que != &header; que = (SRQ*) QUE_NEXT ((*que)))


#define SIZE_SHIFT	2
#define FAST_ALLOC	16

/* Self-relative que block.  Offsets are from the block itself. */

typedef struct srq {
	PTR srq_forward;			/* Forward offset */
	PTR srq_backward;			/* Backward offset */
} SRQ;

/* Global section header */

#define EVH_HASH_SIZE	7

typedef struct evh {
	SLONG evh_length;			/* Current length of global section */
	UCHAR evh_version;			/* Version number of global section */
	SRQ evh_events;				/* Known processes */
	SRQ evh_processes;			/* Known processes */
	PTR evh_free;				/* Free blocks */
	PTR evh_current_process;	/* Current process, if any */
	MTX_T evh_mutex[1];			/* Mutex controlling access */
	SLONG evh_request_id;		/* Next request id */
	PTR evh_hash_table[EVH_HASH_SIZE];
} *EVH;

/* Common block header */

#define type_hdr	1			/* Event header */
#define type_frb	2			/* Free block */
#define type_prb	3			/* Process block */
#define type_rint	4			/* Request interest block */
#define type_req	5			/* Request block */
#define type_evnt	6			/* Event */
#define type_ses	7			/* Session */
#define type_max	8

typedef struct hdr
{
	SLONG hdr_length;			/* Length of block */
	UCHAR hdr_type;				/* Type of block */
} HDR;

/* Free blocks */

typedef struct frb
{
	HDR frb_header;
	SLONG frb_next;				/* Next block */
} *FRB;

/* Process blocks */

typedef struct prb
{
	HDR prb_header;
	SRQ prb_processes;			/* Process que owned by header */
	SRQ prb_sessions;			/* Sessions within process */
	SLONG prb_process_id;		/* Process id */
	SLONG prb_process_uid[2];	/* Process UID (apollo) */
	EVENT_T prb_event[1];		/* Event on which to wait */
	USHORT prb_flags;
} *PRB;

#define PRB_wakeup	1			/* Schedule a wakeup for process */
#define PRB_pending	2			/* Wakeup has been requested, and is dangling */
#define PRB_remap	4			/* need to remap shared memory */
#define	PRB_remap_over	8		/* remap is over */
#define PRB_exiting	16			/* Process is exiting */

/* Session block */

typedef struct ses {
	HDR ses_header;
	SRQ ses_sessions;			/* Sessions within process */
	SRQ ses_requests;			/* Outstanding requests */
	PTR ses_interests;			/* Historical interests */
	PTR ses_process;			/* Parent process */
#ifdef MULTI_THREAD
	USHORT ses_flags;
#endif
} *SES;

#define	SES_delivering	1		/* Watcher thread is delivering an event */

/* Event block */

typedef struct evnt {
	HDR evnt_header;
	SRQ evnt_events;			/* System event que (owned by header) */
	SRQ evnt_interests;			/* Que of request interests in event */
	PTR evnt_hash_collision;	/* Hash table collision pointer */
	PTR evnt_parent;			/* Major event name */
	SLONG evnt_count;			/* Current event count */
	USHORT evnt_length;			/* Length of event name */
	TEXT evnt_name[1];			/* Event name */
} *EVNT;

/* Request block */

typedef struct evt_req {
	HDR req_header;
	SRQ req_requests;			/* Request que owned by session block */
	PTR req_process;			/* Parent process block */
	PTR req_session;			/* Parent session block */
	PTR req_interests;			/* First interest in request */
	void (*req_ast) ();			/* Asynchronous routine */
	void *req_ast_arg;			/* Argument for ast */
	SLONG req_request_id;		/* Request id, dummy */
} *EVT_REQ;

/* Request interest block */

typedef struct rint {
	HDR rint_header;
	SRQ rint_interests;			/* Que owned by event */
	PTR rint_event;				/* Event of interest */
	PTR rint_request;			/* Request of interest */
	PTR rint_next;				/* Next interest of request */
	SLONG rint_count;			/* Threshhold count */
} *RINT;

#define EPB_version1 1

#endif /* JRD_EVENT_H */
