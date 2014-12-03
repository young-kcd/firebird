/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jlx.c
 *	DESCRIPTION:	Jim's non-piggy LEX
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

#ifndef JLX_C_INCLUDED_BY_GRAMMAR_CPP
#error This file gets included by grammar.cpp. Don not try to compile it stand-alone.
#endif

#include "firebird.h"
#include <stdio.h>
#include "../jrd/common.h"

const UCHAR CHR_LETTER	= 1;
const UCHAR CHR_DIGIT	= 2;
const UCHAR CHR_IDENT	= 4;
const UCHAR CHR_QUOTE	= 8;
const UCHAR CHR_WHITE	= 16;
const UCHAR CHR_HEX		= 32;

extern struct symb dbt_symbols[];

static const SSHORT symb_types[] = { ROUTINE, OFFSET, PRINTER, SYMBOL };

static const SCHAR classes[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 8, 0, 4, 0, 0, 8,
	0, 0, 0, 0, 0, 0, 0, 0, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	0, 0, 0, 0, 0, 0, 0, 37, 37, 37, 37, 37, 37, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 4, 0, 37,
	37, 37, 37, 37, 37, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5
};




static int yylex(void)
{
/**************************************
 *
 *	y y l e x
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYMB symbol;
	SCHAR *p;
	const char* q;
	SCHAR string[31];
	SSHORT c;
	SLONG number;

	SCHAR char_class;
	while (((char_class = classes[c = getchar()]) & CHR_WHITE) && c != -1);

	if (c == -1)
		return c;

	if (char_class & CHR_LETTER) {
		p = string;
		*p++ = c;
		while (classes[c = getchar()] & CHR_IDENT)
			*p++ = c;
		*p = 0;
		ungetc(c, stdin);
		for (symbol = dbt_symbols; q = symbol->symb_string; symbol++) {
			for (p = string; *p;)
				if (*p++ != *q++)
					break;
			if (p[-1] == q[-1]) {
				yylval = (YYSTYPE) symbol;
				return symb_types[(int) symbol->symb_type];
			}
		}
		printf("*** %s undefined ***\n", string);
		return OTHER;
	}

	if (c == '0') {
		number = 0;
		while (classes[c = getchar()] & CHR_HEX) {
			if (c >= 'a' && c <= 'f')
				c = c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				c = c - 'A' + 10;
			else
				c = c - '0';
			number = number * 16 + c;
		}
		ungetc(c, stdin);
		yylval = number;
		return NUMBER;
	}

	if (char_class & CHR_DIGIT) {
		number = c - '0';
		while (classes[c = getchar()] & CHR_DIGIT)
			number = number * 10 + (c - '0');
		ungetc(c, stdin);
		yylval = number;
		return NUMBER;
	}

	if (c == '-')
		if ((c = getchar()) == '>')
			return ARROW;
		else
			ungetc(c, stdin);

	return c;
}

