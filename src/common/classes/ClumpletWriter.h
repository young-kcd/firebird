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
 *  $Id: ClumpletWriter.h,v 1.5 2004-12-09 19:18:57 alexpeshkoff Exp $
 *
 */

#ifndef CLUMPLETWRITER_H
#define CLUMPLETWRITER_H

#include "../common/classes/ClumpletReader.h"
#include "../common/classes/array.h"

// This setting of maximum dpb size doesn't mean, that we
// can't process larger DBPs! This is just recommended limit 
// cause it's hard to imagine sensefull DPB of even this size.
const size_t MAX_DPB_SIZE = 1024;

namespace Firebird {

// At the moment you can only declare it on stack, permanent objects are not allowed
class ClumpletWriter : public ClumpletReader {
public:
	// Create empty clumplet writer.
	ClumpletWriter(bool isTagged, size_t limit, UCHAR tag = 0);

	// Create writer from a given buffer
	ClumpletWriter(bool isTagged, size_t limit, const UCHAR* buffer, size_t buffLen, UCHAR tag);

	void reset(UCHAR tag);
	void reset(const UCHAR* buffer, size_t buffLen);

	// Methods to create new clumplet at current position
	void insertInt(UCHAR tag, SLONG value);
	void insertBigInt(UCHAR tag, SINT64 value);
	void insertBytes(UCHAR tag, const UCHAR* bytes, size_t length);
	void insertString(UCHAR tag, const string& str);
	void insertPath(UCHAR tag, const PathName& str);
	void insertString(UCHAR tag, const char* str, size_t length);
	void insertByte(UCHAR tag, const UCHAR byte)
	{
		insertBytesNoLengthCheck(tag, &byte, 1);
	}
	void insertTag(UCHAR tag);
	void insertEndMarker(UCHAR tag);

    // Delete currently selected clumplet from buffer
	void deleteClumplet();

	virtual const UCHAR* getBuffer() const { return dynamic_buffer.begin(); }
protected:
	virtual const UCHAR* getBufferEnd() const { return dynamic_buffer.end(); }
	virtual void size_overflow();
	void insertBytesNoLengthCheck(UCHAR tag, const UCHAR* bytes, UCHAR length);
private:
	size_t sizeLimit;

	// Assignment and copy constructor not implemented.
	ClumpletWriter(const ClumpletWriter& from);
	ClumpletWriter& operator=(const ClumpletWriter& from);

	HalfStaticArray<UCHAR, 128> dynamic_buffer;
};

} // namespace Firebird

#endif
