/*
 *      PROGRAM:        JRD Access Method - Win32 specific functions.
 *      MODULE:         isc_win32.c
 *      DESCRIPTION:    General purpose but non-user routines.
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
 * Added TCP_NO_DELAY option for superserver on Linux
 * FSG 16.03.2001 
 * Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
 */
/*
$Id: isc_win32.cpp,v 1.6 2002-11-06 07:19:01 eku Exp $
*/

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <windows.h>

#include "gen/codes.h"
#include "../jrd/isc.h"
#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/jrd_proto.h"
#include "../utilities/registry.h"


#include <windows.h>
#ifdef TEXT
#undef TEXT
#endif
#define TEXT            SCHAR

#include <process.h>
#include <signal.h>

static USHORT				os_type;
static SECURITY_ATTRIBUTES	security_attr;

static TEXT interbase_directory[MAXPATHLEN];

void			isc_internal_set_config_value(UCHAR, ULONG*, ULONG*);
static BOOLEAN	check_user_privilege();


//____________________________________________________________
//
// Checks if the process indicated by the process id 'pid' exists.
// Returns TRUE if the indicated process exists, FALSE otherwise.
//
int DLL_EXPORT ISC_check_process_existence(SLONG	pid,
										   SLONG	xl_pid,
										   USHORT	super_user)
{
	HANDLE handle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, (DWORD) pid);

	if (!handle && GetLastError() != ERROR_ACCESS_DENIED)
	{
		return FALSE;
	}

	CloseHandle(handle);
	return TRUE;
}


//____________________________________________________________
//
// Get host name. Note that this is not the DNS (TCP/IP) hostname,
// it's the Win32 computer name.
//
TEXT* INTERNAL_API_ROUTINE ISC_get_host(TEXT * string, USHORT length)
{
	DWORD host_len = length;
	if (GetComputerName(string, &host_len))
	{
		string[host_len] = 0;
	}
	else
	{
		strcpy(string, "local");
	}

	return string;
}


//____________________________________________________________
//
// Returns the type of OS. TRUE for NT,
// FALSE for the 16-bit based ones (9x/ME, ...).
//
BOOLEAN ISC_is_WinNT()
{
	OSVERSIONINFO OsVersionInfo;

	if (!os_type)
	{
		os_type = 1;			/* Default to NT */
		/* The first time this routine is called we use the Windows API
		   call GetVersion to determine whether Windows/NT or Chicago
		   is running. */

		OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx((LPOSVERSIONINFO) & OsVersionInfo))
		{
			os_type =
				(OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) ? 1 : 2;
		}

	}

	return (os_type != 2) ? TRUE : FALSE;
}


//____________________________________________________________
//
// Lookup a registry entry.
// Returns TRUE if the lookup was successful, FALSE otherwise.
//
SSHORT ISC_get_registry_var(TEXT*	variable,
							TEXT*	buffer,
							SSHORT	buffer_length,
							void**	user_hkey)
{
	HKEY hkey;
	DWORD len, type;

	if (!user_hkey)
	{
		LPCTSTR szKey = REG_KEY_ROOT_CUR_VER;
		const LONG ret =
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_QUERY_VALUE, &hkey);
		if (ret != ERROR_SUCCESS)
		{
			return -1;
		}
	}
	else
	{
		hkey = *((PHKEY) user_hkey);
	}

	len = buffer_length;

#pragma FB_COMPILER_MESSAGE("Can/should we fix this cast?")

	const LONG ret =
		RegQueryValueEx(hkey, variable, NULL, &type, (LPBYTE) buffer, &len);

	if (ret != ERROR_SUCCESS)
	{
		len = 0;
	}

	if (!user_hkey)
	{
		RegCloseKey(hkey);
	}

	return (SSHORT) len;
}


//____________________________________________________________
//
//
LPSECURITY_ATTRIBUTES ISC_get_security_desc()
{
	if (security_attr.lpSecurityDescriptor)
	{
		return &security_attr;
	}

	PSECURITY_DESCRIPTOR p_security_desc =
		(PSECURITY_DESCRIPTOR) gds__alloc((SLONG)
											SECURITY_DESCRIPTOR_MIN_LENGTH);
/* FREE: apparently never freed */
	if (!p_security_desc)		/* NOMEM: */
	{
		return NULL;
	}
#ifdef DEBUG_GDS_ALLOC
	gds_alloc_flag_unfreed((void *) p_security_desc);
#endif

	if (!InitializeSecurityDescriptor(	p_security_desc,
										SECURITY_DESCRIPTOR_REVISION) ||
		!SetSecurityDescriptorDacl(p_security_desc, TRUE, (PACL) NULL, FALSE))
	{
		gds__free(p_security_desc);
		return NULL;
	}

	security_attr.nLength = sizeof(security_attr);
	security_attr.lpSecurityDescriptor = p_security_desc;
	security_attr.bInheritHandle = TRUE;

	return &security_attr;
}


//____________________________________________________________
//
// Find out who the user is.
//
int INTERNAL_API_ROUTINE ISC_get_user(TEXT*	name,
									  int*	id,
									  int*	group,
									  TEXT*	project,
									  TEXT*	organization,
									  int*	node,
									  TEXT*	user_string)
{
	if (id)
		*id = -1;

	if (group)
		*group = -1;

	if (project)
		*project = 0;

	if (organization)
		*organization = 0;

	if (node)
		*node = 0;

	if (name)
	{
		name[0] = 0;
		DWORD  name_len = 128;
		if (GetUserName(name, &name_len))
		{
			name[name_len] = 0;

			/* NT user name is case insensitive */

			for (DWORD i = 0; i < name_len; i++)
			{
				name[i] = UPPER7(name[i]);
			}

/* This check is not internationalized, the security model needs to be
 * reengineered, especially on SUPERSERVER where none of these local
 * user (in process) assumptions are valid.
 */
			if (!strcmp(name, "ADMINISTRATOR"))
			{
				if (id)
					*id = 0;

				if (group)
					*group = 0;
			}
		}
	}

	return check_user_privilege();
}


//____________________________________________________________
//
// Check to see if the user belongs to the administrator group.
//
// This routine was adapted from code in routine RunningAsAdminstrator
// in \mstools\samples\regmpad\regdb.c.
//
static BOOLEAN check_user_privilege(void)
{
	HANDLE tkhandle;
	SID_IDENTIFIER_AUTHORITY system_sid_authority = SECURITY_NT_AUTHORITY;

	// First we must open a handle to the access token for this thread.

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &tkhandle))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			// If the thread does not have an access token, we'll examine the
			// access token associated with the process.

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tkhandle))
			{
				CloseHandle(tkhandle);
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	TOKEN_GROUPS*	ptg       = NULL;
	DWORD			token_len = 0;

	while (TRUE)
	{
		/* Then we must query the size of the group information associated with
		   the token.  This is guarenteed to fail the first time through
		   because there is no buffer. */

		if (GetTokenInformation(tkhandle,
								TokenGroups,
								ptg,
								token_len,
								&token_len))
		{
			break;
		}

		/* If there had been a buffer, it's either too small or something
		   else is wrong.  Either way, we can dispose of it. */

		if (ptg)
		{
			gds__free(ptg);
		}

		/* Here we verify that GetTokenInformation failed for lack of a large
		   enough buffer. */

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			CloseHandle(tkhandle);
			return FALSE;
		}

		// Allocate a buffer for the group information.
		ptg = (TOKEN_GROUPS *) gds__alloc((SLONG) token_len);

		if (!ptg)
		{
			CloseHandle(tkhandle);
			return FALSE;		/* NOMEM: */
		}
		// FREE: earlier in this loop, and at procedure return
	}

	// Create a System Identifier for the Admin group.

	PSID admin_sid;

	if (!AllocateAndInitializeSid(&system_sid_authority, 2,
								  SECURITY_BUILTIN_DOMAIN_RID,
								  DOMAIN_ALIAS_RID_ADMINS,
								  0, 0, 0, 0, 0, 0, &admin_sid))
	{
		gds__free(ptg);
		CloseHandle(tkhandle);
		return FALSE;
	}

	// Finally we'll iterate through the list of groups for this access
	// token looking for a match against the SID we created above.

	BOOLEAN admin_priv = FALSE;

	for (DWORD i = 0; i < ptg->GroupCount; i++)
	{
		if (EqualSid(ptg->Groups[i].Sid, admin_sid))
		{
			admin_priv = TRUE;
			break;
		}
	}

	// Deallocate the SID we created.

	FreeSid(admin_sid);
	gds__free(ptg);
	CloseHandle(tkhandle);
	return admin_priv;
}


#ifdef SUPERSERVER

//____________________________________________________________
//
//
void isc_internal_set_config_value(UCHAR	key,
								   ULONG*	value1_ptr,
								   ULONG*	value2_ptr)
{
	switch (key)
	{
	case ISCCFG_DBCACHE_KEY:
		JRD_set_cache_default(value1_ptr);
		break;
	case ISCCFG_PRIORITY_KEY:
		{
			ULONG priority;
			if (*value1_ptr == 2)
				priority = HIGH_PRIORITY_CLASS;
			else
				priority = NORMAL_PRIORITY_CLASS;

			if (!SetPriorityClass(GetCurrentProcess(), priority))
			{
				gds__log("Unable to set process priority.");
			}
		}
		break;
	case ISCCFG_MEMMAX_KEY:
	case ISCCFG_MEMMIN_KEY:
		if (*value1_ptr && *value2_ptr && ISC_is_WinNT())
		{
			HANDLE hndl =
				OpenProcess(PROCESS_SET_QUOTA, FALSE, GetCurrentProcessId());

			if (hndl) {
				ULONG value1 = *value1_ptr * 1024;
				ULONG value2 = *value2_ptr * 1024;

				if (!SetProcessWorkingSetSize(hndl, value1, value2))
				{
					switch (GetLastError())
					{
					case ERROR_ACCESS_DENIED:
					case ERROR_PRIVILEGE_NOT_HELD:
						gds__log("Error setting process working set size, access denied.");
						break;
					case ERROR_INVALID_PARAMETER:
						gds__log("Error setting process working set size, improper range.");
						break;
					case ERROR_NO_SYSTEM_RESOURCES:
						gds__log("Error setting process working set size, insufficient memory.");
						break;
					default:
						gds__log("Error setting process working set size, unknown problem.");


					}
					*value1_ptr = 0;
					*value2_ptr = 0;
				}
				CloseHandle(hndl);
			}

		}
	}
}

#endif /* SUPERSERVER */

