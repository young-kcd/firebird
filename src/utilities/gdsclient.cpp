/*
 *	PROGRAM:		Windows NT GDS Client Library Installation
 *	MODULE:			gdsclient.cpp
 *	DESCRIPTION:	Functions which install the FBCLIENT.DLL as GDS32.DLL
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
 *
 * Dec-2003 Olivier Mascia: Initial implementation
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <windows.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install_nt.h"
#include "../utilities/gdsclient_proto.h"

#define GDSVER_MAJOR	6
#define GDSVER_MINOR	3
#define SHARED_KEY	"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\SharedDLLs"

namespace {

	USHORT GetVersion(const TEXT* gds32, DWORD& verMS, DWORD& verLS,
			USHORT(*err_handler)(ULONG, const TEXT *));

	USHORT PatchVersion(const TEXT* gds32, DWORD verMS,
		USHORT(*err_handler)(ULONG, const TEXT *));

	USHORT IncrementSharedCount(const TEXT* gds32,
		USHORT(*err_handler)(ULONG, const TEXT *));

	USHORT DecrementSharedCount(const TEXT* gds32, bool sw_force,
		USHORT(*err_handler)(ULONG, const TEXT *));

};

//
//	--- Public Functions ---
//

USHORT GDSCLIENT_install(const TEXT * rootdir, bool sw_force,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	G D S C L I E N T _ i n s t a l l
 *
 **************************************
 *
 * Functional description
 *	Install FBCLIENT.DLL as GDS32.DLL in the Windows System directory.
 *
 *	When installing and -force is NOT used then 
 * 
 *		test for existence of client library
 *		if earlier version then copy, increment shared library count
 *		if same version then do nothing
 *		if later version then do nothing
 *
 *	If -force is supplied then
 * 
 *		test for existence of client library
 *		if the version doesn't match then copy, increment shared library count
 *		else do nothing 
 *
 **************************************/

	// Get Windows System Directory
	TEXT sysdir[MAXPATHLEN];
	int len = GetSystemDirectory(sysdir, sizeof(sysdir));
	if (len == 0)
		return (*err_handler) (GetLastError(), "GetSystemDirectory()");

	// Compute newverMS and newverLS. They will contain the full GDS version
	// number that we intend to install to WinSysDir.
	TEXT fbdll[MAXPATHLEN];
	lstrcpy(fbdll, rootdir);
	lstrcat(fbdll, "\\bin\\FBCLIENT.DLL");

	DWORD newverMS = 0, newverLS = 0;
	USHORT status = GetVersion(fbdll, newverMS, newverLS, err_handler);
	if (status != FB_SUCCESS)
		return status;
	newverMS = (GDSVER_MAJOR << 16) | GDSVER_MINOR;

	// Check for existence and version of current installed GDS32.DLL
	TEXT gds32[MAXPATHLEN];
	lstrcpy(gds32, sysdir);
	lstrcat(gds32, "\\GDS32.DLL");

	DWORD gdsverMS = 0, gdsverLS = 0;
	status = GetVersion(gds32, gdsverMS, gdsverLS, err_handler);
	if (status == FB_FAILURE)
		return FB_FAILURE;

	if (status == FB_SUCCESS)
	{
		// gds32.dll do exists
		if (! sw_force)
		{
			if (gdsverMS > newverMS)
				return FB_NEWER_GDS32_FOUND;
			if (gdsverMS == newverMS && gdsverLS > newverLS)
				return FB_NEWER_GDS32_FOUND;
		}
		if (gdsverMS == newverMS && gdsverLS == newverLS)
			return FB_SAME_GDS32_FOUND;
	}

	// Copy FBCLIENT.DLL as _GDS32.DLL (in WinSysDir).
	// Such a step is needed because the MoveFile techniques used later
	// can't span different volumes. So the intermediate target has to be
	// already on the same final destination volume as final GDS32.DLL.
	TEXT workfile[MAXPATHLEN];
	lstrcpy(workfile, sysdir);
	lstrcat(workfile, "\\_GDS32.DLL");
	
	if (CopyFile(fbdll, workfile, FALSE) == 0)
	{
		return (*err_handler) (GetLastError(),
			"CopyFile(FBCLIENT.DLL, WinSysDir\\_GDS32.DLL)");
	}

	// Patch _GDS32.DLL File Version
	status = PatchVersion(workfile, newverMS, err_handler);
	if (status != FB_SUCCESS)
		return status;

	// Move _GDS32.DLL as GDS32.DLL
	if (MoveFile(workfile, gds32) == 0)
	{
		// MoveFile failed, this is expected if GDS32.DLL already exists
		ULONG werr = GetLastError();
		if (werr != ERROR_ALREADY_EXISTS)
		{
			DeleteFile(workfile);
			return (*err_handler) (werr,
				"MoveFile(_GDS32.DLL, GDS32.DLL)");
		}
		
		// Failed moving because a destination GDS32.DLL file already exists
		// Let's try again by attempting a remove of the destination
		if (DeleteFile(gds32) != 0)
		{
			// Success deleting the target, MoveFile should now succeed.
			if (MoveFile(workfile, gds32) != 0)
			{
				// Successfull !
				IncrementSharedCount(gds32, err_handler);
				return FB_SUCCESS;
			}
		}
		
		// Deleting the target failed OR moving after target delete failed.
		// Let's try once more using reboot-time update.
		HMODULE kernel32 = LoadLibrary("KERNEL32.DLL");
		if (kernel32 != 0)
		{
			typedef BOOL __stdcall proto_ntmove(LPCSTR, LPCSTR, DWORD);
			proto_ntmove* ntmove =
				(proto_ntmove*)GetProcAddress(kernel32, "MoveFileExA");
			if (ntmove != 0)
			{
				// We are definitely running on a system supporting the
				// MoveFileEx API. Let's use it for setting up the reboot-copy.

				if ((*ntmove) (gds32, 0, MOVEFILE_DELAY_UNTIL_REBOOT) == 0)
				{
					ULONG werr = GetLastError();
					FreeLibrary(kernel32);
					return (*err_handler) (werr, "MoveFileEx(delete GDS32.DLL)");
				}

				if ((*ntmove) (workfile, gds32, MOVEFILE_DELAY_UNTIL_REBOOT) == 0)
				{
					ULONG werr = GetLastError();
					FreeLibrary(kernel32);
					return (*err_handler) (werr, "MoveFileEx(replace GDS32.DLL)");
				}
					
				FreeLibrary(kernel32);
				IncrementSharedCount(gds32, err_handler);
				return FB_GDS32_COPY_REQUIRES_REBOOT;
			}

			FreeLibrary(kernel32);
		}

		// We are running a system which doesn't support MoveFileEx API.
		// Let's use the WININIT.INI method (Win95/98/Me).
		// For this we need "short file names".

		TEXT ssysdir[MAXPATHLEN];
		if (GetShortPathName(sysdir, ssysdir, sizeof(ssysdir)) == 0)
			return (*err_handler) (GetLastError(), "GetShortPathName()");

		TEXT sworkfile[MAXPATHLEN];
		lstrcpy(sworkfile, ssysdir);
		lstrcat(sworkfile, "\\_GDS32.DLL");
		
		TEXT sgds32[MAXPATHLEN];
		lstrcpy(sgds32, ssysdir);
		lstrcat(sgds32, "\\GDS32.DLL");

		if (WritePrivateProfileString("rename", "NUL", sgds32,
				"WININIT.INI") == 0)
		{
			return (*err_handler) (GetLastError(),
				"WritePrivateProfileString(delete GDS32.DLL)");
		}

		if (WritePrivateProfileString("rename", gds32, sworkfile,
				"WININIT.INI") == 0)
		{
			return (*err_handler) (GetLastError(),
				"WritePrivateProfileString(replace GDS32.DLL)");
		}

		IncrementSharedCount(gds32, err_handler);
		return FB_GDS32_COPY_REQUIRES_REBOOT;
	}
	else
	{
		// Straight plain MoveFile succeeded immediately.
		IncrementSharedCount(gds32, err_handler);
		return FB_SUCCESS;
	}
}

USHORT GDSCLIENT_remove(const TEXT * rootdir, bool sw_force,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	G D S C L I E N T _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove Firebird GDS32.DLL from the Windows System directory.
 *
 *	when removing and -force is NOT used
 *     
 *		test for existence of client library
 *		if version matches then decrement shared library count
 *			if count=0 then delete library, remove entry from shared library 
 *		if version doesn't match then do nothing. It is not ours.
 *	
 *	when removing and -force IS used
 *
 *		test for existence of client library
 *		if version matches then delete library, remove entry for shared library
 *		if version doesn't match then do nothing. It is not ours.
 *
 **************************************/

	// Get Windows System Directory
	TEXT sysdir[MAXPATHLEN];
	int len = GetSystemDirectory(sysdir, sizeof(sysdir));
	if (len == 0)
		return (*err_handler) (GetLastError(), "GetSystemDirectory()");

	// Compute ourverMS and ourverLS. They will contain the full GDS version
	// number that we could have installed to WinSysDir.
	TEXT fbdll[MAXPATHLEN];
	lstrcpy(fbdll, rootdir);
	lstrcat(fbdll, "\\bin\\FBCLIENT.DLL");

	DWORD ourverMS = 0, ourverLS = 0;
	USHORT status = GetVersion(fbdll, ourverMS, ourverLS, err_handler);
	if (status != FB_SUCCESS)
		return status;
	ourverMS = (GDSVER_MAJOR << 16) | GDSVER_MINOR;

	// Check for existence and version of current installed GDS32.DLL
	TEXT gds32[MAXPATHLEN];
	lstrcpy(gds32, sysdir);
	lstrcat(gds32, "\\GDS32.DLL");

	DWORD gdsverMS = 0, gdsverLS = 0;
	status = GetVersion(gds32, gdsverMS, gdsverLS, err_handler);
	if (status != FB_SUCCESS)
		return status;	// FB_FAILURE or FB_GDS32_NOT_FOUND

	if (gdsverMS != ourverMS || gdsverLS != ourverLS)
		return FB_CANNOT_REMOVE_ALIEN_GDS32;

	status = DecrementSharedCount(gds32, sw_force, err_handler);
	if (status == FB_SHARED_COUNT_ZERO)
	{
		if (! DeleteFile(gds32))
		{
			// Could not delete the file, restore shared count
			IncrementSharedCount(gds32, err_handler);
			return FB_GDS32_PROBABLY_IN_USE;
		}
	}

	return FB_SUCCESS;
}

USHORT GDSCLIENT_query(ULONG& verMS, ULONG& verLS, ULONG& sharedCount,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	G D S C L I E N T _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Returns version info and shared dll count of the currently installed
 *	GDS32.DLL. Eventually return FB_GDS32_NOT_FOUND.
 *
 **************************************/

	// Get Windows System Directory
	TEXT sysdir[MAXPATHLEN];
	int len = GetSystemDirectory(sysdir, sizeof(sysdir));
	if (len == 0)
		return (*err_handler) (GetLastError(), "GetSystemDirectory()");

	TEXT gds32[MAXPATHLEN];
	lstrcpy(gds32, sysdir);
	lstrcat(gds32, "\\GDS32.DLL");

	verMS = verLS = sharedCount = 0;
	USHORT status = GetVersion(gds32, verMS, verLS, err_handler);
	if (status != FB_SUCCESS)
		return status;	// FB_FAILURE or FB_GDS32_NOT_FOUND

	return FB_SUCCESS;
}

//
//	--- Local Functions ---
//

namespace {

USHORT GetVersion(const TEXT* gds32, DWORD& verMS, DWORD& verLS,
		USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	G e t V e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Get the file version of the gds32.dll whose full path and name is given.
 *	Returns a status value to report a failure, a non-existing file or success.
 *	'version' is only updated in case of success.
 *
 **************************************/

	HANDLE hfile = CreateFile(gds32, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
		return FB_GDS32_NOT_FOUND;

	// We'll keep hfile opened until we have read version so that the file
	// can't be deleted between check for existence and version read.
	
	DWORD dwUnused;
	DWORD rsize = GetFileVersionInfoSize(const_cast<TEXT*>(gds32), &dwUnused);
	if (rsize == 0)
	{
		ULONG werr = GetLastError();
		CloseHandle(hfile);
		return (*err_handler) (werr, "GetFileVersionInfoSize()");
	}

	BYTE* hver = new BYTE[rsize];
	if (! GetFileVersionInfo(const_cast<TEXT*>(gds32), 0, rsize, hver))
	{
		ULONG werr = GetLastError();
		delete [] hver;
		CloseHandle(hfile);
		return (*err_handler) (werr, "GetFileVersionInfo()");
	}
	CloseHandle(hfile);	// Not needed anymore

	VS_FIXEDFILEINFO* ffi;
	UINT uiUnused;
	if (! VerQueryValue(hver, "\\", (void**)&ffi, &uiUnused))
	{
		ULONG werr = GetLastError();
		delete [] hver;
		return (*err_handler) (werr, "VerQueryValue()");
	}

	verMS = ffi->dwFileVersionMS;
	verLS = ffi->dwFileVersionLS;

	delete [] hver;
	return FB_SUCCESS;
}

USHORT PatchVersion(const TEXT* gds32, DWORD verMS,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	P a t c h V e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Patches the FileVersion and ProductVersion of the GDS32.DLL whose full
 *	path is given as "gds32" parameter. The function only updates the major
 *	and minor version numbers, leaving the sub-minor and build intact.
 *	We typically use this trick to build a GDS32.DLL whose version is 6.3
 *	from our FBCLIENT.DLL whose version is 1.5.
 *
 *	The politically correct way (speaking of Win32 API) of changing the
 *	version info, should involve using GetFileVersionInfo() and VerQueryValue()
 *	to read the existing version resource, and using BeginUpdateResource(),
 *	UpdateResource() and EndUpdateResource() to actually update the dll file.
 *	Unfortunately those last 3 APIs are not implemented on Win95/98/Me.
 *
 *	Therefore this function proceeds by straight hacking of the dll file.
 *	This is not intellectually satisfactory, but does work and fits the bill.
 *
 **************************************/

	HANDLE hfile = CreateFile(gds32, GENERIC_READ | GENERIC_WRITE,
		0 /* FILE_SHARE_NONE */, 0, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (hfile == INVALID_HANDLE_VALUE)
		return (*err_handler) (GetLastError(), "CreateFile()");

	DWORD fsize = GetFileSize(hfile, 0);

	HANDLE hmap = CreateFileMapping(hfile, 0,
		PAGE_READWRITE | SEC_COMMIT, 0, 0, 0);
	if (hmap == 0)
	{
		ULONG werr = GetLastError();
		CloseHandle(hfile);
		return (*err_handler) (werr, "CreateFileMapping()");
	}

	BYTE* mem = static_cast<BYTE*>(MapViewOfFile(hmap,
		FILE_MAP_WRITE, 0, 0, 0));
	if (mem == 0)
	{
		ULONG werr = GetLastError();
		CloseHandle(hmap);
		CloseHandle(hfile);
		return (*err_handler) (werr, "MapViewOfFile()");
	}

	BYTE* p = mem;
	BYTE* end = p + fsize;
	BYTE lookup[] = {'V', 0, 'S', 0, '_', 0,
		'V', 0, 'E', 0, 'R', 0, 'S', 0, 'I', 0, 'O', 0, 'N', 0, '_', 0,
		'I', 0, 'N', 0, 'F', 0, 'O', 0, 0, 0, 0, 0, 0xbd, 0x04, 0xef, 0xfe};
	int i = 0;
	while (p < end)
	{
		if (*p++ == lookup[i])
		{
			if (++i == sizeof(lookup)) break;
		}
		else i = 0;
	}
	
	if (p >= end)
	{
		UnmapViewOfFile(mem);
		CloseHandle(hmap);
		CloseHandle(hfile);
		return (*err_handler) (0, "Could not patch the version info resource.");
	}

	// The VS_FIXEDFILEINFO structure starts with the 4 bytes signature
	// (which are the last 4 bytes of the 'lookup' buffer above).
	VS_FIXEDFILEINFO* ffi = (VS_FIXEDFILEINFO*)(p - 4);
	ffi->dwFileVersionMS = verMS;
	ffi->dwProductVersionMS = verMS;

	/*
	ib_printf("Signature: %8.8x\n", ffi->dwSignature);
	ib_printf("FileVersionMS : %8.8x\n", ffi->dwFileVersionMS);
	ib_printf("FileVersionLS : %8.8x\n", ffi->dwFileVersionLS);
	ib_printf("ProductVersionMS : %8.8x\n", ffi->dwProductVersionMS);
	ib_printf("ProductVersionLS : %8.8x\n", ffi->dwProductVersionLS);
	*/
		
	UnmapViewOfFile(mem);
	CloseHandle(hmap);
	CloseHandle(hfile);

	return FB_SUCCESS;
}

USHORT IncrementSharedCount(const TEXT* gds32,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	I n c r e m e n t S h a r e d C o u n t
 *
 **************************************
 *
 * Functional description
 *	Increment the registry Shared Count of a module whose full path & file
 *	name is given in parameter 'gds32'. Typically used to increment the
 *	shared count of the GDS32.DLL after its installation to WinSysDir. But
 *	can be used for just about any other shared component.
 *	User has to have enough rights to write to the HKLM key tree.
 *
 **************************************/

	HKEY hkey;
	LONG status = RegCreateKeyEx(HKEY_LOCAL_MACHINE, SHARED_KEY,
		0, "", REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, 0, &hkey, 0);
	if (status != ERROR_SUCCESS)
		return (*err_handler) (status, "RegCreateKeyEx");

	DWORD type, size;
	DWORD count = 0;
	RegQueryValueEx(hkey, gds32, NULL, &type,
		reinterpret_cast<BYTE*>(&count), &size);

	++count;

	status = RegSetValueEx(hkey, gds32, 0, REG_DWORD,
		reinterpret_cast<BYTE*>(&count), sizeof(DWORD));
	if (status != ERROR_SUCCESS)
	{
		RegCloseKey(hkey);
		return (*err_handler) (status, "RegSetValueEx");
	}
	RegCloseKey(hkey);

	return FB_SUCCESS;
}

USHORT DecrementSharedCount(const TEXT* gds32, bool sw_force,
	USHORT(*err_handler)(ULONG, const TEXT *))
{
/**************************************
 *
 *	D e c r e m e n t S h a r e d C o u n t
 *
 **************************************
 *
 * Functional description
 *	Decrements the registry Shared Count of a module whose full path & file
 *	name is given in parameter 'gds32'. Typically used to decrement the
 *	shared count of the GDS32.DLL prior to its eventual removal from WinSysDir.
 *	Can be used for just about any other shared component.
 *	User has to have enough rights to write to the HKLM key tree.
 *	If the shared count reaches zero (or was already zero), the shared count
 *	value is removed and FB_SHARED_COUNT_ZERO is returned.
 *	If the sw_force is set, the shared count value is removed and
 *	FB_SHARED_COUNT_ZERO returned too).
 *	FB_SUCCESS is returned when the count was decremented without reaching
 *	zero. FB_FAILURE is returned only in cases of registry access failures.
 *
 **************************************/

	HKEY hkey;
	DWORD disp;
	LONG status = RegCreateKeyEx(HKEY_LOCAL_MACHINE, SHARED_KEY,
		0, "", REG_OPTION_NON_VOLATILE, KEY_READ|KEY_WRITE, NULL, &hkey, &disp);
	if (status != ERROR_SUCCESS)
		return (*err_handler) (status, "RegCreateKeyEx");

	DWORD size;
	LONG count = 0;
	if (! sw_force)
	{
		DWORD type;
		RegQueryValueEx(hkey, gds32, NULL, &type,
			reinterpret_cast<BYTE*>(&count), &size);

		--count;
	}

	if (count > 0)
	{
		status = RegSetValueEx(hkey, gds32, 0, REG_DWORD,
			reinterpret_cast<BYTE*>(&count), sizeof(DWORD));
		if (status != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return (*err_handler) (status, "RegSetValueEx");
		}
		RegCloseKey(hkey);
		return FB_SUCCESS;
	}
	else
	{
		status = RegDeleteValue(hkey, gds32);
		if (status != ERROR_SUCCESS)
		{
			RegCloseKey(hkey);
			return (*err_handler) (status, "RegDeleteValue");
		}
		RegCloseKey(hkey);
		return FB_SHARED_COUNT_ZERO;
	}
}

};	// namespace { }

//
// EOF
//
