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

#include "firebird.h"
#include "../intl/ldcommon.h"
#include "cv_narrow.h"
#include "cv_unicode_fss.h"

CHARSET_ENTRY(CS_unicode_fss)
{
	csptr->charset_version = 40;
	csptr->charset_id = CS_UNICODE_FSS;
	csptr->charset_name = (ASCII *) "UNICODE_FSS";
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 3;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";	/* 0x20 */
	csptr->charset_well_formed = NULL;
	CV_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2, CS_UNICODE_FSS,
					(FPTR_SHORT) CS_UTFFSS_fss_to_unicode, NULL, NULL);
	CV_convert_init(&csptr->charset_from_unicode, CS_UNICODE_FSS, CS_UNICODE_UCS2,
					(FPTR_SHORT) CS_UTFFSS_unicode_to_fss, NULL, NULL);
	CHARSET_RETURN;
}
