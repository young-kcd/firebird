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
 *  The Original Code was created by Claudio Valderrama on 25-Dec-2003
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2003 Claudio Valderrama
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */


// =====================================
// Utility functions

#include "../jrd/common.h"

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
#define __need_size_t
#include <stddef.h>
#undef __need_size_t
#endif
#include <stdarg.h>

#include "../jrd/gdsassert.h"
#include "../common/utils_proto.h"
#include "../jrd/constants.h"

namespace fb_utils
{


char* exact_name(char* const str)
{
/**************************************
 *
 *	e x a c t _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Trim off trailing spaces from a metadata name.
 *	eg: insert a null after the last non-blank character.
 *
 *	SQL delimited identifier may have blank as part of the name
 *
 *	Parameters:  str - the string to terminate
 *	Returns:     str
 *
 **************************************/
	char* p = str;
	while (*p)
	    ++p;
	// Now, let's go back
	--p;
	while (p >= str && *p == '\x20') // blank character, ASCII(32)
		--p;
	*(p + 1) = '\0';
	return str;
}


char* exact_name_limit(char* const str, size_t bufsize)
{
/**************************************
 *
 *	e x a c t _ n a m e _ l i m i t
 *
 **************************************
 *
 * Functional description
 *	Trim off trailing spaces from a metadata name.
 *	eg: insert a null after the last non-blank character.
 *	It has a maximum length to ensure working between bounds.
 *
 *	SQL delimited identifier may have blank as part of the name
 *
 *	Parameters:  str - the string to terminate
 *               bufsize - the size of the variable containing the string.
 *	Returns:     str
 *
 **************************************/
	const char* const end = str + bufsize - 1;
	char* p = str;
	while (*p && p < end)
	    ++p;
	// Now, let's go back
	--p;
	while (p >= str && *p == '\x20') // blank character, ASCII(32)
		--p;
	*(p + 1) = '\0';
	return str;
}


// *****************************
// i m p l i c i t _ d o m a i n
// *****************************
// Determines if a domain is of the form RDB$<n[...n]>[<spaces>]
bool implicit_domain(const char* domain_name)
{
	if (strncmp(domain_name, IMPLICIT_DOMAIN_PREFIX, IMPLICIT_DOMAIN_PREFIX_LEN) != 0)
		return false;
		
	int i = IMPLICIT_DOMAIN_PREFIX_LEN;
	while (domain_name[i] >= '0' && domain_name[i] <= '9')
		++i;
		
	if (i == IMPLICIT_DOMAIN_PREFIX_LEN) // 'RDB$' alone isn't valid
		return false;
		
	while (domain_name[i] == ' ')
		++i;
		
	return !domain_name[i]; // we reached null term
}


int name_length(const TEXT* const name)
{
/**************************************
 *
 *	n a m e _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Compute effective length of system relation name and others.
 *	SQL delimited identifier may contain blanks. Trailing blanks are ignored.
 *  Assumes input is null terminated.
 *
 **************************************/
	const TEXT* q = name - 1;
	for (const TEXT* p = name; *p; p++) {
		if (*p != ' ') {
			q = p;
		}
	}

	return (q + 1) - name;
}


//***************
// r e a d e n v
//***************
// Goes to read directly the environment variables from the operating system on Windows
// and provides a stub for UNIX.
bool readenv(const char* env_name, Firebird::string& env_value)
{
#ifdef WIN_NT
	const DWORD rc = GetEnvironmentVariable(env_name, NULL, 0);
	if (rc)
	{
		env_value.reserve(rc - 1);
		DWORD rc2 = GetEnvironmentVariable(env_name, env_value.begin(), rc);
		if (rc2 < rc && rc2 != 0)
		{
			env_value.recalculate_length();
			return true;
		}
	}
#else
	const char* p = getenv(env_name);
	if (p)
		return env_value.assign(p).length() != 0;
#endif
	// Not found, clear the output var.
	env_value.begin()[0] = 0;
	env_value.recalculate_length();
	return false;
}


bool readenv(const char* env_name, Firebird::PathName& env_value)
{
	Firebird::string result;
	bool rc = readenv(env_name, result);
	env_value.assign(result.c_str(), result.length());
	return rc;
}


// ***************
// s n p r i n t f
// ***************
// Provide a single place to deal with vsnprintf and error detection.
int snprintf(char* buffer, size_t count, const char* format...)
{
	va_list args;
	va_start(args, format);
	const int rc = VSNPRINTF(buffer, count, format, args);
	buffer[count - 1] = 0;
	va_end(args);
#if defined(DEV_BUILD) && !defined(HAVE_VSNPRINTF)
	// We don't have the safe functions, then check if we overflowed the buffer.
	// I would prefer to make this functionality available in prod build, too.
	// If the docs are right, the null terminator is not counted => rc < count.
#if defined(fb_assert_continue)
	fb_assert_continue(rc >= 0 && rc < count);
#else
	fb_assert(rc >= 0 && rc < count);
#endif
#endif
	return rc;
}

#ifdef WIN_NT

bool validateProductSuite (LPCSTR lpszSuiteToValidate);
bool isTerminalServicesEnabled();

// hvlad: begins from Windows 2000 we can safely add 'Global\' prefix for 
// names of all kernel objects we use. For Win9x we must not add this prefix. 
// Win NT will accept such names only if Terminal Server is installed. 
// Check OS version carefully and add prefix if we can add it

void prefix_kernel_object_name(char* name, size_t bufsize)
{
	static bool bTSEnabled = false;
	static bool bInitDone = false;

	if (!bInitDone)
	{
		bTSEnabled = isTerminalServicesEnabled();
		bInitDone = true;
	}

	if (bTSEnabled)
	{
		const char *prefix = "Global\\";
		const size_t len_prefix = strlen(prefix);
		const size_t len_name = strlen(name) + 1;

		// if name and prefix can't fit in name's buffer than we must 
		// not overwrite end of name because it contains object type
		const int move_prefix = (len_name + len_prefix > bufsize) ?
			(bufsize - len_name) : len_prefix;

		memmove(name + move_prefix, name, len_name);
		memcpy(name, prefix, move_prefix);
	}
}

// hvlad: two functions below got from 
// http://msdn2.microsoft.com/en-us/library/aa380797.aspx
// and slightly adapted for our coding style

// -------------------------------------------------------------
//   Note that the validateProductSuite and IsTerminalServices
//   functions use ANSI versions of the functions to maintain
//   compatibility with Windows Me/98/95.
//   ------------------------------------------------------------- 

bool isTerminalServicesEnabled() 
{
	DWORD dwVersion = GetVersion();

	// Is Windows NT running?
	if (!(dwVersion & 0x80000000)) 
	{
		// Is it Windows 2000 or greater?
		if (LOBYTE(LOWORD(dwVersion)) > 4) 
		{
			return true; 

			// hvlad: for now we don't need such complex check but i 
			// preserved code in case we will need it later

/***
			// On Windows 2000 and later, use the VerifyVersionInfo and 
			// VerSetConditionMask functions. Don't static link because 
			// it won't load on earlier systems.

			HMODULE hmodNtDll = GetModuleHandleA("ntdll.dll");
			if (hmodNtDll) 
			{
				typedef ULONGLONG (WINAPI *PFnVerSetCondition) (ULONGLONG, ULONG, UCHAR);
				typedef BOOL (WINAPI *PFnVerifyVersionA) (POSVERSIONINFOEXA, DWORD, DWORDLONG);

				PFnVerSetCondition pfnVerSetCondition = 
					(PFnVerSetCondition) GetProcAddress(hmodNtDll, "VerSetConditionMask");

				if (pfnVerSetCondition != NULL) 
				{
					DWORDLONG dwlCondition = 
						(*pfnVerSetCondition) (dwlCondition, VER_SUITENAME, VER_AND);

					// Get a VerifyVersionInfo pointer.
					HMODULE hmodK32 = GetModuleHandleA( "KERNEL32.DLL" );
					if (hmodK32) 
					{
						PFnVerifyVersionA pfnVerifyVersionA = 
							(PFnVerifyVersionA) GetProcAddress(hmodK32, "VerifyVersionInfoA") ;

						if (pfnVerifyVersionA != NULL) 
						{
							OSVERSIONINFOEXA osVersion;

							ZeroMemory(&osVersion, sizeof(osVersion));
							osVersion.dwOSVersionInfoSize = sizeof(osVersion);
							osVersion.wSuiteMask = VER_SUITE_TERMINAL;

							return (*pfnVerifyVersionA) (&osVersion, VER_SUITENAME, dwlCondition);
						}
					}
				}
			}

			return false;
***/
		}
		else  // This is Windows NT 4.0 or earlier.
		{
			return validateProductSuite("Terminal Server");
		}
	}

	return false;
}

////////////////////////////////////////////////////////////
// validateProductSuite function
//
// Terminal Services detection code for systems running
// Windows NT 4.0 and earlier.
//
////////////////////////////////////////////////////////////

bool validateProductSuite (LPCSTR lpszSuiteToValidate) 
{
	bool fValidated = false;
	HKEY hKey = NULL;
	DWORD dwType = 0;
	DWORD dwSize = 0;
	LPSTR lpszProductSuites = 0;
	LPCSTR lpszSuite = 0;

	// Open the ProductOptions key.
	LONG lResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
	if (lResult != ERROR_SUCCESS)
		goto exit;

	// Determine required size of ProductSuite buffer.
	lResult = RegQueryValueExA(hKey, "ProductSuite", NULL, &dwType, NULL, &dwSize);
	if (lResult != ERROR_SUCCESS || !dwSize)
		goto exit;

	// Allocate buffer.
	lpszProductSuites = (LPSTR) LocalAlloc(LPTR, dwSize);
	if (!lpszProductSuites)
		goto exit;

	// Retrieve array of product suite strings.
	lResult = RegQueryValueExA(hKey, "ProductSuite", NULL, &dwType, 
		(LPBYTE) lpszProductSuites, &dwSize);
	if (lResult != ERROR_SUCCESS || dwType != REG_MULTI_SZ)
		goto exit;

	// Search for suite name in array of strings.
	lpszSuite = lpszProductSuites;
	while (*lpszSuite)
	{
		if (lstrcmpA(lpszSuite, lpszSuiteToValidate) == 0)
		{
			fValidated = true;
			break;
		}
		lpszSuite += (lstrlenA(lpszSuite) + 1);
	}

exit:
	if (lpszProductSuites) {
		LocalFree(lpszProductSuites);
	}

	if (hKey) {
		RegCloseKey(hKey);
	}

	return fValidated;
}

#endif
} // namespace fb_utils


