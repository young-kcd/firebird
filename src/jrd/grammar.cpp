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

#include "firebird.h"
#include <stdlib.h>


# line 44 "grammar.y"
#include "../jrd/dbg.h"

void yyerror(const char *string);	// defined in dbg.cpp

void DMP_active(void);
int DBG_open(void);
int DBG_close(void);


extern int *prior_frame;
#define SYM_FIELD(ptr, field) (((SYMB) ptr)->field)
# define NUMBER 257
# define ROUTINE 258
# define OFFSET 259
# define SYMBOL 260
# define OTHER 261
# define ARROW 262
# define PRINTER 263
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256


# line 159 "grammar.y"


#define JLX_C_INCLUDED_BY_GRAMMAR_CPP
#include "../jrd/jlx.c"
#undef JLX_C_INCLUDED_BY_GRAMMAR_CPP

int yyexca[] = {
	-1, 1,
	0, -1,
	10, 10,
	59, 10,
	-2, 0,
};
# define YYNPROD 21
# define YYLAST 237
int yyact[] = {

	8, 32, 22, 20, 19, 21, 22, 31, 20, 6,
	21, 22, 16, 30, 9, 11, 18, 12, 17, 2,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 4, 0, 24, 25, 26, 27, 28, 29,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 10, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 23, 0,
	0, 0, 23, 3, 0, 7, 0, 23, 0, 0,
	5, 0, 0, 14, 0, 15, 13
};
int yypact[] = {

	-1000, -33, 4, 5, -1000, -24, -259, -1000, -1000, -1000,
	-1000, -1000, -35, -1000, -1000, -1000, -24, -24, -24, -24,
	-24, -24, -246, -252, -1000, -44, -40, -35, -44, -44,
	-1000, -1000, -1000
};
int yypgo[] = {

	0, 20, 19, 17
};
int yyr1[] = {

	0, 1, 1, 1, 1, 1, 2, 2, 2, 2,
	2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3
};
int yyr2[] = {

	0, 0, 6, 6, 7, 5, 5, 7, 3, 3,
	0, 3, 2, 7, 7, 3, 7, 7, 5, 5,
	7
};
int yychk[] = {

	-1000, -1, -2, 256, 65, 263, 42, 258, 33, 10,
	59, 10, -3, 260, 257, 259, 36, 42, 40, 263,
	43, 45, 46, 262, -3, -3, -3, -3, -3, -3,
	259, 259, 41
};
int yydef[] = {

	1, -2, 0, 0, 5, 0, 0, 8, 9, 2,
	3, 4, 6, 11, 12, 15, 0, 0, 0, 0,
	0, 0, 0, 0, 18, 19, 0, 7, 13, 14,
	16, 17, 20
};
typedef struct {
	const char* t_name;
	int t_val;
} yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0			/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] = {
	"NUMBER", 257,
	"ROUTINE", 258,
	"OFFSET", 259,
	"SYMBOL", 260,
	"OTHER", 261,
	"ARROW", 262,
	"PRINTER", 263,
	"+", 43,
	"-", 45,
	"*", 42,
	".", 46,
	"$", 36,
	"-unknown-", -1				/* ends search */
};

char *yyreds[] = {
	"-no such reduction-",
	"list : /* empty */",
	"list : list statement '\n'",
	"list : list statement ';'",
	"list : list error '\n'",
	"list : list 'A'",
	"statement : PRINTER expr",
	"statement : '*' PRINTER expr",
	"statement : ROUTINE",
	"statement : '!'",
	"statement : /* empty */",
	"expr : SYMBOL",
	"expr : NUMBER",
	"expr : expr '+' expr",
	"expr : expr '-' expr",
	"expr : OFFSET",
	"expr : expr '.' OFFSET",
	"expr : expr ARROW OFFSET",
	"expr : '$' expr",
	"expr : '*' expr",
	"expr : '(' expr ')'",
};
#endif /* YYDEBUG */
#line 1 "/usr/lib/yaccpar"
/*	@(#)yaccpar 1.10 89/04/04 SMI; from S5R3 1.10	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1			/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;					/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** static variables used by the parser
*/
static YYSTYPE *yyv;			/* value stack */
static int *yys;				/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;				/* top of state stack */

static int yystate;				/* current state */
static int yytmp;				/* extra var (lasts between blocks) */

int yynerrs;					/* number of errors */

int yyerrflag;					/* error recovery flag */
int yychar;						/* current input token number */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int yyparse()
{
	YYSTYPE *yypvt;	/* top of value stack for $vars */
	unsigned yymaxdepth = YYMAXDEPTH;

	/*
	   ** Initialize externals - yyparse may be called more than once
	 */
	yyv = (YYSTYPE *) malloc(yymaxdepth * sizeof(YYSTYPE));
	yys = (int *) malloc(yymaxdepth * sizeof(int));
	if (!yyv || !yys) {
		yyerror("out of memory");
		return (1);
	}
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		YYSTYPE *yy_pv;	/* top of value stack */
		int *yy_ps;	/* top of state stack */
		int yy_state;	/* current state */
		int yy_n;		/* internal state number info */

		/*
		   ** get globals into registers.
		   ** branch to here only if YYBACKUP was called.
		 */
	  yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		   ** get globals into registers.
		   ** either we just started, or we just finished a reduction
		 */
	  yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		   ** top of for (;;) loop while no reductions done
		 */
	  yy_stack:
		/*
		   ** put a state and value onto the stacks
		 */
#if YYDEBUG
		/*
		   ** if debugging, look up token value in list of value vs.
		   ** name pairs.  0 and negative (-1) are special values.
		   ** Note: linear search is used since time is not a real
		   ** consideration while debugging.
		 */
		if (yydebug) {
			int yy_i;

			printf("State %d, token ", yy_state);
			if (yychar == 0)
				printf("end-of-file\n");
			else if (yychar < 0)
				printf("-none-\n");
			else {
				for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
					if (yytoks[yy_i].t_val == yychar)
						break;
				}
				printf("%s\n", yytoks[yy_i].t_name);
			}
		}
#endif /* YYDEBUG */
		if (++yy_ps >= &yys[yymaxdepth]) {	/* room on stack? */
			/*
			   ** reallocate and recover.  Note that pointers
			   ** have to be reset, or bad things will happen
			 */
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			yymaxdepth += YYMAXDEPTH;
			yyv = (YYSTYPE *) realloc((char *) yyv,
									  yymaxdepth * sizeof(YYSTYPE));
			yys = (int *) realloc((char *) yys, yymaxdepth * sizeof(int));
			if (!yyv || !yys) {
				yyerror("yacc stack overflow");
				return (1);
			}
			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		   ** we have a new state - find out what to do
		 */
	  yy_newstate:
		if ((yy_n = yypact[yy_state]) <= YYFLAG)
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		   ** if debugging, need to mark whether new token grabbed
		 */
		yytmp = yychar < 0;
#endif
		if ((yychar < 0) && ((yychar = yylex()) < 0))
			yychar = 0;			/* reached EOF */
#if YYDEBUG
		if (yydebug && yytmp) {
			int yy_i;

			printf("Received token ");
			if (yychar == 0)
				printf("end-of-file\n");
			else if (yychar < 0)
				printf("-none-\n");
			else {
				for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
					if (yytoks[yy_i].t_val == yychar)
						break;
				}
				printf("%s\n", yytoks[yy_i].t_name);
			}
		}
#endif /* YYDEBUG */
		if (((yy_n += yychar) < 0) || (yy_n >= YYLAST))
			goto yydefault;
		if (yychk[yy_n = yyact[yy_n]] == yychar) {	/*valid shift */
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if (yyerrflag > 0)
				yyerrflag--;
			goto yy_stack;
		}

	  yydefault:
		if ((yy_n = yydef[yy_state]) == -2) {
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ((yychar < 0) && ((yychar = yylex()) < 0))
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if (yydebug && yytmp) {
				int yy_i;

				printf("Received token ");
				if (yychar == 0)
					printf("end-of-file\n");
				else if (yychar < 0)
					printf("-none-\n");
				else {
					for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
						if (yytoks[yy_i].t_val == yychar) {
							break;
						}
					}
					printf("%s\n", yytoks[yy_i].t_name);
				}
			}
#endif /* YYDEBUG */
			/*
			   ** look through exception table
			 */
			{
				int *yyxi = yyexca;

				while ((*yyxi != -1) || (yyxi[1] != yy_state)) {
					yyxi += 2;
				}
				while ((*(yyxi += 2) >= 0) && (*yyxi != yychar));
				if ((yy_n = yyxi[1]) < 0)
					YYACCEPT;
			}
		}

		/*
		   ** check for syntax error
		 */
		if (yy_n == 0) {		/* have an error */
			/* no worry about speed here! */
			switch (yyerrflag) {
			case 0:			/* new error */
				yyerror("syntax error");
				goto skip_init;
			  yyerrlab:
				/*
				   ** get globals into registers.
				   ** we have a user generated syntax type error
				 */
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			  skip_init:
			case 1:
			case 2:			/* incompletely recovered error */
				/* try again... */
				yyerrflag = 3;
				/*
				   ** find state where "error" is a legal
				   ** shift action
				 */
				while (yy_ps >= yys) {
					yy_n = yypact[*yy_ps] + YYERRCODE;
					if (yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE) {
						/*
						   ** simulate shift of "error"
						 */
						yy_state = yyact[yy_n];
						goto yy_stack;
					}
					/*
					   ** current state has no shift on
					   ** "error", pop stack
					 */
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if (yydebug)
						printf(_POP_, *yy_ps, yy_ps[-1]);
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				   ** there is no state on stack with "error" as
				   ** a valid shift.  give up.
				 */
				YYABORT;
			case 3:			/* no shift yet; eat a token */
#if YYDEBUG
				/*
				   ** if debugging, look up token in list of
				   ** pairs.  0 and negative shouldn't occur,
				   ** but since timing doesn't matter when
				   ** debugging, it doesn't hurt to leave the
				   ** tests here.
				 */
				if (yydebug) {
					int yy_i;

					printf("Error recovery discards ");
					if (yychar == 0)
						printf("token end-of-file\n");
					else if (yychar < 0)
						printf("token -none-\n");
					else {
						for (yy_i = 0; yytoks[yy_i].t_val >= 0; yy_i++) {
							if (yytoks[yy_i].t_val == yychar) {
								break;
							}
						}
						printf("token %s\n", yytoks[yy_i].t_name);
					}
				}
#endif /* YYDEBUG */
				if (yychar == 0)	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}						/* end if ( yy_n == 0 ) */
		/*
		   ** reduction by production yy_n
		   ** put stack tops, etc. so things right after switch
		 */
#if YYDEBUG
		/*
		   ** if debugging, print the string that is the user's
		   ** specification of the reduction which is just about
		   ** to be done.
		 */
		if (yydebug)
			printf("Reduce by (%d) \"%s\"\n", yy_n, yyreds[yy_n]);
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		   ** Look in goto table for next state
		   ** Sorry about using yy_state here as temporary
		   ** variable, but why not, if it works...
		   ** If yyr2[ yy_n ] doesn't have the low order bit
		   ** set, then there is no action to be done for
		   ** this reduction.  So, no saving & unsaving of
		   ** registers done.  The only difference between the
		   ** code just after the if and the body of the if is
		   ** the goto yy_stack in the body.  This way the test
		   ** can be made before the choice of what to do is needed.
		 */
		{
			/* length of production doubled with extra bit */
			int yy_len = yyr2[yy_n];

			if (!(yy_len & 01)) {
				yy_len >>= 1;
				yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
				yy_state = yypgo[yy_n = yyr1[yy_n]] + *(yy_ps -= yy_len) + 1;
				if (yy_state >= YYLAST ||
					yychk[yy_state = yyact[yy_state]] != -yy_n) {
					yy_state = yyact[yypgo[yy_n]];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = (yy_pv -= yy_len)[1];	/* $$ = $1 */
			yy_state = yypgo[yy_n = yyr1[yy_n]] + *(yy_ps -= yy_len) + 1;
			if (yy_state >= YYLAST ||
				yychk[yy_state = yyact[yy_state]] != -yy_n) {
				yy_state = yyact[yypgo[yy_n]];
			}
		}
		/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	   ** code supplied by user is placed in this switch
	 */
	switch (yytmp) {

	case 4:
# line 60 "grammar.y"
		{
			yyerrok;
		}
		break;
	case 5:
# line 64 "grammar.y"
		{
			DMP_active();
		}
		break;
	case 6:
# line 70 "grammar.y"
		{
			int (*routine) ();
			SYMB symbol;
			SLONG stuff;
			symbol = (SYMB) (yypvt[-1]);
			stuff = yypvt[-0];
			if (routine = *symbol->symb_value)
				reinterpret_cast < void (*) (...) > (*routine) (stuff, 0);
		} break;
	case 7:
# line 80 "grammar.y"
		{
			int (*routine) ();
			SYMB symbol;

			symbol = (SYMB) (yypvt[-1]);
			if (routine = *symbol->symb_value) {
				DBG_open();
				reinterpret_cast < void (*) (...) >
					(*routine) ((SLONG) yypvt[-0], 0);
				DBG_close();
			}
		} break;
	case 8:
# line 93 "grammar.y"
		{
			int (*routine) ();
			SYMB symbol;

			symbol = (SYMB) (yypvt[-0]);
			if (routine = *symbol->symb_value)
				reinterpret_cast < void (*) (...) > (*routine) (0);
		} break;
	case 9:
# line 102 "grammar.y"
		{
			system("csh");
			printf("\n");
		}
		break;
	case 11:
# line 110 "grammar.y"
		{
			SYMB symbol;

			symbol = (SYMB) (yypvt[-0]);
			yyval = (YYSTYPE) symbol->symb_value;
		}
		break;
	case 13:
# line 118 "grammar.y"
		{
			yyval = yypvt[-2] + yypvt[-0];
		}
		break;
	case 14:
# line 122 "grammar.y"
		{
			yyval = yypvt[-2] - yypvt[-0];
		}
		break;
	case 15:
# line 126 "grammar.y"
		{
			SYMB symbol;

			symbol = (SYMB) (yypvt[-0]);
			yyval = (YYSTYPE) symbol->symb_value;
		}
		break;
	case 16:
# line 133 "grammar.y"
		{
			SYMB symbol;

			symbol = (SYMB) (yypvt[-0]);
			yyval = yypvt[-2] + (int) symbol->symb_value;
		} break;
	case 17:
# line 140 "grammar.y"
		{
			SYMB symbol;

			symbol = (SYMB) (yypvt[-0]);
			yyval = *(int *) (yypvt[-2] + symbol->symb_value);
		} break;
	case 18:
# line 147 "grammar.y"
		{
			yyval = prior_frame[yypvt[-0] + 1];
		}
		break;
	case 19:
# line 151 "grammar.y"
		{
			yyval = *(int *) yypvt[-0];
		} break;
	case 20:
# line 155 "grammar.y"
		{
			yyval = yypvt[-1];
		}
		break;
	}
	goto yystack;				/* reset registers in driver code */
}


