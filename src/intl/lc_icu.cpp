/*
 *	PROGRAM:	Firebird International support
 *	MODULE:		lc_icu.cpp
 *	DESCRIPTION:	Collations for ICU character sets
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
 *  Copyright (c) 2004 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "ldcommon.h"
#include "ld_proto.h"
#include "lc_icu.h"
#include "cs_icu.h"
#include "../jrd/IntlUtil.h"


static void texttype_destroy(texttype* tt)
{
	delete [] const_cast<ASCII*>(tt->texttype_name);
}


static bool texttype_default_init(texttype* tt,
								  const ASCII* name,
								  const ASCII* charSetName,
								  USHORT attributes,
								  const UCHAR* specificAttributes,
								  ULONG specificAttributesLength)
{
	charset cs;
	memset(&cs, 0, sizeof(cs));

	// test if that ICU charset exist
	if (CSICU_charset_init(&cs, charSetName))
	{
		if (cs.charset_fn_destroy)
			cs.charset_fn_destroy(&cs);
	}
	else
		return false;

	if ((attributes & ~TEXTTYPE_ATTR_PAD_SPACE) ||
		// disabled TEXTTYPE_ATTR_PAD_SPACE isn't allowed
		// for our ICU collations yet
		!(attributes & TEXTTYPE_ATTR_PAD_SPACE) ||
		specificAttributesLength)
	{
		return false;
	}

	// name comes from stack. Copy it.
	tt->texttype_name = new ASCII[strlen(name) + 1];
	strcpy(const_cast<ASCII*>(tt->texttype_name), name);

	tt->texttype_version = TEXTTYPE_VERSION_1;
	tt->texttype_country = CC_INTL;
	tt->texttype_pad_option = (attributes & TEXTTYPE_ATTR_PAD_SPACE) ? true : false;
	tt->texttype_fn_destroy = texttype_destroy;

	return true;
}


static bool texttype_unicode_init(texttype* tt,
								  const ASCII* name,
								  const ASCII* charSetName,
								  USHORT attributes,
								  const UCHAR* specificAttributes,
								  ULONG specificAttributesLength)
{
	charset* cs = new charset;
	memset(cs, 0, sizeof(*cs));

	// test if that charset exist
	if (!LD_lookup_charset(cs, charSetName))
	{
		delete cs;
		return false;
	}

	Firebird::UCharBuffer specificAttributesBuffer;
	memcpy(specificAttributesBuffer.getBuffer(specificAttributesLength),
		specificAttributes, specificAttributesLength);

	return Firebird::IntlUtil::initUnicodeCollation(tt, cs, name, attributes, specificAttributesBuffer);
}


bool LCICU_texttype_init(texttype* tt,
						 const ASCII* name,
						 const ASCII* charSetName,
						 USHORT attributes,
						 const UCHAR* specificAttributes,
						 ULONG specificAttributesLength)
{
	int len = strlen(name);

	if (strcmp(name, charSetName) == 0)
	{
		return texttype_default_init(
			tt, name, charSetName, attributes,
			specificAttributes, specificAttributesLength);
	}
	else if (len > 8 && strcmp(name + len - 8, "_UNICODE") == 0)
	{
		return texttype_unicode_init(
			tt, name, charSetName, attributes,
			specificAttributes, specificAttributesLength);
	}
	else
		return false;
}
