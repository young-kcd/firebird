/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intl_classes.h
 *	DESCRIPTION:	International text handling definitions
 *
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
 */

#ifndef JRD_INTL_CLASSES_H
#define JRD_INTL_CLASSES_H

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/constants.h"
#include "../jrd/intlobj.h"

class TextType
{
public:
	TextType(struct texttype *_tt) : tt(_tt) {}

	// copy constructor
	TextType(const TextType& obj) : tt(obj.tt) {}

	USHORT key_length(USHORT a) {
		fb_assert(tt);
		fb_assert(tt->texttype_fn_key_length);
		return (*(reinterpret_cast<USHORT (*)(TEXTTYPE,USHORT)>
					(tt->texttype_fn_key_length)))(tt,a);
	}

	USHORT string_to_key(USHORT a,
						 const UCHAR *b,
						 USHORT c,
						 UCHAR *d,
						 USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_string_to_key);
		return (*(reinterpret_cast
			<USHORT(*)(TEXTTYPE,USHORT,const UCHAR*,USHORT,UCHAR*,USHORT)>
				(tt->texttype_fn_string_to_key)))
					(tt,a,b,c,d,e);
	}
	
	SSHORT compare(USHORT a,
				   UCHAR *b,
				   USHORT c,
				   UCHAR *d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_compare);
		return (*(reinterpret_cast
			<short (*)(TEXTTYPE,USHORT,UCHAR*,USHORT,UCHAR*)>
				(tt->texttype_fn_compare)))(tt,a,b,c,d);
	}
	
	USHORT to_upper(USHORT a)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_upper);
		return (*(reinterpret_cast
			<short (*)(TEXTTYPE,USHORT)>
				(tt->texttype_fn_to_upper)))(tt,a);
	}
	
	USHORT to_lower(USHORT a)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_lower);
		return (*(reinterpret_cast
			<USHORT (*)(TEXTTYPE,USHORT)>
				(tt->texttype_fn_to_lower)))(tt,a);
	}
	
	SSHORT str_to_upper(USHORT a,
						UCHAR *b,
						USHORT c,
						UCHAR *d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_str_to_upper);
		return (*(reinterpret_cast
					<short (*)(TEXTTYPE,USHORT,UCHAR*,USHORT,UCHAR*)>
						(tt->texttype_fn_str_to_upper)))
							(tt,a,b,c,d);
	}
	
	USHORT to_wc(UCS2_CHAR *a,
				 USHORT b,
				 UCHAR *c,
				 USHORT d,
				 SSHORT *e,
				 USHORT *f)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_to_wc);
		return (*(reinterpret_cast
					<USHORT (*)(TEXTTYPE,UCS2_CHAR*,USHORT,UCHAR*,USHORT,short*,USHORT*)>
						(tt->texttype_fn_to_wc)))
							(tt,a,b,c,d,e,f);
	}
									
	USHORT mbtowc(UCS2_CHAR *a, UCHAR *b, USHORT c)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_mbtowc);
		return (*(reinterpret_cast<
					USHORT (*)(TEXTTYPE, UCS2_CHAR*, UCHAR*, USHORT)>
						(tt->texttype_fn_mbtowc)))(tt,a,b,c);
	}

	USHORT contains(class tdbb *a, UCHAR *b,
					USHORT c,
					UCHAR *d,
					USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_contains);
		return (*(reinterpret_cast<
					USHORT (*)(class tdbb *,TextType,UCHAR*,USHORT,UCHAR*,USHORT)>
						(tt->texttype_fn_contains)))
							(a,tt,b,c,d,e);
	}
	
	USHORT like(class tdbb *tdbb, UCHAR *a,
							  SSHORT b,
							  UCHAR *c,
							  SSHORT d,
							  SSHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_like);
		return (*(reinterpret_cast<
					USHORT(*)(class tdbb *,TextType,UCHAR*,short,UCHAR*,short,short)>
						(tt->texttype_fn_like)))(tdbb,tt,a,b,c,d,e);
	}
	
	USHORT matches(class tdbb *tdbb, UCHAR *a, SSHORT b, UCHAR *c, SSHORT d)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_matches);
		return (*(reinterpret_cast<
					USHORT (*)(class tdbb *,TextType,UCHAR*,short,UCHAR*,short)>
						(tt->texttype_fn_matches)))
							(tdbb,tt,a,b,c,d);
	}

	USHORT sleuth_check(class tdbb *tdbb, USHORT a,
								UCHAR *b,
								USHORT c,
								UCHAR *d,
								USHORT e)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_sleuth_check);
		return (*(reinterpret_cast<
					USHORT(*)(class tdbb *,TextType,USHORT,UCHAR*,USHORT,UCHAR*,USHORT)>
						(tt->texttype_fn_sleuth_check)))
							(tdbb,tt,a,b,c,d,e);
	}
	
	USHORT sleuth_merge(class tdbb *tdbb, UCHAR *a,
								USHORT b,
								UCHAR *c,
								USHORT d,
								UCHAR *e,
								USHORT f)
	{
		fb_assert(tt);
		fb_assert(tt->texttype_fn_sleuth_merge);
		return (*(reinterpret_cast<
					USHORT(*)(class tdbb *,TextType,UCHAR*,USHORT,UCHAR*,USHORT,UCHAR*,USHORT)>
						(tt->texttype_fn_sleuth_merge)))
							(tdbb,tt,a,b,c,d,e,f);
	}


	USHORT getType() const {
		fb_assert(tt);
		return tt->texttype_type; 
	}

	const char *getName() const { 
		fb_assert(tt);
		return tt->texttype_name; 
	}

	CHARSET_ID getCharSet() const { 
		fb_assert(tt);
		return tt->texttype_character_set; 
	}

	SSHORT getCountry() const { 
		fb_assert(tt);
		return tt->texttype_country; 
	}

	UCHAR getBytesPerChar() const { 
		fb_assert(tt);
		return tt->texttype_bytes_per_char; 
	}

	struct texttype *getStruct() const { return tt; }

private:
	struct texttype *tt;
};

static inline bool operator ==(const TextType& tt1, const TextType& tt2) {
	return tt1.getStruct() == tt2.getStruct();
}

static inline bool operator !=(const TextType& tt1, const TextType& tt2) {
	return tt1.getStruct() != tt2.getStruct();
}

class CsConvert
{ 
public:
	CsConvert(struct csconvert *_cnvt) : cnvt(_cnvt) {}
	CsConvert(const CsConvert& obj) : cnvt(obj.cnvt) {}

	USHORT convert(UCHAR *a,
				   USHORT b,
				   UCHAR *c,
				   USHORT d,
				   SSHORT *e,
				   USHORT *f)
	{
		fb_assert(cnvt != NULL);
		return (*(reinterpret_cast<USHORT (*)(struct csconvert*, UCHAR*,USHORT,
					UCHAR*,USHORT,short*,USHORT*)>(cnvt->csconvert_convert)))
						(cnvt,a,b,c,d,e,f);
	}

	SSHORT getId() const { fb_assert(cnvt); return cnvt->csconvert_id; }
	const char *getName() const { fb_assert(cnvt); return cnvt->csconvert_name; }
	CHARSET_ID getFromCS() const { fb_assert(cnvt); return cnvt->csconvert_from; }
	CHARSET_ID getToCS() const { fb_assert(cnvt); return cnvt->csconvert_to; }

	struct csconvert *getStruct() const { return cnvt; }

private:
	struct csconvert *cnvt;
};

static inline bool operator ==(const CsConvert& cv1, const CsConvert& cv2) {
	return cv1.getStruct() == cv2.getStruct();
}

static inline bool operator !=(const CsConvert& cv1, const CsConvert& cv2) {
	return cv1.getStruct() != cv2.getStruct();
}

class CharSet
{
public:
	CharSet(struct charset *_cs) : cs(_cs) {}
	CharSet(const CharSet &obj) : cs(obj.cs) {};

	CHARSET_ID getId() const { fb_assert(cs); return cs->charset_id; }
	const char *getName() const { fb_assert(cs); return cs->charset_name; }
	UCHAR minBytesPerChar() const { fb_assert(cs); return cs->charset_min_bytes_per_char; }
	UCHAR maxBytesPerChar() const { fb_assert(cs); return cs->charset_max_bytes_per_char; }
	UCHAR getSpaceLength() const { fb_assert(cs); return cs->charset_space_length; }
	const UCHAR *getSpace() const { fb_assert(cs); return cs->charset_space_character; }

	CsConvert getConvToUnicode() { fb_assert(cs); return &cs->charset_to_unicode; }

	CsConvert getConvFromUnicode() { fb_assert(cs); return &cs->charset_from_unicode; }

	struct charset *getStruct() const { return cs; }

private:
	struct charset *cs;
};

static inline bool operator ==(const CharSet& cs1, const CharSet& cs2) {
	return cs1.getStruct() == cs2.getStruct();
}

static inline bool operator !=(const CharSet& cs1, const CharSet& cs2) {
	return cs1.getStruct() != cs2.getStruct();
}

#endif /* JRD_INTL_CLASSES_H */

