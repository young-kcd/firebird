/*
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)stdio.h	8.5 (Berkeley) 4/29/95
 * $FreeBSD: src/include/stdio.h,v 1.21.2.2 1999/08/29 14:38:56 peter Exp $
 */

#ifndef	IB_STDIO_H_
#define	IB_STDIO_H_

#ifndef JRD_COMMON_H
#include "../jrd/common.h"
#endif

#include <stdio.h>
#include <stdarg.h>

#define ib_fpos_t	fpos_t

#ifndef NEED_IB_STDIO

/* This is a platform with an adequate stdio implementation,
 * so just define macros which will map all of our names
 * to the usual names supplied by the platform's stdio.
 */

#define ib_stdin	stdin
#define ib_stdout	stdout
#define ib_stderr	stderr
#define ib_clearerr	clearerr
#define ib_fclose	fclose
#define ib_feof		feof
#define ib_ferror	ferror
#define ib_fflush	fflush
#define ib_fgetc	fgetc
#define ib_fgetpos	fgetpos
#define ib_fgets	fgets
#define ib_fopen	fopen
#define ib_fprintf	fprintf
#define ib_fputc	fputc
#define ib_fputs	fputs
#define ib_fread	fread
#define ib_freopen	freopen
#define ib_fscanf	fscanf
#define ib_fseek	fseek
#define ib_fsetpos	fsetpos
#define ib_ftell	ftell
#define ib_fwrite	fwrite
#define ib_getc		getc
#define ib_getchar	getchar
#define ib_perror	perror
#define ib_printf	printf
#define ib_putc		putc
#define ib_putchar	putchar
#define ib_puts		puts
#define ib_rewind	rewind
#define ib_scanf	scanf
#define ib_setbuf	setbuf
#define ib_setvbuf	setvbuf
#define ib_tmpfile	tmpfile
#define ib_ungetc	ungetc
#define ib_vfprintf	vfprintf
#define ib_vprintf	vprintf
#define ib_fdopen	fdopen
#define ib_fileno	fileno
#define IB_FILE         FILE

#define ib_gets		gets
#define ib_sprintf	sprintf
#define ib_getc_unlocked getc_unlocked
#define ib_putc_unlocked putc_unlocked

#else /* NEED_IB_STDIO */

/* @#$% Solaris doesn't let you fopen() a FILE if the underlying open()
 * returns a file descriptor > 255, so we need our private substitute
 * for stdio.
 */

#ifdef SOLARIS
#error SUN
#endif

#include "../jrd/cdefs.h"
#include "../jrd/ansi.h"

#ifdef	_BSD_SIZE_T_
typedef _BSD_SIZE_T_ size_t;
#undef	_BSD_SIZE_T_
#endif

#ifndef NULL
#define	NULL	0
#endif

#define	IB_FSTDIO				/* Define for new stdio with functions. */

/*
 * NB: to fit things in six character monocase externals, the stdio
 * code uses the prefix `__s' for stdio objects, typically followed
 * by a three-character attempt at a mnemonic.
 *
 * The InterBase version doesn't care about implementations with 6-char
 * limits on external names, and names starting with _ belong to the
 * implementation, so those names all gets "ib" as a prefix.
 */

/* stdio buffers */
struct ib__sbuf {
	unsigned char *_base;
	int _size;
};

/*
 * stdio state variables.
 *
 * The following always hold:
 *
 *	if (_flags&(__SLBF|__SWR)) == (__SLBF|__SWR),
 *		_lbfsize is -_bf._size, else _lbfsize is 0
 *	if _flags&__SRD, _w is 0
 *	if _flags&__SWR, _r is 0
 *
 * This ensures that the getc and putc macros (or inline functions) never
 * try to write or read from a file that is in `read' or `write' mode.
 * (Moreover, they can, and do, automatically switch from read mode to
 * write mode, and back, on "r+" and "w+" files.)
 *
 * _lbfsize is used only to make the inline line-buffered output stream
 * code as compact as possible.
 *
 * _ub, _up, and _ur are used when ungetc() pushes back more characters
 * than fit in the current _bf, or when ungetc() pushes back a character
 * that does not match the previous one in _bf.  When this happens,
 * _ub._base becomes non-nil (i.e., a stream has ungetc() data iff
 * _ub._base!=NULL) and _up and _ur save the current values of _p and _r.
 *
 * NB: see WARNING above before changing the layout of this structure!
 */
typedef struct ib__sFILE {
	unsigned char *_p;			/* current position in (some) buffer */
	int _r;						/* read space left for getc() */
	int _w;						/* write space left for putc() */
	short _flags;				/* flags, below; this FILE is free if 0 */
	short _file;				/* fileno, if Unix descriptor, else -1 */
	struct ib__sbuf _bf;		/* the buffer (at least 1 byte, if !NULL) */
	int _lbfsize;				/* 0 or -_bf._size, for inline putc */

	/* operations */
	void *_cookie;				/* cookie passed to io functions */
	int (*_close) (void *);
	int (*_read) (void *, char *, int);
	  ib_fpos_t(*_seek) (void *, ib_fpos_t, int);
	int (*_write) (void *, const char *, int);

	/* separate buffer for long sequences of ungetc() */
	struct ib__sbuf _ub;		/* ungetc buffer */
	unsigned char *_up;			/* saved _p when _p is doing ungetc data */
	int _ur;					/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];		/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];		/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct ib__sbuf _lb;		/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int _blksize;				/* stat.st_blksize (may be != _bf._size) */
	ib_fpos_t _offset;			/* current lseek offset (see WARNING) */
} IB_FILE;

extern IB_FILE ib__sF[];

#define	IB__SLBF	0x0001		/* line buffered */
#define	IB__SNBF	0x0002		/* unbuffered */
#define	IB__SRD		0x0004		/* OK to read */
#define	IB__SWR		0x0008		/* OK to write */
	/* RD and WR are never simultaneously asserted */
#define	IB__SRW		0x0010		/* open for reading & writing */
#define	IB__SEOF	0x0020		/* found EOF */
#define	IB__SERR	0x0040		/* found error */
#define	IB__SMBF	0x0080		/* _buf is from malloc */
#define	IB__SAPP	0x0100		/* fdopen()ed in append mode */
#define	IB__SSTR	0x0200		/* this is an sprintf/snprintf string */
#define	IB__SOPT	0x0400		/* do fseek() optimization */
#define	IB__SNPT	0x0800		/* do not do fseek() optimization */
#define	IB__SOFF	0x1000		/* set iff _offset is in fact correct */
#define	IB__SMOD	0x2000		/* true => fgetln modified _p text */
#define	IB__SALC	0x4000		/* allocate string space dynamically */

/*
 * The following three definitions are for ANSI C, which took them
 * from System V, which brilliantly took internal interface macros and
 * made them official arguments to setvbuf(), without renaming them.
 * Hence, these ugly _IOxxx names are *supposed* to appear in user code.
 *
 * Although numbered as their counterparts above, the implementation
 * does not rely on this.
 */
/* #define	_IOFBF	0   	*//* setvbuf should set fully buffered */
/* #define	_IOLBF	1	*//* setvbuf should set line buffered */
/* #define	_IONBF	2	*//* setvbuf should set unbuffered */

/* #define	BUFSIZ	1024	*//* size of buffer used by setbuf */
/* #define	EOF	(-1) */

/*
 * FOPEN_MAX is a minimum maximum, and is the number of streams that
 * stdio can provide without attempting to allocate further resources
 * (which could fail).  Do not use this for anything.
 */
				/* must be == _POSIX_STREAM_MAX <limits.h> */
#define	IB_FOPEN_MAX	8		/* must be <= OPEN_MAX <sys/syslimits.h> */
/* Should the above be 8 (_POSIX_STREAM_MAX) or 64 (OPEN_MAX)? ????? */
#define	IB_FILENAME_MAX	1024	/* must be <= PATH_MAX <sys/syslimits.h> on */
				/* BSD, but in <limits.h> on Solaris */


#define	ib_stdin	(&ib__sF[0])
#define	ib_stdout	(&ib__sF[1])
#define	ib_stderr	(&ib__sF[2])

/*
 * Functions defined in ANSI C standard.
 */

void ib_clearerr(IB_FILE *);
int ib_fclose(IB_FILE *);
int ib_feof(IB_FILE *);
int ib_ferror(IB_FILE *);
int ib_fflush(IB_FILE *);
int ib_fgetc(IB_FILE *);
int ib_fgetpos(IB_FILE *, ib_fpos_t *);
char *ib_fgets(char *, int, IB_FILE *);
IB_FILE *ib_fopen(const char *, const char *);
int ib_fprintf(IB_FILE *, const char *, ...);
int ib_fputc(int, IB_FILE *);
int ib_fputs(const char *, IB_FILE *);
size_t ib_fread(void *, size_t, size_t, IB_FILE *);
IB_FILE *ib_freopen(const char *, const char *, IB_FILE *);
int ib_fscanf(IB_FILE *, const char *, ...);
int ib_fseek(IB_FILE *, long, int);
int ib_fsetpos(IB_FILE *, const ib_fpos_t *);
long ib_ftell(IB_FILE *);
size_t ib_fwrite(const void *, size_t, size_t, IB_FILE *);
int ib_getc(IB_FILE *);
int ib_getchar(void);
void ib_perror(const char *);
int ib_printf(const char *, ...);
int ib_putc(int, IB_FILE *);
int ib_putchar(int);
int ib_puts(const char *);
void ib_rewind(IB_FILE *);
int ib_scanf(const char *, ...);
void ib_setbuf(IB_FILE *, char *);
int ib_setvbuf(IB_FILE *, char *, int, size_t);
IB_FILE *ib_tmpfile(void);
int ib_ungetc(int, IB_FILE *);
int ib_vfprintf(IB_FILE *, const char *, va_list);
int ib_vprintf(const char *, va_list);

/*
 * Functions defined in POSIX 1003.1.
 */
/* size for cuserid(3); UT_NAMESIZE + 1, see <utmp.h> */
#define	L_cuserid	17

__BEGIN_DECLS IB_FILE * ib_fdopen(int, const char *);
int ib_fileno(IB_FILE *);

__END_DECLS
/*
 * Routines that are purely local.
 */
__BEGIN_DECLS int ib_asprintf(char **, const char *, ...) __printflike(2, 3);
	 char *ib_fgetln(IB_FILE *, size_t *);
	 int ib_fpurge(IB_FILE *);
	 int ib_fseeko(IB_FILE *, off_t, int);
	 off_t ib_ftello(IB_FILE *);
	 int ib_getw(IB_FILE *);
	 int ib_pclose(IB_FILE *);
	 FILE *ib_popen(const char *, const char *);
	 int ib_putw(int, IB_FILE *);
	 void ib_setbuffer(IB_FILE *, char *, int);
	 int ib_setlinebuf(IB_FILE *);
/* char	*tempnam (const char *, const char *); */
	 int ib_vscanf(const char *, va_list) __scanflike(1, 0);
__END_DECLS
/*
 * This is a #define because the function is used internally and
 * (unlike vfscanf) the name __svfscanf is guaranteed not to collide
 * with a user function when _ANSI_SOURCE or _POSIX_SOURCE is defined.
 */
#define	 ib_vfscanf	ib__svfscanf
/*
 * Stdio function-access interface.
 */
__BEGIN_DECLS
	IB_FILE * ib_funopen(const void *,
							 int (*)(void *, char *, int),
						 int (*)(void *, const char *, int),
						 ib_fpos_t(*)(void *, ib_fpos_t, int),
						 int (*)(void *));
__END_DECLS
#define	ib_fropen(cookie, fn) ib_funopen(cookie, fn, 0, 0, 0)
#define	ib_fwopen(cookie, fn) ib_funopen(cookie, 0, fn, 0, 0)
/*
 * Functions internal to the implementation.
 */
	 __BEGIN_DECLS int ib__srget(IB_FILE *);
	 int ib__svfscanf(IB_FILE *, const char *, va_list);
	 int ib__swbuf(int, IB_FILE *);
__END_DECLS
/*
 * The __sfoo macros are here so that we can
 * define function versions in the C library.
 */
#define	ib__sgetc(p) (--(p)->_r < 0 ? ib__srget(p) : (int)(*(p)->_p++))
#if defined(__GNUC__)
	 static inline int ib__sputc(int _c, IB_FILE * _p)
{
	if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char) _c != '\n'))
		return (*_p->_p++ = _c);
	else
		return (ib__swbuf(_c, _p));
}
#else
/*
 * This has been tuned to generate reasonable code on the vax using pcc.
 */
#define	ib__sputc(c, p) \
	(--(p)->_w < 0 ? \
		(p)->_w >= (p)->_lbfsize ? \
			(*(p)->_p = (c)), *(p)->_p != '\n' ? \
				(int)*(p)->_p++ : \
				ib__swbuf('\n', p) : \
			ib__swbuf((int)(c), p) : \
		(*(p)->_p = (c), (int)*(p)->_p++))
#endif
#define	ib__sfeof(p)	(((p)->_flags & IB__SEOF) != 0)
#define	ib__sferror(p)	(((p)->_flags & IB__SERR) != 0)
#define	ib__sclearerr(p) ((void)((p)->_flags &= ~(IB__SERR|IB__SEOF)))
#define	ib__sfileno(p)	((p)->_file)
/*
 * See ISO/IEC 9945-1 ANSI/IEEE Std 1003.1 Second Edition 1996-07-12
 * B.8.2.7 for the rationale behind the *_unlocked() macros.
 */
#define	ib_feof_unlocked(p)	ib__sfeof(p)
#define	ib_ferror_unlocked(p)	ib__sferror(p)
#define	ib_clearerr_unlocked(p)	ib__sclearerr(p)
#define	ib_fileno_unlocked(p)	ib__sfileno(p)
#define	ib_feof(p)	ib_feof_unlocked(p)
#define	ib_ferror(p)	ib_ferror_unlocked(p)
#define	ib_clearerr(p)	ib_clearerr_unlocked(p)
#define	ib_fileno(p)	ib_fileno_unlocked(p)
#ifndef lint
#define	ib_getc_unlocked(fp)	ib__sgetc(fp)
#define ib_putc_unlocked(x, fp)	ib__sputc(x, fp)
#define	ib_getc(fp)	ib_getc_unlocked(fp)
#define ib_putc(x, fp)	ib_putc_unlocked(x, fp)
#endif /* lint */
#define	ib_getchar()		ib_getc(ib_stdin)
#define	ib_getchar_unlocked()	ib_getc_unlocked(ib_stdin)
#define	ib_putchar(x)		ib_putc(x, ib_stdout)
#define	ib_putchar_unlocked(x)	ib_putc_unlocked(x, ib_stdout)
#endif /* NEED_IB_STDIO */
#endif /* !IB_STDIO_H_ */
