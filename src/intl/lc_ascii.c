/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_ascii.c
 *	DESCRIPTION:	Language Drivers in the binary collation family.
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

#include "firebird.h"
#include "../intl/ldcommon.h"
#include "lc_ascii.h"

#define FAMILY_ASCII(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type =			(id_number); \
	cache->texttype_character_set =		(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	1; \
	cache->texttype_fn_init =		(FPTR_SHORT) (name); \
	cache->texttype_fn_key_length =		(FPTR_SHORT) famasc_key_length; \
	cache->texttype_fn_string_to_key =	(FPTR_SHORT) famasc_string_to_key; \
	cache->texttype_fn_compare =		(FPTR_short) famasc_compare; \
	cache->texttype_fn_to_upper =		(FPTR_SHORT) famasc_to_upper; \
	cache->texttype_fn_to_lower =		(FPTR_SHORT) famasc_to_lower; \
	cache->texttype_fn_str_to_upper =	(FPTR_short) famasc_str_to_upper; \
	cache->texttype_fn_mbtowc =		(FPTR_short) LC_DOS_nc_mbtowc; \
	cache->texttype_collation_table =	(BYTE *) NULL; \
	cache->texttype_toupper_table =		(BYTE *) NULL; \
	cache->texttype_tolower_table =		(BYTE *) NULL; \
	cache->texttype_compress_table =	(BYTE *) NULL; \
	cache->texttype_expand_table =		(BYTE *) NULL; \
	cache->texttype_name =			(SCHAR *) POSIX;



TEXTTYPE_ENTRY(DOS101_init)
{
	static const ASCII POSIX[] = "C.DOS437";

	FAMILY_ASCII(parm1, DOS101_init, CS_DOS_437, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS107_init)
{
	static const ASCII POSIX[] = "C.DOS865";

	FAMILY_ASCII(parm1, DOS107_init, CS_DOS_865, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS160_init)
{
	static const ASCII POSIX[] = "C.DOS850";

	FAMILY_ASCII(parm1, DOS160_init, CS_DOS_850, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(ISO88591_cp_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, ISO88591_cp_init, CS_ISO8859_1, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88592_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_2";

    FAMILY_ASCII (parm1, ISO88592_cp_init, CS_ISO8859_2, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88593_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_3";

    FAMILY_ASCII (parm1, ISO88593_cp_init, CS_ISO8859_3, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88594_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_4";

    FAMILY_ASCII (parm1, ISO88594_cp_init, CS_ISO8859_4, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88595_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_5";

    FAMILY_ASCII (parm1, ISO88595_cp_init, CS_ISO8859_5, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88596_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_6";

    FAMILY_ASCII (parm1, ISO88596_cp_init, CS_ISO8859_6, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88597_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_7";

    FAMILY_ASCII (parm1, ISO88597_cp_init, CS_ISO8859_7, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88598_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_8";

    FAMILY_ASCII (parm1, ISO88598_cp_init, CS_ISO8859_8, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO88599_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_9";

    FAMILY_ASCII (parm1, ISO88599_cp_init, CS_ISO8859_9, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY (ISO885913_cp_init)
{
    static const ASCII	POSIX[] = "C.ISO8859_13";

    FAMILY_ASCII (parm1, ISO885913_cp_init, CS_ISO8859_13, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY(DOS852_c0_init)
{
	static const ASCII POSIX[] = "C.DOS852";

	FAMILY_ASCII(parm1, DOS852_c0_init, CS_DOS_852, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS857_c0_init)
{
	static const ASCII POSIX[] = "C.DOS857";

	FAMILY_ASCII(parm1, DOS857_c0_init, CS_DOS_857, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS860_c0_init)
{
	static const ASCII POSIX[] = "C.DOS860";

	FAMILY_ASCII(parm1, DOS860_c0_init, CS_DOS_860, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS861_c0_init)
{
	static const ASCII POSIX[] = "C.DOS861";

	FAMILY_ASCII(parm1, DOS861_c0_init, CS_DOS_861, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS863_c0_init)
{
	static const ASCII POSIX[] = "C.DOS863";

	FAMILY_ASCII(parm1, DOS863_c0_init, CS_DOS_863, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS737_c0_init)
{
	static const ASCII POSIX[] = "C.DOS737";

	FAMILY_ASCII(parm1, DOS737_c0_init, CS_DOS_737, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS775_c0_init)
{
	static const ASCII POSIX[] = "C.DOS775";

	FAMILY_ASCII(parm1, DOS775_c0_init, CS_DOS_775, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS858_c0_init)
{
	static const ASCII POSIX[] = "C.DOS858";

	FAMILY_ASCII(parm1, DOS858_c0_init, CS_DOS_858, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS862_c0_init)
{
	static const ASCII POSIX[] = "C.DOS862";

	FAMILY_ASCII(parm1, DOS862_c0_init, CS_DOS_862, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS864_c0_init)
{
	static const ASCII POSIX[] = "C.DOS864";

	FAMILY_ASCII(parm1, DOS864_c0_init, CS_DOS_864, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS866_c0_init)
{
	static const ASCII POSIX[] = "C.DOS866";

	FAMILY_ASCII(parm1, DOS866_c0_init, CS_DOS_866, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(DOS869_c0_init)
{
	static const ASCII POSIX[] = "C.DOS869";

	FAMILY_ASCII(parm1, DOS869_c0_init, CS_DOS_869, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(CYRL_c0_init)
{
	static const ASCII POSIX[] = "C.CYRL";

	FAMILY_ASCII(parm1, CYRL_c0_init, CS_CYRL, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(WIN1250_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1250_c0_init, CS_WIN1250, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(WIN1251_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1251_c0_init, CS_WIN1251, CC_C);
	cache->texttype_fn_to_upper =		(FPTR_SHORT) cp1251_to_upper;
	cache->texttype_fn_to_lower =		(FPTR_SHORT) cp1251_to_lower;
	cache->texttype_fn_str_to_upper =	(FPTR_short) cp1251_str_to_upper;

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(WIN1252_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1252_c0_init, CS_WIN1252, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(WIN1253_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1253_c0_init, CS_WIN1253, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(WIN1254_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1254_c0_init, CS_WIN1254, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY(WIN1255_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_5";

	FAMILY_ASCII(parm1, WIN1255_c0_init, CS_WIN1255, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY(WIN1256_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1256_c0_init, CS_WIN1256, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

TEXTTYPE_ENTRY(WIN1257_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1257_c0_init, CS_WIN1257, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"


TEXTTYPE_ENTRY(NEXT_c0_init)
{
	static const ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, NEXT_c0_init, CS_NEXT, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/collations/undef.h"

#undef FAMILY_ASCII


/*
 * Generic base for InterBase 4.0 Language Driver - ASCII family (binary
 * 8 bit sorting)
 */

#define LANGASCII_MAX_KEY	(MAX_KEY)

#define ASCII_SPACE	32			/* ASCII code for space */
#define ASCII_UPPER_A	65		/* ASCII code for 'A' */
#define ASCII_LOWER_A	(ASCII_UPPER_A + 32)	/* ASCII code for 'a' */
#define ASCII_UPPER_Z	90		/* ASCII code for 'Z' */
#define ASCII_LOWER_Z	(ASCII_UPPER_Z + 32)	/* ASCII code for 'z' */

#define	ASCII7_UPPER(ch) \
	((((UCHAR) (ch) >= (UCHAR) ASCII_LOWER_A) && ((UCHAR) (ch) <= (UCHAR) ASCII_LOWER_Z)) \
		? (UCHAR) ((ch)-ASCII_LOWER_A+ASCII_UPPER_A) \
		: (UCHAR) (ch))
#define	ASCII7_LOWER(ch) \
	((((UCHAR) (ch) >= (UCHAR) ASCII_UPPER_A) && ((UCHAR) (ch) <= (UCHAR) ASCII_UPPER_Z)) \
		? (UCHAR) ((ch)-ASCII_UPPER_A+ASCII_LOWER_A) \
		: (UCHAR) (ch))

#define CP1251_UPPER_A 0xC0
#define CP1251_LOWER_A 0xE0
#define CP1251_UPPER_YA 0xDF
#define CP1251_LOWER_YA 0xFF

#define CP1251_UPPER_EX0 0xA8	// ¨
#define CP1251_LOWER_EX0 0xB8	// ¸
#define CP1251_UPPER_EX1 0xA5	// ¥
#define CP1251_LOWER_EX1 0xB4	// ´
#define CP1251_UPPER_EX2 0xAA	// ª
#define CP1251_LOWER_EX2 0xBA	// º
#define CP1251_UPPER_EX3 0xAF	// ¯
#define CP1251_LOWER_EX3 0xBF	// ¿
#define CP1251_UPPER_EX4 0xB2	// ²
#define CP1251_LOWER_EX4 0xB3	// ³

static inline UCHAR CP1251_UPPER(UCHAR ch)
{
	UCHAR res;

	if (ch >= ASCII_LOWER_A && ch <= ASCII_LOWER_Z)
	{
		res = ch - ASCII_LOWER_A + ASCII_UPPER_A;
	}
	else if (ch >= CP1251_LOWER_A && ch <= CP1251_LOWER_YA)
	{
		res = ch - CP1251_LOWER_A + CP1251_UPPER_A;
	}
	else
	{
		switch (ch)
		{
		case CP1251_LOWER_EX0:
			res = CP1251_UPPER_EX0;
			break;
		case CP1251_LOWER_EX1:
			res = CP1251_UPPER_EX1;
			break;
		case CP1251_LOWER_EX2:
			res = CP1251_UPPER_EX2;
			break;
		case CP1251_LOWER_EX3:
			res = CP1251_UPPER_EX3;
			break;
		case CP1251_LOWER_EX4:
			res = CP1251_UPPER_EX4;
			break;
		default:
			res = ch;
		}
	}

	return res;
}

static inline UCHAR CP1251_LOWER(UCHAR ch)
{
	UCHAR res;

	if (ch >= ASCII_UPPER_A && ch <= ASCII_UPPER_Z)
	{
		res = ch - ASCII_UPPER_A + ASCII_LOWER_A;
	}
	else if (ch >= CP1251_UPPER_A && ch <= CP1251_UPPER_YA)
	{
		res = ch - CP1251_UPPER_A + CP1251_LOWER_A;
	}
	else
	{
		switch (ch)
		{
		case CP1251_UPPER_EX0:
			res = CP1251_LOWER_EX0;
			break;
		case CP1251_UPPER_EX1:
			res = CP1251_LOWER_EX1;
			break;
		case CP1251_UPPER_EX2:
			res = CP1251_LOWER_EX2;
			break;
		case CP1251_UPPER_EX3:
			res = CP1251_LOWER_EX3;
			break;
		case CP1251_UPPER_EX4:
			res = CP1251_LOWER_EX4;
			break;
		default:
			res = ch;
		}
	}

	return res;
}


/*
 * key_length (in_len)
 *
 * For an input string of (in_len) bytes, return the maximum
 * key buffer length.
 *
 * This is used for index buffer allocation within the
 * Engine.
 */
USHORT famasc_key_length(TEXTTYPE obj, USHORT inLen)
{
/* assert (inLen <= LANGASCII_MAX_KEY); - possible upper logic error if true */
	return (MIN(inLen, LANGASCII_MAX_KEY));
}


/*
 *
 *  Convert a user string to a sequence that will collate bytewise.
 *
 *  For ASCII type collation (codepoint collation) this mearly
 *  involves stripping the space character off the key.
 * 
 * RETURN:
 *		Length, in bytes, of returned key
 */
USHORT famasc_string_to_key(TEXTTYPE obj, USHORT iInLen, BYTE *pInChar, USHORT iOutLen, BYTE *pOutChar
	, USHORT partial)
{
	BYTE *outbuff;
	BYTE *inbuff;

	assert(pOutChar != NULL);
	assert(pInChar != NULL);
	assert(iInLen <= LANGASCII_MAX_KEY);
	assert(iOutLen <= LANGASCII_MAX_KEY);
	assert(iOutLen >= famasc_key_length(obj, iInLen));

/* point inbuff at last character */
	inbuff = pInChar + iInLen - 1;

/* skip backwards over all spaces & reset input length */
	while ((inbuff >= pInChar) && (*inbuff == ASCII_SPACE))
		inbuff--;
	iInLen = (inbuff - pInChar + 1);

	outbuff = pOutChar;
	while (iInLen-- && iOutLen--) {
		*outbuff++ = *pInChar++;
	}
	return (outbuff - pOutChar);
}


static SSHORT all_spaces(BYTE *s, SSHORT len)
{
	assert(s != NULL);

	while (len-- > 0)
		if (*s++ != ASCII_SPACE)
			return FALSE;
	return TRUE;
}


SSHORT famasc_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
{
	USHORT len;
	USHORT i;

	assert(obj != NULL);
	assert(s1 != NULL);
	assert(s2 != NULL);

	len = MIN(l1, l2);
	for (i = 0; i < len; i++) {
		if (s1[i] == s2[i])
			continue;
		else if (all_spaces(&s1[i], (SSHORT) (l1 - i)))
			return -1;
		else if (all_spaces(&s2[i], (SSHORT) (l2 - i)))
			return 1;
		else if (s1[i] < s2[i])
			return -1;
		else
			return 1;
	}

	if (l1 > len) {
		if (all_spaces(&s1[len], (SSHORT) (l1 - len)))
			return 0;
		return 1;
	}
	if (l2 > len) {
		if (all_spaces(&s2[len], (SSHORT) (l2 - len)))
			return 0;
		return -1;
	}
	return (0);
}



USHORT famasc_to_upper(TEXTTYPE obj, BYTE ch)
{
	return ((USHORT) ASCII7_UPPER(ch));
}



SSHORT famasc_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr)
{
	BYTE *p;
	assert(pStr != NULL);
	assert(pOutStr != NULL);
	assert(iLen <= 32000);		/* almost certainly an error */
	assert(iOutLen <= 32000);	/* almost certainly an error */
	assert(iOutLen >= iLen);
	p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = ASCII7_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	};
	if (iLen != 0)
		return (-1);
	return (pOutStr - p);
}




USHORT famasc_to_lower(TEXTTYPE obj, BYTE ch)
{
	return (ASCII7_LOWER(ch));
}


USHORT cp1251_to_upper(TEXTTYPE obj, BYTE ch)
{
	return ((USHORT) CP1251_UPPER(ch));
}



SSHORT cp1251_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr)
{
	BYTE *p;
	assert(pStr != NULL);
	assert(pOutStr != NULL);
	assert(iLen <= 32000);		/* almost certainly an error */
	assert(iOutLen <= 32000);	/* almost certainly an error */
	assert(iOutLen >= iLen);
	p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = CP1251_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	};
	if (iLen != 0)
		return (-1);
	return (pOutStr - p);
}




USHORT cp1251_to_lower(TEXTTYPE obj, BYTE ch)
{
	return (CP1251_LOWER(ch));
}

#undef ASCII_LOWER
#undef ASCII_UPPER
#undef ASCII_LOWER_Z
#undef ASCII_UPPER_Z
#undef ASCII_LOWER_A
#undef ASCII_UPPER_A

#undef CP1251_LOWER
#undef CP1251_UPPER
#undef CP1251_LOWER_YA
#undef CP1251_UPPER_YA
#undef CP1251_LOWER_A
#undef CP1251_UPPER_A

#undef ASCII_SPACE
#undef LANGASCII_MAX_KEY
