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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 * Created by: Alex Peshkov <AlexPeshkov@users.sourceforge.net>
 * and all contributors, signed below.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include <../jrd/os/sha.h>

// minimum wun32 version: win95 / winnt4
#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <wincrypt.h>

namespace Jrd
{
	class Win32CryptSupport : public CryptSupport
	{
	private:
		friend CryptSupport;
	private:
		HCRYPTPROV hProv;
	private:
		Win32CryptSupport() : CryptSupport() 
		{
			// Acquire crypto-provider context handle.
			if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
			{
				return;
			}
			if (GetLastError() == NTE_BAD_KEYSET)
			{
				// A common cause of this error is that the key container does not exist. 
				// To create a key container, call CryptAcquireContext 
				// using the CRYPT_NEWKEYSET flag. 
				if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				{
					return;
				}
			}
			Firebird::system_call_failed("CryptAcquireContext", GetLastError());
		}
		
		~Win32CryptSupport()
		{
			CryptReleaseContext(hProv, 0);
		}

		void hash(Firebird::string& h, const Firebird::string& data)
		{
			HCRYPTHASH hHash;
			if (! CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) 
			{
				Firebird::system_call_failed("CryptCreateHash", GetLastError());
			}
			
			if (! CryptHashData(hHash, (BYTE*) (data.c_str()), data.length(), 0)) 
			{
				Firebird::system_call_failed("CryptHashData", GetLastError());
			}
			
			DWORD hashSize = 0;
			DWORD sizeSize = sizeof hashSize;
			if(! CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*) (&hashSize), &sizeSize, 0)) 
			{
				Firebird::system_call_failed("CryptGetHashParam", GetLastError());
			}
			
			Firebird::string binHash;
			binHash.resize(hashSize);
			if(! CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*) (binHash.begin()), &hashSize, 0)) 
			{
				Firebird::system_call_failed("CryptGetHashParam", GetLastError());
			}

			if (! CryptDestroyHash(hHash))
			{
				Firebird::system_call_failed("CryptDestroyHash", GetLastError());
			}

			base64(h, binHash);
		}

		void random(Firebird::string& r, size_t length)
		{
			Firebird::string binRand;
			binRand.resize(length);
			if (! CryptGenRandom(hProv, length, (BYTE*) (binRand.begin())))
			{
				Firebird::system_call_failed("CryptGenRandom", GetLastError());
			}
			base64(r, binRand);
			r.resize(length, '$');
		}
	};

	Firebird::InitInstance<CryptSupport, CryptSupport> iCS;

	CryptSupport* CryptSupport::init()
	{
		return FB_NEW(*getDefaultMemoryPool()) Win32CryptSupport;
	}
	
	CryptSupport& CryptSupport::get()
	{
		return iCS();
	}
}
