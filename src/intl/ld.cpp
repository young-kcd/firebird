/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		ld.c
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

#define	EXTERN_texttype(name)	USHORT name (TEXTTYPE, SSHORT, SSHORT)
#define EXTERN_convert(name)	USHORT name (CSCONVERT, SSHORT, SSHORT)
#define EXTERN_charset(name)	USHORT name (CHARSET, SSHORT, SSHORT)

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

EXTERN_texttype(BIG5_init);
EXTERN_texttype(KSC_5601_init);
EXTERN_texttype(ksc_5601_dict_init);
EXTERN_texttype(GB_2312_init);

EXTERN_convert(CV_dos_437_x_iso8859_1);
EXTERN_convert(CV_dos_865_x_iso8859_1);
EXTERN_convert(CV_dos_437_x_dos_865);
EXTERN_convert(CVJIS_sjis_x_eucj);


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
 *	Utility function for assert() macro 
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
	//ib_printf(buffer);
	printf(buffer);
}
#endif


/* Note: Cannot use a lookup table here as SUN4 cannot init pointers inside
 * shared libraries
 */

#define DRIVER(num, name) \
	case (num): \
		*fun = (FPTR_SHORT) (name); \
		return (0);

#define CHARSET_INIT(num, name) \
	case (num): \
		*fun = (FPTR_SHORT) (name); \
		return (0);

#define CONVERT_INIT_BI(to_cs, from_cs, name)\
	if (((parm1 == (to_cs)) && (parm2 == (from_cs))) ||\
	    ((parm2 == (to_cs)) && (parm1 == (from_cs)))) \
	    {\
	    *fun = (FPTR_SHORT) (name);\
	    return (0);\
	    }

USHORT DLL_EXPORT LD_lookup(USHORT objtype,
							FPTR_SHORT * fun, SSHORT parm1, SSHORT parm2)
{
/**************************************
 *
 *	L D _ l o o k u p 
 *
 **************************************
 *
 * Functional description
 *
 *	Lookup an international object implementation via object type
 *	and id (one id for charsets & collations, two ids for converters).
 *
 *	Note: This routine is a cousin of intl/ld2.c:LD2_lookup
 *
 * Return:
 *	If object implemented by this module:
 *		(fun)	set to the object initializer
 *		0	returned	
 *	Otherwise:
 *		(fun)	set to nil.
 *		1	returned
 *
 **************************************/

	switch (objtype) {
	case type_texttype:
		switch (parm1) {

#define CHARSET(name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol) \
	    case (cs_id): \
		*fun = (FPTR_SHORT) (cp_symbol); \
		return (0);
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, symbol) \
	    case (((coll_id) << 8) | (cs_id)): \
		*fun = (FPTR_SHORT) (symbol); \
		return (0);
#define COLLATE_ALIAS(name, coll_id)
#define END_CHARSET

#define _INTL_COMPONENT_

#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS
#undef END_CHARSET

/*
	    DRIVER (CS_DOS_437, DOS101_init);
	    DRIVER (CS_DOS_850, DOS160_init);
	    DRIVER (CS_DOS_865, DOS107_init);
	    DRIVER (CS_ISO8859_1,  ISO88591_cp_init);
	    DRIVER (CS_ISO8859_2,  ISO88592_cp_init);
	    DRIVER (CS_UNICODE_FSS, UNI201_init);
	    DRIVER (CS_SJIS,    JIS220_init);
	    DRIVER (CS_EUCJ,    JIS230_init);

	    DRIVER (101, DOS101_init);
	    DRIVER (102, DOS102_init);
	    DRIVER (105, DOS105_init);
	    DRIVER (106, DOS106_init);
	    DRIVER (107, DOS107_init);
	    DRIVER (139, ISO88591_39_init);
	    DRIVER (140, ISO88591_40_init);
	    DRIVER (141, ISO88591_41_init);
	    DRIVER (142, ISO88591_42_init);
	    DRIVER (143, ISO88591_43_init);
	    DRIVER (144, ISO88591_44_init);
	    DRIVER (145, ISO88591_45_init);
	    DRIVER (146, ISO88591_46_init);
	    DRIVER (148, ISO88591_48_init);
	    DRIVER (149, ISO88591_49_init);
	    DRIVER (151, ISO88591_51_init);
	    DRIVER (152, ISO88591_52_init);
	    DRIVER (153, ISO88591_53_init);
	    DRIVER (154, ISO88591_54_init);
	    DRIVER (160, DOS160_init);
	    DRIVER (200, UNI200_init);
	    DRIVER (201, UNI201_init);
	    DRIVER (220, JIS220_init);
	    DRIVER (230, JIS230_init);
*/

		default:
			*fun = NULL;
			return (1);
		};
	case type_charset:
		switch (parm1) {
#define CHARSET(name, cs_id, coll_id, bytes, num, cs_symbol, cp_symbol) \
	    case (cs_id): \
		*fun = (FPTR_SHORT) (cs_symbol); \
		return (0);
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, symbol)
#define COLLATE_ALIAS(name, coll_id)
#define END_CHARSET

#define _INTL_COMPONENT_


#include "../jrd/intlnames.h"

#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef COLLATE_ALIAS

/*
	    CHARSET_INIT (CS_ISO8859_1,     CS_iso_ISO8859_1);
	    CHARSET_INIT (CS_DOS_437,    CS_dos_437);
	    CHARSET_INIT (CS_DOS_850,    CS_dos_850);
	    CHARSET_INIT (CS_DOS_865,    CS_dos_865);
	    CHARSET_INIT (CS_UNICODE_UCS2, CS_unicode_ucs2);
	    CHARSET_INIT (CS_UNICODE_FSS, CS_unicode_fss);
	    CHARSET_INIT (CS_SJIS,       CS_sjis);
	    CHARSET_INIT (CS_EUCJ,       CS_euc_j);
*/
		default:
			*fun = NULL;
			return (1);
		};
	case type_csconvert:
		{
			CONVERT_INIT_BI(CS_DOS_437, CS_ISO8859_1, CV_dos_437_x_iso8859_1);
			CONVERT_INIT_BI(CS_DOS_865, CS_ISO8859_1, CV_dos_865_x_iso8859_1);
			CONVERT_INIT_BI(CS_DOS_437, CS_DOS_865, CV_dos_437_x_dos_865);
			CONVERT_INIT_BI(CS_SJIS, CS_EUCJ, CVJIS_sjis_x_eucj);
			*fun = NULL;
			return (1);
		}
	default:
#ifdef DEV_BUILD
		assert(0);
#endif
		*fun = NULL;
		return (1);
	}
}

#undef DRIVER
#undef CHARSET_INIT
#undef CONVERT_INIT_BI
