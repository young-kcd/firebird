/*
 *	PROGRAM:	JRD Remote Interface/Server
 *      MODULE:         xnet.c
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../remote/remote.h"
#include "../jrd/gds.h"
#include "../jrd/thd.h"
#include "../jrd/iberr.h"
#include "../remote/xnet.h"
#include "../utilities/install_nt.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../remote/xnet_proto.h"
#include "../remote/window.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"
#include "../remote/inet_proto.h"	/* for INET_alloc_port */
#include "../common/config/config.h"

#include <stdarg.h>

#ifdef WIN_NT
#include <windows.h>
#endif /* WIN_NT */

#if !(defined VMS || defined WIN_NT)
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

extern "C" {

#ifdef WIN_NT
#define ERRNO		GetLastError()
#endif

#ifdef UNIX
#define ERRNO		errno
#endif

#ifndef SYS_ERR
#define SYS_ERR		gds_arg_win32
#endif

#ifndef MAX_DATA
#define MAX_DATA	2048
#endif

#define MAX_SEQUENCE	256

/* see inet.c for platforms other that UNIX */
#ifdef UNIX
#define XNET_RETRY_CALL 5
#define SOCKET int
#define INVALID_SOCKET -1
#endif

extern int xdrmem_create();

static int accept_connection(PORT, P_CNCT *);
static PORT alloc_port(PORT, UCHAR *, USHORT, UCHAR *, ULONG);
static PORT aux_connect(PORT, PACKET *, XDR_INT(*)(void));
static PORT aux_request(PORT, PACKET *);
#ifdef UNIX
static ULONG connection_setup(TEXT *, PACKET *, STATUS *);
#endif
static void cleanup_comm(XCC);
static void cleanup_port(PORT);
static void disconnect(PORT);
static void exit_handler(PORT);
static XPM make_map(USHORT);
static int packet_receive(PORT, USHORT *);
static int packet_send(PORT, SSHORT);
static PORT receive(PORT, PACKET *);
static int send_full(PORT, PACKET *);
static int send_partial(PORT, PACKET *);
#ifdef SUPERCLIENT
#ifdef WIN_NT
static void server_shutdown(void);
static void server_watcher(void);
#endif
#endif
static int xdrxnet_create(XDR *, PORT, UCHAR *, USHORT, enum xdr_op);
static bool_t xdrxnet_endofrecord(XDR *, int);
#ifdef NOT_USED_OR_REPLACED
static void xnet_copy(SCHAR *, SCHAR *, int);
#endif
static int xnet_destroy(XDR *);
static int xnet_error(PORT, TEXT *, STATUS, int);
static void xnet_gen_error(PORT, STATUS, ...);
static bool_t xnet_getbytes(XDR *, SCHAR *, u_int);
static bool_t xnet_getlong(XDR *, SLONG *);
static u_int xnet_getpostn(XDR *);
static caddr_t xnet_inline(XDR *, u_int);
static bool_t xnet_putlong(XDR *, SLONG *);
static bool_t xnet_putbytes(XDR *, SCHAR *, u_int);
static bool_t xnet_read(XDR *);
static bool_t xnet_setpostn(XDR *, u_int);
static bool_t xnet_write(XDR *, int);

static xdr_t::xdr_ops xnet_ops = {
	xnet_getlong,
	xnet_putlong,
	xnet_getbytes,
	xnet_putbytes,
	xnet_getpostn,
	xnet_setpostn,
	xnet_inline,
	xnet_destroy
};

#ifndef MAX_PTYPE
#define MAX_PTYPE	ptype_out_of_band
#endif

static USHORT pages_per_user = XPS_DEF_PAGES_PER_CLI;
static USHORT users_per_map = XPS_DEF_NUM_CLI;
static USHORT num_maps = XPS_DEF_NUM_MAPS;
static XPM xpms = NULL;
static XPM first_xpm = NULL;
static XCC client_threads = NULL;
static XPM client_maps = NULL;
#ifdef WIN_NT
#ifdef SUPERCLIENT
static HANDLE server_process_handle = 0;
#endif
static HANDLE server_watcher_handle = 0;
#endif
static USHORT exit_flag = 0;
static bool initialized = false;
static MUTX_T xnet_mutex;


PORT XNET_analyze(
				  TEXT * file_name,
				  USHORT * file_length,
				  STATUS * status_vector,
				  TEXT * node_name, TEXT * user_string, USHORT uv_flag)
{
/**************************************
 *
 *      X N E T _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	Determine whether the file name has a "\\nodename".
 *	If so, establish an external connection to the node.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/
	RDB rdb;
	PORT port;
	PACKET *packet;
	P_CNCT *cnct;
	SSHORT user_length;
	TEXT *p, user_id[128], buffer[64];

	p_cnct::p_cnct_repeat * protocol;

	*file_length = strlen(file_name);

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	rdb = (RDB) ALLOC(type_rdb);
	packet = &rdb->rdb_packet;

/* Pick up some user identification information */

	user_id[0] = CNCT_user;
	p = user_id + 2;
	ISC_get_user(p, 0, 0, 0, 0, 0, 0);
	user_id[1] = strlen(p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	*p++ = CNCT_host;
	p++;
	ISC_get_host(p, (USHORT) (user_id + sizeof(user_id) - p));
	p[-1] = strlen(p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	if (uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}

	user_length = p - user_id;

/* Establish connection to server */

	cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = *file_length;
	cnct->p_cnct_file.cstr_address = (UCHAR *) file_name;

/* Note: prior to V3.1E a recievers could not in truth handle more
   then 5 protocol descriptions; however, the interprocess server 
   was created in 4.0 so this does not apply */

#ifdef SCROLLABLE_CURSORS
	cnct->p_cnct_count = 8;
#else
	cnct->p_cnct_count = 6;
#endif

	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

	protocol = cnct->p_cnct_versions;

	protocol->p_cnct_version = PROTOCOL_VERSION7;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 2;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION7;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 3;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 4;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION8;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 5;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION10;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 6;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_VERSION10;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 7;

#ifdef SCROLLABLE_CURSORS
	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 8;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 9;
#endif

/* If we can't talk to a server, punt.  Let somebody else generate
   an error. */

	if (!(port = XNET_connect(node_name, packet, status_vector, FALSE))) {
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
		cnct->p_cnct_file.cstr_length = *file_length;
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name;

		/* try again with next set of known protocols */

		cnct->p_cnct_count = 4;

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

		protocol = cnct->p_cnct_versions;

		protocol->p_cnct_version = PROTOCOL_VERSION4;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 2;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION4;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 3;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 4;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION6;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 5;

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
		cnct->p_cnct_file.cstr_length = *file_length;
		cnct->p_cnct_file.cstr_address = (UCHAR *) file_name;

		/* try again with next set of known protocols */

		cnct->p_cnct_count = 2;

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = (UCHAR *) user_id;

		protocol = cnct->p_cnct_versions;

		protocol->p_cnct_version = PROTOCOL_VERSION3;
		protocol->p_cnct_architecture = arch_generic;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 2;

		++protocol;

		protocol->p_cnct_version = PROTOCOL_VERSION3;
		protocol->p_cnct_architecture = ARCHITECTURE;
		protocol->p_cnct_min_type = ptype_rpc;
		protocol->p_cnct_max_type = ptype_batch_send;
		protocol->p_cnct_weight = 3;

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
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds_connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version 
   string to reflect it...  */

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


PORT XNET_connect(TEXT * name,
				  PACKET * packet, STATUS * status_vector, USHORT flag)
{
/**************************************
 *
 *      X N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Otherwise the
 *	connect is for a server process.
 *
 **************************************/
#ifdef SUPERCLIENT
	XCC xcc = (XCC) 0;
	XPM xpm;
	ULONG number;
	USHORT mapped_area, mapped_position;
	TEXT name_buffer[128];
#ifdef WIN_NT
	DWORD last_error;
#endif
	FILE_ID file_handle;
	CADDR_T mapped_address;
	PID_T client_pid;
#ifdef WIN_NT
	DWORD current_thread_id;
#endif
	UCHAR *start_ptr;
	USHORT espace;
	XPS xps;
	USHORT avail;
	PORT port;
#endif

	if (!initialized) {
		initialized = true;
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, NULL);
		THD_mutex_init(&xnet_mutex);
	}

/* set up for unavailable server */

	status_vector[0] = isc_arg_gds;
	status_vector[1] = isc_unavailable;
	status_vector[2] = isc_arg_end;

/* server can't do jumps */

#ifndef SUPERCLIENT

	return NULL;

#else /* SUPERCLIENT */
/* see if this thread has been connected yet, and if not, do so */

	THREAD_EXIT;
	THD_mutex_lock(&xnet_mutex);
	THREAD_ENTER;
#ifdef WIN_NT
	current_thread_id = GetCurrentThreadId();
#else
/* STUB : need to have a equivalent function, should be stored 
   and passed around , I donot think we need thread id */
#endif
/*
for (xcc = client_threads; xcc; xcc = xcc->xcc_next)
    if (xcc->xcc_thread_handle == current_thread_id &&
	!(xcc->xcc_flags & XCCF_SERVER_SHUTDOWN))
	break;
*/
	if (!xcc)
	{
		/* send a message to the server to get the next available
		   attachment, a 32 bit value with a mapped memory file
		   ID in the high order and an allocated area in that
		   mapped file in the low order */

#ifdef WIN_NT
		client_pid = GetCurrentProcessId();
		HWND hWndServer = FindWindow(szClassName, APP_NAME);
		number = (ULONG) SendMessage(hWndServer,
									 XPI_CONNECT_MESSAGE, 0,
									 (LPARAM) client_pid);
#else /* WIN_NT */
		client_pid = getpid();
		number = connection_setup(name, packet, *status_vector);
		/* STUB need to inform the server for connection and receive the
		   return value */
#endif
		if (!number || number == (ULONG) - 1) {
			THD_mutex_unlock(&xnet_mutex);
			return 0;
		}
		pages_per_user = (USHORT) XPS_UNPACK_PAGES(number);
		users_per_map = (USHORT) XPS_UNPACK_MAX_SLOTS(number);
		mapped_area = (SSHORT) XPS_UNPACK_MAPNUM(number);
		mapped_position = (SSHORT) XPS_UNPACK_SLOTNUM(number);

		/* see if area is already mapped for this client */

		for (xpm = client_maps; xpm; xpm = xpm->xpm_next)
			if (xpm->xpm_number == mapped_area &&
				!(xpm->xpm_flags & XPMF_SERVER_SHUTDOWN)) break;

		if (!xpm) {
			/* add new mapping */

			sprintf(name_buffer, XPI_MAPPED_FILE_NAME, XPI_PREFIX, mapped_area);
#ifdef WIN_NT
			file_handle = OpenFileMapping(FILE_MAP_WRITE, FALSE, name_buffer);
			if (!file_handle) {
				THD_mutex_unlock(&xnet_mutex);
				return 0;
			}
			mapped_address =
				MapViewOfFile(file_handle, FILE_MAP_WRITE, 0L, 0L,
							  XPS_MAPPED_SIZE(users_per_map, pages_per_user));
			if (!mapped_address) {
				last_error = ERRNO;
				CloseHandle(file_handle);
				THD_mutex_unlock(&xnet_mutex);
				return 0;
			}
			xpm = (XPM) ALLR_alloc(sizeof(struct xpm));
			if (!xpm) {
				UnmapViewOfFile(mapped_address);
				CloseHandle(file_handle);
				THD_mutex_unlock(&xnet_mutex);
				return 0;
			}
#else /* WIN_NT */
			/* STUB : need to have mapping for the SOlaris client/server here */
#endif
			xpm->xpm_next = client_maps;
			client_maps = xpm;
			xpm->xpm_count = 0;
			xpm->xpm_number = mapped_area;
			xpm->xpm_handle = file_handle;
			xpm->xpm_address = mapped_address;
			xpm->xpm_flags = 0;
		}

		/* there's no thread structure, so make one  */

		xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
		if (!xcc) {
#ifdef WIN_NT
			UnmapViewOfFile(mapped_address);
			CloseHandle(file_handle);
#else
			/* STUB : unmap shared memory here */
#endif
			THD_mutex_unlock(&xnet_mutex);
			return 0;
		}
		xcc->xcc_file_handle = xpm->xpm_handle;
		xcc->xcc_mapped_addr = (UCHAR *) xpm->xpm_address +
			XPS_MAPPED_FOR_CLI(pages_per_user, mapped_position);
		xcc->xcc_file = mapped_area;
		xcc->xcc_slot = mapped_position;
		xcc->xcc_xpm = xpm;
		xcc->xcc_flags = 0;
		xcc->xcc_send_channel_locked = FALSE;
		xps = (XPS) xcc->xcc_mapped_addr;

		/* only speak if server has correct protocol */

		if (xps->xps_server_protocol != 2L) {
#ifdef WIN_NT
			UnmapViewOfFile(mapped_address);
			CloseHandle(file_handle);
#else
			/* STUB : unmap shared memory file here */
#endif
			THD_mutex_unlock(&xnet_mutex);
			return 0;
		}
		xps->xps_client_protocol = 2L;
#ifdef WIN_NT
		xps->xps_server_proc =
			OpenProcess(SYNCHRONIZE, 0, xps->xps_server_id);
		xcc->xcc_server_proc = xps->xps_server_proc;
#else
		/* STUB : we need to use use some other means to figure out hte server has
		   died */
#endif
		xcc->xcc_server_id = xps->xps_server_id;
		xpm->xpm_count++;

		/* get handles of semaphores */

		sprintf(name_buffer, XPI_S_TO_C_SEM_NAME, XPI_PREFIX,
				mapped_area, mapped_position);
#ifdef WIN_NT
		xcc->xcc_recv_sem =
			OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, name_buffer);
#else
		/* STUB : need to call event init for Unix */
#endif
		sprintf(name_buffer, XPI_C_TO_S_SEM_NAME, XPI_PREFIX, mapped_area,
				mapped_position);
#ifdef WIN_NT
		xcc->xcc_send_sem =
			OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, name_buffer);
#else
		/* STUB : need to call event init for Unix */
#endif

		/* added this here from the server side as this part is called by the client 
		   and the server address need not be valid for the client -smistry 10/29/98 */

		/* we also need to add client side flags or channel pointer as they 
		   differ from the server side */

		if (pages_per_user < 4)
			espace = 50;
		else
			espace = 100;

		avail =
			(USHORT) (XPS_USEFUL_SPACE(pages_per_user) - (espace * 2)) / 2;

		start_ptr = (UCHAR *) xps + (sizeof(struct xps) + (2 * espace));

		/* send channel */
		xps->xps_channels[0].xch_client_ptr = start_ptr;
		/* receive channel */
		xps->xps_channels[1].xch_client_ptr = (start_ptr + avail);

		xcc->xcc_receive_channel = &xps->xps_channels[1];
		xcc->xcc_send_channel = &xps->xps_channels[0];

		/* if we're not watching the server yet, do so now */

#ifdef WIN_NT
		if (!server_watcher_handle) {
			server_process_handle = xcc->xcc_server_proc;
			gds__thread_start(reinterpret_cast<FPTR_INT_VOID_PTR>
							  (server_watcher), NULL,
							  THREAD_high, 0, &server_watcher_handle);
		}
#else
		/* STUB : we need just the server id, we donot have this thread */
#endif

		/* link to xcc chain */

		xcc->xcc_next = client_threads;
		client_threads = xcc;
	}

	THD_mutex_unlock(&xnet_mutex);
	port = alloc_port(0, xcc->xcc_send_channel->xch_client_ptr,
					  xcc->xcc_send_channel->xch_size,
					  xcc->xcc_receive_channel->xch_client_ptr,
					  xcc->xcc_receive_channel->xch_size);
	port->port_xcc = (void *) xcc;
	port->port_status_vector = status_vector;
	send_full(port, packet);
	return port;
#endif /* SUPERCLIENT */
}


#if defined(SUPERSERVER) && defined(WIN_NT)
extern "C" {
	static void atexit_close_handles()
	{
		for (XPM pXpm = first_xpm; pXpm; pXpm = pXpm->xpm_next) {
			if (pXpm->xpm_address) {
				UnmapViewOfFile(pXpm->xpm_address);
				pXpm->xpm_address = 0;
			}
			if (pXpm->xpm_handle) {
				CloseHandle(pXpm->xpm_handle);
				pXpm->xpm_handle = 0;
			}
		}
	}
}
#endif	// SUPERSERVER && WIN_NT


USHORT XNET_init(HWND hwnd,
				 USHORT usrs_pr_mp, USHORT pgs_pr_usr, USHORT mx_mps)
{
/**************************************
 *
 *      X N E T _ i n i t
 *
 **************************************
 *
 * Functional description
 *      Init mapped file stuff.   Returns 1 if successful,
 *      -1 if the map already exists, and 0 otherwise.
 *
 **************************************/
	XPM xpm;

	if (!initialized) {
		initialized = true;
		gds__register_cleanup((FPTR_VOID_PTR) exit_handler, NULL);
		THD_mutex_init(&xnet_mutex);
	}

/* init the limits */

	if (usrs_pr_mp &&
		usrs_pr_mp >= XPS_MIN_NUM_CLI &&
		usrs_pr_mp <= XPS_MIN_NUM_CLI)
	{
		users_per_map = usrs_pr_mp;
	}

	if (pgs_pr_usr &&
		pgs_pr_usr >= XPS_MIN_PAGES_PER_CLI &&
		pgs_pr_usr <= XPS_MAX_PAGES_PER_CLI)
	{
		pages_per_user = pgs_pr_usr;
	}

	if (mx_mps && mx_mps >= XPS_MIN_NUM_MAPS && mx_mps <= XPS_MAX_NUM_MAPS) {
		num_maps = mx_mps;
	}

/* init mapped chain and critical regions */

	xpms = NULL;

/* create the first map (failure means it's already there )*/

	xpm = make_map(0);
	if (xpm && (xpm != (XPM) -1)) {
		first_xpm = xpm;
#if defined(SUPERSERVER) && defined(WIN_NT)
		// TMN: 2003-03-11: Close the handles at server shutdown
		// Possibly this is also needed for CS, but since I can't test that
		// I decided to only do it for SS.
		atexit(&atexit_close_handles);
#endif
		return (USHORT) 1;
	}

	return (USHORT)(ULONG) xpm;
}


PORT XNET_start_thread(ULONG client_pid, ULONG * response)
{
/**************************************
 *
 *      X N E T _ s t a r t _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Start an interprocess thread.   This allocates
 *	the next available chunk of the mapped file and
 *	tells the client where it is.
 *
 **************************************/
	XCC xcc;
	PORT port;
	USHORT mapped_area, mapped_position;
	TEXT name_buffer[128];
	XPM xpm;
	USHORT i, j;
	UCHAR *p;
	XPS xps;
	USHORT avail;
	USHORT espace;

/* go through list of maps */

	THD_mutex_lock(&xnet_mutex);
	j = 0;
	for (xpm = xpms; xpm; xpm = xpm->xpm_next) {
		/* find an available unused comm area */

		for (i = 0; i < users_per_map; i++)
			if (!xpm->xpm_ids[i])
				break;

		if (i < users_per_map) {
			xpm->xpm_count++;
			xpm->xpm_ids[i]++;
			j = xpm->xpm_number;
			break;
		}
		j++;
	}

/* if the mapped file structure has not yet been initialized,
   make one now */

	if (!xpm) {
		/* allocate new map file and first slot */

		xpm = make_map(j);

		/* check for errors in creation of mapped file */

		if (!xpm || xpm == (XPM) - 1) {
			THD_mutex_unlock(&xnet_mutex);
			return (PORT) - 1;
		}
		i = 0;
		xpm->xpm_ids[0]++;
		xpm->xpm_count++;
	}

	mapped_area = j;
	mapped_position = i;

/* allocate a communications control structure and fill it in */

	xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
	if (!xcc) {
		THD_mutex_unlock(&xnet_mutex);
		return (PORT) - 1;
	}
	p = (UCHAR *) xpm->xpm_address;
	p += XPS_MAPPED_FOR_CLI(pages_per_user, mapped_position);
	memset(p, (char) 0, XPS_MAPPED_PER_CLI(pages_per_user));
	xcc->xcc_next = NULL;
	xcc->xcc_mapped_addr = p;
	xcc->xcc_xpm = xpm;
	xcc->xcc_slot = mapped_position;
	xcc->xcc_flags = 0;
	xcc->xcc_client_id = (PID_T) client_pid;
#ifdef WIN_NT
	xcc->xcc_client_proc = OpenProcess(SYNCHRONIZE, 0, xcc->xcc_client_id);
#else
/* STUB : need something for Solaris ?? I donot think so */
#endif
	xcc->xcc_file = mapped_area;
	xcc->xcc_slot = mapped_position;
	xcc->xcc_send_channel_locked = FALSE;
	xps = (XPS) xcc->xcc_mapped_addr;
	xps->xps_client_id = xcc->xcc_client_id;
#ifdef WIN_NT
	xps->xps_client_proc = xcc->xcc_client_proc;
	xps->xps_server_id = GetCurrentProcessId();
#else
/* STUB : need something for Solaris ?? I donot think so */
	xps->xps_server_id = getpid();
#endif

/* make sure client knows what this server speaks */

	xps->xps_server_protocol = 2L;
	xps->xps_client_protocol = 0L;

#ifdef WIN_NT
/* create the semaphores and put the handles into the xcc */

	sprintf(name_buffer, XPI_S_TO_C_SEM_NAME, XPI_PREFIX, mapped_area, mapped_position);
	xcc->xcc_send_sem = CreateSemaphore(ISC_get_security_desc(),
										0L, 2L, name_buffer);
	sprintf(name_buffer, XPI_C_TO_S_SEM_NAME, XPI_PREFIX, mapped_area, mapped_position);
	xcc->xcc_recv_sem = CreateSemaphore(ISC_get_security_desc(),
										0L, 2L, name_buffer);

#else
/* STUB : add create events here */
#endif
	xcc->xcc_receive_channel = &xps->xps_channels[0];
	xcc->xcc_send_channel = &xps->xps_channels[1];

#ifdef WIN_NT
/* make sure semaphores are "empty" */

	while (WaitForSingleObject(xcc->xcc_send_sem, 1) == WAIT_OBJECT_0);
	while (WaitForSingleObject(xcc->xcc_recv_sem, 1) == WAIT_OBJECT_0);
#else
/* STUB this should be event clear */
#endif

/* set up the channel structures */

	if (pages_per_user < 4)
		espace = 50;
	else
		espace = 100;

	p += sizeof(struct xps);
	avail = (USHORT) XPS_USEFUL_SPACE(pages_per_user);
	xps->xps_channels[2].xch_buffer = p;	/* client to server events */
	xps->xps_channels[2].xch_size = espace;
	p += espace;
	xps->xps_channels[3].xch_buffer = p;	/* server to client events */
	xps->xps_channels[3].xch_size = espace;
	p += espace;
	avail = (avail - (espace * 2)) / 2;	/* split remainer in half */
	xps->xps_channels[0].xch_buffer = p;	/* client to server data */
	xps->xps_channels[0].xch_size = avail;
	p += avail;
	xps->xps_channels[1].xch_buffer = p;	/* server to client data */
	xps->xps_channels[1].xch_size = avail;

/* finally, allocate and set the port structure for this client */

	port = alloc_port(0, xcc->xcc_send_channel->xch_buffer,
					  xcc->xcc_send_channel->xch_size,
					  xcc->xcc_receive_channel->xch_buffer,
					  xcc->xcc_receive_channel->xch_size);
	if (port)
		port->port_xcc = (void *) xcc;

/* return combined mapped area and number */

	THD_mutex_unlock(&xnet_mutex);
	*response =
		XPS_PACK_PARAMS(users_per_map, pages_per_user, mapped_area,
						mapped_position);
	return port;
}


static int accept_connection( PORT port, P_CNCT * cnct)
{
/**************************************
 *
 *	a c c e p t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Accept an incoming request for connection.  This is purely a lower
 *	level handshaking function, and does not constitute the server
 *	response for protocol selection.
 *
 **************************************/
	return TRUE;
}


static PORT alloc_port(
					   PORT parent,
					   UCHAR * send_buffer,
					   USHORT send_length,
					   UCHAR * receive_buffer, ULONG receive_length)
{
/**************************************
 *
 *	a l l o c _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Allocate a port block, link it in to parent (if there is a parent),
 *	and initialize input and output XDR streams.
 *
 **************************************/
	PORT port;
	TEXT buffer[64];


	port = (PORT) ALLOCV(type_port, 0);
/*
port->port_type = port_ipserver;
*/
	port->port_type = port_xnet;
	port->port_state = state_pending;
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
	port->port_connect =
		reinterpret_cast<PORT(*)(PORT, PACKET *, void (*)())>(aux_connect);
	port->port_request = aux_request;
	port->port_buff_size = send_length;
	xdrxnet_create(&port->port_send, port, send_buffer, send_length,
				   XDR_ENCODE);
	xdrxnet_create(&port->port_receive, port, receive_buffer, 0, XDR_DECODE);
	return port;
}


static PORT aux_connect( PORT port, PACKET * packet, XDR_INT(*ast) (void))
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Try to establish an alternative connection.
 *      Somebody has already done a successfull connect request.
 *      This uses the existing xcc for the parent port to more
 *      or less duplicate a new xcc for the new aux port pointing
 *      to the event stuff in the map.
 *
 **************************************/
	PORT new_port;
	XCC oxcc;
	XCC xcc;
	TEXT name_buffer[128];
	XPS xps;
	XPM xpm;

	USHORT espace;

	if (port->port_server_flags) {
		port->port_flags |= PORT_async;
		return port;
	}

/* make a new xcc */

	oxcc = (XCC) port->port_xcc;
	xps = (XPS) oxcc->xcc_mapped_addr;
	xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
	if (!xcc)
		return NULL;
	xpm = (XPM) xcc->xcc_xpm = oxcc->xcc_xpm;
	xcc->xcc_file = oxcc->xcc_file;
	xcc->xcc_slot = oxcc->xcc_slot;
#ifdef WIN_NT
	xcc->xcc_server_proc = oxcc->xcc_server_proc;
	xcc->xcc_client_proc = oxcc->xcc_client_proc;
#endif
	xcc->xcc_server_id = oxcc->xcc_server_id;
	xcc->xcc_client_id = oxcc->xcc_client_id;
	xcc->xcc_flags = 0;
	xcc->xcc_file_handle = oxcc->xcc_file_handle;
	xcc->xcc_mapped_addr = oxcc->xcc_mapped_addr;
	xcc->xcc_xpm->xpm_count++;

#ifdef WIN_NT
/* get handles of semaphores */

	sprintf(name_buffer, XPI_S_TO_C_EVT_SEM_NAME,XPI_PREFIX, 
			xcc->xcc_file, xcc->xcc_slot);
	xcc->xcc_recv_sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
									  name_buffer);
	sprintf(name_buffer, XPI_C_TO_S_EVT_SEM_NAME, XPI_PREFIX, 
			xcc->xcc_file, xcc->xcc_slot);
	xcc->xcc_send_sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
									  name_buffer);
#else
/* STUB : This should be event_init() calls ? */
#endif
/* the address mapping is not the same in the client and the server hence set 
   the client event channels correctly  correctly  - smistry 10/29/98*/
	if (pages_per_user < 4)
		espace = 50;
	else
		espace = 100;

/* receive events channel */
	xps->xps_channels[3].xch_client_ptr = ((UCHAR *) xpm->xpm_address +
										   sizeof(struct xps) + (espace));
/* send events channel */
	xps->xps_channels[2].xch_client_ptr = ((UCHAR *) xpm->xpm_address +
										   sizeof(struct xps));
	xcc->xcc_receive_channel = &xps->xps_channels[3];
	xcc->xcc_send_channel = &xps->xps_channels[2];

/* link to xcc chain */

	THREAD_EXIT;
	THD_mutex_lock(&xnet_mutex);
	THREAD_ENTER;
	xcc->xcc_next = client_threads;
	client_threads = xcc;
	THD_mutex_unlock(&xnet_mutex);

/* alloc new port and link xcc to it */

	new_port = alloc_port(0, xcc->xcc_send_channel->xch_client_ptr,
						  xcc->xcc_send_channel->xch_size,
						  xcc->xcc_receive_channel->xch_client_ptr,
						  xcc->xcc_receive_channel->xch_size);
	port->port_async = new_port;
	new_port->port_flags = port->port_flags & PORT_no_oob;
	new_port->port_xcc = (void *) xcc;
	return new_port;
}


static PORT aux_request( PORT port, PACKET * packet)
{
/**************************************
 *
 *	a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      A remote interface has requested the server to
 *      prepare an auxiliary connection.   This is done
 *      by allocating a new port and comm (xcc) structure,
 *      using the event stuff in the map rather than the
 *      normal database channels.
 *
 **************************************/
	PORT new_port;
	XCC oxcc;
	XCC xcc;
	XPS xps;
	TEXT name_buffer[128];


/* allocate a communications control structure and fill it in */

	oxcc = (XCC) port->port_xcc;
	xps = (XPS) oxcc->xcc_mapped_addr;
	xcc = (XCC) ALLR_alloc(sizeof(struct xcc));
	if (!xcc)
		return NULL;
	xcc->xcc_next = NULL;
	xcc->xcc_xpm = oxcc->xcc_xpm;
	xcc->xcc_file = oxcc->xcc_file;
	xcc->xcc_slot = oxcc->xcc_slot;
#ifdef WIN_NT
	xcc->xcc_server_proc = oxcc->xcc_server_proc;
	xcc->xcc_client_proc = oxcc->xcc_client_proc;
#endif
	xcc->xcc_server_id = oxcc->xcc_server_id;
	xcc->xcc_client_id = oxcc->xcc_client_id;
	xcc->xcc_flags = 0;
	xcc->xcc_mapped_addr = oxcc->xcc_mapped_addr;
	xcc->xcc_file_handle = oxcc->xcc_file_handle;
	xcc->xcc_xpm->xpm_count++;
	xps = (XPS) xcc->xcc_mapped_addr;

/* create the event semaphores and put the handles into the xcc */

#ifdef WIN_NT
	sprintf(name_buffer, XPI_S_TO_C_EVT_SEM_NAME, XPI_PREFIX, 
			xcc->xcc_file, xcc->xcc_slot);
	xcc->xcc_send_sem = CreateSemaphore(ISC_get_security_desc(),
										0L, 2L, name_buffer);
	sprintf(name_buffer, XPI_C_TO_S_EVT_SEM_NAME, XPI_PREFIX, 
			xcc->xcc_file, xcc->xcc_slot);
	xcc->xcc_recv_sem = CreateSemaphore(ISC_get_security_desc(),
										0L, 2L, name_buffer);
#else
/* STUB : Should be Event_init() calls. */
#endif
	xcc->xcc_receive_channel = &xps->xps_channels[2];
	xcc->xcc_send_channel = &xps->xps_channels[3];

#ifdef WIN_NT
/* make sure semaphores are "empty" */

	while (WaitForSingleObject(xcc->xcc_send_sem, 1) == WAIT_OBJECT_0);
	while (WaitForSingleObject(xcc->xcc_recv_sem, 1) == WAIT_OBJECT_0);
#else
/* STUB : should be event clear */
#endif

/* get a new port structure and link it to the one passed in */

	new_port = alloc_port(0, xcc->xcc_send_channel->xch_buffer,
						  xcc->xcc_send_channel->xch_size,
						  xcc->xcc_receive_channel->xch_buffer,
						  xcc->xcc_receive_channel->xch_size);
	port->port_async = new_port;
	new_port->port_server_flags = port->port_server_flags;
	new_port->port_flags = port->port_flags & PORT_no_oob;
	new_port->port_xcc = (void *) xcc;
	return new_port;
}


#ifdef UNIX
ULONG connection_setup(TEXT * name, PACKET * packet, STATUS * status_vector)
 {
/**************************************
 *
 *      c o n n e c t i o n _ s e t u p
 *
 **************************************
 *
 * Functional description
 *      Setup the socket and send request to the other server for client.
 *
 **************************************/
	struct sockaddr_in address;
	PORT port;
	TEXT *protocol, temp[128], *p;
	struct hostent *host;
	struct servent *service;
	TEXT msg[128];
	int sock_id;
	SOCKET sock;


	port = INET_alloc_port(0);
	port->port_status_vector = status_vector;
/* this just for connection set all the timeouts to 0 */
	port->port_dummy_packet_interval = port->port_connect_timeout = 0;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;
	protocol = Config::getRemoteServiceName();

/* Set up Inter-Net socket address */

	memset((SCHAR *) & address, 0, sizeof(address));

/* U N I X style sockets */
	THREAD_EXIT;
	host = gethostbyname(name);

/* On Windows NT/9x, gethostbyname can only accomodate
 * 1 call at a time.  In this case it returns the error
 * WSAEINPROGRESS. On UNIX systems, this call may not succeed
 * because of a temporary error.  In this case, it returns
 * h_error set to TRY_AGAIN.  When these errors occur,
 * retry the operation a few times.
 * NOTE: This still does not guarantee success, but helps.
 */
	if (!host) {
		int retry;
		if (h_errno == TRY_AGAIN) {
			for (retry = 0; retry < XNET_RETRY_CALL; retry++) {
				host = gethostbyname(name);
				if (host)
					break;
			}
		}
	}
	THREAD_ENTER;
	if (!host) {
		sprintf(msg,
				"XNET/XNET_connection_setup: gethostbyname failed, error code = %d",
				h_errno);
		gds__log(msg, 0);
		xnet_gen_error(port,
					   isc_network_error,
					   isc_arg_string,
					   port->port_connection->str_data,
					   isc_arg_gds,
					   isc_net_lookup_err, isc_arg_gds, isc_host_unknown, 0);
		return NULL;
	}

	address.sin_family = host->h_addrtype;
	if (packet)
		memcpy(&address.sin_addr, host->h_addr, sizeof(address.sin_addr));
	else
		address.sin_addr.s_addr = INADDR_ANY;

	THREAD_EXIT;
	service = getservbyname(protocol, "tcp");
	THREAD_ENTER;
	if (!service) {
		sprintf(msg,
				"INET/INET_connect: getservbyname failed, error code = %d",
				h_errno);
		gds__log(msg, 0);
		xnet_gen_error(port,
					   isc_network_error,
					   isc_arg_string,
					   port->port_connection->str_data,
					   isc_arg_gds,
					   isc_net_lookup_err,
					   isc_arg_gds,
					   isc_service_unknown,
					   isc_arg_string, protocol, isc_arg_string, "tcp", 0);
		return NULL;
	}
	address.sin_port = service->s_port;

/* Allocate a port block and initialize a socket for communications */

	port->port_handle = (HANDLE) socket(AF_INET, SOCK_STREAM, 0);

	if ((SOCKET) port->port_handle == INVALID_SOCKET) {
		xnet_error(port, "socket", isc_net_connect_err, ERRNO);
		return NULL;
	}

/* If we're a host, just make the connection */

	if (packet) {
		THREAD_EXIT;
		sock_id = connect((SOCKET) port->port_handle,
						  (struct sockaddr *) &address, sizeof(address));
		THREAD_ENTER;
		if (sock_id != -1 && send_full(port, packet))
			return port;
		else {
			xnet_error(port, "connect", isc_net_connect_err, ERRNO);
			disconnect(port);
			return NULL;
		}
	}

	sock_id = bind((SOCKET) port->port_handle,
				   (struct sockaddr *) &address, sizeof(address));

	if (sock_id == -1) {
		xnet_error(port, "bind", isc_net_connect_listen_err, ERRNO);
		disconnect(port);
		return NULL;
	}

	sock_id = listen((SOCKET) port->port_handle, 5);

	if (sock_id == -1) {
		xnet_error(port, "listen", isc_net_connect_listen_err, ERRNO);
		return NULL;
	}

	if (flag & SRVR_multi_client) {
		/* Prevent the generation of dummy keepalive packets on the
		   connect port. */

		port->port_dummy_packet_interval = 0;
		port->port_dummy_timeout = 0;
		port->port_server_flags |= (SRVR_server | SRVR_multi_client);
		gds__register_cleanup(exit_handler, (void *) port);
		return port;
	}

	while (TRUE) {
		int l;
		THREAD_EXIT;
		l = sizeof(address);
		sock = accept((SOCKET) port->port_handle,
					  (struct sockaddr *) &address, &l);
		if (sock == INVALID_SOCKET) {
			THREAD_ENTER;
			xnet_error(port, "accept", isc_net_connect_err, ERRNO);
			disconnect(port);
			return NULL;
		}
		if (!fork()) {
			THREAD_ENTER;
			close((SOCKET) port->port_handle);
			port->port_handle = (HANDLE) sock;
			port->port_server_flags |= SRVR_server;
			return port;
		}
		THREAD_ENTER;
		close(sock);
	}
}
#endif /* UNIX */


static void cleanup_comm( XCC xcc)
{
/**************************************
 *
 *      c l e a n u p _ c o m m
 *
 **************************************
 *
 * Functional description
 *      Clean up an xcc structure, close its handles,
 *      unmap its file, and free it.
 *
 **************************************/
	XCC pxcc;
	XPM xpm;
#ifdef SUPERCLIENT
	XPM pxpm;
#endif /* SUPERCLIENT */

#ifdef SUPERSERVER
	SSHORT slot_number = (SSHORT) xcc->xcc_slot;
#endif /* SUPERSERVER */

/* free up semaphores */

	xpm = xcc->xcc_xpm;
#ifdef WIN_NT
	if (xcc->xcc_send_sem)
		CloseHandle(xcc->xcc_send_sem);
	if (xcc->xcc_recv_sem)
		CloseHandle(xcc->xcc_recv_sem);
#ifdef SUPERCLIENT
	if (xcc->xcc_server_proc)
		CloseHandle(xcc->xcc_server_proc);
#endif /* SUPERCLIENT */
#else /* WIN_NT */
/* STUB : this should be event_fini */
#endif /* WIN_NT */

/* find xcc in chain and release */

	THD_mutex_lock(&xnet_mutex);
	if (client_threads == xcc)
		client_threads = xcc->xcc_next;
	else {
		for (pxcc = client_threads; pxcc && pxcc->xcc_next;
			 pxcc = pxcc->xcc_next) if (pxcc->xcc_next == xcc) {
				pxcc->xcc_next = xcc->xcc_next;
				break;
			}
	}

	THD_mutex_unlock(&xnet_mutex);
	ALLR_free((UCHAR *) xcc);

/* if this was the last area for this map, unmap it */

	xpm->xpm_count--;
#ifdef SUPERCLIENT
	if (!xpm->xpm_count && client_maps) {
#ifdef WIN_NT
		UnmapViewOfFile(xpm->xpm_address);
		CloseHandle(xpm->xpm_handle);
#else
		/* STUB : unmap the shared memory segment. */
#endif

		/* find in chain and release */

		THD_mutex_lock(&xnet_mutex);
		if (client_maps == xpm)
			client_maps = xpm->xpm_next;
		else {
			for (pxpm = client_maps; pxpm->xpm_next; pxpm = pxpm->xpm_next)
				if (pxpm->xpm_next == xpm) {
					pxpm->xpm_next = xpm->xpm_next;
					break;
				}
		}
		THD_mutex_unlock(&xnet_mutex);
		ALLR_free((UCHAR *) xpm);
	}
#endif /* SUPERCLIENT */

#ifdef SUPERSERVER
/* free the slot for use by other clients */
	xpm->xpm_ids[slot_number] = 0;
#endif
	return;
}


static void cleanup_port( PORT port)
{
/**************************************
 *
 *      c l e a n u p _ p o r t
 *
 **************************************
 *
 * Functional description
 *      Walk through the port structure freeing
 *      allocated memory and then free the port.
 *
 **************************************/

	if (port->port_xcc)
		cleanup_comm((XCC) port->port_xcc);

	if (port->port_version)
		ALLR_free((UCHAR *) port->port_version);

	if (port->port_connection)
		ALLR_free((UCHAR *) port->port_connection);

	if (port->port_user_name)
		ALLR_free((UCHAR *) port->port_user_name);

	if (port->port_host)
		ALLR_free((UCHAR *) port->port_host);

	if (port->port_object_vector)
		ALLR_free((UCHAR *) port->port_object_vector);

#ifdef DEBUG_XDR_MEMORY
	if (port->port_packet_vector)
		ALLR_free((UCHAR *) port->port_packet_vector);
#endif

	ALLR_release((BLK) port);
	return;
}


static void disconnect( PORT port)
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
	PORT parent, *ptr;

/* If this is a sub-port, unlink it from it's parent */

	if ((parent = port->port_parent) != NULL) {
		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}
		for (ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
			if (*ptr == port) {
				*ptr = port->port_next;
				if (ptr == &parent->port_clients)
					parent->port_next = *ptr;
				break;
			}
	}
	else if (port->port_async) {
		/* If we're MULTI_THREAD then we cannot free the port because another
		 * thread might be using it.  If we're SUPERSERVER we must free the
		 * port to avoid a memory leak.  What we really need to know is if we
		 * have multi-threaded events, but this is transport specific.
		 * -smistry 10/29/98 */
#if     (defined (MULTI_THREAD) && !defined (SUPERSERVER))
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

	cleanup_port(port);
}


static void exit_handler( PORT main_port)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Shutdown all active connections
 *	to allow restart.
 *
 **************************************/
	PORT port;

	for (port = main_port; port; port = port->port_next);
}


static XPM make_map( USHORT map_number)
{
/**************************************
 *
 *      m a k e _ m a p
 *
 **************************************
 *
 * Functional description
 *      Create a mapped file.   This
 *	returns -
 *	    0 - mapped file already exists
 *	   -1 - mapping  or creation error
 *	other - pointer to new structure
 *
 **************************************/
	XPM xpm;
	FILE_ID map_handle;
	CADDR_T map_address;
	USHORT i;
	TEXT name_buffer[128];

	// create the mapped file name and try to open it
	sprintf(name_buffer, XPI_MAPPED_FILE_NAME, XPI_PREFIX, map_number);

#ifdef WIN_NT
	map_handle = CreateFileMapping((HANDLE) 0xFFFFFFFF, NULL,
								   PAGE_READWRITE, 0L,
								   XPS_MAPPED_SIZE(users_per_map,
												   pages_per_user),
								   name_buffer);
	if (map_handle && ERRNO == ERROR_ALREADY_EXISTS) {
		return NULL;
	}
	if (!map_handle) {
		return (XPM) - 1;
	}

	map_address = MapViewOfFile(map_handle, FILE_MAP_WRITE, 0L, 0L,
								XPS_MAPPED_SIZE(users_per_map,
												pages_per_user));
	if (!map_address) {
		CloseHandle(map_handle);
	}

#else
/* STUB : need to do same thing for solaris */
	map_address = 0;
#endif

	if (!map_address) {
		return (XPM) - 1;
	}

/* allocate a structure and initialize it */

	xpm = (XPM) ALLR_alloc(sizeof(struct xpm));
	if (!xpm) {
		return (XPM) - 1;
	}
	xpm->xpm_handle = map_handle;
	xpm->xpm_address = map_address;
	xpm->xpm_number = map_number;
	xpm->xpm_count = 0;
	for (i = 0; i < users_per_map; i++) {
		xpm->xpm_ids[i] = 0;
	}
	xpm->xpm_next = xpms;
	xpm->xpm_flags = 0;
	xpms = xpm;
	return xpm;
}


static int packet_receive( PORT port, USHORT * length)
{
/**************************************
 *
 *	p a c k e t _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *      Receive a packet, unpacking it from the map as
 *      often as necessary until it's all in.
 *
 **************************************/
	USHORT errres;
	XCC xcc;
	XPS xps;
	XCH xch;
	USHORT n;

	xcc = reinterpret_cast<XCC>(port->port_xcc);
	xps = (XPS) (xcc->xcc_mapped_addr);
	xch = xcc->xcc_receive_channel;
	THREAD_EXIT;

/* first, wait for the other side to signal that
   the map is full */

	xch->xch_flags &= ~(XCHFLAG_data_overrun | XCHFLAG_more_data);
	errres = FALSE;
	n = 0;
/* signal the other side that the communications area in mapped memory is emptied */
#ifdef WIN_NT
	if (!ReleaseSemaphore(xcc->xcc_send_sem, 1L, NULL))
#else
/* STUB will be event_post */
	if (0)
#endif
		errres = TRUE;
	else {
#ifdef WIN_NT
		if (WaitForSingleObject(xcc->xcc_recv_sem, INFINITE) == WAIT_FAILED)
			errres = TRUE;
#else
/* STUB will be event_wait */
		if (0) {
		}
#endif
		else {
			n = xch->xch_length;
		}
	}
	THREAD_ENTER;
	if (errres)
		return xnet_error(port, "read from map failed",
						  isc_net_read_err, ERRNO);
	if (!n)
		return xnet_error(port, "read from map end-of-file",
						  isc_net_read_err, ERRNO);
	*length = n;
	return TRUE;
}


static int packet_send( PORT port, SSHORT length)
{
/**************************************
 *
 *	p a c k e t _ s e n d
 *
 **************************************
 *
 * Functional description:
 *      Pack data into map and send it, repeating
 *      as often as necessary until it's all sent.
 *
 **************************************/
	XCC xcc;
	XPS xps;
	XCH xch;
	USHORT errres;

	xcc = reinterpret_cast<XCC>(port->port_xcc);
	xps = (XPS) (xcc->xcc_mapped_addr);
	xch = xcc->xcc_send_channel;

/* first, signal the other side that the communications
   area in mapped memory is filed and ready */

	xch->xch_flags &= ~(XCHFLAG_data_overrun | XCHFLAG_more_data);
	xch->xch_length = length;
	errres = FALSE;
/* release the send channel lock, if we need to send more data we should wait
 * for the receiver to complete emptying the comm. channel area. */
	xcc->xcc_send_channel_locked = FALSE;
#ifdef WIN_NT
	if (!ReleaseSemaphore(xcc->xcc_send_sem, 1L, NULL))
#else
/* STUB add ISC_event_post here */
	if (1)
#endif
		errres = TRUE;
	if (errres)
		return xnet_error(port, "write to map error",
						  isc_net_write_err, ERRNO);

	port->port_flags &= ~PORT_pend_ack;
	return TRUE;
}


static PORT receive( PORT main_port, PACKET * packet)
{
/**************************************
 *
 *	r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a message from a port or clients of a port.  If the process
 *	is a server and a connection request comes in, generate a new port
 *	block for the client.
 *
 **************************************/

	if (!xdr_protocol(&main_port->port_receive, packet))
		packet->p_operation = op_exit;
	return main_port;
}


static int send_full( PORT port, PACKET * packet)
{
/**************************************
 *
 *	s e n d _ f u l l
 *
 **************************************
 *
 * Functional description
 *	Send a packet across a port to another process.
 *
 **************************************/

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;
	return xdrxnet_endofrecord(&port->port_send, TRUE);
}


static int send_partial( PORT port, PACKET * packet)
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
static void server_shutdown(void)
{
/**************************************
 *
 *      s e r v e r _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *      Server shutdown detected, so mark
 *      everything and release handles.
 *
 **************************************/
	XCC xcc;
	XPM xpm;


/* for each xcc structure, shut it and its database */

	for (xcc = client_threads; xcc; xcc = xcc->xcc_next) {
		xcc->xcc_flags |= XCCF_SERVER_SHUTDOWN;

		/* free up semaphores */

#ifdef WIN_NT
		if (xcc->xcc_send_sem) {
			CloseHandle(xcc->xcc_send_sem);
		}
		if (xcc->xcc_recv_sem) {
			CloseHandle(xcc->xcc_recv_sem);
		}
		if (xcc->xcc_server_proc) {
			CloseHandle(xcc->xcc_server_proc);
		}
		xcc->xcc_send_sem = 0;
		xcc->xcc_recv_sem = 0;
		xcc->xcc_server_proc = 0;
#else
/* STUB : Similar event_fini() etc for Solaris */
#endif /* WIN_NT */
	}

/* unmap all mapped files */

	for (xpm = client_maps; xpm; xpm = xpm->xpm_next) {
		xpm->xpm_flags |= XPMF_SERVER_SHUTDOWN;
#ifdef WIN_NT
		UnmapViewOfFile(xpm->xpm_address);
		CloseHandle(xpm->xpm_handle);
#else
		/* STUB : un map shared mmap file */
#endif
		xpm->xpm_handle = 0;
	}
}
#endif	// WIN_NT
#endif	// SUPERCLIENT

#ifdef SUPERCLIENT
#ifdef WIN_NT
static void server_watcher(void)
{
/**************************************
 *
 *      s e r v e r _ w a t c h e r
 *
 **************************************
 *
 * Functional description
 *      This thread just waits for the server
 *      process.   If that process goes away,
 *      this gets triggered and unmaps any
 *      currently mapped maps.
 *
 **************************************/
	DWORD result;

	for (;;) {
		if (exit_flag) {
			break;
		}
		result = WaitForSingleObject(server_process_handle, INFINITE);
		if (result == WAIT_OBJECT_0) {
			/* unmap and close all maps */

			server_shutdown();
			server_process_handle = 0;
			server_watcher_handle = 0;
			break;
		}
	}
}
#endif // WIN_NT
#endif // SUPERCLIENT

static int xdrxnet_create(
						  XDR * xdrs,
						  PORT port,
						  UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *      x d r x n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *      Initialize an XDR stream.
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


static bool_t xdrxnet_endofrecord( XDR * xdrs, bool_t flushnow)
{
/**************************************
 *
 *      x d r x n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return xnet_write(xdrs, flushnow);
}

#ifdef NOT_USED_OR_REPLACED
static void xnet_copy( SCHAR * from, SCHAR * to, int length)
{
/**************************************
 *
 *      x n e t _ c o p y
 *
 **************************************
 *
 * Functional description
 *      Copy a number of bytes;
 *
 **************************************/

	if (length)
		memcpy(to, from, length);
}
#endif

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


static int xnet_error(
					  PORT port,
					  TEXT * function, STATUS operation, int status)
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
	TEXT msg[64];
	TEXT node_name[MAXPATHLEN];
	TEXT *p;

	strcpy(node_name, ((SCHAR *) port->port_connection->str_data) + 2);
	p = strchr(node_name, '\\');
	if (p != NULL)
		*p = '\0';
	if (status) {
		xnet_gen_error(port, isc_network_error,
					   gds_arg_string, (STATUS) node_name,
					   isc_arg_gds, operation, SYS_ERR, status, 0);
#ifdef WIN_NT
		if (status != ERROR_CALL_NOT_IMPLEMENTED)
#endif
		{
			sprintf(msg, "XNET/xnet_error: %s errno = %d", function, status);
			gds__log(msg, 0, 0, 0, 0);
		}
	}
	else {
		xnet_gen_error(port, isc_network_error,
					   gds_arg_string, (STATUS) node_name,
					   isc_arg_gds, operation, 0);
	}
	return 0;
}


static void xnet_gen_error( PORT port, STATUS status, ...)
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
	STATUS *status_vector;

	port->port_flags |= PORT_broken;
	port->port_state = state_broken;
	status_vector = NULL;
	if (port->port_context != NULL)
		status_vector = port->port_context->rdb_status_vector;
	if (status_vector == NULL)
		status_vector = port->port_status_vector;
	if (status_vector != NULL) {
		STUFF_STATUS(status_vector, status)
			REMOTE_save_status_strings(status_vector);
	}
}


static bool_t xnet_getbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *      x n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Fetch a bunch of bytes into a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;
	SLONG to_copy;

	while (bytecount) {
		if (xdrs->x_handy >= bytecount)
			to_copy = bytecount;
		else
			to_copy = xdrs->x_handy;
		memcpy(buff, xdrs->x_private, to_copy);
		xdrs->x_handy -= to_copy;
		bytecount -= to_copy;
		xdrs->x_private += to_copy;
		buff += to_copy;
		if (bytecount && !xnet_read(xdrs))
			return FALSE;
	}

	return TRUE;
}


static bool_t xnet_getlong( XDR * xdrs,  SLONG * lp)
{
/**************************************
 *
 *      x n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast<SCHAR*>(&l), 4))
		return FALSE;
	*lp = ntohl(l);
	return TRUE;
}


static u_int xnet_getpostn( XDR * xdrs)
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


static caddr_t xnet_inline( XDR * xdrs, u_int bytecount)
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


static bool_t xnet_putbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *      x n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Fetch a bunch of bytes into a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;
	SLONG to_copy;

	THREAD_EXIT;
	while (bytecount) {
		if (xdrs->x_handy >= bytecount)
			to_copy = bytecount;
		else
			to_copy = xdrs->x_handy;
		/* need to have some kind of locking with the read process 
		 * ie. we cannot write once again to this are till last data has been
		 * received */
		if (to_copy) {
			PORT port = (PORT) xdrs->x_public;
			XCC xcc = (XCC) port->port_xcc;

			/* check to see that we have the send channel area, if not then wait for the
			 * receiver to finish reading from the common area */
			if (!xcc->xcc_send_channel_locked) {
				/* wait for receiver to read the last sent packet */
#ifdef WIN_NT
				if (WaitForSingleObject(xcc->xcc_recv_sem, INFINITE) ==
					WAIT_FAILED)
#else
				/* SEAN add ISC_event_wait here */
				if (0)
#endif
				{
					THREAD_ENTER;
					xnet_error(port, "write to map error", isc_net_write_err,
							   ERRNO);
					return FALSE;
				}
				xcc->xcc_send_channel_locked = TRUE;
			}
			memcpy(xdrs->x_private, buff, to_copy);
			xdrs->x_handy -= to_copy;
			bytecount -= to_copy;
			xdrs->x_private += to_copy;
			buff += to_copy;
		}
		if (bytecount && !xnet_write(xdrs, 0)) {
			THREAD_ENTER;
			return FALSE;
		}
	}

	THREAD_ENTER;
	return TRUE;
}


static bool_t xnet_putlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *      x n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs, reinterpret_cast<SCHAR*>(AOF32L(l)), 4);
}


static bool_t xnet_read( XDR * xdrs)
{
/**************************************
 *
 *      x n e t _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a buffer full of data.  If we receive a bad packet,
 *	send the moral equivalent of a NAK and retry.  ACK all
 *	partial packets.  Don't ACK the last packet -- the next
 *	message sent will handle this.
 *
 **************************************/
	PORT port;
	USHORT length;

	port = (PORT) xdrs->x_public;
	if (!packet_receive(port, &length))
		return FALSE;
	port->port_flags |= PORT_pend_ack;
	xdrs->x_handy = length;
	xdrs->x_private = xdrs->x_base;
	return TRUE;
}


static bool_t xnet_setpostn( XDR * xdrs, u_int bytecount)
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


static bool_t xnet_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *      x n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
	PORT port;
	SSHORT length;
	XCC xcc;

/* Encode the data portion of the packet */

	port = (PORT) xdrs->x_public;
	xcc = (XCC) port->port_xcc;
	length = xdrs->x_private - xdrs->x_base;
	if (!packet_send(port, length))
		return FALSE;
	port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = xcc->xcc_receive_channel->xch_size;
	return TRUE;
}


void XNET_release_all(void)
{
/**************************************
 *
 *      X N E T _ r e l e a s e _ a l l
 *
 **************************************
 *
 * Functional description
 *      Release all connections and dependant stuff.
 *
 **************************************/
	XCC xcc, nxcc;
	XPM xpm, nxpm;
	XPS xps;

/* get stuff to release and clear list heads */

	exit_flag++;
	if (exit_flag > 1) {
		return;
	}
	xcc = client_threads;
	nxcc = xcc;
	client_threads = NULL;
	xpm = client_maps;
	client_maps = NULL;

#ifdef WIN_NT
	if (server_watcher_handle) {
		TerminateThread(server_watcher_handle, 0);
		server_watcher_handle = 0;
	}
#endif

/* disconnect each thread */

	for (; xcc; xcc = xcc->xcc_next) {
		if (!(xcc->xcc_flags & XCCF_SERVER_SHUTDOWN)) {
			xps = (XPS) xcc->xcc_mapped_addr;
		}
	}

	xcc = nxcc;

/* for each xcc structure, release its database and close it down */

	for (; xcc; xcc = nxcc) {
		nxcc = xcc->xcc_next;

#ifdef WIN_NT
		/* free up semaphores */

		if (xcc->xcc_send_sem) {
			CloseHandle(xcc->xcc_send_sem);
		}
		if (xcc->xcc_send_sem) {
			CloseHandle(xcc->xcc_send_sem);
		}
		if (xcc->xcc_server_proc) {
			CloseHandle(xcc->xcc_server_proc);
		}
#else
		/* SEAN : Similar event_fini() for Solaris */
#endif
		ALLR_free((UCHAR *) xcc);
	}

/* unmap all mapped files */

	for (; xpm; xpm = nxpm) {
		nxpm = xpm->xpm_next;
		if (!(xpm->xpm_flags & XPMF_SERVER_SHUTDOWN)) {
#ifdef WIN_NT
			UnmapViewOfFile(xpm->xpm_address);
			CloseHandle(xpm->xpm_handle);
#else
			/* SEAN : unmap shared memory file */
#endif /* WIN_NT */
		}
		ALLR_free((UCHAR *) xpm);
	}
}

} // extern "C"
