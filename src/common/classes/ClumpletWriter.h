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
 *  $Id: ClumpletWriter.h,v 1.2 2004-10-23 01:21:11 skidder Exp $
 *
 */

#ifndef CLUMPLETWRITER_H
#define CLUMPLETWRITER_H

#include "../common/classes/ClumpletReader.h"

namespace Firebird {

// At the moment you can only declare it on stack, permanent objects are not allowed
class ClumpletWriter : public ClumpletReader {
public:
	// Create empty clumplet writer (unusable before you call one of reset methods)
	ClumpletWriter(size_t limit);

	// Create empty, initialized writer
	ClumpletWriter(UCHAR tag, size_t limit);
 
	// Create writer from a given buffer
	ClumpletWriter(const UCHAR* buffer, size_t buffLen, size_t limit);

	void reset(UCHAR tag);
	void reset(const UCHAR* buffer, size_t buffLen);

	// Methods to create new clumplet at current position
	void insertInt(UCHAR tag, SLONG value);
	void insertBigInt(UCHAR tag, SINT64 value);
	void insertBytes(UCHAR tag, const UCHAR* bytes, UCHAR len);
	void insertString(UCHAR tag, const string& str);
	void insertEndMarker(UCHAR tag);

    // Delete currently selected clumplet from buffer
	void deleteClumplet();

	virtual UCHAR* getBuffer() { 
		return reinterpret_cast<UCHAR*>(dynamic_buffer.begin()); 
	}
protected:
	virtual UCHAR* getBufferEnd() { 
		return reinterpret_cast<UCHAR*>(dynamic_buffer.end()); 
	}
	virtual void size_overflow();
private:
	size_t sizeLimit;

	// Assignment and copy constructor not implemented.
	ClumpletWriter(const ClumpletWriter& from);
	ClumpletWriter& operator=(const ClumpletWriter& from);

	string dynamic_buffer;
};

} // namespace Firebird

#endif
