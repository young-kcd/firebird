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

#include "../common/classes/alloc.h"

typedef unsigned short CHARSET_ID;
typedef unsigned short COLLATE_ID;
typedef unsigned short TTYPE_ID;
typedef unsigned short UCS2_CHAR;
typedef unsigned char NCHAR;
typedef unsigned char MBCHAR;
typedef class tdbb *TDBB;
typedef signed char ASCII;
typedef unsigned char BYTE;
typedef unsigned short (*FPTR_SHORT) ();

enum intl_object_types
{
	type_texttype = 54,
	type_charset = 55,
	type_csconvert = 56
};

class TextType
{
public:
	TextType(unsigned short type, const char *name, CHARSET_ID cs_id,
					short country, unsigned char bpc) :
    		texttype_type(type),
    		texttype_name(name),
    		texttype_character_set(cs_id),
    		texttype_country(country),
    		texttype_bytes_per_char(bpc)
		{}

	// texttype_fn_init
	//static texttype *alloc(MemoryPool &, short parm1, short parm2);
	virtual unsigned short key_length(unsigned short) = 0;
	virtual unsigned short string_to_key(unsigned short,
										 unsigned char*,
										 unsigned short,
										 unsigned char*,
										 unsigned short) = 0;
	virtual short compare(unsigned short,
						  unsigned char*,
						  unsigned short,
						  unsigned char*) = 0;
	virtual unsigned short to_upper(unsigned short) = 0;
	virtual unsigned short to_lower(unsigned short) = 0;
	virtual short str_to_upper(unsigned short,
							   unsigned char*,
							   unsigned short,
							   unsigned char*) = 0;
	/* SD: to_wc returns estimated or real length of the result string in bytes that is rather strange */
	virtual unsigned short to_wc(UCS2_CHAR*,	// destination buffer
				     unsigned short,	// length of the destination buffer in bytes
				     unsigned char*,	// source buffer
				     unsigned short,	// length of source buffer in bytes
				     short*,		// variable to return error code
				     unsigned short*) = 0;	// variable to return offset of the first
							// unprocessed char
	virtual unsigned short contains(TDBB, unsigned char*,
									unsigned short,
									unsigned char*,
									unsigned short) = 0;
	virtual unsigned short like(TDBB, unsigned char*,
								short,
								unsigned char*,
								short,
								short) = 0;
	virtual unsigned short matches(TDBB, unsigned char*, short,
								   unsigned char*, short) = 0;
	virtual unsigned short sleuth_check(TDBB, unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short) = 0;
	virtual unsigned short sleuth_merge(TDBB, unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short) = 0;
	virtual unsigned short mbtowc(UCS2_CHAR*, unsigned char*, unsigned short) = 0;
	//virtual short get_wchar(unsigned short*, unsigned char*, unsigned short) = 0;

	//unsigned short getVersion() { return texttype_version; }
	//unsigned short getFlags() { return texttype_flags; }
	unsigned short getType() { return texttype_type; }
	const signed char *getName() { return (const signed char*)texttype_name; }
	CHARSET_ID getCharSet() { return texttype_character_set; }
	short getCountry() { return texttype_country; }
	unsigned char getBytesPerChar() { return texttype_bytes_per_char; }

protected:
	unsigned short texttype_type;		/* Interpretation ID */
	const char *texttype_name;
	CHARSET_ID texttype_character_set;	/* ID of base character set */
	short texttype_country;	/* ID of base country values */
	unsigned char texttype_bytes_per_char;	/* max bytes per character */
};

class TextTypeNC : public TextType
{
public:
	TextTypeNC(unsigned short type, const char *name, CHARSET_ID cs_id,
					short country, unsigned char bpc) :
    		TextType(type, name, cs_id, country, bpc)
		{}

	unsigned short to_wc(UCS2_CHAR*,	// destination buffer
			     unsigned short,	// length of the destination buffer in bytes
			     unsigned char*,	// source buffer
			     unsigned short,	// length of source buffer in bytes
			     short*,		// variable to return error code
			     unsigned short*);	// variable to return offset of the first
						// unprocessed char
	unsigned short contains(TDBB, unsigned char*,
									unsigned short,
									unsigned char*,
									unsigned short);
	unsigned short like(TDBB, unsigned char*,
								short,
								unsigned char*,
								short,
								short);
	unsigned short matches(TDBB, unsigned char*, short,
								   unsigned char*, short);
	unsigned short sleuth_check(TDBB, unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short sleuth_merge(TDBB, unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short mbtowc(UCS2_CHAR*, unsigned char*, unsigned short);
};

class TextTypeWC : public TextType
{
public:
	TextTypeWC(unsigned short type, const char *name, CHARSET_ID cs_id,
					short country, unsigned char bpc) :
    		TextType(type, name, cs_id, country, bpc)
		{}

	unsigned short to_wc(UCS2_CHAR*,	// destination buffer
			     unsigned short,	// length of the destination buffer in bytes
			     unsigned char*,	// source buffer
			     unsigned short,	// length of source buffer in bytes
			     short*,		// variable to return error code
			     unsigned short*);	// variable to return offset of the first
						// unprocessed char
	unsigned short contains(TDBB, unsigned char*,
									unsigned short,
									unsigned char*,
									unsigned short);
	unsigned short like(TDBB, unsigned char*,
								short,
								unsigned char*,
								short,
								short);
	unsigned short matches(TDBB, unsigned char*, short,
								   unsigned char*, short);
	unsigned short sleuth_check(TDBB, unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short sleuth_merge(TDBB, unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short mbtowc(UCS2_CHAR*, unsigned char*, unsigned short);
};

class TextTypeMB : public TextType
{
public:
	TextTypeMB(unsigned short type, const char *name, CHARSET_ID cs_id,
					short country, unsigned char bpc) :
    		TextType(type, name, cs_id, country, bpc)
		{}

	unsigned short contains(TDBB, unsigned char*,
									unsigned short,
									unsigned char*,
									unsigned short);
	unsigned short like(TDBB, unsigned char*,
								short,
								unsigned char*,
								short,
								short);
	unsigned short matches(TDBB, unsigned char*, short,
								   unsigned char*, short);
	unsigned short sleuth_check(TDBB, unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short sleuth_merge(TDBB, unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short,
										unsigned char*,
										unsigned short);
	unsigned short mbtowc(UCS2_CHAR*, unsigned char*, unsigned short);
};

#define TEXTTYPE_init               1	/* object has been init'ed */
#define TEXTTYPE_reverse_secondary  2	/* Reverse order of secondary keys */
#define TEXTTYPE_ignore_specials    4	/* Do not put special values in keys */
#define TEXTTYPE_expand_before      8	/* Expansion weights before litagure */


class CsConvert
{ 
public:
	CsConvert(short id, const char *name, CHARSET_ID from, CHARSET_ID to) :
			csconvert_id(id),
			csconvert_name(name),
			csconvert_from(from),
			csconvert_to(to)
		{}

	virtual unsigned short convert(unsigned char*,
								   unsigned short,
								   unsigned char*,
								   unsigned short,
								   short*,
								   unsigned short*) = 0;

	//unsigned short getVersion() { return csconvert_version; }
	//unsigned short getFlags() { return csconvert_flags; }
	short getId() { return csconvert_id; }
	const signed char *getName() { return (const signed char*)csconvert_name; }
	CHARSET_ID getFromCS() { return csconvert_from; }
	CHARSET_ID getToCS() { return csconvert_to; }

protected:
	short csconvert_id;
	const char *csconvert_name;
	CHARSET_ID csconvert_from;
	CHARSET_ID csconvert_to;
};

/* values for csconvert_flags */

#define CONVERTTYPE_init	1	/* object has been init'ed */

/* Conversion error codes */

#define	CS_TRUNCATION_ERROR	1	/* output buffer too small  */
#define	CS_CONVERT_ERROR	2	/* can't remap a character      */
#define	CS_BAD_INPUT		3	/* input string detected as bad */

#define	CS_CANT_MAP		0		/* Flag table entries that don't map */





class CharSet
{
public:
	CharSet(CHARSET_ID id, const char *name, unsigned char mnBpc, unsigned char mxBpc,
			unsigned char spLen, const char *spc) :
		charset_id(id),
		charset_name(name),
		charset_min_bytes_per_char(mnBpc),
		charset_max_bytes_per_char(mxBpc),
		charset_space_length(spLen),
		charset_space_character(spc),
		charset_to_unicode(0),
		charset_from_unicode(0)
	{}

	virtual ~CharSet()
		{ delete charset_to_unicode; delete charset_from_unicode; }

	//unsigned short getVersion() { return charset_version; }
	//unsigned short getFlags() { return charset_flags; }
	CHARSET_ID getId() { return charset_id; }
	const signed char *getName() { return (const signed char*)charset_name; }
	unsigned char minBytesPerChar() { return charset_min_bytes_per_char; }
	unsigned char maxBytesPerChar() { return charset_max_bytes_per_char; }
	unsigned char getSpaceLength() { return charset_space_length; }
	const unsigned char *getSpace() { return (const unsigned char*)charset_space_character; }

	CsConvert *getConvToUnicode() { return charset_to_unicode; }
	CsConvert *getConvFromUnicode() { return charset_from_unicode; }
	
protected:
	CHARSET_ID charset_id;
	const char *charset_name;
	unsigned char charset_min_bytes_per_char;
	unsigned char charset_max_bytes_per_char;
	unsigned char charset_space_length;
	const char *charset_space_character;

	CsConvert *charset_to_unicode;
	CsConvert *charset_from_unicode;

/* These need to be moved elsewhere.  They don't belong in this interface
	VEC charset_converters;
	VEC charset_collations;
*/
};

/* values for charset_flags */

#define	CHARSET_init	1
#define CHARSET_narrow	2
#define CHARSET_multi	4
#define CHARSET_wide	8


typedef CharSet *(*CharSetAllocFunc)(MemoryPool&, CHARSET_ID, CHARSET_ID);
typedef TextType *(*TextTypeAllocFunc)(MemoryPool&, CHARSET_ID, CHARSET_ID);
typedef CsConvert *(*CsConvertAllocFunc)(MemoryPool&, CHARSET_ID, CHARSET_ID);

#endif /* JRD_INTL_CLASSES_H */
