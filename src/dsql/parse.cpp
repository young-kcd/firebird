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
  217,   70,   83,  233,  233,  233,  234,  237,  237,  238,
  238,  238,  238,  238,  238,  238,  238,  238,  238,  238,
  238,  238,  238,  238,  235,  235,  236,    2,  240,  240,
  240,  240,  240,  240,  240,  248,  249,  246,  246,  250,
  250,  250,  250,  250,  250,  250,  241,  241,  251,  251,
  251,  251,  251,  251,  251,  245,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  256,
  256,  256,  256,  256,  256,  256,  256,  256,  256,  253,
  253,  255,  255,  254,  252,  252,  252,  247,  247,  243,
  244,  244,  258,  258,  258,  258,  258,  258,  258,  258,
  259,  259,  242,  260,  260,  261,  261,    8,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  262,  262,  262,
  101,  101,  144,  144,  265,  265,  268,  268,  270,  270,
   53,   53,  267,  267,  267,  267,  267,  267,  267,  267,
  267,  275,  275,  266,  266,  266,  266,  276,  276,   61,
   61,   61,   55,   55,  272,  272,  272,  269,  269,  269,
  280,  280,  280,  279,  279,  278,  278,  278,  273,  273,
  281,  281,  281,  282,  282,  274,  274,  274,  274,  283,
  283,   18,   18,   18,  285,  285,  285,   16,   16,   16,
  287,  288,  291,  291,  289,  293,  293,    4,   15,  292,
  292,  294,  294,  295,  295,  284,  296,  296,  297,  297,
  298,  298,  298,  298,  299,  299,  300,  300,  301,  301,
  303,  303,  303,  304,  304,  304,  305,  305,  305,  302,
  307,  307,  307,  308,  308,  306,  306,  309,  311,  311,
  310,  310,  286,   17,  312,  312,  312,  313,  313,  317,
  317,  318,   65,   65,   65,  320,  320,  319,  319,  314,
  314,  323,  323,  315,  315,  316,  327,  328,  329,  321,
  322,  324,  324,  324,  324,  330,  330,  330,  331,  331,
  331,  221,  221,  222,  222,  334,  334,  335,  335,  337,
  337,  325,  339,  339,  340,  340,  340,  231,  343,  343,
  344,  344,  346,  346,  341,  341,  342,  342,  347,  347,
  209,  209,  348,  348,  230,  230,   31,  232,  232,  232,
  232,  232,  232,  232,  232,  225,  225,  349,  349,  350,
  226,  226,  224,  224,  326,  326,  351,  352,  352,  352,
  352,  352,  353,  353,  354,  354,  355,  355,  356,  356,
  356,  357,  357,   10,   10,  359,  359,    7,    7,  360,
  361,  362,   19,   19,  363,  364,  365,  365,  188,  336,
  336,    3,    3,  367,  367,  367,  369,  369,   61,  368,
  368,  370,  370,   34,   34,   85,   84,   84,  358,  358,
  371,  372,  372,  210,  210,  210,  146,  366,  366,  111,
  111,  111,  111,  111,  376,  376,  376,  376,  376,  376,
  375,  375,  375,  378,  378,  378,  378,  378,  378,  378,
  378,  378,  378,  379,  379,  379,  379,  379,  379,  379,
  379,  384,  384,  384,  384,  384,  384,  384,  384,  384,
  384,  384,  384,  384,  384,  384,  384,  390,  390,  380,
  380,  381,  381,  381,  381,  382,  382,  386,  386,  387,
  387,  387,  387,  385,  388,  383,  383,  374,  374,  374,
  377,  377,  377,  391,  391,  392,  389,  394,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,  156,  156,  156,  395,  393,  393,
  151,  151,  401,  401,  401,  401,  401,  397,  397,  397,
  397,  397,  332,  152,  152,  153,  154,  154,  154,  154,
  154,   51,   51,   20,   20,  239,  402,   54,  123,  271,
  271,   95,  396,  396,  396,  396,  403,  403,  403,  403,
  403,  403,  403,  403,  403,  403,  403,  404,  405,  406,
  406,  406,  400,  400,  398,  399,  399,  408,  408,  409,
  409,  410,  410,  413,  413,  411,  411,  416,  416,  415,
  412,  414,  407,  407,  407,  407,  407,  407,  407,  407,
  333,  333,  155,   48,  277,  122,  148,  373,  165,  211,
  264,   63,   60,  215,  112,   66,  338,   35,   41,  345,
  263,   39,   42,  208,   40,  290,  257,  257,  417,  417,
  417,  417,  417,  417,  417,  417,  417,  417,  417,  417,
  417,  417,  417,  417,
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
    1,    1,    1,    6,    3,    0,    0,    0,    0,    3,
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
    0,    2,    1,    3,    1,    1,    1,    6,    1,    0,
    3,    0,    1,    3,    6,    3,    4,    2,    3,    0,
    1,    3,    1,    1,    1,    2,    1,    1,    1,    2,
    1,    2,    1,    2,    0,    3,    0,    1,    3,    1,
    2,    0,    2,    0,    2,    0,    4,    1,    2,    1,
    1,    0,    1,    3,    2,    1,    1,    2,    1,    4,
    2,    1,    3,    8,    5,    1,    3,    1,    1,    4,
    4,    4,    1,    1,    5,    5,    1,    3,    3,    1,
    1,    7,    7,    5,    3,    0,    1,    1,    1,    2,
    0,    1,    1,    1,    0,    3,    1,    3,    1,    0,
    3,    1,    3,    1,    3,    3,    1,    1,    3,    1,
    2,    1,    3,    3,    1,    2,    3,    3,    3,    3,
    1,    3,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    3,    3,    3,    3,    3,    3,
    3,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    1,    1,    5,
    6,    3,    4,    5,    6,    3,    4,    3,    4,    3,
    4,    4,    5,    4,    4,    3,    4,    1,    1,    1,
    1,    1,    1,    1,    3,    3,   10,   10,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    3,
    3,    3,    3,    3,    3,    3,    3,    1,    1,    1,
    1,    3,    1,    1,    1,    1,    1,    4,    1,    3,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    1,    2,    1,    1,    1,    1,    1,
    2,    1,    1,    1,    1,    1,    4,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    6,    6,    6,
    8,    4,    4,    3,    6,    1,    1,    6,    6,    1,
    1,    4,    6,    4,    5,    3,    5,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  552,
  553,  554,  558,  559,  560,    0,  615,  728,  729,  733,
  734,  460,    0,    0,    0,    0,    0,    0,  398,  570,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  906,    0,   21,  904,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  478,    0,    0,   37,   34,
   36,    0,   35,    0,  987,  989,  991,  990,  996,  992,
  999,  997,  993,  995,  998,  994, 1000, 1001, 1002, 1003,
 1004,    0,    0,    0,    0,   32,   68,   66,  979,  988,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  644,    0,    0,    0,  685,    0,  981,
    0,    0,    0,    0,  107,  986,  561,    0,    2,    0,
    0,    0,    0,  428,  429,  430,  431,  432,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
  445,  446,  447,  448,  449,    0,  426,  427,    0,  972,
    0,  405,  976,    0,  402,  978,    0,  687,    0,  401,
  982,    0,  568,  902,    0,  150,  103,    0,  104,    0,
  218,  968,  767,    0,  102,  145,  975,    0,    0,   98,
    0,  105,  908,    0,   99,    0,  100,    0,  101,  985,
  146,  106,  623,  624,    0,  907,  905,    0,   74,    0,
  973,    0,  971,  486,  479,    0,  485,  490,  480,  481,
  488,  482,  483,  484,  489,    0,   30,    0,   39,  754,
   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  912,    0,  893,  638,
  638,  639,  649,  651,  961,  652,    0,  653,  643,  639,
    0,    0,    0,    0,    0,    0,    0,  587,  576,    0,
  579,  581,  582,  583,  584,  590,  591,    0,  980,  686,
    0,  108,    0,  109,    0,  110,    0,    0,    0,  616,
    0,    0,    0,    0,    0,    0,    0,    0,  461,    0,
    0,  368,    0,  221,    0,  408,  399,  458,  459,    0,
    0,    0,    0,    0,    0,  417,    0,  374,  375,    0,
  574,  572,  903,    0,    0,    0,    0,    0,    0,    0,
  153,  132,    0,   96,    0,    0,  111,    0,  121,  120,
    0,    0,    0,    0,    0,    0,  964,   75,    0,    0,
    0,  730,  731,  487,    0,    0,    0,    0,  871,  899,
    0,  963,    0,  873,    0,    0,  900,    0,  894,  885,
  884,  883,    0,    0,    0,    0,  875,  876,  877,  886,
  887,  895,  896,    0,  897,  898,    0,    0,    0,  901,
    0,    0,    0,    0,    0,  889,    0,  764,  868,  869,
  870,  683,  874,  350,  854,    0,    0,    0,  853,    0,
  681,  850,  851,  852,  855,  856,  857,  888,  913,  914,
  915,  916,  936,  937,  940,  941,    0,  757,    0,    0,
   27,    0,    0,    0,   71,   69,  983,   67,   33,    0,
    0,    0,  759,    0,   52,    0,    0,    0,    0,    0,
  566,    0,    0,    0,  646,  648,    0,  655,    0,  741,
    0,    0,  656,    0,  642,    0,  613,    0,  588,    0,
  585,    0,  586,  611,    0,  606,    0,    0,  580,  768,
  737,    0,    0,    0,    0,    0,    0,  563,  562,    0,
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
  366,    0,  221,    0,    0,   83,    0,    0,  749,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  770,  772,  781,  784,  785,  786,  787,  788,  789,  790,
  791,  792,  793,    0,    0,    0,  890,    0,    0,    0,
    0,    0,    0,  891,  892,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  325,  984,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  756,   28,    0,    0,   44,   72,    0,    0,   26,
    0,  762,    0,    0,    0,  725,    0,    0,    0,    0,
    0,  565,    0,  638,    0,    0,    0,    0,  660,    0,
  556,    0,  910,  555,    0,  589,    0,  597,  593,  592,
    0,    0,    0,  608,  596,    0,    0,  735,  736,    0,
    0,    0,  366,    0,    0,    0,  630,  634,    0,    0,
    0,  130,    0,  468,    0,    0,    0,    0,    0,  368,
  411,    0,  368,  369,  416,  501,    0,  274,    0,  268,
  270,  366,  241,  969,    0,    0,  368,    0,    0,  210,
  366,    0,    0,    0,    0,  246,  240,  243,  244,  245,
  420,  456,  455,  419,  418,    0,    0,    0,    0,    0,
    0,  377,    0,  368,    0,  155,  159,    0,    0,  163,
    0,    0,  532,  533,  549,    0,  546,    0,    0,  539,
  537,  538,    0,    0,    0,    0,  502,    0,    0,    0,
    0,  540,    0,    0,  366,    0,  201,    0,  204,  206,
  207,    0,    0,    0,    0,    0,  221,   78,    0,    0,
   77,  520,  965,  521,    0,    0,    0,    0,  771,  783,
    0,    0,  841,  842,  843,    0,  775,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  953,  954,  955,  956,  957,  958,  959,  960,    0,    0,
    0,    0,    0,    0,  946,    0,    0,    0,    0,  866,
  327,  867,  934,    0,    0,  862,  967,    0,    0,    0,
    0,    0,  682,    0,  872,  766,  765,  758,    0,    0,
    0,    0,    0,    0,    0,    0,   53,   61,   63,    0,
    0,   70,    0,    0,    0,  761,    0,    0,    0,    0,
    0,    0,    0,    0,  647,  650,    0,    0,  667,    0,
    0,  665,  666,    0,    0,  657,  977,  659,  557,  911,
  598,  607,  602,  601,    0,  612,  769,  738,  739,  366,
    0,    0,  640,  622,  621,    0,  182,  176,  183,    0,
  179,  467,    0,    0,  178,    0,  189,  170,  169,  909,
  172,  171,  472,    0,    0,    0,  410,  273,    0,  212,
  368,  214,    0,  369,  209,  215,    0,    0,    0,    0,
  423,  424,  368,  366,  369,    0,    0,    0,  395,    0,
  197,  198,  156,    0,  168,    0,    0,    0,    0,    0,
  547,    0,    0,  368,  369,    0,  497,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  199,    0,    0,    0,    0,  354,    0,   82,    0,    0,
    0,    0,   87,   84,    0,    0,    0,  732,  970,    0,
    0,  776,    0,    0,  782,    0,    0,    0,    0,    0,
  819,  818,    0,    0,    0,  826,  844,  836,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  774,    0,
    0,    0,  221,    0,  917,    0,    0,  932,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  942,    0,  952,    0,    0,    0,    0,    0,    0,  933,
  878,   65,   57,   58,   64,   59,   60,    0,    0,   22,
    0,    0,    0,   42,    0,    0,    0,  743,  763,    0,
  726,    0,  742,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  688,    0,    0,    0,    0,    0,
  604,  605,  609,    0,  200,    0,    0,    0,    0,    0,
    0,    0,    0,  190,    0,    0,    0,  406,  224,  225,
  226,  227,  228,  229,  881,    0,    0,    0,  277,    0,
  222,    0,    0,    0,    0,  247,    0,  452,  453,  425,
    0,    0,    0,    0,    0,    0,  368,  476,  369,  167,
  966,    0,    0,  515,    0,  514,  550,    0,  542,    0,
    0,    0,  495,    0,  523,    0,    0,  525,  528,  530,
  366,    0,    0,    0,  119,    0,  205,  368,    0,    0,
  367,    0,   97,    0,  117,    0,   93,    0,    0,    0,
    0,   86,    0,  834,  835,  778,    0,  780,    0,    0,
    0,    0,    0,    0,    0,  837,    0,    0,    0,    0,
    0,    0,    0,  827,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  923,  922,
    0,    0,  919,  918,    0,  927,  926,  925,  924,  921,
  920,    0,    0,  948,    0,    0,    0,  947,    0,    0,
    0,    0,    0,    0,   56,   62,   54,   55,   23,  747,
  748,    0,  745,  752,  753,  750,    0,    0,  724,    0,
    0,    0,  676,    0,    0,    0,  694,  690,  692,    0,
    0,    0,    0,    0,  367,  626,  627,  641,    0,  184,
  180,    0,    0,  195,  191,    0,  369,  882,  280,    0,
    0,  369,  278,    0,    0,    0,  232,    0,    0,  248,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  473,
  517,    0,  518,    0,  134,    0,    0,    0,  138,  140,
  141,  142,  498,  500,  496,  527,    0,    0,  369,    0,
    0,    0,    0,  351,    0,  355,  368,   88,    0,    0,
   90,    0,    0,    0,    0,    0,    0,  846,  845,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  220,  935,  928,
  929,    0,  930,  944,  943,    0,  949,  938,  939,    0,
   40,    0,    0,  727,    0,  679,  677,    0,    0,    0,
  698,    0,    0,  636,  369,  352,  628,    0,  175,  192,
  369,  144,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  301,  302,  304,  305,  306,  309,  267,    0,    0,  208,
  233,    0,    0,    0,    0,  234,  237,    0,    0,  369,
  393,  394,  390,  389,  392,  391,    0,  516,  543,  143,
  131,  139,    0,  368,  264,  126,  369,    0,    0,  370,
  356,    0,    0,   91,   92,   79,    0,  805,  813,    0,
  806,  814,  809,  817,    0,    0,  807,  815,  808,  816,
  802,  810,  803,  811,  804,  812,    0,  945,    0,  744,
    0,    0,  669,    0,    0,  708,  710,    0,  705,    0,
  265,  185,  407,    0,  312,    0,    0,  315,  316,    0,
  308,    0,    0,    0,    0,  307,    0,  337,  338,  293,
  300,    0,  303,  286,    0,  291,    0,  341,  292,  211,
  216,  235,    0,    0,  238,    0,  250,  252,  397,  369,
    0,  372,    0,    0,    0,  361,  362,  363,    0,    0,
   76,   94,    0,  931,    0,    0,    0,  668,    0,  699,
  709,    0,    0,  283,    0,  310,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  332,  333,    0,  349,    0,
    0,    0,    0,  344,  342,  239,    0,    0,    0,    0,
    0,  266,  369,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  673,    0,    0,  716,    0,    0,    0,  282,
  279,  311,    0,    0,  314,    0,    0,    0,  322,    0,
  324,    0,    0,  348,  347,  974,  346,    0,    0,    0,
    0,    0,  249,    0,  256,    0,  255,  373,  365,    0,
    0,  357,  118,    0,    0,    0,    0,  671,  718,  707,
    0,    0,  719,    0,  715,    0,    0,    0,    0,    0,
    0,    0,    0,  335,  334,  343,  345,  236,    0,    0,
  260,  259,  258,    0,    0,  848,  637,  674,  714,    0,
  721,    0,    0,  313,  319,    0,    0,    0,    0,  336,
  263,  261,  262,    0,    0,    0,    0,    0,    0,  339,
  317,    0,    0,    0,    0,  720,  330,  318,  320,  847,
  723,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1555,   28,   29, 1556,
   31,   32,   33,   34,   35,   36, 1557,   38, 1558,  649,
  122,  460,  965, 1200,  123,  715,  966,  124,  464,  720,
  148,  125,  126,  259,  988,  138,  967,  968,  199,  228,
  127,  969,  128,  466,   82,  239,  378,  425,  644, 1101,
  426,  870,  615,  221,  847,  645,  646,  647, 1103,  240,
 1390,   74,  189,   75,  235, 1846, 1313,  220,  225,  227,
  229,  215,  207,  209,  222,  232,  155,  312,  314,  316,
  367,  631,  633,  457,  260,  796,  548,  659, 1037,  371,
  636, 1089, 1305,  581,  743, 1063,  530,  531,  563,  363,
  616, 1075, 1457, 1580, 1248, 1458, 1459, 1460, 1461, 1462,
  660,  216,  208,  359,  589,  360,  361,  590,  591,  592,
  593, 1280, 1031,  533,  534, 1020, 1018,  535, 1025, 1019,
 1238, 1420, 1026, 1027, 1243,  373,  858,  859,  860,  861,
  798, 1044, 1435,  617,  800,  428,  863,  946,  557, 1362,
 1249,  429,  430,  431,  432,  433, 1436, 1437,  566, 1586,
 1739, 1266,  806,  807,  793,  808,  809,  810, 1740, 1741,
 1822,  341,  561, 1257, 1559,  195,  558,  559, 1258, 1259,
 1430, 1715, 1560, 1561, 1562, 1563, 1677, 1564, 1565, 1566,
 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576,
  434, 1807, 1660, 1725, 1811, 1723,  435,  699,  436,  437,
 1108, 1678, 1733, 1734, 1775, 1094, 1311, 1474, 1095, 1096,
  287,  491, 1609,  382, 1230, 1413, 1694, 1695, 1696, 1697,
  989, 1228,  350,  577,  824, 1278,  578,  822,  223,   59,
  345,  200,  163,  328,  186,  335,  192,  784,  781,  336,
  346,  814,  347,  573, 1055,  187,  438,  329,  545,  579,
 1279,   96,  198,  244,  618,  619,  786, 1076,  787, 1077,
 1078,  622,  623,  624,  625, 1069,  874,  626,  627,  628,
  840,  629,  837,   40,   41,   42,   43,   44,   45,  157,
  519,   61,  482,  203,  352,  299,  300,  301,  302,  303,
  304,  305,  306,  307,  749,  505, 1005, 1233,  506,  507,
  754,   46,  162,  322,  526,   47,  521,  522, 1014, 1418,
 1237, 1537,  767,  142,  737, 1534,  726,  485,  289,  143,
  144,  439,  288,  492,  493,  494,  740,  998,  990,  991,
  992,  993, 1642, 1708,  440, 1754, 1222, 1183, 1530, 1531,
 1756, 1650, 1757, 1758, 1759, 1805, 1847,  472, 1212,   48,
   49,  383,   50,   51,  513,  514,  974, 1208, 1392, 1396,
  473,  723,  213,  661,  662,  888,  889,  663,  664,  665,
  666,  667,  668,  669,  670,  671,  672,  673, 1334, 1124,
 1126, 1127,  945,  698,  442,  443,  444,  445,  446,  447,
  448,  237,  449,  450,  451,  452,  929,  453,  454,  455,
  456,  689,  933, 1184, 1179,  690,  130,
};
static short yysindex[] = {                                   6077,
 1207, -101, 3289,  186, 1498,   55, 3435,   54, 9908,   47,
  588, 9161, -101, 1143,  -58, 7490, 1113, 7490,  -43,    0,
  849,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -91,    0,    0,    0,    0,
    0,    0, 9384, 7490, 7490, 7490, 7490, 7490,    0,    0,
  459, 1435, 7490, 7490, 7490,  735, 7490,  544, 7490, 7490,
    0, 7490, 7490,    0,  767,    0,  607,    0,    0,  807,
 7490,    0, 7490, 7490, 7490,  816, 7490, 7490, 7490, 7490,
  544, 7490, 7490, 7490, 7490,    0, 7490,  760,    0,    0,
    0, 1137,    0, 1137,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  882,  908,  221, 1257,    0,    0,    0,    0,    0,
 7490, 7490, 7490,  941, 1006, 1010,  152,  935,  987,  123,
  135,  891,  976,    0, 7490, 1154, 1598,    0, 1076,    0,
 7490, 7490, 7490, 7490,    0,    0,    0, 7490,    0, 1272,
   30, 1237, 1104,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  918,    0,    0, 1435,    0,
  532,    0,    0, 1527,    0,    0, 1536,    0,  549,    0,
    0, 1163,    0,    0, 1146,    0,    0, 1110,    0, 1347,
    0,    0,    0, 1435,    0,    0,    0,  457, 1527,    0,
    4,    0,    0, 1312,    0, 1356,    0, 1137,    0,    0,
    0,    0,    0,    0, 1351,    0,    0, 1244,    0, 1374,
    0, 1273,    0,    0,    0, 1244,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 9128,    0, 7490,    0,    0,
    0, 5078, 1334, 5630, 7490, 1251, 1403, 1715, 1448, 1465,
 5078, 1387, 5630, 1370, 1402, 1302,    0,10819,    0,    0,
    0,    0,    0,    0,    0,    0, 8400,    0,    0,    0,
 1362, 7490, 1469, 1382,  966, 7490, 1407,    0,    0, 1598,
    0,    0,    0,    0,    0,    0,    0, 7490,    0,    0,
 1527,    0, 1312,    0, 1137,    0, 1477,10819, 1437,    0,
 1429, 1419,  512, 1359, -110, 1361,  931, 1104,    0, 1596,
 1081,    0, 7490,    0,  918,    0,    0,    0,    0, 7490,
 1476, 4261, 1441, 4781, 1857,    0, 9384,    0,    0,  742,
    0,    0,    0, 1859, 1859, 1435, 1543, 1435, 3204, 1110,
    0,    0,10681,    0, 7490, 7490,    0, 1476,    0,    0,
 1647,  600, 1919, 7490, 1674, 7490,    0,    0, 1921,  139,
 5694,    0,    0,    0, 1944, 1947, 1948, 1515,    0,    0,
 1951,    0, 1952,    0, 1960, 1962,    0, 1964,    0,    0,
    0,    0,    0, 1968, 1548, 1551,    0,    0,    0,    0,
    0,    0,    0, 1974,    0,    0, 9225, 1975, 1977,    0,
10819,10819, 8498, 7490, 1978,    0, 2529,    0,    0,    0,
    0,    0,    0,    0,    0, 1982, 1930,    0,    0, 1983,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  301,    0, 7490, 1599,
    0, 7490, 7490,  183,    0,    0,    0,    0,    0, 7490,
 7490,  348,    0, 7490,    0, 1714, 7490, 1988, 5078, 1634,
    0, 7490, 9464, 1649,    0,    0,10819,    0, 2529,    0,
 1717, 1994,    0, 1775,    0,  136,    0, 1133,    0,  474,
    0,  474,    0,    0, 2001,    0,  110, 1626,    0,    0,
    0, 2009,  174, 1999, 1476, 2021, 1799,    0,    0, 2071,
 2024,    0,    0, 1695, 1576,    0,    0,  160, 1730, 1734,
    0,    0,    0,    0, 1435,    0,    0, 1735,    0, 1859,
 1859, 1859, 1859,    0, 2046,    0, 1814, 1815,    0,    0,
    0, 1795,    0,10681,    0,    0,10681,  333,    0, 1527,
 1833, 7490,12624,    0,    0,  815,    0, 7490,  466, 1536,
 1702, 1666, 1630,    0,    0,    0,    0,  527, 1704,    0,
 1651,  544,    0, 1435,    0, 1828, 1759,    0,    0, 3204,
    0,    0,    0,    0,  268, 1663,    0,    0,    0, 1673,
    0, 1722, 2072,    0, 1778, 2084,  569,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2038,
  207,    0,    0,    0,    0,   80, 2091, 2092, 2084, 1527,
    0, 1822,    0, 1878,    0, 1435, 1435,    0, 4261,  549,
    0, 1761,    0, 1739, 2101,    0, 6627, 2536,    0, 1765,
 1770, 2119, 6297, 2120,    0,    0,    0, 5597, 8426,  526,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  984, 9703,  237,    0, 7490,10819,  994,
 1140, 1168, 2079,    0,    0,10819, 6527, 2529, 1711,   -6,
10819, 9703,  690,  690, 1143, 2029, 1578, 2122,    0,    0,
 9783, 7490,10819,10819,10819,10819,10819, 9703,10819, 1301,
 7490,    0,    0, 7014, 1736,    0,    0, 1700, 5630,    0,
 1849,    0, 1631, 1143, 2130,    0, 1849, 7014, 1865, 1866,
 7490,    0, 2029,    0, 2106, 2290, 1721, 9703,    0, 7490,
    0,  648,    0,    0,   83,    0, 1726,    0,    0,    0,
 7490,  587, 7490,    0,    0, 7490, 7490,    0,    0, 9703,
 1913, 4261,    0, 1693,10819, 7490,    0,    0, 1435, 1435,
 1435,    0,   26,    0, 1651, 1651, 1738,  544,  931,    0,
    0, 2161,    0,    0,    0,    0, 1920,    0, 7490,    0,
    0,    0,    0,    0, 1933, 2169,    0, 1922, 1928,    0,
    0, 1869, 1874, 1137, 1941,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  544, 7490,12705, 1834, 1840,
 1842,    0,  544,    0, 1514,    0,    0, 1811, 1435,    0,
   61, 1816,    0,    0,    0,  544,    0, 2072,  190,    0,
    0,    0, 1935,  190, 1812,  190,    0, 2197,  544,  544,
  544,    0, 1476, 7490,    0, 1885,    0, 1686,    0,    0,
    0,  742, 1825, 7490, 1691,  100,    0,    0, 2205, 1973,
    0,    0,    0,    0,  139, 7490, 1831, 6775,    0,    0,
 1831, 6297,    0,    0,    0, 6825,    0,  148,  540,10819,
10819, 6872, 2210,  747, 7130,10819, 7282, 1338, 7391, 7650,
10022, 7802, 7911, 8170, 6527, 6527,10819,10819, 1987,10819,
 2211,10819, 2212, 2137,10819,10819,10819,10819,10819,10819,
    0,    0,    0,    0,    0,    0,    0,    0, 1934, 1464,
  -10,10819,   51, 9703,    0, 6527, 2493, 2214,  891,    0,
    0,    0,    0, 2529, 1713,    0,    0, 1984,  690,  690,
 1310, 1310,    0,  738,    0,    0,    0,    0, 7490, 7490,
 7490, 7490, 7490, 7490,  188, 2216,    0,    0,    0, 7014,
 1884,    0,  443, 1904, 7490,    0,  891, 9703, 1904, 2230,
 2216, 7014, 6978, 1955,    0,    0,  729, 2236,    0, 2240,
 1059,    0,    0, 6527, 1957,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  620,    0,    0,    0,    0,    0,
 1729, 1825,    0,    0,    0, 2244,    0,    0,    0, 1755,
    0,    0, 1859, 2014,    0,   26,    0,    0,    0,    0,
    0,    0,    0, 2251, 6527, 3138,    0,    0, 1993,    0,
    0,    0, 3138,    0,    0,    0, 1137, 1137, 1794, 2256,
    0,    0,    0,    0,    0,  375,   -7,  969,    0, 2033,
    0,    0,    0, 7490,    0,  186, 1854, 1888, 1920, 2257,
    0, 1651, 1881,    0,    0, 1242,    0, 2250, 7490, 1279,
  544, 2270, 2271, 2274, 2053,  549, 1993, 1859, 1734, 4261,
    0, 1704,  891, 1868, 1889,    0, 1949,    0,  544, 1931,
 2015, 6627,    0,    0,  544, 1836, 2025,    0,    0, 2285,
 2292,    0, 6527, 6527,    0, 6527, 6527, 2503, 2529, 2295,
    0,    0, 2529, 2297,10261,    0,    0,    0, 1989, 2299,
 2529, 2300, 1705, 2302, 2529, 2304,10819,10819, 2210,10819,
10341, 2308, 2529, 2309, 2313, 2529, 2318,10819, 2529, 2320,
 2529, 2321, 2322, 2529, 2324, 2325, 2529, 2326,    0, 2107,
 2157, 2225,    0, 2337,    0, 2606,10819,    0, 2692, 2700,
 2818, 2945, 2960, 2970,10819,  544, 9703, 2529, 1937, 9703,
    0,10819,    0, 2060,   -8,10819, 9703,10819,10819,    0,
    0,    0,    0,    0,    0,    0,    0, 2043, 7014,    0,
 7014,  188, 2216,    0,  203,  203,  131,    0,    0, 8400,
    0, 1886,    0, 2230, 2216, 2330, 6978,   92, 1059, 2334,
 9703, 1775, 2290, 1995,    0, 1997, 2000, 2028, 2103, 2056,
    0,    0,    0, 1993,    0, 1868,  700, 1967, 1435, 2004,
 1651, 2005, 1651,    0, 6527,  441, 1450,    0,    0,    0,
    0,    0,    0,    0,    0, 1953, 2127, 1993,    0,10819,
    0, 2113, 2003, 1794,  767,    0, 6527,    0,    0,    0,
 2030, 2032, 2045, 2047, 2048, 2049,    0,    0,    0,    0,
    0, 2349,  186,    0, 1738,    0,    0,  190,    0, 3138,
 2023,  190,    0,  190,    0, 1920, 2366,    0,    0,    0,
    0,  742, 2127, 1651,    0, 1734,    0,    0, 8400, 2146,
    0,  479,    0, 2244,    0, 2406,    0, 2412, 1435,  -77,
 2414,    0, 1435,    0,    0,    0, 2107,    0, 2107,10819,
 2039, 2039, 1143, 2421, 1898,    0, 2039, 2039,10819, 2039,
 2039, 2803, 2529,    0, 1876,10819, 2529, 2039, 2039, 2039,
 2039, 2529, 2039, 2039, 2039, 2039, 2039, 2039,    0,    0,
10681, 2422,    0,    0, 3145,    0,    0,    0,    0,    0,
    0, 3247,  144,    0, 9703, 2139, 2036,    0, 9703, 3269,
 1911, 1784, 2529, 2083,    0,    0,    0,    0,    0,    0,
    0, 2037,    0,    0,    0,    0, 1717, 9703,    0, 6978,
 2330, 1775,    0, 1912, 7490, 1059,    0,    0,    0, 2290,
10819, 6527, 2080, 2127,    0,    0,    0,    0, 1859,    0,
    0, 1435, 1514,    0,    0,  697,    0,    0,    0, 7490,
 7525,    0,    0, 2529, 2195, 2113,    0,  978, 7490,    0,
 2141,  714, 2042, 2148, 2051, 2186, 2156, 2194, 1993,    0,
    0, 2458,    0, 2459,    0, 2150, 2195, 2023,    0,    0,
    0,    0,    0,    0,    0,    0, 1993, 1704,    0, 1514,
 2033, 2183, 2126,    0, 1825,    0,    0,    0, 2133,  -98,
    0, 1920, 2142, 2529, 2471, 2473,  891,    0,    0, 2474,
 2489, 2529, 2491, 2500,10819,10819, 2529, 2501, 2502, 2508,
 2518, 2521, 2523, 2524, 2532, 2534, 2537,    0,    0,    0,
    0,  544,    0,    0,    0, 9703,    0,    0,    0, 1721,
    0,  203, 1721,    0, 7490,    0,    0, 1346, 2529, 2538,
    0,  526,  705,    0,    0,    0,    0, 1651,    0,    0,
    0,    0,10681, 2904,  492, 2525,  465, 2545, 2246,10819,
 2530, 2548, 2539, 2542, 2544, 2546, 2255, 2547,    0,    0,
    0, 2283,12506, 2550,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2555, 7490,    0,
    0, 2245, 2263, 7490, 1794,    0,    0, 1137, 7490,    0,
    0,    0,    0,    0,    0,    0, 2127,    0,    0,    0,
    0,    0, 2127,    0,    0,    0,    0, 2327, 1721,    0,
    0,10819, 1435,    0,    0,    0, 1435,    0,    0,10819,
    0,    0,    0,    0, 2529, 2529,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2579,    0, 1957,    0,
 1957, 2583,    0, 6527,10819,    0,    0, 2258,    0, 2584,
    0,    0,    0,  635,    0,10580, 7490,    0,    0,10819,
    0, -118, 2287, 6527, 2529,    0, 6527,    0,    0,    0,
    0, 2616,    0,    0,  581,    0, 2174,    0,    0,    0,
    0,    0, 1794, 1137,    0, 2249,    0,    0,    0,    0,
 2033,    0, 1980, 2589, 1059,    0,    0,    0, 1957, 2529,
    0,    0, 1784,    0, 2056, 2056, 7490,    0,  526,    0,
    0, 6039, 3138,    0, 2575,    0, 2147, 9128, 2096,10819,
 2616,  773, 2577,  782,  842,    0,    0, 2594,    0, 7490,
 7490,  186,  116,    0,    0,    0, 2249,   29, 1794, 2260,
 2261,    0,    0, 1059, 2609, 2327, 2311, 2056, 2611, 1721,
 2080, 2080,    0, 1936, 7490,    0, 2615, 2613, 1214,    0,
    0,    0, 2248, 2616,    0, 1128,  149, 2224,    0, 2357,
    0, 2616, 2017,    0,    0,    0,    0, 7525,  581, 1794,
  606,  606,    0, 2362,    0, 2222,    0,    0,    0, 1059,
 2327,    0,    0, 1957, 2278, 2278, 7490,    0,    0,    0,
 6039, 2627,    0, 7490,    0, 2215, 2617, 1385, 2616, 2384,
 2374, 7525, 7525,    0,    0,    0,    0,    0, 2232,   34,
    0,    0,    0, 1558, 2056,    0,    0,    0,    0, 7490,
    0, 2616, 2635,    0,    0,  170, 7490, 7525, 2378,    0,
    0,    0,    0, 6527, 2080, 2643, 2655, 1942, 7525,    0,
    0, 7525,  526, 2278, 7490,    0,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  245,  576,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  113, 5952,    0,    0,    0,    0,    0,    0,
 2701,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  103,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1077,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2363,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2319,    0,    0,
    0,  272,    0,  272,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2323,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1205,    0,
    0, 8737, 6211,    0,    0,    0, 1213,    0,    0,    0,
 1763,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  109,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    5,    0,    0,    0,    0,  740,    0,
    0, 1247,    0,    0,    0,    0,    0,  229,    0,11341,
    0,    0,    0,    0,    0,    0,    0,    0,    5,    0,
  484,    0,    0, 2663,    0,    0,    0, 2444,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1255,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1268,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  854,    0,    0,
    0,    0,    0,    0,    0, 8073,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1090,    0,    0, 1315,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    5,    0, 2663,    0, 2444,    0, 1333,    0,    0,    0,
    0,  371,    0,    0,    0,    0,    0, 1355,    0, 2431,
    0,    0,    0,    0, 1406,    0,    0,    0,    0,    0,
 2448,  809,12758,    0, 1424,    0,    0,    0,    0,  219,
    0,    0,    0, 2264, 2264,    0,    0,    0, 1425,  543,
    0,    0,    0,    0,    0,    0,    0, 2448,    0,    0,
 1470,    0,    0,    0,    0,    0,    0,    0, 9638, 2332,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1903,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2569,    0,    0,    0, 2873, 3177,    0,
    0,    0,    0,    0,    0,    0,  656,    0,    0,    0,
    0,    0,    0,    0,    0,  178, 3481, 2259,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1433,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1480,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  947,    0,
    0, 2408,    0, 2181,    0,    0,    0,    0,    0,  716,
    0,  716,    0,    0, 1119,    0, 1200, 1574,    0,    0,
    0,    0, 1255,    0, 2448,    0,    0,    0,    0,  161,
  169,    0,    0,  129,    0,    0,    0,  734,    0,  709,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2264,
 2264, 2264, 2264,    0,  877,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2465,
    0,    0,    0,    0,    0, 2431,    0,    0, 1123,    0,
 2266,    0,    0,  639,    0,    0,    0,    0,  211,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1520,
    0,    0,    0,    0, 2864, 4138,    0,    0,    0, 4658,
    0,    0,11156,    0,    0,11168,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3658,
 3659,    0,    0,    0,    0,11180, 7133,    0,11168,    5,
    0,    0,    0,    0,    0,    0,    0,    0,  809,  871,
    0,    0,    0,    0, 2329,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11449,11610,11753,    0,    0,  599,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10899,    0,10899,    0,    0,    0,10899,
10899,10899,    0,    0,    0,    0,    0, 2279,    0,    0,
    0,    0, 4521, 4825, 8889, 2694,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  260,    0,    0, 5952,    0,    0,  260,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  962,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  809,    0,  364,    0,    0,    0,    0,    0,    0,
    0,    0, 1202,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1051,    0,    0,    0,
    0,    0,    0,    0, 2704,    0,    0, 3736, 3732,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  273,  400,
  401,    0,    0,    0,  157,    0,    0,    0,    0,    0,
    0, 3051,    0,    0,    0,    0,    0,11156,    0,    0,
    0,    0,  776,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2448,    0,    0,  164,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10196,    0,    0,    0,  112,
    0,    0,    0,    0, 2437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,10899,    0,
    0,    0,    0, 1731,    0,    0,    0, 3785, 5041, 5345,
 4001, 4305,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1532, 1537,    0,    0,    0,    0,
    0,    0,    0, 1566,    0,    0, 8737,    0, 1566, 1572,
 1602,    0,    0,    0,    0,    0,    0,  829,    0,  856,
 1438,    0,    0,    0, 1573,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  171,    0,    0,    0,    0,
    0,    0, 2264, 2294,    0, 1699,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2482,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1445,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1618,
    0,    0,    0,    0,    0,    0,    0,    0, 3168,    0,
    0,    0,    0,    0,    0,    0,    0, 1432,    0,    0,
    0,    0,    0,    0,    0,  871, 2482, 2264, 1642,  809,
    0, 2490, 8737, 1643,  118,    0,    0,    0,    0,10196,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5178,    0,
    0,    0, 5899,    0,    0,    0,    0,    0,    0,    0,
 6291,    0, 8164,    0, 8699,    0,    0,    0,    0,    0,
    0,    0, 9024,    0,    0, 9104,    0,    0,11207,    0,
11816,    0,    0,11852,    0,    0,11888,    0,    0, 5189,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2328,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1532, 1659,    0, 2336,  321,    0,    0,    0,    0,
    0,    0,    0, 1664, 1665, 1680,    0,    0, 2413, 1744,
    0, 5425,    0, 2423,    0, 2434, 2438,    0,    0, 1320,
    0,    0,    0, 2482,    0, 1643,    0,  303,    0,    0,
    0, 2316,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 8616,    0, 2484,    0,    0,
    0, 2067,    0, 1445, 2363,    0,    0,    0,    0,    0,
  477,  677,  720,  825,  830,  881,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  833,    0,    0,    0,    0, 3811,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1682,    0,    0,    0,    0,
    0,    0,    0, 1683,    0,    0,    0,    0,    0,  201,
    0,    0,    0,    0,    0,    0,  285,    0,  300,    0,
    0,    0, 8889,    0,    0,    0,    0,    0,    0,    0,
    0,    0,11924,    0,11960,    0,12032,    0,    0,    0,
    0,12104,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1792,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1712, 1541,    0,    0,    0, 1571,    0,    0,    0,    0,
    0,    0,  158,    0,    0,    0,    0,    0, 2264,    0,
    0,    0,  542,    0,    0,    0,    0,    0,    0,    0,
 2449,    0,    0, 2743, 1603, 3962,    0, 2431,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2482,    0,
    0,    0,    0,    0,    0,    0, 1728,  885,    0,    0,
    0,    0,    0,    0,    0,    0, 2482, 2490,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  217,    0,12217,    0,    0,10899,    0,    0,    0,
    0,12249,    0,    0,    0,    0,12333,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  654,
    0,  321, 1217,    0, 3475,    0,    0, 2413,  233, 1679,
    0, 1853, 2745,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1439, 1488,    0,    0,    0,    0,    0,    0,
    0,    0, 2478,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3913,    0,    0,  132,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  579,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12362,12396,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
  718, 4146,    0,    0,    0,    0,    0, 2754,    0,    0,
    0,    0,    0, 2737,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2738,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2495,    0,    0,    0,
    0,    0, 3913,  480,    0,  107,    0,    0,    0,    0,
    0,    0,    0,  240, 1383,    0,    0,    0,  108, 2743,
    0,    0,    0,    0,  311,  957,    0,    0, 4666,    0,
    0, 2745,    0,    0,    0,    0,    0,   79,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  525,    0, 1445,  631,
  875,    0,    0, 2413, 1918,    0,    0,  182,    0,  654,
  254,  281,    0,    0, 1483,    0,    0, 2763,    0,    0,
    0,    0, 2746,    0,    0,    0, 2504,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3913,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1434,
    0,    0,    0,  399, 2769, 1139,    0,    0,    0,    0,
 2745,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2413,  311,    0,    0,    0,    0,    0,
    0,    0, 2752,    0,    0,    0,    0,    0,12425,    0,
    0,    0,    0,    0,  254, 2771,    0,    0,    0,    0,
    0,    0, 1233, 2769,    0,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2813,    0,    0, 2815,
    0,    0,    0,    0,    0,    0,   11,    0, 2820,    2,
  259,  -70, -622, 1616,  264, -297, -538, 2809, 2549,    0,
  -28,    0, 2557, -100,  -40,    0,  516, -919,   -3,  -61,
  -36, -172, 2559, 2121,    0,    0,    0,  601,    0,    0,
  -19, 1723, -574,    3, -877, 2199, 1985, -299, 1747, 2764,
 -285,    0,  -54,    0, -482,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -579, -630, 1756, -280,  948, -928,    0,
    0,    0,    0, -334, -123, -742, 1766, -327,  -44,    0,
 -512,    0,    0, 1399, -978,    0, 1400,    0,    0, 2540,
 -621,  -35,    0,    0,    0,    0, 2506,    0, 2272, 2551,
 -175, 1788, -729,    0,    0,    0, -708,    0,    0,    0,
    0,    0,    0, 1843,    0, 2562, 2110, 1789, 2526, 2541,
    0, 1224,    0, -529, 2082,  -33, -642, 1303,    0,    0,
    0, -947, -917, -906, -272, -902,    0, 1449,-1012,    0,
 1147,-1193, 1454,    0, 2331,    0,    0,    0, 1153, 1148,
 1114,   87, -277, -782, -292,    0,    0, 2109,    0, 1637,
    0,    0, -465,    0,    0,    0,    0, -177,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1183,    0, 1241, -442,    0,    0, -355,    0, -383, -246,
 1069, 1230,    0, 1132,    0, 1901, 1685, 1507,    0,-1076,
 -842, -353,    0, -493, -652, -562,    0, -590, 1231,  846,
-1396, -291, -526, -747, -833,-1250,    0,    0,    1,    0,
    0,    0,    0,    0, 2580,    0,    0,    0,    0, 2591,
 2360,    0,    0,    0,    0,    0,   -9, 2608,    0,    0,
    0,    0,  -15,    0,    0,    0, -341, 2093, -339, 1645,
 -454,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2312,    0, 2100,    0,    0,    0,    0,    0,    0,   25,
    0, 2927,    0,    0,    0,    0,    0, 2642,    0,    0,
    0,    0, 2445,    0, 2442,    0,    0,    0, 2200,    0,
    0, -948, -802,    0,    0,   85,    0, 2185,    0,    0,
    0,    0,    0, -625,-1027,-1434,  482, -199,  909,    0,
 2810, -127, 1292,    0, 2208, -649, -996,-1414,    0, -832,
 1970,    0,    0,    0, -143,    0,    0, -244,    0, 1309,
 1426,    0, 1157,    0, 1206,    0, 1105,    0,    0,    0,
    0, 2454,    0,    0,    0, -384, 2235, 1990, -810,    0,
    0,    0, -687, -449,   90,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3322, 1526,
 1829,    0, -592,    0,    0,    0, -897,    0,    0,    0,
 1724,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -76, 1791,    0,    0,
};
#define YYTABLESIZE 13275
static short yytable[] = {                                     129,
  151,  191,  129,  261,   79,   78,  150,  310,  156,  214,
   37,  441,  281,  284,  490,  194,  280,  283,  210,  758,
  582,  620,  957,  621,  785,  909,  219,  788,  197,  211,
  245,  588,  254,  799,  554,  249,  231,  764, 1218,  697,
  224,  744,  206,  188,  190,  193,  196,  150,  201,  250,
  797,  552,  248,  212,  190,  217,  256,  196,  255,  150,
  201, 1021,  230,  129, 1261,  931,  226,  151, 1007,  939,
 1440,  241,  871,  150,  243,  190,  252,  241,  217,  193,
  196,  486,  150,  201,  230,  129,  722,  196, 1250,  253,
  634,  465,  315,  251,  650, 1250, 1188,  267,  977,  269,
  465, 1067,  619,  268, 1066,  980,  257,  697,  631,  291,
 1009,  311,  571,  862, 1092, 1262,  954,  353, 1251,  849,
 1012,  212,  150,  212,  313, 1251, 1270,  375,  633, 1252,
  511,  755, 1402, 1254, 1210,  217, 1252,  328, 1255, 1100,
 1254,  309,  196,  150,  230, 1255, 1291,  257,  156, 1039,
  257,  544,   85,  753, 1219,   85,  196,  706, 1046, 1779,
  625,  619,  278,  125,  512,  257,  697,  631,  618,  337,
  632,  571,  755, 1049,  282,  755,  353,  326,  441,  729,
  742,  702,  317,   77, 1513,  279, 1016,  633, 1115,  981,
  755, 1286, 1772,  279,  364,  265, 1480,  279,  706,  770,
  476,  625,  772,  879,  625,  775,  776,  777,  778,  618,
  396, 1698, 1087, 1772,  517,  196,  706,  757,  475,  625,
 1607,  620,  125,  621,  458, 1405,  719,  618,  151,  632,
   77, 1199,  700,  461, 1072, 1476,  326,  761,  497,  358,
  702,   89,  461,  995,  571,  320,  537,   77,  212, 1438,
 1309,  905,  150,  905,  467,  129, 1614,  524, 1308,  746,
  524,  150,  588,  467,  265,  279,  145,  504,  269,  396,
  135,  369,  382,  700,  510,  136,  700,  475,  911, 1386,
  706, 1388,  193, 1159, 1160,  805,  150,  151, 1053,  319,
  717,  700, 1753, 1273,  706,  160, 1698,  846,  358,  553,
  934,  935,  181,  571, 1303,  368,  556,  831,  211,  832,
  569, 1455,   77,  571, 1185,  755, 1795, 1796,  746,  131,
  522,  706,  642,  212,  630,  777, 1658,  512, 1211, 1781,
  212,  382,  212, 1842,  212,  843,  583,  574,  585,  706,
  779,  712, 1250,  181,  711,  640,  181, 1202,  161, 1698,
 1450,  702,  638,   60, 1520,  196,  201, 1180, 1181, 1214,
 1216,  181,  632,  629,  150,  500,  193, 1234,  370, 1523,
  635,  146, 1251,  790,   83,  906,  789,  906,  147,  522,
   79, 1023, 1828, 1252, 1073, 1843,  132, 1254, 1481, 1659,
 1406, 1685, 1255, 1615, 1698, 1393,  274,  515, 1611,  380,
  381,  275,  490,  523,  629, 1525,  538,  629,  730, 1113,
 1854, 1269, 1810, 1246,  700,  269, 1263, 1264, 1465, 1778,
  619,  716,  629, 1438, 1177,  196, 1379,  752,  769,  635,
  713, 1203, 1112,  984, 1274,  196,  716,  510,  697,  697,
 1743,  721,  619, 1215,  936,  727,   14,  938,  631,  150,
  461, 1414,  196,  467, 1024,   97,  196,  825,  380,  381,
  150, 1512,  158,  953,  150,  490, 1315,  196,  633,  150,
 1782,  273,  156, 1849,  396,  706,  387, 1394,  625,  755,
 1386, 1427,  475,  845,  196,  196,  618,  490,  328, 1736,
  524, 1326, 1327, 1099, 1328, 1329,  285,  706, 1542, 1034,
  625, 1182, 1036, 1577,  196, 1578,  732,  151,  618,   89,
  632,  125,  196,  196,  755,  773, 1042, 1314, 1030,  196,
  755,  502,   85,  755,  257,  524,  848,  871,  151,  801,
 1421,  161, 1335, 1114,  985,  387,  382,  196,  755,  910,
 1605,  196,  152, 1060,  706, 1783,  465,  697,  571,  196,
  700,  196,  794,  212, 1468, 1453,  619,  151,  794, 1302,
  181,  697,  631,  700,  827,  257, 1102,  358,  257,  648,
  358,  353,  700,  181,  196, 1085,  151, 1528,  704, 1008,
  277,  181,  633,  257,  826,  151, 1818,  326, 1030, 1107,
 1209,  151,  277,  277, 1401,   79,   76,  706,  703,  196,
  196,  152,  181, 1033, 1616,  211, 1651,  257,  181,  151,
  181,  706, 1653,  512,  625,  475,  856,  857, 1000,  700,
  741,  702,  618, 1426,  632,  381,  746,  524,  700,  212,
  253,  181,  755,  648, 1604,  702,  718,  704,  873,  703,
  706, 1198,  913,   76, 1620, 1442,  791,  277,   79,  872,
  181, 1028, 1029,  571,  755,  684,  264,  703, 1467,  181,
   76, 1688,  625,  380,  381,  181, 1597,  706,  217,  382,
  777,  253,  700,  196,  253, 1750,  388,  958, 1692,  358,
 1540,  629,  524,  181, 1603,  779,  700,  522, 1241,  253,
  716,  358,  947,  358,  704, 1713,  684,  702,  648,  684,
  212,  212,  905,  629,  467,  490,  702, 1487,  463,  467,
  635, 1640, 1271, 1539,  684,  599,  853,  697,  467,  384,
  181,  196,  504,  510, 1006,   76,  196, 1606,  211,  697,
  997,  706,  458,  177, 1760,  388,  707, 1541, 1475,  376,
  387,  150,  880,  150, 1218,  151,  212,  887, 1524, 1017,
 1017, 1022,  212, 1304, 1590,  556,  212,  755,  697,  755,
 1260, 1742, 1205, 1253,  724, 1250,  755,  463,  987,  122,
 1253, 1749, 1268,  196,  599,  135,  697, 1662,  384,  212,
 1032, 1189, 1441, 1052, 1054,  697, 1192,  905,  338, 1195,
 1532,  725,  177, 1290,  697, 1251,  380,  381,  376,  196,
 1102, 1116,  257, 1381,  257,  348, 1252,  212,  212, 1065,
 1254,  257,  196, 1768, 1788, 1255, 1272,  629, 1051,  181,
  196,  152, 1770, 1059,  383, 1086,  906,  819,  680,  386,
 1191,  512,  136, 1454,  135, 1097, 1070, 1404,  379, 1464,
  115,  196,  152, 1729,  150,  841,  384,  196, 1508,  196,
  747, 1082, 1083, 1084,  150,  662, 1397,  755,  365,  527,
  133,  149,  755, 1361,  820,  339, 1109,  202,  528,  680,
  196,  152,  680,  387,  254,   79,  465,  115, 1206,  755,
  385,   14,  349,  383,  137, 1772,  755,  680,  386,  196,
  152,  136, 1730, 1657,  366, 1093,  662,  196,  196,  152,
 1771, 1196,  257, 1731,  196,  152,  704,  159,  253,  704,
  253,  906,  115,  842,  662,  254,  703,  253,  254, 1193,
  755,  748,  196,  152,  257, 1117,  703, 1197,  242,  703,
  812,  257,  813,  254, 1043,  465, 1658,  490,  703,  385,
  388,  510,  441,  137,  122,  122,  740,  637, 1000,  467,
  196,  201,  467,  230,  129, 1472,  702, 1194,  905,  196,
  467,  704,  684,  684, 1432,  257,  463,  880,  821,  702,
  196,  880,  467,  467,  703,  905,  441,  196,  772,  463,
  755,  704, 1819,  384,  704,  703, 1705,  740, 1706, 1003,
  740,  177, 1826, 1827,  703,  218, 1449,  702,  376, 1659,
  575,   76,  704,  376,  177,  740, 1004, 1732,  253,  376,
 1469,  576,  177, 1654,  463,  702,  463, 1253,  704,  620,
  704,  621, 1709, 1820, 1231,  257, 1639, 1471,  529, 1641,
  253,  233,  704,  177,  905,  968,  490,  253,  703,  177,
  704,  177, 1722,  905,  603, 1724, 1748, 1646,  697,  704,
  524, 1860,  703,  135, 1281,  135,  211,  599,  196,  152,
  204,  205,  177,  458,  236,  684,   79, 1282,  234, 1281,
 1821,  253, 1647,  388,  427, 1232,  573, 1391, 1391, 1395,
  212,  177,  906, 1297, 1538,  957,  242,  212,  383,  594,
  177,  524,  680,  386,  524,  603,  177, 1676, 1128,  906,
 1374, 1317, 1744, 1376,  697,  277,  684, 1321,  968,  524,
  368,  524,  136,  697,  177, 1699,  384, 1423,  600, 1425,
  599, 1535,  457,  599, 1129,  490,  463,  242,  238,  376,
 1648,  253,  999,  802,  465,  573,  376,  246,  619,  599,
  376, 1825, 1751, 1752,  385,   14,  680,  465,  594,  680,
  285,  177,  254,  135,  254, 1790,  680,  805,  906,  680,
  257,  254,  368,  680,  137,  599,  457,  906,  680,  706,
  704,  680,  705,  662,  707,  330,  258,  600, 1373,  619,
 1470,  457,  465,  662,  465, 1792,  662,  680,  105,  467,
  487,  467, 1416,  286,   98,  662, 1612,  619,  495,  610,
 1824,  187, 1417,  427,  569,   79,   79,  467,  242,  540,
  740,  680,  578,  196,  803,  680,  704,  106,  107, 1017,
  108,  383, 1853,  331,  680,  484,  386,  109,  110,  111,
  112,  113,  364,  114,  489,   99,  115,  116,  117,  118,
  119,  680,  662,  285,  120,  121,  575,  100,  500,  242,
  177,  662,  254,  285,  704,  804, 1794,  704,  610,  541,
  187, 1527, 1845,  569,  262,  520,  740,  328,  680, 1275,
  760,  578,  101,  364,  254,  704,  364,  385, 1767,  704,
  680,  254,  680,   79, 1452, 1292,  907,  512,  680,  704,
  263,  364,  704,  542,  465,  662,  915,  760, 1514, 1479,
  266,  704, 1517, 1483, 1689,  575, 1276,  662,  524,  662,
 1690,  543, 1816,  704,  577,  254, 1726,  680,  680,  702,
  680, 1808, 1292, 1691,  524,  270,  328,  680,  680,  680,
  680,  680,  564,  680, 1293,  332,  680,  680,  680,  680,
  680,  102,  956,  702,  680,  680, 1839, 1840,  704,  501,
  524,  103,  702,  333,  403, 1582,  524,  704, 1110,  524,
  702,  323, 1111, 1833,  688, 1726, 1836,  550,  693,  694,
  696, 1296, 1851,  577,  324,  254,  104, 1583,  702, 1224,
  551, 1643,  359, 1858, 1584, 1543, 1859,  334,  271, 1848,
  467,  564,  272, 1225, 1391,  309,  211,  510,  740,  285,
  196,  704, 1017, 1747,  502,  404,  740,  702, 1726,  325,
 1588,  326,  703,  403, 1652,  704, 1814, 1226, 1585,  512,
  212,  503,  276,  400,  160, 1727,  359,  285, 1772,  150,
  733,  594,   43,  360,  735,  740,  740,  663,  740, 1638,
 1253,  359,  917, 1835,  251,  740,  740,  740,  740,  740,
 1726,  740, 1747, 1726,  740,  740,  740,  740,  740,  187,
  600,  524,  740,  740,  404,  354,  594, 1809,  524,  141,
  919, 1227,  187,  441, 1727,  499, 1726,  360,  663,   51,
  187,  663,  400,  160,  989,  251,  524, 1686,  251, 1656,
  292,   43,  360,  308,  594,  600,  663,  594, 1747,  989,
   52,  187,  355,  251,  356,  706,  704,  187,  705,  187,
  707,   53, 1637,  594,  152,  309,  524, 1727,   54,  161,
  524,  327,   79,  600,  499, 1815,  600,  357, 1728,  510,
  187,   41, 1747,  994,  190, 1687,   24,  745,   51,  594,
  661,  610,  600,   55,  704,  318,  153,  704,  994,  187,
 1802,   99,  358,  364,  321, 1684,  297, 1663,  187, 1727,
  364,  154, 1727,  364,  187,  751,  340,  364,  600,  947,
  664,   47,  697,  595,  150,  364,  610, 1728,  161,  193,
  661,  661,  187, 1737,  661, 1727,  351, 1803,  101,  702,
   41,  364,  151, 1701,  703,   24,  955, 1702,  150,  661,
 1804,   49,  217,  704,  610,  886,  353,  610,   56, 1137,
  362,  664,  704,  697,  664,  364, 1718,  477,  941,  187,
 1728,  708,  489,  610,  751, 1138,  914,  372, 1728,  664,
   47,  697,  595,  930,  512,  140,  141,  702,  937,  427,
   57,  127,  371,  217,   58,  364,  217,  196,  944,  610,
  948,  949,  950,  951,  952,  427,  944,  102,   25,  702,
   49,  217, 1728,   48,   50, 1728, 1224,  103,  512,  512,
 1139,  976,  364,  374,  975, 1774,  477,  151,  696,   45,
 1225,  128,  116,  150,  364,  489,  364,  376, 1728,  734,
  706,  704,  104,  705,  512,  707, 1755,  997,  188, 1140,
  127,  371,  150,  377, 1226,  512,  702,  489,  512,  380,
  359,   46,  520,  359, 1385,  702, 1387,   25,  187,  696,
  190, 1776,   48,   50,  381,  695, 1091,  217, 1644, 1090,
  151, 1098,   79, 1777,  867,  462,  150,  696,   45, 1755,
  128,  116,  470,  702,  510,  150,  706,  704,  703,  705,
 1831,  707,  479, 1190,  471,  663, 1189,  188, 1227,  702,
  105,  360,  687,  212,  360,  663, 1141,  474,  663, 1235,
   46,  879, 1090,  702,  879,  151,  695,  663,  510,  510,
  695,  150,  475, 1176,  480, 1755,  217,  997,  477,  106,
  107,  150,  108,  342,  193, 1240,  343,  496, 1239,  109,
  110,  111,  112,  113,  510,  114,  687,  481,  115,  116,
  117,  118,  119,   80,   81,  510,  120,  121,  510,  717,
  193,  687,  359,  879,  663,  706,  704, 1109,  705,  498,
  707,  499,  880,  663,  359,  880,  359, 1118, 1119, 1123,
  508,  344, 1131, 1133, 1135,  193, 1143, 1146, 1149, 1151,
 1154, 1157,  701,   14, 1161, 1162,  717, 1164,  661, 1166,
  518,  661, 1169, 1170, 1171, 1172, 1173, 1174,  661,  717,
  524,  661,  525,  360,  536,  661,  539,  663, 1224, 1178,
  661,  427,  547,  661,  880,  360,  560,  360,  664,  663,
  697,  663, 1225,  701, 1284,  204,  205, 1283,  664,  661,
  570,  664,  987,  697, 1061, 1062,  400,  401,  402,  567,
  664,  701,  697,  695,  584,  595, 1226,  706,  704,  580,
  705, 1289,  707,  661, 1288,  489, 1399,  661,  702, 1398,
  123,  123,  635,  703,  410,  411,  661,  641, 1489,  293,
 1844, 1189,  964,  987,  987,  987,  987,  987,  987,  987,
  595, 1519, 1526,  661,  708,  708,  188,  664,  639,  697,
  643,  987,  987,  987,  987,  908,  664,  912,  697,  188,
 1227,  916,  918,  920,  294,  677, 1798,  188,  595, 1797,
  661,  595, 1857,  674,  702, 1772,  675,  676,  258,  703,
  678,  679,  661,  987,  661,  987,  696,  595,  188,  680,
  661,  681,  295,  682,  188,  296,  188,  683,  684,  696,
  664,  685,  697,  686,  691, 1339,  692,  701,  696, 1693,
  709,  297,  664,  595,  664,  708,  697,  188,  710,  661,
  661,  719,  661,  728,  714,  731,  736,  738,  739,  661,
  661,  661,  661,  661,  751,  661,  188,  298,  661,  661,
  661,  661,  661,  755,  756,  188,  661,  661,  956,  760,
  762,  188,  763,  702,  665,  696,  230,  765,  703,  940,
  706,  704,  944,  705,  696,  707,  766,  771,  665,  188,
  768,  527,  774,  687, 1342, 1343,  665, 1345, 1347,  779,
  687,  780,  782,  687,  783, 1352,  792,  687,  816,  818,
  823,  817,  665,  736,  828,  687,  829,  230,  277,  833,
  230,  836,  706,  704, 1365,  705,  188,  707,  696,  834,
  835,  687, 1372,  839,  427,  230,  838,  427,  844, 1178,
  850,  851,  696, 1380,  427, 1382, 1383,  706,  704,  854,
  705,  855,  707,  864,  867,  687,  986,  706,  704,  866,
  705,  876,  707,  875, 1765,  702,  665,  489,  877,  881,
  703,  932,  942,  971,  987,  973,  987,  987,  427,  978,
  701,  970,  994, 1001,  987,  687, 1010, 1168,  706,  704,
  687,  705,  987,  707,  982,  983, 1496,  987,  706,  704,
  987,  705,  701,  707, 1013, 1030,  845, 1359,  706,  704,
 1035,  705,  687,  707,  987, 1762, 1040, 1434, 1041,  987,
  987,  795, 1047,  987,  687,  188,  687, 1048, 1050, 1056,
  987, 1043,  987,  987,  658, 1057,  987, 1058, 1064, 1079,
  987, 1068,  795,  987, 1074,  987, 1081,  987,  361,  701,
 1088, 1093,  987,  987, 1105,  987, 1106,  724,  701, 1125,
 1163, 1165,  361, 1175, 1832, 1167,  489, 1187,  968, 1201,
  361,  987,  987,  702,  987, 1360,  706,  704, 1204,  705,
 1207,  707,  424, 1199, 1217, 1221,  361, 1484,  987, 1242,
  987,  987,  987, 1223, 1229,  987, 1492,  711,  987,  987,
 1245, 1256,  701, 1497, 1265, 1267, 1277, 1287,  987,  968,
  968,  968,  968,  968,  980,  968,  701, 1294,  702, 1285,
 1298, 1299,  987,  703, 1300,  987, 1301,  968,  968,  968,
  968, 1310,  427, 1319, 1099, 1324,  427, 1322, 1312,  987,
  361,  987, 1325, 1323, 1331,  233, 1332,  987, 1337, 1338,
 1336, 1340,  987, 1341,  242,  489,  230, 1348, 1349,  968,
  702,  968, 1350,  987,  987,  703,  987, 1351, 1529, 1353,
 1354, 1355,  987, 1356, 1357, 1358, 1693, 1378,  905, 1384,
 1410, 1375,  234, 1400, 1403,  702, 1411, 1363,  706,  704,
  703,  705, 1407,  707, 1408,  702, 1412, 1409, 1419, 1451,
  703,  987,  987, 1422,  987, 1424,   14, 1431, 1429,  562,
 1456,  987,  987,  987,  987,  987, 1466,  987,  105, 1439,
  987,  987,  987,  987,  987, 1443,  702, 1444,  987,  987,
 1132,  703, 1136, 1473, 1144, 1147,  702, 1152, 1155, 1158,
 1445,  703, 1446, 1447, 1448, 1764,  702,  106,  107,  105,
  108,  703, 1625, 1626,  242, 1477, 1515,  109,  110,  111,
  112,  113, 1478,  114, 1482, 1333,  115,  116,  117,  118,
  119, 1488, 1509,  427,  120,  121,  242, 1521,  106,  107,
 1516,  108, 1522,  242, 1579, 1533,  105, 1589,  109,  110,
  111,  112,  113, 1591,  114, 1592, 1594,  115,  116,  117,
  118,  119, 1593, 1595, 1596,  120,  121, 1665, 1598, 1599,
  658, 1600, 1608, 1613,  702,  106,  107,  242,  108,  703,
 1610, 1618, 1617, 1619, 1621,  109,  110,  111,  112,  113,
  968,  114,  968,  968,  115,  116,  117,  118,  119, 1622,
  968, 1623,  120,  121,  706,  704, 1186,  705,  968,  707,
 1624, 1627, 1628,  968,  706,  704,  968,  705, 1629,  707,
  921,  922,  923,  924,  925,  926,  927,  928, 1630, 1700,
  968, 1631,  968, 1632, 1633,  968,  968, 1703,  991,  968,
  706,  704, 1634,  705, 1635,  707,  968, 1636,  968,  968,
   77, 1645,  968, 1661, 1664,  132,  968, 1667, 1666,  968,
 1674,  968, 1529,  968, 1672, 1680, 1682, 1668,  968,  968,
 1669,  968, 1670, 1717, 1671, 1673, 1683, 1719, 1679,  991,
  991,  991,  991,  991,  991,  991,  702,  968,  968, 1704,
  968,  703, 1707, 1712, 1675, 1711, 1721,  991,  991,  991,
  991, 1738, 1746, 1761,  968, 1769,  968,  968,  968, 1773,
  661,  968, 1784, 1786,  968,  968, 1364,  706,  704, 1789,
  705, 1793,  707, 1791,  968, 1800, 1801, 1806, 1812,  991,
 1813,  991, 1781, 1782,  160,  427, 1830, 1766,  968,  661,
  661,  968,  661,  424,  105, 1834, 1837, 1838, 1772,  661,
  661,  661,  661,  661, 1852,  661, 1855,  968,  661,  661,
  661,  661,  661,  968, 1841, 1856,  661,  661,  968,  625,
    1,   29,  203,  106,  107,   31,  108,  755,  368,  968,
  968,  271,  968,  109,  110,  111,  112,  113,  968,  114,
  522,  148,  115,  116,  117,  118,  119,  654,  269,  951,
  120,  121, 1366,  706,  704,  454,  705,  684,  707,   81,
 1367,  706,  704,  213,  705,  522,  707,  968,  968,  105,
  968,  193,  276,  396,  275,  695,  289,  968,  968,  968,
  968,  968,  950,  968, 1330,  693,  968,  968,  968,  968,
  968,  522,  702,  194,  968,  968,  689,  703,  106,  107,
  691,  108,  702,  369,  712,  287,  105,  703,  109,  110,
  111,  112,  113,  711,  114,  223,  323,  115,  116,  117,
  118,  119,  288,  713,  331,  120,  121,  340,  702,  619,
  329,  722,   27,  703,   30,  106,  107, 1389,  108,   39,
  137,  478,  469,  468, 1320,  109,  110,  111,  112,  113,
  991,  114,  991,  991,  115,  116,  117,  118,  119,  972,
  991,  865,  120,  121,  706,  704, 1318,  705,  991,  707,
  247, 1104, 1316,  991, 1306, 1601,  991, 1602, 1368,  706,
  704,  830,  705,  522,  707,  594, 1295,  564, 1244,  549,
  991, 1011,  996,  532,  516,  991,  991, 1714, 1307,  991,
 1045, 1681,  565, 1780, 1581,  702,  991, 1785,  991,  991,
  703, 1587,  991, 1787, 1433, 1823,  991, 1038,  811,  991,
 1763,  991, 1720,  991,  522, 1850, 1735,  522,  991,  991,
 1817,  991, 1236,  996,  996,  996,  996,  996,  996,  996,
 1415, 1536,  522, 1745,  522,  555,  572,  991,  991,  815,
  991,  996,  996,  996,  996,  546, 1463, 1071, 1080,  139,
  852,  509,  746,  750,  991,  996,  991,  991,  991, 1015,
 1002,  991,  290, 1710,  991,  991, 1220, 1829, 1649, 1861,
 1799,  979, 1655,  996,  991,  996,  759, 1344, 1213,    0,
 1428,  702, 1377,    0,    0,    0,  703,    0,  991,  702,
    0,  991,    0,    0,  703, 1369,  706,  704,    0,  705,
    0,  707,    0,   76,    0,  105,    0,  991,    0,    0,
 1370,  706,  704,  991,  705,    0,  707,    0,  991,    0,
 1371,  706,  704,    0,  705,    0,  707,    0,    0,  991,
  991,    0,  991,    0,  106,  107,    0,  108,  991,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
  519,  120,  121,    0,    0,    0,    0,  991,  991,    0,
  991,    0,    0,    0, 1495,    0,    0,  991,  991,  991,
  991,  991,    0,  991,    0,  105,  991,  991,  991,  991,
  991,    0,  702,    0,  991,  991,    0,  703,    0,    0,
    0,  519,    0,    0,  519,    0,    0,  702,    0,    0,
    0,    0,  703,    0,  106,  107,    0,  108,    0,  519,
    0,  519,    0,    0,  109,  110,  111,  112,  113,    0,
  114,  522,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,  996,    0,  996,  996,    0,    0,
  522,  522,    0,  522,  996,    0,    0,  522,    0,    0,
  522,    0,  996,    0,    0,    0,    0,  996,    0,    0,
  996,    0,    0,  522,    0,    0,    0,  524,    0,  522,
    0,    0,    0,    0,  996,    0,  992,    0,    0,  996,
  996,    0, 1247,  996,    0, 1510,  706,  704,    0,  705,
  996,  707,  996,  996,    0,    0,  996,    0,    0,    0,
  996,    0,    0,  996,    0,  996,    0,  996,  524,    0,
    0,  524,  996,  996,    0,  996,    0,  992,  992,  992,
  992,  992,  992,  992,  702,    0,  524,    0,  524,  703,
    0,  996,  996,    0,  996,  992,  992,  992,  992,  702,
    0,  522,    0,    0,  703,    0,    0,    0,  996,  702,
  996,  996,  996,    0,  703,  996,    0,    0,  996,  996,
    0,    0,    0,  522,    0,    0,    0,  992,  996,  992,
  522,    0,    0,    0,    0,    0,    0,    0,    0,  522,
    0,  522,  996,    0,    0,  996,    0, 1511,  706,  704,
    0,  705,    0,  707,    0,    0,    0,    0,    0,  522,
    0,  996,    0,    0,  522,    0,    0,  996,  519, 1518,
  706,  704,  996,  705,    0,  707,    0,    0,    0,    0,
    0,    0,    0,  996,  996,    0,  996,  519,  519,    0,
  519,    0,  996,  522,  519,    0,    0,  519,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  519,    0,    0,    0,    0,    0,  519,    0,    0,    0,
    0,  996,  996,  105,  996,    0,    0,    0,    0,    0,
    0,  996,  996,  996,  996,  996,    0,  996,    0,    0,
  996,  996,  996,  996,  996,    0,    0,    0,  996,  996,
    0,    0,  106,  107,    0,  108,    0,    0,    0,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,  702,  524,    0,    0,  519,  703,
    0,    0,  388,    0,    0,    0,    0,    0,  992,    0,
  992,  992,    0,    0,    0,  524,    0,  524,  992,    0,
  519,  524,    0,    0,  524,    0,  992,  519,    0,    0,
  390,  992,    0,    0,  992,    0,    0,  524,  519,    0,
    0,    0,    0,  524,  670,    0,    0,    0,  992,    0,
  849,    0,  540,  992,  992,    0,  519,  992,    0,  392,
    0,  519,    0,    0,  992,    0,  992,  992,    0,    0,
  992,    0,    0,  586,  992,    0,    0,  992,    0,  992,
    0,  992,    0,    0,  670,  670,  992,  992,  670,  992,
  519,  849,  849,  849,  849,  849,  702,  849,    0,    0,
    0,  703,  541,  670,    0,  992,  992,    0,  992,  849,
  849,  849,  849,    0,    0,  524,    0,    0,  702,    0,
    0,  527,  992,  703,  992,  992,  992,    0,    0,  992,
  528,    0,  992,  992,  397,    0,  542,  524,    0,    0,
    0,    0,  992,  849,  524,    0,    0,    0,    0,    0,
  399,    0,   62,    0,  543,  524,  992,    0,    0,  992,
    0,    0,    0,   63,  400,  401,  402,    0,  204,  205,
   64,    0,    0,  524,    0,  992,    0,    0,  524,    0,
    0,  992,    0,   65,    0,    0,  992,  405,  406,  407,
  408,  409,  410,  411,  412,  413,    0,  992,  992,    0,
  992,    0,  415,  416,    0,    0,  992,  524,    0,    0,
    0,  420,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  501,  524,    0,
    0,    0,    0,    0,    0,  992,  992,    0,  992,    0,
    0,    0,    0,    0,   66,  992,  992,  992,  992,  992,
    0,  992,    0,    0,  992,  992,  992,  992,  992,    0,
   67,    0,  992,  992,    0,    0,    0,    0,  501,  524,
    0,  501,  524,    0,    0,    0,    0,   68,    0,    0,
    0,    0,    0,    0,    0,    0,  501,  524,    0,    0,
  587,    0,   69,    0,    0,    0,   70,    0,    0,   71,
    0,  491,    0,    0,    0,  223,    0,   72,    0,   84,
    0,    0,  849,    0,  849,  849,   85,    0,    0,    0,
   86,   87,  849,    0,    0,    0,   73,    0,    0,   88,
  849,    0,    0,    0,    0,  849,    0,    0,  849,    0,
    0,   89,  491,    0,    0,  491,  223,    0,    0,  223,
    0,    0,  849,    0,  861,    0,    0,  849,  849,    0,
  491,  849,  670,    0,  223,  670,    0,    0,  849,    0,
  849,  849,  670,    0,  849,  670,    0,    0,  849,  670,
  524,  849,    0,  849,  670,  849,    0,  670,    0,    0,
  849,  849,    0,  849,    0,  861,  861,  861,  861,  861,
    0,  861,    0,  670,    0,    0,   90,    0,    0,  849,
  849,    0,  849,  861,  861,  861,  861,    0,    0,    0,
    0,  524,    0,   91,  524,    0,  849,  670,  849,  849,
  849,  670,    0,  849,    0,    0,  849,  849,   92,  524,
  670,    0,   93,    0,    0,    0,  849,  861,    0,    0,
    0,    0,    0,   94,    0,    0,    0,  670,    0,    0,
  849,    0,    0,  849,    0,    0,    0,    0,    0,    0,
    0,    0,   95,    0,    0,    0,    0,    0,    0,  849,
    0,    0,  251,    0,  670,  849,    0,    0,    0,    0,
  849,    0,    0,    0,    0,    0,  670,    0,  670,    0,
    0,  849,  849,    0,  849,  501,  524,  501,  524,    0,
  849,  501,  524,    0,  501,  524,    0,    0,    0,    0,
    0,    0,    0,  251,    0,    0,  251,  501,  524,    0,
    0,  231,    0,    0,    0,    0,    0,    0,    0,  849,
  849,  251,  849,    0,    0,    0,    0,    0,    0,  849,
  849,  849,  849,  849,    0,  849,    0,    0,  849,  849,
  849,  849,  849,    0,    0,    0,  849,  849,    0,    0,
  864,    0,  231,    0,    0,  231,    0,    0,    0,  491,
    0,  491,    0,  223,    0,  223,    0,    0,  491,    0,
  231,    0,  223,    0,    0,    0,    0,    0,    0,    0,
    0,  491,    0,    0,    0,  501,  524,    0,    0,    0,
    0,  864,  864,  864,  864,  864,  861,  864,  861,  861,
    0,    0,    0,    0,    0,    0,  861,  501,  524,  864,
  864,  864,  864,    0,  501,  524,    0,    0,    0,  861,
    0,    0,  861,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  861,    0,  524,    0,
  524,  861,  861,  864,    0,  861,    0,  524,  501,  524,
    0,    0,  861,    0,  861,  861,    0,    0,  861,  491,
  524,    0,  861,  223,    0,  861,    0,  861,    0,  861,
    0,    0,    0,    0,  861,  861,    0,  861,    0,    0,
    0,  491,    0,    0,    0,  223,    0,  534,  491,    0,
    0,    0,  223,  861,  861,  672,  861,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  861,    0,  861,  861,  861,    0,    0,  861,    0,    0,
  861,  861,  491,    0,    0,    0,  223,  534,  534,    0,
  861,  534,    0,    0,    0,    0,  672,    0,  524,  672,
  251,    0,  251,    0,  861,    0,  534,  861,  534,  251,
    0,    0,    0,    0,  672,    0,    0,    0,    0,    0,
  524,    0,    0,  861,    0,    0,    0,  524,    0,  861,
    0,    0,    0,    0,  861,    0,    0,    0,  534,    0,
    0,    0,    0,    0,    0,  861,  861,    0,  861,  242,
    0,  231,    0,    0,  861,    0,    0,    0,    0,    0,
    0,  524,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  864,    0,  864,  864,    0,    0,    0,    0,
    0,    0,  864,  861,  861,    0,  861,    0,    0,    0,
    0,    0,    0,  861,  861,  861,  861,  861,  864,  861,
  251,    0,  861,  861,  861,  861,  861,    0,    0,    0,
  861,  861,  864,    0,  865,    0,    0,  864,  864,    0,
    0,  864,  251,    0,    0,    0,    0,    0,  864,  251,
  864,  864,    0,    0,  864,    0,    0,    0,  864,    0,
    0,  864,    0,  864,    0,  864,    0,    0,    0,  242,
  864,  864,    0,  864,    0,  865,  865,  865,  865,  865,
    0,  865,    0,  251,    0,    0,    0,    0,    0,  864,
  864,  242,  864,  865,  865,  865,  865,    0,  242,    0,
    0,    0,    0,    0,    0,    0,  864,    0,  864,  864,
  864,    0,    0,  864,    0,    0,  864,  864,    0,    0,
    0,    0,    0,    0,    0,  534,  864,  865,    0,    0,
    0,    0,  242,    0,    0,    0,    0,    0,    0,    0,
  864,  534,    0,  864,  534,  534,    0,  534,    0,    0,
    0,  534,    0,    0,  534,    0,    0,    0,    0,  864,
    0,    0,    0,    0,    0,  864,    0,  534,    0,    0,
  864,    0,    0,  534,    0,    0,  534,    0,    0,    0,
    0,  864,  864,    0,  864,    0,    0,    0,    0,    0,
  864,    0,    0,  672,    0,    0,  672,    0,    0,    0,
    0,    0,    0,  672,    0,    0,  672,    0,    0,    0,
  672,    0,    0,    0,    0,  672,    0,    0,  672,  864,
  864,    0,  864,    0,    0,    0,    0,    0,    0,  864,
  864,  864,  864,  864,  672,  864,    0,    0,  864,  864,
  864,  864,  864,    0,    0,  534,  864,  864,    0,    0,
  859,    0,    0,    0,    0,    0,    0,    0,  672,    0,
    0,    0,  672,    0,    0,    0,    0,  534,    0,    0,
    0,  672,    0,    0,  534,    0,    0,  562,  534,    0,
    0,    0,    0,    0,    0,  534,    0,    0,  672,    0,
    0,  859,    0,  859,  859,  859,  865,    0,  865,  865,
    0,    0,    0,  534,    0,    0,  865,    0,  534,  859,
  859,  859,  859,    0,    0,  672,    0,    0,    0,    0,
    0,    0,  865,    0,    0,    0,    0,  672,    0,  672,
    0,    0,    0,  534,    0,    0,  865,  534,    0,    0,
    0,  865,  865,  859,    0,  865,    0,    0,    0,    0,
    0,    0,  865,    0,  865,  865,    0,    0,  865,    0,
    0,    0,  865,    0,    0,  865,    0,  865,    0,  865,
    0,    0,    0,    0,  865,  865,    0,  865,    0,    0,
    0,    0, 1485, 1486,    0,    0,    0,  535, 1490, 1491,
    0, 1493, 1494,  865,  865,  675,  865,    0,    0, 1498,
 1499, 1500, 1501,    0, 1502, 1503, 1504, 1505, 1506, 1507,
  865,    0,  865,  865,  865,    0,    0,  865,    0,    0,
  865,  865,    0,    0,    0,    0,    0,  535,  535,    0,
  865,  535,    0,    0,    0,    0,  675,    0,    0,  675,
    0,    0,    0,    0,  865,    0,  535,  865,  535,    0,
  105,    0,    0,    0,  675,    0,    0,    0,    0,    0,
    0,    0,    0,  865,    0,    0,    0,    0,    0,  865,
    0,    0,    0,    0,  865,    0,    0,    0,  535,  106,
  107,    0,  108,    0,    0,  865,  865,    0,  865,  109,
  110,  111,  112,  113,  865,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,  859,    0,  859,  859,    0,    0,    0,    0,
    0,    0,  859,  865,  865,    0,  865,    0,    0,    0,
    0,    0,    0,  865,  865,  865,  865,  865,  859,  865,
    0,    0,  865,  865,  865,  865,  865,    0,    0,    0,
  865,  865,  859,    0,  858,    0,    0,  859,  859,    0,
    0,  859,    0,    0,    0,    0,    0,    0,  859,    0,
  859,  859,    0,    0,  859,    0,    0,    0,  859,    0,
    0,  859,    0,  859,    0,  859,    0,    0,    0,    0,
  859,  859,    0,  859,    0,  858,    0,  858,  858,  858,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  859,
  859,    0,  859,  858,  858,  858,  858,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  859,    0,  859,  859,
  859,    0,    0,  859,    0,    0,  859,  859,    0,    0,
    0,    0,    0,    0,    0,  535,  859,  858,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  859,  535,    0,  859,  535,  535,    0,  535,    0,    0,
    0,  535,    0,    0,  535,    0,    0,    0,    0,  859,
    0,    0,    0,    0,    0,  859,    0,  535,    0,    0,
  859,    0,    0,  535,    0,    0,  535,    0,    0,    0,
    0,  859,  859,    0,  859,    0,    0,    0,    0,    0,
  859,    0,    0,  675,    0,    0,  675,    0,    0,    0,
    0,    0,    0,  675,    0,    0,  675,    0,    0,    0,
  675,    0,    0,    0,    0,  675,    0,    0,  675,  859,
  859,    0,  859,    0,    0,    0,    0,    0,    0,  859,
  859,  859,  859,  859,  675,  859,    0,    0,  859,  859,
  859,  859,  859,    0,    0,  535,  859,  859,    0,    0,
  860,    0,    0,    0,    0,    0,    0,    0,  675,    0,
    0,    0,  675,    0,    0,    0,    0,  535,    0,    0,
    0,  675,    0,    0,  535,    0,    0,  568,  535,    0,
    0,    0,    0,    0,    0,  535,    0,    0,  675,    0,
    0,  860,    0,  860,  860,  860,  858,    0,  858,  858,
    0,    0,    0,  535,    0,    0,  858,    0,  535,  860,
  860,  860,  860,    0,    0,  675,    0,    0,    0,    0,
    0,    0,  858,    0,    0,    0,    0,  675,    0,  675,
    0,    0,    0,  535,    0,    0,  858,  535,    0,    0,
    0,  858,  858,  860,    0,  858,    0,    0,    0,    0,
    0,    0,  858,    0,  858,  858,    0,    0,  858,    0,
    0,    0,  858,    0,    0,  858,    0,  858,    0,  858,
    0,    0,    0,    0,  858,  858,    0,  858,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  828,    0,    0,
    0,    0,    0,  858,  858,    0,  858,    0,  773,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  858,    0,  858,  858,  858,    0,    0,  858,    0,    0,
  858,  858,    0,    0,    0,    0,    0,    0,  828,    0,
  858,  828,    0,    0,    0,    0,    0,    0,    0,  773,
    0,    0,  773,    0,  858,    0,  828,  858,    0,    0,
  105,    0,    0,    0,    0,    0,    0,  773,    0,    0,
    0,    0,    0,  858,    0,    0,    0,    0,    0,  858,
    0,    0,    0,    0,  858,    0,    0,    0,    0,  106,
  107,    0,  108,    0,    0,  858,  858,    0,  858,  109,
  110,  111,  112,  113,  858,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,  860,    0,  860,  860,    0,    0,    0,    0,
    0,    0,  860,  858,  858,    0,  858,    0,    0,    0,
    0,    0,    0,  858,  858,  858,  858,  858,  860,  858,
    0,    0,  858,  858,  858,  858,  858,    0,    0,    0,
  858,  858,  860,    0,  863,    0,    0,  860,  860,    0,
    0,  860,    0,    0,    0,    0,    0,    0,  860,    0,
  860,  860,    0,    0,  860,    0,    0,    0,  860,    0,
    0,  860,    0,  860,    0,  860,    0,    0,    0,    0,
  860,  860,    0,  860,    0,  863,    0,  863,  863,  863,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  860,
  860,    0,  860,  863,  863,  863,  863,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  860,    0,  860,  860,
  860,    0,    0,  860,  678,    0,  860,  860,    0,    0,
    0,    0,    0,    0,    0,    0,  860,  863,    0,  828,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  860,    0,    0,  860,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  678,    0,    0,  678,  860,
    0,    0,    0,    0,    0,  860,    0,    0,    0,    0,
  860,    0,    0,  678,    0,    0,    0,    0,    0,    0,
    0,  860,  860,    0,  860,  828,    0,    0,  828,    0,
  860,    0,    0,    0,    0,  828,  773,    0,  828,  773,
    0,  459,  828,    0,    0,    0,  773,  828,    0,  773,
  828,    0,    0,  773,    0,    0,    0,    0,  773,  860,
  860,  773,  860,    0,    0,    0,  828,  105,    0,  860,
  860,  860,  860,  860,    0,  860,    0,  773,  860,  860,
  860,  860,  860,    0,    0,    0,  860,  860,    0,    0,
  828,    0,    0,  828,  828,    0,  106,  107,    0,  108,
    0,  773,    0,  828,  773,  773,  109,  110,  111,  112,
  113,    0,  114,    0,  773,  115,  116,  117,  118,  119,
  828,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,  773,    0,    0,    0,    0,  863,    0,  863,  863,
    0,    0,  828,    0,    0,    0,  863,  828,    0,    0,
    0,    0,    0,  773,    0,    0,    0,    0,  773,  828,
    0,  828,  863,    0,    0,    0,  658,    0,    0,  421,
  773,  422,  773,    0,    0,    0,  863,    0,    0,    0,
    0,  863,  863,    0,  424,  863,    0,    0,    0,  279,
    0,    0,  863,    0,  863,  863,    0,    0,  863,    0,
    0,    0,  863,    0,    0,  863,    0,  863,    0,  863,
    0,    0,    0,    0,  863,  863,    0,  863,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  863,  863,    0,  863,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  863,    0,  863,  863,  863,    0,    0,  863,    0,    0,
  863,  863,    0,  658,    0,    0,  421,    0,  422,    0,
  863,    0,  678,    0,    0,  678,    0,    0,    0,    0,
    0,  424,  678,    0,  863,  678,  279,  863,    0,  678,
    0,    0,    0,    0,  678,    0,    0,  678,    0,    0,
    0,    0,    0,  863,    0,    0,    0,    0,    0,  863,
    0,    0,    0,  678,  863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  863,  863,    0,  863,    0,
    0,    0,    0,    0,  863,    0,    0,  678,    0,    0,
    0,  678,    0,    0,    0,    0,    0,    0,    0,    0,
  678,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  863,  863,    0,  863,  678,    0,    0,
    0,    0,    0,  863,  863,  863,  863,  863,    0,  863,
    0,    0,  863,  863,  863,  863,  863,    0,    0,    0,
  863,  863,    0,  385,  678,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,    0,  678,    0,  678,    0,
    0,    0,    0,    0,  661,  387,    0,    0,    0,    0,
    0,  388,  389,    0,    0,    0,    0,    0,  797,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  652,    0,    0,  661,  661,    0,  661,    0,    0,  390,
    0,    0,  391,  661,  661,  661,  661,  661,    0,  661,
    0,    0,  661,  661,  661,  661,  661,    0,    0,  797,
  661,  661,  797,    0,    0,    0,    0,    0,    0,    0,
  393,  394,    0,    0,    0,    0,    0,  797,    0,    0,
  385,    0,  395,    0,    0,    0,  396,    0,    0,  386,
    0,    0,    0,    0,  882,    0,    0,    0,    0,    0,
    0,    0,  387,    0,    0,  651,    0,    0,  388,  389,
    0,  645,    0,  645,  645,    0,  645,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  652,    0,  645,
    0,    0,    0,  695,  645,    0,  390,  654,    0,  391,
    0,    0,    0,  397,    0,    0,    0,    0,    0,  398,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  399,
    0,    0,    0,    0,    0,    0,    0,  393,  394,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,  395,
    0,    0,    0,  396,    0,    0,    0,  404,    0,    0,
    0,  653,  463,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,  105,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,    0,    0,  115,  116,  117,  118,  119,  883,
  884,  885,  120,  121,  654,    0,    0,    0,  106,  107,
  397,  108,    0,    0,    0,    0,  398,    0,  109,  110,
  111,  112,  113,    0,  114,    0,  399,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
  797,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
    0,  115,  116,  655,  656,  657,    0,    0,    0,  120,
  121,  645,    0,    0,    0,    0,  797,    0,  645,  797,
    0,    0,    0,    0,    0,    0,  797,  645,    0,  797,
    0,    0,    0,  797,    0,    0,    0,    0,  797,    0,
  645,  797,    0,    0,    0,    0,  645,  645,    0,    0,
  639,    0,  639,  639,  645,  639,    0,  797,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  639,    0,
    0,    0,    0,  639,  645,    0,    0,  645,    0,    0,
    0,  797,    0,    0,  797,  797,    0,    0,    0,    0,
  798,    0,    0,    0,  797,    0,    0,    0,    0,    0,
    0,    0,    0,  645,    0,  645,  645,    0,    0,    0,
    0,  797,    0,    0,    0,    0,    0,  645,    0,    0,
    0,  645,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  798,    0,  797,  798,    0,  658,    1,  797,  421,
    0,  422,    0,    0,    0,    0,    0,    0,    0,  798,
  797,    0,  797,    0,  424,    0,    0,    0,    2,  279,
    0,    0,    0,    0,    0,    0,    3,    0,    0,    0,
    0,    0,    0,    4,    0,    5,    0,    6,  645,    0,
    0, 1646,    7,    0,  645,    0,    0,    0,    0,    8,
    0,    0,    0,    0,  645,    0,    0,    0,    0,    0,
    0,    0,    0,    9,    0,    0, 1647,    0,  645,  645,
  645,  645,  645,  645,   10,    0,    0,    0,    0,    0,
    0,    0,  645,    0,    0,    0,    0,    0,    0,    0,
    0,  645,  645,  645,  645,  645,  645,  645,  645,  645,
  645,  645,    0,  645,    0,    0,  645,  645,    0,  645,
  645,  645,  645,  645,  645,  645,  645,    0,    0,  645,
  645,  645,  645,  645, 1648,    0,    0,  645,  645,    0,
  639,    0,    0,    0,    0,    0,    0,  639,    0,    0,
    0,   11,    0,    0,    0,    0,  639,    0,   12,    0,
    0,   13,    0,   14,   15,    0,    0,    0,  105,  639,
    0,    0,    0,    0,    0,  639,  639,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,    0,   16,    0,
    0,    0,    0,    0,    0,    0,    0,  106,  107,    0,
  108,    0,    0,  639,    0,    0,  639,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,    0,  798,    0,  120,  121,    0,    0,    0,    0,
    0,    0,  639,  385,  639,  639,  658,   17,    0,  421,
    0,  422,  386,    0,    0,    0,  639,    0,    0,    0,
  639,    0,   18,   19,  424,  387,    0,    0,    0,  279,
    0,  388,  389,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  798,    0,
  652,  798,    0,    0,    0,    0,    0,    0,  798,  390,
    0,  798,  391,    0,    0,  798,    0,    0,    0,    0,
  798,    0,    0,  798,    0,    0,    0,  639,    0,    0,
    0,    0,    0,  639,    0,    0,    0,    0,    0,  798,
  393,  394,    0,  639,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,  396,  639,  639,  639,
  639,  639,  639,  798,  878,    0,  798,  798,    0,    0,
    0,  639,    0,    0,    0,    0,  798,    0,    0,    0,
  639,  639,  639,  639,  639,  639,  639,  639,  639,  639,
  639,    0,  639,  798,    0,  639,  639,    0,  639,  639,
  639,  639,  639,  639,  639,  639,    0,  654,  639,  639,
  639,  639,  639,  397,    0,  798,  639,  639,    0,  398,
  798,    0,    0,    0,    0,    0,    0,    0,    0,  399,
    0,    0,  798,    0,  798,    0,    0,    0,    0,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,  385,  417,  418,  419,  111,  112,  113,
  420,  114,  386,    0,  115,  116,  655,  656,  657,    0,
    0,    0,  120,  121,  658,  387,    0,  421,    0,  422,
    0,  388,  389,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  279,    0,    0,
  652,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  940,  706,  704,    0,  705,
    0,  707,    0,    0,    0,    0,    0,    0,    0,    0,
  393,  394,    0,    0,  903,  902,  904,    0,    0,    0,
    0,    0,  395,    0,    0,    0,  396,    0,    0,  868,
    0,    0,    0,  596,  653,    0,    0,    0,    0,    0,
    0,  483,    0,    0,  421,    0,  422,  869,    0,    0,
    0,  597,    0,    0,  598,    0,    0,    0,    0,  424,
    0,    0,    0,    0,  279,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  654,    0,    0,
    0,    0,    0,  397,    0,    0,    0,    0,    0,  398,
    0,    0,    0,    0,    0,  599,    0,    0,    0,  399,
    0,  600,  601,  602,    0,  603,  604,  605,    0,  606,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,  607,
    0,  608,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,  609,    0,  115,  116,  655,  656,  657,    0,
    0,  385,  120,  121,    0,    0,    0,    0,    0,  610,
  386,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  387,    0,    0,    0,    0,    0,  388,
  389,  611,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  652,    0,
    0,    0,    0,    0,    0,    0,  890,  390,    0,    0,
  391,    0,    0,    0,  702,    0,  612,  613,    0,  703,
    0,    0,  891,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  614,    0,    0,    0,    0,  393,  394,
    0, 1120,  529,    0, 1121,    0,    0,    0,  385,    0,
  395,    0,    0,    0,  396,    0,    0,  386,  892,    0,
    0,    0,  878,    0,    0,    0,    0,  893,    0,    0,
  387,    0,    0,    0,    0,  894,  388,  389,    0,  483,
    0,    0,  421,  529,  422,    0,  529,    0,    0,    0,
    0,    0,    0,    0,  895,    0,  896,  424,    0,    0,
    0,  529,  279,  529,  390,  654,    0,  391,    0,    0,
  897,  397,  898,  899,  900,    0,    0,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
    0,    0,    0,  529,    0,  393,  394,    0,    0,    0,
    0,  400,  401,  402,  403,  204,  205,  395,    0,    0,
    0,  396,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,    0,  901,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0, 1122,    0,  397,    0,
    0,    0,    0,    0,  398,  959,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,  483,    0,    0,  421,    0,  422,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,  424,
    0,  959,  404,    0,  279,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121, 1130,
  529,    0, 1121,    0,    0,    0,  385,    0,    0,    0,
    0,    0,    0,    0,    0,  386,  529,    0,    0,  529,
  529,    0,  529,    0,    0,  960,  529,    0,  387,  529,
  962,    0,    0,    0,  388,  389,    0,    0,    0,    0,
  483,    0,  529,  421,    0,  422,    0,  105,  529,    0,
    0,  529,    0,    0,    0,    0,    0,    0,  424,    0,
    0,  961,  390,  279,    0,  391,  962,    0,    0,    0,
    0,    0,  963,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,  105,    0,    0,  109,  110,  111,  112,
  113,  964,  114,  393,  394,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,  395,    0,    0,    0,  396,
    0,    0,  106,  107,    0,  108,    0,    0,    0,    0,
  529,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,  529,    0,    0,    0,    0,    0,    0,  529,
    0, 1134,    0,  529, 1121,    0,    0,    0,  385,    0,
  529,    0,    0,    0, 1122,    0,  397,  386,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,  529,    0,
  387,    0,  399,  529,    0,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,  529,    0,
  404,    0,  529,    0,  390,    0,    0,  391,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,  393,  394,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,  395,    0,    0,
 1142,  396,    0, 1121,    0,    0,    0,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  387,
    0,    0,    0,    0,    0,  388,  389,    0,    0,  483,
    0,    0,  421,    0,  422,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1122,  424,  397,    0,
    0,    0,  279,  390,  398,    0,  391,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,  393,  394,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,  395,    0,    0,    0,
  396,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0, 1431,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1122,    0,  397,    0,    0,
    0,    0,    0,  398,    0,    6,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0, 1544, 1545,    0, 1546,
    0,  483, 1547,    0,  421,    0,  422,  400,  401,  402,
  403,  204,  205,    0,    0,    0, 1548,    0,    0,  424,
    0,  404, 1549,    0,  279,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0, 1145,
    0,    0, 1121,    0,    0,    0,  385,    0,    0,    0,
    0,    0, 1550,    0,    0,  386,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  387,    0,
    0,   14,    0,    0,  388,  389,    0,    0,    0,  105,
  483,    0,    0,  421,    0,  422, 1551,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,  424,    0,
    0,    0,  390,  279,    0,  391,    0, 1552,  106,  107,
    0,  108,    0,    0,  105,    0,    0,    0,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,  117,
  118,  119,    0,  393,  394,  120,  121,    0,    0,    0,
    0,    0,    0, 1553,  107,  395,  108,    0,    0,  396,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115, 1554,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1150,    0,    0, 1121,    0,    0,    0,  385,    0,
    0,    0,    0,    0, 1122,    0,  397,  386,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,    0,
  387,    0,  399,    0,    0,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,  390,    0,    0,  391,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,  393,  394,  115,  116,  117,
  118,  119,    0,  822,    0,  120,  121,  395,    0,    0,
 1153,  396,    0, 1121,    0,    0,    0,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  387,
    0,    0,    0,    0,  822,  388,  389,  822,    0,  483,
    0,    0,  421,    0,  422,    0,    0,    0,    0,    0,
    0,    0,  822,    0,    0,    0, 1122,  424,  397,    0,
    0,    0,  279,  390,  398,    0,  391,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,  393,  394,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,  395,    0,    0,    0,
  396,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1122,    0,  397,    0,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,  822,  120,  121,    0, 1156,
    0,    0, 1121,    0,    0,    0,  385,    0,    0,  483,
    0,  488,  421,    0,  422,  386,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  424,  387,    0,
    0,    0,  279,    0,  388,  389,    0,  706,  704,    0,
  705,    0,  707,    0,    0,    0,    0,    0,    0,    0,
    0,  822,    0,    0,  822,  903,  902,  904,    0,    0,
    0,  822,  390,    0,  822,  391,    0,    0,  822,    0,
    0,    0,    0,  822,    0,    0,  822,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  822,  393,  394,    0,    0,    0,    0,    0,
    0,    0,  567,    0,    0,  395,    0,  483,    0,  396,
  421,    0,  422,    0,    0,    0,  822,    0,    0,  822,
  822,    0,    0,    0,    0,  424,    0,    0,    0,  822,
  279,  567,  567,    0,  567,    0,    0,    0,    0,    0,
    0,  567,  567,  567,  567,  567,  822,  567,    0,    0,
  567,  567,  567,  567,  567,    0,    0,    0,  567,  567,
    0,    0,    0,    0, 1122,    0,  397,    0,  822,    0,
    0,    0,  398,  822,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  822,    0,  822,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,  385,  417,  418,  419,
  111,  112,  113,  420,  114,  386,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,  387,    0,
    0,    0,    0,    0,  388,  389,    0,  890,  801,    0,
    0,    0,    0,    0,    0,  702,    0,    0,    0,    0,
  703,    0,    0,  891,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  801,
    0,    0,  801,    0,    0,    0,    0,    0,    0,  892,
    0,  392,    0,  393,  394,    0,    0,  801,  893,    0,
    0,    0,    0,    0,  385,  395,  894,    0,    0,  396,
    0,    0,    0,  386,    0,    0,  962,    0,  962,  962,
    0,  962,    0,    0,    0,  895,  387,  896,    0,    0,
    0,    0,  388,  389,  962,    0,    0,    0,    0,  962,
    0,  897,    0,  898,  899,  900,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  390,    0,    0,  391,    0,    0,  397,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,  392,
    0,  393,  394,    0,  901,    0,  400,  401,  402,  403,
  204,  205,    0,  395,    0,    0,    0,  396,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,  695,  120,  121,    0,    0,    0,
    0,    0,    0,    0,  397,    0,    0,    0,  645,    0,
  398,  645,    0,  645,    0,    0,    0,    0,    0,    0,
  399,    0,    0,    0,    0,    0,  645,    0,    0,    0,
    0,  645,    0,    0,  400,  401,  402,  403,  204,  205,
  801,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  962,    0,  115,  116,  117,  118,  119,
    0,    0,  962,  120,  121,    0,  801,    0,    0,  801,
    0,    0,    0,  799,    0,  962,  801,    0,    0,  801,
    0,  962,  962,  801,    0,    0,    0,    0,  801,    0,
    0,  801,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  801,    0,  962,
    0,    0,  962,    0,  799,    0,    0,  799,    0,    0,
    0,    0,    0,    0,    0,  281,    0,    0,    0,    0,
    0,  801,  799,    0,  801,  801,    0,    0,  962,    0,
  962,  962,    0,    0,  801,    0,    0,    0,    0,    0,
    0,    0,  962,  800,  281,  281,  962,  281,    0,    0,
    0,  801,    0,    0,  281,  281,  281,  281,  281,    0,
  281,    0,    0,  281,  281,  281,  281,  281,    0,    0,
    0,  281,  281,  801,    0,    0,    0,    0,  801,    0,
    0,    0,    0,    0,  800,    0,    0,  800,  645,    0,
  801,    0,  801,    0,    0,  645,    0,    0,    0,    0,
    0,    0,  800,  962,  645,    0,    0,  423,    0,  962,
  421,    0,  422,    0,    0,    0,    0,  645,    0,  962,
    0,    0,    0,  645,  645,  424,    0,    0,    0,    0,
  279,  645,    0,  962,  962,  962,  962,  962,  962,    0,
    0,    0,    0,    0,    0,    0,    0,  962,    0,    0,
    0,  645,    0,    0,  645,    0,  962,  962,  962,  962,
  962,  962,  962,  962,  962,  962,  962,    0,  962,    0,
    0,  962,  962,    0,  962,  962,  962,  962,  962,  962,
  962,  962,  645,  645,  962,  962,  962,  962,  962,    0,
    0,    0,  962,  962,  645,    0,    0,    0,  645,    0,
    0,    0,    0,    0,  483,    0,    0,  421,    0,  422,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  424,    0,    0,  799,    0,  279,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  645,    0,    0,    0,    0,
    0,  645,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  645,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  799,    0,    0,  799,  645,  645,  645,  645,  645,
  645,  799,    0,    0,  799,    0,    0,    0,  799,  645,
    0,    0,    0,  799,    0,  800,  799,    0,  645,  645,
  645,  645,  645,  645,  645,  645,  645,  645,  645,    0,
  645,    0,  799,  645,  645,    0,  645,  645,  645,  645,
  645,  645,  645,  645,  385,    0,  645,  645,  645,  645,
  645,    0,    0,  386,  645,  645,  799,    0,    0,  799,
  799,    0,    0,    0,    0,    0,  387,    0,    0,  799,
   98,  800,  388,  389,  800,    0,    0,    0,    0,    0,
    0,  800,    0,    0,  800,    0,  799,    0,  800,    0,
    0,    0,    0,  800,    0,    0,  800,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,    0,  799,    0,
    0,   99,  800,  799,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  100,    0,  799,    0,  799,    0,  392,
    0,  393,  394,    0,    0,    0,  800,  134,    0,  800,
  800,  385,    0,  395,    0,    0,    0,  396,  101,  800,
  386,    0,    0,  483,    0,    0,  421,    0,  422,    0,
    0,    0,    0,  387,    0,    0,  800,    0,    0,  388,
  389,  424,    0,    0,    0,    0,  279,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  800,    0,
    0,    0,    0,  800,    0,    0,    0,  390,    0,    0,
  391,    0,    0,    0,  397,  800,    0,  800,    0,    0,
  398,    0,    0,    0,    0,    0,    0,  102,    0,    0,
  399,    0,    0,    0,    0,    0,    0,  103,  393,  394,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
  395,    0,    0,    0,  396,    0,    0,    0,  404,    0,
    0,    0,  104,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
  105,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,  106,
  107,  397,  108,    0,    0,    0,    0,  398,    0,  109,
  110,  111,  112,  113,    0,  114,    0,  399,  115,  116,
  117,  118,  119,    0,    0,  687,  120,  121,    0,    0,
    0,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
  385,    0,  115,  116,  117,  118,  119,    0,    0,  386,
  120,  121,  483,    0,    0,  421,    0,  422,    0,    0,
    0,    0,  387,    0,    0,    0,    0,    0,  388,  389,
  424,    0,    0,    0,    0,  279,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  393,  394,    0,
    0,    0,  483,  943,    0,  421,    0,  422,    0,  395,
    0,    0,    0,  396,    0,    0,    0,    0,    0,    0,
  424,    0,    0,  105,    0,  279,    0,    0,    0,    0,
    0,    0,  164,  165,  166,  167,  168,  169,  170,  171,
  172,  173,  174,  175,  176,  177,  178,  179,    0,    0,
  180,  181,  106,  107,    0,  108,    0,    0,  182,  183,
  695,    0,  109,  110,  111,  112,  113,  184,  114,  185,
  397,  115,  116,  117,  118,  119,  398,    0,    0,  120,
  121,    0,    0,    0,    0,    0,  399,    0,    0,    0,
  221,    0,    0,    0,  221,    0,    0,    0,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,  221,    0,
    0,    0,  221,    0,  404,  221,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,  385,
    0,  115,  116,  117,  118,  119,  221,    0,  386,  120,
  121,    0,  221,  221,  221,    0,  221,  221,  221,    0,
  221,  387,    0,    0,    0,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  221,    0,  221,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  221,    0,    0,    0,    0,   80,  385,
    0,    0,    0,    0,  392,    0,  393,  394,  386,    0,
  221,  483,    0,    0,  421,    0,  422,    0,  395,    0,
    0,  387,  396,    0,    0,    0,    0,  388,  389,  424,
    0,    0,  221,    0,  279,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0,    0,    0,  221,  221,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  397,
    0,    0,    0,    0,  221,  398,  393,  394,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,  395,    0,
    0,    0,  396,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,   98,    0,    0,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,   99,  397,
  115,  116,  117,  118,  119,  398,    0,    0,  120,  121,
  100,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,  101,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  385,    0,
  115,  116,  117,  118,  119,    0,    0,  386,  120,  121,
  483,    0,    0,  421,    0,  422,    0,    0,    0,    0,
  387,    0,    0,    0,  102,    0,  388,  389,  424,    0,
    0,    0,    0,  279,  103,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  390,    0,    0,  391,    0,  104,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,  393,  394,    0,    0,    0,
  483,    0,    0,  421,    0,  422,    0,  395,    0,    0,
    0,  396,    0,    0,    0,    0,  106,  107,  424,  108,
    0,    0,    0,  279,    0,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,  221,    0,
    0,    0,  221,    0,    0, 1148,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,  221,    0,    0,    0,
  221,    0,  404,  221,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,  385,    0,  115,
  116,  117,  118,  119,  221,    0,  386,  120,  121,    0,
  221,  221,  221,    0,  221,  221,  221,    0,  221,  387,
    0,    0,    0,    0,    0,  388,  389,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  221,    0,
  221,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,    0,    0,    0,    0,    0,  385,    0,    0,
    0,    0,    0,    0,  393,  394,  386,    0,  221,  483,
    0,    0,  421,    0,  422,    0,  395,    0,    0,  387,
  396,    0,    0,    0,    0,  388,  389,  424, 1716,    0,
  221,    0,  279,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,    0,    0,    0,    0,  221,  221, 1333,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  397,    0,    0,
    0,    0,  221,  398,  393,  394,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,  395,    0,    0,    0,
  396,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,    0,  397,  115,  116,
  117,  118,  119,  398,    0,    0,  120,  121,    0,    0,
    0,    0,    0,  399,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1346,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  385,    0,  115,  116,
  117,  118,  119,    0,    0,  386,  120,  121,  483,    0,
    0,  421,    0,  422,    0,    0,    0,    0,  387,    0,
    0,    0,    0,    0,  388,  389,  424,    0,    0,    0,
    0,  279,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,  394,    0,    0,    0,  962,    0,
    0,  962,    0,  962,    0,  395,    0,    0,    0,  396,
    0,    0,    0,  595,    0,    0,  962,  596,    0,    0,
    0,  962,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  597,    0,    0,  598,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  397,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,  599,
    0,    0,  399,    0,    0,  600,  601,  602,    0,  603,
  604,  605,    0,  606,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,  607,    0,  608,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,  385,  609,  115,  116,  117,
  118,  119,    0,    0,  386,  120,  121,    0,    0,    0,
    0,    0,    0,  610,    0,    0,    0,  387,    0,    0,
    0,    0,    0,  388,  389,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  611,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  390,    0,    0,  391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  551,    0,    0,    0,    0,
  612,  613,    0,    0,    0,  962,    0,  541,    0,    0,
    0,    0,  393,  394,  962,    0,    0,  614,    0,  526,
    0,    0,    0,    0,  395,    0,    0,  962,  396,    0,
    0,    0,    0,  962,  962,    0,  551,    0,    0,  551,
    0,    0,    0,    0,    0,    0,  830,    0,  541,    0,
    0,  541,    0,    0,  551,    0,  551,    0,    0,    0,
  526,  962,    0,  526,  962,    0,  541,    0,  541,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  526,    0,
  526,    0,    0,    0,    0,  397,  551,  830,    0,    0,
  830,  398,  962,  962,    0,    0,    0,    0,  541,    0,
    0,  399,    0,    0,  962,  830,    0,    0,  962,    0,
  526,    0,    0,    0,    0,  400,  401,  402,  403,  204,
  205,    0,    0,    0,    0,    0,    0,    0,    0,  404,
    0,    0,    0,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,    0,  962,  115,  116,  117,  118,
  119,  962,    0,    0,  120,  121,    0,    0,    0,    0,
    0,  962,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  962,  962,  962,  962,  962,
  962,    0,    0,    0,    0,    0,    0,    0,    0,  962,
    0,    0,    0,    0,    0,    0,    0,    0,  962,  962,
  962,  962,  962,  962,  962,  962,  962,  962,  962,    0,
  962,    0,    0,  962,  962,    0,  962,  962,  962,  962,
  962,  962,  962,  962,    0,    0,  962,  962,  962,  962,
  962,    0,    0,  551,  962,  962,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  541,    0,    0,    0,  551,
    0,    0,    0,  551,    0,  551,    0,  526,    0,  551,
    0,  541,  551,    0,    0,  541,    0,  541,  838,    0,
    0,  541,    0,  526,  541,  551,    0,  526,    0,  526,
    0,  551,    0,  526,  551,    0,  526,  541,  830,    0,
    0,    0,    0,  541,    0,    0,  541,    0,    0,  526,
    0,    0,    0,    0,    0,  526,    0,    0,  526,  838,
 1000, 1000,  838, 1000, 1000, 1000,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  838, 1000, 1000,
 1000,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  830,    0,    0,  830,    0,    0,
    0,    0,    0,  551,  830,    0,    0,  830,    0, 1000,
    0,  830,    0,    0,    0,  541,  830,    0,    0,  830,
    0,    0,    0,    0,    0,  551,    0,  526,    0,    0,
    0,    0,  551,    0,    0,  830,  551,  541,    0,    0,
    0,    0,    0,  551,  541,    0,    0,    0,  541,  526,
    0,    0,    0,    0,    0,  541,  526,    0,    0,  830,
  526,  551,  830,  830,    0,    0,  551,  526,    0,    0,
    0,    0,  830,  541,    0,    0,    0,    0,  541,  839,
    0,    0,    0,  133,    0,  526,    0,  133,    0,  830,
  526,  551,    0,    0,    0,  551,    0,    0,    0,    0,
    0,    0,    0,  541,    0,  133,    0,  541,  133,    0,
    0,  830,    0,    0,    0,  526,  830,    0,    0,  526,
  839, 1001, 1001,  839, 1001, 1001, 1001,    0,  830,    0,
  830,    0,    0,    0,    0,    0,    0,    0,  839, 1001,
 1001, 1001,    0,    0,    0,    0,    0,    0,    0,  133,
    0,    0,    0,    0,    0,  133,  133,  133,    0,  133,
  133,  133,    0,  133,    0,    0,    0,    0,    0,    0,
 1001,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  838,    0,    0,  133,    0,  133,    0,    0,    0,    0,
 1000,    0,    0,    0,    0,    0,    0,    0, 1000,    0,
    0,    0,    0, 1000,    0,    0, 1000,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  133,    0,    0,    0,
    0,    0,  840,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,  838,    0,    0,  838,
    0,    0, 1000,    0,    0,    0,  838,    0,    0,  838,
    0, 1000,    0,  838,    0,  133,    0,    0,  838, 1000,
    0,  838,    0,  840, 1002, 1002,  840, 1002, 1002, 1002,
    0,    0,    0,    0,    0,    0,    0,  838, 1000,    0,
 1000,  840, 1002, 1002, 1002,  794,    0,    0,    0,    0,
  133,  133,    0,    0, 1000,    0, 1000, 1000, 1000,    0,
    0,  838,    0,    0,  838,  838,    0,  133,    0,    0,
    0,    0,    0, 1002,  838,    0,    0,    0,    0,    0,
    0,  795,    0,    0,    0,    0,  794,    0,    0,  794,
    0,  838,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  839,    0,    0,  794,    0,    0, 1000,    0,    0,
    0, 1001,    0,  838,    0,    0,    0,  796,  838, 1001,
    0,    0,  795,    0, 1001,  795,    0, 1001,    0,    0,
  838,    0,  838,    0,    0,    0,    0,    0,    0,    0,
  795,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  829,    0,    0,    0,  839,  796,    0,
  839,  796,    0, 1001,    0,    0,    0,  839,    0,    0,
  839,    0, 1001,    0,  839,    0,  796,    0,    0,  839,
 1001,    0,  839,    0,    0,    0,    0,    0,    0,  823,
    0,    0,    0,    0,  829,    0,    0,  829,  839, 1001,
    0, 1001,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  829,    0,    0, 1001,    0, 1001, 1001, 1001,
    0,    0,  839,    0,    0,  839,  839,    0,    0,    0,
  823,    0,    0,  823,    0,  839,    0,    0,    0,    0,
    0,    0,    0,    0,  840,    0,    0,    0,  823,    0,
    0,    0,  839,    0, 1002,    0,    0,    0,    0,    0,
    0,  831, 1002,    0,    0,    0,    0, 1002, 1001,    0,
 1002,    0,    0,    0,  839,    0,    0,    0,    0,  839,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  839,    0,  839,    0,    0,    0,    0,    0,    0,
  840,    0,  831,  840,    0,  831, 1002,  794,    0,    0,
  840,    0,    0,  840,    0, 1002,    0,  840,    0,    0,
  831,    0,  840, 1002,    0,  840,    0,    0,    0,    0,
    0,    0,    0,  832,    0,    0,    0,    0,    0,    0,
    0,  840, 1002,  795, 1002,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1002,    0,
 1002, 1002, 1002,  794,    0,  840,  794,    0,  840,  840,
    0,    0,    0,  794,  832,    0,  794,  832,  840,  796,
  794,    0,    0,    0,    0,  794,    0,    0,  794,    0,
    0,    0,  832,    0,    0,  840,    0,    0,    0,  795,
    0,    0,  795,    0,  794,    0,    0,    0,    0,  795,
    0, 1002,  795,    0,    0,  829,  795,  840,    0,    0,
    0,  795,  840,    0,  795,    0,    0,    0,  794,    0,
    0,  794,  794,    0,  840,  796,  840,    0,  796,    0,
  795,  794,    0,    0,    0,  796,  820,    0,  796,    0,
    0,  823,  796,    0,    0,    0,    0,  796,  794,    0,
  796,    0,    0,    0,  795,    0,    0,  795,  795,    0,
    0,  829,    0,    0,  829,    0,  796,  795,  824,    0,
  794,  829,    0,    0,  829,  794,    0,  820,  829,    0,
  820,    0,    0,  829,  795,    0,  829,  794,    0,  794,
  796,    0,    0,  796,  796,  820,    0,  823,    0,    0,
  823,    0,  829,  796,    0,    0,  795,  823,    0,  824,
  823,  795,  824,  831,  823,    0,    0,    0,    0,  823,
  796,    0,  823,  795,    0,  795,  829,  824,    0,  829,
  829,    0,    0,    0,    0,    0,    0,    0,  823,  829,
    0,    0,  796,    0,    0,    0,    0,  796,    0,    0,
    0,    0,  833,    0,    0,    0,  829,    0,    0,  796,
    0,  796,  823,    0,    0,  823,  823,    0,    0,  831,
    0,    0,  831,    0,    0,  823,    0,    0,  829,  831,
    0,  821,  831,  829,    0,  832,  831,    0,    0,    0,
    0,  831,  823,  833,  831,  829,  833,  829,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  831,  833,    0,    0,  823,  825,    0,    0,    0,  823,
    0,    0,  821,    0,    0,  821,    0,    0,    0,    0,
    0,  823,    0,  823,  831,    0,    0,  831,  831,    0,
  821,  832,    0,    0,  832,    0,    0,  831,    0,    0,
    0,  832,    0,    0,  832,    0,  825,    0,  832,  825,
    0,    0,    0,  832,  831,    0,  832,    0,    0,    0,
    0,    0,    0,    0,  825,    0,    0,    0,    0,    0,
    0,    0,  832,    0,    0,    0,  831,    0,    0,    0,
    0,  831,    0,    0,    0,    0,    0,    0,  820,    0,
    0,    0,    0,  831,    0,  831,  832,    0,    0,  832,
  832,    0,    0,    0,    0,    0,    0,    0,    0,  832,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  824,    0,    0,    0,    0,    0,  832,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  820,    0,    0,  820,  832,    0,
    0,    0,    0,  832,  820,    0,    0,  820,    0,    0,
    0,  820,    0,    0,    0,  832,  820,  832,    0,  820,
    0,    0,    0,    0,    0,    0,  824,    0,    0,  824,
    0,    0,    0,    0,    0,  820,  824,    0,    0,  824,
    0,    0,    0,  824,    0,    0,    0,    0,  824,    0,
    0,  824,    0,    0,  833,    0,    0,    0,    0,  820,
    0,    0,  820,  820,    0,    0,    0,  824,    0,    0,
    0,    0,  820,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  821,    0,    0,    0,    0,    0,  820,
    0,  824,    0,    0,  824,  824,    0,    0,    0,    0,
    0,    0,    0,    0,  824,    0,    0,    0,    0,    0,
  833,  820,    0,  833,    0,    0,  820,  825,    0,    0,
  833,  824,    0,  833,    0,    0,    0,  833,  820,    0,
  820,    0,  833,    0,    0,  833,    0,    0,    0,  821,
    0,    0,  821,  824,    0,    0,    0,    0,  824,  821,
    0,  833,  821,    0,    0,  321,  821,    0,    0,    0,
  824,  821,  824,    0,  821,    0,    0,    0,    0,    0,
    0,    0,    0,  825,    0,  833,  825,    0,  833,  833,
  821,    0,    0,  825,    0,  321,  825,    0,  833,    0,
  825,    0,  321,    0,    0,  825,  321,  321,  825,  321,
    0,    0,  321,    0,  821,  833,    0,  821,  821,    0,
    0,    0,    0,    0,  825,    0,  321,  821,    0,    0,
    0,    0,  321,    0,    0,    0,    0,  833,    0,    0,
    0,    0,  833,    0,  821,    0, 1431,    0,  825,    0,
    0,  825,  825,    0,  833,    0,  833,    0,    0,    0,
    0,  825,    0,    0,    0,    0,  821,    0,    0,    0,
    0,  821,    0,    0,    0,    0,    6,    0,  825,    0,
    0,    0,    0,  821,    0,  821,    0, 1544, 1545,    0,
 1546,    0,  321, 1547,    0,    0,    0,    0,    0,    0,
  825,    0,    0,    0,    0,  825,    0, 1548,    0,    0,
    0,  321,    0, 1549,    0,    0,    0,  825,    0,  825,
    0,    0,    0,    0,    0,    0,  321,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  321,    0,    0,    0,
    0,    0,    0,    0,    0,  321,    0,  321,    0,    0,
    0,    0,    0,    0,  321,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  595,    0,    0,    0,
  596,    0,    0, 1550,    0,    0,    0,  795,    0,    0,
    0,    0,    0,  321,  321,    0,  321,    0,  597,    0,
    0,  598,   14,  321,  321,  321,  321,  321,    0,  321,
    0,    0,  321,  321,  321,  321,  321, 1551,    0,    0,
  321,  321,    0,    0,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0, 1675,    0, 1552,    0,
    0,    0,  599,    0,    0,  105,    0,    0,  600,  601,
  602,    0,  603,  604,  605,    0,  606,  595,    0,    0,
    0,  596,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1553,  107,  607,  108,  608,  597,
    0,    0,  598,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115, 1554,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,    0,    0,  609,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  599,    0,    0,  610,    0,    0,  600,
  601,  602,    0,  603,  604,  605,    0,  606,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  611,    0,
    0,    0,    0,    0,    0,    0,    0,  607,    0,  608,
    0,    0,    0,  105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  612,  613,    0,    0,    0,    0,    0,
  609,    0,  106,  107,    0,  108,    0,    0,    0,    0,
  614,    0,  109,  110,  111,  112,  113,  610,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,    0,    0,    0,    0,  611,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  612,  613,    0,    0,    0,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,  614,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,  450,    0,    0,
  120,  121,    0,    0,    0,    0,    0,  450,  450,  450,
  450,  450,  450,  450,  450,  450,  450,  450,  450,  450,
  450,  450,    0,    0,  450,  450,  450,  450,    0,  450,
    0,    0,  450,  450,    0,    0,  450,  450,  450,  450,
  450,  450,  450,  450,    0,  450,  450,  450,  450,  450,
    0,    0,    0,  450,  450,
};
static short yycheck[] = {                                       9,
   16,   55,   12,  104,    4,    4,   16,  151,   18,   64,
    0,  256,  140,  141,  287,   56,  140,  141,   63,  513,
  355,  363,  710,  363,  554,  675,   67,  557,   57,   63,
   85,  359,   94,  563,  334,   89,   73,  520,  987,  423,
   69,  496,   62,   53,   54,   55,   56,   57,   58,   90,
  563,  332,   88,   63,   64,   65,   97,   67,   95,   69,
   70,  770,   72,   73, 1043,  687,   70,   83,  756,  695,
 1264,   81,  647,   83,   84,   85,   92,   87,   88,   89,
   90,  281,   92,   93,   94,   95,  471,   97, 1036,   93,
  368,  264,  154,   91,  380, 1043,  939,  131,  724,  133,
  273,  831,    0,  132,   44,  728,    0,    0,    0,  145,
  760,  152,    0,  640,  862, 1044,  709,    0, 1036,   40,
  763,  131,  132,  133,  153, 1043, 1055,  228,    0, 1036,
  308,    0,   41, 1036,  977,  145, 1043,   59, 1036,   40,
 1043,  151,  152,  153,  154, 1043, 1075,   41,  158,  792,
   44,  327,   41,   44,  987,   44,    0,    0,  801,   44,
    0,   59,   40,    0,  308,   59,   59,   59,    0,  189,
    0,   59,   41,  804,   40,   44,   59,    0,  423,  477,
   45,    0,  158,   45,   41,   63,  766,   59,   41,  728,
   59, 1069,   44,   63,  214,   44,  274,   63,   41,   40,
  271,   41,  530,  653,   44,  540,  541,  542,  543,   41,
    0, 1608,  855,   44,  315,   59,   59,   44,    0,   59,
 1471,  563,   59,  563,  258, 1222,   44,   59,    0,   59,
   45,   44,    0,  262,   45, 1312,   59,  515,  292,    0,
   59,   41,  271,  737,    0,  161,  357,   45,  258, 1262,
 1093,  262,  262,  262,  264,  265,  355,   41, 1092,    0,
   44,  271,  590,  273,   44,   63,  325,  296,  264,   59,
   12,  268,    0,   41,  308,   12,   44,   59,   42, 1199,
    0, 1201,  292,  905,  906,  566,  296,   59,  818,  260,
  463,   59, 1707,  301,   41,  387, 1693,   91,   59,  333,
  307,  308,    0,   59, 1087,  219,  340,   40,  342,  595,
  344, 1290,   45,  347,  936,   44, 1751, 1752,   59,  273,
    0,   41,  376,  333,  365,   41,  445,  471,  978,  301,
  340,   59,  342,  300,  344,  616,  356,  347,  358,   59,
   41,   41, 1290,   41,   44,  374,   44,  970,  440, 1746,
 1279,   41,  372,  455, 1382,  365,  366,  307,  308,  982,
  983,   59,  366,    0,  374,  283,  376, 1010,  365, 1397,
    0,  430, 1290,   41,  320,  386,   44,  386,  437,   59,
  380,  356, 1797, 1290,  839,  352,  340, 1290,  466,  508,
 1223, 1585, 1290,  492, 1791, 1206,  138,  311, 1475,    0,
    0,  138,  675,  319,   41, 1402,  517,   44,  479,  262,
 1845, 1054,  264, 1035,  424,  411, 1047, 1048, 1296,  304,
  318,  462,   59, 1436,  435,    0,  435,  318,  269,   59,
  459,  970,  882,  731,  442,  279,  477,  471,  331,   41,
 1691,  470,  340,  982,  451,  474,  417,  692,  340,  459,
  479, 1234,  462,  463,  429,  402,  300,  581,   59,   59,
  470,  318,  506,  708,  474,  738, 1097,  477,  340,  479,
  442,  320,  482,  304,  264,  318,    0, 1207,  318,    0,
 1400,   41,  264,  277,   59,  329,  318,  760,  410, 1683,
  274, 1113, 1114,  394, 1116, 1117,  260,  340, 1427,  780,
  340,  451,  783, 1432,  348, 1434,  482,  279,  340,  309,
  340,  348,  356,  357,  383,  535,  797, 1097,  458,  363,
   41,  439,  411,   44,    0,  309,  447, 1102,  300,  563,
 1239,  440, 1125,  386,  734,   59,  264,  381,   59,  303,
 1469,    0,    0,  824,  387, 1739,  719,  440,  436,  393,
  318,  395,  562,  563, 1302, 1285,  454,  329,  568, 1086,
  258,  454,  454,  331,  584,   41,  866,  328,   44,  431,
  331,  454,  340,  271,  418,  853,  348, 1410,    0,  757,
  458,  279,  454,   59,  582,  357, 1780,  410,  458,  875,
  975,  363,  458,  458, 1217,  595,  458,  440,    0,  443,
   59,   59,  300,  779, 1482,  639, 1535,  501,  306,  381,
  308,  454, 1541,  757,  454,  397,  636,  637,  742,  387,
  485,  440,  454, 1245,  454,  452,  367,  411,  396,  639,
    0,  329,  501,  431, 1468,  454,  454,   59,  648,   41,
  387,  454,  678,  458, 1487, 1267,  560,  458,  648,  648,
  348,  775,  776,  409,  383,    0,  436,   59, 1301,  357,
  458, 1590,  502,  264,  264,  363, 1449,  387,  678,  397,
  386,   41,  440,  517,   44, 1703,    0,  711, 1607,  440,
 1423,  318,  466,  381, 1467,  386,  454,  367, 1023,   59,
  731,  452,  702,  454,   41,   61,   41,  387,  431,   44,
  710,  711,  262,  340,  714,  978,  396, 1333,    0,  719,
  340, 1522,  338, 1422,   59,    0,  630,    0,  728,    0,
  418,  731,  751,  757,  753,  458,  736, 1470,  762,  331,
  740,   42,  766,    0, 1713,   59,   47,   41,  260,    0,
  264,  751,  653,  753, 1693,  517,  756,  658, 1398,  769,
  770,  771,  762, 1088,   41,  789,  766,  278,   41,  280,
 1041, 1690,  320, 1036,  417, 1713,  287,   59,   40,  286,
 1043, 1700, 1053,  348,   59,    0,   59,  313,   59,  789,
  778,   44, 1265,  817,  818,  387,  959,  262,  257,  962,
 1412,  444,   59, 1074,  396, 1713,  397,  397,   59,  258,
 1100,  262,  278, 1187,  280,  257, 1713,  817,  818,  829,
 1713,  287,  271,   41, 1743, 1713,  442,  454,  816,  517,
  279,  279,   41,  823,    0,  854,  386,  301,    0,    0,
   93,  975,    0, 1288,   59,  864,  836, 1221,  238, 1294,
  265,  300,  300,  263,  854,  277,  246,  306, 1361,  308,
  377,  849,  850,  851,  864,    0, 1210,  378,  402,  348,
  273,   16,  383, 1163,  338,  334,  876,  409,  357,   41,
  329,  329,   44,  397,    0,  875,    0,  302,  436,  400,
    0,  417,  334,   59,    0,   44,  407,   59,   59,  348,
  348,   59,  312,  402,  438,  417,   41,  356,  357,  357,
   59,  963,  378,  323,  363,  363,  328,   59,  278,  331,
  280,  386,  337,  345,   59,   41,  318,  287,   44,  960,
  441,  448,  381,  381,  400,  386,  328,  964,   83,  331,
  465,  407,  467,   59,  300,   59,  445, 1210,  340,   59,
  264,  975, 1187,   59,  461,  462,    0,  348, 1072,  959,
  960,  961,  962,  963,  964, 1309,    0,  961,  262,  418,
  970,    0,  307,  308, 1257,  441,  258,  878,  442,  280,
  429,  882,  982,  983,  285,  262, 1221,  987, 1306,  271,
  501,  328,  377,  264,  331,  387, 1639,   41, 1641,  403,
   44,  258, 1795, 1796,  396,  261, 1277,   41,  259,  508,
  259,  458,   41,  264,  271,   59,  420,  427,  378,  270,
 1303,  270,  279, 1543,  306,   59,  308, 1290,  440, 1361,
   59, 1361, 1644,  418,  405,  501, 1520, 1308,  517, 1523,
  400,  265,  454,  300,  262,  397, 1309,  407,  440,  306,
  387,  308, 1664,  262,  405, 1667, 1699,  343,  331,  396,
    0, 1854,  454,  278, 1064,  280, 1090,  342,  517,  517,
  461,  462,  329, 1097,  458,  410, 1066, 1066,  302, 1079,
  465,  441,  368,  397,  436,  456,    0, 1205, 1206, 1207,
 1090,  348,  386, 1081, 1419, 1773,  278, 1097,  264,    0,
  357,   41,  264,  264,   44,  456,  363, 1563,  352,  386,
 1177, 1099, 1693, 1180,  387,  458,  451, 1105,  470,   59,
  278,   61,  280,  396,  381, 1609,  397, 1241,    0, 1243,
  405, 1414,    0,  408,  378, 1398,  418,  319,  322,  259,
  426,  501,  485,  319,  258,   59,  397,  322,    0,  424,
  270, 1794, 1705, 1706,  264,  417,  318,  271,   59,  321,
  260,  418,  278,  378,  280, 1746,  328, 1438,  386,  331,
  401,  287,  278,  335,  280,  450,   44,  386,  340,   42,
   43,  343,   45,  318,   47,  258,   40,   59, 1176,   41,
 1304,   59,  306,  328,  308, 1748,  331,  359,  460, 1199,
  282, 1201,  493,  303,  260,  340, 1477,   59,  290,    0,
 1791,    0,  503,  256,    0, 1205, 1206, 1217,  400,  279,
  264,  383,    0, 1223,  400,  387,    0,  489,  490, 1239,
  492,  397, 1844,  306,  396,  278,  397,  499,  500,  501,
  502,  503,    0,  505,  287,  301,  508,  509,  510,  511,
  512,  413,  387,  260,  516,  517,    0,  313,  283,  441,
  517,  396,  378,  260,    0,  441, 1750,   41,   59,  329,
   59, 1405, 1825,   59,  383,  318,  320,    0,  440,  301,
  417,   59,  338,   41,  400,   59,   44,  397, 1721,  318,
  452,  407,  454, 1283, 1283,   44,  303, 1431,  460,  328,
  383,   59,  331,  363,  418,  440,  303,  444, 1375, 1319,
   44,  340, 1379, 1323, 1597,   59,  338,  452,  258,  454,
 1603,  381, 1778,   59,    0,  441, 1672,  489,  490,    0,
  492, 1764,   44, 1604,  274,  385,   59,  499,  500,  501,
  502,  503,    0,  505,   93,  418,  508,  509,  510,  511,
  512,  407,   42,  387,  516,  517, 1812, 1813,  387,  384,
  300,  417,  396,  436,    0,  378,  306,  396,  877,  309,
   41,  258,  881, 1806,  417, 1721, 1809,  287,  421,  422,
  423,   93, 1838,   59,  271,  501,  442,  400,   59,  321,
  300, 1525,    0, 1849,  407, 1430, 1852,  470,  383, 1832,
 1400,   59,  383,  335, 1522, 1405, 1430, 1431,  452,  260,
 1410,  440, 1422, 1695,  439,    0,  460,  280, 1764,  306,
 1439,  308,  285,   59, 1538,  454, 1772,  359,  441, 1563,
 1430,  456,  436,    0,    0, 1672,   44,  260,   44, 1439,
  483,  342,    0,    0,  487,  489,  490,    0,  492, 1516,
 1713,   59,  303,   59,    0,  499,  500,  501,  502,  503,
 1806,  505, 1744, 1809,  508,  509,  510,  511,  512,  258,
  342,  411,  516,  517,   59,  356,  377,  340,  418,  494,
  303,  413,  271, 1718, 1721,   44, 1832,   44,   41,    0,
  279,   44,   59,   59,   46,   41,  436, 1588,   44, 1544,
  337,   59,   59,  418,  405,  377,   59,  408, 1790,   61,
  294,  300,  393,   59,  395,   42,   43,  306,   45,  308,
   47,  305, 1512,  424,  402, 1525,  466, 1764,  312,    0,
  470,  418, 1522,  405,   93, 1772,  408,  418, 1672, 1563,
  329,    0, 1824,   46, 1544, 1589,    0,  405,   59,  450,
    0,  342,  424,  337,  328,  274,  434,  331,   61,  348,
  337,  301,  443,  321,  318, 1584,  424, 1547,  357, 1806,
  328,  449, 1809,  331,  363,    0,   40,  335,  450, 1579,
    0,    0,    0,    0, 1584,  343,  377, 1721,   59, 1589,
   40,   41,  381, 1684,   44, 1832,  424,  374,  338,  280,
   59,  359, 1608, 1613,  285,   59,  296, 1617, 1608,   59,
  387,    0,    0,  387,  405,  658,  461,  408,  402,  272,
  264,   41,  396,   41,   44,  383, 1657,    0,   41,  418,
 1764,   44,  675,  424,   59,  288,  679,  316, 1772,   59,
   59,   59,   59,  686, 1778,  493,  494,  318,  691,  692,
  434,    0,    0,   41,  438,  413,   44, 1657,  701,  450,
  703,  704,  705,  706,  707,  708,  709,  407,    0,  340,
   59,   59, 1806,    0,    0, 1809,  321,  417, 1812, 1813,
  333,   41,  440,  318,   44, 1730,   59, 1693,    0,    0,
  335,    0,    0, 1693,  452,  738,  454,  337, 1832,   41,
   42,   43,  442,   45, 1838,   47, 1712, 1707,    0,  362,
   59,   59, 1712,  460,  359, 1849,  387,  760, 1852,  336,
  328,    0,  765,  331, 1199,  396, 1201,   59,  517,   41,
 1730, 1731,   59,   59,  452,  343,   41,    0,  383,   44,
 1746,   41, 1732, 1732,   44,  402, 1746,   59,   59, 1755,
   59,   59,  340,  280, 1778, 1755,   42,   43,  285,   45,
 1804,   47,  383,   41,   40,  318,   44,   59,  413,  440,
  460,  328,    0, 1773,  331,  328,  429,  320,  331,   41,
   59,   41,   44,  454,   44, 1791,  343,  340, 1812, 1813,
  343, 1791,  318,  320,  383, 1801,   59, 1797,  402,  489,
  490, 1801,  492,  258, 1804,   41,  261,  436,   44,  499,
  500,  501,  502,  503, 1838,  505,   44,  506,  508,  509,
  510,  511,  512,  316,  317, 1849,  516,  517, 1852,  337,
 1830,   59,  440,   93,  387,   42,   43, 1837,   45,  361,
   47,  450,   41,  396,  452,   44,  454,  890,  891,  892,
  434,  306,  895,  896,  897, 1855,  899,  900,  901,  902,
  903,  904,    0,  417,  907,  908,  374,  910,  318,  912,
  384,  321,  915,  916,  917,  918,  919,  920,  328,  387,
  442,  331,  454,  440,  516,  335,  516,  440,  321,  932,
  340,  934,  287,  343,   93,  452,  411,  454,  318,  452,
  318,  454,  335,   41,   41,  461,  462,   44,  328,  359,
   44,  331,    0,  331,  391,  392,  457,  458,  459,  469,
  340,   59,  340,  343,  372,  342,  359,   42,   43,   61,
   45,   41,   47,  383,   44,  978,   41,  387,  280,   44,
  461,  462,  286,  285,  485,  486,  396,  264,   41,  342,
  383,   44,   40,   41,   42,   43,   44,   45,   46,   47,
  377,   41,   41,  413,   44,   44,  258,  387,   40,  387,
   40,   59,   60,   61,   62,  674,  396,  676,  396,  271,
  413,  680,  681,  682,  377,  461,   41,  279,  405,   44,
  440,  408,   41,   40,  280,   44,   40,   40,   40,  285,
   40,   40,  452,   91,  454,   93,  318,  424,  300,   40,
  460,   40,  405,   40,  306,  408,  308,   40,  461,  331,
  440,  461,  440,   40,   40,  311,   40,   40,  340,   40,
   91,  424,  452,  450,  454,   44,  454,  329,   46,  489,
  490,   44,  492,  320,  436,  402,  320,   44,  264,  499,
  500,  501,  502,  503,   44,  505,  348,  450,  508,  509,
  510,  511,  512,  428,   46,  357,  516,  517,   42,   61,
   40,  363,  264,  280,  321,  387,    0,   44,  285,   41,
   42,   43, 1125,   45,  396,   47,  382,  348,  335,  381,
  505,  348,  348,  321, 1137, 1138,  343, 1140, 1141,   44,
  328,  278,  278,  331,  300, 1148,  264,  335,  397,  470,
  397,  436,  359,  320,  277,  343,  348,   41,  458,  447,
   44,   40,   42,   43, 1167,   45,  418,   47,  440,  447,
  399,  359, 1175,   40, 1177,   59,  349, 1180,   91, 1182,
   40,   40,  454, 1186, 1187, 1188, 1189,   42,   43,  318,
   45,  264,   47,  383,   44,  383,   41,   42,   43,  411,
   45,  382,   47,  389,   59,  280,  413, 1210,   40,   40,
  285,  451,   41,  464,  262,  317,  264,  265, 1221,   40,
  318,  436,  452,  448,  272,  413,  264,   41,   42,   43,
  418,   45,  280,   47,  320,  320,  311,  285,   42,   43,
  288,   45,  340,   47,  502,  458,  277,   41,   42,   43,
   40,   45,  440,   47,  302,   59,  274, 1260,   40,  307,
  308,  284,  344,  311,  452,  517,  454,  344,  278,  386,
  318,  300,  320,  321,   44,  386,  324,  386,  418,  418,
  328,  416,  284,  331,  300,  333,   40,  335,  321,  387,
  356,  417,  340,  341,   40,  343,  274,  417,  396,   40,
  264,   41,  335,  320,   40,   44, 1309,   44,    0,   44,
  343,  359,  360,  280,  362,   41,   42,   43,  385,   45,
  367,   47,   58,   44,  320,   40,  359, 1330,  376,  266,
  378,  379,  380,   44,  328,  383, 1339,   44,  386,  387,
   40,  299,  440, 1346,  501,   40,  264,   41,  396,   41,
   42,   43,   44,   45,   46,   47,  454,   58,  280,  422,
   41,   41,  410,  285,   41,  413,  264,   59,   60,   61,
   62,  454, 1375,  309,  394,   41, 1379,  492,  440,   40,
  413,  429,   41,  309,   40,  265,   40,  435,   40,   40,
  352,   40,  440,   40,  278, 1398,  280,   40,   40,   91,
  280,   93,   40,  451,  452,  285,  454,   40, 1411,   40,
   40,   40,  460,   40,   40,   40,   40,  308,  262,  327,
  343,  435,  302,   44,   41,  280,  274,   41,   42,   43,
  285,   45,  388,   47,  388,  280,  331,  388,  422,   41,
  285,  489,  490,  390,  492,  391,  417,  271,  446,  287,
  378,  499,  500,  501,  502,  503,   41,  505,  460,  407,
  508,  509,  510,  511,  512,  386,  280,  386,  516,  517,
  895,  285,  897,  278,  899,  900,  280,  902,  903,  904,
  386,  285,  386,  386,  386,  340,  280,  489,  490,  460,
  492,  285, 1495, 1496,  378,   40,  308,  499,  500,  501,
  502,  503,   41,  505,   41,  417,  508,  509,  510,  511,
  512,   41,   41, 1516,  516,  517,  400,  385,  489,  490,
  435,  492,  436,  407,  280,  396,  460,  337,  499,  500,
  501,  502,  503,  442,  505,  338,  301,  508,  509,  510,
  511,  512,  442,  338,  301,  516,  517, 1550,   41,   41,
  320,  352,  320,  371,  280,  489,  490,  441,  492,  285,
  385,   41,  371,   41,   41,  499,  500,  501,  502,  503,
  262,  505,  264,  265,  508,  509,  510,  511,  512,   41,
  272,   41,  516,  517,   42,   43,   44,   45,  280,   47,
   41,   41,   41,  285,   42,   43,  288,   45,   41,   47,
  472,  473,  474,  475,  476,  477,  478,  479,   41, 1612,
  302,   41,  304,   41,   41,  307,  308, 1620,    0,  311,
   42,   43,   41,   45,   41,   47,  318,   41,  320,  321,
   45,   44,  324,   59,   40,  340,  328,   40,   59,  331,
  308,  333, 1645,  335,  340,   41,  352,   59,  340,  341,
   59,  343,   59, 1656,   59,   59,  344, 1660,   59,   41,
   42,   43,   44,   45,   46,   47,  280,  359,  360,   41,
  362,  285,   40,   40,  451,  368,  340,   59,   60,   61,
   62,  383,   44,   59,  376,   59,  378,  379,  380,   46,
  460,  383,  383,  383,  386,  387,   41,   42,   43,   41,
   45,   41,   47,  343,  396,   41,   44,  410,  435,   91,
  304,   93,  301,  442,  387, 1718,   40, 1720,  410,  489,
  490,  413,  492,   58,  460,   59,  293,  304,   44,  499,
  500,  501,  502,  503,  307,  505,   44,  429,  508,  509,
  510,  511,  512,  435,  463,   41,  516,  517,  440,  337,
    0,  383,   40,  489,  490,  383,  492,  264,  278,  451,
  452,  264,  454,  499,  500,  501,  502,  503,  460,  505,
  389,  458,  508,  509,  510,  511,  512,  320,  264,  451,
  516,  517,   41,   42,   43,  470,   45,   44,   47,  411,
   41,   42,   43,   40,   45,  309,   47,  489,  490,  460,
  492,  458,  271,  264,  271,  343,  308,  499,  500,  501,
  502,  503,  435,  505,  262,  343,  508,  509,  510,  511,
  512,  436,  280,  458,  516,  517,  343,  285,  489,  490,
  343,  492,  280,   41,   40,  308,  460,  285,  499,  500,
  501,  502,  503,   40,  505,   59,   59,  508,  509,  510,
  511,  512,  308,   41,   59,  516,  517,  304,  280,   41,
   59,   41,    0,  285,    0,  489,  490, 1202,  492,    0,
   12,  273,  266,  265, 1102,  499,  500,  501,  502,  503,
  262,  505,  264,  265,  508,  509,  510,  511,  512,  719,
  272,  643,  516,  517,   42,   43, 1100,   45,  280,   47,
   87,  867, 1097,  285, 1089, 1457,  288, 1458,   41,   42,
   43,  590,   45,    0,   47,  360, 1079,  342, 1026,  330,
  302,  762,    0,  323,  313,  307,  308, 1654, 1090,  311,
  799, 1579,  342, 1737, 1436,  280,  318, 1740,  320,  321,
  285, 1438,  324, 1741, 1258, 1782,  328,  789,  568,  331,
 1718,  333, 1662,  335,   41, 1837, 1677,   44,  340,  341,
 1779,  343, 1012,   41,   42,   43,   44,   45,   46,   47,
 1236, 1415,   59, 1693,   61,  335,  347,  359,  360,  570,
  362,   59,   60,   61,   62,  328, 1292,  838,  846,   13,
  629,  300,  498,  502,  376,  738,  378,  379,  380,  765,
  751,  383,  143, 1645,  386,  387,  987, 1801, 1533, 1855,
 1755,  727,   59,   91,  396,   93,  513, 1139,  979,   -1,
 1247,  280, 1182,   -1,   -1,   -1,  285,   -1,  410,  280,
   -1,  413,   -1,   -1,  285,   41,   42,   43,   -1,   45,
   -1,   47,   -1,  458,   -1,  460,   -1,  429,   -1,   -1,
   41,   42,   43,  435,   45,   -1,   47,   -1,  440,   -1,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,  451,
  452,   -1,  454,   -1,  489,  490,   -1,  492,  460,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
    0,  516,  517,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  262,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  460,  508,  509,  510,  511,
  512,   -1,  280,   -1,  516,  517,   -1,  285,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,  489,  490,   -1,  492,   -1,   59,
   -1,   61,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,  258,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,  262,   -1,  264,  265,   -1,   -1,
  277,  278,   -1,  280,  272,   -1,   -1,  284,   -1,   -1,
  287,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,
  288,   -1,   -1,  300,   -1,   -1,   -1,    0,   -1,  306,
   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   45,  311,   -1,   41,   42,   43,   -1,   45,
  318,   47,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   41,   -1,
   -1,   44,  340,  341,   -1,  343,   -1,   41,   42,   43,
   44,   45,   46,   47,  280,   -1,   59,   -1,   61,  285,
   -1,  359,  360,   -1,  362,   59,   60,   61,   62,  280,
   -1,  378,   -1,   -1,  285,   -1,   -1,   -1,  376,  280,
  378,  379,  380,   -1,  285,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,  400,   -1,   -1,   -1,   91,  396,   93,
  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  416,
   -1,  418,  410,   -1,   -1,  413,   -1,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,  436,
   -1,  429,   -1,   -1,  441,   -1,   -1,  435,  258,   41,
   42,   43,  440,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,  277,  278,   -1,
  280,   -1,  460,  470,  284,   -1,   -1,  287,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,   -1,
   -1,  489,  490,  460,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,  280,  258,   -1,   -1,  378,  285,
   -1,   -1,  295,   -1,   -1,   -1,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,  278,   -1,  280,  272,   -1,
  400,  284,   -1,   -1,  287,   -1,  280,  407,   -1,   -1,
  323,  285,   -1,   -1,  288,   -1,   -1,  300,  418,   -1,
   -1,   -1,   -1,  306,    0,   -1,   -1,   -1,  302,   -1,
    0,   -1,  279,  307,  308,   -1,  436,  311,   -1,  352,
   -1,  441,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,  300,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   40,   41,  340,  341,   44,  343,
  470,   41,   42,   43,   44,   45,  280,   47,   -1,   -1,
   -1,  285,  329,   59,   -1,  359,  360,   -1,  362,   59,
   60,   61,   62,   -1,   -1,  378,   -1,   -1,  280,   -1,
   -1,  348,  376,  285,  378,  379,  380,   -1,   -1,  383,
  357,   -1,  386,  387,  427,   -1,  363,  400,   -1,   -1,
   -1,   -1,  396,   93,  407,   -1,   -1,   -1,   -1,   -1,
  443,   -1,  294,   -1,  381,  418,  410,   -1,   -1,  413,
   -1,   -1,   -1,  305,  457,  458,  459,   -1,  461,  462,
  312,   -1,   -1,  436,   -1,  429,   -1,   -1,  441,   -1,
   -1,  435,   -1,  325,   -1,   -1,  440,  480,  481,  482,
  483,  484,  485,  486,  487,  488,   -1,  451,  452,   -1,
  454,   -1,  495,  496,   -1,   -1,  460,  470,   -1,   -1,
   -1,  504,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,    0,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,  386,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  402,   -1,  516,  517,   -1,   -1,   -1,   -1,   41,   41,
   -1,   44,   44,   -1,   -1,   -1,   -1,  419,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   59,   -1,   -1,
  517,   -1,  434,   -1,   -1,   -1,  438,   -1,   -1,  441,
   -1,    0,   -1,   -1,   -1,    0,   -1,  449,   -1,  305,
   -1,   -1,  262,   -1,  264,  265,  312,   -1,   -1,   -1,
  316,  317,  272,   -1,   -1,   -1,  468,   -1,   -1,  325,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  337,   41,   -1,   -1,   44,   41,   -1,   -1,   44,
   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   59,  311,  318,   -1,   59,  321,   -1,   -1,  318,   -1,
  320,  321,  328,   -1,  324,  331,   -1,   -1,  328,  335,
    0,  331,   -1,  333,  340,  335,   -1,  343,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  359,   -1,   -1,  402,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   41,   -1,  419,   44,   -1,  376,  383,  378,  379,
  380,  387,   -1,  383,   -1,   -1,  386,  387,  434,   59,
  396,   -1,  438,   -1,   -1,   -1,  396,   93,   -1,   -1,
   -1,   -1,   -1,  449,   -1,   -1,   -1,  413,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  468,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,    0,   -1,  440,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,  451,  452,   -1,  454,  278,  278,  280,  280,   -1,
  460,  284,  284,   -1,  287,  287,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,  300,  300,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   59,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
    0,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  278,
   -1,  280,   -1,  278,   -1,  280,   -1,   -1,  287,   -1,
   59,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  300,   -1,   -1,   -1,  378,  378,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,  262,   47,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,  400,  400,   59,
   60,   61,   62,   -1,  407,  407,   -1,   -1,   -1,  285,
   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,  278,   -1,
  280,  307,  308,   93,   -1,  311,   -1,  287,  441,  441,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,  378,
  300,   -1,  328,  378,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,  400,   -1,   -1,   -1,  400,   -1,    0,  407,   -1,
   -1,   -1,  407,  359,  360,    0,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,  441,   -1,   -1,   -1,  441,   40,   41,   -1,
  396,   44,   -1,   -1,   -1,   -1,   41,   -1,  378,   44,
  278,   -1,  280,   -1,  410,   -1,   59,  413,   61,  287,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
  400,   -1,   -1,  429,   -1,   -1,   -1,  407,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,  278,
   -1,  280,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
  378,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,  400,   -1,   -1,   -1,   -1,   -1,  318,  407,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,  378,
  340,  341,   -1,  343,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  441,   -1,   -1,   -1,   -1,   -1,  359,
  360,  400,  362,   59,   60,   61,   62,   -1,  407,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  274,   -1,  413,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,
  440,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  359,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,   -1,  387,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,  396,   -1,   -1,  407,   -1,   -1,  287,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,  413,   -1,
   -1,   41,   -1,   43,   44,   45,  262,   -1,  264,  265,
   -1,   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   59,
   60,   61,   62,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,  288,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,  466,   -1,   -1,  302,  470,   -1,   -1,
   -1,  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1, 1331, 1332,   -1,   -1,   -1,    0, 1337, 1338,
   -1, 1340, 1341,  359,  360,    0,  362,   -1,   -1, 1348,
 1349, 1350, 1351,   -1, 1353, 1354, 1355, 1356, 1357, 1358,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
  396,   44,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,  410,   -1,   59,  413,   61,   -1,
  460,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   91,  489,
  490,   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,
  500,  501,  502,  503,  460,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,  274,   -1,  413,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,
  440,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  359,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,   -1,  387,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,  396,   -1,   -1,  407,   -1,   -1,  287,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,  413,   -1,
   -1,   41,   -1,   43,   44,   45,  262,   -1,  264,  265,
   -1,   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   59,
   60,   61,   62,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,  288,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,  466,   -1,   -1,  302,  470,   -1,   -1,
   -1,  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
  396,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,  410,   -1,   59,  413,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,
  500,  501,  502,  503,  460,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,  318,   -1,   -1,  321,   -1,
  460,   -1,   -1,   -1,   -1,  328,  318,   -1,  331,  321,
   -1,  434,  335,   -1,   -1,   -1,  328,  340,   -1,  331,
  343,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,  489,
  490,  343,  492,   -1,   -1,   -1,  359,  460,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  359,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,  489,  490,   -1,  492,
   -1,  383,   -1,  396,  386,  387,  499,  500,  501,  502,
  503,   -1,  505,   -1,  396,  508,  509,  510,  511,  512,
  413,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,  435,   -1,   -1,   -1,  272,  440,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,  452,
   -1,  454,  288,   -1,   -1,   -1,   40,   -1,   -1,   43,
  452,   45,  454,   -1,   -1,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   -1,   58,  311,   -1,   -1,   -1,   63,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
  396,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   58,  328,   -1,  410,  331,   63,  413,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,   -1,   -1,
   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,  413,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,  267,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  314,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  323,
   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   41,
  516,  517,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
  267,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,
   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,  292,   -1,   -1,  295,  296,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   58,
   -1,   -1,   -1,  417,   63,   -1,  323,  421,   -1,  326,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  378,  443,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,  460,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,  513,
  514,  515,  516,  517,  421,   -1,   -1,   -1,  489,  490,
  427,  492,   -1,   -1,   -1,   -1,  433,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  443,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  260,   -1,   -1,   -1,   -1,  318,   -1,  267,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,  276,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
  289,  343,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   40,   -1,   42,   43,  303,   45,   -1,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  323,   -1,   -1,  326,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,  435,   44,   -1,   40,  261,  440,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
  452,   -1,  454,   -1,   58,   -1,   -1,   -1,  282,   63,
   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,
   -1,   -1,   -1,  297,   -1,  299,   -1,  301,  427,   -1,
   -1,  343,  306,   -1,  433,   -1,   -1,   -1,   -1,  313,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  327,   -1,   -1,  368,   -1,  457,  458,
  459,  460,  461,  462,  338,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,  426,   -1,   -1,  516,  517,   -1,
  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,  405,   -1,   -1,   -1,   -1,  276,   -1,  412,   -1,
   -1,  415,   -1,  417,  418,   -1,   -1,   -1,  460,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  442,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  262,   -1,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,  352,  267,  354,  355,   40,  491,   -1,   43,
   -1,   45,  276,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,  506,  507,   58,  289,   -1,   -1,   -1,   63,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  314,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,  323,
   -1,  331,  326,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  359,
  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,  459,
  460,  461,  462,  383,  378,   -1,  386,  387,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,  413,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  421,  508,  509,
  510,  511,  512,  427,   -1,  435,  516,  517,   -1,  433,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  267,  498,  499,  500,  501,  502,  503,
  504,  505,  276,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   40,  289,   -1,   43,   -1,   45,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  273,
   -1,   -1,   -1,  277,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  291,   -1,   -1,
   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,  443,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,  373,
   -1,  375,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  406,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  267,  516,  517,   -1,   -1,   -1,   -1,   -1,  423,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  272,  323,   -1,   -1,
  326,   -1,   -1,   -1,  280,   -1,  480,  481,   -1,  285,
   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  497,   -1,   -1,   -1,   -1,  354,  355,
   -1,  260,    0,   -1,  263,   -1,   -1,   -1,  267,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  276,  324,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,  333,   -1,   -1,
  289,   -1,   -1,   -1,   -1,  341,  295,  296,   -1,   40,
   -1,   -1,   43,   41,   45,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,  362,   58,   -1,   -1,
   -1,   59,   63,   61,  323,  421,   -1,  326,   -1,   -1,
  376,  427,  378,  379,  380,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   91,   -1,  354,  355,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,  328,   -1,   -1,   -1,   -1,
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
  278,   -1,  280,   -1,   -1,  402,  284,   -1,  289,  287,
  443,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
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
   -1,   40,  318,   -1,   43,   -1,   45,  457,  458,  459,
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
   -1,   -1,  323,   63,   -1,  326,   -1,  453,  489,  490,
   -1,  492,   -1,   -1,  460,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  354,  355,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,  366,  492,   -1,   -1,  370,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  511,  512,   -1,    0,   -1,  516,  517,  366,   -1,   -1,
  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   41,  295,  296,   44,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,  425,   58,  427,   -1,
   -1,   -1,   63,  323,  433,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,
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
  510,  511,  512,   -1,   -1,  262,  516,  517,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,
   -1,   -1,   63,   -1,  295,  296,   -1,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   60,   61,   62,   -1,   -1,
   -1,  328,  323,   -1,  331,  326,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,  366,   -1,   40,   -1,  370,
   43,   -1,   45,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  396,
   63,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  413,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,  435,   -1,
   -1,   -1,  433,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,  500,
  501,  502,  503,  504,  505,  276,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,  272,    0,   -1,
   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,  352,   -1,  354,  355,   -1,   -1,   59,  333,   -1,
   -1,   -1,   -1,   -1,  267,  366,  341,   -1,   -1,  370,
   -1,   -1,   -1,  276,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,  360,  289,  362,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,  429,   -1,  457,  458,  459,  460,
  461,  462,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  417,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,    0,   -1,  289,  328,   -1,   -1,  331,
   -1,  295,  296,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,  323,
   -1,   -1,  326,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,  383,   59,   -1,  386,  387,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,    0,  489,  490,  370,  492,   -1,   -1,
   -1,  413,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  260,   -1,
  452,   -1,  454,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  427,  276,   -1,   -1,   40,   -1,  433,
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
   -1,   -1,   58,   -1,   -1,  262,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,  457,  458,  459,  460,  461,
  462,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  471,
   -1,   -1,   -1,  340,   -1,  262,  343,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,  359,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,  516,  517,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  396,
  260,  318,  295,  296,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  413,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,  435,   -1,
   -1,  301,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  313,   -1,  452,   -1,  454,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,  383,  327,   -1,  386,
  387,  267,   -1,  366,   -1,   -1,   -1,  370,  338,  396,
  276,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  413,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,   -1,
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
  517,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,  291,   -1,
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
   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,  411,  267,
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
  458,  459,  460,  461,  462,   -1,   -1,  260,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  301,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,  516,  517,
  313,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,  338,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,  407,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,  417,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  442,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,  489,  490,   58,  492,
   -1,   -1,   -1,   63,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,  291,   -1,   -1,   -1,
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
   -1,   -1,   -1,  273,   -1,   -1,   58,  277,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,  443,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,  406,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,  267,   -1,    0,   -1,   -1,
   -1,   -1,  354,  355,  276,   -1,   -1,  497,   -1,    0,
   -1,   -1,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   41,   -1,
   -1,   44,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   41,  323,   -1,   44,  326,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,  427,   91,   41,   -1,   -1,
   44,  433,  354,  355,   -1,   -1,   -1,   -1,   91,   -1,
   -1,  443,   -1,   -1,  366,   59,   -1,   -1,  370,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  258,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,  274,
   -1,   -1,   -1,  278,   -1,  280,   -1,  258,   -1,  284,
   -1,  274,  287,   -1,   -1,  278,   -1,  280,    0,   -1,
   -1,  284,   -1,  274,  287,  300,   -1,  278,   -1,  280,
   -1,  306,   -1,  284,  309,   -1,  287,  300,  262,   -1,
   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,  300,
   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   41,
   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,  378,  328,   -1,   -1,  331,   -1,   91,
   -1,  335,   -1,   -1,   -1,  378,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,  400,   -1,  378,   -1,   -1,
   -1,   -1,  407,   -1,   -1,  359,  411,  400,   -1,   -1,
   -1,   -1,   -1,  418,  407,   -1,   -1,   -1,  411,  400,
   -1,   -1,   -1,   -1,   -1,  418,  407,   -1,   -1,  383,
  411,  436,  386,  387,   -1,   -1,  441,  418,   -1,   -1,
   -1,   -1,  396,  436,   -1,   -1,   -1,   -1,  441,    0,
   -1,   -1,   -1,  273,   -1,  436,   -1,  277,   -1,  413,
  441,  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  466,   -1,  295,   -1,  470,  298,   -1,
   -1,  435,   -1,   -1,   -1,  466,  440,   -1,   -1,  470,
   41,   42,   43,   44,   45,   46,   47,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,  445,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,    0,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,  497,   -1,   -1,
   -1,   -1,   -1,   91,  396,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   59,   -1,   -1,  429,   -1,   -1,
   -1,  272,   -1,  435,   -1,   -1,   -1,    0,  440,  280,
   -1,   -1,   41,   -1,  285,   44,   -1,  288,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  318,   41,   -1,
  321,   44,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   59,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  359,  360,
   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   59,   -1,
   -1,   -1,  413,   -1,  272,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  280,   -1,   -1,   -1,   -1,  285,  429,   -1,
  288,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   41,  321,   -1,   44,  324,  262,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   59,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,  262,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,  318,   -1,  383,  321,   -1,  386,  387,
   -1,   -1,   -1,  328,   41,   -1,  331,   44,  396,  262,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,   59,   -1,   -1,  413,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,  359,   -1,   -1,   -1,   -1,  328,
   -1,  429,  331,   -1,   -1,  262,  335,  435,   -1,   -1,
   -1,  340,  440,   -1,  343,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,  452,  318,  454,   -1,  321,   -1,
  359,  396,   -1,   -1,   -1,  328,    0,   -1,  331,   -1,
   -1,  262,  335,   -1,   -1,   -1,   -1,  340,  413,   -1,
  343,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,  318,   -1,   -1,  321,   -1,  359,  396,    0,   -1,
  435,  328,   -1,   -1,  331,  440,   -1,   41,  335,   -1,
   44,   -1,   -1,  340,  413,   -1,  343,  452,   -1,  454,
  383,   -1,   -1,  386,  387,   59,   -1,  318,   -1,   -1,
  321,   -1,  359,  396,   -1,   -1,  435,  328,   -1,   41,
  331,  440,   44,  262,  335,   -1,   -1,   -1,   -1,  340,
  413,   -1,  343,  452,   -1,  454,  383,   59,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  396,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,  413,   -1,   -1,  452,
   -1,  454,  383,   -1,   -1,  386,  387,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,  396,   -1,   -1,  435,  328,
   -1,    0,  331,  440,   -1,  262,  335,   -1,   -1,   -1,
   -1,  340,  413,   41,  343,  452,   44,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   59,   -1,   -1,  435,    0,   -1,   -1,   -1,  440,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,  383,   -1,   -1,  386,  387,   -1,
   59,  318,   -1,   -1,  321,   -1,   -1,  396,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   41,   -1,  335,   44,
   -1,   -1,   -1,  340,  413,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   -1,  452,   -1,  454,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  435,   -1,
   -1,   -1,   -1,  440,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,  452,  340,  454,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,  359,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,  413,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
  318,  435,   -1,  321,   -1,   -1,  440,  262,   -1,   -1,
  328,  413,   -1,  331,   -1,   -1,   -1,  335,  452,   -1,
  454,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,  318,
   -1,   -1,  321,  435,   -1,   -1,   -1,   -1,  440,  328,
   -1,  359,  331,   -1,   -1,  271,  335,   -1,   -1,   -1,
  452,  340,  454,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  383,  321,   -1,  386,  387,
  359,   -1,   -1,  328,   -1,  301,  331,   -1,  396,   -1,
  335,   -1,  308,   -1,   -1,  340,  312,  313,  343,  315,
   -1,   -1,  318,   -1,  383,  413,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,  359,   -1,  332,  396,   -1,   -1,
   -1,   -1,  338,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,  413,   -1,  271,   -1,  383,   -1,
   -1,  386,  387,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  301,   -1,  413,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,  312,  313,   -1,
  315,   -1,  398,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,  332,   -1,   -1,
   -1,  417,   -1,  338,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,  398,   -1,   -1,   -1,  284,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  295,   -1,
   -1,  298,  417,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  432,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,
   -1,   -1,  339,   -1,   -1,  460,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,  373,  492,  375,  295,
   -1,   -1,  298,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  339,   -1,   -1,  423,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
  406,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
  497,   -1,  499,  500,  501,  502,  503,  423,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  460,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
  483,  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,   -1,  499,  500,  501,  502,
  503,  504,  505,  506,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,
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
"view_table : derived_table",
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
"table_reference : derived_table",
"derived_table : '(' union_expr ')' as_noise correlation_name derived_column_list",
"correlation_name : symbol_table_alias_name",
"correlation_name :",
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
case 675:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 676:
{ yyval = yyvsp[-1]; }
break;
case 677:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 678:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 679:
{ yyval = make_list (yyvsp[-1]); }
break;
case 680:
{ yyval = NULL; }
break;
case 682:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 686:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 687:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 688:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 689:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 690:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 696:
{ yyval = make_list (yyvsp[0]); }
break;
case 697:
{ yyval = 0; }
break;
case 699:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 701:
{ yyval = yyvsp[0]; }
break;
case 702:
{ yyval = 0; }
break;
case 703:
{ yyval = yyvsp[0]; }
break;
case 704:
{ yyval = 0; }
break;
case 705:
{ yyval = yyvsp[0]; }
break;
case 706:
{ yyval = 0; }
break;
case 707:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 708:
{ yyval = 0; }
break;
case 709:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 710:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 711:
{ yyval = 0; }
break;
case 712:
{ yyval = 0; }
break;
case 714:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 720:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 721:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 725:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 731:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 735:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 736:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 738:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 739:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 742:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 745:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 749:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 750:
{ yyval = yyvsp[0]; }
break;
case 752:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 755:
{ yyval = NULL; }
break;
case 756:
{ yyval = make_list (yyvsp[-1]); }
break;
case 758:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 760:
{ yyval = NULL; }
break;
case 761:
{ yyval = make_list (yyvsp[-1]); }
break;
case 763:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 767:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 778:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = yyvsp[-1]; }
break;
case 783:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 828:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 830:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 832:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 834:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 836:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 841:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 842:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 843:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_list (yyvsp[-1]); }
break;
case 846:
{ yyval = yyvsp[-1]; }
break;
case 847:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 848:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 858:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 859:
{ yyval = yyvsp[0]; }
break;
case 860:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 861:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 862:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 863:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 864:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 865:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 866:
{ yyval = yyvsp[-1]; }
break;
case 867:
{ yyval = yyvsp[-1]; }
break;
case 871:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 872:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 873:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 874:
{ yyval = yyvsp[0]; }
break;
case 875:
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
case 876:
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
case 877:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 878:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 880:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 882:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 884:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 885:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 886:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 887:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 889:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 890:
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
case 891:
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
case 892:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 893:
{ yyval = make_parameter (); }
break;
case 894:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 895:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 896:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 897:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 898:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 899:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 900:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 901:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 902:
{ yyval = yyvsp[0]; }
break;
case 903:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 905:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 906:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 907:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 908:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 909:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 911:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 912:
{ yyval = yyvsp[0];}
break;
case 917:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 918:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 919:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 920:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 921:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 922:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 923:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 924:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 926:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 928:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 929:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 930:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 931:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 932:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 933:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 934:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 935:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 938:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 939:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 942:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 943:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 944:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 945:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 946:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 947:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 949:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 954:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 955:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 956:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 957:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 958:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 959:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 960:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 963:
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
