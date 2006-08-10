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

namespace Jrd {


SSHORT DataTypeUtil::getResultBlobSubType(const dsc* value1, const dsc* value2)
{	
	SSHORT subType1 = value1->getBlobSubType();
	SSHORT subType2 = value2->getBlobSubType();

	if (subType1 == isc_blob_untyped)
		return subType2;
	else
		return subType1;
}


USHORT DataTypeUtil::getResultTextType(const dsc* value1, const dsc* value2)
{
	USHORT cs1 = value1->getCharSet();
	USHORT cs2 = value2->getCharSet();

	USHORT ttype1 = value1->getTextType();
	USHORT ttype2 = value2->getTextType();

	if (cs1 == CS_NONE || cs2 == CS_BINARY)
		return ttype2;
	else if (cs1 == CS_ASCII && cs2 != CS_NONE)
		return ttype2;
	else
		return ttype1;
}


// ASF: We don't calculate string lengths because we may be called
// from the engine or DSQL, and charset max-bytes-per-char handling
// is different (TextType->maxBytesPerChar() / METD_get_charset_bpc).
// When DSQL is in the engine, we could calculate lengths directly
// here instead of in jrd/cmp.cpp and dsql/make.cpp.


void DataTypeUtil::makeConcatenate(dsc* result, const dsc* value1, const dsc* value2)
{
	memset(result, 0, sizeof(*result));

	if (value1->isBlob() || value2->isBlob())
	{
		result->dsc_dtype = dtype_blob;
		result->dsc_length = sizeof(ISC_QUAD);
		result->setBlobSubType(getResultBlobSubType(value1, value2));
	}
	else
		result->dsc_dtype = dtype_varying;

	result->setTextType(getResultTextType(value1, value2));
}


}	// namespace Jrd
