/*
 *	PROGRAM:	Event manager
 *	MODULE:		print.c
 *	DESCRIPTION:	Global region print utility
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
#include "../jrd/event.h"
#include "../jrd/event_proto.h"
#include "../jrd/gds_proto.h"

static void prt_que(UCHAR *, SRQ *);
static void event_list(void);
static void event_dump_list(void);

static EVH EVENT_header = NULL;


int main(int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	This is a simple utility program to print out the current
 *	contents of the InterBase event table file.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	if (!(EVENT_header = EVENT_init(status_vector, TRUE))) {
		ib_fprintf(ib_stderr, "Can't access global event region\n");
		isc_print_status(status_vector);
		return 1;
	}

	if ((argc == 2) && (strncmp(argv[1], "-d", 2) == 0))
		event_table_dump();
	else if (argc == 1)
		event_list();
	else
		ib_fprintf(ib_stderr, "usage: eventprint [-dump]\n");

	return 0;
}


static void event_list(void)
{
/**************************************
 *
 *	e v e n t _ l i s t 
 *
 **************************************
 *
 * Functional description
 *	List active events in the event table.
 *	This format is more readable for humans.
 *
 **************************************/
	SRQ *que;
	EVNT event;
	SRQ *database_que;
	EVNT database_event;

	QUE_LOOP(EVENT_header->evh_events, database_que) {
		UCHAR *p;
		ULONG l;

		database_event =
			(EVNT) ((UCHAR *) database_que - OFFSET(EVNT, evnt_events));

		/* Skip non-database entries */

		if (database_event->evnt_parent)
			continue;

		/* Print out the magic name for the database, this name 
		   comes from the lock key_id for the database, on Unix
		   this is comprised of the device number and inode */

		ib_printf("Database: ");
		p = (UCHAR *) database_event->evnt_name;
		l = database_event->evnt_length;
		while (l--)
			ib_printf("%02x", *p++);
		ib_printf(" count: %6ld\n", database_event->evnt_count);

		{
			RINT interest;
			SRQ *interest_que;
			/* Print out the interest list for this event */

			QUE_LOOP(database_event->evnt_interests, interest_que) {
				interest =
					(RINT) ((UCHAR *) interest_que -
							OFFSET(RINT, rint_interests));
				if (!interest->rint_request)
					ib_printf("(0)");
				else {
					JRD_REQ request;
					PRB process;

					request = (JRD_REQ) ABS_PTR(interest->rint_request);
					process = (PRB) ABS_PTR(request->req_process);
					ib_printf("%6d ", process->prb_process_id);
				}
			}
		}

		/* Print out each event belonging to this database */

		QUE_LOOP(EVENT_header->evh_events, que) {

			event = (EVNT) ((UCHAR *) que - OFFSET(EVNT, evnt_events));
			fb_assert(event->evnt_header.hdr_type == type_evnt);
			if (event->evnt_parent != REL_PTR(database_event))
				continue;
			ib_printf("    \"%-15s\" count: %6ld Interest",
					  event->evnt_name, event->evnt_count);

			{
				RINT interest;
				SRQ *interest_que;
				/* Print out the interest list for this event */

				QUE_LOOP(event->evnt_interests, interest_que) {
					interest =
						(RINT) ((UCHAR *) interest_que -
								OFFSET(RINT, rint_interests));
					if (!interest->rint_request)
						ib_printf("(0)");
					else {
						JRD_REQ request;
						PRB process;

						request = (JRD_REQ) ABS_PTR(interest->rint_request);
						process = (PRB) ABS_PTR(request->req_process);
						ib_printf("%6d ", process->prb_process_id);
					}
				}
			}
			ib_printf("\n");
		}
	}
}


static void event_table_dump(void)
{
/**************************************
 *
 *	e v e n t _ t a b l e _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump the contents of the event table.
 *	This format exactly matches what's in the event table and
 *	is not very readable by humans.
 *
 **************************************/
	SRQ *que;
	HDR *block;
	PRB process;
	FRB free;
	EVNT event, parent;
	JRD_REQ request;
	SES session;
	RINT interest;
	SLONG offset;

	ib_printf("%.5d GLOBAL REGION HEADER\n", 0);
	ib_printf
		("\tLength: %ld, version: %d, free: %ld, current: %ld, request_id: %ld\n",
		 EVENT_header->evh_length, EVENT_header->evh_version,
		 EVENT_header->evh_free, EVENT_header->evh_current_process,
		 EVENT_header->evh_request_id);

	prt_que("\tProcesses", &EVENT_header->evh_processes);
	prt_que("\tEvents", &EVENT_header->evh_events);

	for (offset = sizeof(evh); offset < EVENT_header->evh_length;
		 offset += block->hdr_length) {
		ib_printf("\n%.5ld ", offset);
		block = (HDR *) ABS_PTR(offset);
		switch (block->hdr_type) {
		case type_prb:
			ib_printf("PROCESS_BLOCK (%ld)\n", block->hdr_length);
			process = (PRB) block;
			ib_printf("\tFlags: %d, pid: %d\n",
					  process->prb_flags, process->prb_process_id);
			prt_que("\tProcesses", &process->prb_processes);
			prt_que("\tSessions", &process->prb_sessions);
			break;

		case type_frb:
			ib_printf("FREE BLOCK (%ld)\n", block->hdr_length);
			free = (FRB) block;
			ib_printf("\tNext: %ld\n", free->frb_next);
			break;

		case type_req:
			ib_printf("JRD_REQ (%ld)\n", block->hdr_length);
			request = (JRD_REQ) block;
			ib_printf("\tProcess: %ld, interests: %ld, ast: %lx, arg: %lx\n",
					  request->req_process, request->req_interests,
					  request->req_ast, request->req_ast_arg);
			ib_printf("\tRequest id: %ld\n", request->req_request_id);
			prt_que("\tRequests", &request->req_requests);
			break;

		case type_evnt:
			ib_printf("EVENT (%ld)\n", block->hdr_length);
			event = (EVNT) block;
			ib_printf("\t\"%s\", count: %ld, parent: %ld\n",
					  event->evnt_name, event->evnt_count,
					  event->evnt_parent);
			prt_que("\tInterests", &event->evnt_interests);
			prt_que("\tEvents", &event->evnt_events);
			break;

		case type_ses:
			ib_printf("SESSION (%ld)\n", block->hdr_length);
			session = (SES) block;
			ib_printf("\tInterests: %ld, process: %ld\n",
					  session->ses_interests, session->ses_process);
			prt_que("\tSessions", &session->ses_sessions);
			prt_que("\tRequests", &session->ses_requests);
			break;

		case type_rint:
			ib_printf("INTEREST (%ld)\n", block->hdr_length);
			interest = (RINT) block;
			if (interest->rint_event) {
				event = (EVNT) ABS_PTR(interest->rint_event);
				if (event->evnt_parent) {
					parent = (EVNT) ABS_PTR(event->evnt_parent);
					ib_printf("\t\"%s\".\"%s\"\n", parent->evnt_name,
							  event->evnt_name);
				}
				else
					ib_printf("\t\"%s\"\n", event->evnt_name);
			}
			ib_printf("\tEvent: %ld, request: %ld, next: %ld, count: %ld\n",
					  interest->rint_event, interest->rint_request,
					  interest->rint_next, interest->rint_count);
			prt_que("\tInterests", &interest->rint_interests);
			break;

		default:
			ib_printf("*** UNKNOWN *** (%ld)\n", block->hdr_length);
			break;
		}
		if (!block->hdr_length)
			break;
	}

}


static void prt_que(UCHAR * string, SRQ * que)
{
/**************************************
 *
 *	p r t _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print the contents of a self-relative que.
 *
 **************************************/
	SLONG offset;

	offset = REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward)
		ib_printf("%s: *empty*\n", string);
	else
		ib_printf("%s: forward: %d, backward: %d\n",
				  string, que->srq_forward, que->srq_backward);
}
