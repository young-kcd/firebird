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
#include "../jrd/thd.h"
#include "../jrd/file_params.h"
#include "../jrd/que.h"

const int SIZE_SHIFT	= 2;
const int FAST_ALLOC	= 16;

/* Global section header */

const int EVH_HASH_SIZE	= 7;

struct evh {
	SLONG evh_length;			/* Current length of global section */
	UCHAR evh_version;			/* Version number of global section */
	srq evh_events;				/* Known processes */
	srq evh_processes;			/* Known processes */
	SRQ_PTR evh_free;				/* Free blocks */
	SRQ_PTR evh_current_process;	/* Current process, if any */
	MTX_T evh_mutex[1];			/* Mutex controlling access */
	SLONG evh_request_id;		/* Next request id */
	SRQ_PTR evh_hash_table[EVH_HASH_SIZE];
};

typedef evh *EVH;

/* Common block header */

const int type_hdr	= 1;		// Event header
const int type_frb	= 2;		// Free block
const int type_prb	= 3;		// Process block
const int type_rint	= 4;		// Request interest block
const int type_reqb	= 5;		// Request block previously req_type also used in blk.h
const int type_evnt	= 6;		// Event 
const int type_ses	= 7;		// Session
const int type_max	= 8;

struct event_hdr // CVC: previous clash with ods.h's hdr
{
	SLONG hdr_length;			/* Length of block */
	UCHAR hdr_type;				/* Type of block */
};

/* Free blocks */

struct frb
{
	event_hdr frb_header;
	SLONG frb_next;				/* Next block */
};
typedef frb *FRB;

/* Process blocks */

struct prb
{
	event_hdr prb_header;
	srq prb_processes;			/* Process que owned by header */
	srq prb_sessions;			/* Sessions within process */
	SLONG prb_process_id;		/* Process id */
	SLONG prb_process_uid[2];	/* Process UID (apollo) */
	event_t prb_event[1];		/* Event on which to wait */
	USHORT prb_flags;
};
typedef prb *PRB;

const int PRB_wakeup	= 1;		/* Schedule a wakeup for process */
const int PRB_pending	= 2;		/* Wakeup has been requested, and is dangling */
const int PRB_remap		= 4;		/* need to remap shared memory */
const int PRB_remap_over= 8;		/* remap is over */
const int PRB_exiting	= 16;		/* Process is exiting */

/* Session block */

struct ses {
	event_hdr ses_header;
	srq ses_sessions;			/* Sessions within process */
	srq ses_requests;			/* Outstanding requests */
	SRQ_PTR ses_interests;			/* Historical interests */
	SRQ_PTR ses_process;			/* Parent process */
#ifdef MULTI_THREAD
	USHORT ses_flags;
#endif
};
typedef ses *SES;

const int SES_delivering	= 1;		/* Watcher thread is delivering an event */

/* Event block */

struct evnt {
	event_hdr evnt_header;
	srq evnt_events;			/* System event que (owned by header) */
	srq evnt_interests;			/* Que of request interests in event */
	SRQ_PTR evnt_hash_collision;	/* Hash table collision pointer */
	SRQ_PTR evnt_parent;			/* Major event name */
	SLONG evnt_count;			/* Current event count */
	USHORT evnt_length;			/* Length of event name */
	TEXT evnt_name[1];			/* Event name */
};
typedef evnt *EVNT;

/* Request block */

struct evt_req {
	event_hdr req_header;
	srq req_requests;			/* Request que owned by session block */
	SRQ_PTR req_process;			/* Parent process block */
	SRQ_PTR req_session;			/* Parent session block */
	SRQ_PTR req_interests;			/* First interest in request */
	FPTR_EVENT_CALLBACK req_ast;	/* Asynchronous routine */
	void *req_ast_arg;			/* Argument for ast */
	SLONG req_request_id;		/* Request id, dummy */
};
typedef evt_req *EVT_REQ;

/* Request interest block */

struct req_int {
	event_hdr rint_header;
	srq rint_interests;			/* Que owned by event */
	SRQ_PTR rint_event;				/* Event of interest */
	SRQ_PTR rint_request;			/* Request of interest */
	SRQ_PTR rint_next;				/* Next interest of request */
	SLONG rint_count;			/* Threshhold count */
};
typedef req_int *RINT;

const int EPB_version1 = 1;

#endif // JRD_EVENT_H

