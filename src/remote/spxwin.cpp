/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		spxwin.c
 *	DESCRIPTION:	SPX for PC Client Communications module.
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

#include "../jrd/time.h"
#include "../jrd/common.h"

#include "../jrd/ib_stdio.h"
#include <errno.h>
#include <string.h>
#include <direct.h>
#include <memory.h>
#include <stdarg.h>
#include <dos.h>

#include "../remote/remote.h"
#include "../include/jrd/gds.h"
#include "../jrd/iberr.h"
#include "../jrd/thd.h"
#include "../remote/ntoh_proto.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/isc_i_proto.h"
#include "../jrd/sch_proto.h"
#include "../remote/xdr_proto.h"
#include "../jrd/doserr.h"		/* include after remote.h */

/*
 * Need to undef FAILURE from common.h (#define FAILURE 1) and MIN from
 * common.h. (#define MIN(a,b)  (((a) < (b)) ? (a) : (b)).  They get
 * redefined by the netware include files, included with spxwin.h, as
 * #define FAILURE  -1 and #define MIN(a,b)  ((a) < (b) ? (a) : (b)).
*/
#ifdef FAILURE
#undef FAILURE
#endif

#ifdef MIN(a,b)
#undef MIN(a,b)
#endif

#include "../remote/spxwin.h"

#define NO_ITIMER
#define SYS_ERR		gds_arg_dos
#define ERRNO		errno
#define MAX_PTYPE	ptype_out_of_band


/* 
** The windows network libraries have to be dynamically loaded because the
** libraries may not be installed if the user does not have SPX (e.g. if
** they are using TCP).  This section defines the SPX functions to
** reference function pointers which will be set using the windows
** GetProcAddress function in initSPX.
**
** IMPORTANT:  If you add a call to a new SPX function, you must add
** a #define for it, define a function pointer for it, and add a
** call to GetProcAddress for it in initSPX so the function pointer
** gets initialized correctly. 
*/

#define IPXCancelEvent			IPXCancelEventPtr
#define IPXCloseSocket			IPXCloseSocketPtr
#define IPXGetInternetworkAddress	IPXGetInternetworkAddressPtr
#define IPXOpenSocket			IPXOpenSocketPtr
#define IPXRelinquishControl		IPXRelinquishControlPtr
#define IPXSPXDeinit			IPXSPXDeinitPtr
#define SPXEstablishConnection		SPXEstablishConnectionPtr
#define SPXInitialize			SPXInitializePtr
#define SPXListenForSequencedPacket	SPXListenForSequencedPacketPtr
#define SPXSendSequencedPacket		SPXSendSequencedPacketPtr
#define SPXTerminateConnection		SPXTerminateConnectionPtr
#define NWCallsInit			NWCallsInitPtr
#define NWGetDefaultConnectionID	NWGetDefaultConnectionIDPtr
#define NWReadPropertyValue		NWReadPropertyValuePtr
#define	NWWordSwap			NWWordSwapPtr

/*
** Declare function pointers, using function prototypes for type checking 
*/
int (FAR PASCAL * IPXCancelEventPtr) (DWORD IPXTaskID,
									  ECB FAR * eventControlBlock);
void (FAR PASCAL * IPXCloseSocketPtr) (DWORD IPXTaskID, WORD socket);
void (FAR PASCAL * IPXGetInternetworkAddressPtr) (DWORD IPXTaskID,
												  BYTE FAR * internetAddress);
int (FAR PASCAL * IPXOpenSocketPtr) (DWORD IPXTaskID, WORD FAR * socket,
									 BYTE socketType);
void (FAR PASCAL * IPXRelinquishControlPtr) (void);
void (FAR PASCAL * IPXSPXDeinitPtr) (DWORD IPXTaskId);

int (FAR PASCAL * SPXEstablishConnectionPtr) (DWORD IPXTaskID,
											  BYTE retryCount,
											  BYTE watchDog,
											  WORD FAR * SPXConnID,
											  ECB FAR * eventControlBlock);
int (FAR PASCAL * SPXInitializePtr) (DWORD FAR * IPXTaskID,
									 WORD maxECBs, WORD maxPacketSize,
									 BYTE FAR * majorRevisionNumber,
									 BYTE FAR * minorRevisionNumber,
									 WORD FAR * maxConnections,
									 WORD FAR * availableConnections);
void (FAR PASCAL * SPXListenForSequencedPacketPtr) (DWORD IPXTaskID,
													ECB FAR *
													eventControlBlock);
void (FAR PASCAL * SPXSendSequencedPacketPtr) (DWORD IPXTaskID,
											   WORD SPXConnID,
											   ECB FAR * eventControlBlock);
void (FAR PASCAL * SPXTerminateConnectionPtr)
  (DWORD IPXTaskID, WORD SPXConnID, ECB FAR * eventControlBlock);

NWCCODE(NWAPI * NWCallsInitPtr) (void NWPTR in, void NWPTR out);
NWCCODE(NWAPI * NWGetDefaultConnectionIDPtr) (NWCONN_HANDLE NWPTR conn);
NWCCODE(NWAPI * NWReadPropertyValuePtr) (NWCONN_HANDLE conn,
										 char NWPTR objectName,
										 WORD objectType,
										 char NWPTR propertyName,
										 BYTE segmentNumber,
										 BYTE NWPTR segmentData,
										 BYTE NWPTR moreSegments,
										 BYTE NWPTR flags);
WORD(NWAPI * NWWordSwapPtr) (WORD swapWord);




extern STR REMOTE_make_string();

void process_async_data(void);
static PORT aux_connect(PORT port, PACKET * packet, XDR_INT(*ast) ());
static void terminate_port(spx_port_handle * handle);
static accept_connection(PORT port, P_CNCT * cnct);
static PORT alloc_port(PORT parent);
static void cleanup_port(PORT port);
static void disconnect(PORT port);
static void spxnet_copy(SCHAR * from, SCHAR * to, int length);
static void spxnet_zero(SCHAR * address, int length);
static PORT receive(PORT main_port, PACKET * packet);
static send_full(PORT port, PACKET * packet);
static send_partial(PORT port, PACKET * packet);
static xdrspxnet_create(XDR * xdrs,
						PORT port,
						UCHAR * buffer, USHORT length, enum xdr_op x_op);
static bool_t xdrspxnet_endofrecord(XDR * xdrs, bool_t flushnow);
static void spxnet_destroy(void);
static int spxnet_error(PORT port,
						TEXT * operation, STATUS, int status, ECB * ecb);
static void spxnet_gen_error(PORT port, STATUS status, ...);
static bool_t spxnet_getbytes(XDR * xdrs, SCHAR * buff, u_int count);
static bool_t spxnet_getlong(XDR * xdrs, SLONG * lp);
static u_int spxnet_getpostn(XDR * xdrs);
static caddr_t spxnet_inline(XDR * xdrs, u_int bytecount);
static bool_t spxnet_putbytes(XDR * xdrs, SCHAR * buff, u_int count);
static bool_t spxnet_putlong(XDR * xdrs, SLONG * lp);
static bool_t spxnet_read(XDR * xdrs);
static bool_t spxnet_setpostn(XDR * xdrs, u_int bytecount);
static bool_t spxnet_write(XDR * xdrs, bool_t end_flag);
static void exit_handler(PORT main_port);
static packet_receive(PORT port,
					  UCHAR * buffer, SSHORT buffer_length, SSHORT * length);
static packet_receive_async(PORT port,
							UCHAR * buffer,
							SSHORT buffer_length, SSHORT * length);
static bool_t packet_send(PORT port, SCHAR * buffer, SSHORT buffer_length);
static void build_ecb_pool(spx_port_handle * handle, void *esr);
static void free_ecb_pool(spx_port_handle * handle);
static ECB *get_next_fragment(spx_port_handle * handle);
static void setup_tx_ecb(ECB FAR * ecb,
						 SPXHeader FAR * txhdr,
						 spx_port_handle * handle, SCHAR FAR * data);
static bool_t wait_for_free_tx_ecbs(PORT port);
static int initSPX(PORT);

static xdr_t::xdr_ops spxnet_ops = {
	spxnet_getlong,
	spxnet_putlong,
	spxnet_getbytes,
	spxnet_putbytes,
	spxnet_getpostn,
	spxnet_setpostn,
	spxnet_inline,
	spxnet_destroy
};

IPXAddress my_address;
static int SPX_initialized = FALSE;

/* This can't be a static since it's used by spxesr.c */
spx_port_handle *head_of_spx_port_list = 0;

/* 
** The net libs are only inited once, so save the handles and task id in
** a global for everyone to share.
*/
static HINSTANCE hSpxIpxLib, hNwCallsLib;
static DWORD tskid;





PORT SPXNET_analyze(
					TEXT * file_name,
					USHORT * file_length,
					STATUS * status_vector,
					TEXT * node_name, TEXT * user_string, USHORT uv_flag)
{
/**************************************
 *
 *	S P X N E T _ a n a l y z e
 *
 **************************************
 *
 * Functional description
 *	File_name is on node_name.
 *	Establish an external connection to node_name.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/
	RDB rdb;
	PORT port;
	PACKET *packet;
	P_CNCT *cnct;
	int eff_gid, eff_uid;
	SSHORT user_length;
	SCHAR outsegs;
	SCHAR outflag;
	UCHAR *p, user_id[200];
	TEXT buffer[64];
	struct p_cnct_repeat *protocol;
	WORD local_socket;
	WORD dest_socket = INTERBASE_SOCKET;
	SCHAR dest_address[128];
	int ret;
	HGLOBAL handle_handle;
	struct spx_port_handle FAR *handle;
	ECB connecb;
	SPXHeader connhdr;
	NWCONN_HANDLE connid;

	*file_length = strlen(file_name);

/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	rdb = (RDB) ALLOC(type_rdb);
	packet = &rdb->rdb_packet;

/* Pick up some user identification information */

	user_id[0] = CNCT_user;
	p = user_id + 2;
	ISC_get_user(p, &eff_uid, &eff_gid, 0, 0, 0, user_string);
	user_id[1] = (UCHAR) strlen((SCHAR *) p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	*p++ = CNCT_host;
	p++;
	ISC_get_host(p, 64);
	p[-1] = (UCHAR) strlen((SCHAR *) p);

	for (; *p; p++)
		if (*p >= 'A' && *p <= 'Z')
			*p = *p - 'A' + 'a';

	if ((eff_uid == -1) || uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}

	user_length = (SSHORT) (p - user_id);

/* Establish connection to server */
/* If we want user verification, we can't speak anything less than version 7 */
/* unlike other protocols, SPX can't talk to server versions which had the 
   bug in which only 5 protocols were supported, so go ahead and try more */

	cnct = &packet->p_cnct;
#ifdef SCROLLABLE_CURSORS
	cnct->p_cnct_count = 6;
#else
	cnct->p_cnct_count = 4;
#endif
	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = user_id;

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

#ifdef SCROLLABLE_CURSORS
	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = arch_generic;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 6;

	++protocol;

	protocol->p_cnct_version = PROTOCOL_SCROLLABLE_CURSORS;
	protocol->p_cnct_architecture = ARCHITECTURE;
	protocol->p_cnct_min_type = ptype_rpc;
	protocol->p_cnct_max_type = MAX_PTYPE;
	protocol->p_cnct_weight = 7;
#endif

/* Try connection using first set of protocols.  punt if error */

	cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = *file_length;
	cnct->p_cnct_file.cstr_address = (UCHAR *) file_name;

/* If we can't talk to a server, punt.  Let somebody else generate
   an error.  status_vector will have the network error info. */
	port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;

	if (port->port_connection)
		ALLR_free(port->port_connection);
	port->port_connection = REMOTE_make_string(node_name);

/* Load the network libraries, and initialize the global task id */
	if (initSPX(port))
		return NULL;

	ret = NWGetDefaultConnectionID(&connid);
	if (ret) {
		spxnet_error(port, "NWGetDefaultConnectionID", isc_net_lookup_err,
					 ret, NULL);
		return NULL;
	}

	ret =
		NWReadPropertyValue(connid, node_name, OT_FILE_SERVER, "NET_ADDRESS",
							1, dest_address, (LPBYTE) & outsegs,
							(LPBYTE) & outflag);

	if (ret != 0) {
		TEXT msg[64];

		spxnet_gen_error(port, isc_network_error,
						 gds_arg_string,
						 (STATUS) port->port_connection->str_data,
						 isc_arg_gds, isc_net_lookup_err, SYS_ERR,
						 ret - 30000 + SPX_ERROR_BEG, 0);

		sprintf(msg, "SPXNET/spxnet_error: ReadPropertyValue errno = %#x",
				ret);
		gds__log(msg, 0);
		return NULL;
	}

	local_socket = 0;
	ret = IPXOpenSocket(tskid, (void far *) &local_socket, 0xff);

	handle_handle =
		GlobalAlloc(GHND | GMEM_DDESHARE, sizeof(spx_port_handle));
	handle = (spx_port_handle FAR *) GlobalLock(handle_handle);
	handle->global_handle = handle_handle;
	handle->task_id = tskid;
	handle->local_socket = local_socket;
	memcpy(&(handle->server_address), dest_address, sizeof(IPXAddress));
	memcpy(&(handle->server_address.socket), &dest_socket, 2);
	build_ecb_pool(handle, 0);
	handle->next_fragment = 0;
	handle->window_handle = 0;
	handle->next = 0;
	handle->connection_id = 0;
	port->port_handle = (HANDLE) handle;

/* queue it up */
	if (head_of_spx_port_list == 0)
		head_of_spx_port_list = handle;
	else {
		handle->next = head_of_spx_port_list;
		head_of_spx_port_list = handle;
	}


	IPXGetInternetworkAddress(tskid, (BYTE FAR *) & my_address);
	memcpy(my_address.socket, &local_socket, 2);

	connhdr.length = NWWordSwap(sizeof(SPXHeader));
	connhdr.packetType = (SCHAR) 5;
	memcpy(&(connhdr.destination), &(handle->server_address),
		   sizeof(IPXAddress));

	connecb.socketNumber = local_socket;
	connecb.fragmentCount = 1;
	connecb.fragmentDescriptor[0].size = sizeof(SPXHeader);
	connecb.fragmentDescriptor[0].address = (SCHAR far *) & connhdr;
	connecb.ESRAddress = 0;

	ret =
		SPXEstablishConnection(tskid, (BYTE) 15, (BYTE) 0xFF,
							   (LPWORD) & connid, &connecb);

	if (!ret) {
		while (connecb.inUseFlag != 0)
			IPXRelinquishControl();
	}

	if (ret || connecb.completionCode) {
		spxnet_error(port, "SpxEstablishConnection", isc_net_connect_err,
					 ret ? ret : connecb.completionCode, NULL);
		disconnect(port);
		ALLR_release((BLK) rdb);
		return NULL;
	}

	handle->connection_id = connid;

	send_full(port, packet);

/* Get response packet from server. */
	rdb->rdb_port = port;
	port->port_context = rdb;
	if (!port->receive(packet)) {
		disconnect(port);
		ALLR_release((BLK) rdb);
		return NULL;
	}

	if (packet->p_operation != op_accept) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds__connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version 
   string to reflect it...  */

	sprintf(buffer, "%s/P%d", port->port_version->str_data,
			port->port_protocol);
	ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(buffer);

	if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE)
		port->port_flags |= PORT_symmetric;

	if (packet->p_acpt.p_acpt_type == ptype_rpc)
		port->port_flags |= PORT_rpc;

	if (packet->p_acpt.p_acpt_type != ptype_out_of_band)
		port->port_flags |= PORT_no_oob;

	return port;
}


static accept_connection(port, cnct)
	 PORT port;
	 P_CNCT *cnct;
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
	TEXT name[64], password[64], *id, *end, *p;
	STR string;
	int length, l;
#if FALSE
	int user_verification;
#endif

/* Default account to "guest" (in theory all packets contain a name) */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account and password, if given */

	id = (TEXT *) cnct->p_cnct_user_id.cstr_address;
	end = id + cnct->p_cnct_user_id.cstr_length;

#if FALSE
	user_verification = 0;
#endif
	while (id < end)
		switch (*id++) {
		case CNCT_user:
			length = l = *id++;
			port->port_user_name = string =
				(STR) ALLOCV(type_str, (int) length);
			string->str_length = length;
			if (length) {
				p = (TEXT *) string->str_data;
				do
					*p++ = *id++;
				while (--l);
			}
			strncpy(name, string->str_data, length);
			name[length] = (TEXT) 0;
			break;

		case CNCT_passwd:
			p = password;
			if ((length = *id++) != NULL)
				do
					*p++ = *id++;
				while (--length);
			*p = 0;
			break;

		case CNCT_user_verification:
#if FALSE
			user_verification = 1;
#endif
			id++;
			break;

		default:
			id += *id + 1;
		}

	return TRUE;
}


static PORT alloc_port(parent)
	 PORT parent;
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
	int size;

	size = BUFFER_SIZE;
	size *= 2;
	port = (PORT) ALLOCV(type_port, size);

	port->port_type = port_spx;
	port->port_state = state_pending;

	ISC_get_host(buffer, sizeof(buffer));
	port->port_host = REMOTE_make_string(buffer);
	port->port_connection = REMOTE_make_string(buffer);
	sprintf(buffer, "SPX (%s)", port->port_host->str_data);
	port->port_version = REMOTE_make_string(buffer);

	if (parent) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = parent->port_next = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_flags = parent->port_flags;
	}

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = 0;
	port->port_buff_size = BUFFER_SIZE;

	xdrspxnet_create(&port->port_send, port,
					 &port->port_buffer[BUFFER_SIZE], BUFFER_SIZE,
					 XDR_ENCODE);

	xdrspxnet_create(&port->port_receive, port, port->port_buffer, 0,
					 XDR_DECODE);

	return port;
}


static void disconnect(port)
	 PORT port;
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
	spx_port_handle FAR *handle;

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
	else {
		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}
	}

	handle = (spx_port_handle *) port->port_handle;

	if (handle != NULL) {
		if (handle->window_handle)
			DestroyWindow(handle->window_handle);

		terminate_port(handle);
	}
	cleanup_port(port);
	return;
}


static void cleanup_port(port)
	 PORT port;
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

	ALLR_release((UCHAR *) port);
	return;
}


static void terminate_port(spx_port_handle * handle)
{
/**************************************
 *
 *	t e r m i n a t e _ p o r t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ECB FAR *tecb;
	SPXHeader FAR *spx;
	spx_port_handle FAR *prev_handle;
	spx_port_handle *tmp_handle;
	HGLOBAL ecb_mem;
	UCHAR *t_ecb;

	if (handle == NULL)
		return;

	if (handle->connection_id) {
		ecb_mem = GlobalAlloc(GPTR | GMEM_DDESHARE,
							  (sizeof(ECB) + MAX_DATA + sizeof(SPXHeader)));
		t_ecb = (UCHAR *) GlobalLock(ecb_mem);

		/* Format and send the disconnect request to the other side */
		t_ecb += MAX_DATA;
		tecb = (ECB *) t_ecb;
		t_ecb += sizeof(ECB);
		spx = (void *) t_ecb;

		tecb->ESRAddress = 0;
		tecb->inUseFlag = 0;
		tecb->completionCode = 0;
		tecb->socketNumber = handle->local_socket;
		tecb->fragmentCount = 1;
		tecb->fragmentDescriptor[0].address = spx;
		tecb->fragmentDescriptor[0].size = sizeof(SPXHeader);
		spx->packetType = (SCHAR) 5;
		spx->length = sizeof(SPXHeader);

		SPXTerminateConnection(handle->task_id, handle->connection_id, tecb);
		while (tecb->inUseFlag != 0)
			IPXRelinquishControl();

		GlobalFree(ecb_mem);
	}

	IPXCloseSocket(handle->task_id, handle->local_socket);

/* Free the pool of ecb's for this connection */
	free_ecb_pool(handle);

/* unlink this structure from the list */
	if (head_of_spx_port_list == handle)
		head_of_spx_port_list = head_of_spx_port_list->next;
	else {
		prev_handle = head_of_spx_port_list;
		tmp_handle = head_of_spx_port_list;
		while (tmp_handle != NULL) {
			if (tmp_handle == handle)
				break;
			prev_handle = tmp_handle;
			tmp_handle = tmp_handle->next;
		}
		prev_handle->next = tmp_handle->next;
	}

	GlobalFree(handle->global_handle);
}


static void spxnet_copy(from, to, length)
	 SCHAR *from, *to;
	 int length;
{
/**************************************
 *
 *	s p x n e t _ c o p y
 *
 **************************************
 *
 * Functional description
 *	Copy a number of bytes;
 *
 **************************************/
	if (length)
		do
			*to++ = *from++;
		while ((--length) != 0);
}


static void spxnet_zero(address, length)
	 SCHAR *address;
	 int length;
{
/**************************************
 *
 *	s p x n e t _  z e r o
 *
 **************************************
 *
 * Functional description
 *	Zero a block of memory.
 *
 **************************************/
	if (length)
		do
			*address++ = 0;
		while ((--length) != 0);
}
static PORT receive(main_port, packet)
	 PORT main_port;
	 PACKET *packet;
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
		return (NULL);

	return main_port;
}


static send_full(port, packet)
	 PORT port;
	 PACKET *packet;
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

	return xdrspxnet_endofrecord(&port->port_send, TRUE);
}


static send_partial(port, packet)
	 PORT port;
	 PACKET *packet;
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


static xdrspxnet_create(xdrs, port, buffer, length, x_op)
	 XDR *xdrs;
	 PORT port;
	 UCHAR *buffer;
	 USHORT length;
	 enum xdr_op x_op;
{
/**************************************
 *
 *	x d r s p x n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/
	xdrs->x_public = (caddr_t) port;
	xdrs->x_base = xdrs->x_private = (SCHAR *) buffer;
	xdrs->x_handy = length;
	xdrs->x_ops = &spxnet_ops;
	xdrs->x_op = x_op;

	return TRUE;
}


static bool_t xdrspxnet_endofrecord(xdrs, flushnow)
	 XDR *xdrs;
	 bool_t flushnow;
{
/**************************************
 *
 *	x d r s p x n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return spxnet_write(xdrs, flushnow);
}


static void spxnet_destroy()
{
/**************************************
 *
 *	s p x n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/
}


static bool_t spxnet_getbytes(xdrs, buff, count)
	 XDR *xdrs;
	 SCHAR *buff;
	 u_int count;
{
/**************************************
 *
 *	s p x n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Get a bunch of bytes from a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > sizeof(GDS_QUAD)) {
		if (xdrs->x_handy >= bytecount) {
			memcpy(buff, xdrs->x_private, bytecount);
			xdrs->x_private += bytecount;
			xdrs->x_handy -= bytecount;
			return TRUE;
		}
		else {
			if (xdrs->x_handy > 0) {
				memcpy(buff, xdrs->x_private, xdrs->x_handy);
				xdrs->x_private += xdrs->x_handy;
				buff += xdrs->x_handy;
				bytecount -= xdrs->x_handy;
				xdrs->x_handy = 0;
			}
			if (!spxnet_read(xdrs))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*buff++ = *xdrs->x_private++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (!xdrs->x_handy && !spxnet_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}


static bool_t spxnet_getlong(xdrs, lp)
	 XDR *xdrs;
	 SLONG *lp;
{
/**************************************
 *
 *	s p x n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, &l, 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}


static u_int spxnet_getpostn(xdrs)
	 XDR *xdrs;
{
/**************************************
 *
 *	s p x n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/
	return (u_int) (xdrs->x_private - xdrs->x_base);
}


static caddr_t spxnet_inline(xdrs, bytecount)
	 XDR *xdrs;
	 u_int bytecount;
{
/**************************************
 *
 *	s p x n e t _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *	Return a pointer to somewhere in the buffer.
 *
 **************************************/
	if (bytecount > xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t spxnet_putbytes(xdrs, buff, count)
	 XDR *xdrs;
	 SCHAR *buff;
	 u_int count;
{
/**************************************
 *
 *	s p x n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Put a bunch of bytes to a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > sizeof(GDS_QUAD)) {
		if (xdrs->x_handy >= bytecount) {
			memcpy(xdrs->x_private, buff, bytecount);
			xdrs->x_private += bytecount;
			xdrs->x_handy -= bytecount;
			return TRUE;
		}
		else {
			if (xdrs->x_handy > 0) {
				memcpy(xdrs->x_private, buff, xdrs->x_handy);
				xdrs->x_private += xdrs->x_handy;
				buff += xdrs->x_handy;
				bytecount -= xdrs->x_handy;
				xdrs->x_handy = 0;
			}
			if (!spxnet_write(xdrs, 0))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do
			*xdrs->x_private++ = *buff++;
		while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (xdrs->x_handy <= 0 && !spxnet_write(xdrs, 0))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}


static bool_t spxnet_putlong(xdrs, lp)
	 XDR *xdrs;
	 SLONG *lp;
{
/**************************************
 *
 *	s p x n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	SLONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs, AOF32L(l), 4);
}


static bool_t spxnet_read(xdrs)
	 XDR *xdrs;
{
/**************************************
 *
 *	s p x n e t _ r e a d
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
	SSHORT length;
	SCHAR *p, *end;

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	end = p + BUFFER_SIZE;

/* If buffer is not completely empty, slide down what what's left */

	if (xdrs->x_handy > 0) {
		memmove(p, xdrs->x_private, xdrs->x_handy);
		p += xdrs->x_handy;
	}

/* If an ACK is pending, do an ACK.  The alternative is deadlock. */

	while (TRUE) {
		length = end - p;
		if (!packet_receive(port, p, length, &length))
			return FALSE;
		if (length >= 0) {
			p += length;
			break;
		}
		p -= length;
		if (!packet_send(port, 0, 0))
			return FALSE;
	}

	port->port_flags |= PORT_pend_ack;
	xdrs->x_handy = (int) ((SCHAR *) p - xdrs->x_base);
	xdrs->x_private = xdrs->x_base;

	return TRUE;
}


static bool_t spxnet_setpostn(xdrs, bytecount)
	 XDR *xdrs;
	 u_int bytecount;
{
/**************************************
 *
 *	s p x n e t _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Set the current position (which is also current length) from stream.
 *
 **************************************/
	if (bytecount > xdrs->x_handy)
		return FALSE;

	xdrs->x_private = xdrs->x_base + bytecount;

	return TRUE;
}


static bool_t spxnet_write(xdrs, end_flag)
	 XDR *xdrs;
	 bool_t end_flag;
{
/**************************************
 *
 *	s p x n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
	SCHAR *p;
	PORT port;
	SSHORT length;

/* Encode the data portion of the packet */

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	length = xdrs->x_private - p;

/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	p = xdrs->x_base;

	if (!packet_send(port, p, length))
		return FALSE;

	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = BUFFER_SIZE;

	return TRUE;
}


static void exit_handler(main_port)
	 PORT main_port;
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

	for (port = main_port; port; port = port->port_next)
		terminate_port((spx_port_handle *) port->port_handle);
}


static bool_t packet_send(port, buffer, buffer_length)
	 PORT port;
	 SCHAR *buffer;
	 SSHORT buffer_length;
{
/**************************************
 *
 *	p a c k e t _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Send some data on it's way.  
 *
 **************************************/
	ECB *ecb;
	SPXHeader FAR *txhdr;
	spx_port_handle *handle;
	SCHAR FAR *tx_mem;
	SCHAR FAR *data;
	int i, ccode;
	WORD length;
	WORD index;

	handle = (spx_port_handle *) port->port_handle;

	if (handle == NULL)
		/* The connection already died and was closed */
		return spxnet_error(port, "SPXSendSequencedPacket", isc_net_write_err,
							SPX_CONNECTION_TERMINATED, NULL);

	index = 0;
	ccode = 0;

/* Keep looping through the ecbs until all data is sent */
	while (buffer_length && !ccode) {
		i = 0;
		tx_mem = handle->tx_pool;

		while (buffer_length && i < TX_POOL_SIZE) {
			data = tx_mem;
			tx_mem += MAX_DATA;

			ecb = (ECB *) tx_mem;
			tx_mem += sizeof(ECB);

			txhdr = (SPXHeader FAR *) tx_mem;
			tx_mem += sizeof(SPXHeader);
			setup_tx_ecb(ecb, txhdr, handle, data);

			length = buffer_length <= MAX_DATA ? buffer_length : MAX_DATA;
			buffer_length -= length;

			if (!buffer_length)
				/* This is the last packet */
				txhdr->connectionControl |= 0x10;
			else
				txhdr->connectionControl = 0;

			txhdr->length = NWWordSwap(sizeof(SPXHeader) + length);
			ecb->fragmentDescriptor[1].size = length;
			memcpy(data, &(buffer[index]), length);
			index += length;

			SPXSendSequencedPacket(handle->task_id, handle->connection_id,
								   ecb);

			i++;
		}

		/* We used up all the ecbs, wait for them to clear and start again */
		if (!wait_for_free_tx_ecbs(port))
			return FALSE;
	}

	return (TRUE);
}


static bool_t wait_for_free_tx_ecbs(PORT port)
{
/**************************************
 *
 * w a i t _ f o r _ f r e e _ t x _ e c b s
 *
 **************************************
 *
 * Functional description
 * wait for free tx ecb pool
 *
 **************************************/
	SCHAR FAR *tx_mem;
	ECB FAR *ecb;
	int i;
	spx_port_handle *handle;

	handle = (spx_port_handle *) port->port_handle;
	tx_mem = handle->tx_pool;
	i = 0;

	while (i < TX_POOL_SIZE) {
		tx_mem += MAX_DATA;
		ecb = (ECB *) tx_mem;
		tx_mem += sizeof(ECB) + sizeof(SPXHeader);

		while (ecb->inUseFlag != 0)
			IPXRelinquishControl();
		if (ecb->completionCode != 0)
			return ((bool_t) (spxnet_error(port, "SPXSendSequencedPacket",
										   isc_net_write_err,
										   ecb->completionCode, ecb)));
		i++;
	}

	return TRUE;
}

static void setup_tx_ecb(ECB FAR * ecb, SPXHeader FAR * txhdr,
						 spx_port_handle * handle, SCHAR FAR * data)
{
/**************************************
 *
 * s e t u p _ t x _ e c b
 *
 **************************************
 *
 * Functional description
 * set up tx ecbs
 *
 **************************************/
	ecb->ESRAddress = 0;
	ecb->inUseFlag = 0;
	ecb->completionCode = 0;
	ecb->fragmentCount = 2;
	ecb->fragmentDescriptor[0].address = txhdr;
	ecb->fragmentDescriptor[0].size = sizeof(SPXHeader);
	ecb->fragmentDescriptor[1].address = data;
	txhdr->packetType = (SCHAR) 5;
	txhdr->dataStreamType = 0;
	memcpy(&(txhdr->destination.network), &(handle->server_address), 12);
	memcpy(&(txhdr->source.network), &my_address, 12);
}

static packet_receive(port, buffer, buffer_length, length)
	 PORT port;
	 UCHAR *buffer;
	 SSHORT buffer_length, *length;
{
/**************************************
 *
 *	p a c k e t _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Receive a packet and pass on it's goodness.  If it's good,
 *	return TRUE and the reported length of the packet, and update
 *	the receive sequence number.  If it's bad, return FALSE.  If it's
 *	a duplicate message, just ignore it.
 *
 **************************************/
	SCHAR FAR *data;
	ECB FAR *ecb;
	SPXHeader FAR *spx;
	spx_port_handle *handle;
	WORD len;
	WORD buffer_offset;
	SCHAR last_fragment = FALSE;

	handle = (spx_port_handle *) port->port_handle;
	if (handle == NULL)
		/* The connection already died and was closed */
		return spxnet_error(port, "SPXListenForSequencedPacket",
							isc_net_read_err, SPX_CONNECTION_TERMINATED,
							NULL);

	buffer_offset = 0;

	last_fragment = FALSE;
	len = 0;

	while (!last_fragment) {
		ecb = get_next_fragment(handle);
		if (!ecb)
			return 0;

		spx = (SPXHeader *) ecb->fragmentDescriptor[0].address;
		data = ecb->fragmentDescriptor[1].address;
		data += sizeof(SLONG);
		len = spx->length;
		len = NWWordSwap(len);
		len -= (sizeof(SPXHeader) + sizeof(SLONG));

		if ((spx->dataStreamType == 0xFE) || (ecb->completionCode != 0)) {
			spxnet_error(port, "SPXListenForSequencedPacket",
						 isc_net_read_err, ecb->completionCode, ecb);

			/* If the connection is still valid, repost the listen on this ecb */
			if (port->port_handle != NULL)
				SPXListenForSequencedPacket(handle->task_id, ecb);
			return 0;
		}
		memcpy(buffer + buffer_offset, data, len);
		buffer_offset += len;
		if (spx->connectionControl & 0x10)
			last_fragment = TRUE;

		/* Repost the listen for this ecb */
		SPXListenForSequencedPacket(handle->task_id, ecb);
	}
	*length = buffer_offset;
	return (TRUE);
}


static ECB *get_next_fragment(spx_port_handle * handle)
{
/**************************************
 *
 * g e t _ n e x t _ f r a g m e n t
 *
 **************************************/
	ECB FAR *ecb;
	int i;
	SCHAR FAR *mem;
	SPXHeader FAR *spx;
	SCHAR FAR *data;
	SLONG fragment_number;
	SCHAR found = FALSE;

	while (!found) {
		i = 0;
		mem = handle->rx_pool;
		while (i < RX_POOL_SIZE) {
			mem += MAX_DATA;
			ecb = (ECB *) mem;
			mem += sizeof(ECB);
			mem += sizeof(SPXHeader);

			if (ecb->inUseFlag == 0) {
				spx = (SPXHeader *) ecb->fragmentDescriptor[0].address;
				if (ecb->completionCode || spx->dataStreamType == 0xFE)
					return ecb;

				data = ecb->fragmentDescriptor[1].address;
				fragment_number = *((SLONG *) data);
				if (fragment_number == handle->next_fragment) {
					found = TRUE;
					break;
				}
			}
			i++;
		}
		if (!found) {
			Yield();
		}
	}
	handle->next_fragment++;
	return (ecb);
}


static void build_ecb_pool(spx_port_handle * handle, void *esr)
{
/**************************************
 *
 * b u i l d _ e c b _ p o o l
 *
 **************************************/
	int i;
	UCHAR *data;
	ECB *ecb;
	SPXHeader *spx;
	UCHAR *rx_mem;


	handle->rx_global = GlobalAlloc(GPTR | GMEM_DDESHARE,
									RX_POOL_SIZE * (sizeof(ECB) + MAX_DATA +
													sizeof(SPXHeader)));
	rx_mem = GlobalLock(handle->rx_global);
	handle->rx_pool = rx_mem;

	handle->tx_global = GlobalAlloc(GPTR | GMEM_DDESHARE,
									TX_POOL_SIZE * (sizeof(ECB) + MAX_DATA +
													sizeof(SPXHeader)));
	handle->tx_pool = GlobalLock(handle->tx_global);


	i = 0;
	while (i < RX_POOL_SIZE) {
		data = rx_mem;
		rx_mem += MAX_DATA;
		ecb = (ECB *) rx_mem;
		rx_mem += sizeof(ECB);
		spx = (SPXHeader *) rx_mem;
		rx_mem += sizeof(SPXHeader);

		ecb->ESRAddress = esr;
		ecb->inUseFlag = 0;
		ecb->completionCode = 0;
		ecb->socketNumber = handle->local_socket;
		ecb->fragmentCount = 2;
		ecb->fragmentDescriptor[0].address = spx;
		ecb->fragmentDescriptor[0].size = sizeof(SPXHeader);
		ecb->fragmentDescriptor[1].address = data;
		ecb->fragmentDescriptor[1].size = MAX_DATA;
		spx->packetType = (SCHAR) 5;
		spx->length = sizeof(SPXHeader);
		SPXListenForSequencedPacket(handle->task_id, ecb);

		i++;
	}
}


static void free_ecb_pool(spx_port_handle * handle)
{
/**************************************
 *
 * f r e e _ e c b _ p o o l
 *
 **************************************/
	int i = 0;
	int ccode;
	ECB FAR *ecb;
	UCHAR FAR *rx_mem;
	UCHAR msg[128];

	rx_mem = handle->rx_pool;

/* Cancel any outstanding listens on the ECBs */
	while (i < RX_POOL_SIZE) {
		/* Get the ecb by skipping past the data */
		rx_mem += MAX_DATA;
		ecb = (ECB *) rx_mem;
		/* Increment rx_mem past ecb and spx header to the next block */
		rx_mem += sizeof(ECB) + sizeof(SPXHeader);

		if (ecb->inUseFlag != 0) {
			ccode = IPXCancelEvent(handle->task_id, ecb);
			if (ccode) {
				sprintf(msg,
						"SPXWIN/spxwin_error:  IPXCancelEvent errno = %#x",
						ccode);
				gds__log(msg, 0);
			}
			else {
				/* Loop, waiting for the event to be canceled */
				while (ecb->completionCode != 0xfc)
					IPXRelinquishControl();
			}
		}
		i++;
	}

/* Free the ecb memory */
	GlobalFree(handle->rx_global);
	GlobalFree(handle->tx_global);
}


void spx_cleanup(void *arg)
{
/**************************************
 *
 *	spx_cleanup
 *
 **************************************
 *
 * Functional description:
 *	Cleanup on exit
 *
 **************************************/
	spx_port_handle *port_handle = head_of_spx_port_list;

	if (SPX_initialized) {
		/* Free any open connections before terminating */
		while (port_handle != NULL) {
			/* 
			   ** Can't call disconnect at this point since the port memory block was
			   ** already freed.
			 */
			terminate_port(port_handle);
			/* 
			   ** disconnect will delete the handle from the list, so rather
			   ** than walking the list, just keep deleting the front until it's
			   ** empty.
			 */
			port_handle = head_of_spx_port_list;
		}

		IPXSPXDeinit(tskid);
		FreeLibrary(hSpxIpxLib);
		FreeLibrary(hNwCallsLib);

		SPX_initialized = FALSE;
	}
}


static int spxnet_error(
						PORT port,
						TEXT * function,
						STATUS operation, int status, ECB * ecb)
{
/**************************************
 *
 *	s p x n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  Call
 * 	spxnet_gen_error with the appropriate args
 *	to format the status vector if any.
 *	In any case, return NULL, which
 *	is used to indicate an error.
 *
 **************************************/
	TEXT msg[64];
	SPXHeader *spx;

	if (ecb != NULL) {
		/* 
		   ** If we have an ecb, check if the connection died on the
		   ** other side and if so, free the resources on our side.
		 */
		spx = (SPXHeader *) (ecb->fragmentDescriptor[0].address);
		if ((spx->dataStreamType == 0xFE) ||
			(ecb->completionCode == SPX_CONNECTION_TERMINATED) ||
			(ecb->completionCode == SPX_CONNECTION_FAILED)) {
			terminate_port((spx_port_handle *) port->port_handle);
			port->port_handle = NULL;
		}
	}

	if (status) {
		spxnet_gen_error(port, isc_network_error,
						 gds_arg_string,
						 (STATUS) port->port_connection->str_data,
						 isc_arg_gds, operation, SYS_ERR,
						 status + SPX_ERROR_BEG, 0);

		sprintf(msg, "SPXNET/spxnet_error: %s errno = %#x", function, status);
		gds__log(msg, 0);
	}
	else {
		/* No status value, just format the basic arguments. */

		spxnet_gen_error(port, isc_network_error,
						 gds_arg_string,
						 (STATUS) port->port_connection->str_data,
						 isc_arg_gds, operation, 0);
	}

	return (int) NULL;
}


static void spxnet_gen_error( PORT port, STATUS status, ...)
{
/**************************************
 *
 *	s p x n e t _ g e n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error has occurred.  
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


#define MAX_TASKS 128

#define GET_WIN_ENTRYPOINT(hlib, funcp, funcname) \
    funcp = GetProcAddress (hlib, funcname);\
    if (funcp == NULL) \
	{ return FAILURE; }

static int initSPX(PORT port)
{
/**************************************
 *
 *	i n i t S P X
 *
 **************************************
 *
 * Functional description:
 *	Initializes the SPX library.
 *
 **************************************/
	int ret = SUCCESS;
	BYTE majrev, minrev;		/* major and minor rev# of SPX      */
	WORD maxcon, avacon;		/* max and avail # of connections   */
	extern void spx_cleanup(void *);
	extern HINSTANCE LoadDll(UCHAR *);

	if (!SPX_initialized) {
		hSpxIpxLib = hNwCallsLib = 0;

		hSpxIpxLib = LoadDll("nwipxspx.dll");
		if (hSpxIpxLib) {
			GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXCancelEventPtr,
							   "IPXCancelEvent")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXCloseSocketPtr,
								   "IPXCloseSocket")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXGetInternetworkAddressPtr,
								   "IPXGetInternetworkAddress")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXOpenSocketPtr,
								   "IPXOpenSocket")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXRelinquishControlPtr,
								   "IPXRelinquishControl")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, IPXSPXDeinitPtr,
								   "IPXSPXDeinit")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, SPXEstablishConnectionPtr,
								   "SPXEstablishConnection")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, SPXInitializePtr,
								   "SPXInitialize")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, SPXListenForSequencedPacketPtr,
								   "SPXListenForSequencedPacket")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, SPXSendSequencedPacketPtr,
								   "SPXSendSequencedPacket")
				GET_WIN_ENTRYPOINT(hSpxIpxLib, SPXTerminateConnectionPtr,
								   "SPXTerminateConnection")

				/* 
				   ** Call SPX Initialize to see if the network is installed.
				   ** On input, tskid is set to indicate resources should be
				   ** allocated in a pool for access by multiple apps.  On output,
				   ** it will be the task id used by later SPX calls.
				 */
				tskid = 0xffffffffL;
			ret = SPXInitialize(&tskid, RX_POOL_SIZE, 576,
								&majrev,
								&minrev,
								(LPWORD) & maxcon, (LPWORD) & avacon);

			/* 
			   ** This needs better error handling so the user gets a 
			   ** status vector explaining what went wrong 
			 */
			if (ret == SPX_INSTALLED || ret == IPXSPX_PREV_INIT) {
				/* 
				   ** If SPX is around, go ahead and load nwcalls dll.  Otherwise,
				   ** if SPX is not installed then loading nwcalls will cause
				   ** windows to get an error looking for netware.dll
				 */
				ret = SUCCESS;
				hNwCallsLib = LoadDll("nwcalls.dll");
				if (hNwCallsLib) {
					GET_WIN_ENTRYPOINT(hNwCallsLib, NWCallsInitPtr,
									   "NWCallsInit")
						GET_WIN_ENTRYPOINT(hNwCallsLib,
										   NWGetDefaultConnectionIDPtr,
										   "NWGetDefaultConnectionID")
						GET_WIN_ENTRYPOINT(hNwCallsLib,
										   NWReadPropertyValuePtr,
										   "NWReadPropertyValue")
						GET_WIN_ENTRYPOINT(hNwCallsLib, NWWordSwapPtr,
										   "NWWordSwap")
				}
				else {
					spxnet_gen_error(port,
									 isc_network_error,
									 gds_arg_string,
									 port->port_connection->str_data,
									 isc_arg_gds,
									 isc_net_init_error,
									 isc_arg_gds,
									 isc_loadlib_failure,
									 isc_arg_string, "nwcalls.dll", 0);
					IPXSPXDeinit(tskid);
					ret = FAILURE;
				}
			}
			else {
				/* There was an error initing SPX */
				spxnet_error(port, "SPXInitialize", isc_net_init_error, ret,
							 NULL);
				ret = FAILURE;
			}
		}
		else {
			/* Unable to load nwipxspx.dll */
			spxnet_gen_error(port,
							 isc_network_error,
							 gds_arg_string,
							 port->port_connection->str_data,
							 isc_arg_gds,
							 isc_net_init_error,
							 isc_arg_gds,
							 isc_loadlib_failure,
							 isc_arg_string, "nwipxspx.dll", 0);
			ret = FAILURE;
		}

		if (ret != SUCCESS) {
			if (hSpxIpxLib)
				FreeLibrary(hSpxIpxLib);
			if (hNwCallsLib)
				FreeLibrary(hNwCallsLib);
			SPX_initialized = FALSE;
			ret = FAILURE;
		}
		else {
			SPX_initialized = TRUE;
		}
	}

	return ret;
}


static PORT aux_connect(PORT port, PACKET * packet, XDR_INT(*ast) ())
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	P_RESP *response;
	PORT new_port;
	spx_port_handle FAR *handle;
	spx_port_handle FAR *new_handle;
	HGLOBAL new_handle_handle;
	WORD local_socket;
	ECB connecb;
	SPXHeader connhdr;
	NWCONN_HANDLE connid;
	IPXAddress dest_address;
	int ret;
	DWORD tskid;
	extern HINSTANCE hInstance;
	HWND hwnd;

/* format response packet with my address in it */
	response = &packet->p_resp;
	spxnet_copy(response->p_resp_data.cstr_address, &dest_address,
				response->p_resp_data.cstr_length);

/* get the spx port handle */
	handle = (spx_port_handle *) port->port_handle;

/* get the task id */
	tskid = handle->task_id;

/* alloc a new port */
	port->port_async = new_port = alloc_port(port->port_parent);
	new_port->port_flags |= PORT_async;
	new_port->port_flags |= port->port_flags & PORT_no_oob;
	new_port->port_ast = ast;

	local_socket = 0;
	ret = IPXOpenSocket(tskid, (void far *) &local_socket, 0xff);

/* set up new spx port handle */
	new_handle_handle =
		GlobalAlloc(GHND | GMEM_DDESHARE, sizeof(spx_port_handle));
	new_handle = (spx_port_handle FAR *) GlobalLock(new_handle_handle);
	new_handle->global_handle = new_handle_handle;
	new_handle->task_id = handle->task_id;
	new_handle->local_socket = local_socket;
	memcpy(&(new_handle->server_address), &(dest_address),
		   sizeof(IPXAddress));
	new_handle->next_fragment = 0;
	new_handle->next = 0;
	build_ecb_pool(new_handle, process_async_data);

/* Create the window which will get posted to in the network esr */
	hwnd = CreateWindow("RemoteMsgClassSpx", "RemoteMsgWndSpx",
						WS_POPUP,
						CW_USEDEFAULT, CW_USEDEFAULT,
						CW_USEDEFAULT, CW_USEDEFAULT,
						NULL, NULL, hInstance, NULL);
	SetWindowLong(hwnd, 0, (long) new_port);
	new_handle->window_handle = hwnd;

/* set up ecb to use to establish connection */
	connhdr.length = NWWordSwap(sizeof(SPXHeader));
	connhdr.packetType = (SCHAR) 5;
	memcpy(&(connhdr.destination), &(dest_address), sizeof(IPXAddress));

	connecb.socketNumber = local_socket;
	connecb.fragmentCount = 1;
	connecb.fragmentDescriptor[0].size = sizeof(SPXHeader);
	connecb.fragmentDescriptor[0].address = (SCHAR far *) & connhdr;
	connecb.ESRAddress = 0;


/* queue it up */
	if (head_of_spx_port_list == 0)
		head_of_spx_port_list = new_handle;
	else {
		new_handle->next = head_of_spx_port_list;
		head_of_spx_port_list = new_handle;
	}

/* establish a connection */
	ret =
		SPXEstablishConnection(tskid, (BYTE) 0x30, (BYTE) 0xFF,
							   (LPWORD) & connid, &connecb);

/* wait for connection to be established */
	if (!ret) {
		while (connecb.inUseFlag != 0)
			IPXRelinquishControl();
	}

	if (ret || connecb.completionCode) {
		spxnet_error(port, "SpxEstablishConnection",
					 isc_net_event_connect_err,
					 ret ? ret : connecb.completionCode, NULL);
		terminate_port(new_handle);
		return NULL;
	}

/* set new connection info */
	new_handle->connection_id = connid;
	new_port->port_handle = (HANDLE) new_handle;

	return new_port;
}


LRESULT FAR PASCAL spxnet_wndproc(HWND hwnd, UINT message, WPARAM wParam,
								  LPARAM lParam)
{
/**************************************
 *
 *	s p x n e t _ w n d p r o c
 *
 * called when app notices a message has been posted
 * to this window.
 *
 **************************************
 *
 * Functional description:
 *	This is the windows equivalent of inet_handler.
 *	When an async request completes, a message will be posted
 *	to this window.  The function then calls the ast function
 *	in the port structure (saved in the window extra bytes)
 *	and re-initiates the async listen on the port.
 *
 **************************************/
	PORT port;

	if (message == WM_USER + 1) {
		/* get port */
		port = (PORT) GetWindowLong(hwnd, 0);

		/* call the ast */

		(port->port_ast) (port);

		/* Tell Windows it does not need to process the message */
		return FALSE;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
