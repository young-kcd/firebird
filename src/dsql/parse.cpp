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
  187,  196,  196,  197,  193,  203,  204,  194,  201,  201,
  198,  208,  192,  209,  206,  206,  206,  206,  202,  205,
  205,  200,  200,  200,  207,  207,  190,  190,  213,  214,
  214,  215,  215,  215,  215,  199,  199,  199,  217,  219,
  218,  220,   11,  221,  221,  221,  222,  222,  222,   12,
  224,  224,  225,  225,  226,   77,   86,  142,  230,   93,
   95,  229,  229,   76,   89,  231,  231,  231,  232,  235,
  235,  236,  236,  236,  236,  236,  236,  236,  236,  236,
  236,  236,  236,  236,  236,  236,  233,  233,  234,    2,
  238,  238,  238,  238,  238,  238,  238,  238,  247,  248,
  244,  244,  249,  249,  249,  249,  249,  249,  249,  239,
  239,  250,  250,  250,  250,  250,  250,  250,  243,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  252,  252,  254,  254,  253,  251,  251,  251,  245,  245,
  246,  246,  246,  241,  242,  242,  258,  258,  258,  258,
  258,  240,  259,  259,  260,  260,    8,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  261,  261,
  107,  107,  139,  139,  264,  264,  267,  267,  269,  269,
   56,   56,  266,  266,  266,  266,  266,  266,  266,  266,
  266,  273,  273,  265,  265,  265,  265,  275,  275,  274,
  274,  274,   58,   58,  270,  270,  270,  268,  268,  268,
  279,  279,  279,  278,  278,  277,  277,  277,  271,  271,
  280,  280,  280,  281,  281,  272,  272,  272,  272,  282,
  282,   19,   19,   19,  284,  284,  284,   17,   17,   17,
  286,  287,  290,  290,  288,  292,  292,    4,   16,  291,
  291,  293,  293,  294,  294,  283,  295,  295,  296,  296,
  297,  297,  297,  297,  298,  298,  299,  299,  300,  300,
  302,  302,  302,  303,  303,  303,  304,  304,  304,  301,
  306,  306,  306,  307,  307,  305,  305,  308,  310,  310,
  309,  309,  285,   18,  311,  311,  313,  313,  312,  312,
  228,  317,  318,  314,  314,  314,  319,  321,  330,  331,
  332,  333,  322,  322,  322,  322,  334,  334,  334,  335,
  335,  335,  323,  323,  324,  324,  337,  337,  338,  338,
  339,  339,  325,  341,  341,  342,  342,  344,  344,  344,
  346,  347,  347,  348,  348,  350,  350,  343,  343,  343,
  351,  352,  353,  355,  355,  356,  357,  345,  345,  358,
  358,  359,  359,   24,  354,  354,  354,  354,  354,  360,
  360,  327,  327,  361,  361,  362,  328,  328,  326,  326,
  329,  329,  363,  364,  364,  364,  364,  364,  365,  365,
  366,  366,  367,  367,  368,  368,  368,  369,  369,  370,
  370,  315,  315,  371,  371,  372,   71,   71,   71,  374,
  374,  373,  373,  316,  316,  316,   10,   10,    7,    7,
  376,  377,   20,   20,  379,  380,  378,  381,  381,  191,
  195,    3,    3,  384,  384,  384,  386,  386,  274,  385,
  385,  387,  387,   34,   34,   91,   90,   90,  375,  375,
  388,  389,  389,  211,  211,  211,  141,  382,  382,  117,
  117,  117,  117,  117,  393,  393,  393,  393,  393,  393,
  392,  392,  392,  395,  395,  395,  395,  395,  395,  395,
  395,  395,  395,  395,  396,  396,  396,  396,  396,  396,
  396,  396,  402,  402,  402,  402,  402,  402,  402,  402,
  402,  402,  402,  402,  402,  402,  402,  402,  407,  407,
  397,  397,  398,  398,  399,  399,  399,  399,  400,  400,
  404,  404,  405,  405,  405,  405,  403,  406,  401,  401,
  391,  391,  391,  394,  394,  394,  408,  408,  409,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,   94,   94,   94,
   94,   94,   94,   94,   94,   94,   94,  152,  152,  152,
  410,  223,  223,  147,  147,  416,  416,  416,  416,  416,
  412,  412,  412,  412,  412,  227,  148,  148,  149,  150,
  150,  150,  150,  150,   54,   54,   21,   21,  237,  417,
   57,  276,  257,  257,  101,  411,  411,  411,  418,  418,
  418,  418,  418,  418,  418,  418,  418,  418,  418,  419,
  421,  420,  420,  423,  424,  424,  383,  383,  413,  414,
  414,  425,  425,  425,  426,  426,  427,  427,  430,  430,
  428,  428,  433,  433,  432,  429,  431,  415,  415,  422,
  422,  422,  422,  422,  422,  422,  422,  336,  336,  320,
  320,  151,   51,   67,  126,  143,  390,  161,  185,  263,
   69,   65,  216,  118,   72,  340,  212,   35,   44,  349,
  262,   42,   45,  210,   43,  289,  256,  256,  434,  434,
  434,  434,  434,  434,  434,  434,  434,  434,  434,  434,
  434,  434,  434,  434,  434,  434,  434,  434,  434,  434,
  434,  434,  434,  434,  434,  434,  434,
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
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    1,    1,    1,
    1,    2,    3,    1,    3,    8,    9,    5,    8,    6,
    3,    0,    3,    2,    1,    1,    3,    3,    7,    2,
    0,    1,    1,    2,    3,    0,    1,    2,    4,    1,
    3,    2,    2,    2,    1,    1,    1,    1,    2,    2,
    5,    0,    5,    1,    3,    0,    2,    4,    0,    7,
    3,    0,    1,    3,    3,    7,    7,    0,    0,    0,
    0,    3,    0,    9,    9,    1,    1,    0,    2,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    5,    5,    5,    5,    5,    5,    2,    0,    4,    2,
    3,    3,    2,    2,    3,    3,    2,    2,    3,    6,
    1,    2,    4,    3,    2,    2,    2,    2,    3,    1,
    3,    3,    3,    2,    2,    4,    4,    5,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    0,    2,    2,
    4,    4,    5,    0,    1,    2,    2,    4,    3,    2,
    2,    7,    1,    0,    1,    0,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    3,    2,    2,    2,    2,
    1,    1,    1,    1,    4,    5,    1,    3,    1,    3,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    4,    4,    6,    5,    3,    0,    2,
    2,    0,    3,    0,    4,    1,    5,    4,    1,    4,
    1,    2,    2,    1,    1,    1,    2,    2,    2,    2,
    0,    3,    5,    1,    1,    2,    3,    1,    2,    3,
    0,    1,    1,    1,    5,    5,    6,    1,    1,    1,
    2,    4,    1,    0,    5,    1,    0,    3,    2,    1,
    0,    2,    0,    1,    0,    3,    1,    0,    1,    2,
    1,    1,    1,    1,    2,    2,    1,    2,    3,    1,
    1,    3,    3,    1,    2,    3,    1,    2,    0,    2,
    1,    1,    0,    1,    1,    1,    3,    2,    3,    0,
    1,    3,    4,    3,    3,    0,    2,    0,    2,    0,
    3,    3,    3,    1,    4,    4,    1,    9,    0,    0,
    0,    0,    3,    2,    1,    0,    3,    5,    3,    2,
    5,    2,    1,    1,    1,    1,    1,    3,    1,    3,
    1,    0,    2,    1,    3,    1,    1,    1,    1,    3,
    6,    1,    0,    3,    0,    1,    3,    1,    1,    1,
    4,    5,    5,    1,    1,    2,    4,    4,    2,    3,
    0,    1,    2,    1,    1,    2,    2,    2,    0,    1,
    0,    3,    0,    1,    3,    1,    2,    0,    2,    0,
    2,    0,    4,    1,    2,    1,    1,    0,    1,    3,
    2,    1,    1,    2,    1,    4,    3,    1,    3,    4,
    0,    3,    0,    1,    3,    3,    1,    1,    0,    1,
    1,    4,    0,    2,    4,    0,    8,    5,    1,    1,
    7,    4,    1,    1,    8,    5,    4,    1,    3,    3,
    1,    7,    7,    5,    3,    0,    1,    1,    1,    2,
    0,    1,    1,    1,    0,    3,    1,    3,    1,    0,
    3,    1,    3,    1,    3,    3,    1,    1,    3,    1,
    2,    1,    3,    3,    1,    2,    3,    3,    3,    3,
    1,    3,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    1,    1,
    5,    6,    5,    6,    3,    4,    5,    6,    3,    4,
    3,    4,    3,    4,    4,    5,    4,    4,    3,    4,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    3,    3,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    3,    1,    1,    1,    1,    1,    1,
    4,    1,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    2,    1,    2,    1,    1,
    1,    1,    1,    2,    1,    1,    1,    1,    4,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    1,
    6,    1,    4,    7,    2,    0,    4,    3,    6,    1,
    1,    6,    8,    6,    1,    1,    4,    6,    4,    5,
    3,    5,    4,    5,    1,    1,    1,    4,    6,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
    0,  562,  563,  564,  568,  569,  570,    0,  634,  637,
  749,  750,  753,  754,  474,    0,    0,    0,    0,    0,
    0,    0,  400,  580,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  122,    0,    0,    0,  102,    0,
  929,    0,   22,  927,    0,    0,   73,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  487,    0,    0,    0,   36,   33,   35,    0,   34,
    0, 1017, 1019, 1020, 1021, 1022, 1023, 1024, 1043, 1044,
 1025, 1027, 1026, 1032, 1028, 1033, 1029, 1031, 1034, 1030,
 1035, 1036, 1037, 1039, 1040, 1038, 1041, 1042, 1046, 1045,
 1047,    0,    0,    0,    0,   31,   68,   66, 1009, 1018,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  645,    0,    0,    0,  692, 1011,    0,    0,
    0,    0,    0,  115, 1016,  571,    0,    2,    0,    0,
    0,    0,    0,    0,  431,  432,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
  447,  448,  449,  450,  451,  459,  453,  454,  452,  455,
  456,  457,  458,  460,    0,  429,  430,    0, 1001,    0,
  407, 1005,    0,  404, 1008,    0,  694,    0,  403, 1012,
    0,  408, 1004,    0,  578,  925,    0,  158,  111,    0,
  112,    0,  208,  997,  787,    0,  109,  153,    0,    0,
  105,    0,  113,  931,    0,  106,    0,  107,    0,  108,
 1015,  154,  114,  110,  737,  738,    0,  930,  928,    0,
   74,    0, 1002,    0, 1000,  495,  488,    0,  494,  499,
  489,  490,  497,  491,  492,  493,  498,  500,    0,    0,
    0,   29,    0,   38,  774,   37,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  935,    0,  916,  639,  639,  640,  650,  652,  988,  653,
    0,  654,  644,  640,    0,    0,    0,    0,    0,    0,
    0,  597,  586,    0,  589,  591,  592,  593,  594,  600,
  601, 1010,  693,    0,    0,  116,    0,  117,    0,  118,
    0,    0,    0,  624,    0,    0,  990,    0,    0,  631,
    0,    0,    0,    0,    0,  475,    0,    0,  370,    0,
  211,    0,  411,  401,  469,  470,    0,    0,    0,    0,
    0,    0,  420,    0,  376,  377,    0,    0,  584,  582,
  926,    0,    0,    0,    0,    0,    0,    0,  161,  140,
    0,  103,    0,    0,  119,    0,  129,  128,    0,    0,
    0,    0,    0,    0,  993,   75,    0,    0,    0,    0,
  752,  496,    0,    0,    0,    0,  893,  922,    0,  992,
    0,  895,    0,    0,  923,    0,  917,  908,  907,  906,
    0,    0,    0,    0,  898,  899,  900,  909,  910,  918,
  919,    0,  920,  921,    0,    0,    0,  924,    0,    0,
    0,    0,    0,    0,    0,  912,    0,  784,  890,  891,
  892,  897,  896,  875,    0,    0,    0,  874,    0,    0,
  879,  871,  872,  873,  876,  877,  878,  911,  936,  937,
  938,  950,  952,  960,  961,  965,  966,  209,    0,    0,
    0,  363,    0,    0,    0,  777,   26,    0,    0,    0,
    0,   71,   69, 1013,   67,   32,    0,    0,    0,  779,
    0,   47,   49,    0,    0,    0,  576,    0,    0,    0,
  647,  649,    0,  656,    0,    0,    0,  657,  643,    0,
  623,    0,  598,    0,  595,    0,  596,  621,    0,  616,
    0,    0,  590,  788,  758,    0,    0,    0,    0,    0,
    0,  573,  572,    0,  625,  629,    0,    0,  734,  636,
  635,    0,  176,    0,    0,  137,    0,  480,    0,  481,
  476,    0,    0,  415,  417,  416,    0,  418,    0,  412,
    0,    0,  262,    0,    0,    0,  424,  425,    0,  462,
    0,    0,    0,    0,    0,    0,    0,  381,  380,  483,
    0,    0,    0,  157,    0,    0,  166,    0,  165,    0,
    0,  172,  155,    0,  170,  162,    0,    0,  519,  554,
  522,    0,  555,    0,    0,  523,    0,    0,    0,  546,
  558,  518,  541,  520,  521,  516,  503,  370,  501,  502,
  504,    0,    0,  513,  514,  515,  517,    0,    0,    0,
    0,    0,  120,    0,  121,    0,  132,    0,    0,  192,
    0,    0,  368,    0,  211,    0,    0,   83,    0,  101,
    0,  100,  994,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  790,  792,  801,  804,  805,  806,  807,
  808,  809,  810,  811,  812,  813,  814,    0,    0,    0,
    0,    0,  913,    0,    0,    0,    0,    0,    0,  914,
  915,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  324, 1014,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  353,    0,    0,
  267,  511,    0,    0,    0,  361,    0,    0,    0,  776,
    0,    0,   43,   72,    0,    0,   25,    0,  782,    0,
    0,  748,    0,    0,    0,    0,  575,    0,  639,    0,
  661,    0,    0,    0,    0,  566,    0,  933,  565,    0,
  599,    0,  607,  603,  602,    0,    0,    0,  618,  606,
    0,    0,    0,  756,    0,    0,    0,  368,    0,    0,
    0,    0,    0,  138,    0,  479,  370,  414,    0,  370,
  371,  419,    0,    0,  258,  368,  231,  998,    0,    0,
  370,    0,    0,  200,  368,    0,    0,    0,    0,  236,
  230,  233,  234,  235,  423,  467,  466,  422,  421,    0,
    0,    0,    0,    0,    0,  379,    0,  370,  472,    0,
  471,    0,  163,  167,    0,    0,  171,    0,    0,  769,
    0,  542,  543,  559,    0,  556,    0,    0,  549,  547,
  548,    0,    0,    0,    0,  512,    0,    0,    0,    0,
  550,    0,    0,  368,    0,  191,    0,  194,  196,  197,
    0,    0,    0,    0,    0,  211,   78,    0,    0,   77,
    0,    0,    0,    0,  791,  803,    0,    0,  864,  865,
  866,    0,  795,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  714,  716,    0,  711,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  980,  981,  982,  983,  984,  985,  986,  987,
    0,    0,    0,    0,    0,    0,  971,    0,    0,    0,
    0,    0,  888,  355,    0,  889,  958,    0,  884,  996,
    0,    0,    0,    0,    0,    0,    0,    0,  325,  326,
    0,    0,  894,  786,  785,  365,  364,    0,  265,    0,
    0,    0,  273,  778,    0,    0,    0,   70,    0,    0,
    0,  781,    0,    0,    0,    0,  648,  651, 1006,  660,
    0,    0,    0,    0,  666,  667,  668,  669,  678,  679,
  680,    0,    0,  658,  567,  934,  608,  617,  612,  611,
    0,  622,  789,  759,    0,    0,  368,    0,    0,  641,
  736,  735,    0,  478,    0,    0,  175,    0,  179,    0,
    0,    0,  413,  368,  368,  270,  371,  263,    0,  202,
  370,  204,  370,  371,  199,  205,    0,    0,    0,    0,
  426,  427,  370,  368,  371,    0,    0,    0,  397,    0,
  473,  187,  188,  164,    0,  174,  530,  531,  932,    0,
    0,    0,    0,    0,  557,    0,    0,  371,    0,    0,
  507,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  189,    0,    0,    0,   82,    0,    0,
    0,    0,   90,   84,    0,    0,   89,   85,    0,  757,
  999,    0,    0,  796,    0,    0,  802,    0,    0,    0,
    0,    0,  840,  839,    0,    0,    0,  849,  867,    0,
  859,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  794,    0,  715,    0,  751,    0,    0,  211,    0,
  939,    0,    0,  953,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  967,    0,    0,    0,
    0,    0,    0,    0,  978,  633,  957,  901,    0,    0,
    0,    0,  260,  279,    0,    0,  360,  274,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   53,   61,   63,
    0,   41,    0,    0,    0,  763,  783,    0,  762,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  695,    0,
    0,    0,    0,    0,    0,    0,  614,  615,  619,    0,
    0,  190,    0,    0,    0,    0,    0,  180,    0,    0,
    0,  409,  214,  215,  216,  217,  218,  219,  904,    0,
    0,  264,    0,    0,    0,    0,    0,    0,    0,  237,
    0,  463,  464,  428,    0,    0,    0,    0,    0,    0,
  370,  485,  371,  173,  995,    0,    0,  525,    0,  524,
  560,    0,  552,    0,    0,    0,  505,  533,    0,    0,
  535,  538,  540,  368,    0,    0,    0,  127,    0,  195,
  370,    0,  369,  104,    0,  125,    0,   93,    0,    0,
    0,    0,   87,   88,    0,  857,  858,  798,    0,  800,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  860,
    0,    0,    0,    0,    0,    0,    0,  850,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  722,    0,    0,    0,  945,  944,    0,
    0,  941,  940,    0,  949,  948,  947,  946,  943,  942,
    0,    0,  973,    0,    0,    0,  972,    0,    0,    0,
    0,  358,  328,  327,  266,    0,    0,  276,  277,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  352,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  308,  309,  310,  311,    0,    0,  346,
  347,  348,    0,  761,   65,   57,   58,   64,   59,   60,
    0,   23,    0,    0,   24,  768,  767,    0,  765,  773,
  772,  770,  747,   44,   45,    0,  670,    0,    0,    0,
  700,  698,  696,    0,  697,    0,    0,    0,    0,    0,
  755,    0,  369,  740,  741,  642,    0,  185,  181,    0,
  371,  905,  268,  269,  371,    0,  142,    0,    0,  222,
    0,    0,  238,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  482,  527,    0,  528,    0,    0,    0,    0,
  146,  148,  149,  150,  510,  508,  506,  537,    0,    0,
  371,    0,    0,    0,  366,  370,   91,    0,    0,   97,
   92,    0,    0,    0,    0,    0,  868,    0,  869,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  724,
  713,    0,    0,  725,    0,  721,  210,  959,  979,  951,
    0,    0,  969,  968,    0,  974,  962,  964,    0,    0,
  275,    0,    0,    0,  307,    0,    0,  306,  334, 1007,
  349,  350,    0,  295,  293,  297,    0,  294,  286,    0,
  291,    0,  337,  292,  296,  298,  299,  300,  301,  302,
  303,  304,  305,    0,    0,  330,    0,   54,   55,   56,
   62,    0,    0,  690,  688,    0,  681,    0,    0,    0,
  704,    0,  638,  371,  367,  742,  182,  371,  152,  257,
    0,    0,  198,  223,    0,    0,    0,    0,  224,  227,
    0,    0,  371,  395,  396,  392,  391,  394,  393,    0,
  526,  553,  151,  139,  147,    0,  370,  254,  134,  371,
  372,    0,    0,   95,    0,   79,    0,  826,  834,  632,
    0,  827,  835,  830,  838,    0,    0,  828,  836,  829,
  837,  823,  831,  824,  832,  825,  833,  720,    0,    0,
    0,  954,  970,    0,    0,    0,  282,  278,    0,    0,
    0,    0,  321,    0,    0,  345,    0,    0,    0,    0,
  340,  338,    0,    0,    0,   39,  764,    0,  672,  682,
    0,    0,  683,  684,  685,    0,  255,  410,  201,  206,
  225,    0,    0,  228,    0,  240,  242,  399,  371,    0,
  374,    0,   76,   98,   99,    0,    0,    0,  727,  963,
  212,  281,    0,    0,    0,    0,  344,  343, 1003,  342,
    0,    0,    0,    0,    0,    0,  671,    0,    0,  705,
  229,    0,    0,    0,    0,    0,  256,  371,    0,    0,
  726,    0,    0,    0,    0,    0,  339,  341,    0,    0,
    0,  676,    0,    0,    0,    0,    0,  239,    0,  246,
    0,  245,  375,  126,  729,    0,  283,    0,    0,    0,
    0,    0,    0,  674,  687,  226,    0,    0,  250,  249,
  248,  730,    0,    0,    0,    0,  329,  677,  253,  251,
  252,  319,    0,  335,  316,  317,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1464,   28,   29, 1465,
 1466,   32,   33,   34,   35,   36,   37, 1467,   39, 1468,
  870,  142,  508,  167, 1255, 1502,  143,  762,  144,  511,
  767,  145,  146,  294, 1032,  156,  157, 1256, 1257, 1258,
 1259,  228,  259,  147, 1260,  148,  513,   87,  271,  416,
  465,  676, 1141,  466,  909,  647,  252,  886,  677,  678,
  679, 1148, 1143, 1591,  272,  681,  682,   79,  218,   80,
  267, 1816, 1364,  251,  256,  258,  260,  247,  239,  241,
  253,  263,  174,  346,  348,  350,  405,  663,  665,  505,
  295,  830,  583,  692, 1073,  409,  668, 1132, 1358,  615,
  788, 1104,  575,  576,  596,  401,  648, 1084, 1569, 1703,
 1302, 1570, 1571, 1572, 1573, 1574,  693,  248,  240,  397,
  623,  398,  399,  624,  625, 1334,  577, 1067, 1068, 1069,
 1297,  411,  897,  898,  899,  900,  832, 1548,  649,  834,
  468,  902,  989,  499, 1421, 1767, 1303,  469,  470,  471,
  472,  473, 1549, 1550,  599, 1709, 1844, 1320,  840,  841,
  827,  842,  843,  844, 1845, 1846, 1890,  378,  504, 1011,
 1469,  224,  592, 1008,  593, 1077,  500, 1012, 1013, 1245,
 1447, 1448, 1449, 1768, 1150, 1470, 1471, 1472, 1473, 1662,
 1474, 1475, 1476, 1477, 1478, 1479, 1480, 1481, 1482, 1483,
 1484, 1485, 1486, 1487, 1488,  998, 1881, 1773,  474,  737,
  475, 1489, 1663, 1780, 1781, 1828, 1490, 1491, 1492, 1653,
  476,  748,  477,  291,  501,  502,  478,   41, 1363, 1585,
  387,  610,  858, 1332,  611,  856,   84,   63,  382,  229,
  184,  365,  215,  372,  221,  232,  821,  818,  373,  383,
  848,  384,  606, 1095,  216,  479, 1119,  366,  612, 1333,
  102,  227,  276,  650,  651,  752, 1120,  753, 1121,  654,
  655,  656,  657, 1507, 1113, 1111,  658,  659,  660,  879,
  661,  876,   42,   43,   44,   45,   46,   47,  176,  563,
   65,  528,  235,  390,  333,  334,  335,  336,  337,  338,
  339,  340,  341,  794,  549, 1051, 1289,  550,  551,  799,
  180,  354,  565,   48,  183,  360, 1387,  736,   49,  358,
   50,  161,  321,  536,  784,  420, 1286, 1530,  709,  531,
  323, 1294, 1696,  162,  163,  322,  537,  538,  782, 1030,
 1033, 1034, 1035, 1036, 1037, 1038, 1788, 1837,  480, 1863,
 1039, 1040, 1041, 1284, 1793, 1794, 1795, 1276,  170, 1522,
 1690, 1691, 1414,  947, 1415, 1416, 1417, 1626, 1817, 1819,
  568,  569, 1061, 1536,  519,   51,   52,  421,   53,   54,
  557,  558,  481, 1020, 1266, 1508, 1512,  520,  770,  245,
  694,  695,  924,  925,  696,  697,  698,  699,  700,  701,
  702,  703,  704,  705,  706,  707, 1166, 1168, 1169,  482,
  483,  484,  485,  486,  487,  488,  269,  489,  490,  491,
  492,  971,  493, 1632,  494,  495,  496,  497,  725,  975,
 1230, 1225,  726,  150,
};
static short yysindex[] = {                                   5244,
 1377, -120, 1488,  182,   39,   22, 2824,  -63,10868,  650,
  233,  -26, -120, 1096,  -30,12910, 1247,12910, -116,    0,
  594,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  401,    0,    0,    0,    0,    0,    0,  -49,    0,    0,
    0,    0,    0,    0,    0,16334,12910,12910,12910,12910,
12910,12910,    0,    0,  380, 1218,12910,12910,12910,  542,
12910,  509,12910,12910,    0,12910,12910,12910,    0,  846,
    0,  564,    0,    0,  718,12910,    0,12910,12910,12910,
  746,12910,12910,12910,12910,  509,12910,12910,12910,12910,
12910,    0,12910, 1128,  712,    0,    0,    0, 1149,    0,
 1149,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  768,  824,  220, 1170,    0,    0,    0,    0,    0,
12910,12910,12910,  900,  919, 2542,12910,  926,   93,  110,
  843,  917,    0,12910, 1033,  999,    0,    0,12910,  982,
12910,12910,12910,    0,    0,    0,12910,    0,  986,  981,
 1165,  953,  934, 1354,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  819,    0,    0, 1218,    0,   49,
    0,    0, 1414,    0,    0, 1268,    0,   89,    0,    0,
  941,    0,    0, 1073,    0,    0, 1068,    0,    0, 1204,
    0, 1248,    0,    0,    0, 1218,    0,    0,  404, 1414,
    0,  472,    0,    0, 1245,    0, 1224,    0, 1149,    0,
    0,    0,    0,    0,    0,    0, 1256,    0,    0, 1141,
    0, 1326,    0, 1182,    0,    0,    0, 1141,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,14766,12910,
 1237,    0,12910,    0,    0,    0, 1250, 1304,12411,12910,
 1201, 1364, 1674, 1402, 1408, 1416, 1373, 1383,  276, 1255,
    0,15004,    0,    0,    0,    0,    0,    0,    0,    0,
13036,    0,    0,    0, 1333,12910, 1410, 1338, 1100,12910,
 1372,    0,    0,  999,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12910, 1414,    0, 1245,    0, 1149,    0,
 1423, 1433, 1316,    0,15004, 1405,    0, 1405,15004,    0,
 -111, 1307, 1312, 1311, 1354,    0, 1556,  784,    0,12910,
    0,  819,    0,    0,    0,    0,12910, 1237, 9874, 1394,
11391, 1817,    0,16334,    0,    0,  966, 1420,    0,    0,
    0, 1808, 1808, 1218, 1505, 1218,  -34, 1204,    0,    0,
 9240,    0,12910,12910,    0, 1237,    0,    0, 1589,  637,
 1836,12910, 1613,12910,    0,    0, 1838, 9611, 9508, 1491,
    0,    0, 1839, 1841, 1842, 1432,    0,    0, 1848,    0,
 1849,    0, 1850, 1852,    0, 1853,    0,    0,    0,    0,
    0, 1857, 1445, 1447,    0,    0,    0,    0,    0,    0,
    0, 1867,    0,    0,13290, 1869, 1870,    0, 1871, 1553,
15004,15004,13528,12910, 1872,    0, 2835,    0,    0,    0,
    0,    0,    0,    0, 1825, 1518, 1881,    0,    0, 1880,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9240, 1873,
 1115,    0, 1888, 1666, 1290,    0,    0,12910,12910,12910,
  138,    0,    0,    0,    0,    0,12910,12910,  -98,    0,
12910,    0,    0, 1250, 1536,12411,    0,12910,13528, 1945,
    0,    0,15004,    0, 1811, 1616, 1892,    0,    0,   72,
    0,  842,    0,  249,    0,  249,    0,    0, 1895,    0,
  199, 1520,    0,    0,    0, 1897,   87, 1883, 1237, 1901,
 1690,    0,    0,12910,    0,    0, 2044, 1909,    0,    0,
    0, 1297,    0, 1615, 1617,    0, 1218,    0, 1618,    0,
    0, 1684, 1687,    0,    0,    0, 1669,    0, 9240,    0,
 9240, 1308,    0, 1705,12910,16167,    0,    0,  495,    0,
12910,  537, 1268, 1581, 1547, 1514,    0,    0,    0,    0,
  -21, 1590,  157,    0, 1533,  509,    0, 1218,    0, 1709,
 1638,    0,    0,  -34,    0,    0,  248, 1550,    0,    0,
    0, 1552,    0, 1606, 1962,    0, 1659, 1968,   21,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1920,  167,    0,    0,    0,    0,   82, 1972, 1973,
 1968, 1414,    0, 1699,    0, 1767,    0, 1218, 1218,    0,
 9874,   89,    0, 1662,    0, 1632, 1993,    0, 5611,    0,
 1664,    0,    0, 1668, 2001, 9727, 2013,    0,    0,    0,
 9244,10739,   33,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  669, 1675, 1046,
15004,  607,    0,12910,15004, 1051, 1166, 1176, 1707,    0,
    0,15004, 9990, 2835, 1619,  671,15004,15004, 9990, 1741,
 1314, 1314, 1954, 1331,  -49, 2030,    0,    0,13782,12910,
15004,15004,15004,15004,15004,15004, 2289,    0,15004, 6727,
    0,    0, 1798, 2014,12910,    0,12910, 1780,12910,    0,
 1650, 1652,    0,    0, 1626,12411,    0, 1769,    0, 1532,
 2045,    0, 1769,12910,12910, 2040,    0, 1954,    0, 2016,
    0,12910, 4729, 1651,15004,    0,  805,    0,    0,   28,
    0, 1657,    0,    0,    0,12910,  662,12910,    0,    0,
12910,12910, 1491,    0,15004, 1828, 9874,    0, 2054, 1608,
15004,15004, 1218,    0,  340,    0,    0,    0, 2059,    0,
    0,    0,  221,12910,    0,    0,    0,    0, 1830, 2064,
    0, 1814, 1827,    0,    0, 1772, 1783, 1149, 1846,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  509,
12910,16265, 1748, 1749, 1750,    0,  509,    0,    0,  871,
    0, 1310,    0,    0, 1721, 1218,    0, 9611,  178,    0,
 1729,    0,    0,    0,  509,    0, 1962,  186,    0,    0,
    0, 1814,  186, 1730,  186,    0, 2098,  509,  509,  509,
    0, 1237,12910,    0, 1787,    0, 1562,    0,    0,    0,
  966, 1405,12910, 1592,  117,    0,    0, 2101,  437,    0,
 9611,12910, 1405,10209,    0,    0, 1405, 9727,    0,    0,
    0,10544,    0,  416,   53,15004,15004,10472, 2102,  878,
10733,15004,10994, 1044,11255,11516,14020,11777,12038,12299,
 9990, 9990,    0,    0, 1781,    0, 2107,  934,15004,15004,
 1904,15004, 2111,15004, 2109, 2103,15004,15004,15004,15004,
15004,15004,    0,    0,    0,    0,    0,    0,    0,    0,
 1840, 1640,  493,15004,  707,15004,    0, 9990, 2517, 2695,
  435,12910,    0,    0,  934,    0,    0, 1637,    0,    0,
 1879, 1314, 1314, 1286, 1286,  857, 9803, 2114,    0,    0,
 2115, 2835,    0,    0,    0,    0,    0, 1689,    0, 1722,
 1893, 1780,    0,    0,12174,12174, 1788,    0,  -81, 1809,
12910,    0,15004, 1809, 1845, 1863,    0,    0,    0,    0,
 6958, 2143, 2140, 1229,    0,    0,    0,    0,    0,    0,
    0, 9990, 1860,    0,    0,    0,    0,    0,    0,    0,
  -55,    0,    0,    0, 1675, 2835,    0, 1701, 1405,    0,
    0,    0, 2835,    0, 1808, 1921,    0,  340,    0, 2150,
 9990, 5347,    0,    0,    0,    0,    0,    0, 1780,    0,
    0,    0,    0,    0,    0,    0, 1149, 1149, 1700, 2152,
    0,    0,    0,    0,    0,    2,  -11,  470,    0, 1932,
    0,    0,    0,    0,12910,    0,    0,    0,    0,  182,
 1703, 1785, 1798, 2157,    0, 1533, 1719,    0, 2144,  875,
    0,12910, 1081,  509, 2160, 2166, 2168, 1947,   89, 1780,
 1808, 1617, 9874,    0, 1590, 1766, 4429,    0,  509, 1826,
 1908, 5611,    0,    0,  509,  482,    0,    0, 1914,    0,
    0, 2182, 2188,    0, 9990, 9990,    0, 9990, 9990, 2025,
 2835, 2193,    0,    0, 2835, 2194,13528,    0,    0, 1918,
    0,   29, 2197, 2835, 2205, 1512, 2206, 2835, 2207,15004,
15004, 2102,15004,14274, 2208, 2835, 2215, 2218, 2835, 2220,
15004, 2835, 2225, 2835, 2232, 2235, 2835, 2236, 2237, 2835,
 2240,    0, 2021,    0,13203,    0, 2163, 2174,    0, 2183,
    0, 2209,15004,    0, 2373, 2391, 2426, 2561, 2579, 2591,
15004,15004,15004, 2835, 1851,15004,    0,15004, 2835, 1979,
  507,15004,15004,15004,    0,    0,    0,    0, 1752, 9803,
 9321,12910,    0,    0,12910,15976,    0,    0,12910,12910,
12910,12910,12910,12910, 1843, 2241, 2242,    0,    0,    0,
 1843,    0,  185,  185,  104,    0,    0, 1765,    0,12174,
12174, 2247, 1229, 2249,15004, 2028, 4729, 1919,    0, 1919,
  666, 1919, 1956, 1966, 2031, 1987,    0,    0,    0,  934,
 1780,    0, 1766,  761, 1533, 1922, 1533,    0, 9990,  417,
 1277,    0,    0,    0,    0,    0,    0,    0,    0, 5347,
 5347,    0, 1893,15004, 5347, 2033, 1916, 1700,  846,    0,
 9990,    0,    0,    0, 1941, 1946, 1948, 1949, 1951, 1953,
    0,    0,    0,    0,    0, 2292,  182,    0, 1884,    0,
    0,  186,    0, 1963,  186,  186,    0,    0, 1798, 2299,
    0,    0,    0,    0,  966, 1893, 1533,    0, 1617,    0,
    0, 2068,    0,    0, 2054,    0, 2306,    0, 2307, 1218,
  -38, 2309,    0,    0, 1218,    0,    0,    0, 2021,    0,
 2021,15004, 1405, 1405, 1782,  -49, 2311,15004, 2037,    0,
 1405, 1405,15004, 1405, 1405, 2722, 2835,    0, 1728,15004,
 2835, 1405, 1405, 1405, 1405, 2835, 1405, 1405, 1405, 1405,
 1405, 1405,12910,    0, 2314, 2316, 1275,    0,    0, 9240,
 2320,    0,    0, 2635,    0,    0,    0,    0,    0,    0,
 2671, 1522,    0,15004, 2057, 1936,    0,15004, 2681, 1784,
 2710,    0,    0,    0,    0, 9240, 2308,    0,    0, 2076,
    0,12910, -109, 2310, 2328, 2034,15004, 2312,    0,12910,
12910,12910,    0, 2315, 2317, 2324, 2048, 2329,    0,    0,
    0, 2088,15838, 2337, 2338, 2341, 2343, 2347, 2349, 2350,
 2353, 2358, 2360,    0,    0,    0,    0,  410, 2363,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2099,    0,12174,12174,    0,    0,    0, 1994,    0,    0,
    0,    0,    0,    0,    0, 2028,    0, 1801,12910, 1229,
    0,    0,    0, 2085,    0, 4729, 4729,15004, 9990, 1491,
    0, 1893,    0,    0,    0,    0, 1310,    0,    0,  494,
    0,    0,    0,    0,    0, 2835,    0, 2148, 2033,    0,
  831,12910,    0, 2093,  813, 1996, 2105, 2007, 2129, 2118,
 2137, 1780,    0,    0, 2402,    0, 2408, 2113, 2148, 1963,
    0,    0,    0,    0,    0,    0,    0,    0, 1780, 1590,
    0, 1310, 1932, 2077,    0,    0,    0, 2092,  443,    0,
    0, 1798, 2094, 2835, 2421, 2425,    0,  934,    0, 2835,
15004, 2429, 2433, 2835, 2434, 2436,15004,15004, 2835, 2439,
 2440, 2443, 2444, 2446, 2447, 2448, 2451, 2452, 2453,    0,
    0,13203, 2457,    0,12910,    0,    0,    0,    0,    0,
15004, 2458,    0,    0,15004,    0,    0,    0,15004,  343,
    0, 2185,15004,15004,    0, 9990, 2835,    0,    0,    0,
    0,    0,12910,    0,    0,    0, 9803,    0,    0,   67,
    0, 2061,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   24, 2465,    0, 2130,    0,    0,    0,
    0,  185,12910,    0,    0, 4729,    0, 2634, 2835, 2463,
    0,   33,    0,    0,    0,    0,    0,    0,    0,    0,
 2469,12910,    0,    0, 2164, 2171,12910, 1700,    0,    0,
 1149,12910,    0,    0,    0,    0,    0,    0,    0, 1893,
    0,    0,    0,    0,    0, 1893,    0,    0,    0,    0,
    0,15004, 1218,    0, 2058,    0, 1218,    0,    0,    0,
 2835,    0,    0,    0,    0, 2835, 2835,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,12910, 2190,
 2835,    0,    0, 2804, 5347, 5347,    0,    0, 2481, 2835,
 1466,  833,    0, 2189, 2114,    0,12910,12910,  182,  164,
    0,    0, 2027, 2201, 9990,    0,    0, 2490,    0,    0,
 9990, 2498,    0,    0,    0,15004,    0,    0,    0,    0,
    0, 1700, 1149,    0, 2170,    0,    0,    0,    0, 1932,
    0, 2835,    0,    0,    0, 2495, 2499, 2502,    0,    0,
    0,    0, 1405, 9803, 2121, 9803,    0,    0,    0,    0,
15976,   67,15004, 9803,  859,12910,    0,   33,12910,    0,
    0, 2170,   16, 1700, 2173, 2175,    0,    0, 2509,12910,
    0,12910, 2511, 2114,15976, 2114,    0,    0, 1480,  229,
 2252,    0, 1803, 1807, 1700,  770,  770,    0, 2259,    0,
 2136,    0,    0,    0,    0, 2522,    0, 2268, 9803, 2283,
 2278,15976,12910,    0,    0,    0, 2125,  429,    0,    0,
    0,    0,15976,  195,12910,15976,    0,    0,    0,    0,
    0,    0,15976,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  216,  680,    0,    0,    0,    0,    0,    0,    0,
    0,11129,  119,12537,    0,    0,    0,    0,    0,    0,
 2581,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  253,    0,    0,    0,    0,    0,    0,  226,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  723,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2251,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   37, 2210,    0,    0,    0,   94,    0,
   94,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2229,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  951,    0,    0,
14512,12798,    0,    0,    0,  969,    0,    0,  215,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  912,
    0, 2176,  196,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   45,    0,    0,    0,    0,  838,    0,    0,
    0,    0,    0,  992,    0,    0,    0,    0,    0,   70,
    0,12034,    0,    0,    0,    0,    0,    0,    0,   45,
    0,  -29,    0,    0, 2567,    0,    0,    0, 2345,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   57,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  192,    0,
 2348,    0,    0,    0,    0,    0,13402,    0,    0,    0,
    0,    0,  914,    0,    0,    0,    0,    0,    0,13656,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  644,    0,    0, 1035,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   45,    0, 2567,    0, 2345,    0,
 1147,  266,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1299,    0, 2335,    0,    0,    0,
    0, 1330,    0,    0,    0,    0,    0, 2348,  496,16403,
    0, 1332,    0,    0,    0,    0,  791,    0,    0,    0,
    0, 2167, 2167,    0,    0,    0, 1363,  141,    0,    0,
    0,    0,    0,    0,    0, 2348,    0,    0, 1282,    0,
    0,    0,    0,    0,    0,    0, 8838,    0,    0,   55,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2074,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2854,    0,    0,    0, 3154, 3454,    0, 3754, 4054,
    0,    0,    0,    0,    0,    0,  641,    0,    0,    0,
    0,    0,    0,    0, 4354, 1371,  259,    0, 2554,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1381,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,13402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9393,    0, 2302,    0,    0,    0,
    0,    0,    0,  498,    0,  498,    0,    0,  767,    0,
  967, 1104,    0,    0,    0,    0,   57,    0, 2348,    0,
    0,    0,    0,    0,    0,    0,  130,  238,    0,    0,
    0,  487,    0,    0, 1005,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2335,    0,
    0, 1254,    0, 2161,    0,    0,  434,    0,    0,    0,
    0,  245,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1437,    0,    0, 3145, 1697,    0,    0,
    0,15521,    0,    0,15533,    0,    0,15570,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1318, 1341,    0,    0,    0,    0,15724, 7754,    0,
15570,   45,    0,    0,    0,    0,    0,    0,    0,    0,
  496, 1281,    0,    0,    0,    0, 2222,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7344, 7523, 7645,
    0,    0,  880,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1853,  113,15258,
    0,15258,    0,    0,    0,15258,15258,15258,    0,    0,
    0,    0,    0, 2184,    0,    0,    0,    0,    0,    0,
 5543, 5832, 2584,    0,   62,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1891,    0,    0,    0,    0, 2359,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  269,    0,    0,
    0,    0,  269,    0,    0, 1444,    0,    0,    0,    0,
    0,    0,    0, 1059,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  488,    0,    0,    0,
    0,    0,   55,    0,    0,    0,  496,    0,  673,  154,
    0,    0,    0,    0,  159,    0,    0,    0,    0,    0,
    0,    0, 1816,    0,    0,    0,    0,    0, 2590,    0,
    0, 1661, 1965,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  797,  882,  905,    0,    0,    0,    0,    0,
    0,  349,    0,    0,    0,    0,    0,    0,    0,    0,
 2238,    0,    0,    0,    0,    0,15533,    0,    0,    0,
    0,  762,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2348,    0,    0,  201,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11773,    0,    0,    0,  261,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1852,    0,    0, 1455,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2594,    0,    0,    0,    0,    0,
 4654, 6121, 6410, 4954, 5254,    0,    0, 1456,    0,    0,
    0,  686,    0,    0,    0,    0,    0,    0,    0,13894,
    0, 2369,    0,    0,    0,    0,    0,    0,    0, 1469,
    0,    0,    0, 1469,    0,    0,    0,    0,    0,    0,
    0, 6619,  510,  153,    0,    0,    0,    0,    0,    0,
    0,    0, 1098,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  113,  170,    0,    0,    0,    0,
    0,    0,  611,    0, 2167, 2191,    0,  690,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2359,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1587,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1474,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3445,    0,    0,    0,    0,    0, 1226,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1281, 2359,
 2167, 1483,  496,    0, 2376, 1490,    0,    0,    0,11773,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  989,    0,    0,    0, 1588,    0,    0,    0,    0,    0,
    0,    0,    0, 1864,    0, 6328,    0, 7728,    0,    0,
    0,    0,    0,    0,    0, 7812,    0,    0, 7934,    0,
    0, 7964,    0, 8087,    0,    0, 8169,    0,    0, 8263,
    0,    0, 2331,    0, 1853,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2216,    0,    0,    0,    0,  774,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  485,    0,    0,    0,    0,
    0,    0,    0,  166, 1507,  175,  209,    0,    0,    0,
 1507,    0, 2214,  356,    0,    0,    0,    0,    0,    0,
    0,    0, 2305, 1132,    0, 7043,    0, 2313,    0, 2313,
 2305, 2313,    0,    0,    0,  145,    0,    0,    0, 1455,
 2359,    0, 1490,    0,    0, 2203,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1025,    0, 1587, 2251,    0,
    0,    0,    0,    0,  906,  913,  957,  970, 1024, 1072,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  724,    0,    0,    0,    0, 2110,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1516,    0,
    0,    0,    0,    0, 1561,    0,    0,    0,    0,    0,
  165,    0,    0,    0,    0,    0,    0,    0,  128,    0,
  144,    0,    0,    0,    0,   62,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 8345,    0, 8434,    0,
 8468,    0,    0,    0,    0, 8528,    0,    0,    0,    0,
    0,    0, 1336,    0,    0, 2610,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2613,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
12847, 2598,    0,    0,    0,    0,    0,    0, 1663, 1706,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  517,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1605,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6834,    0,    0,    0,  610,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  191,
    0,    0,    0,    0,    0,    0,  879,    0,    0,    0,
    0,    0,    0,    0,    0, 2618,    0, 1648, 2065,    0,
 2335,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2359,    0,    0,    0,    0,    0,    0, 1578, 1129,
    0,    0,    0,    0,    0,    0,    0,    0, 2359, 2376,
    0,  265,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  390,    0, 8610,    0,    0,    0, 2594,    0, 8644,
    0,    0,    0, 8704,    0,    0,    0,    0, 8824,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1853,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2602,
    0,    0, 2604,    0,    0,    0, 2605,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2364,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  356, 6661,    0,    0,    0,    0, 2305,  944, 1152,
    0,  572,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2660,    0,    0,
  140,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  176,    0,    0,    0,    0,    0,
 9003,    0,    0,    0,    0, 9063, 9097,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1824,
 2624,    0,    0,    0,    0,    0,    0,    0,    0, 2615,
 2616,    0,    0,    0, 2620,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1092,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2660,  786,    0,  146,    0,    0,    0,    0,    0,
    0, 2618,    0,    0,    0, 2631,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  438,    0,    0,    0,    0,    0,    0, 9174,    0,    0,
    0,  647,    0, 1587, 1208, 1586,    0,    0,    0,    0,
    0,    0,    0, 2622,  438, 2625,    0,    0,    0, 2382,
    0,    0,    0,    0, 2660,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,15740,    0,    0,
    0,  438,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  438,    0,    0,  438,    0,    0,    0,    0,
    0,    0,  438,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2686,    0,    0, 2687,
 2688,    0,    0,    0,    0,    0,    0,    1,    0, 2689,
   -2, 2534, 2169,  -54, -776, 1431, 2538, 1927, 2539, 2179,
    0,    0, 2394, -107,  -32,    0,    0,    0,    0,  282,
  288,  -60,  -78,  -57, -269, 2406, 1943,    0,    0,    0,
 1075,    0,    0,  -53, 1573, -554,  -72, -989, 2042, 1821,
 -339,    0, 1591,    0, 2638, 1832, 1877,    0,  -65,    0,
 -531,  -48,    0,    0,    0,    0,    0, 2654,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -556,
   73, 1598, -329, 1090, -677,    0,    0,    0,    0, -356,
 -142,-1266, 1624, -368,  -51,    0, -555, 1868,    0, 1192,
-1091,    0, 1196,    0,    0, 2395, -619,  -27,    0,    0,
    0,    0, 2370,    0, 2154, 1654,    0,    0,    0, 1704,
    0, 2427, 1975, 1646, 2409, 2416,    0,    0, -468, 1952,
  -44, -647, 1085, -276,    0,    0,    0, -960, -946, -937,
 -925, -914,    0, 1251,-1036,    0,  954,-1264, 1259,    0,
 2200,    0,    0,    0,  960,  968,  945, -101, -270, -902,
 -950,    0,    0,    0, 1990,    0, -684,    0, 1799,    0,
    0,    0,    0,    0,-1200, -423,    0,    0,    0,    0,
 -224,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -963,    0,    0, -725,    0,
 -714, 1355, 1155,    0,  988,    0,    0,    0,    0,    0,
    0,    0, -358,    0,    0, 2066, -148, -441, 1530, 1293,
 -566, -750,-1024,-1280,    0,    0,    3,    0,    0,    0,
    0,    0, 2445,    0,    0,    0,    0,    0, 2455, 2227,
    0,    0,    0,    0,    0,   -9, -498, 2467,    0,    0,
    0,   -1,    0,    0,    0, -375, 1950, -373, 1482,    0,
    0,    0,    0, 2223,    0, -529,    0,    0,    0, 2172,
    0, 1957,    0,    0,    0,    0,    0,    0,  -45,    0,
 2828,    0,    0,    0,    0,    0, 2503,    0,    0,    0,
    0, 2300,    0, 2318,    0,    0,    0, 2067,    0,    0,
    0,    0,    0, -118, -653, -851, 1693,    0, 1294,    0,
    0,    0,    0,    0,    0, -108,    0,    0, -759, -215,
 1246,    0,    0,    0, 2698, 1066,    0, 2072,-1073, -663,
    0, -929, 1834, -805,    0,    0,    0,    0, -164,    0,
    0,    0,    0, 1585,    0,    0,    0,    0, 2779,  374,
    0, 1074, 2180,    0, 1252,    0, 1471,    0, -144,    0,
    0, 2078,    0,    0,    0,    0,    0, 2330,    0,    0,
    0, -425,-1174, 2119, 1878,-1053,    0,    0,    0, -674,
 -434,  272,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1328, 1712,    0,    0,
    0, -889,    0,    0,    0, 1602,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -308, 1676,    0,    0,
};
#define YYTABLESIZE 16923
static short yytable[] = {                                     149,
   38,   83,  246,  296,  343,  226,  168,  809,  175,  220,
  315,  318,  238,  257,  169,  242,  314,  317,  255,  262,
  286,  999,  243,  283,  277,  652,  223,  653,  622,  512,
  751,  589, 1000, 1239,  231,  810,  616,  285,  250,  587,
  831,  789,  287, 1055, 1450,  281,  217,  219,  222,  225,
  168,  230,  233, 1553,  712,  948,  710,  244,  219,  233,
 1247,  225,  282,  168,  230,  280,  261,  149,  233,  159,
  289, 1494, 1009,  288,  254, 1005,  273,  772,  168,  275,
  219,  985,  273,  233,  222,  225,  169,  168,  230,  261,
  149,  233,  769,  225,  349,  284, 1206,  303,  254,  532,
  591, 1273,  733,  973,  734,  901,  302,  594,  304,  981,
 1361, 1304,  733,  712,  861,  710,  787,  347,  581,  555,
  822,  888,  823, 1340,  910, 1305, 1053,  833,  159,  739,
  802,  351,  312, 1236, 1306,  666,  325,  775,  345,  775,
  160,  244,  168,  244,  708,  247, 1307,  149,  406,  316,
 1135,  413,  664,  743,  233,  313, 1140, 1308,  177,  342,
 1059,  225,  168,  261,  374, 1023,  313,  175,  797,  760,
  739,  733,  313,  739,   51, 1076, 1313,  581, 1079,  556,
  775,  766, 1309,  775,  799,  708,  247, 1086,  739,  247,
  712,  356,  402,  664,  743,  746,  664,  743,  775,  160,
  133,  860, 1519,  708,  247,   94,  814, 1832,   52, 1023,
 1509,  664,  743,  760,  694,  581,   96,  177, 1543, 1544,
  652, 1110,  653, 1547, 1023,  733,   82, 1356,  760,   82,
 1116,  712,  573,   51, 1589, 1263,  746,  732, 1240, 1261,
  764,  561,  798,  559,  398,  498, 1130,  313,  506,  712,
  356,  915,  626,  130,  746,  622,  512,  885,  354,  133,
 1651, 1652,  620,  300,  186,  628,  733,   52,  766,  839,
 1697,  999, 1240,  694,  581,  548,  853,  541,  732, 1551,
  244, 1075, 1000,  244,  733,  103, 1327,  869,  806,  514,
  149,  164,   82,  626,  941,  880,  732,  154, 1494,  554,
  362,   86, 1730,  398,   86,  375,  628,  544,  259,  573,
   14,  626,  854, 1866, 1158, 1729,  222,  354,  882,  300,
  168, 1202, 1203,  186,  628,  588,   64,  766, 1389, 1776,
  181,  103,  498, 1783,  243, 1325,  602,  771,   88,  604,
  617, 1287,  619,  664,  735,  385, 1264, 1520,  186, 1304,
 1304,   85,   86,  556, 1304,  532,  670,  672, 1231, 1577,
  244,  881, 1545, 1305, 1305,  674,  159,  244, 1305,  244,
  662,  244, 1306, 1306,  607, 1777, 1390, 1306,  376, 1117,
  988,  177,  182, 1093, 1307, 1307, 1778,  996, 1532, 1307,
 1644,  165, 1288,  225,  230, 1308, 1308,  574,  166, 1312,
 1308, 1290,  168, 1766,  222, 1581, 1316,  186,  683, 1291,
  735,  942,  855,  159,  532,  680,  177, 1324,  386, 1590,
 1309, 1309, 1328,  130,  130, 1309, 1310, 1311,  177,  155,
  534, 1159,   14,  534,  712, 1326,  710,  160, 1531,  362,
 1344,  884, 1683, 1804,  739,  710, 1323,  259,  803, 1867,
  104, 1300, 1774,  761,  738,  177, 1157, 1541,  546,  708,
 1136,  177,  768,  177, 1831,  739,  773,  664,  743,  775,
   94, 1152,  862,  554,  621, 1153,  763,  664, 1234,  664,
  708,   96,  777, 1154,  160, 1779,  744, 1070,  664,  743,
 1072,  699, 1880, 1514, 1515, 1903,  512,  609,  168,  225,
  514, 1082,  177, 1139,  153,  712,  797,  168,  398,  663,
  746,  168, 1551,  797, 1443,  775,  514, 1074,  175,  506,
  887,  311,  799,  815,  708, 1444,  712,  744, 1100, 1848,
  419,  746,  664,  708, 1698, 1378, 1379, 1841, 1380, 1381,
  733,  664,  311,  863,  133,  744,  581,  591,  786,  760,
  663,  835,  732, 1109,  244, 1727,  609, 1445,  760,  311,
  892,  733, 1355, 1027,  864, 1142, 1005,  712,  663,  710,
  712,  707,  735,  732,  733,  739,  708, 1054,  159, 1868,
 1365, 1694, 1001,  765,  664,  828,  244,  910,  626, 1272,
  708,  828,  526,  356,  694, 1267,  664, 1688,  664,  743,
 1886,  628, 1736,  694, 1580, 1006,  311,  868,  186,  665,
  745, 1023,  707,  760,  895,  896,  581, 1293,  254,  792,
   51, 1128,  712,  739,  694,  733,  243, 1109, 1787,  766,
  707,   81,  775,  859,   81,  311,  712,  556,  247, 1765,
  902,  746,  739,  604, 1046,  186,  247,  299,  953,  160,
  665,  745,  178,  665,   52, 1563, 1494,  708,  694, 1720,
  354,  244,  534,   86, 1268,  664,  743,  177,  665,  745,
  868,  733,  627, 1821, 1822, 1043, 1726, 1155,  941, 1540,
 1494,  902,  760,  732,  902,  903,  955,  247,  793,  178,
  247, 1065,  186, 1775, 1904,  534,  941,   81,  626,  902,
  186, 1555,  604,  712,  233,  247, 1579, 1494, 1295, 1146,
  498,  628,  498,  627, 1014,  179,  532, 1046, 1494, 1025,
 1687, 1494,  583,  144, 1674, 1900,  903,  694, 1494,  903,
  990,  627, 1598,  902,  186, 1511,  186,  534,  733,  407,
  244,  548,  763, 1052,  903,  244, 1740,  244,  178,  244,
  732, 1314,  331, 1315,  941,  941,  514,  554,  186, 1064,
 1066,  143,  243, 1322,  168,  225,  610, 1329,  941, 1808,
 1693,  232, 1029,  225, 1357, 1809, 1901, 1091,  903,  498,
  234,  583,  144,  186, 1147,  775,  168, 1554,  168,  289,
  484,  244,  534, 1734, 1156,  942,  384,  244,  403,  331,
 1142,  744,  249, 1330, 1304, 1304, 1092, 1094, 1385, 1566,
  836,  232, 1106,  942,  244, 1125, 1126, 1127, 1305, 1305,
  143,  287,  744,  997,  663,  610,  775, 1306, 1306,  775,
  408,  331, 1001,  404,  663,  609,  663,  378, 1129, 1307,
 1307,  244,  244, 1567,  775,  663, 1575,  534, 1137,  484,
 1308, 1308,  254, 1713, 1675,  384,  556,  186,  683, 1099,
 1854,  430, 1856, 1699, 1627, 1107,  319, 1700, 1701, 1420,
 1860,  942,  942, 1825, 1440, 1309, 1309, 1114,  186,  709,
 1790,  382,  331,  168,  613,  942,  707,  837,  232,  663,
  254,  254,  254,  168,  609,  997,  378,  609,  663, 1861,
  749,  683, 1151, 1728,  383,  389,  952,  707,  680, 1692,
 1089,  630,  390,  609, 1433, 1894, 1518, 1435, 1346, 1223,
  709,  151,  247,  247,  665,  745, 1735,  838,  232,  331,
  247,  999,  744, 1438,  665,  613,  665,  186,  709,  609,
  382,  663, 1000,  706,  123,  665,  745,  178,  699, 1238,
  579,  707,  630,  663, 1235,  663,  386,  916,   81,  178,
  707,  331,  923,  383,  389, 1373,  620, 1347,  588,  385,
  630,  390,  233, 1046,  976,  977,  554, 1640,  123, 1495,
  669,  604, 1498, 1278,  706,  152,  178,  706,  851,  665,
  814,  585,  178,  846,  178,  847, 1279, 1374,  665,  370,
  144, 1562,  706,  707,  477,  514,  514,  943,  627,  579,
 1226, 1227,  123,  268,  604,  386, 1797,  707,  247, 1280,
 1798,  225,  663,  388,  220,  620, 1772,  588,  385,  851,
  944, 1583,  851,  178,  587, 1807,  270,  143,  143,  247,
  604,  665,  902,  604,  652,  247,  653,  851, 1386, 1661,
  585, 1350, 1811,  665,  484,  665,  745, 1049,  710,  604,
  384,  775,  775,  477,  278,  220, 1368,  585,  220,  775,
 1282,  387, 1372, 1050,  941, 1001,  367,  977,  977,  247,
  586,  556,  388,  220,  707,  604,  945,  903,  243,  236,
  237,  675,  506,  587,  941, 1335,  378,  703,  999,  710,
  999,  378,  319,  605,  610,  292,  378, 1336,  999, 1000,
  265, 1000, 1335,  978, 1506, 1506, 1510,  710,  627, 1000,
  941,  368,  665,  244, 1346, 1633,  254,  244,  145, 1636,
  387, 1847,  675,  143, 1849,  675,  186,  610,  703,  247,
 1887,  254,  320,  297,  266,  382,  574,  254,  186, 1228,
  675,  702, 1537,  999, 1539,  756,  703,  775,  755, 1317,
 1318,  775,  605,  610, 1000, 1835,  610,  290,  383,  389,
 1873, 1838, 1862, 1349, 1499,  186,  390, 1170,  775, 1888,
  484,  186,  610,  186,  775,  916,  384,  145,  293,  916,
 1497,  942,  702, 1446,  709,  168, 1500,  498,  178,  298,
  243,  554, 1705, 1413,  709,  574,  709,  243,  610, 1366,
  702,  942,  356,  301, 1582,  709,  977, 1496,  775, 1898,
  386,  839,  186, 1706,  608, 1171, 1889,  378,  369, 1707,
  186,  244,  244,  385,  609, 1451, 1493,  942,  790,  514,
  225,  230,  514,  261,  149, 1534,  370,  630,  243, 1172,
  851,  243,  357,  468,  311, 1535, 1732,  331,  706,  709,
  514,  514,  477, 1708, 1386, 1386,  243,  225,  709,  509,
  706,  382, 1386, 1386,  477, 1386, 1386,  305,  775,  706,
  371, 1045, 1864, 1386, 1386, 1386, 1386,  388, 1386, 1386,
 1386, 1386, 1386, 1386,  383,  389,  306,  468,  405,  186,
  232,  220,  390,  851,  620,  319,  851,  477,  556,  477,
  319,  709,  468,  851, 1180,  851, 1588,  511,  509,  851,
  311, 1593,  780,  706,  851,  709, 1763,  851, 1181,  406,
  760,  402,  706,  759, 1565,  387,  327,  620,  744,  742,
  534,  743,  851,  745,  417,  949,  386, 1101,  825,  780,
  957,  824,  422,  310, 1685,  744,  851,  405,  511,  385,
  745,  511,  168,  620,  851,  326,  620,  851,  851,  328,
  359,  984, 1182,  710,  749,  706,  511,  851,  467,  544,
   42,  534,  620,  710,  534,  710, 1643,  186,  406,  706,
  402,  344,  709,  851,  710,  329,  232, 1810,  330,  534,
 1183,  530,  160,  168,  370,  145,  675, 1857,  620,  675,
  535, 1413,  703,  388,  331,  851,  675,  232,  675,  352,
  851,  168,  675,  232,  703,  319,  353,  675,  554,  359,
  675, 1878,  851,  703,  851,  319,  169,  355,  710,   42,
  332,  605,  219,   46,  567,  675,  359,  710,  572,  666,
 1650, 1151, 1151,  377,  746,  357,  706,  232, 1897,  675,
  388,  387,  666, 1493, 1184,  959,  702,  675,  771, 1902,
  666,  675, 1905,  486,  605,  961,  545,  703,  702, 1906,
  675,  851,  135,  243,  243,  666,  703,  702,  389,  373,
  710,  243, 1001,  514,  514,  169,  675, 1711,  106,  666,
  605,  851,   46,  605,  710,  851,   40,  744,  742,  342,
  743,  400,  745,  746,  357,  136,  225,  225, 1789,  605,
  391,  744,  742,  675,  743,  379,  745,  771,  380,  703,
  546,  702,  486, 1506,  108,  675,  666,  675,  412,  378,
  702,  135,  168,  703,  724,  605, 1278,  547,  373,  378,
  731,  732,  733,  744,  742,  392,  743,  410,  745, 1279,
  124,  533,  740,  744,  742,   40,  743,  741,  745,  539,
  381,  710, 1022,  740,  136, 1021, 1760,  207,  741,  243,
  159,  160, 1280,  702,  675,  244,  241,  818,  414,  393,
  740,  394,  415,  511,  511,  741, 1281,  702,  511,  109,
  243,  511, 1134, 1805,  675, 1133,  243, 1623,  675,  110,
  703,  361,  168,  395,  511,  222,  534,  534,  778,  124,
 1413,  534,  780,  362,  534,  419,  244,  241,  818,  244,
  241,  818, 1138, 1282,  111,  906,  207,  534,  396,  503,
  243,  171, 1624, 1151,  244,  241,  818,  207,  666,  570,
 1010,  571, 1803, 1523, 1625, 1525,  363,  418,  364, 1001,
  143, 1001, 1007, 1806,  702,  997,  556,   55,  723, 1001,
  236,  237,  172,  342, 1784,  507,  225, 1237,   56, 1813,
  749,  744,  742, 1815,  743,   57,  745,  173,  207,  511,
  556,  207,  990, 1102, 1103, 1842,  544,  168,  509,  517,
  243,  143,  222,  723,  143, 1875,  207, 1876, 1025,   58,
  511, 1827,  534,  518, 1001,  723,  511,  556,  521,  143,
 1025,  332,  522, 1025,  812,  438,  439,  440,  556, 1243,
  523,  556, 1242,  534,  131,  131,  544,  544,  556,  534,
  544, 1292,  740, 1338, 1133, 1283, 1337,  741,  524,  222,
  511, 1030,  527,  448,  449,  544,  740,  544,  525, 1343,
  540,  741, 1342, 1030,  333,  542, 1030,  219, 1829,  744,
  742,   59,  743,  534,  745,  950, 1830,  954,   66,  543,
  922,  958,  960,  962, 1678, 1680,  554,  544,  740,   67,
 1679, 1681, 1442,  741,  506, 1240,   68,  552,  740,  562,
  951, 1824,   60,  741,  956, 1513,   61,  564,  749,   69,
  554,  972,  566,   14,  578, 1879,  979,  980,  580, 1393,
  579, 1493, 1597, 1853, 1638,  749, 1029,  749,  467,  244,
  991,  992,  993,  994,  995,  467, 1631,  554, 1002,  582,
  222, 1684,  222, 1884,  749, 1493, 1883, 1885,  554,  818,
  759,  554,  744,  742,  600,  743,  368,  745,  554,  368,
  603,  244,  244,  819,  731,  613,   70,  731,  614,  244,
  618,  667, 1493, 1029,  535,  671,  673,  675,  710,  708,
  711,  712,   71, 1493,  713, 1151, 1493,  714,  715,  716,
  534,  717,  718, 1493, 1056,   62,  719,  720,   72,  721,
  567, 1063,  818,  730,  819,  818,  722,  819,  727,  728,
  729,  739,  818,   73,  818,  746,  740,   74,  818,  747,
   75,  741,  819,  818,  749,  750,  818,  757,   76,  758,
  775,  534,  783,  754,  534,  785,  143,  143,  796,  800,
  807,  818,  801,  805,  143,  744,  742,   77,  743,  534,
  745,  534,  811,  808,  544,  818, 1222,  244,  813,  817,
  573,  816,  819,  818,  501,  820,  818,  818,  826,  544,
  850,  544,  544,  544,  851,  852,  818,  544,  244,  857,
  544,  866,  311,  865,  244,  779,  744,  742,  872,  743,
  873,  745,  818,  544,  983,  744,  742,  874,  743,  544,
  745,  875,  544,  877,  740,  501,   78,  878,  501,  741,
  883,  889,  890,  893,  818, 1160, 1161, 1165,  244,  818,
 1174, 1176, 1178,  501, 1186, 1189, 1192, 1194, 1197, 1200,
  894,  818,  143,  818,  905, 1608,  906,  903, 1207, 1208,
  913, 1210,  912, 1212,  544,  911, 1215, 1216, 1217, 1218,
 1219, 1220,  917,  143,  181,  982, 1028,  744,  742,  143,
  743,  974,  745, 1224,  221, 1229,  744,  742,  544,  743,
  986,  745,  884, 1017,  781, 1010,  313, 1015,  244, 1016,
  818, 1017, 1019,  766, 1023,  744,  742,  740,  743,  544,
  745, 1057,  741,  143, 1042,  544, 1047,  759, 1071,  544,
  818, 1060, 1080, 1081,  818,  221,  544,  829,  221,  534,
 1083, 1087,  467,  993, 1017, 1017, 1017, 1017, 1017, 1017,
 1017, 1090, 1088,  221,  544,  819, 1096, 1097, 1098,  544,
 1105, 1017, 1017, 1017, 1017, 1017, 1112, 1124, 1131, 1122,
 1145, 1167, 1204, 1214,  744,  742, 1205,  743,  534,  745,
  534, 1211, 1213,  534,  544,  740, 1221, 1240,  544, 1244,
 1241, 1270, 1246,  534, 1017, 1262, 1017, 1209,  534, 1265,
  963,  964,  965,  966,  967,  968,  969,  970,  819, 1271,
  740,  819, 1275, 1277, 1285,  741, 1296,  534,  819, 1299,
  819, 1321, 1319,  534,  819, 1331,  534, 1341, 1339,  819,
 1351, 1345,  819, 1418,  744,  742, 1352,  743, 1353,  745,
 1354, 1362, 1139, 1370, 1419,  744,  742,  819,  743, 1375,
  745,  740, 1376, 1422,  744,  742,  741,  743, 1377,  745,
  740,  819, 1383, 1384, 1388,  741, 1391,  529,  534,  819,
  501,  501,  819,  819, 1392, 1394, 1395, 1402,  501, 1423,
  744,  742,  819,  743, 1403,  745,  467, 1404, 1175, 1405,
 1179,  501, 1187, 1190, 1407, 1195, 1198, 1201,  819, 1396,
 1397, 1408, 1399, 1401, 1409, 1410, 1411, 1434,  529, 1412,
 1406,  529,  941, 1437, 1503, 1504, 1382, 1516, 1501, 1517,
  819,  781,  740,  534, 1526,  819,  529,  741,  529, 1521,
  534,  740, 1424, 1528, 1527, 1538,  741,  819,  265,  819,
 1431, 1432, 1229, 1529, 1552, 1229,  595, 1224,  534, 1556,
  740, 1439,  467, 1441, 1557,  741, 1558, 1559,  997, 1560,
  793, 1561, 1564, 1109, 1568, 1017,  501, 1017, 1017, 1578,
  232,  221,  266, 1584, 1017, 1586,  464, 1587,  534, 1592,
 1017, 1599,  534, 1601, 1621, 1017,  819,  501, 1017, 1622,
 1628, 1634, 1635,  501,  467, 1642, 1641, 1646, 1645,  152,
 1648,  793, 1017, 1654,  793, 1655,  819, 1017, 1017,  740,
  819, 1017, 1656, 1657,  741,  534,  534, 1658, 1017,  793,
 1017, 1017, 1659,  534, 1017, 1664, 1665,  501, 1017, 1666,
 1017, 1667, 1017, 1546, 1017, 1668,  534, 1669, 1670, 1017,
 1017, 1671, 1017, 1425,  744,  742, 1672,  743, 1673,  745,
 1676, 1682, 1677, 1686, 1702, 1712, 1717, 1017, 1017, 1714,
 1017, 1426,  744,  742, 1719,  743,  232,  745, 1715,  740,
 1716, 1017, 1721, 1017,  741, 1017, 1017, 1017, 1722, 1017,
  740, 1718, 1017, 1017, 1731,  741, 1733,  232, 1737,  740,
 1723, 1738, 1017,  232,  741, 1739, 1427,  744,  742, 1742,
  743, 1594,  745, 1743, 1744, 1017, 1745, 1600, 1017, 1748,
 1749,  534, 1604, 1750, 1751,  740, 1752, 1753, 1754, 1609,
  741, 1755, 1756, 1757, 1017,  529, 1759,  232, 1762, 1769,
 1017, 1017,  534, 1660, 1785, 1017, 1796, 1786,  534, 1799,
 1802, 1801,  529,  529,  529, 1814, 1017, 1017,  529, 1017,
 1823,  529, 1818, 1229, 1826, 1017, 1833, 1229, 1017, 1836,
 1017, 1017, 1017, 1017,  529, 1017, 1834, 1839, 1850, 1851,
  529, 1852,  534, 1017, 1017, 1843, 1647, 1855, 1869, 1874,
 1871, 1877, 1882,  997, 1017, 1017, 1866, 1017,  744,  742,
 1232,  743, 1892,  745, 1017, 1017, 1017, 1017, 1017, 1867,
 1017, 1893, 1895, 1017, 1017, 1017, 1017, 1017, 1896, 1899,
    1, 1017, 1017,  739,  991,   28, 1017, 1017, 1017, 1017,
 1017, 1017, 1017, 1017,  997,  997,  997,  997,  997, 1010,
  997, 1428,  744,  742,   30,  743,  193,  745,  775,  529,
  370,  261,  997,  997,  997,  997,  156, 1689,  655, 1429,
  744,  742,  465,  743,   81,  745,  976,  902,  272,  203,
  529, 1430,  744,  742,  746,  743,  529,  745,  271,  398,
  183,  532,  975,  699,  997,  793,  997,  529,  793,  740,
  719,  701,  184,  956,  741,  793,  314,  793,  371,  241,
  213,  793,  312,  322,  955,  529,  793,  740,  288,  793,
  529,  728,  741,  313,  315, 1629,  744,  742,  323,  743,
  318,  745,  336,  351,  793,   27,   30,   31,   40,  307,
 1741, 1505,  774,  308,  516,  309, 1746, 1747,  793,  529,
  241, 1026,  740,  241,  776,  515,  793,  741, 1018,  793,
  793, 1630,  744,  742, 1371,  743,  904,  745,  241,  793,
 1761, 1637,  744,  742, 1229,  743, 1144,  745, 1764,  279,
 1369,  264, 1770, 1771, 1367,  793,  744,  742, 1233,  743,
  112,  745, 1149,  113, 1108,  114,  115,  116,  117, 1118,
  118,  744,  742, 1639,  743, 1359,  745,  793,  119,  120,
 1724,  584,  793,  744,  742, 1725,  743,  626,  745,  121,
  122, 1298,  123,  560,  793, 1348,  793,  867, 1360,  124,
  125, 1058,  126,  127, 1085,  128, 1800,  597,  129,  130,
  131,  132,  133,  740,  598, 1865,  134,  135,  741, 1704,
  845,  105,  136,  137,  138, 1872,  139,  140,  141, 1710,
 1248, 1891, 1870, 1078, 1649,  997, 1782,  997,  997, 1858,
 1007, 1812, 1533,  793,  997, 1695,  590, 1576,  605,  849,
  997,  581,  891, 1115, 1123,  997,  553,  740,  997,  106,
  158,  791,  741,  793, 1820,  744,  742,  793,  743,  871,
  745,  107,  997, 1027,  997,  740, 1044,  997,  997,  324,
  741,  997, 1048,  795, 1274, 1524,  274,  740,  997, 1840,
  997,  997,  741, 1758,  997,  108,  744,  742,  997,  743,
  997,  745,  997, 1620,  997, 1689,  804,  946, 1062,  997,
  997, 1024,  997, 1398, 1027, 1027, 1027, 1027, 1027, 1027,
 1027, 1269, 1542, 1436,    0,    0,    0,  997,  997,    0,
  997,  740, 1027, 1027, 1027, 1027,  741,    0,    0,    0,
    0,  997, 1859,  997,    0,  997,  997,  997,    0,  997,
    0,    0,  997,  997,    0,  241,  241,    0,    0,    0,
  109,    0,  997,  241, 1027,    0, 1027,  740,    0,    0,
  110, 1278,  741,    0,    0,  997,    0,  740,  997,    0,
    0,    0,  741,    0, 1279,    0,    0,    0,    0,    0,
    0,  740,    0,    0,  997,  111,  741,    0,    0,    0,
  997,  997,    0, 1607,    0,  997,  740, 1280,    0,    0,
    0,  741,    0,    0,    0,    0,  997,  997,  740,  997,
    0, 1281,    0,  741,    0,  997,    0,    0,  997, 1791,
  997,  997,  997,  997,    0,  997,    0,    0,    0,    0,
    0,    0,    0,  997,  997,    0,    0,    0,    0,    0,
    0,  241,    0,    0,  997,  997,    0,  997, 1282,    0,
    0,    0,    0,    0,  997,  997,  997,  997,  997,    0,
  997,    0,  241,  997,  997,  997,  997,  997,  241,    0,
    0,  997,  997,    0,    0,    0,  997,  997,  997,  997,
  997,  997,  997,  997,    0, 1595, 1596,    0,    0,    0,
  740,    0,    0, 1602, 1603,  741, 1605, 1606,    0,    0,
    0,    0,  241,    0, 1610, 1611, 1612, 1613,    0, 1614,
 1615, 1616, 1617, 1618, 1619,    0,    0,    0,    0,    0,
    0,  740,    0,    0,    0, 1027,  741, 1027, 1027,    0,
    0,    0,    0,    0, 1027,   89, 1792,    0,    0,    0,
 1027,    0,   90,    0,    0, 1027,   91,   92, 1027,    0,
    0,    0,    0,    0,  532,   93,    0,    0,    0,    0,
 1283,    0, 1027, 1032,    0,    0,   94, 1027, 1027,    0,
    0, 1027,    0,    0,    0,    0,    0,    0, 1027,    0,
 1027, 1027,    0,    0, 1027,    0,    0,    0, 1027,    0,
 1027,    0, 1027,    0, 1027,  532,    0,    0,  532, 1027,
 1027,    0, 1027,    0, 1032, 1032, 1032, 1032, 1032, 1032,
 1032,    0,    0,  532,    0,  532,    0, 1027, 1027,    0,
 1027,    0, 1032, 1032, 1032, 1032,    0,    0,   95,    0,
    0, 1027,    0, 1027,    0, 1027, 1027, 1027,    0, 1027,
    0,    0, 1027, 1027,   96,    0,    0,    0,    0,    0,
    0,    0, 1027,    0, 1032,    0, 1032,    0,    0,   97,
    0,    0,    0,   98,    0, 1027,    0,    0, 1027,    0,
    0,    0,    0,    0,   99,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1027,    0,    0,    0,    0,    0,
 1027, 1027,    0,  100,    0, 1027,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1027, 1027,    0, 1027,
    0,    0,    0,    0,    0, 1027,    0,    0, 1027,    0,
 1027, 1027, 1027, 1027,    0, 1027,    0,    0,    0,    0,
    0,    0,    0, 1027, 1027,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1027, 1027,    0, 1027,    0,    0,
    0,    0,  101,    0, 1027, 1027, 1027, 1027, 1027,    0,
 1027,    0,    0, 1027, 1027, 1027, 1027, 1027,    0,    0,
    0, 1027, 1027,    0,    0,    0, 1027, 1027, 1027, 1027,
 1027, 1027, 1027, 1027,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  532,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1032,    0, 1032, 1032,  532,
  532,  532,    0,    0, 1032,  532,    0,    0,  532,    0,
 1032,    0,    0,    0,    0, 1032,    0,    0, 1032,    0,
    0,  532,    0,    0,  534,    0,    0,  532,    0,    0,
    0,    0, 1032, 1028,    0,    0,    0, 1032, 1032,    0,
    0, 1032,    0,    0,    0,    0,    0,    0, 1032,    0,
 1032, 1032,    0,    0, 1032,    0,    0,    0, 1032,    0,
 1032,    0, 1032,    0, 1032,  534,    0,    0,  534, 1032,
 1032,    0, 1032,    0, 1028, 1028, 1028, 1028, 1028, 1028,
 1028,    0,    0,  534,    0,  534,    0, 1032, 1032,    0,
 1032,    0, 1028, 1028, 1028, 1028,  532,    0,    0,    0,
    0, 1032,    0, 1032,    0, 1032, 1032, 1032,    0, 1032,
    0,    0, 1032, 1032,    0,    0,    0,  532,    0,    0,
    0,    0, 1032,  532, 1028,    0, 1028,    0,    0,    0,
    0,    0,  532,    0,  532, 1032,    0,    0, 1032,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  532,    0, 1032,    0,    0,  532,    0,    0,
 1032, 1032,    0,    0,    0, 1032,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1032, 1032,    0, 1032,
    0,    0,    0,    0,    0, 1032,  532,    0, 1032,    0,
 1032, 1032, 1032, 1032,    0, 1032,    0,    0,    0,    0,
    0,    0,    0, 1032, 1032,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1032, 1032,    0, 1032,    0,    0,
    0,    0,    0,    0, 1032, 1032, 1032, 1032, 1032,    0,
 1032,    0,    0, 1032, 1032, 1032, 1032, 1032,    0,    0,
    0, 1032, 1032,    0,    0,    0, 1032, 1032, 1032, 1032,
 1032, 1032, 1032, 1032,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  534,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1028,    0, 1028, 1028,    0,
  534,  534,    0,    0, 1028,  534,    0,    0,  534,    0,
 1028,    0,    0,    0,    0, 1028,    0,    0, 1028,    0,
    0,  534,    0,    0,    0,    0,    0,  534,    0,    0,
    0,    0, 1028, 1041,    0,    0,    0, 1028, 1028,    0,
    0, 1028,    0,    0,    0,    0,    0,    0, 1028,    0,
 1028, 1028,    0,    0, 1028,    0,    0,    0, 1028,    0,
 1028,    0, 1028,    0, 1028,    0,    0,    0,    0, 1028,
 1028,    0, 1028,    0, 1041, 1041, 1041, 1041, 1041, 1041,
 1041,    0,    0,    0,    0,    0,    0, 1028, 1028,    0,
 1028,    0, 1041, 1041, 1041, 1041,  534,    0,    0,    0,
    0, 1028,    0, 1028,    0, 1028, 1028, 1028,    0, 1028,
    0,    0, 1028, 1028,    0,    0,    0,  534,    0,    0,
    0,    0, 1028,  534, 1041,    0, 1041,    0,    0,    0,
    0,    0,    0,    0,  534, 1028,    0,    0, 1028,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  534,    0, 1028,    0,    0,  534,    0,    0,
 1028, 1028,    0,    0,    0, 1028,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1028, 1028,    0, 1028,
    0,    0,    0,    0,    0, 1028,  534,    0, 1028,    0,
 1028, 1028, 1028, 1028,    0, 1028,    0,    0,    0,    0,
    0,    0,    0, 1028, 1028,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1028, 1028,    0, 1028,    0,    0,
    0,    0,    0,    0, 1028, 1028, 1028, 1028, 1028,    0,
 1028,    0,    0, 1028, 1028, 1028, 1028, 1028,    0,    0,
    0, 1028, 1028,    0,    0,    0, 1028, 1028, 1028, 1028,
 1028, 1028, 1028, 1028,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1041,    0, 1041, 1041,    0,
    0,    0,    0,    0, 1041,    0,    0,    0,    0,    0,
 1041,    0,    0,    0,    0, 1041,    0,    0, 1041,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1041, 1046,    0,    0,    0, 1041, 1041,    0,
    0, 1041,    0,    0,    0,    0,    0,    0, 1041,    0,
 1041, 1041,    0,    0, 1041,    0,    0,    0, 1041,    0,
 1041,    0, 1041,    0, 1041,    0,    0,    0,    0, 1041,
 1041,    0, 1041,    0, 1046, 1046, 1046, 1046, 1046, 1046,
 1046,    0,    0,    0,    0,    0,    0, 1041, 1041,    0,
 1041,    0, 1046, 1046, 1046, 1046,    0,    0,    0,    0,
    0, 1041,    0, 1041,    0, 1041, 1041, 1041,    0, 1041,
    0,    0, 1041, 1041,    0,    0,    0,    0,    0,    0,
    0,    0, 1041,    0, 1046,    0, 1046,    0,    0,    0,
    0,    0,    0,    0,    0, 1041,    0,    0, 1041,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1041,    0,    0,    0,    0,    0,
 1041, 1041,    0,    0,    0, 1041,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1041, 1041,    0, 1041,
    0,    0,    0,    0,    0, 1041,    0,    0, 1041,    0,
 1041, 1041, 1041, 1041,    0, 1041,    0,    0,    0,    0,
    0,    0,    0, 1041, 1041,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1041, 1041,    0, 1041,    0,    0,
    0,    0,    0,    0, 1041, 1041, 1041, 1041, 1041,    0,
 1041,    0,    0, 1041, 1041, 1041, 1041, 1041,    0,    0,
    0, 1041, 1041,    0,    0,    0, 1041, 1041, 1041, 1041,
 1041, 1041, 1041, 1041,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1046,    0, 1046, 1046,    0,
    0,    0,    0,    0, 1046,    0,    0,    0,    0,    0,
 1046,    0,    0,    0,    0, 1046,    0,    0, 1046,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1046,  870,    0,    0,    0, 1046, 1046,    0,
    0, 1046,    0,    0,    0,    0,    0,    0, 1046,    0,
 1046, 1046,    0,    0, 1046,    0,    0,    0, 1046,    0,
 1046,    0, 1046,    0, 1046,    0,    0,    0,    0, 1046,
 1046,    0, 1046,    0,  870,  870,  870,  870,  870,    0,
  870,    0,    0,    0,    0,    0,    0, 1046, 1046,    0,
 1046,    0,  870,  870,  870,  870,    0,    0,    0,    0,
    0, 1046,    0, 1046,    0, 1046, 1046, 1046,    0, 1046,
    0,    0, 1046, 1046,    0,    0,    0,    0,    0,    0,
    0,    0, 1046,    0,    0,    0,  870,    0,    0,    0,
    0,    0,    0,    0,    0, 1046,    0,    0, 1046,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  293,    0,
    0,    0,    0,    0, 1046,    0,    0,    0,    0,    0,
 1046, 1046,    0,    0,    0, 1046,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1046, 1046,    0, 1046,
    0,    0,    0,    0,    0, 1046,    0,    0, 1046,    0,
 1046, 1046, 1046, 1046,    0, 1046,    0,    0,    0,    0,
    0,    0,    0, 1046, 1046,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1046, 1046,    0, 1046,    0,    0,
    0,    0,    0,    0, 1046, 1046, 1046, 1046, 1046,    0,
 1046,    0,    0, 1046, 1046, 1046, 1046, 1046,    0,    0,
    0, 1046, 1046,    0,    0,    0, 1046, 1046, 1046, 1046,
 1046, 1046, 1046, 1046,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  870,    0,  870,  870,    0,
    0,    0,    0,    0,  870,    0,    0,    0,    0,    0,
  870,    0,    0,    0,    0,  870,    0,    0,  870,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  870,  883,    0,    0,    0,  870,  870,    0,
    0,  870,    0,    0,    0,    0,    0,    0,  870,    0,
  870,  870,    0,    0,  870,    0,    0,    0,  870,    0,
  870,    0,  870,    0,  870,    0,    0,    0,    0,  870,
  870,    0,  870,    0,  883,  883,  883,  883,  883,    0,
  883,    0,    0,    0,    0,    0,    0,  870,  870,  829,
  870,    0,  883,  883,  883,  883,    0,    0,    0,    0,
    0,  870,    0,  870,    0,  870,  870,  870,    0,  870,
    0,    0,  870,  870,    0,    0,    0,    0,    0,    0,
    0,    0,  870,    0,    0,    0,  883,    0,    0,    0,
    0,    0,    0,    0,    0,  870,    0,    0,  870,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1031,    0,
    0,    0,    0,    0,  870,    0,    0,    0,    0,    0,
  870,  870,    0,    0,    0,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  870,  870,    0,  870,
    0,    0,    0,    0,    0,  870,    0,    0,  870,    0,
  870,  870,  870,  870,    0,  870,    0,    0,    0,    0,
    0,    0,    0,  870,  870,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  870,  870,    0,  870,    0,    0,
    0,    0,    0,    0,  870,  870,  870,  870,  870,    0,
  870,    0,    0,  870,  870,  870,  870,  870,    0,    0,
    0,  870,  870,    0,    0,    0,  870,  870,  870,  870,
  870,  870,  870,  870,    0,    0,    0,    0,    0,    0,
  112,    0,    0,  113,    0,  114,  115,  116,  117,    0,
  118,    0,    0,    0,    0,    0,    0,    0,  119,  120,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  121,
  122,    0,  123,    0,    0,  883,    0,  883,  883,  124,
  125,    0,  126,  127,  883,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,  883,  134,  135,  883,    0,
    0,    0,  136,  137,  138,    0,  139,  140,  141,    0,
    0,    0,  883,  886,    0,    0,    0,  883,  883,    0,
    0,  883,    0,    0,    0,    0,    0,    0,  883,    0,
  883,  883,    0,    0,  883,    0,    0,    0,  883,    0,
  883,    0,  883,    0,  883,    0,    0,    0,    0,  883,
  883,    0,  883,    0,  886,  886,  886,  886,  886,    0,
  886,    0,    0,    0,    0,    0,    0,  883,  883,    0,
  883,    0,  886,  886,  886,  886,    0,    0,    0,    0,
    0,  883,    0,  883,    0,  883,  883,  883,    0,  883,
    0,    0,  883,  883,    0,    0,    0,    0,    0,    0,
    0,    0,  883,    0,    0,    0,  886,    0,    0,    0,
    0,    0,    0,    0,    0,  883,    0,    0,  883,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  883,    0,    0,    0,    0,    0,
  883,  883,    0,    0,    0,  883,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  883,  883,    0,  883,
    0,    0,    0,    0,    0,  883,    0,    0,  883,    0,
  883,  883,  883,  883,    0,  883,    0,    0,    0,    0,
    0,    0,    0,  883,  883,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  883,  883,    0,  883,    0,    0,
    0,    0,    0,    0,  883,  883,  883,  883,  883,    0,
  883,    0,    0,  883,  883,  883,  883,  883,    0,    0,
    0,  883,  883,    0,    0,    0,  883,  883,  883,  883,
  883,  883,  883,  883,    0,    0,    0,    0,    0,    0,
  112,    0,    0,  113,    0,  114,  115,  116,  117,    0,
  118,    0,    0,    0,    0,    0,    0,    0,  119,  120,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  121,
  122,    0,  123,    0,    0,  886,    0,  886,  886,  124,
  125,    0,  126,  127,  886,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,  886,    0,
    0,    0,  136,  137,  138,    0,  139,  140,  141,    0,
    0,    0,  886,  887,    0,    0,    0,  886,  886,    0,
    0,  886,    0,    0,    0,    0,    0,    0,  886,    0,
  886,  886,    0,    0,  886,    0,    0,    0,  886,    0,
  886,    0,  886,    0,  886,    0,    0,    0,    0,  886,
  886,    0,  886,    0,  887,  887,  887,  887,  887,    0,
  887,    0,    0,    0,    0,    0,    0,  886,  886,    0,
  886,    0,  887,  887,  887,  887,    0,    0,    0,    0,
    0,  886,    0,  886,    0,  886,  886,  886,    0,  886,
    0,    0,  886,  886,    0,    0,    0,    0,    0,    0,
    0,    0,  886,    0,    0,    0,  887,    0,    0,    0,
    0,    0,    0,    0,    0,  886,    0,    0,  886,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  886,    0,    0,    0,    0,    0,
  886,  886,    0,    0,    0,  886,    0,    0,    0,    0,
    0, 1301,    0,    0,    0,    0,  886,  886,    0,  886,
    0,    0,    0,    0,    0,  886,    0,    0,  886,    0,
  886,  886,  886,  886,    0,  886,    0,    0,    0,    0,
    0,    0,    0,  886,  886,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  886,  886,    0,  886,    0,    0,
    0,    0,    0,    0,  886,  886,  886,  886,  886,    0,
  886,    0,    0,  886,  886,  886,  886,  886,    0,    0,
    0,  886,  886,    0,    0,    0,  886,  886,  886,  886,
  886,  886,  886,  886,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    1,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  887,    0,  887,  887,    0,
    0,    0,    2,    0,  887,    0,    0,    0,    0,    0,
    3,    0,    0,    0,    0,    0,    0,    4,  887,    5,
    0,    6,  881,    0,    0,    0,    7,    0,    0,    0,
    0,    0,  887,    8,    0,    0,    0,  887,  887,    0,
    0,  887,    0,    0,    0,    0,    0,    9,  887,    0,
  887,  887,    0,    0,  887,    0,    0,   10,  887,    0,
  887,    0,  887,  881,  887,  881,  881,  881,    0,  887,
  887,    0,  887,    0,    0,    0,    0,    0,    0,    0,
    0,  881,  881,  881,  881,    0,    0,  887,  887,    0,
  887,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  887,    0,  887,    0,  887,  887,  887,    0,  887,
    0,    0,  887,  887,    0,  881,    0,    0,  426,    0,
   11,    0,  887,    0,    0,    0,    0,   12,    0,    0,
   13,    0,   14,   15,    0,  887,    0,    0,  887,    0,
    0,    0,    0,    0,    0,    0,  428,    0,    0,    0,
    0,    0,    0,    0,  887,    0,    0,   16,    0,    0,
  887,  887,    0,    0,    0,  887,    0,    0,    0,    0,
    0,    0,    0,    0,  430,    0,  887,  887,    0,  887,
    0,    0,    0,    0,    0,  887,    0,    0,  887,    0,
  887,  887,  887,  887,    0,  887,    0,    0,    0,    0,
    0,    0,    0,  887,  887,    0,   17,    0,    0,    0,
    0,    0,    0,    0,  887,  887,    0,  887,    0,    0,
    0,   18,   19,    0,  887,  887,  887,  887,  887,    0,
  887,    0,    0,  887,  887,  887,  887,  887,    0,    0,
    0,  887,  887,    0,    0,  435,  887,  887,  887,  887,
  887,  887,  887,  887,    0,    0,    0,    0,    0,    0,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  438,  439,  440,    0,  236,
  237,    0,    0,    0,  881,    0,  881,  881,    0,    0,
    0,    0,    0,  881,    0,    0,    0,    0,  443,  444,
  445,  446,  447,  448,  449,  450,  451,  881,    0,    0,
    0,  880,    0,  453,  454,    0,    0,    0,    0,    0,
    0,  881,  458,    0,    0,    0,  881,  881,    0,    0,
  881,    0,    0,    0,    0,    0,    0,  881,    0,  881,
  881,    0,    0,  881,    0,    0,    0,  881,    0,  881,
    0,  881,  880,  881,  880,  880,  880,    0,  881,  881,
    0,  881,  907,    0,    0,  628,    0,    0,    0,    0,
  880,  880,  880,  880,    0,    0,  881,  881,  908,  881,
    0,    0,  629,    0,    0,  630,    0,    0,    0,    0,
  881,    0,  881,    0,  881,  881,  881,    0,  881,    0,
    0,  881,  881,    0,  880,    0,    0,    0,    0,    0,
    0,  881,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  881,  631,    0,  881,    0,    0,
    0,  632,  633,  634,    0,  635,  636,  637,    0,  638,
    0,    0,    0,  881,    0,    0,    0,    0,    0,  881,
  881,    0,    0,    0,  881,    0,    0,  639,    0,  640,
    0,    0,    0,    0,    0,  881,  881,    0,  881,    0,
    0,    0,    0,    0,  881,    0,    0,  881,    0,  881,
  881,  881,  881,    0,  881,    0,    0,    0,  641,    0,
    0,    0,  881,  881,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  881,  881,  642,  881,    0,    0,    0,
    0,    0,    0,  881,  881,  881,  881,  881,    0,  881,
    0,    0,  881,  881,  881,  881,  881,  643,    0,    0,
  881,  881,    0,    0,    0,  881,  881,  881,  881,  881,
  881,  881,  881,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  644,  645,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  880,    0,  880,  880,    0,    0,  646,
    0,    0,  880,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  880,    0,    0,    0,
  882,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  880,    0,    0,    0,    0,  880,  880,    0,    0,  880,
    0,    0,    0,    0,    0,    0,  880,    0,  880,  880,
    0,    0,  880,    0,    0,    0,  880,    0,  880,    0,
  880,  882,  880,  882,  882,  882,    0,  880,  880,    0,
  880,    0,    0,    0,    0,    0,    0,    0,    0,  882,
  882,  882,  882,    0,    0,  880,  880,    0,  880,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  880,
    0,  880,    0,  880,  880,  880,    0,  880,    0,    0,
  880,  880,    0,  882,    0,    0,    0,    0,    0,    0,
  880,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  880,    0,    0,  880,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  880,    0,    0,    0,    0,    0,  880,  880,
    0,    0,    0,  880,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  880,  880,    0,  880,    0,    0,
    0,    0,    0,  880,    0,    0,  880,    0,  880,  880,
  880,  880,    0,  880,    0,    0,    0,    0,    0,    0,
    0,  880,  880,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  880,  880,    0,  880,    0,    0,    0,    0,
    0,    0,  880,  880,  880,  880,  880,  845,  880,    0,
    0,  880,  880,  880,  880,  880,    0,    0,    0,  880,
  880,    0,    0,    0,  880,  880,  880,  880,  880,  880,
  880,  880,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  845,    0,
    0,  845,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  882,    0,  882,  882,  845,    0,    0,    0,
    0,  882,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  882,    0,    0,    0,  885,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  882,
    0,    0,    0,    0,  882,  882,    0,    0,  882,    0,
    0,    0,    0,    0,    0,  882,    0,  882,  882,    0,
    0,  882,    0,    0,    0,  882,    0,  882,    0,  882,
  885,  882,  885,  885,  885,    0,  882,  882,    0,  882,
    0,    0,    0,    0,    0,    0,    0,    0,  885,  885,
  885,  885,    0,    0,  882,  882,    0,  882,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  882,    0,
  882,    0,  882,  882,  882,    0,  882,    0,    0,  882,
  882,    0,  885,    0,    0,    0,    0,    0,    0,  882,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  882,    0,    0,  882,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,    0,    0,    0,    0,    0,  882,  882,    0,
    0,    0,  882,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  882,  882,    0,  882,    0,    0,    0,
    0,    0,  882,    0,    0,  882,    0,  882,  882,  882,
  882,    0,  882,    0,    0,    0,    0,    0,    0,  845,
  882,  882,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,  882,    0,  882,    0,    0,    0,    0,    0,
    0,  882,  882,  882,  882,  882,    0,  882,  691,    0,
  882,  882,  882,  882,  882,    0,    0,    0,  882,  882,
    0,    0,    0,  882,  882,  882,  882,  882,  882,  882,
  882,    0,  845,    0,    0,  845,    0,    0,    0,    0,
    0,    0,  845,    0,  845,    0,    0,    0,  845,  691,
  673,    0,  691,  845,    0,    0,  845,    0,    0,    0,
    0,  885,    0,  885,  885,    0,    0,  691,    0,    0,
  885,  845,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  885,  845,    0,    0,    0,    0,
  673,  673,    0,  845,  673,    0,  845,  845,  885,    0,
    0,    0,    0,  885,  885,    0,  845,  885,    0,  673,
    0,    0,    0,    0,  885,    0,  885,  885,    0,    0,
  885,    0,  845,    0,  885,    0,  885,    0,  885,    0,
  885,    0,    0,    0,    0,  885,  885,    0,  885,    0,
    0,    0,    0,    0,  845,    0,    0,    0,    0,  845,
    0,    0,    0,  885,  885,    0,  885,    0, 1004,    0,
    0,  845,    0,  845,    0,    0,    0,  885,    0,  885,
    0,  885,  885,  885,    0,  885,    0,    0,  885,  885,
    0,    0,    0,    0,    0,    0,    0,    0,  885,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  885,    0,    0,  885,    0,    0,    0,    0,    0,
  845,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  885,    0,    0,  662,    0,    0,  885,  885,    0,    0,
  845,  885,    0,    0,  845,    0,    0,    0,    0,    0,
    0,    0,  885,  885,    0,  885,    0,    0,    0,    0,
    0,  885,    0,    0,  885,    0,  885,  885,  885,  885,
    0,  885,    0,  662,  662,    0,    0,  662,    0,  885,
  885,    0,  691,    0,    0,    0,    0,    0,    0,    0,
  885,  885,  662,  885,    0,    0,    0,    0,    0,    0,
  885,  885,  885,  885,  885,    0,  885,    0,    0,  885,
  885,  885,  885,  885,    0,    0,    0,  885,  885,    0,
    0,    0,  885,  885,  885,  885,  885,  885,  885,  885,
    0,    0,    0,  691,    0,    0,  691,    0,    0,    0,
    0,    0,    0,  691,    0,  691,    0,    0,    0,  691,
    0,    0,    0,    0,  691,    0,    0,  691,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  691,    0,    0,  673,    0,    0,  673,    0,
    0,    0,    0,    0,    0,  673,  691,  673,    0,    0,
    0,  673,    0,    0,  691,    0,  673, 1031,  691,  673,
    0,    0,    0,    0,    0,    0,    0,  691,    0,    0,
    0,    0,    0,    0,  673,    0,    0,    0,    0, 1003,
    0,    0,    0,  691,    0,    0,    0,    0,  673,    0,
    0,    0,    0,    0,    0,    0,  673,    0,    0,    0,
  673,    0,  689,    0,    0,    0,    0,    0,    0,  673,
  691,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  691,    0,  691,  673,    0,    0,    0,    0,
  691,    0,    0,  691,    0,  691,  691,  691,  691,    0,
  691,    0,    0,  689,    0,    0,  689,    0,  691,  691,
    0,    0,  673,    0,    0,    0,    0,    0,    0,  691,
  691,  689,  691,    0,  673,    0,  673,    0,    0,  691,
  691,  691,  691,  691,    0,  691,    0,    0,  691,  691,
  691,  691,  691,    0,    0,    0,  691,  691,    0,    0,
    0,  691,  691,  691,  691,  691,  691,  691,  691,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  662,    0,
    0,  662,    0,  673,    0,    0,    0,    0,  662,    0,
  662,    0,    0,    0,  662,    0,    0,    0,    0,  662,
    0,    0,  662,  673,    0,    0,    0,  673,  112,    0,
    0,  113,    0,  114,  115,  116,  117,  662,  118,    0,
    0,    0,    0,    0,    0,    0,  119,  120,    0,    0,
    0,  662,    0,    0,    0,    0,    0,  121,  122,  662,
  123,    0,    0,  662,    0,    0,    0,  124,  125,    0,
  126,  127,  662,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,  662,    0,
  136,  137,  138,    0,  139,  140,  141,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  662,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  662,    0,  662,
    0,    0,    0,    0,    0,  662,    0,    0,  662,    0,
  662,  662,  662,  662,    0,  662,    0,    0,    0,    0,
    0,    0,    0,  662,  662,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  662,  662,    0,  662,    0,    0,
    0,    0,    0,    0,  662,  662,  662,  662,  662,    0,
  662,    0,    0,  662,  662,  662,  662,  662,    0,    0,
    0,  662,  662,  861,    0,    0,  662,  662,  662,  662,
  662,  662,  662,  662,    0,    0,    0,  689,    0,    0,
  689,    0,    0,    0,    0,    0,   14,  689,    0,  689,
    0,    0,    0,  689,    0,    0,    0,    0,  689,    0,
    0,  689,    0,    0,  861, 1035, 1035,  861, 1035, 1035,
 1035,    0,    0,    0,    0,    0,  689,    0,    0,    0,
    0,    0,  861, 1035, 1035, 1035,    0,    0,    0,  112,
  689,    0,  113,    0,  114,  115,  116,  117,  689,  118,
    0,    0,  689,    0,    0,    0,    0,  119,  120,    0,
    0,  689,    0,    0, 1035,    0,    0,    0,  121,  122,
    0,  123,    0,    0,    0,    0,    0,  689,  124,  125,
    0,  126,  127,    0,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  137,  138,  689,  139,  140,  141,    0,    0,
    0,    0,    0,    0,    0,    0,  689,    0,  689,    0,
    0,    0,    0,    0,  662,    0,    0,  662,    0,  662,
  662,  662,  662,    0,  662,    0,    0,    0,    0,    0,
    0,    0,  662,  662,    0,    0,    0,    0,    0,    0,
    0,    0,  862,  662,  662,    0,  662,    0,    0,    0,
    0,    0,    0,  662,  662,  689,  662,  662,    0,  662,
    0,    0,  662,  662,  662,  662,  662,    0,    0,    0,
  662,  662,    0,    0,    0,  689,  662,  662,  662,  689,
  662,  662,  662,  862, 1036, 1036,  862, 1036, 1036, 1036,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  862, 1036, 1036, 1036,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  861,    0,    0,    0,    0,
    0,    0,    0, 1036, 1035,    0,    0,    0,    0,    0,
 1035,    0,    0,    0,    0, 1035,    0,    0, 1035,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  861,    0,
    0,  861,    0,    0, 1035,    0,    0,    0,  861,    0,
  861,    0, 1035,    0,  861,    0,    0,    0,    0,  861,
 1035,    0,  861,    0,    0,  863, 1037, 1037,  863, 1037,
 1037, 1037,    0,    0,    0,    0,    0,  861, 1035,    0,
 1035,    0,    0,  863, 1037, 1037, 1037,    0,    0,    0,
    0,  861,    0, 1035,    0, 1035, 1035, 1035,    0,  861,
    0,    0,  861,  861,    0,    0,    0,  822,    0,    0,
    0,    0,  861,    0,    0, 1037,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  861,    0,
    0,    0,    0,  539,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1035,    0,    0,    0,  822,    0,
  861,  822,    0,    0,    0,  861,    0,    0,    0,    0,
    0,    0,    0,    0,  862,    0,  822,  861,    0,  861,
    0,    0,    0, 1036,  539,    0,    0,  539,    0, 1036,
    0,    0,    0,    0, 1036,    0,    0, 1036,    0,    0,
    0,  820,  539,    0,  539,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  861,  862,    0,    0,
  862,    0,    0, 1036,  539,    0,    0,  862,    0,  862,
    0, 1036,  820,  862,    0,  820,  861,    0,  862, 1036,
  861,  862,    0,    0,    0,    0,    0,    0,    0,    0,
  820,    0,    0,    0,    0,    0,  862, 1036,    0, 1036,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  862,    0, 1036,    0, 1036, 1036, 1036,    0,  862,    0,
    0,  862,  862,    0,    0,    0,  863,    0,    0,    0,
    0,  862,    0,    0,    0, 1037,    0,    0,    0,    0,
    0, 1037,    0,    0,    0,    0, 1037,  862,    0, 1037,
    0,    0,    0,  821,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1036,    0,    0,    0,    0,    0,  862,
    0,    0,    0,    0,  862,    0,    0,    0,    0,  863,
    0,    0,  863,  853,    0, 1037,  862,    0,  862,  863,
    0,  863,    0, 1037,  821,  863,    0,  821,    0,    0,
  863, 1037,    0,  863,    0,    0,    0,    0,    0,  822,
    0,    0,  821,    0,    0,    0,    0,    0,  863, 1037,
    0, 1037,    0,    0,  853,    0,    0,  853,    0,    0,
    0,  539,  863,    0, 1037,  862, 1037, 1037, 1037,    0,
  863,    0,  853,  863,  863,    0,  539,    0,  539,  539,
  539,    0,    0,  863,  539,  862,    0,  539,    0,  862,
    0,    0,  822,    0,    0,  822,    0,    0,    0,  863,
  539,    0,  822,    0,  822,    0,  539,    0,  822,  539,
    0,    0,    0,  822,    0, 1037,  822,    0,    0,    0,
    0,  863,    0,  820,    0,    0,  863,    0,    0,    0,
    0,  822,    0,    0,    0,    0,  815,    0,  863,    0,
  863,    0,    0,    0,    0,  822,    0,    0,    0,    0,
    0,  539,    0,  822,    0,    0,  822,  822,    0,    0,
    0,    0,    0,    0,    0,    0,  822,    0,    0,    0,
    0,    0,    0,    0,    0,  539,  820,  815,    0,  820,
  815,    0,  822,    0,    0,    0,  820,  863,  820,    0,
    0,    0,  820,    0,    0,  815,  539,  820,    0,    0,
  820,    0,  539,    0,  822,    0,  539,  863,    0,  822,
    0,  863,    0,  539,    0,  820,    0,    0,  816,    0,
    0,  822,    0,  822,    0,    0,    0,    0,    0,  820,
    0,  539,    0,    0,    0,    0,  539,  820,    0,    0,
  820,  820,    0,    0,    0,  821,    0,    0,    0,    0,
  820,    0,    0,    0,    0,    0,    0,    0,    0,  816,
    0,  539,  816,    0,    0,  539,  820,    0,    0,    0,
  822,    0,    0,    0,    0,  853,    0,  816,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  820,    0,
  822,    0,    0,  820,  822,    0,    0,    0,  821,    0,
    0,  821,    0,    0,    0,  820,    0,  820,  821,    0,
  821,    0,  817,    0,  821,    0,    0,    0,    0,  821,
    0,    0,  821,    0,    0,    0,    0,    0,  853,    0,
    0,  853,    0,    0,    0,    0,    0,  821,  853,    0,
  853,    0,    0,    0,  853,    0,    0,    0,    0,  853,
    0,  821,  853,  817,  820,    0,  817,    0,    0,  821,
    0,    0,  821,  821,    0,    0,    0,  853,    0,    0,
    0,  817,  821,    0,  820,    0,    0,    0,  820,    0,
    0,  853,    0,    0,    0,    0,    0,    0,  821,  853,
    0,    0,  853,  853,  852,    0,    0,    0,  815,    0,
    0,    0,  853,    0,    0,    0,    0,    0,    0,    0,
  821,    0,    0,    0,    0,  821,    0,    0,  853,    0,
    0,    0,    0,    0,    0,    0,    0,  821,    0,  821,
    0,    0,    0,    0,    0,  852,    0,    0,  852,    0,
  853,    0,    0,    0,    0,  853,    0,    0,    0,    0,
    0,  815,    0,  852,  815,    0,    0,  853,    0,  853,
    0,  815,    0,  815,    0,    0,    0,  815,    0,    0,
    0,    0,  815,    0,    0,  815,  821,    0,    0,    0,
  816,    0,    0,  846,    0,    0,    0,    0,    0,    0,
  815,    0,    0,    0,    0,    0,  821,    0,    0,    0,
  821,    0,    0,    0,  815,    0,  853,    0,    0,    0,
    0,    0,  815,    0,    0,  815,  815,  854,    0,    0,
    0,    0,    0,    0,  846,  815,  853,  846,    0,    0,
  853,    0,    0,  816,    0,    0,  816,    0,    0,    0,
    0,  815,  846,  816,    0,  816,    0,    0,    0,  816,
    0,    0,    0,    0,  816,    0,    0,  816,  854,    0,
    0,  854,    0,  815,    0,    0,    0,    0,  815,    0,
    0,    0,  816,    0,  817,    0,  854,  855,    0,    0,
  815,    0,  815,    0,    0,    0,  816,    0,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  816,  816,    0,
    0,    0,    0,    0,    0,    0,    0,  816,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  855,    0,
    0,  855,    0,  816,    0,    0,    0,  817,    0,  815,
  817,    0,    0,    0,    0,    0,  855,  817,    0,  817,
    0,    0,    0,  817,    0,  816,    0,    0,  817,  815,
  816,  817,    0,  815,    0,    0,  852,    0,    0,  843,
    0,    0,  816,    0,  816,    0,  817,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  817,    0,    0,    0,    0,    0,    0,    0,  817,    0,
    0,  817,  817,  841,    0,    0,    0,    0,    0,    0,
  843,  817,    0,  843,    0,    0,    0,    0,    0,  852,
    0,  816,  852,    0,    0,    0,    0,  817,  843,  852,
    0,  852,    0,    0,    0,  852,    0,    0,    0,    0,
  852,  816,    0,  852,  841,  816,    0,  841,    0,  817,
    0,    0,    0,    0,  817,  846,    0,    0,  852,    0,
    0,    0,  841,  847,    0,    0,  817,    0,  817,    0,
    0,    0,  852,    0,    0,    0,    0,    0,    0,    0,
  852,    0,    0,  852,  852,    0,    0,    0,    0,  854,
    0,    0,    0,  852,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  847,    0,    0,  847,  846,  852,
    0,  846,    0,    0,    0,  817,    0,    0,  846,    0,
  846,    0,  847,    0,  846,    0,    0,    0,    0,  846,
    0,  852,  846,    0,    0,  817,  852,    0,    0,  817,
    0,    0,  854,    0,    0,  854,    0,  846,  852,  855,
  852,    0,  854,    0,  854,    0,    0,    0,  854,    0,
    0,  846,    0,  854,    0,    0,  854,    0,    0,  846,
    0,    0,  846,  846,    0,    0,    0,    0,    0,    0,
    0,  854,  846,  856,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  854,    0,  852,  846,    0,
    0,    0,  855,  854,    0,  855,  854,  854,    0,    0,
    0,    0,  855,    0,  855,    0,  854,  852,  855,    0,
  846,  852,    0,  855,  856,  846,  855,  856,    0,    0,
    0,  843,  854,    0,    0,    0,    0,  846,    0,  846,
    0,  855,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  854,  855,    0,    0,    0,  854,
    0,    0,    0,  855,    0,  841,  855,  855,    0,    0,
    0,  854,    0,  854,    0,    0,  855,    0,    0,    0,
    0,    0,    0,    0,  843,    0,  846,  843,    0,    0,
    0,    0,  855,    0,  843,    0,  843,    0,    0,    0,
  843,    0,    0,    0,    0,  843,  846,    0,  843,    0,
  846,    0,    0,    0,  855,    0,    0,    0,  841,  855,
  854,  841,    0,  843,    0,  847,    0,    0,  841,    0,
  841,  855,    0,  855,  841,    0,    0,  843,    0,  841,
  854,    0,  841,    0,  854,  843,    0,    0,  843,  843,
    0,    0,    0,    0,    0,    0,    0,  841,  843,    0,
    0,    0,  842,    0,    0,    0,    0,    0,    0,    0,
    0,  841,    0,    0,  843,    0,    0,    0,  847,  841,
  855,  847,  841,  841,    0,    0,    0,    0,  847,    0,
  847,    0,  841,    0,  847,    0,  843,    0,    0,  847,
  855,  843,  847,  842,  855,    0,  842,    0,  841,    0,
    0,    0,    0,  843,    0,  843,    0,  847,    0,    0,
    0,  842,  844,    0,    0,    0,    0,    0,    0,    0,
  841,  847,    0,    0,    0,  841,    0,    0,    0,  847,
    0,    0,  847,  847,    0,  856,    0,  841,    0,  841,
    0,    0,  847,    0,    0,    0,  848,    0,    0,    0,
    0,    0,  843,  844,    0,    0,  844,    0,  847,  211,
    0,    0,  211,    0,    0,    0,    0,    0,    0,    0,
    0,  844,  843,    0,    0,  211,  843,    0,    0,  211,
  847,    0,  211,    0,    0,  847,  841,  848,  856,    0,
  848,  856,    0,    0,    0,    0,    0,  847,  856,  847,
  856,    0,    0,    0,  856,  848,  841,    0,    0,  856,
  841,    0,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  211,  686,    0,    0,    0,  856,  211,  211,
  211,    0,  211,  211,  211,    0,  211,    0,    0,    0,
    0,  856,    0,    0,    0,    0,  847,    0,    0,  856,
    0,    0,  856,  856,  211,    0,  211,    0,    0,    0,
    0,    0,  856,    0,  686,    0,  847,  686,    0,    0,
  847,    0,    0,    0,    0,    0,    0,    0,  856,    0,
    0,    0,  686,    0,    0,  211,    0,    0,    0,    0,
   80,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  856,    0,  211,    0,    0,  856,    0,    0,    0,    0,
    0,    0,    0,    0,  842,    0,    0,  856,    0,  856,
    0,    0,    0,    0,  211,    0,    0,    0,    0,    0,
    0,    0,    0,  691,    0,    0,  461,    0,  462,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  464,    0,    0,    0,    0,  313,    0,    0,  211,
  211,    0,    0,    0,    0,    0,  856,  842,    0,    0,
  842,    0,    0,    0,  844,    0,  211,  842,    0,  842,
    0,    0,    0,  842,    0,    0,  856,    0,  842,    0,
  856,  842,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  842,    0,  848,    0,
    0,    0, 1004,    0,    0,    0,    0,    0,    0,    0,
  842,    0,    0,    0,    0,    0,    0,  844,  842,    0,
  844,  842,  842,    0,    0,    0,    0,  844,    0,  844,
    0,  842,    0,  844,    0,    0,    0,    0,  844,    0,
    0,  844,    0,    0,    0,    0,    0,  842,    0,    0,
    0,  848,    0,    0,  848,    0,  844,    0,    0,    0,
    0,  848,    0,  848,    0,    0,    0,  848,    0,  842,
  844,    0,  848,    0,  842,  848,  659,    0,  844,    0,
    0,  844,  844,    0,    0,    0,  842,    0,  842,    0,
  848,  844,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  848,    0,    0,  844,    0,    0,
    0,    0,  848,    0,    0,  848,  848,    0,    0,    0,
    0,    0,    0,    0,    0,  848,    0,    0,  686,  844,
    0,  686,    0,    0,  844,  842,    0,    0,  686,    0,
  686,  848,    0,    0,  686,    0,  844,    0,  844,  686,
  423,  627,  686,    0,  628,  842,    0,  424,    0,  842,
    0,    0,    0,  848,    0,    0,    0,  686,  848,  425,
    0,  629,    0,    0,  630,  426,  427,    0,    0,    0,
  848,  686,  848,    0,    0,    0,    0,  691,    0,  686,
  461,    0,  462,  686,  685,  844,    0,    0,    0,    0,
    0,    0,  686,  428,    0,  464,  429,    0,    0,    0,
  313,    0,    0,    0,  631,  844,    0,    0,  686,  844,
  632,  633,  634,    0,  635,  636,  637,    0,  638,  848,
    0,  430,    0,  431,  432,    0,    0,    0,    0,    0,
    0,    0,    0,  433,    0,  686,  639,  434,  640,  848,
    0,    0,    0,  848,    0,  918,    0,  686,    0,  686,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  641,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   14,    0,  642,   82,  687,    0,    0,    0,
    0,    0,  435,    0,    0,    0,  686,    0,  436,    0,
    0,    0,    0,    0,    0,    0,  643,    0,  437,    0,
    0,    0,    0,    0,    0,    0,  686,    0,    0,    0,
  686,    0,  438,  439,  440,  441,  236,  237,  113,    0,
  114,  115,  116,  117,    0,  118,  442,    0,    0,  659,
    0,  644,  645,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,  646,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  131,  132,  133,  919,  920,
  921,  134,  135,    0,    0,    0,    0,  136,  459,  138,
    0,  460,  140,  141,    0,    0,  691,    0,    0,  461,
    0,  462,  112,    0,  423,  113,    0,  114,  115,  116,
  117,  424,  118,    0,  464,    0,    0,    0,    0,  313,
  119,  120,    0,  425,    0,    0,  684,    0,    0,  426,
  427,  121,  122,    0,  123,    0,    0,    0,    0,    0,
    0,  124,  125,    0,  126,  127,    0,  128,  685,    0,
  129,  130,  131,  132,  133,    0,    0,  428,  134,  135,
  429,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,  662,    0,    0,  662,    0,  662,
  662,  662,  662,    0,  662,  430,    0,  431,  432,    0,
  464,    0,  662,  662,    0,    0,    0,  433,    0,    0,
    0,  434,    0,  662,  662,    0,  662,    0,    0,  686,
    0,    0,    0,  662,  662,    0,  662,  662,    0,  662,
    0,    0,  662,  662,  662,  662,  662,    0,    0,    0,
  662,  662,    0,    0,    0,    0,  662,  662,  662,    0,
  662,  662,  662,    0,    0,    0,    0,    0,    0,    0,
  687,    0,    0,    0,    0,    0,  435,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,  439,  440,  441,
  236,  237,  113,    0,  114,  115,  116,  117,    0,  118,
  442,    0,    0,    0,    0,    0,    0,  119,  120,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  121,  452,
    0,  123,    0,  423,  453,  454,    0,  455,  456,  457,
  424,  126,  127,  458,  128,    0,    0,  129,  130,  688,
  689,  690,  425,    0,    0,  134,  135,    0,  426,  427,
    0,  136,  459,  138,    0,  460,  140,  141,    0,  691,
    0,    0,  461,    0,  462,    0,    0,  685,    0,    0,
    0,    0,    0,    0,    0,    0,  428,  464,    0,  429,
    0,    0,  313,    0,    0,    0,    0,    0,    0,    0,
   81,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,  430,    0,  431,  432,    0,    0,
  119,  120,    0,    0,    0,    0,  433,    0,    0,    0,
  434,  121,  122,    0,  123,    0,    0,    0,  914,    0,
    0,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,  687,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
    0,  436,    0,    0,    0,    0,    0,  595,    0,    0,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  438,  439,  440,  441,  236,
  237,  113,    0,  114,  115,  116,  117,    0,  118,  442,
    0,    0,    0,    0,    0,    0,  119,  120,  443,  444,
  445,  446,  447,  448,  449,  450,  451,  121,  452,    0,
  123,    0,    0,  453,  454,    0,  455,  456,  457,    0,
  126,  127,  458,  128,    0,    0,  129,  130,  688,  689,
  690,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  459,  138,    0,  460,  140,  141,    0,  691,    0,
    0,  461,    0,  462,  112,    0,  423,  113,    0,  114,
  115,  116,  117,  424,  118,    0,  464,    0,    0,    0,
    0,  313,  119,  120,    0,  425,    0,    0,    0,    0,
    0,  426,  427,  121,  122,    0,  123,    0,    0,    0,
    0,    0,    0,  124,  125,    0,  126,  127,    0,  128,
  685,    0,  129,  130,  131,  132,  133,    0,    0,  428,
  134,  135,  429,    0,    0,    0,  136,  137,  138,    0,
  139,  140,  141,    0,    0,  112,    0,    0,  113,    0,
  114,  115,  116,  117,    0,  118,    0,  430,    0,  431,
  432,    0,    0,  119,  120,    0,    0,    0,    0,  433,
    0,    0,    0,  434,  121,  122,    0,  123,    0,    0,
    0,  686,    0,    0,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,    0,
    0,    0,  687,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  438,  439,
  440,  441,  236,  237,  113,    0,  114,  115,  116,  117,
    0,  118,  442,    0,    0,    0,    0,    0,    0,  119,
  120,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  121,  452,    0,  123,    0,  423,  453,  454,    0,  455,
  456,  457,  424,  126,  127,  458,  128,    0,    0,  129,
  130,  688,  689,  690,  425,    0,    0,  134,  135,    0,
  426,  427,    0,  136,  459,  138,    0,  460,  140,  141,
    0,  529,    0,    0,  461,    0,  462,    0,    0,  685,
    0,    0,    0,    0,    0,    0,    0,    0,  428,  464,
    0,  429,    0,    0,  313,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,  431,  432,
    0,    0,    0,    0,    0,    0,    0,    0,  433,    0,
    0,    0,  434,    0,    0,    0,    0,    0,    0,    0,
  914,    0,    0,    0,  983,  744,  742,    0,  743,    0,
  745,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  939,  938,  940,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  687,    0,    0,    0,    0,    0,  435,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
  441,  236,  237,  113,    0,  114,  115,  116,  117,    0,
  118,  442,    0,    0,    0,    0,    0,    0,  119,  120,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  121,
  452,    0,  123,    0,    0,  453,  454,    0,  455,  456,
  457,    0,  126,  127,  458,  128,    0,    0,  129,  130,
  131,  132,  133,    0,    0,    0,  134,  135,    0,    0,
    0,    0,  136,  459,  138,    0,  460,  140,  141,    0,
    0, 1162,    0,    0, 1163,    0,    0,    0,  423,    0,
    0,    0,    0,    0,    0,  424,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  425,    0,    0,
    0,    0,    0,  426,  427,    0,    0,    0,    0,    0,
    0,    0,  529,    0,    0,  461,    0,  462,    0,    0,
  744,  742,    0,  743,    0,  745,    0,    0,    0,    0,
  464,  428,    0,    0,  429,  313,    0,    0,  939,  938,
  940,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  926,    0,    0,    0,    0,  430,
  740,  431,  432,    0,    0,  741,    0,    0,  927,    0,
    0,  433,    0,    0,    0,  434,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  928,    0,    0,    0,    0,    0,
    0,    0,  929,    0,    0,    0,    0,    0,    0,    0,
  930,    0,    0,    0,    0,    0,    0,    0, 1164,    0,
  435,    0,    0,    0,    0,    0,  436,    0,  931,    0,
  932,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,    0,  933,    0,  934,  935,  936,    0,    0,
  438,  439,  440,  441,  236,  237,  113,    0,  114,  115,
  116,  117,    0,  118,  442,    0,    0,    0,    0,    0,
    0,  119,  120,  443,  444,  445,  446,  447,  448,  449,
  450,  451,  121,  452,    0,  123,    0,    0,  453,  454,
    0,  455,  456,  457,  937,  126,  127,  458,  128,    0,
    0,  129,  130,  131,  132,  133,    0,    0,    0,  134,
  135,    0,    0,    0,    0,  136,  459,  138,    0,  460,
  140,  141, 1173,    0,    0, 1163,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,  424,    0,    0,  926,
    0,    0,    0,    0,    0,  740,    0,    0,  425,    0,
  741,    0,    0,  927,  426,  427,    0,    0,    0,    0,
    0,    0,    0,  529,    0,    0,  461,    0,  462,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  464,  428,    0,    0,  429,  313,    0,    0,  928,
    0,    0,    0,    0,    0,    0,    0,  929,    0,    0,
    0,    0,    0,    0,    0,  930,    0,    0,    0,    0,
  430,    0,  431,  432,    0,    0,    0,    0,    0,    0,
    0,    0,  433,  931,    0,  932,  434,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  933,    0,
  934,  935,  936,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1164,
    0,  435,    0,    0,    0,    0,    0,  436,    0,  937,
    0,    0,    0,    0,    0,  106,    0,  437,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  107,    0,    0,
    0,  438,  439,  440,  441,  236,  237,  113,    0,  114,
  115,  116,  117,    0,  118,  442,    0,    0,    0,    0,
    0,  108,  119,  120,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  121,  452,    0,  123,    0,    0,  453,
  454,    0,  455,  456,  457,    0,  126,  127,  458,  128,
    0,    0,  129,  130,  131,  132,  133,    0,    0,    0,
  134,  135,    0,    0,    0,    0,  136,  459,  138,    0,
  460,  140,  141, 1177,    0,    0, 1163,    0,    0,    0,
  423,    0,    0,    0,    0,    0,  109,  424,    0,    0,
    0,    0,    0,    0,    0,    0,  110,    0,    0,  425,
    0,    0,    0,    0,    0,  426,  427,    0,    0,    0,
    0,    0,    0,    0,  529,    0,    0,  461,    0,  462,
    0,  111,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  464,  428,    0,    0,  429,  313,    0,  112,
    0,    0,  113,    0,  114,  115,  116,  117,    0,  118,
    0,    0,    0,    0,    0,    0,    0,  119,  120,    0,
    0,  430,    0,  431,  432,    0,    0,    0,  121,  122,
    0,  123,    0,  433,    0,    0,    0,  434,  124,  125,
    0,  126,  127,    0,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  137,  138,    0,  139,  140,  141,   48,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1164,    0,  435,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,    0,    0,    0,   48,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   48,    0,
    0,    0,  438,  439,  440,  441,  236,  237,  113,    0,
  114,  115,  116,  117,    0,  118,  442,    0,    0,    0,
    0,    0,   48,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,    0,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  459,  138,
    0,  460,  140,  141, 1185,    0,    0, 1163,    0,    0,
    0,  423,    0,    0,    0,    0,    0,   48,  424,    0,
    0,    0,    0,    0,    0,    0,    0,   48,    0,    0,
  425,    0,    0,    0,    0,    0,  426,  427,    0,    0,
    0,    0,    0,    0,    0,  529,    0,    0,  461,    0,
  462,    0,   48,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  464,  428,    0,    0,  429,  313,    0,
   50,    0,    0,   50,    0,   50,   50,   50,   50,    0,
   50,    0,    0,    0,    0,    0,    0,    0,   50,   50,
    0,    0,  430,    0,  431,  432,    0,    0,    0,   50,
   50,    0,   50,    0,  433,    0,    0,    0,  434,   50,
   50,    0,   50,   50,    0,   50,    0,    0,   50,   50,
   50,   50,   50,    0,    0,    0,   50,   50,    0,    0,
    0,    0,   50,   50,   50,    0,   50,   50,   50,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1164,    0,  435,  601,    0,    0,    0,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  437,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  438,  439,  440,  441,  236,  237,  113,
    0,  114,  115,  116,  117,    0,  118,  442,    0,    0,
    0,    0,    0,    0,  119,  120,  443,  444,  445,  446,
  447,  448,  449,  450,  451,  121,  452,    0,  123,    0,
    0,  453,  454,    0,  455,  456,  457,    0,  126,  127,
  458,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  459,
  138,    0,  460,  140,  141, 1188,    0,    0, 1163,    0,
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
    0,    0, 1164,    0,  435,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  437,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  438,  439,  440,  441,  236,  237,
  113,    0,  114,  115,  116,  117,    0,  118,  442,    0,
    0,    0,    0,    0,    0,  119,  120,  443,  444,  445,
  446,  447,  448,  449,  450,  451,  121,  452,    0,  123,
    0,    0,  453,  454,    0,  455,  456,  457,    0,  126,
  127,  458,  128,    0,    0,  129,  130,  131,  132,  133,
    0,    0,    0,  134,  135,    0,    0,    0,    0,  136,
  459,  138,    0,  460,  140,  141, 1193,    0,    0, 1163,
    0,    0,    0,  423,  211,    0,    0,  211,    0,    0,
  424,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,  425,    0,  211,    0,    0,  211,  426,  427,
    0,    0,    0,    0,    0,    0,    0,  529,    0,    0,
  461,    0,  462,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  464,  428,    0,    0,  429,
  313,    0,    0,    0,    0,    0,    0,  211,    0,    0,
    0,    0,    0,  211,  211,  211,    0,  211,  211,  211,
    0,  211,    0,    0,  430,    0,  431,  432,    0,    0,
    0,    0,    0,    0,    0,    0,  433,    0,    0,  211,
  434,  211,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  211,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  211,    0,    0,
    0,    0,    0, 1164,    0,  435,    0,    0,    0,    0,
    0,  436,    0,    0,    0,    0,    0,    0,    0,  211,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  438,  439,  440,  441,  236,
  237,  113,    0,  114,  115,  116,  117,    0,  118,  442,
    0,    0,    0,    0,  211,  211,  119,  120,  443,  444,
  445,  446,  447,  448,  449,  450,  451,  121,  452,    0,
  123,  211,    0,  453,  454,    0,  455,  456,  457,    0,
  126,  127,  458,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,    0,
  136,  459,  138,    0,  460,  140,  141, 1196,    0,    0,
 1163,    0,    0,    0,  423,  141,    0,    0,  141,    0,
    0,  424,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  425,    0,  141,    0,    0,  141,  426,
  427,    0,    0,    0,    0,    0,    0,    0,  529,    0,
    0,  461,    0,  462,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  464,  428,    0,    0,
  429,  313,    0,    0,    0,    0,    0,    0,  141,    0,
    0,    0,    0,    0,  141,  141,  141,    0,  141,  141,
  141,    0,  141,    0,    0,  430,    0,  431,  432,    0,
    0,    0,    0,    0,    0,    0,    0,  433,    0,    0,
  141,  434,  141,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  141,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  141,    0,
    0,    0,    0,    0, 1164,    0,  435,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
  141,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,  439,  440,  441,
  236,  237,  113,    0,  114,  115,  116,  117, 1249,  118,
  442,    0,    0,    0,    0,  141,  141,  119,  120,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  121,  452,
    0,  123,  141,    0,  453,  454,    0,  455,  456,  457,
    0,  126,  127,  458,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  459,  138,    0,  460,  140,  141, 1199,    0,
    0, 1163,    0,    0,    0,  423,    0,    0, 1250,    0,
    0,    0,  424,    0,    0,    0,  646,    0,  646,  646,
    0,  646,    0,    0,  425,    0,    0,    0,    0,    0,
  426,  427,    0,    0,  646,    0,    0,    0,    0,  646,
    0,    0,    0, 1251,    0,    0,    0,    0, 1252,    0,
    0,    0,    0,    0, 1253,    0,    0,    0,  428,    0,
    0,  429,    0,    0,    0,  112,    0,    0,  113,    0,
  114,  115,  116, 1254,    0,  118,    0,    0,    0,    0,
    0,    0,    0,  119,  120,    0,  430,    0,  431,  432,
    0,    0,    0,    0,  121,  122,    0,  123,  433,    0,
    0,    0,  434,    0,  124,  125,    0,  126,  127,    0,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  137,  138,
    0,  139,  140,  141,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1164,    0,  435,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  438,  439,  440,
  441,  236,  237,  113,    0,  114,  115,  116,  117,    0,
  118,  442,    0,    0,    0,    0,    0,    0,  119,  120,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  121,
  452,    0,  123,    0,    0,  453,  454,    0,  455,  456,
  457,    0,  126,  127,  458,  128,  646,    0,  129,  130,
  131,  132,  133,  646,    0,    0,  134,  135,    0,    0,
  646,    0,  136,  459,  138,    0,  460,  140,  141,    0,
    0,    0,  646,    0,    0,    0,    0,    0,  646,  646,
    0,    0,    0,    0,    0,    0,  646,  640,    0,  640,
  640,    0,  640,    0,    0,  510,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  640,  646,    0,    0,  646,
  640,    0,  112,    0,    0,  113,    0,  114,  115,  116,
  117,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  119,  120,    0,    0,  646,    0,  646,  646,    0,    0,
    0,  121,  122,    0,  123,    0,  646,    0,    0,    0,
  646,  124,  125,    0,  126,  127,    0,  128,    0,    0,
  129,  130,  131,  132,  133,    0,    0,    0,  134,  135,
    0,    0,    0,    0,  136,  137,  138,    0,  139,  140,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  646,    0,    0,    0,    0,
    0,  646,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  646,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  646,  646,  646,  646,  646,
  646,  646,    0,  646,  646,  646,  646,    0,  646,  646,
    0,    0,    0,    0,    0,    0,  646,  646,  646,  646,
  646,  646,  646,  646,  646,  646,  646,  646,  646,    0,
  646,    0,    0,  646,  646,    0,  646,  646,  646,    0,
  646,  646,  646,  646,    0,    0,  646,  646,  646,  646,
  646,    0,    0,    0,  646,  646,    0,    0,    0,    0,
  646,  646,  646,    0,  646,  646,  646,  640,    0,    0,
    0,    0,    0,    0,  640,    0,    0,    0,    0,    0,
    0,  640,    0,    0,    0,  529,    0,  534,  461,    0,
  462,    0,    0,  640,    0,    0,    0,    0,    0,  640,
  640,    0,    0,  464,    0,    0,    0,  640,  313,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  640,  997,    0,
  640,  997,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  999,    0,  997,    0,
    0,  997,    0,    0,    0,  640,    0,  640,  640,    0,
    0,    0,    0,    0,    0,    0,    0,  640,    0,    0,
    0,  640,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  997,    0,    0,    0,    0,    0,  997,  997,  997,
    0,  997,  997,  997,    0,  997,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  997,    0,  997,  640,    0,    0,    0,
    0,    0,  640,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  640,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  997,    0,  640,  640,  640,  640,
  640,  640,  640,    0,  640,  640,  640,  640,    0,  640,
  640,  997,    0,    0,    0,    0,    0,  640,  640,  640,
  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,
    0,  640,    0,  997,  640,  640,    0,  640,  640,  640,
    0,  640,  640,  640,  640,    0,    0,  640,  640,  640,
  640,  640,  423,    0,    0,  640,  640,    0,    0,  424,
    0,  640,  640,  640,    0,  640,  640,  640,  997,  997,
    0,  425,    0,    0,    0,    0,    0,  426,  427,  529,
    0,    0,  461,    0,  462,  997,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  464,    0,    0,
    0,    0,  313,    0,    0,  428,    0,    0,  429,    0,
    0,  112,    0,    0,  113,    0,  114,  115,  116,  117,
    0,  118,    0,    0,    0,    0,    0,    0,    0,  119,
  120,    0,    0,  430,    0,  431,  432,    0,    0,    0,
  121,  122,    0,  123,    0,  433,    0,    0,    0,  434,
  124,  125,    0,  126,  127,    0,  128,    0,    0,  129,
  130,  131,  132,  133,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  137,  138,    0,  139,  140,  141,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  437,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  438,  439,  440,  441,  236,  237,
  113,    0,  114,  115,  116,  117,    0,  118,  442,    0,
    0,    0,    0,    0,    0,  119,  120,  443,  444,  445,
  446,  447,  448,  449,  450,  451,  121,  452,    0,  123,
    0,    0,  453,  454,    0,  455,  456,  457,    0,  126,
  127,  458,  128,    0,    0,  129,  130,  131,  132,  133,
    0,  943,    0,  134,  135,    0,    0,    0,    0,  136,
  459,  138,    0,  460,  140,  141,  423,    0,    0,    0,
    0,    0,    0,  424,  944,    0,    0,  529,    0,    0,
  461,    0,  462,    0,    0,  425,    0,    0,    0,    0,
    0,  426,  427,    0,    0,  464,    0,    0,    0,    0,
  313,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
    0,    0,  429,    0,    0,    0,    0,    0,    0,    0,
  945,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  430,    0,  431,
  432,    0,    0,    0,    0,    0,    0,    0,    0,  433,
    0,    0,    0,  434,  112,    0,    0,  113,    0,  114,
  115,  116,  117,    0,  118,    0,    0,    0,    0,    0,
    0,    0,  119,  120,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  121,  122,    0,  123,    0,    0,    0,
    0,    0,    0,  124,  125,    0,  126,  127,    0,  128,
    0,    0,  129,  130,  131,  132,  133,    0,  435,    0,
  134,  135,    0,    0,  436,    0,  136,  137,  138,    0,
  139,  140,  141,    0,  437,    0,    0,    0,    0,    0,
    0,    0,  723,    0,    0,    0,    0,    0,  438,  439,
  440,  441,  236,  237,  113,    0,  114,  115,  116,  117,
    0,  118,  442,    0,    0,    0,    0,    0,    0,  119,
  120,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  121,  452,    0,  123,    0,    0,  453,  454,    0,  455,
  456,  457,    0,  126,  127,  458,  128,    0,    0,  129,
  130,  131,  132,  133,  423,    0,    0,  134,  135,    0,
    0,  424,    0,  136,  459,  138,    0,  460,  140,  141,
    0,    0,    0,  425,    0,    0,    0,    0,    0,  426,
  427,  529,  987,    0,  461,    0,  462,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  464,
    0,    0,    0,    0,  313,    0,    0,  428,    0,    0,
  429,    0,    0,   27,    0,    0,   27,    0,   27,   27,
   27,   27,    0,   27,    0,    0,    0,    0,    0,    0,
    0,   27,   27,    0,    0,  430,    0,  431,  432,    0,
    0,    0,   27,   27,    0,   27,    0,  433,    0,    0,
    0,  434,   27,   27,    0,   27,   27,    0,   27,    0,
    0,   27,   27,   27,   27,   27,    0,    0,    0,   27,
   27,    0,    0,    0,    0,   27,   27,   27,    0,   27,
   27,   27,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   14,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  435,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,  439,  440,  441,
  236,  237,  113,    0,  114,  115,  116,  117,    0,  118,
  442,    0,    0,    0,    0,    0,    0,  119,  120,  443,
  444,  445,  446,  447,  448,  449,  450,  451,  121,  452,
    0,  123,    0,    0,  453,  454,    0,  455,  456,  457,
    0,  126,  127,  458,  128,    0,    0,  129,  130,  131,
  132,  133,    0,    0,    0,  134,  135,    0,    0,    0,
    0,  136,  459,  138,    0,  460,  140,  141,  423,    0,
    0,    0,    0,    0,    0,  424,    0,    0,    0,  529,
    0,    0,  461,    0,  462,    0,    0,  425,    0,    0,
    0,    0,    0,  426,  427,    0,    0,  464,    0,    0,
    0,    0,  313,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  428,    0,    0,  429,    0,    0,  577,    0,    0,
  577,    0,  577,  577,  577,  577,    0,  577,    0,    0,
    0,    0,    0,    0,    0,  577,  577,    0,    0,  430,
    0,  431,  432,    0,    0,    0,  577,  577,    0,  577,
    0,  433,    0,    0,    0,  434,  577,  577,    0,  577,
  577,    0,  577,    0,    0,  577,  577,  577,  577,  577,
    0,    0,    0,  577,  577,    0,    0,    0,    0,  577,
  577,  577,    0,  577,  577,  577,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,    0,  436,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  438,  439,  440,  441,  236,  237,  113,    0,  114,  115,
  116,  117,    0,  118,  442,    0,    0,    0,    0,    0,
    0,  119,  120,  443,  444,  445,  446,  447,  448,  449,
  450,  451,  121,  452,    0,  123,    0,    0,  453,  454,
    0,  455,  456,  457,    0,  126,  127,  458,  128,    0,
    0,  129,  130,  131,  132,  133,  423,    0,    0,  134,
  135,    0,    0,  424,    0,  136,  459,  138,    0,  460,
  140,  141,    0,    0,    0,  425,    0,    0,    0,    0,
    0,  426,  427,  529,    0,    0,  461,    0,  462,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  464,    0,    0,    0,    0,  313,    0,    0,  428,
    0,    0,  429,    0,    0,  280,    0,    0,  280,    0,
  280,  280,  280,  280,    0,  280,    0,    0,    0,    0,
    0,    0,    0,  280,  280,    0,    0,  430,    0,  431,
  432,    0,    0,    0,  280,  280,    0,  280,    0,  433,
    0,    0,    0,  434,  280,  280,    0,  280,  280,    0,
  280,    0,    0,  280,  280,  280,  280,  280,    0,    0,
    0,  280,  280,    0,    0,    0,    0,  280,  280,  280,
    0,  280,  280,  280,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1191,    0,    0,  438,  439,
  440,  441,  236,  237,  113,    0,  114,  115,  116,  117,
    0,  118,  442,    0,    0,    0,    0,    0,    0,  119,
  120,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  121,  452,    0,  123,    0,    0,  453,  454,    0,  455,
  456,  457,    0,  126,  127,  458,  128,    0,    0,  129,
  130,  131,  132,  133,    0,    0,    0,  134,  135,    0,
    0,    0,    0,  136,  459,  138,    0,  460,  140,  141,
  423,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,  989,    0,  989,  989,    0,  989,    0,    0,  425,
    0,    0,    0,    0,    0,  426,  427,    0,    0,  989,
    0,    0,    0,    0,  989,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,    0,    0,  429,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,  431,  432,    0,    0,    0,    0,    0,
    0,    0,    0,  433,    0,    0,    0,  434,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1400,
    0,    0,  438,  439,  440,  441,  236,  237,  113,    0,
  114,  115,  116,  117,    0,  118,  442,    0,    0,    0,
    0,    0,    0,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,    0,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  131,  132,  133,  989,    0,
    0,  134,  135,    0,    0,  989,    0,  136,  459,  138,
    0,  460,  140,  141,    0,    0,    0,  989,    0,    0,
    0,    0,    0,  989,  989,  463,    0,    0,  461,    0,
  462,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  464,    0,    0,    0,    0,  313,    0,
    0,  989,    0,    0,  989,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  989,
    0,  989,  989,    0,    0,    0,    0,    0,    0,    0,
    0,  989,    0,    0,    0,  989,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  989,    0,    0,    0,    0,    0,  989,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  989,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  989,  989,  989,  989,  989,  989,  989,    0,  989,  989,
  989,  989,    0,  989,  989,    0,    0,    0,    0,    0,
    0,  989,  989,  989,  989,  989,  989,  989,  989,  989,
  989,  989,  989,  989,    0,  989,    0,    0,  989,  989,
    0,  989,  989,  989,    0,  989,  989,  989,  989,    0,
    0,  989,  989,  989,  989,  989,    0,    0,    0,  989,
  989,    0,    0,    0,    0,  989,  989,  989,    0,  989,
  989,  989,  423,    0,    0,    0,    0,    0,    0,  424,
    0,    0,    0,  529,    0,    0,  461,    0,  462,    0,
    0,  425,    0,    0,    0,    0,    0,  426,  427,    0,
    0,  464,    0,    0,    0,    0,  313,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  428,    0,    0,  429,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  430,    0,  431,  432,    0,    0,    0,
    0,    0,    0,    0,    0,  433,    0,    0,    0,  434,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  437,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  438,  439,  440,  441,  236,  237,
  113,    0,  114,  115,  116,  117,    0,  118,  442,    0,
    0,    0,    0,    0,    0,  119,  120,  443,  444,  445,
  446,  447,  448,  449,  450,  451,  121,  452,    0,  123,
    0,    0,  453,  454,    0,  455,  456,  457,    0,  126,
  127,  458,  128,    0,    0,  129,  130,  131,  132,  133,
  423,    0,    0,  134,  135,    0,    0,  424,    0,  136,
  459,  138,    0,  460,  140,  141,    0,    0,    0,  425,
    0,    0,    0,    0,    0,  426,  427,  989,    0,    0,
  989,    0,  989,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  989,    0,    0,    0,    0,
  989,    0,    0,  428,    0,    0,  429,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,  431,  432,    0,    0,    0,    0,    0,
    0,    0,    0,  433,    0,    0,    0,  434,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  438,  439,  440,  441,  236,  237,  113,    0,
  114,  115,  116,  117,    0,  118,  442,    0,    0,    0,
    0,    0,    0,  119,  120,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  121,  452,    0,  123,    0,    0,
  453,  454,    0,  455,  456,  457,    0,  126,  127,  458,
  128,    0,    0,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,    0,    0,    0,    0,  136,  459,  138,
  545,  460,  140,  141,  989,    0,    0,    0,    0,    0,
    0,  989,  561,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  989,    0,    0,    0,    0,    0,  989,
  989,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  545,  545,    0,    0,  545,    0,    0,    0,    0,  551,
    0,    0,    0,  561,    0,    0,  561,  989,    0,  545,
  989,  545,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  561,    0,  561,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  989,    0,  989,  989,    0,
  551,  545,    0,  551,    0,    0,    0,  989,    0,    0,
    0,  989,    0,  561,    0,    0,    0,    0,  551,    0,
  551,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  551,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  989,    0,    0,    0,
    0,    0,  989,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  989,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  989,  989,  989,  989,
  989,  989,  989,    0,  989,  989,  989,  989,    0,  989,
  989,    0,    0,  536,    0,    0,    0,  989,  989,  989,
  989,  989,  989,  989,  989,  989,  989,  989,  989,  989,
    0,  989,    0,    0,  989,  989,    0,  989,  989,  989,
    0,  989,  989,  989,  989,    0,    0,  989,  989,  989,
  989,  989,    0,    0,  536,  989,  989,  536,    0,    0,
    0,  989,  989,  989,    0,  989,  989,  989,  545,    0,
    0,    0,  536,    0,  536,    0,    0,    0,    0,    0,
  561,    0,    0,  545,    0,  545,  545,  545,    0,    0,
    0,  545,    0,    0,  545,  561,    0,    0,  561,  561,
    0,    0,    0,  561,  536,    0,  561,  545,    0,    0,
    0,    0,    0,  545,    0,    0,  545,  551,    0,  561,
    0,    0,    0,    0,    0,  561,    0,    0,  561,    0,
    0,    0,  551,    0,    0,  551,  551,    0,    0,    0,
  551,    0,    0,  551,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  551,    0,  545,    0,
    0,    0,  551,    0,    0,  551,    0,    0,    0,    0,
  561,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  545,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  561,    0,    0,    0,    0,    0,
    0,    0,    0,  545,    0,    0,    0,  551,    0,  545,
    0,    0,    0,  545,    0,  561,    0,    0,    0,    0,
  545,  561,    0,    0,    0,  561,    0,    0,    0,    0,
    0,  551,  561,    0,    0,    0,    0,    0,  545,    0,
    0,    0,    0,  545,    0,    0,    0,    0,    0,    0,
  561,    0,  551,    0,    0,  561,    0,    0,  551,    0,
    0,    0,  551,    0,    0,    0,    0,    0,  545,  551,
    0,  536,  545,    0,    0,    0,    0,    0,    0,    0,
  561,    0,    0,    0,  561,    0,  536,  551,    0,  536,
  536,    0,  551,    0,  536,    0,    0,  536,    0,  320,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  536,    0,    0,    0,    0,    0,  536,  551,    0,  536,
    0,  551,    0,    0,    0,    0,    0,  320,    0,    0,
    0,    0,    0,    0,  320,    0,    0,    0,  320,  320,
    0,  320,    0,    0,  320,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  320,    0,    0,
    0,  536,    0,  320,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  536,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1246,    0,    0,
    0,    0,    0,    0,    0,    0,  536,    0,    0,    0,
    0,    0,  536,    0,    0,    0,  536,    0,    0,    0,
  320,    0,    0,  536,    0,    6,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1452, 1453,  320, 1454,
    0,  536,    0,    0,    0,    0,  536,    0,    0,    0,
    0,    0,    0,  320,    0, 1455,    0,    0,    0,    0,
    0, 1456,    0,  320,    0,    0,    0,    0,    0,    0,
    0,  536,  320,    0,  320,  536,    0,    0,    0,    0,
    0,  320,    0,    0,  320,    0,  320,  320,  320,  320,
    0,  320,    0,    0,    0,    0,    0,    0,    0,  320,
  320,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  320,  320,    0,  320,    0,    0,    0,    0, 1457,    0,
  320,  320,    0,  320,  320,    0,  320,    0,    0,  320,
  320,  320,  320,  320,    0, 1246,   14,  320,  320,  320,
  320,  320,    0,  320,  320,  320,    0,  320,  320,  320,
    0, 1458,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   16,    0,    6,    0,    0,    0,    0,    0,    0,
 1660,    0,    0,    0, 1452, 1453,    0, 1454,    0,  441,
    0,    0,  113,    0,  114,  115,  116,  117,    0,  118,
    0,    0,    0, 1455,    0,    0,    0,  119,  120, 1456,
    0,    0,    0,    0,    0,    0,    0,    0, 1459,  122,
    0,  123,    0,    0,    0,    0,    0,    0,  124,  125,
    0,  126,  127,    0,  128,    0,    0,  129, 1460,  131,
  132,  133,    0,    0,    0,  134,  135, 1461, 1462, 1463,
    0,  136,  137,  138,    0,  139,  140,  141,    0,    0,
    0,    0,    0,    0,    0,    0, 1457,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   14,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1458,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   16,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  441,    0,    0,
  113,    0,  114,  115,  116,  117,    0,  118,  627,    0,
    0,  628,    0,    0,    0,  119,  120,  829,    0,    0,
    0,    0,    0,    0,    0,    0, 1459,  122,  629,  123,
    0,  630,    0,    0,    0,    0,  124,  125,    0,  126,
  127,    0,  128,    0,    0,  129, 1460,  131,  132,  133,
    0,    0,    0,  134,  135, 1461, 1462, 1463,    0,  136,
  137,  138,    0,  139,  140,  141,    0,    0,    0,    0,
    0,  631,    0,    0,    0,    0,    0,  632,  633,  634,
    0,  635,  636,  637,    0,  638,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,    0,  640,  627,    0,    0,  628,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  629,    0,    0,  630,
    0,    0,    0,    0,  641,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  642,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  631,
    0,    0,    0,  643,    0,  632,  633,  634,    0,  635,
  636,  637,    0,  638,    0,    0,    0,    0,  112,    0,
    0,  113,    0,  114,  115,  116,  117,    0,  118,    0,
    0,  639,    0,  640,    0,    0,  119,  120,  644,  645,
    0,    0,    0,    0,    0,    0,    0,  121,  122,    0,
  123,    0,    0,    0,    0,  646,    0,  124,  125,    0,
  126,  127,  641,  128,    0,    0,  129,  130,  131,  132,
  133,    0,    0,    0,  134,  135,    0,    0,    0,  642,
  136,  137,  138,    0,  139,  140,  141,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  643,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  112,    0,    0,  113,
    0,  114,  115,  116,  117,    0,  118,    0,    0,    0,
    0,    0,    0,    0,  119,  120,  644,  645,    0,    0,
    0,    0,    0,    0,    0,  121,  122,    0,  123,    0,
    0,    0,    0,  646,    0,  124,  125,    0,  126,  127,
    0,  128,    0,    0,  129,  130,  131,  132,  133,    0,
    0,    0,  134,  135,    0,    0,    0,    0,  136,  137,
  138,    0,  139,  140,  141,  112,    0,    0,  113,  185,
  114,  115,  116,  117,  186,  118,  187,  188,  189,  190,
  191,  192,  193,  119,  120,  194,  195,  196,  197,  198,
    0,    0,  199,  200,  121,  122,  201,  123,    0,    0,
  202,  203,  204,  205,  124,  125,  206,  126,  127,  207,
  128,  208,  209,  129,  130,  131,  132,  133,    0,    0,
    0,  134,  135,  210,  211,  212,  213,  136,  137,  138,
  214,  139,  140,  141,  461,    0,    0,  461,  461,  461,
  461,  461,  461,    0,  461,  461,  461,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,  461,  461,    0,
    0,  461,  461,  461,  461,  461,  461,    0,    0,  461,
  461,  461,  461,  461,  461,  461,  461,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,    0,    0,    0,
  461,  461,  461,  461,  461,  461,  461,  461,  461,  461,
  461,  461,  461,
};
static short yycheck[] = {                                       9,
    0,    4,   68,  111,  169,   60,   16,  564,   18,   58,
  159,  160,   66,   74,   16,   67,  159,  160,   73,   77,
   99,  747,   67,   96,   90,  401,   59,  401,  397,  299,
  499,  371,  747,  997,   62,  567,  393,   98,   71,  369,
  596,  540,  100,  803, 1245,   94,   56,   57,   58,   59,
   60,   61,   62, 1318,    0,  709,    0,   67,   68,   69,
 1011,   71,   95,   73,   74,   93,   76,   77,   78,    0,
  103, 1246,  757,  101,   72,  750,   86,  519,   88,   89,
   90,  735,   92,   93,   94,   95,   88,   97,   98,   99,
  100,  101,  518,  103,  173,   97,  948,  152,   96,  315,
  377, 1031,   41,  723,  463,  672,  151,  378,  153,  729,
 1135, 1072,    0,   59,  613,   59,   45,  172,    0,  344,
  589,   40,  591, 1113,  679, 1072,  801,  596,   59,    0,
   44,  177,   40,  985, 1072,  406,  164,   44,  171,    0,
    0,  151,  152,  153,    0,    0, 1072,  157,  250,   40,
  901,  259,    0,    0,  164,   63,   40, 1072,    0,  169,
  808,  171,  172,  173,  218,    0,   63,  177,   41,    0,
   41,   59,   63,   44,    0,  823, 1079,   59,  826,  344,
   41,   44, 1072,   44,   41,   41,   41,  835,   59,   44,
    0,    0,  246,   41,   41,    0,   44,   44,   59,   59,
    0,   45, 1276,   59,   59,   41,  575,   44,    0,   44,
 1264,   59,   59,   44,    0,    0,   41,   59, 1310, 1311,
  596,   44,  596, 1315,   59,    0,   45, 1130,   59,   45,
   45,   41,  344,   59,  273,  317,   41,    0,   44, 1016,
  510,  349,   44,  345,    0,  290,  894,   63,  293,   59,
   59,  686,    0,  283,   59,  624,  526,   91,    0,   59,
 1461, 1462,  297,   44,    0,    0,   41,   59,    0,  599,
 1537,  997,   44,   59,   59,  330,  298,  326,   41, 1316,
  290,   61,  997,  293,   59,  395,  298,   40,  559,  299,
  300,  322,   45,   41,  262,  275,   59,  324, 1473,  344,
  264,   41, 1583,   59,   44,  257,   41,  280,  264,  344,
  409,   59,  334,  298,  262, 1582,  326,   59,  648,   44,
  330,  941,  942,   59,   59,  370,  447,   59,  300,  263,
  380,  395,  377,  310,  379,  334,  381,  436,  317,  384,
  394,  397,  396,  404,  463,  257,  428, 1277,    0, 1310,
 1311,  313,  314,  518, 1315,    0,  410,  412,  978, 1349,
  370,  341, 1313, 1310, 1311,  414,  297,  377, 1315,  379,
  403,  381, 1310, 1311,  384,  309,  348, 1315,  330,  878,
  739,  498,  432,  852, 1310, 1311,  320,  746, 1291, 1315,
  500,  422,  448,  403,  404, 1310, 1311,  509,  429, 1077,
 1315, 1055,  412,   61,  414, 1356, 1084,   59,  418, 1057,
  529,  379,  434,  344,   59,  418,  258, 1095,  330,  458,
 1310, 1311,  434,  453,  454, 1315, 1074, 1075,  270,  456,
   41,  379,  409,   44,  380,  434,  380,  297, 1290,  403,
 1118,  275, 1516, 1708,  315,  389, 1094,  403,  557,  434,
  514, 1071, 1653,  508,  464,  297,   41,   41,  431,  315,
  902,  303,  517,  305,  301,  336,  521,  315,  315,  376,
  306,  913,  615,  518,  509,  917,  509,  325,   44,  327,
  336,  306,  528,  918,  344,  419,    0,  817,  336,  336,
  820,  339,  264, 1270, 1271,  301,  766,    0,  508,  509,
  510,  831,  344,  387,  272,  315,  379,  517,  264,    0,
  315,  521, 1549,  315, 1240,  376,  526,  297,  528,  564,
  439,  450,  379,  577,  380, 1240,  336,   41,  858, 1810,
  444,  336,  380,  389,   41, 1155, 1156, 1802, 1158, 1159,
  315,  389,  450,  616,  344,   59,  428,  824,  477,  380,
   41,  596,  315,  450,  564, 1580,   59, 1242,  389,  450,
  662,  336, 1129,  779,  618,  905, 1241,  513,   59,  513,
  380,    0,  691,  336,  513,  446,  432,  802,  509, 1844,
 1137, 1532,  747,  446,  432,  595,  596, 1142,  336, 1031,
  446,  601,  317,  402,  380, 1021,  444, 1527,  446,  446,
 1865,  336, 1592,  389, 1355,  754,  450,  423,  344,    0,
    0,  446,   41,  444,  668,  669,  401, 1059,  616,  371,
  446,  892,  432,  494,  410,  513,  671,  450, 1682,  361,
   59,  450,  493,  477,  450,  450,  446,  802,  493,  297,
    0,  446,  513,    0,  787,  297,    0,  428,   42,  509,
   41,   41,   59,   44,  446, 1333, 1831,  513,  444, 1562,
  402,  671,  273,  403, 1023,  513,  513,  509,   59,   59,
  423,  446,    0, 1765, 1766,  784, 1579,  262,  262, 1299,
 1855,   41,  513,  446,   44,    0,  714,   41,  440,    0,
   44,  352,  344, 1657, 1895,  306,  262,  450,  446,   59,
  352, 1321,   59,  513,  714,   59, 1354, 1882, 1065,  273,
  755,  446,  757,   41,  759,  315,  361,  860, 1893,  774,
 1526, 1896,    0,    0,  315,  297,   41,  513, 1903,   44,
  740,   59, 1386,   93,  386, 1265,  388,  348,  513,  268,
  750,  796,  775,  798,   59,  755, 1598,  757,   59,  759,
  513, 1081,  315, 1083,  262,  262,  766,  802,  410,  813,
  421,    0,  807, 1093,  774,  775,    0,  298,  262, 1720,
 1530,  276,  782,  783, 1131, 1726,  348,  850,   93,  824,
  401,   59,   59,  435,  348,    0,  796, 1319,  798,  305,
    0,  801,  403,  351,  379,  379,    0,  807,  395,  315,
 1140,  315,  261,  334, 1765, 1766,  851,  852, 1167, 1339,
  316,  316,  866,  379,  824,  888,  889,  890, 1765, 1766,
   59,  305,  336,  390,  315,   59,   41, 1765, 1766,   44,
  359,  315,  997,  430,  325,  338,  327,    0,  893, 1765,
 1766,  851,  852, 1342,   59,  336, 1345,  458,  903,   59,
 1765, 1766,  850,   41,  445,   59, 1021,  509,  868,  857,
 1824,  428, 1826, 1541, 1420,  868,  260, 1545, 1546, 1209,
 1834,  379,  379,   41, 1233, 1765, 1766,  875,    0,    0,
 1686,    0,  445,  893,  397,  379,  315,  393,  393,  380,
  888,  889,  890,  903,  397,  462,   59,  400,  389,   41,
   44,  911,  912, 1581,    0,    0,  300,  336,  911, 1529,
  838,    0,    0,  416, 1223, 1879, 1275, 1226,   44,  427,
   41,  272,  276,  277,  315,  315,  484,  433,  433,  445,
  284, 1657,  446,  427,  325,  448,  327,   59,   59,  442,
   59,  432, 1657,    0,  265,  336,  336,  258,  339,   93,
    0,  380,   41,  444,  982,  446,    0,  686,  450,  270,
  389,  445,  691,   59,   59,  484,    0,   93,    0,    0,
   59,   59,  982, 1116,  304,  305, 1021, 1446,  299, 1249,
  344,  338, 1252,  318,   41,  336,  297,   44,    0,  380,
 1359,    0,  303,  457,  305,  459,  331,  516,  389,  276,
  277, 1331,   59,  432,    0, 1015, 1016,  339,  336,   59,
  304,  305,  333,  450,  371,   59, 1694,  446,  372,  354,
 1698, 1031,  513,    0,    0,   59, 1646,   59,   59,   41,
  362, 1361,   44,  344,    0, 1713,  319,  276,  277,  393,
  397,  432,  402,  400, 1420,  399, 1420,   59, 1167, 1473,
   59, 1124, 1730,  444,  264,  446,  446,  396,    0,  416,
  264,  276,  277,   59,  319,   41, 1139,  284,   44,  284,
  405,    0, 1145,  412,  262, 1240,  258,  304,  305,  433,
  297, 1246,   59,   59,  513,  442,  418,  402, 1133,  453,
  454,    0, 1137,   59,  262, 1105,  259,    0, 1824,   41,
 1826,  264,  260,    0,  338,  394,  269, 1110, 1834, 1824,
  265, 1826, 1122,  443, 1263, 1264, 1265,   59,  446, 1834,
  262,  303,  513, 1133,   44, 1434, 1124, 1137,    0, 1438,
   59, 1809,   41,  372, 1812,   44,  258,  371,   41,  493,
  371, 1139,  300,  376,  299,  264,    0, 1145,  270,  443,
   59,    0, 1295, 1879, 1297,   41,   59,  372,   44, 1087,
 1088,  376,   59,  397, 1879, 1785,  400,   40,  264,  264,
 1848, 1791, 1836,   93, 1253,  297,  264,  300,  393,  410,
  390,  303,  416,  305,  399,  914,  390,   59,   40,  918,
 1251,  379,   41, 1245,  315, 1205, 1254, 1242,  509,  376,
 1245, 1246,  372, 1205,  325,   59,  327,    0,  442, 1137,
   59,  379,  260,   44, 1357,  336,  443, 1250,  433, 1883,
  264, 1551,  344,  393,  259,  348,  457,  390,  410,  399,
  352, 1241, 1242,  264,  269, 1245, 1246,  379,  397, 1249,
 1250, 1251, 1252, 1253, 1254,  485,  428,  336,   41,  372,
  262,   44,  300,    0,  450,  495, 1586,  416,  315,  380,
 1270, 1271,  258,  433, 1383, 1384,   59, 1277,  389,   44,
  327,  390, 1391, 1392,  270, 1394, 1395,  378,  493,  336,
  462,  477, 1839, 1402, 1403, 1404, 1405,  264, 1407, 1408,
 1409, 1410, 1411, 1412,  390,  390,  378,   44,    0,  421,
  276,  277,  390,  315,  338,  260,  318,  303, 1473,  305,
  260,  432,   59,  325,  271,  327, 1370,    0,   93,  331,
  450, 1375,  409,  380,  336,  446, 1635,  339,  285,    0,
   41,    0,  389,   44, 1337,  264,  338,  371,   42,   43,
    0,   45,  354,   47,  270,  300,  390,  477,   41,  436,
  300,   44,  278,  428, 1519,   42,  368,   59,   41,  390,
   47,   44,    0,  397,  376,  333,  400,  379,  380,  371,
    0,   41,  329,  315,   44,  432,   59,  389,  289,  280,
    0,   41,  416,  325,   44,  327, 1452,  509,   59,  446,
   59,  410,  513,  405,  336,  397,  372, 1727,  400,   59,
  357,  312,  486, 1413,  276,  277,  315, 1831,  442,  318,
  321, 1413,  315,  390,  416,  427,  325,  393,  327,  434,
  432,   59,  331,  399,  327,  260,  446,  336, 1473,   59,
  339, 1855,  444,  336,  446,  260,    0,  273,  380,   59,
  442,  338, 1452,    0,  355,  354,  513,  389,  359,  318,
 1460, 1461, 1462,   40,    0,    0,  513,  433, 1882,  368,
  520,  390,  331, 1473,  421,  300,  315,  376,    0, 1893,
  339,  380, 1896,    0,  371,  300,  377,  380,  327, 1903,
  389,  493,    0,  276,  277,  354,  389,  336,  416,    0,
  432,  284, 1657, 1503, 1504,   59,  405, 1552,  298,  368,
  397,  513,   59,  400,  446,  517,    0,   42,   43, 1519,
   45,  264,   47,   59,   59,    0, 1526, 1527, 1683,  416,
  453,   42,   43,  432,   45,  258,   47,   59,  261,  432,
  431,  380,   59, 1682,  334,  444,  405,  446,  315,  259,
  389,   59, 1552,  446,  455,  442,  318,  448,   59,  269,
  461,  462,  463,   42,   43,  352,   45,  313,   47,  331,
    0,  316,  277,   42,   43,   59,   45,  282,   47,  324,
  303,  513,   41,  277,   59,   44, 1625,    0,  282,  372,
  485,  486,  354,  432,  493,    0,    0,    0,  333,  386,
  277,  388,  452,  276,  277,  282,  368,  446,  281,  399,
  393,  284,   41, 1711,  513,   44,  399,  333,  517,  409,
  513,  258, 1622,  410,  297, 1625,  276,  277,  529,   59,
 1622,  281,  533,  270,  284,  444,   41,   41,   41,   44,
   44,   44,   41,  405,  434,   44,   59,  297,  435,  403,
  433,  395,  368, 1653,   59,   59,   59,    0,  517,  356,
   46,  358, 1707, 1280,  380, 1282,  303,  332,  305, 1824,
    0, 1826,   58, 1712,  513,   61, 1831,  291,  333, 1834,
  453,  454,  426, 1683, 1674,  426, 1686,   41,  302, 1733,
   44,   42,   43, 1737,   45,  309,   47,  441,   41,  372,
 1855,   44, 1702,  384,  385, 1803,    0, 1707,  395,  336,
  493,   41, 1712,  368,   44, 1850,   59, 1852,   46,  333,
  393, 1777,  372,   40, 1879,  380,  399, 1882,  317,   59,
   58,   59,  315,   61,  428,  449,  450,  451, 1893,   41,
  315, 1896,   44,  393,  453,  454,   40,   41, 1903,  399,
   44,   41,  277,   41,   44,  517,   44,  282,  376, 1759,
  433,   46,  498,  477,  478,   59,  277,   61,  376,   41,
  428,  282,   44,   58,   59,  356,   61, 1777, 1778,   42,
   43,  395,   45,  433,   47,  710, 1779,  712,  291,  442,
  691,  716,  717,  718, 1503, 1504, 1831,   91,  277,  302,
 1503, 1504,   41,  282, 1839,   44,  309,  426,  277,  377,
  711,  336,  426,  282,  715,   41,  430,  375,   44,  322,
 1855,  722,  497,  409,  508,  336,  727,  728,  508,  308,
  509, 1831,   41, 1823,   41,   44, 1836,   44,  739, 1839,
  741,  742,  743,  744,  745,  746,  315, 1882,  749,  284,
 1850,   41, 1852,   41,   44, 1855,   44,   41, 1893,  262,
   44, 1896,   42,   43,  461,   45,   41,   47, 1903,   44,
   44,  276,  277,    0,   41,  446,  379,   44,   61,  284,
  366,  283, 1882, 1883,  785,   40,  264,   40,   40,  389,
   40,   40,  395, 1893,  453, 1895, 1896,   40,   40,   40,
    0,   40,   40, 1903,  805,  519,   40,  453,  411,  453,
  811,  812,  315,  351,   41,  318,   40,   44,   40,   40,
   40,   40,  325,  426,  327,   91,  277,  430,  331,  402,
  433,  282,   59,  336,   44,   46,  339,   40,  441,  264,
  395,   41,  317,   61,   44,   44,  276,  277,   44,  420,
   40,  354,   46,   61,  284,   42,   43,  460,   45,   59,
   47,   61,   44,  264,  258,  368,  317,  372,  344,  276,
  344,  344,  276,  376,    0,  297,  379,  380,  264,  273,
  390,  275,  276,  277,  428,  462,  389,  281,  393,  390,
  284,  344,  450,  275,  399,   41,   42,   43,  439,   45,
  439,   47,  405,  297,   41,   42,   43,  392,   45,  303,
   47,   40,  306,  345,  277,   41,  519,   40,   44,  282,
   91,   40,   40,  315,  427,  926,  927,  928,  433,  432,
  931,  932,  933,   59,  935,  936,  937,  938,  939,  940,
  264,  444,  372,  446,  403,  308,   44,  376,  949,  950,
   40,  952,  375,  954,  348,  382,  957,  958,  959,  960,
  961,  962,   40,  393,  380,  315,   41,   42,   43,  399,
   45,  443,   47,  974,    0,  976,   42,   43,  372,   45,
   41,   47,  275,    0,  264,  296,   63,  428,  493,  428,
  493,  456,  314,   44,   40,   42,   43,  277,   45,  393,
   47,  264,  282,  433,  444,  399,  440,   44,   40,  403,
  513,  494,  273,   40,  517,   41,  410,  281,   44,    0,
  297,  340, 1023,   40,   41,   42,   43,   44,   45,   46,
   47,  276,  340,   59,  428,  262,  379,  379,  379,  433,
  410,   58,   59,   60,   61,   62,  408,   40,  352,  410,
   40,   40,  362,   41,   42,   43,   40,   45,  258,   47,
   41,   41,   44,   44,  458,  277,  317,   44,  462,  438,
   46,  317,  270,  273,   91,  378,   93,  264,   59,  361,
  464,  465,  466,  467,  468,  469,  470,  471,  315,  317,
  277,  318,   40,   44,  325,  282,  266,  297,  325,   40,
  327,   40,  493,  303,  331,  264,  306,   41,  414,  336,
   41,   58,  339,   41,   42,   43,   41,   45,   41,   47,
  264,  446,  387,  306,   41,   42,   43,  354,   45,  306,
   47,  277,   41,   41,   42,   43,  282,   45,   41,   47,
  277,  368,   40,   40,  317,  282,   40,    0,  348,  376,
  276,  277,  379,  380,   40,   40,   40,   40,  284,   41,
   42,   43,  389,   45,   40,   47, 1167,   40,  931,   40,
  933,  297,  935,  936,   40,  938,  939,  940,  405, 1180,
 1181,   40, 1183, 1184,   40,   40,   40,  427,   41,   40,
 1191,   44,  262,  305,   44,   44,  262,   41,  446,   41,
  427,  264,  277,  403,  339,  432,   59,  282,   61,  381,
  410,  277, 1213,  273,  339,  384,  282,  444,  265,  446,
 1221, 1222, 1223,  327,  399, 1226,  284, 1228,  428,  379,
  277, 1232, 1233, 1234,  379,  282,  379,  379,   40,  379,
    0,  379,   41,  450,  372,  262,  372,  264,  265,   41,
  276,  277,  299,  276,  271,   40,   58,   41,  458,   41,
  277,   41,  462,  317,   41,  282,  493,  393,  285,   44,
   41,  305,  427,  399, 1275,  290,   59,   40,   59,  336,
   59,   41,  299,   59,   44,   59,  513,  304,  305,  277,
  517,  308,   59,  336,  282,  276,  277,   59,  315,   59,
  317,  318,  305,  284,  321,   59,   59,  433,  325,   59,
  327,   59,  329, 1314,  331,   59,  297,   59,   59,  336,
  337,   59,  339,   41,   42,   43,   59,   45,   59,   47,
   58,  428,  324,  339,  277,  333,  298,  354,  355,  434,
  357,   41,   42,   43,  298,   45,  372,   47,  334,  277,
  434,  368,   41,  370,  282,  372,  373,  374,   41,  376,
  277,  334,  379,  380,  378,  282,  365,  393,  365,  277,
  348,   41,  389,  399,  282,   41,   41,   42,   43,   41,
   45, 1382,   47,   41,   41,  402,   41, 1388,  405,   41,
   41,  372, 1393,   41,   41,  277,   41,   41,   41, 1400,
  282,   41,   41,   41,  421,  258,   40,  433,   41,  315,
  427,  428,  393,  443,   40,  432,   44,  378,  399,   41,
  340,  348,  275,  276,  277,  458,  443,  444,  281,  446,
   40,  284,  333, 1434,  336,  452,  500, 1438,  455,   40,
  457,  458,  459,  460,  297,  462,  336,   40,   44,   41,
  303,   40,  433,  470,  471,  376, 1457,  427,  376,   41,
  376,   41,  301,    0,  481,  482,  298,  484,   42,   43,
   44,   45,   41,   47,  491,  492,  493,  494,  495,  434,
  497,  304,  290,  500,  501,  502,  503,  504,  301,  455,
    0,  508,  509,  333,  409,  376,  513,  514,  515,  516,
  517,  518,  519,  520,   41,   42,   43,   44,   45,   46,
   47,   41,   42,   43,  376,   45,   40,   47,  264,  372,
  276,  264,   59,   60,   61,   62,  450, 1528,  317,   41,
   42,   43,  462,   45,  403,   47,  443,   44,  270,   40,
  393,   41,   42,   43,   41,   45,  399,   47,  270,  264,
  450,  428,  427,  339,   91,  315,   93,  410,  318,  277,
   41,  339,  450,   41,  282,  325,   59,  327,   41,    0,
   59,  331,   59,   59,   41,  428,  336,  277,  305,  339,
  433,   41,  282,   59,   59,   41,   42,   43,   59,   45,
   59,   47,  301,   59,  354,    0,    0,    0,    0,  156,
 1601, 1261,  524,  156,  301,  157, 1607, 1608,  368,  462,
   41,  775,  277,   44,  526,  300,  376,  282,  766,  379,
  380,   41,   42,   43, 1142,   45,  675,   47,   59,  389,
 1631,   41,   42,   43, 1635,   45,  906,   47, 1639,   92,
 1140,   78, 1643, 1644, 1137,  405,   42,   43,   44,   45,
  452,   47,  911,  455,  868,  457,  458,  459,  460,  882,
  462,   42,   43,   44,   45, 1132,   47,  427,  470,  471,
 1569,  367,  432,   42,   43, 1570,   45,  398,   47,  481,
  482, 1068,  484,  347,  444, 1122,  446,  624, 1133,  491,
  492,  807,  494,  495,  833,  497, 1702,  379,  500,  501,
  502,  503,  504,  277,  379, 1842,  508,  509,  282, 1549,
  601,  260,  514,  515,  516, 1846,  518,  519,  520, 1551,
 1012, 1867, 1845,  824, 1460,  262, 1662,  264,  265, 1832,
  755, 1732, 1293,  493,  271, 1533,  372, 1346,  384,  603,
  277,  365,  661,  877,  885,  282,  334,  277,  285,  298,
   13,  542,  282,  513,   41,   42,   43,  517,   45,  627,
   47,  310,  299,    0,  301,  277,  785,  304,  305,  162,
  282,  308,  796,  546, 1031, 1281,   88,  277,  315, 1796,
  317,  318,  282, 1622,  321,  334,   42,   43,  325,   45,
  327,   47,  329, 1413,  331, 1796,  557,  708,  811,  336,
  337,  773,  339, 1182,   41,   42,   43,   44,   45,   46,
   47, 1024, 1301, 1228,   -1,   -1,   -1,  354,  355,   -1,
  357,  277,   59,   60,   61,   62,  282,   -1,   -1,   -1,
   -1,  368, 1833,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,  276,  277,   -1,   -1,   -1,
  399,   -1,  389,  284,   91,   -1,   93,  277,   -1,   -1,
  409,  318,  282,   -1,   -1,  402,   -1,  277,  405,   -1,
   -1,   -1,  282,   -1,  331,   -1,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,  421,  434,  282,   -1,   -1,   -1,
  427,  428,   -1,  262,   -1,  432,  277,  354,   -1,   -1,
   -1,  282,   -1,   -1,   -1,   -1,  443,  444,  277,  446,
   -1,  368,   -1,  282,   -1,  452,   -1,   -1,  455,  376,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,  372,   -1,   -1,  481,  482,   -1,  484,  405,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,  393,  500,  501,  502,  503,  504,  399,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1, 1383, 1384,   -1,   -1,   -1,
  277,   -1,   -1, 1391, 1392,  282, 1394, 1395,   -1,   -1,
   -1,   -1,  433,   -1, 1402, 1403, 1404, 1405,   -1, 1407,
 1408, 1409, 1410, 1411, 1412,   -1,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,  262,  282,  264,  265,   -1,
   -1,   -1,   -1,   -1,  271,  302,  493,   -1,   -1,   -1,
  277,   -1,  309,   -1,   -1,  282,  313,  314,  285,   -1,
   -1,   -1,   -1,   -1,    0,  322,   -1,   -1,   -1,   -1,
  517,   -1,  299,    0,   -1,   -1,  333,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,   -1,   -1,  395,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,  411,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,  426,
   -1,   -1,   -1,  430,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,  460,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,  519,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,  275,
  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,  297,   -1,   -1,    0,   -1,   -1,  303,   -1,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   41,   -1,   -1,   44,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   59,   -1,   61,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,  389,  399,   91,   -1,   93,   -1,   -1,   -1,
   -1,   -1,  408,   -1,  410,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
  276,  277,   -1,   -1,  271,  281,   -1,   -1,  284,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,  297,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,  372,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,  393,   -1,   -1,
   -1,   -1,  389,  399,   91,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  410,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,  421,   -1,   -1,  433,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,  462,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   91,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  281,
  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  262,   -1,  264,  265,  491,
  492,   -1,  494,  495,  271,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,  282,  508,  509,  285,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,   -1,  262,   -1,  264,  265,  491,
  492,   -1,  494,  495,  271,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,  285,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  299,    0,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   45,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  261,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,  279,   -1,  271,   -1,   -1,   -1,   -1,   -1,
  287,   -1,   -1,   -1,   -1,   -1,   -1,  294,  285,  296,
   -1,  298,    0,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,   -1,  299,  310,   -1,   -1,   -1,  304,  305,   -1,
   -1,  308,   -1,   -1,   -1,   -1,   -1,  324,  315,   -1,
  317,  318,   -1,   -1,  321,   -1,   -1,  334,  325,   -1,
  327,   -1,  329,   41,  331,   43,   44,   45,   -1,  336,
  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   93,   -1,   -1,  292,   -1,
  397,   -1,  389,   -1,   -1,   -1,   -1,  404,   -1,   -1,
  407,   -1,  409,  410,   -1,  402,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,  434,   -1,   -1,
  427,  428,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  348,   -1,  443,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,  483,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,  498,  499,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,  419,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,   -1,  453,
  454,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  285,   -1,   -1,
   -1,    0,   -1,  487,  488,   -1,   -1,   -1,   -1,   -1,
   -1,  299,  496,   -1,   -1,   -1,  304,  305,   -1,   -1,
  308,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,
  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   43,   44,   45,   -1,  336,  337,
   -1,  339,  272,   -1,   -1,  275,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  354,  355,  288,  357,
   -1,   -1,  292,   -1,   -1,  295,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  402,  335,   -1,  405,   -1,   -1,
   -1,  341,  342,  343,   -1,  345,  346,  347,   -1,  349,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
  428,   -1,   -1,   -1,  432,   -1,   -1,  367,   -1,  369,
   -1,   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,  398,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  415,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,  437,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  489,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  299,   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,
   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,
   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,
  329,   41,  331,   43,   44,   45,   -1,  336,  337,   -1,
  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,
  379,  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,
   -1,   -1,  491,  492,  493,  494,  495,    0,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   59,   -1,   -1,   -1,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  304,  305,   -1,   -1,  308,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,
   -1,  321,   -1,   -1,   -1,  325,   -1,  327,   -1,  329,
   41,  331,   43,   44,   45,   -1,  336,  337,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,  354,  355,   -1,  357,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,
  370,   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,
  380,   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  428,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,  262,
  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,  493,  494,  495,   -1,  497,    0,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,  513,  514,  515,  516,  517,  518,  519,
  520,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   41,
    0,   -1,   44,  336,   -1,   -1,  339,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   59,   -1,   -1,
  271,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  285,  368,   -1,   -1,   -1,   -1,
   40,   41,   -1,  376,   44,   -1,  379,  380,  299,   -1,
   -1,   -1,   -1,  304,  305,   -1,  389,  308,   -1,   59,
   -1,   -1,   -1,   -1,  315,   -1,  317,  318,   -1,   -1,
  321,   -1,  405,   -1,  325,   -1,  327,   -1,  329,   -1,
  331,   -1,   -1,   -1,   -1,  336,  337,   -1,  339,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,  354,  355,   -1,  357,   -1,   42,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,  368,   -1,  370,
   -1,  372,  373,  374,   -1,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  402,   -1,   -1,  405,   -1,   -1,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  421,   -1,   -1,    0,   -1,   -1,  427,  428,   -1,   -1,
  513,  432,   -1,   -1,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,  444,   -1,  446,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   40,   41,   -1,   -1,   44,   -1,  470,
  471,   -1,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   59,  484,   -1,   -1,   -1,   -1,   -1,   -1,
  491,  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,  513,  514,  515,  516,  517,  518,  519,  520,
   -1,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,   -1,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,   -1,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,  325,  368,  327,   -1,   -1,
   -1,  331,   -1,   -1,  376,   -1,  336,   40,  380,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,   -1,  293,
   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,   -1,   -1,
  380,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,  389,
  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,  446,  405,   -1,   -1,   -1,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   41,   -1,   -1,   44,   -1,  470,  471,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   59,  484,   -1,  444,   -1,  446,   -1,   -1,  491,
  492,  493,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,  513,  514,  515,  516,  517,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,  493,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,  513,   -1,   -1,   -1,  517,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,  354,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,  481,  482,  376,
  484,   -1,   -1,  380,   -1,   -1,   -1,  491,  492,   -1,
  494,  495,  389,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,  405,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,
   -1,   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,    0,   -1,   -1,  513,  514,  515,  516,
  517,  518,  519,  520,   -1,   -1,   -1,  315,   -1,   -1,
  318,   -1,   -1,   -1,   -1,   -1,  409,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,  452,
  368,   -1,  455,   -1,  457,  458,  459,  460,  376,  462,
   -1,   -1,  380,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  389,   -1,   -1,   91,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,  405,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,  432,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,   -1,
   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,  493,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,  513,  514,  515,  516,  517,
  518,  519,  520,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   91,  271,   -1,   -1,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  325,   -1,
  327,   -1,  329,   -1,  331,   -1,   -1,   -1,   -1,  336,
  337,   -1,  339,   -1,   -1,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
  357,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  389,   -1,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,   41,   -1,
  427,   44,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   59,  444,   -1,  446,
   -1,   -1,   -1,  271,   41,   -1,   -1,   44,   -1,  277,
   -1,   -1,   -1,   -1,  282,   -1,   -1,  285,   -1,   -1,
   -1,    0,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  493,  315,   -1,   -1,
  318,   -1,   -1,  321,   91,   -1,   -1,  325,   -1,  327,
   -1,  329,   41,  331,   -1,   44,  513,   -1,  336,  337,
  517,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  357,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,  370,   -1,  372,  373,  374,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,  389,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,  282,  405,   -1,  285,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  315,
   -1,   -1,  318,    0,   -1,  321,  444,   -1,  446,  325,
   -1,  327,   -1,  329,   41,  331,   -1,   44,   -1,   -1,
  336,  337,   -1,  339,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,  357,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  258,  368,   -1,  370,  493,  372,  373,  374,   -1,
  376,   -1,   59,  379,  380,   -1,  273,   -1,  275,  276,
  277,   -1,   -1,  389,  281,  513,   -1,  284,   -1,  517,
   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,  405,
  297,   -1,  325,   -1,  327,   -1,  303,   -1,  331,  306,
   -1,   -1,   -1,  336,   -1,  421,  339,   -1,   -1,   -1,
   -1,  427,   -1,  262,   -1,   -1,  432,   -1,   -1,   -1,
   -1,  354,   -1,   -1,   -1,   -1,    0,   -1,  444,   -1,
  446,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,   -1,
   -1,  348,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,  315,   41,   -1,  318,
   44,   -1,  405,   -1,   -1,   -1,  325,  493,  327,   -1,
   -1,   -1,  331,   -1,   -1,   59,  393,  336,   -1,   -1,
  339,   -1,  399,   -1,  427,   -1,  403,  513,   -1,  432,
   -1,  517,   -1,  410,   -1,  354,   -1,   -1,    0,   -1,
   -1,  444,   -1,  446,   -1,   -1,   -1,   -1,   -1,  368,
   -1,  428,   -1,   -1,   -1,   -1,  433,  376,   -1,   -1,
  379,  380,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,  458,   44,   -1,   -1,  462,  405,   -1,   -1,   -1,
  493,   -1,   -1,   -1,   -1,  262,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
  513,   -1,   -1,  432,  517,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,  444,   -1,  446,  325,   -1,
  327,   -1,    0,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,   -1,   -1,  354,  325,   -1,
  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,  368,  339,   41,  493,   -1,   44,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,  354,   -1,   -1,
   -1,   59,  389,   -1,  513,   -1,   -1,   -1,  517,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,  405,  376,
   -1,   -1,  379,  380,    0,   -1,   -1,   -1,  262,   -1,
   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,  405,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  427,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,  315,   -1,   59,  318,   -1,   -1,  444,   -1,  446,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,
   -1,   -1,  336,   -1,   -1,  339,  493,   -1,   -1,   -1,
  262,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
  354,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,   -1,   -1,  368,   -1,  493,   -1,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   41,  389,  513,   44,   -1,   -1,
  517,   -1,   -1,  315,   -1,   -1,  318,   -1,   -1,   -1,
   -1,  405,   59,  325,   -1,  327,   -1,   -1,   -1,  331,
   -1,   -1,   -1,   -1,  336,   -1,   -1,  339,   41,   -1,
   -1,   44,   -1,  427,   -1,   -1,   -1,   -1,  432,   -1,
   -1,   -1,  354,   -1,  262,   -1,   59,    0,   -1,   -1,
  444,   -1,  446,   -1,   -1,   -1,  368,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,   -1,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  405,   -1,   -1,   -1,  315,   -1,  493,
  318,   -1,   -1,   -1,   -1,   -1,   59,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,  427,   -1,   -1,  336,  513,
  432,  339,   -1,  517,   -1,   -1,  262,   -1,   -1,    0,
   -1,   -1,  444,   -1,  446,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
   -1,  379,  380,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   41,  389,   -1,   44,   -1,   -1,   -1,   -1,   -1,  315,
   -1,  493,  318,   -1,   -1,   -1,   -1,  405,   59,  325,
   -1,  327,   -1,   -1,   -1,  331,   -1,   -1,   -1,   -1,
  336,  513,   -1,  339,   41,  517,   -1,   44,   -1,  427,
   -1,   -1,   -1,   -1,  432,  262,   -1,   -1,  354,   -1,
   -1,   -1,   59,    0,   -1,   -1,  444,   -1,  446,   -1,
   -1,   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  315,  405,
   -1,  318,   -1,   -1,   -1,  493,   -1,   -1,  325,   -1,
  327,   -1,   59,   -1,  331,   -1,   -1,   -1,   -1,  336,
   -1,  427,  339,   -1,   -1,  513,  432,   -1,   -1,  517,
   -1,   -1,  315,   -1,   -1,  318,   -1,  354,  444,  262,
  446,   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,
   -1,  368,   -1,  336,   -1,   -1,  339,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  389,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,  493,  405,   -1,
   -1,   -1,  315,  376,   -1,  318,  379,  380,   -1,   -1,
   -1,   -1,  325,   -1,  327,   -1,  389,  513,  331,   -1,
  427,  517,   -1,  336,   41,  432,  339,   44,   -1,   -1,
   -1,  262,  405,   -1,   -1,   -1,   -1,  444,   -1,  446,
   -1,  354,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,  368,   -1,   -1,   -1,  432,
   -1,   -1,   -1,  376,   -1,  262,  379,  380,   -1,   -1,
   -1,  444,   -1,  446,   -1,   -1,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  315,   -1,  493,  318,   -1,   -1,
   -1,   -1,  405,   -1,  325,   -1,  327,   -1,   -1,   -1,
  331,   -1,   -1,   -1,   -1,  336,  513,   -1,  339,   -1,
  517,   -1,   -1,   -1,  427,   -1,   -1,   -1,  315,  432,
  493,  318,   -1,  354,   -1,  262,   -1,   -1,  325,   -1,
  327,  444,   -1,  446,  331,   -1,   -1,  368,   -1,  336,
  513,   -1,  339,   -1,  517,  376,   -1,   -1,  379,  380,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  389,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,  405,   -1,   -1,   -1,  315,  376,
  493,  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,
  327,   -1,  389,   -1,  331,   -1,  427,   -1,   -1,  336,
  513,  432,  339,   41,  517,   -1,   44,   -1,  405,   -1,
   -1,   -1,   -1,  444,   -1,  446,   -1,  354,   -1,   -1,
   -1,   59,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,  368,   -1,   -1,   -1,  432,   -1,   -1,   -1,  376,
   -1,   -1,  379,  380,   -1,  262,   -1,  444,   -1,  446,
   -1,   -1,  389,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,  493,   41,   -1,   -1,   44,   -1,  405,  272,
   -1,   -1,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,  513,   -1,   -1,  288,  517,   -1,   -1,  292,
  427,   -1,  295,   -1,   -1,  432,  493,   41,  315,   -1,
   44,  318,   -1,   -1,   -1,   -1,   -1,  444,  325,  446,
  327,   -1,   -1,   -1,  331,   59,  513,   -1,   -1,  336,
  517,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  335,    0,   -1,   -1,   -1,  354,  341,  342,
  343,   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,  493,   -1,   -1,  376,
   -1,   -1,  379,  380,  367,   -1,  369,   -1,   -1,   -1,
   -1,   -1,  389,   -1,   41,   -1,  513,   44,   -1,   -1,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  405,   -1,
   -1,   -1,   59,   -1,   -1,  398,   -1,   -1,   -1,   -1,
  403,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,  415,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,  437,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  472,
  473,   -1,   -1,   -1,   -1,   -1,  493,  315,   -1,   -1,
  318,   -1,   -1,   -1,  262,   -1,  489,  325,   -1,  327,
   -1,   -1,   -1,  331,   -1,   -1,  513,   -1,  336,   -1,
  517,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,   -1,  262,   -1,
   -1,   -1,   42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  368,   -1,   -1,   -1,   -1,   -1,   -1,  315,  376,   -1,
  318,  379,  380,   -1,   -1,   -1,   -1,  325,   -1,  327,
   -1,  389,   -1,  331,   -1,   -1,   -1,   -1,  336,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  405,   -1,   -1,
   -1,  315,   -1,   -1,  318,   -1,  354,   -1,   -1,   -1,
   -1,  325,   -1,  327,   -1,   -1,   -1,  331,   -1,  427,
  368,   -1,  336,   -1,  432,  339,   44,   -1,  376,   -1,
   -1,  379,  380,   -1,   -1,   -1,  444,   -1,  446,   -1,
  354,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,  405,   -1,   -1,
   -1,   -1,  376,   -1,   -1,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  389,   -1,   -1,  315,  427,
   -1,  318,   -1,   -1,  432,  493,   -1,   -1,  325,   -1,
  327,  405,   -1,   -1,  331,   -1,  444,   -1,  446,  336,
  267,  272,  339,   -1,  275,  513,   -1,  274,   -1,  517,
   -1,   -1,   -1,  427,   -1,   -1,   -1,  354,  432,  286,
   -1,  292,   -1,   -1,  295,  292,  293,   -1,   -1,   -1,
  444,  368,  446,   -1,   -1,   -1,   -1,   40,   -1,  376,
   43,   -1,   45,  380,  311,  493,   -1,   -1,   -1,   -1,
   -1,   -1,  389,  320,   -1,   58,  323,   -1,   -1,   -1,
   63,   -1,   -1,   -1,  335,  513,   -1,   -1,  405,  517,
  341,  342,  343,   -1,  345,  346,  347,   -1,  349,  493,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,  432,  367,  364,  369,  513,
   -1,   -1,   -1,  517,   -1,  372,   -1,  444,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  409,   -1,  415,   45,  413,   -1,   -1,   -1,
   -1,   -1,  419,   -1,   -1,   -1,  493,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  513,   -1,   -1,   -1,
  517,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,  317,
   -1,  472,  473,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,  489,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  505,  506,
  507,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,  452,   -1,  267,  455,   -1,  457,  458,  459,
  460,  274,  462,   -1,   58,   -1,   -1,   -1,   -1,   63,
  470,  471,   -1,  286,   -1,   -1,  289,   -1,   -1,  292,
  293,  481,  482,   -1,  484,   -1,   -1,   -1,   -1,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,  311,   -1,
  500,  501,  502,  503,  504,   -1,   -1,  320,  508,  509,
  323,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,  348,   -1,  350,  351,   -1,
   58,   -1,  470,  471,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,  481,  482,   -1,  484,   -1,   -1,  372,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,  267,  487,  488,   -1,  490,  491,  492,
  274,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  286,   -1,   -1,  508,  509,   -1,  292,  293,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  320,   58,   -1,  323,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  450,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
  460,   -1,  462,   -1,  348,   -1,  350,  351,   -1,   -1,
  470,  471,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
  364,  481,  482,   -1,  484,   -1,   -1,   -1,  372,   -1,
   -1,  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,
  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,
   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,
  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,  284,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,   -1,   40,   -1,
   -1,   43,   -1,   45,  452,   -1,  267,  455,   -1,  457,
  458,  459,  460,  274,  462,   -1,   58,   -1,   -1,   -1,
   -1,   63,  470,  471,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
  311,   -1,  500,  501,  502,  503,  504,   -1,   -1,  320,
  508,  509,  323,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,  348,   -1,  350,
  351,   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,  481,  482,   -1,  484,   -1,   -1,
   -1,  372,   -1,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,  267,  487,  488,   -1,  490,
  491,  492,  274,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  286,   -1,   -1,  508,  509,   -1,
  292,  293,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,   58,
   -1,  323,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  372,   -1,   -1,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   58,  320,   -1,   -1,  323,   63,   -1,   -1,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,  348,
  277,  350,  351,   -1,   -1,  282,   -1,   -1,  285,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  329,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  337,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,  355,   -1,
  357,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,  370,   -1,  372,  373,  374,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,  421,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,  277,   -1,   -1,  286,   -1,
  282,   -1,   -1,  285,  292,  293,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  320,   -1,   -1,  323,   63,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  337,   -1,   -1,   -1,   -1,
  348,   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,  355,   -1,  357,  364,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  370,   -1,
  372,  373,  374,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,  421,
   -1,   -1,   -1,   -1,   -1,  298,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,
   -1,  449,  450,  451,  452,  453,  454,  455,   -1,  457,
  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,
   -1,  334,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,
  488,   -1,  490,  491,  492,   -1,  494,  495,  496,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,
  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  399,  274,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,
   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  298,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,  334,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,  399,  274,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,
  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  320,   -1,   -1,  323,   63,   -1,
  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,  481,
  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,  491,
  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,
  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,
   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,  419,  284,   -1,   -1,   -1,   -1,  425,
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
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,  286,   -1,  292,   -1,   -1,  295,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,  347,
   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  367,
  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,
   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,   -1,
   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,
   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,
  454,  455,   -1,  457,  458,  459,  460,   -1,  462,  463,
   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,   -1,
  484,  489,   -1,  487,  488,   -1,  490,  491,  492,   -1,
  494,  495,  496,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,  272,   -1,   -1,  275,   -1,
   -1,  274,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,  292,   -1,   -1,  295,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,
  323,   63,   -1,   -1,   -1,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,   -1,  341,  342,  343,   -1,  345,  346,
  347,   -1,  349,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
  367,  364,  369,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  415,   -1,
   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,  325,  462,
  463,   -1,   -1,   -1,   -1,  472,  473,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,  489,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,  395,   -1,
   -1,   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,
  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  430,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,  320,   -1,
   -1,  323,   -1,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,  348,   -1,  350,  351,
   -1,   -1,   -1,   -1,  481,  482,   -1,  484,  360,   -1,
   -1,   -1,  364,   -1,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  419,   -1,   -1,
   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,
  452,  453,  454,  455,   -1,  457,  458,  459,  460,   -1,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,
  492,   -1,  494,  495,  496,  497,  260,   -1,  500,  501,
  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,
  274,   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,
   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,  435,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  320,   -1,   -1,  323,
   63,   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,
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
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,
  514,  515,  516,   -1,  518,  519,  520,  260,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   58,   -1,   -1,   -1,  300,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,  272,   -1,
  323,  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,  292,   -1,
   -1,  295,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,  369,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  398,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,  415,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,  437,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,  267,   -1,   -1,  508,  509,   -1,   -1,  274,
   -1,  514,  515,  516,   -1,  518,  519,  520,  472,  473,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   40,
   -1,   -1,   43,   -1,   45,  489,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,  360,   -1,   -1,   -1,  364,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,  339,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  274,  362,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  292,  293,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,  364,  452,   -1,   -1,  455,   -1,  457,
  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  470,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,  484,   -1,   -1,   -1,
   -1,   -1,   -1,  491,  492,   -1,  494,  495,   -1,  497,
   -1,   -1,  500,  501,  502,  503,  504,   -1,  419,   -1,
  508,  509,   -1,   -1,  425,   -1,  514,  515,  516,   -1,
  518,  519,  520,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,  267,   -1,   -1,  508,  509,   -1,
   -1,  274,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   40,   41,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,   -1,   -1,
  323,   -1,   -1,  452,   -1,   -1,  455,   -1,  457,  458,
  459,  460,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,   -1,   -1,  348,   -1,  350,  351,   -1,
   -1,   -1,  481,  482,   -1,  484,   -1,  360,   -1,   -1,
   -1,  364,  491,  492,   -1,  494,  495,   -1,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,
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
   -1,  514,  515,  516,   -1,  518,  519,  520,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,  481,  482,   -1,  484,
   -1,  360,   -1,   -1,   -1,  364,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  320,
   -1,   -1,  323,   -1,   -1,  452,   -1,   -1,  455,   -1,
  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,   -1,   -1,  348,   -1,  350,
  351,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,  360,
   -1,   -1,   -1,  364,  491,  492,   -1,  494,  495,   -1,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,  449,  450,
  451,  452,  453,  454,  455,   -1,  457,  458,  459,  460,
   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,   -1,  484,   -1,   -1,  487,  488,   -1,  490,
  491,  492,   -1,  494,  495,  496,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  514,  515,  516,   -1,  518,  519,  520,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,  267,   -1,
   -1,  508,  509,   -1,   -1,  274,   -1,  514,  515,  516,
   -1,  518,  519,  520,   -1,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  292,  293,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  348,
   -1,  350,  351,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,  364,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  449,  450,  451,  452,  453,  454,  455,   -1,  457,  458,
  459,  460,   -1,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,   -1,  484,   -1,   -1,  487,  488,
   -1,  490,  491,  492,   -1,  494,  495,  496,  497,   -1,
   -1,  500,  501,  502,  503,  504,   -1,   -1,   -1,  508,
  509,   -1,   -1,   -1,   -1,  514,  515,  516,   -1,  518,
  519,  520,  267,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,  293,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  320,   -1,   -1,  323,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  348,   -1,  350,  351,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  364,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  449,  450,  451,  452,  453,  454,
  455,   -1,  457,  458,  459,  460,   -1,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,   -1,  484,
   -1,   -1,  487,  488,   -1,  490,  491,  492,   -1,  494,
  495,  496,  497,   -1,   -1,  500,  501,  502,  503,  504,
  267,   -1,   -1,  508,  509,   -1,   -1,  274,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,  286,
   -1,   -1,   -1,   -1,   -1,  292,  293,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  320,   -1,   -1,  323,   -1,   -1,   -1,
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
   -1,   -1,  449,  450,  451,  452,  453,  454,  455,   -1,
  457,  458,  459,  460,   -1,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,   -1,  484,   -1,   -1,
  487,  488,   -1,  490,  491,  492,   -1,  494,  495,  496,
  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,  516,
    0,  518,  519,  520,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  274,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,   -1,   -1,   -1,  292,
  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   41,   -1,   -1,   44,  320,   -1,   59,
  323,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  348,   -1,  350,  351,   -1,
   41,   91,   -1,   44,   -1,   -1,   -1,  360,   -1,   -1,
   -1,  364,   -1,   91,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,   -1,
   -1,   -1,  425,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  449,  450,  451,  452,
  453,  454,  455,   -1,  457,  458,  459,  460,   -1,  462,
  463,   -1,   -1,    0,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
   -1,  484,   -1,   -1,  487,  488,   -1,  490,  491,  492,
   -1,  494,  495,  496,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   41,  508,  509,   44,   -1,   -1,
   -1,  514,  515,  516,   -1,  518,  519,  520,  258,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,  273,   -1,  275,  276,  277,   -1,   -1,
   -1,  281,   -1,   -1,  284,  273,   -1,   -1,  276,  277,
   -1,   -1,   -1,  281,   91,   -1,  284,  297,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,  306,  258,   -1,  297,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  306,   -1,
   -1,   -1,  273,   -1,   -1,  276,  277,   -1,   -1,   -1,
  281,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,  348,   -1,
   -1,   -1,  303,   -1,   -1,  306,   -1,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  393,   -1,   -1,   -1,  348,   -1,  399,
   -1,   -1,   -1,  403,   -1,  393,   -1,   -1,   -1,   -1,
  410,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,   -1,
   -1,  372,  410,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
  428,   -1,  393,   -1,   -1,  433,   -1,   -1,  399,   -1,
   -1,   -1,  403,   -1,   -1,   -1,   -1,   -1,  458,  410,
   -1,  258,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,   -1,   -1,   -1,  462,   -1,  273,  428,   -1,  276,
  277,   -1,  433,   -1,  281,   -1,   -1,  284,   -1,  270,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  297,   -1,   -1,   -1,   -1,   -1,  303,  458,   -1,  306,
   -1,  462,   -1,   -1,   -1,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,  305,   -1,   -1,   -1,  309,  310,
   -1,  312,   -1,   -1,  315,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,
   -1,  348,   -1,  334,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  270,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  393,   -1,   -1,   -1,
   -1,   -1,  399,   -1,   -1,   -1,  403,   -1,   -1,   -1,
  391,   -1,   -1,  410,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  309,  310,  409,  312,
   -1,  428,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,  424,   -1,  328,   -1,   -1,   -1,   -1,
   -1,  334,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  443,   -1,  445,  462,   -1,   -1,   -1,   -1,
   -1,  452,   -1,   -1,  455,   -1,  457,  458,  459,  460,
   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,  484,   -1,   -1,   -1,   -1,  391,   -1,
  491,  492,   -1,  494,  495,   -1,  497,   -1,   -1,  500,
  501,  502,  503,  504,   -1,  270,  409,  508,  509,  510,
  511,  512,   -1,  514,  515,  516,   -1,  518,  519,  520,
   -1,  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  434,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,  309,  310,   -1,  312,   -1,  452,
   -1,   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,
   -1,   -1,   -1,  328,   -1,   -1,   -1,  470,  471,  334,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  484,   -1,   -1,   -1,   -1,   -1,   -1,  491,  492,
   -1,  494,  495,   -1,  497,   -1,   -1,  500,  501,  502,
  503,  504,   -1,   -1,   -1,  508,  509,  510,  511,  512,
   -1,  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  391,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,
  455,   -1,  457,  458,  459,  460,   -1,  462,  272,   -1,
   -1,  275,   -1,   -1,   -1,  470,  471,  281,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  292,  484,
   -1,  295,   -1,   -1,   -1,   -1,  491,  492,   -1,  494,
  495,   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,
   -1,   -1,   -1,  508,  509,  510,  511,  512,   -1,  514,
  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,   -1,  341,  342,  343,
   -1,  345,  346,  347,   -1,  349,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,  369,  272,   -1,   -1,  275,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  292,   -1,   -1,  295,
   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  415,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  335,
   -1,   -1,   -1,  437,   -1,  341,  342,  343,   -1,  345,
  346,  347,   -1,  349,   -1,   -1,   -1,   -1,  452,   -1,
   -1,  455,   -1,  457,  458,  459,  460,   -1,  462,   -1,
   -1,  367,   -1,  369,   -1,   -1,  470,  471,  472,  473,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
  484,   -1,   -1,   -1,   -1,  489,   -1,  491,  492,   -1,
  494,  495,  398,  497,   -1,   -1,  500,  501,  502,  503,
  504,   -1,   -1,   -1,  508,  509,   -1,   -1,   -1,  415,
  514,  515,  516,   -1,  518,  519,  520,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  437,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  455,
   -1,  457,  458,  459,  460,   -1,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  470,  471,  472,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,  484,   -1,
   -1,   -1,   -1,  489,   -1,  491,  492,   -1,  494,  495,
   -1,  497,   -1,   -1,  500,  501,  502,  503,  504,   -1,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,  514,  515,
  516,   -1,  518,  519,  520,  452,   -1,   -1,  455,  456,
  457,  458,  459,  460,  461,  462,  463,  464,  465,  466,
  467,  468,  469,  470,  471,  472,  473,  474,  475,  476,
   -1,   -1,  479,  480,  481,  482,  483,  484,   -1,   -1,
  487,  488,  489,  490,  491,  492,  493,  494,  495,  496,
  497,  498,  499,  500,  501,  502,  503,  504,   -1,   -1,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,  518,  519,  520,  452,   -1,   -1,  455,  456,  457,
  458,  459,  460,   -1,  462,  463,  464,  465,  466,  467,
  468,  469,  470,  471,  472,  473,  474,  475,  476,   -1,
   -1,  479,  480,  481,  482,  483,  484,   -1,   -1,  487,
  488,  489,  490,  491,  492,  493,  494,  495,  496,  497,
  498,  499,  500,  501,  502,  503,  504,   -1,   -1,   -1,
  508,  509,  510,  511,  512,  513,  514,  515,  516,  517,
  518,  519,  520,
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
"statement : exec_procedure",
"statement : exec_block",
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
"proc_statement : insert ';'",
"proc_statement : update ';'",
"proc_statement : delete ';'",
"proc_statement : singleton_select ';'",
"proc_statement : exec_procedure ';'",
"proc_statement : exec_sql ';'",
"proc_statement : exec_into ';'",
"proc_statement : exec_udf ';'",
"proc_statement : excp_statement ';'",
"proc_statement : raise_statement ';'",
"proc_statement : post_event ';'",
"proc_statement : cursor_statement ';'",
"proc_statement : breakleave ';'",
"proc_statement : SUSPEND ';'",
"proc_statement : EXIT ';'",
"proc_statement : if_then_else",
"proc_statement : while",
"proc_statement : for_select",
"proc_statement : for_exec_into",
"excp_statement : EXCEPTION symbol_exception_name",
"excp_statement : EXCEPTION symbol_exception_name value",
"raise_statement : EXCEPTION",
"exec_sql : EXECUTE STATEMENT value",
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt",
"event_argument_opt :",
"singleton_select : select INTO variable_list",
"variable : ':' symbol_variable_name",
"variable_list : variable",
"variable_list : column_name",
"variable_list : variable_list ',' column_name",
"variable_list : variable_list ',' variable",
"while : label_opt WHILE '(' search_condition ')' DO proc_block",
"label_opt : symbol_label_name ':'",
"label_opt :",
"breakleave : KW_BREAK",
"breakleave : LEAVE",
"breakleave : LEAVE symbol_label_name",
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
"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs_opt",
"proc_inputs : value_list",
"proc_inputs : '(' value_list ')'",
"proc_inputs :",
"proc_outputs_opt : RETURNING_VALUES variable_list",
"proc_outputs_opt : RETURNING_VALUES '(' variable_list ')'",
"proc_outputs_opt :",
"exec_block : EXECUTE BLOCK block_input_params output_parameters AS local_declaration_list full_proc_block",
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
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause plan_clause order_clause rows_clause",
"update_positioned : UPDATE table_name SET assignments cursor_clause",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
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
case 306:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 307:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 312:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[0], NULL); }
break;
case 313:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], yyvsp[0]); }
break;
case 314:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 315:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[0]); }
break;
case 316:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 317:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-2], 0, make_list (yyvsp[0])); }
break;
case 319:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 320:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 321:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-1], yyvsp[0]); }
break;
case 322:
{ yyval = NULL; }
break;
case 323:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-2],
					  make_list (yyvsp[0]), NULL, NULL); }
break;
case 324:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 327:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 328:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 329:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 330:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 331:
{ yyval = NULL; }
break;
case 332:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 333:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 334:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[0], NULL)); }
break;
case 335:
{ yyval = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 336:
{ yyval = NULL; }
break;
case 338:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 339:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 342:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 349:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 350:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 351:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 352:
{ yyval = NULL; }
break;
case 353:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count,
					yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 354:
{ yyval = make_list (yyvsp[0]); }
break;
case 355:
{ yyval = make_list (yyvsp[-1]); }
break;
case 356:
{ yyval = NULL; }
break;
case 357:
{ yyval = make_list (yyvsp[0]); }
break;
case 358:
{ yyval = make_list (yyvsp[-1]); }
break;
case 359:
{ yyval = NULL; }
break;
case 360:
{ yyval = make_node (nod_exec_block,
						  (int) e_exe_blk_count, 
					          yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 361:
{ yyval = make_list (yyvsp[-1]); }
break;
case 362:
{ yyval = NULL; }
break;
case 364:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 365:
{ yyval = make_node (nod_param_val, e_prm_val_count, yyvsp[-2], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 368:
{ lex.beginning = lex_position(); }
break;
case 369:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
				   (lex_position() == lex.end) ?
				   lex_position() - lex.beginning : lex.last_token - lex.beginning);}
break;
case 370:
{ lex.beginning = lex.last_token; }
break;
case 371:
{ yyval = (dsql_nod*) MAKE_string(lex.beginning,
					lex_position() - lex.beginning); }
break;
case 372:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 373:
{ yyval = 0; }
break;
case 374:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 375:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 376:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 378:
{ yyval = NULL; }
break;
case 379:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 380:
{ yyval = MAKE_constant ((dsql_str*) 0, CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((dsql_str*)(IPTR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 398:
{ yyval = NULL; }
break;
case 399:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 400:
{ yyval = yyvsp[0]; }
break;
case 401:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 402:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 403:
{ yyval = yyvsp[0]; }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 406:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 407:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 412:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 413:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 417:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 422:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 427:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 429:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 461:
{ yyval = NULL; }
break;
case 462:
{ yyval = NULL; }
break;
case 463:
{ yyval = NULL; }
break;
case 464:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 465:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (dsql_nod*) lex.g_field; }
break;
case 466:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 467:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 468:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 470:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 471:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 472:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 473:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-4],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 474:
{ lex.log_defined = false;
			  lex.cache_defined = false;
			  yyval = NULL; }
break;
case 476:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 477:
{ yyval = yyvsp[0]; }
break;
case 478:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 480:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 481:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 482:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 484:
{ yyval = NULL; }
break;
case 486:
{ yyval = NULL; }
break;
case 487:
{ yyval = yyvsp[0]; }
break;
case 488:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 505:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 506:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 508:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 509:
{ if ((IPTR) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG));
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG),
					MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 510:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((dsql_str*) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 516:
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
case 517:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 519:
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
case 520:
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
case 521:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[-1];
			}
break;
case 528:
{
			lex.g_field->fld_seg_length = (USHORT)(IPTR) yyvsp[0];
		  	}
break;
case 529:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 530:
{
			lex.g_field->fld_sub_type = (USHORT)(IPTR) yyvsp[0];
			}
break;
case 531:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 532:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 533:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 539:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 540:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(IPTR) yyvsp[-1]; 
			}
break;
case 549:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 550:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				}
			}
break;
case 551:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 552:
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
case 553:
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
case 556:
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
case 557:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 559:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 560:
{ yyval = yyvsp[-1]; }
break;
case 561:
{ yyval = 0; }
break;
case 565:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG)); }
break;
case 566:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-2],
				MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64)); }
break;
case 567:
{ yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
				make_node(nod_negate, 1, MAKE_constant((dsql_str*) yyvsp[0], CONSTANT_SINT64))); }
break;
case 571:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 572:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 573:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 574:
{ yyval = 0; }
break;
case 575:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 578:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 579:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 582:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 583:
{ yyval = NULL; }
break;
case 585:
{ yyval = NULL; }
break;
case 586:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 588:
{ yyval = NULL; }
break;
case 590:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 595:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 596:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 597:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 599:
{ yyval = yyvsp[0];}
break;
case 601:
{ yyval = yyvsp[0];}
break;
case 602:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 603:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 604:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 605:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 606:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 607:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 608:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 609:
{ yyval = 0; }
break;
case 610:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 611:
{ yyval = (dsql_nod*) NOD_SHARED; }
break;
case 612:
{ yyval = (dsql_nod*) NOD_PROTECTED ; }
break;
case 613:
{ yyval = (dsql_nod*) 0; }
break;
case 614:
{ yyval = (dsql_nod*) NOD_READ; }
break;
case 615:
{ yyval = (dsql_nod*) NOD_WRITE; }
break;
case 617:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 619:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) yyvsp[-1] | (SSHORT)(IPTR) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 620:
{ yyval = 0; }
break;
case 622:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 623:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 624:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 625:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 626:
{ yyval = NULL; }
break;
case 627:
{ yyval = yyvsp[0]; }
break;
case 628:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 630:
{ yyval = NULL; }
break;
case 631:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 632:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 633:
{ yyval = make_flag_node (nod_select_expr, NOD_SELECT_EXPR_VALUE | NOD_SELECT_EXPR_SINGLETON,
					(int) e_sel_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 635:
{ yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 636:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 638:
{ yyval = make_node (nod_query_spec, (int) e_qry_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 639:
{ lex.limit_clause = true; }
break;
case 640:
{ lex.limit_clause = false; }
break;
case 641:
{ lex.first_detection = true; }
break;
case 642:
{ lex.first_detection = false; }
break;
case 643:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 644:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 645:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 646:
{ yyval = 0; }
break;
case 647:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[-1], CONSTANT_SLONG); }
break;
case 648:
{ yyval = yyvsp[-2]; }
break;
case 649:
{ yyval = yyvsp[-1]; }
break;
case 650:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 651:
{ yyval = yyvsp[-1]; }
break;
case 652:
{ yyval = yyvsp[0]; }
break;
case 653:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 654:
{ yyval = 0; }
break;
case 655:
{ yyval = make_list (yyvsp[0]); }
break;
case 656:
{ yyval = 0; }
break;
case 658:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 660:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 663:
{ yyval = make_list (yyvsp[0]); }
break;
case 665:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 670:
{ yyval = yyvsp[-1]; }
break;
case 671:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 673:
{ yyval = NULL; }
break;
case 674:
{ yyval = make_list (yyvsp[-1]); }
break;
case 675:
{ yyval = NULL; }
break;
case 677:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 681:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-3],
				make_node (nod_join_inner, (int) 0, NULL), yyvsp[0], NULL); }
break;
case 682:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-2], yyvsp[0],
					make_node (nod_flag, 0, NULL)); }
break;
case 683:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 686:
{ yyval = yyvsp[0]; }
break;
case 687:
{ yyval = make_list (yyvsp[-1]); }
break;
case 688:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 689:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 690:
{ yyval = make_list (yyvsp[-1]); }
break;
case 691:
{ yyval = NULL; }
break;
case 693:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 694:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 695:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 698:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 699:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 702:
{ yyval = make_list (yyvsp[0]); }
break;
case 703:
{ yyval = NULL; }
break;
case 705:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 707:
{ yyval = yyvsp[0]; }
break;
case 708:
{ yyval = NULL; }
break;
case 709:
{ yyval = yyvsp[0]; }
break;
case 710:
{ yyval = NULL; }
break;
case 711:
{ yyval = yyvsp[0]; }
break;
case 712:
{ yyval = NULL; }
break;
case 713:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 714:
{ yyval = 0; }
break;
case 715:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 716:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 717:
{ yyval = 0; }
break;
case 718:
{ yyval = 0; }
break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 726:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 727:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 729:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 730:
{ yyval = make_list (yyvsp[-1]); }
break;
case 731:
{ yyval = 0; }
break;
case 732:
{ yyval = make_list (yyvsp[0]); }
break;
case 733:
{ yyval = 0; }
break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 737:
{ yyval = 0; }
break;
case 738:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 739:
{ yyval = 0; }
break;
case 740:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 741:
{ yyval = MAKE_constant((dsql_str*) NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 742:
{ yyval = yyvsp[-1]; }
break;
case 743:
{ yyval = 0; }
break;
case 744:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 745:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG))); }
break;
case 746:
{ yyval = NULL; }
break;
case 747:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 748:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 751:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 752:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 755:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-6], make_list (yyvsp[-4]), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 756:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 759:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 761:
{ yyval = make_node (nod_assign, 2, yyvsp[0], make_node (nod_null, 0, NULL)); }
break;
case 762:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 769:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 770:
{ yyval = yyvsp[0]; }
break;
case 772:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 775:
{ yyval = NULL; }
break;
case 776:
{ yyval = make_list (yyvsp[-1]); }
break;
case 778:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = NULL; }
break;
case 781:
{ yyval = make_list (yyvsp[-1]); }
break;
case 783:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 787:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = yyvsp[-1]; }
break;
case 803:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 837:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_equiv, 2, yyvsp[-4], yyvsp[0])); }
break;
case 842:
{ yyval = make_node (nod_equiv, 2, yyvsp[-5], yyvsp[0]); }
break;
case 843:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 845:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 847:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 848:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 849:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 851:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 853:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 854:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 855:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 856:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 857:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 858:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 859:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 860:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 864:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 865:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 2, CONSTANT_SLONG)); }
break;
case 866:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((dsql_str*) 3, CONSTANT_SLONG)); }
break;
case 868:
{ yyval = make_list (yyvsp[-1]); }
break;
case 869:
{ yyval = yyvsp[-1]; }
break;
case 880:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 881:
{ yyval = yyvsp[0]; }
break;
case 882:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 883:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 884:
{ yyval = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) yyvsp[0], yyvsp[-2]); }
break;
case 885:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 886:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 887:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 888:
{ yyval = yyvsp[-1]; }
break;
case 889:
{ yyval = yyvsp[-1]; }
break;
case 893:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 894:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 895:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 898:
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
case 899:
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
case 900:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 901:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 903:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 905:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 906:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_STRING); }
break;
case 907:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SLONG); }
break;
case 908:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 909:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 910:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_SINT64); }
break;
case 912:
{ yyval = MAKE_str_constant ((dsql_str*) yyvsp[0], lex.att_charset); }
break;
case 913:
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
case 914:
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
case 915:
{ yyval = MAKE_constant ((dsql_str*) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 916:
{ yyval = make_parameter (); }
break;
case 917:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 918:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 919:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 920:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_connection_id, CONSTANT_SLONG)); }
break;
case 921:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 922:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_gdscode, CONSTANT_SLONG)); }
break;
case 923:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 924:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((dsql_str*) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 925:
{ yyval = yyvsp[0]; }
break;
case 926:
{ ((dsql_str*) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 928:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 929:
{ if ((IPTR) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 930:
{ if ((IPTR) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 931:
{ if ((IPTR) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 932:
{ if ((IPTR) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 934:
{ yyval = (dsql_nod*) - (IPTR) yyvsp[0]; }
break;
case 935:
{ yyval = yyvsp[0];}
break;
case 939:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 940:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 941:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 942:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 943:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 944:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 945:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 946:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 947:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 949:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 953:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 954:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((dsql_str*) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 955:
{ yyval = yyvsp[0]; }
break;
case 956:
{ yyval = MAKE_constant ((dsql_str*) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 957:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 958:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 959:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 962:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 963:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, yyvsp[-5], yyvsp[-3]), yyvsp[-1]); }
break;
case 964:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 967:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 968:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 969:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 970:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 971:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 972:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 973:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 974:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 978:
{ yyval = make_node (nod_gen_id, 2, yyvsp[0],
					MAKE_constant((dsql_str*) 1, CONSTANT_SLONG)); }
break;
case 979:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 980:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_year, CONSTANT_SLONG); }
break;
case 981:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_month, CONSTANT_SLONG); }
break;
case 982:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_day, CONSTANT_SLONG); }
break;
case 983:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_hour, CONSTANT_SLONG); }
break;
case 984:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_minute, CONSTANT_SLONG); }
break;
case 985:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_second, CONSTANT_SLONG); }
break;
case 986:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 987:
{ yyval = MAKE_constant ((dsql_str*)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 992:
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
