/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		inet.cpp
 *	DESCRIPTION:	TCP/UCP/IP Communications module.
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
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "EPSON" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "XENIX" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "NCR3000" port
 *
 * 2002-02-23 Sean Leyne - Code Cleanup, removed old M88K and NCR3000 port
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2002.10.30 Sean Leyne - Code Cleanup, removed obsolete "SUN3_3" port
 *
 */

#include "firebird.h"
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include "../jrd/file_params.h"
#include <stdarg.h>

#include "../jrd/jrd_time.h"

#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_GRP_H
#include <grp.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h> /* for socket() */
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef SUPERSERVER
#ifdef	WIN_NT
#define FD_SETSIZE 1024
#endif

#ifdef SET_TCP_NO_DELAY
#include <netinet/tcp.h>
#endif

#endif /* SUPERSERVER */

#if !(defined VMS || defined WIN_NT)
#include <netdb.h>
#include <arpa/inet.h>
/* EKU: SINIX-Z does not define INADDR_NONE */
#ifndef INADDR_NONE
#define INADDR_NONE (unsigned long)-1
#endif
#endif // !(defined VMS || defined WIN_NT)

#ifdef DARWIN
extern "C" int innetgr(const char*, const char*, const char*, const char*);
#endif

const int INET_RETRY_CALL	= 5;

#include "../remote/remote.h"
#include "../jrd/ibase.h"
#include "../jrd/iberr.h"
#include "../jrd/thd.h"
#include "../remote/inet_proto.h"
#include "../remote/proto_proto.h"
#include "../remote/remot_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#ifndef REQUESTER
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/sch_proto.h"
#endif /* REQUESTER */

#include "../jrd/thread_proto.h"
#include "../common/config/config.h"

#if (defined hpux || defined SCO_UNIX)
extern int h_errno;
#endif

#ifdef VMS
#include <perror.h>
#include <socket.h>
#define NO_FORK
const USHORT MAX_PTYPE	= ptype_batch_send;
const char* PROXY_FILE	= "[sysmgr]gds_proxy.dat";
const char* HOSTS_FILE = "";
const char* GDS_HOSTS_FILE = "";
#error "vms implementation must be completed"
#else // VMS
const char* PROXY_FILE	= "/etc/gds_proxy";
const char* HOSTS_FILE	= "/etc/hosts.equiv";
#ifdef MULTI_THREAD
const USHORT MAX_PTYPE	= ptype_batch_send;
#else
const USHORT MAX_PTYPE	= ptype_out_of_band;
#endif
#ifdef SMALL_FILE_NAMES
const char* GDS_HOSTS_FILE	= "/etc/gdshosts.eqv";
#else
const char* GDS_HOSTS_FILE	= "/etc/gds_hosts.equiv";
#endif
#endif // VMS

#ifdef WIN_NT

#include <fcntl.h>
#include <process.h>
#include <signal.h>
#include "../utilities/install/install_nt.h"
#define SOCLOSE		closesocket
#define INET_RETRY_ERRNO	WSAEINPROGRESS
#define INET_ADDR_IN_USE	WSAEADDRINUSE
#define sleep(seconds)  Sleep ((seconds) * 1000)

#else // WIN_NT

#ifndef SOCKET
#define SOCKET  int
#endif
#ifndef SOCLOSE
#define SOCLOSE	close
#endif
#ifndef INET_ADDR_IN_USE
#define INET_ADDR_IN_USE EADDRINUSE
#endif
#ifndef INET_RETRY_ERRNO
#define INET_RETRY_ERRNO TRY_AGAIN
#endif

#endif // WIN_NT

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  -1
#endif

#ifndef SIGURG
#define SIGURG	SIGINT
#endif

#ifndef ENOBUFS
#define ENOBUFS	0
#endif

SLONG INET_remote_buffer;
SLONG INET_max_data;
static bool first_time = true;

//
//#define DEBUG	1
//

#ifndef REQUESTER
#ifdef DEBUG
#ifdef HAVE_SYS_TIMEB_H
# include <sys/timeb.h>
#endif
const int TRACE_packets		= 1 << 0;	/* bit 0 */
const int TRACE_operations	= 1 << 1;	/* bit 1 */
const int TRACE_summary		= 1 << 2;	/* bit 2 */

static int INET_trace = TRACE_summary | TRACE_packets | TRACE_operations;
static time_t INET_start_time = 0;
SLONG INET_force_error = -1;	/* simulate a network error */
static ULONG INET_count_send = 0;
static ULONG INET_count_recv = 0;
static ULONG INET_bytes_send = 0;
static ULONG INET_bytes_recv = 0;

static ULONG inet_debug_timer(void)
{
/**************************************
 *
 *	i n e t _ d e b u g _ t i m e r
 *
 **************************************
 *
 * Functional description
 *	Utility function used in DEBUG mode only to put a timestamp
 *	since start of connection on each debug output message.
 *
 *	This has been implemented and tested on SOLARIS, and may
 *	need tweeking on any other platform where DEBUG is needed.
 *
 **************************************/
#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	GETTIMEOFDAY(&tv);
	return (tv.tv_sec * 1000 + tv.tv_usec - INET_start_time);
#else
	struct timeb now;
	ftime(&now);
	return (now.time * 1000 + now.millitm - INET_start_time);
#endif /* HAVE_GETTIMEOFDAY */
}
#endif /* DEBUG */
#endif /* !REQUESTER */

const SLONG MAX_DATA_LW		= 1448;		/* Low  Water mark */
const SLONG MAX_DATA_HW		= 32768;	/* High Water mark */
const SLONG DEF_MAX_DATA	= 8192;

const int MAX_SEQUENCE		= 256;

const int MAXHOSTLEN		= 64;

const int SELECT_TIMEOUT	= 60;		/* Dispatch thread select timeout (sec) */

typedef struct slct
{
	int		slct_width;
	int		slct_count;
	SLONG	slct_time;
	fd_set	slct_fdset;
} SLCT;

static int		accept_connection(rem_port*, P_CNCT *);
#ifdef HAVE_SETITIMER
static void		alarm_handler(int);
#endif
static rem_port*		alloc_port(rem_port*);
static rem_port*		aux_connect(rem_port*, PACKET*, t_event_ast);
static rem_port*		aux_request(rem_port*, PACKET*);
#if !defined(WIN_NT)
#ifndef VMS
static int		check_host(rem_port*, TEXT*, const TEXT*, const struct passwd*);
#else
static int		check_host(rem_port*, TEXT*, const TEXT*);
#endif // VMS
static bool		check_proxy(rem_port*, TEXT *, TEXT *);
#endif // WIN_NT
static void		cleanup_port(rem_port*);
static void		disconnect(rem_port*);
static void		exit_handler(void *);

#ifdef NO_FORK
static int fork(void);
#endif

#ifdef WIN_NT
static int fork(SOCKET, USHORT);
#endif

static in_addr get_bind_address();
static in_addr get_host_address(const TEXT *);

static void copy_p_cnct_repeat_array(	p_cnct::p_cnct_repeat*			pDest,
										const p_cnct::p_cnct_repeat*	pSource,
										size_t							nEntries);

static void		inet_copy(const SCHAR*, SCHAR*, int);
static int		inet_destroy(XDR *);
static void		inet_gen_error(rem_port*, ISC_STATUS, ...);
static bool_t	inet_getbytes(XDR *, SCHAR *, u_int);
static bool_t	inet_getlong(XDR *, SLONG *);
static u_int	inet_getpostn(XDR *);
#if !(defined WIN_NT)
static void		inet_handler(void* _port);
#endif
static caddr_t	inet_inline(XDR *, u_int);
static void		inet_error(rem_port*, const TEXT*, ISC_STATUS, int);
static bool_t	inet_putlong(XDR*, const SLONG*);
static bool_t	inet_putbytes(XDR*, const SCHAR*, u_int);
static bool_t	inet_read(XDR *);
static bool_t	inet_setpostn(XDR *, u_int);
static rem_port*		inet_try_connect(	PACKET*,
									RDB,
									Firebird::PathName&,
									const TEXT*,
									ISC_STATUS*,
									const SCHAR*,
									SSHORT);
static bool_t	inet_write(XDR *, int);
static void		inet_zero(SCHAR *, int);
#if !(defined WIN_NT)
static int		parse_hosts(const TEXT*, const TEXT*, const TEXT*);
static int		parse_line(const TEXT*, const TEXT*, const TEXT*, const TEXT*);
#endif

#ifdef DEBUG
static void packet_print(const TEXT*, const UCHAR*, int, int);
#endif

static bool_t	packet_receive(rem_port*, UCHAR*, SSHORT, SSHORT*);
static bool_t	packet_send(rem_port*, const SCHAR*, SSHORT);
static rem_port*		receive(rem_port*, PACKET *);
static rem_port*		select_accept(rem_port*);

static rem_port*		select_port(rem_port*, SLCT *);
static int		select_wait(rem_port*, SLCT *);
static int		send_full(rem_port*, PACKET *);
static int		send_partial(rem_port*, PACKET *);

#ifdef  SUPERSERVER
static void		unhook_disconnected_ports(rem_port*);
#endif

static void		unhook_port(rem_port*, rem_port*);
static int		xdrinet_create(XDR *, rem_port*, UCHAR *, USHORT, enum xdr_op);
static bool_t	xdrinet_endofrecord(XDR *, bool_t);


static XDR::xdr_ops inet_ops =
{
	inet_getlong,
	inet_putlong,
	inet_getbytes,
	inet_putbytes,
	inet_getpostn,
	inet_setpostn,
	inet_inline,
	inet_destroy
};


#define MAXCLIENTS	NOFILE - 10

/* Select uses bit masks of file descriptors in longs. */

#ifndef NBBY
#define	NBBY	8
#endif

#ifndef NFDBITS
#if !defined(WIN_NT)
#define NFDBITS		(sizeof(SLONG) * NBBY)

#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	inet_zero((SCHAR *)(p), sizeof(*(p)))
#endif
#endif


static SLCT INET_select = { 0, 0, 0 };
static int INET_max_clients;
#ifdef WIN_NT
static bool INET_initialized = false;
static WSADATA INET_wsadata;
static TEXT INET_command_line[MAXPATHLEN + 32], *INET_p;
#endif


#ifdef	WIN_NT
#define	INTERRUPT_ERROR(x)	(SYSCALL_INTERRUPTED(x) || (x) == WSAEINTR)
#else
#define	INTERRUPT_ERROR(x)	(SYSCALL_INTERRUPTED(x))
#endif

#ifdef  SUPERSERVER

static MUTX_T	port_mutex;
static bool		port_mutex_inited = false;

#define DEFER_PORT_CLEANUP

inline void START_PORT_CRITICAL() {
	if (!port_mutex_inited) {
		port_mutex_inited = true;
	}
	THREAD_EXIT();
	THD_mutex_lock (&port_mutex);
	THREAD_ENTER();
}

inline void STOP_PORT_CRITICAL() {
	THREAD_EXIT();
	THD_mutex_unlock (&port_mutex);
	THREAD_ENTER();
}

#else
inline void START_PORT_CRITICAL() {
}
inline void STOP_PORT_CRITICAL() {
}
#endif


rem_port* INET_analyze(Firebird::PathName& file_name,
					ISC_STATUS*	status_vector,
					const TEXT*	node_name,
					const TEXT*	user_string,
					bool	uv_flag,
					const SCHAR*	dpb,
					SSHORT	dpb_length)
{
/**************************************
 *
 *	I N E T _ a n a l y z e
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
 *	If the "uv_flag" is non-zero, user verification also takes place.
 *
 **************************************/
/* We need to establish a connection to a remote server.  Allocate the necessary
   blocks and get ready to go. */

	RDB rdb = (RDB) ALLR_block(type_rdb, 0);
	PACKET* packet = &rdb->rdb_packet;

/* Pick up some user identification information */
	UCHAR user_id[BUFFER_SMALL];
	user_id[0] = CNCT_user;
	UCHAR* p = user_id + 2;
	// CVC: Warning: p2 is a reference. It avoids several casts below.
	char*& p2 = reinterpret_cast<char*&>(p);
	int eff_gid;
	int eff_uid;
	ISC_get_user(p2, &eff_uid, &eff_gid, 0, 0, 0, user_string);
	user_id[1] = (UCHAR) strlen(p2);
	p = p + user_id[1];
	fb_assert(user_id[1] < BUFFER_SMALL);

	*p++ = CNCT_host;
	p++;
	fb_assert(MAXHOSTLEN <= BUFFER_SMALL - (p - user_id));
	ISC_get_host(p2, MAXHOSTLEN);
	p[-1] = (UCHAR) strlen(p2);

	for (; *p; p++) {
		if (*p >= 'A' && *p <= 'Z') {
			*p = *p - 'A' + 'a';
		}
	}

	if ((eff_uid == -1) || uv_flag) {
		*p++ = CNCT_user_verification;
		*p++ = 0;
	}
#if !(defined VMS)
	else
	{
		/* Communicate group id info to server, as user maybe running under group
		   id other than default specified in /etc/passwd. */

		*p++ = CNCT_group;
		*p++ = sizeof(SLONG);
		eff_gid = htonl(eff_gid);
		memcpy(p, AOF32L(eff_gid), sizeof(SLONG));
		p += sizeof(SLONG);
	}
#endif

	const USHORT user_length = (USHORT) (p - user_id);
	fb_assert(user_length <= sizeof(user_id));

/* Establish connection to server */

/* Note: prior to V3.1E a recievers could not in truth handle more
   then 5 protocol descriptions, so we try them in chunks of 5 or less */

/* If we want user verification, we can't speak anything less than version 7 */

	P_CNCT*	cnct = &packet->p_cnct;

	cnct->p_cnct_user_id.cstr_length = user_length;
	cnct->p_cnct_user_id.cstr_address = user_id;

	static const p_cnct::p_cnct_repeat protocols_to_try1[] =
	{
		REMOTE_PROTOCOL(PROTOCOL_VERSION8, ptype_rpc, MAX_PTYPE, 1),
		REMOTE_PROTOCOL(PROTOCOL_VERSION10, ptype_rpc, MAX_PTYPE, 2)
#ifdef SCROLLABLE_CURSORS
		,
		REMOTE_PROTOCOL(PROTOCOL_SCROLLABLE_CURSORS, ptype_rpc, MAX_PTYPE, 3)
#endif
	};

	cnct->p_cnct_count = FB_NELEM(protocols_to_try1);

	copy_p_cnct_repeat_array(cnct->p_cnct_versions,
							 protocols_to_try1,
							 cnct->p_cnct_count);

/* Try connection using first set of protocols.  punt if error */

	rem_port* port = inet_try_connect(packet, rdb, file_name,
								 node_name, status_vector, dpb, dpb_length);
	if (!port) {
		return NULL;
	}

	if (packet->p_operation == op_reject && !uv_flag)
	{
		disconnect(port);

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try2[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION6, ptype_rpc, ptype_batch_send, 1),
			REMOTE_PROTOCOL(PROTOCOL_VERSION7, ptype_rpc, MAX_PTYPE, 2)
		};

		cnct->p_cnct_count = FB_NELEM(protocols_to_try2);

		copy_p_cnct_repeat_array(cnct->p_cnct_versions,
								 protocols_to_try2,
								 cnct->p_cnct_count);

		port = inet_try_connect(packet, rdb, file_name,
								node_name, status_vector, dpb, dpb_length);
		if (!port)
			return NULL;
	}

	if (packet->p_operation == op_reject && !uv_flag)
	{
		disconnect(port);

		/* try again with next set of known protocols */

		cnct->p_cnct_user_id.cstr_length = user_length;
		cnct->p_cnct_user_id.cstr_address = user_id;

		static const p_cnct::p_cnct_repeat protocols_to_try3[] =
		{
			REMOTE_PROTOCOL(PROTOCOL_VERSION3, ptype_rpc, ptype_batch_send, 1),
			REMOTE_PROTOCOL(PROTOCOL_VERSION4, ptype_rpc, ptype_batch_send, 2)
		};

		cnct->p_cnct_count = FB_NELEM(protocols_to_try3);

		copy_p_cnct_repeat_array(cnct->p_cnct_versions,
								 protocols_to_try3,
								 cnct->p_cnct_count);

		port = inet_try_connect(packet, rdb, file_name,
								node_name, status_vector, dpb, dpb_length);
		if (!port) {
			return NULL;
		}
	}

	if (packet->p_operation != op_accept)
	{
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_connect_reject;
		*status_vector++ = 0;
		disconnect(port);
		return NULL;
	}

	port->port_protocol = packet->p_acpt.p_acpt_version;

/* once we've decided on a protocol, concatenate the version
   string to reflect it...  */

	Firebird::string temp;
	temp.printf("%s/P%d", port->port_version->str_data, port->port_protocol);
	ALLR_free(port->port_version);
	port->port_version = REMOTE_make_string(temp.c_str());

	if (packet->p_acpt.p_acpt_architecture == ARCHITECTURE) {
		port->port_flags |= PORT_symmetric;
	}

	if (packet->p_acpt.p_acpt_type == ptype_rpc) {
		port->port_flags |= PORT_rpc;
	}

	if (packet->p_acpt.p_acpt_type != ptype_out_of_band) {
		port->port_flags |= PORT_no_oob;
	}

	return port;
}

rem_port* INET_connect(const TEXT* name,
							 PACKET* packet,
							 ISC_STATUS* status_vector,
							 USHORT flag, const SCHAR* dpb, SSHORT dpb_length)
{
/**************************************
 *
 *	I N E T _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish half of a communication link.  If a connect packet is given,
 *	the connection is on behalf of a remote interface.  Otherwise the connect
 *	is for a server process.
 *
 **************************************/
#ifdef DEBUG
	{
		if (INET_trace & TRACE_operations) {
			fprintf(stdout, "INET_connect\n");
			fflush(stdout);
		}
		INET_start_time = inet_debug_timer();
		const char* p = getenv("INET_force_error");
		if (p != NULL) {
			INET_force_error = atoi(p);
		}
	}
#endif

	rem_port* port = alloc_port(0);
	port->port_status_vector = status_vector;
	REMOTE_get_timeout_params(port, reinterpret_cast<const UCHAR*>(dpb),
							  dpb_length);
	status_vector[0] = isc_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = isc_arg_end;
#ifdef VMS
	ISC_tcp_setup(ISC_wait, gds__completion_ast);
#endif

	const TEXT* protocol = NULL;
	TEXT temp[BUFFER_TINY];

	if (name) {
		strncpy(temp, name, sizeof(temp));
		temp[sizeof(temp) - 1] = 0;
		for (TEXT* p = temp; *p;) {
			if (*p++ == '/') {
				p[-1] = 0;
				name = temp;
				protocol = p;
				break;
			}
		}
	}

	if (name && *name) {
		if (port->port_connection) {
			ALLR_free(port->port_connection);
		}
		port->port_connection = REMOTE_make_string(name);
	}
	else {
		name = port->port_host->str_data;
	}

	if (!protocol) {
		const unsigned short port2 = Config::getRemoteServicePort();
		const char* Service = Config::getRemoteServiceName();
		if (port2) {
			// EKU: since temp is 128 byte long, the port number will always
			// fit into the buffer, hence snprintf replaced with sprintf
			sprintf(temp, "%hu", port2);
			protocol = temp;
		}
		else {
			protocol = Service;
		}
	}

/* Set up Inter-Net socket address */

	struct sockaddr_in address;
	inet_zero((SCHAR *) &address, sizeof(address));

#ifdef VMS
	/* V M S */
	if (getservport(protocol, "tcp", &address.sin_port) == -1) {
		inet_error(port, "getservbyname", isc_net_connect_err, 0);
		disconnect(port);
		return NULL;
	}
	if (packet) {
		if (getaddr(name, &address) == -1) {
			inet_error(port, "gethostbyname", isc_net_connect_err, 0);
			disconnect(port);
			return NULL;
		}
	}
	else {
		address.sin_addr.s_addr = INADDR_ANY;
	}
#else

	TEXT msg[BUFFER_SMALL];

/* U N I X style sockets */

	address.sin_family = AF_INET;

	in_addr host_addr;

	if (packet) {
		// client connection
		host_addr = get_host_address(name);

		if (host_addr.s_addr == INADDR_NONE)
		{
			SNPRINTF(msg, FB_NELEM(msg),
					"INET/INET_connect: gethostbyname (%s) failed, error code = %d",
					name, H_ERRNO);
			gds__log(msg, 0);
			inet_gen_error(port,
						   isc_network_error,
						   isc_arg_string,
						   port->port_connection->str_data,
						   isc_arg_gds,
						   isc_net_lookup_err, isc_arg_gds, isc_host_unknown, 0);

			disconnect(port);
			return NULL;
		}
	}
	else {
		// server connection
		host_addr = get_bind_address();
	}

	inet_copy(reinterpret_cast<const SCHAR*>(&host_addr),
				(SCHAR*) &address.sin_addr,
				sizeof(address.sin_addr));

	THREAD_EXIT();

	const struct servent* service = getservbyname(protocol, "tcp");
#ifdef WIN_NT
/* On Windows NT/9x, getservbyname can only accomodate
 * 1 call at a time.  In this case it returns the error
 * WSAEINPROGRESS.
 * If this happens, retry the operation a few times.
 * NOTE: This still does not guarantee success, but helps.
 */
	if (!service) {
		if (H_ERRNO == INET_RETRY_ERRNO) {
			for (int retry = 0; retry < INET_RETRY_CALL; retry++) {
				if ( (service = getservbyname(protocol, "tcp")) )
					break;
			}
		}
	}
#endif /* WIN_NT */
	THREAD_ENTER();

/* Modification by luz (slightly modified by FSG)
    instead of failing here, try applying hard-wired
    translation of "gds_db" into "3050"
    This way, a connection to a remote FB server
    works even from clients with missing "gds_db"
    entry in "services" file, which is important
    for zero-installation clients.
    */
	if (!service) {
		if (strcmp(protocol, FB_SERVICE_NAME) == 0) {
			/* apply hardwired translation */
			address.sin_port = htons(FB_SERVICE_PORT);
		}
		/* modification by FSG 23.MAR.2001 */
		else {
			/* modification by FSG 23.MAR.2001 */
			/* The user has supplied something as protocol
			 * let's see whether this is a port number
			 * instead of a service name
			 */
			address.sin_port = htons(atoi(protocol));
		}

		if (address.sin_port == 0)
		{
			/* end of modification by FSG */
			/* this is the original code */
			SNPRINTF(msg, FB_NELEM(msg),
					"INET/INET_connect: getservbyname failed, error code = %d",
					H_ERRNO);
			gds__log(msg, 0);
			inet_gen_error(port,
						   isc_network_error,
						   isc_arg_string,
						   port->port_connection->str_data,
						   isc_arg_gds,
						   isc_net_lookup_err,
						   isc_arg_gds,
						   isc_service_unknown,
						   isc_arg_string,
						   protocol, isc_arg_string, "tcp", 0);
			return NULL;
		}						/* else / not hardwired gds_db translation */
	}
	else {
		/* if we have got a service-struct, get port number from there
		   * (in case of hardwired gds_db to 3050 translation, address.sin_port was
		   * already set above */
		address.sin_port = service->s_port;
	}							/* else (service found) */

/* end of modifications by luz */
#endif /* VMS */

/* Allocate a port block and initialize a socket for communications */

	port->port_handle = (HANDLE) socket(AF_INET, SOCK_STREAM, 0);

	if ((SOCKET) port->port_handle == INVALID_SOCKET)
	{
		inet_error(port, "socket", isc_net_connect_err, INET_ERRNO);
		disconnect(port);
		return NULL;
	}

/* If we're a host, just make the connection */

    int n;
    
	if (packet) {
		THREAD_EXIT();
		n = connect((SOCKET) port->port_handle,
					(struct sockaddr *) &address, sizeof(address));
		THREAD_ENTER();
		if (n != -1 && send_full(port, packet))
			return port;
		else {
			inet_error(port, "connect", isc_net_connect_err, INET_ERRNO);
			disconnect(port);
			return NULL;
		}
	}

/* We're a server, so wait for a host to show up */

	if (flag & SRVR_multi_client) {
		socklen_t optlen;
		struct linger lingerInfo;

		lingerInfo.l_onoff = 0;
		lingerInfo.l_linger = 0;

		int optval = TRUE;
		n = setsockopt((SOCKET) port->port_handle, SOL_SOCKET, SO_REUSEADDR,
					   (SCHAR*) &optval, sizeof(optval));
		if (n == -1) {
			inet_error(port, "setsockopt REUSE", isc_net_connect_listen_err,
					   INET_ERRNO);
			disconnect(port);
			return NULL;
		}

		/* Get any values for SO_LINGER so that they can be reset during
		 * disconnect.  SO_LINGER should be set by default on the socket
		 */
		optlen = sizeof(port->port_linger);
		n = getsockopt((SOCKET) port->port_handle, SOL_SOCKET, SO_LINGER,
					   (SCHAR *) & port->port_linger, &optlen);

		if (n != 0)				/* getsockopt failed */
			port->port_linger.l_onoff = 0;

		n = setsockopt((SOCKET) port->port_handle, SOL_SOCKET, SO_LINGER,
					   (SCHAR *) & lingerInfo, sizeof(lingerInfo));
		if (n == -1) {
			inet_error(port, "setsockopt LINGER", isc_net_connect_listen_err,
					   INET_ERRNO);
			disconnect(port);
			return NULL;
		}

#ifdef SET_TCP_NO_DELAY

		if (Config::getTcpNoNagle()) {

			int optval = TRUE;
			n = setsockopt((SOCKET) port->port_handle, SOL_SOCKET,
						   TCP_NODELAY, (SCHAR*) &optval, sizeof(optval));

			gds__log("inet log: disabled Nagle algorithm \n");

			if (n == -1) {
				inet_error(port, "setsockopt TCP_NODELAY",
						   isc_net_connect_listen_err, INET_ERRNO);
				disconnect(port);
				return NULL;
			}
		}
#endif

	}

	n = bind((SOCKET) port->port_handle,
			 (struct sockaddr *) &address, sizeof(address));

	if (n == -1) {
		/* On Linux platform, when the server dies the system holds a port
		   for some time. */

		if (INET_ERRNO == INET_ADDR_IN_USE) {
			for (int retry = 0; retry < INET_RETRY_CALL; retry++) {
				sleep(10);
				n = bind((SOCKET) port->port_handle,
						 (struct sockaddr *) &address, sizeof(address));
				if (n == 0)
					break;
			}
		}
		else {
			inet_error(port, "bind", isc_net_connect_listen_err, INET_ERRNO);
			disconnect(port);
			return NULL;
		}
	}

	n = listen((SOCKET) port->port_handle, 5);

	if (n == -1) {
		inet_error(port, "listen", isc_net_connect_listen_err, INET_ERRNO);
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

	while (true) {
		THREAD_EXIT();
		socklen_t l = sizeof(address);
		SOCKET s = accept((SOCKET) port->port_handle,
				   (struct sockaddr *) &address, &l);
		if (s == INVALID_SOCKET) {
			THREAD_ENTER();
			inet_error(port, "accept", isc_net_connect_err, INET_ERRNO);
			disconnect(port);
			return NULL;
		}
#ifdef WIN_NT
		if ((flag & SRVR_debug) || !fork(s, flag))
#else
		if ((flag & SRVR_debug) || !fork())
#endif
		{
			THREAD_ENTER();
			SOCLOSE((SOCKET) port->port_handle);
			port->port_handle = (HANDLE) s;
			port->port_server_flags |= SRVR_server;
			return port;
		}
		THREAD_ENTER();
		SOCLOSE(s);
	}
}

rem_port* INET_reconnect(HANDLE handle, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	I N E T _ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	A communications link has been established by another
 *	process.  We have inheritted the handle.  Set up
 *	a port block.
 *
 **************************************/
	rem_port* port = alloc_port(0);
	port->port_status_vector = status_vector;
	status_vector[0] = isc_arg_gds;
	status_vector[1] = 0;
	status_vector[2] = isc_arg_end;

	port->port_handle = handle;
	port->port_server_flags |= SRVR_server;

	return port;
}

rem_port* INET_server(int sock)
{
/**************************************
 *
 *	I N E T _ s e r v e r
 *
 **************************************
 *
 * Functional description
 *	We have been spawned by a master server with a connection
 *	established.  Set up port block with the appropriate socket.
 *
 **************************************/
#ifdef VMS
	ISC_tcp_setup(ISC_wait, gds__completion_ast);
#endif
	rem_port* port = alloc_port(0);
	port->port_server_flags |= SRVR_server;
	port->port_handle = (HANDLE) sock;

	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
			   (SCHAR *) & optval, sizeof(optval));
	return port;
}

void INET_set_clients( int count)
{
/**************************************
 *
 *	I N E T _ s e t _ c l i e n t s
 *
 **************************************
 *
 * Functional description
 *	Set maxinum number of clients served before
 *	starting new server
 *
 **************************************/
	INET_max_clients = (count && count < MAXCLIENTS) ? count : MAXCLIENTS;
}

static int accept_connection(rem_port* port,
							 P_CNCT* cnct)
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
	TEXT name[BUFFER_SMALL], password[BUFFER_TINY];

/* Default account to "guest" (in theory all packets contain a name) */

	strcpy(name, "guest");
	password[0] = 0;

/* Pick up account and password, if given */

	const TEXT* id = (TEXT *) cnct->p_cnct_user_id.cstr_address;
	const TEXT* const end = id + cnct->p_cnct_user_id.cstr_length;

	SLONG eff_gid, eff_uid;
	eff_uid = eff_gid = -1;
	bool user_verification = false;
	while (id < end)
	{
		switch (*id++)
		{
		case CNCT_user:
			{
				const int length = *id++;
				const int l = MIN(length, FB_NELEM(name) - 1);
				strncpy(name, id, l);
				name[l] = 0;
				id += length;
				break;
			}

		case CNCT_passwd:
			{
				const int length = *id++;
				const int l = MIN(length, FB_NELEM(password) - 1);
				strncpy(password, id, l);
				password[l] = 0;
				id += length;
				break;
			}

		case CNCT_group:
			{
				TEXT* p = (TEXT *) &eff_gid;
				int length  = *id++;
				if (length != 0) {
					do {
						*p++ = *id++;
					} while (--length);
				}
				eff_gid = ntohl(eff_gid);
				break;
			}

			/* this case indicates that the client has requested that
			   we force the user name/password to be verified against
			   the security database */

		case CNCT_user_verification:
			user_verification = true;
			id++;
			break;

		default:
			id += *id + 1;
		}
	}

/* See if user exists.  If not, reject connection */

	if (user_verification) {
		eff_gid = eff_uid = -1;
		port->port_flags |= PORT_not_trusted;
	}

#if !defined(WIN_NT)

#ifdef VMS
	else
	{
		TEXT host[MAXHOSTLEN];
		const int trusted = check_host(port, host, name);
		if (!trusted) {
			return FALSE;
		}

		if (trusted == -1) {
			eff_gid = eff_uid = -1;
			port->port_flags |= PORT_not_trusted;
		}
		else
		{
			check_proxy(port, host, name);

			if (!port->port_parent) {
				if (!chuser(name))
					return FALSE;
				ISC_get_user(name, &eff_uid, &eff_gid, 0, 0, 0, 0);
			}
			/* ??? How to get uic for multi-client server case ??? */
		}
	}
#else
	else
	{
		/* Security check should be against remote name passed - so do
		   check_host first */

		TEXT host[MAXHOSTLEN];
		const struct passwd* passwd = getpwnam(name);
		const int trusted = check_host(port, host, name, passwd);
		if (!trusted) {
			return FALSE;
		}

		if (trusted == -1) {
			eff_gid = eff_uid = -1;
			port->port_flags |= PORT_not_trusted;
		}
		else
		{
			if (check_proxy(port, host, name))
				passwd = getpwnam(name);
			if (!passwd)
				return FALSE;
#ifndef HAVE_INITGROUPS
			eff_gid = passwd->pw_gid;
#else

			SLONG gids[BUFFER_TINY];

			initgroups(passwd->pw_name, passwd->pw_gid);
			if (eff_gid != -1) {
				const int gid_count = getgroups(FB_NELEM(gids), (gid_t*)gids);
				int i;
				for (i = 0; i < gid_count; ++i) {
					if (gids[i] == eff_gid) {
						break;
					}
				}
				if ((i == gid_count) && passwd) {
					eff_gid = passwd->pw_gid;
				}
			}
			else
			{
				eff_gid = passwd->pw_gid;
			}
#endif /* HAVE_INITGROUPS */
			eff_uid = passwd->pw_uid;

			/* if not multi-client: set uid, gid and home directory */

			if (!port->port_parent) {
				if (!eff_gid || setregid(passwd->pw_gid, eff_gid) == -1) {
					setregid(passwd->pw_gid, passwd->pw_gid);
				}
				if (!setreuid(passwd->pw_uid, passwd->pw_uid)) {
					chdir(passwd->pw_dir);
				}
			}
		}
	}

	{
/* If the environment varible ISC_INET_SERVER_HOME is set,
 * change the home directory to the specified directory.
 * Note that this will overrule the normal setting of
 * the current directory to the effective user's home directory.
 * This feature was added primarily for testing via remote
 * loopback - but does seem to be of good general use, so
 * is activiated for the production product.
 * 1995-February-27 David Schnepper
 */
		const char* home = getenv("ISC_INET_SERVER_HOME");
		if (home) {
			if (chdir(home)) {
				gds__log("inet_server: unable to cd to %s errno %d\n", home,
						 INET_ERRNO);
				/* We continue after the error */
			}
		}
	}
#endif /* VMS */

#endif /* !WIN_NT */

/* store FULL user identity in port_user_name for security purposes */

	Firebird::string temp;
	temp.printf("%s.%ld.%ld", name, eff_gid, eff_uid);
	port->port_user_name = REMOTE_make_string(temp.c_str());

	port->port_protocol_str = REMOTE_make_string("TCPv4");

	struct sockaddr_in address;
	socklen_t l = sizeof(address);

	inet_zero((SCHAR *) &address, sizeof(address));
	int status = getpeername((SOCKET) port->port_handle, (struct sockaddr *) &address, &l);
	if (status == 0) {
		Firebird::string addr_str;
		UCHAR* ip = (UCHAR*) &address.sin_addr;
		addr_str.printf(
			"%d.%d.%d.%d",
			static_cast<int>(ip[0]), 
			static_cast<int>(ip[1]), 
			static_cast<int>(ip[2]),
			static_cast<int>(ip[3]) );
		port->port_address_str = REMOTE_make_string(addr_str.c_str());
	}

	return TRUE;
}


#if defined(SUPERSERVER) && defined(WIN_NT)
extern "C" {
	static void atexit_shutdown_winsock()
	{
		WSACleanup();
	}
}
#endif	// SUPERSERVER && WIN_NT


static rem_port* alloc_port( rem_port* parent)
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

	TEXT buffer[BUFFER_SMALL];

#ifdef WIN_NT
	if (!INET_initialized) {
	    const WORD version = MAKEWORD(2, 0);
		if (WSAStartup(version, &INET_wsadata)) {
			if (parent)
				inet_error(parent, "WSAStartup", isc_net_init_error, INET_ERRNO);
			else {
				SNPRINTF(buffer, FB_NELEM(buffer),
						 "INET/alloc_port: WSAStartup failed, error code = %d",
						 INET_ERRNO);
				gds__log(buffer, 0);
			}
			return NULL;
		}
		gds__register_cleanup(exit_handler, 0);
#if defined(SUPERSERVER) && defined(WIN_NT)
		// TMN: 2003-03-11: Shutdown Winsock at program exit
		// Possibly this is also needed for CS, but since I can't test that
		// I decided to only do it for SS.
		atexit(&atexit_shutdown_winsock);
#endif
		INET_initialized = true;
	}
#endif

	if (first_time == true)
	{
		INET_remote_buffer = Config::getTcpRemoteBufferSize();
		if (INET_remote_buffer < MAX_DATA_LW ||
		    INET_remote_buffer > MAX_DATA_HW)
		{
			INET_remote_buffer = DEF_MAX_DATA;
		}
		INET_max_data = INET_remote_buffer;
#ifdef DEBUG
		{
			char msg[BUFFER_SMALL];
			SNPRINTF(msg, FB_NELEM(msg), " Info: Remote Buffer Size set to %ld",
					 INET_remote_buffer);
			gds__log(msg, 0);
		}
#endif
		first_time = false;
	}
	rem_port* port = (rem_port*) ALLR_block(type_port, INET_remote_buffer * 2);
	port->port_type = port_inet;
	port->port_state = state_pending;
	REMOTE_get_timeout_params(port, 0, 0);

	gethostname(buffer, sizeof(buffer));
	port->port_host = REMOTE_make_string(buffer);
	port->port_connection = REMOTE_make_string(buffer);
	SNPRINTF(buffer, FB_NELEM(buffer), "tcp (%s)", port->port_host->str_data);
	port->port_version = REMOTE_make_string(buffer);

	START_PORT_CRITICAL();
	if (parent && !(parent->port_server_flags & SRVR_thread_per_port)) {
		port->port_parent = parent;
		port->port_next = parent->port_clients;
		parent->port_clients = parent->port_next = port;
		port->port_handle = parent->port_handle;
		port->port_server = parent->port_server;
		port->port_server_flags = parent->port_server_flags;
	}
	STOP_PORT_CRITICAL();

	port->port_accept = accept_connection;
	port->port_disconnect = disconnect;
	port->port_receive_packet = receive;
	port->port_send_packet = send_full;
	port->port_send_partial = send_partial;
	port->port_connect = aux_connect;
	port->port_request = aux_request;
	port->port_buff_size = (USHORT) INET_remote_buffer;

	xdrinet_create(	&port->port_send, port,
					&port->port_buffer[INET_remote_buffer],
					(USHORT) INET_remote_buffer,
					XDR_ENCODE);

	xdrinet_create(	&port->port_receive,
					port, port->port_buffer,
					0,
					XDR_DECODE);

	return port;
}

static rem_port* aux_connect(rem_port* port, PACKET* packet, t_event_ast ast)
{
/**************************************
 *
 *	a u x _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Try to establish an alternative connection.  Somebody has already
 *	done a successfull connect request ("packet" contains the response).
 *
 **************************************/
	struct sockaddr_in address;

	socklen_t l = sizeof(address);

/* If this is a server, we're got an auxiliary connection.  Accept it */

	if (port->port_server_flags) {
		SOCKET n = accept(port->port_channel, (struct sockaddr *) &address, &l);
		if (n == INVALID_SOCKET) {
			inet_error(port, "accept", isc_net_event_connect_err, INET_ERRNO);
			SOCLOSE(port->port_channel);
			return NULL;
		}
		SOCLOSE(port->port_channel);
		port->port_handle = (HANDLE) n;
		port->port_flags |= PORT_async;
		return port;
	}

	rem_port* new_port = alloc_port(port->port_parent);
	port->port_async = new_port;
	new_port->port_dummy_packet_interval = port->port_dummy_packet_interval;
	new_port->port_dummy_timeout = new_port->port_dummy_packet_interval;

	new_port->port_flags |= PORT_async;
	P_RESP* response = &packet->p_resp;

/* Set up new socket */

	SOCKET n = socket(AF_INET, SOCK_STREAM, 0);
	if (n == INVALID_SOCKET) {
		inet_error(port, "socket", isc_net_event_connect_err, INET_ERRNO);
		return NULL;
	}

/*
 * NJK - Determine address and port to use.
 *
 * The address returned by the server may be incorrect if it is behind a NAT box
 * so we must use the address that was used to connect the main socket, not the
 * address reported by the server.
 *
 * The port number reported by the server is used. For NAT support the port number
 * should be configured to be a fixed port number in the server configuration.
 */

	inet_zero((SCHAR *) &address, sizeof(address));
	int status = getpeername((SOCKET) port->port_handle, (struct sockaddr *) &address, &l);
	if (status != 0) {
		inet_error(port, "socket", isc_net_event_connect_err, INET_ERRNO);
		SOCLOSE(n);
		return NULL;
	}
	address.sin_family = AF_INET;
	address.sin_port = ((struct sockaddr_in *)(response->p_resp_data.cstr_address))->sin_port;

	THREAD_EXIT();
	status = connect(n, (struct sockaddr *) &address, sizeof(address));
	THREAD_ENTER();

	if (status < 0) {
		inet_error(port, "connect", isc_net_event_connect_err, INET_ERRNO);
		SOCLOSE(n);
		return NULL;
	}

#ifdef SIOCSPGRP
	if (ast)
	{
		int arg;
#ifdef HAVE_GETPGRP
		arg = getpgrp();
#else
		arg = getpid();
#endif
		if (ioctl(n, SIOCSPGRP, &arg) < 0) {
			inet_error(port, "ioctl/SIOCSPGRP", isc_net_event_connect_err,
					   INET_ERRNO);
			SOCLOSE(port->port_channel);
			return NULL;
		}

		new_port->port_ast = ast;
		ISC_signal(SIGURG, inet_handler, new_port);
	}
#endif /* SIOCSPGRP */

	new_port->port_handle = (HANDLE) n;
	new_port->port_flags |= port->port_flags & PORT_no_oob;

	return new_port;
}

static rem_port* aux_request( rem_port* port, PACKET* packet)
{
/**************************************
 *
 *	a u x _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	A remote interface has requested the server prepare an auxiliary
 *	connection; the server calls aux_request to set up the connection.
 *
 **************************************/
	struct sockaddr_in address;

/* Set up new socket */

	address.sin_family = AF_INET;
	in_addr bind_addr = get_bind_address();
	inet_copy(reinterpret_cast<const SCHAR*>(&bind_addr),
				(SCHAR*) &address.sin_addr,
				sizeof(address.sin_addr));
	address.sin_port = htons(Config::getRemoteAuxPort());

	SOCKET n = socket(AF_INET, SOCK_STREAM, 0);
	if (n == INVALID_SOCKET) {
		inet_error(port, "socket", isc_net_event_listen_err, INET_ERRNO);
		return NULL;
	}

	int optval;
	setsockopt(n, SOL_SOCKET, SO_REUSEADDR,
			   (SCHAR *) &optval, sizeof(optval));

	if (bind(n, (struct sockaddr *) &address, sizeof(address)) < 0) {
		inet_error(port, "bind", isc_net_event_listen_err, INET_ERRNO);
		return NULL;
	}

	socklen_t length = sizeof(address);

	if (getsockname(n, (struct sockaddr *) &address, &length) < 0) {
		inet_error(port, "getsockname", isc_net_event_listen_err, INET_ERRNO);
		return NULL;
	}

	if (listen(n, 1) < 0) {
		inet_error(port, "listen", isc_net_event_listen_err, INET_ERRNO);
		return NULL;
	}

    rem_port* new_port = alloc_port(port->port_parent);
	port->port_async = new_port;
	new_port->port_dummy_packet_interval = port->port_dummy_packet_interval;
	new_port->port_dummy_timeout = new_port->port_dummy_packet_interval;

	new_port->port_server_flags = port->port_server_flags;
	new_port->port_channel = (int) n;
	new_port->port_flags = port->port_flags & PORT_no_oob;

	P_RESP* response = &packet->p_resp;

	struct sockaddr_in port_address;
	if (getsockname((SOCKET) port->port_handle, (struct sockaddr *) &port_address, &length) < 0) {
		inet_error(port, "getsockname", isc_net_event_listen_err, INET_ERRNO);
		return NULL;
	}
	inet_copy(reinterpret_cast<SCHAR*>(&port_address.sin_addr),
				(SCHAR*) &address.sin_addr,
				sizeof(address.sin_addr));

	response->p_resp_data.cstr_address = 
			reinterpret_cast<UCHAR*>(& response->p_resp_blob_id);
	response->p_resp_data.cstr_length = sizeof(response->p_resp_blob_id);
	inet_copy(reinterpret_cast<const SCHAR*>(&address),
			  reinterpret_cast<char*>(response->p_resp_data.cstr_address),
			  response->p_resp_data.cstr_length);

	return new_port;
}

#ifndef WIN_NT
#ifdef VMS
static int check_host(rem_port* port,
					  TEXT* host_name,
					  const TEXT* user_name)
{
/**************************************
 *
 *	c h e c k _ h o s t	( V M S )
 *
 **************************************
 *
 * Functional description
 *	Check the host on the other end of the socket to see it
 *	it's an equivalent host.
 *
 **************************************/
	struct sockaddr_in address;
	TEXT hosts_file[MAXPATHLEN];

	int length = sizeof(address);

	if (getpeername((int) port->port_handle, &address, &length) == -1)
		return FALSE;

	if (getnamebyaddr(&address, sizeof(address), host_name) == -1)
		return FALSE;

	gethosts(hosts_file);
	const int result = parse_hosts(hosts_file, host_name, user_name);
	if (result == -1)
		result = FALSE;

	return result;
}
#else

static int check_host(
					  rem_port* port,
					  TEXT* host_name,
					  const TEXT* user_name, const struct passwd *passwd)
{
/**************************************
 *
 *	c h e c k _ h o s t 	 ( n o n - V M S )
 *
 **************************************
 *
 * Functional description
 *	Check the host on the other end of the socket to see it
 *	it's an equivalent host.
 * NB.: First check the ~/.rhosts then the HOSTS_FILE - both have
 *	the same line formats (see parse_line)
 *
 **************************************/
	struct sockaddr_in address;

	socklen_t length = sizeof(address);

	if (getpeername((int) port->port_handle, (struct sockaddr*)&address, &length) == -1)
		return 0;

	const struct hostent* host = gethostbyaddr((SCHAR *) & address.sin_addr,
							   sizeof(address.sin_addr), address.sin_family);
	if (!host)
	{
		return 0;
	}

	int result = -1;

	strcpy(host_name, host->h_name);

	TEXT user[BUFFER_TINY], rhosts[MAXPATHLEN];
	if (passwd) {
		strcpy(user, passwd->pw_name);
		strcpy(rhosts, passwd->pw_dir);
		strcat(rhosts, "/.rhosts");
		result = parse_hosts(rhosts, host_name, user);
	}
	else
		strcpy(user, user_name);

	if (result == -1) {
		FILE* fp = fopen(GDS_HOSTS_FILE, "r");
		const TEXT* hosts_file = fp ? (TEXT*)GDS_HOSTS_FILE : (TEXT*)HOSTS_FILE;
		if (fp)
			fclose(fp);

		result = parse_hosts(hosts_file, host_name, user);
		if (result == -1)
			result = FALSE;
	}
	return result;
}
#endif // !non VMS
#endif // !defined(WIN_NT)

#if !(defined WIN_NT)
static bool check_proxy(rem_port* port,
						TEXT * host_name,
						TEXT * user_name)
{
/**************************************
 *
 *	c h e c k _ p r o x y
 *
 **************************************
 *
 * Functional description
 *	Lookup <host_name, user_name> in proxy file.  If found,
 *	change user_name.
 *
 **************************************/
	TEXT proxy_file[MAXPATHLEN];
	TEXT source_user[BUFFER_TINY];
	TEXT source_host[MAXHOSTLEN];
	TEXT target_user[BUFFER_TINY];
	TEXT line[BUFFER_SMALL];

#ifndef VMS
	strcpy(proxy_file, PROXY_FILE);
#else
	gds__prefix(proxy_file, PROXY_FILE);
#endif
	FILE* proxy = fopen(proxy_file, "r");
	if (!proxy)
		return false;

/* Read lines, scan, and compare */

	bool result = false;

	for (;;) {
		int c;
		TEXT* p = line;
		while (((c = getc(proxy)) != 0) && c != EOF && c != '\n')
		{
			*p++ = c;
		}
		*p = 0;
		if (sscanf(line, " %[^:]:%s%s", source_host, source_user, target_user)
			>= 3)
			if ((!strcmp(source_host, host_name) || !strcmp(source_host, "*"))
				&& (!strcmp(source_user, user_name)
					|| !strcmp(source_user, "*")))
			{
				ALLR_free(port->port_user_name);
				const SLONG length = strlen(target_user);
				rem_str* string = (rem_str*) ALLR_block(type_str, (int) length);
				port->port_user_name = string;
				string->str_length = length;
				strncpy(string->str_data, target_user, length);
				strcpy(user_name, target_user);
				result = true;
				break;
			}
		if (c == EOF)
			break;
	}

	fclose(proxy);

	return result;
}
#endif

static void disconnect( rem_port* port)
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

#ifndef VMS
/* SO_LINGER was turned off on the initial bind when the server was started.
 * This will force a reset to be sent to the client when the socket is closed.
 * We only want this behavior in the case of the server terminiating
 * abnormally and not on an orderly shut down.  Because of this, turn the
 * SO_LINGER option back on for the socket.  The result of setsockopt isn't
 * too important at this stage since we are closing the socket anyway.  This
 * is an attempt to return the socket to a state where a graceful shutdown can
 * occur.
 */

	if (port->port_linger.l_onoff) {
		setsockopt((SOCKET) port->port_handle, SOL_SOCKET, SO_LINGER,
					   (SCHAR *) & port->port_linger,
					   sizeof(port->port_linger));
	}

#if defined WIN_NT

	if (port->port_handle && (SOCKET) port->port_handle != INVALID_SOCKET) {
		shutdown((int) port->port_handle, 2);
	}

#else /* WIN_NT */

	if (port->port_handle) {
		shutdown((int) port->port_handle, 2);
	}

#endif /* WIN_NT */

#endif /* !VMS */

#if !(defined VMS || defined WIN_NT)
	if (port->port_ast) {
		ISC_signal_cancel(SIGURG, inet_handler, port);
	}
#endif

/* If this is a sub-port, unlink it from it's parent */

#ifdef  DEFER_PORT_CLEANUP
	bool defer_cleanup = false;
	port->port_state = state_disconnected;
#endif
	rem_port* parent = port->port_parent;
	if (parent != NULL) {
		if (port->port_async) {
			disconnect(port->port_async);
			port->port_async = NULL;
		}
#ifdef	DEFER_PORT_CLEANUP
		defer_cleanup = true;
#else
		unhook_port(port, parent);
#endif
	}
	else if (port->port_async) {
#ifdef MULTI_THREAD
		port->port_async->port_flags |= PORT_disconnect;
#else
		disconnect(port->port_async);
		port->port_async = NULL;
#endif
	}

	if (port->port_handle) {
		SOCLOSE((SOCKET) port->port_handle);
	}

	gds__unregister_cleanup(exit_handler, (void *) port);

#ifdef DEFER_PORT_CLEANUP
	if (!defer_cleanup)
#endif
		cleanup_port(port);

#ifdef DEBUG
	if (INET_trace & TRACE_summary) {
		fprintf(stdout, "INET_count_send = %lu packets\n",
				   INET_count_send);
		fprintf(stdout, "INET_bytes_send = %lu bytes\n",
				   INET_bytes_send);
		fprintf(stdout, "INET_count_recv = %lu packets\n",
				   INET_count_recv);
		fprintf(stdout, "INET_bytes_recv = %lu bytes\n",
				   INET_bytes_recv);
		fflush(stdout);
	}
#endif

	return;
}


static void cleanup_port( rem_port* port)
{
/**************************************
 *
 *	c l e a n u p _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Walk through the port structure freeing
 * 	allocated memory and then free the port.
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

	if (port->port_protocol_str)
		ALLR_free((UCHAR *) port->port_protocol_str);

	if (port->port_address_str)
		ALLR_free((UCHAR *) port->port_address_str);

#ifdef DEBUG_XDR_MEMORY
	if (port->port_packet_vector)
		ALLR_free((UCHAR *) port->port_packet_vector);
#endif

	ALLR_release((UCHAR *) port);
	return;
}

static void exit_handler( void *arg)
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

	rem_port* main_port = (rem_port*) arg;

#ifdef WIN_NT
	if (!main_port) {
		WSACleanup();
		return;
	}
#endif

#ifndef VMS
	for (rem_port* port = main_port; port; port = port->port_next) {
		shutdown((int) port->port_handle, 2);
		SOCLOSE((SOCKET) port->port_handle);
	}
#endif
}

#ifdef NO_FORK
static int fork(void)
{
/**************************************
 *
 *	f o r k		( N O _ F O R K )
 *
 **************************************
 *
 * Functional description
 *	Hmmm.
 *
 **************************************/

	return 1;
}
#endif

#ifdef WIN_NT
static int fork( SOCKET old_handle, USHORT flag)
{
/**************************************
 *
 *	f o r k		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Create a child process.
 *
 **************************************/
	if (!INET_command_line[0]) {

#ifdef CMDLINE_VIA_SERVICE_MANAGER

		SC_HANDLE service;
		SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
		if (manager &&
			(service =
			 OpenService(manager, REMOTE_SERVICE, SERVICE_QUERY_CONFIG)))
		{
			LPQUERY_SERVICE_CONFIG config;
			SCHAR buffer[1024];
			DWORD config_len;

			config = (LPQUERY_SERVICE_CONFIG) buffer;
			if (!QueryServiceConfig
				(service, config, sizeof(buffer), &config_len)) {
				THREAD_ENTER();
				config = (LPQUERY_SERVICE_CONFIG) ALLR_alloc(config_len);
				/* NOMEM: ALLR_alloc handled */
				/* FREE:  later in this block */
				QueryServiceConfig(service, config, config_len, &config_len);
			}
			strcpy(INET_command_line, config->lpBinaryPathName);
			if ((SCHAR *) config != buffer) {
				ALLR_free(config);
				THREAD_EXIT();
			}
			CloseServiceHandle(service);
		}
		else {
			strcpy(INET_command_line, GetCommandLine());
		}
		CloseServiceHandle(manager);
#else
		strcpy(INET_command_line, GetCommandLine());
#endif
		INET_p = INET_command_line + strlen(INET_command_line);
	}

	HANDLE new_handle;
	DuplicateHandle(GetCurrentProcess(), (HANDLE) old_handle,
					GetCurrentProcess(), &new_handle, 0, TRUE,
					DUPLICATE_SAME_ACCESS);

	sprintf(INET_p, " -s -i -h %"SLONGFORMAT, (SLONG) new_handle);
	STARTUPINFO start_crud;
	start_crud.cb = sizeof(STARTUPINFO);
	start_crud.lpReserved = NULL;
	start_crud.lpReserved2 = NULL;
	start_crud.cbReserved2 = 0;
	start_crud.lpDesktop = NULL;
	start_crud.lpTitle = NULL;
	start_crud.dwFlags = STARTF_FORCEOFFFEEDBACK;
	
	PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, INET_command_line, NULL, NULL, TRUE,
							(flag & SRVR_high_priority ?
							 HIGH_PRIORITY_CLASS | DETACHED_PROCESS :
							 NORMAL_PRIORITY_CLASS | DETACHED_PROCESS),
							NULL, NULL, &start_crud, &pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	CloseHandle(new_handle);

	return 1;
}
#endif

static in_addr get_bind_address()
{
/**************************************
 *
 *	g e t _ b i n d _ a d d r e s s
 *
 **************************************
 *
 * Functional description
 *	Return local address to bind sockets to.
 *
 **************************************/
	in_addr config_address;

	const char* config_option = Config::getRemoteBindAddress();
	config_address.s_addr =
		(config_option) ? inet_addr(config_option) : INADDR_NONE;
	if (config_address.s_addr == INADDR_NONE) {
		config_address.s_addr = INADDR_ANY;
	}
	return config_address;
}

static in_addr get_host_address(const TEXT* name)
{
/**************************************
 *
 *	g e t _ h o s t _ a d d r e s s
 *
 **************************************
 *
 * Functional description
 *	Return host address.
 *
 **************************************/
	in_addr address;

	THREAD_EXIT();

	address.s_addr = inet_addr(name);

	if (address.s_addr == INADDR_NONE) {

		const hostent* host = gethostbyname(name);

		/* On Windows NT/9x, gethostbyname can only accomodate
		 * 1 call at a time.  In this case it returns the error
		 * WSAEINPROGRESS. On UNIX systems, this call may not succeed
		 * because of a temporary error.  In this case, it returns
		 * h_error set to TRY_AGAIN.  When these errors occur,
		 * retry the operation a few times.
		 * NOTE: This still does not guarantee success, but helps.
		 */
		if (!host) {
			if (H_ERRNO == INET_RETRY_ERRNO) {
				for (int retry = 0; retry < INET_RETRY_CALL; retry++) {
					if ( (host = gethostbyname(name)) )
						break;
				}
			}
		}

		if (host) {
			inet_copy(host->h_addr, (SCHAR*) &address, sizeof(address));
		}
	}

	THREAD_ENTER();

	return address;
}

//____________________________________________________________
//
//	Copy an array of p_cnct::p_cnct_repeat.
//
static void copy_p_cnct_repeat_array(	p_cnct::p_cnct_repeat*			pDest,
										const p_cnct::p_cnct_repeat*	pSource,
										size_t							nEntries)
{
	for (size_t i = 0; i < nEntries; ++i) {
		pDest[i] = pSource[i];
	}
}


static void inet_copy(const SCHAR* from, SCHAR* to, int length)
{
/**************************************
 *
 *	i n e t _ c o p y
 *
 **************************************
 *
 * Functional description
 *	Copy a number of bytes;
 *
 **************************************/

	if (length) {
		do {
			*to++ = *from++;
		} while ((--length) != 0);
	}
}

static void inet_zero( SCHAR* address, int length)
{
/**************************************
 *
 *	i n e t _  z e r o
 *
 **************************************
 *
 * Functional description
 *	Zero a block of memory.
 *
 **************************************/

	if (length) {
		do {
			*address++ = 0;
		} while ((--length) != 0);
	}
}

#if !(defined WIN_NT)
static int parse_hosts( const TEXT* file_name, const TEXT* host_name,
	const TEXT* user_name)
{
/*****************************************************************
 *
 *	p a r s e _ h o s t s
 *
 *****************************************************************
 *
 * Functional description:
 *	Parse hosts file (.rhosts or hosts.equiv) to determine
 *	if user_name on host_name should be allowed access.
 *
 *****************************************************************/
	TEXT line[BUFFER_SMALL], entry1[BUFFER_SMALL], entry2[BUFFER_SMALL];

	int result = -1;
	FILE* fp = fopen(file_name, "r");

	if (fp) {
		for (;;) {
			entry1[0] = entry2[0] = 0;
			entry1[1] = entry2[1] = 0;
			int c;
			TEXT* p = line;
			while ((c = getc(fp)) != EOF && c != '\n')
				*p++ = c;
			*p = 0;
			sscanf(line, "%s", entry1);
			sscanf(&line[strlen(entry1)], "%s", entry2);
			result = parse_line(entry1, entry2, host_name, user_name);
			if (c == EOF || result > -1)
				break;
		}
		fclose(fp);
	}

	return result;
}
#endif

#if !(defined WIN_NT)
static int parse_line(
					  const TEXT* entry1,
					  const TEXT* entry2,
					  const TEXT* host_name, const TEXT* user_name)
{
/*****************************************************************
 *
 *	p a r s e _ l i n e
 *
 *****************************************************************
 *
 * Functional description:
 *	Parse hosts file (.rhosts or hosts.equiv) to determine
 *	if user_name on host_name should be allowed access.
 *
 *  Returns 
 *  1 if user_name is allowed
 *  0 if not allowed and 
 *  -1 if there is not a host_name or a user_name
 *
 * only supporting:
 *    + - anybody on any machine
 *    -@machinegroup [2nd entry optional] - nobody on that machine
 *    +@machinegroup - anybody on that machine
 *    +@machinegroup username - this person on these machines
 *    +@machinegroup +@usergroup - these people  on these machines
 *    +@machinegroup -@usergroup - anybody on these machines EXCEPT these people
 *    machinename - anyone on this machine
 *    machinename username - this person on this machine
 *    machinename +@usergroup - these people on this machine
 *    machinename -@usergroup - anybody but these people on this machine
 *    machinename + - anybody on this machine
 *
 ******************************************************************/

/* if entry1 is simply a "+" - everyone's in */

	if (!strcmp(entry1, "+"))
		return TRUE;

/* if we don't have a host_name match, don't bother */

	if (strcmp(entry1, host_name))
#if (defined UNIX) && !(defined SINIXZ) && !(defined NETBSD)
		if (entry1[1] == '@') {
			if (!innetgr(&entry1[2], host_name, 0, 0))
				return -1;
		}
		else
#endif
			return -1;

/* if we have a host_name match and an exclude symbol - they're out */

	if (entry1[0] == '-')
		return FALSE;

/* if we matched the machine and seen a + after the machine
   name they are in (eg: <machinename> + ) */

	if ((entry2[0] == '+') && (strlen(entry2) == 1))
		return TRUE;

/* if we don't have a second entry OR it matches the user_name - they're in */

	if (!entry2[0] || !strcmp(entry2, user_name))
		return TRUE;

/* if they're in the user group: + they're in, - they're out */

#if (defined UNIX) && !(defined SINIXZ) && !(defined NETBSD)
	if (entry2[1] == '@') {
		if (innetgr(&entry2[2], 0, user_name, 0)) {
			if (entry2[0] == '+')
				return TRUE;
			else
				return FALSE;
		}
		else {
			/* if they're NOT in the user group AND we're excluding it
			   - they're in */

			if (entry2[0] == '-')
				return TRUE;
		}
	}
#endif

/* if we get here - we matched machine but not user - maybe next line ... */

	return -1;
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
 *	Receive a message from a port or clients of a port.  If the process
 *	is a server and a connection request comes in, generate a new port
 *	block for the client.
 *
 **************************************/

/* If this isn't a multi-client server, just do the operation and get it
   over with */

	if (!(main_port->port_server_flags & SRVR_multi_client)) {
		/* loop as long as we are receiving dummy packets, just
		   throwing them away--note that if we are a server we won't
		   be receiving them, but it is better to check for them at
		   this level rather than try to catch them in all places where
		   this routine is called */

		do {
			if (!xdr_protocol(&main_port->port_receive, packet))
				return NULL;
#ifdef DEBUG
			{
				static ULONG op_rec_count = 0;
				op_rec_count++;
				if (INET_trace & TRACE_operations) {
					fprintf(stdout, "%04lu: OP Recd %5lu opcode %d\n",
							   inet_debug_timer(),
							   op_rec_count, packet->p_operation);
					fflush(stdout);
				}
			}
#endif
		}
		while (packet->p_operation == op_dummy);

		return main_port;
	}

/* Multi-client server multiplexes all known ports for incoming packets. */

	for (;;) {
		rem_port* port = select_port(main_port, &INET_select);
		if (port == main_port) {
			if (port = select_accept(main_port))
				return port;
			continue;
		}
		if (port) {
			if (port->port_dummy_timeout < 0) {
				port->port_dummy_timeout = port->port_dummy_packet_interval;
				if (port->port_flags & PORT_async ||
					port->port_protocol < PROTOCOL_VERSION8) continue;
				packet->p_operation = op_dummy;
				return port;
			}
			/* We've got data -- lap it up and use it */

			if (!xdr_protocol(&port->port_receive, packet))
				packet->p_operation = op_exit;
#ifdef DEBUG
			{
				static ULONG op_rec_count = 0;
				op_rec_count++;
				if (INET_trace & TRACE_operations) {
					fprintf(stdout, "%05lu: OP Recd %5lu opcode %d\n",
							   inet_debug_timer(),
							   op_rec_count, packet->p_operation);
					fflush(stdout);
				}
			}
#endif

/*  Make sure that there are no more messages in this port before blocking
    ourselves in select_wait. If there are more messages then set the flag
    corresponding to this port which was cleared in select_port routine.
*/
			if (port->port_receive.x_handy) {
				FD_SET((SLONG) port->port_handle, &INET_select.slct_fdset);
				++INET_select.slct_count;
			}
			if (packet->p_operation == op_dummy)
				continue;

			return port;
		}
		if (!select_wait(main_port, &INET_select))
			return NULL;
	}
}

static rem_port* select_accept( rem_port* main_port)
{
/**************************************
 *
 *	s e l e c t _ a c c e p t
 *
 **************************************
 *
 * Functional description
 *	Accept a new connection request.
 *	If client load is exceeded then
 *	handoff connection responsibility
 *	to a fresh server.
 *
 **************************************/
	struct sockaddr_in address;

	rem_port* port = alloc_port(main_port);
	socklen_t l = sizeof(address);

	port->port_handle = (HANDLE) accept((SOCKET) main_port->port_handle,
										(struct sockaddr *) &address, &l);
	if ((SOCKET) port->port_handle == INVALID_SOCKET) {
		inet_error(port, "accept", isc_net_connect_err, INET_ERRNO);
		disconnect(port);
		return 0;
	}

	int optval = 1;
	setsockopt((SOCKET) port->port_handle, SOL_SOCKET, SO_KEEPALIVE,
			   (SCHAR*) &optval, sizeof(optval));

#if !(defined SUPERSERVER || defined VMS || defined WIN_NT)
	int n;
	for (n = 0, port = main_port->port_clients; port;
		 n++, port = port->port_next);
	if (n >= INET_max_clients) {
		main_port->port_state = state_closed;
		SOCLOSE((int) main_port->port_handle);
		TEXT msg[BUFFER_SMALL];
		SNPRINTF(msg, FB_NELEM(msg),
				"INET/select_accept: exec new server at client limit: %d", n);
		gds__log(msg, 0);

		setreuid(0, 0);
		kill(getppid(), SIGUSR1);
	}
#endif

	if (main_port->port_server_flags & SRVR_thread_per_port) {
		port->port_server_flags =
			(SRVR_server | SRVR_inet | SRVR_thread_per_port);
		return port;
	}

	return 0;
}

static rem_port* select_port( rem_port* main_port, SLCT * selct)
{
/**************************************
 *
 *	s e l e c t _ p o r t
 *
 **************************************
 *
 * Functional description
 *	Select a descriptor that is ready to read
 *	and return the port block. Additionally,
 *	check if a port's keepalive timer has
 *	expired and return the port block so that
 *	a keepalive packet can be queued. Return
 *	NULL if	none are active.
 *
 **************************************/
#ifdef WIN_NT

/* NT's socket handles are addresses */
/* TMN: No, they are "black-box" handles. */

	START_PORT_CRITICAL();
#ifdef  DEFER_PORT_CLEANUP
	unhook_disconnected_ports(main_port);
#endif
	for (rem_port* port = main_port; port; port = port->port_next) {
		if (FD_ISSET(port->port_handle, &selct->slct_fdset)) {
			port->port_dummy_timeout = port->port_dummy_packet_interval;

#pragma FB_COMPILER_MESSAGE("TODO: Make porthandle a SOCKET on Win32")

			FD_CLR((SOCKET) port->port_handle, &selct->slct_fdset);
			--selct->slct_count;
			STOP_PORT_CRITICAL();
			return port;
		}
		else if (port->port_dummy_timeout < 0) {
			STOP_PORT_CRITICAL();
			return port;
		}
	}

	STOP_PORT_CRITICAL();
#else // !defined(WIN_NT)
	START_PORT_CRITICAL();
#ifdef  DEFER_PORT_CLEANUP
	unhook_disconnected_ports(main_port);
#endif
	for (rem_port* port = main_port; port; port = port->port_next) {
		const int n = (int) port->port_handle;
		if (n < selct->slct_width && FD_ISSET(n, &selct->slct_fdset)) {
			port->port_dummy_timeout = port->port_dummy_packet_interval;
			FD_CLR(n, &selct->slct_fdset);
			--selct->slct_count;
			STOP_PORT_CRITICAL();
			return port;
		}
		else if (port->port_dummy_timeout < 0) {
			STOP_PORT_CRITICAL();
			return port;
		}
	}
	STOP_PORT_CRITICAL();
#endif

	return NULL;
}

static int select_wait( rem_port* main_port, SLCT * selct)
{
/**************************************
 *
 *	s e l e c t _ w a i t
 *
 **************************************
 *
 * Functional description
 *	Select interesting descriptors from
 *	port blocks and wait for something
 *	to read from them.
 *
 **************************************/
	TEXT msg[BUFFER_SMALL];
	struct timeval timeout;

	for (;;)
	{
		selct->slct_count = selct->slct_width = 0;
		FD_ZERO(&selct->slct_fdset);
		bool found = false;

		/* Use the time interval between select() calls to expire
		   keepalive timers on all ports. */

		SLONG delta_time;
		if (selct->slct_time)
		{
			delta_time = (SLONG) time(NULL) - selct->slct_time;
			selct->slct_time += delta_time;
		}
		else
		{
			delta_time = 0;
			selct->slct_time = (SLONG) time(NULL);
		}

		START_PORT_CRITICAL();
#ifdef  DEFER_PORT_CLEANUP
		unhook_disconnected_ports(main_port);
#endif
		for (rem_port* port = main_port; port; port = port->port_next)
		{
			if ((port->port_state == state_active) ||
				(port->port_state == state_pending))
			{
				/* Adjust down the port's keepalive timer. */

				if (port->port_dummy_packet_interval)
				{
					port->port_dummy_timeout -= delta_time;
				}

				FD_SET((SLONG) port->port_handle, &selct->slct_fdset);
#ifdef WIN_NT
				++selct->slct_width;
#else
				selct->slct_width =
					MAX(selct->slct_width, (int) port->port_handle);
#endif
				found = true;
			}
		}
		STOP_PORT_CRITICAL();

		if (!found)
		{
			gds__log("INET/select_wait: client rundown complete, server exiting",
				 0);
			return FALSE;
		}

		THREAD_EXIT();
		++selct->slct_width;

		for (;;)
		{
			// Some platforms change the timeout in the select call.
			// Reset timeout for each iteration to avoid problems.
			timeout.tv_sec = SELECT_TIMEOUT;
			timeout.tv_usec = 0;

#ifdef WIN_NT
			selct->slct_count = select(FD_SETSIZE, &selct->slct_fdset,
									   NULL, NULL, &timeout);
#else
			selct->slct_count = select(selct->slct_width, &selct->slct_fdset,
									   NULL, NULL, &timeout);
#endif
			if (selct->slct_count != -1)
			{
				/* if selct->slct_count is zero it means that we timed out of
				   select with nothing to read or accept, so clear the fd_set
				   bit as this value is undefined on some platforms (eg. HP-UX),
				   when the select call times out. Once these bits are cleared
				   they can be used in select_port() */
				if (selct->slct_count == 0)
				{
					for (rem_port* port = main_port; port; port = port->port_next)
					{
#ifdef WIN_NT
						FD_CLR((SOCKET)port->port_handle, &selct->slct_fdset);
#else
						FD_CLR(port->port_handle, &selct->slct_fdset);
#endif
					}
				}
				THREAD_ENTER();
				return TRUE;
			}
			else if (INTERRUPT_ERROR(INET_ERRNO))
				continue;
#ifndef WIN_NT
			else if (INET_ERRNO == EBADF)
				break;
#else
			else if (INET_ERRNO == WSAENOTSOCK)
				break;
#endif
			else {
				THREAD_ENTER();
				SNPRINTF(msg, FB_NELEM(msg),
						 "INET/select_wait: select failed, errno = %d",
						 INET_ERRNO);
				gds__log(msg, 0);
				return FALSE;
			}
		}	// for (;;)

		THREAD_ENTER();
	}
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
 *
 **************************************/

	if (!xdr_protocol(&port->port_send, packet))
		return FALSE;

#ifdef DEBUG
	{
		static ULONG op_sent_count = 0;
		op_sent_count++;
		if (INET_trace & TRACE_operations) {
			fprintf(stdout, "%05lu: OP Sent %5lu opcode %d\n",
					   inet_debug_timer(),
					   op_sent_count, packet->p_operation);
			fflush(stdout);
		}
	}
#endif

	return xdrinet_endofrecord(&port->port_send, TRUE);
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

#ifdef DEBUG
	{
		static ULONG op_sentp_count = 0;
		op_sentp_count++;
		if (INET_trace & TRACE_operations) {
			fprintf(stdout, "%05lu: OP Sent %5lu opcode %d (partial)\n",
					   inet_debug_timer(),
					   op_sentp_count, packet->p_operation);
			fflush(stdout);
		}
	}
#endif

	return xdr_protocol(&port->port_send, packet);
}


static int xdrinet_create(
						  XDR * xdrs,
						  rem_port* port,
						  UCHAR * buffer, USHORT length, enum xdr_op x_op)
{
/**************************************
 *
 *	x d r i n e t _ c r e a t e
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
	xdrs->x_ops = (xdr_t::xdr_ops *) & inet_ops;
	xdrs->x_op = x_op;

	return TRUE;
}

static bool_t xdrinet_endofrecord( XDR * xdrs, bool_t flushnow)
{
/**************************************
 *
 *	x d r i n e t _ e n d o f r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write out the rest of a record.
 *
 **************************************/

	return inet_write(xdrs, flushnow);
}
#ifdef HAVE_SETITIMER
static void alarm_handler( int x)
{
/**************************************
 *
 *	a l a r m _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Handle an alarm clock interrupt.  If we were waiting on
 *	a semaphone, zap it.
 *
 **************************************/
}
#endif
static XDR_INT inet_destroy( XDR* xdrs)
{
/**************************************
 *
 *	i n e t _ d e s t r o y
 *
 **************************************
 *
 * Functional description
 *	Destroy a stream.  A no-op.
 *
 **************************************/

	return (XDR_INT)0;
}

static void inet_gen_error( rem_port* port, ISC_STATUS status, ...)
{
/**************************************
 *
 *	i n e t _ g e n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error has occurred.  Mark the port as broken.
 *	Format the status vector if there is one and
 *	save the status vector strings in a permanent place.
 *
 **************************************/
	port->port_flags |= PORT_broken;
	port->port_state = state_broken;

	ISC_STATUS* status_vector = NULL;
	if (port->port_context != NULL) {
		status_vector = port->port_context->rdb_status_vector;
	}
	if (status_vector == NULL) {
		status_vector = port->port_status_vector;
	}
	if (status_vector != NULL) {
		STUFF_STATUS(status_vector, status);
		REMOTE_save_status_strings(status_vector);
	}
}

static bool_t inet_getbytes( XDR * xdrs, SCHAR * buff, u_int count)
{
/**************************************
 *
 *	i n e t _ g e t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Get a bunch of bytes from a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > (SLONG) sizeof(ISC_QUAD)) {
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
			if (!inet_read(xdrs))
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
		if (!xdrs->x_handy && !inet_read(xdrs))
			return FALSE;
		*buff++ = *xdrs->x_private++;
		--xdrs->x_handy;
	}

	return TRUE;
}

static bool_t inet_getlong( XDR * xdrs, SLONG * lp)
{
/**************************************
 *
 *	i n e t _ g e t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/

	SLONG l;

	if (!(*xdrs->x_ops->x_getbytes) (xdrs, reinterpret_cast<char*>(&l), 4))
		return FALSE;

	*lp = ntohl(l);

	return TRUE;
}

static u_int inet_getpostn( XDR * xdrs)
{
/**************************************
 *
 *	i n e t _ g e t p o s t n
 *
 **************************************
 *
 * Functional description
 *	Get the current position (which is also current length) from stream.
 *
 **************************************/

	return (u_int) (xdrs->x_private - xdrs->x_base);
}

#if !(defined WIN_NT)
static void inet_handler(void* port_void)
{
/**************************************
 *
 *	i n e t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Inet_handler is called the the signal handler on receipt of
 *	a SIGURG signal indicating out-of-band data.  Since SIGURG
 *	may be noisy, check to see if any IO is pending on the channel.
 *	If not, ignore the signal.  If so, call the port specific
 *	handler to do something appropriate.
 *
 **************************************/
	rem_port* port = static_cast<rem_port*>(port_void);

/* If there isn't any out of band data, this signal isn't for us */
	SCHAR junk;
	const int n = recv((SOCKET) port->port_handle, &junk, 1, MSG_OOB);
	if ((n) < 0) {
		return;
	}
	
	(*port->port_ast) (port);
}
#endif

static caddr_t inet_inline( XDR* xdrs, u_int bytecount)
{
/**************************************
 *
 *	i n e t _  i n l i n e
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

static void inet_error(
					  rem_port* port,
					  const TEXT* function, ISC_STATUS operation, int status)
{
/**************************************
 *
 *	i n e t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An I/O error has occurred.  Call
 * 	inet_gen_error with the appropriate args
 *	to format the status vector if any.
 *
 **************************************/
	TEXT msg[BUFFER_SMALL];

	if (status) {
#ifdef VMS
		if ((status >= sys_nerr || !sys_errlist[status]) &&
			status < WIN_NERR && win_errlist[status])
		{
			inet_gen_error(port, isc_network_error,
						   isc_arg_string,
						   (ISC_STATUS) port->port_connection->str_data,
						   isc_arg_gds, operation, isc_arg_string,
						   (ISC_STATUS) win_errlist[status], 0);
		}
		else
#endif
			inet_gen_error(port, isc_network_error,
						   isc_arg_string,
						   (ISC_STATUS) port->port_connection->str_data,
						   isc_arg_gds, operation, SYS_ERR, status, 0);

		SNPRINTF(msg, FB_NELEM(msg),
				 "INET/inet_error: %s errno = %d", function, status);
		gds__log(msg, 0);
	}
	else {
		/* No status value, just format the basic arguments. */

		inet_gen_error(port, isc_network_error,
					   isc_arg_string,
					   (ISC_STATUS) port->port_connection->str_data, isc_arg_gds,
					   operation, 0);
	}
}

static bool_t inet_putbytes( XDR* xdrs, const SCHAR* buff, u_int count)
{
/**************************************
 *
 *	i n e t _ p u t b y t e s
 *
 **************************************
 *
 * Functional description
 *	Put a bunch of bytes to a memory stream if it fits.
 *
 **************************************/
	SLONG bytecount = count;

/* Use memcpy to optimize bulk transfers. */

	while (bytecount > (SLONG) sizeof(ISC_QUAD)) {
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
			if (!inet_write(xdrs, 0))
				return FALSE;
		}
	}

/* Scalar values and bulk transfer remainder fall thru
   to be moved byte-by-byte to avoid memcpy setup costs. */

	if (!bytecount)
		return TRUE;

	if (xdrs->x_handy >= bytecount) {
		xdrs->x_handy -= bytecount;
		do {
			*xdrs->x_private++ = *buff++;
		} while (--bytecount);
		return TRUE;
	}

	while (--bytecount >= 0) {
		if (xdrs->x_handy <= 0 && !inet_write(xdrs, 0))
			return FALSE;
		--xdrs->x_handy;
		*xdrs->x_private++ = *buff++;
	}

	return TRUE;
}

// CVC: It could be const SLONG* lp, but it should fit into xdr_ops' signature.
static bool_t inet_putlong( XDR* xdrs, const SLONG* lp)
{
/**************************************
 *
 *	i n e t _ p u t l o n g
 *
 **************************************
 *
 * Functional description
 *	Fetch a longword into a memory stream if it fits.
 *
 **************************************/
	const SLONG l = htonl(*lp);
	return (*xdrs->x_ops->x_putbytes) (xdrs,
									   reinterpret_cast<const char*>(AOF32L(l)),
									   4);
}

static bool_t inet_read( XDR * xdrs)
{
/**************************************
 *
 *	i n e t _ r e a d
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
	rem_port* port = (rem_port*) xdrs->x_public;
	char* p = xdrs->x_base;
	const char* const end = p + INET_remote_buffer;

/* If buffer is not completely empty, slide down what's left */

	if (xdrs->x_handy > 0) {
		memmove(p, xdrs->x_private, xdrs->x_handy);
		p += xdrs->x_handy;
	}

/* If an ACK is pending, do an ACK.  The alternative is deadlock. */

/*
if (port->port_flags & PORT_pend_ack)
    if (!packet_send (port, 0, 0))
	return FALSE;
*/

	while (true) {
		SSHORT length = end - p;
		if (!packet_receive
			(port, reinterpret_cast<UCHAR*>(p), length, &length)) {
			return FALSE;
	/***
	if (!packet_send (port, 0, 0))
	    return FALSE;
	continue;
	***/
		}
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

static bool_t inet_setpostn( XDR * xdrs, u_int bytecount)
{
/**************************************
 *
 *	i n e t _ s e t p o s t n
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

static rem_port* inet_try_connect(
							 PACKET* packet,
							 RDB rdb,
							 Firebird::PathName& file_name,
							 const TEXT* node_name, 
							 ISC_STATUS* status_vector,
							 const SCHAR* dpb, 
							 SSHORT dpb_length)
{
/**************************************
 *
 *	i n e t _ t r y _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Given a packet with formatted protocol infomation,
 *	set header information and try the connection.
 *
 *	If a connection is established, return a port block, otherwise
 *	return NULL.
 *
 **************************************/
	P_CNCT* cnct = &packet->p_cnct;
	packet->p_operation = op_connect;
	cnct->p_cnct_operation = op_attach;
	cnct->p_cnct_cversion = CONNECT_VERSION2;
	cnct->p_cnct_client = ARCHITECTURE;
	cnct->p_cnct_file.cstr_length = file_name.length();
	cnct->p_cnct_file.cstr_address = 
		reinterpret_cast<UCHAR*>(file_name.begin());

/* If we can't talk to a server, punt.  Let somebody else generate
   an error.  status_vector will have the network error info. */

	rem_port* port =
		INET_connect(node_name, packet, status_vector, FALSE, dpb,
					 dpb_length);
	if (!port) {
		ALLR_release((BLK) rdb);
		return NULL;
	}

/* Get response packet from server. */

	rdb->rdb_port = port;
	port->port_context = rdb;
	if (!port->receive(packet)) {
		inet_error(port, "receive in try_connect", isc_net_connect_err,
				   INET_ERRNO);
		disconnect(port);
		ALLR_release((BLK) rdb);
		return NULL;
	}

	return port;
}

static bool_t inet_write( XDR * xdrs, bool_t end_flag)
{
/**************************************
 *
 *	i n e t _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a buffer full of data.  If the end_flag isn't set, indicate
 *	that the buffer is a fragment, and reset the XDR for another buffer
 *	load.
 *
 **************************************/
/* Encode the data portion of the packet */

	rem_port* port = (rem_port*) xdrs->x_public;
	const char* p = xdrs->x_base;
	SSHORT length = xdrs->x_private - p;

/* Send data in manageable hunks.  If a packet is partial, indicate
   that with a negative length.  A positive length marks the end. */

	//p = xdrs->x_base; redundant

	while (length) {
		port->port_misc1 = (port->port_misc1 + 1) % MAX_SEQUENCE;
		const SSHORT l = (SSHORT) MIN(length, INET_max_data);
		length -= l;
		if (!packet_send(port, p, (SSHORT) ((length) ? -l : l)))
			return FALSE;
		p += l;
	}

	xdrs->x_private = xdrs->x_base;
	xdrs->x_handy = INET_remote_buffer;

	return TRUE;

#ifdef PIGGYBACK
// CVC: Screwed logic here: if I initialize l2 to zero, nothing useful executes.
	SCHAR aux_buffer[BUFFER_SIZE];
	SSHORT l2 = 0;
#error Assign l2 some meaningful value before running this.
/* If the other end has not piggy-backed the next packet, we're done. */

	if (!l2)
		return TRUE;

/* We've got a piggy-backed response.  If the packet is partial,
   send an ACK for part we did receive. */

	char* p2 = aux_buffer;

	while (l2 < 0) {
		if (!packet_send(port, 0, 0))
			return FALSE;
		p2 -= l2;
		length = aux_buffer + sizeof(aux_buffer) - p2;
		if (!packet_receive(port, p2, length, &l2)) {
			p2 += l2;
			continue;
		}
	}

	length = p2 - aux_buffer + l2;

/* Now we're got a encode glump ready to stuff into the read buffer.
   Unfortunately, if we just add it to the read buffer, we will shortly
   overflow the buffer.  To avoid this, "scrumpf down" the active bits
   in the read buffer, then add out stuff at the end. */

	xdrs = &port->port_receive;
	p2 = xdrs->x_base;

	if (xdrs->x_handy && p2 != xdrs->x_private) {
		memmove(p2, xdrs->x_private, xdrs->x_handy);
	}

	p2 += xdrs->x_handy;

	xdrs->x_private = xdrs->x_base;
/*
xdrs->x_handy += JAP_decode (aux_buffer, length, p2);
*/
	port->port_flags |= PORT_pend_ack;

	return TRUE;
#endif
}

#ifdef DEBUG
static void packet_print(
						 const TEXT* string,
						 const UCHAR* packet, int length, int counter)
{
/**************************************
 *
 *	p a c k e t _ p r i n t
 *
 **************************************
 *
 * Functional description
 *	Print a summary of packet.
 *
 **************************************/
	int sum = 0;
	int l = length;
	if (l)
		do {
			sum += *packet++;
		} while (--l);

	fprintf(stdout,
			   "%05lu:    PKT %s\t(%4d): length = %4d, checksum = %d\n",
			   inet_debug_timer(), string, counter, length, sum);
	fflush(stdout);
}
#endif

static int packet_receive(
						  rem_port* port,
						  UCHAR* buffer,
						  SSHORT buffer_length, SSHORT* length)
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
#ifndef REQUESTER
/* set the time interval for sending dummy packets to the client */

	timeval timeout;
	timeout.tv_sec = port->port_dummy_packet_interval;
	timeout.tv_usec = 0;

	timeval* time_ptr = NULL;
	if (port->port_protocol >= PROTOCOL_VERSION8 &&
		port->port_dummy_packet_interval > 0)
	{
		time_ptr = &timeout;
	}

/* If the protocol is 0 we are still in the process of establishing
   a connection. Add a time out to the wait */
	if (port->port_protocol == 0)
	{
		timeout.tv_sec = port->port_connect_timeout;
		time_ptr = &timeout;
	}

	// on Linux systems (and possibly others too) select will eventually
	// change timout values so save it here for later reuse.
	// Thanks to Brad Pepers who reported this bug  FSG 3 MAY 2001
	const timeval savetime = timeout;

	int ph = (int) port->port_handle;

	// Unsed to send a dummy packet, but too big to be defined in the loop.
	PACKET packet;
#endif

#ifdef VMS
	if (ISC_tcp_gettype() == TCP_TYPE_MULTINET)
	{
		ph = ph / 16;
	}
#endif

	int n = 0;

	for (;;)
	{

#ifndef REQUESTER

		/* Implement an error-detection protocol to ensure that the client
		   is still there.  Use the select() call with a timeout to wait on
		   the connection for an incoming packet.  If none comes within a
		   suitable time interval, write a dummy packet on the connection.
		   If the client is not there, an error will be returned on the write.
		   If the client is there, the dummy packet will be ignored by all
		   InterBase clients V4 or greater.  This protocol will detect when
		   clients are lost abnormally through reboot or network disconnect. */

		/* Don't send op_dummy packets on aux port; the server won't
		   read them because it only writes to aux ports. */

		if (port->port_flags & PORT_async)
		{
			;
		}
		else
#ifdef VMS
		if (ISC_tcp_gettype() == TCP_TYPE_MULTINET)
#endif
		{
			fd_set slct_fdset;
			FD_ZERO(&slct_fdset);
			FD_SET(ph, &slct_fdset);

			THREAD_EXIT();
			int slct_count;
			for (;;) {
#if (defined WIN_NT)
				slct_count = select(FD_SETSIZE, &slct_fdset,
									NULL, NULL, time_ptr);
#else
				slct_count =
					select((SOCKET) port->port_handle + 1, &slct_fdset,
						   NULL, NULL, time_ptr);
#endif

				// restore original timeout value FSG 3 MAY 2001
				timeout = savetime;

				if (slct_count != -1 || !INTERRUPT_ERROR(INET_ERRNO))
				{
					break;
				}
			}
			THREAD_ENTER();

			if (slct_count == -1)
			{
				inet_error(port, "select in packet_receive",
						   isc_net_read_err, INET_ERRNO);
				return FALSE;
			}

			if (!slct_count && port->port_protocol >= PROTOCOL_VERSION8)
			{
#ifdef DEBUG
				if (INET_trace & TRACE_operations)
				{
					fprintf(stdout, "%05lu: OP Sent: op_dummy\n",
							   inet_debug_timer());
					fflush(stdout);
				}
#endif
				packet.p_operation = op_dummy;
				if (!send_full(port, &packet))
				{
					return FALSE;
				}
				continue;
			}

			if (!slct_count && port->port_protocol == 0)
			{
				return FALSE;
			}
		}
#endif /* REQUESTER */

		THREAD_EXIT();
		n =
			recv((SOCKET) port->port_handle,
				 reinterpret_cast<char*>(buffer), buffer_length, 0);
		THREAD_ENTER();
		if (n != -1 || !INTERRUPT_ERROR(INET_ERRNO))
			break;
	}

	if (n == -1) {
		inet_error(port, "read", isc_net_read_err, INET_ERRNO);
		return FALSE;
	}

	if (!n) {
		inet_error(port, "read end_of_file", isc_net_read_err, 0);
		return FALSE;
	}

#ifndef REQUESTER
#ifdef DEBUG
	{
		INET_count_recv++;
		INET_bytes_recv += n;
		if (INET_trace & TRACE_packets)
			packet_print("receive", buffer, n, INET_count_recv);
		INET_force_error--;
		if (INET_force_error == 0) {
			INET_force_error = 1;
			inet_error(port, "simulated error - read",
					   isc_net_read_err, 0);
			return FALSE;
		}
	}
#endif
#endif

	*length = n;

	return TRUE;
}

static bool_t packet_send( rem_port* port, const SCHAR* buffer, SSHORT buffer_length)
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
#ifdef HAVE_SETITIMER
	struct itimerval internal_timer, client_timer;
	struct sigaction internal_handler, client_handler;
#endif /* HAVE_SETITIMER */

#ifdef SINIXZ
// Please systems with ill-defined send() function, like SINIX-Z.
	char* data = const_cast<char*>(buffer);
#else
	const char* data = buffer;
#endif
	SSHORT length = buffer_length;

	while (length) {
		THREAD_EXIT();
#ifdef DEBUG
		if (INET_trace & TRACE_operations) {
			fprintf(stdout, "Before Send\n");
			fflush(stdout);
		}
#endif
		SSHORT n = -1;
		n = send((SOCKET) port->port_handle, data, length, 0);
#ifdef DEBUG
		if (INET_trace & TRACE_operations) {
			fprintf(stdout, "After Send n is %d\n", n);
			fflush(stdout);
		}
#endif
		THREAD_ENTER();
		if (n == length) {
			break;
		}

		if (n == -1)
		{
			if (INTERRUPT_ERROR(INET_ERRNO)) {
				continue;
			}

			inet_error(port, "send", isc_net_write_err, INET_ERRNO);
			return FALSE;
		}

		data += n;
		length -= n;
	}

	if ((port->port_flags & PORT_async) && !(port->port_flags & PORT_no_oob))
	{
		THREAD_EXIT();
		int count = 0;
		SSHORT n;
#ifdef SINIXZ
		char* b = const_cast<char*>(buffer);
#else
		const char* b = buffer;
#endif
		while ((n = send((SOCKET) port->port_handle, b, 1, MSG_OOB)) == -1 &&
				(INET_ERRNO == ENOBUFS || INTERRUPT_ERROR(INET_ERRNO)))
		{
			if (count++ > 20) {
				break;
			}

#ifndef HAVE_SETITIMER
#ifdef WIN_NT
			SleepEx(50, TRUE);
#else
			sleep(1);
#endif
		} // end of while() loop for systems without setitimer.
#else /* HAVE_SETITIMER */
			if (count == 1)
			{
				/* Wait in a loop until the lock becomes available */

				internal_timer.it_interval.tv_sec = 0;
				internal_timer.it_interval.tv_usec = 0;
				internal_timer.it_value.tv_sec = 0;
				internal_timer.it_value.tv_usec = 0;
				setitimer(ITIMER_REAL, &internal_timer, &client_timer);
				internal_handler.sa_handler = alarm_handler;
				sigemptyset(&internal_handler.sa_mask);
				internal_handler.sa_flags = SA_RESTART;
				sigaction(SIGALRM, &internal_handler, &client_handler);
			}

			internal_timer.it_value.tv_sec = 0;
			internal_timer.it_value.tv_usec = 50000;
			setitimer(ITIMER_REAL, &internal_timer, NULL);
			pause();
		} // end of while() loop for systems with setitimer

		if (count)
		{
			/* Restore user's outstanding alarm request and handler */

			internal_timer.it_value.tv_sec = 0;
			internal_timer.it_value.tv_usec = 0;
			setitimer(ITIMER_REAL, &internal_timer, NULL);
			sigaction(SIGALRM, &client_handler, NULL);
			setitimer(ITIMER_REAL, &client_timer, NULL);
		}
#endif /* HAVE_SETITIMER */

		THREAD_ENTER();
		if (n == -1) {
			inet_error(port, "send/oob", isc_net_write_err, INET_ERRNO);
			return FALSE;
		}
	}

#ifndef REQUESTER
#ifdef DEBUG
	{
		INET_count_send++;
		INET_bytes_send += buffer_length;
		if (INET_trace & TRACE_packets)
			packet_print("send", (const UCHAR*) buffer, buffer_length, INET_count_send);
		INET_force_error--;
		if (INET_force_error == 0) {
			INET_force_error = 1;
			inet_error(port, "simulated error - send",
					   isc_net_write_err, 0);
			return FALSE;
		}
	}
#endif
#endif

	port->port_flags &= ~PORT_pend_ack;

	return TRUE;
}

static void unhook_port( rem_port* port, rem_port* parent)
{
/**************************************
 *
 *      u n h o o k _ p a r e n t
 *
 **************************************
 *
 * Functional description
 *      Disconnect a port from its parent
 *      This must be done under
 *      START_PORT_CRITICAL/STOP_PORT_CRITICAL control.
 *
 **************************************/

	for (rem_port** ptr = &parent->port_clients; *ptr; ptr = &(*ptr)->port_next) {
		if (*ptr == port) {
			*ptr = port->port_next;
			if (ptr == &parent->port_clients) {
				parent->port_next = *ptr;
			}
			break;
		}
	}
}

#ifdef  SUPERSERVER
static void unhook_disconnected_ports(rem_port* main_port)
{
/**************************************
 *
 *      u n h o o k _ d i s c o n n e c t e d _ p o r t s
 *
 **************************************
 *
 * Functional description
 *      Go through a list of ports and get rid of any
 *      that are disconnected.   Note that this must be
 *      done under START_PORT_CRITICAL/STOP_PORT_CRITICAL
 *      control.
 *
 **************************************/

	bool more = true;

	while (more) {
		more = false;
		for (rem_port* port = main_port; port; port = port->port_next) {
			if (port->port_state == state_disconnected) {
				more = true;
				unhook_port(port, port->port_parent);
				cleanup_port(port);
				break;
			}
		}
	}
}
#endif


