%{
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
 */
#include "../jrd/dbg.h"
extern int	*prior_frame;
#define SYM_FIELD(ptr, field) (((SYMB) ptr)->field)
%}

%token	NUMBER, ROUTINE, OFFSET, SYMBOL, OTHER, ARROW, PRINTER
%left "+","-"
%left '*'
%right ARROW, "."
%left '$'
%%

list	:
	| list statement '\n'
	| list statement ';'
	| list error '\n'
	    {
	    yyerrok;
	    }
	| list 'A'       	/* dummy to get routines loaded */
	    {
	    DMP_active();
	    }
	;

statement : PRINTER expr
	    {
	    int		(*routine)();
	    SYMB	symbol;
	    SLONG	stuff;
	    symbol = (SYMB) ($1);
	    stuff = $2;
	    if (routine = *symbol->symb_value)
		(*routine) (stuff, 0);
	    }
	| '*'  PRINTER expr
	    {
	    int		(*routine)();
	    SYMB	symbol;

	    symbol = (SYMB) ($2);
	    if (routine = *symbol->symb_value)
		{
		DBG_open();
		(*routine) ((SLONG) $3, 0);
		DBG_close();
		}
	    }
	| ROUTINE
	    {
	    int		(*routine)();
	    SYMB	symbol;

	    symbol = (SYMB) ($1);
	    if (routine = *symbol->symb_value)
		(*routine) (0);
	    }
	| '!'
	    {
	    system ("csh");
	    printf ("\n");
	    }
	|	/* Nothing */
	;

expr	: SYMBOL
	    {
	    SYMB	symbol;

	    symbol = (SYMB) ($1);
	    $$ = (YYSTYPE) symbol->symb_value;
	    }
	| NUMBER
	| expr '+' expr
	    {
	    $$ = $1 + $3;
	    }
	| expr '-' expr
	    {
	    $$ = $1 - $3;
	    }
	| OFFSET
	    {
	    SYMB	symbol;

	    symbol = (SYMB) ($1);
	    $$ = (YYSTYPE) symbol->symb_value;
	    }
	| expr '.' OFFSET
	    {
	    SYMB	symbol;

	    symbol = (SYMB) ($3);
	    $$ = $1 + (int) symbol->symb_value;
	    }
	| expr ARROW OFFSET
	    {
	    SYMB	symbol;

	    symbol = (SYMB) ($3);
	    $$ = *(int *) ($1 + symbol->symb_value);
	    }
	| '$' expr
	    {
		$$ = prior_frame [$2 + 1];
	    }
	| '*' expr
	    {
		$$ = *(int *) $2;
	    }
	| '(' expr ')'
	    {
		$$ = $2;
	    }
	;
%%

#define JLX_C_INCLUDED_BY_GRAMMAR_CPP
#include "../jrd/jlx.c"
#undef JLX_C_INCLUDED_BY_GRAMMAR_CPP
