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
 * Nov-2003 Olivier Mascia: Initial implementation
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

static USHORT PatchVersion(const TEXT* gds32, WORD major, WORD minor,
	USHORT(*err_handler)(ULONG, const TEXT *));


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
 *	Do nothing, if sw_force isn't true and some other GDS32.DLL is present.
 *
 **************************************/

	// Get Windows System Directory
	TEXT sysdir[MAXPATHLEN];
	int len = GetSystemDirectory(sysdir, sizeof(sysdir));
	if (len == 0)
		return (*err_handler) (GetLastError(), "GetSystemDirectory()");

	TEXT fbdll[MAXPATHLEN];
	lstrcpy(fbdll, rootdir);
	lstrcat(fbdll, "\\bin\\FBCLIENT.DLL");
	
	TEXT workfile[MAXPATHLEN];
	lstrcpy(workfile, sysdir);
	lstrcat(workfile, "\\_gds32.dll");
	
	TEXT gds32[MAXPATHLEN];
	lstrcpy(gds32, sysdir);
	lstrcat(gds32, "\\gds32.dll");
	
	// Copy FBCLIENT.DLL as _GDS32.DLL (in WinSysDir).
	// Such a step is needed because the MoveFile techniques used later
	// can't span different volumes. So the intermediate target has to be
	// already on the same final destination volume as final GDS32.DLL.
	if (CopyFile(fbdll, workfile, FALSE) == 0)
	{
		return (*err_handler) (GetLastError(),
			"CopyFile(FBCLIENT.DLL, WinSysDir\\_gds32.dll)");
	}
	
	// Patch _GDS32.DLL File Version to 6.3.?.?.
	USHORT status = PatchVersion(workfile, GDSVER_MAJOR, GDSVER_MINOR, err_handler);
	if (status != FB_SUCCESS)
		return status;

	// Move _GDS32.DLL as GDS32.DLL
	if (MoveFile(workfile, gds32) == 0)
	{
		ULONG werr = GetLastError();
		if (werr !=  ERROR_ALREADY_EXISTS)
		{
			DeleteFile(workfile);
			return (*err_handler) (werr,
				"MoveFile(_gds32.dll, gds32.dll)");
		}
		
		// Failed moving because a destination GDS32.DLL file already exists
		if (! sw_force)
		{
			// Report the reason of failure to the caller
			DeleteFile(workfile);
			return FB_EXISTING_GDS32_FOUND;
		}
		
		// We are asked to -f[orce] the update.
		// Let's try again by attempting a remove of the destination
		if (DeleteFile(gds32) != 0)
		{
			// Success deleting the target, MoveFile should now succeed.
			if (MoveFile(workfile, gds32) != 0)
			{
				// Successfull !
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
					return (*err_handler) (werr, "MoveFileEx(delete gds32.dll)");
				}

				if ((*ntmove) (workfile, gds32, MOVEFILE_DELAY_UNTIL_REBOOT) == 0)
				{
					ULONG werr = GetLastError();
					FreeLibrary(kernel32);
					return (*err_handler) (werr, "MoveFileEx(replace gds32.dll)");
				}
					
				FreeLibrary(kernel32);
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
		lstrcat(sworkfile, "\\_gds32.dll");
		
		TEXT sgds32[MAXPATHLEN];
		lstrcpy(sgds32, ssysdir);
		lstrcat(sgds32, "\\gds32.dll");

		if (WritePrivateProfileString("rename", "NUL", sgds32,
				"WININIT.INI") == 0)
		{
			return (*err_handler) (GetLastError(),
				"WritePrivateProfileString(delete gds32.dll)");
		}

		if (WritePrivateProfileString("rename", gds32, sworkfile,
				"WININIT.INI") == 0)
		{
			return (*err_handler) (GetLastError(),
				"WritePrivateProfileString(replace gds32.dll)");
		}

		return FB_GDS32_COPY_REQUIRES_REBOOT;
	}
	
	return FB_SUCCESS;
}

USHORT GDSCLIENT_remove(const TEXT * rootdir,
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
 *	Do nothing, if the GDS32.DLL is not identified as the Firebird GDS32.DLL.
 *	This DLL is a clone of the FBCLIENT.DLL with its file version at 6.3.x.y.
 *
 **************************************/

	// Get Windows System Directory
	TEXT sysdir[MAXPATHLEN];
	int len = GetSystemDirectory(sysdir, sizeof(sysdir));
	if (len == 0)
		return (*err_handler) (GetLastError(), "GetSystemDirectory()");

	TEXT gds32[MAXPATHLEN];
	lstrcpy(gds32, sysdir);
	lstrcat(gds32, "\\gds32.dll");
	
	HANDLE hfile = CreateFile(gds32, GENERIC_READ,
		0 /* FILE_SHARE_NONE */, 0, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
		return FB_GDS32_NOT_FOUND;
	CloseHandle(hfile);

	DWORD dwUnused;
	DWORD rsize = GetFileVersionInfoSize(gds32, &dwUnused);
	if (rsize == 0)
		return (*err_handler) (GetLastError(), "GetFileVersionInfoSize()");

	BYTE* hver = new BYTE[rsize];
	if (! GetFileVersionInfo(gds32, 0, rsize, hver))
	{
		ULONG werr = GetLastError();
		delete [] hver;
		return (*err_handler) (werr, "GetFileVersionInfo()");
	}

	VS_FIXEDFILEINFO* ffi;
	UINT uiUnused;
	if (! VerQueryValue(hver, "\\", (void**)&ffi, &uiUnused))
	{
		ULONG werr = GetLastError();
		delete [] hver;
		return (*err_handler) (werr, "VerQueryValue()");
	}

	if ((ffi->dwFileVersionMS & 0xffff0000) != (GDSVER_MAJOR << 16) ||
		(ffi->dwFileVersionMS & 0x0000ffff) != GDSVER_MINOR ||
		(ffi->dwProductVersionMS & 0xffff0000) != (GDSVER_MAJOR << 16) ||
		(ffi->dwProductVersionMS & 0x0000ffff) != GDSVER_MINOR)
	{
		delete [] hver;
		return FB_CANNOT_REMOVE_ALIEN_GDS32;
	}

	delete [] hver;
	
	// Identified the GDS32.DLL as _most probably_ our specific version.
	// Let's attempt removing it.
	if (! DeleteFile(gds32))
		return FB_GDS32_PROBABLY_IN_USE;
	
	return FB_SUCCESS;
}

static USHORT PatchVersion(const TEXT* gds32, WORD major, WORD minor,
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
	ffi->dwFileVersionMS =		(major << 16) | minor;
	ffi->dwProductVersionMS =	(major << 16) | minor;

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

//
// EOF
//
