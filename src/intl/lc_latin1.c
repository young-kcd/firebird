/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_latin1.c
 *	DESCRIPTION:	Language Drivers in the Latin1 family.
 *			(full International collation)
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

extern USHORT LC_NARROW_key_length();
extern USHORT LC_NARROW_string_to_key();
extern SSHORT LC_NARROW_compare();
STATIC USHORT fam2_to_upper();
STATIC SSHORT fam2_str_to_upper();
STATIC USHORT fam2_to_lower();
extern SSHORT LC_DOS_nc_mbtowc();

#define FAMILY2(id_number, name, charset, country) \
	cache->texttype_version =		IB_LANGDRV_VERSION; \
	cache->texttype_type =			(id_number); \
	cache->texttype_character_set =		(charset); \
	cache->texttype_country =		(country); \
	cache->texttype_bytes_per_char =	1; \
	cache->texttype_fn_init =		(name); \
	cache->texttype_fn_key_length =		LC_NARROW_key_length; \
	cache->texttype_fn_string_to_key =	LC_NARROW_string_to_key; \
	cache->texttype_fn_compare =		LC_NARROW_compare; \
	cache->texttype_fn_to_upper =		fam2_to_upper; \
	cache->texttype_fn_to_lower =		fam2_to_lower; \
	cache->texttype_fn_str_to_upper =	fam2_str_to_upper; \
	cache->texttype_fn_mbtowc =		LC_DOS_nc_mbtowc; \
	cache->texttype_collation_table =	(BYTE *) NoCaseOrderTbl; \
	cache->texttype_toupper_table =		(BYTE *) ToUpperConversionTbl; \
	cache->texttype_tolower_table =		(BYTE *) ToLowerConversionTbl; \
	cache->texttype_compress_table =	(BYTE *) CompressTbl; \
	cache->texttype_expand_table =		(BYTE *) ExpansionTbl; \
	cache->texttype_name =			POSIX; \
    cache->texttype_flags |= ((LDRV_TIEBREAK) & REVERSE) ? \
            TEXTTYPE_reverse_secondary : 0;



TEXTTYPE_ENTRY(LAT139_init)
{
	static CONST ASCII POSIX[] = "da_DA.ISO8859_1";

#include "../intl/bllt1da0.h"

	FAMILY2(parm1, LAT139_init, CS_LATIN1, CC_DENMARK);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"



TEXTTYPE_ENTRY(LAT140_init)
{
	static CONST ASCII POSIX[] = "du_NL.ISO8859_1";

#include "../intl/bllt1nl0.h"

	FAMILY2(parm1, LAT140_init, CS_LATIN1, CC_NEDERLANDS);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"



TEXTTYPE_ENTRY(LAT141_init)
{
	static CONST ASCII POSIX[] = "fi_FI.ISO8859_1";

#include "../intl/bllt1fi0.h"

	FAMILY2(parm1, LAT141_init, CS_LATIN1, CC_FINLAND);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT142_init)
{
	static CONST ASCII POSIX[] = "fr_FR.ISO8859_1";

#include "../intl/bllt1fr0.h"

	FAMILY2(parm1, LAT142_init, CS_LATIN1, CC_FRANCE);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT143_init)
{
	static CONST ASCII POSIX[] = "fr_CA.ISO8859_1";

#include "../intl/bllt1ca0.h"

	FAMILY2(parm1, LAT143_init, CS_LATIN1, CC_FRENCHCAN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT144_init)
{
	static CONST ASCII POSIX[] = "de_DE.ISO8859_1";

#include "../intl/bllt1de0.h"

	FAMILY2(parm1, LAT144_init, CS_LATIN1, CC_GERMANY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT145_init)
{
	static CONST ASCII POSIX[] = "is_IS.ISO8859_1";

#include "../intl/bllt1is0.h"

	FAMILY2(parm1, LAT145_init, CS_LATIN1, CC_ICELAND);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT146_init)
{
	static CONST ASCII POSIX[] = "it_IT.ISO8859_1";

#include "../intl/bllt1it0.h"

	FAMILY2(parm1, LAT146_init, CS_LATIN1, CC_ITALY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"



TEXTTYPE_ENTRY(LAT148_init)
{
	static CONST ASCII POSIX[] = "no_NO.ISO8859_1";

#include "../intl/bllt1no0.h"

	FAMILY2(parm1, LAT148_init, CS_LATIN1, CC_NORWAY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"



TEXTTYPE_ENTRY(LAT149_init)
{
	static CONST ASCII POSIX[] = "es_ES.ISO8859_1";

#include "../intl/bllt1es0.h"

	FAMILY2(parm1, LAT149_init, CS_LATIN1, CC_SPAIN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT151_init)
{
	static CONST ASCII POSIX[] = "sv_SV.ISO8859_1";

#include "../intl/bllt1sv0.h"

	FAMILY2(parm1, LAT151_init, CS_LATIN1, CC_SWEDEN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT152_init)
{
	static CONST ASCII POSIX[] = "en_UK.ISO8859_1";

#include "../intl/bllt1uk0.h"

	FAMILY2(parm1, LAT152_init, CS_LATIN1, CC_UK);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT153_init)
{
	static CONST ASCII POSIX[] = "en_US.ISO8859_1";

#include "../intl/bllt1us0.h"

	FAMILY2(parm1, LAT153_init, CS_LATIN1, CC_US);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(LAT154_init)
{
	static CONST ASCII POSIX[] = "pt_PT.ISO8859_1";

#include "../intl/bllt1pt0.h"

	FAMILY2(parm1, LAT154_init, CS_LATIN1, CC_PORTUGAL);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1250_c1_init)
{
	static CONST ASCII POSIX[] = "PXW_CSY.WIN1250";

#include "../intl/anczech.h"

	FAMILY2(parm1, WIN1250_c1_init, CS_WIN1250, CC_CZECH);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1250_c2_init)
{
	static CONST ASCII POSIX[] = "PXW_HUNDC.WIN1250";

#include "../intl/anhundc.h"

	FAMILY2(parm1, WIN1250_c2_init, CS_WIN1250, CC_HUNGARY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1250_c3_init)
{
	static CONST ASCII POSIX[] = "PXW_PLK.WIN1250";

#include "../intl/anpolish.h"

	FAMILY2(parm1, WIN1250_c3_init, CS_WIN1250, CC_POLAND);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1250_c4_init)
{
	static CONST ASCII POSIX[] = "PXW_SLOV.WIN1250";

#include "../intl/ansislov.h"

	FAMILY2(parm1, WIN1250_c4_init, CS_WIN1250, CC_YUGOSLAVIA);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1251_c1_init)
{
	static CONST ASCII POSIX[] = "PXW_CYRL.WIN1251";

#include "../intl/ancyrr.h"

	FAMILY2(parm1, WIN1251_c1_init, CS_WIN1251, CC_RUSSIA);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c1_init)
{
	static CONST ASCII POSIX[] = "PXW_INTL.WIN1252";

#include "../intl/ansiintl.h"

	FAMILY2(parm1, WIN1252_c1_init, CS_WIN1252, CC_INTL);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c2_init)
{
	static CONST ASCII POSIX[] = "PXW_INTL850.WIN1252";

#include "../intl/ansii850.h"

	FAMILY2(parm1, WIN1252_c2_init, CS_WIN1252, CC_INTL);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c3_init)
{
	static CONST ASCII POSIX[] = "PXW_NORDAN4.WIN1252";

#include "../intl/ansinor4.h"

	FAMILY2(parm1, WIN1252_c3_init, CS_WIN1252, CC_NORDAN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c4_init)
{
	static CONST ASCII POSIX[] = "PXW_SPAN.WIN1252";

#include "../intl/ansispan.h"

	FAMILY2(parm1, WIN1252_c4_init, CS_WIN1252, CC_SPAIN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1252_c5_init)
{
	static CONST ASCII POSIX[] = "PXW_SWEDFIN.WIN1252";

#include "../intl/ansiswfn.h"

	FAMILY2(parm1, WIN1252_c5_init, CS_WIN1252, CC_SWEDFIN);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1253_c1_init)
{
	static CONST ASCII POSIX[] = "PXW_GREEK.WIN1253";

#include "../intl/angreek1.h"

	FAMILY2(parm1, WIN1253_c1_init, CS_WIN1253, CC_GREECE);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(WIN1254_c1_init)
{
	static CONST ASCII POSIX[] = "PXW_TURK.WIN1254";

#include "../intl/anturk.h"

	FAMILY2(parm1, WIN1254_c1_init, CS_WIN1254, CC_TURKEY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(NEXT_c1_init)
{
	static CONST ASCII POSIX[] = "NXT_US.NEXT";

#include "../intl/blnxtus0.h"

	FAMILY2(parm1, NEXT_c1_init, CS_NEXT, CC_US);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(NEXT_c2_init)
{
	static CONST ASCII POSIX[] = "NXT_DEU.NEXT";

#include "../intl/blnxtde0.h"

	FAMILY2(parm1, NEXT_c2_init, CS_NEXT, CC_GERMANY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(NEXT_c3_init)
{
	static CONST ASCII POSIX[] = "NXT_FRA.NEXT";

#include "../intl/blnxtfr0.h"

	FAMILY2(parm1, NEXT_c3_init, CS_NEXT, CC_FRANCE);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"


TEXTTYPE_ENTRY(NEXT_c4_init)
{
	static CONST ASCII POSIX[] = "NXT_ITA.NEXT";

#include "../intl/blnxtit0.h"

	FAMILY2(parm1, NEXT_c4_init, CS_NEXT, CC_ITALY);

	TEXTTYPE_RETURN;
}

#include "../intl/undef.h"

#undef FAMILY2
#undef NULL_SECONDARY
#undef NULL_TERTIARY



/*
 * Generic base for InterBase 4.0 Language Driver
 */

#define	LOCALE_UPPER(ch)	(obj->texttype_toupper_table[ (unsigned) (ch) ])
#define	LOCALE_LOWER(ch)	(obj->texttype_tolower_table[ (unsigned) (ch) ])




STATIC USHORT fam2_to_upper(obj, ch)
	 TEXTTYPE obj;
	 BYTE ch;
{
	return (LOCALE_UPPER(ch));
}


/*
 *	Returns -1 if output buffer was too small
 */
STATIC SSHORT fam2_str_to_upper(obj, iLen, pStr, iOutLen, pOutStr)
	 TEXTTYPE obj;
	 USHORT iLen;
	 BYTE *pStr;
	 USHORT iOutLen;
	 BYTE *pOutStr;
{
	BYTE *p;
	assert(pStr != NULL);
	assert(pOutStr != NULL);
	assert(iLen <= 32000);		/* almost certainly an error */
	assert(iOutLen <= 32000);	/* almost certainly an error */
	assert(iOutLen >= iLen);
	p = pOutStr;
	while (iLen && iOutLen) {
		*pOutStr++ = LOCALE_UPPER(*pStr);
		pStr++;
		iLen--;
		iOutLen--;
	};
	if (iLen != 0)
		return (-1);
	return (pOutStr - p);
}




STATIC USHORT fam2_to_lower(obj, ch)
	 TEXTTYPE obj;
	 BYTE ch;
{
	return (LOCALE_LOWER(ch));
}





#undef LOCALE_UPPER
#undef LOCALE_LOWER
