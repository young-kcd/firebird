/*
 *      PROGRAM:        InterBase International support
 *      MODULE:         cs_jis.c
 *      DESCRIPTION:    Character Set definitions for JIS family.
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


#include "../intl/ldcommon.h"

extern USHORT CV_wc_to_wc();
extern USHORT CVJIS_sjis_to_unicode();
extern USHORT CVJIS_unicode_to_sjis();
extern USHORT CVJIS_eucj_to_unicode();
extern USHORT CVJIS_unicode_to_eucj();



CHARSET_ENTRY(CS_jis_0208_1990)
{
	static CONST WCHAR space = 0x0020;

#include "../intl/cs_jis_0208_1990.h"

	csptr->charset_version = 40;
	csptr->charset_id = CS_JIS_0208;
	csptr->charset_name = (ASCII *) "JIS_0208_1990";
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 2;
	csptr->charset_max_bytes_per_char = 2;
	csptr->charset_space_length = 2;
	csptr->charset_space_character = (BYTE *) & space;	/* 0x20 */
	csptr->charset_well_formed = (FPTR_SHORT) NULL;

	CV_convert_init(&csptr->charset_to_unicode, CS_UNICODE101, CS_JIS_0208,
					CV_wc_to_wc, to_unicode_mapping_array, to_unicode_map);
	CV_convert_init(&csptr->charset_from_unicode, CS_JIS_0208, CS_UNICODE101,
					CV_wc_to_wc, from_unicode_mapping_array,
					from_unicode_map);
	CHARSET_RETURN;
}


CHARSET_ENTRY(CS_sjis)
{
	CS_jis_0208_1990(csptr, cs_id, dummy);
	csptr->charset_id = CS_SJIS;
	csptr->charset_name = (ASCII *) "SJIS";
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";	/* 0x20 */
	csptr->charset_to_unicode.csconvert_from = CS_SJIS;
	csptr->charset_to_unicode.csconvert_convert = CVJIS_sjis_to_unicode;
	csptr->charset_from_unicode.csconvert_to = CS_SJIS;
	csptr->charset_from_unicode.csconvert_convert = CVJIS_unicode_to_sjis;
	CHARSET_RETURN;
}


CHARSET_ENTRY(CS_euc_j)
{
	CS_jis_0208_1990(csptr, cs_id, dummy);
	csptr->charset_id = CS_EUCJ;
	csptr->charset_name = (ASCII *) "EUC-J";
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";	/* 0x20 */
	csptr->charset_to_unicode.csconvert_from = CS_EUCJ;
	csptr->charset_to_unicode.csconvert_convert = CVJIS_eucj_to_unicode;
	csptr->charset_from_unicode.csconvert_to = CS_EUCJ;
	csptr->charset_from_unicode.csconvert_convert = CVJIS_unicode_to_eucj;
	CHARSET_RETURN;
}
