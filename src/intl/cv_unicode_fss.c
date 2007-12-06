/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		cs_utffss.c
 *	DESCRIPTION:	Character set definition for Unicode FSS format
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

 /* Note: all routines have cousins in jrd/intl.c */

#include "firebird.h"
#include "../intl/ldcommon.h"
#include "cv_unicode_fss.h"

typedef USHORT fss_wchar_t;
typedef SLONG fss_size_t;

static fss_size_t fss_mbtowc( fss_wchar_t * p, NCHAR *s, fss_size_t n);
static fss_size_t fss_wctomb(MBCHAR *s, fss_wchar_t wc);

SSHORT CS_UTFFSS_fss_mbtowc(TEXTTYPE *obj, UCS2_CHAR *wc, NCHAR *p, USHORT n)
{
/**************************************
 *
 *	I N T L _ f s s _ m b t o w c
 *
 **************************************
 *
 * Functional description
 *	InterBase interface to mbtowc function for Unicode
 *	text in FSS bytestream format.
 *
 * Return:	(common to all mbtowc routines)
 *	-1	Error in parsing next character
 *	<n>	Count of characters consumed.
 *	*wc	Next character from byte steam (if wc <> NULL)
 *
 * Note: This routine has a cousin in jrd/intl.c
 *
 **************************************/
	assert(obj);
	assert(wc);
	assert(p);

	return fss_mbtowc(wc, p, n);
}



/*
 * The following was provided by Ken Thompson of AT&T Bell Laboratories,
 * <ken@research.att.com>, on Tue, 8 Sep 92 03:22:07 EDT, to the X/Open
 * Joint Internationalization Group.  Some minor formatting changes have
 * been made by Glenn Adams, <glenn@metis.com>.
 *
 * -------------------------------------------------------------------------
 *  File System Safe Universal Character Set Transformation Format (FSS-UTF)
 * -------------------------------------------------------------------------
 *
 * With the approval of ISO/IEC 10646 (Unicode) as an international
 * standard and the anticipated wide spread use of this universal coded
 * character set (UCS), it is necessary for historically ASCII based
 * operating systems to devise ways to cope with representation and
 * handling of the large number of characters that are possible to be
 * encoded by this new standard.
 *
 * There are several challenges presented by UCS which must be dealt with
 * by historical operating systems and the C-language programming
 * environment.  The most significant of these challenges is the encoding
 * scheme used by UCS. More precisely, the challenge is the marrying of
 * the UCS standard with existing programming languages and existing
 * operating systems and utilities.
 *
 * The challenges of the programming languages and the UCS standard are
 * being dealt with by other activities in the industry.  However, we are
 * still faced with the handling of UCS by historical operating systems
 * and utilities.  Prominent among the operating system UCS handling
 * concerns is the representation of the data within the file system.  An
 * underlying assumption is that there is an absolute requirement to
 * maintain the existing operating system software investment while at
 * the same time taking advantage of the use the large number of
 * characters provided by the UCS.
 *
 * UCS provides the capability to encode multi-lingual text within a
 * single coded character set.  However, UCS and its UTF variant do not
 * protect null bytes and/or the ASCII slash ("/") making these character
 * encodings incompatible with existing Unix implementations.  The
 * following proposal provides a Unix compatible transformation format of
 * UCS such that Unix systems can support multi-lingual text in a single
 * encoding.  This transformation format encoding is intended to be used
 * as a file code.  This transformation format encoding of UCS is
 * intended as an intermediate step towards full UCS support.  However,
 * since nearly all Unix implementations face the same obstacles in
 * supporting UCS, this proposal is intended to provide a common and
 * compatible encoding during this transition stage.
 *
 * Goal/Objective
 * --------------
 *
 * With the assumption that most, if not all, of the issues surrounding
 * the handling and storing of UCS in historical operating system file
 * systems are understood, the objective is to define a UCS
 * transformation format which also meets the requirement of being usable
 * on a historical operating system file system in a non-disruptive
 * manner.  The intent is that UCS will be the process code for the
 * transformation format, which is usable as a file code.
 *
 * Criteria for the Transformation Format
 * --------------------------------------
 *
 * Below are the guidelines that were used in defining the UCS
 * transformation format:
 *
 *	1) Compatibility with historical file systems:
 *
 *	Historical file systems disallow the null byte and the ASCII
 *	slash character as a part of the file name.
 *
 *	2) Compatibility with existing programs:
 *
 *	The existing model for multibyte processing is that ASCII does
 *	not occur anywhere in a multibyte encoding.  There should be
 *	no ASCII code values for any part of a transformation format
 *	representation of a character that was not in the ASCII
 *	character set in the UCS representation of the character.
 *
 *	3) Ease of conversion from/to UCS.
 *
 *	4) The first byte should indicate the number of bytes to
 *	follow in a multibyte sequence.
 *
 *	5) The transformation format should not be extravagant in
 *	terms of number of bytes used for encoding.
 *
 *	6) It should be possible to find the start of a character
 *	efficiently starting from an arbitrary location in a byte
 *	stream.
 *
 * Proposed FSS-UTF
 * ----------------
 *
 * The proposed UCS transformation format encodes UCS values in the range
 * [0,0x7fffffff] using multibyte characters of lengths 1, 2, 3, 4, 5,
 * and 6 bytes.  For all encodings of more than one byte, the initial
 * byte determines the number of bytes used and the high-order bit in
 * each byte is set.  Every byte that does not start 10xxxxxx is the
 * start of a UCS character sequence.
 *
 * An easy way to remember this transformation format is to note that the
 * number of high-order 1's in the first byte signifies the number of
 * bytes in the multibyte character:
 *
 * Bits  Hex Min  Hex Max  Byte Sequence in Binary
 *   7  00000000 0000007f 0vvvvvvv
 *  11  00000080 000007FF 110vvvvv 10vvvvvv
 *  16  00000800 0000FFFF 1110vvvv 10vvvvvv 10vvvvvv
 *  21  00010000 001FFFFF 11110vvv 10vvvvvv 10vvvvvv 10vvvvvv
 *  26  00200000 03FFFFFF 111110vv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
 *  31  04000000 7FFFFFFF 1111110v 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv 10vvvvvv
 *
 * The UCS value is just the concatenation of the v bits in the multibyte
 * encoding.  When there are multiple ways to encode a value, for example
 * UCS 0, only the shortest encoding is legal.
 *
 * Below are sample implementations of the C standard wctomb() and
 * mbtowc() functions which demonstrate the algorithms for converting
 * from UCS to the transformation format and converting from the
 * transformation format to UCS. The sample implementations include error
 * checks, some of which may not be necessary for conformance:
 *
 */



static fss_size_t fss_mbtowc( fss_wchar_t * p, NCHAR *s, fss_size_t n)
{
	long l;
	int c0, c, nc;
	const Tab *t;

	if (s == 0)
		return 0;

	nc = 0;
	if (n <= nc)
		return -1;
	c0 = *s & 0xff;
	l = c0;
	for (t = tab; t->cmask; t++) {
		nc++;
		if ((c0 & t->cmask) == t->cval) {
			l &= t->lmask;
			if (l < t->lval)
				return -1;
			*p = l;
			return nc;
		}
		if (n <= nc)
			return -1;
		s++;
		c = (*s ^ 0x80) & 0xFF;
		if (c & 0xC0)
			return -1;
		l = (l << 6) | c;
	}
	return -1;
}


static fss_size_t fss_wctomb(MBCHAR *s, fss_wchar_t wc)
{
	long l;
	int c, nc;
	const Tab *t;

	if (s == 0)
		return 0;

	l = wc;
	nc = 0;
	for (t = tab; t->cmask; t++) {
		nc++;
		if (l <= t->lmask) {
			c = t->shift;
			*s = t->cval | (l >> c);
			while (c > 0) {
				c -= 6;
				s++;
				*s = 0x80 | ((l >> c) & 0x3F);
			}
			return nc;
		}
	}
	return -1;
}


USHORT CS_UTFFSS_fss_to_unicode(CSCONVERT obj, UNICODE *dest_ptr, USHORT dest_len, NCHAR *src_ptr
								, USHORT src_len, SSHORT *err_code, USHORT *err_position)
{
	UNICODE *start;
	USHORT src_start = src_len;
	fss_size_t res;

	assert(src_ptr != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CS_UTFFSS_fss_to_unicode ||
		   ((TEXTTYPE) obj)->texttype_fn_to_wc == (FPTR_SHORT) CS_UTFFSS_fss_to_unicode);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len * 2);	/* All single byte narrow characters */

	start = dest_ptr;
	src_start = src_len;
	while ((src_len) && (dest_len >= sizeof(*dest_ptr))) {
		res = fss_mbtowc(dest_ptr, src_ptr, src_len);
		if (res == -1) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		assert(res <= src_len);
		dest_ptr++;
		dest_len -= sizeof(*dest_ptr);
		src_ptr += res;
		src_len -= res;
	}
	if (src_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - src_len;
	return ((dest_ptr - start) * sizeof(*dest_ptr));
}


USHORT CS_UTFFSS_unicode_to_fss(CSCONVERT obj, MBCHAR *fss_str, USHORT fss_len, UNICODE *unicode_str,
								USHORT unicode_len, SSHORT *err_code, USHORT *err_position)
{
	MBCHAR *start;
	USHORT src_start = unicode_len;
	MBCHAR tmp_buffer[6];
	MBCHAR *p;
	fss_size_t res;

	assert(unicode_str != NULL || fss_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);
	assert(obj != NULL);
	assert(obj->csconvert_convert == (FPTR_SHORT) CS_UTFFSS_unicode_to_fss);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (fss_str == NULL)
		return ((USHORT) (unicode_len + 1) / 2 * 3);	/* worst case - all han character input */

	start = fss_str;
	while ((fss_len) && (unicode_len >= sizeof(*unicode_str))) {
		/* Convert the wide character into temp buffer */
		res = fss_wctomb(tmp_buffer, *unicode_str);
		if (res == -1) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		/* will the mb sequence fit into space left? */
		if (res > fss_len) {
			*err_code = CS_TRUNCATION_ERROR;
			break;
		}
		/* copy the converted bytes into the destination */
		p = tmp_buffer;
		for (; res; res--, fss_len--)
			*fss_str++ = *p++;
		unicode_len -= sizeof(*unicode_str);
		unicode_str++;
	}
	if (unicode_len && !*err_code) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = src_start - unicode_len;
	return ((fss_str - start) * sizeof(*fss_str));
}
