/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cv_ksc.c
 *	DESCRIPTION:	Codeset conversion for KSC-5601 codesets
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
#include "cv_ksc.h"
#include "ld_proto.h"

/*
*	KSC-5601 -> unicode
*	% KSC-5601 is same to EUC cs1(codeset 1). Then converting 
*	KSC-5601 to EUC is not needed.
*/

USHORT CVKSC_ksc_to_unicode(CSCONVERT obj,
							UCS2_CHAR *dest_ptr,
							USHORT dest_len,
							UCHAR *ksc_str,
							USHORT ksc_len,
							SSHORT *err_code,
							USHORT *err_position)
{
	UCS2_CHAR *start;
	UCS2_CHAR ch;
	UCS2_CHAR wide;
	UCHAR c1, c2;
	USHORT src_start = ksc_len;
	USHORT this_len;

	assert(ksc_str != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVKSC_ksc_to_unicode);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

	if (dest_ptr == NULL)
		return (ksc_len * sizeof(UCS2_CHAR));

	start = dest_ptr;
	while (ksc_len && dest_len > 1) {
		if (*ksc_str & 0x80) {
			c1 = *ksc_str++;

			if (KSC1(c1)) {		/* first byte is KSC */
				if (ksc_len == 1) {
					*err_code = CS_BAD_INPUT;
					break;
				}
				c2 = *ksc_str++;
				if (!(KSC2(c2))) {	/* Bad second byte */
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

			wide = *ksc_str++;
			this_len = 1;
		}

		ch = ((USHORT *) obj->csconvert_datatable)
			[((USHORT *) obj->csconvert_misc)[(USHORT) wide / 256] +
			 (wide % 256)];

		if ((ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*dest_ptr++ = ch;
		dest_len -= sizeof(*dest_ptr);
		ksc_len -= this_len;
	}

	if (ksc_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;
	*err_position = src_start - ksc_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}


USHORT CVKSC_unicode_to_ksc(CSCONVERT obj,
							UCHAR *ksc_str,
							USHORT ksc_len,
							UCS2_CHAR *unicode_str,
							USHORT unicode_len,
							SSHORT *err_code,
							USHORT *err_position)
{
	UCHAR *start;
	UCS2_CHAR ksc_ch;
	UCS2_CHAR wide;
	int tmp1, tmp2;
	USHORT src_start = unicode_len;

	assert(unicode_str != NULL || ksc_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CVKSC_unicode_to_ksc);
	assert(obj->csconvert_datatable != NULL);
	assert(obj->csconvert_misc != NULL);

	*err_code = 0;

	if (ksc_str == NULL)
		return (unicode_len);

	start = ksc_str;
	while (ksc_len && unicode_len > 1) {
		wide = *unicode_str++;

		ksc_ch = ((USHORT *) obj->csconvert_datatable)
				[((USHORT *) obj->csconvert_misc)
					[wide /	256] + (wide % 256)];
		if ((ksc_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		tmp1 = ksc_ch / 256;
		tmp2 = ksc_ch % 256;
		if (tmp1 == 0) {		/* ASCII */
			*ksc_str++ = tmp2;
			ksc_len--;
			unicode_len -= sizeof(*unicode_str);
			continue;
		}
		if (ksc_len < 2) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		else {
			assert(KSC1(tmp1));
			assert(KSC2(tmp2));
			*ksc_str++ = tmp1;
			*ksc_str++ = tmp2;
			unicode_len -= sizeof(*unicode_str);
			ksc_len -= 2;
		}
	}							/* end-while */
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((ksc_str - start) * sizeof(*ksc_str));
}


USHORT CVKSC_check_ksc(UCHAR *ksc_str,
					   USHORT ksc_len)
{
	UCHAR c1;

	while (ksc_len--) {
		c1 = *ksc_str;
		if (KSC1(c1)) {			/* Is it KSC-5601 ? */
			if (ksc_len == 0)	/* truncated KSC */
				return (1);
			else {
				ksc_str += 2;
				ksc_len -= 1;
			}
		}
		else if (c1 > 0x7f)		/* error */
			return 1;
		else					/* ASCII */
			ksc_str++;
	}
	return (0);
}


USHORT CVKSC_ksc_byte2short(CSCONVERT obj,
							USHORT *dst,
							USHORT dst_len,
							UCHAR *src,
							USHORT src_len,
							SSHORT *err_code,
							USHORT *err_position)
{
	USHORT x;
	USHORT *dst_start;
	USHORT src_start = src_len;

	assert(obj != NULL);
	assert(src != NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);

#ifdef DEBUG
	ib_printf("dst_len = %d, src_len = %d\n src = %s\n", dst_len, src_len,
			  src);
#endif

	*err_code = 0;
	if (dst == NULL)
		return (2 * src_len);

	dst_start = dst;
	while (src_len && dst_len > 1) {
		if (KSC1(*src)) {		/* KSC ? */
			if (src_len < 2) {
				*err_code = CS_BAD_INPUT;
				break;
			}
			x = (*src << 8) + *(src + 1);
			src += 2;
			src_len -= 2;
		}
		else if (*src > 0x7f) {	/* error */
			*err_code = CS_BAD_INPUT;
			break;
		}
		else {					/* ASCII */

			x = *src++;
			src_len -= 1;
		}
		*dst = x;
		dst++;
		dst_len -= 2;
	}
	if (src_len && !*err_code)
		*err_code = CS_TRUNCATION_ERROR;

#ifdef DEBUG
	ib_printf("dest len = %d %d characters\n",
			  (dst - dst_start) * sizeof(*dst), dst - dst_start);
	dump_short_hex(dst_start, (dst - dst_start) * sizeof(*dst));
#endif

	*err_position = src_start - src_len;
	return ((dst - dst_start) * sizeof(*dst));
}


short CVKSC_ksc_mbtowc(CSCONVERT obj,
					   UCS2_CHAR *wc,
					   UCHAR *src, 
					   USHORT src_len)
{
	assert(src != NULL);
	assert(obj != NULL);

	if (!src_len)
		return -1;

	if (KSC1(*src)) {			/* KSC */
		if (src_len < 2) {
			return -1;
		}
		if (wc)
			*wc = (*src << 8) + *(src + 1);
		return 2;
	}
	else if (*src > 0x7f)		/* error */
		return -1;
	else {						/* ASCII */

		if (wc)
			*wc = *src++;
		return 1;
	}
}
