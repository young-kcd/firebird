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
extern "C" {
#endif

#define USHORT unsigned short
#define SSHORT short
#define UCHAR unsigned char
#define CHAR char
#define SCHAR char
#define ULONG unsigned long
#define LONG long
#define SLONG signed long
#define VEC	void*
#define BYTE unsigned char

/* duplicate definition from flu.c */

typedef SSHORT(*FPTR_short) ();
#ifndef INTL_ENGINE_INTERNAL
typedef USHORT(*FPTR_SHORT) ();

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

#define MAX_KEY		256
#endif

typedef struct intl_blk {
    UCHAR blk_type;
    UCHAR blk_pool_id;
    USHORT blk_length;
} intl_blk;

typedef unsigned short(*pfn_INTL_init)(struct texttype*, short, short);

typedef struct texttype {
	struct intl_blk texttype_blk;
	USHORT texttype_version;	/* version ID of object */
	USHORT texttype_flags;		/* miscellanous flags */
	TTYPE_ID texttype_type;		/* Interpretation ID */
	const ASCII *texttype_name;
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

	/* MUST BE ALIGNED */
	FPTR_SHORT texttype_fn_init;
	FPTR_SHORT texttype_fn_key_length;
	FPTR_SHORT texttype_fn_string_to_key;
	FPTR_short texttype_fn_compare;
	FPTR_SHORT texttype_fn_to_upper;	/* convert one ch to uppercase */
	FPTR_SHORT texttype_fn_to_lower;	/* One ch to lowercase */
	FPTR_short texttype_fn_str_to_upper;	/* Convert string to uppercase */
	FPTR_SHORT texttype_fn_to_wc;	/* convert string to wc */
	FPTR_SHORT texttype_fn_contains;	/* s1 contains s2? */
	FPTR_SHORT texttype_fn_like;	/* s1 like s2? */
	FPTR_SHORT texttype_fn_matches;	/* s1 matches s2 */
	FPTR_SHORT texttype_fn_sleuth_check;	/* s1 sleuth s2 */
	FPTR_SHORT texttype_fn_sleuth_merge;	/* aux function for sleuth */
	FPTR_short texttype_fn_mbtowc;	/* get next character */
	BYTE *texttype_collation_table;
	BYTE *texttype_toupper_table;
	BYTE *texttype_tolower_table;
	BYTE *texttype_expand_table;
	BYTE *texttype_compress_table;
	BYTE *texttype_misc;		/* Used by some drivers */
	ULONG *texttype_unused[4];	/* spare space */
} *TEXTTYPE;

#define TEXTTYPE_init               1	/* object has been init'ed */
#define TEXTTYPE_reverse_secondary  2	/* Reverse order of secondary keys */
#define TEXTTYPE_ignore_specials    4	/* Do not put special values in keys */
#define TEXTTYPE_expand_before      8	/* Expansion weights before litagure */




typedef struct csconvert {
	struct intl_blk csconvert_blk;
	USHORT csconvert_version;
	USHORT csconvert_flags;
	SSHORT csconvert_id;
	ASCII *csconvert_name;
	CHARSET_ID csconvert_from;
	CHARSET_ID csconvert_to;
	FPTR_SHORT csconvert_convert;
	BYTE *csconvert_datatable;
	BYTE *csconvert_misc;
	ULONG *csconvert_unused[2];
} *CSCONVERT;

/* values for csconvert_flags */

#define CONVERTTYPE_init	1	/* object has been init'ed */

/* Conversion error codes */

#define	CS_TRUNCATION_ERROR	1	/* output buffer too small  */
#define	CS_CONVERT_ERROR	2	/* can't remap a character      */
#define	CS_BAD_INPUT		3	/* input string detected as bad */

#define	CS_CANT_MAP		0		/* Flag table entries that don't map */





typedef struct charset
{
	struct intl_blk charset_blk;
	USHORT charset_version;
	USHORT charset_flags;
	CHARSET_ID charset_id;
	const ASCII *charset_name;
	BYTE charset_min_bytes_per_char;
	BYTE charset_max_bytes_per_char;
	BYTE charset_space_length;
	BYTE *charset_space_character;

	/* Must be aligned */
	FPTR_SHORT charset_well_formed;
	struct csconvert charset_to_unicode;
	struct csconvert charset_from_unicode;

	VEC charset_converters;
	VEC charset_collations;
	ULONG *charset_unused[2];
} *CHARSET;

/* values for charset_flags */

#define	CHARSET_init	1
#define CHARSET_narrow	2
#define CHARSET_multi	4
#define CHARSET_wide	8

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_INTLOBJ_H */

