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
 */

#ifndef CLUMPLETREADER_H
#define CLUMPLETREADER_H

#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "fb_string.h"

namespace Firebird {

// This class provides read access for clumplet structure
// Note: it doesn't make a copy of buffer it reads
class ClumpletReader : protected AutoStorage {
public:
	ClumpletReader(bool isTagged, const UCHAR* buffer, size_t buffLen);

	// Navigation in clumplet buffer
	bool isEof() const { return cur_offset >= getBufferLength(); }
	void moveNext();
	void rewind();
	bool find(UCHAR tag);

    // Methods which work with currently selected clumplet
	UCHAR getClumpTag() const;
	size_t getClumpLength() const;

	SLONG getInt() const;
	bool getBoolean() const;
	SINT64 getBigInt() const;
	string& getString(string& str) const;
	PathName& getPath(PathName& str) const;
	const UCHAR* getBytes() const { return getBuffer() + cur_offset + 2; }

	// Return the tag for buffer (usually structure version)
	UCHAR getBufferTag() const;
	size_t getBufferLength() const 
	{
		size_t rc = getBufferEnd() - getBuffer();
		if (rc == 1)
		{
			rc = 0;
		}
		return rc;
	}
	size_t getCurOffset() const { return cur_offset; }
	void setCurOffset(size_t newOffset) { cur_offset = newOffset; }
protected:
	size_t cur_offset;
	bool mIsTagged;

	// Methods are virtual so writer can override 'em
	virtual const UCHAR* getBuffer() const { return static_buffer; }
	virtual const UCHAR* getBufferEnd() const { return static_buffer_end; }

	// These functions are called when error condition is detected by this class. 
	// They may throw exceptions. If they don't reader tries to do something
	// sensible, certainly not overwrite memory or read past the end of buffer

	// This appears to be a programming error in buffer access pattern
	virtual void usage_mistake(const char* what) const;

	// This is called when passed buffer appears invalid
	virtual void invalid_structure() const;
private:
	// Assignment and copy constructor not implemented.
	ClumpletReader(const ClumpletReader& from);
	ClumpletReader& operator=(const ClumpletReader& from);

	const UCHAR* static_buffer;
	const UCHAR* static_buffer_end;
};

} // namespace Firebird

#endif // CLUMPLETREADER_H

