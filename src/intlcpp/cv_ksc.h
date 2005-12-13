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

/*
*	KSC-5601 -> unicode
*	% KSC-5601 is same to EUC cs1(codeset 1). Then converting 
*	KSC-5601 to EUC is not needed.
*/

/* These macros have a duplicate in lc_ksc.c */
#define	KSC1(uc)	((uc) & 0x80)
#define	KSC2(uc)	((uc) >= 0x41)

#define	GEN_HAN(b1, b2)	((0xb0 <= (b1) && (b1) <= 0xc8) && (0xa1 <= (b2) && (b2) <= 0xfe))

#define	SPE_HAN(b1, b2)	(((b1) == 0xa4) && (((b2) == 0xa2) || ((b2) == 0xa4) || ((b2) == 0xa7) || ((b2) == 0xa8) || ((b2) == 0xa9) || ((b2) == 0xb1) || ((b2) == 0xb2) || ((b2) == 0xb3) || ((b2) == 0xb5) || ((b2) == 0xb6) || ((b2) == 0xb7) || ((b2) == 0xb8) || ((b2) == 0xb9) || ((b2) == 0xba) || ((b2) == 0xbb) || ((b2) == 0xbc) || ((b2) == 0xbd) || ((b2) == 0xbe)))

#define	LANGKSC_MAX_KEY	256

USHORT CVKSC_ksc_to_unicode(CSCONVERT obj, USHORT *dest_ptr, USHORT dest_len, UCHAR *ksc_str, USHORT ksc_len,
							SSHORT *err_code, USHORT *err_position);

USHORT CVKSC_unicode_to_ksc(CSCONVERT obj, UCHAR *ksc_str, USHORT ksc_len, USHORT *unicode_str
							, USHORT unicode_len, SSHORT *err_code, USHORT *err_position);

USHORT CVKSC_check_ksc(UCHAR *ksc_str, USHORT ksc_len);

USHORT CVKSC_ksc_byte2short(CSCONVERT obj, USHORT *dst, USHORT dst_len, UCHAR *src, USHORT src_len
							, SSHORT *err_code,	USHORT *err_position);

short CVKSC_ksc_mbtowc(CSCONVERT obj, UCS2_CHAR *wc, UCHAR *src, USHORT src_len);
