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
 *            clashes with normal DEBUG macro.
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
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
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
 *                           with table alias. Also removed group_by_function and ordinal.
 * 2003.08.14 Arno Brinkman: Added support for derived tables.
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
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
#include "../dsql/node.h"
#include "../dsql/sym.h"
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

/* Can't include ../jrd/err_proto.h here because it pulls jrd.h. */
#if !defined(JRD_ERR_PROTO_H)
extern "C" TEXT *DLL_EXPORT ERR_string(const TEXT*, int);
#endif

ASSERT_FILENAME

static void	yyerror (TEXT *);

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

#define SHRT_POS_MAX		32767
#define SHRT_UNSIGNED_MAX	65535
#define SHRT_NEG_MAX		32768
#define LONG_POS_MAX		2147483647
#define POSITIVE		0
#define NEGATIVE		1
#define UNSIGNED		2

#define MIN_CACHE_BUFFERS       250
#define DEF_CACHE_BUFFERS       1000

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE		2048
#define YYMAXDEPTH		2048

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

#define TRIGGER_TYPE_SUFFIX(slot1, slot2, slot3) \
	((slot1 << 1) | (slot2 << 3) | (slot3 << 5))

extern "C" {

#ifndef SHLIB_DEFS
DSQL_NOD		DSQL_parse;
#else
extern DSQL_NOD	DSQL_parse;
#endif

}	/* extern "C"*/
static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
    {\
    if ((to - string) >= MAX_TOKEN_LEN)        \
	yyabandon (-104, isc_token_too_long); \
    }
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	long_int (DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN	short_int (DSQL_NOD, SLONG *, SSHORT);
#endif
static void	stack_nodes (DSQL_NOD, DLLS *);
inline static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, ISC_STATUS);
static void	check_log_file_attrs (void);

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
static struct LexerState lex;

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
  173,  177,  177,  178,  174,  174,  179,  179,  180,  181,
  181,  182,  182,  183,  183,  175,  185,  185,  185,  186,
  186,  184,  184,  184,  184,  184,  184,  184,  184,  184,
  184,  184,  184,  184,  184,  184,  184,  184,  184,  189,
  189,  190,  191,  192,  203,  203,  193,  198,  199,  194,
  194,  195,  206,  196,  207,  201,  201,  201,  202,  202,
  202,  204,  204,  204,  204,  197,  200,  200,  205,  205,
  187,  187,  212,  213,  213,  214,  214,  214,  214,   11,
   71,   80,  216,  219,  219,  219,  220,  223,  227,  227,
  228,  228,  228,  229,  229,  147,  218,   87,   89,  217,
  217,   70,   83,  236,  236,  236,  237,  240,  240,  241,
  241,  241,  241,  241,  241,  241,  241,  241,  241,  241,
  241,  241,  241,  241,  238,  238,  239,    2,  243,  243,
  243,  243,  243,  243,  243,  251,  252,  249,  249,  253,
  253,  253,  253,  253,  253,  253,  244,  244,  254,  254,
  254,  254,  254,  254,  254,  248,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  256,
  256,  258,  258,  257,  255,  255,  255,  250,  250,  246,
  247,  247,  261,  261,  261,  261,  261,  261,  261,  261,
  262,  262,  245,  263,  263,  264,  264,    8,  265,  265,
  265,  265,  265,  265,  265,  265,  265,  265,  265,  265,
  101,  101,  144,  144,  268,  268,  271,  271,  273,  273,
   53,   53,  270,  270,  270,  270,  270,  270,  270,  270,
  270,  278,  278,  269,  269,  269,  269,  279,  279,   61,
   61,   61,   55,   55,  275,  275,  275,  272,  272,  272,
  283,  283,  283,  282,  282,  281,  281,  281,  276,  276,
  284,  284,  284,  285,  285,  277,  277,  277,  277,  286,
  286,   18,   18,   18,  288,  288,  288,   16,   16,   16,
  290,  291,  294,  294,  292,  296,  296,    4,   15,  295,
  295,  297,  297,  298,  298,  287,  299,  299,  300,  300,
  301,  301,  301,  301,  302,  302,  303,  303,  304,  304,
  306,  306,  306,  307,  307,  307,  308,  308,  308,  305,
  310,  310,  310,  311,  311,  309,  309,  312,  314,  314,
  313,  313,  289,   17,  231,  231,  231,  315,  315,  319,
  319,  320,   65,   65,   65,  322,  322,  321,  321,  316,
  316,  325,  325,  317,  317,  318,  329,  330,  331,  323,
  324,  326,  326,  326,  326,  332,  332,  332,  333,  333,
  333,  221,  221,  222,  222,  336,  336,  337,  337,  232,
  232,  327,  340,  340,  341,  341,  341,  234,  234,  344,
  344,  342,  342,  343,  343,  345,  345,  209,  209,  346,
  346,  230,  230,   31,  235,  235,  235,  235,  235,  235,
  235,  235,  225,  225,  347,  347,  348,  226,  226,  224,
  224,  328,  328,  349,  350,  350,  350,  350,  350,  351,
  351,  352,  352,  353,  353,  354,  354,  354,  355,  355,
   10,   10,  357,  357,    7,    7,  358,  359,  360,   19,
   19,  361,  362,  363,  363,  188,  338,  338,    3,    3,
  365,  365,  365,  367,  367,   61,  366,  366,  368,  368,
   34,   34,   85,   84,   84,  356,  356,  369,  370,  370,
  210,  210,  210,  146,  364,  364,  111,  111,  111,  111,
  111,  374,  374,  374,  374,  374,  374,  373,  373,  373,
  376,  376,  376,  376,  376,  376,  376,  376,  376,  376,
  377,  377,  377,  377,  377,  377,  377,  377,  382,  382,
  382,  382,  382,  382,  382,  382,  382,  382,  382,  382,
  382,  382,  382,  382,  388,  388,  378,  378,  379,  379,
  379,  379,  380,  380,  384,  384,  385,  385,  385,  385,
  383,  386,  381,  381,  372,  372,  372,  375,  375,  375,
  389,  389,  390,  387,  392,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,  156,  156,  156,  393,  391,  391,  151,  151,  399,
  399,  399,  399,  399,  395,  395,  395,  395,  395,  334,
  152,  152,  153,  154,  154,  154,  154,  154,   51,   51,
   20,   20,  242,  400,   54,  123,  274,  274,   95,  394,
  394,  394,  394,  401,  401,  401,  401,  401,  401,  401,
  401,  401,  401,  401,  402,  403,  404,  404,  404,  398,
  398,  396,  397,  397,  406,  406,  407,  407,  408,  408,
  411,  411,  409,  409,  414,  414,  413,  410,  412,  405,
  405,  405,  405,  405,  405,  405,  405,  335,  335,  155,
   48,  280,  122,  148,  371,  165,  211,  267,   63,   60,
  215,  112,   66,  339,   35,   41,  233,  266,   39,   42,
  208,   40,  293,  260,  260,  415,  415,  415,  415,  415,
  415,  415,  415,  415,  415,  415,  415,  415,  415,  415,
  415,
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
    0,    1,    3,    2,    1,    0,    1,    2,    6,    1,
    0,    2,    1,    1,    1,    3,    1,    2,    0,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    2,
    1,    1,    2,    1,    1,    1,    2,    2,    1,    3,
    4,    2,    6,    4,    1,    1,    7,    8,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    6,    2,    2,    3,    0,
    1,    2,    4,    1,    3,    2,    2,    2,    1,    4,
    7,    7,    1,    1,    3,    4,    7,    2,    1,    3,
    1,    1,    6,    6,    3,    0,    0,    0,    0,    3,
    0,    9,    9,    1,    1,    0,    2,    1,    1,    1,
    1,    1,    3,    3,    3,    3,    3,    3,    5,    5,
    5,    5,    5,    5,    2,    0,    4,    2,    3,    3,
    2,    2,    3,    3,    2,    3,    6,    1,    2,    4,
    3,    2,    2,    2,    2,    3,    1,    3,    3,    3,
    2,    2,    4,    4,    5,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    1,    1,    1,    0,    2,    2,    0,
    1,    2,    2,    2,    2,    2,    4,    3,    2,    2,
    1,    3,    7,    1,    0,    1,    0,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    3,    2,    2,    2,
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
    1,    3,    4,    4,    1,    3,    4,    3,    0,    1,
    3,    3,    1,    1,    0,    1,    1,    4,    0,    3,
    0,    2,    0,    2,    0,    9,   10,    0,    0,    0,
    0,    3,    2,    1,    0,    3,    5,    3,    2,    5,
    2,    1,    1,    1,    1,    1,    3,    1,    3,    1,
    0,    2,    1,    3,    1,    1,    6,    3,    0,    1,
    3,    6,    3,    4,    2,    3,    0,    1,    3,    1,
    1,    1,    2,    1,    1,    1,    2,    1,    2,    1,
    2,    0,    3,    0,    1,    3,    1,    2,    0,    2,
    0,    2,    0,    4,    1,    2,    1,    1,    0,    1,
    3,    2,    1,    1,    2,    1,    4,    2,    1,    3,
    8,    5,    1,    3,    1,    1,    4,    4,    4,    1,
    1,    5,    5,    1,    3,    3,    1,    1,    7,    7,
    5,    3,    0,    1,    1,    1,    2,    0,    1,    1,
    1,    0,    3,    1,    3,    1,    0,    3,    1,    3,
    1,    3,    3,    1,    1,    3,    1,    2,    1,    3,
    3,    1,    2,    3,    3,    3,    3,    1,    3,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    3,    3,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    1,    1,    5,    6,    3,    4,
    5,    6,    3,    4,    3,    4,    3,    4,    4,    5,
    4,    4,    3,    4,    1,    1,    1,    1,    1,    1,
    1,    3,    3,   10,   10,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    3,    3,    3,    3,
    3,    3,    3,    3,    1,    1,    1,    1,    3,    1,
    1,    1,    1,    1,    4,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    1,    2,    1,    1,    1,    1,    1,    2,    1,    1,
    1,    1,    1,    4,    5,    5,    5,    5,    5,    5,
    5,    5,    5,    5,    6,    6,    6,    8,    4,    4,
    3,    6,    1,    1,    6,    6,    1,    1,    4,    6,
    4,    5,    3,    5,    4,    5,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,    0,
  552,  553,  554,  558,  559,  560,  615,  725,  726,  730,
  731,  460,    0,    0,    0,    0,    0,    0,  398,  570,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  903,    0,   21,  901,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  478,    0,    0,   37,   34,
   36,    0,   35,    0,  984,  986,  988,  987,  993,  989,
  996,  994,  990,  992,  995,  991,  997,  998,  999, 1000,
 1001,    0,    0,    0,    0,   32,   68,   66,  976,  985,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  644,    0,    0,    0,  682,    0,  978,
    0,    0,    0,    0,  107,  983,  561,    0,    2,    0,
    0,    0,    0,  428,  429,  430,  431,  432,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
  445,  446,  447,  448,  449,    0,  426,  427,    0,  969,
    0,  405,  973,    0,  402,  975,    0,  684,    0,  401,
  979,    0,  568,  899,    0,  150,  103,    0,  104,    0,
  218,  965,  764,    0,  102,  145,  972,    0,    0,   98,
    0,  105,  905,    0,   99,    0,  100,    0,  101,  982,
  146,  106,  623,  624,    0,  904,  902,    0,   74,    0,
  970,    0,  968,  486,  479,    0,  485,  490,  480,  481,
  488,  482,  483,  484,  489,    0,   30,    0,   39,  751,
   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  909,    0,  890,  638,
  638,  639,  649,  651,  958,  652,    0,  653,  643,  639,
    0,    0,    0,    0,    0,    0,    0,  587,  576,    0,
  579,  581,  582,  583,  584,  590,  591,    0,  683,  977,
    0,  108,    0,  109,    0,  110,    0,    0,    0,  616,
    0,    0,    0,    0,    0,    0,    0,    0,  461,    0,
    0,  368,    0,  221,    0,  408,  399,  458,  459,    0,
    0,    0,    0,    0,    0,  417,    0,  374,  375,    0,
  574,  572,  900,    0,    0,    0,    0,    0,    0,    0,
  153,  132,    0,   96,    0,    0,  111,    0,  121,  120,
    0,    0,    0,    0,    0,    0,  961,   75,    0,    0,
    0,  727,  728,  487,    0,    0,    0,    0,  868,  896,
    0,  960,    0,  870,    0,    0,  897,    0,  891,  882,
  881,  880,    0,    0,    0,    0,  872,  873,  874,  883,
  884,  892,  893,    0,  894,  895,    0,    0,    0,  898,
    0,    0,    0,    0,    0,  886,    0,  761,  865,  866,
  867,  680,  871,  350,  851,    0,    0,    0,    0,  850,
  678,  847,  848,  849,  852,  853,  854,  885,  910,  911,
  912,  913,  933,  934,  937,  938,    0,  754,    0,    0,
   27,    0,    0,    0,   71,   69,  980,   67,   33,    0,
    0,    0,  756,    0,   52,    0,    0,    0,    0,    0,
  566,    0,    0,    0,  646,  648,    0,  655,    0,  738,
    0,    0,  656,    0,  642,    0,  613,    0,  588,    0,
  585,    0,  586,  611,    0,  606,    0,    0,  580,  765,
  734,    0,    0,    0,    0,    0,    0,  563,  562,    0,
    0,  620,  617,    0,    0,  614,  186,    0,    0,    0,
  129,  466,  173,  174,    0,  469,  464,    0,  470,    0,
    0,    0,    0,  471,    0,  462,    0,    0,  412,  414,
  413,    0,  415,    0,  409,  219,    0,    0,  272,    0,
    0,    0,    0,  421,  422,    0,  451,    0,    0,    0,
    0,    0,    0,    0,  379,  378,  474,    0,    0,  149,
    0,    0,  158,    0,  157,    0,    0,  164,  147,    0,
  162,  165,  166,  154,    0,    0,  509,  544,  512,    0,
  545,    0,    0,  513,    0,    0,    0,  536,  548,  508,
  531,  510,  511,  506,  493,  368,  491,  492,  494,    0,
    0,  503,  504,  505,  507,    0,    0,    0,    0,    0,
  112,    0,  113,    0,  124,    0,    0,  202,    0,    0,
  366,    0,  221,    0,    0,   83,    0,    0,  746,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  767,  769,  778,  781,  782,  783,  784,  785,  786,  787,
  788,  789,  790,    0,    0,    0,  887,    0,    0,    0,
    0,    0,    0,  888,  889,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  325,  981,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  753,   28,    0,    0,   44,   72,    0,    0,   26,
    0,  759,    0,    0,    0,  722,    0,    0,    0,    0,
    0,  565,    0,  638,    0,    0,    0,    0,  660,    0,
  556,    0,  907,  555,    0,  589,    0,  597,  593,  592,
    0,    0,    0,  608,  596,    0,    0,  732,  733,    0,
    0,    0,  366,    0,    0,    0,  630,  634,    0,    0,
    0,  130,    0,  468,    0,    0,    0,    0,    0,  368,
  411,    0,  368,  369,  416,  501,    0,  274,    0,  268,
  270,  366,  241,  966,    0,    0,  368,    0,    0,  210,
  366,    0,    0,    0,    0,  246,  240,  243,  244,  245,
  420,  456,  455,  419,  418,    0,    0,    0,    0,    0,
    0,  377,    0,  368,    0,  155,  159,    0,    0,  163,
    0,    0,  532,  533,  549,    0,  546,    0,    0,  539,
  537,  538,    0,    0,    0,    0,  502,    0,    0,    0,
    0,  540,    0,    0,  366,    0,  201,    0,  204,  206,
  207,    0,    0,    0,    0,    0,  221,   78,    0,    0,
   77,  520,  962,  521,    0,    0,    0,    0,  768,  780,
    0,    0,  838,  839,  840,    0,  772,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  950,  951,  952,  953,  954,  955,  956,  957,    0,    0,
    0,    0,    0,    0,  943,    0,    0,    0,    0,  863,
  327,  864,  931,    0,    0,  859,  964,    0,    0,    0,
    0,    0,  679,    0,  869,  763,  762,  755,    0,    0,
    0,    0,    0,    0,    0,    0,   53,   61,   63,    0,
    0,   70,    0,    0,    0,  758,    0,    0,    0,    0,
    0,    0,    0,    0,  647,  650,    0,    0,    0,    0,
  665,  666,    0,    0,  657,  974,  659,  557,  908,  598,
  607,  602,  601,    0,  612,  766,  735,  736,  366,    0,
    0,  640,  622,  621,    0,  182,  176,  183,    0,  179,
  467,    0,    0,  178,    0,  189,  170,  169,  906,  172,
  171,  472,    0,    0,    0,  410,  273,    0,  212,  368,
  214,    0,  369,  209,  215,    0,    0,    0,    0,  423,
  424,  368,  366,  369,    0,    0,    0,  395,    0,  197,
  198,  156,    0,  168,    0,    0,    0,    0,    0,  547,
    0,    0,  368,  369,    0,  497,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  199,
    0,    0,    0,    0,  354,    0,   82,    0,    0,    0,
    0,   87,   84,    0,    0,    0,  729,  967,    0,    0,
  773,    0,    0,  779,    0,    0,    0,    0,    0,  816,
  815,    0,    0,    0,  823,  841,  833,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  771,    0,    0,
    0,  221,    0,  914,    0,    0,  929,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  939,
    0,  949,    0,    0,    0,    0,    0,    0,  930,  875,
   65,   57,   58,   64,   59,   60,    0,    0,   22,    0,
    0,    0,   42,    0,    0,    0,  740,  760,    0,  723,
    0,  739,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  685,    0,    0,    0,    0,    0,  604,
  605,  609,    0,  200,    0,    0,    0,    0,    0,    0,
    0,    0,  190,    0,    0,    0,  406,  224,  225,  226,
  227,  228,  229,  878,    0,    0,    0,  277,    0,  222,
    0,    0,    0,    0,  247,    0,  452,  453,  425,    0,
    0,    0,    0,    0,    0,  368,  476,  369,  167,  963,
    0,    0,  515,    0,  514,  550,    0,  542,    0,    0,
    0,  495,    0,  523,    0,    0,  525,  528,  530,  366,
    0,    0,    0,  119,    0,  205,  368,    0,    0,  367,
    0,   97,    0,  117,    0,   93,    0,    0,    0,    0,
   86,    0,  831,  832,  775,    0,  777,    0,    0,    0,
    0,    0,    0,    0,  834,    0,    0,    0,    0,    0,
    0,    0,  824,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  920,  919,    0,
    0,  916,  915,    0,  924,  923,  922,  921,  918,  917,
    0,    0,  945,    0,    0,    0,  944,    0,    0,    0,
    0,    0,    0,   56,   62,   54,   55,   23,  744,  745,
    0,  742,  749,  750,  747,    0,    0,  721,    0,    0,
    0,  673,    0,    0,    0,  691,  687,  689,    0,    0,
    0,    0,    0,  367,  626,  627,  641,    0,  184,  180,
    0,    0,  195,  191,    0,  369,  879,  280,    0,    0,
  369,  278,    0,    0,    0,  232,    0,    0,  248,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  473,  517,
    0,  518,    0,  134,    0,    0,    0,  138,  140,  141,
  142,  498,  500,  496,  527,    0,    0,  369,    0,    0,
    0,    0,  351,    0,  355,  368,   88,    0,    0,   90,
    0,    0,    0,    0,    0,    0,  843,  842,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,  932,  925,  926,
    0,  927,  941,  940,    0,  946,  935,  936,    0,   40,
    0,    0,  724,    0,  676,  674,    0,    0,    0,  695,
    0,    0,  636,  369,  352,  628,    0,  175,  192,  369,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  285,  290,  284,
    0,    0,    0,  294,  295,  296,  297,  298,  299,  301,
  302,  304,  305,  306,  309,  267,    0,    0,  208,  233,
    0,    0,    0,    0,  234,  237,    0,    0,  369,  393,
  394,  390,  389,  392,  391,    0,  516,  543,  143,  131,
  139,    0,  368,  264,  126,  369,    0,    0,  370,  356,
    0,    0,   91,   92,   79,    0,  802,  810,    0,  803,
  811,  806,  814,    0,    0,  804,  812,  805,  813,  799,
  807,  800,  808,  801,  809,    0,  942,    0,  741,    0,
    0,    0,    0,  705,  707,    0,  702,    0,  265,  185,
  407,    0,  312,    0,    0,  315,  316,    0,  308,    0,
    0,    0,    0,  307,    0,  337,  338,  293,  300,    0,
  303,  286,    0,  291,    0,  341,  292,  211,  216,  235,
    0,    0,  238,    0,  250,  252,  397,  369,    0,  372,
    0,    0,    0,  361,  362,    0,    0,   76,   94,    0,
  928,    0,    0,    0,  667,    0,  696,  706,    0,    0,
  283,    0,  310,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  332,  333,    0,  349,    0,    0,    0,    0,
  344,  342,  239,    0,    0,    0,    0,    0,  266,  369,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  670,    0,    0,  713,    0,    0,    0,  282,  279,  311,
    0,    0,  314,    0,    0,    0,  322,    0,  324,    0,
    0,  348,  347,  971,  346,    0,    0,    0,    0,    0,
  249,    0,  256,    0,  255,  373,  365,    0,    0,    0,
  357,  118,    0,    0,    0,    0,  668,  715,  704,    0,
    0,  716,    0,  712,    0,    0,    0,    0,    0,    0,
    0,    0,  335,  334,  343,  345,  236,    0,    0,  260,
  259,  258,    0,    0,    0,  845,  637,  671,  711,    0,
  718,    0,    0,  313,  319,    0,    0,    0,    0,  336,
  263,  261,  262,    0,    0,    0,    0,    0,    0,    0,
  339,  317,    0,  363,    0,    0,    0,  717,  330,  318,
  320,  844,  720,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1554,   28,   29, 1555,
   31,   32,   33,   34,   35,   36, 1556,   38, 1557,  649,
  122,  460,  965, 1199,  123,  715,  966,  124,  464,  720,
  148,  125,  126,  259,  988,  138,  967,  968,  199,  228,
  127,  969,  128,  466,   82,  239,  378,  425,  644, 1100,
  426,  870,  615,  221,  847,  645,  646,  647, 1102,  240,
 1389,   74,  189,   75,  235, 1847, 1312,  220,  225,  227,
  229,  215,  207,  209,  222,  232,  155,  312,  314,  316,
  367,  631,  633,  457,  260,  796,  548,  659, 1036,  371,
  636, 1088, 1304,  581,  743, 1062,  530,  531,  563,  363,
  616, 1074, 1456, 1579, 1247, 1457, 1458, 1459, 1460, 1461,
  660,  216,  208,  359,  589,  360,  361,  590,  591,  592,
  593, 1279, 1030,  533,  534, 1019, 1017,  535, 1024, 1018,
 1237, 1419, 1025, 1026, 1242,  373,  858,  859,  860,  861,
  798, 1043, 1434,  617,  800,  428,  863,  946,  557, 1361,
 1248,  429,  430,  431,  432,  433, 1435, 1436,  566, 1585,
 1736, 1265,  806,  807,  793,  808,  809,  810, 1737, 1738,
 1821,  341,  561, 1256, 1558,  195,  558,  559, 1257, 1258,
 1429, 1712, 1559, 1560, 1561, 1562, 1675, 1563, 1564, 1565,
 1566, 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575,
  434, 1806, 1658, 1722, 1810, 1720,  435,  699,  436,  437,
 1107, 1676, 1730, 1731, 1773, 1093, 1310, 1473, 1094, 1095,
  287,  491, 1608,  382, 1229, 1412, 1692, 1693, 1694, 1695,
   40,  740,  438, 1705, 1227,  350,  577,  824, 1277,  578,
  822,  223,   59,  345,  200,  163,  328,  186,  335,  192,
  784,  781,  336,  346,  814,  347,  573, 1054,  187,  439,
  329,  545,  579, 1278,   96,  198,  244,  618,  619,  786,
 1075,  787, 1076, 1077,  622,  623,  624,  625, 1068,  874,
  626,  627,  628,  840,  629,  837,   41,   42,   43,   44,
   45,   46,  157,  519,   61,  482,  203,  352,  299,  300,
  301,  302,  303,  304,  305,  306,  307,  749,  505, 1004,
 1232,  506,  507,  754,  162,  322,  526,   47,  521,  522,
 1013, 1417, 1236, 1536,  767,  142,  737, 1533,  726,  485,
  289,  143,  144,  440,  288,  492,  493,  494,  997,  989,
  990,  991,  992, 1752, 1221, 1182, 1529, 1530, 1754, 1648,
 1755, 1756, 1757, 1804, 1848,  472, 1211,   48,   49,  383,
   50,   51,  513,  514,  974, 1207, 1391, 1395,  473,  723,
  213,  661,  662,  888,  889,  663,  664,  665,  666,  667,
  668,  669,  670,  671,  672,  673, 1333, 1123, 1125, 1126,
  945,  698,  442,  443,  444,  445,  446,  447,  448,  237,
  449,  450,  451,  452,  929,  453,  454,  455,  456,  689,
  933, 1183, 1178,  690,  130,
};
static short yysindex[] = {                                   8432,
 1214, -145, 3691,  132, 1435,  128, 3600,  -72, 4653,   13,
  441, 6153, -145, 1272,  561, 6184, 1053, 6184,  226,    0,
  756,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -27,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 9351, 6184, 6184, 6184, 6184, 6184,    0,    0,
  450, 1311, 6184, 6184, 6184,  607, 6184,  417, 6184, 6184,
    0, 6184, 6184,    0,  820,    0,  500,    0,    0,  691,
 6184,    0, 6184, 6184, 6184,  898, 6184, 6184, 6184, 6184,
  417, 6184, 6184, 6184, 6184,    0, 6184,  696,    0,    0,
    0, 1213,    0, 1213,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  894, 1030,  202, 1289,    0,    0,    0,    0,    0,
 6184, 6184, 6184, 1090, 1111, 1115,  156, 1376, 1069,  107,
  131,  984, 1039,    0, 6184, 1248, 1298,    0, 1186,    0,
 6184, 6184, 6184, 6184,    0,    0,    0, 6184,    0, 1347,
    3, 1296,  847,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  916,    0,    0, 1311,    0,
  452,    0,    0, 1591,    0,    0, 1036,    0,  646,    0,
    0, 1215,    0,    0, 1180,    0,    0, 1273,    0, 1391,
    0,    0,    0, 1311,    0,    0,    0,  606, 1591,    0,
  -10,    0,    0, 1355,    0, 1383,    0, 1213,    0,    0,
    0,    0,    0,    0, 1397,    0,    0, 1282,    0, 1410,
    0, 1302,    0,    0,    0, 1282,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 9112,    0, 6184,    0,    0,
    0, 4946, 1365, 6027, 6184, 1346, 1437, 1744, 1469, 1478,
 4946, 1408, 6027, 1439, 1444, 1329,    0,10786,    0,    0,
    0,    0,    0,    0,    0,    0, 8292,    0,    0,    0,
 1401, 6184, 1482, 1395,  432, 6184, 1416,    0,    0, 1298,
    0,    0,    0,    0,    0,    0,    0, 6184,    0,    0,
 1591,    0, 1355,    0, 1213,    0, 1473,10786, 1449,    0,
 1434, 1426,  -91, 1366, -115, 1375, 1168,  847,    0, 1600,
 1119,    0, 6184,    0,  916,    0,    0,    0,    0, 6184,
 1486, 4426, 1450, 4771, 1865,    0, 9351,    0,    0,  857,
    0,    0,    0, 1867, 1867, 1311, 1549, 1311, 2165, 1273,
    0,    0,11249,    0, 6184, 6184,    0, 1486,    0,    0,
 1640,  430, 1896, 6184, 1682, 6184,    0,    0, 1913,  253,
 5791,    0,    0,    0, 1915, 1930, 1934, 1498,    0,    0,
 1936,    0, 1955,    0, 1960, 1966,    0, 1968,    0,    0,
    0,    0,    0, 1969, 1505, 1555,    0,    0,    0,    0,
    0,    0,    0, 1977,    0,    0, 9192, 1980, 1982,    0,
10786,10786, 8540, 6184, 1983,    0, 2700,    0,    0,    0,
    0,    0,    0,    0,    0, 1987, 1933, 1986,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1586,    0, 6184, 1590,
    0, 6184, 6184,  102,    0,    0,    0,    0,    0, 6184,
 6184,  588,    0, 6184,    0, 1713, 6184, 1990, 4946, 1633,
    0, 6184, 9431, 2171,    0,    0,10786,    0, 2700,    0,
 1717, 1994,    0, 1775,    0,  207,    0,  502,    0,   14,
    0,   14,    0,    0, 1996,    0,  165, 1615,    0,    0,
    0, 1998,  182, 1981, 1486, 2008, 1785,    0,    0, 2150,
 2009,    0,    0, 1678, 1556,    0,    0,  176, 1718, 1720,
    0,    0,    0,    0, 1311,    0,    0, 1721,    0, 1867,
 1867, 1867, 1867,    0, 2034,    0, 1801, 1802,    0,    0,
    0, 1781,    0,11249,    0,    0,11249, 1644,    0, 1591,
 1819, 6184,12550,    0,    0,  570,    0, 6184,  564, 1036,
 1687, 1650, 1618,    0,    0,    0,    0,  520, 1688,    0,
 1632,  417,    0, 1311,    0, 1814, 1747,    0,    0, 2165,
    0,    0,    0,    0,  292, 1645,    0,    0,    0, 1652,
    0, 1698, 2060,    0, 1752, 2062,   52,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2017,
  186,    0,    0,    0,    0,  158, 2070, 2071, 2062, 1591,
    0, 1800,    0, 1855,    0, 1311, 1311,    0, 4426,  646,
    0, 1740,    0, 1715, 2081,    0,10163, 2649,    0, 1735,
 1749, 2092, 6352, 2094,    0,    0,    0, 5515, 8332,   -3,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  985, 9670,  149,    0, 6184,10786, 1131,
 1210, 1247, 2245,    0,    0,10786, 6438, 2700, 1684,  -34,
10786, 9670,  775,  775, 1272, 2240, 1714, 2095,    0,    0,
 9750, 6184,10786,10786,10786,10786,10786, 9670,10786, 2080,
 6184,    0,    0, 6604, 1706,    0,    0, 1680, 6027,    0,
 1828,    0, 1765, 1272, 2111,    0, 1828, 6604, 1832, 1833,
 6184,    0, 2240,    0, 2263, 1669, 1704, 9670,    0, 6184,
    0, 1024,    0,    0,  -16,    0, 1709,    0,    0,    0,
 6184,  305, 6184,    0,    0, 6184, 6184,    0,    0, 9670,
 1898, 4426,    0, 1661,10786, 6184,    0,    0, 1311, 1311,
 1311,    0,  555,    0, 1632, 1632, 1708,  417, 1168,    0,
    0, 2127,    0,    0,    0,    0, 1899,    0, 6184,    0,
    0,    0,    0,    0, 1901, 2133,    0, 1882, 1904,    0,
    0, 1839, 1843, 1213, 1911,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  417, 6184,12631, 1804, 1808,
 1810,    0,  417,    0, 1595,    0,    0, 1780, 1311,    0,
   97, 1784,    0,    0,    0,  417,    0, 2060,  139,    0,
    0,    0, 1907,  139, 1786,  139,    0, 2163,  417,  417,
  417,    0, 1486, 6184,    0, 1859,    0, 1788,    0,    0,
    0,  857, 1793, 6184, 1792,  146,    0,    0, 2179, 1946,
    0,    0,    0,    0,  253, 6184, 1807, 6668,    0,    0,
 1807, 6352,    0,    0,    0, 6695,    0,  152,   60,10786,
10786, 6926, 2181,  891, 7023,10786, 7281,  998, 7433, 7542,
 9989, 7801, 7953, 8062, 6438, 6438,10786,10786, 1962,10786,
 2186,10786, 2184, 2314,10786,10786,10786,10786,10786,10786,
    0,    0,    0,    0,    0,    0,    0,    0, 1909, 1756,
  466,10786,  598, 9670,    0, 6438, 2283, 2187,  984,    0,
    0,    0,    0, 2700, 1811,    0,    0, 1953,  775,  775,
 1340, 1340,    0,  275,    0,    0,    0,    0, 6184, 6184,
 6184, 6184, 6184, 6184,  117, 2190,    0,    0,    0, 6604,
 1850,    0,  -38, 1869, 6184,    0,  984, 9670, 1869, 2194,
 2190, 6604, 6754, 1921,    0,    0,  949, 2200, 2198, 1402,
    0,    0, 6438, 1918,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -92,    0,    0,    0,    0,    0, 1827,
 1793,    0,    0,    0, 2199,    0,    0,    0, 1854,    0,
    0, 1867, 1984,    0,  555,    0,    0,    0,    0,    0,
    0,    0, 2207, 6438, 3303,    0,    0, 1950,    0,    0,
    0, 3303,    0,    0,    0, 1213, 1213, 1750, 2212,    0,
    0,    0,    0,    0,  472,   48, 1060,    0, 1989,    0,
    0,    0, 6184,    0,  132, 1860, 1834, 1899, 2213,    0,
 1632, 1864,    0,    0,  819,    0, 2205, 6184, 1002,  417,
 2225, 2226, 2228, 2011,  646, 1950, 1867, 1720, 4426,    0,
 1688,  984, 1836, 1846,    0, 1502,    0,  417, 1879, 1979,
10163,    0,    0,  417, 1806, 1991,    0,    0, 2252, 2262,
    0, 6438, 6438,    0, 6438, 6438, 2217, 2700, 2267,    0,
    0, 2700, 2269,10228,    0,    0,    0, 1959, 2272, 2700,
 2274, 1880, 2278, 2700, 2292,10786,10786, 2181,10786,10308,
 2295, 2700, 2297, 2298, 2700, 2299,10786, 2700, 2304, 2700,
 2308, 2309, 2700, 2311, 2312, 2700, 2318,    0, 2100, 2326,
 2377,    0, 2417,    0, 2512,10786,    0, 2983, 3130, 3157,
 3164, 3172, 3224,10786,  417, 9670, 2700, 1928, 9670,    0,
10786,    0, 2046,  484,10786, 9670,10786,10786,    0,    0,
    0,    0,    0,    0,    0,    0, 2038, 6604,    0, 6604,
  117, 2190,    0,  342,  342,  178,    0,    0, 8292,    0,
 1870,    0, 2194, 2190, 2330, 6754,  187, 1402, 2329, 9670,
 1775, 1669, 1992,    0, 1993, 1999, 2035, 2105, 2053,    0,
    0,    0, 1950,    0, 1836,  983, 1964, 1311, 2000, 1632,
 2015, 1632,    0, 6438,  406, 1431,    0,    0,    0,    0,
    0,    0,    0,    0, 1954, 2128, 1950,    0,10786,    0,
 2120, 2004, 1750,  820,    0, 6438,    0,    0,    0, 2026,
 2027, 2030, 2031, 2037, 2039,    0,    0,    0,    0,    0,
 2380,  132,    0, 1708,    0,    0,  139,    0, 3303, 2054,
  139,    0,  139,    0, 1899, 2390,    0,    0,    0,    0,
  857, 2128, 1632,    0, 1720,    0,    0, 8292, 2155,    0,
   74,    0, 2199,    0, 2394,    0, 2400, 1311,   -6, 2409,
    0, 1311,    0,    0,    0, 2100,    0, 2100,10786, 2040,
 2040, 1272, 2414, 1894,    0, 2040, 2040,10786, 2040, 2040,
 2860, 2700,    0, 1926,10786, 2700, 2040, 2040, 2040, 2040,
 2700, 2040, 2040, 2040, 2040, 2040, 2040,    0,    0,11249,
 2420,    0,    0, 3238,    0,    0,    0,    0,    0,    0,
 3289,  169,    0, 9670, 2158, 2032,    0, 9670, 3296, 1903,
 1797, 2700, 2083,    0,    0,    0,    0,    0,    0,    0,
 2041,    0,    0,    0,    0, 1717, 9670,    0, 6754, 2330,
 1775,    0, 1919, 6184, 1402,    0,    0,    0, 1669,10786,
 6438, 2082, 2128,    0,    0,    0,    0, 1867,    0,    0,
 1311, 1595,    0,    0,  486,    0,    0,    0, 6184, 7676,
    0,    0, 2700, 2196, 2120,    0,  987, 6184,    0, 2143,
  685, 2044, 2145, 2049, 2180, 2160, 2195, 1950,    0,    0,
 2462,    0, 2464,    0, 2154, 2196, 2054,    0,    0,    0,
    0,    0,    0,    0,    0, 1950, 1688,    0, 1595, 1989,
 2189, 2126,    0, 1793,    0,    0,    0, 2141,   27,    0,
 1899, 2144, 2700, 2475, 2477,  984,    0,    0, 2483, 2485,
 2700, 2486, 2490,10786,10786, 2700, 2491, 2493, 2494, 2496,
 2497, 2498, 2500, 2501, 2503, 2504,    0,    0,    0,    0,
  417,    0,    0,    0, 9670,    0,    0,    0, 1704,    0,
  342, 1704,    0, 6184,    0,    0, 1158, 2700, 2505,    0,
   -3,  764,    0,    0,    0,    0, 1632,    0,    0,    0,
    0,11249, 2384,  656, 2499,  625, 2510, 2216,10786, 2502,
 2522, 2507, 2508, 2514, 2515, 2224, 2516,    0,    0,    0,
 2268,12432, 2519,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2530, 6184,    0,    0,
 2232, 2242, 6184, 1750,    0,    0, 1213, 6184,    0,    0,
    0,    0,    0,    0,    0, 2128,    0,    0,    0,    0,
    0, 2128,    0,    0,    0,    0, 1893, 1704,    0,    0,
10786, 1311,    0,    0,    0, 1311,    0,    0,10786,    0,
    0,    0,    0, 2700, 2700,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2546,    0, 1918,    0, 1918,
 2553, 6438,10786,    0,    0, 2227,    0, 2558,    0,    0,
    0,  381,    0,10547, 6184,    0,    0,10786,    0, -139,
 2260, 6438, 2700,    0, 6438,    0,    0,    0,    0, 2350,
    0,    0,  714,    0, 2151,    0,    0,    0,    0,    0,
 1750, 1213,    0, 2218,    0,    0,    0,    0, 1989,    0,
 1604, 2559, 1402,    0,    0, 1918, 2700,    0,    0, 1797,
    0, 2053, 2053, 6184,    0,   -3,    0,    0, 5454, 3303,
    0, 2548,    0, 2968, 9112, 1614,10786, 2350,  702, 2550,
  734, 1113,    0,    0, 2564,    0, 6184, 6184,  132,  170,
    0,    0,    0, 2218,  399, 1750, 2222, 2229,    0,    0,
 1402, 2572,  206, 1893, 2271, 2053, 2574, 1704, 2082, 2082,
    0, 1920, 6184,    0, 2576, 2575,  -47,    0,    0,    0,
 2210, 2350,    0, 1627,  167, 2188,    0, 2317,    0, 2350,
 2459,    0,    0,    0,    0, 7676,  714, 1750,  476,  476,
    0, 2324,    0, 2185,    0,    0,    0, 1775, 1402, 1893,
    0,    0, 1918, 2239, 2239, 6184,    0,    0,    0, 5454,
 2588,    0, 6184,    0, 1638, 2579, 1462, 2350, 2338, 2331,
 7676, 7676,    0,    0,    0,    0,    0, 2170,   18,    0,
    0,    0, 6184, 1252, 2053,    0,    0,    0,    0, 6184,
    0, 2350, 2590,    0,    0,  190, 6184, 7676, 2332,    0,
    0,    0,    0, 2553, 6438, 2082, 2596, 2600, 1931, 7676,
    0,    0, 7676,    0,   -3, 2239, 6184,    0,    0,    0,
    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  195,  701,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  105, 5855,    0,    0,    0,    0,    0,    0,
 2642,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  217,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  312,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2306,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2264,    0,    0,
    0,  300,    0,  300,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2265,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  960,    0,
    0, 8626, 6122,    0,    0,    0,  993,    0,    0,    0,
 1575,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  235,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  499,    0,    0,    0,    0,  245,    0,
    0, 1150,    0,    0,    0,    0,    0,  140,    0,12673,
    0,    0,    0,    0,    0,    0,    0,    0,  499,    0,
  651,    0,    0, 2605,    0,    0,    0, 2382,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1177,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1237,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  945,    0,    0,
    0,    0,    0,    0,    0, 7109,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  825,    0,    0, 1266,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  499,    0, 2605,    0, 2382,    0, 1285,    0,    0,    0,
    0,  781,    0,    0,    0,    0,    0, 1300,    0, 2373,
    0,    0,    0,    0, 1319,    0,    0,    0,    0,    0,
 2388,   85, 9902,    0, 1371,    0,    0,    0,    0,  269,
    0,    0,    0, 2197, 2197,    0,    0,    0, 1405,  407,
    0,    0,    0,    0,    0,    0,    0, 2388,    0,    0,
 1528,    0,    0,    0,    0,    0,    0,    0, 9605, 2270,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1937,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2734,    0,    0,    0, 3038, 3342,    0,
    0,    0,    0,    0,    0,    0,  121,    0,    0,    0,
    0,    0,    0,    0,    0,  192, 3646,    0, 2428,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1445,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1485,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1440,    0,
    0, 2341,    0, 2492,    0,    0,    0,    0,    0, 1204,
    0, 1204,    0,    0, 1176,    0, 1183, 1255,    0,    0,
    0,    0, 1177,    0, 2388,    0,    0,    0,    0,  109,
   92,    0,    0,  240,    0,    0,    0, 1045,    0,  459,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2197,
 2197, 2197, 2197,    0, 1000,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2404,
    0,    0,    0,    0,    0, 2373,    0,    0, 1187,    0,
 2208,    0,    0,  802,    0,    0,    0,    0,  783,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1495,
    0,    0,    0,    0, 3029, 4303,    0,    0,    0, 4823,
    0,    0, 6929,    0,    0, 7284,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2230,
 3012,    0,    0,    0,    0, 9014, 5343,    0, 7284,  499,
    0,    0,    0,    0,    0,    0,    0,    0,   85, 1023,
    0,    0,    0,    0, 2247,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11123,11345,11429,    0,    0,  599,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10866,    0,10866,    0,    0,    0,10866,
10866,10866,    0,    0,    0,    0,    0, 2234,    0,    0,
    0,    0, 4470, 4686, 8873, 2633,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  201,    0,    0, 5855,    0,    0,  201,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  981,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  372,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   85,    0,  359,    0,    0,    0,    0,    0,    0,
    0,    0, 1232,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  790,    0,    0,    0,
    0,    0,    0,    0, 2639,    0,    0, 3901, 3824,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  737,  740,
  798,    0,    0,    0, 1364,    0,    0,    0,    0,    0,
    0, 3216,    0,    0,    0,    0,    0, 6929,    0,    0,
    0,    0,  806,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2388,    0,    0,  326,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10648,    0,    0,    0,  248,
    0,    0,    0,    0, 2374,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,10866,    0,
    0,    0,    0, 1171,    0,    0,    0, 3950, 4990, 5206,
  838, 4166,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1537, 1543,    0,    0,    0,    0,
    0,    0,    0, 1558,    0,    0, 8626,    0, 1558, 1559,
 1563,    0,    0,    0,    0,    0,    0, 5316,  807,  654,
    0,    0,    0, 1170,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  320,    0,    0,    0,    0,    0,
    0, 2197, 2231,    0, 1468,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2415,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1363,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1594,    0,
    0,    0,    0,    0,    0,    0,    0, 3333,    0,    0,
    0,    0,    0,    0,    0,    0, 1099,    0,    0,    0,
    0,    0,    0,    0, 1023, 2415, 2197, 1599,   85,    0,
 2423, 8626, 1603,  113,    0,    0,    0,    0,10648,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 5871,    0,    0,
    0, 6112,    0,    0,    0,    0,    0,    0,    0, 8870,
    0,11197,    0,11513,    0,    0,    0,    0,    0,    0,
    0,11585,    0,    0,11659,    0,    0,11693,    0,11722,
    0,    0,11743,    0,    0,11830,    0,    0, 5742,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2253,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1537, 1649,    0, 2255,  284,    0,    0,    0,    0,    0,
    0,    0, 1681, 1697, 1710,    0,    0, 2352, 1157,    0,
 2164,    0, 2353,    0, 2355, 2356,    0,    0,  529,    0,
    0,    0, 2415,    0, 1603,    0,  237,    0,    0,    0,
 2243,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 7156,    0, 2432,    0,    0,    0,
 4127,    0, 1363, 2306,    0,    0,    0,    0,    0,  904,
  929,  959,  976, 1065, 1072,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  864,
    0,    0,    0,    0, 3897,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1711,    0,    0,    0,    0,    0,
    0,    0, 1733,    0,    0,    0,    0,    0,  203,    0,
    0,    0,    0,    0,    0,  262,    0,  310,    0,    0,
    0, 8873,    0,    0,    0,    0,    0,    0,    0,    0,
    0,11863,    0,12000,    0,12029,    0,    0,    0,    0,
12063,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1782,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1736,
 7641,    0,    0,    0,  992,    0,    0,    0,    0,    0,
    0,  295,    0,    0,    0,    0,    0, 2197,    0,    0,
    0,  316,    0,    0,    0,    0,    0,    0,    0, 2397,
    0,    0, 2665, 1523, 4243,    0, 2373,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2415,    0,    0,
    0,    0,    0,    0,    0, 1741, 1092,    0,    0,    0,
    0,    0,    0,    0,    0, 2415, 2423,    0,  434,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  662,    0,12127,    0,    0,10866,    0,    0,    0,    0,
12201,    0,    0,    0,    0,12230,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  832,    0,
  284,  854,    0,    0,    0,    0, 2352,  603, 1384,    0,
  569, 2667,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1562, 1671,    0,    0,    0,    0,    0,    0,    0,
    0, 2401,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 4078,    0,    0,  116,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12264,12275,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  325,    0,  908,
 3640,    0,    0,    0,    0, 2670,    0,    0,    0,    0,
    0, 2652,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2653,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2407,    0,    0,    0,    0,    0,
 4078,  874,    0,  114,    0,    0,    0,    0,    0,    0,
    0,  562,  189,    0,    0,  110, 2665,    0,    0,    0,
    0,  280,  416,    0,    0, 4311,    0,    0, 2667,    0,
    0,    0,    0,    0,  115,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1089,    0, 1363, 1412, 1524,    0,    0,
 2352, 1193,    0,    0,    0,  138,    0,  832,  250,  443,
    0,    0,  687,    0,    0, 2684,    0,    0,    0,    0,
 2668,    0,    0,    0, 2422,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 4078,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7641,  492,    0,
    0,    0,  325, 2687, 1070,    0,    0,    0,    0, 2667,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2352,  280,    0,    0,    0,    0,    0,
    0,    0, 2672,    0,    0,    0,    0,    0,12351,    0,
    0,    0,    0, 2301,    0,  250, 2688,    0,    0,    0,
    0,    0,    0,    0, 3536, 2687,    0,    0,    0,    0,
    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2733,    0,    0, 2737,
    0,    0,    0,    0,    0,    0,   10,    0, 2738,    2,
  273,  -67, -593, 1539,  708, -275, -549, 2732, 2478,    0,
  -35,    0, 2484,  -96,  -40,    0,  185, -969,   -4,  -61,
  -36, -158, 2489, 2043,    0,    0,    0,  950,    0,    0,
  -11, 1656, -518,   11, -902, 2115, 1897, -306, 1666, 2673,
 -293,    0,  -60,    0, -477,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -585,  335, 1674, -301, 1155, -926,    0,
    0,    0,    0, -335, -123,-1081, 1679, -318,  -49,    0,
 -521,    0,    0, 1316, -972,    0, 1325,    0,    0, 2443,
 -635,    7,    0,    0,    0,    0, 2424,    0, 2193, 2463,
 -188, 1707, -700,    0,    0,    0, -680,    0,    0,    0,
    0,    0,    0, 1764,    0, 2487, 2029, 1712, 2457, 2460,
    0, 1151,    0, -525, 2006,  -33, -667, 1238,    0,    0,
    0, -941, -931, -916, -274, -915,    0, 1382,-1029,    0,
 1084,-1192, 1385,    0, 2251,    0,    0,    0, 1082, 1086,
 1041,  -32, -277, -778, -334,    0,    0, 2042,    0, 1569,
    0,    0,-1340,    0,    0,    0,    0, -209,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1114,    0, 1172,   46,    0,    0,  514,    0, -384, 1287,
  991, 1159,    0, 1056,    0, 1824, 1601, 1423,    0,-1098,
 -874, -412,    0, -494, -628, -345,    0,-1394, 1139,  795,
 -925,-1181, -125,  999, -269, -484, -754, -928,-1294,    0,
    0,    1,    0,    0,    0,    0,    0, 2509,    0,    0,
    0,    0, 2511, 2275,    0,    0,    0,    0,    0,   -9,
 2520,    0,    0,    0,    0,  -15,    0,    0,    0, -340,
 2001, -339, 1573, -461,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2236,    0, 2016,    0,    0,    0,    0,
    0,    0,   32,    0, 2856,    0,    0,    0,    0,    0,
 2571,    0,    0,    0,    0, 2379,    0, 2376,    0,    0,
    0, 2124,    0,    0, -379,    0,    0,   89,    0, 2116,
    0,    0,    0,    0,    0, -642,-1166, -906,  436, -114,
 1257,    0, 2747, -129, 1203,    0, 2153, -606,-1416,    0,
 -828, 1910,    0,    0,    0, -241,    0, 1256, 1372,    0,
 1098,    0, 1153,    0, 1051,    0,    0,    0,    0, 2396,
    0,    0,    0, -398, 2183, 1932,-1020,    0,    0,    0,
 -689, -457,  440,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3488,  970, 1774,    0,
 -602,    0,    0,    0, -905,    0,    0,    0, 1667,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -98, 1734,    0,    0,
};
#define YYTABLESIZE 13170
static short yytable[] = {                                     129,
  151,  191,  129,  214,   79,   78,  150,  261,  156,   37,
  281,  284,  490,  210,  441,  194,  280,  283,  758,  582,
  957,  197,  620,  621,  245,  309,  219,  554,  785,  211,
  552,  788,  254,  224,  744,  249,  231,  799,  697, 1404,
  588,  797,  764,  188,  190,  193,  196,  150,  201,  250,
  206,  931,  939,  212,  190,  217,  256,  196,  255,  150,
  201, 1217,  230,  129, 1187,  226, 1006,  151,  909, 1260,
 1439,  241,  722,  150,  243,  190,  252,  241,  217,  193,
  196,  977,  150,  201,  230,  129,  650,  196,  253, 1020,
  634,  618,  315, 1249,  248, 1011,  268,  267,  511,  269,
 1249,  251, 1209, 1250,  571,  465,  954, 1091,  625,  694,
 1250,  311,  353,  257,  465,  752, 1261,  313, 1251, 1253,
  681,  212,  150,  212, 1038, 1251, 1253, 1269,  871, 1254,
 1066,  375,  618, 1045,  980,  217, 1254,  699,  544,  151,
 1065,  310,  196,  150,  230,  719,  278, 1290,  156,  625,
  618,  291,  625, 1008,  257,  862,  752,  257, 1218,  752,
 1198,  681, 1307,  571,  681, 1285,  486,  625,  694,  279,
  282,  353,  257,  328,  752, 1606,   77,  337,  981,  681,
 1015,  441,  512, 1071, 1392, 1099,  368, 1086,  359,  317,
  911,  326, 1114,  279,  571,  879,  699,  849,  151,  265,
  743,  729,  364,  476,  775,  776,  777,  778,  753, 1512,
 1770,  772, 1475, 1777, 1519,  770,  619, 1308,  517, 1524,
  701, 1674,  620,  621,  458,  757,  461, 1401, 1385, 1522,
 1387, 1437,  359, 1770,  631,  461,  181,  761,  497,  633,
  279,  537,  994,   89,  376,  265, 1788,  359,  212,  320,
  326,  742,  150,  571,  467,  129,  527,  369,  905,  743,
  504,  150,  319,  467,  805,  528,  500, 1479,  475, 1158,
 1159,  588,  934,  935,  510,  619,  846,  181,  515,  701,
  181, 1204,  193,  522,  135,  131,  150, 1751,   85, 1801,
  703,   85, 1052,  631,  703,  181, 1741,   77,  633,  553,
 1184,  832,  774,  376,  717, 1656,  556, 1302,  211,   60,
  569,  573, 1230,  571,  843,  196, 1454, 1842, 1188,  632,
  699, 1115,  642,  212,  630,  125, 1802,  475,  841,   97,
  212,  831,  212, 1474,  212,  703,   77,  574,  640, 1803,
 1539, 1233,  522,  752,  583,  512,  585, 1249, 1272, 1789,
  776, 1449,  132,  703,  370,  196,  201, 1250,  629,  160,
  638,  632,  242, 1231,  150,  694,  193, 1190, 1657, 1843,
  573, 1210, 1251, 1253,  196, 1610, 1201, 1072,  632, 1828,
   79, 1613,  906, 1254,  125, 1268,   77, 1605, 1213, 1215,
  747, 1683, 1464, 1405, 1740, 1824,  842, 1205, 1245,  629,
  490,  538,  629,  242,  279, 1437,  152,  523,  285,  618,
  274,  730,  161, 1112,  700,  699,  936,  629,  151,   14,
 1202,  716,  502,  713, 1111,  529,  625,  681,  681, 1385,
 1809,  618, 1214,  196,  721, 1815,  716,  510,  727,  151,
  694, 1710,  703,  461,  769, 1116, 1426,   83,  625,  150,
  938,  910,  196,  467, 1413,  984,  699,  825,  463, 1480,
  150,  748,  845,  490,  150,  152,  953,  196,  151,  150,
 1839, 1840,  156, 1776,  699,  273, 1325, 1326, 1033, 1327,
 1328, 1035,  752,  703,  242,  490, 1511,  151, 1733, 1273,
 1092,  360,  196, 1850,  181, 1041,  151, 1852,  752, 1541,
 1639,  703,  151,  376, 1576, 1393, 1577,  181,  376, 1860,
 1313,   89, 1861,  732,  376,  181,  359,  463, 1614,  359,
  151, 1334, 1059,  773,  328,  242, 1540,  791,  699,  801,
  681,  692,  475, 1748,  619,  360,  181, 1113, 1603, 1098,
  571, 1604,  181, 1781,  181,  618, 1467, 1007,  701,  694,
  360,  701,  794,  212, 1029,  718,  619, 1420,  794, 1101,
  465,  358,  625,  694,  277,  181,  353,  743,  698,  699,
 1197,  681,  827,  196,  631, 1084, 1208,  699, 1615,  633,
 1527, 1106,  871, 1452,  181, 1817,  196,  699,  277,   76,
 1032,  699,  826,  181,  196,   79,  277,  853,  700,  181,
 1301,  326,  697,  571,  848,  211, 1823, 1649, 1425,  698,
  625, 1619,  703, 1651,  257,  196,  752,  181,  999,  985,
  358,  196, 1400,  196,  856,  857,  161,  698,  359,  212,
 1441,  512, 1466,  381,  703, 1029,  703,  264,  873,  700,
  359,  376,  359,  697,  196,  161,  697,  774,   79,  872,
  522, 1027, 1028,  663,  181,  694,  151,  700,   85,  632,
  701,  697, 1686,  196,  277,  475,  699,  905,  217, 1596,
  619,  196,  196,  125,  701,  699,  629,  958,  196, 1690,
 1042,  703,  752,  648,  913,  152, 1240, 1602,  631, 1486,
  716,  741,  947,  633,  663,  776,  196,  663,  629, 1779,
  212,  212,  524,  490,  467,  524,  152, 1002,  338,  467,
   76,  694,  663,  133,  500,  504,  463, 1005,  467,  136,
  694,  196,  648,  510, 1003, 1589,  196,  905,  211,  463,
  996,  158,  458,  196,  703,  152,  382, 1758, 1259,  380,
 1538,  150, 1766,  150,  196,  905,  212,  905,  703,   76,
 1267, 1303,  212,  181,  152,  556,  212, 1016, 1016, 1021,
 1252, 1739,  269,  152,  463, 1743,  463, 1252, 1249,  152,
 1747, 1289,  648,  632, 1768, 1531,  603,  637, 1250,  212,
  635,  196,  396, 1051, 1053,  339, 1440,  152, 1031,  524,
 1523,  906, 1101, 1251, 1253,  382, 1396,  381,  380,   76,
 1191, 1380,  699, 1194, 1254,  135,  662,  212,  212, 1270,
  149,  699,  629, 1786,  159,  501,  706, 1064, 1085,  360,
  819,  707,  360, 1058,  594, 1453, 1050,  603, 1096,  703,
  524, 1463,  196,  524,  692, 1403, 1069,  861, 1507,  635,
 1780,  396, 1794, 1795,  150,  275,  699,  662,  524,  512,
  524,  906, 1818,  701,  150, 1360,  381,  820,  202, 1081,
 1082, 1083, 1291,  136,  135,  662, 1108,  218,  699,  906,
  502,  906,  701,  752,   76,   79,  463,  242,  861,  861,
  861,  861,  861,  594,  861,  145,  698,  503,  802,  358,
  204,  205,  358, 1819,  701, 1471,  861,  861,  861,  861,
 1176, 1195,  348,  387, 1179, 1180,  745,  694,  698,  269,
 1022, 1292,  701, 1271,  752,  699,  700,  752, 1378, 1192,
  697, 1431,  136,  152,  699,  297,  700, 1196,  388,  700,
  861,  360,  752,  697,  490,  524,  122, 1660,  700, 1856,
 1820,  510,  697,  360,  441,  360,  905,  999,  694,  467,
  196,  201,  467,  230,  129,  698, 1193,  236,  384,  569,
  467,  821,  387,  905,  698,  115,  694, 1468,  699,  803,
  524,  663,  467,  467, 1448,  383, 1726,  196,  441,  349,
  701,  663,  699, 1023,  663,  700,  772,  388,  987,  697,
  146,  664,  578,  663,  700,  905,  692,  147,  697,  465,
  382,  358,  115,  380,  724, 1470, 1706,  365,  698, 1702,
  804, 1703,  238,  358, 1252,  358, 1652,  384,  569,  620,
  621,  701,  698,  714, 1638, 1727, 1719, 1640,  812, 1721,
  813,  725,  664,  490,  383,  664, 1728,  115,  700,  701,
  663,   14,  697,  366,  177, 1291,  396,  524, 1181,  663,
  664,  578,  700, 1280,  702,  211,  697, 1655,  465,  703,
  714,  381,  458,  524,  386,   79, 1281, 1746, 1280,  619,
  906,  385,  524,  714, 1390, 1390, 1394, 1373, 1534,  212,
 1375,  957, 1537,  135,  233,  135,  212,  906,  257,  524,
 1296,  137,  880,  663, 1295,  524,  257,  887,  524,  861,
 1656,  861,  861,  177,  323,  663, 1644,  663, 1316,  861,
  619,  122,  122, 1696, 1320,  575, 1422,  324, 1424,  906,
  635,  234,  490,  386,  662,  861,  576,  524,  619,  257,
  385, 1645,  257,  382,  662,  805,  380,  662, 1048,  861,
 1729,  368,  499,  136,  861,  861,  662,  257,  861,  575,
  137,  752,  325,  752,  326,  861, 1770,  861,  861,  701,
  752,  861,  701, 1657, 1825,  861,  594,  387,  861,  694,
  861, 1769,  861,  330, 1611,  600,  701,  861,  861, 1469,
  861,  701,  610,  135,  701, 1372,  457,  379,  467, 1646,
  467,  499,  388,  662,  381,  384,  861,  861,  965,  861,
  524,  594,  662,  599,   79,   79,  467,  524,  575, 1855,
  694,  876,  196,  861,  876,  861,  861,  861,  701,  246,
  861,  331,  384,  861,  861,  524, 1016,  701,  694,  594,
  457,  187,  594,  861,  600,  701,  328,  427,  694,  383,
  701,  610, 1127,  285,  285,  457,  662,  861,  594,  701,
  861,  752,  258, 1793,  595,  524,  752,  465,  662,  524,
  662, 1687,  599,  876,  327,  577,  861, 1688, 1128, 1136,
  465,  965,  861,  752,  594, 1513,  262,  861, 1526, 1516,
  752,  376,   79, 1451,  564, 1137,  286,  907,  861,  861,
  187,  861,  376,  342,  694,  328,  343,  861,  701,  403,
  387, 1689,  177,  694,  512,  465, 1478,  465,  701,  664,
 1482,  701, 1109,  595,  752,  177, 1110,  880,  404,  664,
  701,  880,  664,  177,  577,  388,  861,  861,  386,  861,
 1138,  664,  266,  332,  692,  385,  861,  861,  861,  861,
  861,  344,  861,  564,  177,  861,  861,  861,  861,  861,
  177,  333,  177,  861,  861,  384, 1749, 1750,  403, 1139,
 1274,  757,  251,  196, 1581,   14,  257,  701,  257,  368,
  400,  137,  383,  177,  752,  257,  701,  404,  664, 1542,
 1262, 1263, 1384,  693, 1386,  334, 1582,  664,  757,  467,
  285, 1390,  177, 1583,  310,  211,  510, 1275, 1641,  196,
 1791,  177, 1587,  251,  160,  550,  251,  177,  105, 1016,
  427,  253,  263, 1650, 1826, 1827, 1637,  465,  551,  212,
  701,  251,  196, 1745,  693,  177, 1140, 1584,  150,  400,
 1314,  664,  484,  915,  701, 1252,  512,  106,  107,  737,
  108,  489,  693,  664,   43,  664,  540,  109,  110,  111,
  112,  113,  253,  114,  152,  253,  115,  116,  117,  118,
  119,  386,  177,  160,  120,  121,  257,  188,  385,  285,
  253, 1745,  520,  441,  270, 1415, 1862,  665, 1223, 1846,
  737,  277, 1654,  737,   51, 1416,  153,  694,  257,  187,
 1684,  665, 1224,  271,  161,  257,  541,  272,  737,  665,
  694,  154,  187,   43,  276, 1770,  285,   52,  998,  694,
  187, 1636,  917,  361,  310,  665, 1225,  600,   53, 1745,
 1835,   79,  217,  254,  610,   54,  188,  361,  510,  257,
  542,  187,  141,  190, 1685,  361,   41,  187,  487,  187,
 1642,  258,   24,   51, 1725,  599,  495, 1682,  543,  919,
   55,  361,  600,  161, 1745, 1661,  694,  748,   47,  610,
  187,  177,   49,  217,  254,  694,  217,  254,  947,  665,
 1226,  688, 1223,  150,  684,  693,  694,  696,  193,  187,
  600,  217,  254,  600,  292, 1734, 1224,  610,  187,  257,
  610,  151, 1725,  477,  187,   41,  595,  150,  127,  600,
 1698,   24,  371,  308, 1699,  361,  610,  986,  599,  694,
 1225,  599,  187,  321, 1715,   56,  748,   47,  684,  702,
  318,   49,  986,  694,  703,  600,  712,  599,  354,  711,
  340,  595,  610,  684, 1845,   98, 1725,  733,  351,  293,
  353,  735,  196, 1691, 1725,  196,   99,   57,   25,  187,
  512,   58,  477,  599,  362,  706,  704,  127,  705,  595,
  707,  371,  595,  196, 1226,  355, 1772,  356,  706,  704,
  372,  705, 1763,  707,  294,  151,   99, 1832,  595, 1725,
   48,  150, 1725,  101,  790,  512,  512,  789,  100,  253,
  357,  253,  196, 1753,  996,  424,   50, 1844,  253,  150,
  374,  693,  295,  737,  595,  296, 1725,   25,  987,   45,
  128,  196,  512,  101,  693,  358,  991,  190, 1774,  196,
  196,  297, 1223,  693,  512,  188,  196,  512,  151,   79,
 1775,  991,  116,  376,  150,   46, 1224, 1753,  188,   48,
  217,  377,  510,  150,  196,  380,  188,  298,  187, 1831,
   80,   81,  102,  381,  941,   50,  196,  708,  196,  737,
 1225,  212,  103, 1765,  140,  141,  462,  188,   45,  128,
  693,  204,  205,  188,  151,  188,  470,  510,  510,  693,
  150,  196,  102,  471, 1753,  795,  996,  104,  474,  253,
  150,  116,  103,  193,   46,  475,  188,  706,  704,  217,
  705,  254,  707,  254,  510,  976,  196, 1807,  975,  477,
  254,  253,  886,  310, 1226,  188,  510,  104,  253,  510,
  193,  479,  877,  693,  188,  877,  480, 1108, 1090,  489,
  188, 1089, 1097,  914,  481,  867,  496,  693,  706,  704,
  930,  705,  498,  707,  499,  937,  427,  193,  188,  508,
 1833, 1189,  253, 1836, 1188,  944,  518,  948,  949,  950,
  951,  952,  427,  944, 1131,   14, 1135, 1234, 1143, 1146,
 1089, 1151, 1154, 1157,  877,  524,  908, 1849,  912,  525,
  196,  536,  916,  918,  920,  188,  547,  400,  401,  402,
  539,  737,  489,  702, 1239,  684,  560, 1238,  703,  737,
 1283,  254,  684, 1282, 1288,  684,  702, 1287,  570,  684,
 1398,  703,  253, 1397,  489,  410,  411,  684,  567,  520,
  584,  706,  704,  254,  705,  635,  707,  580,  737,  737,
  254,  737, 1691,  684, 1488,  639,  984, 1188,  737,  737,
  737,  737,  737, 1518,  737,  641,  708,  737,  737,  737,
  737,  737,  643, 1762,  674,  737,  737,  684,  677, 1525,
 1797,  105,  708, 1796,  254,  684, 1808,  706,  704,  675,
  705, 1859,  707,  676, 1770,  678,  961,  984,  984,  984,
  984,  984,  984,  984,  188, 1060, 1061,  684,  123,  123,
  106,  107,  684,  108,  679,  984,  984,  984,  984,  680,
  109,  110,  111,  112,  113,  681,  114,  682,  683,  115,
  116,  117,  118,  119,  684,  685,  686,  120,  121,  691,
   14,  692,  701,  709,  254,  714,  684,  984,  684,  984,
  708,  710,  728,  719,  731,  702,  736,  738,  739,  751,
  703,  760,  755,  756, 1117, 1118, 1122,  762,  763, 1130,
 1132, 1134,  765, 1142, 1145, 1148, 1150, 1153, 1156,  766,
  768, 1160, 1161,  105, 1163,  771, 1165,  527,  774, 1168,
 1169, 1170, 1171, 1172, 1173, 1175,  702,  779,  780,  782,
  783,  703,  792,  816,  823,  817, 1177,  818,  427,  277,
  828,  833,  106,  107,  829,  108,  835,  105,  834,  836,
  838,  839,  109,  110,  111,  112,  113,  844,  114,  850,
  851,  115,  116,  117,  118,  119,  736,  854,  855,  120,
  121,  956,  864,  875,  867,  866,  106,  107,  105,  108,
  876,  877,  489,  881,  932,  942,  109,  110,  111,  112,
  113,  970,  114,  971,  973,  115,  116,  117,  118,  119,
  978,  982,  983,  120,  121,  993, 1000,  106,  107,  702,
  108, 1009, 1012,  675,  703, 1029, 1034,  109,  110,  111,
  112,  113, 1040,  114, 1039,  845,  115,  116,  117,  118,
  119, 1042, 1046, 1723,  120,  121, 1047,  795, 1049, 1055,
 1338,  706,  704, 1056,  705, 1057,  707, 1063,  984, 1067,
  984,  984, 1080, 1078,  675,  702, 1073,  675,  984, 1092,
  703,  734,  706,  704, 1087,  705,  984,  707, 1104, 1105,
 1124,  984,  675,  724,  984, 1162, 1164, 1166, 1174,  501,
 1186, 1723,  702, 1200, 1203, 1206, 1495, 1198,  984, 1220,
 1216, 1222,  711,  984,  984, 1228, 1244,  984, 1255, 1241,
 1264, 1266, 1276, 1286,  984, 1284,  984,  984,  706,  704,
  984,  705, 1293,  707,  984, 1297, 1298,  984, 1299,  984,
  501,  984, 1098,  501, 1300, 1723,  984,  984,  944,  984,
  940,  706,  704, 1813,  705, 1311,  707, 1318,  501, 1309,
 1341, 1342, 1323, 1344, 1346,  984,  984, 1321,  984, 1322,
  669, 1351, 1324,  986,  706,  704, 1330,  705, 1331,  707,
 1335, 1336,  984, 1337,  984,  984,  984, 1339, 1723,  984,
 1364, 1723,  984,  984,  706,  704, 1185,  705, 1371,  707,
  427, 1340,  984,  427, 1347, 1177, 1348, 1349, 1350, 1379,
  427, 1381, 1382, 1352,  669, 1723,  984, 1353, 1354,  984,
 1355, 1356,  105, 1377, 1167,  706,  704, 1357,  705,  669,
  707,  905, 1374,  489, 1383,  984, 1358,  706,  704, 1402,
  705,  984,  707, 1399,  427,  955,  984, 1409, 1410, 1406,
 1407,  106,  107, 1411,  108, 1418, 1408,  984,  984, 1421,
  984,  109,  110,  111,  112,  113,  984,  114, 1430, 1428,
  115,  116,  117,  118,  119, 1423,  562,  424,  120,  121,
 1438, 1442, 1443, 1433,  233, 1444, 1445, 1359,  706,  704,
 1450,  705, 1446,  707, 1447,  984,  984,  965,  984,  702,
 1465, 1455, 1472, 1476,  703,  984,  984,  984,  984,  984,
 1477,  984, 1653,  540,  984,  984,  984,  984,  984, 1481,
  702,  234,  984,  984, 1487,  703, 1332, 1362,  706,  704,
 1508,  705,  489,  707,  586, 1514, 1515, 1520,  965,  965,
  965,  965,  965,  977,  965, 1578, 1521, 1532, 1329, 1588,
 1593,  675, 1591, 1483,  675, 1590,  965,  965,  965,  965,
 1592,  675, 1491,  541,  675, 1595,  702, 1594,  675, 1496,
  956,  703, 1597,  675, 1598, 1599,  675,  501, 1607,  501,
 1609, 1612,  527,  501, 1616, 1617,  501, 1618,  965,  702,
  965,  528,  675, 1620,  703, 1621, 1622,  542,  427,  501,
 1623, 1626,  427, 1627, 1628,  658, 1629, 1630, 1631,  105,
 1632, 1633,  702, 1634, 1635,  543,  675,  703, 1643, 1662,
  675,  489, 1363,  706,  704,  132,  705, 1659,  707,  675,
 1664, 1665,  702, 1670, 1528, 1666, 1667,  703,  106,  107,
 1678,  108, 1668, 1669, 1671, 1672,  675, 1677,  109,  110,
  111,  112,  113, 1680,  114, 1681, 1701,  115,  116,  117,
  118,  119, 1704,  702, 1708,  120,  121, 1709,  703, 1718,
 1735, 1673, 1744,  675, 1782,  702, 1759,  501, 1767, 1771,
  703, 1784, 1787, 1790, 1792,  675, 1799,  675, 1800, 1805,
 1812,  669, 1811,  661, 1779,  160, 1780, 1830,  669,  501,
 1837,  669, 1841, 1770, 1838,  669,  501, 1834, 1853, 1857,
 1858,    1,  625,  669,  203,  752,   29,   31, 1624, 1625,
  368,  271,  661,  661,  148,  661,  702,   81,  522,  669,
  654,  703,  661,  661,  661,  661,  661,  269,  661,  427,
  501,  661,  661,  661,  661,  661,  681,  454,  213,  661,
  661,  587,  522,  669,  948,  276,  396,  947,  193,  965,
  522,  965,  965,   77,  692,  690,  702,  686,  688,  965,
  194,  703,  275, 1663,  289,  369,  709,  965,  287,  708,
  223,  323,  965,  669,  288,  965,  921,  922,  923,  924,
  925,  926,  927,  928,  710,  340,  331,  619,  719,  965,
  329,  965,   27,  988,  965,  965,   30,   39,  965, 1388,
  669,  706,  704,  137,  705,  965,  707,  965,  965,  469,
  478,  965,  669,  468,  669,  965, 1319,  865,  965,  247,
  965,  972,  965, 1103, 1317, 1697, 1305,  965,  965, 1315,
  965, 1600,  549, 1700,  988,  988,  988,  988,  988,  988,
  988, 1601,  830,  594, 1294,  532,  965,  965, 1243,  965,
 1010,  702,  988,  988,  988,  988,  703, 1528,  564,  516,
 1306,  565, 1711,  965, 1044,  965,  965,  965, 1714,  105,
  965,  658, 1716,  965,  965, 1679, 1580, 1778,  811, 1785,
 1822, 1586, 1783,  965,  988, 1432,  988, 1851, 1761, 1742,
 1037, 1717, 1816, 1732, 1235, 1414, 1535,  965,  106,  107,
  965,  108, 1854,  105,  815,  555, 1079,  546,  109,  110,
  111,  112,  113, 1070,  114,  572,  965,  115,  116,  117,
  118,  119,  965, 1462,  852,  120,  121,  965,  139,  427,
  509, 1764,  106,  107, 1001,  108,  746,  750,  965,  965,
 1014,  965,  109,  110,  111,  112,  113,  965,  114,  290,
  995,  115,  116,  117,  118,  119, 1219, 1829, 1707,  120,
  121,  706,  704, 1647,  705, 1798,  707, 1863,  759,  979,
 1212, 1343, 1427,    0, 1376,    0,  965,  965,  105,  965,
    0,    0,    0,    0,    0,    0,  965,  965,  965,  965,
  965,    0,  965,    0,    0,  965,  965,  965,  965,  965,
    0,    0,    0,  965,  965,    0,    0,  106,  107,    0,
  108,  661,    0,    0,    0,    0, 1724,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0,    0,  702,
  661,  661,    0,  661,  703,    0,    0,    0,    0,    0,
  661,  661,  661,  661,  661,  988,  661,  988,  988,  661,
  661,  661,  661,  661, 1724,  988,    0,  661,  661,  706,
  704,  524,  705,  988,  707,    0,    0,    0,  988,    0,
    0,  988,    0, 1365,  706,  704, 1760,  705,  522,  707,
    0,    0,    0,    0,    0,  988,    0,  993,    0,    0,
  988,  988,    0,    0,  988,    0,    0,    0, 1724,    0,
    0,  988,  524,  988,  988,  524, 1814,  988,    0,    0,
    0,  988,    0,    0,  988,    0,  988,    0,  988,  522,
  524,    0,  522,  988,  988,    0,  988,    0,  993,  993,
  993,  993,  993,  993,  993,    0,    0,  522,    0,  522,
    0, 1724,  988,  988, 1724,  988,  993,  993,  993,  993,
    0,    0,    0,    0,    0,    0,   76,    0,  105,  988,
    0,  988,  988,  988,    0,    0,  988,    0, 1724,  988,
  988, 1494,    0,    0,    0,    0,    0,    0,  993,  988,
  993,    0,    0,    0,    0,    0,    0,  106,  107,  702,
  108,    0,    0,  988,  703,    0,  988,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,    0,  988,    0,  120,  121,    0,    0,  988,    0,
 1366,  706,  704,  988,  705,    0,  707,    0,    0,    0,
    0,    0,    0,    0,  988,  988,    0,  988,    0,    0,
    0,    0,    0,  988,    0,    0,    0, 1367,  706,  704,
    0,  705,    0,  707, 1368,  706,  704,    0,  705,    0,
  707,    0, 1369,  706,  704,  519,  705,    0,  707,    0,
    0,    0,  988,  988,    0,  988,    0,    0,    0,    0,
    0,    0,  988,  988,  988,  988,  988,    0,  988,    0,
    0,  988,  988,  988,  988,  988,    0,  702,    0,  988,
  988,    0,  703,    0,    0,    0,  519,    0,    0,  519,
    0,    0,  702,    0, 1370,  706,  704,  703,  705,    0,
  707,    0,    0,    0,  519,    0,  519,    0, 1509,  706,
  704,    0,  705,    0,  707,    0,  522,    0,    0,  524,
    0,  524,    0,    0,    0,  524,    0,    0,  524,  993,
    0,  993,  993,    0,    0,  522,  522,    0,  522,  993,
    0,  524,  522,    0,    0,  522,    0,  993,    0,    0,
    0,    0,  993,    0,    0,  993,    0,    0,  522, 1510,
  706,  704,  524,  705,  522,  707, 1517,  706,  704,  993,
  705,  989,  707,    0,  993,  993,    0, 1246,  993,    0,
    0,    0,    0,    0,    0,  993,    0,  993,  993,    0,
    0,  993,    0,    0,    0,  993,    0,    0,  993,    0,
  993,    0,  993,  524,    0,    0,  524,  993,  993,    0,
  993,    0,  989,  989,  989,  989,  989,  989,  989,  524,
    0,  524,    0,  524,    0,    0,  993,  993,    0,  993,
  989,  989,  989,  989,    0,    0,  522,    0,    0,  702,
    0,  524,    0,  993,  703,  993,  993,  993,  524,    0,
  993,    0,    0,  993,  993,    0,    0,    0,  522,    0,
    0,    0,  989,  993,  989,  522,  702,    0,    0,    0,
    0,  703,    0,  702,  522,    0,  522,  993,  703,    0,
  993,  702,  524,    0,    0,    0,  703,    0,    0,    0,
    0,    0,    0,    0,  522,    0,  993,    0,    0,  522,
    0,    0,  993,  519,    0,    0,    0,  993,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  993,  993,
    0,  993,  519,  519,    0,  519,    0,  993,  522,  519,
    0,    0,  519,  702,    0,    0,    0,    0,  703,    0,
    0,    0,    0,    0,    0,  519,    0,  702,    0,    0,
    0,  519,  703,    0,    0,    0,  993,  993,    0,  993,
    0,    0,    0,    0,    0,  364,  993,  993,  993,  993,
  993,    0,  993,    0,    0,  993,  993,  993,  993,  993,
    0,    0,    0,  993,  993,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  702,    0,
    0,    0,    0,  703,    0,  702,  364,    0,    0,  364,
  703,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  524,    0,    0,  519,  364,    0,    0,  388,    0,    0,
    0,    0,    0,  989,    0,  989,  989,    0,    0,    0,
  524,    0,  524,  989,    0,  519,  524,    0,    0,  524,
    0,  989,  519,    0,    0,  390,  989,    0,    0,  989,
    0,    0,  524,  519,    0,    0,    0,    0,  524,  669,
    0,    0,    0,  989,    0,  846,    0,    0,  989,  989,
    0,  519,  989,    0,  392,    0,  519,    0,    0,  989,
    0,  989,  989,    0,    0,  989,    0,    0,    0,  989,
    0,    0,  989,    0,  989,    0,  989,    0,    0,    0,
  669,  989,  989,  669,  989,  519,  846,  846,  846,  846,
  846,    0,  846,    0,    0,    0,    0,    0,  669,    0,
  989,  989,    0,  989,  846,  846,  846,  846,    0,    0,
  524,    0,    0,    0,    0,    0,    0,  989,    0,  989,
  989,  989,    0,    0,  989,    0,    0,  989,  989,  397,
    0,    0,  524,    0,    0,    0,    0,  989,  846,  524,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
  524,  989,    0,    0,  989,    0,    0,    0,    0,  400,
  401,  402,    0,  204,  205,    0,    0,    0,  524,    0,
  989,    0,    0,  524,    0,    0,  989,    0,    0,    0,
    0,  989,  405,  406,  407,  408,  409,  410,  411,  412,
  413,    0,  989,  989,    0,  989,    0,  415,  416,    0,
    0,  989,  524,    0,    0,    0,  420,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  491,    0,    0,    0,    0,    0,    0,
  989,  989,    0,  989,    0,    0,    0,    0,    0,    0,
  989,  989,  989,  989,  989,    0,  989,    0,    0,  989,
  989,  989,  989,  989,    0,    0,  364,  989,  989,    0,
    0,    0,    0,  364,  491,    0,  364,  491,    0,    0,
  364,    0,    0,    0,    0,    0,    0,    0,  364,    0,
    0,    0,  491,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  364,    0,  524,    0,    0,    0,
  223,    0,    0,    0,   84,    0,    0,  846,    0,  846,
  846,   85,    0,    0,    0,   86,   87,  846,  364,    0,
    0,    0,    0,    0,   88,  846,    0,    0,    0,    0,
  846,    0,    0,  846,    0,    0,   89,  524,    0,    0,
  524,  223,    0,    0,  223,    0,    0,  846,  364,  858,
    0,    0,  846,  846,    0,  524,  846,  669,    0,  223,
  669,    0,    0,  846,    0,  846,  846,  669,    0,  846,
  669,    0,    0,  846,  669,  364,  846,    0,  846,  669,
  846,    0,  669,    0,   62,  846,  846,  364,  846,  364,
  858,  858,  858,  858,  858,   63,  858,    0,  669,    0,
    0,   90,   64,    0,  846,  846,    0,  846,  858,  858,
  858,  858,    0,    0,    0,   65,    0,    0,   91,    0,
    0,  846,  669,  846,  846,  846,  669,    0,  846,    0,
    0,  846,  846,   92,    0,  669,    0,   93,    0,    0,
    0,  846,  858,    0,    0,    0,    0,    0,   94,    0,
    0,    0,  669,    0,    0,  846,    0,    0,  846,    0,
    0,    0,    0,    0,    0,    0,    0,   95,    0,    0,
    0,    0,    0,    0,  846,    0,   66,  251,    0,  669,
  846,    0,    0,    0,    0,  846,    0,    0,    0,    0,
    0,  669,   67,  669,    0,    0,  846,  846,    0,  846,
    0,  491,    0,  491,    0,  846,    0,    0,    0,   68,
  491,    0,    0,    0,    0,    0,    0,    0,  251,    0,
    0,  251,    0,  491,   69,    0,  230,    0,   70,    0,
    0,   71,    0,    0,  846,  846,  251,  846,    0,   72,
    0,    0,    0,    0,  846,  846,  846,  846,  846,    0,
  846,    0,    0,  846,  846,  846,  846,  846,   73,    0,
    0,  846,  846,    0,    0,  862,    0,  230,    0,    0,
  230,    0,    0,    0,  524,    0,  524,    0,  223,    0,
  223,    0,    0,  524,    0,  230,    0,  223,    0,    0,
    0,    0,    0,    0,    0,    0,  524,    0,    0,    0,
    0,  491,    0,    0,    0,    0,  862,  862,  862,  862,
  862,  858,  862,  858,  858,    0,    0,    0,    0,    0,
    0,  858,    0,  491,  862,  862,  862,  862,    0,    0,
  491,    0,    0,    0,  858,    0,    0,  858,    0,    0,
    0,    0,  231,    0,    0,    0,    0,    0,    0,    0,
    0,  858,    0,    0,    0,    0,  858,  858,  862,    0,
  858,    0,    0,    0,  491,    0,    0,  858,    0,  858,
  858,    0,    0,  858,  524,    0,    0,  858,  223,    0,
  858,    0,  858,  231,  858,    0,  231,    0,    0,  858,
  858,    0,  858,    0,    0,    0,  524,    0,    0,    0,
  223,  231,  534,  524,    0,    0,    0,  223,  858,  858,
  672,  858,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  858,    0,  858,  858,  858,
    0,    0,  858,    0,    0,  858,  858,  524,    0,    0,
    0,  223,  534,  534,    0,  858,  534,    0,    0,    0,
    0,  672,    0,    0,  672,  251,    0,  251,    0,  858,
    0,  534,  858,  534,  251,    0,    0,    0,    0,  672,
    0,    0,    0,    0,    0,    0,    0,    0,  858,    0,
    0,    0,    0,    0,  858,    0,    0,    0,    0,  858,
    0,    0,    0,  534,    0,    0,    0,    0,    0,    0,
  858,  858,    0,  858,  242,    0,  230,    0,    0,  858,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  862,    0,  862,
  862,    0,    0,    0,    0,    0,    0,  862,  858,  858,
    0,  858,    0,    0,    0,    0,    0,    0,  858,  858,
  858,  858,  858,  862,  858,  251,    0,  858,  858,  858,
  858,  858,    0,    0,    0,  858,  858,  862,    0,  856,
    0,    0,  862,  862,    0,    0,  862,  251,    0,    0,
    0,    0,    0,  862,  251,  862,  862,    0,    0,  862,
    0,    0,    0,  862,    0,    0,  862,    0,  862,    0,
  862,    0,    0,    0,  242,  862,  862,    0,  862,    0,
  856,    0,  856,  856,  856,    0,    0,    0,  251,    0,
  242,    0,  231,    0,  862,  862,  242,  862,  856,  856,
  856,  856,    0,  242,    0,    0,    0,    0,    0,    0,
    0,  862,    0,  862,  862,  862,    0,    0,  862,    0,
    0,  862,  862,    0,    0,    0,    0,    0,    0,    0,
  534,  862,  856,    0,    0,    0,    0,  242,    0,    0,
    0,    0,    0,    0,    0,  862,  534,    0,  862,  534,
  534,    0,  534,    0,    0,    0,  534,    0,    0,  534,
    0,    0,    0,    0,  862,    0,    0,    0,    0,    0,
  862,    0,  534,    0,    0,  862,    0,    0,  534,    0,
    0,  534,    0,    0,    0,    0,  862,  862,    0,  862,
  242,    0,    0,    0,    0,  862,    0,    0,  672,    0,
    0,  672,    0,    0,    0,    0,    0,    0,  672,    0,
    0,  672,  242,    0,    0,  672,    0,    0,    0,  242,
  672,    0,    0,  672,  862,  862,    0,  862,    0,    0,
    0,    0,    0,    0,  862,  862,  862,  862,  862,  672,
  862,    0,    0,  862,  862,  862,  862,  862,    0,    0,
  534,  862,  862,  242,    0,  855,    0,    0,    0,    0,
    0,    0,    0,  672,    0,    0,    0,  672,    0,    0,
    0,    0,  534,    0,    0,    0,  672,    0,    0,  534,
    0,    0,  562,  534,    0,    0,    0,    0,    0,    0,
  534,    0,    0,  672,    0,    0,  855,    0,  855,  855,
  855,  856,    0,  856,  856,    0,    0,    0,  534,    0,
    0,  856,    0,  534,  855,  855,  855,  855,    0,    0,
  672,    0,    0,    0,    0,    0,    0,  856,    0,    0,
    0,    0,  672,    0,  672,    0,    0,    0,  534,    0,
    0,  856,  534,    0,    0,    0,  856,  856,  855,    0,
  856,    0,    0,    0,    0,    0,    0,  856,    0,  856,
  856,    0,    0,  856,    0,    0,    0,  856,    0,    0,
  856,    0,  856,    0,  856,    0,    0,    0,    0,  856,
  856,    0,  856,    0,    0,    0,    0, 1484, 1485,    0,
    0,    0,  535, 1489, 1490,    0, 1492, 1493,  856,  856,
    0,  856,    0,    0, 1497, 1498, 1499, 1500,    0, 1501,
 1502, 1503, 1504, 1505, 1506,  856,    0,  856,  856,  856,
    0,    0,  856,    0,    0,  856,  856,    0,    0,    0,
    0,    0,  535,  535,    0,  856,  535,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  856,
    0,  535,  856,  535,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  856,    0,
    0,    0,    0,    0,  856,    0,    0,    0,    0,  856,
    0,    0,   98,  535,  106,  107,    0,  108,    0,    0,
  856,  856,    0,  856,  109,  110,  111,  112,  113,  856,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,  855,    0,  855,
  855,    0,    0,   99,    0,    0,    0,  855,  856,  856,
    0,  856,    0,    0,    0,  100,    0,    0,  856,  856,
  856,  856,  856,  855,  856,    0,    0,  856,  856,  856,
  856,  856,    0,    0,    0,  856,  856,  855,    0,  857,
  101,    0,  855,  855,    0,    0,  855,    0,    0,    0,
    0,    0,    0,  855,    0,  855,  855,    0,    0,  855,
    0,    0,    0,  855,    0,    0,  855,    0,  855,    0,
  855,    0,    0,    0,    0,  855,  855,    0,  855,    0,
  857,    0,  857,  857,  857,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  855,  855,    0,  855,  857,  857,
  857,  857,    0,    0,    0,    0,    0,  568,    0,  102,
    0,  855,    0,  855,  855,  855,    0,    0,  855,  103,
    0,  855,  855,    0,    0,    0,    0,    0,    0,    0,
  535,  855,  857,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  104,  855,  535,    0,  855,  535,
  535,    0,  535,    0,    0,    0,  535,    0,    0,  535,
    0,    0,  105,    0,  855,    0,    0,    0,    0,    0,
  855,    0,  535,    0,    0,  855,    0,    0,  535,    0,
    0,  535,    0,    0,    0,    0,  855,  855,    0,  855,
    0,  106,  107,    0,  108,  855,    0,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,    0,    0,    0,  855,  855,    0,  855,    0,    0,
    0,    0,    0,    0,  855,  855,  855,  855,  855,    0,
  855,    0,    0,  855,  855,  855,  855,  855,    0,    0,
  535,  855,  855,    0,    0,  860,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  535,    0,    0,    0,    0,    0,    0,  535,
  105,    0,    0,  535,    0,    0,    0,    0,    0,    0,
  535,    0,    0,    0,    0,    0,  860,    0,  860,  860,
  860,  857,    0,  857,  857,    0,    0,    0,  535,  106,
  107,  857,  108,  535,  860,  860,  860,  860,    0,  109,
  110,  111,  112,  113,    0,  114,    0,  857,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,  535,    0,
    0,  857,  535,    0,    0,    0,  857,  857,  860,    0,
  857,    0,    0,    0,    0,    0,    0,  857,    0,  857,
  857,    0,    0,  857,    0,  677,    0,  857,    0,    0,
  857,    0,  857,    0,  857,    0,    0,    0,    0,  857,
  857,    0,  857,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  529,    0,    0,    0,    0,    0,  857,  857,
    0,  857,    0,    0,    0,    0,  677,    0,    0,  677,
    0,    0,    0,    0,    0,  857,    0,  857,  857,  857,
    0,    0,  857,    0,  677,  857,  857,    0,    0,  459,
    0,    0,    0,  529,    0,  857,  529,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  857,
    0,  529,  857,  529,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  857,    0,
    0,    0,    0,    0,  857,    0,    0,    0,    0,  857,
    0,    0,    0,  529,  106,  107,    0,  108,    0,    0,
  857,  857,    0,  857,  109,  110,  111,  112,  113,  857,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,  860,    0,  860,
  860,    0,    0,    0,    0,    0,    0,  860,  857,  857,
    0,  857,    0,    0,    0,    0,    0,    0,  857,  857,
  857,  857,  857,  860,  857,    0,    0,  857,  857,  857,
  857,  857,    0,    0,    0,  857,  857,  860,    0,    0,
    0,    0,  860,  860,    0,    0,  860,    0,    0,    0,
    0,    0,    0,  860,    0,  860,  860,    0,    0,  860,
    0,    0,    0,  860,    0,    0,  860,    0,  860,    0,
  860,    0,    0,    0,    0,  860,  860,    0,  860,    0,
    0,    0,    0,    0,  658,    0,    0,  421,    0,  422,
    0,    0,    0,    0,  860,  860,    0,  860,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  279,    0,  677,
    0,  860,    0,  860,  860,  860,    0,    0,  860,    0,
    0,  860,  860,    0,    0,    0,    0,    0,    0,    0,
  529,  860,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  860,  529,    0,  860,  529,
  529,    0,  529,    0,    0,    0,  529,    0,    0,  529,
    0,    0,    0,  677,  860,    0,  677,    0,    0,    0,
  860,    0,  529,  677,    0,  860,  677,    0,  529,    0,
  677,  529,    0,    0,    0,  677,  860,  860,  677,  860,
    0,    0,    0,    0,    0,  860,    0,    0,    0,    0,
    0,    0,    0,    0,  677,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  860,  860,    0,  860,  677,    0,
    0,    0,  677,    0,  860,  860,  860,  860,  860,    0,
  860,  677,    0,  860,  860,  860,  860,  860,    0,    0,
  529,  860,  860,    0,    0,    0,    0,    0,  677,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  770,  529,    0,    0,    0,    0,    0,    0,  529,
    0,    0,    0,  529,    0,  677,    0,    0,    0,    0,
  529,    0,    0,    0,    0,    0,    0,  677,    0,  677,
    0,    0,    0,    0,    0,  677,    0,    0,  529,    0,
    0,  385,  770,  529,    0,  770,    0,    0,    0,    0,
  386,    0,    0,    0,    0,    0, 1644,    0,    0,    0,
  770,    0,    0,  387,  677,  677,    0,  677,  529,  388,
  389,    0,  529,    0,  677,  677,  677,  677,  677,    0,
  677, 1645,    0,  677,  677,  677,  677,  677,  652,    0,
  658,  677,  677,  421,    0,  422,    0,  390,    0,    0,
  391,    0,    0,    0,    0,    0,    0,    0,  424,    0,
    0,    0,    0,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  393,  394,
  825,    0,    0,    0,    0,    0,    0,    0,    0, 1646,
  395,    0,    0,    0,  396,    0,    0,    0,    0,    0,
    0,    0,  882,    0,  645,    0,  645,  645,    0,  645,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  825,  645,  105,  825,    0,    0,  645,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  825,
    0,  695,    0,    0,    0,  654,    0,    0,    0,    0,
    0,  397,  106,  107,    0,  108,    0,  398,    0,    0,
    0,    0,  109,  110,  111,  112,  113,  399,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  117,  118,  119,  883,  884,  885,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  385,    0,  770,
    0,    0,  770,    0,    0,    0,  386,    0,    0,  770,
    0,    0,  770,    0,    0,    0,  770,    0,    0,  387,
    0,  770,  651,    0,  770,  388,  389,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  770,    0,    0,    0,  652,    0,    0,    0,    0,    0,
    0,  794,    0,  390,  645,    0,  391,    0,    0,    0,
    0,  645,    0,    0,  770,    0,    0,  770,  770,    0,
  645,    0,  825,    0,    0,    0,    0,  770,    0,    0,
    0,    0,    0,  645,  393,  394,    0,    0,    0,  645,
  645,    0,  794,    0,  770,  794,  395,  645,    0,    0,
  396,  639,    0,  639,  639,    0,  639,    0,  653,    0,
  794,    0,    0,    0,    0,    0,  770,  645,    0,  639,
  645,  770,    0,    0,  639,    0,    0,    0,  825,    0,
    0,  825,    0,  770,    0,  770,    0,    0,  825,    0,
    0,  825,    0,    0,    0,  825,  645,    0,  645,  645,
  825,  654,    0,  825,    0,    0,    0,  397,    0,    0,
  645,    0,    0,  398,  645,    0,    0,    0,    0,  825,
    0,    0,    0,  399,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,  825,    0,    0,  825,  825,    0,    0,
    0,  404,    0,    0,    0,    0,  825,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,  645,  108,  825,    0,  415,  416,  645,  417,  418,
  419,  111,  112,  113,  420,  114,    0,  645,  115,  116,
  655,  656,  657,    0,    0,  825,  120,  121,    0,    0,
  825,  645,  645,  645,  645,  645,  645,    0,    0,    0,
    0,    0,  825,    0,  825,  645,    0,    0,    0,    0,
    0,    0,    0,    0,  645,  645,  645,  645,  645,  645,
  645,  645,  645,  645,  645,    0,  645,    0,    0,  645,
  645,    0,  645,  645,  645,  645,  645,  645,  645,  645,
    0,    0,  645,  645,  645,  645,  645,    0,    0,    0,
  645,  645,    0,  794,    0,    0,    0,    0,    0,    0,
    0,  639,    0,    0,    0,    0,    0,    0,  639,    0,
    0,  658,    0,    0,  421,    0,  422,  639,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  639,    0,   98,    0,  279,    0,  639,  639,    0,    0,
    0,    0,    0,    0,  639,    0,    0,    0,    0,  794,
    0,    0,  794,    0,    0,    0,    0,    0,    0,  794,
    0,    0,  794,    0,  639,    0,  794,  639,    0,    0,
    0,  794,    0,   99,  794,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  100,    0,    0,    0,  463,
  794,    0,    0,  639,    0,  639,  639,  658,    0,  134,
  421,    0,  422,    0,    0,    0,  105,  639,    0,    0,
  101,  639,    0,    0,  794,  424,    0,  794,  794,    0,
  279,    0,    0,    0,    0,    0,    0,  794,    0,    0,
    0,    0,    0,    0,    0,  106,  107,    0,  108,    0,
    0,    0,    0,    0,  794,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,    0,  794,    0,  639,    0,
    0,  794,    0,    0,  639,    0,    0,    0,    0,  102,
    0,    0,    0,  794,  639,  794,    0,    0,    0,  103,
    0,    0,    0,    0,    0,    0,    0,    0,  639,  639,
  639,  639,  639,  639,    0,    0,    0,    0,    0,    0,
    0,    0,  639,    0,  104,    0,    0,    0,    0,    0,
    0,  639,  639,  639,  639,  639,  639,  639,  639,  639,
  639,  639,  105,  639,    0,    0,  639,  639,  385,  639,
  639,  639,  639,  639,  639,  639,  639,  386,    0,  639,
  639,  639,  639,  639,    0,    0,    0,  639,  639,    0,
  387,  106,  107,  105,  108,    0,  388,  389,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,  652,    0,    0,  120,  121,
    0,    0,  106,  107,  390,  108,    0,  391,    0,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,  385,  393,  394,  658,    0,    0,
  421,    0,  422,  386,    0,    0,    0,  395,    0,    0,
    0,  396,    0,    0,    0,  424,  387,    0,    0,  878,
  279,    0,  388,  389,    0,  940,  706,  704,    0,  705,
    0,  707,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  652,    0,    0,  903,  902,  904,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,  654,    0,    0,    0,    0,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
    0,  393,  394,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  396,  400,  401,
  402,  403,  204,  205,    0,  653,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,  654,  115,
  116,  655,  656,  657,  397,    0,    0,  120,  121,    0,
  398,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,  551,  108,
    0,  959,  415,  416,  385,  417,  418,  419,  111,  112,
  113,  420,  114,  386,    0,  115,  116,  655,  656,  657,
    0,    0,    0,  120,  121,    0,  387,    0,    0,    0,
    0,    0,  388,  389,    0,  483,  890,    0,  421,  551,
  422,    0,  551,    0,  702,    0,    0,    0,    0,  703,
    0,  652,  891,  424,    0,    0,    0,  551,  279,  551,
  390,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  960,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  892,  551,
    0,  393,  394,    0,    0,    0,    0,  893,    0,    0,
    0,    0,    0,  395,    0,  894,    0,  396,    0,    0,
    0,  961,    0,    0,    0,  878,  962,    0,    0,    0,
    0,    0,  963,    0,  895,    0,  896,    0,    0,    0,
    0,    0,  483,  105,    0,  421,    0,  422,    0,    0,
  897,  964,  898,  899,  900,    0,    0,    0,    0,    0,
  424,  959,    0,    0,    0,  279,    0,    0,  654,    0,
    0,    0,  106,  107,  397,  108,    0,    0,    0,    0,
  398,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  399,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,  901,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121, 1119,  551,    0, 1120,    0,
    0,    0,  385,    0,    0,    0,  962,    0,    0,    0,
    0,  386,  551,    0,    0,    0,  551,    0,  551,    0,
    0,    0,  551,  105,  387,  551,    0,    0,    0,    0,
  388,  389,    0,    0,    0,    0,    0,    0,  551,    0,
    0,    0,    0,    0,  551,    0,    0,  551,    0,    0,
    0,    0,  106,  107,    0,  108,    0,    0,  390,    0,
    0,  391,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,    0,    0,    0,    0,  393,
  394,    0, 1129,  541,    0, 1120,    0,    0,    0,  385,
    0,  395,    0,    0,    0,  396,    0,    0,  386,    0,
    0,    0,    0,    0,    0,    0,  551,    0,    0,    0,
    0,  387,    0,    0,    0,    0,    0,  388,  389,    0,
  483,    0,    0,  421,  541,  422,    0,  541,  551,    0,
    0,    0,    0,    0,    0,  551,    0,    0,  424,  551,
    0,    0,  541,  279,  541,  390,  551,    0,  391,    0,
 1121,    0,  397,    0,    0,    0,    0,    0,  398,    0,
    0,    0,    0,    0,  551,    0,    0,    0,  399,  551,
    0,    0,    0,    0,  541,    0,  393,  394,    0,    0,
    0,    0,  400,  401,  402,  403,  204,  205,  395,    0,
    0,    0,  396,    0,  551,    0,  404,    0,  551,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0, 1121,    0,  397,
    0,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,  483,    0,    0,  421,    0,  422,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
  424,    0,    0,  404,    0,  279,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
 1133,  541,    0, 1120,    0,    0,    0,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  386,  541,    0,    0,
    0,  541,    0,  541,    0,    0,    0,  541,  567,  387,
  541,    0,    0,    0,    0,  388,  389,    0,    0,    0,
    0,  483,    0,  541,  421,    0,  422,    0,    0,  541,
    0,    0,  541,    0,    0,    0,    0,  567,  567,  424,
  567,    0,    0,  390,  279,    0,  391,  567,  567,  567,
  567,  567,    0,  567,    0,  281,  567,  567,  567,  567,
  567,    0,    0,    0,  567,  567,    0,    0,    0,    0,
    0,    0,    0,    0,  393,  394,    0,    0,    0,    0,
    0,    0,    0,    0,  281,  281,  395,  281,    0,    0,
  396,    0,    0,    0,  281,  281,  281,  281,  281,    0,
  281,  541,    0,  281,  281,  281,  281,  281,    0,    0,
    0,  281,  281,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  541,    0,    0,    0,    0,    0,    0,
  541,    0, 1141,    0,  541, 1120,    0,    0,    0,  385,
    0,  541,    0,    0,    0, 1121,    0,  397,  386,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,  541,
    0,  387,    0,  399,  541,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,  541,
    0,  404,    0,  541,    0,  390,    0,    0,  391,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  393,  394,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,  395,    0,
    0, 1144,  396,    0, 1120,    0,    0,    0,  385,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387,    0,    0,    0,    0,    0,  388,  389,    0,    0,
  483,    0,    0,  421,    0,  422,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1121,  424,  397,
    0,    0,    0,  279,  390,  398,    0,  391,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,  393,  394,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  395,    0,    0,
    0,  396,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0, 1430,    0,  120,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1121,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    6,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0, 1543, 1544,    0,
 1545,    0,  483, 1546,    0,  421,    0,  422,  400,  401,
  402,  403,  204,  205,    0,    0,    0, 1547,    0,    0,
  424,    0,  404, 1548,    0,  279,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
 1149,    0,    0, 1120,    0,    0,    0,  385,    0,    0,
    0,    0,    0, 1549,    0,    0,  386,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  387,
    0,    0,   14,    0,    0,  388,  389,    0,    0,    0,
  661,  483,    0,    0,  421,    0,  422, 1550,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,  424,
    0,    0,    0,  390,  279,    0,  391,    0, 1551,  661,
  661,    0,  661,    0,    0,  105,    0,    0,    0,  661,
  661,  661,  661,  661,    0,  661,    0,    0,  661,  661,
  661,  661,  661,    0,  393,  394,  661,  661,    0,    0,
    0,    0,    0,    0, 1552,  107,  395,  108,    0,    0,
  396,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115, 1553,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1152,    0,    0, 1120,    0,    0,    0,  385,
    0,    0,    0,    0,    0, 1121,    0,  397,  386,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,    0,
    0,  387,    0,  399,    0,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,  390,    0,    0,  391,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  393,  394,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,  395,    0,
    0, 1155,  396,    0, 1120,    0,    0,    0,  385,    0,
    0,  483,    0,  488,  421,    0,  422,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  387,    0,    0,    0,  279,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  706,  704,    0,  705, 1121,  707,  397,
    0,    0,    0,    0,  390,  398,    0,  391,    0,    0,
    0,  903,  902,  904,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,  393,  394,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  395,    0,    0,
    0,  396,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1121,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,  385,  417,
  418,  419,  111,  112,  113,  420,  114,  386,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,  483,
  387,    0,  421,    0,  422,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,  279,  890,    0,    0,    0,    0,    0,    0,
    0,  702,    0,    0,  390,    0,  703,  391,    0,  891,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  392,    0,  393,  394,    0,    0,    0,
    0,    0,    0,    0,    0,  892,    0,  395,    0,    0,
    0,  396,    0,    0,  893,  959,    0,  959,  959,    0,
  959,    0,  894,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  959,    0,    0,    0,    0,  959,    0,
    0,  895,    1,  896,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  897,    0,  898,
  899,  900,    0,    2,    0,    0,    0,    0,  397,    0,
    0,    3,    0,    0,  398,    0,    0,    0,    4,    0,
    5,    0,    6,    0,  399,    0,    0,    7,    0,    0,
    0,    0,    0,    0,    8,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    9,    0,
  901,    0,  404,    0,    0,    0,    0,    0,    0,   10,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,  385,  120,  121,    0,
    0,    0,    0,    0,    0,  386,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  387,    0,
    0,    0,    0,    0,  388,  389,   11,    0,    0,    0,
    0,    0,    0,   12,    0,    0,   13,    0,   14,   15,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,  795,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  392,  959,  393,  394,    0,    0,    0,    0,    0,
    0,  959,    0,    0,    0,  395,    0,    0,    0,  396,
  795,    0,  645,  795,  959,  645,    0,  645,    0,    0,
  959,  959,   17,    0,    0,    0,    0,    0,  795,    0,
  645,    0,    0,    0,    0,  645,    0,   18,   19,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  959,    0,
    0,  959,    0,    0,    0,    0,  695,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  397,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,  959,    0,  959,
  959,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,  959,    0,    0,    0,  959,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,  526,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,  959,    0,  526,  120,  121,  526,  959,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  959,    0,
    0,    0,  526,    0,  526,    0,    0,    0,    0,    0,
    0,    0,  959,  959,  959,  959,  959,  959,    0,    0,
    0,    0,    0,    0,    0,    0,  959,    0,    0,    0,
    0,    0,    0,    0,  526,  959,  959,  959,  959,  959,
  959,  959,  959,  959,  959,  959,    0,  959,    0,    0,
  959,  959,    0,  959,  959,  959,  959,  959,  959,  959,
  959,  795,  645,  959,  959,  959,  959,  959,    0,  645,
    0,  959,  959,    0,    0,    0,    0,    0,  645,    0,
    0,  423,    0,    0,  421,    0,  422,    0,    0,    0,
    0,  645,    0,    0,    0,    0,    0,  645,  645,  424,
    0,    0,    0,    0,  279,  645,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  795,    0,    0,
  795,    0,    0,    0,    0,  645,    0,  795,  645,    0,
  795,    0,    0,    0,  795,    0,    0,    0,    0,  795,
    0,    0,  795,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  645,  645,  795,    0,
    0,  483,    0,    0,  421,    0,  422,    0,  645,    0,
    0,    0,  645,    0,    0,    0,    0,    0,    0,  424,
    0,    0,  795,    0,  279,  795,  795,    0,    0,    0,
    0,    0,    0,    0,    0,  795,    0,    0,    0,    0,
    0,  526,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  795,    0,    0,    0,    0,  526,    0,    0,
    0,  526,    0,  526,    0,    0,    0,  526,    0,  645,
  526,    0,    0,    0,  795,  645,    0,    0,    0,  795,
    0,    0,    0,  526,    0,  645,    0,    0,    0,  526,
    0,  795,  526,  795,    0,    0,    0,    0,    0,  645,
  645,  645,  645,  645,  645,    0,    0,    0,    0,    0,
    0,    0,    0,  645,    0,    0,    0,    0,    0,    0,
    0,    0,  645,  645,  645,  645,  645,  645,  645,  645,
  645,  645,  645,    0,  645,    0,    0,  645,  645,    0,
  645,  645,  645,  645,  645,  645,  645,  645,  385,    0,
  645,  645,  645,  645,  645,    0,    0,  386,  645,  645,
    0,  526,    0,    0,    0,    0,    0,    0,    0,    0,
  387,    0,    0,    0,    0,    0,  388,  389,    0,    0,
    0,    0,    0,  526,    0,    0,    0,    0,    0,    0,
  526,    0,    0,    0,  526,    0,    0,    0,    0,    0,
    0,  526,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  526,
    0,    0,    0,    0,  526,    0,    0,    0,  385,    0,
    0,    0,    0,  392,    0,  393,  394,  386,    0,    0,
  483,    0,    0,  421,    0,  422,    0,  395,    0,  526,
  387,  396,    0,  526,    0,    0,  388,  389,  424,    0,
    0,    0,    0,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  397,    0,
    0,    0,    0,    0,  398,  393,  394,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  395,    0,    0,
    0,  396,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,  397,  115,
  116,  117,  118,  119,  398,    0,    0,  120,  121,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,  687,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,  385,    0,  115,
  116,  117,  118,  119,    0,    0,  386,  120,  121,  483,
    0,    0,  421,    0,  422,    0,    0,    0,    0,  387,
    0,    0,    0,    0,    0,  388,  389,  424,    0,    0,
    0,    0,  279,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,  394,    0,    0,    0,  483,
  943,    0,  421,    0,  422,    0,  395,    0,    0,    0,
  396,    0,    0,    0,    0,    0,    0,  424,    0,    0,
  105,    0,  279,    0,    0,    0,    0,    0,    0,  164,
  165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
  175,  176,  177,  178,  179,    0,    0,  180,  181,  106,
  107,    0,  108,    0,    0,  182,  183,  695,    0,  109,
  110,  111,  112,  113,  184,  114,  185,  397,  115,  116,
  117,  118,  119,  398,    0,    0,  120,  121,    0,    0,
    0,    0,    0,  399,    0,    0,    0,  221,    0,    0,
    0,  221,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,  221,    0,    0,    0,  221,
    0,  404,  221,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  385,    0,  115,  116,
  117,  118,  119,  221,    0,  386,  120,  121,    0,  221,
  221,  221,    0,  221,  221,  221,    0,  221,  387,    0,
    0,    0,    0,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,  221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  221,    0,    0,    0,    0,   80,  385,    0,    0,    0,
    0,  392,    0,  393,  394,  386,    0,  221,  483,    0,
    0,  421,    0,  422,    0,  395,    0,    0,  387,  396,
    0,    0,    0,    0,  388,  389,  424,    0,    0,  221,
    0,  279,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,  221,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  397,    0,    0,    0,
    0,  221,  398,  393,  394,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  395,    0,    0,    0,  396,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,  397,  115,  116,  117,
  118,  119,  398,    0,    0,  120,  121,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,  385,    0,  115,  116,  117,
  118,  119,    0,    0,  386,  120,  121,  483,    0,    0,
  421,    0,  422,    0,    0,    0,    0,  387,    0,    0,
    0,    0,    0,  388,  389,  424,    0,    0,    0,    0,
  279,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  390,    0,    0,  391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  393,  394,    0,    0,    0,  483,    0,    0,
  421,    0,  422,    0,  395,    0,    0,    0,  396,    0,
    0,  450,    0,    0,    0,  424,    0,    0,    0,    0,
  279,  450,  450,  450,  450,  450,  450,  450,  450,  450,
  450,  450,  450,  450,  450,  450,    0,    0,  450,  450,
  450,  450,    0,  450,    0,    0,  450,  450,    0,    0,
  450,  450,  450,  450,  450,  450,  450,  450,    0,  450,
  450,  450,  450,  450,    0,  397,    0,  450,  450,    0,
    0,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,    0,  868,    0,    0,    0,  596,
    0,    0, 1147,    0,    0,  400,  401,  402,  403,  204,
  205,    0,    0,  869,    0,    0,    0,  597,    0,  404,
  598,    0,    0,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,  385,    0,  115,  116,  117,  118,
  119,  599,    0,  386,  120,  121,    0,  600,  601,  602,
    0,  603,  604,  605,    0,  606,  387,    0,    0,    0,
    0,    0,  388,  389,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  607,    0,  608,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  609,    0,
    0,    0,    0,    0,  385,    0,    0,    0,    0,    0,
    0,  393,  394,  386,    0,  610,  483,    0,    0,  421,
    0,  422,    0,  395,    0,    0,  387,  396,    0,    0,
    0,    0,  388,  389,  424, 1713,    0,  611,    0,  279,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,  612,  613, 1332,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  397,    0,    0,    0,    0,  614,
  398,  393,  394,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  395,    0,    0,    0,  396,    0,    0,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,  397,  115,  116,  117,  118,  119,
  398,    0,    0,  120,  121,    0,    0,    0,    0,    0,
  399,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1345,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  385,    0,  115,  116,  117,  118,  119,
    0,    0,  386,  120,  121,  483,    0,    0,  421,    0,
  422,    0,    0,    0,    0,  387,    0,    0,    0,    0,
    0,  388,  389,  424,    0,    0,    0,    0,  279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  393,  394,    0,    0,    0,  959,    0,    0,  959,    0,
  959,    0,  395,    0,    0,    0,  396,    0,    0,    0,
  221,    0,    0,  959,  221,    0,    0,    0,  959,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  221,    0,
    0,    0,  221,    0,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  397,    0,    0,    0,    0,    0,  398,
    0,    0,    0,    0,    0,    0,  221,    0,    0,  399,
    0,    0,  221,  221,  221,    0,  221,  221,  221,    0,
  221,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
  221,    0,  221,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,  385,  221,  115,  116,  117,  118,  119,    0,
    0,  386,  120,  121,    0,    0,    0,    0,    0,    0,
  221,    0,    0,    0,  387,    0,    0,    0,    0,    0,
  388,  389,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,  391,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  835,    0,    0,    0,    0,  221,  221,    0,
    0,    0,  959,    0,    0,    0,    0,    0,    0,  393,
  394,  959,    0,    0,  221,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  959,  396,    0,    0,    0,    0,
  959,  959,    0,  835,  997,  997,  835,  997,  997,  997,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  835,  997,  997,  997,    0,    0,    0,  959,    0,
    0,  959,    0,    0,    0,    0,  819,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  397,  997,    0,    0,    0,    0,  398,  959,
  959,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  959,    0,    0,    0,  959,    0,  819,    0,    0,
  819,    0,  400,  401,  402,  403,  204,  205,    0,    0,
    0,    0,    0,    0,    0,  819,  404,    0,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,    0,  959,  115,  116,  117,  118,  119,  959,    0,
    0,  120,  121,    0,    0,    0,    0,    0,  959,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  959,  959,  959,  959,  959,  959,    0,    0,
    0,    0,    0,    0,    0,    0,  959,    0,    0,    0,
    0,    0,    0,    0,  836,  959,  959,  959,  959,  959,
  959,  959,  959,  959,  959,  959,    0,  959,    0,    0,
  959,  959,    0,  959,  959,  959,  959,  959,  959,  959,
  959,    0,    0,  959,  959,  959,  959,  959,    0,    0,
    0,  959,  959,    0,  835,  836,  998,  998,  836,  998,
  998,  998,    0,    0,  997,    0,    0,    0,    0,    0,
    0,    0,  997,  836,  998,  998,  998,  997,    0,    0,
  997,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  837,    0,
    0,    0,    0,    0,    0,  998,    0,    0,    0,    0,
  835,    0,    0,  835,    0,    0,  997,    0,    0,    0,
  835,    0,    0,  835,    0,  997,    0,  835,  819,    0,
    0,    0,  835,  997,    0,  835,    0,    0,    0,  837,
  999,  999,  837,  999,  999,  999,    0,    0,    0,    0,
    0,  835,  997,    0,  997,    0,    0,  837,  999,  999,
  999,    0,    0,    0,    0,    0,    0,    0,  997,    0,
  997,  997,  997,    0,    0,  835,    0,    0,  835,  835,
    0,    0,  798,    0,  819,    0,    0,  819,  835,  999,
    0,  595,    0,    0,  819,  596,    0,  819,    0,    0,
    0,  819,    0,    0,    0,  835,  819,    0,    0,  819,
    0,    0,    0,  597,    0,    0,  598,    0,    0,    0,
    0,  997,    0,  798,    0,  819,  798,  835,    0,    0,
    0,    0,  835,    0,    0,    0,    0,    0,    0,    0,
    0,  798,    0,    0,  835,    0,  835,    0,    0,  819,
    0,    0,  819,  819,  796,    0,    0,  599,    0,    0,
    0,    0,  819,  600,  601,  602,    0,  603,  604,  605,
    0,  606,    0,    0,    0,    0,  836,    0,    0,  819,
    0,    0,    0,    0,    0,    0,  998,    0,    0,    0,
    0,  607,    0,  608,  998,  796,    0,    0,  796,  998,
    0,  819,  998,    0,    0,    0,  819,    0,    0,    0,
    0,    0,    0,  796,    0,    0,    0,    0,  819,    0,
  819,    0,    0,    0,  609,    0,    0,    0,  797,    0,
    0,    0,  836,    0,    0,  836,    0,    0,  998,    0,
    0,  610,  836,    0,    0,  836,    0,  998,    0,  836,
    0,    0,    0,    0,  836,  998,    0,  836,    0,    0,
  837,    0,  827,  611,    0,    0,    0,    0,    0,  797,
  999,    0,  797,  836,  998,    0,  998,    0,  999,    0,
    0,    0,    0,  999,    0,    0,  999,  797,    0,    0,
  998,  791,  998,  998,  998,    0,    0,  836,  612,  613,
  836,  836,    0,  827,    0,    0,  827,    0,    0,    0,
  836,    0,  792,    0,    0,  614,  837,    0,    0,  837,
    0,  827,  999,    0,    0,    0,  837,  836,    0,  837,
    0,  999,  791,  837,    0,  791,    0,    0,  837,  999,
    0,  837,    0,  998,  798,    0,    0,    0,    0,  836,
  791,    0,    0,  792,  836,    0,  792,  837,  999,    0,
  999,    0,    0,    0,    0,    0,  836,    0,  836,    0,
    0,  792,    0,    0,  999,    0,  999,  999,  999,    0,
    0,  837,    0,    0,  837,  837,    0,    0,    0,    0,
    0,    0,    0,    0,  837,    0,    0,    0,    0,  793,
  798,    0,    0,  798,    0,    0,    0,    0,    0,    0,
  798,  837,    0,  798,    0,    0,  796,  798,    0,    0,
    0,    0,  798,    0,    0,  798,    0,  999,    0,    0,
    0,    0,  826,  837,    0,    0,    0,    0,  837,    0,
  793,  798,    0,  793,    0,    0,    0,    0,    0,    0,
  837,    0,  837,    0,    0,    0,    0,    0,  793,    0,
    0,    0,    0,    0,    0,  798,    0,    0,  798,  798,
    0,    0,  796,  826,    0,  796,  826,    0,  798,    0,
    0,    0,  796,    0,    0,  796,    0,    0,    0,  796,
  797,  826,    0,    0,  796,  798,    0,  796,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  796,    0,    0,    0,  798,    0,    0,
    0,    0,  798,    0,  827,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  798,    0,  798,  796,    0,    0,
  796,  796,    0,    0,    0,    0,  797,    0,    0,  797,
  796,    0,    0,  791,    0,    0,  797,    0,    0,  797,
    0,    0,    0,  797,    0,    0,    0,  796,  797,  820,
    0,  797,    0,    0,  792,    0,    0,    0,    0,    0,
  827,    0,    0,  827,    0,    0,    0,  797,    0,  796,
  827,    0,    0,  827,  796,    0,    0,  827,  828,    0,
    0,    0,  827,    0,    0,  827,  796,    0,  796,  791,
  820,  797,  791,  820,  797,  797,    0,    0,    0,  791,
    0,  827,  791,    0,  797,    0,  791,    0,  820,    0,
  792,  791,  829,  792,  791,    0,    0,    0,    0,  828,
  792,  797,  828,  792,    0,  827,    0,  792,  827,  827,
  791,    0,  792,    0,    0,  792,    0,  828,  827,    0,
    0,  793,    0,  797,    0,    0,    0,    0,  797,    0,
    0,  792,    0,  829,  791,  827,  829,  791,  791,    0,
  797,    0,  797,    0,    0,    0,    0,  791,    0,    0,
    0,  829,    0,    0,  826,  792,  817,  827,  792,  792,
    0,    0,  827,    0,  791,    0,    0,    0,  792,    0,
    0,    0,    0,    0,  827,    0,  827,  793,    0,    0,
  793,    0,    0,    0,    0,  792,  791,  793,    0,    0,
  793,  791,    0,    0,  793,    0,    0,  817,    0,  793,
  817,    0,  793,  791,    0,  791,    0,  792,    0,    0,
  826,    0,  792,  826,    0,  817,    0,    0,  793,    0,
  826,    0,    0,  826,  792,    0,  792,  826,    0,    0,
  821,    0,  826,    0,    0,  826,    0,    0,    0,    0,
    0,    0,  793,    0,    0,  793,  793,    0,    0,    0,
    0,  826,    0,    0,    0,  793,    0,    0,    0,  830,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  821,  793,    0,  821,  826,    0,    0,  826,  826,
    0,    0,    0,    0,    0,    0,    0,    0,  826,  821,
    0,  820,    0,  818,  793,    0,    0,    0,    0,  793,
  830,    0,    0,  830,  822,  826,    0,    0,    0,    0,
    0,  793,    0,  793,    0,    0,    0,    0,  830,    0,
  828,    0,    0,    0,    0,    0,    0,  826,    0,    0,
    0,    0,  826,    0,  818,    0,    0,  818,    0,    0,
    0,    0,    0,    0,  826,  822,  826,  820,  822,    0,
  820,    0,  818,    0,  829,    0,    0,  820,    0,    0,
  820,    0,    0,  822,  820,    0,    0,    0,    0,  820,
    0,    0,  820,    0,    0,    0,  828,    0,    0,  828,
    0,    0,    0,    0,    0,    0,  828,    0,  820,  828,
    0,    0,    0,  828,    0,    0,    0,    0,  828,    0,
    0,  828,    0,    0,    0,    0,    0,    0,    0,    0,
  829,    0,  820,  829,    0,  820,  820,  828,  817,    0,
  829,    0,    0,  829,    0,  820,    0,  829,    0,    0,
    0,    0,  829,    0,    0,  829,    0,    0,    0,    0,
    0,  828,  820,    0,  828,  828,    0,    0,    0,    0,
    0,  829,    0,    0,  828,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  820,    0,    0,    0,    0,  820,
    0,  828,    0,    0,  817,  829,    0,  817,  829,  829,
    0,  820,    0,  820,  817,    0,    0,  817,  829,    0,
    0,  817,  821,  828,    0,    0,  817,    0,  828,  817,
    0,    0,    0,    0,    0,  829,    0,    0,    0,    0,
  828,    0,  828,    0,    0,  817,    0,    0,    0,    0,
    0,  830,    0,    0,    0,    0,    0,  829,    0,    0,
    0,    0,  829,    0,    0,    0,    0,    0,    0,  817,
    0,    0,  817,  817,  829,    0,  829,    0,  821,    0,
    0,  821,  817,    0,    0,  818,    0,    0,  821,    0,
    0,  821,    0,    0,    0,  821,  822,    0,    0,  817,
  821,    0,    0,  821,    0,    0,    0,  830,    0,    0,
  830,    0,    0,    0,    0,    0,    0,  830,    0,  821,
  830,  817,    0,    0,  830,    0,  817,    0,    0,  830,
    0,    0,  830,    0,    0,    0,    0,    0,  817,    0,
  817,  818,    0,  821,  818,    0,  821,  821,  830,    0,
    0,  818,  822,    0,  818,  822,  821,    0,  818,    0,
    0,    0,  822,  818,    0,  822,  818,    0,    0,  822,
    0,    0,  830,  821,  822,  830,  830,  822,    0,    0,
    0,  321,  818,    0,    0,  830,    0,    0,    0,    0,
    0,    0,    0,  822,    0,  821,    0,    0,    0,    0,
  821,    0,  830,    0,    0,    0,  818,    0,    0,  818,
  818,  321,  821,    0,  821,    0,    0,  822,  321,  818,
  822,  822,  321,  321,  830,  321,    0,    0,  321,  830,
  822,    0,    0,    0,    0,    0,  818,    0,    0,    0,
    0,  830,  321,  830,    0,    0,    0,  822,  321,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  818,    0,
    0,    0, 1430,  818,    0,    0,    0,    0,    0,  822,
    0,    0,    0,    0,  822,  818,    0,  818,    0,    0,
    0,    0,    0,    0,    0,    0,  822,    0,  822,    0,
    0,    0,    6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1543, 1544,    0, 1545,    0,  321, 1546,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1547,    0,    0,    0,  321,    0, 1548,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  321,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  321,    0,    0,    0,    0,    0,    0,    0,
    0,  321,    0,  321,    0,    0,    0,    0,    0,    0,
  321,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  595,    0,    0,    0,  596,    0,    0, 1549,
    0,    0,    0,  795,    0,    0,    0,    0,    0,  321,
  321,    0,  321,    0,  597,    0,    0,  598,   14,  321,
  321,  321,  321,  321,    0,  321,    0,    0,  321,  321,
  321,  321,  321, 1550,    0,    0,  321,  321,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0, 1673,    0, 1551,    0,    0,    0,  599,    0,
    0,  105,    0,    0,  600,  601,  602,    0,  603,  604,
  605,    0,  606,  595,    0,    0,    0,  596,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1552,  107,  607,  108,  608,  597,    0,    0,  598,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
 1553,  117,  118,  119,    0,  133,    0,  120,  121,  133,
    0,    0,    0,    0,    0,  609,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  133,    0,  599,
  133,    0,  610,    0,    0,  600,  601,  602,    0,  603,
  604,  605,    0,  606,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  611,    0,    0,    0,    0,    0,
    0,    0,    0,  607,    0,  608,    0,    0,    0,  105,
    0,  133,    0,    0,    0,    0,    0,  133,  133,  133,
    0,  133,  133,  133,    0,  133,    0,    0,    0,  612,
  613,    0,    0,    0,    0,    0,  609,    0,  106,  107,
    0,  108,    0,    0,    0,  133,  614,  133,  109,  110,
  111,  112,  113,  610,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,    0,    0,    0,    0,  611,    0,    0,  133,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,  133,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  612,  613,    0,    0,    0,    0,    0,  133,    0,  106,
  107,    0,  108,    0,    0,    0,    0,  614,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,  133,  133,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  133,
};
static short yycheck[] = {                                       9,
   16,   55,   12,   64,    4,    4,   16,  104,   18,    0,
  140,  141,  287,   63,  256,   56,  140,  141,  513,  355,
  710,   57,  363,  363,   85,  151,   67,  334,  554,   63,
  332,  557,   94,   69,  496,   89,   73,  563,  423, 1221,
  359,  563,  520,   53,   54,   55,   56,   57,   58,   90,
   62,  687,  695,   63,   64,   65,   97,   67,   95,   69,
   70,  987,   72,   73,  939,   70,  756,   83,  675, 1042,
 1263,   81,  471,   83,   84,   85,   92,   87,   88,   89,
   90,  724,   92,   93,   94,   95,  380,   97,   93,  770,
  368,    0,  154, 1035,   88,  763,  132,  131,  308,  133,
 1042,   91,  977, 1035,    0,  264,  709,  862,    0,    0,
 1042,  152,    0,    0,  273,    0, 1043,  153, 1035, 1035,
    0,  131,  132,  133,  792, 1042, 1042, 1054,  647, 1035,
  831,  228,   41,  801,  728,  145, 1042,    0,  327,    0,
   44,  151,  152,  153,  154,   44,   40, 1074,  158,   41,
   59,  145,   44,  760,   41,  640,   41,   44,  987,   44,
   44,   41, 1091,   59,   44, 1068,  281,   59,   59,   63,
   40,   59,   59,   59,   59, 1470,   45,  189,  728,   59,
  766,  423,  308,   45, 1205,   40,  219,  855,    0,  158,
   42,    0,   41,   63,    0,  653,   59,   40,   59,   44,
    0,  477,  214,  271,  540,  541,  542,  543,   44,   41,
   44,  530, 1311,   44, 1381,   40,    0, 1092,  315, 1401,
    0, 1562,  563,  563,  258,   44,  262,   41, 1198, 1396,
 1200, 1261,   44,   44,    0,  271,    0,  515,  292,    0,
   63,  357,  737,   41,    0,   44,   41,   59,  258,  161,
   59,   45,  262,   59,  264,  265,  348,  268,  262,   59,
  296,  271,  260,  273,  566,  357,  283,  274,    0,  905,
  906,  590,  307,  308,  308,   59,   91,   41,  311,   59,
   44,  320,  292,    0,   12,  273,  296, 1704,   41,  337,
   41,   44,  818,   59,    0,   59, 1691,   45,   59,  333,
  936,  595,   41,   59,  463,  445,  340, 1086,  342,  455,
  344,    0,  405,  347,  616,    0, 1289,  300,   44,    0,
   41,  262,  376,  333,  365,    0,  374,   59,  277,  402,
  340,   40,  342,  260,  344,   41,   45,  347,  374,  387,
 1422, 1009,   59,   44,  356,  471,  358, 1289,  301, 1744,
   41, 1278,  340,   59,  365,  365,  366, 1289,    0,  387,
  372,  366,  278,  456,  374,   41,  376,   93,  508,  352,
   59,  978, 1289, 1289,   59, 1474,  970,  839,   59, 1796,
  380,  355,  386, 1289,   59, 1053,   45, 1469,  982,  983,
  377, 1584, 1295, 1222, 1689, 1790,  345,  436, 1034,   41,
  675,  517,   44,  319,   63, 1435,    0,  319,  260,  318,
  138,  479,  440,  262,  424,    0,  451,   59,  279,  417,
  970,  462,  439,  459,  882,  517,  318,  307,  308, 1399,
  264,  340,  982,    0,  470, 1776,  477,  471,  474,  300,
  331,   61,    0,  479,  269,  386,   41,  320,  340,  459,
  692,  303,  462,  463, 1233,  731,   41,  581,    0,  466,
  470,  448,  277,  738,  474,   59,  708,  477,  329,  479,
 1811, 1812,  482,  304,   59,  320, 1112, 1113,  780, 1115,
 1116,  783,  318,   41,  400,  760,  318,  348, 1681,  442,
  417,    0,   59,  304,  258,  797,  357, 1838,  383, 1426,
 1521,   59,  363,  259, 1431, 1206, 1433,  271,  264, 1850,
 1096,  309, 1853,  482,  270,  279,  328,   59,  492,  331,
  381, 1124,  824,  535,  410,  441,   41,  560,    0,  563,
  410,  343,  264, 1700,  318,   44,  300,  386, 1467,  394,
  436, 1468,  306, 1736,  308,  454, 1301,  757,  328,  440,
   59,  331,  562,  563,  458,  454,  340, 1238,  568,  866,
  719,    0,  454,  454,  458,  329,  454,  367,    0,   41,
  454,  451,  584,  258,  340,  853,  975,  440, 1481,  340,
 1409,  875, 1101, 1284,  348, 1778,  271,   59,  458,  458,
  779,  454,  582,  357,  279,  595,  458,  630,    0,  363,
 1085,  410,    0,  409,  447,  639, 1788, 1534, 1244,   41,
  502, 1486,  318, 1540,  501,  300,  501,  381,  742,  734,
   59,  306, 1216,  308,  636,  637,  440,   59,  440,  639,
 1266,  757, 1300,  452,  340,  458,  387,  436,  648,   41,
  452,  397,  454,   41,  329,  440,   44,  386,  648,  648,
  367,  775,  776,    0,  418,  331,  517,   59,  411,  340,
  440,   59, 1589,  348,  458,  397,  387,  262,  678, 1448,
  454,  356,  357,  348,  454,  396,  318,  711,  363, 1606,
  300,  387,  383,  431,  678,  279, 1022, 1466,  454, 1332,
  731,  485,  702,  454,   41,  386,  381,   44,  340,  301,
  710,  711,   41,  978,  714,   44,  300,  403,  257,  719,
  458,  387,   59,  273,  283,  751,  258,  753,  728,   12,
  396,  731,  431,  757,  420,   41,  736,  262,  762,  271,
  740,  506,  766,  418,  440,  329,    0, 1710, 1040,    0,
 1421,  751,   41,  753,  429,  262,  756,  262,  454,  458,
 1052, 1087,  762,  517,  348,  789,  766,  769,  770,  771,
 1035, 1688,  264,  357,  306, 1691,  308, 1042, 1710,  363,
 1697, 1073,  431,  454,   41, 1411,  405,  348, 1710,  789,
    0,  348,    0,  817,  818,  334, 1264,  381,  778,    0,
 1397,  386, 1099, 1710, 1710,   59, 1209,    0,   59,  458,
  959, 1186,  387,  962, 1710,    0,    0,  817,  818,  338,
   16,  396,  454, 1740,   59,  384,   42,  829,  854,  328,
  301,   47,  331,  823,    0, 1287,  816,  456,  864,  387,
   41, 1293,  517,   44,  343, 1220,  836,    0, 1360,   59,
  442,   59, 1749, 1750,  854,  138,  318,   41,   59,  975,
   61,  386,  377,    0,  864, 1162,   59,  338,  409,  849,
  850,  851,   44,    0,   59,   59,  876,  261,  340,  386,
  439,  386,   41,    0,  458,  875,  418,   83,   41,   42,
   43,   44,   45,   59,   47,  325,  318,  456,  319,  328,
  461,  462,  331,  418,   41, 1308,   59,   60,   61,   62,
  435,  963,  257,    0,  307,  308,  405,    0,  340,  411,
  356,   93,   59,  442,   41,  387,  318,   44,  435,  960,
  318, 1256,   59,  517,  396,  424,  328,  964,    0,  331,
   93,  440,   59,  331, 1209,  274,  286,  313,  340, 1846,
  465,  975,  340,  452, 1186,  454,  262, 1071,   41,  959,
  960,  961,  962,  963,  964,  387,  961,  458,    0,    0,
  970,  442,   59,  262,  396,  265,   59, 1302,  440,  400,
  309,  318,  982,  983, 1276,    0,  263,  987, 1220,  334,
    0,  328,  454,  429,  331,  387, 1305,   59,   40,  387,
  430,    0,    0,  340,  396,  262,  343,  437,  396,    0,
  264,  440,  302,  264,  417, 1307, 1642,  402,  440, 1638,
  441, 1640,  322,  452, 1289,  454, 1542,   59,   59, 1360,
 1360,   41,  454,  337, 1519,  312, 1662, 1522,  465, 1665,
  467,  444,   41, 1308,   59,   44,  323,  337,  440,   59,
  387,  417,  440,  438,    0,   44,  264,  258,  451,  396,
   59,   59,  454, 1063,  280, 1089,  454,  402,   59,  285,
  374,  264, 1096,  274,    0, 1065, 1065, 1696, 1078,    0,
  386,    0,  411,  387, 1204, 1205, 1206, 1176, 1413, 1089,
 1179, 1771, 1418,  278,  265,  280, 1096,  386,    0,  300,
 1080,    0,  653,  440,   93,  306,  401,  658,  309,  262,
  445,  264,  265,   59,  258,  452,  343,  454, 1098,  272,
   41,  461,  462, 1608, 1104,  259, 1240,  271, 1242,  386,
  340,  302, 1397,   59,  318,  288,  270,  466,   59,   41,
   59,  368,   44,  397,  328, 1437,  397,  331,  804,  302,
  427,  278,   44,  280,  307,  308,  340,   59,  311,    0,
   59,  278,  306,  280,  308,  318,   44,  320,  321,  328,
  287,  324,  331,  508, 1793,  328,  342,  264,  331,    0,
  333,   59,  335,  258, 1476,    0,    0,  340,  341, 1303,
  343,  328,    0,  378,  331, 1175,    0,  238, 1198,  426,
 1200,   93,  264,  387,  397,  246,  359,  360,  397,  362,
  411,  377,  396,    0, 1204, 1205, 1216,  418,   59, 1845,
   41,   41, 1222,  376,   44,  378,  379,  380,  387,  322,
  383,  306,  264,  386,  387,  436, 1238,  396,   59,  405,
   44,    0,  408,  396,   59,   59,    0,  436,  331,  264,
  387,   59,  352,  260,  260,   59,  440,  410,  424,  396,
  413,  378,   40, 1748,    0,  466,  383,  258,  452,  470,
  454, 1596,   59,   93,  418,    0,  429, 1602,  378,  272,
  271,  470,  435,  400,  450, 1374,  383,  440, 1404, 1378,
  407,  259, 1282, 1282,    0,  288,  303,  303,  451,  452,
   59,  454,  270,  258,  387,   59,  261,  460,  318,    0,
  397, 1603,  258,  396, 1430,  306, 1318,  308,  328,  318,
 1322,  331,  877,   59,  441,  271,  881,  878,    0,  328,
  340,  882,  331,  279,   59,  397,  489,  490,  264,  492,
  333,  340,   44,  418,  343,  264,  499,  500,  501,  502,
  503,  306,  505,   59,  300,  508,  509,  510,  511,  512,
  306,  436,  308,  516,  517,  397, 1702, 1703,   59,  362,
  301,  417,    0,    0,  378,  417,  278,  387,  280,  278,
    0,  280,  397,  329,  501,  287,  396,   59,  387, 1429,
 1046, 1047, 1198,    0, 1200,  470,  400,  396,  444, 1399,
  260, 1521,  348,  407, 1404, 1429, 1430,  338, 1524, 1409,
 1746,  357, 1438,   41,    0,  287,   44,  363,  460, 1421,
  256,    0,  383, 1537, 1794, 1795, 1515,  418,  300, 1429,
  440,   59,   59, 1693,   41,  381,  429,  441, 1438,   59,
 1096,  440,  278,  303,  454, 1710, 1562,  489,  490,    0,
  492,  287,   59,  452,    0,  454,  279,  499,  500,  501,
  502,  503,   41,  505,  402,   44,  508,  509,  510,  511,
  512,  397,  418,   59,  516,  517,  378,    0,  397,  260,
   59, 1741,  318, 1715,  385,  493, 1856,  321,  321, 1825,
   41,  458, 1543,   44,    0,  503,  434,  318,  400,  258,
 1587,  335,  335,  383,    0,  407,  329,  383,   59,  343,
  331,  449,  271,   59,  436,   44,  260,  294,  485,  340,
  279, 1511,  303,  321, 1524,  359,  359,  342,  305, 1789,
   59, 1521,    0,    0,  342,  312,   59,  335, 1562,  441,
  363,  300,  494, 1543, 1588,  343,    0,  306,  282,  308,
  383,   40,    0,   59, 1670,  342,  290, 1583,  381,  303,
  337,  359,  377,   59, 1824, 1546,  387,    0,    0,  377,
  329,  517,    0,   41,   41,  396,   44,   44, 1578,  413,
  413,  417,  321, 1583,    0,  421,  422,  423, 1588,  348,
  405,   59,   59,  408,  337, 1682,  335,  405,  357,  501,
  408, 1607, 1718,    0,  363,   59,  342, 1607,    0,  424,
 1612,   59,    0,  418, 1616,  413,  424,   46,  405,  440,
  359,  408,  381,  318, 1655,  402,   59,   59,   44,  280,
  274,   59,   61,  454,  285,  450,   41,  424,  356,   44,
   40,  377,  450,   59,  383,  260, 1762,  483,  424,  342,
  461,  487,  279,   40, 1770, 1655,  301,  434,    0,  418,
 1776,  438,   59,  450,  264,   42,   43,   59,   45,  405,
   47,   59,  408,  300,  413,  393, 1727,  395,   42,   43,
  316,   45,   59,   47,  377, 1691,  301,   40,  424, 1805,
    0, 1691, 1808,  338,   41, 1811, 1812,   44,  313,  278,
  418,  280,  329, 1709, 1704,   58,    0, 1823,  287, 1709,
  318,  318,  405,  264,  450,  408, 1832,   59,   40,    0,
    0,  348, 1838,  338,  331,  443,   46, 1727, 1728,  356,
  357,  424,  321,  340, 1850,  258,  363, 1853, 1744, 1729,
 1729,   61,    0,  337, 1744,    0,  335, 1753,  271,   59,
    0,  460, 1776, 1753,  381,  336,  279,  450,  517, 1803,
  316,  317,  407,  452,   41,   59,  393,   44,  395,  320,
  359, 1771,  417, 1718,  493,  494,  402,  300,   59,   59,
  387,  461,  462,  306, 1790,  308,  340, 1811, 1812,  396,
 1790,  418,  407,   40, 1800,  284, 1796,  442,  320,  378,
 1800,   59,  417, 1803,   59,  318,  329,   42,   43,   59,
   45,  278,   47,  280, 1838,   41,  443, 1762,   44,  402,
  287,  400,  658, 1823,  413,  348, 1850,  442,  407, 1853,
 1830,  383,   41,  440,  357,   44,  383, 1837,   41,  675,
  363,   44,   41,  679,  506,   44,  436,  454,   42,   43,
  686,   45,  361,   47,  450,  691,  692, 1857,  381,  434,
 1805,   41,  441, 1808,   44,  701,  384,  703,  704,  705,
  706,  707,  708,  709,  895,  417,  897,   41,  899,  900,
   44,  902,  903,  904,   93,  442,  674, 1832,  676,  454,
  517,  516,  680,  681,  682,  418,  287,  457,  458,  459,
  516,  452,  738,  280,   41,  321,  411,   44,  285,  460,
   41,  378,  328,   44,   41,  331,  280,   44,   44,  335,
   41,  285,  501,   44,  760,  485,  486,  343,  469,  765,
  372,   42,   43,  400,   45,  286,   47,   61,  489,  490,
  407,  492,   40,  359,   41,   40,    0,   44,  499,  500,
  501,  502,  503,   41,  505,  264,   44,  508,  509,  510,
  511,  512,   40,  340,   40,  516,  517,  383,  461,   41,
   41,  460,   44,   44,  441,  461,  340,   42,   43,   40,
   45,   41,   47,   40,   44,   40,   40,   41,   42,   43,
   44,   45,   46,   47,  517,  391,  392,  413,  461,  462,
  489,  490,  418,  492,   40,   59,   60,   61,   62,   40,
  499,  500,  501,  502,  503,   40,  505,   40,   40,  508,
  509,  510,  511,  512,  440,  461,   40,  516,  517,   40,
  417,   40,   40,   91,  501,  436,  452,   91,  454,   93,
   44,   46,  320,   44,  402,  280,  320,   44,  264,   44,
  285,   61,  428,   46,  890,  891,  892,   40,  264,  895,
  896,  897,   44,  899,  900,  901,  902,  903,  904,  382,
  505,  907,  908,  460,  910,  348,  912,  348,  348,  915,
  916,  917,  918,  919,  920,  320,  280,   44,  278,  278,
  300,  285,  264,  397,  397,  436,  932,  470,  934,  458,
  277,  447,  489,  490,  348,  492,  399,  460,  447,   40,
  349,   40,  499,  500,  501,  502,  503,   91,  505,   40,
   40,  508,  509,  510,  511,  512,  320,  318,  264,  516,
  517,   42,  383,  389,   44,  411,  489,  490,  460,  492,
  382,   40,  978,   40,  451,   41,  499,  500,  501,  502,
  503,  436,  505,  464,  317,  508,  509,  510,  511,  512,
   40,  320,  320,  516,  517,  452,  448,  489,  490,  280,
  492,  264,  502,    0,  285,  458,   40,  499,  500,  501,
  502,  503,   40,  505,  274,  277,  508,  509,  510,  511,
  512,  300,  344, 1670,  516,  517,  344,  284,  278,  386,
  311,   42,   43,  386,   45,  386,   47,  418,  262,  416,
  264,  265,   40,  418,   41,  280,  300,   44,  272,  417,
  285,   41,   42,   43,  356,   45,  280,   47,   40,  274,
   40,  285,   59,  417,  288,  264,   41,   44,  320,    0,
   44, 1718,  280,   44,  385,  367,  311,   44,  302,   40,
  320,   44,   44,  307,  308,  328,   40,  311,  299,  266,
  501,   40,  264,   41,  318,  422,  320,  321,   42,   43,
  324,   45,   58,   47,  328,   41,   41,  331,   41,  333,
   41,  335,  394,   44,  264, 1762,  340,  341, 1124,  343,
   41,   42,   43, 1770,   45,  440,   47,  309,   59,  454,
 1136, 1137,   41, 1139, 1140,  359,  360,  492,  362,  309,
    0, 1147,   41,   41,   42,   43,   40,   45,   40,   47,
  352,   40,  376,   40,  378,  379,  380,   40, 1805,  383,
 1166, 1808,  386,  387,   42,   43,   44,   45, 1174,   47,
 1176,   40,  396, 1179,   40, 1181,   40,   40,   40, 1185,
 1186, 1187, 1188,   40,   44, 1832,  410,   40,   40,  413,
   40,   40,  460,  308,   41,   42,   43,   40,   45,   59,
   47,  262,  435, 1209,  327,  429,   41,   42,   43,   41,
   45,  435,   47,   44, 1220,  296,  440,  343,  274,  388,
  388,  489,  490,  331,  492,  422,  388,  451,  452,  390,
  454,  499,  500,  501,  502,  503,  460,  505,  271,  446,
  508,  509,  510,  511,  512,  391,  287,   58,  516,  517,
  407,  386,  386, 1259,  265,  386,  386,   41,   42,   43,
   41,   45,  386,   47,  386,  489,  490,    0,  492,  280,
   41,  378,  278,   40,  285,  499,  500,  501,  502,  503,
   41,  505,   59,  279,  508,  509,  510,  511,  512,   41,
  280,  302,  516,  517,   41,  285,  417,   41,   42,   43,
   41,   45, 1308,   47,  300,  308,  435,  385,   41,   42,
   43,   44,   45,   46,   47,  280,  436,  396,  262,  337,
  301,  318,  338, 1329,  321,  442,   59,   60,   61,   62,
  442,  328, 1338,  329,  331,  301,  280,  338,  335, 1345,
   42,  285,   41,  340,   41,  352,  343,  278,  320,  280,
  385,  371,  348,  284,  371,   41,  287,   41,   91,  280,
   93,  357,  359,   41,  285,   41,   41,  363, 1374,  300,
   41,   41, 1378,   41,   41,   44,   41,   41,   41,  460,
   41,   41,  280,   41,   41,  381,  383,  285,   44,   40,
  387, 1397,   41,   42,   43,  340,   45,   59,   47,  396,
   59,   40,  280,  340, 1410,   59,   59,  285,  489,  490,
   41,  492,   59,   59,   59,  308,  413,   59,  499,  500,
  501,  502,  503,  352,  505,  344,   41,  508,  509,  510,
  511,  512,   40,  280,  368,  516,  517,   40,  285,  340,
  383,  451,   44,  440,  383,  280,   59,  378,   59,   46,
  285,  383,   41,  343,   41,  452,   41,  454,   44,  410,
  304,  321,  435,  460,  301,  387,  442,   40,  328,  400,
  293,  331,  463,   44,  304,  335,  407,   59,  307,   44,
   41,    0,  337,  343,   40,  264,  383,  383, 1494, 1495,
  278,  264,  489,  490,  458,  492,  280,  411,  389,  359,
  320,  285,  499,  500,  501,  502,  503,  264,  505, 1515,
  441,  508,  509,  510,  511,  512,   44,  470,   40,  516,
  517,  517,  309,  383,  451,  271,  264,  435,  458,  262,
  436,  264,  265,   45,  343,  343,  280,  343,  343,  272,
  458,  285,  271, 1549,  308,   41,   40,  280,  308,   40,
   59,   59,  285,  413,  308,  288,  472,  473,  474,  475,
  476,  477,  478,  479,   41,  304,   59,   41,   41,  302,
   59,  304,    0,    0,  307,  308,    0,    0,  311, 1201,
  440,   42,   43,   12,   45,  318,   47,  320,  321,  266,
  273,  324,  452,  265,  454,  328, 1101,  643,  331,   87,
  333,  719,  335,  867, 1099, 1611, 1088,  340,  341, 1096,
  343, 1456,  330, 1619,   41,   42,   43,   44,   45,   46,
   47, 1457,  590,  360, 1078,  323,  359,  360, 1025,  362,
  762,  280,   59,   60,   61,   62,  285, 1643,  342,  313,
 1089,  342, 1652,  376,  799,  378,  379,  380, 1654,  460,
  383,  320, 1658,  386,  387, 1578, 1435, 1734,  568, 1738,
 1780, 1437, 1737,  396,   91, 1257,   93, 1837, 1715, 1691,
  789, 1660, 1777, 1675, 1011, 1235, 1414,  410,  489,  490,
  413,  492, 1844,  460,  570,  335,  846,  328,  499,  500,
  501,  502,  503,  838,  505,  347,  429,  508,  509,  510,
  511,  512,  435, 1291,  629,  516,  517,  440,   13, 1715,
  300, 1717,  489,  490,  751,  492,  498,  502,  451,  452,
  765,  454,  499,  500,  501,  502,  503,  460,  505,  143,
  738,  508,  509,  510,  511,  512,  987, 1800, 1643,  516,
  517,   42,   43, 1532,   45, 1753,   47, 1857,  513,  727,
  979, 1138, 1246,   -1, 1181,   -1,  489,  490,  460,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,  489,  490,   -1,
  492,  460,   -1,   -1,   -1,   -1, 1670,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  280,
  489,  490,   -1,  492,  285,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  262,  505,  264,  265,  508,
  509,  510,  511,  512, 1718,  272,   -1,  516,  517,   42,
   43,    0,   45,  280,   47,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   41,   42,   43,   59,   45,    0,   47,
   -1,   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,   -1,   -1, 1762,   -1,
   -1,  318,   41,  320,  321,   44, 1770,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   41,
   59,   -1,   44,  340,  341,   -1,  343,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   59,   -1,   61,
   -1, 1805,  359,  360, 1808,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,  458,   -1,  460,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1, 1832,  386,
  387,  262,   -1,   -1,   -1,   -1,   -1,   -1,   91,  396,
   93,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,  280,
  492,   -1,   -1,  410,  285,   -1,  413,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  429,   -1,  516,  517,   -1,   -1,  435,   -1,
   41,   42,   43,  440,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   41,   42,   43,
   -1,   45,   -1,   47,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   41,   42,   43,    0,   45,   -1,   47,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  280,   -1,  516,
  517,   -1,  285,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,  280,   -1,   41,   42,   43,  285,   45,   -1,
   47,   -1,   -1,   -1,   59,   -1,   61,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,  258,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,  262,
   -1,  264,  265,   -1,   -1,  277,  278,   -1,  280,  272,
   -1,  300,  284,   -1,   -1,  287,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,  300,   41,
   42,   43,    0,   45,  306,   47,   41,   42,   43,  302,
   45,    0,   47,   -1,  307,  308,   -1,   45,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   41,   -1,   -1,   44,  340,  341,   -1,
  343,   -1,   41,   42,   43,   44,   45,   46,   47,  378,
   -1,   59,   -1,   61,   -1,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,   -1,   -1,  378,   -1,   -1,  280,
   -1,  400,   -1,  376,  285,  378,  379,  380,  407,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,  400,   -1,
   -1,   -1,   91,  396,   93,  407,  280,   -1,   -1,   -1,
   -1,  285,   -1,  280,  416,   -1,  418,  410,  285,   -1,
  413,  280,  441,   -1,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,  429,   -1,   -1,  441,
   -1,   -1,  435,  258,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,  277,  278,   -1,  280,   -1,  460,  470,  284,
   -1,   -1,  287,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,   -1,   -1,   -1,   -1,  300,   -1,  280,   -1,   -1,
   -1,  306,  285,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,    0,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,  280,   41,   -1,   -1,   44,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,   -1,   -1,  378,   59,   -1,   -1,  295,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
  278,   -1,  280,  272,   -1,  400,  284,   -1,   -1,  287,
   -1,  280,  407,   -1,   -1,  323,  285,   -1,   -1,  288,
   -1,   -1,  300,  418,   -1,   -1,   -1,   -1,  306,    0,
   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,
   -1,  436,  311,   -1,  352,   -1,  441,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   41,  340,  341,   44,  343,  470,   41,   42,   43,   44,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   59,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,
  378,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,  427,
   -1,   -1,  400,   -1,   -1,   -1,   -1,  396,   93,  407,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
  418,  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,  457,
  458,  459,   -1,  461,  462,   -1,   -1,   -1,  436,   -1,
  429,   -1,   -1,  441,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,  480,  481,  482,  483,  484,  485,  486,  487,
  488,   -1,  451,  452,   -1,  454,   -1,  495,  496,   -1,
   -1,  460,  470,   -1,   -1,   -1,  504,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  321,  516,  517,   -1,
   -1,   -1,   -1,  328,   41,   -1,  331,   44,   -1,   -1,
  335,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  343,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,    0,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  305,   -1,   -1,  262,   -1,  264,
  265,  312,   -1,   -1,   -1,  316,  317,  272,  383,   -1,
   -1,   -1,   -1,   -1,  325,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,  337,   41,   -1,   -1,
   44,   41,   -1,   -1,   44,   -1,   -1,  302,  413,    0,
   -1,   -1,  307,  308,   -1,   59,  311,  318,   -1,   59,
  321,   -1,   -1,  318,   -1,  320,  321,  328,   -1,  324,
  331,   -1,   -1,  328,  335,  440,  331,   -1,  333,  340,
  335,   -1,  343,   -1,  294,  340,  341,  452,  343,  454,
   41,   42,   43,   44,   45,  305,   47,   -1,  359,   -1,
   -1,  402,  312,   -1,  359,  360,   -1,  362,   59,   60,
   61,   62,   -1,   -1,   -1,  325,   -1,   -1,  419,   -1,
   -1,  376,  383,  378,  379,  380,  387,   -1,  383,   -1,
   -1,  386,  387,  434,   -1,  396,   -1,  438,   -1,   -1,
   -1,  396,   93,   -1,   -1,   -1,   -1,   -1,  449,   -1,
   -1,   -1,  413,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  468,   -1,   -1,
   -1,   -1,   -1,   -1,  429,   -1,  386,    0,   -1,  440,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,  452,  402,  454,   -1,   -1,  451,  452,   -1,  454,
   -1,  278,   -1,  280,   -1,  460,   -1,   -1,   -1,  419,
  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  300,  434,   -1,    0,   -1,  438,   -1,
   -1,  441,   -1,   -1,  489,  490,   59,  492,   -1,  449,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  468,   -1,
   -1,  516,  517,   -1,   -1,    0,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,  278,   -1,  280,   -1,  278,   -1,
  280,   -1,   -1,  287,   -1,   59,   -1,  287,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,
   -1,  378,   -1,   -1,   -1,   -1,   41,   42,   43,   44,
   45,  262,   47,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,  400,   59,   60,   61,   62,   -1,   -1,
  407,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   93,   -1,
  311,   -1,   -1,   -1,  441,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,  378,   -1,   -1,  328,  378,   -1,
  331,   -1,  333,   41,  335,   -1,   44,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,  400,   -1,   -1,   -1,
  400,   59,    0,  407,   -1,   -1,   -1,  407,  359,  360,
    0,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,  441,   -1,   -1,
   -1,  441,   40,   41,   -1,  396,   44,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,  278,   -1,  280,   -1,  410,
   -1,   59,  413,   61,  287,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,  278,   -1,  280,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,  288,  505,  378,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  302,   -1,    0,
   -1,   -1,  307,  308,   -1,   -1,  311,  400,   -1,   -1,
   -1,   -1,   -1,  318,  407,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,  378,  340,  341,   -1,  343,   -1,
   41,   -1,   43,   44,   45,   -1,   -1,   -1,  441,   -1,
  278,   -1,  280,   -1,  359,  360,  400,  362,   59,   60,
   61,   62,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,  396,   93,   -1,   -1,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  274,   -1,  413,  277,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,  300,   -1,   -1,  440,   -1,   -1,  306,   -1,
   -1,  309,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
  378,   -1,   -1,   -1,   -1,  460,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,
   -1,  331,  400,   -1,   -1,  335,   -1,   -1,   -1,  407,
  340,   -1,   -1,  343,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  359,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  378,  516,  517,  441,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,   -1,  387,   -1,   -1,
   -1,   -1,  400,   -1,   -1,   -1,  396,   -1,   -1,  407,
   -1,   -1,  287,  411,   -1,   -1,   -1,   -1,   -1,   -1,
  418,   -1,   -1,  413,   -1,   -1,   41,   -1,   43,   44,
   45,  262,   -1,  264,  265,   -1,   -1,   -1,  436,   -1,
   -1,  272,   -1,  441,   59,   60,   61,   62,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,  466,   -1,
   -1,  302,  470,   -1,   -1,   -1,  307,  308,   93,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1, 1330, 1331,   -1,
   -1,   -1,    0, 1336, 1337,   -1, 1339, 1340,  359,  360,
   -1,  362,   -1,   -1, 1347, 1348, 1349, 1350,   -1, 1352,
 1353, 1354, 1355, 1356, 1357,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   40,   41,   -1,  396,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   59,  413,   61,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,  260,   91,  489,  490,   -1,  492,   -1,   -1,
  451,  452,   -1,  454,  499,  500,  501,  502,  503,  460,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  301,   -1,   -1,   -1,  272,  489,  490,
   -1,  492,   -1,   -1,   -1,  313,   -1,   -1,  499,  500,
  501,  502,  503,  288,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  302,   -1,    0,
  338,   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,  287,   -1,  407,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,  417,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,  396,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  442,  410,  274,   -1,  413,  277,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,
   -1,   -1,  460,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,  300,   -1,   -1,  440,   -1,   -1,  306,   -1,
   -1,  309,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,  489,  490,   -1,  492,  460,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  378,  516,  517,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,
  460,   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,
  418,   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,   44,
   45,  262,   -1,  264,  265,   -1,   -1,   -1,  436,  489,
  490,  272,  492,  441,   59,   60,   61,   62,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  288,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  466,   -1,
   -1,  302,  470,   -1,   -1,   -1,  307,  308,   93,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,    0,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   59,  386,  387,   -1,   -1,  434,
   -1,   -1,   -1,   41,   -1,  396,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   59,  413,   61,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   91,  489,  490,   -1,  492,   -1,   -1,
  451,  452,   -1,  454,  499,  500,  501,  502,  503,  460,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,  288,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,  264,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  258,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  274,   -1,  413,  277,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,
   -1,   -1,   -1,  318,  429,   -1,  321,   -1,   -1,   -1,
  435,   -1,  300,  328,   -1,  440,  331,   -1,  306,   -1,
  335,  309,   -1,   -1,   -1,  340,  451,  452,  343,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  383,   -1,
   -1,   -1,  387,   -1,  499,  500,  501,  502,  503,   -1,
  505,  396,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  378,  516,  517,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,
   -1,   -1,   -1,  411,   -1,  440,   -1,   -1,   -1,   -1,
  418,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,  436,   -1,
   -1,  267,   41,  441,   -1,   44,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,  343,   -1,   -1,   -1,
   59,   -1,   -1,  289,  489,  490,   -1,  492,  466,  295,
  296,   -1,  470,   -1,  499,  500,  501,  502,  503,   -1,
  505,  368,   -1,  508,  509,  510,  511,  512,  314,   -1,
   40,  516,  517,   43,   -1,   45,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  378,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   58,  460,   44,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,  417,   -1,   -1,   -1,  421,   -1,   -1,   -1,   -1,
   -1,  427,  489,  490,   -1,  492,   -1,  433,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  443,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,  513,  514,  515,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,  276,   -1,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  289,
   -1,  340,  292,   -1,  343,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,  323,  260,   -1,  326,   -1,   -1,   -1,
   -1,  267,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
  276,   -1,  262,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,  289,  354,  355,   -1,   -1,   -1,  295,
  296,   -1,   41,   -1,  413,   44,  366,  303,   -1,   -1,
  370,   40,   -1,   42,   43,   -1,   45,   -1,  378,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  435,  323,   -1,   58,
  326,  440,   -1,   -1,   63,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,  452,   -1,  454,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,  352,   -1,  354,  355,
  340,  421,   -1,  343,   -1,   -1,   -1,  427,   -1,   -1,
  366,   -1,   -1,  433,  370,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,  427,  492,  413,   -1,  495,  496,  433,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  443,  508,  509,
  510,  511,  512,   -1,   -1,  435,  516,  517,   -1,   -1,
  440,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
  289,   -1,  260,   -1,   63,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  323,   -1,  335,  326,   -1,   -1,
   -1,  340,   -1,  301,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  313,   -1,   -1,   -1,  443,
  359,   -1,   -1,  352,   -1,  354,  355,   40,   -1,  327,
   43,   -1,   45,   -1,   -1,   -1,  460,  366,   -1,   -1,
  338,  370,   -1,   -1,  383,   58,   -1,  386,  387,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,  413,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,  435,   -1,  427,   -1,
   -1,  440,   -1,   -1,  433,   -1,   -1,   -1,   -1,  407,
   -1,   -1,   -1,  452,  443,  454,   -1,   -1,   -1,  417,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,  442,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  460,  492,   -1,   -1,  495,  496,  267,  498,
  499,  500,  501,  502,  503,  504,  505,  276,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
  289,  489,  490,  460,  492,   -1,  295,  296,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  314,   -1,   -1,  516,  517,
   -1,   -1,  489,  490,  323,  492,   -1,  326,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,  267,  354,  355,   40,   -1,   -1,
   43,   -1,   45,  276,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   58,  289,   -1,   -1,  378,
   63,   -1,  295,  296,   -1,   41,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  314,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,
  459,  460,  461,  462,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  421,  508,
  509,  510,  511,  512,  427,   -1,   -1,  516,  517,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,    0,  492,
   -1,  328,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   40,  272,   -1,   43,   41,
   45,   -1,   44,   -1,  280,   -1,   -1,   -1,   -1,  285,
   -1,  314,  288,   58,   -1,   -1,   -1,   59,   63,   61,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  402,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   91,
   -1,  354,  355,   -1,   -1,   -1,   -1,  333,   -1,   -1,
   -1,   -1,   -1,  366,   -1,  341,   -1,  370,   -1,   -1,
   -1,  438,   -1,   -1,   -1,  378,  443,   -1,   -1,   -1,
   -1,   -1,  449,   -1,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   40,  460,   -1,   43,   -1,   45,   -1,   -1,
  376,  468,  378,  379,  380,   -1,   -1,   -1,   -1,   -1,
   58,  328,   -1,   -1,   -1,   63,   -1,   -1,  421,   -1,
   -1,   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,  429,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  260,  258,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,  276,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,
   -1,   -1,  284,  460,  289,  287,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,
   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  323,   -1,
   -1,  326,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,  260,    0,   -1,  263,   -1,   -1,   -1,  267,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   40,   -1,   -1,   43,   41,   45,   -1,   44,  400,   -1,
   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,   58,  411,
   -1,   -1,   59,   63,   61,  323,  418,   -1,  326,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,  443,  441,
   -1,   -1,   -1,   -1,   91,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,  466,   -1,  471,   -1,  470,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,  471,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  260,  258,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,  274,   -1,   -1,
   -1,  278,   -1,  280,   -1,   -1,   -1,  284,  460,  289,
  287,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   40,   -1,  300,   43,   -1,   45,   -1,   -1,  306,
   -1,   -1,  309,   -1,   -1,   -1,   -1,  489,  490,   58,
  492,   -1,   -1,  323,   63,   -1,  326,  499,  500,  501,
  502,  503,   -1,  505,   -1,  460,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,  366,  492,   -1,   -1,
  370,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,  378,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,
  407,   -1,  260,   -1,  411,  263,   -1,   -1,   -1,  267,
   -1,  418,   -1,   -1,   -1,  425,   -1,  427,  276,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  436,
   -1,  289,   -1,  443,  441,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,  466,
   -1,  471,   -1,  470,   -1,  323,   -1,   -1,  326,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  354,  355,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,
   -1,  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   58,  427,
   -1,   -1,   -1,   63,  323,  433,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  271,   -1,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,  301,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  312,  313,   -1,
  315,   -1,   40,  318,   -1,   43,   -1,   45,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,  332,   -1,   -1,
   58,   -1,  471,  338,   -1,   63,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,  417,   -1,   -1,  295,  296,   -1,   -1,   -1,
  460,   40,   -1,   -1,   43,   -1,   45,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   58,
   -1,   -1,   -1,  323,   63,   -1,  326,   -1,  453,  489,
  490,   -1,  492,   -1,   -1,  460,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  354,  355,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,  366,  492,   -1,   -1,
  370,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,  276,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,  443,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  354,  355,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,
   -1,  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
  289,   -1,   -1,   -1,   63,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   42,   43,   -1,   45,  425,   47,  427,
   -1,   -1,   -1,   -1,  323,  433,   -1,  326,   -1,   -1,
   -1,   60,   61,   62,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,
  499,  500,  501,  502,  503,  504,  505,  276,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   40,
  289,   -1,   43,   -1,   45,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  272,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  280,   -1,   -1,  323,   -1,  285,  326,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,  333,   40,   -1,   42,   43,   -1,
   45,   -1,  341,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,  360,  261,  362,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,  282,   -1,   -1,   -1,   -1,  427,   -1,
   -1,  290,   -1,   -1,  433,   -1,   -1,   -1,  297,   -1,
  299,   -1,  301,   -1,  443,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,  313,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,  327,   -1,
  429,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,  338,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  267,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,  405,   -1,   -1,   -1,
   -1,   -1,   -1,  412,   -1,   -1,  415,   -1,  417,  418,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,  267,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   41,   -1,   40,   44,  289,   43,   -1,   45,   -1,   -1,
  295,  296,  491,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,  506,  507,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,  427,   -1,   41,  516,  517,   44,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   91,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  262,  260,  508,  509,  510,  511,  512,   -1,  267,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,  303,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,  323,   -1,  328,  326,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  359,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,  383,   -1,   63,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,  274,   -1,   -1,
   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,  427,
  287,   -1,   -1,   -1,  435,  433,   -1,   -1,   -1,  440,
   -1,   -1,   -1,  300,   -1,  443,   -1,   -1,   -1,  306,
   -1,  452,  309,  454,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,
  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,  466,
  289,  370,   -1,  470,   -1,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  427,  508,
  509,  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  276,  516,  517,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   40,
   41,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  460,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  469,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,   -1,   -1,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,  417,   -1,  499,
  500,  501,  502,  503,  504,  505,  506,  427,  508,  509,
  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,  291,   -1,   -1,   -1,  295,
   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,  339,   -1,  276,  516,  517,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  406,   -1,   -1,   -1,   -1,  411,  267,   -1,   -1,   -1,
   -1,  352,   -1,  354,  355,  276,   -1,  423,   40,   -1,
   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,  370,
   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,  445,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  497,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  427,  508,  509,  510,
  511,  512,  433,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,  460,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,   -1,  508,
  509,  510,  511,  512,   -1,  427,   -1,  516,  517,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,  471,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,  339,   -1,  276,  516,  517,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,  276,   -1,  423,   40,   -1,   -1,   43,
   -1,   45,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,
   -1,   -1,  295,  296,   58,   59,   -1,  445,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  417,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  497,
  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  427,  508,  509,  510,  511,  512,
  433,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  273,   -1,   -1,   58,  277,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,   -1,
   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,  443,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
  373,   -1,  375,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,  406,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,
  423,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,  276,   -1,   -1,  497,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,
  295,  296,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   91,   -1,   -1,   -1,   -1,  433,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   41,   -1,   -1,
   44,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  427,  508,  509,  510,  511,  512,  433,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,  262,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  280,   59,   60,   61,   62,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,  262,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,    0,   -1,  318,   -1,   -1,  321,  396,   91,
   -1,  273,   -1,   -1,  328,  277,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,  413,  340,   -1,   -1,  343,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,
   -1,  429,   -1,   41,   -1,  359,   44,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,  452,   -1,  454,   -1,   -1,  383,
   -1,   -1,  386,  387,    0,   -1,   -1,  339,   -1,   -1,
   -1,   -1,  396,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,   -1,   -1,  262,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,
   -1,  373,   -1,  375,  280,   41,   -1,   -1,   44,  285,
   -1,  435,  288,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,  406,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  324,   -1,
   -1,  423,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
  262,   -1,    0,  445,   -1,   -1,   -1,   -1,   -1,   41,
  272,   -1,   44,  359,  360,   -1,  362,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,   59,   -1,   -1,
  376,    0,  378,  379,  380,   -1,   -1,  383,  480,  481,
  386,  387,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  396,   -1,    0,   -1,   -1,  497,  318,   -1,   -1,  321,
   -1,   59,  324,   -1,   -1,   -1,  328,  413,   -1,  331,
   -1,  333,   41,  335,   -1,   44,   -1,   -1,  340,  341,
   -1,  343,   -1,  429,  262,   -1,   -1,   -1,   -1,  435,
   59,   -1,   -1,   41,  440,   -1,   44,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   59,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,    0,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,  413,   -1,  331,   -1,   -1,  262,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,  429,   -1,   -1,
   -1,   -1,    0,  435,   -1,   -1,   -1,   -1,  440,   -1,
   41,  359,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,  318,   41,   -1,  321,   44,   -1,  396,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  262,   59,   -1,   -1,  340,  413,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
  396,   -1,   -1,  262,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,  413,  340,    0,
   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,  359,   -1,  435,
  328,   -1,   -1,  331,  440,   -1,   -1,  335,    0,   -1,
   -1,   -1,  340,   -1,   -1,  343,  452,   -1,  454,  318,
   41,  383,  321,   44,  386,  387,   -1,   -1,   -1,  328,
   -1,  359,  331,   -1,  396,   -1,  335,   -1,   59,   -1,
  318,  340,    0,  321,  343,   -1,   -1,   -1,   -1,   41,
  328,  413,   44,  331,   -1,  383,   -1,  335,  386,  387,
  359,   -1,  340,   -1,   -1,  343,   -1,   59,  396,   -1,
   -1,  262,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,  359,   -1,   41,  383,  413,   44,  386,  387,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   59,   -1,   -1,  262,  383,    0,  435,  386,  387,
   -1,   -1,  440,   -1,  413,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,  413,  435,  328,   -1,   -1,
  331,  440,   -1,   -1,  335,   -1,   -1,   41,   -1,  340,
   44,   -1,  343,  452,   -1,  454,   -1,  435,   -1,   -1,
  318,   -1,  440,  321,   -1,   59,   -1,   -1,  359,   -1,
  328,   -1,   -1,  331,  452,   -1,  454,  335,   -1,   -1,
    0,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,  359,   -1,   -1,   -1,  396,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,  413,   -1,   44,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   59,
   -1,  262,   -1,    0,  435,   -1,   -1,   -1,   -1,  440,
   41,   -1,   -1,   44,    0,  413,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   59,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   41,  454,  318,   44,   -1,
  321,   -1,   59,   -1,  262,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   59,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  359,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,  383,  321,   -1,  386,  387,  359,  262,   -1,
  328,   -1,   -1,  331,   -1,  396,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,
   -1,  383,  413,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,  359,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,  413,   -1,   -1,  318,  383,   -1,  321,  386,  387,
   -1,  452,   -1,  454,  328,   -1,   -1,  331,  396,   -1,
   -1,  335,  262,  435,   -1,   -1,  340,   -1,  440,  343,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,  359,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,  452,   -1,  454,   -1,  318,   -1,
   -1,  321,  396,   -1,   -1,  262,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,  262,   -1,   -1,  413,
  340,   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  359,
  331,  435,   -1,   -1,  335,   -1,  440,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,  318,   -1,  383,  321,   -1,  386,  387,  359,   -1,
   -1,  328,  318,   -1,  331,  321,  396,   -1,  335,   -1,
   -1,   -1,  328,  340,   -1,  331,  343,   -1,   -1,  335,
   -1,   -1,  383,  413,  340,  386,  387,  343,   -1,   -1,
   -1,  271,  359,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,  413,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,  301,  452,   -1,  454,   -1,   -1,  383,  308,  396,
  386,  387,  312,  313,  435,  315,   -1,   -1,  318,  440,
  396,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,  452,  332,  454,   -1,   -1,   -1,  413,  338,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  271,  440,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  312,  313,   -1,  315,   -1,  398,  318,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  332,   -1,   -1,   -1,  417,   -1,  338,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,   -1,  453,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,  398,
   -1,   -1,   -1,  284,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,  295,   -1,   -1,  298,  417,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  432,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,   -1,  453,   -1,   -1,   -1,  339,   -1,
   -1,  460,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,  373,  492,  375,  295,   -1,   -1,  298,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  273,   -1,  516,  517,  277,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,  339,
  298,   -1,  423,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  460,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   -1,  406,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,  373,  497,  375,  499,  500,
  501,  502,  503,  423,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,  406,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,  445,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  497,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 517
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
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE",
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
"procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
"rprocedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
"replace_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
"alter_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string var_declaration_list full_proc_block end_trigger",
"input_parameters : '(' proc_parameters ')'",
"input_parameters :",
"output_parameters : RETURNS input_parameters",
"output_parameters :",
"proc_parameters : proc_parameter",
"proc_parameters : proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
"var_declaration_list : var_declarations",
"var_declaration_list :",
"var_declarations : var_declaration",
"var_declarations : var_declarations var_declaration",
"var_declaration : DECLARE var_decl_opt column_def_name non_array_type var_init_opt ';'",
"var_decl_opt : VARIABLE",
"var_decl_opt :",
"var_init_opt : '=' default_value",
"var_init_opt : default_opt",
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
"excp_statement : EXCEPTION symbol_exception_name ';'",
"excp_statement : EXCEPTION symbol_exception_name value ';'",
"raise_statement : EXCEPTION ';'",
"exec_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs ';'",
"exec_sql : EXECUTE varstate value ';'",
"varstate : VARCHAR",
"varstate : STATEMENT",
"for_select : FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : FOR EXECUTE varstate value INTO variable_list DO proc_block",
"exec_into : EXECUTE varstate value INTO variable_list ';'",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt ';'",
"event_argument_opt :",
"singleton_select : select INTO variable_list ';'",
"variable : ':' symbol_variable_name",
"proc_inputs : null_or_value_list",
"proc_inputs : '(' null_or_value_list ')'",
"proc_inputs :",
"proc_outputs : RETURNING_VALUES variable_list",
"proc_outputs : RETURNING_VALUES '(' variable_list ')'",
"proc_outputs :",
"variable_list : variable",
"variable_list : column_name",
"variable_list : variable_list ',' column_name",
"variable_list : variable_list ',' variable",
"while : WHILE '(' search_condition ')' DO proc_block",
"breakleave : KW_BREAK ';'",
"breakleave : LEAVE ';'",
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
"view_table : '(' union_expr ')' as_noise symbol_table_alias_name derived_column_list",
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
"trigger_action : AS begin_trigger var_declaration_list full_proc_block",
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
"select : union_expr order_clause for_update_clause lock_clause",
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
"for_update_clause : FOR UPDATE for_update_list",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
"ordered_select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause order_clause",
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
"table_reference : '(' union_expr ')' as_noise symbol_table_alias_name derived_column_list",
"derived_column_list : '(' alias_list ')'",
"derived_column_list :",
"alias_list : symbol_item_alias_name",
"alias_list : alias_list ',' symbol_item_alias_name",
"joined_table : table_reference join_type JOIN table_reference ON search_condition",
"joined_table : '(' joined_table ')'",
"table_proc : symbol_procedure_name proc_table_inputs as_noise symbol_table_alias_name",
"table_proc : symbol_procedure_name proc_table_inputs",
"proc_table_inputs : '(' null_or_value_list ')'",
"proc_table_inputs :",
"null_or_value_list : null_or_value",
"null_or_value_list : null_or_value_list ',' null_or_value",
"null_or_value : null_value",
"null_or_value : value",
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
"access_type : ORDER symbol_index_name",
"index_list : symbol_index_name",
"index_list : symbol_index_name ',' index_list",
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' insert_value_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt ordered_select_expr",
"insert_value_list : rhs",
"insert_value_list : insert_value_list ',' rhs",
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
"column_select : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause",
"column_singleton : SELECT limit_clause distinct_clause value from_clause where_clause group_clause having_clause plan_clause order_clause",
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
"numeric_value_function : EXTRACT '(' timestamp_part FROM value ')'",
"string_value_function : SUBSTRING '(' value FROM pos_short_integer ')'",
"string_value_function : SUBSTRING '(' value FROM pos_short_integer FOR nonneg_short_integer ')'",
"string_value_function : KW_UPPER '(' value ')'",
"udf : symbol_UDF_name '(' value_list ')'",
"udf : symbol_UDF_name '(' ')'",
"cast_specification : CAST '(' rhs AS data_type_descriptor ')'",
"case_expression : case_abbreviation",
"case_expression : case_specification",
"case_abbreviation : NULLIF '(' value ',' value ')'",
"case_abbreviation : COALESCE '(' null_or_value ',' null_or_value_list ')'",
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
"case_result : null_or_value",
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

#ifndef SHLIB_DEFS
int DSQL_yyerrflag;
#endif


#ifndef SHLIB_DEFS
int DSQL_yychar;
#endif

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
		SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) sym;
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
#ifdef DEV_BUILD
    if (DSQL_debug & 32)
        printf("%.*s\n", (int)length, string);
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
static BOOLEAN long_int (
    DSQL_NOD		string,
    SLONG	*long_value)
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
			return FALSE;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return TRUE;
}
#endif

static DSQL_FLD make_field (
    DSQL_NOD		field_name)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;
       
temp_file = FB_NEW(*tdsql->tsql_default) fil;

return temp_file;
}


static DSQL_NOD make_list (
    DSQL_NOD		node)
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
TSQL    tdsql;

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
TSQL    tdsql;

tdsql = GET_THREAD_DATA;

node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
node->nod_type = nod_parameter;
node->nod_line = (USHORT) lex.lines_bk;
node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
node->nod_count = 1;
node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

return node;
}


static DSQL_NOD make_node (
    NOD_TYPE	type,
    int		count,
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
TSQL    tdsql;

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


static DSQL_NOD make_flag_node (
    NOD_TYPE	type,
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
TSQL    tdsql;

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
    DSQL_debug = level;
    if (level >> 8)
        *yydeb = level >> 8;
    /* CVC: I added this code form Mike Nordell to see the output from internal
       operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
       When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
    if (level > 0) {
        /* Console debug code inside this scope */
        if (AllocConsole()) {
    		redirected_output = freopen("CONOUT$", "wt", stdout);
    		printf("DebugConsole - Yes, it's working.\n");
        }
    }
    else if (level <= 0 && redirected_output) {
        fclose (redirected_output);
        redirected_output = 0;
        FreeConsole();
    }
#endif
}

#ifdef NOT_USED_OR_REPLACED
static BOOLEAN short_int (
    DSQL_NOD		string,
    SLONG	*long_value,
    SSHORT	range)
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
		return FALSE;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return FALSE;
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

	BOOLEAN return_value;

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

static void stack_nodes (
    DSQL_NOD		node,
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
DSQL_NOD     curr_node, next_node, start_chain, end_chain, save_link;

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
    while ( TRUE)
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
SYM	sym;
SSHORT	c;
USHORT	buffer_len;

STR	delimited_id_str;

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
	CHECK_COPY_INCR(p, UPPER7(*ptr));
	}
    
    CHECK_BOUND(p);
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

assert(ptr <= end);

if ((tok_class & CHR_DIGIT) ||
    ((c == '.') && (ptr < end) && (classes [*ptr] & CHR_DIGIT)))
    {
    /* The following variables are used to recognize kinds of numbers. */

    BOOLEAN have_error     = FALSE;	/* syntax error or value too large */
    BOOLEAN have_digit     = FALSE;	/* we've seen a digit              */
    BOOLEAN have_decimal   = FALSE;	/* we've seen a '.'                */
    BOOLEAN have_exp       = FALSE;	/* digit ... [eE]                  */
    BOOLEAN have_exp_sign  = FALSE; /* digit ... [eE] {+-]             */
    BOOLEAN have_exp_digit = FALSE; /* digit ... [eE] ... digit        */
    UINT64	number         = 0;
    UINT64	limit_by_10    = MAX_SINT64 / 10;

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
	      have_error = TRUE;
	      break;
	    }
	else if (have_exp)
	    {
	    /* We've seen e or E, but nothing beyond that. */
	    if ( ('-' == c) || ('+' == c) )
		have_exp_sign = TRUE;
	    else if ( classes [c]  & CHR_DIGIT )
		/* We have a digit: we haven't seen a sign yet,
		   but it's too late now. */
		have_exp_digit = have_exp_sign  = TRUE;
	    else
		{
		/* end of the token */
		have_error = TRUE;
		break;
		}
	    }
	else if ('.' == c)
	    {
	    if (!have_decimal)
		have_decimal = TRUE;
	    else
		{
		have_error = TRUE;
		break;
		}
	    }
	else if (classes [c] & CHR_DIGIT)
	  {
	    /* Before computing the next value, make sure there will be
	       no overflow.  */

	    have_digit = TRUE;

	    if (number >= limit_by_10)
		/* possibility of an overflow */
		if ((number > limit_by_10) || (c > '8'))
		    {
		    have_error = TRUE;
		    break;
		    }
	    number = number * 10 + (c - '0');
	  }
	else if ( (('E' == c) || ('e' == c)) && have_digit )
	    have_exp = TRUE;
	else
	    /* Unexpected character: this is the end of the number. */
	    break;
      }

    /* We're done scanning the characters: now return the right kind
       of number token, if any fits the bill. */

    if (!have_error)
	{
	assert(have_digit);

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
    CHECK_COPY_INCR(p, UPPER (c));
    for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
        {
	if (ptr >= end)
	    return -1;
	CHECK_COPY_INCR(p, UPPER (*ptr));
        }

    CHECK_BOUND(p);
    *p = 0;
    sym = HSHD_lookup (NULL_PTR, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
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
					return sym->sym_keyword;
				}
			} /* else fall down and return token as SYMBOL */
		} else {
			yylval = (DSQL_NOD) sym->sym_object;
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
    sym = HSHD_lookup (NULL_PTR, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
	gds_arg_gds, gds_command_end_err,    /* Unexpected end of command */
	0);
else
    {
    ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
 	gds_arg_gds, gds_dsql_token_unk_err, 
	gds_arg_number, (SLONG) lex.lines, 
	gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
	    /* Token unknown - line %d, char %d */
 	gds_arg_gds, gds_random, 
	gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token,
 	0);
    }
}


static void yyabandon (
    SSHORT      sql_code,
    ISC_STATUS      error_symbol)
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
{ yyval = make_node (nod_foreign, e_for_count,
                        make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
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
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 256:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
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
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
break;
case 264:
{ yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 265:
{ yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 266:
{ yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 267:
{ yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
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
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
break;
case 280:
{ yyval = NULL; }
break;
case 281:
{ yyval = NULL; }
break;
case 282:
{ yyval = yyvsp[0]; }
break;
case 283:
{ yyval = yyvsp[0]; }
break;
case 286:
{ yyval = yyvsp[-1]; }
break;
case 287:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 288:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 289:
{ yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
break;
case 291:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 307:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
break;
case 308:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
break;
case 311:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 312:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
break;
case 313:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 314:
{ yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
break;
case 317:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 318:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
break;
case 319:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
break;
case 320:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 321:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 322:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 323:
{ yyval = NULL; }
break;
case 324:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
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
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 337:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 338:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 339:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 340:
{ yyval = NULL; }
break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 350:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 351:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 354:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 356:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 358:
{ yyval = make_list (yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 363:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 364:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 365:
{ yyval = yyvsp[-1]; }
break;
case 366:
{ lex.beginning = lex_position(); }
break;
case 367:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 368:
{ lex.beginning = lex.last_token; }
break;
case 369:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 370:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 371:
{ yyval = 0; }
break;
case 372:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 373:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 374:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 375:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 376:
{ yyval = NULL; }
break;
case 377:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 378:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 396:
{ yyval = NULL; }
break;
case 397:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 398:
{ yyval = yyvsp[0]; }
break;
case 399:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 400:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = yyvsp[0]; }
break;
case 403:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 404:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 405:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 406:
{ yyval = yyvsp[0]; }
break;
case 407:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 409:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 410:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = yyvsp[0]; }
break;
case 413:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 414:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 418:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 420:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 421:
{ yyval = yyvsp[0]; }
break;
case 422:
{ yyval = yyvsp[0]; }
break;
case 423:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 424:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 425:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 426:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 450:
{ yyval = NULL; }
break;
case 451:
{ yyval = NULL; }
break;
case 452:
{ yyval = NULL; }
break;
case 453:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 454:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 455:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 456:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 457:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 458:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 459:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 460:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 462:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 463:
{ yyval = yyvsp[0]; }
break;
case 464:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 465:
{ yyval = yyvsp[0]; }
break;
case 466:
{ yyval = yyvsp[0]; }
break;
case 467:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 468:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 470:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 472:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 473:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 475:
{ yyval = NULL; }
break;
case 477:
{ yyval = NULL; }
break;
case 478:
{ yyval = yyvsp[0]; }
break;
case 479:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 495:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 496:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 498:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 499:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 500:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 506:
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
case 507:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 508:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 509:
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
case 510:
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
case 511:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 514:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 518:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 519:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 520:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 521:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 522:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 523:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 526:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 539:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 540:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 541:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 542:
{         
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
			    yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
			    {
			    if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
case 543:
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
				   (db_dialect     >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect     <= SQL_DIALECT_V5) ) )
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
case 546:
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
case 547:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 548:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 549:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 550:
{ yyval = yyvsp[-1]; }
break;
case 551:
{ yyval = 0; }
break;
case 555:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 556:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 557:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 561:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 562:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 563:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 564:
{ yyval = 0; }
break;
case 565:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 568:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 569:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 572:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 573:
{ yyval = NULL; }
break;
case 575:
{ yyval = NULL; }
break;
case 576:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 578:
{ yyval = NULL; }
break;
case 580:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 585:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 586:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 587:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 588:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 589:
{ yyval = yyvsp[0];}
break;
case 591:
{ yyval = yyvsp[0];}
break;
case 592:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 597:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 599:
{ yyval = 0; }
break;
case 600:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 601:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 602:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 603:
{ yyval = (DSQL_NOD) 0; }
break;
case 604:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 605:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 607:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 608:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 609:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 610:
{ yyval = 0; }
break;
case 612:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 613:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 614:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 615:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 616:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 617:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 618:
{ yyval = make_list (yyvsp[0]); }
break;
case 619:
{ yyval = 0; }
break;
case 621:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 622:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 623:
{ yyval = 0; }
break;
case 624:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 625:
{ yyval = 0; }
break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 627:
{ yyval = 0; }
break;
case 628:
{ yyval = yyvsp[-1]; }
break;
case 629:
{ yyval = 0; }
break;
case 630:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 631:
{ yyval = 0; }
break;
case 632:
{ yyval = yyvsp[0]; }
break;
case 633:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 634:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 635:
{ yyval = 0; }
break;
case 636:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 637:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 638:
{ lex.limit_clause = true; }
break;
case 639:
{ lex.limit_clause = false; }
break;
case 640:
{ lex.first_detection = true; }
break;
case 641:
{ lex.first_detection = false; }
break;
case 642:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 643:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 644:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 645:
{ yyval = 0; }
break;
case 646:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 647:
{ yyval = yyvsp[-2]; }
break;
case 648:
{ yyval = yyvsp[-1]; }
break;
case 649:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 650:
{ yyval = yyvsp[-1]; }
break;
case 651:
{ yyval = yyvsp[0]; }
break;
case 652:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 653:
{ yyval = 0; }
break;
case 654:
{ yyval = make_list (yyvsp[0]); }
break;
case 655:
{ yyval = 0; }
break;
case 657:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 659:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 662:
{ yyval = make_list (yyvsp[0]); }
break;
case 664:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 667:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 668:
{ yyval = make_list (yyvsp[-1]); }
break;
case 669:
{ yyval = NULL; }
break;
case 671:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 672:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 673:
{ yyval = yyvsp[-1]; }
break;
case 674:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 675:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 676:
{ yyval = make_list (yyvsp[-1]); }
break;
case 677:
{ yyval = NULL; }
break;
case 679:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 683:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 684:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 685:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 686:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 687:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 688:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 689:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 690:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 693:
{ yyval = make_list (yyvsp[0]); }
break;
case 694:
{ yyval = 0; }
break;
case 696:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 698:
{ yyval = yyvsp[0]; }
break;
case 699:
{ yyval = 0; }
break;
case 700:
{ yyval = yyvsp[0]; }
break;
case 701:
{ yyval = 0; }
break;
case 702:
{ yyval = yyvsp[0]; }
break;
case 703:
{ yyval = 0; }
break;
case 704:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 705:
{ yyval = 0; }
break;
case 706:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 707:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 708:
{ yyval = 0; }
break;
case 709:
{ yyval = 0; }
break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 712:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 717:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 718:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 722:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 728:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 729:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 732:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 733:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 739:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 741:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 746:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 747:
{ yyval = yyvsp[0]; }
break;
case 749:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 752:
{ yyval = NULL; }
break;
case 753:
{ yyval = make_list (yyvsp[-1]); }
break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = NULL; }
break;
case 758:
{ yyval = make_list (yyvsp[-1]); }
break;
case 760:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 764:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 775:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = yyvsp[-1]; }
break;
case 780:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 819:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 821:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 823:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 827:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 829:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 831:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 833:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 835:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 836:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 837:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 838:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 839:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 840:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 842:
{ yyval = make_list (yyvsp[-1]); }
break;
case 843:
{ yyval = yyvsp[-1]; }
break;
case 844:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 845:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 855:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 856:
{ yyval = yyvsp[0]; }
break;
case 857:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 858:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 859:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 860:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 861:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 862:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 863:
{ yyval = yyvsp[-1]; }
break;
case 864:
{ yyval = yyvsp[-1]; }
break;
case 868:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 869:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 870:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 871:
{ yyval = yyvsp[0]; }
break;
case 872:
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
case 873:
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
case 874:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 875:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 877:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 879:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 880:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 881:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 882:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 884:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 886:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 887:
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
case 888:
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
case 889:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 890:
{ yyval = make_parameter (); }
break;
case 891:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 892:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 893:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 894:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 895:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 896:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 897:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 898:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 899:
{ yyval = yyvsp[0]; }
break;
case 900:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 902:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 903:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 904:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 905:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 906:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 908:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 909:
{ yyval = yyvsp[0];}
break;
case 914:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 915:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 917:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 918:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 919:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 920:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 921:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 922:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 925:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 926:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 928:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 929:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 930:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 932:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 935:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 936:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 939:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 940:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 943:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 944:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 945:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 946:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 954:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 955:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 956:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 957:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 960:
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
