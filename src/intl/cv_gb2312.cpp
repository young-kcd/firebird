/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cv_gb2312.cpp
 *	DESCRIPTION:	Codeset conversion for GB2312 family codesets
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
#include "../intl/cv_gb2312.h"
#include "ld_proto.h"

USHORT CVGB_gb2312_to_unicode(CSCONVERT obj,
							  UCS2_CHAR *dest_ptr,
							  USHORT dest_len,
							  const UCHAR* src_ptr,
							  USHORT src_len,
							  SSHORT *err_code,
							  USHORT *err_position)
{
	fb_assert(src_ptr != NULL || dest_ptr == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_convert == reinterpret_cast<pfn_INTL_convert>(CVGB_gb2312_to_unicode));
	fb_assert(obj->csconvert_datatable != NULL);
	fb_assert(obj->csconvert_misc != NULL);

	const USHORT src_start = src_len;
	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len * sizeof(UCS2_CHAR));

	UCS2_CHAR wide;
	USHORT this_len;
	const UCS2_CHAR* const start = dest_ptr;
	while ((src_len) && (dest_len > 1)) {
		if (*src_ptr & 0x80) {
			const UCHAR c1 = *src_ptr++;

			if (GB1(c1)) {		/* first byte is GB2312 */
				if (src_len == 1) {
					*err_code = CS_BAD_INPUT;
					break;
				}
				const UCHAR c2 = *src_ptr++;
				if (!(GB2(c2))) {	/* Bad second byte */
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

		/* Convert from GB2312 to UNICODE */
		const UCS2_CHAR ch = ((const USHORT*) obj->csconvert_datatable)
			[((const USHORT*) obj->csconvert_misc)[(USHORT) wide / 256]
			 + (wide % 256)];

		if ((ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}

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


USHORT CVGB_unicode_to_gb2312(CSCONVERT obj,
							  UCHAR *gb_str,
							  USHORT gb_len,
							  const UCS2_CHAR* unicode_str,
							  USHORT unicode_len,
							  SSHORT *err_code, 
							  USHORT *err_position)
{
	fb_assert(unicode_str != NULL || gb_str == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_convert == reinterpret_cast<pfn_INTL_convert>(CVGB_unicode_to_gb2312));
	fb_assert(obj->csconvert_datatable != NULL);
	fb_assert(obj->csconvert_misc != NULL);

	const USHORT src_start = unicode_len;
	*err_code = 0;

/* See if we're only after a length estimate */
	if (gb_str == NULL)
		return (unicode_len);	/* worst case - all han character input */

	const UCHAR* const start = gb_str;
	while ((gb_len) && (unicode_len > 1)) {
		/* Convert from UNICODE to GB2312 code */
		const UCS2_CHAR wide = *unicode_str++;

		const UCS2_CHAR gb_ch = ((const USHORT*) obj->csconvert_datatable)
				[((const USHORT*) obj->csconvert_misc)
					[(USHORT)wide / 256]
				 + (wide % 256)];
		if ((gb_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		};

		const int tmp1 = gb_ch / 256;
		const int tmp2 = gb_ch % 256;
		if (tmp1 == 0) {		/* ASCII character */
			*gb_str++ = tmp2;
			gb_len--;
			unicode_len -= sizeof(*unicode_str);
			continue;
		};
		if (gb_len < 2) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		else {
			fb_assert(GB1(tmp1));
			fb_assert(GB2(tmp2));
			*gb_str++ = tmp1;
			*gb_str++ = tmp2;
			unicode_len -= sizeof(*unicode_str);
			gb_len -= 2;
		};
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((gb_str - start) * sizeof(*gb_str));
}


USHORT CVGB_check_gb2312(const UCHAR *gb_str,
						 USHORT gb_len)
{
/**************************************
 * Functional description
 *      Make sure that the GB2312 string does not have any truncated 2 byte
 *      character at the end. 
 * If we have a truncated character then, 
 *          return 1.  
 *          else return(0);
 **************************************/
	while (gb_len--) {
		const UCHAR c1 = *gb_str;
		if (GB1(c1)) {			/* Is it  GB2312 */
			if (gb_len == 0)	/* truncated GB2312 */
				return (1);
			else {
				gb_str += 2;
				gb_len -= 1;
			}
		}
		else {					/* it is a ASCII */

			gb_str++;
		}
	}
	return (0);
}


USHORT CVGB_gb2312_byte2short(TEXTTYPE obj,
							  USHORT* dst,
							  USHORT dst_len,
							  const UCHAR* src,
							  USHORT src_len,
							  SSHORT *err_code,
							  USHORT *err_position)
{
/**************************************
 * Functional description
 *      Convert len number of bytes of GB2312 string in
 *	src (char-based buffer) into dst (short-based buffer).
 *	This routine merges:
 *		1-byte ASCII into 1 short, and
 *		2-byte GB2312 character into 1 short.
 *
 **************************************/
	fb_assert(src != NULL || dst == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);

	const USHORT src_start = src_len;
	*err_code = 0;
/* Length estimate needed? */
	if (dst == NULL)
		return (2 * src_len);	/* worst case */

	USHORT x;
	const USHORT* const dst_start = dst;
	while (src_len && (dst_len > (sizeof(USHORT) - 1))) {
		if (GB1(*src)) {
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
		*dst = x;	/* Assumes alignment */
		++dst;
		dst_len -= sizeof(USHORT);
	}
	if (src_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;
	*err_position = src_start - src_len;
	return (dst - dst_start) * sizeof(*dst);
}


SSHORT CVGB_gb2312_mbtowc(TEXTTYPE obj,
						  UCS2_CHAR* wc,
						  const UCHAR* src,
						  USHORT src_len)
{
/**************************************
 * Functional description
 *	Grab a single character from a mb stream.
 *
 **************************************/
	fb_assert(src != NULL);
	fb_assert(obj != NULL);

	if (!src_len)
		return -1;

	if (GB1(*src)) {
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

