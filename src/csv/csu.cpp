/*
 *	PROGRAM:	Central Server
 *	MODULE:		csu.c
 *	DESCRIPTION:	Central Server Utility
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
/*
$Id: csu.cpp,v 1.3 2001-12-24 02:50:48 tamlin Exp $
*/

#include "firebird.h"
#include "../csv/csi.h"
#include "../jrd/license.h"
#include "gen/codes.h"
#include "../jrd/thd.h"
#include "../csv/css_proto.h"
#include "../jrd/gds_proto.h"

#define ALLOC(type, length)	CSS_alloc_local (type, length)
#define FREE(block)	CSS_free_local (block)

static void move(UCHAR *, UCHAR *, USHORT);
static void parse_command_line(int, TEXT **, USHORT *, CSN *, CSN *);
static void process_list_cmd(PTR, USHORT *);
static void process_nonlist_cmd(PTR, CSN);
static STATUS process_response(MSG_RESP, STATUS *, USHORT *);

static TEXT error_buffer[1024];
static USHORT return_buffer_length;
static UCHAR *return_buffer;


int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Run the central server utility.
 *
 **************************************/
	USHORT sw_list, listed_flag, flags;
	CSN csv_enable_names, csv_other_names, cs_name;
	STATUS status_vector[20];
	SRQ *que;
	PRB process;
	PTR connection, server, client;
	CSH CSS_header;
	SLONG process_number, process_numbers[256], *ptr, *end;

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

	csv_enable_names = csv_other_names = NULL;
	sw_list = FALSE;

	THREAD_ENTER;
	parse_command_line(argc, argv, &sw_list, &csv_enable_names,
					   &csv_other_names);

	if (!CSS_init(status_vector, FALSE)) {
		printf("It's likely that no central servers are operating.\n");
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	client = CSS_create_process(PRB_client);

/* Start by finding all server processes */

	CSS_header = ACQUIRE;
	CSS_probe_processes();

	end = process_numbers;

	QUE_LOOP(CSS_header->csh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		server = REL_PTR(process);
		if (client != server &&
			(process->prb_flags & PRB_server) &&
			process->prb_protocol_version == CSI_PROTOCOL_VERSION)
			*end++ = process->prb_process_number;
	}

	RELEASE;

/* Go back and talk to servers */

/* Initialize a flag that indicates if anything has been listed */

	listed_flag = FALSE;

	for (ptr = process_numbers; ptr < end; ptr++) {
		process_number = *ptr;
		if (server = CSS_find_process(process_number)) {
			connection = CSS_connect(server);

			/* For each server name specified by the user, ask this
			   server to process it. */

			for (cs_name = csv_enable_names;
				 cs_name && CSS_find_process(process_number);
				 cs_name =
				 cs_name->csn_next) process_nonlist_cmd(connection, cs_name);

			/* For each database name specified by the user, ask this
			   server to process it. */

			for (cs_name = csv_other_names;
				 cs_name && CSS_find_process(process_number);
				 cs_name = cs_name->csn_next) if (cs_name->csn_flags)
					process_nonlist_cmd(connection, cs_name);

			/* We may also need to do a list of this server's databases */

			if (sw_list && CSS_find_process(process_number))
				process_list_cmd(connection, &listed_flag);

			CSS_disconnect(connection);
		}
	}

	if (sw_list && !listed_flag)
		printf("\nNo central servers are operating.\n\n");

	for (cs_name = csv_other_names; cs_name; cs_name = cs_name->csn_next)
		if ((flags = cs_name->csn_flags) && !(flags & DBN_cmd_process)) {
			printf("Database \"%s\" ", cs_name->csn_db_name);
			if (cs_name->csn_cs_len)
				printf("running on server\n   \"%s\" could not be ",
					   cs_name->csn_cs_name);
			else
				printf("could not be\n   ");
			printf("found and so it was not %sed.\n",
				   (flags & DBN_reenable) ? "re-enabl" : (flags & DBN_kill) ?
				   "kill" : "disabl");
		}

	for (cs_name = csv_enable_names; cs_name; cs_name = cs_name->csn_next)
		if (!(cs_name->csn_flags & DBN_cmd_process))
			printf
				("Server \"%s\" could not be found and so database\n   \"%s\" was not enabled.\n",
				 cs_name->csn_cs_name, cs_name->csn_db_name);

	exit(FINI_OK);
}


static void move( UCHAR * from_ptr, UCHAR * to_ptr, USHORT length)
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move some bytes.
 *
 **************************************/
	USHORT l;
	UCHAR *from, *to;

	from = from_ptr;
	to = to_ptr;

	if (length)
		do
			*to++ = *from++;
		while (--length);
}


static void parse_command_line(
							   int argc,
							   TEXT ** argv,
							   USHORT * sw_list,
							   CSN * csv_enable_names, CSN * csv_other_names)
{
/**************************************
 *
 *	p a r s e _ c o m m a n d _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Parse a Unix style command line.
 *
 **************************************/
	USHORT sw_enable, sw_name, sw_all, dbn_flag, all_flag;
	USHORT length, listed_flag, csv_name_len;
	TEXT **end, *p, c, expanded_name[256], *q, csv_name[256];
	CSN other_names, enable_names, name, *head;

	other_names = enable_names = NULL;
	dbn_flag = all_flag = sw_enable = 0;
	sw_name = FALSE;

	for (end = argv++ + argc; argv < end;) {
		p = *argv++;
		if (*p++ == '-') {
			/* Clear -a, -e, and -n indicators for each new switch.  Clear -d,
			   -k, and -r indicators except when the switch is "-a" or "-n". */

			sw_name = sw_all = FALSE;
			if (((c = UPPER(*p)) != 'A' && c != 'N') || *(p + 1) || sw_enable)
				dbn_flag = 0;
			sw_enable = csv_name_len = 0;

			while (c = *p++)
				switch (UPPER(c)) {
				case 'L':
					*sw_list = TRUE;
					break;

				case 'D':
					dbn_flag |= DBN_disable;
					break;

				case 'K':
					dbn_flag |= DBN_kill;
					break;

				case 'R':
					dbn_flag |= DBN_reenable;
					break;

				case 'E':
					sw_enable = 1;
					break;

				case 'A':
					sw_all = TRUE;
					break;

				case 'N':
					sw_name = TRUE;
					break;

				case 'Z':
					printf("csu version %s\n", GDS_VERSION);
					break;
				}

			if (sw_enable && (dbn_flag || sw_all || sw_name)) {
				printf
					("The -e switch may only be used in conjunction with\nthe -l and -z switches.\n");
				exit(FINI_ERROR);
			}
			else if (sw_all)
				if (!dbn_flag)
					printf
						("No command indicated with the -a switch. It will be ignored.\n");
				else {
					all_flag |= dbn_flag;
					dbn_flag = 0;
				}
		}
		else if (!dbn_flag && !sw_enable && !sw_name)
			printf
				("No command indicated for database \"%s\".\nThe name will be ignored.\n",
				 p - 1);
		else {
			if (sw_enable == 1 || sw_name) {
				if ((length = strlen(--p)) >= sizeof(csv_name))
					length = sizeof(csv_name) - 1;
				csv_name_len = length;
				for (q = csv_name; length--;) {
					c = *p++;
					*q++ = UPPER(c);
				}
				if (sw_name)
					sw_name = FALSE;
				else
					sw_enable++;
				continue;
			}

			length = ISC_expand_filename(p - 1, 0, expanded_name);

#ifdef VMS
			/* The next expansion is Gateway specific but we'll do it
			   for everyone.  Since '@' isn't valid in a VMS filename,
			   this is ok. */

			/* Look for an '@' after the first SCHARacter in expanded_name.
			   If one is found, try to expand the remainder of the string. */

			if (*(p = expanded_name) != '@')
				while (*p)
					if (*p++ == '@' && *p) {
						length =
							ISC_expand_logical(p,
											   length - (p - expanded_name),
											   p) + (p - expanded_name);
						break;
					}
#endif
			csv_name[csv_name_len] = 0;

			head = (sw_enable) ? &enable_names : &other_names;
			for (name = *head; name; name = name->csn_next)
				if (!strcmp(name->csn_db_name, expanded_name) &&
					!strcmp(name->csn_cs_name, csv_name))
					break;

			if (!name) {
				name =
					(CSN) ALLOC(type_csn,
								sizeof(struct csn) + length + csv_name_len);
				name->csn_next = *head;
				*head = name;
				name->csn_cs_name = name->csn_data;
				name->csn_db_name = name->csn_data + csv_name_len + 1;
				move(csv_name, name->csn_cs_name, csv_name_len + 1);
				move(expanded_name, name->csn_db_name, length + 1);
				name->csn_cs_len = csv_name_len;
				name->csn_db_len = length;
			}

			name->csn_flags |= (sw_enable) ? DBN_enable : dbn_flag;
		}
	}

	if (all_flag) {
		/* We're to do something to all central server databases.
		   But first, make sure we won't try to do the same thing
		   or something similar to a specific database.  Reenable
		   takes precedence over kill and disable.  Kill takes
		   precedence over disable. */

		if (all_flag & DBN_reenable)
			all_flag |= DBN_kill | DBN_disable;
		else if (all_flag & DBN_kill)
			all_flag |= DBN_disable;

		for (name = other_names; name; name = name->csn_next)
			name->csn_flags &= ~all_flag;

		/* The -a switch gets saved as a name with a zero length */

		name = (CSN) ALLOC(type_csn, sizeof(struct csn));
		name->csn_next = other_names;
		other_names = name;
		name->csn_cs_name = name->csn_db_name = name->csn_data;
		name->csn_cs_len = name->csn_db_len = 0;
		name->csn_flags = all_flag;
	}

	*csv_other_names = other_names;
	*csv_enable_names = enable_names;
}


static void process_list_cmd( PTR connection, USHORT * listed_flag)
{
/**************************************
 *
 *	p r o c e s s _ l i s t _ c m d
 *
 **************************************
 *
 * Functional description
 *	Process a list command for a server.
 *
 **************************************/
	MSG_UTIL message;
	MSG_RESP response;
	STATUS status_vector[20];
	USHORT l;
	CSU_LIST list_msg;

	message = (MSG_UTIL) CSS_alloc_message(type_msg, sizeof(struct msg_util));
	message->msg_util_header.msg_type = MSG_util_cmd;
	message->msg_util_cmd = UTIL_list;
	message->msg_util_csn_len = message->msg_util_dbn_len = 0;

	if (!CSS_put_message(connection, message, 0) ||
		!(response = CSS_get_message(connection, 0, 0)))
		return;

	if (!process_response(response, status_vector, 0)) {
		list_msg = (CSU_LIST) return_buffer;

		/* Print a header before the first name listed */

		if (!*listed_flag) {
			printf("\nServer\n\tAttaches Flags Database Name\n\n");
			*listed_flag = TRUE;
		}

		/* Print the server name */

		printf("%s\n", list_msg->csu_list_name);
		list_msg =
			(CSU_LIST) (list_msg->csu_list_name +
						FB_ALIGN(list_msg->csu_list_length + 1, 2));

		/* Loop over returned database name info */

		while (l = list_msg->csu_list_length) {
			printf("\t%7d%s %c %c  %s\n", list_msg->csu_list_attaches,
				   (list_msg->csu_list_flags & DBN_server_att) ? "+s" : "  ",
				   (list_msg->csu_list_flags & DBN_disable) ? 'D' : ' ',
				   (list_msg->csu_list_flags & DBN_kill) ? 'K' : ' ',
				   list_msg->csu_list_name);
			list_msg =
				(CSU_LIST) (list_msg->csu_list_name + FB_ALIGN(l + 1, 2));
		}
	}
	else if (status_vector[1] != gds__unavailable) {
	/**** DO SOMETHING MORE INTELLIGENT ****/
	}
}


static void process_nonlist_cmd( PTR connection, CSN cs_name)
{
/**************************************
 *
 *	p r o c e s s _ n o n l i s t _ c m d
 *
 **************************************
 *
 * Functional description
 *	Process a disable, enable, kill, or re-enable command
 *	for a server.
 *
 **************************************/
	SLONG length;
	MSG_UTIL message;
	MSG_RESP response;
	USHORT l, cmd;
	STATUS status_vector[20];

	l = cs_name->csn_cs_len + cs_name->csn_db_len;
	length = sizeof(struct msg_util) + l;
	message = (MSG_UTIL) CSS_alloc_message(type_msg, (int) length);
	message->msg_util_header.msg_type = MSG_util_cmd;
	if (cs_name->csn_flags & DBN_enable)
		cmd = UTIL_enable;
	else if (cs_name->csn_flags & DBN_reenable)
		cmd = UTIL_reenable;
	else if (cs_name->csn_flags & DBN_kill)
		cmd = UTIL_kill;
	else
		cmd = UTIL_disable;
	message->msg_util_cmd = cmd;
	message->msg_util_csn_len = cs_name->csn_cs_len;
	message->msg_util_dbn_len = cs_name->csn_db_len;
	move(cs_name->csn_data, message->msg_util_data, l + 2);

	if (!CSS_put_message(connection, message, 0) ||
		!(response = CSS_get_message(connection, 0, 0)))
		return;

	if (!process_response(response, status_vector, 0)) {
		/* Set a flag to indicate that a central server has processed
		   the command relating to this server and database name */

		cs_name->csn_flags |= DBN_cmd_process;
	}
	else if (status_vector[1] != gds__unavailable)
		if (cs_name->csn_flags & DBN_enable) {
			printf
				("Server \"%s\" could not enable database \"%s\" because:\n",
				 cs_name->csn_cs_name, cs_name->csn_db_name);
			gds__print_status(status_vector);
			cs_name->csn_flags |= DBN_cmd_process;
		}
		else {
	/**** DO SOMETHING MORE INTELLIGENT ****/
		}
}


static STATUS process_response(
							   MSG_RESP message,
							   STATUS * user_status, USHORT * return_length)
{
/**************************************
 *
 *	p r o c e s s _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Process a response packet from server.
 *
 **************************************/
	TEXT *p, *q;
	STATUS *status, *stuff, *end;
	USHORT l;

/* If there isn't a message, the server has disappeared */

	if (!message || message->msg_resp_header.msg_type != MSG_response) {
		status = user_status;
		*status++ = gds_arg_gds;
		*status++ = gds__random;
		*status++ = gds_arg_string;
		*status++ = (STATUS) "connection lost to central server";
		*status = gds_arg_end;
		return user_status[1];
	}

/* Well, at least we got something -- eat it up */

	p = error_buffer;

	for (status = user_status, end = status + 20, stuff =
		 message->msg_resp_status; (*status = *stuff++) && status < end;)
		switch (*status++) {
		case gds_arg_interpreted:
		case gds_arg_string:
			q = (TEXT *) message + *stuff++;
			*status++ = (STATUS) p;
			while (*p++ = *q++);
			break;

		case gds_arg_cstring:
			l = *status++ = *stuff++;
			q = (TEXT *) message + *stuff++;
			*status++ = (STATUS) p;
			if (l)
				do
					*p++ = *q++;
				while (--l);
			break;

		default:
			*status++ = *stuff++;
		}

	if (!user_status[1] || user_status[1] == gds__segment) {
		if (return_buffer_length < (l = message->msg_resp_length)) {
			if (return_buffer)
				FREE(return_buffer);
			return_buffer = (UCHAR *) ALLOC(0, l);
			return_buffer_length = l;
		}
		if (l)
			move(message->msg_resp_data, return_buffer, l);
		if (return_length)
			*return_length = l;
	}

	CSS_free_global(message);

	return user_status[1];
}
