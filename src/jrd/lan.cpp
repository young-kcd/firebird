/* 
 *	PROGRAM:	JRD Access Method
 *	MODULE:		lan.c
 *	DESCRIPTION:	OS/2 LAN manager-specific JRD routines
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
#include "../jrd/common.h"
#include <os2.h>

#define LAN_SERVERGETINFO	0
#define PROC_count		1

typedef struct {
	TEXT *module;
	TEXT *name;
	PFN address;
} LANENTRY;

#define ENTRYNAME(module, name, address)	module, name, NULL,

static LANENTRY lanentrypoints[PROC_count] = {
	ENTRYNAME("NETAPI", "NETSERVERGETINFO", NETSERVERGETINFO)
};

#define CALL(proc)  (lanentrypoints[proc].address)


int gethostname(SCHAR * buffer, USHORT buflen)
{
/***********************************************
*
*	g e t h o s t n a m e			( M S D O S )
*
************************************************
*
* Functional description
*	This function provides symmetry under OS/2
*	for the UNIX gethostname system call.
*
*	It first checks to see if the LANMAN library is
*	accessible.  If not, it returns a null string.  If it
*	is, it calls the NetServerGetInfo system call to provide
*	the host name.
*
************************************************/
	USHORT totalavail;
	LANENTRY *entry;

	(*entry) = lanentrypoints[0];
	if (!
		(entry->address =
		 (PFN) ISC_lookup_entrypoint(entry->module, entry->name[0], NULL))) {
		buffer[0] = '\0';
	}
	else {
		CALL(LAN_SERVERGETINFO) (NULL, 0, buffer, buflen, &totalavail);
		buffer[totalavail] = '\0';
	}

	return 0;
}
