/*
 *	PROGRAM:	Common Library
 *	MODULE:		Hash.cpp
 *	DESCRIPTION:	Hash of data
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
 *  The Original Code was created by Inprise Corporation
 *  and its predecessors. Portions created by Inprise Corporation are
 *  Copyright (C) Inprise Corporation.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 */

#include "firebird.h"
#include "../common/classes/Hash.h"

namespace Firebird
{

static unsigned int basicHash(const unsigned char* value, unsigned int length)
{
	unsigned int hash_value = 0;
	unsigned char* p;
	const unsigned char* q = value;
	while (length >= 4)
	{
		p = (unsigned char*) &hash_value;
		p[0] += q[0];
		p[1] += q[1];
		p[2] += q[2];
		p[3] += q[3];
		length -= 4;
		q += 4;
	}
	p = (unsigned char*) &hash_value;
    if (length >= 2)
    {
		p[0] += q[0];
		p[1] += q[1];
        length -= 2;
    }
    if (length)
    {
		q += 2;
        *p += *q;
    }
	return hash_value;
}

#if defined(_M_IX86) || defined(_M_X64) || defined(__x86_64__) || defined(__i386__)

#ifdef _MSC_VER

#include <intrin.h>
#define bit_SSE4_2	(1 << 20)
// MS VC has its own definition of __cpuid
static bool SSE4_2Supported()
{
	int flags[4];
	__cpuid(flags, 1);
	return (flags[2] & bit_SSE4_2) != 0;
}

#else

#include <cpuid.h>
// GCC - its own
static bool SSE4_2Supported()
{
	unsigned int eax,ebx,ecx,edx;
	__cpuid(1, eax, ebx, ecx, edx);
	return (ecx & bit_SSE4_2) != 0;
}

#endif

unsigned int CRC32C(const unsigned char* value, unsigned int length);

someHashFunc someHash = SSE4_2Supported()?CRC32C:basicHash;
#else
someHashFunc someHash = basicHash;
#endif // Architecture check

const char* hashName = someHash == CRC32C? "CRC32C": "Basic";

} // namespace