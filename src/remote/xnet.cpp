/*
 *	PROGRAM:	JRD Remote Interface/Server
 *      MODULE:         xnet.cpp
 *      DESCRIPTION:    Interprocess Server Communications module.
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
 * 2003.05.01 Victor Seryodkin, Dmitry Yemanov: Completed XNET implementation
 */

#include "firebird.h"
#include <stdio.h>
#include "../remote/remote.h"
#include "../jrd/ibase.h"
#include "../jrd/thd.h"
#include "../jrd/iberr.h"
#include "../remote/xnet.h"
#include "../utilities/install/install_nt.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../remote/xnet_proto.h"
#include "../remote/serve_proto.h"
#include "../remote/os/win32/window.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thread_proto.h"
#include "../common/classes/fb_string.h"
#include <time.h>

#ifdef WIN_NT
#include <windows.h>
#endif /* WIN_NT */

const int MAX_SEQUENCE	= 256;

static int accept_connection(rem_port*, P_CNCT *);
static rem_port* xnet_alloc_port(rem_port*, UCHAR *, ULONG, UCHAR *, ULONG);
static rem_port* aux_connect(rem_port*, PACKET*, t_event_ast);
static rem_port* aux_request(rem_port*, PACKET*);

static void xnet_cleanup_comm(XCC);
static void xnet_cleanup_port(rem_port*);
static void disconnect(rem_port*);
static void exit_handler(rem_port*);

static rem_port* receive(rem_port*, PACKET *);
static int send_full(rem_port*, PACKET *);
static int send_partial(rem_port*, PACKET *);

#ifdef SUPERCLIENT
static HANDLE server_process_handle = 0;
static void xnet_on_server_shutdown(rem_port* port);
#else
static TEXT XNET_command_line[MAXPATHLEN + 32], *XNET_p;
static XPM xnet_make_xpm(ULONG, time_t);
#endif // SUPERCLIENT

static int xdrxnet_create(XDR *, rem_port*, UCHAR *, USHORT, enum xdr_op);

static int xnet_destroy(XDR *);
static bool_t xnet_getbytes(XDR *, SCHAR *, u_int);
static bool_t xnet_getlong(XDR *, SLONG *);
static u_int xnet_getpostn(XDR *);
static caddr_t xnet_inline(XDR *, u_int);
static bool_t xnet_putlong(XDR *, const SLONG*);
static bool_t xnet_putbytes(XDR*, const SCHAR*, u_int);
static bool_t xnet_setpostn(XDR *, u_int);
static bool_t xnet_read(XDR * xdrs);
static bool_t xnet_write(XDR * xdrs);

static xdr_t::xdr_ops xnet_ops =
{
	xnet_getlong,
	xnet_putlong,
	xnet_getbytes,
	xnet_putbytes,
	xnet_getpostn,
	xnet_setpostn,
	xnet_inline,
	xnet_destroy
};

const USHORT MAX_PTYPE	= ptype_out_of_band;

static ULONG global_pages_per_slot = XPS_DEF_PAGES_PER_CLI;
static ULONG global_slots_per_map = XPS_DEF_NUM_CLI;
static XPM global_client_maps = NULL;

#ifdef WIN_NT

static HANDLE xnet_connect_mutex = 0;
static HANDLE xnet_connect_map_h = 0;
static CADDR_T xnet_connect_map = 0;

static HANDLE xnet_connect_event = 0;
static HANDLE xnet_response_event = 0;
static DWORD CurrentProcessId;

#endif  /* WIN_NT */

static bool_t xnet_initialized = FALSE;
static bool_t xnet_shutdown = FALSE;
static bool_t xnet_mutex_ready = FALSE;

#ifdef SUPERCLIENT
static bool_t xnet_connect_init();
#endif // SUPERCLIENT
static void xnet_connect_fini();
static void xnet_release_all(void);

static MUTX_T xnet_mutex;

#if defined(SUPERCLIENT)

inline void XNET_LOCK() {
	THD_mutex_lock(&xnet_mutex);
}
inline void XNET_UNLOCK() {
	THD_mutex_unlock(&xnet_mutex);
}

#elif defined(SUPERSERVER)

inline void XNET_LOCK() {
	if (!xnet_shutdown)
		THREAD_EXIT();
	THD_mutex_lock(&xnet_mutex);
	if (!xnet_shutdown)
		THREAD_ENTER();
}
inline void XNET_UNLOCK() {
	THD_mutex_unlock(&xnet_mutex);
}

#else // CS

inline void XNET_LOCK() {
}
inline void XNET_UNLOCK() {
}

#endif

static int xnet_error(rem_port*, const TEXT*, ISC_STATUS, int, ULONG);

#define XNET_ERROR(po, fu, op, st) xnet_error(po, fu, op, st, __LINE__);
#define XNET_LOG_ERROR(msg) xnet_log_error(__LINE__, msg)
#define XNET_LOG_ERRORC(msg) xnet_log_error(__LINE__, msg, ERRNO)

static void xnet_log_error(int source_line_num, const char* err_msg, ULONG err_code = 0)
{ 
/**************************************
 *
 *  x n e t _ l o g _ e r r o r
 *
 **************************************
 *
 * Functional description
 *  Error logging when port isn;t yet allocated
 *
 **************************************/
	char err_msg_buff[256];

	if (err_code)
		sprintf(err_msg_buff, "XNET error (xnet:%d)  %s  Win32 error = %"ULONGFORMAT"\n",
			source_line_num, err_msg, err_code);
	else
		sprintf(err_msg_buff, "XNET error (xnet:%d)  %s\n",
			source_line_num, err_msg);

	gds__log(err_msg_buff);
}


rem_port* XNET_analyze(
				  Firebird::PathName& file_name,
				  ISC_STATUS* status_vector,
				  const TEXT* node_name, const TEXT* user_string,
				  bool uv_flag)
{
/**************************************
 *
 *      X N E T _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *  Client performs attempt to establish connection
 *  based on the set of protocols.
 *	If a connection is established, return a port block,
 *	otherwise return NULL.
 *
 **************************************/

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	RDB rdb = (RDB) ALLR_block(type_rdb, 0);
	PACKET* packet = &rdb->rdb_packet;

/* Pick up some user identification information */

	TEXT user_id[128];
	user_id[0] = CNCT_user;
	TEXT* p = user_id + 2;
	ISC_get_user(p, 0, 0, 0, 0, 0, 0);
	user_id[1] = strlen(p);

	for (; *p; p++) {
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';
	}

	*p++ = CNCT_host;
	p++;
	ISC_get_host(p, (USHORT) (user_id + sizeof(user_id) - p));
	p[-1] = strlen(p);

	for (; *p; p++) {
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';
	}

	if (uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}

	const SSHORT user_length = p - user_id;

/* Establish connection to server */

	P_CNCT* cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = file_name.length();
	cnct->p_cnct_file.cstr_address = 
			reinterpret_cast<UCHAR*>(file_name.begin());

/* Note: prior to V3.1E a recievers could not in truth handle more
   then 5 protocol descriptions; however, the interprocess server 
   was created in 4.0 so this does not apply */

	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = (UCHAR*) user_id;

	static const p_cnct::p_cnct_repeat protocols_to_try1[] =
	{
		REMOTE_PROTOCOL(PROTOCOL_VERSION7, ptype_rpc, MAX_PTYPE, 1),
		REMOTE_PROTOCOL(PROTOCOL_VERSION8, ptype_rpc, MAX_PTYPE, 2),
		REMOTE_PROTOCOL(PROTOCOL_VERSION10, ptype_rpc, MAX_PTYPE, 3)
#ifdef SCROLLABLE_CURSORS
		,
		REMOTE_PROTOCOL(PROTOCOL_SCROLLABLE_CURSORS, ptype_rpc, MAX_PTYPE, 4)
#endif
	};
	cnct->p_cnct_count = FB_NELEM(protocols_to_try1);

	for (size_t i = 0; i < cnct->p_cnct_count; i++) {
		cnct->p_cnct_versions[i] = protocols_to_try1[i];
	}

/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */

	rem_port* port = XNET_connect(node_name, packet, status_vector, FALSE);
	if (!port) {
		ALLR_release((BLK) rdb);
		return NULL;
	}

/* Get response packet from server. */

	rdb->rdb_port = port;
	port->port_context = rdb;
	port->receive(packet);

	if (packet->p_operation == op_reject && !uv_flag) {
		disconnect(port);
		packet->p_operation = op_connect;
		cnct->p_cnct_operation = op_attach;
		cnct->p_cnct_cversion = CONNECT_VERSION2;
		cnct->p_cnct_client = ARCHITECTURE;
		cnct->p_cnct_file.cstr_length = file_name.length();
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name.c_str();

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = (UCHAR*) user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try2[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION4, ptype_rpc, ptype_batch_send, 1),
			REMOTE_PROTOCOL(PROTOCOL_VERSION6, ptype_rpc, ptype_batch_send, 2),
		};
		cnct->p_cnct_count = FB_NELEM(protocols_to_try2);

		for (size_t i = 0; i < cnct->p_cnct_count; i++) {
			cnct->p_cnct_versions[i] = protocols_to_try2[i];
		}

		if (!(port = XNET_connect(node_name, packet, status_vector, FALSE))) {
			ALLR_release((BLK) rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation == op_reject && !uv_flag) {
		disconnect(port);
		packet->p_operation = op_connect;
		cnct->p_cnct_operation = op_attach;
		cnct->p_cnct_cversion = CONNECT_VERSION2;
		cnct->p_cnct_client = ARCHITECTURE;
		cnct->p_cnct_file.cstr_length = file_name.length();
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name.c_str();

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = (UCHAR*) user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try3[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION3, ptype_rpc, ptype_batch_send, 1)
		};
		cnct->p_cnct_count = FB_NELEM(protocols_to_try3);

		for (size_t i = 0; i < cnct->p_cnct_count; i++) {
			cnct->p_cnct_versions[i] = protocols_to_try3[i];
		}

		if (!(port = XNET_connect(node_name, packet, status_vector, FALSE))) {
			ALLR_release((BLK) rdb);
			return NULL;
		}

		/* Get response packet from server. */

		rdb->rdb_port = port;
		port->port_context = rdb;
		port->receive(packet);
	}

	if (packet->p_operation != op_accept) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_connect_reject;
		*status_vector++ = isc_arg_end;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version 
   string to reflect it...  */

	TEXT buffer[64];
	sprintf(buffer, "%s/P%d", port->port_version->str_data,
			port->port_protocol);
	ALLR_free((UCHAR *) port->port_version);
	port->port_version = REMOTE_make_string(buffer);

	if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (packet->p_acpt.p_acpt_type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (packet->p_acpt.p_acpt_type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	return port;
}


rem_port* XNET_connect(const TEXT* name, PACKET* packet,
                  ISC_STATUS* status_vector, USHORT flag)
{
/**************************************
 *
 *      X N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.
 *
 **************************************/
	if (xnet_shutdown)
		return NULL;

	// set up for unavailable server
	status_vector[0] = isc_arg_gds;
	status_vector[1] = isc_unavailable;
	status_vector[2] = isc_arg_end;

#ifndef SUPERCLIENT
	// We'll come here on handling op_service_attach packet on server.
	// Server shouldn't do jumps.
	return NULL;
#else
/*
	LONG test_xnet_initialized;

	InterlockedExchange((LPLONG) &test_xnet_initialized, xnet_initialized);
	if (!test_xnet_initialized) {
		InterlockedExchange((LPLONG) &xnet_initialized, TRUE);
		CurrentProcessId = GetCurrentProcessId();
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, NULL);
		InterlockedExchange((LPLONG) &xnet_mutex_ready, (LONG) TRUE);
	}
	else {
		while (!xnet_mutex_ready)
			Sleep(10);
	}
*/
	rem_port* port = NULL;
	XCC xcc = NULL;
	XPM xpm = NULL;
	XPS xps = NULL;

	XNET_RESPONSE response;

	TEXT name_buffer[128];
	FILE_ID file_handle = 0;
	CADDR_T mapped_address = 0;

	if (!xnet_initialized) {
		xnet_initialized = TRUE;
		CurrentProcessId = GetCurrentProcessId();
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, NULL);
	}

	XNET_LOCK();
	if (!xnet_connect_init()) {
		XNET_UNLOCK();
		return NULL;
	}

	// waiting for xnet connect lock to release
	if (WaitForSingleObject(xnet_connect_mutex, XNET_CONNECT_TIMEOUT) != WAIT_OBJECT_0)
	{
		xnet_connect_fini();
		XNET_UNLOCK();
		return NULL;
	}

	/* writing connect request */

	// mark connect area with XNET_INVALID_MAP_NUM to
	// detect server faults on response
	PXNET_RESPONSE(xnet_connect_map)->map_num = XNET_INVALID_MAP_NUM;
	PXNET_RESPONSE(xnet_connect_map)->proc_id = CurrentProcessId; 

	SetEvent(xnet_connect_event);

	// waiting for server response
	if (WaitForSingleObject(xnet_response_event, XNET_CONNECT_TIMEOUT) != WAIT_OBJECT_0)
	{
		ReleaseMutex(xnet_connect_mutex);
		xnet_connect_fini();
		XNET_UNLOCK();
		return NULL;
	}

	memcpy(&response, xnet_connect_map, XNET_CONNECT_RESPONZE_SIZE);
	ReleaseMutex(xnet_connect_mutex);
	xnet_connect_fini();

	XNET_UNLOCK();

	if (response.map_num == XNET_INVALID_MAP_NUM) {
		XNET_LOG_ERROR("server failed to response on connect request");
		return NULL;
	}

	global_pages_per_slot = response.pages_per_slot;
	global_slots_per_map = response.slots_per_map;
	const ULONG map_num = response.map_num;
	const ULONG slot_num = response.slot_num;
	const time_t timestamp = response.timestamp;

	try {

		XNET_LOCK();

		// see if area is already mapped for this client
		for (xpm = global_client_maps; xpm; xpm = xpm->xpm_next) {
			if (xpm->xpm_number == map_num &&
					xpm->xpm_timestamp == timestamp &&
					!(xpm->xpm_flags & XPMF_SERVER_SHUTDOWN))
			{
				break;
			}
		}

		if (!xpm) {

			// Area hasn't been mapped. Open new file mapping.
			sprintf(name_buffer, XNET_MAPPED_FILE_NAME, XNET_PREFIX, map_num, (ULONG) timestamp);

			file_handle = OpenFileMapping(FILE_MAP_WRITE, FALSE, name_buffer);
			if (!file_handle) {
				XNET_UNLOCK();
				throw -1;
			}

			mapped_address = MapViewOfFile(file_handle, FILE_MAP_WRITE, 0L, 0L,
										   XPS_MAPPED_SIZE(global_slots_per_map, global_pages_per_slot));
			if (!mapped_address) {
				XNET_UNLOCK();
				throw -1;
			}

			xpm = (XPM) ALLR_alloc(sizeof(struct xpm));
			if (!xpm) {
				XNET_UNLOCK();
				throw -1;
			}

			xpm->xpm_next = global_client_maps;
			global_client_maps = xpm;
			xpm->xpm_count = 0;
			xpm->xpm_number = map_num;
			xpm->xpm_handle = file_handle;
			xpm->xpm_address = mapped_address;
			xpm->xpm_timestamp = timestamp;
			xpm->xpm_flags = 0;
		}

		XNET_UNLOCK();

		// there's no thread structure, so make one
		xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
		if (!xcc) {
			throw -1;
		}

		xcc->xcc_map_handle = xpm->xpm_handle;
		xcc->xcc_mapped_addr =
			(UCHAR *) xpm->xpm_address + XPS_SLOT_OFFSET(global_pages_per_slot, slot_num);
		xcc->xcc_map_num = map_num;
		xcc->xcc_slot = slot_num;
		xcc->xcc_xpm = xpm;
		xcc->xcc_flags = 0;
		xcc->xcc_proc_h = 0;

		xps = (XPS) xcc->xcc_mapped_addr;

		// only speak if server has correct protocol
		if (xps->xps_server_protocol != XPI_SERVER_PROTOCOL_VERSION) {
			throw -1;
		}

		xps->xps_client_protocol = XPI_CLIENT_PROTOCOL_VERSION;

		// open server process handle to watch server health during communication session
		xcc->xcc_proc_h = OpenProcess(SYNCHRONIZE, 0, xps->xps_server_proc_id);
		if (!xcc->xcc_proc_h) {
			throw -1;
		}

		xpm->xpm_count++;

#ifdef WIN_NT

		sprintf(name_buffer, XNET_E_C2S_DATA_CHAN_FILLED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_send_channel_filled =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_C2S_DATA_CHAN_EMPTED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_send_channel_empted =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_empted) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_DATA_CHAN_FILLED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_recv_channel_filled =
				OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_DATA_CHAN_EMPTED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_recv_channel_empted =
				OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_empted) {
			throw -1;
		}

#else
		// STUB : need to call event init for unix
#endif

		/* added this here from the server side as this part is called by the client 
		   and the server address need not be valid for the client -smistry 10/29/98 */
		xcc->xcc_recv_channel = &xps->xps_channels[XPS_CHANNEL_S2C_DATA];
		xcc->xcc_send_channel = &xps->xps_channels[XPS_CHANNEL_C2S_DATA];

		/* we also need to add client side flags or channel pointer as they 
		   differ from the server side */

		const ULONG avail =
			(ULONG) (XPS_USEFUL_SPACE(global_pages_per_slot) - (XNET_EVENT_SPACE * 2)) / 2;
		UCHAR* start_ptr =
			(UCHAR*) xps + (sizeof(struct xps) + (XNET_EVENT_SPACE * 2));

		/* send channel */
		xps->xps_channels[XPS_CHANNEL_C2S_DATA].xch_client_ptr = start_ptr;
		/* receive channel */
		xps->xps_channels[XPS_CHANNEL_S2C_DATA].xch_client_ptr = (start_ptr + avail);

	}
	catch (...) {

#ifdef WIN_NT

			if (file_handle) {
				if (mapped_address) {
					UnmapViewOfFile(mapped_address);
				}
				CloseHandle(file_handle);
			}

			if (xpm) {
				ALLR_free(xpm);
			}

			if (xcc) {
				if (xcc->xcc_event_send_channel_filled) {
					CloseHandle(xcc->xcc_event_send_channel_filled);
				}
				if (xcc->xcc_event_send_channel_empted) {
					CloseHandle(xcc->xcc_event_send_channel_empted);
				}
				if (xcc->xcc_event_recv_channel_filled) {
					CloseHandle(xcc->xcc_event_recv_channel_filled);
				}
				if (xcc->xcc_event_recv_channel_empted) {
					CloseHandle(xcc->xcc_event_recv_channel_empted);
				}
				ALLR_free(xcc);
			}
		
#else
		// Something for Unix ...
#endif

		return NULL;
	}

	port = xnet_alloc_port(0, xcc->xcc_send_channel->xch_client_ptr,
						   xcc->xcc_send_channel->xch_size,
						   xcc->xcc_recv_channel->xch_client_ptr,
						   xcc->xcc_recv_channel->xch_size);
	if (port) {
		status_vector[1] = FB_SUCCESS;
		port->port_status_vector = status_vector;
		port->port_xcc = (void *) xcc;
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, port);
		send_full(port, packet);
	}
	return port;

#endif //SUPERCLIENT
}


#ifdef SUPERCLIENT
static bool_t xnet_connect_init()
{
/**************************************
 *
 *  x n e t _ c o n n e c t _ i n i t
 *
 **************************************
 *
 * Functional description
 *  Initialization of client side resources used
 *  when client performs connect to server
 *
 **************************************/
	TEXT name_buffer[128];

	xnet_connect_mutex = 0;
	xnet_connect_map_h = 0;
	xnet_connect_map = 0;

	xnet_connect_event = 0;
	xnet_response_event = 0;

	try {
		sprintf(name_buffer, XNET_MU_CONNECT_MUTEX, XNET_PREFIX);
		xnet_connect_mutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, name_buffer);
		if (!xnet_connect_mutex) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_CONNECT_EVENT, XNET_PREFIX);
		xnet_connect_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xnet_connect_event) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_RESPONSE_EVENT, XNET_PREFIX);
		xnet_response_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xnet_response_event) {
			throw -1;
		}

		sprintf(name_buffer, XNET_MA_CONNECT_MAP, XNET_PREFIX);
		xnet_connect_map_h = OpenFileMapping(FILE_MAP_WRITE, TRUE, name_buffer);
		if (!xnet_connect_map_h) {
			throw -1;
		}

		xnet_connect_map =
			MapViewOfFile(xnet_connect_map_h, FILE_MAP_WRITE, 0, 0,
						  XNET_CONNECT_RESPONZE_SIZE);
		if (!xnet_connect_map) {
			throw -1;
		}

		return TRUE;
	}
	catch (...) {
		xnet_connect_fini();
		return FALSE;
	}

}
#endif // SUPERCLIENT

static void xnet_connect_fini()
{
/**************************************
 *
 *  x n e t _ c o n n e c t _ f i n i
 *
 **************************************
 *
 * Functional description
 *  Release resources allocated in
 *  xnet_connect_init() / xnet_srv_init()
 *
 **************************************/

#ifdef WIN_NT

	if (xnet_connect_mutex)
	{
		CloseHandle(xnet_connect_mutex);
		xnet_connect_mutex = 0;
	}

	if (xnet_connect_event)
	{
		CloseHandle(xnet_connect_event);
		xnet_connect_event = 0;
	}

	if (xnet_response_event)
	{
		CloseHandle(xnet_response_event);
		xnet_response_event = 0;
	}

	if (xnet_connect_map)
	{
		UnmapViewOfFile(xnet_connect_map);
		xnet_connect_map = 0;
	}

	if (xnet_connect_map_h)
	{
		CloseHandle(xnet_connect_map_h);
		xnet_connect_map_h = 0;
	}

#endif // WIN_NT
}


static int accept_connection(rem_port* port, P_CNCT * cnct)
{
/**************************************
 *
 *	a c c e p t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Accept an incoming request for connection.
 *
 **************************************/
	port->port_protocol_str = REMOTE_make_string("XNET");

	// Use client process ID as remote address for XNET protocol
	XCC xcc = (XCC) port->port_xcc;
	if (xcc) {
		XPS xps = (XPS) xcc->xcc_mapped_addr;
		if (xps) {
			Firebird::string address;
			address.printf("%u", xps->xps_client_proc_id);
			port->port_address_str = REMOTE_make_string(address.c_str());
		}
	}

	return TRUE;
}


static rem_port* xnet_alloc_port(rem_port* parent,
							UCHAR * send_buffer,
							ULONG send_length,
							UCHAR * receive_buffer,
							ULONG receive_length)
{
/**************************************
 *
 *	x n e t _ a l l o c _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Allocate a port block, link it in to parent (if there is a parent),
 *	and initialize input and output XDR streams.
 *
 **************************************/
	rem_port* port = (rem_port*) ALLR_block(type_port, 0);
	port->port_type = port_xnet;
	port->port_state = state_pending;

	TEXT buffer[64];
	ISC_get_host(buffer, sizeof(buffer));
	port->port_host = REMOTE_make_string(buffer);
	port->port_connection = REMOTE_make_string(buffer);
	sprintf(buffer, "XNET Server (%s)", port->port_host->str_data);
	port->port_version = REMOTE_make_string(buffer);
	if (parent) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = parent->port_next = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_server_flags = parent->port_server_flags;
		if (port->port_connection)
			ALLR_free((UCHAR *) port->port_connection);
		port->port_connection =
			REMOTE_make_string(parent->port_connection->str_data);
	}

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
	port->port_buff_size = send_length;
	port->port_status_vector = NULL;

	xdrxnet_create(&port->port_send, port, send_buffer,	send_length, XDR_ENCODE);
	xdrxnet_create(&port->port_receive, port, receive_buffer, 0, XDR_DECODE);

	return port;
}


// Third param "ast" is unused.
static rem_port* aux_connect(rem_port* port, PACKET* packet, t_event_ast ast)
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Try to establish an alternative connection for handling events.
 *  Somebody has already done a successfull connect request.
 *  This uses the existing xcc for the parent port to more
 *  or less duplicate a new xcc for the new aux port pointing
 *  to the event stuff in the map.
 *
 **************************************/

	if (port->port_server_flags) {
		port->port_flags |= PORT_async;
		return port;
	}

 	rem_port* new_port = NULL;
	XCC parent_xcc = NULL;
	XCC xcc = NULL;
	TEXT name_buffer[128];
	XPS xps = NULL;
	XPM xpm = NULL;

	try {

		// make a new xcc
		parent_xcc = (XCC) port->port_xcc;
		xps = (XPS) parent_xcc->xcc_mapped_addr;

		xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
		if (!xcc) {
			throw -1;
		}

		xpm = xcc->xcc_xpm = parent_xcc->xcc_xpm;
		xcc->xcc_map_num = parent_xcc->xcc_map_num;
		xcc->xcc_slot = parent_xcc->xcc_slot;

#ifdef WIN_NT
		xcc->xcc_proc_h = parent_xcc->xcc_proc_h;
#endif

		xcc->xcc_flags = 0;
		xcc->xcc_map_handle = parent_xcc->xcc_map_handle;
		xcc->xcc_mapped_addr = parent_xcc->xcc_mapped_addr;
		xcc->xcc_xpm->xpm_count++;

		sprintf(name_buffer, XNET_E_C2S_EVNT_CHAN_FILLED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_send_channel_filled =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_C2S_EVNT_CHAN_EMPTED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_send_channel_empted =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_empted) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_EVNT_CHAN_FILLED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_recv_channel_filled =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_EVNT_CHAN_EMPTED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_recv_channel_empted =
			OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_empted) {
			throw -1;
		}

		// send events channel
		xps->xps_channels[XPS_CHANNEL_C2S_EVENTS].xch_client_ptr =
			((UCHAR *) xpm->xpm_address + sizeof(struct xps));

		// receive events channel
		xps->xps_channels[XPS_CHANNEL_S2C_EVENTS].xch_client_ptr =
			((UCHAR *) xpm->xpm_address + sizeof(struct xps) + (XNET_EVENT_SPACE));

		xcc->xcc_send_channel = &xps->xps_channels[XPS_CHANNEL_C2S_EVENTS];		
		xcc->xcc_recv_channel = &xps->xps_channels[XPS_CHANNEL_S2C_EVENTS];

		// alloc new port and link xcc to it
		new_port = xnet_alloc_port(NULL,
								   xcc->xcc_send_channel->xch_client_ptr,
								   xcc->xcc_send_channel->xch_size,
								   xcc->xcc_recv_channel->xch_client_ptr,
								   xcc->xcc_recv_channel->xch_size);
		if (!new_port) {
			throw -1;
		}

		port->port_async = new_port;
		new_port->port_flags = port->port_flags & PORT_no_oob;
		new_port->port_flags |= PORT_async;
		new_port->port_xcc = (void *) xcc;

		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, new_port);

		return new_port;
	}
	catch (...) {

		XNET_LOG_ERROR("aux_connect() failed");

#ifdef WIN_NT

		if (xcc) {
			if (xcc->xcc_event_send_channel_filled) {
				CloseHandle(xcc->xcc_event_send_channel_filled);
			}
			if (xcc->xcc_event_send_channel_empted) {
				CloseHandle(xcc->xcc_event_send_channel_empted);
			}
			if (xcc->xcc_event_recv_channel_filled) {
				CloseHandle(xcc->xcc_event_recv_channel_filled);
			}
			if (xcc->xcc_event_recv_channel_empted) {
				CloseHandle(xcc->xcc_event_recv_channel_empted);
			}
			ALLR_free(xcc);
		}
		
#else
		// Something for Unix ...
#endif

		return NULL;
	}
}


static rem_port* aux_request(rem_port* port, PACKET* packet)
{
/**************************************
 *
 *	a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *  A remote interface has requested the server to
 *  prepare an auxiliary connection.   This is done
 *  by allocating a new port and comm (xcc) structure,
 *  using the event stuff in the map rather than the
 *  normal database channels.
 *
 **************************************/

 	rem_port* new_port = NULL;
	XCC parent_xcc = NULL;
	XCC xcc = NULL;
	TEXT name_buffer[128];
	XPS xps = NULL;
	XPM xpm = NULL;

	try {

		// make a new xcc
		parent_xcc = (XCC) port->port_xcc;
		xps = (XPS) parent_xcc->xcc_mapped_addr;

		xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
		if (!xcc) {
			throw -1;
		}

		xpm = xcc->xcc_xpm = parent_xcc->xcc_xpm;
		xcc->xcc_map_num = parent_xcc->xcc_map_num;
		xcc->xcc_slot = parent_xcc->xcc_slot;

#ifdef WIN_NT
		xcc->xcc_proc_h = parent_xcc->xcc_proc_h;
#endif

		xcc->xcc_flags = 0;
		xcc->xcc_map_handle = parent_xcc->xcc_map_handle;
		xcc->xcc_mapped_addr = parent_xcc->xcc_mapped_addr;
		xcc->xcc_xpm->xpm_count++;

#ifdef WIN_NT

		sprintf(name_buffer, XNET_E_C2S_EVNT_CHAN_FILLED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_recv_channel_filled =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_filled ||
			(xcc->xcc_event_recv_channel_filled && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_E_C2S_EVNT_CHAN_EMPTED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_recv_channel_empted =
			CreateEvent(ISC_get_security_desc(), TRUE, TRUE, name_buffer);
		if (!xcc->xcc_event_recv_channel_empted ||
			(xcc->xcc_event_recv_channel_empted && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_EVNT_CHAN_FILLED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_send_channel_filled =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_filled ||
			(xcc->xcc_event_send_channel_filled && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_EVNT_CHAN_EMPTED,
				XNET_PREFIX, xcc->xcc_map_num, xcc->xcc_slot,
				(ULONG) xpm->xpm_timestamp);
		xcc->xcc_event_send_channel_empted =
			CreateEvent(ISC_get_security_desc(), TRUE, TRUE, name_buffer);
		if (!xcc->xcc_event_send_channel_empted ||
			(xcc->xcc_event_send_channel_empted && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

#else
	// STUB : This should be event_init() calls ?
#endif

		// send events channel
		xps->xps_channels[XPS_CHANNEL_S2C_EVENTS].xch_client_ptr =
			((UCHAR *) xpm->xpm_address + sizeof(struct xps) + (XNET_EVENT_SPACE));

		// receive events channel
		xps->xps_channels[XPS_CHANNEL_C2S_EVENTS].xch_client_ptr =
			((UCHAR *) xpm->xpm_address + sizeof(struct xps));

		xcc->xcc_send_channel = &xps->xps_channels[XPS_CHANNEL_S2C_EVENTS];		
		xcc->xcc_recv_channel = &xps->xps_channels[XPS_CHANNEL_C2S_EVENTS];

		// alloc new port and link xcc to it
		new_port = xnet_alloc_port(NULL,
								   xcc->xcc_send_channel->xch_client_ptr,
								   xcc->xcc_send_channel->xch_size,
								   xcc->xcc_recv_channel->xch_client_ptr,
								   xcc->xcc_recv_channel->xch_size);
		if (!new_port) {
			throw -1;
		}

		port->port_async = new_port;
		new_port->port_xcc = (void *) xcc;
		new_port->port_flags = port->port_flags & PORT_no_oob;
		new_port->port_server_flags = port->port_server_flags;

		return new_port;
	}
	catch (...) {

		XNET_LOG_ERROR("aux_request() failed");

#ifdef WIN_NT

		if (xcc) {

			if (xcc->xcc_event_send_channel_filled) {
				CloseHandle(xcc->xcc_event_send_channel_filled);
			}
			if (xcc->xcc_event_send_channel_empted) {
				CloseHandle(xcc->xcc_event_send_channel_empted);
			}
			if (xcc->xcc_event_recv_channel_filled) {
				CloseHandle(xcc->xcc_event_recv_channel_filled);
			}
			if (xcc->xcc_event_recv_channel_empted) {
				CloseHandle(xcc->xcc_event_recv_channel_empted);
			}
			ALLR_free(xcc);
		}
		
#else
		// Something for Unix ...
#endif

		return NULL;
	}
}


static void xnet_cleanup_comm(XCC xcc)
{
/**************************************
 *
 *  x n e t _ c l e a n u p _ c o m m
 *
 **************************************
 *
 * Functional description
 *  Clean up an xcc structure, close its handles,
 *  unmap its file, and free it.
 *
 **************************************/
#ifdef WIN_NT

	if (xcc->xcc_event_send_channel_filled) {
		CloseHandle(xcc->xcc_event_send_channel_filled);
	}
	if (xcc->xcc_event_send_channel_empted) {
		CloseHandle(xcc->xcc_event_send_channel_empted);
	}
	if (xcc->xcc_event_recv_channel_filled) {
		CloseHandle(xcc->xcc_event_recv_channel_filled);
	}
	if (xcc->xcc_event_recv_channel_empted) {
		CloseHandle(xcc->xcc_event_recv_channel_empted);
	}
	if (xcc->xcc_proc_h) {
		CloseHandle(xcc->xcc_proc_h);
	}

#endif // WIN_NT

	XPM xpm = xcc->xcc_xpm;

	ALLR_free((UCHAR *) xcc);

	// if this was the last area for this map, unmap it
	if (xpm) {
		xpm->xpm_count--;

		if (!xpm->xpm_count && global_client_maps) {
			UnmapViewOfFile(xpm->xpm_address);
			CloseHandle(xpm->xpm_handle);

			// find xpm in chain and release
			if (xpm == global_client_maps) {
				global_client_maps = xpm->xpm_next;
			}
			else {
				for (XPM pxpm = global_client_maps; pxpm->xpm_next;
					pxpm = pxpm->xpm_next)
				{
					if (pxpm->xpm_next == xpm) {
						pxpm->xpm_next = xpm->xpm_next;
						break;
					}
				}
			}
			ALLR_free((UCHAR *) xpm);
		}
	}
}


static void xnet_cleanup_port(rem_port* port)
{
/**************************************
 *
 *  c l e a n u p _ p o r t
 *
 **************************************
 *
 * Functional description
 *  Walk through the port structure freeing
 *  allocated memory and then free the port.
 *
 **************************************/

	if (port->port_xcc) {
		XNET_LOCK();
		xnet_cleanup_comm((XCC) port->port_xcc);
		XNET_UNLOCK();
	}
	
	if (port->port_version) {
		ALLR_free((UCHAR *) port->port_version);
	}

	if (port->port_connection) {
		ALLR_free((UCHAR *) port->port_connection);
	}

	if (port->port_user_name) {
		ALLR_free((UCHAR *) port->port_user_name);
	}

	if (port->port_host) {
		ALLR_free((UCHAR *) port->port_host);
	}

	if (port->port_object_vector) {
		ALLR_free((UCHAR *) port->port_object_vector);
	}

	if (port->port_protocol_str) {
		ALLR_free((UCHAR *) port->port_protocol_str);
	}

	if (port->port_address_str) {
		ALLR_free((UCHAR *) port->port_address_str);
	}

#ifdef SUPERSERVER
	if (port->port_status_vector) {
		ALLR_free(port->port_status_vector);
	}
#endif

	ALLR_free((BLK) port);
}


static void disconnect(rem_port* port)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break a remote connection.
 *
 **************************************/

/* If this is a sub-port, unlink it from it's parent */

	rem_port* parent = port->port_parent;
	if (parent != NULL) {

		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}

		for (rem_port** ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
		{
			if (*ptr == port) {
				*ptr = port->port_next;
				if (ptr == &parent->port_clients)
					parent->port_next = *ptr;
				break;
			}
		}
	}
	else if (port->port_async) {
		/* If we're MULTI_THREAD then we cannot free the port because another
		 * thread might be using it.  If we're SUPERSERVER we must free the
		 * port to avoid a memory leak.  What we really need to know is if we
		 * have multi-threaded events, but this is transport specific.
		 * -smistry 10/29/98 */
#if (defined (MULTI_THREAD) && !defined (SUPERSERVER))
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

	gds__unregister_cleanup((FPTR_VOID_PTR)(exit_handler), port);
		
	xnet_cleanup_port(port);
}


static void exit_handler(rem_port* main_port)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Free port resources or everything
 *
 **************************************/
	xnet_shutdown = TRUE;
	if (main_port) {
		disconnect(main_port);
	}
	else {
		xnet_release_all();
	}
}

#ifndef SUPERCLIENT
static bool_t xnet_make_map(ULONG map_number, time_t timestamp,
							FILE_ID* map_handle, CADDR_T* map_address)
{
/**************************************
 *
 *	x n e t _ m a k e _ m a p
 *
 **************************************
 *
 * Functional description
 *	Create mamory map
 *
 **************************************/
	TEXT name_buffer[128];

	sprintf(name_buffer, XNET_MAPPED_FILE_NAME, XNET_PREFIX, map_number, (ULONG) timestamp);
	*map_handle = CreateFileMapping((HANDLE) 0xFFFFFFFF,
		                              ISC_get_security_desc(),
		                              PAGE_READWRITE,
		                              0L,
		                              XPS_MAPPED_SIZE(global_slots_per_map, global_pages_per_slot),
		                              name_buffer);
	if (!(*map_handle) || (*map_handle && ERRNO == ERROR_ALREADY_EXISTS))
		return FALSE;

	*map_address = MapViewOfFile(*map_handle, FILE_MAP_WRITE, 0, 0,
								 XPS_MAPPED_SIZE(global_slots_per_map, global_pages_per_slot));
	if (!(*map_address)) {
		CloseHandle(*map_handle);
		return FALSE;
	}

	return TRUE;
}

static XPM xnet_make_xpm(ULONG map_number, time_t timestamp)
{
/**************************************
 *
 *      x n e t _ m a k e _ x p m
 *
 **************************************
 *
 * Functional description
 *      Create new xpm structure
 *
 **************************************/
	FILE_ID map_handle = 0;
	CADDR_T map_address = 0;

	if (!xnet_make_map(map_number, timestamp, &map_handle, &map_address))
		return NULL;

/* allocate a structure and initialize it */
	XPM xpm = (XPM) ALLR_alloc(sizeof(struct xpm));
	if (!xpm)
		return NULL;

	xpm->xpm_handle = map_handle;
	xpm->xpm_address = map_address;
	xpm->xpm_number = map_number;
	xpm->xpm_count = 0;
	xpm->xpm_timestamp = timestamp;

	for (USHORT i = 0; i < global_slots_per_map; i++) {
		xpm->xpm_ids[i] = XPM_FREE;
	}
	xpm->xpm_flags = 0;

	xpm->xpm_next = global_client_maps;
	global_client_maps = xpm;

	return xpm;
}
#endif

static rem_port* receive( rem_port* main_port, PACKET * packet)
{
/**************************************
 *
 *	r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a message from a port.
 *
 **************************************/

	if (!xdr_protocol(&main_port->port_receive, packet))
		packet->p_operation = op_exit;

	return main_port;
}


static int send_full( rem_port* port, PACKET * packet)
{
/**************************************
 *
 *	s e n d _ f u l l
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *  Flush data to remote interface
 *
 **************************************/

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

	if (xnet_write(&port->port_send))
		return TRUE;
	else {
		XNET_ERROR(port, "SetEvent()", isc_net_write_err, ERRNO);
		return FALSE;
	}
}


static int send_partial( rem_port* port, PACKET * packet)
{													
/**************************************
 *
 *	s e n d _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *
 **************************************/

	return xdr_protocol(&port->port_send, packet);
}

#ifdef SUPERCLIENT
#ifdef WIN_NT
static void xnet_on_server_shutdown(rem_port* port)
{
/**************************************
 *
 *      s e r v e r _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *   Server shutdown handler (client side only).
 *
 **************************************/
	XCC xcc = (XCC)port->port_xcc;
	XPM xpm = xcc->xcc_xpm;

	XNET_LOG_ERROR("Server shutdown detected");

	XNET_LOCK();

	xcc->xcc_flags |= XCCF_SERVER_SHUTDOWN;

	ULONG dead_proc_id = XPS(xpm->xpm_address)->xps_server_proc_id;

/* mark all mapped areas connected to server with dead_proc_id */
	for (xpm = global_client_maps; xpm; xpm = xpm->xpm_next) {
		if (XPS(xpm->xpm_address)->xps_server_proc_id == dead_proc_id)
		{
			xpm->xpm_flags |= XPMF_SERVER_SHUTDOWN;
			xpm->xpm_handle = 0;
			xpm->xpm_address = NULL;
		}
	}

	XNET_UNLOCK();
}
#endif	// WIN_NT
#endif	// SUPERCLIENT


static int xdrxnet_create(XDR * xdrs, rem_port* port, UCHAR * buffer,
						  USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *  x d r x n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *  Initialize an XDR stream.
 *
 **************************************/

	xdrs->x_public = (caddr_t) port;
	xdrs->x_private = (SCHAR *) buffer;
	xdrs->x_base = xdrs->x_private;
	xdrs->x_handy = length;
	xdrs->x_ops = &xnet_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static int xnet_destroy( XDR * xdrs)
{
/**************************************
 *
 *      x n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/

	return 0;
}


static void xnet_gen_error( rem_port* port, ISC_STATUS status, ...)
{
/**************************************
 *
 *      x n e t _ g e n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error has occurred.  Mark the port as broken.
 *	Format the status vector if there is one and
 *	save the status vector strings in a permanent place.
 *
 **************************************/
	ISC_STATUS *status_vector = NULL;

	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	if (port->port_context != NULL)
		status_vector = port->port_context->rdb_status_vector;
	if (status_vector == NULL)
		status_vector = port->port_status_vector;
	if (status_vector != NULL) {
		STUFF_STATUS(status_vector, status)
		REMOTE_save_status_strings(status_vector);
	}
}


static int xnet_error(rem_port* port, const TEXT* function, ISC_STATUS operation,
											int status, ULONG source_line_num)
{
/**************************************
 *
 *      x n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  If a status vector is present,
 *	generate an error return.  In any case, return NULL, which
 *	is used to indicate and error.
 *
 **************************************/
	xnet_log_error(source_line_num, function, status);

	if (status)
		xnet_gen_error(port, operation, SYS_ERR, status, 0);
	else
		xnet_gen_error(port, operation, 0);
	return 0;
}


static bool_t xnet_getbytes(XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *      x n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Fetch a bunch of bytes from remote interface.
 *
 **************************************/

	SLONG bytecount = count;

	rem_port* port = (rem_port*)xdrs->x_public;
	XCC xcc = (XCC)port->port_xcc;
	XCH xch = (XCH)xcc->xcc_recv_channel;
	XPM xpm = xcc->xcc_xpm;

	while (bytecount && !xnet_shutdown) {

#ifdef SUPERCLIENT
		if (xpm->xpm_flags & XPMF_SERVER_SHUTDOWN) {
			if (!(xcc->xcc_flags & XCCF_SERVER_SHUTDOWN)) {
				xcc->xcc_flags |= XCCF_SERVER_SHUTDOWN;
				XNET_ERROR(port, "connection lost: another side is dead", 
						   isc_lost_db_connection, 0);
			}
			return FALSE;
		}
#endif

		SLONG to_copy;
		if (xdrs->x_handy >= bytecount)
			to_copy = bytecount;
		else
			to_copy = xdrs->x_handy;
		
		if (xdrs->x_handy) {
			if (to_copy == sizeof(SLONG))
				*((SLONG*)buff)	= *((SLONG*)xdrs->x_private);
			else
				memcpy(buff, xdrs->x_private, to_copy);

			xdrs->x_handy -= to_copy;
			xdrs->x_private += to_copy;
		}
		else {
			THREAD_EXIT();
			if (!xnet_read(xdrs)) {
				THREAD_ENTER();
				return FALSE;
			}
			THREAD_ENTER();
		}

		if (to_copy) {
			bytecount -= to_copy;
			buff += to_copy;
		}
	}

	if (xnet_shutdown)
		return FALSE;
	else
		return TRUE;
}


static bool_t xnet_getlong(XDR * xdrs,  SLONG * lp)
{
/**************************************
 *
 *      x n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword from memory stream.
 *
 **************************************/

	return (*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast<SCHAR*>(lp), 4);
}


static u_int xnet_getpostn(XDR * xdrs)
{
/**************************************
 *
 *      x n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return (u_int) (xdrs->x_private - xdrs->x_base);
}


static caddr_t xnet_inline(XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *      x n e t _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *	Return a pointer to somewhere in the buffer.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t xnet_putbytes(XDR* xdrs, const SCHAR* buff, u_int count)
{
/**************************************
 *
 *      x n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Put a bunch of bytes into a memory stream.
 *
 **************************************/
	SLONG bytecount = count;

	rem_port* port = (rem_port*)xdrs->x_public;
	XCC xcc = (XCC)port->port_xcc;
	XCH xch = (XCH)xcc->xcc_send_channel;
	XPM xpm = xcc->xcc_xpm;


	while (bytecount && !xnet_shutdown) {

#ifdef SUPERCLIENT
		if (xpm->xpm_flags & XPMF_SERVER_SHUTDOWN) {
			if (!(xcc->xcc_flags & XCCF_SERVER_SHUTDOWN)) {
				xcc->xcc_flags |= XCCF_SERVER_SHUTDOWN;
				XNET_ERROR(port, "connection lost: another side is dead", 
						   isc_lost_db_connection, 0);
			}
			return FALSE;
		}
#endif
		
		SLONG to_copy;
		if (xdrs->x_handy >= bytecount)
			to_copy = bytecount;
		else
			to_copy = xdrs->x_handy;

		if (xdrs->x_handy) {

			if ((ULONG) xdrs->x_handy == xch->xch_size) {

				THREAD_EXIT();
				while (!xnet_shutdown) {

					const DWORD wait_result =
						WaitForSingleObject(xcc->xcc_event_send_channel_empted,
													  XNET_SEND_WAIT_TIMEOUT);
					if (wait_result == WAIT_OBJECT_0)
						break;

					if (wait_result == WAIT_TIMEOUT) {

						/* Check if another side is alive */
						if (WaitForSingleObject(xcc->xcc_proc_h, 1) == WAIT_TIMEOUT)
							continue; /* Another side is alive */			
						else {

							/* Another side is dead or somthing bad has happaned*/
#ifdef SUPERCLIENT
							xnet_on_server_shutdown(port);
							XNET_ERROR(port, "connection lost: another side is dead", 
							           isc_lost_db_connection, 0);								
#else
							XNET_ERROR(port, "connection lost: another side is dead",
								       isc_conn_lost, 0);
#endif

							THREAD_ENTER();
							return FALSE;
						}
					}
					else {
						XNET_ERROR(port, "WaitForSingleObject()", isc_net_write_err, ERRNO);
						THREAD_ENTER();
						return FALSE; /* a non-timeout result is an error */
					}
				}

				THREAD_ENTER();
			}

			if (to_copy == sizeof(SLONG))
				*((SLONG*)xdrs->x_private) = *((SLONG*)buff);
			else
				memcpy(xdrs->x_private, buff, to_copy);

			xdrs->x_handy -= to_copy;
			xdrs->x_private += to_copy;
		}
		else {
			if (!xnet_write(xdrs)) {
				XNET_ERROR(port, "SetEvent()", isc_net_write_err, ERRNO);
				return FALSE;
			}
		}

		if (to_copy) {
			bytecount -= to_copy;
			buff += to_copy;
		}
	}

	if (xnet_shutdown)
		return FALSE;
	else
		return TRUE;
}


static bool_t xnet_putlong(XDR * xdrs, const SLONG* lp)
{
/**************************************
 *
 *      x n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fit a longword into a memory stream if it fits.
 *
 **************************************/

	return (*xdrs->x_ops->x_putbytes) (xdrs, reinterpret_cast<const char*>(AOF32L(*lp)), 4);
}


static bool_t xnet_read(XDR * xdrs)
{
/**************************************
 *
 *      x n e t _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a buffer full of data.
 *
 **************************************/
	rem_port* port = (rem_port*)xdrs->x_public;
	XCC xcc = (XCC)port->port_xcc;
	XCH xch = (XCH)xcc->xcc_recv_channel;
	XPM xpm = xcc->xcc_xpm;

	if (xnet_shutdown)
		return FALSE;

	if (!SetEvent(xcc->xcc_event_recv_channel_empted)) {
		XNET_ERROR(port, "SetEvent()", isc_net_read_err, ERRNO);
		return FALSE;
	}

	while (!xnet_shutdown) {

#ifdef SUPERCLIENT
		if (xpm->xpm_flags & XPMF_SERVER_SHUTDOWN) {
			if (!(xcc->xcc_flags & XCCF_SERVER_SHUTDOWN)) {
				xcc->xcc_flags |= XCCF_SERVER_SHUTDOWN;
				XNET_ERROR(port, "connection lost: another side is dead", 
						   isc_lost_db_connection, 0);
			}
		}
#endif

		const DWORD wait_result =
			WaitForSingleObject(xcc->xcc_event_recv_channel_filled,
		                                  XNET_RECV_WAIT_TIMEOUT);
		if (wait_result == WAIT_OBJECT_0) {
			/* Client wrote some data for us(for server) to read*/
			xdrs->x_handy = xch->xch_length;
			xdrs->x_private = xdrs->x_base;
			return TRUE;
		}

		if (wait_result == WAIT_TIMEOUT) {

			/* Check if another side is alive */
			if (WaitForSingleObject(xcc->xcc_proc_h, 1) == WAIT_TIMEOUT)
				continue; /* Another side is alive */			
			else {

				/* Another side is dead or somthing bad has happaned*/
#ifdef SUPERCLIENT
				xnet_on_server_shutdown(port);
				XNET_ERROR(port, "connection lost: another side is dead", 
						   isc_lost_db_connection, 0);								
#else
				XNET_ERROR(port, "connection lost: another side is dead",
				           isc_conn_lost, 0);
#endif

				return FALSE;
			}
		}
		else {
			XNET_ERROR(port, "WaitForSingleObject()", isc_net_read_err, ERRNO);
			return FALSE; /* a non-timeout result is an error */
		}
	}

	return FALSE;
	
}


static bool_t xnet_write(XDR * xdrs)
{
/**************************************
 *
 *      x n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Signal remote interface that memory stream is
 *  filled and ready for reading.
 *
 **************************************/
	rem_port* port = (rem_port*)xdrs->x_public;
	XCC xcc = (XCC)(port)->port_xcc;
	XCH xch = (XCH)xcc->xcc_send_channel;

	xch->xch_length = xdrs->x_private - xdrs->x_base;
	if (SetEvent(xcc->xcc_event_send_channel_filled)) {
		port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
		xdrs->x_private = xdrs->x_base;
		xdrs->x_handy = xch->xch_size;

		return TRUE;
	}
	else
		return FALSE;

}


static bool_t xnet_setpostn(XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *      x n e t _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Set the current position (which is also current length) from stream.
 *
 **************************************/

	if (bytecount > (u_int) xdrs->x_handy)
		return FALSE;
	xdrs->x_private = xdrs->x_base + bytecount;
	return TRUE;
}


void xnet_release_all()
{
/**************************************
 *
 *      x n e t _ r e l e a s e _ a l l
 *
 **************************************
 *
 * Functional description
 *      Release all connections and dependant stuff.
 *
 **************************************/

	if (!xnet_initialized)
		return;

#ifndef SUPERCLIENT
	xnet_connect_fini();
#endif

	THD_mutex_lock(&xnet_mutex);

/* release all map stuf left not released by broken ports */
	XPM xpm, nextxpm;
	for (xpm = nextxpm = global_client_maps; nextxpm; xpm = nextxpm) {
		nextxpm = nextxpm->xpm_next;
		UnmapViewOfFile(xpm->xpm_address);
		CloseHandle(xpm->xpm_handle);
		ALLR_free((UCHAR *) xpm);
	}

	global_client_maps = NULL;

	THD_mutex_unlock(&xnet_mutex);

	xnet_initialized = FALSE;
}


/***********************************************************************/
/********************** ONLY SERVER CODE FROM HERE *********************/
/***********************************************************************/

#ifndef SUPERCLIENT

static bool_t xnet_srv_init()
{
/**************************************
 *
 *  X N E T _ s r v _ i n i t
 *
 **************************************
 *
 * Functional description
 *  Initialization of server side resources used
 *  when clients perform connect to server
 *
 **************************************/
	TEXT name_buffer[128];

	// init the limits
#ifdef SUPERSERVER
	global_slots_per_map = XPS_MAX_NUM_CLI;
	global_pages_per_slot = XPS_MAX_PAGES_PER_CLI;
#else
	// For classic server there is always only 1 connection and 1 slot
	global_slots_per_map = 1;
	global_pages_per_slot = XPS_MAX_PAGES_PER_CLI;
#endif

	xnet_connect_mutex = 0;
	xnet_connect_map_h = 0;
	xnet_connect_map = 0;

	xnet_connect_event = 0;
	xnet_response_event = 0;

	try {
		sprintf(name_buffer, XNET_MU_CONNECT_MUTEX, XNET_PREFIX);
		xnet_connect_mutex =
			CreateMutex(ISC_get_security_desc(), FALSE, name_buffer);
		if (!xnet_connect_mutex || (xnet_connect_mutex && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_E_CONNECT_EVENT, XNET_PREFIX);
		xnet_connect_event =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xnet_connect_event || (xnet_connect_event && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_E_RESPONSE_EVENT, XNET_PREFIX);
		xnet_response_event =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xnet_response_event || (xnet_response_event && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		sprintf(name_buffer, XNET_MA_CONNECT_MAP, XNET_PREFIX);
		xnet_connect_map_h = CreateFileMapping((HANDLE)0xFFFFFFFF,
												ISC_get_security_desc(),
												PAGE_READWRITE,
												0,
												XNET_CONNECT_RESPONZE_SIZE,
												name_buffer);
		if (!xnet_connect_map_h || (xnet_connect_map_h && ERRNO == ERROR_ALREADY_EXISTS))
		{
			throw -1;
		}

		xnet_connect_map = MapViewOfFile(xnet_connect_map_h, FILE_MAP_WRITE, 0L, 0L,
										 XNET_CONNECT_RESPONZE_SIZE);
		if (!xnet_connect_map) {
			throw -1;
		}
	
		return TRUE;
	}
	catch (...) {
		xnet_connect_fini();
		return FALSE;
	}
	
}

#ifndef SUPERSERVER

static bool_t xnet_fork(ULONG client_pid, USHORT flag, ULONG* forken_pid)
{
/**************************************
 *
 *  x n e t _ f o r k
 *
 **************************************
 *
 * Functional description
 *  Create child process to serve client connection
 *  It's for classic server only
 *
 **************************************/
	if (!XNET_command_line[0]) {

#ifdef CMDLINE_VIA_SERVICE_MANAGER

		SC_HANDLE service;
		SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
		if ((manager) &&
			(service = OpenService(manager, REMOTE_SERVICE, SERVICE_QUERY_CONFIG)))
		{
			LPQUERY_SERVICE_CONFIG config;
			SCHAR buffer[1024];
			DWORD config_len;

			config = (LPQUERY_SERVICE_CONFIG) buffer;
			THREAD_EXIT();
			if (!QueryServiceConfig(service, config, sizeof(buffer), &config_len))
			{
				config = (LPQUERY_SERVICE_CONFIG) ALLR_alloc(config_len);
				/* NOMEM: ALLR_alloc handled */
				/* FREE:  later in this block */
				QueryServiceConfig(service, config, config_len, &config_len);
			}
			strcpy(XNET_command_line, config->lpBinaryPathName);
			if ((SCHAR *) config != buffer) {
				ALLR_free(config);
			}
			CloseServiceHandle(service);
			THREAD_ENTER();
		}
		else {
			strcpy(XNET_command_line, GetCommandLine());
		}
		CloseServiceHandle(manager);
#else
		strcpy(XNET_command_line, GetCommandLine());
#endif
		XNET_p = XNET_command_line + strlen(XNET_command_line);
	}

	sprintf(XNET_p, " -s -x -h %"ULONGFORMAT, (ULONG) client_pid);
	
	STARTUPINFO start_crud;
	start_crud.cb = sizeof(STARTUPINFO);
	start_crud.lpReserved = NULL;
	start_crud.lpReserved2 = NULL;
	start_crud.cbReserved2 = 0;
	start_crud.lpDesktop = NULL;
	start_crud.lpTitle = NULL;
	start_crud.dwFlags = STARTF_FORCEOFFFEEDBACK;
	PROCESS_INFORMATION pi;
	
	const bool_t cp_result = CreateProcess(NULL,
							  XNET_command_line,
							  NULL,
							  NULL,
							  TRUE,
							  (flag & SRVR_high_priority ? HIGH_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS)
								| DETACHED_PROCESS | CREATE_SUSPENDED | STARTF_FORCEOFFFEEDBACK,
							  NULL, NULL, &start_crud, &pi);

	// Child process ID (forken_pid) used as map number

	if (cp_result) {
		*forken_pid = pi.dwProcessId;
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
		XNET_LOG_ERRORC("CreateProcess() failed");

	return cp_result;
}

#endif // SUPERSERVER

#ifdef SUPERSERVER

static XPM xnet_get_free_slot(ULONG* map_num, ULONG* slot_num, time_t* timestamp)
{
/**************************************
 *
 *  x n e t _ g e t _ f r e e _ s l o t
 *
 **************************************
 *
 * Functional description
 *  Search for free slot in map stuff
  *
 **************************************/

	XPM xpm = NULL;
	ULONG i = 0, j = 0;

	/* go through list of maps */
	for (xpm = global_client_maps; xpm; xpm = xpm->xpm_next) {
		/* find an available unused comm area */

		for (i = 0; i < global_slots_per_map; i++)
			if (xpm->xpm_ids[i] == XPM_FREE)
				break;

		if (i < global_slots_per_map) {
			xpm->xpm_count++;
			xpm->xpm_ids[i] = XPM_BUSY;
			j = xpm->xpm_number;
			break;
		}
		j++;
	}

/* if the mapped file structure has not yet been initialized,
   make one now */

	if (!xpm) {
		/* allocate new map file and first slot */

		xpm = xnet_make_xpm(j, *timestamp);

		/* check for errors in creation of mapped file */
		if (!xpm) {
			XNET_UNLOCK();
			return NULL;
		}

		i = 0;
		xpm->xpm_ids[0] = XPM_BUSY;
		xpm->xpm_count++;
	}
	else
		*timestamp = xpm->xpm_timestamp;

	*map_num = j;
	*slot_num = i;

	return xpm;
}

#endif // SUPERSERVER


static rem_port* xnet_get_srv_port(ULONG client_pid, XPM xpm,
                              ULONG map_num, ULONG slot_num,
															time_t timestamp,
															ISC_STATUS* status_vector)
{
/**************************************
 *
 *  X N E T _ g e t s r v _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Allocates	new rem_port for server side communication .
 *
 **************************************/
	rem_port* port = NULL;
	TEXT name_buffer[128];

	// allocate a communications control structure and fill it in
	XCC xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
	if (!xcc)
		return NULL;

	try {
		UCHAR* p = (UCHAR *) xpm->xpm_address + XPS_SLOT_OFFSET(global_pages_per_slot, slot_num);
		memset(p, (char) 0, XPS_MAPPED_PER_CLI(global_pages_per_slot));
		xcc->xcc_next = NULL;
		xcc->xcc_mapped_addr = p;
		xcc->xcc_xpm = xpm;
		xcc->xcc_slot = slot_num;
		xcc->xcc_flags = 0;

		// Open client process handle to watch clients health during communication session
		xcc->xcc_proc_h = OpenProcess(SYNCHRONIZE, 0, client_pid);
		if (!xcc->xcc_proc_h) {
			throw -1;
		}

		xcc->xcc_map_num = map_num;
		XPS xps = (XPS) xcc->xcc_mapped_addr;
		xps->xps_client_proc_id = client_pid;

#ifdef WIN_NT
		xps->xps_server_proc_id = CurrentProcessId;
#else
		// STUB : need something special for Solaris? I don't think so.
		xps->xps_server_proc_id = getpid();
#endif

		// make sure client knows what this server speaks

		xps->xps_server_protocol = XPI_SERVER_PROTOCOL_VERSION;
		xps->xps_client_protocol = 0L;

#ifdef WIN_NT

		sprintf(name_buffer, XNET_E_C2S_DATA_CHAN_FILLED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_recv_channel_filled =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_C2S_DATA_CHAN_EMPTED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_recv_channel_empted =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_recv_channel_empted) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_DATA_CHAN_FILLED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_send_channel_filled =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_filled) {
			throw -1;
		}

		sprintf(name_buffer, XNET_E_S2C_DATA_CHAN_EMPTED,
				XNET_PREFIX, map_num, slot_num, (ULONG) timestamp);
		xcc->xcc_event_send_channel_empted =
			CreateEvent(ISC_get_security_desc(), FALSE, FALSE, name_buffer);
		if (!xcc->xcc_event_send_channel_empted) {
			throw -1;
		}

#else
		// STUB : add create events here
#endif

		// set up the channel structures

		p += sizeof(struct xps);

		const ULONG avail =
			(USHORT) (XPS_USEFUL_SPACE(global_pages_per_slot) - (XNET_EVENT_SPACE * 2)) / 2;

		xps->xps_channels[XPS_CHANNEL_C2S_EVENTS].xch_buffer = p;	/* client to server events */
		xps->xps_channels[XPS_CHANNEL_C2S_EVENTS].xch_size = XNET_EVENT_SPACE;

		p += XNET_EVENT_SPACE;
		xps->xps_channels[XPS_CHANNEL_S2C_EVENTS].xch_buffer = p;	/* server to client events */
		xps->xps_channels[XPS_CHANNEL_S2C_EVENTS].xch_size = XNET_EVENT_SPACE;

		p += XNET_EVENT_SPACE;
		xps->xps_channels[XPS_CHANNEL_C2S_DATA].xch_buffer = p;	/* client to server data */
		xps->xps_channels[XPS_CHANNEL_C2S_DATA].xch_size = avail;

		p += avail;
		xps->xps_channels[XPS_CHANNEL_S2C_DATA].xch_buffer = p;	/* server to client data */
		xps->xps_channels[XPS_CHANNEL_S2C_DATA].xch_size = avail;

		xcc->xcc_recv_channel = &xps->xps_channels[XPS_CHANNEL_C2S_DATA];
		xcc->xcc_send_channel = &xps->xps_channels[XPS_CHANNEL_S2C_DATA];

	/* finally, allocate and set the port structure for this client */
		port = xnet_alloc_port(0,
			                   xcc->xcc_send_channel->xch_buffer,
			                   xcc->xcc_send_channel->xch_size,
			                   xcc->xcc_recv_channel->xch_buffer,
			                   xcc->xcc_recv_channel->xch_size);
		if (port) {
			port->port_xcc = (void *) xcc;
			port->port_server_flags |= SRVR_server;

			status_vector[0] = isc_arg_gds;
			status_vector[1] = FB_SUCCESS;
			status_vector[2] = isc_arg_end;
			port->port_status_vector = status_vector;
		}
		else {
			throw -1;
		}

		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, port);

	}
	catch (...) {
		if (xcc) {
			if (xcc->xcc_proc_h) {
				CloseHandle(xcc->xcc_proc_h);
			}

			if (xcc->xcc_event_recv_channel_filled) {
				CloseHandle(xcc->xcc_event_recv_channel_filled);
			}
			if (xcc->xcc_event_recv_channel_empted) {
				CloseHandle(xcc->xcc_event_recv_channel_empted);
			}
			if (xcc->xcc_event_send_channel_filled) {
				CloseHandle(xcc->xcc_event_send_channel_filled);
			}
			if (xcc->xcc_event_send_channel_empted) {
				CloseHandle(xcc->xcc_event_send_channel_empted);
			}

			ALLR_free(xcc);
		}
		return NULL;
	}

	return port;
}


void XNET_srv(USHORT flag)
{
/**************************************
 *
 *  X N E T _ s r v
 *
 **************************************
 *
 * Functional description
 *	XNET server main thread
 *
 **************************************/

#ifdef WIN_NT

#ifdef SUPERSERVER
	rem_port* port;
	ULONG map_num, slot_num;
	XPM xpm;

	ISC_STATUS* status_vector;
#endif

	CurrentProcessId = GetCurrentProcessId();
	XNET_command_line[0] = 0;

	if (xnet_srv_init()) {
		xnet_initialized = TRUE;
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, NULL);
	}
	else {
		XNET_LOG_ERROR("XNET server initialization failed");
		return;
	}

	PXNET_RESPONSE presponse = (PXNET_RESPONSE)xnet_connect_map;

	while (!xnet_shutdown) {

		// mark connect area as untouched
//		presponse->proc_id = CurrentProcessId;

		THREAD_EXIT();
		const DWORD wait_res = WaitForSingleObject(xnet_connect_event, INFINITE);
		THREAD_ENTER();

		if (wait_res != WAIT_OBJECT_0) {
			XNET_LOG_ERRORC("WaitForSingleObject() failed");
			break;
		}

		if (xnet_shutdown)
			break;

		// read client process id
		const ULONG client_pid = presponse->proc_id;
		if (client_pid == CurrentProcessId)
			continue; // dummy xnet_connect_event fire -  no connect request
			
		presponse->slots_per_map = global_slots_per_map;
		presponse->pages_per_slot = global_pages_per_slot;
		presponse->timestamp = time_t(0);

#ifdef SUPERSERVER

		time_t timestamp = time(NULL);
		// searhing for free slot
		XNET_LOCK();
		xpm = xnet_get_free_slot(&map_num, &slot_num, &timestamp);
		XNET_UNLOCK();

		// pack combined mapped area and number
		if (xpm) {
			presponse->proc_id = CurrentProcessId;
			presponse->map_num = map_num;
			presponse->slot_num = slot_num;
			presponse->timestamp = timestamp;

			status_vector = (ISC_STATUS*) ALLR_alloc(ISC_STATUS_LENGTH);
			port = xnet_get_srv_port(client_pid, xpm, map_num, slot_num,
				                     timestamp, status_vector);
		}
		else {
			XNET_LOG_ERROR("xnet_get_free_slot() failed");
			return;
		}
					
		if (xpm && port) {
			// start the thread for this client
			gds__thread_start(process_connection_thread,
				              port, THREAD_medium, 0, 0);
		}
		else {
			XNET_LOG_ERROR("failed to allocate server port for communication");
			return;
		}

		SetEvent(xnet_response_event);

#else // CS

		// in case process we'll fail to start child process
		presponse->slot_num = 0;

		// child process ID (presponse->map_num) used as map number
		if (!xnet_fork(client_pid, flag, &(presponse->map_num)))
			SetEvent(xnet_response_event);// if xnet_fork successfully creats child process
		                                // child process will call SetEvent(xnet_response_event)
		                                // by itself
#endif //SUPERSERVER

	}

#else

#endif // WIN_NT
}


rem_port* XNET_reconnect(ULONG client_pid, ISC_STATUS* status_vector)
{
/**************************************
 *
 *  X N E T _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Classic server initialization code
 *
 **************************************/

	rem_port* port = NULL;
	TEXT name_buffer[128];

	global_slots_per_map = 1;
	global_pages_per_slot = XPS_MAX_PAGES_PER_CLI;
	xnet_response_event = 0;

	// CurrentProcessId used as map number
	CurrentProcessId = GetCurrentProcessId();
	try {

		sprintf(name_buffer, XNET_E_RESPONSE_EVENT, XNET_PREFIX);
		xnet_response_event = OpenEvent(EVENT_ALL_ACCESS, FALSE, name_buffer);
		if (!xnet_response_event) {
			throw -1;
		}

		XPM xpm = xnet_make_xpm(CurrentProcessId, time_t(0));
		if (!xpm) {
			throw -1;
		}

		port = xnet_get_srv_port(client_pid, xpm, CurrentProcessId, 0, 0, status_vector);
		if (!port) {
			throw -1;
		}

		//make signal for client
		SetEvent(xnet_response_event);
		if (xnet_response_event) {
			CloseHandle(xnet_response_event);
		}

	}
	catch (...) {
		XNET_LOG_ERROR("Unable to initialize child process");
		status_vector[1] = isc_unavailable;

		if (port) {
			xnet_cleanup_port(port);
			port = NULL;
		}

		if (xnet_response_event) {
			SetEvent(xnet_response_event); // To prevent client blocking
			CloseHandle(xnet_response_event);
		}
	}

	return port;
}

#endif // SUPERCLIENT

