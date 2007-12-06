/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_ksc.c
 *	DESCRIPTION:	Language Drivers in the KSC-5601.
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
#include "../intlcpp/ldcommon.h"
#include "lc_ascii.h"
#include "cv_ksc.h"

#define		ASCII_SPACE	32

STATIC USHORT LCKSC_string_to_key(TEXTTYPE obj, USHORT iInLen, BYTE *pInChar
									, USHORT iOutLen, BYTE *pOutChar);
STATIC USHORT LCKSC_key_length(TEXTTYPE obj, USHORT inLen);
STATIC SSHORT LCKSC_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2);

static int GetGenHanNdx(UCHAR b1, UCHAR b2);
static int GetSpeHanNdx(UCHAR b1, UCHAR b2);

#define	FAMILY_MULTIBYTE(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type = 			(id_number); \
	cache->texttype_character_set = 	(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	2; \
	cache->texttype_fn_init = 		(FPTR_SHORT) (name); \
	cache->texttype_fn_key_length = 	(FPTR_SHORT) famasc_key_length; \
	cache->texttype_fn_string_to_key =	(FPTR_SHORT) famasc_string_to_key; \
	cache->texttype_fn_compare = 		(FPTR_short) famasc_compare; \
	cache->texttype_fn_to_upper = 		(FPTR_SHORT) famasc_to_upper; \
	cache->texttype_fn_to_lower = 		(FPTR_SHORT) famasc_to_lower; \
	cache->texttype_fn_str_to_upper = 	(FPTR_short) famasc_str_to_upper; \
	cache->texttype_collation_table = 	(BYTE *) NULL; \
	cache->texttype_toupper_table = 	(BYTE *) NULL; \
	cache->texttype_tolower_table = 	(BYTE *) NULL; \
	cache->texttype_compress_table =	(BYTE *) NULL; \
	cache->texttype_expand_table = 		(BYTE *) NULL; \
	cache->texttype_name = 			POSIX;

TEXTTYPE_ENTRY(KSC_5601_init)
{
	static const ASCII POSIX[] = "C.KSC_5601";

	FAMILY_MULTIBYTE(5601, KSC_5601_init, CS_KSC5601, CC_C);

	cache->texttype_fn_to_wc = (FPTR_SHORT) CVKSC_ksc_byte2short;
	cache->texttype_fn_mbtowc = (FPTR_short) CVKSC_ksc_mbtowc;

	TEXTTYPE_RETURN;
}

#include "../intlcpp/collations/undef.h"


TEXTTYPE_ENTRY(ksc_5601_dict_init)
{
	static const ASCII POSIX[] = "HANGUL.KSC_5601";

	FAMILY_MULTIBYTE(5602, ksc_5601_dict_init, CS_KSC5601, CC_KOREA);

	cache->texttype_fn_to_wc = (FPTR_SHORT) CVKSC_ksc_byte2short;
	cache->texttype_fn_mbtowc = (FPTR_short) CVKSC_ksc_mbtowc;
	cache->texttype_fn_key_length = (FPTR_SHORT) LCKSC_key_length;
	cache->texttype_fn_string_to_key = (FPTR_SHORT) LCKSC_string_to_key;
	cache->texttype_fn_compare = (FPTR_short) LCKSC_compare;

	TEXTTYPE_RETURN;
}


unsigned char spe_han[18][2] = {
/* special hangul -> character sets with dictionary collation */
	{ 0xa4, 0xa2 },
	{ 0xa4, 0xa4 },
	{ 0xa4, 0xa7 },
	{ 0xa4, 0xa8 },
	{ 0xa4, 0xa9 },
	{ 0xa4, 0xb1 },
	{ 0xa4, 0xb2 },
	{ 0xa4, 0xb3 },
	{ 0xa4, 0xb5 },
	{ 0xa4, 0xb6 },
	{ 0xa4, 0xb7 },
	{ 0xa4, 0xb8 },
	{ 0xa4, 0xb9 },
	{ 0xa4, 0xba },
	{ 0xa4, 0xbb },
	{ 0xa4, 0xbc },
	{ 0xa4, 0xbd },
	{ 0xa4, 0xbe }
};

unsigned char gen_han[18][2] = {
/* general hangul -> character sets with binary collation */
	{ 0xb1, 0xed },
	{ 0xb3, 0xa9 },
	{ 0xb4, 0xd8 },
	{ 0xb5, 0xfa },
	{ 0xb6, 0xf2 },
	{ 0xb8, 0xb5 },
	{ 0xb9, 0xd8 },
	{ 0xba, 0xfb },
	{ 0xbb, 0xe6 },
	{ 0xbd, 0xcd },
	{ 0xbe, 0xc5 },
	{ 0xc0, 0xd9 },
	{ 0xc2, 0xa4 },
	{ 0xc2, 0xf6 },
	{ 0xc4, 0xaa },
	{ 0xc5, 0xb7 },
	{ 0xc6, 0xc3 },
	{ 0xc7, 0xce }
};



STATIC USHORT LCKSC_string_to_key(TEXTTYPE obj, USHORT iInLen, BYTE *pInChar, USHORT iOutLen, BYTE *pOutChar)
{
	USHORT i;
	int idx;
	BYTE *inbuff;
	BYTE *outbuff;

	assert(pOutChar != NULL);
	assert(pInChar != NULL);
	assert(iInLen <= LANGKSC_MAX_KEY);
	assert(iOutLen <= LANGKSC_MAX_KEY);
	assert(iOutLen >= LCKSC_key_length(obj, iInLen));

	inbuff = pInChar + iInLen - 1;
	while ((inbuff >= pInChar) && (*inbuff == ASCII_SPACE))
		inbuff--;
	iInLen = (inbuff - pInChar + 1);

	outbuff = pOutChar;

	for (i = 0; i < iInLen && iOutLen; i++, pInChar++) {
		if (GEN_HAN(*pInChar, *(pInChar + 1))) {	/* general hangul */
			if (!iOutLen)
				break;
			idx = GetGenHanNdx(*pInChar, *(pInChar + 1));
			if (idx >= 0) {
				*outbuff++ = gen_han[idx][0];
				*outbuff++ = gen_han[idx][1];
				*outbuff++ = 1;
				iOutLen -= 3;
			}
			else {
				*outbuff++ = *pInChar;
				*outbuff++ = *(pInChar + 1);
				iOutLen -= 2;
			}
			pInChar += 1;
			i++;
		}
		else if (SPE_HAN(*pInChar, *(pInChar + 1))) {	/* special hangul */
			if (!iOutLen)
				break;
			idx = GetSpeHanNdx(*pInChar, *(pInChar + 1));
			assert(idx >= 0);
			*outbuff++ = gen_han[idx][0];
			*outbuff++ = gen_han[idx][1];
			*outbuff++ = 2;
			iOutLen -= 3;
			pInChar += 1;
			i++;
		}
		else {					/* ascii or rest -> in case with binary collation */

			*outbuff++ = *pInChar;
			iOutLen--;
			assert(KSC1(*pInChar) || (*pInChar < 0x80));
			if (KSC1(*pInChar)) {	/* the rest characters of KSC_5601 table */
				assert(KSC2(*(pInChar + 1)));
				if (!iOutLen)
					break;
				*outbuff++ = *(pInChar + 1);
				iOutLen--;
				pInChar += 1;
				i++;
			}
			else				/* ascii */
				continue;
		}
	}

	return (outbuff - pOutChar);
}


/*
*	function name	:	GetGenHanNdx
*	description	:	in case of gen_han, get the index number from gen_han table
*/

static int GetGenHanNdx(UCHAR b1, UCHAR b2)
{
	int i;

	for (i = 0; i < 18; i++) {
		if (gen_han[i][0] == b1 && b2 == gen_han[i][1])
			return i;
	}
	return -1;
}


/*
*	function name	:	GetSpeHanNdx
*	description	:	in case of spe_han, get index from spe_han table
*/

static int GetSpeHanNdx(UCHAR b1, UCHAR b2)
{
	int i;

	for (i = 0; i < 18; i++) {
		if (b2 == spe_han[i][1])
			return i;
	}
	return -1;
}


STATIC USHORT LCKSC_key_length(TEXTTYPE obj, USHORT inLen)
{
	USHORT len;

	len = inLen + (inLen / 2);

	return (MIN(len, LANGKSC_MAX_KEY));
}


/*
*	function name	:	LCKSC_compare
*	description	:	compare two string
*/
STATIC SSHORT LCKSC_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
{
	BYTE key1[LANGKSC_MAX_KEY];
	BYTE key2[LANGKSC_MAX_KEY];
	USHORT len1;
	USHORT len2;
	USHORT len;
	USHORT i;

	len1 = LCKSC_string_to_key(obj, l1, s1, sizeof(key1), key1);
	len2 = LCKSC_string_to_key(obj, l2, s2, sizeof(key2), key2);
	len = MIN(len1, len2);
	for (i = 0; i < len; i++) {
		if (key1[i] == key2[i])
			continue;
		else if (key1[i] < key2[i])
			return -1;
		else
			return 1;
	}
	if (len1 < len2)
		return -1;
	else if (len1 > len2)
		return 1;
	else
		return 0;
}


#include "../intlcpp/collations/undef.h"
#undef		FAMILY_MULTIBYTE
#undef		ASCII_SPACE
#undef		LANGKSC_MAX_KEY
#undef		ISASCII
#undef		KSC1
#undef		KSC2
#undef		GEN_HAN
#undef		SPE_HAN
