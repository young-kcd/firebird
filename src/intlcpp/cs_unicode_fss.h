/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cs_utffss.c
 *	DESCRIPTION:	Character set definition for Unicode FSS format
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

 /* Note: all routines have cousins in jrd/intl.c */

USHORT CS_UTFFSS_unicode_to_fss(CSCONVERT obj, MBCHAR *fss_str, USHORT fss_len, UNICODE *unicode_str,
								USHORT unicode_len, SSHORT *err_code, USHORT *err_position);

USHORT CS_UTFFSS_fss_to_unicode(CSCONVERT obj, UNICODE *dest_ptr, USHORT dest_len, NCHAR *src_ptr
					, USHORT src_len,	SSHORT *err_code, USHORT *err_position);
SSHORT CS_UTFFSS_fss_mbtowc(TEXTTYPE *obj, UCS2_CHAR *wc, NCHAR *p, USHORT n);
