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

#include "../jrd/common.h"

#include <stdio.h>
#include <stdarg.h>

#define ib_fpos_t	fpos_t

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

#endif /* !IB_STDIO_H_ */
