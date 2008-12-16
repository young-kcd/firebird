/*
 *      PROGRAM:        Firebird Windows platforms
 *      MODULE:         ibinitdll.cpp
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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <windows.h>
#include "../common/classes/fb_string.h"
#include "../jrd/os/config_root.h"
#include "../jrd/os/path_utils.h"

using namespace Firebird;


BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID reserved)
{
#if defined(EMBEDDED)
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			extern HINSTANCE hDllInst;
			extern BOOL bEmbedded;
	
			hDllInst = h;
			bEmbedded = true;

			char filename[MAX_PATH];
			GetModuleFileName(h, filename, sizeof(filename));

			PathName dir, file;
			PathUtils::splitLastComponent(dir, file, filename);

			ConfigRoot::setInstallDirectory(dir.c_str());

			break;
		}
	}
#endif

	return TRUE;
}
