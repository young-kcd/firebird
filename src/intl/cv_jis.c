/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cv_jis.c
 *	DESCRIPTION:	Codeset conversion for JIS family codesets
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
#include "../intl/charsets/cs_sjis.h"
#include "../intl/kanji.h"
#include "cv_jis.h"
#include "cv_narrow.h"

USHORT CVJIS_eucj_to_unicode(CSCONVERT obj, UCS2_CHAR *dest_ptr, USHORT dest_len, UCHAR *src_ptr
							, USHORT src_len, SSHORT *err_code, USHORT *err_position)
{
	UCS2_CHAR *start;
	UCS2_CHAR ch;
	UCS2_CHAR wide;
	UCHAR ch1;
	USHORT src_start = src_len;
	USHORT this_len;

	assert(src_ptr != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVJIS_eucj_to_unicode);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len);

	start = dest_ptr;
	while ((src_len) && (dest_len > 1)) {
		ch1 = *src_ptr++;

		/* Step 1: Convert from EUC to JIS */
		if (!(ch1 & 0x80)) {	/* 1 byte SCHAR */
			/* Plane 0 of EUC-J is defined as ASCII */
			wide = ch1;
			this_len = 1;

			/* Step 2: Convert from ASCII to UNICODE */
			ch = ch1;
		}
		else if (!src_len || !(*src_ptr & 0x80)) {
			/* We received a multi-byte indicator, but either
			   there isn't a 2nd byte or the 2nd byte isn't marked */
			*err_code = CS_BAD_INPUT;
			break;
		}
		else {
			wide = ((ch1 << 8) + (*src_ptr++)) & ~0x8080;
			this_len = 2;

			/* Step 2: Convert from JIS to UNICODE */
			ch = ((USHORT *) obj->csconvert_datatable)
				[((USHORT *) obj->csconvert_misc)
					[(USHORT)wide /	256]
				 + (wide % 256)];
		};


		/* No need to check for CS_CONVERT_ERROR -
		 * EUCJ must convert to Unicode
		 */

		*dest_ptr++ = ch;
		dest_len -= sizeof(*dest_ptr);
		src_len -= this_len;
	};
	if (src_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	};
	*err_position = src_start - src_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}


#define S2E(s1, s2, j1, j2) \
{ \
  if (s2 >= 0x9f) { \
    if (s1 >= 0xe0) j1 = (s1*2 - 0xe0); \
    else j1 = (s1*2 - 0x60); \
    j2 = (s2 + 2); \
  } else { \
    if (s1 >= 0xe0) j1 = (s1*2 - 0xe1); \
    else j1 = (s1*2 - 0x61);\
    if (s2 >= 0x7f) j2 = (s2 + 0x60); \
    else j2 = (s2 +  0x61); \
  } \
}


USHORT CVJIS_sjis_to_unicode(CSCONVERT obj, UCS2_CHAR *dest_ptr, USHORT dest_len, UCHAR *sjis_str
							, USHORT sjis_len, SSHORT *err_code, USHORT *err_position)
{
	UCS2_CHAR *start;
	UCS2_CHAR ch;
	UCS2_CHAR wide;
	UCHAR c1, c2;
	UCHAR tmp1, tmp2;
	USHORT src_start = sjis_len;
	USHORT this_len;
	USHORT table;

	assert(sjis_str != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVJIS_sjis_to_unicode);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (sjis_len * 2);	/* worst case - all ascii input */

	start = dest_ptr;
	while ((sjis_len) && (dest_len > 1)) {
		/* Step 1: Convert from SJIS to JIS code */
		if (*sjis_str & 0x80) {	/* Non-Ascii - High bit set */
			c1 = *sjis_str++;

			if (SJIS1(c1)) {	/* First byte is a KANJI */
				if (sjis_len == 1) {	/* truncated KANJI */
					*err_code = CS_BAD_INPUT;
					break;
				}
				c2 = *sjis_str++;
				if (!(SJIS2(c2))) {	/* Bad second byte */
					*err_code = CS_BAD_INPUT;
					break;
				}
				/* Step 1b: Convert 2 byte SJIS to EUC-J */
				S2E(c1, c2, tmp1, tmp2);

				/* Step 2b: Convert 2 byte EUC-J to JIS */
				wide = ((tmp1 << 8) + tmp2) & ~0x8080;
				this_len = 2;
				table = 1;
			}
			else if (SJIS_SINGLE(c1)) {
				wide = c1;
				this_len = 1;
				table = 2;
			}
			else {				/* It is some bad character */

				*err_code = CS_BAD_INPUT;
				break;
			}
		}
		else {					/* it is a ASCII */

			wide = *sjis_str++;
			this_len = 1;
			table = 2;
		}

		/* Step 2: Convert from JIS code (in wide) to UNICODE */
		if (table == 1)
			ch = ((USHORT *) obj->csconvert_datatable)
				[((USHORT *) obj->csconvert_misc)
					[(USHORT)wide /	256]
				 + (wide % 256)];
		else {
			assert(table == 2);
			assert(wide <= 255);
			ch = sjis_to_unicode_mapping_array
				[sjis_to_unicode_map[(USHORT) wide / 256] + (wide % 256)];
		};

		/* This is only important for bad-SJIS in input stream */
		if ((ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		};
		*dest_ptr++ = ch;
		dest_len -= sizeof(*dest_ptr);
		sjis_len -= this_len;
	}
	if (sjis_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - sjis_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}


/*
From tate.a-t.com!uunet!Eng.Sun.COM!adobe!adobe.com!lunde Tue Nov 19 16:11:24 1991
Return-Path: <uunet!Eng.Sun.COM!adobe!adobe.com!lunde@tate.a-t.com>
Received: by dbase.a-t.com (/\==/\ Smail3.1.21.1 #21.5)
	id <m0kjfXI-0004qKC@dbase.a-t.com>; Tue, 19 Nov 91 16:11 PST
Received: by tate.a-t.com (/\==/\ Smail3.1.21.1 #21.1)
	id <m0kjfPU-000Gf0C@tate.a-t.com>; Tue, 19 Nov 91 16:03 PST
Received: from Sun.COM by relay1.UU.NET with SMTP 
	(5.61/UUNET-internet-primary) id AA21144; Tue, 19 Nov 91 18:45:19 -0500
Received: from Eng.Sun.COM (zigzag-bb.Corp.Sun.COM) by Sun.COM (4.1/SMI-4.1)
	id AA04289; Tue, 19 Nov 91 15:40:59 PST
Received: from cairo.Eng.Sun.COM by Eng.Sun.COM (4.1/SMI-4.1)
	id AA12852; Tue, 19 Nov 91 15:39:48 PST
Received: from snail.Sun.COM by cairo.Eng.Sun.COM (4.1/SMI-4.1)
	id AA23752; Tue, 19 Nov 91 15:36:04 PST
Received: from sun.Eng.Sun.COM by snail.Sun.COM (4.1/SMI-4.1)
	id AA18716; Tue, 19 Nov 91 15:40:34 PST
Received: from adobe.UUCP by sun.Eng.Sun.COM (4.1/SMI-4.1)
	id AA25005; Tue, 19 Nov 91 15:39:21 PST
Received: from lynton.adobe.com by adobe.com (4.0/SMI-4.0)
	id AA22639; Tue, 19 Nov 91 15:39:57 PST
Received: by lynton.adobe.com (4.0/SMI-4.0)
	id AA08147; Tue, 19 Nov 91 15:39:45 PST
Date: Tue, 19 Nov 91 15:39:45 PST
From: tate!uunet!adobe.com!lunde (Ken Lunde)
Message-Id: <9111192339.AA08147@lynton.adobe.com>
To: unicode@Sun.COM
Subject: JIS -> Shift-JIS algorithm...
Status: OR

Hi!

     This posting is in response to Asmus' algorithm for converting
JIS to Shift-JIS.


     This is an explanation of the JIS->Shift-JIS conversion algorithm.
I will first show my function, and then follow-up with a step-by-step
description.
     Note that all values listed are ASCII decimal. Here is the C coded
function:

C O D E:
*/

#define	isodd(x)	((x) & 1)

static void seven2eight(USHORT *p1, USHORT *p2)
{
	if (isodd(*p1))
		*p2 += 31;
	else
		*p2 += 126;
	if ((*p2 >= 127) && (*p2 < 158))
		(*p2)++;
	if ((*p1 >= 33) && (*p1 <= 94)) {
		if (isodd(*p1))
			*p1 = ((*p1 - 1) / 2) + 113;
		else if (!isodd(*p1))
			*p1 = (*p1 / 2) + 112;
	}
	else if ((*p1 >= 95) && (*p1 <= 126)) {
		if (isodd(*p1))
			*p1 = ((*p1 - 1) / 2) + 177;
		else if (!isodd(*p1))
			*p1 = (*p1 / 2) + 176;
	}
}

/*

E X P L A N A T I O N:

ORIGINAL CODE VALUES:
JIS 1st:   76
JIS 2nd:   36

SJIS 1st: 150
SJIS 2nd: 162

STEPS:
1) If the 1st JIS byte is odd, then add 31 to the 2nd byte. Otherwise,
   add 126 to the 2nd byte.

   EX: JIS 1st = 76 (is NOT odd), so JIS 2nd = 162 (36 + 126)

   JIS 1st:   76
   JIS 2nd:  162
   SJIS 1st: 150
   SJIS 2nd: 162

   Note that the second byte has been converted to the target code
   already.

2) If the 2nd JIS byte is greater than or equal to 127 AND less than
   158, then add 1 to the 2nd byte.

   EX: JIS 2nd = 162 (not in the range), so nothing is done.

3) If the 1st JIS byte is greater than or equal to 33 AND less than
   or equal to 94, then do (a). Otherwise, if the 1st JIS byte
   is greater than or equal to 95 AND less than or equal to 126, then
   do step (b).

   a) If the 1st JIS byte is odd, then subtract 1 from its value, divide
      the result by 2, and finally add 113. Otherwise, divide the 1st JIS
      byte by 2 and add 112.

   b) If the 1st JIS byte is odd, then subtract 1 from its value, divide
      the result by 2, and finally add 177. Otherwise, divide the 1st JIS
      byte by 2 and add 176.

EX: JIS 1st is in the range 33-94, so we execute step 3(a). JIS 1st = 76
    (is NOT odd), so JIS 2nd = 150 ((76/2) + 112)
   
JIS 1st:  150
JIS 2nd:  162

SJIS 1st: 150
SJIS 2nd: 162


Here are the specs for JIS:

                 DECIMAL            OCTAL              HEXADECIMAL
1st Byte         033-126            041-176            21-7E
2nd Byte         033-126            041-176            21-7E

Here are the specs for Shift-JIS:

                 DECIMAL            OCTAL              HEXADECIMAL
TYPE 1:
   1st Byte      129-159            201-237            81-9F
   2nd Byte      064-158 (not 127)  100-236 (not 177)  40-9E (not 7F)
TYPE 2:
   1st Byte      224-239            340-357            E0-EF
   2nd Byte      064-158 (not 127)  100-236 (not 177)  40-9E (not 7F)
TYPE 3:
   1st Byte      129-159            201-237            81-9F
   2nd Byte      159-252            237-374            9F-FC
TYPE 4:
   1st Byte      224-239            340-357            E0-EF
   2nd Byte      159-252            237-374            9F-FC
Half-size katakana:
   1st Byte      161-223            241-337            A1-DF

I hope this helps in the discussion.

-- Ken R. Lunde
   Adobe Systems Incorporated
   Japanese Font Production
   lunde@adobe.com
*/


USHORT CVJIS_unicode_to_sjis(CSCONVERT obj, UCHAR *sjis_str, USHORT sjis_len, UCS2_CHAR *unicode_str,
							 USHORT unicode_len, SSHORT *err_code, USHORT *err_position)
{
	UCHAR *start;
	UCS2_CHAR jis_ch;
	UCS2_CHAR wide;
	USHORT tmp1, tmp2;
	USHORT src_start = unicode_len;

	assert(unicode_str != NULL || sjis_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVJIS_unicode_to_sjis);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (sjis_str == NULL)
		return (unicode_len);	/* worst case - all han character input */

	start = sjis_str;
	while ((sjis_len) && (unicode_len > 1)) {
		/* Step 1: Convert from UNICODE to JIS code */
		wide = *unicode_str++;

		jis_ch = ((USHORT *) obj->csconvert_datatable)
				[((USHORT *) obj->csconvert_misc)
					[(USHORT)wide /	256]
				 + (wide % 256)];

		if ((jis_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {

			/* Handle the non-JIS codes in SJIS (ASCII & half-width Kana) */
			jis_ch = sjis_from_unicode_mapping_array
					[sjis_from_unicode_map[(USHORT) wide / 256] + (wide % 256)];
			if ((jis_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
				*err_code = CS_CONVERT_ERROR;
				break;
			};
		};

		/* Step 2: Convert from JIS code to SJIS */
		tmp1 = jis_ch / 256;
		tmp2 = jis_ch % 256;
		if (tmp1 == 0) {		/* ASCII character */
			*sjis_str++ = tmp2;
			sjis_len--;
			unicode_len -= sizeof(*unicode_str);
			continue;
		};
		seven2eight(&tmp1, &tmp2);
		if (tmp1 == 0) {		/* half-width kana ? */
			assert(SJIS_SINGLE(tmp2));
			*sjis_str++ = tmp2;
			unicode_len -= sizeof(*unicode_str);
			sjis_len--;
		}
		else if (sjis_len < 2) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		else {
			assert(SJIS1(tmp1));
			assert(SJIS2(tmp2));
			*sjis_str++ = tmp1;
			*sjis_str++ = tmp2;
			unicode_len -= sizeof(*unicode_str);
			sjis_len -= 2;
		};
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((sjis_str - start) * sizeof(*sjis_str));
}


USHORT CVJIS_unicode_to_eucj(CSCONVERT obj, UCHAR *eucj_str, USHORT eucj_len, UCS2_CHAR *unicode_str,
							 USHORT unicode_len, SSHORT *err_code, USHORT *err_position)
{
	UCHAR *start;
	UCS2_CHAR jis_ch;
	UCS2_CHAR wide;
	USHORT tmp1, tmp2;
	USHORT src_start = unicode_len;

	assert(unicode_str != NULL || eucj_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVJIS_unicode_to_eucj);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (eucj_str == NULL)
		return (unicode_len);	/* worst case - all han character input */

	start = eucj_str;
	while ((eucj_len) && (unicode_len > 1)) {
		/* Step 1: Convert from UNICODE to JIS code */
		wide = *unicode_str++;

		/* ASCII range characters map directly -- others go to the table */
		if (wide <= 0x007F)
			jis_ch = wide;
		else
			jis_ch = ((USHORT *) obj->csconvert_datatable)
					[((USHORT *) obj->csconvert_misc)
						[(USHORT)wide /	256]
					 + (wide % 256)];
		if ((jis_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		};

		/* Step 2: Convert from JIS code to EUC-J */
		tmp1 = jis_ch / 256;
		tmp2 = jis_ch % 256;
		if (tmp1 == 0) {		/* ASCII character */
			assert(!(tmp2 & 0x80));
			*eucj_str++ = tmp2;
			eucj_len--;
			unicode_len -= sizeof(*unicode_str);
			continue;
		};
		if (eucj_len < 2) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		else {
			assert(!(tmp1 & 0x80));
			assert(!(tmp2 & 0x80));
			*eucj_str++ = tmp1 | 0x80;
			*eucj_str++ = tmp2 | 0x80;
			unicode_len -= sizeof(*unicode_str);
			eucj_len -= 2;
		};
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((eucj_str - start) * sizeof(*eucj_str));
}

#ifdef NOT_USED_OR_REPLACED
static USHORT CVJIS_check_euc(UCHAR *euc_str, USHORT euc_len)
{
/**************************************
 *
 *      K A N J I _ c h e c k _ e u c 
 *
 **************************************
 *
 * Functional description
 *	This is a cousin of the KANJI_check_sjis routine.
 *      Make sure that the euc string does not have any truncated 2 byte
 *      character at the end. * If we have a truncated character then, 
 *          return 1.  
 *          else return(0);
 **************************************/
	while (euc_len--) {
		if (*euc_str & 0x80) {	/* Is it  EUC */
			if (euc_len == 0) {	/* truncated kanji */
				return (1);
			}
			else {
				euc_str += 2;
				euc_len -= 1;
			}
		}
		else {					/* it is a ASCII */

			euc_str++;
		}
	}
	return (0);
}



static USHORT CVJIS_check_sjis(UCHAR *sjis_str, USHORT sjis_len)
{
/**************************************
 *
 *      K A N J I _ c h e c k _ s j i s
 *
 **************************************
 *
 * Functional description
 *	This is a cousin of the KANJI_check_euc routine.
 *      Make sure that the sjis string does not have any truncated 2 byte 
 *	character at the end. *	If we have a truncated character then,
 *	    return 1. 
 *	    else return(0);
 **************************************/

	UCHAR c1;

	while (sjis_len--) {
		if (*sjis_str & 0x80) {	/* Is it  SJIS */
			c1 = *sjis_str;
			if (SJIS1(c1)) {	/* It is a KANJI */
				if (sjis_len == 0) {	/* truncated KANJI */
					return (1);
				}
				else {
					sjis_str += 2;
					sjis_len -= 1;
				}
			}
			else {				/*It is a KANA */

				sjis_str++;
			}
		}
		else {					/* it is a ASCII */

			sjis_str++;
		}
	}
	return (0);
}
#endif


static USHORT CVJIS_euc2sjis(CSCONVERT obj, UCHAR *sjis_str, USHORT sjis_len, UCHAR *euc_str
							, USHORT euc_len, SSHORT *err_code, USHORT *err_position)
{
/**************************************
 *
 *      K A N J I _ e u c 2 s j i s
 *
 **************************************
 *
 * Functional description
 *      Convert euc_len number of bytes in euc_str to sjis_str .
 *	sjis_len is the maximum size of the sjis buffer.
 *
 **************************************/
	UCHAR c1, c2;
	UCHAR *sjis_start;
	USHORT src_start = euc_len;

	assert(euc_str != NULL || sjis_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);

	*err_code = 0;

/* Length estimate needed? */
	if (sjis_str == NULL)
		return (euc_len);		/* worst case */

	sjis_start = sjis_str;
	while (euc_len && sjis_len) {
		if (*euc_str & 0x80) {	/* Non-Ascii - High bit set */

			c1 = *euc_str++;

			if (EUC1(c1)) {		/* It is a EUC */
				if (euc_len == 1) {
					*err_code = CS_BAD_INPUT;	/* truncated EUC */
					break;
				}
				c2 = *euc_str++;
				if (!(EUC2(c2))) {
					*err_code = CS_BAD_INPUT;	/* Bad EUC */
					break;
				}
				if (c1 == 0x8e) {	/* Kana */
					sjis_len--;
					*sjis_str++ = c2;
					euc_len -= 2;
				}
				else {			/* Kanji */

					if (sjis_len < 2) {	/*buffer full */
						*err_code = CS_TRUNCATION_ERROR;
						break;
					};
					sjis_len -= 2;
					euc_len -= 2;
					c1 ^= 0x80;
					c2 ^= 0x80;
					*sjis_str++ =
						(USHORT) (c1 - 0x21) / 2 +
						((c1 <= 0x5e) ? 0x81 : 0xc1);
					if (c1 & 1)	/* odd */
						*sjis_str++ = c2 + ((c2 <= 0x5f) ? 0x1f : 0x20);
					else
						*sjis_str++ = c2 + 0x7e;
				}
			}
			else {				/* It is some bad character */

				*err_code = CS_BAD_INPUT;
				break;
			}
		}
		else {					/* ASCII */

			euc_len--;
			sjis_len--;
			*sjis_str++ = *euc_str++;
		}
	}
	if (euc_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;
	*err_position = src_start - euc_len;
	return (sjis_str - sjis_start);
}



USHORT CVJIS_euc_byte2short(CSCONVERT obj, UCHAR *dst, USHORT dst_len, UCHAR *src, USHORT src_len
							, SSHORT *err_code,	USHORT *err_position)
{
/**************************************
 *
 *      C V _ K A N J I _ e u c _ b y t e 2 s h o r t
 *
 **************************************
 *
 * Functional description
 *      Convert len number of bytes of EUC string in
 *	src (char-based buffer) into dst (short-based buffer).
 *	This routine merges:
 *		1-byte ASCII into 1 short, and
 *		2-byte EUC kanji into 1 short.
 *
 **************************************/
	USHORT x;
	UCHAR *dst_start;
	USHORT src_start = src_len;

	assert(src != NULL || dst == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);

	*err_code = 0;
/* Length estimate needed? */
	if (dst == NULL)
		return (2 * src_len);	/* worst case */

	dst_start = dst;
	while (src_len && (dst_len > (sizeof(USHORT) - 1))) {
		if (EUC1(*src)) {
			if (src_len <= 1) {
				*err_code = CS_BAD_INPUT;
				break;
			};
			x = (*src << 8) + (*(src + 1));
			src += 2;
			src_len -= 2;
		}
		else {
			x = *src++;
			src_len--;
		};
		*(USHORT *) dst = x;	/* Assumes alignment */
		dst += sizeof(USHORT);
		dst_len -= sizeof(USHORT);
	}
	if (src_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;
	*err_position = src_start - src_len;
	return (dst - dst_start) * sizeof(*dst);
}


SSHORT CVJIS_euc_mbtowc(CSCONVERT obj, UCS2_CHAR *wc, UCHAR *src, USHORT src_len)
{
/**************************************
 *
 *      C V _ K A N J I _ e u c _ m b t o w c
 *
 **************************************
 *
 * Functional description
 *	Grab a single character from a mb stream.
 *
 **************************************/
	assert(src != NULL);
	assert(obj != NULL);

	if (!src_len)
		return -1;

	if (EUC1(*src)) {
		if (src_len <= 1) {
			return -1;
		};
		if (wc)
			*wc = (*src << 8) + (*(src + 1));
		return 2;
	}
	else {
		if (wc)
			*wc = *src++;
		return 1;
	};
}

static USHORT CVJIS_sjis2euc(CSCONVERT obj, UCHAR *euc_str, USHORT euc_len, UCHAR *sjis_str
							, USHORT sjis_len, SSHORT *err_code, USHORT *err_position)
{
/**************************************
 *
 *      K A N J I _ s j i s 2 e u c
 *
 **************************************
 *
 * Functional description
 *      Convert sjis_len number of bytes in sjis_str to euc_str .
 *
 **************************************/

	UCHAR c1, c2;
	UCHAR *euc_start;
	USHORT src_start = sjis_len;

	assert(sjis_str != NULL || euc_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);

	*err_code = 0;
	if (euc_str == NULL)
		return (2 * sjis_len);	/* worst case */

	euc_start = euc_str;
	while (sjis_len && euc_len) {

		if (*sjis_str & 0x80) {	/* Non-Ascii - High bit set */
			c1 = *sjis_str++;
			if (SJIS1(c1)) {	/* First byte is a KANJI */
				if (sjis_len == 1) {	/* truncated KANJI */
					*err_code = CS_BAD_INPUT;
					break;
				}
				c2 = *sjis_str++;
				if (!(SJIS2(c2))) {	/* Bad second byte */
					*err_code = CS_BAD_INPUT;
					break;
				}
				if (euc_len < 2) {	/*buffer full */
					*err_code = CS_TRUNCATION_ERROR;
					break;
				};
				S2E(c1, c2, *euc_str, *(euc_str + 1));
				euc_str += 2;
				euc_len -= 2;
				sjis_len -= 2;
			}
			else if (SJIS_SINGLE(c1)) {
				if (euc_len < 2) {	/*buffer full */
					*err_code = CS_TRUNCATION_ERROR;
					break;
				}
				euc_len -= 2;	/* Kana */
				sjis_len--;
				*euc_str++ = 0x8e;
				*euc_str++ = c1;
			}
			else {				/* It is some bad character */

				*err_code = CS_BAD_INPUT;
				break;
			}
		}
		else {					/* it is a ASCII */

			euc_len--;
			sjis_len--;
			*euc_str++ = *sjis_str++;
		}
	}
	if (sjis_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;
	*err_position = src_start - sjis_len;
	return (euc_str - euc_start);
}



USHORT CVJIS_sjis_byte2short(CSCONVERT obj, UCHAR *dst, USHORT dst_len, UCHAR *src, USHORT src_len
							, SSHORT *err_code, USHORT *err_position)
{
/**************************************
 *
 *      K A N J I _ s j i s _ b y t e 2 s h o r t
 *
 **************************************
 *
 * Functional description
 *      Convert len number of bytes of SJIS string in
 *	src (char-based buffer) into dst (short-based buffer).
 *	This routine merges:
 *		1-byte ASCII into 1 short,
 *		1-byte SJIS kana 1 short, and
 *		2-byte SJIS kanji into 1 short.
 *	Return the number of "bytes" in dst.
 *
 **************************************/
	USHORT x;
	UCHAR *dst_start;
	USHORT src_start = src_len;

	assert(obj != NULL);
	assert(src != NULL || dst == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);

	*err_code = 0;
	if (dst == NULL)
		return (2 * src_len);	/* worst case */

	dst_start = dst;
	while (src_len && (dst_len > (sizeof(USHORT) - 1))) {
		if (SJIS1(*src)) {
			if (src_len <= 1) {
				*err_code = CS_BAD_INPUT;
				break;
			};
			x = (*src << 8) + *(src + 1);
			src_len -= 2;
			src += 2;
		}
		else {
			x = *src++;
			src_len--;
		}
		*(USHORT *) dst = x;	/* assuming alignment OK */
		dst += sizeof(USHORT);
		dst_len -= sizeof(USHORT);
	}
	if (src_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;

	*err_position = (src_start - src_len);
	return (dst - dst_start) * sizeof(*dst);
}


SSHORT CVJIS_sjis_mbtowc(CSCONVERT obj, UCS2_CHAR *wc, UCHAR *src, USHORT src_len)
{
/**************************************
 *
 *      C V _ K A N J I _ s j i s _ m b t o w c
 *
 **************************************
 *
 * Functional description
 *	Grab a single character from a mb stream.
 *
 **************************************/

	assert(src != NULL);
	assert(obj != NULL);

	if (!src_len)
		return -1;

	if (SJIS1(*src)) {
		if (src_len <= 1) {
			return -1;
		};
		if (wc)
			*wc = (*src << 8) + (*(src + 1));
		return 2;
	}
	else {
		if (wc)
			*wc = *src++;
		return 1;
	};
}

CONVERT_ENTRY(CS_SJIS, CS_EUCJ, CVJIS_sjis_x_eucj)
{
	if (dest_cs == CS_EUCJ)
		CV_convert_init(csptr, dest_cs, source_cs, (FPTR_SHORT) CVJIS_sjis2euc,
						NULL, NULL);
	else
		CV_convert_init(csptr, dest_cs, source_cs, (FPTR_SHORT) CVJIS_euc2sjis,
						NULL, NULL);

	CONVERT_RETURN;
}
