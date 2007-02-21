/*
 *	PROGRAM:		Windows Win32 Client Libraries Installation
 *	MODULE:			install_client.cpp
 *	DESCRIPTION:	Program which install the FBCLIENT.DLL or GDS32.DLL
 *
 *  The contents of this file are subject to the Initial Developer's 
 *  Public License Version 1.0 (the "License"); you may not use this 
 *  file except in compliance with the License. You may obtain a copy 
 *  of the License here:
 *
 *    http://www.ibphoenix.com?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed on an "AS 
 *  IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or 
 *  implied. See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code is (C) 2003 Olivier Mascia.
 *  
 *  The Initial Developer of the Original Code is Olivier Mascia.
 *
 *  All Rights Reserved.
 *  
 *  Contributor(s): ______________________________________. 
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install_nt.h"
#include "../utilities/install_proto.h"

static USHORT inst_error(ULONG, const TEXT *);
static void usage_exit(void);

static struct
{
	TEXT* name;
	USHORT abbrev;
	USHORT code;
} commands[] =
{
	{"INSTALL", 1, COMMAND_INSTALL},
	{"REMOVE", 1, COMMAND_REMOVE},
	{"QUERY", 1, COMMAND_QUERY},
	{NULL, 0, 0}
};

static struct
{
	TEXT* name;
	USHORT abbrev;
	USHORT code;
} clients[] =
{
	{"FBCLIENT", 1, CLIENT_FB},
	{"GDS32", 1, CLIENT_GDS},
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
 *	Installs the FBCLIENT.DLL or GDS32.DLL in the Windows system directory.
 *
 **************************************/

	USHORT sw_command = COMMAND_NONE;
	USHORT sw_client = CLIENT_NONE;
	bool sw_force = false;
	bool sw_version = false;

	// Let's get the root directory from the instance path of this program.
	// argv[0] is only _mostly_ guaranteed to give this info,
	// so we GetModuleFileName()
	TEXT directory[MAXPATHLEN];
	USHORT len = GetModuleFileName(NULL, directory, sizeof(directory));
	if (len == 0)
		return inst_error(GetLastError(), "GetModuleFileName");

	// Get to the last '\' (this one precedes the filename part). There is
	// always one after a call to GetModuleFileName().
	TEXT* p = directory + len;
	do {--p;} while (*p != '\\');
	*p = '\0';

	TEXT** end = argv + argc;
	while (++argv < end)
	{
		if (**argv != '-')
		{
			if (sw_command == COMMAND_NONE)
			{
				TEXT* cmd;
				int i;
				for (i = 0; cmd = commands[i].name; i++)
				{
					TEXT* q;
					for (p = *argv, q = cmd; *p && UPPER(*p) == *q; p++, q++);
					if (!*p && commands[i].abbrev <= (USHORT) (q - cmd))
						break;
				}
				if (!cmd)
				{
					ib_printf("Unknown command \"%s\"\n", *argv);
					usage_exit();
				}
				sw_command = commands[i].code;
			}
			else
			{
				TEXT* cln;
				int i;
				for (i = 0; cln = clients[i].name; i++)
				{
					TEXT* q;
					for (p = *argv, q = cln; *p && UPPER(*p) == *q; p++, q++);
					if (!*p && clients[i].abbrev <= (USHORT) (q - cln))
						break;
				}
				if (!cln)
				{
					ib_printf("Unknown library \"%s\"\n", *argv);
					usage_exit();
				}
				sw_client = clients[i].code;
			}
		}
		else
		{
			p = *argv + 1;
			switch (UPPER(*p))
			{
				case 'F':
					sw_force = true;
					break;

				case 'Z':
					sw_version = true;
					break;

				default:
					ib_printf("Unknown switch \"%s\"\n", p);
					usage_exit();
			}
		}
	}

	if (sw_version)
		ib_printf("instclient version %s\n", GDS_VERSION);

	TEXT fbdll[MAXPATHLEN];
	lstrcpy(fbdll, directory);
	lstrcat(fbdll, "\\");
	lstrcat(fbdll, FBCLIENT_NAME);
	HANDLE hfile = CreateFile(fbdll, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		inst_error(0, "FBCLIENT.DLL not found in instclient.exe directory");
		return FINI_ERROR;
	}
	CloseHandle(hfile);

	if (sw_command == COMMAND_NONE || sw_client == CLIENT_NONE)
	{
		usage_exit();
	}

	TEXT* clientname = sw_client == CLIENT_GDS ? GDS32_NAME : FBCLIENT_NAME;

	USHORT status;
	switch (sw_command)
	{
		
		case COMMAND_INSTALL:
			status = CLIENT_install(directory, sw_client, sw_force, inst_error);
			switch (status)
			{
				case FB_INSTALL_SAME_VERSION_FOUND :
					ib_printf("Existing %s (same version) found.\n", clientname);
					ib_printf("No update needed.\n");
					break;
				case FB_INSTALL_NEWER_VERSION_FOUND :
					ib_printf("Existing %s (newer version) found.\n", clientname);
					ib_printf("You can force replacing the DLL with -f[orce] switch.\n");
					ib_printf("Though this could break some other InterBase(R) "
						"or Firebird installation.\n");
					break;
				case FB_INSTALL_COPY_REQUIRES_REBOOT :
					ib_printf("%s has been scheduled for installation "
						"at the next system reboot.\n", clientname);
					break;
				case FB_SUCCESS :
					ib_printf("%s has been installed to the "
						"System directory.\n", clientname);
					break;
			}
			break;

		case COMMAND_REMOVE:
			status = CLIENT_remove(directory, sw_client, sw_force, inst_error);
			switch (status)
			{
				case FB_INSTALL_FILE_NOT_FOUND :
					ib_printf("%s was not found in the System directory.\n",
						clientname);
					break;
				case FB_INSTALL_CANT_REMOVE_ALIEN_VERSION :
					ib_printf("The installed %s appears to be from an "
						"unsupported version.\n", clientname);
					ib_printf("It probably belongs to another version of "
						"Firebird or InterBase(R).\n");
					break;
				case FB_INSTALL_FILE_PROBABLY_IN_USE :
					ib_printf("The %s can't be removed. It is probably "
						"currently in use.\n", clientname);
					break;
				case FB_SUCCESS :
					ib_printf("The %s has been removed from the "
						"System directory.\n", clientname);
					break;
			}
			break;

		case COMMAND_QUERY:
			ULONG verMS, verLS, sharedCount;
			status = CLIENT_query(sw_client, verMS, verLS, sharedCount, inst_error);
			switch (status)
			{
				case FB_INSTALL_FILE_NOT_FOUND :
					ib_printf("%s was not found in the System directory.\n",
						clientname);
					break;
				case FB_SUCCESS :
					ib_printf("Installed %s version : %u.%u.%u.%u "
						"(shared DLL count %d)\n", clientname,
						verMS >> 16, verMS & 0x0000ffff,
						verLS >> 16, verLS & 0x0000ffff,
						sharedCount);
					break;
			}
			break;
	}

	switch (status)
	{
		case FB_INSTALL_COPY_REQUIRES_REBOOT :
			return -FB_INSTALL_COPY_REQUIRES_REBOOT;
		case FB_SUCCESS :
			return FINI_OK;
		default :
			return FINI_ERROR;
	}
}

static USHORT inst_error(ULONG status, const TEXT * string)
{
/**************************************
 *
 *	i n s t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report an error and punt.
 *
 **************************************/
	TEXT buffer[512];
	SSHORT l;

	if (status == 0)
	{
		// Allows to report non System errors
		ib_printf("%s\n", string);
	}
	else
	{
		ib_printf("Error %u occurred during \"%s\".\n", status, string);

		if (!(l = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								status,
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								buffer,
								sizeof(buffer),
								NULL)))
		{
			ib_printf("Windows NT error %"SLONGFORMAT"\n", status);
		}
		else
		{
			CharToOem(buffer, buffer);
			ib_printf("%s", buffer);	// '\n' is included in system messages
		}
	}
	return FB_FAILURE;
}

static void usage_exit(void)
{
/**************************************
 *
 *	u s a g e _ e x i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	ib_printf("Usage:\n");
	ib_printf("  instclient i[nstall] [ -f[orce] ] library\n");
	ib_printf("             q[uery] library\n");
	ib_printf("             r[emove] library\n");
	ib_printf("\n");
	ib_printf("  where library is:  f[bclient] | g[ds32] \n");
	ib_printf("\n");
	ib_printf("  This utility should be located and run from the 'bin' directory\n");
	ib_printf("  of your Firebird installation.\n");
	ib_printf("  '-z' can be used with any other option, prints version\n");
	ib_printf("\n");
	ib_printf("Purpose:\n");
	ib_printf("  This utility manages deployment of the Firebird client library \n");
	ib_printf("  into the Windows system directory. It caters for two installation\n");
	ib_printf("  scenarios:\n");
	ib_printf("\n");
	ib_printf("    Deployment of the native fbclient.dll.\n");
	ib_printf("    Deployment of gds32.dll to support legacy applications.\n");
	ib_printf("\n");
	ib_printf("  Version information and shared library counts are handled \n");
	ib_printf("  automatically. You may provide the -f[orce] option to override\n");
	ib_printf("  version checks.\n");
	ib_printf("\n");
	ib_printf("  Please, note that if you -f[orce] the installation, you might have\n");
	ib_printf("  to reboot the machine in order to finalize the copy and you might\n");
	ib_printf("  break some other Firebird or InterBase(R) version on the system.\n");

	exit(FINI_ERROR);
}

//
// Until the fb_assert could be converted to a function/object linked with each module
// we need this ugly workaround.
//
extern "C" void API_ROUTINE gds__log(const TEXT* text, ...)
{
	va_list ptr;

	va_start(ptr, text);
	vprintf(text, ptr);
	va_end(ptr);
	printf("\n\n");
}

