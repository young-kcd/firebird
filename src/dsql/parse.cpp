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

/* TMN: Remove Microsoft introduced defines*/
#ifdef DELETE
#undef DELETE
#endif
#ifdef IN
#undef IN
#endif
#ifdef SHARED /* sys/mman.h */
#undef SHARED
#endif

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
	USHORT	param_number;
	/* Fields to handle FIRST/SKIP as non-reserved keywords */
	bool after_select; /* Check this to detect SKIP/FIRST occurence after SELECT */
	bool limit_clause; /* We are inside of limit clause. Need to detect SKIP after FIRST */
	bool first_detection; /* Detect FIRST unconditionally */
	
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
#define DELETE 301
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
#define IN 333
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
#define SHARED 420
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
#define STATEMENT 507
#define LEAVE 508
#define INSERTING 509
#define UPDATING 510
#define DELETING 511
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    9,    9,    9,    9,    9,   23,   23,   22,
   22,   22,   33,   33,   26,   34,   34,   34,   34,   34,
   25,   25,   31,   31,   27,   14,   14,   14,   14,   14,
   14,   14,   37,   28,   28,   28,   28,   38,   38,   38,
   38,   24,   24,   39,   39,   39,   29,   29,   44,   30,
   30,   45,   45,    6,   46,   46,   48,   53,   53,   53,
   50,   50,   50,   57,   57,   58,   58,   51,   51,   60,
   60,   60,   60,   60,   47,    5,   63,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   63,   12,   78,   78,
   78,   13,   82,   82,   65,   65,   68,   68,   68,   76,
   91,   91,   91,   92,   92,   93,   93,   94,   94,   98,
   98,   75,  101,  101,  103,  103,  104,  104,  107,  107,
  108,  109,  109,  110,  111,   73,   77,   74,   95,   95,
  114,  115,  115,  117,  117,  118,  118,  118,  118,  118,
  116,  116,  119,  119,  120,  120,  120,  120,  122,  122,
  122,  122,  121,  121,  126,  126,  125,   99,  127,  127,
  128,  131,  132,  132,  133,  129,  130,  130,  134,  134,
  135,  135,  136,  136,  136,   97,   97,   97,   70,   80,
  137,  137,  137,  138,  138,  139,  139,  140,  140,  140,
  146,   87,   87,  142,  142,  105,  105,  100,  150,  151,
   59,  143,  143,  106,  106,  106,  106,  106,  106,  144,
  144,  158,  158,  159,  161,  161,  161,  161,  161,  141,
  160,  160,  165,  165,  165,  165,  167,  168,  169,  163,
  163,  164,  162,  162,  162,  162,  162,  170,  171,  172,
  172,  172,  172,   69,   79,   83,  177,  173,  173,  174,
  174,  178,  178,  179,  175,  175,  180,  180,  181,  182,
  182,  183,  183,  184,  184,  176,  186,  186,  186,  187,
  187,  185,  185,  185,  185,  185,  185,  185,  185,  185,
  185,  185,  185,  185,  185,  185,  185,  185,  185,  190,
  190,  191,  192,  193,  204,  204,  194,  199,  200,  195,
  195,  196,  207,  207,  197,  208,  202,  202,  202,  203,
  203,  203,  205,  205,  205,  205,  198,  201,  201,  206,
  206,  188,  188,  213,  214,  214,  215,  215,  215,  215,
   11,  217,  217,  217,  218,  218,  218,  218,  218,  218,
   72,   81,  220,  223,  223,  223,  224,  227,  231,  231,
  232,  232,  233,  233,  148,  222,   88,   90,  221,  221,
   71,   84,  236,  236,  236,  237,  240,  240,  241,  241,
  241,  241,  241,  241,  241,  241,  241,  241,  241,  241,
  241,  241,  241,  238,  238,  239,    2,  243,  243,  243,
  243,  243,  243,  243,  251,  252,  249,  249,  253,  253,
  253,  253,  253,  253,  253,  244,  244,  254,  254,  254,
  254,  254,  254,  254,  248,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  259,  259,  259,  259,  259,  259,  259,  259,  259,
  259,  256,  256,  258,  258,  257,  255,  255,  255,  250,
  250,  246,  247,  247,  261,  261,  261,  261,  262,  262,
  245,  263,  263,  264,  264,    8,  265,  265,  265,  265,
  265,  265,  265,  265,  265,  265,  265,  265,  102,  102,
  145,  145,  268,  268,  271,  271,  273,  273,   54,   54,
  270,  270,  270,  270,  270,  270,  270,  270,  270,  278,
  278,  269,  269,  269,  269,  279,  279,   62,   62,   62,
   56,   56,  275,  275,  275,  272,  272,  272,  283,  283,
  283,  282,  282,  281,  281,  281,  276,  276,  286,  284,
  284,  284,  285,  285,  277,  277,  277,  277,  287,  287,
   19,   19,   19,  289,  289,  289,   16,   17,  293,  293,
    4,   15,  292,  292,  294,  294,  295,  295,  288,  296,
  296,  297,  297,  298,  298,  298,  298,  299,  299,  300,
  300,  301,  301,  303,  303,  303,  304,  304,  304,  305,
  305,  305,  302,  307,  307,  307,  308,  308,  306,  306,
  309,  311,  311,  310,  310,  290,   18,  312,  312,  312,
  313,  313,  316,  316,  317,   66,   66,   66,  319,  319,
  318,  318,  314,  314,  322,  322,  323,  323,  315,  327,
  328,  329,  320,  321,  324,  324,  324,  324,  330,  330,
  330,  331,  331,  331,  225,  225,  226,  226,  333,  333,
  334,  334,  334,  325,  337,  337,  338,  338,  339,  339,
  340,  340,  341,  341,  210,  210,  343,  343,  234,  234,
   32,  235,  235,  235,  235,  235,  235,  235,  235,  229,
  229,  344,  344,  345,  345,  345,  345,  345,  347,  347,
  347,  230,  230,  228,  228,  326,  326,  351,  352,  352,
  352,  352,  352,  353,  353,  354,  354,  355,  355,  356,
  356,  356,  357,  357,   10,   10,  359,  359,    7,    7,
  360,  361,  362,   20,   20,  363,  364,  365,  365,  189,
  335,  335,    3,    3,  367,  367,  367,  369,  369,   62,
  368,  368,  370,  370,   35,   35,   86,   85,   85,  358,
  358,  371,  372,  372,  211,  211,  211,  147,  366,  366,
  112,  112,  112,  112,  374,  374,  374,  374,  374,  374,
  374,  374,  374,  374,  374,  374,  375,  375,  375,  375,
  375,  375,  375,  375,  380,  380,  380,  380,  380,  380,
  380,  380,  380,  380,  380,  380,  380,  380,  380,  380,
  387,  387,  376,  376,  377,  377,  377,  377,  378,  378,
  382,  382,  383,  383,  383,  383,  381,  384,  379,  379,
  385,  385,  385,  388,  388,  389,  386,  391,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,  157,  157,  157,  392,  390,  390,
  152,  152,  396,  396,  396,  396,  396,  394,  394,  394,
  394,  394,  219,  153,  153,  154,  155,  155,  155,  155,
  155,   52,   52,   21,   21,  242,  397,   55,  124,  274,
  274,   96,  393,  393,  393,  393,  398,  398,  398,  398,
  398,  398,  398,  398,  398,  398,  398,  399,  348,  349,
  349,  349,  346,  346,  395,  350,  350,  401,  401,  402,
  402,  403,  403,  406,  406,  404,  404,  409,  409,  408,
  405,  407,  400,  400,  400,  400,  400,  400,  400,  400,
  332,  332,  156,   49,  280,  123,  149,  373,  166,  212,
  267,   64,   61,  216,  113,   67,  336,   36,   42,  342,
  266,   40,   43,  209,   41,  291,  260,  260,  410,  410,
  410,  410,  410,  410,  410,  410,  410,  410,  410,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    7,    8,    6,    7,    5,    1,    2,    1,
    2,    1,    1,    3,    1,    1,    1,    1,    2,    2,
    3,    0,    3,    0,    1,    7,    8,    6,    7,    6,
    7,    4,    3,    1,    3,    3,    3,    2,    2,    2,
    2,    1,    3,    1,    2,    2,    1,    3,    1,    1,
    3,    1,    2,    2,    2,    3,    8,    1,    1,    5,
    0,    1,    3,    1,    3,    2,    4,    1,    3,    2,
    3,    4,    4,    2,    9,    2,    3,    7,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    4,    2,    2,    1,    0,    1,    1,    6,    6,
    1,    1,    0,    0,    1,    0,    4,    0,    1,    1,
    2,    8,    1,    0,    3,    0,    0,    1,    1,    2,
    1,    1,    1,    2,    6,    1,    1,    3,    0,    1,
    1,    0,    1,    1,    2,    3,    4,    2,    2,    3,
    0,    1,    1,    2,    1,    1,    1,    4,    3,    3,
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
    6,    4,    2,    0,    4,    2,    1,    3,    0,    2,
    4,    0,    1,    1,    3,    3,    6,    2,    2,    3,
    0,    1,    2,    4,    1,    3,    2,    2,    2,    1,
    4,    1,    3,    0,    1,    1,    1,    3,    3,    3,
    7,    7,    1,    1,    3,    4,    7,    2,    1,    3,
    1,    1,    6,    3,    0,    0,    0,    0,    3,    0,
    9,    9,    1,    1,    0,    2,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    5,    5,    5,
    5,    5,    5,    2,    0,    4,    2,    3,    3,    2,
    2,    3,    3,    2,    3,    6,    1,    2,    4,    3,
    2,    2,    2,    2,    3,    1,    3,    3,    3,    2,
    2,    4,    4,    5,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    1,    1,    1,    0,    2,
    2,    0,    1,    2,    2,    2,    2,    2,    1,    3,
    7,    1,    0,    1,    0,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    3,    2,    2,    2,    1,    1,
    1,    1,    4,    5,    1,    3,    1,    3,    1,    2,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    4,    4,    6,    5,    3,    0,    2,    2,    0,
    3,    0,    4,    1,    5,    4,    1,    4,    1,    2,
    2,    1,    1,    1,    2,    2,    2,    2,    1,    0,
    3,    5,    1,    1,    2,    3,    1,    2,    3,    0,
    1,    1,    1,    5,    5,    6,    2,    5,    1,    0,
    3,    2,    1,    0,    2,    0,    1,    0,    3,    1,
    0,    1,    2,    1,    1,    1,    1,    2,    2,    1,
    2,    3,    1,    1,    3,    3,    1,    2,    3,    1,
    2,    0,    2,    1,    1,    0,    1,    1,    1,    3,
    2,    3,    0,    1,    3,    4,    3,    1,    3,    4,
    3,    0,    1,    3,    3,    1,    1,    0,    1,    1,
    4,    0,    4,    0,    2,    0,    2,    0,    9,   10,
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
    1,    3,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    1,    1,    5,    6,    3,    4,    5,    6,    3,    4,
    3,    4,    3,    4,    4,    5,    4,    4,    3,    4,
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
    1,    1,    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  561,
  562,  563,    0,  618,  729,  730,  734,  735,  472,    0,
    0,    0,    0,    0,    0,  407,  573,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  896,    0,   22,  894,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  486,    0,    0,   38,   35,   37,    0,   36,
    0,  977,  979,  981,  980,  986,  982,  989,  987,  983,
  985,  988,  984,    0,    0,    0,    0,   33,   69,   67,
  969,  978,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  647,    0,    0,    0,  679,
    0,  971,    0,    0,    0,    0,  108,  976,  567,    2,
    0,    0,    0,    0,  437,  438,  439,  440,  441,  442,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
  453,  454,  455,  456,  457,  458,  459,  460,  461,    0,
  435,  436,    0,  962,    0,  414,  966,    0,  411,  968,
    0,  681,    0,  410,  972,    0,  571,  892,    0,  151,
  104,    0,  105,    0,  218,  958,  768,    0,  103,  146,
  965,    0,    0,   99,    0,  106,  898,    0,  100,    0,
  101,    0,  102,  975,  147,  107,  626,  627,    0,  897,
  895,    0,   75,    0,  963,    0,  961,  494,  487,    0,
  493,  498,  488,  489,  496,  490,  491,  492,  497,    0,
   31,    0,   40,  755,   39,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  902,    0,  883,  641,  641,  642,  652,  654,  951,  655,
    0,  656,  646,  642,    0,    0,    0,    0,    0,    0,
    0,  590,  579,    0,  582,  584,  585,  586,  587,  593,
  594,    0,  970,  680,    0,  109,    0,  110,    0,  111,
    0,    0,  619,    0,  617,    0,    0,    0,    0,  473,
    0,    0,  377,    0,  221,    0,  417,  408,  470,  471,
    0,    0,    0,    0,    0,    0,  426,    0,  383,  384,
    0,  577,  575,  893,    0,    0,    0,    0,    0,    0,
    0,  154,  133,    0,   97,    0,    0,  112,    0,  122,
  121,    0,    0,    0,    0,    0,    0,  954,   76,    0,
    0,    0,  731,  732,  495,    0,  953,  875,  874,  873,
    0,    0,  876,  877,    0,    0,  879,  356,  357,  351,
    0,  355,  871,  878,    0,  758,    0,    0,   28,    0,
    0,    0,   72,   70,  973,   68,   34,    0,    0,    0,
  760,    0,   53,    0,    0,    0,    0,    0,  569,    0,
    0,    0,    0,  861,  889,    0,    0,  863,    0,    0,
  890,    0,  884,    0,    0,  865,  866,  867,  885,  886,
    0,  887,  888,    0,    0,    0,  891,    0,    0,    0,
    0,    0,    0,  765,  858,  859,  860,  864,  844,    0,
  843,    0,    0,  847,  905,  906,  846,  840,  841,  842,
  845,  903,  904,  926,  927,  930,  931,  649,  651,    0,
  658,    0,  742,    0,    0,  659,    0,  645,    0,  616,
    0,  591,    0,  588,    0,  589,  614,    0,  609,    0,
    0,  583,  769,  738,    0,    0,    0,    0,    0,    0,
    0,    0,  623,  620,    0,  186,    0,    0,  130,  478,
  173,  174,    0,  476,    0,    0,    0,    0,  479,    0,
  474,    0,    0,  421,  423,  422,    0,  424,    0,  418,
  219,    0,    0,  272,    0,    0,    0,    0,  430,  431,
    0,  463,    0,    0,    0,    0,    0,    0,    0,  388,
  387,  482,    0,    0,  150,    0,    0,  159,    0,  158,
    0,  165,  148,    0,  163,  166,  167,  155,    0,    0,
  517,  553,  520,    0,  554,    0,    0,  521,    0,    0,
    0,  544,  557,  516,  539,  518,  519,  514,  501,  377,
  499,  500,  502,    0,    0,  511,  512,  513,  515,    0,
    0,    0,    0,    0,  113,    0,  114,    0,  125,    0,
    0,  202,    0,    0,  375,    0,  221,    0,    0,   84,
    0,    0,  750,    0,    0,    0,    0,    0,  831,  832,
  833,    0,    0,    0,  771,  775,  776,  777,  778,  779,
  780,  781,  782,  783,  784,  785,  880,  881,  882,  872,
    0,    0,    0,  757,   29,    0,    0,   45,   73,    0,
    0,   27,    0,  763,    0,    0,    0,  726,    0,    0,
    0,    0,    0,  568,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  326,  974,    0,    0,    0,
    0,    0,    0,    0,  641,    0,    0,    0,    0,    0,
    0,    0,  967,  662,  565,    0,  900,  564,    0,  592,
    0,  600,  596,  595,    0,    0,    0,  611,  599,    0,
    0,  736,  737,    0,    0,    0,  375,    0,    0,    0,
    0,    0,    0,  131,    0,    0,    0,    0,    0,    0,
  377,  420,    0,  377,  378,  425,  509,    0,  274,    0,
  268,  270,  375,  241,  959,    0,    0,  377,    0,    0,
  210,  375,    0,    0,    0,    0,  246,  240,  243,  244,
  245,  429,  468,  467,  428,  427,    0,    0,    0,    0,
    0,    0,  386,    0,  377,    0,  156,  160,    0,  164,
    0,    0,  540,  541,  558,    0,  555,    0,    0,  547,
  545,  546,    0,    0,    0,    0,  510,    0,    0,    0,
    0,  548,    0,    0,  375,    0,  201,    0,  204,  206,
  207,    0,    0,    0,    0,    0,  221,   79,    0,    0,
   78,  528,  955,  529,    0,    0,    0,  774,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  353,  359,
  360,  358,  759,    0,    0,    0,    0,    0,    0,    0,
    0,   54,   62,   64,    0,    0,   71,    0,    0,    0,
  762,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  943,  944,  945,  946,  947,  948,  949,
  950,    0,    0,    0,    0,    0,    0,  936,    0,    0,
    0,  677,    0,    0,  856,  857,  924,    0,    0,  852,
  957,    0,    0,    0,    0,    0,  650,    0,  862,  767,
  766,  653,    0,    0,    0,    0,  667,  668,    0,    0,
  660,  663,  566,  901,  601,  610,  605,  604,    0,  615,
  770,  739,  740,  375,    0,    0,  643,  625,  624,    0,
    0,  633,  182,  176,  183,    0,  179,    0,    0,  178,
    0,  189,  170,  169,  899,  172,  171,  480,    0,    0,
    0,  419,  273,    0,  212,  377,  214,    0,  378,  209,
  215,    0,    0,    0,    0,  432,  433,  377,  375,  378,
    0,    0,    0,  404,    0,  197,  198,  157,    0,    0,
    0,    0,    0,    0,  556,    0,    0,  377,  378,    0,
  505,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  199,    0,    0,    0,    0,  364,
    0,   83,    0,    0,    0,    0,   88,   85,    0,    0,
    0,  733,  960,    0,    0,  786,    0,    0,    0,  812,
  811,    0,    0,    0,  819,  834,  829,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  773,    0,   66,
   58,   59,   65,   60,   61,    0,    0,   23,    0,    0,
    0,   43,    0,    0,    0,  744,  764,    0,  727,    0,
  743,    0,    0,    0,    0,    0,    0,  221,    0,  907,
    0,    0,  922,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  932,    0,  942,    0,    0,
    0,    0,    0,    0,  923,  868,    0,    0,    0,    0,
    0,    0,  682,    0,    0,    0,    0,    0,  607,  608,
  612,    0,  200,    0,    0,  637,    0,    0,    0,    0,
    0,    0,  190,    0,    0,  415,  224,  225,  226,  227,
  228,  229,    0,    0,    0,  277,    0,  222,    0,    0,
    0,    0,  247,    0,  464,  465,  434,    0,    0,    0,
    0,    0,    0,  377,  484,  378,  168,  956,    0,    0,
  523,    0,  522,  559,    0,  551,    0,    0,    0,  503,
    0,  531,    0,    0,  533,  536,  538,  375,    0,    0,
    0,  120,    0,  205,  377,    0,    0,  376,    0,   98,
    0,  118,    0,   94,    0,    0,    0,    0,   87,    0,
  827,  828,    0,    0,    0,    0,    0,    0,  830,    0,
    0,    0,    0,    0,    0,    0,  820,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   57,   63,   55,   56,   24,  748,  749,    0,
  746,  753,  754,  751,    0,    0,  725,    0,    0,  913,
  912,    0,    0,  909,  908,    0,  917,  916,  915,  914,
  911,  910,    0,    0,  938,    0,    0,    0,  937,    0,
    0,    0,  675,    0,    0,  670,    0,  671,    0,  688,
  684,  686,    0,    0,    0,    0,    0,  376,  629,  630,
  644,    0,  184,  180,    0,    0,  195,  191,    0,  378,
  280,    0,    0,  378,  278,    0,    0,    0,  232,    0,
    0,  248,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  481,  525,    0,  526,    0,  135,    0,    0,    0,
  139,  141,  142,  143,  506,  508,  504,  535,    0,    0,
  378,    0,    0,    0,    0,  361,    0,  365,  377,   89,
    0,    0,   91,    0,    0,    0,    0,    0,    0,  836,
  835,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   41,
    0,    0,  728,  220,  925,  918,  919,    0,  920,  934,
  933,    0,  939,  928,    0,  929,    0,  673,    0,  549,
    0,  695,    0,    0,  692,  696,  697,  699,  700,  701,
    0,    0,  639,  378,  362,  631,    0,  175,  192,  378,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  285,  290,  284,
    0,    0,    0,  294,  295,  296,  297,  298,  299,  301,
  302,  304,  305,  306,  309,  267,    0,    0,  208,  233,
    0,    0,    0,    0,  234,  237,    0,    0,  378,  402,
  403,  399,  398,  401,  400,    0,  524,  552,  144,  132,
  140,    0,  377,  264,  127,  378,    0,    0,  379,  366,
    0,    0,   92,   93,   80,    0,  798,  806,    0,  799,
  807,  802,  810,    0,    0,  800,  808,  801,  809,  795,
  803,  796,  804,  797,  805,  745,    0,    0,  935,  676,
    0,    0,    0,    0,    0,  709,  711,    0,  706,    0,
  265,  185,  416,    0,  312,    0,    0,  315,  316,    0,
  308,    0,    0,    0,    0,  307,    0,  338,  339,  293,
  300,    0,  303,  286,    0,  291,    0,  342,  292,  211,
  216,  235,    0,    0,  238,    0,  250,  252,  406,  378,
    0,  381,    0,    0,    0,  371,  372,    0,    0,   77,
   95,    0,    0,  921,    0,    0,  698,  693,  710,    0,
    0,  283,    0,  310,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  333,  334,  350,    0,    0,    0,
    0,  345,  343,  239,    0,    0,    0,    0,    0,  266,
  378,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  717,    0,    0,    0,  282,  279,  311,    0,    0,
    0,    0,  314,    0,    0,    0,    0,  322,    0,  325,
    0,  349,  348,  964,  347,    0,    0,    0,    0,    0,
  249,    0,  256,    0,  255,  382,  374,    0,    0,  367,
  119,    0,    0,    0,  719,  708,    0,    0,  720,    0,
  716,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  336,  335,  344,  346,  236,    0,    0,  260,  259,
  258,    0,    0,  640,  838,  715,    0,  722,  328,    0,
    0,  313,  319,    0,    0,    0,    0,  337,  263,  261,
  262,    0,    0,    0,    0,    0,    0,  340,  317,    0,
    0,    0,    0,  721,  331,  318,  320,  837,  724,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1524,   27,   28, 1525,
   30,   31,   32,   33,   34,   35,   36, 1526,   38, 1527,
  633,  114,  398,  890, 1138,  115,  667,  891,  116,  402,
  672,  140,  117,  118,  253,  964,  130,  892,  893,  193,
  222,  119,  894,  120,  404,   79,  233,  369,  452,  628,
 1075,  387,  850,  599, 1490,  827,  629,  630,  631, 1077,
  234, 1328,   71,  183,   72,  229, 1814, 1280,  214,  219,
  221,  223,  209,  201,  203,  216,  226,  147,  306,  308,
  310,  358,  615,  617,  395,  254,  777,  533,  643, 1012,
  362,  620, 1063, 1272,  566,  727, 1038,  518,  519,  548,
  354,  600, 1049, 1419, 1549, 1216, 1420, 1421, 1422, 1423,
 1424,  644,  210,  202,  350,  573,  351,  352,  574,  575,
  576,  577, 1247, 1006,  521,  522,  996,  994,  523, 1000,
  995, 1207, 1383, 1001, 1002, 1212,  364,  838,  839,  840,
  841,  779, 1019, 1397,  601,  781,  454,  843,  950,  542,
 1343, 1217,  455,  456,  457,  942,  458, 1398, 1399,  551,
 1555, 1707, 1233,  787,  788,  774,  789,  790,  791, 1708,
 1709, 1790,  332,  546, 1224, 1528,  189,  543,  544, 1225,
 1226, 1392, 1683, 1529, 1530, 1531, 1532, 1647, 1533, 1534,
 1535, 1536, 1537, 1538, 1539, 1540, 1541, 1542, 1543, 1544,
 1545, 1730, 1775, 1630, 1694, 1779, 1692,  459,  706, 1362,
  460, 1082, 1648, 1701, 1702, 1743,  390,  391,  461, 1068,
 1278, 1436, 1069, 1070,  281,  484, 1578,  373, 1198, 1376,
 1664, 1665, 1666, 1667, 1196,  341,  562,  805, 1245,  563,
  803,  217,   56,  336,  194,  154,  319,  180,  326,  186,
  765,  762,  327,  337,  795,  338,  558, 1030,  181,  462,
  320,  530,  564, 1246,   93,  192,  238,  602,  603,  767,
 1050,  768, 1051, 1052,  606,  607,  608,  609, 1043,  854,
  610,  611,  612,  820,  613, 1492,  817,   40,   41,   42,
  149,   58,  420,  197,  343,  293,  294,  295,  296,  297,
  298,  299,  300,  301,  733,  498,  979, 1201,  499,  500,
  738,   43,  153,  315,   44,  512,  513,  988, 1381, 1205,
 1506,  751,  992,  134,  720, 1503,  678,  478,  283,  135,
  136,  282,  485,  486,  487,  724,  965,  966,  967,  968,
 1190,  463, 1178, 1494, 1495,  464, 1497,  465,  466,  467,
 1722, 1620, 1723, 1724, 1725, 1771, 1815,  410, 1150,   45,
   46,  374,   47,   48,  506,  507,  899, 1146, 1330, 1334,
  411,  675,  207,  645,  646,  647,  648,  649,  650,  651,
  652,  653,  654,  655,  656, 1297, 1093, 1095, 1096,  949,
  705,  468,  469,  470,  471,  394,  231,  472,  473,  932,
  474,  475,  476,  477,  697,  936, 1179, 1174,  698,  122,
};
static short yysindex[] = {                                   2719,
 1836, -148, 3596,   78, 1170,   13, 4264,  -39, 4577,  561,
  506, 3800, -148, 1380,  482, 4468, 1047, 4468,    0,  318,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -100,    0,    0,    0,    0,    0,    0, 7646,
 4468, 4468, 4468, 4468, 4468,    0,    0,  -26, 1236, 4468,
 4468, 4468,  292, 4468,  474, 4468, 4468,    0, 4468, 4468,
    0,   71,    0,  524,    0,    0,  718, 4468,    0, 4468,
 4468, 4468,  748, 4468, 4468, 4468, 4468,  474, 4468, 4468,
 4468, 4468,    0, 4468,  693,    0,    0,    0, 1109,    0,
 1109,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  772,  779,  120, 1179,    0,    0,    0,
    0,    0, 4468, 4468, 4468,  895,  964,  982,  208, 1349,
  949,  143,  225,  601,  976,    0, 4468, 1156, 1331,    0,
 1105,    0, 4468, 4468, 4468, 4468,    0,    0,    0,    0,
 1254,  527, 1223,   25,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1118,
    0,    0, 1236,    0,  535,    0,    0, 1529,    0,    0,
 1126,    0,  584,    0,    0, 1151,    0,    0, 1175,    0,
    0, 1454,    0, 1322,    0,    0,    0, 1236,    0,    0,
    0,  399, 1529,    0,  578,    0,    0, 1377,    0, 1329,
    0, 1109,    0,    0,    0,    0,    0,    0, 1387,    0,
    0, 1280,    0, 1428,    0, 1262,    0,    0,    0, 1280,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2623,
    0, 4468,    0,    0,    0, 4649, 1391, 4432, 4468, 1125,
 1473, 1740, 1484, 1502, 4649, 1420, 4432, 1461, 1463, 1342,
    0, 9887,    0,    0,    0,    0,    0,    0,    0,    0,
 7572,    0,    0,    0, 1426, 4468, 1505, 1425,  549, 4468,
 1448,    0,    0, 1331,    0,    0,    0,    0,    0,    0,
    0, 4468,    0,    0, 1529,    0, 1377,    0, 1109,    0,
 9887, 1496,    0, 1485,    0, 1464, 1557, 1053,   25,    0,
 1633,  771,    0, 4468,    0, 1118,    0,    0,    0,    0,
 4468, 1520, 3132, 1470, 3485, 1889,    0, 7646,    0,    0,
  882,    0,    0,    0, 1876, 1876, 1236, 1569, 1236, 1384,
 1454,    0,    0, 4270,    0, 4468, 4468,    0, 1520,    0,
    0, 1672,  658, 1928, 4468, 1695, 4468,    0,    0, 1936,
  193, 4934,    0,    0,    0, 1528,    0,    0,    0,    0,
 1539, 1543,    0,    0, 1402, 1866,    0,    0,    0,    0,
 1922,    0,    0,    0, 1562,    0, 4468, 1559,    0, 4468,
 4468,  159,    0,    0,    0,    0,    0, 4468, 4468,  758,
    0, 4468,    0, 1685, 4468, 1963, 4649, 1606,    0, 4468,
 1969, 1975, 1980,    0,    0, 1982, 1985,    0, 1988, 1993,
    0, 1994,    0,    0, 2007,    0,    0,    0,    0,    0,
 2012,    0,    0, 8357, 2013, 2015,    0, 9887, 9887, 8455,
 4468, 2018, 2192,    0,    0,    0,    0,    0,    0, 1970,
    0,    0, 2014,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 9887,
    0, 3125,    0, 1746, 2019,    0, 2267,    0,  134,    0,
  -70,    0,  -80,    0,  -80,    0,    0, 2024,    0,  386,
 1643,    0,    0,    0, 2027,  198, 2020, 1520, 2034, 1811,
 1984, 2032,    0,    0, 1697,    0,  272, 1734,    0,    0,
    0,    0, 1236,    0, 1876, 1876, 1876, 1876,    0, 2040,
    0, 1807, 1808,    0,    0,    0, 1795,    0, 4270,    0,
    0, 4270, 1636,    0, 1529, 1834, 4468,11071,    0,    0,
  586,    0, 4468, 1057, 1126, 1704, 1673, 1645,    0,    0,
    0,    0,  725, 1727,    0, 1670,  474,    0, 1236,    0,
 1857,    0,    0, 1384,    0,    0,    0,    0,  230, 1691,
    0,    0,    0, 1693,    0, 1737, 2102,    0, 1800, 2104,
  576,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2056,  162,    0,    0,    0,    0,  205,
 2113, 2115, 2104, 1529,    0, 1838,    0, 1893,    0, 1236,
 1236,    0, 3132,  584,    0, 1775,    0, 1751, 2132,    0,
 6272,  786,    0, 1794, 1805, 2149, 5409, 2156,    0,    0,
    0, 5164, 9624,  556,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1731, 1866, 4468,    0,    0, 4030, 1761,    0,    0, 1735,
 4432,    0, 1886,    0, 1753, 1380, 2166,    0, 1886, 4030,
 1888, 1891, 4468,    0,  845, 8685,  437, 4468, 9887,  926,
 1199, 1418, 2900, 9887, 5409, 3125, 1758,  777, 9887, 8685,
 1040, 1040, 1380, 2272, 2172,    0,    0, 8924, 4468, 9887,
 9887, 9887, 9887, 9887,    0, 9887, 1011, 2349,  632, 1762,
 8685, 4468,    0,    0,    0,  999,    0,    0,  537,    0,
 1767,    0,    0,    0, 4468,  806, 4468,    0,    0, 4468,
 4468,    0,    0, 8685, 1953, 3132,    0, 1714, 9887, 4468,
 1777, 1236, 1236,    0,  -31, 1670, 1670, 1778,  474, 1053,
    0,    0, 2201,    0,    0,    0,    0, 1968,    0, 4468,
    0,    0,    0,    0,    0, 1979, 2217,    0, 1958, 1977,
    0,    0, 1915, 1923, 1109, 1995,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  474, 4468,11152, 1880,
 1890, 1894,    0,  474,    0, 1503,    0,    0, 1854,    0,
  114, 1861,    0,    0,    0,  474,    0, 2102,  104,    0,
    0,    0, 1987,  104, 1870,  104,    0, 2235,  474,  474,
  474,    0, 1520, 4468,    0, 1933,    0, 1757,    0,    0,
    0,  882, 1862, 4468, 1787,  136,    0,    0, 2256, 2029,
    0,    0,    0,    0,  193, 4468, 1892,    0, 1892, 4107,
  418, 9887, 9887, 6052, 2261, 1307, 6306, 9887, 6390, 1100,
 6644, 6898, 9007, 6982, 7236, 7490, 5409, 5409,    0,    0,
    0,    0,    0, 4468, 4468, 4468, 4468, 4468, 4468,  177,
 2262,    0,    0,    0, 4030, 1920,    0,   -2, 1954, 4468,
    0,  601, 8685, 1954, 2268, 2262, 4030, 3450, 2009, 9887,
 9887, 2043, 9887, 2289, 9887, 2292, 2374, 9887, 9887, 9887,
 9887, 9887, 9887,    0,    0,    0,    0,    0,    0,    0,
    0, 2021, 1510,  462, 9887,  810, 8685,    0, 5409, 1901,
 3125,    0, 2294,  601,    0,    0,    0, 3125, 1814,    0,
    0, 2053, 1040, 1040, 1130, 1130,    0,  880,    0,    0,
    0,    0,  632, 2300, 2299, 1092,    0,    0, 5409, 2017,
    0,    0,    0,    0,    0,    0,    0,    0,  642,    0,
    0,    0,    0,    0, 1859, 1862,    0,    0,    0, 2311,
 1851,    0,    0,    0,    0, 1868,    0, 1876, 2095,    0,
  -31,    0,    0,    0,    0,    0,    0,    0, 2322, 5409,
 1555,    0,    0, 2064,    0,    0,    0, 1555,    0,    0,
    0, 1109, 1109, 1867, 2325,    0,    0,    0,    0,    0,
  -72,  381,  915,    0, 2105,    0,    0,    0, 4468,   78,
 1874, 1960, 1968, 2331,    0, 1670, 1884,    0,    0, 1319,
    0, 2317, 4468, 1325,  474, 2335, 2339, 2343, 2135,  584,
 2064, 1876, 1734, 3132,    0, 1727,  601, 1955, 1964,    0,
  607,    0,  474, 2026, 2098, 6272,    0,    0,  474, 1931,
 2109,    0,    0, 2370, 2373,    0, 2771, 3125, 2388,    0,
    0, 3125, 2390, 9237,    0,    0,    0, 2079, 2395, 3125,
 2396, 1879, 2399, 3125, 2400, 9887, 9887, 2261, 9887, 9335,
 2401, 3125, 2410, 2413, 3125, 2415, 9887, 3125, 2418, 3125,
 2420, 2425, 3125, 2427, 2428, 3125, 2429,    0, 2208,    0,
    0,    0,    0,    0,    0, 2144, 4030,    0, 4030,  177,
 2262,    0,  378,  378,  117,    0,    0, 7572,    0, 1909,
    0, 2268, 2262, 2432, 3450, 2473, 2596,    0, 2605,    0,
 2705, 9887,    0, 2763, 2884, 2918, 2957, 2965, 3017, 9887,
  474, 8685, 3125, 2038, 8685,    0, 9887,    0, 2171,  481,
 9887, 8685, 9887, 9887,    0,    0, 1092, 2439, 8685, 4468,
  632, 2096,    0, 2097, 2099, 2140, 2214, 2158,    0,    0,
    0, 2064,    0, 1955,  508,    0, 2068, 1236, 2106, 1670,
 2101, 1670,    0, 5409,  626,    0,    0,    0,    0,    0,
    0,    0, 2048, 2228, 2064,    0, 9887,    0, 2216, 2100,
 1867,   71,    0, 5409,    0,    0,    0, 2119, 2123, 2127,
 2133, 2137, 2141,    0,    0,    0,    0,    0, 2480,   78,
    0, 1778,    0,    0,  104,    0, 1555, 2150,  104,    0,
  104,    0, 1968, 2488,    0,    0,    0,    0,  882, 2228,
 1670,    0, 1734,    0,    0, 7572, 2254,    0,  552,    0,
 2311,    0, 2493,    0, 2498, 1236,  440, 2499,    0, 1236,
    0,    0, 9887, 2117, 2117, 1380, 2502, 1916,    0, 2117,
 2117, 9887, 2117, 2117, 3241, 3125,    0, 1945, 9887, 3125,
 2117, 2117, 2117, 2117, 3125, 2117, 2117, 2117, 2117, 2117,
 2117, 2159,    0,    0,    0,    0,    0,    0,    0, 2114,
    0,    0,    0,    0, 1746, 8685,    0, 3450, 2432,    0,
    0, 4270, 2514,    0,    0, 3085,    0,    0,    0,    0,
    0,    0, 3101,  372,    0, 8685, 2248, 2126,    0, 8685,
 3216, 1926,    0, 1798, 3125,    0, 1940,    0, 1092,    0,
    0,    0,  632, 4774, 5409, 2162, 2228,    0,    0,    0,
    0, 1876,    0,    0, 1236, 1503,    0,    0,  783,    0,
    0, 4468,10973,    0,    0, 3125, 2284, 2216,    0,  764,
 4468,    0, 2229,  814, 2125, 2227, 2128, 2270, 2234, 2273,
 2064,    0,    0, 2532,    0, 2535,    0, 2236, 2284, 2150,
    0,    0,    0,    0,    0,    0,    0,    0, 2064, 1727,
    0, 1503, 2105, 2266, 2202,    0, 1862,    0,    0,    0,
 2218,  -92,    0, 1968, 2219, 3125, 2550, 2553,  601,    0,
    0, 2554, 2559, 3125, 2560, 2566, 9887, 9887, 3125, 2568,
 2571, 2573, 2574, 2575, 2576, 2577, 2580, 2581, 2582,    0,
  378, 1762,    0,    0,    0,    0,    0,  474,    0,    0,
    0, 8685,    0,    0, 8685,    0, 1762,    0, 1521,    0,
 2345,    0, 2586, 2583,    0,    0,    0,    0,    0,    0,
  556,  609,    0,    0,    0,    0, 1670,    0,    0,    0,
    0, 4270, 1472,  -74, 2590,  352, 2593, 2295, 9887, 2594,
 2600, 2598, 2602, 2610, 2611, 2305, 2617,    0,    0,    0,
 2347,10024, 2619,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2638, 4468,    0,    0,
 2333, 2348, 4468, 1867,    0,    0, 1109, 4468,    0,    0,
    0,    0,    0,    0,    0, 2228,    0,    0,    0,    0,
    0, 2228,    0,    0,    0,    0,  811, 1762,    0,    0,
 9887, 1236,    0,    0,    0, 1236,    0,    0, 9887,    0,
    0,    0,    0, 3125, 3125,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2017, 2642,    0,    0,
 2017, 5409, 4468,  975, 4774,    0,    0, 2320,    0, 2649,
    0,    0,    0,  171,    0, 9574, 4468,    0,    0, 9887,
    0,  542, 2351, 5409, 2561,    0, 5409,    0,    0,    0,
    0, 1226,    0,    0, 1058,    0, 2244,    0,    0,    0,
    0,    0, 1867, 1109,    0, 2313,    0,    0,    0,    0,
 2105,    0,  811, 2653, 1092,    0,    0, 2017, 3125,    0,
    0, 1798, 2158,    0, 2158,  556,    0,    0,    0, 3539,
 1555,    0, 2639,    0, 2899, 9804, 1764, 9887, 1226,  827,
 9887, 2641,  851, 1204,    0,    0,    0, 4468, 4468,   78,
  190,    0,    0,    0, 2313,  458, 1867, 2318, 2319,    0,
    0, 1092, 2662,  811, 2362, 2158, 2666, 1762, 2162, 2162,
 4468,    0, 2667, 2668, 1330,    0,    0,    0, 7814, 2301,
 2670, 1226,    0, 1782,  229, 2280, 3125,    0, 2412,    0,
 1226,    0,    0,    0,    0,10973, 1058, 1867,  797,  797,
    0, 2416,    0, 2276,    0,    0,    0, 1092,  811,    0,
    0, 2017, 2332, 2332,    0,    0, 3539, 2681,    0, 4468,
    0, 2272, 1950,  382, 2665, 1609, 1226, 2435, 2421,10973,
10973,    0,    0,    0,    0,    0, 2269,   34,    0,    0,
    0, 1810, 2158,    0,    0,    0, 4468,    0,    0, 1226,
 2685,    0,    0,  196, 4468,10973, 2431,    0,    0,    0,
    0, 5409, 2162, 2690, 2695, 1952,10973,    0,    0,10973,
  556, 2332, 4468,    0,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  202,  994,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  161, 5714,    0,    0,    0,    0,    0, 2739,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  431,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1052,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2404,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2361,    0,    0,    0,  141,    0,
  141,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2366,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1163,    0,    0, 8044, 5798,    0,    0,    0, 1196,    0,
    0,    0, 1919,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  415,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -29,    0,    0,
    0,    0,  664,    0,    0, 1297,    0,    0,    0,    0,
    0, 1817,    0, 5130,    0,    0,    0,    0,    0,    0,
    0,    0,  -29,    0,   35,    0,    0, 2711,    0,    0,
    0, 2490,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1302,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1315,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1077,    0,    0,    0,    0,    0,    0,    0, 4830,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  279,    0,    0, 1327,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -29,    0, 2711,    0, 2490,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1338,    0,
 2477,    0,    0,    0,    0, 1345,    0,    0,    0,    0,
    0, 2496,   36, 9069,    0, 1378,    0,    0,    0,    0,
  417,    0,    0,    0, 2307, 2307,    0,    0,    0, 1414,
 2212,    0,    0,    0,    0,    0,    0,    0, 2496,    0,
    0, 1440,    0,    0,    0,    0,    0,    0,    0, 4761,
 2384,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1450,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1456,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1468,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1792,    0,    0,    0,    0,    0,    0,
 2402,    0,    0,    0, 2537, 2893,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3028,
    0, 2046,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  457,    0,    0, 2443,    0,  317,    0,    0,    0,
    0,    0,  343,    0,  343,    0,    0,  414,    0,  589,
 1184,    0,    0,    0,    0, 1302,    0, 2496,    0,    0,
  110,  762,    0,    0,  132,    0, 3208,  200,    0,    0,
    0,    0,    0,    0, 2307, 2307, 2307, 2307,    0,  294,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2512,    0,    0,    0,    0,    0,
 2477,    0,    0,  951,    0, 2316,    0,    0,  691,    0,
    0,    0,    0,  231,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1492,    0,    0,    0,    0, 1942,  930,
    0,    0,    0, 5283,    0,    0, 4755,    0,    0, 5525,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3054, 3358,    0,    0,    0,    0, 5670,
 1499,    0, 5525,  -29,    0,    0,    0,    0,    0,    0,
    0,    0,   36, 1300,    0,    0,    0,    0, 2372,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1283,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  155,    0,    0, 5714,    0,    0,  155,    0,
    0,    0,    0,    0,10117,    0,10117,    0,    0,10117,
10117,10117,    0,    0,    0, 2326,    0,    0,    0,    0,
 4010, 4366, 8127,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  798,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  720,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   36,    0, 1598,    0,    0,
  432,    0,    0,    0, 3246,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  502,    0,    0,
    0,    0,    0,    0,    0, 2747,    0,    0,  799, 1719,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  596,
  686,  754,    0,    0,    0, 2263,    0,    0,    0,    0,
    0, 2224,    0,    0,    0,    0,    0, 4755,    0,    0,
    0,    0,  251,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2496,    0,    0,  345,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6864,    0,    0,    0,  265,
    0,    0,    0,    0, 2481,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1506,
 1507,    0,    0,    0,    0,    0,    0,    0, 1534,    0,
    0, 8044,    0, 1534, 1535, 1537,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  337,    0,    0,10117,    0,    0,    0, 1666,    0,    0,
    0, 3384, 4501, 4857, 3519, 3875,    0,    0,    0,    0,
    0,    0,    0,  939, 2344, 1864,    0,    0,    0,  894,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  163,
    0,    0,    0,    0,    0,    0,    0, 2307, 2334,    0,
 3439,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2520,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1309,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1538,    0,    0,    0,    0,    0,
    0,    0, 2393,    0,    0,    0,    0,    0,    0,    0,
    0, 1361,    0,    0,    0,    0,    0,    0,    0, 1300,
 2520, 2307, 1574,   36,    0, 2530, 8044, 1579,  166,    0,
    0,    0,    0, 6864,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3774,    0,    0,
    0, 4265,    0,    0,    0,    0,    0,    0,    0, 6002,
    0, 6387,    0, 6979,    0,    0,    0,    0,    0,    0,
    0, 7506,    0,    0, 9004,    0,    0,10102,    0,10319,
    0,    0,10403,    0,    0,10409,    0,    0, 8675,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1506,
 1602,    0, 2359,  212,    0,    0,    0,    0,    0,    0,
    0, 1610, 1615, 1617,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2368,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2454, 1689,    0, 2779,
    0, 2464,    0, 2476, 2478,    0,    0,  130,    0,    0,
    0, 2520,    0, 1579,    0,    0, 3453,    0,    0,    0,
 2369,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 4879,    0, 2555,    0,    0,    0,  127,    0,
 1309, 2404,    0,    0,    0,    0,    0,  902,  963,  978,
  992, 1009, 1064,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  795,    0,    0,
    0,    0, 3296,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1637,    0,    0,    0,    0,    0,    0,    0,
 1641,    0,    0,    0,    0,    0,  146,    0,    0,    0,
    0,    0,    0,    0,    0, 8127,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10432,    0,10552,    0,10585,
    0,    0,    0,    0,10648,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1683,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1707,    0,    0,    0, 2070,    0,
    0,    0,    0,    0,    0,  387,    0,    0,    0,    0,
    0, 2307,    0,    0,    0, 2962,    0,    0,    0,    0,
    0,    0, 2516,    0,    0, 2787, 1599, 1489,    0, 2477,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2520,    0,    0,    0,    0,    0,    0,    0, 1694,  940,
    0,    0,    0,    0,    0,    0,    0,    0, 2520, 2530,
    0,  350,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  448,    0,10698,    0,    0,10117,    0,
    0,    0,    0,10830,    0,    0,    0,    0,10841,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  212, 1160,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1049,    0, 2454,    0,
 1295,    0,    0, 1015,    0,    0,    0,    0,    0,    0,
  219, 2790,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1642, 1728,    0,    0,    0,    0,    0,    0,    0,
    0, 2524,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1743,    0,    0,  101,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  133,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,10864,10999,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1264,    0,    0,    0,
  769,    0,    0,    0,    0,    0,    0, 2793,    0,    0,
    0,    0,    0, 2776,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2777,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2539,    0,    0,    0,
    0,    0, 1743,  140,    0,  129,    0,    0,    0,    0,
    0,    0,    0,  223,  594,    0,    0,  150, 2787,    0,
    0,    0,  305,    0,  282, 3270,    0,    0,    0, 2790,
    0,    0,    0,    0,    0,  145,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  157,    0, 1309,  167,  466,    0,
    0, 2454, 1842,    0,    0,  182,    0, 1049,  308,  121,
 1335,    0,    0, 2799,    0,    0,    0,    0,    0, 2784,
  217,    0,    0,    0, 2545,    0, 2791,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1743,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  738,    0,    0,
    0,  769,  955, 2810,    0,    0, 2790,    0,    0,    0,
    0, 2808,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2454,  282,    0,    0,    0,    0,    0,    0,    0,
 2800,    0,    0,    0,    0,    0, 8264,    0,    0,    0,
    0,    0,  121, 2819,    0,    0,    0,    0,    0,    0,
  852, 2810,    0,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2864,    0,    0, 2866,
    0,    0,    0,    0,    0,    0,    0,    4,    0, 2867,
    2,  260,   21, -556, 1729,  280, -208,  -45, 2859, 2606,
    0,  -28,    0, 2614,  -91,  -40,    0,  631, -403,   22,
  -25,   14, -188, 2616, 2211,    0,    0,    0, 1498,    0,
    0,  -36, 1803, -534,   -1, -868, 2257, 2036, -288, 1812,
 2803, -277,    0,  -53,    0, -462,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -572,  -96, 1818, -296,  709, -927,
    0,    0,    0,    0, -332, -117, -239, 1825, -328,  -49,
    0, -516,    0,    0, 1476, -962,    0, 1480,    0,    0,
 2585, -609,    6,    0,    0,    0,    0, 2547,    0, 2337,
 2592, -211, 1869, -659,    0,    0,    0, -694,    0,    0,
    0,    0,    0,    0, 1911,    0, 2607, 2173, 1881, 2588,
 2595,    0, 1306,    0, -508, 2152,  -35, -608,-1228,    0,
    0,  -20, -916, -913, -900, -233, -898,    0, 1545, -936,
    0, 1243,-1138, 1549,    0, 2398,    0,    0,    0, 1247,
 1249, 1214,  180, -251, -261, -375,    0,    0, 2197,    0,
 1744,    0,    0, -548,    0,    0,    0,    0, -193,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1336, -553,    0,    0,  785,    0,-1126,
  836, 1165, 1324,    0, 1227,    0,    0, 2587, -112, 1990,
 1774, 1601,    0,-1025, -803, -816,    0, -487, -267,-1374,
    0, -397, 1318,  346, -167, -465, -698, -910,-1209,    0,
    0,    1,    0,    0,    0,    0,    0, 2644,    0,    0,
    0,    0, 2657, 2437,    0,    0,    0,    0,    0,   -9,
 2674,    0,    0,    0,    0,  -15,    0,    0,    0, -319,
 2161, -304, 1736, -422,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2381,    0,    0, 2178,    0,    0,    0,
 2591, 2990,    0,    0,    0,    0,    0, 2720,    0,    0,
    0,    0, 2522,    0, 2510,    0,    0,    0, 2282,    0,
    0,    0, -370,    0,  598,    0, 2274,    0,    0,    0,
    0,    0,    0, -557, -970, -712,  807, -150, 1478,    0,
 2880, 1178,    0, 2298, -618, 2302,    0, -766, 2059,    0,
    0, -125, -670,    0, 1415,-1164,    0,-1159,-1135,-1094,
 1532,    0, 1268,    0, 1310,    0, 1218,    0,    0,    0,
    0, 2541,    0,    0,    0, -309, 2371, 2145, -939,    0,
    0,    0, -678,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2762, 1080, 1944,    0, -468,
    0,    0,    0, -238,    0, 2676,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -115, 1878,    0,    0,
};
#define YYTABLESIZE 11660
static short yytable[] = {                                     121,
  143,  185,  121,   37,   76,   75,  142,  208,  148,  255,
  204,  393,  188,  567,  274,  277,  389,  304,  742,  275,
  278,  572,  200,  213,  205,  191,  537,  858,  239,  943,
  766,  778,  861,  769,  604,  243,  539,  218,  961,  780,
  182,  184,  187,  190,  142,  195,  244,  483,  748,  605,
  206,  184,  211,  250,  190, 1228,  142,  195,  997,  224,
  121,  981, 1367,  215,  143,  248,  728,  912,  235,  403,
  142,  237,  184,  246,  235,  211,  187,  190,  403,  142,
  195,  224,  121,  225,  190,  934,  245,  261,  220,  263,
  242, 1229, 1402,  634, 1218,  262,  851, 1219, 1148,  674,
  756, 1218, 1237,  305, 1219,  249,  529,  618,  504,  628,
 1220,  247, 1222,  206,  142,  206,  307, 1220,  902, 1222,
  309, 1258,   74,  905,  479,  983,  230,  211,  257,  703,
  366,  636,  705,  303,  190,  142,  224,  392,  986,  756,
 1183,  756,  285, 1066,  756,  944,  328,  393, 1046,  691,
  628, 1041,  389,  628,  747, 1275,  257, 1040,  842,  756,
  574,  707,  635,  259, 1014,  363,  253,  230,  628,  257,
  230,  355,  257, 1021, 1253, 1074,  505,  990,  726,  273,
  756,  703,  272,  756,  756,  230,   90,  257,  703,  754,
  636,  705,  756,  757,  758,  759, 1187,  257,  756,  475,
  257,  574,  671,  329, 1331,  273,  681,  253,  691, 1496,
  253,  530,  669,  747, 1498,  257,  396,  510,  702,  574,
 1137,  635,  368, 1576,  363,  253, 1061,  399,  604,  388,
  405, 1681,  970, 1747,  269,  490,  399,   74, 1499, 1741,
  703,  741,  206,  605,  829,  572,  142,  958,  405,  121,
  136,  259,  826, 1438,  786,  142,  745,  405,  475,  702,
  574,  497, 1583, 1276,  276, 1238,  503, 1128, 1129,  811,
  530,  127, 1741,  392,   74,  327,  187,  702,  597, 1500,
  142,  368,  316,  505, 1149,  414,  151,  273,  538,  405,
 1028,  128, 1400,  477, 1417,  541,  731,  205, 1719,  554,
 1720,  812,  556,  823,  703,   86,   57,  707,   86,  136,
  568,  753,  570,  242,  206,  614,  626, 1143, 1412, 1651,
  123,  206,  703,  206,  998,  206,  622, 1627,  559, 1180,
  317, 1335,   80, 1810,  729,  227,  624,  597, 1140,  152,
 1218, 1760,  602, 1219,  126,  703,  190,  195,  707,  196,
 1152, 1154,  477,  291,  242,  142, 1220,  187, 1222,  668,
  661,  141,   94,  703, 1472,  388,  707,  732,  665, 1239,
 1628,   76,  228,  503,  668, 1202,  150,  678,  616,  673,
  678,  269,  196,  679, 1677, 1811,  707,  142,  399,  268,
  190,  405,  359, 1487, 1427,  678, 1047,  999,  142, 1584,
 1215,  602,  142,  126,  242,  190,  230,  142,  196,  269,
  148, 1580, 1479,  603,  634, 1655,  483,  756, 1813,  756,
 1236, 1800,   74,  393, 1369,  236,  756,  628,  881,  737,
  622,  638, 1629, 1144,  257,  242,  257,  682,  825,  451,
  273,  707,  318,  257,  253,  707,  253,  703,  806,  628,
 1496, 1711,  483,  253,   90, 1498,  741,  475, 1086, 1434,
  705, 1400, 1511,  705, 1009,  254, 1546, 1011, 1547,  703,
 1018,  636,  603,  634,  909,  483,  242,  723,  914, 1499,
  691, 1017,  403,  756,  508, 1332,  755,  483,  532,  622,
  638,  532, 1778, 1746,  405,  123,  123,  741, 1281, 1817,
  741,  532,  635, 1574,  242,  475,  254,  707, 1035,  254,
  483, 1363,  782, 1384, 1704,  741,  703,  756, 1363, 1573,
 1500,  747,  756,  756,  254,  703,  242,  267,  136, 1073,
  136, 1606,  808,  242,  257,   73,  702,  775,  206,  756,
  752,  851,  532,  775,  253,  532,  756,  982, 1008,  882,
  368,  477,  212,  368,  329,  258,  257, 1076,  702, 1731,
  532,  271,  532,  257,  957,  807,  253,  242, 1751,  703,
 1430, 1005,  705,  253, 1005, 1585, 1621, 1081,  530,   76,
  756, 1059, 1623,  836,  837,  636,  705,  205,  613,  691,
 1147,  271, 1415,  369, 1269,  391,  574,  257, 1339,  477,
  271,  756, 1773,  691, 1389,  702, 1489,  253,  974, 1786,
  574,  628,  670,  206,  702,  505,  635,  475,  725,  363,
  597,  703,  853,  632, 1404, 1298,  327,  883,  136,  257,
 1136, 1658,   76,  852,  906,  703,  661,  369, 1003, 1004,
  756,  880,  668,  678,  678, 1589,  252,  613, 1662,  372,
   73,  828,  369,  206,  391,  597,  405,  257,  702, 1429,
  632,  405,  368,  385, 1632, 1210, 1390,  253,  703,  483,
  405,  963,  702,  190,  368,   86,  368,  703,  211,  877,
  483, 1240,  271,  597,  602,  389,  597,   73, 1024, 1478,
 1508,  703,  126,  916,  707, 1130,  279,  196, 1133,  951,
  703, 1718,  597,  736,  707,  503,  497,  206,  980,  190,
  205,  477,  723, 1442,  396,  993,  993, 1473, 1726, 1227,
  741,  532,  385,  877,  772,  142,  707,  142,  597, 1271,
  206, 1235, 1710, 1324,  541, 1326,  206,  370, 1449,  913,
  206, 1717,  877,  254,  389,  254,  678,  602,  622,  313,
  602, 1257,  254,  390,  634,  603,  532, 1007, 1749,  532,
  206,  621, 1027, 1029, 1218, 1501,  602, 1219,   14, 1403,
  622,  638,  393,  707,  505,  532,  741, 1221,  125,  393,
 1220,  370, 1222, 1756, 1221, 1076,  312,  678,  206,  206,
  603,  329,  602,  833,  137, 1026,  370,  705,  223, 1270,
  356,  532,  621,  878, 1034, 1060,  137,  532,  632,  691,
  532, 1437,  390,  483, 1610, 1071, 1044,  877,  603,  493,
  621,  603, 1241, 1510,  142, 1474,  707, 1056, 1057, 1058,
   74,  493, 1416,  123,  142,   73,  357,  603, 1426,  223,
  339,  102,  223,  254, 1131,  360, 1083,  878, 1394, 1141,
 1663,  373,  821,  137, 1559,   76,  705,  223,  532,  391,
  279, 1153, 1134,  603,  503,  254,  878, 1736,  330, 1342,
  103,  104,  254,  105,  405,  190,  195,  405,  224,  121,
  106,  107,  108,  109,  110,  405,  111,  877,  112,  113,
  776, 1739,  373,  691, 1431,  373, 1172,  405,  405, 1750,
  124,  396, 1135,  280,  783, 1443,  254, 1132,  741,  514,
  373,  138,  532,  532,  483, 1360,  741,  340,  139,  532,
  822,  369,  385, 1184,  369, 1230, 1231,  385,  974,  542,
  613,   73,  494,  385, 1324,  961,  689,  532,  674,  138,
 1377,  878,  361,   14,  754,  741,  741, 1411,  741,  389,
  469, 1616,  691,  190,  622,  741,  741,  741,  741,  741,
  396,  741,  397,  741,  741,  613,  254,  532, 1067,  542,
  542,  532, 1186,  542, 1282,  495, 1617,  393, 1433,  674,
  453,  230,  674, 1646, 1607,  784, 1628,  495,  542,  482,
  542,  392,  391,  613,  469,  622,  613,  674,  138, 1611,
 1379, 1504, 1676, 1624,  496,  621, 1763, 1764,  395,  469,
 1380,  878,  613,  622,  690, 1363,  960,  390,  393,  511,
  542,  397,  604, 1221, 1690,  800,  785, 1693,  205, 1248,
 1329, 1329, 1333,  369, 1618,  396,  393,  605,  613,  232,
   76, 1249,  483, 1248,  877,  369, 1199,  369, 1629, 1507,
  392,  576,  960, 1264,  206,  690, 1355,  535, 1363, 1357,
  385,  206,  801,  394, 1368,  370,  102,  395,  370,  240,
  536, 1284,  377,  690,  137,  877,  223, 1288,  223,  621,
  689,  713,  389,  937,  938,  223,  714,  958,  877,  705,
 1668,  102, 1386,  251, 1388,  103,  104, 1200,  105,  691,
 1822,  621,  483,  786,  279,  106,  107,  108,  109,  110,
  576,  111,  877,  112,  113,  705, 1175, 1176,  198,  199,
  103,  104,  394,  105,  606,  705,  436,  405,  705,  405,
  106,  107,  108,  109,  110, 1735,  111,  705,  112,  113,
  560, 1551, 1581,   76,   76,  405, 1509,  910,  252, 1566,
  390,  561,  696, 1432,  256,  691,  701,  702,  704,  705,
  958,  257,  572, 1552,  691,  396,  802, 1572,  878, 1354,
 1553,  993,  373, 1787,  676,  606,  223,  370, 1776,  373,
  303,  190,  373,  598,  705,  279,  373,  542,  718,  370,
 1659,  370, 1575,  705,  373,  581, 1660, 1784,  223,  878,
  705,  677, 1821,  542, 1554,  223,  542,  542,  977,  542,
  373,  691,  878,  542, 1788, 1242,  542,  377,  705,  138,
 1801,  572,  260, 1804,  691,  978,  397,  939,  918,  542,
 1762, 1807, 1808,  691,  373,  542,  878,  705,  542,  223,
 1480,  393,  598,   73, 1483,  102, 1816, 1741, 1493, 1441,
   76, 1414, 1243, 1445,  581,  392,  674, 1819,  116,  674,
 1177, 1789, 1740,  691,  373, 1712,  674,  505, 1826,  674,
  102, 1827,  395,  674,  103,  104, 1661,  105,  674,  264,
  691,  674,  704,  451,  106,  107,  108,  109,  110,  691,
  111,  373,  112,  113,  694,  116,  578,  674,  396,  103,
  104,  705,  105,  373,  691,  373,  959,  542,  251,  106,
  107,  108,  109,  110,  354,  111, 1758,  112,  113,  709,
 1697,  674,  691,  704,  710,  674,  580,  394,  405,  542,
  116,  525,  690,  691,  674,  694,  542,  412,  694, 1673,
  542,  704, 1512, 1675,  413,  690,  265,  542,  993,  251,
  860,  674,  251,  694,  690,  578,  205,  503, 1329,  397,
  705, 1792, 1259,  190,  266,  542, 1609,  251, 1259, 1698,
  542, 1106, 1557,  354,  393,  321,  705,  409,  674,  705,
 1699,  526,  206,  333,  270,  580,  334, 1107,  392, 1622,
  674,  142, 1794, 1795,  482,  542,  412,  917,  674,  542,
 1716,  690,  933,  413,  507,  395,  505,  940,  941,  709,
  690, 1260, 1192,  161,  710,  527,  948, 1263,  952,  953,
  954,  955,  956,  322,  948,   96, 1193,  674,  674,  482,
  674,  335, 1108,  528,  102,  705,  409,  674,  674,  674,
  674,  674,  393,  674,  705,  674,  674, 1221,  144,  352,
 1194, 1828,  482,  507,  690,   44,  271,  511,  279, 1626,
  394, 1109,   98,  103,  104, 1656,  105,   52,  690,  133,
  102,   76,  161,  106,  107,  108,  109,  110, 1608,  111,
  145,  112,  113,  973, 1700,   77,   78,  705,  231, 1493,
  705,  162,  286,  761, 1793,  146,  503, 1715,  537,  103,
  104,  920,  105,  184, 1195,   42,   25, 1657,  352,  106,
  107,  108,  109,  110,   44,  111, 1493,  112,  113, 1633,
  761,  793,  302,  794, 1654,  598,   52,  311, 1110,  231,
 1625,   99,  231,  752,   48,  323,   50,  485,  951,  537,
  314,  100,  537,  142, 1715, 1670,  705,  231,  187, 1671,
  162,  713,  711,  324,  712,  705,  714,  537,  385,  537,
  598,  143, 1705, 1493,   42,   25,  101,  142,  331,  385,
 1087, 1088, 1092,  128,  342, 1100, 1102, 1104,  380, 1112,
 1115, 1118, 1120, 1123, 1126,  353, 1686,  325,  598,  537,
 1715,  598,  752,   48,  691,   50,  485,  632,  217,  385,
  704,   26,  664,  951,  206,  663, 1493,  598,   95,   49,
  704,  482,  694,  704,   51, 1493,   46,  190, 1156, 1157,
  505, 1159,  704, 1161, 1715,  694, 1164, 1165, 1166, 1167,
 1168, 1169,  128,  598,  694,  344,  129,  380,  632,  217,
  117,  632,  217, 1173, 1742,  941,  365,  143, 1493,   96,
  691, 1493, 1741,  142,  505,  505,  632,  217, 1097,  691,
   26,   97,  525, 1084, 1721, 1085, 1768, 1803,   49,  704,
  142,  718,  287,   51, 1493,   46,  771,  279,  704,  770,
  505,  694,   47,  571, 1098,  102,   98,  979,  184, 1744,
  694,  505,  363,  217,  505,  129,  198,  199,  143,  117,
   76, 1745,  979, 1769,  142, 1721,  869,  288,  718,  869,
  503,  142,  526,  372,  103,  104, 1770,  105,  499, 1798,
  922,  718,  704,  367,  106,  107,  108,  109,  110,  370,
  111,  516,  112,  113,  694,  289,  704,  375,  290,  368,
  517,   47,  251,  143,  503,  503,  527,  870,  694,  142,
  870, 1721,  217,  480,  291,   99,  537,  142,  869,  499,
  187,  488,  499,  371,  528,  100,  242, 1323,  231, 1325,
  503,  879,  537,  984,  662,  537,  537,  499,  537,  409,
  292,  503,  537,  251,  503,  537,  251,  187,  984,  709,
  101,  977,  400,  901,  710, 1083,  900, 1065,  537,  870,
 1064,  251,  948,  412,  537,  713,  711,  537,  712,  345,
  714,  516,  408,  187, 1305, 1306,  152, 1308, 1310,  413,
  517,  415, 1733,  713,  711, 1315,  712, 1072,  714, 1171,
  847,  954,  977,  977,  977,  977,  977,  977,  977,  713,
  711, 1192,  712,  417,  714,  418,  346,  419,  347,  376,
  977,  977,  977,  977, 1185, 1193,  482, 1184,  378,  379,
  380,  489,  911,  665,  915,  491,  242,  919,  921,  923,
 1346,  348,  132,  133,  492,  152,  537,  425, 1353, 1194,
  941,  501,  977,  941,  977, 1173,  383,  384,  242, 1361,
  941, 1364, 1365, 1036, 1037,  242,  349,  941,  537, 1203,
  124,  124, 1064, 1612,  665,  537,  377,  665, 1209,  537,
  385, 1208,   14,  524, 1251,  632,  537, 1250,  681,  532,
  713,  711,  665,  712, 1256,  714,  515, 1255,  273,  242,
  545,  102,  555, 1195,  537, 1396,  565,  632,  552,  537,
  569,  530,  713,  711, 1181,  712, 1101,  714, 1105, 1337,
 1113, 1116, 1336, 1121, 1124, 1127, 1451,  619,  625, 1184,
  103,  104,  681,  105,  537,  662, 1486,  623,  537, 1485,
  106,  107,  108,  109,  110,  627,  111,  681,  112,  113,
 1488,  431,  530, 1485,  482,  530,  713,  711,  657,  712,
 1799,  714, 1825, 1485,  666, 1741,  499,  433,  499,  658,
  530, 1446,  530,  659,  680,  499,  671,  683,  685,  667,
 1454,  378,  379,  380,  686,  198,  199, 1459,  499,  687,
  251,  688,  251,  667,  689,  713,  711,  690,  712,  251,
  714,  667,  691,  692,  381,  382,  436,  437,  438,  383,
  384,  439,  440,  709,  482,  958,  693,  667,  710,  442,
  443,  694,  699,  977,  700,  977,  977,  708,  447,  717,
  716,  709,  721,  977,  941,  719,  710,  735,  941,  666,
  739,  977,  740,  746,  747,  749,  977,  709,  750,  977,
  744,  516,  710,  760,  761,  763,  958,  958,  958,  958,
  958,  970,  958,  977,  764,  152,  499,  773,  977,  977,
  797,  667,  977, 1732,  958,  958,  958,  958,  798,  977,
  666,  977,  977,  666,  799,  977,  152,  719,  499,  977,
  251, 1777,  977,  804,  977,  499,  977,  271,  666,   49,
 1192,  977,  977,  809,  977,  815,  958,  813,  958,  814,
   50,  816,  251,  819, 1193,  152,  824,   51,  818,  251,
  977,  977,  830,  977,  831,  834,  835,  844,  709,  499,
  376,  846,  371,  710,  152, 1594, 1595,  977, 1194,  977,
  977,  977,   52,  152,  977,  847,  371,  977,  977,  152,
  709,  665,  855,  251,  371,  710,  856,  977,  857, 1302,
  941,  665, 1812,  941,  665,  859,  895,  152,  896,  530,
  371,  977,  898,  665,  977,  903,  689,  907,  935, 1491,
  908,  153,  946,  969,  975,  987,  984,  377,  530,  530,
  977,  530, 1195,  527,  709,  530,  977, 1635,  530,  710,
  991,  977,  715,  713,  711, 1005,  712,   53,  714,  681,
 1010,  530,  977,  977,  825,  977,  681,  530,  227,  681,
  665,  977, 1015,  681,  371, 1458, 1016, 1018, 1022,  665,
  776,  681,  196,  709,  527, 1031, 1023,  527,  710,   54,
  153, 1039, 1025,   55, 1055, 1032, 1042,  681, 1067, 1033,
  977,  977,  527,  977,  527,  228, 1048, 1053, 1062, 1669,
  977,  977,  977,  977,  977, 1079,  977, 1672,  977,  977,
 1094,  681, 1080,  665, 1142, 1139, 1158,  958,  676,  958,
  958, 1137,  945,  713,  711,  665,  712,  958,  714,  530,
 1145,  196,  378,  379,  380,  958,  198,  199, 1155, 1160,
  958,  681,  709,  958, 1685, 1162,  681, 1182, 1687, 1189,
 1170,  530, 1191,  664, 1197,  381,  382,  958,  530,  958,
  383,  384,  958,  958,  663, 1206,  958,  530,  681,  530,
 1211, 1214, 1223,  958, 1234,  958,  958, 1232, 1244,  958,
  681, 1254,  681,  958, 1261, 1265,  958,  530,  958, 1266,
  958, 1252,  530, 1267,  664,  958,  958,  666,  958,  962,
  713,  711,  532,  712,  941,  714, 1734,  666, 1268, 1737,
  666,  981,  664, 1279,  958,  958, 1286,  958, 1277,  666,
 1291,  530,  689, 1292, 1163,  713,  711, 1290,  712, 1073,
  714,  958, 1289,  958,  958,  958, 1695, 1294,  958, 1295,
 1299,  958,  958,  532, 1300, 1301,  532, 1772, 1303, 1304,
 1311,  958,  981,  981,  981,  981,  981,  981,  981, 1312,
 1491,  532, 1313,  532, 1314,  958,  666, 1316,  958, 1317,
  981,  981,  981,  981, 1318,  666, 1319, 1320, 1321,  877,
 1322,  709, 1356, 1695,  958, 1338,  710, 1696, 1359, 1366,
  958,  527, 1373, 1370, 1371,  958, 1372, 1374, 1375, 1382,
  153, 1387,  981, 1391,  981, 1385,  958,  958, 1393,  958,
  527,  527,  547,  527, 1405,  958, 1401,  527, 1406,  666,
  527,  153, 1407, 1340,  713,  711, 1695,  712, 1408,  714,
 1413,  666, 1409,  527, 1696, 1782, 1410, 1418, 1428,  527,
  722, 1435, 1439, 1296,  958,  958,  986,  958, 1440, 1444,
  153,  196, 1450, 1470,  958,  958,  958,  958,  958, 1471,
  958,  709,  958,  958, 1475, 1481,  710, 1502, 1695,  153,
 1482, 1695,  196, 1548, 1561, 1558, 1560, 1696,  153, 1562,
 1563, 1564, 1567, 1565,  153, 1568, 1783,  986,  986,  986,
  986,  986,  986,  986, 1695, 1577, 1579, 1569, 1582, 1586,
 1587,  196,  153, 1588, 1590,  986,  986,  986,  986, 1591,
 1592,  527,  713,  711, 1691,  712, 1593,  714, 1596, 1696,
  196, 1597, 1696, 1598, 1599, 1600, 1601, 1602,  196,  196,
 1603, 1604, 1605,  527, 1613,  196, 1615,  986,  709,  986,
  527, 1614, 1634,  710,  124, 1696, 1341,  713,  711, 1637,
  712,  527,  714,  196, 1642, 1344,  713,  711, 1631,  712,
  532,  714, 1636,  709, 1644,  196, 1638,  196,  710,  527,
 1639,  664,  386,  981,  527,  981,  981,  385, 1640, 1641,
  532,  664,  532,  981,  664, 1643,  532, 1649, 1650,  532,
  196,  981, 1674,  664, 1652,  273,  981, 1679, 1680,  981,
 1689, 1653,  532,  527, 1645, 1706, 1714, 1727,  532, 1738,
 1752, 1754, 1757,  981, 1759,  196, 1761, 1766,  981,  981,
 1774, 1767,  981, 1485, 1780, 1781, 1749, 1750,  151,  981,
 1797,  981,  981, 1802, 1806,  981,  102, 1805, 1741,  981,
  664, 1809,  981, 1823,  981, 1824,  981, 1820,    1,  664,
  628,  981,  981,   30,  981, 1345,  713,  711,   32,  712,
  203,  714,  709,  756,  377,  103,  104,  710,  105,  271,
  981,  981,  657,  981,  149,  106,  107,  108,  109,  110,
  532,  111,  530,  112,  113,  269,  941,  981,  672,  981,
  981,  981,   82,  664,  981,  466,  213,  981,  981,  530,
  276,  193,  532,  405,  530,  664,  689,  981,  986,  532,
  986,  986,  940, 1347,  713,  711,  687,  712,  986,  714,
  532,  981,  713,  711,  981,  712,  986,  714,  683,  672,
  685,  986,  672,  289,  986,  275,  194,  378,  532,  713,
  981,  287,  712,  532,  223,  324,  981,  672,  986,  714,
  709,  981,  332,  986,  986,  710,  288,  986,  341,  323,
  622,  678,  981,  981,  986,  981,  986,  986,  330,  723,
  986,  981,  532,   26,  986,   29,   39,  986, 1327,  986,
  129,  986,  416,  407,  406,  709,  986,  986, 1287,  986,
  710,  897, 1078,  845,  709, 1285,  241, 1273, 1283,  710,
  981,  981,  982,  981, 1570,  986,  986,  578,  986, 1571,
  981,  981,  981,  981,  981,  534,  981,  520,  981,  981,
  810, 1213,  986,  509,  986,  986,  986,  376,  985,  986,
  549, 1262,  986,  986, 1348,  713,  711,  550,  712, 1682,
  714, 1020,  986,  982,  982,  982,  982,  982,  982,  982,
  713,  711, 1550,  712, 1274,  714,  986, 1748, 1556,  986,
  792,  982,  982,  982,  982, 1755, 1753, 1728, 1349,  713,
  711,  196,  712, 1791,  714,  986, 1013, 1688, 1395, 1818,
 1703,  986,  661, 1785,  377, 1204,  986, 1378, 1505,    1,
 1713,  557,  540,  982,  709,  982, 1054,  986,  986,  710,
  986,  796,  531,  832, 1425, 1045,  986, 1350,  713,  711,
    2,  712,  131,  714,  734, 1351,  713,  711,    3,  712,
  684,  714,  730,  502,  284,    4,  976,    5,  971,    6,
  196, 1188,  989,  972,    7,  986,  986,  839,  986, 1678,
 1765,    8, 1293, 1619, 1796,  986,  986,  986,  986,  986,
 1829,  986,  709,  986,  986,    9,  743,  710, 1151,  904,
  709, 1307,    0,  509, 1358,  710,   10, 1352,  713,  711,
  660,  712,    0,  714,    0,    0,    0,    0,  839,  839,
  839,  839,  839,    0,  839,    0,    0,    0,    0,  378,
  379,  380,    0,  198,  199,    0,  839,  839,  839,  839,
    0,    0,    0,    0,  509,    0,  672,  509,    0,  672,
    0,    0,  381,  382,    0,    0,  672,  383,  384,  672,
    0,    0,  509,  672,    0,    0,    0,    0,  672,    0,
  839,  672,    0,   11,    0, 1476,  713,  711,    0,  712,
   12,  714,    0,   13,    0,   14,   15,  672,    0,    0,
    0, 1477,  713,  711,    0,  712,    0,  714,    0,    0,
    0,    0,    0,    0,  982,    0,  982,  982,    0,    0,
   16,  672,    0,  709,  982,  672,  713,  711,  710,  712,
    0,  714,  982,    0,  672,    0,    0,  982,  709,    0,
  982,    0,    0,  710,    0,    0,    0,    0,    0,    0,
    0,  672,    0,    0,  982,    0,    0,  709,    0,  982,
  982,    0,  710,  982,    0,    0,    0,  177,    0,   17,
  982,    0,  982,  982,    0,    0,  982,    0,  672,  196,
  982,    0,    0,  982,   18,  982,    0,  982,    0,    0,
  672,    0,  982,  982,    0,  982,  709,    0,    0,    0,
  196,  710,    0,    0,  709,  187,    0,    0,    0,  710,
    0,  982,  982,    0,  982,    0, 1484,  713,  711,    0,
  712,  196,  714,    0,    0,    0,  177,  196,  982,  669,
  982,  982,  982,    0,    0,  982,    0,    0,  982,  982,
    0,    0,  713,  711,    0,  712,    0,  714,  982,  839,
  196,  839,  839,    0,    0,  532,  709,    0,    0,  839,
    0,  710,  982,    0,  187,  982,    0,  839,    0,  196,
  669,    0,  839,  669,    0,  839,    0,  196,  196,    0,
    0,  982,    0,    0,  196,    0,    0,  982,  669,  839,
    0,  509,  982,  509,  839,  839,  532,  509,  839,  532,
  509,    0,  196,  982,  982,  839,  982,  839,  839,    0,
    0,  839,  982,  509,  532,  839,    0,  532,  839,    0,
  839,    0,  839,    0,  709,    0,    0,  839,  839,  710,
  839,  924,  925,  926,  927,  928,  929,  930,  931,  196,
  709,  982,  982,  851,  982,  710,  839,  839,    0,  839,
  196,  982,  982,  982,  982,  982,    0,  982,  532,  982,
  982,  532,    0,  839,  709,  839,  839,  839,    0,  710,
  839,    0,    0,  839,  839,    0,  532,    0,  547,    0,
    0,    0,    0,  839,  851,  851,  851,  851,  851,    0,
  851,  509,    0,    0,    0,    0,    0,  839,  188,    0,
  839,    0,  851,  851,  851,  851,    0,    0,    0,    0,
    0,    0,  181,  509,    0,    0,  839,    0,    0,    0,
  509,    0,  839,    0,    0,  177,    0,  839,    0,    0,
    0,    0,    0,    0,    0,    0,  851,    0,  839,  839,
    0,  839,    0,    0,    0,    0,  177,  839,    0,    0,
    0,    0,    0,  181,  509,  709,  181,  188,    0,    0,
  710,    0, 1457,  187,    0,    0,    0,  177,    0,    0,
    0,  181,    0,  177,    0,    0,  839,  839,  854,  839,
  709,    0,    0,    0,  187,  710,  839,  839,  839,  839,
  839,    0,  839,    0,  839,  839,  177,    0,    0,    0,
    0,    0,    0,    0,    0,  187,    0,    0,    0,    0,
    0,  187,    0,    0,    0,  177,    0,    0,    0,  854,
  854,  854,  854,  854,  177,  854,    0,    0,    0,    0,
  177,    0,    0,  532,  187,  532,    0,  854,  854,  854,
  854,    0,  532,    0,    0,    0,    0,  669,  177,    0,
  669,  102,    0,  187,    0,  532,    0,  669,    0,    0,
  669,    0,  187,    0,  669,    0,    0,    0,  187,  669,
    0,  854,  669,    0,    0,    0,    0,    0,    0,    0,
  103,  104,    0,  105,    0,  177,  187,    0,  669,    0,
  106,  107,  108,  109,  110,  532,  111,  532,  112,  113,
    0,  532,    0,    0,  532,  851,    0,  851,  851,    0,
    0,    0,  669,    0,    0,  851,  669,  532,    0,    0,
    0,    0,    0,  187,    0,  669,    0,    0,  851,    0,
    0,  851,    0,  532,    0,    0,    0,    0,    0,    0,
    0,    0,  669,    0,    0,  851,    0,    0,    0,    0,
  851,  851,    0,    0,  851,  532,  188,    0,    0,    0,
    0,  851,  532,  851,  851,    0,    0,  851,    0,  669,
  181,  851,    0,    0,  851,    0,  851,  188,  851,    0,
    0,  669,    0,  851,  851,    0,  851,    0,    0,    0,
    0,  181,    0,    0,    0,  532,  532,    0,  188,    0,
    0,    0,  851,  851,  188,  851,    0,    0,    0,    0,
    0,    0,  181,    0,    0,    0,    0,  532,  181,  851,
    0,  851,  851,  851,  532,    0,  851,  188,    0,  851,
  851,  553,    0,  821,    0,    0,    0,  884,    0,  851,
  854,  181,  854,  854,    0,    0,  188,    0,    0,    0,
  854,    0,    0,  851,    0,  188,  851,    0,  532,    0,
  181,  188,    0,    0,    0,    0,  854,    0,    0,  181,
    0,    0,  851,    0,  821,  181,    0,  821,  851,  188,
  854,    0,    0,  851,    0,  854,  854,    0,    0,  854,
    0,    0,  821,  181,  851,  851,  854,  851,  854,  854,
    0,    0,  854,  851,    0,    0,  854,    0,    0,  854,
    0,  854,    0,  854,    0,    0,  188,    0,  854,  854,
    0,  854,    0,    0,    0,    0,    0,    0,    0,    0,
  181,    0,  851,  851,  855,  851,    0,  854,  854,    0,
  854, 1616,  851,  851,  851,  851,  851,    0,  851,   59,
  851,  851,  887,    0,  854,    0,  854,  854,  854,    0,
   60,  854,    0,    0,  854,  854, 1617,   61,    0,  102,
    0,    0,    0,    0,  854,  855,  855,  855,  855,  855,
   62,  855,    0,    0,    0,    0,    0,    0,  854,    0,
    0,  854,    0,  855,  855,  855,  855,    0,  103,  104,
    0,  105,    0,    0,  102,    0,    0,  854,  106,  107,
  108,  109,  110,  854,  111,    0,  112,  113,  854,    0,
    0,    0,    0,    0, 1618,    0,    0,  855,    0,  854,
  854,    0,  854,  103,  104,    0,  105,    0,  854,    0,
    0,   63,    0,  106,  107,  108,  109,  110,    0,  111,
    0,  112,  113,    0,    0,    0,    0,   64,  102,    0,
    0,    0,    0,    0,    0,    0,    0,  854,  854,  849,
  854,    0,    0,    0,   65,    0,    0,  854,  854,  854,
  854,  854,    0,  854,    0,  854,  854,  103,  104,   66,
  105,    0,    0,   67,    0,  821,   68,  106,  107,  108,
  109,  110,    0,  111,   69,  112,  113,    0,    0,    0,
  849,    0,  849,  849,  849, 1447, 1448,    0,    0,   95,
    0, 1452, 1453,   70, 1455, 1456,    0,    0,  849,  849,
  849,  849, 1460, 1461, 1462, 1463,    0, 1464, 1465, 1466,
 1467, 1468, 1469,    0,    0,    0,    0,    0,    0,    0,
    0,  821,    0,    0,  821,    0,    0,    0,    0,    0,
   96,  821,  849,    0,  821,    0,    0,    0,  821,    0,
    0,    0,   97,  821,    0,    0,  821,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  126,    0,    0,    0,
    0,    0,  821,    0,    0,    0,  855,   98,  855,  855,
    0,    0,    0,    0,    0,    0,  855,  945,  713,  711,
    0,  712,    0,  714,    0,    0,  821,    0,    0,  821,
  821,    0,  855,    0,    0,    0,  875,  874,  876,  821,
    0,    0,    0,    0,    0,    0,  855,    0,    0,    0,
    0,  855,  855,    0,    0,  855,  821,    0,    0,    0,
    0,    0,  855,    0,  855,  855,    0,    0,  855,    0,
    0,    0,  855,    0,    0,  855,   99,  855,  821,  855,
    0,    0,    0,  821,  855,  855,  100,  855,    0,    0,
    0,    0,    0,    0,    0,  821,    0,  821,    0,    0,
    0,    0,    0,  855,  855,    0,  855,    0,    0,    0,
    0,  101,    0,    0,    0,    0,    0,    0,    0,    0,
  855,    0,  855,  855,  855,    0,    0,  855,    0,  102,
  855,  855,    0,    0,  790,    0,    0,    0,    0,    0,
  855,  849,    0,  849,  849,    0,    0,    0,    0,    0,
    0,  849,    0,    0,  855,    0,    0,  855,  103,  104,
    0,  105,    0,    0,    0,    0,    0,  849,  106,  107,
  108,  109,  110,  855,  111,  790,  112,  113,  790,  855,
    0,  849,    0,    0,  855,    0,  849,  849,    0,    0,
  849,    0,    0,  790,    0,  855,  855,  849,  855,  849,
  849,    0,    0,  849,  855,    0,    0,  849,    0,    0,
  849,    0,  849,    0,  849,    0,    0,    0,    0,  849,
  849,    0,  849,    0,    0,    0,    0,  884,    0,    0,
    0,    0,    0,  855,  855,  848,  855,    0,  849,  849,
    0,  849,    0,  855,  855,  855,  855,  855,  862,  855,
    0,  855,  855,    0,    0,  849,  709,  849,  849,  849,
    0,  710,  849,    0,  863,  849,  849,    0,    0,    0,
    0,    0,    0,    0,    0,  849,  848,    0,  848,  848,
  848,    0,    0,    0,    0,    0,    0,    0,    0,  849,
    0,    0,  849,    0,  848,  848,  848,  848,    0,    0,
  864,  885,    0,    0,    0,    0,    0,    0,  849,  865,
    0,    0,    0,    0,  849,    0,    0,  866,    0,  849,
    0,    0,    0,    0,    0,    0,    0,    0,  848,    0,
  849,  849,    0,  849,    0,    0,  867,  886,  868,  849,
    0,    0,  887,    0,    0,    0,    0,    0,  888,    0,
    0,    0,  869,    0,  870,  871,  872,    0,    0,  102,
    0,    0,    0,    0,    0,    0,    0,  889,  849,  849,
  850,  849,    0,    0,    0,    0,    0,    0,  849,  849,
  849,  849,  849,    0,  849,    0,  849,  849,  103,  104,
    0,  105,    0,    0,    0,    0,  790,    0,  106,  107,
  108,  109,  110,    0,  111,  873,  112,  113,    0,    0,
    0,  850,  579,  850,  850,  850,  580,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  850,
  850,  850,  850,    0,  581,    0,    0,  582,   81,    0,
    0,    0,    0,    0,    0,   82,    0,    0,    0,   83,
   84,    0,  790,    0,    0,  790,    0,    0,   85,    0,
    0,    0,  790,  850,    0,  790,    0,    0,    0,  790,
   86,    0,    0,    0,  790,    0,    0,  790,  583,    0,
    0,    0,    0,    0,  584,  585,  586,    0,  587,  588,
  589,    0,  590,  790,    0,    0,    0,  848,    0,  848,
  848,    0,    0,    0,    0,    0,    0,  848,    0,    0,
    0,    0,  591,    0,  592,    0,    0,  790,    0,    0,
  790,  790,    0,  848,    0,    0,    0,    0,    0,    0,
  790,    0,    0,    0,    0,   87,    0,  848,    0,    0,
    0,    0,  848,  848,    0,  593,  848,  790,    0,    0,
    0,    0,   88,  848,    0,  848,  848,    0,    0,  848,
    0,    0,  594,  848,    0,    0,  848,   89,  848,  790,
  848,   90,    0,    0,  790,  848,  848,    0,  848,    0,
    0,    0,   91,    0,  595,    0,  790,    0,  790,    0,
    0,    0,    0,    0,  848,  848,    0,  848,    0,    0,
    0,   92,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  848,    0,  848,  848,  848,    0,    0,  848,  596,
  597,  848,  848,    0,  560,    0,    0,    0,    0,    0,
    0,  848,  850,    0,  850,  850,  598,    0,    0,    0,
    0,    0,  850,    0,    0,  848,    0,    0,  848,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  850,    0,
    0,    0,    0,    0,  848,  560,    0,    0,  560,    0,
  848,    0,  850,    0,    0,  848,    0,  850,  850,    0,
    0,  850,    0,  560,    0,  560,  848,  848,  850,  848,
  850,  850,    0,    0,  850,  848,    0,    0,  850,    0,
    0,  850,    0,  850,    0,  850,   95,    0,    0,    0,
  850,  850,    0,  850,    0,  560,    0,    0,    0,    0,
    0,    0,    0,    0,  848,  848,  853,  848,    0,  850,
  850,    0,  850,    0,  848,  848,  848,  848,  848,    0,
  848,    0,  848,  848,  401,    0,  850,   96,  850,  850,
  850,    0,    0,  850,    0,    0,  850,  850,    0,   97,
    0,  102,    0,    0,    0,    0,  850,  853,    0,  853,
  853,  853,    0,    0,    0,    0,    0,    0,    0,    0,
  850,    0,    0,  850,   98,  853,  853,  853,  853,    0,
  103,  104,    0,  105,    0,    0,    0,  102,    0,  850,
  106,  107,  108,  109,  110,  850,  111,    0,  112,  113,
  850,    0,    0,    0,    0,    0,    0,    0,    0,  853,
    0,  850,  850,    0,  850,    0,  103,  104,    0,  105,
  850,    0,    0,    0,    0,    0,  106,  107,  108,  109,
  110,    0,  111,  642,  112,  113,  448,    0,  449,    0,
    0,    0,    0,   99,    0,    0,    0,    0,    0,  850,
  850,  451,  850,  100,    0,    0,  273,    0,    0,  850,
  850,  850,  850,  850,    0,  850,    0,  850,  850,    0,
    0,    0,  560,    0,    0,    0,    0,    0,  101,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  560,    0,
    0,    0,  560,  221,  560,    0,  102,  221,  560,    0,
    0,  560,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,    0,    0,  560,  221,    0,    0,  221,    0,
  560,    0,    0,  560,    0,  103,  104,    0,  105,    0,
    0,    0,    0,    0,    0,  106,  107,  108,  109,  110,
    0,  111,  397,  112,  113,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,    0,    0,    0,    0,  221,  221,  221,  102,  221,
  221,  221,    0,  221,    0,    0,    0,    0,  853,    0,
  853,  853,    0,    0,    0,    0,    0,  427,  853,    0,
    0,    0,  560,  221,    0,  221,    0,  103,  104,    0,
  105,    0,    0,    0,  853,    0,    0,  106,  107,  108,
  109,  110,    0,  111,  560,  112,  113,    0,  853,    0,
    0,  560,    0,  853,  853,  560,  221,  853,    0,    0,
    0,   81,  560,    0,  853,    0,  853,  853,    0,    0,
  853,    0,    0,  221,  853,    0,    0,  853,    0,  853,
  560,  853,    0,    0,    0,  560,  853,  853,    0,  853,
  421,    0,    0,  642,    0,  221,  448,    0,  449,  422,
    0,    0,    0,    0,    0,  853,  853,    0,  853,    0,
  560,  451,  423,    0,  560,  635,  273,    0,  376,  424,
    0,   73,  853,  434,  853,  853,  853,    0,    0,  853,
  221,  221,  853,  853,  435,    0,    0,  636,    0,    0,
    0,    0,  853,    0,    0,    0,  425,  221,    0,  426,
    0,    0,  103,  441,    0,  105,  853,    0,    0,  853,
    0,  444,  445,  446,  108,  109,  110,    0,  111,    0,
  112,  113,  543,    0,    0,  853,    0,  427,  428,  570,
    0,  853,    0,    0,    0,    0,  853,    0,    0,  429,
    0,    0,    0,  430,    0,    0,    0,  853,  853,    0,
  853,  637,    0,    0,    0,    0,  853,    0,  570,  570,
    0,  570,  543,  543,    0,    0,  543,    0,  570,  570,
  570,  570,  570,    0,  570,    0,  570,  570,  281,    0,
    0,  543,    0,  543,    0,  853,  853,    0,  853,    0,
    0,    0,    0,    0,  638,  853,  853,  853,  853,  853,
  431,  853,    0,  853,  853,    0,  432,  281,  281,    0,
  281,    0,    0,  543,    0,    0,  433,  281,  281,  281,
  281,  281,    0,  281,    0,  281,  281,    0,    0,    0,
  378,  379,  380,  434,  198,  199,    0,    0,    0,    0,
    0,    0,  134,    0,  435,    0,  134,    0,    0,    0,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,  134,  105,    0,  134,  442,  443,
  421,  444,  445,  446,  108,  109,  110,  447,  111,  422,
  112,  113,  639,  640,  641,    0,    0,    0,  642,    0,
    0,  448,  423,  449,    0,    0,    0,    0,  376,  424,
    0,    0,    0,    0,    0,    0,  451,    0,  134,    0,
    0,  273,    0,    0,  134,  134,  134,  636,  134,  134,
  134,    0,  134,    0,    0,    0,  425,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,  134,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  427,  428,    0,
    0,    0,    0,    0,  550,    0,    0,    0,    0,  429,
    0,    0,    0,  430,    0,  134,    0,    0,    0,    0,
  543,  637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,    0,    0,  543,    0,    0,  543,
  543,    0,  543,    0,    0,  550,  543,    0,  550,  543,
    0,    0,    0,    0,  134,    0,    0,    0,    0,    0,
  703,    0,  543,  550,  638,  550,    0,    0,  543,    0,
  431,  543,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  433,    0,    0,  134,
  134,    0,    0,    0,    0,  550,    0,    0,    0,    0,
  378,  379,  380,  434,  198,  199,  134,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,    0,  105,    0,    0,  442,  443,
  543,  444,  445,  446,  108,  109,  110,  447,  111,  534,
  112,  113,  639,  640,  641,  421,    0,    0,    0,    0,
    0,    0,  543,    0,  422,    0,    0,    0,    0,  543,
    0,    0,    0,  543,    0,    0,    0,  423,    0,    0,
  543,    0,    0,  376,  424,    0,    0,    0,    0,    0,
  534,    0,    0,  534,    0,    0,    0,    0,  543,    0,
    0,    0,  636,  543,    0,    0,    0,    0,  534,    0,
  534,  425,    0,    0,  426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  543,    0,
    0,    0,  543,  648,    0,  648,  648,    0,  648,    0,
  534,    0,  427,  428,    0,    0,    0,    0,    0,    0,
    0,  648,    0,    0,  429,    0,  648,    0,  430,    0,
    0,    0,  550,    0,    0,    0,  637,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  550,    0,
    0,    0,  550,    0,  550,    0,    0,    0,  550,    0,
    0,  550,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  550,    0,    0,    0,    0,  638,
  550,    0,    0,  550,    0,  431,    0,  642,    0,  642,
  642,  432,  642,    0,    0,    0,    0,    0,    0,    0,
    0,  433,    0,    0,    0,  642,    0,    0,    0,    0,
  642,    0,    0,    0,    0,  378,  379,  380,  434,  198,
  199,    0,    0,    0,    0,    0,    0,    0,    0,  435,
    0,    0,    0,    0,    0,    0,    0,    0,  381,  382,
  436,  437,  438,  383,  384,  439,  440,  103,  441,    0,
  105,    0,  550,  442,  443,    0,  444,  445,  446,  108,
  109,  110,  447,  111,    0,  112,  113,  639,  640,  641,
    0,    0,    0,    0,  550,    0,    0,  534,    0,    0,
    0,  550,    0,    0,    0,  550,    0,    0,    0,    0,
    0,    0,  550,  534,    0,    0,    0,  534,    0,  534,
    0,    0,    0,  534,    0,    0,  534,    0,    0,    0,
  550,    0,    0,    0,    0,  550,    0,    0,    0,  534,
    0,    0,    0,  648,    0,  534,    0,    0,  534,    0,
  648,    0,    0,    0,    0,    0,    0,    0,    0,  648,
  550,    0,    0,    0,  550,    0,    0,    0,    0,    0,
    0,  791,  648,    0,    0,    0,    0,    0,  648,  648,
    0,    0,    0,    0,    0,    0,  648,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  648,    0,    0,  648,
    0,    0,  791,    0,    0,  791,    0,  534,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  642,    0,    0,
  791,    0,    0,    0,  642,  648,    0,  648,  648,  534,
    0,    0,    0,  642,    0,    0,  534,    0,    0,  648,
  534,    0,    0,  648,    0,    0,  642,  534,    0,    0,
    0,  450,  642,  642,  448,    0,  449,    0,    0,    0,
  642,    0,    0,    0,    0,  534,    0,    0,    0,  451,
  534,    0,    0,    0,  273,    0,    0,    0,    0,    0,
  642,    0,    0,  642,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  534,    0,    0,    0,  534,
  648,    0,    0,    0,    0,    0,  648,    0,    0,  642,
    0,  642,  642,    0,    0,    0,  648,    0,    0,    0,
    0,    0,    0,  642,    0,    0,    0,  642,    0,    0,
  648,  648,  648,  648,  648,  648,    0,    0,    0,    0,
    0,    0,    0,    0,  648,    0,    0,    0,    0,    0,
    0,    0,    0,  648,  648,  648,  648,  648,  648,  648,
  648,  648,  648,  648,    0,  648,    0,    0,  648,  648,
    0,  648,  648,  648,  648,  648,  648,  648,  648,    0,
  648,  648,    0,    0,  642,    0,    0,    0,    0,    0,
  642,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  642,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  642,  642,  642,  642,  642,  642,
    0,    0,    0,  791,    0,    0,    0,    0,  642,    0,
    0,    0,    0,    0,    0,    0,    0,  642,  642,  642,
  642,  642,  642,  642,  642,  642,  642,  642,    0,  642,
    0,    0,  642,  642,    0,  642,  642,  642,  642,  642,
  642,  642,  642,    0,  642,  642,    0,    0,    0,    0,
    0, 1089,    0,    0, 1090,    0,    0,    0,  421,  791,
    0,    0,  791,    0,    0,    0,    0,  422,    0,  791,
    0,    0,  791,    0,    0,    0,  791,    0,    0,    0,
  423,  791,    0,    0,  791,  450,  376,  424,  448,    0,
  449,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  791,    0,    0,  451,    0,    0,    0,    0,  273,    0,
    0,    0,    0,    0,  425,    0,    0,  426,    0,    0,
    0,    0,    0,    0,  791,    0,  815,  791,  791,    0,
    0,    0,    0,    0,    0,    0,    0,  791,    0,    0,
    0,    0,    0,    0,    0,  427,  428,    0,    0,    0,
    0,    0,    0,    0,  791,    0,    0,  429,    0,    0,
    0,  430,    0,    0,    0,    0,    0,  815,    0,  450,
  815,    0,  448,    0,  449,    0,  791,    0,    0,    0,
    0,  791,    0,    0,    0,  815,    0,  451,    0,    0,
    0,    0,  273,  791,    0,  791,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1091,    0,  431,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  433,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  378,  379,
  380,  434,  198,  199,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,    0,    0,
    0,  381,  382,  436,  437,  438,  383,  384,  439,  440,
  103,  441,    0,  105,  848,    0,  442,  443,  580,  444,
  445,  446,  108,  109,  110,  447,  111,    0,  112,  113,
    0,    0,  849,    0,    0, 1099,  581,    0, 1090,  582,
    0,    0,  421,    0,    0,    0,    0,    0,    0,    0,
    0,  422,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,    0,
  376,  424,    0,    0,    0,    0,    0,    0,    0,    0,
  583,    0,    0,    0,    0,    0,  584,  585,  586,    0,
  587,  588,  589,    0,  590,    0,    0,    0,  425,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  591,    0,  592,    0,  815, 1103,
    0,    0, 1090,    0,    0,    0,  421,    0,    0,  427,
  428,    0,    0,    0,    0,  422,    0,    0,    0,    0,
    0,  429,    0,    0,    0,  430,    0,  593,  423,    0,
    0,    0,    0,  450,  376,  424,  448,    0,  449,    0,
    0,    0,    0,    0,  594,    0,    0,    0,    0,    0,
    0,  451,    0,    0,  815,    0,  273,  815,    0,    0,
    0,    0,  425,    0,  815,  426,  595,  815,    0,    0,
    0,  815,    0,    0,    0,    0,  815,    0,    0,  815,
 1091,    0,  431,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,  427,  428,  815,    0,    0,  433,    0,
    0,  596,  597,    0,    0,  429,    0,    0,    0,  430,
    0,    0,  378,  379,  380,  434,  198,  199,  598,  815,
    0,    0,  815,  815,    0,    0,  435,    0,    0,    0,
    0,    0,  815,    0,    0,  381,  382,  436,  437,  438,
  383,  384,  439,  440,  103,  441,    0,  105,    0,  815,
  442,  443,    0,  444,  445,  446,  108,  109,  110,  447,
  111,    0,  112,  113, 1091,    0,  431,    0,    0,    0,
    0,  815,  432,    0,    0,    0,  815,    0,    0,    0,
    0,    0,  433,    0,    0,    0,    0,    0,  815,    0,
  815,    0,    0,    0,    0,    0,  378,  379,  380,  434,
  198,  199,    0,    0,    0,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,    0,    0,    0,    0,  381,
  382,  436,  437,  438,  383,  384,  439,  440,  103,  441,
    0,  105,    0,    0,  442,  443,    0,  444,  445,  446,
  108,  109,  110,  447,  111,    0,  112,  113,    0,    0,
    0,    0,    0, 1111,    0,    0, 1090,    0,    0,    0,
  421,    0,    0,    0,    0,    0,    0,    0,    0,  422,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,  450,  376,  424,
  448,    0,  449,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  451,    0,    0,    0,    0,
  273,    0,    0,    0,    0,    0,  425,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,  794,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  427,  428,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  429,
    0,    0,    0,  430,    0,    0,    0,    0,    0,  794,
    0,  450,  794,    0,  448,    0,  449,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  794,    0,  451,
    0,    0,    0,    0,  273,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1091,    0,
  431,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  378,  379,  380,  434,  198,  199,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,    0,  105,  221,    0,  442,  443,
  221,  444,  445,  446,  108,  109,  110,  447,  111,    0,
  112,  113,    0,    0,  221,    0,    0, 1114,  221,    0,
 1090,  221,    0,    0,  421,    0,    0,    0,    0,    0,
    0,    0,    0,  422,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  423,    0,    0,    0,
    0,    0,  376,  424,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,  221,  221,
  221,    0,  221,  221,  221,    0,  221,    0,    0,    0,
  425,    0,    0,  426,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,    0,  221,    0,
  794, 1119,    0,    0, 1090,    0,    0,    0,  421,    0,
    0,  427,  428,    0,    0,    0,    0,  422,    0,    0,
    0,    0,    0,  429,    0,    0,    0,  430,    0,  221,
  423,    0,    0,    0,    0,  450,  376,  424,  448,    0,
  449,    0,    0,    0,    0,    0,  221,    0,    0,    0,
    0,    0,    0,  451,    0,    0,  794,    0,  273,  794,
    0,    0,    0,    0,  425,    0,  794,  426,  221,  794,
    0,    0,    0,  794,    0,    0,    0,    0,  794,    0,
    0,  794, 1091,    0,  431,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,  427,  428,  794,    0,    0,
  433,    0,    0,  221,  221,    0,    0,  429,    0,    0,
    0,  430,    0,    0,  378,  379,  380,  434,  198,  199,
  221,  794,    0,    0,  794,  794,    0,    0,  435,    0,
    0,    0,    0,    0,  794,    0,    0,  381,  382,  436,
  437,  438,  383,  384,  439,  440,  103,  441,    0,  105,
    0,  794,  442,  443,    0,  444,  445,  446,  108,  109,
  110,  447,  111,    0,  112,  113, 1091,    0,  431,    0,
    0,    0,    0,  794,  432,    0,    0,    0,  794,    0,
    0,    0,    0,    0,  433,    0,    0,    0,    0,    0,
  794,    0,  794,    0,    0,    0,    0,    0,  378,  379,
  380,  434,  198,  199,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,    0,    0,
    0,  381,  382,  436,  437,  438,  383,  384,  439,  440,
  103,  441,    0,  105,    0,    0,  442,  443,    0,  444,
  445,  446,  108,  109,  110,  447,  111,    0,  112,  113,
    0,    0,    0,    0,    0, 1122,    0,    0, 1090,    0,
    0,    0,  421,    0,    0,  792,    0,    0,    0,    0,
    0,  422,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,  450,
  376,  424,  448,    0,  449,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  792,  451,    0,  792,
    0,    0,  273,    0,    0,    0,    0,    0,  425,    0,
    0,  426,    0,    0,  792,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  427,
  428,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  429,    0,    0,    0,  430,    0,    0,    0,    0,
    0,  450,    0,  481,  448,    0,  449,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  451,
    0,    0,    0,    0,  273,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1091,    0,  431,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  433,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  378,  379,  380,  434,  198,  199,    0,    0,
    0,    0,    0,    0,    0,    0,  435,    0,    0,    0,
    0,    0,    0,    0,    0,  381,  382,  436,  437,  438,
  383,  384,  439,  440,  103,  441,    0,  105,    0,    0,
  442,  443,    0,  444,  445,  446,  108,  109,  110,  447,
  111,    0,  112,  113,    0,    0,    0,    0,    0, 1125,
    0,    0, 1090,    0,    0,    0,  421,    0,    0,    0,
    0,    0,    0,    0,    0,  422,    0,  792,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  423,    0,
    0,    0,    0,    0,  376,  424,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  425,    0,    0,  426,    0,    0,    0,    0,
    0,    0,    0,  792,    0,    0,  792,    0,    0,    0,
    0,    0,    0,  792,    0,    0,  792,    0,  421,    0,
  792,    0,    0,  427,  428,  792,    0,  422,  792,    0,
    0,    0,    0,  450,    0,  429,  448,    0,  449,  430,
  423,    0,    0,    0,  792,    0,  376,  424,    0,    0,
    0,  451,    0,    0,    0,    0,  273,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  792,    0,
    0,  792,  792,    0,  425,    0,    0,  426,    0,    0,
    0,  792,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1091,    0,  431,    0,  792,    0,
    0,    0,  432,  377,    0,  427,  428,    0,    0,    0,
    0,    0,  433,    0,    0,    0,    0,  429,    0,    0,
  792,  430,    0,    0,    0,  792,  378,  379,  380,  434,
  198,  199,    0,    0,    0,    0,    0,  792,    0,  792,
  435,    0,    0,    0,    0,    0,    0,    0,    0,  381,
  382,  436,  437,  438,  383,  384,  439,  440,  103,  441,
    0,  105,    0,    0,  442,  443,    0,  444,  445,  446,
  108,  109,  110,  447,  111,    0,  112,  113,  431,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  433,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  378,  379,
  380,  434,  198,  199,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,    0,    0,
    0,  381,  382,  436,  437,  438,  383,  384,  439,  440,
  103,  441,    0,  105,    0,    0,  442,  443,    0,  444,
  445,  446,  108,  109,  110,  447,  111,    0,  112,  113,
  421,    0,    0,  952,    0,  952,  952,    0,  952,  422,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  952,  423,    0,    0,  102,  952,    0,  376,  424,
    0,    0,    0,    0,  155,  156,  157,  158,  159,  160,
  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,
    0,    0,  171,  172,  103,  104,  425,  105,    0,  426,
  173,  174,    0,    0,  106,  107,  108,  109,  110,  175,
  111,  176,  112,  113,  177,  178,  179,    0,    0,    0,
    0,    0,    0,    0,    0,  377,  648,  427,  428,  648,
    0,  648,    0,    0,    0,    0,    0,    0,    0,  429,
    0,    0,    0,  430,  648,    0,    0,    0,    0,  648,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  703,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  378,  379,  380,  434,  198,  199,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,    0,  105,    0,    0,  442,  443,
  952,  444,  445,  446,  108,  109,  110,  447,  111,  952,
  112,  113,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  952,    0,    0,    0,    0,    0,  952,  952,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  952,    0,    0,  952,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  648,    0,    0,    0,
    0,    0,    0,  648,    0,  952,  450,  952,  952,  448,
    0,  449,  648,    0,    0,    0,    0,    0,    0,  952,
    0,    0,    0,  952,  451,  648,    0,    0,    0,  273,
    0,  648,  648,    0,    0,    0,    0,    0,    0,  648,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  648,
    0,    0,  648,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  952,    0,    0,    0,    0,    0,  952,    0,    0,    0,
  648,  648,    0,    0,    0,    0,  952,    0,    0,    0,
    0,    0,  648,    0,  450,    0,  648,  448,    0,  449,
  952,  952,  952,  952,  952,  952,    0,    0,    0,    0,
    0,    0,  451,    0,  952,    0,    0,  273,    0,    0,
    0,    0,    0,  952,  952,  952,  952,  952,  952,  952,
  952,  952,  952,  952,  321,  952,    0,    0,  952,  952,
    0,  952,  952,  952,  952,  952,  952,  952,  952,    0,
  952,  952,    0,  648,    0,    0,    0,    0,    0,  648,
    0,    0,    0,    0,  321,    0,    0,    0,    0,  648,
    0,  321,    0,    0,    0,  321,  321,    0,  321,    0,
    0,  321,    0,  648,  648,  648,  648,  648,  648,    0,
    0,    0,    0,    0,    0,  321,    0,  648,    0,    0,
    0,  321,    0,    0,    0,    0,  648,  648,  648,  648,
  648,  648,  648,  648,  648,  648,  648,    0,  648,    0,
    0,  648,  648,  421,  648,  648,  648,  648,  648,  648,
  648,  648,  422,  648,  648,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,  376,  424,    0,    0,    0,    0,    0,    0,    0,
    0,  321,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  772,    0,    0,    0,    0,  425,
  321,    0,  426,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  321,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  321,    0,    0,    0,    0,
  427,  428,    0,    0,  321,  772,  321,    0,  772,    0,
    0,  421,  429,  321,  450,    0,  430,  448,    0,  449,
  422,    0,    0,  772,    0,    0,    0,    0,    0,    0,
    0,    0,  451,  423,    0,    0,    0,  273,    0,  376,
  424,    0,  321,  321,    0,  321,    0,    0,    0,    0,
    0,    0,  321,  321,  321,  321,  321,    0,  321,    0,
  321,  321,    0,    0,    0,    0,    0,  425,    0,    0,
  426,    0,    0,  431,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  433,
    0,    0,    0,    0,    0,    0,    0,  695,  427,  428,
    0,    0,    0,  378,  379,  380,  434,  198,  199,    0,
  429,    0,    0,    0,  430,    0,    0,  435,    0,    0,
    0,    0,    0,    0,    0,    0,  381,  382,  436,  437,
  438,  383,  384,  439,  440,  103,  441,    0,  105,    0,
    0,  442,  443,    0,  444,  445,  446,  108,  109,  110,
  447,  111,    0,  112,  113,    0,    0,    0,    0,    0,
    0,  703,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  431,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  433,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  378,  379,  380,  434,  198,  199,    0,    0,    0,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
    0,    0,    0,    0,  381,  382,  436,  437,  438,  383,
  384,  439,  440,  103,  441,    0,  105,    0,    0,  442,
  443,  421,  444,  445,  446,  108,  109,  110,  447,  111,
  422,  112,  113,  450,  947,    0,  448,    0,  449,    0,
    0,    0,    0,  423,    0,    0,    0,    0,    0,  376,
  424,  451,    0,    0,    0,    0,  273,    0,    0,    0,
    0,    0,  772,    0,    0,  772,    0,    0,    0,    0,
    0,    0,  772,  793,    0,  772,    0,  425,    0,  772,
  426,    0,    0,    0,  772,    0,    0,  772,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  772,    0,    0,  377,    0,  427,  428,
    0,    0,    0,    0,  793,    0,  450,  793,    0,  448,
  429,  449,    0,    0,  430,    0,    0,  772,    0,    0,
  772,  772,  793,    0,  451,    0,    0,    0,    0,  273,
  772,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  772,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  772,
    0,  431,    0,    0,  772,    0,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,  772,  433,  772,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  378,  379,  380,  434,  198,  199,    0,    0,    0,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
    0,    0,    0,    0,  381,  382,  436,  437,  438,  383,
  384,  439,  440,  103,  441,    0,  105,    0,    0,  442,
  443,    0,  444,  445,  446,  108,  109,  110,  447,  111,
  421,  112,  113,    0,    0,    0,    0,    0,    0,  422,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,  376,  424,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  425,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  793,    0,    0,    0,    0,
    0,    0,    0,  421,    0,    0,  450,  427,  428,  448,
    0,  449,  422,    0,    0,    0,    0,    0,    0,  429,
    0,    0,    0,  430,  451,  423,    0,    0,    0,  273,
    0,  376,  424,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  793,    0,    0,  793,    0,    0,    0,    0,  425,
    0,  793,  426,    0,  793,    0,    0,    0,  793,    0,
    0,    0,    0,  793,    0,    0,  793,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  432,    0,    0,    0,
  427,  428,  793,    0,    0,    0,  433,    0,    0,    0,
    0,    0,  429,    0,  450,    0,  430,  448,    0,  449,
  378,  379,  380,  434,  198,  199,  793,    0,    0,  793,
  793,    0,  451,    0,  435,    0,    0,  273,    0,  793,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,    0,  105,  793,    0,  442,  443,
    0,  444,  445,  446,  108,  109,  110,  447,  111,    0,
  112,  113,    0,  431,    0,    0,    0,    0,  793,  432,
    0,    0,    0,  793,    0,    0,    0,    0,    0,  433,
    0,    0,    0,    0,    0,  793,    0,  793,    0,    0,
 1117,    0,    0,  378,  379,  380,  434,  198,  199,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,    0,
    0,    0,    0,    0,    0,    0,  381,  382,  436,  437,
  438,  383,  384,  439,  440,  103,  441,    0,  105,    0,
    0,  442,  443,  421,  444,  445,  446,  108,  109,  110,
  447,  111,  422,  112,  113,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  423,    0,    0,  462,    0,
    0,  376,  424,    0,    0,    0,    0,    0,  462,  462,
  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,
  462,  462,  462,    0,    0,  462,  462,  462,  462,  425,
  462,    0,  426,  462,  462,    0,    0,  462,  462,  462,
  462,  462,  462,  462,  462,  462,  462,  462,  462,  462,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  427,  428,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  421,  429,    0,    0,    0,  430,    0,    0,    0,
  422,    0,    0,  450,    0,    0,  448,    0,  449,    0,
    0,    0,    0,  423,    0,    0,    0,    0,    0,  376,
  424,  451, 1684,    0,    0,    0,  273,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1296,    0,    0,    0,  425,    0,    0,
  426,    0,    0,  431,    0,  713,  711,    0,  712,  432,
  714,    0,    0,    0,    0,    0,    0,    0,    0,  433,
    0,    0,    0,  875,  874,  876,    0,    0,  427,  428,
    0,    0,    0,  378,  379,  380,  434,  198,  199,    0,
  429,    0,    0,    0,  430,    0,    0,  435,    0,    0,
    0,    0,    0,    0,    0,    0,  381,  382,  436,  437,
  438,  383,  384,  439,  440,  103,  441,    0,  105,    0,
    0,  442,  443,    0,  444,  445,  446,  108,  109,  110,
  447,  111,    0,  112,  113,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  431,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  433,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1309,    0,
    0,  378,  379,  380,  434,  198,  199,    0,    0,    0,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
    0,    0,    0,    0,  381,  382,  436,  437,  438,  383,
  384,  439,  440,  103,  441,    0,  105,    0,    0,  442,
  443,    0,  444,  445,  446,  108,  109,  110,  447,  111,
  421,  112,  113, 1729,    0,    0,  448,    0,  449,  422,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  451,  423,    0,    0,    0,  273,    0,  376,  424,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  862,  425,    0,    0,  426,
    0,    0,    0,  709,    0,    0,    0,    0,  710,    0,
    0,  863,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  450,  427,  428,  448,
    0,  449,    0,    0,    0,    0,    0,    0,    0,  429,
    0,    0,    0,  430,  451,    0,    0,  864,    0,  273,
    0,    0,    0,    0,    0,    0,  865,    0,    0,    0,
    0,    0,    0,    0,  866,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  867,    0,  868,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  869,
  431,  870,  871,  872,    0,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  378,  379,  380,  434,  198,  199,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,  873,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441,    0,  105,    0,    0,  442,  443,
  421,  444,  445,  446,  108,  109,  110,  447,  111,  422,
  112,  113,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  423,    0,    0,    0,    0,    0,  376,  424,
    0,  823,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  425,    0,    0,  426,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  823,    0,    0,  823,    0,    0,    0,    0,
    0,    0,    0,  421,    0,  377,  952,  427,  428,  952,
  823,  952,  422,    0,    0,    0,    0,    0,    0,  429,
    0,    0,    0,  430,  952,  423,    0,    0,    0,  952,
    0,  376,  424,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  425,
    0,    0,  426,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  432,    0,    0,    0,
  427,  428,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,  429,    0,    0,    0,  430,    0,    0,    0,
  378,  379,  380,  434,  198,  199,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,    0,
    0,    0,    0,  381,  382,  436,  437,  438,  383,  384,
  439,  440,  103,  441, 1393,  105,    0,    0,  442,  443,
    0,  444,  445,  446,  108,  109,  110,  447,  111,    0,
  112,  113,    0,  431,    0,    0,    0,    0,  787,  432,
    0,    0,    0,    0,    6,    0,    0,    0,    0,  433,
    0,    0,    0,    0,    0, 1513, 1514,    0, 1515,    0,
    0, 1516,    0,  378,  379,  380,  434,  198,  199,    0,
    0,    0,    0,    0,    0, 1517,    0,  435,    0,  787,
    0, 1518,  787,  823,    0,    0,  381,  382,  436,  437,
  438,  383,  384,  439,  440,  103,  441,  787,  105,    0,
    0,  442,  443,  952,  444,  445,  446,  108,  109,  110,
  447,  111,  952,  112,  113,    0,    0,    0,    0,    0,
    0,    0,  788,    0,    0,  952,    0,    0,  789,    0,
    0,  952,  952,    0,    0,    0,    0,    0,    0,  823,
    0, 1519,  823,    0,    0,    0,    0,    0,    0,  823,
    0,  822,  823,    0,    0,    0,  823,    0,    0,  952,
   14,  823,  952,  788,  823,    0,  788,    0,    0,  789,
    0,    0,  789,    0,    0, 1520,    0,    0,    0,    0,
  823,  788,    0,    0,    0,   16,    0,  789,    0,    0,
  952,  952,  822,    0, 1645,  822, 1521,    0,    0,    0,
    0,    0,  952,  102,  823,    0,  952,  823,  823,    0,
  822,    0,    0,    0,    0,    0,    0,  823,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1522,  104,  823,  105,    0,    0,    0,    0,
    0,    0,  106,  107,  108,  109,  110,    0,  111,    0,
  112, 1523,    0,    0,    0,    0,  823,    0,    0,    0,
    0,  823,    0,  952,    0,    0,    0,    0,    0,  952,
    0,  816,    0,  823,    0,  823,    0,    0,    0,  952,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  952,  952,  952,  952,  952,  952,    0,
  787,    0,    0,    0,  824,    0,    0,  952,    0,    0,
    0,    0,  816,    0,    0,  816,  952,  952,  952,  952,
  952,  952,  952,  952,  952,  952,  952,    0,  952,    0,
  816,  952,  952,    0,  952,  952,  952,  952,  952,  952,
  952,  952,    0,  952,  952,  824,    0,    0,  824,    0,
    0,    0,    0,    0,    0,    0,  787,    0,    0,  787,
    0,    0,    0,  824,    0,    0,  787,  825,    0,  787,
    0,    0,    0,  787,    0,    0,    0,    0,  787,    0,
    0,  787,    0,    0,  788,    0,    0,    0,    0,    0,
  789,    0,    0,    0,    0,    0,    0,  787,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  825,    0,
    0,  825,    0,  822,    0,    0,    0,  813,    0,    0,
    0,  787,    0,    0,  787,  787,  825,    0,    0,    0,
    0,    0,    0,    0,  787,    0,    0,    0,    0,    0,
  788,    0,    0,  788,    0,    0,  789,    0,    0,  789,
  788,  787,    0,  788,    0,    0,  789,  788,  813,  789,
    0,  813,  788,  789,    0,  788,    0,    0,  789,  822,
    0,  789,  822,  787,    0,    0,  813,    0,  787,  822,
    0,  788,  822,    0,    0,    0,  822,  789,    0,    0,
  787,  822,  787,    0,  822,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  788,    0,    0,  788,  788,
  822,  789,    0,    0,  789,  789,    0,    0,  788,    0,
    0,    0,    0,    0,  789,    0,    0,    0,    0,    0,
    0,    0,    0,  816,  822,  788,    0,  822,  822,    0,
    0,  789,    0,    0,    0,    0,    0,  822,    0,  817,
    0,    0,    0,    0,    0,    0,    0,  788,    0,    0,
  826,    0,  788,  789,  822,    0,  824,    0,  789,    0,
    0,    0,    0,    0,  788,    0,  788,    0,    0,    0,
  789,    0,  789,  814,    0,    0,  822,    0,    0,  816,
  817,  822,  816,  817,    0,    0,    0,    0,    0,  816,
    0,  826,  816,  822,  826,  822,  816,    0,  817,    0,
    0,  816,    0,    0,  816,    0,    0,    0,    0,  826,
    0,    0,  824,    0,  814,  824,    0,  814,    0,  825,
  816,    0,  824,    0,    0,  824,    0,    0,    0,  824,
    0,    0,  814,    0,  824,    0,    0,  824,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  816,  816,    0,
    0,    0,    0,  824,    0,    0,    0,  816,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  813,
    0,    0,    0,    0,  816,  825,    0,  824,  825,    0,
  824,  824,    0,    0,    0,  825,    0,    0,  825,    0,
  824,    0,  825,    0,    0,    0,  816,  825,    0,    0,
  825,  816,    0,    0,    0,    0,    0,  824,  818,    0,
    0,    0,    0,  816,    0,  816,  825,    0,    0,    0,
    0,    0,    0,    0,    0,  813,    0,    0,  813,  824,
    0,    0,    0,    0,  824,  813,    0,    0,  813,    0,
  825,    0,  813,  825,  825,    0,  824,  813,  824,  818,
  813,    0,  818,  825,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  813,  818,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  813,    0,  825,  813,  813,    0,    0,  825,    0,    0,
    0,  817,    0,  813,    0,    0,    0,    0,    0,  825,
    0,  825,  826,    0,    0,    0,    0,    0,    0,    0,
  813,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  814,    0,    0,    0,    0,
    0,    0,  813,    0,    0,    0,    0,  813,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  817,    0,  813,
  817,  813,    0,    0,    0,    0,    0,  817,  826,    0,
  817,  826,    0,    0,  817,    0,    0,    0,  826,  817,
    0,  826,  817,    0,    0,  826,    0,    0,    0,    0,
  826,  814,    0,  826,  814,    0,    0,    0,  817,    0,
    0,  814,    0,    0,  814,    0,    0,    0,  814,  826,
    0,    0,    0,  814,    0,    0,  814,    0,    0,    0,
    0,    0,  817,    0,    0,  817,  817,    0,    0,    0,
    0,    0,  814,  826,    0,  817,  826,  826,    0,    0,
    0,    0,    0,    0,    0,    0,  826,    0,    0,    0,
    0,    0,  817, 1393,    0,    0,  814,    0,    0,  814,
  814,    0,    0,  826,    0,    0,    0,    0,    0,  814,
  818,    0,    0,    0,  817,    0,    0,    0,    0,  817,
    0,    0,    0,    6,    0,  826,  814,    0,    0,    0,
  826,  817,    0,  817, 1513, 1514,    0, 1515,    0,    0,
 1516,    0,  826,    0,  826,    0,    0,    0,  814,    0,
    0,    0,    0,  814, 1517,    0,    0,    0,    0,    0,
 1518,    0,    0,    0,    0,  814,  818,  814,    0,  818,
    0,    0,    0,    0,    0,    0,  818,    0,    0,  818,
    0,    0,    0,  818,    0,    0,    0,    0,  818,    0,
    0,  818,    0,  579,    0,    0,    0,  580,    0,    0,
    0,    0,    0,    0,  776,    0,    0,  818,    0,    0,
    0,    0,    0,    0,    0,  581,    0,    0,  582,    0,
 1519,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  818,    0,    0,  818,  818,    0,    0,    0,   14,
    0,    0,    0,    0,  818,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1520,    0,    0,    0,    0,  583,
    0,  818,    0,    0,   16,  584,  585,  586,    0,  587,
  588,  589,    0,  590,  579, 1521,    0,    0,  580,    0,
    0,    0,  102,  818,    0,    0,    0,    0,  818,    0,
    0,    0,    0,  591,    0,  592,  581,    0,    0,  582,
  818,    0,  818,    0,    0,    0,    0,    0,    0,    0,
    0, 1522,  104,    0,  105,    0,    0,    0,    0,    0,
    0,  106,  107,  108,  109,  110,  593,  111,    0,  112,
 1523,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  583,    0,    0,  594,    0,    0,  584,  585,  586,    0,
  587,  588,  589,    0,  590,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  595,    0,    0,    0,    0,
    0,    0,    0,    0,  591,    0,  592,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  596,  597,    0,    0,    0,    0,    0,  593,    0,  103,
  104,    0,  105,    0,    0,    0,    0,  598,    0,  106,
  107,  108,  109,  110,  594,  111,    0,  112,  113,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  595,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  102,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  596,  597,    0,    0,    0,    0,    0,    0,    0,
  103,  104,    0,  105,    0,    0,    0,    0,  598,    0,
  106,  107,  108,  109,  110,    0,  111,    0,  112,  113,
};
static short yycheck[] = {                                       9,
   16,   52,   12,    0,    4,    4,   16,   61,   18,  101,
   60,  250,   53,  346,  132,  133,  250,  143,  506,  132,
  133,  350,   59,   64,   60,   54,  323,  637,   82,  700,
  539,  548,  642,  542,  354,   86,  325,   66,  717,  548,
   50,   51,   52,   53,   54,   55,   87,  281,  511,  354,
   60,   61,   62,   94,   64, 1018,   66,   67,  753,   69,
   70,  740, 1189,   65,   80,   91,  489,  686,   78,  258,
   80,   81,   82,   89,   84,   85,   86,   87,  267,   89,
   90,   91,   92,   70,   94,  695,   88,  123,   67,  125,
   85, 1019, 1231,  371, 1011,  124,  631, 1011,  902,  409,
    0, 1018, 1030,  144, 1018,   92,  318,  359,  302,    0,
 1011,   90, 1011,  123,  124,  125,  145, 1018,  676, 1018,
  146, 1049,   45,  680,  275,  744,    0,  137,    0,    0,
  222,    0,    0,  143,  144,  145,  146,  250,  747,    0,
  944,   41,  137,  842,   44,  703,  183,  386,   45,    0,
   41,  811,  386,   44,    0, 1066,    0,   44,  624,   59,
    0,   41,    0,   44,  773,    0,    0,   41,   59,   41,
   44,  208,   44,  782, 1043,   40,  302,  750,   45,   63,
   41,    0,   40,   44,   44,   59,   41,   59,   59,  518,
   59,   59,  525,  526,  527,  528,  963,   41,   59,    0,
   44,    0,   44,   59, 1144,   63,  415,   41,   59, 1374,
   44,    0,  401,   59, 1374,   59,  252,  309,    0,   59,
   44,   59,    0, 1433,   59,   59,  835,  256,  548,  250,
    0,   61,  720,   44,  264,  286,  265,   45, 1374,   44,
   59,   44,  252,  548,   40,  574,  256,  716,  258,  259,
    0,   44,   91, 1279,  551,  265,  508,  267,   59,   41,
   59,  290,  355, 1067,   40,  338,  302,  877,  878,   40,
   59,   12,   44,  386,   45,   59,  286,   59,    0, 1374,
  290,   59,  258,  409,  903,  265,  387,   63,  324,   59,
  799,   12, 1229,    0, 1257,  331,  377,  333, 1673,  335,
 1675,  579,  338,  600,    0,   41,  455,    0,   44,   59,
  347,   40,  349,  278,  324,  356,  367,  320, 1246, 1548,
  286,  331,   41,  333,  356,  335,  363,  402,  338,  939,
  306, 1148,  320,  300,  405,  265,  365,   59,  895,  440,
 1257, 1716,    0, 1257,    0,   41,  356,  357,   41,    0,
  907,  908,   59,  424,  319,  365, 1257,  367, 1257,  400,
   44,   16,  402,   59, 1335,  386,   59,  448,  397,  442,
  445,  371,  302,  409,  415,  984,   59,   41,  357,  408,
   44,  411,  409,  412, 1613,  352,    0,  397,  417,  130,
  400,  401,  213, 1364, 1263,   59,  819,  429,  408,  492,
 1010,   59,  412,   59,  278,  415,  280,  417,   59,  130,
  420, 1437,   41,    0,    0, 1554,    0,  278, 1793,  280,
 1029,   40,   45,  662, 1191,   80,  287,  318,  662,   44,
    0,    0,  507,  436,  278,  400,  280,  417,  277,   58,
   63,  451,  418,  287,  278,   59,  280,  318,  566,  340,
 1615, 1661,  686,  287,  309, 1615,    0,  258,   41, 1276,
  328, 1398, 1390,  331,  761,    0, 1394,  764, 1396,  340,
  300,  340,   59,   59,  683,   59,  441,  487,   42, 1615,
  331,  778,  671,  383,  305, 1145,  523,  721,   41,   59,
   59,   44,  264,  304,  264,  461,  462,   41, 1071,  304,
   44,    0,  340, 1431,  378,  306,   41,  387,  805,   44,
  744, 1182,  548, 1208, 1653,   59,  387,  378, 1189, 1430,
 1615,  367,  383,  383,   59,  396,  400,  320,  278,  394,
  280, 1471,  569,  407,  378,  458,  318,  547,  548,  400,
  269, 1076,   41,  553,  378,   44,  407,  741,  760,  662,
  328,  258,  261,  331,  410,  436,  400,  846,  340, 1686,
   59,  458,   61,  407,  715,  567,  400,  441, 1707,  440,
 1269,  458,  440,  407,  458, 1444, 1504,  855,  367,  579,
  441,  833, 1510,  620,  621,  454,  454,  623,    0,  440,
  900,  458, 1252,    0, 1060,    0,  436,  441, 1155,  306,
  458,  501, 1729,  454, 1214,  387, 1373,  441,  726, 1748,
  409,  502,  454,  623,  396,  741,  454,  418,  485,  454,
  342,  440,  632,  431, 1234, 1094,  410,  663,  378,  501,
  454, 1559,  632,  632,  680,  454,  320,   44,  756,  757,
  501,  662,  683,  307,  308, 1449,   40,   59, 1576,  452,
  458,  447,   59,  663,   59,  377,  666,  501,  440, 1268,
  431,  671,  440,    0,  313,  998,   41,  501,  387,  903,
  680,   40,  454,  683,  452,  411,  454,  396,  688,  262,
  264,  301,  458,  405,  342,    0,  408,  458,  785,  318,
 1385,  387,  348,  688,  387,  884,  260,  348,  887,  709,
  396, 1672,  424,  318,  318,  741,  735,  717,  737,  719,
  746,  418,  722,  274,  750,  752,  753, 1336, 1681, 1016,
  264,  274,   59,  262,  545,  735,  340,  737,  450, 1062,
  740, 1028, 1660, 1137,  770, 1139,  746,    0, 1296,  303,
  750, 1669,  262,  278,   59,  280,  410,  405,  318,  152,
  408, 1048,  287,    0,  340,  342,  309,  759,  301,  258,
  770,    0,  798,  799, 1681, 1375,  424, 1681,  417, 1232,
  340,  340, 1011,  387,  900,  274,  320, 1011,  273, 1018,
 1681,   44, 1681, 1711, 1018, 1074,  260,  451,  798,  799,
  377,  257,  450,  614,    0,  797,   59,    0,    0, 1061,
  402,  300,   41,  386,  804,  834,  325,  306,  431,   41,
  309,  260,   59,  397, 1485,  844,  816,  262,  405,  283,
   59,  408,  442,   41,  834, 1342,  440,  829,  830,  831,
   45,  283, 1255,  273,  844,  458,  438,  424, 1261,   41,
  257,  460,   44,  378,  885,  268,  856,  386, 1224,  895,
   40,    0,  277,   59,   41,  855,   59,   59,  411,  264,
  260,  907,  888,  450,  900,  400,  386,   41,  334, 1158,
  489,  490,  407,  492,  884,  885,  886,  887,  888,  889,
  499,  500,  501,  502,  503,  895,  505,  262,  507,  508,
  284,   41,   41,    0, 1270,   44,  435,  907,  908,  442,
  340,    0,  889,  303,  319,  466,  441,  886,  452,  312,
   59,  430,  411,  466, 1148,  435,  460,  334,  437,  418,
  345,  328,  259,   44,  331, 1022, 1023,  264, 1046,    0,
  342,  458,  384,  270, 1338, 1614,  343,  436,    0,    0,
 1202,  386,  365,  417, 1273,  489,  490, 1244,  492,  264,
    0,  343,   59,  963,    0,  499,  500,  501,  502,  503,
   59,  505,    0,  507,  508,  377,  501,  466,  417,   40,
   41,  470,   93,   44, 1071,  439,  368,    0, 1275,   41,
  272,  458,   44, 1532, 1472,  400,  445,  439,   59,  281,
   61,    0,  397,  405,   44,   41,  408,   59,   59, 1487,
  493, 1377, 1612, 1512,  456,  348, 1719, 1720,    0,   59,
  503,  386,  424,   59,    0, 1686,   42,  264, 1257,  311,
   91,   59, 1342, 1257, 1634,  301,  441, 1637, 1064, 1039,
 1143, 1144, 1145,  440,  426, 1071,   59, 1342,  450,  322,
 1040, 1040, 1276, 1053,  262,  452,  405,  454,  507, 1382,
   59,    0,   42, 1055, 1064,   41, 1172,  287, 1729, 1175,
  397, 1071,  338,    0, 1190,  328,  460,   59,  331,  322,
  300, 1073,  278,   59,  280,  262,  278, 1079,  280,  318,
  343,   42,  397,  307,  308,  287,   47,  397,  262,   41,
 1578,  460, 1210,  401, 1212,  489,  490,  456,  492,  331,
 1813,  340, 1336, 1400,  260,  499,  500,  501,  502,  503,
   59,  505,  262,  507,  508,  318,  307,  308,  461,  462,
  489,  490,   59,  492,  405,  328,  436, 1137,  331, 1139,
  499,  500,  501,  502,  503, 1689,  505,  340,  507,  508,
  259,  378, 1439, 1143, 1144, 1155, 1386,  303,   40, 1411,
  397,  270,  444, 1271,  383,  387,  448,  449,  450,    0,
  470,  383,    0,  400,  396,  264,  442, 1429,  386, 1171,
  407, 1208,  321,  377,  417,  456,  378,  440, 1732,  328,
 1190, 1191,  331,    0,  387,  260,  335,  258,  480,  452,
 1566,  454, 1432,  396,  343,    0, 1572, 1746,  400,  386,
   41,  444, 1812,  274,  441,  407,  277,  278,  403,  280,
  359,  318,  386,  284,  418,  301,  287,  278,   59,  280,
 1774,   59,   44, 1777,  331,  420,  264,  451,  303,  300,
 1718, 1780, 1781,  340,  383,  306,  386,  440,  309,  441,
 1356,  264,   59,  458, 1360,  460, 1800,   44, 1374, 1286,
 1250, 1250,  338, 1290,   59,  264,  318, 1806,  265,  321,
  451,  465,   59,    0,  413, 1663,  328, 1393, 1817,  331,
  460, 1820,  264,  335,  489,  490, 1573,  492,  340,  385,
  387,  343,    0,   58,  499,  500,  501,  502,  503,  396,
  505,  440,  507,  508,    0,  302,    0,  359,  397,  489,
  490,    0,  492,  452,   41,  454,  296,  378,    0,  499,
  500,  501,  502,  503,    0,  505, 1714,  507,  508,  280,
  263,  383,   59,   41,  285,  387,    0,  264, 1338,  400,
  337,  279,  318,  440,  396,   41,  407,    0,   44, 1607,
  411,   59, 1392, 1611,    0,  331,  383,  418, 1385,   41,
  642,  413,   44,   59,  340,   59, 1392, 1393, 1471,  397,
   59, 1759,   44, 1373,  383,  436, 1482,   59,   44,  312,
  441,  272, 1401,   59,  397,  258,  328,    0,  440,  331,
  323,  329, 1392,  258,  436,   59,  261,  288,  397, 1507,
  452, 1401, 1763, 1764,  686,  466,   59,  689,  460,  470,
 1668,  387,  694,   59,   44,  397, 1532,  699,  700,  280,
  396,   93,  321,    0,  285,  363,  708,   93,  710,  711,
  712,  713,  714,  306,  716,  301,  335,  489,  490,  721,
  492,  306,  333,  381,  460,  387,   59,  499,  500,  501,
  502,  503, 1681,  505,  396,  507,  508, 1681,  402,    0,
  359, 1822,  744,   93,  440,    0,  458,  749,  260, 1513,
  397,  362,  338,  489,  490, 1557,  492,    0,  454,  494,
  460, 1471,   59,  499,  500,  501,  502,  503, 1478,  505,
  434,  507,  508,  485,  427,  316,  317,  328,    0, 1615,
  331,    0,  337,  417, 1762,  449, 1532, 1665,    0,  489,
  490,  303,  492, 1513,  413,    0,    0, 1558,   59,  499,
  500,  501,  502,  503,   59,  505, 1642,  507,  508, 1516,
  444,  465,  418,  467, 1553,  342,   59,  274,  429,   41,
   59,  407,   44,    0,    0,  418,    0,    0, 1548,   41,
  318,  417,   44, 1553, 1712, 1582,  387,   59, 1558, 1586,
   59,   42,   43,  436,   45,  396,   47,   59,  259,   61,
  377, 1577, 1654, 1689,   59,   59,  442, 1577,   40,  270,
  862,  863,  864,    0,  424,  867,  868,  869,    0,  871,
  872,  873,  874,  875,  876,  264, 1627,  470,  405,   91,
 1758,  408,   59,   59,  331,   59,   59,    0,    0,   45,
  318,    0,   41, 1613, 1614,   44, 1732,  424,  260,    0,
  328,  903,  318,  331,    0, 1741,    0, 1627,  910,  911,
 1746,  913,  340,  915, 1792,  331,  918,  919,  920,  921,
  922,  923,   59,  450,  340,  461,    0,   59,   41,   41,
    0,   44,   44,  935, 1698,  937,  318, 1663, 1774,  301,
  387, 1777,   44, 1663, 1780, 1781,   59,   59,  352,  396,
   59,  313,  279,  857, 1680,  859,  337,   59,   59,  387,
 1680,  337,  342,   59, 1800,   59,   41,  260,  396,   44,
 1806,  387,    0,  300,  378,  460,  338,   46, 1698, 1699,
  396, 1817,  316,    0, 1820,   59,  461,  462, 1714,   59,
 1700, 1700,   61,  374, 1714, 1721,   41,  377,  374,   44,
 1746, 1721,  329,  452,  489,  490,  387,  492,    0, 1770,
  303,  387,  440,  337,  499,  500,  501,  502,  503,  232,
  505,  348,  507,  508,  440,  405,  454,  240,  408,  460,
  357,   59,    0, 1759, 1780, 1781,  363,   41,  454, 1759,
   44, 1767,   59,  276,  424,  407,  258, 1767,   93,   41,
 1770,  284,   44,  336,  381,  417,  278, 1137,  280, 1139,
 1806,   41,  274,   46,   44,  277,  278,   59,  280,   40,
  450, 1817,  284,   41, 1820,  287,   44, 1797,   61,  280,
  442,    0,  402,   41,  285, 1805,   44,   41,  300,   93,
   44,   59, 1094,  320,  306,   42,   43,  309,   45,  356,
   47,  348,  340, 1823, 1106, 1107,    0, 1109, 1110,  318,
  357,  402,   59,   42,   43, 1117,   45,   41,   47,  320,
   44,   40,   41,   42,   43,   44,   45,   46,   47,   42,
   43,  321,   45,  383,   47,  383,  393,  506,  395,  295,
   59,   60,   61,   62,   41,  335, 1148,   44,  457,  458,
  459,  436,  685,    0,  687,  361,  378,  690,  691,  692,
 1162,  418,  493,  494,  450,   59,  378,  323, 1170,  359,
 1172,  434,   91, 1175,   93, 1177,  485,  486,  400, 1181,
 1182, 1183, 1184,  391,  392,  407,  443, 1189,  400,   41,
  461,  462,   44,  383,   41,  407,  352,   44,   41,  411,
   45,   44,  417,  357,   41,  318,  418,   44,    0,  287,
   42,   43,   59,   45,   41,   47,  442,   44,   63,  441,
  411,  460,   44,  413,  436, 1227,   61,  340,  469,  441,
  372,    0,   42,   43,   44,   45,  867,   47,  869,   41,
  871,  872,   44,  874,  875,  876,   41,  286,  264,   44,
  489,  490,   44,  492,  466,   44,   41,   40,  470,   44,
  499,  500,  501,  502,  503,   40,  505,   59,  507,  508,
   41,  427,   41,   44, 1276,   44,   42,   43,  461,   45,
   41,   47,   41,   44,  436,   44,  278,  443,  280,  461,
   59, 1293,   61,  461,  320,  287,   44,  402,   40,  321,
 1302,  457,  458,  459,   40,  461,  462, 1309,  300,   40,
  278,   40,  280,  335,   40,   42,   43,   40,   45,  287,
   47,  343,   40,   40,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  280, 1336,    0,   40,  359,  285,  495,
  496,   40,   40,  262,   40,  264,  265,   40,  504,   46,
   91,  280,   44,  272, 1356,  320,  285,   44, 1360,    0,
  428,  280,   46,   40,  264,   44,  285,  280,  382,  288,
   61,  348,  285,   44,  278,  278,   41,   42,   43,   44,
   45,   46,   47,  302,  300,  279,  378,  264,  307,  308,
  397,  413,  311,  340,   59,   60,   61,   62,  436,  318,
   41,  320,  321,   44,  470,  324,  300,  320,  400,  328,
  378,  340,  331,  397,  333,  407,  335,  458,   59,  294,
  321,  340,  341,  277,  343,  399,   91,  447,   93,  447,
  305,   40,  400,   40,  335,  329,   91,  312,  349,  407,
  359,  360,   40,  362,   40,  318,  264,  383,  280,  441,
  295,  411,  321,  285,  348, 1457, 1458,  376,  359,  378,
  379,  380,  337,  357,  383,   44,  335,  386,  387,  363,
  280,  318,  389,  441,  343,  285,  382,  396,   40,  311,
 1482,  328,  383, 1485,  331,   40,  436,  381,  464,  258,
  359,  410,  317,  340,  413,   40,  343,  320,  451, 1374,
  320,    0,   41,  452,  448,  502,  264,  352,  277,  278,
  429,  280,  413,    0,  280,  284,  435, 1519,  287,  285,
  454,  440,   41,   42,   43,  458,   45,  402,   47,  321,
   40,  300,  451,  452,  277,  454,  328,  306,  265,  331,
  387,  460,  274,  335,  413,  311,   40,  300,  344,  396,
  284,  343,    0,  280,   41,  386,  344,   44,  285,  434,
   59,  418,  278,  438,   40,  386,  416,  359,  417,  386,
  489,  490,   59,  492,   61,  302,  300,  418,  356, 1581,
  499,  500,  501,  502,  503,   40,  505, 1589,  507,  508,
   40,  383,  274,  440,  385,   44,  264,  262,  417,  264,
  265,   44,   41,   42,   43,  452,   45,  272,   47,  378,
  367,   59,  457,  458,  459,  280,  461,  462,  320,   41,
  285,  413,  280,  288, 1626,   44,  418,   44, 1630,   40,
  320,  400,   44,    0,  328,  480,  481,  302,  407,  304,
  485,  486,  307,  308,   44,  505,  311,  416,  440,  418,
  266,   40,  299,  318,   40,  320,  321,  501,  264,  324,
  452,   41,  454,  328,   58,   41,  331,  436,  333,   41,
  335,  422,  441,   41,   41,  340,  341,  318,  343,   41,
   42,   43,    0,   45, 1686,   47, 1688,  328,  264, 1691,
  331,    0,   59,  440,  359,  360,  309,  362,  454,  340,
   41,  470,  343,   41,   41,   42,   43,  309,   45,  394,
   47,  376,  492,  378,  379,  380, 1642,   40,  383,   40,
  352,  386,  387,   41,   40,   40,   44, 1729,   40,   40,
   40,  396,   41,   42,   43,   44,   45,   46,   47,   40,
 1615,   59,   40,   61,   40,  410,  387,   40,  413,   40,
   59,   60,   61,   62,   40,  396,   40,   40,   40,  262,
  327,  280,  435, 1689,  429,   44,  285, 1642,  308,   41,
  435,  258,  343,  388,  388,  440,  388,  274,  331,  422,
  279,  391,   91,  446,   93,  390,  451,  452,  271,  454,
  277,  278,  287,  280,  386,  460,  407,  284,  386,  440,
  287,  300,  386,   41,   42,   43, 1732,   45,  386,   47,
   41,  452,  386,  300, 1689, 1741,  386,  378,   41,  306,
  264,  278,   40,  417,  489,  490,    0,  492,   41,   41,
  329,  279,   41,  385,  499,  500,  501,  502,  503,  436,
  505,  280,  507,  508,   41,  308,  285,  396, 1774,  348,
  435, 1777,  300,  280,  338,  337,  442, 1732,  357,  442,
  301,  338,   41,  301,  363,   41, 1741,   41,   42,   43,
   44,   45,   46,   47, 1800,  320,  385,  352,  371,  371,
   41,  329,  381,   41,   41,   59,   60,   61,   62,   41,
   41,  378,   42,   43,   44,   45,   41,   47,   41, 1774,
  348,   41, 1777,   41,   41,   41,   41,   41,  356,  357,
   41,   41,   41,  400,  280,  363,   44,   91,  280,   93,
  407,   46,   40,  285,  340, 1800,   41,   42,   43,   40,
   45,  418,   47,  381,  340,   41,   42,   43,   59,   45,
  258,   47,   59,  280,  308,  393,   59,  395,  285,  436,
   59,  318,   40,  262,  441,  264,  265,   45,   59,   59,
  278,  328,  280,  272,  331,   59,  284,   59,   41,  287,
  418,  280,   41,  340,  352,   63,  285,  368,   40,  288,
  340,  344,  300,  470,  451,  383,   44,   59,  306,   59,
  383,  383,   41,  302,  343,  443,   41,   41,  307,  308,
  410,   44,  311,   44,  435,  304,  301,  442,  387,  318,
   40,  320,  321,   59,  304,  324,  460,  293,   44,  328,
  387,  463,  331,   44,  333,   41,  335,  307,    0,  396,
  337,  340,  341,  383,  343,   41,   42,   43,  383,   45,
   40,   47,  280,  264,  278,  489,  490,  285,  492,  264,
  359,  360,  320,  362,  458,  499,  500,  501,  502,  503,
  378,  505,  389,  507,  508,  264,  451,  376,    0,  378,
  379,  380,  411,  440,  383,  470,   40,  386,  387,  309,
  271,  458,  400,  264,  436,  452,  343,  396,  262,  407,
  264,  265,  435,   41,   42,   43,  343,   45,  272,   47,
  418,  410,   42,   43,  413,   45,  280,   47,  343,   41,
  343,  285,   44,  308,  288,  271,  458,   41,  436,   40,
  429,  308,   40,  441,   59,   59,  435,   59,  302,   41,
  280,  440,   59,  307,  308,  285,  308,  311,  304,   59,
   41,   44,  451,  452,  318,  454,  320,  321,   59,   41,
  324,  460,  470,    0,  328,    0,    0,  331, 1140,  333,
   12,  335,  267,  260,  259,  280,  340,  341, 1076,  343,
  285,  671,  847,  627,  280, 1074,   84, 1063, 1071,  285,
  489,  490,    0,  492, 1419,  359,  360,  351,  362, 1420,
  499,  500,  501,  502,  503,  321,  505,  316,  507,  508,
  574, 1001,  376,  307,  378,  379,  380,  295,  746,  383,
  333, 1053,  386,  387,   41,   42,   43,  333,   45, 1624,
   47,  780,  396,   41,   42,   43,   44,   45,   46,   47,
   42,   43, 1398,   45, 1064,   47,  410, 1705, 1400,  413,
  553,   59,   60,   61,   62, 1709, 1708,   59,   41,   42,
   43,    0,   45, 1750,   47,  429,  770, 1632, 1225, 1805,
 1647,  435,  386, 1747,  352,  986,  440, 1204, 1378,  261,
 1663,  338,  326,   91,  280,   93,  826,  451,  452,  285,
  454,  555,  319,  613, 1259,  818,  460,   41,   42,   43,
  282,   45,   13,   47,  495,   41,   42,   43,  290,   45,
  420,   47,  491,  294,  135,  297,  735,  299,  721,  301,
   59,  963,  749,  722,  306,  489,  490,    0,  492, 1615,
 1721,  313,  262, 1502, 1767,  499,  500,  501,  502,  503,
 1823,  505,  280,  507,  508,  327,  506,  285,  904,  679,
  280, 1108,   -1,    0, 1177,  285,  338,   41,   42,   43,
  385,   45,   -1,   47,   -1,   -1,   -1,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,  457,
  458,  459,   -1,  461,  462,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   41,   -1,  318,   44,   -1,  321,
   -1,   -1,  480,  481,   -1,   -1,  328,  485,  486,  331,
   -1,   -1,   59,  335,   -1,   -1,   -1,   -1,  340,   -1,
   93,  343,   -1,  405,   -1,   41,   42,   43,   -1,   45,
  412,   47,   -1,  415,   -1,  417,  418,  359,   -1,   -1,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  442,  383,   -1,  280,  272,  387,   42,   43,  285,   45,
   -1,   47,  280,   -1,  396,   -1,   -1,  285,  280,   -1,
  288,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,  302,   -1,   -1,  280,   -1,  307,
  308,   -1,  285,  311,   -1,   -1,   -1,    0,   -1,  491,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,  440,  258,
  328,   -1,   -1,  331,  506,  333,   -1,  335,   -1,   -1,
  452,   -1,  340,  341,   -1,  343,  280,   -1,   -1,   -1,
  279,  285,   -1,   -1,  280,    0,   -1,   -1,   -1,  285,
   -1,  359,  360,   -1,  362,   -1,   41,   42,   43,   -1,
   45,  300,   47,   -1,   -1,   -1,   59,  306,  376,    0,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   42,   43,   -1,   45,   -1,   47,  396,  262,
  329,  264,  265,   -1,   -1,    0,  280,   -1,   -1,  272,
   -1,  285,  410,   -1,   59,  413,   -1,  280,   -1,  348,
   41,   -1,  285,   44,   -1,  288,   -1,  356,  357,   -1,
   -1,  429,   -1,   -1,  363,   -1,   -1,  435,   59,  302,
   -1,  278,  440,  280,  307,  308,   41,  284,  311,   44,
  287,   -1,  381,  451,  452,  318,  454,  320,  321,   -1,
   -1,  324,  460,  300,   59,  328,   -1,    0,  331,   -1,
  333,   -1,  335,   -1,  280,   -1,   -1,  340,  341,  285,
  343,  472,  473,  474,  475,  476,  477,  478,  479,  418,
  280,  489,  490,    0,  492,  285,  359,  360,   -1,  362,
  429,  499,  500,  501,  502,  503,   -1,  505,   41,  507,
  508,   44,   -1,  376,  280,  378,  379,  380,   -1,  285,
  383,   -1,   -1,  386,  387,   -1,   59,   -1,  287,   -1,
   -1,   -1,   -1,  396,   41,   42,   43,   44,   45,   -1,
   47,  378,   -1,   -1,   -1,   -1,   -1,  410,    0,   -1,
  413,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  400,   -1,   -1,  429,   -1,   -1,   -1,
  407,   -1,  435,   -1,   -1,  258,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   93,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,  279,  460,   -1,   -1,
   -1,   -1,   -1,   41,  441,  280,   44,   59,   -1,   -1,
  285,   -1,  262,  258,   -1,   -1,   -1,  300,   -1,   -1,
   -1,   59,   -1,  306,   -1,   -1,  489,  490,    0,  492,
  280,   -1,   -1,   -1,  279,  285,  499,  500,  501,  502,
  503,   -1,  505,   -1,  507,  508,  329,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,
   -1,  306,   -1,   -1,   -1,  348,   -1,   -1,   -1,   41,
   42,   43,   44,   45,  357,   47,   -1,   -1,   -1,   -1,
  363,   -1,   -1,  278,  329,  280,   -1,   59,   60,   61,
   62,   -1,  287,   -1,   -1,   -1,   -1,  318,  381,   -1,
  321,  460,   -1,  348,   -1,  300,   -1,  328,   -1,   -1,
  331,   -1,  357,   -1,  335,   -1,   -1,   -1,  363,  340,
   -1,   93,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,  418,  381,   -1,  359,   -1,
  499,  500,  501,  502,  503,  278,  505,  280,  507,  508,
   -1,  284,   -1,   -1,  287,  262,   -1,  264,  265,   -1,
   -1,   -1,  383,   -1,   -1,  272,  387,  300,   -1,   -1,
   -1,   -1,   -1,  418,   -1,  396,   -1,   -1,  285,   -1,
   -1,  288,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   -1,  311,  400,  258,   -1,   -1,   -1,
   -1,  318,  407,  320,  321,   -1,   -1,  324,   -1,  440,
  258,  328,   -1,   -1,  331,   -1,  333,  279,  335,   -1,
   -1,  452,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,  279,   -1,   -1,   -1,  378,  441,   -1,  300,   -1,
   -1,   -1,  359,  360,  306,  362,   -1,   -1,   -1,   -1,
   -1,   -1,  300,   -1,   -1,   -1,   -1,  400,  306,  376,
   -1,  378,  379,  380,  407,   -1,  383,  329,   -1,  386,
  387,  287,   -1,    0,   -1,   -1,   -1,  328,   -1,  396,
  262,  329,  264,  265,   -1,   -1,  348,   -1,   -1,   -1,
  272,   -1,   -1,  410,   -1,  357,  413,   -1,  441,   -1,
  348,  363,   -1,   -1,   -1,   -1,  288,   -1,   -1,  357,
   -1,   -1,  429,   -1,   41,  363,   -1,   44,  435,  381,
  302,   -1,   -1,  440,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   59,  381,  451,  452,  318,  454,  320,  321,
   -1,   -1,  324,  460,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,  418,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  418,   -1,  489,  490,    0,  492,   -1,  359,  360,   -1,
  362,  343,  499,  500,  501,  502,  503,   -1,  505,  294,
  507,  508,  443,   -1,  376,   -1,  378,  379,  380,   -1,
  305,  383,   -1,   -1,  386,  387,  368,  312,   -1,  460,
   -1,   -1,   -1,   -1,  396,   41,   42,   43,   44,   45,
  325,   47,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   59,   60,   61,   62,   -1,  489,  490,
   -1,  492,   -1,   -1,  460,   -1,   -1,  429,  499,  500,
  501,  502,  503,  435,  505,   -1,  507,  508,  440,   -1,
   -1,   -1,   -1,   -1,  426,   -1,   -1,   93,   -1,  451,
  452,   -1,  454,  489,  490,   -1,  492,   -1,  460,   -1,
   -1,  386,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,  507,  508,   -1,   -1,   -1,   -1,  402,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,    0,
  492,   -1,   -1,   -1,  419,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  507,  508,  489,  490,  434,
  492,   -1,   -1,  438,   -1,  262,  441,  499,  500,  501,
  502,  503,   -1,  505,  449,  507,  508,   -1,   -1,   -1,
   41,   -1,   43,   44,   45, 1294, 1295,   -1,   -1,  260,
   -1, 1300, 1301,  468, 1303, 1304,   -1,   -1,   59,   60,
   61,   62, 1311, 1312, 1313, 1314,   -1, 1316, 1317, 1318,
 1319, 1320, 1321,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
  301,  328,   93,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,  313,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  327,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  262,  338,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,  288,   -1,   -1,   -1,   60,   61,   62,  396,
   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   -1,   -1,  311,  413,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,  407,  333,  435,  335,
   -1,   -1,   -1,  440,  340,  341,  417,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,  460,
  386,  387,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
  396,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,  410,   -1,   -1,  413,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,  288,  499,  500,
  501,  502,  503,  429,  505,   41,  507,  508,   44,  435,
   -1,  302,   -1,   -1,  440,   -1,  307,  308,   -1,   -1,
  311,   -1,   -1,   59,   -1,  451,  452,  318,  454,  320,
  321,   -1,   -1,  324,  460,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,  328,   -1,   -1,
   -1,   -1,   -1,  489,  490,    0,  492,   -1,  359,  360,
   -1,  362,   -1,  499,  500,  501,  502,  503,  272,  505,
   -1,  507,  508,   -1,   -1,  376,  280,  378,  379,  380,
   -1,  285,  383,   -1,  288,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,  413,   -1,   59,   60,   61,   62,   -1,   -1,
  324,  402,   -1,   -1,   -1,   -1,   -1,   -1,  429,  333,
   -1,   -1,   -1,   -1,  435,   -1,   -1,  341,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   93,   -1,
  451,  452,   -1,  454,   -1,   -1,  360,  438,  362,  460,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  449,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  468,  489,  490,
    0,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  507,  508,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  262,   -1,  499,  500,
  501,  502,  503,   -1,  505,  429,  507,  508,   -1,   -1,
   -1,   41,  273,   43,   44,   45,  277,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,  295,   -1,   -1,  298,  305,   -1,
   -1,   -1,   -1,   -1,   -1,  312,   -1,   -1,   -1,  316,
  317,   -1,  318,   -1,   -1,  321,   -1,   -1,  325,   -1,
   -1,   -1,  328,   93,   -1,  331,   -1,   -1,   -1,  335,
  337,   -1,   -1,   -1,  340,   -1,   -1,  343,  339,   -1,
   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  359,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,  402,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,  406,  311,  413,   -1,   -1,
   -1,   -1,  419,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,  423,  328,   -1,   -1,  331,  434,  333,  435,
  335,  438,   -1,   -1,  440,  340,  341,   -1,  343,   -1,
   -1,   -1,  449,   -1,  445,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,
   -1,  468,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,  480,
  481,  386,  387,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  396,  262,   -1,  264,  265,  497,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,  429,   41,   -1,   -1,   44,   -1,
  435,   -1,  302,   -1,   -1,  440,   -1,  307,  308,   -1,
   -1,  311,   -1,   59,   -1,   61,  451,  452,  318,  454,
  320,  321,   -1,   -1,  324,  460,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,  260,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,    0,  492,   -1,  359,
  360,   -1,  362,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  507,  508,  443,   -1,  376,  301,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,  313,
   -1,  460,   -1,   -1,   -1,   -1,  396,   41,   -1,   43,
   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,  338,   59,   60,   61,   62,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,  460,   -1,  429,
  499,  500,  501,  502,  503,  435,  505,   -1,  507,  508,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   93,
   -1,  451,  452,   -1,  454,   -1,  489,  490,   -1,  492,
  460,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   40,  507,  508,   43,   -1,   45,   -1,
   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,  489,
  490,   58,  492,  417,   -1,   -1,   63,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,
   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,  442,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  278,  273,  280,   -1,  460,  277,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  291,   -1,   -1,  300,  295,   -1,   -1,  298,   -1,
  306,   -1,   -1,  309,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,  434,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,  460,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,  354,  272,   -1,
   -1,   -1,  378,  373,   -1,  375,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  288,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,  400,  507,  508,   -1,  302,   -1,
   -1,  407,   -1,  307,  308,  411,  406,  311,   -1,   -1,
   -1,  411,  418,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,  423,  328,   -1,   -1,  331,   -1,  333,
  436,  335,   -1,   -1,   -1,  441,  340,  341,   -1,  343,
  267,   -1,   -1,   40,   -1,  445,   43,   -1,   45,  276,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
  466,   58,  289,   -1,  470,  292,   63,   -1,  295,  296,
   -1,  458,  376,  460,  378,  379,  380,   -1,   -1,  383,
  480,  481,  386,  387,  471,   -1,   -1,  314,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,  323,  497,   -1,  326,
   -1,   -1,  489,  490,   -1,  492,  410,   -1,   -1,  413,
   -1,  498,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,    0,   -1,   -1,  429,   -1,  354,  355,  460,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  451,  452,   -1,
  454,  378,   -1,   -1,   -1,   -1,  460,   -1,  489,  490,
   -1,  492,   40,   41,   -1,   -1,   44,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  507,  508,  460,   -1,
   -1,   59,   -1,   61,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,  421,  499,  500,  501,  502,  503,
  427,  505,   -1,  507,  508,   -1,  433,  489,  490,   -1,
  492,   -1,   -1,   91,   -1,   -1,  443,  499,  500,  501,
  502,  503,   -1,  505,   -1,  507,  508,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,  471,   -1,  277,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  295,  492,   -1,  298,  495,  496,
  267,  498,  499,  500,  501,  502,  503,  504,  505,  276,
  507,  508,  509,  510,  511,   -1,   -1,   -1,   40,   -1,
   -1,   43,  289,   45,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  339,   -1,
   -1,   63,   -1,   -1,  345,  346,  347,  314,  349,  350,
  351,   -1,  353,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,  406,   -1,   -1,   -1,   -1,
  258,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,   -1,  274,   -1,   -1,  277,
  278,   -1,  280,   -1,   -1,   41,  284,   -1,   44,  287,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
  417,   -1,  300,   59,  421,   61,   -1,   -1,  306,   -1,
  427,  309,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  497,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  378,  498,  499,  500,  501,  502,  503,  504,  505,    0,
  507,  508,  509,  510,  511,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  400,   -1,  276,   -1,   -1,   -1,   -1,  407,
   -1,   -1,   -1,  411,   -1,   -1,   -1,  289,   -1,   -1,
  418,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  436,   -1,
   -1,   -1,  314,  441,   -1,   -1,   -1,   -1,   59,   -1,
   61,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,
   -1,   -1,  470,   40,   -1,   42,   43,   -1,   45,   -1,
   91,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  366,   -1,   63,   -1,  370,   -1,
   -1,   -1,  258,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,  421,
  306,   -1,   -1,  309,   -1,  427,   -1,   40,   -1,   42,
   43,  433,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,  378,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  507,  508,  509,  510,  511,
   -1,   -1,   -1,   -1,  400,   -1,   -1,  258,   -1,   -1,
   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,
   -1,   -1,  418,  274,   -1,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,  300,
   -1,   -1,   -1,  260,   -1,  306,   -1,   -1,  309,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   41,   -1,   -1,   44,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
   59,   -1,   -1,   -1,  267,  352,   -1,  354,  355,  400,
   -1,   -1,   -1,  276,   -1,   -1,  407,   -1,   -1,  366,
  411,   -1,   -1,  370,   -1,   -1,  289,  418,   -1,   -1,
   -1,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
  303,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   58,
  441,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,  318,
   -1,   -1,  321,   -1,   -1,   -1,   -1,  276,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
  289,  340,   -1,   -1,  343,   40,  295,  296,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   -1,    0,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   41,   -1,   40,
   44,   -1,   43,   -1,   45,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   59,   -1,   58,   -1,   -1,
   -1,   -1,   63,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,  273,   -1,  495,  496,  277,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
   -1,   -1,  291,   -1,   -1,  260,  295,   -1,  263,  298,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,  262,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,  406,  289,   -1,
   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  318,   -1,   63,  321,   -1,   -1,
   -1,   -1,  323,   -1,  328,  326,  445,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,  354,  355,  359,   -1,   -1,  443,   -1,
   -1,  480,  481,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,  457,  458,  459,  460,  461,  462,  497,  383,
   -1,   -1,  386,  387,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,  413,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  507,  508,  425,   -1,  427,   -1,   -1,   -1,
   -1,  435,  433,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  507,  508,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,  295,  296,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   40,   44,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,  273,   -1,  495,  496,
  277,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,   -1,   -1,  291,   -1,   -1,  260,  295,   -1,
  263,  298,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
  262,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,  406,
  289,   -1,   -1,   -1,   -1,   40,  295,  296,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  318,   -1,   63,  321,
   -1,   -1,   -1,   -1,  323,   -1,  328,  326,  445,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,  354,  355,  359,   -1,   -1,
  443,   -1,   -1,  480,  481,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,  457,  458,  459,  460,  461,  462,
  497,  383,   -1,   -1,  386,  387,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,  413,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  507,  508,  425,   -1,  427,   -1,
   -1,   -1,   -1,  435,  433,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   58,   -1,   44,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
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
  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  267,   -1,
  335,   -1,   -1,  354,  355,  340,   -1,  276,  343,   -1,
   -1,   -1,   -1,   40,   -1,  366,   43,   -1,   45,  370,
  289,   -1,   -1,   -1,  359,   -1,  295,  296,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,  413,   -1,
   -1,   -1,  433,  352,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,  366,   -1,   -1,
  435,  370,   -1,   -1,   -1,  440,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  507,  508,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
  267,   -1,   -1,   40,   -1,   42,   43,   -1,   45,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  289,   -1,   -1,  460,   63,   -1,  295,  296,
   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,  323,  492,   -1,  326,
  495,  496,   -1,   -1,  499,  500,  501,  502,  503,  504,
  505,  506,  507,  508,  509,  510,  511,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  352,   40,  354,  355,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  267,  498,  499,  500,  501,  502,  503,  504,  505,  276,
  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,  352,   40,  354,  355,   43,
   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   58,  289,   -1,   -1,   -1,   63,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   40,   -1,  370,   43,   -1,   45,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  471,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  271,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,  301,   -1,   -1,   -1,   -1,  443,
   -1,  308,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,  318,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,  332,   -1,  471,   -1,   -1,
   -1,  338,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  267,  498,  499,  500,  501,  502,  503,
  504,  505,  276,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  323,
  417,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,  451,   41,  453,   -1,   44,   -1,
   -1,  267,  366,  460,   40,   -1,  370,   43,   -1,   45,
  276,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  289,   -1,   -1,   -1,   63,   -1,  295,
  296,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  354,  355,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,  267,  498,  499,  500,  501,  502,  503,  504,  505,
  276,  507,  508,   40,   41,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,    0,   -1,  331,   -1,  323,   -1,  335,
  326,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,   41,   -1,   40,   44,   -1,   43,
  366,   45,   -1,   -1,  370,   -1,   -1,  383,   -1,   -1,
  386,  387,   59,   -1,   58,   -1,   -1,   -1,   -1,   63,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,  427,   -1,   -1,  440,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,  443,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   40,  354,  355,   43,
   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   58,  289,   -1,   -1,   -1,   63,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,  323,
   -1,  328,  326,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
  354,  355,  359,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   40,   -1,  370,   43,   -1,   45,
  457,  458,  459,  460,  461,  462,  383,   -1,   -1,  386,
  387,   -1,   58,   -1,  471,   -1,   -1,   63,   -1,  396,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,  413,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,   -1,  427,   -1,   -1,   -1,   -1,  435,  433,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
  454,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  267,  498,  499,  500,  501,  502,  503,
  504,  505,  276,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  460,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,  323,
  492,   -1,  326,  495,  496,   -1,   -1,  499,  500,  501,
  502,  503,  504,  505,  506,  507,  508,  509,  510,  511,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  276,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   59,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,  427,   -1,   42,   43,   -1,   45,  433,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   60,   61,   62,   -1,   -1,  354,  355,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,  507,  508,   40,   -1,   -1,   43,   -1,   45,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,  323,   -1,   -1,  326,
   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,  354,  355,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   58,   -1,   -1,  324,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,  333,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  341,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  427,  378,  379,  380,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  267,  498,  499,  500,  501,  502,  503,  504,  505,  276,
  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,  352,   40,  354,  355,   43,
   59,   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   58,  289,   -1,   -1,   -1,   63,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  271,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,   -1,  427,   -1,   -1,   -1,   -1,    0,  433,
   -1,   -1,   -1,   -1,  301,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,  318,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,  332,   -1,  471,   -1,   41,
   -1,  338,   44,  262,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   59,  492,   -1,
   -1,  495,  496,  267,  498,  499,  500,  501,  502,  503,
  504,  505,  276,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  289,   -1,   -1,    0,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,  398,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,
   -1,    0,  331,   -1,   -1,   -1,  335,   -1,   -1,  323,
  417,  340,  326,   41,  343,   -1,   44,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,  432,   -1,   -1,   -1,   -1,
  359,   59,   -1,   -1,   -1,  442,   -1,   59,   -1,   -1,
  354,  355,   41,   -1,  451,   44,  453,   -1,   -1,   -1,
   -1,   -1,  366,  460,  383,   -1,  370,  386,  387,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,  413,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,    0,   -1,  452,   -1,  454,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
  262,   -1,   -1,   -1,    0,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   59,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   59,   -1,   -1,  328,    0,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  262,   -1,   -1,   -1,    0,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,  318,   -1,   -1,  321,
  328,  413,   -1,  331,   -1,   -1,  328,  335,   41,  331,
   -1,   44,  340,  335,   -1,  343,   -1,   -1,  340,  318,
   -1,  343,  321,  435,   -1,   -1,   59,   -1,  440,  328,
   -1,  359,  331,   -1,   -1,   -1,  335,  359,   -1,   -1,
  452,  340,  454,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
  359,  383,   -1,   -1,  386,  387,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,  383,  413,   -1,  386,  387,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  396,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
    0,   -1,  440,  435,  413,   -1,  262,   -1,  440,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
  452,   -1,  454,    0,   -1,   -1,  435,   -1,   -1,  318,
   41,  440,  321,   44,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   41,  331,  452,   44,  454,  335,   -1,   59,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   59,
   -1,   -1,  318,   -1,   41,  321,   -1,   44,   -1,  262,
  359,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   59,   -1,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,   -1,  413,  318,   -1,  383,  321,   -1,
  386,  387,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  396,   -1,  335,   -1,   -1,   -1,  435,  340,   -1,   -1,
  343,  440,   -1,   -1,   -1,   -1,   -1,  413,    0,   -1,
   -1,   -1,   -1,  452,   -1,  454,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  435,
   -1,   -1,   -1,   -1,  440,  328,   -1,   -1,  331,   -1,
  383,   -1,  335,  386,  387,   -1,  452,  340,  454,   41,
  343,   -1,   44,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,   59,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,  435,  386,  387,   -1,   -1,  440,   -1,   -1,
   -1,  262,   -1,  396,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  452,
  321,  454,   -1,   -1,   -1,   -1,   -1,  328,  318,   -1,
  331,  321,   -1,   -1,  335,   -1,   -1,   -1,  328,  340,
   -1,  331,  343,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,  318,   -1,  343,  321,   -1,   -1,   -1,  359,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  359,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,  359,  383,   -1,  396,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,  413,  271,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  396,
  262,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,  301,   -1,  435,  413,   -1,   -1,   -1,
  440,  452,   -1,  454,  312,  313,   -1,  315,   -1,   -1,
  318,   -1,  452,   -1,  454,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  332,   -1,   -1,   -1,   -1,   -1,
  338,   -1,   -1,   -1,   -1,  452,  318,  454,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,   -1,  284,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,  417,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,  339,
   -1,  413,   -1,   -1,  442,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,  273,  453,   -1,   -1,  277,   -1,
   -1,   -1,  460,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,  373,   -1,  375,  295,   -1,   -1,  298,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  406,  505,   -1,  507,
  508,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  339,   -1,   -1,  423,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,  406,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,
  500,  501,  502,  503,  423,  505,   -1,  507,  508,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  507,  508,
};
#define YYFINAL 19
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 511
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
"KW_DEBUG","DECIMAL","DECLARE","DEFAULT","DELETE","DESC","DISTINCT","DO",
"DOMAIN","DROP","ELSE","END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE",
"EXISTS","EXIT","EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION",
"GDSCODE","GEQ","GENERATOR","GEN_ID","GRANT","GROUP","GROUP_COMMIT_WAIT","GTR",
"HAVING","IF","IN","INACTIVE","INNER","INPUT_TYPE","INDEX","INSERT","INTEGER",
"INTO","IS","ISOLATION","JOIN","KEY","KW_CHAR","KW_DEC","KW_DOUBLE","KW_FILE",
"KW_FLOAT","KW_INT","KW_LONG","KW_NULL","KW_NUMERIC","KW_UPPER","KW_VALUE",
"LENGTH","LOGFILE","LPAREN","LEFT","LEQ","LEVEL","LIKE","LOG_BUF_SIZE","LSS",
"MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE","MINIMUM","MODULE_NAME",
"NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT","NOT_GTR","NOT_LSS",
"NUM_LOG_BUFS","OF","ON","ONLY","OPTION","OR","ORDER","OUTER","OUTPUT_TYPE",
"OVERFLOW","PAGE","PAGES","PAGE_SIZE","PARAMETER","PASSWORD","PLAN","POSITION",
"POST_EVENT","PRECISION","PRIMARY","PRIVILEGES","PROCEDURE","PROTECTED",
"RAW_PARTITIONS","READ","REAL","REFERENCES","RESERVING","RETAIN",
"RETURNING_VALUES","RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK","SEGMENT",
"SELECT","SET","SHADOW","SHARED","SINGULAR","KW_SIZE","SMALLINT","SNAPSHOT",
"SOME","SORT","SQLCODE","STABILITY","STARTING","STATISTICS","SUB_TYPE",
"SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION","TRIGGER","UNCOMMITTED",
"UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR","VARIABLE","VARYING",
"VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH","WORK","WRITE",
"FLOAT_NUMBER","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER","ACTION",
"ADMIN","CASCADE","FREE_IT","RESTRICT","ROLE","COLUMN","TYPE","EXTRACT","YEAR",
"MONTH","DAY","HOUR","MINUTE","SECOND","WEEKDAY","YEARDAY","TIME","TIMESTAMP",
"CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT",
"CURRENT_USER","CURRENT_ROLE","KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR",
"FIRST","SKIP","CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE",
"NULLIF","COALESCE","USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT",
"STATEMENT","LEAVE","INSERTING","UPDATING","DELETING",
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
"statement : user_savepoint",
"statement : undo_savepoint",
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
"privilege : DELETE",
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
"delete_rule : ON DELETE referential_action",
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
"invoke_procedure : EXECUTE PROCEDURE symbol_procedure_name prc_inputs",
"prc_inputs : prm_const_list",
"prc_inputs : '(' prm_const_list ')'",
"prc_inputs :",
"prm_const_list : parameter",
"prm_const_list : constant",
"prm_const_list : null_value",
"prm_const_list : prm_const_list ',' parameter",
"prm_const_list : prm_const_list ',' constant",
"prm_const_list : prm_const_list ',' null_value",
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
"trigger_type_suffix : DELETE",
"trigger_type_suffix : INSERT OR UPDATE",
"trigger_type_suffix : INSERT OR DELETE",
"trigger_type_suffix : UPDATE OR INSERT",
"trigger_type_suffix : UPDATE OR DELETE",
"trigger_type_suffix : DELETE OR INSERT",
"trigger_type_suffix : DELETE OR UPDATE",
"trigger_type_suffix : INSERT OR UPDATE OR DELETE",
"trigger_type_suffix : INSERT OR DELETE OR UPDATE",
"trigger_type_suffix : UPDATE OR INSERT OR DELETE",
"trigger_type_suffix : UPDATE OR DELETE OR INSERT",
"trigger_type_suffix : DELETE OR INSERT OR UPDATE",
"trigger_type_suffix : DELETE OR UPDATE OR INSERT",
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
"keyword_or_column : INSERTING",
"keyword_or_column : UPDATING",
"keyword_or_column : DELETING",
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
"user_savepoint : SAVEPOINT symbol_savepoint_name",
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
"lock_type : SHARED",
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
"delete_searched : DELETE FROM table_name where_clause",
"delete_positioned : DELETE FROM table_name cursor_clause",
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
"search_condition : predicate",
"search_condition : search_condition OR search_condition",
"search_condition : search_condition AND search_condition",
"search_condition : NOT search_condition",
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
"predicate : trigger_action_predicate",
"predicate : '(' search_condition ')'",
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
"in_predicate : value IN in_predicate_value",
"in_predicate : value NOT IN in_predicate_value",
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
	lex.after_select = false;
	lex.limit_clause = false;	
	lex.first_detection = false;
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
	return lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
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
bool was_after_select = after_select;
after_select = false;

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
	/* 23 May 2003. Nickolay Samofatov
	 * Detect FIRST/SKIP as non-reserved keywords
	 * 1. We detect FIRST or SKIP as keywords if they appear just after SELECT and
	 *   immediately before parameter mark ('?'), opening brace ('(') or number
	 * 2. We detect SKIP as a part of FIRST/SKIP clause the same way
	 * 3. We detect FIRST if we are explicitly asked for (such as in NULLS FIRST/LAST clause)
	 * 4. In all other cases we return them as SYMBOL
	 */
    if (sym)
	{
		if ((sym->sym_keyword == FIRST && !first_detection) || sym->sym_keyword == SKIP) {
			if (was_after_select || limit_clause) {
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
			if (sym->sym_keyword == SELECT) 
				after_select = true;
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
case 22:
{ prepare_console_debug ((int) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 27:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 29:
{ yyval = yyvsp[0]; }
break;
case 30:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 31:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 32:
{ yyval = make_list (yyvsp[0]); }
break;
case 34:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 35:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 36:
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 37:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 38:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 39:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 40:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 41:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 42:
{ yyval = 0; }
break;
case 43:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 44:
{ yyval = 0; }
break;
case 45:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 46:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
break;
case 47:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 48:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 49:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 50:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 51:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 52:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
break;
case 53:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 57:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 58:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
break;
case 59:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
break;
case 60:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
break;
case 61:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 63:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 64:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 65:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
break;
case 66:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
break;
case 68:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 69:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 71:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 73:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 74:
{ yyval = yyvsp[0];}
break;
case 75:
{ yyval = yyvsp[0]; }
break;
case 76:
{ yyval = yyvsp[0]; }
break;
case 77:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
break;
case 79:
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 80:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-2]; }
break;
case 81:
{ yyval = (DSQL_NOD) NULL; }
break;
case 83:
{ yyval = yyvsp[-1]; }
break;
case 85:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 86:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				              yyvsp[-1], NULL); }
break;
case 87:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 89:
{ yyval = yyvsp[-1]; }
break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((STR) FUN_reference, CONSTANT_SLONG));}
break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((STR) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((STR) FUN_value, CONSTANT_SLONG));}
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG));}
break;
case 94:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
		  		(DSQL_NOD) NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
break;
case 95:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 96:
{ yyval = yyvsp[0]; }
break;
case 97:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 98:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
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
{ yyval = yyvsp[0]; }
break;
case 115:
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 116:
{ yyval = NULL; }
break;
case 117:
{ yyval = make_list (yyvsp[0]); }
break;
case 119:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 120:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
			     yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 121:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 122:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 123:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 124:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 125:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 126:
{ yyval = (DSQL_NOD) 0;}
break;
case 127:
{ yyval = yyvsp[-1]; }
break;
case 128:
{ yyval = (DSQL_NOD) NULL; }
break;
case 131:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 132:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                          yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 133:
{ yyval = NULL; }
break;
case 134:
{ yyval = NULL; }
break;
case 135:
{ yyval = yyvsp[0]; }
break;
case 136:
{ yyval = (DSQL_NOD) NULL; }
break;
case 137:
{ yyval = (DSQL_NOD) NULL; }
break;
case 140:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 141:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 144:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 145:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 146:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 147:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 148:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 151:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) yyvsp[0]; }
break;
case 152:
{yyval = (DSQL_NOD) NULL;}
break;
case 155:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 156:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 157:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 158:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 159:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 160:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 161:
{yyval = (DSQL_NOD) NULL;}
break;
case 164:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 168:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = yyvsp[0]; }
break;
case 324:
{ yyval = NULL; }
break;
case 325:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 326:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 327:
{ yyval = make_list (yyvsp[0]); }
break;
case 328:
{ yyval = make_list (yyvsp[-1]); }
break;
case 329:
{ yyval = NULL; }
break;
case 330:
{ yyval = make_list (yyvsp[0]); }
break;
case 331:
{ yyval = make_list (yyvsp[-1]); }
break;
case 332:
{ yyval = NULL; }
break;
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 336:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 337:
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 338:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 339:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 340:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 341:
{ yyval = NULL; }
break;
case 343:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 351:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 352:
{ yyval = make_list (yyvsp[0]); }
break;
case 353:
{ yyval = make_list (yyvsp[-1]); }
break;
case 354:
{ yyval = NULL; }
break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 362:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 364:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 365:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 366:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 368:
{ yyval = make_list (yyvsp[0]); }
break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 373:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 374:
{ yyval = yyvsp[-1]; }
break;
case 375:
{ lex.beginning = lex_position(); }
break;
case 376:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 377:
{ lex.beginning = lex.last_token; }
break;
case 378:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 379:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 380:
{ yyval = 0; }
break;
case 381:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 382:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 383:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 385:
{ yyval = NULL; }
break;
case 386:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 387:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 405:
{ yyval = NULL; }
break;
case 406:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 409:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 413:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 414:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 418:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = yyvsp[0]; }
break;
case 422:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 427:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 429:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 430:
{ yyval = yyvsp[0]; }
break;
case 431:
{ yyval = yyvsp[0]; }
break;
case 432:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 433:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 434:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 435:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 462:
{ yyval = NULL; }
break;
case 463:
{ yyval = NULL; }
break;
case 464:
{ yyval = NULL; }
break;
case 465:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 466:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 467:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 468:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 470:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 471:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 472:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 474:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 475:
{ yyval = yyvsp[0]; }
break;
case 476:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 477:
{ yyval = yyvsp[0]; }
break;
case 478:
{ yyval = yyvsp[0]; }
break;
case 480:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 483:
{ yyval = NULL; }
break;
case 485:
{ yyval = NULL; }
break;
case 486:
{ yyval = yyvsp[0]; }
break;
case 487:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 503:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 504:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 506:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 507:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 508:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 514:
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
case 515:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 516:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 517:
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
case 518:
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
case 519:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 526:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 527:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 528:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 529:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 530:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 531:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 538:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 547:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 548:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 549:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 550:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 551:
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
case 552:
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
case 555:
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
case 556:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 557:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 558:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 559:
{ yyval = yyvsp[-1]; }
break;
case 560:
{ yyval = 0; }
break;
case 564:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 565:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 566:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 567:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
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
{ yyval = (DSQL_NOD) NULL; }
break;
case 578:
{ yyval = (DSQL_NOD) NULL; }
break;
case 579:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 581:
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
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
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 663:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 664:
{ yyval = make_list (yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 669:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 670:
{ yyval = yyvsp[-1]; }
break;
case 671:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 672:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 673:
{ yyval = make_list (yyvsp[-1]); }
break;
case 674:
{ yyval = NULL; }
break;
case 676:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 680:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 681:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 682:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 683:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 684:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 685:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 686:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 687:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 688:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 689:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 690:
{ yyval = make_list (yyvsp[0]); }
break;
case 691:
{ yyval = 0; }
break;
case 693:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 698:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 702:
{ yyval = yyvsp[0]; }
break;
case 703:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 709:
{ yyval = 0; }
break;
case 710:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 711:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 712:
{ yyval = 0; }
break;
case 713:
{ yyval = 0; }
break;
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 721:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 722:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 726:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 732:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 733:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 736:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 737:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 739:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 743:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 745:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 746:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 750:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 751:
{ yyval = yyvsp[0]; }
break;
case 753:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 756:
{ yyval = NULL; }
break;
case 757:
{ yyval = make_list (yyvsp[-1]); }
break;
case 759:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 761:
{ yyval = NULL; }
break;
case 762:
{ yyval = make_list (yyvsp[-1]); }
break;
case 764:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 768:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 786:
{ yyval = yyvsp[-1]; }
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
case 835:
{ yyval = make_list (yyvsp[-1]); }
break;
case 836:
{ yyval = yyvsp[-1]; }
break;
case 837:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 838:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 848:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 849:
{ yyval = yyvsp[0]; }
break;
case 850:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 851:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 852:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 853:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 854:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 855:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 856:
{ yyval = yyvsp[-1]; }
break;
case 857:
{ yyval = yyvsp[-1]; }
break;
case 861:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 862:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 863:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 864:
{ yyval = yyvsp[0]; }
break;
case 865:
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
case 866:
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
case 867:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 868:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 870:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 872:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 873:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 879:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 880:
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
case 881:
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
case 882:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 883:
{ yyval = make_parameter (); }
break;
case 884:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 885:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 886:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 887:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = yyvsp[0]; }
break;
case 893:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 895:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 896:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 897:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 898:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 899:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 901:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 902:
{ yyval = yyvsp[0];}
break;
case 907:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 908:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 909:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 910:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 911:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 914:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 915:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 918:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 919:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 920:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 921:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 922:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 925:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 928:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 929:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 932:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 933:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 934:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 936:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 937:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 939:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 943:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 953:
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
