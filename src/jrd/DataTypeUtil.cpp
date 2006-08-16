/*
 *	PROGRAM:	
 *	MODULE:		DataTypeUtil.cpp
 *	DESCRIPTION:	Data Type Utility functions
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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/dsc.h"
#include "../jrd/ibase.h"
#include "../jrd/intl.h"
#include "../jrd/intl_proto.h"
#include "../jrd/gdsassert.h"


SSHORT DataTypeUtilBase::getResultBlobSubType(const dsc* value1, const dsc* value2)
{	
	SSHORT subType1 = value1->getBlobSubType();
	SSHORT subType2 = value2->getBlobSubType();

	if (subType1 == isc_blob_untyped)
		return subType2;

	return subType1;
}


USHORT DataTypeUtilBase::getResultTextType(const dsc* value1, const dsc* value2)
{
	USHORT cs1 = value1->getCharSet();
	USHORT cs2 = value2->getCharSet();

	USHORT ttype1 = value1->getTextType();
	USHORT ttype2 = value2->getTextType();

	if (cs1 == CS_NONE || cs2 == CS_BINARY)
		return ttype2;

	if (cs1 == CS_ASCII && cs2 != CS_NONE)
		return ttype2;

	return ttype1;
}


void DataTypeUtilBase::clearDsc(dsc* desc)
{
	memset(desc, 0, sizeof(*desc));
}


ULONG DataTypeUtilBase::convertLength(const dsc* src, const dsc* dst)
{
	fb_assert(dst->isText());

	UCHAR dstCharSet = dst->getCharSet();

	if (dstCharSet == CS_NONE || dstCharSet == CS_BINARY)
		return src->getStringLength();

	return (src->getStringLength() / maxBytesPerChar(src->getCharSet())) *
		maxBytesPerChar(dstCharSet);
}


ULONG DataTypeUtilBase::fixLength(const dsc* desc, ULONG length)
{
	UCHAR bpc = maxBytesPerChar(desc->getCharSet());

	return MIN(((MAX_COLUMN_SIZE - sizeof(USHORT)) / bpc) * bpc, length);
}


void DataTypeUtilBase::makeConcatenate(dsc* result, const dsc* value1, const dsc* value2)
{
	clearDsc(result);

	if (value1->isNull() && value2->isNull())
	{
		makeNullString(result);
		return;
	}

	if (value1->isBlob() || value2->isBlob())
	{
		result->dsc_dtype = dtype_blob;
		result->dsc_length = sizeof(ISC_QUAD);
		result->setBlobSubType(getResultBlobSubType(value1, value2));
		result->setTextType(getResultTextType(value1, value2));
	}
	else
	{
		result->dsc_dtype = dtype_varying;
		result->setTextType(getResultTextType(value1, value2));

		ULONG length = fixLength(result,
			convertLength(value1, result) + convertLength(value2, result));
		result->dsc_length = length + sizeof(USHORT);
	}

	result->dsc_flags = (value1->dsc_flags | value2->dsc_flags) & DSC_nullable;
}


void DataTypeUtilBase::makeNullString(dsc* result)
{
	clearDsc(result);

	// VARCHAR(1) CHARACTER SET NONE
	result->dsc_dtype = dtype_varying;
	result->setTextType(CS_NONE);
	result->dsc_length = sizeof(USHORT) + 1;
	result->dsc_flags = DSC_nullable | DSC_null;
}


void DataTypeUtilBase::makeSubstr(dsc* result, const dsc* value, const dsc* offset, const dsc* length)
{
	clearDsc(result);

	if (value->isNull())
	{
		makeNullString(result);
		return;
	}

	if (value->isBlob())
	{
		result->dsc_dtype = dtype_blob;
		result->dsc_length = sizeof(ISC_QUAD);
		result->setBlobSubType(value->getBlobSubType());
	}
	else
	{
		// Beware that JRD treats substring() always as returning CHAR
		// instead of VARCHAR for historical reasons.
		result->dsc_dtype = dtype_varying;
	}

	result->setTextType(value->getTextType());
	result->setNullable(value->isNullable() || offset->isNullable() || length->isNullable());

	if (result->isText())
		result->dsc_length = fixLength(result, convertLength(value, result)) + sizeof(USHORT);
}


namespace Jrd {


UCHAR DataTypeUtil::maxBytesPerChar(UCHAR charSet)
{
	return INTL_charset_lookup(tdbb, charSet)->maxBytesPerChar();
}


}	// namespace Jrd
