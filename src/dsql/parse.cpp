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
  141,   92,   92,  137,  137,  110,  110,  105,  145,  146,
   61,  138,  138,  111,  111,  111,  111,  111,  111,  139,
  139,  153,  153,  154,  156,  156,  156,  156,  156,  136,
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
   12,  225,  225,  226,  226,  227,   77,   86,  143,  231,
   93,   95,  230,  230,   76,   89,  232,  232,  232,  233,
  236,  236,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  234,  234,  235,
    2,  239,  239,  239,  239,  239,  239,  239,  239,  248,
  249,  245,  245,  250,  250,  250,  250,  250,  250,  250,
  240,  240,  251,  251,  251,  251,  251,  251,  251,  244,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  253,  253,  255,
  255,  254,  252,  252,  252,  246,  246,  247,  247,  247,
  242,  243,  243,  259,  259,  259,  259,  259,  241,  260,
  260,  261,  261,    8,  262,  262,  262,  262,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  107,  107,  140,
  140,  265,  265,  268,  268,  270,  270,   56,   56,  267,
  267,  267,  267,  267,  267,  267,  267,  267,  274,  274,
  266,  266,  266,  266,  276,  276,  275,  275,  275,   58,
   58,  271,  271,  271,  269,  269,  269,  280,  280,  280,
  279,  279,  278,  278,  278,  272,  272,  281,  281,  281,
  282,  282,  273,  273,  273,  273,  283,  283,   19,   19,
   19,  285,  285,  285,   17,   17,   17,  287,  288,  291,
  291,  289,  293,  293,    4,   16,  292,  292,  294,  294,
  295,  295,  284,  296,  296,  297,  297,  298,  298,  298,
  298,  299,  299,  300,  300,  301,  301,  303,  303,  303,
  304,  304,  304,  305,  305,  305,  302,  307,  307,  307,
  308,  308,  306,  306,  309,  311,  311,  310,  310,  286,
   18,  312,  312,  314,  314,  313,  313,  229,  318,  319,
  315,  315,  315,  320,  322,  331,  332,  333,  334,  323,
  323,  323,  323,  335,  335,  335,  336,  336,  336,  324,
  324,  325,  325,  338,  338,  339,  339,  340,  340,  326,
  342,  342,  343,  343,  345,  345,  345,  347,  348,  348,
  349,  349,  351,  351,  344,  344,  344,  352,  353,  354,
  356,  356,  357,  358,  346,  346,  359,  359,  360,  360,
   24,  355,  355,  355,  355,  355,  361,  361,  328,  328,
  362,  362,  363,  329,  329,  327,  327,  330,  330,  364,
  365,  365,  365,  365,  365,  366,  366,  367,  367,  368,
  368,  369,  369,  369,  370,  370,  371,  371,  316,  316,
  372,  372,  373,   71,   71,   71,  375,  375,  374,  374,
  317,  317,  317,   10,   10,    7,    7,  377,  378,  379,
   20,   20,  380,  381,  382,  382,  191,  205,    3,    3,
  385,  385,  385,  387,  387,  275,  386,  386,  388,  388,
   34,   34,   91,   90,   90,  376,  376,  389,  390,  390,
  215,  215,  215,  142,  383,  383,  117,  117,  117,  117,
  117,  394,  394,  394,  394,  394,  394,  393,  393,  393,
  396,  396,  396,  396,  396,  396,  396,  396,  396,  396,
  396,  397,  397,  397,  397,  397,  397,  397,  397,  403,
  403,  403,  403,  403,  403,  403,  403,  403,  403,  403,
  403,  403,  403,  403,  403,  408,  408,  398,  398,  399,
  399,  400,  400,  400,  400,  401,  401,  405,  405,  406,
  406,  406,  406,  404,  407,  402,  402,  392,  392,  392,
  395,  395,  395,  409,  409,  410,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,  152,  152,  152,  411,  214,  214,
  147,  147,  417,  417,  417,  417,  417,  413,  413,  413,
  413,  413,  228,  148,  148,  149,  150,  150,  150,  150,
  150,   54,   54,   21,   21,  238,  418,   57,  277,  258,
  258,  101,  412,  412,  412,  419,  419,  419,  419,  419,
  419,  419,  419,  419,  419,  419,  420,  422,  421,  421,
  424,  425,  425,  384,  384,  414,  415,  415,  426,  426,
  426,  427,  427,  428,  428,  431,  431,  429,  429,  434,
  434,  433,  430,  432,  416,  416,  423,  423,  423,  423,
  423,  423,  423,  423,  337,  337,  321,  321,  151,   51,
   67,  126,  144,  391,  161,  185,  264,   69,   65,  220,
  118,   72,  341,  216,   35,   44,  350,  263,   42,   45,
  213,   43,  290,  257,  257,  435,  435,  435,  435,  435,
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
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    0,    2,    2,    4,    4,    5,
    0,    1,    2,    2,    4,    3,    2,    2,    7,    1,
    0,    1,    0,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    3,    2,    2,    2,    2,    1,    1,    1,
    1,    4,    5,    1,    3,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    4,    4,    6,    5,    3,    0,    2,    2,    0,    3,
    0,    4,    1,    5,    4,    1,    4,    1,    2,    2,
    1,    1,    1,    2,    2,    2,    2,    0,    3,    5,
    1,    1,    2,    3,    1,    2,    3,    0,    1,    1,
    1,    5,    5,    6,    1,    1,    1,    2,    4,    1,
    0,    5,    1,    0,    3,    2,    1,    0,    2,    0,
    1,    0,    3,    1,    0,    1,    2,    1,    1,    1,
    1,    2,    2,    1,    2,    3,    1,    1,    3,    3,
    1,    2,    3,    1,    2,    0,    2,    1,    1,    0,
    1,    1,    1,    3,    2,    3,    0,    1,    3,    4,
    3,    3,    0,    2,    0,    2,    0,    3,    3,    3,
    1,    4,    4,    1,    9,    0,    0,    0,    0,    3,
    2,    1,    0,    3,    5,    3,    2,    5,    2,    1,
    1,    1,    1,    1,    3,    1,    3,    1,    0,    2,
    1,    3,    1,    1,    1,    1,    3,    6,    1,    0,
    3,    0,    1,    3,    1,    1,    1,    4,    5,    5,
    1,    1,    2,    4,    4,    2,    3,    0,    1,    2,
    1,    1,    2,    2,    2,    0,    1,    0,    3,    0,
    1,    3,    1,    2,    0,    2,    0,    2,    0,    4,
    1,    2,    1,    1,    0,    1,    3,    2,    1,    1,
    2,    1,    4,    3,    1,    3,    4,    0,    3,    0,
    1,    3,    3,    1,    1,    0,    1,    1,    4,    0,
    2,    4,    0,    8,    5,    1,    1,    7,    4,    4,
    1,    1,    8,    5,    1,    3,    3,    1,    7,    7,
    5,    3,    0,    1,    1,    1,    2,    0,    1,    1,
    1,    0,    3,    1,    3,    1,    0,    3,    1,    3,
    1,    3,    3,    1,    1,    3,    1,    2,    1,    3,
    3,    1,    2,    3,    3,    3,    3,    1,    3,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    3,    3,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    1,    1,    5,    6,    5,
    6,    3,    4,    5,    6,    3,    4,    3,    4,    3,
    4,    4,    5,    4,    4,    3,    4,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    3,    3,
    3,    3,    3,    3,    3,    3,    1,    1,    1,    1,
    3,    1,    1,    1,    1,    1,    1,    4,    1,    3,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    1,    2,    1,    1,    1,    1,    1,
    2,    1,    1,    1,    1,    4,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    1,    6,    1,    4,
    7,    2,    0,    4,    3,    6,    1,    1,    6,    8,
    6,    1,    1,    4,    6,    4,    5,    3,    5,    4,
    5,    1,    1,    1,    4,    6,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    0,    1,    0,    1,    1,
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
    0,  559,  560,  561,  565,  566,  567,    0,  631,  634,
  746,  747,  751,  752,  471,    0,    0,    0,    0,    0,
    0,    0,  401,  577,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  122,    0,    0,    0,  102,    0,
  926,    0,   22,  924,    0,    0,   73,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  484,    0,    0,    0,   36,   33,   35,    0,   34,
    0, 1014, 1034, 1035, 1016, 1018, 1017, 1023, 1019, 1024,
 1020, 1022, 1025, 1021, 1026, 1027, 1028, 1030, 1031, 1029,
 1032, 1033, 1037, 1036, 1038,    0,    0,    0,    0,   31,
   68,   66, 1006, 1015,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  642,    0,    0,    0,
  689, 1008,    0,    0,    0,    0,    0,  115, 1013,  568,
    0,    2,    0,    0,    0,    0,    0,    0,  432,  433,
  434,  435,  436,  437,  438,  439,  440,  441,  442,  443,
  444,  445,  446,  447,  448,  449,  456,  450,  451,  452,
  453,  454,  455,  457,    0,  430,  431,    0,  998,    0,
  408, 1002,    0,  405, 1005,    0,  691,    0,  404, 1009,
    0,  409, 1001,    0,  575,  922,    0,  158,  111,    0,
  112,    0,  208,  994,  784,    0,  109,  153,    0,    0,
  105,    0,  113,  928,    0,  106,    0,  107,    0,  108,
 1012,  154,  114,  110,  734,  735,    0,  927,  925,    0,
   74,    0,  999,    0,  997,  492,  485,    0,  491,  496,
  486,  487,  494,  488,  489,  490,  495,  497,    0,    0,
    0,   29,    0,   38,  771,   37,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  932,    0,  913,  636,  636,  637,  647,  649,  985,  650,
    0,  651,  641,  637,    0,    0,    0,    0,    0,    0,
    0,  594,  583,    0,  586,  588,  589,  590,  591,  597,
  598, 1007,  690,    0,    0,  116,    0,  117,    0,  118,
    0,    0,    0,  621,    0,    0,  987,    0,    0,  628,
    0,    0,    0,    0,    0,  472,    0,    0,  371,    0,
  211,    0,  412,  402,  466,  467,    0,    0,    0,    0,
    0,    0,  421,    0,  377,  378,    0,    0,  581,  579,
  923,    0,    0,    0,    0,    0,    0,    0,  161,  140,
    0,  103,    0,    0,  119,    0,  129,  128,    0,    0,
    0,    0,    0,    0,  990,   75,    0,    0,    0,    0,
  749,  493,    0,    0,    0,    0,  890,  919,    0,  989,
    0,  892,    0,    0,  920,    0,  914,  905,  904,  903,
    0,    0,    0,    0,  895,  896,  897,  906,  907,  915,
  916,    0,  917,  918,    0,    0,    0,  921,    0,    0,
    0,    0,    0,    0,    0,  909,    0,  781,  887,  888,
  889,  894,  893,  360,  872,    0,    0,  871,    0,    0,
  876,  868,  869,  870,  873,  874,  875,  908,  933,  934,
  935,  947,  949,  957,  958,  962,  963,  209,    0,    0,
    0,  364,    0,    0,    0,  774,   26,    0,    0,    0,
    0,   71,   69, 1010,   67,   32,    0,    0,    0,  776,
    0,   47,   49,    0,    0,    0,  573,    0,    0,    0,
  644,  646,    0,  653,    0,    0,    0,  654,  640,    0,
  620,    0,  595,    0,  592,    0,  593,  618,    0,  613,
    0,    0,  587,  785,  755,    0,    0,    0,    0,    0,
    0,  570,  569,    0,  622,  626,    0,    0,  731,  633,
  632,    0,  176,    0,    0,  137,    0,  477,    0,  478,
  473,    0,    0,  416,  418,  417,    0,  419,    0,  413,
    0,    0,  262,    0,    0,    0,  425,  426,    0,  459,
    0,    0,    0,    0,    0,    0,    0,  382,  381,  480,
    0,    0,    0,  157,    0,    0,  166,    0,  165,    0,
    0,  172,  155,    0,  170,  162,    0,    0,  516,  551,
  519,    0,  552,    0,    0,  520,    0,    0,    0,  543,
  555,  515,  538,  517,  518,  513,  500,  371,  498,  499,
  501,    0,    0,  510,  511,  512,  514,    0,    0,    0,
    0,    0,  120,    0,  121,    0,  132,    0,    0,  192,
    0,    0,  369,    0,  211,    0,    0,   83,    0,  101,
    0,  100,  991,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  787,  789,  798,  801,  802,  803,  804,
  805,  806,  807,  808,  809,  810,  811,    0,    0,    0,
    0,    0,  910,    0,    0,    0,    0,    0,    0,  911,
  912,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  325, 1011,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  267,  508,
    0,    0,    0,  362,    0,    0,    0,  773,    0,    0,
   43,   72,    0,    0,   25,    0,  779,    0,    0,  745,
    0,    0,    0,    0,  572,    0,  636,    0,  658,    0,
    0,    0,    0,  563,    0,  930,  562,    0,  596,    0,
  604,  600,  599,    0,    0,    0,  615,  603,    0,    0,
    0,  754,    0,    0,    0,  369,    0,    0,    0,    0,
    0,  138,    0,  476,  371,  415,    0,  371,  372,  420,
    0,    0,  258,  369,  231,  995,    0,    0,  371,    0,
    0,  200,  369,    0,    0,    0,    0,  236,  230,  233,
  234,  235,  424,  464,  463,  423,  422,    0,    0,    0,
    0,    0,    0,  380,    0,  371,  469,    0,  468,    0,
  163,  167,    0,    0,  171,    0,    0,  766,    0,  539,
  540,  556,    0,  553,    0,    0,  546,  544,  545,    0,
    0,    0,    0,  509,    0,    0,    0,    0,  547,    0,
    0,  369,    0,  191,    0,  194,  196,  197,    0,    0,
    0,    0,    0,  211,   78,    0,    0,   77,    0,    0,
    0,    0,  788,  800,    0,    0,  861,  862,  863,    0,
  792,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  711,  713,    0,  708,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  977,  978,  979,  980,  981,  982,  983,  984,    0,    0,
    0,    0,    0,    0,  968,    0,    0,    0,    0,    0,
  885,  327,    0,  886,  955,    0,  881,  993,    0,    0,
    0,    0,    0,    0,    0,  891,  783,  782,  366,  365,
    0,  265,    0,    0,    0,  273,  775,    0,    0,    0,
   70,    0,    0,    0,  778,    0,    0,    0,    0,  645,
  648, 1003,  657,    0,    0,    0,    0,  663,  664,  665,
  666,  675,  676,  677,    0,    0,  655,  564,  931,  605,
  614,  609,  608,    0,  619,  786,  756,    0,    0,  369,
    0,    0,  638,  733,  732,    0,  475,    0,    0,  175,
    0,  179,    0,    0,    0,  414,  369,  369,  270,  372,
  263,    0,  202,  371,  204,    0,  372,  199,  205,    0,
    0,    0,    0,  427,  428,  371,  369,  372,    0,    0,
    0,  398,    0,  470,  187,  188,  164,    0,  174,  527,
  528,  929,    0,    0,    0,    0,    0,  554,    0,    0,
  371,  372,    0,    0,  504,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,   82,    0,    0,    0,    0,   90,   84,    0,    0,
   89,   85,    0,  750,  996,    0,    0,  793,    0,    0,
  799,    0,    0,    0,    0,    0,  837,  836,    0,    0,
    0,  846,  864,    0,  856,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  791,    0,  712,    0,  748,
    0,    0,  211,    0,  936,    0,    0,  950,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  964,    0,    0,    0,    0,    0,    0,    0,  975,  630,
  954,  898,    0,  260,  279,    0,    0,  361,  274,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   61,
   63,    0,   41,    0,    0,    0,  760,  780,    0,  759,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  692,
    0,    0,    0,    0,    0,    0,    0,  611,  612,  616,
    0,    0,  190,    0,    0,    0,    0,    0,  180,    0,
    0,    0,  410,  214,  215,  216,  217,  218,  219,  901,
    0,    0,  264,    0,    0,  212,    0,    0,    0,    0,
  237,    0,  460,  461,  429,    0,    0,    0,    0,    0,
    0,  371,  482,  372,  173,  992,    0,    0,  522,    0,
  521,  557,    0,  549,    0,    0,    0,    0,  502,  530,
    0,    0,  532,  535,  537,  369,    0,    0,    0,  127,
    0,  195,  371,    0,  370,  104,    0,  125,    0,   93,
    0,    0,    0,    0,   87,   88,    0,  854,  855,  795,
    0,  797,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  857,    0,    0,    0,    0,    0,    0,    0,  847,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  719,    0,    0,    0,  942,
  941,    0,    0,  938,  937,    0,  946,  945,  944,  943,
  940,  939,    0,    0,  970,    0,    0,    0,  969,    0,
    0,    0,    0,  266,    0,    0,  276,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  359,    0,    0,    0,    0,  285,  290,  284,    0,
    0,    0,  294,  295,  296,  297,  298,  299,  301,  302,
  304,  305,  306,  309,    0,    0,    0,    0,  353,  354,
  355,    0,  758,   65,   57,   58,   64,   59,   60,    0,
   23,    0,    0,   24,  765,  764,    0,  762,  770,  769,
  767,  744,   44,   45,    0,  667,    0,    0,    0,  697,
  695,  693,    0,  694,    0,    0,    0,    0,    0,  753,
    0,  370,  737,  738,  639,    0,  185,  181,    0,  372,
  902,  268,  269,  372,    0,    0,    0,  222,    0,    0,
  238,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  479,  524,    0,  525,    0,  142,    0,    0,    0,  146,
  148,  149,  150,  507,  505,  503,  534,    0,    0,  372,
    0,    0,    0,  367,  371,   91,    0,    0,   97,   92,
    0,    0,    0,    0,    0,  865,    0,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  721,  710,
    0,    0,  722,    0,  718,  210,  956,  976,  948,    0,
    0,  966,  965,    0,  971,  959,  961,    0,    0,  275,
    0,  314,    0,    0,    0,  308,    0,    0,  307,  339,
  340,    0, 1004,  356,  357,    0,  293,  300,    0,  303,
  286,    0,  291,    0,  344,  292,  310,  311,    0,    0,
  337,    0,   54,   55,   56,   62,    0,    0,  687,  685,
    0,  678,    0,    0,    0,  701,    0,  635,  372,  368,
  739,  182,  372,  152,  257,    0,    0,  198,  223,    0,
    0,    0,    0,  224,  227,    0,    0,  372,  396,  397,
  393,  392,  395,  394,    0,  523,  550,  151,  139,  147,
    0,  371,  254,  134,  372,  373,    0,    0,   95,    0,
   79,    0,  823,  831,  629,    0,  824,  832,  827,  835,
    0,    0,  825,  833,  826,  834,  820,  828,  821,  829,
  822,  830,  717,    0,    0,    0,  951,  967,    0,    0,
  282,  278,    0,  312,    0,    0,    0,    0,    0,  341,
    0,    0,  332,  333,    0,  352,    0,    0,    0,    0,
  347,  345,    0,    0,    0,   39,  761,    0,  669,  679,
    0,    0,  680,  681,  682,    0,  255,  411,  201,  206,
  225,    0,    0,  228,    0,  240,  242,  400,  372,    0,
  375,    0,   76,   98,   99,    0,    0,    0,  724,  960,
  281,    0,  313,    0,    0,  316,    0,  322,    0,  324,
    0,    0,  351,  350, 1000,  349,    0,    0,    0,    0,
    0,    0,  668,    0,    0,  702,  229,    0,    0,    0,
    0,    0,  256,  372,    0,    0,  723,    0,    0,    0,
    0,    0,    0,    0,  335,  334,  346,  348,    0,    0,
    0,  673,    0,    0,    0,    0,    0,  239,    0,  246,
    0,  245,  376,  126,  726,    0,  283,    0,    0,  315,
  319,    0,    0,    0,    0,    0,    0,  671,  684,  226,
    0,    0,  250,  249,  248,  727,    0,    0,    0,    0,
    0,  336,  674,  253,  251,  252,  330,  320,    0,  342,
  317,  318,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1443,   28,   29, 1444,
   31,   32,   33,   34,   35,   36,   37, 1445,   39, 1446,
  858,  136,  498,  161, 1236, 1481,  137,  750,  138,  501,
  755,  139,  140,  284, 1015,  150,  151, 1237, 1238, 1239,
 1240,  218,  249,  141, 1241,  142,  503,   87,  261,  406,
  455,  666, 1125,  456,  897,  637,  242,  874,  667,  668,
  669, 1132, 1127, 1570,  262,  671,  672,   79,  208,   80,
  257, 1796, 1346,  241,  246,  248,  250,  237,  229,  231,
  243,  253,  168,  336,  338,  340,  395,  653,  655,  495,
  285,  818,  573,  682, 1056,  399,  658, 1116, 1340,  605,
  776, 1087,  565,  566,  586,  391,  638, 1102, 1548, 1678,
 1283, 1549, 1550, 1551, 1552, 1553,  683,  238,  230,  387,
  613,  388,  389,  614,  615, 1315,  567, 1050, 1051, 1052,
 1278,  401,  885,  886,  887,  888,  820, 1067, 1526,  639,
  822,  458,  890,  977,  489, 1403, 1284,  459,  460,  461,
  462,  463, 1527, 1528,  589, 1684, 1830, 1301,  828,  829,
  815,  830,  831,  832, 1831, 1832, 1884,  368,  494,  994,
 1447,  214,  582,  991,  583, 1060,  490,  995,  996, 1226,
 1426, 1427, 1428, 1742, 1134, 1448, 1449, 1450, 1451, 1644,
 1452, 1453, 1454, 1455, 1456, 1457, 1458, 1459, 1460, 1461,
 1462, 1463, 1464, 1465, 1466,  464, 1841, 1467, 1752, 1875,
 1749,  465,  727,  466,  467, 1468, 1645, 1760, 1761, 1814,
 1469, 1470, 1471, 1636,  281,  491,  492,  468,   41, 1345,
 1564,  377,  600,  846, 1313,  601,  844,   84,   63,  372,
  219,  178,  355,  205,  362,  211,  222,  809,  806,  363,
  373,  836,  374,  596, 1078,  206,  469, 1103,  356,  602,
 1314,  102,  217,  266,  640,  641,  740, 1104,  741, 1105,
  644,  645,  646,  647, 1486, 1096, 1094,  648,  649,  650,
  867,  651,  864,   42,   43,   44,   45,   46,   47,  170,
  553,   65,  518,  225,  380,  323,  324,  325,  326,  327,
  328,  329,  330,  331,  782,  539, 1034, 1270,  540,  541,
  787,  174,  344,  555,   48,  177,  350, 1369,  726,   49,
  348,   50,  155,  311,  526,  772,  410, 1267, 1509,  699,
  521,  313, 1275, 1671,  156,  157,  312,  527,  528,  770,
 1013, 1016, 1017, 1018, 1019, 1020, 1021, 1768, 1823,  470,
 1853, 1022, 1023, 1024, 1265, 1773, 1774, 1775, 1257,  164,
 1501, 1665, 1666, 1396,  935, 1397, 1398, 1399, 1605, 1797,
 1799,  558,  559, 1044, 1515,  509,   51,   52,  411,   53,
   54,  547,  548,  471, 1003, 1247, 1487, 1491,  510,  758,
  235,  684,  685,  912,  913,  686,  687,  688,  689,  690,
  691,  692,  693,  694,  695,  696,  697, 1150, 1152, 1153,
  472,  473,  474,  475,  476,  477,  478,  259,  479,  480,
  481,  482,  959,  483, 1611,  484,  485,  486,  487,  715,
  963, 1214, 1209,  716,  144,
};
static short yysindex[] = {                                   5310,
 1428, -152, 3012,   83, 1250,    9, 3411,  -90,10194,  424,
   85,   15, -152, 1103,  505,10730, 1060,10730, -116,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  164,    0,    0,    0,    0,    0,    0,  -76,    0,    0,
    0,    0,    0,    0,    0,15469,10730,10730,10730,10730,
10730,10730,    0,    0,   99, 1157,10730,10730,10730,  317,
10730,  195,10730,10730,    0,10730,10730,10730,    0, 1063,
    0,  276,    0,    0,  416,10730,    0,10730,10730,10730,
  430,10730,10730,10730,10730,  195,10730,10730,10730,10730,
10730,    0,10730,  738,  659,    0,    0,    0,  794,    0,
  794,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  582,  682,   88, 1021,    0,
    0,    0,    0,    0,10730,10730,10730,  735,  774, 1576,
10730,  646,  168,  187,   92,  673,    0,10730,  876,  981,
    0,    0,10730,  806,10730,10730,10730,    0,    0,    0,
10730,    0,  796,  814,  994, 1030,  766, 1304,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  484,    0,    0, 1157,    0,  585,
    0,    0, 1269,    0,    0, 1003,    0,  635,    0,    0,
  801,    0,    0,  925,    0,    0,  893,    0,    0, 1150,
    0, 1127,    0,    0,    0, 1157,    0,    0, 1061, 1269,
    0,  348,    0,    0, 1100,    0, 1114,    0,  794,    0,
    0,    0,    0,    0,    0,    0, 1110,    0,    0,  997,
    0, 1163,    0, 1038,    0,    0,    0,  997,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,14591,10730,
 1097,    0,10730,    0,    0,    0, 1081, 1123, 9933,10730,
 1041, 1175, 1485, 1225, 1264, 1279, 1197, 1259,  124, 1132,
    0,14845,    0,    0,    0,    0,    0,    0,    0,    0,
12623,    0,    0,    0, 1220,10730, 1305, 1247,  824,10730,
 1275,    0,    0,  981,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10730, 1269,    0, 1100,    0,  794,    0,
 1329, 1341, 1223,    0,14845, 1302,    0, 1302,14845,    0,
 -141, 1224, 1213, 1234, 1304,    0, 1456,  579,    0,10730,
    0,  484,    0,    0,    0,    0,10730, 1097, 5558, 1296,
 8979, 1711,    0,15469,    0,    0,  612, 1313,    0,    0,
    0, 1705, 1705, 1157, 1402, 1157,  370, 1150,    0,    0,
 5548,    0,10730,10730,    0, 1097,    0,    0, 1489,  564,
 1738,10730, 1541,10730,    0,    0, 1770, 3180, 9095, 1441,
    0,    0, 1795, 1801, 1802, 1397,    0,    0, 1811,    0,
 1824,    0, 1832, 1833,    0, 1838,    0,    0,    0,    0,
    0, 1849, 1430, 1443,    0,    0,    0,    0,    0,    0,
    0, 1854,    0,    0,12877, 1874, 1882,    0, 1883, 1556,
14845,14845,13115,10730, 1888,    0, 2546,    0,    0,    0,
    0,    0,    0,    0,    0, 1880, 1841,    0,    0, 1890,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5548, 1885,
 1507,    0, 1909, 1687, 1588,    0,    0,10730,10730,10730,
  220,    0,    0,    0,    0,    0,10730,10730,  338,    0,
10730,    0,    0, 1081, 1558, 9933,    0,10730,13115, 1326,
    0,    0,14845,    0, 2054, 1638, 1913,    0,    0,  177,
    0,  640,    0,  351,    0,  351,    0,    0, 1915,    0,
  150, 1542,    0,    0,    0, 1918,   95, 1905, 1097, 1931,
 1722,    0,    0,10730,    0,    0, 1122, 1943,    0,    0,
    0,  879,    0, 1644, 1645,    0, 1157,    0, 1648,    0,
    0, 1721, 1728,    0,    0,    0, 1708,    0, 5548,    0,
 5548, 1616,    0, 1743,10730,15372,    0,    0,  755,    0,
10730,  512, 1003, 1618, 1583, 1550,    0,    0,    0,    0,
  435, 1623,  181,    0, 1570,  195,    0, 1157,    0, 1753,
 1685,    0,    0,  370,    0,    0,  232, 1586,    0,    0,
    0, 1592,    0, 1642, 1995,    0, 1691, 1997,   35,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1947,  147,    0,    0,    0,    0,  184, 1999, 2000,
 1997, 1269,    0, 1732,    0, 1779,    0, 1157, 1157,    0,
 5558,  635,    0, 1673,    0, 1647, 2008,    0, 9405,    0,
 1678,    0,    0, 1686, 2013, 9314, 2022,    0,    0,    0,
 8832, 9662,   22,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -24, 1683, 1048,
14845,  641,    0,10730,14845, 1085, 1112, 1239, 1855,    0,
    0,14845, 9577, 2546, 1621,  508,14845,14845, 9577, 1755,
  867,  867, 1850, 1650,  -76, 2032,    0,    0,13369,10730,
14845,14845,14845,14845,14845,14845,14845, 8919,    0,    0,
 1804, 2017,10730,    0,10730, 1785,10730,    0, 1656, 1657,
    0,    0, 1630, 9933,    0, 1773,    0, 1654, 2060,    0,
 1773,10730,10730, 2059,    0, 1850,    0, 1884,    0,10730,
 2103, 1660,14845,    0,  423,    0,    0,   12,    0, 1675,
    0,    0,    0,10730,  345,10730,    0,    0,10730,10730,
 1441,    0,14845, 1853, 5558,    0, 2069, 1624,14845,14845,
 1157,    0,  502,    0,    0,    0, 2082,    0,    0,    0,
  185,10730,    0,    0,    0,    0, 1852, 2086,    0, 1831,
 1848,    0,    0, 1791, 1793,  794, 1861,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  195,10730,15560,
 1759, 1760, 1761,    0,  195,    0,    0,  686,    0, 1392,
    0,    0, 1725, 1157,    0, 3180,  133,    0, 1734,    0,
    0,    0,  195,    0, 1995,  122,    0,    0,    0, 1865,
  122, 1735,  122,    0, 2109,  195,  195,  195,    0, 1097,
10730,    0, 1794,    0, 1671,    0,    0,    0,  612, 1302,
10730, 1774,   90,    0,    0, 2128,  572,    0, 3180,10730,
 1302, 9796,    0,    0, 1302, 9314,    0,    0,    0, 6288,
    0,  265,   93,14845,14845,10059, 2130,  714,10320,14845,
10581, 1627,10842,11103,13607,11364,11625,11886, 9577, 9577,
    0,    0, 1809,    0, 2132,  766,14845,14845, 2136,14845,
 2134,14845, 2129, 2067,14845,14845,14845,14845,14845,14845,
    0,    0,    0,    0,    0,    0,    0,    0, 1870, 1750,
  477,14845,  702,14845,    0, 9577, 1242, 2110,  339,10730,
    0,    0,  766,    0,    0, 1781,    0,    0, 1912,  867,
  867,  861,  861, 2546,  696,    0,    0,    0,    0,    0,
 1787,    0, 1744, 1921, 1785,    0,    0, 9680, 9680, 1814,
    0,  -20, 1839,10730,    0,14845, 1839, 1876, 1877,    0,
    0,    0,    0, 1574, 2157, 2155, 1465,    0,    0,    0,
    0,    0,    0,    0, 9577, 1878,    0,    0,    0,    0,
    0,    0,    0,  315,    0,    0,    0, 1683, 2546,    0,
 1799, 1302,    0,    0,    0, 2546,    0, 1705, 1935,    0,
  502,    0, 2162, 9577, 5052,    0,    0,    0,    0,    0,
    0, 1785,    0,    0,    0, 5052,    0,    0,    0,  794,
  794, 1712, 2166,    0,    0,    0,    0,    0,  -36,  -70,
  734,    0, 1944,    0,    0,    0,    0,10730,    0,    0,
    0,    0,   83, 1816, 1796, 1804, 2168,    0, 1570, 1821,
    0,    0, 2153,  983,    0,10730, 1152,  195, 2174, 2178,
 2181, 1959,  635, 1785, 1705, 1645, 5558,    0, 1623, 1784,
 1993,    0,  195, 1847, 1919, 9405,    0,    0,  195, -188,
    0,    0, 1930,    0,    0, 2196, 2199,    0, 9577, 9577,
    0, 9577, 9577, 2209, 2546, 2203,    0,    0, 2546, 2204,
13115,    0,    0, 1928,    0,  643, 2207, 2546, 2213, 1662,
 2215, 2546, 2217,14845,14845, 2130,14845,13861, 2226, 2546,
 2227, 2228, 2546, 2232,14845, 2546, 2235, 2546, 2236, 2237,
 2546, 2238, 2239, 2546, 2241,    0, 2020,    0,13282,    0,
 2247, 2395,    0, 2407,    0, 2475,14845,    0, 2613, 2689,
 2721, 2860, 2868, 2910,14845,14845,14845, 2546, 1856,14845,
    0,14845, 2546, 1979,  598,14845,14845,14845,    0,    0,
    0,    0,10730,    0,    0,10730,15311,    0,    0,10730,
10730,10730,10730,10730,10730, 1858, 2255, 2257,    0,    0,
    0, 1858,    0,  240,  240,   72,    0,    0, 1822,    0,
 9680, 9680, 2244, 1465, 2265,14845, 2043, 2103, 1927,    0,
 1927,  690, 1927, 1971, 1977, 2055, 2005,    0,    0,    0,
  766, 1785,    0, 1784, 1031, 1570, 1949, 1570,    0, 9577,
  409, 1167,    0,    0,    0,    0,    0,    0,    0,    0,
 5052, 5052,    0, 1921,14845,    0, 2050, 1936, 1712, 1063,
    0, 9577,    0,    0,    0, 1948, 1960, 1963, 1966, 1967,
 1968,    0,    0,    0,    0,    0, 2297,   83,    0, 1907,
    0,    0,  122,    0, 5052, 1983,  122,  122,    0,    0,
 1804, 2325,    0,    0,    0,    0,  612, 1921, 1570,    0,
 1645,    0,    0, 2093,    0,    0, 2069,    0, 2330,    0,
 2339, 1157,  -39, 2340,    0,    0, 1157,    0,    0,    0,
 2020,    0, 2020,14845, 1302, 1302, 1836,  -76, 2343,14845,
 2068,    0, 1302, 1302,14845, 1302, 1302, 2253, 2546,    0,
 1764,14845, 2546, 1302, 1302, 1302, 1302, 2546, 1302, 1302,
 1302, 1302, 1302, 1302,10730,    0, 2345, 2327, 1014,    0,
    0, 5548, 2347,    0,    0, 3071,    0,    0,    0,    0,
    0,    0, 3083, 1681,    0,14845, 2084, 1964,    0,14845,
 3148, 1843, 2270,    0, 5548, 2334,    0,    0, 2104,    0,
 4680, -109, 2336, 2356, 2061,14845, 2342, 2346, 5269,10730,
10730,    0, 2355, 2357, 2063, 2360,    0,    0,    0, 2097,
12515, 2361,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2362, 2363,  -86, 2371,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2080,
    0, 9680, 9680,    0,    0,    0, 1987,    0,    0,    0,
    0,    0,    0,    0, 2043,    0, 1844,10730, 1465,    0,
    0,    0, 2091,    0, 2103, 2103,14845, 9577, 1441,    0,
 1921,    0,    0,    0,    0, 1392,    0,    0,  426,    0,
    0,    0,    0,    0, 2546, 2154, 2050,    0,  800,10730,
    0, 2099,  432, 2007, 2100, 2009, 2135, 2105, 2146, 1785,
    0,    0, 2405,    0, 2406,    0, 2107, 2154, 1983,    0,
    0,    0,    0,    0,    0,    0,    0, 1785, 1623,    0,
 1392, 1944, 2073,    0,    0,    0, 2088,  -15,    0,    0,
 1804, 2092, 2546, 2418, 2419,    0,  766,    0, 2546,14845,
 2420, 2421, 2546, 2426, 2431,14845,14845, 2546, 2435, 2438,
 2439, 2440, 2442, 2451, 2457, 2459, 2462, 2463,    0,    0,
13282, 2466,    0,10730,    0,    0,    0,    0,    0,14845,
 2469,    0,    0,14845,    0,    0,    0,14845,  335,    0,
 2206,    0,14099,10730,14845,    0, 9577, 2546,    0,    0,
    0, 2460,    0,    0,    0,10730,    0,    0, 3480,    0,
    0,  692,    0, 2083,    0,    0,    0,    0,   38, 2483,
    0, 2147,    0,    0,    0,    0,  240,10730,    0,    0,
 2103,    0, 1444, 2546, 2484,    0,   22,    0,    0,    0,
    0,    0,    0,    0,    0, 2486,10730,    0,    0, 2183,
 2194,10730, 1712,    0,    0,  794,10730,    0,    0,    0,
    0,    0,    0,    0, 1921,    0,    0,    0,    0,    0,
 1921,    0,    0,    0,    0,    0,14845, 1157,    0, 2078,
    0, 1157,    0,    0,    0, 2546,    0,    0,    0,    0,
 2546, 2546,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10730, 2205, 2546,    0,    0, 3155, 5052,
    0,    0, 2497,    0, 2423,14591, 1579,  440, 2481,    0,
 2212, 1016,    0,    0, 2495,    0,10730,10730,   83,  203,
    0,    0, 2045, 2214, 9577,    0,    0, 2509,    0,    0,
 9577, 2511,    0,    0,    0,14845,    0,    0,    0,    0,
    0, 1712,  794,    0, 2182,    0,    0,    0,    0, 1944,
    0, 2546,    0,    0,    0, 2517, 2526, 2528,    0,    0,
    0, 1302,    0, 2167, 3480,    0, 2145,    0, 3480,    0,
 3480, 2880,    0,    0,    0,    0,15311,  692,14845, 3480,
  444,10730,    0,   22,10730,    0,    0, 2182,  -16, 1712,
 2216, 2220,    0,    0, 2534,10730,    0,10730, 2538, 2062,
 2540, 1375,15311, 2558,    0,    0,    0,    0, 1594,  165,
 2307,    0, 1867, 1872, 1712,  436,  436,    0, 2312,    0,
 2179,    0,    0,    0,    0, 2573,    0, 3480, 2558,    0,
    0, 2311, 3480, 2326, 2319,15311,10730,    0,    0,    0,
 2170,  744,    0,    0,    0,    0, 1897,15311,  257,10730,
15311,    0,    0,    0,    0,    0,    0,    0,15311,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  206,  677,    0,    0,    0,    0,    0,    0,    0,
    0,10455,  151,12124,    0,    0,    0,    0,    0,    0,
 2626,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  567,    0,    0,    0,    0,    0,    0,  204,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  859,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2295,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   27, 2256,    0,    0,    0,  119,    0,
  119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2259,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  898,    0,    0,14353,12385,    0,    0,    0,  980,
    0,    0,  146,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  723,    0, 2229,  182,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   58,    0,    0,    0,    0,  878,    0,    0,
    0,    0,    0, 1026,    0,    0,    0,    0,    0,  113,
    0, 5837,    0,    0,    0,    0,    0,    0,    0,   58,
    0,  -22,    0,    0, 2597,    0,    0,    0, 2377,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  126,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1168,    0,
 2379,    0,    0,    0,    0,    0,10977,    0,    0,    0,
    0,    0, 1119,    0,    0,    0,    0,    0,    0,11238,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  758,    0,    0, 1170,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   58,    0, 2597,    0, 2377,    0,
 1246,  589,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1258,    0, 2368,    0,    0,    0,
    0, 1278,    0,    0,    0,    0,    0, 2379,   -7,15619,
    0, 1297,    0,    0,    0,    0,  235,    0,    0,    0,
    0, 2195, 2195,    0,    0,    0, 1300,  131,    0,    0,
    0,    0,    0,    0,    0, 2379,    0,    0, 1228,    0,
    0,    0,    0,    0,    0,    0, 9573,    0,    0,  100,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1859,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2665,    0,    0,    0, 2965, 3265,    0, 3565, 3865,
    0,    0,    0,    0,    0,    0,  273,    0,    0,    0,
    0,    0,    0,    0,    0,  219, 4165,    0, 2365,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1342,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10977,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2580,    0, 2329,    0,    0,    0,
    0,    0,    0,  249,    0,  249,    0,    0,  815,    0,
  840,  963,    0,    0,    0,    0,  126,    0, 2379,    0,
    0,    0,    0,    0,    0,    0,  106,  374,    0,    0,
    0,  196,    0,    0, 1207,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2368,    0,
    0, 1244,    0, 2186,    0,    0,  776,    0,    0,    0,
    0,  785,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1344,    0,    0, 3856, 4948,    0,    0,
    0,11913,    0,    0,13400,    0,    0,13892,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1294, 1965,    0,    0,    0,    0,14384,12650,    0,
13892,   58,    0,    0,    0,    0,    0,    0,    0,    0,
   -7, 1308,    0,    0,    0,    0, 2249,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6778, 6924, 7077,
    0,    0,  668,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2617,   56,15083,
    0,15083,    0,    0,    0,15083,15083,15083,    0,    0,
    0,    0,    0, 2210,    0,    0,    0,    0,    0,    0,
 5354, 5643, 2615,    0,   84,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2590,    0,    0,    0,    0, 2391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  313,    0,    0,    0,    0,
  313,    0,    0, 1350,    0,    0,    0,    0,    0,    0,
    0,  938,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  743,    0,    0,    0,    0,    0,
  100,    0,    0,    0,   -7,    0,  684,  134,    0,    0,
    0,    0,  536,    0,    0,    0,    0,    0,    0,    0,
 1899,    0,    0,    0,    0,    0, 2622,    0,    0, 1476,
 2106,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  253,  302,  829,    0,    0,    0,    0,    0,    0,  161,
    0,    0,    0,    0,    0,    0,    0,    0, 4156,    0,
    0,    0,    0,    0,13400,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2379,
    0,    0,  260,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10838,    0,    0,    0,  154,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2628,    0,    0, 1351,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2633,    0,    0,    0,    0,    0, 4465, 5932,
 6221, 4765, 5065,  987,    0,    0,    0,    0,    0,    0,
    0,    0,11499,    0, 2409,    0,    0,    0,    0,    0,
    0,    0, 1358,    0,    0,    0, 1358,    0,    0,    0,
    0,    0,    0,    0,  944,  620,  550,    0,    0,    0,
    0,    0,    0,    0,    0,  570,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   56,  140,    0,
    0,    0,    0,    0,    0,  437,    0, 2195, 2225,    0,
  784,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2391,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1146,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1361,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 4456,    0,    0,    0,    0,
    0,    0, 1159,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1308, 2391, 2195, 1408,   -7,    0, 2417, 1411,
    0,    0,    0,10838,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1154,    0,    0,    0, 1780,    0,
    0,    0,    0,    0,    0,    0,    0, 7149,    0, 7221,
    0, 7293,    0,    0,    0,    0,    0,    0,    0, 7353,
    0,    0, 7428,    0,    0, 7500,    0, 7572,    0,    0,
 7644,    0,    0, 7704,    0,    0, 8553,    0, 2617,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2258,    0,    0,
    0,    0,  746,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  487,    0,    0,    0,
    0,    0,    0,    0,    0, 1424,  166,  216,    0,    0,
    0, 1424,    0, 2260,  320,    0,    0,    0,    0,    0,
    0,    0,    0, 2348, 1214,    0, 6528,    0, 2352,    0,
 2352, 2348, 2352,    0,    0,    0, 1251,    0,    0,    0,
 1351, 2391,    0, 1411,    0,    0, 2243,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1116,    0, 1146, 2295,
    0,    0,    0,    0,    0,  851,  857,  933,  964,  975,
  984,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  211,    0,    0,    0,    0,
 2542,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1433,    0,    0,    0,    0,    0, 1454,    0,    0,    0,
    0,    0,  217,    0,    0,    0,    0,    0,    0,    0,
  299,    0,  308,    0,    0,    0,    0,   84,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7779,    0,
 7851,    0, 7923,    0,    0,    0,    0, 7995,    0,    0,
    0,    0,    0,    0, 1161,    0,    0, 2654,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2656,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,12926,
    0,    0,    0,    0,    0,    0,    0, 1538, 1733,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  509,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1756,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 6428,    0,    0,    0,  592,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  870,    0,
    0,    0,    0,    0,    0,  427,    0,    0,    0,    0,
    0,    0,    0,    0, 2657, 1232, 1336,    0, 2368,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2391,
    0,    0,    0,    0,    0,    0,    0, 1468,  793,    0,
    0,    0,    0,    0,    0,    0,    0, 2391, 2417,    0,
  329,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  510,    0, 8055,    0,    0,    0, 2633,    0, 8130,    0,
    0,    0, 8202,    0,    0,    0,    0, 8274,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2617,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2640,    0,
    0,    0,    0,    0,    0,    0,    0, 2644,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2408,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  320, 8616,    0,    0,
    0,    0, 2348, 1091, 1151,    0, 1254,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2845,    0,    0,  148,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  242,
    0,    0,    0,    0,    0, 8346,    0,    0,    0,    0,
 8406, 8481,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1904, 2673,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  162,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8688,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2845, 1270,    0,  171,    0,    0,    0,    0,    0,
    0, 2657,    0,    0,    0, 2674,    0,    0,    0,    0,
    0,    0,    0, 2658,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   48,    0,    0,    0,
    0,    0,    0, 8705,    0,    0,    0, 1281,    0, 1146,
 1561, 1658,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   48, 2659,    0,    0,    0,    0,    0, 2415,
    0,    0,    0,    0, 2845,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2662,    0,
    0,15181,    0,    0,    0,   48,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   48,    0,    0,
   48,    0,    0,    0,    0,    0,    0,    0,   48,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2723,    0,    0, 2728,
    0,    0,    0,    0,    0,    0,    0,    2,    0, 2729,
   -3, 2592, 2233,   14, -299, 1498, 2593, 1985, 2595, 2234,
    0,    0, 2458, -106,    4,    0,    0,    0,    0,  268,
  318,  -59,  -72,  -58, -260, 2461, 2001,    0,    0,    0,
 1333,    0,    0,  -53, 1633, -545,  -68, -985, 2096, 1871,
 -325,    0, 1629,    0, 2677, 1873, 1917,    0,  -65,    0,
 -519,  -54,    0,    0,    0,    0,    0, 2693,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -548,
 -122, 1653, -345, 1053, -658,    0,    0,    0,    0, -350,
 -137, -542, 1661, -363,  -55,    0, -552,    0,    0, 1230,
-1025,    0, 1227,    0,    0, 2425, -687,   16,    0,    0,
    0,    0, 2392,    0, 2169, 1679,    0,    0,    0, 1736,
    0, 2447, 1994, 1674, 2427, 2429,    0, 1169,    0, -459,
 1973,  -44, -623, 1118, -253,    0,    0, -950, -922, -914,
 -902, -885,    0, 1273,-1079,    0,  973,-1245, 1274,    0,
 2211,    0,    0,    0,  972,  974,  949, -121, -292, -996,
 -951,    0,    0,    0, 2002,    0, -649,    0, 1812,    0,
    0,    0,    0,    0,-1153,-1219,    0,    0,    0,    0,
 -262,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1066,    0,    0, -130,    0,
    0,  970,    0, -392, 1366, 1374, 1171,    0,  998,    0,
    0,    0,    0,    0,    0,    0, 2077, -143, -439, 1547,
 1310, -550, -791, -998,-1365,    0,    0,  -41,    0,    0,
    0,    0,    0, 2450,    0,    0,    0,    0,    0, 2465,
 2240,    0,    0,    0,    0,    0,   -9, -495, 2470,    0,
    0,    0,    5,    0,    0,    0, -373, 1957, -369, 1503,
    0,    0,    0,    0, 2223,    0, -947,    0,    0,    0,
 2190,    0, 1969,    0,    0,    0,    0,    0,    0,  -26,
    0, 2819,    0,    0,    0,    0,    0, 2513,    0,    0,
    0,    0, 2310,    0, 2308,    0,    0,    0, 2066,    0,
    0,    0,    0,    0, -267, -602, -891, 2121,    0,  888,
    0,    0,    0,    0,    0,    0, -229,    0,    0, -754,
 -259, 1370,    0,    0,    0, 2695, 1039,    0, 2079,-1056,
-1533,    0, -904, 1840,-1257,    0,    0,    0,    0, -155,
    0,    0,    0,    0, 1591,    0,    0,    0,    0, 2767,
  362,    0, 1088, 2163,    0, 1276,    0, 1475,    0, -166,
    0,    0, 2072,    0,    0,    0,    0,    0, 2328,    0,
    0,    0, -451,-1183, 2115, 1881,-1116,    0,    0,    0,
 -718, -422,  650,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1095, 1726,    0,
    0,    0, -879,    0,    0,    0, 1609,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -886, 1682,    0,    0,
};
#define YYTABLESIZE 16139
static short yytable[] = {                                     143,
   83,   38,  236,  210,  286,  797,  162,  333,  169,  305,
  308,  232,  228,  577,  247,  304,  307,  642,  252,  988,
  163,  643,  233,  612,  267,  961,  276,  273,  502,  739,
  244,  969,  606,  819,  777,  579, 1038,  798,  275,  271,
 1296,  277, 1228, 1473, 1190,  522,  207,  209,  212,  215,
  162,  220,  223, 1531,  244,  730,  757,  234,  209,  223,
  724,  215,  213,  162,  220, 1294,  251,  143,  223,  760,
 1036,  545, 1429,  216,  240,  584,  263,  221,  162,  265,
  209, 1220,  263,  223,  212,  215,  245,  162,  220,  251,
  143,  223,  163,  215,  339,  992,  936, 1119,  272,  709,
  292,  274,  294,  656, 1285,  736,  279,  849,  270, 1254,
 1321,  889,  159,  581,  730, 1285,  278, 1338,  396,  810,
 1343,  811,  973,  898,  730,  707,  821,   82, 1488, 1124,
  160,  290, 1286,  740,  303,  234,  162,  234,  790,  757,
 1287,  143,  403, 1286,  341,  691,  736,  772,  223,  736,
  578, 1287, 1288,  332,  364,  215,  162,  251,  709,  293,
  186,  169,  772, 1288,  736,   51, 1099,  290,  335, 1289,
  247,  159, 1042,  315,  740, 1290, 1093,  740,  546,  337,
 1289,  743,  392,  757,  707,  725, 1290, 1059,  772,  160,
 1062,  772,  740,  786,   86,  741, 1705,   86,  757, 1069,
 1498,  802,  563,  730,  691,  578,  772,  302, 1811,  578,
  144,  247,  642,  549,  247,   52,  643, 1529,  326,  186,
  328,  775,  743,  876,   51,  848,  306, 1308, 1649,  247,
  303, 1643,  551, 1568,  481,  488,  741,  873,  496,  752,
  743, 1186, 1187,  827,  730, 1058, 1818, 1662,  606,  303,
  612,  725,  385,  903,  741,  502,  794,   94, 1114,  133,
  130,  531,  730,  754,  578, 1522, 1523, 1473,  232,  144,
  234,  857,  899,  234,   52, 1511,   82,  326, 1215,  504,
  143, 1856,   96,  929,   82, 1624, 1634, 1635, 1852,  544,
  363,  534,  870,  481,   64, 1355, 1244, 1306, 1490, 1546,
 1811,  383,  303,  175,  103, 1141,  212,  606,  232,  868,
  162,  385,  763,  899,  931,  578,  899,  791,  133,  529,
 1415,  259,  488, 1417,  233,   88,  592, 1356,  186,  594,
  607,  899,  609,  538,  654, 1709,  976,  932,  148,  794,
 1285, 1285, 1524, 1893,  985, 1556,  660, 1763,  796,  664,
  234,  309,  546, 1499, 1142,  176,  147,  234, 1650,  234,
  383,  234,  338, 1309,  597,  899, 1281,  564, 1286, 1286,
 1100,  763, 1544,  729, 1285,  869, 1287, 1287,  529, 1510,
 1076,  171, 1218,  215,  220,  232, 1560,  186, 1288, 1288,
 1625,  310,  162,  933,  212, 1740,  652, 1307,  673,  143,
  930, 1293, 1286, 1770,  670, 1289, 1289, 1245, 1297,  159,
 1287, 1290, 1290,  725,  729,  662, 1272, 1857, 1569, 1305,
  736,  872, 1288,  104, 1834,  232,  186,  160, 1874,  363,
  130,  130,  729, 1291, 1292, 1271,  742, 1784, 1658, 1289,
  516,  736,  536, 1326,  728, 1290,   14, 1529,  740, 1520,
 1120, 1360, 1361, 1304, 1362, 1363,  159,  186,  143, 1053,
  259, 1136, 1055,  544,  785, 1137, 1673,  850, 1710,  740,
  149, 1143, 1688, 1065,  160,  172, 1123,  742,  173,  709,
 1807, 1057, 1751, 1138, 1851,  186,  371,  144,  162,  215,
  504,  765,  338,  502,  772,  742,  743,  162,  481,  224,
 1083,  162,  751, 1817,  186,  707,  504, 1010,  169,  496,
  741,  749,  186,  803,  707,  289,  385,  743,  730,  757,
  756, 1092,   94,  772,  761,  691, 1139, 1037,  757, 1612,
  880,  741,   81, 1615,  691,  177, 1827,  851,  409,  730,
 1767,  823, 1026, 1695,  234, 1559,  186,   96,  186,  661,
  531,  736, 1248,  531,  852,  691,   86, 1899,  581, 1669,
 1702, 1701, 1337,  328,  244,  383,  623, 1126,  730,  700,
  186,  301, 1347, 1424, 1253,  816,  234,  239,  578,  740,
  898,  816, 1092,  757, 1858, 1711,  606, 1112,  625,  691,
  661,  662, 1519,  661,  177,  186,  730, 1847,  989,  736,
  929, 1663, 1274,  133,  883,  884,  578,  623,  661, 1880,
  700,   51,  709, 1249, 1533,  397,  233,  301,  736,  660,
  326,  159,  875, 1872,  481,  623,  301,  743,  700,  625,
  301, 1066,  662, 1473,  546,  662,  301, 1029,  707,  160,
  772,  741,  385, 1140,   81,  606,  740,  625,  606,  730,
  662,  234,  757,  774,  856, 1541, 1892,  847,  691, 1473,
  660,   52,  856,  247,  606,  753,  610,  706, 1898,  186,
  929, 1901,  186,  763,  899,  143,  143,  794,  660, 1902,
  529,   81,  941,  624,  186, 1715,  796,  929,  729,   81,
  606,  383, 1473,  929,  223,  145,  186, 1276,  488, 1242,
  488,  929,  997, 1072, 1473,  929,  398, 1473,  706,  729,
 1029, 1268, 1558,  563, 1801, 1473,  730,  930, 1295,  943,
  978,  780,  627,  186,  624,  258,  706, 1738,  234,  186,
 1303,  186,  841,  234,  260,  234, 1900,  234,  929,  736,
 1032,  357,  624, 1788,  504,  544,   14, 1047,  268, 1789,
  233,  742,  162,  215, 1668, 1325, 1033,  601, 1367,  146,
 1012,  215, 1269,  627, 1339, 1577,  751,  488,  842, 1074,
  186,  143,  742,  759,  162, 1008,  162,  280,  186,  234,
 1532,  627,  531,  178,  399,  234,  358,  930, 1222, 1285,
  781,  289,  145,  177, 1075, 1077,  244,  538, 1126, 1035,
 1089,  338,  234, 1082,  930,  177, 1881, 1109, 1110, 1111,
  930,  964,  965,  287,  607,  531,  601, 1286,  930,  729,
 1667, 1097,  930,  338, 1422, 1287,  158, 1545,  384,  234,
  234, 1554,  177,  283,  244,  244,  244, 1288,  177,  617,
  177,  365,  178,  399, 1130, 1882,  673,  186,  546, 1606,
  390,  145, 1090, 1048, 1289,  930,  391,  531,  580,  929,
 1290, 1674,  575, 1497,  661, 1675, 1676, 1402,  843,  709,
  598,  162,  301,  607,  661,  576,  661,  379,  611,  177,
  599,  162,  742, 1368,  700,  661,  729,  384,  696,  673,
 1135,  375, 1883,  359, 1113,  670,  700,  576,  617, 1028,
  309, 1703,  623, 1207, 1121,  700,  662,  659,  734,  390,
  709,  360,  531,  735,  366,  391,  662,  580,  662, 1131,
  734,  732, 1049,  733,  625,  735,  159,  662,  709,  661,
  696,  338,  387,  160,  660,  186,  379,  707,  661, 1748,
  940,  123, 1371,  688,  660,  361,  660, 1298, 1299,  700,
  966, 1493, 1494,  338, 1756,  660,  576,  287,  700,  544,
  223, 1029,  602,  386,  376, 1619, 1540,  531,  834, 1474,
  835,  662, 1477, 1672,  389,  123,  930,  802,  707,  585,
  662,  661,  706,  388,  688, 1219,  900,  688,  504,  504,
 1372,  387,  706,  661,  706,  661,  707, 1562, 1348,  660,
 1757,  700,  688,  706,  215, 1210, 1211, 1259,  660,  123,
 1777, 1758,  623, 1154, 1778,  700,  226,  227, 1704,  624,
 1260,  602,  386,  662, 1420,  582, 1328,  900,  642, 1787,
  900, 1310,  643,  389,  625,  662,  778,  662,  585, 1332,
 1895,  178,  388, 1261,  177,  900, 1791,  706,  399,  974,
  974,  660,  282,  178, 1350,  321,  706,  288,  627, 1811,
 1354, 1155,  661,  660,  291,  660,  244, 1311,  371,  145,
  824,  546,  233,  300, 1810, 1329,  496, 1821, 1316,  900,
  178,  244,  700, 1824,  582, 1156,  178,  244,  178, 1317,
  703, 1896,  384,  988, 1263,  601, 1316, 1368, 1368,  706,
 1485, 1485, 1489,  534,  662, 1368, 1368,  234, 1368, 1368,
 1759,  234,  295,  706,  390,  220, 1368, 1368, 1368, 1368,
  391, 1368, 1368, 1368, 1368, 1368, 1368,  178,  601,  624,
 1833,  703,  660, 1835,  703,  301,  379,  730, 1516,  610,
 1518,  379,  731,  730, 1212,  241,  379,  825,  731,  703,
  699,  296,  607,  848,  601,  730,  220,  601,  154,  220,
  731, 1478, 1084,  734,  732,  994,  733,  328,  735,  584,
 1425, 1680, 1476,  601,  220, 1863, 1479,  617,  488,  162,
  706,  233,  544,  827,  709,  607,  241,  826,  974,  241,
  610,  699, 1681, 1395,  848, 1328,  387,  848, 1682,  601,
  535, 1561,  506,  431,  241,  709,  474,  688,  316,  699,
  617,  607,  848,  234,  607,  334, 1430, 1472,  384, 1707,
  504,  215,  220,  504,  251,  143,  328,  386,  584,  342,
  607,  207, 1683,  560, 1475,  561,  617,  994,  389,  617,
  390,  504,  504,  465, 1331,  571,  391,  388,  215,  709,
  705,  506,  707,  704,  536,  617,  607,  406,  688,  343,
  369,  688,  707,  370,  707,  474,  345,  379,  688,  772,
  688,  537,  207,  707,  688,  207, 1854,  407,  349,  688,
  247,  617,  688,  734,  732, 1216,  733,  465,  735,  346,
  207,  705,  178,  508,  704,  546,  403,  688, 1567,  168,
  602,  709,  465, 1572,  571,  371,  800,  309,  367,  705,
  772,  688,  704,  772, 1543,  709,  406,  707,  317,  688,
  378,  247,  387,  688,  247,  904,  707,  255,  772,  347,
  911,  457,  688,  602,  508,  221,  407,  508,  106,  247,
  379,   42, 1660,  169,  309,  381, 1602,  937,  688,   46,
  743,  318,  508,  386,  520,  403, 1790,  768,  168,  602,
  483,  256,  602,  525,  389, 1623,  767,  734,  732,  707,
  733,  309,  735,  388,  108,  688,  221,  319,  602,  221,
  320, 1603,  709,  707,  945,  162,  255,  688,  900,  688,
  390,  232,  220, 1604,  221,  688,  321,  557,  730, 1395,
   42,  562,  169,  731,  602,  703,  544,  135,   46,  743,
  374,  947,  400,  688,  688,  848,  768,  703, 1811,  483,
  256,  209,  322,   40,  688,  688,  703,  688,  402, 1633,
 1135, 1135,  136, 1871,  688,  688,  688,  688,  688,  109,
  688, 1472,  404,  688,  688,  688,  688,  688,  405,  110,
  707,  688,  688,  124,  165,  393,  688,  688,  688,  688,
  688,  688,  688,  688,  474,  699,  135,  207,  848,  374,
  703,  848,  504,  504,  111,  213,  474,  699,  848,  703,
  848,  409,   40, 1755,  848,  166,  699,  232,  332,  848,
  394,  136,  848,  720,  408,  215,  215,  714,  309,  493,
  167,  382, 1769,  721,  722,  723,  497,  848,  232,  474,
  507,  474,  124, 1485,  232, 1513,  213,  499,  730,  213,
  162,  848,  703,  731,  508, 1514,  207,  777,  720,  848,
  699,  663,  848,  848,  213,  383,  703,  384,  949,  699,
  720,  511,  848, 1686,  663,  772,  772,  744,  232, 1735,
  743,  904,  663,  772,  777,  904,  247,  247,  848,  385,
  243,  351,   85,   86,  247,  705,  379,  663,  704,  508,
  508,  766,  514,  352,  508,  768,  379,  508,  512, 1785,
  848,  663,  699, 1016,  386,  848,  705,  153,  154,  704,
  508,  162,  407,  513,  212, 1016,  699,  848, 1016,  848,
  412,  243,  730,  703,  243, 1395,  353,  731,  354,  226,
  227,  232,  221, 1014,  215,  428,  429,  430,  663,  243,
  734,  732, 1502,  733, 1504,  735, 1135, 1746,  748,  517,
  705,  747, 1786,  704,  515,  734,  732, 1806,  733,  705,
  735,  772,  704,  438,  439,  772,  848,  530,  332, 1755,
 1764,  215,  247, 1755, 1793, 1755,  813,  244, 1795,  812,
  532,  546,  772,  699, 1755,  508,  848,  978,  772, 1865,
  848, 1866,  162,  247, 1842,  523, 1828,  212, 1844,  247,
  131,  131,  705,  529, 1755,  704,  508,  546,  533, 1850,
  972, 1813,  508,  736, 1005, 1783,  705, 1004,  244,  704,
  542,  244,  772,  734,  732,  552,  733,  232,  735, 1869,
   14, 1118, 1755,  247, 1117,  554,  244, 1755,   55,  556,
  546,  569,  734,  732,  212,  733,  508,  735,  232,   56,
  663,  568,  546,  910,  232,  546,   57, 1887,  938,  572,
  942,  570, 1889,  546,  946,  948,  950,  209, 1815, 1653,
 1655,  213,  213,  939,  593, 1816,  590,  944,  603,  213,
   58, 1259,  772,  705,  960,  604,  704,  608,  232,  967,
  968,  657,  544,  247, 1260, 1085, 1086,  661, 1021,  815,
  496,  457, 1259,  979,  980,  981,  982,  983,  984,  457,
 1021,  734,  732, 1021,  733, 1260,  735, 1261,  544, 1654,
 1656, 1007,  234, 1839,  663,  734,  732, 1472,  733,  665,
  735, 1262, 1012, 1004, 1122,  234,  994,  894, 1261, 1771,
  815, 1221,   59,  815,  736,  525,  212, 1224,  212,  698,
 1223,  544, 1262, 1472,  700,  105,  243,  243,  815, 1273,
  701,  702, 1117,  544,  243, 1039,  544,  213, 1263,  703,
  704,  557, 1046,   60,  544,  730, 1319,   61, 1014, 1318,
  731, 1324, 1492,  705, 1323,  736, 1472, 1012,  213, 1263,
  730,  706,  707,  106,  213,  731, 1576,  708, 1472,  736,
 1135, 1472,  710, 1617, 1659,  107,  736,  736,  709, 1472,
  971,  734,  732,  712,  733,  711,  735, 1164,  990, 1014,
 1014, 1014, 1014, 1014, 1014, 1014,  720, 1878,  213,  108,
 1877, 1165, 1879,  717, 1805,  747, 1014, 1014, 1014, 1014,
 1014,  718,  719,  736, 1011,  734,  732,  729,  733, 1873,
  735,  737,  243,  244,  244,  738, 1772, 1897,  730,  369,
 1811,  244,  369,  731,  728,  742,   62,  728,  745, 1014,
  746, 1014,  763,  243,  771, 1166,  773,  730,  784,  243,
 1264,  788,  731,  789,  531,  793, 1144, 1145, 1149, 1375,
  795, 1158, 1160, 1162,  109, 1170, 1173, 1176, 1178, 1181,
 1184, 1264,   14, 1167,  110,  796,  799,  801,  563, 1191,
 1192,  804, 1194,  243, 1196, 1610,  805, 1199, 1200, 1201,
 1202, 1203, 1204,  807,  808,  531,  814,  838,  531,  111,
  839,  840,  845, 1159, 1208, 1163, 1213, 1171, 1174,  301,
 1179, 1182, 1185,  531,  860,  112,  730,  853,  854,  244,
  861,  731,  283,  862,  863,  865,  866,  871,  877,  878,
  730,  815,  882,  113,  114,  731,  881, 1168,  891,  893,
  244,  894,  901,  243,  115,  116,  244,  117,  457,  899,
  900,  905,  175,  962,  118,  119, 1206,  120,  121,  970,
  122, 1587,  974,  123,  124,  125,  126,  127,  872,  303,
  993,  128,  129,  998,  999, 1000, 1002,  130,  131,  132,
  244,  133,  134,  135,  815,  734,  732,  815,  733, 1006,
  735, 1868,  754, 1025,  815,  498,  815, 1198,  734,  732,
  815,  733,  747,  735, 1030,  815, 1040, 1043,  815,  454,
 1014, 1054, 1014, 1014, 1063, 1064,  730, 1066,  817, 1014,
 1070,  731, 1071,  815, 1088, 1014, 1073, 1079, 1080, 1081,
 1014, 1095, 1014, 1014, 1106, 1115,  498,  815, 1108,  498,
  244,  734,  732, 1217,  733,  815,  735, 1014,  815,  815,
  730, 1101, 1014, 1014,  498,  731, 1014, 1129,  815, 1151,
 1188, 1189, 1197, 1014, 1195, 1014, 1014,  734,  732, 1014,
  733, 1225,  735, 1014,  815, 1014, 1205, 1014,  730, 1014,
 1227, 1243, 1251, 1252, 1014, 1014, 1256, 1014, 1258, 1246,
 1277, 1280, 1266,  457, 1300, 1302,  815, 1312, 1322, 1320,
 1327,  815, 1014, 1014, 1333, 1014, 1378, 1379, 1334, 1381,
 1383, 1335, 1336,  815, 1352,  815, 1014, 1388, 1014, 1344,
 1014, 1014, 1014, 1123, 1014, 1357, 1358, 1014, 1014, 1359,
  531,  531, 1365, 1366, 1370,  531, 1373, 1014,  531, 1406,
  734,  732, 1374,  733, 1376,  735, 1377, 1413, 1414, 1213,
 1014,  531, 1213, 1014, 1208, 1384, 1385, 1386, 1421,  457,
 1423, 1387,  815,  817, 1389, 1390, 1391, 1392, 1393, 1014,
 1394,  929, 1416, 1419, 1495, 1014, 1014, 1400,  734,  732,
 1014,  733,  815,  735,  734,  732,  815,  733, 1482,  735,
 1483, 1014, 1014, 1480, 1014, 1496,  769, 1500,  457, 1505,
 1014,  734,  732, 1618,  733, 1506,  735,  769,  951,  952,
  953,  954,  955,  956,  957,  958, 1534, 1507, 1014, 1014,
  730, 1508, 1517,  585, 1530,  731,  531, 1542, 1535, 1014,
 1014, 1536, 1014,  730, 1537, 1538, 1539, 1525,  731, 1014,
 1014, 1014, 1014, 1014, 1547, 1014, 1092,  531, 1014, 1014,
 1014, 1014, 1014,  531,  994, 1557, 1014, 1014, 1563, 1565,
 1601, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1566,
 1571,  498,  498, 1578, 1580, 1600,  730, 1607, 1613,  498,
 1614,  731, 1620, 1621, 1626, 1627,  146,  531, 1639, 1193,
 1629, 1641,  498, 1652, 1630,  994,  994,  994,  994,  994,
 1007,  994,  730, 1637, 1657, 1638, 1573,  731, 1640, 1646,
 1647, 1648, 1579,  994,  994,  994,  994, 1583, 1651, 1661,
 1677, 1687, 1692, 1690, 1588, 1401,  734,  732, 1693,  733,
 1689,  735, 1691, 1694,  112, 1696, 1697, 1404,  734,  732,
 1706,  733, 1708,  735, 1698,  994, 1712,  994, 1713, 1714,
 1717, 1718,  113,  114,  734,  732, 1719,  733, 1213,  735,
 1364, 1720, 1213,  115,  116, 1723,  117,  498, 1724, 1725,
 1726, 1803, 1727,  118,  119,  730,  120,  121, 1628,  122,
  731, 1728,  123,  124,  125,  126,  127, 1729,  498, 1730,
  128,  129, 1731, 1732,  498, 1734,  130,  131,  132, 1737,
  133,  134,  135,  112, 1586, 1405,  734,  732, 1750,  733,
 1743,  735, 1765,  730, 1766, 1642, 1779, 1776,  731,  730,
 1781,  113,  114, 1782,  731, 1794, 1802, 1798,  498, 1808,
 1812,  531,  115,  116, 1819,  117,  730, 1809, 1822, 1820,
 1825,  731,  118,  119,  112,  120,  121, 1829,  122, 1664,
 1836,  123,  124,  125,  126,  127, 1837, 1838, 1840,  128,
  129, 1843,  113,  114, 1864,  130,  131,  132, 1867,  133,
  134,  135,  531,  115,  116,  531,  117,  734,  732,  531,
  733, 1859,  735,  118,  119, 1861,  120,  121, 1870,  122,
  531, 1811,  123,  124,  125,  126,  127, 1876, 1753, 1856,
  128,  129, 1857, 1886, 1888, 1890,  130,  131,  132, 1891,
  133,  134,  135,  656, 1894,    1,  994,  736,  994,  994,
  531,   28, 1716,  531,   30,  994,  193,  988, 1721, 1722,
  772,  994,  261,  371,  156,  652,  994,  462,  531,  994,
  531,   81,  973, 1407,  734,  732,  715,  733,  899,  735,
  272,  203, 1736,  994, 1018,  994, 1213,  714,  994,  994,
 1739,  730,  994,  743,  183, 1745,  731, 1747,  271,  994,
  399,  994,  994,  730,  972,  994,  696,  529,  731,  994,
  698,  994,  184,  994,  716,  994,  953,  372,  213,  730,
  994,  994,  323,  994,  731, 1018, 1018, 1018, 1018, 1018,
 1018, 1018,  288,  952,  725,  343,  331,  358,  994,  994,
  329,  994,   27, 1018, 1018, 1018, 1018,   30,   40, 1408,
  734,  732,  994,  733,  994,  735,  994,  994,  994, 1484,
  994,  297,  298,  994,  994,  299,  762, 1009,  506,  764,
  505,  730, 1351,  994, 1001, 1018,  731, 1018, 1353, 1792,
  892, 1409,  734,  732, 1128,  733,  994,  735,  269,  994,
  254, 1133, 1091, 1349, 1753, 1700, 1341, 1699, 1753,  616,
 1845,  574,  855,  550, 1330,  994, 1279, 1741, 1041, 1753,
 1342,  994,  994, 1068, 1780,  587,  994,  588,  457, 1679,
 1855,  833, 1685, 1862, 1860, 1885, 1229,  994,  994, 1753,
  994, 1804, 1632, 1061, 1762, 1848,  994,  531,  531,  990,
 1512, 1670,  730,  595,  571,  531,  580,  731, 1664, 1107,
 1555,  152,  837, 1098,  994,  994,  543, 1753,  531,  859,
  879,  779, 1753,  783,  241,  994,  994,  531,  994, 1031,
  314, 1027, 1503, 1255,  264,  994,  994,  994,  994,  994,
  934,  994,  531, 1826,  994,  994,  994,  994,  994, 1599,
 1045, 1849,  994,  994,  792, 1007, 1733,  994,  994,  994,
  994,  994,  994,  994,  994,  241,  531, 1250,  241,  730,
 1521, 1380,  531, 1418,  731,  531,  656,    0,    0,    0,
 1410,  734,  732,  241,  733,    0,  735,    0, 1411,  734,
  732,    0,  733,  531,  735,    0,    0,    0,    0,    0,
    0,  987,    0,    0,    0,    0, 1018,    0, 1018, 1018,
    0,    0,    0,    0,  531, 1018,    0,  531,    0,    0,
  531, 1018,    0,    0,    0,    0, 1018,    0,    0, 1018,
 1412,  734,  732,    0,  733,    0,  735,    0,    0,    0,
    0,    0,    0, 1018, 1023,  730,    0,    0, 1018, 1018,
  731,    0, 1018,    0,  531,    0,    0,    0,    0, 1018,
    0, 1018, 1018,    0,    0, 1018,    0,    0,    0, 1018,
    0, 1018,  531, 1018,    0, 1018,    0,  730,    0,  531,
 1018, 1018,  731, 1018, 1754, 1023, 1023, 1023, 1023, 1023,
 1023, 1023,    0,    0,    0,    0,    0,  531, 1018, 1018,
    0, 1018,    0, 1023, 1023, 1023, 1023,    0,    0,    0,
    0,  659, 1018,    0, 1018,    0, 1018, 1018, 1018,    0,
 1018,    0,    0, 1018, 1018,    0,    0,  531,    0,  659,
  659,  531,    0, 1018,    0, 1023,    0, 1023,    0,    0,
  659,  659,    0,  659,    0,    0, 1018,    0,    0, 1018,
  659,  659,    0,  659,  659,    0,  659,    0,    0,  659,
  659,  659,  659,  659,    0, 1018,    0,  659,  659,    0,
    0, 1018, 1018,  659,  659,  659, 1018,  659,  659,  659,
    0,    0,    0,    0,    0,    0,    0, 1018, 1018,    0,
 1018, 1608,  734,  732,    0,  733, 1018,  735,    0,    0,
  241,  241,    0, 1609,  734,  732,    0,  733,  241,  735,
    0,    0,    0,    0, 1018, 1018,  730,    0,    0,    0,
    0,  731,    0,    0,  730, 1018, 1018,    0, 1018,  731,
    0,    0,    0,    0,    0, 1018, 1018, 1018, 1018, 1018,
    0, 1018,    0,    0, 1018, 1018, 1018, 1018, 1018,    0,
 1754,    0, 1018, 1018, 1754,    0, 1846, 1018, 1018, 1018,
 1018, 1018, 1018, 1018, 1018, 1754,  730,    0, 1616,  734,
  732,  731,  733,    0,  735, 1800,  734,  732,    0,  733,
    0,  735,    0,    0,    0, 1754,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  241,    0,    0,    0,
    0,    0,    0,    0,   82,    0, 1023,    0, 1023, 1023,
    0,    0,    0, 1754,    0, 1023,    0,  241, 1754,    0,
    0, 1023,    0,  241,    0,    0, 1023,    0,    0, 1023,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1023, 1019,    0,    0,    0, 1023, 1023,
    0,    0, 1023,    0,    0,    0,    0,  241,    0, 1023,
    0, 1023, 1023,    0,    0, 1023,    0,    0,    0, 1023,
    0, 1023,    0, 1023,    0, 1023,    0,    0,    0,    0,
 1023, 1023,   66, 1023,    0, 1019, 1019, 1019, 1019, 1019,
 1019, 1019,    0,   67,    0,    0,    0,    0, 1023, 1023,
   68, 1023,    0, 1019, 1019, 1019, 1019,    0,    0,    0,
    0,  112, 1023,   69, 1023,    0, 1023, 1023, 1023,    0,
 1023,    0,    0, 1023, 1023,    0,    0,  730,    0,  113,
  114,    0,  731, 1023,    0, 1019,    0, 1019,    0,  730,
  115,  116,    0,  117,  731,    0, 1023,    0,    0, 1023,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0, 1023,    0,  128,  129,    0,
   70, 1023, 1023,  130,  131,  132, 1023,  133,  134,  135,
    0,    0,    0,    0,    0,    0,   71, 1023, 1023,    0,
 1023,    0,    0,    0,    0,    0, 1023,    0,    0,    0,
    0,    0,   72,    0,  730,    0,    0,    0,    0,  731,
    0,  730,    0,    0, 1023, 1023,  731,   73,    0,    0,
    0,   74,    0,    0,   75, 1023, 1023,    0, 1023,    0,
    0,    0,   76,    0,    0, 1023, 1023, 1023, 1023, 1023,
    0, 1023,    0,    0, 1023, 1023, 1023, 1023, 1023,    0,
    0,   77, 1023, 1023,    0,    0,    0, 1023, 1023, 1023,
 1023, 1023, 1023, 1023, 1023, 1574, 1575,    0,    0,    0,
    0,    0,    0, 1581, 1582,    0, 1584, 1585,    0,    0,
    0,    0,    0,    0, 1589, 1590, 1591, 1592,    0, 1593,
 1594, 1595, 1596, 1597, 1598,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1019,    0, 1019, 1019,
   78,    0,    0,    0,    0, 1019,    0,  454,    0,    0,
    0, 1019,    0,    0,    0,    0, 1019,    0,    0, 1019,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1019, 1032,    0,    0,    0, 1019, 1019,
    0,    0, 1019,    0,    0,    0,    0,    0,    0, 1019,
    0, 1019, 1019,    0,    0, 1019,    0,    0,    0, 1019,
    0, 1019,    0, 1019,    0, 1019,    0,    0,    0,    0,
 1019, 1019,    0, 1019,    0, 1032, 1032, 1032, 1032, 1032,
 1032, 1032,    0,    0,    0,    0,    0,    0, 1019, 1019,
    0, 1019,    0, 1032, 1032, 1032, 1032,    0,    0,   81,
    0,  112, 1019,    0, 1019,    0, 1019, 1019, 1019,    0,
 1019,    0,    0, 1019, 1019,    0,    0,    0,    0,  113,
  114,    0,    0, 1019,    0, 1032,    0, 1032,    0,    0,
  115,  116,    0,  117,    0,    0, 1019,    0,    0, 1019,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0, 1019,    0,  128,  129,    0,
    0, 1019, 1019,  130,  131,  132, 1019,  133,  134,  135,
    0,    0,    0,    0,    0,    0,    0, 1019, 1019,    0,
 1019,    0,   89,    0,    0,    0, 1019,    0,    0,   90,
    0,    0,    0,   91,   92,    0,    0,    0,    0,    0,
    0,    0,   93,    0, 1019, 1019,    0,    0,    0,    0,
    0,    0,    0,   94,    0, 1019, 1019,    0, 1019,    0,
    0,    0,    0,    0,    0, 1019, 1019, 1019, 1019, 1019,
    0, 1019,    0,    0, 1019, 1019, 1019, 1019, 1019,    0,
    0,    0, 1019, 1019,    0,    0,    0, 1019, 1019, 1019,
 1019, 1019, 1019, 1019, 1019,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   95,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   96,    0,    0,    0,    0, 1032,    0, 1032, 1032,
    0,    0,    0,    0,    0, 1032,   97,    0,    0,    0,
   98, 1032,    0,    0,    0,    0, 1032,    0,    0, 1032,
    0,   99,    0,    0,    0,  529,    0,    0,    0,    0,
    0,    0,    0, 1032, 1037,    0,    0,    0, 1032, 1032,
  100,    0, 1032,    0,    0,    0,    0,    0,    0, 1032,
    0, 1032, 1032,    0,    0, 1032,    0,    0,    0, 1032,
    0, 1032,    0, 1032,    0, 1032,  529,    0,    0,  529,
 1032, 1032,    0, 1032,    0, 1037, 1037, 1037, 1037, 1037,
 1037, 1037,    0,    0,  529,    0,  529,    0, 1032, 1032,
    0, 1032,    0, 1037, 1037, 1037, 1037,    0,    0,  101,
    0,  112, 1032,    0, 1032,    0, 1032, 1032, 1032,    0,
 1032,    0,    0, 1032, 1032,    0,    0,    0,    0,  113,
  114,    0,    0, 1032,    0, 1037,    0, 1037,    0,    0,
  115,  116,    0,  117,    0,    0, 1032,    0,    0, 1032,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0, 1032,    0,  128,  129,    0,
    0, 1032, 1032,  130,  131,  132, 1032,  133,  134,  135,
    0,    0,    0,    0,    0,    0,    0, 1032, 1032,    0,
 1032,    0,    0,    0,    0,    0, 1032,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1032, 1032,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1032, 1032,    0, 1032,    0,
    0,    0,    0,    0,    0, 1032, 1032, 1032, 1032, 1032,
    0, 1032,    0,    0, 1032, 1032, 1032, 1032, 1032,    0,
    0,    0, 1032, 1032,    0,    0,    0, 1032, 1032, 1032,
 1032, 1032, 1032, 1032, 1032,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  529,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1037,    0, 1037, 1037,
  529,  529,  529,    0,    0, 1037,  529,    0,    0,  529,
    0, 1037,    0,    0,    0,    0, 1037,    0,    0, 1037,
    0,    0,  529,    0,    0,  526,    0,    0,  529,    0,
    0,    0,    0, 1037,  867,    0,    0,    0, 1037, 1037,
    0,    0, 1037,    0,    0,    0,    0,    0,    0, 1037,
    0, 1037, 1037,    0,    0, 1037,    0,    0,    0, 1037,
    0, 1037,    0, 1037,    0, 1037,  526,    0,    0,  526,
 1037, 1037,    0, 1037,    0,  867,  867,  867,  867,  867,
    0,  867,    0,    0,  526,    0,  526,    0, 1037, 1037,
    0, 1037,    0,  867,  867,  867,  867,  529,    0,    0,
    0,    0, 1037,    0, 1037,    0, 1037, 1037, 1037,    0,
 1037,    0,    0, 1037, 1037,    0,    0,    0,  529,    0,
    0,    0,    0, 1037,  529,    0,    0,  867,    0,    0,
    0,    0,    0,  529,    0,  529, 1037,    0,    0, 1037,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  529,    0, 1037,    0,    0,  529,    0,
    0, 1037, 1037,    0,    0,    0, 1037,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1037, 1037,    0,
 1037,    0,    0,    0,    0,    0, 1037,  529,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1037, 1037,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1037, 1037,    0, 1037,    0,
    0,    0,    0,    0,    0, 1037, 1037, 1037, 1037, 1037,
    0, 1037,    0,    0, 1037, 1037, 1037, 1037, 1037,    0,
    0,    0, 1037, 1037,    0,    0,    0, 1037, 1037, 1037,
 1037, 1037, 1037, 1037, 1037,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  526,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  867,    0,  867,  867,
  526,  526,  526,    0,    0,  867,  526,    0,    0,  526,
    0,  867,    0,    0,    0,    0,  867,    0,    0,  867,
    0,    0,  526,    0,    0,  531,    0,    0,  526,    0,
    0,    0,    0,  867,  880,    0,    0,    0,  867,  867,
    0,    0,  867,    0,    0,    0,    0,    0,    0,  867,
    0,  867,  867,    0,    0,  867,    0,    0,    0,  867,
    0,  867,    0,  867,    0,  867,  531,    0,    0,  531,
  867,  867,    0,  867,    0,  880,  880,  880,  880,  880,
    0,  880,    0,    0,  531,    0,  531,    0,  867,  867,
    0,  867,    0,  880,  880,  880,  880,  526,    0,    0,
    0,    0,  867,    0,  867,    0,  867,  867,  867,    0,
  867,    0,    0,  867,  867,    0,    0,    0,  526,    0,
    0,    0,    0,  867,  526,    0,    0,  880,    0,    0,
    0,    0,    0,    0,    0,  526,  867,    0,    0,  867,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  526,    0,  867,    0,    0,  526,    0,
    0,  867,  867,    0,    0,    0,  867,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  867,  867,    0,
  867,    0,    0,    0,    0,    0,  867,  526,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  867,  867,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  867,  867,    0,  867,    0,
    0,    0,    0,    0,    0,  867,  867,  867,  867,  867,
    0,  867,    0,    0,  867,  867,  867,  867,  867,    0,
    0,    0,  867,  867,    0,    0,    0,  867,  867,  867,
  867,  867,  867,  867,  867,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  531,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  880,    0,  880,  880,
    0,  531,  531,    0,    0,  880,  531,    0, 1622,  531,
    0,    0,    0,    0,    0,    0,  880,    0,    0,  880,
    0,    0,  531,    0,    0,    0,    0,    0,  531,    0,
    0,    0,    0,  880,  883,    0,    0,    0,  880,  880,
    0,    0,  880,    0,    0,    0,    0,    0,    0,  880,
    0,  880,  880,    0,    0,  880,    0,    0,    0,  880,
    0,  880,    0,  880,    0,  880,    0,    0,    0,    0,
  880,  880,    0,  880,    0,  883,  883,  883,  883,  883,
    0,  883,    0,    0,    0,    0,    0,    0,  880,  880,
    0,  880,    0,  883,  883,  883,  883,  531,    0,    0,
    0,    0,  880,    0,  880,    0,  880,  880,  880,    0,
  880,    0,    0,  880,  880,    0,    0,    0,  531,    0,
    0,    0,    0,  880,  531,    0,    0,  883,    0,    0,
    0,    0,    0,    0,    0,  531,  880,    0,    0,  880,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  531,    0,  880,    0,    0,  531,    0,
    0,  880,  880,    0,    0,    0,  880,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  880,  880,    0,
  880,    0,    0,    0,    0,    0,  880,  531,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  880,  880,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  880,  880,  541,  880,    0,
    0,    0,    0,    0,    0,  880,  880,  880,  880,  880,
    0,  880,    0,    0,  880,  880,  880,  880,  880,    0,
    0,    0,  880,  880,    0,    0,    0,  880,  880,  880,
  880,  880,  880,  880,  880,    0,    0,  541,  541,    0,
    0,  541,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  541,    0,  541,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  883,    0,  883,  883,
    0,    0,    0,    0,    0,  883,    0,    0,  541,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  883,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  883,  884,    0,    0,    0,  883,  883,
    0,    0,  883,    0,    0,    0,    0,    0,    0,  883,
    0,  883,  883,    0,    0,  883,    0,    0,    0,  883,
    0,  883,    0,  883,    0,  883, 1282,    0,    0,    0,
  883,  883,    0,  883,    0,  884,  884,  884,  884,  884,
    0,  884,    0,    0,    0,    0,    0,    0,  883,  883,
    0,  883,    0,  884,  884,  884,  884,    0,    0,    0,
    0,  112,  883,    0,  883,    0,  883,  883,  883,    0,
  883,    0,    0,  883,  883,    0,    0,    0,    0,  113,
  114,    0,    0,  883,    0,    0,    0,  884,    0,    0,
  115,  116,    0,  117,    0,    0,  883,    0,    0,  883,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0,  883,    0,  128,  129,    0,
    0,  883,  883,  130,  131,  132,  883,  133,  134,  135,
    0,    0,    0,    0,    0,  541,    0,  883,  883,    0,
  883,    0,    0,    0,    0,    0,  883,    0,    0,    0,
  541,    0,  541,  541,  541,    0,    0,    0,  541,    0,
    0,  541,    0,    0,  883,  883,    0,    0,    0,    0,
    0,    0,    0,    0,  541,  883,  883,    0,  883,    0,
  541,    0,    0,  541,    0,  883,  883,  883,  883,  883,
    0,  883,    0,    0,  883,  883,  883,  883,  883,    0,
    0,    0,  883,  883,    0,    0,    0,  883,  883,  883,
  883,  883,  883,  883,  883,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  541,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  541,
    0,    0,    0,    0,    0,    0,  884, 1631,  884,  884,
    0,    0,    0,    0,    0,  884,    0,    0,    0,    0,
  541,    0,    0,  416,    0,    0,  541,    0,    0,  884,
  541,    0,    0,  878,    0,    0,    0,  541,    0,    0,
    0,    0,    0,  884,    0,    0,    0,    0,  884,  884,
    0,  418,  884,    0,    0,  541,    0,    0,    0,  884,
  541,  884,  884,    0,    0,  884,    0,    0,    0,  884,
    0,  884,    0,  884,  878,  884,  878,  878,  878,  420,
  884,  884,    0,  884,    0,  541,    0,    0,    0,  541,
    0,    0,  878,  878,  878,  878,    0,    0,  884,  884,
    0,  884,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  884,    0,  884,    0,  884,  884,  884,    0,
  884,    0,    0,  884,  884,    0,  878,    0,    0,    0,
    0,    0,    0,  884,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  884,    0,    0,  884,
  425,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  884,  427,    0,    0,    0,
    0,  884,  884,    0,    0,    0,  884,    0,    0,    0,
  428,  429,  430,    0,  226,  227,    0,  884,  884,    0,
  884,    0,    0,    0,    0,    0,  884,    0,    0,    0,
    0,    0,    0,  433,  434,  435,  436,  437,  438,  439,
  440,  441,    0,    0,  884,  884,    0,    0,  443,  444,
    0,    0,    0,    0,    0,  884,  884,  448,  884,    0,
    0,    0,    0,    0,    0,  884,  884,  884,  884,  884,
    0,  884,    0,    0,  884,  884,  884,  884,  884,    0,
    1,    0,  884,  884,    0,    0,    0,  884,  884,  884,
  884,  884,  884,  884,  884,    0,    0,    0,    2,    0,
    0,    0,    0,    0,    0,    0,    3,    0,    0,    0,
    0,    0,    0,    4,    0,    5,    0,    6,    0,    0,
    0,    0,    7,    0,    0,  878,    0,  878,  878,    8,
    0,    0,    0,    0,  878,    0,    0,    0,    0,    0,
    0,    0,    0,    9,    0,    0,    0,    0,  878,    0,
    0,    0,  877,   10,    0,    0,    0,    0,    0,    0,
    0,    0,  878,    0,    0,    0,    0,  878,  878,    0,
    0,  878,    0,    0,    0,    0,    0,    0,  878,    0,
  878,  878,    0,    0,  878,    0,    0,    0,  878,    0,
  878,    0,  878,  877,  878,  877,  877,  877,    0,  878,
  878,    0,  878,    0,    0,    0,    0,    0,    0,    0,
    0,  877,  877,  877,  877,    0,   11,  878,  878,    0,
  878,    0,    0,   12,    0,    0,   13,    0,   14,   15,
  112,  878,    0,  878,    0,  878,  878,  878,    0,  878,
    0,    0,  878,  878,    0,  877,    0,    0,  113,  114,
    0,    0,  878,   16,    0,    0,    0,    0,    0,  115,
  116,    0,  117,    0,    0,  878,    0,    0,  878,  118,
  119,    0,  120,  121,    0,  122,    0,    0,  123,  124,
  125,  126,  127,    0,  878,    0,  128,  129,    0,    0,
  878,  878,  130,  131,  132,  878,  133,  134,  135,    0,
    0,    0,   17,    0,    0,    0,  878,  878,    0,  878,
    0,    0,    0,    0,    0,  878,    0,   18,   19,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  617,
    0,    0,  618,  878,  878,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  878,  878,    0,  878,    0,  619,
    0,  585,  620,    0,  878,  878,  878,  878,  878,    0,
  878,    0,    0,  878,  878,  878,  878,  878,    0,    0,
    0,  878,  878,    0,    0,    0,  878,  878,  878,  878,
  878,  878,  878,  878,    0,    0,    0,    0,    0,    0,
    0,    0,  621,    0,    0,    0,    0,    0,  622,  623,
  624,    0,  625,  626,  627,    0,  628,    0,    0,    0,
    0,    0,    0,    0,  877,    0,  877,  877,    0,    0,
    0,    0,    0,  877,  629,    0,  630,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  877,    0,    0,
    0,  879,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  877,    0,    0,    0,  631,  877,  877,    0,    0,
  877,    0,    0,    0,    0,    0,    0,  877,    0,  877,
  877,    0,  632,  877,    0,    0,    0,  877,    0,  877,
    0,  877,  879,  877,  879,  879,  879,    0,  877,  877,
    0,  877,    0,    0,  633,    0,    0,    0,    0,    0,
  879,  879,  879,  879,    0,    0,  877,  877,    0,  877,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  112,
  877,    0,  877,    0,  877,  877,  877,    0,  877,  634,
  635,  877,  877,    0,  879,    0,    0,  113,  114,    0,
    0,  877,    0,    0,    0,    0,  636,    0,  115,  116,
    0,  117,    0,    0,  877,    0,    0,  877,  118,  119,
    0,  120,  121,    0,  122,    0,    0,  123,  124,  125,
  126,  127,    0,  877,    0,  128,  129,    0,    0,  877,
  877,  130,  131,  132,  877,  133,  134,  135,    0,    0,
    0,    0,    0,    0,    0,  877,  877,    0,  877,    0,
    0,    0,    0,    0,  877,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  141,    0,
    0,  141,  877,  877,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  877,  877,    0,  877,    0,  141,    0,
    0,  141,    0,  877,  877,  877,  877,  877,    0,  877,
    0,    0,  877,  877,  877,  877,  877,    0,    0,    0,
  877,  877,    0,    0,    0,  877,  877,  877,  877,  877,
  877,  877,  877,    0,    0,    0,    0,    0,    0,    0,
    0,  141,    0,    0,    0,    0,    0,  141,  141,  141,
    0,  141,  141,  141,    0,  141,    0,    0,    0,    0,
    0,    0,    0,  879,    0,  879,  879,    0,    0,    0,
    0,    0,  879,  141,    0,  141,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  879,    0,    0,    0,
  882,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  879,    0,    0,    0,  141,  879,  879,    0,    0,  879,
    0,    0,    0,    0,    0,    0,  879,    0,  879,  879,
    0,  141,  879,    0,    0,    0,  879,    0,  879,    0,
  879,  882,  879,  882,  882,  882,    0,  879,  879,    0,
  879,    0,    0,  141,    0,    0,    0,    0,    0,  882,
  882,  882,  882,    0,    0,  879,  879,    0,  879,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  879,
    0,  879,    0,  879,  879,  879,    0,  879,  141,  141,
  879,  879,    0,  882,    0,    0,    0,    0,    0,    0,
  879,    0,    0,    0,    0,  141,    0,    0,  971,  734,
  732,    0,  733,  879,  735,    0,  879,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  927,  926,  928,
    0,    0,  879,    0,    0,    0,    0,    0,  879,  879,
    0,    0,    0,  879,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  879,  879,    0,  879,    0,    0,
    0,    0,    0,  879,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  879,  879,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  879,  879,    0,  879,    0,    0,    0,    0,
    0,    0,  879,  879,  879,  879,  879,  659,  879,    0,
    0,  879,  879,  879,  879,  879,    0,    0,    0,  879,
  879,    0,    0,    0,  879,  879,  879,  879,  879,  879,
  879,  879,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  659,  659,    0,
    0,  659,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  882,    0,  882,  882,  659,    0,    0,    0,
    0,  882,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  882,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  882,
    0,    0,    0,    0,  882,  882,    0,  686,  882,    0,
    0,    0,    0,    0,    0,  882,    0,  882,  882,    0,
    0,  882,    0,    0,    0,  882,    0,  882,    0,  882,
    0,  882,    0,    0,    0,    0,  882,  882,  914,  882,
    0,    0,    0,    0,  730,    0,    0,    0,  686,  731,
    0,  686,  915,    0,  882,  882,    0,  882,    0,    0,
    0,    0,    0,    0,    0,    0,  686,    0,  882,    0,
  882,    0,  882,  882,  882,    0,  882,    0,    0,  882,
  882,    0,    0,    0,    0,    0,    0,    0,  916,  882,
    0,    0,    0,    0,    0,    0,  917,    0,    0,    0,
    0,    0,  882,    0,  918,  882,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,  919,    0,  920,    0,    0,  882,  882,    0,
    0,    0,  882,    0,    0,    0,    0,  921,    0,  922,
  923,  924,    0,  882,  882,    0,  882,    0,    0,    0,
    0,    0,  882,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  882,  882,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,  882,    0,  882,    0,    0,    0,  925,    0,
    0,  882,  882,  882,  882,  882,    0,  882,    0,    0,
  882,  882,  882,  882,  882,    0,    0,    0,  882,  882,
    0,    0,    0,  882,  882,  882,  882,  882,  882,  882,
  882,    0,  659,    0,    0,  659,    0,    0,    0,    0,
    0,    0,  659,    0,  659,    0,    0,    0,  659,    0,
    0,    0,    0,  659,    0,    0,  659,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  858,    0,    0,
    0,  659,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  659,    0,    0,    0,    0,
    0,    0,    0,  659,    0,    0,    0,  659,    0,    0,
    0,    0,    0,    0,    0,    0,  659,    0,  858, 1026,
 1026,  858, 1026, 1026, 1026,    0,    0,    0,    0,    0,
    0,    0,  659,    0,    0,    0,  858, 1026, 1026, 1026,
    0,    0,  686,    0,    0,  686,    0,    0,    0,    0,
    0,    0,  686,    0,  686,    0,    0,    0,  686,  659,
    0,    0,    0,  686,    0,    0,  686,    0, 1026,    0,
    0,  659,    0,  659,    0,    0,    0,    0,    0,  659,
    0,  686,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  686,    0,  659,  659,    0,
    0,    0,    0,  686,    0,    0,    0,  686,  659,  659,
    0,  659,    0,    0,    0,    0,  686,    0,  659,  659,
  659,  659,  659,  859,  659,    0,    0,  659,  659,  659,
  659,  659,  686,    0,    0,  659,  659,    0,    0,    0,
  659,  659,  659,  659,  659,  659,  659,  659,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  686,
    0,    0,    0,    0,  859, 1027, 1027,  859, 1027, 1027,
 1027,  686,    0,  686,    0,    0,    0,    0,    0,  659,
    0,    0,  859, 1027, 1027, 1027,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  659,  659,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  659,  659,
    0,  659,    0,    0, 1027,    0,    0,    0,  659,  659,
  686,  659,  659,    0,  659,    0,    0,  659,  659,  659,
  659,  659,    0,    0,    0,  659,  659,    0,    0,  858,
  686,  659,  659,  659,  686,  659,  659,  659, 1026,    0,
    0,    0,    0,    0, 1026,    0,    0,    0,    0, 1026,
    0,    0, 1026,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  860,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  858,    0,    0,  858,    0,    0, 1026,    0,
    0,    0,  858,    0,  858,    0, 1026,    0,  858,    0,
    0,    0,    0,  858, 1026,    0,  858,  860, 1028, 1028,
  860, 1028, 1028, 1028,    0,    0,    0,    0,    0,    0,
    0,  858, 1026,    0, 1026,  860, 1028, 1028, 1028,    0,
    0,    0,    0,    0,    0,  858,    0, 1026,  816, 1026,
 1026, 1026,    0,  858,    0,    0,  858,  858,    0,    0,
    0,    0,    0,    0,    0,    0,  858, 1028,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  858,    0,    0,  859,    0,    0,    0,  816,
    0,    0,  816,    0, 1027,    0,    0,    0, 1026,    0,
 1027,    0,    0,    0,  858, 1027,    0,  816, 1027,  858,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  842,  858,    0,  858,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  859,    0,
    0,  859,    0,    0, 1027,    0,    0,    0,  859,    0,
  859,    0, 1027,    0,  859,    0,    0,    0,    0,  859,
 1027,  842,  859,    0,  842,    0,    0,    0,    0,    0,
  858,    0,    0,    0,    0,    0,    0,  859, 1027,  842,
 1027,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  858,  859,  819, 1027,  858, 1027, 1027, 1027,    0,  859,
    0,    0,  859,  859,    0,    0,    0,    0,    0,    0,
    0,    0,  859,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  859,    0,
    0,    0,    0,  819,    0,    0,  819,    0,  860,    0,
    0,    0,    0,    0, 1027,    0,    0, 1028,    0,    0,
  859,  819,  817, 1028,    0,  859,    0,    0, 1028,    0,
    0, 1028,    0,    0,    0,    0,    0,  859,    0,  859,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  860,    0,  817,  860,    0,  817, 1028,    0,    0,
    0,  860,    0,  860,    0, 1028,    0,  860,    0,    0,
  816,  817,  860, 1028,    0,  860,  859,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  818,    0,    0,
  860, 1028,    0, 1028,    0,    0,  859,    0,    0,    0,
  859,    0,    0,    0,  860,    0, 1028,    0, 1028, 1028,
 1028,    0,  860,    0,    0,  860,  860,    0,    0,    0,
    0,    0,    0,  816,    0,  860,  816,    0,  818,    0,
    0,  818,    0,  816,    0,  816,    0,    0,    0,  816,
    0,  860,  842,    0,  816,    0,  818,  816,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1028,    0,  850,
    0,    0,  816,  860,    0,    0,    0,    0,  860,    0,
    0,    0,    0,    0,    0,    0,  816,    0,    0,    0,
  860,    0,  860,    0,  816,    0,    0,  816,  816,    0,
    0,    0,    0,    0,    0,  842,    0,  816,  842,    0,
  850,    0,    0,  850,    0,  842,    0,  842,    0,    0,
    0,  842,    0,  816,  819,    0,  842,    0,  850,  842,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  860,
    0,  812,    0,    0,  842,  816,    0,    0,    0,    0,
  816,    0,    0,    0,    0,    0,    0,    0,  842,  860,
    0,    0,  816,  860,  816,    0,  842,    0,    0,  842,
  842,    0,    0,    0,    0,    0,    0,  819,    0,  842,
  819,    0,  812,    0,  817,  812,    0,  819,    0,  819,
    0,    0,    0,  819,    0,  842,    0,    0,  819,    0,
  812,  819,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  816,    0,  813,    0,    0,  819,  842,    0,    0,
    0,    0,  842,    0,    0,    0,    0,    0,    0,    0,
  819,  816,    0,    0,  842,  816,  842,  817,  819,    0,
  817,  819,  819,    0,    0,    0,    0,  817,    0,  817,
    0,  819,    0,  817,  813,    0,    0,  813,  817,  818,
    0,  817,    0,    0,    0,    0,    0,  819,    0,    0,
    0,    0,  813,  814,    0,    0,  817,    0,    0,    0,
    0,    0,    0,  842,    0,    0,    0,    0,    0,  819,
  817,    0,    0,    0,  819,    0,    0,    0,  817,    0,
    0,  817,  817,  842,    0,    0,  819,  842,  819,    0,
    0,  817,  818,    0,  814,  818,    0,  814,    0,    0,
    0,    0,  818,    0,  818,    0,    0,  817,  818,    0,
    0,  850,  814,  818,    0,    0,  818,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  849,  817,
    0,  818,    0,    0,  817,  819,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  818,  817,    0,  817,    0,
    0,    0,    0,  818,    0,  819,  818,  818,    0,  819,
    0,    0,    0,    0,  850,    0,  818,  850,    0,  849,
    0,    0,  849,    0,  850,    0,  850,    0,    0,    0,
  850,    0,  818,  812,    0,  850,    0,  849,  850,    0,
    0,    0,    0,    0,    0,  817,    0,    0,    0,    0,
  843,    0,    0,  850,  818,    0,    0,    0,    0,  818,
    0,    0,    0,    0,    0,  817,    0,  850,    0,  817,
    0,  818,    0,  818,    0,  850,    0,    0,  850,  850,
    0,    0,    0,    0,    0,    0,  812,    0,  850,  812,
    0,  843,    0,    0,  843,    0,  812,    0,  812,    0,
    0,    0,  812,    0,  850,  813,    0,  812,    0,  843,
  812,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  818,    0,  851,    0,    0,  812,  850,    0,    0,    0,
    0,  850,    0,    0,    0,    0,    0,    0,    0,  812,
  818,    0,    0,  850,  818,  850,    0,  812,    0,    0,
  812,  812,    0,    0,    0,    0,    0,    0,  813,    0,
  812,  813,    0,  851,    0,  814,  851,    0,  813,    0,
  813,    0,    0,    0,  813,    0,  812,    0,    0,  813,
    0,  851,  813,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  850,    0,  852,    0,    0,  813,  812,    0,
    0,    0,    0,  812,    0,    0,    0,    0,    0,    0,
    0,  813,  850,    0,    0,  812,  850,  812,  814,  813,
    0,  814,  813,  813,    0,    0,    0,    0,  814,    0,
  814,    0,  813,    0,  814,  852,    0,    0,  852,  814,
  849,    0,  814,    0,    0,    0,    0,    0,  813,    0,
    0,    0,    0,  852,  840,    0,    0,  814,    0,    0,
    0,    0,    0,    0,  812,    0,    0,    0,    0,    0,
  813,  814,    0,    0,    0,  813,    0,    0,    0,  814,
    0,    0,  814,  814,  812,    0,    0,  813,  812,  813,
    0,    0,  814,  849,    0,  840,  849,    0,  840,    0,
    0,    0,    0,  849,    0,  849,    0,    0,  814,  849,
    0,    0,  843,  840,  849,    0,    0,  849,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  838,
  814,    0,  849,    0,    0,  814,  813,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  849,  814,    0,  814,
    0,    0,    0,    0,  849,    0,  813,  849,  849,    0,
  813,    0,    0,    0,    0,  843,    0,  849,  843,    0,
  838,    0,    0,  838,    0,  843,    0,  843,    0,    0,
    0,  843,    0,  849,  851,    0,  843,    0,  838,  843,
    0,    0,    0,    0,    0,    0,  814,    0,    0,    0,
    0,  844,    0,    0,  843,  849,    0,    0,    0,    0,
  849,    0,    0,    0,    0,    0,  814,    0,  843,    0,
  814,    0,  849,    0,  849,    0,  843,    0,    0,  843,
  843,    0,    0,    0,    0,    0,    0,  851,    0,  843,
  851,    0,  844,    0,    0,  844,    0,  851,    0,  851,
    0,    0,    0,  851,    0,  843,  852,    0,  851,    0,
  844,  851,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  849,    0,  853,    0,    0,  851,  843,    0,    0,
    0,    0,  843,    0,    0,    0,    0,    0,    0,    0,
  851,  849,    0,    0,  843,  849,  843,    0,  851,    0,
    0,  851,  851,    0,    0,    0,    0,    0,    0,  852,
    0,  851,  852,    0,  853,    0,  840,  853,    0,  852,
    0,  852,    0,    0,    0,  852,    0,  851,    0,    0,
  852,    0,  853,  852,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  843,    0,  839,    0,    0,  852,  851,
    0,    0,    0,    0,  851,    0,    0,    0,    0,    0,
    0,    0,  852,  843,    0,    0,  851,  843,  851,  840,
  852,    0,  840,  852,  852,    0,    0,    0,    0,  840,
    0,  840,    0,  852,    0,  840,  839,    0,    0,  839,
  840,  838,    0,  840,    0,    0,    0,    0,    0,  852,
    0,    0,    0,    0,  839,  841,    0,    0,  840,    0,
    0,    0,    0,    0,    0,  851,    0,    0,    0,    0,
    0,  852,  840,    0,    0,    0,  852,    0,    0,    0,
  840,    0,    0,  840,  840,  851,    0,    0,  852,  851,
  852,    0,    0,  840,  838,    0,  841,  838,    0,  841,
    0,    0,    0,    0,  838,    0,  838,    0,    0,  840,
  838,    0,    0,  844,  841,  838,    0,    0,  838,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  845,  840,    0,  838,    0,    0,  840,  852,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  838,  840,    0,
  840,    0,    0,    0,    0,  838,    0,  852,  838,  838,
    0,  852,    0,    0,    0,    0,  844,    0,  838,  844,
    0,  845,    0,    0,  845,    0,  844,    0,  844,    0,
    0,    0,  844,    0,  838,  853,    0,  844,    0,  845,
  844,    0,    0,    0,    0,    0,    0,  840,    0,    0,
    0,    0,  790,    0,    0,  844,  838,    0,    0,    0,
    0,  838,    0,    0,    0,    0,    0,  840,    0,  844,
    0,  840,    0,  838,    0,  838,    0,  844,    0,    0,
  844,  844,    0,    0,    0,    0,    0,    0,  853,    0,
  844,  853,    0,  790,    0,    0,  790,    0,  853,    0,
  853,    0,    0,    0,  853,    0,  844,  839,    0,  853,
    0,  790,  853,    0,    0,  670,    0,    0,    0,    0,
    0,    0,  838,    0,    0,    0,    0,  853,  844,    0,
    0,    0,    0,  844,    0,    0,    0,    0,    0,    0,
    0,  853,  838,    0,    0,  844,  838,  844,    0,  853,
    0,    0,  853,  853,    0,  670,  670,    0,    0,  670,
  839,    0,  853,  839,    0,    0,    0,  841,    0,    0,
  839,    0,  839,    0,  670,    0,  839,    0,  853,    0,
    0,  839,    0,    0,  839,    0,    0,  672,    0,    0,
    0,    0,    0,    0,  844,    0,    0,    0,    0,  839,
  853,    0,    0,    0,  683,  853,    0,    0,    0,    0,
    0,    0,    0,  839,  844,    0,    0,  853,  844,  853,
  841,  839,    0,  841,  839,  839,    0,    0,  672,    0,
  841,  672,  841,    0,  839,    0,  841,    0,    0,    0,
    0,  841,  845,    0,  841,  683,  672,    0,  683,    0,
  839,    0,    0,    0,    0,    0,    0,    0,    0,  841,
    0,    0,    0,  683,    0,    0,  853,    0,    0,    0,
    0,    0,  839,  841,    0,    0,    0,  839,    0,    0,
    0,  841,    0,    0,  841,  841,  853,    0,    0,  839,
  853,  839,    0,    0,  841,  845,    0,    0,  845,    0,
    0,    0,    0,    0,    0,  845,    0,  845,    0,    0,
  841,  845,    0,    0,    0,    0,  845,    0,    0,  845,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  841,    0,  845,    0,    0,  841,  839,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  845,  841,
    0,  841,    0,    0,    0,    0,  845,    0,  839,  845,
  845,    0,  839,    0,    0,    0,    0,  790,    0,  845,
  790,  681,    0,    0,  451,    0,  452,  790,    0,  790,
    0,    0,    0,  790,    0,  845,    0,    0,  790,  454,
    0,  790,    0,    0,  303,    0,    0,    0,  841,    0,
    0,    0,    0,    0,    0,    0,  790,  845,    0,    0,
    0,    0,  845,    0,    0,    0,    0,    0,  841,    0,
  790,    0,  841,    0,  845,    0,  845,    0,  790,    0,
  670,  790,  790,  670,    0,    0,    0,    0,    0,    0,
  670,  790,  670,    0,    0,    0,  670,    0,    0,    0,
    0,  670,    0,    0,  670,    0,    0,  790,    0,    0,
  987,    0,    0,    0,    0,    0,    0,    0,    0,  670,
    0,    0,    0,  845,    0,    0,    0,    0,    0,  790,
    0,    0,    0,  670,  790,    0,    0,    0,    0,    0,
    0,  670,    0,  845,    0,  670,  790,  845,  790,    0,
    0,    0,  672,    0,  670,  672,    0,    0,    0,    0,
    0,    0,  672,    0,  672,    0,    0,    0,  672,  683,
  670,    0,  683,  672,    0,    0,  672,    0,    0,  683,
    0,  683,    0,    0,    0,  683,    0,    0,    0,    0,
  683,  672,    0,  683,    0,  790,    0,  670,    0,    0,
    0,    0,    0,    0,    0,  672,    0,    0,  683,  670,
    0,  670,    0,  672,    0,  790,    0,  672,    0,  790,
    0,    0,  683,    0,    0,    0,  672,    0,    0,    0,
  683,    0,    0,    0,  683,    0,    0,    0,    0,    0,
    0,    0,  672,  683,    0,    0,    0,    0,  413,    0,
    0,    0,    0,    0,    0,  414,    0,    0,  670,  683,
    0,    0,    0,    0,    0,    0,    0,  415,    0,  672,
    0,    0,    0,  416,  417,    0,    0,    0,  670,    0,
    0,  672,  670,  672,  681,    0,  683,  451,    0,  452,
    0,    0,  675,    0,    0,    0,    0,    0,  683,    0,
  683,  418,  454,    0,  419,    0,    0,  303,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  420,
  672,  421,  422,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,    0,  424,    0,  683,    0,    0,
  672,    0,    0,  906,  672,    0,    0,    0,    0,    0,
    0,  986,    0,    0,    0,    0,    0,  683,    0,    0,
    0,  683,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,  677,    0,    0,    0,    0,    0,
  425,    0,    0,    0,    0,    0,  426,    0,    0,    0,
    0,    0,  591,    0,    0,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,  429,  430,  431,  226,  227,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,  113,  114,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  115,  442,    0,  117,    0,    0,  443,  444,
    0,  445,  446,  447,    0,  120,  121,  448,  122,    0,
    0,  123,  124,  125,  126,  127,  907,  908,  909,  128,
  129,    0,    0,    0,    0,  130,  449,  132,    0,  450,
  134,  135,    0,  681,    0,    0,  451,    0,  452,    0,
    0,  413,    0,    0,    0,    0,    0,    0,  414,    0,
  112,  454,    0,    0,    0,    0,  303,    0,    0,    0,
  415,    0,    0,  674,    0,    0,  416,  417,  113,  114,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  115,
  116,    0,  117,    0,    0,  675,    0,    0,    0,  118,
  119,    0,  120,  121,  418,  122,    0,  419,  123,  124,
  125,  126,  127,    0,    0,    0,  128,  129,    0,    0,
  112,    0,  130,  131,  132,    0,  133,  134,  135,    0,
    0,    0,  420,    0,  421,  422,    0,    0,  113,  114,
    0,    0,    0,    0,  423,    0,    0,    0,  424,  115,
  116,    0,  117,    0,    0,    0,  676,    0,    0,  118,
  119,    0,  120,  121,    0,  122,    0,    0,  123,  124,
  125,  126,  127,    0,    0,    0,  128,  129,    0,    0,
    0,    0,  130,  131,  132,    0,  133,  134,  135,    0,
    0,    0,    0,    0,    0,    0,    0,  677,    0,    0,
    0,    0,    0,  425,    0,    0,    0,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,  429,  430,  431,  226,  227,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,  113,  114,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  115,  442,    0,  117,    0,
  413,  443,  444,    0,  445,  446,  447,  414,  120,  121,
  448,  122,    0,    0,  123,  124,  678,  679,  680,  415,
    0,    0,  128,  129,    0,  416,  417,    0,  130,  449,
  132,    0,  450,  134,  135,    0,  681,    0,    0,  451,
    0,  452,    0,    0,  675,    0,    0,    0,    0,    0,
    0,    0,    0,  418,  454,    0,  419,    0,    0,  303,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  421,  422,    0,    0,    0,    0,    0,
    0,    0,    0,  423,    0,    0,  895,  424,    0,  618,
    0,    0,    0,    0,    0,  902,    0,    0,    0,    0,
    0,    0,  896,    0,    0,    0,  619,    0,    0,  620,
    0,    0,    0,  734,  732,    0,  733,    0,  735,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  927,  926,  928,    0,    0,  677,    0,    0,    0,
    0,    0,  425,    0,    0,    0,    0,    0,  426,  621,
    0,    0,    0,    0,    0,  622,  623,  624,  427,  625,
  626,  627,    0,  628,    0,    0,    0,    0,    0,    0,
    0,    0,  428,  429,  430,  431,  226,  227,    0,    0,
    0,  629,    0,  630,    0,    0,  432,    0,    0,    0,
    0,    0,    0,  113,  114,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  115,  442,    0,  117,    0,    0,
  443,  444,  631,  445,  446,  447,    0,  120,  121,  448,
  122,    0,    0,  123,  124,  678,  679,  680,    0,  632,
    0,  128,  129,    0,    0,    0,    0,  130,  449,  132,
    0,  450,  134,  135,    0,  681,    0,    0,  451,    0,
  452,  633,    0,  413,  211,    0,    0,  211,    0,    0,
  414,    0,    0,  454,    0,    0,    0,    0,  303,    0,
  211,    0,  415,    0,  211,    0,    0,  211,  416,  417,
    0,    0,    0,    0,    0,    0,  634,  635,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  675,    0,    0,
    0,    0,    0,  636,    0,    0,  418,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,  211,    0,    0,
    0,    0,    0,  211,  211,  211,    0,  211,  211,  211,
    0,  211,    0,    0,  420,    0,  421,  422,    0,    0,
    0,    0,  914,    0,    0,    0,  423,    0,  730,  211,
  424,  211,    0,  731,    0,    0,  915,    0,  676,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,    0,    0,    0,   80,    0,    0,    0,    0,
    0,    0,  916,    0,    0,    0,    0,  211,    0,  677,
  917,    0,    0,    0,    0,  425,    0,    0,  918,    0,
    0,  426,    0,    0, 1230,    0,    0,    0,    0,  211,
    0,  427,    0,    0,    0,    0,  919,    0,  920,    0,
    0,    0,    0,    0,    0,  428,  429,  430,  431,  226,
  227,  921,    0,  922,  923,  924,    0,    0,    0,  432,
    0,    0,    0,    0,  211,  211,  113,  114,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  115,  442,    0,
  117,  211,  413,  443,  444,    0,  445,  446,  447,  414,
  120,  121,  448,  122, 1231,    0,  123,  124,  678,  679,
  680,  415,  925,    0,  128,  129,    0,  416,  417,    0,
  130,  449,  132,    0,  450,  134,  135,    0,  519,    0,
    0,  451,    0,  452,    0,    0,  675,    0,    0, 1232,
    0,    0,    0,    0, 1233,  418,  454,    0,  419,    0,
 1234,  303,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  112,    0,    0,    0,    0,    0,    0,    0, 1235,
    0,    0,    0,  420,    0,  421,  422,    0,    0,  113,
  114,    0,    0,    0,    0,  423,    0,    0,    0,  424,
  115,  116,    0,  117,    0,    0,    0,  902,    0,    0,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0,    0,    0,  128,  129,    0,
    0,    0,    0,  130,  131,  132,    0,  133,  134,  135,
    0,    0,    0,    0,    0,    0,    0,    0,  677,    0,
    0,    0,    0,    0,  425,    0,    0,    0,    0,    0,
  426,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  427,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  428,  429,  430,  431,  226,  227,
    0,    0,    0,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,    0,    0,  113,  114,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  115,  442,    0,  117,
    0,    0,  443,  444,    0,  445,  446,  447,    0,  120,
  121,  448,  122,    0,    0,  123,  124,  125,  126,  127,
    0,    0,    0,  128,  129,    0,    0,    0,    0,  130,
  449,  132,    0,  450,  134,  135,    0,    0, 1146,    0,
    0, 1147,    0,    0,    0,  413,    0,    0,    0,    0,
    0,    0,  414,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  415,    0,    0,    0,    0,    0,
  416,  417,    0,    0,    0,    0,    0,    0,    0,  519,
    0,    0,  451,    0,  452,    0,    0,  500,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  454,  418,    0,
    0,  419,  303,    0,  112,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  113,  114,    0,    0,  420,    0,  421,  422,
    0,    0,    0,  115,  116,    0,  117,    0,  423,    0,
    0,    0,  424,  118,  119,    0,  120,  121,    0,  122,
    0,    0,  123,  124,  125,  126,  127,    0,    0,    0,
  128,  129,    0,    0,    0,    0,  130,  131,  132,    0,
  133,  134,  135,  105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1148,    0,  425,    0,    0,
    0,    0,    0,  426,    0,    0,    0,    0,    0,    0,
    0,  106,    0,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  107,    0,    0,    0,  428,  429,  430,
  431,  226,  227,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,    0,  108,  113,  114,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  115,
  442,    0,  117,    0,    0,  443,  444,    0,  445,  446,
  447,    0,  120,  121,  448,  122,    0,    0,  123,  124,
  125,  126,  127,    0,    0,    0,  128,  129,    0,    0,
    0,    0,  130,  449,  132,    0,  450,  134,  135, 1157,
    0,    0, 1147,    0,    0,    0,  413,    0,    0,    0,
    0,    0,  109,  414,    0,    0,    0,    0,    0,    0,
    0,    0,  110,    0,    0,  415,    0,    0,    0,    0,
    0,  416,  417,    0,    0,    0,    0,    0,    0,    0,
  519,    0,    0,  451,    0,  452,    0,  111,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  454,  418,
    0,    0,  419,  303,    0,  112,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  113,  114,    0,    0,  420,    0,  421,
  422,    0,    0,    0,  115,  116,    0,  117,    0,  423,
    0,    0,    0,  424,  118,  119,    0,  120,  121,    0,
  122,    0,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,    0,    0,    0,    0,  130,  131,  132,
    0,  133,  134,  135,   48,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1148,    0,  425,    0,
    0,    0,    0,    0,  426,    0,    0,    0,    0,    0,
    0,    0,   48,    0,  427,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   48,    0,    0,    0,  428,  429,
  430,  431,  226,  227,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,    0,    0,    0,    0,   48,  113,
  114,  433,  434,  435,  436,  437,  438,  439,  440,  441,
  115,  442,    0,  117,    0,    0,  443,  444,    0,  445,
  446,  447,    0,  120,  121,  448,  122,    0,    0,  123,
  124,  125,  126,  127,    0,    0,    0,  128,  129,    0,
    0,    0,    0,  130,  449,  132,    0,  450,  134,  135,
 1161,    0,    0, 1147,    0,    0,    0,  413,    0,    0,
    0,    0,    0,   48,  414,    0,    0,    0,    0,    0,
    0,    0,    0,   48,    0,    0,  415,    0,    0,    0,
    0,    0,  416,  417,    0,    0,    0,    0,    0,    0,
    0,  519,    0,    0,  451,    0,  452,    0,   48,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  454,
  418,    0,    0,  419,  303,    0,   50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   50,   50,    0,    0,  420,    0,
  421,  422,    0,    0,    0,   50,   50,    0,   50,    0,
  423,    0,    0,    0,  424,   50,   50,    0,   50,   50,
    0,   50,    0,    0,   50,   50,   50,   50,   50,    0,
    0,    0,   50,   50,    0,    0,    0,    0,   50,   50,
   50,    0,   50,   50,   50,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1148,    0,  425,
    0,    0,    0,    0,    0,  426,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  427,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
  429,  430,  431,  226,  227,    0,    0,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,    0,
  113,  114,  433,  434,  435,  436,  437,  438,  439,  440,
  441,  115,  442,    0,  117,    0,    0,  443,  444,    0,
  445,  446,  447,    0,  120,  121,  448,  122,    0,    0,
  123,  124,  125,  126,  127,    0,    0,    0,  128,  129,
    0,    0,    0,    0,  130,  449,  132,    0,  450,  134,
  135, 1169,    0,    0, 1147,    0,    0,    0,  413,  211,
    0,    0,  211,    0,    0,  414,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  211,    0,  415,    0,  211,
    0,    0,  211,  416,  417,    0,    0,    0,    0,    0,
    0,    0,  519,    0,    0,  451,    0,  452,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  454,  418,    0,    0,  419,  303,    0,    0,    0,    0,
    0,    0,  211,    0,    0,    0,    0,    0,  211,  211,
  211,  112,  211,  211,  211,    0,  211,    0,    0,  420,
    0,  421,  422,    0,    0,    0,    0,    0,    0,  113,
  114,  423,    0,    0,  211,  424,  211,    0,    0,    0,
  115,  116,    0,  117,    0,    0,    0,    0,    0,    0,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0,  211,    0,  128,  129,    0,
    0,    0,    0,  130,  131,  132,    0,  133,  134,  135,
    0,    0,  211,    0,    0,    0,    0,    0, 1148,    0,
  425,    0,    0,    0,    0,    0,  426,    0,    0,    0,
    0,    0,    0,    0,  211,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,  429,  430,  431,  226,  227,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,  211,
  211,  113,  114,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  115,  442,    0,  117,  211,    0,  443,  444,
    0,  445,  446,  447,    0,  120,  121,  448,  122,    0,
    0,  123,  124,  125,  126,  127,    0,    0,    0,  128,
  129,    0,    0,    0,    0,  130,  449,  132,    0,  450,
  134,  135, 1172,    0,    0, 1147,    0,    0,    0,  413,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  415,    0,
    0,    0,    0,    0,  416,  417,    0,    0,    0,    0,
    0,    0,    0,  519,    0,    0,  451,    0,  452,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  454,  418,    0,    0,  419,  303,    0,   27,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   27,   27,    0,    0,
  420,    0,  421,  422,    0,    0,    0,   27,   27,    0,
   27,    0,  423,    0,    0,    0,  424,   27,   27,    0,
   27,   27,    0,   27,    0,    0,   27,   27,   27,   27,
   27,    0,    0,    0,   27,   27,    0,    0,    0,    0,
   27,   27,   27,    0,   27,   27,   27,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1148,
    0,  425,    0,    0,    0,    0,    0,  426,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  427,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  428,  429,  430,  431,  226,  227,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
    0,    0,  113,  114,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  115,  442,    0,  117,    0,    0,  443,
  444,    0,  445,  446,  447,    0,  120,  121,  448,  122,
    0,    0,  123,  124,  125,  126,  127,    0,    0,    0,
  128,  129,    0,    0,    0,    0,  130,  449,  132,    0,
  450,  134,  135, 1177,    0,    0, 1147,    0,    0,    0,
  413,    0,    0,    0,    0,    0,    0,  414,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,    0,    0,  416,  417,    0,    0,    0,
    0,    0,    0,    0,  519,    0,    0,  451,    0,  452,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  454,  418,    0,    0,  419,  303,    0,  574,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  574,  574,    0,
    0,  420,    0,  421,  422,    0,    0,    0,  574,  574,
    0,  574,    0,  423,    0,    0,    0,  424,  574,  574,
    0,  574,  574,    0,  574,    0,    0,  574,  574,  574,
  574,  574,    0,    0,    0,  574,  574,    0,    0,    0,
    0,  574,  574,  574,    0,  574,  574,  574,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1148,    0,  425,    0,    0,    0,    0,    0,  426,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  427,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,  429,  430,  431,  226,  227,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,    0,  113,  114,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  115,  442,    0,  117,    0,    0,
  443,  444,    0,  445,  446,  447,    0,  120,  121,  448,
  122,    0,    0,  123,  124,  125,  126,  127,    0,    0,
    0,  128,  129,    0,    0,    0,    0,  130,  449,  132,
    0,  450,  134,  135, 1180,    0,    0, 1147,    0,    0,
    0,  413,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,  542,    0,    0,    0,  416,  417,    0,    0,
    0,    0,    0,    0,    0,  519,    0,    0,  451,    0,
  452,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  454,  418,    0,    0,  419,  303,    0,
  280,    0,  542,  542,    0,    0,  542,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  280,  280,
    0,  542,  420,  542,  421,  422,    0,    0,    0,  280,
  280,    0,  280,    0,  423,    0,    0,    0,  424,  280,
  280,    0,  280,  280,    0,  280,    0,    0,  280,  280,
  280,  280,  280,  542,    0,    0,  280,  280,    0,    0,
    0,    0,  280,  280,  280,    0,  280,  280,  280,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1148,    0,  425,    0,    0,    0,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,  429,  430,  431,  226,  227,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,  113,  114,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  115,  442,    0,  117,    0,
    0,  443,  444,    0,  445,  446,  447,    0,  120,  121,
  448,  122,    0,    0,  123,  124,  125,  126,  127,    0,
    0,    0,  128,  129,    0,    0,    0,    0,  130,  449,
  132,    0,  450,  134,  135, 1183,    0,    0, 1147,    0,
    0,    0,  413,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  643,    0,  643,  643,    0,  643,    0,
  542,  415,    0,    0,    0,    0,    0,  416,  417,    0,
    0,  643,    0,    0,    0,  542,  643,  542,  542,  542,
    0,    0,    0,  542,    0,    0,  542,    0,    0,    0,
    0,    0,    0,    0,    0,  418,    0,    0,  419,  542,
    0,    0,    0,    0,    0,  542,    0,    0,  542,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,  421,  422,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,  424,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  542,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  542,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1148,    0,  425,  542,    0,    0,    0,    0,
  426,  542,    0,    0,    0,  542,    0,    0,    0,    0,
  427,    0,  542,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  428,  429,  430,  431,  226,  227,
  542,    0,    0,    0,    0,  542,    0,    0,  432,    0,
    0,    0,    0,    0,    0,  113,  114,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  115,  442,    0,  117,
  542,    0,  443,  444,  542,  445,  446,  447,    0,  120,
  121,  448,  122,  643,    0,  123,  124,  125,  126,  127,
  643,    0,    0,  128,  129,    0,    0,  643,    0,  130,
  449,  132,    0,  450,  134,  135,    0,    0,    0,  643,
    0,    0,    0,    0,    0,  643,  643,    0,    0,    0,
    0,    0,    0,  643,  637,    0,  637,  637,    0,  637,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  637,  643,    0,    0,  643,  637,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  643,    0,  643,  643,    0,    0,    0,    0,    0,
    0,    0,    0,  643,    0,    0,    0,  643,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  643,    0,    0,    0,    0,    0,  643,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  643,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  643,  643,  643,  643,  643,  643,    0,    0,
    0,    0,    0,    0,    0,    0,  643,    0,    0,    0,
    0,    0,    0,  643,  643,  643,  643,  643,  643,  643,
  643,  643,  643,  643,  643,  643,    0,  643,    0,    0,
  643,  643,    0,  643,  643,  643,    0,  643,  643,  643,
  643,    0,    0,  643,  643,  643,  643,  643,    0,    0,
    0,  643,  643,    0,    0,    0,    0,  643,  643,  643,
    0,  643,  643,  643,  637,    0,    0,    0,    0,  536,
    0,  637,    0,    0,    0,    0,    0,    0,  637,    0,
    0,    0,  519,    0,  524,  451,    0,  452,    0,    0,
  637,    0,    0,    0,    0,    0,  637,  637,    0,    0,
  454,    0,    0,    0,  637,  303,    0,    0,    0,    0,
  536,    0,    0,  536,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  637,    0,    0,  637,  536,    0,
  536,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  637,    0,  637,  637,    0,    0,    0,    0,
  536,    0,    0,    0,  637,    0,    0,    0,  637,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1227,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  637,    0,    0,    0,    0,    0,  637,
    0,    0,    6,    0,    0,    0,    0,    0,    0,  637,
    0,    0,    0, 1431, 1432,    0, 1433,    0,    0,    0,
    0,    0,    0,  637,  637,  637,  637,  637,  637,    0,
    0,    0, 1434,    0,    0,    0,    0,  637, 1435,    0,
    0,    0,    0,    0,  637,  637,  637,  637,  637,  637,
  637,  637,  637,  637,  637,  637,  637,    0,  637,    0,
    0,  637,  637,    0,  637,  637,  637,    0,  637,  637,
  637,  637,    0,    0,  637,  637,  637,  637,  637,  413,
    0,    0,  637,  637,    0,    0,  414,    0,  637,  637,
  637,    0,  637,  637,  637, 1436,    0,  536,  415,    0,
    0,    0,    0,    0,  416,  417,  519,    0,    0,  451,
    0,  452,  536,   14,  536,  536,  536,    0,    0,    0,
  536,    0,    0,  536,  454,    0,    0,    0, 1437,  303,
    0,    0,  418,    0,    0,  419,  536,    0,   16,    0,
    0,    0,  536,    0,    0,  536,    0, 1642,    0,    0,
    0,    0,    0,    0,    0,    0,  431,    0,    0,    0,
  420,    0,  421,  422,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,  113,  114,  424,    0,    0,    0,
    0,    0,    0,    0,    0, 1438,  116,  536,  117,    0,
    0,    0,    0,    0,    0,  118,  119,    0,  120,  121,
    0,  122,    0,    0,  123, 1439,  125,  126,  127,    0,
    0,  536,  128,  129, 1440, 1441, 1442,    0,  130,  131,
  132,    0,  133,  134,  135,    0,    0,    0,    0,    0,
    0,  425,  536,    0,    0,    0,    0,  426,  536,    0,
    0,    0,  536,    0,    0,    0,    0,  427,    0,  536,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  428,  429,  430,  431,  226,  227,  536,    0,    0,
    0,    0,  536,    0,    0,  432,    0,    0,    0,    0,
    0,    0,  113,  114,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  115,  442,    0,  117,  536,    0,  443,
  444,  536,  445,  446,  447,    0,  120,  121,  448,  122,
    0,    0,  123,  124,  125,  126,  127,    0,    0,    0,
  128,  129,    0,    0,    0,    0,  130,  449,  132,    0,
  450,  134,  135,  413,    0,    0,    0,    0,    0,    0,
  414,    0,    0,    0,  519,    0,    0,  451,    0,  452,
    0,    0,  415,    0,    0,    0,    0,    0,  416,  417,
    0,    0,  454,    0,    0,    0,    0,  303,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,  994,    0,  419,
  994,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  996,    0,  994,    0,    0,
  994,    0,    0,    0,  420,    0,  421,  422,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
  424,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  994,    0,    0,    0,    0,    0,  994,  994,  994,    0,
  994,  994,  994,    0,  994,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  994,    0,  994,  425,    0,    0,    0,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  427,    0,    0,    0,    0,    0,    0,    0,  713,
    0,    0,    0,  994,    0,  428,  429,  430,  431,  226,
  227,    0,    0,    0,    0,    0,    0,    0,    0,  432,
  994,    0,    0,    0,    0,    0,  113,  114,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  115,  442,    0,
  117,    0,  994,  443,  444,    0,  445,  446,  447,    0,
  120,  121,  448,  122,    0,    0,  123,  124,  125,  126,
  127,  413,    0,    0,  128,  129,    0,    0,  414,    0,
  130,  449,  132,    0,  450,  134,  135,  994,  994,  558,
  415,    0,    0,    0,    0,    0,  416,  417,  519,  975,
    0,  451,    0,  452,  994,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  454,    0,    0,    0,
    0,  303,    0,    0,  418,    0,    0,  419,    0,    0,
  558,    0,    0,  558,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  558,    0,
  558,    0,  420,    0,  421,  422,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,  424,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  558,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   14,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  425,    0,    0,    0,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,  429,  430,  431,  226,  227,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,  113,  114,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  115,  442,    0,  117,    0,
    0,  443,  444,    0,  445,  446,  447,    0,  120,  121,
  448,  122,    0,    0,  123,  124,  125,  126,  127,    0,
  931,    0,  128,  129,    0,    0,    0,    0,  130,  449,
  132,    0,  450,  134,  135,  413,    0,    0,    0,    0,
    0,    0,  414,  932,    0,    0,  519,    0,    0,  451,
    0,  452,    0,    0,  415,    0,    0,  558,    0,    0,
  416,  417,    0,    0,  454,    0,    0,    0,    0,  303,
    0,    0,  558,    0,    0,  558,  558,    0,    0,    0,
  558,    0,    0,  558,    0,    0,    0,    0,  418,    0,
    0,  419,    0,    0,    0,    0,  558,    0,    0,  933,
    0,    0,  558,    0,    0,  558,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,  421,  422,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,  424,  112,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  558,    0,    0,
    0,  113,  114,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,  116,    0,  117,    0,    0,    0,    0,
    0,  558,  118,  119,    0,  120,  121,    0,  122,    0,
    0,  123,  124,  125,  126,  127,    0,  425,    0,  128,
  129,    0,  558,  426,    0,  130,  131,  132,  558,  133,
  134,  135,  558,  427,    0,    0,    0,    0,    0,  558,
    0,    0,    0,    0,    0,    0,    0,  428,  429,  430,
  431,  226,  227,    0,    0,    0,    0,  558,    0,    0,
    0,  432,  558,    0,    0,    0,    0,    0,  113,  114,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  115,
  442,    0,  117,    0,    0,  443,  444,  558,  445,  446,
  447,  558,  120,  121,  448,  122,    0,    0,  123,  124,
  125,  126,  127,  413,    0,    0,  128,  129,    0,    0,
  414,    0,  130,  449,  132,    0,  450,  134,  135,    0,
    0,  548,  415,    0,    0,    0,    0,    0,  416,  417,
  519,    0,    0,  451,    0,  452,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  454,    0,
    0,    0,    0,  303,    0,    0,  418,    0,    0,  419,
    0,    0,  548,    0,    0,  548,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  548,    0,  548,    0,  420,    0,  421,  422,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
  424,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  548,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  425,    0,    0,    0,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  427,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1175,    0,    0,  428,  429,  430,  431,  226,
  227,    0,    0,    0,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,    0,    0,  113,  114,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  115,  442,    0,
  117,    0,    0,  443,  444,    0,  445,  446,  447,    0,
  120,  121,  448,  122,    0,    0,  123,  124,  125,  126,
  127,    0,    0,    0,  128,  129,    0,    0,    0,    0,
  130,  449,  132,    0,  450,  134,  135,  413,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  519,    0,
    0,  451,    0,  452,    0,    0,  415,    0,    0,  548,
    0,    0,  416,  417,    0,    0,  454, 1744,    0,    0,
    0,  303,    0,    0,  548,    0,    0,  548,  548,    0,
    0,    0,  548,    0,    0,  548,    0,    0,    0,    0,
  418,    0,    0,  419,    0,    0,    0,    0,  548,    0,
    0,    0,    0,    0,  548,    0,    0,  548,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  420,    0,
  421,  422,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,  424,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  548,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  548,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  425,
    0,    0,    0,    0,  548,  426,    0,    0,    0,    0,
  548,    0,    0,    0,  548,  427,    0,    0,    0,    0,
    0,  548,    0,    0,    0,    0, 1382,    0,    0,  428,
  429,  430,  431,  226,  227,    0,    0,    0,    0,  548,
    0,    0,    0,  432,  548,    0,    0,    0,    0,    0,
  113,  114,  433,  434,  435,  436,  437,  438,  439,  440,
  441,  115,  442,    0,  117,    0,    0,  443,  444,  548,
  445,  446,  447,  548,  120,  121,  448,  122,    0,    0,
  123,  124,  125,  126,  127,  413,    0,    0,  128,  129,
    0,    0,  414,    0,  130,  449,  132,    0,  450,  134,
  135,    0,    0,  533,  415,    0,    0,    0,    0,    0,
  416,  417,  986,    0,  986,  986,    0,  986,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,    0,    0,    0,    0,  986,    0,    0,  418,    0,
    0,  419,    0,    0,  533,    0,    0,  533,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  533,    0,  533,    0,  420,    0,  421,  422,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,  424,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  533,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  425,    0,    0,
    0,    0,    0,  426,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,  429,  430,
  431,  226,  227,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,    0,    0,  113,  114,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  115,
  442,    0,  117,    0,    0,  443,  444,    0,  445,  446,
  447,    0,  120,  121,  448,  122,    0,    0,  123,  124,
  125,  126,  127,    0,    0,    0,  128,  129,    0,    0,
    0,    0,  130,  449,  132,    0,  450,  134,  135,  986,
    0,    0,    0,    0,    0,    0,  986,    0,    0,    0,
  453,    0,    0,  451,    0,  452,    0,    0,  986,    0,
    0,  533,    0,    0,  986,  986,    0,    0,  454,    0,
    0,    0,    0,  303,    0,    0,  533,    0,    0,  533,
  533,    0,    0,    0,  533,    0,    0,  533,    0,    0,
    0,    0,  986,    0,    0,  986,    0,    0,    0,    0,
  533,    0,    0,    0,    0,    0,  533,    0,    0,  533,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,    0,  986,  986,    0,    0,    0,    0,    0,    0,
    0,    0,  986,    0,    0,    0,  986,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  533,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  533,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  986,    0,    0,    0,    0,  533,  986,    0,    0,
    0,    0,  533,    0,    0,    0,  533,  986,    0,    0,
    0,    0,    0,  533,    0,    0,    0,    0,    0,    0,
    0,  986,  986,  986,  986,  986,  986,    0,    0,    0,
    0,  533,    0,    0,    0,  986,  533,    0,    0,    0,
    0,    0,  986,  986,  986,  986,  986,  986,  986,  986,
  986,  986,  986,  986,  986,    0,  986,    0,    0,  986,
  986,  533,  986,  986,  986,  533,  986,  986,  986,  986,
    0,    0,  986,  986,  986,  986,  986,  413,    0,    0,
  986,  986,    0,    0,  414,    0,  986,  986,  986,    0,
  986,  986,  986,    0,    0,    0,  415,    0,    0,    0,
    0,    0,  416,  417,  519,    0,    0,  451,    0,  452,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  454,    0,    0,    0,    0,  303,    0,    0,
  418,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  420,    0,
  421,  422,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,  424,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  425,
    0,    0,    0,    0,    0,  426,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  427,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
  429,  430,  431,  226,  227,    0,    0,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,    0,
  113,  114,  433,  434,  435,  436,  437,  438,  439,  440,
  441,  115,  442,    0,  117,    0,    0,  443,  444,    0,
  445,  446,  447,    0,  120,  121,  448,  122,    0,    0,
  123,  124,  125,  126,  127,    0,    0,    0,  128,  129,
    0,    0,    0,    0,  130,  449,  132,    0,  450,  134,
  135,  413,    0,    0,    0,    0,    0,    0,  414,    0,
    0,    0,  986,    0,    0,  986,    0,  986,    0,    0,
  415,    0,    0,    0,    0,    0,  416,  417,    0,    0,
  986,    0,    0,    0,    0,  986,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  418,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,  421,  422,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,  424,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  425,    0,    0,    0,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,  429,  430,  431,  226,  227,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,  113,  114,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  115,  442,    0,  117,    0,
    0,  443,  444,    0,  445,  446,  447,    0,  120,  121,
  448,  122,    0,    0,  123,  124,  125,  126,  127,  986,
    0,    0,  128,  129,    0,    0,  986,    0,  130,  449,
  132,    0,  450,  134,  135,    0,    0,    0,  986,    0,
    0,    0,    0,    0,  986,  986,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  986,    0,    0,  986,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,    0,  986,  986,    0,    0,    0,    0,    0,    0,
    0,    0,  986,    0,    0,    0,  986,    0,    0,    0,
  321,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  321,    0,
    0,    0,    0,    0,    0,  321,    0,    0,    0,  321,
  321,    0,  321,    0,    0,  321,    0,    0,    0,    0,
    0,  986,    0,    0,    0,    0,    0,  986,  321,    0,
    0,    0,    0,    0,  321,    0,    0,  986,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  986,  986,  986,  986,  986,  986,    0,    0,    0,
    0,    0,    0,    0,    0,  986,    0,    0,    0,    0,
    0,    0,  986,  986,  986,  986,  986,  986,  986,  986,
  986,  986,  986,  986,  986,    0,  986,    0,    0,  986,
  986,  321,  986,  986,  986,    0,  986,  986,  986,  986,
 1227,    0,  986,  986,  986,  986,  986,    0,    0,  321,
  986,  986,    0,    0,    0,    0,  986,  986,  986,    0,
  986,  986,  986,    0,  321,    0,    0,    0,    6,    0,
    0,    0,    0,    0,  321,    0,    0,    0,    0, 1431,
 1432,    0, 1433,  321,    0,  321,    0,    0,    0,    0,
    0,    0,  321,    0,    0,    0,    0,    0, 1434,    0,
    0,    0,    0,  617, 1435,    0,  618,    0,    0,    0,
  321,  321,  817,    0,    0,    0,    0,    0,    0,    0,
    0,  321,  321,  619,  321,    0,  620,    0,    0,    0,
    0,  321,  321,    0,  321,  321,    0,  321,    0,    0,
  321,  321,  321,  321,  321,    0,    0,    0,  321,  321,
  321,  321,  321,    0,  321,  321,  321,    0,  321,  321,
  321, 1436,    0,    0,    0,    0,  621,    0,    0,    0,
    0,    0,  622,  623,  624,    0,  625,  626,  627,   14,
  628,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1437,    0,    0,    0,  629,    0,
  630,    0,    0,    0,   16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  431,    0,    0,    0,    0,    0,    0,  631,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  113,  114,    0,    0,    0,    0,  632,    0,    0,    0,
    0, 1438,  116,    0,  117,    0,    0,    0,    0,    0,
    0,  118,  119,    0,  120,  121,    0,  122,  633,    0,
  123, 1439,  125,  126,  127,    0,    0,    0,  128,  129,
 1440, 1441, 1442,  112,  130,  131,  132,    0,  133,  134,
  135,  617,    0,    0,  618,    0,    0,    0,    0,    0,
    0,  113,  114,  634,  635,    0,    0,    0,    0,    0,
    0,  619,  115,  116,  620,  117,    0,    0,    0,    0,
  636,    0,  118,  119,    0,  120,  121,    0,  122,    0,
    0,  123,  124,  125,  126,  127,    0,    0,    0,  128,
  129,    0,    0,    0,    0,  130,  131,  132,    0,  133,
  134,  135,    0,    0,  621,    0,    0,    0,    0,    0,
  622,  623,  624,    0,  625,  626,  627,    0,  628,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  112,    0,    0,    0,    0,    0,  629,    0,  630,  179,
  180,  181,  182,  183,  184,  185,  186,  187,  113,  114,
  188,  189,  190,  191,  192,    0,    0,  193,  194,  115,
  116,    0,  117,    0,    0,  195,  196,  631,    0,  118,
  119,  197,  120,  121,  198,  122,  199,    0,  123,  124,
  125,  126,  127,    0,  632,    0,  128,  129,  200,  201,
  202,  203,  130,  131,  132,  204,  133,  134,  135,    0,
    0,    0,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  112,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  113,
  114,  634,  635,    0,    0,    0,    0,    0,    0,    0,
  115,  116,    0,  117,    0,    0,    0,    0,  636,    0,
  118,  119,    0,  120,  121,    0,  122,    0,    0,  123,
  124,  125,  126,  127,    0,    0,    0,  128,  129,    0,
  458,    0,    0,  130,  131,  132,    0,  133,  134,  135,
  458,  458,  458,  458,  458,  458,  458,  458,  458,  458,
  458,  458,  458,  458,  458,    0,    0,  458,  458,  458,
  458,    0,  458,    0,    0,  458,  458,    0,    0,  458,
  458,  458,  458,  458,  458,  458,  458,    0,  458,  458,
  458,  458,  458,    0,    0,    0,  458,  458,  458,  458,
  458,  458,  458,  458,  458,  458,  458,  458,  458,
};
static short yycheck[] = {                                       9,
    4,    0,   68,   58,  111,  554,   16,  163,   18,  153,
  154,   67,   66,  359,   74,  153,  154,  391,   77,  738,
   16,  391,   67,  387,   90,  713,   99,   96,  289,  489,
   72,  719,  383,  586,  530,  361,  791,  557,   98,   94,
 1066,  100,  994, 1227,  936,  305,   56,   57,   58,   59,
   60,   61,   62, 1299,   96,    0,  508,   67,   68,   69,
  453,   71,   59,   73,   74, 1062,   76,   77,   78,  509,
  789,  334, 1226,   60,   71,  368,   86,   62,   88,   89,
   90,  973,   92,   93,   94,   95,   73,   97,   98,   99,
  100,  101,   88,  103,  167,  745,  699,  889,   95,    0,
  145,   97,  147,  396, 1055,    0,  103,  603,   93, 1014,
 1096,  662,    0,  367,   59, 1066,  101, 1114,  240,  579,
 1119,  581,  725,  669,   41,    0,  586,   45, 1245,   40,
    0,   44, 1055,    0,   63,  145,  146,  147,   44,    0,
 1055,  151,  249, 1066,  171,    0,   41,    0,  158,   44,
    0, 1066, 1055,  163,  208,  165,  166,  167,   59,  146,
    0,  171,   44, 1066,   59,    0,   45,   44,  165, 1055,
    0,   59,  796,  158,   41, 1055,   44,   44,  334,  166,
 1066,    0,  236,   44,   59,  453, 1066,  811,   41,   59,
  814,   44,   59,   44,   41,    0, 1562,   44,   59,  823,
 1257,  565,  344,    0,   59,    0,   59,   40,   44,   59,
    0,   41,  586,  335,   44,    0,  586, 1297,    0,   59,
   59,   45,   41,   40,   59,   45,   40,  298,  315,   59,
   63, 1451,  339,  273,    0,  280,   41,   91,  283,  500,
   59,  929,  930,  589,   41,   61,   44, 1505,    0,   63,
  614,  519,    0,  676,   59,  516,  549,   41,  882,    0,
  283,  316,   59,   44,   59, 1291, 1292, 1451,  276,   59,
  280,   40,    0,  283,   59, 1272,   45,   59,  966,  289,
  290,  298,   41,  262,   45,  395, 1440, 1441, 1822,  334,
  264,  280,  638,   59,  447,  484,  317,  334, 1246, 1325,
   44,    0,   63,  380,  395,   41,  316,   59,  316,  275,
  320,   59,    0,   41,  339,  360,   44,  547,   59,    0,
 1207,  264,  367, 1210,  369,  317,  371,  516,    0,  374,
  384,   59,  386,  320,  394,  351,  729,  362,  324,   41,
 1291, 1292, 1294, 1877,  737, 1331,  400,  310,   41,  404,
  360,  260,  508, 1258,  262,  432,  272,  367,  445,  369,
   59,  371,  315,  434,  374,   93, 1054,  509, 1291, 1292,
  866,   59, 1320,    0, 1325,  341, 1291, 1292,   59, 1271,
  840,  498,   44,  393,  394,  393, 1338,   59, 1291, 1292,
  500,  300,  402,  418,  404,   61,  393,  434,  408,    0,
  379, 1060, 1325, 1661,  408, 1291, 1292,  428, 1067,  297,
 1325, 1291, 1292,  681,   41,  402, 1040,  434,  458, 1078,
  315,  275, 1325,  514, 1790,  433,    0,  297,  264,  403,
  453,  454,   59, 1057, 1058, 1038,    0, 1683, 1495, 1325,
  317,  336,  431, 1102,  454, 1325,  409, 1527,  315,   41,
  890, 1139, 1140, 1077, 1142, 1143,  344,  297,   59,  805,
  403,  901,  808,  508,  315,  905,   41,  605,  484,  336,
  456,  379,   41,  819,  344,   59,  387,   41,  315,  380,
   41,  297, 1636,  906,   41,   59,  276,  277,  498,  499,
  500,  518,  445,  754,  376,   59,  315,  507,  264,  401,
  846,  511,  499,  301,  344,  380,  516,  767,  518,  554,
  315,  498,  352,  567,  389,  428,  264,  336,  315,  380,
  507,  450,  306,  376,  511,  380,  262,  790,  389, 1416,
  652,  336,  450, 1420,  389,    0, 1782,  606,  444,  336,
 1657,  586,  772, 1540,  554, 1337,  386,  306,  388,    0,
   41,  446, 1004,   44,  608,  410,  403,  301,  812, 1511,
 1559, 1558, 1113,  402,  606,  264,    0,  893,  513,    0,
  410,  450, 1121, 1223, 1014,  585,  586,  261,  428,  446,
 1126,  591,  450,  444, 1830, 1571,  338,  880,    0,  444,
   41,    0, 1280,   44,   59,  435,  513, 1817,  742,  494,
  262, 1506, 1042,  344,  658,  659,  401,   41,   59, 1855,
   41,  446,  513, 1006, 1302,  268,  661,  450,  513,    0,
  402,  509,  439, 1843,  390,   59,  450,  446,   59,   41,
  450,  297,   41, 1817,  790,   44,  450,  775,  513,  509,
  493,  446,  390,  379,  450,  397,  513,   59,  400,  446,
   59,  661,  513,  477,  423, 1314, 1876,  477,  513, 1843,
   41,  446,  423,  493,  416,  446,  297,    0, 1888,  509,
  262, 1891,  344,  361,  402,  276,  277,  379,   59, 1899,
  361,  450,   42,    0,  258, 1577,  379,  262,  315,  450,
  442,  390, 1876,  262,  704,  272,  270, 1048,  743,  999,
  745,  262,  747,  826, 1888,  262,  359, 1891,   41,  336,
  848,  397, 1336,  344, 1740, 1899,  513,  379, 1064,  704,
  730,  371,    0,  297,   41,  450,   59, 1614,  738,  303,
 1076,  305,  298,  743,  319,  745, 1890,  747,  262,   44,
  396,  258,   59, 1695,  754,  790,  409,  801,  319, 1701,
  795,  315,  762,  763, 1509, 1101,  412,    0, 1151,  336,
  770,  771,  448,   41, 1115, 1368,  763,  812,  334,  838,
  344,  372,  336,  436,  784,  762,  786,   40,  352,  789,
 1300,   59,  273,    0,    0,  795,  303,  379,   93, 1740,
  440,  305,    0,  258,  839,  840,  838,  784, 1124,  786,
  854,  315,  812,  845,  379,  270,  371,  876,  877,  878,
  379,  304,  305,  305,    0,  306,   59, 1740,  379,  446,
 1508,  863,  379,  315, 1217, 1740,  322, 1323,    0,  839,
  840, 1327,  297,   40,  876,  877,  878, 1740,  303,    0,
  305,  257,   59,   59,  273,  410,  856,  421, 1004, 1402,
    0,   59,  856,  352, 1740,  379,    0,  348,    0,  262,
 1740, 1520,  284, 1256,  315, 1524, 1525, 1193,  434,    0,
  259,  881,  450,   59,  325,  297,  327,    0,  509,  344,
  269,  891,  446, 1151,  315,  336,  513,   59,  339,  899,
  900,  257,  457,  410,  881,  899,  327,    0,   59,  477,
  260, 1560,  336,  427,  891,  336,  315,  344,   42,   59,
   41,  428,  403,   47,  330,   59,  325,   59,  327,  348,
   42,   43,  421,   45,  336,   47,  422,  336,   59,  380,
  339,  445,    0,  429,  315,  509,   59,    0,  389, 1627,
  300,  265,  300,    0,  325,  462,  327, 1070, 1071,  380,
  443, 1251, 1252,  445,  263,  336,   59,  376,  389, 1004,
  970, 1099,    0,    0,  330, 1425, 1312,  458,  457, 1230,
  459,  380, 1233, 1516,    0,  299,  379, 1341,   41,    0,
  389,  432,  315,    0,   41,  970,    0,   44,  998,  999,
  348,   59,  325,  444,  327,  446,   59, 1343, 1121,  380,
  309,  432,   59,  336, 1014,  304,  305,  318,  389,  333,
 1669,  320,  446,  300, 1673,  446,  453,  454, 1561,  336,
  331,   59,   59,  432,  427,    0,   44,   41, 1402, 1688,
   44,  298, 1402,   59,  446,  444,  397,  446,   59, 1108,
  297,  258,   59,  354,  509,   59, 1705,  380,  264,  304,
  305,  432,  394,  270, 1123,  416,  389,  376,  336,   44,
 1129,  348,  513,  444,   44,  446, 1108,  334,  276,  277,
  316, 1227, 1117,  428,   59,   93, 1121, 1765, 1088,   93,
  297, 1123,  513, 1771,   59,  372,  303, 1129,  305, 1093,
    0,  348,  264, 1812,  405,  338, 1106, 1365, 1366,  432,
 1244, 1245, 1246,  280,  513, 1373, 1374, 1117, 1376, 1377,
  419, 1121,  378,  446,  264,    0, 1384, 1385, 1386, 1387,
  264, 1389, 1390, 1391, 1392, 1393, 1394,  344,  371,  446,
 1789,   41,  513, 1792,   44,  450,  259,  277, 1276,  397,
 1278,  264,  282,  277,  443,    0,  269,  393,  282,   59,
    0,  378,  338,    0,  397,  277,   41,  400,  486,   44,
  282, 1234,  477,   42,   43,  390,   45,    0,   47,    0,
 1226,  372, 1232,  416,   59, 1834, 1235,  338, 1223, 1189,
  513, 1226, 1227, 1529,  315,  371,   41,  433,  443,   44,
  448,   41,  393, 1189,   41,   44,  264,   44,  399,  442,
  377, 1339,   44,  428,   59,  336,    0,  264,  333,   59,
  371,  397,   59, 1223,  400,  410, 1226, 1227,  390, 1565,
 1230, 1231, 1232, 1233, 1234, 1235,   59,  264,   59,  434,
  416,    0,  433,  346, 1231,  348,  397,  462,  264,  400,
  390, 1251, 1252,    0,   93,    0,  390,  264, 1258,  380,
    0,   93,  315,    0,  431,  416,  442,    0,  315,  446,
  258,  318,  325,  261,  327,   59,  273,  390,  325,    0,
  327,  448,   41,  336,  331,   44, 1825,    0,  513,  336,
    0,  442,  339,   42,   43,   44,   45,   44,   47,  260,
   59,   41,  509,    0,   41, 1451,    0,  354, 1352,    0,
  338,  432,   59, 1357,   59,  303,  428,  260,   40,   59,
   41,  368,   59,   44, 1318,  446,   59,  380,  338,  376,
  520,   41,  390,  380,   44,  676,  389,  265,   59,  300,
  681,  279,  389,  371,   41,    0,   59,   44,  298,   59,
  416,    0, 1498,    0,  260,  453,  333,  300,  405,    0,
    0,  371,   59,  390,  302,   59, 1702,    0,   59,  397,
    0,  299,  400,  311,  390, 1431,   41,   42,   43,  432,
   45,  260,   47,  390,  334,  432,   41,  397,  416,   44,
  400,  368,  513,  446,  300, 1395,  265,  444,  402,  446,
  264,  276,  277,  380,   59,  452,  416,  345,  277, 1395,
   59,  349,   59,  282,  442,  315, 1451,    0,   59,   59,
    0,  300,  313,  470,  471,  262,   59,  327,   44,   59,
  299, 1431,  442,    0,  481,  482,  336,  484,  315, 1439,
 1440, 1441,    0,   59,  491,  492,  493,  494,  495,  399,
  497, 1451,  333,  500,  501,  502,  503,  504,  452,  409,
  513,  508,  509,    0,  395,  395,  513,  514,  515,  516,
  517,  518,  519,  520,  258,  315,   59,    0,  315,   59,
  380,  318, 1482, 1483,  434,    0,  270,  327,  325,  389,
  327,  444,   59, 1639,  331,  426,  336,  372, 1498,  336,
  430,   59,  339,  333,  332, 1505, 1506,  445,  260,  403,
  441,  352, 1658,  451,  452,  453,  426,  354,  393,  303,
  336,  305,   59, 1657,  399,  485,   41,  395,  277,   44,
 1530,  368,  432,  282,   40,  495,   59,  409,  368,  376,
  380,  318,  379,  380,   59,  386,  446,  388,  300,  389,
  380,  317,  389, 1530,  331,  276,  277,   41,  433, 1604,
   44,  902,  339,  284,  436,  906,  276,  277,  405,  410,
    0,  258,  313,  314,  284,  315,  259,  354,  315,  276,
  277,  519,  376,  270,  281,  523,  269,  284,  315, 1686,
  427,  368,  432,   46,  435,  432,  336,  485,  486,  336,
  297, 1601,  260,  315, 1604,   58,  446,  444,   61,  446,
  268,   41,  277,  513,   44, 1601,  303,  282,  305,  453,
  454,  276,  277,   40, 1624,  449,  450,  451,  405,   59,
   42,   43, 1261,   45, 1263,   47, 1636, 1624,   41,  498,
  380,   44, 1687,  380,  376,   42,   43,   59,   45,  389,
   47,  372,  389,  477,  478,  376,  493,  428, 1658, 1805,
 1649, 1661,  372, 1809, 1708, 1811,   41,    0, 1712,   44,
  356, 1817,  393,  513, 1820,  372,  513, 1677,  399, 1836,
  517, 1838, 1682,  393, 1805,  306, 1783, 1687, 1809,  399,
  453,  454,  432,  314, 1840,  432,  393, 1843,  442, 1820,
   41, 1757,  399,   44,   41, 1682,  446,   44,   41,  446,
  426,   44,  433,   42,   43,  377,   45,  372,   47, 1840,
  409,   41, 1868,  433,   44,  375,   59, 1873,  291,  497,
 1876,  509,   42,   43, 1734,   45,  433,   47,  393,  302,
  517,  508, 1888,  681,  399, 1891,  309, 1868,  700,  284,
  702,  508, 1873, 1899,  706,  707,  708, 1757, 1758, 1482,
 1483,  276,  277,  701,   44, 1759,  461,  705,  446,  284,
  333,  318,  493,  513,  712,   61,  513,  366,  433,  717,
  718,  283, 1817,  493,  331,  384,  385,   40,   46,    0,
 1825,  729,  318,  731,  732,  733,  734,  735,  736,  737,
   58,   42,   43,   61,   45,  331,   47,  354, 1843, 1482,
 1483,   46, 1812, 1802,  264,   42,   43, 1817,   45,   40,
   47,  368, 1822,   58,   41, 1825,   61,   44,  354,  376,
   41,   41,  395,   44,   44,  773, 1836,   41, 1838,  389,
   44, 1876,  368, 1843,   40,  260,  276,  277,   59,   41,
   40,   40,   44, 1888,  284,  793, 1891,  372,  405,  453,
   40,  799,  800,  426, 1899,  277,   41,  430,    0,   44,
  282,   41,   41,   40,   44,   44, 1876, 1877,  393,  405,
  277,   40,   40,  298,  399,  282,   41,   40, 1888,   44,
 1890, 1891,  453,   41,   41,  310,   44,   44,   40, 1899,
   41,   42,   43,   40,   45,  453,   47,  271,   40,   41,
   42,   43,   44,   45,   46,   47,  351,   41,  433,  334,
   44,  285,   41,   40,  336,   44,   58,   59,   60,   61,
   62,   40,   40,   44,   41,   42,   43,   40,   45,  336,
   47,   91,  372,  276,  277,   46,  493,   41,  277,   41,
   44,  284,   44,  282,   41,   61,  519,   44,   40,   91,
  264,   93,  395,  393,  317,  329,   44,  277,   44,  399,
  517,  420,  282,   46,    0,   61,  914,  915,  916,  308,
   40,  919,  920,  921,  399,  923,  924,  925,  926,  927,
  928,  517,  409,  357,  409,  264,   44,  344,  344,  937,
  938,  344,  940,  433,  942,  315,  276,  945,  946,  947,
  948,  949,  950,  276,  297,   41,  264,  390,   44,  434,
  428,  462,  390,  919,  962,  921,  964,  923,  924,  450,
  926,  927,  928,   59,  439,  452,  277,  275,  344,  372,
  439,  282,   40,  392,   40,  345,   40,   91,   40,   40,
  277,  262,  264,  470,  471,  282,  315,  421,  376,  403,
  393,   44,   40,  493,  481,  482,  399,  484, 1006,  382,
  375,   40,  380,  443,  491,  492,  317,  494,  495,  315,
  497,  308,   41,  500,  501,  502,  503,  504,  275,   63,
  296,  508,  509,  428,  428,  456,  314,  514,  515,  516,
  433,  518,  519,  520,  315,   42,   43,  318,   45,   40,
   47,   40,   44,  444,  325,    0,  327,   41,   42,   43,
  331,   45,   44,   47,  440,  336,  264,  494,  339,   58,
  262,   40,  264,  265,  273,   40,  277,  297,  281,  271,
  340,  282,  340,  354,  410,  277,  276,  379,  379,  379,
  282,  408,   40,  285,  410,  352,   41,  368,   40,   44,
  493,   42,   43,   44,   45,  376,   47,  299,  379,  380,
  277,  297,  304,  305,   59,  282,  308,   40,  389,   40,
  362,   40,   44,  315,   41,  317,  318,   42,   43,  321,
   45,  438,   47,  325,  405,  327,  317,  329,  277,  331,
  270,  378,  317,  317,  336,  337,   40,  339,   44,  361,
  266,   40,  325, 1151,  493,   40,  427,  264,   41,  414,
   58,  432,  354,  355,   41,  357, 1164, 1165,   41, 1167,
 1168,   41,  264,  444,  306,  446,  368, 1175,  370,  446,
  372,  373,  374,  387,  376,  306,   41,  379,  380,   41,
  276,  277,   40,   40,  317,  281,   40,  389,  284, 1197,
   42,   43,   40,   45,   40,   47,   40, 1205, 1206, 1207,
  402,  297, 1210,  405, 1212,   40,   40,   40, 1216, 1217,
 1218,   40,  493,  281,   40,   40,   40,   40,   40,  421,
   40,  262,  427,  305,   41,  427,  428,   41,   42,   43,
  432,   45,  513,   47,   42,   43,  517,   45,   44,   47,
   44,  443,  444,  446,  446,   41,  264,  381, 1256,  339,
  452,   42,   43,   44,   45,  339,   47,  264,  464,  465,
  466,  467,  468,  469,  470,  471,  379,  273,  470,  471,
  277,  327,  384,  284,  399,  282,  372,   41,  379,  481,
  482,  379,  484,  277,  379,  379,  379, 1295,  282,  491,
  492,  493,  494,  495,  372,  497,  450,  393,  500,  501,
  502,  503,  504,  399,    0,   41,  508,  509,  276,   40,
   44,  513,  514,  515,  516,  517,  518,  519,  520,   41,
   41,  276,  277,   41,  317,   41,  277,   41,  305,  284,
  427,  282,   59,  290,   59,   40,  336,  433,  336,  264,
   59,  305,  297,  324,   59,   41,   42,   43,   44,   45,
   46,   47,  277,   59,  428,   59, 1364,  282,   59,   59,
   59,   59, 1370,   59,   60,   61,   62, 1375,   58,  339,
  277,  333,  298,  334, 1382,   41,   42,   43,  334,   45,
  434,   47,  434,  298,  452,   41,   41,   41,   42,   43,
  378,   45,  365,   47,  348,   91,  365,   93,   41,   41,
   41,   41,  470,  471,   42,   43,   41,   45, 1416,   47,
  262,   41, 1420,  481,  482,   41,  484,  372,   41,   41,
   41,   59,   41,  491,  492,  277,  494,  495, 1436,  497,
  282,   41,  500,  501,  502,  503,  504,   41,  393,   41,
  508,  509,   41,   41,  399,   40,  514,  515,  516,   41,
  518,  519,  520,  452,  262,   41,   42,   43,   59,   45,
  315,   47,   40,  277,  378,  443,   41,   44,  282,  277,
  348,  470,  471,  340,  282,  458,   40,  333,  433,   59,
   46,    0,  481,  482,  500,  484,  277,  336,   40,  336,
   40,  282,  491,  492,  452,  494,  495,  376,  497, 1507,
   44,  500,  501,  502,  503,  504,   41,   40,  402,  508,
  509,  427,  470,  471,   41,  514,  515,  516,   41,  518,
  519,  520,   41,  481,  482,   44,  484,   42,   43,    0,
   45,  376,   47,  491,  492,  376,  494,  495,   59,  497,
   59,   44,  500,  501,  502,  503,  504,  301, 1639,  298,
  508,  509,  434,   41,  304,  290,  514,  515,  516,  301,
  518,  519,  520,   44,  455,    0,  262,  333,  264,  265,
   41,  376, 1580,   44,  376,  271,   40,  409, 1586, 1587,
  264,  277,  264,  276,  450,  317,  282,  462,   59,  285,
   61,  403,  443,   41,   42,   43,   40,   45,   44,   47,
  270,   40, 1610,  299,    0,  301, 1614,   40,  304,  305,
 1618,  277,  308,   41,  450, 1623,  282, 1625,  270,  315,
  264,  317,  318,  277,  427,  321,  339,  428,  282,  325,
  339,  327,  450,  329,   41,  331,   41,   41,   59,  277,
  336,  337,   59,  339,  282,   41,   42,   43,   44,   45,
   46,   47,  305,   41,   41,  301,   59,   59,  354,  355,
   59,  357,    0,   59,   60,   61,   62,    0,    0,   41,
   42,   43,  368,   45,  370,   47,  372,  373,  374, 1242,
  376,  150,  150,  379,  380,  151,  514,  763,  291,  516,
  290,  277, 1124,  389,  754,   91,  282,   93, 1126, 1707,
  665,   41,   42,   43,  894,   45,  402,   47,   92,  405,
   78,  899,  856, 1121, 1805, 1549, 1116, 1548, 1809,  388,
 1811,  357,  614,  337, 1106,  421, 1051, 1619,  795, 1820,
 1117,  427,  428,  821, 1677,  369,  432,  369, 1746, 1527,
 1828,  591, 1529, 1832, 1831, 1857,  995,  443,  444, 1840,
  446, 1746, 1439,  812, 1644, 1818,  452,  276,  277,  743,
 1274, 1512,  277,  374,  355,  284,  362,  282, 1776,  873,
 1328,   13,  593,  865,  470,  471,  324, 1868,  297,  617,
  651,  532, 1873,  536,    0,  481,  482,  258,  484,  784,
  156,  773, 1262, 1014,   88,  491,  492,  493,  494,  495,
  698,  497,  273, 1776,  500,  501,  502,  503,  504, 1395,
  799, 1819,  508,  509,  547,  761, 1601,  513,  514,  515,
  516,  517,  518,  519,  520,   41,  297, 1007,   44,  277,
 1282, 1166,  303, 1212,  282,  306,  317,   -1,   -1,   -1,
   41,   42,   43,   59,   45,   -1,   47,   -1,   41,   42,
   43,   -1,   45,  372,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   42,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,  393,  271,   -1,  348,   -1,   -1,
  399,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,  277,   -1,   -1,  304,  305,
  282,   -1,  308,   -1,  433,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,  403,  329,   -1,  331,   -1,  277,   -1,  410,
  336,  337,  282,  339, 1639,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,  428,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,  458,   -1,  470,
  471,  462,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   41,   42,   43,   -1,   45,  452,   47,   -1,   -1,
  276,  277,   -1,   41,   42,   43,   -1,   45,  284,   47,
   -1,   -1,   -1,   -1,  470,  471,  277,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  277,  481,  482,   -1,  484,  282,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
 1805,   -1,  508,  509, 1809,   -1, 1811,  513,  514,  515,
  516,  517,  518,  519,  520, 1820,  277,   -1,   41,   42,
   43,  282,   45,   -1,   47,   41,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1, 1840,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   45,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1, 1868,   -1,  271,   -1,  393, 1873,   -1,
   -1,  277,   -1,  399,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,  433,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,  291,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,  302,   -1,   -1,   -1,   -1,  354,  355,
  309,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,  322,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,  277,   -1,  470,
  471,   -1,  282,  389,   -1,   91,   -1,   93,   -1,  277,
  481,  482,   -1,  484,  282,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
  379,  427,  428,  514,  515,  516,  432,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,  395,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,  411,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,  277,   -1,   -1,  470,  471,  282,  426,   -1,   -1,
   -1,  430,   -1,   -1,  433,  481,  482,   -1,  484,   -1,
   -1,   -1,  441,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  460,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520, 1365, 1366,   -1,   -1,   -1,
   -1,   -1,   -1, 1373, 1374,   -1, 1376, 1377,   -1,   -1,
   -1,   -1,   -1,   -1, 1384, 1385, 1386, 1387,   -1, 1389,
 1390, 1391, 1392, 1393, 1394,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
  519,   -1,   -1,   -1,   -1,  271,   -1,   58,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,  450,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,  302,   -1,   -1,   -1,  452,   -1,   -1,  309,
   -1,   -1,   -1,  313,  314,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  322,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  333,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  395,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  411,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,  426,   -1,   -1,   -1,
  430,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,  441,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
  460,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,  519,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,  408,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
  275,  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,
   -1,  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,  276,  277,   -1,   -1,  271,  281,   -1,   59,  284,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,
   -1,   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,
   -1,   -1,   -1,  389,  399,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,    0,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,
   -1,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   -1,  331,   45,   -1,   -1,   -1,
  336,  337,   -1,  339,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,
   -1,  427,  428,  514,  515,  516,  432,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,  258,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
  273,   -1,  275,  276,  277,   -1,   -1,   -1,  281,   -1,
   -1,  284,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  297,  481,  482,   -1,  484,   -1,
  303,   -1,   -1,  306,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  372,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   59,  264,  265,
   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
  393,   -1,   -1,  292,   -1,   -1,  399,   -1,   -1,  285,
  403,   -1,   -1,    0,   -1,   -1,   -1,  410,   -1,   -1,
   -1,   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,
   -1,  320,  308,   -1,   -1,  428,   -1,   -1,   -1,  315,
  433,  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,
   -1,  327,   -1,  329,   41,  331,   43,   44,   45,  348,
  336,  337,   -1,  339,   -1,  458,   -1,   -1,   -1,  462,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,
  376,   -1,   -1,  379,  380,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,
  419,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  421,  435,   -1,   -1,   -1,
   -1,  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,
  449,  450,  451,   -1,  453,  454,   -1,  443,  444,   -1,
  446,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,   -1,   -1,  470,  471,   -1,   -1,  487,  488,
   -1,   -1,   -1,   -1,   -1,  481,  482,  496,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  261,   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,
  516,  517,  518,  519,  520,   -1,   -1,   -1,  279,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  287,   -1,   -1,   -1,
   -1,   -1,   -1,  294,   -1,  296,   -1,  298,   -1,   -1,
   -1,   -1,  303,   -1,   -1,  262,   -1,  264,  265,  310,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,    0,  334,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,   -1,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,  397,  354,  355,   -1,
  357,   -1,   -1,  404,   -1,   -1,  407,   -1,  409,  410,
  452,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,  470,  471,
   -1,   -1,  389,  434,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  402,   -1,   -1,  405,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,  421,   -1,  508,  509,   -1,   -1,
  427,  428,  514,  515,  516,  432,  518,  519,  520,   -1,
   -1,   -1,  483,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  498,  499,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,   -1,  275,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  292,
   -1,  284,  295,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,  367,   -1,  369,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  299,   -1,   -1,   -1,  398,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,  415,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,  472,
  473,  379,  380,   -1,   93,   -1,   -1,  470,  471,   -1,
   -1,  389,   -1,   -1,   -1,   -1,  489,   -1,  481,  482,
   -1,  484,   -1,   -1,  402,   -1,   -1,  405,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,  421,   -1,  508,  509,   -1,   -1,  427,
  428,  514,  515,  516,  432,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
   -1,  275,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  292,   -1,
   -1,  295,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,  271,  367,   -1,  369,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,  398,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,  415,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   41,  331,   43,   44,   45,   -1,  336,  337,   -1,
  339,   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,  472,  473,
  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,  489,   -1,   -1,   41,   42,
   43,   -1,   45,  402,   47,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,   62,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,    0,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   59,   -1,   -1,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,    0,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   -1,  331,   -1,   -1,   -1,   -1,  336,  337,  271,  339,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   41,  282,
   -1,   44,  285,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  321,  389,
   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,   -1,
   -1,   -1,  402,   -1,  337,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  421,  355,   -1,  357,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,  370,   -1,  372,
  373,  374,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,  421,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,  432,
   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   91,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,  470,  471,   -1,
   -1,   -1,   -1,  376,   -1,   -1,   -1,  380,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,  389,   -1,  491,  492,
  493,  494,  495,    0,  497,   -1,   -1,  500,  501,  502,
  503,  504,  405,   -1,   -1,  508,  509,   -1,   -1,   -1,
  513,  514,  515,  516,  517,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,   46,
   47,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  452,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   91,   -1,   -1,   -1,  491,  492,
  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  262,
  513,  514,  515,  516,  517,  518,  519,  520,  271,   -1,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  282,
   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,
   -1,   -1,   -1,  336,  337,   -1,  339,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  357,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,  370,    0,  372,
  373,  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,   -1,   -1,  262,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,  271,   -1,   -1,   -1,  421,   -1,
  277,   -1,   -1,   -1,  427,  282,   -1,   59,  285,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   41,  339,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   59,
  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  513,  368,    0,  370,  517,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  262,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,  271,   -1,   -1,
  427,   59,    0,  277,   -1,  432,   -1,   -1,  282,   -1,
   -1,  285,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  315,   -1,   41,  318,   -1,   44,  321,   -1,   -1,
   -1,  325,   -1,  327,   -1,  329,   -1,  331,   -1,   -1,
  262,   59,  336,  337,   -1,  339,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
  354,  355,   -1,  357,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,  368,   -1,  370,   -1,  372,  373,
  374,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   41,   -1,
   -1,   44,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,  405,  262,   -1,  336,   -1,   59,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,    0,
   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,
  444,   -1,  446,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   41,   -1,   -1,   44,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,  262,   -1,  336,   -1,   59,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  493,
   -1,    0,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  513,
   -1,   -1,  444,  517,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   41,   -1,  262,   44,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   -1,
   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,    0,   -1,   -1,  354,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  513,   -1,   -1,  444,  517,  446,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   41,   -1,   -1,   44,  336,  262,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,   -1,   59,    0,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,
  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,   -1,
   -1,  389,  315,   -1,   41,  318,   -1,   44,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,  405,  331,   -1,
   -1,  262,   59,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  427,
   -1,  354,   -1,   -1,  432,  493,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,  446,   -1,
   -1,   -1,   -1,  376,   -1,  513,  379,  380,   -1,  517,
   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,   41,
   -1,   -1,   44,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,  405,  262,   -1,  336,   -1,   59,  339,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
    0,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,  513,   -1,  368,   -1,  517,
   -1,  444,   -1,  446,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,
   -1,   41,   -1,   -1,   44,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,  405,  262,   -1,  336,   -1,   59,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  493,   -1,    0,   -1,   -1,  354,  427,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
  513,   -1,   -1,  444,  517,  446,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   41,   -1,  262,   44,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,
   -1,   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  493,   -1,    0,   -1,   -1,  354,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,  513,   -1,   -1,  444,  517,  446,  315,  376,
   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,  389,   -1,  331,   41,   -1,   -1,   44,  336,
  262,   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   59,    0,   -1,   -1,  354,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,
  427,  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,  513,   -1,   -1,  444,  517,  446,
   -1,   -1,  389,  315,   -1,   41,  318,   -1,   44,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,  405,  331,
   -1,   -1,  262,   59,  336,   -1,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
  427,   -1,  354,   -1,   -1,  432,  493,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,  446,
   -1,   -1,   -1,   -1,  376,   -1,  513,  379,  380,   -1,
  517,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,   -1,
   41,   -1,   -1,   44,   -1,  325,   -1,  327,   -1,   -1,
   -1,  331,   -1,  405,  262,   -1,  336,   -1,   59,  339,
   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,
   -1,    0,   -1,   -1,  354,  427,   -1,   -1,   -1,   -1,
  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,  368,   -1,
  517,   -1,  444,   -1,  446,   -1,  376,   -1,   -1,  379,
  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   -1,   41,   -1,   -1,   44,   -1,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,  262,   -1,  336,   -1,
   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  493,   -1,    0,   -1,   -1,  354,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,  513,   -1,   -1,  444,  517,  446,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  389,  318,   -1,   41,   -1,  262,   44,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,
  336,   -1,   59,  339,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  493,   -1,    0,   -1,   -1,  354,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  368,  513,   -1,   -1,  444,  517,  446,  315,
  376,   -1,  318,  379,  380,   -1,   -1,   -1,   -1,  325,
   -1,  327,   -1,  389,   -1,  331,   41,   -1,   -1,   44,
  336,  262,   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,
   -1,   -1,   -1,   -1,   59,    0,   -1,   -1,  354,   -1,
   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,
   -1,  427,  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,  513,   -1,   -1,  444,  517,
  446,   -1,   -1,  389,  315,   -1,   41,  318,   -1,   44,
   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,  405,
  331,   -1,   -1,  262,   59,  336,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,  427,   -1,  354,   -1,   -1,  432,  493,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,   -1,
  446,   -1,   -1,   -1,   -1,  376,   -1,  513,  379,  380,
   -1,  517,   -1,   -1,   -1,   -1,  315,   -1,  389,  318,
   -1,   41,   -1,   -1,   44,   -1,  325,   -1,  327,   -1,
   -1,   -1,  331,   -1,  405,  262,   -1,  336,   -1,   59,
  339,   -1,   -1,   -1,   -1,   -1,   -1,  493,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  354,  427,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,  368,
   -1,  517,   -1,  444,   -1,  446,   -1,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  389,  318,   -1,   41,   -1,   -1,   44,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,  405,  262,   -1,  336,
   -1,   59,  339,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,  493,   -1,   -1,   -1,   -1,  354,  427,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,  513,   -1,   -1,  444,  517,  446,   -1,  376,
   -1,   -1,  379,  380,   -1,   40,   41,   -1,   -1,   44,
  315,   -1,  389,  318,   -1,   -1,   -1,  262,   -1,   -1,
  325,   -1,  327,   -1,   59,   -1,  331,   -1,  405,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,  354,
  427,   -1,   -1,   -1,    0,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  368,  513,   -1,   -1,  444,  517,  446,
  315,  376,   -1,  318,  379,  380,   -1,   -1,   41,   -1,
  325,   44,  327,   -1,  389,   -1,  331,   -1,   -1,   -1,
   -1,  336,  262,   -1,  339,   41,   59,   -1,   44,   -1,
  405,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   59,   -1,   -1,  493,   -1,   -1,   -1,
   -1,   -1,  427,  368,   -1,   -1,   -1,  432,   -1,   -1,
   -1,  376,   -1,   -1,  379,  380,  513,   -1,   -1,  444,
  517,  446,   -1,   -1,  389,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,
  405,  331,   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,  354,   -1,   -1,  432,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,  444,
   -1,  446,   -1,   -1,   -1,   -1,  376,   -1,  513,  379,
  380,   -1,  517,   -1,   -1,   -1,   -1,  315,   -1,  389,
  318,   40,   -1,   -1,   43,   -1,   45,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  405,   -1,   -1,  336,   58,
   -1,  339,   -1,   -1,   63,   -1,   -1,   -1,  493,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  427,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,  513,   -1,
  368,   -1,  517,   -1,  444,   -1,  446,   -1,  376,   -1,
  315,  379,  380,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  325,  389,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,
   -1,  336,   -1,   -1,  339,   -1,   -1,  405,   -1,   -1,
   42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,  493,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,  368,  432,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  513,   -1,  380,  444,  517,  446,   -1,
   -1,   -1,  315,   -1,  389,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,  315,
  405,   -1,  318,  336,   -1,   -1,  339,   -1,   -1,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,  354,   -1,  339,   -1,  493,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,  354,  444,
   -1,  446,   -1,  376,   -1,  513,   -1,  380,   -1,  517,
   -1,   -1,  368,   -1,   -1,   -1,  389,   -1,   -1,   -1,
  376,   -1,   -1,   -1,  380,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  405,  389,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,  493,  405,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,  432,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,  513,   -1,
   -1,  444,  517,  446,   40,   -1,  432,   43,   -1,   45,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,  444,   -1,
  446,  320,   58,   -1,  323,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
  493,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,  493,   -1,   -1,
  513,   -1,   -1,  372,  517,   -1,   -1,   -1,   -1,   -1,
   -1,  293,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,
   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  409,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,  284,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,  505,  506,  507,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
  452,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
  286,   -1,   -1,  289,   -1,   -1,  292,  293,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  311,   -1,   -1,   -1,  491,
  492,   -1,  494,  495,  320,  497,   -1,  323,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
  452,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,  481,
  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
  267,  487,  488,   -1,  490,  491,  492,  274,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  286,
   -1,   -1,  508,  509,   -1,  292,  293,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   58,   -1,  323,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,  272,  364,   -1,  275,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,  288,   -1,   -1,   -1,  292,   -1,   -1,  295,
   -1,   -1,   -1,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   60,   61,   62,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,  335,
   -1,   -1,   -1,   -1,   -1,  341,  342,  343,  435,  345,
  346,  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,  367,   -1,  369,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,  398,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,  415,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   40,   -1,   -1,   43,   -1,
   45,  437,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,  489,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,  360,   -1,  277,  367,
  364,  369,   -1,  282,   -1,   -1,  285,   -1,  372,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
   -1,   -1,  321,   -1,   -1,   -1,   -1,  415,   -1,  413,
  329,   -1,   -1,   -1,   -1,  419,   -1,   -1,  337,   -1,
   -1,  425,   -1,   -1,  325,   -1,   -1,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  370,   -1,  372,  373,  374,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,  267,  487,  488,   -1,  490,  491,  492,  274,
  494,  495,  496,  497,  395,   -1,  500,  501,  502,  503,
  504,  286,  421,   -1,  508,  509,   -1,  292,  293,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  311,   -1,   -1,  430,
   -1,   -1,   -1,   -1,  435,  320,   58,   -1,  323,   -1,
  441,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
  481,  482,   -1,  484,   -1,   -1,   -1,  372,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,
   -1,  323,   63,   -1,  452,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,
   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  260,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  298,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  310,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,  334,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,  399,  274,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,
   -1,   -1,  323,   63,   -1,  452,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  298,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  310,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,  334,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  399,  274,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
  320,   -1,   -1,  323,   63,   -1,  452,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,  272,
   -1,   -1,  275,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  288,   -1,  286,   -1,  292,
   -1,   -1,  295,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  320,   -1,   -1,  323,   63,   -1,   -1,   -1,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,
  343,  452,  345,  346,  347,   -1,  349,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  360,   -1,   -1,  367,  364,  369,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  398,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,  472,
  473,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,  489,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  320,   -1,   -1,  323,   63,   -1,  452,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,
  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
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
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,  452,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  286,   -1,    0,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,
  452,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   59,  348,   61,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   91,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,
  258,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   58,   -1,   -1,   -1,  273,   63,  275,  276,  277,
   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,  419,  393,   -1,   -1,   -1,   -1,
  425,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
  435,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
  458,   -1,  487,  488,  462,  490,  491,  492,   -1,  494,
  495,  496,  497,  260,   -1,  500,  501,  502,  503,  504,
  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,  300,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,   -1,   -1,    0,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   58,   -1,   -1,   -1,  300,   63,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  270,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,  309,  310,   -1,  312,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,  328,   -1,   -1,   -1,   -1,  463,  334,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  267,
   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,
  516,   -1,  518,  519,  520,  391,   -1,  258,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   40,   -1,   -1,   43,
   -1,   45,  273,  409,  275,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   58,   -1,   -1,   -1,  424,   63,
   -1,   -1,  320,   -1,   -1,  323,  297,   -1,  434,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,  470,  471,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  348,  484,   -1,
   -1,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,  372,  508,  509,  510,  511,  512,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,  419,  393,   -1,   -1,   -1,   -1,  425,  399,   -1,
   -1,   -1,  403,   -1,   -1,   -1,   -1,  435,   -1,  410,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  428,   -1,   -1,
   -1,   -1,  433,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,  458,   -1,  487,
  488,  462,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,  272,   -1,  323,
  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  290,   -1,  292,   -1,   -1,
  295,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,   -1,
  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  367,   -1,  369,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,  398,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
  415,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,  437,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,
  514,  515,  516,   -1,  518,  519,  520,  472,  473,    0,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   40,   41,
   -1,   43,   -1,   45,  489,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
  339,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  274,  362,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  286,   -1,   -1,  258,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,   -1,  297,   -1,   -1,  418,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,  452,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,  372,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,  419,   -1,  508,
  509,   -1,  393,  425,   -1,  514,  515,  516,  399,  518,
  519,  520,  403,  435,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,  463,  433,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,  458,  490,  491,
  492,  462,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,    0,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,  323,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  446,   -1,   -1,  449,  450,  451,  452,  453,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,  258,
   -1,   -1,  292,  293,   -1,   -1,   58,   59,   -1,   -1,
   -1,   63,   -1,   -1,  273,   -1,   -1,  276,  277,   -1,
   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,  297,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,  393,  425,   -1,   -1,   -1,   -1,
  399,   -1,   -1,   -1,  403,  435,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,  463,  433,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,  458,
  490,  491,  492,  462,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,  267,   -1,   -1,  508,  509,
   -1,   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,    0,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   61,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,
   -1,  258,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,  273,   -1,   -1,  276,
  277,   -1,   -1,   -1,  281,   -1,   -1,  284,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,  393,  425,   -1,   -1,
   -1,   -1,  399,   -1,   -1,   -1,  403,  435,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,  428,   -1,   -1,   -1,  463,  433,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,  458,  490,  491,  492,  462,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,   -1,
  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,   -1,
   -1,   -1,  292,  293,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,
  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,
   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,
  450,  451,  452,  453,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,
  490,  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,
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
   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,   -1,  484,   -1,
   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,  495,
  496,  497,   -1,   -1,  500,  501,  502,  503,  504,  267,
   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,
  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,
   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,
  270,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,  305,   -1,   -1,   -1,  309,
  310,   -1,  312,   -1,   -1,  315,   -1,   -1,   -1,   -1,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,  328,   -1,
   -1,   -1,   -1,   -1,  334,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  463,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,  391,  490,  491,  492,   -1,  494,  495,  496,  497,
  270,   -1,  500,  501,  502,  503,  504,   -1,   -1,  409,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  424,   -1,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,  309,
  310,   -1,  312,  443,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,  328,   -1,
   -1,   -1,   -1,  272,  334,   -1,  275,   -1,   -1,   -1,
  470,  471,  281,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  292,  484,   -1,  295,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  514,  515,  516,   -1,  518,  519,
  520,  391,   -1,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,  409,
  349,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,  367,   -1,
  369,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,  437,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
  510,  511,  512,  452,  514,  515,  516,   -1,  518,  519,
  520,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,   -1,   -1,   -1,   -1,   -1,
   -1,  292,  481,  482,  295,  484,   -1,   -1,   -1,   -1,
  489,   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,   -1,   -1,   -1,  367,   -1,  369,  461,
  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,
  472,  473,  474,  475,  476,   -1,   -1,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,  398,   -1,  491,
  492,  493,  494,  495,  496,  497,  498,   -1,  500,  501,
  502,  503,  504,   -1,  415,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,  489,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
  452,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
  462,  463,  464,  465,  466,  467,  468,  469,  470,  471,
  472,  473,  474,  475,  476,   -1,   -1,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,   -1,  491,
  492,  493,  494,  495,  496,  497,  498,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,  516,  517,  518,  519,  520,
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
"column_def : column_def_name data_type_or_domain default_opt end_trigger column_constraint_clause collate_clause",
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
"keyword_or_column : COLUMN",
"keyword_or_column : TYPE",
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
"keyword_or_column : CURRENT_CONNECTION",
"keyword_or_column : CURRENT_TRANSACTION",
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
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
					LexerState savedState = lex;
					int nextToken = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
					int token = yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
case 458:
{ yyval = NULL; }
break;
case 459:
{ yyval = NULL; }
break;
case 460:
{ yyval = NULL; }
break;
case 461:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 462:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 463:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 464:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 465:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 466:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 467:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 468:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 469:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 470:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 471:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 473:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 474:
{ yyval = yyvsp[0]; }
break;
case 475:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 477:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 478:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 479:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 481:
{ yyval = NULL; }
break;
case 483:
{ yyval = NULL; }
break;
case 484:
{ yyval = yyvsp[0]; }
break;
case 485:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 503:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 505:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 506:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 507:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 513:
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
case 514:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 516:
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
case 517:
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
case 518:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 526:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 527:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 528:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 529:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 530:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 546:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 547:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 548:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 549:
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
case 550:
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
case 553:
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
case 554:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 555:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 557:
{ yyval = yyvsp[-1]; }
break;
case 558:
{ yyval = 0; }
break;
case 562:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 563:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 564:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 568:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 569:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 570:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 571:
{ yyval = 0; }
break;
case 572:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 575:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 576:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 579:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 580:
{ yyval = NULL; }
break;
case 582:
{ yyval = NULL; }
break;
case 583:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 585:
{ yyval = NULL; }
break;
case 587:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 592:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 594:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 595:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 596:
{ yyval = yyvsp[0];}
break;
case 598:
{ yyval = yyvsp[0];}
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 604:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 605:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 606:
{ yyval = 0; }
break;
case 607:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 608:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 609:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 610:
{ yyval = (dsql_nod*) 0; }
break;
case 611:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 612:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 614:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 615:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 616:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 617:
{ yyval = 0; }
break;
case 619:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 620:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 621:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 622:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 623:
{ yyval = NULL; }
break;
case 624:
{ yyval = yyvsp[0]; }
break;
case 625:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 627:
{ yyval = NULL; }
break;
case 628:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 629:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 630:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 631:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 632:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 633:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 635:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 636:
{ lex.limit_clause = true; }
break;
case 637:
{ lex.limit_clause = false; }
break;
case 638:
{ lex.first_detection = true; }
break;
case 639:
{ lex.first_detection = false; }
break;
case 640:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 641:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 642:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 643:
{ yyval = 0; }
break;
case 644:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 645:
{ yyval = yyvsp[-2]; }
break;
case 646:
{ yyval = yyvsp[-1]; }
break;
case 647:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 648:
{ yyval = yyvsp[-1]; }
break;
case 649:
{ yyval = yyvsp[0]; }
break;
case 650:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 651:
{ yyval = 0; }
break;
case 652:
{ yyval = make_list (yyvsp[0]); }
break;
case 653:
{ yyval = 0; }
break;
case 655:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 657:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 660:
{ yyval = make_list (yyvsp[0]); }
break;
case 662:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 667:
{ yyval = yyvsp[-1]; }
break;
case 668:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 670:
{ yyval = NULL; }
break;
case 671:
{ yyval = make_list (yyvsp[-1]); }
break;
case 672:
{ yyval = NULL; }
break;
case 674:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 679:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 680:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 683:
{ yyval = yyvsp[0]; }
break;
case 684:
{ yyval = make_list (yyvsp[-1]); }
break;
case 685:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 686:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 687:
{ yyval = make_list (yyvsp[-1]); }
break;
case 688:
{ yyval = NULL; }
break;
case 690:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 691:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 692:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 699:
{ yyval = make_list (yyvsp[0]); }
break;
case 700:
{ yyval = NULL; }
break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 704:
{ yyval = yyvsp[0]; }
break;
case 705:
{ yyval = NULL; }
break;
case 706:
{ yyval = yyvsp[0]; }
break;
case 707:
{ yyval = NULL; }
break;
case 708:
{ yyval = yyvsp[0]; }
break;
case 709:
{ yyval = NULL; }
break;
case 710:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 711:
{ yyval = 0; }
break;
case 712:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 713:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 714:
{ yyval = 0; }
break;
case 715:
{ yyval = 0; }
break;
case 717:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 723:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 724:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_list (yyvsp[-1]); }
break;
case 728:
{ yyval = 0; }
break;
case 729:
{ yyval = make_list (yyvsp[0]); }
break;
case 730:
{ yyval = 0; }
break;
case 732:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 733:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 734:
{ yyval = 0; }
break;
case 735:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 736:
{ yyval = 0; }
break;
case 737:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 738:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 739:
{ yyval = yyvsp[-1]; }
break;
case 740:
{ yyval = 0; }
break;
case 741:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 743:
{ yyval = NULL; }
break;
case 744:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 745:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 749:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 750:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 753:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 754:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 758:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 759:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 766:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 767:
{ yyval = yyvsp[0]; }
break;
case 769:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 772:
{ yyval = NULL; }
break;
case 773:
{ yyval = make_list (yyvsp[-1]); }
break;
case 775:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = NULL; }
break;
case 778:
{ yyval = make_list (yyvsp[-1]); }
break;
case 780:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = yyvsp[-1]; }
break;
case 800:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 839:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 842:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 844:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 846:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 848:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 850:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 852:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 854:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 855:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 856:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 857:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 859:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 860:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 861:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 863:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 865:
{ yyval = make_list (yyvsp[-1]); }
break;
case 866:
{ yyval = yyvsp[-1]; }
break;
case 877:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 878:
{ yyval = yyvsp[0]; }
break;
case 879:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 880:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 881:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 882:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 883:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 884:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 885:
{ yyval = yyvsp[-1]; }
break;
case 886:
{ yyval = yyvsp[-1]; }
break;
case 890:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 891:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 892:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 895:
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
case 896:
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
case 897:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 898:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 900:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 902:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 903:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 904:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 905:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 909:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 910:
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
case 911:
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
case 912:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 913:
{ yyval = make_parameter (); }
break;
case 914:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 915:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 916:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 920:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 921:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 922:
{ yyval = yyvsp[0]; }
break;
case 923:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 925:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 926:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 927:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 928:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 929:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 931:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 932:
{ yyval = yyvsp[0];}
break;
case 936:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 937:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 938:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 939:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 940:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 941:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 943:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 944:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 945:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 946:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 950:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 952:
{ yyval = yyvsp[0]; }
break;
case 953:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 954:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 956:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 959:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 960:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 961:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 965:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 966:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 967:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 968:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 969:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 971:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 975:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 976:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 977:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 979:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 989:
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
