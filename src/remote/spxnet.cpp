/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		spxnet.c
 *	DESCRIPTION:	SPX Communications module.
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
#include "../jrd/ib_stdio.h"
#include <errno.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <process.h>
#include <signal.h>
#include <advanced.h>
#include <malloc.h>
#include <library.h>
#include <sap.h>
#include <nwbindry.h>
#include <nwtypes.h>
#include <nwipxspx.h>
#include <nwsemaph.h>
#include <nwmisc.h>
#include <nwdsapi.h>

#include "../jrd/time.h"
#include "../jrd/common.h"
#include "../jrd/gds.h"
#include "../jrd/thd.h"
#include "../jrd/gdsasser.h"
#include "../remote/remote.h"

#define PROTOCOL_NAME   "gds_db"
#define NO_ITIMER
#define NO_FORK
#define MAX_SEQUENCE    256
#define MAX_SPX_DATA_SIZE       (576 - sizeof(SPX_HEADER) - sizeof(ULONG))
#define BUFFER_SIZE     4000
#define RX_BUFFER_SIZE  (BUFFER_SIZE * 2)	/* Space for two messages */
#define ALLOC_SIGNATURE 0x54524C41
#define SPX_RETRY_COUNT         20
#define SYS_ERR         gds_arg_dos
#define ERRNO   errno
#define EINTR           0
#define INTERBASE_SOCKET 0x8794
#define RX_POOL_SIZE    100

typedef struct rx_data {
	WORD cur_length;
	UCHAR *buffer;
	WORD max_len;
	ULONG semaphore;
	UCHAR rx_done;
	UCHAR client_address[12];
	UCHAR terminated;
	UCHAR buffer_full;
	ULONG fragment_number;
} rx_data;

typedef struct ecb_blk {
	SPX_ECB spx_ecb;
	SPX_HEADER header;
	UCHAR data[MAX_SPX_DATA_SIZE + sizeof(ULONG)];
}
ecb_blk;

typedef struct async_port_handle {
	ecb_blk *spx_lecb;
	WORD connid;
	ULONG event_listen_semaphore;
	USHORT event_socket;
	ecb_blk *ecb_pool;
} async_port_handle;

UCHAR my_network_address[20];
rx_data *spx_rx_data[0x10000];
ecb_blk *main_ecb_pool;
ULONG spx_rx_semaphore;
ULONG spx_sent_semaphore;
ULONG spx_event_semaphore;
ULONG spx_listen_semaphore;
UCHAR spx_receive_active;
UCHAR spx_sent_active;
USHORT spx_socket = INTERBASE_SOCKET;
UCHAR spx_exit = FALSE;
UCHAR admin[255];
UCHAR admin_password[255];
ecb_blk spx_lecb;

SPX_ECB *spx_receive_queue = 0;
SPX_ECB *spx_sent_queue = 0;
SPX_ECB *spx_listen_queue = 0;
SPX_ECB *spx_event_queue = 0;

#ifdef OBSOLETE
void spx_terminate_connection(PORT port);
#endif /* OBSOLETE  */
int spx_check_ecb_error(ecb_blk * ecb);
static BOOLEAN process_rx(ecb_blk * rx_ecb);
void spx_sent_process(void);
ecb_blk *spx_post_receive_ecbs(USHORT socket, SPX_ECB ** queue_head,
							   ULONG semaphore, USHORT pool_size);
extern UpdateUserName(UCHAR *);
extern STR REMOTE_make_string(UCHAR * input);
PORT SPXNET_listen(TEXT * name, PACKET * packet, STATUS * status_vector,
				   USHORT flag);
static accept_connection(PORT port, P_CNCT * cnct);
static PORT alloc_port(PORT parent);
static void cleanup_port(PORT port);
static void disconnect(PORT port);
#ifdef OBSOLETE
static void exit_handler();
#endif /* OBSOLETE */
static void spxnet_destroy(void);
static void spxnet_copy(UCHAR * from, UCHAR * to, int length);
static void spxnet_zero(UCHAR * address, int length);
static PORT receive(PORT main_port, PACKET * packet);
static send_full(PORT port, PACKET * packet);
static send_partial(PORT port, PACKET * packet);
static xdrspxnet_create(XDR * xdrs, PORT port, UCHAR * buffer, USHORT length,
						enum xdr_op x_op);
static bool_t xdrspxnet_endofrecord(XDR * xdrs, bool_t flushnow);
static spxnet_error(PORT port, TEXT * operation, int status);
static bool_t spxnet_getbytes(XDR * xdrs, UCHAR * buff, u_int count);
static bool_t spxnet_getULONG(XDR * xdrs, ULONG * lp);
static u_int spxnet_getpostn(XDR * xdrs);
static caddr_t spxnet_inline(XDR * xdrs, u_int bytecount);
static bool_t spxnet_putbytes(XDR * xdrs, UCHAR * buff, u_int count);
static bool_t spxnet_putULONG(XDR * xdrs, ULONG * lp);
static bool_t spxnet_read(XDR * xdrs);
static bool_t spxnet_setpostn(XDR * xdrs, u_int bytecount);
static bool_t spxnet_write(XDR * xdrs, bool_t end_flag);
packet_receive(PORT port, UCHAR * buffer, SSHORT buffer_length,
			   SSHORT * length);
bool_t packet_send(PORT port, UCHAR * buffer, SSHORT buffer_length);
static void dequeue_ecb(SPX_ECB ** queue, ecb_blk * remove);
static void spx_send(UCHAR * buffer, int buffer_length);
static void spxnet_done(void);
static PORT aux_request(PORT port, PACKET * packet);
static PORT aux_connect(PORT port, PACKET * packet, void (*ast) ());
void spx_receive_process(void);
static BOOLEAN wait_on_local_semaphore_timeout(ULONG semaphore,
											   USHORT timeout);

static xdr_t::xdr_ops spxnet_ops = {
	spxnet_getULONG,
	spxnet_putULONG,
	spxnet_getbytes,
	spxnet_putbytes,
	spxnet_getpostn,
	spxnet_setpostn,
	spxnet_inline,
	spxnet_destroy
};


int SPXNET_init()
{
/**************************************
 *
 *      S P X N E T _ i n i t
 *
 **************************************
 *
 * Functional description
 * initialize spx
 *
 **************************************/
	int i, j;
	int ccode;
	rx_data *rx;

	for (i = 0; i < 0x10000; i++)
		spx_rx_data[i] = 0;

	ccode = IpxGetInternetworkAddress(my_network_address);
	if (ccode != ESUCCESS)
		return (-1);

	ccode = SpxOpenSocket(&spx_socket);
	if (ccode != ESUCCESS)
		return (-1);

	spx_rx_semaphore = OpenLocalSemaphore(0);
	if (spx_rx_semaphore == 0)
		return (-1);

	spx_event_semaphore = OpenLocalSemaphore(0);
	if (spx_event_semaphore == 0)
		return (-1);

	spx_sent_semaphore = OpenLocalSemaphore(0);
	if (spx_sent_semaphore == 0)
		return (-1);

	main_ecb_pool =
		spx_post_receive_ecbs(spx_socket, &spx_receive_queue,
							  spx_rx_semaphore, RX_POOL_SIZE);

	if (main_ecb_pool != NULL)
		ccode = ESUCCESS;
	else
		ccode = EFAILURE;

	if (ccode != ESUCCESS)
		return (-1);

	ccode = BeginThread((void *) spx_receive_process, 0, 0, 0);
	if (ccode == EFAILURE)
		return (-1);

	ccode = BeginThread((void *) spx_sent_process, 0, 0, 0);
	if (ccode == EFAILURE)
		return (-1);

	return (0);
}


void SPXNET_shutdown()
{
/**************************************
 *
 *      S P X N E T _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 * de - initialize spx
 *
 **************************************/
	int i;
	rx_data *rx;

	spx_exit = TRUE;

	if (spx_rx_semaphore != NULL)
		SignalLocalSemaphore(spx_rx_semaphore);

	if (spx_event_semaphore != NULL)
		SignalLocalSemaphore(spx_event_semaphore);

	if (spx_sent_semaphore != NULL)
		SignalLocalSemaphore(spx_sent_semaphore);

	SpxCloseSocket(spx_socket);

	if (main_ecb_pool != NULL)
		for (i = 0; i < RX_POOL_SIZE; i++)
			SpxCancelEvent(&main_ecb_pool[i].spx_ecb);

	SpxCancelEvent(&(spx_lecb.spx_ecb));

	while (spx_receive_active)
		ThreadSwitch();

	if (spx_rx_semaphore != NULL)
		CloseLocalSemaphore(spx_rx_semaphore);

	if (spx_event_semaphore != NULL)
		CloseLocalSemaphore(spx_event_semaphore);

	while (spx_sent_active)
		ThreadSwitch();

	if (spx_event_semaphore != NULL)
		CloseLocalSemaphore(spx_sent_semaphore);

	if (spx_listen_semaphore != NULL)
		SignalLocalSemaphore(spx_listen_semaphore);

	if (main_ecb_pool != NULL)
		gds__free(main_ecb_pool);

	for (i = 0; i < 0x10000; i++) {
		rx = spx_rx_data[i];
		spx_rx_data[i] = NULL;
		if (rx != 0) {
			if (rx->semaphore) {
				CloseLocalSemaphore(rx->semaphore);
				rx->semaphore = 0;
			}
			gds__free(rx->buffer);
			gds__free(rx);
		}
	}
}


PORT SPXNET_listen(name, packet, status_vector, flag)
	 TEXT *name;
	 PACKET *packet;
	 STATUS *status_vector;
	 USHORT flag;
{
/**************************************
 *
 *      S P X N E T _ l i s t e n
 *
 **************************************
 *
 * Functional description
 * listen for a connection
 *
 **************************************/
	PORT port;
	TEXT *protocol, temp[128], *p, *q;
	WORD connid;
	rx_data *spx_data;
	int ccode;

	THREAD_EXIT;
	port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = gds_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = gds_arg_end;
	protocol = PROTOCOL_NAME;

	if (name) {
		strcpy(temp, name);
		for (p = temp; *p;)
			if (*p++ == '/') {
				p[-1] = 0;
				name = temp;
				protocol = p;
				break;
			}
	}
	if (name && *name) {
		if (port->port_connection)
			ALLR_free(port->port_connection);
		port->port_connection = REMOTE_make_string((UCHAR *) name);
	}
	else
		name = port->port_host->str_data;

	spx_listen_semaphore = OpenLocalSemaphore(0);
	if (spx_listen_semaphore == 0) {
		THREAD_ENTER;
		return (0);
	}

	spx_lecb.spx_ecb.fragCount = 1;
	spx_lecb.spx_ecb.fragList[0].fragAddress = (void *) &spx_lecb.header;
	spx_lecb.spx_ecb.fragList[0].fragSize =
		sizeof(SPX_HEADER) + MAX_SPX_DATA_SIZE;
	spx_lecb.spx_ecb.queueHead = (void *) &spx_listen_queue;
	spx_lecb.spx_ecb.semHandle = spx_listen_semaphore;

/*  Post that we are available to listen on the spx_socket port,
 *  and that if we are to be poked, that 'connid' is to be updated
 *  with the connection ID of the SPX connection that is sending
 *  a packet.
 */

	ccode = SpxListenForConnection((USHORT) spx_socket,
								   (SPX_ECB *) & spx_lecb.spx_ecb,
								   SPX_RETRY_COUNT, ENABLE_WATCHDOG, &connid);

/*  If we weren't able to listen, then a serious problem has occurred.  */
	if (ccode != ESUCCESS) {
		CloseLocalSemaphore(spx_listen_semaphore);
		THREAD_ENTER;
		return (0);
	}

/*  All this code is moved from below the WaitOnLocalSemaphore to above it
 *  so it doesn't cause a possible block. 
 */

/*  Allocate memory for our own connection context block  */
	spx_data = (rx_data *) gds__alloc(sizeof(rx_data));
/* FREE: disconnect or unload */
	if (spx_data == NULL) {		/* NOMEM: */
		CloseLocalSemaphore(spx_listen_semaphore);
		THREAD_ENTER;
		return (0);
	}

/*  Allocate some memory for our communication buffer.  */
	spx_data->buffer = (UCHAR *) gds__alloc(RX_BUFFER_SIZE);
/* FREE: disconnect or unload */
	if (spx_data->buffer == NULL) {	/* NOMEM: */
		CloseLocalSemaphore(spx_listen_semaphore);
		gds__free(spx_data);
		THREAD_ENTER;
		return (0);
	}

	spx_data->cur_length = 0;
	spx_data->max_len = RX_BUFFER_SIZE;
	spx_data->semaphore = 0;
	spx_data->rx_done = FALSE;
	spx_data->terminated = FALSE;
	spx_data->buffer_full = FALSE;
	spx_data->fragment_number = 0;

	WaitOnLocalSemaphore(spx_listen_semaphore);

	ccode = spx_check_ecb_error(&spx_lecb);
	if (ccode != ESUCCESS) {
		CloseLocalSemaphore(spx_listen_semaphore);
		spx_listen_semaphore = 0;

		gds__free(spx_data->buffer);
		gds__free(spx_data);
		THREAD_ENTER;
		return (0);
	}

	spx_rx_data[connid] = spx_data;
	port->port_handle = connid;

	CloseLocalSemaphore(spx_listen_semaphore);
	spx_listen_semaphore = 0;

	THREAD_ENTER;
	port->port_server_flags |= (SRVR_server | SRVR_multi_client);
	return (port);
}

/**************************************
 *
 *      d e q u e u e _ e c b
 *
 **************************************
 *
 * de-queue ecb from a queue
 *
 **************************************/
static void dequeue_ecb(SPX_ECB ** queue, ecb_blk * remove)
{
	if (*queue != NULL && remove != NULL) {
		if (*queue == (*queue)->prev && *queue == remove)
			*queue = NULL;
		else {
			ecb_blk *prev = (ecb_blk *) remove->spx_ecb.prev;

			if (prev->spx_ecb.next == NULL)
				*queue = (SPX_ECB *) remove->spx_ecb.next;
			else
				prev->spx_ecb.next = remove->spx_ecb.next;
			if (remove->spx_ecb.next == NULL)
				(*queue)->prev = (SPX_ECB *) prev;
			else
				remove->spx_ecb.next->prev = (SPX_ECB *) prev;
		}
	}

	return;
}

static accept_connection(port, cnct)
	 PORT port;
	 P_CNCT *cnct;
{
/**************************************
 *
 *      a c c e p t _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *      Accept an incoming request for connection.  This is purely a lower
 *      level handshaking function, and does not constitute the server
 *      response for protocol selection.
 *
 **************************************/
	TEXT name[64], password[64], *id, *end, *p;
	STR string;
	ULONG eff_gid, gids[128], eff_uid;
	int length, l, user_verification;

/* Default account to "guest" (in theory all packets contain a name) */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account and password, if given */

	id = (TEXT *) cnct->p_cnct_user_id.cstr_address;
	end = id + cnct->p_cnct_user_id.cstr_length;

	eff_uid = eff_gid = -1;
	user_verification = 0;
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

		case CNCT_group:
			p = (TEXT *) & eff_gid;
			if ((length = *id++) != NULL)
				do
					*p++ = *id++;
				while (--length);
			eff_gid = ntohl(eff_gid);
			break;

		case CNCT_user_verification:
			user_verification = 1;
			id++;
			break;

		default:
			id += *id + 1;
		}

	strncpy(name, port->port_user_name->str_data,
			port->port_user_name->str_length);
	p = &name[port->port_user_name->str_length];
	sprintf(p, ".%ld.%ld", eff_gid, eff_uid);
	ALLR_free(port->port_user_name);
	port->port_user_name = REMOTE_make_string(name);
	port->port_user_name->str_length = strlen(name);

	return TRUE;
}


static PORT alloc_port(parent)
	 PORT parent;
{
/**************************************
 *
 *      a l l o c _ p o r t
 *
 **************************************
 *
 * Functional description
 *      Allocate a port block, link it in to parent (if there is a parent),
 *      and initialize input and output XDR streams.
 *
 **************************************/
	PORT port;
	TEXT buffer[64];

	port = (PORT) ALLOCV(type_port, BUFFER_SIZE * 2);
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
		port->port_server_flags = parent->port_server_flags;
	}

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
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
 *      d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *      Break a remote connection.
 *
 **************************************/
	PORT parent, *ptr;
	async_port_handle *async_handle;

	if (port->port_async) {
		async_handle = (async_port_handle *) port->port_async->port_misc1;
		wait_on_local_semaphore_timeout(spx_event_semaphore, 10);
		if (async_handle != NULL) {
			SpxCloseSocket(async_handle->event_socket);
			if (async_handle->spx_lecb != NULL)
				gds__free(async_handle->spx_lecb);
			if (async_handle->ecb_pool != NULL)
				gds__free(async_handle->ecb_pool);
			gds__free(async_handle);
		}
		disconnect(port->port_async);
		port->port_async = NULL;
	}

/* If this is a sub-port, unlink it from it's parent */

	if ((parent = port->port_parent) != NULL) {
		for (ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next)
			if (*ptr == port) {
				*ptr = port->port_next;
				if (ptr == &parent->port_clients)
					parent->port_next = *ptr;
				break;
			}
	}

//gds__unregister_cleanup (exit_handler, port);
	cleanup_port(port);
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

	if (spx_rx_data[port->port_handle]) {
		if (spx_rx_data[port->port_handle]->buffer)
			gds__free(spx_rx_data[port->port_handle]->buffer);
		gds__free(spx_rx_data[port->port_handle]);
		spx_rx_data[port->port_handle] = NULL;
	}

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


static void spxnet_copy(from, to, length)
	 UCHAR *from, *to;
	 int length;
{
/**************************************
 *
 *      s p x n e t _ c o p y
 *
 **************************************
 *
 * Functional description
 *      Copy a number of bytes;
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while ((--length) != 0);
}


static void spxnet_zero(address, length)
	 UCHAR *address;
	 int length;
{
/**************************************
 *
 *      s p x n e t _  z e r o
 *
 **************************************
 *
 * Functional description
 *      Zero a block of memory.
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
 *      r e c e i v e
 *
 **************************************
 *
 * Functional description
 *      Receive a message from a port or clients of a port.  If the process
 *      is a server and a connection request comes in, generate a new port
 *      block for the client.
 *
 **************************************/

	if (!xdr_protocol(&main_port->port_receive, packet))
		/* Client connection lost.  Generate an opcode for cleanup */
		packet->p_operation = op_exit;

	return main_port;
}




static send_full(port, packet)
	 PORT port;
	 PACKET *packet;
{
/**************************************
 *
 *      s e n d _ f u l l
 *
 **************************************
 *
 * Functional description
 *      Send a packet across a port to another process.
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
 *      s e n d _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *      Send a packet across a port to another process.
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
 *      x d r s p x n e t _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *      Initialize an XDR stream for Apollo mailboxes.
 *
 **************************************/

	xdrs->x_public = (caddr_t) port;
	xdrs->x_base = xdrs->x_private = (UCHAR *) buffer;
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
 *      x d r s p x n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *      Write out the rest of a record.
 *
 **************************************/

	return spxnet_write(xdrs, flushnow);
}


static void spxnet_destroy()
{
/**************************************
 *
 *      s p x n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *      Destroy a stream.  A no-op.
 *
 **************************************/
}


static spxnet_error(port, operation, status)
	 PORT port;
	 TEXT *operation;
	 int status;
{
/**************************************
 *
 *      s p x n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *      An I/O error has occurred.  If a status vector is present,
 *      generate an error return.  In any case, return NULL, which
 *      is used to indicate and error.
 *
 **************************************/
	RDB rdb;
	TEXT **tp, msg[64];
	STATUS *status_vector;

	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	if ((rdb = port->port_context) != NULL)
		status_vector = rdb->rdb_status_vector;
	else
		status_vector = port->port_status_vector;

	if (status_vector) {
		*status_vector++ = gds_arg_gds;
		*status_vector++ = gds__io_error;
		*status_vector++ = gds_arg_string;
		*status_vector++ = (STATUS) operation;
		*status_vector++ = gds_arg_string;
		tp = (TEXT **) status_vector;
		*tp++ = port->port_connection->str_data;
		status_vector = (STATUS *) tp;
		if (status) {
			*status_vector++ = SYS_ERR;
			*status_vector++ = (STATUS) status;
		}
		*status_vector++ = 0;
	}

	if (status) {
		sprintf(msg, "spxnet/spxnet_error: %s errno = %d", operation, status);
		gds__log(msg);
	}

	return (int) NULL;
}


static bool_t spxnet_getbytes(xdrs, buff, count)
	 XDR *xdrs;
	 UCHAR *buff;
	 u_int count;
{
/**************************************
 *
 *      s p x n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *      Get a bunch of bytes from a memory stream if it fits.
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


static bool_t spxnet_getULONG(xdrs, lp)
	 XDR *xdrs;
	 ULONG *lp;
{
/**************************************
 *
 *      s p x n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *      Fetch a ULONGword into a memory stream if it fits.
 *
 **************************************/
	ULONG l;

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
 *      s p x n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *      Get the current position (which is also current length) from stream.
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
 *      s p x n e t _  i n l i n e
 *
 **************************************
 *
 * Functional description
 *      Return a pointer to somewhere in the buffer.
 *
 **************************************/

	if (bytecount > xdrs->x_handy)
		return FALSE;

	return xdrs->x_base + bytecount;
}


static bool_t spxnet_putbytes(xdrs, buff, count)
	 XDR *xdrs;
	 UCHAR *buff;
	 u_int count;
{
/**************************************
 *
 *      s p x n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *      Put a bunch of bytes to a memory stream if it fits.
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


static bool_t spxnet_putULONG(xdrs, lp)
	 XDR *xdrs;
	 ULONG *lp;
{
/**************************************
 *
 *      s p x n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *      Fetch a ULONGword into a memory stream if it fits.
 *
 **************************************/
	ULONG l;

	l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs, AOF32L(l), 4);
}


static bool_t spxnet_read(xdrs)
	 XDR *xdrs;
{
/**************************************
 *
 *      s p x n e t _ r e a d
 *
 **************************************
 *
 * Functional description
 *      Read a buffer full of data.  If we receive a bad packet,
 *      send the moral equivalent of a NAK and retry.  ACK all
 *      partial packets.  Don't ACK the last packet -- the next
 *      message sent will handle this.
 *
 **************************************/
	PORT port;
	SSHORT length;
	UCHAR *p, *end;

	port = (PORT) xdrs->x_public;
	p = xdrs->x_base;
	end = p + BUFFER_SIZE;

/* If buffer is not completely empty, slide down what what's left */

	length = xdrs->x_handy;

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
	xdrs->x_handy = (int) ((UCHAR *) p - xdrs->x_base);
	xdrs->x_private = xdrs->x_base;
	return TRUE;
}


static bool_t spxnet_setpostn(xdrs, bytecount)
	 XDR *xdrs;
	 u_int bytecount;
{
/**************************************
 *
 *      s p x n e t _ s e t p o s t n
 *
 **************************************
 *
 * Functional description
 *      Set the current position (which is also current length) from stream.
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
 *      s p x n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *      Write a buffer fulll of data.  If the end_flag isn't set, indicate
 *      that the buffer is a fragment, and reset the XDR for another buffer
 *      load.
 *
 **************************************/
	UCHAR *p;
	PORT port;
	SSHORT l, length;

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


#ifdef OBSOLETE
static void exit_handler(main_port)
	 PORT main_port;
{
/**************************************
 *
 *      e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *      Shutdown all active connections
 *      to allow restart.
 *
 **************************************/
	PORT port;

	for (port = main_port; port; port = port->port_next)
		spx_terminate_connection(port);
}
#endif /* OBSOLETE */


bool_t packet_send(port, buffer, buffer_length)
	 PORT port;
	 UCHAR *buffer;
	 SSHORT buffer_length;
{
/**************************************
 *
 *      p a c k e t _ s e n d
 *
 **************************************
 *
 * Functional description
 *      Send some data on it's way.  
 *
 **************************************/
	UCHAR *data;
	LONG n, length;
	USHORT status;
	USHORT conn_id, len;
	ecb_blk *send_ecb;
	int ccode = 0;
	int i, j;
	rx_data *rxdata;
	int done = FALSE;
	int index;

	index = 0;
	while (!done) {
		if (buffer_length <= MAX_SPX_DATA_SIZE) {
			done = TRUE;
			length = buffer_length;
		}
		else {
			buffer_length -= MAX_SPX_DATA_SIZE;
			length = MAX_SPX_DATA_SIZE;
		}

		send_ecb = (ecb_blk *) gds__alloc(sizeof(SPX_ECB) + length +
										  sizeof(SPX_HEADER) + sizeof(ULONG));
		/* FREE: in spx_sent_process */
		if (send_ecb == NULL) {	/* NOMEM: */
			return (FALSE);
		}

		rxdata = spx_rx_data[port->port_handle];
		memcpy(&(send_ecb->header.destNet), rxdata->client_address, 12);
		memcpy(&(send_ecb->header.sourceNet), my_network_address, 12);
		len = length + sizeof(SPX_HEADER) + sizeof(ULONG);
		send_ecb->spx_ecb.fragCount = 1;
		send_ecb->spx_ecb.fragList[0].fragAddress = &send_ecb->header;
		send_ecb->spx_ecb.fragList[0].fragSize = length +
			sizeof(SPX_HEADER) + sizeof(ULONG);
		send_ecb->spx_ecb.queueHead = &spx_sent_queue;
		send_ecb->spx_ecb.semHandle = spx_sent_semaphore;
		send_ecb->header.packetLen = IntSwap(len);
		send_ecb->header.dataStreamType = 0;
		if (done)
			send_ecb->header.connectionCtl |= 0x10;
		else
			send_ecb->header.connectionCtl = 0;
		*((ULONG *) send_ecb->data) = rxdata->fragment_number;
		rxdata->fragment_number++;
		memcpy(send_ecb->data + sizeof(ULONG), &(buffer[index]), length);

		ccode = SpxSendSequencedPacket((USHORT) port->port_handle,
									   &send_ecb->spx_ecb);

		if (ccode != 0) {
			return (0);
		}
		index += length;
	}

	port->port_flags &= ~PORT_pend_ack;
	return TRUE;
}


void spx_sent_process()
/**************************************
 *
 * s p x _ s e n t _ p r o c e s s 
 *
 **************************************
 *
 * Functional description
 * Send process
 *
 **************************************/
{
	ecb_blk *sent;
	rx_data *rx;
	int ccode;

	spx_sent_active = TRUE;

	while (!spx_exit) {
		WaitOnLocalSemaphore(spx_sent_semaphore);
		if (!spx_exit) {
			if (spx_sent_queue != NULL) {
				sent = (ecb_blk *) spx_sent_queue->prev;
				dequeue_ecb(&spx_sent_queue, sent);
				ccode = spx_check_ecb_error(sent);
				if (ccode != ESUCCESS) {
					if (sent->spx_ecb.status == STS_SPX_TERMINATED_POORLY)
						rx = spx_rx_data[sent->spx_ecb.protocolWorkspace];
					else
						rx = spx_rx_data[sent->header.destConnectID];
					if (rx != 0) {
						rx->terminated = TRUE;
						if (rx->semaphore != 0)
							SignalLocalSemaphore(rx->semaphore);
					}
				}
				else
					gds__free(sent);
			}
		}
	}
	spx_sent_active = FALSE;
}


ecb_blk *spx_post_receive_ecbs(USHORT socket, SPX_ECB ** queue_head,
							   ULONG semaphore, USHORT pool_size)
/**************************************
 *
 * s p x _ p o s t _ r e c e i v e _ e c b s
 *
 **************************************
 *
 * Functional description
 * post Rx ecb pool
 *
 **************************************/
{
	int i;
	int ccode;
	ecb_blk *local_ecb_pool;

/*  Build a pool of pool_size spx_ecb's  */
	local_ecb_pool = (ecb_blk *) gds__alloc(sizeof(ecb_blk) * pool_size);
/* FREE: disconnect or unload */

	if (local_ecb_pool == NULL)	/* NOMEM: */
		return ((ecb_blk *) NULL);

	for (i = 0; i < pool_size; i++) {
		/*  *sigh*  queue_head is actually a pointer to a global variable which
		 *  will receive a pointer to the head (such as there is) of a circular
		 *  queue of ECB's which have been 'woken' in response to a packet coming
		 *  across a particular socket.
		 */
		local_ecb_pool[i].spx_ecb.queueHead = queue_head;
		/*  Supposedly a semaphore will also be signalled when this occurs, and
		 *  in the case where this is called from SPX_init, this is true, but in
		 *  the case where this is called from aux_request, the spx_event_semaphore
		 *  is *NEVER* listened for.  This causes me some consternation, but the
		 *  port is supposedly used for transmission to a client only.
		 */
		local_ecb_pool[i].spx_ecb.semHandle = semaphore;
		/*  So, how many fragments can we handle?  */
		local_ecb_pool[i].spx_ecb.fragCount = 1;
		/*  And the size of each of those fragments?  */
		local_ecb_pool[i].spx_ecb.fragList[0].fragSize = 576;
		/*  Oh, by the way, where should we store those fragments?  */
		local_ecb_pool[i].spx_ecb.fragList[0].fragAddress =
			&local_ecb_pool[i].header;
		/*  Okay, now start listening...  */
		ccode =
			SpxListenForSequencedPacket(socket, &local_ecb_pool[i].spx_ecb);
		if (ccode != ESUCCESS) {
			/*  Well, for right now, if we're just doing the first one, we can     */
			/*  free the local_ecb_pool safely.  If it's anything beyond the first */
			/*  then we have successfully established a listen for a few ECB's, and */
			/*  we don't want to disturb that.  Hopefully the rest of the system   */
			/*  will be able to run with only, say, four ECB's for listening to    */
			/*  recieves.                                                          */
			if (i == 0) {
				/*  It's the first one, so we haven't done any listens.  */
				gds__free(local_ecb_pool);
				return ((ecb_blk *) NULL);
			}
			else {
				/*  It's NOT the first one, and we'll try to continue operating. */
				return ((ecb_blk *) local_ecb_pool);
			}					/*  Error condition on SPXListenForSequencedPacket  */
		}
	}
	return ((ecb_blk *) local_ecb_pool);
}

/**************************************
 *
 * s p x _ r e c e i v e _ p r o c e s s 
 *
 **************************************
 *
 * Functional description
 * process rx ecbs
 *
 **************************************/
void spx_receive_process()
{
	ecb_blk *curr_ecb;
	ecb_blk *rx_ecb;
	BOOLEAN done;

/* Set flag for use during shutdown. */
	spx_receive_active = TRUE;

/* Continue as long as NLM has not been shutdown. */
	while (!spx_exit) {
		/* Wait until ECB is received into the queue. */
		WaitOnLocalSemaphore(spx_rx_semaphore);

		if (!spx_exit && spx_receive_queue != NULL) {
			/* Start at first received ECB which is at end of list. */
			curr_ecb = (ecb_blk *) spx_receive_queue->prev;

			/* Continue while queue is not empty and not at most recent ECB. */
			done = FALSE;
			while (!done && spx_receive_queue != NULL) {
				/* Save current ECB for processing. */
				rx_ecb = curr_ecb;

				/* Almost done when at most recent received ECB in list. */
				if (curr_ecb == (ecb_blk *) spx_receive_queue)
					done = TRUE;

				/* Move pointer to next ECB. 
				 * If only one in list, prev points to itself.
				 */
				curr_ecb = (ecb_blk *) curr_ecb->spx_ecb.prev;

				/* If ECB can be processed, then remove and post listen again. */
				if (process_rx(rx_ecb)) {
					dequeue_ecb(&spx_receive_queue, rx_ecb);
					assert(rx_ecb->spx_ecb.queueHead == &spx_receive_queue);
					assert(rx_ecb->spx_ecb.fragList[0].fragSize == 576);
					assert(rx_ecb->spx_ecb.semHandle == spx_rx_semaphore);
					SpxListenForSequencedPacket(spx_socket, &rx_ecb->spx_ecb);
				}
			}
		}
	}

/* Reset flag for use during shutdown. */
	spx_receive_active = FALSE;
}

/**************************************
 *
 * p r o c e s s _ r x 
 *
 **************************************
 *
 * process receive ecb, return whether or not to remove ECB from list
 *
 **************************************/
static BOOLEAN process_rx(ecb_blk * rx_ecb)
{
	WORD length;
	rx_data *data;
	int ccode;
	int i;

/* Check success code in ECB. */
	ccode = spx_check_ecb_error(rx_ecb);
	if (ccode != ESUCCESS) {
		if (rx_ecb->spx_ecb.status == STS_SPX_TERMINATED_POORLY)
			data = spx_rx_data[rx_ecb->spx_ecb.protocolWorkspace];
		else
			data = spx_rx_data[rx_ecb->header.destConnectID];

		/* Connection has terminated if entry is NULL. */
		if (data != 0) {
			data->terminated = TRUE;
			if (data->semaphore != 0)
				SignalLocalSemaphore(data->semaphore);
		}
		return TRUE;
	}

/* Get connection data. */
	data = spx_rx_data[rx_ecb->header.destConnectID];

/* Calculate length of data in ECB. */
	length = IntSwap(rx_ecb->header.packetLen) - sizeof(SPX_HEADER);

/* Set flag and return FALSE if not enough space in connection buffer. */
	if ((data->cur_length + length) > data->max_len) {
		data->buffer_full = TRUE;

		return FALSE;
	}

/* Move ECB data into connection buffer. */
	memcpy(data->buffer + data->cur_length, rx_ecb->data, length);
	memcpy(data->client_address, &(rx_ecb->header.sourceNet), 12);
	data->cur_length += length;

/* Check for end of message bit to signal done. */
	if (rx_ecb->header.connectionCtl & 0x10) {
		data->rx_done = TRUE;
		if (data->semaphore != 0)
			SignalLocalSemaphore(data->semaphore);
	}

	return TRUE;
}


packet_receive(port, buffer, buffer_length, length)
	 PORT port;
	 UCHAR *buffer;
	 SSHORT buffer_length, *length;
{
/**************************************
 *
 *      p a c k e t _ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *      Receive a packet and pass on it's goodness. 
 *
 **************************************/
	rx_data *data;
	int i;

	data = spx_rx_data[port->port_handle];

	if (data->terminated) {
		spx_rx_data[port->port_handle] = 0;
		if (data->buffer)
			gds__free(data->buffer);
		gds__free(data);
		return (0);
	}

	if (!data->rx_done) {
		data->semaphore = OpenLocalSemaphore(0);
		THREAD_EXIT;
		WaitOnLocalSemaphore(data->semaphore);
		THREAD_ENTER;
		data = spx_rx_data[port->port_handle];
		if (data == NULL)
			return (0);
		CloseLocalSemaphore(data->semaphore);
		if (data->terminated) {
			spx_rx_data[port->port_handle] = 0;
			if (data->buffer)
				gds__free(data->buffer);
			gds__free(data);
			return (0);
		}
	}

	memcpy(buffer, data->buffer, data->cur_length);
	(*length) = data->cur_length;
	data->cur_length = 0;
	data->semaphore = 0;
	data->rx_done = FALSE;
	if (data->buffer_full) {
		data->buffer_full = FALSE;
		SignalLocalSemaphore(spx_rx_semaphore);
	}

	return TRUE;
}

int spx_check_ecb_error(ecb_blk * ecb)
/**************************************
 *
 * s p x _ c h e c k _ e c b _ e r r o r
 *
 **************************************
 *
 * Functional description
 *      Check packets termination codes
 *
 **************************************/
{
	if (ecb->header.dataStreamType == 0xFE ||
		ecb->spx_ecb.status == STS_SPX_TERMINATED_POORLY ||
		ecb->spx_ecb.status == STS_SPX_CONNECTION_TERMINATED ||
		ecb->spx_ecb.status != 0) {
		return (EFAILURE);
	}

	return (ESUCCESS);
}


#ifdef OBSOLETE
void spx_terminate_connection(PORT port)
/**************************************
 *
 * s p x _ t e r m i n a t e _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *      Terminate connections
 *
 **************************************/
{
	ecb_blk *tecb;

	tecb = (ecb_blk *) gds__alloc(sizeof(ecb_blk));
	/* FREE: apparently never freed? */
	/* NOTE: Why is this allocated instead of stack based? */
	/*    This function is never called.  (Specifically, because
	 *    exit_handler locally is never added to the cleanup handlers
	 *    list.  That's why this function is #IFDEF'ed out.)
	 *    --  Morgan Schweers, November 02, 1994
	 */
	if (!tecb)					/* NOMEM: */
		return;					/* Not the best handler */

	tecb->spx_ecb.fragCount = 1;
	tecb->spx_ecb.fragList[0].fragAddress = &tecb->header;
	tecb->spx_ecb.fragList[0].fragSize
		= sizeof(SPX_HEADER) + MAX_SPX_DATA_SIZE;
	tecb->spx_ecb.queueHead = &spx_sent_queue;
	tecb->spx_ecb.semHandle = spx_sent_semaphore;
	SpxTerminateConnection(port->port_handle, &tecb->spx_ecb);
}
#endif /* OBSOLETE */


static PORT aux_request( PORT port, PACKET * packet)
{
/**************************************
 *
 *      a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *      A remote interface has requested the server prepare an auxiliary 
 *      connection; the server calls aux_request to set up the connection.
 *      Specifically, we are *ONLY* called from *ONE* place, and that is
 *      in SERVER.C at *THEIR* aux_request, which is only called from
 *      process_packet, by the remote command: op_connect_request.
 *
 **************************************/
	PORT new_port;
	P_RESP *response;
	USHORT event_socket = 0;
	UCHAR event_address[12];
	ULONG event_listen_semaphore;
	ecb_blk *spx_event_lecb;
	async_port_handle *async_handle;

	spx_event_lecb =
		(ecb_blk *) gds__alloc(sizeof(SPX_ECB) + sizeof(SPX_HEADER));
/* FREE: disconnect or unload */
	if (!spx_event_lecb)		/* NOMEM: */
		return NULL;
	async_handle =
		(async_port_handle *) gds__alloc(sizeof(async_port_handle));
/* FREE: disconnect or unload */
	if (!async_handle) {		/* NOMEM: */
		gds__free(spx_event_lecb);
		return NULL;
	}

/*  Open a new socket to recieve events on.  */
	SpxOpenSocket(&event_socket);
/*  Set our address  */
	memcpy(event_address, my_network_address, 12);
/*  Set the socket portion of our address  */
	memcpy(&(event_address[10]), &event_socket, 2);

/*  Get a port for our async/event work  */
	new_port = alloc_port(port->port_parent);

/*  Place the new port in our current port's async structure  */
	port->port_async = new_port;

/*  Set the flags to the same as the port on which we received the request,
 *  except for setting no out-of-band data
 */
	new_port->port_flags |= port->port_flags & PORT_no_oob;

/*  Now we use the 'misc1' field of the port structure to hold the odd handle */
	new_port->port_misc1 = (int) async_handle;

/*  Okay, now we need a semaphore to listen for these messages.  */
	event_listen_semaphore = OpenLocalSemaphore(0);

/*  Anyhow, let's make the system recognize that we have a socket (opened
 *  above), a queue, and a semaphore that we can get information on.
 */
	async_handle->ecb_pool =
		spx_post_receive_ecbs(event_socket, &spx_event_queue,
							  spx_event_semaphore, 1);

/*  Errors:  No memory, or unable to post ECB's */
	if (async_handle->ecb_pool == NULL) {
		async_handle->spx_lecb = spx_event_lecb;
		disconnect(new_port);
		CloseLocalSemaphore(event_listen_semaphore);
	}

/*  How many fragments can we handle?  */
	spx_event_lecb->spx_ecb.fragCount = 1;
/*  How big is each one?  */
	spx_event_lecb->spx_ecb.fragList[0].fragSize =
		sizeof(SPX_HEADER) + MAX_SPX_DATA_SIZE;
/*  Where do we store them?  */
	spx_event_lecb->spx_ecb.fragList[0].fragAddress =
		(void *) &spx_event_lecb->header;
/*  What queue should be 'notified' if a situation occurs on this socket?  */
	spx_event_lecb->spx_ecb.queueHead = &spx_event_queue;

/*  What semaphore should be signalled to tell us to look at this queue?  */
	spx_event_lecb->spx_ecb.semHandle = event_listen_semaphore;

/*  So, if the port needs to know what its listen_semaphore for events is... */
	async_handle->event_listen_semaphore = event_listen_semaphore;
/*  The ECB for listening on the event_socket is...  */
	async_handle->spx_lecb = spx_event_lecb;
/*  Last But Not Least, what's the socket we're supposed to listen on? */
	async_handle->event_socket = event_socket;

/*  Now, listen for an event, on the event_socket, with status and data
 *  going to the spx_event_lecb 'spx_ecb' substructure, retrying a few
 *  times, and making sure that if the connection goes quiet that we
 *  know to disconnect, and if we *GET* a connection, put an identifying
 *  number (hopefully less than 0x10000) into the port structure, under
 *  the async_handle's connection id.  *WHEW*.
 */
	SpxListenForConnection(event_socket,
						   (SPX_ECB *) (&spx_event_lecb->spx_ecb),
						   SPX_RETRY_COUNT, ENABLE_WATCHDOG,
						   &(async_handle->connid));

/* return with new port and address */
	response = &packet->p_resp;
/*  NOTE:  If the function calling this w/ packet doesn't set up enough
 *  space, we're hosed.  Thank goodness that doesn't happen.  Yet.
 */
	response->p_resp_data.cstr_length = 12;
/*  Oy.  And why couldn't we have used memcpy?  Sheesh.  */
/*  Basically, set the p_resp->p_resp_data.cstr_address field of the
 *  packet to be sent to the local address and port to communicate with.
 */
	spxnet_copy(event_address, response->p_resp_data.cstr_address,
				response->p_resp_data.cstr_length);

/*  Okay, now we've set up the port, done a listen, added it to the packet,
 *  etc, etc, etc.
 */
	return new_port;
}


static PORT aux_connect( PORT port, PACKET * packet, void (*ast) ())
{
/**************************************
 *
 *      a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	WORD connid;
	rx_data *spx_data;
	SPX_SESSION spx_session;
	async_port_handle *async_handle;
	int i;

/*  Okay, we're being called 'cause we want an auxiliary connection,
 *  the request has been processed, the local-side socket and address
 *  has been passed back, and now we should wait for a connection on
 *  that socket.  The semaphore associated with an ECB which is waiting
 *  on a listen-status-change on that socket will be signalled when the
 *  client decides to try to connect with us.  *whew*.
 */
	async_handle = (async_port_handle *) port->port_misc1;

	WaitOnLocalSemaphore(async_handle->event_listen_semaphore);
	CloseLocalSemaphore(async_handle->event_listen_semaphore);
/*  We don't want to see this semaphore ever again.  It's the listen-only
 *  semaphore.
 */
	async_handle->event_listen_semaphore = 0;

/*  NOW what we've recieved a connection ID, that's the *REAL* port_handle.
 *  (The connection ID was placed in async_handle->connid by passing the
 *  address of async_handle->connid in SPXListenForConnection in aux_request.
 */

	port->port_handle = async_handle->connid;
/*  Now that we've established this port, set a note that this is an async
 *  port.
 */
	port->port_flags |= PORT_async;

/*  Allocate space for the connection's data structure, aka rx_data.  */
	spx_data = (rx_data *) gds__alloc(sizeof(rx_data));
/* FREE: disconnect or unload */
	if (!spx_data)				/* NOMEM: */
		return NULL;

/*  Now allocate space for the buffer portion of rx_data, ie., packet data */
	spx_data->buffer = (UCHAR *) gds__alloc(RX_BUFFER_SIZE);
/* FREE: disconnect or unload */
	if (!spx_data->buffer) {	/* NOMEM: */
		gds__free(spx_data);
		return NULL;
	}

/*  Here's where that fun friendly f**ked up global comes in again...  */
/*  We shouldn't use async_handle anymore, it should be port->port_handle */
/*  OLD STATEMENT: spx_rx_data[async_handle->connid] = spx_data;  */
	spx_rx_data[port->port_handle] = spx_data;
	spx_data->cur_length = 0;
	spx_data->max_len = RX_BUFFER_SIZE;
	spx_data->semaphore = 0;
	spx_data->rx_done = FALSE;
	spx_data->terminated = FALSE;
	spx_data->fragment_number = 0;

/*  Interesting...  We presume, in a large way.  */
/*OLD STATEMENT: SpxGetConnectionStatus(async_handle->connid, &spx_session); */
	SpxGetConnectionStatus(port->port_handle, &spx_session);
	memcpy(&(spx_data->client_address), &(spx_session.sRemoteNet), 12);

	return (port);
}


static BOOLEAN wait_on_local_semaphore_timeout(
											   ULONG semaphore,
											   USHORT timeout)
{
/**************************************
 *
 *      w a i t _ o n _ l o c a l _ s e m a p h o r e _ t i m e o u t
 *
 **************************************
 *
 * Functional description
 *    Wait either for one semaphore to be signal or a timeout to occur.
 *    Return TRUE if semaphore was signal before timeout else FALSE.
 *    The parameter timeout is in seconds.
 *    Note that this only clears one value off the semaphore queue.
 *
 **************************************/
	USHORT count = timeout * 10;

	if (!spx_exit) {
		while (!spx_exit && ExamineLocalSemaphore(semaphore) == 0
			   && count > 0) {
			delay(100);
			count--;
		}
		if (count != 0) {
			if (!spx_exit)
				WaitOnLocalSemaphore(semaphore);
			else
				return (FALSE);
		}
	}
	return (count != 0);
}
