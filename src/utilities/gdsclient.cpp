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
	USHORT status = PatchVersion(workfile, 6, 3, err_handler);
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

	return (*err_handler) (0, "Not yet implemented");
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
 **************************************/

	HMODULE hmod = LoadLibraryEx(gds32, 0, LOAD_LIBRARY_AS_DATAFILE);
	if (hmod == 0)
	{
		return (*err_handler) (GetLastError(),
			"LoadLibraryEx(WinSysDir\\_gds32.dll)");
	}

	HRSRC hres = FindResource(hmod, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hres == 0)
	{
		ULONG werr = GetLastError();
		FreeLibrary(hmod);
		return (*err_handler) (werr, "FindResource()");
	}

	DWORD rsize = SizeofResource(hmod, hres);
	if (rsize == 0)
	{
		ULONG werr = GetLastError();
		FreeLibrary(hmod);
		return (*err_handler) (werr, "SizeOfResource()");
	}

	HGLOBAL hload = LoadResource(hmod, hres);
	if (hload == 0)
	{
		ULONG werr = GetLastError();
		FreeLibrary(hmod);
		return (*err_handler) (werr, "LoadResource()");
	}

	void* hlock = LockResource(hload);
	if (hlock == 0)
	{
		ULONG werr = GetLastError();
		FreeLibrary(hmod);
		return (*err_handler) (werr, "LockResource()");
	}

	// The above memory pointed to by hlock is write-protected.
	// Get a writable copy of the resource block.
	char* hver = new char[rsize];
	memcpy(hver, hlock, rsize);
	FreeResource(hload);
	FreeLibrary(hmod);
	
	VS_FIXEDFILEINFO* ffi = (VS_FIXEDFILEINFO*)(hver+40);

	ffi->dwFileVersionMS =		(major << 16) | minor;
	ffi->dwProductVersionMS =	(major << 16) | minor;

	/*	
	ib_printf("Signature: %8.8x\n", ffi->dwSignature);
	ib_printf("FileVersionMS : %8.8x\n", ffi->dwFileVersionMS);
	ib_printf("FileVersionLS : %8.8x\n", ffi->dwFileVersionLS);
	ib_printf("ProductVersionMS : %8.8x\n", ffi->dwProductVersionMS);
	ib_printf("ProductVersionLS : %8.8x\n", ffi->dwProductVersionLS);
	*/

	HANDLE hupd = BeginUpdateResource(gds32, TRUE);
	if (hupd == 0)
	{
		ULONG werr = GetLastError();
		delete [] hver;
		return (*err_handler) (werr, "BeginUpdateResource()");
	}

	if (! UpdateResource(hupd, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO),
		 MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), hver, rsize))
	{
		ULONG werr = GetLastError();
		delete [] hver;
		EndUpdateResource(hupd, TRUE);	// TRUE = discard updates
		return (*err_handler) (werr, "UpdateResource()");
	}

	if (! EndUpdateResource(hupd, FALSE))	// FALSE == don't discard
	{
		ULONG werr = GetLastError();
		delete [] hver;
		EndUpdateResource(hupd, TRUE);	// Discard
		return (*err_handler) (werr, "EndUpdateResource()");
	}

	delete [] hver;	// It is important to delete AFTER EndUpdateResource !

	return FB_SUCCESS;
}

//
// EOF
//
