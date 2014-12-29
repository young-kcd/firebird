/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Michal Kubecek
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2014 Michal Kubecek <mike@mk-sys.cz>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef REMOTE_SOCKADDR_H
#define REMOTE_SOCKADDR_H

#include <string.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifndef WIN_NT
#include <netinet/in.h>
#else

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Wspiapi.h>

#ifndef IN_LOOPBACKNET
#define IN_LOOPBACKNET 127
#endif

#endif

#include "../remote/remote.h"

class SockAddr
{
private:
	static const unsigned maxLen = sizeof(struct sockaddr_in6);
	char data[maxLen];
	socklen_t len;

public:
	void clear();
	const SockAddr& operator = (const SockAddr& x);

	SockAddr() { clear(); }
	SockAddr(const SockAddr& x) { *this = x; }
	SockAddr(const unsigned char* p_data, unsigned p_len);
	~SockAddr() {}

	struct sockaddr* ptr() { return (struct sockaddr*) data; }
	struct sockaddr_in* ptrIn() { return (struct sockaddr_in*) data; }
	struct sockaddr_in6* ptrIn6() { return (struct sockaddr_in6*) data; }

	unsigned length() const { return len; }
	unsigned short family() const;
	unsigned short port() const;
	void setPort(unsigned short x);
	int connect(SOCKET s) const;
	int accept(SOCKET s);
	int getsockname(SOCKET s);
	int getpeername(SOCKET s);
	void unmapV4();
};


inline void SockAddr::clear()
{
	len = 0;
	memset(data, 0, maxLen);
}


inline const SockAddr& SockAddr::operator = (const SockAddr& x)
{
	memcpy(data, x.data, maxLen);
	len = x.len;
	return *this;
}


inline SockAddr::SockAddr(const unsigned char* p_data, unsigned p_len)
{
	if (p_len > maxLen)
		p_len = maxLen;
	memcpy(data, p_data, p_len);
	len = p_len;
}


inline int SockAddr::connect(SOCKET s) const
{
	return ::connect(s, (struct sockaddr*) data, len);
}


inline int SockAddr::accept(SOCKET s)
{
	return os_utils::accept(s, (struct sockaddr*) data, &len);
}


inline int SockAddr::getsockname(SOCKET s)
{
	len = maxLen;
	int R = ::getsockname(s, (struct sockaddr*) data, &len);
	if (R < 0)
		clear();
	return R;
}


inline int SockAddr::getpeername(SOCKET s)
{
	len = maxLen;
	int R = ::getpeername(s, (struct sockaddr*) data, &len);
	if (R < 0)
		clear();
	return R;
}


inline unsigned short SockAddr::family() const
{
	const struct sockaddr* sa = (const struct sockaddr*) data;
	return sa->sa_family;
}


inline unsigned short SockAddr::port() const
{
	const struct sockaddr* sa = (const struct sockaddr*) data;
	switch(sa->sa_family) {
	case AF_INET:
		return ntohs(((const struct sockaddr_in*) data)->sin_port);
	case AF_INET6:
		return ntohs(((const struct sockaddr_in6*) data)->sin6_port);
	}
	return 0; // exception?
}


inline void SockAddr::setPort(unsigned short x)
{
	const struct sockaddr* sa = (const struct sockaddr*) data;
	switch(sa->sa_family) {
	case AF_INET:
		((struct sockaddr_in*) data)->sin_port = htons(x);
		return;
	case AF_INET6:
		((struct sockaddr_in6*) data)->sin6_port = htons(x);
		return;
	}
	// exception?
}


// if address is embedded IPv4, convert it to normal IPv4
inline void SockAddr::unmapV4()
{
	if (family() != AF_INET6)
		return;

	const struct sockaddr_in6* sa6 = (const struct sockaddr_in6*) data;
	// IPv6 mapped IPv4 addresses are ::ffff:0:0/32
	static const unsigned char v4mapped_pfx[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff};

	if (memcmp(sa6->sin6_addr.s6_addr, v4mapped_pfx, sizeof(v4mapped_pfx)) != 0)
		return;

	unsigned short port = ntohs(sa6->sin6_port);
	struct in_addr addr;
	memcpy(&addr, sa6->sin6_addr.s6_addr + sizeof(v4mapped_pfx), sizeof(addr));

	struct sockaddr_in* sa4 = (struct sockaddr_in*) data;
	sa4->sin_family = AF_INET;
	sa4->sin_port = htons(port);
	sa4->sin_addr.s_addr = addr.s_addr;
	len = sizeof(struct sockaddr_in);
}


#endif // REMOTE_SOCKADDR_H
