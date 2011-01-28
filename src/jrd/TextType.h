/*
 *	PROGRAM:	JRD International support
 *	MODULE:		TextType.h
 *	DESCRIPTION:	International text handling definitions
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
 *  2006.10.10 Adriano dos Santos Fernandes - refactored from intl_classes.h
 *
 */

#ifndef JRD_TEXTTYPE_H
#define JRD_TEXTTYPE_H

struct texttype;

namespace Jrd {

class CharSet;

class TextType
{
public:
	TextType(TTYPE_ID _type, texttype *_tt, CharSet* _cs);

private:
	TextType(const TextType&);	// Not implemented

public:
	virtual ~TextType() {}

	USHORT key_length(USHORT len);

	USHORT string_to_key(USHORT srcLen,
						 const UCHAR* src,
						 USHORT dstLen,
						 UCHAR* dst,
						 USHORT key_type);

	SSHORT compare(ULONG len1,
				   const UCHAR* str1,
				   ULONG len2,
				   const UCHAR* str2);

	ULONG str_to_upper(ULONG srcLen,
					   const UCHAR* src,
					   ULONG dstLen,
					   UCHAR* dst);

	ULONG str_to_lower(ULONG srcLen,
					   const UCHAR* src,
					   ULONG dstLen,
					   UCHAR* dst);

	ULONG canonical(ULONG srcLen,
					const UCHAR* src,
					ULONG dstLen,
					UCHAR* dst);

	const UCHAR* getSqlMatchAnyCanonic() const { return sqlMatchAnyCanonic; }
	const UCHAR* getSqlMatchOneCanonic() const { return sqlMatchOneCanonic; }
	const UCHAR* getGdmlMatchOneCanonic() const { return gdmlMatchOneCanonic; }
	const UCHAR* getGdmlMatchAnyCanonic() const { return gdmlMatchAnyCanonic; }
	const UCHAR* getGdmlQuoteCanonic() const { return gdmlQuoteCanonic; }
	const UCHAR* getGdmlNotCanonic() const { return gdmlNotCanonic; }
	const UCHAR* getGdmlRangeCanonic() const { return gdmlRangeCanonic; }
	const UCHAR* getGdmlClassStartCanonic() const { return gdmlClassStartCanonic; }
	const UCHAR* getGdmlClassEndCanonic() const { return gdmlClassEndCanonic; }
	const UCHAR* getGdmlSubstituteCanonic() const { return gdmlSubstituteCanonic; }
	const UCHAR* getGdmlFlagSetCanonic() const { return gdmlFlagSetCanonic; }
	const UCHAR* getGdmlFlagClearCanonic() const { return gdmlFlagClearCanonic; }
	const UCHAR* getGdmlCommaCanonic() const { return gdmlCommaCanonic; }
	const UCHAR* getGdmlLParenCanonic() const { return gdmlLParenCanonic; }
	const UCHAR* getGdmlRParenCanonic() const { return gdmlRParenCanonic; }
	const UCHAR* getGdmlUpperSCanonic() const { return gdmlUpperSCanonic; }
	const UCHAR* getGdmlLowerSCanonic() const { return gdmlLowerSCanonic; }

	USHORT getType() const
	{
		return type;
	}

	CharSet* getCharSet() const
	{
		return cs;
	}

	BYTE getCanonicalWidth() const
	{ 
		return tt->texttype_canonical_width;
	}

	USHORT getFlags() const
	{ 
		return tt->texttype_flags;
	}

public:
	Firebird::MetaName name;

protected:
	texttype* tt;
	CharSet* cs;

private:
	TTYPE_ID type;
#ifdef RISC_ALIGNMENT
	ULONG dummyAlign;
#endif
	UCHAR sqlMatchAnyCanonic[sizeof(ULONG)];
	UCHAR sqlMatchOneCanonic[sizeof(ULONG)];
	UCHAR gdmlMatchOneCanonic[sizeof(ULONG)];
	UCHAR gdmlMatchAnyCanonic[sizeof(ULONG)];
	UCHAR gdmlQuoteCanonic[sizeof(ULONG)];
	UCHAR gdmlNotCanonic[sizeof(ULONG)];
	UCHAR gdmlRangeCanonic[sizeof(ULONG)];
	UCHAR gdmlClassStartCanonic[sizeof(ULONG)];
	UCHAR gdmlClassEndCanonic[sizeof(ULONG)];
	UCHAR gdmlSubstituteCanonic[sizeof(ULONG)];
	UCHAR gdmlFlagSetCanonic[sizeof(ULONG)];
	UCHAR gdmlFlagClearCanonic[sizeof(ULONG)];
	UCHAR gdmlCommaCanonic[sizeof(ULONG)];
	UCHAR gdmlLParenCanonic[sizeof(ULONG)];
	UCHAR gdmlRParenCanonic[sizeof(ULONG)];
	UCHAR gdmlUpperSCanonic[sizeof(ULONG)];
	UCHAR gdmlLowerSCanonic[sizeof(ULONG)];
};

}	// namespace Jrd


#endif	// JRD_TEXTTYPE_H
