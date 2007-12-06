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

#include "../intlcpp/ldcommon.h"

/* These macros have a duplicate in lc_big5.c */
#define	BIG51(uc)	((UCHAR)((uc)&0xff)>=0xa1 && \
			 (UCHAR)((uc)&0xff)<=0xfe)	/* BIG-5 1st-byte */
#define	BIG52(uc)	((UCHAR)((uc)&0xff)>=0x40 && \
			 (UCHAR)((uc)&0xff)<=0xfe)	/* BIG-5 2nd-byte */

USHORT CVBIG5_big5_to_unicode(CSCONVERT obj, USHORT *dest_ptr, USHORT dest_len, UCHAR *src_ptr
							, USHORT src_len, SSHORT *err_code, USHORT *err_position);
USHORT CVBIG5_unicode_to_big5(CSCONVERT obj, UCHAR *big5_str, USHORT big5_len, USHORT *unicode_str,
							  USHORT unicode_len, SSHORT *err_code, USHORT *err_position);
USHORT CVBIG5_check_big5(UCHAR *big5_str, USHORT big5_len);

USHORT CVBIG5_big5_byte2short(CSCONVERT obj, UCHAR *dst, USHORT dst_len, UCHAR *src, USHORT src_len
							, SSHORT *err_code, USHORT *err_position);

SSHORT CVBIG5_big5_mbtowc(CSCONVERT obj, UCS2_CHAR *wc, UCHAR *src, USHORT src_len);

