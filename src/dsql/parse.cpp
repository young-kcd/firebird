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
  194,  195,  207,  196,  208,  201,  201,  201,  202,  202,
  202,  205,  205,  205,  205,  197,  204,  204,  200,  200,
  200,  206,  206,  187,  187,  214,  215,  215,  216,  216,
  216,  216,   11,   71,   80,  218,  221,  221,  221,  222,
  225,  229,  229,  230,  230,  230,  231,  231,  147,  220,
   87,   89,  219,  219,   70,   83,  235,  235,  235,  236,
  239,  239,  240,  240,  240,  240,  240,  240,  240,  240,
  240,  240,  240,  240,  240,  240,  240,  237,  237,  238,
    2,  242,  242,  242,  242,  242,  242,  242,  250,  251,
  248,  248,  252,  252,  252,  252,  252,  252,  252,  243,
  243,  253,  253,  253,  253,  253,  253,  253,  247,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  255,  255,  257,  257,  256,  254,  254,  254,
  249,  249,  245,  246,  246,  260,  260,  260,  260,  260,
  260,  260,  260,  261,  261,  244,  262,  262,  263,  263,
    8,  264,  264,  264,  264,  264,  264,  264,  264,  264,
  264,  264,  264,  101,  101,  144,  144,  267,  267,  270,
  270,  272,  272,   53,   53,  269,  269,  269,  269,  269,
  269,  269,  269,  269,  277,  277,  268,  268,  268,  268,
  278,  278,   61,   61,   61,   55,   55,  274,  274,  274,
  271,  271,  271,  282,  282,  282,  281,  281,  280,  280,
  280,  275,  275,  283,  283,  283,  284,  284,  276,  276,
  276,  276,  285,  285,   18,   18,   18,  287,  287,  287,
   16,   16,   16,  289,  290,  293,  293,  291,  295,  295,
    4,   15,  294,  294,  296,  296,  297,  297,  286,  298,
  298,  299,  299,  300,  300,  300,  300,  301,  301,  302,
  302,  303,  303,  305,  305,  305,  306,  306,  306,  307,
  307,  307,  304,  309,  309,  309,  310,  310,  308,  308,
  311,  313,  313,  312,  312,  288,   17,  314,  314,  314,
  315,  315,  319,  319,  320,   65,   65,   65,  322,  322,
  321,  321,  316,  316,  325,  325,  317,  317,  318,  329,
  330,  331,  323,  324,  326,  326,  326,  326,  332,  332,
  332,  333,  333,  333,  223,  223,  224,  224,  336,  336,
  337,  337,  339,  339,  327,  341,  341,  342,  342,  342,
  233,  345,  345,  346,  346,  348,  348,  343,  343,  344,
  344,  349,  349,  210,  210,  350,  350,  232,  232,   31,
  234,  234,  234,  234,  234,  234,  234,  234,  227,  227,
  351,  351,  352,  228,  228,  226,  226,  328,  328,  353,
  354,  354,  354,  354,  354,  355,  355,  356,  356,  357,
  357,  358,  358,  358,  359,  359,   10,   10,  361,  361,
    7,    7,  362,  363,  364,   19,   19,  365,  366,  367,
  367,  188,  338,  338,    3,    3,  369,  369,  369,  371,
  371,   61,  370,  370,  372,  372,   34,   34,   85,   84,
   84,  360,  360,  373,  374,  374,  211,  211,  211,  146,
  368,  368,  111,  111,  111,  111,  111,  378,  378,  378,
  378,  378,  378,  377,  377,  377,  380,  380,  380,  380,
  380,  380,  380,  380,  380,  380,  381,  381,  381,  381,
  381,  381,  381,  381,  386,  386,  386,  386,  386,  386,
  386,  386,  386,  386,  386,  386,  386,  386,  386,  386,
  392,  392,  382,  382,  383,  383,  383,  383,  384,  384,
  388,  388,  389,  389,  389,  389,  387,  390,  385,  385,
  376,  376,  376,  379,  379,  379,  393,  393,  394,  391,
  396,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,  156,  156,  156,
  397,  395,  395,  151,  151,  403,  403,  403,  403,  403,
  399,  399,  399,  399,  399,  334,  152,  152,  153,  154,
  154,  154,  154,  154,   51,   51,   20,   20,  241,  404,
   54,  123,  273,  273,   95,  398,  398,  398,  398,  405,
  405,  405,  405,  405,  405,  405,  405,  405,  405,  405,
  406,  407,  409,  408,  408,  411,  412,  412,  402,  402,
  400,  401,  401,  413,  413,  414,  414,  415,  415,  418,
  418,  416,  416,  421,  421,  420,  417,  419,  410,  410,
  410,  410,  410,  410,  410,  410,  335,  335,  155,   48,
  279,  122,  148,  375,  165,  213,  266,   63,   60,  217,
  112,   66,  340,  212,   35,   41,  347,  265,   39,   42,
  209,   40,  292,  259,  259,  422,  422,  422,  422,  422,
  422,  422,  422,  422,  422,  422,  422,  422,  422,  422,
  422,
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
    4,    2,    6,    4,    1,    1,    8,    9,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    7,    2,    0,    2,    2,
    3,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    4,    7,    7,    1,    1,    3,    4,    7,
    2,    1,    3,    1,    1,    1,    6,    3,    0,    0,
    0,    0,    3,    0,    9,    9,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    5,    5,    5,    5,    5,    5,    2,    0,    4,
    2,    3,    3,    2,    2,    3,    3,    2,    3,    6,
    1,    2,    4,    3,    2,    2,    2,    2,    3,    1,
    3,    3,    3,    2,    2,    4,    4,    5,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    0,
    2,    2,    0,    1,    2,    2,    2,    2,    2,    4,
    3,    2,    2,    1,    3,    7,    1,    0,    1,    0,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    3,
    2,    2,    2,    1,    1,    1,    1,    4,    5,    1,
    3,    1,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    4,    4,    6,    5,
    3,    0,    2,    2,    0,    3,    0,    4,    1,    5,
    4,    1,    4,    1,    2,    2,    1,    1,    1,    2,
    2,    2,    2,    0,    3,    5,    1,    1,    2,    3,
    1,    2,    3,    0,    1,    1,    1,    5,    5,    6,
    1,    1,    1,    2,    4,    1,    0,    5,    1,    0,
    3,    2,    1,    0,    2,    0,    1,    0,    3,    1,
    0,    1,    2,    1,    1,    1,    1,    2,    2,    1,
    2,    3,    1,    1,    3,    3,    1,    2,    3,    1,
    2,    0,    2,    1,    1,    0,    1,    1,    1,    3,
    2,    3,    0,    1,    3,    4,    4,    1,    3,    4,
    3,    0,    1,    3,    3,    1,    1,    0,    1,    1,
    4,    0,    3,    0,    2,    0,    2,    0,    9,   10,
    0,    0,    0,    0,    3,    2,    1,    0,    3,    5,
    3,    2,    5,    2,    1,    1,    1,    1,    1,    3,
    1,    3,    1,    0,    2,    1,    3,    1,    1,    1,
    6,    1,    0,    3,    0,    1,    3,    6,    3,    4,
    2,    3,    0,    1,    3,    1,    1,    1,    2,    1,
    1,    1,    2,    1,    2,    1,    2,    0,    3,    0,
    1,    3,    1,    2,    0,    2,    0,    2,    0,    4,
    1,    2,    1,    1,    0,    1,    3,    2,    1,    1,
    2,    1,    4,    2,    1,    3,    8,    5,    1,    3,
    1,    1,    4,    4,    4,    1,    1,    5,    5,    1,
    3,    3,    1,    1,    7,    7,    5,    3,    0,    1,
    1,    1,    2,    0,    1,    1,    1,    0,    3,    1,
    3,    1,    0,    3,    1,    3,    1,    3,    3,    1,
    1,    3,    1,    2,    1,    3,    3,    1,    2,    3,
    3,    3,    3,    1,    3,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    1,    1,    5,    6,    3,    4,    5,    6,    3,    4,
    3,    4,    3,    4,    4,    5,    4,    4,    3,    4,
    1,    1,    1,    1,    1,    1,    1,    3,    3,   10,
   10,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    3,    3,    3,    3,    3,    3,    3,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    1,    1,
    4,    1,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    2,    1,    2,    1,    1,
    1,    1,    1,    2,    1,    1,    1,    1,    1,    4,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    5,
    6,    1,    6,    1,    4,    7,    2,    0,    4,    3,
    6,    1,    1,    6,    6,    1,    1,    4,    6,    4,
    5,    3,    5,    4,    5,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    0,    1,    1,
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
   12,   13,   14,   15,   16,   17,   18,   19,   20,  555,
  556,  557,  561,  562,  563,    0,  618,  731,  732,  736,
  737,  463,    0,    0,    0,    0,    0,    0,  401,  573,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  909,    0,   21,  907,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  481,    0,    0,   37,   34,
   36,    0,   35,    0,  994,  996,  998,  997, 1003,  999,
 1006, 1004, 1000, 1002, 1005, 1001, 1007, 1008, 1009, 1010,
 1011,    0,    0,    0,    0,   32,   68,   66,  986,  995,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  647,    0,    0,    0,  688,    0,  988,
    0,    0,    0,    0,  107,  993,  564,    0,    2,    0,
    0,    0,    0,  431,  432,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,    0,  429,  430,    0,  978,
    0,  408,  982,    0,  405,  985,    0,  690,    0,  404,
  989,    0,  571,  905,    0,  150,  103,    0,  104,    0,
  218,  974,  770,    0,  102,  145,  981,    0,    0,   98,
    0,  105,  911,    0,   99,    0,  100,    0,  101,  992,
  146,  106,  626,  627,    0,  910,  908,    0,   74,    0,
  979,    0,  977,  489,  482,    0,  488,  493,  483,  484,
  491,  485,  486,  487,  492,    0,   30,    0,   39,  757,
   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  915,    0,  896,  641,
  641,  642,  652,  654,  967,  655,    0,  656,  646,  642,
    0,    0,    0,    0,    0,    0,    0,  590,  579,    0,
  582,  584,  585,  586,  587,  593,  594,    0,  987,  689,
    0,  108,    0,  109,    0,  110,    0,    0,    0,  619,
    0,    0,    0,    0,    0,    0,    0,    0,  464,    0,
    0,  371,    0,  221,    0,  411,  402,  461,  462,    0,
    0,    0,    0,    0,    0,  420,    0,  377,  378,    0,
  577,  575,  906,    0,    0,    0,    0,    0,    0,    0,
  153,  132,    0,   96,    0,    0,  111,    0,  121,  120,
    0,    0,    0,    0,    0,    0,  970,   75,    0,    0,
    0,  733,  734,  490,    0,    0,    0,    0,  874,  902,
    0,  969,    0,  876,    0,    0,  903,    0,  897,  888,
  887,  886,    0,    0,    0,    0,  878,  879,  880,  889,
  890,  898,  899,    0,  900,  901,    0,    0,    0,  904,
    0,    0,    0,    0,    0,  892,    0,  767,  871,  872,
  873,  686,  877,  353,  857,    0,    0,    0,  856,    0,
  684,  853,  854,  855,  858,  859,  860,  891,  916,  917,
  918,  919,  932,  934,  942,  943,  946,  947,    0,  760,
    0,    0,   27,    0,    0,    0,   71,   69,  990,   67,
   33,    0,    0,    0,  762,    0,   52,    0,    0,    0,
    0,    0,  569,    0,    0,    0,  649,  651,    0,  658,
    0,  744,    0,    0,  659,    0,  645,    0,  616,    0,
  591,    0,  588,    0,  589,  614,    0,  609,    0,    0,
  583,  771,  740,    0,    0,    0,    0,    0,    0,  566,
  565,    0,    0,  623,  620,    0,    0,  617,  186,    0,
    0,    0,  129,  469,  173,  174,    0,  472,  467,    0,
  473,    0,    0,    0,    0,  474,    0,  465,    0,    0,
  415,  417,  416,    0,  418,    0,  412,  219,    0,    0,
  272,    0,    0,    0,    0,  424,  425,    0,  454,    0,
    0,    0,    0,    0,    0,    0,  382,  381,  477,    0,
    0,  149,    0,    0,  158,    0,  157,    0,    0,  164,
  147,    0,  162,  165,  166,  154,    0,    0,  512,  547,
  515,    0,  548,    0,    0,  516,    0,    0,    0,  539,
  551,  511,  534,  513,  514,  509,  496,  371,  494,  495,
  497,    0,    0,  506,  507,  508,  510,    0,    0,    0,
    0,    0,  112,    0,  113,    0,  124,    0,    0,  202,
    0,    0,  369,    0,  221,    0,    0,   83,    0,    0,
  752,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  773,  775,  784,  787,  788,  789,  790,  791,
  792,  793,  794,  795,  796,    0,    0,    0,  893,    0,
    0,    0,    0,    0,    0,  894,  895,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  325,  991,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  759,   28,    0,    0,   44,   72,    0,
    0,   26,    0,  765,    0,    0,    0,  728,    0,    0,
    0,    0,    0,  568,    0,  641,    0,    0,    0,    0,
  663,    0,  559,    0,  913,  558,    0,  592,    0,  600,
  596,  595,    0,    0,    0,  611,  599,    0,    0,  738,
  739,    0,    0,    0,  369,    0,    0,    0,  633,  637,
    0,    0,    0,  130,    0,  471,    0,    0,    0,    0,
    0,  371,  414,    0,  371,  372,  419,  504,    0,  274,
    0,  268,  270,  369,  241,  975,    0,    0,  371,    0,
    0,  210,  369,    0,    0,    0,    0,  246,  240,  243,
  244,  245,  423,  459,  458,  422,  421,    0,    0,    0,
    0,    0,    0,  380,    0,  371,    0,  155,  159,    0,
    0,  163,    0,    0,  535,  536,  552,    0,  549,    0,
    0,  542,  540,  541,    0,    0,    0,    0,  505,    0,
    0,    0,    0,  543,    0,    0,  369,    0,  201,    0,
  204,  206,  207,    0,    0,    0,    0,    0,  221,   78,
    0,    0,   77,  523,  971,  524,    0,    0,    0,    0,
  774,  786,    0,    0,  844,  845,  846,    0,  778,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  959,  960,  961,  962,  963,  964,  965,  966,
    0,    0,    0,    0,    0,    0,  952,    0,    0,    0,
    0,  869,  327,  870,  940,    0,    0,  865,  973,    0,
    0,    0,    0,    0,  685,    0,  875,  769,  768,  761,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   61,
   63,    0,    0,   70,    0,    0,    0,  764,    0,    0,
    0,    0,    0,    0,    0,    0,  650,  653,    0,    0,
  670,    0,    0,  668,  669,    0,    0,  660,  983,  662,
  560,  914,  601,  610,  605,  604,    0,  615,  772,  741,
  742,  369,    0,    0,  643,  625,  624,    0,  182,  176,
  183,    0,  179,  470,    0,    0,  178,    0,  189,  170,
  169,  912,  172,  171,  475,    0,    0,    0,  413,  273,
    0,  212,  371,  214,    0,  372,  209,  215,    0,    0,
    0,    0,  426,  427,  371,  369,  372,    0,    0,    0,
  398,    0,  197,  198,  156,    0,  168,    0,    0,    0,
    0,    0,  550,    0,    0,  371,  372,    0,  500,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  199,    0,    0,    0,    0,  357,    0,   82,
    0,    0,    0,    0,   87,   84,    0,    0,    0,  735,
  976,    0,    0,  779,    0,    0,  785,    0,    0,    0,
    0,    0,  822,  821,    0,    0,    0,  829,  847,  839,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  777,    0,    0,    0,  221,    0,  920,    0,    0,  935,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  948,    0,  958,    0,    0,    0,    0,    0,
    0,  939,  881,   65,   57,   58,   64,   59,   60,    0,
    0,   22,    0,    0,    0,   42,    0,    0,    0,  746,
  766,    0,  729,    0,  745,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  691,    0,    0,    0,
    0,    0,  607,  608,  612,    0,  200,    0,    0,    0,
    0,    0,    0,    0,    0,  190,    0,    0,    0,  409,
  224,  225,  226,  227,  228,  229,  884,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  455,
  456,  428,    0,    0,    0,    0,    0,    0,  371,  479,
  372,  167,  972,    0,    0,  518,    0,  517,  553,    0,
  545,    0,    0,    0,  498,    0,  526,    0,    0,  528,
  531,  533,  369,    0,    0,    0,  119,    0,  205,  371,
    0,    0,  370,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  837,  838,  781,    0,  783,
    0,    0,    0,    0,    0,    0,    0,  840,    0,    0,
    0,    0,    0,    0,    0,  830,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  926,  925,    0,    0,  922,  921,    0,  930,  929,  928,
  927,  924,  923,    0,    0,  954,    0,    0,    0,  953,
    0,    0,    0,    0,    0,    0,   56,   62,   54,   55,
   23,  750,  751,    0,  748,  755,  756,  753,    0,    0,
  727,    0,    0,    0,  679,    0,    0,    0,  697,  693,
  695,    0,    0,    0,    0,    0,  370,  629,  630,  644,
    0,  184,  180,    0,    0,  195,  191,    0,  372,  885,
  280,    0,    0,  372,  278,    0,    0,    0,  232,    0,
    0,  248,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  476,  520,    0,  521,    0,  134,    0,    0,    0,
  138,  140,  141,  142,  501,  503,  499,  530,    0,    0,
  372,    0,    0,    0,    0,  354,    0,  358,  371,   88,
    0,    0,   90,    0,    0,    0,    0,    0,    0,  849,
  848,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
  941,  931,  933,    0,    0,  950,  949,    0,  955,  944,
  945,    0,   40,    0,    0,  730,    0,  682,  680,    0,
    0,    0,  701,    0,    0,  639,  372,  355,  631,    0,
  175,  192,  372,  144,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  301,  302,  304,  305,  306,  309,    0,    0,    0,  267,
    0,    0,  208,  233,    0,    0,    0,    0,  234,  237,
    0,    0,  372,  396,  397,  393,  392,  395,  394,    0,
  519,  546,  143,  131,  139,    0,  371,  264,  126,  372,
    0,    0,  373,  359,    0,    0,   91,   92,   79,    0,
  808,  816,    0,  809,  817,  812,  820,    0,    0,  810,
  818,  811,  819,  805,  813,  806,  814,  807,  815,    0,
  936,  951,    0,  747,    0,    0,  672,    0,    0,  711,
  713,    0,  708,    0,  265,  185,  410,    0,  312,    0,
    0,  315,  316,    0,  308,    0,    0,  307,  339,  340,
    0,  984,  293,  300,    0,  303,  286,    0,  291,    0,
  344,  292,    0,    0,  337,  211,  216,  235,    0,    0,
  238,    0,  250,  252,  400,  372,    0,  375,    0,    0,
    0,  364,  365,  366,    0,    0,   76,   94,    0,    0,
    0,    0,  671,    0,  702,  712,    0,    0,  283,    0,
  310,    0,    0,    0,    0,    0,  341,    0,  332,  333,
    0,  352,    0,    0,    0,    0,  347,  345,    0,    0,
    0,  239,    0,    0,    0,    0,    0,  266,  372,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  676,    0,
    0,  719,    0,    0,    0,  282,  279,  311,    0,    0,
  314,    0,  322,  324,    0,    0,  351,  350,  980,  349,
    0,    0,    0,    0,    0,    0,    0,    0,  249,    0,
  256,    0,  255,  376,  368,    0,    0,  360,  118,    0,
    0,    0,    0,  674,  721,  710,    0,    0,  722,    0,
  718,    0,    0,    0,    0,  335,  334,  346,  348,    0,
    0,    0,  236,    0,    0,  260,  259,  258,    0,    0,
  851,  640,  677,  717,    0,  724,    0,    0,  313,  319,
    0,    0,    0,    0,    0,  263,  261,  262,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  336,    0,    0,
    0,  723,  330,  320,    0,  342,  317,  850,  726,  318,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1555,   28,   29, 1556,
   31,   32,   33,   34,   35,   36, 1557,   38, 1558,  651,
  122,  462,  967, 1202,  123,  717,  968,  124,  466,  722,
  148,  125,  126,  259,  990,  138,  969,  970,  199,  228,
  127,  971,  128,  468,   82,  239,  378,  425,  646, 1103,
  426,  872,  617,  221,  849,  647,  648,  649, 1105,  240,
 1392,   74,  189,   75,  235, 1851, 1315,  220,  225,  227,
  229,  215,  207,  209,  222,  232,  155,  312,  314,  316,
  367,  633,  635,  459,  260,  798,  550,  661, 1039,  371,
  638, 1091, 1307,  583,  745, 1065,  532,  533,  565,  363,
  618, 1077, 1459, 1583, 1250, 1460, 1461, 1462, 1463, 1464,
  662,  216,  208,  359,  591,  360,  361,  592,  593,  594,
  595, 1282, 1033,  535,  536, 1022, 1020,  537, 1027, 1021,
 1240, 1422, 1028, 1029, 1245,  373,  860,  861,  862,  863,
  800, 1046, 1437,  619,  802,  428,  865,  948,  559, 1364,
 1251,  429,  430,  431,  432,  433, 1438, 1439,  568, 1589,
 1745, 1268,  808,  809,  795,  810,  811,  812, 1746, 1747,
 1827,  341,  563, 1259, 1559,  195,  560,  561, 1260, 1261,
 1432, 1720, 1560, 1561, 1562, 1563, 1680, 1564, 1565, 1566,
 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576,
  434, 1813, 1664, 1577, 1728, 1844, 1726,  435,  701,  436,
  437, 1578, 1110, 1681, 1736, 1737, 1778, 1096, 1313, 1476,
 1097, 1098,  287,  493, 1612,  382, 1232, 1415, 1700, 1701,
 1702, 1703,  991, 1230,  350,  579,  826, 1280,  580,  824,
   79,   59,  345,  200,  163,  328,  186,  335,  192,  786,
  783,  336,  346,  816,  347,  575, 1057,  187,  438,  329,
  547,  581, 1281,   96,  198,  244,  620,  621,  788, 1078,
  789, 1079, 1080,  624,  625,  626,  627, 1071,  876,  628,
  629,  630,  842,  631,  839,   40,   41,   42,   43,   44,
   45,  157,  521,   61,  484,  203,  352,  299,  300,  301,
  302,  303,  304,  305,  306,  307,  751,  507, 1007, 1235,
  508,  509,  756,   46,  162,  322,  528,   47,  523,  524,
 1016, 1420, 1239, 1539,  769,  142,  739, 1536,  728,  487,
  289,  143,  144,  439,  288,  494,  495,  496,  742, 1000,
  992,  993,  994,  995, 1646, 1713,  440, 1760, 1224, 1185,
 1532, 1533, 1762, 1654, 1763, 1764, 1765, 1811, 1852,  474,
 1214,   48,   49,  383,   50,   51,  515,  516,  976, 1210,
 1394, 1398,  475,  725,  213,  663,  664,  890,  891,  665,
  666,  667,  668,  669,  670,  671,  672,  673,  674,  675,
 1336, 1126, 1128, 1129,  947,  700,  442,  443,  444,  445,
  446,  447,  448,  237,  449,  450,  451,  452,  453,  931,
  454, 1515,  455,  456,  457,  458,  691,  935, 1186, 1181,
  692,  130,
};
static short yysindex[] = {                                   6258,
 1346, -106, 3964,  130, 1255,   87, 3616,   75, 4669,   73,
  367, 9342, -106, 1256,  458, 5259, 1098, 5259,  190,    0,
  729,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  561,    0,    0,    0,    0,
    0,    0, 9565, 5259, 5259, 5259, 5259, 5259,    0,    0,
  436, 1293, 5259, 5259, 5259,  663, 5259,  492, 5259, 5259,
    0, 5259, 5259,    0,  937,    0,  545,    0,    0,  688,
 5259,    0, 5259, 5259, 5259,  764, 5259, 5259, 5259, 5259,
  492, 5259, 5259, 5259, 5259,    0, 5259,  618,    0,    0,
    0, 1060,    0, 1060,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  757,  770,  141, 1212,    0,    0,    0,    0,    0,
 5259, 5259, 5259,  810,  911,  967,  103, 1496,  923,  157,
  217,  699,  881,    0, 5259, 1053,  901,    0,  989,    0,
 5259, 5259, 5259, 5259,    0,    0,    0, 5259,    0, 1161,
  432, 1101,  826,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  993,    0,    0, 1293,    0,
   61,    0,    0, 1508,    0,    0, 1331,    0,   77,    0,
    0, 1073,    0,    0, 1147,    0,    0, 1064,    0, 1298,
    0,    0,    0, 1293,    0,    0,    0,  636, 1508,    0,
  109,    0,    0, 1294,    0, 1299,    0, 1060,    0,    0,
    0,    0,    0,    0, 1287,    0,    0, 1183,    0, 1302,
    0, 1203,    0,    0,    0, 1183,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 9309,    0, 5259,    0,    0,
    0, 4962, 1276, 5811, 5259,  932, 1340, 1649, 1395, 1403,
 4962, 1339, 5811, 1350, 1362, 1262,    0,11000,    0,    0,
    0,    0,    0,    0,    0,    0, 8581,    0,    0,    0,
 1341, 5259, 1418, 1333,  -25, 5259, 1351,    0,    0,  901,
    0,    0,    0,    0,    0,    0,    0, 5259,    0,    0,
 1508,    0, 1294,    0, 1060,    0, 1409,11000, 1383,    0,
 1360, 1367,  558, 1315,  362, 1317,  679,  826,    0, 1527,
  583,    0, 5259,    0,  993,    0,    0,    0,    0, 5259,
 1466, 3089, 1416, 4787, 1842,    0, 9565,    0,    0,  858,
    0,    0,    0, 1827, 1827, 1293, 1522, 1293, 1366, 1064,
    0,    0,10862,    0, 5259, 5259,    0, 1466,    0,    0,
 1610,  471, 1864, 5259, 1653, 5259,    0,    0, 1870,  215,
 5875,    0,    0,    0, 1872, 1878, 1879, 1464,    0,    0,
 1889,    0, 1890,    0, 1894, 1895,    0, 1899,    0,    0,
    0,    0,    0, 1900, 1480, 1487,    0,    0,    0,    0,
    0,    0,    0, 1909,    0,    0, 9406, 1915, 1916,    0,
11000,11000, 8679, 5259, 1923,    0, 1997,    0,    0,    0,
    0,    0,    0,    0,    0, 1921, 1881,    0,    0, 1928,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1718,    0,
 5259, 1546,    0, 5259, 5259,  142,    0,    0,    0,    0,
    0, 5259, 5259,  627,    0, 5259,    0, 1667, 5259, 1962,
 4962, 1605,    0, 5259, 9645, 1979,    0,    0,11000,    0,
 1997,    0, 1695, 1973,    0, 1755,    0,  126,    0,  599,
    0,  -50,    0,  -50,    0,    0, 1985,    0,  358, 1607,
    0,    0,    0, 1991,  140, 1987, 1466, 2014, 1796,    0,
    0, 1971, 2017,    0,    0, 1690, 1571,    0,    0,  238,
 1730, 1733,    0,    0,    0,    0, 1293,    0,    0, 1740,
    0, 1827, 1827, 1827, 1827,    0, 2036,    0, 1811, 1812,
    0,    0,    0, 1792,    0,10862,    0,    0,10862, 1719,
    0, 1508, 1830, 5259,12678,    0,    0,  594,    0, 5259,
  758, 1331, 1698, 1660, 1628,    0,    0,    0,    0,  -17,
 1700,    0, 1652,  492,    0, 1293,    0, 1823, 1756,    0,
    0, 1366,    0,    0,    0,    0,  206, 1659,    0,    0,
    0, 1662,    0, 1708, 2071,    0, 1768, 2079,  468,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2029,  214,    0,    0,    0,    0,  178, 2086, 2088,
 2079, 1508,    0, 1813,    0, 1868,    0, 1293, 1293,    0,
 3089,   77,    0, 1750,    0, 1723, 2091,    0, 6808, 2474,
    0, 1747, 1759, 2102, 6478, 2103,    0,    0,    0, 5778,
 8607,   40,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  708, 9884,  124,    0, 5259,
11000, 1145, 1278, 1356, 1844,    0,    0,11000, 6708, 1997,
 1693,  535,11000, 9884,  811,  811, 1256, 2004, 1737, 2111,
    0,    0, 9964, 5259,11000,11000,11000,11000,11000, 9884,
11000, 2120, 5259,    0,    0, 7195, 1725,    0,    0, 1691,
 5811,    0, 1837,    0, 1778, 1256, 2123,    0, 1837, 7195,
 1846, 1850, 5259,    0, 2004,    0, 2126, 2051, 1720, 9884,
    0, 5259,    0,  742,    0,    0,  -31,    0, 1726,    0,
    0,    0, 5259,  975, 5259,    0,    0, 5259, 5259,    0,
    0, 9884, 1913, 3089,    0, 1677,11000, 5259,    0,    0,
 1293, 1293, 1293,    0,  421,    0, 1652, 1652, 1722,  492,
  679,    0,    0, 2141,    0,    0,    0,    0, 1905,    0,
 5259,    0,    0,    0,    0,    0, 1910, 2143,    0, 1893,
 1906,    0,    0, 1855, 1856, 1060, 1929,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  492, 5259,12779,
 1815, 1825, 1831,    0,  492,    0, 1449,    0,    0, 1791,
 1293,    0,   94, 1797,    0,    0,    0,  492,    0, 2071,
  159,    0,    0,    0, 1919,  159, 1810,  159,    0, 2186,
  492,  492,  492,    0, 1466, 5259,    0, 1873,    0, 1788,
    0,    0,    0,  858, 1814, 5259, 1816,  149,    0,    0,
 2192, 1959,    0,    0,    0,    0,  215, 5259, 1820, 6956,
    0,    0, 1820, 6478,    0,    0,    0, 7006,    0,  272,
  111,11000,11000, 7053, 2200,  512, 7311,11000, 7463, 1127,
 7572, 7831,10203, 7983, 8092, 8351, 6708, 6708,11000,11000,
 1977,11000, 2201,11000, 2199, 2253,11000,11000,11000,11000,
11000,11000,    0,    0,    0,    0,    0,    0,    0,    0,
 1924, 1510,  452,11000,  890, 9884,    0, 6708, 1481, 2202,
  699,    0,    0,    0,    0, 1997, 1920,    0,    0, 1967,
  811,  811, 1261, 1261,    0,  298,    0,    0,    0,    0,
 5259, 5259, 5259, 5259, 5259, 5259,  175, 2205,    0,    0,
    0, 7195, 1869,    0,  -43, 1883, 5259,    0,  699, 9884,
 1883, 2209, 2205, 7195, 7159, 1935,    0,    0, 1390, 2220,
    0, 2218, 1151,    0,    0, 6708, 1938,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  274,    0,    0,    0,
    0,    0, 1945, 1814,    0,    0,    0, 2223,    0,    0,
    0, 1957,    0,    0, 1827, 2003,    0,  421,    0,    0,
    0,    0,    0,    0,    0, 2240, 6708, 3015,    0,    0,
 1989,    0,    0,    0, 3015,    0,    0,    0, 1060, 1060,
 1785, 2257,    0,    0,    0,    0,    0,  525,   81, 1115,
    0, 2035,    0,    0,    0, 5259,    0,  130, 1958, 1880,
 1905, 2270,    0, 1652, 1964,    0,    0, 1268,    0, 2255,
 5259, 1353,  492, 2273, 2284, 2290, 2068,   77, 1989, 1827,
 1733, 3089,    0, 1700,  699, 1884, 1901,    0, 1613,    0,
  492, 1941, 2030, 6808,    0,    0,  492, 1848, 2039,    0,
    0, 2304, 2309,    0, 6708, 6708,    0, 6708, 6708, 2163,
 1997, 2311,    0,    0, 1997, 2312,10442,    0,    0,    0,
 2006, 2319, 1997, 2321, 1532, 2330, 1997, 2332,11000,11000,
 2200,11000,10522, 2341, 1997, 2342, 2346, 1997, 2347,11000,
 1997, 2349, 1997, 2351, 2352, 1997, 2353, 2354, 1997, 2355,
    0, 2104, 2326, 2333,    0, 2376,    0, 2387,11000,    0,
 2427, 2448, 2473, 2483, 2528, 2606,11000,11000, 9884, 1997,
 1968, 9884,    0,11000,    0, 2056,  470,11000, 9884,11000,
11000,    0,    0,    0,    0,    0,    0,    0,    0, 2070,
 7195,    0, 7195,  175, 2205,    0,  205,  205,   71,    0,
    0, 8581,    0, 1984,    0, 2209, 2205, 2361, 7159,  127,
 1151, 2358, 9884, 1755, 2051, 2019,    0, 2020, 2021, 2069,
 2139, 2083,    0,    0,    0, 1989,    0, 1884, -142, 1993,
 1293, 2032, 1652, 2033, 1652,    0, 6708,  448, 1457,    0,
    0,    0,    0,    0,    0,    0,    0, 1974, 2155, 1989,
    0,11000,    0, 2144, 2026, 1785,  937,    0, 6708,    0,
    0,    0, 2049, 2050, 2052, 2058, 2059, 2060,    0,    0,
    0,    0,    0, 2396,  130,    0, 1722,    0,    0,  159,
    0, 3015, 2076,  159,    0,  159,    0, 1905, 2406,    0,
    0,    0,    0,  858, 2155, 1652,    0, 1733,    0,    0,
 8581, 2178,    0,  532,    0, 2223,    0, 2417,    0, 2422,
 1293,  -80, 2423,    0, 1293,    0,    0,    0, 2104,    0,
 2104,11000, 2048, 2048, 1256, 2430, 1986,    0, 2048, 2048,
11000, 2048, 2048, 2876, 1997,    0, 1651,11000, 1997, 2048,
 2048, 2048, 2048, 1997, 2048, 2048, 2048, 2048, 2048, 2048,
    0,    0,10862, 2432,    0,    0, 2837,    0,    0,    0,
    0,    0,    0, 2855, 1068,    0, 9884, 2167, 2042,    0,
 9884, 2863, 1990, 1564, 1997, 2093,    0,    0,    0,    0,
    0,    0,    0, 2043,    0,    0,    0,    0, 1695, 9884,
    0, 7159, 2361, 1755,    0, 1992, 5259, 1151,    0,    0,
    0, 2051,11000, 6708, 2084, 2155,    0,    0,    0,    0,
 1827,    0,    0, 1293, 1449,    0,    0,  629,    0,    0,
    0, 5259, 7706,    0,    0, 1997, 2212, 2144,    0,  852,
 5259,    0, 2157,  639, 2054, 2160, 2057, 2180, 2170, 2211,
 1989,    0,    0, 2469,    0, 2476,    0, 2171, 2212, 2076,
    0,    0,    0,    0,    0,    0,    0,    0, 1989, 1700,
    0, 1449, 2035, 2207, 2146,    0, 1814,    0,    0,    0,
 2158,  -45,    0, 1905, 2165, 1997, 2496, 2498,  699,    0,
    0, 2504, 2507, 1997, 2514, 2516,11000,11000, 1997, 2517,
 2523, 2531, 2533, 2540, 2541, 2542, 2543, 2544, 2553,    0,
    0,    0,    0,11000, 2560,    0,    0, 9884,    0,    0,
    0, 1720,    0,  205, 1720,    0, 5259,    0,    0, 1120,
 1997, 2505,    0,   40,  864,    0,    0,    0,    0, 1652,
    0,    0,    0,    0,10862, 2665,  542, 2546, 2562, 2268,
11000, 2555, 2556, 2781, 2557, 2558, 2286, 2565,    0,    0,
    0, 2325,10104, 2568,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -47, 2576,    0,    0,
 2597, 5259,    0,    0, 2287, 2296, 5259, 1785,    0,    0,
 1060, 5259,    0,    0,    0,    0,    0,    0,    0, 2155,
    0,    0,    0,    0,    0, 2155,    0,    0,    0,    0,
 2087, 1720,    0,    0,11000, 1293,    0,    0,    0, 1293,
    0,    0,11000,    0,    0,    0,    0, 1997, 1997,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1997,
    0,    0, 1938,    0, 1938, 2601,    0, 6708,11000,    0,
    0, 2275,    0, 2610,    0,    0,    0,  137,    0,10761,
 5259,    0,    0,11000,    0, 6708, 1997,    0,    0,    0,
 2587,    0,    0,    0, 2508,    0,    0,   82,    0, 2203,
    0,    0,  -20, 2617,    0,    0,    0,    0, 1785, 1060,
    0, 2283,    0,    0,    0,    0, 2035,    0, 1648, 2615,
 1151,    0,    0,    0, 1938, 1997,    0,    0, 1564, 2083,
 2083, 5259,    0,   40,    0,    0, 6220, 3015,    0, 2609,
    0, 2462, 9309, 1391,  645, 2611,    0, 1182,    0,    0,
 2623,    0, 5259, 5259,  130,  161,    0,    0, -130, 2331,
 6708,    0, 2283,  311, 1785, 2295, 2297,    0,    0, 1151,
 2632, 2087, 2338, 2083, 2641, 1720, 2084, 2084,    0, 2009,
 5259,    0, 2648, 2647, 1436,    0,    0,    0, 2282, 2508,
    0, 2263,    0,    0, 2508, 2185,    0,    0,    0,    0,
 7706,   82,11000, 2508,  707, 1785,  690,  690,    0, 2398,
    0, 2258,    0,    0,    0, 1151, 2087,    0,    0, 1938,
 2317, 2317, 5259,    0,    0,    0, 6220, 2668,    0, 5259,
    0, 1686, 2650, 1221, 7706,    0,    0,    0,    0,  878,
  194, 2410,    0, 2247,  566,    0,    0,    0, 1121, 2083,
    0,    0,    0,    0, 5259,    0, 2508, 2671,    0,    0,
 2411, 2508, 2424, 2415, 7706,    0,    0,    0, 6708, 2084,
 2678, 2684, 2011, 7706,  177, 5259, 7706,    0,   40, 2317,
 5259,    0,    0,    0, 7706,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  211,  633,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  150, 6133,    0,    0,    0,    0,    0,    0,
 2727,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  207,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1014,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2393,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2348,    0,    0,
    0,  230,    0,  230,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2357,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1144,    0,
    0, 8918, 6392,    0,    0,    0, 1178,    0,    0,    0,
 1944,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  154,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -10,    0,    0,    0,    0,  715,    0,
    0, 1189,    0,    0,    0,    0,    0,  121,    0,11522,
    0,    0,    0,    0,    0,    0,    0,    0,  -10,    0,
  398,    0,    0, 2692,    0,    0,    0, 2470,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1228,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1369,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1216,    0,    0,
    0,    0,    0,    0,    0, 7671,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1070,    0,    0, 1380,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -10,    0, 2692,    0, 2470,    0, 1422,    0,    0,    0,
    0,  181,    0,    0,    0,    0,    0, 1426,    0, 2457,
    0,    0,    0,    0, 1499,    0,    0,    0,    0,    0,
 2472,  662,12832,    0, 1506,    0,    0,    0,    0,  360,
    0,    0,    0, 2279, 2279,    0,    0,    0, 1536,  241,
    0,    0,    0,    0,    0,    0,    0, 2472,    0,    0,
 1401,    0,    0,    0,    0,    0,    0,    0, 9819, 2350,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1950,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2750,    0,    0,    0, 3054, 3358,    0,
    0,    0,    0,    0,    0,    0, 1410,    0,    0,    0,
    0,    0,    0,    0,    0,  200, 3662, 2441,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1567,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1569,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  855,    0,    0, 2418,    0, 2356,    0,    0,    0,    0,
    0,  428,    0,  428,    0,    0, 1224,    0, 1257, 1314,
    0,    0,    0,    0, 1228,    0, 2472,    0,    0,    0,
    0,  128,  245,    0,    0,  164,    0,    0,    0, 1187,
    0,  623,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2279, 2279, 2279, 2279,    0,  631,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2477,    0,    0,    0,    0,    0, 2457,    0,    0,
 1258,    0, 2274,    0,    0,  900,    0,    0,    0,    0,
  818,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1585,    0,    0,    0,    0, 1335, 4319,    0,    0,
    0, 4839,    0,    0,11337,    0,    0,11349,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 4143, 4329,    0,    0,    0,    0,11361, 7314,    0,
11349,  -10,    0,    0,    0,    0,    0,    0,    0,    0,
  662, 1232,    0,    0,    0,    0, 2335,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11630,11791,11934,    0,
    0, 1109,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11080,    0,11080,    0,    0,
    0,11080,11080,11080,    0,    0,    0,    0,    0, 2300,
    0,    0,    0,    0, 4702, 5006, 9070, 2710,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  285,    0,    0, 6133,    0,    0,  285,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2324,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  541,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  662,    0,  231,    0,    0,    0,    0,
    0,    0,    0,    0, 1213,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  995,    0,
    0,    0,    0,    0,    0,    0, 2715,    0,    0, 3913,
 3917,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  370,  379,  697,    0,    0,    0,  183,    0,    0,    0,
    0,    0,    0, 2928,    0,    0,    0,    0,    0,11337,
    0,    0,    0,    0,  768,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2472,    0,    0,  374,    0,    0,
    0,    0,    0,    0,    0,    0,    0,10377,    0,    0,
    0,  117,    0,    0,    0,    0, 2447,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
11080,    0,    0,    0,    0, 1774,    0,    0,    0, 3966,
 5222, 5526, 4182, 4486,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1598, 1604,    0,    0,
    0,    0,    0,    0,    0, 1614,    0,    0, 8918,    0,
 1614, 1620, 1623,    0,    0,    0,    0,    0,    0,  821,
    0,  851, 3761,    0,    0,    0,  761,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  165,    0,    0,
    0,    0,    0,    0, 2279, 2299,    0, 1675,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2489,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1343,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1627,    0,    0,    0,    0,    0,    0,    0,    0,
 3045,    0,    0,    0,    0,    0,    0,    0,    0, 1381,
    0,    0,    0,    0,    0,    0,    0, 1232, 2489, 2279,
 1642,  662,    0, 2502, 8918, 1646,  120,    0,    0,    0,
    0,10377,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 3468,    0,    0,    0, 5359,    0,    0,    0,    0,    0,
    0,    0, 6080,    0, 6472,    0, 8345,    0,    0,    0,
    0,    0,    0,    0, 8880,    0,    0, 9205,    0,    0,
 9285,    0,11388,    0,    0,11997,    0,    0,12033,    0,
    0, 5370,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2336,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1598, 1668,    0, 2334,  307,    0,    0,
    0,    0,    0,    0,    0, 1699, 1706, 1710,    0,    0,
 2434,  933,    0, 5606,    0, 2436,    0, 2440, 2442,    0,
    0,  376,    0,    0,    0, 2489,    0, 1646,    0,  151,
    0,    0,    0, 2315,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8254,    0, 2515,
    0,    0,    0, 1559,    0, 1343, 2393,    0,    0,    0,
    0,    0,  701,  734,  741,  769,  812,  814,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,    0,    0,    0,    0, 4094,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1711,    0,    0,
    0,    0,    0,    0,    0, 1716,    0,    0,    0,    0,
    0,  339,    0,    0,    0,    0,    0,    0,  267,    0,
  273,    0,    0,    0, 9070,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12069,    0,12105,    0,12141,    0,
    0,    0,    0,12213,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2726,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1794,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1728, 1468,    0,    0,    0, 3781,    0,    0,
    0,    0,    0,    0, 1022,    0,    0,    0,    0,    0,
 2279,    0,    0,    0,  971,    0,    0,    0,    0,    0,
    0,    0,  670,    0,    0, 2734, 1470, 4259,    0, 2457,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2489,    0,    0,    0,    0,    0,    0,    0, 1732,  941,
    0,    0,    0,    0,    0,    0,    0,    0, 2489, 2502,
    0,  383,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  162,    0,12285,    0,    0,11080,    0,
    0,    0,    0,12398,    0,    0,    0,    0,12430,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  668,    0,  307,  868,    0, 3352,    0,    0, 2434,
  155, 2204,    0,  844, 2747,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1808, 1863,    0,    0,    0,    0,    0,    0,
    0,    0,  674,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1865,    0,
    0,    0,    0,    0,    0,    0,    0, 4512,    0,    0,
  115,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  202,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12514,12543,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2748,
    0,    0,  412,    0, 1077, 3656,    0,    0,    0,    0,
    0, 2758,    0,    0,    0,    0,    0, 2731,    0,    0,
    0,    0,    0,    0,    0,    0, 2740,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2494,
    0,    0,    0,    0,    0,    0,    0,    0, 4512,  606,
    0,  132,    0,    0,    0,    0,    0,    0,    0,  476,
  237,    0,    0,    0,  229, 2734,    0,    0,    0,  119,
  291,    0,    0, 4327,    0,    0, 2747,    0,    0,    0,
    0,    0,   93,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  765,    0, 1343,  781,  867,    0,    0, 2434,
 1326,    0,    0,  108,    0,  668,  248,  330,    0,    0,
 1491,    0,    0, 2763,    0,    0,    0,    0, 2746,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    8,    0,    0,    0,    0, 4512,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  681,    0,    0,    0,  412,
 2765, 1342,    0,    0,    0,    0, 2747,    0,    0,    0,
    0,    0,    0,    0,    8,    0,    0,    0,    0,    0,
 2503,    0,    0,    0,    0,    0,    0,    0, 2434,  119,
    0,    0,    0,    0,    0,    0,    0, 2756,    0,    0,
12604,    0,    0,    0,    8,    0,    0,    0,    0,  248,
 2785,    0,    0,    8,    0,    0,    8,    0, 3552, 2765,
    0,    0,    0,    0,    8,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2822,    0,    0, 2830,
    0,    0,    0,    0,    0,    0,    4,    0, 2831,    1,
  282,   38, -632, 1629,  375, -197, -568, 2823, 2561,    0,
  -27,    0, 2570,  -93,  -40,    0,  314, -884,    3,  -61,
   -6, -167, 2573, 2118,    0,    0,    0, 1139,    0,    0,
  -36, 1738, -546,  -67, -940, 2208, 1975, -311, 1745, 2762,
 -286,    0,  -56,    0, -471,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -620,  136, 1751, -289, 1165, -944,    0,
    0,    0,    0, -317, -123,-1082, 1761, -344,  -51,    0,
 -526,    0,    0, 1404, -980,    0, 1402,    0,    0, 2539,
 -675,  -18,    0,    0,    0,    0, 2511,    0, 2285, 2551,
 -198, 1802, -643,    0,    0,    0, -719,    0,    0,    0,
    0,    0,    0, 1847,    0, 2574, 2125, 1807, 2552, 2567,
    0, 1245,    0, -525, 2106,  -41, -664, 1329,    0,    0,
    0, -976, -938, -929, -274, -928,    0, 1474,-1028,    0,
 1170,-1207, 1475,    0, 2344,    0,    0,    0, 1169, 1171,
 1132,   28, -302, -824, -433,    0,    0, 2131,    0, 1664,
    0,    0, -314,    0,    0,    0,    0, -162,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1202,    0, 1188,    0,  823,    0,    0,  464,    0, -388,
 1048, 1372, 1079, 1249,    0, 1154,    0, 1925, 1707, 1520,
    0,-1102, -822, -915,    0, -496, -698, -534,    0, -672,
 1239,  799, -780, -636, -543, -723, -954,-1197,    0,    0,
   23,    0,    0,    0,    0,    0, 2600,    0,    0,    0,
    0, 2619, 2383,    0,    0,    0,    0,    0,   -9, 2628,
    0,    0,    0,    0,  -15,    0,    0,    0, -335, 2112,
 -331, 1665, -473,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2339,    0, 2121,    0,    0,    0,    0,    0,
    0,  227,    0, 2949,    0,    0,    0,    0,    0, 2667,
    0,    0,    0,    0, 2465,    0, 2467,    0,    0,    0,
 2225,    0,    0, -952, -211,    0,    0,  527,    0, 2213,
    0,    0,    0,    0,    0, -615,-1183, -587, -181, -177,
 1382,    0, 2838, -120, 1269,    0, 2248, -625,-1016, -750,
    0, -807, 2005,    0,    0,    0, -145,    0,    0, -246,
    0, 1344, 1460,    0, 1185,    0, 1235,    0, 1138,    0,
    0,    0,    0, 2486,    0,    0,    0, -402, 2277, 2022,
 -867,    0,    0,    0, -671, -485,  257,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2276, 1319, 1861,    0, -616,    0,    0,    0, -920,    0,
    0,    0, 1772,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -282, 1821,
    0,    0,
};
#define YYTABLESIZE 13349
static short yytable[] = {                                     129,
  151,  191,  129,   37,   78,  310,  150,  214,  156,  441,
  261,  210,  492,  933,  590,  194,  280,  283,  760,  281,
  284,  211,  556,  251,  746,  206,  219,  622,  245,  197,
  787,  623,  254,  790,  699,  249, 1220,  584,  799,  801,
  959,  224,  554,  188,  190,  193,  196,  150,  201,  250,
  766,  911, 1023,  212,  190,  217,  256,  196, 1442,  150,
  201, 1252,  230,  129, 1263,  636,  231,  151, 1252,  248,
  724,  241,  226,  150,  243,  190,  252,  241,  217,  193,
  196,  941,  150,  201,  230,  129, 1009,  196,  255,  267,
  223,  269,  315,  652,  956,  253,  467,  982,  864, 1253,
 1014, 1264,  873,  488,  268,  467, 1253,  705, 1254, 1256,
  979,  311, 1272,  223,  758, 1254, 1256, 1257, 1190,  356,
  151,  212,  150,  212, 1257,  313,  291,  628,  546, 1041,
 1288,  257, 1293,  279,  375,  217, 1011, 1068, 1048, 1310,
 1094,  309,  196,  150,  230,  513,  265, 1018,  156,  574,
  181,  328,  337,  634,  703,  758, 1212,   85,  758,  705,
   85,  983,  514,  636,  635,  913,  705, 1404,  628,  881,
  744,  628,  257,  758,   77,  257,  441,  364,  356,  151,
  638, 1221,  196,  759,  265,  721,  628,  774, 1102, 1069,
  257,  181, 1089, 1482,  181,  703,  278, 1718,  703,  326,
 1522,  707,  527, 1074, 1782,  527,  622, 1407,  574,  181,
  574, 1478,  634,  703,  763, 1525,  460,  851, 1201,  279,
 1775,  519,  636,  635,  777,  778,  779,  780,  700,  622,
  632, 1161, 1162,  623,  463, 1440,  362, 1775,  499,  638,
  152,  196,  997,  463,  621,  833,  368,  590,  212,   77,
   77,  502,  150,  269,  469,  129,  282,  502,  326,   77,
  707,  150, 1187,  469, 1305,  622,  512,  279,  506,  574,
 1683,  632, 1311,  758,  632, 1610, 1207,  772,  807,  279,
  362,  731,  193,  821,  749,  621,  150,  700,  709,  632,
  705,  555, 1739,  135, 1055,  362, 1399,  719,  558,  152,
  211,  907,  571,  621,  848,  573,  525,  780,  478, 1617,
  834, 1457, 1117,  782, 1662, 1252, 1388,  338, 1390,  585,
  822,  587,  644,  212,  632,  338,  749,  514,  845,  709,
  212,  705,  212,  348,  212,  640, 1452,  576,  517, 1204,
 1395, 1191, 1542,  749, 1732,  131,  642, 1236,   60,  705,
 1418, 1216, 1218, 1253, 1213,  196,  201, 1467,  503,  478,
 1419, 1248, 1254, 1256,  150,  525,  193, 1075,  634,  385,
  709, 1257, 1118,  125, 1614,  705,  369, 1663,  383,   89,
 1691, 1275,  196,  285,  317, 1483,  136, 1527,  709, 1609,
 1193, 1271, 1208, 1733,  339, 1474,   14,  750, 1114,  151,
  269,  755,  492, 1205, 1734, 1684,   83,  504,  181, 1440,
  349, 1416,  132,  504,  702, 1217,  705, 1408,  478,  274,
  151,  181,  273,  718,  823,  908,  912,  602,  385,  181,
  505,  512,  125,  715,  705,  527, 1045,  383,  718, 1328,
 1329,  196, 1330, 1331,  723,  628, 1618,  940,  729,  151,
  181,  150,  700,  463,  196,  469,  181, 1843,  181,  827,
  338,  196,  150,  955, 1781,  492,  150,  628,  151,  196,
  527,  150,  703,  370,  156,  361,   97,  151, 1316,  181,
 1865, 1742,  196,  151, 1544,  703,  602,  492, 1429, 1580,
  847, 1581, 1036,  634,  703, 1038, 1119,  758,  181, 1749,
  775,  151,  328,  636,  635,  705,  771,  181, 1735, 1044,
 1337,  196,  275,  181,  705, 1607,  828, 1388,  732,  152,
  638, 1423, 1276,  803,  622, 1756, 1608,   85, 1032,  707,
  196,  181,  707, 1115,  361,  986, 1062, 1789,  196,  196,
  152,  703, 1101, 1619, 1304,  196,  622,  705,  632,  829,
  703, 1032, 1087,  467,  796,  212, 1104,  873,  987,  700,
  796,  705,  621,  196,  362, 1396,  161,  362,  181,  152,
  632, 1428,  527,  356, 1211,  196,  264,  196, 1823,  698,
 1470,  628, 1035,  277,  621,  574, 1403,   76,  152,  793,
 1109,  381, 1655, 1444,  703,  720, 1010,  152, 1657,  211,
  196,  858,  859,  152, 1530,  758,  223,  634,  703,  326,
  743, 1787,  758,  514,  277,  758,  277,  636,  635,  574,
 1002,  152,  466,  478,  850,  196, 1600,  527, 1200,  628,
  468,  212,  257,  385,  709,  650,  650,  151, 1469,  133,
  875,  707,  383, 1455, 1606,  650,  758,   89, 1694,  758,
  874,  749,  780, 1030, 1031,  707, 1644, 1116,  782,  855,
  622,  915,   76,   76,  758, 1698, 1623,  181,  700, 1543,
  217,  960,   76,  525,  277,  754,  362,  705, 1233, 1593,
  363,  466,  700,  122,  632, 1772,  705,  320,  362,  468,
  362,  319,  718,  705,  949,  158,  384, 1112,  621,  196,
  390, 1113,  212,  212, 1541,  492,  469, 1243,  707,  907,
  734,  469, 1034,  907,  379,  705,  709,  512,  539, 1489,
  469,  125,  211,  196,  363,  506,  460, 1008,  196, 1234,
  196,  907,  999,  391, 1019, 1019, 1024, 1766, 1534,  363,
  387, 1252,  700,  150,  843,  150, 1220, 1822,  212,  558,
 1053, 1748, 1788, 1262,  212,  384,  478,  152,  212,  390,
  700, 1755,  705, 1255,  257, 1270,  385,  135,  386,  602,
 1255,  705, 1306,  379, 1526,  383, 1025, 1054, 1056, 1253,
  253,  212,  145, 1084, 1085, 1086, 1292,  159, 1254, 1256,
 1104, 1477,  391, 1194, 1067, 1443, 1197, 1257,  700,  387,
 1383,  700,  223,  361, 1794,  257,  361,  700,  257,  212,
  212,  389,  844,  388,  149,  705, 1456,  399,  639,  700,
  683,  253, 1466,  257,  253, 1434,  135,  386, 1088,  705,
 1704,  514,  602,  908, 1406,  602, 1510,  908, 1099,  253,
  223,  936,  937,  704,  202,  525,  150, 1061,   14, 1026,
  665,  602,  708, 1363,  743,  908,  150,  709,  122,  122,
 1072,  683, 1273, 1130,  683, 1847,  254,  707, 1111,  552,
  389, 1471,  388,  223,  223,  223,  399,  602,  540,  683,
  466,  242,  553,  758,  704,  758, 1179,  146,  468, 1131,
  907,  665,  758,  466,  147,  743, 1376,  115,  743, 1378,
  907,  468,  704, 1198, 1381,  529,  907,  254,  707,  665,
  254,  882,  804,  743,  530,  361,  889, 1848, 1704,  708,
  706, 1195,  707,  218,  709,  254,  707,  361,  466,  361,
  466,  204,  205,  136,  115,  512,  468,  492,  468,  242,
  137, 1051,  441, 1661, 1710,  606, 1711,  160, 1095,   76,
 1002,  469,  196,  201,  469,  230,  129,  542,  285, 1199,
  384, 1759,  469,  774,  390, 1196, 1274,  285,  907,  115,
  196, 1704, 1714,  379,  469,  469,  441,  289,  379,  196,
  242,  287, 1537,  758,  379,  938, 1662,  338,  758, 1451,
 1725,  338,  136,  805,  527,  707,  606,  391,  707,  137,
  161,  286,  236,  747,  387,  758, 1754,  543,  363,  238,
  909,  363,  758,  576,  908, 1299, 1704, 1255,  257, 1658,
 1473,  709,  297,  698,  908, 1643, 1750,  622, 1645,  196,
  908,  623,  386, 1319,  806,  527,  492,  365,  527, 1323,
  466,  544,  257,  726,  257,  135,  758,  135,  468, 1663,
  211,  257, 1833,  527,  707,  527, 1283,  460,  253,  545,
  253,  242,  709,  707, 1753, 1785, 1824,  253, 1284,  597,
  727, 1283,  576,  366,  531,  389,  700,  388,  700, 1796,
  709,  399,  212,  323,  683,  246, 1393, 1393, 1397,  212,
  704,  700,  908,  384, 1516,  705,  324,  390, 1519,  258,
  700, 1830,  242, 1540,  959,  223,  758, 1825,  706,  708,
  706,  379,  707, 1753,  709, 1705,  577,  700,  743, 1425,
  363, 1427,  338,  223, 1829,  492,  338,  578,  597,  223,
  391,  325,  363,  326,  363,  700,  882,  387,  683,  262,
  882,  683,  257,  572,  254,  135,  254,  700,  683,  706,
  807,  683,  263,  254, 1826,  683,  700,  704,  253, 1753,
  683,  704,  705,  683,  257,  386, 1695,  706,  665, 1801,
 1802,  257, 1696, 1859,  743, 1757, 1758,  581,  665,  683,
  253,  665, 1472,  704, 1265, 1266,  177,  253,  578, 1615,
  665,  469, 1753,  469,  270,  707, 1182, 1183,  707,  277,
  700,  233,  572,  683, 1019,  257, 1650,  683,  389,  469,
  388,  371,  187,  136,  700,  196,  683, 1842,  371, 1798,
  137,  253,  814,  603,  815, 1775, 1001,  707,  196, 1585,
  704, 1651,   99,  683, 1317, 1642,  581,  665,  234,  704,
 1774,  196,  293,  327,  254,  177,  665,  578, 1679,  196,
  330, 1586,  527,  668,  707,  266,  613,  460, 1587, 1800,
  683, 1529, 1860,  707, 1775,  257,  254,  668,  527,  101,
  196,  187,  683,  254,  683,  668,  196,  294,  196, 1840,
  683,  253,  603,  704, 1481, 1454,  707,  514, 1485, 1652,
  665,  668, 1588,  271,  527, 1850,  974,  704,  331,  196,
  527,  460,  665,  527,  665,  295,  743,  254,  296,  683,
  683, 1294,  683,  598,  743,  613,  460, 1697,  196,  683,
  683,  683,  683,  683,  297,  683,  196,  196,  683,  683,
  683,  683,  683,  196,  525,  430,  683,  683,  102,  709,
 1184,  622,  251,  743,  743,  668,  743,  704,  103,  272,
  298,  196,  705,  743,  743,  743,  743,  743,  276,  743,
 1295,  709,  743,  743,  743,  743,  743,  254,  328,  974,
  743,  743,  598,  104,  141,  525,  379, 1005,  525,  580,
 1545, 1647,  622,  251,  384, 1514,  251, 1019,  196,  292,
  211,  512,  469,  525, 1006,  525, 1294,  309, 1139,  196,
  622,  251,  196, 1393,  285,  527,  308,  700,  709,  687,
  332,  597,  527, 1591, 1140, 1277, 1656,  514,  321,  354,
  427,  567,  212, 1579,  502,  406,  706,  328,  333,  989,
  527,  150,  708,  706,  318,  707,  706,  709,  580,  706,
 1226, 1226,  486, 1255,  177, 1298,  597,  917,  706, 1771,
  687,  491, 1278,  687, 1227, 1227,  355,  177,  356, 1141,
  527,  709,  334,  700,  527,  177, 1818,  664,  687,  217,
  187, 1226,  700,  502,  597,  709,  441,  597, 1228, 1228,
  567,  357,  522,  187,  406, 1227,  177,  196, 1142, 1660,
  379,  187,  177,  597,  177,  706,  351, 1692,  407,  152,
 1841,  379, 1648, 1849,  706,  403,  358,  664,  664, 1228,
  217,  664,  187,  217, 1387,  177, 1389,  309,  187,  597,
  187,  512,  708,  706, 1188,  707,  664,  709,  217, 1731,
 1858,  153, 1229, 1229,  177,  160,  190,  285, 1693, 1864,
  704,  187, 1867,  177, 1672,  705,  154,  340,  706,  177,
 1870,  708,  706, 1579,  707, 1143,  709,  407,  230, 1690,
  187,  362,  706, 1229,  403,  603,   43,  177,   51,  187,
   80,   81,  949,  708,  706,  187,  707,  150,  709, 1707,
  919,  690,  193, 1708,  161,  695,  696,  698,  342, 1831,
 1832,  343,  525,  187,  160,  151, 1743,   41,  613,  230,
  603,  150,  230,   24,  177,  708,  706,  353,  707,  372,
  709,  525,  525,  754,  525,  285,  374,  230,  525,   47,
 1723,  525,   49,  376, 1731,   43,  480,   51,  603, 1731,
  187,  603,  763,  613,  525,  514,  344,  380, 1731,   52,
  525,  127,  377,  161,  542,  374,  364,  603, 1868,  735,
   53,  196,  258,  737,  381,  598,   41,   54,  921,  763,
  364,  613,   24,  489,  613,  588, 1731,   25,  364,  514,
  704,  497,  754,  603,  188,  705, 1777,  464,   47,  472,
  613,   49,   55,  151,  364,  480, 1740, 1699,  473,  150,
  598, 1731,  708,  706,  543,  707, 1731,  709,   48,  514,
  127, 1761,  999,  177,  374,   50,  613,  150,  514,   45,
  128,  514,  525,  529,  476,  116,  687,  687,  598,  514,
  477,  598,  530,  190, 1779, 1837,   25,   46,  544,  187,
 1770,  217,  481,  188,  525, 1780,  151,  598,  364,  512,
  479,  525,  150,  424,  482, 1761,  545,   56,  140,  141,
  525,  150,  525,  204,  205,   98, 1836,   48,  714,  792,
  704,  713,  791,  598,   50,  705,  212,  483,   45,  128,
  525, 1579, 1808,  512,  116,  525,  498,  943,  500,   57,
  710,  151,  501,   58,  510,  664,   46,  150,  664,  704,
  217, 1761,  520,  999,  705,  664,   99,  150,  664,   14,
  193,  526,  664,  512,  525, 1579,   14,  664,  100, 1809,
  664,  704,  512,  549,  882,  512,  705,  882,  978,  687,
  527,  977, 1810,  512,  888,  193,  664,  720, 1093, 1178,
  538, 1092,  541,  101,  883, 1579,  242,  883,  230, 1063,
 1064,  491, 1341,  704, 1579,  916, 1111, 1579,  705,  105,
  664,  193,  932,  996,  664, 1579, 1100,  939,  427,  869,
  687,  123,  123,  664,  720,  996,  882,  946,  996,  950,
  951,  952,  953,  954,  427,  946,  562,  720,  106,  107,
  664,  108,  589,  738,  569,  572,  883,  582,  109,  110,
  111,  112,  113,  586,  114,  637,  797,  115,  116,  117,
  118,  119,  102,  641,  491,  120,  121,  664, 1001,  645,
  987,  676,  103,  400,  401,  402,  643,  677,  678,  664,
 1001,  664,  984, 1001,  679,  974,  491,  664,  680,  681,
  704,  522,  188,  682,  683,  705,  242,  104,  684,  685,
  686,  410,  411,  690,  910,  188,  914,  687,  688,  994,
  918,  920,  922,  188,  693,  694,  664,  664,  242,  664,
 1192, 1498,  703, 1191,  710,  242,  664,  664,  664,  664,
  664,  711,  664,  712,  188,  664,  664,  664,  664,  664,
  188,  716,  188,  664,  664, 1237,  730,  690, 1092,  970,
  994,  994,  994,  994,  994,  994,  994, 1242, 1286,  242,
 1241, 1285,  690,  188, 1291,  721,  733, 1290,  994,  994,
  994,  994,  708,  706,  738,  707,  740,  709,  741,  736,
  708,  706,  188,  707, 1401,  709, 1491, 1400,  753, 1191,
 1521,  188, 1528,  710,  757,  710,  758,  188,  708,  706,
  994,  707,  994,  709,  942,  708,  706,  762,  707, 1804,
  709, 1863, 1803,  764, 1775,  188, 1120, 1121, 1125,  765,
  767, 1133, 1135, 1137,   14, 1145, 1148, 1151, 1153, 1156,
 1159,  768,  105, 1163, 1164,  770, 1166,  773, 1168,  781,
  529, 1171, 1172, 1173, 1174, 1175, 1176,  776,  782,  784,
  989,  785,  188,  794,  818,  819,  825,  820, 1180,  830,
  427,  106,  107,  831,  108,  835,  837,  105,  836,  277,
  838,  109,  110,  111,  112,  113,  840,  114,  841,  846,
  115,  116,  117,  118,  119,  852, 1699,  853,  120,  121,
  856,  857,  866,  868,  869,  877,  106,  107, 1729,  108,
  878,  879,  883,  934,  491,  105,  109,  110,  111,  112,
  113,  944,  114,  975,  973,  115,  116,  117,  118,  119,
  972,  958,  980,  120,  121,  984,  988,  708,  706,  985,
  707,  996,  709, 1003,  106,  107, 1012,  108, 1015, 1032,
 1037,  847, 1043, 1042,  109,  110,  111,  112,  113,  797,
  114,  188, 1045,  115,  116,  117,  118,  119, 1049, 1050,
 1058,  120,  121,  699,  708,  706, 1052,  707, 1066,  709,
 1059,  994, 1070,  994,  994, 1134, 1060, 1138, 1076, 1146,
 1149,  994, 1154, 1157, 1160, 1083,  958, 1081, 1090,  994,
 1095, 1107, 1108, 1729,  994,  233,  726,  994, 1816, 1127,
 1165, 1167, 1169, 1177,  699, 1189,  704, 1729, 1203, 1209,
  704,  994, 1201, 1206, 1219,  705,  994,  994,  704, 1223,
  994, 1225,  699,  705,  690, 1231,  713,  994, 1244,  994,
  994,  690,  234,  994,  690, 1729,  704,  994,  690, 1247,
  994,  705,  994,  704,  994, 1267,  690, 1258,  705,  994,
  994,  946,  994, 1170,  708,  706, 1269,  707, 1279,  709,
 1729, 1287,  690, 1344, 1345, 1729, 1347, 1349,  994,  994,
 1289,  994, 1296, 1300, 1354,  923,  924,  925,  926,  927,
  928,  929,  930,  707, 1301,  994,  690,  994,  994,  994,
 1302, 1303,  994, 1367, 1101,  994,  994, 1312, 1321, 1324,
 1314, 1374, 1375,  427, 1326,  994,  427, 1325, 1180, 1327,
 1333, 1334, 1382,  427, 1384, 1385,  690, 1338, 1339,  994,
 1340,  690,  994, 1380,  707,  907, 1361,  708,  706, 1342,
  707, 1343,  709, 1362,  708,  706,  491,  707,  994,  709,
 1350, 1351,  707,  690,  994, 1352, 1353,  427, 1355,  994,
 1356, 1357, 1358, 1359, 1360,  690, 1386,  690, 1405,  661,
  994,  994, 1377,  994, 1402,  704, 1409, 1410, 1411,  994,
  705, 1412, 1413, 1414, 1421,  957, 1365,  708,  706, 1431,
  707, 1424,  709, 1426, 1332, 1433, 1436, 1366,  708,  706,
  564,  707, 1441,  709, 1445, 1446, 1453, 1447,  994,  994,
  974,  994,  704, 1448, 1449, 1450, 1468,  705,  994,  994,
  994,  994,  994, 1458,  994, 1475, 1479,  994,  994,  994,
  994,  994, 1480, 1484, 1335,  994,  994, 1368,  708,  706,
 1490,  707, 1511,  709, 1517,  491, 1518, 1523, 1524, 1535,
 1597,  974,  974,  974,  974,  974,  987,  974, 1369,  708,
  706, 1582,  707, 1592,  709, 1594, 1486, 1595, 1596,  974,
  974,  974,  974,  708,  706, 1494,  707, 1598,  709, 1601,
  105, 1599, 1499, 1370,  708,  706, 1602,  707,   77,  709,
 1768,  699, 1603, 1371,  708,  706, 1611,  707, 1616,  709,
 1613,  974,  704,  974,  699, 1620, 1621,  705, 1622,  106,
  107,  427,  108,  699, 1624,  427,  105, 1625, 1649,  109,
  110,  111,  112,  113, 1626,  114, 1627, 1630,  115,  116,
  117,  118,  119, 1631,  491,  424,  120,  121, 1372,  708,
  706, 1632,  707, 1633,  709,  106,  107, 1531,  108,  105,
 1634, 1635, 1636, 1637, 1638,  109,  110,  111,  112,  113,
  699,  114, 1814, 1639,  115,  116,  117,  118,  119,  699,
 1641, 1666,  120,  121, 1665,  704, 1821,  132,  106,  107,
  705,  108,  704, 1668, 1669, 1673, 1674,  705,  109,  110,
  111,  112,  113, 1676,  114, 1675, 1682,  115,  116,  117,
  118,  119, 1677, 1685, 1838,  120,  121, 1686, 1688, 1689,
 1712,  707, 1716,  699,  105, 1727, 1373,  708,  706, 1717,
  707,  707,  709, 1678,  707,  704, 1741,  699, 1752, 1853,
  705, 1628, 1629,  707, 1855, 1744,  704, 1767, 1776, 1773,
 1784,  705, 1795,  106,  107,  661,  108, 1790, 1640, 1792,
 1797, 1799,  427,  109,  110,  111,  112,  113, 1806,  114,
 1807, 1812,  115,  116,  117,  118,  119, 1815, 1787, 1788,
  120,  121,  974,  160,  974,  974,  704, 1835, 1839, 1846,
  707,  705,  974, 1845, 1775, 1667, 1856, 1854, 1857,  707,
  974, 1861, 1730, 1659, 1862,  974,    1,  704,  974,  628,
   29,  203,  705,  758,  371,  271,  148,  657,  525,   31,
  269,  704,  974,  457,  974,   81,  705,  974,  974,  998,
  957,  974,  704,  687,  213,  525,  193,  705,  974,  276,
  974,  974,  704,  707,  974,  399,  938,  705,  974,  525,
  956,  974,  194,  974,  372,  974,  698,  707,  696, 1706,
  974,  974,  692,  974,  694,  275,  715, 1709,  937,  223,
  998,  998,  998,  998,  998,  998,  998,  714,  323,  974,
  974,  288,  974,  716,  331,  622,  343,  704,  998,  998,
  998,  998,  705, 1531,  329,  664,  974, 1730,  974,  974,
  974,   27, 1817,  974, 1722,  725,  974,  974, 1724,   30,
   39, 1730, 1391,  480,  137,  471,  974,  470,  974, 1670,
  998, 1322,  998, 1106,  664,  664, 1320,  664,  247, 1318,
  974, 1308,  867,  974,  664,  664,  664,  664,  664, 1730,
  664, 1605, 1604,  664,  664,  664,  664,  664,  551,  974,
  596,  664,  664,  534, 1246,  974,  832, 1512,  708,  706,
  974,  707, 1297,  709, 1730,  704,  518,  427, 1013, 1730,
  705,  974,  974,  566,  974, 1513,  708,  706, 1309,  707,
  974,  709, 1719, 1520,  708,  706, 1047,  707,  567,  709,
 1687, 1584, 1786,  813, 1590, 1793, 1791,  708,  706, 1828,
  707, 1040,  709, 1435, 1769, 1671, 1783,  522, 1738,  974,
  974,   76,  974,  105, 1866, 1819, 1538, 1751, 1238,  974,
  974,  974,  974,  974, 1417,  974,  574, 1820,  974,  974,
  974,  974,  974,  557,  817,  548,  974,  974, 1465, 1082,
 1073,  139,  106,  107,  748,  108,  511,  105,  522,  854,
  752,  522,  109,  110,  111,  112,  113, 1004,  114, 1017,
  290,  115,  116,  117,  118,  119,  522,  998,  522,  120,
  121, 1834, 1715, 1222, 1653, 1805,  106,  107, 1869,  108,
  761, 1346, 1215,    0, 1379,  981,  109,  110,  111,  112,
  113,  998,  114,  998,  998,  115,  116,  117,  118,  119,
 1430,  998,    0,  120,  121,    0,    0,    0,    0,  998,
    0,    0,    0,    0,  998,    0,    0,  998,    0,    0,
    0,    0,    0,    0,  527,    0,    0,    0,    0,    0,
    0,  998,    0, 1003,    0,    0,  998,  998,    0, 1249,
  998,    0,    0,    0,    0,    0,    0,  998,    0,  998,
  998,    0,    0,  998,    0,    0,    0,  998,    0,    0,
  998,    0,  998,    0,  998,  527,    0,    0,  527,  998,
  998,    0,  998,    0, 1003, 1003, 1003, 1003, 1003, 1003,
 1003,    0,    0,  527,    0,  527,    0,    0,  998,  998,
    0,  998, 1003, 1003, 1003, 1003,  704,    0,    0,    0,
    0,  705,    0,    0,  105,  998,    0,  998,  998,  998,
    0,    0,  998,    0,  704,  998,  998, 1497,    0,  705,
    0,    0,  704,    0, 1003,  998, 1003,  705,    0,    0,
    0,    0,    0,  106,  107,  704,  108,    0,    0,  998,
  705,    0,  998,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,  998,    0,
  120,  121,    0,    0,  998,  522,    0,    0,    0,  998,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  998,  998,    0,  998,  522,  522,    0,  522,    0,  998,
    0,  522,    0,    0,  522,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  522,    0,    0,
    0,    0,    0,  522,    0,    0,    0,    0,  998,  998,
  105,  998,    0,    0,    0,    0,    0,    0,  998,  998,
  998,  998,  998,    0,  998,    0,    0,  998,  998,  998,
  998,  998,    0,    0,    0,  998,  998,    0,    0,  106,
  107,    0,  108,    0,    0,    0,    0,    0,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,  527,    0,    0,  522,    0,    0,    0,  388,
    0,    0,    0,    0,    0, 1003,    0, 1003, 1003,    0,
    0,    0,  527,    0,  527, 1003,    0,  522,  527,    0,
    0,  527,    0, 1003,  522,    0,    0,  390, 1003,    0,
    0, 1003,    0,    0,  527,  522,    0,    0,    0,    0,
  527,  673,    0,    0,    0, 1003,    0,  999,    0,    0,
 1003, 1003,    0,  522, 1003,    0,  392,    0,  522,    0,
    0, 1003,    0, 1003, 1003,  564,    0, 1003,    0,    0,
    0, 1003,    0,    0, 1003,    0, 1003,    0, 1003,    0,
    0,  673,  673, 1003, 1003,  673, 1003,  522,  999,  999,
  999,  999,  999,  999,  999,    0,    0,    0,    0,    0,
  673,    0, 1003, 1003,    0, 1003,  999,  999,  999,  999,
    0,    0,  527,    0,    0,    0,    0,    0,    0, 1003,
    0, 1003, 1003, 1003,    0,    0, 1003,    0,    0, 1003,
 1003,  397,    0,    0,  527,    0,    0,    0,  999, 1003,
  999,  527,    0,    0,    0,    0,    0,  399,    0,    0,
    0,    0,  527, 1003,    0,    0, 1003,  831,    0,    0,
    0,  400,  401,  402,    0,  204,  205,    0,    0,    0,
  527,    0, 1003,    0,    0,  527,    0,    0, 1003,    0,
    0,    0,    0, 1003,  405,  406,  407,  408,  409,  410,
  411,  412,  413,    0, 1003, 1003,    0, 1003,  831,  415,
  416,  831,    0, 1003,  527,    0,    0,    0,  420,    0,
    0,    0,    0,    0,    0,    0,  831,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1003, 1003,    0, 1003,    0,    0,  105,    0,
    0,  367, 1003, 1003, 1003, 1003, 1003,    0, 1003,    0,
    0, 1003, 1003, 1003, 1003, 1003,    0,    0,    0, 1003,
 1003,    0,    0,    0,    0,    0,    0,  106,  107,    0,
  108,    0,    0,    0,    0,    0,    0,  109,  110,  111,
  112,  113,  367,  114,    0,  367,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0, 1487, 1488,
  367,    0,    0,    0, 1492, 1493,    0, 1495, 1496,  999,
    0,  999,  999,    0,    0, 1500, 1501, 1502, 1503,  999,
 1504, 1505, 1506, 1507, 1508, 1509,    0,  999,    0,    0,
    0,    0,  999,    0,    0,  999,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  675,    0,    0,    0,  999,
    0,  852,    0,    0,  999,  999,    0,    0,  999,  673,
    0,    0,  673,    0,    0,  999,    0,  999,  999,  673,
    0,  999,  673,    0,    0,  999,  673,    0,  999,    0,
  999,  673,  999,    0,  673,    0,  675,  999,  999,  675,
  999,    0,  852,  852,  852,  852,  852,    0,  852,    0,
  673,    0,    0,    0,  675,    0,  999,  999,    0,  999,
  852,  852,  852,  852,    0,    0,    0,    0,    0,  831,
    0,    0,    0,  999,  673,  999,  999,  999,  673,    0,
  999,    0,    0,  999,  999,    0,    0,  673,    0,    0,
    0,    0,    0,  999,  852,    0,    0,    0,    0,    0,
  666,    0,    0,    0,  673,    0,    0,  999,    0,    0,
  999,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  667,    0,    0,    0,    0,  831,  999,    0,  831,    0,
    0,  673,  999,    0,    0,  831,    0,  999,  831,    0,
    0,  666,  831,  673,  666,  673,    0,  831,  999,  999,
  831,  999,    0,    0,    0,    0,    0,  999,    0,  666,
    0,  667,    0,    0,  667,    0,  831,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  667,
    0,    0,    0,    0,    0,    0,  999,  999,    0,  999,
  831,    0,    0,  831,  831,    0,  999,  999,  999,  999,
  999,    0,  999,  831,    0,  999,  999,  999,  999,  999,
    0,    0,  367,  999,  999,    0,    0,    0,    0,  367,
  831,    0,  367,    0,    0,    0,  367,    0,    0,    0,
    0,    0,    0,    0,  367,    0,    0,    0,    0,    0,
    0,    0,  831,    0,    0,    0,    0,  831,    0,    0,
  367,    0,  223,    0,    0,    0,  494,    0,    0,  831,
   84,  831,    0,  852,    0,  852,  852,   85,    0,    0,
    0,   86,   87,  852,  367,    0,    0,    0,    0,    0,
   88,  852,    0,    0,    0,    0,  852,    0,    0,  852,
    0,    0,   89,  223,    0,    0,  223,  494,    0,    0,
  494,    0,    0,  852,  367,  864,    0,    0,  852,  852,
    0,  223,  852,  675,    0,  494,  675,    0,    0,  852,
    0,  852,  852,  675,    0,  852,  675,    0,    0,  852,
  675,  367,  852,    0,  852,  675,  852,    0,  675,    0,
    0,  852,  852,  367,  852,  367,  864,  864,  864,  864,
  864,    0,  864,    0,  675,    0,    0,   90,    0,    0,
  852,  852,    0,  852,  864,  864,  864,  864,    0,    0,
    0,    0,    0,    0,   91,    0,    0,  852,  675,  852,
  852,  852,  675,    0,  852,    0,    0,  852,  852,   92,
    0,  675,    0,   93,    0,    0,    0,  852,  864,    0,
    0,    0,    0,    0,   94,    0,    0,    0,  675,    0,
    0,  852,    0,    0,  852,    0,    0,    0,  666,    0,
    0,    0,    0,   95,    0,    0,    0,    0,  666,    0,
  852,  666,    0,  527,    0,  675,  852,    0,  667,    0,
  666,  852,    0,  698,    0,    0,    0,  675,  667,  675,
    0,  667,  852,  852,    0,  852,    0,    0,    0,    0,
  667,  852,    0,  698,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  527,    0,    0,  527,    0,    0,
    0,    0,  504,    0,    0,    0,    0,  666,    0,    0,
  852,  852,  527,  852,    0,    0,  666,    0,    0,    0,
  852,  852,  852,  852,  852,    0,  852,  667,    0,  852,
  852,  852,  852,  852,    0,    0,  667,  852,  852,    0,
    0,  867,    0,  504,    0,    0,  504,    0,    0,    0,
  223,    0,  223,    0,  494,    0,  494,    0,    0,  223,
  666,  504,    0,  494,    0,    0,    0,    0,    0,    0,
    0,    0,  666,    0,  666,    0,  494,    0,    0,    0,
  667,    0,  867,  867,  867,  867,  867,  864,  867,  864,
  864,    0,  667,    0,  667,    0,    0,  864,    0,    0,
  867,  867,  867,  867,    0,    0,    0,    0,    0,    0,
  864,    0,    0,  864,    0,    0,    0,   62,  231,    0,
    0,    0,    0,    0,    0,    0,    0,  864,   63,    0,
    0,    0,  864,  864,  867,   64,  864,    0,    0,    0,
    0,    0,    0,  864,    0,  864,  864,    0,   65,  864,
  223,    0,    0,  864,  494,    0,  864,    0,  864,  231,
  864,    0,  231,    0,    0,  864,  864,    0,  864,    0,
    0,    0,  223,    0,    0,    0,  494,  231,  537,  223,
    0,    0,    0,  494,  864,  864,  678,  864,  527,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  864,    0,  864,  864,  864,    0,    0,  864,   66,
    0,  864,  864,  223,    0,    0,    0,  494,  537,  537,
    0,  864,  537,    0,    0,   67,    0,  678,    0,  527,
  678,  527,  527,  527,    0,  864,    0,  537,  864,  537,
  527,    0,   68,    0,    0,  678,    0,  527,    0,    0,
    0,    0,    0,  527,  864,    0,    0,   69,    0,    0,
  864,   70,    0,    0,   71,  864,    0,    0,    0,  537,
    0,    0,   72,    0,    0,    0,  864,  864,    0,  864,
  504,    0,  504,    0,    0,  864,  504,    0,    0,  504,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  504,  867,    0,  867,  867,    0,    0,    0,
    0,    0,    0,  867,  864,  864,    0,  864,    0,    0,
    0,    0,    0,    0,  864,  864,  864,  864,  864,  867,
  864,  527,    0,  864,  864,  864,  864,  864,    0,    0,
    0,  864,  864,  867,    0,  868,    0,    0,  867,  867,
    0,    0,  867,  527,    0,    0,    0,    0,    0,  867,
  527,  867,  867,    0,    0,  867,    0,    0,    0,  867,
    0,  251,  867,    0,  867,    0,  867,    0,    0,    0,
  504,  867,  867,    0,  867,    0,  868,  868,  868,  868,
  868,    0,  868,    0,  527,    0,  242,    0,  231,    0,
  867,  867,  504,  867,  868,  868,  868,  868,    0,  504,
    0,    0,  251,    0,    0,  251,    0,  867,    0,  867,
  867,  867,    0,    0,  867,    0,    0,  867,  867,    0,
  251,    0,    0,    0,    0,    0,  537,  867,  868,    0,
    0,    0,    0,  504,    0,    0,    0,    0,    0,    0,
    0,  867,  537,    0,  867,  537,  537,    0,  537,    0,
    0,    0,  537,    0,    0,  537,  527,    0,  527,    0,
  867,    0,  527,    0,    0,  527,  867,    0,  537,    0,
    0,  867,    0,    0,  537,    0,    0,  537,  527,    0,
    0,    0,  867,  867,    0,  867,  242,    0,    0,    0,
    0,  867,    0,    0,  678,    0,    0,  678,    0,    0,
    0,    0,    0,    0,  678,    0,    0,  678,  242,    0,
    0,  678,    0,    0,    0,  242,  678,    0,    0,  678,
  867,  867,    0,  867,    0,    0,    0,    0,    0,    0,
  867,  867,  867,  867,  867,  678,  867,    0,    0,  867,
  867,  867,  867,  867,    0,    0,  537,  867,  867,  242,
    0,  862,    0,    0,    0,    0,  527,    0,    0,  678,
    0,    0,    0,  678,    0,    0,    0,    0,  537,    0,
    0,    0,  678,    0,    0,  537,    0,    0,  527,  537,
    0,    0,    0,    0,    0,  527,  537,    0,    0,  678,
    0,    0,  862,    0,  862,  862,  862,  868,    0,  868,
  868,    0,    0,    0,  537,    0,    0,  868,    0,  537,
  862,  862,  862,  862,    0,    0,  678,    0,    0,  527,
    0,    0,    0,  868,    0,    0,    0,    0,  678,    0,
  678,    0,    0,    0,  537,    0,    0,  868,  537,  251,
    0,  251,  868,  868,  862,    0,  868,    0,  251,    0,
    0,    0,    0,  868,    0,  868,  868,    0,    0,  868,
    0,    0,    0,  868,    0,    0,  868,    0,  868,    0,
  868,    0,    0,    0,    0,  868,  868,    0,  868,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  538,    0,
    0,    0,    0,    0,  868,  868,    0,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  868,    0,  868,  868,  868,    0,    0,  868,    0,
    0,  868,  868,    0,    0,    0,    0,    0,  538,  538,
    0,  868,  538,    0,    0,    0,    0,    0,    0,  251,
    0,    0,    0,    0,    0,  868,    0,  538,  868,  538,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  251,    0,    0,  868,    0,    0,    0,  251,    0,
  868,    0,    0,    0,    0,  868,    0,    0,   98,  538,
    0,    0,    0,    0,    0,    0,  868,  868,    0,  868,
    0,    0,    0,    0,    0,  868,    0,    0,    0,    0,
    0,    0,  251,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  862,    0,  862,  862,    0,    0,   99,
    0,    0,    0,  862,  868,  868,    0,  868,    0,    0,
    0,  100,    0,    0,  868,  868,  868,  868,  868,  862,
  868,    0,    0,  868,  868,  868,  868,  868,    0,    0,
    0,  868,  868,  862,    0,  861,  101,    0,  862,  862,
    0,    0,  862,    0,    0,    0,    0,    0,    0,  862,
    0,  862,  862,    0,    0,  862,    0,    0,    0,  862,
    0,    0,  862,    0,  862,    0,  862,    0,    0,    0,
    0,  862,  862,    0,  862,    0,  861,    0,  861,  861,
  861,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  862,  862,    0,  862,  861,  861,  861,  861,    0,    0,
    0,    0,    0,  570,    0,  102,    0,  862,    0,  862,
  862,  862,    0,    0,  862,  103,    0,  862,  862,    0,
    0,    0,    0,    0,    0,    0,  538,  862,  861,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  104,  862,  538,    0,  862,  538,  538,    0,  538,    0,
    0,    0,  538,    0,    0,  538,    0,    0,  105,    0,
  862,    0,    0,    0,    0,    0,  862,    0,  538,    0,
    0,  862,    0,    0,  538,    0,    0,  538,    0,    0,
    0,    0,  862,  862,    0,  862,    0,  106,  107,    0,
  108,  862,    0,    0,    0,    0,    0,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0,    0,    0,
  862,  862,    0,  862,    0,    0,    0,    0,    0,    0,
  862,  862,  862,  862,  862,    0,  862,    0,    0,  862,
  862,  862,  862,  862,    0,    0,  538,  862,  862,    0,
    0,  863,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  538,    0,
    0,    0,    0,    0,    0,  538,  105,    0,    0,  538,
    0,    0,    0,    0,    0,    0,  538,    0,    0,    0,
    0,    0,  863,    0,  863,  863,  863,  861,    0,  861,
  861,    0,    0,    0,  538,  106,  107,  861,  108,  538,
  863,  863,  863,  863,    0,  109,  110,  111,  112,  113,
    0,  114,    0,  861,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,  538,    0,    0,  861,  538,    0,
    0,    0,  861,  861,  863,    0,  861,    0,    0,    0,
    0,    0,    0,  861,    0,  861,  861,    0,    0,  861,
    0,    0,    0,  861,    0,    0,  861,    0,  861,    0,
  861,    0,    0,    0,    0,  861,  861,    0,  861,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  800,    0,
    0,    0,    0,    0,  861,  861,    0,  861,    0,  776,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  861,    0,  861,  861,  861,    0,    0,  861,    0,
    0,  861,  861,    0,    0,  461,    0,    0,    0,  800,
    0,  861,  800,    0,    0,    0,    0,    0,    0,    0,
  776,    0,    0,  776,    0,  861,    0,  800,  861,    0,
    0,  105,    0,    0,    0,    0,    0,    0,  776,    0,
    0,    0,    0,    0,  861,    0,    0,    0,    0,    0,
  861,    0,    0,    0,    0,  861,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,  861,  861,    0,  861,
  109,  110,  111,  112,  113,  861,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,  863,    0,  863,  863,    0,    0,    0,
    0,    0,    0,  863,  861,  861,    0,  861,    0,    0,
    0,    0,    0,    0,  861,  861,  861,  861,  861,  863,
  861,    0,    0,  861,  861,  861,  861,  861,    0,    0,
    0,  861,  861,  863,    0,  866,    0,    0,  863,  863,
    0,    0,  863,    0,    0,    0,    0,    0,    0,  863,
    0,  863,  863,    0,    0,  863,    0,    0,    0,  863,
    0,    0,  863,    0,  863,    0,  863,    0,    0,    0,
    0,  863,  863,    0,  863,    0,  866,    0,  866,  866,
  866,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  863,  863,    0,  863,  866,  866,  866,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  863,    0,  863,
  863,  863,    0,    0,  863,  681,    0,  863,  863,    0,
    0,    0,    0,    0,    0,    0,    0,  863,  866,    0,
  800,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  863,    0,    0,  863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  681,    0,    0,  681,
  863,    0,    0,    0,    0,    0,  863,    0,    0,    0,
    0,  863,    0,    0,  681,    0,    0,    0,    0,    0,
    0,    0,  863,  863,    0,  863,  800,    0,    0,  800,
    0,  863,    0,    0,    0,    0,  800,  776,    0,  800,
  776,    0,    0,  800,    0,    0,    0,  776,  800,    0,
  776,  800,    0,    0,  776,    0,    0,    0,    0,  776,
  863,  863,  776,  863,    0,    0,    0,  800,  105,    0,
  863,  863,  863,  863,  863,    0,  863,    0,  776,  863,
  863,  863,  863,  863,    0,    0,    0,  863,  863,    0,
    0,  800,    0,    0,  800,  800,    0,  106,  107,    0,
  108,    0,  776,    0,  800,  776,  776,  109,  110,  111,
  112,  113,    0,  114,    0,  776,  115,  116,  117,  118,
  119,  800,    0,    0,  120,  121,    0,    0,    0,    0,
    0,    0,  776,    0,    0,    0,    0,  866,    0,  866,
  866,    0,    0,  800,    0,    0,    0,  866,  800,    0,
    0,    0,    0,    0,  776,    0,    0,    0,    0,  776,
  800,    0,  800,  866,    0,    0,    0,  660,    0,    0,
  421,  776,  422,  776,    0,    0,    0,  866,    0,    0,
    0,    0,  866,  866,    0,  424,  866,    0,    0,    0,
  279,    0,    0,  866,    0,  866,  866,    0,    0,  866,
    0,    0,    0,  866,    0,    0,  866,    0,  866,    0,
  866,    0,    0,    0,    0,  866,  866,    0,  866,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  866,  866,    0,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  866,    0,  866,  866,  866,    0,    0,  866,    0,
    0,  866,  866,    0,  660,    0,    0,  421,    0,  422,
    0,  866,    0,  681,    0,    0,  681,    0,    0,    0,
    0,    0,  424,  681,    0,  866,  681,  279,  866,    0,
  681,    0,    0,    0,    0,  681,    0,    0,  681,    0,
    0,    0,    0,    0,  866,    0,    0,    0,    0,    0,
  866,    0,    0,    0,  681,  866,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  866,  866,    0,  866,
    0,    0,    0,    0,    0,  866,    0,    0,  681,    0,
    0,    0,  681,    0,    0,    0,    0,    0,    0,    0,
    0,  681,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  866,  866,    0,  866,  681,    0,
    0,    0,    0,    0,  866,  866,  866,  866,  866,    0,
  866,    0,    0,  866,  866,  866,  866,  866,    0,    0,
    0,  866,  866,    0,  385,  681,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,    0,  681,    0,  681,
    0,    0,    0,    0,    0,  664,  387,    0,    0,    0,
    0,    0,  388,  389,    0,    0,    0,    0,    0,  801,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  654,    0,    0,  664,  664,    0,  664,    0,    0,
  390,    0,    0,  391,  664,  664,  664,  664,  664,    0,
  664,    0,    0,  664,  664,  664,  664,  664,    0,    0,
  801,  664,  664,  801,    0,    0,    0,    0,    0,    0,
    0,  393,  394,    0,    0,    0,    0,    0,  801,    0,
    0,  385,    0,  395,    0,    0,    0,  396,    0,    0,
  386,    0,    0,    0,    0,  884,    0,    0,    0,    0,
    0,    0,    0,  387,    0,    0,  653,    0,    0,  388,
  389,    0,  648,    0,  648,  648,    0,  648,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  654,    0,
  648,    0,    0,    0,  697,  648,    0,  390,  656,    0,
  391,    0,    0,    0,  397,    0,    0,    0,    0,    0,
  398,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,    0,    0,    0,    0,    0,  393,  394,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
  395,    0,    0,    0,  396,    0,    0,    0,  404,    0,
    0,    0,  655,  465,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
  105,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,    0,  115,  116,  117,  118,  119,
  885,  886,  887,  120,  121,  656,    0,    0,    0,  106,
  107,  397,  108,    0,    0,    0,    0,  398,    0,  109,
  110,  111,  112,  113,    0,  114,    0,  399,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,  801,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  657,  658,  659,    0,    0,    0,
  120,  121,  648,    0,    0,    0,    0,  801,    0,  648,
  801,    0,    0,    0,    0,    0,    0,  801,  648,    0,
  801,    0,    0,    0,  801,    0,    0,    0,    0,  801,
    0,  648,  801,    0,    0,    0,    0,  648,  648,    0,
    0,  642,    0,  642,  642,  648,  642,    0,  801,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  642,
    0,    0,    0,    0,  642,  648,    0,    0,  648,    0,
    0,    0,  801,    0,    0,  801,  801,    0,    0,    0,
    0,  825,    0,    0,    0,  801,    0,    0,    0,    0,
    0,    0,    0,    0,  648,    0,  648,  648,    0,    0,
    0,    0,  801,    0,    0,    0,    0,    0,  648,    0,
    0,    0,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  825,    0,  801,  825,    0,  660,    1,  801,
  421,    0,  422,    0,    0,    0,    0,    0,    0,    0,
  825,  801,    0,  801,    0,  424,    0,    0,    0,    2,
  279,    0,    0,    0,    0,    0,    0,    3,    0,    0,
    0,    0,    0,    0,    4,    0,    5,    0,    6,  648,
    0,    0, 1650,    7,    0,  648,    0,    0,    0,    0,
    8,    0,    0,    0,    0,  648,    0,    0,    0,    0,
    0,    0,    0,    0,    9,    0,    0, 1651,    0,  648,
  648,  648,  648,  648,  648,   10,    0,    0,    0,    0,
    0,    0,    0,  648,    0,    0,    0,    0,    0,    0,
    0,    0,  648,  648,  648,  648,  648,  648,  648,  648,
  648,  648,  648,    0,  648,    0,    0,  648,  648,    0,
  648,  648,  648,  648,  648,  648,  648,  648,    0,    0,
  648,  648,  648,  648,  648, 1652,    0,    0,  648,  648,
    0,  642,    0,    0,    0,    0,    0,    0,  642,    0,
    0,    0,   11,    0,    0,    0,    0,  642,    0,   12,
    0,    0,   13,    0,   14,   15,    0,    0,    0,  105,
  642,    0,    0,    0,    0,    0,  642,  642,    0,    0,
    0,    0,    0,    0,  642,    0,    0,    0,    0,   16,
    0,    0,    0,    0,    0,    0,    0,    0,  106,  107,
    0,  108,    0,    0,  642,    0,    0,  642,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,  117,
  118,  119,    0,  825,    0,  120,  121,    0,    0,    0,
    0,    0,    0,  642,  385,  642,  642,  660,   17,    0,
  421,    0,  422,  386,    0,    0,    0,  642,    0,    0,
    0,  642,    0,   18,   19,  424,  387,    0,    0,    0,
  279,    0,  388,  389,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  825,
    0,  654,  825,    0,    0,    0,    0,    0,    0,  825,
  390,    0,  825,  391,    0,    0,  825,    0,    0,    0,
    0,  825,    0,    0,  825,    0,    0,    0,  642,    0,
    0,    0,    0,    0,  642,    0,    0,    0,    0,    0,
  825,  393,  394,    0,  642,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  396,  642,  642,
  642,  642,  642,  642,  825,  880,    0,  825,  825,    0,
    0,    0,  642,    0,    0,    0,    0,  825,    0,    0,
    0,  642,  642,  642,  642,  642,  642,  642,  642,  642,
  642,  642,    0,  642,  825,    0,  642,  642,    0,  642,
  642,  642,  642,  642,  642,  642,  642,    0,  656,  642,
  642,  642,  642,  642,  397,    0,  825,  642,  642,    0,
  398,  825,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,  825,    0,  825,    0,    0,    0,    0,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,  385,  417,  418,  419,  111,  112,
  113,  420,  114,  386,    0,  115,  116,  657,  658,  659,
    0,    0,    0,  120,  121,  660,  387,    0,  421,    0,
  422,    0,  388,  389,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  424,    0,    0,    0,    0,  279,    0,
    0,  654,    0,    0,    0,    0,    0,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  942,  708,  706,    0,
  707,    0,  709,    0,    0,    0,    0,    0,    0,    0,
    0,  393,  394,    0,    0,  905,  904,  906,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  396,    0,    0,
  870,    0,    0,    0,  598,  655,    0,    0,    0,    0,
    0,    0,  485,    0,    0,  421,    0,  422,  871,    0,
    0,    0,  599,    0,    0,  600,    0,    0,    0,    0,
  424,    0,    0,    0,    0,  279,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  656,    0,
    0,    0,    0,    0,  397,    0,    0,    0,    0,    0,
  398,    0,    0,    0,    0,    0,  601,    0,    0,    0,
  399,    0,  602,  603,  604,    0,  605,  606,  607,    0,
  608,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
  609,    0,  610,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  611,    0,  115,  116,  657,  658,  659,
    0,    0,  385,  120,  121,    0,    0,    0,    0,    0,
  612,  386,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  387,    0,    0,    0,    0,    0,
  388,  389,  613,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  654,
    0,    0,    0,    0,    0,    0,    0,  892,  390,    0,
    0,  391,    0,    0,    0,  704,    0,  614,  615,    0,
  705,    0,    0,  893,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  616,    0,    0,    0,    0,  393,
  394,    0, 1122,  532,    0, 1123,    0,    0,    0,  385,
    0,  395,    0,    0,    0,  396,    0,    0,  386,  894,
    0,    0,    0,  880,    0,    0,    0,    0,  895,    0,
    0,  387,    0,    0,    0,    0,  896,  388,  389,    0,
  485,    0,    0,  421,  532,  422,    0,  532,    0,    0,
    0,    0,    0,    0,    0,  897,    0,  898,  424,    0,
    0,    0,  532,  279,  532,  390,  656,    0,  391,    0,
    0,  899,  397,  900,  901,  902,    0,    0,  398,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,    0,    0,    0,  532,    0,  393,  394,    0,    0,
    0,    0,  400,  401,  402,  403,  204,  205,  395,    0,
    0,    0,  396,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,    0,  903,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0, 1124,    0,  397,
    0,    0,    0,    0,    0,  398,  961,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,  485,    0,    0,  421,    0,  422,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
  424,    0,  961,  404,    0,  279,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
 1132,  532,    0, 1123,    0,    0,    0,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  386,  532,    0,    0,
  532,  532,    0,  532,    0,    0,  962,  532,    0,  387,
  532,  964,    0,    0,    0,  388,  389,    0,    0,    0,
    0,  485,    0,  532,  421,    0,  422,    0,  105,  532,
    0,    0,  532,    0,    0,    0,    0,    0,    0,  424,
    0,    0,  963,  390,  279,    0,  391,  964,    0,    0,
    0,    0,    0,  965,    0,    0,    0,  106,  107,    0,
  108,    0,    0,    0,  105,    0,    0,  109,  110,  111,
  112,  113,  966,  114,  393,  394,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,  395,    0,    0,    0,
  396,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,  532,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,  532,    0,    0,    0,    0,    0,    0,
  532,    0, 1136,    0,  532, 1123,    0,    0,    0,  385,
    0,  532,    0,    0,    0, 1124,    0,  397,  386,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,  532,
    0,  387,    0,  399,  532,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,  532,
    0,  404,    0,  532,    0,  390,    0,    0,  391,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  393,  394,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,  395,    0,
    0, 1144,  396,    0, 1123,    0,    0,    0,  385,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387,    0,    0,    0,    0,    0,  388,  389,    0,    0,
  485,    0,    0,  421,    0,  422,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1124,  424,  397,
    0,    0,    0,  279,  390,  398,    0,  391,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,  393,  394,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  395,    0,    0,
    0,  396,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0, 1433,    0,  120,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1124,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    6,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0, 1546, 1547,    0,
 1548,    0,  485,    0,    0,  421,    0,  422,  400,  401,
  402,  403,  204,  205,    0,    0,    0, 1549,    0,    0,
  424,    0,  404, 1550,    0,  279,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
 1147,    0,    0, 1123,    0,    0,    0,  385,    0,    0,
    0,    0,    0, 1551,    0,    0,  386,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  387,
    0,    0,   14,    0,    0,  388,  389,    0,    0,    0,
  570,  485,    0,    0,  421,    0,  422, 1552,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,  424,
    0,    0,    0,  390,  279,    0,  391,    0,    0,  570,
  570,    0,  570,    0,    0,  105,    0,    0,    0,  570,
  570,  570,  570,  570,    0,  570,    0,    0,  570,  570,
  570,  570,  570,    0,  393,  394,  570,  570,    0,    0,
    0,    0,    0,    0, 1553,  107,  395,  108,    0,    0,
  396,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115, 1554,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1152,    0,    0, 1123,    0,    0,    0,  385,
    0,    0,    0,    0,    0, 1124,    0,  397,  386,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,    0,
    0,  387,    0,  399,    0,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,  390,    0,    0,  391,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  393,  394,  115,  116,
  117,  118,  119,    0,  804,    0,  120,  121,  395,    0,
    0, 1155,  396,    0, 1123,    0,    0,    0,  385,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387,    0,    0,    0,    0,  804,  388,  389,  804,    0,
  485,    0,    0,  421,    0,  422,    0,    0,    0,    0,
    0,    0,    0,  804,    0,    0,    0, 1124,  424,  397,
    0,    0,    0,  279,  390,  398,    0,  391,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,  393,  394,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  395,    0,    0,
    0,  396,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1124,    0,  397,    0,
    0,    0,    0,    0,  398,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,  804,  120,  121,    0,
 1158,    0,    0, 1123,    0,    0,    0,  385,    0,    0,
  485,    0,  490,  421,    0,  422,  386,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  424,  387,
    0,    0,    0,  279,    0,  388,  389,    0,  708,  706,
    0,  707,    0,  709,    0,    0,    0,    0,    0,    0,
    0,    0,  804,    0,    0,  804,  905,  904,  906,    0,
    0,    0,  804,  390,    0,  804,  391,    0,    0,  804,
    0,    0,    0,    0,  804,    0,    0,  804,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  804,  393,  394,    0,    0,    0,    0,
    0,    0,    0,  281,    0,    0,  395,    0,  485,    0,
  396,  421,    0,  422,    0,    0,    0,  804,    0,    0,
  804,  804,    0,    0,    0,    0,  424,    0,    0,    0,
  804,  279,  281,  281,    0,  281,    0,    0,    0,    0,
    0,    0,  281,  281,  281,  281,  281,  804,  281,    0,
    0,  281,  281,  281,  281,  281,    0,    0,    0,  281,
  281,    0,    0,    0,    0, 1124,    0,  397,    0,  804,
    0,    0,    0,  398,  804,    0,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,  804,    0,  804,    0,
    0,    0,    0,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,  385,  417,  418,
  419,  111,  112,  113,  420,  114,  386,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,  387,
    0,    0,    0,    0,    0,  388,  389,    0,  892,  802,
    0,    0,    0,    0,    0,    0,  704,    0,    0,    0,
    0,  705,    0,    0,  893,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  802,    0,    0,  802,    0,    0,    0,    0,    0,    0,
  894,    0,  392,    0,  393,  394,    0,    0,  802,  895,
    0,    0,    0,    0,    0,  385,  395,  896,    0,    0,
  396,    0,    0,    0,  386,    0,    0,  968,    0,  968,
  968,    0,  968,    0,    0,    0,  897,  387,  898,    0,
    0,    0,    0,  388,  389,  968,    0,    0,    0,    0,
  968,    0,  899,    0,  900,  901,  902,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  390,    0,    0,  391,    0,    0,  397,    0,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  399,    0,    0,    0,    0,    0,    0,
  392,    0,  393,  394,    0,  903,    0,  400,  401,  402,
  403,  204,  205,    0,  395,    0,    0,    0,  396,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,  697,  120,  121,    0,    0,
    0,    0,    0,    0,    0,  397,    0,    0,    0,  648,
    0,  398,  648,    0,  648,    0,    0,    0,    0,    0,
    0,  399,    0,    0,    0,    0,    0,  648,    0,    0,
    0,    0,  648,    0,    0,  400,  401,  402,  403,  204,
  205,  802,    0,    0,    0,    0,    0,    0,    0,  404,
    0,    0,    0,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,  968,    0,  115,  116,  117,  118,
  119,    0,    0,  968,  120,  121,    0,  802,    0,    0,
  802,    0,    0,    0,  803,    0,  968,  802,    0,    0,
  802,    0,  968,  968,  802,    0,    0,    0,    0,  802,
    0,    0,  802,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  802,    0,
  968,    0,    0,  968,    0,  803,    0,    0,  803,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  802,  803,    0,  802,  802,    0,    0,  968,
    0,  968,  968,    0,    0,  802,    0,    0,    0,    0,
    0,    0,    0,  968,  833,    0,    0,  968,    0,    0,
    0,    0,  802,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  802,    0,    0,    0,    0,  802,
    0,    0,    0,    0,    0,  833,    0,    0,  833,  648,
    0,  802,    0,  802,    0,    0,  648,    0,    0,    0,
    0,    0,    0,  833,  968,  648,    0,    0,  423,    0,
  968,  421,    0,  422,    0,    0,    0,    0,  648,    0,
  968,    0,    0,    0,  648,  648,  424,    0,    0,    0,
    0,  279,  648,    0,  968,  968,  968,  968,  968,  968,
    0,    0,    0,    0,    0,    0,    0,    0,  968,    0,
    0,    0,  648,    0,    0,  648,    0,  968,  968,  968,
  968,  968,  968,  968,  968,  968,  968,  968,    0,  968,
    0,    0,  968,  968,    0,  968,  968,  968,  968,  968,
  968,  968,  968,  648,  648,  968,  968,  968,  968,  968,
    0,    0,    0,  968,  968,  648,    0,    0,    0,  648,
    0,    0,    0,    0,    0,  485,    0,    0,  421,    0,
  422,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  424,    0,    0,  803,    0,  279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  648,    0,    0,    0,
    0,    0,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  803,    0,    0,  803,  648,  648,  648,  648,
  648,  648,  803,    0,    0,  803,    0,    0,    0,  803,
  648,    0,    0,    0,  803,    0,  833,  803,    0,  648,
  648,  648,  648,  648,  648,  648,  648,  648,  648,  648,
    0,  648,    0,  803,  648,  648,    0,  648,  648,  648,
  648,  648,  648,  648,  648,  385,    0,  648,  648,  648,
  648,  648,    0,    0,  386,  648,  648,  803,    0,    0,
  803,  803,    0,    0,    0,    0,    0,  387,    0,    0,
  803,   98,  833,  388,  389,  833,    0,    0,    0,    0,
    0,    0,  833,    0,    0,  833,    0,  803,    0,  833,
    0,    0,    0,    0,  833,    0,    0,  833,    0,    0,
    0,  390,    0,    0,  391,    0,    0,    0,    0,  803,
    0,    0,   99,  833,  803,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  100,    0,  803,    0,  803,    0,
  392,    0,  393,  394,    0,    0,    0,  833,  134,    0,
  833,  833,  385,    0,  395,    0,    0,    0,  396,  101,
  833,  386,    0,    0,  485,    0,    0,  421,    0,  422,
    0,    0,    0,    0,  387,    0,    0,  833,    0,    0,
  388,  389,  424,    0,    0,    0,    0,  279,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  833,
    0,    0,    0,    0,  833,    0,    0,    0,  390,    0,
    0,  391,    0,    0,    0,  397,  833,    0,  833,    0,
    0,  398,    0,    0,    0,    0,    0,    0,  102,    0,
    0,  399,    0,    0,    0,    0,    0,    0,  103,  393,
  394,    0,    0,    0,    0,  400,  401,  402,  403,  204,
  205,  395,    0,    0,    0,  396,    0,    0,    0,  404,
    0,    0,    0,  104,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,  105,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0,    0,    0,
  106,  107,  397,  108,    0,    0,    0,    0,  398,    0,
  109,  110,  111,  112,  113,    0,  114,    0,  399,  115,
  116,  117,  118,  119,    0,    0,  689,  120,  121,    0,
    0,    0,  400,  401,  402,  403,  204,  205,    0,    0,
    0,    0,    0,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,  385,    0,  115,  116,  117,  118,  119,    0,    0,
  386,  120,  121,  485,    0,    0,  421,    0,  422,    0,
    0,    0,    0,  387,    0,    0,    0,    0,    0,  388,
  389,  424,    0,    0,    0,    0,  279,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  390,    0,    0,
  391,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  393,  394,
    0,    0,    0,  485,  945,    0,  421,    0,  422,    0,
  395,    0,    0,    0,  396,    0,    0,    0,    0,    0,
    0,  424,    0,    0,  105,    0,  279,    0,    0,    0,
    0,    0,    0,  164,  165,  166,  167,  168,  169,  170,
  171,  172,  173,  174,  175,  176,  177,  178,  179,    0,
    0,  180,  181,  106,  107,    0,  108,    0,    0,  182,
  183,  697,    0,  109,  110,  111,  112,  113,  184,  114,
  185,  397,  115,  116,  117,  118,  119,  398,    0,    0,
  120,  121,    0,    0,    0,    0,    0,  399,    0,    0,
    0,  221,    0,    0,    0,  221,    0,    0,    0,    0,
    0,  400,  401,  402,  403,  204,  205,    0,    0,  221,
    0,    0,    0,  221,    0,  404,  221,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
  385,    0,  115,  116,  117,  118,  119,  221,    0,  386,
  120,  121,    0,  221,  221,  221,    0,  221,  221,  221,
    0,  221,  387,    0,    0,    0,    0,    0,  388,  389,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,    0,  221,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  221,    0,    0,    0,    0,   80,
  385,    0,    0,    0,    0,  392,    0,  393,  394,  386,
    0,  221,  485,    0,    0,  421,    0,  422,    0,  395,
    0,    0,  387,  396,    0,    0,    0,    0,  388,  389,
  424,    0,    0,  221,    0,  279,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0,    0,    0,  221,  221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  397,    0,    0,    0,    0,  221,  398,  393,  394,    0,
    0,    0,    0,    0,    0,    0,  399,    0,    0,  395,
    0,    0,    0,  396,    0,    0,    0,    0,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
    0,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414, 1433,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
  397,  115,  116,  117,  118,  119,  398,    0,    0,  120,
  121,    0,    0,    0,    6,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0, 1546, 1547,    0, 1548,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
    0,    0,    0,    0,  404, 1549,    0,    0,    0,    0,
    0, 1550,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,  385,
    0,  115,  116,  117,  118,  119,    0,    0,  386,  120,
  121,  485,    0,    0,  421,    0,  422,    0,    0,    0,
    0,  387,    0,    0,    0,    0,    0,  388,  389,  424,
    0, 1551,    0,    0,  279,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0, 1552,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1678,    0,  393,  394,    0,    0,
    0,  485,    0,  105,  421,    0,  422,    0,  395,    0,
    0,    0,  396,    0,    0,    0,    0,    0,    0,  424,
    0,    0,    0,    0,  279,    0,    0,    0,    0,    0,
    0,    0, 1553,  107,    0,  108,    0,    0,    0,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115, 1554,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,    0,    0,    0,    0,  397,
    0,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,  221,
    0,    0,    0,  221,    0,    0, 1150,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,  221,    0,    0,
    0,  221,    0,  404,  221,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  385,    0,
  115,  116,  117,  118,  119,  221,    0,  386,  120,  121,
    0,  221,  221,  221,    0,  221,  221,  221,    0,  221,
  387,    0,    0,    0,    0,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,  221,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,  385,    0,
    0,    0,    0,    0,    0,  393,  394,  386,    0,  221,
  485,    0,    0,  421,    0,  422,    0,  395,    0,    0,
  387,  396,    0,    0,    0,    0,  388,  389,  424, 1721,
    0,  221,    0,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,    0,    0,    0,    0,  221,  221, 1335,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  397,    0,
    0,    0,    0,  221,  398,  393,  394,    0,    0,    0,
    0,    0,    0,    0,  399,    0,    0,  395,    0,    0,
    0,  396,    0,    0,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,  397,  115,
  116,  117,  118,  119,  398,    0,    0,  120,  121,    0,
    0,    0,    0,    0,  399,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1348,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,  385,    0,  115,
  116,  117,  118,  119,    0,    0,  386,  120,  121,  485,
    0,    0,  421,    0,  422,    0,    0,    0,    0,  387,
    0,    0,    0,    0,    0,  388,  389,  424,    0,    0,
    0,    0,  279,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,  394,    0,    0,    0,  968,
    0,    0,  968,    0,  968,    0,  395,    0,    0,    0,
  396,    0,    0,    0,  597,    0,    0,  968,  598,    0,
    0,    0,  968,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  599,    0,    0,  600,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  397,    0,    0,
    0,    0,    0,  398,    0,    0,    0,    0,    0,    0,
  601,    0,    0,  399,    0,    0,  602,  603,  604,    0,
  605,  606,  607,    0,  608,    0,    0,  400,  401,  402,
  403,  204,  205,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,  609,    0,  610,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  385,  611,  115,  116,
  117,  118,  119,    0,    0,  386,  120,  121,    0,    0,
    0,    0,    0,    0,  612,    0,    0,    0,  387,    0,
    0,    0,    0,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  613,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  554,    0,    0,    0,
    0,  614,  615,    0,    0,    0,  968,    0,  544,    0,
    0,    0,    0,  393,  394,  968,    0,    0,  616,    0,
  529,    0,    0,    0,    0,  395,    0,    0,  968,  396,
    0,    0,    0,    0,  968,  968,    0,  554,    0,    0,
  554,    0,    0,    0,    0,    0,    0,  797,    0,  544,
    0,    0,  544,    0,    0,  554,    0,  554,    0,    0,
    0,  529,  968,    0,  529,  968,    0,  544,    0,  544,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  529,
    0,  529,    0,    0,    0,    0,  397,  554,  797,    0,
    0,  797,  398,  968,  968,    0,    0,    0,    0,  544,
    0,    0,  399,    0,    0,  968,  797,    0,    0,  968,
    0,  529,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,  968,  115,  116,  117,
  118,  119,  968,    0,    0,  120,  121,    0,    0,    0,
    0,    0,  968,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  968,  968,  968,  968,
  968,  968,    0,    0,    0,    0,    0,    0,    0,    0,
  968,    0,    0,    0,    0,    0,    0,    0,    0,  968,
  968,  968,  968,  968,  968,  968,  968,  968,  968,  968,
    0,  968,    0,    0,  968,  968,    0,  968,  968,  968,
  968,  968,  968,  968,  968,    0,    0,  968,  968,  968,
  968,  968,    0,    0,  554,  968,  968,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  544,    0,    0,    0,
  554,    0,    0,    0,  554,    0,  554,    0,  529,    0,
  554,    0,  544,  554,    0,    0,  544,    0,  544,  841,
    0,    0,  544,    0,  529,  544,  554,    0,  529,    0,
  529,    0,  554,    0,  529,  554,    0,  529,  544,  797,
    0,    0,    0,    0,  544,    0,    0,  544,    0,    0,
  529,    0,    0,    0,    0,    0,  529,    0,    0,  529,
  841, 1007, 1007,  841, 1007, 1007, 1007,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  841, 1007,
 1007, 1007,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  797,    0,    0,  797,    0,
    0,    0,    0,    0,  554,  797,    0,    0,  797,    0,
 1007,    0,  797,    0,    0,    0,  544,  797,    0,    0,
  797,    0,    0,    0,    0,    0,  554,    0,  529,    0,
    0,    0,    0,  554,    0,    0,  797,  554,  544,    0,
    0,    0,    0,    0,  554,  544,    0,    0,    0,  544,
  529,    0,    0,    0,    0,    0,  544,  529,    0,    0,
  797,  529,  554,  797,  797,    0,    0,  554,  529,    0,
    0,    0,    0,  797,  544,    0,    0,    0,    0,  544,
  842,    0,    0,    0,  133,    0,  529,    0,  133,    0,
  797,  529,  554,    0,    0,    0,  554,    0,    0,    0,
    0,    0,    0,    0,  544,    0,  133,    0,  544,  133,
    0,    0,  797,    0,    0,    0,  529,  797,    0,    0,
  529,  842, 1008, 1008,  842, 1008, 1008, 1008,    0,  797,
    0,  797,    0,    0,    0,    0,    0,    0,    0,  842,
 1008, 1008, 1008,    0,    0,    0,    0,    0,    0,    0,
  133,    0,    0,    0,    0,    0,  133,  133,  133,    0,
  133,  133,  133,    0,  133,    0,    0,    0,    0,    0,
    0, 1008,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  841,    0,    0,  133,    0,  133,    0,    0,    0,
    0, 1007,    0,    0,    0,    0,    0,    0,    0, 1007,
    0,    0,    0,    0, 1007,    0,    0, 1007,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  133,    0,    0,
    0,    0,    0,  843,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  133,    0,    0,  841,    0,    0,
  841,    0,    0, 1007,    0,    0,    0,  841,    0,    0,
  841,    0, 1007,    0,  841,    0,  133,    0,    0,  841,
 1007,    0,  841,    0,  843, 1009, 1009,  843, 1009, 1009,
 1009,    0,    0,    0,    0,    0,    0,    0,  841, 1007,
    0, 1007,  843, 1009, 1009, 1009,  798,    0,    0,    0,
    0,  133,  133,    0,    0, 1007,    0, 1007, 1007, 1007,
    0,    0,  841,    0,    0,  841,  841,    0,  133,    0,
    0,    0,    0,    0, 1009,  841,    0,    0,    0,    0,
    0,    0,  799,    0,    0,    0,    0,  798,    0,    0,
  798,    0,  841,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,    0,    0,  798,    0,    0, 1007,    0,
    0,    0, 1008,    0,  841,    0,    0,    0,  832,  841,
 1008,    0,    0,  799,    0, 1008,  799,    0, 1008,    0,
    0,  841,    0,  841,    0,    0,    0,    0,    0,    0,
    0,  799,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  826,    0,    0,    0,  842,  832,
    0,  842,  832,    0, 1008,    0,    0,    0,  842,    0,
    0,  842,    0, 1008,    0,  842,    0,  832,    0,    0,
  842, 1008,    0,  842,    0,    0,    0,    0,    0,    0,
  834,    0,    0,    0,    0,  826,    0,    0,  826,  842,
 1008,    0, 1008,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  826,    0,    0, 1008,    0, 1008, 1008,
 1008,    0,    0,  842,    0,    0,  842,  842,    0,    0,
    0,  834,    0,    0,  834,    0,  842,    0,    0,    0,
    0,    0,    0,    0,    0,  843,    0,    0,    0,  834,
    0,    0,    0,  842,    0, 1009,    0,    0,    0,    0,
    0,    0,  835, 1009,    0,    0,    0,    0, 1009, 1008,
    0, 1009,    0,    0,    0,  842,    0,    0,    0,    0,
  842,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,    0,  842,    0,    0,    0,    0,    0,
    0,  843,    0,  835,  843,    0,  835, 1009,  798,    0,
    0,  843,    0,    0,  843,    0, 1009,    0,  843,    0,
    0,  835,    0,  843, 1009,    0,  843,    0,    0,    0,
    0,    0,    0,    0,  823,    0,    0,    0,    0,    0,
    0,    0,  843, 1009,  799, 1009,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1009,
    0, 1009, 1009, 1009,  798,    0,  843,  798,    0,  843,
  843,    0,    0,    0,  798,  823,    0,  798,  823,  843,
  832,  798,    0,    0,    0,    0,  798,    0,    0,  798,
    0,    0,    0,  823,    0,    0,  843,    0,    0,    0,
  799,    0,    0,  799,    0,  798,    0,    0,    0,    0,
  799,    0, 1009,  799,    0,    0,  826,  799,  843,    0,
    0,    0,  799,  843,    0,  799,    0,    0,    0,  798,
    0,    0,  798,  798,    0,  843,  832,  843,    0,  832,
    0,  799,  798,    0,    0,    0,  832,  827,    0,  832,
    0,    0,  834,  832,    0,    0,    0,    0,  832,  798,
    0,  832,    0,    0,    0,  799,    0,    0,  799,  799,
    0,    0,  826,    0,    0,  826,    0,  832,  799,  836,
    0,  798,  826,    0,    0,  826,  798,    0,  827,  826,
    0,  827,    0,    0,  826,  799,    0,  826,  798,    0,
  798,  832,    0,    0,  832,  832,  827,    0,  834,    0,
    0,  834,    0,  826,  832,    0,    0,  799,  834,    0,
  836,  834,  799,  836,  835,  834,    0,    0,    0,    0,
  834,  832,    0,  834,  799,    0,  799,  826,  836,    0,
  826,  826,    0,    0,    0,    0,    0,    0,    0,  834,
  826,    0,    0,  832,    0,    0,    0,    0,  832,    0,
    0,    0,    0,  824,    0,    0,    0,  826,    0,    0,
  832,    0,  832,  834,    0,    0,  834,  834,    0,    0,
  835,    0,    0,  835,    0,    0,  834,    0,    0,  826,
  835,    0,  828,  835,  826,    0,  823,  835,    0,    0,
    0,    0,  835,  834,  824,  835,  826,  824,  826,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  835,  824,    0,    0,  834,    0,    0,    0,    0,
  834,    0,    0,  828,    0,    0,  828,    0,    0,    0,
    0,    0,  834,    0,  834,  835,    0,    0,  835,  835,
    0,  828,  823,    0,    0,  823,    0,    0,  835,    0,
    0,    0,  823,    0,    0,  823,    0,    0,    0,  823,
    0,    0,    0,    0,  823,  835,    0,  823,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  823,    0,    0,    0,  835,    0,    0,
    0,    0,  835,    0,    0,    0,    0,    0,    0,  827,
    0,    0,    0,    0,  835,    0,  835,  823,    0,    0,
  823,  823,    0,    0,    0,    0,    0,    0,    0,    0,
  823,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  836,    0,    0,    0,    0,    0,  823,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  827,    0,    0,  827,  823,
    0,    0,    0,    0,  823,  827,    0,    0,  827,    0,
    0,    0,  827,    0,    0,    0,  823,  827,  823,    0,
  827,    0,    0,    0,    0,    0,    0,  836,    0,    0,
  836,    0,    0,    0,    0,    0,  827,  836,    0,    0,
  836,    0,    0,    0,  836,    0,    0,    0,    0,  836,
    0,    0,  836,    0,    0,  824,    0,    0,    0,    0,
  827,    0,    0,  827,  827,    0,    0,    0,  836,    0,
    0,    0,    0,  827,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  828,    0,    0,    0,    0,    0,
  827,    0,  836,    0,    0,  836,  836,    0,    0,    0,
    0,    0,    0,    0,    0,  836,    0,    0,    0,    0,
    0,  824,  827,    0,  824,    0,    0,  827,    0,    0,
    0,  824,  836,    0,  824,    0,    0,    0,  824,  827,
    0,  827,    0,  824,    0,    0,  824,    0,    0,    0,
  828,    0,    0,  828,  836,    0,    0,    0,    0,  836,
  828,    0,  824,  828,  321,    0,    0,  828,    0,    0,
    0,  836,  828,  836,    0,  828,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  824,    0,    0,  824,
  824,  828,    0,    0,  321,    0,    0,    0,    0,  824,
    0,  321,    0,    0,    0,  321,  321,    0,  321,    0,
    0,  321,    0,    0,    0,  828,  824,    0,  828,  828,
    0,    0,    0,    0,    0,  321,    0,    0,  828,    0,
    0,  321,    0,    0,    0,    0,    0,    0,  824,    0,
  597,    0,    0,  824,  598,  828,    0,    0,    0,    0,
    0,  797,    0,    0,    0,  824,    0,  824,    0,    0,
    0,    0,  599,    0,    0,  600,    0,  828,    0,    0,
    0,    0,  828,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  828,    0,  828,    0,    0,    0,
    0,  321,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  601,    0,    0,    0,
  321,    0,  602,  603,  604,    0,  605,  606,  607,    0,
  608,    0,    0,    0,    0,  321,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  321,    0,    0,    0,    0,
  609,  597,  610,    0,  321,  598,  321,    0,    0,    0,
    0,    0,    0,  321,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  599,    0,    0,  600,    0,    0,    0,
    0,    0,    0,  611,    0,    0,    0,    0,    0,    0,
    0,    0,  321,  321,    0,  321,    0,    0,    0,    0,
  612,    0,  321,  321,  321,  321,  321,    0,  321,    0,
    0,  321,  321,  321,  321,  321,    0,  601,    0,  321,
  321,    0,  613,  602,  603,  604,    0,  605,  606,  607,
    0,  608,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  609,    0,  610,    0,    0,    0,  614,  615,    0,
    0,    0,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,    0,  616,    0,  109,  110,  111,  112,
  113,    0,  114,    0,  611,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,  612,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  613,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  614,  615,
    0,    0,    0,    0,    0,    0,    0,  106,  107,    0,
  108,    0,    0,    0,    0,  616,    0,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,  453,    0,    0,  120,  121,    0,    0,    0,    0,
    0,  453,  453,  453,  453,  453,  453,  453,  453,  453,
  453,  453,  453,  453,  453,  453,    0,    0,  453,  453,
  453,  453,    0,  453,    0,    0,  453,  453,    0,    0,
  453,  453,  453,  453,  453,  453,  453,  453,    0,  453,
  453,  453,  453,  453,    0,    0,    0,  453,  453,
};
static short yycheck[] = {                                       9,
   16,   55,   12,    0,    4,  151,   16,   64,   18,  256,
  104,   63,  287,  689,  359,   56,  140,  141,  515,  140,
  141,   63,  334,   91,  498,   62,   67,  363,   85,   57,
  556,  363,   94,  559,  423,   89,  989,  355,  565,  565,
  712,   69,  332,   53,   54,   55,   56,   57,   58,   90,
  522,  677,  772,   63,   64,   65,   97,   67, 1266,   69,
   70, 1038,   72,   73, 1045,  368,   73,   83, 1045,   88,
  473,   81,   70,   83,   84,   85,   92,   87,   88,   89,
   90,  697,   92,   93,   94,   95,  758,   97,   95,  131,
   68,  133,  154,  380,  711,   93,  264,  730,  642, 1038,
  765, 1046,  649,  281,  132,  273, 1045,    0, 1038, 1038,
  726,  152, 1057,   91,    0, 1045, 1045, 1038,  941,    0,
    0,  131,  132,  133, 1045,  153,  145,    0,  327,  794,
 1071,    0, 1077,   63,  228,  145,  762,   44,  803, 1094,
  864,  151,  152,  153,  154,  308,   44,  768,  158,    0,
    0,   59,  189,    0,    0,   41,  979,   41,   44,   41,
   44,  730,  308,    0,    0,   42,   59,   41,   41,  655,
   45,   44,   41,   59,   45,   44,  423,  214,   59,   59,
    0,  989,    0,   44,   44,   44,   59,  532,   40,  833,
   59,   41,  857,  274,   44,   41,   40,   61,   44,    0,
 1384,    0,   41,   45,   44,   44,    0, 1224,   59,   59,
    0, 1314,   59,   59,  517, 1399,  258,   40,   44,   63,
   44,  315,   59,   59,  542,  543,  544,  545,    0,  565,
    0,  907,  908,  565,  262, 1264,    0,   44,  292,   59,
    0,   59,  739,  271,    0,   40,  219,  592,  258,   45,
   45,  283,  262,  264,  264,  265,   40,  283,   59,   45,
   59,  271,  938,  273, 1089,   59,  308,   63,  296,   59,
  318,   41, 1095,   44,   44, 1473,  320,   40,  568,   63,
   44,  479,  292,  301,    0,   41,  296,   59,   41,   59,
    0,  333,  313,   12,  820,   59, 1212,  465,  340,   59,
  342,  262,  344,   59,   91,  347,    0,   41,  271,  355,
  597, 1292,   41,   41,  445, 1292, 1201,  257, 1203,  356,
  338,  358,  376,  333,  365,  318,  377,  473,  618,    0,
  340,   41,  342,  257,  344,  372, 1281,  347,  311,  972,
 1208,   44, 1425,   59,  263,  273,  374, 1012,  455,   59,
  493,  984,  985, 1292,  980,  365,  366, 1298,  384,    0,
  503, 1037, 1292, 1292,  374,   59,  376,  841,  366,    0,
   41, 1292,  262,    0, 1477,    0,  268,  508,    0,   41,
 1588,  301,    0,  260,  158,  466,   12, 1404,   59, 1472,
   93, 1056,  436,  312,  334, 1311,  417,  448,  884,  279,
  411,   44,  677,  972,  323,  453,  320,  439,  258, 1438,
  334, 1236,  340,  439,  424,  984,   41, 1225,   59,  138,
  300,  271,  320,  464,  442,  386,  303,    0,   59,  279,
  456,  473,   59,  461,   59,  274,  300,   59,  479, 1115,
 1116,   59, 1118, 1119,  472,  318,  492,  694,  476,  329,
  300,  461,   41,  481,  464,  465,  306,  264,  308,  583,
  453,  279,  472,  710,  304,  740,  476,  340,  348,  479,
  309,  481,  318,  365,  484,    0,  402,  357, 1099,  329,
  304, 1689,  300,  363, 1429,  331,   59,  762,   41, 1434,
  277, 1436,  782,  340,  340,  785,  386,  383,  348, 1697,
  537,  381,  410,  340,  340,  387,  269,  357,  427,  799,
 1127,  329,  138,  363,  396, 1470,  584, 1402,  481,  279,
  340, 1241,  442,  565,  318, 1709, 1471,  411,  458,  328,
  348,  381,  331,  262,   59,  733,  826, 1745,  356,  357,
  300,  387,  394, 1484, 1088,  363,  340,  440,  318,  586,
  396,  458,  855,  721,  564,  565,  868, 1104,  736,  331,
  570,  454,  318,  381,  328, 1209,  440,  331,  418,  329,
  340, 1247,  411,  454,  977,  393,  436,  395, 1786,  343,
 1304,  454,  781,  458,  340,  436, 1219,  458,  348,  562,
  877,  452, 1537, 1269,  440,  454,  759,  357, 1543,  641,
  418,  638,  639,  363, 1412,    0,  584,  454,  454,  410,
  485,  301,  383,  759,  458,  501,  458,  454,  454,  409,
  744,  381,    0,  264,  447,  443, 1451,  466,  454,  502,
    0,  641,  501,  264,  387,  431,  431,  517, 1303,  273,
  650,  440,  264, 1287, 1469,  431,   41,  309, 1593,   44,
  650,  367,  386,  777,  778,  454, 1524,  386,  386,  632,
  454,  680,  458,  458,   59, 1610, 1489,  517,  440,   41,
  680,  713,  458,  367,  458,  318,  440,  387,  405,   41,
    0,   59,  454,  286,  454,   41,  396,  161,  452,   59,
  454,  260,  733,  318,  704,  506,    0,  879,  454,  517,
    0,  883,  712,  713, 1424,  980,  716, 1025,   41,  262,
  484,  721,  780,  262,    0,  340,  387,  759,  357, 1335,
  730,  348,  764,  733,   44,  753,  768,  755,  738,  456,
  348,  262,  742,    0,  771,  772,  773, 1718, 1414,   59,
    0, 1718,  331,  753,  277,  755, 1699,   41,  758,  791,
  818, 1696,  442, 1043,  764,   59,  397,  517,  768,   59,
    0, 1706,  387, 1038,    0, 1055,  397,    0,    0,  342,
 1045,  396, 1090,   59, 1400,  397,  356,  819,  820, 1718,
    0,  791,  325,  851,  852,  853, 1076,   59, 1718, 1718,
 1102,  260,   59,  961,  831, 1267,  964, 1718,  387,   59,
 1189,   41,  780,  328, 1749,   41,  331,  396,   44,  819,
  820,    0,  345,    0,   16,  440, 1290,    0,  348,   59,
    0,   41, 1296,   59,   44, 1259,   59,   59,  856,  454,
 1611,  977,  405,  386, 1223,  408, 1363,  386,  866,   59,
  818,  307,  308,    0,  409,  319,  856,  825,  417,  429,
    0,  424,   42, 1165,    0,  386,  866,   47,  461,  462,
  838,   41,  338,  352,   44,  300,    0,    0,  878,  287,
   59, 1305,   59,  851,  852,  853,   59,  450,  517,   59,
  258,   83,  300,  278,   41,  280,  435,  430,  258,  378,
  262,   41,  287,  271,  437,   41, 1179,  265,   44, 1182,
  262,  271,   59,  965,  435,  348,  262,   41,   41,   59,
   44,  655,  319,   59,  357,  440,  660,  352, 1699,   42,
   43,  962,   45,  261,   47,   59,   59,  452,  306,  454,
  308,  461,  462,    0,  302,  977,  306, 1212,  308,  278,
    0,  806, 1189,  402, 1643,  405, 1645,  387,  417,  458,
 1074,  961,  962,  963,  964,  965,  966,  279,  260,  966,
  264, 1712,  972, 1308,  264,  963,  442,  260,  262,  337,
    0, 1752, 1648,  259,  984,  985, 1223,  308,  264,  989,
  319,  308, 1416,  378,  270,  451,  445,  318,  383, 1279,
 1666,  318,   59,  400,    0,  328,  456,  264,  331,   59,
  440,  303,  458,  405,  264,  400, 1705,  329,  328,  322,
  303,  331,  407,    0,  386, 1083, 1797, 1292,  401, 1545,
 1310,    0,  424,  343,  386, 1522, 1699, 1363, 1525,   59,
  386, 1363,  264, 1101,  441,   41, 1311,  402,   44, 1107,
  418,  363,  278,  417,  280,  278,  441,  280,  418,  508,
 1092,  287, 1803,   59,  387,   61, 1066, 1099,  278,  381,
  280,  400,   41,  396, 1701, 1741,  377,  287, 1068,    0,
  444, 1081,   59,  438,  517,  264,    0,  264,  318, 1752,
   59,  264, 1092,  258,  264,  322, 1207, 1208, 1209, 1099,
  280,  331,  386,  397, 1377,  285,  271,  397, 1381,   40,
  340, 1800,  441, 1421, 1776, 1083,  501,  418,    0,   42,
   43,  397,   45, 1750,   47, 1612,  259,   41,  264, 1243,
  440, 1245,  453, 1101, 1797, 1400,  453,  270,   59, 1107,
  397,  306,  452,  308,  454,   59,  880,  397,  318,  383,
  884,  321,  378,    0,  278,  378,  280,  387,  328,   41,
 1440,  331,  383,  287,  465,  335,  396,  280,  378, 1796,
  340,  318,  285,  343,  400,  397, 1600,   59,  318, 1757,
 1758,  407, 1606, 1849,  320, 1710, 1711,    0,  328,  359,
  400,  331, 1306,  340, 1049, 1050,    0,  407,    0, 1479,
  340, 1201, 1829, 1203,  385,  328,  307,  308,  331,  458,
  440,  265,   59,  383, 1241,  441,  343,  387,  397, 1219,
  397,  278,    0,  280,  454, 1225,  396,  340,  278, 1754,
  280,  441,  465,    0,  467,   44,  485,    0,  258,  378,
  387,  368,  301,  413, 1099, 1518,   59,  387,  302,  396,
   59,  271,  342,  418,  378,   59,  396,   59, 1563,  279,
  258,  400,  258,  321,  387,   44,    0,    0,  407, 1756,
  440, 1407, 1850,  396,   44,  501,  400,  335,  274,  338,
  300,   59,  452,  407,  454,  343,  306,  377,  308,   59,
  460,  501,   59,  440, 1321, 1285,   59, 1433, 1325,  426,
  440,  359,  441,  383,  300, 1830,  397,  454,  306,  329,
  306,   44,  452,  309,  454,  405,  452,  441,  408,  489,
  490,   44,  492,    0,  460,   59,   59, 1607,  348,  499,
  500,  501,  502,  503,  424,  505,  356,  357,  508,  509,
  510,  511,  512,  363,    0,  436,  516,  517,  407,  318,
  451,    0,    0,  489,  490,  413,  492,  280,  417,  383,
  450,  381,  285,  499,  500,  501,  502,  503,  436,  505,
   93,  340,  508,  509,  510,  511,  512,  501,    0,  470,
  516,  517,   59,  442,  494,   41,  238,  403,   44,    0,
 1432, 1527,   41,   41,  246,  318,   44, 1424,  418,  337,
 1432, 1433, 1402,   59,  420,   61,   44, 1407,  272,  429,
   59,   59, 1412, 1524,  260,  411,  418,  331,  387,    0,
  418,  342,  418, 1441,  288,  301, 1540, 1563,  318,  356,
  256,    0, 1432, 1433,   44,    0,  318,   59,  436,   40,
  436, 1441,   42,   43,  274,   45,  328,   47,   59,  331,
  321,  321,  278, 1718,  258,   93,  377,  303,  340,   59,
   41,  287,  338,   44,  335,  335,  393,  271,  395,  333,
  466,  440,  470,  387,  470,  279, 1781,    0,   59,    0,
  258,  321,  396,   93,  405,  454, 1723,  408,  359,  359,
   59,  418,  318,  271,   59,  335,  300,  517,  362, 1546,
  259,  279,  306,  424,  308,  387,  424, 1591,    0,  402,
 1815,  270,  383,  383,  396,    0,  443,   40,   41,  359,
   41,   44,  300,   44, 1201,  329, 1203, 1527,  306,  450,
  308, 1563,   42,   43,   44,   45,   59,   47,   59, 1675,
 1845,  434,  413,  413,  348,    0, 1546,  260, 1592, 1854,
  280,  329, 1857,  357, 1554,  285,  449,   40,  440,  363,
 1865,   42,   43, 1563,   45,  429,   47,   59,    0, 1587,
  348,  264,  454,  413,   59,  342,    0,  381,    0,  357,
  316,  317, 1582,   42,   43,  363,   45, 1587,   47, 1616,
  303,  417, 1592, 1620,    0,  421,  422,  423,  258, 1801,
 1802,  261,  258,  381,   59, 1611, 1690,    0,  342,   41,
  377, 1611,   44,    0,  418,   42,   43,  461,   45,  316,
   47,  277,  278,    0,  280,  260,  318,   59,  284,    0,
 1661,  287,    0,  337, 1770,   59,    0,   59,  405, 1775,
  418,  408,  417,  377,  300, 1781,  306,  336, 1784,  294,
  306,    0,  460,   59,  279,    0,  321,  424, 1860,  485,
  305, 1661,   40,  489,  452,  342,   59,  312,  303,  444,
  335,  405,   59,  282,  408,  300, 1812,    0,  343, 1815,
  280,  290,   59,  450,    0,  285, 1733,  402,   59,  340,
  424,   59,  337, 1699,  359,   59, 1683,   40,   40, 1699,
  377, 1837,   42,   43,  329,   45, 1842,   47,    0, 1845,
   59, 1717, 1712,  517,   59,    0,  450, 1717, 1854,    0,
    0, 1857,  378,  348,  320,    0,  307,  308,  405, 1865,
  318,  408,  357, 1733, 1734,   40,   59,    0,  363,  517,
  340,    0,  383,   59,  400, 1735, 1752,  424,  413, 1781,
  402,  407, 1752,   58,  383, 1761,  381,  402,  493,  494,
  416, 1761,  418,  461,  462,  260, 1810,   59,   41,   41,
  280,   44,   44,  450,   59,  285, 1776,  506,   59,   59,
  436, 1781,  337, 1815,   59,  441,  436,   41,  361,  434,
   44, 1797,  450,  438,  434,  318,   59, 1797,  321,  280,
   59, 1807,  384, 1803,  285,  328,  301, 1807,  331,  417,
 1810,  442,  335, 1845,  470, 1815,  417,  340,  313,  374,
  343,  280, 1854,  287,   41, 1857,  285,   44,   41,  410,
  454,   44,  387, 1865,  660, 1835,  359,  337,   41,  320,
  516,   44,  516,  338,   41, 1845,  278,   44,  280,  391,
  392,  677,  311,  280, 1854,  681, 1856, 1857,  285,  460,
  383, 1861,  688,   46,  387, 1865,   41,  693,  694,   44,
  451,  461,  462,  396,  374,   58,   93,  703,   61,  705,
  706,  707,  708,  709,  710,  711,  411,  387,  489,  490,
  413,  492,  517,  320,  469,   44,   93,   61,  499,  500,
  501,  502,  503,  372,  505,  286,  284,  508,  509,  510,
  511,  512,  407,   40,  740,  516,  517,  440,   46,   40,
   46,   40,  417,  457,  458,  459,  264,   40,   40,  452,
   58,  454,   58,   61,  461,   61,  762,  460,   40,   40,
  280,  767,  258,   40,   40,  285,  378,  442,   40,   40,
  461,  485,  486,    0,  676,  271,  678,  461,   40,    0,
  682,  683,  684,  279,   40,   40,  489,  490,  400,  492,
   41,  311,   40,   44,   44,  407,  499,  500,  501,  502,
  503,   91,  505,   46,  300,  508,  509,  510,  511,  512,
  306,  436,  308,  516,  517,   41,  320,   44,   44,   40,
   41,   42,   43,   44,   45,   46,   47,   41,   41,  441,
   44,   44,   59,  329,   41,   44,  402,   44,   59,   60,
   61,   62,   42,   43,  320,   45,   44,   47,  264,   41,
   42,   43,  348,   45,   41,   47,   41,   44,   44,   44,
   41,  357,   41,   44,  428,   44,   46,  363,   42,   43,
   91,   45,   93,   47,   41,   42,   43,   61,   45,   41,
   47,   41,   44,   40,   44,  381,  892,  893,  894,  264,
   44,  897,  898,  899,  417,  901,  902,  903,  904,  905,
  906,  382,  460,  909,  910,  505,  912,  348,  914,   44,
  348,  917,  918,  919,  920,  921,  922,  348,  278,  278,
   40,  300,  418,  264,  397,  436,  397,  470,  934,  277,
  936,  489,  490,  348,  492,  447,  399,  460,  447,  458,
   40,  499,  500,  501,  502,  503,  349,  505,   40,   91,
  508,  509,  510,  511,  512,   40,   40,   40,  516,  517,
  318,  264,  383,  411,   44,  389,  489,  490, 1675,  492,
  382,   40,   40,  451,  980,  460,  499,  500,  501,  502,
  503,   41,  505,  317,  464,  508,  509,  510,  511,  512,
  436,   42,   40,  516,  517,  320,   41,   42,   43,  320,
   45,  452,   47,  448,  489,  490,  264,  492,  502,  458,
   40,  277,   40,  274,  499,  500,  501,  502,  503,  284,
  505,  517,  300,  508,  509,  510,  511,  512,  344,  344,
  386,  516,  517,    0,   42,   43,  278,   45,  418,   47,
  386,  262,  416,  264,  265,  897,  386,  899,  300,  901,
  902,  272,  904,  905,  906,   40,   42,  418,  356,  280,
  417,   40,  274, 1770,  285,  265,  417,  288, 1775,   40,
  264,   41,   44,  320,   41,   44,  280, 1784,   44,  367,
  280,  302,   44,  385,  320,  285,  307,  308,  280,   40,
  311,   44,   59,  285,  321,  328,   44,  318,  266,  320,
  321,  328,  302,  324,  331, 1812,  280,  328,  335,   40,
  331,  285,  333,  280,  335,  501,  343,  299,  285,  340,
  341, 1127,  343,   41,   42,   43,   40,   45,  264,   47,
 1837,  422,  359, 1139, 1140, 1842, 1142, 1143,  359,  360,
   41,  362,   58,   41, 1150,  472,  473,  474,  475,  476,
  477,  478,  479,    0,   41,  376,  383,  378,  379,  380,
   41,  264,  383, 1169,  394,  386,  387,  454,  309,  492,
  440, 1177, 1178, 1179,   41,  396, 1182,  309, 1184,   41,
   40,   40, 1188, 1189, 1190, 1191,  413,  352,   40,  410,
   40,  418,  413,  308,   41,  262,   41,   42,   43,   40,
   45,   40,   47,   41,   42,   43, 1212,   45,  429,   47,
   40,   40,   59,  440,  435,   40,   40, 1223,   40,  440,
   40,   40,   40,   40,   40,  452,  327,  454,   41,   44,
  451,  452,  435,  454,   44,  280,  388,  388,  388,  460,
  285,  343,  274,  331,  422,  296,   41,   42,   43,  446,
   45,  390,   47,  391,  262,  271, 1262,   41,   42,   43,
  287,   45,  407,   47,  386,  386,   41,  386,  489,  490,
    0,  492,  280,  386,  386,  386,   41,  285,  499,  500,
  501,  502,  503,  378,  505,  278,   40,  508,  509,  510,
  511,  512,   41,   41,  417,  516,  517,   41,   42,   43,
   41,   45,   41,   47,  308, 1311,  435,  385,  436,  396,
  301,   41,   42,   43,   44,   45,   46,   47,   41,   42,
   43,  280,   45,  337,   47,  442, 1332,  338,  442,   59,
   60,   61,   62,   42,   43, 1341,   45,  338,   47,   41,
  460,  301, 1348,   41,   42,   43,   41,   45,   45,   47,
   59,  318,  352,   41,   42,   43,  320,   45,  371,   47,
  385,   91,  280,   93,  331,  371,   41,  285,   41,  489,
  490, 1377,  492,  340,   41, 1381,  460,   41,   44,  499,
  500,  501,  502,  503,   41,  505,   41,   41,  508,  509,
  510,  511,  512,   41, 1400,   58,  516,  517,   41,   42,
   43,   41,   45,   41,   47,  489,  490, 1413,  492,  460,
   41,   41,   41,   41,   41,  499,  500,  501,  502,  503,
  387,  505, 1770,   41,  508,  509,  510,  511,  512,  396,
   41,   40,  516,  517,   59,  280, 1784,  340,  489,  490,
  285,  492,  280,   59,   59,   59,   59,  285,  499,  500,
  501,  502,  503,   59,  505,  340,   59,  508,  509,  510,
  511,  512,  308,   58, 1812,  516,  517,   41,  352,  344,
   40,  318,  368,  440,  460,   59,   41,   42,   43,   40,
   45,  328,   47,  451,  331,  280,   40,  454,   44, 1837,
  285, 1497, 1498,  340, 1842,  383,  280,   59,   46,   59,
  340,  285,   41,  489,  490,  320,  492,  383, 1514,  383,
  343,   41, 1518,  499,  500,  501,  502,  503,   41,  505,
   44,  410,  508,  509,  510,  511,  512,  435,  301,  442,
  516,  517,  262,  387,  264,  265,  280,   40,   59,  463,
  387,  285,  272,  304,   44, 1551,  293,  307,  304,  396,
  280,   44, 1675,   59,   41,  285,    0,  280,  288,  337,
  383,   40,  285,  264,  278,  264,  458,  320,  389,  383,
  264,  280,  302,  470,  304,  411,  285,  307,  308,    0,
  451,  311,  280,   44,   40,  309,  458,  285,  318,  271,
  320,  321,  280,  440,  324,  264,   41,  285,  328,  436,
  435,  331,  458,  333,   41,  335,  343,  454,  343, 1615,
  340,  341,  343,  343,  343,  271,   40, 1623,   41,   59,
   41,   42,   43,   44,   45,   46,   47,   40,   59,  359,
  360,  308,  362,   41,   59,   41,  304,  280,   59,   60,
   61,   62,  285, 1649,   59,  460,  376, 1770,  378,  379,
  380,    0, 1775,  383, 1660,   41,  386,  387, 1664,    0,
    0, 1784, 1204,  273,   12,  266,  396,  265,  721,   59,
   91, 1104,   93,  869,  489,  490, 1102,  492,   87, 1099,
  410, 1091,  645,  413,  499,  500,  501,  502,  503, 1812,
  505, 1460, 1459,  508,  509,  510,  511,  512,  330,  429,
  360,  516,  517,  323, 1028,  435,  592,   41,   42,   43,
  440,   45, 1081,   47, 1837,  280,  313, 1723,  764, 1842,
  285,  451,  452,  342,  454,   41,   42,   43, 1092,   45,
  460,   47, 1658,   41,   42,   43,  801,   45,  342,   47,
 1582, 1438, 1743,  570, 1440, 1747, 1746,   42,   43, 1788,
   45,  791,   47, 1260, 1723, 1554, 1739,    0, 1680,  489,
  490,  458,  492,  460, 1856, 1782, 1417, 1699, 1014,  499,
  500,  501,  502,  503, 1238,  505,  347, 1783,  508,  509,
  510,  511,  512,  335,  572,  328,  516,  517, 1294,  848,
  840,   13,  489,  490,  500,  492,  300,  460,   41,  631,
  504,   44,  499,  500,  501,  502,  503,  753,  505,  767,
  143,  508,  509,  510,  511,  512,   59,  740,   61,  516,
  517, 1807, 1649,  989, 1535, 1761,  489,  490, 1861,  492,
  515, 1141,  981,   -1, 1184,  729,  499,  500,  501,  502,
  503,  262,  505,  264,  265,  508,  509,  510,  511,  512,
 1249,  272,   -1,  516,  517,   -1,   -1,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,   45,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   41,   -1,   -1,   44,  340,
  341,   -1,  343,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   59,   -1,   61,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  460,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,  280,  386,  387,  262,   -1,  285,
   -1,   -1,  280,   -1,   91,  396,   93,  285,   -1,   -1,
   -1,   -1,   -1,  489,  490,  280,  492,   -1,   -1,  410,
  285,   -1,  413,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  429,   -1,
  516,  517,   -1,   -1,  435,  258,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,  277,  278,   -1,  280,   -1,  460,
   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
   -1,   -1,   -1,  306,   -1,   -1,   -1,   -1,  489,  490,
  460,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  258,   -1,   -1,  378,   -1,   -1,   -1,  295,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,  278,   -1,  280,  272,   -1,  400,  284,   -1,
   -1,  287,   -1,  280,  407,   -1,   -1,  323,  285,   -1,
   -1,  288,   -1,   -1,  300,  418,   -1,   -1,   -1,   -1,
  306,    0,   -1,   -1,   -1,  302,   -1,    0,   -1,   -1,
  307,  308,   -1,  436,  311,   -1,  352,   -1,  441,   -1,
   -1,  318,   -1,  320,  321,  287,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   40,   41,  340,  341,   44,  343,  470,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,
   59,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,  427,   -1,   -1,  400,   -1,   -1,   -1,   91,  396,
   93,  407,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,  418,  410,   -1,   -1,  413,    0,   -1,   -1,
   -1,  457,  458,  459,   -1,  461,  462,   -1,   -1,   -1,
  436,   -1,  429,   -1,   -1,  441,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  480,  481,  482,  483,  484,  485,
  486,  487,  488,   -1,  451,  452,   -1,  454,   41,  495,
  496,   44,   -1,  460,  470,   -1,   -1,   -1,  504,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  460,   -1,
   -1,    0,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   41,  505,   -1,   44,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1, 1333, 1334,
   59,   -1,   -1,   -1, 1339, 1340,   -1, 1342, 1343,  262,
   -1,  264,  265,   -1,   -1, 1350, 1351, 1352, 1353,  272,
 1355, 1356, 1357, 1358, 1359, 1360,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  302,
   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,  318,
   -1,   -1,  321,   -1,   -1,  318,   -1,  320,  321,  328,
   -1,  324,  331,   -1,   -1,  328,  335,   -1,  331,   -1,
  333,  340,  335,   -1,  343,   -1,   41,  340,  341,   44,
  343,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
  359,   -1,   -1,   -1,   59,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,  376,  383,  378,  379,  380,  387,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,  396,   93,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  413,   -1,   -1,  410,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,  318,  429,   -1,  321,   -1,
   -1,  440,  435,   -1,   -1,  328,   -1,  440,  331,   -1,
   -1,   41,  335,  452,   44,  454,   -1,  340,  451,  452,
  343,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   59,
   -1,   41,   -1,   -1,   44,   -1,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
  383,   -1,   -1,  386,  387,   -1,  499,  500,  501,  502,
  503,   -1,  505,  396,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  321,  516,  517,   -1,   -1,   -1,   -1,  328,
  413,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
  359,   -1,    0,   -1,   -1,   -1,    0,   -1,   -1,  452,
  305,  454,   -1,  262,   -1,  264,  265,  312,   -1,   -1,
   -1,  316,  317,  272,  383,   -1,   -1,   -1,   -1,   -1,
  325,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,  337,   41,   -1,   -1,   44,   41,   -1,   -1,
   44,   -1,   -1,  302,  413,    0,   -1,   -1,  307,  308,
   -1,   59,  311,  318,   -1,   59,  321,   -1,   -1,  318,
   -1,  320,  321,  328,   -1,  324,  331,   -1,   -1,  328,
  335,  440,  331,   -1,  333,  340,  335,   -1,  343,   -1,
   -1,  340,  341,  452,  343,  454,   41,   42,   43,   44,
   45,   -1,   47,   -1,  359,   -1,   -1,  402,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,  419,   -1,   -1,  376,  383,  378,
  379,  380,  387,   -1,  383,   -1,   -1,  386,  387,  434,
   -1,  396,   -1,  438,   -1,   -1,   -1,  396,   93,   -1,
   -1,   -1,   -1,   -1,  449,   -1,   -1,   -1,  413,   -1,
   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,  318,   -1,
   -1,   -1,   -1,  468,   -1,   -1,   -1,   -1,  328,   -1,
  429,  331,   -1,    0,   -1,  440,  435,   -1,  318,   -1,
  340,  440,   -1,  343,   -1,   -1,   -1,  452,  328,  454,
   -1,  331,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,
  340,  460,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,  387,   -1,   -1,
  489,  490,   59,  492,   -1,   -1,  396,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  387,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  396,  516,  517,   -1,
   -1,    0,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  278,   -1,  280,   -1,  278,   -1,  280,   -1,   -1,  287,
  440,   59,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,  300,   -1,   -1,   -1,
  440,   -1,   41,   42,   43,   44,   45,  262,   47,  264,
  265,   -1,  452,   -1,  454,   -1,   -1,  272,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,   -1,  294,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,  305,   -1,
   -1,   -1,  307,  308,   93,  312,  311,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,  325,  324,
  378,   -1,   -1,  328,  378,   -1,  331,   -1,  333,   41,
  335,   -1,   44,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,  400,   -1,   -1,   -1,  400,   59,    0,  407,
   -1,   -1,   -1,  407,  359,  360,    0,  362,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,  386,
   -1,  386,  387,  441,   -1,   -1,   -1,  441,   40,   41,
   -1,  396,   44,   -1,   -1,  402,   -1,   41,   -1,   41,
   44,  278,   44,  280,   -1,  410,   -1,   59,  413,   61,
  287,   -1,  419,   -1,   -1,   59,   -1,   59,   -1,   -1,
   -1,   -1,   -1,  300,  429,   -1,   -1,  434,   -1,   -1,
  435,  438,   -1,   -1,  441,  440,   -1,   -1,   -1,   91,
   -1,   -1,  449,   -1,   -1,   -1,  451,  452,   -1,  454,
  278,   -1,  280,   -1,   -1,  460,  284,   -1,   -1,  287,
   -1,  468,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  300,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,
  505,  378,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  302,   -1,    0,   -1,   -1,  307,  308,
   -1,   -1,  311,  400,   -1,   -1,   -1,   -1,   -1,  318,
  407,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,    0,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
  378,  340,  341,   -1,  343,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,  441,   -1,  278,   -1,  280,   -1,
  359,  360,  400,  362,   59,   60,   61,   62,   -1,  407,
   -1,   -1,   41,   -1,   -1,   44,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,
   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  410,  274,   -1,  413,  277,  278,   -1,  280,   -1,
   -1,   -1,  284,   -1,   -1,  287,  278,   -1,  280,   -1,
  429,   -1,  284,   -1,   -1,  287,  435,   -1,  300,   -1,
   -1,  440,   -1,   -1,  306,   -1,   -1,  309,  300,   -1,
   -1,   -1,  451,  452,   -1,  454,  378,   -1,   -1,   -1,
   -1,  460,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,  400,   -1,
   -1,  335,   -1,   -1,   -1,  407,  340,   -1,   -1,  343,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  359,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  378,  516,  517,  441,
   -1,    0,   -1,   -1,   -1,   -1,  378,   -1,   -1,  383,
   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,  400,   -1,
   -1,   -1,  396,   -1,   -1,  407,   -1,   -1,  400,  411,
   -1,   -1,   -1,   -1,   -1,  407,  418,   -1,   -1,  413,
   -1,   -1,   41,   -1,   43,   44,   45,  262,   -1,  264,
  265,   -1,   -1,   -1,  436,   -1,   -1,  272,   -1,  441,
   59,   60,   61,   62,   -1,   -1,  440,   -1,   -1,  441,
   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,  466,   -1,   -1,  302,  470,  278,
   -1,  280,  307,  308,   93,   -1,  311,   -1,  287,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   40,   41,
   -1,  396,   44,   -1,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,   -1,   -1,   -1,  410,   -1,   59,  413,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  400,   -1,   -1,  429,   -1,   -1,   -1,  407,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,  260,   91,
   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  301,
   -1,   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,
   -1,  313,   -1,   -1,  499,  500,  501,  502,  503,  288,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  302,   -1,    0,  338,   -1,  307,  308,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  287,   -1,  407,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,  417,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  258,  396,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  442,  410,  274,   -1,  413,  277,  278,   -1,  280,   -1,
   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,  460,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,  300,   -1,
   -1,  440,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,
   -1,   -1,  451,  452,   -1,  454,   -1,  489,  490,   -1,
  492,  460,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  378,  516,  517,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,
   -1,   -1,   -1,   -1,   -1,  407,  460,   -1,   -1,  411,
   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   43,   44,   45,  262,   -1,  264,
  265,   -1,   -1,   -1,  436,  489,  490,  272,  492,  441,
   59,   60,   61,   62,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  288,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  466,   -1,   -1,  302,  470,   -1,
   -1,   -1,  307,  308,   93,   -1,  311,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,  434,   -1,   -1,   -1,   41,
   -1,  396,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  410,   -1,   59,  413,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,  451,  452,   -1,  454,
  499,  500,  501,  502,  503,  460,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  288,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  302,   -1,    0,   -1,   -1,  307,  308,
   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,  452,   -1,  454,  318,   -1,   -1,  321,
   -1,  460,   -1,   -1,   -1,   -1,  328,  318,   -1,  331,
  321,   -1,   -1,  335,   -1,   -1,   -1,  328,  340,   -1,
  331,  343,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
  489,  490,  343,  492,   -1,   -1,   -1,  359,  460,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  359,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,  489,  490,   -1,
  492,   -1,  383,   -1,  396,  386,  387,  499,  500,  501,
  502,  503,   -1,  505,   -1,  396,  508,  509,  510,  511,
  512,  413,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  435,   -1,   -1,   -1,  272,  440,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
  452,   -1,  454,  288,   -1,   -1,   -1,   40,   -1,   -1,
   43,  452,   45,  454,   -1,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   58,  311,   -1,   -1,   -1,
   63,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  396,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   58,  328,   -1,  410,  331,   63,  413,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,  383,   -1,
   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  413,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,  267,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  314,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
  323,   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   41,  516,  517,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   -1,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  292,   -1,   -1,  295,
  296,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,
   58,   -1,   -1,   -1,  417,   63,   -1,  323,  421,   -1,
  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  378,  443,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
  460,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
  513,  514,  515,  516,  517,  421,   -1,   -1,   -1,  489,
  490,  427,  492,   -1,   -1,   -1,   -1,  433,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  443,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,   -1,   -1,   -1,   -1,  318,   -1,  267,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,  276,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,  289,  343,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   40,   -1,   42,   43,  303,   45,   -1,  359,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  323,   -1,   -1,  326,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,  435,   44,   -1,   40,  261,  440,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,  452,   -1,  454,   -1,   58,   -1,   -1,   -1,  282,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,
   -1,   -1,   -1,   -1,  297,   -1,  299,   -1,  301,  427,
   -1,   -1,  343,  306,   -1,  433,   -1,   -1,   -1,   -1,
  313,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  327,   -1,   -1,  368,   -1,  457,
  458,  459,  460,  461,  462,  338,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,  426,   -1,   -1,  516,  517,
   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,  405,   -1,   -1,   -1,   -1,  276,   -1,  412,
   -1,   -1,  415,   -1,  417,  418,   -1,   -1,   -1,  460,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  442,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  323,   -1,   -1,  326,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  262,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  352,  267,  354,  355,   40,  491,   -1,
   43,   -1,   45,  276,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,  506,  507,   58,  289,   -1,   -1,   -1,
   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,  314,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,
  323,   -1,  331,  326,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
  359,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,  458,
  459,  460,  461,  462,  383,  378,   -1,  386,  387,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,  413,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  421,  508,
  509,  510,  511,  512,  427,   -1,  435,  516,  517,   -1,
  433,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   40,  289,   -1,   43,   -1,
   45,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  273,   -1,   -1,   -1,  277,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  291,   -1,
   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,
  443,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  406,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  267,  516,  517,   -1,   -1,   -1,   -1,   -1,
  423,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,  323,   -1,
   -1,  326,   -1,   -1,   -1,  280,   -1,  480,  481,   -1,
  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  497,   -1,   -1,   -1,   -1,  354,
  355,   -1,  260,    0,   -1,  263,   -1,   -1,   -1,  267,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,  324,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,  333,   -1,
   -1,  289,   -1,   -1,   -1,   -1,  341,  295,  296,   -1,
   40,   -1,   -1,   43,   41,   45,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,  362,   58,   -1,
   -1,   -1,   59,   63,   61,  323,  421,   -1,  326,   -1,
   -1,  376,  427,  378,  379,  380,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   91,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  429,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,  328,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   58,   -1,  328,  471,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  260,  258,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,  274,   -1,   -1,
  277,  278,   -1,  280,   -1,   -1,  402,  284,   -1,  289,
  287,  443,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   40,   -1,  300,   43,   -1,   45,   -1,  460,  306,
   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,  438,  323,   63,   -1,  326,  443,   -1,   -1,
   -1,   -1,   -1,  449,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  460,   -1,   -1,  499,  500,  501,
  502,  503,  468,  505,  354,  355,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  366,   -1,   -1,   -1,
  370,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  378,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,
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
  315,   -1,   40,   -1,   -1,   43,   -1,   45,  457,  458,
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
   -1,   -1,   -1,  323,   63,   -1,  326,   -1,   -1,  489,
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
  510,  511,  512,   -1,    0,   -1,  516,  517,  366,   -1,
   -1,  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   41,  295,  296,   44,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,  425,   58,  427,
   -1,   -1,   -1,   63,  323,  433,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
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
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  262,  516,  517,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   40,   -1,   42,   43,   -1,   45,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,
   -1,   -1,   -1,   63,   -1,  295,  296,   -1,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   60,   61,   62,   -1,
   -1,   -1,  328,  323,   -1,  331,  326,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,  366,   -1,   40,   -1,
  370,   43,   -1,   45,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
  396,   63,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  413,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,  435,
   -1,   -1,   -1,  433,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,
  500,  501,  502,  503,  504,  505,  276,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,  272,    0,
   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,  352,   -1,  354,  355,   -1,   -1,   59,  333,
   -1,   -1,   -1,   -1,   -1,  267,  366,  341,   -1,   -1,
  370,   -1,   -1,   -1,  276,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,  360,  289,  362,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,  376,   -1,  378,  379,  380,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,  429,   -1,  457,  458,  459,
  460,  461,  462,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  417,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,
   -1,  433,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,
  462,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,  516,  517,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,    0,   -1,  289,  328,   -1,   -1,
  331,   -1,  295,  296,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,
  323,   -1,   -1,  326,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   59,   -1,  386,  387,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,    0,   -1,   -1,  370,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  260,
   -1,  452,   -1,  454,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  427,  276,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
  443,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,  303,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  262,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,  457,  458,  459,  460,
  461,  462,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  471,   -1,   -1,   -1,  340,   -1,  262,  343,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,  359,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
  396,  260,  318,  295,  296,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  413,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,  435,
   -1,   -1,  301,  359,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  313,   -1,  452,   -1,  454,   -1,
  352,   -1,  354,  355,   -1,   -1,   -1,  383,  327,   -1,
  386,  387,  267,   -1,  366,   -1,   -1,   -1,  370,  338,
  396,  276,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,  289,   -1,   -1,  413,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,  417,  354,
  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,  460,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  489,  490,  427,  492,   -1,   -1,   -1,   -1,  433,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  443,  508,
  509,  510,  511,  512,   -1,   -1,  451,  516,  517,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  460,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,  469,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,  483,  484,   -1,
   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,  417,   -1,  499,  500,  501,  502,  503,  504,  505,
  506,  427,  508,  509,  510,  511,  512,  433,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,  291,
   -1,   -1,   -1,  295,   -1,  471,  298,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,  339,   -1,  276,
  516,  517,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,  411,
  267,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  276,
   -1,  423,   40,   -1,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  497,  433,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  271,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  427,  508,  509,  510,  511,  512,  433,   -1,   -1,  516,
  517,   -1,   -1,   -1,  301,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,  332,   -1,   -1,   -1,   -1,
   -1,  338,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,  516,
  517,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,  398,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,   -1,  354,  355,   -1,   -1,
   -1,   40,   -1,  460,   43,   -1,   45,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,
   -1,   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,  291,   -1,   -1,
   -1,  295,   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,  339,   -1,  276,  516,  517,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,
   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,  276,   -1,  423,
   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,
  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,   59,
   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  417,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  497,  433,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  427,  508,
  509,  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,
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
   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,  273,   -1,   -1,   58,  277,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
  339,   -1,   -1,  443,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,  406,  508,  509,
  510,  511,  512,   -1,   -1,  276,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,  267,   -1,    0,   -1,
   -1,   -1,   -1,  354,  355,  276,   -1,   -1,  497,   -1,
    0,   -1,   -1,   -1,   -1,  366,   -1,   -1,  289,  370,
   -1,   -1,   -1,   -1,  295,  296,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   41,
   -1,   -1,   44,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   41,  323,   -1,   44,  326,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,  427,   91,   41,   -1,
   -1,   44,  433,  354,  355,   -1,   -1,   -1,   -1,   91,
   -1,   -1,  443,   -1,   -1,  366,   59,   -1,   -1,  370,
   -1,   91,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
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
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  258,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,
  274,   -1,   -1,   -1,  278,   -1,  280,   -1,  258,   -1,
  284,   -1,  274,  287,   -1,   -1,  278,   -1,  280,    0,
   -1,   -1,  284,   -1,  274,  287,  300,   -1,  278,   -1,
  280,   -1,  306,   -1,  284,  309,   -1,  287,  300,  262,
   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,
  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,   -1,   -1,  378,  328,   -1,   -1,  331,   -1,
   91,   -1,  335,   -1,   -1,   -1,  378,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,  400,   -1,  378,   -1,
   -1,   -1,   -1,  407,   -1,   -1,  359,  411,  400,   -1,
   -1,   -1,   -1,   -1,  418,  407,   -1,   -1,   -1,  411,
  400,   -1,   -1,   -1,   -1,   -1,  418,  407,   -1,   -1,
  383,  411,  436,  386,  387,   -1,   -1,  441,  418,   -1,
   -1,   -1,   -1,  396,  436,   -1,   -1,   -1,   -1,  441,
    0,   -1,   -1,   -1,  273,   -1,  436,   -1,  277,   -1,
  413,  441,  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  466,   -1,  295,   -1,  470,  298,
   -1,   -1,  435,   -1,   -1,   -1,  466,  440,   -1,   -1,
  470,   41,   42,   43,   44,   45,   46,   47,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  423,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,  445,   -1,   -1,  340,
  341,   -1,  343,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,    0,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,  497,   -1,
   -1,   -1,   -1,   -1,   91,  396,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   59,   -1,   -1,  429,   -1,
   -1,   -1,  272,   -1,  435,   -1,   -1,   -1,    0,  440,
  280,   -1,   -1,   41,   -1,  285,   44,   -1,  288,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  318,   41,
   -1,  321,   44,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   59,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   59,
   -1,   -1,   -1,  413,   -1,  272,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  280,   -1,   -1,   -1,   -1,  285,  429,
   -1,  288,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   41,  321,   -1,   44,  324,  262,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   59,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,  262,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,  318,   -1,  383,  321,   -1,  386,
  387,   -1,   -1,   -1,  328,   41,   -1,  331,   44,  396,
  262,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   59,   -1,   -1,  413,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,  359,   -1,   -1,   -1,   -1,
  328,   -1,  429,  331,   -1,   -1,  262,  335,  435,   -1,
   -1,   -1,  340,  440,   -1,  343,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,  452,  318,  454,   -1,  321,
   -1,  359,  396,   -1,   -1,   -1,  328,    0,   -1,  331,
   -1,   -1,  262,  335,   -1,   -1,   -1,   -1,  340,  413,
   -1,  343,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,  318,   -1,   -1,  321,   -1,  359,  396,    0,
   -1,  435,  328,   -1,   -1,  331,  440,   -1,   41,  335,
   -1,   44,   -1,   -1,  340,  413,   -1,  343,  452,   -1,
  454,  383,   -1,   -1,  386,  387,   59,   -1,  318,   -1,
   -1,  321,   -1,  359,  396,   -1,   -1,  435,  328,   -1,
   41,  331,  440,   44,  262,  335,   -1,   -1,   -1,   -1,
  340,  413,   -1,  343,  452,   -1,  454,  383,   59,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  396,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  413,   -1,   -1,
  452,   -1,  454,  383,   -1,   -1,  386,  387,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,  396,   -1,   -1,  435,
  328,   -1,    0,  331,  440,   -1,  262,  335,   -1,   -1,
   -1,   -1,  340,  413,   41,  343,  452,   44,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,   59,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  383,   -1,   -1,  386,  387,
   -1,   59,  318,   -1,   -1,  321,   -1,   -1,  396,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,  413,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,   -1,  452,   -1,  454,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  435,
   -1,   -1,   -1,   -1,  440,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  452,  340,  454,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,  359,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,  359,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  413,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,  318,  435,   -1,  321,   -1,   -1,  440,   -1,   -1,
   -1,  328,  413,   -1,  331,   -1,   -1,   -1,  335,  452,
   -1,  454,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
  318,   -1,   -1,  321,  435,   -1,   -1,   -1,   -1,  440,
  328,   -1,  359,  331,  271,   -1,   -1,  335,   -1,   -1,
   -1,  452,  340,  454,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,  359,   -1,   -1,  301,   -1,   -1,   -1,   -1,  396,
   -1,  308,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,  318,   -1,   -1,   -1,  383,  413,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,  332,   -1,   -1,  396,   -1,
   -1,  338,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
  273,   -1,   -1,  440,  277,  413,   -1,   -1,   -1,   -1,
   -1,  284,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,  295,   -1,   -1,  298,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,
  417,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
  373,  273,  375,   -1,  451,  277,  453,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,
   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
  423,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  339,   -1,  516,
  517,   -1,  445,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  406,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,  460,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
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
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE varstate value INTO variable_list DO proc_block",
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
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 319:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
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
{ yyval = make_node (nod_while, e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 337:
{ yyval = make_node (nod_label, 1, yyvsp[-1]); }
break;
case 338:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
break;
case 341:
{ yyval = make_node (nod_breakleave, e_breakleave_count,
				make_node (nod_label, 1, yyvsp[-1])); }
break;
case 342:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_on_error, e_err_count,
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
case 353:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 354:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 357:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 361:
{ yyval = make_list (yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 368:
{ yyval = yyvsp[-1]; }
break;
case 369:
{ lex.beginning = lex_position(); }
break;
case 370:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 371:
{ lex.beginning = lex.last_token; }
break;
case 372:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
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
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = NULL; }
break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 381:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 399:
{ yyval = NULL; }
break;
case 400:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
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
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
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
case 453:
{ yyval = NULL; }
break;
case 454:
{ yyval = NULL; }
break;
case 455:
{ yyval = NULL; }
break;
case 456:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 457:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 458:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 459:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 460:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 461:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 462:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 463:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 465:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 466:
{ yyval = yyvsp[0]; }
break;
case 467:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 468:
{ yyval = yyvsp[0]; }
break;
case 469:
{ yyval = yyvsp[0]; }
break;
case 470:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 471:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 472:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 473:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 478:
{ yyval = NULL; }
break;
case 480:
{ yyval = NULL; }
break;
case 481:
{ yyval = yyvsp[0]; }
break;
case 482:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 498:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 499:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 502:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 503:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 509:
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
case 510:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 512:
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
case 513:
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
case 514:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 521:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 522:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 523:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 524:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 525:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 526:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 542:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 543:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 544:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 545:
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
case 546:
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
case 549:
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
case 550:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 553:
{ yyval = yyvsp[-1]; }
break;
case 554:
{ yyval = 0; }
break;
case 558:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 559:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 560:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 564:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 565:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 566:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 567:
{ yyval = 0; }
break;
case 568:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 571:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 572:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 575:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 576:
{ yyval = NULL; }
break;
case 578:
{ yyval = NULL; }
break;
case 579:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 581:
{ yyval = NULL; }
break;
case 583:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 588:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 592:
{ yyval = yyvsp[0];}
break;
case 594:
{ yyval = yyvsp[0];}
break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 602:
{ yyval = 0; }
break;
case 603:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 604:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 605:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 606:
{ yyval = (DSQL_NOD) 0; }
break;
case 607:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 608:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 610:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 612:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 613:
{ yyval = 0; }
break;
case 615:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 616:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 620:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 621:
{ yyval = make_list (yyvsp[0]); }
break;
case 622:
{ yyval = 0; }
break;
case 624:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 625:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 626:
{ yyval = 0; }
break;
case 627:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 628:
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 630:
{ yyval = 0; }
break;
case 631:
{ yyval = yyvsp[-1]; }
break;
case 632:
{ yyval = 0; }
break;
case 633:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 634:
{ yyval = 0; }
break;
case 635:
{ yyval = yyvsp[0]; }
break;
case 636:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 637:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 638:
{ yyval = 0; }
break;
case 639:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 640:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 641:
{ lex.limit_clause = true; }
break;
case 642:
{ lex.limit_clause = false; }
break;
case 643:
{ lex.first_detection = true; }
break;
case 644:
{ lex.first_detection = false; }
break;
case 645:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 646:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 647:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 648:
{ yyval = 0; }
break;
case 649:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 650:
{ yyval = yyvsp[-2]; }
break;
case 651:
{ yyval = yyvsp[-1]; }
break;
case 652:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 653:
{ yyval = yyvsp[-1]; }
break;
case 654:
{ yyval = yyvsp[0]; }
break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 656:
{ yyval = 0; }
break;
case 657:
{ yyval = make_list (yyvsp[0]); }
break;
case 658:
{ yyval = 0; }
break;
case 660:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 662:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 665:
{ yyval = make_list (yyvsp[0]); }
break;
case 667:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
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
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 679:
{ yyval = yyvsp[-1]; }
break;
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 681:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 682:
{ yyval = make_list (yyvsp[-1]); }
break;
case 683:
{ yyval = NULL; }
break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 689:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 690:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 691:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 698:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 699:
{ yyval = make_list (yyvsp[0]); }
break;
case 700:
{ yyval = 0; }
break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 704:
{ yyval = yyvsp[0]; }
break;
case 705:
{ yyval = 0; }
break;
case 706:
{ yyval = yyvsp[0]; }
break;
case 707:
{ yyval = 0; }
break;
case 708:
{ yyval = yyvsp[0]; }
break;
case 709:
{ yyval = 0; }
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
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 728:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 733:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 734:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 738:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 739:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 741:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 745:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 747:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 752:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 753:
{ yyval = yyvsp[0]; }
break;
case 755:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 758:
{ yyval = NULL; }
break;
case 759:
{ yyval = make_list (yyvsp[-1]); }
break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = NULL; }
break;
case 764:
{ yyval = make_list (yyvsp[-1]); }
break;
case 766:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = yyvsp[-1]; }
break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 827:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 829:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 831:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 832:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 833:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 835:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 838:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
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
case 844:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 848:
{ yyval = make_list (yyvsp[-1]); }
break;
case 849:
{ yyval = yyvsp[-1]; }
break;
case 850:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 851:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 861:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 862:
{ yyval = yyvsp[0]; }
break;
case 863:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 864:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 865:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 866:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 867:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 868:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 869:
{ yyval = yyvsp[-1]; }
break;
case 870:
{ yyval = yyvsp[-1]; }
break;
case 874:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 875:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 876:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 877:
{ yyval = yyvsp[0]; }
break;
case 878:
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
case 879:
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
case 880:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 881:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 883:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 885:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 886:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 887:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 888:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 889:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 890:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 892:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 893:
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
case 894:
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
case 895:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 896:
{ yyval = make_parameter (); }
break;
case 897:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 898:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 899:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 900:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 901:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 902:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 903:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 904:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 905:
{ yyval = yyvsp[0]; }
break;
case 906:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 908:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 909:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 910:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 911:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 912:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 914:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 915:
{ yyval = yyvsp[0];}
break;
case 920:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 921:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 922:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 923:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 924:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 925:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 926:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 927:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 928:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 929:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 930:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 931:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 933:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 937:
{ yyval = yyvsp[0]; }
break;
case 938:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 939:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 940:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 941:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 944:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 945:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 949:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 950:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 951:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 952:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 953:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 954:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 955:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 959:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 960:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 961:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 962:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 964:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 965:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 966:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 969:
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
