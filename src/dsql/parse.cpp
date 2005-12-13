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

/* Make bison allocate static stack */
#define YYINITDEPTH 2048
/* #define YYSTACK_USE_ALLOCA 1 */

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
#define KW_PAGE_SIZE 393
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
  190,  191,  192,  193,  198,  199,  194,  194,  195,  205,
  196,  206,  201,  201,  201,  202,  202,  202,  203,  203,
  203,  203,  197,  200,  200,  204,  204,  187,  187,  211,
  212,  212,  213,  213,  213,  213,   11,   71,   80,  215,
  218,  218,  218,  219,  222,  226,  226,  227,  227,  228,
  228,  147,  217,   87,   89,  216,  216,   70,   83,  231,
  231,  231,  232,  235,  235,  236,  236,  236,  236,  236,
  236,  236,  236,  236,  236,  236,  236,  236,  236,  236,
  233,  233,  234,    2,  238,  238,  238,  238,  238,  238,
  238,  246,  247,  244,  244,  248,  248,  248,  248,  248,
  248,  248,  239,  239,  249,  249,  249,  249,  249,  249,
  249,  243,  254,  254,  254,  254,  254,  254,  254,  254,
  254,  254,  254,  254,  254,  254,  254,  254,  254,  254,
  254,  254,  254,  254,  254,  251,  251,  253,  253,  252,
  250,  250,  250,  245,  245,  241,  242,  242,  256,  256,
  256,  256,  257,  257,  240,  258,  258,  259,  259,    8,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  101,  101,  144,  144,  263,  263,  266,  266,
  268,  268,   53,   53,  265,  265,  265,  265,  265,  265,
  265,  265,  265,  273,  273,  264,  264,  264,  264,  275,
  275,  274,  274,  274,   55,   55,  270,  270,  270,  267,
  267,  267,  279,  279,  279,  278,  278,  277,  277,  277,
  271,  271,  282,  280,  280,  280,  281,  281,  272,  272,
  272,  272,  283,  283,   18,   18,   18,  285,  285,  285,
   16,   16,   16,  287,  288,  291,  291,  289,  293,  293,
    4,   15,  292,  292,  294,  294,  295,  295,  284,  296,
  296,  297,  297,  298,  298,  298,  298,  299,  299,  300,
  300,  301,  301,  303,  303,  303,  304,  304,  304,  305,
  305,  305,  302,  307,  307,  307,  308,  308,  306,  306,
  309,  311,  311,  310,  310,  286,   17,  312,  312,  312,
  313,  313,  317,  317,  318,   65,   65,   65,  320,  320,
  319,  319,  314,  314,  323,  323,  315,  315,  316,  327,
  328,  329,  321,  322,  324,  324,  324,  324,  330,  330,
  330,  331,  331,  331,  220,  220,  221,  221,  334,  334,
  335,  335,  335,  325,  338,  338,  339,  339,  340,  340,
  341,  341,  342,  342,  208,  208,  344,  344,  229,  229,
   31,  230,  230,  230,  230,  230,  230,  230,  230,  224,
  224,  345,  345,  346,  346,  346,  346,  346,  348,  348,
  348,  225,  225,  223,  223,  326,  326,  352,  353,  353,
  353,  353,  353,  354,  354,  355,  355,  356,  356,  357,
  357,  357,  358,  358,   10,   10,  360,  360,    7,    7,
  361,  362,  363,   19,   19,  364,  365,  366,  366,  188,
  336,  336,    3,    3,  368,  368,  368,  370,  370,   61,
  369,  369,  371,  371,   34,   34,   85,   84,   84,  359,
  359,  372,  373,  373,  209,  209,  209,  146,  367,  367,
  111,  111,  111,  111,  111,  377,  377,  377,  377,  377,
  377,  376,  376,  376,  379,  379,  379,  379,  379,  379,
  379,  379,  379,  379,  380,  380,  380,  380,  380,  380,
  380,  380,  385,  385,  385,  385,  385,  385,  385,  385,
  385,  385,  385,  385,  385,  385,  385,  385,  391,  391,
  381,  381,  382,  382,  382,  382,  383,  383,  387,  387,
  388,  388,  388,  388,  386,  389,  384,  384,  375,  375,
  375,  378,  378,  378,  392,  392,  393,  390,  395,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,  156,  156,  156,  396,  394,
  394,  151,  151,  400,  400,  400,  400,  400,  398,  398,
  398,  398,  398,  332,  152,  152,  153,  154,  154,  154,
  154,  154,   51,   51,   20,   20,  237,  401,   54,  123,
  269,  269,   95,  397,  397,  397,  397,  402,  402,  402,
  402,  402,  402,  402,  402,  402,  402,  402,  403,  349,
  350,  350,  350,  347,  347,  399,  351,  351,  405,  405,
  406,  406,  407,  407,  410,  410,  408,  408,  413,  413,
  412,  409,  411,  404,  404,  404,  404,  404,  404,  404,
  404,  333,  333,  155,   48,  276,  122,  148,  374,  165,
  210,  262,   63,   60,  214,  112,   66,  337,   35,   41,
  343,  261,   39,   42,  207,   40,  290,  255,  255,  414,
  414,  414,  414,  414,  414,  414,  414,  414,  414,  414,
  414,  414,  414,
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
    2,    6,    4,    7,    8,    6,    8,    6,    4,    0,
    4,    2,    1,    3,    0,    2,    4,    0,    1,    1,
    3,    3,    6,    2,    2,    3,    0,    1,    2,    4,
    1,    3,    2,    2,    2,    1,    4,    7,    7,    1,
    1,    3,    4,    7,    2,    1,    3,    1,    1,    6,
    3,    0,    0,    0,    0,    3,    0,    9,    9,    1,
    1,    0,    2,    1,    1,    1,    1,    1,    3,    3,
    3,    3,    3,    3,    5,    5,    5,    5,    5,    5,
    2,    0,    4,    2,    3,    3,    2,    2,    3,    3,
    2,    3,    6,    1,    2,    4,    3,    2,    2,    2,
    2,    3,    1,    3,    3,    3,    2,    2,    4,    4,
    5,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    2,    1,
    1,    1,    0,    2,    2,    0,    1,    2,    2,    2,
    2,    2,    1,    3,    7,    1,    0,    1,    0,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    3,    2,
    2,    2,    1,    1,    1,    1,    4,    5,    1,    3,
    1,    3,    1,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    4,    4,    6,    5,    3,
    0,    2,    2,    0,    3,    0,    4,    1,    5,    4,
    1,    4,    1,    2,    2,    1,    1,    1,    2,    2,
    2,    2,    1,    0,    3,    5,    1,    1,    2,    3,
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
    1,    2,    3,    2,    1,    3,    1,    1,    6,    3,
    3,    2,    3,    0,    1,    3,    1,    1,    1,    2,
    1,    1,    1,    2,    1,    2,    1,    2,    0,    3,
    0,    1,    3,    1,    1,    1,    1,    3,    1,    1,
    1,    2,    0,    2,    0,    2,    0,    4,    1,    2,
    1,    1,    0,    1,    3,    2,    1,    1,    2,    1,
    4,    2,    1,    3,    8,    5,    1,    3,    1,    1,
    4,    4,    4,    1,    1,    5,    5,    1,    3,    3,
    1,    1,    7,    7,    5,    3,    0,    1,    1,    1,
    2,    0,    1,    1,    1,    0,    3,    1,    3,    1,
    0,    3,    1,    3,    1,    3,    3,    1,    1,    3,
    1,    2,    1,    3,    3,    1,    2,    3,    3,    3,
    3,    1,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    1,    1,
    5,    6,    3,    4,    5,    6,    3,    4,    3,    4,
    3,    4,    4,    5,    4,    4,    3,    4,    1,    1,
    1,    1,    1,    1,    1,    3,    3,   10,   10,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    3,    3,    3,    3,    3,    3,    3,    3,    1,    1,
    1,    1,    3,    1,    1,    1,    1,    1,    4,    1,
    3,    1,    2,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    2,    1,    1,    1,    1,
    1,    2,    1,    1,    1,    1,    1,    4,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    6,    6,
    6,    8,    4,    4,    3,    6,    1,    1,    6,    6,
    1,    1,    4,    6,    4,    5,    3,    5,    4,    5,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  545,
  546,  547,  551,  552,  553,    0,  608,  719,  720,  724,
  725,  456,    0,    0,    0,    0,    0,    0,  394,  563,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  897,    0,   21,  895,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,   37,   34,
   36,    0,   35,    0,  978,  980,  982,  981,  987,  983,
  990,  988,  984,  986,  989,  985,  991,  992,  993,    0,
    0,    0,    0,   32,   68,   66,  970,  979,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  637,    0,    0,    0,  669,    0,  972,    0,    0,
    0,    0,  107,  977,  554,    0,    2,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
  443,  444,  445,    0,  422,  423,    0,  963,    0,  401,
  967,    0,  398,  969,    0,  671,    0,  397,  973,    0,
  561,  893,    0,  150,  103,    0,  104,    0,  217,  959,
  758,    0,  102,  145,  966,    0,    0,   98,    0,  105,
  899,    0,   99,    0,  100,    0,  101,  976,  146,  106,
  616,  617,    0,  898,  896,    0,   74,    0,  964,    0,
  962,  478,  471,    0,  477,  482,  472,  473,  480,  474,
  475,  476,  481,    0,   30,    0,   39,  745,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  903,    0,  884,  631,  631,  632,
  642,  644,  952,  645,    0,  646,  636,  632,    0,    0,
    0,    0,    0,    0,    0,  580,  569,    0,  572,  574,
  575,  576,  577,  583,  584,    0,  971,  670,    0,  108,
    0,  109,    0,  110,    0,    0,    0,  609,    0,    0,
    0,    0,    0,    0,  457,    0,    0,  364,    0,  220,
    0,  404,  395,  454,  455,    0,    0,    0,    0,    0,
    0,  413,    0,  370,  371,    0,  567,  565,  894,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  955,   75,    0,    0,    0,  721,  722,  479,
    0,    0,    0,    0,  862,  890,    0,  954,    0,  864,
    0,    0,  891,    0,  885,  876,  875,  874,    0,    0,
    0,    0,  866,  867,  868,  877,  878,  886,  887,    0,
  888,  889,    0,    0,    0,  892,    0,    0,    0,    0,
    0,  880,    0,  755,  859,  860,  861,  667,  865,  347,
  845,    0,    0,    0,  844,    0,  665,  848,  906,  907,
  847,  841,  842,  843,  846,  879,  904,  905,  927,  928,
  931,  932,    0,  748,    0,    0,   27,    0,    0,    0,
   71,   69,  974,   67,   33,    0,    0,    0,  750,    0,
   52,    0,    0,    0,    0,    0,  559,    0,    0,    0,
  639,  641,    0,  648,    0,  732,    0,    0,  649,    0,
  635,    0,  606,    0,  581,    0,  578,    0,  579,  604,
    0,  599,    0,    0,  573,  759,  728,    0,    0,    0,
    0,    0,    0,  556,  555,    0,    0,  613,  610,    0,
    0,  607,  185,    0,    0,  129,  462,  172,  173,    0,
  460,    0,    0,    0,    0,  463,    0,  458,    0,    0,
  408,  410,  409,    0,  411,    0,  405,  218,    0,    0,
  271,    0,    0,    0,    0,  417,  418,    0,  447,    0,
    0,    0,    0,    0,    0,    0,  375,  374,  466,    0,
    0,  149,    0,    0,  158,    0,  157,    0,  164,  147,
    0,  162,  165,  166,  154,    0,    0,  501,  537,  504,
    0,  538,    0,    0,  505,    0,    0,    0,  528,  541,
  500,  523,  502,  503,  498,  485,  364,  483,  484,  486,
    0,    0,  495,  496,  497,  499,    0,    0,    0,    0,
    0,  112,    0,  113,    0,  124,    0,    0,  201,    0,
    0,  362,    0,  220,    0,    0,   83,    0,  740,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  761,  763,  772,  775,  776,  777,  778,  779,  780,  781,
  782,  783,  784,    0,    0,    0,  881,    0,    0,    0,
    0,    0,    0,  882,  883,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  322,  975,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  747,   28,    0,    0,   44,   72,    0,    0,   26,
    0,  753,    0,    0,    0,  716,    0,    0,    0,    0,
    0,  558,    0,  631,    0,    0,    0,    0,    0,  968,
  652,  549,    0,  901,  548,    0,  582,    0,  590,  586,
  585,    0,    0,    0,  601,  589,    0,    0,  726,  727,
    0,    0,    0,  362,    0,    0,    0,  623,  627,    0,
    0,  130,    0,    0,    0,    0,    0,    0,  364,  407,
    0,  364,  365,  412,  493,    0,  273,    0,  267,  269,
  362,  240,  960,    0,    0,  364,    0,    0,  209,  362,
    0,    0,    0,    0,  245,  239,  242,  243,  244,  416,
  452,  451,  415,  414,    0,    0,    0,    0,    0,    0,
  373,    0,  364,    0,  155,  159,    0,  163,    0,    0,
    0,  524,  525,  542,    0,  539,    0,    0,  531,  529,
  530,    0,    0,    0,    0,  494,    0,    0,    0,    0,
  532,    0,    0,  362,    0,  200,    0,  203,  205,  206,
    0,    0,    0,    0,    0,  220,   78,    0,    0,   77,
    0,    0,    0,    0,  762,  774,    0,    0,  832,  833,
  834,    0,  766,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  944,  945,  946,  947,
  948,  949,  950,  951,    0,    0,    0,    0,    0,    0,
  937,    0,    0,    0,    0,  857,  324,  858,  925,    0,
    0,  853,  958,    0,    0,    0,    0,    0,  666,    0,
  863,  757,  756,  749,    0,    0,    0,    0,    0,    0,
    0,    0,   53,   61,   63,    0,    0,   70,    0,    0,
    0,  752,    0,    0,    0,    0,    0,    0,    0,    0,
  640,  643,    0,    0,    0,    0,  657,  658,    0,    0,
  650,  653,  550,  902,  591,  600,  595,  594,    0,  605,
  760,  729,  730,  362,    0,    0,  633,  615,  614,    0,
  181,  175,  182,    0,  178,    0,    0,  177,    0,  188,
  169,  168,  900,  171,  170,  464,    0,    0,    0,  406,
  272,    0,  211,  364,  213,    0,  365,  208,  214,    0,
    0,    0,    0,  419,  420,  364,  362,  365,    0,    0,
    0,  391,    0,  196,  197,  156,    0,  512,  956,  513,
    0,    0,    0,    0,    0,  540,    0,    0,  364,  365,
    0,  489,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  198,    0,    0,    0,    0,
  351,    0,   82,    0,    0,    0,    0,   87,   84,    0,
    0,    0,  723,  961,    0,    0,  767,    0,    0,  773,
    0,    0,    0,    0,    0,  810,  809,    0,    0,    0,
  817,  835,  827,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  765,    0,    0,    0,  220,    0,  908,
    0,    0,  923,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  933,    0,  943,    0,    0,
    0,    0,    0,    0,  924,  869,   65,   57,   58,   64,
   59,   60,    0,    0,   22,    0,    0,    0,   42,    0,
    0,    0,  734,  754,    0,  717,    0,  733,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  672,    0,
    0,    0,    0,    0,  597,  598,  602,    0,  199,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,  402,  223,  224,  225,  226,  227,  228,  872,    0,
    0,    0,  276,    0,  221,    0,    0,    0,    0,  246,
    0,  448,  449,  421,    0,    0,    0,    0,    0,    0,
  364,  468,  365,  167,  957,    0,    0,  507,    0,  506,
  543,    0,  535,    0,    0,    0,  487,    0,  515,    0,
    0,  517,  520,  522,  362,    0,    0,    0,  119,    0,
  204,  364,    0,    0,  363,    0,   97,    0,  117,    0,
   93,    0,    0,    0,    0,   86,    0,  825,  826,  769,
    0,  771,    0,    0,    0,    0,    0,    0,    0,  828,
    0,    0,    0,    0,    0,    0,    0,  818,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  914,  913,    0,    0,  910,  909,    0,  918,
  917,  916,  915,  912,  911,    0,    0,  939,    0,    0,
    0,  938,    0,    0,    0,    0,    0,    0,   56,   62,
   54,   55,   23,  738,  739,    0,  736,  743,  744,  741,
    0,    0,  715,    0,    0,  660,    0,  661,    0,  678,
  674,  676,    0,    0,    0,    0,    0,  363,  619,  620,
  634,    0,  183,  179,    0,    0,  194,  190,    0,  365,
  873,  279,    0,    0,  365,  277,    0,    0,    0,  231,
    0,    0,  247,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  465,  509,    0,  510,    0,  134,    0,    0,
    0,  138,  140,  141,  142,  490,  492,  488,  519,    0,
    0,  365,    0,    0,    0,    0,  348,    0,  352,  364,
   88,    0,    0,   90,    0,    0,    0,    0,    0,    0,
  837,  836,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  219,  926,  919,  920,    0,  921,  935,  934,    0,  940,
  929,  930,    0,   40,    0,    0,  718,  663,    0,  533,
    0,  685,    0,    0,  682,  686,  687,  689,  690,  691,
    0,    0,  629,  365,  349,  621,    0,  174,  191,  365,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  284,  289,  283,
    0,    0,    0,  293,  294,  295,  296,  297,  298,  300,
  301,  303,  304,  305,  308,  266,    0,    0,  207,  232,
    0,    0,    0,    0,  233,  236,    0,    0,  365,  389,
  390,  386,  385,  388,  387,    0,  508,  536,  143,  131,
  139,    0,  364,  263,  126,  365,    0,    0,  366,  353,
    0,    0,   91,   92,   79,    0,  796,  804,    0,  797,
  805,  800,  808,    0,    0,  798,  806,  799,  807,  793,
  801,  794,  802,  795,  803,    0,  936,    0,  735,    0,
    0,    0,    0,    0,  699,  701,    0,  696,    0,  264,
  184,  403,    0,  311,    0,    0,    0,  307,    0,    0,
    0,    0,  306,    0,  334,  335,  292,  299,    0,  302,
  285,    0,  290,    0,  338,  291,  210,  215,  234,    0,
    0,  237,    0,  249,  251,  393,  365,    0,  368,    0,
    0,    0,  358,  359,    0,    0,   76,   94,    0,  922,
    0,    0,    0,  688,  683,  700,    0,    0,  282,    0,
  309,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  329,  330,  346,    0,    0,    0,    0,  341,  339,  238,
    0,    0,    0,    0,    0,  265,  365,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  707,    0,    0,
    0,  281,  278,  310,    0,    0,  313,    0,    0,    0,
  319,    0,  321,    0,  345,  344,  965,  343,    0,    0,
    0,    0,    0,  248,    0,  255,    0,  254,  369,  361,
    0,    0,  354,  118,    0,    0,    0,  709,  698,    0,
    0,  710,    0,  706,    0,    0,    0,    0,    0,    0,
    0,    0,  332,  331,  340,  342,  235,    0,    0,  259,
  258,  257,    0,    0,  839,  630,  705,    0,  712,    0,
    0,  312,  316,    0,    0,    0,    0,  333,  262,  260,
  261,    0,    0,    0,    0,    0,    0,  336,  314,    0,
    0,    0,    0,  711,  327,  315,  317,  838,  714,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1544,   28,   29, 1545,
   31,   32,   33,   34,   35,   36, 1546,   38, 1547,  639,
  120,  456,  951, 1185,  121,  705,  952,  122,  460,  710,
  146,  123,  124,  257,  974,  136,  953,  954,  197,  226,
  125,  955,  126,  462,   82,  237,  374,  421,  635, 1086,
  422,  859,  606, 1510,  836,  636,  637,  638, 1088,  238,
 1374,   74,  187,   75,  233, 1824, 1297,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  622,  624,  453,  258,  785,  540,  649, 1020,  367,
  627, 1074, 1289,  573,  734, 1046,  525,  526,  555,  359,
  607, 1060, 1440, 1569, 1232, 1441, 1442, 1443, 1444, 1445,
  650,  214,  206,  355,  580,  356,  357,  581,  582,  583,
  584, 1264, 1014,  528,  529, 1004, 1002,  530, 1008, 1003,
 1222, 1403, 1009, 1010, 1227,  369,  847,  848,  849,  850,
  787, 1027, 1418,  608,  789,  424,  852,  932,  549, 1346,
 1233,  425,  426,  427,  428,  429, 1419, 1420,  558, 1575,
 1723, 1250,  795,  796,  782,  797,  798,  799, 1724, 1725,
 1801,  337,  553, 1241, 1548,  193,  550,  551, 1242, 1243,
 1413, 1700, 1549, 1550, 1551, 1552, 1664, 1553, 1554, 1555,
 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565,
  430, 1786, 1710, 1790, 1708,  431,  689,  432,  433, 1093,
 1665, 1717, 1718, 1756, 1079, 1295, 1457, 1080, 1081,  285,
  487, 1598,  378, 1214, 1396, 1681, 1682, 1683, 1684, 1212,
  346,  569,  813, 1262,  570,  811,  221,   59,  341,  198,
  161,  324,  184,  331,  190,  773,  770,  332,  342,  803,
  343,  565, 1038,  185,  434,  325,  537,  571, 1263,   96,
  196,  242,  609,  610,  775, 1061,  776, 1062, 1063,  613,
  614,  615,  616,  821, 1054, 1050,  617,  618,  619,  829,
  620, 1512,  826,   40,   41,   42,   43,   44,   45,  155,
  515,   61,  478,  201,  348,  297,  298,  299,  300,  301,
  302,  303,  304,  305,  740,  501,  989, 1217,  502,  503,
  745,   46,  160,  320,  522,   47,  517,  518,  998, 1401,
 1221, 1526,  758,  140,  727, 1523,  716,  481,  287,  141,
  142,  435,  286,  488,  489,  490,  731,  975,  976,  977,
  978, 1206,  436, 1168, 1514, 1515,  438, 1517,  439,  440,
  441, 1738, 1639, 1739, 1740, 1741, 1784, 1825,  468, 1197,
   48,   49,  379,   50,   51,  509,  510,  960, 1193, 1376,
 1380,  469,  713,  211,  651,  652,  874,  875,  653,  654,
  655,  656,  657,  658,  659,  660,  661,  662,  663, 1318,
 1109, 1111, 1112,  931,  688,  442,  443,  444,  445,  446,
  235,  447,  448,  915,  449,  450,  451,  452,  679,  919,
 1169, 1164,  680,  128,
};
static short yysindex[] = {                                   7362,
 2333, -106, 2093,  130, 1309,   58, 2661,  319, 2840,   57,
  238, 4063, -106, 1174,  518, 4780, 1255, 4780,  271,    0,
  742,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  644,    0,    0,    0,    0,
    0,    0,12097, 4780, 4780, 4780, 4780, 4780,    0,    0,
  398, 1342, 4780, 4780, 4780,  695, 4780,  645, 4780, 4780,
    0, 4780, 4780,    0, 1193,    0,  698,    0,    0,  862,
 4780,    0, 4780, 4780, 4780,  939, 4780, 4780, 4780, 4780,
  645, 4780, 4780, 4780, 4780,    0, 4780,  938,    0,    0,
    0, 1286,    0, 1286,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1009,
 1020,   94, 1410,    0,    0,    0,    0,    0, 4780, 4780,
 4780, 1076, 1094, 1155,  157, 1879, 1092,  170,  175,  595,
 1097,    0, 4780, 1276, 1184,    0, 1218,    0, 4780, 4780,
 4780, 4780,    0,    0,    0, 4780,    0, 1377,    9, 1372,
   42,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1131,    0,    0, 1342,    0,   59,    0,
    0, 1652,    0,    0, 1074,    0,  428,    0,    0, 1279,
    0,    0, 1257,    0,    0, 1266,    0, 1473,    0,    0,
    0, 1342,    0,    0,    0, 1084, 1652,    0,   60,    0,
    0, 1443,    0, 1455,    0, 1286,    0,    0,    0,    0,
    0,    0, 1432,    0,    0, 1345,    0, 1439,    0, 1362,
    0,    0,    0, 1345,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8297,    0, 4780,    0,    0,    0, 4548,
 1419, 4861, 4780, 1062, 1485, 1795, 1531, 1543, 4548, 1466,
 4861, 1500, 1504, 1388,    0, 9988,    0,    0,    0,    0,
    0,    0,    0,    0, 7569,    0,    0,    0, 1460, 4780,
 1542, 1463,  982, 4780, 1478,    0,    0, 1184,    0,    0,
    0,    0,    0,    0,    0, 4780,    0,    0, 1652,    0,
 1443,    0, 1286,    0, 1547, 9988, 1519,    0, 1495, 1484,
  892, 1586, 1103,   42,    0, 1663,  908,    0, 4780,    0,
 1131,    0,    0,    0,    0, 4780, 1533, 3123, 1482, 3608,
 1916,    0,12097,    0,    0,  663,    0,    0,    0, 1913,
 1913, 1342, 1604, 1342, 2497, 1266,    0,    0, 7535,    0,
 4780, 4780,    0, 1533,    0,    0, 1697,  557, 1952, 4780,
 1730, 4780,    0,    0, 1956,  130, 5157,    0,    0,    0,
 1959, 1971, 1986, 1566,    0,    0, 1993,    0, 1994,    0,
 1995, 1996,    0, 2003,    0,    0,    0,    0,    0, 2004,
 1584, 1590,    0,    0,    0,    0,    0,    0,    0, 2017,
    0,    0, 8394, 2019, 2022,    0, 9988, 9988, 7667, 4780,
 2023,    0, 2226,    0,    0,    0,    0,    0,    0,    0,
    0, 2020, 1974,    0,    0, 2024,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1633,    0, 4780, 1639,    0, 4780, 4780,   69,
    0,    0,    0,    0,    0, 4780, 4780,  290,    0, 4780,
    0, 1758, 4780, 2043, 4548, 1680,    0, 4780, 8633, 1887,
    0,    0, 9988,    0, 2226,    0, 1768, 2045,    0, 2163,
    0,   82,    0,  602,    0,  -65,    0,  -65,    0,    0,
 2056,    0,  126, 1673,    0,    0,    0, 2063,  111, 2049,
 1533, 2064, 1848,    0,    0, 2160, 2076,    0,    0, 1739,
 1622,    0,    0,  232, 1780,    0,    0,    0,    0, 1342,
    0, 1913, 1913, 1913, 1913,    0, 2087,    0, 1854, 1855,
    0,    0,    0, 1834,    0, 7535,    0,    0, 7535, 1641,
    0, 1652, 1872, 4780,12015,    0,    0,  -21,    0, 4780,
  885, 1074, 1741, 1705, 1677,    0,    0,    0,    0,  573,
 1746,    0, 1692,  645,    0, 1342,    0, 1874,    0,    0,
 2497,    0,    0,    0,    0,  183, 1706,    0,    0,    0,
 1707,    0, 1759, 2115,    0, 1808, 2119,   45,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2070,  151,    0,    0,    0,    0,  138, 2122, 2123, 2119,
 1652,    0, 1851,    0, 1902,    0, 1342, 1342,    0, 3123,
  428,    0, 1787,    0, 1760, 2129,    0, 9365,    0, 1785,
 1793, 2139, 5567, 2143,    0,    0,    0, 5060, 6813,   18,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  934, 8872,  131,    0, 4780, 9988, 1128,
 1152, 1154, 2611,    0,    0, 9988, 5719, 2226, 1726,  562,
 9988, 8872,  834,  834, 1174, 2038, 1657, 2145,    0,    0,
 8952, 4780, 9988, 9988, 9988, 9988, 9988, 8872, 9988, 1233,
 4780,    0,    0, 5305, 1748,    0,    0, 1729, 4861,    0,
 1877,    0, 1710, 1174, 2157,    0, 1877, 5305, 1886, 1896,
 4780,    0, 2038,    0, 2258,  633, 1767, 8872, 4780,    0,
    0,    0, 1072,    0,    0,  474,    0, 1751,    0,    0,
    0, 4780,  -46, 4780,    0,    0, 4780, 4780,    0,    0,
 8872, 1960, 3123,    0, 1723, 9988, 4780,    0,    0, 1342,
 1342,    0,  -70, 1692, 1692, 1770,  645, 1103,    0,    0,
 2197,    0,    0,    0,    0, 1961,    0, 4780,    0,    0,
    0,    0,    0, 1967, 2205,    0, 1946, 1963,    0,    0,
 1910, 1917, 1286, 1977,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  645, 4780,12053, 1866, 1871, 1876,
    0,  645,    0, 1486,    0,    0, 1846,    0,  711,  114,
 1862,    0,    0,    0,  645,    0, 2115,  146,    0,    0,
    0, 1970,  146, 1861,  146,    0, 2243,  645,  645,  645,
    0, 1533, 4780,    0, 1932,    0, 1717,    0,    0,    0,
  663, 1878, 4780, 1724,  141,    0,    0, 2249, 2030,    0,
  130, 4780, 1881, 5958,    0,    0, 1881, 5567,    0,    0,
    0, 4593,    0,  699,   28, 9988, 9988, 6055, 2251,   -8,
 6309, 9988, 6461, 1235, 6570, 6824, 9191, 6976, 7085, 7339,
 5719, 5719, 9988, 9988, 2042, 9988, 2266, 9988, 2264, 2310,
 9988, 9988, 9988, 9988, 9988, 9988,    0,    0,    0,    0,
    0,    0,    0,    0, 1991, 1315,  508, 9988,  609, 8872,
    0, 5719, 1775, 2268,  595,    0,    0,    0,    0, 2226,
 1747,    0,    0, 2033,  834,  834,  756,  756,    0,  803,
    0,    0,    0,    0, 4780, 4780, 4780, 4780, 4780, 4780,
  176, 2273,    0,    0,    0, 5305, 1934,    0,   16, 1955,
 4780,    0,  595, 8872, 1955, 2280, 2273, 5305, 4632, 2005,
    0,    0,  633, 2286, 2283,  757,    0,    0, 5719, 2000,
    0,    0,    0,    0,    0,    0,    0,    0,  -62,    0,
    0,    0,    0,    0, 1749, 1878,    0,    0,    0, 2285,
    0,    0,    0, 1756,    0, 1913, 2071,    0,  -70,    0,
    0,    0,    0,    0,    0,    0, 2290, 5719, 1610,    0,
    0, 2040,    0,    0,    0, 1610,    0,    0,    0, 1286,
 1286, 1843, 2308,    0,    0,    0,    0,    0,  -61,  -13,
 1301,    0, 2085,    0,    0,    0, 4780,    0,    0,    0,
  130, 1829, 1937, 1961, 2313,    0, 1692, 1835,    0,    0,
 1006,    0, 2298, 4780, 1230,  645, 2319, 2321, 2322, 2102,
  428, 2040, 1913, 1780, 3123,    0, 1746,  595, 1914, 1927,
    0,  491,    0,  645, 1983, 2075, 9365,    0,    0,  645,
 1894, 2084,    0,    0, 2354, 2355,    0, 5719, 5719,    0,
 5719, 5719, 2331, 2226, 2359,    0,    0, 2226, 2362, 9430,
    0,    0,    0, 2055, 2386, 2226, 2388, 1497, 2391, 2226,
 2392, 9988, 9988, 2251, 9988, 9510, 2393, 2226, 2394, 2395,
 2226, 2397, 9988, 2226, 2399, 2226, 2401, 2403, 2226, 2404,
 2406, 2226, 2408,    0, 2187, 2562, 2589,    0, 2676,    0,
 2749, 9988,    0, 2765, 3075, 3091, 3129, 3321, 3387, 9988,
  645, 8872, 2226, 2021, 8872,    0, 9988,    0, 2147,  641,
 9988, 8872, 9988, 9988,    0,    0,    0,    0,    0,    0,
    0,    0, 2130, 5305,    0, 5305,  176, 2273,    0,  211,
  211,  148,    0,    0, 7569,    0, 1841,    0, 2280, 2273,
 2415, 4632,  757, 2419, 8872, 4780,  633, 2078,    0, 2079,
 2080, 2120, 2190, 2140,    0,    0,    0, 2040,    0, 1914,
  460, 2048, 1342, 2082, 1692, 2089, 1692,    0, 5719,  735,
 1326,    0,    0,    0,    0,    0,    0,    0,    0, 2027,
 2204, 2040,    0, 9988,    0, 2191, 2074, 1843, 1193,    0,
 5719,    0,    0,    0, 2096, 2106, 2112, 2128, 2133, 2135,
    0,    0,    0,    0,    0, 2464,  130,    0, 1770,    0,
    0,  146,    0, 1610, 2138,  146,    0,  146,    0, 1961,
 2467,    0,    0,    0,    0,  663, 2204, 1692,    0, 1780,
    0,    0, 7569, 2245,    0,   13,    0, 2285,    0, 2485,
    0, 2495, 1342,  -45, 2498,    0, 1342,    0,    0,    0,
 2187,    0, 2187, 9988, 2109, 2109, 1174, 2499, 1873,    0,
 2109, 2109, 9988, 2109, 2109, 3269, 2226,    0, 1942, 9988,
 2226, 2109, 2109, 2109, 2109, 2226, 2109, 2109, 2109, 2109,
 2109, 2109,    0,    0, 7535, 2500,    0,    0, 3457,    0,
    0,    0,    0,    0,    0, 3548,  156,    0, 8872, 2237,
 2111,    0, 8872, 3557, 1908, 1406, 2226, 2162,    0,    0,
    0,    0,    0,    0,    0, 2114,    0,    0,    0,    0,
 1768, 8872,    0, 4632, 2415,    0, 1928,    0,  757,    0,
    0,    0,  633, 5633, 5719, 2158, 2204,    0,    0,    0,
    0, 1913,    0,    0, 1342, 1486,    0,    0,  771,    0,
    0,    0, 4780,11833,    0,    0, 2226, 2275, 2191,    0,
 1090, 4780,    0, 2216,  869, 2116, 2219, 2121, 2263, 2228,
 2269, 2040,    0,    0, 2530,    0, 2532,    0, 2222, 2275,
 2138,    0,    0,    0,    0,    0,    0,    0,    0, 2040,
 1746,    0, 1486, 2085, 2259, 2196,    0, 1878,    0,    0,
    0, 2212, -100,    0, 1961, 2213, 2226, 2545, 2546,  595,
    0,    0, 2547, 2550, 2226, 2551, 2553, 9988, 9988, 2226,
 2559, 2560, 2565, 2567, 2569, 2571, 2572, 2573, 2576, 2578,
    0,    0,    0,    0,  645,    0,    0,    0, 8872,    0,
    0,    0, 1767,    0,  211, 1767,    0,    0, 1245,    0,
 2317,    0, 2574, 2577,    0,    0,    0,    0,    0,    0,
   18,  431,    0,    0,    0,    0, 1692,    0,    0,    0,
    0, 7535, 1911, -105, 2563,  468, 2585, 2288, 9988, 2580,
 2586, 2583, 2587, 2588, 2591, 2293, 2592,    0,    0,    0,
 2327,11806, 2598,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2596, 4780,    0,    0,
 2292, 2304, 4780, 1843,    0,    0, 1286, 4780,    0,    0,
    0,    0,    0,    0,    0, 2204,    0,    0,    0,    0,
    0, 2204,    0,    0,    0,    0,  792, 1767,    0,    0,
 9988, 1342,    0,    0,    0, 1342,    0,    0, 9988,    0,
    0,    0,    0, 2226, 2226,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2617,    0, 2000,    0, 2000,
 5719, 4780, 1399, 5633,    0,    0, 2291,    0, 2620,    0,
    0,    0,  206,    0, 9749, 4780, 9988,    0, 2153, 2329,
 5719, 2226,    0, 5719,    0,    0,    0,    0, 1456,    0,
    0,  954,    0, 2225,    0,    0,    0,    0,    0, 1843,
 1286,    0, 2284,    0,    0,    0,    0, 2085,    0,  792,
 2633,  757,    0,    0, 2000, 2226,    0,    0, 1406,    0,
 2140, 2140,   18,    0,    0,    0, 5786, 1610,    0, 2623,
    0, 2167, 8297, 2007, 9988, 1456,  886, 2625,  890,  976,
    0,    0,    0, 4780, 4780,  130,  155,    0,    0,    0,
 2284,   -5, 1843, 2303, 2320,    0,    0,  757, 2644,  792,
 2344, 2140, 2669, 1767, 2158, 2158, 4780,    0, 2670, 2668,
 1373,    0,    0,    0, 2305, 1456,    0, 1850,  215, 2278,
    0, 2412,    0, 1456,    0,    0,    0,    0,11833,  954,
 1843,  686,  686,    0, 2423,    0, 2287,    0,    0,    0,
  757,  792,    0,    0, 2000, 2338, 2338,    0,    0, 5786,
 2680,    0, 4780,    0,  917, 2667, 1421, 1456, 2435, 2427,
11833,11833,    0,    0,    0,    0,    0, 2270,  812,    0,
    0,    0, 1282, 2140,    0,    0,    0, 4780,    0, 1456,
 2688,    0,    0,  327, 4780,11833, 2438,    0,    0,    0,
    0, 5719, 2158, 2690, 2705, 1949,11833,    0,    0,11833,
   18, 2338, 4780,    0,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  258, 1145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  160, 1520,    0,    0,    0,    0,    0,    0,
 2747,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  893,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2411,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2368,    0,    0,
    0,  219,    0,  219,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2375,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1032,    0,    0, 7906,
 5415,    0,    0,    0, 1039,    0,    0,    0, 1080,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  209,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    6,    0,    0,    0,    0,  613,    0,    0, 1042,
    0,    0,    0,    0,    0, 1248,    0,10349,    0,    0,
    0,    0,    0,    0,    0,    0,    6,    0,  516,    0,
    0, 2719,    0,    0,    0, 2501,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1061,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1087,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1141,    0,    0,    0,    0,
    0,    0,    0, 5093,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  494,    0,    0, 1130,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    6,    0,
 2719,    0, 2501,    0, 1182,    0,    0,    0,    0,  279,
    0,    0,    0, 1191,    0, 2490,    0,    0,    0,    0,
 1204,    0,    0,    0,    0,    0, 2505, 1071,12144,    0,
 1247,    0,    0,    0,    0,  708,    0,    0,    0, 2314,
 2314,    0,    0,    0, 1256, 1450,    0,    0,    0,    0,
    0,    0,    0, 2505,    0,    0, 1457,    0,    0,    0,
    0,    0,    0,    0, 7841,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1880,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2442,
    0,    0,    0, 2695, 2948,    0,    0,    0,    0,    0,
    0,    0, 1246,    0,    0,    0,    0,    0,    0,    0,
    0,  184, 3201, 2189,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1289,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1313,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  145,    0,    0, 2454,    0,  217,
    0,    0,    0,    0,    0, 1051,    0, 1051,    0,    0,
  629,    0,  830,  909,    0,    0,    0,    0, 1061,    0,
 2505,    0,    0,    0,    0,  136,  252,    0,    0,  236,
    0,    0,    0, 2764,  166,    0,    0,    0,    0,    0,
    0, 2314, 2314, 2314, 2314,    0,  250,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2514,    0,    0,    0,    0,    0, 2490,    0,    0,
 1462,    0, 2309,    0,    0,  553,    0,    0,    0,    0,
  849,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1338,    0,    0,    0,    0, 3382, 1067,    0,    0,    0,
 5683,    0,    0, 8196,    0,    0, 8769,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2880, 3107,    0,    0,    0,    0,10307, 5029,    0, 8769,
    6,    0,    0,    0,    0,    0,    0,    0,    0, 1071,
  999,    0,    0,    0,    0, 2369,    0,    0,    0,    0,
    0,    0,    0,    0,10559,10691,10866,    0,    0,  224,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10068,    0,10068,    0,    0,    0,10068,
10068,10068,    0,    0,    0,    0,    0, 2330,    0,    0,
    0,    0, 4171, 4403, 8058, 2740,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  310,    0,    0, 1520,    0,    0,  310,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  495,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -41,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1071,    0,  144,    0,    0,    0,    0,    0,
    0,    0, 3211,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  925,    0,    0,    0,    0,
    0,    0,    0, 2753,    0,    0, 2302, 1997,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  752,  758,  782,
    0,    0,    0, 1383,    0,    0,    0,    0,    0,    0,
 2058,    0,    0,    0,    0,    0, 8196,    0,    0,    0,
    0,  808,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2505,    0,    0,  376,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9850,    0,    0,    0,  336,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,10068,    0,    0,    0,    0, 1538,
    0,    0,    0, 3433, 4656, 4888, 3686, 3918,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1340, 1346,    0,    0,    0,    0,    0,    0,    0, 1424,
    0,    0, 7906,    0, 1424, 1435, 1440,    0,    0,    0,
    0,    0,    0,  819,  172, 2365,    0,    0,    0, 1056,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  240,
    0,    0,    0,    0,    0, 2314, 2337,    0, 3696,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2516,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1312,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1446,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3845,    0,    0,    0,    0,    0,    0,
    0,    0, 1293,    0,    0,    0,    0,    0,    0,    0,
  999, 2516, 2314, 1476, 1071,    0, 2525, 7906, 1488,  133,
    0,    0,    0,    0, 9850,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 5934,    0,    0,    0, 6260,    0,    0,
    0,    0,    0,    0,    0, 7355,    0,10334,    0,10775,
    0,    0,    0,    0,    0,    0,    0,10943,    0,    0,
10949,    0,    0,11081,    0,11123,    0,    0,11213,    0,
    0,11255,    0,    0, 9173,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2363,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1340, 1491,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1511, 1551,
 1555,    0, 2456, 1451,    0, 3998,    0, 2457,    0, 2460,
 2466,    0,    0,  488,    0,    0,    0, 2516,    0, 1488,
    0, 3017,    0,    0,    0, 2353,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6149,
    0, 2542,    0,    0,    0, 1708,    0, 1312, 2411,    0,
    0,    0,    0,    0,  793,  795,  831,  833,  865,  945,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  887,    0,    0,    0,    0, 3278,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1593,
    0,    0,    0,    0,    0,    0,    0, 1594,    0,    0,
    0,    0,    0,  159,    0,    0,    0,    0,    0,    0,
  107,    0,  124,    0,    0,    0, 8058,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11287,    0,11370,    0,
11390,    0,    0,    0,    0,11486,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1655,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1607,    0,    0,    0, 2518,    0,
    0,    0,    0,    0,    0,  856,    0,    0,    0,    0,
    0, 2314,    0,    0,    0, 2556,    0,    0,    0,    0,
    0,    0,    0, 2507,    0,    0, 2775, 1619, 2137,    0,
 2490,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2516,    0,    0,    0,    0,    0,    0,    0, 1621,
 1005,    0,    0,    0,    0,    0,    0,    0,    0, 2516,
 2525,    0,  384,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  455,    0,11520,    0,    0,10068,
    0,    0,    0,    0,11584,    0,    0,    0,    0,11616,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  472,    0,    0, 1185,    0,    0, 2456,    0,
 1954,    0,    0,  187,    0,    0,    0,    0,    0,    0,
 1789, 2787,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1454, 1525,    0,    0,    0,    0,    0,    0,    0,
    0, 2522,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2824,    0,    0,  115,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  230,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11658,11754,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  634,    0,  372,
    0,    0,    0,    0,    0,    0, 2791,    0,    0,    0,
    0,    0, 2773,    0,    0,    0,    0,    0,    0,    0,
    0, 2774,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2526,    0,    0,    0,    0,    0, 2824,
  282,    0,  108,    0,    0,    0,    0,    0,    0,    0,
  287,  139,    0,    0,  340, 2775,    0,    0,    0,    0,
  309,  306, 4483,    0,    0,    0, 2787,    0,    0,    0,
    0,    0,  189,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  109,    0, 1312,  473,  617,    0,    0, 2456, 1805,    0,
    0,  135,    0,  472,  120,  216, 1379,    0,    0, 2796,
    0,    0,    0,    0, 2780,    0,    0,    0, 2536,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2824,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  642,    0,    0,    0,  634, 2800, 1229,    0,    0, 2787,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2456,  309,    0,    0,    0,    0,    0,    0,
 2784,    0,    0,    0,    0,    0,11710,    0,    0,    0,
    0,    0,  120, 2803,    0,    0,    0,    0,    0,    0,
 3822, 2800,    0,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2848,    0,    0, 2850,
    0,    0,    0,    0,    0,    0,   10,    0, 2851,    1,
  272,   22, -601, 1666,  280, -302,  -56, 2845, 2593,    0,
  -28,    0, 2595,  -96,  -40,    0,  286, -322,  -27,  -63,
  -43, -220, 2600, 2161,    0,    0,    0, 1437,    0,    0,
  -50, 1779, -487,   -1, -858, 2233, 2016, -298, 1788, 2788,
 -275,    0,  -60,    0, -476,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -345, -629, 1794, -294,  951, -881,    0,
    0,    0,    0, -327, -125,-1048, 1807, -332,  -48,    0,
 -518,    0,    0, 1444, -967,    0, 1438,    0,    0, 2561,
 -577,  -17,    0,    0,    0,    0, 2533,    0, 2307, 2570,
 -230, 1822, -688,    0,    0,    0, -708,    0,    0,    0,
    0,    0,    0, 1883,    0, 2579, 2142, 1823, 2568, 2582,
    0, 1254,    0, -451, 2113,  -35, -662,-1244,    0,    0,
    0, -949, -920, -914, -265, -912,    0, 1480, -496,    0,
 1179,-1179, 1487,    0, 2343,    0,    0,    0, 1186, 1183,
 1146,  193, -299, -531, -353,    0,    0, 2132,    0, 1674,
    0,    0, -218,    0,    0,    0,    0, -209,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1212,    0,  229,    0,    0,   96,    0, -384, -170, 1102,
 1258,    0, 1158,    0, 1929, 1703, 1528,    0,-1051, -834,
 -432,    0, -492,-1352, -623,    0, -512, 1249,  323, -284,
 -506, -720, -923,-1220,    0,    0,   -3,    0,    0,    0,
    0,    0, 2584,    0,    0,    0,    0, 2597, 2371,    0,
    0,    0,    0,    0,   -9, 2606,    0,    0,    0,    0,
  -10,    0,    0,    0, -337, 2100, -333, 1660, -430,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2318,
    0,    0, 2136,    0,    0,    0,    0,    0,    0,    7,
    0, 2927,    0,    0,    0,    0,    0, 2648,    0,    0,
    0,    0, 2455,    0, 2470,    0,    0,    0, 2220,    0,
    0,    0, -789,    0,    0,   68,    0, 2199,    0,    0,
    0,    0,    0, -598, -910, -944,  852, -176,  873,    0,
 2823, -120, 1376,    0, 2241, -627, 2236,    0, -771, 1998,
    0,    0, -116, -243,    0, 1347, -954,    0, -658, -609,
 -605, 1465,    0, 1196,    0, 1242,    0, 1149,    0,    0,
    0,    0, 2475,    0,    0,    0, -428, 2271, 2031,-1022,
    0,    0,    0, -679, -417,  181,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2575,
  977, 1875,    0, -485,    0,    0,    0, -908,    0, 1769,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -183, 1837,    0,    0,
};
#define YYTABLESIZE 12656
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  259,  154,   37,
  437,  204,  278,  281,  208,  192,  749,  279,  282,  486,
  943,  611,  579,  574,  243,  612,  217,  209,  195,  229,
  252,  546,  308,  544,  687,  247,  786,  895,  712,  755,
  222,  461,  224,  186,  188,  191,  194,  148,  199,  248,
  461,  253, 1005,  210,  188,  215,  254,  194, 1245,  148,
  199,  735,  228,  127,  625,  251,  219,  991, 1423, 1234,
  246,  239,  149,  148,  241,  188, 1234,  239,  215,  191,
  194,  250,  148,  199,  228,  127,  925,  194,  313,  249,
 1173,  996,  536,  265,  774,  267,  507,  777, 1235,  917,
  640,  266,  482,  788, 1236, 1235, 1238,  256,  256,  309,
 1239, 1236,  709, 1238,  746,  963,  966, 1239, 1022,  210,
  148,  210,  311,  993,  851,  289,  733, 1029, 1195,  371,
 1077, 1052,  350,  215,  693,  618,  333,  263,  356,  307,
  194,  148,  228,  622,  731, 1246,  154,  768,  256,  256,
  860,  256,  256, 1292,  748,  746, 1254, 1051,  746,  564,
  697,  360,  315, 1032,  770,  459,  256,  256, 1377,  744,
  719,  654,  897,  746,   77,  437,  618,  838, 1275,  618,
 1085, 1072,  356,  323,  622,  731,  680,  622,  731,  508,
 1057,  350,  762,  693,  618, 1270, 1496,  356, 1760,   89,
  263, 1203,  622,  731,  764,  765,  766,  767,  624,  276,
  277,  752,  654,  940,  280,  697,  513,  611,  564, 1184,
  454,  612,  820,  694,  459,  865,  318,  680, 1463,  695,
  654,  457,  277, 1596,  980,  626,  493,  277,  707,  625,
  457,  835,  323, 1293, 1459,  680,  210,  325,  579,  461,
  148,  611,  463,  127, 1603,   77,  697,  564, 1754,  148,
  651,  463,  746,  794,  694,  500, 1698,  624,  317,  268,
  506,  761, 1458,  277,  697, 1691, 1255, 1692,  628,  891,
  191,  746,  694,  133,  148, 1006,  355, 1257,  695, 1101,
  472,  134,  611,  545,  626, 1762, 1646,  791,  625,  321,
  548,  575,  209,  577,  561,  693, 1438,  563,  461,  737,
  611,  738,  832, 1144, 1145,  334,  564,  629,  633,  210,
  621,  830,  746, 1668, 1234,  746,  210,  365,  210,  129,
  210, 1218, 1732,  566,  623, 1190, 1196,  628,  147,  681,
  746,  631, 1215, 1113, 1170,  355,  693,  322,   60,  693,
  508,  194,  199, 1235, 1187, 1036,  987, 1529, 1007, 1236,
  148, 1238,  191,  596,  693, 1239, 1199, 1201,  737, 1114,
 1754,  681,   79,  988, 1253,  125,   85,   83,  792,   85,
 1256, 1433,  739,  195,  519,  612,  256, 1694,  256,  831,
  283, 1604,  335, 1216, 1672,  256,  130, 1058,  681,  486,
 1247, 1248, 1647,  892, 1595,  240, 1600,  272,  731,  364,
  690, 1000,  681, 1102,  596,  273,  268,  706,  970,  793,
 1464, 1448, 1804,  459,  366,   14,  703,  834, 1258, 1078,
  681,  506,  706,  896,  125, 1389, 1763,  711,  924, 1516,
 1230,  717,  195,  743,  612,  148,  457,  814,  194,  463,
 1097, 1191, 1299,  618,  939, 1503,  148, 1727, 1759,  323,
  148,  622,  486,  194,  731,  148,  356,   89,  154,  356,
 1506,  459,  252, 1495, 1017,  618,  271, 1019, 1789,  763,
  730,  679, 1629,  622,  722,  486,  256,  693,  461,  654,
 1720, 1025,  768,  587,  695,  516,  720,  746,  516,  654,
  760,  511,  654, 1378,  680, 1026,  697,  461,  256,  770,
  131,  654,  695,  252, 1404,  256,  252,  680, 1043,  790,
 1310, 1311,  708, 1312, 1313,  816,  680, 1593, 1531,  262,
  256,  252, 1194, 1566, 1084, 1567,  651, 1016,  992,  275,
 1287,  694, 1070, 1764,  783,  210,  693,  971,  624,  256,
  783,  694,  587,  695,  694,  461, 1087,  695,  654,  746,
  695,  746,  377,  694, 1286, 1451,  732,  654,  746,  611,
 1594, 1013,  815,  680,  693,  626,  845,  846,  356,  625,
 1436, 1797,  680,  459,  837, 1092,  350,   76,  693,  618,
  356,  611,  356,  323,  209,  564,  731,  622,  325,  860,
 1385,  746,  697,  275,  731, 1013, 1605,  984,  256,  256,
  694,  654,  372,  819,  355,  746,  253,  355,  628,  694,
  210, 1509, 1450,  654, 1319,  654,  680,  275,  593, 1183,
 1827,  508,  275,  731,  731, 1609,  731,  618, 1011, 1012,
  680,  357, 1640,  731,  731,  731,  731,  731, 1642,  731,
  899, 1409,  731,  731,  731,  731,  731,  253,  215,  746,
  253,  967,  624,  694,  746,  944,  564,  461,   76,  695,
  681,  372,  973, 1425,  681,  253,  737,  694, 1225, 1516,
  706,  746,  933,  695,  344,  357, 1397,  593,  746,  626,
  210,  210,  693,  625,  463,  693, 1528, 1675,  486,  463,
  357,  693,  681,  612,  693,  611,  714,  467,  463, 1001,
 1001,  194,  506,  500, 1679,  990,  194,  209, 1470,  730,
   97,  454,  746,  125, 1177,  612,  355, 1180,  516, 1244,
 1742,  195,  148,  715,  148, 1518, 1298,  210,  355, 1100,
  355, 1252,  548,  210,  780, 1288,   85,  210, 1234, 1421,
  252,  378,  252, 1237, 1507,   77,  496,  376,  681,  252,
 1237,  345, 1381,  516, 1274, 1015,  467,  681,  210,  891,
 1035, 1037, 1424, 1635,  784, 1410,  156, 1235, 1734,  681,
 1649,  377,  746, 1236, 1519, 1238, 1087, 1365, 1520, 1239,
 1776, 1777,  383,  681,  384, 1726,  210,  210, 1636,  695,
  157,  122,  695, 1034, 1733,  693,  200,  135, 1042, 1049,
  378, 1530,  695,  842, 1071,   79,  376, 1521,  664, 1048,
 1387, 1055,  695,  866, 1082,  695, 1491,  693,  873,  603,
  380, 1680,  379,  148,  695,  587, 1067, 1068, 1069,  612,
  377, 1437,  143,  148,  508, 1769, 1174, 1447,  392, 1345,
  252,  383, 1094,  384,  283,  697, 1637,   79,  695,  664,
 1455, 1370,  664, 1372,  382,  516,  135,  695,  920,  921,
  587,  372,  252,  808,  693,  696,  372,  664, 1832,  252,
  697,  695,  372,  693,   14, 1181,  136, 1415,  603,  380,
  695,  379,  566,  892,  253, 1176,  253,  284,  587, 1188,
 1586,  587,  891,  253,  628, 1178, 1182,  392,  588, 1579,
  809, 1200,  498,  252,  697, 1165, 1166,  587, 1592, 1179,
  516,  567, 1421,  382,  516,  506, 1750,  693,  437,  486,
 1752,  984,  568, 1452,  695,  463,  194,  199,  463,  228,
  127,  693, 1162,  587,  381,  136,  463,  144,  695,  959,
  105,  566, 1399,  943,  145,  216, 1810,  762,  463,  463,
 1098,  437, 1400,  194,  681,  516, 1432,  588,  516,  357,
  593,  467,  357,  252,  420, 1518,  122,  122, 1358,  106,
  107, 1360,  108,  516,  679,  516, 1805, 1806,  423,  109,
  110,  111,  112,  113,  253,  114,  891, 1454,  115,  116,
  117,  118,  119,  381,  137,  593,  736,  611, 1237,  372,
 1628,  612,  922, 1630,  810,  378,  253,  202,  203, 1754,
  681,  376,  959,  253, 1519,  295,  892,  486, 1520,  681,
  158,  562,  891,  593, 1753,  692,  593, 1265,  571,  209,
  693,  568, 1838, 1524,  866,  377,  454,   79,  866, 1276,
  592, 1266,  593, 1693, 1265,  681,  383,  253,  384, 1167,
  695, 1370, 1798,  137, 1281,  210,  526, 1735, 1736, 1375,
 1375, 1379,  210, 1707, 1527, 1363, 1709, 1208,  593,  671,
 1643,  357, 1301,  159, 1099,  135,  325,  135, 1305, 1388,
  562, 1209,  105,  357,  380,  357,  379,  571, 1277, 1406,
  568, 1408,   76, 1799,  467, 1685,  526,  526, 1773,  592,
  526, 1820,  392,  692,  681, 1210,  486,  253,  693,  695,
  892,  106,  107,  671,  108,  526,  794,  526,  382,  570,
  891,  109,  110,  111,  112,  113,  664,  114,  671,  664,
  115,  116,  117,  118,  119,  325,  664,  891,  378,  664,
 1800,  891,  483,  664,  376,  234,  892,  526,  664, 1357,
  491,  664, 1453, 1821,  364, 1601,  136, 1728,   76, 1211,
  105,  603, 1001,  697,  463, 1497,  463,  664,  377, 1500,
 1823,  557,  516,  236,  695,  135,   79,   79,  570,  383,
  399,  384,  463,  283,  542,  697,  307,  194,  516,  106,
  107,  664,  108,  400,  423,  664,  603,  543,  381,  109,
  110,  111,  112,  113,  664,  114, 1713, 1771,  115,  116,
  117,  118,  119, 1511,  516,  695,  480,  380,  612,  379,
  516,  664, 1676,  516,  603,  485,  893,  603, 1677,  523,
  557, 1775,  697,  695, 1831,  668,  396,  151,  524,  399,
  588,  105, 1462,  603,  892,  160, 1466,  372,  664, 1803,
  244,  382,  400,   79,  496, 1714,  516, 1435,  372,  612,
  664,  892,  664, 1276,  942,  892, 1715, 1513,  664,  603,
  106,  107,  364,  108,  137,  588,  668,  612,   43,  668,
  109,  110,  111,  112,  113,  697,  114,  508, 1678,  115,
  116,  117,  118,  119,  668,  396,  151,  664,  664,  697,
  664,  250,   51,  588,  160, 1627,  588,  664,  664,  664,
  664,  664, 1280,  664,  526,  256,  664,  664,  664,  664,
  664,  338,  588, 1663,  339,  516,  491,  161,  255,   41,
  526,  381,  516,  526,  526,   24,  526,   43,  241,  801,
  526,  802,  250,  526, 1001,  250,  696,  694,  588,  695,
  516,  697,   99,  678, 1532,  497,  526,  683,  684,  686,
  250,   51,  526,  681,  463,  526,  105,  209,  506,  340,
 1716,  532,  195,  194, 1375,  491,  681,  283,  326,  241,
  516,  260,  592, 1577,  516,  681,  161, 1731,   41,  101,
  671, 1641,  261,  210,   24,  106,  107,  671,  108,  115,
  671,  283,  148,  283,  671,  109,  110,  111,  112,  113,
  498,  114,  671,  742,  115,  116,  117,  118,  119,  723,
  901,  533, 1237,  725,   47,  508,  327,  499,  671,   49,
  942,  195,  681, 1731,  526,  469,  115,  696,  694,  152,
  695,  681,  697,  264,  903,  592,  905,  231,  592,  437,
  268,  453,  671, 1511, 1754,  534,  526, 1571,  102, 1369,
  241, 1371, 1645,  526,  592,  127,  269,  526,  103, 1813,
 1673,  115,  742,  535,  526,  361, 1731,  367, 1712, 1572,
   25, 1626,  671,   47,  232,  681, 1573,  671,   49,  980,
  592,   79,  526,  104,  469,  453, 1122,  526,  152,  681,
   48,  241,  695,  420,  980,  695,  506, 1513, 1731,  671,
  453,  362, 1123,  188, 1674,  291,  151,  274,  941,  275,
 1574,  671,  526,  671,  127, 1712,  526,  270,  696,  694,
 1795,  695, 1513,  697, 1671, 1650,  367,  151,  328,   25,
   50, 1687,  668,  668,   45, 1688,  983,  751,  933,  638,
  292,  638,  638,  148,  638, 1208,  329, 1124,  191,   48,
  985,  695, 1817, 1818, 1721, 1712,  151,  638,  870, 1209,
  695,  870,  638, 1794,  751,  985,  149,  148,  293, 1513,
  139,  294,  128,  116,  692,  151, 1125, 1829,  872,  693,
  330, 1259, 1208, 1210,  151, 1703,   46,  295, 1836,   50,
  151, 1837,  290,   45, 1712,  485, 1209, 1712,  216,  900,
  216,  350,  933,  210,   80,   81,  916, 1631,  151, 1513,
  870,  923,  423,  296, 1161,  306,  194, 1513, 1260, 1712,
 1210,  930,  508,  934,  935,  936,  937,  938,  423,  930,
  316,  128,  116, 1755, 1231,  668,  150, 1211,  351,  216,
  352,  195,  216, 1126, 1822,   46,  138,  139, 1513,  149,
  148, 1513,  375,  702,  508,  508,  701,  216,  485,  216,
  380,  779,  195,  353,  778,  692, 1737,  148,  151,  319,
  693,  336,  105, 1513, 1211,  871,  668,  927,  871,  508,
  698,  485,  347,  152,  188, 1757,  516,  229,  354, 1781,
  508,  195,   79,  508, 1095,  708, 1758,  349, 1096,  149,
  148,  106,  107,  506,  108,  726, 1737,  148,  152, 1809,
  195,  109,  110,  111,  112,  113,  358,  114,  195,  195,
  115,  116,  117,  118,  119,  195, 1782,  871,  229,  152,
  962,  229,  708,  961, 1711,  506,  506, 1076,  368, 1783,
 1075,  149,  148,  195, 1083,  708,  229,  856,  372, 1737,
  148,  657,  370,  191,  376,  195,  692,  195,  152,  638,
  506,  693,  396,  397,  398,  657,  638, 1175,  692, 1219,
 1174,  506, 1075,  657,  506,  638, 1224,  152,  191, 1223,
  195, 1711,  202,  203,  373, 1094,  152, 1323,  638,  657,
  406,  407,  152,  377,  638,  638,  696,  694, 1171,  695,
  458,  697,  638,  191,  466,  195, 1103, 1104, 1108,  692,
  152, 1116, 1118, 1120,  467, 1128, 1131, 1134, 1136, 1139,
 1142, 1711,  638, 1146, 1147,  638, 1149,  692, 1151, 1793,
  470, 1154, 1155, 1156, 1157, 1158, 1159, 1117,  105, 1121,
  471, 1129, 1132,  657, 1137, 1140, 1143,  473, 1163, 1268,
  423,  638, 1267,  638,  638, 1273, 1044, 1045, 1272,  978,
 1711, 1383,  475, 1711, 1382,  638,  476,  106,  107,  638,
  108,  696,  694,  477,  695,  492,  697,  109,  110,  111,
  112,  113,  494,  114,  384, 1711,  115,  116,  117,  118,
  119,  504,  495, 1472,  485,  105, 1174,  123,  123,  955,
  978,  978,  978,  978,  978,  978,  978,  724,  696,  694,
  514,  695,  386,  697, 1749,   14,  520,  521,  978,  978,
  978,  978,  531,  552,  106,  107,  638,  108, 1502,  539,
  559,  698,  638,  684,  109,  110,  111,  112,  113,  562,
  114,  388,  638,  115,  116,  117,  118,  119, 1508, 1644,
  978,  698,  978,  572, 1787,  576,  638,  638,  638,  638,
  638,  638,  626,  696,  694,  241,  695,  229,  697, 1835,
  638,  630, 1754,  632,  684,  634,  483,  684,  664,  638,
  638,  638,  638,  638,  638,  638,  638,  638,  638,  638,
  665,  638,  684, 1811,  638,  638, 1814,  638,  638,  638,
  638,  638,  638,  638,  638,  666,  667,  638,  638,  638,
  638,  638,  668,  669,  670,  671,  393,  483, 1826,  894,
  483,  898,  672,  673,  674,  902,  904,  906,  696,  694,
  675,  695,  395,  697,  692,  483,  676,  511,  681,  693,
  930,  682,  691,  698,  699, 1747,  396,  397,  398,  700,
  202,  203, 1326, 1327,  704, 1329, 1331,  718,  926,  696,
  694,  721,  695, 1336,  697,  241,  709,  726,  728,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  511,  742,
  746,  511, 1349,  753,  411,  412,  692,  241,  747,  751,
 1356,  754,  423,  416,  241,  423,  511, 1163,  511,  756,
  757, 1364,  423, 1366, 1367,  358,  759,  523,  692,  692,
  768,  769,  771,  772,  693,  781,  230,  805,   98,  358,
  806,  978,  812,  978,  978,  485,  807,  358,  241,  275,
  817,  978,  822,  823,  825,  423,  827,  824,  828,  978,
  833,  839,  840,  358,  978,  844,  692,  978,  843,  853,
  855,  693,  856,  861,  862,  692,  918,  230,  863,   99,
  230,  978,  867,  956,  692,  928,  978,  978,  959, 1788,
  978,  100,  957,  959, 1417,  230,  964,  978,  985,  978,
  978,  696,  694,  978,  695,  968,  697,  978,  696,  694,
  978,  695,  978,  697,  978,  969,  101,  358,  979,  978,
  978,  692,  978,  994,  997, 1744,  693, 1013,  692,  959,
  959,  959,  959,  959,  971,  959, 1018,  834,  978,  978,
 1023,  978,  692,  485, 1024, 1026,  784,  959,  959,  959,
  959, 1039, 1479, 1030, 1033,  978, 1040,  978,  978,  978,
 1031, 1041,  978, 1047, 1467,  978,  978,  696,  694, 1059,
  695,  684,  697, 1475,  483,  978,  483, 1053, 1064,  959,
 1480,  959, 1066,  483,  684,  102,  692, 1073, 1090,  978,
 1110,  693,  978,  684, 1078,  103,  483,  714,  972,  696,
  694,  222,  695, 1091,  697, 1148, 1150, 1152,  978,  423,
 1160, 1172,  692,  423,  978,  511, 1186,  692, 1189,  978,
  104, 1192,  693, 1184, 1202, 1205, 1207, 1213,  701, 1229,
  978,  978,  485,  978,  511,  511, 1226,  511, 1240,  978,
  684,  511,  222, 1249,  511,  222, 1746, 1251, 1261,  684,
 1153,  696,  694, 1271,  695, 1278,  697,  511, 1269, 1282,
  222, 1283, 1284,  511,  655, 1285, 1296, 1294,  978,  978,
  105,  978,  696,  694,  483,  695, 1084,  697,  978,  978,
  978,  978,  978, 1303,  978, 1306,   62,  978,  978,  978,
  978,  978, 1307,  684, 1308, 1309,  483,   63, 1315,  106,
  107, 1316,  108,  483,   64,  655, 1320,  684,  655,  109,
  110,  111,  112,  113,  241,  114,  230,   65,  115,  116,
  117,  118,  119,  655,  231, 1321,  729, 1322, 1614, 1615,
 1324, 1325, 1332, 1333, 1334,  511, 1335,  483, 1337,  692,
 1338,  982, 1339, 1340,  693, 1341,  692, 1342,  891,  423,
  959,  693,  959,  959, 1362, 1359, 1368,  511, 1384, 1386,
  959,  232, 1393, 1394,  511, 1390, 1391, 1392,  959, 1402,
 1395, 1405, 1412,  959, 1414,  511,  959,  554,   66, 1407,
 1422, 1426,  982,  982,  982,  982,  982,  982,  982, 1652,
  959, 1427,  959,  511,   67,  959,  959, 1428,  511,  959,
  982,  982,  982,  982, 1434,  692,  959, 1449,  959,  959,
  693,   68,  959, 1429,  241, 1439,  959,  656, 1430,  959,
 1431,  959, 1456,  959, 1460, 1317,   69,  511,  959,  959,
   70,  959,  982,   71,  982, 1461,  241,  692, 1465, 1471,
 1492,   72,  693,  241, 1498, 1499, 1504,  959,  959, 1505,
  959, 1686, 1578, 1522, 1568,  195, 1581, 1580,  656, 1689,
   73,  656, 1582, 1583,  959, 1584,  959,  959,  959, 1585,
 1587,  959, 1588, 1589,  959,  959,  656,  241, 1597,  222,
 1599,  222, 1602, 1606,  959, 1607, 1608, 1610,  222,  692,
 1611, 1612, 1314, 1613,  693, 1702, 1632, 1704,  959, 1616,
 1617,  959, 1343,  696,  694, 1618,  695, 1619,  697, 1620,
  692, 1621, 1622, 1623,  195,  693, 1624,  959, 1625, 1633,
 1634, 1648,  105,  959, 1651, 1654,   52,  130,  959, 1344,
  696,  694, 1659,  695, 1661,  697, 1667,   53, 1653,  959,
  959, 1655,  959, 1669,   54, 1656, 1657, 1670,  959, 1658,
 1660,  106,  107,  423,  108, 1748, 1666, 1690, 1696, 1697,
 1705,  109,  110,  111,  112,  113, 1722,  114, 1706,   55,
  115,  116,  117,  118,  119, 1662, 1730,  959,  959,  222,
  959, 1743,  655, 1751, 1770, 1765, 1772,  959,  959,  959,
  959,  959,  655,  959,  987,  655,  959,  959,  959,  959,
  959,  222, 1767,  982,  655,  982,  982,  679,  222, 1774,
 1779, 1780, 1791,  982, 1785, 1792, 1347,  696,  694, 1808,
  695,  982,  697, 1762,  158, 1812,  982, 1815, 1763,  982,
 1816, 1754, 1819, 1833,   56,  987,  987,  987,  987,  987,
  987,  987,  222,  982, 1830, 1834,    1,  618,  982,  982,
   29,  655,  982,  987,  987,  987,  987,   31,  202,  982,
  655,  982,  982,  176,  746,  982,   57,  364,  270,  982,
   58,  148,  982,  647,  982,  532,  982,  268,  450,   81,
  942,  982,  982,  668,  982,  987,  275,  987,  392, 1348,
  696,  694,  212,  695,  192,  697,  578,  941,  679,  677,
  982,  982,  673,  982,  655, 1350,  696,  694,  675,  695,
  193,  697,  274,  195,  288,  365,  655,  982,  655,  982,
  982,  982,  176,  250,  982,  533,  703,  982,  982,  286,
  702,  222,  320,  287,  195,  656,  704,  982,  328,  337,
  612,  692,  326,  713,  523,  656,  693,   27,  656,   30,
   39,  982, 1373,  524,  982,  195,  135,  656,  465,  534,
  679,  195,  464,  474,  250, 1304,  854,  250,  692,  958,
  982, 1089, 1302,  693,  245, 1300,  982,  535, 1591,  493,
 1290,  982,  250, 1590,  195, 1279,  541,  818,  585,  512,
  527, 1228,  982,  982,  995,  982, 1699, 1291, 1570, 1761,
 1028,  982,  800,  195,  656,  556, 1766, 1576, 1802, 1021,
 1768,  195,  195,  656, 1745, 1416, 1828, 1796,  195,  557,
  493, 1719, 1398,  493, 1220, 1525,  564,  547, 1729,  538,
  982,  982,  804,  982, 1065, 1446,  195,  841,  493,  137,
  982,  982,  982,  982,  982,  505,  982,  983,  737,  982,
  982,  982,  982,  982,  999,  692,  987,  656,  987,  987,
  693,  986, 1056,  288,  982,   84,  987,  741,  981,  656,
 1204,  656,   85,  195,  987, 1807,   86,   87, 1778,  987,
 1695, 1839,  987,  750,  195,   88, 1638,  965,  983,  983,
  983,  983,  983,  983,  983, 1198,  987,   89, 1328, 1411,
    0,  987,  987, 1361,    0,  987,  983,  983,  983,  983,
    0,    0,  987,    0,  987,  987,  180,    0,  987,    0,
    0,  176,  987,    0,    0,  987,    0,  987,  692,  987,
    0,    0,    0,  693,  987,  987,    0,  987,  983,    0,
  983,    0,  176,    0,  692,    0,    0,    0,    0,  693,
    0,    0,    0,  987,  987,    0,  987,  180,    0,    0,
  180,    0,   90,  176,    0,    0,    0,    0,    0,  176,
  987,    0,  987,  987,  987,  180,    0,  987,    0,   91,
  987,  987,  907,  908,  909,  910,  911,  912,  913,  914,
  987,    0,  176,    0,   92,    0,    0,    0,   93,   98,
    0,  250,    0,  250,  987,    0,  516,  987,    0,   94,
  250,  176,    0,    0,    0, 1351,  696,  694,    0,  695,
  176,  697,    0,  987,    0,    0,  176,    0,   95,  987,
    0, 1352,  696,  694,  987,  695,    0,  697,    0,    0,
   99,    0,    0,    0,  176,  987,  987,  516,  987,    0,
  516,    0,  100,    0,  987,    0,    0,  493,    0,  493,
    0,    0,    0,  493,    0,  516,  493,    0,    0, 1353,
  696,  694,    0,  695,    0,  697,    0,  101,    0,  493,
    0,  176,    0,  987,  987,    0,  987,    0,    0,    0,
    0,    0,    0,  987,  987,  987,  987,  987,    0,  987,
  840,  250,  987,  987,  987,  987,  987,    0,    0,  983,
  186,  983,  983,    0,    0,    0,    0,    0,    0,  983,
    0,    0,    0,  250,    0,    0,    0,  983,    0,    0,
  250,    0,  983,    0,    0,  983,    0,    0,    0,    0,
    0,  840,  840,  840,  840,  840,  102,  840,    0,  983,
    0,    0,    0,    0,  983,  983,  103,  493,  983,  840,
  840,  840,  840,    0,  250,  983,    0,  983,  983,  186,
    0,  983,    0,    0,  180,  983,    0,  516,  983,  493,
  983,  104,  983,    0,    0,    0,  493,  983,  983,    0,
  983,    0,    0,  840,    0,  180,    0,    0,    0,  105,
    0,    0,    0,    0,    0,    0,  983,  983,    0,  983,
  696,  694,    0,  695,    0,  697,  180,    0,  516,    0,
  493,  516,  180,  983,    0,  983,  983,  983,  106,  107,
  983,  108,    0,  983,  983,    0,  516,    0,  109,  110,
  111,  112,  113,  983,  114,  180,    0,  115,  116,  117,
  118,  119,    0,    0,  692,    0,    0,  983,    0,  693,
  983, 1354,  696,  694,  180,  695,    0,  697,    0,    0,
  692,    0,    0,  180,    0,  693,  983,    0,    0,  180,
    0,  514,  983,    0,  516,    0,  516,  983,    0,    0,
  516,    0,    0,  516,    0,    0,    0,  180,  983,  983,
    0,  983,    0,    0,    0,    0,  516,  983,  692,  554,
    0,    0,    0,  693,    0,    0,    0,    0,    0,    0,
    0,    0,  514,    0,    0,  514,    0, 1355,  696,  694,
    0,  695,  852,  697,  180,    0,  983,  983,    0,  983,
  514,    0,  514,    0,    0,    0,  983,  983,  983,  983,
  983,    0,  983,    0,    0,  983,  983,  983,  983,  983,
    0,    0,  840,    0,  840,  840,    0,    0,  186,    0,
    0,    0,  840,  852,  852,  852,  852,  852,    0,  852,
  840,    0,    0,    0,  516,  840,    0,    0,  840,  186,
    0,  852,  852,  852,  852,    0,    0, 1493,  696,  694,
    0,  695,  840,  697,    0,    0,  516,  840,  840,    0,
  186,  840,    0,  516,    0,    0,  186,    0,  840,    0,
  840,  840,    0,    0,  840,  852,    0,    0,  840,    0,
 1478,  840,    0,  840,    0,  840,    0,    0,    0,  186,
  840,  840,    0,  840,    0,    0,    0,  516,  692,    0,
    0,    0,    0,  693,    0,  516,    0,  516,  186,  840,
  840,    0,  840,    0,  516,    0,    0,  186,    0,    0,
    0,    0,    0,  186,    0,    0,  840,  516,  840,  840,
  840,    0,  105,  840,    0,    0,  840,  840, 1494,  696,
  694,  186,  695,    0,  697,    0,  840, 1501,  696,  694,
  692,  695,    0,  697,    0,  693,    0,    0,    0,    0,
  840,  106,  107,  840,  108,    0,    0,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,  186,  840,
  115,  116,  117,  118,  119,  840,    0,    0,    0,  514,
  840,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  840,  840,    0,  840,  516,    0,    0,  514,  514,
  840,  514,    0,    0,    0,  514,  692,    0,  514,    0,
    0,  693,    0,    0,    0,    0,    0,  516,    0,    0,
    0,  514,    0,    0,  516,  855,    0,  514,    0,  840,
  840,    0,  840,    0,  852,  187,  852,  852,    0,  840,
  840,  840,  840,  840,  852,  840,    0,    0,  840,  840,
  840,  840,  840,    0,    0,    0,    0,  852,  516,    0,
  852,    0,    0,    0,    0,    0,  855,  855,  855,  855,
  855,    0,  855,    0,  852,    0,  692,    0,    0,  852,
  852,  693,    0,  852,  855,  855,  855,  855,    0,    0,
  852,    0,  852,  852,  187,    0,  852,    0,    0,  514,
  852,    0,    0,  852,    0,  852,    0,  852,    0,    0,
    0,    0,  852,  852,    0,  852,    0,    0,  855,    0,
    0,  514,    0,    0,    0,    0,    0,    0,  514,    0,
    0,  852,  852,    0,  852,    0,    0,  514,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,  852,    0,
  852,  852,  852,    0,    0,  852,    0,  514,  852,  852,
    0,  360,  514,    0,    0,    0,    0,  692,  852,    0,
    0,    0,  693,    0,    0,    0,  692,    0,    0,    0,
    0,  693,  852,    0,  516,  852,    0,    0,    0,    0,
    0,  514,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  852,  360,    0,    0,  360,    0,  852,    0,    0,
    0,    0,  852,    0,    0,    0,    0,    0,    0,    0,
  360,    0,    0,  852,  852,  516,  852,    0,  516, 1468,
 1469,    0,  852,    0,  560, 1473, 1474,    0, 1476, 1477,
    0,    0,    0,  516,    0,  516, 1481, 1482, 1483, 1484,
    0, 1485, 1486, 1487, 1488, 1489, 1490,  856,    0,    0,
    0,  852,  852,    0,  852,    0,    0,    0,    0,    0,
    0,  852,  852,  852,  852,  852,    0,  852,    0,    0,
  852,  852,  852,  852,  852,    0,    0,  855,    0,  855,
  855,    0,    0,  187,    0,    0,    0,  855,  856,  856,
  856,  856,  856,    0,  856,    0,    0,    0,    0,    0,
    0,    0,    0,  855,  187,    0,  856,  856,  856,  856,
    0,    0,    0,    0,    0,    0,    0,  855,    0,    0,
    0,    0,  855,  855,    0,  187,  855,  662,    0,    0,
    0,  187,    0,  855,    0,  855,  855,    0,    0,  855,
  856,    0,    0,  855,    0,    0,  855,    0,  855,    0,
  855,    0,    0,    0,  187,  855,  855,    0,  855,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  662,    0,
    0,  662,    0,  187,  855,  855,    0,  855,    0,    0,
    0,    0,  187,    0,    0,    0,  662,    0,  187,    0,
    0,  855,    0,  855,  855,  855,    0,  105,  855,    0,
    0,  855,  855,    0,    0,    0,  187,    0,    0,    0,
    0,  855,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  855,  106,  107,  855,  108,
    0,    0,  516,    0,    0,    0,  109,  110,  111,  112,
  113,    0,  114,  187,  855,  115,  116,  117,  118,  119,
  855,    0,  516,    0,  516,  855,    0,    0,  516,    0,
    0,  516,    0,    0,    0,    0,  855,  855,    0,  855,
    0,    0,  360,    0,  516,  855,    0,    0,    0,  360,
  516,    0,  360,    0,    0,    0,  360,    0,    0,    0,
    0,    0,    0,    0,  360,    0,    0,    0,    0,    0,
  850,    0,    0,    0,  855,  855,    0,  855,    0,  856,
  360,  856,  856,    0,  855,  855,  855,  855,  855,  856,
  855,    0,    0,  855,  855,  855,  855,  855,    0,    0,
    0,    0,    0,    0,  360,  856,    0,    0,    0,    0,
    0,  850,    0,  850,  850,  850,    0,    0,    0,  856,
    0,    0,  516,    0,  856,  856,    0,    0,  856,  850,
  850,  850,  850,    0,  360,  856,    0,  856,  856,    0,
    0,  856,    0,    0,  516,  856,    0,    0,  856,    0,
  856,  516,  856,    0,    0,    0,    0,  856,  856,    0,
  856,  360,  516,  850,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  360,    0,  360,  856,  856,    0,  856,
  516,    0,    0,    0,    0,  516,    0,    0,    0,    0,
    0,    0,    0,  856,    0,  856,  856,  856,    0,    0,
  856,    0,    0,  856,  856,    0,    0,    0,    0,    0,
    0,    0,    0,  856,  516,  662,    0,    0,  662,    0,
    0,    0,   98,    0,    0,  662,    0,  856,  662,    0,
  856,    0,  662,    0,    0,    0,    0,  662,    0,    0,
  662,    0,    0,    0,    0,    0,  856,    0,    0,    0,
    0,    0,  856,    0,    0,    0,  662,  856,    0,    0,
    0,    0,    0,   99,    0,    0,    0,    0,  856,  856,
    0,  856,    0,    0,    0,  100,    0,  856,    0,    0,
  662,    0,    0,    0,  662,    0,    0,    0,    0,  132,
    0,    0,    0,  662,    0,    0,    0,    0,    0,    0,
  101,    0,  849,    0,    0,    0,  856,  856,    0,  856,
  662,    0,    0,    0,    0,    0,  856,  856,  856,  856,
  856,    0,  856,    0,    0,  856,  856,  856,  856,  856,
    0,    0,  850,    0,  850,  850,    0,  662,    0,    0,
    0,    0,  850,  849,    0,  849,  849,  849,    0,  662,
    0,  662,    0,    0,    0,    0,    0,    0,  850,    0,
    0,  849,  849,  849,  849,    0,    0,    0,    0,  102,
    0,    0,  850,    0,    0,    0,    0,  850,  850,  103,
    0,  850,  659,    0,    0,    0,    0,    0,  850,    0,
  850,  850,    0,    0,  850,  849,    0,    0,  850,    0,
    0,  850,    0,  850,  104,  850,    0,    0,    0,    0,
  850,  850,    0,  850,    0,    0,    0,    0,    0,    0,
    0,    0,  105,  659,    0,    0,  659,    0,    0,  850,
  850,    0,  850,    0,    0,    0,    0,    0,    0,    0,
    0,  659,    0,    0,    0,    0,  850,    0,  850,  850,
  850,  106,  107,  850,  108,    0,  850,  850,    0,    0,
    0,  109,  110,  111,  112,  113,  850,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,    0,    0,
  850,    0,    0,  850,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  850,
    0,    0,    0,    0,    0,  850,    0,    0,    0,    0,
  850,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  850,  850,    0,  850,    0,    0,    0,    0,    0,
  850,    0,    0,  926,  696,  694,    0,  695,    0,  697,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  889,  888,  890,  851,    0,    0,    0,  850,
  850,    0,  850,    0,  849,    0,  849,  849,    0,  850,
  850,  850,  850,  850,  849,  850,    0,    0,  850,  850,
  850,  850,  850,    0,    0,    0,    0,    0,    0,    0,
  849,    0,    0,    0,    0,    0,  851,    0,  851,  851,
  851,    0,    0,    0,  849,    0,    0,    0,    0,  849,
  849,    0,    0,  849,  851,  851,  851,  851,    0,    0,
  849,    0,  849,  849,    0,    0,  849,    0,    0,    0,
  849,    0,    0,  849,    0,  849,    0,  849,    0,    0,
    0,    0,  849,  849,    0,  849,    0,    0,  851,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  849,  849,    0,  849,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  849,    0,
  849,  849,  849,    0,    0,  849,    0,    0,  849,  849,
    0,    0,    0,    0,    0,    0,    0,    0,  849,    0,
  659,    0,    0,  659,    0,    0,    0,    0,    0,    0,
  659,    0,  849,  659,    0,  849,    0,  659,    0,    0,
    0,    0,  659,    0,    0,  659,    0,    0,    0,    0,
    0,  849,    0,    0,    0,    0,    0,  849,    0,    0,
    0,  659,  849,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  849,  849,    0,  849,    0,    0,    0,
    0,    0,  849,    0,  876,  659,    0,    0,    0,  659,
    0,    0,  692,    0,    0,    0,    0,  693,  659,    0,
  877,    0,    0,    0,    0,    0,    0,  854,    0,    0,
    0,  849,  849,    0,  849,  659,    0,    0,    0,    0,
    0,  849,  849,  849,  849,  849,    0,  849,    0,    0,
  849,  849,  849,  849,  849,    0,  878,  851,    0,  851,
  851,    0,  659,    0,    0,  879,    0,  851,  854,    0,
  854,  854,  854,  880,  659,    0,  659,    0,    0,    0,
    0,    0,    0,  851,    0,    0,  854,  854,  854,  854,
    0,    0,  881,    0,  882,    0,    0,  851,    0,  945,
    0,    0,  851,  851,    0,    0,  851,    0,  883,    0,
  884,  885,  886,  851,    0,  851,  851,    0,    0,  851,
  854,  455,    0,  851,    0,    0,  851,    0,  851,    0,
  851,    0,    0,    0,    0,  851,  851,    0,  851,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,  851,  851,    0,  851,    0,    0,
    0,  887,    0,    0,    0,    0,    0,    0,  521,    0,
    0,  851,    0,  851,  851,  851,  106,  107,  851,  108,
    0,  851,  851,    0,    0,    0,  109,  110,  111,  112,
  113,  851,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,    0,    0,  851,    0,    0,  851,  521,
    0,    0,  521,    0,  948,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  851,    0,    0,  521,    0,  521,
  851,  105,    0,    0,    0,  851,    0,    0,    0,  648,
    0,    0,  417,    0,  418,    0,  851,  851,    0,  851,
    0,    0,    0,    0,    0,  851,    0,  420,    0,  521,
  106,  107,  277,  108,    0,    0,    0,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,  851,  851,    0,  851,    0,  854,
    0,  854,  854,    0,  851,  851,  851,  851,  851,  854,
  851,    0,    0,  851,  851,  851,  851,  851,    0,    0,
    0,    0,    0,    0,    0,  854,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  854,
    0,    0,    0,    0,  854,  854,  648,    0,  854,  417,
    0,  418,    0,    0,    0,  854,    0,  854,  854,    0,
    0,  854,    0,    0,  420,  854,    0,    0,  854,  277,
  854,    0,  854,    0,    0,    0,    0,  854,  854,    0,
  854,    0,    0,    0,    0,    0,    0,    0,    0,  105,
    0,    0,    0,    0,    0,    0,  854,  854,    0,  854,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  854,    0,  854,  854,  854,  106,  107,
  854,  108,    0,  854,  854,    0,    0,    0,  109,  110,
  111,  112,  113,  854,  114,    0,  521,  115,  116,  117,
  118,  119,    0,    0,    0,    0,    0,  854,    0,    0,
  854,    0,  521,  459,    0,  521,  521,    0,  521,    0,
    0,    0,  521,    0,    0,  521,  854,    0,    0,    0,
  105,    0,  854,    0,    0,    0,  381,  854,  521,    0,
    0,    0,    0,    0,  521,  382,    0,  521,  854,  854,
    0,  854,    0,    0,    0,    0,    0,  854,  383,  106,
  107,    0,  108,    0,  384,  385,    0,    0,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,  642,    0,    0,  854,  854,    0,  854,
    0,    0,  386,    0,    0,  387,  854,  854,  854,  854,
  854,    0,  854,    0,    0,  854,  854,  854,  854,  854,
    0,    0,    0,    0,    0,    0,  521,    0,    0,    0,
    0,    0,    0,  389,  390,    0,    0,    0,    0,    0,
    0,    0,    0,  381,    0,  391,    0,    0,  521,  392,
    0,    0,  382,    0,    0,  521,    0,  868,    0,  521,
    0,    0,    0,    0,    0,  383,  521,    0,  641,    0,
    0,  384,  385,    0,  632,    0,  632,  632,    0,  632,
    0,    0,    0,    0,  521,    0,    0,    0,    0,  521,
  642,    0,  632,    0,    0,    0,  685,  632,    0,  386,
  644,    0,  387,    0,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,  521,    0,    0,    0,  521,    0,
    0,    0,  395,    0,    0,    0,    0,    0,    0,    0,
  389,  390,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,  391,    0,    0,    0,  392,    0,    0,    0,
  400,    0,    0,    0,  643,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,  560,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,  869,  870,  871,    0,    0,  644,    0,    0,
    0,  560,  560,  393,  560,    0,    0,    0,    0,  394,
    0,  560,  560,  560,  560,  560,    0,  560,    0,  395,
  560,  560,  560,  560,  560,    0,  648,    0,    0,  417,
    0,  418,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,  420,    0,    0,  400,    0,  277,
    0,    0,  945,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  645,  646,  647,    0,
    0,    0,    0,    0,  632,    0,    0,    0,    0,    0,
    0,  632,  527,    0,    0,    0,    0,    0,    0,    0,
  632,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  632,    0,    0,  946,    0,    0,  632,
  632,    0,    0,    0,    0,    0,    0,  632,    0,    0,
    0,    0,  527,  527,    0,    0,  527,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  632,    0,    0,
  632,  527,  947,  527,    0,    0,    0,  948,    0,    0,
    0,    0,    0,  949,    0,    0,    0,    0,  648,    0,
    0,  417,    0,  418,  105,    0,  632,    0,  632,  632,
    0,    0,  950,  527,    0,    0,  420,    0,    0,    0,
  632,  277,    0,    0,  632,    0,    0,    0,    0,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  381,    0,    0,    0,    0,    0,    0,
    0,  632,  382,    0,    0,    0,    0,  632,    0,    0,
    0,    0,    0,    0,    0,  383,    0,  632,    0,    0,
    0,  384,  385,    0,    0,    0,    0,    0,    0,    0,
    0,  632,  632,  632,  632,  632,  632,    0,    0,    0,
  642,    0,    0,    0,    0,  632,    0,    0,    0,  386,
    0,    0,  387,    0,  632,  632,  632,  632,  632,  632,
  632,  632,  632,  632,  632,    0,  632,    0,    0,  632,
  632,    0,  632,  632,  632,  632,  632,  632,  632,  632,
  389,  390,  632,  632,  632,  632,  632,    0,    0,    0,
    0,    0,  391,  819,    0,    0,  392,    0,    0,    0,
  527,    0,    0,    0,  864,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  527,    0,    0,  527,
  527,    0,  527,    0,    0,    0,  527,    0,    0,  527,
    0,    0,    0,    0,  819,    0,    0,  819,    0,    0,
    0,    0,  527,    0,    0,  381,  389,  644,  527,    0,
    0,  527,  819,  393,  382,    0,    0,  648,    0,  394,
  417,    0,  418,    0,    0,    0,    0,  383,    0,  395,
    0,    0,    0,  384,  385,  420,    0,    0,    0,    0,
  277,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,  642,    0,    0,    0,    0,  400,    0,    0,
    0,  386,    0,    0,  387,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
  527,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,  389,  390,  115,  116,  645,  646,  647,    0,
    0,    0,  527,    0,  391,    0,    0,    0,  392,  527,
   76,    0,  399,  527,  479,    0,  643,  417,    0,  418,
  527,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,    0,    0,    0,  277,  527,    0,
    0,  106,  410,  527,  108,    0,    0,    0, 1635,    0,
  413,  414,  415,  111,  112,  113,    0,  114,    0,  644,
  115,  116,  117,  118,  119,  393,    0,    0,  527,    0,
    0,  394,  527, 1636,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,  819,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108, 1637,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,  381,    0,  115,  116,  645,  646,
  647,    0,    0,  382,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  105,  383,    0,    0,    0,
    0,  819,  384,  385,  819,    0,    0,    0,    0,  788,
    0,  819,    0,    0,  819,    0,    0,    0,  819,    0,
    0,  642,    0,  819,  106,  107,  819,  108,    0,    0,
  386,    0,    0,  387,  109,  110,  111,  112,  113,    0,
  114,    0,  819,  115,  116,  117,  118,  119,    0,    0,
  788,    0,    0,  788,    0,    0,    0,    0,    0,    0,
    0,  389,  390,    0, 1105,    0,  819, 1106,  788,  819,
  819,  381,    0,  391,    0,    0,    0,  392,    0,  819,
  382,    0,    0,    0,    0,  864,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,  819,    0,  479,  384,
  385,  417,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,  819,    0,
    0,  277,    0,  819,    0,    0,    0,  386,  644,    0,
  387,    0,    0,    0,  393,  819,    0,  819,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,    0,    0,  389,  390,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
  391,    0,    0,    0,  392,    0,    0,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1107,
    0,  393,    0,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,  420,    0,
    0,  788,    0,  277,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
    0,    0,  115,  116,  117,  118,  119,    0, 1115,    0,
    0, 1106,    0,    0,    0,  381,    0,  788,    0,    0,
  788,    0,    0,    0,  382,    0,    0,  788,    0,    0,
  788,    0,    0,    0,  788,    0,    0,  383,    0,  788,
    0,    0,  788,  384,  385,    0,    0,    0,  280,  479,
    0,    0,  417,    0,  418,    0,    0,    0,  788,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,    0,
    0,  386,  277,    0,  387,    0,    0,  280,  280,    0,
  280,    0,  788,    0,    0,  788,  788,  280,  280,  280,
  280,  280,    0,  280,    0,  788,  280,  280,  280,  280,
  280,    0,  389,  390,    0,    0,    0,    0,    0,    0,
    0,    0,  788,    0,  391,    0,    0,    0,  392,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  788,    0,    0,    0,    0,  788,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  788,    0,  788,    0,    0,    0,    0,    0,    0,
 1119,    0,    0, 1106,    0,    0,    0,  381,    0,    0,
    0,    0,    0, 1107,    0,  393,  382,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,  383,
    0,  395,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,  386,    0,    0,  387,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,  389,  390,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  391,    0,    0, 1127,
  392,    0, 1106,    0,    0,    0,  381,    0,    0,    0,
    0,    0,    0,    0,    0,  382,    0,    0,    0,    0,
    0,    0,    0,    0,  696,  694,    0,  695,  383,  697,
    0,    0,    0,  479,  384,  385,  417,    0,  418,    0,
    0,    0,  889,  888,  890,    0,    0,    0,    0,    0,
    0,  420,    0,    0,    0, 1107,  277,  393,    0,    0,
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
    0,    0,    0,    0, 1107,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  479,    0,    0,  417,    0,
  418,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,  420,    0,    0,    0,    0,  277,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,    0, 1130,  876,    0, 1106,    0,    0,    0,
  381,    0,  692,    0,    0,    0,    0,  693,    0,  382,
  877,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
    0,    0,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,  878,    0,    0,    0,
    0,    0,  420,    0,    0,  879,  386,  277,    0,  387,
    0,    0,    0,  880,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  881,    0,  882,    0,    0,  389,  390,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  883,  391,
  884,  885,  886,  392,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1135,    0,    0, 1106,    0,
    0,  887,  381,    0,    0,    0,    0,    0, 1107,    0,
  393,  382,    0,    0,    0,    0,  394,    0,    0,    0,
    0,    0,    0,    0,  383,    0,  395,    0,    0,    0,
  384,  385,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,  386,    0,
    0,  387,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,  389,
  390,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,  391,    0,    0, 1138,  392,    0, 1106,    0,    0,
    0,  381,    0,    0,  789,    0,    0,    0,    0,    0,
  382,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,    0,    0,  479,  384,
  385,  417,    0,  418,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  789,  420,    0,  789,    0,
 1107,  277,  393,    0,    0,    0,    0,  386,  394,    0,
  387,    0,    0,  789,    0,    0,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,  389,  390,
    0,    0,    0,    0,    0,    0,  400,    0,    0,    0,
  391,    0,    0,    0,  392,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1107,
    0,  393,    0,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
    0,    0,  115,  116,  117,  118,  119,    0, 1141,    0,
    0, 1106,    0,    0,    0,  381,    0,    0,  479,    0,
  484,  417,    0,  418,  382,    0,  789,    0,    0,    0,
    0,    0,    1,    0,    0,    0,  420,  383,    0,    0,
    0,  277,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    2,    0,    0,    0,    0,    0,    0,
    0,    3,    0,    0,    0,    0,    0,    0,    4,    0,
    5,  386,    6,    0,  387,    0,    0,    7,    0,    0,
    0,    0,  789,    0,    8,  789,    0,    0,    0,    0,
    0,    0,  789,    0,    0,  789,    0,    0,    9,  789,
    0,    0,  389,  390,  789,    0,    0,  789,    0,   10,
    0,    0,    0,    0,  391,    0,  479,    0,  392,  417,
    0,  418,    0,  789,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  420,    0,    0,    0,    0,  277,
    0,    0,    0,    0,    0,    0,    0,  789,    0,    0,
  789,  789,    0,    0,    0,    0,    0,    0,    0,    0,
  789,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1107,    0,  393,   11,  789,    0,    0,
    0,  394,    0,   12,    0,    0,   13,    0,   14,   15,
    0,  395,    0,    0,    0,    0,    0,    0,    0,  789,
    0,    0,    0,    0,  789,  396,  397,  398,  399,  202,
  203,    0,    0,   16,    0,    0,  789,  586,  789,  400,
    0,  587,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,  588,
  108,    0,  589,  411,  412,  381,  413,  414,  415,  111,
  112,  113,  416,  114,  382,    0,  115,  116,  117,  118,
  119,    0,   17,    0,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,   18,   19,    0,
    0,    0,    0,  590,    0,    0,    0,    0,    0,  591,
  592,  593,    0,  594,  595,  596,    0,  597,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  598,    0,  599,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  388,    0,  389,  390,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  381,  391,    0,    0,    0,  392,    0,
  600,    0,  382,    0,    0,  953,    0,  953,  953,    0,
  953,    0,    0,    0,    0,  383,    0,  601,    0,    0,
    0,  384,  385,  953,    0,    0,    0,    0,  953,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  602,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,  393,    0,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  603,  604,    0,    0,  388,    0,
  389,  390,    0,    0,    0,  396,  397,  398,  399,  202,
  203,  605,  391,    0,    0,    0,  392,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,  685,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,    0,    0,    0,  638,    0,  394,
  638,    0,  638,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,  220,    0,  638,    0,  220,    0,    0,
  638,    0,    0,  396,  397,  398,  399,  202,  203,    0,
    0,  220,    0,    0,    0,  220,    0,  400,  220,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,  953,    0,  115,  116,  117,  118,  119,  220,
    0,  953,    0,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,  953,  544,    0,    0,    0,    0,
  953,  953,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  220,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  953,    0,
    0,  953,    0,    0,    0,    0,  544,    0,    0,  544,
    0,    0,    0,    0,    0,    0,  220,    0,    0,    0,
    0,   80,    0,    0,  544,    0,  544,  953,    0,  953,
  953,    0,    0,  220,    0,    0,    0,    0,    0,    0,
    0,  953,    0,    0,    0,  953,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  220,  544,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  638,    0,    0,
  220,  220,    0,    0,  638,    0,    0,    0,    0,    0,
    0,    0,  953,  638,    0,    0,  419,  220,  953,  417,
    0,  418,    0,    0,    0,    0,  638,    0,  953,    0,
    0,    0,  638,  638,  420,    0,    0,    0,    0,  277,
  638,    0,  953,  953,  953,  953,  953,  953,    0,    0,
    0,    0,    0,    0,    0,    0,  953,    0,    0,    0,
  638,    0,    0,  638,    0,  953,  953,  953,  953,  953,
  953,  953,  953,  953,  953,  953,    0,  953,    0,    0,
  953,  953,    0,  953,  953,  953,  953,  953,  953,  953,
  953,  638,  638,  953,  953,  953,  953,  953,    0,    0,
    0,    0,    0,  638,    0,    0,    0,  638,    0,    0,
    0,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  544,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  544,
    0,    0,    0,  544,    0,  544,    0,    0,    0,  544,
    0,    0,  544,    0,  638,    0,    0,    0,    0,    0,
  638,    0,    0,    0,    0,  544,    0,    0,    0,    0,
  638,  544,    0,    0,  544,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  638,  638,  638,  638,  638,  638,
    0,    0,    0,    0,    0,    0,    0,    0,  638,    0,
    0,    0,    0,    0,    0,    0,    0,  638,  638,  638,
  638,  638,  638,  638,  638,  638,  638,  638,    0,  638,
    0,    0,  638,  638,    0,  638,  638,  638,  638,  638,
  638,  638,  638,  381,    0,  638,  638,  638,  638,  638,
    0,    0,  382,  544,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  383,    0,    0,    0,    0,
    0,  384,  385,    0,    0,  544,    0,    0,    0,    0,
    0,    0,  544,    0,    0,    0,  544,    0,    0,    0,
    0,    0,    0,  544,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,    0,    0,    0,
    0,  544,    0,    0,    0,    0,  544,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  388,    0,
  389,  390,    0,    0,    0,    0,    0,    0,    0,    0,
  381,  544,  391,    0,    0,  544,  392,    0,    0,  382,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,  387,
    0,    0,    0,  393,    0,    0,    0,    0,    0,  394,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,  391,
    0,    0,    0,  392,    0,    0,    0,  400,  534,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,  534,
    0,    0,  534,    0,    0,    0,    0,    0,    0,    0,
  393,    0,    0,    0,    0,    0,  394,  534,    0,  534,
    0,    0,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,  677,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,  534,
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
    0,    0,    0,    0,    0,    0,  534,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  534,    0,    0,    0,  534,    0,  534,  685,
    0,    0,  534,    0,    0,  534,    0,    0,    0,  393,
    0,    0,    0,    0,    0,  394,    0,    0,  534,    0,
    0,    0,    0,    0,  534,  395,    0,  534,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  381,    0,
  115,  116,  117,  118,  119,    0,  534,  382,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,  534,    0,
    0,    0,  764,    0,    0,  534,    0,    0,    0,  534,
    0,    0,    0,    0,    0,    0,  534,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,  387,    0,    0,
    0,    0,    0,    0,  534,    0,    0,    0,    0,  534,
    0,    0,    0,  764,    0,    0,  764,    0,  381,    0,
    0,    0,    0,  388,    0,  389,  390,  382,    0,    0,
  479,  764,    0,  417,  534,  418,    0,  391,  534,    0,
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
  764,    0,  277,  764,    0,    0,    0,    0,    0,    0,
  764,    0,    0,  764,    0,    0,    0,  764,    0,    0,
    0,    0,  764,  386,    0,  764,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  764,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,  479,
    0,    0,  417,    0,  418,  764,  391,    0,  764,  764,
  392,    0,    0,    0,    0,    0,    0,  420,  764,    0,
    0,    0,  277,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  764,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  764,    0,    0,
    0,    0,  764,    0,    0,    0,    0,  393,    0,    0,
    0,    0,    0,  394,  764,    0,  764,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  857,    0,    0,
    0,  587,    0,    0, 1133,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,  858,    0,    0,    0,  588,
    0,  400,  589,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,  381,    0,  115,  116,
  117,  118,  119,  590,    0,  382,    0,    0,    0,  591,
  592,  593,    0,  594,  595,  596,    0,  597,  383,    0,
    0,    0,    0,    0,  384,  385,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  598,    0,  599,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  600,    0,    0,    0,    0,    0,  381,    0,    0,    0,
    0,    0,    0,  389,  390,  382,    0,  601,  479,    0,
    0,  417,    0,  418,    0,  391,    0,    0,  383,  392,
    0,    0,    0,    0,  384,  385,  420, 1701,    0,  602,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,  603,  604, 1317,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,  605,  394,  389,  390,    0,    0,    0,    0,    0,
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
    0,    0,  389,  390,    0,    0,    0,  953,    0,    0,
  953,    0,  953,    0,  391,    0,    0,    0,  392,    0,
    0,    0,  220,    0,    0,  953,  220,    0,    0,    0,
  953,    0,    0,    0,    0,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,  518,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
  220,    0,    0,  813,  953,    0,    0,    0,    0,    0,
    0,  389,  390,  953,    0,    0,  220,  518,    0,    0,
  518,    0,    0,  391,    0,    0,  953,  392,    0,    0,
    0,    0,  953,  953,    0,  518,    0,  518,    0,    0,
    0,    0,    0,    0,  813,    0,    0,  813,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  953,    0,  813,  953,    0,    0,    0,  518,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,  953,  953,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,  953,    0,    0,    0,  953,    0,    0,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,  953,  115,  116,  117,  118,  119,
  953,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  953,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  953,  953,  953,  953,  953,  953,
    0,    0,    0,    0,    0,    0,    0,    0,  953,    0,
    0,    0,    0,    0,    0,    0,    0,  953,  953,  953,
  953,  953,  953,  953,  953,  953,  953,  953,  829,  953,
    0,    0,  953,  953,  518,  953,  953,  953,  953,  953,
  953,  953,  953,    0,    0,  953,  953,  953,  953,  953,
  518,    0,    0,    0,  518,    0,  518,    0,    0,    0,
  518,    0,    0,  518,    0,  813,    0,    0,    0,  829,
  991,  991,  829,  991,  991,  991,  518,    0,    0,    0,
    0,    0,  518,    0,    0,  518,    0,  829,  991,  991,
  991,  133,    0,    0,    0,  133,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,  133,    0,    0,  991,
    0,  813,    0,    0,  813,    0,    0,    0,    0,    0,
    0,  813,    0,    0,  813,    0,    0,    0,  813,    0,
    0,    0,    0,  813,    0,    0,  813,    0,    0,    0,
    0,    0,    0,    0,  518,    0,    0,  133,    0,    0,
  830,    0,  813,  133,  133,  133,    0,  133,  133,  133,
    0,  133,    0,    0,    0,    0,  518,    0,    0,    0,
    0,    0,    0,  518,    0,    0,  813,  518,    0,  813,
  813,  133,    0,  133,  518,    0,    0,    0,    0,  813,
    0,  830,  992,  992,  830,  992,  992,  992,    0,    0,
    0,    0,  518,    0,    0,    0,  813,  518,    0,  830,
  992,  992,  992,    0,  133,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  813,    0,
    0,  133,  518,  813,  792,    0,  518,    0,    0,    0,
    0,  992,    0,    0,    0,  813,    0,  813,    0,    0,
    0,    0,    0,  133,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  792,    0,    0,  792,    0,
  829,    0,    0,    0,    0,    0,    0,    0,  133,  133,
  991,    0,    0,  792,    0,    0,    0,    0,  991,    0,
    0,    0,    0,  991,    0,  133,  991,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  831,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  829,    0,    0,  829,
    0,    0,  991,    0,    0,    0,  829,    0,    0,  829,
    0,  991,    0,  829,    0,    0,    0,    0,  829,  991,
    0,  829,    0,    0,    0,    0,  831,  993,  993,  831,
  993,  993,  993,    0,    0,    0,    0,  829,  991,    0,
  991,    0,    0,    0,  831,  993,  993,  993,    0,    0,
    0,    0,    0,    0,  991,    0,  991,  991,  991,    0,
    0,  829,  790,    0,  829,  829,    0,    0,  791,    0,
    0,    0,  830,    0,  829,    0,  993,    0,    0,    0,
    0,    0,  992,    0,    0,    0,    0,    0,    0,    0,
  992,  829,    0,    0,    0,  992,    0,    0,  992,    0,
    0,    0,    0,  790,    0,    0,  790,  991,    0,  791,
    0,    0,  791,  829,    0,    0,    0,    0,  829,    0,
    0,  790,    0,    0,    0,    0,    0,  791,  830,    0,
  829,  830,  829,    0,  992,    0,    0,    0,  830,    0,
    0,  830,    0,  992,    0,  830,    0,    0,    0,    0,
  830,  992,    0,  830,    0,    0,  792,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  830,
  992,    0,  992,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  992,    0,  992,  992,
  992,    0,    0,  830,    0,    0,  830,  830,    0,    0,
  821,    0,    0,    0,    0,    0,  830,    0,    0,    0,
    0,    0,  792,    0,    0,  792,    0,    0,    0,    0,
    0,    0,  792,  830,    0,  792,    0,    0,    0,  792,
    0,    0,    0,    0,  792,    0,    0,  792,    0,  992,
    0,  821,  785,    0,  821,  830,    0,  831,    0,    0,
  830,    0,    0,  792,    0,    0,    0,  993,    0,  821,
    0,    0,  830,    0,  830,  993,    0,    0,    0,    0,
  993,    0,    0,  993,    0,    0,    0,  792,    0,    0,
  792,  792,    0,  785,    0,    0,  785,    0,    0,    0,
  792,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  785,    0,  831,    0,    0,  831,  792,    0,  993,
    0,    0,    0,  831,    0,    0,  831,    0,  993,    0,
  831,    0,    0,    0,  790,  831,  993,    0,  831,  792,
  791,    0,  786,    0,  792,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  831,  993,  792,  993,  792,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  993,    0,  993,  993,  993,    0,    0,  831,    0,
    0,  831,  831,  786,  787,    0,  786,    0,    0,    0,
  790,  831,    0,  790,    0,    0,  791,    0,    0,  791,
  790,  786,    0,  790,    0,    0,  791,  790,  831,  791,
    0,    0,  790,  791,    0,  790,  820,    0,  791,    0,
    0,  791,    0,    0,  993,  787,    0,    0,  787,    0,
  831,  790,    0,    0,    0,  831,    0,  791,    0,    0,
    0,    0,    0,  787,    0,    0,    0,  831,    0,  831,
    0,    0,    0,    0,    0,  790,    0,  820,  790,  790,
  820,  791,    0,    0,  791,  791,    0,    0,  790,    0,
    0,    0,  821,    0,  791,  820,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  790,    0,    0,    0,    0,
    0,  791,    0,    0,    0,    0,    0,    0,    0,  814,
    0,    0,    0,    0,    0,    0,    0,  790,    0,    0,
    0,    0,  790,  791,  785,    0,    0,    0,  791,  822,
    0,    0,    0,    0,  790,    0,  790,    0,  821,    0,
  791,  821,  791,    0,    0,    0,    0,    0,  821,    0,
  814,  821,    0,  814,    0,  821,    0,    0,    0,    0,
  821,    0,    0,  821,    0,    0,    0,    0,  814,    0,
  822,    0,    0,  822,    0,    0,    0,    0,    0,  821,
  785,    0,    0,  785,    0,    0,    0,    0,  822,    0,
  785,    0,    0,  785,    0,    0,    0,  785,    0,    0,
    0,    0,  785,  821,    0,  785,  821,  821,    0,    0,
    0,    0,    0,    0,  786,    0,  821,    0,    0,    0,
    0,  785,    0,    0,    0,  823,    0,    0,    0,    0,
    0,    0,    0,  821,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  785,    0,    0,  785,  785,
    0,    0,    0,    0,    0,  821,  787,    0,  785,  811,
  821,    0,    0,    0,    0,    0,  823,    0,    0,  823,
  786,    0,  821,  786,  821,  785,    0,    0,    0,    0,
  786,    0,    0,  786,  823,    0,    0,  786,  820,    0,
    0,    0,  786,    0,    0,  786,    0,  785,    0,    0,
  811,    0,  785,  811,    0,    0,    0,    0,    0,    0,
    0,  786,  787,    0,  785,  787,  785,    0,  811,    0,
    0,    0,  787,  815,    0,  787,    0,    0,    0,  787,
    0,    0,    0,    0,  787,  786,    0,  787,  786,  786,
    0,    0,    0,    0,  820,    0,    0,  820,  786,    0,
    0,    0,    0,  787,  820,  824,    0,  820,    0,    0,
    0,  820,    0,    0,  815,  786,  820,  815,    0,  820,
    0,  814,    0,    0,    0,    0,    0,  787,    0,    0,
  787,  787,  815,    0,    0,  820,    0,  786,    0,    0,
  787,  822,  786,    0,    0,    0,  824,  812,    0,  824,
    0,    0,    0,    0,  786,    0,  786,  787,    0,  820,
    0,    0,  820,  820,  824,    0,    0,    0,    0,    0,
    0,    0,  820,    0,    0,    0,    0,  814,    0,  787,
  814,    0,    0,    0,  787,    0,    0,  814,  812,  820,
  814,  812,    0,    0,  814,    0,  787,  822,  787,  814,
  822,    0,  814,    0,    0,    0,  812,  822,    0,    0,
  822,  820,    0,    0,  822,    0,  820,    0,  814,  822,
    0,    0,  822,    0,    0,    0,    0,    0,  820,    0,
  820,    0,    0,    0,    0,    0,    0,  823,  822,    0,
    0,    0,  814,  816,    0,  814,  814,    0,    0,    0,
    0,    0,    0,    0,    0,  814,    0,    0,    0,    0,
    0,    0,  822,    0,    0,  822,  822,    0,    0,    0,
    0,  811,  814,    0,    0,  822,    0,    0,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  816,    0,    0,
    0,    0,  822,  823,  814,    0,  823,    0,    0,  814,
    0,    0,  816,  823,    0,    0,  823,    0,    0,    0,
  823,  814,    0,  814,  822,  823,    0,    0,  823,  822,
    0,    0,    0,    0,    0,    0,    0,  811,    0,    0,
  811,  822,    0,  822,  823,  815,    0,  811,    0,    0,
  811,    0,    0,    0,  811,    0,    0,    0,    0,  811,
    0,    0,  811,    0,    0,    0,    0,    0,  823,    0,
    0,  823,  823,    0,    0,    0,    0,  824,  811,    0,
    0,  823,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  823,    0,
    0,  815,  811,    0,  815,  811,  811,    0,    0,    0,
    0,  815,    0,    0,  815,  811,    0,    0,  815,  812,
  823,    0,    0,  815,    0,  823,  815,    0,    0,    0,
    0,    0,  811,  824,    0,    0,  824,  823,    0,  823,
    0,    0,  815,  824,    0,    0,  824,    0,    0,    0,
  824,    0,    0,    0,  811,  824,    0,    0,  824,  811,
    0,    0,    0,    0,    0,    0,  815,    0,    0,  815,
  815,  811,    0,  811,  824,  812,    0,    0,  812,  815,
  318,    0,    0,    0,    0,  812,    0,    0,  812,    0,
    0,    0,  812,    0,    0,    0,  815,  812,  824,    0,
  812,  824,  824,    0,    0,    0,    0,    0,    0,    0,
  318,  824,    0,    0,    0,  816,  812,  318,  815,    0,
    0,  318,  318,  815,  318,    0,    0,  318,  824,    0,
    0,    0,    0,    0,    0,  815,    0,  815,    0,    0,
  812,  318,    0,  812,  812,    0,    0,  318,    0,    0,
  824,    0,    0,  812,    0,  824,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  824,    0,  824,
  812,  816,    0,    0,  816,    0, 1414,    0,    0,    0,
    0,  816,    0,    0,  816,    0,    0,    0,  816,    0,
    0,    0,  812,  816,    0,    0,  816,  812,    0,    0,
    0,    0,    0, 1414,    0,    0,    6,  318,    0,  812,
    0,  812,  816,    0,    0,    0,    0, 1533, 1534,    0,
 1535,    0,    0, 1536,    0,    0,  318,    0,    0,    0,
    0,    0,    0,    6,    0,    0,  816, 1537,    0,  816,
  816,  318,    0, 1538, 1533, 1534,    0, 1535,    0,  816,
 1536,  318,    0,    0,    0,    0,    0,    0,    0,    0,
  318,    0,  318,    0, 1537,    0,  816,    0,    0,  318,
 1538,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  816,    0,
    0,    0,    0,  816,    0,    0,    0,    0,  318,  318,
    0,  318,    0, 1539,    0,  816,    0,  816,  318,  318,
  318,  318,  318,    0,  318,    0,    0,  318,  318,  318,
  318,  318,   14,    0,    0,    0,    0,    0,    0,    0,
 1539,    0,    0,    0,    0,    0,    0, 1540,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,   14,
    0,    0,    0,    0,    0,    0, 1662,    0, 1541,    0,
    0,    0,    0,    0, 1540,  105,    0,    0,    0,    0,
    0,    0,    0,    0,   16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1541,    0,  586,    0,    0,
    0,  587,  105,    0, 1542,  107,    0,  108,  784,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,  588,
  114,    0,  589,  115, 1543,  117,  118,  119,    0,    0,
    0, 1542,  107,    0,  108,  586,    0,    0,    0,  587,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115, 1543,  117,  118,  119,    0,    0,  588,    0,    0,
  589,    0,    0,  590,    0,    0,    0,    0,    0,  591,
  592,  593,    0,  594,  595,  596,    0,  597,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  598,    0,  599,
    0,  590,    0,    0,    0,    0,    0,  591,  592,  593,
    0,  594,  595,  596,    0,  597,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  600,    0,    0,    0,    0,  598,    0,  599,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  601,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  600,  602,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  105,  601,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  603,  604,    0,  602,    0,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,  605,  105,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,  603,  604,    0,    0,    0,    0,    0,    0,
    0,  106,  107,    0,  108,    0,    0,    0,    0,  605,
    0,  109,  110,  111,  112,  113,  105,  114,    0,    0,
  115,  116,  117,  118,  119,  162,  163,  164,  165,  166,
  167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
  177,    0,    0,  178,  179,  106,  107,    0,  108,    0,
    0,  180,  181,    0,    0,  109,  110,  111,  112,  113,
  182,  114,  183,  446,  115,  116,  117,  118,  119,    0,
    0,    0,    0,  446,  446,  446,  446,  446,  446,  446,
  446,  446,  446,  446,  446,  446,  446,  446,    0,    0,
  446,  446,  446,  446,    0,  446,    0,    0,  446,  446,
    0,    0,  446,  446,  446,  446,  446,  446,  446,  446,
    0,  446,  446,  446,  446,  446,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  104,   18,    0,
  254,   62,  138,  139,   63,   56,  509,  138,  139,  285,
  700,  359,  355,  351,   85,  359,   67,   63,   57,   73,
   94,  330,  149,  328,  419,   89,  555,  665,  467,  516,
   69,  262,   70,   53,   54,   55,   56,   57,   58,   90,
  271,   95,  761,   63,   64,   65,   97,   67, 1026,   69,
   70,  492,   72,   73,  364,   93,   68,  747, 1248, 1019,
   88,   81,   83,   83,   84,   85, 1026,   87,   88,   89,
   90,   92,   92,   93,   94,   95,  685,   97,  152,   91,
  925,  754,  323,  129,  546,  131,  306,  549, 1019,  677,
  376,  130,  279,  555, 1019, 1026, 1019,    0,    0,  150,
 1019, 1026,   44, 1026,    0,  714,  718, 1026,  781,  129,
  130,  131,  151,  751,  631,  143,   45,  790,  963,  226,
  851,  820,    0,  143,    0,    0,  187,   44,    0,  149,
  150,  151,  152,    0,    0, 1027,  156,   41,   41,   41,
  638,   44,   44, 1077,   44,   41, 1038,   44,   44,    0,
   41,  212,  156,  793,   41,    0,   59,   59, 1191,   44,
  473,    0,   42,   59,   45,  419,   41,   40, 1060,   44,
   40,  844,   44,    0,   41,   41,    0,   44,   44,  306,
   45,   59,  525,   59,   59, 1054,   41,   59,   44,   41,
   44,  973,   59,   59,  532,  533,  534,  535,    0,   40,
   63,  511,   41,  699,   40,    0,  313,  555,   59,   44,
  256,  555,   40,    0,   59,  643,  159,   41,  274,    0,
   59,  260,   63, 1454,  727,    0,  290,   63,  459,    0,
  269,   91,   59, 1078, 1296,   59,  256,   59,  581,    0,
  260,    0,  262,  263,  355,   45,   41,    0,   44,  269,
   44,  271,   44,  558,   41,  294,   61,   59,  260,  264,
  306,   40,  260,   63,   59, 1628,  338, 1630,    0,  262,
  290,    0,   59,   12,  294,  356,    0,  301,   59,  262,
  269,   12,   41,  329,   59,  301,  402,  319,   59,  258,
  336,  352,  338,  354,  340,    0, 1274,  343,   59,    0,
   59,  377,  607,  891,  892,  257,   59,  368,  372,  329,
  361,  277,   41, 1568, 1274,   44,  336,  268,  338,  273,
  340,  994, 1685,  343,  362,  320,  964,   59,   16,    0,
   59,  370,  405,  352,  922,   59,   41,  306,  455,   41,
  467,  361,  362, 1274,  956,  807,  403, 1406,  429, 1274,
  370, 1274,  372,  405,   59, 1274,  968,  969,   59,  378,
   44,    0,  376,  420, 1037,    0,   41,  320,  400,   44,
  442, 1263,  448,    0,  317,    0,  278, 1632,  280,  345,
  260,  492,  334,  456, 1574,  287,  340,  828,   59,  665,
 1030, 1031,  508,  386, 1453,   83, 1458,  136,  264,  217,
  420,  757,   41,  386,  456,  136,  411,  458,  721,  441,
  466, 1280, 1775,  258,  365,  417,  455,  277,  442,  417,
   59,  467,  473,  303,   59, 1207,  442,  466,  682, 1394,
 1018,  470,   59,  318,   59,  455,  475,  573,  458,  459,
  868,  436, 1082,  318,  698, 1366,  466, 1678,  304,  418,
  470,  318,  728,  473,  320,  475,  328,  309,  478,  331,
 1381,  306,    0,  318,  769,  340,  320,  772,  264,  530,
  490,  343, 1505,  340,  478,  751,  378,    0,  709,  318,
 1670,  786,  386,    0,    0,   41,  475,  383,   44,  328,
  269,  309,  331, 1192,  318,  300,  387,  258,  400,  386,
  273,  340,   41,   41, 1223,  407,   44,  331,  813,  555,
 1098, 1099,  454, 1101, 1102,  576,  340, 1451, 1410,  436,
   40,   59,  961, 1415,  394, 1417,  320,  768,  748,  458,
 1072,  318,  842, 1723,  554,  555,   59,  724,  340,  441,
  560,  328,   59,   59,  331,  306,  855,  328,  387,  278,
  331,  280,  452,  340, 1071, 1286,  485,  396,  287,  318,
 1452,  458,  574,  387,  440,  340,  627,  628,  440,  340,
 1269, 1761,  396,  418,  447,  861,  454,  458,  454,  454,
  452,  340,  454,  410,  630,  436,  452,  454,  410, 1087,
 1202,  383,  387,  458,  460,  458, 1465,  733,  501,  501,
  387,  440,    0,  431,  328,  501,    0,  331,  340,  396,
  630, 1393, 1285,  452, 1110,  454,  440,  458,    0,  454,
  304,  748,  458,  489,  490, 1470,  492,  502,  764,  765,
  454,    0, 1524,  499,  500,  501,  502,  503, 1530,  505,
  668, 1229,  508,  509,  510,  511,  512,   41,  668,  378,
   44,  718,  454,  440,  383,  701,  409,  418,  458,  440,
  331,   59,   40, 1251,   41,   59,  367,  454, 1006, 1634,
  721,  400,  692,  454,  257,   44, 1218,   59,  407,  454,
  700,  701,  387,  454,  704,  387, 1405, 1579,  964,  709,
   59,  396,  331,  318,  396,  454,  417,    0,  718,  760,
  761,  721,  748,  742, 1596,  744,  726,  753, 1317,  729,
  402,  757,  441,  348,  945,  340,  440,  948,  274, 1024,
 1698,  348,  742,  444,  744, 1394, 1082,  747,  452,   41,
  454, 1036,  778,  753,  552, 1073,  411,  757, 1698, 1246,
  278,    0,  280, 1019, 1382,   45,  283,    0,  387,  287,
 1026,  334, 1195,  309, 1059,  767,   59,  396,  778,  262,
  806,  807, 1249,  343,  284,   41,  506, 1698, 1689,  440,
  313,    0,  501, 1698, 1394, 1698, 1085, 1172, 1394, 1698,
 1735, 1736,    0,  454,    0, 1677,  806,  807,  368,  328,
   59,  286,  331,  805, 1686,  318,  409,    0,  812,  819,
   59,   41,  318,  621,  843,  819,   59, 1395,    0,  819,
 1205,  825,  328,  643,  853,  331, 1345,  340,  648,    0,
    0,   40,    0,  843,  340,  342,  838,  839,  840,  454,
   59, 1272,  325,  853,  961, 1727,   44, 1278,    0, 1148,
  378,   59,  862,   59,  260,    0,  426,  861,  387,   41,
 1293, 1184,   44, 1186,    0,  411,   59,  396,  307,  308,
  377,  259,  400,  301,  387,   42,  264,   59, 1823,  407,
   47,  387,  270,  396,  417,  949,    0, 1241,   59,   59,
  396,   59,    0,  386,  278,   93,  280,  303,  405,  956,
 1432,  408,  262,  287,  348,  946,  950,   59,    0,   41,
  338,  968,  439,  441,   59,  307,  308,  424, 1450,  947,
  466,  259, 1419,   59,    0,  961,   41,  440, 1172, 1195,
   41, 1057,  270, 1287,  440,  945,  946,  947,  948,  949,
  950,  454,  435,  450,    0,   59,  956,  430,  454,  397,
  460,   59,  493, 1633,  437,  261,   40, 1290,  968,  969,
  262, 1205,  503,  973,  331,   41, 1261,   59,   44,  328,
  342,  264,  331,  501,   58, 1634,  461,  462, 1162,  489,
  490, 1165,  492,   59,  343,   61, 1776, 1777,  436,  499,
  500,  501,  502,  503,  378,  505,  262, 1292,  508,  509,
  510,  511,  512,   59,    0,  377,  405, 1345, 1274,  397,
 1503, 1345,  451, 1506,  442,  264,  400,  461,  462,   44,
  387,  264,  470,  407, 1634,  424,  386, 1293, 1634,  396,
  387,    0,  262,  405,   59,  280,  408, 1047,    0, 1075,
  285,    0, 1832, 1397,  864,  264, 1082, 1051,  868,   44,
    0, 1051,  424, 1631, 1064,    0,  264,  441,  264,  451,
    0, 1384,  377,   59, 1066, 1075,    0, 1691, 1692, 1190,
 1191, 1192, 1082, 1651, 1402,  435, 1654,  321,  450,    0,
 1532,  440, 1084,  440,  386,  278,    0,  280, 1090, 1206,
   59,  335,  460,  452,  264,  454,  264,   59,   93, 1225,
   59, 1227,  458,  418,  397, 1598,   40,   41, 1732,   59,
   44,  300,  264,  280,   59,  359, 1382,  501,  285,   59,
  386,  489,  490,   44,  492,   59, 1421,   61,  264,    0,
  262,  499,  500,  501,  502,  503,  318,  505,   59,  321,
  508,  509,  510,  511,  512,   59,  328,  262,  397,  331,
  465,  262,  280,  335,  397,  458,  386,   91,  340, 1161,
  288,  343, 1288,  352,  278, 1460,  280, 1680,  458,  413,
  460,  342, 1223,  318, 1184, 1359, 1186,  359,  397, 1363,
 1804,    0,  258,  322,    0,  378, 1190, 1191,   59,  397,
    0,  397, 1202,  260,  287,  340, 1206, 1207,  274,  489,
  490,  383,  492,    0,  254,  387,  377,  300,  264,  499,
  500,  501,  502,  503,  396,  505,  263, 1730,  508,  509,
  510,  511,  512, 1394,  300,   41,  276,  397,    0,  397,
  306,  413, 1586,  309,  405,  285,  303,  408, 1592,  348,
   59, 1734,  387,   59, 1822,    0,    0,    0,  357,   59,
  342,  460, 1303,  424,  386,    0, 1307,  259,  440, 1772,
  322,  397,   59, 1267,  283,  312,  316, 1267,  270,   41,
  452,  386,  454,   44,   42,  386,  323, 1394,  460,  450,
  489,  490,  278,  492,  280,  377,   41,   59,    0,   44,
  499,  500,  501,  502,  503,  440,  505, 1414, 1593,  508,
  509,  510,  511,  512,   59,   59,   59,  489,  490,  454,
  492,    0,    0,  405,   59, 1499,  408,  499,  500,  501,
  502,  503,   93,  505,  258,   40,  508,  509,  510,  511,
  512,  258,  424, 1552,  261,  411,   44,    0,  401,    0,
  274,  397,  418,  277,  278,    0,  280,   59,  278,  465,
  284,  467,   41,  287, 1405,   44,   42,   43,  450,   45,
  436,   47,  301,  413, 1413,  384,  300,  417,  418,  419,
   59,   59,  306,  318, 1384,  309,  460, 1413, 1414,  306,
  427,  279,    0, 1393, 1505,   93,  331,  260,  258,  319,
  466,  383,  342, 1422,  470,  340,   59, 1682,   59,  338,
  321, 1527,  383, 1413,   59,  489,  490,  328,  492,  265,
  331,  260, 1422,  260,  335,  499,  500,  501,  502,  503,
  439,  505,  343,    0,  508,  509,  510,  511,  512,  479,
  303,  329, 1698,  483,    0, 1552,  306,  456,  359,    0,
   42,   59,  387, 1728,  378,    0,  302,   42,   43,    0,
   45,  396,   47,   44,  303,  405,  303,  265,  408, 1703,
  385,    0,  383, 1634,   44,  363,  400,  378,  407, 1184,
  400, 1186, 1533,  407,  424,    0,  383,  411,  417,   59,
 1577,  337,   59,  381,  418,  402, 1771,    0, 1659,  400,
    0, 1495,  413,   59,  302,  440,  407,  418,   59,   46,
  450, 1505,  436,  442,   59,   44,  272,  441,   59,  454,
    0,  441,  328,   58,   61,  331, 1552, 1634, 1803,  440,
   59,  438,  288, 1533, 1578,  342,  279,  436,  296,  458,
  441,  452,  466,  454,   59, 1706,  470,  383,   42,   43,
 1759,   45, 1659,   47, 1573, 1536,   59,  300,  418,   59,
    0, 1602,  307,  308,    0, 1606,  485,  417, 1568,   40,
  377,   42,   43, 1573,   45,  321,  436,  333, 1578,   59,
   46,  387, 1791, 1792, 1671, 1746,  329,   58,   41,  335,
  396,   44,   63, 1754,  444,   61, 1597, 1597,  405, 1706,
  494,  408,    0,    0,  280,  348,  362, 1816,  648,  285,
  470,  301,  321,  359,  357, 1646,    0,  424, 1827,   59,
  363, 1830,  337,   59, 1785,  665,  335, 1788,    0,  669,
    0,  356, 1632, 1633,  316,  317,  676,  383,  381, 1746,
   93,  681,  682,  450,  320,  418, 1646, 1754,  338, 1810,
  359,  691, 1759,  693,  694,  695,  696,  697,  698,  699,
  274,   59,   59, 1714,   45,  410,  402,  413,  393,   41,
  395,  279,   44,  429,  383,   59,  493,  494, 1785, 1680,
 1680, 1788,  236,   41, 1791, 1792,   44,   59,  728,   59,
  244,   41,  300,  418,   44,  280, 1697, 1697,  434,  318,
  285,   40,  460, 1810,  413,   41,  451,   41,   44, 1816,
   44,  751,  424,  449, 1714, 1715,  756,    0,  443,  337,
 1827,  329, 1716, 1830,  863,  337, 1716,  461,  867, 1730,
 1730,  489,  490, 1759,  492,  320, 1737, 1737,  279, 1783,
  348,  499,  500,  501,  502,  503,  264,  505,  356,  357,
  508,  509,  510,  511,  512,  363,  374,   93,   41,  300,
   41,   44,  374,   44, 1659, 1791, 1792,   41,  316,  387,
   44, 1772, 1772,  381,   41,  387,   59,   44,  337, 1780,
 1780,  321,  318, 1783,  336,  393,  280,  395,  329,  260,
 1816,  285,  457,  458,  459,  335,  267,   41,    0,   41,
   44, 1827,   44,  343, 1830,  276,   41,  348, 1808,   44,
  418, 1706,  461,  462,  460, 1815,  357,  311,  289,  359,
  485,  486,  363,  452,  295,  296,   42,   43,   44,   45,
  402,   47,  303, 1833,  340,  443,  876,  877,  878,   41,
  381,  881,  882,  883,   40,  885,  886,  887,  888,  889,
  890, 1746,  323,  893,  894,  326,  896,   59,  898, 1754,
  320,  901,  902,  903,  904,  905,  906,  881,  460,  883,
  318,  885,  886,  413,  888,  889,  890,  402,  918,   41,
  920,  352,   44,  354,  355,   41,  391,  392,   44,    0,
 1785,   41,  383, 1788,   44,  366,  383,  489,  490,  370,
  492,   42,   43,  506,   45,  436,   47,  499,  500,  501,
  502,  503,  361,  505,  295, 1810,  508,  509,  510,  511,
  512,  434,  450,   41,  964,  460,   44,  461,  462,   40,
   41,   42,   43,   44,   45,   46,   47,   41,   42,   43,
  384,   45,  323,   47, 1706,  417,  442,  454,   59,   60,
   61,   62,  357,  411,  489,  490,  427,  492,   41,  287,
  469,   44,  433,    0,  499,  500,  501,  502,  503,   44,
  505,  352,  443,  508,  509,  510,  511,  512,   41,   59,
   91,   44,   93,   61, 1746,  372,  457,  458,  459,  460,
  461,  462,  286,   42,   43,  278,   45,  280,   47,   41,
  471,   40,   44,  264,   41,   40,    0,   44,   40,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   40,  492,   59, 1785,  495,  496, 1788,  498,  499,  500,
  501,  502,  503,  504,  505,   40,  461,  508,  509,  510,
  511,  512,   40,   40,   40,   40,  427,   41, 1810,  664,
   44,  666,   40,   40,  461,  670,  671,  672,   42,   43,
  461,   45,  443,   47,  280,   59,   40,    0,   40,  285,
 1110,   40,   40,   44,   91,   59,  457,  458,  459,   46,
  461,  462, 1122, 1123,  436, 1125, 1126,  320,   41,   42,
   43,  402,   45, 1133,   47,  378,   44,  320,   44,  480,
  481,  482,  483,  484,  485,  486,  487,  488,   41,   44,
  428,   44, 1152,   40,  495,  496,  318,  400,   46,   61,
 1160,  264, 1162,  504,  407, 1165,   59, 1167,   61,   44,
  382, 1171, 1172, 1173, 1174,  321,  505,  348,  340,  280,
   44,  278,  278,  300,  285,  264,    0,  397,  260,  335,
  436,  262,  397,  264,  265, 1195,  470,  343,  441,  458,
  277,  272,  447,  447,   40, 1205,  349,  399,   40,  280,
   91,   40,   40,  359,  285,  264,  280,  288,  318,  383,
  411,  285,   44,  389,  382,  387,  451,   41,   40,  301,
   44,  302,   40,  436,  396,   41,  307,  308,    0,  340,
  311,  313,  464,  317, 1244,   59,   40,  318,  448,  320,
  321,   42,   43,  324,   45,  320,   47,  328,   42,   43,
  331,   45,  333,   47,  335,  320,  338,  413,  452,  340,
  341,  280,  343,  264,  502,   59,  285,  458,  440,   41,
   42,   43,   44,   45,   46,   47,   40,  277,  359,  360,
  274,  362,  454, 1293,   40,  300,  284,   59,   60,   61,
   62,  386,  311,  344,  278,  376,  386,  378,  379,  380,
  344,  386,  383,  418, 1314,  386,  387,   42,   43,  300,
   45,  318,   47, 1323,  278,  396,  280,  416,  418,   91,
 1330,   93,   40,  287,  331,  407,  280,  356,   40,  410,
   40,  285,  413,  340,  417,  417,  300,  417,   41,   42,
   43,    0,   45,  274,   47,  264,   41,   44,  429, 1359,
  320,   44,  280, 1363,  435,  258,   44,  280,  385,  440,
  442,  367,  285,   44,  320,   40,   44,  328,   44,   40,
  451,  452, 1382,  454,  277,  278,  266,  280,  299,  460,
  387,  284,   41,  501,  287,   44,  340,   40,  264,  396,
   41,   42,   43,   41,   45,   58,   47,  300,  422,   41,
   59,   41,   41,  306,    0,  264,  440,  454,  489,  490,
  460,  492,   42,   43,  378,   45,  394,   47,  499,  500,
  501,  502,  503,  309,  505,  492,  294,  508,  509,  510,
  511,  512,  309,  440,   41,   41,  400,  305,   40,  489,
  490,   40,  492,  407,  312,   41,  352,  454,   44,  499,
  500,  501,  502,  503,  278,  505,  280,  325,  508,  509,
  510,  511,  512,   59,  265,   40,  264,   40, 1478, 1479,
   40,   40,   40,   40,   40,  378,   40,  441,   40,  280,
   40,    0,   40,   40,  285,   40,  280,   40,  262, 1499,
  262,  285,  264,  265,  308,  435,  327,  400,   44,   41,
  272,  302,  343,  274,  407,  388,  388,  388,  280,  422,
  331,  390,  446,  285,  271,  418,  288,  287,  386,  391,
  407,  386,   41,   42,   43,   44,   45,   46,   47, 1539,
  302,  386,  304,  436,  402,  307,  308,  386,  441,  311,
   59,   60,   61,   62,   41,  280,  318,   41,  320,  321,
  285,  419,  324,  386,  378,  378,  328,    0,  386,  331,
  386,  333,  278,  335,   40,  417,  434,  470,  340,  341,
  438,  343,   91,  441,   93,   41,  400,  280,   41,   41,
   41,  449,  285,  407,  308,  435,  385,  359,  360,  436,
  362, 1601,  337,  396,  280,    0,  338,  442,   41, 1609,
  468,   44,  442,  301,  376,  338,  378,  379,  380,  301,
   41,  383,   41,  352,  386,  387,   59,  441,  320,  278,
  385,  280,  371,  371,  396,   41,   41,   41,  287,  280,
   41,   41,  262,   41,  285, 1645,  280, 1647,  410,   41,
   41,  413,   41,   42,   43,   41,   45,   41,   47,   41,
  280,   41,   41,   41,   59,  285,   41,  429,   41,   46,
   44,   59,  460,  435,   40,   40,  294,  340,  440,   41,
   42,   43,  340,   45,  308,   47,   41,  305,   59,  451,
  452,   59,  454,  352,  312,   59,   59,  344,  460,   59,
   59,  489,  490, 1703,  492, 1705,   59,   41,  368,   40,
  508,  499,  500,  501,  502,  503,  383,  505,  340,  337,
  508,  509,  510,  511,  512,  451,   44,  489,  490,  378,
  492,   59,  318,   59,   41,  383,  343,  499,  500,  501,
  502,  503,  328,  505,    0,  331,  508,  509,  510,  511,
  512,  400,  383,  262,  340,  264,  265,  343,  407,   41,
   41,   44,  435,  272,  410,  304,   41,   42,   43,   40,
   45,  280,   47,  301,  387,   59,  285,  293,  442,  288,
  304,   44,  463,   44,  402,   41,   42,   43,   44,   45,
   46,   47,  441,  302,  307,   41,    0,  337,  307,  308,
  383,  387,  311,   59,   60,   61,   62,  383,   40,  318,
  396,  320,  321,    0,  264,  324,  434,  278,  264,  328,
  438,  458,  331,  320,  333,  279,  335,  264,  470,  411,
  451,  340,  341,   44,  343,   91,  271,   93,  264,   41,
   42,   43,   40,   45,  458,   47,  300,  435,  343,  343,
  359,  360,  343,  362,  440,   41,   42,   43,  343,   45,
  458,   47,  271,  258,  308,   41,  452,  376,  454,  378,
  379,  380,   59,    0,  383,  329,   40,  386,  387,  308,
   40,   59,   59,  308,  279,  318,   41,  396,   59,  304,
   41,  280,   59,   41,  348,  328,  285,    0,  331,    0,
    0,  410, 1187,  357,  413,  300,   12,  340,  264,  363,
  343,  306,  263,  271,   41, 1087,  634,   44,  280,  709,
  429,  856, 1085,  285,   87, 1082,  435,  381, 1441,    0,
 1074,  440,   59, 1440,  329, 1064,  326,  581,  356,  311,
  321, 1009,  451,  452,  753,  454, 1643, 1075, 1419, 1721,
  788,  460,  560,  348,  387,  338, 1724, 1421, 1763,  778,
 1725,  356,  357,  396, 1703, 1242, 1815, 1760,  363,  338,
   41, 1664, 1220,   44,  996, 1398,  343,  331, 1680,  324,
  489,  490,  562,  492,  835, 1276,  381,  620,   59,   13,
  499,  500,  501,  502,  503,  298,  505,    0,  494,  508,
  509,  510,  511,  512,  756,  280,  262,  440,  264,  265,
  285,  742,  827,  141,  729,  305,  272,  498,  728,  452,
  973,  454,  312,  418,  280, 1780,  316,  317, 1737,  285,
 1634, 1833,  288,  509,  429,  325, 1522,  717,   41,   42,
   43,   44,   45,   46,   47,  965,  302,  337, 1124, 1231,
   -1,  307,  308, 1167,   -1,  311,   59,   60,   61,   62,
   -1,   -1,  318,   -1,  320,  321,    0,   -1,  324,   -1,
   -1,  258,  328,   -1,   -1,  331,   -1,  333,  280,  335,
   -1,   -1,   -1,  285,  340,  341,   -1,  343,   91,   -1,
   93,   -1,  279,   -1,  280,   -1,   -1,   -1,   -1,  285,
   -1,   -1,   -1,  359,  360,   -1,  362,   41,   -1,   -1,
   44,   -1,  402,  300,   -1,   -1,   -1,   -1,   -1,  306,
  376,   -1,  378,  379,  380,   59,   -1,  383,   -1,  419,
  386,  387,  472,  473,  474,  475,  476,  477,  478,  479,
  396,   -1,  329,   -1,  434,   -1,   -1,   -1,  438,  260,
   -1,  278,   -1,  280,  410,   -1,    0,  413,   -1,  449,
  287,  348,   -1,   -1,   -1,   41,   42,   43,   -1,   45,
  357,   47,   -1,  429,   -1,   -1,  363,   -1,  468,  435,
   -1,   41,   42,   43,  440,   45,   -1,   47,   -1,   -1,
  301,   -1,   -1,   -1,  381,  451,  452,   41,  454,   -1,
   44,   -1,  313,   -1,  460,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   59,  287,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,   -1,  338,   -1,  300,
   -1,  418,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
    0,  378,  508,  509,  510,  511,  512,   -1,   -1,  262,
    0,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   -1,  400,   -1,   -1,   -1,  280,   -1,   -1,
  407,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,  407,   47,   -1,  302,
   -1,   -1,   -1,   -1,  307,  308,  417,  378,  311,   59,
   60,   61,   62,   -1,  441,  318,   -1,  320,  321,   59,
   -1,  324,   -1,   -1,  258,  328,   -1,    0,  331,  400,
  333,  442,  335,   -1,   -1,   -1,  407,  340,  341,   -1,
  343,   -1,   -1,   93,   -1,  279,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   42,   43,   -1,   45,   -1,   47,  300,   -1,   41,   -1,
  441,   44,  306,  376,   -1,  378,  379,  380,  489,  490,
  383,  492,   -1,  386,  387,   -1,   59,   -1,  499,  500,
  501,  502,  503,  396,  505,  329,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  280,   -1,   -1,  410,   -1,  285,
  413,   41,   42,   43,  348,   45,   -1,   47,   -1,   -1,
  280,   -1,   -1,  357,   -1,  285,  429,   -1,   -1,  363,
   -1,    0,  435,   -1,  278,   -1,  280,  440,   -1,   -1,
  284,   -1,   -1,  287,   -1,   -1,   -1,  381,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,  300,  460,  280,  287,
   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   41,   42,   43,
   -1,   45,    0,   47,  418,   -1,  489,  490,   -1,  492,
   59,   -1,   61,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,  258,   -1,
   -1,   -1,  272,   41,   42,   43,   44,   45,   -1,   47,
  280,   -1,   -1,   -1,  378,  285,   -1,   -1,  288,  279,
   -1,   59,   60,   61,   62,   -1,   -1,   41,   42,   43,
   -1,   45,  302,   47,   -1,   -1,  400,  307,  308,   -1,
  300,  311,   -1,  407,   -1,   -1,  306,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   93,   -1,   -1,  328,   -1,
  262,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,  329,
  340,  341,   -1,  343,   -1,   -1,   -1,  441,  280,   -1,
   -1,   -1,   -1,  285,   -1,  278,   -1,  280,  348,  359,
  360,   -1,  362,   -1,  287,   -1,   -1,  357,   -1,   -1,
   -1,   -1,   -1,  363,   -1,   -1,  376,  300,  378,  379,
  380,   -1,  460,  383,   -1,   -1,  386,  387,   41,   42,
   43,  381,   45,   -1,   47,   -1,  396,   41,   42,   43,
  280,   45,   -1,   47,   -1,  285,   -1,   -1,   -1,   -1,
  410,  489,  490,  413,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,  418,  429,
  508,  509,  510,  511,  512,  435,   -1,   -1,   -1,  258,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,  378,   -1,   -1,  277,  278,
  460,  280,   -1,   -1,   -1,  284,  280,   -1,  287,   -1,
   -1,  285,   -1,   -1,   -1,   -1,   -1,  400,   -1,   -1,
   -1,  300,   -1,   -1,  407,    0,   -1,  306,   -1,  489,
  490,   -1,  492,   -1,  262,    0,  264,  265,   -1,  499,
  500,  501,  502,  503,  272,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,  285,  441,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   44,
   45,   -1,   47,   -1,  302,   -1,  280,   -1,   -1,  307,
  308,  285,   -1,  311,   59,   60,   61,   62,   -1,   -1,
  318,   -1,  320,  321,   59,   -1,  324,   -1,   -1,  378,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   93,   -1,
   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,  416,   -1,  418,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,  436,  386,  387,
   -1,    0,  441,   -1,   -1,   -1,   -1,  280,  396,   -1,
   -1,   -1,  285,   -1,   -1,   -1,  280,   -1,   -1,   -1,
   -1,  285,  410,   -1,    0,  413,   -1,   -1,   -1,   -1,
   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  429,   41,   -1,   -1,   44,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,  451,  452,   41,  454,   -1,   44, 1315,
 1316,   -1,  460,   -1,  287, 1321, 1322,   -1, 1324, 1325,
   -1,   -1,   -1,   59,   -1,   61, 1332, 1333, 1334, 1335,
   -1, 1337, 1338, 1339, 1340, 1341, 1342,    0,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  258,   -1,   -1,   -1,  272,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  288,  279,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,  300,  311,    0,   -1,   -1,
   -1,  306,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   93,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,  329,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  348,  359,  360,   -1,  362,   -1,   -1,
   -1,   -1,  357,   -1,   -1,   -1,   59,   -1,  363,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,  460,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,  381,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  410,  489,  490,  413,  492,
   -1,   -1,  258,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,  418,  429,  508,  509,  510,  511,  512,
  435,   -1,  278,   -1,  280,  440,   -1,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
   -1,   -1,  321,   -1,  300,  460,   -1,   -1,   -1,  328,
  306,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  262,
  359,  264,  265,   -1,  499,  500,  501,  502,  503,  272,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,  383,  288,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,  302,
   -1,   -1,  378,   -1,  307,  308,   -1,   -1,  311,   59,
   60,   61,   62,   -1,  413,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,  400,  328,   -1,   -1,  331,   -1,
  333,  407,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,  440,  418,   93,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,  359,  360,   -1,  362,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,  470,  318,   -1,   -1,  321,   -1,
   -1,   -1,  260,   -1,   -1,  328,   -1,  410,  331,   -1,
  413,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,  359,  440,   -1,   -1,
   -1,   -1,   -1,  301,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,  313,   -1,  460,   -1,   -1,
  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,  327,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
  338,   -1,    0,   -1,   -1,   -1,  489,  490,   -1,  492,
  413,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  262,   -1,  264,  265,   -1,  440,   -1,   -1,
   -1,   -1,  272,   41,   -1,   43,   44,   45,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,  407,
   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,  417,
   -1,  311,    0,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   93,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,  442,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   41,   -1,   -1,   44,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,  489,  490,  383,  492,   -1,  386,  387,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  396,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,    0,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,  262,   -1,  264,  265,   -1,  499,
  500,  501,  502,  503,  272,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,
  308,   -1,   -1,  311,   59,   60,   61,   62,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,  410,  331,   -1,  413,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  460,   -1,  272,  383,   -1,   -1,   -1,  387,
   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,  396,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  324,  262,   -1,  264,
  265,   -1,  440,   -1,   -1,  333,   -1,  272,   41,   -1,
   43,   44,   45,  341,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,   -1,  288,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  360,   -1,  362,   -1,   -1,  302,   -1,  328,
   -1,   -1,  307,  308,   -1,   -1,  311,   -1,  376,   -1,
  378,  379,  380,  318,   -1,  320,  321,   -1,   -1,  324,
   93,  434,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,  376,   -1,  378,  379,  380,  489,  490,  383,  492,
   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,  502,
  503,  396,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   41,
   -1,   -1,   44,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  429,   -1,   -1,   59,   -1,   61,
  435,  460,   -1,   -1,   -1,  440,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,  451,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   58,   -1,   91,
  489,  490,   63,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,  489,  490,   -1,  492,   -1,  262,
   -1,  264,  265,   -1,  499,  500,  501,  502,  503,  272,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,
   -1,   -1,   -1,   -1,  307,  308,   40,   -1,  311,   43,
   -1,   45,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   58,  328,   -1,   -1,  331,   63,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,
  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,  500,
  501,  502,  503,  396,  505,   -1,  258,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,
  413,   -1,  274,  443,   -1,  277,  278,   -1,  280,   -1,
   -1,   -1,  284,   -1,   -1,  287,  429,   -1,   -1,   -1,
  460,   -1,  435,   -1,   -1,   -1,  267,  440,  300,   -1,
   -1,   -1,   -1,   -1,  306,  276,   -1,  309,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,  289,  489,
  490,   -1,  492,   -1,  295,  296,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  314,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,  366,   -1,   -1,  400,  370,
   -1,   -1,  276,   -1,   -1,  407,   -1,  378,   -1,  411,
   -1,   -1,   -1,   -1,   -1,  289,  418,   -1,  292,   -1,
   -1,  295,  296,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
  314,   -1,   58,   -1,   -1,   -1,  417,   63,   -1,  323,
  421,   -1,  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,  466,   -1,   -1,   -1,  470,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,  460,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,   -1,   -1,  421,   -1,   -1,
   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,  433,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  443,
  508,  509,  510,  511,  512,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,  471,   -1,   63,
   -1,   -1,  328,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,
   -1,  267,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  402,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,
   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   59,  438,   61,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,  449,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,  460,   -1,  352,   -1,  354,  355,
   -1,   -1,  468,   91,   -1,   -1,   58,   -1,   -1,   -1,
  366,   63,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
  314,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  354,  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,  366,    0,   -1,   -1,  370,   -1,   -1,   -1,
  258,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,  277,
  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  300,   -1,   -1,  267,  354,  421,  306,   -1,
   -1,  309,   59,  427,  276,   -1,   -1,   40,   -1,  433,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
  378,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  354,  355,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  400,   -1,  366,   -1,   -1,   -1,  370,  407,
  458,   -1,  460,  411,   40,   -1,  378,   43,   -1,   45,
  418,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  436,   -1,
   -1,  489,  490,  441,  492,   -1,   -1,   -1,  343,   -1,
  498,  499,  500,  501,  502,  503,   -1,  505,   -1,  421,
  508,  509,  510,  511,  512,  427,   -1,   -1,  466,   -1,
   -1,  433,  470,  368,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,  426,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,
   -1,  318,  295,  296,  321,   -1,   -1,   -1,   -1,    0,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,  314,   -1,  340,  489,  490,  343,  492,   -1,   -1,
  323,   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,
  505,   -1,  359,  508,  509,  510,  511,  512,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,  260,   -1,  383,  263,   59,  386,
  387,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,  396,
  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  413,   -1,   40,  295,
  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  435,   -1,
   -1,   63,   -1,  440,   -1,   -1,   -1,  323,  421,   -1,
  326,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,   -1,
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
   -1,   -1,  343,  295,  296,   -1,   -1,   -1,  460,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  359,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  323,   63,   -1,  326,   -1,   -1,  489,  490,   -1,
  492,   -1,  383,   -1,   -1,  386,  387,  499,  500,  501,
  502,  503,   -1,  505,   -1,  396,  508,  509,  510,  511,
  512,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   42,   43,   -1,   45,  289,   47,
   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
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
  511,  512,   -1,  260,  272,   -1,  263,   -1,   -1,   -1,
  267,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,  276,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,  333,  323,   63,   -1,  326,
   -1,   -1,   -1,  341,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,  362,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,  366,
  378,  379,  380,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,
   -1,  429,  267,   -1,   -1,   -1,   -1,   -1,  425,   -1,
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
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,  276,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  261,   -1,   -1,   -1,   58,  289,   -1,   -1,
   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,
  299,  323,  301,   -1,  326,   -1,   -1,  306,   -1,   -1,
   -1,   -1,  318,   -1,  313,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,  327,  335,
   -1,   -1,  354,  355,  340,   -1,   -1,  343,   -1,  338,
   -1,   -1,   -1,   -1,  366,   -1,   40,   -1,  370,   43,
   -1,   45,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,  405,  413,   -1,   -1,
   -1,  433,   -1,  412,   -1,   -1,  415,   -1,  417,  418,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,  457,  458,  459,  460,  461,
  462,   -1,   -1,  442,   -1,   -1,  452,  273,  454,  471,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  295,
  492,   -1,  298,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,  491,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,  506,  507,   -1,
   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,  370,   -1,
  406,   -1,  276,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,  423,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  480,  481,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  497,  366,   -1,   -1,   -1,  370,   -1,   -1,  471,
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
   -1,  326,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
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
   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,
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
   -1,   -1,    0,   -1,   -1,  407,   -1,   -1,   -1,  411,
   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  267,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,   -1,
   40,   59,   -1,   43,  466,   45,   -1,  366,  470,   -1,
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
  318,   -1,   63,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,  323,   -1,  343,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  383,  366,   -1,  386,  387,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,  396,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,  452,   -1,  454,   -1,   -1,   -1,
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
  396,  262,  440,   -1,   -1,   -1,   41,    0,   -1,   44,
   -1,   -1,   -1,   -1,  452,   -1,  454,  413,   -1,  383,
   -1,   -1,  386,  387,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,  318,   -1,  435,
  321,   -1,   -1,   -1,  440,   -1,   -1,  328,   41,  413,
  331,   44,   -1,   -1,  335,   -1,  452,  318,  454,  340,
  321,   -1,  343,   -1,   -1,   -1,   59,  328,   -1,   -1,
  331,  435,   -1,   -1,  335,   -1,  440,   -1,  359,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,   -1,  262,  359,   -1,
   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,  262,  413,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  413,  318,  435,   -1,  321,   -1,   -1,  440,
   -1,   -1,   59,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,  452,   -1,  454,  435,  340,   -1,   -1,  343,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,  452,   -1,  454,  359,  262,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,  262,  359,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,  318,  383,   -1,  321,  386,  387,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,  396,   -1,   -1,  335,  262,
  435,   -1,   -1,  340,   -1,  440,  343,   -1,   -1,   -1,
   -1,   -1,  413,  318,   -1,   -1,  321,  452,   -1,  454,
   -1,   -1,  359,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,   -1,   -1,   -1,  435,  340,   -1,   -1,  343,  440,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,  452,   -1,  454,  359,  318,   -1,   -1,  321,  396,
  271,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  413,  340,  383,   -1,
  343,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  301,  396,   -1,   -1,   -1,  262,  359,  308,  435,   -1,
   -1,  312,  313,  440,  315,   -1,   -1,  318,  413,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
  383,  332,   -1,  386,  387,   -1,   -1,  338,   -1,   -1,
  435,   -1,   -1,  396,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
  413,  318,   -1,   -1,  321,   -1,  271,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  435,  340,   -1,   -1,  343,  440,   -1,   -1,
   -1,   -1,   -1,  271,   -1,   -1,  301,  398,   -1,  452,
   -1,  454,  359,   -1,   -1,   -1,   -1,  312,  313,   -1,
  315,   -1,   -1,  318,   -1,   -1,  417,   -1,   -1,   -1,
   -1,   -1,   -1,  301,   -1,   -1,  383,  332,   -1,  386,
  387,  432,   -1,  338,  312,  313,   -1,  315,   -1,  396,
  318,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  451,   -1,  453,   -1,  332,   -1,  413,   -1,   -1,  460,
  338,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,  398,   -1,  452,   -1,  454,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,  417,
   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,
   -1,   -1,   -1,   -1,  432,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  453,   -1,  273,   -1,   -1,
   -1,  277,  460,   -1,  489,  490,   -1,  492,  284,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  295,
  505,   -1,  298,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  489,  490,   -1,  492,  273,   -1,   -1,   -1,  277,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  295,   -1,   -1,
  298,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  406,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  460,  423,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,  445,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  497,  460,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,
   -1,  499,  500,  501,  502,  503,  460,  505,   -1,   -1,
  508,  509,  510,  511,  512,  469,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,   -1,  499,  500,  501,  502,  503,
  504,  505,  506,  460,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,   -1,   -1,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,   -1,  499,  500,  501,  502,  503,  504,  505,  506,
   -1,  508,  509,  510,  511,  512,
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
"OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","KW_PAGE_SIZE","PARAMETER",
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
"filter_decl_clause : symbol_filter_name INPUT_TYPE blob_subtype_io OUTPUT_TYPE blob_subtype_io ENTRY_POINT sql_string MODULE_NAME sql_string",
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
"exec_sql : EXECUTE STATEMENT value ';'",
"for_select : FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list ';'",
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
"blob_subtype_value : blob_subtype_io",
"blob_subtype_value : parameter",
"blob_subtype_io : signed_short_integer",
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
case 314:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 315:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
break;
case 316:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
break;
case 317:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 318:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 319:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 320:
{ yyval = NULL; }
break;
case 321:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 322:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 323:
{ yyval = make_list (yyvsp[0]); }
break;
case 324:
{ yyval = make_list (yyvsp[-1]); }
break;
case 325:
{ yyval = NULL; }
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
case 331:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 332:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 333:
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 334:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 335:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 336:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 337:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 347:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 348:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 351:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 353:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 355:
{ yyval = make_list (yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 361:
{ yyval = yyvsp[-1]; }
break;
case 362:
{ lex.beginning = lex_position(); }
break;
case 363:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 364:
{ lex.beginning = lex.last_token; }
break;
case 365:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 366:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 367:
{ yyval = 0; }
break;
case 368:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 369:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 370:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 371:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 372:
{ yyval = NULL; }
break;
case 373:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 374:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 375:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 376:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 392:
{ yyval = NULL; }
break;
case 393:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 394:
{ yyval = yyvsp[0]; }
break;
case 395:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 396:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 397:
{ yyval = yyvsp[0]; }
break;
case 398:
{ yyval = yyvsp[0]; }
break;
case 399:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 400:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 401:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 402:
{ yyval = yyvsp[0]; }
break;
case 403:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 405:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 406:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 410:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 414:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 415:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 416:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 420:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 421:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 422:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 446:
{ yyval = NULL; }
break;
case 447:
{ yyval = NULL; }
break;
case 448:
{ yyval = NULL; }
break;
case 449:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 450:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 451:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 452:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 453:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 454:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 455:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 456:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 458:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 459:
{ yyval = yyvsp[0]; }
break;
case 460:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 461:
{ yyval = yyvsp[0]; }
break;
case 462:
{ yyval = yyvsp[0]; }
break;
case 464:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 465:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 467:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = yyvsp[0]; }
break;
case 471:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 472:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 473:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 474:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 477:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 487:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 488:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 490:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 491:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 492:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 498:
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
case 499:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 500:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 501:
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
case 502:
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
case 503:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 506:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 507:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 508:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 510:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 511:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 512:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 513:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 514:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 515:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 531:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 532:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 533:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 534:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 535:
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
case 536:
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
case 539:
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
case 540:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 543:
{ yyval = yyvsp[-1]; }
break;
case 544:
{ yyval = 0; }
break;
case 548:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 549:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 550:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 554:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 555:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 556:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 557:
{ yyval = 0; }
break;
case 558:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 561:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 562:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 565:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 566:
{ yyval = (DSQL_NOD) NULL; }
break;
case 568:
{ yyval = (DSQL_NOD) NULL; }
break;
case 569:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 571:
{ yyval = (DSQL_NOD) NULL; }
break;
case 573:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 578:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 579:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 580:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 581:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 582:
{ yyval = yyvsp[0];}
break;
case 584:
{ yyval = yyvsp[0];}
break;
case 585:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 586:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 587:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 592:
{ yyval = 0; }
break;
case 593:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 594:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 595:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 596:
{ yyval = (DSQL_NOD) 0; }
break;
case 597:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 598:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 600:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 601:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 602:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 603:
{ yyval = 0; }
break;
case 605:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 606:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 607:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 608:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 610:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 611:
{ yyval = make_list (yyvsp[0]); }
break;
case 612:
{ yyval = 0; }
break;
case 614:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 615:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 616:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 618:
{ yyval = 0; }
break;
case 619:
{ yyval = MAKE_constant((STR)NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 620:
{ yyval = MAKE_constant((STR)NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 621:
{ yyval = yyvsp[-1]; }
break;
case 622:
{ yyval = 0; }
break;
case 623:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 624:
{ yyval = 0; }
break;
case 625:
{ yyval = yyvsp[0]; }
break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 627:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 628:
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 630:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 631:
{ lex.limit_clause = true; }
break;
case 632:
{ lex.limit_clause = false; }
break;
case 633:
{ lex.first_detection = true; }
break;
case 634:
{ lex.first_detection = false; }
break;
case 635:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 636:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 638:
{ yyval = 0; }
break;
case 639:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 640:
{ yyval = yyvsp[-2]; }
break;
case 641:
{ yyval = yyvsp[-1]; }
break;
case 642:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 643:
{ yyval = yyvsp[-1]; }
break;
case 644:
{ yyval = yyvsp[0]; }
break;
case 645:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 646:
{ yyval = 0; }
break;
case 647:
{ yyval = make_list (yyvsp[0]); }
break;
case 648:
{ yyval = 0; }
break;
case 650:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 652:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 653:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 654:
{ yyval = make_list (yyvsp[0]); }
break;
case 656:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 659:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 660:
{ yyval = yyvsp[-1]; }
break;
case 661:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 662:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 663:
{ yyval = make_list (yyvsp[-1]); }
break;
case 664:
{ yyval = NULL; }
break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 670:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 671:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 672:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 673:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 674:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 678:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 680:
{ yyval = make_list (yyvsp[0]); }
break;
case 681:
{ yyval = 0; }
break;
case 683:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 688:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 692:
{ yyval = yyvsp[0]; }
break;
case 693:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 699:
{ yyval = 0; }
break;
case 700:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 701:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 702:
{ yyval = 0; }
break;
case 703:
{ yyval = 0; }
break;
case 705:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 706:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 709:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 710:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 711:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 712:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 714:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 716:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 722:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 726:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 727:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 729:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 733:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 734:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 737:
{ yyval = NULL; }
break;
case 740:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 741:
{ yyval = yyvsp[0]; }
break;
case 742:
{ yyval = NULL; }
break;
case 743:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 746:
{ yyval = NULL; }
break;
case 747:
{ yyval = make_list (yyvsp[-1]); }
break;
case 749:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 751:
{ yyval = NULL; }
break;
case 752:
{ yyval = make_list (yyvsp[-1]); }
break;
case 754:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 756:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 758:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = yyvsp[-1]; }
break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 813:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 815:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 817:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 819:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 821:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 823:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 829:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
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
case 836:
{ yyval = make_list (yyvsp[-1]); }
break;
case 837:
{ yyval = yyvsp[-1]; }
break;
case 838:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 839:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 849:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 850:
{ yyval = yyvsp[0]; }
break;
case 851:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 852:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 854:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 855:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 856:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 857:
{ yyval = yyvsp[-1]; }
break;
case 858:
{ yyval = yyvsp[-1]; }
break;
case 862:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 863:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 864:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 865:
{ yyval = yyvsp[0]; }
break;
case 866:
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
case 867:
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
case 868:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 869:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 871:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 873:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 880:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 881:
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
case 882:
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
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 884:
{ yyval = make_parameter (); }
break;
case 885:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 886:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 887:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = yyvsp[0]; }
break;
case 894:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 896:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 897:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 898:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 899:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 902:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 903:
{ yyval = yyvsp[0];}
break;
case 908:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 909:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 911:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 915:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 919:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 920:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 922:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 923:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 926:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 929:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 930:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 933:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 934:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 936:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 937:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 938:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 939:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 940:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 954:
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
