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
#include "lc_narrow.h"

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
USHORT LC_NARROW_key_length(TEXTTYPE obj, USHORT inLen)
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
USHORT LC_NARROW_string_to_key(TEXTTYPE obj, USHORT iInLen, BYTE *pInChar, USHORT iOutLen, BYTE *pOutChar,
							   USHORT partial)
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
	SortOrderTblEntry* coll;

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
			&((SortOrderTblEntry*) obj->
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
			ExpandChar* exp = &((ExpandChar*) obj->texttype_expand_table)[0];
			while (exp->Ch && exp->Ch != *pInChar)
				exp++;
			assert(exp->Ch == *pInChar);
			for (j = 0; j < 2; j++) {
				if (j)
					coll =
						&((SortOrderTblEntry*) obj->
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

			CompressPair* cmp;
			if ((USHORT) (i + 1) < iInLen) {
				cmp =
					&((CompressPair*) obj->
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
	SortOrderTblEntry* stat_waiting;
} *COLSTAT;




static SSHORT special_scan(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
{
	SortOrderTblEntry* col1;
	SortOrderTblEntry* col2;
	USHORT index1, index2;

	index1 = 0;
	index2 = 0;

	while (1) {
		/* Scan to find ignore char from l1 */
		while (l1) {
			col1 =
				&((SortOrderTblEntry*) obj->
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
				&((SortOrderTblEntry*) obj->
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


SortOrderTblEntry* get_coltab_entry(TEXTTYPE obj, UCHAR **p, USHORT *l, COLSTAT stat)
{
	SortOrderTblEntry* col;

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
			&((SortOrderTblEntry*) obj->
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
			ExpandChar* exp = &((ExpandChar*) obj->texttype_expand_table)[0];
			while (exp->Ch && exp->Ch != **p)
				exp++;
			assert(exp->Ch == **p);
			/* Have coll1 */
			/* Have waiting */

			stat->stat_waiting =
				&((SortOrderTblEntry*) obj->
				  texttype_collation_table)[exp->ExpCh2];
			stat->stat_flags |= HAVE_WAITING;
			return col;
		}
		else {					/* (col->IsCompress) */

			CompressPair* cmp;
			if (*l > 1) {
				cmp =
					&((CompressPair*) obj->
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

SSHORT LC_NARROW_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
{
	SortOrderTblEntry* col1;
	SortOrderTblEntry* col2;
	coltab_status stat1, stat2;
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
static SSHORT old_fam2_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
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
static SSHORT fam2_compare(TEXTTYPE obj, USHORT l1, BYTE *s1, USHORT l2, BYTE *s2)
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



#undef LANGFAM2_MAX_KEY

#undef ASCII_SPACE
#undef NULL_WEIGHT
#undef NULL_SECONDARY
#undef NULL_TERTIARY
