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
	int  prev_keyword;
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
  195,  206,  206,  196,  207,  201,  201,  201,  202,  202,
  202,  204,  204,  204,  204,  197,  200,  200,  205,  205,
  187,  187,  212,  213,  213,  214,  214,  214,  214,   11,
   71,   80,  216,  219,  219,  219,  220,  223,  227,  227,
  228,  228,  229,  229,  147,  218,   87,   89,  217,  217,
   70,   83,  232,  232,  232,  233,  236,  236,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  237,  234,  234,  235,    2,  239,  239,  239,
  239,  239,  239,  239,  247,  248,  245,  245,  249,  249,
  249,  249,  249,  249,  249,  240,  240,  250,  250,  250,
  250,  250,  250,  250,  244,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  252,  252,
  254,  254,  253,  251,  251,  251,  246,  246,  242,  243,
  243,  257,  257,  257,  257,  258,  258,  241,  259,  259,
  260,  260,    8,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  101,  101,  144,  144,  264,
  264,  267,  267,  269,  269,   53,   53,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  274,  274,  265,  265,
  265,  265,  275,  275,   61,   61,   61,   55,   55,  271,
  271,  271,  268,  268,  268,  279,  279,  279,  278,  278,
  277,  277,  277,  272,  272,  282,  280,  280,  280,  281,
  281,  273,  273,  273,  273,  283,  283,   18,   18,   18,
  285,  285,  285,   16,   16,   16,  287,  288,  291,  291,
  289,  293,  293,    4,   15,  292,  292,  294,  294,  295,
  295,  284,  296,  296,  297,  297,  298,  298,  298,  298,
  299,  299,  300,  300,  301,  301,  303,  303,  303,  304,
  304,  304,  305,  305,  305,  302,  307,  307,  307,  308,
  308,  306,  306,  309,  311,  311,  310,  310,  286,   17,
  312,  312,  312,  313,  313,  316,  316,  317,   65,   65,
   65,  319,  319,  318,  318,  314,  314,  322,  322,  323,
  323,  315,  327,  328,  329,  320,  321,  324,  324,  324,
  324,  330,  330,  330,  331,  331,  331,  221,  221,  222,
  222,  334,  334,  335,  335,  335,  325,  338,  338,  339,
  339,  340,  340,  341,  341,  342,  342,  209,  209,  344,
  344,  230,  230,   31,  231,  231,  231,  231,  231,  231,
  231,  231,  225,  225,  345,  345,  346,  346,  346,  346,
  346,  348,  348,  348,  226,  226,  224,  224,  326,  326,
  352,  353,  353,  353,  353,  353,  354,  354,  355,  355,
  356,  356,  357,  357,  357,  358,  358,   10,   10,  360,
  360,    7,    7,  361,  362,  363,   19,   19,  364,  365,
  366,  366,  188,  336,  336,    3,    3,  368,  368,  368,
  370,  370,   61,  369,  369,  371,  371,   34,   34,   85,
   84,   84,  359,  359,  372,  373,  373,  210,  210,  210,
  146,  367,  367,  111,  111,  111,  111,  111,  377,  377,
  377,  377,  377,  377,  376,  376,  376,  379,  379,  379,
  379,  379,  379,  379,  379,  379,  379,  380,  380,  380,
  380,  380,  380,  380,  380,  385,  385,  385,  385,  385,
  385,  385,  385,  385,  385,  385,  385,  385,  385,  385,
  385,  391,  391,  381,  381,  382,  382,  382,  382,  383,
  383,  387,  387,  388,  388,  388,  388,  386,  389,  384,
  384,  375,  375,  375,  378,  378,  378,  392,  392,  393,
  390,  395,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,  156,  156,
  156,  396,  394,  394,  151,  151,  400,  400,  400,  400,
  400,  398,  398,  398,  398,  398,  332,  152,  152,  153,
  154,  154,  154,  154,  154,   51,   51,   20,   20,  238,
  401,   54,  123,  270,  270,   95,  397,  397,  397,  397,
  402,  402,  402,  402,  402,  402,  402,  402,  402,  402,
  402,  403,  349,  350,  350,  350,  347,  347,  399,  351,
  351,  405,  405,  406,  406,  407,  407,  410,  410,  408,
  408,  413,  413,  412,  409,  411,  404,  404,  404,  404,
  404,  404,  404,  404,  333,  333,  155,   48,  276,  122,
  148,  374,  165,  211,  263,   63,   60,  215,  112,   66,
  337,   35,   41,  343,  262,   39,   42,  208,   40,  290,
  256,  256,  414,  414,  414,  414,  414,  414,  414,  414,
  414,  414,  414,  414,  414,  414,
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
    4,    2,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    6,    2,    2,    3,    0,
    1,    2,    4,    1,    3,    2,    2,    2,    1,    4,
    7,    7,    1,    1,    3,    4,    7,    2,    1,    3,
    1,    1,    6,    3,    0,    0,    0,    0,    3,    0,
    9,    9,    1,    1,    0,    2,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    5,    5,    5,
    5,    5,    5,    2,    0,    4,    2,    3,    3,    2,
    2,    3,    3,    2,    3,    6,    1,    2,    4,    3,
    2,    2,    2,    2,    3,    1,    3,    3,    3,    2,
    2,    4,    4,    5,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    2,    1,    1,    1,    0,    2,    2,    0,    1,
    2,    2,    2,    2,    2,    1,    3,    7,    1,    0,
    1,    0,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    3,    2,    2,    2,    1,    1,    1,    1,    4,
    5,    1,    3,    1,    3,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    4,    4,
    6,    5,    3,    0,    2,    2,    0,    3,    0,    4,
    1,    5,    4,    1,    4,    1,    2,    2,    1,    1,
    1,    2,    2,    2,    2,    1,    0,    3,    5,    1,
    1,    2,    3,    1,    2,    3,    0,    1,    1,    1,
    5,    5,    6,    1,    1,    1,    2,    4,    1,    0,
    5,    1,    0,    3,    2,    1,    0,    2,    0,    1,
    0,    3,    1,    0,    1,    2,    1,    1,    1,    1,
    2,    2,    1,    2,    3,    1,    1,    3,    3,    1,
    2,    3,    1,    2,    0,    2,    1,    1,    0,    1,
    1,    1,    3,    2,    3,    0,    1,    3,    4,    3,
    1,    3,    4,    3,    0,    1,    3,    3,    1,    1,
    0,    1,    1,    4,    0,    4,    0,    2,    0,    2,
    0,    9,   10,    0,    0,    0,    0,    3,    2,    1,
    0,    3,    5,    3,    2,    5,    2,    1,    1,    1,
    1,    1,    3,    1,    2,    3,    2,    1,    3,    1,
    1,    6,    3,    3,    2,    3,    0,    1,    3,    1,
    1,    1,    2,    1,    1,    1,    2,    1,    2,    1,
    2,    0,    3,    0,    1,    3,    1,    1,    1,    1,
    3,    1,    1,    1,    2,    0,    2,    0,    2,    0,
    4,    1,    2,    1,    1,    0,    1,    3,    2,    1,
    1,    2,    1,    4,    2,    1,    3,    8,    5,    1,
    3,    1,    1,    4,    4,    4,    1,    1,    5,    5,
    1,    3,    3,    1,    1,    7,    7,    5,    3,    0,
    1,    1,    1,    2,    0,    1,    1,    1,    0,    3,
    1,    3,    1,    0,    3,    1,    3,    1,    3,    3,
    1,    1,    3,    1,    2,    1,    3,    3,    1,    2,
    3,    3,    3,    3,    1,    3,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    1,    1,    5,    6,    3,    4,    5,    6,    3,
    4,    3,    4,    3,    4,    4,    5,    4,    4,    3,
    4,    1,    1,    1,    1,    1,    1,    1,    3,    3,
   10,   10,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    3,    3,    3,    3,    3,    3,    3,
    3,    1,    1,    1,    1,    3,    1,    1,    1,    1,
    1,    4,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    1,    2,    1,
    1,    1,    1,    1,    2,    1,    1,    1,    1,    1,
    4,    5,    5,    5,    5,    5,    5,    5,    5,    5,
    5,    6,    6,    6,    8,    4,    4,    3,    6,    1,
    1,    6,    6,    1,    1,    4,    6,    4,    5,    3,
    5,    4,    5,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    0,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  548,
  549,  550,  554,  555,  556,    0,  611,  722,  723,  727,
  728,  459,    0,    0,    0,    0,    0,    0,  397,  566,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  900,    0,   21,  898,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  473,    0,    0,   37,   34,
   36,    0,   35,    0,  981,  983,  985,  984,  990,  986,
  993,  991,  987,  989,  992,  988,  994,  995,  996,    0,
    0,    0,    0,   32,   68,   66,  973,  982,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    0,    0,    0,  672,    0,  975,    0,    0,
    0,    0,  107,  980,  557,    0,    2,    0,    0,    0,
    0,  427,  428,  429,  430,  431,  432,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
  446,  447,  448,    0,  425,  426,    0,  966,    0,  404,
  970,    0,  401,  972,    0,  674,    0,  400,  976,    0,
  564,  896,    0,  150,  103,    0,  104,    0,  217,  962,
  761,    0,  102,  145,  969,    0,    0,   98,    0,  105,
  902,    0,   99,    0,  100,    0,  101,  979,  146,  106,
  619,  620,    0,  901,  899,    0,   74,    0,  967,    0,
  965,  481,  474,    0,  480,  485,  475,  476,  483,  477,
  478,  479,  484,    0,   30,    0,   39,  748,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  906,    0,  887,  634,  634,  635,
  645,  647,  955,  648,    0,  649,  639,  635,    0,    0,
    0,    0,    0,    0,    0,  583,  572,    0,  575,  577,
  578,  579,  580,  586,  587,    0,  974,  673,    0,  108,
    0,  109,    0,  110,    0,    0,    0,  612,    0,  610,
    0,    0,    0,    0,  460,    0,    0,  367,    0,  220,
    0,  407,  398,  457,  458,    0,    0,    0,    0,    0,
    0,  416,    0,  373,  374,    0,  570,  568,  897,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  958,   75,    0,    0,    0,  724,  725,  482,
    0,    0,    0,    0,  865,  893,    0,  957,    0,  867,
    0,    0,  894,    0,  888,  879,  878,  877,    0,    0,
    0,    0,  869,  870,  871,  880,  881,  889,  890,    0,
  891,  892,    0,    0,    0,  895,    0,    0,    0,    0,
    0,  883,    0,  758,  862,  863,  864,  670,  868,  350,
  848,    0,    0,    0,  847,    0,  668,  851,  909,  910,
  850,  844,  845,  846,  849,  882,  907,  908,  930,  931,
  934,  935,    0,  751,    0,    0,   27,    0,    0,    0,
   71,   69,  977,   67,   33,    0,    0,    0,  753,    0,
   52,    0,    0,    0,    0,    0,  562,    0,    0,    0,
  642,  644,    0,  651,    0,  735,    0,    0,  652,    0,
  638,    0,  609,    0,  584,    0,  581,    0,  582,  607,
    0,  602,    0,    0,  576,  762,  731,    0,    0,    0,
    0,    0,    0,  559,  558,    0,    0,  616,  613,    0,
  185,    0,    0,  129,  465,  172,  173,    0,  463,    0,
    0,    0,    0,  466,    0,  461,    0,    0,  411,  413,
  412,    0,  414,    0,  408,  218,    0,    0,  271,    0,
    0,    0,    0,  420,  421,    0,  450,    0,    0,    0,
    0,    0,    0,    0,  378,  377,  469,    0,    0,  149,
    0,    0,  158,    0,  157,    0,  164,  147,    0,  162,
  165,  166,  154,    0,    0,  504,  540,  507,    0,  541,
    0,    0,  508,    0,    0,    0,  531,  544,  503,  526,
  505,  506,  501,  488,  367,  486,  487,  489,    0,    0,
  498,  499,  500,  502,    0,    0,    0,    0,    0,  112,
    0,  113,    0,  124,    0,    0,  201,    0,    0,  365,
    0,  220,    0,    0,   83,    0,    0,  743,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  764,
  766,  775,  778,  779,  780,  781,  782,  783,  784,  785,
  786,  787,    0,    0,    0,  884,    0,    0,    0,    0,
    0,    0,  885,  886,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  325,  978,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  750,   28,    0,    0,   44,   72,    0,    0,   26,    0,
  756,    0,    0,    0,  719,    0,    0,    0,    0,    0,
  561,    0,  634,    0,    0,    0,    0,    0,  971,  655,
  552,    0,  904,  551,    0,  585,    0,  593,  589,  588,
    0,    0,    0,  604,  592,    0,    0,  729,  730,    0,
    0,    0,  365,    0,    0,    0,    0,    0,    0,  130,
    0,    0,    0,    0,    0,    0,  367,  410,    0,  367,
  368,  415,  496,    0,  273,    0,  267,  269,  365,  240,
  963,    0,    0,  367,    0,    0,  209,  365,    0,    0,
    0,    0,  245,  239,  242,  243,  244,  419,  455,  454,
  418,  417,    0,    0,    0,    0,    0,    0,  376,    0,
  367,    0,  155,  159,    0,  163,    0,    0,  527,  528,
  545,    0,  542,    0,    0,  534,  532,  533,    0,    0,
    0,    0,  497,    0,    0,    0,    0,  535,    0,    0,
  365,    0,  200,    0,  203,  205,  206,    0,    0,    0,
    0,    0,  220,   78,    0,    0,   77,  515,  959,  516,
    0,    0,    0,    0,  765,  777,    0,    0,  835,  836,
  837,    0,  769,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  947,  948,  949,  950,
  951,  952,  953,  954,    0,    0,    0,    0,    0,    0,
  940,    0,    0,    0,    0,  860,  327,  861,  928,    0,
    0,  856,  961,    0,    0,    0,    0,    0,  669,    0,
  866,  760,  759,  752,    0,    0,    0,    0,    0,    0,
    0,    0,   53,   61,   63,    0,    0,   70,    0,    0,
    0,  755,    0,    0,    0,    0,    0,    0,    0,    0,
  643,  646,    0,    0,    0,    0,  660,  661,    0,    0,
  653,  656,  553,  905,  594,  603,  598,  597,    0,  608,
  763,  732,  733,  365,    0,    0,  636,  618,  617,    0,
    0,  626,  181,  175,  182,    0,  178,    0,    0,  177,
    0,  188,  169,  168,  903,  171,  170,  467,    0,    0,
    0,  409,  272,    0,  211,  367,  213,    0,  368,  208,
  214,    0,    0,    0,    0,  422,  423,  367,  365,  368,
    0,    0,    0,  394,    0,  196,  197,  156,    0,    0,
    0,    0,    0,    0,  543,    0,    0,  367,  368,    0,
  492,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  198,    0,    0,    0,    0,  354,
    0,   82,    0,    0,    0,    0,   87,   84,    0,    0,
    0,  726,  964,    0,    0,  770,    0,    0,  776,    0,
    0,    0,    0,    0,  813,  812,    0,    0,    0,  820,
  838,  830,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  768,    0,    0,    0,  220,    0,  911,    0,
    0,  926,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  936,    0,  946,    0,    0,    0,
    0,    0,    0,  927,  872,   65,   57,   58,   64,   59,
   60,    0,    0,   22,    0,    0,    0,   42,    0,    0,
    0,  737,  757,    0,  720,    0,  736,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  675,    0,    0,
    0,    0,    0,  600,  601,  605,    0,  199,    0,    0,
  630,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,  405,  223,  224,  225,  226,  227,  228,  875,    0,
    0,    0,  276,    0,  221,    0,    0,    0,    0,  246,
    0,  451,  452,  424,    0,    0,    0,    0,    0,    0,
  367,  471,  368,  167,  960,    0,    0,  510,    0,  509,
  546,    0,  538,    0,    0,    0,  490,    0,  518,    0,
    0,  520,  523,  525,  365,    0,    0,    0,  119,    0,
  204,  367,    0,    0,  366,    0,   97,    0,  117,    0,
   93,    0,    0,    0,    0,   86,    0,  828,  829,  772,
    0,  774,    0,    0,    0,    0,    0,    0,    0,  831,
    0,    0,    0,    0,    0,    0,    0,  821,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  917,  916,    0,    0,  913,  912,    0,  921,
  920,  919,  918,  915,  914,    0,    0,  942,    0,    0,
    0,  941,    0,    0,    0,    0,    0,    0,   56,   62,
   54,   55,   23,  741,  742,    0,  739,  746,  747,  744,
    0,    0,  718,    0,    0,  663,    0,  664,    0,  681,
  677,  679,    0,    0,    0,    0,    0,  366,  622,  623,
  637,    0,  183,  179,    0,    0,  194,  190,    0,  368,
  876,  279,    0,    0,  368,  277,    0,    0,    0,  231,
    0,    0,  247,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  468,  512,    0,  513,    0,  134,    0,    0,
    0,  138,  140,  141,  142,  493,  495,  491,  522,    0,
    0,  368,    0,    0,    0,    0,  351,    0,  355,  367,
   88,    0,    0,   90,    0,    0,    0,    0,    0,    0,
  840,  839,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  219,  929,  922,  923,    0,  924,  938,  937,    0,  943,
  932,  933,    0,   40,    0,    0,  721,  666,    0,  536,
    0,  688,    0,    0,  685,  689,  690,  692,  693,  694,
    0,    0,  632,  368,  352,  624,    0,  174,  191,  368,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  284,  289,  283,
    0,    0,    0,  293,  294,  295,  296,  297,  298,  300,
  301,  303,  304,  305,  308,  266,    0,    0,  207,  232,
    0,    0,    0,    0,  233,  236,    0,    0,  368,  392,
  393,  389,  388,  391,  390,    0,  511,  539,  143,  131,
  139,    0,  367,  263,  126,  368,    0,    0,  369,  356,
    0,    0,   91,   92,   79,    0,  799,  807,    0,  800,
  808,  803,  811,    0,    0,  801,  809,  802,  810,  796,
  804,  797,  805,  798,  806,    0,  939,    0,  738,    0,
    0,    0,    0,    0,  702,  704,    0,  699,    0,  264,
  184,  406,    0,  311,    0,    0,  314,  315,    0,  307,
    0,    0,    0,    0,  306,    0,  337,  338,  292,  299,
    0,  302,  285,    0,  290,    0,  341,  291,  210,  215,
  234,    0,    0,  237,    0,  249,  251,  396,  368,    0,
  371,    0,    0,    0,  361,  362,    0,    0,   76,   94,
    0,  925,    0,    0,    0,  691,  686,  703,    0,    0,
  282,    0,  309,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  332,  333,  349,    0,    0,    0,    0,
  344,  342,  238,    0,    0,    0,    0,    0,  265,  368,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  710,    0,    0,    0,  281,  278,  310,    0,    0,  313,
    0,    0,    0,    0,  321,    0,  324,    0,  348,  347,
  968,  346,    0,    0,    0,    0,    0,  248,    0,  255,
    0,  254,  372,  364,    0,    0,  357,  118,    0,    0,
    0,  712,  701,    0,    0,  713,    0,  709,    0,    0,
    0,    0,    0,    0,    0,    0,  335,  334,  343,  345,
  235,    0,    0,  259,  258,  257,    0,    0,  842,  633,
  708,    0,  715,    0,    0,  312,  318,    0,    0,    0,
    0,  336,  262,  260,  261,    0,    0,    0,    0,    0,
    0,  339,  316,    0,    0,    0,    0,  714,  330,  317,
  319,  841,  717,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1544,   28,   29, 1545,
   31,   32,   33,   34,   35,   36, 1546,   38, 1547,  638,
  120,  456,  951, 1184,  121,  704,  952,  122,  460,  709,
  146,  123,  124,  257,  974,  136,  953,  954,  197,  226,
  125,  955,  126,  462,   82,  237,  374,  421,  633, 1085,
  422,  856,  604, 1510,  833,  634,  635,  636, 1087,  238,
 1374,   74,  187,   75,  233, 1828, 1297,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  620,  622,  453,  258,  783,  538,  648, 1022,  367,
  625, 1073, 1289,  571,  733, 1048,  523,  524,  553,  359,
  605, 1059, 1440, 1569, 1232, 1441, 1442, 1443, 1444, 1445,
  649,  214,  206,  355,  578,  356,  357,  579,  580,  581,
  582, 1264, 1016,  526,  527, 1006, 1004,  528, 1010, 1005,
 1222, 1403, 1011, 1012, 1227,  369,  844,  845,  846,  847,
  785, 1029, 1418,  606,  787,  424,  849,  932,  547, 1346,
 1233,  425,  426,  427,  428,  429, 1419, 1420,  556, 1575,
 1726, 1250,  793,  794,  780,  795,  796,  797, 1727, 1728,
 1805,  337,  551, 1241, 1548,  193,  548,  549, 1242, 1243,
 1413, 1702, 1549, 1550, 1551, 1552, 1666, 1553, 1554, 1555,
 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565,
  430, 1790, 1649, 1713, 1794, 1711,  431,  688,  432,  433,
 1092, 1667, 1720, 1721, 1760, 1078, 1295, 1457, 1079, 1080,
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
  441, 1741, 1639, 1742, 1743, 1744, 1788, 1829,  468, 1196,
   48,   49,  379,   50,   51,  509,  510,  960, 1192, 1376,
 1380,  469,  712,  211,  650,  651,  874,  875,  652,  653,
  654,  655,  656,  657,  658,  659,  660,  661,  662, 1318,
 1108, 1110, 1111,  931,  687,  442,  443,  444,  445,  446,
  235,  447,  448,  915,  449,  450,  451,  452,  678,  919,
 1168, 1163,  679,  128,
};
static short yysindex[] = {                                   7523,
 2112, -151, 2559,  104, 1213,   90, 3211,  354, 4247,   42,
  489, 3739, -151, 1287,  597, 4941,  925, 4941,  317,    0,
  792,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  578,    0,    0,    0,    0,
    0,    0,12195, 4941, 4941, 4941, 4941, 4941,    0,    0,
  448, 1301, 4941, 4941, 4941,  643, 4941,  465, 4941, 4941,
    0, 4941, 4941,    0,  955,    0,  577,    0,    0,  777,
 4941,    0, 4941, 4941, 4941,  833, 4941, 4941, 4941, 4941,
  465, 4941, 4941, 4941, 4941,    0, 4941,  807,    0,    0,
    0, 1179,    0, 1179,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  889,
  987,  233, 1195,    0,    0,    0,    0,    0, 4941, 4941,
 4941,  952,  994, 1072,  138, 1485, 1033,  141,  144,   32,
  991,    0, 4941, 1172, 1725,    0, 1123,    0, 4941, 4941,
 4941, 4941,    0,    0,    0, 4941,    0, 1292,   -4, 1252,
   12,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  996,    0,    0, 1301,    0,  492,    0,
    0, 1551,    0,    0, 1093,    0,  727,    0,    0, 1178,
    0,    0, 1152,    0,    0, 1162,    0, 1362,    0,    0,
    0, 1301,    0,    0,    0,  954, 1551,    0,   20,    0,
    0, 1320,    0, 1337,    0, 1179,    0,    0,    0,    0,
    0,    0, 1347,    0,    0, 1235,    0, 1377,    0, 1316,
    0,    0,    0, 1235,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8458,    0, 4941,    0,    0,    0, 4709,
 1342, 5022, 4941, 1190, 1460, 1762, 1495, 1491, 4709, 1415,
 5022, 1438, 1442, 1334,    0,10149,    0,    0,    0,    0,
    0,    0,    0,    0, 7730,    0,    0,    0, 1391, 4941,
 1476, 1394,  766, 4941, 1425,    0,    0, 1725,    0,    0,
    0,    0,    0,    0,    0, 4941,    0,    0, 1551,    0,
 1320,    0, 1179,    0, 1490,10149, 1473,    0, 1461,    0,
 1040, 1506, 1205,   12,    0, 1631,  851,    0, 4941,    0,
  996,    0,    0,    0,    0, 4941, 1509, 3971, 1453, 4035,
 1887,    0,12195,    0,    0,  674,    0,    0,    0, 1876,
 1876, 1301, 1576, 1301, 3172, 1162,    0,    0, 7696,    0,
 4941, 4941,    0, 1509,    0,    0, 1666,  711, 1925, 4941,
 1706, 4941,    0,    0, 1934,  191, 5318,    0,    0,    0,
 1936, 1942, 1943, 1523,    0,    0, 1953,    0, 1955,    0,
 1965, 1967,    0, 1977,    0,    0,    0,    0,    0, 1983,
 1557, 1566,    0,    0,    0,    0,    0,    0,    0, 1990,
    0,    0, 8555, 1991, 1992,    0,10149,10149, 7828, 4941,
 2007,    0, 1810,    0,    0,    0,    0,    0,    0,    0,
    0, 2016, 1975,    0,    0, 2022,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1014,    0, 4941, 1637,    0, 4941, 4941,  100,
    0,    0,    0,    0,    0, 4941, 4941, -107,    0, 4941,
    0, 1754, 4941, 2033, 4709, 1678,    0, 4941, 8794, 1883,
    0,    0,10149,    0, 1810,    0, 1763, 2038,    0, 3486,
    0,  122,    0,  439,    0,  -86,    0,  -86,    0,    0,
 2040,    0,  182, 1657,    0,    0,    0, 2041,  113, 2036,
 1509, 2061, 1839,    0,    0, 1597, 2062,    0,    0, 1723,
    0,  174, 1771,    0,    0,    0,    0, 1301,    0, 1876,
 1876, 1876, 1876,    0, 2064,    0, 1843, 1845,    0,    0,
    0, 1828,    0, 7696,    0,    0, 7696, 1446,    0, 1551,
 1886, 4941,12070,    0,    0,  517,    0, 4941, 1322, 1093,
 1727, 1704, 1681,    0,    0,    0,    0,  614, 1757,    0,
 1698,  465,    0, 1301,    0, 1881,    0,    0, 3172,    0,
    0,    0,    0,  189, 1713,    0,    0,    0, 1726,    0,
 1773, 2121,    0, 1825, 2136,  -20,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2089,  607,
    0,    0,    0,    0,  184, 2145, 2148, 2136, 1551,    0,
 1878,    0, 1945,    0, 1301, 1301,    0, 3971,  727,    0,
 1823,    0, 1802, 2171,    0, 9526, 1073,    0, 1830, 1835,
 2182, 5728, 2184,    0,    0,    0, 5221, 4754,    5,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  119, 9033,  193,    0, 4941,10149,  808, 1186,
 1212, 2574,    0,    0,10149, 5880, 1810, 1780,  574,10149,
 9033, 1049, 1049, 1287, 1961, 1452, 2185,    0,    0, 9113,
 4941,10149,10149,10149,10149,10149, 9033,10149, 1011, 4941,
    0,    0, 4520, 1799,    0,    0, 1784, 5022,    0, 1932,
    0, 1749, 1287, 2212,    0, 1932, 4520, 1935, 1937, 4941,
    0, 1961,    0, 2075,  814, 1804, 9033, 4941,    0,    0,
    0,  730,    0,    0,  -50,    0, 1820,    0,    0,    0,
 4941, 1074, 4941,    0,    0, 4941, 4941,    0,    0, 9033,
 1994, 3971,    0, 1768,10149, 4941, 1819, 1301, 1301,    0,
  -83, 1698, 1698, 1813,  465, 1205,    0,    0, 2237,    0,
    0,    0,    0, 2002,    0, 4941,    0,    0,    0,    0,
    0, 2006, 2241,    0, 1982, 1999,    0,    0, 1946, 1948,
 1179, 2008,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  465, 4941,12151, 1901, 1903, 1907,    0,  465,
    0, 1521,    0,    0, 1877,    0,   93, 1880,    0,    0,
    0,  465,    0, 2121,  148,    0,    0,    0, 2000,  148,
 1884,  148,    0, 2254,  465,  465,  465,    0, 1509, 4941,
    0, 1947,    0, 1760,    0,    0,    0,  674, 1890, 4941,
 1775,  118,    0,    0, 2271, 2043,    0,    0,    0,    0,
  191, 4941, 1895, 6119,    0,    0, 1895, 5728,    0,    0,
    0, 4269,    0,  134,  510,10149,10149, 6216, 2275, 1003,
 6470,10149, 6622, 1124, 6731, 6985, 9352, 7137, 7246, 7500,
 5880, 5880,10149,10149, 2054,10149, 2278,10149, 2277, 2263,
10149,10149,10149,10149,10149,10149,    0,    0,    0,    0,
    0,    0,    0,    0, 2011, 1402,  -12,10149,  587, 9033,
    0, 5880, 2094, 2301,   32,    0,    0,    0,    0, 1810,
 1790,    0,    0, 2042, 1049, 1049, 1427, 1427,    0,  255,
    0,    0,    0,    0, 4941, 4941, 4941, 4941, 4941, 4941,
  172, 2302,    0,    0,    0, 4520, 1962,    0,  -42, 1987,
 4941,    0,   32, 9033, 1987, 2307, 2302, 4520, 4793, 2032,
    0,    0,  814, 2321, 2318,  651,    0,    0, 5880, 2037,
    0,    0,    0,    0,    0,    0,    0,    0,  514,    0,
    0,    0,    0,    0, 1831, 1890,    0,    0,    0, 2320,
 1862,    0,    0,    0,    0, 1853,    0, 1876, 2104,    0,
  -83,    0,    0,    0,    0,    0,    0,    0, 2332, 5880,
 2014,    0,    0, 2074,    0,    0,    0, 2014,    0,    0,
    0, 1179, 1179, 1874, 2338,    0,    0,    0,    0,    0,
  -41,  -59,  -21,    0, 2117,    0,    0,    0, 4941,  104,
 1854, 1964, 2002, 2343,    0, 1698, 1860,    0,    0, 1138,
    0, 2331, 4941, 1208,  465, 2350, 2352, 2353, 2131,  727,
 2074, 1876, 1771, 3971,    0, 1757,   32, 1944, 1957,    0,
  634,    0,  465, 2005, 2091, 9526,    0,    0,  465, 1910,
 2095,    0,    0, 2364, 2367,    0, 5880, 5880,    0, 5880,
 5880, 2068, 1810, 2369,    0,    0, 1810, 2370, 9591,    0,
    0,    0, 2060, 2374, 1810, 2378, 1656, 2379, 1810, 2382,
10149,10149, 2275,10149, 9671, 2385, 1810, 2388, 2389, 1810,
 2392,10149, 1810, 2394, 1810, 2395, 2396, 1810, 2402, 2405,
 1810, 2406,    0, 2188, 2670, 2711,    0, 2718,    0, 2730,
10149,    0, 2738, 2796, 2926, 2994, 3017, 3032,10149,  465,
 9033, 1810, 2012, 9033,    0,10149,    0, 2144,  493,10149,
 9033,10149,10149,    0,    0,    0,    0,    0,    0,    0,
    0, 2127, 4520,    0, 4520,  172, 2302,    0,  177,  177,
  146,    0,    0, 7730,    0, 1870,    0, 2307, 2302, 2412,
 4793,  651, 2417, 9033, 4941,  814, 2072,    0, 2073, 2077,
 2124, 2194, 2138,    0,    0,    0, 2074,    0, 1944,  780,
    0, 2058, 1301, 2092, 1698, 2096, 1698,    0, 5880,  266,
 1781,    0,    0,    0,    0,    0,    0,    0,    0, 2044,
 2220, 2074,    0,10149,    0, 2205, 2097, 1874,  955,    0,
 5880,    0,    0,    0, 2119, 2120, 2122, 2126, 2129, 2130,
    0,    0,    0,    0,    0, 2466,  104,    0, 1813,    0,
    0,  148,    0, 2014, 2135,  148,    0,  148,    0, 2002,
 2476,    0,    0,    0,    0,  674, 2220, 1698,    0, 1771,
    0,    0, 7730, 2243,    0,    3,    0, 2320,    0, 2482,
    0, 2484, 1301,  -15, 2485,    0, 1301,    0,    0,    0,
 2188,    0, 2188,10149, 2106, 2106, 1287, 2487, 1888,    0,
 2106, 2106,10149, 2106, 2106, 2436, 1810,    0, 1796,10149,
 1810, 2106, 2106, 2106, 2106, 1810, 2106, 2106, 2106, 2106,
 2106, 2106,    0,    0, 7696, 2489,    0,    0, 3179,    0,
    0,    0,    0,    0,    0, 3214,  167,    0, 9033, 2226,
 2102,    0, 9033, 3230, 1913, 1636, 1810, 2154,    0,    0,
    0,    0,    0,    0,    0, 2109,    0,    0,    0,    0,
 1763, 9033,    0, 4793, 2412,    0, 1978,    0,  651,    0,
    0,    0,  814, 3081, 5880, 2142, 2220,    0,    0,    0,
    0, 1876,    0,    0, 1301, 1521,    0,    0,  285,    0,
    0,    0, 4941,11969,    0,    0, 1810, 2260, 2205,    0,
  960, 4941,    0, 2210,  464, 2110, 2217, 2115, 2257, 2225,
 2264, 2074,    0,    0, 2525,    0, 2526,    0, 2216, 2260,
 2135,    0,    0,    0,    0,    0,    0,    0,    0, 2074,
 1757,    0, 1521, 2117, 2252, 2189,    0, 1890,    0,    0,
    0, 2202, -106,    0, 2002, 2204, 1810, 2537, 2538,   32,
    0,    0, 2539, 2540, 1810, 2542, 2543,10149,10149, 1810,
 2546, 2549, 2550, 2551, 2552, 2554, 2561, 2565, 2566, 2568,
    0,    0,    0,    0,  465,    0,    0,    0, 9033,    0,
    0,    0, 1804,    0,  177, 1804,    0,    0, 1405,    0,
 2330,    0, 2567, 2556,    0,    0,    0,    0,    0,    0,
    5,  736,    0,    0,    0,    0, 1698,    0,    0,    0,
    0, 7696, 1489,  505, 2553,  476, 2576, 2280,10149, 2563,
 2584, 2575, 2577, 2580, 2583, 2287, 2592,    0,    0,    0,
 2325,11895, 2593,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2594, 4941,    0,    0,
 2291, 2310, 4941, 1874,    0,    0, 1179, 4941,    0,    0,
    0,    0,    0,    0,    0, 2220,    0,    0,    0,    0,
    0, 2220,    0,    0,    0,    0,  926, 1804,    0,    0,
10149, 1301,    0,    0,    0, 1301,    0,    0,10149,    0,
    0,    0,    0, 1810, 1810,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2614,    0, 2037,    0, 2037,
 5880, 4941,  951, 3081,    0,    0, 2292,    0, 2621,    0,
    0,    0,  103,    0, 9910, 4941,    0,    0,10149,    0,
  622, 2328, 5880, 3043,    0, 5880,    0,    0,    0,    0,
 2324,    0,    0,  918,    0, 2221,    0,    0,    0,    0,
    0, 1874, 1179,    0, 2288,    0,    0,    0,    0, 2117,
    0,  926, 2630,  651,    0,    0, 2037, 1810,    0,    0,
 1636,    0, 2138, 2138,    5,    0,    0,    0, 5466, 2014,
    0, 2619,    0, 3245, 8458, 1063,10149, 2324,  609,10149,
 2620,  831, 1587,    0,    0,    0, 4941, 4941,  104,  373,
    0,    0,    0, 2288,  -55, 1874, 2298, 2299,    0,    0,
  651, 2646,  926, 2345, 2138, 2648, 1804, 2142, 2142, 4941,
    0, 2649, 2647,  491,    0,    0,    0, 2282, 2324,    0,
 1203,  220, 2262, 1810,    0, 2391,    0, 2324,    0,    0,
    0,    0,11969,  918, 1874,  694,  694,    0, 2398,    0,
 2258,    0,    0,    0,  651,  926,    0,    0, 2037, 2314,
 2314,    0,    0, 5466, 2663,    0, 4941,    0,  787, 2645,
 1714, 2324, 2415, 2401,11969,11969,    0,    0,    0,    0,
    0, 2246,   36,    0,    0,    0, 1915, 2138,    0,    0,
    0, 4941,    0, 2324, 2674,    0,    0,  423, 4941,11969,
 2413,    0,    0,    0,    0, 5880, 2142, 2675, 2673, 1984,
11969,    0,    0,11969,    5, 2314, 4941,    0,    0,    0,
    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  179, 1010,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  156, 1553,    0,    0,    0,    0,    0,    0,
 2723,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  752,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1125,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2397,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2346,    0,    0,
    0,  326,    0,  326,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2356,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1165,    0,    0, 8067,
 5576,    0,    0,    0, 1249,    0,    0,    0, 2882,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  355,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   15,    0,    0,    0,    0,  409,    0,    0, 1269,
    0,    0,    0,    0,    0, 1497,    0,10510,    0,    0,
    0,    0,    0,    0,    0,    0,   15,    0,  639,    0,
    0, 2693,    0,    0,    0, 2472,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1321,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1361,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  772,    0,    0,    0,    0,
    0,    0,    0, 5254,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0, 1363,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   15,    0,
 2693,    0, 2472,    0, 1371,    0,    0,    0,    0,    0,
    0,    0,    0, 1383,    0, 2462,    0,    0,    0,    0,
 1421,    0,    0,    0,    0,    0, 2477,  475, 5816,    0,
 1439,    0,    0,    0,    0,  376,    0,    0,    0, 2285,
 2285,    0,    0,    0, 1448, 1814,    0,    0,    0,    0,
    0,    0,    0, 2477,    0,    0, 1419,    0,    0,    0,
    0,    0,    0,    0, 8002, 2355,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1824,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2603,
    0,    0,    0, 2856, 3109,    0,    0,    0,    0,    0,
    0,    0,  349,    0,    0,    0,    0,    0,    0,    0,
    0,  206, 3362, 2297,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1467,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1468,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0, 2426,    0,  175,
    0,    0,    0,    0,    0,  132,    0,  132,    0,    0,
  744,    0,  790,  886,    0,    0,    0,    0, 1321,    0,
 2477,    0,    0,    0,    0,  159,  920,    0,    0,  131,
    0, 2831,  166,    0,    0,    0,    0,    0,    0, 2285,
 2285, 2285, 2285,    0,  228,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2491,
    0,    0,    0,    0,    0, 2462,    0,    0, 1670,    0,
 2300,    0,    0,  869,    0,    0,    0,    0,  740,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1478,    0,
    0,    0,    0, 1341, 2153,    0,    0,    0, 5844,    0,
    0, 8357,    0,    0, 8930,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1609, 1791,
    0,    0,    0,    0,10468, 5190,    0, 8930,   15,    0,
    0,    0,    0,    0,    0,    0,    0,  475,  703,    0,
    0,    0,    0, 2357,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10720,10852,11027,    0,    0,  603,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10229,    0,10229,    0,    0,    0,10229,10229,
10229,    0,    0,    0,    0,    0, 2323,    0,    0,    0,
    0, 4332, 4564, 8219, 2725,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  269,
    0,    0, 1553,    0,    0,  269,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  838,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  684,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  475,    0,  839,    0,    0,  406,    0,    0,    0,
 2866,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  252,    0,    0,    0,    0,    0,    0,
    0, 2736,    0,    0, 2444, 1365,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  377,  395,  428,    0,    0,
    0, 2020,    0,    0,    0,    0,    0, 1985,    0,    0,
    0,    0,    0, 8357,    0,    0,    0,    0,  805,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2477,    0,
    0,  312,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10011,    0,    0,    0,  231,    0,    0,    0,    0,
 2469,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,10229,    0,    0,    0,    0, 1571,
    0,    0,    0, 3594, 4817, 5049, 3847, 4079,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1479, 1499,    0,    0,    0,    0,    0,    0,    0, 1508,
    0,    0, 8067,    0, 1508, 1535, 1542,    0,    0,    0,
    0,    0,    0,  701, 1331, 2146,    0,    0,    0, 1164,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  133,
    0,    0,    0,    0,    0,    0,    0, 2285, 2333,    0,
 3084,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2511,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1589,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1544,    0,    0,    0,    0,    0,
    0,    0, 2570,    0,    0,    0,    0,    0,    0,    0,
    0, 1276,    0,    0,    0,    0,    0,    0,    0,  703,
 2511, 2285, 1627,  475,    0, 2524, 8067, 1635,  124,    0,
    0,    0,    0,10011,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2407,    0,    0,    0, 6095,    0,    0,    0,
    0,    0,    0,    0, 6421,    0, 7516,    0,10495,    0,
    0,    0,    0,    0,    0,    0,10936,    0,    0,11104,
    0,    0,11110,    0,11242,    0,    0,11284,    0,    0,
11374,    0,    0, 9334,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2360,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1479, 1658,    0, 2365,  333,
    0,    0,    0,    0,    0,    0,    0, 1676, 1680, 1697,
    0, 2449, 1279,    0, 3219,    0, 2461,    0, 2467, 2468,
    0,    0,  637,    0,    0,    0, 2511,    0, 1635,    0,
    0, 3467,    0,    0,    0, 2354,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 5947,
    0, 2544,    0,    0,    0,  535,    0, 1589, 2397,    0,
    0,    0,    0,    0,  705,  723,  750,  751,  778,  788,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  455,    0,    0,    0,    0, 1847,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1710,
    0,    0,    0,    0,    0,    0,    0, 1711,    0,    0,
    0,    0,    0,  171,    0,    0,    0,    0,    0,    0,
  289,    0,  300,    0,    0,    0, 8219,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11416,    0,11448,    0,
11531,    0,    0,    0,    0,11551,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1817,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1736,    0,    0,    0, 2201,    0,
    0,    0,    0,    0,    0,  185,    0,    0,    0,    0,
    0, 2285,    0,    0,    0, 3431,    0,    0,    0,    0,
    0,    0,    0, 2509,    0,    0, 2777, 1610, 1792,    0,
 2462,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2511,    0,    0,    0,    0,    0,    0,    0, 1746,
  978,    0,    0,    0,    0,    0,    0,    0,    0, 2511,
 2524,    0,  366,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  130,    0,11647,    0,    0,10229,
    0,    0,    0,    0,11681,    0,    0,    0,    0,11745,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  767,    0,  333, 1346,    0,    0, 2449,    0,
  953,    0,    0, 1221,    0,    0,    0,    0,    0,    0,
  227, 2779,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1654, 1733,    0,    0,    0,    0,    0,    0,    0,
    0, 2513,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2626,    0,    0,  128,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  106,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11777,11819,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  121,    0, 1021,
    0,    0,    0,    0,    0,    0, 2790,    0,    0,    0,
    0,    0, 2781,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2786,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2541,    0,    0,    0,    0,
    0, 2626,  568,    0,  109,    0,    0,    0,    0,    0,
    0,    0,  117,  199,    0,    0,  151, 2777,    0,    0,
    0,    0,  291,  343, 3685,    0,    0,    0, 2779,    0,
    0,    0,    0,    0,  111,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  589,    0, 1589,  829,  957,    0,    0,
 2449, 1487,    0,    0,  161,    0,  767,  145,  306, 1319,
    0,    0, 2805,    0,    0,    0,    0, 2793,    0,    0,
    0, 2558,    0, 2799,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2626,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  531,    0,    0,    0,  121, 2814,
 1309,    0,    0, 2779,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2449,  291,    0,    0,
    0,    0,    0,    0, 2801,    0,    0,    0,    0,    0,
11871,    0,    0,    0,    0,    0,  145, 2822,    0,    0,
    0,    0,    0,    0, 2686, 2814,    0,    0,    0,    0,
    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2869,    0,    0, 2872,
    0,    0,    0,    0,    0,    0,   11,    0, 2873,    1,
  262,  -46, -588, 1688,  856, -191, -537, 2865, 2607,    0,
  -28,    0, 2615,  -96,  -40,    0,  407, -404,    7,  -62,
  -36, -220, 2617, 2173,    0,    0,    0, 1122,    0,    0,
  -52, 1800, -533,   -2, -865, 2255, 2039, -304, 1805, 2806,
 -275,    0,  -60,    0, -481,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -583,  200, 1815, -261, 1057, -913,    0,
    0,    0,    0, -336, -108, -576, 1821, -334,  -51,    0,
 -529,    0,    0, 1455, -976,    0, 1466,    0,    0, 2582,
 -608,    9,    0,    0,    0,    0, 2564,    0, 2340, 2588,
 -230, 1849, -672,    0,    0,    0, -716,    0,    0,    0,
    0,    0,    0, 1911,    0, 2618, 2176, 1856, 2595, 2597,
    0, 1289,    0, -482, 2151,  -35, -640, -455,    0,    0,
    0, -929, -917, -916, -267, -914,    0, 1520, -998,    0,
 1216,-1210, 1530,    0, 2384,    0,    0,    0, 1224, 1226,
 1187,   13, -256, -747, -882,    0,    0, 2180,    0, 1715,
    0,    0, -814,    0,    0,    0,    0, -170,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1253,    0, 1308, -243,    0,    0,  762,    0, -386,  735,
 1141, 1298,    0, 1202,    0, 1976, 1755, 1577,    0, -567,
 -872, -918,    0, -495, -369, -565,    0, -891, 1294,  324,
 -431, -475, -753, -938, -678,    0,    0,   -3,    0,    0,
    0,    0,    0, 2637,    0,    0,    0,    0, 2653, 2425,
    0,    0,    0,    0,    0,   -9, 2668,    0,    0,    0,
    0,  -10,    0,    0,    0, -342, 2155, -340, 1718, -458,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2377,
    0,    0, 2177,    0,    0,    0,    0,    0,    0,   46,
    0, 2989,    0,    0,    0,    0,    0, 2707,    0,    0,
    0,    0, 2515,    0, 2514,    0,    0,    0, 2279,    0,
    0,    0, -554,    0,   39,    0, 2267,    0,    0,    0,
    0,    0,    0, -615, -887,-1449,  528, -164, 1408,    0,
 2878, -116, 1429,    0, 2303, -625, 2296,    0, -855, 2052,
    0,    0, -129, -241,    0, 1397,-1133,    0, -903, -710,
 -646, 1512,    0, 1244,    0, 1302,    0, 1207,    0,    0,
    0,    0, 2547,    0,    0,    0, -397, 2349, 2101,-1027,
    0,    0,    0, -659,  160,  214,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2504,
 1083, 1938,    0, -543,    0,    0,    0, -902,    0, 1837,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -232, 1904,    0,    0,
};
#define YYTABLESIZE 12707
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  259,  154,  204,
   37,  208,  437,  748,  572,  192,  609,  486,  610,  308,
  577,  279,  282,  784,  243,  544,  217,  209,  195,  278,
  281,  252,  686,  734,  754,  247,  229, 1423,  895,  943,
  222,  461, 1007,  186,  188,  191,  194,  148,  199,  248,
  461, 1245, 1172,  210,  188,  215,  254,  194,  253,  148,
  199,  772,  228,  127,  775,  219,  542,  917,  925,  711,
  786,  239,  149,  148,  241,  188,  224,  239,  215,  191,
  194,  250,  148,  199,  228,  127,  991,  194,  249,  313,
 1194, 1234,  534,  265, 1076,  267,  246,  963, 1234,  251,
  639,  266,  857, 1235, 1236,  698, 1238,  623,  256,  309,
 1235, 1236,  996, 1238,  482, 1246,  358, 1202, 1239,  210,
  148,  210,  311,  353,  993, 1239, 1254,  749,  966,  371,
  629,  595,  628,  215,  333,  507, 1050, 1292, 1024,  307,
  194,  148,  228,  708, 1051, 1275,  154, 1031,   77,  256,
  684,  289,  256,  848,  940,  567,  747, 1084,  621,  360,
  696,  684, 1377, 1700,  698,  462,  732,  256,  749,  328,
  519,  749, 1000,  519, 1099,  358,  508,  437,  567,  967,
  276,  263,  353,  280,  700,  700,  749, 1270,  760,  629,
  595,  628, 1056,  762,  763,  764,  765,  318,  359,  621,
 1071,  315,  621,  277, 1293,  326,  277, 1496,  277,  684,
  609,   89,  610,  759,  567, 1183,  513,  621,  654,  696,
  454,   77,  472,  835,  462,  743,  695,  464,  817,  364,
  980,  457,  496,   77,  897,   77,  493,  567,  706,  277,
  457, 1257,  359,  700,  577, 1766,  210, 1421, 1603,  891,
  148,  519,  463,  127,  751,  317,  827,  359, 1463,  148,
 1516,  463, 1458, 1758,  326,  500,  891,  695,  740,  321,
  506,   85, 1008,  133,   85, 1381,  263, 1189,  268, 1259,
  191,  718, 1143, 1144,  148,  695,  464,  365, 1780, 1781,
  737,  283,  519,  543,  792,  519, 1255, 1438, 1173,  573,
  546,  575,  209,   60,  559,  700, 1410,  561,  818,  713,
  519,  125,  519, 1169,  129,  627, 1260,  322,  631,  210,
  619,  511, 1038, 1287,  828, 1530,  210,  740,  210,  771,
  210,  696,  517,  564,  284, 1824,  714,  508, 1195,  147,
  773,  629,  696,  829, 1234, 1009,  700, 1175,  671, 1433,
 1389,  194,  199, 1217,  627,  519, 1235, 1236, 1415, 1238,
  148,  738,  191, 1674,  700,  195, 1057, 1186,  621,  749,
  125, 1239,   79,  892, 1455,  470,  381, 1836,  283, 1198,
 1200,  130, 1258,  696,  366, 1604, 1767, 1825,  498,  671,
  892,  517,  671, 1190,  379, 1097,  486,  272, 1253,  734,
 1256,  696, 1028,  519, 1452,  631,  240,  671,  375,   83,
  689, 1230,   14,  627, 1448,  590, 1764,  705, 1187, 1077,
 1421,  893, 1161,  462,  195,  268,  702,  380,  719,  323,
 1199,  506,  705,  698,  470,  381,  698,  710,  519,  924,
  734,  716,  758,  734,  358,  148,  457,  358,  194,  463,
 1464,  684,  283,  379,  136,  939,  148,  271,  734,  486,
  148, 1723,  812,  194,  631,  148, 1758,  375,  154, 1397,
  629,  462,  628,  595,  590,  761,  621, 1629, 1503,   89,
  729,  684,  486, 1793, 1495,  464,  380,  461, 1310, 1311,
 1518, 1312, 1313, 1506,  654,  896, 1531, 1298,  621,  742,
 1516, 1566,  700, 1567, 1579, 1019, 1404,  684, 1021,  519,
  749, 1083, 1593,  136, 1524, 1768,  684,  788, 1378, 1098,
  328,  814, 1027,  721,  700,  519,  359,  891,  970,  359,
  360,  700, 1451,  464,  229, 1018,  595, 1509, 1594,  595,
  519,  682,  781,  210,  695,  698,  891, 1086,  781, 1045,
 1015,  519,  857,  707, 1801,  595,  358,  519,  971,  698,
  519,   76,  778, 1193,  377, 1319,  695,  749,  358,  813,
  358,  700,  842,  843,  360,  229,  992,  353,  229,  275,
   79,  595, 1069,  462,  629, 1091,  628,  567,  256,  360,
  684,  567,  209,  229, 1286,  519, 1436, 1609,  275, 1605,
  696,  275,  697, 1015,  684,  275,  731,  637,  749,  256,
 1640,  749, 1385,  695,  696,  326, 1642,  508,  210,  637,
 1409,  637,  695,  984,  700, 1182,  749,  859,  749,  256,
  834,  839,  256,   79,   76,  740,  696,  858,  359,  470,
  381,   85, 1425,  697, 1450,  464,   76,  256,   76, 1753,
  359,  892,  359, 1013, 1014,  671,  671,  215,  379,  125,
  621,  697,  519,  734,  944, 1677,  695,  375,  262,  519,
  892, 1225,  375,  256,  771,  899, 1763,  696,  375,  705,
  695,  933, 1681, 1519, 1586,  773,  696,  519, 1528,  210,
  210,  380,  700,  463,  627,  696,  486,  832,  463,  517,
  667, 1470, 1592, 1678,  386, 1003, 1003,  463,  749, 1679,
  194,  506,  500,  195,  990,  194,  209,  519,  729,  734,
  454,  519,  387, 1745, 1176,  891, 1831, 1179, 1459,  696,
 1518,  148,  367,  148,  136, 1288,  210, 1665,  696,  395,
  546,  667,  210,  596,  667,  631,  210, 1520,  334,  383,
  382,  615,  241, 1237,  891,   97, 1507,  590,  671,  667,
 1237,  131, 1017,  386, 1244, 1729,  210, 1424, 1037, 1039,
 1234, 1100,  470,  381, 1736, 1596, 1252,  385, 1370, 1086,
 1372,  387, 1235, 1236, 1365, 1238, 1521,  384, 1651,  606,
 1731,  379,  590,  241,  210,  210, 1274, 1239,  395,  671,
 1036,  865,  596, 1737,  135,  375, 1044,  698,  383,  382,
  615, 1070,  241, 1437,  229, 1491, 1773, 1387, 1054, 1447,
  590, 1081,  156,  590,  380,  335, 1814, 1785,  252, 1529,
  148,  508, 1066, 1067, 1068,  789,  385,  698,  625,  590,
  148, 1775, 1345,  735,  420,  749,  384,  749,  606,  892,
  157,  734, 1093,  973,  749,  866,  200,   79,  360,  734,
  873,  360,  295,  135, 1786,  590,  256,  134,  256,  252,
  891, 1756,  252,  682,  241,  256, 1595, 1787,  892,  625,
  920,  921,  625,  831, 1807,  591, 1180,  252,  734,  734,
 1600,  734,   14, 1164, 1165, 1101,  698,  625,  734,  734,
  734,  734,  734,  216,  734, 1177, 1646,  734,  734,  734,
  734,  734,  241, 1181,  806,  241,  790,  782, 1214,  614,
  697,  143,   76, 1519,  122,  506,  486, 1363, 1358,  437,
  697, 1360,  565,  697,  241,  463,  194,  199,  463,  228,
  127,  241,  697,  566,  591,  749,  463,  984, 1799, 1647,
  749,  807,  687, 1178,  696,  760,  253,  791,  463,  463,
  614,  375,  437,  194,  158, 1682,  256,  749,  386, 1215,
  360, 1207,  375,  943,  749,  241,  696,  137,  614, 1370,
 1821, 1822,  360,  344,  360, 1208,  387, 1520,  256,  697,
 1034,  273,  942,  687,  892,  256,  687,  253,  697, 1432,
  253, 1730,  609,  395,  610, 1833, 1237, 1628,  749, 1209,
 1630,  687, 1648,  383,  382,  253, 1840,  159,  667, 1841,
  684,  667, 1695,  696,  922,  486,  144, 1096,  667,  256,
 1454,  667,  696,  145,  234,  667,  137, 1166,  209, 1265,
  667,  385,  697,  667, 1709,  454,   79, 1712,  496, 1643,
 1266,  384,  942, 1265,  701,  808,  697,  700,  626,  667,
  345,  684, 1281, 1210,  210, 1527, 1647,  283,  749,  615,
 1802,  210, 1375, 1375, 1379, 1388,  696,  866, 1635,  684,
 1301,  866,  135,  667,  135,  596, 1305,  667,  599,  256,
  695,  615,  891,  105,  698,  696,  667,  698,  236,  122,
  122,  386, 1687, 1636,  695,  693,  252,  694,  252,  696,
  901, 1803, 1670,  667,  486,  252, 1406,   77, 1408,  387,
  596, 1750,  106,  107,  569,  108, 1497, 1738, 1739, 1648,
 1500,  606,  109,  110,  111,  112,  113,  540,  114,  599,
  667,  115,  116,  117,  118,  119,  383,  382,  596,  497,
  541,  596,  667,  698,  244,  698,  625, 1357, 1804,  792,
  667, 1637,  698,  684,  565,  698,  606,  596,  698, 1777,
 1003,  202,  203,  463,  385,  463, 1696,  698,  625, 1453,
 1716, 1276,  135,  569,  384,   79,   79,  275,  754,  667,
  667,  463,  667,  596,  606,  307,  194,  606, 1601,  667,
  667,  667,  667,  667,  498,  667,  252,  255,  667,  667,
  667,  667,  667,  606,  983,  754,  892, 1835,  256,  231,
  683,  499,  684,  565,  698, 1809, 1810,  591,  252, 1717,
 1277, 1247, 1248,  698,  253,  252,  253,  614,  264,  606,
 1718, 1779, 1827,  253,  695,  693,  105,  694,  574,  696,
 1462, 1276, 1734,  326, 1466,  367,  232,  137, 1693,  614,
 1694,  683,  591,   79, 1513,  962, 1627, 1435,  571,  252,
  687,  260, 1399,  105,  115,  106,  107,  698,  108,  683,
 1299, 1842, 1400,  687,  508,  109,  110,  111,  112,  113,
  591,  114,  687,  591,  115,  116,  117,  118,  119, 1734,
 1280,  327,  106,  107,  426,  108,  941,  574,  615,  591,
  423,  115,  109,  110,  111,  112,  113, 1735,  114,  494,
  698,  115,  116,  117,  118,  119,  150,  571,  691,  252,
  657, 1680,  480,  692,  253,  591,  268, 1571,  962,  687,
  517,  485,  691, 1734, 1719,  698,  115,  692,  687,  615,
  338,  684, 1003,  339, 1112,  361,  253,  375,  151, 1572,
  328, 1532,  573,  253,  486,  380, 1573,  615,  494,  261,
  560,  657,  516,  152,  463, 1734,  269,  209,  506,  698,
 1113,  517,  402,  194,  517,  105,  698,  521, 1375,  657,
 1094,  362,  687, 1577, 1095, 1121,  522,  253,  340,  517,
 1574,  517, 1749,  210,  698,  486,  687,  684,  486, 1808,
  105, 1122,  148,  328,  106,  107,  684,  108, 1641,  328,
  403,  573,  508,  486,  109,  110,  111,  112,  113,  560,
  114,  329, 1237,  115,  116,  117,  118,  119,  399,  106,
  107,  402,  108,  695,  693,  283,  694,  160,  696,  109,
  110,  111,  112,  113,  270,  114, 1123,  253,  115,  116,
  117,  118,  119,  437, 1752,  330,   43,   51,  274,  677,
  105,  283, 1645,  682,  683,  685,  987,  161,   41,  403,
 1675,  684,  691,  530,  139, 1124,  777,  692,  903,  776,
   99, 1626,  927,  988,  684,  697,  151,  399,   24,  106,
  107,   79,  108,  684, 1513, 1791,  160,  745,  290,  109,
  110,  111,  112,  113,  905,  114,  506,  350,  115,  116,
  117,  118,  119,  188, 1676,   43,   51,  101,   80,   81,
   76, 1513,  105,  531,   47,  722,  161,   41,  683,  724,
  306,   49, 1792,  472, 1673, 1815, 1652, 1644, 1818, 1689,
  684,  683, 1125, 1690,  351,  151,  352,   24,  933,  684,
  683,  106,  107,  148,  108,  316,  745,  532,  191,  319,
 1830,  109,  110,  111,  112,  113, 1724,  114, 1513,  353,
  115,  116,  117,  118,  119,  533,  149,  148,  250, 1369,
  336, 1371,  641,   47,  641,  641,  102,  641,  517,  660,
   49,  347,  472,  684,  354, 1705,  103,  683,  496,  216,
  641,  873,  349,  660,  873,  641,  683,  517,  517, 1513,
  517,  660,  933,  210,  517,  358,  127,  517, 1513,  250,
 1758,  104,  250,  508,  370,  368,  194,  660,  695,  693,
  517,  694,  486,  696,  486, 1757,  517,  250,  657,  496,
  216,  486,  496,  216,  370,  711, 1759,   25,  657, 1513,
  683,  657, 1513,  873,  486,  508,  508,  496,  216,  456,
  657,  149,  148,  698,  683,   48,  698,  695,  693,   50,
  694,  691,  696,  372, 1513,  127,  692,  483, 1740,  148,
  508,  660,  711,  370,  373,  491,   45,  695,  693,  983,
  694,  508,  696,  872,  508,  711,  691,  188, 1761,  128,
  116,  692,  376,  456,  983,   79,   25,  657,  517, 1762,
  485, 1160,  149,  148,  900, 1207,  657,  506,  456, 1740,
  148,  916,  698, 1813,   48,   46,  923,  423,   50, 1208,
  517,  698,  486,  458,   98,  216,  930,  517,  934,  935,
  936,  937,  938,  423,  930,   45,  517, 1758,  517,  506,
  506,  202,  203, 1209,  486,  149,  148,  377,  128,  116,
  657,  486, 1817, 1740,  148,  151,  517,  191,  988,  138,
  139,  517,  657,  485,  506,   99,  799, 1631,  800,  962,
  519,  230,  961,  988,   46,  506,  151,  100,  506,  466,
 1075,  467,  191, 1074,  216,  486,  485,  361,  471, 1093,
  517,  516,  641,  152,  470, 1082,  473, 1210,  853,  641,
  475,  361,  101,  981,  476,  151,  492,  191,  641,  361,
 1174,  519,  230, 1173,  519,  230,  494,  695,  693,  477,
  694,  641,  696,  495,  151,  361,  519,  641,  641,  519,
  230,  695,  693,  151,  694,  641,  696,  874,  504,  151,
  874,  231,  529,  958,  981,  981,  981,  981,  981,  981,
  981, 1218,  152,  514, 1074,  641,  691,  151,  641,  123,
  123,  692,  981,  981,  981,  981,  496,  519,  496,   14,
  519,  102,  496, 1224, 1268,  496, 1223, 1267,  232,  361,
 1273,  103,  520, 1272,  641,  519,  641,  641,  496,  874,
 1383, 1046, 1047, 1382,  981,  691,  981,  537,  641,  550,
  692,  557,  641,  723,  695,  693,  104,  694, 1472,  696,
  560, 1173, 1102, 1103, 1107,  691,  570, 1115, 1117, 1119,
  692, 1127, 1130, 1133, 1135, 1138, 1141,  574,  105, 1145,
 1146,  624, 1148, 1502, 1150,  725,  697, 1153, 1154, 1155,
 1156, 1157, 1158, 1116,  628, 1120, 1323, 1128, 1131,  630,
 1136, 1139, 1142,  632, 1162,  663,  423,  106,  107,  641,
  108,  664,  665,  666,  514,  641,  496,  109,  110,  111,
  112,  113,  667,  114,  668,  641,  115,  116,  117,  118,
  119,  926,  695,  693,  669,  694,  670,  696,  496,  641,
  641,  641,  641,  641,  641,  496,  671,  673, 1508,  195,
  485,  697,  672,  641, 1839,  514,  674, 1758,  514,  675,
  680,  681,  641,  641,  641,  641,  641,  641,  641,  641,
  641,  641,  641,  514,  641,  514,  690,  641,  641,  496,
  641,  641,  641,  641,  641,  641,  641,  641, 1231,  697,
  641,  641,  641,  641,  641,  698,  291,  699,  519,  241,
  519,  230,  703,  717,  519,  691,  708,  519,  195,  720,
  692,  727,  725,  741,  745,  981,  746,  981,  981,  691,
  519,  894,  152,  898,  692,  981,  750,  902,  904,  906,
  752,  292,  753,  981,  756,  755, 1479,  766,  981,  695,
  693,  981,  694,  152,  696,  972,  695,  693,  521,  694,
  767,  696,  769,  803,  519,  981,  519,  770, 1511,  293,
  981,  981,  294,  519,  981,  695,  693, 1170,  694,  804,
  696,  981,  152,  981,  981,  658,  519,  981,  295,  779,
  805,  981,  529,  810,  981,  275,  981,  815,  981,  819,
  822,  152,  691,  981,  981,  930,  981,  692,  519,  241,
  152,  821,  820,  824,  296,  825,  152, 1326, 1327,  830,
 1329, 1331,  981,  981,  836,  981,  658,  837, 1336,  658,
  519,  241,  529,  529,  152,  840,  529,  519,  241,  981,
  659,  981,  981,  981,  658,  850,  981, 1349,  841,  981,
  981,  529,  852,  529,  853, 1356,  862,  423,  861,  981,
  423,  863, 1162,  867,  519,  928, 1364,  423, 1366, 1367,
  918,  519,  241,  981,  956, 1207,  981,  396,  397,  398,
  691,  659,  514,  529,  659,  692,  519,  957,  959, 1208,
  485,  964,  981,  519,  968,  979,  969,  994,  981,  659,
  423,  514,  514,  981,  514,  406,  407,  985,  514,  997,
 1015,  514, 1001, 1209,  981,  981, 1020,  981,  831, 1025,
 1026, 1028,  782,  981,  514, 1035, 1041,  519, 1042, 1032,
  514, 1033, 1043, 1065, 1049, 1052,  962, 1826,  195, 1058,
 1417, 1063, 1072, 1152,  695,  693, 1077,  694,  384,  696,
 1089,  713,  981,  981, 1109,  981, 1090, 1147, 1149,  195,
 1151,  691,  981,  981,  981,  981,  981, 1210,  981, 1314,
 1159,  981,  981,  981,  981,  981,  386,  962,  962,  962,
  962,  962,  974,  962, 1171, 1185, 1188,  691,  195,  485,
 1183, 1201,  692, 1191,  691,  962,  962,  962,  962,  692,
 1204, 1206,  514,  700, 1212,  388, 1221,  195, 1511, 1226,
 1467, 1229, 1240,  691, 1249,  195,  195, 1251,  692, 1475,
 1261,  420,  195, 1271,  514, 1269, 1480,  962, 1278,  962,
 1282,  514, 1283, 1284, 1285, 1715, 1296, 1294, 1083, 1303,
  195, 1306,  514, 1307, 1308,   52,  822, 1309, 1315, 1316,
  529, 1320,  195, 1321,  195,  423,   53, 1322, 1324,  423,
  514, 1325, 1714,   54, 1332,  514,  529, 1333, 1334,  529,
  529, 1335,  529, 1337, 1338, 1339,  529,  195,  485,  529,
  393, 1340, 1715,  222, 1341, 1342, 1359,  822,   55,  891,
  822, 1362,  529, 1368,  514, 1384,  395, 1386,  529, 1390,
 1391,  529,  195,  658, 1392,  822, 1393, 1394, 1395, 1714,
  396,  397,  398,  658,  202,  203,  658,  695,  693, 1402,
  694, 1405,  696, 1715,  222,  658, 1407,  222,  682, 1412,
 1414,  552, 1798,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  222, 1422, 1426, 1427, 1434, 1428,  411,  412,
 1714, 1429, 1439,   56, 1430, 1431, 1449,  416,  659, 1797,
 1456, 1460, 1317, 1715, 1461, 1465, 1715, 1471,  659, 1492,
  529,  659,  658, 1498, 1614, 1615, 1499, 1522, 1504, 1568,
  659,  658,  691,  682, 1505,   57, 1578,  692, 1715,   58,
 1714, 1580,  529, 1714, 1581,  423, 1582, 1583,  962,  529,
  962,  962, 1584,  529, 1585, 1587, 1588, 1589,  962,  519,
  529, 1597, 1602, 1599, 1606, 1714,  962, 1607, 1608, 1610,
 1611,  962, 1612, 1613,  962,  658, 1616,  659,  529, 1617,
 1618, 1619, 1620,  529, 1621, 1654,  659,  658,  962, 1634,
  962, 1622,  985,  962,  962, 1623, 1624,  962, 1625, 1632,
  519, 1650, 1633,  519,  962, 1653,  962,  962,  529,  130,
  962, 1655,  529, 1656,  962,  250, 1661,  962,  519,  962,
  519,  962, 1663, 1657, 1669, 1658,  962,  962, 1659,  962,
  659, 1660, 1671,  985,  985,  985,  985,  985,  985,  985,
 1662, 1668,  659, 1672, 1692,  962,  962, 1688,  962, 1698,
 1699,  985,  985,  985,  985, 1691,  250, 1708,  822,  250,
 1725, 1664,  962, 1733,  962,  962,  962, 1746, 1755,  962,
 1769, 1771,  962,  962,  250,  363, 1774, 1776, 1778, 1783,
 1784, 1789,  962,  985, 1796,  985, 1795, 1478, 1766, 1767,
  158, 1704, 1812, 1816, 1820, 1706,  962, 1819, 1823,  962,
 1343,  695,  693, 1838,  694,  691,  696, 1758, 1837, 1834,
  692,  222,    1,  222,  822,  962,  363,  822,   29,  363,
  222,  962,  202,  621,  822,  749,  962,  822,   31,  367,
  270,  822,  148,  517,  363,  650,  822,  962,  962,  822,
  962, 1344,  695,  693,  268,  694,  962,  696, 1347,  695,
  693,  423,  694, 1751,  696,  822, 1754,   81,  671,  453,
 1348,  695,  693,  945,  694,  212,  696,  517, 1350,  695,
  693,  275,  694,  105,  696,  962,  962,  395,  962,  822,
  192,  682,  822,  822,  944,  962,  962,  962,  962,  962,
  517,  962,  822,  680,  962,  962,  962,  962,  962,  676,
  678,  193,  106,  107,  274,  108,  288,  368,  706,  822,
  286,  222,  109,  110,  111,  112,  113,  519,  114,  705,
  176,  115,  116,  117,  118,  119, 1351,  695,  693,  222,
  694,  822,  696,  222,  323,  707,  822,  519,  287,  519,
  222,  331,   62,  519,  615,  990,  519,  322,  822,  329,
  822,  340,  716,   63,  985,  186,  985,  985,   27,  519,
   64,   30,   39, 1373,  985,  519,  135,  474,  465,  464,
  958,  674,  985,   65,  222, 1304,  851,  985, 1302,  176,
  985, 1088,  245, 1290, 1590, 1300,  990,  990,  990,  990,
  990,  990,  990,  250,  985,  250, 1591,  539,  525,  985,
  985, 1279,  250,  985,  990,  990,  990,  990,  816,  583,
  985, 1228,  985,  985,  186,  674,  985,  995,  512, 1291,
  985, 1701,  554,  985,  555,  985, 1030,  985, 1570, 1765,
  674,  798,  985,  985,   66,  985,  990,  519,  990,  691,
 1576, 1772, 1770, 1806,  692, 1023, 1416, 1748, 1707, 1832,
   67,  985,  985, 1722,  985, 1800, 1352,  695,  693,  519,
  694, 1219,  696, 1398, 1525, 1732,  519,   68,  985,  562,
  985,  985,  985,  545,  802,  985, 1064,  519,  985,  985,
  691,  536,   69, 1446,  838,  692,   70,  691,  985,   71,
 1055,  137,  692,  250,  505,  519,  363,   72,  736,  691,
  519,  740,  985,  363,  692,  985,  363,  691,  288,  986,
  363,  999,  692,  982, 1203,  250,   73, 1811,  363,  981,
 1697,  985,  250, 1638, 1353,  695,  693,  985,  694,  519,
  696, 1782,  985, 1843,  363,  907,  908,  909,  910,  911,
  912,  913,  914,  985,  985,  749,  985, 1354,  695,  693,
 1328,  694,  985,  696,  965, 1197,  250, 1411,  363, 1361,
    0,    0, 1355,  695,  693,  691,  694,    0,  696,    0,
  692,    0,    0,  187,  695,  693, 1710,  694,  176,  696,
    0,  985,  985,    0,  985,    0,    0,    0,  363,    0,
    0,  985,  985,  985,  985,  985,    0,  985,  986,  176,
  985,  985,  985,  985,  985,    0,    0,  990,    0,  990,
  990,    0,    0,  186,    0,  363,    0,  990,    0,    0,
  176,    0,    0,    0,    0,  990,  176,  363,    0,  363,
  990,    0,  187,  990,  186,    0,    0,    0,    0,  986,
  986,  986,  986,  986,  986,  986,    0,  990,    0,  176,
    0,    0,  990,  990,    0,  186,  990,  986,  986,  986,
  986,  186,    0,  990,    0,  990,  990,    0,  176,  990,
    0,    0,    0,  990,    0,    0,  990,  176,  990,    0,
  990,    0,    0,  176,  186,  990,  990,    0,  990,  986,
    0,  986,  674,    0,    0,  691,    0,    0,    0,  674,
  692,  176,  674,  186,  990,  990,  674,  990,  665, 1493,
  695,  693,  186,  694,  674,  696,    0,    0,  186,    0,
    0,  990,    0,  990,  990,  990,    0,    0,  990,    0,
  674,  990,  990,    0,    0,    0,  186,    0,  176,    0,
    0,  990,    0,    0, 1494,  695,  693,    0,  694,  665,
  696,    0,  665,    0,  674,  990,    0,    0,  990,    0,
 1501,  695,  693,  691,  694,    0,  696,  665,  692,    0,
    0,    0,    0,  186,  990,    0,  695,  693,    0,  694,
  990,  696,    0,    0,  674,  990,  691,    0,    0,  674,
    0,  692,    0, 1747,    0,    0,  990,  990,    0,  990,
    0,  691,    0,    0,    0,  990,  692,    0,    0,    0,
    0,  674,  691,    0,    0,    0,    0,  692,    0,    0,
    0,    0,    0,  674,    0,  674,    0,    0,    0,    0,
    0,  187,    0,    0,  990,  990,    0,  990,    0,    0,
    0,    0,    0,    0,  990,  990,  990,  990,  990,    0,
  990,  843,  187,  990,  990,  990,  990,  990,    0,    0,
  986,    0,  986,  986,    0,    0,    0,    0,    0,    0,
  986,    0,    0,  187,    0,    0,    0,    0,  986,  187,
    0,    0,    0,  986,    0,    0,  986,    0,    0,    0,
    0,    0,  843,  843,  843,  843,  843,    0,  843,    0,
  986,    0,  187,    0,    0,  986,  986,    0,    0,  986,
  843,  843,  843,  843,    0,    0,  986,    0,  986,  986,
  195,  187,  986,    0,  389,    0,  986,    0,    0,  986,
  187,  986,    0,  986,    0,    0,  187,    0,  986,  986,
  530,  986,    0,    0,  843,    0,    0,    0,  691,    0,
    0,    0,    0,  692,  187,    0,  180,  986,  986,    0,
  986,  576,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  986,    0,  986,  986,  986,  195,
    0,  986,    0,  691,  986,  986,    0,    0,  692,    0,
  531,  187,    0,    0,  986,    0,    0,  180,    0,  691,
  180,    0,    0,    0,  692,   84,    0,    0,  986,  521,
    0,  986,   85,    0,  691,  180,   86,   87,  522,  692,
    0,    0,    0,    0,  532,   88,  665,  986,   76,  665,
  399,    0,    0,  986,    0,    0,  665,   89,  986,  665,
    0,  400,  533,  665,    0,    0,    0,    0,  665,  986,
  986,  665,  986,    0,    0,    0,    0,    0,  986,  106,
  410,    0,  108,    0,    0,    0,    0,  665,  413,  414,
  415,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,  855,    0,    0,    0,  986,  986,    0,
  986,  665,    0,    0,    0,  665,    0,  986,  986,  986,
  986,  986,   90,  986,  665,    0,  986,  986,  986,  986,
  986,    0,    0,  843,    0,  843,  843,    0,    0,   91,
    0,  665,    0,  843,  855,  855,  855,  855,  855,    0,
  855,  843,    0,    0,   92,    0,  843,    0,   93,  843,
    0,    0,  855,  855,  855,  855,    0,    0,  665,   94,
    0,    0,    0,  843,    0,    0,    0,    0,  843,  843,
  665,    0,  843,    0,    0,    0,    0,    0,   95,  843,
    0,  843,  843,    0,  662,  843,  855,    0,  195,  843,
    0,    0,  843,    0,  843,    0,  843,    0,    0,    0,
    0,  843,  843,    0,  843,    0,    0,    0,    0,  195,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  843,  843,    0,  843,  180,  662,    0,    0,  662,    0,
  195,    0,    0,    0,    0,    0,  195,  843,    0,  843,
  843,  843,    0,  662,  843,  180,    0,  843,  843,  728,
    0,    0,    0,    0,    0,    0,    0,  843,    0,  195,
    0,    0,    0,    0,    0,    0,  180,    0,    0,    0,
    0,  843,  180,    0,  843,    0,    0,    0,  195,    0,
    0,    0,    0,    0,    0,    0,  195,  195,    0,    0,
  843,    0,    0,  195,    0,  180,  843,    0,    0,    0,
    0,  843,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  195,  843,  843,  180,  843,    0,    0, 1468, 1469,
    0,  843,    0,  180, 1473, 1474,    0, 1476, 1477,  180,
    0,    0,    0,    0,    0, 1481, 1482, 1483, 1484,    0,
 1485, 1486, 1487, 1488, 1489, 1490,  858,  180,  195,    0,
  843,  843,    0,  843,    0,  855,    0,  855,  855,  195,
  843,  843,  843,  843,  843,  855,  843,    0,    0,  843,
  843,  843,  843,  843,    0,    0,    0,    0,  855,    0,
    0,  855,    0,    0,  180,    0,    0,  858,  858,  858,
  858,  858,    0,  858,    0,  855,    0,    0,    0,    0,
  855,  855,    0,    0,  855,  858,  858,  858,  858,    0,
    0,  855,    0,  855,  855,    0,    0,  855,    0,    0,
    0,  855,    0,    0,  855,    0,  855,    0,  855,    0,
    0,    0,    0,  855,  855,    0,  855,    0,    0,  858,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,  855,  855,    0,  855,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  855,
    0,  855,  855,  855,  106,  107,  855,  108,    0,  855,
  855,    0,    0,    0,  109,  110,  111,  112,  113,  855,
  114,    0,    0,  115,  116,  117,  118,  119,   98,    0,
    0,    0,  662,  855,    0,  662,  855,    0,    0,    0,
    0,    0,  662,    0,    0,  662,    0,    0,    0,  662,
    0,    0,  855,    0,  662,    0,    0,  662,  855,    0,
    0,    0,    0,  855,    0,    0,    0,    0,    0,   99,
    0,    0,    0,  662,  855,  855,    0,  855,    0,    0,
    0,  100,    0,  855,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  132,    0,  662,    0,    0,
    0,  662,    0,    0,    0,    0,  101,    0,  859,    0,
  662,    0,  855,  855,    0,  855,    0,    0,    0,    0,
    0,    0,  855,  855,  855,  855,  855,  662,  855,    0,
    0,  855,  855,  855,  855,  855,    0,    0,  858,    0,
  858,  858,    0,    0,    0,    0,    0,    0,  858,  859,
  859,  859,  859,  859,  662,  859,    0,    0,    0,    0,
    0,    0,    0,    0,  858,    0,  662,  859,  859,  859,
  859,    0,    0,    0,    0,  102,    0,    0,  858,    0,
    0,    0,    0,  858,  858,  103,    0,  858,    0,    0,
    0,    0,    0,    0,  858,    0,  858,  858,    0,    0,
  858,  859,    0,    0,  858,    0,    0,  858,    0,  858,
  104,  858,    0,    0,    0,    0,  858,  858,    0,  858,
    0,    0,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  858,  858,    0,  858,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  858,    0,  858,  858,  858,  106,  107,  858,
  108,    0,  858,  858,    0,    0,    0,  109,  110,  111,
  112,  113,  858,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  858,  552,    0,  858,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  858,    0,    0,    0,    0,
    0,  858,    0,    0,    0,    0,  858,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  858,  858,    0,
  858,    0,    0,    0,    0,    0,  858,    0,    0,  926,
  695,  693,    0,  694,    0,  696,    0,    0,    0,    0,
    0,  558,    0,    0,    0,    0,    0,    0,  889,  888,
  890,  853,    0,    0,    0,  858,  858,    0,  858,    0,
  859,    0,  859,  859,    0,  858,  858,  858,  858,  858,
  859,  858,    0,    0,  858,  858,  858,  858,  858,    0,
    0,    0,    0,    0,    0,    0,  859,    0,    0,    0,
    0,    0,  853,    0,  853,  853,  853,    0,    0,    0,
  859,    0,    0,    0,    0,  859,  859,    0,    0,  859,
  853,  853,  853,  853,    0,    0,  859,    0,  859,  859,
    0,    0,  859,    0,    0,    0,  859,    0,    0,  859,
    0,  859,    0,  859,    0,    0,    0,    0,  859,  859,
    0,  859,    0,    0,  853,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,    0,    0,  859,  859,    0,
  859,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  859,    0,  859,  859,  859,  106,
  107,  859,  108,    0,  859,  859,    0,    0,    0,  109,
  110,  111,  112,  113,  859,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,  859,    0,
    0,  859,    0,    0,  105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   98,  859,    0,    0,
    0,    0,    0,  859,    0,    0,    0,    0,  859,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,  859,
  859,    0,  859,  109,  110,  111,  112,  113,  859,  114,
  876,    0,  115,  116,  117,  118,  119,   99,  691,    0,
    0,    0,    0,  692,    0,    0,  877,    0,    0,  100,
    0,    0,    0,  852,    0,    0,    0,  859,  859,    0,
  859,    0,    0,    0,    0,    0,    0,  859,  859,  859,
  859,  859,    0,  859,  101,    0,  859,  859,  859,  859,
  859,    0,  878,  853,    0,  853,  853,    0,    0,    0,
    0,  879,    0,  853,  852,    0,  852,  852,  852,  880,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  853,
    0,    0,  852,  852,  852,  852,    0,    0,  881,    0,
  882,    0,    0,  853,    0,    0,    0,    0,  853,  853,
    0,    0,  853,    0,  883,    0,  884,  885,  886,  853,
    0,  853,  853,  102,    0,  853,  852,    0,    0,  853,
    0,    0,  853,  103,  853,    0,  853,    0,    0,    0,
    0,  853,  853,    0,  853,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  104,    0,
  853,  853,    0,  853,    0,    0,    0,  887,    0,    0,
    0,    0,    0,    0,    0,    0,  105,  853,    0,  853,
  853,  853,    0,    0,  853,    0,    0,  853,  853,    0,
    0,    0,    0,    0,    0,    0,    0,  853,    0,    0,
    0,    0,    0,    0,    0,  106,  107,    0,  108,    0,
    0,  853,    0,    0,  853,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
  853,    0,    0,    0,    0,    0,  853,    0,    0,    0,
    0,  853,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  853,  853,    0,  853,    0,    0,    0,    0,
    0,  853,    0,    0,    0,  695,  693,    0,  694,    0,
  696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  889,  888,  890,  854,    0,    0,    0,
  853,  853,    0,  853,    0,  852,    0,  852,  852,    0,
  853,  853,  853,  853,  853,  852,  853,    0,    0,  853,
  853,  853,  853,  853,    0,    0,    0,  945,    0,    0,
    0,  852,    0,    0,    0,    0,    0,  854,    0,  854,
  854,  854,    0,    0,    0,  852,    0,    0,    0,    0,
  852,  852,    0,    0,  852,  854,  854,  854,  854,    0,
    0,  852,    0,  852,  852,    0,    0,  852,    0,    0,
    0,  852,    0,    0,  852,    0,  852,    0,  852,    0,
    0,    0,    0,  852,  852,    0,  852,    0,    0,  854,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  946,  852,  852,    0,  852,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  852,
    0,  852,  852,  852,    0,    0,  852,    0,    0,  852,
  852,    0,    0,    0,    0,    0,    0,  947,    0,  852,
    0,    0,  948,    0,    0,    0,    0,    0,  949,    0,
    0,    0,    0,  852,    0,    0,  852,    0,    0,  105,
    0,    0,    0,    0,    0,    0,    0,  950,    0,    0,
    0,    0,  852,    0,    0,    0,    0,    0,  852,    0,
    0,    0,    0,  852,    0,    0,    0,    0,  106,  107,
    0,  108,    0,    0,  852,  852,    0,  852,  109,  110,
  111,  112,  113,  852,  114,  876,    0,  115,  116,  117,
  118,  119,    0,  691,    0,    0,    0,    0,  692,    0,
    0,  877,    0,    0,    0,    0,    0,    0,  857,    0,
    0,    0,  852,  852,    0,  852,    0,    0,    0,    0,
    0,    0,  852,  852,  852,  852,  852,    0,  852,    0,
    0,  852,  852,  852,  852,  852,    0,  878,  854,    0,
  854,  854,    0,    0,    0,    0,  879,    0,  854,  857,
    0,  857,  857,  857,  880,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  854,    0,    0,  857,  857,  857,
  857,    0,    0,  881,    0,  882,    0,    0,  854,    0,
  945,    0,    0,  854,  854,    0,    0,  854,    0,  883,
    0,  884,  885,  886,  854,    0,  854,  854,    0,    0,
  854,  857,  455,    0,  854,    0,    0,  854,    0,  854,
    0,  854,    0,    0,    0,    0,  854,  854,    0,  854,
    0,    0,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  854,  854,    0,  854,    0,
    0,    0,  887,    0,    0,    0,    0,    0,    0,  524,
    0,    0,  854,    0,  854,  854,  854,  106,  107,  854,
  108,    0,  854,  854,    0,    0,    0,  109,  110,  111,
  112,  113,  854,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  854,    0,    0,  854,
  524,    0,    0,  524,    0,  948,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  854,    0,    0,  524,    0,
  524,  854,  105,    0,    0,    0,  854,    0,    0,    0,
  647,    0,    0,  417,    0,  418,    0,  854,  854,    0,
  854,    0,    0,    0,    0,    0,  854,    0,  420,    0,
  524,  106,  107,  277,  108,    0,    0,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,  854,  854,    0,  854,    0,
  857,    0,  857,  857,    0,  854,  854,  854,  854,  854,
  857,  854,    0,    0,  854,  854,  854,  854,  854,    0,
    0,    0,    0,    0,    0,    0,  857,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  857,    0,    0,    0,    0,  857,  857,  647,    0,  857,
  417,    0,  418,    0,    0,    0,  857,    0,  857,  857,
    0,    0,  857,    0,    0,  420,  857,    0,    0,  857,
  277,  857,    0,  857,    0,    0,    0,    0,  857,  857,
    0,  857,    0,    0,    0,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,    0,    0,  857,  857,    0,
  857,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  857,    0,  857,  857,  857,  106,
  107,  857,  108,    0,  857,  857,    0,    0,    0,  109,
  110,  111,  112,  113,  857,  114,    0,  524,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,  857,    0,
    0,  857,    0,  524,  459,    0,  524,  524,    0,  524,
    0,    0,    0,  524,    0,    0,  524,  857,    0,    0,
    0,  105,    0,  857,    0,    0,    0,  381,  857,  524,
    0,    0,    0,    0,    0,  524,  382,    0,  524,  857,
  857,    0,  857,    0,    0,    0,    0,    0,  857,  383,
  106,  107,    0,  108,    0,  384,  385,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,  641,    0,    0,  857,  857,    0,
  857,    0,    0,  386,    0,    0,  387,  857,  857,  857,
  857,  857,    0,  857,    0,    0,  857,  857,  857,  857,
  857,    0,    0,    0,    0,    0,    0,  524,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,    0,
    0,    0,    0,    0,  381,    0,  391,    0,    0,  524,
  392,    0,    0,  382,    0,    0,  524,    0,  868,    0,
  524,    0,    0,    0,    0,    0,  383,  524,    0,  640,
    0,    0,  384,  385,    0,  635,    0,  635,  635,    0,
  635,    0,    0,    0,    0,  524,    0,    0,    0,    0,
  524,  641,    0,  635,    0,    0,    0,  684,  635,    0,
  386,  643,    0,  387,    0,    0,    0,  393,    0,    0,
    0,    0,    0,  394,    0,  524,    0,    0,    0,  524,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,  389,  390,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,  391,    0,    0,    0,  392,    0,    0,
    0,  400,    0,    0,    0,  642,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,  563,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,  869,  870,  871,    0,    0,  643,    0,
    0,    0,  563,  563,  393,  563,    0,    0,    0,    0,
  394,    0,  563,  563,  563,  563,  563,    0,  563,    0,
  395,  563,  563,  563,  563,  563,    0,  647,    0,    0,
  417,    0,  418,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,  420,    0,    0,  400,    0,
  277,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410, 1635,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  644,  645,  646,
    0,    0,    0, 1636,    0,  635,    0,    0,    0,    0,
    0,    0,  635,  530,    0,    0,    0,    0,    0,    0,
    0,  635,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  635,    0,    0,    0,    0,    0,
  635,  635,    0,    0,    0,    0,    0,    0,  635,    0,
    0,    0,    0,  530,  530,    0,    0,  530,    0,    0,
    0, 1637,    0,    0,    0,    0,    0,    0,  635,    0,
    0,  635,  530,    0,  530,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  647,
    0,    0,  417,    0,  418,  105,    0,  635,    0,  635,
  635,    0,    0,    0,  530,    0,    0,  420,    0,    0,
    0,  635,  277,    0,    0,  635,    0,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  381,    0,    0,    0,    0,    0,
    0,    0,  635,  382,    0,    0,    0,    0,  635,    0,
    0,    0,    0,    0,    0,    0,  383,    0,  635,    0,
    0,    0,  384,  385,    0,    0,    0,    0,    0,    0,
    0,    0,  635,  635,  635,  635,  635,  635,    0,    0,
    0,  641,    0,    0,    0,    0,  635,    0,    0,    0,
  386,    0,    0,  387,    0,  635,  635,  635,  635,  635,
  635,  635,  635,  635,  635,  635,    0,  635,    0,    0,
  635,  635,    0,  635,  635,  635,  635,  635,  635,  635,
  635,  389,  390,  635,  635,  635,  635,  635,    0,    0,
    0,    0,    0,  391,  791,    0,    0,  392,    0,    0,
    0,  530,    0,    0,    0,  864,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  530,    0,    0,
  530,  530,    0,  530,    0,    0,    0,  530,    0,    0,
  530,    0,    0,    0,    0,  791,    0,    0,  791,    0,
    0,    0,    0,  530,    0,    0,  381,    0,  643,  530,
    0,    0,  530,  791,  393,  382,    0,    0,  647,    0,
  394,  417,    0,  418,    0,    0,    0,    0,  383,    0,
  395,    0,    0,    0,  384,  385,  420,    0,    0,    0,
    0,  277,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,  641,    0,    0,    0,    0,  400,    0,
    0,    0,  386,    0,    0,  387,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,  530,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  389,  390,  115,  116,  644,  645,  646,
    0,    0,    0,  530,    0,  391,    0,    0,    0,  392,
  530,    0,    0,    0,  530,  479,    0,  642,  417,    0,
  418,  530,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,  449,    0,    0,  277,  530,
    0,    0,    0,    0,  530,  449,  449,  449,  449,  449,
  449,  449,  449,  449,  449,  449,  449,  449,  449,  449,
  643,    0,  449,  449,  449,  449,  393,  449,    0,  530,
  449,  449,  394,  530,  449,  449,  449,  449,  449,  449,
  449,  449,  395,  449,  449,  449,  449,  449,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,    0,    0,  791,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,    0,  115,  116,  644,
  645,  646,    0,    0,  382,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  280,  383,    0,    0,
    0,    0,  791,  384,  385,  791,    0,    0,    0,    0,
  792,    0,  791,    0,    0,  791,    0,    0,    0,  791,
    0,    0,  641,    0,  791,  280,  280,  791,  280,    0,
    0,  386,    0,    0,  387,  280,  280,  280,  280,  280,
    0,  280,    0,  791,  280,  280,  280,  280,  280,    0,
    0,  792,    0,    0,  792,    0,    0,    0,    0,    0,
    0,    0,  389,  390,    0, 1104,    0,  791, 1105,  792,
  791,  791,  381,    0,  391,    0,    0,    0,  392,    0,
  791,  382,    0,    0,    0,    0,  864,    0,    0,    0,
    0,    0,    0,    0,  383,    0,    0,  791,    0,  479,
  384,  385,  417,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,  791,
    0,    0,  277,    0,  791,    0,    0,    0,  386,  643,
    0,  387,    0,    0,    0,  393,  791,    0,  791,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,  389,
  390,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,  391,    0,    0,    0,  392,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1106,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,  479,    0,    0,  417,    0,  418,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,    0,  420,
    0,    0,  792,    0,  277,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0, 1114,
    0,    0, 1105,    0,    0,    0,  381,    0,  792,    0,
    0,  792,    0,    0,    0,  382,    0,    0,  792,    0,
    0,  792,    0,    0,    0,  792,    0,    0,  383,    0,
  792,    0,    0,  792,  384,  385,    0,    0,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,  792,
    0,    0,    0,    0,    0,    0,    0,    0,  420,    0,
    0,    0,  386,  277,    0,  387,    0,    0,    0,    0,
    0,    0,    0,  792,    0,    0,  792,  792,    0,    0,
    0,    0,    0,    0,    0,    0,  792,    0,    0,    0,
    0,    0,    0,  389,  390,    0,    0,    0,    0,    0,
    0,    0,    0,  792,    0,  391,    0,    0,    0,  392,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  792,    0,    0,    0,    0,
  792,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  792,    0,  792,    0,    0,    0,    0,    0,
    0, 1118,    0,    0, 1105,    0,    0,    0,  381,    0,
    0,    0,    0,    0, 1106,    0,  393,  382,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
  383,    0,  395,    0,    0,    0,  384,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,  386,    0,    0,  387,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  389,  390,  115,  116,  117,
  118,  119,    0,    0,    0,    0,    0,  391,    0,    0,
 1126,  392,    0, 1105,    0,    0,    0,  381,    0,    0,
    0,    0,    0,    0,    0,    0,  382,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  383,
    0,    0,    0,    0,  479,  384,  385,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,    0,    0, 1106,  277,  393,    0,
    0,    0,    0,  386,  394,    0,  387,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,  389,  390,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,  391,    0,    0,    0,
  392,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1106,    0,  393,    0,    0,
    0,    0,    0,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,  479,    0,    0,  417,
    0,  418,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,  420,    0,    0,    0,    0,  277,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,    0, 1129,    0,    0, 1105,    0,    0,
    0,  381,    0,    0,    0,    0,    0,    0,    0,    0,
  382,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,    0,    0,    0,  384,
  385,    0,    0,    0,    0,  479,    0,    0,  417,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,    0,    0,  386,  277,    0,
  387,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  389,  390,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  391,    0,    0,    0,  392,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1134,    0,    0, 1105,
    0,    0,    0,  381,    0,    0,    0,    0,    0, 1106,
    0,  393,  382,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,  383,    0,  395,    0,    0,
    0,  384,  385,    0,    0,    0,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,  386,
    0,    0,  387,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
  389,  390,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,  391,    0,    0, 1137,  392,    0, 1105,    0,
    0,    0,  381,    0,    0,  816,    0,    0,    0,    0,
    0,  382,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  383,    0,    0,    0,    0,  479,
  384,  385,  417,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  816,  420,    0,  816,
    0, 1106,  277,  393,    0,    0,    0,    0,  386,  394,
    0,  387,    0,    0,  816,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,  389,
  390,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,  391,    0,    0,    0,  392,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1106,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,    0,    0,
    0,    0,    0,    0,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0, 1140,
    0,    0, 1105,    0,    0,    0,  381,    0,    0,  479,
    0,  484,  417,    0,  418,  382,    0,  816,    0,    0,
    0,    0,    0,    1,    0,    0,    0,  420,  383,    0,
    0,    0,  277,    0,  384,  385,    0,    0,    0,    0,
    0,    0,    0,    0,    2,    0,    0,    0,    0,    0,
    0,    0,    3,    0,    0,    0,    0,    0,    0,    4,
    0,    5,  386,    6,    0,  387,    0,    0,    7,    0,
    0,    0,    0,  816,    0,    8,  816,    0,    0,    0,
    0,    0,    0,  816,    0,    0,  816,    0,    0,    9,
  816,    0,    0,  389,  390,  816,    0,    0,  816,    0,
   10,    0,    0,    0,    0,  391,    0,  479,    0,  392,
  417,    0,  418,    0,  816,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  420,    0,    0,    0,    0,
  277,    0,    0,    0,    0,    0,    0,    0,  816,    0,
    0,  816,  816,    0,    0,    0,    0,    0,    0,    0,
    0,  816,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1106,    0,  393,   11,  816,    0,
    0,    0,  394,    0,   12,    0,    0,   13,    0,   14,
   15,    0,  395,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,    0,    0,  816,  396,  397,  398,  399,
  202,  203,    0,    0,   16,    0,    0,  816,  584,  816,
  400,    0,  585,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
  586,  108,    0,  587,  411,  412,  381,  413,  414,  415,
  111,  112,  113,  416,  114,  382,    0,  115,  116,  117,
  118,  119,    0,   17,    0,    0,    0,    0,  383,    0,
    0,    0,    0,    0,  384,  385,    0,    0,   18,   19,
    0,    0,    0,    0,  588,    0,    0,    0,    0,    0,
  589,  590,  591,    0,  592,  593,  594,    0,  595,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  596,    0,
  597,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  388,    0,  389,  390,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  381,  391,    0,    0,    0,  392,
    0,  598,    0,  382,    0,    0,  956,    0,  956,  956,
    0,  956,    0,    0,    0,    0,  383,    0,  599,    0,
    0,    0,  384,  385,  956,    0,    0,    0,    0,  956,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  600,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  601,  602,    0,    0,  388,
    0,  389,  390,    0,    0,    0,  396,  397,  398,  399,
  202,  203,  603,  391,    0,    0,    0,  392,    0,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,  684,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,    0,  641,    0,
  394,  641,    0,  641,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,  220,    0,  641,    0,  220,    0,
    0,  641,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,  220,    0,    0,    0,  220,    0,  400,  220,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  956,    0,  115,  116,  117,  118,  119,
  220,    0,  956,    0,    0,    0,  220,  220,  220,    0,
  220,  220,  220,    0,  220,  956,  547,    0,    0,    0,
    0,  956,  956,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  220,    0,  220,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  956,
    0,    0,  956,    0,    0,    0,    0,  547,    0,    0,
  547,    0,    0,    0,    0,    0,    0,  220,    0,    0,
    0,    0,   80,    0,    0,  547,    0,  547,  956,    0,
  956,  956,    0,    0,  220,    0,    0,    0,    0,    0,
    0,    0,  956,    0,    0,    0,  956,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,  547,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  641,    0,
    0,  220,  220,    0,    0,  641,    0,    0,    0,    0,
    0,    0,    0,  956,  641,    0,    0,  419,  220,  956,
  417,    0,  418,    0,    0,    0,    0,  641,    0,  956,
    0,    0,    0,  641,  641,  420,    0,    0,    0,    0,
  277,  641,    0,  956,  956,  956,  956,  956,  956,    0,
    0,    0,    0,    0,    0,    0,    0,  956,    0,    0,
    0,  641,    0,    0,  641,    0,  956,  956,  956,  956,
  956,  956,  956,  956,  956,  956,  956,    0,  956,    0,
    0,  956,  956,    0,  956,  956,  956,  956,  956,  956,
  956,  956,  641,  641,  956,  956,  956,  956,  956,    0,
    0,    0,    0,    0,  641,    0,    0,    0,  641,    0,
    0,    0,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,  547,    0,    0,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  547,    0,    0,    0,  547,    0,  547,    0,    0,    0,
  547,    0,    0,  547,    0,  641,    0,    0,    0,    0,
    0,  641,    0,    0,    0,    0,  547,    0,    0,    0,
    0,  641,  547,    0,    0,  547,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  641,  641,  641,  641,  641,
  641,    0,    0,    0,    0,    0,    0,    0,    0,  641,
    0,    0,    0,    0,    0,    0,    0,    0,  641,  641,
  641,  641,  641,  641,  641,  641,  641,  641,  641,    0,
  641,    0,    0,  641,  641,    0,  641,  641,  641,  641,
  641,  641,  641,  641,  381,    0,  641,  641,  641,  641,
  641,    0,    0,  382,  547,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,    0,    0,  547,    0,    0,    0,
    0,    0,    0,  547,    0,    0,    0,  547,    0,    0,
    0,    0,    0,    0,  547,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,  547,    0,    0,    0,    0,  547,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  388,
    0,  389,  390,    0,    0,    0,    0,    0,    0,    0,
    0,  381,  547,  391,    0,    0,  547,  392,    0,    0,
  382,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,  383,    0,    0,    0,    0,    0,  384,
  385,  420,    0,    0,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
  387,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,    0,    0,  389,  390,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
  391,    0,    0,    0,  392,    0,    0,    0,  400,  537,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
  537,    0,    0,  537,    0,    0,    0,    0,    0,    0,
    0,  393,    0,    0,    0,    0,    0,  394,  537,    0,
  537,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,  676,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
  537,    0,    0,    0,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
  381,    0,  115,  116,  117,  118,  119,    0,    0,  382,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,  387,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,  479,  929,    0,  417,    0,  418,    0,  391,
    0,    0,    0,  392,    0,    0,    0,    0,    0,    0,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  537,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  537,    0,    0,    0,  537,    0,  537,
  684,    0,    0,  537,    0,    0,  537,    0,    0,    0,
  393,    0,    0,    0,    0,    0,  394,    0,    0,  537,
    0,    0,    0,    0,    0,  537,  395,    0,  537,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,  381,
    0,  115,  116,  117,  118,  119,    0,  537,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,  537,
    0,    0,    0,  767,    0,    0,  537,    0,    0,    0,
  537,    0,    0,    0,    0,    0,    0,  537,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,  537,    0,    0,    0,    0,
  537,    0,    0,    0,  767,    0,    0,  767,    0,  381,
    0,    0,    0,    0,  388,    0,  389,  390,  382,    0,
    0,  479,  767,    0,  417,  537,  418,    0,  391,  537,
    0,  383,  392,    0,    0,    0,    0,  384,  385,  420,
    0,    0,    0,    0,  277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,    0,    0,    0,  394,  389,  390,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,  391,    0,
    0,    0,  392,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,  393,
  115,  116,  117,  118,  119,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  381,    0,
  115,  116,  117,  118,  119,    0,    0,  382,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,  420,    0,
    0,  767,    0,  277,  767,    0,    0,    0,    0,    0,
    0,  767,    0,    0,  767,    0,    0,    0,  767,    0,
    0,    0,    0,  767,  386,    0,  767,  387,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  767,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  389,  390,    0,    0,    0,
  479,    0,    0,  417,    0,  418,  767,  391,    0,  767,
  767,  392,    0,    0,    0,    0,    0,    0,  420,  767,
    0,    0,    0,  277,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  767,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  767,    0,
    0,    0,    0,  767,    0,    0,    0,    0,  393,    0,
    0,    0,    0,    0,  394,  767,    0,  767,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,  854,    0,
    0,    0,  585,    0,    0, 1132,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,  855,    0,    0,    0,
  586,    0,  400,  587,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,  381,    0,  115,
  116,  117,  118,  119,  588,    0,  382,    0,    0,    0,
  589,  590,  591,    0,  592,  593,  594,    0,  595,  383,
    0,    0,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  596,    0,
  597,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  598,    0,    0,    0,    0,    0,  381,    0,    0,
    0,    0,    0,    0,  389,  390,  382,    0,  599,  479,
    0,    0,  417,    0,  418,    0,  391,    0,    0,  383,
  392,    0,    0,    0,    0,  384,  385,  420, 1703,    0,
  600,    0,  277,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,  601,  602, 1317,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  393,    0,    0,
    0,    0,  603,  394,  389,  390,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,  391,    0,    0,    0,
  392,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,  393,  115,  116,
  117,  118,  119,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1330,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,  381,    0,  115,  116,
  117,  118,  119,    0,    0,  382,    0,    0,  479,    0,
    0,  417,    0,  418,    0,    0,    0,    0,  383,    0,
    0,    0,    0,    0,  384,  385,  420,    0,    0,    0,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  389,  390,    0,    0,    0,  956,    0,
    0,  956,    0,  956,    0,  391,    0,    0,    0,  392,
    0,    0,    0,  220,    0,    0,  956,  220,    0,    0,
    0,  956,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  220,    0,    0,    0,  220,    0,    0,  220,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,  220,
    0,    0,  395,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,  220,    0,  220,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,  220,  115,  116,  117,
  118,  119,    0,    0,  382,    0,    0,    0,    0,    0,
    0,    0,    0,  220,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  521,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,  220,    0,    0,  795,  956,    0,    0,    0,    0,
    0,    0,  389,  390,  956,    0,    0,  220,  521,    0,
    0,  521,    0,    0,  391,    0,    0,  956,  392,    0,
    0,    0,    0,  956,  956,    0,  521,    0,  521,    0,
    0,    0,    0,    0,    0,  795,    0,    0,  795,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  956,    0,  795,  956,    0,    0,    0,  521,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,    0,
    0,  394,  956,  956,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  956,    0,    0,    0,  956,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,  956,  115,  116,  117,  118,
  119,  956,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  956,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  956,  956,  956,  956,  956,
  956,    0,    0,    0,    0,    0,    0,    0,    0,  956,
    0,    0,    0,    0,    0,    0,    0,    0,  956,  956,
  956,  956,  956,  956,  956,  956,  956,  956,  956,  832,
  956,    0,    0,  956,  956,  521,  956,  956,  956,  956,
  956,  956,  956,  956,    0,    0,  956,  956,  956,  956,
  956,  521,    0,    0,    0,  521,    0,  521,    0,    0,
    0,  521,    0,    0,  521,    0,  795,    0,    0,    0,
  832,  994,  994,  832,  994,  994,  994,  521,    0,    0,
    0,    0,    0,  521,    0,    0,  521,    0,  832,  994,
  994,  994,  133,    0,    0,    0,  133,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  133,    0,    0,  133,    0,    0,
  994,    0,  795,    0,    0,  795,    0,    0,    0,    0,
    0,    0,  795,    0,    0,  795,    0,    0,    0,  795,
    0,    0,    0,    0,  795,    0,    0,  795,    0,    0,
    0,    0,    0,    0,    0,  521,    0,    0,  133,    0,
    0,  833,    0,  795,  133,  133,  133,    0,  133,  133,
  133,    0,  133,    0,    0,    0,    0,  521,    0,    0,
    0,    0,    0,    0,  521,    0,    0,  795,  521,    0,
  795,  795,  133,    0,  133,  521,    0,    0,    0,    0,
  795,    0,  833,  995,  995,  833,  995,  995,  995,    0,
    0,    0,    0,  521,    0,    0,    0,  795,  521,    0,
  833,  995,  995,  995,    0,  133,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  795,
    0,    0,  133,  521,  795,  793,    0,  521,    0,    0,
    0,    0,  995,    0,    0,    0,  795,    0,  795,    0,
    0,    0,    0,    0,  133,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  793,    0,    0,  793,
    0,  832,    0,    0,    0,    0,    0,    0,    0,  133,
  133,  994,    0,    0,  793,    0,    0,    0,    0,  994,
    0,    0,    0,    0,  994,    0,  133,  994,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  834,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  832,    0,    0,
  832,    0,    0,  994,    0,    0,    0,  832,    0,    0,
  832,    0,  994,    0,  832,    0,    0,    0,    0,  832,
  994,    0,  832,    0,    0,    0,    0,  834,  996,  996,
  834,  996,  996,  996,    0,    0,    0,    0,  832,  994,
    0,  994,    0,    0,    0,  834,  996,  996,  996,    0,
    0,    0,    0,    0,    0,  994,    0,  994,  994,  994,
    0,    0,  832,  794,    0,  832,  832,    0,    0,  824,
    0,    0,    0,  833,    0,  832,    0,  996,    0,    0,
    0,    0,    0,  995,    0,    0,    0,    0,    0,    0,
    0,  995,  832,    0,    0,    0,  995,    0,    0,  995,
    0,    0,    0,    0,  794,    0,    0,  794,  994,    0,
  824,    0,    0,  824,  832,    0,    0,    0,    0,  832,
    0,    0,  794,    0,    0,    0,    0,    0,  824,  833,
    0,  832,  833,  832,    0,  995,    0,    0,    0,  833,
    0,    0,  833,    0,  995,    0,  833,    0,    0,    0,
    0,  833,  995,    0,  833,    0,    0,  793,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  833,  995,    0,  995,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  995,    0,  995,
  995,  995,    0,    0,  833,    0,    0,  833,  833,    0,
    0,  788,    0,    0,    0,    0,    0,  833,    0,    0,
    0,    0,    0,  793,    0,    0,  793,    0,    0,    0,
    0,    0,    0,  793,  833,    0,  793,    0,    0,    0,
  793,    0,    0,    0,    0,  793,    0,    0,  793,    0,
  995,    0,  788,  789,    0,  788,  833,    0,  834,    0,
    0,  833,    0,    0,  793,    0,    0,    0,  996,    0,
  788,    0,    0,  833,    0,  833,  996,    0,    0,    0,
    0,  996,    0,    0,  996,    0,    0,    0,  793,    0,
    0,  793,  793,    0,  789,    0,    0,  789,    0,    0,
    0,  793,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  789,    0,  834,    0,    0,  834,  793,    0,
  996,    0,    0,    0,  834,    0,    0,  834,    0,  996,
    0,  834,    0,    0,    0,  794,  834,  996,    0,  834,
  793,  824,    0,  790,    0,  793,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  834,  996,  793,  996,  793,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  996,    0,  996,  996,  996,    0,    0,  834,
    0,    0,  834,  834,  790,  823,    0,  790,    0,    0,
    0,  794,  834,    0,  794,    0,    0,  824,    0,    0,
  824,  794,  790,    0,  794,    0,    0,  824,  794,  834,
  824,    0,    0,  794,  824,    0,  794,  817,    0,  824,
    0,    0,  824,    0,    0,  996,  823,    0,    0,  823,
    0,  834,  794,    0,    0,    0,  834,    0,  824,    0,
    0,    0,    0,    0,  823,    0,    0,    0,  834,    0,
  834,    0,    0,    0,    0,    0,  794,    0,  817,  794,
  794,  817,  824,    0,    0,  824,  824,    0,    0,  794,
    0,    0,    0,  788,    0,  824,  817,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  794,    0,    0,    0,
    0,    0,  824,    0,    0,    0,    0,    0,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,  794,    0,
    0,    0,    0,  794,  824,  789,    0,    0,    0,  824,
  826,    0,    0,    0,    0,  794,    0,  794,    0,  788,
    0,  824,  788,  824,    0,    0,    0,    0,    0,  788,
    0,  825,  788,    0,  825,    0,  788,    0,    0,    0,
    0,  788,    0,    0,  788,    0,    0,    0,    0,  825,
    0,  826,    0,    0,  826,    0,    0,    0,    0,    0,
  788,  789,    0,    0,  789,    0,    0,    0,    0,  826,
    0,  789,    0,    0,  789,    0,    0,    0,  789,    0,
    0,    0,    0,  789,  788,    0,  789,  788,  788,    0,
    0,    0,    0,    0,    0,  790,    0,  788,    0,    0,
    0,    0,  789,    0,    0,    0,  814,    0,    0,    0,
    0,    0,    0,    0,  788,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  789,    0,    0,  789,
  789,    0,    0,    0,    0,    0,  788,  823,    0,  789,
  818,  788,    0,    0,    0,    0,    0,  814,    0,    0,
  814,  790,    0,  788,  790,  788,  789,    0,    0,    0,
    0,  790,    0,    0,  790,  814,    0,    0,  790,  817,
    0,    0,    0,  790,    0,    0,  790,    0,  789,    0,
    0,  818,    0,  789,  818,    0,    0,    0,    0,    0,
    0,    0,  790,  823,    0,  789,  823,  789,    0,  818,
    0,    0,    0,  823,  827,    0,  823,    0,    0,    0,
  823,    0,    0,    0,    0,  823,  790,    0,  823,  790,
  790,    0,    0,    0,    0,  817,    0,    0,  817,  790,
    0,    0,    0,    0,  823,  817,  815,    0,  817,    0,
    0,    0,  817,    0,    0,  827,  790,  817,  827,    0,
  817,    0,  825,    0,    0,    0,    0,    0,  823,    0,
    0,  823,  823,  827,    0,    0,  817,    0,  790,    0,
    0,  823,  826,  790,    0,    0,    0,  815,  819,    0,
  815,    0,    0,    0,    0,  790,    0,  790,  823,    0,
  817,    0,    0,  817,  817,  815,    0,    0,    0,    0,
    0,    0,    0,  817,    0,    0,    0,    0,  825,    0,
  823,  825,    0,    0,    0,  823,    0,    0,  825,  819,
  817,  825,  819,    0,    0,  825,    0,  823,  826,  823,
  825,  826,    0,  825,    0,    0,    0,  819,  826,    0,
    0,  826,  817,    0,    0,  826,    0,  817,    0,  825,
  826,    0,    0,  826,    0,    0,    0,    0,    0,  817,
    0,  817,    0,    0,    0,    0,    0,    0,  814,  826,
    0,    0,    0,  825,    0,    0,  825,  825,    0,    0,
    0,    0,    0,    0,    0,    0,  825,    0,    0,    0,
    0,    0,    0,  826,    0,    0,  826,  826,    0,    0,
    0,    0,  818,  825,    0,    0,  826,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  826,  814,  825,    0,  814,    0,    0,
  825,    0,    0,    0,  814,    0,    0,  814,    0,    0,
    0,  814,  825,    0,  825,  826,  814,    0,    0,  814,
  826,    0,    0,    0,    0,    0,    0,    0,  818,    0,
    0,  818,  826,    0,  826,  814,  827,    0,  818,    0,
    0,  818,    0,    0,    0,  818,    0,    0,    0,    0,
  818,    0,    0,  818,    0,    0,    0,    0,    0,  814,
    0,    0,  814,  814,    0,    0,    0,    0,  815,  818,
    0,    0,  814,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  814,
    0,    0,  827,  818,    0,  827,  818,  818,    0,    0,
    0,    0,  827,    0,    0,  827,  818,    0,    0,  827,
  819,  814,    0,    0,  827,    0,  814,  827,    0,    0,
    0,    0,    0,  818,  815,    0,    0,  815,  814,    0,
  814,    0,    0,  827,  815,    0,    0,  815,    0,    0,
    0,  815,    0,    0,    0,  818,  815,    0,    0,  815,
  818,    0,    0,    0,    0,    0,    0,  827,    0,    0,
  827,  827,  818,    0,  818,  815,  819,    0,    0,  819,
  827,  320,    0,    0,    0,    0,  819,    0,    0,  819,
    0,    0,    0,  819,    0,    0,    0,  827,  819,  815,
    0,  819,  815,  815,    0, 1414,    0,    0,    0,    0,
    0,  320,  815,    0,    0,    0,    0,  819,  320,  827,
    0,    0,  320,  320,  827,  320,    0,    0,  320,  815,
    0,    0,    0,    0,    0,    6,  827,    0,  827,    0,
    0,  819,  320,    0,  819,  819, 1533, 1534,  320, 1535,
    0,  815, 1536,    0,  819,    0,  815,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1537,    0,  815,    0,
  815,  819, 1538,    0,    0,    0,    0,    0,    0, 1414,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  819,    0,    0,    0,    0,  819,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  320,    6,
  819,    0,  819,    0,    0,    0,    0,    0,    0,    0,
 1533, 1534,    0, 1535,    0,    0, 1536,  320,    0,    0,
    0,    0, 1539,    0,    0,    0,    0,    0,    0,    0,
 1537,    0,  320,    0,    0,    0, 1538,    0,    0,    0,
    0,   14,  320,    0,    0,    0,    0,    0,    0,    0,
    0,  320,    0,  320,    0,    0, 1540,    0,    0,    0,
  320,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,  584,    0,    0, 1664,  585, 1541,    0,    0,
    0,    0,    0,  782,  105,    0,    0,    0,    0,  320,
  320,    0,  320,    0,  586,    0, 1539,  587,    0,  320,
  320,  320,  320,  320,    0,  320,    0,    0,  320,  320,
  320,  320,  320, 1542,  107,   14,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
 1540,    0,  115, 1543,  117,  118,  119,    0,  588,    0,
   16,    0,    0,    0,  589,  590,  591,    0,  592,  593,
  594, 1541,  595,  584,    0,    0,    0,  585,  105,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  596,    0,  597,  586,    0,    0,  587,    0,
    0,    0,    0,    0,    0,    0,    0, 1542,  107,    0,
  108,    0,    0,    0,    0,    0,    0,  109,  110,  111,
  112,  113,    0,  114,    0,  598,  115, 1543,  117,  118,
  119,    0,    0,    0,    0,    0,    0,    0,    0,  588,
    0,    0,  599,    0,    0,  589,  590,  591,    0,  592,
  593,  594,    0,  595,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  600,    0,    0,    0,    0,    0,
    0,    0,    0,  596,    0,  597,    0,    0,    0,  105,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  601,
  602,    0,    0,    0,    0,    0,  598,    0,  106,  107,
    0,  108,    0,    0,    0,    0,  603,    0,  109,  110,
  111,  112,  113,  599,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  600,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  601,  602,    0,    0,    0,    0,    0,    0,    0,  106,
  107,    0,  108,    0,    0,    0,    0,  603,    0,  109,
  110,  111,  112,  113,  105,  114,    0,    0,  115,  116,
  117,  118,  119,  162,  163,  164,  165,  166,  167,  168,
  169,  170,  171,  172,  173,  174,  175,  176,  177,    0,
    0,  178,  179,  106,  107,    0,  108,    0,    0,  180,
  181,    0,    0,  109,  110,  111,  112,  113,  182,  114,
  183,    0,  115,  116,  117,  118,  119,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  104,   18,   62,
    0,   63,  254,  509,  351,   56,  359,  285,  359,  149,
  355,  138,  139,  553,   85,  330,   67,   63,   57,  138,
  139,   94,  419,  492,  516,   89,   73, 1248,  664,  699,
   69,  262,  759,   53,   54,   55,   56,   57,   58,   90,
  271, 1028,  925,   63,   64,   65,   97,   67,   95,   69,
   70,  544,   72,   73,  547,   68,  328,  676,  684,  467,
  553,   81,   83,   83,   84,   85,   70,   87,   88,   89,
   90,   92,   92,   93,   94,   95,  746,   97,   91,  152,
  963, 1021,  323,  129,  848,  131,   88,  713, 1028,   93,
  376,  130,  636, 1021, 1021,    0, 1021,  364,    0,  150,
 1028, 1028,  753, 1028,  279, 1029,    0,  973, 1021,  129,
  130,  131,  151,    0,  750, 1028, 1040,    0,  717,  226,
    0,    0,    0,  143,  187,  306,   44, 1076,  779,  149,
  150,  151,  152,   44,  817, 1059,  156,  788,   45,   41,
    0,  143,   44,  629,  698,    0,   44,   40,    0,  212,
    0,   41, 1190,   61,   59,    0,   45,   59,   41,   59,
   41,   44,  756,   44,   41,   59,  306,  419,    0,  717,
   40,   44,   59,   40,    0,   41,   59, 1053,  523,   59,
   59,   59,   45,  530,  531,  532,  533,  159,    0,   41,
  841,  156,   44,   63, 1077,    0,   63,   41,   63,   59,
  553,   41,  553,   40,   59,   44,  313,   59,   44,   59,
  256,   45,  269,   40,   59,   44,    0,    0,   40,  217,
  726,  260,  283,   45,   42,   45,  290,   59,  459,   63,
  269,  301,   44,   59,  579,  301,  256, 1246,  355,  262,
  260,    0,  262,  263,  511,  260,  277,   59,  274,  269,
 1394,  271,  260,   44,   59,  294,  262,   41,    0,  258,
  306,   41,  356,   12,   44, 1194,   44,  320,  264,  301,
  290,  473,  891,  892,  294,   59,   59,  268, 1738, 1739,
  377,  260,   41,  329,  556,   44,  338, 1274,   44,  352,
  336,  354,  338,  455,  340,    0,   41,  343,  584,  417,
   59,    0,   61,  922,  273,  368,  338,  306,  372,  329,
  361,  309,  805, 1071,  345,   41,  336,   59,  338,   41,
  340,   41,    0,  343,  303,  300,  444,  467,  964,   16,
   41,  370,    0,  605, 1274,  429,   41,   93,    0, 1263,
 1206,  361,  362,  994,    0,  317, 1274, 1274, 1241, 1274,
  370,  448,  372, 1574,   59,    0,  825,  956,  362,   44,
   59, 1274,  376,  386, 1293,    0,    0, 1827,  260,  968,
  969,  340,  442,   41,  365,  492,  442,  352,  439,   41,
  386,   59,   44,  436,    0,  262,  664,  136, 1039,    0,
  442,   59,  300,  274, 1287,    0,   83,   59,    0,  320,
  420, 1020,  417,   59, 1280,    0,   44,  458,  956,  417,
 1419,  303,  435,  258,   59,  411,  455,    0,  475,  418,
  968,  467,  473,  328,   59,   59,  331,  466,  309,  681,
   41,  470,  269,   44,  328,  455,  475,  331,  458,  459,
  466,  331,  260,   59,    0,  697,  466,  320,   59,  727,
  470, 1672,  571,  473,   59,  475,   44,   59,  478, 1217,
  340,  306,  340,  342,   59,  528,  318, 1505, 1366,  309,
  490,  331,  750,  264,  318,  258,   59,  708, 1097, 1098,
 1394, 1100, 1101, 1381,  320,  303, 1410, 1081,  340,  318,
 1634, 1415,  318, 1417,   41,  767, 1223,  387,  770,  258,
  383,  394, 1451,   59, 1397, 1726,  396,  553, 1191,  386,
  410,  574,  784,  478,  340,  274,  328,  262,  720,  331,
    0,  387, 1286,  306,    0,  766,  405, 1393, 1452,  408,
  411,  343,  552,  553,  318,  440,  262,  852,  558,  811,
  458,  300, 1086,  454, 1765,  424,  440,  306,  723,  454,
  309,  458,  550,  961,  452, 1109,  340,    0,  452,  572,
  454,  387,  625,  626,   44,   41,  747,  454,   44,  458,
  584,  450,  839,  418,  454,  861,  454,  409,    0,   59,
  440,  436,  628,   59, 1070,  466, 1269, 1470,  458, 1465,
  440,  458,    0,  458,  454,  458,  485,  431,   41,  501,
 1524,   44, 1201,  387,  454,  410, 1530,  747,  628,  431,
 1229,  431,  396,  732,  440,  454,   59,  637,  501,   41,
  447,  619,   44,  637,  458,  367,    0,  637,  440,  264,
  264,  411, 1251,   41, 1285,  418,  458,   59,  458,   41,
  452,  386,  454,  762,  763,  307,  308,  667,  264,  348,
  502,   59,  411,  264,  700, 1579,  440,  259,  436,  418,
  386, 1008,  264,   40,  386,  667,  304,  387,  270,  720,
  454,  691, 1596, 1394, 1432,  386,  396,  436, 1405,  699,
  700,  264,  387,  703,  340,   59,  964,   91,  708,  367,
    0, 1317, 1450, 1586,    0,  758,  759,  717,  383, 1592,
  720,  747,  741,  348,  743,  725,  752,  466,  728,  320,
  756,  470,    0, 1700,  945,  262,  304,  948, 1296,  387,
 1634,  741,  278,  743,  280, 1072,  746, 1552,  396,    0,
  776,   41,  752,    0,   44,  340,  756, 1394,  257,    0,
    0,    0,  278, 1021,  262,  402, 1382,  342,  410,   59,
 1028,  273,  765,   59, 1026, 1679,  776, 1249,  804,  805,
 1700,  262,  397,  397, 1688, 1454, 1038,    0, 1183, 1084,
 1185,   59, 1700, 1700, 1171, 1700, 1395,    0,  313,    0,
 1682,  397,  377,  319,  804,  805, 1058, 1700,   59,  451,
  803,  642,   59, 1691,    0,  397,  810,   41,   59,   59,
   59,  840,  278, 1272,  280, 1345, 1730, 1204,  822, 1278,
  405,  850,  506,  408,  397,  334,   40,  337,    0, 1406,
  840,  961,  835,  836,  837,  319,   59,    0,    0,  424,
  850, 1733, 1147,  405,   58,  278,   59,  280,   59,  386,
   59,  452,  862,   40,  287,  642,  409,  861,  328,  460,
  647,  331,  424,   59,  374,  450,  278,   12,  280,   41,
  262,   41,   44,  343,  400,  287, 1453,  387,  386,   41,
  307,  308,   44,  277, 1776,    0,  949,   59,  489,  490,
 1458,  492,  417,  307,  308,  386,   59,   59,  499,  500,
  501,  502,  503,  261,  505,  946,  402,  508,  509,  510,
  511,  512,  378,  950,  301,  441,  400,  284,  405,    0,
  318,  325,  458, 1634,  286,  961, 1194,  435, 1161, 1171,
  328, 1164,  259,  331,  400,  945,  946,  947,  948,  949,
  950,  407,  340,  270,   59,  378,  956, 1056, 1763,  445,
  383,  338,    0,  947,  318, 1290,    0,  441,  968,  969,
   41,  259, 1204,  973,  387,   40,  378,  400,  264,  456,
  440,  321,  270, 1633,  407,  441,  340,    0,   59, 1384,
 1795, 1796,  452,  257,  454,  335,  264, 1634,  400,  387,
  791,  136,   42,   41,  386,  407,   44,   41,  396, 1261,
   44, 1680, 1345,  264, 1345, 1820, 1274, 1503,  441,  359,
 1506,   59,  508,  264,  264,   59, 1831,  440,  318, 1834,
    0,  321, 1631,  387,  451, 1293,  430,  868,  328,  441,
 1292,  331,  396,  437,  458,  335,   59,  451, 1074, 1049,
  340,  264,  440,  343, 1653, 1081, 1050, 1656,  283, 1532,
 1050,  264,   42, 1063,   41,  442,  454,   44,  348,  359,
  334,   41, 1065,  413, 1074, 1402,  445,  260,  501,  318,
  377, 1081, 1189, 1190, 1191, 1205,  440,  864,  343,   59,
 1083,  868,  278,  383,  280,  342, 1089,  387,  405,  501,
   42,  340,  262,  460,  328,   47,  396,  331,  322,  461,
  462,  397, 1598,  368,   42,   43,  278,   45,  280,   47,
  303,  418, 1568,  413, 1382,  287, 1225,   45, 1227,  397,
  377,   59,  489,  490,    0,  492, 1359, 1693, 1694,  508,
 1363,  342,  499,  500,  501,  502,  503,  287,  505,  456,
  440,  508,  509,  510,  511,  512,  397,  397,  405,  384,
  300,  408,  452,  387,  322,  318,  318, 1160,  465, 1421,
  460,  426,  396,    0,    0,  328,  377,  424,  331, 1735,
 1223,  461,  462, 1183,  397, 1185, 1632,  340,  340, 1288,
  263,   44,  378,   59,  397, 1189, 1190,  458,  417,  489,
  490, 1201,  492,  450,  405, 1205, 1206,  408, 1460,  499,
  500,  501,  502,  503,  439,  505,  378,  401,  508,  509,
  510,  511,  512,  424,  485,  444,  386, 1826,   40,  265,
    0,  456,   59,   59,  387, 1780, 1781,  342,  400,  312,
   93, 1032, 1033,  396,  278,  407,  280,  318,   44,  450,
  323, 1737, 1808,  287,   42,   43,  460,   45,    0,   47,
 1303,   44, 1684,  258, 1307,  278,  302,  280, 1628,  340,
 1630,   41,  377, 1267, 1394,  397, 1499, 1267,    0,  441,
  318,  383,  493,  460,  265,  489,  490,  440,  492,   59,
 1081, 1836,  503,  331, 1414,  499,  500,  501,  502,  503,
  405,  505,  340,  408,  508,  509,  510,  511,  512, 1731,
   93,  306,  489,  490,  436,  492,  296,   59,    0,  424,
  254,  302,  499,  500,  501,  502,  503, 1687,  505,   44,
    0,  508,  509,  510,  511,  512,  402,   59,  280,  501,
    0, 1593,  276,  285,  378,  450,  385,  378,  470,  387,
    0,  285,  280, 1775,  427,    0,  337,  285,  396,   41,
  258,  331, 1405,  261,  352,  402,  400,  236,  434,  400,
    0, 1413,    0,  407,    0,  244,  407,   59,   93,  383,
    0,   41,  316,  449, 1384, 1807,  383, 1413, 1414,   59,
  378,   41,    0, 1393,   44,  460,   41,  348, 1505,   59,
  863,  438,  440, 1422,  867,  272,  357,  441,  306,   59,
  441,   61,  340, 1413,   59,   41,  454,  387,   44, 1779,
  460,  288, 1422,  418,  489,  490,  396,  492, 1527,   59,
    0,   59, 1552,   59,  499,  500,  501,  502,  503,   59,
  505,  436, 1700,  508,  509,  510,  511,  512,    0,  489,
  490,   59,  492,   42,   43,  260,   45,    0,   47,  499,
  500,  501,  502,  503,  383,  505,  333,  501,  508,  509,
  510,  511,  512, 1705, 1708,  470,    0,    0,  436,  413,
  460,  260, 1533,  417,  418,  419,  403,    0,    0,   59,
 1577,  318,  280,  279,  494,  362,   41,  285,  303,   44,
  301, 1495,   41,  420,  331,   44,    0,   59,    0,  489,
  490, 1505,  492,  340, 1634, 1749,   59,    0,  337,  499,
  500,  501,  502,  503,  303,  505, 1552,  356,  508,  509,
  510,  511,  512, 1533, 1578,   59,   59,  338,  316,  317,
  458, 1661,  460,  329,    0,  479,   59,   59,  318,  483,
  418,    0,  340,    0, 1573, 1789, 1536,   59, 1792, 1602,
  387,  331,  429, 1606,  393,   59,  395,   59, 1568,  396,
  340,  489,  490, 1573,  492,  274,   59,  363, 1578,  318,
 1814,  499,  500,  501,  502,  503, 1673,  505, 1708,  418,
  508,  509,  510,  511,  512,  381, 1597, 1597,    0, 1183,
   40, 1185,   40,   59,   42,   43,  407,   45,  258,  321,
   59,  424,   59,  440,  443, 1646,  417,  387,    0,    0,
   58,   41,  461,  335,   44,   63,  396,  277,  278, 1749,
  280,  343, 1632, 1633,  284,  264,    0,  287, 1758,   41,
   44,  442,   44, 1763,    0,  316, 1646,  359,   42,   43,
  300,   45,  278,   47,  280,   59,  306,   59,  318,   41,
   41,  287,   44,   44,  318,  337, 1717,    0,  328, 1789,
  440,  331, 1792,   93,  300, 1795, 1796,   59,   59,    0,
  340, 1682, 1682,  328,  454,    0,  331,   42,   43,    0,
   45,  280,   47,  337, 1814,   59,  285,  280, 1699, 1699,
 1820,  413,  374,   59,  460,  288,    0,   42,   43,   46,
   45, 1831,   47,  647, 1834,  387,  280, 1717, 1718,    0,
    0,  285,  336,   44,   61, 1719,   59,  387,  378, 1719,
  664,  320, 1733, 1733,  668,  321,  396, 1763,   59, 1740,
 1740,  675,  387, 1787,   59,    0,  680,  681,   59,  335,
  400,  396,  378,  402,  260,    0,  690,  407,  692,  693,
  694,  695,  696,  697,  698,   59,  416,   44,  418, 1795,
 1796,  461,  462,  359,  400, 1776, 1776,  452,   59,   59,
  440,  407,   59, 1784, 1784,  279,  436, 1787,   46,  493,
  494,  441,  452,  727, 1820,  301,  465,  383,  467,   41,
    0,    0,   44,   61,   59, 1831,  300,  313, 1834,  340,
   41,   40, 1812,   44,   59,  441,  750,  321,  318, 1819,
  470,  755,  260,    0,  320,   41,  402,  413,   44,  267,
  383,  335,  338,    0,  383,  329,  436, 1837,  276,  343,
   41,   41,   41,   44,   44,   44,  361,   42,   43,  506,
   45,  289,   47,  450,  348,  359,    0,  295,  296,   59,
   59,   42,   43,  357,   45,  303,   47,   41,  434,  363,
   44,  265,  357,   40,   41,   42,   43,   44,   45,   46,
   47,   41,   59,  384,   44,  323,  280,  381,  326,  461,
  462,  285,   59,   60,   61,   62,  278,   41,  280,  417,
   44,  407,  284,   41,   41,  287,   44,   44,  302,  413,
   41,  417,  442,   44,  352,   59,  354,  355,  300,   93,
   41,  391,  392,   44,   91,  280,   93,  287,  366,  411,
  285,  469,  370,   41,   42,   43,  442,   45,   41,   47,
   44,   44,  876,  877,  878,  280,   61,  881,  882,  883,
  285,  885,  886,  887,  888,  889,  890,  372,  460,  893,
  894,  286,  896,   41,  898,  320,   44,  901,  902,  903,
  904,  905,  906,  881,   40,  883,  311,  885,  886,  264,
  888,  889,  890,   40,  918,   40,  920,  489,  490,  427,
  492,   40,   40,  461,    0,  433,  378,  499,  500,  501,
  502,  503,   40,  505,   40,  443,  508,  509,  510,  511,
  512,   41,   42,   43,   40,   45,   40,   47,  400,  457,
  458,  459,  460,  461,  462,  407,   40,  461,   41,    0,
  964,   44,   40,  471,   41,   41,  461,   44,   44,   40,
   40,   40,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   59,  492,   61,   40,  495,  496,  441,
  498,  499,  500,  501,  502,  503,  504,  505,   45,   44,
  508,  509,  510,  511,  512,   91,  342,   46,  278,  278,
  280,  280,  436,  320,  284,  280,   44,  287,   59,  402,
  285,   44,  320,   44,  428,  262,   46,  264,  265,  280,
  300,  663,  279,  665,  285,  272,   61,  669,  670,  671,
   40,  377,  264,  280,  382,   44,  311,   44,  285,   42,
   43,  288,   45,  300,   47,   41,   42,   43,  348,   45,
  278,   47,  278,  397,  278,  302,  280,  300, 1394,  405,
  307,  308,  408,  287,  311,   42,   43,   44,   45,  436,
   47,  318,  329,  320,  321,    0,  300,  324,  424,  264,
  470,  328,    0,  397,  331,  458,  333,  277,  335,  447,
   40,  348,  280,  340,  341, 1109,  343,  285,  378,  378,
  357,  399,  447,  349,  450,   40,  363, 1121, 1122,   91,
 1124, 1125,  359,  360,   40,  362,   41,   40, 1132,   44,
  400,  400,   40,   41,  381,  318,   44,  407,  407,  376,
    0,  378,  379,  380,   59,  383,  383, 1151,  264,  386,
  387,   59,  411,   61,   44, 1159,  382, 1161,  389,  396,
 1164,   40, 1166,   40,  378,   41, 1170, 1171, 1172, 1173,
  451,  441,  441,  410,  436,  321,  413,  457,  458,  459,
  280,   41,  258,   91,   44,  285,  400,  464,  317,  335,
 1194,   40,  429,  407,  320,  452,  320,  264,  435,   59,
 1204,  277,  278,  440,  280,  485,  486,  448,  284,  502,
  458,  287,  454,  359,  451,  452,   40,  454,  277,  274,
   40,  300,  284,  460,  300,  278,  386,  441,  386,  344,
  306,  344,  386,   40,  418,  416,    0,  383,  279,  300,
 1244,  418,  356,   41,   42,   43,  417,   45,  295,   47,
   40,  417,  489,  490,   40,  492,  274,  264,   41,  300,
   44,  280,  499,  500,  501,  502,  503,  413,  505,  262,
  320,  508,  509,  510,  511,  512,  323,   41,   42,   43,
   44,   45,   46,   47,   44,   44,  385,  280,  329, 1293,
   44,  320,  285,  367,  280,   59,   60,   61,   62,  285,
   40,   44,  378,   44,  328,  352,  505,  348, 1634,  266,
 1314,   40,  299,  280,  501,  356,  357,   40,  285, 1323,
  264,   58,  363,   41,  400,  422, 1330,   91,   58,   93,
   41,  407,   41,   41,  264, 1661,  440,  454,  394,  309,
  381,  492,  418,  309,   41,  294,    0,   41,   40,   40,
  258,  352,  393,   40,  395, 1359,  305,   40,   40, 1363,
  436,   40, 1661,  312,   40,  441,  274,   40,   40,  277,
  278,   40,  280,   40,   40,   40,  284,  418, 1382,  287,
  427,   40, 1708,    0,   40,   40,  435,   41,  337,  262,
   44,  308,  300,  327,  470,   44,  443,   41,  306,  388,
  388,  309,  443,  318,  388,   59,  343,  274,  331, 1708,
  457,  458,  459,  328,  461,  462,  331,   42,   43,  422,
   45,  390,   47, 1749,   41,  340,  391,   44,  343,  446,
  271,  287, 1758,  480,  481,  482,  483,  484,  485,  486,
  487,  488,   59,  407,  386,  386,   41,  386,  495,  496,
 1749,  386,  378,  402,  386,  386,   41,  504,  318, 1758,
  278,   40,  417, 1789,   41,   41, 1792,   41,  328,   41,
  378,  331,  387,  308, 1478, 1479,  435,  396,  385,  280,
  340,  396,  280,  343,  436,  434,  337,  285, 1814,  438,
 1789,  442,  400, 1792,  338, 1499,  442,  301,  262,  407,
  264,  265,  338,  411,  301,   41,   41,  352,  272,    0,
  418,  320,  371,  385,  371, 1814,  280,   41,   41,   41,
   41,  285,   41,   41,  288,  440,   41,  387,  436,   41,
   41,   41,   41,  441,   41, 1539,  396,  452,  302,   44,
  304,   41,    0,  307,  308,   41,   41,  311,   41,  280,
   41,   59,   46,   44,  318,   40,  320,  321,  466,  340,
  324,   59,  470,   40,  328,    0,  340,  331,   59,  333,
   61,  335,  308,   59,   41,   59,  340,  341,   59,  343,
  440,   59,  352,   41,   42,   43,   44,   45,   46,   47,
   59,   59,  452,  344,   41,  359,  360, 1601,  362,  368,
   40,   59,   60,   61,   62, 1609,   41,  340,  262,   44,
  383,  451,  376,   44,  378,  379,  380,   59,   59,  383,
  383,  383,  386,  387,   59,    0,   41,  343,   41,   41,
   44,  410,  396,   91,  304,   93,  435,  262,  301,  442,
  387, 1645,   40,   59,  304, 1649,  410,  293,  463,  413,
   41,   42,   43,   41,   45,  280,   47,   44,   44,  307,
  285,  278,    0,  280,  318,  429,   41,  321,  383,   44,
  287,  435,   40,  337,  328,  264,  440,  331,  383,  278,
  264,  335,  458,  389,   59,  320,  340,  451,  452,  343,
  454,   41,   42,   43,  264,   45,  460,   47,   41,   42,
   43, 1705,   45, 1707,   47,  359, 1710,  411,   44,  470,
   41,   42,   43,  451,   45,   40,   47,  309,   41,   42,
   43,  271,   45,  460,   47,  489,  490,  264,  492,  383,
  458,  343,  386,  387,  435,  499,  500,  501,  502,  503,
  436,  505,  396,  343,  508,  509,  510,  511,  512,  343,
  343,  458,  489,  490,  271,  492,  308,   41,   40,  413,
  308,  378,  499,  500,  501,  502,  503,  258,  505,   40,
    0,  508,  509,  510,  511,  512,   41,   42,   43,   59,
   45,  435,   47,  400,   59,   41,  440,  278,  308,  280,
  407,   59,  294,  284,   41,    0,  287,   59,  452,   59,
  454,  304,   41,  305,  262,    0,  264,  265,    0,  300,
  312,    0,    0, 1186,  272,  306,   12,  271,  264,  263,
  708,    0,  280,  325,  441, 1086,  632,  285, 1084,   59,
  288,  853,   87, 1073, 1440, 1081,   41,   42,   43,   44,
   45,   46,   47,  278,  302,  280, 1441,  326,  321,  307,
  308, 1063,  287,  311,   59,   60,   61,   62,  579,  356,
  318, 1011,  320,  321,   59,   44,  324,  752,  311, 1074,
  328, 1643,  338,  331,  338,  333,  786,  335, 1419, 1724,
   59,  558,  340,  341,  386,  343,   91,  378,   93,  280,
 1421, 1728, 1727, 1767,  285,  776, 1242, 1705, 1651, 1819,
  402,  359,  360, 1666,  362, 1764,   41,   42,   43,  400,
   45,  996,   47, 1219, 1398, 1682,  407,  419,  376,  343,
  378,  379,  380,  331,  560,  383,  832,  418,  386,  387,
  280,  324,  434, 1276,  618,  285,  438,  280,  396,  441,
  824,   13,  285,  378,  298,  436,  321,  449,  494,  280,
  441,  498,  410,  328,  285,  413,  331,  280,  141,  741,
  335,  755,  285,  728,  973,  400,  468, 1784,  343,  727,
 1634,  429,  407, 1522,   41,   42,   43,  435,   45,  470,
   47, 1740,  440, 1837,  359,  472,  473,  474,  475,  476,
  477,  478,  479,  451,  452,  509,  454,   41,   42,   43,
 1123,   45,  460,   47,  716,  965,  441, 1231,  383, 1166,
   -1,   -1,   41,   42,   43,  280,   45,   -1,   47,   -1,
  285,   -1,   -1,    0,   42,   43,   44,   45,  258,   47,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,  413,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,    0,  279,
  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  258,   -1,  440,   -1,  272,   -1,   -1,
  300,   -1,   -1,   -1,   -1,  280,  306,  452,   -1,  454,
  285,   -1,   59,  288,  279,   -1,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,  302,   -1,  329,
   -1,   -1,  307,  308,   -1,  300,  311,   59,   60,   61,
   62,  306,   -1,  318,   -1,  320,  321,   -1,  348,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,  357,  333,   -1,
  335,   -1,   -1,  363,  329,  340,  341,   -1,  343,   91,
   -1,   93,  321,   -1,   -1,  280,   -1,   -1,   -1,  328,
  285,  381,  331,  348,  359,  360,  335,  362,    0,   41,
   42,   43,  357,   45,  343,   47,   -1,   -1,  363,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
  359,  386,  387,   -1,   -1,   -1,  381,   -1,  418,   -1,
   -1,  396,   -1,   -1,   41,   42,   43,   -1,   45,   41,
   47,   -1,   44,   -1,  383,  410,   -1,   -1,  413,   -1,
   41,   42,   43,  280,   45,   -1,   47,   59,  285,   -1,
   -1,   -1,   -1,  418,  429,   -1,   42,   43,   -1,   45,
  435,   47,   -1,   -1,  413,  440,  280,   -1,   -1,  418,
   -1,  285,   -1,   59,   -1,   -1,  451,  452,   -1,  454,
   -1,  280,   -1,   -1,   -1,  460,  285,   -1,   -1,   -1,
   -1,  440,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,  258,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,    0,  279,  508,  509,  510,  511,  512,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,   -1,   -1,  300,   -1,   -1,   -1,   -1,  280,  306,
   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
  302,   -1,  329,   -1,   -1,  307,  308,   -1,   -1,  311,
   59,   60,   61,   62,   -1,   -1,  318,   -1,  320,  321,
    0,  348,  324,   -1,  354,   -1,  328,   -1,   -1,  331,
  357,  333,   -1,  335,   -1,   -1,  363,   -1,  340,  341,
  279,  343,   -1,   -1,   93,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,  381,   -1,    0,  359,  360,   -1,
  362,  300,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   59,
   -1,  383,   -1,  280,  386,  387,   -1,   -1,  285,   -1,
  329,  418,   -1,   -1,  396,   -1,   -1,   41,   -1,  280,
   44,   -1,   -1,   -1,  285,  305,   -1,   -1,  410,  348,
   -1,  413,  312,   -1,  280,   59,  316,  317,  357,  285,
   -1,   -1,   -1,   -1,  363,  325,  318,  429,  458,  321,
  460,   -1,   -1,  435,   -1,   -1,  328,  337,  440,  331,
   -1,  471,  381,  335,   -1,   -1,   -1,   -1,  340,  451,
  452,  343,  454,   -1,   -1,   -1,   -1,   -1,  460,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  359,  498,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,    0,   -1,   -1,   -1,  489,  490,   -1,
  492,  383,   -1,   -1,   -1,  387,   -1,  499,  500,  501,
  502,  503,  402,  505,  396,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  419,
   -1,  413,   -1,  272,   41,   42,   43,   44,   45,   -1,
   47,  280,   -1,   -1,  434,   -1,  285,   -1,  438,  288,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  440,  449,
   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
  452,   -1,  311,   -1,   -1,   -1,   -1,   -1,  468,  318,
   -1,  320,  321,   -1,    0,  324,   93,   -1,  258,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,  279,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,  258,   41,   -1,   -1,   44,   -1,
  300,   -1,   -1,   -1,   -1,   -1,  306,  376,   -1,  378,
  379,  380,   -1,   59,  383,  279,   -1,  386,  387,  264,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,  329,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,
   -1,  410,  306,   -1,  413,   -1,   -1,   -1,  348,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  356,  357,   -1,   -1,
  429,   -1,   -1,  363,   -1,  329,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  381,  451,  452,  348,  454,   -1,   -1, 1315, 1316,
   -1,  460,   -1,  357, 1321, 1322,   -1, 1324, 1325,  363,
   -1,   -1,   -1,   -1,   -1, 1332, 1333, 1334, 1335,   -1,
 1337, 1338, 1339, 1340, 1341, 1342,    0,  381,  418,   -1,
  489,  490,   -1,  492,   -1,  262,   -1,  264,  265,  429,
  499,  500,  501,  502,  503,  272,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,  418,   -1,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   -1,  311,   59,   60,   61,   62,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   93,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,  489,  490,  383,  492,   -1,  386,
  387,   -1,   -1,   -1,  499,  500,  501,  502,  503,  396,
  505,   -1,   -1,  508,  509,  510,  511,  512,  260,   -1,
   -1,   -1,  318,  410,   -1,  321,  413,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  429,   -1,  340,   -1,   -1,  343,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  301,
   -1,   -1,   -1,  359,  451,  452,   -1,  454,   -1,   -1,
   -1,  313,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  327,   -1,  383,   -1,   -1,
   -1,  387,   -1,   -1,   -1,   -1,  338,   -1,    0,   -1,
  396,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  413,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,   41,
   42,   43,   44,   45,  440,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,  452,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,  407,   -1,   -1,  302,   -1,
   -1,   -1,   -1,  307,  308,  417,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   93,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
  442,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  396,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,  287,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
  262,   -1,  264,  265,   -1,  499,  500,  501,  502,  503,
  272,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,  311,
   59,   60,   61,   62,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   93,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,  489,
  490,  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  396,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  451,
  452,   -1,  454,  499,  500,  501,  502,  503,  460,  505,
  272,   -1,  508,  509,  510,  511,  512,  301,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,  313,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,  338,   -1,  508,  509,  510,  511,
  512,   -1,  324,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,  333,   -1,  272,   41,   -1,   43,   44,   45,  341,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  360,   -1,
  362,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
   -1,   -1,  311,   -1,  376,   -1,  378,  379,  380,  318,
   -1,  320,  321,  407,   -1,  324,   93,   -1,   -1,  328,
   -1,   -1,  331,  417,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,
  359,  360,   -1,  362,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  410,   -1,   -1,  413,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   60,   61,   62,    0,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,  262,   -1,  264,  265,   -1,
  499,  500,  501,  502,  503,  272,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  328,   -1,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,
   44,   45,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   -1,  311,   59,   60,   61,   62,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   93,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  402,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,  438,   -1,  396,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,   -1,
   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  468,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,
  501,  502,  503,  460,  505,  272,   -1,  508,  509,  510,
  511,  512,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  324,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,  333,   -1,  272,   41,
   -1,   43,   44,   45,  341,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  360,   -1,  362,   -1,   -1,  302,   -1,
  328,   -1,   -1,  307,  308,   -1,   -1,  311,   -1,  376,
   -1,  378,  379,  380,  318,   -1,  320,  321,   -1,   -1,
  324,   93,  434,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  396,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,
   41,   -1,   -1,   44,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   59,   -1,
   61,  435,  460,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   58,   -1,
   91,  489,  490,   63,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  489,  490,   -1,  492,   -1,
  262,   -1,  264,  265,   -1,  499,  500,  501,  502,  503,
  272,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   40,   -1,  311,
   43,   -1,   45,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   58,  328,   -1,   -1,  331,
   63,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,  489,
  490,  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  396,  505,   -1,  258,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,  274,  443,   -1,  277,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,  429,   -1,   -1,
   -1,  460,   -1,  435,   -1,   -1,   -1,  267,  440,  300,
   -1,   -1,   -1,   -1,   -1,  306,  276,   -1,  309,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,  289,
  489,  490,   -1,  492,   -1,  295,  296,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,  314,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,  366,   -1,   -1,  400,
  370,   -1,   -1,  276,   -1,   -1,  407,   -1,  378,   -1,
  411,   -1,   -1,   -1,   -1,   -1,  289,  418,   -1,  292,
   -1,   -1,  295,  296,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,  314,   -1,   58,   -1,   -1,   -1,  417,   63,   -1,
  323,  421,   -1,  326,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,  466,   -1,   -1,   -1,  470,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,  460,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,  513,  514,  515,   -1,   -1,  421,   -1,
   -1,   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  471,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,  343,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  368,   -1,  260,   -1,   -1,   -1,   -1,
   -1,   -1,  267,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   -1,  426,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  460,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,   91,   -1,   -1,   58,   -1,   -1,
   -1,  366,   63,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,  276,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  366,    0,   -1,   -1,  370,   -1,   -1,
   -1,  258,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
  277,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,
  287,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  300,   -1,   -1,  267,   -1,  421,  306,
   -1,   -1,  309,   59,  427,  276,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
  443,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,  378,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  400,   -1,  366,   -1,   -1,   -1,  370,
  407,   -1,   -1,   -1,  411,   40,   -1,  378,   43,   -1,
   45,  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,  460,   -1,   -1,   63,  436,
   -1,   -1,   -1,   -1,  441,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
  421,   -1,  487,  488,  489,  490,  427,  492,   -1,  466,
  495,  496,  433,  470,  499,  500,  501,  502,  503,  504,
  505,  506,  443,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,
   -1,   -1,  318,  295,  296,  321,   -1,   -1,   -1,   -1,
    0,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  314,   -1,  340,  489,  490,  343,  492,   -1,
   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,  503,
   -1,  505,   -1,  359,  508,  509,  510,  511,  512,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  260,   -1,  383,  263,   59,
  386,  387,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,
  396,  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,  413,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  435,
   -1,   -1,   63,   -1,  440,   -1,   -1,   -1,  323,  421,
   -1,  326,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   58,
   -1,   -1,  262,   -1,   63,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,  276,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  289,   -1,
  340,   -1,   -1,  343,  295,  296,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  359,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  323,   63,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
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
  511,  512,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  425,   63,  427,   -1,
   -1,   -1,   -1,  323,  433,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,  323,   63,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  354,  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,  260,  370,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   58,   -1,   44,
   -1,  425,   63,  427,   -1,   -1,   -1,   -1,  323,  433,
   -1,  326,   -1,   -1,   59,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,  276,   -1,  262,   -1,   -1,
   -1,   -1,   -1,  261,   -1,   -1,   -1,   58,  289,   -1,
   -1,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,   -1,  297,
   -1,  299,  323,  301,   -1,  326,   -1,   -1,  306,   -1,
   -1,   -1,   -1,  318,   -1,  313,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,  327,
  335,   -1,   -1,  354,  355,  340,   -1,   -1,  343,   -1,
  338,   -1,   -1,   -1,   -1,  366,   -1,   40,   -1,  370,
   43,   -1,   45,   -1,  359,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,  405,  413,   -1,
   -1,   -1,  433,   -1,  412,   -1,   -1,  415,   -1,  417,
  418,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,  457,  458,  459,  460,
  461,  462,   -1,   -1,  442,   -1,   -1,  452,  273,  454,
  471,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
  295,  492,   -1,  298,  495,  496,  267,  498,  499,  500,
  501,  502,  503,  504,  505,  276,   -1,  508,  509,  510,
  511,  512,   -1,  491,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,  506,  507,
   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,  370,
   -1,  406,   -1,  276,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  423,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  480,  481,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  497,  366,   -1,   -1,   -1,  370,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,  273,   -1,   58,   -1,  277,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,  471,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
  339,   -1,  276,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,  289,    0,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,  411,   -1,   -1,   59,   -1,   61,  352,   -1,
  354,  355,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   91,   -1,   -1,
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
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  258,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,
  284,   -1,   -1,  287,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,
   -1,  443,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,  400,   -1,   -1,   -1,
   -1,   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,  466,  366,   -1,   -1,  470,  370,   -1,   -1,
  276,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
  417,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,  300,
   -1,   -1,   -1,   -1,   -1,  306,  443,   -1,  309,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,  378,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,  400,
   -1,   -1,   -1,    0,   -1,   -1,  407,   -1,   -1,   -1,
  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  267,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,
   -1,   40,   59,   -1,   43,  466,   45,   -1,  366,  470,
   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,  318,   -1,   63,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,  323,   -1,  343,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  383,  366,   -1,  386,
  387,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,  396,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,  291,   -1,   -1,   -1,
  295,   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,  339,   -1,  276,   -1,   -1,   -1,
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
  510,  511,  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  276,   -1,   -1,   40,   -1,
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
   -1,  291,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
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
  511,  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,    0,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,  276,   -1,   -1,  497,   41,   -1,
   -1,   44,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   -1,   59,   -1,   61,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   59,  326,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,    0,
  492,   -1,   -1,  495,  496,  258,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,  262,   -1,   -1,   -1,
   41,   42,   43,   44,   45,   46,   47,  300,   -1,   -1,
   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,   59,   60,
   61,   62,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,
   91,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,  339,   -1,
   -1,    0,   -1,  359,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,   -1,  383,  411,   -1,
  386,  387,  373,   -1,  375,  418,   -1,   -1,   -1,   -1,
  396,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,  413,  441,   -1,
   59,   60,   61,   62,   -1,  406,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,  423,  466,  440,    0,   -1,  470,   -1,   -1,
   -1,   -1,   91,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  272,   -1,   -1,   59,   -1,   -1,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,  497,  288,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   -1,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,    0,
   -1,   -1,   -1,  262,   -1,  396,   -1,   91,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  280,  413,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  429,   -1,
   41,   -1,   -1,   44,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   59,  318,
   -1,  452,  321,  454,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,  413,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
  429,   -1,   41,    0,   -1,   44,  435,   -1,  262,   -1,
   -1,  440,   -1,   -1,  359,   -1,   -1,   -1,  272,   -1,
   59,   -1,   -1,  452,   -1,  454,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,  318,   -1,   -1,  321,  413,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,  262,  340,  341,   -1,  343,
  435,  262,   -1,    0,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,  452,  362,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
   -1,   -1,  386,  387,   41,    0,   -1,   44,   -1,   -1,
   -1,  318,  396,   -1,  321,   -1,   -1,  318,   -1,   -1,
  321,  328,   59,   -1,  331,   -1,   -1,  328,  335,  413,
  331,   -1,   -1,  340,  335,   -1,  343,    0,   -1,  340,
   -1,   -1,  343,   -1,   -1,  429,   41,   -1,   -1,   44,
   -1,  435,  359,   -1,   -1,   -1,  440,   -1,  359,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  383,   -1,   41,  386,
  387,   44,  383,   -1,   -1,  386,  387,   -1,   -1,  396,
   -1,   -1,   -1,  262,   -1,  396,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  435,  262,   -1,   -1,   -1,  440,
    0,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  318,
   -1,  452,  321,  454,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   41,  331,   -1,   44,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   59,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
  359,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   59,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,  383,   -1,  343,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  396,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,  435,  262,   -1,  396,
    0,  440,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,  318,   -1,  452,  321,  454,  413,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   59,   -1,   -1,  335,  262,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,  435,   -1,
   -1,   41,   -1,  440,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  318,   -1,  452,  321,  454,   -1,   59,
   -1,   -1,   -1,  328,    0,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,  383,   -1,  343,  386,
  387,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  396,
   -1,   -1,   -1,   -1,  359,  328,    0,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   41,  413,  340,   44,   -1,
  343,   -1,  262,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   59,   -1,   -1,  359,   -1,  435,   -1,
   -1,  396,  262,  440,   -1,   -1,   -1,   41,    0,   -1,
   44,   -1,   -1,   -1,   -1,  452,   -1,  454,  413,   -1,
  383,   -1,   -1,  386,  387,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,  318,   -1,
  435,  321,   -1,   -1,   -1,  440,   -1,   -1,  328,   41,
  413,  331,   44,   -1,   -1,  335,   -1,  452,  318,  454,
  340,  321,   -1,  343,   -1,   -1,   -1,   59,  328,   -1,
   -1,  331,  435,   -1,   -1,  335,   -1,  440,   -1,  359,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,  262,  359,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,  262,  413,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,  318,  435,   -1,  321,   -1,   -1,
  440,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,  452,   -1,  454,  435,  340,   -1,   -1,  343,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
   -1,  321,  452,   -1,  454,  359,  262,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,  262,  359,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,  318,  383,   -1,  321,  386,  387,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,  396,   -1,   -1,  335,
  262,  435,   -1,   -1,  340,   -1,  440,  343,   -1,   -1,
   -1,   -1,   -1,  413,  318,   -1,   -1,  321,  452,   -1,
  454,   -1,   -1,  359,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,  435,  340,   -1,   -1,  343,
  440,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,  452,   -1,  454,  359,  318,   -1,   -1,  321,
  396,  271,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,  413,  340,  383,
   -1,  343,  386,  387,   -1,  271,   -1,   -1,   -1,   -1,
   -1,  301,  396,   -1,   -1,   -1,   -1,  359,  308,  435,
   -1,   -1,  312,  313,  440,  315,   -1,   -1,  318,  413,
   -1,   -1,   -1,   -1,   -1,  301,  452,   -1,  454,   -1,
   -1,  383,  332,   -1,  386,  387,  312,  313,  338,  315,
   -1,  435,  318,   -1,  396,   -1,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  332,   -1,  452,   -1,
  454,  413,  338,   -1,   -1,   -1,   -1,   -1,   -1,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,  301,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  312,  313,   -1,  315,   -1,   -1,  318,  417,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  332,   -1,  432,   -1,   -1,   -1,  338,   -1,   -1,   -1,
   -1,  417,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,   -1,  453,   -1,   -1,  432,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,  273,   -1,   -1,  451,  277,  453,   -1,   -1,
   -1,   -1,   -1,  284,  460,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,  295,   -1,  398,  298,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  489,  490,  417,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
  432,   -1,  508,  509,  510,  511,  512,   -1,  339,   -1,
  442,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,  453,  353,  273,   -1,   -1,   -1,  277,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,  295,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  406,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,  423,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   -1,  406,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,  500,
  501,  502,  503,  423,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,
  500,  501,  502,  503,  460,  505,   -1,   -1,  508,  509,
  510,  511,  512,  469,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,  483,  484,   -1,
   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,   -1,  499,  500,  501,  502,  503,  504,  505,
  506,   -1,  508,  509,  510,  511,  512,
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
"event_argument_opt : ',' value",
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
			)) 
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
if ( c == '(' && !brace_analysis ) {
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
			if (prev_keyword == '(') {			
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
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 364:
{ yyval = yyvsp[-1]; }
break;
case 365:
{ lex.beginning = lex_position(); }
break;
case 366:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 367:
{ lex.beginning = lex.last_token; }
break;
case 368:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 369:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 370:
{ yyval = 0; }
break;
case 371:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 372:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 373:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 374:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 375:
{ yyval = NULL; }
break;
case 376:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 395:
{ yyval = NULL; }
break;
case 396:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 397:
{ yyval = yyvsp[0]; }
break;
case 398:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 399:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 400:
{ yyval = yyvsp[0]; }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 403:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 404:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 408:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 409:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 413:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 417:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 418:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = yyvsp[0]; }
break;
case 422:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 423:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 425:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 449:
{ yyval = NULL; }
break;
case 450:
{ yyval = NULL; }
break;
case 451:
{ yyval = NULL; }
break;
case 452:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 453:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 454:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 455:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 456:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 457:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 458:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 459:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 461:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 462:
{ yyval = yyvsp[0]; }
break;
case 463:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 464:
{ yyval = yyvsp[0]; }
break;
case 465:
{ yyval = yyvsp[0]; }
break;
case 467:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 468:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 470:
{ yyval = NULL; }
break;
case 472:
{ yyval = NULL; }
break;
case 473:
{ yyval = yyvsp[0]; }
break;
case 474:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 477:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 490:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 491:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 493:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 494:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 495:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 501:
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
case 502:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 503:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 504:
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
case 505:
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
case 506:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 512:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 513:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 514:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 515:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 516:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 517:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 518:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 534:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 535:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 536:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 537:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 538:
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
case 539:
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
case 542:
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
case 543:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 546:
{ yyval = yyvsp[-1]; }
break;
case 547:
{ yyval = 0; }
break;
case 551:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 552:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 553:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 557:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 558:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 559:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 560:
{ yyval = 0; }
break;
case 561:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 564:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 565:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 568:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 569:
{ yyval = (DSQL_NOD) NULL; }
break;
case 571:
{ yyval = (DSQL_NOD) NULL; }
break;
case 572:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 574:
{ yyval = (DSQL_NOD) NULL; }
break;
case 576:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 581:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 582:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 583:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 584:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 585:
{ yyval = yyvsp[0];}
break;
case 587:
{ yyval = yyvsp[0];}
break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 590:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 594:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 595:
{ yyval = 0; }
break;
case 596:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 597:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 598:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 599:
{ yyval = (DSQL_NOD) 0; }
break;
case 600:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 601:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 603:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 604:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 605:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 606:
{ yyval = 0; }
break;
case 608:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 609:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 610:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 612:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 613:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 614:
{ yyval = make_list (yyvsp[0]); }
break;
case 615:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 619:
{ yyval = 0; }
break;
case 620:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 621:
{ yyval = 0; }
break;
case 622:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 623:
{ yyval = 0; }
break;
case 624:
{ yyval = yyvsp[-1]; }
break;
case 625:
{ yyval = 0; }
break;
case 626:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 627:
{ yyval = 0; }
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
{ yyval = 0; }
break;
case 632:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 633:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 634:
{ lex.limit_clause = true; }
break;
case 635:
{ lex.limit_clause = false; }
break;
case 636:
{ lex.first_detection = true; }
break;
case 637:
{ lex.first_detection = false; }
break;
case 638:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 639:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 640:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 641:
{ yyval = 0; }
break;
case 642:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 643:
{ yyval = yyvsp[-2]; }
break;
case 644:
{ yyval = yyvsp[-1]; }
break;
case 645:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 646:
{ yyval = yyvsp[-1]; }
break;
case 647:
{ yyval = yyvsp[0]; }
break;
case 648:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 649:
{ yyval = 0; }
break;
case 650:
{ yyval = make_list (yyvsp[0]); }
break;
case 651:
{ yyval = 0; }
break;
case 653:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 655:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 656:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 657:
{ yyval = make_list (yyvsp[0]); }
break;
case 659:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 662:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 663:
{ yyval = yyvsp[-1]; }
break;
case 664:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 665:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 666:
{ yyval = make_list (yyvsp[-1]); }
break;
case 667:
{ yyval = NULL; }
break;
case 669:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 673:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 674:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 675:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 678:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 680:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 681:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 682:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 683:
{ yyval = make_list (yyvsp[0]); }
break;
case 684:
{ yyval = 0; }
break;
case 686:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 691:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 695:
{ yyval = yyvsp[0]; }
break;
case 696:
{ yyval = 0; }
break;
case 697:
{ yyval = yyvsp[0]; }
break;
case 698:
{ yyval = 0; }
break;
case 699:
{ yyval = yyvsp[0]; }
break;
case 700:
{ yyval = 0; }
break;
case 701:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 702:
{ yyval = 0; }
break;
case 703:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 704:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 705:
{ yyval = 0; }
break;
case 706:
{ yyval = 0; }
break;
case 708:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 709:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 712:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 713:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 714:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 715:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 717:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 719:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 725:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 729:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 730:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 733:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 736:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 738:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 739:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 743:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 744:
{ yyval = yyvsp[0]; }
break;
case 746:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 749:
{ yyval = NULL; }
break;
case 750:
{ yyval = make_list (yyvsp[-1]); }
break;
case 752:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 754:
{ yyval = NULL; }
break;
case 755:
{ yyval = make_list (yyvsp[-1]); }
break;
case 757:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 759:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 761:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = yyvsp[-1]; }
break;
case 777:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 828:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
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
case 839:
{ yyval = make_list (yyvsp[-1]); }
break;
case 840:
{ yyval = yyvsp[-1]; }
break;
case 841:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 842:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 852:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 853:
{ yyval = yyvsp[0]; }
break;
case 854:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 855:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 856:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 857:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 858:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 859:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 860:
{ yyval = yyvsp[-1]; }
break;
case 861:
{ yyval = yyvsp[-1]; }
break;
case 865:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 866:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 867:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 868:
{ yyval = yyvsp[0]; }
break;
case 869:
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
case 870:
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
case 871:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 872:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 874:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 876:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 880:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 881:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 883:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 884:
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
case 885:
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
case 886:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 887:
{ yyval = make_parameter (); }
break;
case 888:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 889:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 890:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 894:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 895:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 896:
{ yyval = yyvsp[0]; }
break;
case 897:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 899:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 901:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 902:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 903:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 905:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 906:
{ yyval = yyvsp[0];}
break;
case 911:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 912:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 913:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 915:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 916:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 917:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 918:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 920:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 922:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 923:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 925:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 926:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 928:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 929:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 932:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 933:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 937:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 939:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 940:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 941:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 943:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 954:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 957:
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
