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
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
#include <stdio.h>
/*#include <wincon.h>*/
#endif

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../jrd/gds.h"
#include "../jrd/flags.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/parse_proto.h"
#include "../dsql/keywords.h"
#include "../dsql/misc_func.h"
#include "../jrd/gds_proto.h"
#include "../jrd/thd_proto.h"
#include "../wal/wal.h"
#include "../jrd/err_proto.h"

static void	yyerror (TEXT *);

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

const int MIN_CACHE_BUFFERS	= 250;
const int DEF_CACHE_BUFFERS	= 1000;

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


DSQL_NOD		DSQL_parse;

static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(DSQL_NOD, SLONG *, SSHORT);
#endif
static void	stack_nodes (DSQL_NOD, DLLS *);
inline static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, ISC_STATUS);
static void	check_log_file_attrs (void);

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
	DSQL_FLD g_field;
	FIL	g_file;
	DSQL_NOD g_field_name;
	SSHORT log_defined, cache_defined;
	int dsql_debug;
	
	/* Actual lexer state begins from here */
	TEXT *beginning;
	TEXT	*ptr, *end, *last_token, *line_start;
	TEXT	*last_token_bk, *line_start_bk;
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
		BOOLEAN	*stmt_ambiguous);
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
#define BASENAME 269
#define BEFORE 270
#define BEGIN 271
#define BETWEEN 272
#define BLOB 273
#define BY 274
#define CACHE 275
#define CAST 276
#define CHARACTER 277
#define CHECK 278
#define CHECK_POINT_LEN 279
#define COLLATE 280
#define COMMA 281
#define COMMIT 282
#define COMMITTED 283
#define COMPUTED 284
#define CONCATENATE 285
#define CONDITIONAL 286
#define CONSTRAINT 287
#define CONTAINING 288
#define COUNT 289
#define CREATE 290
#define CSTRING 291
#define CURRENT 292
#define CURSOR 293
#define DATABASE 294
#define DATE 295
#define DB_KEY 296
#define KW_DEBUG 297
#define DECIMAL 298
#define DECLARE 299
#define DEFAULT 300
#define KW_DELETE 301
#define DESC 302
#define DISTINCT 303
#define DO 304
#define DOMAIN 305
#define DROP 306
#define ELSE 307
#define END 308
#define ENTRY_POINT 309
#define EQL 310
#define ESCAPE 311
#define EXCEPTION 312
#define EXECUTE 313
#define EXISTS 314
#define EXIT 315
#define EXTERNAL 316
#define FILTER 317
#define FOR 318
#define FOREIGN 319
#define FROM 320
#define FULL 321
#define FUNCTION 322
#define GDSCODE 323
#define GEQ 324
#define GENERATOR 325
#define GEN_ID 326
#define GRANT 327
#define GROUP 328
#define GROUP_COMMIT_WAIT 329
#define GTR 330
#define HAVING 331
#define IF 332
#define KW_IN 333
#define INACTIVE 334
#define INNER 335
#define INPUT_TYPE 336
#define INDEX 337
#define INSERT 338
#define INTEGER 339
#define INTO 340
#define IS 341
#define ISOLATION 342
#define JOIN 343
#define KEY 344
#define KW_CHAR 345
#define KW_DEC 346
#define KW_DOUBLE 347
#define KW_FILE 348
#define KW_FLOAT 349
#define KW_INT 350
#define KW_LONG 351
#define KW_NULL 352
#define KW_NUMERIC 353
#define KW_UPPER 354
#define KW_VALUE 355
#define LENGTH 356
#define LOGFILE 357
#define LPAREN 358
#define LEFT 359
#define LEQ 360
#define LEVEL 361
#define LIKE 362
#define LOG_BUF_SIZE 363
#define LSS 364
#define MANUAL 365
#define MAXIMUM 366
#define MAX_SEGMENT 367
#define MERGE 368
#define MESSAGE 369
#define MINIMUM 370
#define MODULE_NAME 371
#define NAMES 372
#define NATIONAL 373
#define NATURAL 374
#define NCHAR 375
#define NEQ 376
#define NO 377
#define NOT 378
#define NOT_GTR 379
#define NOT_LSS 380
#define NUM_LOG_BUFS 381
#define OF 382
#define ON 383
#define ONLY 384
#define OPTION 385
#define OR 386
#define ORDER 387
#define OUTER 388
#define OUTPUT_TYPE 389
#define OVERFLOW 390
#define PAGE 391
#define PAGES 392
#define PAGE_SIZE 393
#define PARAMETER 394
#define PASSWORD 395
#define PLAN 396
#define POSITION 397
#define POST_EVENT 398
#define PRECISION 399
#define PRIMARY 400
#define PRIVILEGES 401
#define PROCEDURE 402
#define PROTECTED 403
#define RAW_PARTITIONS 404
#define READ 405
#define REAL 406
#define REFERENCES 407
#define RESERVING 408
#define RETAIN 409
#define RETURNING_VALUES 410
#define RETURNS 411
#define REVOKE 412
#define RIGHT 413
#define RPAREN 414
#define ROLLBACK 415
#define SEGMENT 416
#define SELECT 417
#define SET 418
#define SHADOW 419
#define KW_SHARED 420
#define SINGULAR 421
#define KW_SIZE 422
#define SMALLINT 423
#define SNAPSHOT 424
#define SOME 425
#define SORT 426
#define SQLCODE 427
#define STABILITY 428
#define STARTING 429
#define STATISTICS 430
#define SUB_TYPE 431
#define SUSPEND 432
#define SUM 433
#define TABLE 434
#define THEN 435
#define TO 436
#define TRANSACTION 437
#define TRIGGER 438
#define UNCOMMITTED 439
#define UNION 440
#define UNIQUE 441
#define UPDATE 442
#define USER 443
#define VALUES 444
#define VARCHAR 445
#define VARIABLE 446
#define VARYING 447
#define VERSION 448
#define VIEW 449
#define WAIT 450
#define WHEN 451
#define WHERE 452
#define WHILE 453
#define WITH 454
#define WORK 455
#define WRITE 456
#define FLOAT_NUMBER 457
#define NUMBER 458
#define NUMERIC 459
#define SYMBOL 460
#define STRING 461
#define INTRODUCER 462
#define ACTION 463
#define ADMIN 464
#define CASCADE 465
#define FREE_IT 466
#define RESTRICT 467
#define ROLE 468
#define COLUMN 469
#define TYPE 470
#define EXTRACT 471
#define YEAR 472
#define MONTH 473
#define DAY 474
#define HOUR 475
#define MINUTE 476
#define SECOND 477
#define WEEKDAY 478
#define YEARDAY 479
#define TIME 480
#define TIMESTAMP 481
#define CURRENT_DATE 482
#define CURRENT_TIME 483
#define CURRENT_TIMESTAMP 484
#define NUMBER64BIT 485
#define SCALEDINT 486
#define CURRENT_USER 487
#define CURRENT_ROLE 488
#define KW_BREAK 489
#define SUBSTRING 490
#define RECREATE 491
#define KW_DESCRIPTOR 492
#define FIRST 493
#define SKIP 494
#define CURRENT_CONNECTION 495
#define CURRENT_TRANSACTION 496
#define BIGINT 497
#define CASE 498
#define NULLIF 499
#define COALESCE 500
#define USING 501
#define NULLS 502
#define LAST 503
#define ROW_COUNT 504
#define LOCK 505
#define SAVEPOINT 506
#define RELEASE 507
#define STATEMENT 508
#define LEAVE 509
#define INSERTING 510
#define UPDATING 511
#define DELETING 512
#define KW_INSERTING 513
#define KW_UPDATING 514
#define KW_DELETING 515
#define BACKUP 516
#define KW_DIFFERENCE 517
#define OPEN 518
#define CLOSE 519
#define FETCH 520
#define ROWS 521
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    9,    9,    9,    9,    9,   22,   22,   21,   21,
   21,   32,   32,   25,   33,   33,   33,   33,   33,   24,
   24,   30,   30,   26,   14,   14,   14,   14,   14,   14,
   14,   36,   27,   27,   27,   27,   37,   37,   37,   37,
   23,   23,   38,   38,   38,   28,   28,   43,   29,   29,
   44,   44,    6,   45,   45,   47,   52,   52,   52,   49,
   49,   49,   56,   56,   57,   57,   50,   50,   59,   59,
   59,   59,   59,   46,    5,   62,   62,   62,   62,   62,
   62,   62,   62,   62,   62,   62,   12,   77,   77,   77,
   13,   81,   81,   64,   64,   67,   67,   67,   75,   90,
   90,   90,   91,   91,   92,   92,   93,   93,   97,   97,
   74,  100,  100,  102,  102,  103,  103,  106,  106,  107,
  108,  108,  109,  110,   72,   76,   73,   94,   94,  113,
  114,  114,  116,  116,  117,  117,  117,  117,  117,  115,
  115,  118,  118,  119,  119,  119,  119,  119,  121,  121,
  121,  121,  120,  120,  125,  125,  124,   98,  126,  126,
  127,  130,  131,  131,  132,  128,  129,  129,  133,  133,
  134,  134,  135,  135,  135,   96,   96,   96,   69,   79,
  136,  136,  136,  137,  137,  138,  138,  139,  139,  139,
  145,   86,   86,  141,  141,  104,  104,   99,  149,  150,
   58,  142,  142,  105,  105,  105,  105,  105,  105,  143,
  143,  157,  157,  158,  160,  160,  160,  160,  160,  140,
  159,  159,  164,  164,  164,  164,  166,  167,  168,  162,
  162,  163,  161,  161,  161,  161,  161,  169,  170,  171,
  171,  171,  171,   68,   78,   82,  176,  172,  172,  173,
  173,  177,  177,  178,  174,  174,  179,  179,  180,  182,
  182,  183,  181,  181,  185,  185,  184,  188,  188,  175,
  190,  190,  190,  191,  191,  189,  189,  189,  189,  189,
  189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
  189,  189,  189,  189,  194,  194,  195,  196,  197,  209,
  209,  198,  203,  204,  199,  199,  200,  213,  201,  214,
  207,  207,  207,  208,  208,  208,  211,  211,  211,  211,
  202,  210,  210,  205,  205,  205,  212,  212,  192,  192,
  219,  220,  220,  221,  221,  221,  221,  206,  206,  206,
  223,  225,  224,  226,   11,   71,   80,  227,  230,  230,
  230,  231,  234,  238,  238,  239,  239,  240,  240,  147,
  229,   87,   89,  228,  228,   70,   83,  243,  243,  243,
  244,  247,  247,  248,  248,  248,  248,  248,  248,  248,
  248,  248,  248,  248,  248,  248,  248,  248,  245,  245,
  246,    2,  250,  250,  250,  250,  250,  250,  250,  258,
  259,  256,  256,  260,  260,  260,  260,  260,  260,  260,
  251,  251,  261,  261,  261,  261,  261,  261,  261,  255,
  266,  266,  266,  266,  266,  266,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  266,  263,  263,  265,
  265,  264,  262,  262,  262,  257,  257,  253,  254,  254,
  268,  268,  268,  268,  268,  268,  268,  268,  269,  269,
  252,  270,  270,  271,  271,    8,  272,  272,  272,  272,
  272,  272,  272,  272,  272,  272,  272,  272,  101,  101,
  144,  144,  275,  275,  278,  278,  280,  280,   53,   53,
  277,  277,  277,  277,  277,  277,  277,  277,  277,  285,
  285,  276,  276,  276,  276,  286,  286,   61,   61,   61,
   55,   55,  282,  282,  282,  279,  279,  279,  290,  290,
  290,  289,  289,  288,  288,  288,  283,  283,  291,  291,
  291,  292,  292,  284,  284,  284,  284,  293,  293,   18,
   18,   18,  295,  295,  295,   16,   16,   16,  297,  298,
  301,  301,  299,  303,  303,    4,   15,  302,  302,  304,
  304,  305,  305,  294,  306,  306,  307,  307,  308,  308,
  308,  308,  309,  309,  310,  310,  311,  311,  313,  313,
  313,  314,  314,  314,  315,  315,  315,  312,  317,  317,
  317,  318,  318,  316,  316,  319,  321,  321,  320,  320,
  296,   17,  322,  322,  322,  323,  323,  328,  328,  329,
   65,   65,   65,  331,  331,  330,  330,  324,  324,  324,
  325,  325,  334,  334,  326,  326,  327,  187,  338,  339,
  332,  333,  335,  335,  335,  335,  340,  340,  340,  341,
  341,  341,  232,  232,  233,  233,  344,  344,  345,  345,
  347,  347,  336,  349,  349,  350,  350,  350,  353,  354,
  354,  355,  355,  357,  357,  351,  351,  352,  352,  358,
  358,  241,  241,   31,  242,  242,  242,  242,  242,  242,
  242,  242,  236,  236,  359,  359,  360,  237,  237,  235,
  235,  337,  337,  361,  362,  362,  362,  362,  362,  363,
  363,  364,  364,  365,  365,  366,  366,  366,  367,  367,
  368,  368,   10,   10,    7,    7,  370,  371,  372,   19,
   19,  373,  374,  375,  375,  193,  346,  346,  216,  216,
    3,    3,  377,  377,  377,  379,  379,   61,  378,  378,
  380,  380,   34,   34,   85,   84,   84,  369,  369,  381,
  382,  382,  217,  217,  217,  146,  376,  376,  111,  111,
  111,  111,  111,  386,  386,  386,  386,  386,  386,  385,
  385,  385,  388,  388,  388,  388,  388,  388,  388,  388,
  388,  388,  389,  389,  389,  389,  389,  389,  389,  389,
  394,  394,  394,  394,  394,  394,  394,  394,  394,  394,
  394,  394,  394,  394,  394,  394,  400,  400,  390,  390,
  391,  391,  391,  391,  392,  392,  396,  396,  397,  397,
  397,  397,  395,  398,  393,  393,  384,  384,  384,  387,
  387,  387,  401,  401,  402,  399,  404,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,  156,  156,  156,  405,  403,  403,  151,
  151,  411,  411,  411,  411,  411,  407,  407,  407,  407,
  407,  342,  152,  152,  153,  154,  154,  154,  154,  154,
   51,   51,   20,   20,  249,  412,   54,  123,  281,  281,
   95,  406,  406,  406,  406,  413,  413,  413,  413,  413,
  413,  413,  413,  413,  413,  413,  414,  415,  417,  416,
  416,  419,  420,  420,  410,  410,  408,  409,  409,  421,
  421,  422,  422,  423,  423,  426,  426,  424,  424,  429,
  429,  428,  425,  427,  418,  418,  418,  418,  418,  418,
  418,  418,  343,  343,  155,   48,  287,  122,  148,  383,
  165,  186,  274,   63,   60,  222,  112,   66,  348,  218,
   35,   41,  356,  273,   39,   42,  215,   40,  300,  267,
  267,  430,  430,  430,  430,  430,  430,  430,  430,  430,
  430,  430,  430,  430,  430,  430,  430,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    7,    8,    6,    7,    5,    1,    2,    1,    2,
    1,    1,    3,    1,    1,    1,    1,    2,    2,    3,
    0,    3,    0,    1,    7,    8,    6,    7,    6,    7,
    4,    3,    1,    3,    3,    3,    2,    2,    2,    2,
    1,    3,    1,    2,    2,    1,    3,    1,    1,    3,
    1,    2,    2,    2,    3,    8,    1,    1,    5,    0,
    1,    3,    1,    3,    2,    4,    1,    3,    2,    3,
    4,    4,    2,    9,    2,    3,    7,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    4,    2,    2,    1,    0,    1,    1,    6,    6,    1,
    1,    0,    0,    1,    0,    4,    0,    1,    1,    2,
    8,    1,    0,    3,    0,    0,    1,    1,    2,    1,
    1,    1,    2,    6,    1,    1,    3,    0,    1,    1,
    0,    1,    1,    2,    3,    4,    2,    2,    3,    0,
    1,    1,    2,    1,    1,    1,    4,    3,    3,    3,
    3,    3,    1,    1,    6,    3,    1,    3,    1,    3,
    2,    1,    0,    2,    3,    1,    0,    1,    1,    2,
    3,    4,    0,    1,    2,    0,    1,    1,    5,    5,
    3,    2,    0,    1,    3,    1,    1,    6,    3,    2,
    6,    2,    1,    2,    2,    2,    0,    1,    1,    2,
    0,    2,    0,    1,    1,    1,    1,    1,    1,    0,
    1,    1,    2,    2,    2,    5,    1,    2,    3,    2,
    2,    0,    1,    1,    1,    1,    3,    4,    8,    4,
    0,    6,    1,    1,    2,    2,    0,    3,    3,    1,
    2,    2,    2,    8,    8,    8,    8,    3,    0,    2,
    0,    1,    3,    2,    1,    0,    1,    2,    4,    1,
    1,    3,    1,    0,    2,    1,    6,    1,    1,    3,
    1,    2,    0,    1,    2,    2,    2,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    2,    1,    1,    1,
    2,    2,    1,    2,    3,    4,    2,    6,    4,    1,
    1,    8,    9,    6,    8,    6,    4,    0,    4,    2,
    1,    3,    0,    2,    4,    0,    1,    1,    3,    3,
    7,    2,    0,    2,    2,    3,    3,    0,    1,    2,
    4,    1,    3,    2,    2,    2,    1,    1,    1,    1,
    2,    2,    5,    0,    4,    7,    7,    1,    1,    3,
    4,    7,    2,    1,    3,    1,    1,    6,    3,    0,
    0,    0,    0,    3,    0,    9,    9,    1,    1,    0,
    2,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    5,    5,    5,    5,    5,    5,    2,    0,
    4,    2,    3,    3,    2,    2,    3,    3,    2,    3,
    6,    1,    2,    4,    3,    2,    2,    2,    2,    3,
    1,    3,    3,    3,    2,    2,    4,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    0,    2,    2,    0,    1,    2,
    2,    2,    2,    2,    4,    3,    2,    2,    1,    3,
    7,    1,    0,    1,    0,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    3,    2,    2,    2,    1,    1,
    1,    1,    4,    5,    1,    3,    1,    3,    1,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    4,    4,    6,    5,    3,    0,    2,    2,    0,
    3,    0,    4,    1,    5,    4,    1,    4,    1,    2,
    2,    1,    1,    1,    2,    2,    2,    2,    0,    3,
    5,    1,    1,    2,    3,    1,    2,    3,    0,    1,
    1,    1,    5,    5,    6,    1,    1,    1,    2,    4,
    1,    0,    5,    1,    0,    3,    2,    1,    0,    2,
    0,    1,    0,    3,    1,    0,    1,    2,    1,    1,
    1,    1,    2,    2,    1,    2,    3,    1,    1,    3,
    3,    1,    2,    3,    1,    2,    0,    2,    1,    1,
    0,    1,    1,    1,    3,    2,    3,    0,    1,    3,
    4,    5,    1,    3,    4,    3,    0,    1,    3,    3,
    1,    1,    0,    1,    1,    4,    0,    2,    4,    0,
    3,    0,    2,    0,    2,    0,    9,   11,    0,    0,
    0,    0,    3,    2,    1,    0,    3,    5,    3,    2,
    5,    2,    1,    1,    1,    1,    1,    3,    1,    3,
    1,    0,    2,    1,    3,    1,    1,    1,    6,    1,
    0,    3,    0,    1,    3,    6,    3,    4,    2,    3,
    0,    1,    2,    1,    1,    1,    2,    1,    2,    1,
    2,    0,    3,    0,    1,    3,    1,    2,    0,    2,
    0,    2,    0,    4,    1,    2,    1,    1,    0,    1,
    3,    2,    1,    1,    2,    1,    4,    3,    1,    3,
    4,    0,    8,    5,    1,    1,    4,    4,    4,    1,
    1,    5,    5,    1,    3,    3,    1,    1,    1,    3,
    7,    7,    5,    3,    0,    1,    1,    1,    2,    0,
    1,    1,    1,    0,    3,    1,    3,    1,    0,    3,
    1,    3,    1,    3,    3,    1,    1,    3,    1,    2,
    1,    3,    3,    1,    2,    3,    3,    3,    3,    1,
    3,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    3,    3,    3,    3,    3,    3,    3,    3,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    1,    1,    5,    6,
    3,    4,    5,    6,    3,    4,    3,    4,    3,    4,
    4,    5,    4,    4,    3,    4,    1,    1,    1,    1,
    1,    1,    1,    3,    3,   11,   11,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    3,    3,
    3,    3,    3,    3,    3,    3,    1,    1,    1,    1,
    3,    1,    1,    1,    1,    1,    4,    1,    3,    1,
    2,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    1,    2,    1,    1,    1,    1,    1,    2,
    1,    1,    1,    1,    1,    4,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    6,    1,    6,    1,
    4,    7,    2,    0,    4,    3,    6,    1,    1,    6,
    6,    1,    1,    4,    6,    4,    5,    3,    5,    4,
    5,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    0,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  570,
  571,  572,  576,  577,  578,    0,  633,  745,  746,  750,
  751,  478,    0,    0,    0,    0,    0,    0,  412,  588,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  925,    0,   21,  923,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  496,    0,    0,   37,   34,
   36,    0,   35,    0, 1010, 1012, 1014, 1013, 1019, 1015,
 1022, 1020, 1016, 1018, 1021, 1017, 1023, 1024, 1025, 1026,
 1027,    0,    0,    0,    0,   32,   68,   66, 1002, 1011,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  665,    0,    0,    0,  702,    0, 1004,
    0,    0,    0,    0,  107, 1009,  579,    0,    2,    0,
    0,    0,    0,  442,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
  459,  460,  461,  462,  463,  464,  465,  466,  467,    0,
  440,  441,    0,  994,    0,  419,  998,    0,  416, 1001,
    0,  704,    0,  415, 1005,    0,  586,  921,    0,  150,
  103,    0,  104,    0,  218,  990,  786,    0,  102,  145,
  997,    0,    0,   98,    0,  105,  927,    0,   99,    0,
  100,    0,  101, 1008,  146,  106,  641,  642,    0,  926,
  924,    0,   74,    0,  995,    0,  993,  504,  497,    0,
  503,  508,  498,  499,  506,  500,  501,  502,  507,    0,
   30,    0,   39,  773,   38,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  931,    0,  912,  659,  659,  660,  670,  672,  983,  673,
    0,  674,  664,  660,    0,    0,    0,    0,    0,    0,
    0,  605,  594,    0,  597,  599,  600,  601,  602,  608,
  609,    0, 1003,  703,    0,  108,    0,  109,    0,  110,
    0,    0,    0,  634,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,  382,    0,  221,    0,  422,
  413,  476,  477,    0,    0,    0,    0,    0,    0,  431,
    0,  388,  389,    0,  592,  590,  922,    0,    0,    0,
    0,    0,    0,    0,  153,  132,    0,   96,    0,    0,
  111,    0,  121,  120,    0,    0,    0,    0,    0,    0,
  986,   75,    0,    0,    0,  747,  748,  505,    0,    0,
    0,    0,  890,  918,    0,  985,    0,  892,    0,    0,
  919,    0,  913,  904,  903,  902,    0,    0,    0,    0,
  894,  895,  896,  905,  906,  914,  915,    0,  916,  917,
    0,    0,    0,  920,    0,    0,    0,    0,    0,  908,
    0,  783,  887,  888,  889,  758,  893,  365,  873,    0,
    0,    0,  872,  759,    0,  869,  870,  871,  874,  875,
  876,  907,  932,  933,  934,  935,  948,  950,  958,  959,
  962,  963,    0,  776,    0,    0,   27,    0,    0,    0,
   71,   69, 1006,   67,   33,    0,    0,    0,  778,    0,
   52,    0,    0,    0,    0,    0,  584,    0,    0,    0,
  667,  669,    0,  676,    0,    0,  677,    0,  663,    0,
  631,    0,  606,    0,  603,    0,  604,  629,    0,  624,
    0,    0,  598,  787,  754,    0,    0,    0,    0,    0,
    0,  581,  580,    0,    0,  638,  635,    0,    0,    0,
  186,    0,    0,    0,  129,  484,  173,  174,    0,  487,
  482,    0,  488,    0,    0,    0,    0,  489,    0,  480,
    0,    0,  426,  428,  427,    0,  429,    0,  423,  219,
    0,    0,  272,    0,    0,    0,    0,  435,  436,    0,
  469,    0,    0,    0,    0,    0,    0,    0,  393,  392,
  492,    0,    0,  149,    0,    0,  158,    0,  157,    0,
    0,  164,  147,    0,  162,  165,  166,  154,    0,    0,
  527,  562,  530,    0,  563,    0,    0,  531,    0,    0,
    0,  554,  566,  526,  549,  528,  529,  524,  511,  382,
  509,  510,  512,    0,    0,  521,  522,  523,  525,    0,
    0,    0,    0,    0,  112,    0,  113,    0,  124,    0,
    0,  202,    0,    0,  380,    0,  221,    0,    0,   83,
    0,    0,  768,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  789,  791,  800,  803,  804,  805,
  806,  807,  808,  809,  810,  811,  812,    0,    0,    0,
  909,    0,    0,    0,    0,    0,    0,  910,  911,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  330, 1007,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  775,   28,    0,    0,   44,
   72,    0,    0,   26,    0,  781,    0,    0,    0,  744,
    0,    0,    0,    0,    0,  583,    0,  659,    0,    0,
    0,    0,  681,    0,  574,    0,  929,  573,    0,  607,
    0,  615,  611,  610,    0,    0,    0,  626,  614,    0,
    0,  752,  753,    0,    0,    0,  380,    0,    0,    0,
    0,    0,  632,    0,    0,    0,  130,    0,  486,    0,
    0,    0,    0,    0,  382,  425,    0,  382,  383,  430,
  519,    0,  274,    0,  268,  270,  380,  241,  991,    0,
    0,  382,    0,    0,  210,  380,    0,    0,    0,    0,
  246,  240,  243,  244,  245,  434,  474,  473,  433,  432,
    0,    0,    0,    0,    0,    0,  391,    0,  382,    0,
  155,  159,    0,    0,  163,    0,    0,  550,  551,  567,
    0,  564,    0,    0,  557,  555,  556,    0,    0,    0,
    0,  520,    0,    0,    0,    0,  558,    0,    0,  380,
    0,  201,    0,  204,  206,  207,    0,    0,    0,    0,
    0,  221,   78,    0,    0,   77,  538,  987,  539,    0,
    0,    0,    0,  790,  802,    0,    0,  860,  861,  862,
    0,  794,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  975,  976,  977,  978,  979,
  980,  981,  982,    0,    0,    0,    0,    0,    0,  968,
    0,    0,    0,    0,  885,  332,  886,  956,    0,    0,
  881,  989,    0,    0,    0,    0,    0,  760,    0,  891,
  785,  784,  777,    0,    0,    0,    0,    0,    0,    0,
    0,   53,   61,   63,    0,    0,   70,    0,    0,    0,
  780,    0,    0,    0,    0,    0,    0,    0,    0,  668,
  671,    0,    0,    0,    0,  686,  687,  688,    0,    0,
  678,  999,  680,  575,  930,  616,  625,  620,  619,    0,
  630,  788,  755,  756,  380,    0,    0,  661,  640,  639,
    0,    0,  651,  655,  182,  176,  183,    0,  179,  485,
    0,    0,  178,    0,  189,  170,  169,  928,  172,  171,
  490,    0,    0,    0,  424,  273,    0,  212,  382,  214,
    0,  383,  209,  215,    0,    0,    0,    0,  437,  438,
  382,  380,  383,    0,    0,    0,  409,    0,  197,  198,
  156,    0,  168,    0,    0,    0,    0,    0,  565,    0,
    0,  382,  383,    0,  515,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  199,    0,
    0,    0,    0,  369,    0,   82,    0,    0,    0,    0,
   87,   84,    0,    0,    0,  749,  992,    0,    0,  795,
    0,    0,  801,    0,    0,    0,    0,    0,  838,  837,
    0,    0,    0,  845,  863,  855,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  793,    0,    0,    0,
  221,    0,  936,    0,    0,  951,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  964,    0,
  974,    0,    0,    0,    0,    0,    0,  955,  897,   65,
   57,   58,   64,   59,   60,    0,    0,   22,    0,    0,
    0,   42,    0,    0,    0,  762,  782,    0,    0,  761,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  705,    0,    0,    0,    0,    0,  622,  623,  627,
    0,  200,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  190,    0,    0,    0,  420,  224,  225,  226,  227,
  228,  229,  900,    0,    0,    0,  277,    0,  222,    0,
    0,    0,    0,  247,    0,  470,  471,  439,    0,    0,
    0,    0,    0,    0,  382,  494,  383,  167,  988,    0,
    0,  533,    0,  532,  568,    0,  560,    0,    0,    0,
  513,    0,  541,    0,    0,  543,  546,  548,  380,    0,
    0,    0,  119,    0,  205,  382,    0,    0,  381,    0,
   97,    0,  117,    0,   93,    0,    0,    0,    0,   86,
    0,  853,  854,  797,    0,  799,    0,    0,    0,    0,
    0,    0,    0,  856,    0,    0,    0,    0,    0,    0,
    0,  846,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  942,  941,    0,    0,
  938,  937,    0,  946,  945,  944,  943,  940,  939,    0,
    0,  970,    0,    0,    0,  969,    0,    0,    0,    0,
    0,    0,   56,   62,   54,   55,   23,  766,  767,    0,
  764,  771,  772,  769,    0,  743,    0,    0,    0,  697,
    0,    0,    0,  711,  707,  709,    0,    0,    0,    0,
    0,  381,  644,  645,  662,    0,  184,  180,    0,    0,
  195,  191,    0,  383,  901,  283,    0,    0,  383,  278,
    0,    0,    0,  232,    0,    0,  248,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  491,  535,    0,  536,
    0,  134,    0,    0,    0,  138,  140,  141,  142,  516,
  518,  514,  545,    0,    0,  383,    0,    0,    0,    0,
  366,    0,  370,  382,   88,    0,    0,   90,    0,    0,
    0,    0,    0,    0,  865,  864,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  220,  957,  947,  949,    0,    0,
  966,  965,    0,  971,  960,  961,    0,   40,    0,    0,
    0,  700,  698,    0,    0,    0,  715,    0,    0,  657,
  383,  367,  646,    0,  175,  192,  383,  144,    0,    0,
  280,  281,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  364,    0,    0,    0,    0,
  289,  294,  288,    0,    0,    0,  298,  299,  300,  301,
  302,  303,  305,  306,  308,  309,  310,  313,    0,    0,
    0,  358,  359,  360,    0,  267,    0,    0,  208,  233,
    0,    0,    0,    0,  234,  237,    0,    0,  383,  407,
  408,  404,  403,  406,  405,    0,  534,  561,  143,  131,
  139,    0,  382,  264,  126,  383,    0,    0,  384,  371,
    0,    0,   91,   92,   79,    0,  824,  832,    0,  825,
  833,  828,  836,    0,    0,  826,  834,  827,  835,  821,
  829,  822,  830,  823,  831,    0,  952,  967,    0,  763,
    0,    0,  690,    0,    0,  725,  727,    0,  722,    0,
  265,  185,  421,    0,  279,    0,  317,    0,    0,  320,
  321,    0,  312,    0,    0,  311,  344,  345,    0, 1000,
  361,  362,    0,  297,  304,    0,  307,  290,    0,  295,
    0,  349,  296,  314,    0,    0,  342,  211,  216,  235,
    0,    0,  238,    0,  250,  252,  411,  383,    0,  386,
    0,    0,    0,  376,  377,    0,    0,   76,   94,    0,
    0,    0,    0,  689,    0,  716,  726,    0,    0,  286,
  282,    0,  315,    0,    0,    0,    0,    0,  346,    0,
    0,  337,  338,    0,  357,    0,    0,    0,    0,  352,
  350,    0,    0,    0,  239,    0,    0,    0,    0,    0,
  266,  383,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  694,    0,    0,  733,    0,    0,    0,  285,    0,
  316,    0,    0,  319,    0,  327,    0,  329,    0,    0,
  356,  355,  996,  354,    0,    0,    0,    0,    0,    0,
    0,    0,  249,    0,  256,    0,  255,  387,  379,    0,
    0,  372,  118,    0,    0,    0,    0,  692,  735,  724,
    0,    0,  736,    0,  732,    0,    0,    0,    0,    0,
    0,  340,  339,  351,  353,    0,    0,    0,  236,    0,
    0,  260,  259,  258,    0,    0,    0,    0,  695,  731,
    0,    0,  287,    0,    0,  318,  324,    0,    0,    0,
    0,    0,  263,  261,  262,    0,    0,  867,  658,    0,
    0,    0,  738,    0,    0,    0,    0,    0,  341,    0,
    0,    0,  737,    0,  335,  325,    0,  347,  322,    0,
  740,    0,  323,  866,  741,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1567,   28,   29, 1568,
   31,   32,   33,   34,   35,   36, 1569,   38, 1570,  653,
  122,  466,  970, 1208,  123,  719,  971,  124,  470,  724,
  148,  125,  126,  263,  993,  138,  972,  973,  203,  232,
  127,  974,  128,  472,   82,  243,  382,  429,  648, 1109,
  430,  875,  619,  225,  852,  649,  650,  651, 1111,  244,
 1398,   74,  193,   75,  239, 1880, 1321,  224,  229,  231,
  233,  219,  211,  213,  226,  236,  155,  316,  318,  320,
  371,  635,  637,  463,  264,  801,  552,  663, 1045,  375,
  640, 1097, 1313,  585,  747, 1071,  534,  535,  567,  367,
  620, 1083, 1464, 1599, 1256, 1465, 1466, 1467, 1468, 1469,
  664,  220,  212,  363,  593,  364,  365,  594,  595,  596,
  597, 1288, 1039,  537,  538, 1028, 1026,  539, 1033, 1027,
 1246, 1427, 1034, 1035, 1251,  377,  863,  864,  865,  866,
  803, 1052, 1442,  621,  805,  432,  868,  951,  561, 1370,
 1257,  433,  434,  435,  436,  437, 1443, 1444,  570, 1605,
 1768, 1274,  811,  812,  798,  813,  814,  815, 1769, 1770,
 1853,  345,  565, 1265, 1571,  199,  562,  563, 1266, 1267,
 1437, 1550, 1551, 1552, 1741, 1116,  730, 1572, 1573, 1574,
 1575, 1701, 1576, 1577, 1578, 1579, 1580, 1581, 1582, 1583,
 1584, 1585, 1586, 1587, 1588, 1589,  438, 1838, 1682, 1590,
 1751, 1871, 1748,  439,  703,  440,  441, 1591, 1702, 1759,
 1760, 1802, 1592, 1593, 1594, 1693, 1102, 1319, 1481, 1103,
 1104,  291,  495, 1628,  386, 1237, 1420, 1722, 1723, 1724,
 1725, 1235,  354,  581,  829, 1286,  582,  827,   79,   59,
  349,  204,  163,  332,  190,  339,  196,  789,  786,  340,
  350,  819,  351,  577, 1063,  191,  442,  333,  549,  583,
 1287,   96,  202,  248,  622,  623,  791, 1084,  792, 1085,
 1086,  626,  627,  628,  629, 1077,  879,  630,  631,  632,
  845,  633,  842,   40,   41,   42,   43,   44,   45,  157,
  523,   61,  488,  207,  356,  303,  304,  305,  306,  307,
  308,  309,  310,  311,  753,  509, 1010, 1240,  510,  511,
  758,   46,  162,  326,  530,  773,   47,  525,  526, 1019,
 1425, 1244, 1543, 1023,  142,  741, 1540,  491,  293,  143,
  144,  443,  292,  496,  497,  444,  744, 1003,  994,  995,
  996,  997,  998, 1662, 1734,  445, 1783, 1229, 1536, 1537,
 1785, 1670, 1786, 1787, 1788, 1835, 1881, 1883,  478,   48,
   49,  387,   50,   51,  517,  518,  979, 1216, 1400, 1404,
  479,  727,  217,  665,  666,  893,  894,  667,  668,  669,
  670,  671,  672,  673,  674,  675,  676,  677, 1342, 1132,
 1134, 1135,  950,  702,  446,  447,  448,  449,  450,  451,
  452,  241,  453,  454,  455,  456,  457,  934,  458, 1520,
  459,  460,  461,  462,  693,  938, 1192, 1187,  694,  130,
};
static short yysindex[] = {                                   5872,
 1280,  -90, 3340,   93, 1374,  189, 3249,    8,10481,   45,
  497, 9521,  -90, 1222,  709, 7792,  498, 7792,  288,    0,
  766,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  536,    0,    0,    0,    0,
    0,    0,10704, 7792, 7792, 7792, 7792, 7792,    0,    0,
  488, 1367, 7792, 7792, 7792,  761, 7792,  573, 7792, 7792,
    0, 7792, 7792,    0,  868,    0,  583,    0,    0,  856,
 7792,    0, 7792, 7792, 7792,  873, 7792, 7792, 7792, 7792,
  573, 7792, 7792, 7792, 7792,    0, 7792,  682,    0,    0,
    0, 1073,    0, 1073,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  846,  852,  202, 1259,    0,    0,    0,    0,    0,
 7792, 7792, 7792, 1041, 1111, 1117,  128, 1266, 1070,   87,
  121, 1027, 1088,    0, 7792, 1224, 1216,    0, 1193,    0,
 7792, 7792, 7792, 7792,    0,    0,    0, 7792,    0, 1362,
    3, 1124,  993,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  899,
    0,    0, 1367,    0,   68,    0,    0, 1612,    0,    0,
 1413,    0,  547,    0,    0, 1235,    0,    0, 1246,    0,
    0, 1402,    0, 1421,    0,    0,    0, 1367,    0,    0,
    0,  605, 1612,    0,   24,    0,    0, 1361,    0, 1450,
    0, 1073,    0,    0,    0,    0,    0,    0, 1426,    0,
    0, 1324,    0, 1473,    0, 1341,    0,    0,    0, 1324,
    0,    0,    0,    0,    0,    0,    0,    0,    0,10448,
    0, 7792,    0,    0,    0, 8298, 1412, 7927, 7792,  912,
 1517, 1784, 1513, 1557, 8298, 1485, 7927, 1497, 1508, 1389,
    0,12139,    0,    0,    0,    0,    0,    0,    0,    0,
 9720,    0,    0,    0, 1470, 7792, 1567, 1468,   -6, 7792,
 1490,    0,    0, 1216,    0,    0,    0,    0,    0,    0,
    0, 7792,    0,    0, 1612,    0, 1361,    0, 1073,    0,
 1556,12139, 1531,    0,12139, 1640,  402, 1467,  -82, 1474,
 1029,  993,    0, 1706, 1187,    0, 7792,    0,  899,    0,
    0,    0,    0, 7792, 1583, 6655, 1533, 7371, 1960,    0,
10704,    0,    0, 1090,    0,    0,    0, 1944, 1944, 1367,
 1637, 1367, 1615, 1402,    0,    0,10958,    0, 7792, 7792,
    0, 1583,    0,    0, 1720,  -40, 1970, 7792, 1747, 7792,
    0,    0, 1972,  186, 7213,    0,    0,    0, 1974, 1983,
 1984, 1565,    0,    0, 1989,    0, 1992,    0, 1993, 1998,
    0, 1999,    0,    0,    0,    0,    0, 2001, 1566, 1579,
    0,    0,    0,    0,    0,    0,    0, 2004,    0,    0,
10545, 2009, 2010,    0,12139,12139, 9818, 7792, 2012,    0,
 3012,    0,    0,    0,    0,    0,    0,    0,    0, 2011,
 1962,    0,    0,    0, 2008,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1182,    0, 7792, 1621,    0, 7792, 7792,   72,
    0,    0,    0,    0,    0, 7792, 7792,  974,    0, 7792,
    0, 1739, 7792, 2016, 8298, 1660,    0, 7792,10784, 1825,
    0,    0,12139,    0, 1743, 2020,    0, 1807,    0,   90,
    0,  -25,    0,  614,    0,  614,    0,    0, 2023,    0,
  173, 1646,    0,    0,    0, 2029,  154, 2015, 1583, 2038,
 1813,    0,    0, 2037, 2025,    0,    0, 1129, 1644, 1633,
    0,  161, 1744, 1746,    0,    0,    0,    0, 1367,    0,
    0, 1748,    0, 1944, 1944, 1944, 1944,    0, 2047,    0,
 1815, 1817,    0,    0,    0, 1799,    0,10958,    0,    0,
10958, 1483,    0, 1612, 1834, 7792,12768,    0,    0,  781,
    0, 7792, 1383, 1413, 1704, 1666, 1642,    0,    0,    0,
    0,  670, 1707,    0, 1656,  573,    0, 1367,    0, 1841,
 1772,    0,    0, 1615,    0,    0,    0,    0,  389, 1674,
    0,    0,    0, 1675,    0, 1724, 2085,    0, 1778, 2089,
  939,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2042,  672,    0,    0,    0,    0,  201,
 2090, 2095, 2089, 1612,    0, 1819,    0, 1875,    0, 1367,
 1367,    0, 6655,  547,    0, 1757,    0, 1731, 2102,    0,
 7054, 4605,    0, 1759, 1767, 2111, 7704, 2117,    0,    0,
    0, 6954, 9529,   19,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1054,11023,  453,
    0, 7792,12139, 1275, 1330, 1386, 2508,    0,    0,12139,
 7856, 3012, 1713,  774,12139,11023,  791,  791, 1222, 1956,
 1500, 2120,    0,    0,11103, 7792,12139,12139,12139,12139,
12139,11023,12139,  960, 7792,    0,    0, 8334, 1732,    0,
    0, 1702, 7927,    0, 1850,    0, 1710, 1222, 2131,    0,
 1850, 8334, 1852, 1853, 7792,    0, 1956,    0, 2113, 2723,
 1722,11023,    0, 7792,    0,  782,    0,    0,  580,    0,
 1735,    0,    0,    0, 7792,  522, 7792,    0,    0, 7792,
 7792,    0,    0,11023, 1923, 6655,    0, 1688,12139,12139,
 1806, 1687,    0, 1367, 1367, 1367,    0,  560,    0, 1656,
 1656, 1750,  573, 1029,    0,    0, 2154,    0,    0,    0,
    0, 1921,    0, 7792,    0,    0,    0,    0,    0, 1927,
 2162,    0, 1911, 1928,    0,    0, 1873, 1888, 1073, 1955,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  573, 7792,12858, 1836, 1848, 1849,    0,  573,    0, 1585,
    0,    0, 1824, 1367,    0,  199, 1822,    0,    0,    0,
  573,    0, 2085,  106,    0,    0,    0, 1943,  106, 1828,
  106,    0, 2209,  573,  573,  573,    0, 1583, 7792,    0,
 1896,    0, 1736,    0,    0,    0, 1090, 1837, 7792, 1798,
  196,    0,    0, 2213, 1985,    0,    0,    0,    0,  186,
 7792, 1840, 8095,    0,    0, 1840, 7704,    0,    0,    0,
 7945,    0,  130,   31,12139,12139, 8192, 2226,  342, 8450,
12139, 8602, 1009, 8711, 8970,11342, 9122, 9231, 9490, 7856,
 7856,12139,12139, 2005,12139, 2227,12139, 2229, 2354,12139,
12139,12139,12139,12139,12139,    0,    0,    0,    0,    0,
    0,    0,    0, 1950, 1613,  538,12139,  829,11023,    0,
 7856, 2064, 2232, 1027,    0,    0,    0,    0, 3012, 1805,
    0,    0, 2014,  791,  791, 1260, 1260,    0, 1004,    0,
    0,    0,    0, 7792, 7792, 7792, 7792, 7792, 7792,  143,
 2234,    0,    0,    0, 8334, 1894,    0,  452, 1914, 7792,
    0, 1027,11023, 1914, 2240, 2234, 8334, 7442, 1968,    0,
    0, 1400, 2249, 2247, 1267,    0,    0,    0, 7856, 1969,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -56,
    0,    0,    0,    0,    0, 1814, 1837,    0,    0,    0,
 3012, 7792,    0,    0,    0,    0,    0, 1820,    0,    0,
 1944, 2034,    0,  560,    0,    0,    0,    0,    0,    0,
    0, 2265, 7856, 4106,    0,    0, 2022,    0,    0,    0,
 4106,    0,    0,    0, 1073, 1073, 1808, 2273,    0,    0,
    0,    0,    0,  -69,  420, 1116,    0, 2050,    0,    0,
    0, 7792,    0,   93, 1833, 1901, 1921, 2283,    0, 1656,
 1863,    0,    0, 1160,    0, 2267, 7792, 1320,  573, 2285,
 2286, 2287, 2066,  547, 2022, 1944, 1746, 6655,    0, 1707,
 1027, 1884, 1892,    0, 2416,    0,  573, 1939, 2031, 7054,
    0,    0,  573, 1851, 2041,    0,    0, 2305, 2311,    0,
 7856, 7856,    0, 7856, 7856, 2680, 3012, 2317,    0,    0,
 3012, 2319,11581,    0,    0,    0, 2017, 2326, 3012, 2327,
 1919, 2332, 3012, 2333,12139,12139, 2226,12139,11661, 2334,
 3012, 2335, 2339, 3012, 2340,12139, 3012, 2346, 3012, 2349,
 2351, 3012, 2352, 2360, 3012, 2362,    0, 2141, 2571, 2612,
    0, 2816,    0, 2893,12139,    0, 2921, 2998, 3115, 3123,
 3202, 3225,12139,12139,11023, 3012, 1971,11023,    0,12139,
    0, 2097,  625,12139,11023,12139,12139,    0,    0,    0,
    0,    0,    0,    0,    0, 2080, 8334,    0, 8334,  143,
 2234,    0,  233,  233,   83,    0,    0, 9720, 1878,    0,
 2240, 2234, 2364, 7442, 2368, 1267, 2371,11023, 1807, 2723,
 2030,    0, 2044, 2045, 2093, 2167, 2112,    0,    0,    0,
 2022,    0, 1884, 1318, 2400, 2024, 1367, 2055, 1656, 2059,
 1656,    0, 7856,  449, 1522,    0,    0,    0,    0,    0,
    0,    0,    0, 2006, 2176, 2022,    0,12139,    0, 2164,
 2048, 1808,  868,    0, 7856,    0,    0,    0, 2068, 2074,
 2075, 2076, 2077, 2084,    0,    0,    0,    0,    0, 2431,
   93,    0, 1750,    0,    0,  106,    0, 4106, 2101,  106,
    0,  106,    0, 1921, 2432,    0,    0,    0,    0, 1090,
 2176, 1656,    0, 1746,    0,    0, 9720, 2204,    0,   47,
    0, 2400,    0, 2448,    0, 2449, 1367,  -42, 2452,    0,
 1367,    0,    0,    0, 2141,    0, 2141,12139, 2078, 2078,
 1222, 2453, 1879,    0, 2078, 2078,12139, 2078, 2078, 2931,
 3012,    0, 2000,12139, 3012, 2078, 2078, 2078, 2078, 3012,
 2078, 2078, 2078, 2078, 2078, 2078,    0,    0,10958, 2457,
    0,    0, 3239,    0,    0,    0,    0,    0,    0, 3263,
 1437,    0,11023, 2192, 2069,    0,11023, 3302, 1905, 1839,
 3012, 2116,    0,    0,    0,    0,    0,    0,    0, 2070,
    0,    0,    0,    0, 1743,    0, 7442, 2364, 1807,    0,
 1915, 7792, 1267,    0,    0,    0, 2723,12139, 7856, 2109,
 2176,    0,    0,    0,    0, 1944,    0,    0, 1367, 1585,
    0,    0,  463,    0,    0,    0, 7792, 8845,    0,    0,
 3012, 2233, 2164,    0,  865, 7792,    0, 2175,  470, 2079,
 2181, 2081, 2219, 2184, 2224, 2022,    0,    0, 2485,    0,
 2486,    0, 2178, 2233, 2101,    0,    0,    0,    0,    0,
    0,    0,    0, 2022, 1707,    0, 1585, 2050, 2208, 2149,
    0, 1837,    0,    0,    0, 2168,  -46,    0, 1921, 2169,
 3012, 2494, 2497, 1027,    0,    0, 2500, 2501, 3012, 2506,
 2509,12139,12139, 3012, 2510, 2511, 2519, 2521, 2522, 2528,
 2529, 2530, 2533, 2534,    0,    0,    0,    0,12139, 2537,
    0,    0,11023,    0,    0,    0, 1722,    0,  233, 1722,
 7792,    0,    0, 1062, 3012, 2535,    0,   19,  722,    0,
    0,    0,    0, 1656,    0,    0,    0,    0,10958, 2523,
    0,    0, 2288,    0, 6724,  685, 2526, 2547, 2248,12139,
 2539, 2541, 7021, 7792, 7792,    0, 2542, 2548, 2252, 2549,
    0,    0,    0, 2281,11243, 2556,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2558,  -12,
 2563,    0,    0,    0,    0,    0, 2582, 7792,    0,    0,
 2272, 2282, 7792, 1808,    0,    0, 1073, 7792,    0,    0,
    0,    0,    0,    0,    0, 2176,    0,    0,    0,    0,
    0, 2176,    0,    0,    0,    0, 3028, 1722,    0,    0,
12139, 1367,    0,    0,    0, 1367,    0,    0,12139,    0,
    0,    0,    0, 3012, 3012,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 3012,    0,    0, 1969,    0,
 1969, 2585,    0, 7856,12139,    0,    0, 2259,    0, 2588,
    0,    0,    0,  688,    0, 2312,    0,11900, 7792,    0,
    0,12139,    0, 7856, 3012,    0,    0,    0, 2574,    0,
    0,    0, 7792,    0,    0, 5975,    0,    0,  765,    0,
 2186,    0,    0,    0,  409, 2595,    0,    0,    0,    0,
 1808, 1073,    0, 2262,    0,    0,    0,    0, 2050,    0,
 3028, 2602, 1267,    0,    0, 1969, 3012,    0,    0, 1839,
 2112, 2112, 7792,    0,   19,    0,    0, 7605, 4106,    0,
    0, 2608,    0, 2957,10448, 1745,  586, 2592,    0, 2316,
  331,    0,    0, 2616,    0, 7792, 7792,   93,  632,    0,
    0, -125, 2324, 7856,    0, 2262,  545, 1808, 2290, 2292,
    0,    0, 1267, 2624, 3028, 2328, 2112, 2635, 1722, 2109,
 2109,    0, 1916, 7792,    0, 2637, 2639, 1227,    0, 1840,
    0, 2269, 5975,    0, 2246,    0, 5975,    0, 5975, 3331,
    0,    0,    0,    0, 8845,  765,12139, 5975,  615, 1808,
  -21,  -21,    0, 2391,    0, 2251,    0,    0,    0, 1267,
 3028,    0,    0, 1969, 2304, 2304, 7792,    0,    0,    0,
 7605, 2654,    0, 7792,    0, 2662, 2094, 2645, 1563, 8845,
 2661,    0,    0,    0,    0, 1251,  674, 2403,    0, 2253,
  591,    0,    0,    0, 1170, 2112, 1124, 1124,    0,    0,
 7792, 2373,    0, 5975, 2661,    0,    0, 2408, 5975, 2424,
 2420, 8845,    0,    0,    0, 7856, 2109,    0,    0, 2682,
 2694, 2696,    0, 1924, 8845,  635, 7792, 8845,    0,   19,
 2304, 7792,    0, 7792,    0,    0, 8845,    0,    0, 1124,
    0, 2698,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  211, 1098,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  165, 7293,    0,    0,    0,    0,    0,    0,
 2742,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  256,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  995,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2406,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2361,    0,    0,
    0,  230,    0,  230,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2367,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  998,    0,
    0,10057, 7552,    0,    0,    0, 1061,    0,    0,    0,
 4633,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  137,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  595,    0,    0,
    0,    0,  927,    0,    0, 1180,    0,    0,    0,    0,
    0,  831,    0,11516,    0,    0,    0,    0,    0,    0,
    0,    0,  595,    0,  489,    0,    0, 2705,    0,    0,
    0, 2487,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1245,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1311,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1071,    0,    0,    0,    0,    0,    0,    0, 8810,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  693,    0,    0, 1328,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  595,    0, 2705,    0, 2487,    0,
 1343,    0,    0,    0,    0,  168,    0,    0,    0,    0,
    0, 1368,    0, 2474,    0,    0,    0,    0, 1394,    0,
    0,    0,    0,    0, 2495,  675,11820,    0, 1399,    0,
    0,    0,    0,  345,    0,    0,    0, 2302, 2302,    0,
    0,    0, 1433, 1268,    0,    0,    0,    0,    0,    0,
    0, 2495,    0,    0, 1452,    0,    0,    0,    0,    0,
    0,    0, 4893, 2377,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1975,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2687,    0,    0,
    0, 2991, 3295,    0,    0,    0,    0,    0,    0,    0,
 2056,    0,    0,    0,    0,    0,    0,    0,    0,  185,
 3599, 2378,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1443,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1444,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2442,    0, 3633,    0,    0,
    0,    0,    0,  588,    0,  588,    0,    0,  737,    0,
 1059, 1326,    0,    0,    0,    0, 1245,    0, 2495,    0,
    0,    0,    0,  179,  322,    0,    0,  262,    0,  812,
    0, 4537,    0,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2302, 2302, 2302, 2302,    0,  861,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2505,    0,    0,    0,    0,    0, 2474,
    0,    0, 1315,    0, 2300,    0,    0,  689,    0,    0,
    0,    0,  225,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1493,    0,    0,    0,    0, 3929,12476,
    0,    0,    0,12488,    0,    0,12500,    0,    0,12698,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1987, 2390,    0,    0,    0,    0,12751,
 8453,    0,12698,  595,    0,    0,    0,    0,    0,    0,
    0,    0,  675, 1511,    0,    0,    0,    0, 2366,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  823, 5697,
 5945,    0,    0,  992,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12219,    0,12219,
    0,    0,    0,12219,12219,12219,    0,    0,    0,    0,
    0, 2320,    0,    0,    0,    0, 4639, 4943,10209, 2728,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  332,    0,    0, 7293,    0,    0,
  332,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1094,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  659,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  675,    0,  206,    0,    0,
  311,    0,    0,    0,    0,    0,    0, 5577,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 3588,    0,    0,    0,    0,    0,    0,    0, 2733,
    0,    0, 4264, 4031,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  377,  840,  858,    0,    0,    0, 1478,
    0,    0,    0,    0,    0,    0, 5296,    0,    0,    0,
    0,    0,12500,    0,    0,    0,    0,  702,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2495,    0,    0,
  323,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 9992,    0,    0,    0,  156,    0,    0,    0,    0, 2472,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12219,    0,    0,    0,    0,  976,    0,
    0,    0, 3903, 5159, 5463, 4119, 4423,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1504,
 1510,    0,    0,    0,    0,    0,    0,    0, 1512,    0,
    0,10057,    0, 1512, 1516, 1528,    0,    0,    0,    0,
    0,    0, 6008,  578,  174,    0,    0,    0,    0, 1155,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  347,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2302, 2325,    0, 5795,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2513,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1345,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1539,    0,    0,
    0,    0,    0,    0,    0,    0, 2231,    0,    0,    0,
    0,    0,    0,    0,    0, 1445,    0,    0,    0,    0,
    0,    0,    0, 1511, 2513, 2302, 1580,  675,    0, 2518,
10057, 1605,  132,    0,    0,    0,    0, 9992,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1730,    0,    0,    0,
 1810,    0,    0,    0,    0,    0,    0,    0, 2458,    0,
 2767,    0, 3071,    0,    0,    0,    0,    0,    0,    0,
 3375,    0,    0, 4320,    0,    0, 4840,    0, 5269,    0,
    0, 5367,    0,    0, 5861,    0,    0, 1292,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2350,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1504,
 1608,    0, 2356,  336,    0,    0,    0,    0,    0,    0,
 1634, 1638, 1662,    0,    0, 2444,  903,    0, 6238,    0,
 2451,    0, 2456, 2459,    0,    0,  114,    0,    0,    0,
 2513,    0, 1605,    0,  367, 3713,    0,    0,    0, 2337,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 9552,    0, 2525,    0,    0,    0, 2103,
    0, 1345, 2406,    0,    0,    0,    0,    0,  874,  876,
  886,  944,  964,  968,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  769,    0,
    0,    0,    0, 4448,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1686,    0,    0,    0,    0,    0,    0,
    0, 1694,    0,    0,    0,    0,    0,  312,    0,    0,
    0,    0,    0,    0,  244,    0,  289,    0,    0,    0,
10209,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 6259,    0, 6375,    0, 6386,    0,    0,    0,    0, 6504,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2759,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1457,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1700, 6041,    0,
    0,    0,  440,    0,    0,    0,    0,    0,    0,  368,
    0,    0,    0,    0,    0, 2302,    0,    0,    0, 1067,
    0,    0,    0,    0,    0,    0,    0,  593,    0,    0,
 2762, 1606, 2136,    0, 2474,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2513,    0,    0,    0,    0,
    0,    0,    0, 1764,  895,    0,    0,    0,    0,    0,
    0,    0,    0, 2513, 2518,    0,  344,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  119,    0,
 6527,    0,    0,12219,    0,    0,    0,    0, 6591,    0,
    0,    0,    0, 6733,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  111,    0,  336,  272,
 1190,    0,    0, 2444,  175, 1241,    0,  258, 2764,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10383,    0,    0,    0,    0,    0,    0,
    0, 1641, 1721,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  696,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1740,    0,    0,    0,    0,    0,
    0,    0,    0, 4449,    0,    0,  148,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  523,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6765, 6861,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2765,    0,    0,  142,    0,
  169,  926,    0,    0,    0,    0,    0, 2769,    0,    0,
    0,    0,    0, 2746,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2751,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2504,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 4449, 1929,    0,  235,    0,    0,    0,    0,    0,    0,
    0,  357,  834,    0,    0,  475, 2762,    0,    0,    0,
  125,  140,    0,    0, 2301,    0,    0, 2764,    0,    0,
    0,    0,    0,    0,  129,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  940,    0, 1345, 1803, 1941,
    0,    0, 2444, 1484,    0,    0,  170,    0,  111,   56,
  153,    0,    0, 1428,    0,    0, 2774,    0,    0,    0,
    0, 2757,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  603,    0,    0,    0,    0, 4449,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1189,
    0,    0,    0,  142,   28,  118,    0,    0,    0,    0,
 2764,    0,    0,    0,    0,    0,    0,    0,    0,  603,
 2760,    0,    0,    0,    0,    0, 2516,    0,    0,    0,
    0,    0,    0,    0, 2444,  125, 2780, 1331,    0,    0,
    0, 1930,    0,    0, 2766,    0,    0,12735,    0,    0,
    0,  603,    0,    0,    0,    0,   56,    0,    0, 2782,
    0,    0,    0,    0,  603,    0,    0,  603,    0, 5153,
   28,    0,    0,    0,    0,    0,  603,    0,    0, 2780,
    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2824,    0,    0, 2827,
    0,    0,    0,    0,    0,    0,    5,    0, 2828,   -2,
  274,  -89, -601, 1623,  276, -185,  -65, 2822, 2559,    0,
  -27,    0, 2565,  -93,  -31,    0,  777, -370,  -20,  -60,
  -38, -186, 2570, 2119,    0,    0,    0, 1257,    0,    0,
  -36, 1733, -538,  -68, -897, 2193, 1976, -298, 1738, 2763,
 -297,    0,  -58,    0, -481,  -51,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -177, -143, 1742, -316, 1030, -954,    0,
    0,    0,    0, -342, -128, -575, 1752, -339,  -55,    0,
 -528,    0,    0, 1388, -998,    0, 1390,    0,    0, 2531,
 -659,    2,    0,    0,    0,    0, 2489,    0, 2266, 2545,
 -229, 1775, -733,    0,    0,    0, -705,    0,    0,    0,
    0,    0,    0, 1830,    0, 2552, 2100, 1776, 2527, 2536,
    0, 1201,    0, -496, 2087,  -32, -639, 1286,    0,    0,
    0, -955, -944, -940, -936, -934,    0, 1442, -975,    0,
 1130,-1179, 1455,    0, 2329,    0,    0,    0, 1132, 1134,
 1092,   44, -277, -931, -564,    0,    0, 2115,    0, 1645,
    0,    0,    0,    0,    0,-1275, -849, -535,    0,    0,
    0,    0, -192,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1162,    0, 1151,    0,
  783,    0,    0,  392,    0, -376,  660, 1351, 1219,    0,
 1123,    0,    0,    0,    0,    0, 1906, 1679, 1509,    0,
 -986, -852, -870,    0, -501, -617, -722,    0,-1420, 1209,
  330, -389, -510, -711, -839,-1241,    0,    0,  -39,    0,
    0,    0,    0,    0, 2586,    0,    0,    0,    0, 2606,
 2376,    0,    0,    0,    0,    0,   -9, 2621,    0,    0,
    0,    0,  -15,    0,    0,    0, -353, 2104, -345, 1654,
 -472,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2323,    0, 2114,    0,    0,    0,    0,    0,    0,   32,
    0, 2945,    0,    0,    0,    0,    0, 2657,    0,    0,
    0,    0, 2467,    0, 2464,    0,    0,    0, 2216,    0,
    0,    0, -449, -204,    0,    0,  582,    0, 2221,    0,
    0,    0,    0,    0, -574,-1197, -521, -179, 1276,    0,
 2834, -122, 1152,    0, 2250, -281, -958,-1339,    0, -851,
 1996,    0,    0,    0,    0, -136,    0,    0,    0, 1332,
 1454,    0, 1165,    0, 1217,    0,   97,    0,    0,    0,
    0, 2492,    0,    0,    0, -436, 2275, 2019,-1001,    0,
    0,    0, -693, -393,  241,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2107, 2039,
 1865,    0, -580,    0,    0,    0, -932,    0,    0,    0,
 1755,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -508, 1823,    0,    0,
};
#define YYTABLESIZE 13375
static short yytable[] = {                                     129,
  151,   78,  129,  195,   37,  218,  150,  214,  156,  498,
  265,  284,  287,  624,  314,  762,  586,  285,  288,  556,
  962,  625,  255,  592,  198,  210,  249,  748,  227,  201,
  215,  936, 1118,  258,  235,  223, 1119,  253,  802,  558,
  726,  228,  768,  192,  194,  197,  200,  150,  205,  230,
  701,  227, 1269,  216,  194,  221,  259,  200,  254,  150,
  205,  790,  234,  129,  793,  260, 1012,  151,  637, 1029,
  804,  245,  257,  150,  247,  194,  256,  245,  221,  197,
  200,  471,  150,  205,  234,  129,  654,  200, 1258,  252,
  471, 1196, 1447,  319,  638, 1258,  723, 1270,  271, 1259,
  273,  548, 1075, 1260,  272,  492, 1259, 1261, 1278, 1262,
 1260, 1263,  876,  719, 1261,  723, 1262,  637, 1263,  515,
  315,  216,  150,  216,  944,  317,  282, 1017, 1299, 1218,
  985,  368,  959,  867,  746,  221,  650,   77,  379,  719,
 1226,  313,  200,  150,  234,  283,  295,  774,  156,  283,
 1080,  721,  723,  982,  719, 1100,  341, 1047,  637,  542,
  286, 1553,  542, 1311,  589,  719, 1054,  652,  714,  719,
 1123,  269,  719,  684,  717,  516,  637,  650,  643, 1294,
  719,  368,  714,  283,  331,  482, 1207,  333,  774,  321,
  368,  774, 1527,  723,  777,  650,   85,  761,  719,   85,
  775,  780,  781,  782,  783,  647,  774, 1530,  652,  714,
  589,  723, 1401,  624,  684,  717,  757,  684,  717,  643,
 1095,  625,  643,  589,  410,  521,  652,  714,  719,  464,
   77, 1487,  684,  717,  257, 1108, 1626,  643,  467, 1000,
  854,  765, 1074,  331,  501,  269,  647,  467, 1317,  647,
 1167, 1168,  216,  810,  592,  637,  150,  718,  473,  129,
 1316,  648,  323,  884,  647,  150,  372,  473, 1279,  589,
 1412,  721,  508,  774,  541,  257,  504,   77,  257,  514,
  910, 1193,  721,  410,  796,  135,  197,  136, 1691, 1692,
  150,  373, 1124,  257, 1445,  283,  637,  733,  718, 1462,
 1773,  837,  648,  848,  557, 1705, 1482,  641, 1633, 1421,
  654,  560,  721,  215,  637,  573,  718,  131,  575, 1680,
  648,  636,  125,  587,  342,  589, 1061,  216,  646,  798,
  721,  765, 1457, 1483,  216,  540,  216,  634,  216,  642,
  516,  578, 1258,  196,  493,  149,  649, 1405, 1238,  636,
  644,  654,   89, 1259, 1820, 1850,  373, 1260,  519,  200,
  205, 1261,  636, 1262,   60, 1263,  653,  723,  150,  654,
  197, 1081, 1280, 1210, 1799, 1241,  396,  505, 1413,  749,
  636,  125, 1681, 1254,  132, 1221, 1223,  649,  374, 1798,
  765, 1121,  542, 1782,  540,  734, 1851,  914,  301, 1239,
 1855,  343,  196,  493,  911,  649, 1472,  653,  723,   97,
  481,  278,  246,  279,  943,  373, 1125, 1750,  704,   14,
  208,  209, 1277, 1488, 1713,  653,  723,  542,  836,  774,
  958,  719,  506,   77,  542,  396,  720,  717,  721,  685,
 1706,  721,  723, 1852,  514, 1634, 1479,  277,  725,  507,
 1531,  720,  731,  719,  650,  150,  830,  467,  200,  473,
  498, 1334, 1335, 1101, 1336, 1337,  150, 1445, 1042,  481,
  150, 1044,  714,  200,  714,  150,  650, 1772,  156, 1548,
  685, 1402, 1014,  685, 1596, 1050, 1597, 1859,  410, 1434,
  756,  684,  717, 1120,  916, 1630,  643,  721,  685,  714,
  719,  684,  778, 1547,  684,  717,  721,  652,   83,  719,
 1609,  719, 1068,  684,  717, 1122,  712,  831,  643,  736,
  719, 1624,  721,  647, 1616,  722,  719, 1660,  714,  542,
  774, 1765, 1779,  714,  806,  719,  471,  714,  333,  723,
 1038, 1428, 1622, 1217,  281,  647,  227,  281,  637,  989,
   76,  832, 1343,  719, 1041,  714,  799,  216,  990, 1460,
  684,  717,  799,  281,  714, 1534,   85,  719, 1013,  684,
  717,  876, 1110,  637,  745,  718,  723,  683,  281,  648,
 1093,  721, 1115, 1310,  542,  368, 1671,  617, 1813, 1107,
  650, 1635, 1673, 1433,  331,  637, 1206,  718, 1475,  721,
  589,  648,  721,  861,  862,  385, 1219,  796,  493,  719,
  215, 1898,  774,  684,  717, 1449,  652, 1005,  683,  589,
   89,  652, 1408,  719,  516,  684, 1795,  684,  717,  796,
 1849,  721,  643,  216,  719, 1623,  683,  268,  637,  636,
  396, 1639,  878,   76,  718,  719,  617,  853,  774,  877,
  654, 1036, 1037,  718, 1716, 1848, 1038, 1191,  721,  647,
  719,  636,  714,  652,  649, 1057,  986,  721,  481, 1474,
  125, 1720,  221,  723,  798, 1806, 1382,  858, 1799, 1384,
  643,  481,  963,  918,  373,  723,  649,  373, 1249,  714,
   76,  196,  612, 1136,  684,  717,  952,  718,  765,  643,
 1439,  135,  540,  720,  216,  216,  653,  723,  473,  637,
  910,  718,  289,  473, 1040,  648,  481, 1799,  481, 1137,
 1281, 1762,  473, 1545,  910,  200,  647,  508,  514, 1011,
  200,  910, 1268,  215, 1002,  257,  618, 1025, 1025, 1030,
 1789,  493,  324,  227, 1276,  150, 1476,  150, 1739,  531,
  216,  612, 1059, 1312,  723,  915,  216,  685,  532, 1538,
  135,  560,  851, 1771,  654, 1298, 1494,  685,  136,  133,
  685, 1213, 1778,  396,  122,  636,  637, 1200,  718,  685,
 1203,  227,  712, 1258,  216, 1090, 1091, 1092, 1067, 1060,
 1062, 1448,  721,  158, 1259,  618,  373, 1073, 1260,  910,
  649, 1078, 1261,  352, 1262,  714, 1263,  723,  373, 1110,
  373,  656,  216,  216,  227,  227,  227, 1818, 1389,  652,
  653,  723,  857, 1461,  159,   14,  685,  136,  481, 1471,
  151, 1094,  710,  374,  911,  685, 1394,  711, 1396,  394,
 1515, 1105,  636,  516, 1245, 1811,   76,  910,  911,  150,
  721, 1411,  656,  721, 1546,  911, 1541,  395,  269,  150,
  483, 1282,  504,  857, 1023, 1023,  857, 1023, 1023, 1023,
  656, 1117, 1369,  401, 1521,  402,  910,  374, 1524,  685,
  353,  857, 1023, 1023, 1023,  398,  910, 1214,  723,  151,
 1874,  685,  374,  685,  137,  683,  206,  885,  394,  152,
  293, 1625,  892, 1191,  527,  683, 1191, 1204,  683, 1211,
  343, 1271, 1272, 1023,  714, 1031,  395,  683,  533,  483,
  343, 1222,  160,  911, 1008,  693,  390, 1322,  714,  617,
 1205,  153,  401, 1201,  402, 1805,  498, 1870, 1897,  257,
 1836, 1009, 1875,  397,  398, 1202,  154,  514,  850,  122,
  122, 1005,  242,  137,  473,  200,  205,  473,  234,  129,
  685, 1323,  721,  400,  683,  473,  693,  399, 1456,  693,
  824,  911, 1185,  683,  777,  161,  721,  473,  473,  135,
  257,  135,  200,  257,  693,  390, 1812, 1051, 1032,  464,
  751,  720,  617,  242,  591,  617, 1225,  587,  257, 1478,
  911,  961,  397,  291, 1735,  269,  369,  825, 1780, 1781,
  911,  617,  216,  343, 1658,  624,  898,  683,  506,  898,
 1305,  222,  400,  625, 1747, 1659,  399, 1755, 1661,  683,
   76,  683,  720,  145,  612,  498, 1394,  617, 1325, 1700,
  240, 1731,  370, 1732, 1329,  343,  382, 1197,  136,  227,
  720, 1717, 1674,  591, 1822,  343,  587, 1718,  628, 1387,
  596,  752, 1289,  621, 1666,  215,  196,  227,  898,  612,
  706, 1290,  464,  227,  242,  707, 1756, 1289,  618,  135,
  939,  940,  261, 1544,  857,  990, 1679, 1757,  216, 1667,
 1399, 1399, 1403,  721, 1023,  216, 1199,  612,  683,  807,
  612, 1191, 1023,  394, 1809, 1191,  962, 1023, 1777,  151,
 1023,  826,  262,  618,  621,  242,  612,  628,  483,  596,
 1430,  395, 1432,  885,  441,  196, 1726,  885,  810, 1680,
  151,  483,  237, 1877,  721, 1188, 1189,  401,  146,  402,
  857,  618,  612,  857,  618,  147, 1023, 1668,  343,  398,
  857,  656,  721,  857,  714, 1023,  334,  857,  990,  151,
  618,  374,  857, 1023,  374,  857,  483, 1631,  483,  238,
  710,  708,  382,  709,  137,  711,  712,  242,  151,  593,
  808,  857, 1023, 1477, 1023,  390,  618,  151,  375,  691,
  390, 1758, 1681,  151,  250,  714,  390,  473, 1023,  473,
 1023, 1023, 1023, 1300,  335,  857, 1856,  397,  857,  857,
 1025,  151,   99,  714,  473,  846, 1890,  257,  857,  257,
  200,  809,  716,  686,  941,  715,  257,  400,  266,  691,
  691,  399,  375,  691,  267,  857,  394,  686,  593,  281,
  713, 1191, 1601,  693,  721,  686,  693,  375,  691,  101,
  327, 1023, 1301,  693,  395,  960,  693,  857, 1825, 1826,
  693,  686,  857,  328, 1602,  693, 1004,  152,  693, 1844,
  401, 1603,  402,  374,  857, 1533,  857, 1824,  483, 1190,
 1145,  713,  398,  847,  693,  374,  289,  374, 1459,  431,
 1486,  792,  710,  708, 1490,  709, 1146,  711,  329,  713,
  330,  516,  270,  721, 1868, 1604, 1719,  544,  693,  720,
  333,  490,  693,  289,  475,  686,  336,  257,  102,  720,
  431,  693,  720,  390,  196,  613,  152,  595,  103,  290,
  650,  720,  792, 1776,  337,  792, 1889,  196,  693,  257,
  397, 1147,  582,  857,  251,  196,  257,  151,  579, 1896,
  792,  524, 1899,  104,  528, 1891,  912,  545,  475,  580,
  400, 1903,  115, 1300,  399,  693,  196,  417,  338,  333,
 1148,  650,  196,  475,  196, 1857, 1858,  693,  720,  693,
  257, 1549, 1231, 1776,  613,  251,  595,  720,  251,  650,
  728,  546, 1025,  418, 1663,  196, 1232,  473,  414,  115,
  628,  582,  313,  251,  215,  514, 1399,  200,  706,  547,
  331,  721, 1304,  707,  196, 1672, 1283,  729, 1607,  105,
 1233,  721,  196,  196,  721,  274,  417, 1554, 1595,  196,
 1776,  720,  160,  721,  115,  628,  150, 1149,  516,  992,
  257, 1900,   43,   51, 1664,  720,  693,  196,  106,  107,
  692,  108,  418, 1284,  697,  698,  700,  414,  109,  110,
  111,  112,  113,  628,  114, 1776,  628,  115,  116,  117,
  118,  119,  714,  554, 1234,  120,  121,  196,  710,  708,
  721,  709,  628,  711,  196,  714,  555,  779,  517,  721,
 1231,  160,  161,  275,  714,  196, 1678,  899,  383,  276,
  899,   43,   51,   41, 1232,  280,  388,  691,  628,   24,
  691,  770,  720, 1714,  779,   47,  375,  691,  737,  375,
  691,  313,  739,  795,  691,   98,  794,   49, 1233,  691,
  706,  712,  691,  721,  289,  707,  196,  517,  495,  706,
  946,  714,  514,  712,  707,  194,  152,  721,  691,  899,
  714,  161, 1876, 1690, 1117, 1117, 1715,  297,  713, 1754,
  296,  493,   41, 1832,  770, 1595,   99,  152,   24,  499,
  770,  713,  691,   52,   47, 1712,  691,  920,  100,  127,
  713,  141, 1234,  196,   53,  691,   49, 1231,  952,  289,
 1869,   54,  298,  150,  714, 1728,  152,  495,  197, 1729,
 1833, 1232,  691,  101,  385,  217, 1799,   25,  714,  792,
  312,  151,  792, 1834,  721,  152,   55,  150, 1766,  792,
  299, 1867,  792,  300,  152, 1233,  792,  713,  375,  691,
  152,  792,  922,   48,  792,  322,  713,   50,  127,  301,
  375,  691,  375,  691,  325,  289,  217, 1745,  152,  217,
  792,  344, 1878, 1879,  710,  708, 1754,  709,  355,  711,
 1754,   45, 1754,  385,  217,  302,   25,  613,  516,  200,
  346, 1754,  102,  347,  792,  714,  376,  792,  792, 1234,
  713,   56,  103, 1117,  366,  128, 1012,  792,  924,   80,
   81,  891,   48,  116,  713, 1904,   50, 1801, 1012,   46,
 1754, 1012,  613,  516,  792,  151,  357,  104,  431, 1763,
  691,  150,  919,   57,  140,  141,  706,   58,  348,  935,
   45,  707, 1784, 1002,  942,  431,  792, 1754,  150,  847,
  613,  792, 1754,  613,  949,  516,  953,  954,  955,  956,
  957,  431,  949,  792,  128,  792,  194, 1803,  516,  613,
  981,  516,  116,  980, 1519, 1804,  196,  358,   46,  151,
  516,  713,  380,  217,  734,  150, 1017,  378, 1784,  390,
  847,  431,  514,  847,  150,  613, 1099,  196, 1017, 1098,
  390, 1017, 1862,  381,  152, 1003,  710,  708,  847,  709,
  216,  711,  385,  431,  359, 1595,  360, 1000,  524, 1021,
  990,  734,  253, 1794,  376,  151,  196,  514,  384,  816,
 1423,  150,  792,  468,  734, 1784,   14, 1002,  376,  361,
 1424,  150,  217,  477,  197,  196,  376,  208,  209,  913,
 1595,  917,  480,  196,  196,  921,  923,  925, 1106,  514,
  196,  872,  376,  253,  362, 1198,  253,  817, 1197,  818,
  816,  197,  514,  816, 1242,  514,  476, 1098,  196,  105,
 1248,  253, 1595, 1247,  514,  738,  710,  708,  816,  709,
  196,  711,  196, 1292,  481, 1595, 1291, 1117, 1595,  485,
  710,  708,  197,  709,  197,  711,  483, 1595,  106,  107,
  486,  108,  706,  544,  487,  196,  376,  707,  109,  110,
  111,  112,  113, 1297,  114,  500, 1296,  115,  116,  117,
  118,  119,  123,  123,  590,  120,  121,  503, 1406, 1496,
  196,  712, 1197,  512, 1126, 1127, 1131,  502,  774, 1139,
 1141, 1143, 1184, 1151, 1154, 1157, 1159, 1162, 1165,  522,
  254, 1169, 1170,  545, 1172, 1526, 1174,   14,  712, 1177,
 1178, 1179, 1180, 1181, 1182, 1532, 1828,  529,  712, 1827,
  710,  708,  531,  709, 1895,  711, 1186, 1799,  431,  774,
  742,  532,  774,  742, 1010, 1069, 1070,  546,  404,  405,
  406,  254,  540, 1393,  254, 1395,  519,  774, 1901,  543,
 1902,  847,  551,  564,  196,  547,  945,  710,  708,  254,
  709,  571,  711,  574,  584,  639,  414,  415,  588,  643,
  645,  647,  431,  678,  986, 1010, 1010, 1010, 1010, 1010,
 1010, 1010,  679,  680,  706,  681,  688,  519,  682,  707,
  519,  683,  684, 1010, 1010, 1010, 1010,  685,  686,  689,
  687,  710,  708,  690,  709,  519,  711,  847,  695,  696,
  847,  705,  713,  714,  712,  757,  718,  847,  732,  723,
  847,  735,  740,  742,  847, 1010,  755, 1010,  769,  847,
  743,  816,  847,  759,  760,  764,  767,  766,  710,  708,
  253,  709,  253,  711, 1793,  771,  772, 1752,  847,  253,
  784,  776,  785,  531,  787,  779,  757,  797,  788,  757,
  821,  822,  230,  828,  706,  710,  708, 1194,  709,  707,
  711,  823,  847,  281,  757,  847,  847,  833,  706,  834,
  838,  839,  840,  707,  841,  847,  843,  816,  844,  855,
  816,  591,  849, 1864,  856,  231,  859,  816,  860,  869,
  816,  871,  847,  230,  816,  872,  230,  880,  881,  816,
  882,  428,  816,  991,  710,  708,  886,  709,  740,  711,
  947,  230,  949,  937,  847,  976,  978,  975,  816,  847,
  983,  987,  988,  999, 1350, 1351,  231, 1353, 1355,  231,
  253,  847, 1006,  847, 1752, 1360, 1015, 1022, 1752, 1018,
 1842, 1024,  816, 1043,  231,  816,  816,  850,  706, 1752,
 1048, 1049,  253,  707, 1373,  816,  774, 1038,  774,  253,
 1051,  800, 1380, 1381,  431,  774, 1055,  431,  254, 1186,
  254, 1064,  816, 1388,  431, 1390, 1391,  254, 1752, 1347,
  542, 1056, 1058, 1065, 1066,  706, 1010, 1076, 1010, 1010,
  707, 1072, 1082,  253,  816, 1087, 1010,  431, 1089,  816,
  847, 1096, 1113, 1101, 1010, 1752,  728,  431, 1114, 1010,
 1752,  816, 1010,  816,  519, 1133,  519, 1173, 1171, 1183,
  519,  542, 1175,  519,  542, 1195, 1010, 1209, 1212,  706,
 1215, 1010, 1010, 1207,  707, 1010,  519, 1224, 1228,  542,
 1230,  542, 1010,  706, 1010, 1010, 1236, 1441, 1010, 1250,
  696,  237, 1010,  253, 1253, 1010,  774, 1010, 1273, 1010,
 1503,  774, 1275, 1285, 1010, 1010,  706, 1010,  254,  757,
 1264,  707, 1293, 1295, 1302, 1306, 1307, 1308,  774, 1309,
  816, 1320, 1107, 1010, 1010,  774, 1010, 1318,  238, 1327,
  254,  696, 1330,  706,  696, 1332,  431,  254,  707, 1331,
 1010, 1333, 1010, 1010, 1010, 1753, 1339, 1010, 1340,  696,
 1010, 1010,  757,  757,  519, 1345, 1346, 1491, 1344,  774,
 1010, 1348, 1349, 1356, 1357,  757, 1499,  990, 1358, 1359,
  242,  254,  230, 1504, 1010, 1361,  519, 1010, 1362,  542,
 1363, 1364,  706,  519, 1176,  710,  708,  707,  709, 1365,
  711, 1366,  910, 1010, 1386, 1383, 1392, 1407, 1409, 1010,
 1010, 1410,  431,  242, 1010,  231,  431, 1414,  990,  990,
  990,  990,  990, 1003,  990, 1010, 1010,  519, 1010,  774,
  542, 1415, 1416,  542, 1010, 1417,  990,  990,  990,  990,
 1418,  254, 1419,  715, 1429, 1426, 1438, 1535,  542, 1431,
  566, 1436, 1753, 1450, 1446,  262, 1753,  817, 1843, 1451,
 1452, 1453, 1454, 1010, 1010,  757, 1010, 1753,  990, 1455,
  990, 1458, 1473, 1010, 1010, 1010, 1010, 1010, 1463, 1010,
  242, 1480, 1010, 1010, 1010, 1010, 1010, 1484,  542, 1485,
 1010, 1010, 1489, 1495, 1341, 1010, 1753, 1516,  817, 1522,
 1528,  817,  242, 1523, 1539, 1529,  757,  757,  542,  242,
  542, 1608, 1598,  242,  542,  757,  817,  542, 1611, 1613,
 1610, 1614, 1612, 1753, 1615, 1617, 1618, 1627, 1753, 1619,
  542, 1644, 1645, 1629, 1637,  242,  542, 1638, 1632, 1636,
 1640, 1641,  242,  242,  757,  757, 1642,  757, 1656, 1643,
 1646, 1647,  431,  105,  757,  757,  757,  757,  757, 1648,
  757, 1649, 1650,  757,  757,  757,  757,  757, 1651, 1652,
 1653,  757,  757, 1654, 1655, 1839,  242, 1657, 1665, 1841,
 1676, 1675,  106,  107, 1683,  108, 1684,  132, 1698, 1685,
 1847, 1696,  109,  110,  111,  112,  113, 1686,  114, 1687,
 1694,  115,  116,  117,  118,  119, 1695, 1697,  542,  120,
  121, 1367,  710,  708, 1703,  709, 1704,  711,  696, 1865,
 1707,  696, 1708, 1710, 1733, 1711, 1737, 1738,  696, 1742,
  542,  696, 1749,  706, 1764,  696, 1699,  542,  707,  990,
  696,  990,  990,  696, 1767, 1775, 1884, 1790,  542,  990,
 1796, 1886, 1368,  710,  708, 1797,  709,  990,  711,  696,
 1727, 1800,  990, 1808, 1819,  990,  542,  542, 1730,  542,
 1821,  542, 1814,  542, 1816, 1823,  542, 1830, 1837,  990,
 1840,  990, 1831,  696,  990,  990, 1014,  696,  990,  542,
  160, 1811, 1812, 1861, 1535,  990,  696,  990,  990,  800,
  542,  990, 1863, 1866, 1799,  990, 1872, 1744,  990, 1882,
  990, 1746,  990,  696, 1885, 1873, 1887,  990,  990,  817,
  990,  710,  708, 1888,  709, 1892,  711, 1014, 1014, 1014,
 1014, 1014, 1014, 1014, 1893, 1894,  990,  990, 1905,  990,
  696,    1,  643,   29,  203, 1014, 1014, 1014, 1014,   31,
  774,  382,  696,  990,  696,  990,  990,  990,  271,  148,
  990,  675,  992,  990,  990,  540,  841,  542,  269,  472,
  973,  757,  213,  990,  431,  817,   81, 1014,  817, 1014,
  540,  410,  193,  276,  972,  817,  712,  990,  817,  542,
  990,  540,  817,  710,  194,  275,  542,  817,  706,  954,
  817,  708,  383,  729,  223,  953,  990,  841,  728,  328,
  841,  292,  990,  990,  730,  336,  817,  990,  363,  348,
  650,  696,  739,   27,  334,  841,   30,   39,  990,  990,
  542,  990, 1397,  137,  475,  484, 1846,  990,  474,  870,
  817,  977, 1328,  817,  817, 1326, 1324, 1112, 1314,  251,
  706, 1620,  598,  817, 1621,  707, 1371,  710,  708,  835,
  709, 1303,  711, 1252,  553, 1016,  990,  990,  520,  990,
  817,  536,  568, 1315, 1740,  105,  990,  990,  990,  990,
  990,  569,  990, 1709, 1600,  990,  990,  990,  990,  990,
 1053,  706,  817,  990,  990, 1810,  707,  817,  990, 1606,
  816, 1817, 1815, 1854,  106,  107, 1792,  108, 1046,  817,
 1440,  817, 1807, 1689,  109,  110,  111,  112,  113, 1761,
  114, 1422, 1243,  115,  116,  117,  118,  119, 1845, 1774,
 1542,  120,  121, 1372,  710,  708,  576,  709, 1140,  711,
 1144, 1338, 1152, 1155,  559, 1160, 1163, 1166, 1014,  820,
 1014, 1014,  550, 1470, 1088,  857, 1079,  139, 1014,  706,
  513, 1374,  710,  708,  707,  709, 1014,  711,  750,  754,
 1007, 1014,  710,  708, 1014,  709,  294,  711,  817,  926,
  927,  928,  929,  930,  931,  932,  933, 1227, 1014, 1020,
 1019, 1001, 1669, 1014, 1014, 1860, 1736, 1014,  710,  708,
 1829,  709, 1220,  711, 1014,  984, 1014, 1014,  763, 1435,
 1014, 1352, 1385,    0, 1014, 1791,    0, 1014,    0, 1014,
    0, 1014,    0,    0,    0,    0, 1014, 1014,  841, 1014,
    0, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1375,  710,
  708,    0,  709,    0,  711, 1014, 1014,    0, 1014, 1019,
 1019, 1019, 1019,  710,  708,    0,  709,    0,  711,    0,
    0,    0, 1014,    0, 1014, 1014, 1014, 1721,    0, 1014,
  820,    0, 1014, 1014,    0,    0,    0,    0,    0,    0,
    0, 1019, 1014, 1019,  841,    0,    0,  841,    0,    0,
    0,    0,    0,    0,  841,  706, 1014,  841,    0, 1014,
  707,  841,    0,    0,    0,    0,  841,    0,    0,  841,
    0,  820,    0,    0,  820, 1014,    0,    0,    0,    0,
    0, 1014, 1014,    0,    0,  841, 1014,    0,    0,  820,
    0,    0,    0,    0,    0,    0,    0, 1014, 1014,    0,
 1014,    0,    0,    0,    0,    0, 1014,    0,    0,  841,
    0,    0,  841,  841,    0, 1376,  710,  708,    0,  709,
    0,  711,  841, 1377,  710,  708,    0,  709,    0,  711,
    0,    0,  706,    0,    0, 1014, 1014,  707, 1014,  841,
    0,    0,  105,    0,    0, 1014, 1014, 1014, 1014, 1014,
    0, 1014, 1502,    0, 1014, 1014, 1014, 1014, 1014,    0,
  706,  841, 1014, 1014,    0,  707,  841, 1014,    0,    0,
  706,  106,  107,    0,  108,  707,    0,    0,  841,    0,
  841,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,  706,    0,  120,  121,
    0,  707, 1378,  710,  708,    0,  709,    0,  711,    0,
    0,    0, 1019,    0, 1019, 1019,    0,    0,    0,    0,
    0,    0, 1019,    0,    0, 1379,  710,  708,    0,  709,
 1019,  711,    0,    0,    0, 1019,    0,  706, 1019, 1517,
  710,  708,  707,  709,    0,  711,    0,  841,    0,    0,
    0,  706, 1019,    0, 1015,    0,  707, 1019, 1019,    0,
    0, 1019,    0, 1518,  710,  708,    0,  709, 1019,  711,
 1019, 1019,    0,    0, 1019,    0,    0,    0, 1019,    0,
    0, 1019,    0, 1019,    0, 1019,    0,    0,    0,    0,
 1019, 1019,  820, 1019,    0, 1015, 1015, 1015, 1015, 1015,
 1015, 1015, 1525,  710,  708,    0,  709,    0,  711, 1019,
 1019,    0, 1019, 1015, 1015, 1015, 1015,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1019,    0, 1019, 1019,
 1019,    0,  961, 1019,  818,    0, 1019, 1019,    0,    0,
    0,    0,    0,    0,    0, 1015, 1019, 1015,  820,    0,
    0,  820,    0,    0,  706,    0,    0,    0,  820,  707,
 1019,  820,  706, 1019,    0,  820,    0,  707,    0,    0,
  820,    0,    0,  820,    0,  818,    0,    0,  818, 1019,
    0,    0,    0,    0,    0, 1019, 1019,    0,    0,  820,
 1019,    0,    0,  818,    0,    0,    0,    0,    0,    0,
    0, 1019, 1019,    0, 1019, 1492, 1493,    0,    0,    0,
 1019, 1497, 1498,  820, 1500, 1501,  820,  820,    0,    0,
    0,    0, 1505, 1506, 1507, 1508,  820, 1509, 1510, 1511,
 1512, 1513, 1514,    0,    0,    0,    0,    0,    0, 1019,
 1019,  706, 1019,  820,    0,    0,  707,  105,    0, 1019,
 1019, 1019, 1019, 1019,    0, 1019,    0,    0, 1019, 1019,
 1019, 1019, 1019,    0,  706,  820, 1019, 1019,    0,  707,
  820, 1019,    0,    0,    0,    0,  106,  107,  706,  108,
    0,    0,  820,  707,  820,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,  706,  120,  121,    0,    0,  707,    0,    0,
    0,    0,    0,   84,    0,    0, 1015,    0, 1015, 1015,
   85,    0,    0,    0,   86,   87, 1015,    0,    0,    0,
    0,    0,    0,   88, 1015,    0,    0,    0,    0, 1015,
    0,  706, 1015,    0,    0,   89,  707,  542,    0,    0,
    0,  820,    0,    0,    0,    0, 1015,    0,  868,    0,
    0, 1015, 1015,    0,    0, 1015,    0,    0,    0,    0,
    0,    0, 1015,    0, 1015, 1015,    0,    0, 1015,    0,
    0,    0, 1015,    0,    0, 1015,    0, 1015,  542, 1015,
    0,  542,    0,   62, 1015, 1015,  818, 1015,    0,  868,
  868,  868,  868,  868,   63,  868,  542,    0,  542,    0,
   90,   64,    0, 1015, 1015,    0, 1015,  868,  868,  868,
  868,    0,    0,    0,   65,    0,    0,   91,    0,    0,
 1015,    0, 1015, 1015, 1015,    0,  679, 1015,    0,    0,
 1015, 1015,   92,    0,    0,    0,   93,    0,    0,    0,
 1015,  868,  818,    0,    0,  818,    0,   94,    0,    0,
    0,    0,  818,    0, 1015,  818,    0, 1015,    0,  818,
    0,    0,  181,    0,  818,    0,   95,  818,    0,    0,
    0,    0,    0, 1015,    0,   66,    0,    0,    0, 1015,
 1015,    0,    0,  818, 1015,    0,    0,    0,    0,    0,
    0,   67,    0,    0,    0, 1015, 1015,    0, 1015,    0,
    0,    0,    0,  181, 1015,    0,  181,  818,   68,    0,
  818,  818,    0,    0,    0,    0,    0,    0,    0,    0,
  818,  181,    0,   69,    0,    0,    0,   70,    0,    0,
   71,    0,    0, 1015, 1015,    0, 1015,  818,   72,    0,
  105,    0,    0, 1015, 1015, 1015, 1015, 1015,    0, 1015,
    0,    0, 1015, 1015, 1015, 1015, 1015,   73,    0,  818,
 1015, 1015,    0,    0,  818, 1015,    0,    0,    0,  106,
  107,    0,  108,    0,    0,    0,  818,    0,  818,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,  542,  120,  121,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  868,  542,  868,  868,    0,    0,    0,    0,    0,    0,
  868,    0,    0,    0,    0,    0,    0,    0,  868,    0,
    0,    0,    0,  868,    0,    0,  868,  542,    0,    0,
    0,    0,    0,  542,    0,  818,  542,    0,    0,    0,
  868,    0,  880,    0,    0,  868,  868,    0,    0,  868,
    0,    0,    0,    0,    0,    0,  868,    0,  868,  868,
    0,    0,  868,    0,    0,    0,  868,    0,  540,  868,
    0,  868,    0,  868,    0,    0,    0,    0,  868,  868,
    0,  868,    0,  880,  880,  880,  880,  880,    0,  880,
    0,    0,  679,    0,    0,    0,    0,  868,  868,    0,
  868,  880,  880,  880,  880,    0,    0,    0,    0,  540,
  181,    0,  540,    0,  868,    0,  868,  868,  868,    0,
    0,  868,    0,  181,  868,  868,    0,  540,    0,  540,
    0,  181,    0,    0,  868,  880,    0,    0,  542,    0,
    0,    0,    0,    0,    0,  542,    0,    0,  868,    0,
    0,  868,  181,    0,    0,    0,    0,    0,  181,    0,
  181,    0,    0,  542,    0,    0,    0,  868,    0,    0,
  509,    0,    0,  868,  868,    0,    0,    0,  868,    0,
    0,  181,    0,    0,    0,    0,    0,    0,    0,  868,
  868,    0,  868,  542,    0,    0,    0,  542,  868,    0,
  181,    0,    0,    0,    0,    0,    0,    0,    0,  181,
    0,  509,    0,    0,  509,  181,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  868,  868,  509,
  868,    0,  682,  181,    0,    0,    0,  868,  868,  868,
  868,  868,    0,  868,    0,    0,  868,  868,  868,  868,
  868,    0,    0,    0,  868,  868,    0,    0,  883,  868,
    0,  682,  682,    0,  682,    0,    0,    0,    0,    0,
  181,  682,  682,  682,  682,  682,    0,  682,    0,    0,
  682,  682,  682,  682,  682,    0,    0,    0,  682,  682,
 1255,    0,    0,    0,    0,    0,    0,    0,    0,  883,
  883,  883,  883,  883,  880,  883,  880,  880,    0,    0,
    0,    0,    0,    0,  880,    0,    0,  883,  883,  883,
  883,    0,    0,    0,    0,    0,  540,  880,    0,    0,
  880,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  880,  540,  540,    0,  540,  880,
  880,  883,  540,  880,    0,  540,    0,    0,    0,    0,
  880,    0,  880,  880,    0,    0,  880,    0,  540,  181,
  880,    0,    0,  880,  540,  880,    0,  880,    0,    0,
    0,    0,  880,  880,    0,  880,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  880,  880,  223,  880,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  880,    0,
  880,  880,  880,    0,    0,  880,    0,    0,  880,  880,
    0,    0,    0,    0,    0,    0,    0,    0,  880,    0,
    0,    0,    0,    0,  223,    0,  540,  223,  509,    0,
  509,    0,  880,    0,    0,  880,    0,  509,    0,  819,
    0,    0,  223,    0,    0,    0,    0,    0,  540,    0,
  509,  880,    0,    0,    0,  540,    0,  880,  880,    0,
    0,    0,  880,    0,  540,    0,  540,    0,    0,    0,
    0,    0,    0,  880,  880,    0,  880,    0,    0,    0,
  819,    0,  880,  819,  540,    0,    0,    0,    0,  540,
    0,    0,    0,    0,    0,    0,    0,    0,  819,    0,
  883,    0,  883,  883,    0,    0,    0,    0,    0,    0,
  883,  880,  880,    0,  880,    0,    0,    0,  540,    0,
  392,  880,  880,  880,  880,  880,  883,  880,  509,    0,
  880,  880,  880,  880,  880,    0,    0,    0,  880,  880,
  883,    0,  884,  880,    0,  883,  883,    0,  394,  883,
  509,    0,    0,    0,    0,    0,  883,  509,  883,  883,
    0,    0,  883,    0,    0,    0,  883,  542,  251,  883,
    0,  883,    0,  883,    0,    0,    0,  396,  883,  883,
    0,  883,    0,  884,  884,  884,  884,  884,    0,  884,
    0,  509,    0,    0,    0,    0,    0,  883,  883,    0,
  883,  884,  884,  884,  884,    0,    0,    0,  542,  251,
    0,  542,  251,    0,  883,    0,  883,  883,  883,    0,
    0,  883,    0,    0,  883,  883,  542,  251,    0,    0,
    0,    0,    0,    0,  883,  884,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  883,    0,
    0,  883,  401,    0,    0,    0,  177,    0,    0,    0,
    0,  223,    0,  223,    0,    0,    0,  883,  403,    0,
  223,    0,    0,  883,  883,    0,    0,    0,  883,    0,
    0,    0,  404,  405,  406,    0,  208,  209,    0,  883,
  883,    0,  883,    0,    0,    0,    0,    0,  883,    0,
    0,  819,    0,    0,    0,  409,  410,  411,  412,  413,
  414,  415,  416,  417,    0,  177,    0,    0,    0,    0,
  419,  420,    0,    0,    0,    0,    0,  883,  883,  424,
  883,    0,    0,    0,    0,    0,    0,  883,  883,  883,
  883,  883,    0,  883,    0,    0,  883,  883,  883,  883,
  883,    0,  704,    0,  883,  883,    0,  819,  878,  883,
  819,  223,    0,    0,    0,    0,    0,  819,    0,   77,
  819,    0,    0,    0,  819,    0,    0,    0,    0,  819,
    0,    0,  819,  223,    0,    0,    0,    0,    0,    0,
  223,    0,    0,    0,    0,    0,  704,    0,  819,  878,
    0,  878,  878,  878,  884,    0,  884,  884,    0,    0,
    0,  704,    0,    0,  884,    0,    0,  878,  878,  878,
  878,    0,  819,    0,  223,  819,  819,    0,    0,    0,
  884,    0,    0,    0,    0,  819,    0,    0,    0,    0,
    0,    0,    0,    0,  884,  542,  251,  542,  251,  884,
  884,  878,  819,  884,  542,  251,    0,    0,    0,    0,
  884,    0,  884,  884,    0,    0,  884,  542,    0,    0,
  884,    0,    0,  884,  819,  884,    0,  884,    0,  819,
    0,    0,  884,  884,    0,  884,    0,    0,    0,    0,
    0,  819,    0,  819,    0,    0,    0,    0,    0,    0,
    0,  884,  884,    0,  884,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  177,    0,    0,    0,  884,    0,
  884,  884,  884,    0,    0,  884,    0,  177,  884,  884,
    0,    0,    0,    0,    0,  177,    0,    0,  884,    0,
    0,    0,    0,    0,    0,  542,  251,    0,    0,    0,
    0,    0,  884,    0,    0,  884,  177,    0,    0,  849,
  819,    0,  177,    0,  177,    0,    0,  542,  251,    0,
    0,  884,    0,    0,  542,  251,    0,  884,  884,    0,
    0,    0,  884,    0,    0,  177,    0,    0,    0,    0,
    0,    0,    0,  884,  884,    0,  884,    0,    0,    0,
  849,    0,  884,  849,  177,    0,    0,    0,  542,  251,
    0,    0,    0,  177,    0,    0,    0,    0,  849,  177,
  878,    0,  878,  878,    0,    0,    0,    0,    0,    0,
  878,  884,  884,    0,  884,    0,    0,  177,    0,    0,
    0,  884,  884,  884,  884,  884,  878,  884,    0,    0,
  884,  884,  884,  884,  884,    0,    0,    0,  884,  884,
  878,    0,  877,  884,    0,  878,  878,    0,    0,  878,
    0,    0,    0,  704,  177,    0,  878,    0,  878,  878,
  704,    0,  878,  704,    0,    0,  878,  704,    0,  878,
    0,  878,    0,  878,    0,  704,    0,    0,  878,  878,
    0,  878,    0,  877,    0,  877,  877,  877,    0,    0,
    0,  704,    0,    0,    0,    0,    0,  878,  878,    0,
  878,  877,  877,  877,  877,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  878,  704,  878,  878,  878,    0,
    0,  878,    0,    0,  878,  878,    0,    0,    0,    0,
    0,    0,    0,    0,  878,  877,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  704,    0,    0,  878,    0,
  704,  878,    0,  177,    0,    0,    0,    0,    0,    0,
    0,    0,   76,    0,  105,    0,    0,  878,    0,    0,
    0,    0,  704,  878,  878,    0,    0,    0,  878,    0,
    0,    0,    0,    0,  704,    0,  704,    0,    0,  878,
  878,    0,  878,  106,  107,    0,  108,    0,  878,    0,
    0,  849,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0,    0,  878,  878,    0,
  878,    0,    0,    0,    0,    0,    0,  878,  878,  878,
  878,  878,    0,  878,    0,    0,  878,  878,  878,  878,
  878,    0,  378,    0,  878,  878,    0,  849,  879,  878,
  849,    0,    0,    0,    0,  221,    0,  849,    0,  221,
  849,    0,    0,    0,  849,    0,    0,    0,    0,  849,
    0,    0,  849,  221,    0,    0,    0,  221,    0,    0,
  221,    0,    0,  378,    0,    0,  378,    0,  849,  879,
    0,  879,  879,  879,  877,    0,  877,  877,    0,    0,
    0,  378,    0,    0,  877,    0,    0,  879,  879,  879,
  879,    0,  849,    0,    0,  849,  849,    0,    0,    0,
  877,  221,    0,    0,    0,  849,    0,  221,  221,  221,
    0,  221,  221,  221,  877,  221,    0,    0,    0,  877,
  877,  879,  849,  877,    0,    0,    0,    0,    0,    0,
  877,    0,  877,  877,    0,  221,  877,  221,  813,    0,
  877,    0,    0,  877,  849,  877,    0,  877,    0,  849,
    0,    0,  877,  877,    0,  877,    0,    0,    0,    0,
    0,  849,    0,  849,    0,  537,    0,    0,  221,    0,
    0,  877,  877,   80,  877,    0,    0,    0,    0,  813,
    0,    0,  813,    0,    0,  221,    0,    0,  877,    0,
  877,  877,  877,    0,    0,  877,    0,  813,  877,  877,
    0,    0,    0,    0,    0,    0,  537,  221,  877,  537,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  877,    0,  537,  877,  537,    0,    0,    0,
  849,    0,    0,    0,    0,    0,  814,    0,    0,    0,
    0,  877,  221,  221,    0,    0,    0,  877,  877,    0,
    0,    0,  877,    0,    0,    0,    0,    0,    0,  221,
    0,    0,    0,  877,  877,    0,  877,    0,    0,    0,
    0,    0,  877,    0,    0,    0,    0,  814,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  879,    0,  879,  879,    0,  814,    0,    0,    0,    0,
  879,  877,  877,    0,  877,    0,    0,    0,    0,    0,
    0,  877,  877,  877,  877,  877,  879,  877,    0,    0,
  877,  877,  877,  877,  877,    0,    0,    0,  877,  877,
  879,    0,  882,  877,    0,  879,  879,    0,    0,  879,
    0,    0,    0,  378,    0,    0,  879,    0,  879,  879,
  378,    0,  879,  378,    0,    0,  879,  378,    0,  879,
    0,  879,    0,  879,    0,  378,    0,    0,  879,  879,
    0,  879,    0,  882,    0,  882,  882,  882,    0,    0,
    0,  378,    0,    0,    0,    0,    0,  879,  879,    0,
  879,  882,  882,  882,  882,    0,    0,    0,    0,    0,
  813,    0,    0,    0,  879,  378,  879,  879,  879,    0,
    0,  879,    0,    0,  879,  879,    0,    0,    0,    0,
    0,    0,    0,  537,  879,  882,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  378,    0,    0,  879,    0,
    0,  879,  537,  537,    0,  537,  187,    0,    0,  537,
    0,    0,  537,    0,    0,    0,  813,  879,    0,  813,
    0,    0,  378,  879,  879,  537,  813,    0,  879,  813,
    0,  537,    0,  813,  378,    0,  378,    0,  813,  879,
  879,  813,  879,    0,    0,    0,    0,    0,  879,    0,
    0,    0,    0,    0,    0,    0,    0,  813,  814,    0,
    0,    0,    0,    0,    0,  187,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  879,  879,    0,
  879,  813,    0,    0,  813,  813,    0,  879,  879,  879,
  879,  879,    0,  879,  813,    0,  879,  879,  879,  879,
  879,    0,    0,  537,  879,  879,    0,    0,    0,  879,
    0,  813,    0,    0,  814,    0,    0,  814,    0,    0,
    0,    0,    0,    0,  814,  537,  858,  814,    0,    0,
    0,  814,  537,  813,    0,    0,  814,    0,  813,  814,
    0,    0,    0,  537,    0,    0,    0,    0,    0,    0,
  813,    0,  813,    0,  882,  814,  882,  882,    0,    0,
    0,  537,    0,    0,  882,    0,  537,  858, 1024, 1024,
  858, 1024, 1024, 1024,    0,    0,    0,    0,    0,  814,
  882,    0,  814,  814,    0,  858, 1024, 1024, 1024,    0,
    0,    0,  814,    0,  882,  537,    0,    0,    0,  882,
  882,    0,    0,  882,    0,    0,    0,    0,    0,  814,
  882,    0,  882,  882,    0,    0,  882, 1024,    0,  813,
  882,    0,    0,  882,  188,  882,    0,  882,    0,    0,
    0,  814,  882,  882,    0,  882,  814,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  814,    0,
  814,  882,  882,    0,  882,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  187,    0,    0,    0,  882,    0,
  882,  882,  882,    0,    0,  882,    0,  187,  882,  882,
    0,    0,    0,  188,    0,  187,    0,    0,  882,    0,
  815,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  882,    0,    0,  882,  187,    0,    0,    0,
    0,    0,  187,    0,  187,    0,    0,  814,    0,    0,
    0,  882,    0,    0,    0,    0,    0,  882,  882,    0,
    0,  815,  882,    0,  815,  187,    0,    0,    0,    0,
    0,    0,    0,  882,  882,    0,  882,    0,    0,  815,
    0,    0,  882,    0,  187,    0,    0,    0,    0,    0,
    0,    0,    0,  187,    0,    0,    0,    0,    0,  187,
    0,    0,    0,    0,  859,    0,    0,    0,    0,    0,
    0,  882,  882,    0,  882,    0,    0,  187,  858,    0,
    0,  882,  882,  882,  882,  882,    0,  882, 1024,    0,
  882,  882,  882,  882,  882,    0, 1024,    0,  882,  882,
    0, 1024,    0,  882, 1024,  859, 1025, 1025,  859, 1025,
 1025, 1025,    0,    0,  187,    0,    0,    0,    0,    0,
    0,    0,    0,  859, 1025, 1025, 1025,  701,    0,    0,
    0,    0,    0,    0,  858,    0,    0,  858,    0,    0,
 1024,    0,    0,    0,  858,    0,    0,  858,    0, 1024,
    0,  858,  428,    0,    0, 1025,  858, 1024,    0,  858,
  682,    0,    0,    0,    0,    0,    0,    0,  701,    0,
    0,  701,  188,    0,    0,  858, 1024,    0, 1024,    0,
    0,    0,    0,    0,    0,  188,  701,    0,    0,    0,
    0,    0, 1024,  188, 1024, 1024, 1024,    0,    0,  858,
  682,  682,  858,  858,  682,    0,    0,    0,    0,    0,
    0,    0,  858,  187,  188,    0,    0,    0,    0,  682,
  188,    0,  188,    0,    0,    0,    0,    0,    0,  858,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  815,  188,    0, 1024,    0,    0,    0,    0,
    0,  858,    1,    0,    0,    0,  858,    0,    0,    0,
    0,    0,  188,    0,    0,    0,    0,    0,  858,    0,
  858,  188,    0,    2,    0,    0,    0,  188,    0,    0,
    0,    3,    0,    0,    0,    0,    0,    0,    4,    0,
    5,    0,    6,    0,    0,  188,    0,    7,  815,    0,
    0,  815,    0,    0,    8,    0,    0,    0,  815,    0,
    0,  815,    0,    0,    0,  815,    0,    0,    9,    0,
  815,    0,    0,  815,    0,    0,  859,    0,    0,   10,
    0,    0,  188,    0,    0,    0, 1025,  858,    0,  815,
    0,    0,    0,    0, 1025,    0,    0,    0,    0, 1025,
    0,    0, 1025,    0,    0,    0,    0,  699,    0,    0,
    0,    0,    0,  815,    0,    0,  815,  815,    0,    0,
    0,    0,    0,    0,    0,    0,  815,    0,  848,    0,
    0,    0,  859,    0,    0,  859,    0,    0, 1025,    0,
    0,  701,  859,  815,    0,  859,   11, 1025,  699,  859,
    0,  699,    0,   12,  859, 1025,   13,  859,   14,   15,
    0,    0,    0,    0,    0,  815,  699,    0,    0,  848,
  815,    0,  848,  859, 1025,    0, 1025,    0,    0,    0,
    0,  188,  815,   16,  815,    0,    0,  848,    0,    0,
 1025,    0, 1025, 1025, 1025,  701,    0,  859,  701,    0,
  859,  859,    0,    0,    0,  701,    0,    0,  701,    0,
  859,    0,  701,    0,    0,    0,    0,  701,    0,    0,
  701,    0,    0,    0,    0,    0,    0,  859,  682,    0,
    0,  682,   17,    0,    0,    0,  701,    0,  682,    0,
    0,  682,    0, 1025,  842,  682,    0,   18,   19,  859,
  682,  815,    0,  682,  859,  850,    0,    0,    0,    0,
  701,    0,    0,    0,  701,    0,  859,    0,  859,  682,
    0,    0,    0,  701,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  842,    0,    0,  842,    0,
  701,    0,    0,  682,    0,    0,  850,  682,    0,  850,
    0,    0,    0,  842,  105,    0,  682,    0,    0,    0,
    0,    0,    0,    0,  850,    0,    0,  701,    0,    0,
    0,    0,    0,  682,    0,    0,    0,    0,    0,  701,
    0,  701,    0,  106,  107,  859,  108,  701,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
  682,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,  682,    0,  682,    0,  701,  701,    0,  701,
  682,    0,    0,  851,    0,    0,  701,  701,  701,  701,
  701,    0,  701,    0,    0,  701,  701,  701,  701,  701,
  848,    0,    0,  701,  701,    0,  839,    0,  701,  682,
  682,    0,  682,    0,    0,    0,    0,    0,    0,  682,
  682,  682,  682,  682,  851,  682,    0,  851,  682,  682,
  682,  682,  682,    0,    0,  699,  682,  682,  699,    0,
    0,  682,  851,    0,    0,  699,    0,  839,  699,    0,
  839,    0,  699,    0,    0,    0,  848,  699,    0,  848,
  699,    0,    0,    0,    0,  839,  848,    0,    0,  848,
  843,    0,    0,  848,    0,    0,  699,    0,  848,    0,
    0,  848,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  848,    0,    0,
  699,    0,    0,    0,  699,    0,    0,    0,    0,    0,
    0,  843,    0,  699,  843,    0,  842,    0,    0,    0,
    0,  848,    0,    0,  848,  848,    0,  850,    0,  843,
  699,    0,    0,    0,  848,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  848,    0,    0,    0,    0,    0,  699,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  699,
    0,  699,  842,  848,    0,  842,    0,  682,  848,    0,
    0,    0,  842,  850,    0,  842,  850,    0,    0,  842,
  848,    0,  848,  850,  842,    0,  850,  842,    0,    0,
  850,    0,    0,    0,    0,  850,  682,  682,  850,  682,
    0,    0,  852,  842,    0,    0,  682,  682,  682,  682,
  682,    0,  682,    0,  850,  682,  682,  682,  682,  682,
    0,    0,    0,  682,  682,    0,    0,  842,  699,    0,
  842,  842,    0,    0,  840,  851,    0,    0,  850,    0,
  842,  850,  850,  852,    0,    0,  852,    0,    0,  848,
    0,  850, 1677,    0,    0,    0,    0,  842,  839,    0,
    0,  852,    0,    0,    0,    0,    0,    0,  850,    0,
    0,    0,    0,    0,    0,  840,    0,    0,  840,  842,
    0,    0,    0,    0,  842,    0,    0,    0,    0,    0,
  850,  851,    0,  840,  851,  850,  842,    0,  842,    0,
    0,  851,    0,    0,  851,    0,    0,  850,  851,  850,
    0,    0,    0,  851,  839,    0,  851,  839,    0,    0,
    0,    0,  843,    0,  839,    0,    0,  839,    0,    0,
  844,  839,  851,    0,    0,    0,  839,    0,    0,  839,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  839,  851,    0,    0,  851,
  851,    0,    0,    0,    0,  842,    0,    0,    0,  851,
    0,  844,    0,    0,  844,    0,  850,    0,  843,  839,
    0,  843,  839,  839,    0,    0,  851,    0,  843,  844,
    0,  843,  839,    0,    0,  843,    0,    0,    0,    0,
  843,    0,    0,  843,    0,    0,    0,    0,  851,  839,
    0,  566,    0,  851,    0,    0,    0,    0,    0,  843,
    0,    0,    0,    0,    0,  851,    0,  851,    0,    0,
    0,  839,    0,    0,    0,    0,  839,    0,    0,    0,
    0,    0,    0,  843,    0,    0,  843,  843,  839,    0,
  839,    0,    0,    0,    0,    0,  843,    0,    0,    0,
    0,    0,    0,  662,  852,    0,  425,    0,  426,    0,
    0,    0,    0,  843,    0,    0,    0,    0,    0,    0,
    0,  428,    0,    0,    0,    0,  283,    0,    0,    0,
    0,    0,    0,    0,  851,  843,  840,    0,    0,    0,
  843,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  843,    0,  843,    0,    0,  839,    0,    0,
  852,    0,    0,  852,    0,    0,    0,    0,    0,    0,
  852,    0,    0,  852,    0,    0,    0,  852,    0,    0,
    0,    0,  852,    0,    0,  852,    0,    0,    0, 1688,
    0,    0,  840,    0,    0,  840,    0,    0,    0,    0,
    0,  852,  840,    0,    0,  840,    0,    0,    0,  840,
    0,    0,    0,    0,  840,    0,    0,  840,    0,    0,
    0,  843,    0,    0,  105,  852,    0,    0,  852,  852,
    0,    0,  844,  840,    0,    0,    0,    0,  852,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  106,  107,  852,  108,  840,    0,    0,
  840,  840,    0,  109,  110,  111,  112,  113,    0,  114,
  840,    0,  115,  116,  117,  118,  119,  852,    0,    0,
  120,  121,  852,    0,    0,    0,    0,  840,  844,    0,
    0,  844,    0,  105,  852,    0,  852,    0,  844,    0,
    0,  844,    0,    0,    0,  844,    0,    0,    0,  840,
  844,    0,    0,  844,  840,    0,    0,    0,    0,    0,
    0,    0,  106,  107,    0,  108,  840,    0,  840,  844,
  389,    0,  109,  110,  111,  112,  113,    0,  114,  390,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,  391,  844,    0,    0,  844,  844,  392,  393,
    0,    0,  662,  852,    0,  425,  844,  426,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  656,    0,    0,
  428,    0,    0,  844,    0,  283,  394,    0,    0,  395,
    0,    0,    0,    0,    0,  840,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  844,    0,    0,    0,    0,
  844,    0,    0,    0,    0,    0,    0,  397,  398,    0,
    0,    0,  844,    0,  844,    0,    0,    0,    0,  399,
    0,    0,    0,  400,    0,    0,  873,    0,    0,    0,
  600,  887,  666,    0,  666,  666,    0,  666,    0,    0,
    0,    0,    0,    0,  874,    0,    0,    0,  601,    0,
  666,  602,    0,    0,    0,  666,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  699,    0,    0,    0,  658,    0,    0,    0,    0,    0,
  401,  844,    0,    0,    0,    0,  402,    0,    0,    0,
    0,    0,  603,    0,    0,    0,  403,    0,  604,  605,
  606,    0,  607,  608,  609,    0,  610,    0,    0,    0,
  404,  405,  406,  407,  208,  209,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,  611,    0,  612,    0,
    0,    0,    0,  409,  410,  411,  412,  413,  414,  415,
  416,  417,  106,  418,    0,  108,    0,    0,  419,  420,
    0,  421,  422,  423,  111,  112,  113,  424,  114,  613,
    0,  115,  116,  117,  118,  119,  888,  889,  890,  120,
  121,    0,    0,    0,    0,    0,  614,    0,    0,  389,
  105,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  615,    0,
    0,  391,    0,    0,  655,    0,    0,  392,  393,  106,
  107,    0,  108,    0,    0,    0,    0,    0,    0,  109,
  110,  111,  112,  113,    0,  114,  656,    0,  115,  116,
  117,  118,  119,  616,  617,  394,  120,  121,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  618,    0,  666,    0,    0,    0,    0,    0,    0,  666,
    0,    0,    0,    0,    0,    0,  397,  398,  666,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  666,  400,    0,    0,    0,    0,  666,  666,    0,
  657,  660,    0,  660,  660,  666,  660,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  660,
    0,    0,    0,    0,  660,  666,    0,    0,  666,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  658,    0,    0,    0,    0,    0,  401,
    0,    0,    0,    0,  666,  402,  666,  666,    0,    0,
    0,    0,    0,    0,    0,  403,    0,  572,  666,    0,
    0,    0,  666,    0,    0,    0,    0,    0,    0,  404,
  405,  406,  407,  208,  209,    0,    0,    0,    0,    0,
    0,    0,    0,  408,    0,    0,    0,    0,    0,    0,
    0,    0,  409,  410,  411,  412,  413,  414,  415,  416,
  417,  106,  418,    0,  108,    0,    0,  419,  420,    0,
  421,  422,  423,  111,  112,  113,  424,  114,    0,  666,
  115,  116,  659,  660,  661,  666,    0,    0,  120,  121,
    0,    0,    0,    0,    0,  666,    0,    0,    0,    0,
    0,    0,    0,  662,    0,    0,  425,    0,  426,  666,
  666,  666,  666,  666,  666,    0,    0,    0,    0,    0,
    0,  428,    0,  666,    0,    0,  283,    0,    0,  964,
    0,    0,  666,  666,  666,  666,  666,  666,  666,  666,
  666,  666,  666,    0,  666,    0,    0,  666,  666,    0,
  666,  666,  666,  666,  666,  666,  666,  666,    0,    0,
  666,  666,  666,  666,  666,    0,    0,    0,  666,  666,
    0,  660,    0,    0,    0,    0,    0,    0,  660,    0,
    0,    0,    0,    0,    0,    0,    0,  660,    0,    0,
  105,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  660,    0,    0,    0,    0,    0,  660,  660,    0,    0,
    0,    0,    0,    0,  660,    0,    0,    0,    0,  106,
  107,    0,  108,    0,    0,    0,    0,    0,    0,  109,
  110,  111,  112,  113,  660,  114,    0,  660,  115,  116,
  117,  118,  119,    0,  967,    0,  120,  121,    0,    0,
    0,    0,    0,    0,    0,  662,    0,    0,  425,    0,
  426,  105,    0,  660,    0,  660,  660,    0,    0,    0,
    0,    0,    0,  428,    0,    0,    0,  660,  283,    0,
    0,  660,    0,    0,    0,    0,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,    0,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114, 1666,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  389,    0, 1667,    0,    0,    0,    0,    0,  660,  390,
    0,    0,    0,    0,  660,  945,  710,  708,    0,  709,
    0,  711,  391,    0,  660,    0,    0,    0,  392,  393,
    0,    0,    0,    0,  908,  907,  909,    0,  660,  660,
  660,  660,  660,  660,    0,    0,    0,  656,    0,    0,
    0,    0,  660,    0,    0,    0,  394,    0,    0,  395,
 1668,  660,  660,  660,  660,  660,  660,  660,  660,  660,
  660,  660,    0,  660,    0,    0,  660,  660,    0,  660,
  660,  660,  660,  660,  660,  660,  660,  397,  398,  660,
  660,  660,  660,  660,  105,    0,    0,  660,  660,  399,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,  883,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,  389,    0,  658,    0,    0,    0,    0,    0,
  401,  390,    0,    0,  662,    0,  402,  425,    0,  426,
    0,    0,    0,    0,  391,    0,  403,    0,    0,    0,
  392,  393,  428,    0,    0,    0,    0,  283,    0,    0,
  404,  405,  406,  407,  208,  209,    0,    0,    0,  656,
    0,    0,    0,    0,  408,    0,    0,    0,  394,    0,
    0,  395,    0,  409,  410,  411,  412,  413,  414,  415,
  416,  417,  106,  418,    0,  108,    0,    0,  419,  420,
    0,  421,  422,  423,  111,  112,  113,  424,  114,  397,
  398,  115,  116,  659,  660,  661,  895,    0,    0,  120,
  121,  399,    0,    0,  706,  400,    0,    0,    0,  707,
    0,  489,  896,  657,  425,    0,  426,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
    0,  105,    0,    0,  283,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  897,    0,
    0,    0,    0,    0,    0,    0,  658,  898,    0,    0,
  106,  107,  401,  108,    0,  899,    0,    0,  402,    0,
  109,  110,  111,  112,  113,    0,  114,    0,  403,  115,
  116,  117,  118,  119,  900,    0,  901,  120,  121,    0,
    0,    0,  404,  405,  406,  407,  208,  209,    0,    0,
  902,    0,  903,  904,  905,    0,  408,    0,    0,    0,
    0,    0,    0,    0,    0,  409,  410,  411,  412,  413,
  414,  415,  416,  417,  106,  418,    0,  108,    0,    0,
  419,  420,    0,  421,  422,  423,  111,  112,  113,  424,
  114,  389,    0,  115,  116,  659,  660,  661,    0,  469,
  390,  120,  121,  906,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  391,    0,    0,  105,    0,    0,  392,
  393,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  656,    0,
    0,    0,    0,    0,    0,  106,  107,  394,  108,    0,
  395,    0,    0,    0,    0,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,    0,    0,    0,  397,  398,
    0, 1128,  547,    0, 1129,    0,    0,    0,  389,    0,
  399,    0,    0,    0,  400,    0,    0,  390,    0,    0,
    0,    0,  883,    0,    0,    0,    0,    0,    0,    0,
  391,    0,    0,    0,    0,    0,  392,  393,    0,  489,
    0,    0,  425,  547,  426,    0,  547,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,    0,    0,
    0,  547,  283,  547,  394,  658,    0,  395,    0,    0,
    0,  401,    0,    0,    0,    0,    0,  402,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  403,    0,    0,
    0,    0,    0,  547,    0,  397,  398,    0,    0,    0,
    0,  404,  405,  406,  407,  208,  209,  399,    0,    0,
    0,  400,    0,    0,    0,  408,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  413,  414,
  415,  416,  417,  106,  418,    0,  108,    0,    0,  419,
  420,    0,  421,  422,  423,  111,  112,  113,  424,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0, 1130,    0,  401,    0,
    0,    0,    0,    0,  402,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,    0,    0,
    0,  489,    0,    0,  425,    0,  426,    0,  404,  405,
  406,  407,  208,  209,    0,    0,    0,    0,    0,  428,
    0,  964,  408,    0,  283,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  413,  414,  415,  416,  417,
  106,  418,    0,  108,    0,    0,  419,  420,    0,  421,
  422,  423,  111,  112,  113,  424,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121, 1138,
  547,    0, 1129,    0,    0,    0,  389,    0,    0,    0,
    0,    0,    0,    0,    0,  390,  547,    0,    0,  547,
  547,  465,  547,    0,    0,  965,  547,    0,  391,  547,
    0,    0,    0,    0,  392,  393,    0,    0,    0,    0,
  489,    0,  547,  425,    0,  426,    0,  105,  547,    0,
    0,  547,    0,    0,    0,    0,    0,    0,  428,    0,
    0,  966,  394,  283,    0,  395,  967,    0,    0,    0,
    0,    0,  968,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,  105,    0,    0,  109,  110,  111,  112,
  113,  969,  114,  397,  398,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,  399,    0,    0,    0,  400,
    0,    0,  106,  107,    0,  108,    0,    0,    0,    0,
  547,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,  547,    0,    0,    0,    0,    0,    0,  547,
    0, 1142,    0,  547, 1129,    0,    0,    0,  389,    0,
  547,    0,    0,    0, 1130,    0,  401,  390,    0,    0,
    0,    0,  402,    0,    0,    0,    0,    0,  547,    0,
  391,    0,  403,  547,    0,    0,  392,  393,    0,    0,
    0,    0,    0,    0,    0,    0,  404,  405,  406,  407,
  208,  209,    0,    0,    0,    0,    0,    0,  547,    0,
  408,    0,  547,    0,  394,    0,    0,  395,    0,  409,
  410,  411,  412,  413,  414,  415,  416,  417,  106,  418,
    0,  108,    0,    0,  419,  420,    0,  421,  422,  423,
  111,  112,  113,  424,  114,  397,  398,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,  399,    0,    0,
 1150,  400,    0, 1129,    0,    0,    0,  389,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  391,
    0,    0,    0,    0,    0,  392,  393,    0,    0,  489,
    0,    0,  425,    0,  426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1130,  428,  401,    0,
    0,    0,  283,  394,  402,    0,  395,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  404,  405,
  406,  407,  208,  209,  397,  398,    0,    0,    0,    0,
    0,    0,  408,    0,    0,    0,  399,    0,    0,    0,
  400,  409,  410,  411,  412,  413,  414,  415,  416,  417,
  106,  418,    0,  108,    0,    0,  419,  420,    0,  421,
  422,  423,  111,  112,  113,  424,  114,    0,    0,  115,
  116,  117,  118,  119,    0, 1438,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1130,    0,  401,    0,    0,
    0,    0,    0,  402,    0,    6,    0,    0,    0,    0,
    0,    0,    0,  403,    0,    0, 1555, 1556,    0, 1557,
    0,  489,    0,    0,  425,    0,  426,  404,  405,  406,
  407,  208,  209,    0,    0,    0, 1558,    0,    0,  428,
    0,  408, 1559,    0,  283,    0,    0,    0,    0,    0,
  409,  410,  411,  412,  413,  414,  415,  416,  417,  106,
  418,    0,  108,    0,    0,  419,  420,    0,  421,  422,
  423,  111,  112,  113,  424,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0, 1153,
    0,    0, 1129,    0,    0,    0,  389,    0,    0,    0,
    0,    0, 1560,    0,    0,  390,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  391,    0,
    0,   14,    0,    0,  392,  393,    0,    0,    0,  585,
  489,    0,    0,  425,    0,  426, 1561,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,  428,    0,
    0,    0,  394,  283,    0,  395,    0,    0,  585,  585,
    0,  585,    0,    0,  105,    0,    0,    0,  585,  585,
  585,  585,  585,    0,  585,    0,    0,  585,  585,  585,
  585,  585,    0,  397,  398,  585,  585,    0,    0,    0,
    0,    0,    0, 1562,  107,  399,  108,    0,    0,  400,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115, 1563,  117,  118,  119,    0,    0,    0,
  120,  121, 1564, 1565, 1566,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1158,    0,    0, 1129,    0,    0,    0,  389,    0,
    0,    0,    0,    0, 1130,    0,  401,  390,    0,    0,
    0,    0,  402,    0,    0,    0,    0,    0,    0,    0,
  391,    0,  403,    0,    0,    0,  392,  393,    0,    0,
    0,    0,    0,    0,    0,    0,  404,  405,  406,  407,
  208,  209,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,  394,    0,    0,  395,    0,  409,
  410,  411,  412,  413,  414,  415,  416,  417,  106,  418,
    0,  108,    0,    0,  419,  420,    0,  421,  422,  423,
  111,  112,  113,  424,  114,  397,  398,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,  399,    0,    0,
 1161,  400,    0, 1129,    0,    0,    0,  389,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  391,
    0,    0,    0,    0,    0,  392,  393,    0,    0,  489,
    0,    0,  425,    0,  426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1130,  428,  401,    0,
    0,    0,  283,  394,  402,    0,  395,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,    0,    0,
  710,  708,    0,  709,    0,  711,    0,    0,  404,  405,
  406,  407,  208,  209,  397,  398,    0,    0,  908,  907,
  909,    0,  408,    0,    0,    0,  399,    0,    0,    0,
  400,  409,  410,  411,  412,  413,  414,  415,  416,  417,
  106,  418,    0,  108,    0,    0,  419,  420,    0,  421,
  422,  423,  111,  112,  113,  424,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1130,    0,  401,    0,    0,
    0,    0,    0,  402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  403,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  404,  405,  406,
  407,  208,  209,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
  409,  410,  411,  412,  413,  414,  415,  416,  417,  106,
  418,    0,  108,    0,    0,  419,  420,    0,  421,  422,
  423,  111,  112,  113,  424,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0, 1164,
    0,    0, 1129,    0,    0,    0,  389,    0,    0,  489,
    0,  494,  425,    0,  426,  390,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,  391,    0,
   98,    0,  283,    0,  392,  393,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  895,    0,    0,    0,    0,    0,    0,    0,  706,    0,
    0,    0,  394,  707,    0,  395,  896,    0,    0,    0,
    0,   99,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  100,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  397,  398,    0,    0,  134,    0,    0,
    0,    0,  897,    0,    0,  399,    0,  489,  101,  400,
  425,  898,  426,    0,    0,    0,    0,    0,    0,  899,
    0,    0,    0,    0,    0,  428,    0,    0,    0,    0,
  283,    0,    0,    0,    0,    0,    0,    0,  900,    0,
  901,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  902,    0,  903,  904,  905,    0,
    0,    0,    0,    0, 1130,    0,  401,    0,    0,    0,
    0,    0,  402,    0,    0,    0,    0,  102,    0,    0,
    0,    0,  403,    0,    0,    0,    0,  103,    0,    0,
    0,    0,    0,    0,    0,    0,  404,  405,  406,  407,
  208,  209,    0,    0,    0,    0,    0,  906,    0,    0,
  408,    0,  104,    0,    0,    0,    0,    0,    0,  409,
  410,  411,  412,  413,  414,  415,  416,  417,  106,  418,
  105,  108,    0,    0,  419,  420,  389,  421,  422,  423,
  111,  112,  113,  424,  114,  390,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,  391,  106,
  107,  284,  108,    0,  392,  393,    0,    0,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
  284,  284,  394,  284,    0,  395,    0,    0,    0,    0,
  284,  284,  284,  284,  284,    0,  284,    0,    0,  284,
  284,  284,  284,  284,    0,    0,    0,  284,  284,    0,
    0,  396,    0,  397,  398,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  399,    0,    0,    0,  400,
    0,    0,    0,  390,    0,    0,  984,    0,  984,  984,
    0,  984,    0,    0,    0,    0,  391,    0,    0,    0,
    0,    0,  392,  393,  984,    0,    0,    0,    0,  984,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  394,    0,    0,  395,    0,    0,  401,    0,    0,    0,
    0,    0,  402,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  403,    0,    0,    0,    0,    0,    0,  396,
    0,  397,  398,    0,    0,    0,  404,  405,  406,  407,
  208,  209,    0,  399,    0,    0,    0,  400,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,  409,
  410,  411,  412,  413,  414,  415,  416,  417,  106,  418,
    0,  108,    0,    0,  419,  420,    0,  421,  422,  423,
  111,  112,  113,  424,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,  699,  120,  121,    0,    0,    0,
    0,    0,    0,    0,  401,    0,    0,    0,  666,    0,
  402,  666,    0,  666,    0,    0,    0,    0,    0,    0,
  403,    0,    0,    0,  221,    0,  666,    0,  221,    0,
    0,  666,    0,    0,  404,  405,  406,  407,  208,  209,
    0,    0,  221,    0,    0,    0,  221,    0,  408,  221,
    0,    0,    0,    0,    0,    0,    0,  409,  410,  411,
  412,  413,  414,  415,  416,  417,  106,  418,    0,  108,
    0,    0,  419,  420,    0,  421,  422,  423,  111,  112,
  113,  424,  114,  984,    0,  115,  116,  117,  118,  119,
  221,    0,  984,  120,  121,    0,  221,  221,  221,    0,
  221,  221,  221,    0,  221,  984,    0,    0,    0,    0,
    0,  984,  984,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  221,    0,  221,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  984,
    0,    0,  984,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  984,    0,
  984,  984,    0,    0,  221,    0,    0,    0,    0,    0,
    0,    0,  984,    0,    0,    0,  984,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  666,    0,
    0,  221,  221,    0,    0,  666,    0,    0,    0,    0,
    0,    0,    0,  984,  666,    0,    0,  427,  221,  984,
  425,    0,  426,    0,    0,    0,    0,  666,    0,  984,
    0,    0,    0,  666,  666,  428,    0,    0,    0,    0,
  283,  666,    0,  984,  984,  984,  984,  984,  984,    0,
    0,    0,    0,    0,    0,    0,    0,  984,    0,    0,
    0,  666,    0,    0,  666,    0,  984,  984,  984,  984,
  984,  984,  984,  984,  984,  984,  984,    0,  984,    0,
    0,  984,  984,    0,  984,  984,  984,  984,  984,  984,
  984,  984,  666,  666,  984,  984,  984,  984,  984,    0,
    0,    0,  984,  984,  666,    0,    0,    0,  666,    0,
    0,    0,    0,    0,  489,    0,    0,  425,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,    0,    0,    0,    0,  283,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  666,    0,    0,    0,    0,
    0,  666,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  666,    0,    0,    0,  990,    0,    0,    0,  990,
    0,    0,    0,    0,    0,  666,  666,  666,  666,  666,
  666,    0,    0,    0,    0,  992,    0,  990,    0,  666,
  990,    0,    0,    0,    0,    0,    0,    0,  666,  666,
  666,  666,  666,  666,  666,  666,  666,  666,  666,    0,
  666,    0,    0,  666,  666,    0,  666,  666,  666,  666,
  666,  666,  666,  666,  389,    0,  666,  666,  666,  666,
  666,  990,    0,  390,  666,  666,    0,  990,  990,  990,
    0,  990,  990,  990,    0,  990,  391,    0,    0,    0,
   98,    0,  392,  393,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  990,    0,  990,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  394,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,   99,    0,    0,    0,    0,    0,    0,  990,    0,
    0,    0,    0,  100,    0,    0,    0,    0,    0,  396,
    0,  397,  398,    0,    0,  990,    0,    0,    0,    0,
    0,  389,    0,  399,    0,    0,    0,  400,  101,    0,
  390,    0,    0,  489,    0,    0,  425,  990,  426,    0,
    0,    0,    0,  391,    0,    0,    0,    0,    0,  392,
  393,  428,    0,    0,    0,    0,  283,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  990,  990,    0,    0,    0,  394,    0,    0,
  395,    0,    0,    0,  401,    0,    0,    0,    0,  990,
  402,    0,    0,    0,    0,    0,    0,  102,    0,    0,
  403,    0,    0,    0,    0,    0,    0,  103,  397,  398,
    0,    0,    0,    0,  404,  405,  406,  407,  208,  209,
  399,    0,    0,    0,  400,    0,    0,    0,  408,    0,
    0,    0,  104,    0,    0,    0,    0,  409,  410,  411,
  412,  413,  414,  415,  416,  417,  106,  418,    0,  108,
  105,    0,  419,  420,    0,  421,  422,  423,  111,  112,
  113,  424,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,  106,
  107,  401,  108,    0,    0,    0,    0,  402,    0,  109,
  110,  111,  112,  113,    0,  114,    0,  403,  115,  116,
  117,  118,  119,    0,    0,  691,  120,  121,    0,    0,
    0,  404,  405,  406,  407,  208,  209,    0,    0,    0,
    0,    0,    0,    0,    0,  408,    0,    0,    0,    0,
    0,    0,    0,    0,  409,  410,  411,  412,  413,  414,
  415,  416,  417,  106,  418,    0,  108,    0,    0,  419,
  420,    0,  421,  422,  423,  111,  112,  113,  424,  114,
  389,    0,  115,  116,  117,  118,  119,    0,    0,  390,
  120,  121,  489,    0,    0,  425,    0,  426,    0,    0,
    0,    0,  391,    0,    0,    0,    0,    0,  392,  393,
  428,    0,    0,    0,    0,  283,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  394,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  397,  398,    0,
    0,    0,  489,  948,    0,  425,    0,  426,    0,  399,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
  428,    0,    0,  105,    0,  283,    0,    0,    0,    0,
    0,    0,  164,  165,  166,  167,  168,  169,  170,  171,
  172,  173,  174,  175,  176,  177,  178,  179,    0,    0,
  180,  181,  106,  107,    0,  108,    0,    0,  182,  183,
  699,    0,  109,  110,  111,  112,  113,  184,  114,  185,
  401,  115,  116,  117,  118,  119,  402,    0,    0,  120,
  121,  186,  187,  188,  189,    0,  403,    0,    0,    0,
  599,    0,    0,    0,  600,    0,    0,    0,    0,    0,
  404,  405,  406,  407,  208,  209,    0,    0,    0,    0,
    0,    0,  601,    0,  408,  602,    0,    0,    0,    0,
    0,    0,    0,  409,  410,  411,  412,  413,  414,  415,
  416,  417,  106,  418,    0,  108,    0,    0,  419,  420,
    0,  421,  422,  423,  111,  112,  113,  424,  114,  389,
    0,  115,  116,  117,  118,  119,  603,    0,  390,  120,
  121,    0,  604,  605,  606,    0,  607,  608,  609,    0,
  610,  391,    0,    0,    0,    0,    0,  392,  393,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  611,    0,  612,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  394,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  613,    0,    0,    0,    0,    0,  389,
    0,    0,    0,    0,  396,    0,  397,  398,  390,    0,
  614,  489,    0,    0,  425,    0,  426,    0,  399,    0,
    0,  391,  400,    0,    0,    0,    0,  392,  393,  428,
    0,    0,  615,    0,  283,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  394,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,  616,  617,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  401,
    0,    0,    0,    0,  618,  402,  397,  398,    0,    0,
    0,    0,    0,    0,    0,  403,    0,    0,  399,    0,
    0,    0,  400,    0,    0,    0,    0,    0,    0,  404,
  405,  406,  407,  208,  209,    0,    0,    0,    0,    0,
    0,    0,    0,  408,    0,    0,    0,    0,    0,    0,
    0,    0,  409,  410,  411,  412,  413,  414,  415,  416,
  417,  106,  418, 1438,  108,    0,    0,  419,  420,    0,
  421,  422,  423,  111,  112,  113,  424,  114,    0,  401,
  115,  116,  117,  118,  119,  402,    0,    0,  120,  121,
    0,    0,    0,    6,    0,  403,    0,    0,    0,    0,
    0,    0,    0,    0, 1555, 1556,    0, 1557,    0,  404,
  405,  406,  407,  208,  209,    0,    0,    0,    0,    0,
    0,    0,    0,  408, 1558,    0,    0,    0,    0,    0,
 1559,    0,  409,  410,  411,  412,  413,  414,  415,  416,
  417,  106,  418,    0,  108,    0,    0,  419,  420,    0,
  421,  422,  423,  111,  112,  113,  424,  114,  389,    0,
  115,  116,  117,  118,  119,    0,    0,  390,  120,  121,
  489,    0,    0,  425,    0,  426,    0,    0,    0,    0,
  391,    0,    0,    0,    0,    0,  392,  393,  428,    0,
 1560,    0,    0,  283,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   14,
    0,    0,    0,    0,  394,    0,    0,  395,    0,    0,
    0,    0,    0,    0, 1561,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   16,    0,    0,    0,    0,    0,
    0,    0,    0, 1699,    0,  397,  398,    0,    0,    0,
  489,    0,  105,  425,    0,  426,    0,  399,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,  428,    0,
    0,    0,    0,  283,    0,    0,    0,    0,    0,    0,
    0, 1562,  107,    0,  108,    0,    0,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115, 1563,  117,  118,  119,    0,    0,    0,  120,  121,
 1564, 1565, 1566,    0,    0,    0,    0,    0,  401,    0,
    0,    0,    0,    0,  402,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  403,    0,    0,    0,  133,    0,
    0,    0,  133,    0,    0, 1156,    0,    0,  404,  405,
  406,  407,  208,  209,    0,    0,    0,    0,    0,    0,
  133,    0,  408,  133,    0,    0,    0,    0,    0,    0,
    0,  409,  410,  411,  412,  413,  414,  415,  416,  417,
  106,  418,    0,  108,    0,    0,  419,  420,    0,  421,
  422,  423,  111,  112,  113,  424,  114,  389,    0,  115,
  116,  117,  118,  119,  133,    0,  390,  120,  121,    0,
  133,  133,  133,    0,  133,  133,  133,    0,  133,  391,
    0,    0,    0,    0,    0,  392,  393,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  133,    0,
  133,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  394,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  133,    0,    0,    0,    0,    0,  389,    0,    0,
    0,    0,    0,    0,  397,  398,  390,    0,  133,  489,
    0,    0,  425,    0,  426,    0,  399,    0,    0,  391,
  400,    0,    0,    0,    0,  392,  393,  428, 1743,    0,
  133,    0,  283,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  394,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,    0,  133,  133, 1341,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  401,    0,    0,
    0,    0,  133,  402,  397,  398,    0,    0,    0,    0,
    0,    0,    0,  403,    0,    0,  399,    0,    0,    0,
  400,    0,    0,    0,    0,    0,    0,  404,  405,  406,
  407,  208,  209,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
  409,  410,  411,  412,  413,  414,  415,  416,  417,  106,
  418,    0,  108,    0,    0,  419,  420,    0,  421,  422,
  423,  111,  112,  113,  424,  114,    0,  401,  115,  116,
  117,  118,  119,  402,    0,    0,  120,  121,    0,    0,
    0,    0,    0,  403,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1354,    0,    0,  404,  405,  406,
  407,  208,  209,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
  409,  410,  411,  412,  413,  414,  415,  416,  417,  106,
  418,    0,  108,    0,    0,  419,  420,    0,  421,  422,
  423,  111,  112,  113,  424,  114,  389,    0,  115,  116,
  117,  118,  119,    0,    0,  390,  120,  121,  489,    0,
    0,  425,    0,  426,    0,    0,    0,    0,  391,    0,
    0,    0,    0,    0,  392,  393,  428,    0,    0,    0,
    0,  283,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  394,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  397,  398,    0,    0,    0,  984,    0,
    0,  984,    0,  984,    0,  399,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,  984,    0,    0,  468,
    0,  984,    0,    0,    0,    0,    0,    0,    0,  468,
  468,  468,  468,  468,  468,  468,  468,  468,  468,  468,
  468,  468,  468,  468,    0,    0,  468,  468,  468,  468,
    0,  468,    0,    0,  468,  468,    0,    0,  468,  468,
  468,  468,  468,  468,  468,  468,  401,  468,  468,  468,
  468,  468,  402,    0,    0,  468,  468,  468,  468,  468,
  468,    0,  403,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  404,  405,  406,  407,
  208,  209,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,    0,    0,    0,    0,    0,    0,  409,
  410,  411,  412,  413,  414,  415,  416,  417,  106,  418,
    0,  108,    0,    0,  419,  420,    0,  421,  422,  423,
  111,  112,  113,  424,  114,  389,    0,  115,  116,  117,
  118,  119,    0,    0,  390,  120,  121,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  391,    0,    0,
    0,    0,    0,  392,  393,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  394,    0,    0,  395,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  552,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  984,    0,  553,    0,    0,
    0,    0,  397,  398,  984,    0,    0,    0,    0,  569,
    0,    0,    0,    0,  399,    0,    0,  984,  400,    0,
    0,    0,    0,  984,  984,  552,  552,    0,    0,  552,
    0,    0,    0,    0,    0,    0,    0,  553,  553,    0,
    0,  553,    0,    0,  552,    0,  552,    0,    0,    0,
  569,  984,    0,  569,  984,    0,  553,    0,  553,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  569,    0,
  569,    0,    0,    0,    0,  401,  552,    0,    0,    0,
    0,  402,  984,  984,    0,    0,    0,    0,  553,    0,
    0,  403,    0,    0,  984,    0,    0,    0,  984,    0,
  569,    0,    0,    0,    0,  404,  405,  406,  407,  208,
  209,    0,    0,    0,    0,    0,    0,    0,    0,  408,
    0,    0,    0,    0,    0,    0,    0,    0,  409,  410,
  411,  412,  413,  414,  415,  416,  417,  106,  418,    0,
  108,    0,    0,  419,  420,    0,  421,  422,  423,  111,
  112,  113,  424,  114,    0,  984,  115,  116,  117,  118,
  119,  984,    0,    0,  120,  121,    0,    0,    0,    0,
    0,  984,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  984,  984,  984,  984,  984,
  984,    0,    0,    0,    0,    0,    0,    0,    0,  984,
    0,    0,    0,    0,    0,    0,    0,  559,  984,  984,
  984,  984,  984,  984,  984,  984,  984,  984,  984,    0,
  984,    0,    0,  984,  984,    0,  984,  984,  984,  984,
  984,  984,  984,  984,    0,    0,  984,  984,  984,  984,
  984,    0,    0,  552,  984,  984,    0,    0,  559,    0,
    0,  559,    0,    0,    0,  553,    0,    0,    0,  552,
  544,    0,  552,  552,    0,  552,  559,  569,  559,  552,
    0,  553,  552,    0,  553,  553,    0,  553,    0,    0,
    0,  553,    0,  569,  553,  552,    0,  569,    0,  569,
    0,  552,    0,  569,  552,    0,  569,  553,  559,    0,
    0,  544,    0,  553,  544,    0,  553,    0,    0,  569,
    0,    0,    0,    0,    0,  569,    0,    0,  569,  544,
    0,  544,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  544,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  552,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  553,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  552,    0,  569,    0,    0,
    0,    0,  552,    0,    0,    0,  552,  553,    0,    0,
    0,    0,    0,  552,  553,    0,    0,    0,  553,  569,
    0,    0,    0,    0,    0,  553,  569,    0,    0,    0,
  569,  552,    0,    0,    0,    0,  552,  569,    0,    0,
    0,    0,    0,  553,    0,    0,    0,    0,  553,    0,
    0,    0,    0,    0,    0,  569,    0,    0,    0,    0,
  569,  552,    0,    0,    0,  552,    0,    0,    0,    0,
    0,    0,    0,  553,    0,  559,    0,  553,    0,    0,
    0,    0,    0,    0,    0,  569,    0,    0,    0,  569,
    0,  559,    0,    0,    0,  559,    0,  559,    0,    0,
    0,  559,    0,    0,  559,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  559,    0,    0,
    0,    0,    0,  559,    0,  326,  559,    0,  544,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  544,    0,    0,    0,  544,    0,
  544,    0,    0,    0,  544,  326,    0,  544,    0,    0,
  599,    0,  326,    0,  600,    0,  326,  326,    0,  326,
  544,  800,  326,    0,    0,    0,  544,    0,    0,  544,
    0,    0,  601,    0,    0,  602,  326,    0,    0,    0,
    0,    0,  326,    0,    0,  559,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  559,    0,    0,
    0,    0,    0,    0,  559,    0,  603,    0,  559,    0,
    0,    0,  604,  605,  606,  559,  607,  608,  609,    0,
  610,    0,    0,    0,    0,    0,    0,    0,  544,    0,
  599,    0,  326,  559,  600,    0,    0,    0,  559,    0,
  611,    0,  612,    0,    0,    0,    0,    0,    0,    0,
  544,  326,  601,    0,    0,  602,    0,  544,    0,    0,
    0,  544,    0,  559,    0,    0,  326,  559,  544,    0,
    0,    0,    0,  613,    0,    0,  326,    0,    0,    0,
    0,    0,    0,    0,    0,  326,  544,  326,    0,    0,
  614,  544,    0,    0,  326,    0,  603,    0,    0,    0,
    0,    0,  604,  605,  606,    0,  607,  608,  609,    0,
  610,    0,  615,    0,    0,    0,  544,    0,    0,    0,
  544,    0,    0,  326,  326,    0,  326,  105,    0,    0,
  611,    0,  612,  326,  326,  326,  326,  326,    0,  326,
    0,    0,  326,  326,  326,  326,  326,  616,  617,    0,
  326,  326,  326,  326,  326,    0,  106,  107,    0,  108,
    0,    0,    0,  613,  618,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
  614,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  615,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  616,  617,    0,
    0,    0,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,    0,  618,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,
};
static short yycheck[] = {                                       9,
   16,    4,   12,   55,    0,   64,   16,   63,   18,  291,
  104,  140,  141,  367,  151,  517,  359,  140,  141,  336,
  714,  367,   91,  363,   56,   62,   85,  500,   68,   57,
   63,  691,  882,   94,   73,   67,  886,   89,  567,  338,
  477,   69,  524,   53,   54,   55,   56,   57,   58,   70,
  427,   91, 1051,   63,   64,   65,   95,   67,   90,   69,
   70,  558,   72,   73,  561,   97,  760,   83,   41,  775,
  567,   81,   93,   83,   84,   85,   92,   87,   88,   89,
   90,  268,   92,   93,   94,   95,  384,   97, 1044,   88,
  277,  944, 1272,  154,  372, 1051,   41, 1052,  131, 1044,
  133,  331,  836, 1044,  132,  285, 1051, 1044, 1063, 1044,
 1051, 1044,  651,    0, 1051,   44, 1051,    0, 1051,  312,
  152,  131,  132,  133,  699,  153,   40,  767, 1083,  982,
  732,    0,  713,  644,   45,  145,    0,   45,  232,    0,
  992,  151,  152,  153,  154,   63,  145,    0,  158,   63,
   45,   41,    0,  728,   41,  867,  193,  797,   41,   41,
   40, 1437,   44, 1095,    0,   41,  806,    0,    0,    0,
   41,   44,   59,    0,    0,  312,   59,   41,    0, 1077,
   41,  218,   41,   63,    0,  275,   44,   59,   41,  158,
   59,   44, 1390,   41,  534,   59,   41,   44,   59,   44,
   40,  544,  545,  546,  547,    0,   59, 1405,   41,   41,
    0,   59, 1214,  567,   41,   41,   44,   44,   44,   41,
  860,  567,   44,   59,    0,  319,   59,   59,   59,  262,
   45,  274,   59,   59,    0,   40, 1478,   59,  266,  741,
   40,  519,   44,   59,  296,   44,   41,  275, 1101,   44,
  910,  911,  262,  570,  594,    0,  266,    0,  268,  269,
 1100,    0,  260,  657,   59,  275,  223,  277,  338,   59,
 1229,    0,  300,   44,  357,   41,  283,   45,   44,  312,
  262,  941,  469,   59,   41,   12,  296,   12, 1564, 1565,
  300,  268,  262,   59, 1270,   63,   41,  483,   41, 1298,
 1721,  599,   41,  620,  337,  318,  260,  348,  355, 1241,
    0,  344,   41,  346,   59,  348,   59,  273,  351,  445,
   59,    0,    0,  360,  257,  362,  823,  337,  380,   41,
   59,    0, 1287, 1320,  344,    0,  346,  369,  348,  376,
  477,  351, 1298,    0,    0,   16,    0, 1218,  405,  370,
  378,   41,   41, 1298, 1775,  377,    0, 1298,  315,  369,
  370, 1298,   41, 1298,  455, 1298,    0,    0,  378,   59,
  380,  844,  442,  975,   44, 1015,    0,  384, 1230,  405,
   59,   59,  508, 1043,  340,  987,  988,   41,  365,   59,
   59,  262,  274, 1733,   59,  485,  418,  679,  424,  456,
 1821,  334,   59,   59,  386,   59, 1304,   41,   41,  402,
    0,  138,   83,  138,  696,   59,  386, 1693,  428,  417,
  461,  462, 1062,  466, 1604,   59,   59,  309,   40,  269,
  712,  318,  439,   45,  517,   59,  468,  465,  328,    0,
  453,  331,  387,  465,  477,  492, 1317,  320,  476,  456,
 1409,  483,  480,  340,  318,  465,  585,  485,  468,  469,
  742, 1121, 1122,  417, 1124, 1125,  476, 1443,  785,   59,
  480,  788,  331,  483,    0,  485,  340, 1719,  488, 1434,
   41, 1215,  764,   44, 1439,  802, 1441, 1827,  264,   41,
  318,  318,  318,  887,   42, 1482,  318,  387,   59,  331,
  387,  328,  539,   41,  331,  331,  396,  340,  320,  396,
   41,  387,  829,  340,  340,  386,  343,  586,  340,  488,
  396, 1476,    0,  318, 1456,  454,  387, 1529,  387,  411,
  383, 1711, 1730,   59,  567,  396,  723,  396,  410,  387,
  458, 1247, 1474,  980,  458,  340,  586,  458,  521,  735,
  458,  588, 1133,  440,  784,  387,  566,  567,  738, 1293,
  387,  387,  572,  458,  396, 1417,  411,  454,  761,  396,
  396, 1110,  871,  318,  485,  318,  521,    0,  458,  318,
  858,   59,  880, 1094,  466,  454, 1541,    0, 1768,  394,
  454, 1489, 1547, 1253,  410,  340,  454,  340, 1310,  328,
  436,  340,  331,  640,  641,  452,  983,  564,  264,  440,
  643, 1887,  383,  440,  440, 1275,  431,  746,   41,  409,
  309,  454, 1224,  454,  761,  452,   41,  454,  454,  386,
 1810,  521,  454,  643,  521, 1475,   59,  436,  521,  318,
  264, 1494,  652,  458,  387,  521,   59,  447,  501,  652,
  340,  780,  781,  396, 1609,   41,  458,  939,  387,  454,
  521,  340,  521,  431,  318,  809,  732,  396,  258, 1309,
  348, 1626,  682,  521,  386,   44, 1185,  634,   44, 1188,
  502,  271,  715,  682,  328,  318,  340,  331, 1031,  521,
  458,  348,    0,  352,  521,  521,  706,  440,  367,  521,
 1265,    0,  367,  735,  714,  715,  340,  340,  718,  454,
  262,  454,  260,  723,  783,  454,  306,   44,  308,  378,
  301,  313,  732, 1429,  262,  735,  521,  755,  761,  757,
  740,  262, 1049,  766,  744,  501,    0,  774,  775,  776,
 1739,  397,  161,  783, 1061,  755, 1311,  757,   61,  348,
  760,   59,  821, 1096,  387,  303,  766,  318,  357, 1419,
   59,  794,   91, 1718,  454, 1082, 1341,  328,    0,  273,
  331,  320, 1727,  397,  286,  454,  521,  964,  521,  340,
  967,  821,  343, 1739,  794,  854,  855,  856,  828,  822,
  823, 1273,  521,  506, 1739,   59,  440,  834, 1739,  262,
  454,  841, 1739,  257, 1739,  331, 1739,  440,  452, 1108,
  454,    0,  822,  823,  854,  855,  856, 1772, 1195,  431,
  454,  454,    0, 1296,   59,  417,  387,   59,  418, 1302,
    0,  859,   42,    0,  386,  396, 1207,   47, 1209,    0,
 1369,  869,  521,  980, 1022,  301,  458,  262,  386,  859,
  328, 1228,   41,  331, 1430,  386, 1421,    0,  264,  869,
    0,  442,  283,   41,   42,   43,   44,   45,   46,   47,
   59,  881, 1171,    0, 1383,    0,  262,   44, 1387,  440,
  334,   59,   60,   61,   62,    0,  262,  436,  521,   59,
  300,  452,   59,  454,    0,  318,  409,  657,   59,  402,
  308, 1477,  662, 1185,  323,  328, 1188,  968,  331,  975,
  318, 1055, 1056,   91,  440,  356,   59,  340,  517,   59,
  318,  987,  387,  386,  403,    0,    0, 1105,  454,  342,
  969,  434,   59,  965,   59,  304, 1218,  264,  304,    0,
 1790,  420,  352,    0,   59,  966,  449,  980,  277,  461,
  462, 1080,  278,   59,  964,  965,  966,  967,  968,  969,
  521, 1105,  440,    0,  387,  975,   41,    0, 1285,   44,
  301,  386,  435,  396, 1314,  440,  454,  987,  988,  278,
   41,  280,  992,   44,   59,   59,  442,  300,  429, 1022,
  377,    0,  405,  319,    0,  408,  992,    0,   59, 1316,
  386,   42,   59,  308, 1664,  411,  402,  338, 1731, 1732,
  386,  424, 1022,  318, 1523, 1369,   41,  440,  439,   44,
 1089,  261,   59, 1369, 1684, 1527,   59,  263, 1530,  452,
  458,  454,   41,  325,  342, 1317, 1407,  450, 1107, 1575,
  458, 1659,  438, 1661, 1113,  453,  278,   44,  280, 1089,
   59, 1616, 1549,   59, 1777,  453,   59, 1622,    0,  435,
    0,  448, 1072,  405,  343, 1098,    0, 1107,   93,  377,
  280, 1074, 1105, 1113,  400,  285,  312, 1087,  342,  378,
  307,  308,  401, 1426,  262,  397,  402,  323, 1098,  368,
 1213, 1214, 1215,    0,  272, 1105,   93,  405,  521,  319,
  408, 1383,  280,  264, 1764, 1387, 1800,  285, 1726,  279,
  288,  442,   40,  377,  456,  441,  424,   59,  258,   59,
 1249,  264, 1251,  883,  436,   59, 1628,  887, 1445,  445,
  300,  271,  265, 1856,   41,  307,  308,  264,  430,  264,
  318,  405,  450,  321,  408,  437,  324,  426,  453,  264,
  328,  340,   59,  331,    0,  333,  258,  335,  470,  329,
  424,  328,  340,  341,  331,  343,  306, 1484,  308,  302,
   42,   43,  278,   45,  280,   47,  343,  322,  348,    0,
  400,  359,  360, 1312,  362,  259,  450,  357,    0,    0,
  264,  427,  508,  363,  322,   41,  270, 1207,  376, 1209,
  378,  379,  380,   44,  306,  383, 1824,  264,  386,  387,
 1247,  381,  301,   59, 1224,  277, 1876,  278,  396,  280,
 1230,  441,   41,  321,  451,   44,  287,  264,  383,   40,
   41,  264,   44,   44,  383,  413,  397,  335,   59,  458,
    0, 1523,  378,  318,    0,  343,  321,   59,   59,  338,
  258,  429,   93,  328,  397,  296,  331,  435, 1780, 1781,
  335,  359,  440,  271,  400,  340,  485,    0,  343, 1805,
  397,  407,  397,  440,  452, 1412,  454, 1779,  418,  451,
  272,   41,  397,  345,  359,  452,  260,  454, 1291,  260,
 1327,    0,   42,   43, 1331,   45,  288,   47,  306,   59,
  308, 1438,   44,   59, 1840,  441, 1623,  279,  383,  318,
    0,  282,  387,  260,    0,  413,  418,  378,  407,  328,
  291,  396,  331,  397,  258,    0,   59,    0,  417,  303,
    0,  340,   41, 1723,  436,   44, 1872,  271,  413,  400,
  397,  333,    0,  521,    0,  279,  407,  517,  259, 1885,
   59,  322, 1888,  442,  325, 1877,  303,  329,   44,  270,
  397, 1897,  265,   44,  397,  440,  300,    0,  470,   59,
  362,   41,  306,   59,  308, 1825, 1826,  452,  387,  454,
  441, 1437,  321, 1773,   59,   41,   59,  396,   44,   59,
  417,  363, 1429,    0, 1531,  329,  335, 1407,    0,  302,
  342,   59, 1412,   59, 1437, 1438, 1529, 1417,  280,  381,
  418,  318,   93,  285,  348, 1544,  301,  444, 1446,  460,
  359,  328,  356,  357,  331,  385,   59, 1437, 1438,  363,
 1820,  440,    0,  340,  337,  377, 1446,  429, 1575,   40,
  501, 1891,    0,    0,  383,  454,  521,  381,  489,  490,
  421,  492,   59,  338,  425,  426,  427,   59,  499,  500,
  501,  502,  503,  405,  505, 1855,  408,  508,  509,  510,
  511,  512,  318,  287,  413,  516,  517,    0,   42,   43,
  387,   45,  424,   47,  418,  331,  300,  417,   44,  396,
  321,   59,    0,  383,  340,  429, 1555,   41,  242,  383,
   44,   59,   59,    0,  335,  436,  250,  318,  450,    0,
  321,    0,  521, 1607,  444,    0,  328,  328,  489,  331,
  331, 1531,  493,   41,  335,  260,   44,    0,  359,  340,
  280,  343,  343,  440,  260,  285,   59,   93,    0,  280,
   41,  387, 1575,   44,  285, 1555,  279,  454,  359,   93,
  396,   59,  383, 1563, 1564, 1565, 1608,  342,  318, 1696,
  337,  286,   59,  337,  436, 1575,  301,  300,   59,  294,
   59,  331,  383,  294,   59, 1603,  387,  303,  313,    0,
  340,  494,  413,  517,  305,  396,   59,  321, 1598,  260,
  340,  312,  377, 1603,  440, 1632,  329,   59, 1608, 1636,
  374,  335,  413,  338,    0,    0,   44,    0,  454,  318,
  418, 1627,  321,  387,  521,  348,  337, 1627, 1712,  328,
  405,   59,  331,  408,  357,  359,  335,  387,  440,  440,
  363,  340,  303,    0,  343,  274,  396,    0,   59,  424,
  452,  452,  454,  454,  521,  260,   41, 1679,  381,   44,
  359,   40, 1857, 1858,   42,   43, 1793,   45,  424,   47,
 1797,    0, 1799,   59,   59,  450,   59,  342, 1805, 1679,
  258, 1808,  407,  261,  383,  521,  316,  386,  387,  413,
  440,  402,  417, 1693,  264,    0,   46,  396,  303,  316,
  317,  662,   59,    0,  454, 1900,   59, 1756,   58,    0,
 1837,   61,  377, 1840,  413, 1721,  461,  442,  679, 1705,
  521, 1721,  683,  434,  493,  494,  280,  438,  306,  690,
   59,  285, 1738, 1733,  695,  696,  435, 1864, 1738,    0,
  405,  440, 1869,  408,  705, 1872,  707,  708,  709,  710,
  711,  712,  713,  452,   59,  454, 1756, 1757, 1885,  424,
   41, 1888,   59,   44,  318, 1758,  279,  356,   59, 1775,
 1897,  521,  337,    0,  337, 1775,   46,  318, 1784,  259,
   41,  742, 1805,   44, 1784,  450,   41,  300,   58,   44,
  270,   61, 1834,  460,  517,   46,   42,   43,   59,   45,
 1800,   47,  452,  764,  393, 1805,  395,   58,  769,  770,
   61,  374,    0,   59,  321, 1821,  329, 1840,  336,    0,
  493, 1821,  521,  402,  387, 1831,  417, 1827,  335,  418,
  503, 1831,   59,   40, 1834,  348,  343,  461,  462,  678,
 1840,  680,  320,  356,  357,  684,  685,  686,   41, 1872,
  363,   44,  359,   41,  443,   41,   44,  465,   44,  467,
   41, 1861, 1885,   44,   41, 1888,  340,   44,  381,  460,
   41,   59, 1872,   44, 1897,   41,   42,   43,   59,   45,
  393,   47,  395,   41,  318, 1885,   44, 1887, 1888,  383,
   42,   43, 1892,   45, 1894,   47,  402, 1897,  489,  490,
  383,  492,  280,  279,  506,  418,  413,  285,  499,  500,
  501,  502,  503,   41,  505,  436,   44,  508,  509,  510,
  511,  512,  461,  462,  300,  516,  517,  450,   41,   41,
  443,   44,   44,  434,  895,  896,  897,  361,    0,  900,
  901,  902,  320,  904,  905,  906,  907,  908,  909,  384,
    0,  912,  913,  329,  915,   41,  917,  417,   44,  920,
  921,  922,  923,  924,  925,   41,   41,  318,   44,   44,
   42,   43,  348,   45,   41,   47,  937,   44,  939,   41,
   41,  357,   44,   44,    0,  391,  392,  363,  457,  458,
  459,   41,  516, 1207,   44, 1209,    0,   59, 1892,  516,
 1894,  262,  287,  411,  517,  381,   41,   42,   43,   59,
   45,  469,   47,   44,   61,  286,  485,  486,  372,   40,
  264,   40,  983,   40,   40,   41,   42,   43,   44,   45,
   46,   47,   40,   40,  280,  461,  461,   41,   40,  285,
   44,   40,   40,   59,   60,   61,   62,   40,   40,  461,
   40,   42,   43,   40,   45,   59,   47,  318,   40,   40,
  321,   40,   91,   46,   44,    0,  436,  328,  320,   44,
  331,  402,  320,   44,  335,   91,   44,   93,   44,  340,
  264,  262,  343,  428,   46,   61,  264,   40,   42,   43,
  278,   45,  280,   47,  340,  442,  454, 1696,  359,  287,
   44,  348,  278,  348,  278,  348,   41,  264,  300,   44,
  397,  436,    0,  397,  280,   42,   43,   44,   45,  285,
   47,  470,  383,  458,   59,  386,  387,  277,  280,  348,
  447,  447,  399,  285,   40,  396,  349,  318,   40,   40,
  321,  517,   91,   40,   40,    0,  318,  328,  264,  383,
  331,  411,  413,   41,  335,   44,   44,  389,  382,  340,
   40,   58,  343,   41,   42,   43,   40,   45,  320,   47,
   41,   59, 1133,  451,  435,  464,  317,  436,  359,  440,
   40,  320,  320,  452, 1145, 1146,   41, 1148, 1149,   44,
  378,  452,  448,  454, 1793, 1156,  264,  382, 1797,  502,
 1799,  505,  383,   40,   59,  386,  387,  277,  280, 1808,
  274,   40,  400,  285, 1175,  396,  278,  458,  280,  407,
  300,  284, 1183, 1184, 1185,  287,  344, 1188,  278, 1190,
  280,  386,  413, 1194, 1195, 1196, 1197,  287, 1837,  311,
    0,  344,  278,  386,  386,  280,  262,  416,  264,  265,
  285,  418,  300,  441,  435,  418,  272, 1218,   40,  440,
  521,  356,   40,  417,  280, 1864,  417, 1228,  274,  285,
 1869,  452,  288,  454,  278,   40,  280,   41,  264,  320,
  284,   41,   44,  287,   44,   44,  302,   44,  385,  280,
  367,  307,  308,   44,  285,  311,  300,  320,   40,   59,
   44,   61,  318,  280,  320,  321,  328, 1268,  324,  266,
    0,  265,  328,  501,   40,  331,  378,  333,  501,  335,
  311,  383,   40,  264,  340,  341,  280,  343,  378,  264,
  299,  285,  422,   41,   58,   41,   41,   41,  400,  264,
  521,  440,  394,  359,  360,  407,  362,  454,  302,  309,
  400,   41,  492,  280,   44,   41, 1317,  407,  285,  309,
  376,   41,  378,  379,  380, 1696,   40,  383,   40,   59,
  386,  387,  307,  308,  378,   40,   40, 1338,  352,  441,
  396,   40,   40,   40,   40,  320, 1347,    0,   40,   40,
  278,  441,  280, 1354,  410,   40,  400,  413,   40,    0,
   40,   40,  280,  407,   41,   42,   43,  285,   45,   40,
   47,   40,  262,  429,  308,  435,  327,   44,   41,  435,
  436,   41, 1383,  278,  440,  280, 1387,  388,   41,   42,
   43,   44,   45,   46,   47,  451,  452,  441,  454,  501,
   41,  388,  388,   44,  460,  343,   59,   60,   61,   62,
  274,  501,  331,   44,  390,  422,  271, 1418,   59,  391,
  287,  446, 1793,  386,  407,   40, 1797,    0, 1799,  386,
  386,  386,  386,  489,  490,  410,  492, 1808,   91,  386,
   93,   41,   41,  499,  500,  501,  502,  503,  378,  505,
  378,  278,  508,  509,  510,  511,  512,   40,  258,   41,
  516,  517,   41,   41,  417,  521, 1837,   41,   41,  308,
  385,   44,  400,  435,  396,  436,  451,  452,  278,  407,
  280,  337,  280,  378,  284,  460,   59,  287,  338,  301,
  442,  338,  442, 1864,  301,   41,   41,  320, 1869,  352,
  300, 1502, 1503,  385,   41,  400,  306,   41,  371,  371,
   41,   41,  407,  441,  489,  490,   41,  492, 1519,   41,
   41,   41, 1523,  460,  499,  500,  501,  502,  503,   41,
  505,   41,   41,  508,  509,  510,  511,  512,   41,   41,
   41,  516,  517,   41,   41, 1793,  441,   41,   44, 1797,
  293,   59,  489,  490,   59,  492,   40,  340,  308, 1560,
 1808,  340,  499,  500,  501,  502,  503,   59,  505,   59,
   59,  508,  509,  510,  511,  512,   59,   59,  378,  516,
  517,   41,   42,   43,   59,   45,   59,   47,  318, 1837,
   58,  321,   41,  352,   40,  344,  368,   40,  328,  318,
  400,  331,   59,  280,   40,  335,  451,  407,  285,  262,
  340,  264,  265,  343,  383,   44, 1864,   40,  418,  272,
   59, 1869,   41,   42,   43,  340,   45,  280,   47,  359,
 1631,   46,  285,  340,   41,  288,  436,  278, 1639,  280,
  343,  441,  383,  284,  383,   41,  287,   41,  410,  302,
  435,  304,   44,  383,  307,  308,    0,  387,  311,  300,
  387,  301,  442,   40, 1665,  318,  396,  320,  321,  284,
  470,  324,   41,   59,   44,  328,  304, 1678,  331,  337,
  333, 1682,  335,  413,  307,  463,  293,  340,  341,  262,
  343,   42,   43,  304,   45,   44,   47,   41,   42,   43,
   44,   45,   46,   47,   41,   40,  359,  360,   41,  362,
  440,    0,  337,  383,   40,   59,   60,   61,   62,  383,
  264,  278,  452,  376,  454,  378,  379,  380,  264,  458,
  383,  320,   40,  386,  387,  389,    0,  378,  264,  470,
  451,   44,   40,  396, 1745,  318,  411,   91,  321,   93,
  309,  264,  458,  271,  435,  328,  343,  410,  331,  400,
  413,  436,  335,  343,  458,  271,  407,  340,  343,   41,
  343,  343,   41,   40,   59,   41,  429,   41,   40,   59,
   44,  308,  435,  436,   41,   59,  359,  440,   59,  304,
   41,  521,   41,    0,   59,   59,    0,    0,  451,  452,
  441,  454, 1210,   12,  270,  277, 1807,  460,  269,  647,
  383,  723, 1110,  386,  387, 1108, 1105,  872, 1097,   87,
  280, 1464,  364,  396, 1465,  285,   41,   42,   43,  594,
   45, 1087,   47, 1034,  334,  766,  489,  490,  317,  492,
  413,  327,  346, 1098, 1674,  460,  499,  500,  501,  502,
  503,  346,  505, 1598, 1443,  508,  509,  510,  511,  512,
  804,  280,  435,  516,  517, 1766,  285,  440,  521, 1445,
  572, 1770, 1769, 1812,  489,  490, 1745,  492,  794,  452,
 1266,  454, 1762, 1563,  499,  500,  501,  502,  503, 1701,
  505, 1243, 1017,  508,  509,  510,  511,  512, 1806, 1721,
 1422,  516,  517,   41,   42,   43,  351,   45,  900,   47,
  902,  262,  904,  905,  339,  907,  908,  909,  262,  574,
  264,  265,  332, 1300,  851,  633,  843,   13,  272,  280,
  304,   41,   42,   43,  285,   45,  280,   47,  502,  506,
  755,  285,   42,   43,  288,   45,  143,   47,  521,  472,
  473,  474,  475,  476,  477,  478,  479,  992,  302,  769,
    0,  742, 1539,  307,  308, 1831, 1665,  311,   42,   43,
 1784,   45,  984,   47,  318,  731,  320,  321,  517, 1255,
  324, 1147, 1190,   -1,  328,   59,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,  262,  343,
   -1,   41,   42,   43,   44,   45,   46,   47,   41,   42,
   43,   -1,   45,   -1,   47,  359,  360,   -1,  362,   59,
   60,   61,   62,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   40,   -1,  383,
    0,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,  396,   93,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,   -1,  328,  280,  410,  331,   -1,  413,
  285,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   41,   -1,   -1,   44,  429,   -1,   -1,   -1,   -1,
   -1,  435,  436,   -1,   -1,  359,  440,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   41,   42,   43,   -1,   45,
   -1,   47,  396,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,  280,   -1,   -1,  489,  490,  285,  492,  413,
   -1,   -1,  460,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,  262,   -1,  508,  509,  510,  511,  512,   -1,
  280,  435,  516,  517,   -1,  285,  440,  521,   -1,   -1,
  280,  489,  490,   -1,  492,  285,   -1,   -1,  452,   -1,
  454,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  280,   -1,  516,  517,
   -1,  285,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   41,   42,   43,   -1,   45,
  280,   47,   -1,   -1,   -1,  285,   -1,  280,  288,   41,
   42,   43,  285,   45,   -1,   47,   -1,  521,   -1,   -1,
   -1,  280,  302,   -1,    0,   -1,  285,  307,  308,   -1,
   -1,  311,   -1,   41,   42,   43,   -1,   45,  318,   47,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,  262,  343,   -1,   41,   42,   43,   44,   45,
   46,   47,   41,   42,   43,   -1,   45,   -1,   47,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   42,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,  396,   93,  318,   -1,
   -1,  321,   -1,   -1,  280,   -1,   -1,   -1,  328,  285,
  410,  331,  280,  413,   -1,  335,   -1,  285,   -1,   -1,
  340,   -1,   -1,  343,   -1,   41,   -1,   -1,   44,  429,
   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,   -1,  359,
  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454, 1339, 1340,   -1,   -1,   -1,
  460, 1345, 1346,  383, 1348, 1349,  386,  387,   -1,   -1,
   -1,   -1, 1356, 1357, 1358, 1359,  396, 1361, 1362, 1363,
 1364, 1365, 1366,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,  280,  492,  413,   -1,   -1,  285,  460,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  280,  435,  516,  517,   -1,  285,
  440,  521,   -1,   -1,   -1,   -1,  489,  490,  280,  492,
   -1,   -1,  452,  285,  454,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  280,  516,  517,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,  305,   -1,   -1,  262,   -1,  264,  265,
  312,   -1,   -1,   -1,  316,  317,  272,   -1,   -1,   -1,
   -1,   -1,   -1,  325,  280,   -1,   -1,   -1,   -1,  285,
   -1,  280,  288,   -1,   -1,  337,  285,    0,   -1,   -1,
   -1,  521,   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,
   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   41,  335,
   -1,   44,   -1,  294,  340,  341,  262,  343,   -1,   41,
   42,   43,   44,   45,  305,   47,   59,   -1,   61,   -1,
  402,  312,   -1,  359,  360,   -1,  362,   59,   60,   61,
   62,   -1,   -1,   -1,  325,   -1,   -1,  419,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   44,  383,   -1,   -1,
  386,  387,  434,   -1,   -1,   -1,  438,   -1,   -1,   -1,
  396,   93,  318,   -1,   -1,  321,   -1,  449,   -1,   -1,
   -1,   -1,  328,   -1,  410,  331,   -1,  413,   -1,  335,
   -1,   -1,    0,   -1,  340,   -1,  468,  343,   -1,   -1,
   -1,   -1,   -1,  429,   -1,  386,   -1,   -1,   -1,  435,
  436,   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  402,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   41,  460,   -1,   44,  383,  419,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   59,   -1,  434,   -1,   -1,   -1,  438,   -1,   -1,
  441,   -1,   -1,  489,  490,   -1,  492,  413,  449,   -1,
  460,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  468,   -1,  435,
  516,  517,   -1,   -1,  440,  521,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  452,   -1,  454,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  258,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,  274,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,  300,   -1,   -1,
   -1,   -1,   -1,  306,   -1,  521,  309,   -1,   -1,   -1,
  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,    0,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,  320,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   41,
  258,   -1,   44,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,  271,  386,  387,   -1,   59,   -1,   61,
   -1,  279,   -1,   -1,  396,   93,   -1,   -1,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,  410,   -1,
   -1,  413,  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,
  308,   -1,   -1,  436,   -1,   -1,   -1,  429,   -1,   -1,
    0,   -1,   -1,  435,  436,   -1,   -1,   -1,  440,   -1,
   -1,  329,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,  466,   -1,   -1,   -1,  470,  460,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  357,
   -1,   41,   -1,   -1,   44,  363,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   59,
  492,   -1,  460,  381,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,    0,  521,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
  418,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   42,   43,   44,   45,  262,   47,  264,  265,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,  258,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  302,  277,  278,   -1,  280,  307,
  308,   93,  284,  311,   -1,  287,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,  300,  517,
  328,   -1,   -1,  331,  306,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,    0,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  378,   44,  278,   -1,
  280,   -1,  410,   -1,   -1,  413,   -1,  287,   -1,    0,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  400,   -1,
  300,  429,   -1,   -1,   -1,  407,   -1,  435,  436,   -1,
   -1,   -1,  440,   -1,  416,   -1,  418,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   41,   -1,  460,   44,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,  489,  490,   -1,  492,   -1,   -1,   -1,  470,   -1,
  295,  499,  500,  501,  502,  503,  288,  505,  378,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  302,   -1,    0,  521,   -1,  307,  308,   -1,  323,  311,
  400,   -1,   -1,   -1,   -1,   -1,  318,  407,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,    0,    0,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,  352,  340,  341,
   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,  441,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   41,   41,
   -1,   44,   44,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   59,   59,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,  427,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,  278,   -1,  280,   -1,   -1,   -1,  429,  443,   -1,
  287,   -1,   -1,  435,  436,   -1,   -1,   -1,  440,   -1,
   -1,   -1,  457,  458,  459,   -1,  461,  462,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,  262,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,   -1,   59,   -1,   -1,   -1,   -1,
  495,  496,   -1,   -1,   -1,   -1,   -1,  489,  490,  504,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,    0,   -1,  516,  517,   -1,  318,    0,  521,
  321,  378,   -1,   -1,   -1,   -1,   -1,  328,   -1,   45,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,  400,   -1,   -1,   -1,   -1,   -1,   -1,
  407,   -1,   -1,   -1,   -1,   -1,   44,   -1,  359,   41,
   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,   -1,
   -1,   59,   -1,   -1,  272,   -1,   -1,   59,   60,   61,
   62,   -1,  383,   -1,  441,  386,  387,   -1,   -1,   -1,
  288,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  302,  278,  278,  280,  280,  307,
  308,   93,  413,  311,  287,  287,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,  300,   -1,   -1,
  328,   -1,   -1,  331,  435,  333,   -1,  335,   -1,  440,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,  271,  386,  387,
   -1,   -1,   -1,   -1,   -1,  279,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,  378,  378,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,  413,  300,   -1,   -1,    0,
  521,   -1,  306,   -1,  308,   -1,   -1,  400,  400,   -1,
   -1,  429,   -1,   -1,  407,  407,   -1,  435,  436,   -1,
   -1,   -1,  440,   -1,   -1,  329,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   41,   -1,  460,   44,  348,   -1,   -1,   -1,  441,  441,
   -1,   -1,   -1,  357,   -1,   -1,   -1,   -1,   59,  363,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,  489,  490,   -1,  492,   -1,   -1,  381,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  288,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  302,   -1,    0,  521,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,  321,  418,   -1,  318,   -1,  320,  321,
  328,   -1,  324,  331,   -1,   -1,  328,  335,   -1,  331,
   -1,  333,   -1,  335,   -1,  343,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,
   -1,  359,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,  383,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,  410,   -1,
  418,  413,   -1,  517,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  458,   -1,  460,   -1,   -1,  429,   -1,   -1,
   -1,   -1,  440,  435,  436,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,  451,
  452,   -1,  454,  489,  490,   -1,  492,   -1,  460,   -1,
   -1,  262,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,    0,   -1,  516,  517,   -1,  318,    0,  521,
  321,   -1,   -1,   -1,   -1,  273,   -1,  328,   -1,  277,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,  291,   -1,   -1,   -1,  295,   -1,   -1,
  298,   -1,   -1,   41,   -1,   -1,   44,   -1,  359,   41,
   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,   -1,
   -1,   59,   -1,   -1,  272,   -1,   -1,   59,   60,   61,
   62,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
  288,  339,   -1,   -1,   -1,  396,   -1,  345,  346,  347,
   -1,  349,  350,  351,  302,  353,   -1,   -1,   -1,  307,
  308,   93,  413,  311,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,  373,  324,  375,    0,   -1,
  328,   -1,   -1,  331,  435,  333,   -1,  335,   -1,  440,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,    0,   -1,   -1,  406,   -1,
   -1,  359,  360,  411,  362,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,  423,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   59,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   41,  445,  396,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   59,  413,   61,   -1,   -1,   -1,
  521,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,  429,  480,  481,   -1,   -1,   -1,  435,  436,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,  497,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   59,   -1,   -1,   -1,   -1,
  272,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  288,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  302,   -1,    0,  521,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,  321,   -1,   -1,  318,   -1,  320,  321,
  328,   -1,  324,  331,   -1,   -1,  328,  335,   -1,  331,
   -1,  333,   -1,  335,   -1,  343,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,
   -1,  359,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,  376,  383,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,  396,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,  410,   -1,
   -1,  413,  277,  278,   -1,  280,    0,   -1,   -1,  284,
   -1,   -1,  287,   -1,   -1,   -1,  318,  429,   -1,  321,
   -1,   -1,  440,  435,  436,  300,  328,   -1,  440,  331,
   -1,  306,   -1,  335,  452,   -1,  454,   -1,  340,  451,
  452,  343,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,  383,   -1,   -1,  386,  387,   -1,  499,  500,  501,
  502,  503,   -1,  505,  396,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  378,  516,  517,   -1,   -1,   -1,  521,
   -1,  413,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,   -1,  328,  400,    0,  331,   -1,   -1,
   -1,  335,  407,  435,   -1,   -1,  340,   -1,  440,  343,
   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,  262,  359,  264,  265,   -1,   -1,
   -1,  436,   -1,   -1,  272,   -1,  441,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,  383,
  288,   -1,  386,  387,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,  396,   -1,  302,  470,   -1,   -1,   -1,  307,
  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,  413,
  318,   -1,  320,  321,   -1,   -1,  324,   91,   -1,  521,
  328,   -1,   -1,  331,    0,  333,   -1,  335,   -1,   -1,
   -1,  435,  340,  341,   -1,  343,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,  271,  386,  387,
   -1,   -1,   -1,   59,   -1,  279,   -1,   -1,  396,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,  413,  300,   -1,   -1,   -1,
   -1,   -1,  306,   -1,  308,   -1,   -1,  521,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,  436,   -1,
   -1,   41,  440,   -1,   44,  329,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   59,
   -1,   -1,  460,   -1,  348,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  357,   -1,   -1,   -1,   -1,   -1,  363,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  381,  262,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,  272,   -1,
  508,  509,  510,  511,  512,   -1,  280,   -1,  516,  517,
   -1,  285,   -1,  521,  288,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   58,   -1,   -1,   91,  340,  341,   -1,  343,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,  258,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,  271,   59,   -1,   -1,   -1,
   -1,   -1,  376,  279,  378,  379,  380,   -1,   -1,  383,
   40,   41,  386,  387,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,  517,  300,   -1,   -1,   -1,   -1,   59,
  306,   -1,  308,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,  329,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,  261,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,  357,   -1,  282,   -1,   -1,   -1,  363,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
  299,   -1,  301,   -1,   -1,  381,   -1,  306,  318,   -1,
   -1,  321,   -1,   -1,  313,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  327,   -1,
  340,   -1,   -1,  343,   -1,   -1,  262,   -1,   -1,  338,
   -1,   -1,  418,   -1,   -1,   -1,  272,  521,   -1,  359,
   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,
   -1,   -1,  288,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,    0,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  324,   -1,
   -1,  264,  328,  413,   -1,  331,  405,  333,   41,  335,
   -1,   44,   -1,  412,  340,  341,  415,  343,  417,  418,
   -1,   -1,   -1,   -1,   -1,  435,   59,   -1,   -1,   41,
  440,   -1,   44,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,  517,  452,  442,  454,   -1,   -1,   59,   -1,   -1,
  376,   -1,  378,  379,  380,  318,   -1,  383,  321,   -1,
  386,  387,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  396,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,  413,  318,   -1,
   -1,  321,  491,   -1,   -1,   -1,  359,   -1,  328,   -1,
   -1,  331,   -1,  429,    0,  335,   -1,  506,  507,  435,
  340,  521,   -1,  343,  440,    0,   -1,   -1,   -1,   -1,
  383,   -1,   -1,   -1,  387,   -1,  452,   -1,  454,  359,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  413,   -1,   -1,  383,   -1,   -1,   41,  387,   -1,   44,
   -1,   -1,   -1,   59,  460,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,  489,  490,  521,  492,  460,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
  440,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  452,   -1,  454,   -1,  489,  490,   -1,  492,
  460,   -1,   -1,    0,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
  262,   -1,   -1,  516,  517,   -1,    0,   -1,  521,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   41,  505,   -1,   44,  508,  509,
  510,  511,  512,   -1,   -1,  318,  516,  517,  321,   -1,
   -1,  521,   59,   -1,   -1,  328,   -1,   41,  331,   -1,
   44,   -1,  335,   -1,   -1,   -1,  318,  340,   -1,  321,
  343,   -1,   -1,   -1,   -1,   59,  328,   -1,   -1,  331,
    0,   -1,   -1,  335,   -1,   -1,  359,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,  396,   44,   -1,  262,   -1,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,  262,   -1,   59,
  413,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  318,  435,   -1,  321,   -1,  460,  440,   -1,
   -1,   -1,  328,  318,   -1,  331,  321,   -1,   -1,  335,
  452,   -1,  454,  328,  340,   -1,  331,  343,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,  489,  490,  343,  492,
   -1,   -1,    0,  359,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  359,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,  383,  521,   -1,
  386,  387,   -1,   -1,    0,  262,   -1,   -1,  383,   -1,
  396,  386,  387,   41,   -1,   -1,   44,   -1,   -1,  521,
   -1,  396,   59,   -1,   -1,   -1,   -1,  413,  262,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
  435,  318,   -1,   59,  321,  440,  452,   -1,  454,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,  452,  335,  454,
   -1,   -1,   -1,  340,  318,   -1,  343,  321,   -1,   -1,
   -1,   -1,  262,   -1,  328,   -1,   -1,  331,   -1,   -1,
    0,  335,  359,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,  396,
   -1,   41,   -1,   -1,   44,   -1,  521,   -1,  318,  383,
   -1,  321,  386,  387,   -1,   -1,  413,   -1,  328,   59,
   -1,  331,  396,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,  435,  413,
   -1,  287,   -1,  440,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   40,  262,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  521,  435,  262,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,  521,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   59,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,  359,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,  521,   -1,   -1,  460,  383,   -1,   -1,  386,  387,
   -1,   -1,  262,  359,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,  413,  492,  383,   -1,   -1,
  386,  387,   -1,  499,  500,  501,  502,  503,   -1,  505,
  396,   -1,  508,  509,  510,  511,  512,  435,   -1,   -1,
  516,  517,  440,   -1,   -1,   -1,   -1,  413,  318,   -1,
   -1,  321,   -1,  460,  452,   -1,  454,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  435,
  340,   -1,   -1,  343,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,  452,   -1,  454,  359,
  267,   -1,  499,  500,  501,  502,  503,   -1,  505,  276,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  289,  383,   -1,   -1,  386,  387,  295,  296,
   -1,   -1,   40,  521,   -1,   43,  396,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,
   58,   -1,   -1,  413,   -1,   63,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,  521,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  273,   -1,   -1,   -1,
  277,  378,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,
   58,  298,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  521,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,  339,   -1,   -1,   -1,  443,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  406,
   -1,  508,  509,  510,  511,  512,  513,  514,  515,  516,
  517,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,  267,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,  289,   -1,   -1,  292,   -1,   -1,  295,  296,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,  314,   -1,  508,  509,
  510,  511,  512,  480,  481,  323,  516,  517,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  497,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,   -1,
  378,   40,   -1,   42,   43,  303,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  352,  433,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,  287,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,  471,   -1,   -1,   63,   -1,   -1,  328,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  323,  505,   -1,  326,  508,  509,
  510,  511,  512,   -1,  443,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,  460,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,  366,   63,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  343,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,  368,   -1,   -1,   -1,   -1,   -1,  427,  276,
   -1,   -1,   -1,   -1,  433,   41,   42,   43,   -1,   45,
   -1,   47,  289,   -1,  443,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   60,   61,   62,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,  314,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,   -1,  326,
  426,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  354,  355,  508,
  509,  510,  511,  512,  460,   -1,   -1,  516,  517,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  267,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,  276,   -1,   -1,   40,   -1,  433,   43,   -1,   45,
   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,  314,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  354,
  355,  508,  509,  510,  511,  512,  272,   -1,   -1,  516,
  517,  366,   -1,   -1,  280,  370,   -1,   -1,   -1,  285,
   -1,   40,  288,  378,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  460,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,  333,   -1,   -1,
  489,  490,  427,  492,   -1,  341,   -1,   -1,  433,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  443,  508,
  509,  510,  511,  512,  360,   -1,  362,  516,  517,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,  443,
  276,  516,  517,  429,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  460,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,  323,  492,   -1,
  326,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  354,  355,
   -1,  260,    0,   -1,  263,   -1,   -1,   -1,  267,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   40,
   -1,   -1,   43,   41,   45,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   59,   63,   61,  323,  421,   -1,  326,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   91,   -1,  354,  355,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  328,  471,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,  260,
  258,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,  274,   -1,   -1,  277,
  278,  434,  280,   -1,   -1,  402,  284,   -1,  289,  287,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   40,   -1,  300,   43,   -1,   45,   -1,  460,  306,   -1,
   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,  438,  323,   63,   -1,  326,  443,   -1,   -1,   -1,
   -1,   -1,  449,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,  460,   -1,   -1,  499,  500,  501,  502,
  503,  468,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  366,   -1,   -1,   -1,  370,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
  378,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,
   -1,  260,   -1,  411,  263,   -1,   -1,   -1,  267,   -1,
  418,   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  436,   -1,
  289,   -1,  443,  441,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,
  471,   -1,  470,   -1,  323,   -1,   -1,  326,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  354,  355,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,   -1,
  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  425,   58,  427,   -1,
   -1,   -1,   63,  323,  433,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  271,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,  301,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  312,  313,   -1,  315,
   -1,   40,   -1,   -1,   43,   -1,   45,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,  332,   -1,   -1,   58,
   -1,  471,  338,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,  417,   -1,   -1,  295,  296,   -1,   -1,   -1,  460,
   40,   -1,   -1,   43,   -1,   45,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   58,   -1,
   -1,   -1,  323,   63,   -1,  326,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  460,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  354,  355,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,  366,  492,   -1,   -1,  370,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,  443,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  354,  355,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,   -1,
  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  425,   58,  427,   -1,
   -1,   -1,   63,  323,  433,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,  457,  458,
  459,  460,  461,  462,  354,  355,   -1,   -1,   60,   61,
   62,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,
  260,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,  323,  285,   -1,  326,  288,   -1,   -1,   -1,
   -1,  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  313,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,  327,   -1,   -1,
   -1,   -1,  324,   -1,   -1,  366,   -1,   40,  338,  370,
   43,  333,   45,   -1,   -1,   -1,   -1,   -1,   -1,  341,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,  407,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
  471,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
  460,  492,   -1,   -1,  495,  496,  267,  498,  499,  500,
  501,  502,  503,  504,  505,  276,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,  289,  489,
  490,  460,  492,   -1,  295,  296,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
  489,  490,  323,  492,   -1,  326,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,  276,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  417,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,  273,   -1,   58,   -1,  277,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,  471,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
  339,   -1,  276,  516,  517,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,
   -1,  480,  481,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,  276,   -1,   -1,   40,  497,  433,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,  303,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  354,  355,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,  293,   -1,  295,   -1,  471,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,  339,   -1,  276,  516,  517,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,  289,   -1,   -1,   -1,
  260,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  301,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,
   -1,   -1,   -1,  313,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,  423,   -1,   -1,   -1,   -1,
   -1,  267,   -1,  366,   -1,   -1,   -1,  370,  338,   -1,
  276,   -1,   -1,   40,   -1,   -1,   43,  445,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  497,
  433,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,  417,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  442,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
  460,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  489,
  490,  427,  492,   -1,   -1,   -1,   -1,  433,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  443,  508,  509,
  510,  511,  512,   -1,   -1,  451,  516,  517,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,
  516,  517,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,  460,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,  469,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,   -1,   -1,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  417,   -1,  499,  500,  501,  502,  503,  504,  505,  506,
  427,  508,  509,  510,  511,  512,  433,   -1,   -1,  516,
  517,  518,  519,  520,  521,   -1,  443,   -1,   -1,   -1,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,  295,   -1,  471,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,  339,   -1,  276,  516,
  517,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,
  423,   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,
   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  497,  433,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,  271,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,  516,  517,
   -1,   -1,   -1,  301,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,  332,   -1,   -1,   -1,   -1,   -1,
  338,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
  398,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,   -1,  354,  355,   -1,   -1,   -1,
   40,   -1,  460,   43,   -1,   45,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  518,  519,  520,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
  295,   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,  339,   -1,  276,  516,  517,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,  276,   -1,  423,   40,
   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,
  370,   -1,   -1,   -1,   -1,  295,  296,   58,   59,   -1,
  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,  497,  433,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  427,  508,  509,
  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  276,  516,  517,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  460,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,   -1,  499,  500,
  501,  502,  503,  504,  505,  506,  427,  508,  509,  510,
  511,  512,  433,   -1,   -1,  516,  517,  518,  519,  520,
  521,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,    0,   -1,   -1,
   -1,   -1,  354,  355,  276,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   40,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   41,  323,   -1,   44,  326,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,  427,   91,   -1,   -1,   -1,
   -1,  433,  354,  355,   -1,   -1,   -1,   -1,   91,   -1,
   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   91,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  258,  516,  517,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,  258,   -1,   -1,   -1,  274,
    0,   -1,  277,  278,   -1,  280,   59,  258,   61,  284,
   -1,  274,  287,   -1,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,  274,  287,  300,   -1,  278,   -1,  280,
   -1,  306,   -1,  284,  309,   -1,  287,  300,   91,   -1,
   -1,   41,   -1,  306,   44,   -1,  309,   -1,   -1,  300,
   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  400,   -1,  378,   -1,   -1,
   -1,   -1,  407,   -1,   -1,   -1,  411,  400,   -1,   -1,
   -1,   -1,   -1,  418,  407,   -1,   -1,   -1,  411,  400,
   -1,   -1,   -1,   -1,   -1,  418,  407,   -1,   -1,   -1,
  411,  436,   -1,   -1,   -1,   -1,  441,  418,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  466,   -1,  258,   -1,  470,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,
   -1,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
   -1,   -1,   -1,  306,   -1,  271,  309,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,
  280,   -1,   -1,   -1,  284,  301,   -1,  287,   -1,   -1,
  273,   -1,  308,   -1,  277,   -1,  312,  313,   -1,  315,
  300,  284,  318,   -1,   -1,   -1,  306,   -1,   -1,  309,
   -1,   -1,  295,   -1,   -1,  298,  332,   -1,   -1,   -1,
   -1,   -1,  338,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,  339,   -1,  411,   -1,
   -1,   -1,  345,  346,  347,  418,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,
  273,   -1,  398,  436,  277,   -1,   -1,   -1,  441,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  400,  417,  295,   -1,   -1,  298,   -1,  407,   -1,   -1,
   -1,  411,   -1,  466,   -1,   -1,  432,  470,  418,   -1,
   -1,   -1,   -1,  406,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  436,  453,   -1,   -1,
  423,  441,   -1,   -1,  460,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,  445,   -1,   -1,   -1,  466,   -1,   -1,   -1,
  470,   -1,   -1,  489,  490,   -1,  492,  460,   -1,   -1,
  373,   -1,  375,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  480,  481,   -1,
  516,  517,  518,  519,  520,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,  406,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
  423,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 521
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
"AS","ASC","AT","AVG","AUTO","BASENAME","BEFORE","BEGIN","BETWEEN","BLOB","BY",
"CACHE","CAST","CHARACTER","CHECK","CHECK_POINT_LEN","COLLATE","COMMA","COMMIT",
"COMMITTED","COMPUTED","CONCATENATE","CONDITIONAL","CONSTRAINT","CONTAINING",
"COUNT","CREATE","CSTRING","CURRENT","CURSOR","DATABASE","DATE","DB_KEY",
"KW_DEBUG","DECIMAL","DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO",
"DOMAIN","DROP","ELSE","END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE",
"EXISTS","EXIT","EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION",
"GDSCODE","GEQ","GENERATOR","GEN_ID","GRANT","GROUP","GROUP_COMMIT_WAIT","GTR",
"HAVING","IF","KW_IN","INACTIVE","INNER","INPUT_TYPE","INDEX","INSERT",
"INTEGER","INTO","IS","ISOLATION","JOIN","KEY","KW_CHAR","KW_DEC","KW_DOUBLE",
"KW_FILE","KW_FLOAT","KW_INT","KW_LONG","KW_NULL","KW_NUMERIC","KW_UPPER",
"KW_VALUE","LENGTH","LOGFILE","LPAREN","LEFT","LEQ","LEVEL","LIKE",
"LOG_BUF_SIZE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE",
"MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT",
"NOT_GTR","NOT_LSS","NUM_LOG_BUFS","OF","ON","ONLY","OPTION","OR","ORDER",
"OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","PAGE_SIZE","PARAMETER",
"PASSWORD","PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY","PRIVILEGES",
"PROCEDURE","PROTECTED","RAW_PARTITIONS","READ","REAL","REFERENCES","RESERVING",
"RETAIN","RETURNING_VALUES","RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK",
"SEGMENT","SELECT","SET","SHADOW","KW_SHARED","SINGULAR","KW_SIZE","SMALLINT",
"SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING","STATISTICS",
"SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION","TRIGGER",
"UNCOMMITTED","UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR","VARIABLE",
"VARYING","VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH","WORK","WRITE",
"FLOAT_NUMBER","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER","ACTION",
"ADMIN","CASCADE","FREE_IT","RESTRICT","ROLE","COLUMN","TYPE","EXTRACT","YEAR",
"MONTH","DAY","HOUR","MINUTE","SECOND","WEEKDAY","YEARDAY","TIME","TIMESTAMP",
"CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT",
"CURRENT_USER","CURRENT_ROLE","KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR",
"FIRST","SKIP","CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE",
"NULLIF","COALESCE","USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT",
"RELEASE","STATEMENT","LEAVE","INSERTING","UPDATING","DELETING","KW_INSERTING",
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH",
"ROWS",
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
"statement : recreate",
"statement : replace",
"statement : revoke",
"statement : rollback",
"statement : savepoint",
"statement : select",
"statement : set",
"statement : update",
"statement : KW_DEBUG signed_short_integer",
"grant : GRANT privileges ON prot_table_name TO user_grantee_list grant_option",
"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO user_grantee_list grant_option",
"grant : GRANT privileges ON prot_table_name TO grantee_list",
"grant : GRANT proc_privileges ON PROCEDURE simple_proc_name TO grantee_list",
"grant : GRANT role_name_list TO role_grantee_list role_admin_option",
"prot_table_name : simple_table_name",
"prot_table_name : TABLE simple_table_name",
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
"revoke : REVOKE rev_grant_option privileges ON prot_table_name FROM user_grantee_list",
"revoke : REVOKE rev_grant_option proc_privileges ON PROCEDURE simple_proc_name FROM user_grantee_list",
"revoke : REVOKE privileges ON prot_table_name FROM user_grantee_list",
"revoke : REVOKE proc_privileges ON PROCEDURE simple_proc_name FROM user_grantee_list",
"revoke : REVOKE privileges ON prot_table_name FROM grantee_list",
"revoke : REVOKE proc_privileges ON PROCEDURE simple_proc_name FROM grantee_list",
"revoke : REVOKE role_name_list FROM role_grantee_list",
"rev_grant_option : GRANT OPTION FOR",
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
"arg_desc : init_data_type udf_data_type",
"arg_desc : init_data_type udf_data_type BY KW_DESCRIPTOR",
"return_value1 : return_value",
"return_value1 : '(' return_value ')'",
"return_value : init_data_type udf_data_type",
"return_value : init_data_type udf_data_type FREE_IT",
"return_value : init_data_type udf_data_type BY KW_VALUE",
"return_value : init_data_type udf_data_type BY KW_DESCRIPTOR",
"return_value : PARAMETER pos_short_integer",
"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_subtype OUTPUT_TYPE blob_subtype ENTRY_POINT sql_string MODULE_NAME sql_string",
"create : CREATE create_clause",
"create_clause : EXCEPTION symbol_exception_name sql_string",
"create_clause : unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition",
"create_clause : PROCEDURE procedure_clause",
"create_clause : TABLE table_clause",
"create_clause : TRIGGER def_trigger_clause",
"create_clause : VIEW view_clause",
"create_clause : GENERATOR generator_clause",
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
"db_initial_option : PAGE_SIZE equals pos_short_integer",
"db_initial_option : LENGTH equals long_integer page_noise",
"db_initial_option : USER sql_string",
"db_initial_option : PASSWORD sql_string",
"db_initial_option : SET NAMES sql_string",
"db_rem_desc1 :",
"db_rem_desc1 : db_rem_desc",
"db_rem_desc : db_rem_option",
"db_rem_desc : db_rem_desc db_rem_option",
"db_rem_option : db_file",
"db_rem_option : db_log",
"db_rem_option : db_log_option",
"db_rem_option : DEFAULT CHARACTER SET symbol_character_set_name",
"db_rem_option : KW_DIFFERENCE KW_FILE sql_string",
"db_log_option : GROUP_COMMIT_WAIT equals long_integer",
"db_log_option : CHECK_POINT_LEN equals long_integer",
"db_log_option : NUM_LOG_BUFS equals pos_short_integer",
"db_log_option : LOG_BUF_SIZE equals unsigned_short_integer",
"db_log : db_default_log_spec",
"db_log : db_rem_log_spec",
"db_rem_log_spec : LOGFILE '(' logfiles ')' OVERFLOW logfile_desc",
"db_rem_log_spec : LOGFILE BASENAME logfile_desc",
"db_default_log_spec : LOGFILE",
"db_file : file1 sql_string file_desc1",
"logfiles : logfile_desc",
"logfiles : logfiles ',' logfile_desc",
"logfile_desc : logfile_name logfile_attrs",
"logfile_name : sql_string",
"logfile_attrs :",
"logfile_attrs : logfile_attrs logfile_attr",
"logfile_attr : KW_SIZE equals long_integer",
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
"input_parameters : '(' proc_parameters ')'",
"input_parameters :",
"output_parameters : RETURNS input_parameters",
"output_parameters :",
"proc_parameters : proc_parameter",
"proc_parameters : proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
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
"cursor_declaration_item : symbol_cursor_name CURSOR FOR '(' ordered_select_expr ')'",
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
"excp_statement : EXCEPTION symbol_exception_name ';'",
"excp_statement : EXCEPTION symbol_exception_name value ';'",
"raise_statement : EXCEPTION ';'",
"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs ';'",
"exec_sql : EXECUTE varstate value ';'",
"varstate : VARCHAR",
"varstate : STATEMENT",
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE varstate value INTO variable_list DO proc_block",
"exec_into : EXECUTE varstate value INTO variable_list ';'",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt ';'",
"event_argument_opt :",
"singleton_select : select INTO variable_list ';'",
"variable : ':' symbol_variable_name",
"proc_inputs : rhs_list",
"proc_inputs : '(' rhs_list ')'",
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
"view_clause : symbol_view_name column_parens_opt AS begin_string union_view check_opt end_string",
"rview_clause : symbol_view_name column_parens_opt AS begin_string union_view check_opt end_string",
"union_view : union_view_expr",
"union_view_expr : select_view_expr",
"union_view_expr : union_view_expr UNION select_view_expr",
"union_view_expr : union_view_expr UNION ALL select_view_expr",
"select_view_expr : SELECT distinct_clause select_list from_view_clause where_clause group_clause having_clause",
"from_view_clause : FROM from_view_list",
"from_view_list : view_table",
"from_view_list : from_view_list ',' view_table",
"view_table : joined_view_table",
"view_table : table_name",
"joined_view_table : view_table join_type JOIN view_table ON search_condition",
"joined_view_table : '(' joined_view_table ')'",
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
"keyword_or_column : WEEKDAY",
"keyword_or_column : YEARDAY",
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
"init_alter_db :",
"alter_db : db_alter_clause",
"alter_db : alter_db db_alter_clause",
"db_alter_clause : ADD db_file_list",
"db_alter_clause : DROP LOGFILE",
"db_alter_clause : SET db_log_option_list",
"db_alter_clause : ADD db_log",
"db_alter_clause : ADD KW_DIFFERENCE KW_FILE sql_string",
"db_alter_clause : DROP KW_DIFFERENCE KW_FILE",
"db_alter_clause : BEGIN BACKUP",
"db_alter_clause : END BACKUP",
"db_log_option_list : db_log_option",
"db_log_option_list : db_log_option_list ',' db_log_option",
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
"select : union_expr order_clause rows_clause for_update_clause lock_clause",
"union_expr : select_expr",
"union_expr : union_expr UNION select_expr",
"union_expr : union_expr UNION ALL select_expr",
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
"for_update_clause : FOR UPDATE for_update_list",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
"ordered_select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
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
"select_item : rhs",
"select_item : rhs as_noise symbol_item_alias_name",
"as_noise : AS",
"as_noise :",
"from_clause : FROM from_list",
"from_list : table_reference",
"from_list : from_list ',' table_reference",
"table_reference : joined_table",
"table_reference : table_proc",
"table_reference : derived_table",
"derived_table : '(' select ')' as_noise correlation_name derived_column_list",
"correlation_name : symbol_table_alias_name",
"correlation_name :",
"derived_column_list : '(' alias_list ')'",
"derived_column_list :",
"alias_list : symbol_item_alias_name",
"alias_list : alias_list ',' symbol_item_alias_name",
"joined_table : table_reference join_type JOIN table_reference ON search_condition",
"joined_table : '(' joined_table ')'",
"table_proc : symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name",
"table_proc : symbol_procedure_name table_proc_inputs",
"table_proc_inputs : '(' rhs_list ')'",
"table_proc_inputs :",
"table_name : simple_table_name",
"table_name : symbol_table_name symbol_table_alias_name",
"simple_table_name : symbol_table_name",
"join_type : INNER",
"join_type : LEFT",
"join_type : LEFT OUTER",
"join_type : RIGHT",
"join_type : RIGHT OUTER",
"join_type : FULL",
"join_type : FULL OUTER",
"join_type :",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' rhs_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt ordered_select_expr",
"delete : delete_searched",
"delete : delete_positioned",
"delete_searched : KW_DELETE FROM table_name where_clause",
"delete_positioned : KW_DELETE FROM table_name cursor_clause",
"cursor_clause : WHERE CURRENT OF symbol_cursor_name",
"update : update_searched",
"update : update_positioned",
"update_searched : UPDATE table_name SET assignments where_clause",
"update_positioned : UPDATE table_name SET assignments cursor_clause",
"assignments : assignment",
"assignments : assignments ',' assignment",
"assignment : update_column_name '=' rhs",
"rhs : value",
"rhs : null_value",
"rhs_list : rhs",
"rhs_list : rhs_list ',' rhs",
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
"predicate : between_predicate",
"predicate : like_predicate",
"predicate : in_predicate",
"predicate : null_predicate",
"predicate : quantified_predicate",
"predicate : exists_predicate",
"predicate : containing_predicate",
"predicate : starting_predicate",
"predicate : unique_predicate",
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
"exists_predicate : EXISTS '(' ordered_select_expr ')'",
"unique_predicate : SINGULAR '(' ordered_select_expr ')'",
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
"column_select : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
"column_singleton : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause rows_clause",
"value : column_name",
"value : array_element",
"value : function",
"value : u_constant",
"value : parameter",
"value : variable",
"value : cast_specification",
"value : case_expression",
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
"function : generate_value_function",
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
"generate_value_function : GEN_ID '(' symbol_generator_name ',' value ')'",
"numeric_value_function : extract_expression",
"extract_expression : EXTRACT '(' timestamp_part FROM value ')'",
"string_value_function : substring_function",
"string_value_function : KW_UPPER '(' value ')'",
"substring_function : SUBSTRING '(' value FROM value string_length_opt ')'",
"string_length_opt : FOR value",
"string_length_opt :",
"udf : symbol_UDF_name '(' value_list ')'",
"udf : symbol_UDF_name '(' ')'",
"cast_specification : CAST '(' rhs AS data_type_descriptor ')'",
"case_expression : case_abbreviation",
"case_expression : case_specification",
"case_abbreviation : NULLIF '(' value ',' value ')'",
"case_abbreviation : COALESCE '(' rhs ',' rhs_list ')'",
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
"case_result : rhs",
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
"non_reserved_word : USING",
"non_reserved_word : INSERTING",
"non_reserved_word : UPDATING",
"non_reserved_word : DELETING",
"non_reserved_word : BACKUP",
"non_reserved_word : KW_DIFFERENCE",
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
	for (const TOK *token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		DSQL_SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) dsql_sym;
		symbol->sym_string = (TEXT *) token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		STR str_ = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) str;
		str_->str_length = symbol->sym_length;
		strncpy((char*)str_->str_data, (char*)symbol->sym_string, symbol->sym_length);
		symbol->sym_object = (void *) str_;
		HSHD_insert(symbol);
	}
}


void LEX_string (
	TEXT	*string,
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


static void check_log_file_attrs (void)
{
/**********************************************
 *
 *	c h e c k _ l o g _ f i l e _ a t t r s
 *
 **********************************************
 *
 * Functional description
 *	Check if log file attributes are valid
 *
 *********************************************/

	if (lex.g_file->fil_partitions) {
		if (!lex.g_file->fil_length) {
			yyabandon (-261, isc_log_length_spec);
			/* Total length of a partitioned log must be specified */
		}
		
		if (PARTITION_SIZE (OneK * lex.g_file->fil_length, lex.g_file->fil_partitions) <
			(OneK*MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_partition_too_small);
			/* Log partition size too small */
		}
	}
	else {
		if ((lex.g_file->fil_length) && (lex.g_file->fil_length < MIN_LOG_LENGTH)) {
			yyabandon (-239, isc_log_too_small);   /* Log size too small */
		}
	}	 
}


static TEXT *lex_position (void)
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
static bool long_int(DSQL_NOD string,
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

	for (const char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return true;
}
#endif

static DSQL_FLD make_field (DSQL_NOD field_name)
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
	DSQL_FLD	field;
	STR	string;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (field_name == NULL)
	{
		field = FB_NEW_RPT(*tdsql->tsql_default, sizeof (INTERNAL_FIELD_NAME)) dsql_fld;
		strcpy (field->fld_name, (TEXT*) INTERNAL_FIELD_NAME);
		return field;
	}
	string = (STR) field_name->nod_arg [1];
	field = FB_NEW_RPT(*tdsql->tsql_default, strlen ((SCHAR*) string->str_data)) dsql_fld;
	strcpy (field->fld_name, (TEXT*) string->str_data);

	return field;
}


static FIL make_file (void)
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
	FIL	temp_file;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;
		   
	temp_file = FB_NEW(*tdsql->tsql_default) fil;

	return temp_file;
}


static DSQL_NOD make_list (DSQL_NOD node)
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
	DSQL_NOD	*ptr;
	DLLS	stack, temp;
	USHORT	l;
	DSQL_NOD	old;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	if (!node)
		return node;

	stack = 0;
	stack_nodes (node, &stack);
	for (l = 0, temp = stack; temp; temp = temp->lls_next)
		l++;

	old  = node;
	node = FB_NEW_RPT(*tdsql->tsql_default, l) dsql_nod;
	node->nod_count = l;
	node->nod_type  = nod_list;
	node->nod_flags = old->nod_flags;
	ptr = node->nod_arg + node->nod_count;

	while (stack)
		*--ptr = (DSQL_NOD) LLS_POP (&stack);

	return node;
}


static DSQL_NOD make_parameter (void)
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
	DSQL_NOD	node;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = 1;
	node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

	return node;
}


static DSQL_NOD make_node (NOD_TYPE	type,
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, DSQL_NOD);

	return node;
}


static DSQL_NOD make_flag_node (NOD_TYPE	type,
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
	DSQL_NOD	node, *p;
	va_list	ptr;
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	p = node->nod_arg;
	VA_START (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, DSQL_NOD);

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
static bool short_int(DSQL_NOD string,
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

	if (((STR) string)->str_length > 5) {
		return false;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
		}
	}

	/* there are 5 or fewer digits, it's value may still be greater
	 * than 32767... */

	SCHAR buf[10];	
	buf [0] = ((STR) string)->str_data[0];
	buf [1] = ((STR) string)->str_data[1];
	buf [2] = ((STR) string)->str_data[2];
	buf [3] = ((STR) string)->str_data[3];
	buf [4] = ((STR) string)->str_data[4];
	buf [5] = '\0';

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

static void stack_nodes (DSQL_NOD	node,
						 DLLS		*stack)
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
	DSQL_NOD	*ptr, *end;
	DSQL_NOD	 curr_node, next_node, start_chain, end_chain, save_link;

	if (node->nod_type != nod_list)
		{
		LLS_PUSH (node, stack);
		return;
		}

	/* To take care of cases where long lists of nodes are in a chain
	   of list nodes with exactly one entry, this algorithm will look
	   for a pattern of repeated list nodes with two entries, the first
	   being a list node and the second being a non-list node.   Such
	   a list will be reverse linked, and then re-reversed, stacking the
	   non-list nodes in the process.   The purpose of this is to avoid
	   massive recursion of this function. */

	start_chain = node;
	end_chain = NULL;
	curr_node = node;
	next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
		{

		/* pattern was found so reverse the links and go to next node */

		save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
		}

	/* see if any chain was found */

	if ( end_chain)
		{

		/* first, handle the rest of the nodes */
		/* note that next_node still points to the first non-pattern node */

		stack_nodes( next_node, stack);

		/* stack the non-list nodes and reverse the chain on the way back */
		
		curr_node = end_chain;
		while (true)
			{
			LLS_PUSH( curr_node->nod_arg[1], stack);
			if ( curr_node == start_chain)
				break;
			save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
			}
		return;
		}

	for (ptr = node->nod_arg, end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

inline static int yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
{
	int temp = lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
}

int LexerState::yylex (
	USHORT	client_dialect,
	USHORT	db_dialect,
	USHORT	parser_version,
	BOOLEAN	*stmt_ambiguous)
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
	char* p;
	char* buffer;
	char* buffer_end;
	char* new_buffer;
	DSQL_SYM	sym;
	SSHORT	c;
	USHORT	buffer_len;

	STR delimited_id_str;

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

		tok_class = classes [c];

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
		p = string;
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
		if (ptr >= end)
			return -1;
		check_copy_incr(p, UPPER7(*ptr), string);
		}
		
		check_bound(p, string);
		*p = 0;

		/* make a string value to hold the name, the name 
		 * is resolved in pass1_constant */

		yylval = (DSQL_NOD) (MAKE_string(string, p - string))->str_data;

		return INTRODUCER;
		}

	/* parse a quoted string, being sure to look for double quotes */

	if (tok_class & CHR_QUOTE)
		{
		buffer = string;
		buffer_len = sizeof (string);
		buffer_end = buffer + buffer_len - 1;
		for (p = buffer; ; p++)
		{
		if (ptr >= end)
			{
			if (buffer != string)
				gds__free (buffer);
			return -1;
			}
		/* *ptr is quote - if next != quote we're at the end */
		if ((*ptr == c) && ((++ptr == end) || (*ptr != c)))
			break;
			if (p > buffer_end)
			{
			new_buffer = (char*)gds__alloc (2 * buffer_len);
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
		*stmt_ambiguous = TRUE; /* string delimited by double quotes could be
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
			yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
			delimited_id_str = (STR) yylval;
			delimited_id_str->str_flags |= STR_delimited_id;
			if (buffer != string)
			gds__free (buffer);
			return SYMBOL;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(buffer, p - buffer);
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
		((c == '.') && (ptr < end) && (classes [*ptr] & CHR_DIGIT)))
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
			if (have_exp_digit && (! (classes [c]  & CHR_DIGIT)))
				/* First non-digit after exponent and digit terminates
				 the token. */
				break;
			else if (have_exp_sign && (! (classes [c]  & CHR_DIGIT)))
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
				else if ( classes [c]  & CHR_DIGIT )
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
			else if (classes [c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
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
				yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);
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
				yylval = (DSQL_NOD) (ULONG) number;
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
							   gds_arg_string,
							   ERR_string( last_token,
								   ptr - last_token ),
							   gds_arg_end );
					ERRD_post_warning( isc_dsql_warning_number_ambiguous1,
							   gds_arg_end );
					}

				yylval = (DSQL_NOD) MAKE_string(last_token, ptr - last_token);

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
		p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER (*ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		sym = HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
				(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
				 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
				 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
			{			
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
				lex = savedState;
				if (nextToken==OR || nextToken==AND) {
					switch(sym->sym_keyword) {
					case INSERTING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (DSQL_NOD) sym->sym_object;
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
			if ((sym->sym_keyword == FIRST && !first_detection) || sym->sym_keyword == SKIP) {
				if (prev_keyword == SELECT || limit_clause) {
					LexerState savedState = lex;
					int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (nextToken != NUMBER && nextToken != '?' && nextToken != '(') {
						yylval = (DSQL_NOD) MAKE_string(string, p - string);
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return SYMBOL;
					} else {
						yylval = (DSQL_NOD) sym->sym_object;
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			} else {
				yylval = (DSQL_NOD) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
				return sym->sym_keyword;
			}
		}
		yylval = (DSQL_NOD) MAKE_string(string, p - string);
		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;
		return SYMBOL;
	}

	/* Must be punctuation -- test for double character punctuation */

	if (last_token + 1 < end)
	{
		sym = HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
		(prev_keyword=='(' || prev_keyword==NOT || prev_keyword==AND || prev_keyword==OR ||
		 prev_keyword==ON || prev_keyword==HAVING || prev_keyword==WHERE || prev_keyword==WHEN) ) 
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
		} while (nextToken == '(');
		DSQL_NOD temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect,db_dialect,
						  parser_version,stmt_ambiguous) == ')')
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
					(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
					 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
					 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
				{			
					savedState = lex;
					int token = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (token==OR || token==AND) {
						switch(nextToken) {
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
		} else {
			/* Restore status quo. */
			lex = savedState;
		}
	}

	/* Single character punctuation are simply passed on */

	return c;
}


static void yyerror (
	TEXT	*error_string)
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
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
		0);
	else
	{
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_dsql_token_unk_err, 
		gds_arg_number, (SLONG) lex.lines, 
		gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
		gds_arg_gds, gds_random, 
		gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
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

	ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) sql_code, 
		gds_arg_gds, error_symbol, 0);
}
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
dsql_yyparse(USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous)
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
case 21:
{ prepare_console_debug ((int) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 22:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 28:
{ yyval = yyvsp[0]; }
break;
case 29:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 30:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 31:
{ yyval = make_list (yyvsp[0]); }
break;
case 33:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 34:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 35:
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 36:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 37:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 38:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 39:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 40:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 41:
{ yyval = 0; }
break;
case 42:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 43:
{ yyval = 0; }
break;
case 44:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 45:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
break;
case 46:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 47:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 48:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 49:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 50:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 51:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
break;
case 52:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-2]; }
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
							  yyvsp[-1], NULL); }
break;
case 86:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 88:
{ yyval = yyvsp[-1]; }
break;
case 89:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((STR) FUN_reference, CONSTANT_SLONG));}
break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((STR) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((STR) FUN_value, CONSTANT_SLONG));}
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG));}
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
				NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
break;
case 94:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 95:
{ yyval = yyvsp[0]; }
break;
case 96:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 97:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 98:
{ yyval = yyvsp[0]; }
break;
case 99:
{ yyval = yyvsp[0]; }
break;
case 100:
{ yyval = yyvsp[0]; }
break;
case 101:
{ yyval = yyvsp[0]; }
break;
case 102:
{ yyval = yyvsp[0]; }
break;
case 103:
{ yyval = yyvsp[0]; }
break;
case 104:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 115:
{ yyval = NULL; }
break;
case 116:
{ yyval = make_list (yyvsp[0]); }
break;
case 118:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 119:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
				 yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 120:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 121:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 122:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 123:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 124:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 125:
{ yyval = (DSQL_NOD) 0;}
break;
case 126:
{ yyval = yyvsp[-1]; }
break;
case 127:
{ yyval = NULL; }
break;
case 130:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 131:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
										  yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 132:
{ yyval = NULL; }
break;
case 133:
{ yyval = NULL; }
break;
case 134:
{ yyval = yyvsp[0]; }
break;
case 135:
{ yyval = NULL; }
break;
case 136:
{ yyval = NULL; }
break;
case 139:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 140:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 143:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 144:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 145:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 146:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 147:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 150:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) yyvsp[0]; }
break;
case 151:
{yyval = NULL;}
break;
case 154:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 155:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 156:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 157:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 158:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 159:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 160:
{yyval = NULL;}
break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 168:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
break;
case 169:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
break;
case 170:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
break;
case 171:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
break;
case 172:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
break;
case 173:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 174:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 176:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 177:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
break;
case 178:
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 181:
{ 
				 check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
												(DSQL_NOD) lex.g_file); }
break;
case 182:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
break;
case 185:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
break;
case 186:
{ lex.g_file  = make_file ();}
break;
case 191:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
break;
case 192:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
break;
case 199:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 200:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 201:
{ yyval = yyvsp[0]; }
break;
case 202:
{ yyval = yyvsp[0]; }
break;
case 203:
{ yyval = NULL; }
break;
case 205:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 210:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 211:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 214:
{ yyval = NULL; }
break;
case 215:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
											yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 216:
{ yyval = yyvsp[0]; }
break;
case 217:
{ yyval = NULL; }
break;
case 218:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 219:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
break;
case 220:
{ yyval = yyvsp[-1]; }
break;
case 221:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 222:
{ yyval = yyvsp[0]; }
break;
case 223:
{ yyval = NULL; }
break;
case 227:
{ yyval = yyvsp[0]; }
break;
case 228:
{ yyval = yyvsp[0]; }
break;
case 229:
{ yyval = yyvsp[0]; }
break;
case 230:
{ yyval = NULL; }
break;
case 233:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 234:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 235:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 236:
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
break;
case 240:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 241:
{ yyval = yyvsp[0]; }
break;
case 242:
{ yyval = NULL ;}
break;
case 247:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 248:
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
					 yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 250:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 251:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 252:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 253:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 256:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 257:
{ yyval = NULL;}
break;
case 258:
{ yyval = yyvsp[0];}
break;
case 259:
{ yyval = yyvsp[0];}
break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 264:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 265:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 266:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 267:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 268:
{ yyval = make_list (yyvsp[-1]); }
break;
case 269:
{ yyval = NULL; }
break;
case 270:
{ yyval = yyvsp[0]; }
break;
case 271:
{ yyval = NULL; }
break;
case 273:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 274:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 275:
{ yyval = make_list (yyvsp[0]); }
break;
case 276:
{ yyval = NULL; }
break;
case 278:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 279:
{ yyval = yyvsp[-1]; }
break;
case 282:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 283:
{ yyval = NULL; }
break;
case 284:
{ yyval = NULL; }
break;
case 285:
{ yyval = yyvsp[0]; }
break;
case 286:
{ yyval = yyvsp[0]; }
break;
case 287:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 290:
{ yyval = yyvsp[-1]; }
break;
case 291:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 292:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 293:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 295:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 311:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 312:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 315:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 316:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 317:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 318:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 319:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 322:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 323:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 325:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 327:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 328:
{ yyval = NULL; }
break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 330:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 331:
{ yyval = make_list (yyvsp[0]); }
break;
case 332:
{ yyval = make_list (yyvsp[-1]); }
break;
case 333:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_list (yyvsp[0]); }
break;
case 335:
{ yyval = make_list (yyvsp[-1]); }
break;
case 336:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 342:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 344:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 345:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 346:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 347:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 348:
{ yyval = NULL; }
break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 361:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 362:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 363:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 364:
{ yyval = NULL; }
break;
case 365:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
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
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL, NULL); }
break;
case 369:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 371:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 373:
{ yyval = make_list (yyvsp[0]); }
break;
case 375:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 378:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 379:
{ yyval = yyvsp[-1]; }
break;
case 380:
{ lex.beginning = lex_position(); }
break;
case 381:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 382:
{ lex.beginning = lex.last_token; }
break;
case 383:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 384:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 385:
{ yyval = 0; }
break;
case 386:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 387:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 388:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 390:
{ yyval = NULL; }
break;
case 391:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 392:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 407:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 408:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 409:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 410:
{ yyval = NULL; }
break;
case 411:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 412:
{ yyval = yyvsp[0]; }
break;
case 413:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 418:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 419:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 423:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 428:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 429:
{ yyval = yyvsp[0]; }
break;
case 430:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 432:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 433:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 434:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 435:
{ yyval = yyvsp[0]; }
break;
case 436:
{ yyval = yyvsp[0]; }
break;
case 437:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 438:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 439:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 440:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 468:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = NULL; }
break;
case 471:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 472:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 473:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 476:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 477:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 478:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 480:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 481:
{ yyval = yyvsp[0]; }
break;
case 482:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 483:
{ yyval = yyvsp[0]; }
break;
case 484:
{ yyval = yyvsp[0]; }
break;
case 485:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 487:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 488:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 490:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 493:
{ yyval = NULL; }
break;
case 495:
{ yyval = NULL; }
break;
case 496:
{ yyval = yyvsp[0]; }
break;
case 497:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 501:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 503:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 504:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 505:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 506:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 507:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 508:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 513:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 514:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 516:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 517:
{ if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 518:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 524:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "BIGINT",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "BIGINT",
					0);
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 527:
{ 
			*stmt_ambiguous = TRUE;
			if (client_dialect <= SQL_DIALECT_V5)
				{
				/* Post warning saying that DATE is equivalent to TIMESTAMP */
					ERRD_post_warning (isc_sqlwarn, gds_arg_number, (SLONG) 301, 
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
case 528:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 536:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 537:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 538:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 539:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 540:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 541:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 557:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 558:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 559:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 560:
{		 
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );

					}
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else 
				if ((SLONG) yyvsp[-1] < 5)
					{
					lex.g_field->fld_dtype = dtype_short; 
					lex.g_field->fld_length = sizeof (SSHORT); 
					}
				else
					{
					lex.g_field->fld_dtype = dtype_long; 
					lex.g_field->fld_length = sizeof (SLONG); 
					}
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 561:
{ 
			if ( ((SLONG) yyvsp[-3] < 1) || ((SLONG) yyvsp[-3] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision should be between 1 and 18 */ 
			if (((SLONG) yyvsp[-1] > (SLONG) yyvsp[-3]) || ((SLONG) yyvsp[-1] < 0))
				yyabandon (-842, isc_scale_nogt);
				/* Scale must be between 0 and precision */
			if ((SLONG) yyvsp[-3] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
					ERRD_post (gds_sqlerr,
					   gds_arg_number, (SLONG) -817,
					   gds_arg_gds,
					   isc_ddl_not_allowed_by_db_sql_dial,
					   gds_arg_number, (SLONG) db_dialect,
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
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous1,
					gds_arg_end );
					ERRD_post_warning (
					isc_dsql_warn_precision_ambiguous2,
					gds_arg_end );
				  }
				  /* client_dialect >= SQL_DIALECT_V6 */
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
					}
				}
			else
				{
				if ((SLONG) yyvsp[-3] < 5)
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
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT)(SLONG) yyvsp[-1];
			}
break;
case 564:
{ 
			if ((SLONG) yyvsp[0] > 7)
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
case 565:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 566:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 567:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 568:
{ yyval = yyvsp[-1]; }
break;
case 569:
{ yyval = 0; }
break;
case 573:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 574:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 575:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 579:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 580:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 581:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 582:
{ yyval = 0; }
break;
case 583:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 586:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 587:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 590:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 591:
{ yyval = NULL; }
break;
case 593:
{ yyval = NULL; }
break;
case 594:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 596:
{ yyval = NULL; }
break;
case 598:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 603:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 604:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 605:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 606:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 607:
{ yyval = yyvsp[0];}
break;
case 609:
{ yyval = yyvsp[0];}
break;
case 610:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 613:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 614:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 615:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 616:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 617:
{ yyval = 0; }
break;
case 618:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 619:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 620:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 621:
{ yyval = (DSQL_NOD) 0; }
break;
case 622:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 623:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 625:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 626:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 627:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 628:
{ yyval = 0; }
break;
case 630:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 631:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 632:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 633:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 634:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 635:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 636:
{ yyval = make_list (yyvsp[0]); }
break;
case 637:
{ yyval = 0; }
break;
case 639:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 640:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 641:
{ yyval = 0; }
break;
case 642:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 643:
{ yyval = 0; }
break;
case 644:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 645:
{ yyval = 0; }
break;
case 646:
{ yyval = yyvsp[-1]; }
break;
case 647:
{ yyval = 0; }
break;
case 648:
{ yyval = make_node (nod_rows, (int) e_rows_count, NULL, yyvsp[0]); }
break;
case 649:
{ yyval = make_node (nod_rows, (int) e_rows_count,
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)),
				make_node (nod_add, 2,
					make_node (nod_subtract, 2, yyvsp[0], yyvsp[-2]),
					MAKE_constant ((STR) 1, CONSTANT_SLONG))); }
break;
case 650:
{ yyval = NULL; }
break;
case 651:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 652:
{ yyval = 0; }
break;
case 653:
{ yyval = yyvsp[0]; }
break;
case 654:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 656:
{ yyval = 0; }
break;
case 657:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL, NULL); }
break;
case 658:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-9], yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 659:
{ lex.limit_clause = true; }
break;
case 660:
{ lex.limit_clause = false; }
break;
case 661:
{ lex.first_detection = true; }
break;
case 662:
{ lex.first_detection = false; }
break;
case 663:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 664:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 665:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 666:
{ yyval = 0; }
break;
case 667:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 668:
{ yyval = yyvsp[-2]; }
break;
case 669:
{ yyval = yyvsp[-1]; }
break;
case 670:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 671:
{ yyval = yyvsp[-1]; }
break;
case 672:
{ yyval = yyvsp[0]; }
break;
case 673:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 674:
{ yyval = 0; }
break;
case 675:
{ yyval = make_list (yyvsp[0]); }
break;
case 676:
{ yyval = 0; }
break;
case 678:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 680:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 683:
{ yyval = make_list (yyvsp[0]); }
break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 689:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 691:
{ yyval = NULL; }
break;
case 692:
{ yyval = make_list (yyvsp[-1]); }
break;
case 693:
{ yyval = NULL; }
break;
case 695:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 696:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 697:
{ yyval = yyvsp[-1]; }
break;
case 698:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 699:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 700:
{ yyval = make_list (yyvsp[-1]); }
break;
case 701:
{ yyval = NULL; }
break;
case 703:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 704:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 705:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 706:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 707:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 708:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 709:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 710:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 711:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 712:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 713:
{ yyval = make_list (yyvsp[0]); }
break;
case 714:
{ yyval = 0; }
break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 718:
{ yyval = yyvsp[0]; }
break;
case 719:
{ yyval = 0; }
break;
case 720:
{ yyval = yyvsp[0]; }
break;
case 721:
{ yyval = 0; }
break;
case 722:
{ yyval = yyvsp[0]; }
break;
case 723:
{ yyval = 0; }
break;
case 724:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 725:
{ yyval = 0; }
break;
case 726:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 727:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 728:
{ yyval = 0; }
break;
case 729:
{ yyval = 0; }
break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 737:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 738:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 741:
{ yyval = make_list (yyvsp[-1]); }
break;
case 742:
{ yyval = 0; }
break;
case 743:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 744:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 747:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 748:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 749:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 752:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 753:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 756:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 768:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 769:
{ yyval = yyvsp[0]; }
break;
case 771:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 774:
{ yyval = NULL; }
break;
case 775:
{ yyval = make_list (yyvsp[-1]); }
break;
case 777:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = NULL; }
break;
case 780:
{ yyval = make_list (yyvsp[-1]); }
break;
case 782:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 786:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = yyvsp[-1]; }
break;
case 802:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 834:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 841:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 843:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 845:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 847:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 848:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 849:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 850:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 851:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 853:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 854:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 855:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 856:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 857:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 859:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 860:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 861:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 862:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 864:
{ yyval = make_list (yyvsp[-1]); }
break;
case 865:
{ yyval = yyvsp[-1]; }
break;
case 866:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 867:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-9], yyvsp[-8], make_list (yyvsp[-7]), yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
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
{ yyval = make_node (nod_collate, (int) e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
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
case 893:
{ yyval = yyvsp[0]; }
break;
case 894:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = make_node (nod_current_date, 0, NULL);
			}
break;
case 895:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 896:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 897:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 899:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 901:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 902:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 903:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 904:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 905:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 906:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 908:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 909:
{ 
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "DATE",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "DATE",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DATE);
			}
break;
case 910:
{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_dialect_datatype_unsupport,
					gds_arg_number, client_dialect,
					gds_arg_string, "TIME",
					0);
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104, 
					gds_arg_gds, isc_sql_db_dialect_dtype_unsupport,
					gds_arg_number, db_dialect,
					gds_arg_string, "TIME",
					0);
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIME); 
			}
break;
case 911:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 912:
{ yyval = make_parameter (); }
break;
case 913:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 914:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 915:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 916:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 920:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 921:
{ yyval = yyvsp[0]; }
break;
case 922:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 924:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 925:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 926:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 927:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 928:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 930:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 931:
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
case 947:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 949:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 952:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 953:
{ yyval = yyvsp[0]; }
break;
case 954:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 955:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 956:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 957:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 960:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
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
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 976:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 977:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 979:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 980:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 981:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 982:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 985:
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
