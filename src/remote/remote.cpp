/*
 *	PROGRAM:	JRD Remote Interface
 *	MODULE:		remote.cpp
 *	DESCRIPTION:	Common routines for remote interface/server
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
#include <string.h>
#include <stdlib.h>
#include "../jrd/gds.h"
#include "../remote/remote.h"
#include "../jrd/file_params.h"
#include "../jrd/gdsassert.h"
#include "../jrd/isc.h"
#include "../remote/allr_proto.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
#include "../common/config/config.h"

#ifdef REMOTE_DEBUG
IMPLEMENT_TRACE_ROUTINE(remote_trace, "REMOTE");
#endif

int xdrmem_create(XDR *, SCHAR *, u_int, enum xdr_op);

extern "C" {

#define DUMMY_INTERVAL		60	/* seconds */
#define ATTACH_FAILURE_SPACE	2048	/* bytes */

static TEXT* attach_failures = NULL;
static TEXT* attach_failures_ptr;

static void cleanup_memory(void*);
static SLONG get_parameter(const UCHAR**);

void THD_init_data();


void REMOTE_cleanup_transaction( RTR transaction)
{
/**************************************
 *
 *	R E M O T E _ c l e a n u p _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	A transaction is being committed or rolled back.  
 *	Purge any active messages in case the user calls
 *	receive while we still have something cached.
 *
 **************************************/
	for (RRQ request = transaction->rtr_rdb->rdb_requests; request;
		 request = request->rrq_next) 
	{
		if (request->rrq_rtr == transaction) {
			REMOTE_reset_request(request, 0);
			request->rrq_rtr = NULL;
		}
		for (RRQ level = request->rrq_levels; level; level = level->rrq_next)
			if (level->rrq_rtr == transaction) {
				REMOTE_reset_request(level, 0);
				level->rrq_rtr = NULL;
			}
	}

	for (RSR statement = transaction->rtr_rdb->rdb_sql_requests; statement;
		 statement = statement->rsr_next)
	{
		if (statement->rsr_rtr == transaction) {
			REMOTE_reset_statement(statement);
			statement->rsr_flags &= ~RSR_fetched;
			statement->rsr_rtr = NULL;
		}
	}
}


ULONG REMOTE_compute_batch_size(PORT port,
								USHORT buffer_used, P_OP op_code, FMT format)
{
/**************************************
 *
 *	R E M O T E _ c o m p u t e _ b a t c h _ s i z e
 *
 **************************************
 *
 * Functional description
 *
 * When batches of records are returned, they are returned as
 *    follows:
 *     <op_fetch_response> <data_record 1>
 *     <op_fetch_response> <data_record 2>
 * 	...
 *     <op_fetch_response> <data_record n-1>
 *     <op_fetch_response> <data_record n>
 * 
 * end-of-batch is indicated by setting p_sqldata_messages to
 * 0 in the op_fetch_response.  End of cursor is indicated
 * by setting p_sqldata_status to a non-zero value.  Note
 * that a fetch CAN be attempted after end of cursor, this
 * is sent to the server for the server to return the appropriate
 * error code. 
 * 
 * Each data block has one overhead packet
 * to indicate the data is present.
 * 
 * (See also op_send in receive_msg() - which is a kissing cousin
 *  to this routine)
 * 
 * Here we make a guess for the optimal number of records to
 * send in each batch.  This is important as we wait for the
 * whole batch to be received before we return the first item
 * to the client program.  How many are cached on the client also
 * impacts client-side memory utilization.
 * 
 * We optimize the number by how many can fit into a packet.
 * The client calculates this number (n from the list above)
 * and sends it to the server.
 * 
 * I asked why it is that the client doesn't just ask for a packet 
 * full of records and let the server return however many fits in 
 * a packet.  According to Sudesh, this is because of a bug in 
 * Superserver which showed up in the WIN_NT 4.2.x kits.  So I 
 * imagine once we up the protocol so that we can be sure we're not 
 * talking to a 4.2 kit, then we can make this optimization. 
 *           - Deej 2/28/97
 * 
 * Note: A future optimization can look at setting the packet 
 * size to optimize the transfer.
 *
 * Note: This calculation must use worst-case to determine the
 * packing.  Should the data record have VARCHAR data, it is
 * often possible to fit more than the packing specification
 * into each packet.  This is also a candidate for future 
 * optimization.
 * 
 * The data size is either the XDR data representation, or the
 * actual message size (rounded up) if this is a symmetric
 * architecture connection. 
 * 
 **************************************/

#define	MAX_PACKETS_PER_BATCH	 4	/* packets    - picked by SWAG */
#define MIN_PACKETS_PER_BATCH	 2	/* packets    - picked by SWAG */
#define DESIRED_ROWS_PER_BATCH	20	/* data rows  - picked by SWAG */
#define MIN_ROWS_PER_BATCH	10	/* data rows  - picked by SWAG */

	USHORT op_overhead = (USHORT) xdr_protocol_overhead(op_code);
	ULONG row_size;

#ifdef DEBUG
	ib_fprintf(ib_stderr,
			   "port_buff_size = %d fmt_net_length = %d fmt_length = %d overhead = %d\n",
			   port->port_buff_size, format->fmt_net_length,
			   format->fmt_length, op_overhead);
#endif

	if (port->port_flags & PORT_symmetric) {
		/* Same architecture connection */
		row_size = (ROUNDUP(format->fmt_length, 4)
					+ op_overhead);
	}
	else {
		/* Using XDR for data transfer */
		row_size = (ROUNDUP(format->fmt_net_length, 4)
					+ op_overhead);
	}

	USHORT num_packets = (USHORT) (((DESIRED_ROWS_PER_BATCH * row_size)	/* data set */
							 + buffer_used	/* used in 1st pkt */
							 + (port->port_buff_size - 1))	/* to round up */
							/port->port_buff_size);
	if (num_packets > MAX_PACKETS_PER_BATCH) {
		num_packets = (USHORT) (((MIN_ROWS_PER_BATCH * row_size)	/* data set */
								 + buffer_used	/* used in 1st pkt */
								 + (port->port_buff_size - 1))	/* to round up */
								/port->port_buff_size);
	}
	num_packets = MAX(num_packets, MIN_PACKETS_PER_BATCH);

/* Now that we've picked the number of packets in a batch,
   pack as many rows as we can into the set of packets */

	ULONG result = (num_packets * port->port_buff_size - buffer_used) / row_size;

/* Must always send some messages, even if message size is more 
   than packet size. */

	result = MAX(result, MIN_ROWS_PER_BATCH);

#ifdef DEBUG
	{
		char* p = getenv("DEBUG_BATCH_SIZE");
		if (p)
			result = atoi(p);
		ib_fprintf(ib_stderr, "row_size = %lu num_packets = %d\n",
				   row_size, num_packets);
		ib_fprintf(ib_stderr, "result = %lu\n", result);
	}
#endif

	return result;
}


RRQ REMOTE_find_request(RRQ request, USHORT level)
{
/**************************************
 *
 *	R E M O T E _ f i n d _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Find sub-request if level is non-zero.
 *
 **************************************/

/* See if we already know about the request level */

	for (;;) {
		if (request->rrq_level == level)
			return request;
		if (!request->rrq_levels)
			break;
		request = request->rrq_levels;
	}

/* This is a new level -- make up a new request block. */

	request->rrq_levels = (RRQ) ALLR_clone(&request->rrq_header);
/* NOMEM: handled by ALLR_clone, FREE: REMOTE_remove_request() */
#ifdef DEBUG_REMOTE_MEMORY
	ib_printf("REMOTE_find_request       allocate request %x\n",
			  request->rrq_levels);
#endif
	request = request->rrq_levels;
	request->rrq_level = level;
	request->rrq_levels = NULL;

/* Allocate message block for known messages */

	rrq::rrq_repeat* tail = request->rrq_rpt;
	const rrq::rrq_repeat* const end = tail + request->rrq_max_msg;
	for (; tail <= end; tail++) {
		FMT format = tail->rrq_format;
		if (!format)
			continue;
		REM_MSG msg = (REM_MSG) ALLOCV(type_msg, format->fmt_length);
		tail->rrq_xdr = msg;
#ifdef DEBUG_REMOTE_MEMORY
		ib_printf("REMOTE_find_request       allocate message %x\n", msg);
#endif
		msg->msg_next = msg;
#ifdef SCROLLABLE_CURSORS
		msg->msg_prior = msg;
#endif
		msg->msg_number = tail->rrq_message->msg_number;
		tail->rrq_message = msg;
	}

	return request;
}


void REMOTE_free_packet( PORT port, PACKET * packet)
{
/**************************************
 *
 *	R E M O T E _ f r e e _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Zero out a packet block.
 *
 **************************************/
	XDR xdr;
	USHORT n;

	if (packet) {
		xdrmem_create(&xdr, reinterpret_cast < char *>(packet),
					  sizeof(PACKET), XDR_FREE);
		xdr.x_public = (caddr_t) port;

		for (n = (USHORT) op_connect; n < (USHORT) op_max; n++) {
			packet->p_operation = (P_OP) n;
			xdr_protocol(&xdr, packet);
		}
#ifdef DEBUG_XDR_MEMORY
		/* All packet memory allocations should now be voided. */

		for (n = 0; n < P_MALLOC_SIZE; n++)
			fb_assert(packet->p_malloc[n].p_operation == op_void);
#endif
		packet->p_operation = op_void;
	}
}


void REMOTE_get_timeout_params(
										  PORT port,
										  const UCHAR* dpb, USHORT dpb_length)
{
/**************************************
 *
 *	R E M O T E _ g e t _ t i m e o u t _ p a r a m s
 *
 **************************************
 *
 * Functional description
 *	Determine the connection timeout parameter values for this newly created
 *	port.  If the client did a specification in the DPB, use those values.
 *	Otherwise, see if there is anything in the configuration file.  The
 *	configuration file management code will set the default values if there
 *	is no other specification.
 *
 **************************************/
	bool got_dpb_connect_timeout = false;
	bool got_dpb_dummy_packet_interval = false;

	fb_assert(isc_dpb_connect_timeout == isc_spb_connect_timeout);
	fb_assert(isc_dpb_dummy_packet_interval == isc_spb_dummy_packet_interval);

	port->port_flags &= ~PORT_dummy_pckt_set;

	if (dpb && dpb_length) {
		const UCHAR* p = dpb;
		const UCHAR* const end = p + dpb_length;

		if (*p++ == gds_dpb_version1) {
			while (p < end)
				switch (*p++) {
				case gds_dpb_connect_timeout:
					port->port_connect_timeout = get_parameter(&p);
					got_dpb_connect_timeout = true;
					break;

// 22 Aug 2003. Do not receive this parameter from the client as dummy packets
//   either kill idle client process or cause unexpected disconnections. 
//   This applies to all IB/FB versions.
//				case gds_dpb_dummy_packet_interval:
//					port->port_dummy_packet_interval = get_parameter(&p);
//					got_dpb_dummy_packet_interval = true;
//					port->port_flags |= PORT_dummy_pckt_set;
//					break;

				case isc_dpb_sys_user_name:
			/** Store the user name in thread specific storage.
			We need this later while expanding filename to
			get the users home directory.
			Normally the working directory is stored in
			the attachment but in this case the attachment is 
			not yet created.
			Also note that the thread performing this task
			has already called THREAD_ENTER
		    **/
					{
						char* t_data;
						int i = 0;

						THD_init_data();

						int l = *(p++);
						if (l) {
							t_data = (char *) malloc(l + 1);
							do {
								t_data[i] = *p;
								if (t_data[i] == '.')
									t_data[i] = 0;
								i++;
								p++;
							} while (--l);
						}
						else
							t_data = (char *) malloc(1);
						t_data[i] = 0;


						THD_putspecific_data((void *) t_data);

					}
					break;

				default:
					{
						// Skip over this parameter - not important to us
						const USHORT len = *p++;
						p += len;
						break;
					}
				}
		}
	}

	if (!got_dpb_connect_timeout || !got_dpb_dummy_packet_interval) {
		/* Didn't find all parameters in the dpb, fetch configuration
		   information from the configuration file and set the missing
		   values */

		if (!got_dpb_connect_timeout)
			port->port_connect_timeout = Config::getConnectionTimeout();

		if (!got_dpb_dummy_packet_interval) {
			port->port_flags |= PORT_dummy_pckt_set;
			port->port_dummy_packet_interval = Config::getDummyPacketInterval();
		}
	}
/* Insure a meaningful keepalive interval has been set. Otherwise, too
   many keepalive packets will drain network performance. */

	if (port->port_dummy_packet_interval < 0)
		port->port_dummy_packet_interval = DUMMY_INTERVAL;

	port->port_dummy_timeout = port->port_dummy_packet_interval;
#ifdef DEBUG
	ib_printf("REMOTE_get_timeout dummy = %lu conn = %lu\n",
			  port->port_dummy_packet_interval, port->port_connect_timeout);
	ib_fflush(ib_stdout);
#endif
}


STR REMOTE_make_string(const SCHAR* input)
{
/**************************************
 *
 *	R E M O T E _ m a k e _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Copy a given string to a permanent location, returning
 *	address of new string.
 *
 **************************************/
	const USHORT length = strlen(input);
	STR string = (STR) ALLOCV(type_str, length);
#ifdef DEBUG_REMOTE_MEMORY
	ib_printf("REMOTE_make_string        allocate string  %x\n", string);
#endif
	strcpy(string->str_data, input);
	string->str_length = length;

	return string;
}


void REMOTE_release_messages( REM_MSG messages)
{
/**************************************
 *
 *	R E M O T E _ r e l e a s e _ m e s s a g e s
 *
 **************************************
 *
 * Functional description
 *	Release a circular list of messages.
 *
 **************************************/
	REM_MSG message = messages;
	if (message)
		while (true) {
			REM_MSG temp = message;
			message = message->msg_next;
#ifdef DEBUG_REMOTE_MEMORY
			ib_printf("REMOTE_release_messages   free message     %x\n",
					  temp);
#endif
			ALLR_release(temp);
			if (message == messages)
				break;
		}
}


void REMOTE_release_request( RRQ request)
{
/**************************************
 *
 *	R E M O T E _ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request block and friends.
 *
 **************************************/
	RDB rdb = request->rrq_rdb;

	for (RRQ* p = &rdb->rdb_requests; *p; p = &(*p)->rrq_next)
		if (*p == request) {
			*p = request->rrq_next;
			break;
		}

/* Get rid of request and all levels */

	for (;;) {
		rrq::rrq_repeat* tail = request->rrq_rpt;
		rrq::rrq_repeat* const end = tail + request->rrq_max_msg;
		for (; tail <= end; tail++)
		{
		    REM_MSG message = tail->rrq_message;
			if (message) {
				if (!request->rrq_level) {
#ifdef DEBUG_REMOTE_MEMORY
					ib_printf
						("REMOTE_release_request    free format      %x\n",
						 tail->rrq_format);
#endif
					ALLR_release(tail->rrq_format);
				}
				REMOTE_release_messages(message);
			}
		}
		RRQ next = request->rrq_levels;
#ifdef DEBUG_REMOTE_MEMORY
		ib_printf("REMOTE_release_request    free request     %x\n", request);
#endif
		ALLR_release(request);
		if (!(request = next))
			break;
	}
}


void REMOTE_reset_request( RRQ request, REM_MSG active_message)
{
/**************************************
 *
 *	R E M O T E _ r e s e t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Clean up a request in preparation to use it again.  Since
 *	there may be an active message (start_and_send), exercise
 *	some care to avoid zapping that message.
 *
 **************************************/
	rrq::rrq_repeat* tail = request->rrq_rpt;
	rrq::rrq_repeat* end = tail + request->rrq_max_msg;
	for (; tail <= end; tail++) {
	    REM_MSG message = tail->rrq_message;
		if (message != NULL && message != active_message) {
			tail->rrq_xdr = message;
			tail->rrq_rows_pending = 0;
			tail->rrq_reorder_level = 0;
			tail->rrq_batch_count = 0;
			while (true) {
				message->msg_address = NULL;
				message = message->msg_next;
				if (message == tail->rrq_message)
					break;
			}
		}
	}

/* Initialize the request status to FB_SUCCESS */

	request->rrq_status_vector[1] = 0;
}


void REMOTE_reset_statement( RSR statement)
{
/**************************************
 *
 *	R E M O T E _ r e s e t _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Reset a statement by releasing all buffers except 1
 *
 **************************************/
	REM_MSG message;

	if ((!statement) || (!(message = statement->rsr_message)))
		return;

/* Reset all the pipeline counters */

	statement->rsr_rows_pending = 0;
	statement->rsr_msgs_waiting = 0;
	statement->rsr_reorder_level = 0;
	statement->rsr_batch_count = 0;

/* only one entry */

	if (message->msg_next == message)
		return;

/* find the entry before statement->rsr_message */

	REM_MSG temp;
	for (temp = message->msg_next; temp->msg_next != message;
		 temp = temp->msg_next);

	temp->msg_next = message->msg_next;
	message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
	message->msg_prior = message;
#endif

	statement->rsr_buffer = statement->rsr_message;

	REMOTE_release_messages(temp);
}


void REMOTE_save_status_strings( ISC_STATUS* vector)
{
/**************************************
 *
 *	R E M O T E _ s a v e _ s t a t u s _ s t r i n g s
 *
 **************************************
 *
 * Functional description
 *	There has been a failure during attach/create database.
 *	The included string have been allocated off of the database block,
 *	which is going to be released before the message gets passed 
 *	back to the user.  So, to preserve information, copy any included
 *	strings to a special buffer.
 *
 **************************************/
	if (!attach_failures)
	{
		attach_failures =
			(TEXT*) ALLOC_LIB_MEMORY((SLONG) ATTACH_FAILURE_SPACE);
		/* FREE: freed by exit handler cleanup_memory() */
		if (!attach_failures)	/* NOMEM: don't bother trying to copy */
			return;
#ifdef DEBUG_GDS_ALLOC
		/* This buffer is freed by the exit handler - but some of the
		 * reporting mechanisms will report it anyway, so flag it as
		 * "known unfreed" to get the reports quiet.
		 */
		gds_alloc_flag_unfreed((void*) attach_failures);
#endif	/* DEBUG_GDS_ALLOC */
		attach_failures_ptr = attach_failures;
		gds__register_cleanup(cleanup_memory, 0);
	}

	TEXT* p;
	USHORT l;
	while (*vector)
	{
		const ISC_STATUS status = *vector++;
		switch (status)
		{
		case gds_arg_cstring:
			l = (USHORT) *vector++;

		case gds_arg_interpreted:
		case gds_arg_string:
			p = (TEXT*) *vector;
			if (status != gds_arg_cstring)
				l = strlen(p) + 1;

			/* If there isn't any more room in the buffer,
			   start at the beginning again */

			if (attach_failures_ptr + l > attach_failures + ATTACH_FAILURE_SPACE)
				attach_failures_ptr = attach_failures;
			*vector++ = (ISC_STATUS) attach_failures_ptr;
			while (l--)
				*attach_failures_ptr++ = *p++;
			break;

		default:
			++vector;
			break;
		}
	}
}


OBJCT REMOTE_set_object(PORT port, BLK object, OBJCT slot)
{
/**************************************
 *
 *	R E M O T E _ s e t _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Set an object into the object vector.
 *
 **************************************/

/* If it fits, do it */

	VEC vector = port->port_object_vector;
	if ((vector != NULL) && slot < vector->vec_count) {
		vector->vec_object[slot] = object;
		return slot;
	}

/* Prevent the creation of object handles that can't be
   transferred by the remote protocol. */

	if (slot + 10 > MAX_OBJCT_HANDLES)
		return (OBJCT) NULL;

	VEC new_vector = (VEC) ALLOCV(type_vec, slot + 10);
	port->port_object_vector = new_vector;
#ifdef DEBUG_REMOTE_MEMORY
	ib_printf("REMOTE_set_object         allocate vector  %x\n", new_vector);
#endif
	port->port_objects = new_vector->vec_object;
	new_vector->vec_count = slot + 10;

	if (vector) {
		blk** p = new_vector->vec_object;
		blk* const* q = vector->vec_object;
		const blk* const* const end = q + (int) vector->vec_count;
		while (q < end)
			*p++ = *q++;
#ifdef DEBUG_REMOTE_MEMORY
		ib_printf("REMOTE_release_request    free vector      %x\n", vector);
#endif
		ALLR_release(vector);
	}

	new_vector->vec_object[slot] = object;

	return slot;
}


static void cleanup_memory( void *block)
{
/**************************************
 *
 *	c l e a n u p _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *	Cleanup any allocated memory.
 *
 **************************************/

	if (attach_failures)
		FREE_LIB_MEMORY(attach_failures);

	gds__unregister_cleanup(cleanup_memory, 0);
	attach_failures = NULL;
}


static SLONG get_parameter(const UCHAR** ptr)
{
/**************************************
 *
 *	g e t _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *	Pick up a VAX format parameter from a parameter block, including the
 *	length byte.
 *	This is a clone of jrd/jrd.c:get_parameter()
 *
 **************************************/
	const SSHORT l = *(*ptr)++;
	const SLONG parameter = gds__vax_integer(*ptr, l);
	*ptr += l;

	return parameter;
}


} // extern "C"


// TMN: Beginning of C++ port - ugly but a start

int port::accept(p_cnct* cnct)
{
	return (*this->port_accept)(this, cnct);
}

void port::disconnect()
{
	(*this->port_disconnect)(this);
}

port* port::receive(PACKET* pckt)
{
	return (*this->port_receive_packet)(this, pckt);
}

XDR_INT port::send(PACKET* pckt)
{
	return (*this->port_send_packet)(this, pckt);
}

XDR_INT port::send_partial(PACKET* pckt)
{
	return (*this->port_send_partial)(this, pckt);
}

port* port::connect(PACKET* pckt, void(*ast)())
{
	return (*this->port_connect)(this, pckt, ast);
}

port* port::request(PACKET* pckt)
{
	return (*this->port_request)(this, pckt);
}

