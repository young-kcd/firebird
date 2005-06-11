/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		ld.cpp
 *	DESCRIPTION:	Language Driver lookup & support routines
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
#include "../intl/ld_proto.h"

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h> /* for MAXPATHLEN */
#endif
#include <stdio.h>

/* Commented out to make Linux version work because it is inaccessiable on all 
  known platforms. Nickolay Samofatov, 10 Sept 2002
void gds__log(UCHAR*, ...);
*/

#define	EXTERN_texttype(name)	INTL_BOOL name (TEXTTYPE, const ASCII*, const ASCII*, USHORT, const UCHAR*, ULONG)
// #define EXTERN_convert(name)	INTL_BOOL name (csconvert*, const ASCII*, const ASCII*)
#define EXTERN_charset(name)	INTL_BOOL name (charset*, const ASCII*)

EXTERN_texttype(DOS101_init);
EXTERN_texttype(DOS101_c2_init);
EXTERN_texttype(DOS101_c3_init);
EXTERN_texttype(DOS101_c4_init);
EXTERN_texttype(DOS101_c5_init);
EXTERN_texttype(DOS101_c6_init);
EXTERN_texttype(DOS101_c7_init);
EXTERN_texttype(DOS101_c8_init);
EXTERN_texttype(DOS101_c9_init);
EXTERN_texttype(DOS101_c10_init);

EXTERN_texttype(DOS102_init);
EXTERN_texttype(DOS105_init);
EXTERN_texttype(DOS106_init);

EXTERN_texttype(DOS107_init);
EXTERN_texttype(DOS107_c1_init);
EXTERN_texttype(DOS107_c2_init);
EXTERN_texttype(DOS107_c3_init);

EXTERN_texttype(DOS852_c0_init);
EXTERN_texttype(DOS852_c1_init);
EXTERN_texttype(DOS852_c2_init);
EXTERN_texttype(DOS852_c4_init);
EXTERN_texttype(DOS852_c5_init);
EXTERN_texttype(DOS852_c6_init);
EXTERN_texttype(DOS852_c7_init);
EXTERN_texttype(DOS852_c8_init);

EXTERN_texttype(DOS857_c0_init);
EXTERN_texttype(DOS857_c1_init);

EXTERN_texttype(DOS860_c0_init);
EXTERN_texttype(DOS860_c1_init);

EXTERN_texttype(DOS861_c0_init);
EXTERN_texttype(DOS861_c1_init);

EXTERN_texttype(DOS863_c0_init);
EXTERN_texttype(DOS863_c1_init);

EXTERN_texttype(DOS737_c0_init);
EXTERN_texttype(DOS775_c0_init);
EXTERN_texttype(DOS858_c0_init);
EXTERN_texttype(DOS862_c0_init);
EXTERN_texttype(DOS864_c0_init);
EXTERN_texttype(DOS866_c0_init);
EXTERN_texttype(DOS869_c0_init);

EXTERN_texttype(CYRL_c0_init);
EXTERN_texttype(CYRL_c1_init);
EXTERN_texttype(CYRL_c2_init);

/* Latin 1 character set */
EXTERN_texttype(ISO88591_cp_init);

/* Latin 1 collations */
EXTERN_texttype(ISO88591_39_init);
EXTERN_texttype(ISO88591_40_init);
EXTERN_texttype(ISO88591_41_init);
EXTERN_texttype(ISO88591_42_init);
EXTERN_texttype(ISO88591_43_init);
EXTERN_texttype(ISO88591_44_init);
EXTERN_texttype(ISO88591_45_init);
EXTERN_texttype(ISO88591_46_init);
EXTERN_texttype(ISO88591_48_init);
EXTERN_texttype(ISO88591_49_init);
EXTERN_texttype(ISO88591_51_init);
EXTERN_texttype(ISO88591_52_init);
EXTERN_texttype(ISO88591_53_init);
EXTERN_texttype(ISO88591_54_init);
EXTERN_texttype(ISO88591_55_init);

/* Latin 2 character set */
EXTERN_texttype (ISO88592_cp_init);

/* latin 2 collations */
EXTERN_texttype (ISO88592_c1_init);
EXTERN_texttype (ISO88592_c2_init);

EXTERN_texttype (ISO88593_cp_init);
EXTERN_texttype (ISO88594_cp_init);
EXTERN_texttype (ISO88595_cp_init);
EXTERN_texttype (ISO88596_cp_init);
EXTERN_texttype (ISO88597_cp_init);
EXTERN_texttype (ISO88598_cp_init);
EXTERN_texttype (ISO88599_cp_init);
EXTERN_texttype (ISO885913_cp_init);

EXTERN_texttype(WIN1250_c0_init);
EXTERN_texttype(WIN1250_c1_init);
EXTERN_texttype(WIN1250_c2_init);
EXTERN_texttype(WIN1250_c3_init);
EXTERN_texttype(WIN1250_c4_init);
EXTERN_texttype(WIN1250_c5_init);

EXTERN_texttype(WIN1251_c0_init);
EXTERN_texttype(WIN1251_c1_init);
EXTERN_texttype(WIN1251_c2_init);

EXTERN_texttype(WIN1252_c0_init);
EXTERN_texttype(WIN1252_c1_init);
EXTERN_texttype(WIN1252_c2_init);
EXTERN_texttype(WIN1252_c3_init);
EXTERN_texttype(WIN1252_c4_init);
EXTERN_texttype(WIN1252_c5_init);
EXTERN_texttype(WIN1252_c6_init);
EXTERN_texttype(WIN1252_c7_init);

EXTERN_texttype(WIN1253_c0_init);
EXTERN_texttype(WIN1253_c1_init);

EXTERN_texttype(WIN1254_c0_init);
EXTERN_texttype(WIN1254_c1_init);

EXTERN_texttype(WIN1255_c0_init);
EXTERN_texttype(WIN1256_c0_init);
EXTERN_texttype(WIN1257_c0_init);

EXTERN_texttype(NEXT_c0_init);
EXTERN_texttype(NEXT_c1_init);
EXTERN_texttype(NEXT_c2_init);
EXTERN_texttype(NEXT_c3_init);
EXTERN_texttype(NEXT_c4_init);

EXTERN_texttype(DOS160_init);
EXTERN_texttype(DOS160_c1_init);
EXTERN_texttype(DOS160_c2_init);
EXTERN_texttype(DOS160_c3_init);
EXTERN_texttype(DOS160_c4_init);
EXTERN_texttype(DOS160_c5_init);
EXTERN_texttype(DOS160_c6_init);
EXTERN_texttype(DOS160_c7_init);
EXTERN_texttype(DOS160_c8_init);
EXTERN_texttype(DOS160_c9_init);
EXTERN_texttype(DOS160_c10_init);

EXTERN_texttype(UNI200_init);
EXTERN_texttype(UNI201_init);

EXTERN_texttype(JIS220_init);
EXTERN_texttype(JIS230_init);

EXTERN_texttype(KOI8R_c0_init);
EXTERN_texttype(KOI8U_c0_init);

EXTERN_charset(CS_iso_ISO8859_1);
EXTERN_charset(CS_iso_ISO8859_2);
EXTERN_charset(CS_iso_ISO8859_3);
EXTERN_charset(CS_iso_ISO8859_4);
EXTERN_charset(CS_iso_ISO8859_5);
EXTERN_charset(CS_iso_ISO8859_6);
EXTERN_charset(CS_iso_ISO8859_7);
EXTERN_charset(CS_iso_ISO8859_8);
EXTERN_charset(CS_iso_ISO8859_9);
EXTERN_charset(CS_iso_ISO8859_13);
EXTERN_charset(CS_win1250);
EXTERN_charset(CS_win1251);
EXTERN_charset(CS_win1252);
EXTERN_charset(CS_win1253);
EXTERN_charset(CS_win1254);
EXTERN_charset(CS_win1255);
EXTERN_charset(CS_win1256);
EXTERN_charset(CS_win1257);
EXTERN_charset(CS_next);
EXTERN_charset(CS_cyrl);
EXTERN_charset(CS_dos_437);
EXTERN_charset(CS_dos_850);
EXTERN_charset(CS_dos_852);
EXTERN_charset(CS_dos_857);
EXTERN_charset(CS_dos_860);
EXTERN_charset(CS_dos_861);
EXTERN_charset(CS_dos_863);
EXTERN_charset(CS_dos_865);
EXTERN_charset(CS_dos_737);
EXTERN_charset(CS_dos_775);
EXTERN_charset(CS_dos_858);
EXTERN_charset(CS_dos_862);
EXTERN_charset(CS_dos_864);
EXTERN_charset(CS_dos_866);
EXTERN_charset(CS_dos_869);
EXTERN_charset(CS_unicode_ucs2);
EXTERN_charset(CS_unicode_fss);
EXTERN_charset(CS_sjis);
EXTERN_charset(CS_euc_j);

EXTERN_charset(CS_big_5);
EXTERN_charset(CS_gb_2312);
EXTERN_charset(CS_ksc_5601);

EXTERN_charset(CS_koi8r);
EXTERN_charset(CS_koi8u);

EXTERN_texttype(BIG5_init);
EXTERN_texttype(KSC_5601_init);
EXTERN_texttype(ksc_5601_dict_init);
EXTERN_texttype(GB_2312_init);

#ifdef NOT_USED_OR_REPLACED
EXTERN_convert(CV_dos_437_x_iso8859_1);
EXTERN_convert(CV_dos_865_x_iso8859_1);
EXTERN_convert(CV_dos_437_x_dos_865);
EXTERN_convert(CVJIS_sjis_x_eucj);
#endif


#ifdef DEV_BUILD
void LD_assert(const SCHAR* filename, int lineno)
{
/**************************************
 *
 *	L D _ a s s e r t
 *
 **************************************
 *
 * Functional description
 *
 *	Utility function for fb_assert() macro 
 *	Defined locally (clone from jrd/err.c) as ERR_assert isn't
 *	a shared module entry point on all platforms, whereas gds__log is.
 *
 **************************************/
	char buffer[MAXPATHLEN];

	sprintf(buffer,
			"Assertion failed: component intl, file \"%s\", line %d\n",
			filename, lineno);
#if !(defined VMS || defined WIN_NT)
/*	gds__log(buffer); -- see note above */
#endif
	printf(buffer);
}
#endif


/* Note: Cannot use a lookup table here as SUN4 cannot init pointers inside
 * shared libraries
 */

/*
#define DRIVER(num, name) \
	case (num): \
		*fun = (FPTR_SHORT) (name); \
		return (0);

#define CHARSET_INIT(num, name) \
	case (num): \
		*fun = (FPTR_SHORT) (name); \
		return (0);
*/

#ifdef NOT_USED_OR_REPLACED
#define CONVERT_INIT_BI(to_cs, from_cs, name)\
	if (((parm1 == (to_cs)) && (parm2 == (from_cs))) ||\
	    ((parm2 == (to_cs)) && (parm1 == (from_cs)))) \
	    {\
	    *fun = (FPTR_SHORT) (name);\
	    return (0);\
	    }
#endif

INTL_BOOL FB_DLL_EXPORT LD_lookup_charset(charset* cs, const ASCII* name)
{
#define CHARSET(cs_name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol, attr) \
    if (strcmp(name, cs_name) == 0) \
		return cs_symbol(cs, name);
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, symbol, attr)
#define COLLATE_ALIAS(name, coll_id)
#define END_CHARSET

#define INTL_COMPONENT_FB

#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS
#undef END_CHARSET

	return (false);
}

INTL_BOOL FB_DLL_EXPORT LD_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
										   USHORT attributes, const UCHAR* specific_attributes,
										   ULONG specific_attributes_length, INTL_BOOL ignore_attributes)
{
#define CHARSET(cs_name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol, coll_attr) \
	if (strcmp(charset_name, cs_name) == 0) { \
    if (strcmp(texttype_name, cs_name) == 0) \
	return cp_symbol(tt, texttype_name, charset_name, (ignore_attributes ? coll_attr : attributes), \
					 (ignore_attributes ? NULL : specific_attributes), \
					 (ignore_attributes ? 0 : specific_attributes_length));
#define CSALIAS(name, cs_id)
#define END_CHARSET }
#define COLLATION(tt_name, cc_id, cs_id, coll_id, symbol, coll_attr) \
    if (strcmp(texttype_name, tt_name) == 0) \
		return symbol(tt, texttype_name, charset_name, (ignore_attributes ? coll_attr : attributes), \
					  (ignore_attributes ? NULL : specific_attributes), \
					  (ignore_attributes ? 0 : specific_attributes_length));
#define COLLATE_ALIAS(name, coll_id)

#define INTL_COMPONENT_FB

#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS
#undef END_CHARSET

	return (false);
}

#undef DRIVER
#undef CHARSET_INIT
#undef CONVERT_INIT_BI
