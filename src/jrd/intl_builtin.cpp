#include "intl_classes.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include "../common/classes/alloc.h"
#include "../jrd/intl.h"
#include "../intl/country_codes.h"
#include "../jrd/gdsassert.h"


// types unsed internal to this file
typedef USHORT fss_wchar_t;
typedef int fss_size_t;
typedef unsigned char FILECHAR;   

// internal functions
static fss_size_t fss_wctomb(UCHAR *, fss_wchar_t);
static fss_size_t fss_mbtowc(fss_wchar_t *, UCHAR *, fss_size_t);
static unsigned short cvt_utffss_to_ascii(UCHAR *, USHORT, UCHAR *, USHORT,
								  short*, USHORT *);
static USHORT internal_string_to_key(USHORT, UCHAR *, USHORT,
									 UCHAR *, USHORT, UCHAR);
static SSHORT internal_compare(USHORT, UCHAR *, USHORT, UCHAR *, UCHAR);
static SSHORT internal_str_to_upper(USHORT, UCHAR *, USHORT,
									UCHAR *);
static unsigned short internal_unicode_to_fss(
							unsigned char *,
							unsigned short,
							unsigned char *,
							unsigned short ,
							short *,
							unsigned short *);
static USHORT  internal_fss_to_unicode (
    UCS2_CHAR*,
    USHORT,
    NCHAR*,
    USHORT,
    SSHORT*,
    USHORT*);
/********************************************************************************/
// This module contains the code to handle the default international character
// sets.  As such it defines its own subclasses of the international interface.
// The classes are defined in this section and implemented twords the end of the
// file.


class TextType_Binary : public TextTypeNC
{
public:
	static TextType *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) TextType_Binary; }
	TextType_Binary() : TextTypeNC(ttype_binary, "C.OCTETS", CS_BINARY, CC_C, 1)
		{}

	unsigned short key_length(unsigned short len)
		{ return len; }
	unsigned short string_to_key(unsigned short a,
										 unsigned char *b,
										 unsigned short c,
										 unsigned char *d,
										 unsigned short e)
		{ return internal_string_to_key(a,b,c,d,e,'\0'); }
	
	short compare(unsigned short a,
						  unsigned char *b,
						  unsigned short c,
						  unsigned char *d)
		{ return internal_compare(a,b,c,d,'\0'); }
	
	unsigned short to_upper(unsigned short ch)
		{ return (ch); }
	
	unsigned short to_lower(unsigned short ch)
		{ return (ch); }
	
	short str_to_upper(unsigned short,
							   unsigned char *,
							   unsigned short,
							   unsigned char *);
};

class TextType_UFSS : public TextTypeMB
{
public:
	static TextType *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) TextType_UFSS; }
	TextType_UFSS() : TextTypeMB(ttype_unicode_fss, "C.UNICODE_FSS", CS_UNICODE_FSS, CC_C, 3)
		{}

	unsigned short key_length(unsigned short len)
		{ return len; }
	unsigned short string_to_key(unsigned short a,
										 unsigned char *b,
										 unsigned short c,
										 unsigned char *d,
										 unsigned short e)
		{ return internal_string_to_key(a,b,c,d,e,' '); }
	
	short compare(unsigned short a,
						  unsigned char *b,
						  unsigned short c,
						  unsigned char *d)
		{ return internal_compare(a,b,c,d,' '); }
	
	unsigned short to_upper(unsigned short ch)
		{ return (UPPER7(ch)); }
	
	unsigned short to_lower(unsigned short ch)
		{ return ((((ch) >= 'A') && ((ch) < 'Z')) ? ((ch) - 'A' + 'a') : (ch)); }
	
	short str_to_upper(unsigned short a,
							   unsigned char *b,
							   unsigned short c,
							   unsigned char *d)
		{ return internal_str_to_upper(a,b,c,d); }

	unsigned short to_wc(UCS2_CHAR *a,
									unsigned short b,
									unsigned char *c,
									unsigned short d,
									short *e,
									unsigned short *f)
		{ return internal_fss_to_unicode(a,b,c,d,e,f); }

	unsigned short mbtowc(UCS2_CHAR *wc, unsigned char *p, unsigned short n)
		{ return fss_mbtowc(wc, p, n); }
};

class TextType_ASCII : public TextTypeNC
{
public:
	static TextType *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) TextType_ASCII; }
	TextType_ASCII() : TextTypeNC(ttype_ascii, "C.ASCII", CS_ASCII, CC_C, 1)
		{}

	unsigned short key_length(unsigned short len)
		{ return len; }
	unsigned short string_to_key(unsigned short a,
										 unsigned char *b,
										 unsigned short c,
										 unsigned char *d,
										 unsigned short e)
		{ return internal_string_to_key(a,b,c,d,e,' '); }
	
	short compare(unsigned short a,
						  unsigned char *b,
						  unsigned short c,
						  unsigned char *d)
		{ return internal_compare(a,b,c,d,' '); }
	
	unsigned short to_upper(unsigned short ch)
		{ return (UPPER7(ch)); }
	
	unsigned short to_lower(unsigned short ch)
		{ return ((((ch) >= 'A') && ((ch) < 'Z')) ? ((ch) - 'A' + 'a') : (ch)); }
	
	short str_to_upper(unsigned short a,
							   unsigned char *b,
							   unsigned short c,
							   unsigned char *d)
		{ return internal_str_to_upper(a,b,c,d); }
};
	
class TextType_None : public TextTypeNC
{
public:
	static TextType *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) TextType_None; }
	TextType_None() : TextTypeNC(ttype_none, "C", CS_NONE, CC_C, 1)
		{}
		
	unsigned short key_length(unsigned short len)
		{ return len; }
	unsigned short string_to_key(unsigned short a,
										 unsigned char *b,
										 unsigned short c,
										 unsigned char *d,
										 unsigned short e)
		{ return internal_string_to_key(a,b,c,d,e,' '); }
	
	short compare(unsigned short a,
						  unsigned char *b,
						  unsigned short c,
						  unsigned char *d)
		{ return internal_compare(a,b,c,d,' '); }
		
	unsigned short to_upper(unsigned short ch)
		{ return (UPPER7(ch)); }
	
	unsigned short to_lower(unsigned short ch)
		{ return ((((ch) >= 'A') && ((ch) < 'Z')) ? ((ch) - 'A' + 'a') : (ch)); }
	
	short str_to_upper(unsigned short a,
							   unsigned char *b,
							   unsigned short c,
							   unsigned char *d)
		{ return internal_str_to_upper(a,b,c,d); }
};

class CsConvert_ASCII_UFSS : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_ASCII_UFSS; }
	CsConvert_ASCII_UFSS() : CsConvert(0, "DIRECT", CS_ASCII, CS_UNICODE_FSS)
			{}
			
	unsigned short convert(unsigned char *a,
						   unsigned short b,
						   unsigned char *c,
						   unsigned short d,
						   short *e,
						   unsigned short *f)
		{ return cvt_utffss_to_ascii(a,b,c,d,e,f); }
};

class CsConvert_UFSS_ASCII : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_UFSS_ASCII; }
	CsConvert_UFSS_ASCII() : CsConvert(0, "DIRECT", CS_UNICODE_FSS, CS_ASCII)
			{}
	unsigned short convert(unsigned char *a,
						   unsigned short b,
						   unsigned char *c,
						   unsigned short d,
						   short *e,
						   unsigned short *f)
		{ return cvt_utffss_to_ascii(a,b,c,d,e,f); }
};

class CsConvert_UFSS_None : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_UFSS_None; }
	CsConvert_UFSS_None() : CsConvert(0, "DIRECT", CS_UNICODE_FSS, CS_NONE)
			{}
	unsigned short convert(unsigned char *a,
						   unsigned short b,
						   unsigned char *c,
						   unsigned short d,
						   short *e,
						   unsigned short *f)
		{ return cvt_utffss_to_ascii(a,b,c,d,e,f); }
};

class CsConvert_None_Unicode : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_None_Unicode; }
	CsConvert_None_Unicode() : CsConvert(0, "DIRECT", CS_NONE, CS_UNICODE_UCS2)
			{}
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_Unicode_None : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_Unicode_None; }
	CsConvert_Unicode_None() : CsConvert(0, "DIRECT", CS_UNICODE_UCS2, CS_NONE)
			{}
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_ASCII_Unicode : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_ASCII_Unicode; }
	CsConvert_ASCII_Unicode() : CsConvert(0, "DIRECT", CS_ASCII, CS_UNICODE_UCS2)
			{}
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_Unicode_ASCII : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_Unicode_ASCII; }
	CsConvert_Unicode_ASCII() : CsConvert(0, "DIRECT", CS_UNICODE_UCS2, CS_ASCII)
			{}
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_UFSS_Unicode : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_UFSS_Unicode; }
	CsConvert_UFSS_Unicode() : CsConvert(0, "DIRECT", CS_UNICODE_FSS, CS_UNICODE_UCS2)
			{}
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_Unicode_UFSS : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_Unicode_UFSS; }
	CsConvert_Unicode_UFSS() : CsConvert(0, "DIRECT", CS_UNICODE_UCS2, CS_UNICODE_FSS)
			{}
	unsigned short convert(unsigned char *a,
						   unsigned short b,
						   unsigned char *c,
						   unsigned short d,
						   short *e,
						   unsigned short *f)
		{ return internal_unicode_to_fss(a,b,c,d,e,f); }
};

class CsConvert_Binary_Unicode : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_Binary_Unicode; }
	CsConvert_Binary_Unicode() : CsConvert(0, "DIRECT", CS_BINARY, CS_UNICODE_UCS2)
		{}	
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CsConvert_Unicode_Binary : public CsConvert
{
public:
	static CsConvert *object_factory(MemoryPool& p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CsConvert_Unicode_Binary; }
	CsConvert_Unicode_Binary() : CsConvert(0, "DIRECT", CS_UNICODE_UCS2, CS_BINARY)
		{}	
	unsigned short convert(unsigned char*,
						   unsigned short,
						   unsigned char*,
						   unsigned short,
						   short*,
						   unsigned short*);
};

class CharSet_None : public CharSet
{
public:
	static CharSet *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CharSet_None(p); }
	CharSet_None(MemoryPool &p) : CharSet(CS_NONE, "NONE", 1, 1, 1, " ")
	{
		charset_to_unicode = CsConvert_None_Unicode::object_factory(p,0,0);
		charset_from_unicode = CsConvert_Unicode_None::object_factory(p,0,0);
	}
};

class CharSet_ASCII : public CharSet
{
public:
	static CharSet *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CharSet_ASCII(p); }
	CharSet_ASCII(MemoryPool &p) : CharSet(CS_ASCII, "ASCII", 1, 1, 1, " ")
	{
		charset_to_unicode = CsConvert_ASCII_Unicode::object_factory(p,0,0);
		charset_from_unicode = CsConvert_Unicode_ASCII::object_factory(p,0,0);
	}
};

class CharSet_Unicode_FSS : public CharSet
{
public:
	static CharSet *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CharSet_Unicode_FSS(p); }
	CharSet_Unicode_FSS(MemoryPool &p) : CharSet(CS_UNICODE_FSS, "UNICODE_FSS", 1, 1, 1, " ")
	{
		charset_to_unicode = CsConvert_UFSS_Unicode::object_factory(p,0,0);
		charset_from_unicode = CsConvert_Unicode_UFSS::object_factory(p,0,0);
	}
};

class CharSet_Unicode : public CharSet
{
public:
	static CharSet *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CharSet_Unicode(p); }
	CharSet_Unicode(MemoryPool &p) : CharSet(CS_UNICODE_UCS2, "UNICODE_UCS2", 2, 2, 2, 0)
	{
		static const UCS2_CHAR space = 0x0020;
		charset_space_character = (const char *) & space;	/* 0x0020 */
	}
};

class CharSet_Binary : public CharSet
{
public:
	static CharSet *object_factory(MemoryPool &p, CHARSET_ID u1, CHARSET_ID u2)
		{ return FB_NEW(p) CharSet_Binary(p); }
	CharSet_Binary(MemoryPool &p) : CharSet(CS_BINARY, "BINARY", 1, 1, 1, "\0")
	{
		charset_to_unicode = CsConvert_Binary_Unicode::object_factory(p,0,0);
		charset_from_unicode = CsConvert_Unicode_Binary::object_factory(p,0,0);
	}
};

/********************************************************************************/

#ifndef HAVE_SWAB
#ifdef HAVE__SWAB
#define swab _swab
#else // use generic swab(). Slow (but faster than previous implementation) and buggy
void swab(char * a, char * b, int n)
{
 while(--n>0)
 {
  *b++ = a[1];
  *b++ = *a++;
  a++; n--;
 }
}
#endif
#endif

unsigned short CsConvert_Unicode_Binary::convert(
							unsigned char *pDest,
							unsigned short nDest,
							unsigned char *pSrcUC,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
{
/**************************************
 *
 *      CsConvert_Unicode_Binary::convert 
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string to network form - high-endian
 *      byte stream.
 *
 *************************************/
	unsigned short res;

	assert((pSrcUC != NULL) || (pDest == NULL));
	assert(err_code != NULL);
	assert(err_position != NULL);

	*err_code = 0;
	if (pDest == NULL)			/* length estimate needed? */
		return (nSrc);

	assert(nSrc&1 == 0); // check for even length

	if (nSrc>nDest) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	res = nSrc>nDest?nDest:nSrc;
#ifdef WORDS_BIGENDIAN
	MOVE_FAST(pSrcUC,pDest,res);
#else
	swab((char *)pSrcUC,(char *)pDest,res);
#endif
	*err_position = res;

	return res;
}


unsigned short CsConvert_Binary_Unicode::convert(
							unsigned char *pDestUC,
							unsigned short nDest,
							unsigned char *pSrc,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
{
/**************************************
 *
 *      CsConvert_Binary_Unicode::convert
 *
 **************************************
 *
 * Functional description
 *      Convert a wc string from network form - high-endian
 *      byte stream.
 *	Symmetrical with CsConvert_Unicode_Binary::convert
 *	but really the same
 *
 *************************************/
	unsigned short res;

	assert((pSrc != NULL) || (pDestUC == NULL));
	assert(err_code != NULL);
	assert(err_position != NULL);

	*err_code = 0;
	if (pDestUC == NULL)			/* length estimate needed? */
		return (nSrc);

	assert(nSrc&1 == 0);

	if (nSrc>nDest) {
		*err_code = CS_TRUNCATION_ERROR;
	}
	res = nSrc>nDest?nDest:nSrc;
#ifdef WORDS_BIGENDIAN
	MOVE_FAST(pSrc,pDestUC,res);
#else
	swab((char *)pSrc,(char *)pDestUC,res);
#endif
	*err_position = res;

	return res;
}

unsigned short CsConvert_UFSS_Unicode::convert(
							unsigned char *dest_ptrUC,
							unsigned short dest_len,
							unsigned char *src_ptr,
							unsigned short src_len,
							short *err_code,
							unsigned short *err_position)
{
	UCS2_CHAR *start, *dest_ptr = (UCS2_CHAR*)dest_ptrUC;
	USHORT src_start = src_len;
	fss_size_t res;

	assert(src_ptr != NULL || dest_ptr == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);

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

unsigned short CsConvert_Unicode_ASCII::convert(
							unsigned char *pDest,
							unsigned short nDest,
							unsigned char *pSrcUC,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
{
/**************************************
 *
 *      CsConvert_Unicode_ASCII::convert
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
	UCS2_CHAR *pStart_src, *pSrc = (UCS2_CHAR*)pSrcUC;

	assert((pSrc != NULL) || (pDest == NULL));
	assert(err_code != NULL);

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

unsigned short CsConvert_ASCII_Unicode::convert(
							unsigned char *pDestUC,
							unsigned short nDest,
							unsigned char *pSrc,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
{
/**************************************
 *
 *      CsConvert_ASCII_Unicode::convert
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create BAD_INPUT
 *
 *************************************/
	UCS2_CHAR *pStart, *pDest = (UCS2_CHAR*)pDestUC;
	UCHAR *pStart_src;

	assert((pSrc != NULL) || (pDest == NULL));
	assert(err_code != NULL);

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

unsigned short CsConvert_Unicode_None::convert(
							unsigned char *pDest,
							unsigned short nDest,
							unsigned char *pSrcUC,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
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
	UCS2_CHAR *pStart_src, *pSrc = (UCS2_CHAR*)pSrcUC;

	assert((pSrc != NULL) || (pDest == NULL));
	assert(err_code != NULL);
	assert(err_position != NULL);

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

unsigned short CsConvert_None_Unicode::convert(
							unsigned char *pDestUC,
							unsigned short nDest,
							unsigned char *pSrc,
							unsigned short nSrc,
							short *err_code,
							unsigned short *err_position)
{
/**************************************
 *
 *      CsConvert_None_Unicode::convert
 *
 **************************************
 *
 * Functional description
 *      Convert CHARACTER SET NONE to UNICODE (wide char).
 *      Byte values below 128 treated as ASCII.
 *      Byte values >= 128 create CONVERT ERROR
 *
 *************************************/
	UCS2_CHAR *pStart, *pDest = (UCS2_CHAR*)pDestUC;
	UCHAR *pStart_src;

	assert((pSrc != NULL) || (pDest == NULL));
	assert(err_code != NULL);

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

static unsigned short cvt_utffss_to_ascii(UCHAR * pDest, USHORT nDest,	/* byte count */
								  UCHAR * pSrc, USHORT nSrc,	/* byte count */
								  short *err_code, unsigned short *err_position)
{
/**************************************
 *
 *      c v t _ u t f f s s _ t o _ a s c i i 
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

	assert((pSrc != NULL) || (pDest == NULL));
	assert(err_code != NULL);

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
 *      1) Compatibility with historical file systems:
 *
 *      Historical file systems disallow the null byte and the ASCII
 *      slash character as a part of the file name.
 *
 *      2) Compatibility with existing programs:
 *
 *      The existing model for multibyte processing is that ASCII does
 *      not occur anywhere in a multibyte encoding.  There should be
 *      no ASCII code values for any part of a transformation format
 *      representation of a character that was not in the ASCII
 *      character set in the UCS representation of the character.
 *
 *      3) Ease of conversion from/to UCS.
 *
 *      4) The first byte should indicate the number of bytes to
 *      follow in a multibyte sequence.
 *
 *      5) The transformation format should not be extravagant in
 *      terms of number of bytes used for encoding.
 *
 *      6) It should be possible to find the start of a character
 *      efficiently starting from an arbitrary location in a byte
 *      stream.
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
 * UCS 0, only the SSHORTest encoding is legal.
 *
 * Below are sample implementations of the C standard wctomb() and
 * mbtowc() functions which demonstrate the algorithms for converting
 * from UCS to the transformation format and converting from the
 * transformation format to UCS. The sample implementations include error
 * checks, some of which may not be necessary for conformance:
 *
 */

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



/************************************************************************************/
static unsigned short internal_string_to_key(
									 USHORT inLen,
									 UCHAR * src,
									 USHORT outLen,
UCHAR * dest, USHORT partial, UCHAR pad_char)
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

	pStart = dest;
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

static SSHORT internal_compare(USHORT length1,
							   UCHAR * p1, USHORT length2, UCHAR * p2, UCHAR pad)
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

short TextType_Binary::str_to_upper(USHORT inLen,
								UCHAR * src, USHORT outLen, UCHAR * dest)
{
/**************************************
 *
 *      TextType_Binary::str_to_upper
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

static SSHORT internal_str_to_upper(USHORT inLen,
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

unsigned short internal_unicode_to_fss(
							unsigned char *fss_strUC,
							unsigned short fss_len,
							unsigned char *unicode_strUC,
							unsigned short unicode_len,
							short *err_code,
							unsigned short *err_position)
{
	MBCHAR *fss_str = (MBCHAR*)fss_strUC;
	UCS2_CHAR *unicode_str = (UCS2_CHAR*)unicode_strUC;
	UCHAR *start;
	USHORT src_start = unicode_len;
	UCHAR tmp_buffer[6];
	UCHAR *p;
	fss_size_t res;

	assert(unicode_str != NULL || fss_str == NULL);
	assert(err_code != NULL);
	assert(err_position != NULL);

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

static USHORT  internal_fss_to_unicode (
    UCS2_CHAR     *dest_ptr,
    USHORT      dest_len,               /* BYTE count */
    NCHAR       *src_ptr,
    USHORT      src_len,
    SSHORT      *err_code,
    USHORT      *err_position)
{
	UCS2_CHAR         *start;
	USHORT          src_start = src_len;
	fss_size_t      res;
	
	assert (src_ptr  != NULL || dest_ptr == NULL);
	assert (err_code != NULL);
	assert (err_position != NULL);
	
	*err_code = 0;
	
	/* See if we're only after a length estimate */
	if (dest_ptr == NULL)
		return (src_len*2); /* All single byte narrow characters */
	
	start = dest_ptr;
	src_start = src_len;
	while ((src_len) && (dest_len >= sizeof (*dest_ptr))) 
		{
		res = fss_mbtowc (dest_ptr, src_ptr, src_len);
		if (res == -1)
			{
			*err_code = CS_BAD_INPUT;
			break;
			}
		assert (res <= src_len);
		dest_ptr++;
		dest_len -= sizeof (*dest_ptr);
		src_ptr += res;
		src_len -= res;
		}
	if (src_len && !*err_code) 
		{
		*err_code = CS_TRUNCATION_ERROR;
		}
	*err_position = src_start - src_len;
	return ((dest_ptr - start)*sizeof (*dest_ptr)); 
}

CharSetAllocFunc INTL_charset_alloc_func(short charset)
{
	switch(charset)
	{
		case CS_NONE:
			return CharSet_None::object_factory;
		case CS_ASCII:
			return CharSet_ASCII::object_factory;
		case CS_UNICODE_FSS:
			return CharSet_Unicode_FSS::object_factory;
		case CS_UNICODE_UCS2:
			return CharSet_Unicode::object_factory;
		case CS_BINARY:
			return CharSet_Binary::object_factory;
		default:
			return NULL;
		}
	return NULL;
}

TextTypeAllocFunc INTL_texttype_alloc_func(short ttype)
{
	switch(ttype)
	{
		case ttype_none:
			return TextType_None::object_factory;
		case ttype_ascii:
			return TextType_ASCII::object_factory;
		case ttype_unicode_fss:
			return TextType_UFSS::object_factory;
		case ttype_binary:
			return TextType_Binary::object_factory;
		default:
			return NULL;
	}
	return NULL;
}

CsConvertAllocFunc INTL_csconvert_alloc_func(short from, short to)
{
	if (from == CS_ASCII && to == CS_UNICODE_FSS)
		return CsConvert_ASCII_UFSS::object_factory;
	if (from == CS_UNICODE_FSS && to == CS_ASCII)
		return CsConvert_UFSS_ASCII::object_factory;
	if (from == CS_UNICODE_FSS && to == CS_NONE)
		return CsConvert_UFSS_None::object_factory;
	return NULL;
}

#if NOT_USED_OR_REPLACED
//========================================================================================
//========================================================================================
//========================================================================================
//========================================================================================
//
// JMB: absolete code - to be removed later
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
	cache->texttype_fn_mbtowc =             (FPTR_short) internal_nc_mbtowc; \
	cache->texttype_collation_table =       (BYTE *) " "; \
	cache->texttype_toupper_table =         (BYTE *) NULL; \
	cache->texttype_tolower_table =         (BYTE *) NULL; \
	cache->texttype_compress_table =        (BYTE *) NULL; \
	cache->texttype_expand_table =          (BYTE *) NULL; \
	cache->texttype_name =                  const_cast<char*>(POSIX);



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
	cache->texttype_fn_to_wc = (FPTR_SHORT) INTL_fss_to_unicode;
	cache->texttype_fn_mbtowc = (FPTR_short) INTL_fss_mbtowc;

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
	csptr->charset_well_formed = (FPTR_SHORT) NULL;
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
						CS_UNICODE_FSS, (FPTR_SHORT) INTL_fss_to_unicode,
						NULL, NULL);
	common_convert_init(&csptr->charset_from_unicode, CS_UNICODE_FSS,
						CS_UNICODE_UCS2, (FPTR_SHORT) INTL_unicode_to_fss, NULL,
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
	csptr->charset_id = CS_UNICODE;
	csptr->charset_name = (*const* ASCII) "UNICODE_UCS2";
	csptr->charset_flags = 0;
	csptr->charset_min_bytes_per_char = 2;
	csptr->charset_max_bytes_per_char = 2;
	csptr->charset_space_length = 2;
	csptr->charset_space_character = (BYTE *) & space;	/* 0x0020 */
	csptr->charset_well_formed = (FPTR_SHORT) NULL;
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
#endif
