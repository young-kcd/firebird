/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		ClumpletWriter.h
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
 *  $Id: ClumpletWriter.cpp,v 1.3 2004-11-03 08:38:09 skidder Exp $
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
		dynamic_buffer += static_cast<char>(tag);
	}
}
 
ClumpletWriter::ClumpletWriter(bool isTagged, size_t limit, const UCHAR* buffer, size_t buffLen) :
  ClumpletReader(isTagged, NULL, 0), sizeLimit(limit), 
	  dynamic_buffer(getPool(), reinterpret_cast<const char*>(buffer), buffLen) 
{
}

void ClumpletWriter::reset(UCHAR tag) {
	if (!mIsTagged) {
		usage_mistake("buffer is not tagged");

		dynamic_buffer.resize(0);
		cur_offset = 0;
		return;
	}

	cur_offset = 1;
	dynamic_buffer.resize(1);
	dynamic_buffer[0] = static_cast<char>(tag);
}

void ClumpletWriter::reset(const UCHAR* buffer, size_t buffLen) {
	cur_offset = mIsTagged ? 1 : 0;
	dynamic_buffer.assign(reinterpret_cast<const char*>(buffer), buffLen);
}


void ClumpletWriter::size_overflow() {
	fatal_exception::raise("Clumplet buffer size limit reached");
}

void ClumpletWriter::insertInt(UCHAR tag, SLONG value) {
#if defined(WORDS_BIGENDIAN)
	char bytes[4];
	UCHAR* ptr = &value;
	bytes[0] = ptr[3];
	bytes[1] = ptr[2];
	bytes[2] = ptr[1];
	bytes[3] = ptr[0];
	insertBytesNoLengthCheck(tag, bytes, sizeof(bytes));
#else
	insertBytesNoLengthCheck(tag, reinterpret_cast<UCHAR*>(&value), sizeof(value));
#endif 
}

void ClumpletWriter::insertBigInt(UCHAR tag, SINT64 value) {
#if defined(WORDS_BIGENDIAN)
	char bytes[8];
	UCHAR* ptr = static_cast<UCHAR*>(&value);
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

void ClumpletWriter::insertString(UCHAR tag, const string& str) {
	insertString(tag, str.c_str(), str.length());
}

void ClumpletWriter::insertString(UCHAR tag, const char* str, size_t length) {
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

void ClumpletWriter::insertBytes(UCHAR tag, const UCHAR* bytes, size_t length) {
	if (length > 255) {
		usage_mistake("byte sequence is too long to be inserted into clumplet buffer");
		return;
	}
	insertBytesNoLengthCheck(tag, bytes, length);
}

void ClumpletWriter::insertBytesNoLengthCheck(UCHAR tag, const UCHAR* bytes, UCHAR length) {
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.length()) {
		usage_mistake("write past EOF");
		return;
	}
	
	// Check that resulting data doesn't overflow size limit
	if (dynamic_buffer.length() + length + 2 > sizeLimit) {
		size_overflow();
	}

	// Insert the data
	dynamic_buffer.insert(cur_offset, static_cast<char>(tag));
	dynamic_buffer.insert(cur_offset + 1, static_cast<char>(length));
	dynamic_buffer.insert(cur_offset + 2, reinterpret_cast<const char*>(bytes));
	cur_offset += length + 2;
}

void ClumpletWriter::insertEndMarker(UCHAR tag) {
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.length()) {
		usage_mistake("write past EOF");
		return;
	}

	// Check that resulting data doesn't overflow size limit
	if (cur_offset + 1 > sizeLimit) {
		size_overflow();
	}

	dynamic_buffer.erase(cur_offset, string::npos);
	dynamic_buffer += static_cast<char>(tag);

	cur_offset += 2; // Go past EOF to indicate we set the marker
}

void ClumpletWriter::deleteClumplet() {
	UCHAR* clumplet = getBuffer() + cur_offset;
	UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		usage_mistake("write past EOF");
		return;
	}

	if (buffer_end - clumplet < 2) {
		// It appears we're erasing EOF marker
		dynamic_buffer.erase(cur_offset, string::npos);
	} else {
		size_t length = clumplet[1];
		dynamic_buffer.erase(cur_offset, length + 2);
	}
}


}
