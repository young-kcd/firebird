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
  115,  118,  118,  119,  119,  119,  119,  121,  121,  121,
  121,  120,  120,  125,  125,  124,   98,  126,  126,  127,
  130,  131,  131,  132,  128,  129,  129,  133,  133,  134,
  134,  135,  135,  135,   96,   96,   96,   69,   79,  136,
  136,  136,  137,  137,  138,  138,  139,  139,  139,  145,
   86,   86,  141,  141,  104,  104,   99,  149,  150,   58,
  142,  142,  105,  105,  105,  105,  105,  105,  143,  143,
  157,  157,  158,  160,  160,  160,  160,  160,  140,  159,
  159,  164,  164,  164,  164,  166,  167,  168,  162,  162,
  163,  161,  161,  161,  161,  161,  169,  170,  171,  171,
  171,  171,   68,   78,   82,  176,  172,  172,  173,  173,
  177,  177,  178,  174,  174,  179,  179,  180,  181,  181,
  182,  182,  183,  183,  175,  185,  185,  185,  186,  186,
  184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
  184,  184,  184,  184,  184,  184,  184,  184,  189,  189,
  190,  191,  192,  203,  203,  193,  198,  199,  194,  194,
  195,  206,  196,  207,  201,  201,  201,  202,  202,  202,
  204,  204,  204,  204,  197,  200,  200,  205,  205,  187,
  187,  212,  213,  213,  214,  214,  214,  214,   11,   71,
   80,  216,  219,  219,  219,  220,  223,  227,  227,  228,
  228,  229,  229,  147,  218,   87,   89,  217,  217,   70,
   83,  232,  232,  232,  233,  236,  236,  237,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  234,  234,  235,    2,  239,  239,  239,  239,
  239,  239,  239,  247,  248,  245,  245,  249,  249,  249,
  249,  249,  249,  249,  240,  240,  250,  250,  250,  250,
  250,  250,  250,  244,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  252,  252,  254,
  254,  253,  251,  251,  251,  246,  246,  242,  243,  243,
  257,  257,  257,  257,  258,  258,  241,  259,  259,  260,
  260,    8,  261,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  101,  101,  144,  144,  264,  264,
  267,  267,  269,  269,   53,   53,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  274,  274,  265,  265,  265,
  265,  275,  275,   61,   61,   61,   55,   55,  271,  271,
  271,  268,  268,  268,  279,  279,  279,  278,  278,  277,
  277,  277,  272,  272,  282,  280,  280,  280,  281,  281,
  273,  273,  273,  273,  283,  283,   18,   18,   18,  285,
  285,  285,   16,   16,   16,  287,  288,  291,  291,  289,
  293,  293,    4,   15,  292,  292,  294,  294,  295,  295,
  284,  296,  296,  297,  297,  298,  298,  298,  298,  299,
  299,  300,  300,  301,  301,  303,  303,  303,  304,  304,
  304,  305,  305,  305,  302,  307,  307,  307,  308,  308,
  306,  306,  309,  311,  311,  310,  310,  286,   17,  312,
  312,  312,  313,  313,  316,  316,  317,   65,   65,   65,
  319,  319,  318,  318,  314,  314,  322,  322,  323,  323,
  315,  327,  328,  329,  320,  321,  324,  324,  324,  324,
  330,  330,  330,  331,  331,  331,  221,  221,  222,  222,
  334,  334,  335,  335,  335,  325,  338,  338,  339,  339,
  340,  340,  341,  341,  342,  342,  209,  209,  344,  344,
  230,  230,   31,  231,  231,  231,  231,  231,  231,  231,
  231,  225,  225,  345,  345,  346,  346,  346,  346,  346,
  348,  348,  348,  226,  226,  224,  224,  326,  326,  352,
  353,  353,  353,  353,  353,  354,  354,  355,  355,  356,
  356,  357,  357,  357,  358,  358,   10,   10,  360,  360,
    7,    7,  361,  362,  363,   19,   19,  364,  365,  366,
  366,  188,  336,  336,    3,    3,  368,  368,  368,  370,
  370,   61,  369,  369,  371,  371,   34,   34,   85,   84,
   84,  359,  359,  372,  373,  373,  210,  210,  210,  146,
  367,  367,  111,  111,  111,  111,  111,  377,  377,  377,
  377,  377,  377,  376,  376,  376,  379,  379,  379,  379,
  379,  379,  379,  379,  379,  379,  380,  380,  380,  380,
  380,  380,  380,  380,  385,  385,  385,  385,  385,  385,
  385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
  391,  391,  381,  381,  382,  382,  382,  382,  383,  383,
  387,  387,  388,  388,  388,  388,  386,  389,  384,  384,
  375,  375,  375,  378,  378,  378,  392,  392,  393,  390,
  395,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,  156,  156,  156,
  396,  394,  394,  151,  151,  400,  400,  400,  400,  400,
  398,  398,  398,  398,  398,  332,  152,  152,  153,  154,
  154,  154,  154,  154,   51,   51,   20,   20,  238,  401,
   54,  123,  270,  270,   95,  397,  397,  397,  397,  402,
  402,  402,  402,  402,  402,  402,  402,  402,  402,  402,
  403,  349,  350,  350,  350,  347,  347,  399,  351,  351,
  405,  405,  406,  406,  407,  407,  410,  410,  408,  408,
  413,  413,  412,  409,  411,  404,  404,  404,  404,  404,
  404,  404,  404,  333,  333,  155,   48,  276,  122,  148,
  374,  165,  211,  263,   63,   60,  215,  112,   66,  337,
   35,   41,  343,  262,   39,   42,  208,   40,  290,  256,
  256,  414,  414,  414,  414,  414,  414,  414,  414,  414,
  414,  414,  414,  414,  414,
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
    3,    1,    1,    6,    3,    1,    3,    1,    3,    2,
    1,    0,    2,    3,    1,    0,    1,    1,    2,    3,
    4,    0,    1,    2,    0,    1,    1,    5,    5,    3,
    2,    0,    1,    3,    1,    1,    6,    3,    2,    6,
    2,    1,    2,    2,    2,    0,    1,    1,    2,    0,
    2,    0,    1,    1,    1,    1,    1,    1,    0,    1,
    1,    2,    2,    2,    5,    1,    2,    3,    2,    2,
    0,    1,    1,    1,    1,    3,    4,    8,    4,    0,
    6,    1,    1,    2,    2,    0,    3,    3,    1,    2,
    2,    2,    8,    8,    8,    8,    3,    0,    2,    0,
    1,    3,    2,    1,    0,    1,    2,    6,    1,    0,
    2,    1,    1,    1,    3,    1,    2,    0,    1,    2,
    2,    2,    1,    1,    1,    1,    1,    1,    2,    1,
    1,    2,    1,    1,    1,    2,    2,    1,    3,    4,
    2,    6,    4,    1,    1,    7,    8,    6,    8,    6,
    4,    0,    4,    2,    1,    3,    0,    2,    4,    0,
    1,    1,    3,    3,    6,    2,    2,    3,    0,    1,
    2,    4,    1,    3,    2,    2,    2,    1,    4,    7,
    7,    1,    1,    3,    4,    7,    2,    1,    3,    1,
    1,    6,    3,    0,    0,    0,    0,    3,    0,    9,
    9,    1,    1,    0,    2,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    5,    5,    5,    5,
    5,    5,    2,    0,    4,    2,    3,    3,    2,    2,
    3,    3,    2,    3,    6,    1,    2,    4,    3,    2,
    2,    2,    2,    3,    1,    3,    3,    3,    2,    2,
    4,    4,    5,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    0,    2,    2,    0,    1,    2,
    2,    2,    2,    2,    1,    3,    7,    1,    0,    1,
    0,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    3,    2,    2,    2,    1,    1,    1,    1,    4,    5,
    1,    3,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    4,    4,    6,
    5,    3,    0,    2,    2,    0,    3,    0,    4,    1,
    5,    4,    1,    4,    1,    2,    2,    1,    1,    1,
    2,    2,    2,    2,    1,    0,    3,    5,    1,    1,
    2,    3,    1,    2,    3,    0,    1,    1,    1,    5,
    5,    6,    1,    1,    1,    2,    4,    1,    0,    5,
    1,    0,    3,    2,    1,    0,    2,    0,    1,    0,
    3,    1,    0,    1,    2,    1,    1,    1,    1,    2,
    2,    1,    2,    3,    1,    1,    3,    3,    1,    2,
    3,    1,    2,    0,    2,    1,    1,    0,    1,    1,
    1,    3,    2,    3,    0,    1,    3,    4,    3,    1,
    3,    4,    3,    0,    1,    3,    3,    1,    1,    0,
    1,    1,    4,    0,    4,    0,    2,    0,    2,    0,
    9,   10,    0,    0,    0,    0,    3,    2,    1,    0,
    3,    5,    3,    2,    5,    2,    1,    1,    1,    1,
    1,    3,    1,    2,    3,    2,    1,    3,    1,    1,
    6,    3,    3,    2,    3,    0,    1,    3,    1,    1,
    1,    2,    1,    1,    1,    2,    1,    2,    1,    2,
    0,    3,    0,    1,    3,    1,    1,    1,    1,    3,
    1,    1,    1,    2,    0,    2,    0,    2,    0,    4,
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
    6,    6,    6,    8,    4,    4,    3,    6,    1,    1,
    6,    6,    1,    1,    4,    6,    4,    5,    3,    5,
    4,    5,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    0,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  547,
  548,  549,  553,  554,  555,    0,  610,  721,  722,  726,
  727,  458,    0,    0,    0,    0,    0,    0,  396,  565,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  899,    0,   21,  897,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  472,    0,    0,   37,   34,
   36,    0,   35,    0,  980,  982,  984,  983,  989,  985,
  992,  990,  986,  988,  991,  987,  993,  994,  995,    0,
    0,    0,    0,   32,   68,   66,  972,  981,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  639,    0,    0,    0,  671,    0,  974,    0,    0,
    0,    0,  107,  979,  556,    0,    2,    0,    0,    0,
    0,  426,  427,  428,  429,  430,  431,  432,  433,  434,
  435,  436,  437,  438,  439,  440,  441,  442,  443,  444,
  445,  446,  447,    0,  424,  425,    0,  965,    0,  403,
  969,    0,  400,  971,    0,  673,    0,  399,  975,    0,
  563,  895,    0,  150,  103,    0,  104,    0,  217,  961,
  760,    0,  102,  145,  968,    0,    0,   98,    0,  105,
  901,    0,   99,    0,  100,    0,  101,  978,  146,  106,
  618,  619,    0,  900,  898,    0,   74,    0,  966,    0,
  964,  480,  473,    0,  479,  484,  474,  475,  482,  476,
  477,  478,  483,    0,   30,    0,   39,  747,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  905,    0,  886,  633,  633,  634,
  644,  646,  954,  647,    0,  648,  638,  634,    0,    0,
    0,    0,    0,    0,    0,  582,  571,    0,  574,  576,
  577,  578,  579,  585,  586,    0,  973,  672,    0,  108,
    0,  109,    0,  110,    0,    0,    0,  611,    0,  609,
    0,    0,    0,    0,  459,    0,    0,  366,    0,  220,
    0,  406,  397,  456,  457,    0,    0,    0,    0,    0,
    0,  415,    0,  372,  373,    0,  569,  567,  896,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  957,   75,    0,    0,    0,  723,  724,  481,
    0,    0,    0,    0,  864,  892,    0,  956,    0,  866,
    0,    0,  893,    0,  887,  878,  877,  876,    0,    0,
    0,    0,  868,  869,  870,  879,  880,  888,  889,    0,
  890,  891,    0,    0,    0,  894,    0,    0,    0,    0,
    0,  882,    0,  757,  861,  862,  863,  669,  867,  349,
  847,    0,    0,    0,  846,    0,  667,  850,  908,  909,
  849,  843,  844,  845,  848,  881,  906,  907,  929,  930,
  933,  934,    0,  750,    0,    0,   27,    0,    0,    0,
   71,   69,  976,   67,   33,    0,    0,    0,  752,    0,
   52,    0,    0,    0,    0,    0,  561,    0,    0,    0,
  641,  643,    0,  650,    0,  734,    0,    0,  651,    0,
  637,    0,  608,    0,  583,    0,  580,    0,  581,  606,
    0,  601,    0,    0,  575,  761,  730,    0,    0,    0,
    0,    0,    0,  558,  557,    0,    0,  615,  612,    0,
  185,    0,    0,  129,  464,  172,  173,    0,  462,    0,
    0,    0,    0,  465,    0,  460,    0,    0,  410,  412,
  411,    0,  413,    0,  407,  218,    0,    0,  271,    0,
    0,    0,    0,  419,  420,    0,  449,    0,    0,    0,
    0,    0,    0,    0,  377,  376,  468,    0,    0,  149,
    0,    0,  158,    0,  157,    0,  164,  147,    0,  162,
  165,  166,  154,    0,    0,  503,  539,  506,    0,  540,
    0,    0,  507,    0,    0,    0,  530,  543,  502,  525,
  504,  505,  500,  487,  366,  485,  486,  488,    0,    0,
  497,  498,  499,  501,    0,    0,    0,    0,    0,  112,
    0,  113,    0,  124,    0,    0,  201,    0,    0,  364,
    0,  220,    0,    0,   83,    0,    0,  742,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  763,
  765,  774,  777,  778,  779,  780,  781,  782,  783,  784,
  785,  786,    0,    0,    0,  883,    0,    0,    0,    0,
    0,    0,  884,  885,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  324,  977,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  749,   28,    0,    0,   44,   72,    0,    0,   26,    0,
  755,    0,    0,    0,  718,    0,    0,    0,    0,    0,
  560,    0,  633,    0,    0,    0,    0,    0,  970,  654,
  551,    0,  903,  550,    0,  584,    0,  592,  588,  587,
    0,    0,    0,  603,  591,    0,    0,  728,  729,    0,
    0,    0,  364,    0,    0,    0,    0,    0,    0,  130,
    0,    0,    0,    0,    0,    0,  366,  409,    0,  366,
  367,  414,  495,    0,  273,    0,  267,  269,  364,  240,
  962,    0,    0,  366,    0,    0,  209,  364,    0,    0,
    0,    0,  245,  239,  242,  243,  244,  418,  454,  453,
  417,  416,    0,    0,    0,    0,    0,    0,  375,    0,
  366,    0,  155,  159,    0,  163,    0,    0,  526,  527,
  544,    0,  541,    0,    0,  533,  531,  532,    0,    0,
    0,    0,  496,    0,    0,    0,    0,  534,    0,    0,
  364,    0,  200,    0,  203,  205,  206,    0,    0,    0,
    0,    0,  220,   78,    0,    0,   77,  514,  958,  515,
    0,    0,    0,    0,  764,  776,    0,    0,  834,  835,
  836,    0,  768,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  946,  947,  948,  949,
  950,  951,  952,  953,    0,    0,    0,    0,    0,    0,
  939,    0,    0,    0,    0,  859,  326,  860,  927,    0,
    0,  855,  960,    0,    0,    0,    0,    0,  668,    0,
  865,  759,  758,  751,    0,    0,    0,    0,    0,    0,
    0,    0,   53,   61,   63,    0,    0,   70,    0,    0,
    0,  754,    0,    0,    0,    0,    0,    0,    0,    0,
  642,  645,    0,    0,    0,    0,  659,  660,    0,    0,
  652,  655,  552,  904,  593,  602,  597,  596,    0,  607,
  762,  731,  732,  364,    0,    0,  635,  617,  616,    0,
    0,  625,  181,  175,  182,    0,  178,    0,    0,  177,
    0,  188,  169,  168,  902,  171,  170,  466,    0,    0,
    0,  408,  272,    0,  211,  366,  213,    0,  367,  208,
  214,    0,    0,    0,    0,  421,  422,  366,  364,  367,
    0,    0,    0,  393,    0,  196,  197,  156,    0,    0,
    0,    0,    0,    0,  542,    0,    0,  366,  367,    0,
  491,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  198,    0,    0,    0,    0,  353,
    0,   82,    0,    0,    0,    0,   87,   84,    0,    0,
    0,  725,  963,    0,    0,  769,    0,    0,  775,    0,
    0,    0,    0,    0,  812,  811,    0,    0,    0,  819,
  837,  829,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  767,    0,    0,    0,  220,    0,  910,    0,
    0,  925,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  935,    0,  945,    0,    0,    0,
    0,    0,    0,  926,  871,   65,   57,   58,   64,   59,
   60,    0,    0,   22,    0,    0,    0,   42,    0,    0,
    0,  736,  756,    0,  719,    0,  735,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  674,    0,    0,
    0,    0,    0,  599,  600,  604,    0,  199,    0,    0,
  629,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,  404,  223,  224,  225,  226,  227,  228,  874,    0,
    0,    0,  276,    0,  221,    0,    0,    0,    0,  246,
    0,  450,  451,  423,    0,    0,    0,    0,    0,    0,
  366,  470,  367,  167,  959,    0,    0,  509,    0,  508,
  545,    0,  537,    0,    0,    0,  489,    0,  517,    0,
    0,  519,  522,  524,  364,    0,    0,    0,  119,    0,
  204,  366,    0,    0,  365,    0,   97,    0,  117,    0,
   93,    0,    0,    0,    0,   86,    0,  827,  828,  771,
    0,  773,    0,    0,    0,    0,    0,    0,    0,  830,
    0,    0,    0,    0,    0,    0,    0,  820,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  916,  915,    0,    0,  912,  911,    0,  920,
  919,  918,  917,  914,  913,    0,    0,  941,    0,    0,
    0,  940,    0,    0,    0,    0,    0,    0,   56,   62,
   54,   55,   23,  740,  741,    0,  738,  745,  746,  743,
    0,    0,  717,    0,    0,  662,    0,  663,    0,  680,
  676,  678,    0,    0,    0,    0,    0,  365,  621,  622,
  636,    0,  183,  179,    0,    0,  194,  190,    0,  367,
  875,  279,    0,    0,  367,  277,    0,    0,    0,  231,
    0,    0,  247,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  467,  511,    0,  512,    0,  134,    0,    0,
    0,  138,  140,  141,  142,  492,  494,  490,  521,    0,
    0,  367,    0,    0,    0,    0,  350,    0,  354,  366,
   88,    0,    0,   90,    0,    0,    0,    0,    0,    0,
  839,  838,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  219,  928,  921,  922,    0,  923,  937,  936,    0,  942,
  931,  932,    0,   40,    0,    0,  720,  665,    0,  535,
    0,  687,    0,    0,  684,  688,  689,  691,  692,  693,
    0,    0,  631,  367,  351,  623,    0,  174,  191,  367,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  284,  289,  283,
    0,    0,    0,  293,  294,  295,  296,  297,  298,  300,
  301,  303,  304,  305,  308,  266,    0,    0,  207,  232,
    0,    0,    0,    0,  233,  236,    0,    0,  367,  391,
  392,  388,  387,  390,  389,    0,  510,  538,  143,  131,
  139,    0,  366,  263,  126,  367,    0,    0,  368,  355,
    0,    0,   91,   92,   79,    0,  798,  806,    0,  799,
  807,  802,  810,    0,    0,  800,  808,  801,  809,  795,
  803,  796,  804,  797,  805,    0,  938,    0,  737,    0,
    0,    0,    0,    0,  701,  703,    0,  698,    0,  264,
  184,  405,    0,  311,    0,    0,  314,  315,    0,  307,
    0,    0,    0,    0,  306,    0,  336,  337,  292,  299,
    0,  302,  285,    0,  290,    0,  340,  291,  210,  215,
  234,    0,    0,  237,    0,  249,  251,  395,  367,    0,
  370,    0,    0,    0,  360,  361,    0,    0,   76,   94,
    0,  924,    0,    0,    0,  690,  685,  702,    0,    0,
  282,    0,  309,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  331,  332,  348,    0,    0,    0,    0,  343,
  341,  238,    0,    0,    0,    0,    0,  265,  367,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  709,
    0,    0,    0,  281,  278,  310,    0,    0,  313,    0,
    0,    0,  321,    0,  323,    0,  347,  346,  967,  345,
    0,    0,    0,    0,    0,  248,    0,  255,    0,  254,
  371,  363,    0,    0,  356,  118,    0,    0,    0,  711,
  700,    0,    0,  712,    0,  708,    0,    0,    0,    0,
    0,    0,    0,    0,  334,  333,  342,  344,  235,    0,
    0,  259,  258,  257,    0,    0,  841,  632,  707,    0,
  714,    0,    0,  312,  318,    0,    0,    0,    0,  335,
  262,  260,  261,    0,    0,    0,    0,    0,    0,  338,
  316,    0,    0,    0,    0,  713,  329,  317,  319,  840,
  716,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1544,   28,   29, 1545,
   31,   32,   33,   34,   35,   36, 1546,   38, 1547,  638,
  120,  456,  951, 1184,  121,  704,  952,  122,  460,  709,
  146,  123,  124,  257,  974,  136,  953,  954,  197,  226,
  125,  955,  126,  462,   82,  237,  374,  421,  633, 1085,
  422,  856,  604, 1510,  833,  634,  635,  636, 1087,  238,
 1374,   74,  187,   75,  233, 1826, 1297,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  620,  622,  453,  258,  783,  538,  648, 1022,  367,
  625, 1073, 1289,  571,  733, 1048,  523,  524,  553,  359,
  605, 1059, 1440, 1569, 1232, 1441, 1442, 1443, 1444, 1445,
  649,  214,  206,  355,  578,  356,  357,  579,  580,  581,
  582, 1264, 1016,  526,  527, 1006, 1004,  528, 1010, 1005,
 1222, 1403, 1011, 1012, 1227,  369,  844,  845,  846,  847,
  785, 1029, 1418,  606,  787,  424,  849,  932,  547, 1346,
 1233,  425,  426,  427,  428,  429, 1419, 1420,  556, 1575,
 1725, 1250,  793,  794,  780,  795,  796,  797, 1726, 1727,
 1803,  337,  551, 1241, 1548,  193,  548,  549, 1242, 1243,
 1413, 1702, 1549, 1550, 1551, 1552, 1666, 1553, 1554, 1555,
 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565,
  430, 1788, 1649, 1712, 1792, 1710,  431,  688,  432,  433,
 1092, 1667, 1719, 1720, 1758, 1078, 1295, 1457, 1079, 1080,
  285,  487, 1598,  378, 1213, 1396, 1683, 1684, 1685, 1686,
 1211,  346,  567,  811, 1262,  568,  809,  221,   59,  341,
  198,  161,  324,  184,  331,  190,  771,  768,  332,  342,
  801,  343,  563, 1040,  185,  434,  325,  535,  569, 1263,
   96,  196,  242,  607,  608,  773, 1060,  774, 1061, 1062,
  611,  612,  613,  614, 1053,  860,  615,  616,  617,  826,
  618, 1512,  823,   40,   41,   42,   43,   44,   45,  155,
  515,   61,  478,  201,  348,  297,  298,  299,  300,  301,
  302,  303,  304,  305,  739,  501,  989, 1216,  502,  503,
  744,   46,  160,  320,   47,  517,  518,  998, 1401, 1220,
 1526,  757, 1002,  140,  726, 1523,  715,  481,  287,  141,
  142,  435,  286,  488,  489,  490,  730,  975,  976,  977,
  978, 1205,  436, 1167, 1514, 1515,  438, 1517,  439,  440,
  441, 1740, 1639, 1741, 1742, 1743, 1786, 1827,  468, 1196,
   48,   49,  379,   50,   51,  509,  510,  960, 1192, 1376,
 1380,  469,  712,  211,  650,  651,  874,  875,  652,  653,
  654,  655,  656,  657,  658,  659,  660,  661,  662, 1318,
 1108, 1110, 1111,  931,  687,  442,  443,  444,  445,  446,
  235,  447,  448,  915,  449,  450,  451,  452,  678,  919,
 1168, 1163,  679,  128,
};
static short yysindex[] = {                                   5885,
 1241, -153, 3097,  136, 1450,   -6, 3282,  -62, 4269,  428,
  278, 3784, -153, 1404,  998, 5276, 1394, 5276,  232,    0,
  816,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -80,    0,    0,    0,    0,
    0,    0, 9565, 5276, 5276, 5276, 5276, 5276,    0,    0,
  489, 1424, 5276, 5276, 5276,  660, 5276,  561, 5276, 5276,
    0, 5276, 5276,    0,  562,    0,  578,    0,    0,  610,
 5276,    0, 5276, 5276, 5276,  726, 5276, 5276, 5276, 5276,
  561, 5276, 5276, 5276, 5276,    0, 5276,  659,    0,    0,
    0, 1069,    0, 1069,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  738,
  778,  142, 1141,    0,    0,    0,    0,    0, 5276, 5276,
 5276,  860,  907,  924,  183, 1126,  900,  124,  189, 1029,
  918,    0, 5276, 1089, 1181,    0, 1049,    0, 5276, 5276,
 5276, 5276,    0,    0,    0, 5276,    0, 1223,  -30, 1191,
  732,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  643,    0,    0, 1424,    0,   36,    0,
    0, 1564,    0,    0, 1621,    0,  522,    0,    0, 1169,
    0,    0, 1178,    0,    0,  -52,    0, 1452,    0,    0,
    0, 1424,    0,    0,    0, 1014, 1564,    0,   21,    0,
    0, 1423,    0, 1444,    0, 1069,    0,    0,    0,    0,
    0,    0, 1412,    0,    0, 1332,    0, 1474,    0, 1362,
    0,    0,    0, 1332,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8757,    0, 5276,    0,    0,    0, 4963,
 1421, 5044, 5276, 1402, 1476, 1787, 1509, 1513, 4963, 1438,
 5044, 1467, 1482, 1371,    0,10448,    0,    0,    0,    0,
    0,    0,    0,    0, 8029,    0,    0,    0, 1445, 5276,
 1546, 1449, 1066, 5276, 1469,    0,    0, 1181,    0,    0,
    0,    0,    0,    0,    0, 5276,    0,    0, 1564,    0,
 1423,    0, 1069,    0, 1529,10448, 1498,    0, 1490,    0,
 1335, 1567, 1033,  732,    0, 1659,   48,    0, 5276,    0,
  643,    0,    0,    0,    0, 5276, 1539, 4057, 1496, 4478,
 1930,    0, 9565,    0,    0,   86,    0,    0,    0, 1924,
 1924, 1424, 1615, 1424, 1227,  -52,    0,    0, 7765,    0,
 5276, 5276,    0, 1539,    0,    0, 1703,  -72, 1994, 5276,
 1730, 5276,    0,    0, 2004,  130, 5340,    0,    0,    0,
 2013, 2014, 2016, 1555,    0,    0, 2020,    0, 2021,    0,
 2024, 2026,    0, 2027,    0,    0,    0,    0,    0, 2028,
 1608, 1609,    0,    0,    0,    0,    0,    0,    0, 2046,
    0,    0, 8854, 2047, 2048,    0,10448,10448, 8127, 5276,
 2049,    0, 1954,    0,    0,    0,    0,    0,    0,    0,
    0, 2052, 1999,    0,    0, 2054,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1516,    0, 5276, 1657,    0, 5276, 5276,  164,
    0,    0,    0,    0,    0, 5276, 5276,  989,    0, 5276,
    0, 1778, 5276, 2064, 4963, 1710,    0, 5276, 9093, 1350,
    0,    0,10448,    0, 1954,    0, 1796, 2070,    0, 3993,
    0,   92,    0,  838,    0,  -67,    0,  -67,    0,    0,
 2073,    0,  174, 1690,    0,    0,    0, 2074,  155, 2058,
 1539, 2082, 1861,    0,    0, 1867, 2079,    0,    0, 1745,
    0,  272, 1780,    0,    0,    0,    0, 1424,    0, 1924,
 1924, 1924, 1924,    0, 2086,    0, 1855, 1860,    0,    0,
    0, 1837,    0, 7765,    0,    0, 7765, 1781,    0, 1564,
 1886, 5276,12258,    0,    0,  809,    0, 5276, 1116, 1621,
 1749, 1715, 1684,    0,    0,    0,    0,  -32, 1758,    0,
 1698,  561,    0, 1424,    0, 1881,    0,    0, 1227,    0,
    0,    0,    0,  153, 1714,    0,    0,    0, 1718,    0,
 1763, 2128,    0, 1828, 2133,   69,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2100,  634,
    0,    0,    0,    0,  113, 2152, 2155, 2133, 1564,    0,
 1878,    0, 1934,    0, 1424, 1424,    0, 4057,  522,    0,
 1817,    0, 1790, 2160,    0, 9825, 1459,    0, 1819, 1838,
 2170, 5941, 2181,    0,    0,    0, 5243, 4291,  471,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1104, 9332,  175,    0, 5276,10448, 1322, 1325,
 1414, 2389,    0,    0,10448, 6171, 1954, 1776,  674,10448,
 9332, 1088, 1088, 1404, 1569, 1794, 2188,    0,    0, 9412,
 5276,10448,10448,10448,10448,10448, 9332,10448,  976, 5276,
    0,    0, 5534, 1797,    0,    0, 1768, 5044,    0, 1923,
    0, 1864, 1404, 2203,    0, 1923, 5534, 1925, 1928, 5276,
    0, 1569,    0, 2068,  819, 1800, 9332, 5276,    0,    0,
    0, 1091,    0,    0,  -19,    0, 1805,    0,    0,    0,
 5276, -103, 5276,    0,    0, 5276, 5276,    0,    0, 9332,
 1995, 4057,    0, 1756,10448, 5276, 1806, 1424, 1424,    0,
  410, 1698, 1698, 1803,  561, 1033,    0,    0, 2222,    0,
    0,    0,    0, 1989,    0, 5276,    0,    0,    0,    0,
    0, 1990, 2227,    0, 1969, 1992,    0,    0, 1926, 1933,
 1069, 2001,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  561, 5276,12432, 1894, 1899, 1900,    0,  561,
    0, 1553,    0,    0, 1869,    0,  128, 1879,    0,    0,
    0,  561,    0, 2128,  137,    0,    0,    0, 1997,  137,
 1883,  137,    0, 2256,  561,  561,  561,    0, 1539, 5276,
    0, 1943,    0, 1882,    0,    0,    0,   86, 1885, 5276,
 1884,  131,    0,    0, 2263, 2040,    0,    0,    0,    0,
  130, 5276, 1901, 6418,    0,    0, 1901, 5941,    0,    0,
    0, 3806,    0,  166,  671,10448,10448, 6515, 2286,  530,
 6769,10448, 6921, 1764, 7030, 7284, 9651, 7436, 7545, 7799,
 6171, 6171,10448,10448, 2056,10448, 2287,10448, 2288, 2143,
10448,10448,10448,10448,10448,10448,    0,    0,    0,    0,
    0,    0,    0,    0, 2017, 1123,   -4,10448,  748, 9332,
    0, 6171, 1413, 2290, 1029,    0,    0,    0,    0, 1954,
 1889,    0,    0, 2061, 1088, 1088, 1228, 1228,    0,  754,
    0,    0,    0,    0, 5276, 5276, 5276, 5276, 5276, 5276,
  167, 2292,    0,    0,    0, 5534, 1958,    0,  -38, 1983,
 5276,    0, 1029, 9332, 1983, 2312, 2292, 5534, 4542, 2037,
    0,    0,  819, 2318, 2315,  775,    0,    0, 6171, 2039,
    0,    0,    0,    0,    0,    0,    0,    0, -106,    0,
    0,    0,    0,    0, 1932, 1885,    0,    0,    0, 2324,
 1865,    0,    0,    0,    0, 1976,    0, 1924, 2107,    0,
  410,    0,    0,    0,    0,    0,    0,    0, 2334, 6171,
 1993,    0,    0, 2076,    0,    0,    0, 1993,    0,    0,
    0, 1069, 1069, 1875, 2337,    0,    0,    0,    0,    0,
  362,  451, 1442,    0, 2118,    0,    0,    0, 5276,  136,
 1977, 1961, 1989, 2345,    0, 1698, 1982,    0,    0, 1289,
    0, 2330, 5276, 1316,  561, 2348, 2349, 2350, 2129,  522,
 2076, 1924, 1780, 4057,    0, 1758, 1029, 1938, 1959,    0,
  718,    0,  561, 2007, 2099, 9825,    0,    0,  561, 1919,
 2105,    0,    0, 2376, 2377,    0, 6171, 6171,    0, 6171,
 6171, 1937, 1954, 2379,    0,    0, 1954, 2381, 9890,    0,
    0,    0, 2072, 2386, 1954, 2387, 1739, 2390, 1954, 2391,
10448,10448, 2286,10448, 9970, 2393, 1954, 2397, 2400, 1954,
 2401,10448, 1954, 2413, 1954, 2416, 2417, 1954, 2418, 2419,
 1954, 2420,    0, 2167, 2270, 2280,    0, 2451,    0, 2793,
10448,    0, 2802, 2975, 3016, 3057, 3066, 3112,10448,  561,
 9332, 1954, 2030, 9332,    0,10448,    0, 2134,  538,10448,
 9332,10448,10448,    0,    0,    0,    0,    0,    0,    0,
    0, 2140, 5534,    0, 5534,  167, 2292,    0,  105,  105,
  194,    0,    0, 8029,    0, 1986,    0, 2312, 2292, 2426,
 4542,  775, 2421, 9332, 5276,  819, 2083,    0, 2084, 2094,
 2141, 2209, 2168,    0,    0,    0, 2076,    0, 1938,  688,
    0, 2081, 1424, 2096, 1698, 2110, 1698,    0, 6171,  442,
 1435,    0,    0,    0,    0,    0,    0,    0,    0, 2059,
 2235, 2076,    0,10448,    0, 2220, 2103, 1875,  562,    0,
 6171,    0,    0,    0, 2126, 2130, 2132, 2136, 2142, 2144,
    0,    0,    0,    0,    0, 2474,  136,    0, 1803,    0,
    0,  137,    0, 1993, 2148,  137,    0,  137,    0, 1989,
 2478,    0,    0,    0,    0,   86, 2235, 1698,    0, 1780,
    0,    0, 8029, 2242,    0,   12,    0, 2324,    0, 2481,
    0, 2490, 1424,  208, 2495,    0, 1424,    0,    0,    0,
 2167,    0, 2167,10448, 2120, 2120, 1404, 2497, 1987,    0,
 2120, 2120,10448, 2120, 2120, 2297, 1954,    0, 1859,10448,
 1954, 2120, 2120, 2120, 2120, 1954, 2120, 2120, 2120, 2120,
 2120, 2120,    0,    0, 7765, 2499,    0,    0, 3252,    0,
    0,    0,    0,    0,    0, 3267,  168,    0, 9332, 2233,
 2108,    0, 9332, 3278, 1991, 1214, 1954, 2161,    0,    0,
    0,    0,    0,    0,    0, 2109,    0,    0,    0,    0,
 1796, 9332,    0, 4542, 2426,    0, 1996,    0,  775,    0,
    0,    0,  819, 4326, 6171, 2153, 2235,    0,    0,    0,
    0, 1924,    0,    0, 1424, 1553,    0,    0,  681,    0,
    0,    0, 5276,12168,    0,    0, 1954, 2268, 2220,    0,
 1240, 5276,    0, 2214,  690, 2111, 2216, 2115, 2251, 2223,
 2261, 2076,    0,    0, 2522,    0, 2525,    0, 2217, 2268,
 2148,    0,    0,    0,    0,    0,    0,    0,    0, 2076,
 1758,    0, 1553, 2118, 2250, 2187,    0, 1885,    0,    0,
    0, 2215,  -77,    0, 1989, 2225, 1954, 2533, 2539, 1029,
    0,    0, 2544, 2554, 1954, 2557, 2559,10448,10448, 1954,
 2560, 2562, 2564, 2565, 2567, 2568, 2570, 2571, 2572, 2577,
    0,    0,    0,    0,  561,    0,    0,    0, 9332,    0,
    0,    0, 1800,    0,  105, 1800,    0,    0, 1392,    0,
 2340,    0, 2576, 2584,    0,    0,    0,    0,    0,    0,
  471,  633,    0,    0,    0,    0, 1698,    0,    0,    0,
    0, 7765, 1573,  791, 2575,  606, 2589, 2295,10448, 2579,
 2601, 2583, 2585, 2586, 2587, 2309, 2596,    0,    0,    0,
 2353,12135, 2597,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2616, 5276,    0,    0,
 2306, 2329, 5276, 1875,    0,    0, 1069, 5276,    0,    0,
    0,    0,    0,    0,    0, 2235,    0,    0,    0,    0,
    0, 2235,    0,    0,    0,    0,  845, 1800,    0,    0,
10448, 1424,    0,    0,    0, 1424,    0,    0,10448,    0,
    0,    0,    0, 1954, 1954,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2622,    0, 2039,    0, 2039,
 6171, 5276, 1245, 4326,    0,    0, 2308,    0, 2634,    0,
    0,    0,  172,    0,10209, 5276,    0,    0,10448,    0,
 -190, 2342, 6171, 1954,    0, 6171,    0,    0,    0,    0,
 1503,    0,    0,  750,    0, 2229,    0,    0,    0,    0,
    0, 1875, 1069,    0, 2300,    0,    0,    0,    0, 2118,
    0,  845, 2646,  775,    0,    0, 2039, 1954,    0,    0,
 1214,    0, 2168, 2168,  471,    0,    0,    0, 4815, 1993,
    0, 2632,    0, 2247, 8757, 1453,10448, 1503,  713, 2635,
  737,  770,    0,    0,    0, 5276, 5276,  136,  156,    0,
    0,    0, 2300,  474, 1875, 2313, 2317,    0,    0,  775,
 2652,  845, 2358, 2168, 2663, 1800, 2153, 2153, 5276,    0,
 2670, 2668, 1145,    0,    0,    0, 2303, 1503,    0, 1494,
  204, 2279,    0, 2415,    0, 1503,    0,    0,    0,    0,
12168,  750, 1875,  795,  795,    0, 2423,    0, 2283,    0,
    0,    0,  775,  845,    0,    0, 2039, 2335, 2335,    0,
    0, 4815, 2681,    0, 5276,    0,  792, 2675,  993, 1503,
 2434, 2431,12168,12168,    0,    0,    0,    0,    0, 2275,
  988,    0,    0,    0, 1478, 2168,    0,    0,    0, 5276,
    0, 1503, 2695,    0,    0,  198, 5276,12168, 2433,    0,
    0,    0,    0, 6171, 2153, 2697, 2702, 2002,12168,    0,
    0,12168,  471, 2335, 5276,    0,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  206, 1078,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  144, 5598,    0,    0,    0,    0,    0,    0,
 2755,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1021,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  288,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2427,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2373,    0,    0,
    0,  148,    0,  148,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2374,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  946,    0,    0, 8366,
 5855,    0,    0,    0, 1009,    0,    0,    0, 2904,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   23,    0,    0,    0,    0,  554,    0,    0, 1025,
    0,    0,    0,    0,    0, 1824,    0,10809,    0,    0,
    0,    0,    0,    0,    0,    0,   23,    0,  -37,    0,
    0, 2718,    0,    0,    0, 2496,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1094,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1237,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1204,    0,    0,    0,    0,
    0,    0,    0, 5304,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  315,    0,    0, 1239,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   23,    0,
 2718,    0, 2496,    0, 1342,    0,    0,    0,    0,    0,
    0,    0,    0, 1359,    0, 2485,    0,    0,    0,    0,
 1410,    0,    0,    0,    0,    0, 2501,  865,12340,    0,
 1431,    0,    0,    0,    0,  263,    0,    0,    0, 2311,
 2311,    0,    0,    0, 1448, 2166,    0,    0,    0,    0,
    0,    0,    0, 2501,    0,    0, 1548,    0,    0,    0,
    0,    0,    0,    0, 8301, 2383,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1895,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2625,
    0,    0,    0, 2878, 3131,    0,    0,    0,    0,    0,
    0,    0,   95,    0,    0,    0,    0,    0,    0,    0,
    0,  180, 3384, 2319,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1470,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1472,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  378,    0,    0, 2446,    0,  300,
    0,    0,    0,    0,    0,  225,    0,  225,    0,    0,
  520,    0,  592,  790,    0,    0,    0,    0, 1094,    0,
 2501,    0,    0,    0,    0,  108,  945,    0,    0,   67,
    0, 3106,  277,    0,    0,    0,    0,    0,    0, 2311,
 2311, 2311, 2311,    0,  408,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2510,
    0,    0,    0,    0,    0, 2485,    0,    0, 1471,    0,
 2305,    0,    0,  765,    0,    0,    0,    0,  224,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1489,    0,
    0,    0,    0, 2310, 1511,    0,    0,    0, 8656,    0,
    0, 5212,    0,    0, 9229,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1600, 2224,
    0,    0,    0,    0,10767, 1771,    0, 9229,   23,    0,
    0,    0,    0,    0,    0,    0,    0,  865, 1221,    0,
    0,    0,    0, 2365,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11019,11151,11326,    0,    0,  169,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10528,    0,10528,    0,    0,    0,10528,10528,
10528,    0,    0,    0,    0,    0, 2331,    0,    0,    0,
    0, 4354, 4586, 8518, 2737,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  295,
    0,    0, 5598,    0,    0,  295,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  587,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  685,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  865,    0,  400,    0,    0,  902,    0,    0,    0,
 3489,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1998,    0,    0,    0,    0,    0,    0,
    0, 2750,    0,    0, 2648, 1931,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  358,  377,  392,    0,    0,
    0, 2888,    0,    0,    0,    0,    0, 2592,    0,    0,
    0,    0,    0, 5212,    0,    0,    0,    0,  891,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2501,    0,
    0,  313,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10310,    0,    0,    0,  229,    0,    0,    0,    0,
 2482,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,10528,    0,    0,    0,    0, 1297,
    0,    0,    0, 3616, 4839, 5071, 3869, 4101,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1538, 1540,    0,    0,    0,    0,    0,    0,    0, 1542,
    0,    0, 8366,    0, 1542, 1544, 1563,    0,    0,    0,
    0,    0,    0,  637, 1114,  764,    0,    0,    0, 1034,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  145,
    0,    0,    0,    0,    0,    0,    0, 2311, 2347,    0,
 3714,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2521,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1521,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1574,    0,    0,    0,    0,    0,
    0,    0, 1400,    0,    0,    0,    0,    0,    0,    0,
    0, 1327,    0,    0,    0,    0,    0,    0,    0, 1221,
 2521, 2311, 1595,  865,    0, 2529, 8366, 1596,  146,    0,
    0,    0,    0,10310,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2402,    0,    0,    0, 5614,    0,    0,    0,
    0,    0,    0,    0, 6156,    0, 6720,    0, 7815,    0,
    0,    0,    0,    0,    0,    0,10794,    0,    0,11235,
    0,    0,11403,    0,11409,    0,    0,11541,    0,    0,
11583,    0,    0, 2104,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2371,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1538, 1598,    0, 2380,  296,
    0,    0,    0,    0,    0,    0,    0, 1626, 1627, 1636,
    0, 2464, 1103,    0, 2709,    0, 2467,    0, 2470, 2480,
    0,    0,  406,    0,    0,    0, 2521,    0, 1596,    0,
    0, 3453,    0,    0,    0, 2359,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 5506,
    0, 2555,    0,    0,    0, 2135,    0, 1521, 2427,    0,
    0,    0,    0,    0,  395,  412,  445,  727,  837,  844,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  953,    0,    0,    0,    0, 2573,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1682,
    0,    0,    0,    0,    0,    0,    0, 1701,    0,    0,
    0,    0,    0,  121,    0,    0,    0,    0,    0,    0,
  147,    0,  283,    0,    0,    0, 8518,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11673,    0,11715,    0,
11747,    0,    0,    0,    0,11830,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1746,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1702,    0,    0,    0, 1381,    0,
    0,    0,    0,    0,    0,  555,    0,    0,    0,    0,
    0, 2311,    0,    0,    0, 1313,    0,    0,    0,    0,
    0,    0,    0, 2517,    0,    0, 2791, 1550, 2534,    0,
 2485,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2521,    0,    0,    0,    0,    0,    0,    0, 1711,
  971,    0,    0,    0,    0,    0,    0,    0,    0, 2521,
 2529,    0,  316,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  811,    0,11850,    0,    0,10528,
    0,    0,    0,    0,11946,    0,    0,    0,    0,11980,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  517,    0,  296,  851,    0,    0, 2464,    0,
  591,    0,    0,  648,    0,    0,    0,    0,    0,    0,
  735, 2799,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1037, 1457,    0,    0,    0,    0,    0,    0,    0,
    0, 2538,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2703,    0,    0,  115,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  185,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,12044,12076,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  662,    0,  871,
    0,    0,    0,    0,    0,    0, 2801,    0,    0,    0,
    0,    0, 2774,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2789,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2547,    0,    0,    0,    0,
    0, 2703,  399,    0,  107,    0,    0,    0,    0,    0,
    0,    0,  191,  231,    0,    0,  163, 2791,    0,    0,
    0,    0,  143,  364, 3241,    0,    0,    0, 2799,    0,
    0,    0,    0,    0,  114,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  585,    0, 1521,  825,  876,    0,    0, 2464,
 1393,    0,    0,  117,    0,  517,  160,  179, 1579,    0,
    0, 2811,    0,    0,    0,    0, 2798,    0,    0,    0,
 2578,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2703,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1074,    0,    0,    0,  662, 2817, 1310,    0,
    0, 2799,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2464,  143,    0,    0,    0,    0,
    0,    0, 2800,    0,    0,    0,    0,    0, 7936,    0,
    0,    0,    0,    0,  160, 2830,    0,    0,    0,    0,
    0,    0, 2708, 2817,    0,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2874,    0,    0, 2875,
    0,    0,    0,    0,    0,    0,   11,    0, 2877,    1,
  955,  -23, -589, 1694, 1016, -258,   42, 2869, 2612,    0,
  -28,    0, 2620,  -94,  -40,    0,  619, -874,    7,  -55,
  -36, -200, 2623, 2177,    0,    0,    0, 1460,    0,    0,
  -29, 1807, -544,   -2, -896, 2259, 2041, -304, 1811, 2809,
 -323,    0,  -60,    0, -476,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -566,   83, 1818, -277,  970, -926,    0,
    0,    0,    0, -336, -108, -709, 1827, -334,  -50,    0,
 -529,    0,    0, 1461, -994,    0, 1465,    0,    0, 2581,
 -624,   17,    0,    0,    0,    0, 2546,    0, 2332, 2582,
 -193, 1845, -716,    0,    0,    0, -717,    0,    0,    0,
    0,    0,    0, 1898,    0, 2603, 2163, 1842, 2580, 2591,
    0, 1274,    0, -454, 2145,  -35, -618, -796,    0,    0,
    0, -917, -915, -912, -271, -909,    0, 1523, -559,    0,
 1207,-1161, 1531,    0, 2392,    0,    0,    0, 1217, 1229,
 1189,  192, -299, -722, -380,    0,    0, 2183,    0, 1719,
    0,    0, -395,    0,    0,    0,    0, -179,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1252,    0, 1311,   55,    0,    0,  777,    0, -387,  235,
 1147, 1301,    0, 1210,    0, 1978, 1757, 1580,    0,-1061,
 -834, -908,    0, -492, -587,-1402,    0, -407, 1295,  771,
 -525, -498, -710, -850,-1157,    0,    0,   -3,    0,    0,
    0,    0,    0, 2636,    0,    0,    0,    0, 2651, 2428,
    0,    0,    0,    0,    0,   -9, 2662,    0,    0,    0,
    0,  -10,    0,    0,    0, -339, 2157, -337, 1717, -457,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2378,
    0,    0, 2171,    0,    0,    0,    0,    0,    0,  227,
    0, 2978,    0,    0,    0,    0,    0, 2696,    0,    0,
    0,    0, 2503,    0, 2500,    0,    0,    0, 2265,    0,
    0,    0, -464,    0,   77,    0, 2245,    0,    0,    0,
    0,    0,    0, -615, -965, -467,  584, -211, 1430,    0,
 2861, -120, 1436,    0, 2282, -626, 2285,    0, -818, 2034,
    0,    0, -141, -242,    0, 1385,-1181,    0,-1154,-1138,
-1120, 1493,    0, 1242,    0, 1284,    0, 1190,    0,    0,
    0,    0, 2523,    0,    0,    0, -424, 2325, 2066, -988,
    0,    0,    0, -676,   31,  347,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1462,
 1361, 1911,    0, -520,    0,    0,    0, -903,    0, 1814,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -831, 1876,    0,    0,
};
#define YYTABLESIZE 12944
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  308,  154,  259,
   37,  437,  208,  486,  572,  192,  748,  279,  282,  609,
  577,  610,  943,  784,  243,  544,  217,  209,  195,  278,
  281,  686,  204, 1245,  734,  247,  229,  895,  252,  754,
  222, 1007,  711,  186,  188,  191,  194,  148,  199,  248,
  542,  917,  639,  210,  188,  215,  254,  194,  253,  148,
  199,  461,  228,  127,  623,  219,  628,  482,  925,  991,
  461,  239,  149,  148,  241,  188,  224,  239,  215,  191,
  194,  250,  148,  199,  228,  127, 1423,  194,  249,  772,
 1172,  857,  775,  265,  670,  267,  313,  963,  786,  251,
 1051,  266, 1246, 1234,  246, 1235,  256,  620, 1236,  309,
 1234, 1238, 1235, 1254,  748, 1236,  695, 1239, 1238,  210,
  148,  210,  311,  993, 1239,  628,  507,  966, 1194,  534,
  848,  371, 1275,  215,  996,  670,  732, 1076,  670,  307,
  194,  148,  228,  566,  627,  352,  154,  256,  620,   77,
  256,  620,  835,  670, 1202,  748, 1270,  333,  748,  289,
 1024,   89,  683,  276,  508,  256,  620,  277,  696, 1031,
 1084, 1050,  327,  748,   77,  695,  437,  940,  699,  325,
   77, 1056,  360,  695,  697,  263,  277,  770,  760, 1000,
  357,  748,  817,  762,  763,  764,  765,   77,  747, 1762,
  699, 1377,  566,  627,  352,  566, 1099,  708, 1496,  696,
 1183,  751, 1516,  609,  718,  610,  897,  743,  513,  699,
  454,  683, 1071,  394,  594, 1292,  263,  696,  280,  317,
  358,  457, 1700,  980, 1459,  318,  493,  699,  325, 1518,
  457, 1756, 1293,  697,  577,  472,  210, 1756,  122,  357,
  148,  277,  463,  127, 1647, 1519,  277,  891,  706,  148,
  818,  463,  469,  496,  566,  500, 1143, 1144,  806,   85,
  506, 1458,   85, 1520,  358,  626,  461, 1603,  792, 1438,
  191, 1189,  394,  594,  148, 1381,  268,  568,  365,  358,
 1737, 1738,  334,  543,  739,  516, 1596, 1169, 1214,  987,
  546,   60,  209,  350,  559,  807,  158,  561, 1370,  737,
 1372,  759,  125,   83,  589,  195,  988, 1648,  631,  210,
  619,  469,  573,  772,  575,  508,  210,  829,  210, 1358,
  210, 1775, 1360,  564,  540,  461, 1433, 1195,  627,   97,
  351,  629,  352,  653,  565,  827,  568,  541, 1287, 1215,
 1038,  194,  199,  739,  516,  566, 1234,  380, 1235,  159,
  148, 1236,  191,  695, 1238,  353, 1186, 1057,  621,  335,
 1239,  125,   79,  589,  195, 1217,  378,  733, 1198, 1200,
  738,  892,  315, 1448, 1455,  366,   14, 1389,  202,  203,
  354,  379,  486,  519,  385, 1230, 1600, 1190,  748,  624,
 1503,  670,  670, 1825,  695,  695,  628,  463,  364,  808,
  689,  386, 1674,  828, 1604, 1506,  380,  705,  733,  498,
 1253,  733,  695,  122,  122,  620,  702, 1097, 1077,   89,
 1161,  506,  705,  268,  283,  378,  733,  710,  924,  748,
  624,  716,  748,  624,  382,  148,  457,  620,  194,  463,
  379,  719, 1516,  385,  939,  486,  148,  748,  624, 1761,
  148,  970,  812,  194,  695,  148,  463, 1791,  154,  626,
  386, 1028, 1310, 1311, 1378, 1312, 1313,  896,  486, 1518,
  729, 1463, 1410, 1531,  627, 1495,  696,  394, 1566, 1019,
 1567,  742, 1021,  683, 1397, 1519,  696,  748,  761,  696,
  511, 1829,  271,  382,  670, 1404, 1027,  461,  696, 1370,
 1722,  971,  697, 1520, 1298,  697, 1629,  788,  357,  595,
  628,  357, 1729,  327, 1083, 1594,  469, 1497,  626,  695,
  748, 1500,  770, 1045,  461,  637, 1193, 1091,  695, 1069,
  758,  857,  781,  210,  814,  670,  699, 1086,  781,  275,
  131, 1098, 1436,  374,  699,  696,  695,  697,  358,  834,
  637,  358,   76, 1766,  696,  699,  594,  992, 1605,  813,
  695, 1286, 1018,  681, 1509, 1451,  731,  262,  595,  566,
   79,  275,  461,  637,  256, 1015,  697,   76, 1319,  325,
  686,  605,  209,   76,  275,  842,  843, 1640,  627,  352,
 1593, 1799,  683, 1642, 1409,  508,  377,  256,  696,  620,
   76, 1385,  374,  699,  566,  748,  683,  707,  210,  653,
 1182,  380,  696,  984,  697,  256, 1425,  859,  256,  594,
  357,  686,  594,   79,  686, 1609,  666,  858,  697,   85,
  378,  733,  357,  256,  357,  697,  275,  682,  594,  686,
  605, 1015, 1677, 1013, 1014,  379,  589,  215,  385,  469,
  125,  739,  516,  195,  944,  463, 1450, 1627,  772, 1681,
  358, 1225,  865, 1464,  594,  386,  748,  666,  748,  705,
  666,  933,  358,  899,  358,  748, 1421, 1528,  682,  210,
  210,  589,  486,  463,  461,  666, 1529,  733,  463, 1255,
  129, 1470,  683,  891,  721, 1744,  682,  463,  382, 1586,
  194,  506,  500,  463,  990,  194,  209,  624,  729,  589,
  454, 1530,  589,  695,  832, 1736,  381, 1592, 1003, 1003,
 1579,  148,  891,  148,  694, 1288,  210,  156,  589,  624,
  546,  778,  210, 1595, 1176,  695,  210, 1179, 1244, 1237,
  695, 1257, 1728, 1752,  380, 1507, 1237,  256,  967,  695,
 1252, 1735, 1017,  657,  589, 1008,  210,  130, 1037, 1039,
 1521, 1670, 1424,  378, 1764,  694,  748, 1754,  344, 1086,
 1274,  748, 1234, 1365, 1235,  381,  147, 1236,  379,  590,
 1238,  385,  695,  694,  210,  210, 1239, 1173,  748,  891,
 1036,  695, 1771, 1256,  657,  748, 1044,  657,  386,  626,
  839, 1070,  374, 1756, 1437, 1491, 1387,  374, 1054,  508,
 1447, 1081,  657,  374,  252,  463,  231,  892, 1755,  733,
  148, 1812, 1066, 1067, 1068, 1696,  384,  733, 1009,  748,
  148,  382, 1345,  383,  697,  695, 1175,  697,  590,  420,
  697,  518, 1093,  240,  518,  345,  892,   79,  973, 1421,
 1415,  595,  256,  232,  256,  252,  733,  733,  252,  733,
  683,  256,  699, 1034,  157,  253,  733,  733,  733,  733,
  733, 1112,  733,  252, 1682,  733,  733,  733,  733,  733,
  135,  697, 1258, 1180,  699,  384,  595,  200, 1096,  748,
  326,  630,  383,  697,  697, 1177, 1452, 1113,  686,  697,
  831,  683,  697, 1181,  697, 1765,  253,  697, 1651,  253,
  216,  686,  486,  892,  595,  506,  697,  595,  437,  683,
  686,  236, 1100,  605,  253,  463,  194,  199,  463,  228,
  127,  699,  891,  595,  613,  564,  463,  984,  327,  135,
  374,  891,  136, 1178,  666,  760,  943,  666,  463,  463,
  630,  437,  256,  194,  666,  682,  133,  666,  605,  595,
  137,  666, 1363,  697,  891, 1635,  666,  686,  682,  666,
  920,  921,  697, 1432,  256,  613,  686,  682,  866,  321,
  381,  256,  683,  873,  699,  666,  605, 1187,  891,  605,
 1636,  782, 1237,  613,  564,  609, 1695,  610,  573, 1199,
 1628,  136, 1715, 1630, 1454,  605, 1524,  942,   76,  666,
  614,  486,   14,  666,  570,  256,  697,  134, 1709,  137,
  686, 1711,  666,  683,  682,  234, 1756,  322,  209, 1265,
 1693,  605, 1694,  682,  686,  454,   79,  244,  683,  666,
 1266, 1815,  694, 1265, 1164, 1165, 1101,  683, 1637,  255,
  328, 1716, 1281, 1388,  210, 1527,  892,  573, 1375, 1375,
 1379,  210, 1717,  359,  694,  892,  666, 1643,  329,  614,
 1301,  657,  982,  570,  518,  256, 1305,  682,  666,  598,
  272,  657,  683,  697,  657, 1207,  666,  982,  892, 1734,
  384,  682,  252,  657,  252, 1687,  681,  383,  256, 1208,
  486,  252,  330,  656, 1247, 1248, 1406,  359, 1408,  518,
  260,  694,  892,  381,  922,  666,  666,  789,  666,  695,
  694,  590,  359, 1209,  696,  666,  666,  666,  666,  666,
  598,  666,  241,  792,  666,  666,  666,  666,  666,  323,
  657,  273,  697,  253,  656,  253, 1665, 1357, 1733,  657,
  261,  961,  253, 1299,  695,  693,  590,  694,  135,  696,
  135, 1800,  656,  463,  694,  463, 1718,  105,  697, 1453,
 1399,  697, 1601,  241,  264,   79,   79, 1210,  694, 1806,
 1400,  463, 1646, 1003,  590,  307,  194,  590, 1166, 1833,
  425,  683,  252,  657, 1733, 1678,  106,  107,  790,  108,
  866, 1679, 1801,  590,  866,  657,  109,  110,  111,  112,
  113,  518,  114,  423,  252,  115,  116,  117,  118,  119,
  366,  252,  136,  384,  961, 1647,  327,  697,  572,  590,
  383,  630,  735, 1777,  268,  480,  697, 1733,  366,  791,
  137,  105, 1513,  253,  485,  695,  693,  683,  694, 1802,
  696,  295,  613,   79,  241,  252,  683, 1435,  135, 1778,
 1779,  941,  508, 1462, 1730,  253,  518, 1466,  105, 1733,
  106,  107,  253,  108,  613,  516,  942, 1822,  283,  269,
  109,  110,  111,  112,  113,  327,  114,  572, 1648,  115,
  116,  117,  118,  119,  105,  241,  270,  106,  107,  614,
  108,  530,  195, 1807, 1808, 1680,  253,  109,  110,  111,
  112,  113,  143,  114, 1773,  252,  115,  116,  117,  118,
  119,  284, 1276,  106,  107,  274,  108,  872,  614, 1823,
  872,  559,  115,  109,  110,  111,  112,  113,  496,  114,
  614,  683,  115,  116,  117,  118,  119, 1834,  401, 1276,
  614,  531, 1532,  283,  683, 1797, 1805,  691,  614, 1840,
  493,  195,  692,  683,  463, 1003,  253,  209,  506,  115,
  658, 1277,  677,  194, 1375,   98,  682,  683,  685,  872,
  723,  695,  693, 1577,  694,  532,  696, 1819, 1820,  518,
  559,  359,  691,  210,  359,  713,  893,  692, 1280,  402,
  508,  139,  148,  533,  115,  361,  681,  401, 1641,  493,
  683,  658, 1831,  659,  658,  290,   99,  144, 1237,  683,
  398,  656,  714, 1838,  145,  105, 1839,  659,  100,  658,
  518,  656, 1160,  518,  656,  659, 1094,  160,  722,  497,
 1095,  362,  724,  656,  695,  693, 1170,  694,  518,  696,
  518,  659,  437,  101,  106,  107,  306,  108,  402,   43,
  455,   51, 1645,  683,  109,  110,  111,  112,  113,  374,
  114, 1783, 1675,  115,  116,  117,  118,  119,  161,  398,
  374, 1626, 1513,  691,  695,  693,  316,  694,  692,  696,
  656,   79,  987,   77,  498,  530,  160,  691,  319,  656,
  528, 1749,  692,  359,  455,  659,  506,  987, 1784, 1513,
  250,  499,  291,  188, 1676,  359,  576,  359,   43,  455,
   51, 1785,  102,  725,   52,  695,  693,   41,  694,   24,
  696,  744,  103,   47, 1673,   53, 1652,  161,  275,  216,
  528,  528,   54,  656,  528,  531,  701,  292,  933,  700,
  420,  250,   49,  148,  250,  656, 1513,  104,  191,  528,
  195,  528, 1689,  471,  521,  983, 1690,   55, 1723,  250,
  799,  283,  800,  522,  283,  293,  149,  148,  294,  532,
  216,  195,  347,  216,  127,  369,   41,   25,   24,  495,
  744,  528,   47,  336,  295, 1705, 1513,  533,  216,  926,
  695,  693,  195,  694, 1513,  696,  872, 1571,  195,  508,
  753,   49,  933,  210,  901,   48,   50,  903, 1511,  691,
  296, 1644,  471,  485,  692,   45,  194,  900,  349, 1572,
  495,  195,   56,  495,  916, 1513, 1573,  753, 1513,  923,
  423,  508,  508,  127,  369, 1757,   25,  518,  495,  930,
  195,  934,  935,  936,  937,  938,  423,  930,  195,  195,
 1513,  149,  148,  283,   57,  195,  508,  518,   58,  518,
 1574,  128,  521,  518,   48,   50,  518,  508, 1739,  148,
  508,  522,  691,  195,   45,  375,  485,  692,  658,  518,
  116,   46,   99,  380,  105,  518,  188, 1759,  658,  483,
  216,  658, 1207,  360,   79,  358,  905,  491, 1760,  485,
  658,  149,  148,  681,  516,  506, 1208,  360, 1739,  148,
  195, 1811,  691,  106,  107,  360,  108,  692,  368,  101,
  128,  195, 1259,  109,  110,  111,  112,  113,  372,  114,
 1209,  360,  115,  116,  117,  118,  119,  506,  506,  116,
   46,  370, 1751,  149,  148,   80,   81,  658,  528,  216,
  523, 1739,  148,  691, 1631,  191,  658,  518,  692, 1260,
  695,  693,  506,  694,  528,  696,  873,  528,  528,  873,
  528,  373, 1748,  506,  528,  150,  506,  528, 1207,  518,
  191, 1369, 1789, 1371, 1210,  360,  518, 1093,  102,  376,
  528,  523, 1208,  377,  523,  466,  528,  518,  103,  528,
  658,  777,  458,  151,  776,  191,  467,  151,  470,  523,
  471,  523,  658, 1790,  927,  518, 1209,  697,  873,  473,
  518, 1813,  152,  104, 1816, 1102, 1103, 1107,  691,  475,
 1115, 1117, 1119,  692, 1127, 1130, 1133, 1135, 1138, 1141,
 1824,  523, 1145, 1146,  476, 1148, 1828, 1150, 1511,  518,
 1153, 1154, 1155, 1156, 1157, 1158,  477,  495,  338,  495,
  492,  339,  151,  495,  202,  203,  495, 1162,  528,  423,
 1210,  396,  397,  398,  980, 1714,  138,  139,  495,  495,
  695,  693,  504,  694,  962,  696,  494,  961,  695,  693,
  528,  694,  514,  696,   14,  710,   76,  528,  105,  406,
  407,  528, 1075,  529, 1082, 1074,  340,  853,  528, 1174,
  485,  520, 1173,  485,  957,  980,  980,  980,  980,  980,
  980,  980, 1714, 1046, 1047,  537,  528,  106,  107,  550,
  108,  528,  710,  980,  980,  980,  980,  109,  110,  111,
  112,  113,  105,  114,  557,  710,  115,  116,  117,  118,
  119,  485, 1218,  560,  485, 1074,  528,  495,  695,  693,
  528,  694, 1714,  696,  570,  980,  574,  980,  624,  485,
 1796,  106,  107,  630,  108,  695,  693,  518,  694,  495,
  696,  109,  110,  111,  112,  113,  495,  114,  123,  123,
  115,  116,  117,  118,  119,  666, 1224, 1268,  691, 1223,
 1267, 1714, 1273,  692, 1714, 1272, 1383, 1472,  523, 1382,
 1173, 1502,  105,  628,  697, 1121, 1508, 1231,  518,  697,
  495,  518, 1837,  632,  523, 1756, 1714,  523,  523, 1323,
  523, 1122,  663,  664,  523,  665,  518,  523,  518,  667,
  668,  106,  107,  669,  108,  670,  671,  672,  673,  674,
  523,  109,  110,  111,  112,  113,  523,  114,  930,  523,
  115,  116,  117,  118,  119,  675,  680,  681,  690,  698,
 1326, 1327,  703, 1329, 1331,  697, 1123,  717,  894,  699,
  898, 1336,  151,  766,  902,  904,  906,  708,  972,  695,
  693,  720,  694,  727,  696,  725,  741,  745,  750,  746,
 1349,  752,  755,  151,  753, 1124,  756,  521, 1356,  766,
  423,  231,  767,  423,  229, 1162,  770,  769,  691, 1364,
  423, 1366, 1367,  692,  766,  803,  691,  766,  523,  779,
  804,  692,  151,  805,  810,  275,  980,  815,  980,  980,
  819,  821,  766,  485,  820,  152,  980,  822,  232, 1479,
  523,  151,  825,  423,  980,  229,  824,  523,  229,  980,
  151,  523,  980, 1152,  695,  693,  151,  694,  523,  696,
  830,  836, 1125,  229,  837,  840,  980,  841, 1314,  850,
  852,  980,  980,  853,  151,  980,  523,  861,  485,  863,
  485,  523,  980, 1417,  980,  980,  691,  485,  980,  862,
  867,  692,  980,  518,  152,  980,  918,  980,  928,  980,
  485,  957,  956,  691,  980,  980,  523,  980,  692,  959,
  523, 1116,  964, 1120,  968, 1128, 1131,  969, 1136, 1139,
 1142,  979,  985,  980,  980,  518,  980,  997,  994, 1001,
 1015, 1020,  485, 1025,  518,  831, 1026,  518, 1028, 1032,
  980,  518,  980,  980,  980,  782, 1033,  980, 1035, 1041,
  980,  980,  518, 1467, 1042, 1043, 1049,  384,  695,  693,
  980,  694, 1475,  696, 1052, 1065, 1058,  518, 1072, 1480,
 1063, 1077, 1089,  518,  980, 1746,  518,  980,  485,  516,
 1343,  695,  693, 1090,  694,  386,  696,  713,  961, 1147,
 1344,  695,  693,  980,  694, 1109,  696, 1149,  423,  980,
  485, 1151,  423, 1171,  980, 1185, 1159,  485,  695,  693,
  691,  694, 1188,  696,  388,  980,  980,  691,  980, 1191,
  516,  485,  692,  516,  980, 1183, 1201, 1204, 1206,  961,
  961,  961,  961,  961,  973,  961, 1212,  700,  516, 1221,
  516,  485, 1226, 1229, 1240, 1249, 1251,  961,  961,  961,
  961, 1261, 1269,  980,  980, 1271,  980, 1278, 1282, 1283,
 1284, 1294, 1285,  980,  980,  980,  980,  980, 1296,  980,
 1083,  821,  980,  980,  980,  980,  980, 1303,  518,  961,
 1306,  961,  241, 1307,  229,  518, 1308, 1309, 1315,  393,
 1316,  766,  691, 1320,  766, 1321, 1322,  692,  891, 1324,
 1325,  766, 1332,  518,  766,  395, 1333, 1713,  766, 1334,
 1335, 1362,  821,  766,  152,  821,  766, 1614, 1615,  396,
  397,  398, 1337,  202,  203, 1338, 1339, 1340, 1341, 1342,
  821, 1386,  766,  518, 1359,  152, 1368,  518,  423, 1384,
 1390, 1391,  401,  402,  403,  404,  405,  406,  407,  408,
  409, 1392, 1394, 1393, 1713, 1405,  766,  411,  412,  766,
  766, 1347,  695,  693,  152,  694,  416,  696, 1395,  766,
 1407,  518, 1402,  518, 1412, 1414,  552,  518, 1654, 1422,
  518, 1426,  241,  152, 1434, 1427,  766, 1428, 1449, 1456,
 1460, 1429,  152,  518, 1713, 1439,  691, 1430,  152, 1431,
 1461,  692, 1795,  230,  241, 1465, 1317, 1471,  766, 1492,
 1498,  241, 1499,  766, 1505, 1504,  152, 1568, 1522,  691,
 1578, 1583, 1580, 1581,  692,  766, 1582,  766, 1478,  691,
 1584, 1585, 1587, 1713,  692, 1588, 1713,  516, 1589, 1597,
 1688, 1599,  518, 1607,  230,  241,  691,  230, 1691, 1608,
  961,  692,  961,  961, 1610, 1602,  516,  516, 1713,  516,
  961,  513,  230,  516, 1611, 1606,  516, 1612,  961, 1613,
 1616,  518, 1617,  961, 1618, 1619,  961, 1620, 1621,  516,
 1622, 1623, 1624,  518, 1704,  516,  518, 1625, 1706, 1632,
  961, 1633,  961,  518,  984,  961,  961, 1634, 1653,  961,
  518,  518,  513, 1650,  130,  513,  961, 1655,  961,  961,
 1656, 1657,  961, 1658, 1659, 1660,  961,  222, 1661,  961,
  513,  961,  513,  961, 1662, 1668, 1669, 1671,  961,  961,
 1663,  961, 1692,  821,  518,  984,  984,  984,  984,  984,
  984,  984, 1672, 1699,  423, 1698, 1750,  961,  961, 1664,
  961, 1708, 1724,  984,  984,  984,  984,  516,  222, 1732,
 1745,  222, 1772, 1753,  961, 1767,  961,  961,  961, 1769,
 1774,  961,  250, 1776,  961,  961,  222,  362,  664,  516,
 1781, 1782, 1787, 1793,  961,  984,  516,  984, 1794,  821,
 1810,  158,  821, 1764, 1765,  516, 1817,  516,  961,  821,
  691,  961,  821, 1814, 1818,  692,  821, 1821, 1756, 1832,
 1835,  821, 1836,  250,  821,  516,  250,  961,  362,  664,
  516,  362,  664,  961,    1,   29,   31,  202,  961,  748,
  821,  250,  366,  620,  270,  649,  362,  664,  148,  961,
  961,  516,  961,  268,  452,   81, 1468, 1469,  961,  516,
  670,  944, 1473, 1474,  821, 1476, 1477,  821,  821,  212,
  516,  275,  394, 1481, 1482, 1483, 1484,  821, 1485, 1486,
 1487, 1488, 1489, 1490,  192,  943,  681,  961,  961,  679,
  961,  241,  675,  230,  821,  516,  193,  961,  961,  961,
  961,  961,  677,  961,  288,  274,  961,  961,  961,  961,
  961,  367,  222, 1348,  695,  693,  821,  694,  705,  696,
  704,  821, 1350,  695,  693,  286,  694,  322,  696,  513,
  518,  706,  518,  821,  287,  821,  330,  614,  328,  518,
  907,  908,  909,  910,  911,  912,  913,  914,  513,  513,
  715,  513,  518,   27,   30,  513,   39,  989,  513, 1373,
  135,  339,  474,  465,  958,  464,  984,  195,  984,  984,
  851,  513, 1304, 1088, 1302,  245,  984,  513, 1300, 1290,
 1590,  583,  525,  673,  984, 1591,  539, 1279, 1228,  984,
  816,  241,  984,  512,  995, 1291, 1701,  554,  989,  989,
  989,  989,  989,  989,  989,  222,  984,  222,  555, 1763,
 1030,  984,  984,  241,  222,  984,  989,  989,  989,  989,
  241, 1570,  984, 1770,  984,  984,  195,  673,  984,  798,
  518, 1576,  984, 1804, 1768,  984, 1747,  984, 1023,  984,
 1416, 1707,  673, 1830,  984,  984, 1721,  984,  989,  513,
  989, 1798,  518, 1219,  241, 1398, 1731, 1525,  562,  518,
  250,  545,  250,  984,  984,  536,  984,  802, 1064,  250,
  137,  513, 1446,  505, 1055,  838,  736,  740,  513,  999,
  984,  288,  984,  984,  984,  986, 1203,  984,  981,  513,
  984,  984,  982,  518, 1638, 1351,  695,  693, 1697,  694,
  984,  696, 1780, 1809, 1841,  222,  664,  513,  362,  664,
 1197,  749,  513, 1328,  984,  362,  664,  984,  362,  664,
  965, 1361,  362,  664, 1411,    0,    0,  222,  664,    0,
  362,  664,    0,  984,  222,    0, 1352,  695,  693,  984,
  694,  513,  696,    0,  984,    0,  362,  664,    0,    0,
    0,    0,  691,    0,    0,  984,  984,  692,  984,    0,
  250,  691,    0,    0,  984,    0,  692,    0,  222,    0,
  362,  664,    0,    0,    0,  664,    0, 1353,  695,  693,
    0,  694,  250,  696,  664,  176, 1354,  695,  693,  250,
  694,    0,  696,  984,  984,    0,  984,    0,    0,    0,
  362,  664,    0,  984,  984,  984,  984,  984,    0,  984,
  985,    0,  984,  984,  984,  984,  984,    0,    0,  989,
    0,  989,  989,  250,    0,    0,    0,  362,  664,  989,
    0,    0, 1355,  695,  693,    0,  694,  989,  696,  362,
  664,  362,  989,    0,  176,  989,  195,    0,    0,    0,
    0,  985,  985,  985,  985,  985,  985,  985,    0,  989,
    0,    0,    0,    0,  989,  989,    0,  195,  989,  985,
  985,  985,  985,    0,    0,  989,    0,  989,  989,    0,
    0,  989,    0,    0,    0,  989,    0,    0,  989,    0,
  989,    0,  989,    0,    0,    0,  195,  989,  989,    0,
  989,  985,    0,  985,  673,    0,    0,    0,    0,    0,
    0,  673,    0,    0,  673,  195,  989,  989,  673,  989,
  661,    0,    0,  195,  195,    0,  673,    0,    0,    0,
  195,    0,    0,  989,  691,  989,  989,  989,    0,  692,
  989,    0,  673,  989,  989,    0,    0,    0,  195,    0,
    0,    0,    0,  989,    0,    0,    0,    0,    0,    0,
  195,  661,  195,    0,  661,    0,  673,  989,    0,    0,
  989,    0, 1493,  695,  693,  691,  694,    0,  696,  661,
  692,    0,    0,    0,    0,  195,  989, 1494,  695,  693,
    0,  694,  989,  696,    0,    0,  673,  989, 1501,  695,
  693,  673,  694,    0,  696,    0,    0,    0,  989,  989,
  195,  989,    0,    0,    0,    0,  691,  989,    0,    0,
    0,  692,    0,  673,    0,  691,    0,    0,    0,    0,
  692,    0,    0,    0,    0,  673,    0,  673,    0,    0,
    0,    0,    0,  176,    0,    0,  989,  989,    0,  989,
    0,    0,    0,    0,    0,    0,  989,  989,  989,  989,
  989,    0,  989,  842,  176,  989,  989,  989,  989,  989,
   62,  691,  985,    0,  985,  985,  692,    0,    0,    0,
    0,   63,  985,    0,    0,  176,    0,    0,   64,    0,
  985,  176,    0,    0,    0,  985,    0,    0,  985,    0,
    0,   65,    0,    0,  842,  842,  842,  842,  842,    0,
  842,    0,  985,    0,  176,    0,    0,  985,  985,    0,
    0,  985,  842,  842,  842,  842,    0,    0,  985,    0,
  985,  985,  180,  176,  985,    0,    0,    0,  985,    0,
    0,  985,  176,  985,    0,  985,    0,    0,  176,    0,
  985,  985,    0,  985,    0,    0,  842,    0,    0,    0,
    0,    0,   66,    0,    0,    0,  176,    0,  186,  985,
  985,    0,  985,  180,    0,    0,  180,    0,   67,    0,
    0,    0,    0,    0,    0,    0,  985,    0,  985,  985,
  985,  180,    0,  985,    0,   68,  985,  985,    0,    0,
    0,    0,    0,  176,    0,    0,  985,    0,    0,    0,
   69,  691,    0,    0,   70,    0,  692,   71,    0,    0,
  985,    0,    0,  985,    0,   72,  691,  186,    0,    0,
    0,  692,    0,    0,    0,    0,    0,  691,  661,  985,
    0,  661,  692,    0,   73,  985,    0,    0,  661,    0,
  985,  661,    0,    0,    0,  661,    0,    0,    0,    0,
  661,  985,  985,  661,  985,    0,   84,    0,    0,    0,
  985,    0,    0,   85,    0,    0,    0,   86,   87,  661,
    0,    0,    0,    0,    0,    0,   88,    0,    0,    0,
    0,    0,    0,    0,    0,  854,    0,    0,   89,  985,
  985,    0,  985,  661,    0,    0,    0,  661,    0,  985,
  985,  985,  985,  985,    0,  985,  661,    0,  985,  985,
  985,  985,  985,    0,    0,  842,    0,  842,  842,    0,
    0,    0,    0,  661,    0,  842,  854,  854,  854,  854,
  854,    0,  854,  842,    0,    0,    0,    0,  842,    0,
    0,  842,    0,    0,  854,  854,  854,  854,    0,    0,
  661,    0,    0,   90,    0,  842,    0,    0,    0,    0,
  842,  842,  661,    0,  842,    0,    0,    0,    0,    0,
   91,  842,    0,  842,  842,    0,    0,  842,  854,    0,
  180,  842,    0,  187,  842,   92,  842,    0,  842,   93,
    0,    0,    0,  842,  842,    0,  842,    0,    0,    0,
   94,  180,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,  842,    0,  842,  186,    0,    0,   95,
    0,    0,  180,    0,    0,    0,    0,    0,  180,  842,
    0,  842,  842,  842,    0,    0,  842,  186,    0,  842,
  842,    0,  187,    0,    0,    0,    0,    0,    0,  842,
    0,  180,    0,    0,    0,    0,    0,    0,  186,    0,
    0,    0,    0,  842,  186,    0,  842,    0,    0,    0,
  180,    0,    0,    0,    0,    0,    0,    0,    0,  180,
    0,    0,  842,    0,    0,  180,    0,  186,  842,    0,
    0,    0,    0,  842,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  180,  842,  842,  186,  842,    0,    0,
    0,    0,    0,  842,    0,  186,  926,  695,  693,    0,
  694,  186,  696,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  889,  888,  890,  857,  186,
  180,    0,  842,  842,    0,  842,    0,  854,    0,  854,
  854,    0,  842,  842,  842,  842,  842,  854,  842,    0,
    0,  842,  842,  842,  842,  842,    0,    0,    0,    0,
  854,    0,    0,  854,    0,    0,  186,    0,    0,  857,
  857,  857,  857,  857,    0,  857,    0,  854,    0,    0,
    0,    0,  854,  854,    0,    0,  854,  857,  857,  857,
  857,    0,    0,  854,    0,  854,  854,    0,    0,  854,
    0,    0,    0,  854,    0,    0,  854,    0,  854,    0,
  854,    0,    0,    0,    0,  854,  854,    0,  854,    0,
    0,  857,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  187,    0,    0,  854,  854,    0,  854,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  854,  187,  854,  854,  854,    0,    0,  854,    0,
    0,  854,  854,    0,    0,    0,    0,    0,    0,    0,
    0,  854,    0,  187,    0,    0,    0,    0,    0,  187,
    0,    0,    0,    0,    0,  854,    0,    0,  854,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  187,   98,  854,    0,    0,    0,    0,    0,
  854,    0,    0,    0,    0,  854,    0,    0,    0,    0,
    0,  187,    0,    0,    0,    0,  854,  854,    0,  854,
  187,    0,    0,    0,    0,  854,  187,  876,    0,    0,
    0,    0,    0,    0,   99,  691,    0,    0,    0,    0,
  692,    0,    0,  877,  187,    0,  100,    0,    0,    0,
  858,    0,    0,    0,  854,  854,    0,  854,    0,    0,
  132,    0,    0,    0,  854,  854,  854,  854,  854,    0,
  854,  101,    0,  854,  854,  854,  854,  854,    0,  878,
  857,  187,  857,  857,    0,    0,    0,    0,  879,    0,
  857,  858,  858,  858,  858,  858,  880,  858,    0,    0,
    0,    0,    0,    0,    0,    0,  857,    0,    0,  858,
  858,  858,  858,    0,    0,  881,    0,  882,    0,    0,
  857,    0,    0,    0,    0,  857,  857,    0,    0,  857,
    0,  883,    0,  884,  885,  886,  857,    0,  857,  857,
  102,    0,  857,  858,    0,    0,  857,    0,    0,  857,
  103,  857,    0,  857,    0,    0,    0,    0,  857,  857,
    0,  857,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  104,    0,  857,  857,    0,
  857,    0,    0,    0,  887,    0,    0,    0,    0,    0,
    0,    0,    0,  105,  857,    0,  857,  857,  857,    0,
    0,  857,    0,    0,  857,  857,  728,    0,    0,    0,
    0,    0,    0,    0,  857,    0,    0,    0,    0,    0,
    0,    0,  106,  107,    0,  108,    0,    0,  857,    0,
    0,  857,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,    0,  857,    0,    0,
    0,    0,    0,  857,    0,    0,    0,    0,  857,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  857,
  857,    0,  857,    0,    0,    0,    0,    0,  857,    0,
    0,    0,  695,  693,    0,  694,    0,  696,    0,    0,
    0,    0,    0,  552,    0,    0,    0,    0,    0,    0,
  889,  888,  890,  852,    0,    0,    0,  857,  857,    0,
  857,    0,  858,    0,  858,  858,    0,  857,  857,  857,
  857,  857,  858,  857,    0,    0,  857,  857,  857,  857,
  857,    0,    0,    0,    0,    0,    0,    0,  858,    0,
    0,    0,    0,    0,  852,    0,  852,  852,  852,    0,
    0,    0,  858,    0,    0,    0,    0,  858,  858,    0,
    0,  858,  852,  852,  852,  852,    0,    0,  858,    0,
  858,  858,    0,    0,  858,    0,    0,    0,  858,    0,
    0,  858,    0,  858,    0,  858,    0,    0,    0,    0,
  858,  858,    0,  858,    0,    0,  852,    0,    0,    0,
    0,    0,  105,    0,    0,    0,    0,    0,    0,  858,
  858,    0,  858,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  858,    0,  858,  858,
  858,  106,  107,  858,  108,    0,  858,  858,    0,    0,
    0,  109,  110,  111,  112,  113,  858,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,    0,    0,
  858,    0,    0,  858,    0,    0,  105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   98,  858,
    0,    0,    0,    0,    0,  858,    0,    0,    0,    0,
  858,    0,    0,    0,    0,  106,  107,    0,  108,    0,
    0,  858,  858,    0,  858,  109,  110,  111,  112,  113,
  858,  114,  876,    0,  115,  116,  117,  118,  119,   99,
  691,    0,    0,    0,    0,  692,    0,    0,  877,    0,
    0,  100,    0,    0,    0,  851,    0,    0,    0,  858,
  858,    0,  858,    0,    0,    0,    0,    0,    0,  858,
  858,  858,  858,  858,    0,  858,  101,    0,  858,  858,
  858,  858,  858,    0,  878,  852,    0,  852,  852,    0,
    0,    0,    0,  879,    0,  852,  851,    0,  851,  851,
  851,  880,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  852,    0,    0,  851,  851,  851,  851,    0,    0,
  881,    0,  882,    0,    0,  852,    0,    0,    0,    0,
  852,  852,    0,    0,  852,    0,  883,    0,  884,  885,
  886,  852,    0,  852,  852,  102,    0,  852,  851,  389,
    0,  852,    0,    0,  852,  103,  852,    0,  852,    0,
    0,    0,    0,  852,  852,    0,  852,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  104,    0,  852,  852,    0,  852,    0,    0,    0,  887,
    0,    0,    0,    0,    0,    0,    0,    0,  105,  852,
    0,  852,  852,  852,    0,    0,  852,    0,    0,  852,
  852,    0,    0,    0,    0,    0,    0,    0,    0,  852,
    0,    0,    0,    0,    0,    0,    0,  106,  107,    0,
  108,    0,    0,  852,  558,    0,  852,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115,  116,  117,  118,
  119,    0,  852,   76,    0,  399,    0,    0,  852,    0,
    0,    0,    0,  852,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,  852,  852,    0,  852,    0,    0,
    0,    0,    0,  852,  106,  410,    0,  108,    0,    0,
    0,    0,    0,  413,  414,  415,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,  853,    0,
    0,    0,  852,  852,    0,  852,    0,  851,    0,  851,
  851,    0,  852,  852,  852,  852,  852,  851,  852,    0,
    0,  852,  852,  852,  852,  852,    0,    0,    0,  945,
    0,    0,    0,  851,    0,    0,    0,    0,    0,  853,
    0,  853,  853,  853,    0,    0,    0,  851,    0,    0,
    0,    0,  851,  851,    0,    0,  851,  853,  853,  853,
  853,    0,    0,  851,    0,  851,  851,    0,    0,  851,
    0,    0,    0,  851,    0,    0,  851,    0,  851,    0,
  851,    0,    0,    0,    0,  851,  851,    0,  851,    0,
    0,  853,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,  851,  851,    0,  851,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  851,    0,  851,  851,  851,  106,  107,  851,  108,
    0,  851,  851,    0,    0,    0,  109,  110,  111,  112,
  113,  851,  114,    0,  948,  115,  116,  117,  118,  119,
    0,    0,    0,    0,    0,  851,    0,    0,  851,    0,
    0,  105,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  851,    0,    0,    0,    0,    0,
  851,    0,    0,    0,    0,  851,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,  851,  851,    0,  851,
  109,  110,  111,  112,  113,  851,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  856,    0,    0,    0,  851,  851,    0,  851,    0,    0,
    0,    0,    0,    0,  851,  851,  851,  851,  851,    0,
  851,    0,    0,  851,  851,  851,  851,  851,    0,    0,
  853,    0,  853,  853,    0,    0,    0,    0,    0,    0,
  853,  856,    0,  856,  856,  856,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  853,    0,    0,  856,
  856,  856,  856,    0,    0,    0,    0,    0,    0,    0,
  853,    0,    0,    0,    0,  853,  853,    0,    0,  853,
    0,    0,    0,    0,    0,    0,  853, 1635,  853,  853,
    0,    0,  853,  856,    0,    0,  853,    0,    0,  853,
    0,  853,    0,  853,    0,    0,    0,    0,  853,  853,
    0,  853, 1636,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853,  853,    0,
  853,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  546,    0,    0,  853,    0,  853,  853,  853,    0,
    0,  853,    0,    0,  853,  853,    0,    0,    0,    0,
    0,    0,    0,    0,  853,    0,    0,    0,    0,    0,
 1637,    0,    0,    0,    0,    0,    0,    0,  853,    0,
    0,  853,  546,    0,    0,  546,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853,    0,    0,
  546,    0,  546,  853,  105,    0,    0,    0,  853,    0,
    0,    0,  647,    0,    0,  417,    0,  418,    0,  853,
  853,    0,  853,    0,    0,    0,    0,    0,  853,    0,
  420,    0,  546,  106,  107,  277,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,  853,  853,    0,
  853,    0,  856,    0,  856,  856,    0,  853,  853,  853,
  853,  853,  856,  853,    0,    0,  853,  853,  853,  853,
  853,    0,    0,    0,    0,    0,    0,    0,  856,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  856,    0,    0,    0,    0,  856,  856,  647,
    0,  856,  417,    0,  418,    0,    0,    0,  856,    0,
  856,  856,    0,    0,  856,    0,  455,  420,  856,    0,
    0,  856,  277,  856,    0,  856,    0,    0,    0,    0,
  856,  856,    0,  856,    0,    0,    0,    0,    0,    0,
    0,    0,  105,    0,    0,    0,    0,    0,    0,  856,
  856,    0,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  856,    0,  856,  856,
  856,  106,  107,  856,  108,    0,  856,  856,    0,    0,
    0,  109,  110,  111,  112,  113,  856,  114,    0,  546,
  115,  116,  117,  118,  119,    0,    0,    0,    0,    0,
  856,    0,    0,  856,    0,  546,  459,    0,    0,  546,
    0,  546,    0,    0,    0,  546,    0,    0,  546,  856,
    0,    0,    0,  105,    0,  856,    0,    0,    0,  381,
  856,  546,    0,    0,    0,    0,    0,  546,  382,    0,
  546,  856,  856,    0,  856,    0,    0,    0,    0,    0,
  856,  383,  106,  107,    0,  108,    0,  384,  385,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  117,  118,  119,  641,    0,    0,  856,
  856,    0,  856,    0,    0,  386,    0,    0,  387,  856,
  856,  856,  856,  856,    0,  856,    0,    0,  856,  856,
  856,  856,  856,    0,    0,    0,    0,    0,    0,  546,
    0,    0,    0,    0,    0,    0,  389,  390,    0,    0,
    0,    0,    0,    0,    0,    0,  381,    0,  391,    0,
    0,  546,  392,  790,    0,  382,    0,    0,  546,    0,
  868,    0,  546,    0,    0,    0,    0,    0,  383,  546,
    0,  640,    0,    0,  384,  385,    0,  640,    0,  640,
  640,    0,  640,    0,    0,    0,    0,  546,    0,    0,
    0,    0,  546,  641,  790,  640,    0,  790,    0,  684,
  640,    0,  386,  643,    0,  387,    0,    0,    0,  393,
    0,    0,  790,    0,    0,  394,    0,  546,    0,    0,
    0,  546,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,  389,  390,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,  391,    0,    0,    0,  392,
    0,    0,    0,  400,    0,    0,    0,  642,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,  105,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,    0,
  115,  116,  117,  118,  119,  869,  870,  871,    0,    0,
  643,    0,    0,  562,  106,  107,  393,  108,    0,    0,
    0,    0,  394,    0,  109,  110,  111,  112,  113,    0,
  114,    0,  395,  115,  116,  117,  118,  119,    0,    0,
    0,    0,  562,  562,    0,  562,  396,  397,  398,  399,
  202,  203,  562,  562,  562,  562,  562,    0,  562,    0,
  400,  562,  562,  562,  562,  562,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  644,
  645,  646,    0,    0,    0,    0,    0,  640,    0,    0,
    0,  945,    0,    0,  640,    0,    0,    0,    0,    0,
    0,    0,    0,  640,    0,  790,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  640,    0,    0,    0,
    0,    0,  640,  640,  634,    0,  634,  634,    0,  634,
  640,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  634,    0,    0,    0,    0,  634,    0,    0,
  640,    0,    0,  640,    0,    0,    0,    0,    0,    0,
    0,  790,    0,    0,  790,  946,    0,    0,    0,    0,
    0,  790,    0,    0,  790,    0,    0,    0,  790,  640,
    0,  640,  640,  790,    0,    0,  790,    0,    0,    0,
    0,    0,    0,  640,    0,  280,    0,  640,    0,    0,
    0,  947,  790,    0,    0,    0,  948,    0,    0,    0,
  647,    0,  949,  417,    0,  418,    0,    0,    0,    0,
    0,    0,    0,  105,  280,  280,  790,  280,  420,  790,
  790,  950,    0,  277,  280,  280,  280,  280,  280,  790,
  280,    0,    0,  280,  280,  280,  280,  280,    0,    0,
    0,    0,  106,  107,  640,  108,  790,    0,    0,    0,
  640,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  640,  115,  116,  117,  118,  119,    0,    0,  790,    0,
    0,    0,    0,  790,  640,  640,  640,  640,  640,  640,
    0,    0,    0,    0,    0,  790,    0,  790,  640,    0,
    0,    0,    0,    0,    0,    0,    0,  640,  640,  640,
  640,  640,  640,  640,  640,  640,  640,  640,    0,  640,
    0,    0,  640,  640,    0,  640,  640,  640,  640,  640,
  640,  640,  640,    0,    0,  640,  640,  640,  640,  640,
    0,    0,    0,    0,  634,    0,    0,    0,    0,    0,
    0,  634,    0,    0,    0,    0,    0,    0,    0,    0,
  634,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  634,    0,    1,    0,    0,    0,  634,
  634,    0,    0,    0,    0,  791,    0,  634,    0,    0,
    0,    0,    0,    0,    0,    0,    2,    0,    0,    0,
    0,    0,    0,    0,    3,    0,    0,  634,    0,    0,
  634,    4,    0,    5,    0,    6,    0,    0,    0,    0,
    7,    0,    0,    0,    0,    0,  791,    8,    0,  791,
    0,    0,    0,    0,    0,    0,  634,  381,  634,  634,
  647,    9,    0,  417,  791,  418,  382,    0,    0,    0,
  634,    0,   10,    0,  634,    0,    0,    0,  420,  383,
    0,    0,    0,  277,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  641,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  634,    0,    0,    0,    0,    0,  634,    0,   11,
    0,    0,    0,    0,  389,  390,   12,  634,    0,   13,
    0,   14,   15,    0,    0,    0,  391,    0,    0,    0,
  392,  634,  634,  634,  634,  634,  634,    0,  864,    0,
    0,    0,    0,    0,    0,  634,   16,    0,    0,    0,
    0,    0,    0,    0,  634,  634,  634,  634,  634,  634,
  634,  634,  634,  634,  634,    0,  634,    0,    0,  634,
  634,    0,  634,  634,  634,  634,  634,  634,  634,  634,
    0,  643,  634,  634,  634,  634,  634,  393,    0,    0,
    0,    0,    0,  394,    0,   17,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
   18,   19,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,  791,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,  381,  413,  414,
  415,  111,  112,  113,  416,  114,  382,    0,  115,  116,
  644,  645,  646,    0,    0,    0,    0,  647,    0,  383,
  417,    0,  418,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,  791,    0,  420,  791,    0,    0,    0,
  277,    0,    0,  791,  641,    0,  791,    0,    0,    0,
  791,    0,    0,  386,    0,  791,  387,    0,  791,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  791,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  391,    0,  791,    0,
  392,  791,  791,    0,    0,    0,    0,    0,  642,    0,
    0,  791,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,  791,    0,
    0,    0,  420,    0,    0,    0,    0,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  791,  643,    0,    0,    0,  791,    0,  393,    0,    0,
    0,    0,    0,  394,    0,    0,    0,  791,    0,  791,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  644,  645,  646,    0,  381,    0,    0,    0,    0,    0,
    0,    0,    0,  382,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,    0,    0,    0,    0,    0,  815,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  815,    0,    0,  815,    0,    0,    0,    0,    0,    0,
    0,  389,  390,    0, 1104,    0,    0, 1105,  815,    0,
    0,  381,    0,  391,    0,    0,    0,  392,    0,    0,
  382,    0,    0,    0,    0,  864,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,    0,    0,  479,  384,
  385,  417,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
    0,  277,    0,    0,    0,    0,    0,  386,  643,    0,
  387,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,    0,    0,  389,  390,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
  391,    0,    0,    0,  392,    0,    0,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1106,
    0,  393,    0,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,  420,    0,
    0,  815,    0,  277,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
    0,    0,  115,  116,  117,  118,  119,    0, 1114,    0,
    0, 1105,    0,    0,    0,  381,    0,  815,    0,    0,
  815,    0,    0,    0,  382,    0,    0,  815,    0,    0,
  815,    0,    0,    0,  815,    0,    0,  383,    0,  815,
    0,    0,  815,  384,  385,    0,    0,    0,    0,  479,
    0,    0,  417,    0,  418,    0,    0,    0,  815,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,    0,
    0,  386,  277,    0,  387,    0,    0,    0,    0,    0,
    0,    0,  815,    0,    0,  815,  815,    0,    0,    0,
    0,    0,    0,    0,    0,  815,    0,    0,    0,    0,
    0,    0,  389,  390,    0,    0,    0,    0,    0,    0,
    0,    0,  815,    0,  391,    0,    0,    0,  392,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  815,    0,    0,    0,    0,  815,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  815,    0,  815,    0,    0,    0,    0,    0,    0,
 1118,    0,    0, 1105,    0,    0,    0,  381,    0,    0,
    0,    0,    0, 1106,    0,  393,  382,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,  383,
    0,  395,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,  386,    0,    0,  387,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,  389,  390,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  391,    0,    0, 1126,
  392,    0, 1105,    0,    0,    0,  381,    0,    0,    0,
    0,    0,    0,    0,    0,  382,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  383,    0,
    0,    0,    0,  479,  384,  385,  417,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,    0,    0, 1106,  277,  393,    0,    0,
    0,    0,  386,  394,    0,  387,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,  389,  390,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,  391,    0,    0,    0,  392,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1106,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  479,    0,    0,  417,    0,
  418,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,  420,    0,    0,    0,    0,  277,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,    0, 1129,    0,    0, 1105,    0,    0,    0,
  381,    0,    0,    0,    0,    0,    0,    0,    0,  382,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
    0,    0,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,    0,    0,  386,  277,    0,  387,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  391,
    0,    0,    0,  392,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1134,    0,    0, 1105,    0,
    0,    0,  381,    0,    0,    0,    0,    0, 1106,    0,
  393,  382,    0,    0,    0,    0,  394,    0,    0,    0,
    0,    0,    0,    0,  383,    0,  395,    0,    0,    0,
  384,  385,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,  386,    0,
    0,  387,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,  389,
  390,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,  391,    0,    0, 1137,  392,    0, 1105,    0,    0,
    0,  381,    0,    0,  794,    0,    0,    0,    0,    0,
  382,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,    0,    0,  479,  384,
  385,  417,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  794,  420,    0,  794,    0,
 1106,  277,  393,    0,    0,    0,    0,  386,  394,    0,
  387,    0,    0,  794,    0,    0,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,  389,  390,
    0,    0,    0,    0,    0,    0,  400,    0,    0,    0,
  391,    0,    0,    0,  392,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1106,
    0,  393,    0,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,  584,    0,  411,
  412,  585,  413,  414,  415,  111,  112,  113,  416,  114,
    0,    0,  115,  116,  117,  118,  119,    0, 1140,  586,
    0, 1105,  587,    0,    0,  381,    0,    0,  479,    0,
  484,  417,    0,  418,  382,    0,  794,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,  383,    0,    0,
    0,  277,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,  588,    0,    0,    0,    0,    0,  589,
  590,  591,    0,  592,  593,  594,    0,  595,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,  794,    0,    0,  794,    0,  596,    0,  597,
    0,    0,  794,    0,    0,  794,    0,    0,    0,  794,
    0,    0,  389,  390,  794,    0,    0,  794,    0,    0,
    0,    0,    0,    0,  391,    0,  479,    0,  392,  417,
  598,  418,    0,  794,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  420,    0,    0,  599,    0,  277,
    0,    0,    0,    0,    0,    0,    0,  794,    0,    0,
  794,  794,    0,    0,    0,    0,  320,    0,    0,  600,
  794,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1106,    0,  393,    0,  794,    0,    0,
    0,  394,    0,    0,    0,    0,  320,    0,    0,    0,
    0,  395,    0,  320,  601,  602,    0,  320,  320,  794,
  320,    0,    0,  320,  794,  396,  397,  398,  399,  202,
  203,  603,    0,    0,    0,    0,  794,  320,  794,  400,
    0,    0,    0,  320,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,  381,  413,  414,  415,  111,
  112,  113,  416,  114,  382,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,  320,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,  320,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  320,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  320,    0,    0,
  388,    0,  389,  390,    0,    0,  320,    0,  320,    0,
    0,    0,    0,  381,  391,  320,    0,    0,  392,    0,
    0,    0,  382,    0,    0,  955,    0,  955,  955,    0,
  955,    0,    0,    0,    0,  383,    0,    0,    0,    0,
    0,  384,  385,  955,  320,  320,    0,  320,  955,    0,
    0,    0,    0,    0,  320,  320,  320,  320,  320,    0,
  320,    0,    0,  320,  320,  320,  320,  320,    0,  386,
    0,    0,  387,    0,    0,  393,    0,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,  388,    0,
  389,  390,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,  391,    0,    0,    0,  392,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,  684,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,    0,    0,    0,  640,    0,  394,
  640,    0,  640,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,  220,    0,  640,    0,  220,    0,    0,
  640,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,  220,    0,    0,    0,  220,    0,  400,  220,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,  955,    0,  115,  116,  117,  118,  119,  220,
    0,  955,    0,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,  955,  529,    0,    0,    0,    0,
  955,  955,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  220,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  955,    0,
    0,  955,    0,    0,    0,  529,  529,    0,    0,  529,
    0,    0,    0,    0,    0,    0,  220,    0,    0,    0,
    0,   80,    0,    0,  529,    0,  529,  955,    0,  955,
  955,    0,    0,  220,    0,    0,    0,    0,    0,    0,
    0,  955,    0,    0,    0,  955,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  220,  529,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  640,    0,    0,
  220,  220,    0,    0,  640,    0,    0,    0,    0,    0,
    0,    0,  955,  640,    0,    0,  419,  220,  955,  417,
    0,  418,    0,    0,    0,    0,  640,    0,  955,    0,
    0,    0,  640,  640,  420,    0,    0,    0,    0,  277,
  640,    0,  955,  955,  955,  955,  955,  955,    0,    0,
    0,    0,    0,    0,    0,    0,  955,    0,    0,    0,
  640,    0,    0,  640,    0,  955,  955,  955,  955,  955,
  955,  955,  955,  955,  955,  955,    0,  955,    0,    0,
  955,  955,    0,  955,  955,  955,  955,  955,  955,  955,
  955,  640,  640,  955,  955,  955,  955,  955,    0,    0,
    0,    0,    0,  640,    0,    0,    0,  640,    0,    0,
    0,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  529,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  529,
    0,    0,  529,  529,    0,  529,    0,    0,    0,  529,
    0,    0,  529,    0,  640,    0,    0,    0,    0,    0,
  640,    0,    0,    0,    0,  529,    0,    0,    0,    0,
  640,  529,    0,    0,  529,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  640,  640,  640,  640,  640,  640,
    0,    0,    0,    0,    0,    0,    0,    0,  640,    0,
    0,    0,    0,    0,    0,    0,    0,  640,  640,  640,
  640,  640,  640,  640,  640,  640,  640,  640,    0,  640,
    0,    0,  640,  640,    0,  640,  640,  640,  640,  640,
  640,  640,  640,  381,    0,  640,  640,  640,  640,  640,
    0,    0,  382,  529,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  383,    0,    0,    0,    0,
    0,  384,  385,    0,    0,  529,    0,    0,    0,    0,
    0,    0,  529,    0,    0,    0,  529,    0,    0,    0,
    0,    0,    0,  529,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,    0,    0,    0,
    0,  529,    0,    0,    0,    0,  529,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  388,    0,
  389,  390,    0,    0,    0,    0,    0,    0,    0,    0,
  381,  529,  391,    0,    0,  529,  392,    0,    0,  382,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,  387,
    0,    0,    0,  393,    0,    0,    0,    0,    0,  394,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,  391,
    0,    0,    0,  392,    0,    0,    0,  400,  536,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,  536,
    0,    0,  536,    0,    0,    0,    0,    0,    0,    0,
  393,    0,    0,    0,    0,    0,  394,  536,    0,  536,
    0,    0,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,  676,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,  536,
    0,    0,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,  381,
    0,  115,  116,  117,  118,  119,    0,    0,  382,    0,
    0,  479,    0,    0,  417,    0,  418,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,  420,
    0,    0,    0,    0,  277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  389,  390,    0,    0,
    0,  479,  929,    0,  417,    0,  418,    0,  391,    0,
    0,    0,  392,    0,    0,    0,    0,    0,    0,  420,
    0,    0,    0,    0,  277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  536,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  536,    0,    0,    0,  536,    0,  536,  684,
    0,    0,  536,    0,    0,  536,    0,    0,    0,  393,
    0,    0,    0,    0,    0,  394,    0,    0,  536,    0,
    0,    0,    0,    0,  536,  395,    0,  536,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  381,    0,
  115,  116,  117,  118,  119,    0,  536,  382,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,  536,    0,
    0,    0,    0,    0,    0,  536,    0,    0,    0,  536,
    0,    0,    0,    0,    0,    0,  536,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,  387,    0,    0,
    0,    0,    0,    0,  536,    0,    0,    0,    0,  536,
    0,    0,    0,    0,    0,    0,    0,    0,  381,    0,
    0,    0,    0,  388,    0,  389,  390,  382,    0,    0,
  479,    0,    0,  417,  536,  418,    0,  391,  536,    0,
  383,  392,    0,    0,    0,    0,  384,  385,  420,    0,
    0,    0,    0,  277,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,  387,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  393,    0,
    0,    0,    0,    0,  394,  389,  390,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,  391,    0,    0,
    0,  392,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,    0,  393,  115,
  116,  117,  118,  119,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,  381,    0,  115,
  116,  117,  118,  119,    0,    0,  382,    0,    0,  479,
    0,    0,  417,    0,  418,    0,    0,    0,    0,  383,
    0,    0,    0,    0,    0,  384,  385,  420,    0,    0,
    0,    0,  277,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,  479,
    0,    0,  417,    0,  418,    0,  391,    0,    0,    0,
  392,    0,    0,    0,  105,    0,    0,  420,    0,    0,
    0,    0,  277,  162,  163,  164,  165,  166,  167,  168,
  169,  170,  171,  172,  173,  174,  175,  176,  177,    0,
    0,  178,  179,  106,  107,    0,  108,    0,    0,  180,
  181,    0,    0,  109,  110,  111,  112,  113,  182,  114,
  183,    0,  115,  116,  117,  118,  119,  393,    0,    0,
    0,    0,    0,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  854,    0,    0,
    0,  585,    0,    0, 1132,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,  855,    0,    0,    0,  586,
    0,  400,  587,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,  381,    0,  115,  116,
  117,  118,  119,  588,    0,  382,    0,    0,    0,  589,
  590,  591,    0,  592,  593,  594,    0,  595,  383,    0,
    0,    0,    0,    0,  384,  385,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  596,    0,  597,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  598,    0,    0,    0,    0,    0,  381,    0,    0,    0,
    0,    0,    0,  389,  390,  382,    0,  599,  479,    0,
    0,  417,    0,  418,    0,  391,    0,    0,  383,  392,
    0,    0,    0,    0,  384,  385,  420, 1703,    0,  600,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,  601,  602, 1317,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,  603,  394,  389,  390,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  391,    0,    0,    0,  392,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,  393,  115,  116,  117,
  118,  119,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1330,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,    0,  115,  116,  117,
  118,  119,    0,    0,  382,    0,    0,  479,    0,    0,
  417,    0,  418,    0,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,  420,    0,    0,    0,    0,
  277,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  389,  390,    0,    0,    0,  955,    0,    0,
  955,    0,  955,    0,  391,    0,    0,    0,  392,    0,
    0,    0,  220,    0,    0,  955,  220,    0,    0,    0,
  955,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,    0,    0,    0,  220,    0,    0,  220,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,  220,    0,
    0,  395,    0,    0,  220,  220,  220,    0,  220,  220,
  220,    0,  220,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,  220,    0,  220,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,  381,  220,  115,  116,  117,  118,
  119,    0,    0,  382,    0,    0,    0,    0,    0,    0,
    0,    0,  220,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  520,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
  220,    0,    0,  792,  955,    0,    0,    0,    0,    0,
    0,  389,  390,  955,    0,    0,  220,  520,    0,    0,
  520,    0,    0,  391,    0,    0,  955,  392,    0,    0,
    0,    0,  955,  955,    0,  520,    0,  520,    0,    0,
    0,    0,    0,    0,  792,    0,    0,  792,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  955,    0,  792,  955,    0,    0,    0,  520,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,  955,  955,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,  955,    0,    0,    0,  955,    0,    0,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,  955,  115,  116,  117,  118,  119,
  955,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  955,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  955,  955,  955,  955,  955,  955,
    0,    0,    0,    0,    0,    0,    0,    0,  955,    0,
    0,    0,    0,    0,    0,    0,    0,  955,  955,  955,
  955,  955,  955,  955,  955,  955,  955,  955,  831,  955,
    0,    0,  955,  955,  520,  955,  955,  955,  955,  955,
  955,  955,  955,    0,    0,  955,  955,  955,  955,  955,
  520,    0,    0,    0,  520,    0,  520,    0,    0,    0,
  520,    0,    0,  520,    0,  792,    0,    0,    0,  831,
  993,  993,  831,  993,  993,  993,  520,    0,    0,    0,
    0,    0,  520,    0,    0,  520,    0,  831,  993,  993,
  993,  133,    0,    0,    0,  133,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,  133,    0,    0,  993,
    0,  792,    0,    0,  792,    0,    0,    0,    0,    0,
    0,  792,    0,    0,  792,    0,    0,    0,  792,    0,
    0,    0,    0,  792,    0,    0,  792,    0,    0,    0,
    0,    0,    0,    0,  520,    0,    0,  133,    0,    0,
  832,    0,  792,  133,  133,  133,    0,  133,  133,  133,
    0,  133,    0,    0,    0,    0,  520,    0,    0,    0,
    0,    0,    0,  520,    0,    0,  792,  520,    0,  792,
  792,  133,    0,  133,  520,    0,    0,    0,    0,  792,
    0,  832,  994,  994,  832,  994,  994,  994,    0,    0,
    0,    0,  520,    0,    0,    0,  792,  520,    0,  832,
  994,  994,  994,    0,  133,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  792,    0,
    0,  133,  520,  792,  793,    0,  520,    0,    0,    0,
    0,  994,    0,    0,    0,  792,    0,  792,    0,    0,
    0,    0,    0,  133,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  793,    0,    0,  793,    0,
  831,    0,    0,    0,    0,    0,    0,    0,  133,  133,
  993,    0,    0,  793,    0,    0,    0,    0,  993,    0,
    0,    0,    0,  993,    0,  133,  993,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  833,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  831,    0,    0,  831,
    0,    0,  993,    0,    0,    0,  831,    0,    0,  831,
    0,  993,    0,  831,    0,    0,    0,    0,  831,  993,
    0,  831,    0,    0,    0,    0,  833,  995,  995,  833,
  995,  995,  995,    0,    0,    0,    0,  831,  993,    0,
  993,    0,    0,    0,  833,  995,  995,  995,    0,    0,
    0,    0,    0,    0,  993,    0,  993,  993,  993,    0,
    0,  831,  823,    0,  831,  831,    0,    0,  787,    0,
    0,    0,  832,    0,  831,    0,  995,    0,    0,    0,
    0,    0,  994,    0,    0,    0,    0,    0,    0,    0,
  994,  831,    0,    0,    0,  994,    0,    0,  994,    0,
    0,    0,    0,  823,    0,    0,  823,  993,    0,  787,
    0,    0,  787,  831,    0,    0,    0,    0,  831,    0,
    0,  823,    0,    0,    0,    0,    0,  787,  832,    0,
  831,  832,  831,    0,  994,    0,    0,    0,  832,    0,
    0,  832,    0,  994,    0,  832,    0,    0,    0,    0,
  832,  994,    0,  832,    0,    0,  793,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  832,
  994,    0,  994,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  994,    0,  994,  994,
  994,    0,    0,  832,    0,    0,  832,  832,    0,    0,
  788,    0,    0,    0,    0,    0,  832,    0,    0,    0,
    0,    0,  793,    0,    0,  793,    0,    0,    0,    0,
    0,    0,  793,  832,    0,  793,    0,    0,    0,  793,
    0,    0,    0,    0,  793,    0,    0,  793,    0,  994,
    0,  788,  789,    0,  788,  832,    0,  833,    0,    0,
  832,    0,    0,  793,    0,    0,    0,  995,    0,  788,
    0,    0,  832,    0,  832,  995,    0,    0,    0,    0,
  995,    0,    0,  995,    0,    0,    0,  793,    0,    0,
  793,  793,    0,  789,    0,    0,  789,    0,    0,    0,
  793,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  789,    0,  833,    0,    0,  833,  793,    0,  995,
    0,    0,    0,  833,    0,    0,  833,    0,  995,    0,
  833,    0,    0,    0,  823,  833,  995,    0,  833,  793,
  787,    0,  822,    0,  793,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  833,  995,  793,  995,  793,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  995,    0,  995,  995,  995,    0,    0,  833,    0,
    0,  833,  833,  822,  816,    0,  822,    0,    0,    0,
  823,  833,    0,  823,    0,    0,  787,    0,    0,  787,
  823,  822,    0,  823,    0,    0,  787,  823,  833,  787,
    0,    0,  823,  787,    0,  823,  824,    0,  787,    0,
    0,  787,    0,    0,  995,  816,    0,    0,  816,    0,
  833,  823,    0,    0,    0,  833,    0,  787,    0,    0,
    0,    0,    0,  816,    0,    0,    0,  833,    0,  833,
    0,    0,    0,    0,    0,  823,    0,  824,  823,  823,
  824,  787,    0,    0,  787,  787,    0,    0,  823,    0,
    0,    0,  788,    0,  787,  824,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  823,    0,    0,    0,    0,
    0,  787,    0,    0,    0,    0,    0,    0,    0,  825,
    0,    0,    0,    0,    0,    0,    0,  823,    0,    0,
    0,    0,  823,  787,  789,    0,    0,    0,  787,  813,
    0,    0,    0,    0,  823,    0,  823,    0,  788,    0,
  787,  788,  787,    0,    0,    0,    0,    0,  788,    0,
  825,  788,    0,  825,    0,  788,    0,    0,    0,    0,
  788,    0,    0,  788,    0,    0,    0,    0,  825,    0,
  813,    0,    0,  813,    0,    0,    0,    0,    0,  788,
  789,    0,    0,  789,    0,    0,    0,    0,  813,    0,
  789,    0,    0,  789,    0,    0,    0,  789,    0,    0,
    0,    0,  789,  788,    0,  789,  788,  788,    0,    0,
    0,    0,    0,    0,  822,    0,  788,    0,    0,    0,
    0,  789,    0,    0,    0,  817,    0,    0,    0,    0,
    0,    0,    0,  788,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  789,    0,    0,  789,  789,
    0,    0,    0,    0,    0,  788,  816,    0,  789,  826,
  788,    0,    0,    0,    0,    0,  817,    0,    0,  817,
  822,    0,  788,  822,  788,  789,    0,    0,    0,    0,
  822,    0,    0,  822,  817,    0,    0,  822,  824,    0,
    0,    0,  822,    0,    0,  822,    0,  789,    0,    0,
  826,    0,  789,  826,    0,    0,    0,    0,    0,    0,
    0,  822,  816,    0,  789,  816,  789,    0,  826,    0,
    0,    0,  816,  814,    0,  816,    0,    0,    0,  816,
    0,    0,    0,    0,  816,  822,    0,  816,  822,  822,
    0,    0,    0,    0,  824,    0,    0,  824,  822,    0,
    0,    0,    0,  816,  824,  818,    0,  824,    0,    0,
    0,  824,    0,    0,  814,  822,  824,  814,    0,  824,
    0,  825,    0,    0,    0,    0,    0,  816,    0,    0,
  816,  816,  814,    0,    0,  824,    0,  822,    0,    0,
  816,  813,  822,    0,    0,    0,  818,    0,    0,  818,
    0,    0,    0,    0,  822,    0,  822,  816,    0,  824,
    0,    0,  824,  824,  818,    0,    0,    0,    0,    0,
    0,    0,  824,    0,    0,    0,    0,  825,    0,  816,
  825,    0,    0,    0,  816,    0,    0,  825,    0,  824,
  825,    0,    0,    0,  825,    0,  816,  813,  816,  825,
  813,    0,  825,    0,    0,    0,    0,  813,    0,    0,
  813,  824,    0,    0,  813,    0,  824,    0,  825,  813,
    0,    0,  813,    0,    0,    0,    0,    0,  824,    0,
  824,    0,    0,    0,    0,    0,    0,  817,  813,    0,
    0,    0,  825,    0,    0,  825,  825,    0,    0,    0,
    0,    0,    0,    0,    0,  825,    0,    0,    0,    0,
    0,    0,  813,    0,    0,  813,  813,    0,    0,    0,
    0,  826,  825,    0,    0,  813,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,  817,  825,    0,  817,    0,    0,  825,
    0,    0,    0,  817,    0,    0,  817,    0,    0,    0,
  817,  825,    0,  825,  813,  817,    0,    0,  817,  813,
    0,    0,    0,    0,    0,    0,    0,  826,    0,    0,
  826,  813,    0,  813,  817,  814,    0,  826,    0,    0,
  826,    0,    0,    0,  826,    0,    0,    0,    0,  826,
    0,    0,  826,    0,    0,    0,    0,    0,  817,    0,
    0,  817,  817,    0,    0,    0,    0,  818,  826,    0,
    0,  817,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  817,    0,
    0,  814,  826,    0,  814,  826,  826,    0,    0,    0,
    0,  814,    0,    0,  814,  826,    0,    0,  814,    0,
  817,    0,    0,  814,    0,  817,  814,    0,    0,    0,
    0,    0,  826,  818,    0,    0,  818,  817,    0,  817,
    0,    0,  814,  818,    0, 1414,  818,    0,    0,    0,
  818,    0,    0,    0,  826,  818,    0,    0,  818,  826,
    0,    0,    0,    0,    0,    0,  814,    0,    0,  814,
  814,  826,    0,  826,  818,    6,    0,    0, 1414,  814,
    0,    0,    0,    0,    0,    0, 1533, 1534,    0, 1535,
    0,    0, 1536,    0,    0,    0,  814,    0,  818,    0,
    0,  818,  818,    0,    0,    0, 1537,    0,    6,    0,
    0,  818, 1538,    0,    0,    0,    0,    0,  814, 1533,
 1534,    0, 1535,  814,    0, 1536,    0,    0,  818,    0,
    0,    0,    0,    0,    0,  814,    0,  814,    0, 1537,
    0,    0,    0,    0,    0, 1538,    0,    0,    0,    0,
  818,    0,    0,    0,    0,  818,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  818,    0,  818,
  584,    0, 1539,    0,  585,    0,    0,    0,    0,    0,
    0,  782,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,  586,    0,    0,  587,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1539, 1540,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,    0,    0,   14, 1664,    0, 1541,    0,    0,
    0,    0,    0,    0,  105,    0,  588,    0,    0, 1540,
    0,    0,  589,  590,  591,    0,  592,  593,  594,   16,
  595,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1541,    0,    0, 1542,  107,    0,  108,  105,    0,    0,
  596,    0,  597,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115, 1543,  117,  118,  119,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1542,  107,    0,  108,
    0,    0,    0,  598,    0,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115, 1543,  117,  118,  119,
  599,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  600,    0,  584,    0,    0,    0,  585,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,    0,  586,    0,    0,  587,
    0,    0,    0,    0,    0,    0,    0,  601,  602,    0,
    0,    0,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,    0,  603,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
  588,    0,    0,    0,    0,    0,  589,  590,  591,    0,
  592,  593,  594,    0,  595,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  448,
    0,    0,    0,    0,  596,    0,  597,    0,    0,  448,
  448,  448,  448,  448,  448,  448,  448,  448,  448,  448,
  448,  448,  448,  448,    0,    0,  448,  448,  448,  448,
    0,  448,    0,    0,  448,  448,    0,  598,  448,  448,
  448,  448,  448,  448,  448,  448,    0,  448,  448,  448,
  448,  448,    0,    0,  599,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  600,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  105,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  601,  602,    0,    0,    0,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,    0,    0,  603,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  149,   18,  104,
    0,  254,   63,  285,  351,   56,  509,  138,  139,  359,
  355,  359,  699,  553,   85,  330,   67,   63,   57,  138,
  139,  419,   62, 1028,  492,   89,   73,  664,   94,  516,
   69,  759,  467,   53,   54,   55,   56,   57,   58,   90,
  328,  676,  376,   63,   64,   65,   97,   67,   95,   69,
   70,  262,   72,   73,  364,   68,    0,  279,  684,  746,
  271,   81,   83,   83,   84,   85,   70,   87,   88,   89,
   90,   92,   92,   93,   94,   95, 1248,   97,   91,  544,
  925,  636,  547,  129,    0,  131,  152,  713,  553,   93,
  817,  130, 1029, 1021,   88, 1021,    0,    0, 1021,  150,
 1028, 1021, 1028, 1040,    0, 1028,    0, 1021, 1028,  129,
  130,  131,  151,  750, 1028,   59,  306,  717,  963,  323,
  629,  226, 1059,  143,  753,   41,   45,  848,   44,  149,
  150,  151,  152,    0,    0,    0,  156,   41,   41,   45,
   44,   44,   40,   59,  973,   41, 1053,  187,   44,  143,
  779,   41,    0,   40,  306,   59,   59,   63,    0,  788,
   40,   44,   59,   59,   45,   59,  419,  698,    0,    0,
   45,   45,  212,   41,    0,   44,   63,   41,  523,  756,
    0,   44,   40,  530,  531,  532,  533,   45,   44,   44,
   41, 1190,   59,   59,   59,    0,   41,   44,   41,   41,
   44,  511, 1394,  553,  473,  553,   42,   44,  313,   41,
  256,   59,  841,    0,    0, 1076,   44,   59,   40,  260,
    0,  260,   61,  726, 1296,  159,  290,   59,   59, 1394,
  269,   44, 1077,   59,  579,  269,  256,   44,  286,   59,
  260,   63,  262,  263,  445, 1394,   63,  262,  459,  269,
  584,  271,    0,  283,   59,  294,  891,  892,  301,   41,
  306,  260,   44, 1394,   44,  348,    0,  355,  556, 1274,
  290,  320,   59,   59,  294, 1194,  264,    0,  268,   59,
 1693, 1694,  257,  329,    0,    0, 1454,  922,  405,  403,
  336,  455,  338,  356,  340,  338,  387,  343, 1183,  377,
 1185,   40,    0,  320,    0,    0,  420,  508,  372,  329,
  361,   59,  352,   41,  354,  467,  336,  605,  338, 1161,
  340, 1734, 1164,  343,  287,   59, 1263,  964,  368,  402,
  393,  370,  395,   44,  259,  277,   59,  300, 1071,  456,
  805,  361,  362,   59,   59,  270, 1274,    0, 1274,  440,
  370, 1274,  372,    0, 1274,  418,  956,  825,  362,  334,
 1274,   59,  376,   59,   59,  994,    0,    0,  968,  969,
  448,  386,  156, 1280, 1293,  365,  417, 1206,  461,  462,
  443,    0,  664,  317,    0, 1020, 1458,  436,    0,    0,
 1366,  307,  308, 1806,   41,    0,  340,    0,  217,  442,
  420,    0, 1574,  345,  492, 1381,   59,  458,   41,  439,
 1039,   44,   59,  461,  462,  318,  455,  262,  417,  309,
  435,  467,  473,  411,  260,   59,   59,  466,  681,   41,
   41,  470,   44,   44,    0,  455,  475,  340,  458,  459,
   59,  475, 1634,   59,  697,  727,  466,   59,   59,  304,
  470,  720,  571,  473,   59,  475,   59,  264,  478,    0,
   59,  300, 1097, 1098, 1191, 1100, 1101,  303,  750, 1634,
  490,  274,   41, 1410,  340,  318,  318,  264, 1415,  767,
 1417,  318,  770,  331, 1217, 1634,  328,  383,  528,  331,
  309,  304,  320,   59,  410, 1223,  784,  708,  340, 1384,
 1672,  723,  328, 1634, 1081,  331, 1505,  553,  328,    0,
  454,  331, 1680,  410,  394, 1452,  264, 1359,   59,  387,
  383, 1363,  386,  811,  258,  431,  961,  861,  396,  839,
  269, 1086,  552,  553,  574,  451,  387,  852,  558,  458,
  273,  386, 1269,    0,    0,  387,  440,   41,  328,  447,
  431,  331,  458, 1725,  396,  387,  342,  747, 1465,  572,
  454, 1070,  766,  343, 1393, 1286,  485,  436,   59,  436,
  584,  458,  306,  431,    0,  458,    0,  458, 1109,  410,
    0,    0,  628,  458,  458,  625,  626, 1524,  454,  454,
 1451, 1763,  440, 1530, 1229,  747,  452,  501,  440,  502,
  458, 1201,   59,   59,  409,  501,  454,  454,  628,  320,
  454,  264,  454,  732,  440,   41, 1251,  637,   44,  405,
  440,   41,  408,  637,   44, 1470,    0,  637,  454,  411,
  264,  264,  452,   59,  454,   59,  458,    0,  424,   59,
   59,  458, 1579,  762,  763,  264,  342,  667,  264,  397,
  348,  367,  367,  348,  700,  258, 1285, 1499,  386, 1596,
  440, 1008,  642,  466,  450,  264,  278,   41,  280,  720,
   44,  691,  452,  667,  454,  287, 1246, 1405,   41,  699,
  700,  377,  964,  703,  418,   59, 1406,  320,  708,  338,
  273, 1317,   41,  262,  478, 1700,   59,  717,  264, 1432,
  720,  747,  741,  306,  743,  725,  752,  318,  728,  405,
  756,   41,  408,  318,   91, 1691,    0, 1450,  758,  759,
   41,  741,  262,  743,    0, 1072,  746,  506,  424,  340,
  776,  550,  752, 1453,  945,  340,  756,  948, 1026, 1021,
  387,  301, 1679,   41,  397, 1382, 1028,   40,  717,  396,
 1038, 1688,  765,    0,  450,  356,  776,  340,  804,  805,
 1395, 1568, 1249,  397,  301,   41,  378,   41,  257, 1084,
 1058,  383, 1700, 1171, 1700,   59,   16, 1700,  397,    0,
 1700,  397,  387,   59,  804,  805, 1700,   44,  400,  262,
  803,  396, 1729,  442,   41,  407,  810,   44,  397,  340,
  619,  840,  259,   44, 1272, 1345, 1204,  264,  822,  961,
 1278,  850,   59,  270,    0,  418,  265,  386,   59,  452,
  840,   40,  835,  836,  837, 1632,    0,  460,  429,  441,
  850,  397, 1147,    0,  328,  440,   93,  331,   59,   58,
    0,   41,  862,   83,   44,  334,  386,  861,   40, 1419,
 1241,  342,  278,  302,  280,   41,  489,  490,   44,  492,
    0,  287,  318,  791,   59,    0,  499,  500,  501,  502,
  503,  352,  505,   59,   40,  508,  509,  510,  511,  512,
    0,   41,  442,  949,  340,   59,  377,  409,  868,  501,
  258,    0,   59,  387,  318,  946, 1287,  378,  318,   59,
  277,   41,  396,  950,  328,  442,   41,  331,  313,   44,
  261,  331, 1194,  386,  405,  961,  340,  408, 1171,   59,
  340,  322,  262,  342,   59,  945,  946,  947,  948,  949,
  950,  387,  262,  424,    0,    0,  956, 1056,  306,   59,
  397,  262,    0,  947,  318, 1290, 1633,  321,  968,  969,
   59, 1204,  378,  973,  328,  318,   12,  331,  377,  450,
    0,  335,  435,  387,  262,  343,  340,  387,  331,  343,
  307,  308,  396, 1261,  400,   41,  396,  340,  642,  258,
  264,  407,  331,  647,  440,  359,  405,  956,  262,  408,
  368,  284, 1274,   59,   59, 1345, 1631, 1345,    0,  968,
 1503,   59,  263, 1506, 1292,  424, 1397,   42,  458,  383,
    0, 1293,  417,  387,    0,  441,  440,   12, 1653,   59,
  440, 1656,  396,    0,  387,  458,   44,  306, 1074, 1049,
 1628,  450, 1630,  396,  454, 1081, 1050,  322,  387,  413,
 1050,   59,  318, 1063,  307,  308,  386,  396,  426,  401,
  418,  312, 1065, 1205, 1074, 1402,  386,   59, 1189, 1190,
 1191, 1081,  323,    0,  340,  386,  440, 1532,  436,   59,
 1083,  318,   46,   59,  274,  501, 1089,  440,  452,  405,
  136,  328,   59,    0,  331,  321,  460,   61,  386, 1687,
  264,  454,  278,  340,  280, 1598,  343,  264,   40,  335,
 1382,  287,  470,    0, 1032, 1033, 1225,   44, 1227,  309,
  383,  387,  386,  397,  451,  489,  490,  319,  492,   42,
  396,  342,   59,  359,   47,  499,  500,  501,  502,  503,
  456,  505,  278, 1421,  508,  509,  510,  511,  512,  418,
  387,  136,   59,  278,   41,  280, 1552, 1160, 1684,  396,
  383,  397,  287, 1081,   42,   43,  377,   45,  278,   47,
  280,  377,   59, 1183,  440, 1185,  427,  460,  328, 1288,
  493,  331, 1460,  319,   44, 1189, 1190,  413,  454, 1777,
  503, 1201,  402, 1223,  405, 1205, 1206,  408,  451, 1824,
  436,  331,  378,  440, 1730, 1586,  489,  490,  400,  492,
  864, 1592,  418,  424,  868,  452,  499,  500,  501,  502,
  503,  411,  505,  254,  400,  508,  509,  510,  511,  512,
  278,  407,  280,  397,  470,  445,    0,  387,    0,  450,
  397,  340,  405, 1736,  385,  276,  396, 1773,  278,  441,
  280,  460, 1394,  378,  285,   42,   43,  387,   45,  465,
   47,  424,  318, 1267,  400,  441,  396, 1267,  378, 1737,
 1738,  296, 1414, 1303, 1682,  400,  466, 1307,  460, 1805,
  489,  490,  407,  492,  340,  316,   42,  300,  260,  383,
  499,  500,  501,  502,  503,   59,  505,   59,  508,  508,
  509,  510,  511,  512,  460,  441,  383,  489,  490,    0,
  492,  279,    0, 1778, 1779, 1593,  441,  499,  500,  501,
  502,  503,  325,  505, 1732,  501,  508,  509,  510,  511,
  512,  303,   44,  489,  490,  436,  492,   41,  318,  352,
   44,    0,  265,  499,  500,  501,  502,  503,  283,  505,
   41,  318,  508,  509,  510,  511,  512, 1825,    0,   44,
  340,  329, 1413,  260,  331, 1761, 1774,  280,   59, 1834,
   44,   59,  285,  340, 1384, 1405,  501, 1413, 1414,  302,
    0,   93,  413, 1393, 1505,  260,  417,  418,  419,   93,
   41,   42,   43, 1422,   45,  363,   47, 1793, 1794,    0,
   59,  328,  280, 1413,  331,  417,  303,  285,   93,    0,
 1552,  494, 1422,  381,  337,  402,  343,   59, 1527,   93,
  387,   41, 1818,  321,   44,  337,  301,  430, 1700,  396,
    0,  318,  444, 1829,  437,  460, 1832,  335,  313,   59,
   41,  328,  320,   44,  331,  343,  863,    0,  479,  384,
  867,  438,  483,  340,   42,   43,   44,   45,   59,   47,
   61,  359, 1705,  338,  489,  490,  418,  492,   59,    0,
    0,    0, 1533,  440,  499,  500,  501,  502,  503,  259,
  505,  337, 1577,  508,  509,  510,  511,  512,    0,   59,
  270, 1495, 1634,  280,   42,   43,  274,   45,  285,   47,
  387, 1505,   46,   45,  439,  279,   59,  280,  318,  396,
    0,   59,  285,  440,   44,  413, 1552,   61,  374, 1661,
    0,  456,  342, 1533, 1578,  452,  300,  454,   59,   59,
   59,  387,  407,  320,  294,   42,   43,    0,   45,    0,
   47,    0,  417,    0, 1573,  305, 1536,   59,  458,    0,
   40,   41,  312,  440,   44,  329,   41,  377, 1568,   44,
   58,   41,    0, 1573,   44,  452, 1708,  442, 1578,   59,
  258,   61, 1602,    0,  348,  485, 1606,  337, 1673,   59,
  465,  260,  467,  357,  260,  405, 1597, 1597,  408,  363,
   41,  279,  424,   44,    0,    0,   59,    0,   59,    0,
   59,   91,   59,   40,  424, 1646, 1748,  381,   59,   41,
   42,   43,  300,   45, 1756,   47,  647,  378,  306, 1761,
  417,   59, 1632, 1633,  303,    0,    0,  303, 1394,  280,
  450,   59,   59,  664,  285,    0, 1646,  668,  461,  400,
   41,  329,  402,   44,  675, 1787,  407,  444, 1790,  680,
  681, 1793, 1794,   59,   59, 1716,   59,  258,   59,  690,
  348,  692,  693,  694,  695,  696,  697,  698,  356,  357,
 1812, 1682, 1682,  260,  434,  363, 1818,  278,  438,  280,
  441,    0,  348,  284,   59,   59,  287, 1829, 1699, 1699,
 1832,  357,  280,  381,   59,  236,  727,  285,  318,  300,
    0,    0,  301,  244,  460,  306, 1716, 1717,  328,  280,
    0,  331,  321,  321, 1718,  264,  303,  288, 1718,  750,
  340, 1732, 1732,  343,  755, 1761,  335,  335, 1739, 1739,
  418, 1785,  280,  489,  490,  343,  492,  285,  316,  338,
   59,  429,  301,  499,  500,  501,  502,  503,  337,  505,
  359,  359,  508,  509,  510,  511,  512, 1793, 1794,   59,
   59,  318, 1708, 1774, 1774,  316,  317,  387,  258,   59,
    0, 1782, 1782,  280,  383, 1785,  396,  378,  285,  338,
   42,   43, 1818,   45,  274,   47,   41,  277,  278,   44,
  280,  460,  340, 1829,  284,  402, 1832,  287,  321,  400,
 1810, 1183, 1748, 1185,  413,  413,  407, 1817,  407,  336,
  300,   41,  335,  452,   44,  340,  306,  418,  417,  309,
  440,   41,  402,    0,   44, 1835,   40,  434,  320,   59,
  318,   61,  452,  340,   41,  436,  359,   44,   93,  402,
  441, 1787,  449,  442, 1790,  876,  877,  878,  280,  383,
  881,  882,  883,  285,  885,  886,  887,  888,  889,  890,
  383,   91,  893,  894,  383,  896, 1812,  898, 1634,  470,
  901,  902,  903,  904,  905,  906,  506,  278,  258,  280,
  436,  261,   59,  284,  461,  462,  287,  918,  378,  920,
  413,  457,  458,  459,    0, 1661,  493,  494,  450,  300,
   42,   43,  434,   45,   41,   47,  361,   44,   42,   43,
  400,   45,  384,   47,  417,  337,  458,  407,  460,  485,
  486,  411,   41,  357,   41,   44,  306,   44,  418,   41,
    0,  442,   44,  964,   40,   41,   42,   43,   44,   45,
   46,   47, 1708,  391,  392,  287,  436,  489,  490,  411,
  492,  441,  374,   59,   60,   61,   62,  499,  500,  501,
  502,  503,  460,  505,  469,  387,  508,  509,  510,  511,
  512,   41,   41,   44,   44,   44,  466,  378,   42,   43,
  470,   45, 1748,   47,   61,   91,  372,   93,  286,   59,
 1756,  489,  490,  264,  492,   42,   43,    0,   45,  400,
   47,  499,  500,  501,  502,  503,  407,  505,  461,  462,
  508,  509,  510,  511,  512,  461,   41,   41,  280,   44,
   44, 1787,   41,  285, 1790,   44,   41,   41,  258,   44,
   44,   41,  460,   40,   44,  272,   41,   45,   41,   44,
  441,   44,   41,   40,  274,   44, 1812,  277,  278,  311,
  280,  288,   40,   40,  284,   40,   59,  287,   61,   40,
   40,  489,  490,   40,  492,   40,   40,   40,  461,  461,
  300,  499,  500,  501,  502,  503,  306,  505, 1109,  309,
  508,  509,  510,  511,  512,   40,   40,   40,   40,   91,
 1121, 1122,  436, 1124, 1125,   44,  333,  320,  663,   46,
  665, 1132,  279,    0,  669,  670,  671,   44,   41,   42,
   43,  402,   45,   44,   47,  320,   44,  428,   61,   46,
 1151,   40,   44,  300,  264,  362,  382,  348, 1159,   44,
 1161,  265,  278, 1164,    0, 1166,  300,  278,  280, 1170,
 1171, 1172, 1173,  285,   41,  397,  280,   44,  378,  264,
  436,  285,  329,  470,  397,  458,  262,  277,  264,  265,
  447,  399,   59, 1194,  447,    0,  272,   40,  302,  311,
  400,  348,   40, 1204,  280,   41,  349,  407,   44,  285,
  357,  411,  288,   41,   42,   43,  363,   45,  418,   47,
   91,   40,  429,   59,   40,  318,  302,  264,  262,  383,
  411,  307,  308,   44,  381,  311,  436,  389,  278,   40,
  280,  441,  318, 1244,  320,  321,  280,  287,  324,  382,
   40,  285,  328,    0,   59,  331,  451,  333,   41,  335,
  300,  464,  436,  280,  340,  341,  466,  343,  285,  317,
  470,  881,   40,  883,  320,  885,  886,  320,  888,  889,
  890,  452,  448,  359,  360,  258,  362,  502,  264,  454,
  458,   40, 1293,  274,   41,  277,   40,   44,  300,  344,
  376,  274,  378,  379,  380,  284,  344,  383,  278,  386,
  386,  387,   59, 1314,  386,  386,  418,  295,   42,   43,
  396,   45, 1323,   47,  416,   40,  300,  300,  356, 1330,
  418,  417,   40,  306,  410,   59,  309,  413,  378,    0,
   41,   42,   43,  274,   45,  323,   47,  417,    0,  264,
   41,   42,   43,  429,   45,   40,   47,   41, 1359,  435,
  400,   44, 1363,   44,  440,   44,  320,  407,   42,   43,
  280,   45,  385,   47,  352,  451,  452,  280,  454,  367,
   41, 1382,  285,   44,  460,   44,  320,   40,   44,   41,
   42,   43,   44,   45,   46,   47,  328,   44,   59,  505,
   61,  441,  266,   40,  299,  501,   40,   59,   60,   61,
   62,  264,  422,  489,  490,   41,  492,   58,   41,   41,
   41,  454,  264,  499,  500,  501,  502,  503,  440,  505,
  394,    0,  508,  509,  510,  511,  512,  309,  411,   91,
  492,   93,  278,  309,  280,  418,   41,   41,   40,  427,
   40,  318,  280,  352,  321,   40,   40,  285,  262,   40,
   40,  328,   40,  436,  331,  443,   40, 1661,  335,   40,
   40,  308,   41,  340,  279,   44,  343, 1478, 1479,  457,
  458,  459,   40,  461,  462,   40,   40,   40,   40,   40,
   59,   41,  359,  466,  435,  300,  327,  470, 1499,   44,
  388,  388,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  388,  274,  343, 1708,  390,  383,  495,  496,  386,
  387,   41,   42,   43,  329,   45,  504,   47,  331,  396,
  391,  278,  422,  280,  446,  271,  287,  284, 1539,  407,
  287,  386,  378,  348,   41,  386,  413,  386,   41,  278,
   40,  386,  357,  300, 1748,  378,  280,  386,  363,  386,
   41,  285, 1756,    0,  400,   41,  417,   41,  435,   41,
  308,  407,  435,  440,  436,  385,  381,  280,  396,  280,
  337,  301,  442,  338,  285,  452,  442,  454,  262,  280,
  338,  301,   41, 1787,  285,   41, 1790,  258,  352,  320,
 1601,  385,    0,   41,   41,  441,  280,   44, 1609,   41,
  262,  285,  264,  265,   41,  371,  277,  278, 1812,  280,
  272,    0,   59,  284,   41,  371,  287,   41,  280,   41,
   41,  378,   41,  285,   41,   41,  288,   41,   41,  300,
   41,   41,   41,   41, 1645,  306,   44,   41, 1649,  280,
  302,   46,  304,  400,    0,  307,  308,   44,   40,  311,
  407,   59,   41,   59,  340,   44,  318,   59,  320,  321,
   40,   59,  324,   59,   59,   59,  328,    0,  340,  331,
   59,  333,   61,  335,   59,   59,   41,  352,  340,  341,
  308,  343,   41,  262,  441,   41,   42,   43,   44,   45,
   46,   47,  344,   40, 1705,  368, 1707,  359,  360,  451,
  362,  340,  383,   59,   60,   61,   62,  378,   41,   44,
   59,   44,   41,   59,  376,  383,  378,  379,  380,  383,
  343,  383,    0,   41,  386,  387,   59,    0,    0,  400,
   41,   44,  410,  435,  396,   91,  407,   93,  304,  318,
   40,  387,  321,  301,  442,  416,  293,  418,  410,  328,
  280,  413,  331,   59,  304,  285,  335,  463,   44,  307,
   44,  340,   41,   41,  343,  436,   44,  429,   41,   41,
  441,   44,   44,  435,    0,  383,  383,   40,  440,  264,
  359,   59,  278,  337,  264,  320,   59,   59,  458,  451,
  452,  389,  454,  264,  470,  411, 1315, 1316,  460,  470,
   44,  451, 1321, 1322,  383, 1324, 1325,  386,  387,   40,
  309,  271,  264, 1332, 1333, 1334, 1335,  396, 1337, 1338,
 1339, 1340, 1341, 1342,  458,  435,  343,  489,  490,  343,
  492,  278,  343,  280,  413,  436,  458,  499,  500,  501,
  502,  503,  343,  505,  308,  271,  508,  509,  510,  511,
  512,   41,   59,   41,   42,   43,  435,   45,   40,   47,
   40,  440,   41,   42,   43,  308,   45,   59,   47,  258,
  278,   41,  280,  452,  308,  454,   59,   41,   59,  287,
  472,  473,  474,  475,  476,  477,  478,  479,  277,  278,
   41,  280,  300,    0,    0,  284,    0,    0,  287, 1186,
   12,  304,  271,  264,  708,  263,  262,    0,  264,  265,
  632,  300, 1086,  853, 1084,   87,  272,  306, 1081, 1073,
 1440,  356,  321,    0,  280, 1441,  326, 1063, 1011,  285,
  579,  378,  288,  311,  752, 1074, 1643,  338,   41,   42,
   43,   44,   45,   46,   47,  278,  302,  280,  338, 1723,
  786,  307,  308,  400,  287,  311,   59,   60,   61,   62,
  407, 1419,  318, 1727,  320,  321,   59,   44,  324,  558,
  378, 1421,  328, 1765, 1726,  331, 1705,  333,  776,  335,
 1242, 1651,   59, 1817,  340,  341, 1666,  343,   91,  378,
   93, 1762,  400,  996,  441, 1219, 1682, 1398,  343,  407,
  278,  331,  280,  359,  360,  324,  362,  560,  832,  287,
   13,  400, 1276,  298,  824,  618,  494,  498,  407,  755,
  376,  141,  378,  379,  380,  741,  973,  383,  727,  418,
  386,  387,  728,  441, 1522,   41,   42,   43, 1634,   45,
  396,   47, 1739, 1782, 1835,  378,  318,  436,  321,  321,
  965,  509,  441, 1123,  410,  328,  328,  413,  331,  331,
  716, 1166,  335,  335, 1231,   -1,   -1,  400,  340,   -1,
  343,  343,   -1,  429,  407,   -1,   41,   42,   43,  435,
   45,  470,   47,   -1,  440,   -1,  359,  359,   -1,   -1,
   -1,   -1,  280,   -1,   -1,  451,  452,  285,  454,   -1,
  378,  280,   -1,   -1,  460,   -1,  285,   -1,  441,   -1,
  383,  383,   -1,   -1,   -1,  387,   -1,   41,   42,   43,
   -1,   45,  400,   47,  396,    0,   41,   42,   43,  407,
   45,   -1,   47,  489,  490,   -1,  492,   -1,   -1,   -1,
  413,  413,   -1,  499,  500,  501,  502,  503,   -1,  505,
    0,   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,
   -1,  264,  265,  441,   -1,   -1,   -1,  440,  440,  272,
   -1,   -1,   41,   42,   43,   -1,   45,  280,   47,  452,
  452,  454,  285,   -1,   59,  288,  279,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,  302,
   -1,   -1,   -1,   -1,  307,  308,   -1,  300,  311,   59,
   60,   61,   62,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,  329,  340,  341,   -1,
  343,   91,   -1,   93,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,  348,  359,  360,  335,  362,
    0,   -1,   -1,  356,  357,   -1,  343,   -1,   -1,   -1,
  363,   -1,   -1,  376,  280,  378,  379,  380,   -1,  285,
  383,   -1,  359,  386,  387,   -1,   -1,   -1,  381,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
  393,   41,  395,   -1,   44,   -1,  383,  410,   -1,   -1,
  413,   -1,   41,   42,   43,  280,   45,   -1,   47,   59,
  285,   -1,   -1,   -1,   -1,  418,  429,   41,   42,   43,
   -1,   45,  435,   47,   -1,   -1,  413,  440,   41,   42,
   43,  418,   45,   -1,   47,   -1,   -1,   -1,  451,  452,
  443,  454,   -1,   -1,   -1,   -1,  280,  460,   -1,   -1,
   -1,  285,   -1,  440,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  258,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,    0,  279,  508,  509,  510,  511,  512,
  294,  280,  262,   -1,  264,  265,  285,   -1,   -1,   -1,
   -1,  305,  272,   -1,   -1,  300,   -1,   -1,  312,   -1,
  280,  306,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  325,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,  302,   -1,  329,   -1,   -1,  307,  308,   -1,
   -1,  311,   59,   60,   61,   62,   -1,   -1,  318,   -1,
  320,  321,    0,  348,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,  357,  333,   -1,  335,   -1,   -1,  363,   -1,
  340,  341,   -1,  343,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,  386,   -1,   -1,   -1,  381,   -1,    0,  359,
  360,   -1,  362,   41,   -1,   -1,   44,   -1,  402,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   59,   -1,  383,   -1,  419,  386,  387,   -1,   -1,
   -1,   -1,   -1,  418,   -1,   -1,  396,   -1,   -1,   -1,
  434,  280,   -1,   -1,  438,   -1,  285,  441,   -1,   -1,
  410,   -1,   -1,  413,   -1,  449,  280,   59,   -1,   -1,
   -1,  285,   -1,   -1,   -1,   -1,   -1,  280,  318,  429,
   -1,  321,  285,   -1,  468,  435,   -1,   -1,  328,   -1,
  440,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,  451,  452,  343,  454,   -1,  305,   -1,   -1,   -1,
  460,   -1,   -1,  312,   -1,   -1,   -1,  316,  317,  359,
   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,  337,  489,
  490,   -1,  492,  383,   -1,   -1,   -1,  387,   -1,  499,
  500,  501,  502,  503,   -1,  505,  396,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,  413,   -1,  272,   41,   42,   43,   44,
   45,   -1,   47,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  440,   -1,   -1,  402,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,  452,   -1,  311,   -1,   -1,   -1,   -1,   -1,
  419,  318,   -1,  320,  321,   -1,   -1,  324,   93,   -1,
  258,  328,   -1,    0,  331,  434,  333,   -1,  335,  438,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
  449,  279,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,  258,   -1,   -1,  468,
   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,  376,
   -1,  378,  379,  380,   -1,   -1,  383,  279,   -1,  386,
  387,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,  329,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,
   -1,   -1,   -1,  410,  306,   -1,  413,   -1,   -1,   -1,
  348,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  357,
   -1,   -1,  429,   -1,   -1,  363,   -1,  329,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  381,  451,  452,  348,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,  357,   41,   42,   43,   -1,
   45,  363,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   60,   61,   62,    0,  381,
  418,   -1,  489,  490,   -1,  492,   -1,  262,   -1,  264,
  265,   -1,  499,  500,  501,  502,  503,  272,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,  418,   -1,   -1,   41,
   42,   43,   44,   45,   -1,   47,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   -1,  311,   59,   60,   61,
   62,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  258,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,  279,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,
   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  329,  260,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
  357,   -1,   -1,   -1,   -1,  460,  363,  272,   -1,   -1,
   -1,   -1,   -1,   -1,  301,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,  381,   -1,  313,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
  327,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,  338,   -1,  508,  509,  510,  511,  512,   -1,  324,
  262,  418,  264,  265,   -1,   -1,   -1,   -1,  333,   -1,
  272,   41,   42,   43,   44,   45,  341,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  360,   -1,  362,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,  376,   -1,  378,  379,  380,  318,   -1,  320,  321,
  407,   -1,  324,   93,   -1,   -1,  328,   -1,   -1,  331,
  417,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,  264,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  410,   -1,
   -1,  413,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,
   60,   61,   62,    0,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   -1,
   -1,  311,   59,   60,   61,   62,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   93,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,  489,  490,  383,  492,   -1,  386,  387,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  396,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  451,  452,   -1,  454,  499,  500,  501,  502,  503,
  460,  505,  272,   -1,  508,  509,  510,  511,  512,  301,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  313,   -1,   -1,   -1,    0,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,  338,   -1,  508,  509,
  510,  511,  512,   -1,  324,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,  333,   -1,  272,   41,   -1,   43,   44,
   45,  341,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  288,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  360,   -1,  362,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,  376,   -1,  378,  379,
  380,  318,   -1,  320,  321,  407,   -1,  324,   93,  354,
   -1,  328,   -1,   -1,  331,  417,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  442,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,  410,  287,   -1,  413,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  429,  458,   -1,  460,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,  498,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,    0,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,  262,   -1,  264,
  265,   -1,  499,  500,  501,  502,  503,  272,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  328,
   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   43,   44,   45,   -1,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   -1,  311,   59,   60,   61,
   62,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   93,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,  489,  490,  383,  492,
   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,  502,
  503,  396,  505,   -1,  443,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,  451,  452,   -1,  454,
  499,  500,  501,  502,  503,  460,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,  343,  320,  321,
   -1,   -1,  324,   93,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
  426,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   59,   -1,   61,  435,  460,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   58,   -1,   91,  489,  490,   63,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  489,  490,   -1,
  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   40,
   -1,  311,   43,   -1,   45,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,  434,   58,  328,   -1,
   -1,  331,   63,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,  489,  490,  383,  492,   -1,  386,  387,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  396,  505,   -1,  258,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,  274,  443,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,  429,
   -1,   -1,   -1,  460,   -1,  435,   -1,   -1,   -1,  267,
  440,  300,   -1,   -1,   -1,   -1,   -1,  306,  276,   -1,
  309,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,  289,  489,  490,   -1,  492,   -1,  295,  296,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,  314,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,  323,   -1,   -1,  326,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,  366,   -1,
   -1,  400,  370,    0,   -1,  276,   -1,   -1,  407,   -1,
  378,   -1,  411,   -1,   -1,   -1,   -1,   -1,  289,  418,
   -1,  292,   -1,   -1,  295,  296,   -1,   40,   -1,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,  436,   -1,   -1,
   -1,   -1,  441,  314,   41,   58,   -1,   44,   -1,  417,
   63,   -1,  323,  421,   -1,  326,   -1,   -1,   -1,  427,
   -1,   -1,   59,   -1,   -1,  433,   -1,  466,   -1,   -1,
   -1,  470,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  378,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,  460,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,  513,  514,  515,   -1,   -1,
  421,   -1,   -1,  460,  489,  490,  427,  492,   -1,   -1,
   -1,   -1,  433,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  443,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,  457,  458,  459,  460,
  461,  462,  499,  500,  501,  502,  503,   -1,  505,   -1,
  471,  508,  509,  510,  511,  512,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
   -1,  328,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   40,   -1,   42,   43,   -1,   45,
  303,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,  402,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  352,
   -1,  354,  355,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,  460,   -1,  370,   -1,   -1,
   -1,  438,  359,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   40,   -1,  449,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,  489,  490,  383,  492,   58,  386,
  387,  468,   -1,   63,  499,  500,  501,  502,  503,  396,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,  489,  490,  427,  492,  413,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,  261,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,    0,   -1,  303,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,   -1,  323,   -1,   -1,
  326,  297,   -1,  299,   -1,  301,   -1,   -1,   -1,   -1,
  306,   -1,   -1,   -1,   -1,   -1,   41,  313,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,  352,  267,  354,  355,
   40,  327,   -1,   43,   59,   45,  276,   -1,   -1,   -1,
  366,   -1,  338,   -1,  370,   -1,   -1,   -1,   58,  289,
   -1,   -1,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,  405,
   -1,   -1,   -1,   -1,  354,  355,  412,  443,   -1,  415,
   -1,  417,  418,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  457,  458,  459,  460,  461,  462,   -1,  378,   -1,
   -1,   -1,   -1,   -1,   -1,  471,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  421,  508,  509,  510,  511,  512,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,  491,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
  506,  507,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,
  500,  501,  502,  503,  504,  505,  276,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   40,   -1,  289,
   43,   -1,   45,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   58,  321,   -1,   -1,   -1,
   63,   -1,   -1,  328,  314,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,  323,   -1,  340,  326,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,  383,   -1,
  370,  386,  387,   -1,   -1,   -1,   -1,   -1,  378,   -1,
   -1,  396,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,  421,   -1,   -1,   -1,  440,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  260,   -1,   -1,  263,   59,   -1,
   -1,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,  295,
  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,  323,  421,   -1,
  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   58,   -1,
   -1,  262,   -1,   63,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,  276,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,  289,   -1,  340,
   -1,   -1,  343,  295,  296,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  359,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  323,   63,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,  443,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  354,  355,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,  260,
  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,  425,   63,  427,   -1,   -1,
   -1,   -1,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  323,   63,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  354,
  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,  260,  370,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,  295,
  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   58,   -1,   44,   -1,
  425,   63,  427,   -1,   -1,   -1,   -1,  323,  433,   -1,
  326,   -1,   -1,   59,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,  273,   -1,  495,
  496,  277,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,  295,
   -1,  263,  298,   -1,   -1,  267,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,  276,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,   -1,
   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,  373,   -1,  375,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  354,  355,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   40,   -1,  370,   43,
  406,   45,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,  423,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,  271,   -1,   -1,  445,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,  413,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,  301,   -1,   -1,   -1,
   -1,  443,   -1,  308,  480,  481,   -1,  312,  313,  435,
  315,   -1,   -1,  318,  440,  457,  458,  459,  460,  461,
  462,  497,   -1,   -1,   -1,   -1,  452,  332,  454,  471,
   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,  417,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,
  352,   -1,  354,  355,   -1,   -1,  451,   -1,  453,   -1,
   -1,   -1,   -1,  267,  366,  460,   -1,   -1,  370,   -1,
   -1,   -1,  276,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   58,  489,  490,   -1,  492,   63,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,  323,
   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,  433,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,  273,   -1,   58,   -1,  277,   -1,   -1,
   63,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,  291,   -1,   -1,   -1,  295,   -1,  471,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,  339,
   -1,  276,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,  289,    0,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   40,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,  411,   -1,   -1,   59,   -1,   61,  352,   -1,  354,
  355,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   91,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
  480,  481,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,  276,   -1,   -1,   40,  497,  433,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
  303,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,  258,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,
   -1,   -1,  277,  278,   -1,  280,   -1,   -1,   -1,  284,
   -1,   -1,  287,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,
  443,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,  400,   -1,   -1,   -1,   -1,
   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,
   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,  466,  366,   -1,   -1,  470,  370,   -1,   -1,  276,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   91,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   40,   41,   -1,   43,   -1,   45,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,  417,
   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,  300,   -1,
   -1,   -1,   -1,   -1,  306,  443,   -1,  309,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,  378,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,  400,   -1,
   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,  411,
   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,   -1,
   40,   -1,   -1,   43,  466,   45,   -1,  366,  470,   -1,
  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
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
  509,  510,  511,  512,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  276,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,  460,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  469,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,  483,  484,   -1,
   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,   -1,  499,  500,  501,  502,  503,  504,  505,
  506,   -1,  508,  509,  510,  511,  512,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,  291,   -1,   -1,   -1,  295,
   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,  339,   -1,  276,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,  276,   -1,  423,   40,   -1,
   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,  370,
   -1,   -1,   -1,   -1,  295,  296,   58,   59,   -1,  445,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  497,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  427,  508,  509,  510,
  511,  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,  273,   -1,   -1,   58,  277,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  291,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,  443,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,  406,  508,  509,  510,  511,
  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,    0,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,  276,   -1,   -1,  497,   41,   -1,   -1,
   44,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,
   -1,   -1,  295,  296,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   59,  326,   -1,   -1,   -1,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  427,  508,  509,  510,  511,  512,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,    0,  492,
   -1,   -1,  495,  496,  258,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,
  284,   -1,   -1,  287,   -1,  262,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   46,   47,  300,   -1,   -1,   -1,
   -1,   -1,  306,   -1,   -1,  309,   -1,   59,   60,   61,
   62,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   91,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  378,   -1,   -1,  339,   -1,   -1,
    0,   -1,  359,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,
   -1,   -1,   -1,  407,   -1,   -1,  383,  411,   -1,  386,
  387,  373,   -1,  375,  418,   -1,   -1,   -1,   -1,  396,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   -1,  413,  441,   -1,   59,
   60,   61,   62,   -1,  406,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,  423,  466,  440,    0,   -1,  470,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  272,   -1,   -1,   59,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,  497,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,    0,   -1,  386,  387,   -1,   -1,    0,   -1,
   -1,   -1,  262,   -1,  396,   -1,   91,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  280,  413,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,  429,   -1,   41,
   -1,   -1,   44,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   59,  318,   -1,
  452,  321,  454,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,  413,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,  429,
   -1,   41,    0,   -1,   44,  435,   -1,  262,   -1,   -1,
  440,   -1,   -1,  359,   -1,   -1,   -1,  272,   -1,   59,
   -1,   -1,  452,   -1,  454,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,  318,   -1,   -1,  321,  413,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,  262,  340,  341,   -1,  343,  435,
  262,   -1,    0,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,  452,  362,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   41,    0,   -1,   44,   -1,   -1,   -1,
  318,  396,   -1,  321,   -1,   -1,  318,   -1,   -1,  321,
  328,   59,   -1,  331,   -1,   -1,  328,  335,  413,  331,
   -1,   -1,  340,  335,   -1,  343,    0,   -1,  340,   -1,
   -1,  343,   -1,   -1,  429,   41,   -1,   -1,   44,   -1,
  435,  359,   -1,   -1,   -1,  440,   -1,  359,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   41,  386,  387,
   44,  383,   -1,   -1,  386,  387,   -1,   -1,  396,   -1,
   -1,   -1,  262,   -1,  396,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,  435,  262,   -1,   -1,   -1,  440,    0,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  318,   -1,
  452,  321,  454,   -1,   -1,   -1,   -1,   -1,  328,   -1,
   41,  331,   -1,   44,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   59,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,  359,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   59,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,  383,   -1,  343,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  396,   -1,   -1,   -1,
   -1,  359,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,  435,  262,   -1,  396,    0,
  440,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
  318,   -1,  452,  321,  454,  413,   -1,   -1,   -1,   -1,
  328,   -1,   -1,  331,   59,   -1,   -1,  335,  262,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,  435,   -1,   -1,
   41,   -1,  440,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  318,   -1,  452,  321,  454,   -1,   59,   -1,
   -1,   -1,  328,    0,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,  383,   -1,  343,  386,  387,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  396,   -1,
   -1,   -1,   -1,  359,  328,    0,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   41,  413,  340,   44,   -1,  343,
   -1,  262,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   59,   -1,   -1,  359,   -1,  435,   -1,   -1,
  396,  262,  440,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,  452,   -1,  454,  413,   -1,  383,
   -1,   -1,  386,  387,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,  318,   -1,  435,
  321,   -1,   -1,   -1,  440,   -1,   -1,  328,   -1,  413,
  331,   -1,   -1,   -1,  335,   -1,  452,  318,  454,  340,
  321,   -1,  343,   -1,   -1,   -1,   -1,  328,   -1,   -1,
  331,  435,   -1,   -1,  335,   -1,  440,   -1,  359,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,   -1,  262,  359,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,  262,  413,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,  318,  435,   -1,  321,   -1,   -1,  440,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,  452,   -1,  454,  435,  340,   -1,   -1,  343,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,  452,   -1,  454,  359,  262,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,  262,  359,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,  318,  383,   -1,  321,  386,  387,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,  396,   -1,   -1,  335,   -1,
  435,   -1,   -1,  340,   -1,  440,  343,   -1,   -1,   -1,
   -1,   -1,  413,  318,   -1,   -1,  321,  452,   -1,  454,
   -1,   -1,  359,  328,   -1,  271,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,  435,  340,   -1,   -1,  343,  440,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,  452,   -1,  454,  359,  301,   -1,   -1,  271,  396,
   -1,   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,  413,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,  332,   -1,  301,   -1,
   -1,  396,  338,   -1,   -1,   -1,   -1,   -1,  435,  312,
  313,   -1,  315,  440,   -1,  318,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  332,
   -1,   -1,   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
  273,   -1,  398,   -1,  277,   -1,   -1,   -1,   -1,   -1,
   -1,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,  451,   -1,  453,   -1,   -1,
   -1,   -1,   -1,   -1,  460,   -1,  339,   -1,   -1,  432,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,  442,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  453,   -1,   -1,  489,  490,   -1,  492,  460,   -1,   -1,
  373,   -1,  375,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,  406,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  406,  499,  500,
  501,  502,  503,  504,  505,  506,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 515
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
"KW_UPDATING","KW_DELETING",
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
"ordinal : pos_short_integer",
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
"select : union_expr order_clause for_update_clause",
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
"for_update_clause : FOR UPDATE for_update_list lock_clause",
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
"select_item : rhs symbol_item_alias_name",
"select_item : rhs AS symbol_item_alias_name",
"from_clause : FROM from_list",
"from_list : table_reference",
"from_list : from_list ',' table_reference",
"table_reference : joined_table",
"table_reference : table_proc",
"joined_table : table_reference join_type JOIN table_reference ON search_condition",
"joined_table : '(' joined_table ')'",
"table_proc : symbol_procedure_name proc_table_inputs symbol_table_alias_name",
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
"group_clause : GROUP BY grp_column_list",
"group_clause :",
"grp_column_list : grp_column_elem",
"grp_column_list : grp_column_list ',' grp_column_elem",
"grp_column_elem : column_name",
"grp_column_elem : ordinal",
"grp_column_elem : udf",
"grp_column_elem : group_by_function",
"grp_column_elem : column_name COLLATE symbol_collation_name",
"group_by_function : numeric_value_function",
"group_by_function : string_value_function",
"group_by_function : case_expression",
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
node->nod_arg[0] = (DSQL_NOD)lex.param_number++;

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
end_chain = (DSQL_NOD) NULL;
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
		yylval = (DSQL_NOD) number;
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
			lex.g_field->fld_character_length = (USHORT) yyvsp[-2]; }
break;
case 80:
{ yyval = (DSQL_NOD) NULL; }
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
		  		(DSQL_NOD) NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
break;
case 136:
{ yyval = (DSQL_NOD) NULL; }
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
{yyval = (DSQL_NOD) NULL;}
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
{yyval = (DSQL_NOD) NULL;}
break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 168:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
break;
case 169:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
break;
case 170:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
break;
case 171:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
break;
case 172:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 173:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 174:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 176:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
break;
case 177:
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
break;
case 179:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 180:
{ 
		         check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
                                                (DSQL_NOD) lex.g_file); }
break;
case 181:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
break;
case 184:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
break;
case 185:
{ lex.g_file  = make_file ();}
break;
case 190:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
break;
case 191:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
break;
case 198:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 199:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 200:
{ yyval = yyvsp[0]; }
break;
case 201:
{ yyval = yyvsp[0]; }
break;
case 202:
{ yyval = (DSQL_NOD) NULL; }
break;
case 204:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 210:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 213:
{ yyval = NULL; }
break;
case 214:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                            yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 215:
{ yyval = yyvsp[0]; }
break;
case 216:
{ yyval = (DSQL_NOD) NULL; }
break;
case 217:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 218:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
break;
case 219:
{ yyval = yyvsp[-1]; }
break;
case 220:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 221:
{ yyval = yyvsp[0]; }
break;
case 222:
{ yyval = (DSQL_NOD) NULL; }
break;
case 226:
{ yyval = yyvsp[0]; }
break;
case 227:
{ yyval = yyvsp[0]; }
break;
case 228:
{ yyval = yyvsp[0]; }
break;
case 229:
{ yyval = (DSQL_NOD) NULL; }
break;
case 232:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 233:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 234:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 235:
{ yyval = make_node (nod_foreign, e_for_count,
                        make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 237:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 240:
{ yyval = yyvsp[0]; }
break;
case 241:
{ yyval = NULL ;}
break;
case 246:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 247:
{ yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 248:
{ yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
			         yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 250:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 251:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 252:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 253:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 256:
{ yyval = NULL;}
break;
case 257:
{ yyval = yyvsp[0];}
break;
case 258:
{ yyval = yyvsp[0];}
break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 264:
{ yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 265:
{ yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 266:
{ yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 267:
{ yyval = make_list (yyvsp[-1]); }
break;
case 268:
{ yyval = NULL; }
break;
case 269:
{ yyval = yyvsp[0]; }
break;
case 270:
{ yyval = NULL; }
break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 273:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 274:
{ yyval = make_list (yyvsp[0]); }
break;
case 275:
{ yyval = NULL; }
break;
case 277:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
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
case 285:
{ yyval = yyvsp[-1]; }
break;
case 286:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 287:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 288:
{ yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
break;
case 290:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 306:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
break;
case 307:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 309:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 311:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
break;
case 312:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 313:
{ yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
break;
case 316:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 317:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
break;
case 318:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
break;
case 319:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 320:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 321:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 322:
{ yyval = NULL; }
break;
case 323:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 324:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 325:
{ yyval = make_list (yyvsp[0]); }
break;
case 326:
{ yyval = make_list (yyvsp[-1]); }
break;
case 327:
{ yyval = NULL; }
break;
case 328:
{ yyval = make_list (yyvsp[0]); }
break;
case 329:
{ yyval = make_list (yyvsp[-1]); }
break;
case 330:
{ yyval = NULL; }
break;
case 333:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 336:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 337:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 338:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 339:
{ yyval = NULL; }
break;
case 341:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 342:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 349:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 350:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 353:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 355:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 357:
{ yyval = make_list (yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 362:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 363:
{ yyval = yyvsp[-1]; }
break;
case 364:
{ lex.beginning = lex_position(); }
break;
case 365:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 366:
{ lex.beginning = lex.last_token; }
break;
case 367:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 368:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 369:
{ yyval = 0; }
break;
case 370:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 371:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 372:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 373:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 374:
{ yyval = NULL; }
break;
case 375:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 376:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 394:
{ yyval = NULL; }
break;
case 395:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 396:
{ yyval = yyvsp[0]; }
break;
case 397:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 398:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 399:
{ yyval = yyvsp[0]; }
break;
case 400:
{ yyval = yyvsp[0]; }
break;
case 401:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 402:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 403:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 407:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 408:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 412:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 413:
{ yyval = yyvsp[0]; }
break;
case 414:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 416:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 417:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 418:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 422:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 424:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 448:
{ yyval = NULL; }
break;
case 449:
{ yyval = NULL; }
break;
case 450:
{ yyval = NULL; }
break;
case 451:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 452:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 453:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 454:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 455:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 456:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 457:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 458:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 460:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 461:
{ yyval = yyvsp[0]; }
break;
case 462:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 463:
{ yyval = yyvsp[0]; }
break;
case 464:
{ yyval = yyvsp[0]; }
break;
case 466:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 467:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 469:
{ yyval = NULL; }
break;
case 471:
{ yyval = NULL; }
break;
case 472:
{ yyval = yyvsp[0]; }
break;
case 473:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 474:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 477:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 489:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 490:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 492:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 493:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 494:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 500:
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
case 501:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 502:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 503:
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
case 504:
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
case 505:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 508:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 512:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 513:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 514:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 515:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 516:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 517:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 533:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 534:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 535:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 536:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 537:
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-1];
			}
break;
case 538:
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
			lex.g_field->fld_precision = (USHORT) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT) yyvsp[-1];
			}
break;
case 541:
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
case 542:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 545:
{ yyval = yyvsp[-1]; }
break;
case 546:
{ yyval = 0; }
break;
case 550:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 551:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 552:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 556:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 557:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 558:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 559:
{ yyval = 0; }
break;
case 560:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 563:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 564:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 567:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 568:
{ yyval = (DSQL_NOD) NULL; }
break;
case 570:
{ yyval = (DSQL_NOD) NULL; }
break;
case 571:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 573:
{ yyval = (DSQL_NOD) NULL; }
break;
case 575:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 580:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 581:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 582:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 583:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 584:
{ yyval = yyvsp[0];}
break;
case 586:
{ yyval = yyvsp[0];}
break;
case 587:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 594:
{ yyval = 0; }
break;
case 595:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 596:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 597:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 598:
{ yyval = (DSQL_NOD) 0; }
break;
case 599:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 600:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 602:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 603:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 604:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 605:
{ yyval = 0; }
break;
case 607:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 608:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 610:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 612:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 613:
{ yyval = make_list (yyvsp[0]); }
break;
case 614:
{ yyval = 0; }
break;
case 616:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 618:
{ yyval = 0; }
break;
case 619:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 620:
{ yyval = 0; }
break;
case 621:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 622:
{ yyval = 0; }
break;
case 623:
{ yyval = yyvsp[-1]; }
break;
case 624:
{ yyval = 0; }
break;
case 625:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 626:
{ yyval = 0; }
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
{ yyval = 0; }
break;
case 631:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 632:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 633:
{ lex.limit_clause = true; }
break;
case 634:
{ lex.limit_clause = false; }
break;
case 635:
{ lex.first_detection = true; }
break;
case 636:
{ lex.first_detection = false; }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 638:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 639:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 640:
{ yyval = 0; }
break;
case 641:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 642:
{ yyval = yyvsp[-2]; }
break;
case 643:
{ yyval = yyvsp[-1]; }
break;
case 644:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 645:
{ yyval = yyvsp[-1]; }
break;
case 646:
{ yyval = yyvsp[0]; }
break;
case 647:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 648:
{ yyval = 0; }
break;
case 649:
{ yyval = make_list (yyvsp[0]); }
break;
case 650:
{ yyval = 0; }
break;
case 652:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 654:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 655:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 656:
{ yyval = make_list (yyvsp[0]); }
break;
case 658:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 661:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 662:
{ yyval = yyvsp[-1]; }
break;
case 663:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 664:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 665:
{ yyval = make_list (yyvsp[-1]); }
break;
case 666:
{ yyval = NULL; }
break;
case 668:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 672:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 673:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 674:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 678:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 680:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 681:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 682:
{ yyval = make_list (yyvsp[0]); }
break;
case 683:
{ yyval = 0; }
break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 690:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 694:
{ yyval = yyvsp[0]; }
break;
case 695:
{ yyval = 0; }
break;
case 696:
{ yyval = yyvsp[0]; }
break;
case 697:
{ yyval = 0; }
break;
case 698:
{ yyval = yyvsp[0]; }
break;
case 699:
{ yyval = 0; }
break;
case 700:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 701:
{ yyval = 0; }
break;
case 702:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 703:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 704:
{ yyval = 0; }
break;
case 705:
{ yyval = 0; }
break;
case 707:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 708:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 711:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 712:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 713:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 714:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 717:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 718:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 724:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 728:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 729:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 735:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 738:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 742:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 743:
{ yyval = yyvsp[0]; }
break;
case 745:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 748:
{ yyval = NULL; }
break;
case 749:
{ yyval = make_list (yyvsp[-1]); }
break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 753:
{ yyval = NULL; }
break;
case 754:
{ yyval = make_list (yyvsp[-1]); }
break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 758:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 759:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 760:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 775:
{ yyval = yyvsp[-1]; }
break;
case 776:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 815:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 817:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 819:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 821:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 823:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 827:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 835:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 836:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 838:
{ yyval = make_list (yyvsp[-1]); }
break;
case 839:
{ yyval = yyvsp[-1]; }
break;
case 840:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 841:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 851:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 852:
{ yyval = yyvsp[0]; }
break;
case 853:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 854:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 855:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 856:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 857:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 858:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 859:
{ yyval = yyvsp[-1]; }
break;
case 860:
{ yyval = yyvsp[-1]; }
break;
case 864:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 865:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 866:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 867:
{ yyval = yyvsp[0]; }
break;
case 868:
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
case 869:
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
case 870:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 871:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 873:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 875:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 880:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 882:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 883:
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
case 884:
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
case 885:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 886:
{ yyval = make_parameter (); }
break;
case 887:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 888:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 889:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 894:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 895:
{ yyval = yyvsp[0]; }
break;
case 896:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 898:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 899:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 901:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 902:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 904:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 905:
{ yyval = yyvsp[0];}
break;
case 910:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 911:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 915:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 916:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 917:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 920:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 921:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 922:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 924:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 925:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 926:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 928:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 931:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 932:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 936:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 939:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 940:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 956:
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
