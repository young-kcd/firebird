/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		ucx.c
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
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <netdb.h>
#include <types.h>
#include <in.h>
#include "../jrd/common.h"

#define ETC_SERVICES	"sys$library:services"

static struct servent service;
static TEXT text[256];

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct timeval {
	SLONG tv_sec;				/* seconds */
	SLONG tv_usec;				/* and microseconds */
};


int ISC_tcp_accept( int s, struct sockaddr *addr, int *addrlen)
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

	return vaxc$accept(s, addr, addrlen);
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

	return vaxc$bind(s, name, namelen);
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

	return vaxc$connect(s, name, namelen);
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


struct hostent *ISC_tcp_gethostbyaddr(SCHAR * address, int length, int family)
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

	return vaxc$gethostbyaddr(address, length, family);
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

	return vaxc$gethostbyname(name);
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

	return vaxc$gethostname(name, namelen);
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

	strcpy(filename, "???");
}


int ISC_tcp_getpeername( int s, struct sockaddr *name, int namelen)
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

	return vaxc$getpeername(s, name, namelen);
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
	TEXT line[256], service_name[32], protocol_name[32], *p, *q;
	IB_FILE *file;
	int port;

	if (!(file = ib_fopen(ETC_SERVICES, "r")))
		return NULL;

	while (ib_fgets(line, sizeof(line), file))
		if (sscanf(line, "%s%d/%s", service_name, &port, protocol_name) == 3
			&& !strcmp(service_name, name) && !strcmp(protocol_name, proto)) {
			service.s_port = (port >> 8) + ((port & 0xff) << 8);
			service.s_name = p = text;
			for (q = protocol_name; *p++ = *q++;);
			service.s_proto = p;
			for (q = protocol_name; *p++ = *q++;);
			service.s_aliases = NULL;
			ib_fclose(file);
			return &service;
		}

	ib_fclose(file);

	return NULL;
}


int ISC_tcp_getsockname( int s, struct sockaddr *name, int namelen)
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

	return vaxc$getsockname(s, name, namelen);
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

	return vaxc$listen(s, backlog);
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

	return vaxc$recv(s, buf, len, flags);
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

	return vaxc$send(s, buf, len, flags);
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

	return vaxc$socket(af, type, protocol);
}
