/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_dos.c
 *	DESCRIPTION:	Language Drivers for compatibility with DOS products.
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
#include "lc_dos.h"
#include "lc_narrow.h"
#include "ld_proto.h"

static inline void FAMILY1(TEXTTYPE cache,
							TTYPE_ID id_number,
							pfn_INTL_init name,
							CHARSET_ID charset,
							SSHORT country,
							USHORT flags,
							const SortOrderTblEntry * NoCaseOrderTbl,
							const BYTE * ToUpperConversionTbl,
							const BYTE * ToLowerConversionTbl,
							const CompressPair * CompressTbl,
							const ExpandChar * ExpansionTbl,
							const ASCII *POSIX)
//#define FAMILY1(id_number, name, charset, country)
{
	cache->texttype_version			= IB_LANGDRV_VERSION;
	cache->texttype_type			= id_number;
	cache->texttype_character_set	= charset;
	cache->texttype_country			= country;
	cache->texttype_bytes_per_char	= 1;
	cache->texttype_fn_init			= (FPTR_SHORT) name;
	cache->texttype_fn_key_length	= (FPTR_SHORT) LC_NARROW_key_length;
	cache->texttype_fn_string_to_key= (FPTR_SHORT) LC_NARROW_string_to_key;
	cache->texttype_fn_compare		= (FPTR_short) LC_NARROW_compare;
	cache->texttype_fn_to_upper		= (FPTR_SHORT) fam1_to_upper;
	cache->texttype_fn_to_lower		= (FPTR_SHORT) fam1_to_lower;
	cache->texttype_fn_str_to_upper = (FPTR_short) fam1_str_to_upper;
	cache->texttype_fn_mbtowc		= (FPTR_short) LC_DOS_nc_mbtowc;
	cache->texttype_collation_table = (BYTE *) NoCaseOrderTbl;
	cache->texttype_toupper_table	= (BYTE *) ToUpperConversionTbl;
	cache->texttype_tolower_table	= (BYTE *) ToLowerConversionTbl;
	cache->texttype_compress_table	= (BYTE *) CompressTbl;
	cache->texttype_expand_table	= (BYTE *) ExpansionTbl;
	cache->texttype_name			= POSIX;
	cache->texttype_flags			|= ((flags) & REVERSE) ?
									(TEXTTYPE_reverse_secondary | TEXTTYPE_ignore_specials) : 0;
}



TEXTTYPE_ENTRY(DOS102_init)
{
	static const ASCII POSIX[] = "INTL.DOS437";

#include "../intl/collations/pd437intl.h"

	FAMILY1(cache, parm1, DOS102_init, CS_DOS_437, CC_INTL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);
	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS105_init)
{
	static const ASCII POSIX[] = "NORDAN4.DOS437";

#include "../intl/collations/pd865nordan40.h"

	FAMILY1(cache, parm1, DOS105_init, CS_DOS_865, CC_NORDAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS106_init)
{
	static const ASCII POSIX[] = "SWEDFIN.DOS437";

#include "../intl/collations/pd437swedfin.h"

	FAMILY1(cache, parm1, DOS106_init, CS_DOS_437, CC_SWEDFIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c2_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437de0.h"

	FAMILY1(cache, parm1, DOS101_c2_init, CS_DOS_437, CC_GERMANY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c3_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437es1.h"

	FAMILY1(cache, parm1, DOS101_c3_init, CS_DOS_437, CC_SPAIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c4_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437fi0.h"

	FAMILY1(cache, parm1, DOS101_c4_init, CS_DOS_437, CC_FINLAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c5_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437fr0.h"

	FAMILY1(cache, parm1, DOS101_c5_init, CS_DOS_437, CC_FRANCE, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c6_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437it0.h"

	FAMILY1(cache, parm1, DOS101_c6_init, CS_DOS_437, CC_ITALY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c7_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437nl0.h"

	FAMILY1(cache, parm1, DOS101_c7_init, CS_DOS_437, CC_NEDERLANDS, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c8_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437sv0.h"

	FAMILY1(cache, parm1, DOS101_c8_init, CS_DOS_437, CC_SWEDEN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c9_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437uk0.h"

	FAMILY1(cache, parm1, DOS101_c9_init, CS_DOS_437, CC_UK, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS101_c10_init)
{
	static const ASCII POSIX[] = "DBASE.DOS437";

#include "../intl/collations/db437us0.h"

	FAMILY1(cache, parm1, DOS101_c10_init, CS_DOS_437, CC_US, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c1_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850cf0.h"

	FAMILY1(cache, parm1, DOS160_c1_init, CS_DOS_850, CC_FRENCHCAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c2_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850de0.h"

	FAMILY1(cache, parm1, DOS160_c2_init, CS_DOS_850, CC_GERMANY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c3_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850es0.h"

	FAMILY1(cache, parm1, DOS160_c3_init, CS_DOS_850, CC_SPAIN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c4_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850fr0.h"

	FAMILY1(cache, parm1, DOS160_c4_init, CS_DOS_850, CC_FRANCE, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c5_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850it1.h"

	FAMILY1(cache, parm1, DOS160_c5_init, CS_DOS_850, CC_ITALY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c6_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850nl0.h"

	FAMILY1(cache, parm1, DOS160_c6_init, CS_DOS_850, CC_NEDERLANDS, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c7_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850pt0.h"

	FAMILY1(cache, parm1, DOS160_c7_init, CS_DOS_850, CC_PORTUGAL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c8_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850sv1.h"

	FAMILY1(cache, parm1, DOS160_c8_init, CS_DOS_850, CC_SWEDEN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c9_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850uk0.h"

	FAMILY1(cache, parm1, DOS160_c9_init, CS_DOS_850, CC_UK, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS160_c10_init)
{
	static const ASCII POSIX[] = "DBASE.DOS850";

#include "../intl/collations/db850us0.h"

	FAMILY1(cache, parm1, DOS160_c10_init, CS_DOS_850, CC_US, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS107_c1_init)
{
	static const ASCII POSIX[] = "PDOX.DOS865";

#include "../intl/collations/pd865nordan40.h"

	FAMILY1(cache, parm1, DOS107_c1_init, CS_DOS_865, CC_NORDAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS107_c2_init)
{
	static const ASCII POSIX[] = "DBASE.DOS865";

#include "../intl/collations/db865da0.h"

	FAMILY1(cache, parm1, DOS107_c2_init, CS_DOS_865, CC_DENMARK, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS107_c3_init)
{
	static const ASCII POSIX[] = "DBASE.DOS865";

#include "../intl/collations/db865no0.h"

	FAMILY1(cache, parm1, DOS107_c3_init, CS_DOS_865, CC_NORWAY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c1_init)
{
	static const ASCII POSIX[] = "DBASE.DOS852";

#include "../intl/collations/db852cz0.h"

	FAMILY1(cache, parm1, DOS852_c1_init, CS_DOS_852, CC_CZECH, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c2_init)
{
	static const ASCII POSIX[] = "DBASE.DOS852";

#include "../intl/collations/db852po0.h"

	FAMILY1(cache, parm1, DOS852_c2_init, CS_DOS_852, CC_POLAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c4_init)
{
	static const ASCII POSIX[] = "DBASE.DOS852";

#include "../intl/collations/db852sl0.h"

	FAMILY1(cache, parm1, DOS852_c4_init, CS_DOS_852, CC_YUGOSLAVIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c5_init)
{
	static const ASCII POSIX[] = "PDOX.DOS852";

#include "../intl/collations/pd852czech.h"

	FAMILY1(cache, parm1, DOS852_c5_init, CS_DOS_852, CC_CZECH, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c6_init)
{
	static const ASCII POSIX[] = "PDOX.DOS852";

#include "../intl/collations/pd852polish.h"

	FAMILY1(cache, parm1, DOS852_c6_init, CS_DOS_852, CC_POLAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c7_init)
{
	static const ASCII POSIX[] = "PDOX.DOS852";

#include "../intl/collations/pd852hundc.h"

	FAMILY1(cache, parm1, DOS852_c7_init, CS_DOS_852, CC_HUNGARY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS852_c8_init)
{
	static const ASCII POSIX[] = "PDOX.DOS852";

#include "../intl/collations/pd852slovene.h"

	FAMILY1(cache, parm1, DOS852_c8_init, CS_DOS_852, CC_YUGOSLAVIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS857_c1_init)
{
	static const ASCII POSIX[] = "DBASE.DOS857";

#include "../intl/collations/db857tr0.h"

	FAMILY1(cache, parm1, DOS857_c1_init, CS_DOS_857, CC_TURKEY, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS860_c1_init)
{
	static const ASCII POSIX[] = "DBASE.DOS860";

#include "../intl/collations/db860pt0.h"

	FAMILY1(cache, parm1, DOS860_c1_init, CS_DOS_860, CC_PORTUGAL, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS861_c1_init)
{
	static const ASCII POSIX[] = "PDOX.DOS861";

#include "../intl/collations/pd861iceland.h"

	FAMILY1(cache, parm1, DOS861_c1_init, CS_DOS_861, CC_ICELAND, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(DOS863_c1_init)
{
	static const ASCII POSIX[] = "DBASE.DOS863";

#include "../intl/collations/db863cf1.h"

	FAMILY1(cache, parm1, DOS863_c1_init, CS_DOS_863, CC_FRENCHCAN, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(CYRL_c1_init)
{
	static const ASCII POSIX[] = "DBASE.CYRL";

#include "../intl/collations/db866ru0.h"

	FAMILY1(cache, parm1, CYRL_c1_init, CS_CYRL, CC_RUSSIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


TEXTTYPE_ENTRY(CYRL_c2_init)
{
	static const ASCII POSIX[] = "PDOX.CYRL";

#include "../intl/collations/pd866cyrr.h"

	FAMILY1(cache, parm1, CYRL_c2_init, CS_CYRL, CC_RUSSIA, LDRV_TIEBREAK,
			NoCaseOrderTbl, ToUpperConversionTbl, ToLowerConversionTbl,
			CompressTbl, ExpansionTbl, POSIX);

	TEXTTYPE_RETURN;
}


/*
 * Generic base for InterBase 4.0 Language Driver - family1
 *	Paradox DOS/Windows compatible
 */


#define	LOCALE_UPPER(ch)	(obj->texttype_toupper_table [(unsigned)(ch)])
#define	LOCALE_LOWER(ch)	(obj->texttype_tolower_table [(unsigned)(ch)])




USHORT fam1_to_upper(TEXTTYPE obj, BYTE ch)
{
	return (LOCALE_UPPER(ch));
}




/*
 *	Returns -1 if output buffer was too small
 */
SSHORT fam1_str_to_upper(TEXTTYPE obj, USHORT iLen, BYTE *pStr, USHORT iOutLen, BYTE *pOutStr)
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



USHORT fam1_to_lower(TEXTTYPE obj, BYTE ch)
{
	return (LOCALE_LOWER(ch));
}


SSHORT LC_DOS_nc_mbtowc(TEXTTYPE obj, UCS2_CHAR *wc, UCHAR *ptr, USHORT count)
{
/**************************************
 *
 *	L C _ D O S _ n c _ m b t o w c 
 *
 **************************************
 *
 * Functional description
 *	Get the next character from the multibyte
 *	input stream.
 *	Narrow character version.
 *  Returns:
 *	Count of bytes consumed from the input stream.
 *
 **************************************/

	assert(obj);
	assert(ptr);

	if (count >= 1) {
		if (wc)
			*wc = *ptr;
		return 1;
	}
	if (wc)
		*wc = 0;
	return -1;					/* No more characters */
}


#undef LANGFAM1_MAX_KEY

#undef ASCII_SPACE
#undef NULL_WEIGHT
#undef NULL_SECONDARY
#undef NULL_TERTIARY

#undef LOCALE_UPPER
#undef LOCALE_LOWER
