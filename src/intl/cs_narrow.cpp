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
#include "cv_narrow.h"

static void common_8bit_init(CHARSET csptr,
							 SSHORT id,
							 const ASCII *name,
							 const USHORT *to_unicode_tbl,
							 const UCHAR *from_unicode_tbl1,
							 const USHORT *from_unicode_tbl2)
{
	csptr->charset_version = 40;
	csptr->charset_id = id;
	csptr->charset_name = name;
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";
	csptr->charset_well_formed = NULL;
	CV_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2, id,
					(FPTR_SHORT) CV_nc_to_unicode, to_unicode_tbl, NULL);
	CV_convert_init(&csptr->charset_from_unicode, id, CS_UNICODE_UCS2,
					(FPTR_SHORT) CV_unicode_to_nc, from_unicode_tbl1, from_unicode_tbl2);
}


CHARSET_ENTRY(CS_iso_ISO8859_1)
{
#include "../intl/charsets/cs_iso8859_1.h"

	common_8bit_init(csptr, CS_ISO8859_1, "ISO88591", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_2)
{
#include "../intl/charsets/cs_iso8859_2.h"

    common_8bit_init (csptr, CS_ISO8859_2, "ISO88592", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_3)
{
#include "../intl/charsets/cs_iso8859_3.h"

    common_8bit_init (csptr, CS_ISO8859_3, "ISO88593", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_4)
{
#include "../intl/charsets/cs_iso8859_4.h"

    common_8bit_init (csptr, CS_ISO8859_4, "ISO88594", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_5)
{
#include "../intl/charsets/cs_iso8859_5.h"

    common_8bit_init (csptr, CS_ISO8859_5, "ISO88595", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_6)
{
#include "../intl/charsets/cs_iso8859_6.h"

    common_8bit_init (csptr, CS_ISO8859_6, "ISO88596", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_7)
{
#include "../intl/charsets/cs_iso8859_7.h"

    common_8bit_init (csptr, CS_ISO8859_7, "ISO88597", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_8)
{
#include "../intl/charsets/cs_iso8859_8.h"

    common_8bit_init (csptr, CS_ISO8859_8, "ISO88598", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_9)
{
#include "../intl/charsets/cs_iso8859_9.h"

    common_8bit_init (csptr, CS_ISO8859_9, "ISO88599", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY (CS_iso_ISO8859_13)
{
#include "../intl/charsets/cs_iso8859_13.h"

    common_8bit_init (csptr, CS_ISO8859_13, "ISO885913", to_unicode_map,
                      from_unicode_mapping_array, from_unicode_map);
    CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_437)
{
#include "../intl/charsets/cs_437.h"

	common_8bit_init(csptr, CS_DOS_437, "DOS437", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_865)
{
#include "../intl/charsets/cs_865.h"

	common_8bit_init(csptr, CS_DOS_865, "DOS865", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_850)
{
#include "../intl/charsets/cs_850.h"

	common_8bit_init(csptr, CS_DOS_850, "DOS850", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_852)
{
#include "../intl/charsets/cs_852.h"

	common_8bit_init(csptr, CS_DOS_852, "DOS852", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_857)
{
#include "../intl/charsets/cs_857.h"

	common_8bit_init(csptr, CS_DOS_857, "DOS857", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_860)
{
#include "../intl/charsets/cs_860.h"

	common_8bit_init(csptr, CS_DOS_860, "DOS860", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_861)
{
#include "../intl/charsets/cs_861.h"

	common_8bit_init(csptr, CS_DOS_861, "DOS861", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_863)
{
#include "../intl/charsets/cs_863.h"

	common_8bit_init(csptr, CS_DOS_863, "DOS863", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_737)
{
#include "../intl/charsets/cs_737.h"

	common_8bit_init(csptr, CS_DOS_737, "DOS737", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_775)
{
#include "../intl/charsets/cs_775.h"

	common_8bit_init(csptr, CS_DOS_775, "DOS775", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_858)
{
#include "../intl/charsets/cs_858.h"

	common_8bit_init(csptr, CS_DOS_858, "DOS858", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_862)
{
#include "../intl/charsets/cs_862.h"

	common_8bit_init(csptr, CS_DOS_862, "DOS862", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_864)
{
#include "../intl/charsets/cs_864.h"

	common_8bit_init(csptr, CS_DOS_864, "DOS864", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_866)
{
#include "../intl/charsets/cs_866.h"

	common_8bit_init(csptr, CS_DOS_866, "DOS866", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_dos_869)
{
#include "../intl/charsets/cs_869.h"

	common_8bit_init(csptr, CS_DOS_869, "DOS869", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_cyrl)
{
#include "../intl/charsets/cs_cyrl.h"

	common_8bit_init(csptr, CS_CYRL, "CYRL", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1250)
{
#include "../intl/charsets/cs_w1250.h"

	common_8bit_init(csptr, CS_WIN1250, "WIN1250", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1251)
{
#include "../intl/charsets/cs_w1251.h"

	common_8bit_init(csptr, CS_WIN1251, "WIN1251", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1252)
{
#include "../intl/charsets/cs_w1252.h"

	common_8bit_init(csptr, CS_WIN1252, "WIN1252", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1253)
{
#include "../intl/charsets/cs_w1253.h"

	common_8bit_init(csptr, CS_WIN1253, "WIN1253", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1254)
{
#include "../intl/charsets/cs_w1254.h"

	common_8bit_init(csptr, CS_WIN1254, "WIN1254", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1255)
{
#include "../intl/charsets/cs_w1255.h"

	common_8bit_init(csptr, CS_WIN1255, "WIN1255", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1256)
{
#include "../intl/charsets/cs_w1256.h"

	common_8bit_init(csptr, CS_WIN1256, "WIN1256", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_win1257)
{
#include "../intl/charsets/cs_w1257.h"

	common_8bit_init(csptr, CS_WIN1257, "WIN1257", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}

CHARSET_ENTRY(CS_next)
{
#include "../intl/charsets/cs_next.h"

	common_8bit_init(csptr, CS_NEXT, "NEXT", to_unicode_map,
					 from_unicode_mapping_array, from_unicode_map);
	CHARSET_RETURN;
}
