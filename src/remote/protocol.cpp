/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		protocol.cpp
 *	DESCRIPTION:	Protocol data structure mapper
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#include <string.h>
#include "../remote/remote.h"
#include "gen/iberror.h"
#include "../jrd/sdl.h"
#include "../jrd/gdsassert.h"
#include "../remote/parse_proto.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sdl_proto.h"

#ifdef DEBUG_XDR_MEMORY
inline bool_t P_TRUE(XDR* xdrs, PACKET* p) {
	return xdr_debug_packet(xdrs, XDR_FREE, p);
}
inline bool_t P_FALSE(XDR* xdrs, PACKET* p) {
	return !xdr_debug_packet(xdrs, XDR_FREE, p);
}
inline void DEBUG_XDR_PACKET(XDR* xdrs, PACKET* p) {
	xdr_debug_packet(xdrs, XDR_DECODE, p);
}
inline void DEBUG_XDR_ALLOC(XDR* xdrs, const void* xdrvar, const void* addr, ULONG len) {
	xdr_debug_memory(xdrs, XDR_DECODE, xdrvar, addr, len);
}
inline void DEBUG_XDR_FREE(XDR* xdrs, const void* xdrvar, const void* addr, ULONG len) {
	xdr_debug_memory(xdrs, XDR_DECODE, xdrvar, addr, len);
}
#else
inline bool_t P_TRUE(XDR* xdrs, PACKET* p) {
	return TRUE;
}
inline bool_t P_FALSE(XDR* xdrs, PACKET* p) {
	return FALSE;
}
inline void DEBUG_XDR_PACKET(XDR* xdrs, PACKET* p) {
}
inline void DEBUG_XDR_ALLOC(XDR* xdrs, const void* xdrvar, const void* addr, ULONG len) {
}
inline void DEBUG_XDR_FREE(XDR* xdrs, const void* xdrvar, const void* addr, ULONG len) {
}
#endif /* DEBUG_XDR_MEMORY */

#define MAP(routine, ptr)	if (!routine (xdrs, &ptr)) return P_FALSE(xdrs, p);
const ULONG MAX_OPAQUE		= 32768;

typedef enum {
	TYPE_IMMEDIATE,
	TYPE_PREPARED
} SQL_STMT_TYPE;

static bool alloc_cstring(XDR *, CSTRING *);
static void free_cstring(XDR *, CSTRING *);
static RSR get_statement(XDR *, SSHORT);
static bool_t xdr_cstring(XDR*, CSTRING*);
static inline bool_t xdr_cstring_const(XDR*, CSTRING_CONST*);
static bool_t xdr_datum(XDR *, DSC *, BLOB_PTR *);
#ifdef DEBUG_XDR_MEMORY
static bool_t xdr_debug_packet(XDR *, enum xdr_op, PACKET *);
#endif
static bool_t xdr_longs(XDR *, CSTRING *);
static bool_t xdr_message(XDR *, REM_MSG, rem_fmt*);
static bool_t xdr_quad(XDR *, struct bid *);
static bool_t xdr_request(XDR *, USHORT, USHORT, USHORT);
#ifdef VMS
static bool_t xdr_semi_opaque(XDR *, REM_MSG, rem_fmt*);
static bool_t xdr_semi_opaque_slice(XDR *, lstring *);
#endif
static bool_t xdr_slice(XDR*, lstring*, USHORT, const UCHAR*);
static bool_t xdr_status_vector(XDR *, ISC_STATUS *, TEXT * strings[]);
static bool_t xdr_sql_blr(XDR *, SLONG, CSTRING *, int, SQL_STMT_TYPE);
static bool_t xdr_sql_message(XDR *, SLONG);
static bool_t xdr_trrq_blr(XDR *, CSTRING *);
static bool_t xdr_trrq_message(XDR *, USHORT);

#ifdef NOT_USED_OR_REPLACED
// TMN: Patched away this for now, it should probably be removed.
// Now why the ... would anyone want to define functions differently
// in an implementation file than they  are defined in their header?!

bool_t xdr_enum();
bool_t xdr_short();
bool_t xdr_u_short();
bool_t xdr_long();
#  ifdef SOLARIS
bool_t xdr_hyper();
#  endif
bool_t xdr_opaque();
bool_t xdr_string();
bool_t xdr_float();
bool_t xdr_double();
bool_t xdr_wrapstring();

bool_t xdr_free();

#else // NOT_USED_OR_REPLACED

#  include "../remote/xdr_proto.h"

#endif // NOT_USED_OR_REPLACED


#ifdef VMS
double MTH$CVT_D_G(), MTH$CVT_G_D();

static rem_str* gfloat_buffer;
#endif



#ifdef DEBUG
static ULONG xdr_save_size = 0;
inline void DEBUG_PRINTSIZE(XDR* xdrs, P_OP p) {
	fprintf (stderr, "xdr_protocol: %s op %d size %lu\n",
		((xdrs->x_op == XDR_FREE)   ? "free" :
		 (xdrs->x_op == XDR_ENCODE) ? "enc " :
		 (xdrs->x_op == XDR_DECODE) ? "dec " : "othr"), p,
		((xdrs->x_op == XDR_ENCODE) ? (xdrs->x_handy - xdr_save_size)
		 : (xdr_save_size - xdrs->x_handy)));
}
#else
inline void DEBUG_PRINTSIZE(XDR* xdrs, P_OP p) {
}
#endif


#ifdef DEBUG_XDR_MEMORY
void xdr_debug_memory(
					  XDR* xdrs,
					  enum xdr_op xop,
					  const void* xdrvar, const void* address, ULONG length)
{
/**************************************
 *
 *	x d r _ d e b u g _ m e m o r y
 *
 **************************************
 *
 * Functional description
 *	Track memory allocation patterns of XDR aggregate
 *	types (i.e. xdr_cstring, xdr_string, etc.) to
 *	validate that memory is not leaked by overwriting
 *	XDR aggregate pointers and that freeing a packet
 *	with REMOTE_free_packet() does not miss anything.
 *
 *	All memory allocations due to marshalling XDR
 *	variables are recorded in a debug memory alloca-
 *	tion table stored at the front of a packet.
 *
 *	Once a packet is being tracked it is an assertion
 *	error if a memory allocation can not be recorded
 *	due to space limitations or if a previous memory
 *	allocation being freed cannot be found. At most
 *	P_MALLOC_SIZE entries can be stored in the memory
 *	allocation table. A rough estimate of the number
 *	of XDR aggregates that can hang off a packet can
 *	be obtained by examining the subpackets defined
 *	in <remote/protocol.h>: A guestimate of 36 at this
 *	time includes 10 strings used to decode an xdr
 *	status vector.
 *
 **************************************/
	rem_port* port = (rem_port*) xdrs->x_public;
	fb_assert(port != 0);
	fb_assert(port->port_header.blk_type == type_port);

/* Compare the XDR variable address with the lower and upper bounds
   of each packet to determine which packet contains it. Record or
   delete an entry in that packet's memory allocation table. */

	rem_vec* vector = port->port_packet_vector;
	if (!vector)	/* Not tracking port's protocol */
		return;

	ULONG i;
	for (i = 0; i < vector->vec_count; i++) {
		PACKET* packet = (PACKET*) vector->vec_object[i];
		if (packet) {
			fb_assert(packet->p_operation > op_void
				   && packet->p_operation < op_max);

			if ((SCHAR *) xdrvar >= (SCHAR *) packet &&
				(SCHAR *) xdrvar < (SCHAR *) packet + sizeof(PACKET))
			{
				ULONG j;
				for (j = 0; j < P_MALLOC_SIZE; j++) {
					if (xop == XDR_FREE) {
						if ((SCHAR *) packet->p_malloc[j].p_address ==
							(SCHAR *) address)
						{
							packet->p_malloc[j].p_operation = op_void;
							packet->p_malloc[j].p_allocated = NULL;
							packet->p_malloc[j].p_address = 0;
							/*  packet->p_malloc [j].p_xdrvar = 0; */
							return;
						}
					}
					else {		/* XDR_ENCODE or XDR_DECODE */

						fb_assert(xop == XDR_ENCODE || xop == XDR_DECODE);
						if (packet->p_malloc[j].p_operation == op_void) {
							packet->p_malloc[j].p_operation =
								packet->p_operation;
							packet->p_malloc[j].p_allocated = length;
							packet->p_malloc[j].p_address = address;
							/*  packet->p_malloc [j].p_xdrvar = xdrvar; */
							return;
						}
					}
				}
				/* Assertion failure if not enough entries to record every xdr
				   memory allocation or an entry to be freed can't be found. */

				fb_assert(j < P_MALLOC_SIZE);	/* Increase P_MALLOC_SIZE if necessary */
			}
		}
	}
	fb_assert(i < vector->vec_count);	/* Couldn't find packet for this xdr arg */
}
#endif


bool_t xdr_protocol(XDR* xdrs, PACKET* p)
{
/**************************************
 *
 *	x d r _ p r o t o c o l
 *
 **************************************
 *
 * Functional description
 *	Encode, decode, or free a protocol packet.
 *
 **************************************/
	USHORT i;
	p_cnct::p_cnct_repeat * tail;
	rem_port* port;
	P_CNCT *connect;
	P_ACPT *accept;
	P_ATCH *attach;
	P_RESP *response;
	P_CMPL *compile;
	P_STTR *transaction;
	P_DATA *data;
	P_RLSE *release;
	P_BLOB *blob;
	P_SGMT *segment;
	P_INFO *info;
	P_PREP *prepare;
	P_EVENT *event;
	P_REQ *request;
	P_DDL *ddl;
	P_SLC *slice;
	P_SLR *slice_response;
	P_SEEK *seek;
	P_SQLFREE *free_stmt;
	P_SQLCUR *sqlcur;
	P_SQLST *prep_stmt;
	P_SQLDATA *sqldata;
	P_TRRQ *trrq;
#ifdef DEBUG
	xdr_save_size = xdrs->x_handy;
#endif

	DEBUG_XDR_PACKET(xdrs, p);

	if (!xdr_enum(xdrs, reinterpret_cast<xdr_op*>(&p->p_operation)))
		return P_FALSE(xdrs, p);

	switch (p->p_operation) {
	case op_reject:
	case op_disconnect:
	case op_dummy:
		return P_TRUE(xdrs, p);

	case op_connect:
		connect = &p->p_cnct;
		MAP(xdr_enum,
			reinterpret_cast<xdr_op&>(connect->p_cnct_operation));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(connect->p_cnct_cversion));
		MAP(xdr_enum, reinterpret_cast<xdr_op&>(connect->p_cnct_client));
		MAP(xdr_cstring, connect->p_cnct_file);
		MAP(xdr_short, reinterpret_cast<SSHORT&>(connect->p_cnct_count));
		MAP(xdr_cstring, connect->p_cnct_user_id);
		for (i = 0, tail = connect->p_cnct_versions;
			 i < connect->p_cnct_count; i++, tail++)
		{
			MAP(xdr_short,
				reinterpret_cast<SSHORT&>(tail->p_cnct_version));
			MAP(xdr_enum,
				reinterpret_cast<xdr_op&>(tail->p_cnct_architecture));
			MAP(xdr_u_short, tail->p_cnct_min_type);
			MAP(xdr_u_short, tail->p_cnct_max_type);
			MAP(xdr_short,
				reinterpret_cast<SSHORT&>(tail->p_cnct_weight));
		}
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_accept:
		accept = &p->p_acpt;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(accept->p_acpt_version));
		MAP(xdr_enum,
			reinterpret_cast<xdr_op&>(accept->p_acpt_architecture));
		MAP(xdr_u_short, accept->p_acpt_type);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_connect_request:
	case op_aux_connect:
		request = &p->p_req;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(request->p_req_type));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(request->p_req_object));
		MAP(xdr_long, reinterpret_cast<SLONG&>(request->p_req_partner));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_attach:
	case op_create:
	case op_service_attach:
		attach = &p->p_atch;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(attach->p_atch_database));
		MAP(xdr_cstring, attach->p_atch_file);
		MAP(xdr_cstring, attach->p_atch_dpb);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_compile:
		compile = &p->p_cmpl;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(compile->p_cmpl_database));
		MAP(xdr_cstring, compile->p_cmpl_blr);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_receive:
	case op_start:
	case op_start_and_receive:
		data = &p->p_data;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(data->p_data_request));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_incarnation));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_transaction));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_message_number));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(data->p_data_messages));
#ifdef SCROLLABLE_CURSORS
		port = (rem_port*) xdrs->x_public;
		if ((p->p_operation == op_receive) &&
			(port->port_protocol > PROTOCOL_VERSION8))
		{
			MAP(xdr_short,
				reinterpret_cast<SSHORT&>(data->p_data_direction));
			MAP(xdr_long, data->p_data_offset);
		}

#endif
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_send:
	case op_start_and_send:
	case op_start_send_and_receive:
		data = &p->p_data;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(data->p_data_request));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_incarnation));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_transaction));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(data->p_data_message_number));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(data->p_data_messages));

		/* Changes to this op's protocol must mirror in xdr_protocol_overhead */

		return xdr_request(xdrs, data->p_data_request,
						   data->p_data_message_number,
						   data->p_data_incarnation) ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);

	case op_response:
	case op_response_piggyback:

		/* Changes to this op's protocol must be mirrored
		   in xdr_protocol_overhead */

		response = &p->p_resp;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(response->p_resp_object));
		MAP(xdr_quad, response->p_resp_blob_id);
		MAP(xdr_cstring, response->p_resp_data);
		return xdr_status_vector(xdrs, response->p_resp_status_vector,
								 reinterpret_cast<char**>(response->p_resp_strings))
								 ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);

	case op_transact:
		trrq = &p->p_trrq;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(trrq->p_trrq_database));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(trrq->p_trrq_transaction));
		xdr_trrq_blr(xdrs, &trrq->p_trrq_blr);
		MAP(xdr_cstring, trrq->p_trrq_blr);
		MAP(xdr_short, reinterpret_cast<SSHORT&>(trrq->p_trrq_messages));
		if (trrq->p_trrq_messages)
			return xdr_trrq_message(xdrs, 0) ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_transact_response:
		data = &p->p_data;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(data->p_data_messages));
		if (data->p_data_messages)
			return xdr_trrq_message(xdrs, 1) ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_open_blob2:
	case op_create_blob2:
		blob = &p->p_blob;
		MAP(xdr_cstring_const, blob->p_blob_bpb);
		// fall into:

	case op_open_blob:
	case op_create_blob:
		blob = &p->p_blob;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(blob->p_blob_transaction));
		MAP(xdr_quad, blob->p_blob_id);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_get_segment:
	case op_put_segment:
	case op_batch_segments:
		segment = &p->p_sgmt;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(segment->p_sgmt_blob));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(segment->p_sgmt_length));
		MAP(xdr_cstring_const, segment->p_sgmt_segment);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_seek_blob:
		seek = &p->p_seek;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(seek->p_seek_blob));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(seek->p_seek_mode));
		MAP(xdr_long, seek->p_seek_offset);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_reconnect:
	case op_transaction:
		transaction = &p->p_sttr;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(transaction->p_sttr_database));
		MAP(xdr_cstring, transaction->p_sttr_tpb);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_info_blob:
	case op_info_database:
	case op_info_request:
	case op_info_transaction:
	case op_service_info:
	case op_info_sql:
		info = &p->p_info;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(info->p_info_object));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(info->p_info_incarnation));
		MAP(xdr_cstring, info->p_info_items);
		if (p->p_operation == op_service_info)
			MAP(xdr_cstring, info->p_info_recv_items);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(info->p_info_buffer_length));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_service_start:
		info = &p->p_info;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(info->p_info_object));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(info->p_info_incarnation));
		MAP(xdr_cstring, info->p_info_items);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_commit:
	case op_prepare:
	case op_rollback:
	case op_unwind:
	case op_release:
	case op_close_blob:
	case op_cancel_blob:
	case op_detach:
	case op_drop_database:
	case op_service_detach:
	case op_commit_retaining:
	case op_rollback_retaining:
	case op_allocate_statement:
		release = &p->p_rlse;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(release->p_rlse_object));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_prepare2:
		prepare = &p->p_prep;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prepare->p_prep_transaction));
		MAP(xdr_cstring, prepare->p_prep_data);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_que_events:
	case op_event:
		{
			event = &p->p_event;
			MAP(xdr_short,
				reinterpret_cast<SSHORT&>(event->p_event_database));
			MAP(xdr_cstring, event->p_event_items);
			
			// Nickolay Samofatov: these values are parsed, but are ignored by the client.
			// Values are useful only for debugging, anyway since upper words of pointers
			// are trimmed for 64-bit clients
			MAP(xdr_long, reinterpret_cast<SLONG&>(event->p_event_ast));
			MAP(xdr_long, event->p_event_arg);
			
			MAP(xdr_long, event->p_event_rid);
			DEBUG_PRINTSIZE(xdrs, p->p_operation);
			return P_TRUE(xdrs, p);
		}

	case op_cancel_events:
		event = &p->p_event;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(event->p_event_database));
		MAP(xdr_long, event->p_event_rid);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_ddl:
		ddl = &p->p_ddl;
		MAP(xdr_short, reinterpret_cast<SSHORT&>(ddl->p_ddl_database));
		MAP(xdr_short, reinterpret_cast<SSHORT&>(ddl->p_ddl_transaction));
		MAP(xdr_cstring_const, ddl->p_ddl_blr);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_get_slice:
	case op_put_slice:
		slice = &p->p_slc;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(slice->p_slc_transaction));
		MAP(xdr_quad, slice->p_slc_id);
		MAP(xdr_long, reinterpret_cast<SLONG&>(slice->p_slc_length));
		MAP(xdr_cstring, slice->p_slc_sdl);
		MAP(xdr_longs, slice->p_slc_parameters);
		slice_response = &p->p_slr;
		if (slice_response->p_slr_sdl) {
			if (!xdr_slice(xdrs, &slice->p_slc_slice, slice_response->p_slr_sdl_length,
						   slice_response->p_slr_sdl)) 
			{
				return P_FALSE(xdrs, p);
			}
		}
		else
			if (!xdr_slice(xdrs, &slice->p_slc_slice, slice->p_slc_sdl.cstr_length,
						   slice->p_slc_sdl.cstr_address)) 
			{
				return P_FALSE(xdrs, p);
			}
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_slice:
		slice_response = &p->p_slr;
		MAP(xdr_long,
			reinterpret_cast<SLONG&>(slice_response->p_slr_length));
		if (!xdr_slice (xdrs, &slice_response->p_slr_slice, slice_response->p_slr_sdl_length,
			 slice_response->p_slr_sdl)) 
		{
			return P_FALSE(xdrs, p);
		}
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_execute:
	case op_execute2:
		sqldata = &p->p_sqldata;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_statement));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_transaction));
		if (xdrs->x_op == XDR_DECODE) {
			/* the statement should be reset for each execution so that
			   all prefetched information from a prior execute is properly
			   cleared out.  This should be done before fetching any message
			   information (for example: blr info)
			 */

			RSR statement = NULL;
			statement = get_statement(xdrs, sqldata->p_sqldata_statement);
			if (statement)
				REMOTE_reset_statement(statement);
		}

		xdr_sql_blr(xdrs, (SLONG) sqldata->p_sqldata_statement,
					&sqldata->p_sqldata_blr, FALSE, TYPE_PREPARED);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_message_number));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_messages));
		if (sqldata->p_sqldata_messages) {
			if (!xdr_sql_message(xdrs, (SLONG) sqldata->p_sqldata_statement))
				return P_FALSE(xdrs, p);
		}
		if (p->p_operation == op_execute2) {
			xdr_sql_blr(xdrs, (SLONG) - 1, &sqldata->p_sqldata_out_blr, TRUE,
						TYPE_PREPARED);
			MAP(xdr_short,
				reinterpret_cast<SSHORT&>(sqldata->p_sqldata_out_message_number));
		}
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_exec_immediate2:
		prep_stmt = &p->p_sqlst;
		xdr_sql_blr(xdrs, (SLONG) - 1, &prep_stmt->p_sqlst_blr, FALSE,
					TYPE_IMMEDIATE);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_message_number));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_messages));
		if (prep_stmt->p_sqlst_messages) {
			if (!xdr_sql_message(xdrs, (SLONG) - 1))
				return P_FALSE(xdrs, p);
		}
		xdr_sql_blr(xdrs, (SLONG) - 1, &prep_stmt->p_sqlst_out_blr, TRUE,
					TYPE_IMMEDIATE);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_out_message_number));
		/* Fall into ... */

	case op_exec_immediate:
	case op_prepare_statement:
		prep_stmt = &p->p_sqlst;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_transaction));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_statement));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_SQL_dialect));
		MAP(xdr_cstring, prep_stmt->p_sqlst_SQL_str);
		MAP(xdr_cstring, prep_stmt->p_sqlst_items);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(prep_stmt->p_sqlst_buffer_length));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_fetch:
		sqldata = &p->p_sqldata;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_statement));
		xdr_sql_blr(xdrs, (SLONG) sqldata->p_sqldata_statement,
					&sqldata->p_sqldata_blr, TRUE, TYPE_PREPARED);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_message_number));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_messages));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_fetch_response:
		sqldata = &p->p_sqldata;
		MAP(xdr_long,
			reinterpret_cast<SLONG&>(sqldata->p_sqldata_status));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_messages));

		/* Changes to this op's protocol must mirror in xdr_protocol_overhead */

		port = (rem_port*) xdrs->x_public;
		if (
			(port->port_protocol > PROTOCOL_VERSION7
			 && sqldata->p_sqldata_messages)
			|| (port->port_protocol <= PROTOCOL_VERSION7
				&& !sqldata->p_sqldata_status))
		{
			return xdr_sql_message(xdrs, (SLONG)sqldata->p_sqldata_statement) ?
				P_TRUE(xdrs, p) : P_FALSE(xdrs, p);
		}
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_free_statement:
		free_stmt = &p->p_sqlfree;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(free_stmt->p_sqlfree_statement));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(free_stmt->p_sqlfree_option));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_insert:
		sqldata = &p->p_sqldata;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_statement));
		xdr_sql_blr(xdrs, (SLONG) sqldata->p_sqldata_statement,
					&sqldata->p_sqldata_blr, FALSE, TYPE_PREPARED);
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_message_number));
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_messages));
		if (sqldata->p_sqldata_messages)
			return xdr_sql_message(xdrs,
								   (SLONG) sqldata->
								   p_sqldata_statement) ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_set_cursor:
		sqlcur = &p->p_sqlcur;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqlcur->p_sqlcur_statement));
		MAP(xdr_cstring, sqlcur->p_sqlcur_cursor_name);
		MAP(xdr_short, reinterpret_cast<SSHORT&>(sqlcur->p_sqlcur_type));
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	case op_sql_response:
		sqldata = &p->p_sqldata;
		MAP(xdr_short,
			reinterpret_cast<SSHORT&>(sqldata->p_sqldata_messages));
		if (sqldata->p_sqldata_messages)
			return xdr_sql_message(xdrs, (SLONG) - 1) ? P_TRUE(xdrs, p) : P_FALSE(xdrs, p);
		DEBUG_PRINTSIZE(xdrs, p->p_operation);
		return P_TRUE(xdrs, p);

	default:
#ifdef DEBUG
		if (xdrs->x_op != XDR_FREE)
			fprintf(stderr, "xdr_packet: operation %d not recognized\n",
					   p->p_operation);
#endif
		fb_assert(xdrs->x_op == XDR_FREE);
		return P_FALSE(xdrs, p);
	}
}


ULONG xdr_protocol_overhead(P_OP op)
{
/**************************************
 *
 *	x d r _ p r o t o c o l _ o v e r h e a d
 *
 **************************************
 *
 * Functional description
 *	Report the overhead size of a particular packet.
 *	NOTE: This is not the same as the actual size to
 *	send the packet - as this figure discounts any data
 *	to be sent with the packet.  It's purpose is to figure
 *	overhead for deciding on a batching window count.
 *
 *	A better version of this routine would use xdr_sizeof - but
 *	it is unknown how portable that Solaris call is to other
 *	OS.
 *
 **************************************/
	ULONG size = 4 /* xdr_sizeof (xdr_enum, p->p_operation) */ ;

	switch (op) {
	case op_fetch_response:
		size += 4				/* xdr_sizeof (xdr_long, sqldata->p_sqldata_status) */
			+ 4 /* xdr_sizeof (xdr_short, sqldata->p_sqldata_messages) */ ;
		break;

	case op_send:
	case op_start_and_send:
	case op_start_send_and_receive:
		size += 4				/* xdr_sizeof (xdr_short, data->p_data_request) */
			+ 4					/* xdr_sizeof (xdr_short, data->p_data_incarnation) */
			+ 4					/* xdr_sizeof (xdr_short, data->p_data_transaction) */
			+ 4					/* xdr_sizeof (xdr_short, data->p_data_message_number) */
			+ 4 /* xdr_sizeof (xdr_short, data->p_data_messages) */ ;
		break;

	case op_response:
	case op_response_piggyback:
		/* Note: minimal amounts are used for cstring & status_vector */
		size += 4				/* xdr_sizeof (xdr_short, response->p_resp_object) */
			+ 8					/* xdr_sizeof (xdr_quad, response->p_resp_blob_id) */
			+ 4					/* xdr_sizeof (xdr_cstring, response->p_resp_data) */
			+
			3 *
			4
			/* xdr_sizeof (xdr_status_vector (xdrs, response->p_resp_status_vector */
			;
		break;

	default:
		fb_assert(FALSE);			/* Not supported operation */
		return 0;
	}
	return size;
}


static bool alloc_cstring(XDR* xdrs,
						  CSTRING* cstring)
{
/**************************************
 *
 *	a l l o c _ c s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Handle allocation for cstring.
 *
 **************************************/

	if (!cstring->cstr_length)
		return true;

	if (cstring->cstr_length > cstring->cstr_allocated &&
		cstring->cstr_allocated) 
	{
		free_cstring(xdrs, cstring);
	}

	if (!cstring->cstr_address) {
		// fb_assert(!cstring->cstr_allocated);
		if (!
			(cstring->cstr_address =
			 ALLR_alloc((SLONG) cstring->cstr_length)))
		{
			/* NOMEM: handled by ALLR_alloc() */
			/* FREE:  in realloc case above & free_cstring() */
			return false;
		}

		cstring->cstr_allocated = cstring->cstr_length;
		DEBUG_XDR_ALLOC(xdrs, cstring, cstring->cstr_address,
						cstring->cstr_allocated);
	}

	return true;
}


static void free_cstring( XDR* xdrs, CSTRING* cstring)
{
/**************************************
 *
 *	f r e e _ c s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Free any memory allocated for a cstring.
 *
 **************************************/

	if (cstring->cstr_allocated) {
		ALLR_free(cstring->cstr_address);
		DEBUG_XDR_FREE(xdrs, cstring, cstring->cstr_address,
					   cstring->cstr_allocated);
	}

	cstring->cstr_address = NULL;
	cstring->cstr_allocated = 0;
}


// CVC: This function is a little stub to validate that indeed, bpb's aren't
// overwritten by accident. Even though xdr_string writes to cstr_address,
// an action we wanted to block, it first allocates a new buffer.
// The problem is that bpb's aren't copied, but referenced by address, so we
// don't want a const param being hijacked and its memory location overwritten.
// The same test has been applied to put_segment and batch_segments operations.
// The layout of CSTRING and CSTRING_CONST is exactly the same.
// Changing CSTRING to use cstr_address as const pointer would upset other
// places of the code, so only P_BLOB was changed to use CSTRING_CONST.
// The same function is being used to check P_SGMT & P_DDL.
static inline bool_t xdr_cstring_const(XDR* xdrs, CSTRING_CONST* cstring)
{
#ifdef SUPERCLIENT
#ifdef DEV_BUILD
	const bool cond =
		!(xdrs->x_op == XDR_DECODE &&
			cstring->cstr_length <= cstring->cstr_allocated
			&& cstring->cstr_allocated);
	fb_assert(cond);
#endif
#endif
	return xdr_cstring(xdrs, reinterpret_cast<CSTRING*>(cstring));
}

static bool_t xdr_cstring( XDR* xdrs, CSTRING* cstring)
{
/**************************************
 *
 *	x d r _ c s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Map a counted string structure.
 *
 **************************************/
	SLONG l;
	SCHAR trash[4];
	static const SCHAR filler[4] = { 0, 0, 0, 0 };

	if (!xdr_short
		(xdrs,
		 reinterpret_cast<SSHORT*>(&cstring->cstr_length)))
	{
		return FALSE;
	}

	switch (xdrs->x_op) {
	case XDR_ENCODE:
		if (cstring->cstr_length &&
			!(*xdrs->x_ops->x_putbytes) (xdrs,
										 reinterpret_cast<
										 const SCHAR*>(cstring->cstr_address),
										 cstring->cstr_length)) 
		{
			return FALSE;
		}
		l = (4 - cstring->cstr_length) & 3;
		if (l)
			return (*xdrs->x_ops->x_putbytes) (xdrs,
											   filler,
											   l);
		{
			return TRUE;
		}

	case XDR_DECODE:
		if (!alloc_cstring(xdrs, cstring))
			return FALSE;
		if (!(*xdrs->x_ops->x_getbytes)
			(xdrs, reinterpret_cast<SCHAR*>(cstring->cstr_address),
			 cstring->cstr_length)) 
		{
			return FALSE;
		}
		l = (4 - cstring->cstr_length) & 3;
		if (l)
			return (*xdrs->x_ops->x_getbytes) (xdrs, trash, l);
		return TRUE;

	case XDR_FREE:
		free_cstring(xdrs, cstring);
		return TRUE;
	}

	return FALSE;
}


static bool_t xdr_datum( XDR* xdrs, DSC* desc, BLOB_PTR* buffer)
{
/**************************************
 *
 *	x d r _ d a t u m
 *
 **************************************
 *
 * Functional description
 *	Handle a data item by relative descriptor and buffer.
 *
 **************************************/
	SSHORT n;

	BLOB_PTR* p = buffer + (IPTR) desc->dsc_address;

	switch (desc->dsc_dtype) {
	case dtype_text:
		if (!xdr_opaque
			(xdrs, reinterpret_cast<SCHAR*>(p),
			 desc->dsc_length))
		{
			return FALSE;
		}
		break;

	case dtype_varying:
		fb_assert(desc->dsc_length >= sizeof(USHORT));
		if (!xdr_short(xdrs,
			reinterpret_cast<SSHORT*>(&((vary*) p)->vary_length)))
		{
			return FALSE;
		}
		if (!xdr_opaque(xdrs, 
			reinterpret_cast<SCHAR*>(((vary*) p)->vary_string),
			MIN((USHORT) (desc->dsc_length - 2), ((vary*) p)->vary_length)))
		{
			return FALSE;
		}
		break;

	case dtype_cstring:
		if (xdrs->x_op == XDR_ENCODE)
			n =
				MIN(strlen(reinterpret_cast<char*>(p)),
					(ULONG) (desc->dsc_length - 1));
		if (!xdr_short(xdrs, &n))
			return FALSE;
		if (!xdr_opaque(xdrs, reinterpret_cast<SCHAR*>(p), n))
			return FALSE;
		if (xdrs->x_op == XDR_DECODE)
			p[n] = 0;
		break;

	case dtype_short:
		fb_assert(desc->dsc_length >= sizeof(SSHORT));
		if (!xdr_short(xdrs, reinterpret_cast<SSHORT*>(p)))
			return FALSE;
		break;

	case dtype_sql_time:
	case dtype_sql_date:
	case dtype_long:
		fb_assert(desc->dsc_length >= sizeof(SLONG));
		if (!xdr_long(xdrs, reinterpret_cast<SLONG*>(p)))
			return FALSE;
		break;

	case dtype_real:
		fb_assert(desc->dsc_length >= sizeof(float));
		if (!xdr_float(xdrs, reinterpret_cast<float*>(p)))
			  return FALSE;
		break;

	case dtype_double:
		fb_assert(desc->dsc_length >= sizeof(double));
		if (!xdr_double(xdrs, reinterpret_cast<double*>(p)))
			  return FALSE;
		break;

#ifdef VMS
	case dtype_d_float:
		fb_assert(desc->dsc_length >= sizeof(d_float));
		if (!xdr_d_float(xdrs, p))
			return FALSE;
		break;
#endif

	case dtype_timestamp:
		fb_assert(desc->dsc_length >= 2 * sizeof(SLONG));
		if (!xdr_long(xdrs, &((SLONG *) p)[0]))
			return FALSE;
		if (!xdr_long(xdrs, &((SLONG *) p)[1]))
			return FALSE;
		break;

	case dtype_int64:
		fb_assert(desc->dsc_length >= sizeof(SINT64));
		if (!xdr_hyper(xdrs, (SINT64 *) p))
			return FALSE;
		break;

	case dtype_array:
	case dtype_quad:
	case dtype_blob:
		fb_assert(desc->dsc_length >= sizeof(struct bid));
		if (!xdr_quad(xdrs, (struct bid *) p))
			return FALSE;
		break;

	default:
		fb_assert(FALSE);
		return FALSE;
	}

	return TRUE;
}


#ifdef DEBUG_XDR_MEMORY
static bool_t xdr_debug_packet( XDR* xdrs, enum xdr_op xop, PACKET* packet)
{
/**************************************
 *
 *	x d r _ d e b u g _ p a c k e t
 *
 **************************************
 *
 * Functional description
 *	Start/stop monitoring a packet's memory allocations by
 *	entering/removing from a port's packet tracking vector.
 *
 **************************************/
	ULONG i;

	rem_port* port = (rem_port*) xdrs->x_public;
	fb_assert(port != 0);
	fb_assert(port->port_header.blk_type == type_port);

	if (xop == XDR_FREE) {
		/* Free a slot in the packet tracking vector */

		rem_vec* vector = port->port_packet_vector;
		if (vector)
			for (i = 0; i < vector->vec_count; i++)
				if (vector->vec_object[i] == (BLK) packet) {
					vector->vec_object[i] = 0;
					return TRUE;
				}
	}
	else {						/* XDR_ENCODE or XDR_DECODE */

		/* Allocate an unused slot in the packet tracking vector
		   to start recording memory allocations for this packet. */

		fb_assert(xop == XDR_ENCODE || xop == XDR_DECODE);
		rem_vec* vector = ALLR_vector(&port->port_packet_vector, 0);

		for (i = 0; i < vector->vec_count; i++)
			if (vector->vec_object[i] == (BLK) packet)
				return TRUE;

		for (i = 0; i < vector->vec_count; i++)
			if (vector->vec_object[i] == 0)
				break;

		if (i >= vector->vec_count)
			vector = ALLR_vector(&port->port_packet_vector, i);

		vector->vec_object[i] = (BLK) packet;
	}

	return TRUE;
}
#endif


static bool_t xdr_longs( XDR* xdrs, CSTRING* cstring)
{
/**************************************
 *
 *	x d r _ l o n g s
 *
 **************************************
 *
 * Functional description
 *	Pass a vector of longs.
 *
 **************************************/
	if (!xdr_short
		(xdrs,
		 reinterpret_cast<SSHORT*>(&cstring->cstr_length)))
	{
		return FALSE;
	}

/* Handle operation specific stuff, particularly memory allocation/deallocation */

	switch (xdrs->x_op) {
	case XDR_ENCODE:
		break;

	case XDR_DECODE:
		if (!alloc_cstring(xdrs, cstring))
			return FALSE;
		break;

	case XDR_FREE:
		free_cstring(xdrs, cstring);
		return TRUE;
	}

	const ULONG n = cstring->cstr_length / sizeof(SLONG);

	SLONG* next = (SLONG*) cstring->cstr_address;
	for (const SLONG* const end = next + (int) n; next < end; next++)
	{
		if (!xdr_long(xdrs, next))
			return FALSE;
	}

	return TRUE;
}


static bool_t xdr_message( XDR* xdrs, REM_MSG message, rem_fmt* format)
{
/**************************************
 *
 *	x d r _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Map a formatted message.
 *
 **************************************/
	if (xdrs->x_op == XDR_FREE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;

/* If we are running a symmetric version of the protocol, just slop
   the bits and don't sweat the translations */

	if (port->port_flags & PORT_symmetric)
#ifndef VMS
		return xdr_opaque(xdrs,
						  reinterpret_cast<SCHAR*>(message->msg_address),
						  format->fmt_length);
#else
		if (port->port_protocol >= PROTOCOL_VERSION5)
			return xdr_opaque(xdrs, message->msg_address, format->fmt_length);
		else
			return xdr_semi_opaque(xdrs, message, format);
#endif

	dsc* desc = format->fmt_desc;
	for (const dsc* const end = desc + format->fmt_count; desc < end; ++desc)
		if (!xdr_datum(xdrs, desc, message->msg_address))
			return FALSE;

	DEBUG_PRINTSIZE(xdrs, op_void);
	return TRUE;
}


static bool_t xdr_quad( XDR* xdrs, struct bid* ip)
{
/**************************************
 *
 *	x d r _ q u a d
 *
 **************************************
 *
 * Functional description
 *	Map from external to internal representation (or vice versa).
 *	A "quad" is represented by two longs.
 *	Currently used only for blobs
 *
 **************************************/

	switch (xdrs->x_op) {
	case XDR_ENCODE:
		if ((*xdrs->x_ops->x_putlong)
			(xdrs, reinterpret_cast<SLONG*>(&ip->bid_relation_id))
			&& (*xdrs->x_ops->x_putlong) (xdrs,
										  reinterpret_cast<
										  SLONG*>(&ip->bid_number)))
		{
			return TRUE;
		}
		return FALSE;

	case XDR_DECODE:
		if (!(*xdrs->x_ops->x_getlong)
			(xdrs,
			 reinterpret_cast<SLONG*>(&ip->bid_relation_id)))
		{
			return FALSE;
		}
		return (*xdrs->x_ops->x_getlong) (xdrs,
										  reinterpret_cast<SLONG*>(&ip->bid_number));

	case XDR_FREE:
		return TRUE;
	}

	return FALSE;
}


static bool_t xdr_request(
						  XDR* xdrs,
						  USHORT request_id,
						  USHORT message_number, USHORT incarnation)
{
/**************************************
 *
 *	x d r _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Map a formatted message.
 *
 **************************************/
	if (xdrs->x_op == XDR_FREE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;
	rrq* request = (rrq*) port->port_objects[request_id];

	if (!request)
		return FALSE;

	if (incarnation && !(request = REMOTE_find_request(request, incarnation)))
		return FALSE;

	rrq::rrq_repeat* tail = &request->rrq_rpt[message_number];

	REM_MSG message = tail->rrq_xdr;
	if (!message)
		return FALSE;

	tail->rrq_xdr = message->msg_next;
	rem_fmt* format = tail->rrq_format;

/* Find the address of the record */

	if (!message->msg_address)
		message->msg_address = message->msg_buffer;

	return xdr_message(xdrs, message, format);
}


#ifdef VMS
static bool_t xdr_semi_opaque( XDR* xdrs, REM_MSG message, rem_fmt* format)
{
/**************************************
 *
 *	x d r _ s e m i _ o p a q u e
 *
 **************************************
 *
 * Functional description
 *	Move data while checking for doubles in d_float format.
 *
 **************************************/
	DSC *desc, *end;
	BLOB_PTR *msg_address;
	double *convert;

	switch (xdrs->x_op) {
	case XDR_ENCODE:
		for (desc = format->fmt_desc, end = desc + format->fmt_count;
			 desc < end; desc++)
		{
			if (desc->dsc_dtype == dtype_d_float)
				break;
		}

		if (desc >= end)
			return xdr_opaque(xdrs, message->msg_address, format->fmt_length);

		if (!gfloat_buffer || gfloat_buffer->str_length < format->fmt_length) {
			if (gfloat_buffer)
				ALLR_free(gfloat_buffer);
			gfloat_buffer = (rem_str*) ALLR_block(type_str, format->fmt_length);
			gfloat_buffer->str_length = format->fmt_length;
		}

		msg_address = gfloat_buffer->str_data;
		memcpy(msg_address, message->msg_address, format->fmt_length);

		for (desc = format->fmt_desc, end = desc + format->fmt_count;
			 desc < end; desc++)
		{
			if (desc->dsc_dtype == dtype_d_float) {
				convert = (double *) (msg_address + (IPTR)desc->dsc_address);
				*convert = MTH$CVT_D_G(convert);
			}
		}

		return xdr_opaque(xdrs, msg_address, format->fmt_length);

	case XDR_DECODE:
		if (!xdr_opaque(xdrs, message->msg_address, format->fmt_length))
			return FALSE;

		for (desc = format->fmt_desc, end = desc + format->fmt_count;
			 desc < end; desc++)
		{
			if (desc->dsc_dtype == dtype_d_float) {
				convert =
					(double *) (message->msg_address +
								(IPTR) desc->dsc_address);
				*convert = MTH$CVT_G_D(convert);
			}
		}
		return TRUE;

	case XDR_FREE:
		return TRUE;
	}

    return FALSE;
}
#endif


#ifdef VMS
static bool_t xdr_semi_opaque_slice( XDR* xdrs, lstring* slice)
{
/**************************************
 *
 *	x d r _ s e m i _ o p a q u e _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Move data while converting for doubles in d_float format.
 *
 **************************************/
	BLOB_PTR* p = slice->lstr_address;
	for (ULONG n = slice->lstr_length; n; n -= msg_len, p += msg_len) {
		const ULONG msg_len = MIN(n, MAX_OPAQUE);

		BLOB_PTR* msg_addr;
		if (xdrs->x_op == XDR_ENCODE) {
			/* Using a rem_str structure is fine as long as MAX_OPAQUE < 64K */

			if (!gfloat_buffer || gfloat_buffer->str_length < msg_len) {
				if (gfloat_buffer) {
					ALLR_free(gfloat_buffer);
				}
				gfloat_buffer = (rem_str*) ALLR_block(type_str, msg_len);
				gfloat_buffer->str_length = msg_len;
			}

			msg_addr = gfloat_buffer->str_data;
			memcpy(msg_addr, p, msg_len);

			double* convert = (double*) msg_addr;
			for (double* const end = (double*) (msg_addr + msg_len);
				convert < end; ++convert)
			{
				*convert = MTH$CVT_D_G(convert);
			}
		}
		else
			msg_addr = p;

		if (!xdr_opaque(xdrs, msg_addr, msg_len))
			return FALSE;

		if (xdrs->x_op == XDR_DECODE) {
			double* convert = (double*) msg_addr
			for (double* const end = (double*) (msg_addr + msg_len); 
				convert < end; ++convert)
			{
				*convert = MTH$CVT_G_D(convert);
			}
		}
	}

	return TRUE;
}
#endif


static bool_t xdr_slice(
						XDR* xdrs,
						lstring* slice, USHORT sdl_length, const UCHAR* sdl)
{
/**************************************
 *
 *	x d r _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Move a slice of an array under
 *
 **************************************/
	if (!xdr_long(xdrs, reinterpret_cast<SLONG*>(&slice->lstr_length)))
		return FALSE;

/* Handle operation specific stuff, particularly memory allocation/deallocation */

	switch (xdrs->x_op) {
	case XDR_ENCODE:
		break;

	case XDR_DECODE:
		if (!slice->lstr_length)
			return TRUE;
		if (slice->lstr_length > slice->lstr_allocated &&
			slice->lstr_allocated)
		{
			ALLR_free(slice->lstr_address);
			DEBUG_XDR_FREE(xdrs, slice, slice->lstr_address, slice->lstr_allocated);
			slice->lstr_address = NULL;
		}
		if (!slice->lstr_address) {
			if (!
				(slice->lstr_address =
				 ALLR_alloc((SLONG) slice->lstr_length)))
			{
				/* NOMEM: handled by ALLR_alloc() */
				/* FREE:  in realloc case above & XDR_FREE case of this routine */
				return FALSE;
			}

			slice->lstr_allocated = slice->lstr_length;
			DEBUG_XDR_ALLOC(xdrs, slice, slice->lstr_address,
							slice->lstr_allocated);
		}
		break;

	case XDR_FREE:
		if (slice->lstr_allocated) {
			ALLR_free(slice->lstr_address);
			DEBUG_XDR_FREE(xdrs, slice, slice->lstr_address, slice->lstr_allocated);
		}
		slice->lstr_address = NULL;
		slice->lstr_allocated = 0;
		return TRUE;
	}

/* Get descriptor of array element */

	ISC_STATUS_ARRAY status_vector;
	struct sdl_info info;
	if (SDL_info(status_vector, sdl, &info, 0))
		return FALSE;

	dsc* desc = &info.sdl_info_element;
	rem_port* port = (rem_port*) xdrs->x_public;
	BLOB_PTR* p = (BLOB_PTR *) slice->lstr_address;
	ULONG n;

	if (port->port_flags & PORT_symmetric) {
#ifdef VMS
		if (port->port_protocol < PROTOCOL_VERSION5 &&
			desc->dsc_dtype == dtype_d_float)
		{
			return xdr_semi_opaque_slice(xdrs, slice);
		}
#endif

		for (n = slice->lstr_length; n > MAX_OPAQUE;
			n -= MAX_OPAQUE, p += (int) MAX_OPAQUE)
		{
			if (!xdr_opaque (xdrs, reinterpret_cast<SCHAR*>(p), MAX_OPAQUE))
				 return FALSE;
		}
		if (n)
			if (!xdr_opaque(xdrs, reinterpret_cast<SCHAR*>(p), n))
				return FALSE;
	}
	else {
		for (n = 0; n < slice->lstr_length / desc->dsc_length; n++) {
			if (!xdr_datum(xdrs, desc, p))
				return FALSE;
			p = p + (ULONG) desc->dsc_length;
		}
	}

	return TRUE;
}


static bool_t xdr_sql_blr(
						  XDR* xdrs,
						  SLONG statement_id,
						  CSTRING* blr,
						  int direction, SQL_STMT_TYPE stmt_type)
{
/**************************************
 *
 *	x d r _ s q l _ b l r
 *
 **************************************
 *
 * Functional description
 *	Map an sql blr string.  This work is necessary because
 *	we will use the blr to read data in the current packet.
 *
 **************************************/
	if (!xdr_cstring(xdrs, blr))
		return FALSE;

/* We care about all receives and sends from fetch */

	if (xdrs->x_op == XDR_FREE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;
	RSR statement;
	if (statement_id >= 0) {
		if (!(statement = (RSR) port->port_objects[statement_id]))
			return FALSE;
	}
	else {
		if (!(statement = port->port_statement))
			statement = port->port_statement = (RSR) ALLR_block(type_rsr, 0);
	}

	if ((xdrs->x_op == XDR_ENCODE) && !direction) {
		if (statement->rsr_bind_format)
			statement->rsr_format = statement->rsr_bind_format;
		return TRUE;
	}

/* Parse the blr describing the message. */

	rem_fmt** fmt_ptr = (direction) ?
		&statement->rsr_select_format : &statement->rsr_bind_format;

	if (xdrs->x_op == XDR_DECODE) {
		/* For an immediate statement, flush out any previous format information
		 * that might be hanging around from an earlier execution.
		 * For all statements, if we have new blr, flush out the format information
		 * for the old blr.
		 */
		if (*fmt_ptr
			&& ((stmt_type == TYPE_IMMEDIATE) || blr->cstr_length != 0))
		{
			ALLR_release(*fmt_ptr);
			*fmt_ptr = NULL;
		}

		/* If we have BLR describing a new input/output message, get ready by
		 * setting up a format
		 */
		if (blr->cstr_length) {
			REM_MSG temp_msg =
				(REM_MSG) PARSE_messages(blr->cstr_address, blr->cstr_length);
			if (temp_msg != (REM_MSG) -1) {
				*fmt_ptr = (rem_fmt*) temp_msg->msg_address;
				ALLR_release(temp_msg);
			}
		}
	}

/* If we know the length of the message, make sure there is a buffer
   large enough to hold it. */

	if (!(statement->rsr_format = *fmt_ptr))
		return TRUE;

    REM_MSG message = statement->rsr_buffer;
	if (!(message != 0) ||
		statement->rsr_format->fmt_length > statement->rsr_fmt_length)
	{
		REMOTE_release_messages(message);
		statement->rsr_fmt_length = statement->rsr_format->fmt_length;
		statement->rsr_buffer = message =
			(REM_MSG) ALLR_block(type_msg, statement->rsr_fmt_length);
		statement->rsr_message = message;
		message->msg_next = message;
#ifdef SCROLLABLE_CURSORS
		message->msg_prior = message;
#endif
	}

	return TRUE;
}


static bool_t xdr_sql_message( XDR* xdrs, SLONG statement_id)
{
/**************************************
 *
 *	x d r _ s q l _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Map a formatted sql message.
 *
 **************************************/
	REM_MSG message;
	RSR statement;

	if (xdrs->x_op == XDR_FREE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;
	if (statement_id >= 0) {
		if (!(statement = (RSR) port->port_objects[statement_id]))
			return FALSE;
	}
	else
		statement = port->port_statement;

	if ((message = statement->rsr_buffer) != 0) {
		statement->rsr_buffer = message->msg_next;
		if (!message->msg_address)
			message->msg_address = message->msg_buffer;
	}

	return xdr_message(xdrs, message, statement->rsr_format);
}


static bool_t xdr_status_vector(
								XDR* xdrs, ISC_STATUS* vector, TEXT* strings[])
{
/**************************************
 *
 *	x d r _ s t a t u s _ v e c t o r
 *
 **************************************
 *
 * Functional description
 *	Map a status vector.  This is tricky since the status vector
 *	may contain argument types, numbers, and strings.
 *
 **************************************/
	TEXT **sp, **end;
	SLONG vec;
	XDR temp_xdrs;

/* If this is a free operation, release any allocated strings */

	if (xdrs->x_op == XDR_FREE) {
		for (sp = strings, end = strings + 10; sp < end; sp++)
			if (*sp && !xdr_wrapstring(xdrs, sp))
				return FALSE;
		return TRUE;
	}

	while (true) {
		if (xdrs->x_op == XDR_ENCODE)
			vec = (SLONG) * vector++;
		if (!xdr_long(xdrs, &vec))
			return FALSE;
		if (xdrs->x_op == XDR_DECODE)
			*vector++ = (ISC_STATUS) vec;
		switch ((USHORT) vec) {
		case isc_arg_end:
			return TRUE;

		case isc_arg_interpreted:
		case isc_arg_string:
			if (xdrs->x_op == XDR_ENCODE) {
				if (!xdr_wrapstring
					(xdrs, reinterpret_cast<SCHAR**>(vector++)))
					return FALSE;
			}
			else {
				/* Use the first slot in the strings table */
				sp = strings;
				if (*sp) {
					/* Slot is used, by a string passed in a previous
					 * status vector.  Free that string, and allocate
					 * a new one to prevent any size mismatches trashing
					 * memory.
					 */
					temp_xdrs.x_public = xdrs->x_public;
					temp_xdrs.x_op = XDR_FREE;
					if (!xdr_wrapstring(&temp_xdrs, sp))
						return FALSE;
					*sp = NULL;
				}
				if (!xdr_wrapstring(xdrs, sp))
					return FALSE;
				*vector++ = (ISC_STATUS) * sp;
				strings++;
			}
			break;

		case isc_arg_number:
		default:
			if (xdrs->x_op == XDR_ENCODE)
				vec = (SLONG) * vector++;
			if (!xdr_long(xdrs, &vec))
				return FALSE;
			if (xdrs->x_op == XDR_DECODE)
				*vector++ = (ISC_STATUS) vec;
			break;
		}
	}
}


static bool_t xdr_trrq_blr( XDR* xdrs, CSTRING* blr)
{
/**************************************
 *
 *	x d r _ t r r q  _ b l r
 *
 **************************************
 *
 * Functional description
 *	Map a message blr string.  This work is necessary because
 *	we will use the blr to read data in the current packet.
 *
 **************************************/
	REM_MSG message, temp;

	if (!xdr_cstring(xdrs, blr))
		return FALSE;

/* We care about all receives and sends from fetch */

	if (xdrs->x_op == XDR_FREE || xdrs->x_op == XDR_ENCODE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;
	RPR procedure = port->port_rpr;
	if (!procedure)
		procedure = port->port_rpr = (RPR) ALLR_block(type_rpr, 0);

/* Parse the blr describing the message. */

	if (procedure->rpr_in_msg) {
		ALLR_release(procedure->rpr_in_msg);
		procedure->rpr_in_msg = NULL;
	}
	if (procedure->rpr_in_format) {
		ALLR_release(procedure->rpr_in_format);
		procedure->rpr_in_format = NULL;
	}
	if (procedure->rpr_out_msg) {
		ALLR_release(procedure->rpr_out_msg);
		procedure->rpr_out_msg = NULL;
	}
	if (procedure->rpr_out_format) {
		ALLR_release(procedure->rpr_out_format);
		procedure->rpr_out_format = NULL;
	}
	if ((message = PARSE_messages(blr->cstr_address, blr->cstr_length)) !=
		(REM_MSG) - 1) {
		while (message) {
			if (message->msg_number == 0) {
				procedure->rpr_in_msg = message;
				procedure->rpr_in_format = (rem_fmt*) message->msg_address;
				message->msg_address = message->msg_buffer;
				message = message->msg_next;
				procedure->rpr_in_msg->msg_next = NULL;
			}
			else if (message->msg_number == 1) {
				procedure->rpr_out_msg = message;
				procedure->rpr_out_format = (rem_fmt*) message->msg_address;
				message->msg_address = message->msg_buffer;
				message = message->msg_next;
				procedure->rpr_out_msg->msg_next = NULL;
			}
			else {
				temp = message;
				message = message->msg_next;
				ALLR_release(temp);
			}
		}
	}
	else
		fb_assert(FALSE);

	return TRUE;
}


static bool_t xdr_trrq_message( XDR* xdrs, USHORT msg_type)
{
/**************************************
 *
 *	x d r _ t r r q _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Map a formatted transact request message.
 *
 **************************************/
	if (xdrs->x_op == XDR_FREE)
		return TRUE;

	rem_port* port = (rem_port*) xdrs->x_public;
	RPR procedure = port->port_rpr;

	if (msg_type == 1)
		return xdr_message(xdrs, procedure->rpr_out_msg,
						   procedure->rpr_out_format);
	else
		return xdr_message(xdrs, procedure->rpr_in_msg,
						   procedure->rpr_in_format);
}


static RSR get_statement( XDR * xdrs, SSHORT statement_id)
{
/**************************************
 *
 *	g e t _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Returns the statement based upon the statement id
 *      if statement_id = -1 then statement = port_statement
 *      otherwise, the statement comes from port_objects[statement_id]
 *      if there are no port_objects, then statement = NULL
 *
 **************************************/

	RSR statement = NULL;
	rem_port* port = (rem_port*) xdrs->x_public;

/* if the statement ID is -1, this seems to indicate that we are
   re-executing the previous statement.  This is not a
   well-understood area of the implementation.

if (statement_id == -1)
    statement = port->port_statement;
else
*/

	fb_assert(statement_id >= -1);

	if ((port->port_objects) &&
		((SLONG) statement_id < (SLONG) port->port_object_vector->vec_count)
		&& (statement_id >= 0))
	{
		statement = (RSR) port->port_objects[(SLONG) statement_id];
	}

/* Check that what we found really is a statement structure */
	fb_assert(!statement || (statement->rsr_header.blk_type == type_rsr));
	return statement;
}

