/* 
 * This is a substitute for stdio, incorporated into InterBase for use
 * on platforms, such as Solaris, where the vendor-supplied stdio
 * prevents fopen() and its kin from working if the open() system call
 * returns a file descriptor greater than 255.
 *
 * Helloooo Suuun!  We're not running on PDP-11s any more!
 */

/*-
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
 */

/* The following copyright notice applies to the function ib__dtoa()
 * below, which was extracted from stdlib/netbsd_strtod.c on FreeBSD.
 */

/****************************************************************
 *
 * The author of this software is David M. Gay.
 *
 * Copyright (c) 1991 by AT&T.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR AT&T MAKES ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *
 ***************************************************************/

/* Please send bug reports to
	David M. Gay
	AT&T Bell Laboratories, Room 2C-463
	600 Mountain Avenue
	Murray Hill, NJ 07974-2070
	U.S.A.
	dmg@research.att.com or research!dmg
 */


/*
 * We started with /usr/src/lib/libc/stdio from FreeBSD 3.4, for those
 * trying to track the origin of this code.
 */

/* Firebird changes:-
 * Small fixes to get gcc to compile module on Solaris sparc - Neil McCalden
 * Add support for Solaris x86  - Neil McCalden
 */

/*
$Id: ib_stdio.cpp,v 1.9 2003-02-12 12:51:06 brodsom Exp $
*/

#include "firebird.h"
//#include "source/jrd/common.h"
#ifdef NEED_IB_STDIO
#include "ib_stdio.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <stdarg.h>
#include <math.h>
#ifdef SOLARIS
#include <ieeefp.h>
#endif
#include <ctype.h>
#include <limits.h>
#include <signal.h>

/* The following definition was included from <machine/param.h> on BSD,
 * but that header doesn't exist on some other platforms.
 * This definition may be wrong for some platforms.
 */
#define ALIGNBYTES (sizeof(int) - 1)

/* From <paths.h> on BSD */
#define _PATH_TMP "/tmp/"

#define DEFFILEMODE (0666)

/*  On BSD, these types come from sys/types.h or whatever.  We equate them
 *  to types which we define for each platform in jrd/common.h.
 */
#define u_quad_t UINT64
#define quad_t   SINT64
#define QUAD_MAX MAX_SINT64

/* Give some BSD <stdlib.h> function names Solaris equivalents. */
#define strtoq  strtoll
#define strtouq strtoull

/* These macros call do-nothing functions on BSD: we'll make the macro
   definitions empty for our purposes. */
#define FLOCKFILE(fp)
#define FUNLOCKFILE(fp)

/* These come from stdio/local.h on BSD */

/*
 * Return true iff the given FILE cannot be written now.
 */
#define	cantwrite(fp) \
	((((fp)->_flags & IB__SWR) == 0 || (fp)->_bf._base == NULL) && \
	 ib__swsetup(fp))

/*
 * Test whether the given stdio file has an active ungetc buffer;
 * release such a buffer, without restoring ordinary unread data.
 */
#define	HASUB(fp) ((fp)->_ub._base != NULL)
#define	FREEUB(fp) { \
	if ((fp)->_ub._base != (fp)->_ubuf) \
		free((char *)(fp)->_ub._base); \
	(fp)->_ub._base = NULL; \
}

/*
 * test for an fgetln() buffer.
 */
#define	HASLB(fp) ((fp)->_lb._base != NULL)
#define	FREELB(fp) { \
	free((char *)(fp)->_lb._base); \
	(fp)->_lb._base = NULL; \
}


/* BSD stdio wants to see __collate_load_error, which is privately
 * available from locale/collate.c in libc.  In moving this off BSD,
 * we will treat this as a constant.  The value 1 is what the BSD
 * library initializes the variable to: the value is unchanged when
 * the client program asks for either of the locales "C" or "POSIX",
 * so we use that as the fixed value of this unvarying variable.
 */
static const int ib__collate_load_error = 1;

 /*
    * The first few FILEs are statically allocated; others are dynamically
    * allocated and linked in via this glue structure.
  */
static struct glue {
	struct glue *next;
	int niobs;
	IB_FILE *iobs;
};

/* The following 2 defines are taken from stdio/floatio.h in BSD */

/*
 * Floating point scanf/printf (input/output) definitions.
 */

/* 11-bit exponent (VAX G floating point) is 308 decimal digits */
#define	MAXEXP		308
/* 128 bit fraction takes up 39 decimal digits; max reasonable precision */
#define	MAXFRACT	39



/* Prototypes for functions used before their definitions. */

static int ib__sread(void *, char *, int);
static int ib__swrite(void *, char const *, int n);
static ib_fpos_t ib__sseek(void *, ib_fpos_t, int);
static int ib__sclose(void *);
static IB_FILE *ib__sfp(void);
static void ib__sinit(void);
static void ib__smakebuf(IB_FILE *);
static char *ib__dtoa(double, int, int, int *, int *, char **);
int ib__sflush(IB_FILE * fp);

/*
 * I/O descriptors for ib__sfvwrite().
 */
struct ib__siov {
	void *iov_base;
	size_t iov_len;
};
struct ib__suio {
	struct ib__siov *uio_iov;
	int uio_iovcnt;
	int uio_resid;
};

/* The following function comes from stdlib on BSD. */

static void *reallocf(void *ptr, size_t size)
{
	void *nptr;

	nptr = realloc(ptr, size);
	if (!nptr && ptr)
		free(ptr);
	return (nptr);
}




#undef ib_clearerr

void ib_clearerr(IB_FILE * fp)
{
	FLOCKFILE(fp);
	ib__sclearerr(fp);
	FUNLOCKFILE(fp);
}

int ib_fclose(IB_FILE * fp)
{
	int r;

	if (fp->_flags == 0) {		/* not open! */
		errno = EBADF;
		return (EOF);
	}
	FLOCKFILE(fp);
	r = fp->_flags & IB__SWR ? ib__sflush(fp) : 0;
	if (fp->_close != NULL && (*fp->_close) (fp->_cookie) < 0)
		r = EOF;
	if (fp->_flags & IB__SMBF)
		free((char *) fp->_bf._base);
	if (HASUB(fp))
		FREEUB(fp);
	if (HASLB(fp))
		FREELB(fp);
	FUNLOCKFILE(fp);
	fp->_flags = 0;				/* Release this FILE for reuse. */
	fp->_file = -1;
	fp->_r = fp->_w = 0;		/* Mess up if reaccessed. */
	return (r);
}

IB_FILE *ib_fdopen(int fd, const char *mode)
{
	IB_FILE *fp;
	static int nofile;
	int flags, oflags, fdflags, tmp;

	if (nofile == 0)
		nofile = getdtablesize();

	if ((flags = ib__sflags(mode, &oflags)) == 0)
		return (NULL);

	/* Make sure the mode the user wants is a subset of the actual mode. */
	if ((fdflags = fcntl(fd, F_GETFL, 0)) < 0)
		return (NULL);
	tmp = fdflags & O_ACCMODE;
	if (tmp != O_RDWR && (tmp != (oflags & O_ACCMODE))) {
		errno = EINVAL;
		return (NULL);
	}

	if ((fp = ib__sfp()) == NULL)
		return (NULL);
	fp->_flags = flags;
	/*
	 * If opened for appending, but underlying descriptor does not have
	 * O_APPEND bit set, assert IB__SAPP so that ib__swrite() will lseek
	 * to end before each write.
	 */
	if ((oflags & O_APPEND) && !(fdflags & O_APPEND))
		fp->_flags |= IB__SAPP;
	fp->_file = fd;
	fp->_cookie = fp;
	fp->_read = ib__sread;
	fp->_write = ib__swrite;
	fp->_seek = ib__sseek;
	fp->_close = ib__sclose;
	return (fp);
}

/*
 * A subroutine version of the macro feof.
 */
#undef ib_feof

int ib_feof(IB_FILE * fp)
{
	return (ib__sfeof(fp));
}

/*
 * A subroutine version of the macro ferror.
 */
#undef ib_ferror

int ib_ferror(IB_FILE * fp)
{
	return (ib__sferror(fp));
}


/* Flush a single file, or (if fp is NULL) all files.  */
int ib_fflush(IB_FILE * fp)
{
	int retval;

	if (fp == NULL)
		return (ib_fwalk(ib__sflush));
	FLOCKFILE(fp);
	if ((fp->_flags & (IB__SWR | IB__SRW)) == 0) {
		errno = EBADF;
		retval = EOF;
	}
	else {
		retval = ib__sflush(fp);
	}
	FUNLOCKFILE(fp);
	return (retval);
}

int ib__sflush(IB_FILE * fp)
{
	unsigned char *p;
	int n, t;

	t = fp->_flags;
	if ((t & IB__SWR) == 0)
		return (0);

	if ((p = fp->_bf._base) == NULL)
		return (0);

	n = fp->_p - p;				/* write this much */

	/*
	 * Set these immediately to allow exchange buffering
	 * (via setvbuf) in user write function.
	 */
	fp->_p = p;
	fp->_w = t & (IB__SLBF | IB__SNBF) ? 0 : fp->_bf._size;

	for (; n > 0; n -= t, p += t) {
		t = (*fp->_write) (fp->_cookie, (char *) p, n);
		if (t <= 0) {
			fp->_flags |= IB__SERR;
			return (EOF);
		}
	}
	return (0);
}


int ib_fgetc(IB_FILE * fp)
{
	int retval;
	FLOCKFILE(fp);
	retval = ib__sgetc(fp);
	FUNLOCKFILE(fp);
	return (retval);
}


/*
 * Expand the line buffer.  Return -1 on error.
 */
int ib__slbexpand(IB_FILE * fp, size_t newsize)
{
	void *p;

	if (fp->_lb._size >= newsize)
		return (0);
	if ((p = realloc(fp->_lb._base, newsize)) == NULL)
		return (-1);
	fp->_lb._base = p;
	fp->_lb._size = newsize;
	return (0);
}

/*
 * Get an input line.  The returned pointer often (but not always)
 * points into a stdio buffer.  Fgetln does not alter the text of
 * the returned line (which is thus not a C string because it will
 * not necessarily end with '\0'), but does allow callers to modify
 * it if they wish.  Thus, we set __SMOD in case the caller does.
 */
char *ib_fgetln(IB_FILE * fp, size_t * lenp)
{
	unsigned char *p;
	size_t len;
	size_t off;

	/* make sure there is input */
	if (fp->_r <= 0 && ib__srefill(fp)) {
		*lenp = 0;
		return (NULL);
	}

	/* look for a newline in the input */
	if ((p = memchr((void *) fp->_p, '\n', (size_t) fp->_r)) != NULL) {
		char *ret;

		/*
		 * Found one.  Flag buffer as modified to keep fseek from
		 * `optimising' a backward seek, in case the user stomps on
		 * the text.
		 */
		p++;					/* advance over it */
		ret = (char *) fp->_p;
		*lenp = len = p - fp->_p;
		fp->_flags |= IB__SMOD;
		fp->_r -= len;
		fp->_p = p;
		return (ret);
	}

	/*
	 * We have to copy the current buffered data to the line buffer.
	 * As a bonus, though, we can leave off the IB__SMOD.
	 *
	 * OPTIMISTIC is length that we (optimistically) expect will
	 * accomodate the `rest' of the string, on each trip through the
	 * loop below.
	 */
#define OPTIMISTIC 80

	for (len = fp->_r, off = 0;; len += fp->_r) {
		size_t diff;

		/*
		 * Make sure there is room for more bytes.  Copy data from
		 * file buffer to line buffer, refill file and look for
		 * newline.  The loop stops only when we find a newline.
		 */
		if (ib__slbexpand(fp, len + OPTIMISTIC))
			goto error;
		(void) memcpy((void *) (fp->_lb._base + off), (void *) fp->_p,
					  len - off);
		off = len;
		if (ib__srefill(fp))
			break;				/* EOF or error: return partial line */
		if ((p = memchr((void *) fp->_p, '\n', (size_t) fp->_r)) == NULL)
			continue;

		/* got it: finish up the line (like code above) */
		p++;
		diff = p - fp->_p;
		len += diff;
		if (ib__slbexpand(fp, len))
			goto error;
		(void) memcpy((void *) (fp->_lb._base + off), (void *) fp->_p, diff);
		fp->_r -= diff;
		fp->_p = p;
		break;
	}
	*lenp = len;
	return ((char *) fp->_lb._base);

  error:
	*lenp = 0;					/* ??? */
	return (NULL);				/* ??? */
}

int ib_fgetpos(IB_FILE * fp, ib_fpos_t * pos)
{
	int retval;
	FLOCKFILE(fp);
	retval = (*pos = ib_ftello(fp)) == (ib_fpos_t) - 1;
	FUNLOCKFILE(fp);
	return (retval);
}

/*
 * Read at most n-1 characters from the given file.
 * Stop when a newline has been read, or the count runs out.
 * Return first argument, or NULL if no characters were read.
 */
char *ib_fgets(char *buf, int n, IB_FILE * fp)
{
	size_t len;
	char *s;
	unsigned char *p, *t;

	if (n <= 0)					/* sanity check */
		return (NULL);

	FLOCKFILE(fp);
	s = buf;
	n--;						/* leave space for NUL */
	while (n != 0) {
		/*
		 * If the buffer is empty, refill it.
		 */
		if ((len = fp->_r) <= 0) {
			if (ib__srefill(fp)) {
				/* EOF/error: stop with partial or no line */
				if (s == buf) {
					FUNLOCKFILE(fp);
					return (NULL);
				}
				break;
			}
			len = fp->_r;
		}
		p = fp->_p;

		/*
		 * Scan through at most n bytes of the current buffer,
		 * looking for '\n'.  If found, copy up to and including
		 * newline, and stop.  Otherwise, copy entire chunk
		 * and loop.
		 */
		if (len > n)
			len = n;
		t = memchr((void *) p, '\n', len);
		if (t != NULL) {
			len = ++t - p;
			fp->_r -= len;
			fp->_p = t;
			(void) memcpy((void *) s, (void *) p, len);
			s[len] = 0;
			FUNLOCKFILE(fp);
			return (buf);
		}
		fp->_r -= len;
		fp->_p += len;
		(void) memcpy((void *) s, (void *) p, len);
		s += len;
		n -= len;
	}
	*s = 0;
	FUNLOCKFILE(fp);
	return (buf);
}

/*
 * A subroutine version of the macro fileno.
 */
#undef ib_fileno

int ib_fileno(IB_FILE * fp)
{
	return (ib__sfileno(fp));
}


static int ib__sdidinit;

#define	NDYNAMIC 10				/* add ten more whenever necessary */

#define	std(flags, file) \
	{0,0,0,flags,file,{0},0,ib__sF+file,ib__sclose,ib__sread,ib__sseek,ib__swrite}
/*	 p r w flags file _bf z  cookie      close     read      seek      write */

				/* the usual - (stdin + stdout + stderr) */
static IB_FILE usual[IB_FOPEN_MAX - 3];
static struct glue uglue = { 0, IB_FOPEN_MAX - 3, usual };

#define IB_STDIN_FILENO 0
#define IB_STDOUT_FILENO 1
#define IB_STDERR_FILENO 2

IB_FILE ib__sF[3] = {
	std(IB__SRD, IB_STDIN_FILENO),	/* stdin */
	std(IB__SWR, IB_STDOUT_FILENO),	/* stdout */
	std(IB__SWR | IB__SNBF, IB_STDERR_FILENO)	/* stderr */
};

struct glue ib__sglue = { &uglue, 3, ib__sF };

static struct glue *moreglue(int);

static struct glue *moreglue(int n)
{
	struct glue *g;
	IB_FILE *p;
	static IB_FILE empty;

	g = (struct glue *) malloc(sizeof(*g) + ALIGNBYTES + n * sizeof(IB_FILE));
	if (g == NULL)
		return (NULL);
	/*
	 * Note that the ALIGN macro invoked is the one in
	 * jrd/common.h, not the one which BSD code expects to find in
	 * <machine/param.h>, which has a different number of arguments
	 */
	p = (IB_FILE *) FB_ALIGN(((unsigned) (g + 1)), (sizeof(int)));
	g->next = NULL;
	g->niobs = n;
	g->iobs = p;
	while (--n >= 0)
		*p++ = empty;
	return (g);
}

/*
 * Find a free FILE for fopen et al.
 */
IB_FILE *ib__sfp(void)
{
	IB_FILE *fp;
	int n;
	struct glue *g;

	if (!ib__sdidinit)
		ib__sinit();
	for (g = &ib__sglue;; g = g->next) {
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags == 0)
				goto found;
		if (g->next == NULL && (g->next = moreglue(NDYNAMIC)) == NULL)
			break;
	}
	return (NULL);
  found:
	fp->_flags = 1;				/* reserve this slot; caller sets real flags */
	fp->_p = NULL;				/* no current pointer */
	fp->_w = 0;					/* nothing to read or write */
	fp->_r = 0;
	fp->_bf._base = NULL;		/* no buffer */
	fp->_bf._size = 0;
	fp->_lbfsize = 0;			/* not line buffered */
	fp->_file = -1;				/* no file */
/*	fp->_cookie = <any>; *//* caller sets cookie, _read/_write etc */
	fp->_ub._base = NULL;		/* no ungetc buffer */
	fp->_ub._size = 0;
	fp->_lb._base = NULL;		/* no line buffer */
	fp->_lb._size = 0;
	return (fp);
}

/*
 * exit() calls _cleanup() through *__cleanup, set whenever we
 * open or buffer a file.  This chicanery is done so that programs
 * that do not use stdio need not link it all in.
 *
 * The name `_cleanup' is, alas, fairly well known outside stdio.
 *
 * This InterBase version will use the atexit() mechanism for cleanup
 * so the _cleanup magic name is not an issue for us.
 */
void ib_cleanup(void)
{
	/* (void) ib_fwalk(ib_fclose); */
	(void) ib_fwalk(ib__sflush);	/* `cheating' */
}

/*
 * ib__sinit() is called whenever stdio's internal variables must be set up.
 */
static void ib__sinit(void)
{
	/* make sure we clean up on exit */
	/*    ib__cleanup = ib_cleanup;       /* conservative */
	atexit(ib_cleanup);
	ib__sdidinit = 1;
}

/*
 * Return the (stdio) flags for a given mode.  Store the flags
 * to be passed to an open() syscall through *optr.
 * Return 0 on error.
 */
int ib__sflags(const char *mode, int *optr)
{
	int ret, m, o;

	switch (*mode++) {

	case 'r':					/* open for reading */
		ret = IB__SRD;
		m = O_RDONLY;
		o = 0;
		break;

	case 'w':					/* open for writing */
		ret = IB__SWR;
		m = O_WRONLY;
		o = O_CREAT | O_TRUNC;
		break;

	case 'a':					/* open for appending */
		ret = IB__SWR;
		m = O_WRONLY;
		o = O_CREAT | O_APPEND;
		break;

	default:					/* illegal mode */
		errno = EINVAL;
		return (0);
	}

	/* [rwa]\+ or [rwa]b\+ means read and write */
	if (*mode == '+' || (*mode == 'b' && mode[1] == '+')) {
		ret = IB__SRW;
		m = O_RDWR;
	}
	*optr = m | o;
	return (ret);
}

IB_FILE *ib_fopen(const char *file, const char *mode)
{
	IB_FILE *fp;
	int f;
	int flags, oflags;

	if ((flags = ib__sflags(mode, &oflags)) == 0)
		return (NULL);
	if ((fp = ib__sfp()) == NULL)
		return (NULL);
	if ((f = open(file, oflags, DEFFILEMODE)) < 0) {
		fp->_flags = 0;			/* release */
		return (NULL);
	}
	fp->_file = f;
	fp->_flags = flags;
	fp->_cookie = fp;
	fp->_read = ib__sread;
	fp->_write = ib__swrite;
	fp->_seek = ib__sseek;
	fp->_close = ib__sclose;

	/*
	 * When opening in append mode, even though we use O_APPEND,
	 * we need to seek to the end so that ftell() gets the right
	 * answer.  If the user then alters the seek pointer, or
	 * the file extends, this will fail, but there is not much
	 * we can do about this.  (We could set __SAPP and check in
	 * fseek and ftell.)
	 */
	if (oflags & O_APPEND)
		(void) ib__sseek((void *) fp, (fpos_t) 0, SEEK_END);
	return (fp);
}

int ib_fprintf(IB_FILE * fp, const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = ib_vfprintf(fp, fmt, ap);
	va_end(ap);
	return (ret);
}

/*
 * fpurge: like fflush, but without writing anything: leave the
 * given FILE's buffer empty.
 */
int ib_fpurge(IB_FILE * fp)
{
	int retval;
	FLOCKFILE(fp);
	if (!fp->_flags) {
		errno = EBADF;
		retval = EOF;
	}
	else {
		if (HASUB(fp))
			FREEUB(fp);
		fp->_p = fp->_bf._base;
		fp->_r = 0;
		fp->_w = fp->_flags & (IB__SLBF | IB__SNBF) ? 0 : fp->_bf._size;
		retval = 0;
	}
	FUNLOCKFILE(fp);
	return (retval);
}
int ib_fputc(int c, IB_FILE * fp)
{
	int retval;
	FLOCKFILE(fp);
	retval = ib_putc(c, fp);
	FUNLOCKFILE(fp);
	return (retval);
}


/* a prototype which is needed now ... */
int ib__sfvwrite(IB_FILE *, struct ib__suio *);

/*
 * Write the given string to the given file.
 */
int ib_fputs(const char *s, IB_FILE * fp)
{
	int retval;
	struct ib__suio uio;
	struct ib__siov iov;

	iov.iov_base = (void *) s;
	iov.iov_len = uio.uio_resid = strlen(s);
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;
	FLOCKFILE(fp);
	retval = ib__sfvwrite(fp, &uio);
	FUNLOCKFILE(fp);
	return (retval);
}

size_t ib_fread(void *buf, size_t size, size_t count, IB_FILE * fp)
{
	size_t resid;
	char *p;
	int r;
	size_t total;

	/*
	 * The ANSI standard requires a return value of 0 for a count
	 * or a size of 0.  Peculiarly, it imposes no such requirements
	 * on fwrite; it only requires fread to be broken.
	 */
	if ((resid = count * size) == 0)
		return (0);
	FLOCKFILE(fp);
	if (fp->_r < 0)
		fp->_r = 0;
	total = resid;
	p = buf;
	while (resid > (r = fp->_r)) {
		(void) memcpy((void *) p, (void *) fp->_p, (size_t) r);
		fp->_p += r;
		/* fp->_r = 0 ... done in ib__srefill */
		p += r;
		resid -= r;
		if (ib__srefill(fp)) {
			/* no more input: return partial result */
			FUNLOCKFILE(fp);
			return ((total - resid) / size);
		}
	}
	(void) memcpy((void *) p, (void *) fp->_p, resid);
	fp->_r -= resid;
	fp->_p += resid;
	FUNLOCKFILE(fp);
	return (count);
}

/*
 * Re-direct an existing, open (probably) file to some other file.
 * ANSI is written such that the original file gets closed if at
 * all possible, no matter what.
 */
IB_FILE *ib_freopen(const char *file, const char *mode, IB_FILE * fp)
{
	int f;
	int flags, isopen, oflags, sverrno, wantfd;

	if ((flags = ib__sflags(mode, &oflags)) == 0) {
		(void) ib_fclose(fp);
		return (NULL);
	}

	if (!ib__sdidinit)
		ib__sinit();

	/*
	 * There are actually programs that depend on being able to "freopen"
	 * descriptors that weren't originally open.  Keep this from breaking.
	 * Remember whether the stream was open to begin with, and which file
	 * descriptor (if any) was associated with it.  If it was attached to
	 * a descriptor, defer closing it; freopen("/dev/stdin", "r", stdin)
	 * should work.  This is unnecessary if it was not a Unix file.
	 */
	if (fp->_flags == 0) {
		fp->_flags = IB__SEOF;	/* hold on to it */
		isopen = 0;
		wantfd = -1;
	}
	else {
		/* flush the stream; ANSI doesn't require this. */
		if (fp->_flags & IB__SWR)
			(void) ib__sflush(fp);
		/* if close is NULL, closing is a no-op, hence pointless */
		isopen = fp->_close != NULL;
		if ((wantfd = fp->_file) < 0 && isopen) {
			(void) (*fp->_close) (fp->_cookie);
			isopen = 0;
		}
	}

	/* Get a new descriptor to refer to the new file. */
	f = open(file, oflags, DEFFILEMODE);
	if (f < 0 && isopen) {
		/* If out of fd's close the old one and try again. */
		if (errno == ENFILE || errno == EMFILE) {
			(void) (*fp->_close) (fp->_cookie);
			isopen = 0;
			f = open(file, oflags, DEFFILEMODE);
		}
	}
	sverrno = errno;

	/*
	 * Finish closing fp.  Even if the open succeeded above, we cannot
	 * keep fp->_base: it may be the wrong size.  This loses the effect
	 * of any setbuffer calls, but stdio has always done this before.
	 */
	if (isopen)
		(void) (*fp->_close) (fp->_cookie);
	if (fp->_flags & IB__SMBF)
		free((char *) fp->_bf._base);
	fp->_w = 0;
	fp->_r = 0;
	fp->_p = NULL;
	fp->_bf._base = NULL;
	fp->_bf._size = 0;
	fp->_lbfsize = 0;
	if (HASUB(fp))
		FREEUB(fp);
	fp->_ub._size = 0;
	if (HASLB(fp))
		FREELB(fp);
	fp->_lb._size = 0;

	if (f < 0) {				/* did not get it after all */
		fp->_flags = 0;			/* set it free */
		errno = sverrno;		/* restore in case _close clobbered */
		return (NULL);
	}

	/*
	 * If reopening something that was open before on a real file, try
	 * to maintain the descriptor.  Various C library routines (perror)
	 * assume stderr is always fd STDERR_FILENO, even if being freopen'd.
	 */
	if (wantfd >= 0 && f != wantfd) {
		if (dup2(f, wantfd) >= 0) {
			(void) close(f);
			f = wantfd;
		}
	}

	fp->_flags = flags;
	fp->_file = f;
	fp->_cookie = fp;
	fp->_read = ib__sread;
	fp->_write = ib__swrite;
	fp->_seek = ib__sseek;
	fp->_close = ib__sclose;
	return (fp);
}

int ib_fscanf(IB_FILE * fp, char const *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	FLOCKFILE(fp);
	ret = ib__svfscanf(fp, fmt, ap);
	va_end(ap);
	FUNLOCKFILE(fp);
	return (ret);
}

#define	POS_ERR	(-(ib_fpos_t)1)

int ib_fseek(IB_FILE * fp, long offset, int whence)
{
	return (ib_fseeko(fp, offset, whence));
}

/*
 * Seek the given file to the given offset.
 * `Whence' must be one of the three SEEK_* macros.
 */
int ib_fseeko(IB_FILE * fp, off_t offset, int whence)
{
	ib_fpos_t(*seekfn) (void *, fpos_t, int);
	ib_fpos_t target, curoff;
	size_t n;
	struct stat st;
	int havepos;

	/* make sure stdio is set up */
	if (!ib__sdidinit)
		ib__sinit();

	FLOCKFILE(fp);
	/*
	 * Have to be able to seek.
	 */
	if ((seekfn = fp->_seek) == NULL) {
		errno = ESPIPE;			/* historic practice */
		FUNLOCKFILE(fp);
		return (EOF);
	}

	/*
	 * Change any SEEK_CUR to SEEK_SET, and check `whence' argument.
	 * After this, whence is either SEEK_SET or SEEK_END.
	 */
	switch (whence) {

	case SEEK_CUR:
		/*
		 * In order to seek relative to the current stream offset,
		 * we have to first find the current stream offset a la
		 * ftell (see ftell for details).
		 */
		if (fp->_flags & IB__SOFF)
			curoff = fp->_offset;
		else {
			curoff = (*seekfn) (fp->_cookie, (fpos_t) 0, SEEK_CUR);
			if (curoff == -1) {
				FUNLOCKFILE(fp);
				return (EOF);
			}
		}
		if (fp->_flags & IB__SRD) {
			curoff -= fp->_r;
			if (HASUB(fp))
				curoff -= fp->_ur;
		}
		else if (fp->_flags & IB__SWR && fp->_p != NULL)
			curoff += fp->_p - fp->_bf._base;

		offset += curoff;
		whence = SEEK_SET;
		havepos = 1;
		break;

	case SEEK_SET:
	case SEEK_END:
		curoff = 0;				/* XXX just to keep gcc quiet */
		havepos = 0;
		break;

	default:
		errno = EINVAL;
		FUNLOCKFILE(fp);
		return (EOF);
	}

	/*
	 * Can only optimise if:
	 *  reading (and not reading-and-writing);
	 *  not unbuffered; and
	 *  this is a `regular' Unix file (and hence seekfn==ib__sseek).
	 * We must check IB__NBF first, because it is possible to have IB__NBF
	 * and IB__SOPT both set.
	 */
	if (fp->_bf._base == NULL)
		ib__smakebuf(fp);
	if (fp->_flags & (IB__SWR | IB__SRW | IB__SNBF | IB__SNPT))
		goto dumb;
	if ((fp->_flags & IB__SOPT) == 0) {
		if (seekfn != ib__sseek ||
			fp->_file < 0 || fstat(fp->_file, &st) ||
			(st.st_mode & S_IFMT) != S_IFREG) {
			fp->_flags |= IB__SNPT;
			goto dumb;
		}
		fp->_blksize = st.st_blksize;
		fp->_flags |= IB__SOPT;
	}

	/*
	 * We are reading; we can try to optimise.
	 * Figure out where we are going and where we are now.
	 */
	if (whence == SEEK_SET)
		target = offset;
	else {
		if (fstat(fp->_file, &st))
			goto dumb;
		target = st.st_size + offset;
	}

	if (!havepos) {
		if (fp->_flags & IB__SOFF)
			curoff = fp->_offset;
		else {
			curoff = (*seekfn) (fp->_cookie, (fpos_t) 0, SEEK_CUR);
			if (curoff == POS_ERR)
				goto dumb;
		}
		curoff -= fp->_r;
		if (HASUB(fp))
			curoff -= fp->_ur;
	}

	/*
	 * Compute the number of bytes in the input buffer (pretending
	 * that any ungetc() input has been discarded).  Adjust current
	 * offset backwards by this count so that it represents the
	 * file offset for the first byte in the current input buffer.
	 */
	if (HASUB(fp)) {
		curoff += fp->_r;		/* kill off ungetc */
		n = fp->_up - fp->_bf._base;
		curoff -= n;
		n += fp->_ur;
	}
	else {
		n = fp->_p - fp->_bf._base;
		curoff -= n;
		n += fp->_r;
	}

	/*
	 * If the target offset is within the current buffer,
	 * simply adjust the pointers, clear EOF, undo ungetc(),
	 * and return.  (If the buffer was modified, we have to
	 * skip this; see fgetln.c.)
	 */
	if ((fp->_flags & IB__SMOD) == 0 &&
		target >= curoff && target < curoff + n) {
		int o = target - curoff;

		fp->_p = fp->_bf._base + o;
		fp->_r = n - o;
		if (HASUB(fp))
			FREEUB(fp);
		fp->_flags &= ~IB__SEOF;
		FUNLOCKFILE(fp);
		return (0);
	}

	/*
	 * The place we want to get to is not within the current buffer,
	 * but we can still be kind to the kernel copyout mechanism.
	 * By aligning the file offset to a block boundary, we can let
	 * the kernel use the VM hardware to map pages instead of
	 * copying bytes laboriously.  Using a block boundary also
	 * ensures that we only read one block, rather than two.
	 */
	curoff = target & ~(fp->_blksize - 1);
	if ((*seekfn) (fp->_cookie, curoff, SEEK_SET) == POS_ERR)
		goto dumb;
	fp->_r = 0;
	fp->_p = fp->_bf._base;
	if (HASUB(fp))
		FREEUB(fp);
	fp->_flags &= ~IB__SEOF;
	n = target - curoff;
	if (n) {
		if (ib__srefill(fp) || fp->_r < n)
			goto dumb;
		fp->_p += n;
		fp->_r -= n;
	}
	FUNLOCKFILE(fp);
	return (0);

	/*
	 * We get here if we cannot optimise the seek ... just
	 * do it.  Allow the seek function to change fp->_bf._base.
	 */
  dumb:
	if (ib__sflush(fp) ||
		(*seekfn) (fp->_cookie, (ib_fpos_t) offset, whence) == POS_ERR) {
		FUNLOCKFILE(fp);
		return (EOF);
	}
	/* success: clear EOF indicator and discard ungetc() data */
	if (HASUB(fp))
		FREEUB(fp);
	fp->_p = fp->_bf._base;
	fp->_r = 0;
/* fp->_w = 0; *//* unnecessary (I think...) */
	fp->_flags &= ~IB__SEOF;
	FUNLOCKFILE(fp);
	return (0);
}

/*
 * fsetpos: like fseek.
 */
int ib_fsetpos(IB_FILE * iop, const ib_fpos_t * pos)
{
	return (ib_fseeko(iop, (off_t) * pos, SEEK_SET));
}

/*
 * standard ftell function.
 */
long ib_ftell(IB_FILE * fp)
{
	off_t rv;
	rv = ib_ftello(fp);
	if ((long) rv != rv) {
		errno = EOVERFLOW;
		return (-1);
	}
	return (rv);
}

/*
 * ib_ftello: return current offset.
 */
off_t ib_ftello(IB_FILE * fp)
{
	ib_fpos_t pos;

	if (fp->_seek == NULL) {
		errno = ESPIPE;			/* historic practice */
		return (-1L);
	}

	FLOCKFILE(fp);
	/*
	 * Find offset of underlying I/O object, then
	 * adjust for buffered bytes.
	 */
	if (fp->_flags & IB__SOFF)
		pos = fp->_offset;
	else {
		pos = (*fp->_seek) (fp->_cookie, (fpos_t) 0, SEEK_CUR);
		if (pos == -1) {
			FUNLOCKFILE(fp);
			return (pos);
		}
	}
	if (fp->_flags & IB__SRD) {
		/*
		 * Reading.  Any unread characters (including
		 * those from ungetc) cause the position to be
		 * smaller than that in the underlying object.
		 */
		pos -= fp->_r;
		if (HASUB(fp))
			pos -= fp->_ur;
	}
	else if (fp->_flags & IB__SWR && fp->_p != NULL) {
		/*
		 * Writing.  Any buffered characters cause the
		 * position to be greater than that in the
		 * underlying object.
		 */
		pos += fp->_p - fp->_bf._base;
	}
	FUNLOCKFILE(fp);
	return (pos);
}

IB_FILE *ib_funopen(const void *cookie,
					int (*readfn) (),
					int (*writefn) (),
					ib_fpos_t(*seekfn) (void *cookie, ib_fpos_t off,
										int whence), int (*closefn) ())
{
	IB_FILE *fp;
	int flags;

	if (readfn == NULL) {
		if (writefn == NULL) {	/* illegal */
			errno = EINVAL;
			return (NULL);
		}
		else
			flags = IB__SWR;	/* write only */
	}
	else {
		if (writefn == NULL)
			flags = IB__SRD;	/* read only */
		else
			flags = IB__SRW;	/* read-write */
	}
	if ((fp = ib__sfp()) == NULL)
		return (NULL);
	fp->_flags = flags;
	fp->_file = -1;
	fp->_cookie = (void *) cookie;
	fp->_read = readfn;
	fp->_write = writefn;
	fp->_seek = seekfn;
	fp->_close = closefn;
	return (fp);
}


int ib_fwalk(int (*function) (IB_FILE *))
{
	IB_FILE *fp;
	int n, ret;
	struct glue *g;

	ret = 0;
	for (g = &ib__sglue; g != NULL; g = g->next)
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags != 0)
				ret |= (*function) (fp);
	return (ret);
}


/*
 * Write `count' objects (each size `size') from memory to the given file.
 * Return the number of whole objects written.
 */
size_t ib_fwrite(const void *buf, size_t size, size_t count, IB_FILE * fp)
{
	size_t n;
	struct ib__suio uio;
	struct ib__siov iov;

	iov.iov_base = (void *) buf;
	uio.uio_resid = iov.iov_len = n = count * size;
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;

	FLOCKFILE(fp);
	/*
	 * The usual case is success (ib__sfvwrite returns 0);
	 * skip the divide if this happens, since divides are
	 * generally slow and since this occurs whenever size==0.
	 */
	if (ib__sfvwrite(fp, &uio) != 0)
		count = (n - uio.uio_resid) / size;
	FUNLOCKFILE(fp);
	return (count);
}

/*
 * A subroutine version of the macro getc.
 */
#undef ib_getc

int ib_getc(IB_FILE * fp)
{
	int retval;
	FLOCKFILE(fp);
	retval = ib__sgetc(fp);
	FUNLOCKFILE(fp);
	return (retval);
}

/*
 * A subroutine version of the macro getchar.
 */

#undef ib_getchar

int ib_getchar(void)
{
	int retval;
	FLOCKFILE(ib_stdin);
	retval = ib_getc(ib_stdin);
	FUNLOCKFILE(ib_stdin);
	return (retval);
}

int ib_getw(IB_FILE * fp)
{
	int x;

	return (ib_fread((void *) &x, sizeof(x), 1, fp) == 1 ? x : EOF);
}

/*
 * Allocate a file buffer, or switch to unbuffered I/O.
 * Per the ANSI C standard, ALL tty devices default to line buffered.
 *
 * As a side effect, we set IB__SOPT or IB__SNPT (en/dis-able fseek
 * optimisation) right after the fstat() that finds the buffer size.
 */
static void ib__smakebuf(IB_FILE * fp)
{
	void *p;
	int flags;
	size_t size;
	int couldbetty;

	if (fp->_flags & IB__SNBF) {
		fp->_bf._base = fp->_p = fp->_nbuf;
		fp->_bf._size = 1;
		return;
	}
	flags = ib__swhatbuf(fp, &size, &couldbetty);
	if ((p = malloc(size)) == NULL) {
		fp->_flags |= IB__SNBF;
		fp->_bf._base = fp->_p = fp->_nbuf;
		fp->_bf._size = 1;
		return;
	}
	/* ib__cleanup = ib_cleanup; */
	flags |= IB__SMBF;
	fp->_bf._base = fp->_p = p;
	fp->_bf._size = size;
	if (couldbetty && isatty(fp->_file))
		flags |= IB__SLBF;
	fp->_flags |= flags;
}

/*
 * Internal routine to determine `proper' buffering for a file.
 */
int ib__swhatbuf(IB_FILE * fp, size_t * bufsize, int *couldbetty)
{
	struct stat st;

	if (fp->_file < 0 || fstat(fp->_file, &st) < 0) {
		*couldbetty = 0;
		*bufsize = BUFSIZ;
		return (IB__SNPT);
	}

	/* could be a tty iff it is a character device */
	*couldbetty = (st.st_mode & S_IFMT) == S_IFCHR;
	if (st.st_blksize <= 0) {
		*bufsize = BUFSIZ;
		return (IB__SNPT);
	}

	/*
	 * Optimise ib_fseek() only if it is a regular file.  (The test for
	 * ib__sseek is mainly paranoia.)  It is safe to set _blksize
	 * unconditionally; it will only be used if IB__SOPT is also set.
	 */
	*bufsize = st.st_blksize;
	fp->_blksize = st.st_blksize;
	return ((st.st_mode & S_IFMT) == S_IFREG && fp->_seek == ib__sseek ?
			IB__SOPT : IB__SNPT);
}

void ib_perror(const char *s)
{
	struct iovec *v;
	struct iovec iov[4];

	v = iov;
	if (s != NULL && *s != '\0') {
		v->iov_base = (char *) s;
		v->iov_len = strlen(s);
		v++;
		v->iov_base = ": ";
		v->iov_len = 2;
		v++;
	}
	v->iov_base = strerror(errno);
	v->iov_len = strlen(v->iov_base);
	v++;
	v->iov_base = "\n";
	v->iov_len = 1;
	(void) writev(STDERR_FILENO, iov, (v - iov) + 1);
}


int ib_printf(char const *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = ib_vfprintf(ib_stdout, fmt, ap);
	va_end(ap);
	return (ret);
}

/*
 * A subroutine version of the macro putc.
 */
#undef ib_putc

int ib_putc(int c, IB_FILE * fp)
{
	int retval;
	FLOCKFILE(fp);
	retval = ib__sputc(c, fp);
	FUNLOCKFILE(fp);
	return (retval);
}

#undef ib_putchar

/*
 * A subroutine version of the macro putchar
 */
int ib_putchar(int c)
{
	int retval;
	IB_FILE *so = ib_stdout;

	FLOCKFILE(so);
	retval = ib__sputc(c, so);
	FUNLOCKFILE(so);
	return (retval);
}

/*
 * Write the given string to stdout, appending a newline.
 */
int ib_puts(char const *s)
{
	int retval;
	size_t c = strlen(s);
	struct ib__suio uio;
	struct ib__siov iov[2];

	iov[0].iov_base = (void *) s;
	iov[0].iov_len = c;
	iov[1].iov_base = "\n";
	iov[1].iov_len = 1;
	uio.uio_resid = c + 1;
	uio.uio_iov = &iov[0];
	uio.uio_iovcnt = 2;
	FLOCKFILE(ib_stdout);
	retval = ib__sfvwrite(ib_stdout, &uio) ? EOF : '\n';
	FUNLOCKFILE(ib_stdout);
	return (retval);
}

int ib_putw(int w, IB_FILE * fp)
{
	int retval;
	struct ib__suio uio;
	struct ib__siov iov;

	iov.iov_base = &w;
	iov.iov_len = uio.uio_resid = sizeof(w);
	uio.uio_iov = &iov;
	uio.uio_iovcnt = 1;
	FLOCKFILE(fp);
	retval = ib__sfvwrite(fp, &uio);
	FUNLOCKFILE(fp);
	return (retval);
}


static int lflush(IB_FILE * fp)
{

	if ((fp->_flags & (IB__SLBF | IB__SWR)) == (IB__SLBF | IB__SWR))
		return (ib__sflush(fp));
	return (0);
}

/*
 * Refill a stdio buffer.
 * Return EOF on eof or error, 0 otherwise.
 */
int ib__srefill(IB_FILE * fp)
{

	/* make sure stdio is set up */
	if (!ib__sdidinit)
		ib__sinit();

	fp->_r = 0;					/* largely a convenience for callers */

	/* SysV does not make this test; take it out for compatibility */
	if (fp->_flags & IB__SEOF)
		return (EOF);

	/* if not already reading, have to be reading and writing */
	if ((fp->_flags & IB__SRD) == 0) {
		if ((fp->_flags & IB__SRW) == 0) {
			errno = EBADF;
			return (EOF);
		}
		/* switch to reading */
		if (fp->_flags & IB__SWR) {
			if (ib__sflush(fp))
				return (EOF);
			fp->_flags &= ~IB__SWR;
			fp->_w = 0;
			fp->_lbfsize = 0;
		}
		fp->_flags |= IB__SRD;
	}
	else {
		/*
		 * We were reading.  If there is an ungetc buffer,
		 * we must have been reading from that.  Drop it,
		 * restoring the previous buffer (if any).  If there
		 * is anything in that buffer, return.
		 */
		if (HASUB(fp)) {
			FREEUB(fp);
			if ((fp->_r = fp->_ur) != 0) {
				fp->_p = fp->_up;
				return (0);
			}
		}
	}

	if (fp->_bf._base == NULL)
		ib__smakebuf(fp);

	/*
	 * Before reading from a line buffered or unbuffered file,
	 * flush all line buffered output files, per the ANSI C
	 * standard.
	 */
	if (fp->_flags & (IB__SLBF | IB__SNBF))
		(void) ib_fwalk(lflush);
	fp->_p = fp->_bf._base;
	fp->_r = (*fp->_read) (fp->_cookie, (char *) fp->_p, fp->_bf._size);
	fp->_flags &= ~IB__SMOD;	/* buffer contents are again pristine */
	if (fp->_r <= 0) {
		if (fp->_r == 0)
			fp->_flags |= IB__SEOF;
		else {
			fp->_r = 0;
			fp->_flags |= IB__SERR;
		}
		return (EOF);
	}
	return (0);
}


void ib_rewind(IB_FILE * fp)
{
	FLOCKFILE(fp);
	(void) ib_fseek(fp, 0L, SEEK_SET);
	ib_clearerr(fp);
	FUNLOCKFILE(fp);
	errno = 0;					/* not required, but seems reasonable */
}

int ib__srefill(IB_FILE *);

/*
 * Handle getc() when the buffer ran out:
 * Refill, then return the first character
 * in the newly-filled buffer.
 */
int ib__srget(IB_FILE * fp)
{
	if (ib__srefill(fp) == 0) {
		fp->_r--;
		return (*fp->_p++);
	}
	return (EOF);
}
int ib_scanf(char const *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	FLOCKFILE(ib_stdin);
	ret = ib__svfscanf(ib_stdin, fmt, ap);
	FUNLOCKFILE(ib_stdin);
	va_end(ap);
	return (ret);
}

void ib_setbuf(IB_FILE * fp, char *buf)
{
	(void) ib_setvbuf(fp, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}

void ib_setbuffer(IB_FILE * fp, char *buf, int size)
{

	(void) ib_setvbuf(fp, buf, buf ? _IOFBF : _IONBF, (size_t) size);
}

/*
 * set line buffering
 */
int ib_setlinebuf(IB_FILE * fp)
{

	return (ib_setvbuf(fp, (char *) NULL, _IOLBF, (size_t) 0));
}

/*
 * Set one of the three kinds of buffering, optionally including
 * a buffer.
 */
int ib_setvbuf(IB_FILE * fp,
			   char *buf, int mode, size_t size)
{
	int ret, flags;
	size_t iosize;
	int ttyflag;

	/*
	 * Verify arguments.  The `int' limit on `size' is due to this
	 * particular implementation.  Note, buf and size are ignored
	 * when setting _IONBF.
	 */
	if (mode != _IONBF)
		if ((mode != _IOFBF && mode != _IOLBF) || (int) size < 0)
			return (EOF);

	FLOCKFILE(fp);
	/*
	 * Write current buffer, if any.  Discard unread input (including
	 * ungetc data), cancel line buffering, and free old buffer if
	 * malloc()ed.  We also clear any eof condition, as if this were
	 * a seek.
	 */
	ret = 0;
	(void) ib__sflush(fp);
	if (HASUB(fp))
		FREEUB(fp);
	fp->_r = fp->_lbfsize = 0;
	flags = fp->_flags;
	if (flags & IB__SMBF)
		free((void *) fp->_bf._base);
	flags &= ~(IB__SLBF | IB__SNBF | IB__SMBF | IB__SOPT |
			   IB__SNPT | IB__SEOF);

	/* If setting unbuffered mode, skip all the hard work. */
	if (mode == _IONBF)
		goto nbf;

	/*
	 * Find optimal I/O size for seek optimization.  This also returns
	 * a `tty flag' to suggest that we check isatty(fd), but we do not
	 * care since our caller told us how to buffer.
	 */
	flags |= ib__swhatbuf(fp, &iosize, &ttyflag);
	if (size == 0) {
		buf = NULL;				/* force local allocation */
		size = iosize;
	}

	/* Allocate buffer if needed. */
	if (buf == NULL) {
		if ((buf = malloc(size)) == NULL) {
			/*
			 * Unable to honor user's request.  We will return
			 * failure, but try again with file system size.
			 */
			ret = EOF;
			if (size != iosize) {
				size = iosize;
				buf = malloc(size);
			}
		}
		if (buf == NULL) {
			/* No luck; switch to unbuffered I/O. */
		  nbf:
			fp->_flags = flags | IB__SNBF;
			fp->_w = 0;
			fp->_bf._base = fp->_p = fp->_nbuf;
			fp->_bf._size = 1;
			FUNLOCKFILE(fp);
			return (ret);
		}
		flags |= IB__SMBF;
	}

	/*
	 * Kill any seek optimization if the buffer is not the
	 * right size.
	 *
	 * SHOULD WE ALLOW MULTIPLES HERE (i.e., ok iff (size % iosize) == 0)?
	 */
	if (size != iosize)
		flags |= IB__SNPT;

	/*
	 * Fix up the FILE fields, and set ib__cleanup for output flush on
	 * exit (since we are buffered in some way).
	 */
	if (mode == _IOLBF)
		flags |= IB__SLBF;
	fp->_flags = flags;
	fp->_bf._base = fp->_p = (unsigned char *) buf;
	fp->_bf._size = size;
	/* fp->_lbfsize is still 0 */
	if (flags & IB__SWR) {
		/*
		 * Begin or continue writing: see ib__swsetup().  Note
		 * that IB__SNBF is impossible (it was handled earlier).
		 */
		if (flags & IB__SLBF) {
			fp->_w = 0;
			fp->_lbfsize = -fp->_bf._size;
		}
		else
			fp->_w = size;
	}
	else {
		/* begin/continue reading, or stay in intermediate state */
		fp->_w = 0;
	}
	/* ib__cleanup = ib_cleanup; */

	FUNLOCKFILE(fp);
	return (ret);
}

/*
 * Small standard I/O/seek/close functions.
 * These maintain the `known seek offset' for seek optimisation.
 */
static int ib__sread(void *cookie, char *buf, int n)
{
	IB_FILE *fp = cookie;
	int ret;

	ret = read(fp->_file, buf, (size_t) n);
	/* if the read succeeded, update the current offset */
	if (ret >= 0)
		fp->_offset += ret;
	else
		fp->_flags &= ~IB__SOFF;	/* paranoia */
	return (ret);
}

static int ib__swrite(void *cookie, char const *buf, int n)
{
	IB_FILE *fp = cookie;

	if (fp->_flags & IB__SAPP)
		(void) lseek(fp->_file, (off_t) 0, SEEK_END);
	fp->_flags &= ~IB__SOFF;	/* in case FAPPEND mode is set */
	return (write(fp->_file, buf, (size_t) n));
}

static ib_fpos_t ib__sseek(void *cookie, ib_fpos_t offset, int whence)
{
	IB_FILE *fp = cookie;
	off_t ret;

	ret = lseek(fp->_file, (off_t) offset, whence);
	if (ret == -1)
		fp->_flags &= ~IB__SOFF;
	else {
		fp->_flags |= IB__SOFF;
		fp->_offset = ret;
	}
	return (ret);
}

static int ib__sclose(void *cookie)
{
	return (close(((IB_FILE *) cookie)->_file));
}

IB_FILE *ib_tmpfile(void)
{
	sigset_t set, oset;
	IB_FILE *fp;
	int fd, sverrno;
#define	TRAILER	"tmp.XXXXXX"
	char buf[sizeof(_PATH_TMP) + sizeof(TRAILER)];

	(void) memcpy(buf, _PATH_TMP, sizeof(_PATH_TMP) - 1);
	(void) memcpy(buf + sizeof(_PATH_TMP) - 1, TRAILER, sizeof(TRAILER));

	sigfillset(&set);
	(void) sigprocmask(SIG_BLOCK, &set, &oset);

	fd = mkstemp(buf);
	if (fd != -1)
		(void) unlink(buf);

	(void) sigprocmask(SIG_SETMASK, &oset, NULL);

	if (fd == -1)
		return (NULL);

	if ((fp = ib_fdopen(fd, "w+")) == NULL) {
		sverrno = errno;
		(void) close(fd);
		errno = sverrno;
		return (NULL);
	}
	return (fp);
}

/*
 * Expand the ungetc buffer `in place'.  That is, adjust fp->_p when
 * the buffer moves, so that it points the same distance from the end,
 * and move the bytes in the buffer around as necessary so that they
 * are all at the end (stack-style).
 */
static int ib__submore(IB_FILE * fp)
{
	int i;
	unsigned char *p;

	if (fp->_ub._base == fp->_ubuf) {
		/*
		 * Get a new buffer (rather than expanding the old one).
		 */
		if ((p = malloc((size_t) BUFSIZ)) == NULL)
			return (EOF);
		fp->_ub._base = p;
		fp->_ub._size = BUFSIZ;
		p += BUFSIZ - sizeof(fp->_ubuf);
		for (i = sizeof(fp->_ubuf); --i >= 0;)
			p[i] = fp->_ubuf[i];
		fp->_p = p;
		return (0);
	}
	i = fp->_ub._size;
	p = realloc(fp->_ub._base, (size_t) (i << 1));
	if (p == NULL)
		return (EOF);
	/* no overlap (hence can use memcpy) because we doubled the size */
	(void) memcpy((void *) (p + i), (void *) p, (size_t) i);
	fp->_p = p + i;
	fp->_ub._base = p;
	fp->_ub._size = i << 1;
	return (0);
}

int ib_ungetc(int c, IB_FILE * fp)
{
	if (c == EOF)
		return (EOF);
	if (!ib__sdidinit)
		ib__sinit();
	FLOCKFILE(fp);
	if ((fp->_flags & IB__SRD) == 0) {
		/*
		 * Not already reading: no good unless reading-and-writing.
		 * Otherwise, flush any current write stuff.
		 */
		if ((fp->_flags & IB__SRW) == 0) {
			FUNLOCKFILE(fp);
			return (EOF);
		}
		if (fp->_flags & IB__SWR) {
			if (ib__sflush(fp)) {
				FUNLOCKFILE(fp);
				return (EOF);
			}
			fp->_flags &= ~IB__SWR;
			fp->_w = 0;
			fp->_lbfsize = 0;
		}
		fp->_flags |= IB__SRD;
	}
	c = (unsigned char) c;

	/*
	 * If we are in the middle of ungetc'ing, just continue.
	 * This may require expanding the current ungetc buffer.
	 */
	if (HASUB(fp)) {
		if (fp->_r >= fp->_ub._size && ib__submore(fp)) {
			FUNLOCKFILE(fp);
			return (EOF);
		}
		*--fp->_p = c;
		fp->_r++;
		FUNLOCKFILE(fp);
		return (c);
	}
	fp->_flags &= ~IB__SEOF;

	/*
	 * If we can handle this by simply backing up, do so,
	 * but never replace the original character.
	 * (This makes sscanf() work when scanning `const' data.)
	 */
	if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
		fp->_p--;
		fp->_r++;
		FUNLOCKFILE(fp);
		return (c);
	}

	/*
	 * Create an ungetc buffer.
	 * Initially, we will use the `reserve' buffer.
	 */
	fp->_ur = fp->_r;
	fp->_up = fp->_p;
	fp->_ub._base = fp->_ubuf;
	fp->_ub._size = sizeof(fp->_ubuf);
	fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
	fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
	fp->_r = 1;
	FUNLOCKFILE(fp);
	return (c);
}

/*
 * Actual printf innards.
 *
 * This code is large and complicated...
 */

/* Define FLOATING_POINT to get floating point. */
#define	FLOATING_POINT

static int ib__sprint(IB_FILE *, struct ib__suio *);
static int ib__sbprintf(IB_FILE *, const char *, va_list);
static char *ib__ultoa(u_long, char *, int, int, char *);
static char *ib__uqtoa(u_quad_t, char *, int, int, char *);
static void ib__find_arguments(const char *, va_list, void ***);
static void ib__grow_type_table(int, unsigned char **, int *);

/*
 * Flush out all the vectors defined by the given uio,
 * then reset it so that it can be reused.
 */
static int ib__sprint(IB_FILE * fp, struct ib__suio *uio)
{
	int err;

	if (uio->uio_resid == 0) {
		uio->uio_iovcnt = 0;
		return (0);
	}
	err = ib__sfvwrite(fp, uio);
	uio->uio_resid = 0;
	uio->uio_iovcnt = 0;
	return (err);
}

/*
 * Helper function for `fprintf to unbuffered unix file': creates a
 * temporary buffer.  We only work on write-only files; this avoids
 * worries about ungetc buffers and so forth.
 */
static int ib__sbprintf(IB_FILE * fp, const char *fmt, va_list ap)
{
	int ret;
	IB_FILE fake;
	unsigned char buf[BUFSIZ];

	/* copy the important variables */
	fake._flags = fp->_flags & ~IB__SNBF;
	fake._file = fp->_file;
	fake._cookie = fp->_cookie;
	fake._write = fp->_write;

	/* set up the buffer */
	fake._bf._base = fake._p = buf;
	fake._bf._size = fake._w = sizeof(buf);
	fake._lbfsize = 0;			/* not actually used, but Just In Case */

	/* do the work, then copy any error status */
	ret = ib_vfprintf(&fake, fmt, ap);
	if (ret >= 0 && ib_fflush(&fake))
		ret = EOF;
	if (fake._flags & IB__SERR)
		fp->_flags |= IB__SERR;
	return (ret);
}

/*
 * Macros for converting digits to letters and vice versa
 */
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
 * Convert an unsigned long to ASCII for printf purposes, returning
 * a pointer to the first character of the string representation.
 * Octal numbers can be forced to have a leading zero; hex numbers
 * use the given digits.
 */
static char *ib__ultoa(u_long val, char *endp, int base, int octzero,
					   char *xdigs)
{
	char *cp = endp;
	long sval;

	/*
	 * Handle the three cases separately, in the hope of getting
	 * better/faster code.
	 */
	switch (base) {
	case 10:
		if (val < 10) {			/* many numbers are 1 digit */
			*--cp = to_char(val);
			return (cp);
		}
		/*
		 * On many machines, unsigned arithmetic is harder than
		 * signed arithmetic, so we do at most one unsigned mod and
		 * divide; this is sufficient to reduce the range of
		 * the incoming value to where signed arithmetic works.
		 */
		if (val > LONG_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		}
		else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:					/* oops */
		abort();
	}
	return (cp);
}

/* Identical to ib__ultoa, but for quads. */
static char *ib__uqtoa(u_quad_t val, char *endp,
					   int base, int octzero, char *xdigs)
{
	char *cp = endp;
	quad_t sval;

	/* quick test for small values; ib__ultoa is typically much faster */
	/* (perhaps instead we should run until small, then call ib__ultoa?) */
	if (val <= ULONG_MAX)
		return (ib__ultoa((u_long) val, endp, base, octzero, xdigs));
	switch (base) {
	case 10:
		if (val < 10) {
			*--cp = to_char(val % 10);
			return (cp);
		}
		if (val > QUAD_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		}
		else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:
		abort();
	}
	return (cp);
}


#define	BUF		(MAXEXP+MAXFRACT+1)	/* + decimal point */
#define	DEFPREC		6

static char *cvt(double, int, int, char *, int *, int, int *);
static int exponent(char *, int, int);

#ifdef SOLARIS

/* On Solaris 2.6 or 7, "man isinf" will tell you to include <sunmath.h>
 * and link with -lsunmath.  Unfortunatel, neither /usr/include/sunmath.h
 * nor /usr/lib/libsunmath.* exists on Sparc Solaris 2.6 or 7 systems
 * available to us, so we supply a substitute here.
 */
static int isinf(double x)
{
	fpclass_t class_;

	class_ = fpclass(x);
	return (FP_NINF == class_) || (FP_PINF == class_);
}

#endif


#define STATIC_ARG_TBL_SIZE 8	/* Size of static argument table. */

/*
 * Flags used during conversion.
 */
#define	ALT		0x001			/* alternate form */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix */
#define	LADJUST		0x004		/* left adjustment */
#define	LONGDBL		0x008		/* long double */
#define	LONGINT		0x010		/* long integer */
#define	QUADINT		0x020		/* quad integer */
#define	SHORTINT	0x040		/* short integer */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */
#define FPT		0x100			/* Floating point number */
int ib_vfprintf(IB_FILE * fp, const char *fmt0, va_list ap)
{
	char *fmt;			/* format string */
	int ch;			/* character from fmt */
	int n, n2;			/* handy integer (short term usage) */
	char *cp;			/* handy char pointer (short term usage) */
	struct ib__siov *iovp;	/* for PRINT macro */
	int flags;			/* flags as above */
	int ret;					/* return value accumulator */
	int width;					/* width from format (%8d), or 0 */
	int prec;					/* precision from format (%.3d), or -1 */
	char sign;					/* sign prefix (' ', '+', '-', or \0) */
	char softsign;				/* temporary negative sign for floats */
	double _double;				/* double precision arguments %[eEfgG] */
	int expt;					/* integer value of exponent */
	int expsize;				/* character count for expstr */
	int ndig;					/* actual number of digits returned by cvt */
	char expstr[7];				/* buffer for exponent string */
	u_long ulval;				/* integer arguments %[diouxX] */
	u_quad_t uqval;				/* %q integers */
	int base;					/* base for [diouxX] conversion */
	int dprec;					/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;					/* field size expanded by dprec, sign, etc */
	int size;					/* size of converted field or string */
	int prsize;					/* max size of printed field */
	char *xdigs;				/* digits for [xX] conversion */
#define NIOV 8
	struct ib__suio uio;		/* output information: summary */
	struct ib__siov iov[NIOV];	/* ... and individual io vectors */
	char buf[BUF];				/* space for %c, %[diouxX], %[eEfgG] */
	char ox[2];					/* space for 0x hex-prefix */
	void **argtable;			/* args, built due to positional arg */
	void *statargtable[STATIC_ARG_TBL_SIZE];
	int nextarg;				/* 1-based argument index */
	va_list orgap;				/* original argument pointer */

	/*
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initialisers
	 * below longer.
	 */
#define	PADSIZE	16				/* pad chunk size */
	static char blanks[PADSIZE] =
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' '
	};
	static char zeroes[PADSIZE] =
		{ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
		'0', '0', '0'
	};

	/*
	 * BEWARE, these `goto error' on error, and PAD uses `n'.
	 */
#define	PRINT(ptr, len) { \
	iovp->iov_base = (ptr); \
	iovp->iov_len = (len); \
	uio.uio_resid += (len); \
	iovp++; \
	if (++uio.uio_iovcnt >= NIOV) { \
		if (ib__sprint(fp, &uio)) \
			goto error; \
		iovp = iov; \
	} \
}
#define	PAD(howmany, with) { \
	if ((n = (howmany)) > 0) { \
		while (n > PADSIZE) { \
			PRINT(with, PADSIZE); \
			n -= PADSIZE; \
		} \
		PRINT(with, n); \
	} \
}
#define	FLUSH() { \
	if (uio.uio_resid && ib__sprint(fp, &uio)) \
		goto error; \
	uio.uio_iovcnt = 0; \
	iovp = iov; \
}

	/*
	 * Get the argument indexed by nextarg.   If the argument table is
	 * built, use it to get the argument.  If its not, get the next
	 * argument (and arguments must be gotten sequentially).
	 */
#define GETARG(type) \
        ((argtable != NULL) ? *((type*)(argtable[nextarg++])) : \
            (nextarg++, va_arg(ap, type)))

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#define	SARG() \
	(flags&LONGINT ? GETARG(long) : \
	    flags&SHORTINT ? (long)(short)GETARG(int) : \
	    (long)GETARG(int))
#define	UARG() \
	(flags&LONGINT ? GETARG(u_long) : \
	    flags&SHORTINT ? (u_long)(u_short)GETARG(int) : \
	    (u_long)GETARG(u_int))

	/*
	 * Get * arguments, including the form *nn$.  Preserve the nextarg
	 * that the argument can be gotten once the type is determined.
	 */
#define GETASTER(val) \
        n2 = 0; \
        cp = fmt; \
        while (is_digit(*cp)) { \
                n2 = 10 * n2 + to_digit(*cp); \
                cp++; \
        } \
        if (*cp == '$') { \
            	int hold = nextarg; \
                if (argtable == NULL) { \
                        argtable = statargtable; \
                        ib__find_arguments (fmt0, orgap, &argtable); \
                } \
                nextarg = n2; \
                val = GETARG (int); \
                nextarg = hold; \
                fmt = ++cp; \
        } else { \
		val = GETARG (int); \
        }


	FLOCKFILE(fp);
	/* sorry, ib_fprintf(read_only_file, "") returns EOF, not 0 */
	if (cantwrite(fp)) {
		FUNLOCKFILE(fp);
		return (EOF);
	}

	/* optimise ib_fprintf(stderr) (and other unbuffered Unix files) */
	if ((fp->_flags & (IB__SNBF | IB__SWR | IB__SRW)) == (IB__SNBF | IB__SWR)
		&& fp->_file >= 0) {
		FUNLOCKFILE(fp);
		return (ib__sbprintf(fp, fmt0, ap));
	}

	fmt = (char *) fmt0;
	argtable = NULL;
	nextarg = 1;
	orgap = ap;
	uio.uio_iov = iovp = iov;
	uio.uio_resid = 0;
	uio.uio_iovcnt = 0;
	ret = 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */ ;
		if ((n = fmt - cp) != 0) {
			if ((unsigned) ret + n > INT_MAX) {
				ret = EOF;
				goto error;
			}
			PRINT(cp, n);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;					/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

	  rflag:ch = *fmt++;
	  reswitch:switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *  -- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *  -- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			GETASTER(width);
			if (width >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				GETASTER(n);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *  -- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				if (argtable == NULL) {
					argtable = statargtable;
					ib__find_arguments(fmt0, orgap, &argtable);
				}
				goto rflag;
			}
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (flags & LONGINT)
				flags |= QUADINT;
			else
				flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			*(cp = buf) = GETARG(int);
			size = 1;
			sign = '\0';
			break;
		case 'D':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'd':
		case 'i':
			if (flags & QUADINT) {
				uqval = GETARG(quad_t);
				if ((quad_t) uqval < 0) {
					uqval = -uqval;
					sign = '-';
				}
			}
			else {
				ulval = SARG();
				if ((long) ulval < 0) {
					ulval = -ulval;
					sign = '-';
				}
			}
			base = 10;
			goto number;
		case 'e':
		case 'E':
		case 'f':
			goto fp_begin;
		case 'g':
		case 'G':
			if (prec == 0)
				prec = 1;
		  fp_begin:if (prec == -1)
				prec = DEFPREC;
			if (flags & LONGDBL)
				/* XXX this loses precision. */
				_double = (double) GETARG(long double);
			else
				_double = GETARG(double);
			/* do this before tricky precision changes */
			if (isinf(_double)) {
				if (_double < 0)
					sign = '-';
				cp = "Inf";
				size = 3;
				break;
			}
			if (isnan(_double)) {
				cp = "NaN";
				size = 3;
				break;
			}
			flags |= FPT;
			cp = cvt(_double, prec, flags, &softsign, &expt, ch, &ndig);
			if (ch == 'g' || ch == 'G') {
				if (expt <= -4 || expt > prec)
					ch = (ch == 'g') ? 'e' : 'E';
				else
					ch = 'g';
			}
			if (ch <= 'e') {	/* 'e' or 'E' fmt */
				--expt;
				expsize = exponent(expstr, expt, ch);
				size = expsize + ndig;
				if (ndig > 1 || flags & ALT)
					++size;
			}
			else if (ch == 'f') {	/* f fmt */
				if (expt > 0) {
					size = expt;
					if (prec || flags & ALT)
						size += prec + 1;
				}
				else			/* "0.X" */
					size = prec + 2;
			}
			else if (expt >= ndig) {	/* fixed g fmt */
				size = expt;
				if (flags & ALT)
					++size;
			}
			else
				size = ndig + (expt > 0 ? 1 : 2 - expt);

			if (softsign)
				sign = '-';
			break;

		case 'n':
			if (flags & QUADINT)
				*GETARG(quad_t *) = ret;
			else if (flags & LONGINT)
				*GETARG(long *) = ret;
			else if (flags & SHORTINT)
				*GETARG(short *) = ret;
			else
				*GETARG(int *) = ret;
			continue;			/* no output */
		case 'O':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'o':
			if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
				ulval = UARG();
			base = 8;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *  -- ANSI X3J11
			 */
			ulval = (u_long) GETARG(void *);
			base = 16;
			xdigs = "0123456789abcdef";
			flags = (flags & ~QUADINT) | HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
			if ((cp = GETARG(char *)) == NULL)
				  cp = "(null)";
			if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
				char *p = memchr(cp, 0, (size_t) prec);

				if (p != NULL) {
					size = p - cp;
					if (size > prec)
						size = prec;
				}
				else
					size = prec;
			}
			else
				size = strlen(cp);
			sign = '\0';
			break;
		case 'U':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'u':
			if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
				ulval = UARG();
			base = 10;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
		  hex:if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
				ulval = UARG();
			base = 16;
			/* leading 0x/X only if non-zero */
			if (flags & ALT && (flags & QUADINT ? uqval != 0 : ulval != 0))
				flags |= HEXPREFIX;

			/* unsigned conversions */
		  nosign:sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *  -- ANSI X3J11
			 */
		  number:if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *  -- ANSI X3J11
			 */
			cp = buf + BUF;
			if (flags & QUADINT) {
				if (uqval != 0 || prec != 0)
					cp = ib__uqtoa(uqval, cp, base, flags & ALT, xdigs);
			}
			else {
				if (ulval != 0 || prec != 0)
					cp = ib__ultoa(ulval, cp, base, flags & ALT, xdigs);
			}
			size = buf + BUF - cp;
			break;
		default:				/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz += 2;

		prsize = width > realsz ? width : realsz;
		if ((unsigned) ret + prsize > INT_MAX) {
			ret = EOF;
			goto error;
		}

		/* right-adjusting blank padding */
		if ((flags & (LADJUST | ZEROPAD)) == 0)
			PAD(width - realsz, blanks);

		/* prefix */
		if (sign) {
			PRINT(&sign, 1);
		}
		else if (flags & HEXPREFIX) {
			ox[0] = '0';
			ox[1] = ch;
			PRINT(ox, 2);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST | ZEROPAD)) == ZEROPAD)
			PAD(width - realsz, zeroes);

		/* leading zeroes from decimal precision */
		PAD(dprec - size, zeroes);

		/* the string or number proper */

		if ((flags & FPT) == 0) {
			PRINT(cp, size);
		}
		else {					/* glue together f_p fragments */
			if (ch >= 'f') {	/* 'f' or 'g' */
				if (_double == 0) {
					/* kludge for ib__dtoa irregularity */
					if (expt >= ndig && (flags & ALT) == 0) {
						PRINT("0", 1);
					}
					else {
						PRINT("0.", 2);
						PAD(ndig - 1, zeroes);
					}
				}
				else if (expt <= 0) {
					PRINT("0.", 2);
					PAD(-expt, zeroes);
					PRINT(cp, ndig);
				}
				else if (expt >= ndig) {
					PRINT(cp, ndig);
					PAD(expt - ndig, zeroes);
					if (flags & ALT)
						PRINT(".", 1);
				}
				else {
					PRINT(cp, expt);
					cp += expt;
					PRINT(".", 1);
					PRINT(cp, ndig - expt);
				}
			}
			else {				/* 'e' or 'E' */
				if (ndig > 1 || flags & ALT) {
					ox[0] = *cp++;
					ox[1] = '.';
					PRINT(ox, 2);
					if (_double) {
						PRINT(cp, ndig - 1);
					}
					else		/* 0.[0..] */
						/* ib__dtoa irregularity */
						PAD(ndig - 1, zeroes);
				}
				else			/* XeYYY */
					PRINT(cp, 1);
				PRINT(expstr, expsize);
			}
		}

		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += prsize;

		FLUSH();				/* copy out the I/O vectors */
	}
  done:
	FLUSH();
  error:
	if (ib__sferror(fp))
		ret = EOF;
	FUNLOCKFILE(fp);
	if ((argtable != NULL) && (argtable != statargtable))
		free(argtable);
	return (ret);
	/* NOTREACHED */
}

/*
 * Type ids for argument type table.
 */
#define T_UNUSED	0
#define T_SHORT		1
#define T_U_SHORT	2
#define TP_SHORT	3
#define T_INT		4
#define T_U_INT		5
#define TP_INT		6
#define T_LONG		7
#define T_U_LONG	8
#define TP_LONG		9
#define T_QUAD		10
#define T_U_QUAD	11
#define TP_QUAD		12
#define T_DOUBLE	13
#define T_LONG_DOUBLE	14
#define TP_CHAR		15
#define TP_VOID		16

/*
 * Find all arguments when a positional parameter is encountered.  Returns a
 * table, indexed by argument number, of pointers to each arguments.  The
 * initial argument table should be an array of STATIC_ARG_TBL_SIZE entries.
 * It will be replaces with a malloc-ed on if it overflows.
 */
static void ib__find_arguments(const char *fmt0, va_list ap, void ***argtable)
{
	char *fmt;			/* format string */
	int ch;			/* character from fmt */
	int n, n2;			/* handy integer (short term usage) */
	char *cp;			/* handy char pointer (short term usage) */
	int flags;			/* flags as above */
	int width;					/* width from format (%8d), or 0 */
	unsigned char *typetable;	/* table of types */
	unsigned char stattypetable[STATIC_ARG_TBL_SIZE];
	int tablesize;				/* current size of type table */
	int tablemax;				/* largest used index in table */
	int nextarg;				/* 1-based argument index */

	/*
	 * Add an argument type to the table, expanding if necessary.
	 */
	/* Solaris C was complaining that the two sides of the next ":"
	 * operator have different types, so we cast the 0 to void,
	 * so it will match the void return type of the function on the left.
	 */
	/* ib__grow_type_table(nextarg, &typetable, &tablesize) : 0,  */
#define ADDTYPE(type) \
	((nextarg >= tablesize) ? \
		ib__grow_type_table(nextarg, &typetable, &tablesize) : \
	         (void) 0, \
	typetable[nextarg++] = type, \
	(nextarg > tablemax) ? tablemax = nextarg : 0)

#define	ADDSARG() \
	((flags&LONGINT) ? ADDTYPE(T_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_SHORT) : ADDTYPE(T_INT)))

#define	ADDUARG() \
	((flags&LONGINT) ? ADDTYPE(T_U_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_U_SHORT) : ADDTYPE(T_U_INT)))

	/*
	 * Add * arguments to the type array.
	 */
#define ADDASTER() \
	n2 = 0; \
	cp = fmt; \
	while (is_digit(*cp)) { \
		n2 = 10 * n2 + to_digit(*cp); \
		cp++; \
	} \
	if (*cp == '$') { \
		int hold = nextarg; \
		nextarg = n2; \
		ADDTYPE (T_INT); \
		nextarg = hold; \
		fmt = ++cp; \
	} else { \
		ADDTYPE (T_INT); \
	}
	fmt = (char *) fmt0;
	typetable = stattypetable;
	tablesize = STATIC_ARG_TBL_SIZE;
	tablemax = 0;
	nextarg = 1;
	memset(typetable, T_UNUSED, STATIC_ARG_TBL_SIZE);

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */ ;
		if (ch == '\0')
			goto done;
		fmt++;					/* skip over '%' */

		flags = 0;
		width = 0;

	  rflag:ch = *fmt++;
	  reswitch:switch (ch) {
		case ' ':
		case '#':
			goto rflag;
		case '*':
			/*            ADDASTER (); */
			goto rflag;
		case '-':
		case '+':
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				ADDASTER();
				goto rflag;
			}
			while (is_digit(ch)) {
				ch = *fmt++;
			}
			goto reswitch;
		case '0':
			goto rflag;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				goto rflag;
			}
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			if (flags & LONGINT)
				flags |= QUADINT;
			else
				flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			ADDTYPE(T_INT);
			break;
		case 'D':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'd':
		case 'i':
			if (flags & QUADINT) {
				ADDTYPE(T_QUAD);
			}
			else {
				ADDSARG();
			}
			break;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			if (flags & LONGDBL)
				ADDTYPE(T_LONG_DOUBLE);
			else
				ADDTYPE(T_DOUBLE);
			break;
#endif /* FLOATING_POINT */
		case 'n':
			if (flags & QUADINT)
				ADDTYPE(TP_QUAD);
			else if (flags & LONGINT)
				ADDTYPE(TP_LONG);
			else if (flags & SHORTINT)
				ADDTYPE(TP_SHORT);
			else
				ADDTYPE(TP_INT);
			continue;			/* no output */
		case 'O':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'o':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		case 'p':
			ADDTYPE(TP_VOID);
			break;
		case 's':
			ADDTYPE(TP_CHAR);
			break;
		case 'U':
			flags |= LONGINT;
		 /*FALLTHROUGH*/ case 'u':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		case 'X':
		case 'x':
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
				ADDUARG();
			break;
		default:				/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			break;
		}
	}
  done:
	/*
	 * Build the argument table.
	 */
	if (tablemax >= STATIC_ARG_TBL_SIZE) {
		*argtable = (void **)
			malloc(sizeof(void *) * (tablemax + 1));
	}

	(*argtable)[0] = NULL;
	for (n = 1; n <= tablemax; n++) {
		switch (typetable[n]) {
		case T_UNUSED:
			(*argtable)[n] = (void *) &va_arg(ap, int);
			break;
		case T_SHORT:
			(*argtable)[n] = (void *) &va_arg(ap, int);
			break;
		case T_U_SHORT:
			(*argtable)[n] = (void *) &va_arg(ap, int);
			break;
		case TP_SHORT:
			(*argtable)[n] = (void *) &va_arg(ap, short *);
			break;
		case T_INT:
			(*argtable)[n] = (void *) &va_arg(ap, int);
			break;
		case T_U_INT:
			(*argtable)[n] = (void *) &va_arg(ap, unsigned int);
			break;
		case TP_INT:
			(*argtable)[n] = (void *) &va_arg(ap, int *);
			break;
		case T_LONG:
			(*argtable)[n] = (void *) &va_arg(ap, long);
			break;
		case T_U_LONG:
			(*argtable)[n] = (void *) &va_arg(ap, unsigned long);
			break;
		case TP_LONG:
			(*argtable)[n] = (void *) &va_arg(ap, long *);
			break;
		case T_QUAD:
			(*argtable)[n] = (void *) &va_arg(ap, quad_t);
			break;
		case T_U_QUAD:
			(*argtable)[n] = (void *) &va_arg(ap, u_quad_t);
			break;
		case TP_QUAD:
			(*argtable)[n] = (void *) &va_arg(ap, quad_t *);
			break;
		case T_DOUBLE:
			(*argtable)[n] = (void *) &va_arg(ap, double);
			break;
		case T_LONG_DOUBLE:
			(*argtable)[n] = (void *) &va_arg(ap, long double);
			break;
		case TP_CHAR:
			(*argtable)[n] = (void *) &va_arg(ap, char *);
			break;
		case TP_VOID:
			(*argtable)[n] = (void *) &va_arg(ap, void *);
			break;
		}
	}

	if ((typetable != NULL) && (typetable != stattypetable))
		free(typetable);
}

/*
 * Increase the size of the type table.
 */
static void
ib__grow_type_table(int nextarg, unsigned char **typetable, int *tablesize)
{
	unsigned char *oldtable = *typetable;
	int newsize = *tablesize * 2;

	if (*tablesize == STATIC_ARG_TBL_SIZE) {
		*typetable = (unsigned char *)
			malloc(sizeof(unsigned char) * newsize);
		bcopy(oldtable, *typetable, *tablesize);
	}
	else {
		*typetable = (unsigned char *)
			reallocf(typetable, sizeof(unsigned char) * newsize);

	}
	memset(&typetable[*tablesize], T_UNUSED, (newsize - *tablesize));

	*tablesize = newsize;
}


static char *cvt(double value, int ndigits, int flags, char *sign,
				 int *decpt, int ch, int *length)
{
	int mode, dsgn;
	char *digits, *bp, *rve;

	if (ch == 'f')
		mode = 3;				/* ndigits after the decimal point */
	else {
		/*
		 * To obtain ndigits after the decimal point for the 'e'
		 * and 'E' formats, round to ndigits + 1 significant
		 * figures.
		 */
		if (ch == 'e' || ch == 'E')
			ndigits++;
		mode = 2;				/* ndigits significant digits */
	}
	if (value < 0) {
		value = -value;
		*sign = '-';
	}
	else
		*sign = '\000';
	digits = ib__dtoa(value, mode, ndigits, decpt, &dsgn, &rve);
	if ((ch != 'g' && ch != 'G') || flags & ALT) {
		/* print trailing zeros */
		bp = digits + ndigits;
		if (ch == 'f') {
			if (*digits == '0' && value)
				*decpt = -ndigits + 1;
			bp += *decpt;
		}
		if (value == 0)			/* kludge for ib__dtoa irregularity */
			rve = bp;
		while (rve < bp)
			*rve++ = '0';
	}
	*length = rve - digits;
	return (digits);
}

static int exponent(char *p0, int exp, int fmtch)
{
	char *p, *t;
	char expbuf[MAXEXP];

	p = p0;
	*p++ = fmtch;
	if (exp < 0) {
		exp = -exp;
		*p++ = '-';
	}
	else
		*p++ = '+';
	t = expbuf + MAXEXP;
	if (exp > 9) {
		do {
			*--t = to_char(exp % 10);
		} while ((exp /= 10) > 9);
		*--t = to_char(exp);
		for (; t < expbuf + MAXEXP; *p++ = *t++);
	}
	else {
		*p++ = '0';
		*p++ = to_char(exp);
	}
	return (p - p0);
}


#define	BUF		513				/* Maximum length of numeric string. */

/*
 * Flags used during conversion.
 */
#define	LONG		0x01		/* l: long or double */
#define	LONGDBL		0x02		/* L: long double */
#define	SHORT		0x04		/* h: short */
#define	SUPPRESS	0x08		/* suppress assignment */
#define	POINTER		0x10		/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x20		/* do not skip blanks */
#define	QUAD		0x400

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */
#define	SIGNOK		0x40		/* +/- is (still) legal */
#define	NDIGITS		0x80		/* no digits detected */

#define	DPTOK		0x100		/* (float) decimal point is still legal */
#define	EXPOK		0x200		/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		0x100		/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200		/* no zero digits detected */

/*
 * Conversion types.
 */
#define	CT_CHAR		0			/* %c conversion */
#define	CT_CCL		1			/* %[...] conversion */
#define	CT_STRING	2			/* %s conversion */
#define	CT_INT		3			/* integer, i.e., strtoq or strtouq */
#define	CT_FLOAT	4			/* floating, i.e., strtod */

#define u_char unsigned char
#define u_long unsigned long

static u_char *ib__sccl(char *, u_char *);

/*
 * vfscanf
 */
int ib__svfscanf(IB_FILE * fp, char const *fmt0, va_list ap)
{
	u_char *fmt = (u_char *) fmt0;
	int c;				/* character from format, or conversion */
	size_t width;		/* field width, or 0 */
	char *p;			/* points into all kinds of strings */
	int n;				/* handy integer */
	int flags;			/* flags as defined above */
	char *p0;			/* saves original value of p when necessary */
	int nassigned;				/* number of fields assigned */
	int nconversions;			/* number of conversions */
	int nread;					/* number of characters consumed from fp */
	int base;					/* base argument to strtoq/strtouq */
	u_quad_t(*ccfn) ();			/* conversion function (strtoq/strtouq) */
	char ccltab[256];			/* character class table for %[...] */
	char buf[BUF];				/* buffer for numeric conversions */

	/* `basefix' is used to avoid `if' tests in the integer scanner */
	static short basefix[17] =
		{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

	nassigned = 0;
	nconversions = 0;
	nread = 0;
	base = 0;					/* XXX just to keep gcc happy */
	ccfn = NULL;				/* XXX just to keep gcc happy */
	for (;;) {
		c = *fmt++;
		if (c == 0)
			return (nassigned);
		if (isspace(c)) {
			while ((fp->_r > 0 || ib__srefill(fp) == 0) && isspace(*fp->_p))
				nread++, fp->_r--, fp->_p++;
			continue;
		}
		if (c != '%')
			goto literal;
		width = 0;
		flags = 0;
		/*
		 * switch on the format.  continue if done;
		 * break once format type is derived.
		 */
	  again:c = *fmt++;
		switch (c) {
		case '%':
		  literal:
			if (fp->_r <= 0 && ib__srefill(fp))
				goto input_failure;
			if (*fp->_p != c)
				goto match_failure;
			fp->_r--, fp->_p++;
			nread++;
			continue;

		case '*':
			flags |= SUPPRESS;
			goto again;
		case 'l':
			flags |= LONG;
			goto again;
		case 'q':
			flags |= QUAD;
			goto again;
		case 'L':
			flags |= LONGDBL;
			goto again;
		case 'h':
			flags |= SHORT;
			goto again;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			width = width * 10 + c - '0';
			goto again;

			/*
			 * Conversions.
			 * Those marked `compat' are for 4.[123]BSD compatibility.
			 *
			 * (According to ANSI, E and X formats are supposed
			 * to the same as e and x.  Sorry about that.)
			 */
		case 'D':				/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'd':
			c = CT_INT;
			ccfn = (u_quad_t(*)())strtoq;
			base = 10;
			break;

		case 'i':
			c = CT_INT;
			ccfn = (u_quad_t(*)())strtoq;
			base = 0;
			break;

		case 'O':				/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'o':
			c = CT_INT;
			ccfn = strtouq;
			base = 8;
			break;

		case 'u':
			c = CT_INT;
			ccfn = strtouq;
			base = 10;
			break;

		case 'X':				/* compat   XXX */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'x':
			flags |= PFXOK;		/* enable 0x prefixing */
			c = CT_INT;
			ccfn = strtouq;
			base = 16;
			break;

		case 'E':				/* compat   XXX */
		case 'F':				/* compat */
			flags |= LONG;
			/* FALLTHROUGH */
		case 'e':
		case 'f':
		case 'g':
			c = CT_FLOAT;
			break;

		case 's':
			c = CT_STRING;
			break;

		case '[':
			fmt = ib__sccl(ccltab, fmt);
			flags |= NOSKIP;
			c = CT_CCL;
			break;

		case 'c':
			flags |= NOSKIP;
			c = CT_CHAR;
			break;

		case 'p':				/* pointer format is like hex */
			flags |= POINTER | PFXOK;
			c = CT_INT;
			ccfn = strtouq;
			base = 16;
			break;

		case 'n':
			nconversions++;
			if (flags & SUPPRESS)	/* ??? */
				continue;
			if (flags & SHORT)
				*va_arg(ap, short *) = nread;
			else if (flags & LONG)
				*va_arg(ap, long *) = nread;
			else if (flags & QUAD)
				*va_arg(ap, quad_t *) = nread;
			else
				*va_arg(ap, int *) = nread;
			continue;

			/*
			 * Disgusting backwards compatibility hacks.    XXX
			 */
		case '\0':				/* compat */
			return (EOF);

		default:				/* compat */
			if (isupper(c))
				flags |= LONG;
			c = CT_INT;
			ccfn = (u_quad_t(*)())strtoq;
			base = 10;
			break;
		}

		/*
		 * We have a conversion that requires input.
		 */
		if (fp->_r <= 0 && ib__srefill(fp))
			goto input_failure;

		/*
		 * Consume leading white space, except for formats
		 * that suppress this.
		 */
		if ((flags & NOSKIP) == 0) {
			while (isspace(*fp->_p)) {
				nread++;
				if (--fp->_r > 0)
					fp->_p++;
				else if (ib__srefill(fp))
					goto input_failure;
			}
			/*
			 * Note that there is at least one character in
			 * the buffer, so conversions that do not set NOSKIP
			 * ca no longer result in an input failure.
			 */
		}

		/*
		 * Do the conversion.
		 */
		switch (c) {

		case CT_CHAR:
			/* scan arbitrary characters (sets NOSKIP) */
			if (width == 0)
				width = 1;
			if (flags & SUPPRESS) {
				size_t sum = 0;
				for (;;) {
					if ((n = fp->_r) < width) {
						sum += n;
						width -= n;
						fp->_p += n;
						if (ib__srefill(fp)) {
							if (sum == 0)
								goto input_failure;
							break;
						}
					}
					else {
						sum += width;
						fp->_r -= width;
						fp->_p += width;
						break;
					}
				}
				nread += sum;
			}
			else {
				size_t r = ib_fread((void *) va_arg(ap, char *), 1,
									width, fp);

				if (r == 0)
					goto input_failure;
				nread += r;
				nassigned++;
			}
			nconversions++;
			break;

		case CT_CCL:
			/* scan a (nonempty) character class (sets NOSKIP) */
			if (width == 0)
				width = (size_t) ~ 0;	/* `infinity' */
			/* take only those things in the class */
			if (flags & SUPPRESS) {
				n = 0;
				while (ccltab[*fp->_p]) {
					n++, fp->_r--, fp->_p++;
					if (--width == 0)
						break;
					if (fp->_r <= 0 && ib__srefill(fp)) {
						if (n == 0)
							goto input_failure;
						break;
					}
				}
				if (n == 0)
					goto match_failure;
			}
			else {
				p0 = p = va_arg(ap, char *);
				while (ccltab[*fp->_p]) {
					fp->_r--;
					*p++ = *fp->_p++;
					if (--width == 0)
						break;
					if (fp->_r <= 0 && ib__srefill(fp)) {
						if (p == p0)
							goto input_failure;
						break;
					}
				}
				n = p - p0;
				if (n == 0)
					goto match_failure;
				*p = 0;
				nassigned++;
			}
			nread += n;
			nconversions++;
			break;

		case CT_STRING:
			/* like CCL, but zero-length string OK, & no NOSKIP */
			if (width == 0)
				width = (size_t) ~ 0;
			if (flags & SUPPRESS) {
				n = 0;
				while (!isspace(*fp->_p)) {
					n++, fp->_r--, fp->_p++;
					if (--width == 0)
						break;
					if (fp->_r <= 0 && ib__srefill(fp))
						break;
				}
				nread += n;
			}
			else {
				p0 = p = va_arg(ap, char *);
				while (!isspace(*fp->_p)) {
					fp->_r--;
					*p++ = *fp->_p++;
					if (--width == 0)
						break;
					if (fp->_r <= 0 && ib__srefill(fp))
						break;
				}
				*p = 0;
				nread += p - p0;
				nassigned++;
			}
			nconversions++;
			continue;

		case CT_INT:
			/* scan an integer as if by strtoq/strtouq */
			/* size_t is unsigned, hence this optimisation */
			if (--width > sizeof(buf) - 2)
				width = sizeof(buf) - 2;
			width++;
			flags |= SIGNOK | NDIGITS | NZDIGITS;
			for (p = buf; width; width--) {
				c = *fp->_p;
				/*
				 * Switch on the character; `goto ok'
				 * if we accept it as a part of number.
				 */
				switch (c) {

					/*
					 * The digit 0 is always legal, but is
					 * special.  For %i conversions, if no
					 * digits (zero or nonzero) have been
					 * scanned (only signs), we will have
					 * base==0.  In that case, we should set
					 * it to 8 and enable 0x prefixing.
					 * Also, if we have not scanned zero digits
					 * before this, do not turn off prefixing
					 * (someone else will turn it off if we
					 * have scanned any nonzero digits).
					 */
				case '0':
					if (base == 0) {
						base = 8;
						flags |= PFXOK;
					}
					if (flags & NZDIGITS)
						flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
					else
						flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

					/* 1 through 7 always legal */
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					base = basefix[base];
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

					/* digits 8 and 9 ok iff decimal or hex */
				case '8':
				case '9':
					base = basefix[base];
					if (base <= 8)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

					/* letters ok iff hex */
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
					/* no need to fix base here */
					if (base <= 10)
						break;	/* not legal here */
					flags &= ~(SIGNOK | PFXOK | NDIGITS);
					goto ok;

					/* sign ok only as first character */
				case '+':
				case '-':
					if (flags & SIGNOK) {
						flags &= ~SIGNOK;
						goto ok;
					}
					break;

					/* x ok iff flag still set & 2nd char */
				case 'x':
				case 'X':
					if (flags & PFXOK && p == buf + 1) {
						base = 16;	/* if %i */
						flags &= ~PFXOK;
						goto ok;
					}
					break;
				}

				/*
				 * If we got here, c is not a legal character
				 * for a number.  Stop accumulating digits.
				 */
				break;
			  ok:
				/*
				 * c is legal: store it and look at the next.
				 */
				*p++ = c;
				if (--fp->_r > 0)
					fp->_p++;
				else if (ib__srefill(fp))
					break;		/* EOF */
			}
			/*
			 * If we had only a sign, it is no good; push
			 * back the sign.  If the number ends in `x',
			 * it was [sign] '0' 'x', so push back the x
			 * and treat it as [sign] '0'.
			 */
			if (flags & NDIGITS) {
				if (p > buf)
					(void) ib_ungetc(*(u_char *)-- p, fp);
				goto match_failure;
			}
			c = ((u_char *) p)[-1];
			if (c == 'x' || c == 'X') {
				--p;
				(void) ib_ungetc(c, fp);
			}
			if ((flags & SUPPRESS) == 0) {
				u_quad_t res;

				*p = 0;
				res = (*ccfn) (buf, (char **) NULL, base);
				if (flags & POINTER)
					*va_arg(ap, void **) = (void *) (u_long) res;
				else if (flags & SHORT)
					*va_arg(ap, short *) = res;
				else if (flags & LONG)
					*va_arg(ap, long *) = res;
				else if (flags & QUAD)
					*va_arg(ap, quad_t *) = res;
				else
					*va_arg(ap, int *) = res;
				nassigned++;
			}
			nread += p - buf;
			nconversions++;
			break;

		case CT_FLOAT:
			/* scan a floating point number as if by strtod */
			/* size_t is unsigned, hence this optimisation */
			if (--width > sizeof(buf) - 2)
				width = sizeof(buf) - 2;
			width++;
			flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
			for (p = buf; width; width--) {
				c = *fp->_p;
				/*
				 * This code mimicks the integer conversion
				 * code, but is much simpler.
				 */
				switch (c) {

				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					flags &= ~(SIGNOK | NDIGITS);
					goto fok;

				case '+':
				case '-':
					if (flags & SIGNOK) {
						flags &= ~SIGNOK;
						goto fok;
					}
					break;
				case '.':
					if (flags & DPTOK) {
						flags &= ~(SIGNOK | DPTOK);
						goto fok;
					}
					break;
				case 'e':
				case 'E':
					/* no exponent without some digits */
					if ((flags & (NDIGITS | EXPOK)) == EXPOK) {
						flags = (flags & ~(EXPOK | DPTOK)) | SIGNOK | NDIGITS;
						goto fok;
					}
					break;
				}
				break;
			  fok:
				*p++ = c;
				if (--fp->_r > 0)
					fp->_p++;
				else if (ib__srefill(fp))
					break;		/* EOF */
			}
			/*
			 * If no digits, might be missing exponent digits
			 * (just give back the exponent) or might be missing
			 * regular digits, but had sign and/or decimal point.
			 */
			if (flags & NDIGITS) {
				if (flags & EXPOK) {
					/* no digits at all */
					while (p > buf)
						ib_ungetc(*(u_char *)-- p, fp);
					goto match_failure;
				}
				/* just a bad exponent (e and maybe sign) */
				c = *(u_char *)-- p;
				if (c != 'e' && c != 'E') {
					(void) ib_ungetc(c, fp);	/* sign */
					c = *(u_char *)-- p;
				}
				(void) ib_ungetc(c, fp);
			}
			if ((flags & SUPPRESS) == 0) {
				double res;

				*p = 0;
				/* XXX this loses precision for long doubles. */
				res = strtod(buf, (char **) NULL);
				if (flags & LONGDBL)
					*va_arg(ap, long double *) = res;
				else if (flags & LONG)
					*va_arg(ap, double *) = res;
				else
					*va_arg(ap, float *) = res;
				nassigned++;
			}
			nread += p - buf;
			nconversions++;
			break;
		}
	}
  input_failure:
	return (nconversions != 0 ? nassigned : EOF);
  match_failure:
	return (nassigned);
}

/*
 * Fill in the given table from the scanset at the given format
 * (just after `[').  Return a pointer to the character past the
 * closing `]'.  The table has a 1 wherever characters should be
 * considered part of the scanset.
 */
static u_char *ib__sccl(char *tab, u_char * fmt)
{
	int c, n, v, i;

	/* first `clear' the whole table */
	c = *fmt++;					/* first char hat => negated scanset */
	if (c == '^') {
		v = 1;					/* default => accept */
		c = *fmt++;				/* get new first char */
	}
	else
		v = 0;					/* default => reject */

	/* XXX: Will not work if sizeof(tab*) > sizeof(char) */
	(void) memset(tab, v, 256);

	if (c == 0)
		return (fmt - 1);		/* format ended before closing ] */

	/*
	 * Now set the entries corresponding to the actual scanset
	 * to the opposite of the above.
	 *
	 * The first character may be ']' (or '-') without being special;
	 * the last character may be '-'.
	 */
	v = 1 - v;
	for (;;) {
		tab[c] = v;				/* take character c */
	  doswitch:
		n = *fmt++;				/* and examine the next */
		switch (n) {

		case 0:				/* format ended too soon */
			return (fmt - 1);

		case '-':
			/*
			 * A scanset of the form
			 *  [01+-]
			 * is defined as `the digit 0, the digit 1,
			 * the character +, the character -', but
			 * the effect of a scanset such as
			 *  [a-zA-Z0-9]
			 * is implementation defined.  The V7 Unix
			 * scanf treats `a-z' as `the letters a through
			 * z', but treats `a-a' as `the letter a, the
			 * character -, and the letter a'.
			 *
			 * For compatibility, the `-' is not considerd
			 * to define a range if the character following
			 * it is either a close bracket (required by ANSI)
			 * or is not numerically greater than the character
			 * we just stored in the table (c).
			 */
			n = *fmt;
			if (n == ']'
				|| (ib__collate_load_error ? n < c :
					ib__collate_range_cmp(n, c) < 0)
				) {
				c = '-';
				break;			/* resume the for(;;) */
			}
			fmt++;
			/* fill in the range */
			if (ib__collate_load_error) {
				do {
					tab[++c] = v;
				} while (c < n);
			}
			else {
				for (i = 0; i < 256; i++)
					if (ib__collate_range_cmp(c, i) < 0
						&& ib__collate_range_cmp(i, n) <= 0)
						tab[i] = v;
			}
#ifdef NOT_USED_OR_REPLACED							/* XXX another disgusting compatibility hack */
			c = *fmt++;
			if (c == 0)
				return (fmt - 1);
			if (c == ']')
				return (fmt);
#else
			c = n;
			/*
			 * Alas, the V7 Unix scanf also treats formats
			 * such as [a-c-e] as `the letters a through e'.
			 * This too is permitted by the standard....
			 */
			goto doswitch;
#endif
			break;

		case ']':				/* end of scanset */
			return (fmt);

		default:				/* just another character */
			c = n;
			break;
		}
	}
	/* NOTREACHED */
}
int ib_vprintf(char const *fmt, va_list ap)
{
	return (ib_vfprintf(ib_stdout, fmt, ap));
}
int ib_vscanf(const char *fmt, va_list ap)
{
	int retval;

	FLOCKFILE(stdin);
	retval = ib__svfscanf(ib_stdin, fmt, ap);
	FUNLOCKFILE(ib_stdin);
	return (retval);
}


/*
 * Write the given character into the (probably full) buffer for
 * the given file.  Flush the buffer out if it is or becomes full,
 * or if c=='\n' and the file is line buffered.
 */
int ib__swbuf(int c, IB_FILE * fp)
{
	int n;

	/*
	 * In case we cannot write, or an exception takes us out early,
	 * make sure _w is 0 (if fully- or un-buffered) or -_bf._size
	 * (if line buffered) so that we will get called again.
	 * If we did not do this, a sufficient number of ib_putc()
	 * calls might wrap _w from negative to positive.
	 */
	fp->_w = fp->_lbfsize;
	if (cantwrite(fp))
		return (EOF);
	c = (unsigned char) c;

	/*
	 * If it is completely full, flush it out.  Then, in any case,
	 * stuff c into the buffer.  If this causes the buffer to fill
	 * completely, or if c is '\n' and the file is line buffered,
	 * flush it (perhaps a second time).  The second flush will always
	 * happen on unbuffered streams, where _bf._size==1; fflush()
	 * guarantees that ib_putc() will always call wbuf() by setting _w
	 * to 0, so we need not do anything else.
	 */
	n = fp->_p - fp->_bf._base;
	if (n >= fp->_bf._size) {
		if (ib_fflush(fp))
			return (EOF);
		n = 0;
	}
	fp->_w--;
	*fp->_p++ = c;
	if (++n == fp->_bf._size || (fp->_flags & IB__SLBF && c == '\n'))
		if (ib_fflush(fp))
			return (EOF);
	return (c);
}


/*
 * Various output routines call wsetup to be sure it is safe to write,
 * because either _flags does not include IB__SWR, or _buf is NULL.
 * _wsetup returns 0 if OK to write, nonzero otherwise.
 */
int ib__swsetup(IB_FILE * fp)
{
	/* make sure stdio is set up */
	if (!ib__sdidinit)
		ib__sinit();

	/*
	 * If we are not writing, we had better be reading and writing.
	 */
	if ((fp->_flags & IB__SWR) == 0) {
		if ((fp->_flags & IB__SRW) == 0)
			return (EOF);
		if (fp->_flags & IB__SRD) {
			/* clobber any ib_ungetc data */
			if (HASUB(fp))
				FREEUB(fp);
			fp->_flags &= ~(IB__SRD | IB__SEOF);
			fp->_r = 0;
			fp->_p = fp->_bf._base;
		}
		fp->_flags |= IB__SWR;
	}

	/*
	 * Make a buffer if necessary, then set _w.
	 */
	if (fp->_bf._base == NULL)
		ib__smakebuf(fp);
	if (fp->_flags & IB__SLBF) {
		/*
		 * It is line buffered, so make _lbfsize be -_bufsize
		 * for the putc() macro.  We will change _lbfsize back
		 * to 0 whenever we turn off IB__SWR.
		 */
		fp->_w = 0;
		fp->_lbfsize = -fp->_bf._size;
	}
	else
		fp->_w = fp->_flags & IB__SNBF ? 0 : fp->_bf._size;
	return (0);
}


/*
 * Write some memory regions.  Return zero on success, EOF on error.
 *
 * This routine is large and unsightly, but most of the ugliness due
 * to the three different kinds of output buffering is handled here.
 */
int ib__sfvwrite(IB_FILE * fp, struct ib__suio *uio)
{
	size_t len;
	char *p;
	struct ib__siov *iov;
	int w, s;
	char *nl;
	int nlknown, nldist;

	if ((len = uio->uio_resid) == 0)
		return (0);
	/* make sure we can write */
	if (cantwrite(fp))
		return (EOF);

#define	MIN(a, b) ((a) < (b) ? (a) : (b))
#define	COPY(n)	  (void)memcpy((void *)fp->_p, (void *)p, (size_t)(n))

	iov = uio->uio_iov;
	p = iov->iov_base;
	len = iov->iov_len;
	iov++;
#define GETIOV(extra_work) \
	while (len == 0) { \
		extra_work; \
		p = iov->iov_base; \
		len = iov->iov_len; \
		iov++; \
	}
	if (fp->_flags & IB__SNBF) {
		/*
		 * Unbuffered: write up to BUFSIZ bytes at a time.
		 */
		do {
			GETIOV(;);
			w = (*fp->_write) (fp->_cookie, p, MIN(len, BUFSIZ));
			if (w <= 0)
				goto err;
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	}
	else if ((fp->_flags & IB__SLBF) == 0) {
		/*
		 * Fully buffered: fill partially full buffer, if any,
		 * and then flush.  If there is no partial buffer, write
		 * one _bf._size byte chunk directly (without copying).
		 *
		 * String output is a special case: write as many bytes
		 * as fit, but pretend we wrote everything.  This makes
		 * snprintf() return the number of bytes needed, rather
		 * than the number used, and avoids its write function
		 * (so that the write function can be invalid).
		 */
		do {
			GETIOV(;);
			if ((fp->_flags & (IB__SALC | IB__SSTR)) ==
				(IB__SALC | IB__SSTR) && fp->_w < len) {
				size_t blen = fp->_p - fp->_bf._base;

				/*
				 * Alloc an extra 128 bytes (+ 1 for NULL)
				 * so we don't call realloc(3) so often.
				 */
				fp->_w = len + 128;
				fp->_bf._size = blen + len + 128;
				fp->_bf._base = reallocf(fp->_bf._base, fp->_bf._size + 1);
				if (fp->_bf._base == NULL)
					goto err;
				fp->_p = fp->_bf._base + blen;
			}
			w = fp->_w;
			if (fp->_flags & IB__SSTR) {
				if (len < w)
					w = len;
				if (w > 0) {
					COPY(w);	/* copy MIN(fp->_w,len), */
					fp->_w -= w;
					fp->_p += w;
				}
				w = len;		/* but pretend copied all */
			}
			else if (fp->_p > fp->_bf._base && len > w) {
				/* fill and flush */
				COPY(w);
/* fp->_w -= w; *//* unneeded */
				fp->_p += w;
				if (ib_fflush(fp))
					goto err;
			}
			else if (len >= (w = fp->_bf._size)) {
				/* write directly */
				w = (*fp->_write) (fp->_cookie, p, w);
				if (w <= 0)
					goto err;
			}
			else {
				/* fill and done */
				w = len;
				COPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	}
	else {
		/*
		 * Line buffered: like fully buffered, but we
		 * must check for newlines.  Compute the distance
		 * to the first newline (including the newline),
		 * or `infinity' if there is none, then pretend
		 * that the amount to write is MIN(len,nldist).
		 */
		nlknown = 0;
		nldist = 0;				/* XXX just to keep gcc happy */
		do {
			GETIOV(nlknown = 0);
			if (!nlknown) {
				nl = memchr((void *) p, '\n', len);
				nldist = nl ? nl + 1 - p : len + 1;
				nlknown = 1;
			}
			s = MIN(len, nldist);
			w = fp->_w + fp->_bf._size;
			if (fp->_p > fp->_bf._base && s > w) {
				COPY(w);
				/* fp->_w -= w; */
				fp->_p += w;
				if (ib_fflush(fp))
					goto err;
			}
			else if (s >= (w = fp->_bf._size)) {
				w = (*fp->_write) (fp->_cookie, p, w);
				if (w <= 0)
					goto err;
			}
			else {
				w = s;
				COPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			if ((nldist -= w) == 0) {
				/* copied the newline: flush and forget */
				if (ib_fflush(fp))
					goto err;
				nlknown = 0;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	}
	return (0);

  err:
	fp->_flags |= IB__SERR;
	return (EOF);
}

/* This stuff all comes from stdlib/netbsd_strtod.c */


#ifdef MALLOC
#ifdef KR_headers
extern char *MALLOC();
#else
extern void *MALLOC(size_t);
#endif
#else
#define MALLOC malloc
#endif

#ifdef SOLARIS
#ifdef sparc
#define IEEE_BIG_ENDIAN
#undef WORDS_BIGENDIAN /* EKU: why do we undefine WORDS_BIGENDIAN here ??? */
#else
#define IEEE_LITTLE_ENDIAN
#endif
#define IEEE_ARITHMETIC
#endif


#ifdef IEEE_ARITHMETIC
#define DBL_DIG 15
#define DBL_MAX_10_EXP 308
#define DBL_MAX_EXP 1024
#define FLT_RADIX 2
#define FLT_ROUNDS 1
#define DBL_MAX 1.7976931348623157e+308
#endif


#ifdef IEEE_LITTLE_ENDIAN
#define word0(x) ((ULONG *)&x)[1]
#define word1(x) ((ULONG *)&x)[0]
#else
#define word0(x) ((ULONG *)&x)[0]
#define word1(x) ((ULONG *)&x)[1]
#endif

#if defined(IEEE_LITTLE_ENDIAN) + defined(IEEE_BIG_ENDIAN) + !defined(WORDS_BIGENDIAN) + defined(IBM) != 1
Exactly one of IEEE_LITTLE_ENDIAN IEEE_BIG_ENDIAN, WORDS_BIGENDIAN, or IBM should be defined.
#endif
#if defined(IEEE_LITTLE_ENDIAN) + defined(IEEE_BIG_ENDIAN)
#define Exp_shift  20
#define Exp_shift1 20
#define Exp_msk1    0x100000
#define Exp_msk11   0x100000
#define Exp_mask  0x7ff00000
#define P 53
#define Bias 1023
#define IEEE_Arith
#define Emin (-1022)
#define Exp_1  0x3ff00000
#define Exp_11 0x3ff00000
#define Ebits 11
#define Frac_mask  0xfffff
#define Frac_mask1 0xfffff
#define Ten_pmax 22
#define Bletch 0x10
#define Bndry_mask  0xfffff
#define Bndry_mask1 0xfffff
#define LSB 1
#define Sign_bit 0x80000000
#define Log2P 1
#define Tiny0 0
#define Tiny1 1
#define Quick_max 14
#define Int_max 14
#define Infinite(x) (word0(x) == 0x7ff00000)	/* sufficient test for here */
#else
#undef  Sudden_Underflow
#define Sudden_Underflow
#ifdef IBM
#define Exp_shift  24
#define Exp_shift1 24
#define Exp_msk1   0x1000000
#define Exp_msk11  0x1000000
#define Exp_mask  0x7f000000
#define P 14
#define Bias 65
#define Exp_1  0x41000000
#define Exp_11 0x41000000
#define Ebits 8					/* exponent has 7 bits, but 8 is the right value in b2d */
#define Frac_mask  0xffffff
#define Frac_mask1 0xffffff
#define Bletch 4
#define Ten_pmax 22
#define Bndry_mask  0xefffff
#define Bndry_mask1 0xffffff
#define LSB 1
#define Sign_bit 0x80000000
#define Log2P 4
#define Tiny0 0x100000
#define Tiny1 0
#define Quick_max 14
#define Int_max 15
#else							/* VAX */
#define Exp_shift  23
#define Exp_shift1 7
#define Exp_msk1    0x80
#define Exp_msk11   0x800000
#define Exp_mask  0x7f80
#define P 56
#define Bias 129
#define Exp_1  0x40800000
#define Exp_11 0x4080
#define Ebits 8
#define Frac_mask  0x7fffff
#define Frac_mask1 0xffff007f
#define Ten_pmax 24
#define Bletch 2
#define Bndry_mask  0xffff007f
#define Bndry_mask1 0xffff007f
#define LSB 0x10000
#define Sign_bit 0x8000
#define Log2P 1
#define Tiny0 0x80
#define Tiny1 0
#define Quick_max 15
#define Int_max 15
#endif
#endif
#ifndef IEEE_Arith
#define ROUND_BIASED
#endif
#define Sign_Extend(a,b)		/*no-op */
	struct
	Bigint {
	struct Bigint *next;
	int k, maxwds, sign, wds;
	ULONG x[1];
};

typedef struct Bigint Bigint;

static const double tens[] = {
	1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9,
	1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19,
	1e20, 1e21, 1e22
#ifndef WORDS_BIGENDIAN
		, 1e23, 1e24
#endif
};

#ifdef IEEE_Arith
static const double bigtens[] = { 1e16, 1e32, 1e64, 1e128, 1e256 };
static const double tinytens[] = { 1e-16, 1e-32, 1e-64, 1e-128, 1e-256 };
#define n_bigtens 5
#else
#ifdef IBM
static const double bigtens[] = { 1e16, 1e32, 1e64 };
static const double tinytens[] = { 1e-16, 1e-32, 1e-64 };
#define n_bigtens 3
#else
static const double bigtens[] = { 1e16, 1e32 };
static const double tinytens[] = { 1e-16, 1e-32 };
#define n_bigtens 2
#endif
#endif

static Bigint *Balloc(int);
static void Bfree(Bigint *);
static Bigint *multadd(Bigint *, int, int);
static Bigint *mult(Bigint *, Bigint *);
static Bigint *i2b(int);

static Bigint *p5s;

static Bigint *pow5mult(Bigint * b, int k)
{
	Bigint *b1, *p5, *p51;
	int i;
	static int p05[3] = { 5, 25, 125 };

	if ((i = k & 3) != 0)
		b = multadd(b, p05[i - 1], 0);

	if (!(k >>= 2))
		return b;
	if (!(p5 = p5s)) {
		/* first time */
		p5 = p5s = i2b(625);
		p5->next = 0;
	}
	for (;;) {
		if (k & 1) {
			b1 = mult(b, p5);
			Bfree(b);
			b = b1;
		}
		if (!(k >>= 1))
			break;
		if (!(p51 = p5->next)) {
			p51 = p5->next = mult(p5, p5);
			p51->next = 0;
		}
		p5 = p51;
	}
	return b;
}


static Bigint *lshift(Bigint * b, int k)
{
	int i, k1, n, n1;
	Bigint *b1;
	ULONG *x, *x1, *xe, z;

	n = k >> 4;
	k1 = b->k;
	n1 = n + b->wds + 1;
	for (i = b->maxwds; n1 > i; i <<= 1)
		k1++;
	b1 = Balloc(k1);
	x1 = b1->x;
	for (i = 0; i < n; i++)
		*x1++ = 0;
	x = b->x;
	xe = x + b->wds;
	if (k &= 0xf) {
		k1 = 16 - k;
		z = 0;
		do {
			*x1++ = *x << k & 0xffff | z;
			z = *x++ >> k1;
		}
		while (x < xe);
		if (*x1 = z)
			++n1;
	}
	else
		do
			*x1++ = *x++;
		while (x < xe);
	b1->wds = n1 - 1;
	Bfree(b);
	return b1;
}

static int cmp(Bigint * a, Bigint * b)
{
	ULONG *xa, *xa0, *xb, *xb0;
	int i, j;

	i = a->wds;
	j = b->wds;
#ifdef DEBUG
	if (i > 1 && !a->x[i - 1])
		Bug("cmp called with a->x[a->wds-1] == 0");
	if (j > 1 && !b->x[j - 1])
		Bug("cmp called with b->x[b->wds-1] == 0");
#endif
	if (i -= j)
		return i;
	xa0 = a->x;
	xa = xa0 + j;
	xb0 = b->x;
	xb = xb0 + j;
	for (;;) {
		if (*--xa != *--xb)
			return *xa < *xb ? -1 : 1;
		if (xa <= xa0)
			break;
	}
	return 0;
}

static Bigint *diff(Bigint * a, Bigint * b)
{
	Bigint *c;
	int i, wa, wb;
	SLONG borrow, y;			/* We need signed shifts here. */
	ULONG *xa, *xae, *xb, *xbe, *xc;

	i = cmp(a, b);
	if (!i) {
		c = Balloc(0);
		c->wds = 1;
		c->x[0] = 0;
		return c;
	}
	if (i < 0) {
		c = a;
		a = b;
		b = c;
		i = 1;
	}
	else
		i = 0;
	c = Balloc(a->k);
	c->sign = i;
	wa = a->wds;
	xa = a->x;
	xae = xa + wa;
	wb = b->wds;
	xb = b->x;
	xbe = xb + wb;
	xc = c->x;
	borrow = 0;
	do {
		y = *xa++ - *xb++ + borrow;
		borrow = y >> 16;
		Sign_Extend(borrow, y);
		*xc++ = y & 0xffff;
	}
	while (xb < xbe);
	while (xa < xae) {
		y = *xa++ + borrow;
		borrow = y >> 16;
		Sign_Extend(borrow, y);
		*xc++ = y & 0xffff;
	}
	while (!*--xc)
		wa--;
	c->wds = wa;
	return c;
}

static Bigint *d2b(double d, int *e, int *bits)
{
	Bigint *b;
	int de, i, k;
	ULONG *x, y, z;
#ifndef WORDS_BIGENDIAN
	ULONG d0, d1;
	d0 = word0(d) >> 16 | word0(d) << 16;
	d1 = word1(d) >> 16 | word1(d) << 16;
#else
#define d0 word0(d)
#define d1 word1(d)
#endif

	b = Balloc(2);
	x = b->x;

	z = d0 & Frac_mask;
	d0 &= 0x7fffffff;			/* clear sign bit, which we ignore */
#ifdef Sudden_Underflow
	de = (int) (d0 >> Exp_shift);
#ifndef IBM
	z |= Exp_msk11;
#endif
#else
	if ((de = (int) (d0 >> Exp_shift)) != 0)
		z |= Exp_msk1;
#endif
	if (y = d1) {
		if (k = lo0bits(&y))
			if (k >= 16) {
				x[0] = y | z << 32 - k & 0xffff;
				x[1] = z >> k - 16 & 0xffff;
				x[2] = z >> k;
				i = 2;
			}
			else {
				x[0] = y & 0xffff;
				x[1] = y >> 16 | z << 16 - k & 0xffff;
				x[2] = z >> k & 0xffff;
				x[3] = z >> k + 16;
				i = 3;
			}
		else {
			x[0] = y & 0xffff;
			x[1] = y >> 16;
			x[2] = z & 0xffff;
			x[3] = z >> 16;
			i = 3;
		}
	}
	else {
#ifdef DEBUG
		if (!z)
			Bug("Zero passed to d2b");
#endif
		k = lo0bits(&z);
		if (k >= 16) {
			x[0] = z;
			i = 0;
		}
		else {
			x[0] = z & 0xffff;
			x[1] = z >> 16;
			i = 1;
		}
		k += 32;
	}
	while (!x[i])
		--i;
	b->wds = i + 1;
#ifndef Sudden_Underflow
	if (de) {
#endif
#ifdef IBM
		*e = (de - Bias - (P - 1) << 2) + k;
		*bits = 4 * P + 8 - k - hi0bits(word0(d) & Frac_mask);
#else
		*e = de - Bias - (P - 1) + k;
		*bits = P - k;
#endif
#ifndef Sudden_Underflow
	}
	else {
		*e = de - Bias - (P - 1) + 1 + k;
		*bits = (i + 2) * 16 - hi0bits(x[i]);
	}
#endif
	return b;
}

#undef d0
#undef d1

#define Kmax 15
static Bigint *freelist[Kmax + 1];

static Bigint *Balloc(int k)
{
	int x;
	Bigint *rv;

	if ((rv = freelist[k]) != NULL) {
		freelist[k] = rv->next;
	}
	else {
		x = 1 << k;
		rv = (Bigint *) MALLOC(sizeof(Bigint) + (x - 1) * sizeof(SLONG));
		rv->k = k;
		rv->maxwds = x;
	}
	rv->sign = rv->wds = 0;
	return rv;
}

static void Bfree(Bigint * v)
{
	if (v) {
		v->next = freelist[v->k];
		freelist[v->k] = v;
	}
}

#define Bcopy(x,y) memcpy((char *)&x->sign, (char *)&y->sign, \
y->wds*sizeof(SLONG) + 2*sizeof(int))

static Bigint *multadd(Bigint * b, int m, int a)
	/* multiply by m and add a */
{
	int i, wds;
	ULONG *x, y;
	Bigint *b1;

	wds = b->wds;
	x = b->x;
	i = 0;
	do {
		y = *x * m + a;
		a = (int) (y >> 16);
		*x++ = y & 0xffff;
	}
	while (++i < wds);
	if (a) {
		if (wds >= b->maxwds) {
			b1 = Balloc(b->k + 1);
			Bcopy(b1, b);
			Bfree(b);
			b = b1;
		}
		b->x[wds++] = a;
		b->wds = wds;
	}
	return b;
}



static int hi0bits(ULONG x)
{
	int k = 0;

	if (!(x & 0xffff0000)) {
		k = 16;
		x <<= 16;
	}
	if (!(x & 0xff000000)) {
		k += 8;
		x <<= 8;
	}
	if (!(x & 0xf0000000)) {
		k += 4;
		x <<= 4;
	}
	if (!(x & 0xc0000000)) {
		k += 2;
		x <<= 2;
	}
	if (!(x & 0x80000000)) {
		k++;
		if (!(x & 0x40000000))
			return 32;
	}
	return k;
}

static int lo0bits(ULONG * y)
{
	int k;
	ULONG x = *y;

	if (x & 7) {
		if (x & 1)
			return 0;
		if (x & 2) {
			*y = x >> 1;
			return 1;
		}
		*y = x >> 2;
		return 2;
	}
	k = 0;
	if (!(x & 0xffff)) {
		k = 16;
		x >>= 16;
	}
	if (!(x & 0xff)) {
		k += 8;
		x >>= 8;
	}
	if (!(x & 0xf)) {
		k += 4;
		x >>= 4;
	}
	if (!(x & 0x3)) {
		k += 2;
		x >>= 2;
	}
	if (!(x & 1)) {
		k++;
		x >>= 1;
		if (!x & 1)
			return 32;
	}
	*y = x;
	return k;
}

static Bigint *i2b(int i)
{
	Bigint *b;

	b = Balloc(1);
	b->x[0] = i;
	b->wds = 1;
	return b;
}

static Bigint *mult(Bigint * a, Bigint * b)
{
	Bigint *c;
	int k, wa, wb, wc;
	ULONG carry, y, z;
	ULONG *x, *xa, *xae, *xb, *xbe, *xc, *xc0;

	if (a->wds < b->wds) {
		c = a;
		a = b;
		b = c;
	}
	k = a->k;
	wa = a->wds;
	wb = b->wds;
	wc = wa + wb;
	if (wc > a->maxwds)
		k++;
	c = Balloc(k);
	for (x = c->x, xa = x + wc; x < xa; x++)
		*x = 0;
	xa = a->x;
	xae = xa + wa;
	xb = b->x;
	xbe = xb + wb;
	xc0 = c->x;
	for (; xb < xbe; xc0++) {
		if (y = *xb++) {
			x = xa;
			xc = xc0;
			carry = 0;
			do {
				z = *x++ * y + *xc + carry;
				carry = z >> 16;
				*xc++ = z & 0xffff;
			}
			while (x < xae);
			*xc = carry;
		}
	}
	for (xc0 = c->x, xc = xc0 + wc; wc > 0 && !*--xc; --wc);
	c->wds = wc;
	return c;
}

static int quorem(Bigint * b, Bigint * S)
{
	int n;
	SLONG borrow, y;
	ULONG carry, q, ys;
	ULONG *bx, *bxe, *sx, *sxe;

	n = S->wds;
#ifdef DEBUG
	/*debug */ if (b->wds > n)
		/*debug */
		Bug("oversize b in quorem");
#endif
	if (b->wds < n)
		return 0;
	sx = S->x;
	sxe = sx + --n;
	bx = b->x;
	bxe = bx + n;
	q = *bxe / (*sxe + 1);		/* ensure q <= true quotient */
#ifdef DEBUG
	/*debug */ if (q > 9)
		/*debug */
		Bug("oversized quotient in quorem");
#endif
	if (q) {
		borrow = 0;
		carry = 0;
		do {
			ys = *sx++ * q + carry;
			carry = ys >> 16;
			y = *bx - (ys & 0xffff) + borrow;
			borrow = y >> 16;
			Sign_Extend(borrow, y);
			*bx++ = y & 0xffff;
		}
		while (sx <= sxe);
		if (!*bxe) {
			bx = b->x;
			while (--bxe > bx && !*bxe)
				--n;
			b->wds = n;
		}
	}
	if (cmp(b, S) >= 0) {
		q++;
		borrow = 0;
		carry = 0;
		bx = b->x;
		sx = S->x;
		do {
			ys = *sx++ + carry;
			carry = ys >> 16;
			y = *bx - (ys & 0xffff) + borrow;
			borrow = y >> 16;
			Sign_Extend(borrow, y);
			*bx++ = y & 0xffff;
		}
		while (sx <= sxe);
		bx = b->x;
		bxe = bx + n;
		if (!*bxe) {
			while (--bxe > bx && !*bxe)
				--n;
			b->wds = n;
		}
	}
	return q;
}

/* dtoa for IEEE arithmetic (dmg): convert double to ASCII string.
 *
 * Inspired by "How to Print Floating-Point Numbers Accurately" by
 * Guy L. Steele, Jr. and Jon L. White [Proc. ACM SIGPLAN '90, pp. 92-101].
 *
 * Modifications:
 *	1. Rather than iterating, we use a simple numeric overestimate
 *	   to determine k = floor(log10(d)).  We scale relevant
 *	   quantities using O(log2(k)) rather than O(k) multiplications.
 *	2. For some modes > 2 (corresponding to ecvt and fcvt), we don't
 *	   try to generate digits strictly left to right.  Instead, we
 *	   compute with fewer bits and propagate the carry if necessary
 *	   when rounding the final digit up.  This is often faster.
 *	3. Under the assumption that input will be rounded nearest,
 *	   mode 0 renders 1e23 as 1e23 rather than 9.999999999999999e22.
 *	   That is, we allow equality in stopping tests when the
 *	   round-nearest rule will give the same floating-point value
 *	   as would satisfaction of the stopping test with strict
 *	   inequality.
 *	4. We remove common factors of powers of 2 from relevant
 *	   quantities.
 *	5. When converting floating-point integers less than 1e16,
 *	   we use floating-point arithmetic rather than resorting
 *	   to multiple-precision integers.
 *	6. When asked to produce fewer than 15 digits, we first try
 *	   to get by with floating-point arithmetic; we resort to
 *	   multiple-precision integer arithmetic only if we cannot
 *	   guarantee that the floating-point calculation has given
 *	   the correctly rounded result.  For k requested digits and
 *	   "uniformly" distributed input, the probability is
 *	   something like 10^(k-15) that we must resort to the SLONG
 *	   calculation.
 */

char *ib__dtoa
	(double d, int mode, int ndigits, int *decpt, int *sign, char **rve) {
	/* Arguments ndigits, decpt, sign are similar to those
	   of ecvt and fcvt; trailing zeros are suppressed from
	   the returned string.  If not null, *rve is set to point
	   to the end of the return value.  If d is +-Infinity or NaN,
	   then *decpt is set to 9999.

	   mode:
	   0 ==> shortest string that yields d when read in
	   and rounded to nearest.
	   1 ==> like 0, but with Steele & White stopping rule;
	   e.g. with IEEE P754 arithmetic , mode 0 gives
	   1e23 whereas mode 1 gives 9.999999999999999e22.
	   2 ==> max(1,ndigits) significant digits.  This gives a
	   return value similar to that of ecvt, except
	   that trailing zeros are suppressed.
	   3 ==> through ndigits past the decimal point.  This
	   gives a return value similar to that from fcvt,
	   except that trailing zeros are suppressed, and
	   ndigits can be negative.
	   4-9 should give the same return values as 2-3, i.e.,
	   4 <= mode <= 9 ==> same return as mode
	   2 + (mode & 1).  These modes are mainly for
	   debugging; often they run slower but sometimes
	   faster than modes 2-3.
	   4,5,8,9 ==> left-to-right digit generation.
	   6-9 ==> don't try fast floating-point estimate
	   (if applicable).

	   Values of mode other than 0-9 are treated as mode 0.

	   Sufficient space is allocated to the return value
	   to hold the suppressed trailing zeros.
	 */

	int bbits, b2, b5, be, dig, i, ieps, ilim0,
		j, j1, k, k0, k_check, leftright, m2, m5, s2, s5, try_quick;
	int ilim = 0, ilim1 = 0, spec_case = 0;	/* pacify gcc */
	SLONG L;
#ifndef Sudden_Underflow
	int denorm;
	ULONG x;
#endif
	Bigint *b, *b1, *delta, *mhi, *S;
	Bigint *mlo = NULL;			/* pacify gcc */
	double d2, ds, eps;
	char *s, *s0;
	static Bigint *result;
	static int result_k;

	if (result) {
		result->k = result_k;
		result->maxwds = 1 << result_k;
		Bfree(result);
		result = 0;
	}

	if (word0(d) & Sign_bit) {
		/* set sign for everything, including 0's and NaNs */
		*sign = 1;
		word0(d) &= ~Sign_bit;	/* clear sign bit */
	}
	else
		*sign = 0;

#if defined(IEEE_Arith) + !defined(WORDS_BIGENDIAN)
#ifdef IEEE_Arith
	if ((word0(d) & Exp_mask) == Exp_mask)
#else
	if (word0(d) == 0x8000)
#endif
	{
		/* Infinity or NaN */
		*decpt = 9999;
		s =
#ifdef IEEE_Arith
			!word1(d) && !(word0(d) & 0xfffff) ? "Infinity" :
#endif
			"NaN";
		if (rve)
			*rve =
#ifdef IEEE_Arith
				s[3] ? s + 8 :
#endif
				s + 3;
		return s;
	}
#endif
#ifdef IBM
	d += 0;						/* normalize */
#endif
	if (!d) {
		*decpt = 1;
		s = "0";
		if (rve)
			*rve = s + 1;
		return s;
	}

	b = d2b(d, &be, &bbits);
#ifdef Sudden_Underflow
	i = (int) (word0(d) >> Exp_shift1 & (Exp_mask >> Exp_shift1));
#else
	if ((i = (int) (word0(d) >> Exp_shift1 & (Exp_mask >> Exp_shift1))) != 0) {
#endif
		d2 = d;
		word0(d2) &= Frac_mask1;
		word0(d2) |= Exp_11;
#ifdef IBM
		if (j = 11 - hi0bits(word0(d2) & Frac_mask))
			d2 /= 1 << j;
#endif

		/* log(x)   ~=~ log(1.5) + (x-1.5)/1.5
		 * log10(x)  =  log(x) / log(10)
		 *      ~=~ log(1.5)/log(10) + (x-1.5)/(1.5*log(10))
		 * log10(d) = (i-Bias)*log(2)/log(10) + log10(d2)
		 *
		 * This suggests computing an approximation k to log10(d) by
		 *
		 * k = (i - Bias)*0.301029995663981
		 *  + ( (d2-1.5)*0.289529654602168 + 0.176091259055681 );
		 *
		 * We want k to be too large rather than too small.
		 * The error in the first-order Taylor series approximation
		 * is in our favor, so we just round up the constant enough
		 * to compensate for any error in the multiplication of
		 * (i - Bias) by 0.301029995663981; since |i - Bias| <= 1077,
		 * and 1077 * 0.30103 * 2^-52 ~=~ 7.2e-14,
		 * adding 1e-13 to the constant term more than suffices.
		 * Hence we adjust the constant term to 0.1760912590558.
		 * (We could get a more accurate k by invoking log10,
		 *  but this is probably not worthwhile.)
		 */

		i -= Bias;
#ifdef IBM
		i <<= 2;
		i += j;
#endif
#ifndef Sudden_Underflow
		denorm = 0;
	}
	else {
		/* d is denormalized */

		i = bbits + be + (Bias + (P - 1) - 1);
		x = i > 32 ? word0(d) << (64 - i) | word1(d) >> (i - 32)
			: word1(d) << (32 - i);
		d2 = x;
		word0(d2) -= 31 * Exp_msk1;	/* adjust exponent */
		i -= (Bias + (P - 1) - 1) + 1;
		denorm = 1;
	}
#endif
	ds =
		(d2 - 1.5) * 0.289529654602168 + 0.1760912590558 +
		i * 0.301029995663981;
	k = (int) ds;
	if (ds < 0. && ds != k)
		k--;					/* want k = floor(ds) */
	k_check = 1;
	if (k >= 0 && k <= Ten_pmax) {
		if (d < tens[k])
			k--;
		k_check = 0;
	}
	j = bbits - i - 1;
	if (j >= 0) {
		b2 = 0;
		s2 = j;
	}
	else {
		b2 = -j;
		s2 = 0;
	}
	if (k >= 0) {
		b5 = 0;
		s5 = k;
		s2 += k;
	}
	else {
		b2 -= k;
		b5 = -k;
		s5 = 0;
	}
	if (mode < 0 || mode > 9)
		mode = 0;
	try_quick = 1;
	if (mode > 5) {
		mode -= 4;
		try_quick = 0;
	}
	leftright = 1;
	switch (mode) {
	case 0:
	case 1:
		ilim = ilim1 = -1;
		i = 18;
		ndigits = 0;
		break;
	case 2:
		leftright = 0;
		/* no break */
	case 4:
		if (ndigits <= 0)
			ndigits = 1;
		ilim = ilim1 = i = ndigits;
		break;
	case 3:
		leftright = 0;
		/* no break */
	case 5:
		i = ndigits + k + 1;
		ilim = i;
		ilim1 = i - 1;
		if (i <= 0)
			i = 1;
	}
	j = sizeof(ULONG);
	for (result_k = 0; sizeof(Bigint) - sizeof(ULONG) + j <= i; j <<= 1)
		result_k++;
	result = Balloc(result_k);
	s = s0 = (char *) result;

	if (ilim >= 0 && ilim <= Quick_max && try_quick) {

		/* Try to get by with floating-point arithmetic. */

		i = 0;
		d2 = d;
		k0 = k;
		ilim0 = ilim;
		ieps = 2;				/* conservative */
		if (k > 0) {
			ds = tens[k & 0xf];
			j = k >> 4;
			if (j & Bletch) {
				/* prevent overflows */
				j &= Bletch - 1;
				d /= bigtens[n_bigtens - 1];
				ieps++;
			}
			for (; j; j >>= 1, i++)
				if (j & 1) {
					ieps++;
					ds *= bigtens[i];
				}
			d /= ds;
		}
		else if ((j1 = -k) != 0) {
			d *= tens[j1 & 0xf];
			for (j = j1 >> 4; j; j >>= 1, i++)
				if (j & 1) {
					ieps++;
					d *= bigtens[i];
				}
		}
		if (k_check && d < 1. && ilim > 0) {
			if (ilim1 <= 0)
				goto fast_failed;
			ilim = ilim1;
			k--;
			d *= 10.;
			ieps++;
		}
		eps = ieps * d + 7.;
		word0(eps) -= (P - 1) * Exp_msk1;
		if (ilim == 0) {
			S = mhi = 0;
			d -= 5.;
			if (d > eps)
				goto one_digit;
			if (d < -eps)
				goto no_digits;
			goto fast_failed;
		}
		if (leftright) {
			/* Use Steele & White method of only
			 * generating digits needed.
			 */
			eps = 0.5 / tens[ilim - 1] - eps;
			for (i = 0;;) {
				L = d;
				d -= L;
				*s++ = '0' + (int) L;
				if (d < eps)
					goto ret1;
				if (1. - d < eps)
					goto bump_up;
				if (++i >= ilim)
					break;
				eps *= 10.;
				d *= 10.;
			}
		}
		else {
			/* Generate ilim digits, then fix them up. */
			eps *= tens[ilim - 1];
			for (i = 1;; i++, d *= 10.) {
				L = d;
				d -= L;
				*s++ = '0' + (int) L;
				if (i == ilim) {
					if (d > 0.5 + eps)
						goto bump_up;
					else if (d < 0.5 - eps) {
						while (*--s == '0');
						s++;
						goto ret1;
					}
					break;
				}
			}
		}
	  fast_failed:
		s = s0;
		d = d2;
		k = k0;
		ilim = ilim0;
	}

	/* Do we have a "small" integer? */

	if (be >= 0 && k <= Int_max) {
		/* Yes. */
		ds = tens[k];
		if (ndigits < 0 && ilim <= 0) {
			S = mhi = 0;
			if (ilim < 0 || d <= 5 * ds)
				goto no_digits;
			goto one_digit;
		}
		for (i = 1;; i++) {
			L = d / ds;
			d -= L * ds;
			*s++ = '0' + (int) L;
			if (i == ilim) {
				d += d;
				if (d > ds || (d == ds && L & 1)) {
				  bump_up:
					while (*--s == '9')
						if (s == s0) {
							k++;
							*s = '0';
							break;
						}
					++*s++;
				}
				break;
			}
			if (!(d *= 10.))
				break;
		}
		goto ret1;
	}

	m2 = b2;
	m5 = b5;
	mhi = mlo = 0;
	if (leftright) {
		if (mode < 2) {
			i =
#ifndef Sudden_Underflow
				denorm ? be + (Bias + (P - 1) - 1 + 1) :
#endif
#ifdef IBM
				1 + 4 * P - 3 - bbits + ((bbits + be - 1) & 3);
#else
				1 + P - bbits;
#endif
		}
		else {
			j = ilim - 1;
			if (m5 >= j)
				m5 -= j;
			else {
				s5 += j -= m5;
				b5 += j;
				m5 = 0;
			}
			if ((i = ilim) < 0) {
				m2 -= i;
				i = 0;
			}
		}
		b2 += i;
		s2 += i;
		mhi = i2b(1);
	}
	if (m2 > 0 && s2 > 0) {
		i = m2 < s2 ? m2 : s2;
		b2 -= i;
		m2 -= i;
		s2 -= i;
	}
	if (b5 > 0) {
		if (leftright) {
			if (m5 > 0) {
				mhi = pow5mult(mhi, m5);
				b1 = mult(mhi, b);
				Bfree(b);
				b = b1;
			}
			if ((j = b5 - m5) != 0)
				b = pow5mult(b, j);
		}
		else
			b = pow5mult(b, b5);
	}
	S = i2b(1);
	if (s5 > 0)
		S = pow5mult(S, s5);

	/* Check for special case that d is a normalized power of 2. */

	if (mode < 2) {
		if (!word1(d) && !(word0(d) & Bndry_mask)
#ifndef Sudden_Underflow
			&& word0(d) & Exp_mask
#endif
			) {
			/* The special case */
			b2 += Log2P;
			s2 += Log2P;
			spec_case = 1;
		}
		else
			spec_case = 0;
	}

	/* Arrange for convenient computation of quotients:
	 * shift left if necessary so divisor has 4 leading 0 bits.
	 *
	 * Perhaps we should just compute leading 28 bits of S once
	 * and for all and pass them and a shift to quorem, so it
	 * can do shifts and ors to compute the numerator for q.
	 */
	if (i = ((s5 ? 32 - hi0bits(S->x[S->wds - 1]) : 1) + s2) & 0xf)
		i = 16 - i;
	if (i > 4) {
		i -= 4;
		b2 += i;
		m2 += i;
		s2 += i;
	}
	else if (i < 4) {
		i += 28;
		b2 += i;
		m2 += i;
		s2 += i;
	}
	if (b2 > 0)
		b = lshift(b, b2);
	if (s2 > 0)
		S = lshift(S, s2);
	if (k_check) {
		if (cmp(b, S) < 0) {
			k--;
			b = multadd(b, 10, 0);	/* we botched the k estimate */
			if (leftright)
				mhi = multadd(mhi, 10, 0);
			ilim = ilim1;
		}
	}
	if (ilim <= 0 && mode > 2) {
		if (ilim < 0 || cmp(b, S = multadd(S, 5, 0)) <= 0) {
			/* no digits, fcvt style */
		  no_digits:
			k = -1 - ndigits;
			goto ret;
		}
	  one_digit:
		*s++ = '1';
		k++;
		goto ret;
	}
	if (leftright) {
		if (m2 > 0)
			mhi = lshift(mhi, m2);

		/* Compute mlo -- check for special case
		 * that d is a normalized power of 2.
		 */

		mlo = mhi;
		if (spec_case) {
			mhi = Balloc(mhi->k);
			Bcopy(mhi, mlo);
			mhi = lshift(mhi, Log2P);
		}

		for (i = 1;; i++) {
			dig = quorem(b, S) + '0';
			/* Do we yet have the shortest decimal string
			 * that will round to d?
			 */
			j = cmp(b, mlo);
			delta = diff(S, mhi);
			j1 = delta->sign ? 1 : cmp(b, delta);
			Bfree(delta);
#ifndef ROUND_BIASED
			if (j1 == 0 && !mode && !(word1(d) & 1)) {
				if (dig == '9')
					goto round_9_up;
				if (j > 0)
					dig++;
				*s++ = dig;
				goto ret;
			}
#endif
			if (j < 0 || (j == 0 && !mode
#ifndef ROUND_BIASED
						  && !(word1(d) & 1)
#endif
				)) {
				if (j1 > 0) {
					b = lshift(b, 1);
					j1 = cmp(b, S);
					if ((j1 > 0 || (j1 == 0 && dig & 1))
						&& dig++ == '9')
						goto round_9_up;
				}
				*s++ = dig;
				goto ret;
			}
			if (j1 > 0) {
				if (dig == '9') {	/* possible if i == 1 */
				  round_9_up:
					*s++ = '9';
					goto roundoff;
				}
				*s++ = dig + 1;
				goto ret;
			}
			*s++ = dig;
			if (i == ilim)
				break;
			b = multadd(b, 10, 0);
			if (mlo == mhi)
				mlo = mhi = multadd(mhi, 10, 0);
			else {
				mlo = multadd(mlo, 10, 0);
				mhi = multadd(mhi, 10, 0);
			}
		}
	}
	else
		for (i = 1;; i++) {
			*s++ = dig = quorem(b, S) + '0';
			if (i >= ilim)
				break;
			b = multadd(b, 10, 0);
		}

	/* Round off last digit */

	b = lshift(b, 1);
	j = cmp(b, S);
	if (j > 0 || (j == 0 && dig & 1)) {
	  roundoff:
		while (*--s == '9')
			if (s == s0) {
				k++;
				*s++ = '1';
				goto ret;
			}
		++*s++;
	}
	else {
		while (*--s == '0');
		s++;
	}
  ret:
	Bfree(S);
	if (mhi) {
		if (mlo && mlo != mhi)
			Bfree(mlo);
		Bfree(mhi);
	}
  ret1:
	Bfree(b);
	if (s == s0) {				/* don't return empty string */
		*s++ = '0';
		k = 0;
	}
	*s = 0;
	*decpt = k + 1;
	if (rve)
		*rve = s;
	return s0;
}

/* end of inclusions from libc/stdlib/netbsd_strtod.c */

/* From libc/locale/collcmp.c */

/*
 * Copyright (C) 1996 by Andrey A. Chernov, Moscow, Russia.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
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
 * $FreeBSD: src/lib/libc/locale/collcmp.c,v 1.11.2.1 1999/08/29 14:47:13 peter Exp $
 */

#define ASCII_COMPATIBLE_COLLATE	/* see usr.bin/colldef/data */

/*
 * Compare two characters converting collate information
 * into ASCII-compatible range, it allows to handle
 * "[a-z]"-type ranges with national characters.
 */

static int ib__collate_range_cmp(c1, c2)
	 int c1, c2;
{
	static char s1[2], s2[2];
	int ret;
#ifndef ASCII_COMPATIBLE_COLLATE
	int as1, as2, al1, al2;
#endif

	c1 &= UCHAR_MAX;
	c2 &= UCHAR_MAX;
	if (c1 == c2)
		return (0);

#ifndef ASCII_COMPATIBLE_COLLATE
	as1 = isascii(c1);
	as2 = isascii(c2);
	al1 = isalpha(c1);
	al2 = isalpha(c2);

	if (as1 || as2 || al1 || al2) {
		if ((as1 && as2) || (!al1 && !al2))
			return (c1 - c2);
		if (al1 && !al2) {
			if (isupper(c1))
				return ('A' - c2);
			else
				return ('a' - c2);
		}
		else if (al2 && !al1) {
			if (isupper(c2))
				return (c1 - 'A');
			else
				return (c1 - 'a');
		}
	}
#endif
	s1[0] = c1;
	s2[0] = c2;
	if ((ret = strcoll(s1, s2)) != 0)
		return (ret);
	return (c1 - c2);
}


#endif /* NEED_IB_STDIO */
