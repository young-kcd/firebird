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
 *  $Id: ClumpletWriter.cpp,v 1.1 2004-10-22 06:24:40 skidder Exp $
 *
 */

#include "firebird.h"

#include "../common/classes/ClumpletWriter.h"
#include "fb_exception.h"

namespace Firebird {

ClumpletWriter(UCHAR tag, size_t limit) : 
  ClumpletReader(NULL, 0), sizeLimit(limit), dynamic_buffer(getPool(), 128) 
{
	dynamic_buffer.add(tag);
}
 
ClumpletWriter(const UCHAR* buffer, size_t buffLen, size_t limit) :
  ClumpletReader(NULL, 0), sizeLimit(limit), dynamic_buffer(getPool(), buffer, buffLen) 
{
}

virtual void ClumpletWriter::size_overflow() {
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
	insertBytes(tag, bytes, sizeof(bytes));
#else
	insertBytes(tag, static_cast<UCHAR*>(&value), sizeof(value));
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
	insertBytes(tag, bytes, sizeof(bytes));
#else
	insertBytes(tag, static_cast<UCHAR*>(&value), sizeof(value));
#endif 
}

void ClumpletWriter::insertString(UCHAR tag, const string& str) {
	if (str.length() > 255) {
		size_overflow();
		insertBytes(str.c_str(), 255);
	} else
		insertBytes(str.c_str(), str.length());
}

void ClumpletWriter::insertBytes(UCHAR tag, const UCHAR* bytes, UCHAR len) {
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.length()) {
		read_past_eof();
		return;
	}
	
	// Check that resulting data doesn't overflow size limit
	if (dynamic_buffer.length() + len + 2 > sizeLimit) {
		size_overflow();
	}

	// Insert the data
	dynamic_buffer.insert(cur_offset, static_cast<char>(tag));
	dynamic_buffer.insert(cur_offset + 1, static_cast<char>(len));
	dynamic_buffer.insert(cur_offset + 2, static_cast<char*>(bytes));
	cur_offset += len + 2;
}

void ClumpletWriter::insertEndMarker(UCHAR tag) {
	// Check that we're not beyond the end of buffer.
	// We get there when we set end marker.
	if (cur_offset > dynamic_buffer.length()) {
		read_past_eof();
		return;
	}

	// Check that resulting data doesn't overflow size limit
	if (current_offset + 1 > sizeLimit) {
		size_overflow();
	}

	dynamic_buffer.erase(cur_offset, npos);
	dynamic_buffer += static_cast<char>(tag);

	cur_offset += 2; // Go past EOF to indicate we set the marker
}

void ClumpletWriter::deleteClumplet() {
	UCHAR* clumplet = getBuffer() + cur_offset;
	UCHAR* buffer_end = getBufferEnd();

	// Check for EOF
	if (clumplet >= buffer_end) {
		read_past_eof();
		return;
	}

	if (buffer_end - clumplet < 2) {
		// It appears we're erasing EOF marker
		dynamic_buffer.erase(cur_offset, npos);
	} else {
		size_t length = clumplet[1];
		dynamic_buffer.erase(cur_offset, length + 2);
	}
}


}
