/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		winvx.c
 *	DESCRIPTION:	Interface to Wollongong TCP
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
#include stdio
#include iodef
#include descrip

#ifdef __STDDEF_LOADED			/* Wollongong expects VAX C 3.0 style */
#define __STDDEF				/* include symbols to be defined */
#endif

#include "../jrd/common.h"
#include "../remote/tcptypes.h"
#include "twg$tcp:[netdist.include]errno.h"
#include "twg$tcp:[netdist.include.sys]types.h"
#include "twg$tcp:[netdist.include.vms]inetiodef.h"
#include "twg$tcp:[netdist.include.sys]socket.h"
#include "twg$tcp:[netdist.include]netdb.h"
#include "../remote/in.h"

#define ERRNO(status)	(0x7fff & status) >> 3
#define INET0		"_INET0:"
#define HOST_FILE	"twg$tcp:[netdist.etc]hosts."
#define HOSTS_EQUIV	"twg$tcp:[netdist.etc]hosts.equiv"
#define SERVICE_FILE	"twg$tcp:[netdist.etc]services."
#define HOST_NAME	"ARPANET_HOST_NAME"
#define EVENT_FLAG	15

#ifndef MAXHOSTLEN
#define MAXHOSTLEN	64
#endif

#ifndef	__ALPHA
extern volatile int noshare errno;
#endif

extern struct hostent *ISC_tcp_gethostbyname(struct in_addr *, int, int);
extern struct servent *ISC_tcp_getservbyname(SCHAR *, SCHAR *);

static int assign(SCHAR *);
static void bcopy(SCHAR *, SCHAR *, int);
static int error_check(int, SSHORT *);
static struct hostent *get_host(IB_FILE *);
static int get_line(IB_FILE *, SCHAR *);

static struct servent local_service;
static SCHAR local_login[64], service_name[16], proto_name[16],
	host_name[MAXHOSTLEN];
static struct hostent local_host;
static struct in_addr local_address;
static struct in_addr *local_address_list[] = { &local_address };
static (*isc_wait_ptr) ();
static (*completion_ptr) ();


int ISC_tcp_accept( int s, struct sockaddr *addr, int addrlen)
{
/**************************************
 *
 *	I S C _ t c p _ a c c e p t
 *
 **************************************
 *
 * Functional description
 *	Accept a connection on a socket.
 *
 **************************************/
	int status, new;
	SSHORT iosb[4];
	struct {
		SSHORT hack_length;
		struct sockaddr_in hack_address;
	} hack;

/* Wait for incoming connection. */

	status = sys$qio(EVENT_FLAG,
					 s,
					 IO$_ACCEPT_WAIT,
					 iosb, completion_ptr, iosb, 0, 0, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

/* Establish new connection */

	if (!(new = assign(INET0)))
		return -1;

/* Attach new connection to new channel */

	status = sys$qio(EVENT_FLAG, new, IO$_ACCEPT, iosb, completion_ptr, iosb,
					 &hack.hack_length,
					 sizeof(hack.hack_length) + sizeof(hack.hack_address),
					 s, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return new;
}


int ISC_tcp_bind( int s, struct sockaddr *name, int namelen)
{
/**************************************
 *
 *	I S C _ t c p _ b i n d
 *
 **************************************
 *
 * Functional description
 *	Bind a name to a socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG, s, IO$_BIND, iosb, completion_ptr, iosb,
					 name, namelen, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


int ISC_tcp_connect( int s, struct sockaddr *name, int namelen)
{
/**************************************
 *
 *	I S C _ t c p _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Initiate a connection on a socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG, s, IO$_CONNECT, iosb, completion_ptr, iosb,
					 name, namelen, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


int ISC_tcp_getaddr( SCHAR * name, struct sockaddr_in *addr)
{
/**************************************
 *
 *	I S C _ t c p _ g e t a d d r
 *
 **************************************
 *
 * Functional description
 *	Load sockaddr based on hostname name.
 *
 **************************************/
	struct hostent *host;

	if (!(host = ISC_tcp_gethostbyname(name)))
		return -1;

	addr->sin_family = host->h_addrtype;
	bcopy(host->h_addr, &(addr->sin_addr), sizeof(addr->sin_addr));
	return 0;
}


int ISC_tcp_geterrno(void)
{
/**************************************
 *
 *	I S C _ t c p _ g e t e r r n o
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return errno;
}


struct hostent *ISC_tcp_gethostbyaddr(struct in_addr *address,
									  int length, int family)
{
/**************************************
 *
 *	I S C _ t c p _ g e t h o s t b y a d d r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IB_FILE *file;
	struct hostent *host;
	struct in_addr *test;

	if (!(file = ib_fopen(HOST_FILE, "r")))
		return NULL;

	while (host = get_host(file)) {
		test = host->h_addr;
		if (host->h_addrtype == family &&
			test->S_un.S_un_b.s_b1 == address->S_un.S_un_b.s_b1 &&
			test->S_un.S_un_b.s_b2 == address->S_un.S_un_b.s_b2 &&
			test->S_un.S_un_b.s_b3 == address->S_un.S_un_b.s_b3 &&
			test->S_un.S_un_b.s_b4 == address->S_un.S_un_b.s_b4)
			break;
	}

	ib_fclose(file);

	return host;
}


struct hostent *ISC_tcp_gethostbyname(SCHAR * name)
{
/**************************************
 *
 *	I S C _ t c p _ g e t h o s t b y n a m e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	IB_FILE *file;
	struct hostent *host;

	if (!(file = ib_fopen(HOST_FILE, "r")))
		return NULL;

	while (host = get_host(file))
		if (!strcmp(name, host->h_name))
			break;

	ib_fclose(file);

	return host;
}


int ISC_tcp_gethostname( SCHAR * name, int namelen)
{
/**************************************
 *
 *	I S C _ t c p _ g e t h o s t n a m e
 *
 **************************************
 *
 * Functional description
 *	Get host name.
 *
 **************************************/
	struct dsc$descriptor_s in, out;
	int status;
	SSHORT length;

	in.dsc$b_class = DSC$K_CLASS_S;
	in.dsc$b_dtype = DSC$K_DTYPE_T;
	in.dsc$w_length = sizeof(HOST_NAME) - 1;
	in.dsc$a_pointer = HOST_NAME;

	out.dsc$b_class = DSC$K_CLASS_S;
	out.dsc$b_dtype = DSC$K_DTYPE_T;
	out.dsc$w_length = namelen;
	out.dsc$a_pointer = name;

	status = lib$sys_trnlog(&in, &length, &out, NULL, NULL, NULL);
	if (status & 1) {
		if (length < namelen)
			name[length] = 0;
		return 0;
	}

	return -1;
}


void ISC_tcp_gethosts( SCHAR * filename)
{
/**************************************
 *
 *	I S C _ t c p _ g e t h o s t s
 *
 **************************************
 *
 * Functional description
 *	Get hosts.equiv filename.
 *
 **************************************/

	strcpy(filename, HOSTS_EQUIV);
}


int ISC_tcp_getnamebyaddr(
						  struct sockaddr_in *addr, int addrlen, SCHAR * name)
{
/**************************************
 *
 *	I S C _ t c p _ g e t n a m e b y a d d r
 *
 **************************************
 *
 * Functional description
 *	Get hostname that matches address.
 *
 **************************************/
	struct hostent *host;

	if (!
		(host =
		 ISC_tcp_gethostbyaddr(&(addr->sin_addr), sizeof(addr->sin_addr),
							   addr->sin_family))) return -1;

	strcpy(name, host->h_name);
	return 0;
}


int ISC_tcp_getservport( SCHAR * name, SCHAR * proto, SSHORT * portnum)
{
/**************************************
 *
 *	I S C _ t c p _ g e t s e r v p o r t
 *
 **************************************
 *
 * Functional description
 *	Look up a protocol by name.
 *
 **************************************/
	struct servent *service;

	if (!(service = ISC_tcp_getservbyname(name, proto)))
		return -1;

	*portnum = service->s_port;
	return 0;
}


int ISC_tcp_getpeername( int s, struct sockaddr *name, int *namelen)
{
/**************************************
 *
 *	I S C _ t c p _ g e t p e e r n a m e
 *
 **************************************
 *
 * Functional description
 *	Get name (i.e. port) of peer.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG,	/* Event flag */
					 s,			/* Channel */
					 IO$_GETPEERNAME,	/* Function */
					 iosb,		/* I/O status block */
					 completion_ptr,	/* AST address */
					 iosb,		/* AST parameter */
					 name,		/* p1 */
					 namelen,	/* p2 */
					 0, 0, 0, 0);	/* p3, p4, p5, p6 */

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


struct servent *ISC_tcp_getservbyname(SCHAR * name, SCHAR * proto)
{
/**************************************
 *
 *	I S C _ t c p _ g e t s e r v b y n a m e
 *
 **************************************
 *
 * Functional description
 *	Look up a protocol by name.
 *
 **************************************/
	IB_FILE *file;
	int port;
	SCHAR *p, comment, buffer[80], local_serv_name[sizeof(buffer)], *end, c;

	if (!(file = ib_fopen(SERVICE_FILE, "r")))
		return NULL;

/* Make sure that the service name to match is in lower case */

	for (p = local_serv_name, end = p + sizeof(local_serv_name) - 1;
		 (c = *name++) && p < end;)
		*p++ = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
	*p = 0;

	while (get_line(file, buffer)) {
		if (sscanf(buffer, "%s%d/%s", service_name, &port, proto_name) < 3 ||
			strcmp(service_name, local_serv_name) ||
			(proto && strcmp(proto_name, proto)))
			continue;

		local_service.s_name = service_name;
		local_service.s_port = (port >> 8) + ((port & 0xff) << 8);
		local_service.s_proto = proto_name;
		ib_fclose(file);
		return &local_service;
	}

	ib_fclose(file);
	return NULL;
}


int ISC_tcp_getsockname( int s, struct sockaddr *name, int *namelen)
{
/**************************************
 *
 *	I S C _ t c p _ g e t s o c k n a m e
 *
 **************************************
 *
 * Functional description
 *	Get name (i.e. port) of socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG,	/* Event flag */
					 s,			/* Channel */
					 IO$_GETSOCKNAME,	/* Function */
					 iosb,		/* I/O status block */
					 completion_ptr,	/* AST address */
					 iosb,		/* AST parameter */
					 name,		/* p1 */
					 namelen,	/* p2 */
					 0, 0, 0, 0);	/* p3, p4, p5, p6 */

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


int ISC_tcp_gettype(void)
{
/**************************************
 *
 *	I S C _ t c p _ g e t t y p e
 *
 **************************************
 *
 *	Return a type value so the caller
 *	knows what this product is
 *
 **************************************/

	return TCP_TYPE_WOLLONGONG;
}


int ISC_tcp_listen( int s, int backlog)
{
/**************************************
 *
 *	I S C _ t c p _ l i s t e n
 *
 **************************************
 *
 * Functional description
 *	Listen for connections on a socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG, s, IO$_LISTEN, iosb, completion_ptr, iosb,
					 backlog, 0, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


int ISC_tcp_recv( int s, SCHAR * buf, int len, int flags)
{
/**************************************
 *
 *	I S C _ t c p _ r e c v
 *
 **************************************
 *
 * Functional description
 *	Receive a message  from a socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG, s, IO$_READVBLK, iosb, completion_ptr, iosb,
					 buf, len, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return iosb[1];
}


int ISC_tcp_select(
				   int width,
				   int *read,
				   int *write, int *except, struct timeval *timeout)
{
/**************************************
 *
 *	I S C _ t c p _ s e l e c t
 *
 **************************************
 *
 * Functional description
 *	Synchronous I/O multiplexing.
 *
 **************************************/

	return 0;
}


int ISC_tcp_send( int s, SCHAR * buf, int len, int flags)
{
/**************************************
 *
 *	I S C _ t c p _ s e n d 
 *
 **************************************
 *
 * Functional description
 *	Send a message from a socket.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qio(EVENT_FLAG, s, IO$_WRITEVBLK, iosb, completion_ptr, iosb,	/* ast argument */
					 buf,		/* p1 */
					 len,		/* p2 */
					 flags,		/* p3 */
					 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return iosb[1];
}


int ISC_tcp_setsockopt(
					   int s,
					   int level, int optname, SCHAR * optval, int optlen)
{
/**************************************
 *
 *	I S C _ t c p _ s e t s o c k o p t
 *
 **************************************
 *
 * Functional description
 *	Set options on sockets.
 *
 **************************************/
	int status;
	SSHORT iosb[4];

	status = sys$qiow(EVENT_FLAG, s, IO$_SETSOCKOPT, iosb, completion_ptr, iosb,	/* ast argument */
					  level,	/* p1 */
					  optname,	/* p2 */
					  optval,	/* p3 */
					  optlen,	/* p4 */
					  0, 0);

	if (error_check(status, iosb))
		return -1;

	return 0;
}


void ISC_tcp_setup( int (*waitaddr) (), int (*astaddr) ())
{
/**************************************
 *
 *	I S C _ t c p _ s e t u p
 *
 **************************************
 *
 * Functional description
 *	Set options on sockets.
 *
 **************************************/

	isc_wait_ptr = waitaddr;
	completion_ptr = astaddr;
}


int ISC_tcp_socket( int af, int type, int protocol)
{
/**************************************
 *
 *	I S C _ t c p _ s o c k e t
 *
 **************************************
 *
 * Functional description
 *	Create an endpoint for communication.
 *
 **************************************/
	int s, status;
	SSHORT iosb[4];

	if (!(s = assign(INET0)))
		return -1;

	status = sys$qio(EVENT_FLAG, s, IO$_SOCKET, iosb, completion_ptr, iosb,
					 af, type, 0, 0, 0, 0);

	if (status & 1)
		(*isc_wait_ptr) (iosb, EVENT_FLAG);

	if (error_check(status, iosb))
		return -1;

	return s;
}


static int assign( SCHAR * string)
{
/**************************************
 *
 *	a s s i g n
 *
 **************************************
 *
 * Functional description
 *	Assign a channel for communications.
 *
 **************************************/
	SSHORT channel;
	int status;
	struct dsc$descriptor_s desc;

	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$w_length = strlen(string);
	desc.dsc$a_pointer = string;

	status = sys$assign(&desc, &channel, NULL, NULL);

	return (status & 1) ? channel : 0;
}


static void bcopy( SCHAR * from, SCHAR * to, int length)
{
/**************************************
 *
 *	b c o p y
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
		while (--length);
}


static int error_check( int status, SSHORT * iosb)
{
/**************************************
 *
 *	e r r o r _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Check both a return status and IOSB (if present) for errors.  If
 *	an error occured, set up "errno" and return FB_FAILURE.
 *
 **************************************/

	if (!(status & 1)) {
		errno = 5;
		return FB_FAILURE;
	}

	if (!iosb || (iosb[0] & 1))
		return FB_SUCCESS;

	errno = ERRNO(iosb[0]);

	return FB_FAILURE;
}


static struct hostent *get_host(IB_FILE * file)
{
/**************************************
 *
 *	g e t _ h o s t
 *
 **************************************
 *
 * Functional description
 *	Get the next host entry.
 *
 **************************************/
	SCHAR buffer[80];
	int n, h1, h2, h3, h4;

	while (get_line(file, buffer)) {
		n = sscanf(buffer, "%d.%d.%d.%d%s", &h1, &h2, &h3, &h4, host_name);
		if (n < 5)
			continue;
		local_host.h_name = host_name;
		local_host.h_addrtype = AF_INET;
		local_host.h_length = sizeof(local_address);
		local_host.h_addr_list = local_address_list;
		local_address.S_un.S_un_b.s_b1 = h1;
		local_address.S_un.S_un_b.s_b2 = h2;
		local_address.S_un.S_un_b.s_b3 = h3;
		local_address.S_un.S_un_b.s_b4 = h4;
		return &local_host;
	}

	return NULL;
}


static int get_line( IB_FILE * file, SCHAR * buffer)
{
/**************************************
 *
 *	g e t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Get a line from a TCP control file, stopping at comment.  Return
 *	at end of file.
 *
 **************************************/
	SSHORT c, comment;

	comment = FALSE;

	for (;;) {
		c = ib_getc(file);
		if (c == '\n')
			break;
		if (c == EOF)
			return FALSE;
		if (c == '#')
			comment = TRUE;
		else if (!comment)
			*buffer++ = c;
	}

	*buffer = 0;

	return TRUE;
}
