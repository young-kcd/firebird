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
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"

#define CHR_LETTER	1
#define CHR_DIGIT	2
#define CHR_IDENT	4
#define CHR_QUOTE	8
#define CHR_WHITE	16
#define CHR_HEX		32

extern struct symb dbt_symbols[];

static SSHORT symb_types[] = { ROUTINE, OFFSET, PRINTER, SYMBOL };

static SCHAR classes[] = {
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
	SCHAR *p, *q;
	SCHAR class_, string[31];
	SSHORT c;
	SLONG number;

	while (((class_ = classes[c = ib_getchar()]) & CHR_WHITE) && c != -1);

	if (c == -1)
		return c;

	if (class_ & CHR_LETTER) {
		p = string;
		*p++ = c;
		while (classes[c = ib_getchar()] & CHR_IDENT)
			*p++ = c;
		*p = 0;
		ib_ungetc(c, ib_stdin);
		for (symbol = dbt_symbols; q = symbol->symb_string; symbol++) {
			for (p = string; *p;)
				if (*p++ != *q++)
					break;
			if (p[-1] == q[-1]) {
				yylval = (YYSTYPE) symbol;
				return symb_types[(int) symbol->symb_type];
			}
		}
		ib_printf("*** %s undefined ***\n", string);
		return OTHER;
	}

	if (c == '0') {
		number = 0;
		while (classes[c = ib_getchar()] & CHR_HEX) {
			if (c >= 'a' && c <= 'f')
				c = c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				c = c - 'A' + 10;
			else
				c = c - '0';
			number = number * 16 + c;
		}
		ib_ungetc(c, ib_stdin);
		yylval = number;
		return NUMBER;
	}

	if (class_ & CHR_DIGIT) {
		number = c - '0';
		while (classes[c = ib_getchar()] & CHR_DIGIT)
			number = number * 10 + (c - '0');
		ib_ungetc(c, ib_stdin);
		yylval = number;
		return NUMBER;
	}

	if (c == '-')
		if ((c = ib_getchar()) == '>')
			return ARROW;
		else
			ib_ungetc(c, ib_stdin);

	return c;
}
