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


USHORT famasc_key_length(TEXTTYPE obj, USHORT inLen);
USHORT famasc_string_to_key(TEXTTYPE obj, USHORT iInLen, BYTE *pInChar, USHORT iOutLen, BYTE *pOutChar
							, USHORT partial);
SSHORT famasc_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2);

USHORT famasc_to_upper(TEXTTYPE obj, BYTE ch);
USHORT famasc_to_lower(TEXTTYPE obj, BYTE ch);
SSHORT famasc_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr);

USHORT cp1251_to_upper(TEXTTYPE obj, BYTE ch);
USHORT cp1251_to_lower(TEXTTYPE obj, BYTE ch);
SSHORT cp1251_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr);

SSHORT LC_DOS_nc_mbtowc(TEXTTYPE obj, UCS2_CHAR *wc, UCHAR *ptr, USHORT count);

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



#undef ASCII_LOWER
#undef ASCII_UPPER
#undef ASCII_LOWER_Z
#undef ASCII_UPPER_Z
#undef ASCII_LOWER_A
#undef ASCII_UPPER_A

#undef ASCII_SPACE
#undef LANGASCII_MAX_KEY
