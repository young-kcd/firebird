/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		ClumpletWriter.cpp
 *	DESCRIPTION:	Secure handling of clumplet buffers
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

 */

#include "firebird.h"

#include "../common/classes/ClumpletWriter.h"
#include "fb_exception.h"

#include "../jrd/ibase.h"

namespace Firebird {

ClumpletWriter::ClumpletWriter(Kind k, size_t limit, UCHAR tag) : 
	ClumpletReader(k, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	initNewBuffer(tag);
	rewind();
}
 
ClumpletWriter::ClumpletWriter(MemoryPool& given_pool, Kind k, size_t limit, UCHAR tag) : 
	ClumpletReader(given_pool, k, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	initNewBuffer(tag);
	rewind();
}
 
void ClumpletWriter::initNewBuffer(UCHAR tag)
{
	switch (kind)
	{
		case SpbAttach:
			if (tag != isc_spb_version1)
			{
				dynamic_buffer.push(isc_spb_version);
			}
			dynamic_buffer.push(tag);
			break;
		case Tagged:
		case Tpb:
		case WideTagged:
			dynamic_buffer.push(tag);
			break;
		default:
			break;
	}
}

ClumpletWriter::ClumpletWriter(Kind k, size_t limit, const UCHAR* buffer, size_t buffLen, UCHAR tag) :
	ClumpletReader(k, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	if (buffer && buffLen) {
		dynamic_buffer.push(buffer, buffLen);
	}
	else {
		initNewBuffer(tag);
	}
	rewind();
}

ClumpletWriter::ClumpletWriter(MemoryPool& given_pool, Kind k, size_t limit,
							   const UCHAR* buffer, size_t buffLen, UCHAR tag) :
	ClumpletReader(given_pool, k, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	if (buffer && buffLen) {
		dynamic_buffer.push(buffer, buffLen);
	}
	else {
		initNewBuffer(tag);
	}
	rewind();
}

void ClumpletWriter::reset(UCHAR tag)
{
	dynamic_buffer.shrink(0);
	initNewBuffer(tag);
	rewind();
}

void ClumpletWriter::reset(const UCHAR* buffer, size_t buffLen)
{
	dynamic_buffer.clear();
	if (buffer && buffLen) {
		dynamic_buffer.push(buffer, buffLen);
	}
	else {
		UCHAR tag = (kind == SpbStart || kind == UnTagged || kind == WideUnTagged) ? getBufferTag() : 0;
		initNewBuffer(tag);
	}
	rewind();
}

void ClumpletWriter::size_overflow()
{
	fatal_exception::raise("Clumplet buffer size limit reached");
}

void ClumpletWriter::insertInt(UCHAR tag, SLONG value)
{
#if defined(WORDS_BIGENDIAN)
	UCHAR bytes[4];
	const UCHAR* ptr = reinterpret_cast<UCHAR*>(&value);
	bytes[0] = ptr[3];
	bytes[1] = ptr[2];
	bytes[2] = ptr[1];
	bytes[3] = ptr[0];
	insertBytesLengthCheck(tag, bytes, sizeof(bytes));
#else
	insertBytesLengthCheck(tag, reinterpret_cast<UCHAR*>(&value), sizeof(value));
#endif 
}

void ClumpletWriter::insertBigInt(UCHAR tag, SINT64 value)
{
#if defined(WORDS_BIGENDIAN)
	UCHAR bytes[8];
	const UCHAR* ptr = reinterpret_cast<UCHAR*>(&value);
	bytes[0] = ptr[7];
	bytes[1] = ptr[6];
	bytes[2] = ptr[5];
	bytes[3] = ptr[4];
	bytes[4] = ptr[3];
	bytes[5] = ptr[2];
	bytes[6] = ptr[1];
	bytes[7] = ptr[0];
	insertBytesLengthCheck(tag, bytes, sizeof(bytes));
#else
	insertBytesLengthCheck(tag, reinterpret_cast<UCHAR*>(&value), sizeof(value));
#endif 
}

void ClumpletWriter::insertString(UCHAR tag, const string& str)
{
	insertString(tag, str.c_str(), str.length());
}

void ClumpletWriter::insertPath(UCHAR tag, const PathName& str)
{
	insertString(tag, str.c_str(), str.length());
}

void ClumpletWriter::insertString(UCHAR tag, const char* str, size_t length)
{
	insertBytesLengthCheck(tag, reinterpret_cast<const UCHAR*>(str), length);
}

void ClumpletWriter::insertBytes(UCHAR tag, const UCHAR* bytes, size_t length)
{
	insertBytesLengthCheck(tag, bytes, length);
}

void ClumpletWriter::insertByte(UCHAR tag, const UCHAR byte)
{
	insertBytesLengthCheck(tag, &byte, 1);
}

void ClumpletWriter::insertBytesLengthCheck(UCHAR tag, const UCHAR* bytes, size_t length)
{
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.getCount()) {
		usage_mistake("write past EOF");
		return;
	}
	
	// Check length according to clumplet type
	ClumpletType t = getClumpletType(tag);
	UCHAR lenSize = 0;
	switch (t)
	{
	case Wide:
		if (length > MAX_ULONG)
		{
			string m;
			m.printf("attempt to store %d bytes in a clumplet", length);
			usage_mistake(m.c_str());
			return;
		}
		lenSize = 4;
		break;
	case TraditionalDpb:
		if (length > MAX_UCHAR)
		{
			string m;
			m.printf("attempt to store %d bytes in a clumplet with maximum size 255 bytes", length);
			usage_mistake(m.c_str());
			return;
		}
		lenSize = 1;
		break;
	case SingleTpb:
		if (length > 0)
		{
        	usage_mistake("attempt to store data in dataless clumplet");
			return;
		}
		break;
	case StringSpb:
		if (length > MAX_USHORT)
		{
			string m;
			m.printf("attempt to store %d bytes in a clumplet", length);
			usage_mistake(m.c_str());
			return;
		}
		lenSize = 2;
		break;
	case IntSpb:
		if (length != 4)
		{
			string m;
			m.printf("attempt to store %d bytes in a clumplet, need 4", length);
			usage_mistake(m.c_str());
			return;
		}
		break;
	case ByteSpb:
		if (length != 1)
		{
			string m;
			m.printf("attempt to store %d bytes in a clumplet, need 1", length);
			usage_mistake(m.c_str());
			return;
		}
		break;
	}

	// Check that resulting data doesn't overflow size limit
	if (dynamic_buffer.getCount() + length + lenSize + 1 > sizeLimit) {
		size_overflow();
	}

	// Insert the data
	size_t saved_offset = cur_offset;
	dynamic_buffer.insert(cur_offset++, tag);
	switch (lenSize)
	{
	case 1:
		dynamic_buffer.insert(cur_offset++, static_cast<UCHAR>(length));
		break;
	case 2:
		{
			USHORT value = static_cast<USHORT>(length);
			fb_assert(sizeof(USHORT) == 2);
			const UCHAR* ptr = reinterpret_cast<UCHAR*>(&value);
#if defined(WORDS_BIGENDIAN)
			UCHAR b[2];
			b[0] = ptr[1];
			b[1] = ptr[0];
			dynamic_buffer.insert(cur_offset, b, sizeof(b));
#else
			dynamic_buffer.insert(cur_offset, ptr, sizeof(value));
#endif
			cur_offset += 2;
		}
		break;
	case 4:
		{
			ULONG value = static_cast<ULONG>(length);
			fb_assert(sizeof(ULONG) == 4);
			const UCHAR* ptr = reinterpret_cast<UCHAR*>(&value);
#if defined(WORDS_BIGENDIAN)
			UCHAR b[4];
			b[0] = ptr[3];
			b[1] = ptr[2];
			b[2] = ptr[1];
			b[3] = ptr[0];
			dynamic_buffer.insert(cur_offset, b, sizeof(b));
#else
			dynamic_buffer.insert(cur_offset, ptr, sizeof(value));
#endif
			cur_offset += 4;
		}
		break;
	}
	dynamic_buffer.insert(cur_offset, bytes, length);
	cur_offset += length;
	size_t new_offset = cur_offset;
	cur_offset = saved_offset;
    adjustSpbState();
	cur_offset = new_offset;
}

void ClumpletWriter::insertTag(UCHAR tag)
{
	insertBytesLengthCheck(tag, 0, 0);
}


void ClumpletWriter::insertEndMarker(UCHAR tag)
{
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.getCount()) {
		usage_mistake("write past EOF");
		return;
	}

	// Check that resulting data doesn't overflow size limit
	if (cur_offset + 1 > sizeLimit) {
		size_overflow();
	}

	dynamic_buffer.shrink(cur_offset);
	dynamic_buffer.push(tag);

	cur_offset += 2; // Go past EOF to indicate we set the marker
}

void ClumpletWriter::deleteClumplet()
{
	const UCHAR* clumplet = getBuffer() + cur_offset;
	const UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		usage_mistake("write past EOF");
		return;
	}

	if (buffer_end - clumplet < 2) {
		// It appears we're erasing EOF marker
		dynamic_buffer.shrink(cur_offset);
	} 
	else {
		dynamic_buffer.removeCount(cur_offset, getClumpletSize(true, true, true));
	}
}

bool ClumpletWriter::deleteWithTag(UCHAR tag)
{
   bool rc = false;
   while (find(tag))
   {
       rc = true;
       deleteClumplet();
   }

   return rc;
}

} // namespace

