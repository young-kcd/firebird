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
#include libdtdef.h
#include "../jrd/common.h"
#include "../remote/tcptypes.h"

#ifndef	__ALPHA
#include "zenda::multinet_root:[multinet.include.sys]types.h"
#include "zenda::multinet_root:[multinet.include.sys]time.h"
#include "zenda::multinet_root:[multinet.include.sys]socket.h"
#include "zenda::multinet_root:[multinet.include.netinet]in.h"
#include "zenda::multinet_root:[multinet.include]errno.h"
#include "zenda::multinet_root:[multinet.include]netdb.h"
#include "zenda::multinet_root:[multinet.include.vms]inetiodef.h"
#else
#include "multinet_root:[multinet.include.sys]types.h"
#include "multinet_root:[multinet.include.sys]time.h"
#include "multinet_root:[multinet.include.sys]socket.h"
#include "multinet_root:[multinet.include.netinet]in.h"
#include "multinet_root:[multinet.include]errno.h"
#include "multinet_root:[multinet.include]netdb.h"
#include "multinet_root:[multinet.include.vms]inetiodef.h"
#endif

#define	SELECT_READABLE		2
#define	SELECT_WRITEABLE	4
#define	SELECT_EXCEPTION	8

#define ERRNO(status)	(0x7fff & status) >> 3
#define INET0		"_INET0:"
#define EVENT_FLAG	15
#define MAXH_ERRNO	4
/* extern			gds__completion_ast(); */

extern int h_errno;

static int assign(SCHAR *);
static int error_check(int, SSHORT *);
static void select_completion_ast(int *);

static (*isc_wait_ptr) ();
static (*completion_ptr) ();

static int h_errlist[] = {
	0,
	60,							/* host not found = connection timed out */
	64,							/* server fail = host is down */
	53,							/* no recovery = connection aborted */
	65							/* no data = no route to host */
};

static int numsel = 0;			/* number of outstanding selects */


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

	if (!(host = gethostbyname(name)))
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
	struct hostent *h;

/* return gethostbyaddr (address, length, family); */

	if (!(h = gethostbyaddr(address, length, family)) &&
		(0 < h_errno) && (h_errno <= MAXH_ERRNO))
		errno = h_errlist[h_errno];

	return h;
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
	struct hostent *h;

/* return gethostbyname (name); */

	if (!(h = gethostbyname(name)) &&
		(0 < h_errno) && (h_errno <= MAXH_ERRNO)) errno = h_errlist[h_errno];

	return h;
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

	return gethostname(name, namelen);
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

	strcpy(filename, "MULTINET:HOSTS.EQUIV");
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

	if (!(host = gethostbyaddr(&(addr->sin_addr), sizeof(addr->sin_addr),
							   addr->sin_family))) return -1;

	strcpy(name, host->h_name);
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

	return getpeername(s, name, namelen);
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

	return getservbyname(name, proto);
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

	if (!(service = getservbyname(name, proto)))
		return -1;

	*portnum = service->s_port;
	return 0;
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

	return getsockname(s, name, namelen);
}


int ISC_tcp_gettype(void)
{
/**************************************
 *
 *	I S C _ t c p _ g e t t y p e
 *
 **************************************
 *
 *	Return a value to the caller
 *	saying this is multinet
 *
 **************************************/

	return TCP_TYPE_MULTINET;
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
				   fd_set * read,
				   fd_set * write, fd_set * except, struct timeval *timeout)
{
/**************************************
 *
 *	I S C _ t c p _ s e l e c t
 *
 **************************************
 *
 * Functional description
 *	Synchronous I/O multiplexing.
 *	This function starts up as many
 *	select qio's as there are channels
 *	specified.   The ast gathers the
 *	resulting information.   This also
 *	sets a timer, and when the timer
 *	expires, it cancels any outstanding
 *	requests.
 *
 **************************************/
	int channel;
	int numchan;
	int numresult;
	SLONG results[FD_SETSIZE];
	int mode;
	int status;
	int timer[2];
	int seconds;
	int action;
	SSHORT iosb[4];

/* initialize all the static stuff used by the ast */

	numsel = 0;

/* for each channel, see if it requires any I/O, and if so, clear the
   applicable fd_set bit, set up a qio, and issue it */

	numchan = 0;
	for (channel = 0; channel < width && channel < FD_SETSIZE; channel++) {
		results[channel] = 0;
		if (read && FD_ISSET(channel, read)) {
			results[channel] |= SELECT_READABLE;
		}
		if (write && FD_ISSET(channel, write)) {
			results[channel] |= SELECT_WRITEABLE;
		}
		if (except && FD_ISSET(channel, except)) {
			results[channel] |= SELECT_EXCEPTION;
		}
		if (results[channel]) {
			numchan++;
			numsel++;
			status = sys$qio(EVENT_FLAG, channel, IO$_SELECT, iosb, select_completion_ast, channel,	/* ast argument */
							 &results[channel],	/* p1 */
							 0, 0, 0, 0, 0);
			if (!(status & 1))
				return -1;
		}
	}

/* if there's a timeout specified, start a timer */

	if (timeout) {
		/* convert any timeout to vms timer */

		mode = LIB$K_DELTA_SECONDS;
		seconds = timeout->tv_sec;
		lib$cvt_to_internal_time(&mode, &seconds, &timer);
		mode = timer[0] & 0x80000000;
		timer[0] = timer[0] - (timeout->tv_usec * 10);
		if (!mode && (timer[0] & 0x80000000))
			timer[1]--;
		sys$schdwk(NULL, NULL, &timer, NULL);
	}

/* wait for completion or timeout */

	sys$hiber();

/* for each channel, see if there was anything to do, and if so,
   find the result and transfer it to the outgoing fd sets */

	numresult = 0;
	for (channel = 0; channel < width && channel < FD_SETSIZE; channel++) {
		mode = 0;
		action = 0;
		if (read && FD_ISSET(channel, read)) {
			mode++;
			if (results[channel] & SELECT_READABLE)
				action++;
			else
				FD_CLR(channel, read);
		}
		if (write && FD_ISSET(channel, write)) {
			mode++;
			if (results[channel] & SELECT_WRITEABLE)
				action++;
			else
				FD_CLR(channel, write);
		}
		if (except && FD_ISSET(channel, except)) {
			mode++;
			if (results[channel] & SELECT_EXCEPTION)
				action++;
			else
				FD_CLR(channel, except);
		}
		if (mode) {
			sys$cancel(channel);
			if (action)
				numresult++;
		}
	}

	return numresult;
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
 *	Fill in routine pointers (so we're not gdsshr-dependents!)
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
 *	an error occured, set up "errno" and return FAILURE.
 *
 **************************************/

	if (!(status & 1)) {
		errno = 5;
		return FAILURE;
	}

	if (!iosb || (iosb[0] & 1))
		return FBOK;

	errno = ERRNO(iosb[0]);

	return FAILURE;
}


static void select_completion_ast( int *arg)
{
/**************************************
 *
 *      s e l e c t _ c o m p l e t i o n _ a s t
 *
 **************************************
 *
 * Functional description
 *      An asynchronous operation has completed.
 *	If this was the last outstanding I/O,
 *	wake up.
 *
 **************************************/

	numsel--;
	if (!numsel)
		sys$wake();
}
