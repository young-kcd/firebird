/*
 *	PROGRAM:		Windows NT service control panel installation program
 *	MODULE:			install_svc.cpp
 *	DESCRIPTION:	Service control panel installation program
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
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install/install_nt.h"
#include "../utilities/install/servi_proto.h"
#include "../utilities/install/registry.h"
#include "../common/config/config.h"

#define REMOTE_EXECUTABLE ((sw_arch == ARCH_SS) ? REMOTE_SS_EXECUTABLE : REMOTE_CS_EXECUTABLE)

static void svc_query(const char*, const char*, SC_HANDLE manager);
static USHORT svc_error(SLONG, const TEXT*, SC_HANDLE);
static void usage(void);

static const struct
{
	const TEXT *name;
	USHORT abbrev;
	USHORT code;
} commands[] =
{
	{"INSTALL", 1, COMMAND_INSTALL},
	{"REMOVE", 1, COMMAND_REMOVE},
	{"START", 3, COMMAND_START},
	{"STOP", 3, COMMAND_STOP},
	{"QUERY", 1, COMMAND_QUERY},
	{NULL, 0, 0}
};

int CLIB_ROUTINE main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Install or remove a Firebird service.
 *
 **************************************/
	USHORT sw_command = COMMAND_NONE;
	bool sw_version = false;
	USHORT sw_startup = STARTUP_AUTO;
	USHORT sw_mode = DEFAULT_PRIORITY;
	USHORT sw_guardian = NO_GUARDIAN;
	USHORT sw_arch = ARCH_SS;

	TEXT *username = 0;
	TEXT *password = 0;

	// Let's get the root directory from the instance path of this program.
	// argv[0] is only _mostly_ guaranteed to give this info,
	// so we GetModuleFileName()
	TEXT directory[MAXPATHLEN];
	const USHORT len = GetModuleFileName(NULL, directory, sizeof(directory));
	if (len == 0)
		return svc_error(GetLastError(), "GetModuleFileName", NULL);

	// Get to the last '\' (this one precedes the filename part). There is
	// always one after a call to GetModuleFileName().
	TEXT* p = directory + len;
	do {--p;} while (*p != '\\');

	// Get to the previous '\' (this one should precede the supposed 'bin\\' part).
	// There is always an additional '\' OR a ':'.
	do {--p;} while (*p != '\\' && *p != ':');
	*p = '\0';

	TEXT full_username[128];
	TEXT oem_username[128];
	TEXT keyb_password[64];

	const TEXT* const* const end = argv + argc;
	while (++argv < end)
	{
		if (**argv != '-')
		{
			USHORT i;
			const TEXT* cmd;
			for (i = 0; cmd = commands[i].name; i++)
			{
				const TEXT* q;
				for (p = *argv, q = cmd; *p && UPPER(*p) == *q; p++, q++);
				if (!*p && commands[i].abbrev <= (USHORT) (q - cmd))
					break;
			}
			if (!cmd)
			{
				printf("Unknown command \"%s\"\n", *argv);
				usage();
			}
			sw_command = commands[i].code;
		}
		else
		{
			p = *argv + 1;
			switch (UPPER(*p))
			{
				case 'A':
					sw_startup = STARTUP_AUTO;
					break;

				case 'D':
					sw_startup = STARTUP_DEMAND;
					break;

				/*
				case 'R':
					sw_mode = NORMAL_PRIORITY;
					break;
				*/
				case 'B':
					sw_mode = HIGH_PRIORITY;
					break;

				case 'Z':
					sw_version = true;
					break;

				case 'G':
					sw_guardian = USE_GUARDIAN;
					break;

				case 'S':
					sw_arch = ARCH_SS;
					break;

				case 'C':
					sw_arch = ARCH_CS;
					break;

				case 'L':
					if (++argv < end)
						username = *argv;
					if (++argv < end)
					{
						if (**argv == '-')	// Next switch
							--argv;
						else
							password = *argv;
					}
					break;

				default:
					printf("Unknown switch \"%s\"\n", p);
					usage();
			}
		}
	}

	if (sw_version)
		printf("instsvc version %s\n", GDS_VERSION);

	if (sw_command == COMMAND_NONE ||
		(username != 0 && sw_command != COMMAND_INSTALL))
	{
		usage();
	}

	if (sw_command == COMMAND_INSTALL && username != 0)
	{
		p = username;
		while (*p != '\0' && *p != '\\') ++p;
		if (*p == '\0')
		{
			DWORD cnlen = sizeof(full_username);
			GetComputerName(full_username, &cnlen);
			strcat(full_username, "\\");
			strncat(full_username, username, sizeof(full_username) - (cnlen + 1));
		}
		else
			strncpy(full_username, username, sizeof(full_username));
		full_username[sizeof(full_username) - 1] = '\0';
		CharToOem(full_username, oem_username);
		username = full_username;

		if (password == 0)
		{
			printf("Enter %s user password : ", oem_username);
			p = keyb_password;
			const TEXT* const end = p + sizeof(keyb_password) - 1;	// keep room for '\0'
			while (p < end && (*p++ = getch()) != '\r')
				putch('*'); // Win32 only
			*(p - 1) = '\0';	// Cuts at '\r'
			printf("\n");
			OemToChar(keyb_password, keyb_password);
			password = keyb_password;
		}

		// Let's grant "Logon as a Service" right to the -login user
		switch (SERVICES_grant_logon_right(full_username, svc_error))
		{
			case FB_LOGON_SRVC_RIGHT_ALREADY_DEFINED :
				/*
				// OM - I think it is better not to bother the admin with this message.
				printf("The 'Logon as a Service' right was already "
					"granted to %s\n", oem_username);
				*/
				break;
			case FB_SUCCESS :
				printf("The 'Logon as a Service' right has been "
					"granted to %s\n", oem_username);
				break;
			case FB_FAILURE :
			default :
				printf("Failed granting the 'Logon as a Service' right "
					"to %s\n", oem_username);
				exit(FINI_ERROR);
				break;
		}
	}

	SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (manager == NULL)
	{
		svc_error(GetLastError(), "OpenSCManager", NULL);
		exit(FINI_ERROR);
	}

	USHORT status, status2;
	SC_HANDLE service;

	switch (sw_command)
	{
		case COMMAND_INSTALL:
			/* First, lets do the guardian, if it has been specified */
			if (sw_guardian)
			{
				status = SERVICES_install(manager, ISCGUARD_SERVICE,
					ISCGUARD_DISPLAY_NAME, ISCGUARD_DISPLAY_DESCR,
					ISCGUARD_EXECUTABLE, directory, NULL, sw_startup,
					username, password, svc_error);
				status2 = FB_SUCCESS;
				if (username != 0)
				{
					status2 = SERVICES_grant_access_rights(ISCGUARD_SERVICE,
						username, svc_error);
				}
				if (status == FB_SUCCESS && status2 == FB_SUCCESS)
					printf("Service \"%s\" successfully created.\n", ISCGUARD_DISPLAY_NAME);

				/* Set sw_startup to manual in preparation for install the service */
				sw_startup = STARTUP_DEMAND;
			}

			/* do the install of the server */
			status = SERVICES_install(manager, REMOTE_SERVICE,
					REMOTE_DISPLAY_NAME, REMOTE_DISPLAY_DESCR, REMOTE_EXECUTABLE,
						directory, NULL, sw_startup, username, password, svc_error);
			status2 = FB_SUCCESS;
			if (username != 0)
			{
				status2 = SERVICES_grant_access_rights(REMOTE_SERVICE,
					username, svc_error);
			}
			if (status == FB_SUCCESS && status2 == FB_SUCCESS)
				printf("Service \"%s\" successfully created.\n", REMOTE_DISPLAY_NAME);
			break;

		case COMMAND_REMOVE:
			service = OpenService(manager, ISCGUARD_SERVICE, SERVICE_ALL_ACCESS);
			if (service)
			{
				CloseServiceHandle(service);
				status = SERVICES_remove(manager, ISCGUARD_SERVICE,
					ISCGUARD_DISPLAY_NAME, svc_error);
				if (status == FB_SUCCESS)
				{
					printf("Service \"%s\" successfully deleted.\n", ISCGUARD_DISPLAY_NAME);
				}
				else if (status == IB_SERVICE_RUNNING)
				{
					printf("Service \"%s\" not deleted.\n", ISCGUARD_DISPLAY_NAME);
					printf("You must stop it before attempting to delete it.\n\n");
				}
			}
			else
				status = FB_FAILURE;

			service = OpenService(manager, REMOTE_SERVICE, SERVICE_ALL_ACCESS);
			if (service)
			{
				CloseServiceHandle(service);
				status2 = SERVICES_remove(manager, REMOTE_SERVICE, REMOTE_DISPLAY_NAME,
										 svc_error);
				if (status2 == FB_SUCCESS)
				{
				    printf("Service \"%s\" successfully deleted.\n", REMOTE_DISPLAY_NAME);
				}
				else if (status2 == IB_SERVICE_RUNNING)
				{
					printf("Service \"%s\" not deleted.\n", REMOTE_DISPLAY_NAME);
					printf("You must stop it before attempting to delete it.\n\n");
				}
			}
			else
				status2 = FB_FAILURE;

			if (status != FB_SUCCESS && status2 != FB_SUCCESS)
				status = FB_FAILURE;
			break;

		case COMMAND_START:
			/* Test for use of the guardian. If so, start the guardian else start the server */
			service = OpenService(manager, ISCGUARD_SERVICE, SERVICE_ALL_ACCESS);
			if (service)
			{
				CloseServiceHandle(service);
				status = SERVICES_start(manager, ISCGUARD_SERVICE, ISCGUARD_DISPLAY_NAME,
										sw_mode, svc_error);
				if (status == FB_SUCCESS)
				    printf("Service \"%s\" successfully started.\n", ISCGUARD_DISPLAY_NAME);
			}
			else
			{
				CloseServiceHandle(service);
				status = SERVICES_start(manager, REMOTE_SERVICE, REMOTE_DISPLAY_NAME,
										sw_mode, svc_error);
				if (status == FB_SUCCESS)
					printf("Service \"%s\" successfully started.\n", REMOTE_DISPLAY_NAME);
			}
			break;

		case COMMAND_STOP:
			/* Test for use of the guardian. If so, stop the guardian else stop the server */
			service = OpenService(manager, ISCGUARD_SERVICE, SERVICE_ALL_ACCESS);
			if (service)
			{
				CloseServiceHandle(service);
				status = SERVICES_stop(manager, ISCGUARD_SERVICE,
					ISCGUARD_DISPLAY_NAME, svc_error);
				if (status == FB_SUCCESS)
					printf("Service \"%s\" successfully stopped.\n", ISCGUARD_DISPLAY_NAME);
			}
			else
			{
				CloseServiceHandle(service);
				status = SERVICES_stop(manager, REMOTE_SERVICE,
					REMOTE_DISPLAY_NAME, svc_error);
				if (status == FB_SUCCESS)
					printf("Service \"%s\" successfully stopped.\n", REMOTE_DISPLAY_NAME);
			}
			break;

		case COMMAND_QUERY:
			svc_query(ISCGUARD_SERVICE, ISCGUARD_DISPLAY_NAME, manager);
			svc_query(REMOTE_SERVICE, REMOTE_DISPLAY_NAME, manager);
			status = FB_SUCCESS;
			break;

		default:
			status = FB_SUCCESS;
	}

	CloseServiceHandle(manager);

	return (status == FB_SUCCESS) ? FINI_OK : FINI_ERROR;
}

static void svc_query(const char* name, const char* display_name, SC_HANDLE manager)
{
/**************************************
 *
 *	s v c _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Report (print) the status and configuration of a service.
 *
 **************************************/
	if (manager == NULL)
		return;

	SC_HANDLE service = OpenService(manager, name, SERVICE_ALL_ACCESS);
	if (service)
	{
		printf("\n%s IS installed.\n", display_name);
		SERVICE_STATUS service_status;
		if (QueryServiceStatus(service, &service_status))
		{
			printf("  Status  : ");
			switch (service_status.dwCurrentState)
			{
				case SERVICE_STOPPED : printf("stopped\n"); break;
				case SERVICE_START_PENDING : printf("starting\n"); break;
				case SERVICE_STOP_PENDING : printf("stopping\n"); break;
				case SERVICE_RUNNING : printf("running\n"); break;
				default: printf("unknown state\n");
			}
		}
		else
			svc_error(GetLastError(), "QueryServiceStatus", NULL);

		ULONG uSize;
		QueryServiceConfig(service, NULL, 0, &uSize);
		QUERY_SERVICE_CONFIG* qsc = (QUERY_SERVICE_CONFIG*) new UCHAR[uSize];
		if (qsc && QueryServiceConfig(service, qsc, uSize, &uSize))
		{
			CharToOem(qsc->lpBinaryPathName, qsc->lpBinaryPathName);
			CharToOem(qsc->lpServiceStartName, qsc->lpServiceStartName);
			printf("  Path    : %s\n", qsc->lpBinaryPathName);
			printf("  Startup : ");
			switch (qsc->dwStartType)
			{
				case SERVICE_AUTO_START : printf("automatic\n"); break;
				case SERVICE_DEMAND_START : printf("manual\n"); break;
				case SERVICE_DISABLED : printf("disabled\n"); break;
				default : printf("invalid setting\n");
			}
			if (! qsc->lpServiceStartName)
				printf("  Run as  : LocalSystem");
			else
				printf("  Run as  : %s", qsc->lpServiceStartName);

			if (qsc->dwServiceType & SERVICE_INTERACTIVE_PROCESS)
				printf(" (Interactive)\n");
			else
				printf("\n");
		}
		else
			svc_error(GetLastError(), "QueryServiceConfig", NULL);
		if (qsc)
			delete [] (UCHAR*)qsc;

		CloseServiceHandle(service);
	}
	else
		printf("\n%s is NOT installed.\n", display_name);

	return;
}

static USHORT svc_error( SLONG status, const TEXT* string, SC_HANDLE service)
{
/**************************************
 *
 *	s v c _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report an error and punt.
 *
 **************************************/
	TEXT buffer[512];

	if (service != NULL)
		CloseServiceHandle(service);

	if (status == 0)
	{
		// Allows to report non System errors
		printf("%s\n", string);
	}
	else
	{
		printf("Error occurred during \"%s\".\n", string);

		if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								status,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								buffer,
								sizeof(buffer),
								NULL))
		{
			printf("Windows NT error %"SLONGFORMAT"\n", status);
		}
		else
		{
			CharToOem(buffer, buffer);
			printf("%s", buffer);	// '\n' is included in system messages
		}
	}
	return FB_FAILURE;
}

static void usage(void)
{
/**************************************
 *
 *	u s a g e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	printf("\nUsage:\n");
	printf("  instsvc i[nstall] [ -s[uperserver]* | -c[lassic] ]\n");
	printf("                    [ -a[uto]* | -d[emand] ]\n");
	printf("                    [ -g[uardian] ]\n");
	printf("                    [ -l[ogin] username [password] ]\n\n");
	printf("          sta[rt]   [ -b[oostpriority] ]\n");
	printf("          sto[p]\n");
	printf("          q[uery]\n");
	printf("          r[emove]\n\n");
	printf("  This utility should be located and run from the 'bin' directory\n");
	printf("  of your Firebird installation.\n\n");
	printf("  '*' denotes the default values\n");
	printf("  '-z' can be used with any other option, prints version\n");
	printf("  'username' refers by default to a local account on this machine.\n");
	printf("  Use the format 'domain\\username' or 'server\\username' if appropriate.\n");

	exit(FINI_ERROR);
}

//
// Until the fb_assert could be converted to a function/object linked with each module
// we need this ugly workaround.
//
void API_ROUTINE gds__log(const TEXT* text, ...)
{
	va_list ptr;

	va_start(ptr, text);
	vprintf(text, ptr);
	va_end(ptr);
	printf("\n\n");
}

