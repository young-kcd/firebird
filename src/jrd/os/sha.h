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

#ifndef JRD_OS_SHA_H
#define JRD_OS_SHA_H

#include "firebird.h"
#include "fb_string.h"
#include "../common/classes/init.h"

namespace Jrd 
{
	class CryptSupport
	{
	public:
		virtual ~CryptSupport() {}
		// hash and random return base64-coded values
		virtual void hash(Firebird::string& h, const Firebird::string& data) = 0;
		virtual void random(Firebird::string& r, size_t length) = 0;
		static CryptSupport& get();
		static CryptSupport* init();

	protected:
		CryptSupport() {}

		static char conv_bin2ascii(ULONG l)
		{
			return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[l & 0x3f];
		}

		static void base64(Firebird::string& b64, const Firebird::string& bin)
		{
			b64.erase();
			const unsigned char* f = 
				reinterpret_cast<const unsigned char*>(bin.c_str());
			for (int i = bin.length(); i > 0; i -= 3, f += 3)
			{
				if (i >= 3)
				{
					ULONG l = (ULONG(f[0]) << 16) |
							  (ULONG(f[1]) <<  8) | f[2];
					b64 += conv_bin2ascii(l >> 18);
					b64 += conv_bin2ascii(l >> 12);
					b64 += conv_bin2ascii(l >> 6);
					b64 += conv_bin2ascii(l);
				}
				else
				{
					ULONG l = ULONG(f[0]) << 16;
					if (i == 2) l |= (ULONG(f[1]) << 8);

					b64 += conv_bin2ascii(l >> 18);
					b64 += conv_bin2ascii(l >> 12);
					b64 += (i == 1 ? '=' : conv_bin2ascii(l >> 6));
					b64 += '=';
				}
			}
		}
	};
}

#endif //JRD_OS_SHA_H
