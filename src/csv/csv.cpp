/*
 *	PROGRAM:	Central Server
 *	MODULE:		csv.c
 *	DESCRIPTION:	Central Server
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
$Id: csv.cpp,v 1.10 2003-02-10 13:28:10 eku Exp $
*/

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../csv/csi.h"
#include "../jrd/gds.h"
#include "../jrd/license.h"
#include "../csv/css_proto.h"
#include "../jrd/gds_proto.h"
#include "../remote/merge_proto.h"

#ifdef PIPE_SERVER
#ifdef VMS
#include "../jrd/lnmdef.h"
#include jpidef
#include descrip
#endif

#endif

#define ALLOC(type, length)	CSS_alloc_local (type, length)
#define FREE(block)		CSS_free_local (block)
#define EVENT_FLAG		65

static void allocate_statement(MSG_OP);
static void alt_connection(MSG_OP);
static void attach_database(MSG_ATT);
#ifndef PIPE_SERVER
static STATUS attach_for_servicing(DBN, STATUS *, USHORT);
#endif
static void cancel_events(MSG_OP);
static void check_if_done(DBN);
static void compile(MSG_OP);
static void ddl(MSG_DDL);
static void disable_or_kill(DBN, USHORT);
static void disconnect(PTR);
static void drop_database(MSG_OP);
static void end_blob(MSG_OP);
static void end_database(MSG_OP);
static void end_request(MSG_OP);
static void end_statement(MSG_OP);
static void end_transaction(MSG_OP);
static void execute_immediate(MSG_EXNOW);
static void execute_statement(MSG_SQLMSG);
static void fetch(MSG_SQLMSG);
static DBN find_dbname(TEXT *, USHORT, USHORT);
static void free_buffer(UCHAR *, USHORT);
static UCHAR *get_buffer(USHORT *);
static void get_segment(MSG_SEG);
static void get_slice(MSG_SLICE);
static void info(MSG_INFO);
static void insert(MSG_SQLMSG);
static RTR make_transaction(RDB, HANDLE);
static void move(UCHAR *, UCHAR *, USHORT);
static void multi_thread(void);
static void open_blob(MSG_BLOB);
static void ping(MSG_OP);
static void prepare_statement(MSG_PSTMT);
static void process_message(CSV_MSG);
static void put_segment(MSG_SEG);
static void put_slice(MSG_SLICE);
static void que_events(MSG_EVENT);
static void receive_msg(MSG_MSG);
static void reconnect(MSG_OP);
static void release_array(ARRAY);
static void release_blob(RBL);
static void release_database(RDB);
static void release_request(RRQ);
static void release_sql_request(RSR);
static void release_transaction(RTR);
static void seek_blob(MSG_SEEK);
static void send_msg(MSG_MSG);
static int send_response(CSV_MSG, STATUS *, SLONG, USHORT, UCHAR *);
static void server_ast(EVNT, USHORT, UCHAR *);
static void set_cursor(MSG_SETCUR);
static void start(MSG_MSG);
static void start_and_send(MSG_MSG);
static void start_transaction(MSG_TRANS);
static void server_utility(MSG_UTIL);
#ifdef MULTI_THREAD
static void thread(void);
#endif
static void transact_request(MSG_TRRQ);
#ifdef PIPE_SERVER
#ifdef VMS
static void trans_logicals(void);
#endif
#endif
static void unwind(MSG_OP);

static RDB CSV_databases;
static DBN CSV_dbnames;
static TEXT *CSV_name;
static USHORT CSV_name_len;
static UCHAR *blob_buffer;
static USHORT blob_buffer_length;
static USHORT sw_attach, sw_version, num_buffers, sw_shutdown;

static SLONG threads_waiting;

/* Transaction element block */

typedef struct teb {
	SLONG *teb_database;
	int teb_tpb_length;
	UCHAR *teb_tpb;
} TEB;

typedef struct req {
	struct req *req_next;
	CSV_MSG req_message;
} *REQ;

static REQ request_que, free_requests;
static EVENT_T thread_event[1];

#ifdef PIPE_SERVER
#ifdef VMS
/* Define logical names that sub-process should inherit */

static SCHAR *inherit_logicals[] = {
	"SYS$LOGIN",
	"SYS$SCRATCH",
	"SYS$NODE",
	"SYS$INTERBASE",
	0
};
#endif
#endif


#define GDS_ATTACH_DATABASE	gds__attach_database
#define GDS_BLOB_INFO		gds__blob_info
#define GDS_CANCEL_BLOB		gds__cancel_blob
#define GDS_CLOSE_BLOB		gds__close_blob
#define GDS_COMMIT		gds__commit_transaction
#define GDS_COMMIT_RETAINING	gds__commit_retaining
#define GDS_COMPILE		gds__compile_request
#define GDS_CREATE_BLOB		gds__create_blob2
#define GDS_CREATE_DATABASE	gds__create_database
#define GDS_DATABASE_INFO	gds__database_info
#define GDS_DDL			gds__ddl
#define GDS_DETACH		gds__detach_database
#define GDS_DROP_DATABASE	isc_drop_database
#define GDS_GET_SEGMENT		gds__get_segment
#define GDS_GET_SLICE		gds__get_slice
#define GDS_OPEN_BLOB		gds__open_blob2
#define GDS_PREPARE2		gds__prepare_transaction2
#define GDS_PUT_SEGMENT		gds__put_segment
#define GDS_PUT_SLICE		gds__put_slice
#define GDS_RECONNECT		gds__reconnect_transaction
#define GDS_RECEIVE		gds__receive
#define GDS_RELEASE_REQUEST	gds__release_request
#define GDS_REQUEST_INFO	gds__request_info
#define GDS_ROLLBACK		gds__rollback_transaction
#define GDS_SEND		gds__send
#define GDS_SEEK_BLOB		gds__seek_blob
#define GDS_START_AND_SEND	gds__start_and_send
#define GDS_START		gds__start_request
#define GDS_START_MULTIPLE	gds__start_multiple
#define GDS_START_TRANSACTION	gds__start_transaction
#define GDS_TRANSACT_REQUEST	isc_transact_request
#define GDS_TRANSACTION_INFO	gds__transaction_info
#define GDS_UNWIND		gds__unwind_request
#define GDS_QUE_EVENTS		gds__que_events
#define GDS_CANCEL_EVENTS	gds__cancel_events

#define GDS_DSQL_ALLOCATE	isc_dsql_allocate_statement
#define GDS_DSQL_EXECUTE	isc_dsql_execute2_m
#define GDS_DSQL_EXECUTE_IMMED	isc_dsql_exec_immed3_m
#define GDS_DSQL_FETCH		isc_dsql_fetch_m
#define GDS_DSQL_FREE		isc_dsql_free_statement
#define GDS_DSQL_INSERT		isc_dsql_insert_m
#define GDS_DSQL_PREPARE	isc_dsql_prepare_m
#define GDS_DSQL_SET_CURSOR	isc_dsql_set_cursor_name
#define GDS_DSQL_SQL_INFO	isc_dsql_sql_info

#ifndef PIPE_SERVER
int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n		( c e n t r a l _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Start up central server.
 *
 **************************************/
	CSV_MSG message;
	STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT sw_multi, sw_name, sw_buffers, sw_limit, length;
	TEXT **end, *p, *q, c, expanded_name[256], dflt_name[16];
	DBN db_name;
	CSH CSS_header;
	PRB process;
	SLONG debug_value;

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

	THREAD_ENTER;

	gds__enable_subsystem("GDSSHR");
	gds__enable_subsystem("GDSSHR5");
	num_buffers = 500;			/* choose a liberal number of buffers */

#ifdef MULTI_THREAD
	sw_multi = TRUE;
#else
	sw_multi = FALSE;
#endif

	sw_attach = sw_version = FALSE;
	sw_name = sw_buffers = sw_limit = FALSE;

	for (end = argv++ + argc; argv < end;) {
		p = *argv++;
		if (*p == '-') {
			sw_name = sw_buffers = sw_limit = FALSE;
			while (c = *++p)
				switch (UPPER(c)) {
#ifdef MULTI_THREAD
				case 'M':
					sw_multi = TRUE;
					break;

				case 'S':
					sw_multi = FALSE;
					break;
#endif

				case 'A':
					sw_attach = TRUE;
					break;

				case 'N':
					sw_name = TRUE;
					break;

				case 'B':
					sw_buffers = TRUE;
					break;

				case 'L':
					sw_limit = TRUE;
					break;


				case 'Z':
					if (!sw_version)
						printf("csv version %s\n", GDS_VERSION);
					sw_version = TRUE;
					break;
				}
		}
		else if (!sw_name && !sw_buffers && !sw_limit) {
			length = ISC_expand_filename(p, 0, expanded_name);
			db_name = (DBN) ALLOC(type_dbn, sizeof(struct dbn) + length);
			db_name->dbn_next = CSV_dbnames;
			CSV_dbnames = db_name;
			move(expanded_name, db_name->dbn_name, length + 1);
			db_name->dbn_length = length;
		}
		else {
			if (sw_name) {
				CSV_name_len = length = strlen(p);
				CSV_name = q = (TEXT *) ALLOC(0, CSV_name_len + 1);
				do {
					c = *p++;
					*q++ = UPPER(c);
				}
				while (length--);
			}

			if (sw_buffers)
				num_buffers = atoi(p);

			if (sw_limit && CSV_dbnames)
				CSV_dbnames->dbn_max_attaches = atoi(p);

			sw_name = sw_buffers = sw_limit = FALSE;
		}
	}

	if (!(CSS_header = CSS_init(status_vector, TRUE))) {
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	if (sw_attach)
		for (db_name = CSV_dbnames; db_name; db_name = db_name->dbn_next)
			if (attach_for_servicing(db_name, status_vector, TRUE)) {
				gds__print_status(status_vector);
				exit(FINI_ERROR);
			}

	process = (PRB) ABS_PTR(CSS_create_process(PRB_server));

	if (!CSV_name) {
		sprintf(dflt_name, "CSV_%ld", process->prb_process_number);
		CSV_name = dflt_name;
		CSV_name_len = strlen(CSV_name);
	}

	if (sw_multi)
		multi_thread();
	else
		while (!sw_shutdown) {
			message = CSS_get_message((SLONG) 0, 0, 0);
			process_message(message);
		}
}
#endif


#ifdef PIPE_SERVER
int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n		( p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Start up single user server.
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	CSH CSS_header;
	SRQ *que;
	PRB process;
	PTR connection, server, client;
	MSG_RESP response;
	CSV_MSG message;
	SSHORT parent_pin;

	THREAD_ENTER;

	gds__enable_subsystem("GDSSHR");
	gds__enable_subsystem("GDSSHR5");

	parent_pin = FATHER();

	if (!(CSS_header = CSS_init(status_vector, FALSE, parent_pin))) {
		gds__print_status(status_vector);
		exit(FINI_ERROR);
	}

	client = (PRB) ABS_PTR(CSS_create_process(PRB_client));

	CSS_header = ACQUIRE;
	server = (PTR) 0;

	QUE_LOOP(CSS_header->csh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		if (client != REL_PTR(process) &&
			process->prb_protocol_version == CSI_PROTOCOL_VERSION &&
			process->prb_process_id == parent_pin) {
			server = REL_PTR(process);
			break;
		}
	}

	RELEASE;

	if (!server) {
		printf("Unable to find parent process.\n");
		exit(FINI_ERROR);
	}

	connection = CSS_connect(server);
	response = CSS_alloc_message(type_msg, sizeof(struct msg_resp));
	response->msg_resp_header.msg_type = MSG_response;
	response->msg_resp_length = 0;
	if (CSS_put_message(connection, response, 0))
		while (!sw_shutdown && (message = CSS_get_message((SLONG) 0, 0, 0)))
			process_message(message);
}
#endif


static void allocate_statement( MSG_OP message)
{
/**************************************
 *
 *	a l l o c a t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate a statement handle.
 *
 **************************************/
	RDB rdb;
	RSR statement;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;
	handle = NULL;

	THREAD_EXIT;
	GDS_DSQL_ALLOCATE(status_vector, &rdb->rdb_handle, &handle);
	THREAD_ENTER;

	if (!status_vector[1]) {
		/* Allocate SQL request block */

		statement = (RSR) ALLOC(type_rsr, sizeof(struct rsr));
		statement->rsr_handle = handle;
		statement->rsr_rdb = rdb;
		statement->rsr_next = rdb->rdb_sql_requests;
		rdb->rdb_sql_requests = statement;
	}

	send_response(message, status_vector, statement, 0, 0);
}


static void alt_connection( MSG_OP message)
{
/**************************************
 *
 *	a l t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Accept a secondary connection for asynchronous events.
 *
 **************************************/
	RDB rdb;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;
	rdb->rdb_connection2 = message->msg_op_header.msg_connection;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;
	send_response(message, status_vector, rdb, 0, 0);
}


static void attach_database( MSG_ATT message)
{
/**************************************
 *
 *	a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Process an attach or create packet.
 *
 **************************************/
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH], *sv;
	RDB rdb;
	TEXT *expanded_name;
	UCHAR *dpb;
	DBN db_name;

/* Assume this is going to fail! */

	status_vector[0] = gds_arg_gds;
	status_vector[1] = gds__unavailable;
	status_vector[2] = gds_arg_end;

	handle = NULL;
	expanded_name =
		(TEXT *) message->msg_att_data + message->msg_att_file_length;
	dpb =
		message->msg_att_data + message->msg_att_file_length +
		message->msg_att_expanded_length;

	if (!
		(db_name =
		 find_dbname(expanded_name, message->msg_att_expanded_length, FALSE))
|| (db_name->dbn_max_attaches
	&& db_name->dbn_attaches >= db_name->dbn_max_attaches)) {
		send_response(message, status_vector, 0, 0, 0);
		return;
	}

/* Try initially with expanded name.  If that fails, try once more with
   original name */

	rdb = NULL;
	THREAD_EXIT;

	if (message->msg_att_header.msg_type == MSG_attach_database)
		GDS_ATTACH_DATABASE(status_vector,
							message->msg_att_expanded_length,
							GDS_VAL(expanded_name),
							GDS_REF(handle),
							message->msg_att_dpb_length, GDS_VAL(dpb));
	else
		GDS_CREATE_DATABASE(status_vector,
							message->msg_att_expanded_length,
							GDS_VAL(expanded_name),
							GDS_REF(handle),
							message->msg_att_dpb_length, GDS_VAL(dpb), 0);

	THREAD_ENTER;

	if (!status_vector[1]) {
		rdb = (RDB) ALLOC(type_rdb, sizeof(struct rdb));
		rdb->rdb_handle = handle;
		rdb->rdb_connection = message->msg_att_header.msg_connection;
		rdb->rdb_dbn = db_name;
		rdb->rdb_next = CSV_databases;
		CSV_databases = rdb;
		++db_name->dbn_attaches;
	}

	send_response(message, status_vector, rdb, 0, 0);
}


#ifndef PIPE_SERVER
static STATUS attach_for_servicing(
								   DBN db_name,
								   STATUS * status_vector, USHORT detach_flag)
{
/**************************************
 *
 *	a t t a c h _ f o r _ s e r v i c i n g
 *
 **************************************
 *
 * Functional description
 *	Attach a database that is to be serviced
 *	by this central server.
 *
 **************************************/
	TEXT dummy[128];
	USHORT length;
	HANDLE handle;
	RDB rdb;
	STATUS local_status[ISC_STATUS_LENGTH];
	SCHAR string[16], *dpb, *p;

/* generate the proper database parameter block */

	p = dpb = string;
	if (num_buffers) {
		*p++ = gds_dpb_version1;
		*p++ = gds__dpb_num_buffers;
		*p++ = 2;
		*p++ = num_buffers;
		*p++ = num_buffers >> 8;
	}

	handle = NULL;
	length = (USHORT) (p - dpb);
	THREAD_EXIT;
	GDS_ATTACH_DATABASE(GDS_VAL(status_vector),
						db_name->dbn_length,
						GDS_VAL(db_name->dbn_name),
						GDS_REF(handle), length, GDS_VAL(dpb));
	if (sw_version && !status_vector[1])
		gds__version(&handle, NULL, NULL);
	THREAD_ENTER;

	if (!status_vector[1]) {
		db_name->dbn_server = rdb = (RDB) ALLOC(type_rdb, sizeof(struct rdb));
		rdb->rdb_handle = handle;
		db_name->dbn_flags |= DBN_server_att;
	}
	else if (detach_flag) {
		printf("Unable to attach to database %s.\n", db_name->dbn_name);
		for (db_name = CSV_dbnames; db_name; db_name = db_name->dbn_next)
			if (rdb = db_name->dbn_server) {
				THREAD_EXIT;
				GDS_DETACH(local_status, GDS_REF(rdb->rdb_handle));
				THREAD_ENTER;
				FREE(rdb);
				db_name->dbn_server = NULL;
			}
	}

	return status_vector[1];
}
#endif


static void cancel_events( MSG_OP message)
{
/**************************************
 *
 *	c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Try to cancel an outstanding event.
 *
 **************************************/
	RDB rdb;
	EVNT event;
	SLONG id;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;
	id = message->msg_op_level;
	THREAD_EXIT;

	if (!GDS_CANCEL_EVENTS(status_vector,
						   GDS_REF(rdb->rdb_handle),
						   GDS_REF(id)))
			for (event = rdb->rdb_events; event; event = event->evnt_next)
			if (event->evnt_id == id) {
				event->evnt_id = 0;
				break;
			}

	THREAD_ENTER;

	send_response(message, status_vector, rdb, 0, 0);
}


static void check_if_done( DBN db_name)
{
/**************************************
 *
 *	c h e c k _ i f _ d o n e
 *
 **************************************
 *
 * Functional description
 *	Determine whether central server can exit.
 *	If it can, do so.
 *
 **************************************/
	RDB rdb;
	STATUS status_vector[ISC_STATUS_LENGTH];
	DBN *ptr;

/* If the database isn't disabled, in the process of being
   killed, or there are still attachments remaining, there's
   nothing to do but return. */

	if (!(db_name->dbn_flags & (DBN_disable | DBN_kill))
		|| db_name->dbn_attaches) return;

/* We're through with this database.  If the central server had
   its own attachment, get rid of it. */

	if (rdb = db_name->dbn_server) {
		THREAD_EXIT;
		GDS_DETACH(status_vector, GDS_REF(rdb->rdb_handle));
		THREAD_ENTER;
		FREE(rdb);
	}

/* Untangle database name block from global list of names */

	for (ptr = &CSV_dbnames; *ptr; ptr = &(*ptr)->dbn_next)
		if (*ptr == db_name) {
			*ptr = db_name->dbn_next;
			break;
		}

	FREE(db_name);

/* If no more name blocks remain, it's time to go! */

	if (!CSV_dbnames)
		sw_shutdown = TRUE;
}


static void compile( MSG_OP message)
{
/**************************************
 *
 *	c o m p i l e
 *
 **************************************
 *
 * Functional description
 *	Compile and request.
 *
 **************************************/
	RDB rdb;
	RRQ request;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;
	handle = NULL;
	request = NULL;

	THREAD_EXIT;
	GDS_COMPILE(status_vector,
				GDS_REF(rdb->rdb_handle),
				GDS_REF(handle),
				message->msg_op_length, message->msg_op_data);
	THREAD_ENTER;

	if (!status_vector[1]) {
		request = (RRQ) ALLOC(type_rrq, sizeof(struct rrq));
		request->rrq_handle = handle;
		request->rrq_rdb = rdb;
		request->rrq_next = rdb->rdb_requests;
		rdb->rdb_requests = request;
	}

	send_response(message, status_vector, request, 0, 0);
}


static void ddl( MSG_DDL message)
{
/**************************************
 *
 *	d d l
 *
 **************************************
 *
 * Functional description
 *	Do meta-data update.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_ddl_database;
	transaction = (RTR) message->msg_ddl_transaction;

	THREAD_EXIT;
	GDS_DDL(status_vector,
			GDS_REF(rdb->rdb_handle),
			GDS_REF(transaction->rtr_handle),
			message->msg_ddl_length, message->msg_ddl_data);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


#ifndef PIPE_SERVER
static void disable_or_kill( DBN db_name, USHORT cmd)
{
/**************************************
 *
 *	d i s a b l e _ o r _ k i l l
 *
 **************************************
 *
 * Functional description
 *	Disable, Kill, or Re-enable a database name for the
 *	central server utility.
 *
 **************************************/
	RDB *ptr, rdb;

/* Set or clear the appropriate flag */

	if (cmd == UTIL_reenable) {
		db_name->dbn_flags &= ~DBN_disable;
		return;
	}

	db_name->dbn_flags |= (cmd == UTIL_kill) ? DBN_kill : DBN_disable;

/* Do a quick check to see if we're done */

	check_if_done(db_name);

	if (cmd == UTIL_disable)
		return;

/* For the kill command loop through the database attachments
   and zap the ones attached to this database name */

	for (ptr = &CSV_databases; rdb = *ptr;)
		if (rdb->rdb_dbn == db_name)
			disconnect(rdb->rdb_connection);
		else
			ptr = &(*ptr)->rdb_next;
}
#endif


static void disconnect( PTR connection)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Handle a disconnect message.
 *
 **************************************/
	RDB rdb;
	RTR *ptr, transaction;
	STATUS status_vector[ISC_STATUS_LENGTH];

/* Start by breaking connection to remote */

	CSS_disconnect(connection);

/* Find related database block.  If we can't find it, just
   break connection.  Otherwise, shutdown the database */

	for (rdb = CSV_databases; rdb; rdb = rdb->rdb_next)
		if (rdb->rdb_connection == connection)
			break;

	if (!rdb)
		return;

/* Abort any outstanding, non-limbo transactions */

	for (ptr = &rdb->rdb_transactions; transaction = *ptr;)
		if (!(transaction->rtr_flags & RTR_limbo)) {
			THREAD_EXIT;
			GDS_ROLLBACK(status_vector, GDS_REF(transaction->rtr_handle));
			THREAD_ENTER;
			release_transaction(transaction);
		}
		else
			ptr = &(*ptr)->rtr_next;

	THREAD_EXIT;
	GDS_DETACH(status_vector, GDS_REF(rdb->rdb_handle));
	THREAD_ENTER;
	release_database(rdb);
}


static void drop_database( MSG_OP message)
{
/**************************************
 *
 *	d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
	RDB rdb;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;

	THREAD_EXIT;
	GDS_DROP_DATABASE(status_vector, rdb->rdb_handle);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);

	if (!status_vector[1]) {
		/* Start by breaking connection to remote */

		CSS_disconnect(rdb->rdb_connection);
		release_database(rdb);
	}
}


static void end_blob( MSG_OP message)
{
/**************************************
 *
 *	e n d _ b l o b
 *
 **************************************
 *
 * Functional description
 *	End a blob..
 *
 **************************************/
	RBL blob;
	STATUS status_vector[ISC_STATUS_LENGTH];

	blob = (RBL) message->msg_op_handle;
	THREAD_EXIT;

	if (message->msg_op_header.msg_type == MSG_close_blob)
		GDS_CLOSE_BLOB(status_vector, GDS_REF(blob->rbl_handle));
	else
		GDS_CANCEL_BLOB(status_vector, GDS_REF(blob->rbl_handle));

	THREAD_ENTER;

	if (!status_vector[1])
		release_blob(blob);

	send_response(message, status_vector, 0, 0, 0);
}


static void end_database( MSG_OP message)
{
/**************************************
 *
 *	e n d _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
	RDB rdb;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;

	THREAD_EXIT;
	GDS_DETACH(status_vector, GDS_REF(rdb->rdb_handle));
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);

	if (!status_vector[1]) {

		/* Start by breaking connection to remote */

		CSS_disconnect(rdb->rdb_connection);
		release_database(rdb);
	}
}


static void end_request( MSG_OP message)
{
/**************************************
 *
 *	e n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	End a request..
 *
 **************************************/
	RDB rdb;
	RRQ request;
	STATUS status_vector[ISC_STATUS_LENGTH];

	request = (RRQ) message->msg_op_handle;
	THREAD_EXIT;
	GDS_RELEASE_REQUEST(status_vector, GDS_REF(request->rrq_handle));
	THREAD_ENTER;

	if (!status_vector[1])
		release_request(request);

	send_response(message, status_vector, 0, 0, 0);
}


static void end_statement( MSG_OP message)
{
/**************************************
 *
 *	e n d _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Free a statement.
 *
 **************************************/
	RDB rdb;
	RSR statement;
	STATUS status_vector[ISC_STATUS_LENGTH];

	statement = (RSR) message->msg_op_handle;

	THREAD_EXIT;
	GDS_DSQL_FREE(status_vector,
				  &statement->rsr_handle, (USHORT) message->msg_op_level);
	THREAD_ENTER;

	if (!status_vector[1] && !statement->rsr_handle) {
		release_sql_request(statement);
		statement = NULL;
	}

	send_response(message, status_vector, statement, 0, 0);
}


static void end_transaction( MSG_OP message)
{
/**************************************
 *
 *	e n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	End a transaction.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	STATUS status_vector[ISC_STATUS_LENGTH];

	transaction = (RTR) message->msg_op_handle;
	THREAD_EXIT;

	switch (message->msg_op_header.msg_type) {
	case MSG_commit:
		if (!GDS_COMMIT(status_vector, GDS_REF(transaction->rtr_handle)))
			release_transaction(transaction);
		break;

	case MSG_rollback:
		if (!GDS_ROLLBACK(status_vector, GDS_REF(transaction->rtr_handle)))
			release_transaction(transaction);
		break;

	case MSG_commit_retaining:
		if (!GDS_COMMIT_RETAINING(status_vector,
								  GDS_REF(transaction->rtr_handle)))
				transaction->rtr_flags |= RTR_limbo;
		break;

	case MSG_prepare:
		if (!GDS_PREPARE2(status_vector,
						  GDS_REF(transaction->rtr_handle),
						  message->msg_op_length,
						  message->msg_op_data))
				transaction->rtr_flags |= RTR_limbo;
		break;
	}

	THREAD_ENTER;
	send_response(message, status_vector, 0, 0, 0);
}


static void execute_immediate( MSG_EXNOW message)
{
/**************************************
 *
 *	e x e c u t e _ i m m e d i a t e
 *
 **************************************
 *
 * Functional description
 *	Prepare and execute a statement
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	SCHAR *string, *in_blr, *in_msg, *out_blr, *out_msg;
	USHORT buffer_length;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_exnow_database;
	transaction = (RTR) message->msg_exnow_transaction;

	in_msg = (SCHAR *) message->msg_exnow_data;
	in_blr = in_msg + message->msg_exnow_in_msg_length;
	string = in_blr + message->msg_exnow_in_blr_length;
	out_blr = string + message->msg_exnow_SQL_length;

	buffer_length = message->msg_exnow_out_msg_length;
	out_msg = get_buffer(&buffer_length);

	handle = (transaction) ? transaction->rtr_handle : (HANDLE) 0;

	THREAD_EXIT;
	GDS_DSQL_EXECUTE_IMMED(status_vector,
						   &rdb->rdb_handle,
						   &handle,
						   message->msg_exnow_SQL_length,
						   string,
						   message->msg_exnow_SQL_dialect,
						   message->msg_exnow_in_blr_length,
						   in_blr,
						   message->msg_exnow_in_msg_type,
						   message->msg_exnow_in_msg_length,
						   in_msg,
						   message->msg_exnow_out_blr_length,
						   out_blr,
						   message->msg_exnow_out_msg_type,
						   message->msg_exnow_out_msg_length, out_msg);
	THREAD_ENTER;

	if (!status_vector[1])
		if (transaction && !handle) {
			release_transaction(transaction);
			transaction = NULL;
		}
		else if (!transaction && handle)
			transaction = make_transaction(rdb, handle);

	send_response(message, status_vector, transaction,
				  message->msg_exnow_out_msg_length, out_msg);
	free_buffer(out_msg, buffer_length);
}


static void execute_statement( MSG_SQLMSG message)
{
/**************************************
 *
 *	e x e c u t e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Execute a non-SELECT dynamic SQL statement.
 *
 **************************************/
	RTR transaction;
	RSR statement;
	SCHAR *in_blr, *in_msg, *out_blr, *out_msg;
	USHORT buffer_length;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	transaction = (RTR) message->msg_sqlmsg_transaction;
	statement = (RSR) message->msg_sqlmsg_statement;

	in_msg = (SCHAR *) message->msg_sqlmsg_data;
	in_blr = in_msg + message->msg_sqlmsg_in_msg_length;
	out_blr = in_blr + message->msg_sqlmsg_in_blr_length;

	buffer_length = message->msg_sqlmsg_out_msg_length;
	out_msg = get_buffer(&buffer_length);

	handle = (transaction) ? transaction->rtr_handle : (HANDLE) 0;

	THREAD_EXIT;
	GDS_DSQL_EXECUTE(status_vector,
					 &handle,
					 &statement->rsr_handle,
					 message->msg_sqlmsg_in_blr_length,
					 in_blr,
					 message->msg_sqlmsg_in_msg_type,
					 message->msg_sqlmsg_in_msg_length,
					 in_msg,
					 message->msg_sqlmsg_out_blr_length,
					 out_blr,
					 message->msg_sqlmsg_out_msg_type,
					 message->msg_sqlmsg_out_msg_length, out_msg);
	THREAD_ENTER;

	if (!status_vector[1])
		if (transaction && !handle) {
			release_transaction(transaction);
			transaction = NULL;
		}
		else if (!transaction && handle)
			transaction = make_transaction(statement->rsr_rdb, handle);

	send_response(message, status_vector, transaction,
				  message->msg_sqlmsg_out_msg_length, out_msg);
	free_buffer(out_msg, buffer_length);
}


static void fetch( MSG_SQLMSG message)
{
/**************************************
 *
 *	f e t c h
 *
 **************************************
 *
 * Functional description
 *	Fetch next record from a dynamic SQL cursor.
 *
 **************************************/
	RSR statement;
	USHORT msg_length;
	UCHAR *msg;
	STATUS s, status_vector[ISC_STATUS_LENGTH];

	statement = (RSR) message->msg_sqlmsg_statement;
	msg_length = message->msg_sqlmsg_in_msg_length;
	msg = get_buffer(&msg_length);

	THREAD_EXIT;
	s = GDS_DSQL_FETCH(status_vector,
					   &statement->rsr_handle,
					   message->msg_sqlmsg_in_blr_length,
					   message->msg_sqlmsg_data,
					   message->msg_sqlmsg_in_msg_type,
					   message->msg_sqlmsg_in_msg_length, msg);
	THREAD_ENTER;

	send_response(message, status_vector, s,
				  message->msg_sqlmsg_in_msg_length, msg);
	free_buffer(msg, msg_length);
}


#ifndef PIPE_SERVER
static DBN find_dbname(
					   TEXT * expanded_name,
					   USHORT expanded_length, USHORT search_flag)
{
/**************************************
 *
 *	f i n d _ d b n a m e	( c e n t r a l _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Find a database name in the list of database that
 *	can be serviced by this server.
 *
 **************************************/
	DBN db_name;

	for (db_name = CSV_dbnames; db_name; db_name = db_name->dbn_next)
		if (search_flag || !(db_name->dbn_flags & (DBN_disable | DBN_kill)))
			if (!strncmp(db_name->dbn_name, expanded_name, expanded_length))
				return db_name;

	return NULL;
}
#endif


#ifdef PIPE_SERVER
static DBN find_dbname(
					   TEXT * expanded_name,
					   USHORT expanded_length, USHORT search_flag)
{
/**************************************
 *
 *	f i n d _ d b n a m e	( p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Find a database name in the list of database that
 *	can be serviced by this server.
 *
 **************************************/

	if (!CSV_dbnames)
		CSV_dbnames = (DBN) ALLOC(type_dbn, sizeof(struct dbn));

	return CSV_dbnames;
}
#endif


static void free_buffer( UCHAR * buffer, USHORT length)
{
/**************************************
 *
 *	f r e e _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Release a buffer.  Keep one hanging
 *	around for the next guy.  If 
 *	another thread has already released
 *	one, let the smallest one go.
 *
 **************************************/

	if (!buffer)
		return;

	if (!blob_buffer) {
		blob_buffer = buffer;
		blob_buffer_length = length;
		return;
	}

	if (blob_buffer_length < length) {
		FREE(blob_buffer);
		blob_buffer = buffer;
		blob_buffer_length = length;
		return;
	}

	FREE(buffer);
}


static UCHAR *get_buffer( USHORT * length)
{
/**************************************
 *
 *	g e t _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	Get a buffer "big enough".
 *	Multiple buffers could be in use by
 *	multiple threads, so be sure to 
 *	allocate a new one in that case.
 *
 **************************************/
	UCHAR *buffer;

	if (!*length)
		return NULL;

	if (buffer = blob_buffer) {
		blob_buffer = NULL;
		if (blob_buffer_length >= *length) {
			*length = blob_buffer_length;
			return buffer;
		}
		else
			FREE(buffer);
	}

	return (UCHAR *) ALLOC(0, *length);
}


static void get_segment( MSG_SEG message)
{
/**************************************
 *
 *	g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Get a single blob segment.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	USHORT l, buffer_length, length;
	UCHAR *p, *buffer;
	HANDLE state;
	STATUS status_vector[ISC_STATUS_LENGTH];

	blob = (RBL) message->msg_seg_handle;
	buffer_length = message->msg_seg_buffer_length;
	buffer = get_buffer(&buffer_length);

	if (message->msg_seg_header.msg_type == MSG_get_segment) {
		/* Do the call.  We basically don't care whether it succeeds or not */

		length = 0;
		THREAD_EXIT;
		GDS_GET_SEGMENT(status_vector,
						GDS_REF(blob->rbl_handle),
						GDS_REF(length),
						message->msg_seg_buffer_length, GDS_VAL(buffer));
		THREAD_ENTER;

		send_response(message, status_vector, 0, length, buffer);
	}
	else {						/* msg_type == MSG_get_segment_buf */

		/* Gobble up a buffer's worth of segments */

		p = buffer;
		state = (HANDLE) 0;

		while (buffer_length > 2) {
			buffer_length -= 2;
			p += 2;
			THREAD_EXIT;
			GDS_GET_SEGMENT(status_vector,
							GDS_REF(blob->rbl_handle),
							GDS_REF(length), buffer_length, GDS_VAL(p));
			THREAD_ENTER;
			if (status_vector[1] == gds__segstr_eof) {
				state = (HANDLE) 2;
				status_vector[1] = FB_SUCCESS;
				p -= 2;
				break;
			}
			if (status_vector[1] && (status_vector[1] != gds__segment)) {
				p -= 2;
				break;
			}
			p[-2] = length;
			p[-1] = length >> 8;
			p += length;
			buffer_length -= length;
			if (status_vector[1] == gds__segment) {
				state = (HANDLE) 1;
				status_vector[1] = FB_SUCCESS;
				break;
			}
		}

		length = p - buffer;
		send_response(message, status_vector, state, length, buffer);
	}

	free_buffer(buffer, buffer_length);
}


static void get_slice( MSG_SLICE message)
{
/**************************************
 *
 *	g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	UCHAR *sdl, *param, *slice;
	ARRAY array, handle;
	USHORT length;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_slice_database;
	transaction = (RTR) message->msg_slice_transaction;

/* Arrays can be longer than 64k bytes but we limit central server
   messages to that length.  Therefore we may need to send the
   slice back to the user in pieces. */

	if (!(array = (ARRAY) message->msg_slice_handle)) {
		/* This is the first time through for this array.  Allocate memory
		   to hold the array and link it in with the transaction.  Then get
		   the data from the access method. */

		array =
			(ARRAY) gds__alloc(sizeof(struct array) +
							   message->msg_slice_slice_length - 1);
		array->array_rdb = rdb;
		array->array_rtr = transaction;
		array->array_length = 0;
		array->array_slice = array->array_data;
		array->array_next = transaction->rtr_arrays;
		transaction->rtr_arrays = array;

		sdl = message->msg_slice_data;
		param = sdl + message->msg_slice_sdl_length;

		THREAD_EXIT;

		GDS_GET_SLICE(status_vector,
					  GDS_REF(rdb->rdb_handle),
					  GDS_REF(transaction->rtr_handle),
					  message->msg_slice_id,
					  message->msg_slice_sdl_length,
					  GDS_VAL(sdl),
					  message->msg_slice_param_length,
					  GDS_VAL(param),
					  message->msg_slice_slice_length,
					  GDS_VAL(array->array_data),
					  GDS_REF(array->array_length));

		THREAD_ENTER;
	}
	else {
		status_vector[0] = gds_arg_gds;
		status_vector[1] = 0;
		status_vector[2] = gds_arg_end;

		/* We've already retrieved the array.  Figure out where we
		   saved the stuff and then send the next piece. */

		for (handle = transaction->rtr_arrays; handle;
			 handle = handle->array_next) if (handle == array)
				break;

		if (!handle ||
			handle->array_rdb != rdb || handle->array_rtr != transaction) {
			array = NULL;
			status_vector[1] = gds__segstr_no_op;
		}
	}

	if (!status_vector[1]) {
		length = MIN(array->array_length, MAX_ARRAY_MESSAGE);
		slice = array->array_slice;
		handle = (length < array->array_length) ? array : NULL;
	}
	else {
		length = 0;
		handle = NULL;
	}

	send_response(message, status_vector, handle, length, slice);

/* If we're done with the array, get rid of the memory that was used to
   hold it.  If there's more left to send, increment the pointers. */

	if (array)
		if (length && length < array->array_length) {
			array->array_length -= (SLONG) length;
			array->array_slice += length;
		}
		else
			release_array(array);
}


static void info( MSG_INFO message)
{
/**************************************
 *
 *	i n f o
 *
 **************************************
 *
 * Functional description
 *	Issue information.
 *
 **************************************/
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT buffer_length;
	UCHAR *buffer, temp[1024];
	RDB rdb;
	RTR transaction;
	RBL blob;
	RRQ request;
	RSR statement;

	handle = message->msg_info_handle;
	buffer_length = message->msg_info_buffer_length;
	buffer = get_buffer(&buffer_length);
	THREAD_EXIT;

	switch (message->msg_info_header.msg_type) {
	case MSG_database_info:
		rdb = (RDB) handle;
		if (!GDS_DATABASE_INFO(status_vector,
							   GDS_REF(rdb->rdb_handle),
							   message->msg_info_length,
							   message->msg_info_data,
							   sizeof(temp),
							   temp))
				MERGE_database_info(temp, buffer,
									message->msg_info_buffer_length,
#ifndef PIPE_SERVER
									IMPLEMENTATION, 10, 1, GDS_VERSION, "",
									0);
#else
									IMPLEMENTATION, 8, 1, GDS_VERSION, "", 0);
#endif
		break;

	case MSG_blob_info:
		blob = (RBL) handle;
		GDS_BLOB_INFO(status_vector,
					  GDS_REF(blob->rbl_handle),
					  message->msg_info_length,
					  message->msg_info_data,
					  message->msg_info_buffer_length, GDS_VAL(buffer));
		break;

	case MSG_request_info:
		request = (RRQ) handle;
		GDS_REQUEST_INFO(status_vector,
						 GDS_REF(request->rrq_handle),
						 message->msg_info_level,
						 message->msg_info_length,
						 message->msg_info_data,
						 message->msg_info_buffer_length, GDS_VAL(buffer));
		break;

	case MSG_transaction_info:
		transaction = (RTR) handle;
		GDS_TRANSACTION_INFO(status_vector,
							 GDS_REF(transaction->rtr_handle),
							 message->msg_info_length,
							 message->msg_info_data,
							 message->msg_info_buffer_length,
							 GDS_VAL(buffer));
		break;

	case MSG_sql_info:
		statement = (RSR) handle;
		GDS_DSQL_SQL_INFO(status_vector,
						  &statement->rsr_handle,
						  message->msg_info_length,
						  message->msg_info_data,
						  message->msg_info_buffer_length, buffer);
		break;
	}

	THREAD_ENTER;
	send_response(message, status_vector, 0, message->msg_info_buffer_length,
				  buffer);
	free_buffer(buffer, buffer_length);
}


static void insert( MSG_SQLMSG message)
{
/**************************************
 *
 *	i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert next record into a dynamic SQL cursor.
 *
 **************************************/
	RSR statement;
	UCHAR *blr, *msg;
	STATUS status_vector[ISC_STATUS_LENGTH];

	statement = (RSR) message->msg_sqlmsg_statement;

	msg = (SCHAR *) message->msg_sqlmsg_data;
	blr = msg + message->msg_sqlmsg_in_msg_length;

	THREAD_EXIT;
	GDS_DSQL_INSERT(status_vector,
					&statement->rsr_handle,
					message->msg_sqlmsg_in_blr_length,
					blr,
					message->msg_sqlmsg_in_msg_type,
					message->msg_sqlmsg_in_msg_length, msg);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


static RTR make_transaction( RDB rdb, HANDLE handle)
{
/**************************************
 *
 *	m a k e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Create a local transaction handle.
 *
 **************************************/
	RTR transaction;

	transaction = (RTR) ALLOC(type_rtr, sizeof(struct rtr));
	transaction->rtr_rdb = rdb;
	transaction->rtr_handle = handle;
	transaction->rtr_next = rdb->rdb_transactions;
	rdb->rdb_transactions = transaction;

	return transaction;
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
	SCHAR *from, *to;

	from = from_ptr;
	to = to_ptr;

	if (length)
		do
			*to++ = *from++;
		while (--length);
}


static void multi_thread(void)
{
/**************************************
 *
 *	m u l t i _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Multi-threaded flavor of server.
 *
 **************************************/
#ifdef MULTI_THREAD
	REQ request, *ptr;
	SLONG pending_requests;

	gds__thread_enable(TRUE);
	ISC_event_init(thread_event, EVENT_FLAG);

	while (!sw_shutdown) {
		if (request = free_requests)
			free_requests = request->req_next;
		if (!request)
			request = (REQ) gds__alloc((SLONG) sizeof(struct req));
		request->req_next = NULL;
		request->req_message = CSS_get_message((SLONG) 0, 0, 0);

		for (pending_requests = 1, ptr = &request_que; *ptr;
			 ptr = &(*ptr)->req_next) ++pending_requests;
		*ptr = request;

		if (pending_requests > threads_waiting)
			gds__thread_start(thread);
		ISC_event_post(thread_event);
	}

	while (request = request_que) {
		request_que = request->req_next;
		CSS_free_global(request->req_message);
	}
#endif
}


static void open_blob( MSG_BLOB message)
{
/**************************************
 *
 *	o p e n _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Open or create a new blob.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	RTR transaction;
	USHORT length;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_blob_database;
	transaction = (RTR) message->msg_blob_transaction;
	handle = NULL;
	blob = NULL;
	length = 0;
	THREAD_EXIT;

	if (message->msg_blob_header.msg_type == MSG_open_blob) {
		GDS_OPEN_BLOB(status_vector,
					  GDS_REF(rdb->rdb_handle),
					  GDS_REF(transaction->rtr_handle),
					  GDS_REF(handle),
					  message->msg_blob_id,
					  message->msg_blob_bpb_length, message->msg_blob_bpb);
	}
	else {
		GDS_CREATE_BLOB(status_vector,
						GDS_REF(rdb->rdb_handle),
						GDS_REF(transaction->rtr_handle),
						GDS_REF(handle),
						message->msg_blob_id,
						message->msg_blob_bpb_length, message->msg_blob_bpb);
		length = sizeof(message->msg_blob_id);
	}

	THREAD_ENTER;

	if (!status_vector[1]) {
		blob = (RBL) ALLOC(type_rbl, sizeof(struct rbl));
		blob->rbl_handle = handle;
		blob->rbl_rdb = rdb;
		blob->rbl_rtr = transaction;
		blob->rbl_next = transaction->rtr_blobs;
		transaction->rtr_blobs = blob;
	}

	send_response(message, status_vector, blob, length, message->msg_blob_id);
}


static void ping( MSG_OP message)
{
/**************************************
 *
 *	p i n g
 *
 **************************************
 *
 * Functional description
 *	Bounce a message straight back.
 *
 **************************************/
	RRQ request;
	STATUS status_vector[ISC_STATUS_LENGTH];

	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	send_response(message, status_vector, 0, 0, 0);
}


static void prepare_statement( MSG_PSTMT message)
{
/**************************************
 *
 *	p r e p a r e _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Prepare a dynamic SQL statement for execution.
 *
 **************************************/
	RTR transaction;
	RSR statement;
	SCHAR *string, *items;
	USHORT buffer_length;
	UCHAR *buffer;
	STATUS status_vector[ISC_STATUS_LENGTH];
	HANDLE handle;

	transaction = (RTR) message->msg_pstmt_transaction;
	statement = (RSR) message->msg_pstmt_statement;

	handle = (transaction) ? transaction->rtr_handle : (HANDLE) 0;

	string = (SCHAR *) message->msg_pstmt_data;
	items = string + message->msg_pstmt_SQL_length;
	buffer_length = message->msg_pstmt_buffer_length;
	buffer = get_buffer(&buffer_length);

	THREAD_EXIT;
	GDS_DSQL_PREPARE(status_vector,
					 &handle,
					 &statement->rsr_handle,
					 message->msg_pstmt_SQL_length,
					 string,
					 message->msg_pstmt_SQL_dialect,
					 message->msg_pstmt_item_length,
					 items, buffer_length, buffer);
	THREAD_ENTER;

	send_response(message, status_vector, 0, message->msg_pstmt_buffer_length,
				  buffer);
	free_buffer(buffer, buffer_length);
}


static void process_message( CSV_MSG message)
{
/**************************************
 *
 *	p r o c e s s _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Handle a single message.
 *
 **************************************/

	switch (message->msg_type) {
	case MSG_alt_connection:
		alt_connection(message);
		break;

	case MSG_attach_database:
	case MSG_create_database:
		attach_database(message);
		break;

	case MSG_compile:
		compile(message);
		break;

	case MSG_ddl:
		ddl(message);
		break;

	case MSG_cancel_events:
		cancel_events(message);
		break;

	case MSG_detach:
		end_database(message);
		break;

	case MSG_drop_database:
		drop_database(message);
		break;

	case MSG_que_events:
		que_events(message);
		break;

	case MSG_receive:
		receive_msg(message);
		break;

	case MSG_send:
		send_msg(message);
		break;

	case MSG_start_request:
		start(message);
		break;

	case MSG_start_and_send:
		start_and_send(message);
		break;

	case MSG_start_transaction:
		start_transaction(message);
		break;

	case MSG_prepare:
	case MSG_rollback:
	case MSG_commit:
	case MSG_commit_retaining:
		end_transaction(message);
		break;

	case MSG_create_blob:
	case MSG_open_blob:
		open_blob(message);
		break;

	case MSG_put_segment:
	case MSG_put_segment_buf:
		put_segment(message);
		break;

	case MSG_get_segment:
	case MSG_get_segment_buf:
		get_segment(message);
		break;

	case MSG_seek_blob:
		seek_blob(message);
		break;

	case MSG_get_slice:
		get_slice(message);
		break;

	case MSG_put_slice:
		put_slice(message);
		break;

	case MSG_cancel_blob:
	case MSG_close_blob:
		end_blob(message);
		break;

	case MSG_release:
		end_request(message);
		break;

	case MSG_reconnect:
		reconnect(message);
		break;

	case MSG_transact_request:
		transact_request(message);
		break;

	case MSG_unwind:
		unwind(message);
		break;

	case MSG_blob_info:
	case MSG_request_info:
	case MSG_database_info:
	case MSG_transaction_info:
	case MSG_sql_info:
		info(message);
		break;

	case MSG_ping:
		ping(message);
		break;

	case MSG_allocate_stmt:
		allocate_statement(message);
		break;

	case MSG_execute:
		execute_statement(message);
		break;

	case MSG_execute_immediate:
		execute_immediate(message);
		break;

	case MSG_fetch:
		fetch(message);
		break;

	case MSG_free_stmt:
		end_statement(message);
		break;

	case MSG_insert:
		insert(message);
		break;

	case MSG_prepare_stmt:
		prepare_statement(message);
		break;

	case MSG_set_cursor:
		set_cursor(message);
		break;

	case MSG_util_cmd:
		server_utility(message);
		break;

	case MSG_disconnect:
		disconnect(message->msg_connection);
		CSS_free_global(message);
		break;

	default:
		printf("Unknown message type %d\n", message->msg_type);
	}
}


static void put_segment( MSG_SEG message)
{
/**************************************
 *
 *	p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Write a single blob segment.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	USHORT buffer_length, length;
	UCHAR *buffer, *end;
	STATUS status_vector[ISC_STATUS_LENGTH];

	blob = (RBL) message->msg_seg_handle;
	buffer_length = message->msg_seg_length;
	buffer = message->msg_seg_data;

	if (message->msg_seg_header.msg_type == MSG_put_segment) {
		/* Do the signal segment version.  If it failed, just pass on the
		   bad news. */

		THREAD_EXIT;
		GDS_PUT_SEGMENT(status_vector,
						GDS_REF(blob->rbl_handle),
						buffer_length, GDS_VAL(buffer));
		THREAD_ENTER;
	}
	else {						/* msg_type == MSG_put_segment_buf */

		/* We've got a batch of segments.  This is only slightly more complicated */

		end = buffer + buffer_length;

		while (buffer < end) {
			length = *buffer++;
			length += *buffer++ << 8;
			THREAD_EXIT;
			GDS_PUT_SEGMENT(status_vector,
							GDS_REF(blob->rbl_handle),
							length, GDS_VAL(buffer));
			THREAD_ENTER;
			if (status_vector[1])
				break;
			buffer += length;
		}
	}

	send_response(message, status_vector, 0, 0, 0);
}


static void put_slice( MSG_SLICE message)
{
/**************************************
 *
 *	p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Put a slice of an array.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	UCHAR *sdl, *param;
	ARRAY array, handle;
	USHORT length;
	STATUS status_vector[ISC_STATUS_LENGTH];
	UCHAR *from, *to;

	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

	rdb = (RDB) message->msg_slice_database;
	transaction = (RTR) message->msg_slice_transaction;

/* Arrays can be longer than 64k bytes but we limit central server
   messages to that length.  Therefore we may need to get the slice
   from the user in pieces. */

	if (!(array = (ARRAY) message->msg_slice_handle)) {
		/* This is the first time through for this array.  Allocate memory
		   to hold the array and link it in with the transaction. */

		array =
			(ARRAY) gds__alloc(sizeof(struct array) +
							   message->msg_slice_slice_length - 1);
		array->array_rdb = rdb;
		array->array_rtr = transaction;
		array->array_length = message->msg_slice_slice_length;
		array->array_slice = array->array_data;
		array->array_next = transaction->rtr_arrays;
		transaction->rtr_arrays = array;
	}
	else {
		/* We've already allocated the array.  Figure out where we
		   were and save the next piece. */

		for (handle = transaction->rtr_arrays; handle;
			 handle = handle->array_next) if (handle == array)
				break;

		if (!handle ||
			handle->array_rdb != rdb || handle->array_rtr != transaction) {
			array = NULL;
			status_vector[1] = gds__segstr_no_op;
		}
	}

	if (!status_vector[1]) {
		length = MIN(array->array_length, MAX_ARRAY_MESSAGE);
		array->array_length -= (SLONG) length;

		sdl = message->msg_slice_data;
		param = sdl + message->msg_slice_sdl_length;
		from = param + message->msg_slice_param_length;;

		to = array->array_slice;
		if (length)
			do
				*to++ = *from++;
			while (--length);

		array->array_slice = to;

		if (!array->array_length) {
			array->array_length = array->array_slice - array->array_data;
			THREAD_EXIT;

			GDS_PUT_SLICE(status_vector,
						  GDS_REF(rdb->rdb_handle),
						  GDS_REF(transaction->rtr_handle),
						  message->msg_slice_id,
						  message->msg_slice_sdl_length,
						  GDS_VAL(sdl),
						  message->msg_slice_param_length,
						  GDS_VAL(param),
						  array->array_length, GDS_VAL(array->array_data));

			THREAD_ENTER;

			release_array(array);
			array = NULL;
		}
	}

	length = status_vector[1] ? 0 : 8;
	send_response(message, status_vector, array, length,
				  message->msg_slice_id);
}


static void que_events( MSG_EVENT message)
{
/**************************************
 *
 *	q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que a request for event notification.
 *
 **************************************/
	RDB rdb;
	EVNT event;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_event_database;

/* Find available event block */

	for (event = rdb->rdb_events; event; event = event->evnt_next)
		if (!event->evnt_id)
			break;

	if (!event) {
		event = (EVNT) CSS_alloc_local(type_evnt, sizeof(struct evnt));
		event->evnt_next = rdb->rdb_events;
		rdb->rdb_events = event;
		event->evnt_rdb = rdb;
	}

	event->evnt_ast = message->msg_event_ast;
	event->evnt_arg = message->msg_event_arg;

	THREAD_EXIT;
	GDS_QUE_EVENTS(status_vector,
				   GDS_REF(rdb->rdb_handle),
				   GDS_REF(event->evnt_id),
				   message->msg_event_length,
				   message->msg_event_data, server_ast, GDS_VAL(event));
	THREAD_ENTER;

	send_response(message, status_vector, event->evnt_id, 0, 0);
}


static void receive_msg( MSG_MSG message)
{
/**************************************
 *
 *	r e c e i v e _ m s g
 *
 **************************************
 *
 * Functional description
 *	Receive a message.
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	RRQ request;
	USHORT buffer_length;
	UCHAR *buffer;

	request = (RRQ) message->msg_msg_request;
	buffer_length = message->msg_msg_length;
	buffer = get_buffer(&buffer_length);

	THREAD_EXIT;
	GDS_RECEIVE(status_vector,
				GDS_REF(request->rrq_handle),
				message->msg_msg_type,
				message->msg_msg_length,
				GDS_VAL(buffer), message->msg_msg_level);
	THREAD_ENTER;

	send_response(message, status_vector, 0, message->msg_msg_length, buffer);
	free_buffer(buffer, buffer_length);
}


static void reconnect( MSG_OP message)
{
/**************************************
 *
 *	r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Reconnect to a transaction in limbo.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_op_handle;
	handle = NULL;
	transaction = NULL;

	THREAD_EXIT;
	GDS_RECONNECT(status_vector,
				  GDS_REF(rdb->rdb_handle),
				  GDS_REF(handle),
				  message->msg_op_length, message->msg_op_data);
	THREAD_ENTER;

	if (!status_vector[1])
		transaction = make_transaction(rdb, handle);

	send_response(message, status_vector, transaction, 0, 0);
}


static void release_array( ARRAY array)
{
/**************************************
 *
 *	r e l e a s e _ a r r a y
 *
 **************************************
 *
 * Functional description
 *	Release an array block and friends.
 *
 **************************************/
	RTR transaction;
	ARRAY *p;

	transaction = array->array_rtr;

	for (p = &transaction->rtr_arrays; *p; p = &(*p)->array_next)
		if (*p == array) {
			*p = array->array_next;
			break;
		}

	FREE(array);
}


static void release_blob( RBL blob)
{
/**************************************
 *
 *	r e l e a s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Release a blob block and friends.
 *
 **************************************/
	RTR transaction;
	RBL *p;

	transaction = blob->rbl_rtr;

	for (p = &transaction->rtr_blobs; *p; p = &(*p)->rbl_next)
		if (*p == blob) {
			*p = blob->rbl_next;
			break;
		}

	FREE(blob);
}


static void release_database( RDB rdb)
{
/**************************************
 *
 *	r e l e a s e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Release database block and friends and relations.
 *
 **************************************/
	EVNT event;
	RDB *ptr;
	DBN db_name;

	db_name = rdb->rdb_dbn;

/* Next, get rid of any request or transaction block */

	while (rdb->rdb_requests)
		release_request(rdb->rdb_requests);

	while (rdb->rdb_sql_requests)
		release_sql_request(rdb->rdb_sql_requests);

	while (rdb->rdb_transactions)
		release_transaction(rdb->rdb_transactions);

	while (event = rdb->rdb_events) {
		rdb->rdb_events = event->evnt_next;
		FREE(event);
	}

/* Untangle database block from global list of databases */

	for (ptr = &CSV_databases; *ptr; ptr = &(*ptr)->rdb_next)
		if (*ptr == rdb) {
			*ptr = rdb->rdb_next;
			break;
		}

/* Finally, release the database block itself */

	FREE(rdb);

/* Decrement the attachment counter and see if the server can exit */

	--db_name->dbn_attaches;
	check_if_done(db_name);
}


static void release_request( RRQ request)
{
/**************************************
 *
 *	r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request block and friends.
 *
 **************************************/
	RDB rdb;
	RRQ *p;
	USHORT i;

	rdb = request->rrq_rdb;

	for (p = &rdb->rdb_requests; *p; p = &(*p)->rrq_next)
		if (*p == request) {
			*p = request->rrq_next;
			break;
		}

	FREE(request);
}


static void release_sql_request( RSR stmt)
{
/**************************************
 *
 *	r e l e a s e _ s q l _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release an SQL request block.
 *
 **************************************/
	RDB rdb;
	RSR *p;

	rdb = stmt->rsr_rdb;

	for (p = &rdb->rdb_sql_requests; *p; p = &(*p)->rsr_next)
		if (*p == stmt) {
			*p = stmt->rsr_next;
			break;
		}

	FREE(stmt);
}


static void release_transaction( RTR transaction)
{
/**************************************
 *
 *	r e l e a s e _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Release a transaction block and friends.
 *
 **************************************/
	RDB rdb;
	RTR *p;

	rdb = transaction->rtr_rdb;

	while (transaction->rtr_blobs)
		release_blob(transaction->rtr_blobs);

	while (transaction->rtr_arrays)
		release_array(transaction->rtr_arrays);

	for (p = &rdb->rdb_transactions; *p; p = &(*p)->rtr_next)
		if (*p == transaction) {
			*p = transaction->rtr_next;
			break;
		}

	FREE(transaction);
}


static void seek_blob( MSG_SEEK message)
{
/**************************************
 *
 *	g e t _ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Perform a blob seek.
 *
 **************************************/
	RDB rdb;
	RBL blob;
	SLONG result;
	STATUS status_vector[ISC_STATUS_LENGTH];

	blob = (RBL) message->msg_seek_handle;

/* Do the call.  We basically don't care whether it succeeds or not */

	THREAD_EXIT;
	GDS_SEEK_BLOB(status_vector,
				  GDS_REF(blob->rbl_handle),
				  message->msg_seek_mode,
				  message->msg_seek_offset, GDS_REF(result));
	THREAD_ENTER;

	send_response(message, status_vector, 0, sizeof(result), &result);
}


static void send_msg( MSG_MSG message)
{
/**************************************
 *
 *	s e n d _ m s g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	RRQ request;
	RTR transaction;
	RDB rdb;

	request = (RRQ) message->msg_msg_request;

	THREAD_EXIT;
	GDS_SEND(status_vector,
			 GDS_REF(request->rrq_handle),
			 message->msg_msg_type,
			 message->msg_msg_length,
			 message->msg_msg_data, message->msg_msg_level);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


static int send_response(
						 CSV_MSG source,
						 STATUS * status_vector,
						 SLONG handle, USHORT length, UCHAR * data)
{
/**************************************
 *
 *	s e n d _ r e s p o n s e
 *
 **************************************
 *
 * Functional description
 *	Send a response packet.
 *
 **************************************/
	STATUS code, *status, *target;
	UCHAR *p, *q, **s;
	MSG_RESP message;
	USHORT l, msg_length;

#ifdef DEBUG
	fprintf(stderr, "send_response: %x %x %x %x\n",
			status_vector[0], status_vector[1], status_vector[2],
			status_vector[3]);
#endif

/* Compute variable length portion of message vector */

	msg_length = 0;

	if (!status_vector[1])
		status_vector[2] = gds_arg_end;
	else
		for (status = status_vector; *status && status < status_vector + ISC_STATUS_LENGTH;)
			switch (*status++) {
			case gds_arg_interpreted:
			case gds_arg_string:
				s = (UCHAR **) status;
				p = *s++;
				status = (STATUS *) s;
				msg_length += strlen(p) + 1;
				break;

			case gds_arg_cstring:
				msg_length += *status++;
				++status;
				break;

			default:
				++status;
			}

/* Allocate and format response message */

	l = sizeof(struct msg_resp) + msg_length + length;
	message = CSS_alloc_message(type_msg, (int) l);
	message->msg_resp_header.msg_type = MSG_response;
	message->msg_resp_handle = handle;
	p = message->msg_resp_data;

	if (l = message->msg_resp_length = length) {
		q = data;
		do
			*p++ = *q++;
		while (--l);
	}


/* Start by translating the status vector into "generic" form */

	for (status = status_vector, target = message->msg_resp_status;
		 (*target++ = *status) && status < status_vector + ISC_STATUS_LENGTH;)
		switch (*status++) {
		case gds_arg_interpreted:
		case gds_arg_string:
			s = (UCHAR **) status;
			q = *s++;
			status = (STATUS *) s;
			*target++ = p - (UCHAR *) message;
			while (*p++ = *q++);
			break;

		case gds_arg_cstring:
			*target++ = l = *status++;
			s = (UCHAR **) status;
			q = *s++;
			status = (STATUS *) s;
			*target++ = p - (UCHAR *) message;
			if (l)
				do
					*p++ = *q++;
				while (--l);
			break;

		default:
			*target++ = *status++;
			break;
		}

	return CSS_put_message(source->msg_connection, message, source);
}


static void server_ast( EVNT event, USHORT length, UCHAR * data)
{
/**************************************
 *
 *	s e r v e r _ a s t
 *
 **************************************
 *
 * Functional description
 *	Send a message across central interface.
 *
 **************************************/
	RDB rdb;
	MSG_EVENT message;

	rdb = event->evnt_rdb;
	message =
		(MSG_EVENT) CSS_alloc_message(type_msg,
									  (int) (sizeof(struct msg_event) +
											 length));
	message->msg_event_header.msg_type = MSG_event;
	message->msg_event_ast = event->evnt_ast;
	message->msg_event_arg = event->evnt_arg;
	message->msg_event_length = length;
	move(data, message->msg_event_data, length);
	CSS_put_message(rdb->rdb_connection2, message, NULL);
	event->evnt_id = 0;
}


static void set_cursor( MSG_SETCUR message)
{
/**************************************
 *
 *	s e t _ c u r s o r
 *
 **************************************
 *
 * Functional description
 *	Declare a cursor for a dynamic request.
 *
 **************************************/
	RSR statement;
	STATUS status_vector[ISC_STATUS_LENGTH];

	statement = (RSR) message->msg_setcur_statement;

	THREAD_EXIT;
	GDS_DSQL_SET_CURSOR(status_vector,
						&statement->rsr_handle,
						message->msg_setcur_cursor, message->msg_setcur_type);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


static void start( MSG_MSG message)
{
/**************************************
 *
 *	s t a r t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	RRQ request;
	RTR transaction;

	request = (RRQ) message->msg_msg_request;
	transaction = (RTR) message->msg_msg_transaction;

	THREAD_EXIT;
	GDS_START(status_vector,
			  GDS_REF(request->rrq_handle),
			  GDS_REF(transaction->rtr_handle), message->msg_msg_level);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


static void start_and_send( MSG_MSG message)
{
/**************************************
 *
 *	s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	RRQ request;
	RTR transaction;

	request = (RRQ) message->msg_msg_request;
	transaction = (RTR) message->msg_msg_transaction;

	THREAD_EXIT;
	GDS_START_AND_SEND(status_vector,
					   GDS_REF(request->rrq_handle),
					   GDS_REF(transaction->rtr_handle),
					   message->msg_msg_type,
					   message->msg_msg_length,
					   message->msg_msg_data, message->msg_msg_level);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}


static void start_transaction( MSG_TRANS message)
{
/**************************************
 *
 *	s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	TEB tebs[16], *teb;
	TDB *tdb, *end;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	for (tdb = message->msg_trans_tdb, end =
		 tdb + message->msg_trans_count, teb = tebs; tdb < end; tdb++, teb++) {
		rdb = (RDB) tdb->tdb_database;
		teb->teb_database = &rdb->rdb_handle;
		teb->teb_tpb_length = tdb->tdb_tpb_length;
		teb->teb_tpb = (UCHAR *) message + tdb->tdb_tpb;
	}

	handle = NULL;
	transaction = NULL;

	THREAD_EXIT;
	GDS_START_MULTIPLE(status_vector,
					   GDS_REF(handle), message->msg_trans_count, tebs);
	THREAD_ENTER;

	if (!status_vector[1])
		transaction = make_transaction(rdb, handle);

	send_response(message, status_vector, transaction, 0, 0);
}


#ifndef PIPE_SERVER
static void server_utility( MSG_UTIL message)
{
/**************************************
 *
 *	s e r v e r _ u t i l i t y
 *
 **************************************
 *
 * Functional description
 *	Process a message from the central server utility.
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT cmd, length, l;
	CSU_LIST list_msg, ptr_data;
	DBN db_name, *ptr;
	TEXT *name;

/* Assume everything is going to go ok */

	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

	if (message->msg_util_csn_len && strcmp(message->msg_util_data, CSV_name)) {
		/* The message is not meant for us */

		status_vector[1] = gds__unavailable;
		send_response(message, status_vector, (SLONG) 0, 0, 0);
		return;
	}

	name = message->msg_util_data + message->msg_util_csn_len + 1;

	switch (cmd = message->msg_util_cmd) {
	case UTIL_list:
		length =
			FB_ALIGN(CSV_name_len + sizeof(struct csu_list),
					 2) + sizeof(struct csu_list);
		for (db_name = CSV_dbnames; db_name; db_name = db_name->dbn_next)
			length +=
				FB_ALIGN(sizeof(struct csu_list) + db_name->dbn_length, 2);
		list_msg = ptr_data = (CSU_LIST) ALLOC(0, length);

		ptr_data->csu_list_length = CSV_name_len;
		move(CSV_name, ptr_data->csu_list_name, CSV_name_len + 1);
		ptr_data =
			(CSU_LIST) (ptr_data->csu_list_name +
						FB_ALIGN(CSV_name_len + 1, 2));

		for (db_name = CSV_dbnames; db_name; db_name = db_name->dbn_next) {
			ptr_data->csu_list_attaches = db_name->dbn_attaches;
			ptr_data->csu_list_flags = db_name->dbn_flags;
			ptr_data->csu_list_length = l = db_name->dbn_length;
			move(db_name->dbn_name, ptr_data->csu_list_name, l + 1);
			ptr_data =
				(CSU_LIST) (ptr_data->csu_list_name + FB_ALIGN(l + 1, 2));
		}
		ptr_data->csu_list_length = 0;

		send_response(message, status_vector, (SLONG) 0, length, list_msg);
		FREE(list_msg);
		break;

	case UTIL_disable:
	case UTIL_kill:
	case UTIL_reenable:
		/* See if the utility specified a particular database name
		   and if so, whether we know about it.  If we don't know
		   about it, return "unavailable". */

		if ((length = message->msg_util_dbn_len) &&
			!(db_name = find_dbname(name, length, TRUE)))
			status_vector[1] = gds__unavailable;

		send_response(message, status_vector, (SLONG) 0, 0, 0);

		/* If we know about the database name, disable, re-enable, or kill
		   one or all databases */

		if (!status_vector[1])
			if (length)
				disable_or_kill(db_name, cmd);
			else
				for (ptr = &CSV_dbnames; db_name = *ptr;) {
					disable_or_kill(db_name, cmd);
					if (db_name == *ptr)
						ptr = &(*ptr)->dbn_next;
				}
		break;

	case UTIL_enable:
		/* Let's make sure that we're not already servicing the database.
		   If we're not and the -a switch was specified, try to attach the
		   database. */

		length = message->msg_util_dbn_len;
		if (!find_dbname(name, length, TRUE)) {
			db_name = (DBN) ALLOC(type_dbn, sizeof(struct dbn) + length);
			move(name, db_name->dbn_name, length + 1);
			db_name->dbn_length = length;
			if (!sw_attach
				|| !attach_for_servicing(db_name, status_vector, FALSE)) {
				db_name->dbn_next = CSV_dbnames;
				CSV_dbnames = db_name;
			}
		}
		else {
			status_vector[1] = gds__random;
			status_vector[2] = gds_arg_string;
			status_vector[3] = (STATUS) "Database already known to server";
			status_vector[4] = gds_arg_end;
		}
		send_response(message, status_vector, (SLONG) 0, 0, 0);
		break;

	default:
		printf("Unknown server utility command %d\n", message->msg_util_cmd);
		status_vector[1] = gds__unavailable;
		send_response(message, status_vector, (SLONG) 0, 0, 0);
		break;
	}
}
#endif


#ifdef PIPE_SERVER
static void server_utility( MSG_UTIL message)
{
/**************************************
 *
 *	s e r v e r _ u t i l i t y
 *
 **************************************
 *
 * Functional description
 *	Process a message from the central server utility.
 *
 **************************************/
	STATUS status_vector[ISC_STATUS_LENGTH];
	RDB *ptr, rdb;
	PTR connection;

	switch (message->msg_util_cmd) {
	case UTIL_kill:
		/* Loop through the database attachments and zap them all */

		for (ptr = &CSV_databases; rdb = *ptr;)
			disconnect(rdb->rdb_connection);

		sw_shutdown = TRUE;

		connection = message->msg_util_header.msg_connection;
		CSS_free_global(message);
		CSS_disconnect(connection);
		return;

	default:
		status_vector[0] = gds_arg_gds;
		status_vector[1] = gds__unavailable;
		status_vector[2] = gds_arg_end;
		send_response(message, status_vector, (SLONG) 0, 0, 0);
		break;
	}
}
#endif


#ifdef MULTI_THREAD
static void thread(void)
{
/**************************************
 *
 *	t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Execute requests in a happy loop.
 *
 **************************************/
	REQ request;
	SLONG value;
	EVENT ptr;

	request = NULL;
	++threads_waiting;
	THREAD_ENTER;

	while (!sw_shutdown) {
		value = ISC_event_clear(thread_event);

		if (request) {
			request->req_next = free_requests;
			free_requests = request;
		}

		if (request = request_que)
			request_que = request->req_next;

		if (request) {
			--threads_waiting;
			process_message(request->req_message);
			++threads_waiting;
		}
		else {
			ptr = thread_event;
			THREAD_EXIT;
			ISC_event_wait(1, &ptr, &value, -1, 0, 0);
			THREAD_ENTER;
		}
	}

	ISC_event_clear(thread_event);
	ISC_event_post(thread_event);

	THREAD_EXIT;
	--threads_waiting;
}
#endif


static void transact_request( MSG_TRRQ message)
{
/**************************************
 *
 *	t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Execute a stored procedure.
 *
 **************************************/
	RDB rdb;
	RTR transaction;
	SCHAR *blr, *in_msg, *out_msg;
	USHORT buffer_length;
	HANDLE handle;
	STATUS status_vector[ISC_STATUS_LENGTH];

	rdb = (RDB) message->msg_trrq_database;
	transaction = (RTR) message->msg_trrq_transaction;

	blr = in_msg;
	in_msg = blr + message->msg_trrq_blr_length;

	buffer_length = message->msg_trrq_out_msg_length;
	out_msg = get_buffer(&buffer_length);

	handle = (transaction) ? transaction->rtr_handle : (HANDLE) 0;

	THREAD_EXIT;
	GDS_TRANSACT_REQUEST(status_vector,
						 &rdb->rdb_handle,
						 &handle,
						 message->msg_trrq_blr_length,
						 blr,
						 message->msg_trrq_in_msg_length,
						 in_msg, message->msg_trrq_out_msg_length, out_msg);
	THREAD_ENTER;

	if (!status_vector[1])
		if (transaction && !handle) {
			release_transaction(transaction);
			transaction = NULL;
		}
		else if (!transaction && handle)
			transaction = make_transaction(rdb, handle);

	send_response(message, status_vector, transaction,
				  message->msg_trrq_out_msg_length, out_msg);
	free_buffer(out_msg, buffer_length);
}


#ifdef PIPE_SERVER
#ifdef VMS
static void trans_logicals(void)
{
/**************************************
 *
 *	t r a n s _ l o g i c a l s
 *
 **************************************
 *
 * Functional description
 *	Translate logicals and write their values to
 *	the process logical table.
 *
 **************************************/
	UCHAR **logicals, value[256], job_logical[32];
	int attr, status;
	SSHORT len;
	ITM items[2];
	struct dsc$descriptor_s tab_desc, log_desc;

	for (logicals = inherit_logicals; *logicals; logicals++) {
		items[0].itm_length = sizeof(value);
		items[0].itm_code = LNM$_STRING;
		items[0].itm_buffer = value;
		items[0].itm_return_length = &len;
		items[1].itm_length = 0;
		items[1].itm_code = 0;

		attr = LNM$M_CASE_BLIND;

		sprintf(job_logical, "GDS_PIPE_%s", *logicals);
		ISC_make_desc(job_logical, &log_desc, 0);

		ISC_make_desc("LNM$JOB", &tab_desc, sizeof("LNM$JOB") - 1);
		if (!(sys$trnlnm(&attr, &tab_desc, &log_desc, NULL, items) & 1))
			continue;

		/* Logical must be copied into the process logical table */

		items[0].itm_length = len;
		ISC_make_desc(*logicals, &log_desc, 0);
		ISC_make_desc("LNM$PROCESS", &tab_desc, sizeof("LNM$PROCESS") - 1);
		sys$crelnm(NULL, &tab_desc, &log_desc, NULL, items);
	}
}
#endif
#endif


static void unwind( MSG_OP message)
{
/**************************************
 *
 *	u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a request.
 *
 **************************************/
	RRQ request;
	STATUS status_vector[ISC_STATUS_LENGTH];

	request = (RRQ) message->msg_op_handle;

	THREAD_EXIT;
	GDS_UNWIND(status_vector,
			   GDS_REF(request->rrq_handle), (SSHORT) message->msg_op_level);
	THREAD_ENTER;

	send_response(message, status_vector, 0, 0, 0);
}
