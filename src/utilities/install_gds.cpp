/*
 *	PROGRAM:		Windows NT GDS Client Library Installation
 *	MODULE:			install_gds.cpp
 *	DESCRIPTION:	Program which install the FBCLIENT.DLL as GDS32.DLL
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
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install_nt.h"
#include "../utilities/gdsclient_proto.h"
//#include "../utilities/registry.h"
//#include "../common/config/config.h"

static USHORT gds_error(ULONG, const TEXT *);
static void usage(void);

static struct
{
	TEXT *name;
	USHORT abbrev;
	USHORT code;
} commands[] =
{
	{"INSTALL", 1, COMMAND_INSTALL},
	{"REMOVE", 1, COMMAND_REMOVE},
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
 *	Installs the FBCLIENT.DLL as GDS32.DLL in the Windows system directory.
 *
 **************************************/

	USHORT sw_command = COMMAND_NONE;
	bool sw_force = false;
	bool sw_version = false;

	// Let's get the root directory from the instance path of this program.
	// argv[0] is only _mostly_ guaranteed to give this info,
	// so we GetModuleFileName()
	TEXT directory[MAXPATHLEN];
	USHORT len = GetModuleFileName(NULL, directory, sizeof(directory));
	if (len == 0)
		return gds_error(GetLastError(), "GetModuleFileName");

	// Get to the last '\' (this one precedes the filename part). There is
	// always one after a call to GetModuleFileName().
	TEXT* p = directory + len;
	do {--p;} while (*p != '\\');

	// Get to the previous '\' (this one should precede the supposed 'bin\\' part).
	// There is always an additional '\' OR a ':'.
	do {--p;} while (*p != '\\' && *p != ':');
	*p = '\0';

	TEXT** end = argv + argc;
	while (++argv < end)
	{
		if (**argv != '-')
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
				usage();
			}
			sw_command = commands[i].code;
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
					usage();
			}
		}
	}

	if (sw_version)
		ib_printf("instgds version %s\n", GDS_VERSION);

	if (sw_command == COMMAND_NONE)
	{
		usage();
	}

	USHORT status;
	switch (sw_command)
	{
		
		case COMMAND_INSTALL:
			status = GDSCLIENT_install(directory, sw_force, gds_error);
			switch (status)
			{
				case FB_EXISTING_GDS32_FOUND :
					ib_printf("Existing GDS32.DLL found.\n");
					ib_printf("You can force replacing the DLL with -f[orce] "
						"switch, though this could\n");
					ib_printf("break an existing Firebird or InterBase(R) "
						"other installation.\n");
					break;
				case FB_GDS32_COPY_REQUIRES_REBOOT :
					ib_printf("Firebird GDS32.DLL has been scheduled for "
						"update at the next system reboot.\n");
				case FB_SUCCESS :
					ib_printf("The compatibility layer GDS32.DLL has been copied "
						"to the System directory.\n");
					break;
			}
			break;

		case COMMAND_REMOVE:
			status = GDSCLIENT_remove(directory, gds_error);
			break;
	}

	return (status == FB_SUCCESS) ? FINI_OK : FINI_ERROR;
}

static USHORT gds_error(ULONG status, const TEXT * string)
{
/**************************************
 *
 *	g d s _ e r r o r
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
	ib_printf("\nUsage:\n");
	ib_printf("  instgds i[nstall] [ -f[orce] ]\n");
	ib_printf("          q[uery]\n");
	ib_printf("          r[emove]\n\n");
	ib_printf("  This utility should be located and run from the 'bin' directory\n");
	ib_printf("  of your Firebird installation.\n\n");
	ib_printf("  '-z' can be used with any other option, prints version\n");

	ib_printf("\nPurpose:\n");
	ib_printf("  This utility installs a copy of the Firebird client library (FBCLIENT.DLL),\n");
	ib_printf("  renamed GDS32.DLL, to the Windows System Directory. This configuration\n");
	ib_printf("  step might be usefull for legacy application support.\n\n");
	ib_printf("  Please, note that if you -f[orce] the installation, you might have\n");
	ib_printf("  to reboot the machine in order to finalize the copy and you might\n");
	ib_printf("  break some other Firebird or InterBase(R) version on the system.\n\n");

	exit(FINI_ERROR);
}
