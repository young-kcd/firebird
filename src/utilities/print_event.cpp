/*
 *	PROGRAM:	Event manager
 *	MODULE:		print_event.cpp
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

static void prt_que(UCHAR *, srq *);
static void event_list(void);
static void event_dump_list(void);

static EVH EVENT_header = NULL;

static const UCHAR* SRQ_BASE	= (UCHAR *) EVENT_header;

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
		fprintf(stderr, "Can't access global event region\n");
		isc_print_status(status_vector);
		return 1;
	}

	if ((argc == 2) && (strncmp(argv[1], "-d", 2) == 0))
		event_table_dump();
	else if (argc == 1)
		event_list();
	else
		fprintf(stderr, "usage: eventprint [-dump]\n");

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
	srq *que_inst;
	EVNT event;
	srq *database_que;
	EVNT database_event;

	SRQ_LOOP(EVENT_header->evh_events, database_que) {
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

		printf("Database: ");
		p = (UCHAR *) database_event->evnt_name;
		l = database_event->evnt_length;
		while (l--)
			printf("%02x", *p++);
		printf(" count: %6ld\n", database_event->evnt_count);

		{
			RINT interest;
			srq *interest_que;
			/* Print out the interest list for this event */

			SRQ_LOOP(database_event->evnt_interests, interest_que) {
				interest =
					(RINT) ((UCHAR *) interest_que -
							OFFSET(RINT, rint_interests));
				if (!interest->rint_request)
					printf("(0)");
				else {
					jrd_req* request;
					PRB process;

					request = (jrd_req*) SRQ_ABS_PTR(interest->rint_request);
					process = (PRB) SRQ_ABS_PTR(request->req_process);
					printf("%6d ", process->prb_process_id);
				}
			}
		}

		/* Print out each event belonging to this database */

		SRQ_LOOP(EVENT_header->evh_events, que_inst) {

			event = (EVNT) ((UCHAR *) que_inst - OFFSET(EVNT, evnt_events));
			fb_assert(event->evnt_header.hdr_type == type_evnt);
			if (event->evnt_parent != SRQ_REL_PTR(database_event))
				continue;
			printf("    \"%-15s\" count: %6ld Interest",
					  event->evnt_name, event->evnt_count);

			{ // scope
				RINT interest;
				srq *interest_que;
				/* Print out the interest list for this event */

				SRQ_LOOP(event->evnt_interests, interest_que) {
					interest =
						(RINT) ((UCHAR *) interest_que -
								OFFSET(RINT, rint_interests));
					if (!interest->rint_request)
						printf("(0)");
					else {
						jrd_req* request;
						PRB process;

						request = (jrd_req*) SRQ_ABS_PTR(interest->rint_request);
						process = (PRB) SRQ_ABS_PTR(request->req_process);
						printf("%6d ", process->prb_process_id);
					}
				}
			} // scope
			printf("\n");
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
	srq *que_inst;
	PRB process;
	FRB free;
	EVNT event, parent;
	jrd_req* request;
	SES session;
	RINT interest;
	SLONG offset;

	printf("%.5d GLOBAL REGION HEADER\n", 0);
	printf
		("\tLength: %ld, version: %d, free: %ld, current: %ld, request_id: %ld\n",
		 EVENT_header->evh_length, EVENT_header->evh_version,
		 EVENT_header->evh_free, EVENT_header->evh_current_process,
		 EVENT_header->evh_request_id);

	prt_que("\tProcesses", &EVENT_header->evh_processes);
	prt_que("\tEvents", &EVENT_header->evh_events);

	for (offset = sizeof(evh); offset < EVENT_header->evh_length;
		 offset += block->hdr_length) 
	{
		printf("\n%.5ld ", offset);
		event_hdr* block = (event_hdr*) SRQ_ABS_PTR(offset);
		switch (block->hdr_type) {
		case type_prb:
			printf("PROCESS_BLOCK (%ld)\n", block->hdr_length);
			process = (PRB) block;
			printf("\tFlags: %d, pid: %d\n",
					  process->prb_flags, process->prb_process_id);
			prt_que("\tProcesses", &process->prb_processes);
			prt_que("\tSessions", &process->prb_sessions);
			break;

		case type_frb:
			printf("FREE BLOCK (%ld)\n", block->hdr_length);
			free = (FRB) block;
			printf("\tNext: %ld\n", free->frb_next);
			break;

		case type_req:
			printf("jrd_req* (%ld)\n", block->hdr_length);
			request = (jrd_req*) block;
			printf("\tProcess: %ld, interests: %ld, ast: %lx, arg: %lx\n",
					  request->req_process, request->req_interests,
					  request->req_ast, request->req_ast_arg);
			printf("\tRequest id: %ld\n", request->req_request_id);
			prt_que("\tRequests", &request->req_requests);
			break;

		case type_evnt:
			printf("EVENT (%ld)\n", block->hdr_length);
			event = (EVNT) block;
			printf("\t\"%s\", count: %ld, parent: %ld\n",
					  event->evnt_name, event->evnt_count,
					  event->evnt_parent);
			prt_que("\tInterests", &event->evnt_interests);
			prt_que("\tEvents", &event->evnt_events);
			break;

		case type_ses:
			printf("SESSION (%ld)\n", block->hdr_length);
			session = (SES) block;
			printf("\tInterests: %ld, process: %ld\n",
					  session->ses_interests, session->ses_process);
			prt_que("\tSessions", &session->ses_sessions);
			prt_que("\tRequests", &session->ses_requests);
			break;

		case type_rint:
			printf("INTEREST (%ld)\n", block->hdr_length);
			interest = (RINT) block;
			if (interest->rint_event) {
				event = (EVNT) SRQ_ABS_PTR(interest->rint_event);
				if (event->evnt_parent) {
					parent = (EVNT) SRQ_ABS_PTR(event->evnt_parent);
					printf("\t\"%s\".\"%s\"\n", parent->evnt_name,
							  event->evnt_name);
				}
				else
					printf("\t\"%s\"\n", event->evnt_name);
			}
			printf("\tEvent: %ld, request: %ld, next: %ld, count: %ld\n",
					  interest->rint_event, interest->rint_request,
					  interest->rint_next, interest->rint_count);
			prt_que("\tInterests", &interest->rint_interests);
			break;

		default:
			printf("*** UNKNOWN *** (%ld)\n", block->hdr_length);
			break;
		}
		if (!block->hdr_length)
			break;
	}

}


static void prt_que(UCHAR * string, srq * que_inst)
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

	offset = SRQ_REL_PTR(que_inst);

	if (offset == que_inst->srq_forward && offset == que_inst->srq_backward)
		printf("%s: *empty*\n", string);
	else
		printf("%s: forward: %d, backward: %d\n",
				  string, que_inst->srq_forward, que_inst->srq_backward);
}
