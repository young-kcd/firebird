/*
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
 * Changes made by Claudio Valderrama for the Firebird project 
 *   changes to substr and added substrlen 
 * 
 */
#include "firebird.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include "ib_util.h"
#include "ib_udf.h"

#ifndef SOLARIS

int MATHERR(struct exception *e)
{
	return 1;
}

#endif

double EXPORT IB_UDF_abs( double *a)
{
	return (*a < 0.0) ? -*a : *a;
}

double EXPORT IB_UDF_acos( double *a)
{
	return (acos(*a));
}

char *EXPORT IB_UDF_ascii_char( int *a)
{
	char *b;
	b = (char *) ib_util_malloc(2);
	*b = (char) (*a);
	/* let us not forget to NULL terminate */
	b[1] = '\0';
	return (b);
}

int EXPORT IB_UDF_ascii_val( char *a)
{
	return ((int) (*a));
}

double EXPORT IB_UDF_asin( double *a)
{
	return (asin(*a));
}

double EXPORT IB_UDF_atan( double *a)
{
	return (atan(*a));
}

double EXPORT IB_UDF_atan2( double *a, double *b)
{
	return (atan2(*a, *b));
}

long EXPORT IB_UDF_bin_and( long *a, long *b)
{
	return (*a & *b);
}

long EXPORT IB_UDF_bin_or( long *a, long *b)
{
	return (*a | *b);
}

long EXPORT IB_UDF_bin_xor( long *a, long *b)
{
	return (*a ^ *b);
}

double EXPORT IB_UDF_ceiling( double *a)
{
	return (ceil(*a));
}

double EXPORT IB_UDF_cos( double *a)
{
	return (cos(*a));
}

double EXPORT IB_UDF_cosh( double *a)
{
	return (cosh(*a));
}

double EXPORT IB_UDF_cot( double *a)
{
	return (1.0 / tan(*a));
}

double EXPORT IB_UDF_div( long *a, long *b)
{
	if (*b != 0) {
		div_t div_result = div(*a, *b);
		return (div_result.quot);
	}
	else
		/* This is a Kludge!  We need to return INF, 
		   but this seems to be the only way to do 
		   it since there seens to be no constant for it. */
		return (1 / tan(0));

}

double EXPORT IB_UDF_floor( double *a)
{
	return (floor(*a));
}

double EXPORT IB_UDF_ln( double *a)
{
	return (log(*a));
}

double EXPORT IB_UDF_log( double *a, double *b)
{
	return (log(*a) / log(*b));
}

double EXPORT IB_UDF_log10( double *a)
{
	return (log10(*a));
}

char *EXPORT IB_UDF_lower( char *s)
{
	char *buf;
	char *p;

	buf = (char *) ib_util_malloc(strlen(s) + 1);
	p = buf;
	while (*s)
		if (*s >= 'A' && *s <= 'Z') {
			*p++ = *s++ - 'A' + 'a';
		}
		else
			*p++ = *s++;
	*p = '\0';

	return buf;
}

char *EXPORT IB_UDF_lpad( char *s, long *a, char *c)
{
	char *buf;
	char *p;
	long avalue = *a;
	long length;
	long padlength;
	long current;

	if (avalue > 0 && (length = strlen(s)) ) {
		current = 0;
		padlength = strlen(c);
		buf = (char*) ib_util_malloc(avalue + 1);

		while (current + length < avalue) {
			memcpy(&buf[current], c, padlength);
			current += padlength;
		}
		memcpy(&buf[(avalue - length < 0) ? 0 : avalue - length], s,
			(avalue - length < 0) ? avalue : length);

		p = buf + avalue;
		*p = '\0';
	}
	else
		return NULL;

	return buf;
}

char *EXPORT IB_UDF_ltrim( char *s)
{
	char *buf;
	char *p;
	long length;

	if ( (length = strlen(s)) ) {
		while (*s == ' ')		/* skip leading blanks */
			s++;

		length = strlen(s);
		buf = (char *) ib_util_malloc(length + 1);
		memcpy(buf, s, length);
		p = buf + length;
		*p = '\0';
	}
	else
		return NULL;

	return buf;
}

double EXPORT IB_UDF_mod( long *a, long *b)
{
	if (*b != 0) {
		div_t div_result = div(*a, *b);
		return (div_result.rem);
	}
	else
		/* This is a Kludge!  We need to return INF, 
		   but this seems to be the only way to do 
		   it since there seens to be no constant for it. */
		return (1 / tan(0));
}

double EXPORT IB_UDF_pi()
{
	return (IB_PI);
}

double EXPORT IB_UDF_rand()
{
	srand((unsigned) time(NULL));
	return ((float) rand() / (float) RAND_MAX);
}

char *EXPORT IB_UDF_rpad( char *s, long *a, char *c)
{
	char *buf;
	char *p;
	long avalue = *a;
	long length;
	long padlength;
	long current;

	if (avalue > 0 && (length = strlen(s)) ) {
		current = (avalue - length) < 0 ? avalue : length;
		padlength = strlen(c);
		buf = (char*) ib_util_malloc (avalue + 1);
		memcpy(&buf[0], s, current);

		while (current + padlength < avalue) {
			memcpy(&buf[current], c, padlength);
			current += padlength;
		}
		memcpy(&buf[current], c, avalue - current);

		p = buf + avalue;
		*p = '\0';
	}
	else
		return NULL;

	return buf;
}

char *EXPORT IB_UDF_rtrim( char *s)
{
	char *p;
	char *q;
	char *buf;
	long length;

	if ( (length = strlen(s)) ) {
		p = s + length - 1;
		while ((s != p) && (*p == ' '))
			p--;
		length = p - s + 1;
		buf = (char *) ib_util_malloc(length + 1);
		memcpy(buf, s, length);
		q = buf + length;
		*q = '\0';
	}
	else
		return NULL;

	return buf;
}

int EXPORT IB_UDF_sign( double *a)
{
	if (*a > 0)
		return 1;
	if (*a < 0)
		return -1;
	/* If neither is true then it eauals 0 */
	return 0;
}

double EXPORT IB_UDF_sin( double *a)
{
	return (sin(*a));
}

double EXPORT IB_UDF_sinh( double *a)
{
	return (sinh(*a));
}

double EXPORT IB_UDF_sqrt( double *a)
{
	return (sqrt(*a));
}

char* EXPORT IB_UDF_substr(char* s, short* m, short* n)
{
	char *p;
	char *buf;
	long length;

	if (!s) {
		return 0;
	}

	length = strlen(s);
	if (!length ||
		*m > *n ||
		*m < 1  ||
		*n < 1  ||
		*m > length)
	{
		buf = (char*)ib_util_malloc(1);
		buf[0] = '\0';
	}
	else
	{
		/* we want from the mth char to the
		   nth char inclusive, so add one to
		   the length. */
		/* CVC: We need to compensate for n if it's longer than s's length */
		if (*n > length) {
			length -= *m - 1;
		} else {
			length = *n - *m + 1;
		}
		buf = (char*)ib_util_malloc (length + 1);
		memcpy(buf, s + *m - 1, length);
		p = buf + length;
		*p = '\0';
	}
	return buf;
}

char* EXPORT IB_UDF_substrlen(char* s, short* m, short* n)
{
	char* p;
	char* buf;
	long length;

	/* Created by Claudio Valderrama for the Firebird project,
		2001.04.17 We don't want to return NULL when params are wrong
		and we'll return the remaining characters if the final position
		is greater than the string's length, unless NULL is provided.
	*/
	if (!s) {
		return NULL;
	}
	length = strlen(s);
	if (!length ||
		*m < 1  ||
		*n < 1  ||
		*m > length)
	{
		buf = (char*)ib_util_malloc(1);
		buf[0] = '\0';
	}
	else {
		/* we want from the mth char to the (m+n)th char inclusive,
		 * so add one to the length.
		 */
		/* CVC: We need to compensate for n if it's longer than s's length */
		if (*m + *n - 1 > length) {
			length -= *m - 1;
		} else {
			length = *n;
		}
		buf = (char*) ib_util_malloc (length + 1);
		memcpy(buf, s + *m - 1, length);
		p = buf + length;
		*p = '\0';
	}
	return buf;
}

int EXPORT IB_UDF_strlen( char *a)
{
	return (strlen(a));
}

double EXPORT IB_UDF_tan( double *a)
{
	return (tan(*a));
}

double EXPORT IB_UDF_tanh( double *a)
{
	return (tanh(*a));
}
