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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "../jrd/IntlUtil.h"
#include "../intl/ldcommon.h"
#include "../intl/ld_proto.h"
#include "../intl/cs_icu.h"
#include "../intl/lc_icu.h"
#include "fb_exception.h"

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h> /* for MAXPATHLEN */
#endif
#include <stdio.h>

using namespace Firebird;

/* Commented out to make Linux version work because it is inaccessiable on all
  known platforms. Nickolay Samofatov, 10 Sept 2002
void gds__log(UCHAR*, ...);
*/

#define	EXTERN_texttype(name)	INTL_BOOL name (texttype*, charset*, const ASCII*, const ASCII*, USHORT, const UCHAR*, ULONG, const ASCII*)
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
EXTERN_texttype(ISO88591_56_init);

/* Latin 2 character set */
EXTERN_texttype (ISO88592_cp_init);

/* latin 2 collations */
EXTERN_texttype (ISO88592_c1_init);
EXTERN_texttype (ISO88592_c2_init);
EXTERN_texttype (ISO88592_c3_init);

EXTERN_texttype (ISO88593_cp_init);
EXTERN_texttype (ISO88594_cp_init);
EXTERN_texttype (ISO88595_cp_init);
EXTERN_texttype (ISO88596_cp_init);
EXTERN_texttype (ISO88597_cp_init);
EXTERN_texttype (ISO88598_cp_init);
EXTERN_texttype (ISO88599_cp_init);

EXTERN_texttype (ISO885913_cp_init);
EXTERN_texttype (ISO885913_c1_init);

EXTERN_texttype(WIN1250_c0_init);
EXTERN_texttype(WIN1250_c1_init);
EXTERN_texttype(WIN1250_c2_init);
EXTERN_texttype(WIN1250_c3_init);
EXTERN_texttype(WIN1250_c4_init);
EXTERN_texttype(WIN1250_c5_init);
EXTERN_texttype(WIN1250_c6_init);
EXTERN_texttype(WIN1250_c7_init);
EXTERN_texttype(WIN1250_c8_init);

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
EXTERN_texttype(WIN1257_c1_init);
EXTERN_texttype(WIN1257_c2_init);
EXTERN_texttype(WIN1257_c3_init);

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
EXTERN_texttype(KOI8R_c1_init);

EXTERN_texttype(KOI8U_c0_init);
EXTERN_texttype(KOI8U_c1_init);

EXTERN_texttype(WIN1258_c0_init);

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

EXTERN_charset(CS_win1258);

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

// ASF: FB 2.0 doesn't call LD_version function, then
// version should be INTL_VERSION_1 by default.
USHORT version = INTL_VERSION_1;


INTL_BOOL FB_DLL_EXPORT LD_lookup_charset(charset* cs, const ASCII* name, const ASCII* config_info)
{
	// ASF: We can't read config_info if version < INTL_VERSION_2,
	// since it wasn't pushed in the stack by the engine.

	try
	{
#define CHARSET(cs_name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol, attr)	\
	{																				\
		EXTERN_charset((*lookup_symbol)) = cs_symbol;								\
																					\
		if (lookup_symbol && strcmp(name, cs_name) == 0)							\
			return lookup_symbol(cs, name);											\
	}
#define CSALIAS(name, cs_id)
#define COLLATION(name, base_name, cc_id, cs_id, coll_id, symbol, attr, specific_attr)
#define COLLATE_ALIAS(name, coll_id)
#define END_CHARSET

#define INTL_COMPONENT_FB

#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS
#undef END_CHARSET

		return CSICU_charset_init(cs, name);
	}
	catch (Firebird::BadAlloc)
	{
		fb_assert(false);
		return false;
	}
}


INTL_BOOL FB_DLL_EXPORT LD_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
										   USHORT attributes, const UCHAR* specific_attributes,
										   ULONG specific_attributes_length, INTL_BOOL ignore_attributes,
										   const ASCII* config_info)
{
	const ASCII* configInfo;

	// ASF: We can't read config_info if version < INTL_VERSION_2,
	// since it wasn't pushed in the stack by the engine.
	if (version >= INTL_VERSION_2)
		configInfo = config_info;
	else
		configInfo = "";

	try
	{
#define CHARSET(cs_name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol, coll_attr)	\
	if (strcmp(charset_name, cs_name) == 0)												\
	{																					\
		EXTERN_charset((*lookup_cs_symbol)) = cs_symbol;								\
		EXTERN_texttype((*lookup_symbol)) = cp_symbol;									\
																						\
		charset cs;																		\
		memset(&cs, 0, sizeof(cs));														\
																						\
		if (lookup_cs_symbol != NULL && lookup_cs_symbol(&cs, charset_name) &&			\
			lookup_symbol != NULL && strcmp(texttype_name, cs_name) == 0)				\
		{																				\
			INTL_BOOL ret = lookup_symbol(												\
				tt, &cs, texttype_name, charset_name,									\
				(ignore_attributes ? coll_attr : attributes),							\
				(ignore_attributes ? NULL : specific_attributes),						\
				(ignore_attributes ? 0 : specific_attributes_length),					\
				configInfo);															\
																						\
			if (cs.charset_fn_destroy)													\
				cs.charset_fn_destroy(&cs);												\
																						\
			return ret;																	\
		}
#define CSALIAS(name, cs_id)
#define END_CHARSET	\
	}
#define COLLATION(tt_name, base_name, cc_id, cs_id, coll_id, symbol, coll_attr, specific_attr)	\
	{																	\
		EXTERN_texttype((*coll_lookup_symbol)) = symbol;				\
																		\
		if (coll_lookup_symbol && strcmp(texttype_name, (base_name ? base_name : tt_name)) == 0)	\
		{																\
			INTL_BOOL ret = coll_lookup_symbol(							\
				tt, &cs, texttype_name, charset_name,					\
				(ignore_attributes ? coll_attr : attributes),			\
				(ignore_attributes ? (UCHAR*) specific_attr : specific_attributes),	\
				(ignore_attributes ?									\
					(specific_attr ? strlen(specific_attr) : 0) :		\
					specific_attributes_length),						\
				configInfo);											\
																		\
			if (cs.charset_fn_destroy)									\
				cs.charset_fn_destroy(&cs);								\
																		\
			return ret;													\
		}																\
	}
#define COLLATE_ALIAS(name, coll_id)

#define INTL_COMPONENT_FB

#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS
#undef END_CHARSET

		return LCICU_texttype_init(
			tt, texttype_name, charset_name, (ignore_attributes ? TEXTTYPE_ATTR_PAD_SPACE : attributes),
			(ignore_attributes ? NULL : specific_attributes),
			(ignore_attributes ? 0 : specific_attributes_length), configInfo);
	}
	catch (Firebird::BadAlloc)
	{
		fb_assert(false);
		return false;
	}
}

#undef DRIVER
#undef CHARSET_INIT
#undef CONVERT_INIT_BI


ULONG FB_DLL_EXPORT LD_setup_attributes(
	const ASCII* textTypeName, const ASCII* charSetName, const ASCII* configInfo,
	ULONG srcLen, const UCHAR* src, ULONG dstLen, UCHAR* dst)
{
	Firebird::string specificAttributes((const char*) src, srcLen);
	Firebird::string newSpecificAttributes = specificAttributes;
	
	if (!LCICU_setup_attributes(textTypeName, charSetName, configInfo,
			specificAttributes, newSpecificAttributes))
	{
		return INTL_BAD_STR_LENGTH;
	}

	if (dstLen == 0)
		return newSpecificAttributes.length();
	else if (newSpecificAttributes.length() <= dstLen)
	{
		memcpy(dst, newSpecificAttributes.begin(), newSpecificAttributes.length());
		return newSpecificAttributes.length();
	}
	else
		return INTL_BAD_STR_LENGTH;
}


void FB_DLL_EXPORT LD_version(USHORT* version)
{
	// We support version 1 and 2.
	if (*version != INTL_VERSION_1)
		*version = INTL_VERSION_2;

	::version = *version;
}
