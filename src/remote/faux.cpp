/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		inet.c
 *	DESCRIPTION:	TCP/IP stub function substitution
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
#ifdef NETWARE_386
#include <stddef.h>
#include <advanced.h>
#include <string.h>

static int dont_gethostname(char *unused1, int unused2);
static struct hostent *dont_gethostbyname(char *unused1);
static struct servent *dont_getservbyname(char *unused1, int unused2);

int (*FAUX_gethostname_var) ();
struct servent *(*FAUX_getservbyname_var) ();
struct hostent *(*FAUX_gethostbyname_var) ();

int dont_gethostname( char *unused1, int unused2)
{
/**************************************
 *
 *	N O T g e t h o s t n a m e
 *
 **************************************
 *
 * Functional description
 *	Fail a gethostname function.
 *
 **************************************/
	return -1;
}

struct hostent *dont_gethostbyname(char *unused1)
{
/**************************************
 *
 *	N O T g e t h o s t b y n a m e
 *
 **************************************
 *
 * Functional description
 *	Fail the gethostbyname function.
 *
 **************************************/
	return NULL;
}

struct servent *dont_getservbyname(char *unused1, int unused2)
{
/**************************************
 *
 *	N O T g e t s e r v b y n a m e
 *
 **************************************
 *
 * Functional description
 *	Fail a getservbyname function.
 *
 **************************************/
	return NULL;
}

int FAUX_check(void)
{
/**************************************
 *
 *	F A U X _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Determine if TCP/IP is loaded.  If TCP/IP is loaded, set up
 *	the FAUX calls to call directly to the appropriate functions.
 *      If TCP/IP is *NOT* loaded, point all calls to dummy functions
 *      and return -1 to indicate the lack of TCP/IP.
 *
 **************************************/
	int MyNLMHandle;
	unsigned long tcpip_thereP;

	MyNLMHandle = GetNLMHandle();
	tcpip_thereP = (unsigned long) ImportSymbol(MyNLMHandle, "gethostname");

	if (tcpip_thereP) {
		FAUX_gethostname_var = ImportSymbol(MyNLMHandle, "gethostname");
		FAUX_gethostbyname_var = ImportSymbol(MyNLMHandle, "NWgethostbyname");
		FAUX_getservbyname_var = ImportSymbol(MyNLMHandle, "NWgetservbyname");
		return (0);
	}
	else {
		FAUX_gethostname_var = &dont_gethostname;
		FAUX_gethostbyname_var = &dont_gethostbyname;
		FAUX_getservbyname_var = &dont_getservbyname;
		return (-1);
	}
}
#endif
