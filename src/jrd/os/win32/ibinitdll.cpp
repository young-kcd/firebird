/*
 *      PROGRAM:        InterBase Window platforms
 *      MODULE:         ibinitdll.c
 *      DESCRIPTION:    DLL entry/exit function
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
#include <windows.h>
#include "../../utilities/install/registry.h"

HINSTANCE hIBDLLInstance;

#ifdef GDS32
HINSTANCE hFBDLLInstance;
const char *FBDLLDIR = "bin\\";
const char *FBDLLNAME = "fbclient.dll";
#endif

#ifdef SUPERSERVER

#ifdef  _MSC_VER
BOOL WINAPI _CRT_INIT(HINSTANCE, DWORD, LPVOID);
#else
BOOL WINAPI _CRT_INIT(HINSTANCE HIdummy, DWORD DWdummy, LPVOID LPVdummy)
{
	return TRUE;
}
#endif

#else /* SUPERSERVER */

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID reserved)
{
	/* save instance value */
	hIBDLLInstance = h;

	switch (reason)	{

	case DLL_PROCESS_ATTACH:
#ifdef GDS32
		{
		char buffer[MAXPATHLEN];
		HKEY hkey_instances;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				REG_KEY_ROOT_INSTANCES, 0, KEY_READ,
				&hkey_instances) == ERROR_SUCCESS)
		{
			DWORD keytype;
			DWORD buflen = sizeof(buffer);
			if (RegQueryValueEx(hkey_instances, FB_DEFAULT_INSTANCE, 0,
					&keytype, reinterpret_cast<UCHAR*>(buffer),
					&buflen) == ERROR_SUCCESS && keytype == REG_SZ)
			{
				lstrcat(buffer, FBDLLDIR);
				lstrcat(buffer, FBDLLNAME);
				if (!GetModuleHandle(buffer) && !GetModuleHandle(FBDLLNAME))
				{
					hFBDLLInstance = LoadLibrary(buffer);
					if (!hFBDLLInstance)
					{
						hFBDLLInstance = LoadLibrary(FBDLLNAME);
					}
				}
			}
			RegCloseKey(hkey_instances);
		}
		}
#endif
		break;

	case DLL_PROCESS_DETACH:
#ifdef GDS32
		FreeLibrary(hFBDLLInstance);
#endif
#ifdef EMBEDDED
		THREAD_ENTER;
		JRD_shutdown_all();
#endif
		break;
	}

	return TRUE;
}

#endif /* SUPERSERVER */
