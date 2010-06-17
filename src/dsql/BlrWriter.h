/*
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 * Adriano dos Santos Fernandes - refactored from others modules.
 */

#ifndef DSQL_BLR_WRITER_H
#define DSQL_BLR_WRITER_H

#include "../jrd/common.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"

namespace Jrd {

class dsql_nod;


// BLR/DYN writer.
class BlrWriter : public Firebird::PermanentStorage
{
public:
	explicit BlrWriter(MemoryPool& p)
		: PermanentStorage(p),
		  blrData(p),
		  debugData(p),
		  baseOffset(0)
	{
	}

	void appendUChar(const UCHAR byte)
	{
		blrData.add(byte);
	}

	// Cram a word into the blr buffer.
	void appendUShort(USHORT word)
	{
		appendUChar(word);
		appendUChar(word >> 8);
	}

	void appendULong(ULONG val)
	{
		appendUShort(val);
		appendUShort(val >> 16);
	}

	void appendUCharRepeated(UCHAR byte, int count)
	{
		for (int i = 0; i < count; ++i)
			appendUChar(byte);
	}

	void appendBytes(const UCHAR* string, USHORT len)
	{
		blrData.add(string, len);
	}

	// Write out a string with one byte of length.
	void appendNullString(const char* string)
	{
		size_t len = strlen(string);

		// CVC: Maybe the Release version should truncate "len" to 255?
		fb_assert(len >= 0 && len <= 255);

		appendUChar(len);
		appendBytes(reinterpret_cast<const UCHAR*>(string), len);
	}

	// Write out a string valued attribute.
	void appendNullString(UCHAR verb, const char* string)
	{
		const USHORT length = string ? strlen(string) : 0;
		appendString(verb, string, length);
	}

	// Write out a string in metadata charset with one byte of length.
	void appendMetaString(const char* string)
	{
		appendString(0, string, strlen(string));
	}

	void appendString(UCHAR verb, const char* string, USHORT len);

	void appendString(UCHAR verb, const Firebird::MetaName& name)
	{
		appendString(verb, name.c_str(), name.length());
	}

	void appendString(UCHAR verb, const Firebird::string& name)
	{
		appendString(verb, name.c_str(), name.length());
	}

	void appendNumber(UCHAR verb, SSHORT number);
	void appendUShortWithLength(USHORT val);
	void appendULongWithLength(ULONG val);
	void appendFileLength(ULONG length);
	void appendFileStart(ULONG start);

	void beginBlr(UCHAR verb);
	void endBlr();

	void beginDebug();
	void endDebug();
	void putDebugSrcInfo(USHORT, USHORT);
	void putDebugVariable(USHORT, const TEXT*);
	void putDebugArgument(UCHAR, USHORT, const TEXT*);
	void appendDebugInfo();

	Firebird::HalfStaticArray<UCHAR, 1024>& getBlrData() { return blrData; }
	Firebird::HalfStaticArray<UCHAR, 128>& getDebugData() { return debugData; }

	ULONG getBaseOffset() const { return baseOffset; }
	void setBaseOffset(ULONG value) { baseOffset = value; }

protected:
	virtual bool isVersion4() = 0;
	virtual bool isDdlDyn() = 0;

private:
	Firebird::HalfStaticArray<UCHAR, 1024> blrData;
	Firebird::HalfStaticArray<UCHAR, 128> debugData;
	ULONG baseOffset;					// place to go back and stuff in blr length
};


} // namespace

#endif // DSQL_BLR_WRITER_H
