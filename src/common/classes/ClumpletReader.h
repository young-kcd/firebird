/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		ClumpletReader.h
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
 *  $Id: ClumpletReader.h,v 1.1 2004-10-22 06:24:40 skidder Exp $
 *
 */

#ifndef CLUMPLET_BUFFER_H
#define CLUMPLET_BUFFER_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "fb_string.h"

namespace Firebird {

// This class provides read access to dynamic clumplet structures
// Note: it doesn't make a copy of buffer it reads
class ClumpletReader : protected AutoStorage {
public:
	ClumpletReader(const UCHAR* buffer, size_t buffLen) :
	  cur_offset(1), static_buffer(buffer), 
		  static_buffer_end(buffer + buffLen) { };

	// Navigation in clumplet buffer
	bool isEof() { return current_pos >= getBufferLength(); }
	void moveNext();

    // Methods which work with currently selected clumplet
	UCHAR getClumpTag();
	size_t getClumpLength();

	SLONG getInt();
	SINT64 getBigInt();
	string& getString(string& str);
	const char* getBytes() { return clumplet + 2; }

	// Return the tag for buffer (usually structure version)
	UCHAR getBufferTag();
	size_t getBufferLength() { return getBufferEnd() - getBuffer(); }
protected:
	size_t cur_offset;

	// Methods are virtual so writer can override 'em
	virtual UCHAR* getBuffer() { return const_cast<UCHAR*>(static_buffer); }
	virtual UCHAR* getBufferEnd() { return const_cast<UCHAR*>(static_buffer_end); }

	// These functions are called when error condition is detected by this class. 
	// They may throw exceptions. If they don't reader tries to do something
	// sensible, certainly not overwrite memory or read past the end of buffer
	virtual void read_past_eof();
	virtual void invalid_structure();
private:
	// Assignment and copy constructor not implemented.
	ClumpletReader(const ClumpletReader& from);
	ClumpletReader& operator=(const ClumpletReader& from);

	const UCHAR* static_buffer;
	const UCHAR* static_buffer_end;
}

} // namespace Firebird

#endif
