/*
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
#include <stdio.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include "../jrd/svc_undoc.h"
#include "../common/stuff.h"

int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
*
*      m a i n
*
**************************************
*
*Functional Description
*  This utility uses the interbase service api to inform the server
*  to print out the memory pool information into a specified file.
*  is utilitiy is for WIN_NT only, In case of UNIX ibmgr utility will
*  should be used.
*
*************************************************************************/
	char buffer[512];
	char fname[512];
	ISC_STATUS_ARRAY status;
	isc_svc_handle svc_handle = NULL;
	char svc_name[256];

	if (argc != 2 && argc != 1) {
		printf("Usage %s \n      %s filename\n");
		exit(1);
	}
	if (argc == 1) {
		printf(" Filename : ");
		gets(fname);
	}
	else
		strcpy(fname, argv[1]);

	strcpy(buffer, fname);
	printf("Filename to dump pool info = %s \n", buffer);
	sprintf(svc_name, "localhost:anonymous");
	if (isc_service_attach(status, 0, svc_name, &svc_handle, 0, NULL)) {
		printf("Failed to attach service\n");
		return 0;
	}

	const unsigned short path_length = strlen(buffer);

	char sendbuf[512];
	char* sptr = sendbuf;
	*sptr = isc_info_svc_dump_pool_info;
	++sptr;
	add_word(sptr, path_length);
	strcpy(sptr, buffer);
	sptr += path_length;

	char respbuf[256];
	if (isc_service_query
		(status, &svc_handle, NULL, 0, NULL, sptr - sendbuf, sendbuf, sizeof(respbuf),
		 respbuf))
	{
		printf("Failed to query service\n");
		isc_service_detach(status, &svc_handle);
		return 0;
	}

	isc_service_detach(status, &svc_handle);
}

