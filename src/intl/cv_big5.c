/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cv_big5.c
 *	DESCRIPTION:	Codeset conversion for BIG5 family codesets
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
#include "../intl/cv_big5.h"

USHORT CVBIG5_big5_to_unicode(CSCONVERT obj, UCS2_CHAR *dest_ptr, USHORT dest_len, UCHAR *src_ptr
							, USHORT src_len, SSHORT *err_code, USHORT *err_position)
{
	UCS2_CHAR *start;
	UCS2_CHAR ch;
	UCS2_CHAR wide;
	USHORT src_start = src_len;
	USHORT this_len;
	UCHAR c1, c2;

	assert(src_ptr != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVBIG5_big5_to_unicode);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len * sizeof(UCS2_CHAR));

	start = dest_ptr;
	while ((src_len) && (dest_len > 1)) {
		if (*src_ptr & 0x80) {
			c1 = *src_ptr++;

			if (BIG51(c1)) {	/* first byte is Big5 */
				if (src_len == 1) {
					*err_code = CS_BAD_INPUT;
					break;
				}
				c2 = *src_ptr++;
				if (!(BIG52(c2))) {	/* Bad second byte */
					*err_code = CS_BAD_INPUT;
					break;
				}
				wide = (c1 << 8) + c2;
				this_len = 2;
			}
			else {
				*err_code = CS_BAD_INPUT;
				break;
			}
		}
		else {					/* it is ASCII */

			wide = *src_ptr++;
			this_len = 1;
		}

		/* Convert from BIG5 to UNICODE */
		ch = ((USHORT *) obj->csconvert_datatable)
			[((USHORT *) obj->csconvert_misc)[(USHORT) wide / 256]
			 + (wide % 256)];

		if ((ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}

		*dest_ptr++ = ch;
		dest_len -= sizeof(UCS2_CHAR);
		src_len -= this_len;
	};
	if (src_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	};
	*err_position = src_start - src_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}


USHORT CVBIG5_unicode_to_big5(CSCONVERT obj, UCHAR *big5_str, USHORT big5_len, UCS2_CHAR *unicode_str,
							  USHORT unicode_len, SSHORT *err_code, USHORT *err_position)
{
	UCHAR *start;
	UCS2_CHAR big5_ch;
	UCS2_CHAR wide;
	int tmp1, tmp2;
	USHORT src_start = unicode_len;

	assert(unicode_str != NULL || big5_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVBIG5_unicode_to_big5);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (big5_str == NULL)
		return (unicode_len);	/* worst case - all han character input */

	start = big5_str;
	while ((big5_len) && (unicode_len > 1)) {
		/* Convert from UNICODE to BIG5 code */
		wide = *unicode_str++;

		big5_ch = ((USHORT *) obj->csconvert_datatable)
				[((USHORT *) obj->csconvert_misc)
					[(USHORT)wide/256]
				 +(wide % 256)];
		if ((big5_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		};

		tmp1 = big5_ch / 256;
		tmp2 = big5_ch % 256;
		if (tmp1 == 0) {		/* ASCII character */
			*big5_str++ = tmp2;
			big5_len--;
			unicode_len -= sizeof(*unicode_str);
			continue;
		};
		if (big5_len < 2) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		else {
			assert(BIG51(tmp1));
			assert(BIG52(tmp2));
			*big5_str++ = tmp1;
			*big5_str++ = tmp2;
			unicode_len -= sizeof(*unicode_str);
			big5_len -= 2;
		};
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((big5_str - start) * sizeof(*big5_str));
}


USHORT CVBIG5_check_big5(UCHAR *big5_str, USHORT big5_len)
{
/**************************************
 * Functional description
 *      Make sure that the big5 string does not have any truncated 2 byte
 *      character at the end. 
 * If we have a truncated character then, 
 *          return 1.  
 *          else return(0);
 **************************************/
	UCHAR c1;

	while (big5_len--) {
		c1 = *big5_str;
		if (BIG51(c1)) {		/* Is it  BIG-5 */
			if (big5_len == 0)	/* truncated BIG-5 */
				return (1);
			else {
				big5_str += 2;
				big5_len -= 1;
			}
		}
		else {					/* it is a ASCII */

			big5_str++;
		}
	}
	return (0);
}


USHORT CVBIG5_big5_byte2short(CSCONVERT obj, UCHAR *dst, USHORT dst_len, UCHAR *src, USHORT src_len
							, SSHORT *err_code,  USHORT *err_position)
{
/**************************************
 * Functional description
 *      Convert len number of bytes of BIG5 string in
 *	src (char-based buffer) into dst (short-based buffer).
 *	This routine merges:
 *		1-byte ASCII into 1 short, and
 *		2-byte BIG5 character into 1 short.
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
		if (BIG51(*src)) {
			if (src_len < 2) {
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


SSHORT CVBIG5_big5_mbtowc(CSCONVERT obj, UCS2_CHAR *wc, UCHAR *src, USHORT src_len)
{
/**************************************
 * Functional description
 *	Grab a single character from a mb stream.
 *
 **************************************/
	assert(src != NULL);
	assert(obj != NULL);

	if (!src_len)
		return -1;

	if (BIG51(*src)) {
		if (src_len < 2) {
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
