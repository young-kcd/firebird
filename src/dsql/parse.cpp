#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (DSQL_yychar=(-1))
#define yyerrok (DSQL_yyerrflag=0)
#define YYRECOVERING (DSQL_yyerrflag!=0)
#define YYPREFIX "yy"
/* 
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		parse.y
 *	DESCRIPTION:	Dynamic SQL parser
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
 *
 * 2002-02-24 Sean Leyne - Code Cleanup of old Win 3.1 port (WINDOWS_ONLY)
 * 2001.05.20 Neil McCalden: Allow a udf to be used in a 'group by' clause.
 * 2001.05.30 Claudio Valderrama: DROP TABLE and DROP VIEW lead now to two
 *   different node types so DDL can tell which is which.
 * 2001.06.13 Claudio Valderrama: SUBSTRING is being surfaced.
 * 2001.06.30 Claudio valderrama: Feed (line,column) for each node. See node.h.
 * 2001.07.10 Claudio Valderrama: Better (line,column) report and "--" for comments.
 * 2001.07.28 John Bellardo: Changes to support parsing LIMIT and FIRST
 * 2001.08.03 John Bellardo: Finalized syntax for LIMIT, change LIMIT to SKIP
 * 2001.08.05 Claudio Valderrama: closed Bug #448062 and other spaces that appear
 *   in rdb$*_source fields when altering domains plus one unexpected null pointer.
 * 2001.08.12 Claudio Valderrama: adjust SUBSTRING's starting pos argument here
 *   and not in gen.c; this closes Bug #450301.
 * 2001.10.01 Claudio Valderrama: enable explicit GRANT...to ROLE role_name.
 * 2001.10.06 Claudio Valderrama: Honor explicit USER keyword in GRANTs and REVOKEs.
 * 2002.07.05 Mark O'Donohue: change keyword DEBUG to KW_DEBUG to avoid
 *			clashes with normal DEBUG macro.
 * 2002.07.30 Arno Brinkman:  
 * 2002.07.30 	Let IN predicate handle value_expressions
 * 2002.07.30 	tokens CASE, NULLIF, COALESCE added
 * 2002.07.30 	See block < CASE expression > what is added to value as case_expression
 * 2002.07.30 	function is split up into aggregate_function, numeric_value_function, string_value_function, generate_value_function
 * 2002.07.30 	new group_by_function and added to grp_column_elem
 * 2002.07.30 	cast removed from function and added as cast_specification to value
 * 2002.08.04 Claudio Valderrama: allow declaring and defining variables at the same time
 * 2002.08.04 Dmitry Yemanov: ALTER VIEW
 * 2002.08.06 Arno Brinkman: ordinal added to grp_column_elem for using positions in group by
 * 2002.08.07 Dmitry Yemanov: INT64/LARGEINT are replaced with BIGINT and available in dialect 3 only
 * 2002.08.31 Dmitry Yemanov: allowed user-defined index names for PK/FK/UK constraints
 * 2002.09.01 Dmitry Yemanov: RECREATE VIEW
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *							exception handling in SPs/triggers,
 *							implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.12.03 Dmitry Yemanov: Implemented ORDER BY clause in subqueries.
 * 2002.12.18 Dmitry Yemanov: Added support for SQL-compliant labels and LEAVE statement
 * 2002.12.28 Dmitry Yemanov: Added support for parametrized events.
 * 2003.01.14 Dmitry Yemanov: Fixed bug with cursors in triggers.
 * 2003.01.15 Dmitry Yemanov: Added support for runtime trigger action checks.
 * 2003.02.10 Mike Nordell  : Undefined Microsoft introduced macros to get a clean compile.
 * 2003.05.24 Nickolay Samofatov: Make SKIP and FIRST non-reserved keywords
 * 2003.06.13 Nickolay Samofatov: Make INSERTING/UPDATING/DELETING non-reserved keywords
 * 2003.07.01 Blas Rodriguez Somoza: Change DEBUG and IN to avoid conflicts in win32 build/bison
 * 2003.08.11 Arno Brinkman: Changed GROUP BY to support all expressions and added "AS" support
 *						   with table alias. Also removed group_by_function and ordinal.
 * 2003.08.14 Arno Brinkman: Added support for derived tables.
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL.
 * 2004.01.16 Vlad Horsun: added support for default parameters and 
 *   EXECUTE BLOCK statement
 */

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/keywords.h"
#include "../dsql/misc_func.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd.h"
#include "../jrd/err_proto.h"

static void	yyerror(const TEXT*);

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

const long SHRT_POS_MAX			= 32767;
const long SHRT_UNSIGNED_MAX	= 65535;
const long SHRT_NEG_MAX			= 32768;
const long LONG_POS_MAX			= 2147483647;
const int POSITIVE	= 0;
const int NEGATIVE	= 1;
const int UNSIGNED	= 2;

/*const int MIN_CACHE_BUFFERS	= 250;*/
/*const int DEF_CACHE_BUFFERS	= 1000;*/

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048

/* Using this option causes build problems on Win32 with bison 1.28*/
/*#define YYSTACK_USE_ALLOCA 1*/

typedef dsql_nod* YYSTYPE;
#define YYSTYPE YYSTYPE
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME[] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING[] = "";

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


dsql_nod* DSQL_parse;

/*static void	yyerror(const TEXT*); redeclaration.*/

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, bool* stmt_ambiguous

#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

static const TEXT* lex_position();
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(dsql_nod*, SLONG*);
#endif
static dsql_fld*	make_field (dsql_nod*);
static dsql_fil*	make_file();
static dsql_nod*	make_list (dsql_nod*);
static dsql_nod*	make_node (NOD_TYPE, int, ...);
static dsql_nod*	make_parameter (void);
static dsql_nod*	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(dsql_nod*, SLONG*, SSHORT);
#endif
static void	stack_nodes (dsql_nod*, DsqlNodStack&);
inline static int	yylex (USHORT, USHORT, USHORT, bool*);
static void	yyabandon (SSHORT, ISC_STATUS);

inline void check_bound(const char* const to, const char* const string)
{
	if ((to - string) >= MAX_TOKEN_LEN)
		yyabandon (-104, isc_token_too_long);
}

inline void check_copy_incr(char*& to, const char ch, const char* const string)
{
	check_bound(to, string); 
	*to++ = ch;
}

struct LexerState {
	/* This is, in fact, parser state. Not used in lexer itself */
	dsql_fld* g_field;
	dsql_fil* g_file;
	dsql_nod* g_field_name;
	bool log_defined, cache_defined;
	int dsql_debug;
	
	/* Actual lexer state begins from here */
	const TEXT* beginning;
	const TEXT* ptr;
	const TEXT* end;
	const TEXT* last_token;
	const TEXT* line_start;
	const TEXT* last_token_bk;
	const TEXT* line_start_bk;
	SSHORT	lines, att_charset;
	SSHORT	lines_bk;
	int  prev_keyword, prev_prev_keyword;
	USHORT	param_number;
	/* Fields to handle FIRST/SKIP as non-reserved keywords */
	bool limit_clause; /* We are inside of limit clause. Need to detect SKIP after FIRST */
	bool first_detection; /* Detect FIRST unconditionally */
	/* Fields to handle INSERTING/UPDATING/DELETING as non-reserved keywords */
	bool brace_analysis; /* When this is true lexer is informed not to swallow braces around INSERTING/UPDATING/DELETING */
	
	int yylex (
		USHORT	client_dialect,
		USHORT	db_dialect,
		USHORT	parser_version,
		bool* stmt_ambiguous);
};

/* Get ready for thread-safety. Move this to BISON object pointer when we 
   switch to generating "pure" reenterant parser. */
static LexerState lex;

#define ACTIVE 257
#define ADD 258
#define AFTER 259
#define ALL 260
#define ALTER 261
#define AND 262
#define ANY 263
#define AS 264
#define ASC 265
#define AT 266
#define AVG 267
#define AUTO 268
#define BEFORE 269
#define BEGIN 270
#define BETWEEN 271
#define BLOB 272
#define BY 273
#define CAST 274
#define CHARACTER 275
#define CHECK 276
#define COLLATE 277
#define COMMA 278
#define COMMIT 279
#define COMMITTED 280
#define COMPUTED 281
#define CONCATENATE 282
#define CONDITIONAL 283
#define CONSTRAINT 284
#define CONTAINING 285
#define COUNT 286
#define CREATE 287
#define CSTRING 288
#define CURRENT 289
#define CURSOR 290
#define DATABASE 291
#define DATE 292
#define DB_KEY 293
#define KW_DEBUG 294
#define DECIMAL 295
#define DECLARE 296
#define DEFAULT 297
#define KW_DELETE 298
#define DESC 299
#define DISTINCT 300
#define DO 301
#define DOMAIN 302
#define DROP 303
#define ELSE 304
#define END 305
#define ENTRY_POINT 306
#define EQL 307
#define ESCAPE 308
#define EXCEPTION 309
#define EXECUTE 310
#define EXISTS 311
#define EXIT 312
#define EXTERNAL 313
#define FILTER 314
#define FOR 315
#define FOREIGN 316
#define FROM 317
#define FULL 318
#define FUNCTION 319
#define GDSCODE 320
#define GEQ 321
#define GENERATOR 322
#define GEN_ID 323
#define GRANT 324
#define GROUP 325
#define GTR 326
#define HAVING 327
#define IF 328
#define KW_IN 329
#define INACTIVE 330
#define INNER 331
#define INPUT_TYPE 332
#define INDEX 333
#define INSERT 334
#define INTEGER 335
#define INTO 336
#define IS 337
#define ISOLATION 338
#define JOIN 339
#define KEY 340
#define KW_CHAR 341
#define KW_DEC 342
#define KW_DOUBLE 343
#define KW_FILE 344
#define KW_FLOAT 345
#define KW_INT 346
#define KW_LONG 347
#define KW_NULL 348
#define KW_NUMERIC 349
#define KW_UPPER 350
#define KW_VALUE 351
#define LENGTH 352
#define LPAREN 353
#define LEFT 354
#define LEQ 355
#define LEVEL 356
#define LIKE 357
#define LSS 358
#define MANUAL 359
#define MAXIMUM 360
#define MAX_SEGMENT 361
#define MERGE 362
#define MESSAGE 363
#define MINIMUM 364
#define MODULE_NAME 365
#define NAMES 366
#define NATIONAL 367
#define NATURAL 368
#define NCHAR 369
#define NEQ 370
#define NO 371
#define NOT 372
#define NOT_GTR 373
#define NOT_LSS 374
#define OF 375
#define ON 376
#define ONLY 377
#define OPTION 378
#define OR 379
#define ORDER 380
#define OUTER 381
#define OUTPUT_TYPE 382
#define OVERFLOW 383
#define PAGE 384
#define PAGES 385
#define KW_PAGE_SIZE 386
#define PARAMETER 387
#define PASSWORD 388
#define PLAN 389
#define POSITION 390
#define POST_EVENT 391
#define PRECISION 392
#define PRIMARY 393
#define PRIVILEGES 394
#define PROCEDURE 395
#define PROTECTED 396
#define READ 397
#define REAL 398
#define REFERENCES 399
#define RESERVING 400
#define RETAIN 401
#define RETURNING_VALUES 402
#define RETURNS 403
#define REVOKE 404
#define RIGHT 405
#define RPAREN 406
#define ROLLBACK 407
#define SEGMENT 408
#define SELECT 409
#define SET 410
#define SHADOW 411
#define KW_SHARED 412
#define SINGULAR 413
#define KW_SIZE 414
#define SMALLINT 415
#define SNAPSHOT 416
#define SOME 417
#define SORT 418
#define SQLCODE 419
#define STABILITY 420
#define STARTING 421
#define STATISTICS 422
#define SUB_TYPE 423
#define SUSPEND 424
#define SUM 425
#define TABLE 426
#define THEN 427
#define TO 428
#define TRANSACTION 429
#define TRIGGER 430
#define UNCOMMITTED 431
#define UNION 432
#define UNIQUE 433
#define UPDATE 434
#define USER 435
#define VALUES 436
#define VARCHAR 437
#define VARIABLE 438
#define VARYING 439
#define VERSION 440
#define VIEW 441
#define WAIT 442
#define WHEN 443
#define WHERE 444
#define WHILE 445
#define WITH 446
#define WORK 447
#define WRITE 448
#define FLOAT_NUMBER 449
#define NUMBER 450
#define NUMERIC 451
#define SYMBOL 452
#define STRING 453
#define INTRODUCER 454
#define ACTION 455
#define ADMIN 456
#define CASCADE 457
#define FREE_IT 458
#define RESTRICT 459
#define ROLE 460
#define COLUMN 461
#define TYPE 462
#define EXTRACT 463
#define YEAR 464
#define MONTH 465
#define DAY 466
#define HOUR 467
#define MINUTE 468
#define SECOND 469
#define WEEKDAY 470
#define YEARDAY 471
#define TIME 472
#define TIMESTAMP 473
#define CURRENT_DATE 474
#define CURRENT_TIME 475
#define CURRENT_TIMESTAMP 476
#define NUMBER64BIT 477
#define SCALEDINT 478
#define CURRENT_USER 479
#define CURRENT_ROLE 480
#define KW_BREAK 481
#define SUBSTRING 482
#define RECREATE 483
#define KW_DESCRIPTOR 484
#define FIRST 485
#define SKIP 486
#define CURRENT_CONNECTION 487
#define CURRENT_TRANSACTION 488
#define BIGINT 489
#define CASE 490
#define NULLIF 491
#define COALESCE 492
#define USING 493
#define NULLS 494
#define LAST 495
#define ROW_COUNT 496
#define LOCK 497
#define SAVEPOINT 498
#define RELEASE 499
#define STATEMENT 500
#define LEAVE 501
#define INSERTING 502
#define UPDATING 503
#define DELETING 504
#define KW_INSERTING 505
#define KW_UPDATING 506
#define KW_DELETING 507
#define BACKUP 508
#define KW_DIFFERENCE 509
#define OPEN 510
#define CLOSE 511
#define FETCH 512
#define ROWS 513
#define BLOCK 514
#define IIF 515
#define SCALAR_ARRAY 516
#define CROSS 517
#define NEXT 518
#define SEQUENCE 519
#define RESTART 520
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    9,    9,    9,   23,   23,   22,   22,   22,
   32,   32,   27,   33,   33,   33,   33,   33,   26,   26,
   31,   31,   28,   15,   15,   15,   36,   36,   37,   37,
   25,   25,   38,   38,   38,   38,   40,   40,   40,   40,
   39,   39,   41,   41,   41,   29,   29,   46,   30,   30,
   47,   47,    6,   48,   48,   50,   55,   55,   55,   52,
   52,   52,   59,   59,   60,   62,   62,   62,   62,   53,
   53,   63,   63,   64,   64,   64,   64,   64,   49,   66,
   66,    5,   68,   68,   68,   68,   68,   68,   68,   68,
   68,   68,   68,   68,   13,   83,   83,   83,   14,   87,
   87,   70,   70,   73,   73,   73,   81,   96,   96,   96,
   97,   97,   98,   98,   99,   99,  103,  103,   80,  106,
  106,  108,  108,  109,  109,  112,  112,  113,  114,  114,
  115,  116,   78,   82,   79,  100,  100,  119,  120,  120,
  122,  122,  123,  123,  123,  123,  123,  121,  121,  124,
  124,  125,  125,  125,  104,  127,  128,  128,  129,  129,
  130,  130,  131,  131,  131,  102,  102,  102,   75,   85,
  132,  132,  132,  133,  133,  134,  134,  135,  135,  135,
  140,   92,   92,  137,  137,  110,  110,  105,  144,  145,
   61,  146,  146,  111,  111,  111,  111,  111,  111,  138,
  138,  153,  153,  154,  156,  156,  156,  156,  156,  136,
  155,  155,  160,  160,  160,  160,  162,  163,  164,  158,
  158,  159,  157,  157,  157,  157,  157,  165,  166,  167,
  167,  167,  167,   74,   84,   88,  172,  168,  168,  169,
  169,  173,  173,  175,  174,  174,  177,  176,  176,  176,
  170,  170,  178,  178,  179,  181,  181,  182,  180,  180,
  184,  184,  183,  186,  186,  171,  188,  188,  188,  189,
  189,  187,  187,  187,  187,  187,  187,  187,  187,  187,
  187,  187,  187,  187,  187,  187,  187,  187,  187,  187,
  187,  192,  192,  193,  194,  195,  196,  201,  202,  197,
  197,  198,  211,  199,  212,  206,  206,  206,  207,  207,
  207,  209,  209,  209,  209,  200,  208,  208,  203,  203,
  203,  210,  210,  190,  190,  217,  218,  218,  219,  219,
  219,  219,  204,  204,  204,  221,  223,  222,  224,   11,
   12,  225,  225,  226,  226,  227,   77,   86,  142,  231,
   93,   95,  230,  230,   76,   89,  232,  232,  232,  233,
  236,  236,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  234,  234,  235,
    2,  239,  239,  239,  239,  239,  239,  239,  239,  248,
  249,  245,  245,  250,  250,  250,  250,  250,  250,  250,
  240,  240,  251,  251,  251,  251,  251,  251,  251,  244,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  253,  253,  255,  255,  254,  252,  252,  252,  246,
  246,  247,  247,  247,  242,  243,  243,  259,  259,  259,
  259,  259,  241,  260,  260,  261,  261,    8,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  262,  262,  262,
  262,  107,  107,  139,  139,  265,  265,  268,  268,  270,
  270,   56,   56,  267,  267,  267,  267,  267,  267,  267,
  267,  267,  274,  274,  266,  266,  266,  266,  276,  276,
  275,  275,  275,   58,   58,  271,  271,  271,  269,  269,
  269,  280,  280,  280,  279,  279,  278,  278,  278,  272,
  272,  281,  281,  281,  282,  282,  273,  273,  273,  273,
  283,  283,   19,   19,   19,  285,  285,  285,   17,   17,
   17,  287,  288,  291,  291,  289,  293,  293,    4,   16,
  292,  292,  294,  294,  295,  295,  284,  296,  296,  297,
  297,  298,  298,  298,  298,  299,  299,  300,  300,  301,
  301,  303,  303,  303,  304,  304,  304,  305,  305,  305,
  302,  307,  307,  307,  308,  308,  306,  306,  309,  311,
  311,  310,  310,  286,   18,  312,  312,  314,  314,  313,
  313,  229,  318,  319,  315,  315,  315,  320,  322,  331,
  332,  333,  334,  323,  323,  323,  323,  335,  335,  335,
  336,  336,  336,  324,  324,  325,  325,  338,  338,  339,
  339,  340,  340,  326,  342,  342,  343,  343,  345,  345,
  345,  347,  348,  348,  349,  349,  351,  351,  344,  344,
  344,  352,  353,  354,  356,  356,  357,  358,  346,  346,
  359,  359,  360,  360,   24,  355,  355,  355,  355,  355,
  361,  361,  328,  328,  362,  362,  363,  329,  329,  327,
  327,  330,  330,  364,  365,  365,  365,  365,  365,  366,
  366,  367,  367,  368,  368,  369,  369,  369,  370,  370,
  371,  371,  316,  316,  372,  372,  373,   71,   71,   71,
  375,  375,  374,  374,  317,  317,  317,   10,   10,    7,
    7,  377,  378,  379,   20,   20,  380,  381,  382,  382,
  191,  205,    3,    3,  385,  385,  385,  387,  387,  275,
  386,  386,  388,  388,   34,   34,   91,   90,   90,  376,
  376,  389,  390,  390,  215,  215,  215,  141,  383,  383,
  117,  117,  117,  117,  117,  394,  394,  394,  394,  394,
  394,  393,  393,  393,  396,  396,  396,  396,  396,  396,
  396,  396,  396,  396,  396,  397,  397,  397,  397,  397,
  397,  397,  397,  403,  403,  403,  403,  403,  403,  403,
  403,  403,  403,  403,  403,  403,  403,  403,  403,  408,
  408,  398,  398,  399,  399,  400,  400,  400,  400,  401,
  401,  405,  405,  406,  406,  406,  406,  404,  407,  402,
  402,  392,  392,  392,  395,  395,  395,  409,  409,  410,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,  152,  152,
  152,  411,  214,  214,  147,  147,  417,  417,  417,  417,
  417,  413,  413,  413,  413,  413,  228,  148,  148,  149,
  150,  150,  150,  150,  150,   54,   54,   21,   21,  238,
  418,   57,  277,  258,  258,  101,  412,  412,  412,  419,
  419,  419,  419,  419,  419,  419,  419,  419,  419,  419,
  420,  422,  421,  421,  424,  425,  425,  384,  384,  414,
  415,  415,  426,  426,  426,  427,  427,  428,  428,  431,
  431,  429,  429,  434,  434,  433,  430,  432,  416,  416,
  423,  423,  423,  423,  423,  423,  423,  423,  337,  337,
  321,  321,  151,   51,   67,  126,  143,  391,  161,  185,
  264,   69,   65,  220,  118,   72,  341,  216,   35,   44,
  350,  263,   42,   45,  213,   43,  290,  257,  257,  435,
  435,  435,  435,  435,  435,  435,  435,  435,  435,  435,
  435,  435,  435,  435,  435,  435,  435,  435,  435,  435,
  435,  435,  435,  435,  435,  435,  435,  435,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    8,    8,    5,    1,    0,    1,    2,    1,
    1,    3,    1,    1,    1,    1,    2,    2,    3,    0,
    3,    0,    1,    8,    8,    5,    3,    0,    3,    0,
    1,    1,    1,    3,    3,    3,    2,    2,    2,    2,
    1,    3,    1,    2,    2,    1,    3,    1,    1,    3,
    1,    2,    2,    2,    3,    8,    1,    1,    5,    0,
    1,    3,    1,    3,    3,    0,    2,    2,    1,    1,
    3,    3,    2,    0,    2,    2,    1,    3,    9,    1,
    1,    2,    3,    7,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    4,    2,
    2,    1,    0,    1,    1,    6,    6,    1,    1,    0,
    0,    1,    0,    4,    0,    1,    1,    2,    8,    1,
    0,    3,    0,    0,    1,    1,    2,    1,    1,    1,
    2,    6,    1,    1,    3,    0,    1,    1,    0,    1,
    1,    2,    3,    4,    2,    2,    3,    0,    1,    1,
    2,    1,    4,    3,    3,    1,    0,    1,    1,    2,
    3,    4,    0,    1,    2,    0,    1,    1,    5,    5,
    3,    2,    0,    1,    3,    1,    1,    6,    3,    2,
    6,    2,    1,    2,    2,    2,    0,    1,    1,    2,
    0,    2,    0,    1,    1,    1,    1,    1,    1,    0,
    1,    1,    2,    2,    2,    5,    1,    2,    3,    2,
    2,    0,    1,    1,    1,    1,    3,    4,    8,    4,
    0,    6,    1,    1,    2,    2,    0,    3,    3,    1,
    2,    2,    2,    8,    8,    8,    8,    3,    0,    4,
    0,    1,    3,    4,    1,    3,    2,    3,    3,    1,
    1,    0,    1,    2,    4,    1,    1,    3,    1,    0,
    2,    1,    6,    1,    1,    3,    1,    2,    0,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    2,
    1,    1,    2,    1,    1,    1,    2,    2,    1,    2,
    2,    3,    4,    2,    6,    4,    8,    9,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    7,    2,    0,    2,    2,
    3,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    1,    1,    1,    2,    2,    5,    0,    4,
    7,    3,    0,    1,    3,    3,    7,    7,    0,    0,
    0,    0,    3,    0,    9,    9,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    5,    5,    5,    5,    5,    5,    2,    0,    4,
    2,    3,    3,    2,    2,    3,    3,    2,    2,    3,
    6,    1,    2,    4,    3,    2,    2,    2,    2,    3,
    1,    3,    3,    3,    2,    2,    4,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    1,    1,    1,    0,    2,
    2,    4,    4,    5,    0,    1,    2,    2,    4,    3,
    2,    2,    7,    1,    0,    1,    0,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    3,    2,    2,    2,
    2,    1,    1,    1,    1,    4,    5,    1,    3,    1,
    3,    1,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    4,    4,    6,    5,    3,    0,
    2,    2,    0,    3,    0,    4,    1,    5,    4,    1,
    4,    1,    2,    2,    1,    1,    1,    2,    2,    2,
    2,    0,    3,    5,    1,    1,    2,    3,    1,    2,
    3,    0,    1,    1,    1,    5,    5,    6,    1,    1,
    1,    2,    4,    1,    0,    5,    1,    0,    3,    2,
    1,    0,    2,    0,    1,    0,    3,    1,    0,    1,
    2,    1,    1,    1,    1,    2,    2,    1,    2,    3,
    1,    1,    3,    3,    1,    2,    3,    1,    2,    0,
    2,    1,    1,    0,    1,    1,    1,    3,    2,    3,
    0,    1,    3,    4,    3,    3,    0,    2,    0,    2,
    0,    3,    3,    3,    1,    4,    4,    1,    9,    0,
    0,    0,    0,    3,    2,    1,    0,    3,    5,    3,
    2,    5,    2,    1,    1,    1,    1,    1,    3,    1,
    3,    1,    0,    2,    1,    3,    1,    1,    1,    1,
    3,    6,    1,    0,    3,    0,    1,    3,    1,    1,
    1,    4,    5,    5,    1,    1,    2,    4,    4,    2,
    3,    0,    1,    2,    1,    1,    2,    2,    2,    0,
    1,    0,    3,    0,    1,    3,    1,    2,    0,    2,
    0,    2,    0,    4,    1,    2,    1,    1,    0,    1,
    3,    2,    1,    1,    2,    1,    4,    3,    1,    3,
    4,    0,    3,    0,    1,    3,    3,    1,    1,    0,
    1,    1,    4,    0,    2,    4,    0,    8,    5,    1,
    1,    7,    4,    4,    1,    1,    8,    5,    1,    3,
    3,    1,    7,    7,    5,    3,    0,    1,    1,    1,
    2,    0,    1,    1,    1,    0,    3,    1,    3,    1,
    0,    3,    1,    3,    1,    3,    3,    1,    1,    3,
    1,    2,    1,    3,    3,    1,    2,    3,    3,    3,
    3,    1,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    5,    6,    3,    4,    5,    6,    3,
    4,    3,    4,    3,    4,    4,    5,    4,    4,    3,
    4,    1,    1,    1,    1,    1,    1,    1,    3,    3,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    3,    3,    3,    3,    3,    3,    3,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    1,    1,
    1,    4,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    1,    2,    1,
    1,    1,    1,    1,    2,    1,    1,    1,    1,    4,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,
    1,    6,    1,    4,    7,    2,    0,    4,    3,    6,
    1,    1,    6,    8,    6,    1,    1,    4,    6,    4,
    5,    3,    5,    4,    5,    1,    1,    1,    4,    6,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,
    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  563,  564,  565,  569,  570,  571,    0,  635,  638,
  750,  751,  755,  756,  475,    0,    0,    0,    0,    0,
    0,    0,  401,  581,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  122,    0,    0,    0,  102,    0,
  930,    0,   22,  928,    0,    0,   73,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  488,    0,    0,    0,   36,   33,   35,    0,   34,
    0, 1018, 1020, 1021, 1022, 1023, 1024, 1025, 1044, 1045,
 1026, 1028, 1027, 1033, 1029, 1034, 1030, 1032, 1035, 1031,
 1036, 1037, 1038, 1040, 1041, 1039, 1042, 1043, 1047, 1046,
 1048,    0,    0,    0,    0,   31,   68,   66, 1010, 1019,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  646,    0,    0,    0,  693, 1012,    0,    0,
    0,    0,    0,  115, 1017,  572,    0,    2,    0,    0,
    0,    0,    0,    0,  432,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,  460,  454,  455,  453,  456,
  457,  458,  459,  461,    0,  430,  431,    0, 1002,    0,
  408, 1006,    0,  405, 1009,    0,  695,    0,  404, 1013,
    0,  409, 1005,    0,  579,  926,    0,  158,  111,    0,
  112,    0,  208,  998,  788,    0,  109,  153,    0,    0,
  105,    0,  113,  932,    0,  106,    0,  107,    0,  108,
 1016,  154,  114,  110,  738,  739,    0,  931,  929,    0,
   74,    0, 1003,    0, 1001,  496,  489,    0,  495,  500,
  490,  491,  498,  492,  493,  494,  499,  501,    0,    0,
    0,   29,    0,   38,  775,   37,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  936,    0,  917,  640,  640,  641,  651,  653,  989,  654,
    0,  655,  645,  641,    0,    0,    0,    0,    0,    0,
    0,  598,  587,    0,  590,  592,  593,  594,  595,  601,
  602, 1011,  694,    0,    0,  116,    0,  117,    0,  118,
    0,    0,    0,  625,    0,    0,  991,    0,    0,  632,
    0,    0,    0,    0,    0,  476,    0,    0,  371,    0,
  211,    0,  412,  402,  470,  471,    0,    0,    0,    0,
    0,    0,  421,    0,  377,  378,    0,    0,  585,  583,
  927,    0,    0,    0,    0,    0,    0,    0,  161,  140,
    0,  103,    0,    0,  119,    0,  129,  128,    0,    0,
    0,    0,    0,    0,  994,   75,    0,    0,    0,    0,
  753,  497,    0,    0,    0,    0,  894,  923,    0,  993,
    0,  896,    0,    0,  924,    0,  918,  909,  908,  907,
    0,    0,    0,    0,  899,  900,  901,  910,  911,  919,
  920,    0,  921,  922,    0,    0,    0,  925,    0,    0,
    0,    0,    0,    0,    0,  913,    0,  785,  891,  892,
  893,  898,  897,  360,  876,    0,    0,  875,    0,    0,
  880,  872,  873,  874,  877,  878,  879,  912,  937,  938,
  939,  951,  953,  961,  962,  966,  967,  209,    0,    0,
    0,  364,    0,    0,    0,  778,   26,    0,    0,    0,
    0,   71,   69, 1014,   67,   32,    0,    0,    0,  780,
    0,   47,   49,    0,    0,    0,  577,    0,    0,    0,
  648,  650,    0,  657,    0,    0,    0,  658,  644,    0,
  624,    0,  599,    0,  596,    0,  597,  622,    0,  617,
    0,    0,  591,  789,  759,    0,    0,    0,    0,    0,
    0,  574,  573,    0,  626,  630,    0,    0,  735,  637,
  636,    0,  176,    0,    0,  137,    0,  481,    0,  482,
  477,    0,    0,  416,  418,  417,    0,  419,    0,  413,
    0,    0,  262,    0,    0,    0,  425,  426,    0,  463,
    0,    0,    0,    0,    0,    0,    0,  382,  381,  484,
    0,    0,    0,  157,    0,    0,  166,    0,  165,    0,
    0,  172,  155,    0,  170,  162,    0,    0,  520,  555,
  523,    0,  556,    0,    0,  524,    0,    0,    0,  547,
  559,  519,  542,  521,  522,  517,  504,  371,  502,  503,
  505,    0,    0,  514,  515,  516,  518,    0,    0,    0,
    0,    0,  120,    0,  121,    0,  132,    0,    0,  192,
    0,    0,  369,    0,  211,    0,    0,   83,    0,  101,
    0,  100,  995,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  791,  793,  802,  805,  806,  807,  808,
  809,  810,  811,  812,  813,  814,  815,    0,    0,    0,
    0,    0,  914,    0,    0,    0,    0,    0,    0,  915,
  916,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  325, 1015,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  267,  512,
    0,    0,    0,  362,    0,    0,    0,  777,    0,    0,
   43,   72,    0,    0,   25,    0,  783,    0,    0,  749,
    0,    0,    0,    0,  576,    0,  640,    0,  662,    0,
    0,    0,    0,  567,    0,  934,  566,    0,  600,    0,
  608,  604,  603,    0,    0,    0,  619,  607,    0,    0,
    0,  758,    0,    0,    0,  369,    0,    0,    0,    0,
    0,  138,    0,  480,  371,  415,    0,  371,  372,  420,
    0,    0,  258,  369,  231,  999,    0,    0,  371,    0,
    0,  200,  369,    0,    0,    0,    0,  236,  230,  233,
  234,  235,  424,  468,  467,  423,  422,    0,    0,    0,
    0,    0,    0,  380,    0,  371,  473,    0,  472,    0,
  163,  167,    0,    0,  171,    0,    0,  770,    0,  543,
  544,  560,    0,  557,    0,    0,  550,  548,  549,    0,
    0,    0,    0,  513,    0,    0,    0,    0,  551,    0,
    0,  369,    0,  191,    0,  194,  196,  197,    0,    0,
    0,    0,    0,  211,   78,    0,    0,   77,    0,    0,
    0,    0,  792,  804,    0,    0,  865,  866,  867,    0,
  796,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  715,  717,    0,  712,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  981,  982,  983,  984,  985,  986,  987,  988,    0,    0,
    0,    0,    0,    0,  972,    0,    0,    0,    0,    0,
  889,  327,    0,  890,  959,    0,  885,  997,    0,    0,
    0,    0,    0,    0,    0,  895,  787,  786,  366,  365,
    0,  265,    0,    0,    0,  273,  779,    0,    0,    0,
   70,    0,    0,    0,  782,    0,    0,    0,    0,  649,
  652, 1007,  661,    0,    0,    0,    0,  667,  668,  669,
  670,  679,  680,  681,    0,    0,  659,  568,  935,  609,
  618,  613,  612,    0,  623,  790,  760,    0,    0,  369,
    0,    0,  642,  737,  736,    0,  479,    0,    0,  175,
    0,  179,    0,    0,    0,  414,  369,  369,  270,  372,
  263,    0,  202,  371,  204,  371,  372,  199,  205,    0,
    0,    0,    0,  427,  428,  371,  369,  372,    0,    0,
    0,  398,    0,  474,  187,  188,  164,    0,  174,  531,
  532,  933,    0,    0,    0,    0,    0,  558,    0,    0,
  372,    0,    0,  508,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  189,    0,    0,    0,
   82,    0,    0,    0,    0,   90,   84,    0,    0,   89,
   85,    0,  754, 1000,    0,    0,  797,    0,    0,  803,
    0,    0,    0,    0,    0,  841,  840,    0,    0,    0,
  850,  868,    0,  860,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  795,    0,  716,    0,  752,    0,
    0,  211,    0,  940,    0,    0,  954,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  968,
    0,    0,    0,    0,    0,    0,    0,  979,  634,  958,
  902,    0,  260,  279,    0,    0,  361,  274,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   53,   61,   63,
    0,   41,    0,    0,    0,  764,  784,    0,  763,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  696,    0,
    0,    0,    0,    0,    0,    0,  615,  616,  620,    0,
    0,  190,    0,    0,    0,    0,    0,  180,    0,    0,
    0,  410,  214,  215,  216,  217,  218,  219,  905,    0,
    0,  264,    0,    0,    0,    0,    0,    0,    0,  237,
    0,  464,  465,  429,    0,    0,    0,    0,    0,    0,
  371,  486,  372,  173,  996,    0,    0,  526,    0,  525,
  561,    0,  553,    0,    0,    0,  506,  534,    0,    0,
  536,  539,  541,  369,    0,    0,    0,  127,    0,  195,
  371,    0,  370,  104,    0,  125,    0,   93,    0,    0,
    0,    0,   87,   88,    0,  858,  859,  799,    0,  801,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  861,
    0,    0,    0,    0,    0,    0,    0,  851,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  723,    0,    0,    0,  946,  945,    0,
    0,  942,  941,    0,  950,  949,  948,  947,  944,  943,
    0,    0,  974,    0,    0,    0,  973,    0,    0,    0,
    0,  266,    0,    0,  276,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  359,
    0,    0,    0,    0,  285,  290,  284,    0,    0,    0,
  294,  295,  296,  297,  298,  299,  301,  302,  304,  305,
  306,  309,    0,    0,    0,    0,  353,  354,  355,    0,
  762,   65,   57,   58,   64,   59,   60,    0,   23,    0,
    0,   24,  769,  768,    0,  766,  774,  773,  771,  748,
   44,   45,    0,  671,    0,    0,    0,  701,  699,  697,
    0,  698,    0,    0,    0,    0,    0,  757,    0,  370,
  741,  742,  643,    0,  185,  181,    0,  372,  906,  268,
  269,  372,    0,  142,    0,    0,  222,    0,    0,  238,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  483,
  528,    0,  529,    0,    0,    0,    0,  146,  148,  149,
  150,  511,  509,  507,  538,    0,    0,  372,    0,    0,
    0,  367,  371,   91,    0,    0,   97,   92,    0,    0,
    0,    0,    0,  869,    0,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  725,  714,    0,    0,
  726,    0,  722,  210,  960,  980,  952,    0,    0,  970,
  969,    0,  975,  963,  965,    0,    0,  275,    0,  314,
    0,    0,    0,  308,    0,    0,  307,  339,  340,    0,
 1008,  356,  357,    0,  293,  300,    0,  303,  286,    0,
  291,    0,  344,  292,  310,  311,    0,    0,  337,    0,
   54,   55,   56,   62,    0,    0,  691,  689,    0,  682,
    0,    0,    0,  705,    0,  639,  372,  368,  743,  182,
  372,  152,  257,    0,    0,  198,  223,    0,    0,    0,
    0,  224,  227,    0,    0,  372,  396,  397,  393,  392,
  395,  394,    0,  527,  554,  151,  139,  147,    0,  371,
  254,  134,  372,  373,    0,    0,   95,    0,   79,    0,
  827,  835,  633,    0,  828,  836,  831,  839,    0,    0,
  829,  837,  830,  838,  824,  832,  825,  833,  826,  834,
  721,    0,    0,    0,  955,  971,    0,    0,    0,  282,
  278,    0,  312,    0,    0,    0,    0,    0,  341,    0,
    0,  332,  333,    0,  352,    0,    0,    0,    0,  347,
  345,    0,    0,    0,   39,  765,    0,  673,  683,    0,
    0,  684,  685,  686,    0,  255,  411,  201,  206,  225,
    0,    0,  228,    0,  240,  242,  400,  372,    0,  375,
    0,   76,   98,   99,    0,    0,    0,  728,  964,  212,
  281,    0,  313,    0,    0,  316,    0,  322,    0,  324,
    0,    0,  351,  350, 1004,  349,    0,    0,    0,    0,
    0,    0,  672,    0,    0,  706,  229,    0,    0,    0,
    0,    0,  256,  372,    0,    0,  727,    0,    0,    0,
    0,    0,    0,    0,  335,  334,  346,  348,    0,    0,
    0,  677,    0,    0,    0,    0,    0,  239,    0,  246,
    0,  245,  376,  126,  730,    0,  283,    0,    0,  315,
  319,    0,    0,    0,    0,    0,    0,  675,  688,  226,
    0,    0,  250,  249,  248,  731,    0,    0,    0,    0,
    0,  336,  678,  253,  251,  252,  330,  320,    0,  342,
  317,  318,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1451,   28,   29, 1452,
   31,   32,   33,   34,   35,   36,   37, 1453,   39, 1454,
  868,  142,  508,  167, 1245, 1489,  143,  760,  144,  511,
  765,  145,  146,  294, 1025,  156,  157, 1246, 1247, 1248,
 1249,  228,  259,  147, 1250,  148,  513,   87,  271,  416,
  465,  676, 1134,  466,  907,  647,  252,  884,  677,  678,
  679, 1141, 1136, 1578,  272,  681,  682,   79,  218,   80,
  267, 1805, 1354,  251,  256,  258,  260,  247,  239,  241,
  253,  263,  174,  346,  348,  350,  405,  663,  665,  505,
  295,  828,  583,  692, 1066,  409,  668, 1125, 1348,  615,
  786, 1097,  575,  576,  596,  401,  648, 1077, 1556, 1686,
 1292, 1557, 1558, 1559, 1560, 1561,  693,  248,  240,  397,
  623,  398,  399,  624,  625, 1324,  577, 1060, 1061, 1062,
 1287,  411,  895,  896,  897,  898,  830, 1535,  649,  832,
  468,  900,  987,  499, 1411, 1750, 1293,  469,  470,  471,
  472,  473, 1536, 1537,  599, 1692, 1840, 1310,  838,  839,
  825,  840,  841,  842, 1841, 1842, 1894,  378,  504, 1004,
 1455,  224,  592, 1001,  593, 1070,  500, 1005, 1006, 1235,
 1434, 1435, 1436, 1751, 1143, 1456, 1457, 1458, 1459, 1652,
 1460, 1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468, 1469,
 1470, 1471, 1472, 1473, 1474,  474, 1851, 1475, 1761, 1885,
 1758,  475,  737,  476,  477, 1476, 1653, 1769, 1770, 1824,
 1477, 1478, 1479, 1644,  291,  501,  502,  478,   41, 1353,
 1572,  387,  610,  856, 1322,  611,  854,   84,   63,  382,
  229,  184,  365,  215,  372,  221,  232,  819,  816,  373,
  383,  846,  384,  606, 1088,  216,  479, 1112,  366,  612,
 1323,  102,  227,  276,  650,  651,  750, 1113,  751, 1114,
  654,  655,  656,  657, 1494, 1106, 1104,  658,  659,  660,
  877,  661,  874,   42,   43,   44,   45,   46,   47,  176,
  563,   65,  528,  235,  390,  333,  334,  335,  336,  337,
  338,  339,  340,  341,  792,  549, 1044, 1279,  550,  551,
  797,  180,  354,  565,   48,  183,  360, 1377,  736,   49,
  358,   50,  161,  321,  536,  782,  420, 1276, 1517,  709,
  531,  323, 1284, 1679,  162,  163,  322,  537,  538,  780,
 1023, 1026, 1027, 1028, 1029, 1030, 1031, 1777, 1833,  480,
 1863, 1032, 1033, 1034, 1274, 1782, 1783, 1784, 1266,  170,
 1509, 1673, 1674, 1404,  945, 1405, 1406, 1407, 1613, 1806,
 1808,  568,  569, 1054, 1523,  519,   51,   52,  421,   53,
   54,  557,  558,  481, 1013, 1256, 1495, 1499,  520,  768,
  245,  694,  695,  922,  923,  696,  697,  698,  699,  700,
  701,  702,  703,  704,  705,  706,  707, 1159, 1161, 1162,
  482,  483,  484,  485,  486,  487,  488,  269,  489,  490,
  491,  492,  969,  493, 1619,  494,  495,  496,  497,  725,
  973, 1223, 1218,  726,  150,
};
static short yysindex[] = {                                   5106,
 2278, -148, 2420,   77, 1058,  -12, 2817, -108,11612,  481,
  104,  -28, -148,  978,  497,13363,  899,13363,   -7,    0,
  702,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  370,    0,    0,    0,    0,    0,    0,   22,    0,    0,
    0,    0,    0,    0,    0,16701,13363,13363,13363,13363,
13363,13363,    0,    0,  438, 1173,13363,13363,13363,  581,
13363,  409,13363,13363,    0,13363,13363,13363,    0, 1099,
    0,  415,    0,    0,  577,13363,    0,13363,13363,13363,
  613,13363,13363,13363,13363,  409,13363,13363,13363,13363,
13363,    0,13363,  938,  683,    0,    0,    0, 1013,    0,
 1013,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  762,  784,  153, 1171,    0,    0,    0,    0,    0,
13363,13363,13363,  839,  860, 1300,13363,  842,  161,  197,
   40,  824,    0,13363, 1062, 1765,    0,    0,13363,  992,
13363,13363,13363,    0,    0,    0,13363,    0,  975, 1049,
 1148,  798, 1019, 1252,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  956,    0,    0, 1173,    0,   -5,
    0,    0, 1497,    0,    0, 1218,    0,   37,    0,    0,
 1021,    0,    0, 1195,    0,    0, 1097,    0,    0,  951,
    0, 1298,    0,    0,    0, 1173,    0,    0,  608, 1497,
    0,  302,    0,    0, 1276,    0, 1317,    0, 1013,    0,
    0,    0,    0,    0,    0,    0, 1304,    0,    0, 1198,
    0, 1313,    0, 1210,    0,    0,    0, 1198,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,14965,13363,
 1268,    0,13363,    0,    0,    0, 1235, 1270,12372,13363,
 1167, 1342, 1633, 1372, 1380, 1391, 1307, 1335,  113, 1206,
    0,15219,    0,    0,    0,    0,    0,    0,    0,    0,
12997,    0,    0,    0, 1289,13363, 1371, 1302,  688,13363,
 1316,    0,    0, 1765,    0,    0,    0,    0,    0,    0,
    0,    0,    0,13363, 1497,    0, 1276,    0, 1013,    0,
 1373, 1379, 1265,    0,15219, 1365,    0, 1365,15219,    0,
  -38, 1273, 1274, 1294, 1252,    0, 1516,  717,    0,13363,
    0,  956,    0,    0,    0,    0,13363, 1268,11361, 1351,
12135, 1773,    0,16701,    0,    0, 1336, 1386,    0,    0,
    0, 1758, 1758, 1173, 1468, 1173,   47,  951,    0,    0,
 7096,    0,13363,13363,    0, 1268,    0,    0, 1557,  565,
 1812,13363, 1584,13363,    0,    0, 1820,10317, 9442, 1475,
    0,    0, 1829, 1846, 1862, 1453,    0,    0, 1868,    0,
 1872,    0, 1878, 1896,    0, 1902,    0,    0,    0,    0,
    0, 1915, 1477, 1490,    0,    0,    0,    0,    0,    0,
    0, 1916,    0,    0,13251, 1917, 1919,    0, 1927, 1545,
15219,15219,13489,13363, 1929,    0, 2556,    0,    0,    0,
    0,    0,    0,    0,    0, 1906, 1883,    0,    0, 1931,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7096, 1900,
 1381,    0, 1935, 1714, 1455,    0,    0,13363,13363,13363,
  105,    0,    0,    0,    0,    0,13363,13363,  580,    0,
13363,    0,    0, 1235, 1585,12372,    0,13363,13489, 1651,
    0,    0,15219,    0, 1694, 1664, 1941,    0,    0,  217,
    0,  995,    0,  243,    0,  243,    0,    0, 1942,    0,
  168, 1572,    0,    0,    0, 1950,   69, 1944, 1268, 1963,
 1742,    0,    0,13363,    0,    0, 1606, 1964,    0,    0,
    0, 1154,    0, 1663, 1665,    0, 1173,    0, 1666,    0,
    0, 1739, 1741,    0,    0,    0, 1721,    0, 7096,    0,
 7096, 1625,    0, 1762,13363,16534,    0,    0,  287,    0,
13363, -127, 1218, 1637, 1609, 1577,    0,    0,    0,    0,
  591, 1650,  247,    0, 1591,  409,    0, 1173,    0, 1767,
 1701,    0,    0,   47,    0,    0,  231, 1610,    0,    0,
    0, 1612,    0, 1661, 2016,    0, 1716, 2019,  606,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1969,  152,    0,    0,    0,    0,   96, 2022, 2023,
 2019, 1497,    0, 1751,    0, 1805,    0, 1173, 1173,    0,
11361,   37,    0, 1695,    0, 1669, 2034,    0, 5473,    0,
 1702,    0,    0, 1708, 2046, 9680, 2054,    0,    0,    0,
 9188, 6959,   20,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  773, 1719,  845,
15219,  491,    0,13363,15219, 1229, 1258, 1309, 1761,    0,
    0,15219, 9934, 2556, 1652,  -32,15219,15219, 9934, 1781,
  881,  881, 1671, 1647,   22, 2060,    0,    0,13743,13363,
15219,15219,15219,15219,15219,15219,15219, 9554,    0,    0,
 1832, 2041,13363,    0,13363, 1813,13363,    0, 1680, 1687,
    0,    0, 1667,12372,    0, 1803,    0, 1656, 2079,    0,
 1803,13363,13363, 2077,    0, 1671,    0, 1683,    0,13363,
 9316, 1678,15219,    0,  626,    0,    0,   17,    0, 1684,
    0,    0,    0,13363,  844,13363,    0,    0,13363,13363,
 1475,    0,15219, 1861,11361,    0, 2082, 1641,15219,15219,
 1173,    0,  384,    0,    0,    0, 2091,    0,    0,    0,
  208,13363,    0,    0,    0,    0, 1864, 2099,    0, 1845,
 1867,    0,    0, 1809, 1811, 1013, 1876,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  409,13363,16632,
 1766, 1774, 1776,    0,  409,    0,    0,  752,    0, 1337,
    0,    0, 1748, 1173,    0,10317,   72,    0, 1755,    0,
    0,    0,  409,    0, 2016,   87,    0,    0,    0, 1845,
   87, 1749,   87,    0, 2120,  409,  409,  409,    0, 1268,
13363,    0, 1814,    0, 1699,    0,    0,    0, 1336, 1365,
13363, 1715,   89,    0,    0, 2129,   25,    0,10317,13363,
 1365,10172,    0,    0, 1365, 9680,    0,    0,    0, 6339,
    0,  218,  569,15219,15219,10433, 2130,  665,10694,15219,
10955, 1149,11216,11477,13981,11738,11999,12260, 9934, 9934,
    0,    0, 1823,    0, 2142, 1019,15219,15219, 1856,15219,
 2126,15219, 2145, 1808,15219,15219,15219,15219,15219,15219,
    0,    0,    0,    0,    0,    0,    0,    0, 1873, 1402,
  353,15219,    3,15219,    0, 9934, 1752, 2229,  607,13363,
    0,    0, 1019,    0,    0, 1732,    0,    0, 1918,  881,
  881,  836,  836, 2556,  912,    0,    0,    0,    0,    0,
 1760,    0, 1754, 1932, 1813,    0,    0,12871,12871, 1827,
    0,  309, 1838,13363,    0,15219, 1838, 1884, 1892,    0,
    0,    0,    0, 1573, 2172, 2169, 1541,    0,    0,    0,
    0,    0,    0,    0, 9934, 1894,    0,    0,    0,    0,
    0,    0,    0,  280,    0,    0,    0, 1719, 2556,    0,
 1789, 1365,    0,    0,    0, 2556,    0, 1758, 1951,    0,
  384,    0, 2181, 9934, 5209,    0,    0,    0,    0,    0,
    0, 1813,    0,    0,    0,    0,    0,    0,    0, 1013,
 1013, 1729, 2183,    0,    0,    0,    0,    0,   43,   51,
  528,    0, 1971,    0,    0,    0,    0,13363,    0,    0,
    0,    0,   77, 1797, 1831, 1832, 2202,    0, 1591, 1801,
    0, 2188, 1193,    0,13363, 1254,  409, 2209, 2215, 2216,
 2001,   37, 1813, 1758, 1665,11361,    0, 1650, 1842, 4291,
    0,  409, 1880, 1980, 5473,    0,    0,  409,  246,    0,
    0, 1989,    0,    0, 2234, 2236,    0, 9934, 9934,    0,
 9934, 9934, 2437, 2556, 2257,    0,    0, 2556, 2259,13489,
    0,    0, 1984,    0,  973, 2262, 2556, 2264, 1257, 2265,
 2556, 2273,15219,15219, 2130,15219,14235, 2280, 2556, 2284,
 2293, 2556, 2295,15219, 2556, 2308, 2556, 2315, 2317, 2556,
 2319, 2322, 2556, 2325,    0, 2056,    0,13164,    0, 1921,
 2005,    0, 2069,    0, 2087,15219,    0, 2132, 2219, 2251,
 2267, 2274, 2285,15219,15219,15219, 2556, 1913,15219,    0,
15219, 2556, 2061,  561,15219,15219,15219,    0,    0,    0,
    0,13363,    0,    0,13363,16343,    0,    0,13363,13363,
13363,13363,13363,13363, 1938, 2323, 2331,    0,    0,    0,
 1938,    0,  347,  347,   75,    0,    0, 1817,    0,12871,
12871, 2337, 1541, 2345,15219, 2125, 9316, 2009,    0, 2009,
  783, 2009, 2052, 2062, 2133, 2073,    0,    0,    0, 1019,
 1813,    0, 1842, 1135, 1591, 2021, 1591,    0, 9934,  359,
 1338,    0,    0,    0,    0,    0,    0,    0,    0, 5209,
 5209,    0, 1932,15219, 5209, 2123, 2011, 1729, 1099,    0,
 9934,    0,    0,    0, 2033, 2036, 2038, 2039, 2043, 2047,
    0,    0,    0,    0,    0, 2372,   77,    0, 1977,    0,
    0,   87,    0, 2057,   87,   87,    0,    0, 1832, 2391,
    0,    0,    0,    0, 1336, 1932, 1591,    0, 1665,    0,
    0, 2165,    0,    0, 2082,    0, 2403,    0, 2404, 1173,
  -34, 2405,    0,    0, 1173,    0,    0,    0, 2056,    0,
 2056,15219, 1365, 1365, 1840,   22, 2407,15219, 2134,    0,
 1365, 1365,15219, 1365, 1365, 2487, 2556,    0, 1586,15219,
 2556, 1365, 1365, 1365, 1365, 2556, 1365, 1365, 1365, 1365,
 1365, 1365,13363,    0, 2409, 2411, 1211,    0,    0, 7096,
 2412,    0,    0, 2311,    0,    0,    0,    0,    0,    0,
 2378, 1470,    0,15219, 2151, 2042,    0,15219, 2392, 1841,
 2300,    0, 7096, 2413,    0,    0, 2184,    0,10830,  304,
 2414, 2431, 2147,15219, 2426, 2427,11091,13363,13363,    0,
 2428, 2432, 2154, 2435,    0,    0,    0, 2190,16205, 2438,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2439, 2441,  -85, 2446,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2186,    0,12871,
12871,    0,    0,    0, 2084,    0,    0,    0,    0,    0,
    0,    0, 2125,    0, 1863,13363, 1541,    0,    0,    0,
 2166,    0, 9316, 9316,15219, 9934, 1475,    0, 1932,    0,
    0,    0,    0, 1337,    0,    0,  373,    0,    0,    0,
    0,    0, 2556,    0, 2237, 2123,    0,  662,13363,    0,
 2180,  424, 2081, 2187, 2083, 2221, 2189, 2224, 1813,    0,
    0, 2484,    0, 2486, 2192, 2237, 2057,    0,    0,    0,
    0,    0,    0,    0,    0, 1813, 1650,    0, 1337, 1971,
 2153,    0,    0,    0, 2171, -115,    0,    0, 1832, 2173,
 2556, 2500, 2501,    0, 1019,    0, 2556,15219, 2502, 2513,
 2556, 2517, 2520,15219,15219, 2556, 2522, 2525, 2533, 2535,
 2537, 2540, 2544, 2545, 2550, 2551,    0,    0,13164, 2506,
    0,13363,    0,    0,    0,    0,    0,15219, 2555,    0,
    0,15219,    0,    0,    0,15219,  214,    0, 2287,    0,
14473,13363,15219,    0, 9934, 2556,    0,    0,    0, 2541,
    0,    0,    0,13363,    0,    0,10578,    0,    0,  664,
    0, 2175,    0,    0,    0,    0,   97, 2569,    0, 2241,
    0,    0,    0,    0,  347,13363,    0,    0, 9316,    0,
 2307, 2556, 2568,    0,   20,    0,    0,    0,    0,    0,
    0,    0,    0, 2579,13363,    0,    0, 2275, 2294,13363,
 1729,    0,    0, 1013,13363,    0,    0,    0,    0,    0,
    0,    0, 1932,    0,    0,    0,    0,    0, 1932,    0,
    0,    0,    0,    0,15219, 1173,    0, 2177,    0, 1173,
    0,    0,    0, 2556,    0,    0,    0,    0, 2556, 2556,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,13363, 2291, 2556,    0,    0, 2423, 5209, 5209,    0,
    0, 2596,    0, 2505,14965, 1500,  473, 2578,    0, 2303,
 1389,    0,    0, 2594,    0,13363,13363,   77,  117,    0,
    0, 2141, 2306, 9934,    0,    0, 2603,    0,    0, 9934,
 2604,    0,    0,    0,15219,    0,    0,    0,    0,    0,
 1729, 1013,    0, 2269,    0,    0,    0,    0, 1971,    0,
 2556,    0,    0,    0, 2602, 2606, 2608,    0,    0,    0,
    0, 1365,    0, 2247,10578,    0, 2223,    0,10578,    0,
10578, 9808,    0,    0,    0,    0,16343,  664,15219,10578,
  725,13363,    0,   20,13363,    0,    0, 2269,   58, 1729,
 2277, 2281,    0,    0, 2610,13363,    0,13363, 2613, 4591,
 2597, 1465,16343, 2618,    0,    0,    0,    0, 1428,  226,
 2362,    0, 1869, 1890, 1729,  539,  539,    0, 2366,    0,
 2231,    0,    0,    0,    0, 2625,    0,10578, 2618,    0,
    0, 2363,10578, 2380, 2367,16343,13363,    0,    0,    0,
 2217,  897,    0,    0,    0,    0, 1891,16343,  174,13363,
16343,    0,    0,    0,    0,    0,    0,    0,16343,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  288,  955,    0,    0,    0,    0,    0,    0,    0,
    0,11873,  220,12498,    0,    0,    0,    0,    0,    0,
 2671,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  187,    0,    0,    0,    0,    0,    0,  137,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  917,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2343,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    1, 2301,    0,    0,    0,  112,    0,
  112,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2309,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1084,    0,    0,
14727,12759,    0,    0,    0, 1123,    0,    0,  195,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  825,
    0, 2270,  838,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   79,    0,    0,    0,    0,  743,    0,    0,
    0,    0,    0, 1134,    0,    0,    0,    0,    0,  100,
    0, 8872,    0,    0,    0,    0,    0,    0,    0,   79,
    0,  451,    0,    0, 2644,    0,    0,    0, 2424,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  127,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1165,    0,
 2425,    0,    0,    0,    0,    0,13617,    0,    0,    0,
    0,    0,  846,    0,    0,    0,    0,    0,    0,13855,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  963,    0,    0, 1222,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   79,    0, 2644,    0, 2424,    0,
 1247,  192,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1285,    0, 2415,    0,    0,    0,
    0, 1287,    0,    0,    0,    0,    0, 2425,  659,16770,
    0, 1292,    0,    0,    0,    0,  408,    0,    0,    0,
    0, 2242, 2242,    0,    0,    0, 1328,  135,    0,    0,
    0,    0,    0,    0,    0, 2425,    0,    0, 1278,    0,
    0,    0,    0,    0,    0,    0,12755,    0,    0,  123,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1879,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2716,    0,    0,    0, 3016, 3316,    0, 3616, 3916,
    0,    0,    0,    0,    0,    0,  933,    0,    0,    0,
    0,    0,    0,    0,    0,  286, 4216,    0, 2416,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1330,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,13617,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,10046,    0, 2377,    0,    0,    0,
    0,    0,    0,  204,    0,  204,    0,    0, 1035,    0,
 1091, 1152,    0,    0,    0,    0,  127,    0, 2425,    0,
    0,    0,    0,    0,    0,    0,  166,  342,    0,    0,
    0,  871,    0,    0, 1111,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2415,    0,
    0, 1107,    0, 2244,    0,    0,  700,    0,    0,    0,
    0,  331,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1356,    0,    0, 2127,15712,    0,    0,
    0,15755,    0,    0,15923,    0,    0,16021,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1821, 3155,    0,    0,    0,    0,16091,15769,    0,
16021,   79,    0,    0,    0,    0,    0,    0,    0,    0,
  659, 1403,    0,    0,    0,    0, 2310,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6829, 7078, 7224,
    0,    0,  106,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1915,  114,15457,
    0,15457,    0,    0,    0,15457,15457,15457,    0,    0,
    0,    0,    0, 2253,    0,    0,    0,    0,    0,    0,
 5405, 5694, 2659,    0,   66,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1487,    0,    0,    0,    0, 2440,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  221,    0,    0,    0,    0,
  221,    0,    0, 1367,    0,    0,    0,    0,    0,    0,
    0,  628,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  870,    0,    0,    0,    0,    0,
  123,    0,    0,    0,  659,    0,  244,  295,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,    0,
 1903,    0,    0,    0,    0,    0, 2669,    0,    0,  382,
 1707,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  504,  856,  893,    0,    0,    0,    0,    0,    0,  945,
    0,    0,    0,    0,    0,    0,    0,    0, 2707,    0,
    0,    0,    0,    0,15923,    0,    0,    0,    0,  158,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2425,
    0,    0,  274,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 5762,    0,    0,    0,  279,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1902,    0,    0, 1368,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2677,    0,    0,    0,    0,    0, 4516, 5983,
 6272, 4816, 5116,  950,    0,    0,    0,    0,    0,    0,
    0,    0,14109,    0, 2453,    0,    0,    0,    0,    0,
    0,    0, 1378,    0,    0,    0, 1378,    0,    0,    0,
    0,    0,    0,    0, 2113,  883,  209,    0,    0,    0,
    0,    0,    0,    0,    0,  337,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  114,  205,    0,
    0,    0,    0,    0,    0,  903,    0, 2242, 2276,    0,
  617,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2440,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1382,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 3007,    0,    0,    0,    0,
    0, 1366,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1403, 2440, 2242, 1383,  659,    0, 2461, 1397,    0,
    0,    0, 5762,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1236,    0,    0,    0, 7303,    0,    0,
    0,    0,    0,    0,    0,    0, 7375,    0, 7459,    0,
 7531,    0,    0,    0,    0,    0,    0,    0, 7614,    0,
    0, 7698,    0,    0, 7744,    0, 7828,    0,    0, 7874,
    0,    0, 8032,    0,    0, 8937,    0, 1915,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2305,    0,    0,    0,
    0,   54,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  509,    0,    0,    0,    0,
    0,    0,    0,  188, 1407,  120,  154,    0,    0,    0,
 1407,    0, 2299,  245,    0,    0,    0,    0,    0,    0,
    0,    0, 2389, 2185,    0, 6579,    0, 2396,    0, 2396,
 2389, 2396,    0,    0,    0,  877,    0,    0,    0, 1368,
 2440,    0, 1397,    0,    0, 2286,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1580,    0, 1146, 2343,    0,
    0,    0,    0,    0,  998, 1010, 1011, 1027, 1056, 1068,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  811,    0,    0,    0,    0, 3307,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1424,    0,
    0,    0,    0,    0, 1443,    0,    0,    0,    0,    0,
  143,    0,    0,    0,    0,    0,    0,    0,  110,    0,
  150,    0,    0,    0,    0,   66,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8104,    0, 8179,    0,
 8239,    0,    0,    0,    0, 8273,    0,    0,    0,    0,
    0,    0, 1238,    0,    0, 2689,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2697,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6880,    0,    0,
    0,    0,    0,    0,    0, 1527, 1538,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  538,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1561,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6479,    0,    0,    0,  549,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  130,    0,    0,    0,
    0,    0,    0,  827,    0,    0,    0,    0,    0,    0,
    0,    0, 2698,    0, 1249, 1870,    0, 2415,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2440,    0,
    0,    0,    0,    0,    0, 1452,  878,    0,    0,    0,
    0,    0,    0,    0,    0, 2440, 2461,    0,  379,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  858,    0,
 8389,    0,    0,    0, 2677,    0, 8473,    0,    0,    0,
 8572,    0,    0,    0,    0, 8667,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1915,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2681,    0,    0,    0,
    0,    0,    0,    0,    0, 2685,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2445,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  245, 1023,    0,    0,    0,    0,
 2389,  670,  832,    0,  880,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  872,    0,    0,  139,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  162,    0,    0,
    0,    0,    0, 8695,    0,    0,    0,    0, 8815, 8909,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1910, 2705,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   91,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 9032,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  872, 1081,    0,  170,    0,    0,    0,    0,    0,    0,
 2698,    0,    0,    0, 2713,    0,    0,    0,    0,    0,
    0,    0,    0, 2706,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   -2,    0,    0,    0,
    0,    0,    0, 9060,    0,    0,    0, 1184,    0, 1146,
 1209, 1494,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   -2, 2708,    0,    0,    0,    0,    0, 2455,
    0,    0,    0,    0,  872,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2715,    0,
    0,16107,    0,    0,    0,   -2,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   -2,    0,    0,
   -2,    0,    0,    0,    0,    0,    0,    0,   -2,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2772,    0,    0, 2780,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2781,
   -3, 2627, 2261,  -47, -810, 1540, 2637, 2025, 2645, 2268,
    0,    0, 2503, -107,  -40,    0,    0,    0,    0,  385,
  426,  -63,  -76,  -59, -270, 2508, 2037,    0,    0,    0,
 1230,    0,    0,  -46, 1668, -575,  -66,-1011, 2131, 1907,
 -334,    0, 1677,    0, 2720, 1904, 1948,    0,  -65,    0,
 -533,  -48,    0,    0,    0,    0,    0, 2738,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -556,
   -1, 1690, -297, 1064, -903,    0,    0,    0,    0, -351,
 -143,-1223, 1692, -369,  -62,    0, -564, 1943,    0, 1266,
-1074,    0, 1269,    0,    0, 2458, -605,  -23,    0,    0,
    0,    0, 2429,    0, 2204, 1717,    0,    0,    0, 1768,
    0, 2483, 2029, 1709, 2457, 2460,    0,    0, -463, 2010,
  -55, -631, 1155, -268,    0,    0,    0, -991, -989, -948,
 -920, -912,    0, 1306,-1072,    0, 1007,-1252, 1314,    0,
 2246,    0,    0,    0, 1009, 1014,  987, -104, -266, -856,
 -917,    0,    0,    0, 2035,    0, -654,    0, 1851,    0,
    0,    0,    0,    0,-1191, -919,    0,    0,    0,    0,
 -236,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1103,    0,    0,  361,    0,
    0, -508,    0, -420, -175, 1416, 1212,    0, 1037,    0,
    0,    0,    0,    0,    0,    0, 2114, -145, -453, 1583,
 1349, -569, -778, -994,-1208,    0,    0,  -39,    0,    0,
    0,    0,    0, 2488,    0,    0,    0,    0,    0, 2498,
 2279,    0,    0,    0,    0,    0,   -9, -502, 2512,    0,
    0,    0,    5,    0,    0,    0, -377, 1996, -374, 1547,
    0,    0,    0,    0, 2254,    0, -471,    0,    0,    0,
 2227,    0, 2014,    0,    0,    0,    0,    0,    0,   -8,
    0, 2871,    0,    0,    0,    0,    0, 2557,    0,    0,
    0,    0, 2348,    0, 2339,    0,    0,    0, 2098,    0,
    0,    0,    0,    0, -353, -664, -864, 1569,    0,  140,
    0,    0,    0,    0,    0,    0, -342,    0,    0, -761,
 -216, 1262,    0,    0,    0, 2732, 1051,    0, 2112,-1043,
-1020,    0, -949, 1875,-1247,    0,    0,    0,    0, -163,
    0,    0,    0,    0, 1630,    0,    0,    0,    0, 2808,
 -312,    0, 1117, 2195,    0, 1295,    0, 1502,    0, -210,
    0,    0, 2097,    0,    0,    0,    0,    0, 2355,    0,
    0,    0, -457,-1182, 2143, 1898,-1139,    0,    0,    0,
 -726, -438,  260,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  891, 1746,    0,
    0,    0, -888,    0,    0,    0, 1631,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -732, 1705,    0,    0,
};
#define YYTABLESIZE 17290
static short yytable[] = {                                     149,
   83,   38,  246,  296,  242,  343,  168,  807,  175,  220,
  257,  243,  226,  315,  318,  314,  317,  262,  223,  238,
  169,  998,  286,  652,  277,  255,  653,  622,  512,  283,
  250,  829,  254,  808,  285,  749,  589,  787,  231, 1048,
  287,  616,  734, 1437,  946,  281,  217,  219,  222,  225,
  168,  230,  233, 1481,  282, 1540,  254,  244,  219,  233,
  767,  225,  289,  168,  230,  770,  261,  149,  233,  280,
  983,  587, 1046, 1294, 1263, 1295,  273,  288,  168,  275,
  219, 1199,  273,  233,  222,  225, 1237,  168,  230,  261,
  149,  233,  169,  225, 1330,  302,  349,  304,  532,  159,
 1002,  284,  899,  908,  303,  710,  734,  555,  591,  735,
  859,  594,  800,  734, 1496, 1103, 1296,  971, 1229,   51,
 1128,   82,  713,  979,  347,  820,  711,  821, 1133,  713,
  345, 1109,  831, 1351,  160,  886,  734,  313,  776,  666,
  325,  244,  168,  244, 1297,  406,  710,  149,  764,  328,
  798,  413, 1298,   52,  233,  776,  300,  143,  159,  342,
 1828,  225,  168,  261,  710,  740, 1302,  175,  351,  247,
  713,  374,  734, 1306, 1052,  735, 1299,  734,   51,  776,
  556,  713,  776,   94, 1314,  711,  627, 1024,  713, 1069,
  800,  629, 1072,  160,  695,  734,  300,  776, 1251,  402,
  312, 1079,   96,  610,  761,  812,  740, 1334,  665,  740,
  247,  796,   52,  247,  801, 1303,  143, 1821,  652,  582,
  767,  653, 1506,  313,  740, 1530, 1531,  627,  247, 1657,
 1534, 1024,  629, 1538,  498, 1717,  316,  506, 1576,  762,
  559,  561,  883,  628,  533,  627, 1024,  913,  761,  665,
  629,  375,  665,  695,  622,  512, 1642, 1643, 1150,  313,
 1123,  785,  610,  761,  363, 1670, 1346,  665, 1068, 1821,
  867,  974,  975,  133, 1749,   82, 1481,  541,  582,  767,
  244,  939,  548,  244,  628,  326,  103,  582,  554,  514,
  149,  858,  804,  385,  744,  154,  544, 1139,   64,  319,
 1680,  837,  628,  533,   88,  573, 1219, 1220, 1294, 1294,
 1295, 1295,  338, 1294,  588, 1295,  222, 1507,  986,   86,
  168,  498,   86,  243,  376,  602,  995, 1564,  604,  844,
  399,  845,  133, 1195, 1196,  744,  704,  735,  744,  320,
  664,  733,  259,  620,  326, 1712,  582,  617, 1317,  619,
  880, 1296, 1296,  744,  556, 1866, 1296,  978,  978, 1658,
  244, 1713,  662,  670,  672,  674,  386,  244, 1718,  244,
 1224,  244, 1140, 1110,  607,  153, 1315,  704,  186, 1297,
 1297,  143,  733, 1280, 1297, 1532, 1086, 1298, 1298,  399,
  573,   82, 1298,  225,  230,  704,  159,  177,  940, 1528,
  733,  181,  168,  363,  222,  104, 1772,  485,  683,  313,
  976, 1299, 1299, 1681,  680, 1518, 1299, 1827, 1281, 1550,
  710, 1779,  143, 1577, 1519,  143,  882,  155, 1568,  526,
  710,  160,  710,  143,  143, 1300, 1301,  186, 1793, 1036,
  143,  710,  338,  159,  713, 1221, 1129,  546,   94, 1501,
 1502,  734, 1760,  182,  738, 1313,  177, 1145, 1290, 1666,
  759, 1146,  554, 1538, 1696,  713,  485,   96,  761,  766,
  574,  860,  734,  771, 1909, 1132, 1316, 1147,  160, 1148,
  740,  259,  795, 1423, 1318,  710, 1425,  776,  798, 1884,
  177, 1867,  328,  512,  710,  570,  978,  571,  168,  225,
  514,  740,  713,  385, 1067,   14,  711,  168,  506,  713,
 1748,  168,  419, 1817,  776,  711,  514, 1063,  175,  775,
 1065, 1102,  627,  665, 1102, 1776,   81,  629,  800,  143,
  813, 1075,  951,  665,  885,  665,  311,  710, 1837, 1651,
  833,  610, 1368, 1369,  665, 1370, 1371,  700,  666,  861,
  763,  710, 1345,  591,  244,  621, 1257,  890, 1093,  908,
 1020,  713,  385, 1047, 1671,   51, 1567, 1719, 1135,  407,
 1262,  862, 1710, 1355,  695,  713,  254, 1432,  734,  628,
  299,  767,  734,  695,  761,  826,  244, 1868,  665,  666,
 1844,  826,  666,  761,  399, 1258, 1149,  665, 1283,   52,
  610, 1677,  834,  610,  695,  533,  999,  666,  159,  744,
  311,  740, 1890,  790,  939,  243,  178,  133,  710,  610,
  939,  893,  894, 1121, 1682, 1253,  734,  711, 1683, 1684,
  744,  776,  627, 1024,  939,  713,  556,  629,  695,  711,
  665, 1039,  713,  160, 1481,  610,  311,  582,  761,  734,
 1227,  704,  665,  866,  665,  177,  733,  143,  143,  740,
  408,  244,  247,  704, 1711,  143,  311,  177,  711,  707,
 1481,  485,  704, 1810, 1811,  178, 1277,  733,  740,  835,
   81,   86,  791, 1527,  179,  939,  711,  326,  582,  628,
  953, 1620, 1703,  784,  177, 1623,  311,  498, 1632,  498,
  177, 1007,  177, 1481,  233, 1542, 1285,  695, 1910, 1709,
  707, 1585, 1566,  707, 1039, 1481,  704,  761, 1481,  836,
 1723,  665,  186,  857, 1018,  704, 1481, 1278,  707, 1363,
  988,  940,  761,  130,  939, 1058, 1254,  940,  244, 1375,
  744,  177,  379,  244,  554,  244,  548,  244, 1045,  243,
  319,  940,  151,  143,  514, 1676, 1294, 1294, 1295, 1295,
  178, 1364,  168,  225, 1057, 1861,  498,  385,  704,  866,
 1022,  225, 1347, 1786,  143, 1541, 1304, 1787, 1305, 1216,
  143, 1084,  704, 1498,  168, 1797,  168,  733, 1312,  244,
  950, 1798, 1796, 1085, 1087,  244,   81,  485, 1135, 1296,
 1296,  379,  940, 1633, 1059, 1430, 1376,  744,  254, 1800,
  144, 1862,  244,  289,  143, 1092,  152, 1099,  164, 1118,
 1119, 1120,  939,  338,  631, 1319,  186, 1297, 1297, 1554,
 1151,  703, 1562, 1107, 1082, 1298, 1298,  747,  234,  244,
  244,  249,  287, 1122, 1505, 1614,  254,  254,  254,  704,
  556,  940,  338, 1130,  733,  383,  683, 1553,   81, 1299,
 1299, 1320, 1100,  666,  268,  631, 1903, 1410,  939,  144,
  745,  241,  703,  666,  178,  666,  709,  145,  747,  708,
  878,  168,  664,  631,  666,  186,  178,  700,  851, 1746,
  703,  168,  384,  385, 1843,  270,  747, 1845,  535,  683,
 1144,  535,  746,  130,  130,  680,  177, 1857,  669, 1891,
 1675,  745,  241,  178,  383,  241,  584,  709,  165,  178,
  708,  178,  744,  664,  852,  166, 1765,  745,  666,  745,
  241,  278,  903, 1882,  232,  709,  145,  666,  708,  940,
 1873,  664,  711,  746,  186,  914,  879, 1152, 1892,  904,
  921,  384,  711,  338,  711,  746, 1228, 1510,  554, 1512,
  178,  746,  605,  711, 1163, 1039, 1902,  544, 1482, 1627,
  233, 1485, 1766,  903,  232,  584,  903,  290, 1908,  812,
  666, 1911,  338, 1767,  707,  940,  939, 1428,   14, 1912,
  904,  903,  666,  904,  666, 1893,  707,  390,  514,  514,
  585,  379,  403,  186, 1231,  707,  379,  711,  904,  391,
  387,  379, 1164,  586,  225,  769,  711,  236,  237, 1376,
 1376,  605,  674, 1549,  853,  903,  386, 1376, 1376, 1757,
 1376, 1376,  652, 1688,  611,  653, 1165,  404, 1376, 1376,
 1376, 1376,  904, 1376, 1376, 1376, 1376, 1376, 1376,  707,
 1340,  232,  293, 1570, 1689,  389,  390,  356,  707,  711,
 1690,  666,  674,  674,  545, 1358,  674,  388,  391,  387,
  243, 1362,  556,  711,  506,  311,  292,  254, 1307, 1308,
  776,  674, 1768,  580,  186,  386,  371,  144, 1325,  998,
  621,  232,  254,  611, 1691,  998,  186,  357,  254, 1326,
 1268,  707, 1038,  940,  319, 1325,  469, 1493, 1493, 1497,
  478,  941,  740, 1269,  389,  707,  244,  741,  546,  383,
  244,  776,  589,  186,  776,  178,  388,  431, 1356,  186,
  535,  186,  379,  586,  942,  547, 1270,  297, 1762,  776,
  711, 1524,  580, 1526,  947,  241,  703,  241,  241,  621,
  469,  606,  747,  371,  145,  241,  384,  740,  703,  298,
  631,  998,  741,  535,  328,  469, 1486,  703, 1831,  478,
  186,  914, 1433,  747, 1834,  914,  498, 1484,  186,  243,
  554,  589,  707,  247, 1487,  745,  241, 1272,  168,  241,
  943,  709,  586, 1905,  708,  744,  742,  664,  743, 1483,
  745,  311, 1403, 1569,  241,  535,  745,  664,  243,  664,
  606,  703,  709,  367,  301,  708,  305,  746,  664,  123,
  703,  588,  244,  328,  247, 1438, 1480,  247, 1094,  514,
  225,  230,  514,  261,  149,  852, 1336,  306,  746, 1042,
  837,  186,  247,  241, 1906,  383,  575,  186,  207,  243,
  514,  514,  243,  123,  781, 1043,  709,  225,  368,  708,
  535,  390,  664,  703,  241,  709,  614,  243,  708,  310,
  241,  664, 1379,  391,  387, 1715,  852,  703, 1864,  852,
  588,  781,  384,  747,  406, 1337,  407,  123,  186,  207,
  386,  403,  207,  171,  852,  556,  186, 1336,  744,  742,
  605,  743,  392,  745,  241,  575, 1762,  207,  709,  160,
 1762,  708, 1855, 1575,  664,  535,  745,  614, 1580,  389,
 1380, 1762,  709, 1552,  172,  708,  664,  168,  664,   42,
  186,  388,  186,  605,  903,  186,  393,  674,  394,  173,
  674, 1762, 1668,  406,  703,  407, 1339,  674,  746,  674,
  403,  904,  467,  674,  186,  169,  776,  776,  674,  605,
  395,  674,  605,  265,  776,  369,   46,  747,  478, 1762,
   85,   86,  611, 1631, 1762,  530,  674,  772,  605,  186,
  478,  487,  135,  370,  535,  396,  168,  390,   42,  709,
  674,  788,  708,  168,  326,  664,  374,  266,  674,  391,
  387,  344,  674,  554,  605,  611,   40, 1403,  352,  510,
  331,  674, 1799,  478,  169,  478,  386,  371,  567, 1173,
  355,  754,  572,  136,  753,   46,  747,  674,  621,  219,
  740,  611, 1821, 1174,  611,  741,  772, 1641, 1144, 1144,
  487,  135,  124,  744,  742,  389,  743, 1820,  745, 1480,
  611,  207,  776,  186,  674,  374,  776,  388,  510,  247,
  247,  621,  159,  160,  106,   40,  674,  247,  674,  744,
  742, 1763,  743,  776,  745,  379,  611, 1175,  380,  776,
  514,  514,  136, 1764,  243,  243,  535,  621,  319,  606,
  621, 1694,  243,  244,  353,  758,  342,  852,  757,  417,
  108,  124, 1778,  225,  225, 1176,  621,  422, 1821,  361,
  207,  744,  742,  776,  743,  674,  745,  319,  724, 1493,
  381,  362,  606, 1881,  731,  732,  733,  535,  955,  168,
  535,  359,  621,  740,  244,  674,  377,  244,  741,  674,
  388,  744,  742, 1610,  743,  535,  745,  535,  606,  391,
  852,  606,  244,  852,  363,  247,  364,  957, 1816,  105,
  852,  400,  852, 1743, 1383,  109,  852,  606,  319, 1177,
  724,  852, 1026,  776,  852,  110,  247,  533, 1611,  220,
  243,  810,  247, 1031, 1026,  539, 1794, 1026,  410,  852,
 1612, 1755,  776,  606,  608, 1031,  778,  106, 1031,  168,
  111,  243,  222,  852,  609,  724, 1011,  243,  959,  107,
  389,  852, 1024, 1403,  852,  852,  247,  724, 1008, 1521,
  220,  998,  225,  220,  852,  236,  237,  744,  742, 1522,
  743,  412,  745,  108, 1144, 1875,  414, 1876,  220, 1763,
  852,  243, 1792, 1763,  418, 1856, 1795,  744,  742,  415,
  743, 1764,  745,  419, 1763, 1764,  342, 1764, 1773,  225,
  507,  379,  852,  556,  509,  823, 1764,  852,  822, 1802,
  503,  379,  518, 1804, 1763,  988,  247,  517,  740,  852,
  168,  852,  524,  741, 1838,  222, 1764,  982,  521,  556,
  746,  777,  744,  742,  522,  743, 1015,  745,  109, 1014,
 1823,  243, 1763,  527,  740,  523,  502, 1763,  110,  741,
  525,  981,  744,  742, 1764,  743,  540,  745, 1215, 1764,
 1095, 1096,  556, 1021,  744,  742,  542,  743,  852,  745,
  131,  131,  222,  111,  556,  744,  742,  556,  743, 1127,
  745,  552, 1126,  543,  535,  556,  740,  502,  852,  562,
  502,  741,  852,  564,  920, 1131,  219, 1825,  904,  535,
  948,  566,  952, 1883, 1826,  502,  956,  958,  960,  244,
  244,  554, 1230,   14,  949,  746,  740,  244,  954,  506,
  578,  741,  579,  535, 1618,  970,  438,  439,  440,  535,
  977,  978,  535,  744,  742, 1225,  743,  554,  745,  582,
 1233,  580,  467, 1232,  989,  990,  991,  992,  993,  994,
  467,  600,  244, 1849,  448,  449,  603, 1480,  614, 1168,
  512, 1172, 1022, 1180, 1183,  244, 1188, 1191, 1194, 1282,
  554,  613, 1126,  618,  535, 1815,  222, 1328,  222,  667,
 1327, 1333,  554, 1480, 1332,  554,  535,  673, 1207,  744,
  742,  671,  743,  554,  745,  232,  220, 1500, 1268,  675,
  746,  512,  740,  708,  512,  244, 1049,  741,  710,  221,
  265, 1269,  567, 1056, 1661, 1663, 1480, 1022, 1018,  512,
 1584, 1625,  740,  746,  746,  711,  244,  741, 1480,  535,
 1144, 1480,  244, 1595, 1270,  730,  535,  744,  742, 1480,
  743,  712,  745, 1667,  266,  713,  746,  714, 1271, 1888,
  221,  715, 1887,  221,  535, 1662, 1664,  716,  994, 1018,
 1018, 1018, 1018, 1018, 1018, 1018,  244,  740,  221,  720,
 1889, 1907,  741,  757, 1821,  717, 1018, 1018, 1018, 1018,
 1018,  718,  721,  369,  535, 1272,  369,  740,  535,  746,
  732,  232,  741,  732,  719,  722,  727,  779,  728,  740,
  752, 1408,  744,  742,  741,  743,  729,  745,  739, 1018,
  740, 1018,  232,  747,  755,  741,  748,  756,  232,  773,
  781,   14,  502,  502,  783,  794,  244, 1153, 1154, 1158,
  502,  798, 1167, 1169, 1171,  799, 1179, 1182, 1185, 1187,
 1190, 1193,  805,  502,  803,  806,  811,  809,  573,  814,
 1200, 1201,  232, 1203,  815, 1205,  817,  818, 1208, 1209,
 1210, 1211, 1212, 1213,  112,  824,  848,  113,  740,  114,
  115,  116,  117,  741,  118, 1217,  849, 1222,  850,  855,
  311,  863,  119,  120,  864, 1409,  744,  742,  870,  743,
  871,  745,  872,  121,  122,  873,  123, 1273,  876,  881,
  875,  887,  888,  124,  125,  891,  126,  127,  892,  128,
  901,  903,  129,  130,  131,  132,  133,  904,  502,  467,
  134,  135,  910,  909,  740,  911,  136,  137,  138,  741,
  139,  140,  141,  915,  972,  980,  512,  512,  181,  502,
  984,  512,  327,  313,  512,  502,  882, 1008, 1003, 1412,
  744,  742,  692,  743, 1009,  745, 1012,  512, 1016, 1202,
  764, 1035, 1010, 1040, 1050,  757,  533, 1413,  744,  742,
 1064,  743,  740,  745, 1053,  328, 1073,  741, 1074,  502,
 1018, 1076, 1018, 1018, 1089,  232,  221,  827, 1080, 1018,
 1081, 1083, 1090,  692, 1091, 1018,  692, 1098, 1115, 1117,
 1018,  329, 1105, 1018,  330, 1124, 1204,  533, 1138, 1160,
  533,  692, 1415,  744,  742, 1852,  743, 1018,  745, 1854,
  331, 1198, 1018, 1018, 1197,  533, 1018,  533, 1206, 1214,
 1860, 1234,  512, 1018,  740, 1018, 1018,  740, 1255, 1018,
 1260, 1236,  741, 1018, 1252, 1018,  332, 1018, 1261, 1018,
 1879, 1265, 1267,  512, 1018, 1018, 1286, 1018, 1275,  512,
 1289, 1309, 1311,  467,  961,  962,  963,  964,  965,  966,
  967,  968, 1018, 1018, 1321, 1018, 1386, 1387, 1897, 1389,
 1391,  232, 1331, 1899, 1329, 1335, 1018, 1396, 1018, 1341,
 1018, 1018, 1018,  512, 1018, 1342, 1343, 1018, 1018, 1416,
  744,  742,  232,  743, 1344,  745, 1132, 1018,  232, 1414,
  744,  742, 1226,  743, 1366,  745, 1367, 1421, 1422, 1222,
 1018,  740, 1222, 1018, 1217, 1360,  741, 1352, 1429,  467,
 1431, 1417,  744,  742, 1365,  743, 1373,  745, 1374, 1018,
 1378, 1381,  232, 1382, 1384, 1018, 1018, 1418,  744,  742,
 1018,  743, 1385,  745, 1419,  744,  742,  939,  743, 1392,
  745, 1018, 1018, 1393, 1018, 1420,  744,  742,  467,  743,
 1018,  745, 1394, 1018, 1395, 1018, 1018, 1018, 1018, 1424,
 1018,  744,  742, 1626,  743,  740,  745, 1397, 1018, 1018,
  741, 1616,  744,  742, 1398,  743, 1399,  745, 1400, 1018,
 1018, 1401, 1018,  740, 1402, 1427, 1490, 1533,  741, 1018,
 1018, 1018, 1018, 1018, 1491, 1018,  692, 1503, 1018, 1018,
 1018, 1018, 1018, 1488,  533, 1504, 1018, 1018,  779, 1508,
 1513, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1516,
 1514,  533,  533,  533, 1525, 1515,  595,  533,  740, 1539,
  533, 1543, 1551,  741, 1544,  998, 1545, 1546, 1617,  744,
  742, 1547,  743,  533,  745, 1548, 1102,  692, 1555,  533,
  692, 1565, 1624,  744,  742, 1581,  743,  692,  745,  692,
 1571, 1587, 1573,  692, 1574, 1579, 1591, 1586,  692, 1608,
 1588,  692, 1615, 1596, 1609, 1621,  998,  998,  998,  998,
  998, 1011,  998, 1809,  744,  742,  692,  743, 1622,  745,
 1635, 1628, 1634, 1629,  998,  998,  998,  998,  744,  742,
  692,  743,  152,  745, 1637, 1638, 1645, 1222,  692, 1647,
 1646, 1222,  692, 1648, 1649,  740, 1654, 1655,  533, 1656,
  741,  692,  667, 1659, 1669,  740,  998, 1636,  998, 1660,
  741, 1665, 1695, 1685, 1697,  667, 1699,  692, 1700,  533,
 1698, 1702, 1701,  667, 1704,  533, 1705,  740,  744,  742,
 1714,  743,  741,  745,  533, 1716,  533, 1720,  667, 1706,
 1721, 1722, 1725,  740,  692, 1742,  744,  742,  741,  743,
  740,  745,  667, 1726,  533,  741,  692, 1727,  692,  533,
 1728,  740, 1731, 1813,  692, 1732,  741,  692,   55,  692,
  692,  692,  692, 1733,  692, 1734,  740, 1735, 1672,   56,
 1736,  741,  692,  692, 1737, 1738,   57,  740,  533,  667,
 1739, 1740,  741,  692,  692, 1745,  692,  744,  742, 1759,
  743, 1752,  745,  692,  692,  692,  692,  692, 1774,  692,
   58, 1785,  692,  692,  692,  692,  692, 1650, 1775, 1788,
  692,  692, 1790, 1807, 1268,  692,  692,  692,  692,  692,
  692,  692,  692, 1791, 1803, 1812, 1818, 1269, 1819, 1822,
 1829, 1830, 1832, 1835, 1839, 1846, 1847, 1848, 1850, 1853,
 1874, 1724, 1869, 1877,  740, 1880, 1871, 1729, 1730,  741,
 1270, 1821, 1886, 1866, 1867, 1896, 1898, 1901,  740, 1900,
    1, 1904,   59,  741, 1271,  740,   28,  998,  992,  998,
  998, 1744, 1780,  193,   30, 1222,  998,  776,  261, 1747,
  371,  156,  998,  656, 1754,  977, 1756,  998, 1372,  740,
  998,  667,  903,   60,  741,  466,  530,   61,  203,  272,
   66, 1272,   81,  740,  998, 1028,  998,  747,  741,  998,
  998,   67,  271,  998,  399,  183,  533,  700,   68,  720,
  998,  976,  998,  998,  702,  184,  998,  957,  372,  213,
  998,   69,  998,  323,  998,  956,  998,  530, 1594,  288,
  530,  998,  998,  729,  998,  343, 1028, 1028, 1028, 1028,
 1028, 1028, 1028,  740,  331,  530,  358,  530,  741,  998,
  998,   27,  998,  329, 1028, 1028, 1028, 1028, 1801,   30,
   40,  740,  307,  998,  772,  998,  741,  998,  998,  998,
 1492,  998,  308,  774,  998,  998,   62, 1019,   70, 1781,
 1011,  309, 1361,  516,  998,  902, 1028,  515, 1028, 1359,
 1137,  279, 1142, 1101,   71,  264, 1349,  998,  467, 1357,
  998, 1707, 1111, 1273,  584, 1708,  626,  865, 1288,  560,
   72, 1338,  740, 1051, 1350,  597,  998,  741,  598, 1789,
 1078, 1687,  998,  998, 1865,   73,  843,  998, 1672,   74,
 1872, 1693,   75, 1895, 1870, 1238, 1071, 1814,  998,  998,
   76,  998, 1640, 1771, 1858, 1520, 1000,  998, 1678,  590,
  998,  605,  998,  998,  998,  998,  581,  998, 1116,   77,
  869,  847, 1563,  158,  793,  998,  998,  889, 1108,  789,
  553, 1041, 1859,  324, 1037,  274,  998,  998, 1264,  998,
 1511, 1836,  944, 1741, 1607, 1055,  998,  998,  998,  998,
  998,  802,  998, 1017, 1259,  998,  998,  998,  998,  998,
 1388, 1529,    0,  998,  998, 1426,    0,    0,  998,  998,
  998,  998,  998,  998,  998,  998,    0,    0,   78,    0,
    0, 1582, 1583,    0,    0,    0,    0,    0,    0, 1589,
 1590,    0, 1592, 1593,    0,    0,    0,    0,    0,    0,
 1597, 1598, 1599, 1600,  530, 1601, 1602, 1603, 1604, 1605,
 1606,    0,    0,    0,    0,    0,    0, 1028,    0, 1028,
 1028,  530,  530,  530,    0,    0, 1028,  530,    0,    0,
  530,    0, 1028,    0,    0,    0,    0, 1028,    0,    0,
 1028,    0,    0,  530,    0,    0,  535,    0,    0,  530,
    0,    0,    0,    0, 1028, 1033,    0,    0,    0, 1028,
 1028,    0,    0, 1028,    0,    0,    0,    0,    0,    0,
 1028,    0, 1028, 1028,    0,    0, 1028,    0,    0,    0,
 1028,    0, 1028,    0, 1028,    0, 1028,  535,    0,    0,
  535, 1028, 1028,    0, 1028,    0, 1033, 1033, 1033, 1033,
 1033, 1033, 1033,    0,    0,  535,    0,  535,    0, 1028,
 1028,    0, 1028,    0, 1033, 1033, 1033, 1033,  530,    0,
    0,    0,    0, 1028,    0, 1028,    0, 1028, 1028, 1028,
    0, 1028,    0,    0, 1028, 1028,    0,    0,    0,  530,
    0,    0,    0,    0, 1028,  530, 1033,    0, 1033,    0,
    0,    0,    0,    0,    0,    0,  530, 1028,   89,    0,
 1028,    0,    0,    0,    0,   90,    0,    0,    0,   91,
   92,    0,    0,    0,  530,    0, 1028,    0,   93,  530,
    0,    0, 1028, 1028,    0,    0,    0, 1028,    0,   94,
    0,    0,    0,    0,  535,    0,    0,    0, 1028, 1028,
    0, 1028,    0,    0,    0,    0,    0, 1028,  530,    0,
 1028,    0, 1028, 1028, 1028, 1028,    0, 1028,    0,    0,
    0,    0,    0,    0,    0, 1028, 1028,    0,    0,    0,
    0,    0,    0,    0,    0,  535, 1028, 1028,  535, 1028,
    0,    0,    0,    0,    0,    0, 1028, 1028, 1028, 1028,
 1028,   95, 1028,  535,    0, 1028, 1028, 1028, 1028, 1028,
    0,    0,    0, 1028, 1028,    0,    0,   96, 1028, 1028,
 1028, 1028, 1028, 1028, 1028, 1028,    0,    0,    0,    0,
    0,    0,   97,    0,    0,    0,   98,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   99,    0,    0,
    0,    0,    0,    0,  535,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  100, 1033,    0, 1033,
 1033,    0,  535,  535,    0,    0, 1033,  535,    0,    0,
  535,    0, 1033,    0,    0,    0,    0, 1033,    0,    0,
 1033,    0,    0,  535,    0,    0,  535,    0,    0,  535,
    0,    0,    0,    0, 1033, 1029,    0,    0,    0, 1033,
 1033,    0,    0, 1033,    0,    0,    0,    0,    0,    0,
 1033,    0, 1033, 1033,    0,  101, 1033,    0,    0,    0,
 1033,    0, 1033,    0, 1033,    0, 1033,  535,    0,    0,
  535, 1033, 1033,    0, 1033,    0, 1029, 1029, 1029, 1029,
 1029, 1029, 1029,    0,    0,  535,    0,    0,    0, 1033,
 1033,    0, 1033,    0, 1029, 1029, 1029, 1029,  535,    0,
    0,    0,    0, 1033,    0, 1033,    0, 1033, 1033, 1033,
    0, 1033,    0,    0, 1033, 1033,    0,    0,    0,  535,
    0,    0,    0,    0, 1033,  535, 1029,    0, 1029,    0,
    0,    0,    0,    0,    0,    0,  535, 1033,    0,    0,
 1033,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  535,  535,    0,    0,  535,  535, 1033,    0,  535,  535,
    0,    0, 1033, 1033,    0,    0,    0, 1033,    0,    0,
    0,  535,    0,    0,    0,    0,    0,    0, 1033, 1033,
    0, 1033,    0,    0,    0,    0,    0, 1033,  535,    0,
 1033,    0, 1033, 1033, 1033, 1033,    0, 1033,    0,    0,
    0,    0,    0,    0,    0, 1033, 1033,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1033, 1033,    0, 1033,
    0,    0,    0,    0,    0,    0, 1033, 1033, 1033, 1033,
 1033,    0, 1033,    0,    0, 1033, 1033, 1033, 1033, 1033,
    0,    0,    0, 1033, 1033,    0,  535,    0, 1033, 1033,
 1033, 1033, 1033, 1033, 1033, 1033,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  535,    0,    0,
    0,    0,    0,  535,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1029,    0, 1029,
 1029,    0,  535,  535,    0,    0, 1029,  535,    0,    0,
  535,    0, 1029,    0,    0,    0,    0, 1029,    0,    0,
 1029,    0,    0,  535,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1029, 1042,    0,    0,    0, 1029,
 1029,    0,    0, 1029,    0,    0,    0,    0,    0,    0,
 1029,    0, 1029, 1029,    0,    0, 1029,    0,    0,    0,
 1029,    0, 1029,    0, 1029,    0, 1029,    0,    0,    0,
    0, 1029, 1029,    0, 1029,    0, 1042, 1042, 1042, 1042,
 1042, 1042, 1042,    0,    0,    0,    0,    0,    0, 1029,
 1029,    0, 1029,    0, 1042, 1042, 1042, 1042,  535,    0,
    0,    0,    0, 1029,    0, 1029,    0, 1029, 1029, 1029,
    0, 1029,    0,    0, 1029, 1029,    0,    0,    0,  535,
    0,    0,    0,    0, 1029,  535, 1042,    0, 1042,    0,
    0,    0,    0,    0,    0,    0,    0, 1029,    0,    0,
 1029,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1029,    0,    0,  535,
    0,    0, 1029, 1029,    0,    0,    0, 1029,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1029, 1029,
    0, 1029,    0,    0,    0,    0,    0, 1029,    0,    0,
 1029,    0, 1029, 1029, 1029, 1029,    0, 1029,    0,    0,
    0,    0,    0,    0,    0, 1029, 1029,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1029, 1029,    0, 1029,
    0,    0,    0,    0,    0,    0, 1029, 1029, 1029, 1029,
 1029,    0, 1029,    0,    0, 1029, 1029, 1029, 1029, 1029,
    0,    0,    0, 1029, 1029,    0,    0,    0, 1029, 1029,
 1029, 1029, 1029, 1029, 1029, 1029,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1042,    0, 1042,
 1042,    0,    0,    0,    0,    0, 1042,    0,    0,    0,
    0,    0, 1042,    0,    0,    0,    0, 1042,    0,    0,
 1042,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1042, 1047,    0,    0,    0, 1042,
 1042,    0,    0, 1042,    0,    0,    0,    0,    0,    0,
 1042,    0, 1042, 1042,    0,    0, 1042,    0,    0,    0,
 1042,    0, 1042,    0, 1042,    0, 1042,    0,    0,    0,
    0, 1042, 1042,    0, 1042,    0, 1047, 1047, 1047, 1047,
 1047, 1047, 1047,    0,    0,    0,    0,    0,    0, 1042,
 1042,    0, 1042,    0, 1047, 1047, 1047, 1047,    0,    0,
    0,    0,    0, 1042,    0, 1042,    0, 1042, 1042, 1042,
    0, 1042,    0,    0, 1042, 1042,    0,    0,    0,    0,
    0,    0,    0,    0, 1042,    0, 1047,    0, 1047,    0,
    0,    0,    0,    0,    0,    0,    0, 1042,    0,    0,
 1042,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1042,    0,    0,    0,
    0,    0, 1042, 1042,    0,    0,    0, 1042,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1042, 1042,
    0, 1042,    0,    0,    0,    0,    0, 1042,    0,    0,
 1042,    0, 1042, 1042, 1042, 1042,    0, 1042,    0,    0,
    0,    0,    0,    0,    0, 1042, 1042,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1042, 1042,    0, 1042,
    0,    0,    0,    0,    0,    0, 1042, 1042, 1042, 1042,
 1042,    0, 1042,    0,    0, 1042, 1042, 1042, 1042, 1042,
    0,    0,    0, 1042, 1042,    0,    0,    0, 1042, 1042,
 1042, 1042, 1042, 1042, 1042, 1042,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1047,    0, 1047,
 1047,    0,    0,    0,    0,    0, 1047,    0,    0,    0,
    0,    0, 1047,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1047,  871,    0,    0,    0, 1047,
 1047,    0,    0, 1047,    0,    0,    0,    0,    0,    0,
 1047,    0, 1047, 1047,    0,    0, 1047,    0,    0,    0,
 1047,    0, 1047,    0, 1047,    0, 1047,    0,    0,    0,
    0, 1047, 1047,    0, 1047,    0,  871,  871,  871,  871,
  871,    0,  871,    0,    0,    0,    0,    0,    0, 1047,
 1047,    0, 1047,    0,  871,  871,  871,  871,    0,    0,
    0,    0,    0, 1047,    0, 1047,    0, 1047, 1047, 1047,
    0, 1047,    0,    0, 1047, 1047,    0,    0,    0,    0,
    0,    0,    0,    0, 1047,    0,    0,    0,  871,    0,
    0,    0,    0,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  293,    0,    0,    0,    0,    0, 1047,    0,    0,    0,
    0,    0, 1047, 1047,    0,    0,    0, 1047,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1047, 1047,
    0, 1047,    0,    0,    0,    0,    0, 1047,    0,    0,
 1047,    0, 1047, 1047, 1047, 1047,    0, 1047,    0,    0,
    0,    0,    0,    0,    0, 1047, 1047,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1047, 1047,    0, 1047,
    0,    0,    0,    0,    0,    0, 1047, 1047, 1047, 1047,
 1047,    0, 1047,    0,    0, 1047, 1047, 1047, 1047, 1047,
    0,    0,    0, 1047, 1047,    0,    0,    0, 1047, 1047,
 1047, 1047, 1047, 1047, 1047, 1047,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  871,    0,  871,
  871,    0,    0,    0,    0,    0,  871,    0,    0,    0,
    0,    0,  871,    0,    0,    0,    0,  871,    0,    0,
  871,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  871,  884,    0,    0,    0,  871,
  871,    0,    0,  871,    0,    0,    0,    0,    0,    0,
  871,    0,  871,  871,    0,    0,  871,    0,    0,    0,
  871,    0,  871,    0,  871,    0,  871,    0,    0,    0,
    0,  871,  871,    0,  871,    0,  884,  884,  884,  884,
  884,    0,  884,    0,    0,    0,    0,    0,    0,  871,
  871,  827,  871,    0,  884,  884,  884,  884,    0,    0,
    0,    0,    0,  871,    0,  871,    0,  871,  871,  871,
    0,  871,    0,    0,  871,  871,    0,    0,    0,    0,
    0,    0,    0,    0,  871,    0,    0,    0,  884,    0,
    0,    0,    0,    0,    0,    0,    0,  871,    0,    0,
  871,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1878,    0,    0,    0,    0,    0,  871,    0,    0,    0,
    0,    0,  871,  871,    0,    0,    0,  871,  464,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  871,  871,
    0,  871,    0,    0,    0,    0,    0,  871,    0,    0,
  871,    0,  871,  871,  871,  871,    0,  871,    0,    0,
    0,    0,    0,    0,    0,  871,  871,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  871,  871,    0,  871,
    0,    0,    0,    0,    0,    0,  871,  871,  871,  871,
  871,    0,  871,    0,    0,  871,  871,  871,  871,  871,
    0,    0,    0,  871,  871,    0,    0,    0,  871,  871,
  871,  871,  871,  871,  871,  871,    0,    0,    0,    0,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  121,  122,    0,  123,    0,    0,  884,    0,  884,
  884,  124,  125,    0,  126,  127,  884,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,  884,  134,  135,
  884,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,  884,  887,    0,    0,    0,  884,
  884,    0,    0,  884,    0,    0,    0,    0,    0,    0,
  884,    0,  884,  884,    0,    0,  884,    0,    0,    0,
  884,    0,  884,    0,  884,    0,  884,    0,    0,    0,
    0,  884,  884,    0,  884,    0,  887,  887,  887,  887,
  887,    0,  887,    0,    0,    0,    0,    0,    0,  884,
  884,    0,  884,    0,  887,  887,  887,  887,    0,    0,
    0,    0,    0,  884,    0,  884,    0,  884,  884,  884,
    0,  884,    0,    0,  884,  884,    0,    0,    0,    0,
    0,    0,    0,    0,  884,    0,    0,    0,  887,    0,
    0,    0,    0,    0,    0,    0,    0,  884,    0,    0,
  884,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  884,    0,    0,    0,
    0,    0,  884,  884,    0,    0,    0,  884,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  884,  884,
    0,  884,    0,    0,    0,    0,    0,  884,    0,    0,
  884,    0,  884,  884,  884,  884,    0,  884,    0,    0,
    0,    0,    0,    0,    0,  884,  884,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  884,  884,    0,  884,
    0,    0,    0,    0,    0,    0,  884,  884,  884,  884,
  884,    0,  884,    0,    0,  884,  884,  884,  884,  884,
    0,    0,    0,  884,  884,    0,    0,    0,  884,  884,
  884,  884,  884,  884,  884,  884,    0,    0,    0,    0,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  121,  122,    0,  123,    0,    0,  887,    0,  887,
  887,  124,  125,    0,  126,  127,  887,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
  887,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,  887,  888,    0,    0,    0,  887,
  887,    0,    0,  887,    0,    0,    0,    0,    0,    0,
  887,    0,  887,  887,    0,    0,  887,    0,    0,    0,
  887,    0,  887,    0,  887,    0,  887,    0,    0,    0,
    0,  887,  887,    0,  887,    0,  888,  888,  888,  888,
  888,    0,  888,    0,    0,    0,    0,    0,    0,  887,
  887,    0,  887,    0,  888,  888,  888,  888,    0,    0,
    0,    0,    0,  887,    0,  887,    0,  887,  887,  887,
    0,  887,    0,    0,  887,  887,    0,    0,    0,    0,
    0,    0,    0,    0,  887,    0,    0,    0,  888,    0,
    0,    0,    0,    0,    0,    0,    0,  887,    0,    0,
  887,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  887,    0,    0,    0,
    0,    0,  887,  887,    0,    0,    0,  887,    0,    0,
    0,    0,    0, 1291,    0,    0,    0,    0,  887,  887,
    0,  887,    0,    0,    0,    0,    0,  887,    0,    0,
  887,    0,  887,  887,  887,  887,    0,  887,    0,    0,
    0,    0,    0,    0,    0,  887,  887,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  887,  887,    0,  887,
    0,    0,    0,    0,    0,    0,  887,  887,  887,  887,
  887,    0,  887,    0,    0,  887,  887,  887,  887,  887,
    0,    0,    0,  887,  887,    0,    0,    0,  887,  887,
  887,  887,  887,  887,  887,  887,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  888,    0,  888,
  888,    0,    0,    0,    2,    0,  888,    0,    0,    0,
    0,    0,    3,    0,    0,    0,    0,    0,    0,    4,
  888,    5,    0,    6,  882,    0,    0,    0,    7,    0,
    0,    0,    0,    0,  888,    8,    0,    0,    0,  888,
  888,    0,    0,  888,    0,    0,    0,    0,    0,    9,
  888,    0,  888,  888,    0,    0,  888,    0,    0,   10,
  888,    0,  888,    0,  888,  882,  888,  882,  882,  882,
    0,  888,  888,    0,  888,    0,    0,    0,    0,    0,
    0,    0,    0,  882,  882,  882,  882,    0,    0,  888,
  888,    0,  888,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  888,    0,  888,    0,  888,  888,  888,
    0,  888,    0,    0,  888,  888,    0,  882,    0,    0,
  426,    0,   11,    0,  888,    0,    0,    0,    0,   12,
    0,    0,   13,    0,   14,   15,    0,  888,    0,    0,
  888,    0,    0,    0,    0,    0,    0,    0,  428,    0,
    0,    0,    0,    0,    0,    0,  888,    0,    0,   16,
    0,    0,  888,  888,    0,    0,    0,  888,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,  888,  888,
    0,  888,    0,    0,    0,    0,    0,  888,    0,    0,
  888,    0,  888,  888,  888,  888,    0,  888,    0,    0,
    0,    0,    0,    0,    0,  888,  888,    0,   17,    0,
    0,    0,    0,    0,    0,    0,  888,  888,    0,  888,
    0,    0,    0,   18,   19,    0,  888,  888,  888,  888,
  888,    0,  888,    0,    0,  888,  888,  888,  888,  888,
    0,    0,    0,  888,  888,    0,    0,  435,  888,  888,
  888,  888,  888,  888,  888,  888,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
    0,  236,  237,    0,    0,    0,  882,    0,  882,  882,
    0,    0,    0,    0,    0,  882,    0,    0,    0,    0,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  882,
    0,    0,    0,  881,    0,  453,  454,    0,    0,    0,
    0,    0,    0,  882,  458,    0,    0,    0,  882,  882,
    0,    0,  882,    0,    0,    0,    0,    0,    0,  882,
    0,  882,  882,    0,    0,  882,    0,    0,    0,  882,
    0,  882,    0,  882,  881,  882,  881,  881,  881,    0,
  882,  882,    0,  882,  905,    0,    0,  628,    0,    0,
    0,    0,  881,  881,  881,  881,    0,    0,  882,  882,
  906,  882,    0,    0,  629,    0,    0,  630,    0,    0,
    0,    0,  882,    0,  882,    0,  882,  882,  882,    0,
  882,    0,    0,  882,  882,    0,  881,    0,    0,    0,
    0,    0,    0,  882,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  882,  631,    0,  882,
    0,    0,    0,  632,  633,  634,    0,  635,  636,  637,
    0,  638,    0,    0,    0,  882,    0,    0,    0,    0,
    0,  882,  882,    0,    0,    0,  882,    0,    0,  639,
    0,  640,    0,    0,    0,    0,    0,  882,  882,    0,
  882,    0,    0,    0,    0,    0,  882,    0,    0,  882,
    0,  882,  882,  882,  882,    0,  882,    0,    0,    0,
  641,    0,    0,    0,  882,  882,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  882,  882,  642,  882,    0,
    0,    0,    0,    0,    0,  882,  882,  882,  882,  882,
    0,  882,    0,    0,  882,  882,  882,  882,  882,  643,
    0,    0,  882,  882,    0,    0,    0,  882,  882,  882,
  882,  882,  882,  882,  882,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  644,  645,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  881,    0,  881,  881,    0,
    0,  646,    0,    0,  881,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  881,    0,
    0,    0,  883,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  881,    0,    0,    0,    0,  881,  881,    0,
    0,  881,    0,    0,    0,    0,    0,    0,  881,    0,
  881,  881,    0,    0,  881,    0,    0,    0,  881,    0,
  881,    0,  881,  883,  881,  883,  883,  883,    0,  881,
  881,    0,  881,  211,    0,    0,  211,    0,    0,    0,
    0,  883,  883,  883,  883,    0,    0,  881,  881,  211,
  881,    0,    0,  211,    0,    0,  211,    0,    0,    0,
    0,  881,    0,  881,    0,  881,  881,  881,    0,  881,
    0,    0,  881,  881,    0,  883,    0,    0,    0,    0,
    0,    0,  881,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  881,  211,    0,  881,    0,
    0,    0,  211,  211,  211,    0,  211,  211,  211,    0,
  211,    0,    0,    0,  881,    0,    0,    0,    0,    0,
  881,  881,    0,    0,    0,  881,    0,    0,  211,    0,
  211,    0,    0,    0,    0,    0,  881,  881,    0,  881,
    0,    0,    0,    0,    0,  881,    0,    0,  881,    0,
  881,  881,  881,  881,    0,  881,    0,    0,    0,  211,
    0,    0,    0,  881,  881,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  881,  881,  211,  881,    0,    0,
    0,    0,    0,    0,  881,  881,  881,  881,  881,    0,
  881,    0,    0,  881,  881,  881,  881,  881,  211,    0,
    0,  881,  881,    0,    0,    0,  881,  881,  881,  881,
  881,  881,  881,  881,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  211,  211,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  883,    0,  883,  883,    0,    0,
  211,    0,    0,  883,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  883,    0,    0,
    0,  886,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  883,    0,    0,    0,    0,  883,  883,    0,    0,
  883,    0,    0,    0,    0,    0,    0,  883,    0,  883,
  883,    0,    0,  883,    0,    0,    0,  883,    0,  883,
    0,  883,  886,  883,  886,  886,  886,    0,  883,  883,
    0,  883,    0,    0,    0,    0,    0,    0,    0,    0,
  886,  886,  886,  886,    0,    0,  883,  883,    0,  883,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  883,    0,  883,    0,  883,  883,  883,    0,  883,    0,
    0,  883,  883,    0,  886,    0,    0,    0,    0,    0,
    0,  883,    0,    0,    0,    0,    0,    0,    0,  981,
  744,  742,    0,  743,  883,  745,    0,  883,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  937,  936,
  938,    0,    0,  883,    0,    0,    0,    0,    0,  883,
  883,    0,    0,    0,  883,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  883,  883,    0,  883,    0,
    0,    0,    0,    0,  883,    0,    0,  883,    0,  883,
  883,  883,  883,    0,  883,    0,    0,    0,    0,    0,
    0,    0,  883,  883,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  883,  883,    0,  883,    0,    0,    0,
    0,    0,    0,  883,  883,  883,  883,  883,  663,  883,
    0,    0,  883,  883,  883,  883,  883,    0,    0,    0,
  883,  883,    0,    0,    0,  883,  883,  883,  883,  883,
  883,  883,  883,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  663,  663,
    0,    0,  663,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  886,    0,  886,  886,  663,    0,    0,
    0,    0,  886,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  886,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  886,    0,    0,    0,    0,  886,  886,    0,  690,  886,
    0,    0,    0,    0,    0,    0,  886,    0,  886,  886,
    0,    0,  886,    0,    0,    0,  886,    0,  886,    0,
  886,    0,  886,    0,    0,    0,    0,  886,  886,  924,
  886,    0,    0,    0,    0,  740,    0,    0,    0,  690,
  741,    0,  690,  925,    0,  886,  886,    0,  886,    0,
    0,    0,    0,    0,    0,    0,    0,  690,    0,  886,
    0,  886,    0,  886,  886,  886,    0,  886,    0,    0,
  886,  886,    0,    0,    0,    0,    0,    0,    0,  926,
  886,    0,    0,    0,    0,    0,    0,  927,    0,    0,
    0,    0,    0,  886,    0,  928,  886,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  886,  929,    0,  930,    0,    0,  886,  886,
    0,    0,    0,  886,    0,    0,    0,    0,  931,    0,
  932,  933,  934,    0,  886,  886,    0,  886,    0,    0,
    0,    0,    0,  886,    0,    0,  886,    0,  886,  886,
  886,  886,    0,  886,    0,    0,    0,    0,    0,    0,
    0,  886,  886,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  886,  886,    0,  886,    0,    0,    0,  935,
    0,    0,  886,  886,  886,  886,  886,    0,  886,    0,
    0,  886,  886,  886,  886,  886,    0,    0,    0,  886,
  886,    0,    0,    0,  886,  886,  886,  886,  886,  886,
  886,  886,    0,  663,    0,    0,  663,    0,    0,    0,
    0,    0,    0,  663,    0,  663,    0,    0,    0,  663,
    0,    0,    0,    0,  663,    0,    0,  663,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  862,    0,
    0,    0,  663,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  663,    0,    0,    0,
    0,    0,    0,    0,  663,    0,    0,    0,  663,    0,
    0,    0,    0,    0,    0,    0,    0,  663,    0,  862,
 1036, 1036,  862, 1036, 1036, 1036,    0,    0,    0,    0,
    0,    0,    0,  663,    0,    0,    0,  862, 1036, 1036,
 1036,    0,    0,  690,    0,    0,  690,    0,    0,    0,
    0,    0,    0,  690,    0,  690,    0,    0,    0,  690,
  663,    0,    0,    0,  690,    0,    0,  690,    0, 1036,
    0,    0,  663,    0,  663,    0,    0,    0,    0,    0,
  663,    0,  690,  663,    0,  663,  663,  663,  663,    0,
  663,    0,    0,    0,    0,    0,  690,    0,  663,  663,
    0,    0,    0,    0,  690,    0,    0,    0,  690,  663,
  663,    0,  663,    0,    0,    0,    0,  690,    0,  663,
  663,  663,  663,  663,    0,  663,    0,    0,  663,  663,
  663,  663,  663,  690,    0,    0,  663,  663,    0,    0,
    0,  663,  663,  663,  663,  663,  663,  663,  663,    0,
  744,  742,    0,  743,    0,  745,    0,    0,    0,    0,
  690,    0,    0,    0,    0,    0,    0,    0,  937,  936,
  938,    0,  690,    0,  690,    0,    0,    0,    0,    0,
  663,    0,    0,  663,    0,  663,  663,  663,  663,    0,
  663,    0,    0,    0,    0,    0,    0,    0,  663,  663,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  663,
  663,    0,  663,    0,    0,    0,    0,    0,    0,  663,
  663,  690,  663,  663,    0,  663,    0,  863,  663,  663,
  663,  663,  663,    0,    0,    0,  663,  663,    0,    0,
  862,  690,  663,  663,  663,  690,  663,  663,  663, 1036,
    0,    0,    0,    0,    0, 1036,    0,    0,    0,    0,
 1036,    0,    0, 1036,    0,    0,    0,    0,  863, 1037,
 1037,  863, 1037, 1037, 1037,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  863, 1037, 1037, 1037,
    0,    0,    0,  862,    0,    0,  862,    0,    0, 1036,
    0,  998,    0,  862,  998,  862,    0, 1036,    0,  862,
    0,    0,    0,    0,  862, 1036,    0,  862, 1037, 1000,
    0,  998,    0,    0,  998,    0,    0,    0,    0,    0,
    0,    0,  862, 1036,    0, 1036,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  862,    0, 1036,    0,
 1036, 1036, 1036,    0,  862,    0,    0,  862,  862,    0,
    0,    0,    0,    0,  998,    0,    0,  862,    0,    0,
  998,  998,  998,  864,  998,  998,  998,    0,  998,  924,
    0,    0,    0,  862,    0,  740,    0,    0,    0,    0,
  741,    0,    0,  925,    0,    0,  998,    0,  998, 1036,
    0,    0,    0,    0,    0,  862,    0,    0,    0,    0,
  862,    0,    0,    0,  864, 1038, 1038,  864, 1038, 1038,
 1038,    0,  862,    0,  862,    0,    0,  998,    0,  926,
    0,    0,  864, 1038, 1038, 1038,    0,  927,    0,    0,
    0,    0,    0,    0,  998,  928,    0,    0,    0,    0,
    0,    0,  819,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  929, 1038,  930,  998,    0,    0,    0,
    0,  862,    0,    0,    0,    0,    0,    0,  931,    0,
  932,  933,  934,    0,    0,    0,    0,    0,    0,  863,
    0,  862,    0,  819,    0,  862,  819,    0, 1037,    0,
    0,  998,  998,    0, 1037,    0,    0,    0,    0, 1037,
    0,  819, 1037,    0,    0,    0,    0,  627,  998,    0,
  628,    0,    0,    0,  820,    0,    0,    0,    0,  935,
    0,    0,    0,    0,    0,    0,    0,  629,    0,    0,
  630,    0,  863,    0,    0,  863,    0,    0, 1037,    0,
    0,    0,  863,    0,  863,    0, 1037,    0,  863,    0,
    0,    0,    0,  863, 1037,  820,  863,    0,  820,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  631,  863, 1037,  820, 1037,    0,  632,  633,  634,    0,
  635,  636,  637,    0,  638,  863,    0, 1037,    0, 1037,
 1037, 1037,    0,  863,    0,    0,  863,  863,  846,    0,
    0,    0,  639,    0,  640,    0,  863,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  863,    0,    0,  864,    0,    0,    0,    0,
    0,    0,    0,  641, 1038,    0,    0,    0, 1037,  846,
 1038,    0,  846,    0,  863, 1038,    0,    0, 1038,  863,
  642,    0,    0,    0,    0,    0,    0,  846,    0,    0,
    0,  863,    0,  863,    0,    0,    0,    0,    0,    0,
  823,    0,  643,    0,    0,    0,    0,    0,  864,    0,
    0,  864,    0,    0, 1038,    0,    0,    0,  864,    0,
  864,    0, 1038,    0,  864,    0,    0,    0,    0,  864,
 1038,    0,  864,    0,  819,    0,    0,  644,  645,    0,
  863,  823,    0,    0,  823,    0,    0,  864, 1038,    0,
 1038,    0,    0,    0,  646,    0,    0,    0,    0,  823,
  863,  864,    0, 1038,  863, 1038, 1038, 1038,    0,  864,
    0,    0,  864,  864,    0,    0,    0,    0,    0,    0,
    0,    0,  864,  821,    0,    0,    0,  819,    0,    0,
  819,    0,    0,    0,    0,    0,    0,  819,  864,  819,
    0,    0,    0,  819,    0,    0,  820,    0,  819,    0,
    0,  819,    0,    0, 1038,    0,    0,    0,    0,    0,
  864,    0,    0,    0,  821,  864,  819,  821,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  864,    0,  864,
  819,    0,  821,    0,    0,    0,    0,    0,  819,    0,
    0,  819,  819,    0,    0,    0,    0,    0,    0,  820,
    0,  819,  820,    0,    0,    0,    0,  822,    0,  820,
    0,  820,    0,    0,    0,  820,    0,  819,    0,    0,
  820,    0,    0,  820,    0,    0,  864,    0,    0,    0,
  846,    0,    0,    0,    0,    0,    0,    0,  820,  819,
    0,    0,    0,    0,  819,    0,  864,    0,  822,    0,
  864,  822,  820,  854,    0,    0,  819,    0,  819,    0,
  820,    0,    0,  820,  820,    0,  822,    0,    0,    0,
    0,    0,    0,  820,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  846,    0,    0,  846,    0,    0,  820,
    0,    0,    0,  846,  854,  846,    0,  854,    0,  846,
    0,    0,  823,    0,  846,  819,    0,  846,    0,    0,
    0,  820,  854,    0,    0,    0,  820,    0,    0,    0,
    0,    0,  846,    0,    0,  819,    0,    0,  820,  819,
  820,    0,    0,    0,    0,    0,  846,  816,    0,    0,
    0,    0,    0,    0,  846,    0,    0,  846,  846,    0,
    0,    0,    0,    0,    0,  823,    0,  846,  823,    0,
    0,    0,    0,    0,    0,  823,    0,  823,    0,    0,
    0,  823,    0,  846,    0,    0,  823,  820,  816,  823,
    0,  816,    0,  817,    0,  821,    0,    0,    0,    0,
    0,    0,    0,    0,  823,  846,  816,  820,    0,    0,
  846,  820,    0,    0,    0,    0,    0,    0,  823,    0,
    0,    0,  846,    0,  846,    0,  823,    0,    0,  823,
  823,    0,    0,    0,  817,    0,    0,  817,    0,  823,
    0,    0,    0,    0,    0,    0,    0,    0,  821,    0,
    0,  821,  817,    0,    0,  823,    0,    0,  821,    0,
  821,    0,    0,    0,  821,    0,    0,    0,    0,  821,
    0,  846,  821,    0,    0,    0,    0,  823,    0,  822,
    0,    0,  823,    0,    0,    0,    0,  821,    0,    0,
    0,  846,    0,    0,  823,  846,  823,    0,    0,    0,
    0,  821,    0,    0,    0,    0,    0,    0,    0,  821,
    0,    0,  821,  821,    0,    0,    0,    0,    0,    0,
    0,    0,  821,    0,    0,  854,    0,    0,    0,    0,
    0,    0,  822,    0,    0,  822,    0,    0,  821,    0,
    0,    0,  822,  823,  822,    0,    0,    0,  822,    0,
    0,  818,    0,  822,    0,    0,  822,    0,    0,    0,
  821,    0,    0,  823,    0,  821,    0,  823,    0,    0,
    0,  822,    0,    0,    0,    0,    0,  821,  854,  821,
    0,  854,    0,    0,    0,  822,    0,    0,  854,    0,
  854,    0,  818,  822,  854,  818,  822,  822,    0,  854,
    0,    0,  854,    0,    0,    0,  822,    0,    0,  816,
  818,    0,    0,    0,    0,    0,    0,  854,    0,    0,
    0,    0,  822,  853,    0,    0,  821,    0,    0,    0,
    0,  854,    0,    0,    0,    0,    0,    0,    0,  854,
    0,    0,  854,  854,  822,    0,  821,    0,    0,  822,
  821,    0,  854,    0,    0,  817,    0,    0,    0,    0,
    0,  822,  816,  822,  853,  816,    0,  853,  854,    0,
    0,    0,  816,    0,  816,    0,    0,    0,  816,    0,
    0,    0,  853,  816,    0,    0,  816,    0,    0,    0,
  854,    0,    0,    0,    0,  854,    0,    0,  847,    0,
    0,  816,    0,    0,    0,    0,    0,  854,  817,  854,
  822,  817,    0,    0,    0,  816,    0,    0,  817,    0,
  817,    0,    0,  816,  817,    0,  816,  816,    0,  817,
  822,    0,  817,    0,  822,    0,  816,    0,    0,  847,
    0,    0,  847,    0,    0,    0,    0,  817,    0,    0,
    0,    0,  816,    0,    0,    0,  854,  847,  855,    0,
    0,  817,    0,    0,    0,    0,    0,    0,    0,  817,
    0,    0,  817,  817,  816,    0,  854,    0,    0,  816,
  854,    0,  817,    0,    0,    0,    0,    0,    0,    0,
    0,  816,  856,  816,    0,    0,    0,    0,  817,  855,
    0,    0,  855,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  818,    0,    0,    0,  855,    0,    0,
  817,    0,    0,    0,    0,  817,    0,    0,    0,    0,
    0,    0,    0,  856,    0,    0,  856,  817,    0,  817,
  816,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  856,    0,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,    0,  816,    0,  818,    0,    0,  818,
    0,    0,    0,    0,    0,    0,  818,    0,  818,    0,
    0,    0,  818,    0,    0,  853,  817,  818,    0,    0,
  818,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  818,  817,    0,  844,    0,
  817,    0,    0,    0,    0,    0,    0,    0,    0,  818,
    0,    0,    0,    0,    0,    0,    0,  818,    0,    0,
  818,  818,    0,    0,    0,    0,    0,    0,  853,    0,
  818,  853,    0,    0,    0,    0,    0,    0,  853,  844,
  853,    0,  844,    0,  853,    0,  818,    0,    0,  853,
  847,    0,  853,    0,    0,    0,    0,  844,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853,  818,    0,
    0,    0,    0,  818,    0,    0,    0,    0,    0,    0,
    0,  853,  842,    0,    0,  818,    0,  818,    0,  853,
    0,    0,  853,  853,    0,    0,    0,    0,    0,    0,
    0,    0,  853,  847,    0,    0,  847,    0,    0,    0,
  855,    0,    0,  847,    0,  847,    0,    0,  853,  847,
    0,    0,    0,  842,  847,    0,  842,  847,    0,    0,
    0,    0,    0,    0,  818,    0,    0,    0,    0,    0,
  853,  842,  847,    0,  856,  853,    0,    0,    0,    0,
    0,    0,    0,    0,  818,    0,  847,  853,  818,  853,
    0,    0,    0,  855,  847,    0,  855,  847,  847,    0,
    0,    0,    0,  855,    0,  855,    0,  847,    0,  855,
    0,  848,    0,    0,  855,    0,    0,  855,    0,    0,
    0,    0,    0,  847,    0,    0,    0,  856,    0,    0,
  856,    0,  855,    0,    0,    0,  853,  856,    0,  856,
    0,    0,    0,  856,    0,  847,  855,    0,  856,    0,
  847,  856,  848,    0,  855,  848,  853,  855,  855,    0,
  853,    0,  847,    0,  847,    0,  856,  855,    0,    0,
  848,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  856,    0,    0,  855,    0,    0,    0,    0,  856,    0,
  844,  856,  856,    0,    0,    0,    0,    0,    0,    0,
    0,  856,    0,    0,    0,  855,  857,    0,    0,    0,
  855,  847,    0,    0,    0,    0,    0,  856,    0,    0,
    0,    0,  855,    0,  855,    0,    0,    0,    0,    0,
    0,  847,    0,    0,  843,  847,    0,    0,    0,  856,
    0,    0,    0,  844,  856,    0,  844,  857,    0,    0,
  857,    0,    0,  844,    0,  844,  856,    0,  856,  844,
    0,    0,    0,    0,  844,  857,    0,  844,    0,    0,
    0,  855,    0,    0,  842,  843,    0,    0,  843,    0,
    0,    0,  844,    0,    0,    0,    0,    0,    0,    0,
    0,  855,    0,  843,    0,  855,  844,    0,    0,    0,
    0,    0,    0,    0,  844,  856,    0,  844,  844,    0,
    0,    0,    0,    0,    0,    0,    0,  844,    0,    0,
    0,    0,    0,    0,    0,  856,    0,  842,    0,  856,
  842,    0,    0,  844,    0,    0,    0,  842,    0,  842,
    0,    0,    0,  842,    0,    0,    0,    0,  842,    0,
    0,  842,    0,    0,  845,  844,    0,    0,    0,    0,
  844,    0,    0,    0,    0,    0,  842,    0,    0,    0,
    0,    0,  844,  848,  844,    0,    0,    0,    0,    0,
  842,    0,    0,    0,    0,    0,    0,    0,  842,    0,
    0,  842,  842,    0,    0,  845,    0,    0,  845,    0,
    0,  842,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  845,    0,    0,    0,  842,    0,    0,
    0,  844,    0,    0,    0,    0,  848,    0,    0,  848,
    0,    0,    0,    0,    0,    0,  848,    0,  848,  842,
    0,  844,  848,    0,  842,  844,    0,  848,  849,    0,
  848,    0,    0,    0,    0,    0,  842,    0,  842,    0,
    0,    0,    0,    0,    0,  848,    0,    0,  857,    0,
    0,    0,    0,    0,    0,    0,  794,    0,    0,  848,
    0,    0,    0,    0,    0,    0,    0,  848,    0,  849,
  848,  848,  849,    0,    0,    0,  843,    0,    0,    0,
  848,    0,    0,    0,    0,  842,    0,  849,    0,    0,
    0,    0,    0,    0,    0,    0,  848,  794,    0,    0,
  794,  857,    0,    0,  857,  842,    0,    0,    0,  842,
    0,  857,    0,  857,    0,  794,    0,  857,  848,    0,
    0,    0,  857,  848,    0,  857,    0,    0,    0,  843,
    0,    0,  843,    0,    0,  848,    0,  848,    0,  843,
  857,  843,    0,    0,    0,  843,    0,    0,    0,    0,
  843,  676,    0,  843,  857,    0,    0,    0,    0,    0,
    0,    0,  857,    0,    0,  857,  857,    0,  843,    0,
    0,    0,    0,    0,    0,  857,    0,    0,    0,  687,
    0,    0,  843,    0,  848,    0,    0,    0,    0,    0,
  843,  857,  676,  843,  843,  676,  845,    0,    0,    0,
    0,    0,    0,  843,  848,    0,    0,    0,  848,    0,
  676,    0,    0,  857,    0,    0,    0,    0,  857,  843,
  687,    0,    0,  687,    0,    0,    0,    0,    0,    0,
  857,    0,  857,    0,    0,    0,    0,    0,  687,    0,
    0,  843,    0,    0,    0,    0,  843,    0,    0,  845,
    0,    0,  845,    0,    0,    0,    0,    0,  843,  845,
  843,  845,    0,  141,    0,  845,  141,    0,    0,    0,
  845,    0,    0,  845,    0,    0,    0,    0,    0,  857,
    0,    0,    0,  141,    0,    0,  141,    0,  845,    0,
  849,    0,    0,    0,    0,    0,    0,    0,    0,  857,
    0,    0,  845,  857,    0,    0,    0,  843,    0,    0,
  845,    0,    0,  845,  845,    0,    0,    0,    0,    0,
    0,    0,    0,  845,    0,    0,  141,  843,    0,    0,
    0,  843,  141,  141,  141,    0,  141,  141,  141,  845,
  141,    0,    0,  849,    0,    0,  849,  691,    0,    0,
  461,    0,  462,  849,    0,  849,    0,    0,  141,  849,
  141,  845,    0,    0,  849,  464,  845,  849,    0,    0,
  313,  794,    0,    0,  794,    0,    0,    0,  845,    0,
  845,  794,  849,  794,    0,    0,    0,  794,    0,  141,
    0,    0,  794,    0,    0,  794,  849,    0,    0,    0,
    0,    0,    0,    0,  849,    0,  141,  849,  849,    0,
  794,    0,    0,    0,    0,    0,    0,  849,    0,    0,
    0,    0,    0,    0,  794,    0,    0,  845,  141,    0,
    0,    0,  794,  849,    0,  794,  794,    0,    0,    0,
    0,    0,    0,    0,    0,  794,    0,  845,    0,    0,
    0,  845,    0,    0,    0,  849,    0,    0,    0,    0,
  849,  794,    0,  141,  141,    0,  676,    0,    0,  676,
    0,    0,  849,    0,  849, 1024,  676,    0,  676,    0,
  141,    0,  676,  794,    0,    0,    0,  676,  794,    0,
  676,    0,    0,    0,  687,    0,    0,  687,    0,    0,
  794,    0,  794,    0,  687,  676,  687,    0,    0,    0,
  687,    0,    0,    0,    0,  687,    0,    0,  687,  676,
    0,  849,    0,    0,    0,    0,    0,  676,    0,    0,
    0,  676,    0,  687,    0,    0,    0,    0,    0,    0,
  676,  849,    0,    0,    0,  849,    0,  687,    0,  794,
    0,    0,    0,    0,    0,  687,  676,    0,    0,  687,
    0,    0,    0,    0,    0,    0,    0,    0,  687,  794,
    0,    0,    0,  794,  423,    0,    0,    0,    0,    0,
    0,  424,    0,  676,  687,    0,    0,    0,    0,    0,
    0,    0,    0,  425,    0,  676,    0,  676,    0,  426,
  427,  691,    0,    0,  461,    0,  462,    0,    0,    0,
    0,  687,    0,    0,    0,    0,    0,    0,  685,  464,
    0,    0,    0,  687,  313,  687,    0,  428,    0,    0,
  429,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  676,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  430,    0,  431,  432,    0,
    0,    0,    0,    0,  676,    0,    0,  433,  676,    0,
    0,  434,  687,    0,    0,    0,    0,    0,    0,  916,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  687,    0,    0,    0,  687,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  997,   14,    0,    0,    0,
  687,    0,    0,    0,    0,    0,  435,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,  439,  440,  441,
  236,  237,  113,    0,  114,  115,  116,  117,    0,  118,
  442,    0,    0,    0,    0,    0,    0,  119,  120,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  121,  452,
    0,  123,    0,    0,  453,  454,    0,  455,  456,  457,
    0,  126,  127,  458,  128,    0,    0,  129,  130,  131,
  132,  133,  917,  918,  919,  134,  135,    0,    0,    0,
    0,  136,  459,  138,    0,  460,  140,  141,  423,    0,
    0,    0,    0,    0,    0,  424,    0,    0,    0,  691,
    0,    0,  461,    0,  462,    0,    0,  425,    0,    0,
  684,    0,    0,  426,  427,    0,    0,  464,    0,    0,
    0,    0,  313,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  685,    0,    0,    0,    0,    0,    0,    0,
    0,  428,    0,    0,  429,    0,    0,  112,    0,    0,
  113,    0,  114,  115,  116,  117,    0,  118,    0,    0,
    0,    0,    0,    0,    0,  119,  120,    0,    0,  430,
    0,  431,  432,    0,    0,    0,  121,  122,    0,  123,
    0,  433,    0,    0,    0,  434,  124,  125,    0,  126,
  127,    0,  128,  686,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  137,  138,    0,  139,  140,  141,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  996,    0,    0,  997,
    0,    0,    0,    0,  687,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,    0,  436,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  438,  439,  440,  441,  236,  237,  113,    0,  114,  115,
  116,  117,    0,  118,  442,    0,    0,    0,    0,    0,
    0,  119,  120,  443,  444,  445,  446,  447,  448,  449,
  450,  451,  121,  452,    0,  123,    0,    0,  453,  454,
    0,  455,  456,  457,    0,  126,  127,  458,  128,    0,
    0,  129,  130,  688,  689,  690,  423,    0,    0,  134,
  135,    0,    0,  424,    0,  136,  459,  138,    0,  460,
  140,  141,    0,    0,    0,  425,    0,    0,    0,    0,
    0,  426,  427,  691,    0,    0,  461,    0,  462,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  685,  464,    0,    0,    0,    0,  313,    0,    0,  428,
    0,    0,  429,    0,    0,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,    0,    0,    0,
    0,    0,    0,  119,  120,    0,    0,  430,    0,  431,
  432,    0,    0,    0,  121,  122,    0,  123,    0,  433,
    0,    0,    0,  434,  124,  125,    0,  126,  127,    0,
  128,  912,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  660,
    0,    0,  687,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  438,  439,
  440,  441,  236,  237,  113,    0,  114,  115,  116,  117,
    0,  118,  442,    0,    0,    0,    0,    0,    0,  119,
  120,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  121,  452,    0,  123,    0,    0,  453,  454,    0,  455,
  456,  457,    0,  126,  127,  458,  128,    0,    0,  129,
  130,  688,  689,  690,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  459,  138,    0,  460,  140,  141,
  423,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,  691,    0,    0,  461,    0,  462,    0,    0,  425,
    0,    0,    0,    0,    0,  426,  427,    0,    0,  464,
    0,    0,    0,    0,  313,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  685,    0,    0,    0,    0,    0,
    0,    0,    0,  428,    0,    0,  429,    0,    0,  112,
    0,    0,  113,    0,  114,  115,  116,  117,    0,  118,
    0,    0,    0,    0,    0,    0,    0,  119,  120,    0,
    0,  430,    0,  431,  432,    0,    0,    0,  121,  122,
    0,  123,    0,  433,    0,    0,    0,  434,  124,  125,
    0,  126,  127,    0,  128,  686,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  137,  138,    0,  139,  140,  141,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  687,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,  436,    0,
    0,   82,  660,    0,    0,    0,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  438,  439,  440,  441,  236,  237,  113,    0,
  114,  115,  116,  117,    0,  118,  442,    0,    0,    0,
    0,    0,    0,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,    0,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  688,  689,  690,  423,    0,
    0,  134,  135,    0,    0,  424,    0,  136,  459,  138,
    0,  460,  140,  141,    0,    0,    0,  425,    0,    0,
    0,    0,    0,  426,  427,    0,    0,    0,    0,    0,
    0,    0,  529,    0,    0,  461,    0,  462,    0,    0,
    0,    0,  685,    0,    0,    0,    0,    0,    0,    0,
  464,  428,    0,    0,  429,  313,    0,  663,    0,    0,
  663,    0,  663,  663,  663,  663,    0,  663,    0,    0,
    0,    0,    0,    0,    0,  663,  663,    0,    0,  430,
    0,  431,  432,    0,    0,    0,  663,  663,    0,  663,
    0,  433,    0,    0,    0,  434,  663,  663,    0,  663,
  663,    0,  663,  912,    0,  663,  663,  663,  663,  663,
    0,    0,    0,  663,  663,    0,    0,    0,    0,  663,
  663,  663,    0,  663,  663,  663,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  687,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,    0,  436,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  438,  439,  440,  441,  236,  237,  113,    0,  114,  115,
  116,  117,    0,  118,  442,  464,    0,    0,    0,    0,
    0,  119,  120,  443,  444,  445,  446,  447,  448,  449,
  450,  451,  121,  452,    0,  123,    0,    0,  453,  454,
    0,  455,  456,  457,    0,  126,  127,  458,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  459,  138,    0,  460,
  140,  141, 1155,    0,    0, 1156,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  425,    0,
    0,    0,    0,    0,  426,  427,    0,    0,    0,    0,
    0,    0,    0,  529,    0,    0,  461,    0,  462,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  464,  428,    0,    0,  429,  313,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   81,    0,  112,    0,
    0,  113,    0,  114,  115,  116,  117,    0,  118,    0,
  430,    0,  431,  432,    0,    0,  119,  120,    0,    0,
    0,    0,  433,    0,    0,    0,  434,  121,  122,    0,
  123,    0,    0,    0,    0,    0,    0,  124,  125,    0,
  126,  127,    0,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  137,  138,    0,  139,  140,  141,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1157,
    0,  435,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  437,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  438,  439,  440,  441,  236,  237,  113, 1630,  114,
  115,  116,  117,    0,  118,  442,    0,    0,    0,    0,
    0,    0,  119,  120,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  121,  452,    0,  123,    0,    0,  453,
  454,    0,  455,  456,  457,    0,  126,  127,  458,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  459,  138,    0,
  460,  140,  141, 1166,    0,    0, 1156,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  425,
    0,    0,    0,    0,    0,  426,  427,    0,    0,    0,
    0,    0,    0,    0,  529,    0,    0,  461,    0,  462,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  464,  428,    0,    0,  429,  313,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  112,
    0,    0,  113,    0,  114,  115,  116,  117,    0,  118,
    0,  430,    0,  431,  432,    0,    0,  119,  120,    0,
    0,    0,    0,  433,    0,    0,    0,  434,  121,  122,
    0,  123,    0,    0,    0,    0,    0,    0,  124,  125,
    0,  126,  127,    0,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  137,  138,    0,  139,  140,  141,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1157,    0,  435,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  438,  439,  440,  441,  236,  237,  113, 1639,
  114,  115,  116,  117,    0,  118,  442,    0,    0,    0,
    0,    0,    0,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,    0,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  459,  138,
    0,  460,  140,  141, 1170,    0,    0, 1156,    0,    0,
    0,  423,    0,    0,    0,    0,    0,    0,  424,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  425,    0,    0,    0,    0,    0,  426,  427,    0,    0,
    0,    0,    0,    0,    0,  529,    0,    0,  461,    0,
  462,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  464,  428,    0,    0,  429,  313,    0,
    0,  112,    0,    0,  113,    0,  114,  115,  116,  117,
    0,  118,    0,    0,    0,    0,    0,    0,    0,  119,
  120,    0,  430,    0,  431,  432,    0,    0,    0,    0,
  121,  122,    0,  123,  433,    0,    0,    0,  434,    0,
  124,  125,    0,  126,  127,    0,  128,    0,    0,  129,
  130,  131,  132,  133,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  137,  138,    0,  139,  140,  141,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1157,    0,  435,    0,    0,    0,    0,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  437,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  438,  439,  440,  441,  236,  237,  113,
    0,  114,  115,  116,  117,    0,  118,  442,    0,    0,
    0,    0,    0,    0,  119,  120,  443,  444,  445,  446,
  447,  448,  449,  450,  451,  121,  452,    0,  123,    0,
    0,  453,  454,    0,  455,  456,  457,    0,  126,  127,
  458,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  459,
  138,    0,  460,  140,  141, 1178,    0,    0, 1156,    0,
    0,    0,  423,    0,    0,    0,    0,    0,    0,  424,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  425,    0,    0,    0,    0,    0,  426,  427,    0,
    0,    0,    0,    0,    0,    0,  529,    0,    0,  461,
    0,  462,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  464,  428,    0,    0,  429,  313,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,  430,    0,  431,  432,    0,    0,    0,
    0,  121,  122,    0,  123,  433,    0,    0,    0,  434,
    0,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1157,    0,  435,    0,    0,    0,    0,    0,
  436,    0,    0,    0,  595,    0,    0,    0,    0,    0,
  437,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  438,  439,  440,  441,  236,  237,
  113,    0,  114,  115,  116,  117,    0,  118,  442,    0,
    0,    0,    0,    0,    0,  119,  120,  443,  444,  445,
  446,  447,  448,  449,  450,  451,  121,  452,    0,  123,
    0,    0,  453,  454,    0,  455,  456,  457,    0,  126,
  127,  458,  128,    0,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  459,  138,    0,  460,  140,  141, 1181,    0,    0, 1156,
    0,    0,    0,  423,    0,    0,    0,    0,    0,    0,
  424,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  425,    0,    0,    0,    0,    0,  426,  427,
    0,    0,    0,    0,    0,    0,    0,  529,    0,    0,
  461,    0,  462,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  464,  428,    0,    0,  429,
  313,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,  430,    0,  431,  432,    0,    0,
  119,  120,    0,    0,    0,    0,  433,    0,    0,    0,
  434,  121,  122,    0,  123,    0,    0,    0,    0,    0,
    0,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,  105,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1157,    0,  435,    0,    0,    0,    0,
    0,  436,    0,    0,    0,    0,    0,    0,    0,  106,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  107,    0,    0,    0,  438,  439,  440,  441,  236,
  237,  113,    0,  114,  115,  116,  117,    0,  118,  442,
    0,    0,    0,    0,    0,  108,  119,  120,  443,  444,
  445,  446,  447,  448,  449,  450,  451,  121,  452,    0,
  123,    0,    0,  453,  454,    0,  455,  456,  457,    0,
  126,  127,  458,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  459,  138,    0,  460,  140,  141, 1186,    0,    0,
 1156,    0,    0,    0,  423,    0,    0,    0,    0,    0,
  109,  424,    0,    0,    0,    0,    0,    0,    0,    0,
  110,    0,    0,  425,    0,    0,    0,    0,    0,  426,
  427,    0,    0,    0,    0,    0,    0,    0,  529,    0,
    0,  461,    0,  462,    0,  111,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  464,  428,    0,    0,
  429,  313,    0,  112,    0,    0,  113,    0,  114,  115,
  116,  117,    0,  118,    0,    0,    0,    0,    0,    0,
    0,  119,  120,    0,    0,  430,    0,  431,  432,    0,
    0,    0,  121,  122,    0,  123,    0,  433,    0,    0,
    0,  434,  124,  125,    0,  126,  127,    0,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  137,  138,    0,  139,
  140,  141,   48,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1157,    0,  435,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
   48,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   48,    0,    0,    0,  438,  439,  440,  441,
  236,  237,  113,    0,  114,  115,  116,  117,    0,  118,
  442,    0,    0,    0,    0,    0,   48,  119,  120,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  121,  452,
    0,  123,    0,    0,  453,  454,    0,  455,  456,  457,
    0,  126,  127,  458,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  459,  138,    0,  460,  140,  141, 1189,    0,
    0, 1156,    0,    0,    0,  423,    0,    0,    0,    0,
    0,   48,  424,    0,    0,    0,    0,    0,    0,    0,
    0,   48,    0,    0,  425,    0,    0,    0,    0,    0,
  426,  427,    0,    0,    0,    0,    0,    0,    0,  529,
    0,    0,  461,    0,  462,    0,   48,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  464,  428,    0,
    0,  429,  313,    0,   50,    0,    0,   50,    0,   50,
   50,   50,   50,    0,   50,    0,    0,    0,    0,    0,
    0,    0,   50,   50,    0,    0,  430,    0,  431,  432,
    0,    0,    0,   50,   50,    0,   50,    0,  433,    0,
    0,    0,  434,   50,   50,    0,   50,   50,    0,   50,
    0,    0,   50,   50,   50,   50,   50,    0,    0,    0,
   50,   50,    0,    0,    0,    0,   50,   50,   50,    0,
   50,   50,   50,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1157,    0,  435,  601,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
  441,  236,  237,  113,    0,  114,  115,  116,  117,    0,
  118,  442,    0,    0,    0,    0,    0,    0,  119,  120,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  121,
  452,    0,  123,    0,    0,  453,  454,    0,  455,  456,
  457,    0,  126,  127,  458,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
    0,    0,  136,  459,  138,    0,  460,  140,  141, 1192,
    0,    0, 1156,    0,    0,    0,  423,    0,    0,    0,
    0,    0,    0,  424,    0,    0,    0,  647,    0,  647,
  647,    0,  647,    0,    0,  425,    0,    0,    0,    0,
    0,  426,  427,    0,    0,  647,    0,    0,    0,    0,
  647,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
    0,    0,  429,    0,    0,    0,  112,    0,    0,  113,
    0,  114,  115,  116,  117,    0,  118,    0,    0,    0,
    0,    0,    0,    0,  119,  120,    0,  430,    0,  431,
  432,    0,    0,    0,    0,  121,  122,    0,  123,  433,
    0,    0,    0,  434,    0,  124,  125,    0,  126,  127,
    0,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  137,
  138,    0,  139,  140,  141,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1157,    0,  435,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  438,  439,
  440,  441,  236,  237,  113,    0,  114,  115,  116,  117,
    0,  118,  442,    0,    0,    0,    0,    0,    0,  119,
  120,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  121,  452,    0,  123,    0,    0,  453,  454,    0,  455,
  456,  457,    0,  126,  127,  458,  128,  647,    0,  129,
  130,  131,  132,  133,  647,    0,    0,  134,  135,    0,
    0,  647,    0,  136,  459,  138,    0,  460,  140,  141,
    0,    0,    0,  647,    0,    0,    0,    0,    0,  647,
  647,    0,    0,    0,    0,    0,    0,  647,  641,    0,
  641,  641,    0,  641,    0,    0,  510,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  641,  647,    0,    0,
  647,  641,    0,  112,    0,    0,  113,    0,  114,  115,
  116,  117,    0,  118,    0,    0,    0,    0,    0,    0,
    0,  119,  120,    0,    0,  647,    0,  647,  647,    0,
    0,    0,  121,  122,    0,  123,    0,  647,    0,    0,
    0,  647,  124,  125,    0,  126,  127,    0,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  137,  138,    0,  139,
  140,  141,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  647,    0,    0,    0,
    0,    0,  647,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  647,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  647,  647,  647,  647,
  647,  647,  647,    0,  647,  647,  647,  647,    0,  647,
  647,    0,    0,    0,    0,    0,    0,  647,  647,  647,
  647,  647,  647,  647,  647,  647,  647,  647,  647,  647,
    0,  647,    0,    0,  647,  647,    0,  647,  647,  647,
    0,  647,  647,  647,  647,    0,    0,  647,  647,  647,
  647,  647,    0,    0,    0,  647,  647,    0,    0,    0,
    0,  647,  647,  647,    0,  647,  647,  647,  641,    0,
    0,    0,    0,    0,    0,  641,  211,    0,    0,  211,
    0,    0,  641,    0,    0,    0,  529,    0,  534,  461,
    0,  462,  211,    0,  641,    0,  211,    0,    0,  211,
  641,  641,    0,    0,  464,    0,    0,    0,  641,  313,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  641,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,  211,
    0,    0,    0,    0,    0,  211,  211,  211,    0,  211,
  211,  211,    0,  211,    0,    0,  641,    0,  641,  641,
    0,    0,    0,    0,    0,    0,    0,    0,  641,    0,
    0,  211,  641,  211,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  211,    0,    0,    0,    0,   80,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  211,
    0,    0,    0,    0,    0,    0,    0,  641,    0,    0,
    0,    0,    0,  641,    0,    0,    0,    0,    0,    0,
    0,  211,    0,  641,    0, 1239,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  641,  641,  641,
  641,  641,  641,  641,    0,  641,  641,  641,  641,    0,
  641,  641,    0,    0,    0,    0,  211,  211,  641,  641,
  641,  641,  641,  641,  641,  641,  641,  641,  641,  641,
  641,    0,  641,  211,    0,  641,  641,    0,  641,  641,
  641,    0,  641,  641,  641,  641,    0,    0,  641,  641,
  641,  641,  641,  423,    0, 1240,  641,  641,    0,    0,
  424,    0,  641,  641,  641,    0,  641,  641,  641,    0,
    0,    0,  425,    0,    0,    0,    0,    0,  426,  427,
  529,    0,    0,  461,    0,  462,    0,    0,    0,    0,
 1241,    0,    0,    0,    0, 1242,    0,    0,  464,    0,
    0, 1243,    0,  313,    0,    0,  428,    0,    0,  429,
    0,    0,  112,    0,    0,  113,    0,  114,  115,  116,
 1244,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,  430,    0,  431,  432,    0,    0,
    0,  121,  122,    0,  123,    0,  433,    0,    0,    0,
  434,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
    0,  436,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  438,  439,  440,  441,  236,
  237,  113,    0,  114,  115,  116,  117,    0,  118,  442,
    0,    0,    0,    0,    0,    0,  119,  120,  443,  444,
  445,  446,  447,  448,  449,  450,  451,  121,  452,    0,
  123,    0,    0,  453,  454,    0,  455,  456,  457,    0,
  126,  127,  458,  128,    0,    0,  129,  130,  131,  132,
  133,    0,  941,    0,  134,  135,    0,    0,    0,    0,
  136,  459,  138,    0,  460,  140,  141,  423,    0,    0,
    0,    0,    0,    0,  424,  942,    0,    0,  529,    0,
    0,  461,    0,  462,    0,    0,  425,    0,    0,    0,
    0,    0,  426,  427,    0,    0,  464,    0,    0,    0,
    0,  313,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,    0,    0,  429,    0,    0,    0,    0,    0,    0,
    0,  943,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  430,    0,
  431,  432,    0,    0,    0,    0,    0,    0,    0,    0,
  433,    0,    0,    0,  434,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,    0,    0,    0,
    0,    0,    0,  119,  120,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  121,  122,    0,  123,    0,    0,
    0,    0,    0,    0,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129,  130,  131,  132,  133,    0,  435,
    0,  134,  135,    0,    0,  436,    0,  136,  137,  138,
    0,  139,  140,  141,    0,  437,    0,    0,    0,    0,
    0,    0,    0,  723,    0,    0,    0,    0,    0,  438,
  439,  440,  441,  236,  237,  113,    0,  114,  115,  116,
  117,    0,  118,  442,    0,    0,    0,    0,    0,    0,
  119,  120,  443,  444,  445,  446,  447,  448,  449,  450,
  451,  121,  452,    0,  123,    0,    0,  453,  454,    0,
  455,  456,  457,    0,  126,  127,  458,  128,    0,    0,
  129,  130,  131,  132,  133,  423,    0,    0,  134,  135,
    0,    0,  424,    0,  136,  459,  138,    0,  460,  140,
  141,    0,    0,    0,  425,    0,    0,    0,    0,    0,
  426,  427,  529,  985,    0,  461,    0,  462,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  464,    0,    0,    0,    0,  313,    0,    0,  428,    0,
    0,  429,    0,    0,  112,    0,    0,  113,    0,  114,
  115,  116,  117,    0,  118,    0,    0,    0,    0,    0,
    0,    0,  119,  120,    0,    0,  430,    0,  431,  432,
    0,    0,    0,  121,  122,    0,  123,    0,  433,    0,
    0,    0,  434,  124,  125,    0,  126,  127,    0,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  137,  138,    0,
  139,  140,  141,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
  441,  236,  237,  113,    0,  114,  115,  116,  117,    0,
  118,  442,    0,    0,    0,    0,    0,    0,  119,  120,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  121,
  452,    0,  123,    0,    0,  453,  454,    0,  455,  456,
  457,    0,  126,  127,  458,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
    0,    0,  136,  459,  138,    0,  460,  140,  141,  423,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
  529,    0,    0,  461,    0,  462,    0,    0,  425,    0,
    0,    0,    0,    0,  426,  427,    0,    0,  464,    0,
    0,    0,    0,  313,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,    0,    0,  429,    0,    0,   27,    0,
    0,   27,    0,   27,   27,   27,   27,    0,   27,    0,
    0,    0,    0,    0,    0,    0,   27,   27,    0,    0,
  430,    0,  431,  432,    0,    0,    0,   27,   27,    0,
   27,    0,  433,    0,    0,    0,  434,   27,   27,    0,
   27,   27,    0,   27,    0,    0,   27,   27,   27,   27,
   27,    0,    0,    0,   27,   27,    0,    0,    0,    0,
   27,   27,   27,    0,   27,   27,   27,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  435,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  437,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  438,  439,  440,  441,  236,  237,  113,    0,  114,
  115,  116,  117,    0,  118,  442,    0,    0,    0,    0,
    0,    0,  119,  120,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  121,  452,    0,  123,    0,    0,  453,
  454,    0,  455,  456,  457,    0,  126,  127,  458,  128,
    0,    0,  129,  130,  131,  132,  133,  423,    0,    0,
  134,  135,    0,    0,  424,    0,  136,  459,  138,    0,
  460,  140,  141,    0,    0,    0,  425,    0,    0,    0,
    0,    0,  426,  427,  529,    0,    0,  461,    0,  462,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  464,    0,    0,    0,    0,  313,    0,    0,
  428,    0,    0,  429,    0,    0,  578,    0,    0,  578,
    0,  578,  578,  578,  578,    0,  578,    0,    0,    0,
    0,    0,    0,    0,  578,  578,    0,    0,  430,    0,
  431,  432,    0,    0,    0,  578,  578,    0,  578,    0,
  433,    0,    0,    0,  434,  578,  578,    0,  578,  578,
    0,  578,    0,    0,  578,  578,  578,  578,  578,    0,
    0,    0,  578,  578,    0,    0,    0,    0,  578,  578,
  578,    0,  578,  578,  578,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  435,
    0,    0,    0,    0,    0,  436,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  437,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1184,    0,    0,  438,
  439,  440,  441,  236,  237,  113,    0,  114,  115,  116,
  117,    0,  118,  442,    0,    0,    0,    0,    0,    0,
  119,  120,  443,  444,  445,  446,  447,  448,  449,  450,
  451,  121,  452,    0,  123,    0,    0,  453,  454,    0,
  455,  456,  457,    0,  126,  127,  458,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  459,  138,    0,  460,  140,
  141,  423,    0,    0,    0,    0,    0,    0,  424,    0,
    0,    0,  529,    0,    0,  461,    0,  462,    0,    0,
  425,    0,    0,    0,    0,    0,  426,  427,    0,    0,
  464, 1753,    0,    0,    0,  313,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  428,    0,    0,  429,    0,    0,
  280,    0,    0,  280,    0,  280,  280,  280,  280,    0,
  280,    0,    0,    0,    0,    0,    0,    0,  280,  280,
    0,    0,  430,    0,  431,  432,    0,    0,    0,  280,
  280,    0,  280,    0,  433,    0,    0,    0,  434,  280,
  280,    0,  280,  280,    0,  280,    0,    0,  280,  280,
  280,  280,  280,    0,    0,    0,  280,  280,    0,    0,
    0,    0,  280,  280,  280,    0,  280,  280,  280,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  435,    0,    0,    0,    0,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  437,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1390,    0,    0,  438,  439,  440,  441,  236,  237,  113,
    0,  114,  115,  116,  117,    0,  118,  442,    0,    0,
    0,    0,    0,    0,  119,  120,  443,  444,  445,  446,
  447,  448,  449,  450,  451,  121,  452,    0,  123,    0,
    0,  453,  454,    0,  455,  456,  457,    0,  126,  127,
  458,  128,    0,    0,  129,  130,  131,  132,  133,  423,
    0,    0,  134,  135,    0,    0,  424,    0,  136,  459,
  138,    0,  460,  140,  141,    0,    0,    0,  425,    0,
    0,    0,    0,    0,  426,  427,  990,    0,  990,  990,
    0,  990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  990,    0,    0,    0,    0,  990,
    0,    0,  428,    0,    0,  429,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  430,    0,  431,  432,    0,    0,    0,    0,    0,    0,
    0,    0,  433,    0,    0,    0,  434,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  435,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  437,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  438,  439,  440,  441,  236,  237,  113,    0,  114,
  115,  116,  117,    0,  118,  442,    0,    0,    0,    0,
    0,    0,  119,  120,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  121,  452,    0,  123,    0,    0,  453,
  454,    0,  455,  456,  457,    0,  126,  127,  458,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  459,  138,    0,
  460,  140,  141,  990,    0,    0,    0,    0,    0,    0,
  990,    0,    0,    0,  463,    0,    0,  461,    0,  462,
    0,    0,  990,    0,    0,    0,    0,    0,  990,  990,
    0,    0,  464,    0,    0,    0,    0,  313,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  990,    0,    0,  990,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  990,    0,  990,  990,    0,    0,
    0,    0,    0,    0,    0,    0,  990,    0,    0,    0,
  990,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  990,    0,    0,    0,    0,
    0,  990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  990,  990,  990,  990,  990,
  990,  990,    0,  990,  990,  990,  990,    0,  990,  990,
    0,    0,    0,    0,    0,    0,  990,  990,  990,  990,
  990,  990,  990,  990,  990,  990,  990,  990,  990,    0,
  990,    0,    0,  990,  990,    0,  990,  990,  990,    0,
  990,  990,  990,  990,    0,    0,  990,  990,  990,  990,
  990,  423,    0,    0,  990,  990,    0,    0,  424,    0,
  990,  990,  990,    0,  990,  990,  990,    0,    0,    0,
  425,    0,    0,    0,    0,    0,  426,  427,  529,    0,
    0,  461,    0,  462,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  464,    0,    0,    0,
    0,  313,    0,    0,  428,    0,    0,  429,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  430,    0,  431,  432,    0,    0,    0,    0,
    0,    0,    0,    0,  433,    0,    0,    0,  434,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  435,    0,    0,    0,    0,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  437,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  438,  439,  440,  441,  236,  237,  113,
    0,  114,  115,  116,  117,    0,  118,  442,    0,    0,
    0,    0,    0,    0,  119,  120,  443,  444,  445,  446,
  447,  448,  449,  450,  451,  121,  452,    0,  123,    0,
    0,  453,  454,    0,  455,  456,  457,    0,  126,  127,
  458,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  459,
  138,    0,  460,  140,  141,  423,    0,    0,    0,    0,
    0,    0,  424,    0,    0,    0,  990,    0,    0,  990,
    0,  990,    0,    0,  425,    0,    0,    0,    0,    0,
  426,  427,    0,    0,  990,    0,    0,    0,    0,  990,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  428,    0,
    0,  429,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,  431,  432,
    0,    0,    0,    0,    0,    0,    0,    0,  433,    0,
    0,    0,  434,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
  441,  236,  237,  113,    0,  114,  115,  116,  117,    0,
  118,  442,    0,    0,    0,    0,    0,    0,  119,  120,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  121,
  452,    0,  123,    0,    0,  453,  454,    0,  455,  456,
  457,  545,  126,  127,  458,  128,    0,    0,  129,  130,
  131,  132,  133,  990,    0,    0,  134,  135,    0,    0,
  990,    0,  136,  459,  138,    0,  460,  140,  141,    0,
    0,    0,  990,    0,    0,    0,    0,    0,  990,  990,
    0,  545,  545,    0,  546,  545,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  540,    0,
  545,    0,  545,    0,    0,    0,  990,    0,    0,  990,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  546,  546,    0,    0,  546,    0,
    0,    0,  545,    0,  990,    0,  990,  990,    0,  540,
    0,    0,  540,  546,    0,  546,  990,    0,    0,    0,
  990,    0,    0,    0,    0,    0,    0,  540,    0,  540,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  546,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  540,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  990,    0,    0,    0,    0,
    0,  990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  990,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  990,  990,  990,  990,  990,
  990,  990,    0,  990,  990,  990,  990,    0,  990,  990,
    0,    0,  562,    0,    0,    0,  990,  990,  990,  990,
  990,  990,  990,  990,  990,  990,  990,  990,  990,    0,
  990,    0,    0,  990,  990,    0,  990,  990,  990,    0,
  990,  990,  990,  990,    0,    0,  990,  990,  990,  990,
  990,    0,    0,  562,  990,  990,  562,    0,    0,  545,
  990,  990,  990,    0,  990,  990,  990,    0,    0,    0,
    0,  562,    0,  562,  545,    0,  545,  545,  545,    0,
    0,    0,  545,    0,    0,  545,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  545,    0,
    0,    0,  546,  562,  545,    0,    0,  545,    0,    0,
  552,    0,    0,    0,    0,    0,  540,  546,    0,  546,
  546,  546,    0,    0,    0,  546,    0,    0,  546,    0,
    0,  540,    0,  540,  540,  540,    0,    0,    0,  540,
    0,  546,  540,    0,    0,    0,    0,  546,    0,  545,
  546,  552,    0,    0,  552,  540,    0,    0,    0,    0,
    0,  540,    0,    0,  540,    0,    0,    0,    0,  552,
    0,  552,    0,  545,    0,    0,    0,    0,    0,    0,
  537,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  546,    0,  545,    0,    0,    0,    0,    0,
  545,  552,    0,    0,  545,    0,  540,    0,    0,    0,
    0,  545,    0,    0,    0,    0,  546,    0,    0,    0,
    0,  537,    0,    0,  537,    0,    0,    0,    0,  545,
  540,    0,    0,    0,  545,    0,    0,  546,    0,  537,
    0,  537,    0,  546,    0,    0,    0,  546,    0,    0,
    0,  540,    0,    0,  546,    0,    0,  540,    0,  545,
    0,  540,    0,  545,    0,    0,    0,    0,  540,    0,
  562,  537,  546,    0,    0,    0,    0,  546,    0,    0,
    0,    0,    0,    0,    0,  562,  540,    0,  562,  562,
    0,  540,    0,  562,    0,    0,  562,    0,    0,    0,
    0,    0,  546,    0,    0,    0,  546,    0,    0,  562,
    0,    0,    0,    0,    0,  562,  540,    0,  562,    0,
  540,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  562,    0,    0,    0,    0,    0,    0,    0,  552,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  552,  562,    0,  552,  552,    0,    0,
    0,  552,    0,    0,  552,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  562,    0,  552,    0,    0,
    0,  562,    0,  552,    0,  562,  552,    0,    0,    0,
    0,    0,  562,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  537,    0,
  562,    0,    0,    0,    0,  562,    0,    0,    0,    0,
    0,    0,    0,  537,    0,    0,  537,  537,  552,    0,
    0,  537,    0,    0,  537,    0,  321,    0,    0,    0,
  562,    0,    0,    0,  562,    0,    0,  537,    0,    0,
    0,    0,  552,  537,    0,    0,  537,    0,    0,    0,
    0,    0,    0,    0,  321,    0,    0,    0,    0,    0,
    0,  321,    0,  552,    0,  321,  321,    0,  321,  552,
    0,  321,    0,  552,    0,    0,    0,    0,    0,    0,
  552,    0,    0,    0,  321,    0,    0,    0,  537,    0,
  321,    0,    0,    0,    0,    0,    0,    0,  552,    0,
    0,    0,    0,  552,    0,    0,    0,    0,    0,    0,
    0,    0,  537,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1236,    0,    0,    0,  552,    0,
    0,    0,  552,  537,    0,    0,    0,    0,    0,  537,
    0,    0,    0,  537,    0,    0,    0,  321,    0,    0,
  537,    0,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1439, 1440,  321, 1441,    0,  537,    0,
    0,    0,    0,  537,    0,    0,    0,    0,    0,    0,
  321,    0, 1442,    0,    0,    0,    0,    0, 1443,    0,
  321,    0,    0,    0,    0,    0,    0,    0,  537,  321,
    0,  321,  537,    0,    0,    0,    0,    0,  321,    0,
    0,  321,    0,  321,  321,  321,  321,    0,  321,    0,
    0,    0,    0,    0,    0,    0,  321,  321,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  321,  321,    0,
  321,    0,    0,    0,    0, 1444,    0,  321,  321,    0,
  321,  321,    0,  321,    0,    0,  321,  321,  321,  321,
  321,    0, 1236,   14,  321,  321,  321,  321,  321,    0,
  321,  321,  321,    0,  321,  321,  321,    0, 1445,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   16,    0,
    6,    0,    0,    0,    0,    0,    0, 1650,    0,    0,
    0, 1439, 1440,    0, 1441,    0,  441,    0,    0,  113,
    0,  114,  115,  116,  117,    0,  118,    0,    0,    0,
 1442,    0,    0,    0,  119,  120, 1443,    0,    0,    0,
    0,    0,    0,    0,    0, 1446,  122,    0,  123,    0,
    0,    0,    0,    0,    0,  124,  125,    0,  126,  127,
    0,  128,    0,    0,  129, 1447,  131,  132,  133,    0,
    0,    0,  134,  135, 1448, 1449, 1450,    0,  136,  137,
  138,    0,  139,  140,  141,    0,    0,    0,    0,    0,
    0,    0,    0, 1444,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1445,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  441,    0,    0,  113,    0,  114,
  115,  116,  117,    0,  118,  627,    0,    0,  628,    0,
    0,    0,  119,  120,  827,    0,    0,    0,    0,    0,
    0,    0,    0, 1446,  122,  629,  123,    0,  630,    0,
    0,    0,    0,  124,  125,    0,  126,  127,    0,  128,
    0,    0,  129, 1447,  131,  132,  133,    0,    0,    0,
  134,  135, 1448, 1449, 1450,    0,  136,  137,  138,    0,
  139,  140,  141,    0,    0,    0,    0,    0,  631,    0,
    0,    0,    0,    0,  632,  633,  634,    0,  635,  636,
  637,    0,  638,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  639,    0,  640,  627,    0,    0,  628,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  629,    0,    0,  630,    0,    0,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  642,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  631,    0,    0,    0,
  643,    0,  632,  633,  634,    0,  635,  636,  637,    0,
  638,    0,    0,    0,    0,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,    0,  639,    0,
  640,    0,    0,  119,  120,  644,  645,    0,    0,    0,
    0,    0,    0,    0,  121,  122,    0,  123,    0,    0,
    0,    0,  646,    0,  124,  125,    0,  126,  127,  641,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,  642,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  643,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  112,    0,    0,  113,    0,  114,  115,
  116,  117,    0,  118,    0,    0,    0,    0,    0,    0,
    0,  119,  120,  644,  645,    0,    0,    0,    0,    0,
    0,    0,  121,  122,    0,  123,    0,    0,    0,    0,
  646,    0,  124,  125,    0,  126,  127,    0,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  137,  138,    0,  139,
  140,  141,  112,    0,    0,  113,  185,  114,  115,  116,
  117,  186,  118,  187,  188,  189,  190,  191,  192,  193,
  119,  120,  194,  195,  196,  197,  198,    0,    0,  199,
  200,  121,  122,  201,  123,    0,    0,  202,  203,  204,
  205,  124,  125,  206,  126,  127,  207,  128,  208,  209,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
  210,  211,  212,  213,  136,  137,  138,  214,  139,  140,
  141,  462,    0,    0,  462,  462,  462,  462,  462,  462,
    0,  462,  462,  462,  462,  462,  462,  462,  462,  462,
  462,  462,  462,  462,  462,  462,    0,    0,  462,  462,
  462,  462,  462,  462,    0,    0,  462,  462,  462,  462,
  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,
  462,  462,  462,  462,    0,    0,    0,  462,  462,  462,
  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,
};
static short yycheck[] = {                                       9,
    4,    0,   68,  111,   67,  169,   16,  564,   18,   58,
   74,   67,   60,  159,  160,  159,  160,   77,   59,   66,
   16,  748,   99,  401,   90,   73,  401,  397,  299,   96,
   71,  596,   72,  567,   98,  499,  371,  540,   62,  801,
  100,  393,  463, 1235,  709,   94,   56,   57,   58,   59,
   60,   61,   62, 1236,   95, 1308,   96,   67,   68,   69,
  518,   71,  103,   73,   74,  519,   76,   77,   78,   93,
  735,  369,  799, 1065, 1024, 1065,   86,  101,   88,   89,
   90,  946,   92,   93,   94,   95, 1004,   97,   98,   99,
  100,  101,   88,  103, 1106,  151,  173,  153,  315,    0,
  755,   97,  672,  679,  152,    0,   41,  344,  377,  463,
  613,  378,   44,    0, 1254,   44, 1065,  723,  983,    0,
  899,   45,    0,  729,  172,  589,    0,  591,   40,    0,
  171,   45,  596, 1128,    0,   40,    0,   63,    0,  406,
  164,  151,  152,  153, 1065,  250,   41,  157,   44,   59,
   41,  259, 1065,    0,  164,   44,   44,    0,   59,  169,
   44,  171,  172,  173,   59,    0, 1070,  177,  177,    0,
   41,  218,   59, 1077,  806,  529, 1065,   41,   59,   41,
  344,   59,   44,   41, 1088,   59,    0,    0,   59,  821,
   41,    0,  824,   59,    0,   59,   44,   59, 1009,  246,
   40,  833,   41,    0,    0,  575,   41, 1111,    0,   44,
   41,   44,   59,   44,  557, 1072,   59,   44,  596,    0,
    0,  596, 1266,   63,   59, 1300, 1301,   41,   59,  315,
 1305,   44,   41, 1306,  290,  351,   40,  293,  273,  510,
  345,  349,   91,    0,    0,   59,   59,  686,   44,   41,
   59,  257,   44,   59,  624,  526, 1448, 1449,   41,   63,
  892,   45,   59,   59,  264, 1513, 1123,   59,   61,   44,
   40,  304,  305,    0,   61,   45, 1459,  326,   59,   59,
  290,  262,  330,  293,   41,    0,  395,    0,  344,  299,
  300,   45,  559,  257,    0,  324,  280,  273,  447,  260,
 1524,  599,   59,   59,  317,  344,  304,  305, 1300, 1301,
 1300, 1301,  315, 1305,  370, 1305,  326, 1267,  739,   41,
  330,  377,   44,  379,  330,  381,  747, 1339,  384,  457,
    0,  459,   59,  939,  940,   41,    0,  691,   44,  300,
  404,    0,  264,  297,   59, 1569,   59,  394,  298,  396,
  648, 1300, 1301,   59,  518,  298, 1305,  304,  305,  445,
  370, 1570,  403,  410,  412,  414,  330,  377,  484,  379,
  976,  381,  348,  876,  384,  272,  334,   41,    0, 1300,
 1301,    0,   41, 1048, 1305, 1303,  850, 1300, 1301,   59,
  344,   45, 1305,  403,  404,   59,  297,    0,  379,   41,
   59,  380,  412,  403,  414,  514,  310,    0,  418,   63,
  443, 1300, 1301,   41,  418, 1280, 1305,  301, 1050, 1323,
  315, 1669,   41,  458, 1281,   44,  275,  456, 1346,  317,
  325,  297,  327,  276,  277, 1067, 1068,   59, 1691,  782,
   59,  336,  445,  344,  315,  443,  900,  431,  306, 1260,
 1261,  315, 1644,  432,  464, 1087,   59,  911, 1064, 1503,
  508,  915,  518, 1536,   41,  336,   59,  306,  509,  517,
  509,  615,  336,  521,  301,  387,  434,  916,  344,  262,
  315,  403,  315, 1216,  434,  380, 1219,  376,  379,  264,
  498,  434,  402,  764,  389,  356,  443,  358,  508,  509,
  510,  336,  380,    0,  297,  409,  380,  517,  564,  380,
  297,  521,  444,   41,  376,  389,  526,  815,  528,  528,
  818,  450,  336,  315,  450, 1665,  450,  336,  379,  372,
  577,  829,   42,  325,  439,  327,  450,  432, 1791, 1459,
  596,  338, 1148, 1149,  336, 1151, 1152,  339,    0,  616,
  446,  446, 1122,  822,  564,  509, 1014,  662,  856, 1135,
  777,  432,   59,  800, 1514,  446, 1345, 1579,  903,  268,
 1024,  618, 1567, 1130,  380,  446,  616, 1232,  513,  336,
  428,  361,  446,  389,  380,  595,  596, 1840,  380,   41,
 1799,  601,   44,  389,  264, 1016,  379,  389, 1052,  446,
  397, 1519,  316,  400,  410,  361,  752,   59,  509,  315,
  450,  446, 1865,  371,  262,  671,    0,  344,  513,  416,
  262,  668,  669,  890, 1528,  317,  513,    0, 1532, 1533,
  336,  493,  446,  446,  262,  513,  800,  446,  444,  513,
  432,  785,  513,  509, 1827,  442,  450,  428,  444,  513,
   44,  315,  444,  423,  446,  258,  315,  276,  277,  494,
  359,  671,  493,  327, 1568,  284,  450,  270,   41,    0,
 1853,  264,  336, 1748, 1749,   59,  397,  336,  513,  393,
  450,  403,  440, 1289,  315,  262,   59,  402,  401,  446,
  714, 1424, 1549,  477,  297, 1428,  450,  753,  395,  755,
  303,  757,  305, 1886,  714, 1311, 1058,  513, 1900, 1566,
   41, 1376, 1344,   44,  858, 1898,  380,  513, 1901,  433,
 1585,  513,  344,  477,  772,  389, 1909,  448,   59,  484,
  740,  379,  773,  283,  262,  352,  428,  379,  748, 1160,
  446,  344,    0,  753,  800,  755,  794,  757,  796,  805,
  260,  379,  272,  372,  764, 1517, 1748, 1749, 1748, 1749,
   59,  516,  772,  773,  811,   41,  822,  264,  432,  423,
  780,  781, 1124, 1677,  393, 1309, 1074, 1681, 1076,  427,
  399,  848,  446, 1255,  794, 1703,  796,  446, 1086,  799,
  300, 1709, 1696,  849,  850,  805,  450,  390, 1133, 1748,
 1749,   59,  379,  500,  421, 1226, 1160,  513,  848, 1713,
    0, 1832,  822,  305,  433,  855,  336,  864,  322,  886,
  887,  888,  262,  315,    0,  298,    0, 1748, 1749, 1332,
  262,    0, 1335,  873,  836, 1748, 1749,    0,  401,  849,
  850,  261,  305,  891, 1265, 1410,  886,  887,  888,  513,
 1014,  379,  315,  901,  513,    0,  866, 1329,  450, 1748,
 1749,  334,  866,  315,  450,   41, 1887, 1202,  262,   59,
    0,    0,   41,  325,  258,  327,    0,    0,   41,    0,
  275,  891,    0,   59,  336,   59,  270,  339,  298, 1622,
   59,  901,    0,  390, 1798,  319,   59, 1801,   41,  909,
  910,   44,    0,  453,  454,  909,  509, 1827,  344,  371,
 1516,   41,   41,  297,   59,   44,    0,   41,  422,  303,
   41,  305,   42,   41,  334,  429,  263,   47,  380,   59,
   59,  319,    0, 1853,  276,   59,   59,  389,   59,  379,
 1844,   59,  315,   41,    0,  686,  341,  379,  410,    0,
  691,   59,  325,  445,  327,   44,  980, 1270, 1014, 1272,
  344,   59,    0,  336,  300, 1109, 1886,  280, 1239, 1433,
  980, 1242,  309,   41,  316,   59,   44,   40, 1898, 1349,
  432, 1901,  445,  320,  315,  379,  262,  427,  409, 1909,
   41,   59,  444,   44,  446,  457,  327,    0, 1008, 1009,
  284,  259,  395,   59,   93,  336,  264,  380,   59,    0,
    0,  269,  348,  297, 1024,  436,  389,  453,  454, 1373,
 1374,   59,    0, 1321,  434,   93,    0, 1381, 1382, 1635,
 1384, 1385, 1410,  372,    0, 1410,  372,  430, 1392, 1393,
 1394, 1395,   93, 1397, 1398, 1399, 1400, 1401, 1402,  380,
 1117,  393,   40, 1351,  393,    0,   59,  260,  389,  432,
  399,  513,   40,   41,  377, 1132,   44,    0,   59,   59,
 1126, 1138, 1236,  446, 1130,  450,  394, 1117, 1080, 1081,
    0,   59,  419,    0,  258,   59,  276,  277, 1098,  390,
    0,  433, 1132,   59,  433, 1822,  270,  300, 1138, 1103,
  318,  432,  477,  379,  260, 1115,    0, 1253, 1254, 1255,
    0,  339,  277,  331,   59,  446, 1126,  282,  431,  264,
 1130,   41,    0,  297,   44,  509,   59,  428, 1130,  303,
  273,  305,  390,    0,  362,  448,  354,  376, 1647,   59,
  513, 1285,   59, 1287,  300,    0,  315,  276,  277,   59,
   44,    0,  315,  276,  277,  284,  264,  277,  327,  376,
  336,  462,  282,  306,    0,   59, 1243,  336, 1774,   59,
  344,  912, 1235,  336, 1780,  916, 1232, 1241,  352, 1235,
 1236,   59,  513,    0, 1244,  315,   41,  405, 1198,   44,
  418,  315,   59,  297,  315,   42,   43,  315,   45, 1240,
   47,  450, 1198, 1347,   59,  348,  336,  325,    0,  327,
   59,  380,  336,  258,   44,  336,  378,  315,  336,  265,
  389,    0, 1232,   59,   41, 1235, 1236,   44,  477, 1239,
 1240, 1241, 1242, 1243, 1244,    0,   44,  378,  336,  396,
 1538,  297,   59,  372,  348,  390,    0,  421,    0,   41,
 1260, 1261,   44,  299,  409,  412,  380, 1267,  303,  380,
  403,  264,  380,  432,  393,  389,  397,   59,  389,  428,
  399,  389,  300,  264,  264, 1573,   41,  446, 1835,   44,
   59,  436,  390,  446,    0,   93,    0,  333,  344,   41,
  264,    0,   44,  395,   59, 1459,  352,   44,   42,   43,
  338,   45,  352,   47,  433,   59, 1815,   59,  432,  486,
 1819,  432, 1821, 1360,  432,  458,  446,  448, 1365,  264,
  348, 1830,  446, 1327,  426,  446,  444,    0,  446,    0,
  386,  264,  388,  371,  402,  509,  386,  315,  388,  441,
  318, 1850, 1506,   59,  513,   59,   93,  325,  446,  327,
   59,  402,  289,  331,  410,    0,  276,  277,  336,  397,
  410,  339,  400,  265,  284,  410,    0,    0,  258, 1878,
  313,  314,  338, 1439, 1883,  312,  354,    0,  416,  435,
  270,    0,    0,  428,  321,  435,   59,  390,   59,  513,
  368,  397,  513, 1403,  333,  513,    0,  299,  376,  390,
  390,  410,  380, 1459,  442,  371,    0, 1403,  434,   44,
  416,  389, 1710,  303,   59,  305,  390,  462,  355,  271,
  273,   41,  359,    0,   44,   59,   59,  405,  338, 1439,
  277,  397,   44,  285,  400,  282,   59, 1447, 1448, 1449,
   59,   59,    0,   42,   43,  390,   45,   59,   47, 1459,
  416,    0,  372,  509,  432,   59,  376,  390,   93,  276,
  277,  371,  485,  486,  298,   59,  444,  284,  446,   42,
   43, 1647,   45,  393,   47,  258,  442,  329,  261,  399,
 1490, 1491,   59, 1647,  276,  277,    0,  397,  260,  338,
  400, 1539,  284,    0,  446,   41, 1506,  262,   44,  270,
  334,   59, 1666, 1513, 1514,  357,  416,  278,   44,  258,
   59,   42,   43,  433,   45,  493,   47,  260,  455, 1665,
  303,  270,  371,   59,  461,  462,  463,   41,  300, 1539,
   44,  513,  442,  277,   41,  513,   40,   44,  282,  517,
  520,   42,   43,  333,   45,   59,   47,   61,  397,  453,
  315,  400,   59,  318,  303,  372,  305,  300,   59,  260,
  325,  264,  327, 1612,  308,  399,  331,  416,  260,  421,
  333,  336,   46,  493,  339,  409,  393,  316,  368,    0,
  372,  428,  399,   46,   58,  324, 1694,   61,  313,  354,
  380, 1632,  529,  442,  259,   58,  533,  298,   61, 1609,
  434,  393, 1612,  368,  269,  368,   46,  399,  300,  310,
  416,  376,   40, 1609,  379,  380,  433,  380,   58,  485,
   41,   61, 1632,   44,  389,  453,  454,   42,   43,  495,
   45,  315,   47,  334, 1644, 1846,  333, 1848,   59, 1815,
  405,  433, 1690, 1819,  332, 1821, 1695,   42,   43,  452,
   45, 1815,   47,  444, 1830, 1819, 1666, 1821, 1657, 1669,
  426,  259,  427, 1827,  395,   41, 1830,  432,   44, 1716,
  403,  269,   40, 1720, 1850, 1685,  493,  336,  277,  444,
 1690,  446,  376,  282, 1792, 1695, 1850,   41,  317, 1853,
   44,   41,   42,   43,  315,   45,   41,   47,  399,   44,
 1766,  493, 1878,  498,  277,  315,    0, 1883,  409,  282,
  376,   41,   42,   43, 1878,   45,  428,   47,  317, 1883,
  384,  385, 1886,   41,   42,   43,  356,   45,  493,   47,
  453,  454, 1742,  434, 1898,   42,   43, 1901,   45,   41,
   47,  426,   44,  442,  258, 1909,  277,   41,  513,  377,
   44,  282,  517,  375,  691,   41, 1766, 1767,   44,  273,
  710,  497,  712,  336, 1768,   59,  716,  717,  718,  276,
  277, 1827,   41,  409,  711,   44,  277,  284,  715, 1835,
  508,  282,  509,  297,  315,  722,  449,  450,  451,  303,
  727,  728,  306,   42,   43,   44,   45, 1853,   47,  284,
   41,  508,  739,   44,  741,  742,  743,  744,  745,  746,
  747,  461, 1822, 1812,  477,  478,   44, 1827,   61,  929,
    0,  931, 1832,  933,  934, 1835,  936,  937,  938,   41,
 1886,  446,   44,  366,  348,  336, 1846,   41, 1848,  283,
   44,   41, 1898, 1853,   44, 1901,  783,  264,   41,   42,
   43,   40,   45, 1909,   47,  276,  277,   41,  318,   40,
   44,   41,  277,  389,   44,  372,  803,  282,   40,    0,
  265,  331,  809,  810, 1490, 1491, 1886, 1887,    0,   59,
   41,   41,  277,   44,   44,   40,  393,  282, 1898,  403,
 1900, 1901,  399,  308,  354,  351,  410,   42,   43, 1909,
   45,   40,   47,   41,  299,  453,   44,   40,  368,   41,
   41,   40,   44,   44,  428, 1490, 1491,   40,   40,   41,
   42,   43,   44,   45,   46,   47,  433,  277,   59,  453,
   41,   41,  282,   44,   44,   40,   58,   59,   60,   61,
   62,   40,  453,   41,  458,  405,   44,  277,  462,   44,
   41,  372,  282,   44,   40,   40,   40,  264,   40,  277,
   61,   41,   42,   43,  282,   45,   40,   47,   40,   91,
  277,   93,  393,   91,   40,  282,   46,  264,  399,  395,
  317,  409,  276,  277,   44,   44,  493,  924,  925,  926,
  284,  420,  929,  930,  931,   46,  933,  934,  935,  936,
  937,  938,   40,  297,   61,  264,  344,   44,  344,  344,
  947,  948,  433,  950,  276,  952,  276,  297,  955,  956,
  957,  958,  959,  960,  452,  264,  390,  455,  277,  457,
  458,  459,  460,  282,  462,  972,  428,  974,  462,  390,
  450,  275,  470,  471,  344,   41,   42,   43,  439,   45,
  439,   47,  392,  481,  482,   40,  484,  517,   40,   91,
  345,   40,   40,  491,  492,  315,  494,  495,  264,  497,
  376,  403,  500,  501,  502,  503,  504,   44,  372, 1016,
  508,  509,  375,  382,  277,   40,  514,  515,  516,  282,
  518,  519,  520,   40,  443,  315,  276,  277,  380,  393,
   41,  281,  338,   63,  284,  399,  275,  428,  296,   41,
   42,   43,    0,   45,  428,   47,  314,  297,   40,  264,
   44,  444,  456,  440,  264,   44,    0,   41,   42,   43,
   40,   45,  277,   47,  494,  371,  273,  282,   40,  433,
  262,  297,  264,  265,  379,  276,  277,  281,  340,  271,
  340,  276,  379,   41,  379,  277,   44,  410,  410,   40,
  282,  397,  408,  285,  400,  352,   41,   41,   40,   40,
   44,   59,   41,   42,   43, 1815,   45,  299,   47, 1819,
  416,   40,  304,  305,  362,   59,  308,   61,   44,  317,
 1830,  438,  372,  315,  277,  317,  318,  277,  361,  321,
  317,  270,  282,  325,  378,  327,  442,  329,  317,  331,
 1850,   40,   44,  393,  336,  337,  266,  339,  325,  399,
   40,  493,   40, 1160,  464,  465,  466,  467,  468,  469,
  470,  471,  354,  355,  264,  357, 1173, 1174, 1878, 1176,
 1177,  372,   41, 1883,  414,   58,  368, 1184,  370,   41,
  372,  373,  374,  433,  376,   41,   41,  379,  380,   41,
   42,   43,  393,   45,  264,   47,  387,  389,  399, 1206,
   42,   43,   44,   45,   41,   47,   41, 1214, 1215, 1216,
  402,  277, 1219,  405, 1221,  306,  282,  446, 1225, 1226,
 1227,   41,   42,   43,  306,   45,   40,   47,   40,  421,
  317,   40,  433,   40,   40,  427,  428,   41,   42,   43,
  432,   45,   40,   47,   41,   42,   43,  262,   45,   40,
   47,  443,  444,   40,  446,   41,   42,   43, 1265,   45,
  452,   47,   40,  455,   40,  457,  458,  459,  460,  427,
  462,   42,   43,   44,   45,  277,   47,   40,  470,  471,
  282,   41,   42,   43,   40,   45,   40,   47,   40,  481,
  482,   40,  484,  277,   40,  305,   44, 1304,  282,  491,
  492,  493,  494,  495,   44,  497,  264,   41,  500,  501,
  502,  503,  504,  446,  258,   41,  508,  509,  264,  381,
  339,  513,  514,  515,  516,  517,  518,  519,  520,  327,
  339,  275,  276,  277,  384,  273,  284,  281,  277,  399,
  284,  379,   41,  282,  379,    0,  379,  379,   41,   42,
   43,  379,   45,  297,   47,  379,  450,  315,  372,  303,
  318,   41,   41,   42,   43, 1372,   45,  325,   47,  327,
  276, 1378,   40,  331,   41,   41, 1383,   41,  336,   41,
  317,  339,   41, 1390,   44,  305,   41,   42,   43,   44,
   45,   46,   47,   41,   42,   43,  354,   45,  427,   47,
   40,   59,   59,  290,   59,   60,   61,   62,   42,   43,
  368,   45,  336,   47,   59,   59,   59, 1424,  376,  336,
   59, 1428,  380,   59,  305,  277,   59,   59,  372,   59,
  282,  389,  318,   58,  339,  277,   91, 1444,   93,  324,
  282,  428,  333,  277,  434,  331,  434,  405,  298,  393,
  334,  298,  334,  339,   41,  399,   41,  277,   42,   43,
  378,   45,  282,   47,  408,  365,  410,  365,  354,  348,
   41,   41,   41,  277,  432,   40,   42,   43,  282,   45,
  277,   47,  368,   41,  428,  282,  444,   41,  446,  433,
   41,  277,   41,   59,  452,   41,  282,  455,  291,  457,
  458,  459,  460,   41,  462,   41,  277,   41, 1515,  302,
   41,  282,  470,  471,   41,   41,  309,  277,  462,  405,
   41,   41,  282,  481,  482,   41,  484,   42,   43,   59,
   45,  315,   47,  491,  492,  493,  494,  495,   40,  497,
  333,   44,  500,  501,  502,  503,  504,  443,  378,   41,
  508,  509,  348,  333,  318,  513,  514,  515,  516,  517,
  518,  519,  520,  340,  458,   40,   59,  331,  336,   46,
  500,  336,   40,   40,  376,   44,   41,   40,  402,  427,
   41, 1588,  376,   41,  277,   59,  376, 1594, 1595,  282,
  354,   44,  301,  298,  434,   41,  304,  301,  277,  290,
    0,  455,  395,  282,  368,  333,  376,  262,  409,  264,
  265, 1618,  376,   40,  376, 1622,  271,  264,  264, 1626,
  276,  450,  277,  317, 1631,  443, 1633,  282,  262,  277,
  285,  517,   44,  426,  282,  462,    0,  430,   40,  270,
  291,  405,  403,  277,  299,    0,  301,   41,  282,  304,
  305,  302,  270,  308,  264,  450,  428,  339,  309,   41,
  315,  427,  317,  318,  339,  450,  321,   41,   41,   59,
  325,  322,  327,   59,  329,   41,  331,   41,  262,  305,
   44,  336,  337,   41,  339,  301,   41,   42,   43,   44,
   45,   46,   47,  277,   59,   59,   59,   61,  282,  354,
  355,    0,  357,   59,   59,   60,   61,   62, 1715,    0,
    0,  277,  156,  368,  524,  370,  282,  372,  373,  374,
 1251,  376,  156,  526,  379,  380,  519,  773,  379,  493,
  764,  157, 1135,  301,  389,  675,   91,  300,   93, 1133,
  904,   92,  909,  866,  395,   78, 1125,  402, 1755, 1130,
  405, 1556,  880,  517,  367, 1557,  398,  624, 1061,  347,
  411, 1115,  277,  805, 1126,  379,  421,  282,  379, 1685,
  831, 1536,  427,  428, 1838,  426,  601,  432, 1785,  430,
 1842, 1538,  433, 1867, 1841, 1005,  822, 1755,  443,  444,
  441,  446, 1447, 1652, 1828, 1283,  753,  452, 1520,  372,
  455,  384,  457,  458,  459,  460,  365,  462,  883,  460,
  627,  603, 1336,   13,  546,  470,  471,  661,  875,  542,
  334,  794, 1829,  162,  783,   88,  481,  482, 1024,  484,
 1271, 1785,  708, 1609, 1403,  809,  491,  492,  493,  494,
  495,  557,  497,  771, 1017,  500,  501,  502,  503,  504,
 1175, 1291,   -1,  508,  509, 1221,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,  519,   -1,
   -1, 1373, 1374,   -1,   -1,   -1,   -1,   -1,   -1, 1381,
 1382,   -1, 1384, 1385,   -1,   -1,   -1,   -1,   -1,   -1,
 1392, 1393, 1394, 1395,  258, 1397, 1398, 1399, 1400, 1401,
 1402,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,  302,   -1,
  405,   -1,   -1,   -1,   -1,  309,   -1,   -1,   -1,  313,
  314,   -1,   -1,   -1,  428,   -1,  421,   -1,  322,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,  333,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,  481,  482,   44,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,  395,  497,   59,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,  411,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,  426,   -1,   -1,   -1,  430,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,  262,   -1,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  281,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,  519,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,
   44,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,  277,   -1,   -1,  428,  281,  421,   -1,  284,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  297,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,  372,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,  276,  277,   -1,   -1,  271,  433,   -1,   -1,
  284,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,
   -1,   -1,   -1,   -1,  389,  399,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  433,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,  281,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   58,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  262,   -1,  264,
  265,  491,  492,   -1,  494,  495,  271,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  282,  508,  509,
  285,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,  262,   -1,  264,
  265,  491,  492,   -1,  494,  495,  271,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  285,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,
  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,
   -1,  336,  337,   -1,  339,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   45,   -1,   -1,   -1,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  261,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,  279,   -1,  271,   -1,   -1,   -1,
   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,  294,
  285,  296,   -1,  298,    0,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   -1,  299,  310,   -1,   -1,   -1,  304,
  305,   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,  324,
  315,   -1,  317,  318,   -1,   -1,  321,   -1,   -1,  334,
  325,   -1,  327,   -1,  329,   41,  331,   43,   44,   45,
   -1,  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,
  355,   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,
   -1,  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,
  292,   -1,  397,   -1,  389,   -1,   -1,   -1,   -1,  404,
   -1,   -1,  407,   -1,  409,  410,   -1,  402,   -1,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,  434,
   -1,   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  443,  444,
   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,  483,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,   -1,   -1,  498,  499,   -1,  491,  492,  493,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,  419,  513,  514,
  515,  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
   -1,  453,  454,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  285,
   -1,   -1,   -1,    0,   -1,  487,  488,   -1,   -1,   -1,
   -1,   -1,   -1,  299,  496,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   43,   44,   45,   -1,
  336,  337,   -1,  339,  272,   -1,   -1,  275,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,
  288,  357,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,  335,   -1,  405,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,  367,
   -1,  369,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
  398,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,  437,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,  272,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,  288,
  357,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,  335,   -1,  405,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,
  369,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,  398,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  415,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,  437,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  489,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   42,   43,   -1,   45,  402,   47,   -1,  405,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,    0,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   59,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,    0,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   -1,  331,   -1,   -1,   -1,   -1,  336,  337,  271,
  339,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   41,
  282,   -1,   44,  285,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  321,
  389,   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,
   -1,   -1,   -1,  402,   -1,  337,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,  355,   -1,  357,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  370,   -1,
  372,  373,  374,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,  421,
   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
  432,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   91,
   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  354,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,  368,   -1,  470,  471,
   -1,   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,  389,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,  405,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,    0,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
  262,  513,  514,  515,  516,  517,  518,  519,  520,  271,
   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,   -1,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,
   -1,  272,   -1,  325,  275,  327,   -1,  329,   -1,  331,
   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   91,  290,
   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  357,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,   -1,
  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,  335,   -1,   -1,  389,   -1,   -1,
  341,  342,  343,    0,  345,  346,  347,   -1,  349,  271,
   -1,   -1,   -1,  405,   -1,  277,   -1,   -1,   -1,   -1,
  282,   -1,   -1,  285,   -1,   -1,  367,   -1,  369,  421,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,  444,   -1,  446,   -1,   -1,  398,   -1,  321,
   -1,   -1,   59,   60,   61,   62,   -1,  329,   -1,   -1,
   -1,   -1,   -1,   -1,  415,  337,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  355,   91,  357,  437,   -1,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,   -1,   -1,  370,   -1,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  513,   -1,   41,   -1,  517,   44,   -1,  271,   -1,
   -1,  472,  473,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   59,  285,   -1,   -1,   -1,   -1,  272,  489,   -1,
  275,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  421,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,   -1,   -1,
  295,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   41,  339,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  335,  354,  355,   59,  357,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,  368,   -1,  370,   -1,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,    0,   -1,
   -1,   -1,  367,   -1,  369,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,  271,   -1,   -1,   -1,  421,   41,
  277,   -1,   44,   -1,  427,  282,   -1,   -1,  285,  432,
  415,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  437,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,  262,   -1,   -1,  472,  473,   -1,
  493,   41,   -1,   -1,   44,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,  489,   -1,   -1,   -1,   -1,   59,
  513,  368,   -1,  370,  517,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,    0,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,  405,  327,
   -1,   -1,   -1,  331,   -1,   -1,  262,   -1,  336,   -1,
   -1,  339,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   41,  432,  354,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
  368,   -1,   59,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  389,  318,   -1,   -1,   -1,   -1,    0,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,  493,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,
   -1,   -1,   -1,   -1,  432,   -1,  513,   -1,   41,   -1,
  517,   44,  368,    0,   -1,   -1,  444,   -1,  446,   -1,
  376,   -1,   -1,  379,  380,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,
   -1,   -1,   -1,  325,   41,  327,   -1,   44,   -1,  331,
   -1,   -1,  262,   -1,  336,  493,   -1,  339,   -1,   -1,
   -1,  427,   59,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,  513,   -1,   -1,  444,  517,
  446,   -1,   -1,   -1,   -1,   -1,  368,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,   -1,   -1,  336,  493,   41,  339,
   -1,   44,   -1,    0,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  427,   59,  513,   -1,   -1,
  432,  517,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   59,   -1,   -1,  405,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,  493,  339,   -1,   -1,   -1,   -1,  427,   -1,  262,
   -1,   -1,  432,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,  513,   -1,   -1,  444,  517,  446,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  405,   -1,
   -1,   -1,  325,  493,  327,   -1,   -1,   -1,  331,   -1,
   -1,    0,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
  427,   -1,   -1,  513,   -1,  432,   -1,  517,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
   -1,  318,   -1,   -1,   -1,  368,   -1,   -1,  325,   -1,
  327,   -1,   41,  376,  331,   44,  379,  380,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,  389,   -1,   -1,  262,
   59,   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   -1,  405,    0,   -1,   -1,  493,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,  427,   -1,  513,   -1,   -1,  432,
  517,   -1,  389,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,  444,  315,  446,   41,  318,   -1,   44,  405,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   59,  336,   -1,   -1,  339,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,    0,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,  444,  315,  446,
  493,  318,   -1,   -1,   -1,  368,   -1,   -1,  325,   -1,
  327,   -1,   -1,  376,  331,   -1,  379,  380,   -1,  336,
  513,   -1,  339,   -1,  517,   -1,  389,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,  493,   59,    0,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,  427,   -1,  513,   -1,   -1,  432,
  517,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,    0,  446,   -1,   -1,   -1,   -1,  405,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   59,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,  444,   -1,  446,
  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  513,   -1,   -1,   -1,  517,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,   -1,  262,  493,  336,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  513,   -1,    0,   -1,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   -1,   -1,   -1,   -1,   -1,  325,   41,
  327,   -1,   44,   -1,  331,   -1,  405,   -1,   -1,  336,
  262,   -1,  339,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,    0,   -1,   -1,  444,   -1,  446,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  315,   -1,   -1,  318,   -1,   -1,   -1,
  262,   -1,   -1,  325,   -1,  327,   -1,   -1,  405,  331,
   -1,   -1,   -1,   41,  336,   -1,   44,  339,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  427,   59,  354,   -1,  262,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  513,   -1,  368,  444,  517,  446,
   -1,   -1,   -1,  315,  376,   -1,  318,  379,  380,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,  389,   -1,  331,
   -1,    0,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,   -1,   -1,  405,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,  354,   -1,   -1,   -1,  493,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  427,  368,   -1,  336,   -1,
  432,  339,   41,   -1,  376,   44,  513,  379,  380,   -1,
  517,   -1,  444,   -1,  446,   -1,  354,  389,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,  405,   -1,   -1,   -1,   -1,  376,   -1,
  262,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,  427,    0,   -1,   -1,   -1,
  432,  493,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,
   -1,  513,   -1,   -1,    0,  517,   -1,   -1,   -1,  427,
   -1,   -1,   -1,  315,  432,   -1,  318,   41,   -1,   -1,
   44,   -1,   -1,  325,   -1,  327,  444,   -1,  446,  331,
   -1,   -1,   -1,   -1,  336,   59,   -1,  339,   -1,   -1,
   -1,  493,   -1,   -1,  262,   41,   -1,   -1,   44,   -1,
   -1,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  513,   -1,   59,   -1,  517,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,  493,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  513,   -1,  315,   -1,  517,
  318,   -1,   -1,  405,   -1,   -1,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,    0,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,  444,  262,  446,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,  405,   -1,   -1,
   -1,  493,   -1,   -1,   -1,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,  427,
   -1,  513,  331,   -1,  432,  517,   -1,  336,    0,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,  368,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   41,
  379,  380,   44,   -1,   -1,   -1,  262,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,  493,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  405,   41,   -1,   -1,
   44,  315,   -1,   -1,  318,  513,   -1,   -1,   -1,  517,
   -1,  325,   -1,  327,   -1,   59,   -1,  331,  427,   -1,
   -1,   -1,  336,  432,   -1,  339,   -1,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,  444,   -1,  446,   -1,  325,
  354,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,    0,   -1,  339,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,    0,
   -1,   -1,  368,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  376,  405,   41,  379,  380,   44,  262,   -1,   -1,   -1,
   -1,   -1,   -1,  389,  513,   -1,   -1,   -1,  517,   -1,
   59,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,  405,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  444,  325,
  446,  327,   -1,  272,   -1,  331,  275,   -1,   -1,   -1,
  336,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  493,
   -1,   -1,   -1,  292,   -1,   -1,  295,   -1,  354,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  513,
   -1,   -1,  368,  517,   -1,   -1,   -1,  493,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,  335,  513,   -1,   -1,
   -1,  517,  341,  342,  343,   -1,  345,  346,  347,  405,
  349,   -1,   -1,  315,   -1,   -1,  318,   40,   -1,   -1,
   43,   -1,   45,  325,   -1,  327,   -1,   -1,  367,  331,
  369,  427,   -1,   -1,  336,   58,  432,  339,   -1,   -1,
   63,  315,   -1,   -1,  318,   -1,   -1,   -1,  444,   -1,
  446,  325,  354,  327,   -1,   -1,   -1,  331,   -1,  398,
   -1,   -1,  336,   -1,   -1,  339,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  415,  379,  380,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,  493,  437,   -1,
   -1,   -1,  376,  405,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,  513,   -1,   -1,
   -1,  517,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  432,  405,   -1,  472,  473,   -1,  315,   -1,   -1,  318,
   -1,   -1,  444,   -1,  446,   40,  325,   -1,  327,   -1,
  489,   -1,  331,  427,   -1,   -1,   -1,  336,  432,   -1,
  339,   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,
  444,   -1,  446,   -1,  325,  354,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,  368,
   -1,  493,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,
   -1,  380,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
  389,  513,   -1,   -1,   -1,  517,   -1,  368,   -1,  493,
   -1,   -1,   -1,   -1,   -1,  376,  405,   -1,   -1,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,  513,
   -1,   -1,   -1,  517,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,  432,  405,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,  444,   -1,  446,   -1,  292,
  293,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,  311,   58,
   -1,   -1,   -1,  444,   63,  446,   -1,  320,   -1,   -1,
  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,  513,   -1,   -1,  360,  517,   -1,
   -1,  364,  493,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  513,   -1,   -1,   -1,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   42,  409,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  505,  506,  507,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,
  289,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,  372,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  293,   -1,   -1,   42,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,  508,
  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  311,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,  372,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   44,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,  372,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   45,  317,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  320,   -1,   -1,  323,   63,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,  372,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   58,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  320,   -1,   -1,  323,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  450,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
  348,   -1,  350,  351,   -1,   -1,  470,  471,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   59,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   59,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,  360,   -1,   -1,   -1,  364,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,  360,   -1,   -1,   -1,  364,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,  348,   -1,  350,  351,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,  260,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  310,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,  334,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
  399,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  260,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  298,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  310,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,  334,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,  399,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  409,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,
   -1,  323,   63,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,  284,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,
   -1,   -1,   -1,  364,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,  260,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,  300,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,   -1,   -1,   -1,   -1,  267,  272,   -1,   -1,  275,
   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,  288,   -1,  286,   -1,  292,   -1,   -1,  295,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,  300,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,  403,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  437,   -1,  435,   -1,  325,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,  472,  473,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  267,   -1,  395,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  430,   -1,   -1,   -1,   -1,  435,   -1,   -1,   58,   -1,
   -1,  441,   -1,   63,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,
  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,  339,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,  362,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  419,
   -1,  508,  509,   -1,   -1,  425,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,   -1,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   40,   41,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,
  450,  451,  452,  453,  454,  455,   -1,  457,  458,  459,
  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   58,   59,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  446,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  267,
   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,  455,
   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,    0,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   40,   41,   -1,    0,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   59,   -1,   61,   -1,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,
   -1,   -1,   91,   -1,  348,   -1,  350,  351,   -1,   41,
   -1,   -1,   44,   59,   -1,   61,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,    0,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   41,  508,  509,   44,   -1,   -1,  258,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   59,   -1,   61,  273,   -1,  275,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
   -1,   -1,  258,   91,  303,   -1,   -1,  306,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  258,  273,   -1,  275,
  276,  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,
   -1,  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,
   -1,  297,  284,   -1,   -1,   -1,   -1,  303,   -1,  348,
  306,   41,   -1,   -1,   44,  297,   -1,   -1,   -1,   -1,
   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  393,   -1,   -1,   -1,   -1,   -1,
  399,   91,   -1,   -1,  403,   -1,  348,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  428,
  372,   -1,   -1,   -1,  433,   -1,   -1,  393,   -1,   59,
   -1,   61,   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,
   -1,  393,   -1,   -1,  410,   -1,   -1,  399,   -1,  458,
   -1,  403,   -1,  462,   -1,   -1,   -1,   -1,  410,   -1,
  258,   91,  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  273,  428,   -1,  276,  277,
   -1,  433,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,
   -1,   -1,  458,   -1,   -1,   -1,  462,   -1,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,  458,   -1,  306,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,  372,   -1,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  393,   -1,  297,   -1,   -1,
   -1,  399,   -1,  303,   -1,  403,  306,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,  276,  277,  348,   -1,
   -1,  281,   -1,   -1,  284,   -1,  270,   -1,   -1,   -1,
  458,   -1,   -1,   -1,  462,   -1,   -1,  297,   -1,   -1,
   -1,   -1,  372,  303,   -1,   -1,  306,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,
   -1,  305,   -1,  393,   -1,  309,  310,   -1,  312,  399,
   -1,  315,   -1,  403,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,   -1,  328,   -1,   -1,   -1,  348,   -1,
  334,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  270,   -1,   -1,   -1,  458,   -1,
   -1,   -1,  462,  393,   -1,   -1,   -1,   -1,   -1,  399,
   -1,   -1,   -1,  403,   -1,   -1,   -1,  391,   -1,   -1,
  410,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  309,  310,  409,  312,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
  424,   -1,  328,   -1,   -1,   -1,   -1,   -1,  334,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  443,
   -1,  445,  462,   -1,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,  391,   -1,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,  270,  409,  508,  509,  510,  511,  512,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,  424,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,
  298,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,  309,  310,   -1,  312,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
  328,   -1,   -1,   -1,  470,  471,  334,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  391,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,  272,   -1,   -1,  275,   -1,
   -1,   -1,  470,  471,  281,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  292,  484,   -1,  295,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,  369,  272,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
  437,   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,
  349,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,  367,   -1,
  369,   -1,   -1,  470,  471,  472,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,  398,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  415,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  452,   -1,   -1,  455,  456,  457,  458,  459,
  460,  461,  462,  463,  464,  465,  466,  467,  468,  469,
  470,  471,  472,  473,  474,  475,  476,   -1,   -1,  479,
  480,  481,  482,  483,  484,   -1,   -1,  487,  488,  489,
  490,  491,  492,  493,  494,  495,  496,  497,  498,  499,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,  513,  514,  515,  516,  517,  518,  519,
  520,  452,   -1,   -1,  455,  456,  457,  458,  459,  460,
   -1,  462,  463,  464,  465,  466,  467,  468,  469,  470,
  471,  472,  473,  474,  475,  476,   -1,   -1,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
  491,  492,  493,  494,  495,  496,  497,  498,  499,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,  518,  519,  520,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 520
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,0,0,0,0,0,0,0,
"':'","';'","'<'","'='","'>'","'?'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ACTIVE","ADD","AFTER","ALL","ALTER","AND","ANY",
"AS","ASC","AT","AVG","AUTO","BEFORE","BEGIN","BETWEEN","BLOB","BY","CAST",
"CHARACTER","CHECK","COLLATE","COMMA","COMMIT","COMMITTED","COMPUTED",
"CONCATENATE","CONDITIONAL","CONSTRAINT","CONTAINING","COUNT","CREATE",
"CSTRING","CURRENT","CURSOR","DATABASE","DATE","DB_KEY","KW_DEBUG","DECIMAL",
"DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO","DOMAIN","DROP","ELSE",
"END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE","EXISTS","EXIT",
"EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION","GDSCODE","GEQ",
"GENERATOR","GEN_ID","GRANT","GROUP","GTR","HAVING","IF","KW_IN","INACTIVE",
"INNER","INPUT_TYPE","INDEX","INSERT","INTEGER","INTO","IS","ISOLATION","JOIN",
"KEY","KW_CHAR","KW_DEC","KW_DOUBLE","KW_FILE","KW_FLOAT","KW_INT","KW_LONG",
"KW_NULL","KW_NUMERIC","KW_UPPER","KW_VALUE","LENGTH","LPAREN","LEFT","LEQ",
"LEVEL","LIKE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE",
"MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT",
"NOT_GTR","NOT_LSS","OF","ON","ONLY","OPTION","OR","ORDER","OUTER",
"OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","KW_PAGE_SIZE","PARAMETER","PASSWORD",
"PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY","PRIVILEGES","PROCEDURE",
"PROTECTED","READ","REAL","REFERENCES","RESERVING","RETAIN","RETURNING_VALUES",
"RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK","SEGMENT","SELECT","SET",
"SHADOW","KW_SHARED","SINGULAR","KW_SIZE","SMALLINT","SNAPSHOT","SOME","SORT",
"SQLCODE","STABILITY","STARTING","STATISTICS","SUB_TYPE","SUSPEND","SUM",
"TABLE","THEN","TO","TRANSACTION","TRIGGER","UNCOMMITTED","UNION","UNIQUE",
"UPDATE","USER","VALUES","VARCHAR","VARIABLE","VARYING","VERSION","VIEW","WAIT",
"WHEN","WHERE","WHILE","WITH","WORK","WRITE","FLOAT_NUMBER","NUMBER","NUMERIC",
"SYMBOL","STRING","INTRODUCER","ACTION","ADMIN","CASCADE","FREE_IT","RESTRICT",
"ROLE","COLUMN","TYPE","EXTRACT","YEAR","MONTH","DAY","HOUR","MINUTE","SECOND",
"WEEKDAY","YEARDAY","TIME","TIMESTAMP","CURRENT_DATE","CURRENT_TIME",
"CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT","CURRENT_USER","CURRENT_ROLE",
"KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR","FIRST","SKIP",
"CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE","NULLIF","COALESCE",
"USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT","RELEASE","STATEMENT",
"LEAVE","INSERTING","UPDATING","DELETING","KW_INSERTING","KW_UPDATING",
"KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH","ROWS","BLOCK",
"IIF","SCALAR_ARRAY","CROSS","NEXT","SEQUENCE","RESTART",
};
char *yyrule[] = {
"$accept : top",
"top : statement",
"top : statement ';'",
"statement : alter",
"statement : blob",
"statement : commit",
"statement : create",
"statement : declare",
"statement : delete",
"statement : drop",
"statement : grant",
"statement : insert",
"statement : invoke_procedure",
"statement : invoke_block",
"statement : recreate",
"statement : replace",
"statement : revoke",
"statement : rollback",
"statement : savepoint",
"statement : select",
"statement : set",
"statement : update",
"statement : KW_DEBUG signed_short_integer",
"grant : GRANT privileges ON table_noise simple_table_name TO non_role_grantee_list grant_option",
"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO non_role_grantee_list grant_option",
"grant : GRANT role_name_list TO role_grantee_list role_admin_option",
"table_noise : TABLE",
"table_noise :",
"privileges : ALL",
"privileges : ALL PRIVILEGES",
"privileges : privilege_list",
"privilege_list : privilege",
"privilege_list : privilege_list ',' privilege",
"proc_privileges : EXECUTE",
"privilege : SELECT",
"privilege : INSERT",
"privilege : KW_DELETE",
"privilege : UPDATE column_parens_opt",
"privilege : REFERENCES column_parens_opt",
"grant_option : WITH GRANT OPTION",
"grant_option :",
"role_admin_option : WITH ADMIN OPTION",
"role_admin_option :",
"simple_proc_name : symbol_procedure_name",
"revoke : REVOKE rev_grant_option privileges ON table_noise simple_table_name FROM non_role_grantee_list",
"revoke : REVOKE rev_grant_option proc_privileges ON PROCEDURE simple_proc_name FROM non_role_grantee_list",
"revoke : REVOKE rev_admin_option role_name_list FROM role_grantee_list",
"rev_grant_option : GRANT OPTION FOR",
"rev_grant_option :",
"rev_admin_option : ADMIN OPTION FOR",
"rev_admin_option :",
"non_role_grantee_list : grantee_list",
"non_role_grantee_list : user_grantee_list",
"grantee_list : grantee",
"grantee_list : grantee_list ',' grantee",
"grantee_list : grantee_list ',' user_grantee",
"grantee_list : user_grantee_list ',' grantee",
"grantee : PROCEDURE symbol_procedure_name",
"grantee : TRIGGER symbol_trigger_name",
"grantee : VIEW symbol_view_name",
"grantee : ROLE symbol_role_name",
"user_grantee_list : user_grantee",
"user_grantee_list : user_grantee_list ',' user_grantee",
"user_grantee : symbol_user_name",
"user_grantee : USER symbol_user_name",
"user_grantee : GROUP symbol_user_name",
"role_name_list : role_name",
"role_name_list : role_name_list ',' role_name",
"role_name : symbol_role_name",
"role_grantee_list : role_grantee",
"role_grantee_list : role_grantee_list ',' role_grantee",
"role_grantee : symbol_user_name",
"role_grantee : USER symbol_user_name",
"declare : DECLARE declare_clause",
"declare_clause : FILTER filter_decl_clause",
"declare_clause : EXTERNAL FUNCTION udf_decl_clause",
"udf_decl_clause : symbol_UDF_name arg_desc_list1 RETURNS return_value1 ENTRY_POINT sql_string MODULE_NAME sql_string",
"udf_data_type : simple_type",
"udf_data_type : BLOB",
"udf_data_type : CSTRING '(' pos_short_integer ')' charset_clause",
"arg_desc_list1 :",
"arg_desc_list1 : arg_desc_list",
"arg_desc_list1 : '(' arg_desc_list ')'",
"arg_desc_list : arg_desc",
"arg_desc_list : arg_desc_list ',' arg_desc",
"arg_desc : init_data_type udf_data_type param_mechanism",
"param_mechanism :",
"param_mechanism : BY KW_DESCRIPTOR",
"param_mechanism : BY SCALAR_ARRAY",
"param_mechanism : KW_NULL",
"return_value1 : return_value",
"return_value1 : '(' return_value ')'",
"return_value : init_data_type udf_data_type return_mechanism",
"return_value : PARAMETER pos_short_integer",
"return_mechanism :",
"return_mechanism : BY KW_VALUE",
"return_mechanism : BY KW_DESCRIPTOR",
"return_mechanism : FREE_IT",
"return_mechanism : BY KW_DESCRIPTOR FREE_IT",
"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_filter_subtype OUTPUT_TYPE blob_filter_subtype ENTRY_POINT sql_string MODULE_NAME sql_string",
"blob_filter_subtype : symbol_blob_subtype_name",
"blob_filter_subtype : signed_short_integer",
"create : CREATE create_clause",
"create_clause : EXCEPTION symbol_exception_name sql_string",
"create_clause : unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition",
"create_clause : PROCEDURE procedure_clause",
"create_clause : TABLE table_clause",
"create_clause : TRIGGER def_trigger_clause",
"create_clause : VIEW view_clause",
"create_clause : GENERATOR generator_clause",
"create_clause : SEQUENCE generator_clause",
"create_clause : DATABASE db_clause",
"create_clause : DOMAIN domain_clause",
"create_clause : SHADOW shadow_clause",
"create_clause : ROLE role_clause",
"recreate : RECREATE recreate_clause",
"recreate_clause : PROCEDURE rprocedure_clause",
"recreate_clause : TABLE rtable_clause",
"recreate_clause : VIEW rview_clause",
"replace : CREATE OR ALTER replace_clause",
"replace_clause : PROCEDURE replace_procedure_clause",
"replace_clause : TRIGGER replace_trigger_clause",
"unique_opt : UNIQUE",
"unique_opt :",
"index_definition : column_list",
"index_definition : column_parens",
"index_definition : computed_by '(' begin_trigger value end_trigger ')'",
"shadow_clause : pos_short_integer manual_auto conditional sql_string first_file_length sec_shadow_files",
"manual_auto : MANUAL",
"manual_auto : AUTO",
"manual_auto :",
"conditional :",
"conditional : CONDITIONAL",
"first_file_length :",
"first_file_length : LENGTH equals long_integer page_noise",
"sec_shadow_files :",
"sec_shadow_files : db_file_list",
"db_file_list : db_file",
"db_file_list : db_file_list db_file",
"domain_clause : column_def_name as_opt data_type begin_trigger domain_default_opt end_trigger domain_constraint_clause collate_clause",
"as_opt : AS",
"as_opt :",
"domain_default_opt : DEFAULT begin_trigger default_value",
"domain_default_opt :",
"domain_constraint_clause :",
"domain_constraint_clause : domain_constraint_list",
"domain_constraint_list : domain_constraint_def",
"domain_constraint_list : domain_constraint_list domain_constraint_def",
"domain_constraint_def : domain_constraint",
"domain_constraint : null_constraint",
"domain_constraint : domain_check_constraint",
"null_constraint : NOT KW_NULL",
"domain_check_constraint : begin_trigger CHECK '(' search_condition ')' end_trigger",
"generator_clause : symbol_generator_name",
"role_clause : symbol_role_name",
"db_clause : db_name db_initial_desc1 db_rem_desc1",
"equals :",
"equals : '='",
"db_name : sql_string",
"db_initial_desc1 :",
"db_initial_desc1 : db_initial_desc",
"db_initial_desc : db_initial_option",
"db_initial_desc : db_initial_desc db_initial_option",
"db_initial_option : KW_PAGE_SIZE equals pos_short_integer",
"db_initial_option : LENGTH equals long_integer page_noise",
"db_initial_option : USER sql_string",
"db_initial_option : PASSWORD sql_string",
"db_initial_option : SET NAMES sql_string",
"db_rem_desc1 :",
"db_rem_desc1 : db_rem_desc",
"db_rem_desc : db_rem_option",
"db_rem_desc : db_rem_desc db_rem_option",
"db_rem_option : db_file",
"db_rem_option : DEFAULT CHARACTER SET symbol_character_set_name",
"db_rem_option : KW_DIFFERENCE KW_FILE sql_string",
"db_file : file1 sql_string file_desc1",
"file1 : KW_FILE",
"file_desc1 :",
"file_desc1 : file_desc",
"file_desc : file_clause",
"file_desc : file_desc file_clause",
"file_clause : STARTING file_clause_noise long_integer",
"file_clause : LENGTH equals long_integer page_noise",
"file_clause_noise :",
"file_clause_noise : AT",
"file_clause_noise : AT PAGE",
"page_noise :",
"page_noise : PAGE",
"page_noise : PAGES",
"table_clause : simple_table_name external_file '(' table_elements ')'",
"rtable_clause : simple_table_name external_file '(' table_elements ')'",
"external_file : EXTERNAL KW_FILE sql_string",
"external_file : EXTERNAL sql_string",
"external_file :",
"table_elements : table_element",
"table_elements : table_elements ',' table_element",
"table_element : column_def",
"table_element : table_constraint_definition",
"column_def : column_def_name data_type_or_domain domain_default_opt end_trigger column_constraint_clause collate_clause",
"column_def : column_def_name non_array_type def_computed",
"column_def : column_def_name def_computed",
"def_computed : computed_by '(' begin_trigger value end_trigger ')'",
"computed_by : COMPUTED BY",
"computed_by : COMPUTED",
"data_type_or_domain : data_type begin_trigger",
"data_type_or_domain : simple_column_name begin_string",
"collate_clause : COLLATE symbol_collation_name",
"collate_clause :",
"column_def_name : simple_column_name",
"simple_column_def_name : simple_column_name",
"data_type_descriptor : init_data_type data_type",
"init_data_type :",
"default_opt : DEFAULT default_value",
"default_opt :",
"default_value : constant",
"default_value : current_user",
"default_value : current_role",
"default_value : internal_info",
"default_value : null_value",
"default_value : datetime_value_expression",
"column_constraint_clause :",
"column_constraint_clause : column_constraint_list",
"column_constraint_list : column_constraint_def",
"column_constraint_list : column_constraint_list column_constraint_def",
"column_constraint_def : constraint_name_opt column_constraint",
"column_constraint : NOT KW_NULL",
"column_constraint : REFERENCES simple_table_name column_parens_opt referential_trigger_action constraint_index_opt",
"column_constraint : check_constraint",
"column_constraint : UNIQUE constraint_index_opt",
"column_constraint : PRIMARY KEY constraint_index_opt",
"table_constraint_definition : constraint_name_opt table_constraint",
"constraint_name_opt : CONSTRAINT symbol_constraint_name",
"constraint_name_opt :",
"table_constraint : unique_constraint",
"table_constraint : primary_constraint",
"table_constraint : referential_constraint",
"table_constraint : check_constraint",
"unique_constraint : UNIQUE column_parens constraint_index_opt",
"primary_constraint : PRIMARY KEY column_parens constraint_index_opt",
"referential_constraint : FOREIGN KEY column_parens REFERENCES simple_table_name column_parens_opt referential_trigger_action constraint_index_opt",
"constraint_index_opt : USING order_direction INDEX symbol_index_name",
"constraint_index_opt :",
"check_constraint : begin_trigger CHECK '(' search_condition ')' end_trigger",
"referential_trigger_action : update_rule",
"referential_trigger_action : delete_rule",
"referential_trigger_action : delete_rule update_rule",
"referential_trigger_action : update_rule delete_rule",
"referential_trigger_action :",
"update_rule : ON UPDATE referential_action",
"delete_rule : ON KW_DELETE referential_action",
"referential_action : CASCADE",
"referential_action : SET DEFAULT",
"referential_action : SET KW_NULL",
"referential_action : NO ACTION",
"procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"rprocedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"replace_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"alter_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"input_parameters : '(' input_proc_parameters ')'",
"input_parameters :",
"output_parameters : RETURNS '(' output_proc_parameters ')'",
"output_parameters :",
"input_proc_parameters : input_proc_parameter",
"input_proc_parameters : input_proc_parameters ',' input_proc_parameter",
"input_proc_parameter : simple_column_def_name non_array_type default_par_opt end_trigger",
"output_proc_parameters : proc_parameter",
"output_proc_parameters : output_proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
"default_par_opt : DEFAULT begin_string default_value",
"default_par_opt : '=' begin_string default_value",
"default_par_opt : begin_string",
"local_declaration_list : local_declarations",
"local_declaration_list :",
"local_declarations : local_declaration",
"local_declarations : local_declarations local_declaration",
"local_declaration : DECLARE var_decl_opt local_declaration_item ';'",
"local_declaration_item : var_declaration_item",
"local_declaration_item : cursor_declaration_item",
"var_declaration_item : column_def_name non_array_type var_init_opt",
"var_decl_opt : VARIABLE",
"var_decl_opt :",
"var_init_opt : '=' default_value",
"var_init_opt : default_opt",
"cursor_declaration_item : symbol_cursor_name CURSOR FOR '(' select ')'",
"proc_block : proc_statement",
"proc_block : full_proc_block",
"full_proc_block : BEGIN full_proc_block_body END",
"full_proc_block_body : proc_statements",
"full_proc_block_body : proc_statements excp_hndl_statements",
"full_proc_block_body :",
"proc_statements : proc_block",
"proc_statements : proc_statements proc_block",
"proc_statement : assignment ';'",
"proc_statement : delete ';'",
"proc_statement : excp_statement",
"proc_statement : raise_statement",
"proc_statement : exec_procedure",
"proc_statement : exec_sql",
"proc_statement : for_select",
"proc_statement : if_then_else",
"proc_statement : insert ';'",
"proc_statement : post_event",
"proc_statement : singleton_select",
"proc_statement : update ';'",
"proc_statement : while",
"proc_statement : for_exec_into",
"proc_statement : exec_into",
"proc_statement : SUSPEND ';'",
"proc_statement : EXIT ';'",
"proc_statement : breakleave",
"proc_statement : cursor_statement ';'",
"proc_statement : exec_udf ';'",
"excp_statement : EXCEPTION symbol_exception_name ';'",
"excp_statement : EXCEPTION symbol_exception_name value ';'",
"raise_statement : EXCEPTION ';'",
"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs ';'",
"exec_sql : EXECUTE STATEMENT value ';'",
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list ';'",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt ';'",
"event_argument_opt :",
"singleton_select : select INTO variable_list ';'",
"variable : ':' symbol_variable_name",
"proc_inputs : value_list",
"proc_inputs : '(' value_list ')'",
"proc_inputs :",
"proc_outputs : RETURNING_VALUES variable_list",
"proc_outputs : RETURNING_VALUES '(' variable_list ')'",
"proc_outputs :",
"variable_list : variable",
"variable_list : column_name",
"variable_list : variable_list ',' column_name",
"variable_list : variable_list ',' variable",
"while : label_opt WHILE '(' search_condition ')' DO proc_block",
"label_opt : symbol_label_name ':'",
"label_opt :",
"breakleave : KW_BREAK ';'",
"breakleave : LEAVE ';'",
"breakleave : LEAVE symbol_label_name ';'",
"cursor_def : AS CURSOR symbol_cursor_name",
"cursor_def :",
"excp_hndl_statements : excp_hndl_statement",
"excp_hndl_statements : excp_hndl_statements excp_hndl_statement",
"excp_hndl_statement : WHEN errors DO proc_block",
"errors : err",
"errors : errors ',' err",
"err : SQLCODE signed_short_integer",
"err : GDSCODE symbol_gdscode_name",
"err : EXCEPTION symbol_exception_name",
"err : ANY",
"cursor_statement : open_cursor",
"cursor_statement : fetch_cursor",
"cursor_statement : close_cursor",
"open_cursor : OPEN symbol_cursor_name",
"close_cursor : CLOSE symbol_cursor_name",
"fetch_cursor : FETCH fetch_opt symbol_cursor_name INTO variable_list",
"fetch_opt :",
"invoke_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs",
"invoke_block : EXECUTE BLOCK block_input_params output_parameters AS local_declaration_list full_proc_block",
"block_input_params : '(' block_parameters ')'",
"block_input_params :",
"block_parameters : block_parameter",
"block_parameters : block_parameters ',' block_parameter",
"block_parameter : proc_parameter '=' parameter",
"view_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_string",
"rview_clause : symbol_view_name column_parens_opt AS begin_string select_expr check_opt end_string",
"begin_string :",
"end_string :",
"begin_trigger :",
"end_trigger :",
"check_opt : WITH CHECK OPTION",
"check_opt :",
"def_trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position begin_trigger trigger_action end_trigger",
"replace_trigger_clause : symbol_trigger_name FOR simple_table_name trigger_active trigger_type trigger_position begin_trigger trigger_action end_trigger",
"trigger_active : ACTIVE",
"trigger_active : INACTIVE",
"trigger_active :",
"trigger_type : trigger_type_prefix trigger_type_suffix",
"trigger_type_prefix : BEFORE",
"trigger_type_prefix : AFTER",
"trigger_type_suffix : INSERT",
"trigger_type_suffix : UPDATE",
"trigger_type_suffix : KW_DELETE",
"trigger_type_suffix : INSERT OR UPDATE",
"trigger_type_suffix : INSERT OR KW_DELETE",
"trigger_type_suffix : UPDATE OR INSERT",
"trigger_type_suffix : UPDATE OR KW_DELETE",
"trigger_type_suffix : KW_DELETE OR INSERT",
"trigger_type_suffix : KW_DELETE OR UPDATE",
"trigger_type_suffix : INSERT OR UPDATE OR KW_DELETE",
"trigger_type_suffix : INSERT OR KW_DELETE OR UPDATE",
"trigger_type_suffix : UPDATE OR INSERT OR KW_DELETE",
"trigger_type_suffix : UPDATE OR KW_DELETE OR INSERT",
"trigger_type_suffix : KW_DELETE OR INSERT OR UPDATE",
"trigger_type_suffix : KW_DELETE OR UPDATE OR INSERT",
"trigger_position : POSITION nonneg_short_integer",
"trigger_position :",
"trigger_action : AS begin_trigger local_declaration_list full_proc_block",
"alter : ALTER alter_clause",
"alter_clause : EXCEPTION symbol_exception_name sql_string",
"alter_clause : TABLE simple_table_name alter_ops",
"alter_clause : TRIGGER alter_trigger_clause",
"alter_clause : PROCEDURE alter_procedure_clause",
"alter_clause : DATABASE init_alter_db alter_db",
"alter_clause : DOMAIN alter_column_name alter_domain_ops",
"alter_clause : INDEX alter_index_clause",
"alter_clause : SEQUENCE alter_sequence_clause",
"domain_default_opt2 : DEFAULT begin_trigger default_value",
"domain_check_constraint2 : CHECK begin_trigger '(' search_condition ')' end_trigger",
"alter_domain_ops : alter_domain_op",
"alter_domain_ops : alter_domain_ops alter_domain_op",
"alter_domain_op : SET begin_trigger domain_default_opt2 end_trigger",
"alter_domain_op : ADD CONSTRAINT domain_check_constraint2",
"alter_domain_op : ADD domain_check_constraint",
"alter_domain_op : DROP DEFAULT",
"alter_domain_op : DROP CONSTRAINT",
"alter_domain_op : TO simple_column_name",
"alter_domain_op : TYPE init_data_type non_array_type",
"alter_ops : alter_op",
"alter_ops : alter_ops ',' alter_op",
"alter_op : DROP simple_column_name drop_behaviour",
"alter_op : DROP CONSTRAINT symbol_constraint_name",
"alter_op : ADD column_def",
"alter_op : ADD table_constraint_definition",
"alter_op : col_opt simple_column_name POSITION pos_short_integer",
"alter_op : col_opt alter_column_name TO simple_column_name",
"alter_op : col_opt alter_col_name TYPE alter_data_type_or_domain end_trigger",
"alter_column_name : keyword_or_column",
"keyword_or_column : valid_symbol_name",
"keyword_or_column : ADMIN",
"keyword_or_column : COLUMN",
"keyword_or_column : EXTRACT",
"keyword_or_column : YEAR",
"keyword_or_column : MONTH",
"keyword_or_column : DAY",
"keyword_or_column : HOUR",
"keyword_or_column : MINUTE",
"keyword_or_column : SECOND",
"keyword_or_column : TIME",
"keyword_or_column : TIMESTAMP",
"keyword_or_column : CURRENT_DATE",
"keyword_or_column : CURRENT_TIME",
"keyword_or_column : CURRENT_TIMESTAMP",
"keyword_or_column : CURRENT_USER",
"keyword_or_column : CURRENT_ROLE",
"keyword_or_column : RECREATE",
"keyword_or_column : CURRENT_CONNECTION",
"keyword_or_column : CURRENT_TRANSACTION",
"keyword_or_column : BIGINT",
"keyword_or_column : CASE",
"keyword_or_column : RELEASE",
"keyword_or_column : ROW_COUNT",
"keyword_or_column : SAVEPOINT",
"keyword_or_column : OPEN",
"keyword_or_column : CLOSE",
"keyword_or_column : FETCH",
"keyword_or_column : ROWS",
"keyword_or_column : USING",
"keyword_or_column : CROSS",
"col_opt : ALTER",
"col_opt : ALTER COLUMN",
"alter_data_type_or_domain : non_array_type begin_trigger",
"alter_data_type_or_domain : simple_column_name begin_string",
"alter_col_name : simple_column_name",
"drop_behaviour : RESTRICT",
"drop_behaviour : CASCADE",
"drop_behaviour :",
"alter_index_clause : symbol_index_name ACTIVE",
"alter_index_clause : symbol_index_name INACTIVE",
"alter_sequence_clause : symbol_generator_name RESTART WITH signed_long_integer",
"alter_sequence_clause : symbol_generator_name RESTART WITH NUMBER64BIT",
"alter_sequence_clause : symbol_generator_name RESTART WITH '-' NUMBER64BIT",
"init_alter_db :",
"alter_db : db_alter_clause",
"alter_db : alter_db db_alter_clause",
"db_alter_clause : ADD db_file_list",
"db_alter_clause : ADD KW_DIFFERENCE KW_FILE sql_string",
"db_alter_clause : DROP KW_DIFFERENCE KW_FILE",
"db_alter_clause : BEGIN BACKUP",
"db_alter_clause : END BACKUP",
"alter_trigger_clause : symbol_trigger_name trigger_active new_trigger_type trigger_position begin_trigger new_trigger_action end_trigger",
"new_trigger_type : trigger_type",
"new_trigger_type :",
"new_trigger_action : trigger_action",
"new_trigger_action :",
"drop : DROP drop_clause",
"drop_clause : EXCEPTION symbol_exception_name",
"drop_clause : INDEX symbol_index_name",
"drop_clause : PROCEDURE symbol_procedure_name",
"drop_clause : TABLE symbol_table_name",
"drop_clause : TRIGGER symbol_trigger_name",
"drop_clause : VIEW symbol_view_name",
"drop_clause : FILTER symbol_filter_name",
"drop_clause : DOMAIN symbol_domain_name",
"drop_clause : EXTERNAL FUNCTION symbol_UDF_name",
"drop_clause : SHADOW pos_short_integer",
"drop_clause : ROLE symbol_role_name",
"drop_clause : GENERATOR symbol_generator_name",
"drop_clause : SEQUENCE symbol_generator_name",
"data_type : non_array_type",
"data_type : array_type",
"non_array_type : simple_type",
"non_array_type : blob_type",
"array_type : non_charset_simple_type '[' array_spec ']'",
"array_type : character_type '[' array_spec ']' charset_clause",
"array_spec : array_range",
"array_spec : array_spec ',' array_range",
"array_range : signed_long_integer",
"array_range : signed_long_integer ':' signed_long_integer",
"simple_type : non_charset_simple_type",
"simple_type : character_type charset_clause",
"non_charset_simple_type : national_character_type",
"non_charset_simple_type : numeric_type",
"non_charset_simple_type : float_type",
"non_charset_simple_type : BIGINT",
"non_charset_simple_type : integer_keyword",
"non_charset_simple_type : SMALLINT",
"non_charset_simple_type : DATE",
"non_charset_simple_type : TIME",
"non_charset_simple_type : TIMESTAMP",
"integer_keyword : INTEGER",
"integer_keyword : KW_INT",
"blob_type : BLOB blob_subtype blob_segsize charset_clause",
"blob_type : BLOB '(' unsigned_short_integer ')'",
"blob_type : BLOB '(' unsigned_short_integer ',' signed_short_integer ')'",
"blob_type : BLOB '(' ',' signed_short_integer ')'",
"blob_segsize : SEGMENT KW_SIZE unsigned_short_integer",
"blob_segsize :",
"blob_subtype : SUB_TYPE signed_short_integer",
"blob_subtype : SUB_TYPE symbol_blob_subtype_name",
"blob_subtype :",
"charset_clause : CHARACTER SET symbol_character_set_name",
"charset_clause :",
"national_character_type : national_character_keyword '(' pos_short_integer ')'",
"national_character_type : national_character_keyword",
"national_character_type : national_character_keyword VARYING '(' pos_short_integer ')'",
"character_type : character_keyword '(' pos_short_integer ')'",
"character_type : character_keyword",
"character_type : varying_keyword '(' pos_short_integer ')'",
"varying_keyword : VARCHAR",
"varying_keyword : CHARACTER VARYING",
"varying_keyword : KW_CHAR VARYING",
"character_keyword : CHARACTER",
"character_keyword : KW_CHAR",
"national_character_keyword : NCHAR",
"national_character_keyword : NATIONAL CHARACTER",
"national_character_keyword : NATIONAL KW_CHAR",
"numeric_type : KW_NUMERIC prec_scale",
"numeric_type : decimal_keyword prec_scale",
"prec_scale :",
"prec_scale : '(' signed_long_integer ')'",
"prec_scale : '(' signed_long_integer ',' signed_long_integer ')'",
"decimal_keyword : DECIMAL",
"decimal_keyword : KW_DEC",
"float_type : KW_FLOAT precision_opt",
"float_type : KW_LONG KW_FLOAT precision_opt",
"float_type : REAL",
"float_type : KW_DOUBLE PRECISION",
"precision_opt : '(' nonneg_short_integer ')'",
"precision_opt :",
"set : set_transaction",
"set : set_generator",
"set : set_statistics",
"set_generator : SET GENERATOR symbol_generator_name TO signed_long_integer",
"set_generator : SET GENERATOR symbol_generator_name TO NUMBER64BIT",
"set_generator : SET GENERATOR symbol_generator_name TO '-' NUMBER64BIT",
"savepoint : set_savepoint",
"savepoint : release_savepoint",
"savepoint : undo_savepoint",
"set_savepoint : SAVEPOINT symbol_savepoint_name",
"release_savepoint : RELEASE SAVEPOINT symbol_savepoint_name release_only_opt",
"release_only_opt : ONLY",
"release_only_opt :",
"undo_savepoint : ROLLBACK optional_work TO optional_savepoint symbol_savepoint_name",
"optional_savepoint : SAVEPOINT",
"optional_savepoint :",
"commit : COMMIT optional_work optional_retain",
"rollback : ROLLBACK optional_work",
"optional_work : WORK",
"optional_work :",
"optional_retain : RETAIN opt_snapshot",
"optional_retain :",
"opt_snapshot : SNAPSHOT",
"opt_snapshot :",
"set_transaction : SET TRANSACTION tran_opt_list_m",
"tran_opt_list_m : tran_opt_list",
"tran_opt_list_m :",
"tran_opt_list : tran_opt",
"tran_opt_list : tran_opt_list tran_opt",
"tran_opt : access_mode",
"tran_opt : lock_wait",
"tran_opt : isolation_mode",
"tran_opt : tbl_reserve_options",
"access_mode : READ ONLY",
"access_mode : READ WRITE",
"lock_wait : WAIT",
"lock_wait : NO WAIT",
"isolation_mode : ISOLATION LEVEL iso_mode",
"isolation_mode : iso_mode",
"iso_mode : snap_shot",
"iso_mode : READ UNCOMMITTED version_mode",
"iso_mode : READ COMMITTED version_mode",
"snap_shot : SNAPSHOT",
"snap_shot : SNAPSHOT TABLE",
"snap_shot : SNAPSHOT TABLE STABILITY",
"version_mode : VERSION",
"version_mode : NO VERSION",
"version_mode :",
"tbl_reserve_options : RESERVING restr_list",
"lock_type : KW_SHARED",
"lock_type : PROTECTED",
"lock_type :",
"lock_mode : READ",
"lock_mode : WRITE",
"restr_list : restr_option",
"restr_list : restr_list ',' restr_option",
"restr_option : table_list table_lock",
"table_lock : FOR lock_type lock_mode",
"table_lock :",
"table_list : simple_table_name",
"table_list : table_list ',' simple_table_name",
"set_statistics : SET STATISTICS INDEX symbol_index_name",
"select : select_expr for_update_clause lock_clause",
"for_update_clause : FOR UPDATE for_update_list",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : select_expr_body order_clause rows_clause",
"column_select : select_expr_body order_clause rows_clause",
"column_singleton : select_expr_body order_clause rows_clause",
"select_expr_body : query_term",
"select_expr_body : select_expr_body UNION distinct_noise query_term",
"select_expr_body : select_expr_body UNION ALL query_term",
"query_term : query_spec",
"query_spec : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
"begin_limit :",
"end_limit :",
"begin_first :",
"end_first :",
"limit_clause : first_clause skip_clause end_limit",
"limit_clause : first_clause end_limit",
"limit_clause : skip_clause",
"limit_clause :",
"first_clause : FIRST long_integer begin_limit",
"first_clause : FIRST '(' value ')' begin_limit",
"first_clause : FIRST parameter begin_limit",
"skip_clause : SKIP long_integer",
"skip_clause : SKIP '(' end_limit value ')'",
"skip_clause : SKIP parameter",
"distinct_clause : DISTINCT",
"distinct_clause : all_noise",
"select_list : select_items",
"select_list : '*'",
"select_items : select_item",
"select_items : select_items ',' select_item",
"select_item : value",
"select_item : value as_noise symbol_item_alias_name",
"as_noise : AS",
"as_noise :",
"from_clause : FROM from_list",
"from_list : table_reference",
"from_list : from_list ',' table_reference",
"table_reference : joined_table",
"table_reference : table_primary",
"table_primary : table_proc",
"table_primary : derived_table",
"table_primary : '(' joined_table ')'",
"derived_table : '(' select_expr ')' as_noise correlation_name derived_column_list",
"correlation_name : symbol_table_alias_name",
"correlation_name :",
"derived_column_list : '(' alias_list ')'",
"derived_column_list :",
"alias_list : symbol_item_alias_name",
"alias_list : alias_list ',' symbol_item_alias_name",
"joined_table : cross_join",
"joined_table : natural_join",
"joined_table : qualified_join",
"cross_join : table_reference CROSS JOIN table_primary",
"natural_join : table_reference NATURAL join_type JOIN table_primary",
"qualified_join : table_reference join_type JOIN table_reference join_specification",
"join_specification : join_condition",
"join_specification : named_columns_join",
"join_condition : ON search_condition",
"named_columns_join : USING '(' column_list ')'",
"table_proc : symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name",
"table_proc : symbol_procedure_name table_proc_inputs",
"table_proc_inputs : '(' value_list ')'",
"table_proc_inputs :",
"table_name : simple_table_name",
"table_name : symbol_table_name symbol_table_alias_name",
"simple_table_name : symbol_table_name",
"join_type : INNER",
"join_type : LEFT outer_noise",
"join_type : RIGHT outer_noise",
"join_type : FULL outer_noise",
"join_type :",
"outer_noise : OUTER",
"outer_noise :",
"group_clause : GROUP BY group_by_list",
"group_clause :",
"group_by_list : group_by_item",
"group_by_list : group_by_list ',' group_by_item",
"group_by_item : value",
"having_clause : HAVING search_condition",
"having_clause :",
"where_clause : WHERE search_condition",
"where_clause :",
"plan_clause : PLAN plan_expression",
"plan_clause :",
"plan_expression : plan_type '(' plan_item_list ')'",
"plan_type : JOIN",
"plan_type : SORT MERGE",
"plan_type : MERGE",
"plan_type : SORT",
"plan_type :",
"plan_item_list : plan_item",
"plan_item_list : plan_item ',' plan_item_list",
"plan_item : table_or_alias_list access_type",
"plan_item : plan_expression",
"table_or_alias_list : symbol_table_name",
"table_or_alias_list : symbol_table_name table_or_alias_list",
"access_type : NATURAL",
"access_type : INDEX '(' index_list ')'",
"access_type : ORDER symbol_index_name extra_indices_opt",
"index_list : symbol_index_name",
"index_list : symbol_index_name ',' index_list",
"extra_indices_opt : INDEX '(' index_list ')'",
"extra_indices_opt :",
"order_clause : ORDER BY order_list",
"order_clause :",
"order_list : order_item",
"order_list : order_list ',' order_item",
"order_item : value order_direction nulls_clause",
"order_direction : ASC",
"order_direction : DESC",
"order_direction :",
"nulls_placement : FIRST",
"nulls_placement : LAST",
"nulls_clause : NULLS begin_first nulls_placement end_first",
"nulls_clause :",
"rows_clause : ROWS value",
"rows_clause : ROWS value TO value",
"rows_clause :",
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' value_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt select_expr",
"delete : delete_searched",
"delete : delete_positioned",
"delete_searched : KW_DELETE FROM table_name where_clause plan_clause order_clause rows_clause",
"delete_positioned : KW_DELETE FROM table_name cursor_clause",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause plan_clause order_clause rows_clause",
"update_positioned : UPDATE table_name SET assignments cursor_clause",
"assignments : assignment",
"assignments : assignments ',' assignment",
"assignment : update_column_name '=' value",
"exec_udf : udf",
"blob : READ BLOB simple_column_name FROM simple_table_name filter_clause segment_clause",
"blob : INSERT BLOB simple_column_name INTO simple_table_name filter_clause segment_clause",
"filter_clause : FILTER FROM blob_subtype_value TO blob_subtype_value",
"filter_clause : FILTER TO blob_subtype_value",
"filter_clause :",
"blob_subtype_value : blob_subtype",
"blob_subtype_value : parameter",
"blob_subtype : signed_short_integer",
"segment_clause : MAX_SEGMENT segment_length",
"segment_clause :",
"segment_length : unsigned_short_integer",
"segment_length : parameter",
"column_parens_opt : column_parens",
"column_parens_opt :",
"column_parens : '(' column_list ')'",
"column_list : simple_column_name",
"column_list : column_list ',' simple_column_name",
"ins_column_parens_opt : ins_column_parens",
"ins_column_parens_opt :",
"ins_column_parens : '(' ins_column_list ')'",
"ins_column_list : update_column_name",
"ins_column_list : ins_column_list ',' update_column_name",
"column_name : simple_column_name",
"column_name : symbol_table_alias_name '.' symbol_column_name",
"column_name : symbol_table_alias_name '.' '*'",
"simple_column_name : symbol_column_name",
"update_column_name : simple_column_name",
"update_column_name : symbol_table_alias_name '.' symbol_column_name",
"search_condition : trigger_action_predicate",
"search_condition : NOT trigger_action_predicate",
"search_condition : simple_search_condition",
"search_condition : search_condition OR search_condition",
"search_condition : search_condition AND search_condition",
"bracable_search_condition : simple_search_condition",
"bracable_search_condition : NOT trigger_action_predicate",
"bracable_search_condition : bracable_search_condition OR search_condition",
"bracable_search_condition : bracable_search_condition AND search_condition",
"bracable_search_condition : special_trigger_action_predicate OR search_condition",
"bracable_search_condition : special_trigger_action_predicate AND search_condition",
"simple_search_condition : predicate",
"simple_search_condition : '(' bracable_search_condition ')'",
"simple_search_condition : NOT simple_search_condition",
"predicate : comparison_predicate",
"predicate : distinct_predicate",
"predicate : between_predicate",
"predicate : like_predicate",
"predicate : in_predicate",
"predicate : null_predicate",
"predicate : quantified_predicate",
"predicate : exists_predicate",
"predicate : containing_predicate",
"predicate : starting_predicate",
"predicate : singular_predicate",
"comparison_predicate : value '=' value",
"comparison_predicate : value '<' value",
"comparison_predicate : value '>' value",
"comparison_predicate : value GEQ value",
"comparison_predicate : value LEQ value",
"comparison_predicate : value NOT_GTR value",
"comparison_predicate : value NOT_LSS value",
"comparison_predicate : value NEQ value",
"quantified_predicate : value '=' ALL '(' column_select ')'",
"quantified_predicate : value '<' ALL '(' column_select ')'",
"quantified_predicate : value '>' ALL '(' column_select ')'",
"quantified_predicate : value GEQ ALL '(' column_select ')'",
"quantified_predicate : value LEQ ALL '(' column_select ')'",
"quantified_predicate : value NOT_GTR ALL '(' column_select ')'",
"quantified_predicate : value NOT_LSS ALL '(' column_select ')'",
"quantified_predicate : value NEQ ALL '(' column_select ')'",
"quantified_predicate : value '=' some '(' column_select ')'",
"quantified_predicate : value '<' some '(' column_select ')'",
"quantified_predicate : value '>' some '(' column_select ')'",
"quantified_predicate : value GEQ some '(' column_select ')'",
"quantified_predicate : value LEQ some '(' column_select ')'",
"quantified_predicate : value NOT_GTR some '(' column_select ')'",
"quantified_predicate : value NOT_LSS some '(' column_select ')'",
"quantified_predicate : value NEQ some '(' column_select ')'",
"some : SOME",
"some : ANY",
"distinct_predicate : value IS DISTINCT FROM value",
"distinct_predicate : value IS NOT DISTINCT FROM value",
"between_predicate : value BETWEEN value AND value",
"between_predicate : value NOT BETWEEN value AND value",
"like_predicate : value LIKE value",
"like_predicate : value NOT LIKE value",
"like_predicate : value LIKE value ESCAPE value",
"like_predicate : value NOT LIKE value ESCAPE value",
"in_predicate : value KW_IN in_predicate_value",
"in_predicate : value NOT KW_IN in_predicate_value",
"containing_predicate : value CONTAINING value",
"containing_predicate : value NOT CONTAINING value",
"starting_predicate : value STARTING value",
"starting_predicate : value NOT STARTING value",
"starting_predicate : value STARTING WITH value",
"starting_predicate : value NOT STARTING WITH value",
"exists_predicate : EXISTS '(' select_expr ')'",
"singular_predicate : SINGULAR '(' select_expr ')'",
"null_predicate : value IS KW_NULL",
"null_predicate : value IS NOT KW_NULL",
"trigger_action_predicate : INSERTING",
"trigger_action_predicate : UPDATING",
"trigger_action_predicate : DELETING",
"special_trigger_action_predicate : KW_INSERTING",
"special_trigger_action_predicate : KW_UPDATING",
"special_trigger_action_predicate : KW_DELETING",
"in_predicate_value : table_subquery",
"in_predicate_value : '(' value_list ')'",
"table_subquery : '(' column_select ')'",
"value : column_name",
"value : array_element",
"value : function",
"value : u_constant",
"value : parameter",
"value : variable",
"value : cast_specification",
"value : case_expression",
"value : next_value_expression",
"value : udf",
"value : '-' value",
"value : '+' value",
"value : value '+' value",
"value : value CONCATENATE value",
"value : value COLLATE symbol_collation_name",
"value : value '-' value",
"value : value '*' value",
"value : value '/' value",
"value : '(' value ')'",
"value : '(' column_singleton ')'",
"value : current_user",
"value : current_role",
"value : internal_info",
"value : DB_KEY",
"value : symbol_table_alias_name '.' DB_KEY",
"value : KW_VALUE",
"value : datetime_value_expression",
"value : null_value",
"datetime_value_expression : CURRENT_DATE",
"datetime_value_expression : CURRENT_TIME",
"datetime_value_expression : CURRENT_TIMESTAMP",
"array_element : column_name '[' value_list ']'",
"value_list : value",
"value_list : value_list ',' value",
"constant : u_constant",
"constant : '-' u_numeric_constant",
"u_numeric_constant : NUMERIC",
"u_numeric_constant : NUMBER",
"u_numeric_constant : FLOAT_NUMBER",
"u_numeric_constant : NUMBER64BIT",
"u_numeric_constant : SCALEDINT",
"u_constant : u_numeric_constant",
"u_constant : sql_string",
"u_constant : DATE STRING",
"u_constant : TIME STRING",
"u_constant : TIMESTAMP STRING",
"parameter : '?'",
"current_user : USER",
"current_user : CURRENT_USER",
"current_role : CURRENT_ROLE",
"internal_info : CURRENT_CONNECTION",
"internal_info : CURRENT_TRANSACTION",
"internal_info : GDSCODE",
"internal_info : SQLCODE",
"internal_info : ROW_COUNT",
"sql_string : STRING",
"sql_string : INTRODUCER STRING",
"signed_short_integer : nonneg_short_integer",
"signed_short_integer : '-' neg_short_integer",
"nonneg_short_integer : NUMBER",
"neg_short_integer : NUMBER",
"pos_short_integer : nonneg_short_integer",
"unsigned_short_integer : NUMBER",
"signed_long_integer : long_integer",
"signed_long_integer : '-' long_integer",
"long_integer : NUMBER",
"function : aggregate_function",
"function : numeric_value_function",
"function : string_value_function",
"aggregate_function : COUNT '(' '*' ')'",
"aggregate_function : COUNT '(' all_noise value ')'",
"aggregate_function : COUNT '(' DISTINCT value ')'",
"aggregate_function : SUM '(' all_noise value ')'",
"aggregate_function : SUM '(' DISTINCT value ')'",
"aggregate_function : AVG '(' all_noise value ')'",
"aggregate_function : AVG '(' DISTINCT value ')'",
"aggregate_function : MINIMUM '(' all_noise value ')'",
"aggregate_function : MINIMUM '(' DISTINCT value ')'",
"aggregate_function : MAXIMUM '(' all_noise value ')'",
"aggregate_function : MAXIMUM '(' DISTINCT value ')'",
"numeric_value_function : extract_expression",
"extract_expression : EXTRACT '(' timestamp_part FROM value ')'",
"string_value_function : substring_function",
"string_value_function : KW_UPPER '(' value ')'",
"substring_function : SUBSTRING '(' value FROM value string_length_opt ')'",
"string_length_opt : FOR value",
"string_length_opt :",
"udf : symbol_UDF_name '(' value_list ')'",
"udf : symbol_UDF_name '(' ')'",
"cast_specification : CAST '(' value AS data_type_descriptor ')'",
"case_expression : case_abbreviation",
"case_expression : case_specification",
"case_abbreviation : NULLIF '(' value ',' value ')'",
"case_abbreviation : IIF '(' search_condition ',' value ',' value ')'",
"case_abbreviation : COALESCE '(' value ',' value_list ')'",
"case_specification : simple_case",
"case_specification : searched_case",
"simple_case : CASE case_operand simple_when_clause END",
"simple_case : CASE case_operand simple_when_clause ELSE case_result END",
"simple_when_clause : WHEN when_operand THEN case_result",
"simple_when_clause : simple_when_clause WHEN when_operand THEN case_result",
"searched_case : CASE searched_when_clause END",
"searched_case : CASE searched_when_clause ELSE case_result END",
"searched_when_clause : WHEN search_condition THEN case_result",
"searched_when_clause : searched_when_clause WHEN search_condition THEN case_result",
"when_operand : value",
"case_operand : value",
"case_result : value",
"next_value_expression : NEXT KW_VALUE FOR symbol_generator_name",
"next_value_expression : GEN_ID '(' symbol_generator_name ',' value ')'",
"timestamp_part : YEAR",
"timestamp_part : MONTH",
"timestamp_part : DAY",
"timestamp_part : HOUR",
"timestamp_part : MINUTE",
"timestamp_part : SECOND",
"timestamp_part : WEEKDAY",
"timestamp_part : YEARDAY",
"all_noise : ALL",
"all_noise :",
"distinct_noise : DISTINCT",
"distinct_noise :",
"null_value : KW_NULL",
"symbol_UDF_name : SYMBOL",
"symbol_blob_subtype_name : valid_symbol_name",
"symbol_character_set_name : valid_symbol_name",
"symbol_collation_name : valid_symbol_name",
"symbol_column_name : valid_symbol_name",
"symbol_constraint_name : valid_symbol_name",
"symbol_cursor_name : valid_symbol_name",
"symbol_domain_name : valid_symbol_name",
"symbol_exception_name : valid_symbol_name",
"symbol_filter_name : valid_symbol_name",
"symbol_gdscode_name : valid_symbol_name",
"symbol_generator_name : valid_symbol_name",
"symbol_index_name : valid_symbol_name",
"symbol_item_alias_name : valid_symbol_name",
"symbol_label_name : valid_symbol_name",
"symbol_procedure_name : valid_symbol_name",
"symbol_role_name : valid_symbol_name",
"symbol_table_alias_name : valid_symbol_name",
"symbol_table_name : valid_symbol_name",
"symbol_trigger_name : valid_symbol_name",
"symbol_user_name : valid_symbol_name",
"symbol_variable_name : valid_symbol_name",
"symbol_view_name : valid_symbol_name",
"symbol_savepoint_name : valid_symbol_name",
"valid_symbol_name : SYMBOL",
"valid_symbol_name : non_reserved_word",
"non_reserved_word : ACTION",
"non_reserved_word : CASCADE",
"non_reserved_word : FREE_IT",
"non_reserved_word : RESTRICT",
"non_reserved_word : ROLE",
"non_reserved_word : TYPE",
"non_reserved_word : KW_BREAK",
"non_reserved_word : KW_DESCRIPTOR",
"non_reserved_word : SUBSTRING",
"non_reserved_word : COALESCE",
"non_reserved_word : LAST",
"non_reserved_word : LEAVE",
"non_reserved_word : LOCK",
"non_reserved_word : NULLIF",
"non_reserved_word : NULLS",
"non_reserved_word : STATEMENT",
"non_reserved_word : INSERTING",
"non_reserved_word : UPDATING",
"non_reserved_word : DELETING",
"non_reserved_word : BLOCK",
"non_reserved_word : BACKUP",
"non_reserved_word : KW_DIFFERENCE",
"non_reserved_word : IIF",
"non_reserved_word : SCALAR_ARRAY",
"non_reserved_word : WEEKDAY",
"non_reserved_word : YEARDAY",
"non_reserved_word : SEQUENCE",
"non_reserved_word : NEXT",
"non_reserved_word : RESTART",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
static int yynerrs;

int DSQL_yyerrflag;


int DSQL_yychar;

short *DSQL_DSQL_yyssp;
static YYSTYPE *yyvsp;
static YYSTYPE yyval;
static YYSTYPE yylval;
static short DSQL_yyss[YYSTACKSIZE];
static YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE


/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		lex.c
 *	DESCRIPTION:	Lexical routine
 *
 */


void LEX_dsql_init (void)
{
/**************************************
 *
 *	L E X _ d s q l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize LEX for processing.  This is called only once
 *	per session.
 *
 **************************************/
	for (const TOK* token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		DSQL_SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) dsql_sym;
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		dsql_str* str = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) dsql_str;
		str->str_length = symbol->sym_length;
		strncpy((char*)str->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str;
		HSHD_insert(symbol);
	}
}


void LEX_string (
	const TEXT* string,
	USHORT	length,
	SSHORT	character_set)
{
/**************************************
 *
 *	L E X _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Initialize LEX to process a string.
 *
 **************************************/

	lex.line_start = lex.ptr = string;
	lex.end = string + length;
	lex.lines = 1;
	lex.att_charset = character_set;
	lex.line_start_bk = lex.line_start;
	lex.lines_bk = lex.lines;
	lex.param_number = 1;
	lex.prev_keyword = -1;
	lex.prev_prev_keyword = -1;
	lex.limit_clause = false;	
	lex.first_detection = false;
	lex.brace_analysis = false;
#ifdef DSQL_DEBUG
	if (DSQL_debug & 32)
		dsql_trace("Source DSQL string:\n%.*s", (int)length, string);
#endif
}


static const TEXT* lex_position (void)
{
/**************************************
 *
 *	l e x _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Return the current position of LEX 
 *	in the input string.
 *
 **************************************/

	return lex.ptr;
}


#ifdef NOT_USED_OR_REPLACED
static bool long_int(dsql_nod* string,
					 SLONG *long_value)
{
/*************************************
 *
 *	l o n g _ i n t
 * 
 *************************************
 *
 * Functional description
 *	checks for all digits in the
 *	number and return an atol().
 *
 *************************************/

	for (const UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; 
		 classes[*p] & CHR_DIGIT; p++)
	{
		if (!(classes[*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol(((dsql_str*) string)->str_data);

	return true;
}
#endif

static dsql_fld* make_field (dsql_nod* field_name)
{
/**************************************
 *
 *	m a k e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Make a field block of given name.
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();

	if (field_name == NULL)
	{
		dsql_fld* field =
			FB_NEW_RPT(*tdsql->getDefaultPool(), sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, INTERNAL_FIELD_NAME);
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field =
		FB_NEW_RPT(*tdsql->getDefaultPool(), strlen ((SCHAR*) string->str_data)) dsql_fld;
	strcpy (field->fld_name, (TEXT*) string->str_data);

	return field;
}


static dsql_fil* make_file()
{
/**************************************
 *
 *	m a k e _ f i l e 
 *
 **************************************
 *
 * Functional description
 *	Make a file block
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();
		   
	dsql_fil* temp_file = FB_NEW(*tdsql->getDefaultPool()) dsql_fil;

	return temp_file;
}


static dsql_nod* make_list (dsql_nod* node)
{
/**************************************
 *
 *	m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Collapse nested list nodes into single list.
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();

	if (node)
	{
		DsqlNodStack stack;
		stack_nodes(node, stack);
		USHORT l = stack.getCount();

		dsql_nod* old = node;
		node = FB_NEW_RPT(*tdsql->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		if (MemoryPool::blk_type(old) == dsql_type_nod)
		{
			node->nod_flags = old->nod_flags;
		}
		dsql_nod** ptr = node->nod_arg + node->nod_count;

		while (stack.hasData())
			*--ptr = stack.pop();
	}

	return node;
}


static dsql_nod* make_parameter (void)
{
/**************************************
 *
 *	m a k e _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *	Make parameter node
 *	Any change should also be made to function below
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), 1) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = 1;
	node->nod_arg[0] = (dsql_nod*)(IPTR) lex.param_number++;

	return node;
}


static dsql_nod* make_node (NOD_TYPE	type,
						   int count,
						   ...)
{
/**************************************
 *
 *	m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type.
 *	Any change should also be made to function below
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


static dsql_nod* make_flag_node (NOD_TYPE	type,
								SSHORT	flag,
								int		count,
								...)
{
/**************************************
 *
 *	m a k e _ f l a g _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type. Set flag field
 *
 **************************************/
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


static void prepare_console_debug (int level, int *yydeb)
{
/*************************************
 *
 *	p r e p a r e _ c o n s o l e _ d e b u g
 * 
 *************************************
 *
 * Functional description
 *	Activate debug info. In WinNT, redirect the standard
 *	output so one can see the generated information.
 *	Feel free to add your platform specific code.
 *
 *************************************/
#ifdef DSQL_DEBUG
	DSQL_debug = level;
#endif
	if (level >> 8)
		*yydeb = level >> 8;
}

#ifdef NOT_USED_OR_REPLACED
static bool short_int(dsql_nod* string,
					  SLONG *long_value,
					  SSHORT range)
{
/*************************************
 *
 *	s h o r t _ i n t
 * 
 *************************************
 *
 * Functional description
 *	is the string a valid representation 
 *	of a positive short int?
 *
 *************************************/

	if (((dsql_str*) string)->str_length > 5) {
		return false;
	}

	for (UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; 
		classes[*p] & CHR_DIGIT; p++)
	{
		if (!(classes[*p] & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];	
	buf[0] = ((dsql_str*) string)->str_data[0];
	buf[1] = ((dsql_str*) string)->str_data[1];
	buf[2] = ((dsql_str*) string)->str_data[2];
	buf[3] = ((dsql_str*) string)->str_data[3];
	buf[4] = ((dsql_str*) string)->str_data[4];
	buf[5] = '\0';

	*long_value = atoi (buf);

	bool return_value;

	switch (range) 
	{
		case POSITIVE:
			return_value = *long_value > SHRT_POS_MAX;
			break;
		case NEGATIVE:
			return_value = *long_value > SHRT_NEG_MAX;
			break;
		case UNSIGNED:
			return_value = *long_value > SHRT_UNSIGNED_MAX;
			break;
	}
	return !return_value;
}
#endif

static void stack_nodes (dsql_nod*	node,
						 DsqlNodStack& stack)
{
/**************************************
 *
 *	s t a c k _ n o d e s
 *
 **************************************
 *
 * Functional description
 *	Assist in turning a tree of misc nodes into a clean list.
 *
 **************************************/
	if (node->nod_type != nod_list)
	{
		stack.push(node);
		return;
	}

	/* To take care of cases where long lists of nodes are in a chain
	   of list nodes with exactly one entry, this algorithm will look
	   for a pattern of repeated list nodes with two entries, the first
	   being a list node and the second being a non-list node.   Such
	   a list will be reverse linked, and then re-reversed, stacking the
	   non-list nodes in the process.   The purpose of this is to avoid
	   massive recursion of this function. */

	dsql_nod* start_chain = node;
	dsql_nod* end_chain = NULL;
	dsql_nod* curr_node = node;
	dsql_nod* next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
	{

		/* pattern was found so reverse the links and go to next node */

		dsql_nod* save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
	}

	/* see if any chain was found */

	if (end_chain)
	{

		/* first, handle the rest of the nodes */
		/* note that next_node still points to the first non-pattern node */

		stack_nodes (next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */
		
		curr_node = end_chain;
		while (true)
		{
			stack.push(curr_node->nod_arg[1]);
			if ( curr_node == start_chain)
				break;
			dsql_nod* save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
		}
		return;
	}

	dsql_nod** ptr = node->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

inline static int yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	bool* stmt_ambiguous)
{
	const int temp =
		lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
}

int LexerState::yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	bool* stmt_ambiguous)
{
/**************************************
 *
 *	y y l e x
 *
 **************************************
 *
 * Functional description: lexer.
 *
 **************************************/
	UCHAR	tok_class;
	char  string[MAX_TOKEN_LEN];
	SSHORT	c;

	/* Find end of white space and skip comments */

	for (;;)
	{
		if (ptr >= end)
			return -1;

		c = *ptr++;

		/* Process comments */

		if (c == '\n') {
			lines++;
			line_start = ptr;
			continue;
		}

		if ((c == '-') && (*ptr == '-')) {
			
			/* single-line */
			
			ptr++;
			while (ptr < end) {
				if ((c = *ptr++) == '\n') {
					lines++;
					line_start = ptr /* + 1*/; /* CVC: +1 left out. */
					break;
				}
			}
			if (ptr >= end)
				return -1;
			continue;
		}
		else if ((c == '/') && (*ptr == '*')) {
			
			/* multi-line */
			
			ptr++;
			while (ptr < end) {
				if ((c = *ptr++) == '*') {
					if (*ptr == '/')
						break;
				}
				if (c == '\n') {
					lines++;
					line_start = ptr /* + 1*/; /* CVC: +1 left out. */

				}
			}
			if (ptr >= end)
				return -1;
			ptr++;
			continue;
		}

		tok_class = classes[c];

		if (!(tok_class & CHR_WHITE))
		break;
	}

	/* Depending on tok_class of token, parse token */

	last_token = ptr - 1;

	if (tok_class & CHR_INTRODUCER)
	{
		/* The Introducer (_) is skipped, all other idents are copied
		 * to become the name of the character set
		 */
		char* p = string;
		for (; ptr < end && classes[static_cast<UCHAR>(*ptr)] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER7(*ptr), string);
		}
		
		check_bound(p, string);
		*p = 0;

		/* make a string value to hold the name, the name 
		 * is resolved in pass1_constant */

		yylval = (dsql_nod*) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
	}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
	{
		char* buffer = string;
		size_t buffer_len = sizeof (string);
		const char* buffer_end = buffer + buffer_len - 1;
		char* p;
		for (p = buffer; ; ++p)
		{
			if (ptr >= end)
			{
				if (buffer != string)
					gds__free (buffer);
				return -1;
			}
			// Care about multi-line constants and identifiers
			if (*ptr == '\n') {
				lines++;
				line_start = ptr + 1;
			}
			/* *ptr is quote - if next != quote we're at the end */
			if ((*ptr == c) && ((++ptr == end) || (*ptr != c)))
				break;
			if (p > buffer_end)
			{
				char* const new_buffer = (char*) gds__alloc (2 * buffer_len);
			/* FREE: at outer block */
				if (!new_buffer)		/* NOMEM: */
				{
					if (buffer != string)
						gds__free (buffer);
					return -1;
				}
				memcpy (new_buffer, buffer, buffer_len);
				if (buffer != string)
					gds__free (buffer);
				buffer = new_buffer;
				p = buffer + buffer_len;
				buffer_len = 2 * buffer_len;
				buffer_end = buffer + buffer_len - 1;
			}
			*p = *ptr++;
		}
		if (c == '"')
		{
			*stmt_ambiguous = true; /* string delimited by double quotes could be
					**   either a string constant or a SQL delimited
					**   identifier, therefore marks the SQL
					**   statement as ambiguous  */
			if (client_dialect == SQL_DIALECT_V6_TRANSITION)
			{
				if (buffer != string)
					gds__free (buffer);
				yyabandon (-104, isc_invalid_string_constant);
			}
			else if (client_dialect >= SQL_DIALECT_V6)
			{
				if ((p - buffer) >= MAX_TOKEN_LEN)
				{
					if (buffer != string)
						gds__free (buffer);
					yyabandon (-104, isc_token_too_long);
				}
				yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
				dsql_str* delimited_id_str = (dsql_str*) yylval;
				delimited_id_str->str_flags |= STR_delimited_id;
				if (buffer != string)
					gds__free (buffer);
				return SYMBOL;
			}
		}
		yylval = (dsql_nod*) MAKE_string(buffer, p - buffer);
		if (buffer != string)
		gds__free (buffer);
		return STRING;
	}
												 
/* 
 * Check for a numeric constant, which starts either with a digit or with
 * a decimal point followed by a digit.
 * 
 * This code recognizes the following token types:
 * 
 * NUMBER: string of digits which fits into a 32-bit integer
 * 
 * NUMBER64BIT: string of digits whose value might fit into an SINT64,
 *   depending on whether or not there is a preceding '-', which is to
 *   say that "9223372036854775808" is accepted here.
 *
 * SCALEDINT: string of digits and a single '.', where the digits
 *   represent a value which might fit into an SINT64, depending on
 *   whether or not there is a preceding '-'.
 *
 * FLOAT: string of digits with an optional '.', and followed by an "e"
 *   or "E" and an optionally-signed exponent.
 *
 * NOTE: we swallow leading or trailing blanks, but we do NOT accept
 *   embedded blanks:
 *
 * Another note: c is the first character which need to be considered,
 *   ptr points to the next character.
 */

	fb_assert(ptr <= end);

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (ptr < end) && (classes[static_cast<UCHAR>(*ptr)] & CHR_DIGIT)))
	{
		/* The following variables are used to recognize kinds of numbers. */

		bool have_error	 = false;	/* syntax error or value too large */
		bool have_digit	 = false;	/* we've seen a digit			  */
		bool have_decimal   = false;	/* we've seen a '.'				*/
		bool have_exp	   = false;	/* digit ... [eE]				  */
		bool have_exp_sign  = false; /* digit ... [eE] {+-]			 */
		bool have_exp_digit = false; /* digit ... [eE] ... digit		*/
		UINT64	number		 = 0;
		UINT64	limit_by_10	= MAX_SINT64 / 10;

		for (--ptr ; ptr < end ; ptr++)
		{
			c = *ptr;
			if (have_exp_digit && (! (classes[c]  & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes[c]  & CHR_DIGIT)))
			{
				/* only digits can be accepted after "1E-" */
				have_error = true;
				break;
			}
			else if (have_exp)
			{
				/* We've seen e or E, but nothing beyond that. */
				if ( ('-' == c) || ('+' == c) )
					have_exp_sign = true;
				else if ( classes[c]  & CHR_DIGIT )
					/* We have a digit: we haven't seen a sign yet,
					but it's too late now. */
					have_exp_digit = have_exp_sign  = true;
				else
				{
					/* end of the token */
					have_error = true;
					break;
				}
			}
			else if ('.' == c)
			{
				if (!have_decimal)
					have_decimal = true;
				else
				{
					have_error = true;
					break;
				}
			}
			else if (classes[c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				{
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
					}
				}
				number = number * 10 + (c - '0');
			}
			else if ( (('E' == c) || ('e' == c)) && have_digit )
				have_exp = true;
			else
				/* Unexpected character: this is the end of the number. */
				break;
		}

		/* We're done scanning the characters: now return the right kind
		   of number token, if any fits the bill. */

		if (!have_error)
		{
			fb_assert(have_digit);

			if (have_exp_digit)
			{
				yylval = (dsql_nod*) MAKE_string(last_token, ptr - last_token);
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;

				return FLOAT_NUMBER;
			}
			else if (!have_exp)
			{

				/* We should return some kind (scaled-) integer type
				   except perhaps in dialect 1. */

				if (!have_decimal && (number <= MAX_SLONG))
				{
					yylval = (dsql_nod*) (IPTR) number;
					return NUMBER;
				}
				else
				{
					/* We have either a decimal point with no exponent
					   or a string of digits whose value exceeds MAX_SLONG:
					   the returned type depends on the client dialect,
					   so warn of the difference if the client dialect is
					   SQL_DIALECT_V6_TRANSITION.
					*/

					if (SQL_DIALECT_V6_TRANSITION == client_dialect)
					{
						/* Issue a warning about the ambiguity of the numeric
						 * numeric literal.  There are multiple calls because
						 * the message text exceeds the 119-character limit
						 * of our message database.
						 */
						ERRD_post_warning( isc_dsql_warning_number_ambiguous,
							   isc_arg_string,
							   ERR_string( last_token, ptr - last_token ),
							   isc_arg_end );
						ERRD_post_warning( isc_dsql_warning_number_ambiguous1,
							   isc_arg_end );
					}

					yylval = (dsql_nod*) MAKE_string(last_token, ptr - last_token);

					last_token_bk = last_token;
					line_start_bk = line_start;
					lines_bk = lines;

					if (client_dialect < SQL_DIALECT_V6_TRANSITION)
						return FLOAT_NUMBER;
					else if (have_decimal)
						return SCALEDINT;
					else
						return NUMBER64BIT;
				}
			} /* else if (!have_exp) */
		} /* if (!have_error) */

		/* we got some kind of error or overflow, so don't recognize this
		 * as a number: just pass it through to the next part of the lexer.
		 */
	}

	/* Restore the status quo ante, before we started our unsuccessful
	   attempt to recognize a number. */
	ptr = last_token;
	c   = *ptr++;
	/* We never touched tok_class, so it doesn't need to be restored. */

	/* end of number-recognition code */


	if (tok_class & CHR_LETTER)
	{
		char* p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; ptr < end && classes[static_cast<UCHAR>(*ptr)] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER (*ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		dsql_sym* sym =
			HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
		if (sym)
		{
		/* 13 June 2003. Nickolay Samofatov
		 * Detect INSERTING/UPDATING/DELETING as non-reserved keywords.
		 * We need to help parser from lexer because our grammar is not LARL(1) in this case
		 */
			if (prev_keyword == '(' && !brace_analysis &&
				(sym->sym_keyword == INSERTING ||
				 sym->sym_keyword == UPDATING ||
				 sym->sym_keyword == DELETING
				) &&
				/* Produce special_trigger_action_predicate only where we can handle it -
				  in search conditions */
				(prev_prev_keyword == '(' || prev_prev_keyword == NOT || prev_prev_keyword == AND ||
				 prev_prev_keyword == OR || prev_prev_keyword == ON || prev_prev_keyword == HAVING ||
				 prev_prev_keyword == WHERE || prev_prev_keyword == WHEN) )
			{			
				const LexerState savedState = lex;
				const int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
				lex = savedState;
				if (nextToken == OR || nextToken == AND) {
					switch (sym->sym_keyword) {
					case INSERTING:
						yylval = (dsql_nod*) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (dsql_nod*) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (dsql_nod*) sym->sym_object;
						return KW_DELETING;
					}
				}
			}
		/* 23 May 2003. Nickolay Samofatov
		 * Detect FIRST/SKIP as non-reserved keywords
		 * 1. We detect FIRST or SKIP as keywords if they appear just after SELECT and
		 *   immediately before parameter mark ('?'), opening brace ('(') or number
		 * 2. We detect SKIP as a part of FIRST/SKIP clause the same way
		 * 3. We detect FIRST if we are explicitly asked for (such as in NULLS FIRST/LAST clause)
		 * 4. In all other cases we return them as SYMBOL
		 */
			if ((sym->sym_keyword == FIRST && !first_detection) ||
				sym->sym_keyword == SKIP)
			{
				if (prev_keyword == SELECT || limit_clause) {
					const LexerState savedState = lex;
					const int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
					lex = savedState;
					if (nextToken != NUMBER && nextToken != '?' && nextToken != '(') {
						yylval = (dsql_nod*) MAKE_string(string, p - string);
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return SYMBOL;
					}
					else {
						yylval = (dsql_nod*) sym->sym_object;
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			}
			else {
				yylval = (dsql_nod*) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
				return sym->sym_keyword;
			}
		}
		yylval = (dsql_nod*) MAKE_string(string, p - string);
		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end)
	{
		dsql_sym* sym =
			HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
		if (sym)
		{
			++ptr;
			return sym->sym_keyword;
		}
	}
		
	/* We need to swallow braces around INSERTING/UPDATING/DELETING keywords */
	/* This algorithm is not perfect, but it is ok for now. 
	  It should be dropped when BOOLEAN datatype is introduced in Firebird */
	if ( c == '(' && !brace_analysis && 
		/* 1) We need to swallow braces in all boolean expressions
		   2) We may swallow braces in ordinary expressions 
		   3) We should not swallow braces after special tokens 
			 like IF, FIRST, SKIP, VALUES and 30 more other	   
		*/
		(prev_keyword == '(' || prev_keyword == NOT || prev_keyword == AND || prev_keyword == OR ||
		 prev_keyword == ON || prev_keyword == HAVING || prev_keyword == WHERE || prev_keyword == WHEN) )
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
		} while (nextToken == '(');
		dsql_nod* temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect, db_dialect,
						  parser_version, stmt_ambiguous) == ')')
			{
				openCount--;
			}
			if (openCount) {
				/* Not enough closing braces. Restore status quo. */
				lex = savedState;
			}
			else {
				/* Cool! We successfully swallowed braces ! */
				brace_analysis = false;
				yylval = temp_val;
				/* Check if we need to handle LR(2) grammar case */
				if (prev_keyword == '(' &&
					/* Produce special_trigger_action_predicate only where we can handle it -
					  in search conditions */
					(prev_prev_keyword == '(' || prev_prev_keyword == NOT || prev_prev_keyword == AND ||
					 prev_prev_keyword == OR || prev_prev_keyword == ON || prev_prev_keyword == HAVING ||
					 prev_prev_keyword == WHERE || prev_prev_keyword == WHEN) )
				{
					savedState = lex;
					const int token = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
					lex = savedState;
					if (token == OR || token == AND) {
						switch (nextToken) {
						case INSERTING:
							return KW_INSERTING;
						case UPDATING:
							return KW_UPDATING;
						case DELETING:
							return KW_DELETING;
						}
					}
				}
				return nextToken;
			}
		}
		else {
			/* Restore status quo. */
			lex = savedState;
		}
	}

	/* Single character punctuation are simply passed on */

	return c;
}


// The argument passed to this function is ignored. Therefore, messages like
// "syntax error" and "yacc stack overflow" are never seen.
static void yyerror(const TEXT* error_string)
{
/**************************************
 *
 *	y y e r r o r
 *
 **************************************
 *
 * Functional description
 *	Print a syntax error.
 *
 **************************************/

	if (DSQL_yychar < 1)
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			isc_arg_gds, isc_command_end_err,	/* Unexpected end of command */
			0);
	else
	{
		ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104,
			isc_arg_gds, isc_dsql_token_unk_err,
			isc_arg_number, (SLONG) lex.lines,
			isc_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
			isc_arg_gds, isc_random,
			isc_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
	}
}


static void yyabandon (SSHORT		sql_code,
					   ISC_STATUS	error_symbol)
{
/**************************************
 *
 *	y y a b a n d o n
 *
 **************************************
 *
 * Functional description
 *	Abandon the parsing outputting the supplied string
 *
 **************************************/

	ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) sql_code, 
		isc_arg_gds, error_symbol, 0);
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
dsql_yyparse(USHORT client_dialect, USHORT db_dialect, USHORT parser_version, bool* stmt_ambiguous)
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    ;

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            lex.dsql_debug = yyn - '0';
    }
#endif

    yynerrs = 0;
    DSQL_yyerrflag = 0;
    DSQL_yychar = (-1);

    DSQL_DSQL_yyssp = DSQL_yyss;
    yyvsp = yyvs;
    *DSQL_DSQL_yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (DSQL_yychar < 0)
    {
        if ((DSQL_yychar = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)) < 0) DSQL_yychar = 0;
#if YYDEBUG
        if (lex.dsql_debug)
        {
            yys = 0;
            if (DSQL_yychar <= YYMAXTOKEN) yys = yyname[DSQL_yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, DSQL_yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += DSQL_yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == DSQL_yychar)
    {
#if YYDEBUG
        if (lex.dsql_debug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++DSQL_DSQL_yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        DSQL_yychar = (-1);
        if (DSQL_yyerrflag > 0)  --DSQL_yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += DSQL_yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == DSQL_yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (DSQL_yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (DSQL_yyerrflag < 3)
    {
        DSQL_yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*DSQL_DSQL_yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (lex.dsql_debug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *DSQL_DSQL_yyssp, yytable[yyn]);
#endif
                if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++DSQL_DSQL_yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (lex.dsql_debug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *DSQL_DSQL_yyssp);
#endif
                if (DSQL_DSQL_yyssp <= DSQL_yyss) goto yyabort;
                --DSQL_DSQL_yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (DSQL_yychar == 0) goto yyabort;
#if YYDEBUG
        if (lex.dsql_debug)
        {
            yys = 0;
            if (DSQL_yychar <= YYMAXTOKEN) yys = yyname[DSQL_yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, DSQL_yychar, yys);
        }
#endif
        DSQL_yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (lex.dsql_debug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
{ DSQL_parse = yyvsp[0]; }
break;
case 2:
{ DSQL_parse = yyvsp[-1]; }
break;
case 22:
{ prepare_console_debug ((IPTR) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 28:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 29:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 30:
{ yyval = make_list (yyvsp[0]); }
break;
case 32:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 33:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 34:
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 35:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 36:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 37:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 38:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 39:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 40:
{ yyval = NULL; }
break;
case 41:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 42:
{ yyval = NULL; }
break;
case 43:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 44:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 45:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 46:
{ yyval = make_node (nod_revoke, (int) e_grant_count,
					make_list(yyvsp[-2]), make_list(yyvsp[0]), NULL, yyvsp[-3]); }
break;
case 47:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 48:
{ yyval = NULL; }
break;
case 49:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 50:
{ yyval = NULL; }
break;
case 54:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 57:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
break;
case 58:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
break;
case 59:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
break;
case 60:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 62:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 63:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 64:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
break;
case 65:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
break;
case 67:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 68:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 70:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 71:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 73:
{ yyval = yyvsp[0];}
break;
case 74:
{ yyval = yyvsp[0]; }
break;
case 75:
{ yyval = yyvsp[0]; }
break;
case 76:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
break;
case 78:
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 79:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-2]; }
break;
case 80:
{ yyval = NULL; }
break;
case 82:
{ yyval = yyvsp[-1]; }
break;
case 84:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 85:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
break;
case 86:
{ yyval = NULL; }
break;
case 87:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 88:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_scalar_array, CONSTANT_SLONG); }
break;
case 89:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_ref_with_null, CONSTANT_SLONG); }
break;
case 91:
{ yyval = yyvsp[-1]; }
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
							  yyvsp[-2], yyvsp[0]); }
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) e_udf_param_count,
				NULL, MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG));}
break;
case 94:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_reference, CONSTANT_SLONG); }
break;
case 95:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_value, CONSTANT_SLONG); }
break;
case 96:
{ yyval = MAKE_constant ((dsql_str*) Jrd::FUN_descriptor, CONSTANT_SLONG); }
break;
case 97:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_reference), CONSTANT_SLONG); }
break;
case 98:
{ yyval = MAKE_constant ((dsql_str*) (-1 * Jrd::FUN_descriptor), CONSTANT_SLONG); }
break;
case 99:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 100:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 101:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 102:
{ yyval = yyvsp[0]; }
break;
case 103:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 104:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 105:
{ yyval = yyvsp[0]; }
break;
case 106:
{ yyval = yyvsp[0]; }
break;
case 107:
{ yyval = yyvsp[0]; }
break;
case 108:
{ yyval = yyvsp[0]; }
break;
case 109:
{ yyval = yyvsp[0]; }
break;
case 110:
{ yyval = yyvsp[0]; }
break;
case 111:
{ yyval = yyvsp[0]; }
break;
case 112:
{ yyval = yyvsp[0]; }
break;
case 113:
{ yyval = yyvsp[0]; }
break;
case 114:
{ yyval = yyvsp[0]; }
break;
case 115:
{ yyval = yyvsp[0]; }
break;
case 116:
{ yyval = yyvsp[0]; }
break;
case 117:
{ yyval = yyvsp[0]; }
break;
case 118:
{ yyval = yyvsp[0]; }
break;
case 119:
{ yyval = yyvsp[0]; }
break;
case 120:
{ yyval = yyvsp[0]; }
break;
case 121:
{ yyval = yyvsp[0]; }
break;
case 122:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 123:
{ yyval = NULL; }
break;
case 124:
{ yyval = make_list (yyvsp[0]); }
break;
case 126:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 127:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 128:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 129:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 130:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 131:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 132:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 133:
{ yyval = (dsql_nod*) 0;}
break;
case 134:
{ yyval = yyvsp[-1]; }
break;
case 135:
{ yyval = NULL; }
break;
case 138:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 139:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 140:
{ yyval = NULL; }
break;
case 141:
{ yyval = NULL; }
break;
case 142:
{ yyval = yyvsp[0]; }
break;
case 143:
{ yyval = NULL; }
break;
case 144:
{ yyval = NULL; }
break;
case 147:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 148:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 151:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 152:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 153:
{ yyval = make_node (nod_def_generator, (int) e_gen_count, yyvsp[0]); }
break;
case 154:
{ yyval = make_node (nod_def_role, (int) 1, yyvsp[0]); }
break;
case 155:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 158:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = (dsql_nod*) yyvsp[0]; }
break;
case 159:
{yyval = NULL;}
break;
case 162:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 163:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 164:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 165:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 166:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 167:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 168:
{yyval = NULL;}
break;
case 171:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 173:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 174:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 175:
{ lex.g_file->fil_name = (dsql_str*) yyvsp[-1];
			  yyval = (dsql_nod*) make_node (nod_file_desc, (int) 1,
						(dsql_nod*) lex.g_file); }
break;
case 176:
{ lex.g_file  = make_file();}
break;
case 181:
{ lex.g_file->fil_start = (IPTR) yyvsp[0];}
break;
case 182:
{ lex.g_file->fil_length = (IPTR) yyvsp[-1];}
break;
case 189:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 190:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 191:
{ yyval = yyvsp[0]; }
break;
case 192:
{ yyval = yyvsp[0]; }
break;
case 193:
{ yyval = NULL; }
break;
case 195:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 198:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 199:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 200:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 201:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 204:
{ yyval = NULL; }
break;
case 205:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 206:
{ yyval = yyvsp[0]; }
break;
case 207:
{ yyval = NULL; }
break;
case 208:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 209:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (dsql_nod*) lex.g_field; }
break;
case 210:
{ yyval = yyvsp[-1]; }
break;
case 211:
{ lex.g_field = make_field (NULL);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 212:
{ yyval = yyvsp[0]; }
break;
case 213:
{ yyval = NULL; }
break;
case 220:
{ yyval = NULL; }
break;
case 223:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 224:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 225:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 226:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 228:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 229:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 230:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 231:
{ yyval = yyvsp[0]; }
break;
case 232:
{ yyval = NULL ;}
break;
case 237:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 240:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 241:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 242:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 243:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 244:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 245:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 246:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 247:
{ yyval = NULL;}
break;
case 248:
{ yyval = yyvsp[0];}
break;
case 249:
{ yyval = yyvsp[0];}
break;
case 250:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 251:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 252:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 253:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 254:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 255:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 256:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 257:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 258:
{ yyval = make_list (yyvsp[-1]); }
break;
case 259:
{ yyval = NULL; }
break;
case 260:
{ yyval = make_list (yyvsp[-1]); }
break;
case 261:
{ yyval = NULL; }
break;
case 263:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 264:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 266:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 267:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 268:
{ yyval = yyvsp[0]; }
break;
case 269:
{ yyval = yyvsp[0]; }
break;
case 270:
{ yyval = (dsql_nod*) NULL; }
break;
case 271:
{ yyval = make_list (yyvsp[0]); }
break;
case 272:
{ yyval = NULL; }
break;
case 274:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 275:
{ yyval = yyvsp[-1]; }
break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 279:
{ yyval = NULL; }
break;
case 280:
{ yyval = NULL; }
break;
case 281:
{ yyval = yyvsp[0]; }
break;
case 282:
{ yyval = yyvsp[0]; }
break;
case 283:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 286:
{ yyval = yyvsp[-1]; }
break;
case 287:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 288:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 289:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 307:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 308:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 314:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 315:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 316:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 317:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 319:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 321:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 322:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 323:
{ yyval = NULL; }
break;
case 324:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 325:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 326:
{ yyval = make_list (yyvsp[0]); }
break;
case 327:
{ yyval = make_list (yyvsp[-1]); }
break;
case 328:
{ yyval = NULL; }
break;
case 329:
{ yyval = make_list (yyvsp[0]); }
break;
case 330:
{ yyval = make_list (yyvsp[-1]); }
break;
case 331:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 336:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 337:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 338:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 341:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 342:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 356:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 357:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 358:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 359:
{ yyval = NULL; }
break;
case 360:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 361:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 362:
{ yyval = make_list (yyvsp[-1]); }
break;
case 363:
{ yyval = NULL; }
break;
case 365:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 368:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 369:
{ lex.beginning = lex_position(); }
break;
case 370:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 371:
{ lex.beginning = lex.last_token; }
break;
case 372:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 373:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 374:
{ yyval = 0; }
break;
case 375:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 377:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = NULL; }
break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 399:
{ yyval = NULL; }
break;
case 400:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 403:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 407:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 408:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 413:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 418:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 422:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 428:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 429:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 430:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 462:
{ yyval = NULL; }
break;
case 463:
{ yyval = NULL; }
break;
case 464:
{ yyval = NULL; }
break;
case 465:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 466:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 467:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 468:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 470:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 471:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 472:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 473:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 474:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 475:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 477:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 478:
{ yyval = yyvsp[0]; }
break;
case 479:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 481:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 482:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 483:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 485:
{ yyval = NULL; }
break;
case 487:
{ yyval = NULL; }
break;
case 488:
{ yyval = yyvsp[0]; }
break;
case 489:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 501:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 506:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 507:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 509:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 510:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 511:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 517:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 520:
{ 
			*stmt_ambiguous = true;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, isc_arg_number, (SLONG) 301, 
											   isc_arg_warning, isc_dtype_renamed, 0);
				lex.g_field->fld_dtype = dtype_timestamp; 
				lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
				}
			else if (client_dialect == SQL_DIALECT_V6_TRANSITION)
				yyabandon (-104, isc_transitional_date);
			else
				{
				lex.g_field->fld_dtype = dtype_sql_date; 
				lex.g_field->fld_length = sizeof (ULONG);
				}
			}
break;
case 521:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 529:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 530:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 531:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 532:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 533:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 534:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 550:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 551:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 552:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 553:
{		 
			if ( ((IPTR) yyvsp[-1] < 1) || ((IPTR) yyvsp[-1] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((IPTR) yyvsp[-1] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (isc_sqlerr,
					   isc_arg_number, (SLONG) -817,
					   isc_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   isc_arg_number, (SLONG) db_dialect,
					   0);
				if (client_dialect <= SQL_DIALECT_V5)
					{
				lex.g_field->fld_dtype = dtype_double;
				lex.g_field->fld_length = sizeof (double);
					}
				else
					{
				if (client_dialect == SQL_DIALECT_V6_TRANSITION)
					{
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					isc_arg_end );

					}
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else 
				if ((IPTR) yyvsp[-1] < 5)
					{
					lex.g_field->fld_dtype = dtype_short; 
					lex.g_field->fld_length = sizeof (SSHORT); 
					}
				else
					{
					lex.g_field->fld_dtype = dtype_long; 
					lex.g_field->fld_length = sizeof (SLONG); 
					}
			lex.g_field->fld_precision = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 554:
{ 
			if ( ((IPTR) yyvsp[-3] < 1) || ((IPTR) yyvsp[-3] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision should be between 1 and 18 */ 
			if (((IPTR) yyvsp[-1] > (IPTR) yyvsp[-3]) || ((IPTR) yyvsp[-1] < 0))
				yyabandon (-842, isc_scale_nogt);
				/* Scale must be between 0 and precision */
			if ((IPTR) yyvsp[-3] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (isc_sqlerr,
					   isc_arg_number, (SLONG) -817,
					   isc_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   isc_arg_number, (SLONG) db_dialect,
					   0);
				if (client_dialect <= SQL_DIALECT_V5)
					{
				lex.g_field->fld_dtype = dtype_double;
				lex.g_field->fld_length = sizeof (double); 
					}
				else
					{
				if (client_dialect == SQL_DIALECT_V6_TRANSITION)
				  {
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					isc_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					isc_arg_end );
				  }
				  /* client_dialect >= SQL_DIALECT_V6 */
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else
				{
				if ((IPTR) yyvsp[-3] < 5)
					{
					lex.g_field->fld_dtype = dtype_short; 
					lex.g_field->fld_length = sizeof (SSHORT); 
					}
				else
					{
					lex.g_field->fld_dtype = dtype_long; 
					lex.g_field->fld_length = sizeof (SLONG); 
					}
				}
			lex.g_field->fld_precision = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT)(IPTR) yyvsp[-1];
			}
break;
case 557:
{ 
			if ((IPTR) yyvsp[0] > 7)
				{
				lex.g_field->fld_dtype = dtype_double;
				lex.g_field->fld_length = sizeof (double); 
				}
			else
				{
				lex.g_field->fld_dtype = dtype_real; 
				lex.g_field->fld_length = sizeof (float);
				}
			}
break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 560:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 561:
{ yyval = yyvsp[-1]; }
break;
case 562:
{ yyval = 0; }
break;
case 566:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 567:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 568:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 572:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 573:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 574:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 575:
{ yyval = 0; }
break;
case 576:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 579:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 580:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 583:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 584:
{ yyval = NULL; }
break;
case 586:
{ yyval = NULL; }
break;
case 587:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 589:
{ yyval = NULL; }
break;
case 591:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 596:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 597:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 600:
{ yyval = yyvsp[0];}
break;
case 602:
{ yyval = yyvsp[0];}
break;
case 603:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 604:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 605:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 606:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 607:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 608:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 609:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 610:
{ yyval = 0; }
break;
case 611:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 612:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 613:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 614:
{ yyval = (dsql_nod*) 0; }
break;
case 615:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 616:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 618:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 620:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 621:
{ yyval = 0; }
break;
case 623:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 624:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 625:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 626:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 627:
{ yyval = NULL; }
break;
case 628:
{ yyval = yyvsp[0]; }
break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 630:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 631:
{ yyval = NULL; }
break;
case 632:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 633:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 634:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 636:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 637:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 639:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 640:
{ lex.limit_clause = true; }
break;
case 641:
{ lex.limit_clause = false; }
break;
case 642:
{ lex.first_detection = true; }
break;
case 643:
{ lex.first_detection = false; }
break;
case 644:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 645:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 646:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 647:
{ yyval = 0; }
break;
case 648:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 649:
{ yyval = yyvsp[-2]; }
break;
case 650:
{ yyval = yyvsp[-1]; }
break;
case 651:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 652:
{ yyval = yyvsp[-1]; }
break;
case 653:
{ yyval = yyvsp[0]; }
break;
case 654:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 655:
{ yyval = 0; }
break;
case 656:
{ yyval = make_list (yyvsp[0]); }
break;
case 657:
{ yyval = 0; }
break;
case 659:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 661:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 664:
{ yyval = make_list (yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 671:
{ yyval = yyvsp[-1]; }
break;
case 672:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 674:
{ yyval = NULL; }
break;
case 675:
{ yyval = make_list (yyvsp[-1]); }
break;
case 676:
{ yyval = NULL; }
break;
case 678:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 682:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 683:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 684:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 687:
{ yyval = yyvsp[0]; }
break;
case 688:
{ yyval = make_list (yyvsp[-1]); }
break;
case 689:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 690:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 691:
{ yyval = make_list (yyvsp[-1]); }
break;
case 692:
{ yyval = NULL; }
break;
case 694:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 695:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 696:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 698:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 699:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 700:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 703:
{ yyval = make_list (yyvsp[0]); }
break;
case 704:
{ yyval = NULL; }
break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 708:
{ yyval = yyvsp[0]; }
break;
case 709:
{ yyval = NULL; }
break;
case 710:
{ yyval = yyvsp[0]; }
break;
case 711:
{ yyval = NULL; }
break;
case 712:
{ yyval = yyvsp[0]; }
break;
case 713:
{ yyval = NULL; }
break;
case 714:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 715:
{ yyval = 0; }
break;
case 716:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 717:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 718:
{ yyval = 0; }
break;
case 719:
{ yyval = 0; }
break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 727:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 728:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 731:
{ yyval = make_list (yyvsp[-1]); }
break;
case 732:
{ yyval = 0; }
break;
case 733:
{ yyval = make_list (yyvsp[0]); }
break;
case 734:
{ yyval = 0; }
break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 738:
{ yyval = 0; }
break;
case 739:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 740:
{ yyval = 0; }
break;
case 741:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 742:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 743:
{ yyval = yyvsp[-1]; }
break;
case 744:
{ yyval = 0; }
break;
case 745:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 747:
{ yyval = NULL; }
break;
case 748:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 749:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 752:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 753:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 754:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 757:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 758:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 762:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 763:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 770:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 771:
{ yyval = yyvsp[0]; }
break;
case 773:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 776:
{ yyval = NULL; }
break;
case 777:
{ yyval = make_list (yyvsp[-1]); }
break;
case 779:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = NULL; }
break;
case 782:
{ yyval = make_list (yyvsp[-1]); }
break;
case 784:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 788:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = yyvsp[-1]; }
break;
case 804:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 837:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 843:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 846:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 848:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 850:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 852:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 854:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 855:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 856:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 858:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 859:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 860:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 861:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 863:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 864:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 865:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 866:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 867:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 869:
{ yyval = make_list (yyvsp[-1]); }
break;
case 870:
{ yyval = yyvsp[-1]; }
break;
case 881:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 882:
{ yyval = yyvsp[0]; }
break;
case 883:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 884:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 885:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 886:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 887:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 888:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 889:
{ yyval = yyvsp[-1]; }
break;
case 890:
{ yyval = yyvsp[-1]; }
break;
case 894:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 895:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 896:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 899:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
break;
case 900:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 901:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 902:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 904:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 906:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 908:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 909:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 910:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 911:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 913:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 914:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DATE);
			}
break;
case 915:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_dialect_datatype_unsupport,
					isc_arg_number, client_dialect,
					isc_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (isc_sqlerr, isc_arg_number, (SLONG) -104, 
					isc_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					isc_arg_number, db_dialect,
					isc_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIME);
			}
break;
case 916:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 917:
{ yyval = make_parameter (); }
break;
case 918:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 919:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 920:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 921:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 922:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 923:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 924:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 925:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 926:
{ yyval = yyvsp[0]; }
break;
case 927:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 929:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 930:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 931:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 932:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 933:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 935:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 936:
{ yyval = yyvsp[0];}
break;
case 940:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 941:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 942:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 943:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 944:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 945:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 946:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 947:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 949:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 950:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 952:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 954:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 956:
{ yyval = yyvsp[0]; }
break;
case 957:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 958:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 959:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 960:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 963:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 964:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 965:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 968:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 969:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 971:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 972:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 973:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 974:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 975:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 979:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 980:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 985:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 988:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 993:
{ yyval = make_node (nod_null, 0, NULL); }
break;
    }
    DSQL_DSQL_yyssp -= yym;
    yystate = *DSQL_DSQL_yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (lex.dsql_debug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++DSQL_DSQL_yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (DSQL_yychar < 0)
        {
            if ((DSQL_yychar = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous)) < 0) DSQL_yychar = 0;
#if YYDEBUG
            if (lex.dsql_debug)
            {
                yys = 0;
                if (DSQL_yychar <= YYMAXTOKEN) yys = yyname[DSQL_yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, DSQL_yychar, yys);
            }
#endif
        }
        if (DSQL_yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (lex.dsql_debug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *DSQL_DSQL_yyssp, yystate);
#endif
    if (DSQL_DSQL_yyssp >= DSQL_yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++DSQL_DSQL_yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
