/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intl_classes.h
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
 */

#ifndef JRD_INTL_CLASSES_H
#define JRD_INTL_CLASSES_H

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/intlobj.h"

namespace Jrd {

class LikeObject {
public:
	virtual void reset() = 0;
	virtual bool process(thread_db*, TextType, const UCHAR*, SSHORT) = 0;
	virtual bool result() = 0;
	virtual ~LikeObject() {}
};

class ContainsObject {
public:
	virtual void reset() = 0;
	virtual bool process(Jrd::thread_db*, Jrd::TextType, const UCHAR*, SSHORT) = 0;
	virtual bool result() = 0;
	virtual ~ContainsObject() {}
};

class TextType
{
public:
	TextType(texttype *_tt) : tt(_tt) {}

	// copy constructor
	TextType(const TextType& obj) : tt(obj.tt) {}

	USHORT key_length(USHORT a) {
		fb_assert(tt);
		fb_assert(tt->texttype_fn_key_length);
		return (*tt->texttype_fn_key_length)(tt, a);
	}

	USHORT string_to_key(USHORT a,
						 const UCHAR* b,
						 USHORT c,
						 UCHAR* d,
						 USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_string_to_key);
		return (*tt->texttype_fn_string_to_key)(tt, a, b, c, d, e);
	}
	
	SSHORT compare(USHORT a,
				   UCHAR* b,
				   USHORT c,
				   UCHAR* d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_compare);
		return (*tt->texttype_fn_compare)(tt, a, b, c, d);
	}
	
	USHORT to_upper(USHORT a)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_upper);
		return (*tt->texttype_fn_to_upper)(tt, a);
	}
	
	USHORT to_lower(USHORT a)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_lower);
		return (*tt->texttype_fn_to_lower)(tt, a);
	}
	
	SSHORT str_to_upper(USHORT a,
						const UCHAR* b,
						USHORT c,
						UCHAR* d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_str_to_upper);
		return (*tt->texttype_fn_str_to_upper)(tt, a, b, c, d);
	}
	
	USHORT to_wc(UCS2_CHAR* a,
				 USHORT b,
				 const UCHAR* c,
				 USHORT d,
				 SSHORT* e,
				 USHORT* f)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_wc);
		return (*tt->texttype_fn_to_wc)(tt, a, b, c, d, e, f);
	}
									
	USHORT mbtowc(UCS2_CHAR* a, const UCHAR* b, USHORT c)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_mbtowc);
		return (*tt->texttype_fn_mbtowc)(tt, a, b, c);
	}

	bool like(thread_db* tdbb, const UCHAR* s, SSHORT sl, const UCHAR* p, SSHORT pl, SSHORT escape)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like);
		return (tt->texttype_fn_like)(tdbb, tt, s, sl, p, pl, escape);
	}
	
	LikeObject *like_create(thread_db* tdbb, const UCHAR* p, SSHORT pl, SSHORT escape)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_create);
		return (tt->texttype_fn_like_create)(tdbb, *this, p, pl, escape);
	}
	
	bool contains(thread_db* tdbb, const UCHAR* s, SSHORT sl, const UCHAR* p, SSHORT pl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains);
		return (tt->texttype_fn_contains)(tdbb, tt, s, sl, p, pl);
	}
	
	ContainsObject *contains_create(thread_db* tdbb, const UCHAR* p, SSHORT pl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_create);
		return (tt->texttype_fn_contains_create)(tdbb, tt, p, pl);
	}
	
	USHORT matches(thread_db* tdbb, const UCHAR* a, SSHORT b, const UCHAR* c, SSHORT d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_matches);
		return (*(tt->texttype_fn_matches))(tdbb, tt, a, b, c, d);
	}

	USHORT sleuth_check(thread_db* tdbb, USHORT a,
								const UCHAR* b,
								USHORT c,
								const UCHAR* d,
								USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_sleuth_check);
		return (*(tt->texttype_fn_sleuth_check))(tdbb, tt, a, b, c, d, e);
	}
	
	USHORT sleuth_merge(thread_db* tdbb, const UCHAR* a,
								USHORT b,
								const UCHAR* c,
								USHORT d,
								UCHAR* e,
								USHORT f)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_sleuth_merge);
		return (*(tt->texttype_fn_sleuth_merge))(tdbb, tt, a, b, c, d, e, f);
	}


	USHORT getType() const
	{
		fb_assert(tt);
		return tt->texttype_type; 
	}

	const char *getName() const
	{ 
		fb_assert(tt);
		return tt->texttype_name; 
	}

	CHARSET_ID getCharSet() const
	{ 
		fb_assert(tt);
		return tt->texttype_character_set; 
	}

	SSHORT getCountry() const
	{ 
		fb_assert(tt);
		return tt->texttype_country; 
	}

	UCHAR getBytesPerChar() const
	{ 
		fb_assert(tt);
		return tt->texttype_bytes_per_char; 
	}

	texttype* getStruct() const { return tt; }

private:
	texttype* tt;
};

static inline bool operator ==(const TextType& tt1, const TextType& tt2) {
	return tt1.getStruct() == tt2.getStruct();
}

static inline bool operator !=(const TextType& tt1, const TextType& tt2)
{
	return tt1.getStruct() != tt2.getStruct();
}

class CsConvert
{ 
public:
	CsConvert(csconvert* _cnvt) : cnvt(_cnvt) {}
	CsConvert(const CsConvert& obj) : cnvt(obj.cnvt) {}

	// CVC: Beware of this can of worms: csconvert_convert gets assigned
	// different functions that not necessarily take the same argument. Typically,
	// the src pointer and the dest pointer use different types.
	// How does this work without crashing is a miracle of IT.
	USHORT convert(UCHAR* a,
				   USHORT b,
				   const UCHAR* c,
				   USHORT d,
				   SSHORT* e,
				   USHORT* f)
	{
		fb_assert(cnvt != NULL);
		return (*cnvt->csconvert_convert)(cnvt, a, b, c, d, e, f);
	}

	SSHORT getId() const { fb_assert(cnvt); return cnvt->csconvert_id; }
	const char* getName() const { fb_assert(cnvt); return cnvt->csconvert_name; }
	CHARSET_ID getFromCS() const { fb_assert(cnvt); return cnvt->csconvert_from; }
	CHARSET_ID getToCS() const { fb_assert(cnvt); return cnvt->csconvert_to; }

	csconvert* getStruct() const { return cnvt; }

private:
	csconvert* cnvt;
};

static inline bool operator ==(const CsConvert& cv1, const CsConvert& cv2)
{
	return cv1.getStruct() == cv2.getStruct();
}

static inline bool operator !=(const CsConvert& cv1, const CsConvert& cv2)
{
	return cv1.getStruct() != cv2.getStruct();
}

class CharSet
{
public:
	CharSet(charset* _cs) : cs(_cs) {}
	CharSet(const CharSet &obj) : cs(obj.cs) {}

	CHARSET_ID getId() const { fb_assert(cs); return cs->charset_id; }
	const char* getName() const { fb_assert(cs); return cs->charset_name; }
	UCHAR minBytesPerChar() const { fb_assert(cs); return cs->charset_min_bytes_per_char; }
	UCHAR maxBytesPerChar() const { fb_assert(cs); return cs->charset_max_bytes_per_char; }
	UCHAR getSpaceLength() const { fb_assert(cs); return cs->charset_space_length; }
	const UCHAR* getSpace() const { fb_assert(cs); return cs->charset_space_character; }

	CsConvert getConvToUnicode() { fb_assert(cs); return &cs->charset_to_unicode; }

	CsConvert getConvFromUnicode() { fb_assert(cs); return &cs->charset_from_unicode; }

	charset* getStruct() const { return cs; }

private:
	charset* cs;
};

static inline bool operator ==(const CharSet& cs1, const CharSet& cs2)
{
	return cs1.getStruct() == cs2.getStruct();
}

static inline bool operator !=(const CharSet& cs1, const CharSet& cs2)
{
	return cs1.getStruct() != cs2.getStruct();
}

} //namespace Jrd

#endif /* JRD_INTL_CLASSES_H */

