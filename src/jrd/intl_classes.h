/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intl_classes.h
 *	DESCRIPTION:	International text handling definitions
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 *
 */

#ifndef JRD_INTL_CLASSES_H
#define JRD_INTL_CLASSES_H

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/constants.h"
#include "../jrd/intlobj.h"

namespace Jrd {

class TextType
{
public:
	//typedef bool   (*FPTR_CONTAINS)(thread_db* tdbb, TextType ttype, const UCHAR* s, SSHORT ls, const UCHAR* p, SSHORT lp);
	//typedef void*  (*FPTR_CONTAINS_CREATE)(thread_db* tdbb, TextType ttype, const UCHAR* p, SSHORT lp);
	//typedef void   (*FPTR_CONTAINS_DESTROY)(void* object);
	//typedef void   (*FPTR_CONTAINS_RESET)(void* object);
	//typedef bool   (*FPTR_CONTAINS_PROCESS)(thread_db* tdbb, TextType ttype, void* object, const UCHAR* s, SSHORT ls);
	//typedef bool   (*FPTR_CONTAINS_RESULT)(void* object);

	//typedef bool   (*FPTR_LIKE)(thread_db* tdbb, TextType ttype, const UCHAR* s, SSHORT ls, const UCHAR* p, SSHORT lp, UCS2_CHAR escape);
	//typedef void*  (*FPTR_LIKE_CREATE)(thread_db* tdbb, TextType ttype, const UCHAR* p, SSHORT lp, UCS2_CHAR escape);
	//typedef void   (*FPTR_LIKE_DESTROY)(void* object);
	//typedef void   (*FPTR_LIKE_RESET)(void* object);
	//typedef bool   (*FPTR_LIKE_PROCESS)(thread_db* tdbb, TextType ttype, void* object, const UCHAR* s, SSHORT ls);
	//typedef bool   (*FPTR_LIKE_RESULT)(void* object);

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
		return //reinterpret_cast<FPTR_LIKE>
			(tt->texttype_fn_like)(tdbb, tt, s, sl, p, pl, escape);
	}
	
	void *like_create(thread_db* tdbb, const UCHAR* p, SSHORT pl, SSHORT escape)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_create);
		return //reinterpret_cast<FPTR_LIKE_CREATE>
			(tt->texttype_fn_like_create)(tdbb, *this, p, pl, escape);
	}
	
	void like_reset(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_reset);
		//reinterpret_cast<FPTR_LIKE_RESET>
			(tt->texttype_fn_like_reset)(object);
	}
	
	bool like_result(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_result);
		return //reinterpret_cast<FPTR_LIKE_RESULT>
			(tt->texttype_fn_like_result)(object);
	}
	
	void like_destroy(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_destroy);
		//reinterpret_cast<FPTR_LIKE_DESTROY>
			(tt->texttype_fn_like_destroy)(object);
	}
	
	bool like_process(thread_db* tdbb, void* object, const UCHAR* s, SSHORT sl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like_process);
		return //reinterpret_cast<FPTR_LIKE_PROCESS>
			(tt->texttype_fn_like_process)(tdbb, tt, object, s, sl);
	}
	
	bool contains(thread_db* tdbb, const UCHAR* s, SSHORT sl, const UCHAR* p, SSHORT pl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains);
		return //reinterpret_cast<FPTR_CONTAINS>
			(tt->texttype_fn_contains)(tdbb, tt, s, sl, p, pl);
	}
	
	void *contains_create(thread_db* tdbb, const UCHAR* p, SSHORT pl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_create);
		return //reinterpret_cast<FPTR_CONTAINS_CREATE>
			(tt->texttype_fn_contains_create)(tdbb, tt, p, pl);
	}
	
	void contains_reset(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_reset);
		//reinterpret_cast<FPTR_CONTAINS_RESET>
			(tt->texttype_fn_contains_reset)(object);
	}
	
	bool contains_result(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_result);
		return //reinterpret_cast<FPTR_CONTAINS_RESULT>
			(tt->texttype_fn_contains_result)(object);
	}
	
	void contains_destroy(void* object)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_destroy);
		//reinterpret_cast<FPTR_CONTAINS_DESTROY>
			(tt->texttype_fn_contains_destroy)(object);
	}
	
	bool contains_process(thread_db* tdbb, void* object, const UCHAR* s, SSHORT sl)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains_process);
		return //reinterpret_cast<FPTR_CONTAINS_PROCESS>
			(tt->texttype_fn_contains_process)(tdbb, tt, object, s, sl);
	}
	
	USHORT matches(thread_db* tdbb, const UCHAR* a, SSHORT b, const UCHAR* c, SSHORT d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_matches);
		return (*(//reinterpret_cast<
				//	USHORT (*)(thread_db*, TextType, const UCHAR*, short, const UCHAR*, short)>
						(tt->texttype_fn_matches)))
							(tdbb, tt, a, b, c, d);
	}

	USHORT sleuth_check(thread_db* tdbb, USHORT a,
								const UCHAR* b,
								USHORT c,
								const UCHAR* d,
								USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_sleuth_check);
		return (*(//reinterpret_cast<
				//	USHORT(*)(thread_db*, TextType, USHORT, const UCHAR*, USHORT, const UCHAR*, USHORT)>
						(tt->texttype_fn_sleuth_check)))
							(tdbb, tt, a, b, c, d, e);
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
		return (*(//reinterpret_cast<
				//	USHORT(*)(thread_db*, TextType, const UCHAR*, USHORT, const UCHAR*, USHORT, UCHAR*, USHORT)>
						(tt->texttype_fn_sleuth_merge)))
							(tdbb, tt, a, b, c, d, e, f);
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
	CharSet(const CharSet &obj) : cs(obj.cs) {};

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

