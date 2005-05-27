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

namespace Firebird {

ClumpletWriter::ClumpletWriter(bool isTagged, size_t limit, UCHAR tag) : 
	ClumpletReader(isTagged, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	if (isTagged) {
		dynamic_buffer.push(tag);
	}
}
 
ClumpletWriter::ClumpletWriter(bool isTagged, size_t limit, const UCHAR* buffer, size_t buffLen, UCHAR tag) :
	ClumpletReader(isTagged, NULL, 0), sizeLimit(limit), dynamic_buffer(getPool()) 
{
	if (buffer && buffLen) {
		dynamic_buffer.push(buffer, buffLen);
	}
	else if (isTagged) {
		dynamic_buffer.push(tag);
	}
}

void ClumpletWriter::reset(UCHAR tag)
{
	dynamic_buffer.shrink(0);

	if (!mIsTagged) {
		usage_mistake("buffer is not tagged");

		cur_offset = 0;
		return;
	}

	cur_offset = 1;
	dynamic_buffer.push(tag);
}

void ClumpletWriter::reset(const UCHAR* buffer, size_t buffLen)
{
	cur_offset = mIsTagged ? 1 : 0;
	dynamic_buffer.clear();
	dynamic_buffer.push(buffer, buffLen);
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
	insertBytesNoLengthCheck(tag, bytes, sizeof(bytes));
#else
	insertBytesNoLengthCheck(tag, reinterpret_cast<UCHAR*>(&value), sizeof(value));
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
	insertBytesNoLengthCheck(tag, bytes, sizeof(bytes));
#else
	insertBytesNoLengthCheck(tag, reinterpret_cast<UCHAR*>(&value), sizeof(value));
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
	if (length > 255) {
		string errStr(str, 100);
		errStr.append("...");

		string msgStr;
		msgStr.printf("string \"%s\" is too long to be inserted into clumplet buffer",
			errStr.c_str());
		usage_mistake(msgStr.c_str());
		return;
	}
	insertBytesNoLengthCheck(tag, reinterpret_cast<const UCHAR*>(str), length);
}

void ClumpletWriter::insertBytes(UCHAR tag, const UCHAR* bytes, size_t length)
{
	if (length > 255) {
		usage_mistake("byte sequence is too long to be inserted into clumplet buffer");
		return;
	}
	insertBytesNoLengthCheck(tag, bytes, length);
}

void ClumpletWriter::insertBytesNoLengthCheck(UCHAR tag, const UCHAR* bytes, UCHAR length)
{
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.getCount()) {
		usage_mistake("write past EOF");
		return;
	}
	
	// Check that resulting data doesn't overflow size limit
	if (dynamic_buffer.getCount() + length + 2 > sizeLimit) {
		size_overflow();
	}

		// Insert the data
	dynamic_buffer.insert(cur_offset, tag);
	dynamic_buffer.insert(cur_offset + 1, length);
	dynamic_buffer.insert(cur_offset + 2, bytes, length);
	cur_offset += length + 2;
}

void ClumpletWriter::insertTag(UCHAR tag)
{
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.getCount()) {
		usage_mistake("write past EOF");
		return;
	}
	
	// Check that resulting data doesn't overflow size limit
	if (dynamic_buffer.getCount() + 2 > sizeLimit) {
		size_overflow();
	}

	// Insert the data
	dynamic_buffer.insert(cur_offset++, tag);
	dynamic_buffer.insert(cur_offset++, 0);
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
	} else {
		size_t length = clumplet[1];
		dynamic_buffer.removeCount(cur_offset, length + 2);
	}
}

} // namespace

