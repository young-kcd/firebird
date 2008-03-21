/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cv_ksc.cpp
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
#include "cv_narrow.h"
#include "cv_ksc.h"
#include "ld_proto.h"

/*
*	KSC-5601 -> unicode
*	% KSC-5601 is same to EUC cs1(codeset 1). Then converting 
*	KSC-5601 to EUC is not needed.
*/

ULONG CVKSC_ksc_to_unicode(csconvert* obj,
						   ULONG ksc_len,
						   const UCHAR* ksc_str,
						   ULONG dest_len,
						   USHORT *dest_ptr,
						   USHORT *err_code,
						   ULONG *err_position)
{
	fb_assert(ksc_str != NULL || dest_ptr == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == reinterpret_cast<pfn_INTL_convert>(CVKSC_ksc_to_unicode));
	fb_assert(obj->csconvert_impl->csconvert_datatable != NULL);
	fb_assert(obj->csconvert_impl->csconvert_misc != NULL);

	const ULONG src_start = ksc_len;
	*err_code = 0;

	if (dest_ptr == NULL)
		return (ksc_len * sizeof(USHORT));

	USHORT this_len;
	USHORT wide;
	const USHORT* const start = dest_ptr;
	while (ksc_len && dest_len > 1) {
		if (*ksc_str & 0x80) {
			const UCHAR c1 = *ksc_str++;

			if (KSC1(c1)) {		/* first byte is KSC */
				if (ksc_len == 1) {
					*err_code = CS_BAD_INPUT;
					break;
				}
				const UCHAR c2 = *ksc_str++;
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

		const USHORT ch = ((const USHORT*) obj->csconvert_impl->csconvert_datatable)
			[((const USHORT*) obj->csconvert_impl->csconvert_misc)[(USHORT) wide / 256] +
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


ULONG CVKSC_unicode_to_ksc(csconvert* obj,
						   ULONG unicode_len,
						   const USHORT* unicode_str,
						   ULONG ksc_len,
						   UCHAR *ksc_str,
						   USHORT *err_code,
						   ULONG *err_position)
{
	fb_assert(unicode_str != NULL || ksc_str == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_fn_convert == reinterpret_cast<pfn_INTL_convert>(CVKSC_unicode_to_ksc));
	fb_assert(obj->csconvert_impl->csconvert_datatable != NULL);
	fb_assert(obj->csconvert_impl->csconvert_misc != NULL);

	const ULONG src_start = unicode_len;
	*err_code = 0;

	if (ksc_str == NULL)
		return (unicode_len);

	const UCHAR* const start = ksc_str;
	while (ksc_len && unicode_len > 1) {
		const USHORT wide = *unicode_str++;

		const USHORT ksc_ch = ((const USHORT*) obj->csconvert_impl->csconvert_datatable)
				[((const USHORT*) obj->csconvert_impl->csconvert_misc)
					[wide /	256] + (wide % 256)];
		if ((ksc_ch == CS_CANT_MAP) && !(wide == CS_CANT_MAP)) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		const int tmp1 = ksc_ch / 256;
		const int tmp2 = ksc_ch % 256;
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
			fb_assert(KSC1(tmp1));
			fb_assert(KSC2(tmp2));
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


INTL_BOOL CVKSC_check_ksc(charset* cs,
						  ULONG ksc_len,
						  const UCHAR* ksc_str,
						  ULONG* offending_position)
{
	const UCHAR* ksc_str_start = ksc_str;

	while (ksc_len--) {
		const UCHAR c1 = *ksc_str;
		if (KSC1(c1)) {			/* Is it KSC-5601 ? */
			if (ksc_len == 0)	/* truncated KSC */
			{
				if (offending_position)
					*offending_position = ksc_str - ksc_str_start;
				return (false);
			}
			else {
				ksc_str += 2;
				ksc_len -= 1;
			}
		}
		else if (c1 > 0x7f)		/* error */
		{
			if (offending_position)
				*offending_position = ksc_str - ksc_str_start;
			return false;
		}
		else					/* ASCII */
			ksc_str++;
	}
	return (true);
}
