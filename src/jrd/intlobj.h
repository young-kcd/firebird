/*
 *	PROGRAM:	JRD International support
 *	MODULE:		intlobj.h
 *	DESCRIPTION:	International text handling definitions
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
 *
 * 2001.10.09 Claudio Valderrama: reinstall the missing "license mask" field
 *   in the texttype struct so we remain compatible with any IB WRT intl module.
 */

#ifndef JRD_INTLOBJ_H
#define JRD_INTLOBJ_H

#ifdef __cplusplus
namespace Jrd {
	class LikeObject;
	class ContainsObject;
	class vec;
	class thread_db;
	class TextType;
}
typedef Jrd::LikeObject engine_LikeObject;
typedef Jrd::ContainsObject engine_ContainsObject;
typedef Jrd::vec engine_vec;
typedef Jrd::thread_db engine_thread_db;
typedef Jrd::TextType engine_TextType;
#else
struct engine_LikeObject;
struct engine_ContainsObject;
struct engine_vec;
struct engine_thread_db;
struct engine_TextType;
#endif


#ifndef INCLUDE_FB_TYPES_H
typedef unsigned short USHORT;
typedef short SSHORT;
typedef unsigned char UCHAR;
typedef char CHAR;
typedef char SCHAR;
typedef unsigned char BYTE;

typedef unsigned int ULONG;
typedef int LONG;
typedef signed int SLONG;

#endif

typedef USHORT (*FPTR_SHORT) ();

typedef SSHORT CHARSET_ID;
typedef SSHORT COLLATE_ID;
typedef USHORT TTYPE_ID;
typedef SCHAR ASCII;
typedef unsigned char NCHAR;	/* Narrow Char */
typedef unsigned short UCS2_CHAR;	/* Not very Wide Char */
typedef unsigned char MBCHAR;	/* Multibyte Char */

#define type_texttype 54
#define type_charset 55
#define type_csconvert 56

typedef struct intl_blk {
    UCHAR blk_type;
    UCHAR blk_pool_id;
    USHORT blk_length;
} intl_blk;

struct texttype; /* forward decl for the fc signatures before the struct itself. */
struct csconvert;

typedef USHORT (*pfn_INTL_init)(texttype*, USHORT, USHORT);
typedef USHORT (*pfn_INTL_keylength)(texttype*, USHORT);
/* Last param is bool, but since it's a C interface, I left it as USHORT. */
typedef USHORT (*pfn_INTL_str2key)(texttype*, USHORT, const UCHAR*, USHORT, UCHAR*, USHORT);
typedef SSHORT (*pfn_INTL_compare)(texttype*, USHORT, const UCHAR*, USHORT, const UCHAR*);
typedef USHORT (*pfn_INTL_ch_case)(texttype*, UCHAR ch);
typedef SSHORT (*pfn_INTL_str2upper)(texttype*, USHORT, const UCHAR*, USHORT, UCHAR*);
typedef USHORT (*pfn_INTL_2wc)(texttype*, UCS2_CHAR*, USHORT, const UCHAR*, USHORT, SSHORT*, USHORT*);

typedef SSHORT (*pfn_INTL_mb2wc)(texttype*, UCS2_CHAR*, const UCHAR*, USHORT);


typedef struct texttype {
	/* DATA THAT IS USED BY BOTH ENGINE AND DRIVERS -------------------------------- */
	struct intl_blk texttype_blk; /* Filled by engine for backward compatibility */
	USHORT texttype_version;	/* version ID of object */
	USHORT texttype_flags;		/* miscellanous flags */
	TTYPE_ID texttype_type;		/* Interpretation ID */
	const ASCII* texttype_name;
	CHARSET_ID texttype_character_set;	/* ID of base character set */
	SSHORT texttype_country;	/* ID of base country values */
	BYTE texttype_bytes_per_char;	/* max bytes per character */

    /* CVC: This struct member is needed for compatibility with older versions
    regarding the international support module, so don't remove it. It was removed
    in an oversight when the code was opensourced and reinstalled by Borland later,
    but never put back by Firebird until 2001.10.09. Originally it was named
    texttype_license_mask, but since it's useless in the open source version, I
    followed the new name given in the BSC tree. */
    ULONG texttype_obsolete_field;		/* required bits for license */
	/*\\ END OF INTL PUBLIC DATA --------------------------------------------------- */

	/* DRIVER API FUNCTIONS. Called by the engine ---------------------------------- */
	pfn_INTL_init		texttype_fn_init;
	pfn_INTL_keylength	texttype_fn_key_length;
	pfn_INTL_str2key	texttype_fn_string_to_key;
	pfn_INTL_compare	texttype_fn_compare;
	pfn_INTL_ch_case	texttype_fn_to_upper;	/* convert one ch to uppercase */
	pfn_INTL_ch_case	texttype_fn_to_lower;	/* One ch to lowercase */
	pfn_INTL_str2upper	texttype_fn_str_to_upper;	/* Convert string to uppercase */
	pfn_INTL_2wc		texttype_fn_to_wc;	/* convert string to wc */
	/*\\ END OF DRIVER API FUNCTIONS ----------------------------------------------- */

	/* ENGINE INTERNAL FUNCTIONS - do not implement in collation drivers ----------- */
	typedef bool (*pfn_INTL_contains)(engine_thread_db*, engine_TextType, const UCHAR*,
		SSHORT, const UCHAR*, SSHORT);
	typedef bool (*pfn_INTL_like)(engine_thread_db*, engine_TextType, const UCHAR*,
		SSHORT, const UCHAR*, SSHORT, UCS2_CHAR);
	typedef bool (*pfn_INTL_matches)(engine_thread_db*, engine_TextType, const UCHAR*, SSHORT, 
		const UCHAR*, SSHORT);
	typedef bool (*pfn_INTL_sleuth_check)(engine_thread_db*, engine_TextType, USHORT, 
		const UCHAR*, USHORT, const UCHAR*, USHORT);
	typedef USHORT (*pfn_INTL_sleuth_merge)(engine_thread_db*, engine_TextType, const UCHAR*,
		USHORT, const UCHAR*, USHORT, UCHAR*, USHORT);

	pfn_INTL_contains		texttype_fn_contains;	/* s1 contains s2? */
	pfn_INTL_like			texttype_fn_like;	/* s1 like s2? */
	pfn_INTL_matches		texttype_fn_matches;	/* s1 matches s2 */
	pfn_INTL_sleuth_check	texttype_fn_sleuth_check;	/* s1 sleuth s2 */
	pfn_INTL_sleuth_merge	texttype_fn_sleuth_merge;	/* aux function for sleuth */
	/*\\ END OF INTERNAL FUNCTIONS ------------------------------------------------- */

	/* DRIVER API FUNCTIONS. Called by the engine ---------------------------------- */
	pfn_INTL_mb2wc		texttype_fn_mbtowc;	/* get next character */
	/*\\ END OF DRIVER API FUNCTIONS ----------------------------------------------- */

    /* DATA USED BY COLLATION DRIVERS. Never used by engine directly --------------- */
	const BYTE* texttype_collation_table;
	const BYTE* texttype_toupper_table;
	const BYTE* texttype_tolower_table;
	const BYTE* texttype_expand_table;
	const BYTE* texttype_compress_table;
	const BYTE* texttype_misc;		/* Used by some drivers */
	ULONG* texttype_unused[4];	/* spare space for use by drivers */
	/*\\ END OF COLLATION DRIVER DATA ---------------------------------------------- */

	/* ENGINE INTERNAL FUNCTIONS - do not implement in collation drivers ----------- */
	typedef engine_LikeObject* (*pfn_INTL_like_create)(engine_thread_db*, engine_TextType, 
		const UCHAR*, SSHORT, UCS2_CHAR);
	typedef engine_ContainsObject* (*pfn_INTL_contains_create)(engine_thread_db*, engine_TextType, 
		const UCHAR*, SSHORT);

	pfn_INTL_like_create		texttype_fn_like_create;
	pfn_INTL_contains_create	texttype_fn_contains_create;
	/*\\ END OF INTERNAL FUNCTIONS ------------------------------------------------- */
} *TEXTTYPE;

#define TEXTTYPE_init               1	/* object has been init'ed */
#define TEXTTYPE_reverse_secondary  2	/* Reverse order of secondary keys */
#define TEXTTYPE_ignore_specials    4	/* Do not put special values in keys */
#define TEXTTYPE_expand_before      8	/* Expansion weights before litagure */



typedef USHORT (*pfn_INTL_convert)(csconvert*, UCHAR*, USHORT,
	const UCHAR*, USHORT, SSHORT*, USHORT*);

struct csconvert {
	struct intl_blk csconvert_blk;
	USHORT csconvert_version;
	USHORT csconvert_flags;
	SSHORT csconvert_id;
	const ASCII* csconvert_name;
	CHARSET_ID csconvert_from;
	CHARSET_ID csconvert_to;
	pfn_INTL_convert csconvert_convert;
	const BYTE* csconvert_datatable;
	const BYTE* csconvert_misc;
	ULONG* csconvert_unused[2];
};

/* values for csconvert_flags */

#define CONVERTTYPE_init	1	/* object has been init'ed */

/* Conversion error codes */

#define	CS_TRUNCATION_ERROR	1	/* output buffer too small  */
#define	CS_CONVERT_ERROR	2	/* can't remap a character      */
#define	CS_BAD_INPUT		3	/* input string detected as bad */

#define	CS_CANT_MAP		0		/* Flag table entries that don't map */




typedef USHORT (*pfn_well_formed)(const UCHAR*, USHORT);

struct charset
{
	struct intl_blk charset_blk;
	USHORT charset_version;
	USHORT charset_flags;
	CHARSET_ID charset_id;
	const ASCII* charset_name;
	BYTE charset_min_bytes_per_char;
	BYTE charset_max_bytes_per_char;
	BYTE charset_space_length;
	const BYTE* charset_space_character;

	/* Must be aligned */
	pfn_well_formed	charset_well_formed;
	csconvert		charset_to_unicode;
	csconvert		charset_from_unicode;

	engine_vec* charset_converters;
	engine_vec* charset_collations;
	ULONG* charset_unused[2];
};

/* values for charset_flags */

#define	CHARSET_init	1
#define CHARSET_narrow	2
#define CHARSET_multi	4
#define CHARSET_wide	8

#endif /* JRD_INTLOBJ_H */

