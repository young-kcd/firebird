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
  312,  312,  313,  313,  317,  317,  318,   65,   65,   65,
  320,  320,  319,  319,  314,  314,  323,  323,  315,  315,
  316,  327,  328,  329,  321,  322,  324,  324,  324,  324,
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
    1,    3,    2,    3,    0,    1,    3,    4,    4,    1,
    3,    4,    3,    0,    1,    3,    3,    1,    1,    0,
    1,    1,    4,    0,    3,    0,    2,    0,    2,    0,
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
    0,  109,    0,  110,    0,    0,    0,  611,    0,    0,
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
    0,  609,  185,    0,    0,  129,  464,  172,  173,    0,
  462,    0,    0,    0,    0,  465,    0,  460,    0,    0,
  410,  412,  411,    0,  413,    0,  407,  218,    0,    0,
  271,    0,    0,    0,    0,  419,  420,    0,  449,    0,
    0,    0,    0,    0,    0,    0,  377,  376,  468,    0,
    0,  149,    0,    0,  158,    0,  157,    0,  164,  147,
    0,  162,  165,  166,  154,    0,    0,  503,  539,  506,
    0,  540,    0,    0,  507,    0,    0,    0,  530,  543,
  502,  525,  504,  505,  500,  487,  366,  485,  486,  488,
    0,    0,  497,  498,  499,  501,    0,    0,    0,    0,
    0,  112,    0,  113,    0,  124,    0,    0,  201,    0,
    0,  364,    0,  220,    0,    0,   83,    0,    0,  742,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  763,  765,  774,  777,  778,  779,  780,  781,  782,
  783,  784,  785,  786,    0,    0,    0,  883,    0,    0,
    0,    0,    0,    0,  884,  885,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  324,
  977,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  749,   28,    0,    0,   44,   72,    0,    0,
   26,    0,  755,    0,    0,    0,  718,    0,    0,    0,
    0,    0,  560,    0,  633,    0,    0,    0,    0,    0,
  970,  654,  551,    0,  903,  550,    0,  584,    0,  592,
  588,  587,    0,    0,    0,  603,  591,    0,    0,  728,
  729,    0,    0,    0,  364,    0,    0,    0,  625,  629,
    0,    0,  130,    0,    0,    0,    0,    0,    0,  366,
  409,    0,  366,  367,  414,  495,    0,  273,    0,  267,
  269,  364,  240,  962,    0,    0,  366,    0,    0,  209,
  364,    0,    0,    0,    0,  245,  239,  242,  243,  244,
  418,  454,  453,  417,  416,    0,    0,    0,    0,    0,
    0,  375,    0,  366,    0,  155,  159,    0,  163,    0,
    0,  526,  527,  544,    0,  541,    0,    0,  533,  531,
  532,    0,    0,    0,    0,  496,    0,    0,    0,    0,
  534,    0,    0,  364,    0,  200,    0,  203,  205,  206,
    0,    0,    0,    0,    0,  220,   78,    0,    0,   77,
  514,  958,  515,    0,    0,    0,    0,  764,  776,    0,
    0,  834,  835,  836,    0,  768,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  946,
  947,  948,  949,  950,  951,  952,  953,    0,    0,    0,
    0,    0,    0,  939,    0,    0,    0,    0,  859,  326,
  860,  927,    0,    0,  855,  960,    0,    0,    0,    0,
    0,  668,    0,  865,  759,  758,  751,    0,    0,    0,
    0,    0,    0,    0,    0,   53,   61,   63,    0,    0,
   70,    0,    0,    0,  754,    0,    0,    0,    0,    0,
    0,    0,    0,  642,  645,    0,    0,    0,    0,  659,
  660,    0,    0,  652,  655,  552,  904,  593,  602,  597,
  596,    0,  607,  762,  731,  732,  364,    0,    0,  635,
  617,  616,    0,  181,  175,  182,    0,  178,    0,    0,
  177,    0,  188,  169,  168,  902,  171,  170,  466,    0,
    0,    0,  408,  272,    0,  211,  366,  213,    0,  367,
  208,  214,    0,    0,    0,    0,  421,  422,  366,  364,
  367,    0,    0,    0,  393,    0,  196,  197,  156,    0,
    0,    0,    0,    0,    0,  542,    0,    0,  366,  367,
    0,  491,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  198,    0,    0,    0,    0,
  353,    0,   82,    0,    0,    0,    0,   87,   84,    0,
    0,    0,  725,  963,    0,    0,  769,    0,    0,  775,
    0,    0,    0,    0,    0,  812,  811,    0,    0,    0,
  819,  837,  829,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  767,    0,    0,    0,  220,    0,  910,
    0,    0,  925,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  935,    0,  945,    0,    0,
    0,    0,    0,    0,  926,  871,   65,   57,   58,   64,
   59,   60,    0,    0,   22,    0,    0,    0,   42,    0,
    0,    0,  736,  756,    0,  719,    0,  735,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  674,    0,
    0,    0,    0,    0,  599,  600,  604,    0,  199,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
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
   31,   32,   33,   34,   35,   36, 1546,   38, 1547,  640,
  120,  456,  954, 1185,  121,  706,  955,  122,  460,  711,
  146,  123,  124,  257,  977,  136,  956,  957,  197,  226,
  125,  958,  126,  462,   82,  237,  374,  421,  635, 1086,
  422,  859,  606, 1510,  836,  636,  637,  638, 1088,  238,
 1374,   74,  187,   75,  233, 1826, 1297,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  622,  624,  453,  258,  786,  540,  650, 1023,  367,
  627, 1074, 1289,  573,  735, 1049,  525,  526,  555,  359,
  607, 1060, 1440, 1569, 1232, 1441, 1442, 1443, 1444, 1445,
  651,  214,  206,  355,  580,  356,  357,  581,  582,  583,
  584, 1264, 1017,  528,  529, 1007, 1005,  530, 1011, 1006,
 1222, 1403, 1012, 1013, 1227,  369,  847,  848,  849,  850,
  788, 1030, 1418,  608,  790,  424,  852,  935,  549, 1346,
 1233,  425,  426,  427,  428,  429, 1419, 1420,  558, 1575,
 1725, 1250,  796,  797,  783,  798,  799,  800, 1726, 1727,
 1803,  337,  553, 1241, 1548,  193,  550,  551, 1242, 1243,
 1413, 1702, 1549, 1550, 1551, 1552, 1666, 1553, 1554, 1555,
 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565,
  430, 1788, 1649, 1712, 1792, 1710,  431,  690,  432,  433,
 1093, 1667, 1719, 1720, 1758, 1079, 1295, 1457, 1080, 1081,
  285,  487, 1598,  378, 1214, 1396, 1683, 1684, 1685, 1686,
 1212,  346,  569,  814, 1262,  570,  812,  221,   59,  341,
  198,  161,  324,  184,  331,  190,  774,  771,  332,  342,
  804,  343,  565, 1041,  185,  434,  325,  537,  571, 1263,
   96,  196,  242,  609,  610,  776, 1061,  777, 1062, 1063,
  613,  614,  615,  616, 1054,  863,  617,  618,  619,  829,
  620, 1512,  826,   40,   41,   42,   43,   44,   45,  155,
  515,   61,  478,  201,  348,  297,  298,  299,  300,  301,
  302,  303,  304,  305,  741,  501,  992, 1217,  502,  503,
  746,   46,  160,  320,  522,   47,  517,  518, 1001, 1401,
 1221, 1526,  759,  140,  728, 1523,  717,  481,  287,  141,
  142,  435,  286,  488,  489,  490,  732,  978,  979,  980,
  981, 1206,  436, 1168, 1514, 1515,  438, 1517,  439,  440,
  441, 1740, 1639, 1741, 1742, 1743, 1786, 1827,  468, 1197,
   48,   49,  379,   50,   51,  509,  510,  963, 1193, 1376,
 1380,  469,  714,  211,  652,  653,  877,  878,  654,  655,
  656,  657,  658,  659,  660,  661,  662,  663,  664, 1318,
 1109, 1111, 1112,  934,  689,  442,  443,  444,  445,  446,
  235,  447,  448,  918,  449,  450,  451,  452,  680,  922,
 1169, 1164,  681,  128,
};
static short yysindex[] = {                                   6532,
 1206, -171, 2891,  156,   70,   73, 3308,  113, 4321,   40,
  518, 3351, -171,  941,  449, 5564, 1064, 5564,  385,    0,
  765,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  627,    0,    0,    0,    0,
    0,    0, 7732, 5564, 5564, 5564, 5564, 5564,    0,    0,
  427, 1119, 5564, 5564, 5564,  684, 5564,  591, 5564, 5564,
    0, 5564, 5564,    0,   47,    0,  619,    0,    0,  775,
 5564,    0, 5564, 5564, 5564,  821, 5564, 5564, 5564, 5564,
  591, 5564, 5564, 5564, 5564,    0, 5564,  713,    0,    0,
    0, 1172,    0, 1172,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  897,
  933,  201, 1128,    0,    0,    0,    0,    0, 5564, 5564,
 5564,  929, 1041, 1075,  134, 1655, 1067,  136,  229,  567,
 1029,    0, 5564, 1214, 1186,    0, 1137,    0, 5564, 5564,
 5564, 5564,    0,    0,    0, 5564,    0, 1323,  -11, 1286,
   78,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  716,    0,    0, 1119,    0,  674,    0,
    0, 1581,    0,    0, 1307,    0,  794,    0,    0, 1227,
    0,    0, 1208,    0,    0, 1639,    0, 1435,    0,    0,
    0, 1119,    0,    0,    0,  798, 1581,    0,  511,    0,
    0, 1388,    0, 1400,    0, 1172,    0,    0,    0,    0,
    0,    0, 1376,    0,    0, 1271,    0, 1405,    0, 1309,
    0,    0,    0, 1271,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8463,    0, 5564,    0,    0,    0, 5129,
 1373, 5882, 5564,  883, 1427, 1748, 1478, 1485, 5129, 1412,
 5882, 1424, 1445, 1333,    0, 9757,    0,    0,    0,    0,
    0,    0,    0,    0, 7653,    0,    0,    0, 1394, 5564,
 1498, 1425,  529, 5564, 1431,    0,    0, 1186,    0,    0,
    0,    0,    0,    0,    0, 5564,    0,    0, 1581,    0,
 1388,    0, 1172,    0, 1493, 9757, 1462,    0, 1453, 1459,
 1115, 1557,  931,   78,    0, 1637, 1296,    0, 5564,    0,
  716,    0,    0,    0,    0, 5564, 1521, 4385, 1482, 4805,
 1909,    0, 7732,    0,    0, 1090,    0,    0,    0, 1894,
 1894, 1119, 1587, 1119, 1505, 1639,    0,    0, 9027,    0,
 5564, 5564,    0, 1521,    0,    0, 1679,  520, 1929, 5564,
 1706, 5564,    0,    0, 1936,   91, 5165,    0,    0,    0,
 1938, 1940, 1942, 1523,    0,    0, 1947,    0, 1952,    0,
 1956, 1960,    0, 1979,    0,    0,    0,    0,    0, 1985,
 1541, 1565,    0,    0,    0,    0,    0,    0,    0, 1988,
    0,    0, 8527, 1991, 1995,    0, 9757, 9757, 7900, 5564,
 1996,    0, 2666,    0,    0,    0,    0,    0,    0,    0,
    0, 1970, 1954,    0,    0, 1997,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1635,    0, 5564, 1610,    0, 5564, 5564,  137,
    0,    0,    0,    0,    0, 5564, 5564,  -42,    0, 5564,
    0, 1727, 5564, 2004, 5129, 1649,    0, 5564, 8767, 1905,
    0,    0, 9757,    0, 2666,    0, 1743, 2012,    0, 3836,
    0,  101,    0,  655,    0,  -84,    0,  -84,    0,    0,
 2020,    0,  160, 1638,    0,    0,    0, 2024,  126, 2015,
 1521, 2040, 1817,    0,    0, 1353, 2039,    0,    0, 1702,
 1589,    0,    0,  202, 1750,    0,    0,    0,    0, 1119,
    0, 1894, 1894, 1894, 1894,    0, 2055,    0, 1829, 1830,
    0,    0,    0, 1815,    0, 9027,    0,    0, 9027, 1670,
    0, 1581, 1857, 5564,11926,    0,    0,  688,    0, 5564,
  904, 1307, 1721, 1680, 1653,    0,    0,    0,    0,  841,
 1728,    0, 1672,  591,    0, 1119,    0, 1847,    0,    0,
 1505,    0,    0,    0,    0,  108, 1685,    0,    0,    0,
 1688,    0, 1737, 2097,    0, 1792, 2100,   34,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2052,  206,    0,    0,    0,    0,   71, 2105, 2106, 2100,
 1581,    0, 1832,    0, 1884,    0, 1119, 1119,    0, 4385,
  794,    0, 1771,    0, 1747, 2112,    0, 4903, 1382,    0,
 1770, 1789, 2122, 5726, 2124,    0,    0,    0, 4907, 5770,
  494,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  949, 8831,  125,    0, 5564, 9757,
 1080, 1147, 1158, 2890,    0,    0, 9757, 5812, 2666, 1731,
    9, 9757, 8831,  800,  800,  941, 2032, 1716, 2142,    0,
    0, 9061, 5564, 9757, 9757, 9757, 9757, 9757, 8831, 9757,
 1346, 5564,    0,    0, 6519, 1751,    0,    0, 1722, 5882,
    0, 1867,    0, 1788,  941, 2149,    0, 1867, 6519, 1872,
 1875, 5564,    0, 2032,    0, 2086,  920, 1755, 8831, 5564,
    0,    0,    0,  604,    0,    0,   54,    0, 1749,    0,
    0,    0, 5564,  -25, 5564,    0,    0, 5564, 5564,    0,
    0, 8831, 1949, 4385,    0, 1700, 9757, 5564,    0,    0,
 1119, 1119,    0,  -24, 1672, 1672, 1767,  591,  931,    0,
    0, 2189,    0,    0,    0,    0, 1953,    0, 5564,    0,
    0,    0,    0,    0, 1958, 2197,    0, 1944, 1959,    0,
    0, 1901, 1902, 1172, 1977,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  591, 5564,11997, 1866, 1877,
 1882,    0,  591,    0, 1389,    0,    0, 1849,    0,   63,
 1855,    0,    0,    0,  591,    0, 2097,  158,    0,    0,
    0, 1972,  158, 1861,  158,    0, 2244,  591,  591,  591,
    0, 1521, 5564,    0, 1925,    0, 1820,    0,    0,    0,
 1090, 1870, 5564, 1826,   98,    0,    0, 2248, 2017,    0,
    0,    0,    0,   91, 5564, 1879, 6042,    0,    0, 1879,
 5726,    0,    0,    0, 3881,    0,  139,  506, 9757, 9757,
 6296, 2249, 1152, 6393, 9757, 6647, 1004, 6799, 6908, 9159,
 7162, 7314, 7423, 5812, 5812, 9757, 9757, 2028, 9757, 2257,
 9757, 2260, 2110, 9757, 9757, 9757, 9757, 9757, 9757,    0,
    0,    0,    0,    0,    0,    0,    0, 1981, 1242,    2,
 9757,  810, 8831,    0, 5812, 1780, 2263,  567,    0,    0,
    0,    0, 2666, 1862,    0,    0, 2029,  800,  800,  926,
  926,    0, 1071,    0,    0,    0,    0, 5564, 5564, 5564,
 5564, 5564, 5564,  168, 2266,    0,    0,    0, 6519, 1926,
    0,  -38, 1946, 5564,    0,  567, 8831, 1946, 2270, 2266,
 6519, 5101, 1998,    0,    0,  920, 2275, 2282, 1042,    0,
    0, 5812, 2008,    0,    0,    0,    0,    0,    0,    0,
    0,  936,    0,    0,    0,    0,    0, 1881, 1870,    0,
    0,    0, 2283,    0,    0,    0, 1892,    0, 1894, 2053,
    0,  -24,    0,    0,    0,    0,    0,    0,    0, 2297,
 5812, 3931,    0,    0, 2044,    0,    0,    0, 3931,    0,
    0,    0, 1172, 1172, 1838, 2304,    0,    0,    0,    0,
    0,  -48,  549,   82,    0, 2087,    0,    0,    0, 5564,
  156, 1893, 1928, 1953, 2312,    0, 1672, 1898,    0,    0,
 1193,    0, 2296, 5564, 1279,  591, 2318, 2319, 2322, 2101,
  794, 2044, 1894, 1750, 4385,    0, 1728,  567, 1914, 1930,
    0,  395,    0,  591, 1975, 2065, 4903,    0,    0,  591,
 1885, 2069,    0,    0, 2335, 2340,    0, 5812, 5812,    0,
 5812, 5812, 2840, 2666, 2348,    0,    0, 2666, 2353, 9389,
    0,    0,    0, 2030, 2358, 2666, 2359, 1854, 2361, 2666,
 2363, 9757, 9757, 2249, 9757, 9453, 2364, 2666, 2366, 2367,
 2666, 2368, 9757, 2666, 2369, 2666, 2371, 2372, 2666, 2378,
 2379, 2666, 2380,    0, 2159, 2125, 2288,    0, 2342,    0,
 2538, 9757,    0, 2565, 2572, 2588, 2629, 2702, 2791, 9757,
  591, 8831, 2666, 1993, 8831,    0, 9757,    0, 2118,  542,
 9757, 8831, 9757, 9757,    0,    0,    0,    0,    0,    0,
    0,    0, 2102, 6519,    0, 6519,  168, 2266,    0,  211,
  211,  154,    0,    0, 7653,    0, 1900,    0, 2270, 2266,
 2388, 5101, 1042, 2392, 8831, 5564,  920, 2047,    0, 2048,
 2049, 2095, 2165, 2109,    0,    0,    0, 2044,    0, 1914,
  476, 2021, 1119, 2056, 1672, 2054, 1672,    0, 5812,  486,
 1719,    0,    0,    0,    0,    0,    0,    0,    0, 2003,
 2179, 2044,    0, 9757,    0, 2166, 2050, 1838,   47,    0,
 5812,    0,    0,    0, 2070, 2072, 2074, 2080, 2083, 2089,
    0,    0,    0,    0,    0, 2414,  156,    0, 1767,    0,
    0,  158,    0, 3931, 2098,  158,    0,  158,    0, 1953,
 2437,    0,    0,    0,    0, 1090, 2179, 1672,    0, 1750,
    0,    0, 7653, 2203,    0,    7,    0, 2283,    0, 2442,
    0, 2443, 1119,  -36, 2447,    0, 1119,    0,    0,    0,
 2159,    0, 2159, 9757, 2066, 2066,  941, 2448, 1920,    0,
 2066, 2066, 9757, 2066, 2066, 3227, 2666,    0, 1943, 9757,
 2666, 2066, 2066, 2066, 2066, 2666, 2066, 2066, 2066, 2066,
 2066, 2066,    0,    0, 9027, 2449,    0,    0, 3043,    0,
    0,    0,    0,    0,    0, 3071,  121,    0, 8831, 2184,
 2059,    0, 8831, 3098, 1922, 1620, 2666, 2113,    0,    0,
    0,    0,    0,    0,    0, 2061,    0,    0,    0,    0,
 1743, 8831,    0, 5101, 2388,    0, 1969,    0, 1042,    0,
    0,    0,  920, 6136, 5812, 2103, 2179,    0,    0,    0,
    0, 1894,    0,    0, 1119, 1389,    0,    0,  620,    0,
    0,    0, 5564,11752,    0,    0, 2666, 2221, 2166,    0,
 1154, 5564,    0, 2167,  658, 2060, 2168, 2063, 2207, 2169,
 2217, 2044,    0,    0, 2475,    0, 2478,    0, 2172, 2221,
 2098,    0,    0,    0,    0,    0,    0,    0,    0, 2044,
 1728,    0, 1389, 2087, 2200, 2136,    0, 1870,    0,    0,
    0, 2161,  -80,    0, 1953, 2170, 2666, 2497, 2499,  567,
    0,    0, 2501, 2503, 2666, 2504, 2508, 9757, 9757, 2666,
 2510, 2512, 2516, 2519, 2522, 2523, 2524, 2525, 2526, 2529,
    0,    0,    0,    0,  591,    0,    0,    0, 8831,    0,
    0,    0, 1755,    0,  211, 1755,    0,    0, 1365,    0,
 2291,    0, 2536, 2542,    0,    0,    0,    0,    0,    0,
  494, 1022,    0,    0,    0,    0, 1672,    0,    0,    0,
    0, 9027, 1601,  592, 2528,  501, 2549, 2252, 9757, 2537,
 2563, 2545, 2546, 2550, 2557, 2278, 2562,    0,    0,    0,
 2315,10319, 2573,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2593, 5564,    0,    0,
 2284, 2293, 5564, 1838,    0,    0, 1172, 5564,    0,    0,
    0,    0,    0,    0,    0, 2179,    0,    0,    0,    0,
    0, 2179,    0,    0,    0,    0,  979, 1755,    0,    0,
 9757, 1119,    0,    0,    0, 1119,    0,    0, 9757,    0,
    0,    0,    0, 2666, 2666,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2598,    0, 2008,    0, 2008,
 5812, 5564,  648, 6136,    0,    0, 2272,    0, 2602,    0,
    0,    0,  228,    0, 9693, 5564,    0,    0, 9757,    0,
 -146, 2303, 5812, 2666,    0, 5812,    0,    0,    0,    0,
 1418,    0,    0,  746,    0, 2194,    0,    0,    0,    0,
    0, 1838, 1172,    0, 2264,    0,    0,    0,    0, 2087,
    0,  979, 2604, 1042,    0,    0, 2008, 2666,    0,    0,
 1620,    0, 2109, 2109,  494,    0,    0,    0, 6543, 3931,
    0, 2587,    0, 2206, 8463, 1395, 9757, 1418,  685, 2592,
  725,  855,    0,    0,    0, 5564, 5564,  156,  184,    0,
    0,    0, 2264,  557, 1838, 2269, 2274,    0,    0, 1042,
 2617,  979, 2317, 2109, 2620, 1755, 2103, 2103, 5564,    0,
 2621, 2619, 1256,    0,    0,    0, 2254, 1418,    0, 1774,
  653, 2230,    0, 2375,    0, 1418,    0,    0,    0,    0,
11752,  746, 1838,  981,  981,    0, 2381,    0, 2231,    0,
    0,    0, 1042,  979,    0,    0, 2008, 2298, 2298,    0,
    0, 6543, 2646,    0, 5564,    0,  843, 2630, 1403, 1418,
 2395, 2386,11752,11752,    0,    0,    0,    0,    0, 2228,
  507,    0,    0,    0, 1407, 2109,    0,    0,    0, 5564,
    0, 1418, 2648,    0,    0,  372, 5564,11752, 2389,    0,
    0,    0,    0, 5812, 2103, 2654, 2658, 1974,11752,    0,
    0,11752,  494, 2298, 5564,    0,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  250,  930,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  164, 5229,    0,    0,    0,    0,    0,    0,
 2703,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  193,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  954,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2365,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2323,    0,    0,
    0,  232,    0,  232,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2327,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1095,    0,    0, 7986,
 5496,    0,    0,    0, 1122,    0,    0,    0, 2241,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  128,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    4,    0,    0,    0,    0,  702,    0,    0, 1169,
    0,    0,    0,    0,    0, 1185,    0,12253,    0,    0,
    0,    0,    0,    0,    0,    0,    4,    0,  472,    0,
    0, 2679,    0,    0,    0, 2458,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1275,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1315,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  727,    0,    0,    0,    0,
    0,    0,    0, 6225,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  227,    0,    0, 1357,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    4,    0,
 2679,    0, 2458,    0, 1386,    0,    0,    0,    0,  315,
    0,    0,    0, 1393,    0, 2450,    0,    0,    0,    0,
 1436,    0,    0,    0,    0,    0, 2465,  863,12236,    0,
 1512,    0,    0,    0,    0,  370,    0,    0,    0, 2273,
 2273,    0,    0,    0, 1544, 2531,    0,    0,    0,    0,
    0,    0,    0, 2465,    0,    0, 1325,    0,    0,    0,
    0,    0,    0,    0,12171, 2345,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1709,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2215,
    0,    0,    0, 2468, 2721,    0,    0,    0,    0,    0,
    0,    0,  998,    0,    0,    0,    0,    0,    0,    0,
    0,  243, 2974, 1962,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1546,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1553,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  819,    0,    0, 2415,    0,  189,
    0,    0,    0,    0,    0, 1277,    0, 1277,    0,    0,
  857,    0, 1132, 1297,    0,    0,    0,    0, 1275,    0,
 2465,    0,    0,    0,    0,  116,  306,    0,    0,  163,
    0,    0,    0, 3079,  453,    0,    0,    0,    0,    0,
    0, 2273, 2273, 2273, 2273,    0,  531,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2472,    0,    0,    0,    0,    0, 2450,    0,    0,
 1477,    0, 2267,    0,    0,  695,    0,    0,    0,    0,
  338,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1609,    0,    0,    0,    0, 1916, 5045,    0,    0,    0,
10248,    0,    0,10275,    0,    0,10321,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2147, 2400,    0,    0,    0,    0,10335,10236,    0,10321,
    4,    0,    0,    0,    0,    0,    0,    0,    0,  863,
 1091,    0,    0,    0,    0, 2328,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10527,10722,10875,    0,    0,
  805,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9987,    0, 9987,    0,    0,    0,
 9987, 9987, 9987,    0,    0,    0,    0,    0, 2290,    0,
    0,    0,    0, 3944, 4176, 8233, 2698,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  333,    0,    0, 5229,    0,    0,  333,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  166,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  959,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  863,    0,  214,    0,    0,    0,    0,
    0,    0,    0, 3322,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1326,    0,    0,    0,
    0,    0,    0,    0, 2710,    0,    0, 2597, 2653,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  657,  706,
  781,    0,    0,    0, 1999,    0,    0,    0,    0,    0,
 3133,    0,    0,    0,    0,    0,10275,    0,    0,    0,
    0,  129,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2465,    0,    0,  151,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12190,    0,    0,    0,  247,    0,
    0,    0,    0, 2445,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 9987,    0,    0,
    0,    0, 1590,    0,    0,    0, 3206, 4429, 4661, 3459,
 3691,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1622, 1628,    0,    0,    0,    0,    0,
    0,    0, 1658,    0,    0, 7986,    0, 1658, 1666, 1684,
    0,    0,    0,    0,    0,    0, 4735,  127,  584,    0,
    0,    0,  672,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  200,    0,    0,    0,    0,    0, 2273, 2294,
    0, 3528,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2480,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1475,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1720,    0,    0,    0,    0,
    0,    0,    0, 1531,    0,    0,    0,    0,    0,    0,
    0,    0, 1324,    0,    0,    0,    0,    0,    0,    0,
 1091, 2480, 2273, 1734,  863,    0, 2491, 7986, 1736,  133,
    0,    0,    0,    0,12190,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 5576,    0,    0,    0, 6027,    0,    0,
    0,    0,    0,    0,    0, 6246,    0, 7638,    0,10690,
    0,    0,    0,    0,    0,    0,    0,10939,    0,    0,
11028,    0,    0,11124,    0,11184,    0,    0,11204,    0,
    0,11264,    0,    0, 5424,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2324,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1622, 1740,    0, 2321,
  355,    0,    0,    0,    0,    0,    0,    0, 1745, 1782,
 1785,    0, 2417, 1038,    0, 3771,    0, 2418,    0, 2426,
 2429,    0,    0,  389,    0,    0,    0, 2480,    0, 1736,
    0, 2320,    0,    0,    0, 2300,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6738,
    0, 2506,    0,    0,    0, 2493,    0, 1475, 2365,    0,
    0,    0,    0,    0,  786,  792,  806,  808,  849,  852,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  743,    0,    0,    0,    0, 2880,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1791,
    0,    0,    0,    0,    0,    0,    0, 1810,    0,    0,
    0,    0,    0,  165,    0,    0,    0,    0,    0,    0,
  289,    0,  294,    0,    0,    0, 8233,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11336,    0,11372,    0,
11432,    0,    0,    0,    0,11492,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1819,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1843,    0,    0,    0, 1258,    0,
    0,    0,    0,    0,    0,  548,    0,    0,    0,    0,
    0, 2273,    0,    0,    0, 2067,    0,    0,    0,    0,
    0,    0,    0, 2466,    0,    0, 2743, 1570, 2746,    0,
 2450,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2480,    0,    0,    0,    0,    0,    0,    0, 1852,
  818,    0,    0,    0,    0,    0,    0,    0,    0, 2480,
 2491,    0,  318,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  382,    0,11542,    0,    0, 9987,
    0,    0,    0,    0,11602,    0,    0,    0,    0,11685,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  759,    0,  355, 1229,    0,    0, 2417,    0,
 1558,    0,    0,  951,    0,    0,    0,    0,    0,    0,
  287, 2745,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1380, 1480,    0,    0,    0,    0,    0,    0,    0,
    0, 2483,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2906,    0,    0,  161,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  149,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11757,11793,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  588,    0, 1254,
    0,    0,    0,    0,    0,    0, 2753,    0,    0,    0,
    0,    0, 2735,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2736,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2489,    0,    0,    0,    0,
    0, 2906,  130,    0,  172,    0,    0,    0,    0,    0,
    0,    0,  381,  750,    0,    0,  198, 2743,    0,    0,
    0,    0,  400,  341, 4256,    0,    0,    0, 2745,    0,
    0,    0,    0,    0,   76,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  185,    0, 1475,  263,  552,    0,    0, 2417,
 1545,    0,    0,  224,    0,  759,  115,  236, 1359,    0,
    0, 2757,    0,    0,    0,    0, 2747,    0,    0,    0,
 2498,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2906,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1207,    0,    0,    0,  588, 2763, 1061,    0,
    0, 2745,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2417,  400,    0,    0,    0,    0,
    0,    0, 2754,    0,    0,    0,    0,    0, 8120,    0,
    0,    0,    0,    0,  115, 2774,    0,    0,    0,    0,
    0,    0, 3110, 2763,    0,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2816,    0,    0, 2817,
    0,    0,    0,    0,    0,    0,   11,    0, 2819,    1,
  260,   10, -605, 1633,  789,  235, -546, 2809, 2551,    0,
  -28,    0, 2560,  -94,  -40,    0,  508, -406,   -4,  -62,
  -43, -194, 2566, 2115,    0,    0,    0, 1434,    0,    0,
  -29, 1739, -530,   35, -930, 2201, 1983, -295, 1752, 2755,
 -306,    0,  -60,    0, -465,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -603,  -96, 1758, -297,  334, -891,    0,
    0,    0,    0, -338, -121, -371, 1769, -335,  -51,    0,
 -529,    0,    0, 1409, -976,    0, 1417,    0,    0, 2527,
 -526,   16,    0,    0,    0,    0, 2500,    0, 2280, 2541,
 -231, 1795, -701,    0,    0,    0, -667,    0,    0,    0,
    0,    0,    0, 1851,    0, 2554, 2116, 1794, 2534, 2548,
    0, 1223,    0, -512, 2078,  -35, -637, -706,    0,    0,
    0, -960, -931, -929, -247, -923,    0, 1455, -998,    0,
 1153,-1183, 1468,    0, 2330,    0,    0,    0, 1164, 1166,
 1130,  142, -277, -407, -897,    0,    0, 2117,    0, 1656,
    0,    0,  -55,    0,    0,    0,    0, -175,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1194,    0, 1250, -238,    0,    0,  330,    0, -377, -540,
 1085, 1238,    0, 1143,    0, 1908, 1690, 1513,    0, -996,
 -869, -871,    0, -488, -521, -291,    0, -851, 1231,  906,
 -391, -494, -750, -964,-1236,    0,    0,   -3,    0,    0,
    0,    0,    0, 2574,    0,    0,    0,    0, 2584, 2354,
    0,    0,    0,    0,    0,   -9, 2594,    0,    0,    0,
    0,  -10,    0,    0,    0, -344, 2088, -340, 1650, -452,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2305,
    0,    0, 2108,    0,    0,    0,    0,    0,    0,   26,
    0, 2914,    0,    0,    0,    0,    0, 2631,    0,    0,
    0,    0, 2436,    0, 2434,    0,    0,    0, 2193,    0,
    0,    0, -617,    0,    0,  119,    0, 2180,    0,    0,
    0,    0,    0, -571,-1010, -741,   23, -170, 1415,    0,
 2797, -116, 1508,    0, 2212, -627, 2213,    0, -859, 1966,
    0,    0, -141, -240,    0, 1310,-1203,    0,-1164,-1159,
-1124, 1423,    0, 1167,    0, 1209,    0, 1117,    0,    0,
    0,    0, 2446,    0,    0,    0, -400, 2236, 1994,-1007,
    0,    0,    0, -677, -467,  309,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2723,
 1347, 1835,    0, -461,    0,    0,    0, -917,    0, 1725,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -189, 1796,    0,    0,
};
#define YYTABLESIZE 12750
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  308,  154,  259,
   37,  208,  574,  437,  611,  192,  278,  281,  612,  579,
  750,  279,  282,  946,  243,  787,  217,  209,  195,  229,
  544,  252,  204,  775,  546,  247,  778,  486,  898,  736,
  222,  688,  789,  186,  188,  191,  194,  148,  199,  248,
  756,  253, 1245,  210,  188,  215,  254,  194, 1173,  148,
  199, 1234,  228,  127, 1423,  224,  713,  461, 1234,  641,
  994,  239,  149,  148,  241,  188,  461,  239,  215,  191,
  194,  250,  148,  199,  228,  127,  625,  194,  251,  313,
 1235,  536, 1236,  265, 1008,  267, 1195, 1235, 1238, 1236,
 1077,  266,  219,  246, 1239, 1238, 1051,  860,  482,  309,
  838, 1239, 1292,  969,  928,  620, 1203,  999, 1052,  210,
  148,  210,  311, 1270,  996,  249,  656,  626,  135,  748,
  507,  371,  352,  215,  327,   77,  851, 1085, 1246,  307,
  194,  148,  228,  966, 1025,  734,  154,  820,  697, 1254,
  125,  920,   77, 1032, 1003,  699,  620,  333,  289,  620,
  748, 1496,  628,  566,  508,  697,  900,  656, 1275,  749,
  748,  256,  970,  748,  620,  276,  868,  263,  437, 1100,
  710,  315,  360, 1377,  256,  656,  626,  135,  748,  763,
 1516,  352,  614,  765,  766,  767,  768,  683,  277,  627,
   77,  748, 1057,  745,  748,   89, 1072,  697, 1293,  125,
  611, 1184,  256,  624,  612,  256,  277, 1596,  513,  748,
  454,  628,  566,  695,  697,  256,  589, 1762,  256, 1518,
  256,  457,  653,  753, 1519,  699,  493, 1463,  943,  983,
  457,  762,  325,  256,  263,  579,  210, 1421,  317,  566,
  148,  614,  463,  127,  624,   77,  683,  624,  627,  148,
  795,  463,  252,  894,  708,  500, 1458,  268,  280, 1520,
  506,  133,  624,  277, 1603,  748,  699,  318,  472,  821,
  191, 1190,  695,   60,  148,  589,  694,   85, 1700, 1255,
   85,  277,  739,  545,  699, 1039,  835, 1438, 1647, 1459,
  548,  325,  209,  252,  561,  613,  252,  563,  566,  832,
  830,  231,  129, 1234,  630,  923,  924,  195,  633,  210,
  621,  252,  575, 1381,  577,  508,  210,  694,  210,  770,
  210, 1009,  739,  566,  772,  321,  496,  394,  629, 1196,
  695,  631, 1235, 1415, 1236,  694,  613, 1389,  232, 1448,
 1238,  194,  199, 1187,  516, 1503, 1239,  623,  364, 1218,
  148, 1648,  191,  740,  613, 1199, 1201, 1144, 1145,  469,
 1506, 1433,   79,  630,  715, 1058,  195,  990,  831,  130,
  357,  695, 1259,  322,  283,   80,   81,  895,  695, 1452,
 1674,  739,   83, 1256,  991,  272,  394, 1191, 1170,  695,
 1098,  716, 1253, 1097, 1010,   14,  135,  748,  135,  748,
  691, 1604, 1188,  516,  268, 1756,  748,  707,  486, 1260,
 1421, 1455,  518, 1078, 1200,  518,  704,  899,  469, 1464,
 1516,  506,  707,  620,  256,  519, 1162,  712, 1495,  357,
  695,  718,  927, 1729,  656,  148,  457,  695,  194,  463,
  511,  815,  461,  271,  656,  620,  148,  656,  942,  925,
  148, 1600,  256,  194,  256,  148,  656,  626,  154, 1518,
  761,  256, 1020,   89, 1519, 1022,  697,  744, 1298,  697,
  731,  486,  834,  697,  721,  327, 1593, 1761, 1722, 1028,
 1378, 1084,  498,  697, 1230,  323,  697, 1629,  125, 1524,
  764,  699,  628,  723,  486,  697,  135,  748,  653, 1520,
  614,  461,  748,  656,   97,  461, 1046,  837, 1531,  791,
 1016,  639,  656, 1566, 1099, 1567, 1410, 1029,  683,  748,
  463,  624,  614, 1509, 1605, 1451,  748, 1019,  639,  627,
  252, 1766,  252,  748,  784,  210,  817,  699,   76,  252,
  784,  253,  697,  624,  974, 1404,  860, 1092,  275, 1087,
 1594,  697,  256, 1194, 1070,   76,  656, 1436,  589,  620,
  748, 1310, 1311,  995, 1312, 1313, 1286,  377,  656, 1799,
  656,  626,   79,  657,  256,  733,  352,  423,  697,  463,
  709,  256,  253,  275,  209,  253, 1385,  845,  846,  566,
 1609,  394,  697,  589,  694,  697,  699,  508,  816,  480,
  253, 1016,  987,   76,  748,  275,  628,  620,  485,  697,
  210, 1183,  699,  613,  657,  256,  694,  657,  683,  862,
  748,  589, 1640,  469,  589,   79,  262,  683, 1642,  861,
  252,  639,  657, 1014, 1015,  613,  614, 1450, 1319,  516,
  589,  683,  325,  627,  630,  518,  380,   85,  566,  215,
 1530,  748,  252,  695, 1287,  195,  947,  624,   76,  252,
 1225,  683,  256,  694,  770, 1829,  589,  695,  785,  772,
 1736,  707,  694,  936,  902,  256,  275, 1677, 1678,  945,
  518,  210,  210,  781, 1679,  463, 1756, 1035, 1579,  739,
  463,  374, 1409,  252, 1681,  378,  695,  720,  357,  463,
  461,  357,  194,  506,  500,  380,  993,  194,  209,  486,
  731,  516,  454, 1744, 1425, 1752,  694,  695,  695, 1244,
  683, 1004, 1004,  148, 1288,  148,  695, 1528,  210, 1234,
  694, 1252,  136,  548,  210, 1470,  679,  894,  210,  358,
  684,  685,  687, 1177, 1507,  894, 1180,  122,  461,  613,
  374, 1274,  842,  252,  378, 1754,  469, 1101, 1235,  210,
 1236, 1038, 1040,  143, 1237,  695, 1238, 1370,  365, 1372,
  379, 1237, 1239, 1424,  695,  385,  695, 1728,  463, 1087,
  131,  386,  518,  358, 1365,  695, 1735,  210,  210,  697,
  134,  136, 1018,  894,  696,  382, 1822,  381,  358, 1045,
 1397,  496,  724, 1651, 1071, 1491,  726,  137,  733, 1437,
  357, 1055,  508,  157, 1082, 1447,  283, 1387,  695,  253,
 1730,  253,  357,  148,  357,  200,  463, 1771,  253,  379,
 1037,  697,  695,  148,  385,  696,  698,  518,  384, 1257,
  386,  383, 1345, 1511,  105, 1094,  595, 1764, 1823,  733,
   79, 1670,  733,  696,  382,  699,  381,  628, 1521,  284,
  461,  895, 1067, 1068, 1069,  366,  137,  733,  144,  895,
 1773,  894, 1812,  106,  107,  145,  108,  699, 1095, 1181,
  156, 1102, 1096,  109,  110,  111,  112,  113, 1756,  114,
  420,  657,  115,  116,  117,  118,  119,  384, 1178, 1182,
  383,  657,  497, 1755,  657,  595, 1791,   14,  683,  894,
  380,  147, 1805,  657,  273, 1696,  681,  895,  506,  253,
  334,  437,  122,  122,  699,  987, 1247, 1248,  463,  194,
  199,  463,  228,  127,  216, 1179,  894,  486,  463,  463,
  682,  253,  869,  568,  763,  946,  973,  876,  253,  976,
  374,  463,  463, 1432,  437,  374,  194,  498, 1399,  378,
  657,  374, 1358,  326,  683, 1360, 1363, 1370, 1400,  657,
  202,  203,  875,  683,  499, 1299,  894,  699,  240,  683,
 1258,  682,  253, 1646, 1454, 1778, 1779,  670, 1765,  485,
  611,  699,  683,  903,  612,  895,  792,  335, 1715,  682,
  919,  683,  568,  158, 1628,  926,  423, 1630, 1682, 1643,
  366,  327,  136,  657, 1586,  933, 1237,  937,  938,  939,
  940,  941,  423,  933, 1529,  657, 1647,  657,  670,  209,
 1265,  670, 1592,  895,  379,  486,  454,   79,   76,  385,
  344, 1266,  253,  380, 1265,  386,  670, 1716,  683,  737,
  614,  275,  485, 1527, 1388,  210,  159,  683, 1717,  382,
  895,  381,  210, 1375, 1375, 1379,  234,  358,  295,  693,
  358, 1595,  733, 1834,  694,  485,  697,  793,  986,  697,
  516,  961,  681, 1511,  564,  366,  236,  137,  374, 1648,
 1281,  614,  378, 1406, 1695, 1408, 1693,  105, 1694, 1687,
  895,  683,  384,  255, 1174,  383, 1165, 1166, 1301,  614,
 1714,  573,  696,  795, 1305,  683, 1709,  345,  794, 1711,
  425,  605,  696,  328,  486,  696,  106,  107,  733,  108,
  241,  809,  244,  753,  696,  697,  109,  110,  111,  112,
  113,  329,  114,  564,  697,  115,  116,  117,  118,  119,
 1807, 1808, 1601, 1176,  961, 1734, 1453, 1714,  570, 1497,
  753,  264, 1718, 1500,  463,  869,  463,  379,  810,  869,
  573,  241,  385,   99,  151,  330,   79,   79,  386,  358,
  605,  696,  463, 1004,  115, 1357,  307,  194,  595,  361,
  696,  358,  382,  358,  381,  693,  359, 1714,  283,  532,
  694,  256, 1103, 1104, 1108, 1796, 1840, 1116, 1118, 1120,
  101, 1128, 1131, 1134, 1136, 1139, 1142,  570,  697, 1146,
 1147,  115, 1149,  595, 1151,  362, 1276, 1154, 1155, 1156,
 1157, 1158, 1159,  151,  696,  384, 1714, 1777,  383, 1714,
  359,  896, 1513,  683, 1163, 1806,  423,  658,  696,  533,
 1167,  595,  241,   79,  595,  359,  115, 1435,  682,  697,
  733, 1714,  508, 1462,  697, 1122,  594, 1466,  733,  260,
  595,  682,  811,  697,  695, 1277,  696,  697,  698,  102,
  682, 1123, 1733,  534,  683, 1680,  590, 1833,  658,  103,
  485,  658,  105,  241,  670,  670,  595,  733,  733, 1627,
  733,  535,  683,  268,  327,  261,  658,  733,  733,  733,
  733,  733, 1276,  733,  104,  518,  733,  733,  733,  733,
  733,  106,  107,  697,  108,  594, 1124,  682, 1733,  283,
 1215,  109,  110,  111,  112,  113,  682,  114,  567,  374,
  115,  116,  117,  118,  119,  590,  572, 1800,  659,  568,
  374, 1532, 1208,  598, 1635, 1125,  518,  493,  802,  518,
  803, 1280,  659,  327,  463, 1004, 1209,  209,  506,  105,
  659, 1733,  904,  194,  518,  559,  518,  945, 1375, 1636,
  682, 1216,  401, 1577,  697,  695,  659,  696, 1801,  698,
 1210, 1737, 1738,  210,  682, 1641,  283,  670,  106,  107,
  508,  108,  148, 1733,  598,  572,  493,  283,  109,  110,
  111,  112,  113,  269,  114,  982,   77,  115,  116,  117,
  118,  119, 1126,  138,  139,  402,  697,  695,  105,  696,
  982,  698, 1775,  933,  559, 1802, 1756, 1637,  670,  906,
  659,  401, 1237, 1749, 1211, 1326, 1327,  270, 1329, 1331,
  908, 1815,  523,  151,  437,  150, 1336,  106,  107, 1751,
  108,  524, 1645,  605,  250,  420,  455,  109,  110,  111,
  112,  113, 1675,  114,  151, 1349,  115,  116,  117,  118,
  119, 1626, 1513, 1356,  402,  423, 1665,  151,  423,   52,
 1163,   79,  274, 1113, 1364,  423, 1366, 1367,  605, 1789,
   53,  398,  152,  151, 1825,  250,  506,   54,  250, 1513,
  455,  693,  139,  188, 1676,  987,  694,  291,  485, 1114,
  518, 1571,  151,  250,  359,  455,  605,  359,  423,  605,
  987,  151,   55,  160, 1673,   43, 1652,  151, 1813,  681,
  290, 1816,   51, 1572,  306,  605,  697,  686,  936,  697,
 1573, 1161,  292,  148,  338,  151, 1513,  339,  191,  216,
  398,  518, 1689, 1828,  518,  658, 1690, 1417, 1723,  202,
  203,  605,  542,  518,  683,  658,  149,  148,  658,  518,
  293,  518, 1783,  294, 1574,  543,  316,  658,  686,  518,
  681,  686,  160,  319,   43, 1705, 1513,   56,  161,  295,
  216,   51,  340,  216, 1513,  697,  686,  231,  594,  508,
  336,   41,  936,  210,  697,  518,  485,   24,  216, 1784,
  872,  518,  693,  872,  518,  296,  194,  694,  590,   57,
  683,  944, 1785,   58,  658, 1513,  359, 1467, 1513,  683,
  347,  508,  508,  658,  232, 1757, 1475,  744,  359, 1644,
  359,  697,  695, 1480,  696,   47,  698,  161,  349,  375,
 1513,  149,  148,  590,  693,  703,  508,  380,  702,  694,
   41,  594,  872,   49,  594, 1208,   24,  508, 1739,  148,
  508, 1369,  423, 1371,  483,  710,  423,  658,  358, 1209,
  594,  590,  491,  368,  590, 1797,  188, 1759,  980,  658,
  780,  658,  372,  779,   79,  485,  744,  370, 1760,  471,
  590,  149,  148, 1210,   47,  506,  594, 1208, 1739,  148,
  373, 1811,  710,  127, 1748,  369,  518, 1819, 1820,   25,
  376, 1209,   49,  518,   48,  710,  590, 1631,  957,  980,
  980,  980,  980,  980,  980,  980,  930,  506,  506,  699,
  377,  518, 1831,  149,  148, 1210,  466,  980,  980,  980,
  980, 1739,  148, 1838,  458,  191, 1839, 1211,  471, 1047,
 1048,   50,  506,  532,   45,  123,  123,  467,  518, 1824,
  128,  518,  127,  506,  369,  518,  506,  470,   25,  980,
  191,  980,  471,   48,  578,  105,  475, 1094,  518,  116,
  518, 1614, 1615,  473,  518,  697,  695,  518,  696, 1211,
  698,  697,  695, 1171,  696,  191,  698,  476,  965,  492,
  518,  964,  423,  533,  106,  107,  518,  108,  477,   76,
   50,  105,   46,   45,  109,  110,  111,  112,  113,  128,
  114,  216,  523,  115,  116,  117,  118,  119,  494,  873,
 1076,  524,  873, 1075,  504,  360, 1083,  534,  116,  856,
  106,  107, 1654,  108,  495,  686,  514,  105,   14,  360,
  109,  110,  111,  112,  113,  535,  114,  360,  686,  115,
  116,  117,  118,  119,  520,  697,  695,  686,  696,  693,
  698,   46, 1175,  360,  694, 1174,  106,  107,  518,  108,
  216,  873,  521,  531,   98,  516,  109,  110,  111,  112,
  113, 1219,  114,  539, 1075,  115,  116,  117,  118,  119,
  518,  552, 1224, 1268, 1688, 1223, 1267,  518, 1273,  727,
 1383, 1272, 1691, 1382,  686,  725,  697,  695,  518,  696,
  559,  698,  562,  686,  572,   99,  516,  360,  576,  516,
 1472,  961, 1502, 1174,  626,  699,  518,  100,  630,  632,
  980,  518,  980,  980,  516,  634,  516,  665, 1704,  666,
  980,  667, 1706,  668,  697,  695,  669,  696,  980,  698,
 1713,  670,  101,  980,  350,  671,  980,  686,  195,  672,
  518,  675,  961,  961,  961,  961,  961,  973,  961, 1508,
  980,  686,  699,  699, 1837,  980,  980, 1756,  673,  980,
  961,  961,  961,  961,  674,  676,  980,  677,  980,  980,
  682,  351,  980,  352,  683,  692,  980, 1713,  423,  980,
 1750,  980,  701,  980,  700,  705,  719,  710,  980,  980,
  722,  980,  961,  693,  961,  729,  353,  195,  694,  693,
  105,  102,  727,  743,  694,  747,  195,  980,  980,  748,
  980,  103,  929,  697,  695,  752,  696, 1713,  698,  754,
  755,  354,  757,  758,  980, 1795,  980,  980,  980,  106,
  107,  980,  108,  760,  980,  980,  104,  523,  769,  109,
  110,  111,  112,  113,  980,  114,  770,  772,  115,  116,
  117,  118,  119, 1790,  773,  807, 1713,  806,  980, 1713,
  782,  980,  808,  818,  813,  195,  975,  697,  695,  275,
  696,  822,  698,  693,  823,  824,  825,  980,  694,  828,
  827, 1713,  833,  980,  839,  840,  495,  844,  980,  843,
 1153,  697,  695,  853,  696,  856,  698,  855,  864,  980,
  980,  866,  980,  870, 1323, 1343,  697,  695,  980,  696,
  865,  698,  897,  516,  901,  396,  397,  398,  905,  907,
  909,  921,  931,  962,  693,  960,  959,  495,  967,  694,
  495,  971,  516,  516,  972,  516,  988,  980,  980,  516,
  980, 1000,  516,  406,  407,  495,  982,  980,  980,  980,
  980,  980,  997,  980,  984,  516,  980,  980,  980,  980,
  980,  516,  693,  961, 1016,  961,  961,  694, 1021,  834,
 1117, 1026, 1121,  961, 1129, 1132, 1027, 1137, 1140, 1143,
  673,  961,  785, 1029, 1033, 1034,  961,  697,  695,  961,
  696, 1042,  698, 1479, 1036,  984,  984,  984,  984,  984,
  984,  984, 1043,  961, 1746,  961, 1050, 1044,  961,  961,
 1053, 1059,  961,  984,  984,  984,  984,  195, 1064,  961,
 1073,  961,  961, 1066,  673,  961, 1078, 1090, 1110,  961,
 1091, 1148,  961,  516,  961,  715,  961, 1150,  195,  673,
 1160,  961,  961, 1152,  961,  984, 1172,  984,  693, 1186,
 1189,  693, 1192, 1184, 1205,  516,  694, 1202, 1226,  180,
  961,  961,  516,  961,  195, 1207,  702,  195, 1344,  697,
  695,  516,  696,  516,  698, 1213, 1229,  961, 1249,  961,
  961,  961, 1240, 1251,  961,  195,  195,  961,  961, 1269,
 1261,  516, 1271, 1278,  195,  195,  516,  961, 1282, 1283,
  180,  195, 1284,  180, 1285,  693,  195, 1294, 1084, 1296,
  694,  961,  195, 1303,  961, 1308, 1306, 1307,  180,  195,
 1309, 1320, 1347,  697,  695,  516,  696, 1315,  698,  693,
  961,  195, 1316,  195,  694,  195,  961, 1321, 1322,  518,
 1324,  961, 1325, 1332,  693, 1333, 1334, 1335, 1337,  694,
 1338, 1339,  961,  961,  195,  961,  195, 1340, 1341, 1342,
  894,  961,  195,  195,  495, 1362,  495, 1359, 1368,  195,
  495, 1384, 1386,  495, 1390, 1391, 1392, 1393, 1394, 1395,
  518,  195, 1402,  518, 1407, 1405,  495,  195, 1412, 1414,
  961,  961,  554,  961, 1434, 1426, 1422, 1427,  518, 1428,
  961,  961,  961,  961,  961, 1429,  961,  989, 1430,  961,
  961,  961,  961,  961, 1431, 1439,  984, 1449,  984,  984,
 1456, 1460, 1317, 1461,  195,  693,  984, 1465, 1471, 1492,
  694, 1498,  229, 1499,  984,  195, 1505, 1504, 1522,  984,
 1568, 1580,  984, 1578, 1582, 1581, 1584, 1583,  989,  989,
  989,  989,  989,  989,  989, 1587,  984, 1585, 1588, 1597,
 1599,  984,  984, 1589,  495,  984,  989,  989,  989,  989,
  152, 1602,  984,  229,  984,  984,  229, 1607,  984, 1608,
 1606, 1610,  984, 1611, 1612,  984,  495,  984, 1613,  984,
 1616,  229, 1617,  495,  984,  984, 1618,  984,  989, 1619,
  989,  673, 1620, 1621, 1622, 1623, 1624,  693,  673, 1625,
 1632,  673,  694,  984,  984,  673,  984,  180, 1348,  697,
  695, 1633,  696,  673,  698, 1634, 1650,  495, 1653,  152,
  984,  130,  984,  984,  984, 1655,  222,  984,  180,  673,
  984,  984, 1656, 1657, 1658, 1350,  697,  695, 1659,  696,
  984,  698, 1351,  697,  695, 1660,  696, 1661,  698,  180,
 1662,  693, 1663,  673,  984,  180,  694,  984, 1352,  697,
  695, 1668,  696, 1669,  698, 1671, 1672,  222, 1692, 1698,
  222, 1699, 1708,  984, 1664, 1745, 1724, 1732,  180,  984,
 1753, 1767,  485,  673,  984,  222, 1769, 1772,  673, 1774,
 1776, 1781, 1782, 1787, 1793,  984,  984,  180,  984, 1353,
  697,  695, 1765,  696,  984,  698,  180,  518, 1794,  518,
  673, 1764,  180,  518,  158, 1810,  518, 1817, 1814, 1818,
 1821, 1756,  673,  485,  673, 1832,  485, 1835, 1836,  518,
  180,  620,    1,  984,  984,   29,  984,  697,  695,   31,
  696,  485,  698,  984,  984,  984,  984,  984,  202,  984,
  985,  748,  984,  984,  984,  984,  984,  366,  270,  989,
  148,  989,  989,  516,  649,  268,  452,  180,   81,  989,
  944,  670, 1354,  697,  695,  230,  696,  989,  698,  212,
  275,  192,  989,  516,  394,  989,  516,  193,  943,  681,
  679,  985,  985,  985,  985,  985,  985,  985,  675,  989,
  241,  677,  229,  288,  989,  989,  274,  518,  989,  985,
  985,  985,  985,  367,  705,  989,  230,  989,  989,  230,
  286,  989,  704,  222,  322,  989,  287,  706,  989,  518,
  989,  339,  989,  614,  230,  330,  518,  989,  989,  152,
  989,  985,  328,  985,  715,   27,   30,  693,   39, 1373,
  135,  474,  694,  465,  961, 1304,  989,  989,  464,  989,
  152, 1355,  697,  695,  854,  696, 1302,  698, 1089, 1300,
  518,  245, 1290,  989,  693,  989,  989,  989, 1590,  694,
  989,  693,  541,  989,  989,  585,  694, 1591, 1279,  152,
  819,  527, 1228,  989,  512, 1701, 1031,  693, 1291,  998,
  241,  556,  694, 1570,  222, 1763,  222,  989,  152,  518,
  989,  697,  695,  222,  696,  557,  698,  152, 1576,  801,
 1770, 1768,  241,  152, 1804, 1024,  989, 1416, 1747,  241,
 1707, 1830,  989, 1721, 1798,  250, 1220,  989,  693, 1398,
 1525,  152, 1731,  694,  547,  805,  564,  538,  989,  989,
  518,  989, 1065,  518,  841, 1446,  137,  989,  505,  738,
  485,  742,  485,  241, 1056,  989, 1002,  288,  518,  485,
  984, 1204,  985, 1697, 1638,  693,  250, 1780, 1809,  250,
  694, 1841,  485,  968,  751, 1411,  989,  989, 1328,  989,
    0, 1198, 1361,    0,  250,    0,  989,  989,  989,  989,
  989,    0,  989,  842,  222,  989,  989,  989,  989,  989,
    0,  693,  985,    0,  985,  985,  694,    0,    0,    0,
    0,    0,  985,    0,    0,    0,  222,    0,    0,    0,
  985,    0,    0,  222,    0,  985,    0,    0,  985,    0,
    0,    0,    0,    0,  842,  842,  842,  842,  842,    0,
  842,    0,  985,  241,    0,  230,    0,  985,  985,    0,
  485,  985,  842,  842,  842,  842,    0,  222,  985,    0,
  985,  985,    0,    0,  985,    0,    0,    0,  985,    0,
    0,  985,  485,  985,    0,  985,    0,    0,    0,  485,
  985,  985,    0,  985,    0,    0,  842,    0,    0,    0,
  693,    0,    0,    0,    0,  694,    0,    0,  176,  985,
  985,    0,  985, 1493,  697,  695,    0,  696,    0,  698,
    0,    0,    0,  485,    0,    0,  985,    0,  985,  985,
  985, 1314,    0,  985,    0,    0,  985,  985,    0,  362,
    0, 1494,  697,  695,    0,  696,  985,  698,    0,  693,
    0,    0,    0,  241,  694,    0,    0,    0,    0,    0,
  985,    0,  513,  985,    0,    0,    0,  176, 1501,  697,
  695,    0,  696,    0,  698,  241,    0,    0,    0,  985,
  362,    0,  241,  362,    0,  985,    0,  518,    0,  518,
  985,    0,    0,    0,    0,    0,  518,    0,  362,    0,
    0,  985,  985,  513,  985,    0,  513,    0,    0,  518,
  985,    0,    0,  250,   62,  250,  241,    0,    0,    0,
    0,  513,  250,  513,    0,   63,    0,    0,    0,    0,
    0,    0,   64,    0,    0,  854,    0,    0,    0,  985,
  985,    0,  985,    0,    0,   65,    0,    0,    0,  985,
  985,  985,  985,  985,    0,  985,    0,    0,  985,  985,
  985,  985,  985,    0,    0,  842,    0,  842,  842,    0,
    0,    0,    0,    0,    0,  842,  854,  854,  854,  854,
  854,    0,  854,  842,    0,    0,    0,  518,  842,    0,
    0,  842,    0,    0,  854,  854,  854,  854,  697,  695,
    0,  696,    0,  698,    0,  842,   66,    0,    0,  518,
  842,  842,    0,  250,  842,    0,  518,    0,    0,    0,
    0,  842,   67,  842,  842,    0,    0,  842,  854,    0,
    0,  842,    0,    0,  842,  250,  842,    0,  842,   68,
    0,    0,  250,  842,  842,    0,  842,    0,    0,    0,
  518,  186,  693,    0,   69,    0,    0,  694,   70,    0,
    0,   71,  842,  842,    0,  842,  176,    0,    0,   72,
    0,    0,    0,    0,    0,    0,  250,    0,    0,  842,
  693,  842,  842,  842,    0,  694,  842,  176,   73,  842,
  842,  910,  911,  912,  913,  914,  915,  916,  917,  842,
    0,    0,    0,    0,    0,    0,    0,  693,  176,    0,
  186,    0,  694,  842,  176,    0,  842,    0,    0,    0,
  513,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  842,    0,    0,    0,    0,  176,  842,  513,
  513,    0,  513,  842,    0,    0,  513,    0,    0,  513,
    0,    0,    0,    0,  842,  842,  176,  842,    0,    0,
  362,    0,  513,  842,    0,  176,    0,  362,  513,    0,
  362,  176,    0,    0,  362,    0,    0,    0,    0,    0,
    0,    0,  362,    0,    0,    0,    0,    0,  857,  176,
    0,    0,  842,  842,    0,  842,    0,  854,  362,  854,
  854,    0,  842,  842,  842,  842,  842,  854,  842,    0,
    0,  842,  842,  842,  842,  842,    0,    0, 1478,    0,
  854,    0,  362,  854,    0,    0,  176,    0,    0,  857,
  857,  857,  857,  857,    0,  857,  693,  854,    0,    0,
  513,  694,  854,  854,    0,    0,  854,  857,  857,  857,
  857,    0,  362,  854,    0,  854,  854,  187,    0,  854,
    0,    0,  513,  854,    0,    0,  854,    0,  854,  513,
  854,    0,    0,    0,    0,  854,  854,    0,  854,  362,
  513,  857,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  362,    0,  362,  854,  854,    0,  854,  513,    0,
    0,    0,    0,  513,    0,    0,    0,    0,    0,  186,
    0,  854,    0,  854,  854,  854,  187,    0,  854,    0,
    0,  854,  854,    0,    0,    0,    0,    0,    0,    0,
  186,  854,  513,    0,    0,    0,    0,    0,    0,    0,
   98,    0,   84,    0,    0,  854,    0,    0,  854,   85,
    0,  186,    0,   86,   87,    0,    0,  186,    0,    0,
    0,    0,   88,    0,  854,    0,    0,    0,    0,    0,
  854,    0,    0,    0,   89,  854,    0,    0,    0,    0,
  186,   99,    0,    0,    0,    0,  854,  854,    0,  854,
    0,    0,    0,  100,    0,  854,    0,    0,    0,  186,
    0,    0,    0,    0,    0,    0,    0,  132,  186,    0,
    0,    0,    0,    0,  186,    0,    0,    0,  101,    0,
  858,    0,    0,    0,  854,  854,    0,  854,    0,    0,
    0,    0,  186,    0,  854,  854,  854,  854,  854,   90,
  854,    0,    0,  854,  854,  854,  854,  854,    0,    0,
  857,    0,  857,  857,    0,    0,   91,    0,    0,    0,
  857,  858,  858,  858,  858,  858,    0,  858,    0,  186,
    0,   92,    0,    0,    0,   93,  857,    0,    0,  858,
  858,  858,  858,    0,    0,    0,   94,  102,    0,    0,
  857,    0,    0,    0,    0,  857,  857,  103,    0,  857,
  664,    0,    0,    0,    0,   95,  857,    0,  857,  857,
    0,    0,  857,  858,    0,  187,  857,    0,    0,  857,
    0,  857,  104,  857,    0,    0,    0,    0,  857,  857,
    0,  857,    0,    0,    0,    0,  187,    0,    0,    0,
  105,  664,    0,    0,  664,    0,    0,  857,  857,    0,
  857,    0,    0,    0,    0,    0,    0,  187,    0,  664,
    0,    0,    0,  187,  857,    0,  857,  857,  857,  106,
  107,  857,  108,    0,  857,  857,    0,    0,    0,  109,
  110,  111,  112,  113,  857,  114,  187,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,  857,    0,
    0,  857,    0,    0,    0,  187,    0,    0,    0,    0,
    0,    0,    0,    0,  187,    0,    0,  857,    0,    0,
  187,    0,    0,  857,    0,    0,    0,    0,  857,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  187,  857,
  857,    0,  857,    0,    0,    0,    0,    0,  857,    0,
    0,  929,  697,  695,    0,  696,    0,  698,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  892,  891,  893,  852,    0,  187,    0,  857,  857,    0,
  857,    0,  858,    0,  858,  858,    0,  857,  857,  857,
  857,  857,  858,  857,    0,    0,  857,  857,  857,  857,
  857,    0,    0,    0,    0, 1231,    0,    0,  858,    0,
    0,    0,    0,    0,  852,    0,  852,  852,  852,    0,
    0,    0,  858,    0,    0,    0,    0,  858,  858,    0,
    0,  858,  852,  852,  852,  852,    0,    0,  858,    0,
  858,  858,    0,    0,  858,    0,    0,    0,  858,    0,
    0,  858,    0,  858,    0,  858,    0,    0,    0,    0,
  858,  858,    0,  858,    0,    0,  852, 1468, 1469,    0,
    0,    0,    0, 1473, 1474,    0, 1476, 1477,    0,  858,
  858,    0,  858,    0, 1481, 1482, 1483, 1484,    0, 1485,
 1486, 1487, 1488, 1489, 1490,    0,  858,    0,  858,  858,
  858,    0,    0,  858,    0,    0,  858,  858,    0,    0,
    0,    0,    0,    0,    0,    0,  858,    0,  664,    0,
    0,  664,    0,    0,    0,    0,    0,    0,  664,  730,
  858,  664,    0,  858,    0,  664,    0,    0,    0,    0,
  664,    0,    0,  664,    0,    0,    0,    0,    0,  858,
    0,    0,    0,    0,    0,  858,    0,    0,    0,  664,
  858,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  858,  858,    0,  858,    0,    0,    0,    0,    0,
  858,    0,  879,  664,    0,    0,    0,  664,    0,    0,
  693,    0,    0,    0,    0,  694,  664,    0,  880,    0,
    0,    0,    0,    0,    0,  851,    0,    0,    0,  858,
  858,    0,  858,  664,    0,    0,    0,    0,    0,  858,
  858,  858,  858,  858,    0,  858,    0,    0,  858,  858,
  858,  858,  858,    0,  881,  852,    0,  852,  852,    0,
  664,    0,    0,  882,    0,  852,  851,    0,  851,  851,
  851,  883,  664,    0,  664,  384,    0,    0,    0,    0,
    0,  852,    0,    0,  851,  851,  851,  851,    0,    0,
  884,    0,  885,    0,    0,  852,    0,    0,    0,    0,
  852,  852,    0,  386,  852,  661,  886,    0,  887,  888,
  889,  852,    0,  852,  852,    0,    0,  852,  851,    0,
    0,  852,    0,    0,  852,    0,  852,    0,  852,    0,
    0,    0,  388,  852,  852,    0,  852,    0,    0,    0,
    0,    0,    0,    0,    0,  105,  661,    0,    0,  661,
    0,    0,  852,  852,    0,  852,    0,    0,    0,  890,
    0,    0,    0,    0,  661,    0,    0,    0,    0,  852,
    0,  852,  852,  852,  106,  107,  852,  108,    0,  852,
  852,    0,    0,    0,  109,  110,  111,  112,  113,  852,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,  852,    0,    0,  852,  393,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  852,  395,    0,    0,    0,    0,  852,    0,
    0,    0,    0,  852,    0,    0,    0,  396,  397,  398,
    0,  202,  203,    0,  852,  852,    0,  852,    0,    0,
    0,    0,    0,  852,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,    0,
    0,    0,    0,    0,    0,  411,  412,    0,  853,    0,
    0,    0,  852,  852,  416,  852,    0,  851,    0,  851,
  851,    0,  852,  852,  852,  852,  852,  851,  852,    0,
    0,  852,  852,  852,  852,  852,    0,    0,    0,    0,
    0,    0,    0,  851,    0,    0,    0,    0,    0,  853,
    0,  853,  853,  853,    0,    0,    0,  851,    0,    0,
    0,    0,  851,  851,    0,    0,  851,  853,  853,  853,
  853,    0,    0,  851,    0,  851,  851,    0,    0,  851,
    0,    0,    0,  851,    0,    0,  851,    0,  851,    0,
  851,    0,    0,    0,    0,  851,  851,    0,  851,    0,
    0,  853,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  851,  851,    0,  851,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  851,    0,  851,  851,  851,    0,    0,  851,    0,
    0,  851,  851,    0,    0,    0,    0,    0,    0,    0,
    0,  851,    0,  661,    0,    0,  661,    0,    0,    0,
   98,    0,    0,  661,    0,  851,  661,    0,  851,    0,
  661,    0,    0,    0,    0,  661,    0,    0,  661,    0,
    0,    0,    0,    0,  851,    0,    0,    0,    0,    0,
  851,    0,    0,    0,  661,  851,    0,    0,    0,    0,
    0,   99,    0,    0,    0,    0,  851,  851,    0,  851,
    0,    0,    0,  100,    0,  851,    0,    0,  661,    0,
    0,    0,  661,    0,    0,    0,    0,    0,    0,    0,
    0,  661,    0,    0,    0,    0,    0,    0,  101,    0,
  856,    0,    0,    0,  851,  851,    0,  851,  661,    0,
    0,  554,    0,    0,  851,  851,  851,  851,  851,    0,
  851,    0,    0,  851,  851,  851,  851,  851,    0,    0,
  853,    0,  853,  853,    0,  661,    0,    0,    0,    0,
  853,  856,    0,  856,  856,  856,    0,  661,    0,  661,
    0,    0,    0,    0,    0,    0,  853,    0,    0,  856,
  856,  856,  856,    0,    0,    0,    0,  102,    0,    0,
  853,    0,    0,    0,  666,  853,  853,  103,    0,  853,
    0,    0,    0,    0,    0,    0,  853,    0,  853,  853,
    0,    0,  853,  856,    0,    0,  853,    0,    0,  853,
    0,  853,  104,  853,    0,    0,    0,    0,  853,  853,
    0,  853,    0,    0,    0,  666,    0,    0,  666,    0,
  105,    0,    0,    0,    0,    0,    0,  853,  853,    0,
  853,    0,    0,  666,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  853,    0,  853,  853,  853,  106,
  107,  853,  108,    0,  853,  853,    0,    0,    0,  109,
  110,  111,  112,  113,  853,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,  853,    0,
    0,  853,    0,    0,  105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853,    0,    0,
    0,    0,    0,  853,    0,    0,    0,    0,  853,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,  853,
  853,    0,  853,  109,  110,  111,  112,  113,  853,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  853,  853,    0,
  853,    0,  856,    0,  856,  856,    0,  853,  853,  853,
  853,  853,  856,  853,    0,    0,  853,  853,  853,  853,
  853,    0,    0,    0,    0,    0,  649,    0,  856,  417,
    0,  418,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  856,    0,  420,    0,    0,  856,  856,  277,
    0,  856,    0,    0,    0,    0,    0,    0,  856,    0,
  856,  856,    0,    0,  856,    0,    0,    0,  856,    0,
    0,  856,    0,  856,    0,  856,    0,    0,    0,    0,
  856,  856,    0,  856,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  856,
  856,    0,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  856,    0,  856,  856,
  856,    0,    0,  856,  528,    0,  856,  856,    0,    0,
    0,    0,  666,    0,    0,  666,  856,    0,    0,    0,
    0,    0,  666,    0,    0,  666,    0,    0,    0,  666,
  856,    0,    0,  856,  666,    0,    0,  666,    0,    0,
    0,    0,    0,    0,  528,  528,    0,    0,  528,  856,
    0,  560,    0,  666,    0,  856,    0,    0,    0,    0,
  856,    0,    0,  528,    0,  528,    0,    0,    0,    0,
    0,  856,  856,    0,  856,    0,    0,  666,    0,    0,
  856,  666,    0,    0,    0,    0,    0,    0,    0,    0,
  666,    0,    0,    0,    0,  528,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  666,    0,  856,
  856,    0,  856,    0,    0,    0,    0,    0,    0,  856,
  856,  856,  856,  856,    0,  856,    0,    0,  856,  856,
  856,  856,  856,  381,  666,  857,    0,    0,    0,  587,
    0,    0,  382,    0,    0,    0,  666,    0,  666,    0,
    0,    0,    0,  858,  666,  383,    0,  588,    0,    0,
  589,  384,  385,    0,  649,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  643,    0,  420,  666,  666,    0,  666,  277,    0,  386,
    0,    0,  387,  666,  666,  666,  666,  666,    0,  666,
    0,  590,  666,  666,  666,  666,  666,  591,  592,  593,
    0,  594,  595,  596,    0,  597,    0,    0,    0,    0,
  389,  390,    0,    0,  105,    0,    0,    0,  640,    0,
  640,  640,  391,  640,    0,  598,  392,  599,    0,    0,
    0,    0,    0,    0,  871,    0,  640,    0,    0,    0,
    0,  640,    0,  106,  107,    0,  108,    0,    0,    0,
    0,    0,  528,  109,  110,  111,  112,  113,  600,  114,
    0,    0,  115,  116,  117,  118,  119,    0,  528,    0,
    0,  528,  528,  686,  528,  601,    0,  645,  528,    0,
    0,  528,    0,  393,    0,    0,    0,    0,    0,  394,
    0,    0,    0,    0,  528,    0,    0,  602,    0,  395,
  528,    0,    0,  528,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,    0,  603,  604,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,  605,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,  872,
  873,  874,  528,  766,    0,    0,    0,    0,  948,    0,
    0,  381,    0,    0,    0,    0,    0,    0,    0,    0,
  382,    0,    0,    0,  528,    0,    0,    0,    0,    0,
    0,  528,    0,  383,    0,  528,  642,    0,    0,  384,
  385,    0,  528,    0,  766,    0,    0,  766,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  643,    0,
  528,    0,  766,    0,    0,  528,    0,  386,  640,    0,
  387,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,    0,    0,    0,  640,    0,    0,    0,    0,    0,
  528,    0,    0,    0,  528,    0,    0,  640,  389,  390,
    0,    0,    0,  640,  640,    0,    0,    0,    0,    0,
  391,  640,    0,    0,  392,  634,    0,  634,  634,    0,
  634,    0,  644,  951,    0,    0,    0,    0,    0,    0,
    0,  640,    0,  634,  640,    0,    0,    0,  634,    0,
  105,    0,  455,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  821,    0,    0,    0,    0,
  640,    0,  640,  640,    0,  645,    0,    0,  105,  106,
  107,  393,  108,    0,  640,    0,    0,  394,  640,  109,
  110,  111,  112,  113,    0,  114,    0,  395,  115,  116,
  117,  118,  119,    0,    0,    0,  821,  106,  107,  821,
  108,  396,  397,  398,  399,  202,  203,  109,  110,  111,
  112,  113,    0,  114,  821,  400,  115,  116,  117,  118,
  119,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,  640,  108,    0,    0,  411,
  412,  640,  413,  414,  415,  111,  112,  113,  416,  114,
    0,  640,  115,  116,  646,  647,  648,    0,    0,    0,
    0,    0,    0,    0,    0,  640,  640,  640,  640,  640,
  640,    0,    0,    0,    0,    0,    0,    0,    0,  640,
    0,    0,    0,    0,    0,    0,    0,    0,  640,  640,
  640,  640,  640,  640,  640,  640,  640,  640,  640,    0,
  640,    0,    0,  640,  640,    0,  640,  640,  640,  640,
  640,  640,  640,  640,    0,    0,  640,  640,  640,  640,
  640,  766,    0,    0,  766,    0,    0,    0,    0,    0,
    0,  766,    0,    0,  766,  634,    0,    0,  766,    0,
    0,    0,  634,  766,    0,  649,  766,    0,  417,    0,
  418,  634,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  766,  420,  634,    0,    0,    0,  277,    0,
  634,  634,    0,    0,    0,    0,    0,    0,  634,    0,
    0,    0,    0,    0,    0,    0,  766,    0,    0,  766,
  766,  697,  695,    0,  696,    0,  698,    0,  634,  766,
    0,  634,    0,    0,    0,    0,    0,    0,    0,  892,
  891,  893,    0,    0,    0,    0,  766,  821,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  634,    0,  634,
  634,  649,    0,    0,  417,    0,  418,    0,  766,    0,
    0,  634,    0,  766,    0,  634,    0,    0,    0,  420,
    0,    0,    0,    0,  277,  766,    0,  766,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  821,    0,    0,  821,    0,    0,    0,
    0,    0,    0,  821,    0,    0,  821,    0,    0,    0,
  821,    0,    0,    0,    0,  821,    0,    0,  821,    0,
    0,    0,  634,    0,    0,    0,    0,    0,  634,    0,
    0,    0,    0,    0,  821,    0,    0,    0,  634,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  634,  634,  634,  634,  634,  634,  821,    0,
    0,  821,  821,    0,    0,    0,  634,    0,    0,    0,
    0,  821,    0,    0,    0,  634,  634,  634,  634,  634,
  634,  634,  634,  634,  634,  634,    0,  634,  821,    0,
  634,  634,  381,  634,  634,  634,  634,  634,  634,  634,
  634,  382,    0,  634,  634,  634,  634,  634,    0,    0,
  821,    0,    0,    0,  383,  821,    0,    0,    0,    0,
  384,  385,    0,  105,    0,    0,  790,  821,    0,  821,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  643,
    0,  879,    0,    0,    0,    0,    0,    0,  386,  693,
    0,  387,  106,  107,  694,  108,    0,  880,    0,    0,
    0,    0,  109,  110,  111,  112,  113,  790,  114,    0,
  790,  115,  116,  117,  118,  119,    0,    0,  381,  389,
  390,  649,    0,    0,  417,  790,  418,  382,    0,    0,
    0,  391,    0,  881,    0,  392,    0,    0,    0,  420,
  383,    0,  882,  867,  277,    0,  384,  385,    0,    0,
  883,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  643,    0,    0,    0,  884,
    0,  885,    0,    0,  386,    0,    0,  387,    0,    0,
    0,    0,    0,    0,    0,  886,  645,  887,  888,  889,
    0,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,  389,  390,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,  391,    0,    0,
    0,  392,  396,  397,  398,  399,  202,  203,    0,  644,
    0,    0,    0,    0,    0,    0,  400,    0,  890,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,  645,  115,  116,  646,  647,  648,  393,    0,
    0,    0,    0,    0,  394,  791,    0,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,  791,    0,  790,  791,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,  791,    0,  411,  412,  381,  413,
  414,  415,  111,  112,  113,  416,  114,  382,    0,  115,
  116,  646,  647,  648,  459,    0,    0,    0,    0,    0,
  383,    0,    0,    0,    0,  479,  384,  385,  417,    0,
  418,  105,    0,    0,  790,    0,    0,  790,    0,    0,
    0,    0,    0,  420,  790,  643,    0,  790,  277,    0,
    0,  790,    0,    0,  386,    0,  790,  387,    0,  790,
  106,  107,    0,  108,    0,    0,    0,    0,    0,    0,
  109,  110,  111,  112,  113,  790,  114,    0,    0,  115,
  116,  117,  118,  119,    0,  389,  390,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  391,    0,  790,
    0,  392,  790,  790,    0,    0,    0,    0,    0,  867,
    0,    0,  790,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  479,    0,    0,  417,    0,  418,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,  790,  645,    0,    0,    0,  790,    0,  393,    0,
    0,    0,    0,    0,  394,    0,    0,    0,  790,    0,
  790,    0,    0,    0,  395,    0,    0,    0,    0,  389,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,    0,  791,    0,    0,
    0,    0,  400,    0,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,    0,    0,  115,
  116,  117,  118,  119,    0, 1105,    0,    0, 1106,    0,
    0,    0,  381,  791,    0,    0,  791,    0,    0,    0,
    0,  382,    0,  791,    0,    0,  791,    0,    0,    0,
  791,    0,    0,    0,  383,  791,    0,    0,  791,    0,
  384,  385,    0,   76,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,  791,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  386,    0,
    0,  387,    0,    0,  106,  410,    0,  108,  791,    0,
    0,  791,  791,  413,  414,  415,  111,  112,  113,    0,
  114,  791,    0,  115,  116,  117,  118,  119,    0,  389,
  390,    0, 1115,    0,    0, 1106,    0,    0,  791,  381,
    0,  391,    0,    0,    0,  392,    0,    0,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  791,  383,    0,    0,  562,  791,  479,  384,  385,  417,
    0,  418,    0,    0,    0,    0,    0,  791,    0,  791,
    0,    0,    0,    0,  420,    0,    0,    0,    0,  277,
    0,    0,    0,  562,  562,  386,  562,    0,  387,    0,
 1107,    0,  393,  562,  562,  562,  562,  562,  394,  562,
    0,    0,  562,  562,  562,  562,  562,    0,  395,    0,
    0,    0,    0,    0,    0,    0,  389,  390,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,  391,    0,
    0,    0,  392,    0,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    1,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    2,    0,    0,    0, 1107,    0,  393,
    0,    3,    0,    0,    0,  394,    0,    0,    4,    0,
    5,    0,    6,    0,    0,  395,    0,    7,  479,    0,
    0,  417,    0,  418,    8,    0,  948,    0,    0,  396,
  397,  398,  399,  202,  203,    0,  420,    0,    9,    0,
    0,  277,    0,  400,    0,    0,    0,    0,    0,   10,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108, 1635,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,    0,
  115,  116,  117,  118,  119,    0, 1119,    0,    0, 1106,
 1636,    0,    0,  381,    0,    0,    0,    0,    0,    0,
  949,    0,  382,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  383,   11,    0,    0,    0,
    0,  384,  385,   12,    0,    0,   13,  479,   14,   15,
  417,    0,  418,    0,    0,    0,  950,    0,    0,    0,
    0,  951,    0,    0,    0,  420,    0,  952, 1637,  386,
  277,    0,  387,   16,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,    0,  953,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  389,  390,  105,    0,    0,    0,    0,  106,  107,    0,
  108,    0,  391,    0,    0,    0,  392,  109,  110,  111,
  112,  113,   17,  114,    0,    0,  115,  116,  117,  118,
  119,  106,  107,    0,  108,    0,    0,   18,   19,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0, 1127,    0,
    0, 1106,    0,    0,    0,  381,    0,    0,    0,    0,
    0, 1107,    0,  393,  382,    0,    0,    0,    0,  394,
    0,    0,    0,    0,    0,    0,    0,  383,    0,  395,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,  386,    0,    0,  387,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,  389,  390,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,  391,    0,    0, 1130,  392,    0,
 1106,    0,    0,    0,  381,    0,    0,    0,    0,    0,
    0,    0,    0,  382,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  383,  280,    0,    0,
    0,  479,  384,  385,  417,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  420,
    0,    0,    0, 1107,  277,  393,  280,  280,    0,  280,
  386,  394,    0,  387,    0,    0,  280,  280,  280,  280,
  280,  395,  280,    0,    0,  280,  280,  280,  280,  280,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,  389,  390,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,  391,    0,    0,    0,  392,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1107,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
    0,  420,    0,    0,    0,    0,  277,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
    0, 1135,    0,    0, 1106,    0,    0,    0,  381,    0,
    0,    0,    0,    0,    0,    0,    0,  382,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,    0,    0,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,    0,    0,    0,  386,  277,    0,  387,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  389,  390,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  391,    0,    0,
    0,  392,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1138,    0,    0, 1106,    0,    0,    0,
  381,    0,    0,    0,    0,    0, 1107,    0,  393,  382,
    0,    0,    0,    0,  394,    0,    0,    0,    0,    0,
    0,    0,  383,    0,  395,    0,    0,    0,  384,  385,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,  386,  815,    0,  387,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,  389,  390,  115,
  116,  117,  118,  119,    0,    0,    0,    0,  815,  391,
    0,  815, 1141,  392,    0, 1106,    0,    0,    0,  381,
    0,    0,  479,    0,  484,  417,  815,  418,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,  383,    0,    0,    0,  277,    0,  384,  385,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1107,    0,
  393,    0,    0,    0,    0,  386,  394,    0,  387,    0,
    0,    0,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,  389,  390,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,  391,    0,
    0,    0,  392,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1107,    0,  393,
    0,    0,    0,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,  815,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,  381,
  413,  414,  415,  111,  112,  113,  416,  114,  382,    0,
  115,  116,  117,  118,  119,    0,    0,    0,    0,  479,
    0,  383,  417,    0,  418,    0,    0,  384,  385,    0,
    0,    0,    0,    0,    0,  815,    0,  420,  815,    0,
    0,    0,  277,    0,    0,  815,    0,    0,  815,    0,
    0,    0,  815,    0,    0,  386,    0,  815,  387,    0,
  815,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  815,    0,    0,    0,
    0,    0,    0,    0,  388,    0,  389,  390,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  391,    0,
  815,    0,  392,  815,  815,  955,    0,  955,  955,    0,
  955,    0,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  955,    0,    0,    0,    0,  955,    0,
  815,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  815,    0,    0,    0,    0,  815,    0,  393,
    0,    0,    0,    0,    0,  394,    0,    0,    0,  815,
    0,  815,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,    0,
  115,  116,  117,  118,  119,    0,  381,    0,    0,    0,
    0,    0,    0,    0,    0,  382,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  383,    0,
    0,  105,    0,    0,  384,  385,    0,    0,    0,    0,
  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
  172,  173,  174,  175,  176,  177,    0,    0,  178,  179,
  106,  107,  386,  108,    0,  387,  180,  181,    0,    0,
  109,  110,  111,  112,  113,  182,  114,  183,    0,  115,
  116,  117,  118,  119,    0,    0,    0,    0,    0,    0,
    0,  388,  955,  389,  390,    0,    0,    0,    0,    0,
    0,  955,    0,    0,    0,  391,    0,    0,    0,  392,
    0,    0,  640,    0,  955,  640,    0,  640,    0,    0,
  955,  955,    0,    0,    0,    0,    0,    0,    0,    0,
  640,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  955,    0,
    0,  955,    0,    0,    0,    0,  686,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,  955,    0,  955,
  955,    0,  395,    0,    0,    0,    0,    0,    0,    0,
    0,  955,    0,    0,    0,  955,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
  320,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,  955,    0,    0,    0,    0,    0,  955,    0,
  320,    0,    0,    0,    0,    0,    0,  320,  955,    0,
    0,  320,  320,    0,  320,    0,    0,  320,    0,    0,
    0,    0,  955,  955,  955,  955,  955,  955,    0,    0,
    0,  320,    0,    0,    0,    0,  955,  320,    0,    0,
    0,    0,    0,    0,    0,  955,  955,  955,  955,  955,
  955,  955,  955,  955,  955,  955,    0,  955,    0,    0,
  955,  955,    0,  955,  955,  955,  955,  955,  955,  955,
  955,    0,  640,  955,  955,  955,  955,  955,    0,  640,
    0,    0,  419,    0,    0,  417,    0,  418,  640,    0,
    0,    0,    0,    0,    0,    0,    0,  320,    0,    0,
  420,  640,    0,    0,    0,  277,    0,  640,  640,    0,
    0,    0,    0,    0,    0,  640,  320,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  320,    0,    0,    0,  640,    0,    0,  640,    0,
    0,  320,    0,    0,    0,    0,  479,    0,    0,  417,
  320,  418,  320,    0,    0,    0,    0,    0,    0,  320,
    0,    0,    0,    0,  420,    0,  640,  640,    0,  277,
    0,    0,    0,    0,    0,    0,    0,    0,  640,    0,
    0,    0,  640,    0,    0,    0,    0,    0,  320,  320,
    0,  320,    0,    0,    0,    0,    0,    0,  320,  320,
  320,  320,  320,    0,  320,    0,    0,  320,  320,  320,
  320,  320,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  640,
    0,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  640,
  640,  640,  640,  640,  640,    0,    0,    0,    0,    0,
    0,    0,    0,  640,    0,    0,    0,    0,    0,    0,
    0,    0,  640,  640,  640,  640,  640,  640,  640,  640,
  640,  640,  640,    0,  640,    0,    0,  640,  640,  381,
  640,  640,  640,  640,  640,  640,  640,  640,  382,    0,
  640,  640,  640,  640,  640,    0,    0,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,  381,    0,    0,    0,    0,    0,    0,
    0,    0,  382,    0,    0,    0,  479,    0,    0,  417,
    0,  418,    0,    0,  388,  383,  389,  390,    0,    0,
    0,  384,  385,    0,  420,    0,    0,    0,  391,  277,
    0,    0,  392,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
  389,  390,    0,    0,    0,    0,    0,    0,  420,  393,
    0,    0,  391,  277,    0,  394,  392,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,  393,  108,    0,    0,  411,  412,  394,
  413,  414,  415,  111,  112,  113,  416,  114,    0,  395,
  115,  116,  117,  118,  119,    0,    0,  678,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,  381,  115,  116,  117,  118,  119,    0,
    0,    0,  382,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  383,    0,    0,    0,    0,
    0,  384,  385,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,  381,    0,    0,
  479,  932,    0,  417,    0,  418,  382,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  420,  383,
  389,  390,    0,  277,    0,  384,  385,    0,    0,    0,
    0,    0,  391,    0,    0,    0,  392,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  388,  686,  389,  390,    0,    0,    0,    0,
    0,    0,    0,  393,    0,    0,  391,    0,  479,  394,
  392,  417,    0,  418,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
    0,  277,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,  393,  108,    0,
    0,  411,  412,  394,  413,  414,  415,  111,  112,  113,
  416,  114,    0,  395,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,  586,
    0,  400,    0,  587,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,  588,  108,    0,  589,  411,  412,  381,  413,  414,
  415,  111,  112,  113,  416,  114,  382,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,    0,  383,
    0,    0,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,  590,    0,    0,    0,    0,
    0,  591,  592,  593,    0,  594,  595,  596,    0,  597,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  598,
    0,  599,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  381,  391,    0,  479,    0,
  392,  417,  600,  418,  382,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,  383,    0,  601,
    0,  277,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  602,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,    0,    0,  387,    0,    0,  393,    0,    0,
    0,    0,  479,  394,    0,  417,    0,  418,    0,    0,
    0,    0,    0,  395,    0,    0,  603,  604,    0,    0,
  420,    0,  389,  390,    0,  277,    0,  396,  397,  398,
  399,  202,  203,  605,  391,    0,    0,    0,  392,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1133,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,  381,  413,  414,  415,  111,
  112,  113,  416,  114,  382,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,  381,
    0,    0,    0,    0,    0,    0,    0,    0,  382,    0,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,  383,  389,  390,    0,    0,    0,  384,  385,    0,
  420, 1703,    0,    0,  391,  277,    0,    0,  392,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  479,    0,    0,  417,
    0,  418,    0,    0,    0, 1317,  389,  390,    0,    0,
    0,    0,    0,    0,  420,  393,    0,    0,  391,  277,
    0,  394,  392,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,  393,
  108,    0,    0,  411,  412,  394,  413,  414,  415,  111,
  112,  113,  416,  114,    0,  395,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0, 1330,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,  381,
  115,  116,  117,  118,  119,    0,    0,    0,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,  381,    0,    0,  955,    0,    0,  955,
    0,  955,  382,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  955,  383,  389,  390,    0,  955,
    0,  384,  385,    0,    0,    0,    0,    0,  391,    0,
    0,    0,  392,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  389,  390,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,  391,    0,    0,  394,  392,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,  393,  108,    0,    0,  411,  412,  394,
  413,  414,  415,  111,  112,  113,  416,  114,    0,  395,
  115,  116,  117,  118,  119,    0,    0,    0,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,  523,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,  529,  108,    0,
    0,  411,  412,  955,  413,  414,  415,  111,  112,  113,
  416,  114,  955,    0,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,  546,  955,  523,    0,    0,  523,
    0,  955,  955,    0,    0,    0,    0,  529,  529,    0,
    0,  529,    0,    0,  523,    0,  523,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  529,    0,  529,  955,
    0,    0,  955,    0,    0,  546,    0,    0,  546,    0,
  536,    0,    0,    0,    0,    0,  523,    0,    0,    0,
    0,    0,    0,  546,  520,  546,    0,    0,  529,    0,
  955,  955,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  955,    0,    0,    0,  955,    0,    0,    0,
    0,  536,    0,    0,  536,  546,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  520,    0,    0,  520,  536,
    0,  536,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  520,    0,  520,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  536,    0,  955,    0,    0,    0,    0,    0,  955,
    0,    0,    0,    0,    0,  520,    0,    0,    0,  955,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  955,  955,  955,  955,  955,  955,    0,
    0,    0,    0,    0,    0,    0,    0,  955,    0,    0,
    0,    0,    0,    0,    0,    0,  955,  955,  955,  955,
  955,  955,  955,  955,  955,  955,  955,    0,  955,    0,
    0,  955,  955,    0,  955,  955,  955,  955,  955,  955,
  955,  955,    0,  523,  955,  955,  955,  955,  955,    0,
    0,    0,    0,    0,    0,  529,    0,    0,    0,  523,
    0,    0,  523,  523,    0,  523,    0,    0,    0,  523,
    0,  529,  523,    0,  529,  529,  831,  529,    0,    0,
    0,  529,  546,    0,  529,  523,    0,    0,    0,    0,
    0,  523,    0,    0,  523,    0,    0,  529,  546,    0,
    0,    0,  546,  529,  546,    0,  529,    0,  546,    0,
    0,  546,    0,    0,    0,    0,    0,  831,  993,  993,
  831,  993,  993,  993,  546,    0,    0,    0,  536,    0,
  546,    0,    0,  546,    0,  831,  993,  993,  993, 1414,
    0,    0,  520,    0,  536,    0,    0,    0,  536,    0,
  536,    0,    0,    0,  536,    0,    0,  536,  520,    0,
    0,    0,  520,  523,  520,    0,    0,  993,  520,    6,
  536,  520,    0,    0,    0,  529,  536,    0,    0,  536,
 1533, 1534,    0, 1535,  520,  523, 1536,    0,    0,    0,
  520,    0,  523,  520,    0,    0,  523,  529,    0,    0,
 1537,    0,  546,  523,  529,    0, 1538,    0,  529,    0,
    0,    0,    0,    0,    0,  529,    0,    0,    0,    0,
    0,  523,    0,    0,  546,    0,  523,    0,    0,    0,
    0,  546,    0,  529,    0,  546,    0,    0,  529,  794,
    0,    0,  546,    0,    0,    0,    0,    0,  536,    0,
    0,  523,    0,    0,    0,  523,    0,    0,    0,    0,
  546,    0,  520,  529,    0,  546, 1539,  529,    0,    0,
  536,  832,    0,    0,    0,    0,    0,  536,    0,    0,
  794,  536,    0,  794,  520,   14,    0,    0,  536,    0,
  546,  520,    0,    0,  546,  520,    0,    0,  794,    0,
 1540,    0,  520,    0,    0,    0,  536,    0,    0,    0,
   16,  536,  832,  994,  994,  832,  994,  994,  994, 1664,
  520, 1541,    0,    0,    0,  520,    0,    0,  105,    0,
  832,  994,  994,  994,    0,    0,  536,    0,  831,    0,
  536,    0,    0,    0,    0,    0,    0,    0,  993,    0,
  520,    0,    0,    0,  520,    0,  993, 1542,  107,    0,
  108,  993,  994,    0,  993,    0,    0,  109,  110,  111,
  112,  113,    0,  114,    0,    0,  115, 1543,  117,  118,
  119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  831,    0,    0,  831,    0,    0,
  993,    0,    0,    0,  831,    0,    0,  831,    0,  993,
    0,  831,    0,    0,    0,    0,  831,  993,    0,  831,
    0,    0,    0,    0,  833,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  831,  993,    0,  993,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  993,    0,  993,  993,  993,    0,    0,  831,
    0,    0,  831,  831,    0,  833,  995,  995,  833,  995,
  995,  995,  831,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  833,  995,  995,  995,    0,  792,  831,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  794,    0,    0,    0,  993,    0,    0,    0,    0,
    0,  831,    0,    0,    0,  995,  831,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  831,  792,
  831,    0,  792,  832,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  994,    0,    0,    0,  792,    0,    0,
    0,  994,    0,    0,    0,    0,  994,  794,    0,  994,
  794,    0,    0,    0,    0,    0,    0,  794,    0,    0,
  794,    0,    0,    0,  794,    0,    0,  793,    0,  794,
    0,    0,  794,    0,    0,    0,    0,    0,    0,  832,
    0,    0,  832,    0,    0,  994,    0,    0,  794,  832,
    0,    0,  832,    0,  994,    0,  832,    0,    0,    0,
    0,  832,  994,    0,  832,    0,    0,    0,  793,    0,
    0,  793,  794,    0,    0,  794,  794,    0,    0,    0,
  832,  994,    0,  994,    0,  794,  793,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  994,    0,  994,
  994,  994,  794,    0,  832,    0,    0,  832,  832,    0,
    0,    0,    0,    0,    0,    0,    0,  832,    0,    0,
    0,    0,    0,  823,  794,    0,    0,    0,    0,  794,
    0,    0,    0,    0,  832,    0,  833,    0,    0,    0,
    0,  794,    0,  794,    0,    0,  995,    0,    0,    0,
  994,    0,    0,    0,  995,    0,  832,    0,    0,  995,
    0,  832,  995,    0,  823,    0,    0,  823,    0,    0,
    0,    0,    0,  832,    0,  832,    0,    0,    0,    0,
    0,    0,  823,  787,    0,    0,    0,    0,    0,    0,
    0,    0,  833,    0,    0,  833,    0,    0,  995,    0,
  792,    0,  833,  788,    0,  833,    0,  995,    0,  833,
    0,    0,    0,    0,  833,  995,    0,  833,    0,    0,
    0,    0,    0,    0,  787,    0,    0,  787,    0,    0,
    0,    0,    0,  833,  995,    0,  995,    0,    0,    0,
    0,    0,  787,    0,  788,    0,    0,  788,    0,    0,
  995,    0,  995,  995,  995,    0,  792,  833,    0,  792,
  833,  833,  788,  789,    0,    0,  792,    0,    0,  792,
  833,    0,    0,  792,    0,    0,    0,    0,  792,    0,
    0,  792,    0,    0,    0,    0,    0,  833,    0,  793,
    0,    0,    0,    0,    0,    0,    0,  792,    0,    0,
    0,    0,    0,  995,  789,    0,    0,  789,    0,  833,
    0,    0,    0,    0,  833,    0,    0,    0,    0,    0,
    0,  792,  789,    0,  792,  792,  833,    0,  833,    0,
    0,    0,    0,    0,  792,  822,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  793,    0,    0,  793,    0,
    0,  792,    0,    0,    0,  793,    0,    0,  793,    0,
    0,    0,  793,    0,    0,    0,    0,  793,    0,    0,
  793,  816,    0,  792,    0,    0,  822,    0,  792,  822,
    0,    0,    0,    0,    0,  823,  793,    0,    0,    0,
  792,    0,  792,    0,  822,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  793,    0,  816,  793,  793,  816,    0,    0,    0,    0,
    0,    0,    0,  793,    0,    0,    0,    0,    0,    0,
  816,  824,    0,    0,    0,    0,    0,    0,    0,    0,
  793,  823,    0,    0,  823,  787,    0,    0,    0,    0,
    0,  823,    0,    0,  823,    0,    0,    0,  823,    0,
    0,    0,  793,  823,    0,  788,  823,  793,    0,    0,
    0,    0,  824,    0,    0,  824,    0,    0,    0,  793,
    0,  793,  823,    0,    0,    0,    0,    0,    0,    0,
  824,  825,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  787,    0,    0,  787,    0,  823,    0,    0,  823,
  823,  787,    0,    0,  787,    0,    0,    0,  787,  823,
    0,  788,    0,  787,  788,  789,  787,    0,    0,    0,
    0,  788,  825,    0,  788,  825,  823,    0,  788,    0,
    0,  813,  787,  788,    0,    0,  788,    0,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,  823,    0,
    0,    0,  788,  823,    0,    0,  787,    0,    0,  787,
  787,    0,    0,    0,    0,  823,    0,  823,    0,  787,
    0,  789,  813,    0,  789,  813,  788,    0,    0,  788,
  788,  789,    0,    0,  789,    0,  787,  822,  789,  788,
  813,  817,    0,  789,    0,    0,  789,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  788,    0,  787,    0,
    0,    0,  789,  787,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  816,    0,  787,    0,  787,  788,    0,
    0,    0,  817,  788,    0,  817,  789,    0,    0,  789,
  789,    0,    0,  822,    0,  788,  822,  788,    0,  789,
  817,    0,    0,  822,    0,    0,  822,    0,    0,    0,
  822,    0,    0,    0,    0,  822,  789,    0,  822,    0,
    0,    0,    0,    0,  826,    0,    0,    0,    0,  816,
    0,    0,  816,  824,  822,    0,    0,    0,  789,  816,
    0,    0,  816,  789,    0,    0,  816,    0,    0,    0,
    0,  816,    0,    0,  816,  789,    0,  789,  822,    0,
    0,  822,  822,    0,    0,  826,    0,    0,  826,    0,
  816,  822,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  826,    0,    0,    0,    0,  822,  824,
    0,    0,  824,  825,  816,    0,  814,  816,  816,  824,
    0,    0,  824,    0,    0,    0,  824,  816,    0,    0,
  822,  824,    0,    0,  824,  822,    0,    0,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  822,    0,  822,
  824,    0,  818,    0,    0,    0,    0,  814,    0,    0,
  814,    0,    0,  813,    0,    0,  816,    0,    0,  825,
    0,  816,  825,    0,  824,  814,    0,  824,  824,  825,
    0,    0,  825,  816,    0,  816,  825,  824,    0,    0,
    0,  825,    0,  818,  825,    0,  818,    0,    0,    0,
    0,    0,    0,    0,  824,    0,    0,    0,    0,    0,
  825,  818,    0,    0,    0,    0,    0,    0,    0,  813,
    0,    0,  813,  817,    0,    0,  824,    0,    0,  813,
    0,  824,  813,    0,  825,    0,  813,  825,  825,    0,
    0,  813,    0,  824,  813,  824,    0,  825,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  813,    0,    0,    0,  825,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  817,
    0,    0,  817,    0,  813,    0,  825,  813,  813,  817,
    0,  825,  817,    0,    0,    0,  817,  813,    0,    0,
    0,  817,    0,  825,  817,  825,  826,    0,    0,    0,
    0,    0,    0,    0,  813,    0,    0,    0,    0,    0,
  817,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  813,    0,    0,    0,
    0,  813,    0,    0,  817,    0,    0,  817,  817,    0,
    0,    0,    0,  813,    0,  813,    0,  817,    0,    0,
    0,    0,  826,    0,    0,  826,    0,    0,    0,    0,
    0,    0,  826,    0,  817,  826,    0,    0,  814,  826,
    0,    0, 1414,    0,  826,    0,    0,  826,    0,    0,
    0,    0,    0,    0,    0,    0,  817,    0,    0,    0,
    0,  817,    0,  826,    0,    0,    0,    0,    0,    0,
    0,    0,    6,  817,  818,  817,    0,    0,    0,    0,
    0,    0,    0, 1533, 1534,    0, 1535,  826,    0, 1536,
  826,  826,    0,    0,  814,    0,    0,  814,    0,    0,
  826,    0,    0, 1537,  814,    0,    0,  814,    0, 1538,
    0,  814,    0,    0,    0,    0,  814,  826,    0,  814,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  818,    0,    0,  818,    0,  814,    0,    0,    0,  826,
  818,    0,    0,  818,  826,    0,    0,  818,    0,    0,
    0,    0,  818,    0,    0,  818,  826,    0,  826,  814,
    0,    0,  814,  814,    0,    0,    0,    0,    0, 1539,
    0,  818,  814,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14,  814,
    0,    0,    0,    0,    0,  818,    0,    0,  818,  818,
    0,    0,    0, 1540,    0,    0,    0,    0,  818,    0,
    0,  814,    0,   16,    0,    0,  814,    0,  586,    0,
    0,    0,  587,    0, 1541,  818,    0,    0,  814,  785,
  814,  105,    0,    0,    0,    0,    0,    0,    0,    0,
  588,    0,    0,  589,    0,    0,    0,  818,    0,    0,
    0,    0,  818,    0,    0,    0,    0,    0,    0,    0,
 1542,  107,    0,  108,  818,    0,  818,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
 1543,  117,  118,  119,  590,    0,    0,    0,    0,  586,
  591,  592,  593,  587,  594,  595,  596,    0,  597,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  588,    0,    0,  589,    0,    0,    0,  598,    0,
  599,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  600,    0,    0,    0,  590,    0,    0,    0,    0,
    0,  591,  592,  593,    0,  594,  595,  596,  601,  597,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  598,
  602,  599,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  600,    0,    0,  603,  604,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,  601,
    0,    0,  605,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  602,    0,  220,    0,    0,    0,  220,    0,    0,
    0,    0,    0,    0,    0,    0,  105,    0,    0,    0,
    0,  220,  220,    0,    0,  220,  220,    0,  220,    0,
    0,    0,    0,    0,    0,    0,  603,  604,    0,    0,
  220,    0,    0,    0,  220,  106,  107,  220,  108,    0,
    0,    0,    0,  605,    0,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,  220,
    0,    0,    0,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,    0,  133,    0,    0,  220,  133,
    0,    0,    0,    0,  220,  220,  220,    0,  220,  220,
  220,    0,  220,  220,    0,  220,    0,  133,    0,    0,
  133,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  220,    0,  220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,    0,    0,    0,
    0,   80,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  133,    0,  220,    0,  220,    0,  133,  133,  133,
    0,  133,  133,  133,    0,  133,    0,    0,    0,    0,
    0,    0,  220,    0,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  133,    0,  133,    0,    0,
    0,    0,    0,    0,  220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,  220,    0,    0,    0,    0,    0,    0,  133,    0,
    0,    0,    0,    0,    0,    0,    0,  220,    0,  220,
  220,    0,    0,    0,    0,  133,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,    0,    0,    0,
    0,    0,    0,    0,    0,  448,    0,  133,    0,    0,
    0,    0,    0,    0,    0,  448,  448,  448,  448,  448,
  448,  448,  448,  448,  448,  448,  448,  448,  448,  448,
    0,    0,  448,  448,  448,  448,    0,  448,    0,    0,
  448,  448,  133,  133,  448,  448,  448,  448,  448,  448,
  448,  448,    0,  448,  448,  448,  448,  448,    0,  133,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  149,   18,  104,
    0,   63,  351,  254,  359,   56,  138,  139,  359,  355,
  509,  138,  139,  701,   85,  555,   67,   63,   57,   73,
  328,   94,   62,  546,  330,   89,  549,  285,  666,  492,
   69,  419,  555,   53,   54,   55,   56,   57,   58,   90,
  516,   95, 1029,   63,   64,   65,   97,   67,  928,   69,
   70, 1022,   72,   73, 1248,   70,  467,  262, 1029,  376,
  748,   81,   83,   83,   84,   85,  271,   87,   88,   89,
   90,   92,   92,   93,   94,   95,  364,   97,   93,  152,
 1022,  323, 1022,  129,  762,  131,  966, 1029, 1022, 1029,
  851,  130,   68,   88, 1022, 1029,   44,  638,  279,  150,
   40, 1029, 1077,  719,  686,    0,  976,  755,  820,  129,
  130,  131,  151, 1054,  752,   91,    0,    0,    0,    0,
  306,  226,    0,  143,   59,   45,  631,   40, 1030,  149,
  150,  151,  152,  715,  782,   45,  156,   40,    0, 1041,
    0,  678,   45,  791,  758,   41,   41,  187,  143,   44,
    0,   41,    0,    0,  306,    0,   42,   41, 1060,   44,
   41,    0,  719,   44,   59,   40,  644,   44,  419,   41,
   44,  156,  212, 1191,    0,   59,   59,   59,   59,  525,
 1394,   59,    0,  532,  533,  534,  535,    0,   63,    0,
   45,   41,   45,   44,   44,   41,  844,   59, 1078,   59,
  555,   44,   41,    0,  555,   44,   63, 1454,  313,   59,
  256,   59,   59,    0,   59,   41,    0,   44,   44, 1394,
   59,  260,   44,  511, 1394,    0,  290,  274,  700,  728,
  269,   40,    0,   59,   44,  581,  256, 1246,  260,    0,
  260,   59,  262,  263,   41,   45,   59,   44,   59,  269,
  558,  271,    0,  262,  459,  294,  260,  264,   40, 1394,
  306,   12,   59,   63,  355,   44,   41,  159,  269,  586,
  290,  320,   59,  455,  294,   59,    0,   41,   61,  338,
   44,   63,  377,  329,   59,  808,   91, 1274,  445, 1296,
  336,   59,  338,   41,  340,    0,   44,  343,   59,  607,
  277,  265,  273, 1274,    0,  307,  308,    0,  372,  329,
  361,   59,  352, 1195,  354,  467,  336,   41,  338,   41,
  340,  356,    0,  343,   41,  258,  283,    0,  368,  967,
    0,  370, 1274, 1241, 1274,   59,   41, 1207,  302, 1280,
 1274,  361,  362,  959,    0, 1366, 1274,  362,  217,  997,
  370,  508,  372,  448,   59,  971,  972,  894,  895,    0,
 1381, 1263,  376,   59,  417,  828,   59,  403,  345,  340,
    0,   41,  301,  306,  260,  316,  317,  386,    0, 1287,
 1574,   59,  320,  442,  420,  136,   59,  436,  925,   59,
  262,  444, 1040,  871,  429,  417,  278,  278,  280,  280,
  420,  492,  959,   59,  411,   44,  287,  458,  666,  338,
 1419, 1293,   41,  417,  971,   44,  455,  303,   59,  466,
 1634,  467,  473,  318,   40,  317,  435,  466,  318,   59,
   41,  470,  683, 1680,  318,  455,  475,   59,  458,  459,
  309,  573,    0,  320,  328,  340,  466,  331,  699,  451,
  470, 1458,  278,  473,  280,  475,  340,  340,  478, 1634,
  269,  287,  770,  309, 1634,  773,  328,  318, 1082,  331,
  490,  729,  277,  318,  475,  410, 1451,  304, 1672,  787,
 1192,  394,  439,  328, 1021,  418,  331, 1505,  348, 1397,
  530,  387,  340,  478,  752,  340,  378,  378,  320, 1634,
  318,   59,  383,  387,  402,  710,  814,  447, 1410,  555,
  458,  431,  396, 1415,  386, 1417,   41,  300,  331,  400,
    0,  318,  340, 1393, 1465, 1286,  407,  769,  431,  340,
  278, 1725,  280,  383,  554,  555,  576,    0,  458,  287,
  560,    0,  387,  340,  725, 1223, 1087,  864,  458,  855,
 1452,  396,  378,  964,  842,  458,  440, 1269,  342,  454,
  441, 1098, 1099,  749, 1101, 1102, 1071,  452,  452, 1763,
  454,  454,  586,    0,  400,  485,  454,  254,  440,   59,
  454,  407,   41,  458,  630,   44, 1202,  627,  628,  436,
 1470,  264,  454,  377,  318,  440,   59,  749,  574,  276,
   59,  458,  734,  458,  383,  458,  454,  502,  285,  454,
  630,  454,  387,  318,   41,  441,  340,   44,   41,  639,
  501,  405, 1524,  264,  408,  639,  436,  440, 1530,  639,
  378,  431,   59,  765,  766,  340,  454, 1285, 1110,  316,
  424,  454,  410,  454,  340,  274,    0,  411,  409,  669,
   41,  501,  400,  440, 1072,  348,  702,  454,  458,  407,
 1009,    0,  501,  387,  386,  304,  450,  454,  284,  386,
 1691,  722,  396,  693,  669,  501,  458, 1579, 1586,   42,
  309,  701,  702,  552, 1592,  705,   44,  794,   41,  367,
  710,    0, 1229,  441, 1596,    0,  318,  473,  328,  719,
  258,  331,  722,  749,  743,   59,  745,  727,  754,  967,
  730,  367,  758, 1700, 1251,   41,  440,  387,  340, 1027,
   59,  761,  762,  743, 1073,  745,  396, 1405,  748, 1700,
  454, 1039,    0,  779,  754, 1317,  413,  262,  758,    0,
  417,  418,  419,  948, 1382,  262,  951,  286,  306,  454,
   59, 1059,  621,  501,   59,   41,  397,  262, 1700,  779,
 1700,  807,  808,  325, 1022,  387, 1700, 1184,  268, 1186,
    0, 1029, 1700, 1249,  396,    0,  387, 1679,  258, 1085,
  273,    0,  411,   44, 1172,  396, 1688,  807,  808,   41,
   12,   59,  768,  262,    0,    0,  300,    0,   59,  813,
 1218,  283,  479,  313,  843, 1345,  483,    0,    0, 1272,
  440,  825,  964,   59,  853, 1278,  260, 1205,  440,  278,
 1682,  280,  452,  843,  454,  409,  306, 1729,  287,   59,
  806,   42,  454,  853,   59,   41,   47,  466,    0,  301,
   59,    0, 1148, 1394,  460,  865,    0,  301,  352,   41,
  864, 1568,   44,   59,   59,  318,   59,  348, 1395,  303,
  418,  386,  838,  839,  840,  365,   59,   59,  430,  386,
 1732,  262,   40,  489,  490,  437,  492,  340,  866,  952,
  506,  386,  870,  499,  500,  501,  502,  503,   44,  505,
   58,  318,  508,  509,  510,  511,  512,   59,  949,  953,
   59,  328,  384,   59,  331,   59,  264,  417,  331,  262,
  264,   16, 1774,  340,  136, 1632,  343,  386,  964,  378,
  257, 1172,  461,  462,  387, 1057, 1033, 1034,  948,  949,
  950,  951,  952,  953,  261,  950,  262, 1195,  418,  959,
    0,  400,  644,    0, 1290, 1633,  722,  649,  407,   40,
  259,  971,  972, 1261, 1205,  264,  976,  439,  493,  264,
  387,  270, 1162,  258,  387, 1165,  435, 1384,  503,  396,
  461,  462,  649,  396,  456, 1082,  262,  440,   83,  318,
  442,   41,  441,  402, 1292, 1737, 1738,    0,  442,  666,
 1345,  454,  331,  670, 1345,  386,  319,  334,  263,   59,
  677,  340,   59,  387, 1503,  682,  683, 1506,   40, 1532,
  278,  306,  280,  440, 1432,  692, 1274,  694,  695,  696,
  697,  698,  699,  700, 1406,  452,  445,  454,   41, 1075,
 1050,   44, 1450,  386,  264, 1293, 1082, 1051,  458,  264,
  257, 1051,  501,  397, 1064,  264,   59,  312,  387,  405,
    0,  458,  729, 1402, 1206, 1075,  440,  396,  323,  264,
  386,  264, 1082, 1190, 1191, 1192,  458,  328,  424,  280,
  331, 1453,  264, 1825,  285,  752,  328,  400,  485,  331,
  757,  397,  343, 1634,    0,  278,  322,  280,  397,  508,
 1066,   41,  397, 1225, 1631, 1227, 1628,  460, 1630, 1598,
  386,  440,  264,  401,   44,  264,  307,  308, 1084,   59,
 1661,    0,  318, 1421, 1090,  454, 1653,  334,  441, 1656,
  436,    0,  328,  418, 1382,  331,  489,  490,  320,  492,
  278,  301,  322,  417,  340,  387,  499,  500,  501,  502,
  503,  436,  505,   59,  396,  508,  509,  510,  511,  512,
 1778, 1779, 1460,   93,  470, 1687, 1288, 1708,    0, 1359,
  444,   44,  427, 1363, 1184,  867, 1186,  397,  338,  871,
   59,  319,  397,  301,    0,  470, 1190, 1191,  397,  440,
   59,  387, 1202, 1223,  265, 1161, 1206, 1207,  342,  402,
  396,  452,  397,  454,  397,  280,    0, 1748,  260,  279,
  285,   40,  879,  880,  881, 1756, 1834,  884,  885,  886,
  338,  888,  889,  890,  891,  892,  893,   59,    0,  896,
  897,  302,  899,  377,  901,  438,   44,  904,  905,  906,
  907,  908,  909,   59,  440,  397, 1787, 1736,  397, 1790,
   44,  303, 1394,    0,  921, 1777,  923,    0,  454,  329,
  451,  405,  400, 1267,  408,   59,  337, 1267,  318,   41,
  452, 1812, 1414, 1303,    0,  272,    0, 1307,  460,  383,
  424,  331,  442,   42,   43,   93,   45,   59,   47,  407,
  340,  288, 1684,  363,   41, 1593,    0, 1824,   41,  417,
  967,   44,  460,  441,  307,  308,  450,  489,  490, 1499,
  492,  381,   59,  385,    0,  383,   59,  499,  500,  501,
  502,  503,   44,  505,  442,    0,  508,  509,  510,  511,
  512,  489,  490,   59,  492,   59,  333,  387, 1730,  260,
  405,  499,  500,  501,  502,  503,  396,  505,  259,  259,
  508,  509,  510,  511,  512,   59,    0,  377,  321,  270,
  270, 1413,  321,  405,  343,  362,   41,   44,  465,   44,
  467,   93,  335,   59, 1384, 1405,  335, 1413, 1414,  460,
  343, 1773,  303, 1393,   59,    0,   61,   42, 1505,  368,
  440,  456,    0, 1422,   42,   43,  359,   45,  418,   47,
  359, 1693, 1694, 1413,  454, 1527,  260,  410,  489,  490,
 1552,  492, 1422, 1805,  456,   59,   93,  260,  499,  500,
  501,  502,  503,  383,  505,   46,   45,  508,  509,  510,
  511,  512,  429,  493,  494,    0,   42,   43,  460,   45,
   61,   47, 1734, 1110,   59,  465,   44,  426,  451,  303,
  413,   59, 1700,   59,  413, 1122, 1123,  383, 1125, 1126,
  303,   59,  348,  279, 1705,  402, 1133,  489,  490, 1708,
  492,  357, 1533,  342,    0,   58,    0,  499,  500,  501,
  502,  503, 1577,  505,  300, 1152,  508,  509,  510,  511,
  512, 1495, 1634, 1160,   59, 1162, 1552,  434, 1165,  294,
 1167, 1505,  436,  352, 1171, 1172, 1173, 1174,  377, 1748,
  305,    0,  449,  329, 1806,   41, 1552,  312,   44, 1661,
   44,  280,  494, 1533, 1578,   46,  285,  342, 1195,  378,
    0,  378,  348,   59,  328,   59,  405,  331, 1205,  408,
   61,  357,  337,    0, 1573,    0, 1536,  363, 1787,  343,
  337, 1790,    0,  400,  418,  424,  328,    0, 1568,  331,
  407,  320,  377, 1573,  258,  381, 1708,  261, 1578,    0,
   59,   41, 1602, 1812,   44,  318, 1606, 1244, 1673,  461,
  462,  450,  287,  258,  331,  328, 1597, 1597,  331,   59,
  405,   61,  337,  408,  441,  300,  274,  340,   41,  274,
  343,   44,   59,  318,   59, 1646, 1748,  402,    0,  424,
   41,   59,  306,   44, 1756,  387,   59,  265,  342, 1761,
   40,    0, 1632, 1633,  396,  300, 1293,    0,   59,  374,
   41,  306,  280,   44,  309,  450, 1646,  285,  342,  434,
  387,  296,  387,  438,  387, 1787,  440, 1314, 1790,  396,
  424, 1793, 1794,  396,  302, 1716, 1323,    0,  452,   59,
  454,   42,   43, 1330,   45,    0,   47,   59,  461,  236,
 1812, 1682, 1682,  377,  280,   41, 1818,  244,   44,  285,
   59,  405,   93,    0,  408,  321,   59, 1829, 1699, 1699,
 1832, 1184, 1359, 1186,  280,  337, 1363,  440,  264,  335,
  424,  405,  288,  316,  408, 1761, 1716, 1717,    0,  452,
   41,  454,  337,   44, 1718, 1382,   59,  318, 1718,    0,
  424, 1732, 1732,  359,   59, 1761,  450,  321, 1739, 1739,
  460, 1785,  374,    0,  340,    0,  411, 1793, 1794,    0,
  336,  335,   59,  418,    0,  387,  450,  383,   40,   41,
   42,   43,   44,   45,   46,   47,   41, 1793, 1794,   44,
  452,  436, 1818, 1774, 1774,  359,  340,   59,   60,   61,
   62, 1782, 1782, 1829,  402, 1785, 1832,  413,   59,  391,
  392,    0, 1818,  279,    0,  461,  462,   40,  258,  383,
    0,  466,   59, 1829,   59,  470, 1832,  320,   59,   91,
 1810,   93,  318,   59,  300,  460,  383, 1817,  278,    0,
  280, 1478, 1479,  402,  284,   42,   43,  287,   45,  413,
   47,   42,   43,   44,   45, 1835,   47,  383,   41,  436,
  300,   44, 1499,  329,  489,  490,  306,  492,  506,  458,
   59,  460,    0,   59,  499,  500,  501,  502,  503,   59,
  505,    0,  348,  508,  509,  510,  511,  512,  361,   41,
   41,  357,   44,   44,  434,  321,   41,  363,   59,   44,
  489,  490, 1539,  492,  450,  318,  384,  460,  417,  335,
  499,  500,  501,  502,  503,  381,  505,  343,  331,  508,
  509,  510,  511,  512,  442,   42,   43,  340,   45,  280,
   47,   59,   41,  359,  285,   44,  489,  490,  378,  492,
   59,   93,  454,  357,  260,    0,  499,  500,  501,  502,
  503,   41,  505,  287,   44,  508,  509,  510,  511,  512,
  400,  411,   41,   41, 1601,   44,   44,  407,   41,  320,
   41,   44, 1609,   44,  387,   41,   42,   43,  418,   45,
  469,   47,   44,  396,   61,  301,   41,  413,  372,   44,
   41,    0,   41,   44,  286,   44,  436,  313,   40,  264,
  262,  441,  264,  265,   59,   40,   61,   40, 1645,   40,
  272,   40, 1649,  461,   42,   43,   40,   45,  280,   47,
 1661,   40,  338,  285,  356,   40,  288,  440,    0,   40,
  470,  461,   41,   42,   43,   44,   45,   46,   47,   41,
  302,  454,   44,   44,   41,  307,  308,   44,   40,  311,
   59,   60,   61,   62,   40,  461,  318,   40,  320,  321,
   40,  393,  324,  395,   40,   40,  328, 1708, 1705,  331,
 1707,  333,   46,  335,   91,  436,  320,   44,  340,  341,
  402,  343,   91,  280,   93,   44,  418,   59,  285,  280,
  460,  407,  320,   44,  285,  428,    0,  359,  360,   46,
  362,  417,   41,   42,   43,   61,   45, 1748,   47,   40,
  264,  443,   44,  382,  376, 1756,  378,  379,  380,  489,
  490,  383,  492,  505,  386,  387,  442,  348,   44,  499,
  500,  501,  502,  503,  396,  505,  278,  278,  508,  509,
  510,  511,  512,  340,  300,  436, 1787,  397,  410, 1790,
  264,  413,  470,  277,  397,   59,   41,   42,   43,  458,
   45,  447,   47,  280,  447,  399,   40,  429,  285,   40,
  349, 1812,   91,  435,   40,   40,    0,  264,  440,  318,
   41,   42,   43,  383,   45,   44,   47,  411,  389,  451,
  452,   40,  454,   40,  311,   41,   42,   43,  460,   45,
  382,   47,  665,  258,  667,  457,  458,  459,  671,  672,
  673,  451,   41,  317,  280,  464,  436,   41,   40,  285,
   44,  320,  277,  278,  320,  280,  448,  489,  490,  284,
  492,  502,  287,  485,  486,   59,  452,  499,  500,  501,
  502,  503,  264,  505,    0,  300,  508,  509,  510,  511,
  512,  306,  280,  262,  458,  264,  265,  285,   40,  277,
  884,  274,  886,  272,  888,  889,   40,  891,  892,  893,
    0,  280,  284,  300,  344,  344,  285,   42,   43,  288,
   45,  386,   47,  311,  278,   41,   42,   43,   44,   45,
   46,   47,  386,  302,   59,  304,  418,  386,  307,  308,
  416,  300,  311,   59,   60,   61,   62,  279,  418,  318,
  356,  320,  321,   40,   44,  324,  417,   40,   40,  328,
  274,  264,  331,  378,  333,  417,  335,   41,  300,   59,
  320,  340,  341,   44,  343,   91,   44,   93,  280,   44,
  385,  280,  367,   44,   40,  400,  285,  320,  266,    0,
  359,  360,  407,  362,  258,   44,   44,  329,   41,   42,
   43,  416,   45,  418,   47,  328,   40,  376,  501,  378,
  379,  380,  299,   40,  383,  279,  348,  386,  387,  422,
  264,  436,   41,   58,  356,  357,  441,  396,   41,   41,
   41,  363,   41,   44,  264,  280,  300,  454,  394,  440,
  285,  410,  306,  309,  413,   41,  492,  309,   59,  381,
   41,  352,   41,   42,   43,  470,   45,   40,   47,  280,
  429,  393,   40,  395,  285,  329,  435,   40,   40,    0,
   40,  440,   40,   40,  280,   40,   40,   40,   40,  285,
   40,   40,  451,  452,  348,  454,  418,   40,   40,   40,
  262,  460,  356,  357,  278,  308,  280,  435,  327,  363,
  284,   44,   41,  287,  388,  388,  388,  343,  274,  331,
   41,  443,  422,   44,  391,  390,  300,  381,  446,  271,
  489,  490,  287,  492,   41,  386,  407,  386,   59,  386,
  499,  500,  501,  502,  503,  386,  505,    0,  386,  508,
  509,  510,  511,  512,  386,  378,  262,   41,  264,  265,
  278,   40,  417,   41,  418,  280,  272,   41,   41,   41,
  285,  308,    0,  435,  280,  429,  436,  385,  396,  285,
  280,  442,  288,  337,  442,  338,  338,  301,   41,   42,
   43,   44,   45,   46,   47,   41,  302,  301,   41,  320,
  385,  307,  308,  352,  378,  311,   59,   60,   61,   62,
    0,  371,  318,   41,  320,  321,   44,   41,  324,   41,
  371,   41,  328,   41,   41,  331,  400,  333,   41,  335,
   41,   59,   41,  407,  340,  341,   41,  343,   91,   41,
   93,  321,   41,   41,   41,   41,   41,  280,  328,   41,
  280,  331,  285,  359,  360,  335,  362,  258,   41,   42,
   43,   46,   45,  343,   47,   44,   59,  441,   40,   59,
  376,  340,  378,  379,  380,   59,    0,  383,  279,  359,
  386,  387,   40,   59,   59,   41,   42,   43,   59,   45,
  396,   47,   41,   42,   43,   59,   45,  340,   47,  300,
   59,  280,  308,  383,  410,  306,  285,  413,   41,   42,
   43,   59,   45,   41,   47,  352,  344,   41,   41,  368,
   44,   40,  340,  429,  451,   59,  383,   44,  329,  435,
   59,  383,    0,  413,  440,   59,  383,   41,  418,  343,
   41,   41,   44,  410,  435,  451,  452,  348,  454,   41,
   42,   43,  442,   45,  460,   47,  357,  278,  304,  280,
  440,  301,  363,  284,  387,   40,  287,  293,   59,  304,
  463,   44,  452,   41,  454,  307,   44,   44,   41,  300,
  381,  337,    0,  489,  490,  383,  492,   42,   43,  383,
   45,   59,   47,  499,  500,  501,  502,  503,   40,  505,
    0,  264,  508,  509,  510,  511,  512,  278,  264,  262,
  458,  264,  265,  389,  320,  264,  470,  418,  411,  272,
  451,   44,   41,   42,   43,    0,   45,  280,   47,   40,
  271,  458,  285,  309,  264,  288,  436,  458,  435,  343,
  343,   41,   42,   43,   44,   45,   46,   47,  343,  302,
  278,  343,  280,  308,  307,  308,  271,  378,  311,   59,
   60,   61,   62,   41,   40,  318,   41,  320,  321,   44,
  308,  324,   40,   59,   59,  328,  308,   41,  331,  400,
  333,  304,  335,   41,   59,   59,  407,  340,  341,  279,
  343,   91,   59,   93,   41,    0,    0,  280,    0, 1187,
   12,  271,  285,  264,  710, 1087,  359,  360,  263,  362,
  300,   41,   42,   43,  634,   45, 1085,   47,  856, 1082,
  441,   87, 1074,  376,  280,  378,  379,  380, 1440,  285,
  383,  280,  326,  386,  387,  356,  285, 1441, 1064,  329,
  581,  321, 1012,  396,  311, 1643,  789,  280, 1075,  754,
  378,  338,  285, 1419,  278, 1723,  280,  410,  348,    0,
  413,   42,   43,  287,   45,  338,   47,  357, 1421,  560,
 1727, 1726,  400,  363, 1765,  779,  429, 1242, 1705,  407,
 1651, 1817,  435, 1666, 1762,    0,  999,  440,  280, 1220,
 1398,  381, 1682,  285,  331,  562,  343,  324,  451,  452,
   41,  454,  835,   44,  620, 1276,   13,  460,  298,  494,
  278,  498,  280,  441,  827,  743,  757,  141,   59,  287,
  729,  976,  730, 1634, 1522,  280,   41, 1739, 1782,   44,
  285, 1835,  300,  718,  509, 1231,  489,  490, 1124,  492,
   -1,  968, 1167,   -1,   59,   -1,  499,  500,  501,  502,
  503,   -1,  505,    0,  378,  508,  509,  510,  511,  512,
   -1,  280,  262,   -1,  264,  265,  285,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   -1,  400,   -1,   -1,   -1,
  280,   -1,   -1,  407,   -1,  285,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,  302,  278,   -1,  280,   -1,  307,  308,   -1,
  378,  311,   59,   60,   61,   62,   -1,  441,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,  400,  333,   -1,  335,   -1,   -1,   -1,  407,
  340,  341,   -1,  343,   -1,   -1,   93,   -1,   -1,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,    0,  359,
  360,   -1,  362,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,  441,   -1,   -1,  376,   -1,  378,  379,
  380,  262,   -1,  383,   -1,   -1,  386,  387,   -1,    0,
   -1,   41,   42,   43,   -1,   45,  396,   47,   -1,  280,
   -1,   -1,   -1,  378,  285,   -1,   -1,   -1,   -1,   -1,
  410,   -1,    0,  413,   -1,   -1,   -1,   59,   41,   42,
   43,   -1,   45,   -1,   47,  400,   -1,   -1,   -1,  429,
   41,   -1,  407,   44,   -1,  435,   -1,  278,   -1,  280,
  440,   -1,   -1,   -1,   -1,   -1,  287,   -1,   59,   -1,
   -1,  451,  452,   41,  454,   -1,   44,   -1,   -1,  300,
  460,   -1,   -1,  278,  294,  280,  441,   -1,   -1,   -1,
   -1,   59,  287,   61,   -1,  305,   -1,   -1,   -1,   -1,
   -1,   -1,  312,   -1,   -1,    0,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,  325,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   41,   42,   43,   44,
   45,   -1,   47,  280,   -1,   -1,   -1,  378,  285,   -1,
   -1,  288,   -1,   -1,   59,   60,   61,   62,   42,   43,
   -1,   45,   -1,   47,   -1,  302,  386,   -1,   -1,  400,
  307,  308,   -1,  378,  311,   -1,  407,   -1,   -1,   -1,
   -1,  318,  402,  320,  321,   -1,   -1,  324,   93,   -1,
   -1,  328,   -1,   -1,  331,  400,  333,   -1,  335,  419,
   -1,   -1,  407,  340,  341,   -1,  343,   -1,   -1,   -1,
  441,    0,  280,   -1,  434,   -1,   -1,  285,  438,   -1,
   -1,  441,  359,  360,   -1,  362,  258,   -1,   -1,  449,
   -1,   -1,   -1,   -1,   -1,   -1,  441,   -1,   -1,  376,
  280,  378,  379,  380,   -1,  285,  383,  279,  468,  386,
  387,  472,  473,  474,  475,  476,  477,  478,  479,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,  300,   -1,
   59,   -1,  285,  410,  306,   -1,  413,   -1,   -1,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,  329,  435,  277,
  278,   -1,  280,  440,   -1,   -1,  284,   -1,   -1,  287,
   -1,   -1,   -1,   -1,  451,  452,  348,  454,   -1,   -1,
  321,   -1,  300,  460,   -1,  357,   -1,  328,  306,   -1,
  331,  363,   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,    0,  381,
   -1,   -1,  489,  490,   -1,  492,   -1,  262,  359,  264,
  265,   -1,  499,  500,  501,  502,  503,  272,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,
  285,   -1,  383,  288,   -1,   -1,  418,   -1,   -1,   41,
   42,   43,   44,   45,   -1,   47,  280,  302,   -1,   -1,
  378,  285,  307,  308,   -1,   -1,  311,   59,   60,   61,
   62,   -1,  413,  318,   -1,  320,  321,    0,   -1,  324,
   -1,   -1,  400,  328,   -1,   -1,  331,   -1,  333,  407,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,  440,
  418,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,  359,  360,   -1,  362,  436,   -1,
   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  258,
   -1,  376,   -1,  378,  379,  380,   59,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  279,  396,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,  305,   -1,   -1,  410,   -1,   -1,  413,  312,
   -1,  300,   -1,  316,  317,   -1,   -1,  306,   -1,   -1,
   -1,   -1,  325,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,  337,  440,   -1,   -1,   -1,   -1,
  329,  301,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,   -1,   -1,  313,   -1,  460,   -1,   -1,   -1,  348,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  327,  357,   -1,
   -1,   -1,   -1,   -1,  363,   -1,   -1,   -1,  338,   -1,
    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  381,   -1,  499,  500,  501,  502,  503,  402,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,  419,   -1,   -1,   -1,
  272,   41,   42,   43,   44,   45,   -1,   47,   -1,  418,
   -1,  434,   -1,   -1,   -1,  438,  288,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,  449,  407,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,  417,   -1,  311,
    0,   -1,   -1,   -1,   -1,  468,  318,   -1,  320,  321,
   -1,   -1,  324,   93,   -1,  258,  328,   -1,   -1,  331,
   -1,  333,  442,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,  279,   -1,   -1,   -1,
  460,   41,   -1,   -1,   44,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   59,
   -1,   -1,   -1,  306,  376,   -1,  378,  379,  380,  489,
  490,  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  396,  505,  329,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  357,   -1,   -1,  429,   -1,   -1,
  363,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  381,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   60,   61,   62,    0,   -1,  418,   -1,  489,  490,   -1,
  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   45,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   -1,
   -1,  311,   59,   60,   61,   62,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   93, 1315, 1316,   -1,
   -1,   -1,   -1, 1321, 1322,   -1, 1324, 1325,   -1,  359,
  360,   -1,  362,   -1, 1332, 1333, 1334, 1335,   -1, 1337,
 1338, 1339, 1340, 1341, 1342,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,  264,
  410,  331,   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,  359,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,  272,  383,   -1,   -1,   -1,  387,   -1,   -1,
  280,   -1,   -1,   -1,   -1,  285,  396,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  489,
  490,   -1,  492,  413,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  324,  262,   -1,  264,  265,   -1,
  440,   -1,   -1,  333,   -1,  272,   41,   -1,   43,   44,
   45,  341,  452,   -1,  454,  295,   -1,   -1,   -1,   -1,
   -1,  288,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
  360,   -1,  362,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,  323,  311,    0,  376,   -1,  378,  379,
  380,  318,   -1,  320,  321,   -1,   -1,  324,   93,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,  352,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   41,   -1,   -1,   44,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,  489,  490,  383,  492,   -1,  386,
  387,   -1,   -1,   -1,  499,  500,  501,  502,  503,  396,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   -1,  413,  427,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,  443,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,  457,  458,  459,
   -1,  461,  462,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,   -1,
   -1,   -1,   -1,   -1,   -1,  495,  496,   -1,    0,   -1,
   -1,   -1,  489,  490,  504,  492,   -1,  262,   -1,  264,
  265,   -1,  499,  500,  501,  502,  503,  272,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   43,   44,   45,   -1,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   -1,  311,   59,   60,   61,
   62,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  260,   -1,   -1,  328,   -1,  410,  331,   -1,  413,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,
   -1,  301,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,   -1,   -1,  313,   -1,  460,   -1,   -1,  383,   -1,
   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,  338,   -1,
    0,   -1,   -1,   -1,  489,  490,   -1,  492,  413,   -1,
   -1,  287,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  262,   -1,  264,  265,   -1,  440,   -1,   -1,   -1,   -1,
  272,   41,   -1,   43,   44,   45,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,  407,   -1,   -1,
  302,   -1,   -1,   -1,    0,  307,  308,  417,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   93,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,  442,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,  489,
  490,  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  396,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  451,
  452,   -1,  454,  499,  500,  501,  502,  503,  460,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   40,   -1,  288,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  302,   -1,   58,   -1,   -1,  307,  308,   63,
   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,  396,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  410,   -1,   -1,  413,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   41,   -1,   -1,   44,  429,
   -1,  287,   -1,  359,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,  383,   -1,   -1,
  460,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  267,  440,  273,   -1,   -1,   -1,  277,
   -1,   -1,  276,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,  291,  460,  289,   -1,  295,   -1,   -1,
  298,  295,  296,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  314,   -1,   58,  489,  490,   -1,  492,   63,   -1,  323,
   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,  505,
   -1,  339,  508,  509,  510,  511,  512,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,  460,   -1,   -1,   -1,   40,   -1,
   42,   43,  366,   45,   -1,  373,  370,  375,   -1,   -1,
   -1,   -1,   -1,   -1,  378,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,  258,  499,  500,  501,  502,  503,  406,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  274,   -1,
   -1,  277,  278,  417,  280,  423,   -1,  421,  284,   -1,
   -1,  287,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,  300,   -1,   -1,  445,   -1,  443,
  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,  497,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,  513,
  514,  515,  378,    0,   -1,   -1,   -1,   -1,  328,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,
   -1,  407,   -1,  289,   -1,  411,  292,   -1,   -1,  295,
  296,   -1,  418,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,
  436,   -1,   59,   -1,   -1,  441,   -1,  323,  260,   -1,
  326,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
  466,   -1,   -1,   -1,  470,   -1,   -1,  289,  354,  355,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
  366,  303,   -1,   -1,  370,   40,   -1,   42,   43,   -1,
   45,   -1,  378,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   58,  326,   -1,   -1,   -1,   63,   -1,
  460,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,  421,   -1,   -1,  460,  489,
  490,  427,  492,   -1,  366,   -1,   -1,  433,  370,  499,
  500,  501,  502,  503,   -1,  505,   -1,  443,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   41,  489,  490,   44,
  492,  457,  458,  459,  460,  461,  462,  499,  500,  501,
  502,  503,   -1,  505,   59,  471,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,  427,  492,   -1,   -1,  495,
  496,  433,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  443,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,  260,   -1,   -1,  335,   -1,
   -1,   -1,  267,  340,   -1,   40,  343,   -1,   43,   -1,
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,   58,  289,   -1,   -1,   -1,   63,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   42,   43,   -1,   45,   -1,   47,   -1,  323,  396,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,
   61,   62,   -1,   -1,   -1,   -1,  413,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   40,   -1,   -1,   43,   -1,   45,   -1,  435,   -1,
   -1,  366,   -1,  440,   -1,  370,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,  413,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  435,   -1,   -1,   -1,  289,  440,   -1,   -1,   -1,   -1,
  295,  296,   -1,  460,   -1,   -1,    0,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,
   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,  323,  280,
   -1,  326,  489,  490,  285,  492,   -1,  288,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   41,  505,   -1,
   44,  508,  509,  510,  511,  512,   -1,   -1,  267,  354,
  355,   40,   -1,   -1,   43,   59,   45,  276,   -1,   -1,
   -1,  366,   -1,  324,   -1,  370,   -1,   -1,   -1,   58,
  289,   -1,  333,  378,   63,   -1,  295,  296,   -1,   -1,
  341,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,   -1,  360,
   -1,  362,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,  421,  378,  379,  380,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,  457,  458,  459,  460,  461,  462,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,  429,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  421,  508,  509,  510,  511,  512,  427,   -1,
   -1,   -1,   -1,   -1,  433,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   41,   -1,  262,   44,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   59,   -1,  495,  496,  267,  498,
  499,  500,  501,  502,  503,  504,  505,  276,   -1,  508,
  509,  510,  511,  512,  443,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   40,  295,  296,   43,   -1,
   45,  460,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,   -1,   58,  328,  314,   -1,  331,   63,   -1,
   -1,  335,   -1,   -1,  323,   -1,  340,  326,   -1,  343,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,  359,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,  383,
   -1,  370,  386,  387,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,  435,  421,   -1,   -1,   -1,  440,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,  276,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,  289,  340,   -1,   -1,  343,   -1,
  295,  296,   -1,  458,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,  489,  490,   -1,  492,  383,   -1,
   -1,  386,  387,  498,  499,  500,  501,  502,  503,   -1,
  505,  396,   -1,  508,  509,  510,  511,  512,   -1,  354,
  355,   -1,  260,   -1,   -1,  263,   -1,   -1,  413,  267,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,  289,   -1,   -1,  460,  440,   40,  295,  296,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,  489,  490,  323,  492,   -1,  326,   -1,
  425,   -1,  427,  499,  500,  501,  502,  503,  433,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  261,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  282,   -1,   -1,   -1,  425,   -1,  427,
   -1,  290,   -1,   -1,   -1,  433,   -1,   -1,  297,   -1,
  299,   -1,  301,   -1,   -1,  443,   -1,  306,   40,   -1,
   -1,   43,   -1,   45,  313,   -1,  328,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   58,   -1,  327,   -1,
   -1,   63,   -1,  471,   -1,   -1,   -1,   -1,   -1,  338,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,  343,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  260,   -1,   -1,  263,
  368,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  402,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,  405,   -1,   -1,   -1,
   -1,  295,  296,  412,   -1,   -1,  415,   40,  417,  418,
   43,   -1,   45,   -1,   -1,   -1,  438,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   58,   -1,  449,  426,  323,
   63,   -1,  326,  442,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  468,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,  460,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,  366,   -1,   -1,   -1,  370,  499,  500,  501,
  502,  503,  491,  505,   -1,   -1,  508,  509,  510,  511,
  512,  489,  490,   -1,  492,   -1,   -1,  506,  507,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,  425,   -1,  427,  276,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  354,  355,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,  260,  370,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,  460,   -1,   -1,
   -1,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,  425,   63,  427,  489,  490,   -1,  492,
  323,  433,   -1,  326,   -1,   -1,  499,  500,  501,  502,
  503,  443,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,  323,   63,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,  276,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  323,    0,   -1,  326,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  354,  355,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   41,  366,
   -1,   44,  260,  370,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   40,   -1,   42,   43,   59,   45,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,  323,  433,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,
  498,  499,  500,  501,  502,  503,  504,  505,  276,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   40,
   -1,  289,   43,   -1,   45,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,   58,  321,   -1,
   -1,   -1,   63,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,  323,   -1,  340,  326,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
  383,   -1,  370,  386,  387,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,  460,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
  469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,  323,  492,   -1,  326,  495,  496,   -1,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,  267,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   40,   -1,  289,   43,   -1,   45,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
  271,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
  301,   -1,   -1,   -1,   -1,   -1,   -1,  308,  443,   -1,
   -1,  312,  313,   -1,  315,   -1,   -1,  318,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,  332,   -1,   -1,   -1,   -1,  471,  338,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  260,  508,  509,  510,  511,  512,   -1,  267,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,  303,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  432,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,  442,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
  451,   45,  453,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   58,   -1,  354,  355,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,
  498,  499,  500,  501,  502,  503,  504,  505,  276,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,  352,  289,  354,  355,   -1,   -1,
   -1,  295,  296,   -1,   58,   -1,   -1,   -1,  366,   63,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   58,  427,
   -1,   -1,  366,   63,   -1,  433,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,  427,  492,   -1,   -1,  495,  496,  433,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  443,
  508,  509,  510,  511,  512,   -1,   -1,  451,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  267,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   40,   41,   -1,   43,   -1,   45,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,
  354,  355,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,  417,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,  366,   -1,   40,  433,
  370,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,  427,  492,   -1,
   -1,  495,  496,  433,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  443,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,  273,
   -1,  471,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,  295,  492,   -1,  298,  495,  496,  267,  498,  499,
  500,  501,  502,  503,  504,  505,  276,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,
   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,  366,   -1,   40,   -1,
  370,   43,  406,   45,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,  423,
   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   40,  433,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  480,  481,   -1,   -1,
   58,   -1,  354,  355,   -1,   63,   -1,  457,  458,  459,
  460,  461,  462,  497,  366,   -1,   -1,   -1,  370,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,  289,  354,  355,   -1,   -1,   -1,  295,  296,   -1,
   58,   59,   -1,   -1,  366,   63,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,  417,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   58,  427,   -1,   -1,  366,   63,
   -1,  433,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  427,
  492,   -1,   -1,  495,  496,  433,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  443,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  267,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,  289,  354,  355,   -1,   63,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,  366,   -1,   -1,  433,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,  427,  492,   -1,   -1,  495,  496,  433,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  443,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,    0,  492,   -1,
   -1,  495,  496,  267,  498,  499,  500,  501,  502,  503,
  504,  505,  276,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,    0,  289,   41,   -1,   -1,   44,
   -1,  295,  296,   -1,   -1,   -1,   -1,   40,   41,   -1,
   -1,   44,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,  323,
   -1,   -1,  326,   -1,   -1,   41,   -1,   -1,   44,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,
   -1,   -1,   -1,   59,    0,   61,   -1,   -1,   91,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   91,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  258,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,  274,
   -1,   -1,  277,  278,   -1,  280,   -1,   -1,   -1,  284,
   -1,  274,  287,   -1,  277,  278,    0,  280,   -1,   -1,
   -1,  284,  258,   -1,  287,  300,   -1,   -1,   -1,   -1,
   -1,  306,   -1,   -1,  309,   -1,   -1,  300,  274,   -1,
   -1,   -1,  278,  306,  280,   -1,  309,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,  300,   -1,   -1,   -1,  258,   -1,
  306,   -1,   -1,  309,   -1,   59,   60,   61,   62,  271,
   -1,   -1,  258,   -1,  274,   -1,   -1,   -1,  278,   -1,
  280,   -1,   -1,   -1,  284,   -1,   -1,  287,  274,   -1,
   -1,   -1,  278,  378,  280,   -1,   -1,   91,  284,  301,
  300,  287,   -1,   -1,   -1,  378,  306,   -1,   -1,  309,
  312,  313,   -1,  315,  300,  400,  318,   -1,   -1,   -1,
  306,   -1,  407,  309,   -1,   -1,  411,  400,   -1,   -1,
  332,   -1,  378,  418,  407,   -1,  338,   -1,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,
   -1,  436,   -1,   -1,  400,   -1,  441,   -1,   -1,   -1,
   -1,  407,   -1,  436,   -1,  411,   -1,   -1,  441,    0,
   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,  378,   -1,
   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,   -1,
  436,   -1,  378,  466,   -1,  441,  398,  470,   -1,   -1,
  400,    0,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,
   41,  411,   -1,   44,  400,  417,   -1,   -1,  418,   -1,
  466,  407,   -1,   -1,  470,  411,   -1,   -1,   59,   -1,
  432,   -1,  418,   -1,   -1,   -1,  436,   -1,   -1,   -1,
  442,  441,   41,   42,   43,   44,   45,   46,   47,  451,
  436,  453,   -1,   -1,   -1,  441,   -1,   -1,  460,   -1,
   59,   60,   61,   62,   -1,   -1,  466,   -1,  262,   -1,
  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
  466,   -1,   -1,   -1,  470,   -1,  280,  489,  490,   -1,
  492,  285,   91,   -1,  288,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   41,   42,   43,   44,   45,
   46,   47,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,    0,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   91,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   41,
  454,   -1,   44,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   -1,   59,   -1,   -1,
   -1,  280,   -1,   -1,   -1,   -1,  285,  318,   -1,  288,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,    0,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,  324,   -1,   -1,  359,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   41,   -1,
   -1,   44,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,  396,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,  413,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,    0,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,  413,   -1,  262,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,  272,   -1,   -1,   -1,
  429,   -1,   -1,   -1,  280,   -1,  435,   -1,   -1,  285,
   -1,  440,  288,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   59,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  324,   -1,
  262,   -1,  328,    0,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   41,   -1,   -1,   44,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,  318,  383,   -1,  321,
  386,  387,   59,    0,   -1,   -1,  328,   -1,   -1,  331,
  396,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,  413,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,  429,   41,   -1,   -1,   44,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   59,   -1,  386,  387,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  396,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,  413,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,    0,   -1,  435,   -1,   -1,   41,   -1,  440,   44,
   -1,   -1,   -1,   -1,   -1,  262,  359,   -1,   -1,   -1,
  452,   -1,  454,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   41,  386,  387,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   59,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  413,  318,   -1,   -1,  321,  262,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  435,  340,   -1,  262,  343,  440,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  452,
   -1,  454,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,  383,   -1,   -1,  386,
  387,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  396,
   -1,  318,   -1,  340,  321,  262,  343,   -1,   -1,   -1,
   -1,  328,   41,   -1,  331,   44,  413,   -1,  335,   -1,
   -1,    0,  359,  340,   -1,   -1,  343,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  359,  440,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  396,
   -1,  318,   41,   -1,  321,   44,  383,   -1,   -1,  386,
  387,  328,   -1,   -1,  331,   -1,  413,  262,  335,  396,
   59,    0,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,  435,   -1,
   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  452,   -1,  454,  435,   -1,
   -1,   -1,   41,  440,   -1,   44,  383,   -1,   -1,  386,
  387,   -1,   -1,  318,   -1,  452,  321,  454,   -1,  396,
   59,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,   -1,  340,  413,   -1,  343,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,  262,  359,   -1,   -1,   -1,  435,  328,
   -1,   -1,  331,  440,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,  452,   -1,  454,  383,   -1,
   -1,  386,  387,   -1,   -1,   41,   -1,   -1,   44,   -1,
  359,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  413,  318,
   -1,   -1,  321,  262,  383,   -1,    0,  386,  387,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,  396,   -1,   -1,
  435,  340,   -1,   -1,  343,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,  452,   -1,  454,
  359,   -1,    0,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,  262,   -1,   -1,  435,   -1,   -1,  318,
   -1,  440,  321,   -1,  383,   59,   -1,  386,  387,  328,
   -1,   -1,  331,  452,   -1,  454,  335,  396,   -1,   -1,
   -1,  340,   -1,   41,  343,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  359,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,  262,   -1,   -1,  435,   -1,   -1,  328,
   -1,  440,  331,   -1,  383,   -1,  335,  386,  387,   -1,
   -1,  340,   -1,  452,  343,  454,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,  383,   -1,  435,  386,  387,  328,
   -1,  440,  331,   -1,   -1,   -1,  335,  396,   -1,   -1,
   -1,  340,   -1,  452,  343,  454,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,  396,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,  413,  331,   -1,   -1,  262,  335,
   -1,   -1,  271,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  301,  452,  262,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  312,  313,   -1,  315,  383,   -1,  318,
  386,  387,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,
  396,   -1,   -1,  332,  328,   -1,   -1,  331,   -1,  338,
   -1,  335,   -1,   -1,   -1,   -1,  340,  413,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,  359,   -1,   -1,   -1,  435,
  328,   -1,   -1,  331,  440,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,  452,   -1,  454,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,  398,
   -1,  359,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,  413,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  396,   -1,
   -1,  435,   -1,  442,   -1,   -1,  440,   -1,  273,   -1,
   -1,   -1,  277,   -1,  453,  413,   -1,   -1,  452,  284,
  454,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  295,   -1,   -1,  298,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,  452,   -1,  454,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,  339,   -1,   -1,   -1,   -1,  273,
  345,  346,  347,  277,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,  345,  346,  347,   -1,  349,  350,  351,  423,  353,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,
  445,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  406,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  423,
   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  445,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,  291,  273,   -1,   -1,  295,  277,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,
  291,   -1,   -1,   -1,  295,  489,  490,  298,  492,   -1,
   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,  273,   -1,   -1,  339,  277,
   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  373,   -1,  375,   -1,  295,   -1,   -1,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  339,   -1,  423,   -1,  406,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  497,   -1,  480,
  481,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  497,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,  445,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  480,  481,  499,  500,  501,  502,  503,  504,
  505,  506,   -1,  508,  509,  510,  511,  512,   -1,  497,
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
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
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
