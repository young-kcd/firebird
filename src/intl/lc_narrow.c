/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		lc_narrow.c
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


#include "firebird.h"
#include "../intl/ldcommon.h"

USHORT LC_NARROW_key_length();
USHORT LC_NARROW_string_to_key();
SSHORT LC_NARROW_compare();



/*
 * Generic base for InterBase 4.0 Language Driver
 */


#define LANGFAM2_MAX_KEY     	(MAX_KEY)

#define ASCII_SPACE	32
#define	NULL_WEIGHT	0
#define	NULL_SECONDARY	0
#define	NULL_TERTIARY	0




/*
 * key_length (obj, inLen)
 *
 * For an input string of (inLen) bytes, return the maximum
 * key buffer length.
 *
 * This is used for index buffer allocation within the
 * Engine.
 *
 *  Note:
 *	Strings containing (m) expand chars need 2*(m)*3
 *	bytes for key values of the expansion.  This is
 *	offset by string values that don't have corresponding
 *	secondary or tertiary key values.
 *	Let:
 *		n = length of input string
 *		np = count of "simple" bytes in the string, alphabetic
 *		     no secondary and no tertiary.
 *		ns = Has secondary or tertiary, but not both
 *		nt = Has tertiary and secondary.
 *		nc = is a COMPRESSED value.
 *		ne = Has an EXPAND value.
 *		nsp = Is a special value.
 *
 *		n = np + ns + nt + nc + ne + nsp
 *
 *		Key_length(n) =
 *			np
 *			+ 2 * ns
 *			+ 3 * nt
 *			+ 3 * (nc/2)
 *			+ 3 * 2 * ne 
 *			+ 2 * nsp
 *			+ 1 (if nsp > 0, for separating keys from special keys)
 *
 *	Clearly this is maximized when the string consists solely of
 *	EXPAND characters.  This degenerate case doesn't occur in
 *	standard text usage, except for short strings (1-2 characters).
 *
 *	Therefore, we compute the keylength based on the "normal" case
 *	of the (nt) term.  It is likely we could choose a probabilistic value
 *	(such as 2.5 * n) for the length of the key.
 *
 *	The degenerate case of short strings is handled by a minimal key
 *	length.
 */
USHORT LC_NARROW_key_length(obj, inLen)
	 TEXTTYPE obj;
	 USHORT inLen;
{
	USHORT len;
/* assert (inLen <= LANGFAM2_MAX_KEY); *//* almost certainly an error */
	len = 3 * MAX(inLen, 2);
	return (MIN(len, LANGFAM2_MAX_KEY));
}



#ifdef DEBUG
#include "../jrd/ib_stdio.h"
static ULONG do_debug = 0;
#endif

/*
 *
 *  Convert a user string to a sequence that will collate bytewise.
 * 
 * RETURN:
 *		Length, in bytes, of returned key
 */
USHORT LC_NARROW_string_to_key(obj, iInLen, pInChar, iOutLen, pOutChar,
							   partial)
	 TEXTTYPE obj;
	 USHORT iInLen;
	 BYTE *pInChar;
	 USHORT iOutLen;
	 BYTE *pOutChar;
	 USHORT partial;
{
	USHORT lprimary;
	BYTE secondary[LANGFAM2_MAX_KEY];
	USHORT lsecondary;
	BYTE tertiary[LANGFAM2_MAX_KEY];
	USHORT ltertiary;
	BYTE special[LANGFAM2_MAX_KEY * 2];
	USHORT lspecial;

	USHORT i, j;

	BYTE *outbuff;
	BYTE *inbuff;
	struct SortOrderTblEntry *coll;

	assert(pOutChar != NULL);
	assert(pInChar != NULL);
/* assert (iInLen   <= LANGFAM2_MAX_KEY); */
	assert(iOutLen <= LANGFAM2_MAX_KEY);
	assert(iOutLen >= LC_NARROW_key_length(obj, iInLen));

#ifdef DEBUG
/* Dump out the input string */
	if (do_debug) {
		ib_printf("string: (%02d) '%*s'\n", iInLen, iInLen, pInChar);
		ib_fflush(ib_stdout);
	};
#endif	/* DEBUG */

	outbuff = pOutChar;
	lprimary = 0;
	lsecondary = 0;
	ltertiary = 0;
	lspecial = 0;

/* point inbuff at last character */
	inbuff = pInChar + iInLen - 1;

/* skip backwards over all spaces & reset input length */
	while ((inbuff >= pInChar) && (*inbuff == ASCII_SPACE))
		inbuff--;
	iInLen = (inbuff - pInChar + 1);

	for (i = 0; i < iInLen; i++, pInChar++) {
		assert(lprimary < iOutLen);
		assert(lsecondary < sizeof(secondary));
		assert(ltertiary < sizeof(tertiary));
		assert(lspecial < sizeof(special));

		coll =
			&((struct SortOrderTblEntry *) obj->
			  texttype_collation_table)[*pInChar];
		if (!(coll->IsExpand || coll->IsCompress)) {
			if (coll->Primary != NULL_WEIGHT)
				outbuff[lprimary++] = coll->Primary;
			if (coll->Secondary != NULL_SECONDARY)
				secondary[lsecondary++] = coll->Secondary;
			if (coll->Tertiary != NULL_TERTIARY)
				tertiary[ltertiary++] = coll->Tertiary;
		}
		else if (coll->IsExpand && coll->IsCompress) {
			/* Both flags set indicate a special value */
			if ((coll->Primary != NULL_WEIGHT) &&
				!(obj->texttype_flags & TEXTTYPE_ignore_specials)) {
				special[lspecial++] = (i + 1);	/* position */
				special[lspecial++] = coll->Primary;
			};
		}
		else if (coll->IsExpand) {
			struct ExpandChar *exp;
			exp = &((struct ExpandChar *) obj->texttype_expand_table)[0];
			while (exp->Ch && exp->Ch != *pInChar)
				exp++;
			assert(exp->Ch == *pInChar);
			for (j = 0; j < 2; j++) {
				if (j)
					coll =
						&((struct SortOrderTblEntry *) obj->
						  texttype_collation_table)[exp->ExpCh2];
				if (coll->Primary != NULL_WEIGHT)
					outbuff[lprimary++] = coll->Primary;
				if (coll->Secondary != NULL_SECONDARY)
					secondary[lsecondary++] = coll->Secondary;
				if (coll->Tertiary != NULL_TERTIARY)
					tertiary[ltertiary++] = coll->Tertiary;
			}
		}
		else {					/* (col->IsCompress) */

			struct CompressPair *cmp;
			if ((USHORT) (i + 1) < iInLen) {
				cmp =
					&((struct CompressPair *) obj->
					  texttype_compress_table)[0];
				while (cmp->CharPair[0]) {
					if ((cmp->CharPair[0] == *pInChar) &&
						(cmp->CharPair[1] == *(pInChar + 1))) {

						/* Gobble the two-to-1 entry */
						coll = &cmp->NoCaseWeight;
						pInChar++;
						i++;
						break;
					}
					cmp++;
				}
			}
			if (coll->Primary != NULL_WEIGHT)
				outbuff[lprimary++] = coll->Primary;
			if (coll->Secondary != NULL_SECONDARY)
				secondary[lsecondary++] = coll->Secondary;
			if (coll->Tertiary != NULL_TERTIARY)
				tertiary[ltertiary++] = coll->Tertiary;
		}
	};

/* primary keys are already in output key */

	outbuff += lprimary;
	iOutLen -= lprimary;

	if (partial)
		/* return length of key */
		return (outbuff - pOutChar);

/* put secondary keys into output key */
	if (obj->texttype_flags & TEXTTYPE_reverse_secondary) {
		for (i = 0; i < lsecondary && iOutLen; i++) {
			*outbuff++ = secondary[lsecondary - i - 1];
			iOutLen--;
		}
	}
	else {
		for (i = 0; i < lsecondary && iOutLen; i++) {
			*outbuff++ = secondary[i];
			iOutLen--;
		};
	};

/* put tertiary keys into output key */
	for (i = 0; i < ltertiary && iOutLen; i++) {
		*outbuff++ = tertiary[i];
		iOutLen--;
	};

/* put special keys into output key */
	if ((lspecial && iOutLen) &&
		!(obj->texttype_flags & TEXTTYPE_ignore_specials)) {
		/* Insert the marker-byte */
		*outbuff++ = 0;
		iOutLen--;
		for (i = 0; i < lspecial && iOutLen; i++) {
			*outbuff++ = special[i];
			iOutLen--;
		};
	};

#ifdef DEBUG
/* Dump out the computed key */
	if (do_debug) {
		UCHAR *p;
		ib_printf("   key: (%02d) ", (outbuff - pOutChar));
		for (p = pOutChar; p < outbuff; p++)
			ib_printf("%2x ", *p);
		ib_printf("\n");
		ib_fflush(ib_stdout);
	};
#endif

/* return length of key */
	return (outbuff - pOutChar);
}



#define	HAVE_WAITING	1
#define HAVE_SPECIAL	2

/* expansion char go before the expansion. */
/* eg: S-set collates before ss */

typedef struct coltab_status {
	USHORT stat_flags;
	struct SortOrderTblEntry *stat_waiting;
} *COLSTAT;




static SSHORT special_scan(obj, l1, s1, l2, s2)
	 TEXTTYPE obj;
	 USHORT l1;
	 BYTE *s1;
	 USHORT l2;
	 BYTE *s2;
{
	struct SortOrderTblEntry *col1;
	struct SortOrderTblEntry *col2;
	USHORT index1, index2;

	index1 = 0;
	index2 = 0;

	while (1) {
		/* Scan to find ignore char from l1 */
		while (l1) {
			col1 =
				&((struct SortOrderTblEntry *) obj->
				  texttype_collation_table)[*s1];
			if (col1->IsExpand && col1->IsCompress)
				break;
			l1--;
			s1++;
			index1++;
		}

		/* Scan to find ignore char from l2 */
		while (l2) {
			col2 =
				&((struct SortOrderTblEntry *) obj->
				  texttype_collation_table)[*s2];
			if (col2->IsExpand && col2->IsCompress)
				break;
			l2--;
			s2++;
			index2++;
		}
		if (!l1 && !l2)			/* All out of ignore characters */
			return 0;
		if (l1 && !l2)			/* Out in l2 only */
			return 1000;
		if (!l1 && l2)			/* Out in l1 only */
			return -1000;
		if (index1 < index2)	/* l1 has ignore ch before l2 */
			return -2000;
		if (index1 > index2)	/* l2 has ignore ch before l1 */
			return 2000;
		if (col1->Primary != col2->Primary)
			return (col1->Primary - col2->Primary);
		l1--;
		l2--;
		s1++;
		s2++;
		index1++;
		index2++;
	}
}


struct SortOrderTblEntry *get_coltab_entry(obj, p, l, stat)
	 TEXTTYPE obj;
	 UCHAR **p;
	 USHORT *l;
	 COLSTAT stat;
{
	struct SortOrderTblEntry *col;

	if (stat->stat_flags & HAVE_WAITING) {
		(*l)--;
		(*p)++;
		stat->stat_flags &= ~HAVE_WAITING;
		assert(stat->stat_waiting);
		return stat->stat_waiting;
	}

	stat->stat_waiting = NULL;
	while (*l) {
		col =
			&((struct SortOrderTblEntry *) obj->
			  texttype_collation_table)[**p];
		if (!(col->IsExpand || col->IsCompress)) {
			/* Have col */
			(*l)--;
			(*p)++;
			return col;
		}
		else if (col->IsExpand && col->IsCompress) {
			/* Both flags set indicate a special value */
			/* Need a new col */
			(*l)--;
			(*p)++;
			stat->stat_flags |= HAVE_SPECIAL;
			continue;
		}
		else if (col->IsExpand) {
			struct ExpandChar *exp;
			exp = &((struct ExpandChar *) obj->texttype_expand_table)[0];
			while (exp->Ch && exp->Ch != **p)
				exp++;
			assert(exp->Ch == **p);
			/* Have coll1 */
			/* Have waiting */

			stat->stat_waiting =
				&((struct SortOrderTblEntry *) obj->
				  texttype_collation_table)[exp->ExpCh2];
			stat->stat_flags |= HAVE_WAITING;
			return col;
		}
		else {					/* (col->IsCompress) */

			struct CompressPair *cmp;
			if (*l > 1) {
				cmp =
					&((struct CompressPair *) obj->
					  texttype_compress_table)[0];
				while (cmp->CharPair[0]) {
					if ((cmp->CharPair[0] == **p) &&
						(cmp->CharPair[1] == *(*p + 1))) {
						/* Have Col */
						col = &cmp->NoCaseWeight;
						(*l) -= 2;
						(*p) += 2;
						return col;
					}
					cmp++;
				}
			}
			/* Have col */
			(*l)--;
			(*p)++;
			return col;
		}
	}
	return NULL;
}



#define XOR	^					/* C bitwise XOR operator - defined for readability */

SSHORT LC_NARROW_compare(obj, l1, s1, l2, s2)
	 TEXTTYPE obj;
	 USHORT l1;
	 BYTE *s1;
	 USHORT l2;
	 BYTE *s2;
{
	struct SortOrderTblEntry *col1, *col2;
	struct coltab_status stat1, stat2;
	SSHORT save_secondary = 0;
	SSHORT save_tertiary = 0;
	SSHORT save_quandary = 0;
	USHORT save_l1, save_l2;
	BYTE *save_s1, *save_s2;
	BYTE *p;

	stat1.stat_flags = 0;
	stat2.stat_flags = 0;

/* Start at EOS, scan backwards to find non-space */
	p = s1 + l1 - 1;
	while ((p >= s1) && (*p == ASCII_SPACE))
		p--;
	l1 = (p - s1 + 1);

	p = s2 + l2 - 1;
	while ((p >= s2) && (*p == ASCII_SPACE))
		p--;
	l2 = (p - s2 + 1);

	save_l1 = l1;
	save_l2 = l2;
	save_s1 = s1;
	save_s2 = s2;

	while (1) {
		col1 = get_coltab_entry(obj, &s1, &l1, &stat1);
		col2 = get_coltab_entry(obj, &s2, &l2, &stat2);
		if (!col1 || !col2)
			break;
		if (col1->Primary != col2->Primary)
			return (col1->Primary - col2->Primary);
		if (col1->Secondary != col2->Secondary) {
			if ((obj->texttype_flags & TEXTTYPE_reverse_secondary) ||
				!save_secondary)
					save_secondary = (col1->Secondary - col2->Secondary);
		}
		else if (col1->Tertiary != col2->Tertiary) {
			if (!save_tertiary)
				save_tertiary = (col1->Tertiary - col2->Tertiary);
		}
		else if (((stat1.stat_flags & HAVE_WAITING) XOR
				  (stat2.stat_flags & HAVE_WAITING)) && !save_quandary) {
			if (obj->texttype_flags & TEXTTYPE_expand_before)
				save_quandary = (stat1.stat_flags & HAVE_WAITING) ? -1 : 1;
			else
				save_quandary = (stat1.stat_flags & HAVE_WAITING) ? 1 : -1;
		}
	}

/* One of the strings ended */
	assert(l1 == 0 || l2 == 0);
	assert(col1 == NULL || col2 == NULL);

	if (col1 && !col2)
		return 500;

	if (!col1 && col2)
		return -500;

	if (l1 == 0 && l2 == 0) {
		if (save_secondary)
			return save_secondary;
		if (save_tertiary)
			return save_tertiary;
		if (save_quandary)
			return save_quandary;
		if (
			((stat1.stat_flags & HAVE_SPECIAL)
			 || (stat2.stat_flags & HAVE_SPECIAL))
			&& !(obj->
				 texttype_flags & TEXTTYPE_ignore_specials)) return
				special_scan(obj, save_l1, save_s1, save_l2, save_s2);
		return 0;
	}

	if (l1)
		return 600;
	return -600;
}



#ifdef DEBUG_COMPARE

/*
 * Debugging only
 * Routine used for comparing results from comparision algorithm
 * to results from key creation algorithm
 */
STATIC SSHORT old_fam2_compare(obj, l1, s1, l2, s2)
	 TEXTTYPE obj;
	 USHORT l1;
	 BYTE *s1;
	 USHORT l2;
	 BYTE *s2;
{
	BYTE key1[LANGFAM2_MAX_KEY];
	BYTE key2[LANGFAM2_MAX_KEY];
	USHORT len1;
	USHORT len2;
	USHORT len;
	USHORT i;

	assert(obj != NULL);
	assert(s1 != NULL);
	assert(s2 != NULL);

	len1 = LC_NARROW_string_to_key(obj, l1, s1, sizeof(key1), key1, FALSE);
	len2 = LC_NARROW_string_to_key(obj, l2, s2, sizeof(key2), key2, FALSE);
	len = MIN(len1, len2);
	for (i = 0; i < len; i++) {
		if (key1[i] == key2[i])
			continue;
		else if (key1[i] < key2[i])
			return (-1);
		else
			return (1);
	};
	if (len1 < len2)
		return (-1);
	else if (len1 > len2)
		return (1);
	return (0);
}
#endif	/* DEBUG_COMPARE */


#ifdef DEBUG_COMPARE

#define	SIGN(x)	(((x) < 0)?-1:(((x) == 0)?0:1))

/*
 * Debugging only
 * Routine used for comparing results from comparision algorithm
 * to results from key creation algorithm
 */
STATIC SSHORT fam2_compare(obj, l1, s1, l2, s2)
	 TEXTTYPE obj;
	 USHORT l1;
	 BYTE *s1;
	 USHORT l2;
	 BYTE *s2;
{
	SSHORT res1, res2;

	res1 = old_fam2_compare(obj, l1, s1, l2, s2);
	res2 = LC_NARROW_compare(obj, l1, s1, l2, s2);

	if (SIGN(res1) != SIGN(res2)) {
		ib_printf("different compares:\n%d %s\n%d %s\nold = %d new = %d\n",
				  l1, s1, l2, s2, res1, res2);
		ib_fflush(ib_stdout);
		do_debug = 1;
		res1 = old_fam2_compare(obj, l1, s1, l2, s2);
		res2 = LC_NARROW_compare(obj, l1, s1, l2, s2);
		do_debug = 0;
	}
	return res2;
}

#endif	/* DEBUG_COMPARE */



#ifdef DEBUG_LD
/*
 * Debugging code for dumping out information contained within a language
 * driver.  Used for V3.3 of Interbase & automatic generation of doc
 * files.  Not currently used for V4 (2 June 1994) but may be needed
 * someday to regenerate doc files
 */

struct texttype tmp_obj;

TEXTTYPE debug_obj = &tmp_obj;

void check_table()
{
	int found_pmax = 0;
	int found_smax = 0;
	int found_tmax = 0;
	int i;
	struct CompressPair *cmp;

	for (i = 0; i < 256; i++) {
#ifdef MAX_NCO_PRIMARY
		if (NoCaseOrderTbl[i].Primary > MAX_NCO_PRIMARY)
			ib_fprintf(ib_stderr, "Error Primary MAX: %d\n", i);
		if (NoCaseOrderTbl[i].Secondary > MAX_NCO_SECONDARY)
			ib_fprintf(ib_stderr, "Error Secondary MAX: %d\n", i);
		if (NoCaseOrderTbl[i].Tertiary > MAX_NCO_TERTIARY)
			ib_fprintf(ib_stderr, "Error Tertiary MAX: %d\n", i);
#endif
		found_pmax = MAX(NoCaseOrderTbl[i].Primary, found_pmax);
		found_smax = MAX(NoCaseOrderTbl[i].Secondary, found_smax);
		found_tmax = MAX(NoCaseOrderTbl[i].Tertiary, found_tmax);
	};
	for (cmp = &CompressTbl[0]; (cmp->CharPair[0]); cmp++) {
#ifdef MAX_NCO_PRIMARY
		if (cmp->NoCaseWeight.Primary > MAX_NCO_PRIMARY)
			ib_fprintf(ib_stderr, "Error Primary MAX: %d\n", i);
		if (cmp->NoCaseWeight.Secondary > MAX_NCO_SECONDARY)
			ib_fprintf(ib_stderr, "Error Secondary MAX: %d\n", i);
		if (cmp->NoCaseWeight.Tertiary > MAX_NCO_TERTIARY)
			ib_fprintf(ib_stderr, "Error Tertiary MAX: %d\n", i);
#endif
		found_pmax = MAX(cmp->NoCaseWeight.Primary, found_pmax);
		found_smax = MAX(cmp->NoCaseWeight.Secondary, found_smax);
		found_tmax = MAX(cmp->NoCaseWeight.Tertiary, found_tmax);
	};
#ifdef MAX_NCO_PRIMARY
	if (found_pmax != MAX_NCO_PRIMARY)
		ib_fprintf(ib_stderr, "Error, real primary max is %d not %d\n",
				   found_pmax, MAX_NCO_PRIMARY);

	if (found_smax != MAX_NCO_SECONDARY)
		ib_fprintf(ib_stderr, "Error, real secondary max is %d not %d\n",
				   found_smax, MAX_NCO_SECONDARY);

	if (found_tmax != MAX_NCO_TERTIARY)
		ib_fprintf(ib_stderr, "Error, real tertiary max is %d not %d\n",
				   found_tmax, MAX_NCO_TERTIARY);
#else
	ib_printf("#define MAX_NCO_PRIMARY\t%d\n", found_pmax);
	ib_printf("#define MAX_NCO_SECONDARY\t%d\n", found_smax);
	ib_printf("#define MAX_NCO_TERTIARY\t%d\n", found_tmax);
#endif
}

void printKey(f, len, pKey)
	 IB_FILE *f;
	 int len;
	 BYTE *pKey;
{
	int i;
	ib_fprintf(f, " <%d>", len);
	for (i = 0; i < len; i++)
		ib_fprintf(f, " %3d", pKey[i]);
}

void do_test_string(str)
	 SCHAR *str;
{
	BYTE key[LANGFAM2_MAX_KEY];
	int len;
	len =
		fam2_string_to_key(debug_obj, strlen(str), str, sizeof(key), key,
						   FALSE);
	printKey(ib_stdout, len, key);
	ib_printf("\n");
	len = fam2_str_to_upper(debug_obj, strlen(str), str, sizeof(key), key);
	key[len] = 0;
	ib_printf("%s\n", key);
}

int byte_compare(l1, s1, l2, s2)
	 int l1, l2;
	 BYTE *s1, *s2;
{
	while (l1 > 0 && l2 > 0) {
		if (*s1 < *s2)
			return (-1);
		if (*s1 > *s2)
			return (1);
		s1++;
		s2++;
		l1--;
		l2--;
	};
	if (l1)
		return (1);
	if (l2)
		return (-1);
	return (0);
}

#define SIGN (x)	(((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))

int compare(l1, s1, l2, s2)
	 int l1, l2;
	 BYTE *s1, *s2;
{
	int res1, res2;
	BYTE key1[LANGFAM2_MAX_KEY], key2[LANGFAM2_MAX_KEY];
	int len1, len2;

	res1 = fam2_compare(debug_obj, l1, s1, l2, s2);
	res2 = fam2_compare(debug_obj, l1, s1, l2, s2);
	if (res1 != res2)
		ib_fprintf(ib_stderr, "compare: '%s' '%s' fails consistancy check\n",
				   s1, s2);
	res2 = fam2_compare(debug_obj, l2, s2, l1, s1);
	if (SIGN(res1) != -SIGN(res2))
		ib_fprintf(ib_stderr,
				   "compare: '%s' '%s' fails reverse arguments test\n", s1,
				   s2);
	len1 = fam2_string_to_key(debug_obj, l1, s1, sizeof(key1), key1, FALSE);
	len2 = fam2_string_to_key(debug_obj, l2, s2, sizeof(key2), key2, FALSE);
	res2 = byte_compare(len1, key1, len2, key2);
	if (SIGN(res1) != SIGN(res2))
		ib_fprintf(ib_stderr,
				   "compare: '%s' '%s' fails compatibility test with key-creation\n",
				   s1, s2);
	return (res1);
}


typedef struct vary {
	USHORT length;
	UCHAR bytes[10];
} VARY;


int is_printable(ch)
	 UCHAR ch;
{
	switch (IB_CHARSET_ID) {
	case CS_LATIN1:
	case CS_ANSI:
		return (!(((ch % 128) < 32) || (ch == 127)));
	case CS_DOS_437:
		return (ch >= 32);
	case CS_DOS_865:
		return (ch >= 32);
	default:
		ib_fprintf(ib_stderr, "Unknown code page# %d\n", IB_CHARSET_ID);
		return (1);
	};
}


UCHAR to_printable(ch)
	 UCHAR ch;
{
	if (!is_printable(ch))
		return ((UCHAR) ' ');
	return (ch);
}

UCHAR *make_printable(ch)
	 UCHAR ch;
{
	static UCHAR buffer[50];
	if (!is_printable(ch)) {
		sprintf(buffer, "<%03d>", ch);
	}
	else {
		buffer[0] = ch;
		buffer[1] = 0;
	}
	return (buffer);
}

void fprint_vary_name(f, v)
	 IB_FILE *f;
	 VARY *v;
{
	int i;
	if (v->length > 1) {
		ib_fprintf(f, "%3d: ", 999);
		for (i = 0; i < v->length; i++)
			ib_fprintf(f, "%s", make_printable(v->bytes[i]));
	}
	else {
		ib_fprintf(f, "%3d:  %c\t\t%s", v->bytes[0],
				   to_printable(v->bytes[0]),
				   to_unicode_map[v->bytes[0]].uniname);
	};

}

void fprint_vary(f, v)
	 IB_FILE *f;
	 VARY *v;
{
	int i;
	for (i = 0; i < v->length; i++)
		ib_fprintf(f, "%s", make_printable(v->bytes[i]));
}

int compare_vary(v1, v2)
	 VARY *v1;
	 VARY *v2;
{
	return (compare(v1->length, v1->bytes, v2->length, v2->bytes));
}

SCHAR fileheader[] =
	"<MIFFile 3.00> # Generated by FrameMaker 3.0\n#\n<TextFlow\n <TFTag `A'>\n <TFAutoConnect Yes >\n <Notes \n > # end of Notes\n";

SCHAR filefooter[] = "> # end of TextFlow\n# End of MIFFile\n";

SCHAR parafooter[] = " >\n > # end of Para\n";

SCHAR paraheader[] = " <Para \n <PgfTag `Body'>\n";
SCHAR parafmthead[] = " <Pgf \n <PgfNumTabs 2>\n";
SCHAR parafmttab[] =
	" <TabStop \n <TSX  %3.1f\">\n <TSType Left >\n <TSLeaderStr ` '>\n > # end of TabStop\n";
SCHAR parafmtfoot[] = " > # end of Pgf\n";
SCHAR paralinehead[] = " <ParaLine \n ";


#if IB_CHARSET_ID == CS_DOS_437
#include "../intl/mac_437.h"
#define to_mac_rom cp437_to_mac_rom
#endif
#if IB_CHARSET_ID == CS_DOS_865
#include "../intl/mac_865.h"
#define to_mac_rom cp865_to_mac_rom
#endif
#if IB_CHARSET_ID == CS_ANSI
#include "../intl/mac_lat1.h"
#define to_mac_rom latin1_to_mac_rom
#endif
#if IB_CHARSET_ID == CS_LATIN1
#include "../intl/mac_lat1.h"
#define to_mac_rom latin1_to_mac_rom
#endif


void output_doc(f, fmt)
	 IB_FILE *f;
	 int fmt;
{
	VARY tests[256 + NUM_EXPAND_CHARS + NUM_COMPRESS_CHARS + 10];
	VARY uppercase;
	int numtests;
	int i;
	int done;
	int have_expand = 0;
	int have_compress = 0;

	numtests = 0;
	for (i = 0; i < 256; i++) {
		tests[i].length = 1;
		tests[i].bytes[0] = i;
		tests[i].bytes[1] = 0;
		numtests++;
		/* only make test cases if the expand/compress tables are used */
		have_expand += NoCaseOrderTbl[i].IsExpand;
		have_compress += NoCaseOrderTbl[i].IsCompress;
	};
	if (have_expand)
		for (i = 0; i < NUM_EXPAND_CHARS; i++) {
			tests[numtests].length = 2;
			tests[numtests].bytes[0] = ExpansionTbl[i].ExpCh1;
			tests[numtests].bytes[1] = ExpansionTbl[i].ExpCh2;
			tests[numtests].bytes[2] = 0;
			numtests++;
		};
	if (have_compress)
		for (i = 0; i < NUM_COMPRESS_CHARS; i++) {
			tests[numtests].length = 2;
			tests[numtests].bytes[0] = CompressTbl[i].CharPair[0];
			tests[numtests].bytes[1] = CompressTbl[i].CharPair[1];
			tests[numtests].bytes[2] = 0;
			numtests++;
		};

	/* just a quick bubble sort */
	done = 0;
	while (!done) {
		done = 1;
		for (i = 0; i < numtests - 1; i++)
			if (compare_vary(&tests[i], &tests[i + 1]) > 0) {
				VARY tmp;
				done = 0;
				tmp = tests[i];
				tests[i] = tests[i + 1];
				tests[i + 1] = tmp;
			};
	};
	/* print out results */
	if (fmt == 1) {
		ib_fprintf(f, "%s\n", ID);
		ib_fprintf(f, "Number of test cases = %d\n", numtests);
		ib_fprintf(f, "----------\n");
		for (i = 0; i < numtests; i++) {
			fprint_vary_name(f, &tests[i]);
			ib_fprintf(f, "\n");
		};
	}
	else if (fmt == 2) {
		ib_fprintf(f, "%s", fileheader);
		for (i = 0; i < numtests; i++) {
			ib_fprintf(f, "%s", paraheader);
			ib_fprintf(f, "%s", parafmthead);
			ib_fprintf(f, parafmttab, 0.5);
			ib_fprintf(f, parafmttab, 1.0);
			ib_fprintf(f, "%s", parafmtfoot);
			ib_fprintf(f, "%s", paralinehead);

/*
   <String `128'>
   <Char Tab >
   <String `\x82 '>
   <Char Tab >
   <String `C cedilla'>
*/
			if (tests[i].length == 1) {
				ib_fprintf(f, "<String `%03d'>\n", tests[i].bytes[0]);
				ib_fprintf(f, "<Char Tab >\n");
				ib_fprintf(f, "<String `\\x%02X '>\n",
						   to_mac_rom[tests[i].bytes[0]]);
				ib_fprintf(f, "<Char Tab >\n");
				ib_fprintf(f, "<String `%s'>\n",
						   to_unicode_map[tests[i].bytes[0]].uniname);
			}
			else {
				ib_fprintf(f, "<String `--'>\n");
				ib_fprintf(f, "<Char Tab >\n");
				ib_fprintf(f, "<String `%s'>\n", /* Mac Rom */
						   tests[i].bytes);
				ib_fprintf(f, "<Char Tab >\n");
				ib_fprintf(f, "<String `--'>\n");
			};
			ib_fprintf(f, "%s", parafooter);
		};
		ib_fprintf(f, "%s", filefooter);
	}
	else
		assert(0);

	if (fmt == 2)
		return;

	ib_fprintf(f, "----------\n");
	ib_fprintf(f, "UPPERCASING\n");
	for (i = 0; i < numtests; i++) {
		uppercase.length =
			fam2_str_to_upper(debug_obj, tests[i].length, tests[i].bytes,
							  sizeof(uppercase.bytes) /
							  sizeof(uppercase.bytes[1]), uppercase.bytes);
		if (byte_compare
			(tests[i].length, tests[i].bytes, uppercase.length,
			 uppercase.bytes)) {
			fprint_vary(f, &tests[i]);
			ib_fprintf(f, " --> ");
			fprint_vary(f, &uppercase);
			ib_fprintf(f, "\n");
		};

	};
}

main(argc, argv)
	 int argc;
	 SCHAR *argv[];
{
	INIT_ENTRY_NAME(IB_INTERBASE_DRIVER_NUMBER, debug_obj);
	if ((argc > 3) || ((argc == 2) && (strcmp(argv[1], "-help") == 0))) {
		ib_fprintf(ib_stderr, "Usage: test <string>  ===> key\n");
		ib_fprintf(ib_stderr, "       test <s1> <s2> ===> iStrCmp(s1,s2)\n");
	}
	else if ((argc == 2) && (strncmp(argv[1], "-doc", 4) == 0)) {
		output_doc(ib_stdout, 1);
	}
	else if ((argc == 2) && (strncmp(argv[1], "-framemaker", 4) == 0)) {
		output_doc(ib_stdout, 2);
	}
	else {
		ib_printf("%s\n", ID);
		if (argc == 1) {
			SCHAR buffer[256];
			while (!ib_feof(ib_stdin)) {
				gets(buffer);
				do_test_string(buffer);
			};
		}
		else if (argc == 2) {
			do_test_string(argv[1]);
		}
		else if (argc == 3) {
			int res;
			res = compare(strlen(argv[1]), argv[1], strlen(argv[2]), argv[2]);
			ib_printf("%s cmp %s => %d\n", argv[1], argv[2], res);
		};
	};
	return (0);
};

#endif	/* DEBUG_LD */


#undef LANGFAM2_MAX_KEY

#undef ASCII_SPACE
#undef NULL_WEIGHT
#undef NULL_SECONDARY
#undef NULL_TERTIARY
