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


#include "../intl/ldcommon.h"

extern USHORT famasc_key_length();
extern USHORT famasc_string_to_key();
extern SSHORT famasc_compare();
extern USHORT famasc_to_upper();
extern SSHORT famasc_str_to_upper();
extern USHORT famasc_to_lower();
extern SSHORT LC_DOS_nc_mbtowc();

#define FAMILY_ASCII(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type =			(id_number); \
	cache->texttype_character_set =		(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	1; \
	cache->texttype_fn_init =		(FPTR_SHORT) (name); \
	cache->texttype_fn_key_length =		famasc_key_length; \
	cache->texttype_fn_string_to_key =	famasc_string_to_key; \
	cache->texttype_fn_compare =		famasc_compare; \
	cache->texttype_fn_to_upper =		famasc_to_upper; \
	cache->texttype_fn_to_lower =		famasc_to_lower; \
	cache->texttype_fn_str_to_upper =	famasc_str_to_upper; \
	cache->texttype_fn_mbtowc =		LC_DOS_nc_mbtowc; \
	cache->texttype_collation_table =	(BYTE *) NULL; \
	cache->texttype_toupper_table =		(BYTE *) NULL; \
	cache->texttype_tolower_table =		(BYTE *) NULL; \
	cache->texttype_compress_table =	(BYTE *) NULL; \
	cache->texttype_expand_table =		(BYTE *) NULL; \
	cache->texttype_name =			(SCHAR *) POSIX;



TEXTTYPE_ENTRY(DOS101_init)
{
	static CONST ASCII POSIX[] = "C.DOS437";

	FAMILY_ASCII(parm1, DOS101_init, CS_DOS_437, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS107_init)
{
	static CONST ASCII POSIX[] = "C.DOS865";

	FAMILY_ASCII(parm1, DOS107_init, CS_DOS_865, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS160_init)
{
	static CONST ASCII POSIX[] = "C.DOS850";

	FAMILY_ASCII(parm1, DOS160_init, CS_DOS_850, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LATIN1_cp_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, LATIN1_cp_init, CS_LATIN1, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"

TEXTTYPE_ENTRY (LATIN2_cp_init)
{
    static CONST ASCII	POSIX[] = "C.ISO8859_2";

    FAMILY_ASCII (parm1, LATIN2_cp_init, CS_LATIN2, CC_C); 
    
    TEXTTYPE_RETURN;
}

#include "../intl/undef.h"

TEXTTYPE_ENTRY(DOS852_c0_init)
{
	static CONST ASCII POSIX[] = "C.DOS852";

	FAMILY_ASCII(parm1, DOS852_c0_init, CS_DOS_852, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS857_c0_init)
{
	static CONST ASCII POSIX[] = "C.DOS857";

	FAMILY_ASCII(parm1, DOS857_c0_init, CS_DOS_857, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS860_c0_init)
{
	static CONST ASCII POSIX[] = "C.DOS860";

	FAMILY_ASCII(parm1, DOS860_c0_init, CS_DOS_860, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS861_c0_init)
{
	static CONST ASCII POSIX[] = "C.DOS861";

	FAMILY_ASCII(parm1, DOS861_c0_init, CS_DOS_861, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(DOS863_c0_init)
{
	static CONST ASCII POSIX[] = "C.DOS863";

	FAMILY_ASCII(parm1, DOS863_c0_init, CS_DOS_863, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(CYRL_c0_init)
{
	static CONST ASCII POSIX[] = "C.CYRL";

	FAMILY_ASCII(parm1, CYRL_c0_init, CS_CYRL, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1250_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1250_c0_init, CS_WIN1250, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1251_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1251_c0_init, CS_WIN1251, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1252_c0_init, CS_WIN1252, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1253_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1253_c0_init, CS_WIN1253, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1254_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, WIN1254_c0_init, CS_WIN1254, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(NEXT_c0_init)
{
	static CONST ASCII POSIX[] = "C.ISO8859_1";

	FAMILY_ASCII(parm1, NEXT_c0_init, CS_NEXT, CC_C);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"

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



/*
 * key_length (in_len)
 *
 * For an input string of (in_len) bytes, return the maximum
 * key buffer length.
 *
 * This is used for index buffer allocation within the
 * Engine.
 */
USHORT famasc_key_length(obj, inLen)
	 TEXTTYPE obj;
	 USHORT inLen;
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
USHORT famasc_string_to_key(obj, iInLen, pInChar, iOutLen, pOutChar, partial)
	 TEXTTYPE obj;
	 USHORT iInLen;
	 BYTE *pInChar;
	 USHORT iOutLen;
	 BYTE *pOutChar;
	 USHORT partial;
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


static SSHORT all_spaces(s, len)
	 BYTE *s;
	 SSHORT len;
{
	assert(s != NULL);

	while (len-- > 0)
		if (*s++ != ASCII_SPACE)
			return FALSE;
	return TRUE;
}


SSHORT famasc_compare(obj, l1, s1, l2, s2)
	 TEXTTYPE obj;
	 USHORT l1;
	 BYTE *s1;
	 USHORT l2;
	 BYTE *s2;
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
		else if (all_spaces(&s1[i], l1 - i))
			return -1;
		else if (all_spaces(&s2[i], l2 - i))
			return 1;
		else if (s1[i] < s2[i])
			return -1;
		else
			return 1;
	}

	if (l1 > len) {
		if (all_spaces(&s1[len], l1 - len))
			return 0;
		return 1;
	}
	if (l2 > len) {
		if (all_spaces(&s2[len], l2 - len))
			return 0;
		return -1;
	}
	return (0);
}



USHORT famasc_to_upper(obj, ch)
	 TEXTTYPE obj;
	 BYTE ch;
{
	return ((USHORT) ASCII7_UPPER(ch));
}



SSHORT famasc_str_to_upper(obj, iLen, pStr, iOutLen, pOutStr)
	 TEXTTYPE obj;
	 USHORT iLen;
	 BYTE *pStr;
	 USHORT iOutLen;
	 BYTE *pOutStr;
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




USHORT famasc_to_lower(obj, ch)
	 TEXTTYPE obj;
	 BYTE ch;
{
	return (ASCII7_LOWER(ch));
}


#undef ASCII_LOWER
#undef ASCII_UPPER
#undef ASCII_LOWER_Z
#undef ASCII_UPPER_Z
#undef ASCII_LOWER_A
#undef ASCII_UPPER_A

#undef ASCII_SPACE
#undef LANGASCII_MAX_KEY
