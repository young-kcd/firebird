#include "intl_classes.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "../common/classes/alloc.h"
#include "../jrd/intl.h"
#include "../intl/country_codes.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/err_proto.h"
#include "../intl/charsets.h"


#define TEXTTYPE_RETURN         return (0)

#define FAMILY_INTERNAL(id_number, name, charset, country) \
	cache->texttype_version =               40; \
	cache->texttype_type =                  (id_number); \
	cache->texttype_character_set =         (charset); \
	cache->texttype_country =               (country); \
	cache->texttype_bytes_per_char =        1; \
	cache->texttype_fn_init =               (FPTR_SHORT) (name); \
	cache->texttype_fn_key_length =         (FPTR_SHORT) internal_keylength; \
	cache->texttype_fn_string_to_key =      (FPTR_SHORT) internal_string_to_key; \
	cache->texttype_fn_compare =            (FPTR_short) internal_compare; \
	cache->texttype_fn_to_upper =           (FPTR_SHORT) internal_ch_to_upper; \
	cache->texttype_fn_to_lower =           (FPTR_SHORT) internal_ch_to_lower; \
	cache->texttype_fn_str_to_upper =       (FPTR_short) internal_str_to_upper; \
	cache->texttype_fn_mbtowc =             (FPTR_short) INTL_builtin_nc_mbtowc; \
	cache->texttype_collation_table =       (BYTE *) " "; \
	cache->texttype_toupper_table =         NULL; \
	cache->texttype_tolower_table =         NULL; \
	cache->texttype_compress_table =        NULL; \
	cache->texttype_expand_table =          NULL; \
	cache->texttype_name =                  const_cast<char*>(POSIX);



typedef unsigned char FILECHAR;
typedef USHORT UNICODE;

typedef USHORT fss_wchar_t;
typedef int fss_size_t;

typedef struct {
	int cmask;
	int cval;
	int shift;
	SLONG lmask;
	SLONG lval;
} Tab;

static const Tab tab[] = {
	{ 0x80, 0x00, 0 * 6, 0x7F, 0 },	/* 1 byte sequence */
	{ 0xE0, 0xC0, 1 * 6, 0x7FF, 0x80 },	/* 2 byte sequence */
	{ 0xF0, 0xE0, 2 * 6, 0xFFFF, 0x800 },	/* 3 byte sequence */
	{ 0xF8, 0xF0, 3 * 6, 0x1FFFFF, 0x10000 },	/* 4 byte sequence */
	{ 0xFC, 0xF8, 4 * 6, 0x3FFFFFF, 0x200000 },	/* 5 byte sequence */
	{ 0xFE, 0xFC, 5 * 6, 0x7FFFFFFF, 0x4000000 },	/* 6 byte sequence */
	{ 0, 0, 0, 0, 0 } 				/* end of table    */
};

static fss_size_t fss_mbtowc(fss_wchar_t * p, UCHAR * s, fss_size_t n)
{
	SLONG l;
	int c0, c, nc;
	Tab *t;

	if (s == 0)
		return 0;

	nc = 0;
	if (n <= nc)
		return -1;
	c0 = *s & 0xff;
	l = c0;
	for (t = const_cast < Tab * >(tab); t->cmask; t++) {
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

static fss_size_t fss_wctomb(UCHAR * s, fss_wchar_t wc)
{
	SLONG l;
	int c, nc;
	Tab *t;

	if (s == 0)
		return 0;

	l = wc;
	nc = 0;
	for (t = const_cast < Tab * >(tab); t->cmask; t++) {
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

static SSHORT internal_fss_mbtowc(TEXTTYPE * obj,
						   UCS2_CHAR * wc, NCHAR * p, USHORT n)
{
/**************************************
 *
 *      I N T L _ f s s _ m b t o w c
 *
 **************************************
 *
 * Functional description
 *      InterBase interface to mbtowc function for Unicode
 *      text in FSS bytestream format.
 *
 * Return:      (common to all mbtowc routines)
 *      -1      Error in parsing next character
 *      <n>     Count of characters consumed.
 *      *wc     Next character from byte stream (if wc <> NULL)
 *
 * Note: This routine has a cousin in intl/cv_utffss.c
 *
 **************************************/
	fb_assert(obj);
	fb_assert(wc);
	fb_assert(p);

	return fss_mbtowc(wc, p, n);
}

static USHORT internal_fss_to_unicode(CSCONVERT obj,
							   UNICODE * dest_ptr, USHORT dest_len,	/* BYTE count */
							   NCHAR * src_ptr,
							   USHORT src_len,
							   SSHORT * err_code,
							   USHORT * err_position)
{
	UNICODE *start;
	USHORT src_start = src_len;
	fss_size_t res;

	fb_assert(src_ptr != NULL || dest_ptr == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);

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
		fb_assert(res <= src_len);
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

USHORT internal_unicode_to_fss(CSCONVERT obj,
									  MBCHAR * fss_str,
									  USHORT fss_len,
									  UNICODE * unicode_str, USHORT unicode_len,	/* BYTE count */
									  SSHORT * err_code,
									  USHORT * err_position)
{
	UCHAR *start;
	USHORT src_start = unicode_len;
	UCHAR tmp_buffer[6];
	UCHAR *p;
	fss_size_t res;

	fb_assert(unicode_str != NULL || fss_str == NULL);
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);
	fb_assert(obj != NULL);
	fb_assert(obj->csconvert_convert == (FPTR_SHORT) internal_unicode_to_fss);

	*err_code = 0;

/* See if we're only after a length estimate */
	if (fss_str == NULL)
		return ((unicode_len + 1) / 2 * 3);	/* worst case - all han character input */

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

static SSHORT internal_str_copy(
								TEXTTYPE obj,
								USHORT inLen,
								UCHAR * src, USHORT outLen, UCHAR * dest)
{
/**************************************
 *
 *      i n t e r n a l _ s t r _ c o p y
 *
 **************************************
 *
 * Functional description
 *      Note: dest may equal src.
 *
 **************************************/
	UCHAR *pStart;

	pStart = dest;
	while (inLen-- && outLen--) {
		*dest++ = *src++;
	}

	return (dest - pStart);
}

static USHORT internal_keylength(TEXTTYPE obj, USHORT iLength)
{
/**************************************
 *
 *      i n t e r n a l _ k e y l e n g t h
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return (iLength);
}

static USHORT internal_string_to_key(
									 TEXTTYPE obj,
									 USHORT inLen,
									 const UCHAR* src,
									 USHORT outLen,
	UCHAR* dest, USHORT partial)
{
/**************************************
 *
 *      i n t e r n a l _ s t r i n g _ t o _ k e y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *pStart;
	UCHAR pad_char;

	pStart = dest;
	pad_char = *obj->texttype_collation_table;
	while (inLen-- && outLen--)
		*dest++ = *src++;

/* strip off ending pad characters */
	while (dest > pStart)
		if (*(dest - 1) == pad_char)
			dest--;
		else
			break;

	return (dest - pStart);
}

static SSHORT internal_compare(
							   TEXTTYPE obj,
							   USHORT length1,
							   UCHAR * p1, USHORT length2, UCHAR * p2)
{
/**************************************
 *
 *      i n t e r n a l _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SSHORT fill;
	UCHAR pad;

	pad = (obj->texttype_type == ttype_binary) ? 0 : ' ';
	fill = length1 - length2;
	if (length1 >= length2) {
		if (length2)
			do
				if (*p1++ != *p2++)
					if (p1[-1] > p2[-1])
						return 1;
					else
						return -1;
			while (--length2);
		if (fill > 0)
			do
				if (*p1++ != pad)
					if (p1[-1] > pad)
						return 1;
					else
						return -1;
			while (--fill);
		return 0;
	}

	if (length1)
		do
			if (*p1++ != *p2++)
				if (p1[-1] > p2[-1])
					return 1;
				else
					return -1;
		while (--length1);

	do
		if (*p2++ != pad)
			if (pad > p2[-1])
				return 1;
			else
				return -1;
	while (++fill);

	return 0;
}

static USHORT internal_ch_to_upper(TEXTTYPE obj, UCHAR ch)
{
/**************************************
 *
 *      i n t e r n a l _ c h _ t o _ u p p e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return (UPPER7(ch));
}

static USHORT internal_ch_to_lower(TEXTTYPE obj, UCHAR ch)
{
/**************************************
 *
 *      i n t e r n a l _ c h _ t o _ l o w e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return ((((ch) >= 'A') && ((ch) < 'Z')) ? ((ch) - 'A' + 'a') : (ch));
}

SSHORT INTL_builtin_nc_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR * wc, UCHAR * ptr, USHORT count)
{
/**************************************
 *
 *      i n t e r n a l _ n c _ m b t o w c 
 *
 **************************************
 *
 * Functional description
 *      Get the next character from the multibyte
 *      input stream.
 *      Narrow character version.
 *  Returns:
 *      Count of bytes consumed from the input stream.
 *
 **************************************/

	fb_assert(obj);
	fb_assert(ptr);

	if (count >= 1) {
		if (wc)
			*wc = *ptr;
		return 1;
	}
	if (wc)
		*wc = 0;
	return -1;					/* No more characters */
}

SSHORT INTL_builtin_mb_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR * wc, UCHAR * ptr, USHORT count)
{
/**************************************
 *
 *      i n t e r n a l _ m b _ m b t o w c 
 *
 **************************************
 *
 * Functional description
 *      Get the next character from the multibyte
 *      input stream.
 *      Multibyte version character version.
 *  Returns:
 *      Count of bytes consumed from the input stream.
 *
 **************************************/

	fb_assert(obj);
	fb_assert(ptr);

	if (count >= 2) {
		if (wc)
			*wc = *(UCS2_CHAR *) ptr;
		return 2;
	}
	if (wc)
		*wc = 0;
	return -1;					/* No more characters */
}


SSHORT INTL_builtin_wc_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR * wc, UCHAR * ptr, USHORT count)
{
/**************************************
 *
 *      i n t e r n a l _ w c _ m b t o w c 
 *
 **************************************
 *
 * Functional description
 *      Get the next character from the multibyte
 *      input stream.
 *      Wide character version.
 *  Returns:
 *      Count of bytes consumed from the input stream.
 *
 **************************************/

	fb_assert(obj);
	fb_assert(ptr);

	if (count >= 2) {
		if (wc)
			*wc = *(UCS2_CHAR *) ptr;
		return 2;
	}
	if (wc)
		*wc = 0;
	return -1;					/* No more characters */
}

static SSHORT internal_str_to_upper(
									TEXTTYPE obj,
									USHORT inLen,
									UCHAR * src, USHORT outLen, UCHAR * dest)
{
/**************************************
 *
 *      i n t e r n a l _ s t r _ t o _ u p p e r
 *
 **************************************
 *
 * Functional description
 *      Note: dest may equal src.
 *
 **************************************/
	UCHAR *pStart;
	pStart = dest;
	while (inLen-- && outLen--) {
		*dest++ = UPPER7(*src);
		src++;
	}

	return (dest - pStart);
}

static USHORT internal_ch_copy(TEXTTYPE obj, UCHAR ch)
{
/**************************************
 *
 *      i n t e r n a l _ c h _ c o p y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return (ch);
}

static USHORT wc_to_nc(CSCONVERT obj, NCHAR * pDest, USHORT nDest,	/* byte count */
					   UCS2_CHAR * pSrc, USHORT nSrc,	/* byte count */
					   SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      w c _ t o _ n c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	NCHAR *pStart;
	UCS2_CHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return ((nSrc + 1) / 2);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest && nSrc >= sizeof(*pSrc)) {
		if (*pSrc >= 256) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static USHORT mb_to_wc(CSCONVERT obj, UCS2_CHAR * pDest, USHORT nDest,	/* byte count */
					   MBCHAR * pSrc, USHORT nSrc,	/* byte count */
					   SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      m b _ t o _ w c
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string from network form - high-endian
 *      byte stream.
 *
 *************************************/
	UCS2_CHAR *pStart;
	MBCHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest > 1 && nSrc > 1) {
		*pDest++ = *pSrc * 256 + *(pSrc + 1);
		pSrc += 2;
		nDest -= 2;
		nSrc -= 2;
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}


static USHORT wc_to_mb(CSCONVERT obj, MBCHAR * pDest, USHORT nDest,	/* byte count */
					   UCS2_CHAR * pSrc, USHORT nSrc,	/* byte count */
					   SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      w c _ t o _ m b 
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string to network form - high-endian
 *      byte stream.
 *
 *************************************/
	MBCHAR *pStart;
	UCS2_CHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);
	fb_assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest > 1 && nSrc > 1) {
		*pDest++ = *pSrc / 256;
		*pDest++ = *pSrc++ % 256;
		nDest -= 2;
		nSrc -= 2;
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static USHORT ttype_ascii_init(TEXTTYPE cache, USHORT parm1, USHORT dummy)
{
/**************************************
 *
 *      t t y p e _ a s c i i _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.ASCII";

	FAMILY_INTERNAL(ttype_ascii, ttype_ascii_init, CS_ASCII, CC_C);

	TEXTTYPE_RETURN;
}


static USHORT ttype_none_init(TEXTTYPE cache, USHORT parm1, USHORT dummy)
{
/**************************************
 *
 *      t t y p e _ n o n e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C";

	FAMILY_INTERNAL(ttype_none, ttype_none_init, CS_NONE, CC_C);

	TEXTTYPE_RETURN;
}


static USHORT ttype_unicode_fss_init(
									 TEXTTYPE cache,
									 USHORT parm1, USHORT dummy)
{
/**************************************
 *
 *      t t y p e _ u n i c o d e _ f s s _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.UNICODE_FSS";

	FAMILY_INTERNAL(ttype_unicode_fss, ttype_unicode_fss_init, CS_UNICODE_FSS,
					CC_C);
	cache->texttype_bytes_per_char = 3;
	cache->texttype_fn_to_wc = (FPTR_SHORT) internal_fss_to_unicode;
	cache->texttype_fn_mbtowc = (FPTR_short) internal_fss_mbtowc;

	TEXTTYPE_RETURN;
}


static USHORT ttype_binary_init(TEXTTYPE cache, USHORT parm1, USHORT dummy)
{
/**************************************
 *
 *      t t y p e _ b i n a r y _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const ASCII POSIX[] = "C.OCTETS";

	FAMILY_INTERNAL(ttype_binary, ttype_binary_init, CS_BINARY, CC_C);
	cache->texttype_fn_to_upper = (FPTR_SHORT) internal_ch_copy;
	cache->texttype_fn_to_lower = (FPTR_SHORT) internal_ch_copy;
	cache->texttype_fn_str_to_upper = (FPTR_short) internal_str_copy;
	cache->texttype_collation_table = (BYTE *) "\0";	/* pad character */

	TEXTTYPE_RETURN;
}


/*
 *      Start of Character set definitions 
 */

#define CHARSET_RETURN          return (0)

static void common_8bit_init(
							 CHARSET csptr,
							 USHORT id,
							 ASCII * name,
							 BYTE * to_unicode_tbl,
							 BYTE * from_unicode_tbl1,
BYTE * from_unicode_tbl2)
{
/**************************************
 *
 *      c o m m o n _ 8 b i t _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	csptr->charset_version = 40;
	csptr->charset_id = id;
	csptr->charset_name = name;
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 1;
	csptr->charset_max_bytes_per_char = 1;
	csptr->charset_space_length = 1;
	csptr->charset_space_character = (BYTE *) " ";
	csptr->charset_well_formed = NULL;
}


static void common_convert_init(
								CSCONVERT csptr,
								USHORT to_cs,
								USHORT from_cs,
								FPTR_SHORT cvt_fn,
BYTE * datatable, BYTE * datatable2)
{
/**************************************
 *
 *      c o m m o n _ c o n v e r t _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	csptr->csconvert_version = 40;
	csptr->csconvert_name = (ASCII *) "DIRECT";
	csptr->csconvert_from = from_cs;
	csptr->csconvert_to = to_cs;
	csptr->csconvert_convert = cvt_fn;
	csptr->csconvert_datatable = datatable;
	csptr->csconvert_misc = datatable2;
}

static USHORT cvt_ascii_to_unicode(CSCONVERT obj, UCS2_CHAR * pDest, USHORT nDest,	/* byte count */
								   UCHAR * pSrc, USHORT nSrc,	/* byte count */
								   SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      c v t _ a s c i i _ t o _ u n i c o d e
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create BAD_INPUT
 *
 *************************************/
	UCS2_CHAR *pStart;
	UCHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (2 * nSrc);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_BAD_INPUT;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static USHORT cvt_unicode_to_ascii(CSCONVERT obj, NCHAR * pDest, USHORT nDest,	/* byte count */
								   UCS2_CHAR * pSrc, USHORT nSrc,	/* byte count */
								   SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      c v t _ u n i c o d e _ t o _ a s c i i 
 *
 **************************************
 *
 * Functional description
 *      Convert UNICODE to CHARACTER SET ASCII (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create CONVERT_ERROR
 *
 *************************************/
	NCHAR *pStart;
	UCS2_CHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc / 2);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static USHORT cvt_none_to_unicode(CSCONVERT obj, UCS2_CHAR * pDest, USHORT nDest,	/* byte count */
								  UCHAR * pSrc, USHORT nSrc,	/* byte count */
								  SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      c v t _ n o n e _ t o _ u n i c o d e
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create CONVERT ERROR
 *
 *************************************/
	UCS2_CHAR *pStart;
	UCHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (2 * nSrc);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static USHORT cvt_utffss_to_ascii(CSCONVERT obj, UCHAR * pDest, USHORT nDest,	/* byte count */
								  UCHAR * pSrc, USHORT nSrc,	/* byte count */
								  SSHORT * err_code, USHORT * err_position)
{
/**************************************
 *
 *      c v t _ u t f f s s _ t o _ a s c i i 
 * also
 *      c v t _ a s c i i _ t o _ u t f f s s 
 * also
 *      c v t _ n o n e _ t o _ u t f f s s 
 *
 **************************************
 *
 * Functional description
 *      Perform a pass-through transformation of ASCII to Unicode
 *      in FSS format.  Note that any byte values greater than 127
 *      cannot be converted in either direction, so the same
 *      routine does double duty.
 *
 *************************************/
	UCHAR *pStart;
	UCHAR *pStart_src;

	fb_assert(obj != NULL);
	fb_assert((pSrc != NULL) || (pDest == NULL));
	fb_assert(err_code != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);
	pStart = pDest;
	pStart_src = pSrc;
	while (nDest >= sizeof(*pDest) && nSrc >= sizeof(*pSrc)) {
		if (*pSrc > 127) {
			/* In the cvt_ascii_to_utffss case this should be CS_BAD_INPUT */
			/* but not in cvt_none_to_utffss or cvt_utffss_to_ascii */
			*err_code = CS_CONVERT_ERROR;
			break;
		}
		*pDest++ = *pSrc++;
		nDest -= sizeof(*pDest);
		nSrc -= sizeof(*pSrc);
	}
	if (!*err_code && nSrc) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	*err_position = (pSrc - pStart_src) * sizeof(*pSrc);

	return ((pDest - pStart) * sizeof(*pDest));
}

static USHORT cs_ascii_init(CHARSET csptr, USHORT cs_id, USHORT dummy)
{
/**************************************
 *
 *      c s _ a s c i i _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, CS_ASCII, (ASCII *) "ASCII", NULL, NULL, NULL);
	common_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2, CS_ASCII,
						(FPTR_SHORT) cvt_ascii_to_unicode, NULL, NULL);
	common_convert_init(&csptr->charset_from_unicode, CS_ASCII, CS_UNICODE_UCS2,
						(FPTR_SHORT) cvt_unicode_to_ascii, NULL, NULL);
	CHARSET_RETURN;
}


static USHORT cs_none_init(CHARSET csptr, USHORT cs_id, USHORT dummy)
{
/**************************************
 *
 *      c s _ n o n e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, CS_NONE, (ASCII *) "NONE", NULL, NULL, NULL);
/*
common_convert_init (&csptr->charset_to_unicode, CS_UNICODE_UCS2, id,
	nc_to_wc, to_unicode_tbl, NULL);
*/
	common_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2, CS_NONE,
						(FPTR_SHORT) cvt_none_to_unicode, NULL, NULL);
	common_convert_init(&csptr->charset_from_unicode, CS_NONE, CS_UNICODE_UCS2,
						(FPTR_SHORT) wc_to_nc, NULL, NULL);
	CHARSET_RETURN;
}


static USHORT cs_unicode_fss_init(CHARSET csptr, USHORT cs_id, USHORT dummy)
{
/**************************************
 *
 *      c s _ u n i c o d e _ f s s _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, CS_UNICODE_FSS, (ASCII *) "UNICODE_FSS", NULL,
					 NULL, NULL);
	common_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2,
						CS_UNICODE_FSS, (FPTR_SHORT) internal_fss_to_unicode,
						NULL, NULL);
	common_convert_init(&csptr->charset_from_unicode, CS_UNICODE_FSS,
						CS_UNICODE_UCS2, (FPTR_SHORT) internal_unicode_to_fss, NULL,
						NULL);
	CHARSET_RETURN;
}


static USHORT cs_unicode_ucs2_init(CHARSET csptr, USHORT cs_id, USHORT dummy)
{
/**************************************
 *
 *      c s _ u n i c o d e _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/
	static const UCS2_CHAR space = 0x0020;

	csptr->charset_version = 40;
	csptr->charset_id = CS_UNICODE_UCS2;
	csptr->charset_name = "UNICODE_UCS2";
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 2;
	csptr->charset_max_bytes_per_char = 2;
	csptr->charset_space_length = 2;
	csptr->charset_space_character = (BYTE *) & space;	/* 0x0020 */
	csptr->charset_well_formed = NULL;
	CHARSET_RETURN;
}


static USHORT cs_binary_init(CHARSET csptr, USHORT cs_id, USHORT dummy)
{
/**************************************
 *
 *      c s _ b i n a r y _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_8bit_init(csptr, CS_BINARY, (ASCII *) "BINARY", NULL, NULL, NULL);
	csptr->charset_space_character = (BYTE *) "\0";
	common_convert_init(&csptr->charset_to_unicode, CS_UNICODE_UCS2, CS_BINARY,
						(FPTR_SHORT) mb_to_wc, NULL, NULL);
	common_convert_init(&csptr->charset_from_unicode, CS_BINARY,
						CS_UNICODE_UCS2, (FPTR_SHORT) wc_to_mb, NULL, NULL);
	CHARSET_RETURN;
}


/*
 *      Start of Conversion entries
 */

#define CONVERT_RETURN  return (0)


static USHORT cvt_ascii_utf_init(
								 CSCONVERT csptr,
								 USHORT dest_cs, USHORT source_cs)
{
/**************************************
 *
 *      c v t _ a s c i i _ u t f _ i n i t
 *
 **************************************
 *
 * Functional description
 *
 *************************************/

	common_convert_init(csptr, dest_cs, source_cs,
						(FPTR_SHORT) cvt_utffss_to_ascii, NULL, NULL);
	CONVERT_RETURN;
}

FPTR_SHORT INTL_builtin_lookup(USHORT objtype, SSHORT parm1, SSHORT parm2) {
	switch (objtype) {
	case type_texttype:
		if (parm1 == ttype_none)
			return (FPTR_SHORT)ttype_none_init;
		if (parm1 == ttype_ascii)
			return (FPTR_SHORT)ttype_ascii_init;
		if (parm1 == ttype_unicode_fss)
			return (FPTR_SHORT)ttype_unicode_fss_init;
		if (parm1 == ttype_binary)
			return (FPTR_SHORT)ttype_binary_init;
		break;
	case type_charset:
		if (parm1 == CS_NONE)
			return (FPTR_SHORT)cs_none_init;
		if (parm1 == CS_ASCII)
			return (FPTR_SHORT)cs_ascii_init;
		if (parm1 == CS_UNICODE_FSS)
			return (FPTR_SHORT)cs_unicode_fss_init;
		if (parm1 == CS_UNICODE_UCS2)
			return (FPTR_SHORT)cs_unicode_ucs2_init;
		if (parm1 == CS_BINARY)
			return (FPTR_SHORT)cs_binary_init;
		break;
	case type_csconvert:
		if (((parm1 == CS_ASCII) && (parm2 == CS_UNICODE_FSS)) ||
			((parm2 == CS_ASCII) && (parm1 == CS_UNICODE_FSS)))
			return (FPTR_SHORT)cvt_ascii_utf_init;

		/* converting FROM NONE to UNICODE has a short cut 
		 * - it's treated like ASCII */
		if ((parm2 == CS_NONE) && (parm1 == CS_UNICODE_FSS))
			return (FPTR_SHORT)cvt_ascii_utf_init;

#ifdef DEV_BUILD
		/* Converting TO character set NONE should have been handled at
		 * a higher level
		 */
		fb_assert(parm1 != CS_NONE);
#endif
		break;
	default:
		BUGCHECK(1);
		break;
	}
	return NULL;
}

