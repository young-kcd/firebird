/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.cpp
 *	DESCRIPTION:	Portable GUID (win32)
 *
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
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: guid.cpp,v 1.9 2004-11-16 08:34:35 robocop Exp $
 *
 */
 
// minimum win32 version: win95 / winnt4
#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <objbase.h>
#include <stdio.h>

#include "../jrd/os/guid.h"
#include "firebird.h"
#include "fb_exception.h"

void GenerateRandomBytes(void* buffer, size_t size)
{
	HCRYPTPROV hProv;

	// Acquire crypto-provider context handle.
	if (! CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			// A common cause of this error is that the key container does not exist. 
			// To create a key container, call CryptAcquireContext 
			// using the CRYPT_NEWKEYSET flag. 
			if (! CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				Firebird::system_call_failed::raise("CryptAcquireContext");
			}
		}
		else 
		{
			Firebird::system_call_failed::raise("CryptAcquireContext");
		}
	}

	if (! CryptGenRandom(hProv, size, static_cast<UCHAR*>(buffer)))
	{
		Firebird::system_call_failed::raise("CryptGenRandom");
	}
	CryptReleaseContext(hProv, 0);
}

void GenerateGuid(FB_GUID* guid) {
	const HRESULT error = CoCreateGuid((GUID*)guid);
	if (!SUCCEEDED(error))
		Firebird::system_call_failed::raise("CoCreateGuid", error);
}

void GuidToString(char* buffer, const FB_GUID* guid) {
	sprintf(buffer, "{%04hX%04hX-%04hX-%04hX-%04hX-%04hX%04hX%04hX}", 
		guid->data[0], guid->data[1], guid->data[2], guid->data[3],
		guid->data[4], guid->data[5], guid->data[6], guid->data[7]);
}

void StringToGuid(FB_GUID* guid, const char* buffer) {
	sscanf(buffer, "{%04hX%04hX-%04hX-%04hX-%04hX-%04hX%04hX%04hX}", 
		&guid->data[0], &guid->data[1], &guid->data[2], &guid->data[3],
		&guid->data[4], &guid->data[5], &guid->data[6], &guid->data[7]);
}

