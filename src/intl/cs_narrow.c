/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cs_narrow.c
 *	DESCRIPTION:	Character set definitions for narrow character sets.
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

extern USHORT CV_nc_to_unicode();
extern USHORT CV_unicode_to_nc();



static void common_8bit_init(csptr, id, name, to_unicode_tbl,
							 from_unicode_tbl1, from_unicode_tbl2)
	 CHARSET csptr;
	 SSHORT id;
	 ASCII *name;
	 BYTE *to_unicode_tbl;
	 BYTE *from_unicode_tbl1;
	 BYTE *from_unicode_tbl2;
{
	csptr->charset_version = 40;
	csptr->charset_id = id;
	csptr->charset_name = name;
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";
	csptr->charset_well_formed = (FPTR_SHORT) NULL;
	CV_convert_init(&csptr->charset_to_unicode, CS_UNICODE101, id,
					CV_nc_to_unicode, to_unicode_tbl, NULL);
	CV_convert_init(&csptr->charset_from_unicode, id, CS_UNICODE101,
					CV_unicode_to_nc, from_unicode_tbl1, from_unicode_tbl2);
}


CHARSET_ENTRY(CS_iso_latin1)
{
#include "../intl/cs_latin1.h"

	common_8bit_init(csptr, CS_LATIN1, "LATIN1", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_437)
{
#include "../intl/cs_437.h"

	common_8bit_init(csptr, CS_DOS_437, "DOS437", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_865)
{
#include "../intl/cs_865.h"

	common_8bit_init(csptr, CS_DOS_865, "DOS865", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_850)
{
#include "../intl/cs_850.h"

	common_8bit_init(csptr, CS_DOS_850, "DOS850", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_852)
{
#include "../intl/cs_852.h"

	common_8bit_init(csptr, CS_DOS_852, "DOS852", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_857)
{
#include "../intl/cs_857.h"

	common_8bit_init(csptr, CS_DOS_857, "DOS857", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_860)
{
#include "../intl/cs_860.h"

	common_8bit_init(csptr, CS_DOS_860, "DOS860", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_861)
{
#include "../intl/cs_861.h"

	common_8bit_init(csptr, CS_DOS_861, "DOS861", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_863)
{
#include "../intl/cs_863.h"

	common_8bit_init(csptr, CS_DOS_863, "DOS863", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_cyrl)
{
#include "../intl/cs_cyrl.h"

	common_8bit_init(csptr, CS_CYRL, "CYRL", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1250)
{
#include "../intl/cs_w1250.h"

	common_8bit_init(csptr, CS_WIN1250, "WIN1250", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1251)
{
#include "../intl/cs_w1251.h"

	common_8bit_init(csptr, CS_WIN1251, "WIN1251", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1252)
{
#include "../intl/cs_w1252.h"

	common_8bit_init(csptr, CS_WIN1252, "WIN1252", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1253)
{
#include "../intl/cs_w1253.h"

	common_8bit_init(csptr, CS_WIN1253, "WIN1253", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1254)
{
#include "../intl/cs_w1254.h"

	common_8bit_init(csptr, CS_WIN1254, "WIN1254", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_next)
{
#include "../intl/cs_next.h"

	common_8bit_init(csptr, CS_NEXT, "NEXT", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}
