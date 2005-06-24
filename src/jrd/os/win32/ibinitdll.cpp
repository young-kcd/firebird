/*
 *      PROGRAM:        InterBase Window platforms
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
 */

#include "firebird.h"
#include <windows.h>
#include "../jrd/common.h"
#include "../../utilities/install/registry.h"
#include "../jrd/thread_proto.h"

HINSTANCE hIBDLLInstance;

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
		break;

	case DLL_PROCESS_DETACH:
#ifdef EMBEDDED
		JRD_shutdown_all(false);
#endif
		break;
	}

	return TRUE;
}

#endif /* SUPERSERVER */
