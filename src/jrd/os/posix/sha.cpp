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

#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/err.h>

namespace Jrd
{
	class OpenSslCryptSupport : public CryptSupport
	{
		friend class CryptSupport;
	private:
		OpenSslCryptSupport() : CryptSupport() { }
		
		void hash(Firebird::string& h, const Firebird::string& data)
		{
			Firebird::string binHash;

			binHash.resize(SHA_DIGEST_LENGTH);
			SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), 
				 data.length(), 
				 reinterpret_cast<unsigned char*>(binHash.begin()));

			base64(h, binHash);
		}

		void random(Firebird::string& r, size_t length)
		{
			Firebird::string binRand;
			
			binRand.resize(length);
			if (! RAND_bytes(reinterpret_cast<unsigned char*>(binRand.begin()), 
							 length))
			{
				Firebird::system_call_failed("RAND_bytes", ERR_peek_last_error());
			}
			
			base64(r, binRand);
			r.resize(length, '$');
		}
	};

	Firebird::InitInstance<CryptSupport, CryptSupport> iCS;

	CryptSupport* CryptSupport::init()
	{
		return FB_NEW(*getDefaultMemoryPool()) OpenSslCryptSupport;
	}
	
	CryptSupport& CryptSupport::get()
	{
		return iCS();
	}
}
