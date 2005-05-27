/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_narrow.h
 *	DESCRIPTION:	Common base for Narrow language drivers
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

#define TEXTTYPE_reverse_secondary				0x01	/* Reverse order of secondary keys */
#define TEXTTYPE_ignore_specials				0x02	/* Do not put special values in keys */
#define TEXTTYPE_expand_before					0x04	/* Expansion weights before litagure */
#define TEXTTYPE_secondary_insensitive			0x08	/* Don't use secondary level for comparisions */
#define TEXTTYPE_tertiary_insensitive			0x10	/* Don't use tertiary level for comparisions */
#define TEXTTYPE_non_multi_level				0x20	/* Sortkey isn't more precise than equivalence class */

struct TextTypeImpl
{
	USHORT texttype_flags;
	BYTE texttype_bytes_per_key;
	const BYTE* texttype_collation_table;
	const BYTE* texttype_expand_table;
	const BYTE* texttype_compress_table;
	const BYTE* texttype_toupper_table;
	const BYTE* texttype_tolower_table;
};

USHORT LC_NARROW_key_length(TEXTTYPE obj, USHORT inLen);
USHORT LC_NARROW_string_to_key(TEXTTYPE obj, USHORT iInLen, const BYTE* pInChar,
	USHORT iOutLen, BYTE *pOutChar, USHORT partial);
SSHORT LC_NARROW_compare(TEXTTYPE obj, ULONG l1, const BYTE* s1, ULONG l2, const BYTE* s2, 
	INTL_BOOL* error_flag);
ULONG LC_NARROW_canonical(TEXTTYPE obj, ULONG srcLen, const UCHAR* src, ULONG dstLen, UCHAR* dst);
void LC_NARROW_destroy(TEXTTYPE obj);
