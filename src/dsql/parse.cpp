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
static DSQL_FLD	g_field;
static FIL	g_file;
static DSQL_NOD	g_field_name;
static TEXT	*beginning;
static SSHORT log_defined, cache_defined;
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

static int dsql_debug;

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
static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, STATUS);
static void	check_log_file_attrs (void);

static TEXT	*ptr, *end, *last_token, *line_start;
static TEXT	*last_token_bk, *line_start_bk;
static SSHORT	lines, att_charset;
static SSHORT	lines_bk;
static USHORT   param_number;

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
  259,  259,  259,  256,  256,  258,  258,  257,  255,  255,
  255,  250,  250,  246,  247,  247,  261,  261,  261,  261,
  262,  262,  245,  263,  263,  264,  264,    8,  265,  265,
  265,  265,  265,  265,  265,  265,  265,  265,  265,  265,
  102,  102,  145,  145,  268,  268,  271,  271,  273,  273,
   54,   54,  270,  270,  270,  270,  270,  270,  270,  270,
  270,  278,  278,  269,  269,  269,  269,  279,  279,   62,
   62,   62,   56,   56,  275,  275,  275,  272,  272,  272,
  283,  283,  283,  282,  282,  281,  281,  281,  276,  276,
  286,  284,  284,  284,  285,  285,  277,  277,  277,  277,
  287,  287,   19,   19,   19,  289,  289,  289,   16,   17,
  293,  293,    4,   15,  292,  292,  294,  294,  295,  295,
  288,  296,  296,  297,  297,  298,  298,  298,  298,  299,
  299,  300,  300,  301,  301,  303,  303,  303,  304,  304,
  304,  305,  305,  305,  302,  307,  307,  307,  308,  308,
  306,  306,  309,  311,  311,  310,  310,  290,   18,  312,
  312,  312,  313,  313,  316,  316,  317,   66,   66,   66,
  318,  318,  318,  314,  314,  319,  319,  320,  320,  315,
  324,  321,  321,  321,  321,  325,  325,  325,  326,  326,
  326,  225,  225,  226,  226,  328,  328,  329,  329,  329,
  322,  332,  332,  333,  333,  334,  334,  335,  335,  336,
  336,  210,  210,  338,  338,  234,  234,   32,  235,  235,
  235,  235,  235,  235,  235,  235,  229,  229,  339,  339,
  340,  340,  340,  340,  340,  342,  342,  342,  230,  230,
  228,  228,  323,  323,  346,  347,  347,  347,  347,  347,
  348,  348,  349,  349,  350,  350,  351,  351,  351,  352,
  352,   10,   10,  354,  354,    7,    7,  355,  356,  357,
   20,   20,  358,  359,  360,  360,  189,  330,  330,    3,
    3,  362,  362,  362,  364,  364,   62,  363,  363,  365,
  365,   35,   35,   86,   85,   85,  353,  353,  366,  367,
  367,  211,  211,  211,  147,  361,  361,  112,  112,  112,
  112,  369,  369,  369,  369,  369,  369,  369,  369,  369,
  369,  369,  369,  370,  370,  370,  370,  370,  370,  370,
  370,  375,  375,  375,  375,  375,  375,  375,  375,  375,
  375,  375,  375,  375,  375,  375,  375,  382,  382,  371,
  371,  372,  372,  372,  372,  373,  373,  377,  377,  378,
  378,  378,  378,  376,  379,  374,  374,  380,  380,  380,
  383,  383,  384,  381,  386,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,  157,  157,  157,  387,  385,  385,  152,  152,  391,
  391,  391,  391,  391,  389,  389,  389,  389,  389,  219,
  153,  153,  154,  155,  155,  155,  155,  155,   52,   52,
   21,   21,  242,  392,   55,  124,  274,  274,   96,  388,
  388,  388,  388,  393,  393,  393,  393,  393,  393,  393,
  393,  393,  393,  393,  394,  343,  344,  344,  344,  341,
  341,  390,  345,  345,  396,  396,  397,  397,  398,  398,
  401,  401,  399,  399,  404,  404,  403,  400,  402,  395,
  395,  395,  395,  395,  395,  395,  395,  327,  327,  156,
   49,  280,  123,  149,  368,  166,  212,  267,   64,   61,
  216,  113,   67,  331,   36,   42,  337,  266,   40,   43,
  209,   41,  291,  260,  260,  405,  405,  405,  405,  405,
  405,  405,  405,  405,  405,  405,
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
    1,    1,    1,    1,    2,    2,    2,    1,    1,    1,
    0,    2,    2,    0,    1,    2,    2,    2,    2,    2,
    1,    3,    7,    1,    0,    1,    0,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    3,    2,    2,    2,
    1,    1,    1,    1,    4,    5,    1,    3,    1,    3,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    4,    4,    6,    5,    3,    0,    2,
    2,    0,    3,    0,    4,    1,    5,    4,    1,    4,
    1,    2,    2,    1,    1,    1,    2,    2,    2,    2,
    1,    0,    3,    5,    1,    1,    2,    3,    1,    2,
    3,    0,    1,    1,    1,    5,    5,    6,    2,    5,
    1,    0,    3,    2,    1,    0,    2,    0,    1,    0,
    3,    1,    0,    1,    2,    1,    1,    1,    1,    2,
    2,    1,    2,    3,    1,    1,    3,    3,    1,    2,
    3,    1,    2,    0,    2,    1,    1,    0,    1,    1,
    1,    3,    2,    3,    0,    1,    3,    4,    3,    1,
    3,    4,    3,    0,    1,    3,    3,    1,    1,    0,
    2,    2,    0,    4,    0,    2,    0,    2,    0,    9,
   10,    2,    1,    1,    0,    2,    4,    2,    2,    4,
    2,    1,    1,    1,    1,    1,    3,    1,    2,    3,
    2,    1,    3,    1,    1,    6,    3,    3,    2,    3,
    0,    1,    3,    1,    1,    1,    2,    1,    1,    1,
    2,    1,    2,    1,    2,    0,    3,    0,    1,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    2,    0,
    2,    0,    2,    0,    4,    1,    2,    1,    1,    0,
    1,    3,    2,    1,    1,    2,    1,    4,    2,    1,
    3,    8,    5,    1,    3,    1,    1,    4,    4,    4,
    1,    1,    5,    5,    1,    3,    3,    1,    1,    7,
    7,    5,    3,    0,    1,    1,    1,    2,    0,    1,
    1,    1,    0,    3,    1,    3,    1,    0,    3,    1,
    3,    1,    3,    3,    1,    1,    3,    1,    3,    3,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    1,    1,    5,
    6,    3,    4,    5,    6,    3,    4,    3,    4,    3,
    4,    4,    5,    4,    4,    3,    4,    1,    1,    1,
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
    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  563,
  564,  565,    0,  620,  726,  727,  731,  732,  474,    0,
    0,    0,    0,    0,    0,  407,  575,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  893,    0,   22,  891,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  488,    0,    0,   38,   35,   37,    0,   36,
    0,  974,  976,  978,  977,  983,  979,  986,  984,  980,
  982,  985,  981,    0,    0,    0,    0,   33,   69,   67,
  966,  975,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  644,    0,    0,    0,  676,
    0,  968,    0,    0,    0,    0,  108,  973,  569,    2,
    0,    0,    0,    0,  437,  438,  439,  440,  441,  442,
  443,  444,  445,  446,  447,  448,  449,  450,  451,  452,
  453,  454,  455,  456,  457,  458,  459,  460,  461,  462,
  463,    0,  435,  436,    0,  959,    0,  414,  963,    0,
  411,  965,    0,  678,    0,  410,  969,    0,  573,  889,
    0,  151,  104,    0,  105,    0,  218,  955,  765,    0,
  103,  146,  962,    0,    0,   99,    0,  106,  895,    0,
  100,    0,  101,    0,  102,  972,  147,  107,  628,  629,
    0,  894,  892,    0,   75,    0,  960,    0,  958,  496,
  489,    0,  495,  500,  490,  491,  498,  492,  493,  494,
  499,    0,   31,    0,   40,  752,   39,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  899,    0,  880,  646,  648,    0,  649,  651,
  948,  652,    0,  653,  642,    0,    0,    0,    0,    0,
    0,    0,  592,  581,    0,  584,  586,  587,  588,  589,
  595,  596,    0,  967,  677,    0,  109,    0,  110,    0,
  111,    0,    0,  621,    0,  619,    0,    0,    0,    0,
  475,    0,    0,  377,    0,  221,    0,  417,  408,  472,
  473,    0,    0,    0,    0,    0,    0,  426,    0,  383,
  384,    0,  579,  577,  890,    0,    0,    0,    0,    0,
    0,    0,  154,  133,    0,   97,    0,    0,  112,    0,
  122,  121,    0,    0,    0,    0,    0,    0,  951,   76,
    0,    0,    0,  728,  729,  497,    0,  950,  872,  871,
  870,    0,    0,  873,  874,    0,    0,  876,  356,  357,
  351,    0,  355,  868,  875,    0,  755,    0,    0,   28,
    0,    0,    0,   72,   70,  970,   68,   34,    0,    0,
    0,  757,    0,   53,    0,    0,    0,    0,    0,  571,
    0,    0,    0,    0,  858,  886,    0,    0,  860,    0,
    0,  887,    0,  881,    0,    0,  862,  863,  864,  882,
  883,    0,  884,  885,    0,    0,    0,  888,    0,    0,
    0,    0,    0,    0,  762,  855,  856,  857,  861,  841,
    0,  840,    0,    0,  844,  902,  903,  843,  837,  838,
  839,  842,  900,  901,  923,  924,  927,  928,    0,  655,
    0,  739,    0,    0,  656,    0,    0,  618,    0,  593,
    0,  590,    0,  591,  616,    0,  611,    0,    0,  585,
  766,  735,    0,    0,    0,    0,    0,    0,    0,    0,
  625,  622,    0,  186,    0,    0,  130,  480,  173,  174,
    0,  478,    0,    0,    0,    0,  481,    0,  476,    0,
    0,  421,  423,  422,    0,  424,    0,  418,  219,    0,
    0,  272,    0,    0,    0,    0,  430,  431,    0,  465,
    0,    0,    0,    0,    0,    0,    0,  388,  387,  484,
    0,    0,  150,    0,    0,  159,    0,  158,    0,  165,
  148,    0,  163,  166,  167,  155,    0,    0,  519,  555,
  522,    0,  556,    0,    0,  523,    0,    0,    0,  546,
  559,  518,  541,  520,  521,  516,  503,  377,  501,  502,
  504,    0,    0,  513,  514,  515,  517,    0,    0,    0,
    0,    0,  113,    0,  114,    0,  125,    0,    0,  202,
    0,    0,  375,    0,  221,    0,    0,   84,    0,    0,
  747,    0,    0,    0,    0,    0,  828,  829,  830,    0,
    0,    0,  768,  772,  773,  774,  775,  776,  777,  778,
  779,  780,  781,  782,  877,  878,  879,  869,    0,    0,
    0,  754,   29,    0,    0,   45,   73,    0,    0,   27,
    0,  760,    0,    0,    0,  723,    0,    0,    0,    0,
    0,  570,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  326,  971,    0,    0,    0,    0,    0,
    0,    0,  647,    0,    0,  650,    0,    0,    0,    0,
  964,  659,  567,    0,  897,  566,    0,  594,    0,  602,
  598,  597,    0,    0,    0,  613,  601,    0,    0,  733,
  734,    0,    0,    0,  375,    0,    0,    0,    0,    0,
    0,  131,    0,    0,    0,    0,    0,    0,  377,  420,
    0,  377,  378,  425,  511,    0,  274,    0,  268,  270,
  375,  241,  956,    0,    0,  377,    0,    0,  210,  375,
    0,    0,    0,    0,  246,  240,  243,  244,  245,  429,
  470,  469,  428,  427,    0,    0,    0,    0,    0,    0,
  386,    0,  377,    0,  156,  160,    0,  164,    0,    0,
  542,  543,  560,    0,  557,    0,    0,  549,  547,  548,
    0,    0,    0,    0,  512,    0,    0,    0,    0,  550,
    0,    0,  375,    0,  201,    0,  204,  206,  207,    0,
    0,    0,    0,    0,  221,   79,    0,    0,   78,  530,
  952,  531,    0,    0,    0,  771,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  353,  359,  360,  358,
  756,    0,    0,    0,    0,    0,    0,    0,    0,   54,
   62,   64,    0,    0,   71,    0,    0,    0,  759,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  940,  941,  942,  943,  944,  945,  946,  947,    0,
    0,    0,    0,    0,    0,  933,    0,    0,    0,  674,
    0,    0,  853,  854,  921,    0,    0,  849,  954,    0,
    0,    0,    0,    0,    0,  859,  764,  763,    0,    0,
    0,    0,  664,  665,    0,    0,  657,  660,  568,  898,
  603,  612,  607,  606,    0,  617,  767,  736,  737,  375,
    0,    0,    0,  627,  626,    0,    0,  634,  182,  176,
  183,    0,  179,    0,    0,  178,    0,  189,  170,  169,
  896,  172,  171,  482,    0,    0,    0,  419,  273,    0,
  212,  377,  214,    0,  378,  209,  215,    0,    0,    0,
    0,  432,  433,  377,  375,  378,    0,    0,    0,  404,
    0,  197,  198,  157,    0,    0,    0,    0,    0,    0,
  558,    0,    0,  377,  378,    0,  507,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  199,    0,    0,    0,    0,  364,    0,   83,    0,    0,
    0,    0,   88,   85,    0,    0,    0,  730,  957,    0,
    0,  783,    0,    0,    0,  809,  808,    0,    0,    0,
  816,  831,  826,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  770,    0,   66,   58,   59,   65,   60,
   61,    0,    0,   23,    0,    0,    0,   43,    0,    0,
    0,  741,  761,    0,  724,    0,  740,    0,    0,    0,
    0,    0,    0,  221,    0,  904,    0,    0,  919,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  929,    0,  939,    0,    0,    0,    0,    0,    0,
  920,  865,    0,    0,    0,    0,    0,    0,  679,    0,
    0,    0,    0,    0,  609,  610,  614,    0,  200,    0,
  631,  632,  638,    0,    0,    0,    0,    0,    0,  190,
    0,    0,  415,  224,  225,  226,  227,  228,  229,    0,
    0,    0,  277,    0,  222,    0,    0,    0,    0,  247,
    0,  466,  467,  434,    0,    0,    0,    0,    0,    0,
  377,  486,  378,  168,  953,    0,    0,  525,    0,  524,
  561,    0,  553,    0,    0,    0,  505,    0,  533,    0,
    0,  535,  538,  540,  375,    0,    0,    0,  120,    0,
  205,  377,    0,    0,  376,    0,   98,    0,  118,    0,
   94,    0,    0,    0,    0,   87,    0,  824,  825,    0,
    0,    0,    0,    0,    0,  827,    0,    0,    0,    0,
    0,    0,    0,  817,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   57,
   63,   55,   56,   24,  745,  746,    0,  743,  750,  751,
  748,    0,    0,  722,    0,    0,  910,  909,    0,    0,
  906,  905,    0,  914,  913,  912,  911,  908,  907,    0,
    0,  935,    0,    0,    0,  934,    0,    0,    0,  672,
    0,    0,  667,    0,  668,    0,  685,  681,  683,    0,
    0,    0,    0,    0,  376,    0,  184,  180,    0,    0,
  195,  191,    0,  378,  280,    0,    0,  378,  278,    0,
    0,    0,  232,    0,    0,  248,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  483,  527,    0,  528,    0,
  135,    0,    0,    0,  139,  141,  142,  143,  508,  510,
  506,  537,    0,    0,  378,    0,    0,    0,    0,  361,
    0,  365,  377,   89,    0,    0,   91,    0,    0,    0,
    0,    0,    0,  833,  832,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   41,    0,    0,  725,  220,  922,  915,
  916,    0,  917,  931,  930,    0,  936,  925,    0,  926,
    0,  670,    0,  551,    0,  692,    0,    0,  689,  693,
  694,  696,  697,  698,    0,    0,  640,  378,  362,    0,
  175,  192,  378,  145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  285,  290,  284,    0,    0,    0,  294,  295,  296,  297,
  298,  299,  301,  302,  304,  305,  306,  309,  267,    0,
    0,  208,  233,    0,    0,    0,    0,  234,  237,    0,
    0,  378,  402,  403,  399,  398,  401,  400,    0,  526,
  554,  144,  132,  140,    0,  377,  264,  127,  378,    0,
    0,  379,  366,    0,    0,   92,   93,   80,    0,  795,
  803,    0,  796,  804,  799,  807,    0,    0,  797,  805,
  798,  806,  792,  800,  793,  801,  794,  802,  742,    0,
    0,  932,  673,    0,    0,    0,    0,    0,  706,  708,
    0,  703,    0,  265,  185,  416,    0,  312,    0,    0,
  315,  316,    0,  308,    0,    0,    0,    0,  307,    0,
  338,  339,  293,  300,    0,  303,  286,    0,  291,    0,
  342,  292,  211,  216,  235,    0,    0,  238,    0,  250,
  252,  406,  378,    0,  381,    0,    0,    0,  371,  372,
    0,    0,   77,   95,    0,    0,  918,    0,    0,  695,
  690,  707,    0,    0,  283,    0,  310,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  333,  334,  350,
    0,    0,    0,    0,  345,  343,  239,    0,    0,    0,
    0,    0,  266,  378,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  714,    0,    0,    0,  282,  279,
  311,    0,    0,    0,    0,  314,    0,    0,    0,    0,
  322,    0,  325,    0,  349,  348,  961,  347,    0,    0,
    0,    0,    0,  249,    0,  256,    0,  255,  382,  374,
    0,    0,  367,  119,    0,    0,    0,  716,  705,    0,
    0,  717,    0,  713,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  336,  335,  344,  346,  236,    0,
    0,  260,  259,  258,    0,    0,  641,  835,  712,    0,
  719,  328,    0,    0,  313,  319,    0,    0,    0,    0,
  337,  263,  261,  262,    0,    0,    0,    0,    0,    0,
  340,  317,    0,    0,    0,    0,  718,  331,  318,  320,
  834,  721,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1517,   27,   28, 1518,
   30,   31,   32,   33,   34,   35,   36, 1519,   38, 1520,
  631,  114,  399,  888, 1134,  115,  665,  889,  116,  403,
  670,  140,  117,  118,  255,  960,  130,  890,  891,  195,
  224,  119,  892,  120,  405,   79,  235,  370,  453,  626,
 1071,  388,  848,  597, 1484,  825,  627,  628,  629, 1073,
  236, 1325,   71,  185,   72,  231, 1807, 1277,  216,  221,
  223,  225,  211,  203,  205,  218,  228,  147,  307,  309,
  311,  359,  613,  615,  396,  256,  775,  531,  641, 1008,
  363,  618, 1059, 1269,  564,  725, 1034,  516,  517,  546,
  355,  598, 1045, 1413, 1542, 1213, 1414, 1415, 1416, 1417,
 1418,  642,  212,  204,  351,  571,  352,  353,  572,  573,
  574,  575, 1244, 1002,  519,  520,  992,  990,  521,  996,
  991, 1204, 1377,  997,  998, 1209,  365,  836,  837,  838,
  839,  777, 1015, 1391,  599,  779,  455,  841,  948,  540,
 1340, 1214,  456,  457,  458,  940,  459, 1392, 1393,  549,
 1548, 1700, 1230,  785,  786,  772,  787,  788,  789, 1701,
 1702, 1783,  333,  544, 1221, 1521,  191,  541,  542, 1222,
 1223, 1386, 1676, 1522, 1523, 1524, 1525, 1640, 1526, 1527,
 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536, 1537,
 1538, 1723, 1768, 1623, 1687, 1772, 1685,  460,  704, 1359,
  461, 1078, 1641, 1694, 1695, 1736,  391,  392,  462, 1064,
 1275, 1430, 1065, 1066,  283,  483, 1571,  374, 1194, 1373,
 1657, 1658, 1659, 1660, 1192,  342,  560,  803, 1242,  561,
  801,  219,   56,  337,  196,  154,  320,  182,  327,  188,
  763,  760,  328,  338,  793,  339,  556, 1026,  183,  463,
  321,  528,  562, 1243,   93,  194,  240,  600,  601,  765,
 1046,  766, 1047, 1048,  604,  605,  606,  607, 1039,  852,
  608,  609,  610,  818,  611, 1486,  815,   40,   41,   42,
  149,   58,  421,  199,  344,  294,  295,  296,  297,  298,
  299,  300,  301,  302,  731,  496,  975, 1197,  497,  498,
  736,   43,  153,  316,   44,  510,  511,  984,  749,  988,
  134,  718, 1497,  676,  135,  136,  284,  484,  485,  486,
  722,  961,  962,  963,  964, 1186,  464, 1174, 1488, 1489,
  465, 1491,  466,  467,  468, 1715, 1613, 1716, 1717, 1718,
 1764, 1808,  411, 1146,   45,   46,  375,   47,   48,  504,
  505,  897, 1142, 1327, 1331,  412,  673,  209,  643,  644,
  645,  646,  647,  648,  649,  650,  651,  652,  653,  654,
 1294, 1089, 1091, 1092,  947,  703,  469,  470,  471,  472,
  395,  233,  473,  474,  930,  475,  476,  477,  478,  695,
  934, 1175, 1170,  696,  122,
};
static short yysindex[] = {                                   4172,
 1385, -110, 2754,  142, 1395,   54, 3619,   12, 5616,   94,
  473, 3780, -110, 1268,  611, 4012, 1216, 4012,    0,  719,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -56,    0,    0,    0,    0,    0,    0,11076,
 4012, 4012, 4012, 4012, 4012,    0,    0,  385, 1323, 4012,
 4012, 4012,  639, 4012,  455, 4012, 4012,    0, 4012, 4012,
    0,  957,    0,  539,    0,    0,  684, 4012,    0, 4012,
 4012, 4012,  728, 4012, 4012, 4012, 4012,  455, 4012, 4012,
 4012, 4012,    0, 4012,  653,    0,    0,    0, 1042,    0,
 1042,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  709,  776,   88, 1060,    0,    0,    0,
    0,    0, 4012, 4012, 4012,  802,  845,  875,  273, 2252,
  892,  160,  161,  935,  850,    0, 4012, 1038, 1121,    0,
 1012,    0, 4012, 4012, 4012, 4012,    0,    0,    0,    0,
 1232,   -1, 1254,  498,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  761,    0,    0, 1323,    0,   34,    0,    0, 1579,
    0,    0, 1480,    0,   77,    0,    0, 1180,    0,    0,
 1185,    0,    0, 1347,    0, 1403,    0,    0,    0, 1323,
    0,    0,    0,  -52, 1579,    0,    1,    0,    0, 1375,
    0, 1363,    0, 1042,    0,    0,    0,    0,    0,    0,
 1369,    0,    0, 1256,    0, 1398,    0, 1287,    0,    0,
    0, 1256,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2634,    0, 4012,    0,    0,    0, 4137, 1355, 4045,
 4012,  765, 1426, 1730, 1479, 1511, 4137, 1408, 4045, 1450,
 1456, 1344,    0, 9747,    0,    0,    0, 9747,    0,    0,
    0,    0, 7536,    0,    0, 1410, 4012, 1491, 1405,  561,
 4012, 1424,    0,    0, 1121,    0,    0,    0,    0,    0,
    0,    0, 4012,    0,    0, 1579,    0, 1375,    0, 1042,
    0, 9747, 1442,    0, 1418,    0,  585, 1505,  915,  498,
    0, 1576,  530,    0, 4012,    0,  761,    0,    0,    0,
    0, 4012, 1454, 3478, 1402, 3646, 1828,    0,11076,    0,
    0,  986,    0,    0,    0, 1814, 1814, 1323, 1509, 1323,
 1985, 1347,    0,    0, 7336,    0, 4012, 4012,    0, 1454,
    0,    0, 1594,  537, 1854, 4012, 1632, 4012,    0,    0,
 1857,  109,  909,    0,    0,    0, 1438,    0,    0,    0,
    0, 1440, 1444,    0,    0, 1357, 2994,    0,    0,    0,
    0, 1863,    0,    0,    0, 1332,    0, 4012, 1462,    0,
 4012, 4012,  127,    0,    0,    0,    0,    0, 4012, 4012,
  701,    0, 4012,    0, 1595, 4012, 1866, 4137, 1512,    0,
 4012, 1877, 1881, 1882,    0,    0, 1883, 1887,    0, 1899,
 1904,    0, 1905,    0,    0, 1908,    0,    0,    0,    0,
    0, 1909,    0,    0, 8209, 1911, 1913,    0, 9747, 9747,
 8451, 4012, 1915, 2025,    0,    0,    0,    0,    0,    0,
 1870,    0,    0, 1918,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2571,    0,
 2357,    0, 1647, 1926,    0, 2738,   65,    0,  529,    0,
  345,    0,  345,    0,    0, 1927,    0,  229, 1529,    0,
    0,    0, 1928,  149, 1914, 1454, 1937, 1716, 2082, 1938,
    0,    0, 1607,    0,  221, 1624,    0,    0,    0,    0,
 1323,    0, 1814, 1814, 1814, 1814,    0, 1946,    0, 1718,
 1720,    0,    0,    0, 1697,    0, 7336,    0,    0, 7336,
 1437,    0, 1579, 1742, 4012,10968,    0,    0,  424,    0,
 4012, 1222, 1480, 1615, 1571, 1539,    0,    0,    0,    0,
   20, 1618,    0, 1566,  455,    0, 1323,    0, 1744,    0,
    0, 1985,    0,    0,    0,    0,  118, 1580,    0,    0,
    0, 1582,    0, 1640, 1986,    0, 1682, 2002,   45,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1952,  220,    0,    0,    0,    0,  155, 2004, 2007,
 2002, 1579,    0, 1734,    0, 1796,    0, 1323, 1323,    0,
 3478,   77,    0, 1670,    0, 1651, 2027,    0, 6342,  781,
    0, 1684, 1699, 2044, 5144, 2045,    0,    0,    0, 4914,
 7604,  -22,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1455, 2994,
 4012,    0,    0, 4556, 1652,    0,    0, 1630, 4045,    0,
 1779,    0, 1610, 1268, 2059,    0, 1779, 4556, 1780, 1782,
 4012,    0, 1030, 8533,  729, 4012, 9747, 1051, 1296, 1352,
 2557, 9747, 5144, 2357, 1654,  583, 9747, 8533,  822,  822,
 1268, 2693, 2062,    0,    0, 8775, 4012, 9747, 9747, 9747,
 9747, 9747,    0, 9747,  403,    0,  943, 1658, 8533, 4012,
    0,    0,    0, 1000,    0,    0,   25,    0, 1664,    0,
    0,    0, 4012,  681, 4012,    0,    0, 4012, 4012,    0,
    0, 8533, 1849, 3478,    0, 1629, 9747, 4012, 1660, 1323,
 1323,    0,  -67, 1566, 1566, 1674,  455,  915,    0,    0,
 2095,    0,    0,    0,    0, 1860,    0, 4012,    0,    0,
    0,    0,    0, 1865, 2107,    0, 1848, 1867,    0,    0,
 1808, 1812, 1042, 1880,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  455, 4012,11032, 1773, 1786, 1787,
    0,  455,    0, 1436,    0,    0, 1749,    0,  112, 1760,
    0,    0,    0,  455,    0, 1986,  145,    0,    0,    0,
 1879,  145, 1762,  145,    0, 2125,  455,  455,  455,    0,
 1454, 4012,    0, 1821,    0, 1612,    0,    0,    0,  986,
 1765, 4012, 1666,  117,    0,    0, 2146, 1924,    0,    0,
    0,    0,  109, 4012, 1772,    0, 1772, 4903,  101, 9747,
 9747, 5868, 2152, 1288, 6122, 9747, 6376,  801, 6460, 6714,
 8857, 6968, 7052, 7306, 5144, 5144,    0,    0,    0,    0,
    0, 4012, 4012, 4012, 4012, 4012, 4012,  137, 2156,    0,
    0,    0, 4556, 1824,    0,  443, 1850, 4012,    0,  935,
 8533, 1850, 2175, 2156, 4556, 3921, 1907, 9747, 9747, 1960,
 9747, 2184, 9747, 2185, 2705, 9747, 9747, 9747, 9747, 9747,
 9747,    0,    0,    0,    0,    0,    0,    0,    0, 1912,
 1789,  500, 9747,  679, 8533,    0, 5144, 2541, 2357,    0,
 2191,  935,    0,    0,    0, 2357, 1680,    0,    0, 1956,
  822,  822, 1179, 1179, 1073,    0,    0,    0,  943, 2199,
 2198, 1043,    0,    0, 5144, 1916,    0,    0,    0,    0,
    0,    0,    0,    0,  571,    0,    0,    0,    0,    0,
 1712, 1765,  972,    0,    0, 2201, 1748,    0,    0,    0,
    0, 1727,    0, 1814, 1983,    0,  -67,    0,    0,    0,
    0,    0,    0,    0, 2216, 5144, 2139,    0,    0, 1959,
    0,    0,    0, 2139,    0,    0,    0, 1042, 1042, 1767,
 2221,    0,    0,    0,    0,    0,  478,  -13,   79,    0,
 2006,    0,    0,    0, 4012,  142, 1731, 1851, 1860, 2230,
    0, 1566, 1735,    0,    0, 1416,    0, 2217, 4012, 1497,
  455, 2236, 2239, 2240, 2019,   77, 1959, 1814, 1624, 3478,
    0, 1618,  935, 1830, 1852,    0,  848,    0,  455, 1894,
 1988, 6342,    0,    0,  455, 1802, 1991,    0,    0, 2254,
 2261,    0, 2266, 2357, 2263,    0,    0, 2357, 2264, 9099,
    0,    0,    0, 1954, 2272, 2357, 2275, 1936, 2281, 2357,
 2282, 9747, 9747, 2152, 9747, 9181, 2284, 2357, 2285, 2286,
 2357, 2287, 9747, 2357, 2289, 2357, 2290, 2291, 2357, 2299,
 2303, 2357, 2306,    0, 2087,    0,    0,    0,    0,    0,
    0, 2026, 4556,    0, 4556,  137, 2156,    0,  297,  297,
  134,    0,    0, 7536,    0, 1739,    0, 2175, 2156, 2312,
 3921, 2820, 2864,    0, 2898,    0, 2951, 9747,    0, 2999,
 3081, 3221, 3311, 3389, 3556, 9747,  455, 8533, 2357, 1923,
 8533,    0, 9747,    0, 2052,  589, 9747, 8533, 9747, 9747,
    0,    0, 1043, 2322, 8533, 4012,  943, 1977,    0, 1981,
 1987, 2037, 2115, 2061,    0,    0,    0, 1959,    0, 1830,
    0,    0,    0, 1972, 1323, 2005, 1566, 2012, 1566,    0,
 5144,  413,    0,    0,    0,    0,    0,    0,    0, 1950,
 2126, 1959,    0, 9747,    0, 2118, 1999, 1767,  957,    0,
 5144,    0,    0,    0, 2022, 2028, 2029, 2030, 2033, 2034,
    0,    0,    0,    0,    0, 2370,  142,    0, 1674,    0,
    0,  145,    0, 2139, 2035,  145,    0,  145,    0, 1860,
 2380,    0,    0,    0,    0,  986, 2126, 1566,    0, 1624,
    0,    0, 7536, 2144,    0,   46,    0, 2201,    0, 2390,
    0, 2391, 1323,   -8, 2392,    0, 1323,    0,    0, 9747,
 2014, 2014, 1268, 2394, 1752,    0, 2014, 2014, 9747, 2014,
 2014, 2606, 2357,    0, 2212, 9747, 2357, 2014, 2014, 2014,
 2014, 2357, 2014, 2014, 2014, 2014, 2014, 2014, 2051,    0,
    0,    0,    0,    0,    0,    0, 2001,    0,    0,    0,
    0, 1647, 8533,    0, 3921, 2312,    0,    0, 7336, 2397,
    0,    0, 3563,    0,    0,    0,    0,    0,    0, 3572,
  114,    0, 8533, 2131, 2010,    0, 8533, 3689, 1759,    0,
 1861, 2357,    0, 1776,    0, 1043,    0,    0,    0,  943,
 4211, 5144, 2050, 2126,    0, 1814,    0,    0, 1323, 1436,
    0,    0,  618,    0,    0, 4012,10791,    0,    0, 2357,
 2160, 2118,    0,  836, 4012,    0, 2112,  682, 2011, 2114,
 2032, 2153, 2129, 2169, 1959,    0,    0, 2415,    0, 2430,
    0, 2128, 2160, 2035,    0,    0,    0,    0,    0,    0,
    0,    0, 1959, 1618,    0, 1436, 2006, 2158, 2096,    0,
 1765,    0,    0,    0, 2116,  427,    0, 1860, 2132, 2357,
 2448, 2458,  935,    0,    0, 2459, 2461, 2357, 2463, 2464,
 9747, 9747, 2357, 2465, 2468, 2474, 2479, 2481, 2484, 2485,
 2486, 2489, 2490,    0,  297, 1658,    0,    0,    0,    0,
    0,  455,    0,    0,    0, 8533,    0,    0, 8533,    0,
 1658,    0, 1134,    0, 2255,    0, 2488, 2495,    0,    0,
    0,    0,    0,    0,  -22,  468,    0,    0,    0, 1566,
    0,    0,    0,    0, 7336, 1170,  712, 2473,  512, 2500,
 2204, 9747, 2496, 2505, 2497, 2498, 2508, 2510, 2208, 2511,
    0,    0,    0, 2246,10735, 2513,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2533,
 4012,    0,    0, 2229, 2243, 4012, 1767,    0,    0, 1042,
 4012,    0,    0,    0,    0,    0,    0,    0, 2126,    0,
    0,    0,    0,    0, 2126,    0,    0,    0,    0, 1556,
 1658,    0,    0, 9747, 1323,    0,    0,    0, 1323,    0,
    0, 9747,    0,    0,    0,    0, 2357, 2357,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1916,
 2551,    0,    0, 1916, 5144, 4012, 1590, 4211,    0,    0,
 2225,    0, 2562,    0,    0,    0,  153,    0, 9423, 4012,
    0,    0, 9747,    0,  -84, 2265, 5144, 2743,    0, 5144,
    0,    0,    0,    0,  814,    0,    0,  793,    0, 2164,
    0,    0,    0,    0,    0, 1767, 1042,    0, 2220,    0,
    0,    0,    0, 2006,    0, 1556, 2560, 1043,    0,    0,
 1916, 2357,    0,    0, 1861, 2061,    0, 2061,  -22,    0,
    0,    0, 3956, 2139,    0, 2547,    0, 1493, 9505, 1210,
 9747,  814,  699, 9747, 2552,  711,  726,    0,    0,    0,
 4012, 4012,  142,  115,    0,    0,    0, 2220,  102, 1767,
 2234, 2245,    0,    0, 1043, 2588, 1556, 2288, 2061, 2589,
 1658, 2050, 2050, 4012,    0, 2597, 2595,  794,    0,    0,
    0, 7634, 2231, 2608,  814,    0, 1340,  195, 2222, 2357,
    0, 2351,    0,  814,    0,    0,    0,    0,10791,  793,
 1767,  592,  592,    0, 2355,    0, 2219,    0,    0,    0,
 1043, 1556,    0,    0, 1916, 2271, 2271,    0,    0, 3956,
 2624,    0, 4012,    0, 2693, 1781, 1013, 2607, 1096,  814,
 2372, 2364,10791,10791,    0,    0,    0,    0,    0, 2214,
  908,    0,    0,    0, 1280, 2061,    0,    0,    0, 4012,
    0,    0,  814, 2627,    0,    0,  152, 4012,10791, 2371,
    0,    0,    0,    0, 5144, 2050, 2636, 2640, 1804,10791,
    0,    0,10791,  -22, 2271, 4012,    0,    0,    0,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  198,  691,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  123, 5512,    0,    0,    0,    0,    0, 2682,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  365,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  866,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2346,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2302,    0,    0,    0,  280,    0,
  280,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2304,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  964,    0,    0, 7873, 5784,    0,    0,    0,  966,    0,
    0,    0, 2155,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  294,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  372,
    0,    0,    0,    0,  673,    0,    0, 1133,    0,    0,
    0,    0,    0, 3110,    0, 7806,    0,    0,    0,    0,
    0,    0,    0,    0,  372,    0,  -66,    0,    0, 2646,
    0,    0,    0, 2424,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1191,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1299,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1200,    0,    0,    0,    0,    0,    0,
    0, 4412,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  433,    0,    0, 1301,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  372,    0, 2646,    0, 2424,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1372,
    0, 2414,    0,    0,    0,    0, 1379,    0,    0,    0,
    0,    0, 2431,  570,11128,    0, 1381,    0,    0,    0,
    0,  208,    0,    0,    0, 2241, 2241,    0,    0,    0,
 1388, 3166,    0,    0,    0,    0,    0,    0,    0, 2431,
    0,    0, 1376,    0,    0,    0,    0,    0,    0,    0,
 4741, 2315,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1415,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1428,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1533,    0,    0,    0,    0,    0,
    0, 2382,    0,    0,    0, 2517, 2873,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 3008,    0, 1958,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  710,    0,    0, 2385,    0,  374,    0,    0,    0,    0,
  246,    0,  246,    0,    0,  736,    0, 1099, 1183,    0,
    0,    0,    0, 1191,    0, 2431,    0,    0,  120, 1298,
    0,    0,  121,    0, 3226,  167,    0,    0,    0,    0,
    0,    0, 2241, 2241, 2241, 2241,    0,  172,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2443,    0,    0,    0,    0,    0, 2414,    0,
    0, 1189,    0, 2238,    0,    0,  645,    0,    0,    0,
    0,  241,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1452,    0,    0,    0,    0, 1524, 5260,    0,    0,
    0, 5405,    0,    0, 2077,    0,    0, 5432,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 3338, 3525,    0,    0,    0,    0, 5444, 4735,    0,
 5432,  372,    0,    0,    0,    0,    0,    0,    0,    0,
  570, 1064,    0,    0,    0,    0, 2298,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1747,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  194,    0,    0, 5512,    0,    0,  194,    0,    0,    0,
    0,    0, 9829,    0, 9829,    0,    0, 9829, 9829, 9829,
    0,    0,    0, 2260,    0,    0,    0,    0, 3990, 4346,
 8127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2154,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  732,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  570,    0, 1266,    0,    0,  387,    0,
    0,    0, 3419,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1018,    0,    0,    0,    0,
    0,    0,    0, 2672,    0,    0, 1676, 1569,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  238,  256,  340,
    0,    0,    0, 2451,    0,    0,    0,    0,    0, 2101,
    0,    0,    0,    0,    0, 2077,    0,    0,    0,    0,
  828,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2431,    0,    0,  332,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6934,    0,    0,    0,  184,    0,    0,
    0,    0, 2405,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1471, 1492,    0,
    0,    0,    0,    0,    0,    0, 1494,    0,    0, 7873,
    0, 1494, 1500, 1508,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  231,    0,
    0, 9829,    0,    0,    0, 1123,    0,    0,    0, 3364,
 4481, 4837, 3499, 3855,    0,    0,    0,    0,    0,  620,
 1900, 2193,    0,    0,    0, 1932,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  170,    0,    0,    0,    0,
    0,    0,    0, 2241, 2258,    0, 3433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2447,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1261,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1522,    0,    0,    0,    0,    0,    0,    0, 1708,    0,
    0,    0,    0,    0,    0,    0,    0, 1559,    0,    0,
    0,    0,    0,    0,    0, 1064, 2447, 2241, 1527,  570,
    0, 2456, 7873, 1528,  126,    0,    0,    0,    0, 6934,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3754,    0,    0,    0, 4245,    0,    0,
    0,    0,    0,    0,    0, 5865,    0, 6457,    0, 7049,
    0,    0,    0,    0,    0,    0,    0, 7521,    0,    0,
 8401,    0,    0, 9697,    0,10017,    0,    0,10089,    0,
    0,10100,    0,    0, 5707,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1471, 1562,    0, 2293,  243,
    0,    0,    0,    0,    0,    0,    0, 1563, 1570, 1602,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2292,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2378, 1118,    0, 2759,    0, 2381,    0, 2383,
 2388,    0,    0,  474,    0,    0,    0, 2447,    0, 1528,
    0,    0,    0, 3714,    0,    0,    0, 2274,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 4629,
    0, 2462,    0,    0,    0, 1733,    0, 1261, 2346,    0,
    0,    0,    0,    0,  347,  382,  410,  470,  598,  738,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  388,    0,    0,    0,    0, 3276,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1627,
    0,    0,    0,    0,    0,    0,    0, 1635,    0,    0,
    0,    0,    0,  162,    0,    0,    0,    0,    0,    0,
    0,    0, 8127,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10123,    0,10261,    0,10282,    0,    0,    0,
    0,10409,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1637,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1719,    0,    0,    0, 2332,    0,    0,    0,    0,
    0,    0,  853,    0,    0, 2241,    0,    0,    0, 2942,
    0,    0,    0,    0,    0,    0, 2429,    0,    0, 2702,
 1564, 1826,    0, 2414,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2447,    0,    0,    0,    0,    0,
    0,    0, 1643,  871,    0,    0,    0,    0,    0,    0,
    0,    0, 2447, 2456,    0,  354,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  251,    0,10473,
    0,    0, 9829,    0,    0,    0,    0,10505,    0,    0,
    0,    0,10547,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  243, 1095,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  649,    0, 2378,    0,  175,    0,    0,  528,    0,    0,
    0,    0,    0,    0,  177, 2709,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1119, 1313,    0,    0,    0,    0,
    0,    0,    0,    0, 2437,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1750,    0,    0,  125,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  173,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,10611,10643,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  381,
    0,    0,    0,  659,    0,    0,    0,    0,    0,    0,
 2713,    0,    0,    0,    0,    0, 2695,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2696,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2449,
    0,    0,    0,    0,    0, 1750,  398,    0,  124,    0,
    0,    0,    0,    0,    0,    0,  188,  721,    0,    0,
  204, 2702,    0,    0,    0,  898,    0,  260, 3250,    0,
    0,    0, 2709,    0,    0,    0,    0,    0,  103,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  148,    0, 1261,
  408,  796,    0,    0, 2378, 1223,    0,    0,  176,    0,
  649,  310,  209,  847,    0,    0, 2715,    0,    0,    0,
    0,    0, 2704,  168,    0,    0,    0, 2460,    0, 2707,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1750,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1094,    0,    0,    0,  659, 1365, 2726,    0,    0, 2709,
    0,    0,    0,    0, 2727,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2378,  260,    0,    0,    0,    0,
    0,    0,    0, 2711,    0,    0,    0,    0,    0,10702,
    0,    0,    0,    0,    0,  209, 2732,    0,    0,    0,
    0,    0,    0, 2632, 2726,    0,    0,    0,    0,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2774,    0,    0, 2775,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2776,
    2,  291,   32, -586, 1641,  301,   39,   53, 2771, 2522,
    0,   85,    0, 2531,  -93,  -31,    0,  597, -859,   17,
  -59,  -33, -181, 2535, 2135,    0,    0,    0,  608,    0,
    0,  -36, 1722, -520,  -38, -887, 2173, 1961, -298, 1740,
 2717, -281,    0,  -57,    0, -469,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -588,  -96, 1745, -294, 1017, -904,
    0,    0,    0,    0, -313, -116, -351, 1754, -330,  -45,
    0, -508,    0,    0, 1396, -967,    0, 1401,    0,    0,
 2494, -540,  -19,    0,    0,    0,    0, 2471,    0, 2248,
 2512, -200, 1778, -704,    0,    0,    0, -687,    0,    0,
    0,    0,    0,    0, 1833,    0, 2523, 2099, 1793, 2506,
 2515,    0, 1230,    0, -511, 2076,  -55, -642,-1212,    0,
    0, -107, -921, -913, -910, -185, -908,    0, 1447,-1020,
    0, 1157,-1132, 1465,    0, 2313,    0,    0,    0, 1164,
 1169, 1132,  190, -252, -414, -409,    0,    0, 2110,    0,
 1677,    0,    0, -892,    0,    0,    0,    0, -176,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1255, -458,    0,    0,  481,    0,-1136,
  782, 1090, 1252,    0, 1158,    0,    0, 2514, -119, 1920,
 1710, 1537,    0, -899, -798, -846,    0, -485, -565, -441,
    0, -609, 1265,  321,-1349, -482, -710, -912,-1165,    0,
    0,    7,    0,    0,    0,    0,    0, 2569,    0,    0,
    0,    0, 2596, 2369,    0,    0,    0,    0,    0,   -9,
 2604,    0,    0,    0,    0,  -15,    0,    0,    0, -331,
 2102, -324, 1669, -439,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2317,    0,    0, 2120,    0,    0,    0,
 2516, 2918,    0,    0,    0,    0,    0, 2643,    0,    0,
    0,    0, 2455,    0, 2454,    0,    0,    0, 2215,    0,
    0,    0, -394,    0,  131,    0, 2202,    0,    0,    0,
 -584,-1068, -878,  891,    0, 2815, 1481,    0, 2232, -630,
 2235,    0, -816, 1995,    0,    0, -125, -678,    0, 1348,
-1127,    0, -646, -624, -623, 1463,    0, 1198,    0, 1247,
    0, 1146,    0,    0,    0,    0, 2466,    0,    0,    0,
 -321, 2295, 2063, -935,    0,    0,    0, -676,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 4281, 1338, 1872,    0, -568,    0,    0,    0, -240,    0,
 2593,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -365, 1801,    0,    0,
};
#define YYTABLESIZE 11639
static short yytable[] = {                                     121,
  143,  187,  121,  210,  207,   75,  142,  257,  148,   37,
   76,  394,  277,  280,  206,  276,  279,  305,  740,  941,
  570,  190,  202,  602,  241,  764,  217,  537,  767,  535,
  603,  250,  215,  565,  778,  245,  227,  776,  958,  746,
  184,  186,  189,  192,  142,  197, 1225,  726, 1364,  247,
  208,  186,  213,  910,  192,  246,  142,  197,  251,  226,
  121,  977,  252,  993,  143,  244,  390,  263,  237,  265,
  142,  239,  186,  248,  237,  213,  189,  192,  404,  142,
  197,  226,  121,  222,  192, 1215,  310,  404,  672,  900,
  632,  903, 1215, 1216,  856, 1396, 1217,  482, 1219,  859,
 1216, 1144,  982, 1217, 1037, 1219,  249,  616,  849,  724,
 1226,  979,  306,  208,  142,  208,  942,  286,  527,  630,
  637, 1234,  576,  257,  753,  363,  502,  213, 1010, 1062,
  367,  261,  393,  304,  192,  142,  226, 1017,  193,  840,
 1255, 1082, 1183, 1179,  389,  955,  394,  257,  329, 1272,
  220, 1250,  932,   74, 1473, 1036, 1070,  809, 1740,  986,
  630,  329,   74,  630,  257,  753,  477,  257,  753,  636,
  669,  479,  702,  356,  691,  700,  699,  503,  630,  637,
 1133,  576,  257,  753,  363,  752,   74,  368,  257, 1042,
 1057,  257,  739,  744,  827, 1734,  275,  576,  397,  274,
  278,  390,   90,  688, 1328, 1394,  257,  485,  264,  754,
  755,  756,  757, 1674,  602,  691,  508,  699,  691,  123,
  667,  603,  275,  275,   86,  477,  327,   86,  636,  308,
  479,  702,  966,  691,  700,  699,  488,  391, 1734,  875,
  405,  570,  532, 1490,  208,  604,  368,  501,  142,  704,
  406,  121,  744,  743,  784,  389,  576,  142,  313,  406,
  751, 1569,  688, 1466, 1273, 1436,  485,  393,  361,  536,
 1145,  675,  735, 1321,  675, 1323,  539,  189,  207,  389,
  552,  142,  314,  554,  503, 1024, 1411, 1237,  994,  675,
  330,  534, 1481,  635,  534,  810,  391, 1332,  415,  405,
  700,  532,  127,  821,  604, 1431, 1136,  491, 1708,  704,
  824,  566,  128,  568,  389,  208,  261,  624, 1148, 1150,
  798,  819,  208,  753,  208,  612,  208,  620, 1644,  557,
  151,  126, 1215,  340, 1124, 1125,  141, 1198, 1406,  390,
 1216,   74,  400, 1217,   57, 1219,  396,  192,  197,  357,
  704,  400,  635,  196,  501, 1708,  142,  799,  189,  275,
 1621,  995,  875,  876,  624,  362,  123,  331,  704,  666,
 1366, 1394, 1421,   80,  614,  495, 1432, 1043,   76, 1239,
  688,  397, 1233,  152,  666,  358,  639,  137,  142,  820,
  126,  192,  406, 1670,  123,  123, 1176,  753,  390,  142,
  238, 1708, 1742,  142,  360,  396,  192,  253,  142,  393,
  341,  148,  196,   94, 1648,   14, 1240,  658, 1739,  394,
  270,  688, 1622,  624,  477,  257, 1428,  257, 1238,  479,
  271, 1472,  599,  124,  257, 1708, 1329,  630,  753,  688,
  397,  753,  705,  512,  957,  639,  137,  804,  253,  680,
  622,  253, 1014, 1384,  679, 1810,  753, 1437, 1771,  630,
  637,  800, 1063,  493, 1005, 1212,  253, 1007,  393,  392,
   90,  485,  477,  700,  879, 1321,  721,  479, 1278, 1504,
 1490, 1013,  663, 1539,  753, 1540,  876,  404, 1704,  750,
  780,  599,  691,  671,  699,  506,  823,  677,  482, 1360,
  702,  391,  400,  702,  405,  691, 1360,  753, 1031,  636,
 1069, 1566,  329, 1697,  691,  368,  699, 1378,  368,  389,
 1567, 1295,  273,  260,  534,  257,  805,  687,  392, 1599,
  806, 1573,  700,  482,  688,  773,  208,  675,  675,  630,
  880,  773, 1724, 1743, 1409, 1072,  734,  257,  630,  723,
 1578,  849,  878, 1483,  257, 1424,  482, 1004,  576,  534,
  744,  691,  978,  699, 1336,  207,   73, 1744,  687, 1001,
  691, 1077,  699, 1266,  637,   73, 1143,  327, 1055,  363,
  668,  834,  835,   76,  477, 1766,  687,  604,  257,  479,
 1132, 1001,  269, 1614,   86,  704, 1711,  395, 1616,   73,
  373,  826,  273,  390,  485,  881,  576,  970, 1779,  532,
  396,  208,  702,  503,  691,  700,  699,  273,  273,  671,
  851,  630, 1423,  636,  257,  753,  702,  368,  691,  700,
  699,  850, 1639,  635,  391,  269,   76,  999, 1000,  368,
  675,  368, 1267,  688, 1582,  397,  700, 1651,  257,  666,
  604,  208,  389,  604,  406,  700,  395,  688, 1503,  406,
  671,  534,  753,  671, 1655,  377,  914,  137,  406,  604,
 1383,  192,  385,  393,  875,  753,  213,  753,  671,  126,
 1207,  675,  624,  501,  753,  253, 1020,  253,  207,  702,
 1398, 1501,  397,  658,  253,  604,  704,  949,  956,  688,
 1126,  196, 1467, 1129,  624,  208, 1719,  192, 1443,  738,
  721,  688,  539,  989,  989,  482,  534, 1224, 1003,  907,
  369,  729, 1552,  142, 1492,  142,  639,  630,  208, 1232,
  904,  385,  770,  392,  208,  605,  390,  394,  208, 1729,
 1023, 1025,  781,  396, 1268,  125, 1493, 1494, 1703, 1254,
  738, 1732, 1215,  738,   73,  317, 1022, 1710,  208, 1397,
 1216,  875, 1139, 1217,  369, 1219,  394,  688,  738, 1734,
  912, 1072,  503,  394,  599,  753,  688,  150,  397,  369,
  753, 1576,  269, 1374, 1733,  253,  208,  208, 1052, 1053,
 1054,  700,  730,  198,  605,  254,  394,  753,  876, 1749,
 1603,  831, 1352,  318,  753, 1354,  393,  253, 1030,  599,
 1609, 1388, 1410,  700,  253, 1235,  533,  495, 1420,  976,
 1040, 1218,  142,  782, 1625,   74, 1130,  136, 1218,  534,
 1468, 1495,  142, 1756, 1757, 1610,  254,  599,  753,  254,
  599,  371,  501,  491, 1079,  687, 1777,  242,  253,  376,
  875, 1127,  704, 1131,  254, 1339,  599, 1425,  687,   76,
  700,  395,  102,  711,  783,  578,  392,  687,  712,  700,
  138,  452,  406,  192,  197,  406,  226,  121, 1140,  875,
 1800, 1801,  599,  406,  619,  876,  136,  254,  242,  935,
  936,  103,  104, 1611,  105,  406,  406,  700,  753,  214,
 1128,  106,  107,  108,  109,  110, 1812,  111,  253,  112,
  113,  704,   73,  700,  687,  319, 1056, 1819, 1577, 1236,
 1820, 1227, 1228,  687,  578,  970, 1067, 1815,   14,  138,
  958,  385,  514,  727, 1168,  137,  385,  671,  700,  752,
  671,  515,  385,  875,  492, 1137, 1405,  671,  640,  192,
  671,  449,  292,  450,  671,  116,  700, 1149,  482,  671,
  875, 1492,  671,  574, 1498,  583,  452,  687, 1780,  242,
 1279,  275,  875,  738,  876, 1195,  702, 1427,  671,  702,
 1600,  687,  959, 1493, 1494, 1171, 1172, 1474,  281,  688,
 1559, 1477,  116, 1617,  395, 1604,  232,  200,  201,  493,
 1360,  394,  671,  876,  207,  234,  671,  602, 1565, 1781,
  242,  397, 1261,  394,  603,  671,  494,  534,  322, 1326,
 1326, 1330,  574, 1357,  583, 1245, 1196,  116, 1502,  738,
 1281,  911,  671,  937, 1666,  702, 1285, 1246, 1668, 1245,
  138,  955,   76, 1360,  702,  688, 1705,  139,  369,  242,
  208,  369, 1793,  253,  688, 1690, 1782,  208,  534,  671,
 1365,  534, 1500,  686, 1669,   96,  323,  876, 1218,  385,
  452,  671, 1102,  254, 1568,  254,  534,  605,  534,  671,
  436,  254,  254,  973,  876, 1661, 1683,  482, 1103, 1686,
 1380,  258, 1382,  370,  702, 1709,  876, 1751,  615,  784,
  974,  707,   98,  262, 1691,  136,  708,  136,  671,  671,
 1602,  671,  605, 1620,  955, 1692, 1180,  674,  671,  671,
  671,  671,  671,  406,  671,  406,  671,  671, 1351, 1173,
 1761,  774,  580, 1104,  394,  702,  608,  370, 1574, 1734,
  605,  406, 1785,  605,  675,   76,   76,  482,  377, 1652,
  138, 1426,  370,  702, 1796, 1653, 1621,  615,  259,  605,
  369,  738, 1105,  866,  976, 1182,  866, 1762,  989,  738,
  704,   99,  369,  254,  369,  422,  304,  192,  324,  976,
 1763,  100,  600,  715,  423,  605,  266,  608,  471, 1786,
  702,  580,  704,  523,  281,  254,  325,  424,  738,  738,
  633,  738,  254,  377,  425,  136,  101, 1803,  738,  738,
  738,  738,  738, 1544,  738,  866,  738,  738, 1622, 1693,
  715,  229,  634, 1728, 1712, 1755, 1713,  267, 1618, 1106,
  326,  426,  471,  715,  427, 1545,  254,  282,   73,  704,
  102,  600, 1546,  524,  558, 1487, 1435,  471, 1408,  702,
 1439,  711,  709,   76,  710,  559,  712,  268,  230, 1804,
  251,  503,  428,  429, 1814,  633, 1769, 1753, 1726,  103,
  104, 1654,  105,  102,  430,  534, 1547,  525,  431,  106,
  107,  108,  109,  110,  700,  111,  635,  112,  113,  281,
  454,  534,  704,  700,  479,  526,  254,  623,  354,  481,
  582,  251,  103,  104,  251,  105,  633,  102, 1794,  633,
  281, 1797,  106,  107,  108,  109,  110,  534,  111,  251,
  112,  113,  385,  534,  633,  406,  534,  272,  509,  636,
  207,  501,  908,  385, 1809,  432,  103,  104,  623,  105,
 1505,  433,  989,  133, 1806, 1326,  106,  107,  108,  109,
  110,  434,  111,  916,  112,  113,  623,  354,  981,  582,
  192, 1787, 1788, 1188,  624,  379,  380,  381,  435,  200,
  201,  412,  662,  981,  287,  661,  208, 1189,  413,  436,
  409,  711,  709, 1615,  710,  142,  712,  161,  382,  383,
  437,  438,  439,  384,  385,  440,  441,  103,  442,  503,
  105, 1190,  102,  443,  444,  624,  445,  446,  447,  108,
  109,  110,  448,  111,  352,  112,  113,  637,  638,  639,
 1821,  370,  702,  624,  370,  702,   44,   52,  534,  303,
  412,  103,  104,  394,  105,  534,  686,  413,  664,  409,
  615,  106,  107,  108,  109,  110,  161,  111, 1619,  112,
  113,  162,  664,  534, 1188, 1191, 1649,  273,  707, 1256,
  664,  694,  288,  708, 1201,  699,  700,  702, 1189,  501,
   42,   76,  102,  352, 1202,  615,  664,  769, 1601, 1550,
  768,  702, 1487,  534,  969,   44,   52,  534, 1218,  707,
  702,   25, 1190,  749,  708,  877,  186,  289,  660,   48,
 1650,  103,  104,  615,  105,  312,  615,   50, 1257, 1487,
  162,  106,  107,  108,  109,  110, 1605,  111, 1626,  112,
  113,  487,  615,  532,  600,  290,  128,  380,  291,   42,
  664,  949,  974,  370,  711,  709,  142,  710, 1663,  712,
 1256,  189, 1664,  371,  292,  370, 1191,  370,  615, 1725,
   25, 1721,  749, 1698,  143,  281, 1487,  371,   48,  600,
  142,   26,   49,  217,  532,  371,   50,  532,  501,   51,
  293,  315,  951,  974,  974,  974,  974,  974,  974,  974,
  487,  371,  532,  633,  532,  128,  380,  600, 1679, 1260,
  600,  974,  974,  974,  974, 1656,  949,  208,  918, 1487,
 1188,   46,  509,  343,  217,  633,  600,  217, 1487,  501,
  192,  281,  501,  503, 1189,  623,  758,  144,  332,  707,
   26,   49,  217,  974,  708,  974,  129,  501,   51,  102,
 1647,  957,  600, 1735,  117,  371,   47,  623, 1190, 1093,
  143, 1487,  217,  758, 1487,  345,  142,  503,  503,  145,
  899,  509, 1061,  898,  920, 1060,  858, 1714,  103,  104,
   46,  105, 1805,  142,  146, 1094,  354, 1487,  106,  107,
  108,  109,  110,  503,  111,  223,  112,  113,   49, 1770,
  366,  186, 1737,  501,  503,  129,  791,  503,  792,   50,
  364,  143, 1191,  117, 1738,   47,   51,  142, 1714,   76,
  481,  217,  346,  915,  142,  368, 1068,  534,  931,  845,
   77,   78, 1791,  938,  939,  369,  223,  501,  501,  223,
 1181,   52,  946, 1180,  950,  951,  952,  953,  954, 1320,
  946, 1322,  230,  372,  223,  481,  143,  334,  373,  347,
  335,  348,  142,  501, 1714, 1080,  701, 1081,  534,  251,
  142,  534, 1199,  189,  501, 1060,  401,  501,  481,  867,
  132,  133,  867,  509,  349,  409,  534, 1206,  534,  410,
 1205, 1248,  707,  230, 1247, 1253,  230,  708, 1252, 1334,
  189,  532, 1333,  200,  201,  336,   53,  701, 1079,  350,
  251,  230, 1445,  251,  974, 1180,  974,  974,  413, 1480,
  532,  532, 1479,  532,  974,  701,  189,  532,  251,  416,
  532,  867,  974,  379,  380,  381, 1482,  974,   54, 1479,
  974, 1792,   55,  532, 1479,  231, 1032, 1033,  414,  532,
  711,  709,  418,  710,  974,  712,  124,  124,  419,  974,
  974,  384,  385,  974, 1818,  487,  501, 1734,  501,  420,
  974,  489,  974,  974,  490,  501,  974,  499,   14,  513,
  974,  522,  530,  974,  543,  974,  231,  974,  501,  231,
  550,  553,  974,  974,  563,  974, 1083, 1084, 1088,  617,
  567, 1096, 1098, 1100,  231, 1108, 1111, 1114, 1116, 1119,
 1122,  974,  974,  621,  974,  623,  625,  664,  655,  661,
  656,  532,  711,  709,  657,  710,  660,  712,  974,  669,
  974,  974,  974,  681,  678,  974,  683,  481,  974,  974,
  684,  685,  686,  532, 1152, 1153,  687, 1155,  974, 1157,
  532,  688, 1160, 1161, 1162, 1163, 1164, 1165,  688,  532,
  661,  532,  974,  689,  690,  974,  501,  691,  692, 1169,
  697,  939,  698,  223,  706,  223,  737,  955,  661,  532,
  714,  974,  223,  715,  532,  534,  717,  974,  501,  719,
  733,  514,  974,  738,  742,  501,  744,  711,  709,  745,
  710,  747,  712,  974,  974,  534,  974,  534,  748,  758,
  688,  534,  974,  532,  534,  759,  762,  761,  955,  955,
  955,  955,  955,  967,  955,  771,  796,  534,  797,  501,
  242,  795,  230,  534,  802,  102,  955,  955,  955,  955,
  807,  974,  974,  273,  974,  814,  811,  251,  812,  251,
  816,  974,  974,  974,  974,  974,  251,  974,  813,  974,
  974,  817,  822,  828,  103,  104,  829,  105,  955,  102,
  955,  832,  842,  223,  106,  107,  108,  109,  110,  833,
  111,  844,  112,  113,  701,  713,  711,  709,  707,  710,
  845,  712,  853,  708,  701,  223,  562,  701,  103,  104,
  854,  105,  223,  855,  857,  534,  701,  893,  106,  107,
  108,  109,  110,  894,  111,  896,  112,  113,  901,  905,
  529,  906,  944,  242,  933,  231,  946,  534, 1167,  965,
  242,  971,  980,  987,  534, 1688,  223,  562, 1302, 1303,
  562, 1305, 1307,  711,  709,  534,  710,  251,  712, 1312,
  983, 1001,  242,  701, 1006,  562,  823,  562, 1011,  242,
  707,  529,  701,  534,  529,  708, 1012, 1014,  534,  251,
  774, 1018, 1485,  702,  678, 1019,  251, 1021, 1027,  529,
  481,  529, 1688,  909, 1051,  913, 1035,  562,  917,  919,
  921, 1028, 1029,  242, 1343, 1038, 1058,  534, 1044, 1049,
  717, 1063, 1350,  386,  939, 1075,  701,  939,  674, 1169,
  251, 1090,  662, 1358,  939, 1361, 1362, 1076,  678, 1135,
  701,  939, 1097,  242, 1101, 1688, 1109, 1112, 1138, 1117,
 1120, 1123,  702,  678, 1775,  707, 1141,  661, 1133,  955,
  708,  955,  955, 1154, 1156,  242, 1151,  661, 1158,  955,
  661, 1166,  242,  662, 1178,  707,  662,  955, 1185,  661,
 1390, 1187,  955, 1193,  661,  955, 1299, 1688, 1208,  688,
 1688,  662, 1203,  711,  709, 1211,  710, 1220,  712,  955,
 1231,  955,  688,  523,  955,  955,  242, 1229,  955, 1241,
 1251,  688, 1249, 1688, 1258,  955, 1262,  955,  955, 1263,
 1264,  955, 1265, 1274,  569,  955,  661, 1069,  955,  481,
  955, 1276,  955, 1286, 1288,  661, 1283,  955,  955, 1287,
  955, 1289, 1291, 1292,  707, 1296, 1440,  711,  709,  708,
  710, 1297,  712,  524, 1298, 1448,  955,  955,  688,  955,
 1300, 1301, 1453, 1308, 1309, 1310, 1311,  688, 1313, 1314,
 1315,  663,  514,  955,  562,  955,  955,  955, 1316,  661,
  955,  515, 1317,  955,  955, 1318,  229,  525,  875,  481,
  562,  661, 1319,  955,  562, 1335,  562, 1353,  529, 1356,
  562,  707, 1363,  562, 1367,  526,  708,  955, 1368,  939,
  955,  688,  663,  939, 1369,  663,  562,  529,  529, 1370,
  529,  978,  562,  230,  529,  562,  955,  529, 1371, 1485,
  663, 1372,  955, 1376, 1379, 1385, 1387,  955,  711,  709,
  529,  710, 1381,  712,  545, 1395,  529, 1399,  955,  955,
 1407,  955, 1412, 1400, 1401, 1402, 1689,  955, 1403, 1404,
 1422, 1429,  978,  978,  978,  978,  978,  978,  978, 1433,
 1293, 1434, 1438,  377, 1444, 1464, 1465, 1469, 1475, 1541,
  978,  978,  978,  978, 1476, 1496,  955,  955, 1551,  955,
  196, 1554, 1553, 1556,  562, 1560,  955,  955,  955,  955,
  955,  426,  955, 1689,  955,  955, 1557, 1587, 1588, 1558,
 1561,  702,  978, 1555,  978,  678,  562, 1570,  529, 1562,
 1572,  702,  678,  562,  702,  678, 1575,  562, 1580,  678,
  378,  707,  939,  702,  562,  939,  708,  678, 1581, 1583,
  529, 1584, 1579, 1585, 1586, 1589, 1689,  529, 1590,  196,
  662,   95,  562,  678, 1591, 1776,  983,  562,  529, 1592,
  662, 1593, 1452,  662, 1594, 1595, 1596, 1290, 1628, 1597,
 1598, 1624,  662, 1607, 1606,  686,  529,  678, 1608, 1627,
  702,  529,  562,  124, 1630,  707,  562, 1635, 1689,  702,
  708, 1689,   96, 1637, 1629, 1631, 1632,  983,  983,  983,
  983,  983,  983,  983,   97,  432, 1633,  678, 1634, 1636,
  529, 1642,  678, 1643, 1689,  983,  983,  983,  983,  662,
 1645,  434,  711,  709, 1177,  710, 1646,  712,  662,   98,
 1662, 1667, 1672,  702,  678,  379,  380,  381, 1665,  200,
  201, 1673, 1699, 1707, 1682, 1720,  678,  983,  678,  983,
 1731,  716,  711,  709, 1638,  710, 1745,  712,  382,  383,
  437,  438,  439,  384,  385,  440,  441, 1747, 1750, 1754,
 1752,  373,  662,  443,  444, 1678,  707, 1759, 1760, 1680,
 1767,  708,  448,  978,  662,  978,  978,  711,  709,  663,
  710, 1479,  712,  978, 1774, 1742, 1773,  151,   99,  663,
 1743,  978,  663, 1790, 1798, 1795,  978, 1799,  100,  978,
 1734,  663,  373,  387,  686,  373, 1802, 1813,  386, 1816,
 1817,    1,  630,  978,   30,  203,   32,  753,  978,  978,
  373,  377,  978,  101,  271,  939,  275, 1727,  149,  978,
 1730,  978,  978,  532,  654,  978,  269,  468,   82,  978,
  938,  213,  978,  532,  978,  193,  978,  276,  663,  405,
  686,  978,  978,  684,  978,  680,  937,  663,  532,  196,
  682,  194,  275,  943,  711,  709,  289,  710, 1765,  712,
  978,  978,  378,  978,  287, 1159,  711,  709,  710,  710,
  196,  712,  709,  223,  324,  711,  288,  978,  669,  978,
  978,  978,  332,  341,  978,  323,  624,  978,  978,  330,
  675,  663,  720,   26,   29,   39, 1324,  978,  983,  196,
  983,  983,  129,  663,  711,  709, 1684,  710,  983,  712,
  417,  978,  408, 1284,  978,  407,  983,  843,  196,  669,
  243,  983,  669,  895,  983, 1074,  196,  196, 1563, 1282,
  978, 1280, 1270,  196, 1564,  532,  978,  669,  983,  808,
  707,  978,  576,  983,  983,  708, 1259,  983,  518, 1210,
  507,  196,  978,  978,  983,  978,  983,  983, 1543,  547,
  983,  978,  981,  196,  983,  196, 1675,  983,  548,  983,
  707,  983, 1271, 1016, 1741,  708,  983,  983, 1549,  983,
 1337,  711,  709,  790,  710, 1748,  712, 1451,  196, 1746,
  978,  978,  979,  978, 1784,  983,  983, 1009,  983, 1681,
  978,  978,  978,  978,  978,  707,  978, 1811,  978,  978,
  708, 1696,  983,  196,  983,  983,  983, 1778, 1389,  983,
  659, 1200,  983,  983, 1338,  711,  709,  555,  710, 1375,
  712, 1499,  983,  979,  979,  979,  979,  979,  979,  979,
 1706,  794,  538,  529, 1419, 1050,  983,  830,  377,  983,
  131,  979,  979,  979,  979, 1041,  682,  500, 1341,  711,
  709,  196,  710,  728,  712,  983,  732,  972,  985,  285,
  967,  983,  373, 1184,  968, 1671,  983, 1789, 1612,  373,
 1758, 1822,  373,  979, 1147,  979,  373,  983,  983,  741,
  983,  902,  707, 1355,  373, 1304,  983,  708,  658,    0,
    0,    0,    0,    0,  707,  378,    0,    0,    0,  708,
  373, 1342,  711,  709,    0,  710,    0,  712,    0,    0,
  196,  720,    0,    0,    0,  983,  983,  836,  983,    0,
    0,    0,    0,    0,  373,  983,  983,  983,  983,  983,
    0,  983,  707,  983,  983,    0,    0,  708,  922,  923,
  924,  925,  926,  927,  928,  929,    0,    0,  386, 1344,
  711,  709,    0,  710,  373,  712,    0,   59,  836,  836,
  836,  836,  836,    0,  836,    0,  275,    0,   60,    0,
    0,    0,    0,    0,    0,   61,  836,  836,  836,  836,
    0,  373,    0,    0,    0,    0,  669,    0,   62,  669,
    0,    0,    0,  373,    0,  373,  669,    0,    0,  669,
  379,  380,  381,  669,  200,  201,    0,    0,  669,  707,
  836,  669,    0,    0,  708,    0,    0,    0,    0,  152,
    0,    0,    0,  382,  383,    0,    0,  669,  384,  385,
    0, 1345,  711,  709,    0,  710,    0,  712,    0,    0,
    0,    0,    0,    0,  979,    0,  979,  979,    0,   63,
    0,  669,    0,  707,  979,  669,    0,    0,  708,    0,
    0,    0,  979,    0,  669,   64,    0,  979,    0,    0,
  979,    0,    0,    0,    0,  153,    0,    0,  152,    0,
    0,  669,   65,    0,  979,    0,    0,  707,    0,  979,
  979,    0,  708,  979,    0,    0,    0,   66,    0,    0,
  979,   67,  979,  979,   68,    0,  979,  102,  669,  196,
  979,    0,   69,  979,    0,  979,    0,  979,    0,    0,
  669,    0,  979,  979,    0,  979,    0,    0,    0,    0,
  196,   70,    0,    0,  153,  177,  103,  104,    0,  105,
  707,  979,  979,    0,  979,  708,  106,  107,  108,  109,
  110,  196,  111,    0,  112,  113,    0,  196,  979,  666,
  979,  979,  979,    0,    0,  979,    0,    0,  979,  979,
    0, 1346,  711,  709,    0,  710,    0,  712,  979,  836,
  196,  836,  836,    0,    0,  534,    0,    0,  707,  836,
    0,    0,  979,  708,  177,  979,    0,  836,  377,  196,
  666,    0,  836,  666,    0,  836,    0,  196,  196,    0,
    0,  979,    0,    0,  196,    0,    0,  979,  666,  836,
    0,    0,  979,    0,  836,  836,  534,    0,  836,  534,
    0,    0,  196,  979,  979,  836,  979,  836,  836,    0,
    0,  836,  979,    0,  534,  836,    0,  511,  836,    0,
  836,    0,  836,    0,    0,  378,    0,  836,  836,    0,
  836, 1347,  711,  709,    0,  710,    0,  712,    0,  196,
  707,  979,  979,  848,  979,  708,  836,  836,    0,  836,
  196,  979,  979,  979,  979,  979,    0,  979,  511,  979,
  979,  511,    0,  836,    0,  836,  836,  836,  152,    0,
  836,    0,    0,  836,  836,    0,  511,    0,    0,    0,
    0,    0,    0,  836,  848,  848,  848,  848,  848,  152,
  848,    0,    0,    0,    0,    0,    0,  836,  187,    0,
  836,    0,  848,  848,  848,  848,    0,    0,    0, 1348,
  711,  709,  188,  710,    0,  712,  836,    0,  152,    0,
    0,    0,  836,    0,  153,    0,    0,  836,    0,    0,
  379,  380,  381,    0,  200,  201,  848,  152,  836,  836,
    0,  836,    0,    0,    0,  153,  152,  836,    0,    0,
    0,    0,  152,  382,  383,    0,    0,  187,  384,  385,
    0,    0,    0,  177,    0,    0,    0,    0,    0,    0,
  152,  188,    0,    0,  153,    0,  836,  836,  851,  836,
  707,    0,    0,    0,  177,  708,  836,  836,  836,  836,
  836,    0,  836,  153,  836,  836,    0,    0,    0,    0,
    0,    0,  153,    0,  534,  177,    0,    0,  153,    0,
    0,  177,    0,    0,    0,    0,    0,    0,    0,  851,
  851,  851,  851,  851,    0,  851,  153,    0,    0,    0,
    0,    0,    0,  534,  177,  534,    0,  851,  851,  851,
  851,    0,  534,    0,    0,  534,    0,  666,  534,    0,
  666,    0,    0,  177,    0,  534,    0,  666,    0,    0,
  666,    0,  177,  534,  666,    0,    0,    0,  177,  666,
  707,  851,  666,    0,    0,  708, 1349,  711,  709,    0,
  710,    0,  712, 1470,  711,  709,  177,  710,  666,  712,
    0,    0, 1471,  711,  709,  511,  710,  511,  712,    0,
    0,  511,    0,    0,  511,  848,    0,  848,  848,    0,
    0,    0,  666,    0,    0,  848,  666,  511,    0,    0,
    0,    0,    0,  177,    0,  666,    0,    0,  848,    0,
    0,  848,    0,  534,    0,    0,    0,    0,    0,    0,
    0,    0,  666,    0,    0,  848,    0,    0,  707,    0,
  848,  848,    0,  708,  848,  534,  187,    0,    0,    0,
    0,  848,  534,  848,  848,    0,    0,  848,    0,  666,
  188,  848,    0,    0,  848,    0,  848,  187,  848,    0,
    0,  666,    0,  848,  848,    0,  848,    0,    0,    0,
    0,  188,    0,  181,    0,  511,  534,    0,  187,    0,
    0,    0,  848,  848,  187,  848,    0,    0,    0, 1478,
  711,  709,  188,  710,    0,  712,    0,  511,  188,  848,
    0,  848,  848,  848,  511,    0,  848,  187,    0,  848,
  848,    0,    0,  818,  181,    0,    0,  181,    0,  848,
  851,  188,  851,  851,  545,    0,  187,    0,    0,    0,
  851,    0,  181,  848,    0,  187,  848,    0,  511,    0,
  188,  187,    0,    0,    0,    0,  851,    0,    0,  188,
    0,    0,  848,    0,  818,  188,    0,  818,  848,  187,
  851,    0,  534,  848,  534,  851,  851,    0,  534,  851,
    0,  534,  818,  188,  848,  848,  851,  848,  851,  851,
    0,    0,  851,  848,  534,    0,  851,    0,    0,  851,
    0,  851,    0,  851,    0,  707,  187,    0,  851,  851,
  708,  851,  707,    0,    0,    0,    0,  708,    0,    0,
  188,  707,  848,  848,  852,  848,  708,  851,  851,    0,
  851,    0,  848,  848,  848,  848,  848,    0,  848,    0,
  848,  848,    0,    0,  851,    0,  851,  851,  851,    0,
    0,  851,    0,    0,  851,  851,    0,    0,    0,    0,
    0,    0,    0,    0,  851,  852,  852,  852,  852,  852,
    0,  852,  534,    0,    0,    0,    0,    0,  851,    0,
    0,  851,    0,  852,  852,  852,  852,    0,    0,    0,
    0,    0,    0,   81,  534,    0,    0,  851,    0,    0,
   82,  534,  551,  851,   83,   84,    0,  102,  851,    0,
    0,    0,    0,   85,    0,    0,    0,  852,    0,  851,
  851,    0,  851,    0,    0,   86,    0,    0,  851,    0,
    0,    0,    0,    0,    0,  534,  103,  104,  707,  105,
    0,  181,    0,  708,    0,    0,  106,  107,  108,  109,
  110,    0,  111,    0,  112,  113,    0,  851,  851,  846,
  851,    0,  181,    0,    0,    0,    0,  851,  851,  851,
  851,  851,    0,  851,    0,  851,  851,    0,    0,    0,
    0,    0,    0,  181,    0,  818,    0,    0,    0,  181,
   87,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  846,    0,  846,  846,  846,    0,    0,   88,    0,   95,
    0,    0,  181,    0,    0,    0,    0,    0,  846,  846,
  846,  846,   89,    0,    0,    0,   90,    0,    0,    0,
    0,  181,    0,    0,    0,    0,    0,   91,    0,    0,
  181,  818,    0,    0,  818,    0,  181,    0,    0,    0,
   96,  818,  846,    0,  818,    0,   92,    0,  818,    0,
    0,    0,   97,  818,  181,    0,  818,    0,    0,    0,
    0,    0,    0,    0,    0,  102,  126,    0,    0,    0,
    0,    0,  818,    0,    0,    0,  852,   98,  852,  852,
    0,    0,    0,    0,    0,    0,  852,    0,    0,    0,
    0,  181,    0,    0,  103,  104,  818,  105,    0,  818,
  818,    0,  852,    0,  106,  107,  108,  109,  110,  818,
  111,    0,  112,  113,    0,    0,  852,    0,    0,    0,
    0,  852,  852,    0,    0,  852,  818,    0,    0,    0,
    0,    0,  852,    0,  852,  852,    0,    0,  852,    0,
    0,    0,  852,    0,    0,  852,   99,  852,  818,  852,
    0,    0,    0,  818,  852,  852,  100,  852,    0,    0,
    0,    0,    0,    0,    0,  818,    0,  818,    0,    0,
    0,    0,    0,  852,  852,    0,  852,    0,    0,    0,
    0,  101,    0,    0,    0,    0,    0,    0,    0,    0,
  852,    0,  852,  852,  852,    0,    0,  852,    0,  102,
  852,  852,    0,    0,  787,    0,    0,    0,  882,    0,
  852,  846,    0,  846,  846,    0,    0,    0,    0,    0,
    0,  846,    0,    0,  852,    0,    0,  852,  103,  104,
    0,  105,    0,    0,    0,    0,    0,  846,  106,  107,
  108,  109,  110,  852,  111,  787,  112,  113,  787,  852,
    0,  846,    0,    0,  852,    0,  846,  846, 1609,    0,
  846,    0,    0,  787,    0,  852,  852,  846,  852,  846,
  846,    0,    0,  846,  852,    0,    0,  846,    0,    0,
  846,    0,  846, 1610,  846,    0,    0,    0,    0,  846,
  846,    0,  846,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  852,  852,  845,  852,    0,  846,  846,
    0,  846,    0,  852,  852,  852,  852,  852,    0,  852,
    0,  852,  852,  885,    0,  846,    0,  846,  846,  846,
    0,    0,  846,    0,    0,  846,  846,    0,    0,    0,
  102, 1611,    0,    0,    0,  846,  845,    0,  845,  845,
  845,    0,    0,    0,    0,    0,    0,    0,    0,  846,
    0,    0,  846,    0,  845,  845,  845,  845,    0,  103,
  104,    0,  105,    0,    0,  102,    0,    0,  846,  106,
  107,  108,  109,  110,  846,  111,    0,  112,  113,  846,
    0,    0,    1,    0,    0,    0,    0,    0,  845,    0,
  846,  846,    0,  846,  103,  104,    0,  105,    0,  846,
    0,    0,    0,    2,  106,  107,  108,  109,  110,    0,
  111,    3,  112,  113,    0,    0,    0,    0,    4,    0,
    5,  102,    6,    0,    0,    0,    0,    7,  846,  846,
  847,  846,    0,    0,    8,    0,    0,  402,  846,  846,
  846,  846,  846,    0,  846,    0,  846,  846,    9,    0,
  103,  104,    0,  105,  102,    0,  787,    0,    0,   10,
  106,  107,  108,  109,  110,    0,  111,    0,  112,  113,
    0,  847,    0,  847,  847,  847,    0,    0,    0,    0,
    0,    0,    0,  103,  104,    0,  105,    0,    0,  847,
  847,  847,  847,  106,  107,  108,  109,  110,    0,  111,
    0,  112,  113,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  787,    0,  428,  787,    0,    0,    0,    0,
  398,    0,  787,  847,    0,  787,   11,    0,    0,  787,
    0,    0,    0,   12,  787,    0,   13,  787,   14,   15,
    0,    0,    0,    0,    0,    0,  102,    0,    0,    0,
    0,    0,    0,  787,    0,    0,    0,  845,    0,  845,
  845,    0,    0,   16,    0,    0,    0,  845,    0,    0,
    0,    0,    0,    0,    0,  103,  104,  787,  105,    0,
  787,  787,    0,  845,    0,  106,  107,  108,  109,  110,
  787,  111,    0,  112,  113,    0,    0,  845,    0,    0,
    0,    0,  845,  845,    0,    0,  845,  787,    0,    0,
    0,    0,   17,  845,    0,  845,  845,    0,   73,  845,
  435,    0,    0,  845,    0,    0,  845,   18,  845,  787,
  845,  436,    0,    0,  787,  845,  845,    0,  845,    0,
    0,    0,    0,    0,    0,    0,  787,    0,  787,  103,
  442,    0,  105,    0,  845,  845,    0,  845,  445,  446,
  447,  108,  109,  110,    0,  111,    0,  112,  113,    0,
    0,  845,    0,  845,  845,  845,    0,    0,  845,    0,
    0,  845,  845,    0,  539,    0,    0,    0,    0,    0,
    0,  845,  847,    0,  847,  847,    0,    0,    0,    0,
    0,    0,  847,    0,    0,  845,    0,    0,  845,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  847,    0,
    0,    0,    0,    0,  845,  539,    0,    0,  539,    0,
  845,    0,  847,    0,    0,  845,    0,  847,  847,    0,
    0,  847,    0,  539,    0,  539,  845,  845,  847,  845,
  847,  847,    0,    0,  847,  845,    0,    0,  847,    0,
    0,  847,    0,  847,    0,  847,    0,    0,    0,    0,
  847,  847,    0,  847,    0,  539,    0,    0,    0,    0,
    0,    0,    0,    0,  845,  845,  850,  845,    0,  847,
  847,    0,  847,    0,  845,  845,  845,  845,  845,    0,
  845,    0,  845,  845,    0,    0,  847,    0,  847,  847,
  847,    0,    0,  847,    0,    0,  847,  847,    0,    0,
    0,  572,    0,    0,    0,    0,  847,  850,    0,  850,
  850,  850,    0,  882,    0,    0,    0,    0,    0,    0,
  847,    0,    0,  847,    0,  850,  850,  850,  850,    0,
  572,  572,    0,  572,    0,    0,    0,    0,    0,  847,
  572,  572,  572,  572,  572,  847,  572,    0,  572,  572,
  847,    0,    0,    0,    0,    0,    0,    0,    0,  850,
    0,  847,  847,    0,  847,    0,    0,    0,    0,    0,
  847,    0,    0,  943,  711,  709,    0,  710,    0,  712,
    0,    0,    0,  640,    0,    0,  449,  883,  450,    0,
    0,    0,  873,  872,  874,    0,    0,    0,    0,  847,
  847,  452,  847,    0,    0,    0,  275,    0,    0,  847,
  847,  847,  847,  847,    0,  847,    0,  847,  847,    0,
    0,    0,  539,  884,    0,    0,    0,    0,  885,    0,
    0,    0,    0,    0,  886,    0,    0,    0,  539,    0,
    0,  539,  539,  221,  539,  102,    0,  221,  539,    0,
    0,  539,    0,  887,    0,    0,    0,    0,    0,    0,
    0,  221,    0,    0,  539,  221,    0,    0,  221,    0,
  539,    0,    0,  539,  103,  104,    0,  105,    0,    0,
    0,    0,    0,    0,  106,  107,  108,  109,  110,    0,
  111,    0,  112,  113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,    0,    0,    0,    0,  221,  221,  221,  281,  221,
  221,  221,    0,  221,    0,    0,    0,    0,  850,    0,
  850,  850,    0,    0,    0,    0,    0,    0,  850,    0,
    0,    0,  539,  221,    0,  221,    0,  281,  281,    0,
  281,    0,    0,    0,  850,    0,    0,  281,  281,  281,
  281,  281,    0,  281,  539,  281,  281,    0,  850,    0,
    0,  539,    0,  850,  850,  539,  221,  850,    0,    0,
    0,   81,  539,    0,  850,    0,  850,  850,    0,    0,
  850,    0,    0,  221,  850,    0,    0,  850,    0,  850,
  539,  850,    0,    0,  860,  539,  850,  850,    0,  850,
  422,    0,  707,  640,    0,  221,  449,  708,  450,  423,
  861,    0,    0,    0,    0,  850,  850,    0,  850,    0,
  539,  452,  424,    0,  539,    0,  275,    0,  377,  425,
    0,    0,  850,    0,  850,  850,  850,    0,    0,  850,
  221,  221,  850,  850,    0,    0,  862,  634,    0,    0,
    0,    0,  850,    0,    0,  863,  426,  221,    0,  427,
    0,    0,    0,  864,    0,    0,  850,    0,    0,  850,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  544,
    0,    0,  865,    0,  866,  850,    0,  428,  429,    0,
    0,  850,    0,    0,    0,    0,  850,    0,  867,  430,
  868,  869,  870,  431,    0,    0,    0,  850,  850,    0,
  850,  635,    0,    0,    0,    0,  850,    0,    0,  544,
  544,    0,    0,  544,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  544,    0,
  544,    0,    0,    0,    0,  850,  850,    0,  850,    0,
  701,  871,    0,    0,  636,  850,  850,  850,  850,  850,
  432,  850,    0,  850,  850,    0,  433,    0,    0,    0,
  544,    0,    0,    0,    0,    0,  434,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  379,  380,  381,  435,  200,  201,    0,    0,    0,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,  382,  383,  437,  438,  439,  384,  385,
  440,  441,  103,  442,  545,  105,    0,    0,  443,  444,
  422,  445,  446,  447,  108,  109,  110,  448,  111,  423,
  112,  113,  637,  638,  639,    0,    0,    0,    0,    0,
    0,  552,  424,    0,    0,    0,    0,    0,  377,  425,
    0,    0,    0,  536,  545,  545,    0,    0,  545,    0,
    0,    0,    0,    0,    0,    0,    0,  634,    0,    0,
    0,    0,    0,  545,    0,  545,  426,    0,    0,  427,
    0,    0,  552,    0,    0,  552,    0,    0,    0,    0,
    0,    0,    0,    0,  536,    0,    0,  536,    0,    0,
  552,    0,  552,    0,    0,  545,    0,  428,  429,    0,
    0,    0,  536,    0,  536,    0,    0,    0,    0,  430,
    0,    0,    0,  431,    0,    0,    0,  544,    0,    0,
    0,  635,  552,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  544,  536,    0,  544,  544,    0,  544,
    0,    0,    0,  544,    0,    0,  544,    0,    0,    0,
    0,  645,    0,  645,  645,    0,  645,    0,    0,  544,
    0,    0,    0,    0,  636,  544,    0,    0,  544,  645,
  432, 1441, 1442,    0,  645,    0,  433, 1446, 1447,    0,
 1449, 1450,    0,    0,    0,    0,  434,    0, 1454, 1455,
 1456, 1457,    0, 1458, 1459, 1460, 1461, 1462, 1463,    0,
  379,  380,  381,  435,  200,  201,    0,    0,    0,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,  382,  383,  437,  438,  439,  384,  385,
  440,  441,  103,  442,    0,  105,    0,  544,  443,  444,
    0,  445,  446,  447,  108,  109,  110,  448,  111,    0,
  112,  113,  637,  638,  639,    0,    0,    0,    0,  544,
    0,    0,  545,    0,    0,    0,  544,    0,    0,    0,
  544,    0,    0,    0,    0,    0,    0,  544,  545,    0,
    0,  545,  545,    0,  545,    0,    0,    0,  545,  552,
    0,  545,    0,    0,    0,  544,    0,    0,    0,    0,
  544,  536,    0,    0,  545,  552,  769,    0,    0,  552,
  545,  552,    0,  545,    0,  552,    0,  536,  552,    0,
    0,  536,    0,  536,    0,  544,    0,  536,    0,  544,
  536,  552,    0,    0,    0,    0,    0,  552,    0,    0,
  552,    0,    0,  536,    0,    0,    0,  769,    0,  536,
  769,    0,  536,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  769,    0,    0,    0,    0,
    0,  645,    0,    0,    0,    0,    0,    0,  645,    0,
    0,    0,  545,    0,    0,    0,    0,  645,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  645,    0,    0,    0,  545,    0,  645,  645,    0,  552,
    0,  545,    0,    0,  645,  545,    0,    0,    0,    0,
    0,  536,  545,  643,    0,  643,  643,    0,  643,    0,
    0,  552,    0,    0,  645,    0,    0,  645,  552,    0,
  545,  643,  552,  536,    0,  545,  643,    0,    0,  552,
  536,    0,    0,    0,  536,    0,    0,    0,    0,    0,
    0,  536,    0,  645,  788,  645,  645,  552,    0,    0,
  545,    0,  552,    0,  545,   95,    0,  645,    0,  536,
    0,  645,    0,    0,  536,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  552,    0,    0,
    0,  552,    0,    0,    0,  788,    0,  451,  788,  536,
  449,    0,  450,  536,    0,    0,   96,    0,    0,    0,
    0,    0,    0,  788,    0,  452,    0,    0,   97,    0,
  275,    0,    0,    0,    0,    0,    0,    0,  645,    0,
    0,    0,    0,    0,  645,    0,    0,    0,    0,    0,
    0,    0,    0,   98,  645,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  645,  645,
  645,  645,  645,  645,    0,    0,    0,    0,    0,    0,
    0,    0,  645,    0,    0,    0,    0,    0,    0,    0,
    0,  645,  645,  645,  645,  645,  645,  645,  645,  645,
  645,  645,    0,  645,    0,    0,  645,  645,    0,  645,
  645,  645,  645,  645,  645,  645,  645,    0,  645,  645,
    0,    0,   99,    0,  769,    0,    0,  769,    0,    0,
    0,    0,  100,    0,  769,    0,    0,  769,    0,    0,
    0,  769,    0,  643,    0,    0,  769,    0,    0,  769,
  643,    0,    0,    0,    0,    0,    0,  101,    0,  643,
    0,    0,    0,    0,    0,  769,    0,    0,    0,    0,
    0,    0,  643,    0,    0,  102,    0,    0,  643,  643,
    0,    0,    0,    0,    0,    0,  643,    0,    0,  769,
    0,    0,  769,  769,    0,    0,    0,    0,    0,    0,
    0,    0,  769,    0,  103,  104,  643,  105,    0,  643,
    0,    0,    0,    0,  106,  107,  108,  109,  110,  769,
  111,    0,  112,  113,    0,    0,  788, 1085,    0,    0,
 1086,    0,    0,    0,  422,  643,    0,  643,  643,    0,
    0,  769,    0,  423,    0,    0,  769,    0,    0,  643,
    0,    0,    0,  643,    0,    0,  424,    0,  769,    0,
  769,  451,  377,  425,  449,    0,  450,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  452,
    0,    0,  788,    0,  275,  788,    0,    0,    0,    0,
  426,    0,  788,  427,    0,  788,    0,    0,    0,  788,
    0,    0,    0,    0,  788,    0,    0,  788,    0,    0,
  643,    0,    0,    0,    0,    0,  643,    0,    0,    0,
    0,  428,  429,  788,    0,    0,  643,    0,    0,    0,
    0,    0,    0,  430,    0,    0,    0,  431,    0,    0,
  643,  643,  643,  643,  643,  643,    0,  788,    0,    0,
  788,  788,    0,    0,  643,    0,    0,    0,    0,    0,
  788,    0,    0,  643,  643,  643,  643,  643,  643,  643,
  643,  643,  643,  643,    0,  643,    0,  788,  643,  643,
    0,  643,  643,  643,  643,  643,  643,  643,  643,    0,
  643,  643, 1087,    0,  432,    0,    0,    0,    0,  788,
  433,    0,    0,    0,  788,    0,    0,    0,    0,    0,
  434,    0,    0,    0,    0,    0,  788,    0,  788,    0,
    0,    0,    0,    0,  379,  380,  381,  435,  200,  201,
    0,    0,    0,    0,    0,    0,    0,    0,  436,    0,
    0,    0,    0,    0,    0,    0,    0,  382,  383,  437,
  438,  439,  384,  385,  440,  441,  103,  442,    0,  105,
    0,    0,  443,  444,    0,  445,  446,  447,  108,  109,
  110,  448,  111,    0,  112,  113,    0,    0,    0,    0,
    0, 1095,    0,    0, 1086,    0,    0,    0,  422,    0,
    0,    0,    0,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  424,    0,    0,    0,    0,  451,  377,  425,  449,    0,
  450,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  452,    0,    0,    0,    0,  275,    0,
    0,    0,    0,    0,  426,    0,    0,  427,    0,    0,
    0,    0,    0,    0,    0,    0,  812,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  428,  429,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  430,    0,    0,
    0,  431,    0,    0,    0,    0,    0,  812,    0,  451,
  812,    0,  449,    0,  450,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  812,    0,  452,    0,    0,
    0,    0,  275,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1087,    0,  432,    0,
    0,    0,    0,    0,  433,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  434,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  379,  380,
  381,  435,  200,  201,    0,    0,    0,    0,    0,    0,
    0,    0,  436,    0,    0,    0,    0,    0,    0,    0,
    0,  382,  383,  437,  438,  439,  384,  385,  440,  441,
  103,  442,    0,  105,  846,    0,  443,  444,  578,  445,
  446,  447,  108,  109,  110,  448,  111,    0,  112,  113,
    0,    0,  847,    0,    0, 1099,  579,    0, 1086,  580,
    0,    0,  422,    0,    0,    0,    0,    0,    0,    0,
    0,  423,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,    0,
  377,  425,    0,    0,    0,    0,    0,    0,    0,    0,
  581,    0,    0,    0,    0,    0,  582,  583,  584,    0,
  585,  586,  587,    0,  588,    0,    0,    0,  426,    0,
    0,  427,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  589,    0,  590,    0,  812, 1107,
    0,    0, 1086,    0,    0,    0,  422,    0,    0,  428,
  429,    0,    0,    0,    0,  423,    0,    0,    0,    0,
    0,  430,    0,    0,    0,  431,    0,  591,  424,    0,
    0,    0,    0,  451,  377,  425,  449,    0,  450,    0,
    0,    0,    0,    0,  592,    0,    0,    0,    0,    0,
    0,  452,    0,    0,  812,    0,  275,  812,    0,    0,
    0,    0,  426,    0,  812,  427,  593,  812,    0,    0,
    0,  812,    0,    0,    0,    0,  812,    0,    0,  812,
 1087,    0,  432,    0,    0,    0,    0,    0,  433,    0,
    0,    0,    0,  428,  429,  812,    0,    0,  434,    0,
    0,  594,  595,    0,    0,  430,    0,    0,    0,  431,
    0,    0,  379,  380,  381,  435,  200,  201,  596,  812,
    0,    0,  812,  812,    0,    0,  436,    0,    0,    0,
    0,    0,  812,    0,    0,  382,  383,  437,  438,  439,
  384,  385,  440,  441,  103,  442,    0,  105,    0,  812,
  443,  444,    0,  445,  446,  447,  108,  109,  110,  448,
  111,    0,  112,  113, 1087,    0,  432,    0,    0,    0,
    0,  812,  433,    0,    0,    0,  812,    0,    0,    0,
    0,    0,  434,    0,    0,    0,    0,    0,  812,    0,
  812,    0,    0,    0,    0,    0,  379,  380,  381,  435,
  200,  201,    0,    0,    0,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,    0,    0,    0,    0,  382,
  383,  437,  438,  439,  384,  385,  440,  441,  103,  442,
    0,  105,    0,    0,  443,  444,    0,  445,  446,  447,
  108,  109,  110,  448,  111,    0,  112,  113,    0,    0,
    0,    0,    0, 1110,    0,    0, 1086,    0,    0,    0,
  422,    0,    0,    0,    0,    0,    0,    0,    0,  423,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  451,  377,  425,
  449,    0,  450,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  452,    0,    0,    0,    0,
  275,    0,    0,    0,    0,    0,  426,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,  791,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,  429,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  430,
    0,    0,    0,  431,    0,    0,    0,    0,    0,  791,
    0,  451,  791,    0,  449,    0,  450,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  791,    0,  452,
    0,    0,    0,    0,  275,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1087,    0,
  432,    0,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  434,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  379,  380,  381,  435,  200,  201,    0,    0,    0,    0,
    0,    0,    0,    0,  436,    0,    0,    0,    0,    0,
    0,    0,    0,  382,  383,  437,  438,  439,  384,  385,
  440,  441,  103,  442,    0,  105,  221,    0,  443,  444,
  221,  445,  446,  447,  108,  109,  110,  448,  111,    0,
  112,  113,    0,    0,  221,    0,    0, 1115,  221,    0,
 1086,  221,    0,    0,  422,    0,    0,    0,    0,    0,
    0,    0,    0,  423,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  424,    0,    0,    0,
    0,    0,  377,  425,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,  221,  221,
  221,    0,  221,  221,  221,    0,  221,    0,    0,    0,
  426,    0,    0,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,    0,  221,    0,
  791, 1118,    0,    0, 1086,    0,    0,    0,  422,    0,
    0,  428,  429,    0,    0,    0,    0,  423,    0,    0,
    0,    0,    0,  430,    0,    0,    0,  431,    0,  221,
  424,    0,    0,    0,    0,  451,  377,  425,  449,    0,
  450,    0,    0,    0,    0,    0,  221,    0,    0,    0,
    0,    0,    0,  452,    0,    0,  791,    0,  275,  791,
    0,    0,    0,    0,  426,    0,  791,  427,  221,  791,
    0,    0,    0,  791,    0,    0,    0,    0,  791,    0,
    0,  791, 1087,    0,  432,    0,    0,    0,    0,    0,
  433,    0,    0,    0,    0,  428,  429,  791,    0,    0,
  434,    0,    0,  221,  221,    0,    0,  430,    0,    0,
    0,  431,    0,    0,  379,  380,  381,  435,  200,  201,
  221,  791,    0,    0,  791,  791,    0,    0,  436,    0,
    0,    0,    0,    0,  791,    0,    0,  382,  383,  437,
  438,  439,  384,  385,  440,  441,  103,  442,    0,  105,
    0,  791,  443,  444,    0,  445,  446,  447,  108,  109,
  110,  448,  111,    0,  112,  113, 1087,    0,  432,    0,
    0,    0,    0,  791,  433,    0,    0,    0,  791,    0,
    0,    0,    0,    0,  434,    0,    0,    0,    0,    0,
  791,    0,  791,    0,    0,    0,    0,    0,  379,  380,
  381,  435,  200,  201,    0,    0,    0,    0,    0,    0,
  789,    0,  436,    0,    0,    0,    0,    0,    0,    0,
    0,  382,  383,  437,  438,  439,  384,  385,  440,  441,
  103,  442,    0,  105,    0,    0,  443,  444,    0,  445,
  446,  447,  108,  109,  110,  448,  111,    0,  112,  113,
    0,  789,    0,    0,  789, 1121,    0,    0, 1086,    0,
    0,    0,  422,    0,    0,  451,    0,  480,  449,  789,
  450,  423,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  452,  424,    0,    0,    0,  275,    0,
  377,  425,    0,    0,    0,    0,    0,    0,  577,    0,
    0,    0,  578,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  426,    0,
  579,  427,    0,  580,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  711,  709,    0,  710,    0,
  712,    0,    0,    0,    0,    0,    0,    0,    0,  428,
  429,    0,    0,  873,  872,  874,    0,    0,    0,    0,
    0,  430,    0,  451,  581,  431,  449,    0,  450,    0,
  582,  583,  584,    0,  585,  586,  587,    0,  588,    0,
    0,  452,    0,    0,    0,    0,  275,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  589,    0,
  590,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1087,    0,  432,    0,    0,    0,    0,    0,  433,    0,
    0,  591,    0,    0,    0,    0,    0,    0,  434,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  592,    0,
    0,    0,  379,  380,  381,  435,  200,  201,    0,    0,
    0,    0,    0,    0,    0,    0,  436,    0,    0,    0,
  593,    0,  789,    0,    0,  382,  383,  437,  438,  439,
  384,  385,  440,  441,  103,  442,    0,  105,    0,    0,
  443,  444,  422,  445,  446,  447,  108,  109,  110,  448,
  111,  423,  112,  113,    0,  594,  595,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,    0,
  377,  425,  596,    0,    0,    0,    0,    0,  789,    0,
    0,  789,    0,    0,    0,    0,    0,    0,  789,    0,
    0,  789,    0,    0,    0,  789,    0,    0,  426,    0,
  789,  427,    0,  789,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  860,    0,    0,    0,  789,
    0,    0,    0,  707,    0,    0,    0,  378,  708,  428,
  429,  861,    0,    0,    0,    0,    0,    0,    0,    0,
  422,  430,    0,  789,    0,  431,  789,  789,    0,  423,
    0,    0,  949,    0,  949,  949,  789,  949,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  862,  377,  425,
  949,    0,    0,  789,    0,  949,  863,    0,    0,    0,
    0,    0,    0,    0,  864,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  789,  426,    0,    0,  427,
  789,    0,  432,  865,    0,  866,    0,    0,  433,    0,
    0,    0,  789,    0,  789,    0,    0,    0,  434,  867,
    0,  868,  869,  870,    0,  378,    0,  428,  429,    0,
    0,    0,  379,  380,  381,  435,  200,  201,    0,  430,
    0,    0,    0,  431,    0,    0,  436,    0,    0,    0,
    0,    0,    0,    0,    0,  382,  383,  437,  438,  439,
  384,  385,  440,  441,  103,  442,    0,  105,    0,    0,
  443,  444,  871,  445,  446,  447,  108,  109,  110,  448,
  111,    0,  112,  113,    0,    0,    0,    0,    0,    0,
  701,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  434,    0,  134,    0,
    0,    0,  134,    0,    0,    0,    0,    0,    0,    0,
  379,  380,  381,  435,  200,  201,    0,    0,    0,    0,
  134,    0,    0,  134,  436,    0,    0,    0,    0,    0,
    0,    0,    0,  382,  383,  437,  438,  439,  384,  385,
  440,  441,  103,  442,    0,  105,    0,    0,  443,  444,
    0,  445,  446,  447,  108,  109,  110,  448,  111,  949,
  112,  113,    0,    0,  134,    0,    0,    0,  949,    0,
  134,  134,  134,    0,  134,  134,  134,    0,  134,    0,
    0,  949,    0,    0,    0,    0,  645,  949,  949,  645,
    0,  645,    0,    0,    0,    0,    0,    0,  134,    0,
  134,    0,    0,    0,  645,    0,    0,    0,    0,  645,
    0,    0,    0,    0,    0,  949,    0,    0,  949,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  134,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  949,    0,  949,  949,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  949,    0,
    0,    0,  949,    0,    0,    0,    0,    0,  451,    0,
  134,  449,    0,  450,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  452,    0,    0,    0,
    0,  275,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  134,  134,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  949,
    0,    0,  134,    0,    0,  949,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  949,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  949,
  949,  949,  949,  949,  949,    0,    0,    0,    0,    0,
    0,    0,    0,  949,    0,    0,    0,    0,    0,    0,
    0,    0,  949,  949,  949,  949,  949,  949,  949,  949,
  949,  949,  949,    0,  949,    0,    0,  949,  949,    0,
  949,  949,  949,  949,  949,  949,  949,  949,    0,  949,
  949,    0,    0,    0,    0,    0,  645,    0,    0,    0,
    0,    0,    0,  645,    0,    0,    0,    0,    0,    0,
  790,    0,  645,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  645,    0,    0,    0,    0,
    0,  645,  645,    0,    0,    0,    0,    0,    0,  645,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  790,    0,    0,  790,    0,    0,    0,    0,  645,
    0,    0,  645,    0,    0,    0,    0,    0,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  422,    0,    0,    0,    0,
  645,  645,    0,    0,  423,    0,    0,    0,    0,    0,
  451,    0,  645,  449,    0,  450,  645,  424,    0,    0,
    0,    0,    0,  377,  425,    0,    0,    0,  452,    0,
    0,    0,    0,  275,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  426,    0,    0,  427,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  645,    0,    0,    0,    0,    0,  645,
    0,    0,  428,  429,    0,    0,    0,    0,    0,  645,
    0,    0,  451,    0,  430,  449,    0,  450,  431,    0,
    0,    0,    0,  645,  645,  645,  645,  645,  645,    0,
  452,    0,    0,    0,    0,  275,    0,  645,    0,    0,
    0,    0,    0,    0,    0,    0,  645,  645,  645,  645,
  645,  645,  645,  645,  645,  645,  645,    0,  645,    0,
    0,  645,  645,    0,  645,  645,  645,  645,  645,  645,
  645,  645,    0,  645,  645,  432,    0,    0,    0,    0,
    0,  433,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  434,    0,    0,    0,    0,    0,    0,    0,  693,
    0,    0,  790,    0,    0,  379,  380,  381,  435,  200,
  201,    0,    0,    0,    0,    0,    0,    0,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,  382,  383,
  437,  438,  439,  384,  385,  440,  441,  103,  442,    0,
  105,    0,    0,  443,  444,    0,  445,  446,  447,  108,
  109,  110,  448,  111,    0,  112,  113,  422,  790,    0,
    0,  790,    0,    0,    0,    0,  423,    0,  790,    0,
    0,  790,    0,    0,    0,  790,    0,    0,    0,  424,
  790,    0,    0,  790,    0,  377,  425,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  426,    0,    0,  427,    0,    0,    0,
    0,    0,    0,  790,    0,    0,  790,  790,    0,    0,
    0,    0,    0,    0,    0,    0,  790,    0,    0,  422,
    0,    0,    0,    0,  428,  429,    0,    0,  423,    0,
    0,    0,    0,  790,  451,  945,  430,  449,    0,  450,
  431,  424,    0,    0,    0,    0,    0,  377,  425,    0,
    0,    0,  452,    0,    0,  790,    0,  275,    0,    0,
  790,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  790,    0,  790,  426,    0,    0,  427,    0,
    0,    0,    0,    0,    0,    0,    0,  701,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,  433,  378,    0,  428,  429,    0,    0,
    0,    0,    0,  434,    0,    0,  451,    0,  430,  449,
    0,  450,  431,    0,    0,    0,    0,  379,  380,  381,
  435,  200,  201,    0,  452,    0,    0,    0,    0,  275,
    0,  436,    0,    0,    0,    0,    0,    0,    0,    0,
  382,  383,  437,  438,  439,  384,  385,  440,  441,  103,
  442,    0,  105,    0,    0,  443,  444,    0,  445,  446,
  447,  108,  109,  110,  448,  111,    0,  112,  113,  432,
    0,    0,    0,    0,    0,  433,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  434,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  379,
  380,  381,  435,  200,  201,    0,    0,    0,    0,    0,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,  382,  383,  437,  438,  439,  384,  385,  440,
  441,  103,  442,    0,  105,    0,    0,  443,  444,    0,
  445,  446,  447,  108,  109,  110,  448,  111,    0,  112,
  113,  422,    0,    0,    0,    0,    0,    0,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  424,    0,    0,    0,    0,    0,  377,
  425,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  426,    0,    0,
  427,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  422,    0,    0,    0,    0,  428,  429,
    0,    0,  423,    0,    0,    0,    0,    0,  451,    0,
  430,  449,    0,  450,  431,  424,    0,    0,    0,    0,
    0,  377,  425,    0,    0,    0,  452,    0,    0,    0,
    0,  275,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  426,
    0,    0,  427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,    0,  433,    0,    0,
  428,  429,    0,    0,    0,    0,    0,  434,    0,    0,
  451,    0,  430,  449,    0,  450,  431,    0,    0,    0,
    0,  379,  380,  381,  435,  200,  201,    0,  452,    0,
    0,    0,    0,  275,    0,  436,    0,    0,    0,    0,
    0,    0,    0,    0,  382,  383,  437,  438,  439,  384,
  385,  440,  441,  103,  442,    0,  105,    0,    0,  443,
  444,    0,  445,  446,  447,  108,  109,  110,  448,  111,
    0,  112,  113,  432,    0,    0,    0,    0,    0,  433,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  434,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1113,    0,    0,  379,  380,  381,  435,  200,  201,    0,
    0,    0,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,    0,    0,    0,    0,  382,  383,  437,  438,
  439,  384,  385,  440,  441,  103,  442,    0,  105,    0,
    0,  443,  444,    0,  445,  446,  447,  108,  109,  110,
  448,  111,    0,  112,  113,  422,    0,    0,    0,    0,
    0,    0,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,    0,    0,  377,  425,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  426,    0,    0,  427,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  422,    0,    0,
    0,    0,  428,  429,    0,    0,  423,    0,    0,    0,
    0,    0,  451,    0,  430,  449,    0,  450,  431,  424,
    0,    0,    0,    0,    0,  377,  425,    0,    0,    0,
  452, 1677,    0,    0,    0,  275,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  426,    0,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0, 1293,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
    0,  433,    0,    0,  428,  429,    0,    0,    0,    0,
    0,  434,    0,    0, 1722,    0,  430,  449,    0,  450,
  431,    0,    0,    0,    0,  379,  380,  381,  435,  200,
  201,    0,  452,    0,    0,    0,    0,  275,    0,  436,
    0,    0,    0,    0,    0,    0,    0,    0,  382,  383,
  437,  438,  439,  384,  385,  440,  441,  103,  442,    0,
  105,    0,    0,  443,  444,    0,  445,  446,  447,  108,
  109,  110,  448,  111,    0,  112,  113,  432,    0,    0,
    0,    0,    0,  433,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  434,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1306,    0,    0,  379,  380,  381,
  435,  200,  201,    0,    0,    0,    0,    0,    0,    0,
    0,  436,    0,    0,    0,    0,    0,    0,    0,    0,
  382,  383,  437,  438,  439,  384,  385,  440,  441,  103,
  442,    0,  105,    0,    0,  443,  444,    0,  445,  446,
  447,  108,  109,  110,  448,  111,    0,  112,  113,  422,
    0,    0,    0,    0,    0,    0,  820,    0,  423,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  424,    0,    0,    0,    0,    0,  377,  425,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  820,    0,    0,
  820,    0,    0,    0,    0,  426,    0,    0,  427,    0,
    0,    0,    0,    0,    0,  820,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  422,    0,    0,    0,    0,  428,  429,    0,    0,
  423,    0,    0,    0,    0,    0,  451,    0,  430,  449,
    0,  450,  431,  424,    0,    0,    0,    0,    0,  377,
  425,    0,    0,    0,  452,    0,    0,    0,    0,  275,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  426,    0,    0,
  427,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,    0,  433,  378,    0,  428,  429,
    0,    0,    0,    0,    0,  434,    0,    0,  949,    0,
  430,  949,    0,  949,  431,    0,    0,    0,    0,  379,
  380,  381,  435,  200,  201,    0,  949,    0,    0,    0,
    0,  949,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,  382,  383,  437,  438,  439,  384,  385,  440,
  441,  103,  442,    0,  105,    0,    0,  443,  444,    0,
  445,  446,  447,  108,  109,  110,  448,  111,    0,  112,
  113,  432,    0,    0,    0,    0,    0,  433,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  434,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  820,    0,
    0,  379,  380,  381,  435,  200,  201,    0,    0,    0,
    0,    0,    0,    0,    0,  436,    0,    0,    0,    0,
    0,    0,    0,    0,  382,  383,  437,  438,  439,  384,
  385,  440,  441,  103,  442,    0,  105,    0,    0,  443,
  444,    0,  445,  446,  447,  108,  109,  110,  448,  111,
    0,  112,  113,  422,  820,    0,  784,  820,    0,    0,
    0,    0,  423,    0,  820,    0,    0,  820,    0,    0,
    0,  820,    0,    0,    0,  424,  820,    0,    0,  820,
    0,  377,  425,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  820,    0,  784,    0,    0,
  784,    0,    0,    0,    0,    0,    0,    0,    0,  426,
    0,    0,  427,    0,    0,  784,    0,    0,    0,  820,
    0,    0,  820,  820,    0,    0,    0,    0,  785,    0,
    0,    0,  820,    0,    0,  949,    0,    0,    0,  786,
  428,  429,    0,    0,  949,    0,    0,    0,    0,  820,
    0,    0,  430,    0,    0,    0,  431,  949,    0,    0,
    0,    0,  819,  949,  949,    0,    0,    0,    0,  785,
    0,  820,  785,    0,    0,    0,  820,    0,    0,    0,
  786,    0,    0,  786,    0,    0,    0,  785,  820,    0,
  820,  949,    0,    0,  949,    0,    0,    0,  786,    0,
    0,    0,    0,  819,    0,    0,  819,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,  433,
    0,  819,  949,  949,    0,    0,    0,    0,    0,  434,
    0,    0,    0,    0,  949,    0,    0,    0,  949,    0,
    0,    0,    0,  379,  380,  381,  435,  200,  201,    0,
    0,    0,    0,    0,    0,    0,    0,  436,    0,    0,
    0,    0,    0,    0,    0,    0,  382,  383,  437,  438,
  439,  384,  385,  440,  441,  103,  442,    0,  105,    0,
    0,  443,  444,    0,  445,  446,  447,  108,  109,  110,
  448,  111,    0,  112,  113,  949,    0,    0,    0,    0,
  813,  949,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  949,    0,    0,    0,    0,    0,    0,  784,    0,
    0,  821,    0,    0,    0,  949,  949,  949,  949,  949,
  949,    0,    0,    0,    0,    0,    0,    0,    0,  949,
    0,  813,    0,    0,  813,    0,    0,    0,  949,  949,
  949,  949,  949,  949,  949,  949,  949,  949,  949,  813,
  949,    0,  821,  949,  949,  821,  949,  949,  949,  949,
  949,  949,  949,  949,  784,  949,  949,  784,    0,    0,
  821,    0,    0,    0,  784,    0,    0,  784,    0,    0,
  785,  784,    0,    0,    0,    0,  784,    0,    0,  784,
    0,  786,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  784,    0,    0,    0,    0,
    0,    0,    0,    0,  819,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  784,
    0,    0,  784,  784,    0,    0,  785,    0,  822,  785,
    0,    0,  784,    0,    0,    0,  785,  786,    0,  785,
  786,    0,    0,  785,    0,    0,    0,  786,  785,  784,
  786,  785,    0,    0,  786,    0,    0,    0,    0,  786,
  819,    0,  786,  819,    0,    0,    0,  785,    0,  822,
  819,  784,  822,  819,    0,    0,  784,  819,  786,    0,
    0,    0,  819,    0,    0,  819,    0,  822,  784,    0,
  784,  785,  810,    0,  785,  785,    0,    0,    0,    0,
    0,  819,  786,    0,  785,  786,  786,    0,    0,    0,
    0,    0,    0,    0,    0,  786,    0,    0,    0,    0,
    0,  785,    0,    0,  814,  819,    0,    0,  819,  819,
    0,    0,  786,  810,    0,    0,  810,    0,  819,    0,
    0,    0,  813,  785,    0,    0,    0,    0,  785,    0,
    0,  810,    0,    0,  786,  819,    0,    0,    0,  786,
  785,    0,  785,  821,    0,  814,  823,    0,  814,    0,
    0,  786,    0,  786,    0,    0,    0,  819,    0,    0,
    0,    0,  819,  814,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  819,    0,  819,    0,  813,    0,
    0,  813,    0,    0,    0,    0,    0,  823,  813,    0,
  823,  813,    0,    0,    0,  813,    0,    0,    0,  821,
  813,    0,  821,  813,    0,  823,    0,    0,    0,  821,
  811,    0,  821,    0,    0,    0,  821,    0,    0,  813,
    0,  821,    0,    0,  821,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  821,    0,  815,  813,    0,    0,  813,  813,    0,    0,
    0,  811,    0,    0,  811,    0,  813,    0,    0,    0,
    0,    0,    0,    0,  821,    0,    0,  821,  821,  811,
  822,    0,    0,  813,    0,    0,    0,  821,    0,    0,
    0,    0,    0,  815,    0,    0,  815,    0,    0,    0,
    0,    0,    0,    0,  821,  813,    0,    0,    0,    0,
  813,  815,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,    0,  813,    0,  821,    0,    0,    0,
    0,  821,    0,    0,    0,    0,  822,    0,    0,  822,
    0,    0,    0,  821,  810,  821,  822,    0,    0,  822,
    0,    0,    0,  822,    0,    0,    0,    0,  822,    0,
    0,  822,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  814,  822,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  810,  822,    0,  810,  822,  822,    0,    0,    0,    0,
  810,    0,    0,  810,  822,    0,    0,  810,  823,    0,
    0,    0,  810,    0,    0,  810,    0,    0,    0,    0,
    0,  822,  814,    0,    0,  814,    0,    0,    0,    0,
    0,  810,  814,    0,    0,  814,    0,    0,    0,  814,
    0,    0,    0,  822,  814,    0,    0,  814,  822,    0,
    0,    0,    0,    0,    0,  810,    0,    0,  810,  810,
  822,    0,  822,  814,  823,    0,    0,  823,  810,    0,
    0,    0,  811,    0,  823,    0,    0,  823,    0,    0,
    0,  823,    0,    0,    0,  810,  823,  814,    0,  823,
  814,  814,    0,    0,    0,    0,    0,    0,    0,    0,
  814,    0,    0,    0,  815,  823,    0,  810,    0,    0,
    0,    0,  810,    0,    0,    0,    0,  814,    0,    0,
    0,    0,    0,    0,  810,    0,  810,    0,  811,  823,
    0,  811,  823,  823,    0,    0,    0,    0,  811,  814,
    0,  811,  823,    0,  814,  811,    0,    0,    0,    0,
  811,    0,    0,  811,    0,    0,  814,    0,  814,  823,
  815,    0,    0,  815,    0,    0,    0,    0,    0,  811,
  815,    0,  321,  815,    0,    0,    0,  815,    0,    0,
    0,  823,  815,    0,    0,  815,  823,    0,    0,    0,
    0,    0,    0,  811,    0,    0,  811,  811,  823,    0,
  823,  815,  321,    0,    0, 1387,  811,    0,    0,  321,
    0,    0,    0,  321,  321,    0,  321,    0,    0,  321,
    0,    0,    0,  811,    0,  815,    0,    0,  815,  815,
    0,    0,    0,  321,    0,    6,    0,    0,  815,  321,
    0,    0,    0,    0,    0,  811, 1506, 1507,    0, 1508,
  811,    0, 1509,    0,    0,  815,    0,    0,    0,    0,
    0, 1387,  811,    0,  811,    0, 1510,    0,    0,    0,
    0,    0, 1511,    0,    0,    0,    0,  815,    0,    0,
    0,    0,  815,    0,    0,    0,    0,    0,    0,    0,
    0,    6,    0,    0,  815,    0,  815,    0,    0,  321,
    0,    0, 1506, 1507,    0, 1508,    0,    0, 1509,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  321,    0,
    0,    0, 1510,    0,    0,    0,    0,    0, 1511,    0,
    0,    0, 1512,  321,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  321,    0,    0,    0,    0,    0,    0,
    0,   14,  321,    0,  321,    0,    0,    0,    0,    0,
    0,  321,    0,    0,    0,    0, 1513,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,    0,    0,    0, 1638,    0, 1514, 1512,    0,
  321,  321,    0,  321,  102,    0,    0,    0,    0,    0,
  321,  321,  321,  321,  321,    0,  321,   14,  321,  321,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1513, 1515,  104,    0,  105,    0,    0,    0,
    0,    0,   16,  106,  107,  108,  109,  110,    0,  111,
  577,  112, 1516, 1514,  578,    0,    0,    0,    0,    0,
  102,  774,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  579,    0,    0,  580,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1515,
  104,    0,  105,    0,    0,    0,    0,    0,    0,  106,
  107,  108,  109,  110,    0,  111,    0,  112, 1516,    0,
    0,    0,    0,    0,  577,    0,  581,    0,  578,    0,
    0,    0,  582,  583,  584,    0,  585,  586,  587,    0,
  588,    0,    0,    0,    0,    0,  579,    0,    0,  580,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  589,    0,  590,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  581,    0,    0,  591,    0,    0,  582,  583,  584,    0,
  585,  586,  587,    0,  588,    0,    0,    0,    0,    0,
  592,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  589,    0,  590,    0,    0,    0,
    0,    0,  593,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  102,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  591,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  594,  595,    0,
    0,    0,    0,    0,  592,    0,  103,  104,    0,  105,
    0,    0,    0,    0,  596,    0,  106,  107,  108,  109,
  110,    0,  111,    0,  112,  113,  593,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  102,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  594,  595,    0,    0,    0,    0,    0,    0,    0,
  103,  104,    0,  105,    0,    0,    0,    0,  596,    0,
  106,  107,  108,  109,  110,  102,  111,    0,  112,  113,
    0,    0,    0,    0,  155,  156,  157,  158,  159,  160,
  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,
    0,    0,  171,  172,  103,  104,    0,  105,  173,  174,
  175,  176,    0,    0,  106,  107,  108,  109,  110,  177,
  111,  178,  112,  113,  179,  180,  181,  464,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  464,  464,  464,
  464,  464,  464,  464,  464,  464,  464,  464,  464,  464,
  464,  464,    0,    0,  464,  464,  464,  464,    0,  464,
  464,  464,  464,  464,    0,    0,  464,  464,  464,  464,
  464,  464,  464,  464,  464,  464,  464,  464,  464,
};
static short yycheck[] = {                                       9,
   16,   52,   12,   61,   60,    4,   16,  101,   18,    0,
    4,  252,  132,  133,   60,  132,  133,  143,  504,  698,
  351,   53,   59,  355,   82,  537,   65,  326,  540,  324,
  355,   91,   64,  347,  546,   86,   70,  546,  715,  509,
   50,   51,   52,   53,   54,   55, 1014,  487, 1185,   88,
   60,   61,   62,  684,   64,   87,   66,   67,   92,   69,
   70,  738,   94,  751,   80,   85,  252,  123,   78,  125,
   80,   81,   82,   89,   84,   85,   86,   87,  260,   89,
   90,   91,   92,   67,   94, 1007,  146,  269,  410,  674,
  372,  678, 1014, 1007,  635, 1228, 1007,  283, 1007,  640,
 1014,  900,  745, 1014,  809, 1014,   90,  360,  629,   45,
 1015,  742,  144,  123,  124,  125,  701,  137,  319,    0,
    0, 1026,    0,    0,    0,    0,  303,  137,  771,  840,
  224,   44,  252,  143,  144,  145,  146,  780,   54,  622,
 1045,   41,  959,  942,  252,  714,  387,    0,  185, 1062,
   66, 1039,  693,   45,   41,   44,   40,   40,   44,  748,
   41,   59,   45,   44,   41,   41,    0,   44,   44,    0,
   44,    0,    0,  210,    0,    0,    0,  303,   59,   59,
   44,   59,   59,   59,   59,  516,   45,    0,   41,   45,
  833,   44,   44,    0,   40,   44,   63,    0,  254,   40,
   40,  387,   41,    0, 1140, 1226,   59,    0,  124,  523,
  524,  525,  526,   61,  546,   41,  310,   41,   44,  286,
  402,  546,   63,   63,   41,   59,   59,   44,   59,  145,
   59,   59,  718,   59,   59,   59,  287,    0,   44,  262,
    0,  572,    0, 1371,  254,    0,   59,  303,  258,   41,
  260,  261,   59,  506,  549,    0,   59,  267,  260,  269,
   40, 1427,   59, 1332, 1063,  274,   59,  387,  268,  325,
  901,   41,   44, 1133,   44, 1135,  332,  287,  334,  387,
  336,  291,  152,  339,  410,  797, 1254,  301,  356,   59,
  257,   41, 1361,    0,   44,  577,   59, 1144,  267,   59,
   41,   59,   12,  598,   59,  260,  893,  283, 1658,    0,
   91,  348,   12,  350,   59,  325,   44,  368,  905,  906,
  301,  277,  332,   44,  334,  357,  336,  364, 1541,  339,
  387,    0, 1254,  257,  875,  876,   16,  980, 1243,    0,
 1254,   45,  258, 1254,  455, 1254,    0,  357,  358,  402,
   41,  267,   59,    0,  410, 1705,  366,  338,  368,   63,
  445,  429,  262,  386,    0,  365,  273,  334,   59,  401,
 1187, 1392, 1260,  320,  358,  291, 1276,  817,  372,  301,
    0,    0, 1025,  440,  416,  438,    0,    0,  398,  345,
   59,  401,  402, 1606,  461,  462,  937,    0,   59,  409,
   80, 1751,  301,  413,  215,   59,  416,    0,  418,    0,
  334,  421,   59,  402, 1547,  417,  338,   44,  304,  660,
  130,   41,  507,   59,  258,  278, 1273,  280,  442,  258,
  130,  318,    0,  340,  287, 1785, 1141,  318,   41,   59,
   59,   44,  452,  313,   42,   59,   59,  564,   41,  418,
  366,   44,  300,   41,  416,  304,   59,  466,  264,  340,
  340,  442,  417,  439,  759, 1006,   59,  762,   59,    0,
  309,  264,  306,    0,  660, 1335,  486,  306, 1067, 1384,
 1608,  776,  398, 1388,  521, 1390,  386,  669, 1654,  269,
  546,   59,  318,  409,  318,  306,  277,  413,  684, 1178,
  328,  264,  418,  331,  264,  331, 1185,  383,  803,  340,
  394, 1424,  410, 1646,  340,  328,  340, 1205,  331,  264,
 1425, 1090,  458,  436,  274,  378,  565,    0,   59, 1465,
  567, 1431,   59,  719,  331,  545,  546,  307,  308,  431,
  660,  551, 1679,  442, 1249,  844,  318,  400,  431,  485,
 1438, 1072,  660, 1370,  407, 1266,  742,  758,  436,  309,
  367,  387,  739,  387, 1151,  621,  458, 1700,   41,  458,
  396,  853,  396, 1056,  454,  458,  898,  410,  831,  454,
  454,  618,  619,  577,  418, 1722,   59,  342,  441,  418,
  454,  458,  320, 1498,  411,  387, 1665,    0, 1503,  458,
  452,  447,  458,  264,  397,  661,  409,  724, 1741,  367,
  264,  621,  440,  739,  440,  440,  440,  458,  458,    0,
  630,  502, 1265,  454,  501,  501,  454,  440,  454,  454,
  454,  630, 1525,  340,  397,  264,  630,  754,  755,  452,
  410,  454, 1057,  440, 1443,  264,  387, 1552,  501,  681,
  405,  661,  397,  408,  664,  396,   59,  454,   41,  669,
   41,  411,  383,   44, 1569,  278,  686,  280,  678,  424,
 1211,  681,    0,  264,  262,  278,  686,  280,   59,  348,
  994,  451,  318,  739,  287,  278,  783,  280,  744,   41,
 1231, 1379,  748,  320,  287,  450,  387,  707,  296,   41,
  882,  348, 1333,  885,  340,  715, 1674,  717, 1293,    0,
  720,  331,  768,  750,  751,  901,  466, 1012,  757,  681,
    0,  377,   41,  733, 1371,  735,  340,  431,  738, 1024,
  678,   59,  543,  264,  744,    0,  397,    0,  748,   41,
  796,  797,  319,  397, 1058,  273, 1371, 1371, 1653, 1044,
   41,   41, 1674,   44,  458,  258,  795, 1662,  768, 1229,
 1674,  262,  320, 1674,   44, 1674, 1007,  387,   59,   44,
   42, 1070,  898, 1014,  342,  378,  396,   59,  397,   59,
  383,  355,  411, 1198,   59,  378,  796,  797,  827,  828,
  829,  318,  448,  409,   59,    0,   59,  400,  386, 1704,
 1479,  612, 1168,  306,  407, 1171,  397,  400,  802,  377,
  343, 1221, 1252,  340,  407,  338,  287,  733, 1258,  735,
  814, 1007,  832,  400,  313,   45,  886,    0, 1014,  300,
 1339, 1372,  842, 1712, 1713,  368,   41,  405,  441,   44,
  408,  234,  898,  283,  854,  318, 1739,  278,  441,  242,
  262,  883,    0,  887,   59, 1154,  424, 1267,  331,  853,
  387,  264,  460,   42,  441,    0,  397,  340,   47,  396,
    0,   58,  882,  883,  884,  885,  886,  887,  436,  262,
 1773, 1774,  450,  893,  348,  386,   59,   40,  319,  307,
  308,  489,  490,  426,  492,  905,  906,    0,  501,  261,
  884,  499,  500,  501,  502,  503, 1799,  505,  501,  507,
  508,   59,  458,  440,  387,  418,  832, 1810,  492,  442,
 1813, 1018, 1019,  396,   59, 1042,  842, 1806,  417,   59,
 1607,  259,  348,  405,  435,  325,  264,  318,   41, 1270,
  321,  357,  270,  262,  384,  893, 1241,  328,   40,  959,
  331,   43,  424,   45,  335,  265,   59,  905, 1144,  340,
  262, 1608,  343,    0, 1374,    0,   58,  440,  377,  400,
 1067,   63,  262,  264,  386,  405,  328, 1272,  359,  331,
 1466,  454,   40, 1608, 1608,  307,  308, 1353,  260,  331,
 1405, 1357,  302, 1505,  397, 1481,  458,  461,  462,  439,
 1679,  264,  383,  386, 1060,  322,  387, 1339, 1423,  418,
  441, 1067, 1051, 1254, 1339,  396,  456,    0,  258, 1139,
 1140, 1141,   59,  435,   59, 1035,  456,  337, 1380,  320,
 1069,  303,  413,  451, 1600,  387, 1075, 1036, 1604, 1049,
  430,  397, 1036, 1722,  396,  387, 1656,  437,  328,  322,
 1060,  331,   40,  401,  396,  263,  465, 1067,   41,  440,
 1186,   44, 1376,  343, 1605,  301,  306,  386, 1254,  397,
   58,  452,  272,  278, 1426,  280,   59,  342,   61,  460,
  436,   40,  287,  403,  386, 1571, 1627, 1273,  288, 1630,
 1207,  383, 1209,    0,    0, 1661,  386, 1707,    0, 1394,
  420,  280,  338,   44,  312,  278,  285,  280,  489,  490,
 1476,  492,  377,  402,  470,  323,   44,  417,  499,  500,
  501,  502,  503, 1133,  505, 1135,  507,  508, 1167,  451,
  337,  284,    0,  333,  397,   41,  405,   44, 1433,   44,
  405, 1151, 1752,  408,  444, 1139, 1140, 1333,  278, 1559,
  280, 1268,   59,   59,   59, 1565,  445,   59,  383,  424,
  440,  452,  362,   41,   46,   93,   44,  374, 1205,  460,
  318,  407,  452,  378,  454,  267, 1186, 1187,  418,   61,
  387,  417,    0,  337,  276,  450,  385,  456,    0, 1755,
    0,   59,  340,  279,  260,  400,  436,  289,  489,  490,
  292,  492,  407,  295,  296,  378,  442,  300,  499,  500,
  501,  502,  503,  378,  505,   93,  507,  508,  507,  427,
  374,  265,  314, 1682, 1666, 1711, 1668,  383,   59,  429,
  470,  323,   44,  387,  326,  400,  441,  303,  458,  387,
  460,   59,  407,  329,  259, 1371, 1283,   59, 1247,   59,
 1287,   42,   43, 1247,   45,  270,   47,  383,  302,  352,
    0, 1387,  354,  355, 1805,    0, 1725, 1709,   59,  489,
  490, 1566,  492,  460,  366,  258,  441,  363,  370,  499,
  500,  501,  502,  503,  387,  505,  378,  507,  508,  260,
  274,  274,  440,  396,  278,  381,  501,    0,    0,  283,
    0,   41,  489,  490,   44,  492,   41,  460, 1767,   44,
  260, 1770,  499,  500,  501,  502,  503,  300,  505,   59,
  507,  508,  259,  306,   59, 1335,  309,  436,  312,  421,
 1386, 1387,  303,  270, 1793,  427,  489,  490,   41,  492,
 1386,  433, 1379,  494, 1786, 1465,  499,  500,  501,  502,
  503,  443,  505,  303,  507,  508,   59,   59,   46,   59,
 1370, 1756, 1757,  321,    0,  457,  458,  459,  460,  461,
  462,    0,   41,   61,  337,   44, 1386,  335,    0,  471,
    0,   42,   43, 1500,   45, 1395,   47,    0,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490, 1525,
  492,  359,  460,  495,  496,   41,  498,  499,  500,  501,
  502,  503,  504,  505,    0,  507,  508,  509,  510,  511,
 1815,  328,  328,   59,  331,  331,    0,    0,  411,  418,
   59,  489,  490, 1674,  492,  418,  343,   59,  321,   59,
  342,  499,  500,  501,  502,  503,   59,  505, 1506,  507,
  508,    0,  335,  436,  321,  413, 1550,  458,  280,   44,
  343,  445,  342,  285,  493,  449,  450,  451,  335, 1525,
    0, 1465,  460,   59,  503,  377,  359,   41, 1472, 1395,
   44,  387, 1608,  466,  485,   59,   59,  470, 1674,  280,
  396,    0,  359,    0,  285,   41, 1506,  377,   44,    0,
 1551,  489,  490,  405,  492,  274,  408,    0,   93, 1635,
   59,  499,  500,  501,  502,  503,  383,  505, 1509,  507,
  508,    0,  424,    0,  342,  405,    0,    0,  408,   59,
  413, 1541,    0,  440,   42,   43, 1546,   45, 1575,   47,
   44, 1551, 1579,  321,  424,  452,  413,  454,  450,  340,
   59,   59,   59, 1647, 1570,  260, 1682,  335,   59,  377,
 1570,    0,    0,    0,   41,  343,   59,   44,    0,    0,
  450,  318,   40,   41,   42,   43,   44,   45,   46,   47,
   59,  359,   59,  318,   61,   59,   59,  405, 1620,   93,
  408,   59,   60,   61,   62,   40, 1606, 1607,  303, 1725,
  321,    0,   44,  424,   41,  340,  424,   44, 1734,   41,
 1620,  260,   44, 1739,  335,  318,  417,  402,   40,  280,
   59,   59,   59,   91,  285,   93,    0,   59,   59,  460,
 1546,   42,  450, 1691,    0,  413,    0,  340,  359,  352,
 1656, 1767,    0,  444, 1770,  461, 1656, 1773, 1774,  434,
   41,   93,   41,   44,  303,   44,  640, 1673,  489,  490,
   59,  492,  383, 1673,  449,  378,  264, 1793,  499,  500,
  501,  502,  503, 1799,  505,    0,  507,  508,  294,  340,
  318, 1691, 1692, 1739, 1810,   59,  465, 1813,  467,  305,
  316, 1707,  413,   59, 1693,   59,  312, 1707, 1714, 1693,
  684,   59,  356,  687, 1714,  337,   41,    0,  692,   44,
  316,  317, 1763,  697,  698,  460,   41, 1773, 1774,   44,
   41,  337,  706,   44,  708,  709,  710,  711,  712, 1133,
  714, 1135,    0,  336,   59,  719, 1752,  258,  452,  393,
  261,  395, 1752, 1799, 1760,  855,    0,  857,   41,    0,
 1760,   44,   41, 1763, 1810,   44,  402, 1813,  742,   41,
  493,  494,   44,  747,  418,  340,   59,   41,   61,   40,
   44,   41,  280,   41,   44,   41,   44,  285,   44,   41,
 1790,  258,   44,  461,  462,  306,  402,   41, 1798,  443,
   41,   59,   41,   44,  262,   44,  264,  265,  320,   41,
  277,  278,   44,  280,  272,   59, 1816,  284,   59,  402,
  287,   93,  280,  457,  458,  459,   41,  285,  434,   44,
  288,   41,  438,  300,   44,    0,  391,  392,  318,  306,
   42,   43,  383,   45,  302,   47,  461,  462,  383,  307,
  308,  485,  486,  311,   41,  436,  278,   44,  280,  506,
  318,  361,  320,  321,  450,  287,  324,  434,  417,  442,
  328,  357,  287,  331,  411,  333,   41,  335,  300,   44,
  469,   44,  340,  341,   61,  343,  860,  861,  862,  286,
  372,  865,  866,  867,   59,  869,  870,  871,  872,  873,
  874,  359,  360,   40,  362,  264,   40,  436,  461,    0,
  461,  378,   42,   43,  461,   45,   44,   47,  376,   44,
  378,  379,  380,  402,  320,  383,   40,  901,  386,  387,
   40,   40,   40,  400,  908,  909,   40,  911,  396,  913,
  407,    0,  916,  917,  918,  919,  920,  921,   40,  416,
   41,  418,  410,   40,   40,  413,  378,   40,   40,  933,
   40,  935,   40,  278,   40,  280,  428,    0,   59,  436,
   91,  429,  287,   46,  441,  258,  320,  435,  400,   44,
   44,  348,  440,   46,   61,  407,   40,   42,   43,  264,
   45,   44,   47,  451,  452,  278,  454,  280,  382,   44,
   59,  284,  460,  470,  287,  278,  300,  278,   41,   42,
   43,   44,   45,   46,   47,  264,  436,  300,  470,  441,
  278,  397,  280,  306,  397,  460,   59,   60,   61,   62,
  277,  489,  490,  458,  492,   40,  447,  278,  447,  280,
  349,  499,  500,  501,  502,  503,  287,  505,  399,  507,
  508,   40,   91,   40,  489,  490,   40,  492,   91,  460,
   93,  318,  383,  378,  499,  500,  501,  502,  503,  264,
  505,  411,  507,  508,  318,   41,   42,   43,  280,   45,
   44,   47,  389,  285,  328,  400,    0,  331,  489,  490,
  382,  492,  407,   40,   40,  378,  340,  436,  499,  500,
  501,  502,  503,  464,  505,  317,  507,  508,   40,  320,
    0,  320,   41,  278,  451,  280, 1090,  400,  320,  452,
  378,  448,  264,  454,  407, 1635,  441,   41, 1102, 1103,
   44, 1105, 1106,   42,   43,  418,   45,  378,   47, 1113,
  502,  458,  400,  387,   40,   59,  277,   61,  274,  407,
  280,   41,  396,  436,   44,  285,   40,  300,  441,  400,
  284,  344, 1371,    0,    0,  344,  407,  278,  386,   59,
 1144,   61, 1682,  683,   40,  685,  418,   91,  688,  689,
  690,  386,  386,  441, 1158,  416,  356,  470,  300,  418,
  320,  417, 1166,   45, 1168,   40,  440, 1171,  417, 1173,
  441,   40,    0, 1177, 1178, 1179, 1180,  274,   44,   44,
  454, 1185,  865,  378,  867, 1725,  869,  870,  385,  872,
  873,  874,   59,   59, 1734,  280,  367,  318,   44,  262,
  285,  264,  265,  264,   41,  400,  320,  328,   44,  272,
  331,  320,  407,   41,   44,  280,   44,  280,   40,  340,
 1224,   44,  285,  328,   44,  288,  311, 1767,  266,  318,
 1770,   59,  505,   42,   43,   40,   45,  299,   47,  302,
   40,  304,  331,  279,  307,  308,  441,  501,  311,  264,
   41,  340,  422, 1793,   58,  318,   41,  320,  321,   41,
   41,  324,  264,  454,  300,  328,  387,  394,  331, 1273,
  333,  440,  335,  492,   41,  396,  309,  340,  341,  309,
  343,   41,   40,   40,  280,  352, 1290,   42,   43,  285,
   45,   40,   47,  329,   40, 1299,  359,  360,  387,  362,
   40,   40, 1306,   40,   40,   40,   40,  396,   40,   40,
   40,    0,  348,  376,  258,  378,  379,  380,   40,  440,
  383,  357,   40,  386,  387,   40,  265,  363,  262, 1333,
  274,  452,  327,  396,  278,   44,  280,  435,  258,  308,
  284,  280,   41,  287,  388,  381,  285,  410,  388, 1353,
  413,  440,   41, 1357,  388,   44,  300,  277,  278,  343,
  280,    0,  306,  302,  284,  309,  429,  287,  274, 1608,
   59,  331,  435,  422,  390,  446,  271,  440,   42,   43,
  300,   45,  391,   47,  287,  407,  306,  386,  451,  452,
   41,  454,  378,  386,  386,  386, 1635,  460,  386,  386,
   41,  278,   41,   42,   43,   44,   45,   46,   47,   40,
  417,   41,   41,  295,   41,  385,  436,   41,  308,  280,
   59,   60,   61,   62,  435,  396,  489,  490,  337,  492,
    0,  338,  442,  301,  378,   41,  499,  500,  501,  502,
  503,  323,  505, 1682,  507,  508,  338, 1451, 1452,  301,
   41,  318,   91,  442,   93,  321,  400,  320,  378,  352,
  385,  328,  328,  407,  331,  331,  371,  411,   41,  335,
  352,  280, 1476,  340,  418, 1479,  285,  343,   41,   41,
  400,   41,  371,   41,   41,   41, 1725,  407,   41,   59,
  318,  260,  436,  359,   41, 1734,    0,  441,  418,   41,
  328,   41,  311,  331,   41,   41,   41,  262, 1512,   41,
   41,   59,  340,   46,  280,  343,  436,  383,   44,   40,
  387,  441,  466,  340,   40,  280,  470,  340, 1767,  396,
  285, 1770,  301,  308,   59,   59,   59,   41,   42,   43,
   44,   45,   46,   47,  313,  427,   59,  413,   59,   59,
  470,   59,  418,   41, 1793,   59,   60,   61,   62,  387,
  352,  443,   42,   43,   44,   45,  344,   47,  396,  338,
 1574,   41,  368,  440,  440,  457,  458,  459, 1582,  461,
  462,   40,  383,   44,  340,   59,  452,   91,  454,   93,
   59,   41,   42,   43,  451,   45,  383,   47,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  383,   41,   41,
  343,    0,  440,  495,  496, 1619,  280,   41,   44, 1623,
  410,  285,  504,  262,  452,  264,  265,   42,   43,  318,
   45,   44,   47,  272,  304,  301,  435,  387,  407,  328,
  442,  280,  331,   40,  293,   59,  285,  304,  417,  288,
   44,  340,   41,   40,  343,   44,  463,  307,   45,   44,
   41,    0,  337,  302,  383,   40,  383,  264,  307,  308,
   59,  278,  311,  442,  264, 1679,   63, 1681,  458,  318,
 1684,  320,  321,  389,  320,  324,  264,  470,  411,  328,
  451,   40,  331,  309,  333,  458,  335,  271,  387,  264,
  343,  340,  341,  343,  343,  343,  435,  396,  436,  279,
  343,  458,  271,   41,   42,   43,  308,   45, 1722,   47,
  359,  360,   41,  362,  308,   41,   42,   43,   40,   45,
  300,   47,   40,   59,   59,   41,  308,  376,    0,  378,
  379,  380,   59,  304,  383,   59,   41,  386,  387,   59,
   44,  440,   41,    0,    0,    0, 1136,  396,  262,  329,
  264,  265,   12,  452,   42,   43,   44,   45,  272,   47,
  269,  410,  262, 1072,  413,  261,  280,  625,  348,   41,
   84,  285,   44,  669,  288,  845,  356,  357, 1413, 1070,
  429, 1067, 1059,  363, 1414,  322,  435,   59,  302,  572,
  280,  440,  352,  307,  308,  285, 1049,  311,  317,  997,
  308,  381,  451,  452,  318,  454,  320,  321, 1392,  334,
  324,  460,  744,  393,  328,  395, 1617,  331,  334,  333,
  280,  335, 1060,  778, 1698,  285,  340,  341, 1394,  343,
   41,   42,   43,  551,   45, 1702,   47,  262,  418, 1701,
  489,  490,    0,  492, 1743,  359,  360,  768,  362, 1625,
  499,  500,  501,  502,  503,  280,  505, 1798,  507,  508,
  285, 1640,  376,  443,  378,  379,  380, 1740, 1222,  383,
  387,  982,  386,  387,   41,   42,   43,  339,   45, 1200,
   47, 1375,  396,   41,   42,   43,   44,   45,   46,   47,
 1656,  553,  327,  320, 1256,  824,  410,  611,  295,  413,
   13,   59,   60,   61,   62,  816,  421,  295,   41,   42,
   43,    0,   45,  489,   47,  429,  493,  733,  747,  135,
  719,  435,  321,  959,  720, 1608,  440, 1760, 1496,  328,
 1714, 1816,  331,   91,  902,   93,  335,  451,  452,  504,
  454,  677,  280, 1173,  343, 1104,  460,  285,  386,   -1,
   -1,   -1,   -1,   -1,  280,  352,   -1,   -1,   -1,  285,
  359,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   59,  264,   -1,   -1,   -1,  489,  490,    0,  492,   -1,
   -1,   -1,   -1,   -1,  383,  499,  500,  501,  502,  503,
   -1,  505,  280,  507,  508,   -1,   -1,  285,  472,  473,
  474,  475,  476,  477,  478,  479,   -1,   -1,   45,   41,
   42,   43,   -1,   45,  413,   47,   -1,  294,   41,   42,
   43,   44,   45,   -1,   47,   -1,   63,   -1,  305,   -1,
   -1,   -1,   -1,   -1,   -1,  312,   59,   60,   61,   62,
   -1,  440,   -1,   -1,   -1,   -1,  318,   -1,  325,  321,
   -1,   -1,   -1,  452,   -1,  454,  328,   -1,   -1,  331,
  457,  458,  459,  335,  461,  462,   -1,   -1,  340,  280,
   93,  343,   -1,   -1,  285,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  480,  481,   -1,   -1,  359,  485,  486,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,  386,
   -1,  383,   -1,  280,  272,  387,   -1,   -1,  285,   -1,
   -1,   -1,  280,   -1,  396,  402,   -1,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,    0,   -1,   -1,   59,   -1,
   -1,  413,  419,   -1,  302,   -1,   -1,  280,   -1,  307,
  308,   -1,  285,  311,   -1,   -1,   -1,  434,   -1,   -1,
  318,  438,  320,  321,  441,   -1,  324,  460,  440,  258,
  328,   -1,  449,  331,   -1,  333,   -1,  335,   -1,   -1,
  452,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
  279,  468,   -1,   -1,   59,    0,  489,  490,   -1,  492,
  280,  359,  360,   -1,  362,  285,  499,  500,  501,  502,
  503,  300,  505,   -1,  507,  508,   -1,  306,  376,    0,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   41,   42,   43,   -1,   45,   -1,   47,  396,  262,
  329,  264,  265,   -1,   -1,    0,   -1,   -1,  280,  272,
   -1,   -1,  410,  285,   59,  413,   -1,  280,  295,  348,
   41,   -1,  285,   44,   -1,  288,   -1,  356,  357,   -1,
   -1,  429,   -1,   -1,  363,   -1,   -1,  435,   59,  302,
   -1,   -1,  440,   -1,  307,  308,   41,   -1,  311,   44,
   -1,   -1,  381,  451,  452,  318,  454,  320,  321,   -1,
   -1,  324,  460,   -1,   59,  328,   -1,    0,  331,   -1,
  333,   -1,  335,   -1,   -1,  352,   -1,  340,  341,   -1,
  343,   41,   42,   43,   -1,   45,   -1,   47,   -1,  418,
  280,  489,  490,    0,  492,  285,  359,  360,   -1,  362,
  429,  499,  500,  501,  502,  503,   -1,  505,   41,  507,
  508,   44,   -1,  376,   -1,  378,  379,  380,  279,   -1,
  383,   -1,   -1,  386,  387,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   41,   42,   43,   44,   45,  300,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  410,    0,   -1,
  413,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   41,
   42,   43,    0,   45,   -1,   47,  429,   -1,  329,   -1,
   -1,   -1,  435,   -1,  279,   -1,   -1,  440,   -1,   -1,
  457,  458,  459,   -1,  461,  462,   93,  348,  451,  452,
   -1,  454,   -1,   -1,   -1,  300,  357,  460,   -1,   -1,
   -1,   -1,  363,  480,  481,   -1,   -1,   59,  485,  486,
   -1,   -1,   -1,  258,   -1,   -1,   -1,   -1,   -1,   -1,
  381,   59,   -1,   -1,  329,   -1,  489,  490,    0,  492,
  280,   -1,   -1,   -1,  279,  285,  499,  500,  501,  502,
  503,   -1,  505,  348,  507,  508,   -1,   -1,   -1,   -1,
   -1,   -1,  357,   -1,    0,  300,   -1,   -1,  363,   -1,
   -1,  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   -1,   47,  381,   -1,   -1,   -1,
   -1,   -1,   -1,  278,  329,  280,   -1,   59,   60,   61,
   62,   -1,  287,   -1,   -1,   41,   -1,  318,   44,   -1,
  321,   -1,   -1,  348,   -1,  300,   -1,  328,   -1,   -1,
  331,   -1,  357,   59,  335,   -1,   -1,   -1,  363,  340,
  280,   93,  343,   -1,   -1,  285,   41,   42,   43,   -1,
   45,   -1,   47,   41,   42,   43,  381,   45,  359,   47,
   -1,   -1,   41,   42,   43,  278,   45,  280,   47,   -1,
   -1,  284,   -1,   -1,  287,  262,   -1,  264,  265,   -1,
   -1,   -1,  383,   -1,   -1,  272,  387,  300,   -1,   -1,
   -1,   -1,   -1,  418,   -1,  396,   -1,   -1,  285,   -1,
   -1,  288,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,  302,   -1,   -1,  280,   -1,
  307,  308,   -1,  285,  311,  400,  258,   -1,   -1,   -1,
   -1,  318,  407,  320,  321,   -1,   -1,  324,   -1,  440,
  258,  328,   -1,   -1,  331,   -1,  333,  279,  335,   -1,
   -1,  452,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,  279,   -1,    0,   -1,  378,  441,   -1,  300,   -1,
   -1,   -1,  359,  360,  306,  362,   -1,   -1,   -1,   41,
   42,   43,  300,   45,   -1,   47,   -1,  400,  306,  376,
   -1,  378,  379,  380,  407,   -1,  383,  329,   -1,  386,
  387,   -1,   -1,    0,   41,   -1,   -1,   44,   -1,  396,
  262,  329,  264,  265,  287,   -1,  348,   -1,   -1,   -1,
  272,   -1,   59,  410,   -1,  357,  413,   -1,  441,   -1,
  348,  363,   -1,   -1,   -1,   -1,  288,   -1,   -1,  357,
   -1,   -1,  429,   -1,   41,  363,   -1,   44,  435,  381,
  302,   -1,  278,  440,  280,  307,  308,   -1,  284,  311,
   -1,  287,   59,  381,  451,  452,  318,  454,  320,  321,
   -1,   -1,  324,  460,  300,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,  280,  418,   -1,  340,  341,
  285,  343,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,
  418,  280,  489,  490,    0,  492,  285,  359,  360,   -1,
  362,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   41,   42,   43,   44,   45,
   -1,   47,  378,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,  305,  400,   -1,   -1,  429,   -1,   -1,
  312,  407,  287,  435,  316,  317,   -1,  460,  440,   -1,
   -1,   -1,   -1,  325,   -1,   -1,   -1,   93,   -1,  451,
  452,   -1,  454,   -1,   -1,  337,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  441,  489,  490,  280,  492,
   -1,  258,   -1,  285,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  507,  508,   -1,  489,  490,    0,
  492,   -1,  279,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  507,  508,   -1,   -1,   -1,
   -1,   -1,   -1,  300,   -1,  262,   -1,   -1,   -1,  306,
  402,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   43,   44,   45,   -1,   -1,  419,   -1,  260,
   -1,   -1,  329,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,  434,   -1,   -1,   -1,  438,   -1,   -1,   -1,
   -1,  348,   -1,   -1,   -1,   -1,   -1,  449,   -1,   -1,
  357,  318,   -1,   -1,  321,   -1,  363,   -1,   -1,   -1,
  301,  328,   93,   -1,  331,   -1,  468,   -1,  335,   -1,
   -1,   -1,  313,  340,  381,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,  327,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  262,  338,  264,  265,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,
   -1,  418,   -1,   -1,  489,  490,  383,  492,   -1,  386,
  387,   -1,  288,   -1,  499,  500,  501,  502,  503,  396,
  505,   -1,  507,  508,   -1,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   -1,   -1,  311,  413,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,   -1,  328,   -1,   -1,  331,  407,  333,  435,  335,
   -1,   -1,   -1,  440,  340,  341,  417,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,  460,
  386,  387,   -1,   -1,    0,   -1,   -1,   -1,  328,   -1,
  396,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,  410,   -1,   -1,  413,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,  288,  499,  500,
  501,  502,  503,  429,  505,   41,  507,  508,   44,  435,
   -1,  302,   -1,   -1,  440,   -1,  307,  308,  343,   -1,
  311,   -1,   -1,   59,   -1,  451,  452,  318,  454,  320,
  321,   -1,   -1,  324,  460,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,  368,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,    0,  492,   -1,  359,  360,
   -1,  362,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,  507,  508,  443,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
  460,  426,   -1,   -1,   -1,  396,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,  413,   -1,   59,   60,   61,   62,   -1,  489,
  490,   -1,  492,   -1,   -1,  460,   -1,   -1,  429,  499,
  500,  501,  502,  503,  435,  505,   -1,  507,  508,  440,
   -1,   -1,  261,   -1,   -1,   -1,   -1,   -1,   93,   -1,
  451,  452,   -1,  454,  489,  490,   -1,  492,   -1,  460,
   -1,   -1,   -1,  282,  499,  500,  501,  502,  503,   -1,
  505,  290,  507,  508,   -1,   -1,   -1,   -1,  297,   -1,
  299,  460,  301,   -1,   -1,   -1,   -1,  306,  489,  490,
    0,  492,   -1,   -1,  313,   -1,   -1,  443,  499,  500,
  501,  502,  503,   -1,  505,   -1,  507,  508,  327,   -1,
  489,  490,   -1,  492,  460,   -1,  262,   -1,   -1,  338,
  499,  500,  501,  502,  503,   -1,  505,   -1,  507,  508,
   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   59,
   60,   61,   62,  499,  500,  501,  502,  503,   -1,  505,
   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  354,  321,   -1,   -1,   -1,   -1,
  434,   -1,  328,   93,   -1,  331,  405,   -1,   -1,  335,
   -1,   -1,   -1,  412,  340,   -1,  415,  343,  417,  418,
   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,  262,   -1,  264,
  265,   -1,   -1,  442,   -1,   -1,   -1,  272,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,  383,  492,   -1,
  386,  387,   -1,  288,   -1,  499,  500,  501,  502,  503,
  396,  505,   -1,  507,  508,   -1,   -1,  302,   -1,   -1,
   -1,   -1,  307,  308,   -1,   -1,  311,  413,   -1,   -1,
   -1,   -1,  491,  318,   -1,  320,  321,   -1,  458,  324,
  460,   -1,   -1,  328,   -1,   -1,  331,  506,  333,  435,
  335,  471,   -1,   -1,  440,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  489,
  490,   -1,  492,   -1,  359,  360,   -1,  362,  498,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  396,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,  410,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,
   -1,   -1,   -1,   -1,  429,   41,   -1,   -1,   44,   -1,
  435,   -1,  302,   -1,   -1,  440,   -1,  307,  308,   -1,
   -1,  311,   -1,   59,   -1,   61,  451,  452,  318,  454,
  320,  321,   -1,   -1,  324,  460,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,    0,  492,   -1,  359,
  360,   -1,  362,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  507,  508,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,  396,   41,   -1,   43,
   44,   45,   -1,  328,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,   -1,  413,   -1,   59,   60,   61,   62,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,  429,
  499,  500,  501,  502,  503,  435,  505,   -1,  507,  508,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   93,
   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   40,   -1,   -1,   43,  402,   45,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,  489,
  490,   58,  492,   -1,   -1,   -1,   63,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,
   -1,   -1,  258,  438,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,  449,   -1,   -1,   -1,  274,   -1,
   -1,  277,  278,  273,  280,  460,   -1,  277,  284,   -1,
   -1,  287,   -1,  468,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  291,   -1,   -1,  300,  295,   -1,   -1,  298,   -1,
  306,   -1,   -1,  309,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,  460,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
   -1,   -1,  378,  373,   -1,  375,   -1,  489,  490,   -1,
  492,   -1,   -1,   -1,  288,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,  400,  507,  508,   -1,  302,   -1,
   -1,  407,   -1,  307,  308,  411,  406,  311,   -1,   -1,
   -1,  411,  418,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,  423,  328,   -1,   -1,  331,   -1,  333,
  436,  335,   -1,   -1,  272,  441,  340,  341,   -1,  343,
  267,   -1,  280,   40,   -1,  445,   43,  285,   45,  276,
  288,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
  466,   58,  289,   -1,  470,   -1,   63,   -1,  295,  296,
   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,
  480,  481,  386,  387,   -1,   -1,  324,  314,   -1,   -1,
   -1,   -1,  396,   -1,   -1,  333,  323,  497,   -1,  326,
   -1,   -1,   -1,  341,   -1,   -1,  410,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  360,   -1,  362,  429,   -1,  354,  355,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,  376,  366,
  378,  379,  380,  370,   -1,   -1,   -1,  451,  452,   -1,
  454,  378,   -1,   -1,   -1,   -1,  460,   -1,   -1,   40,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
  417,  429,   -1,   -1,  421,  499,  500,  501,  502,  503,
  427,  505,   -1,  507,  508,   -1,  433,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,    0,  492,   -1,   -1,  495,  496,
  267,  498,  499,  500,  501,  502,  503,  504,  505,  276,
  507,  508,  509,  510,  511,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,    0,   40,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,  323,   -1,   -1,  326,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   59,   -1,   61,   -1,   -1,   91,   -1,  354,  355,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  258,   -1,   -1,
   -1,  378,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   91,   -1,  277,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,  300,
   -1,   -1,   -1,   -1,  421,  306,   -1,   -1,  309,   58,
  427, 1291, 1292,   -1,   63,   -1,  433, 1297, 1298,   -1,
 1300, 1301,   -1,   -1,   -1,   -1,  443,   -1, 1308, 1309,
 1310, 1311,   -1, 1313, 1314, 1315, 1316, 1317, 1318,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,  378,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,  509,  510,  511,   -1,   -1,   -1,   -1,  400,
   -1,   -1,  258,   -1,   -1,   -1,  407,   -1,   -1,   -1,
  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,  274,   -1,
   -1,  277,  278,   -1,  280,   -1,   -1,   -1,  284,  258,
   -1,  287,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,  258,   -1,   -1,  300,  274,    0,   -1,   -1,  278,
  306,  280,   -1,  309,   -1,  284,   -1,  274,  287,   -1,
   -1,  278,   -1,  280,   -1,  466,   -1,  284,   -1,  470,
  287,  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,
  309,   -1,   -1,  300,   -1,   -1,   -1,   41,   -1,  306,
   44,   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,  378,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,  400,   -1,  295,  296,   -1,  378,
   -1,  407,   -1,   -1,  303,  411,   -1,   -1,   -1,   -1,
   -1,  378,  418,   40,   -1,   42,   43,   -1,   45,   -1,
   -1,  400,   -1,   -1,  323,   -1,   -1,  326,  407,   -1,
  436,   58,  411,  400,   -1,  441,   63,   -1,   -1,  418,
  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,  352,    0,  354,  355,  436,   -1,   -1,
  466,   -1,  441,   -1,  470,  260,   -1,  366,   -1,  436,
   -1,  370,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,   -1,
   -1,  470,   -1,   -1,   -1,   41,   -1,   40,   44,  466,
   43,   -1,   45,  470,   -1,   -1,  301,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   58,   -1,   -1,  313,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  338,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
   -1,   -1,  407,   -1,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,  417,   -1,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,  260,   -1,   -1,  340,   -1,   -1,  343,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,  276,
   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,  460,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,  489,  490,  323,  492,   -1,  326,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  413,
  505,   -1,  507,  508,   -1,   -1,  262,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,  352,   -1,  354,  355,   -1,
   -1,  435,   -1,  276,   -1,   -1,  440,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  289,   -1,  452,   -1,
  454,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,  318,   -1,   63,  321,   -1,   -1,   -1,   -1,
  323,   -1,  328,  326,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,  354,  355,  359,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,  413,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  507,  508,  425,   -1,  427,   -1,   -1,   -1,   -1,  435,
  433,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   40,  295,  296,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   41,   -1,   40,
   44,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
    0,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
   -1,   41,   -1,   -1,   44,  260,   -1,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,   40,   -1,   42,   43,   59,
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  289,   -1,   -1,   -1,   63,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
  295,  326,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   40,  339,  370,   43,   -1,   45,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  445,   -1,  262,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,  507,  508,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,  497,   -1,   -1,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  323,   -1,
  340,  326,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,  359,
   -1,   -1,   -1,  280,   -1,   -1,   -1,  352,  285,  354,
  355,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,  366,   -1,  383,   -1,  370,  386,  387,   -1,  276,
   -1,   -1,   40,   -1,   42,   43,  396,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,  324,  295,  296,
   58,   -1,   -1,  413,   -1,   63,  333,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  341,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,  323,   -1,   -1,  326,
  440,   -1,  427,  360,   -1,  362,   -1,   -1,  433,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,  443,  376,
   -1,  378,  379,  380,   -1,  352,   -1,  354,  355,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  429,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
  295,   -1,   -1,  298,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
  507,  508,   -1,   -1,  339,   -1,   -1,   -1,  276,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   40,  295,  296,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  423,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   40,   -1,
  445,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,  497,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  507,
  508,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   40,   -1,  366,   43,   -1,   45,  370,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   40,   -1,  366,   43,   -1,   45,  370,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
   -1,   -1,  262,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  507,  508,  267,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,  276,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  289,
  340,   -1,   -1,  343,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,  267,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,  276,   -1,
   -1,   -1,   -1,  413,   40,   41,  366,   43,   -1,   45,
  370,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   58,   -1,   -1,  435,   -1,   63,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   40,   -1,  366,   43,
   -1,   45,  370,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  507,  508,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  507,
  508,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   40,   -1,
  366,   43,   -1,   45,  370,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   40,   -1,  366,   43,   -1,   45,  370,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  507,  508,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  454,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   40,   -1,  366,   43,   -1,   45,  370,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   58,   59,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   40,   -1,  366,   43,   -1,   45,
  370,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  507,  508,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  507,  508,  267,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   40,   -1,  366,   43,
   -1,   45,  370,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   40,   -1,
  366,   43,   -1,   45,  370,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  507,
  508,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  507,  508,  267,  318,   -1,    0,  321,   -1,   -1,
   -1,   -1,  276,   -1,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,  289,  340,   -1,   -1,  343,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   59,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  396,   -1,   -1,  267,   -1,   -1,   -1,    0,
  354,  355,   -1,   -1,  276,   -1,   -1,   -1,   -1,  413,
   -1,   -1,  366,   -1,   -1,   -1,  370,  289,   -1,   -1,
   -1,   -1,    0,  295,  296,   -1,   -1,   -1,   -1,   41,
   -1,  435,   44,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   59,  452,   -1,
  454,  323,   -1,   -1,  326,   -1,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   59,  354,  355,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  507,  508,  427,   -1,   -1,   -1,   -1,
    0,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,    0,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   59,
  492,   -1,   41,  495,  496,   44,  498,  499,  500,  501,
  502,  503,  504,  505,  318,  507,  508,  321,   -1,   -1,
   59,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,
  262,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,  318,   -1,    0,  321,
   -1,   -1,  396,   -1,   -1,   -1,  328,  318,   -1,  331,
  321,   -1,   -1,  335,   -1,   -1,   -1,  328,  340,  413,
  331,  343,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
  318,   -1,  343,  321,   -1,   -1,   -1,  359,   -1,   41,
  328,  435,   44,  331,   -1,   -1,  440,  335,  359,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   59,  452,   -1,
  454,  383,    0,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,  359,  383,   -1,  396,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,    0,  383,   -1,   -1,  386,  387,
   -1,   -1,  413,   41,   -1,   -1,   44,   -1,  396,   -1,
   -1,   -1,  262,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   59,   -1,   -1,  435,  413,   -1,   -1,   -1,  440,
  452,   -1,  454,  262,   -1,   41,    0,   -1,   44,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   41,  328,   -1,
   44,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  318,
  340,   -1,  321,  343,   -1,   59,   -1,   -1,   -1,  328,
    0,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  359,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,    0,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   59,
  262,   -1,   -1,  413,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  413,  435,   -1,   -1,   -1,   -1,
  440,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,  452,  262,  454,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,  383,   -1,  321,  386,  387,   -1,   -1,   -1,   -1,
  328,   -1,   -1,  331,  396,   -1,   -1,  335,  262,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,
   -1,  413,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,  359,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,  435,  340,   -1,   -1,  343,  440,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
  452,   -1,  454,  359,  318,   -1,   -1,  321,  396,   -1,
   -1,   -1,  262,   -1,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,  413,  340,  383,   -1,  343,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,  262,  359,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,  318,  383,
   -1,  321,  386,  387,   -1,   -1,   -1,   -1,  328,  435,
   -1,  331,  396,   -1,  440,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,  452,   -1,  454,  413,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,  359,
  328,   -1,  271,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  435,  340,   -1,   -1,  343,  440,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  452,   -1,
  454,  359,  301,   -1,   -1,  271,  396,   -1,   -1,  308,
   -1,   -1,   -1,  312,  313,   -1,  315,   -1,   -1,  318,
   -1,   -1,   -1,  413,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,  332,   -1,  301,   -1,   -1,  396,  338,
   -1,   -1,   -1,   -1,   -1,  435,  312,  313,   -1,  315,
  440,   -1,  318,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,  271,  452,   -1,  454,   -1,  332,   -1,   -1,   -1,
   -1,   -1,  338,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  301,   -1,   -1,  452,   -1,  454,   -1,   -1,  398,
   -1,   -1,  312,  313,   -1,  315,   -1,   -1,  318,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
   -1,   -1,  332,   -1,   -1,   -1,   -1,   -1,  338,   -1,
   -1,   -1,  398,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,  451,   -1,  453,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,  398,   -1,
  489,  490,   -1,  492,  460,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  417,  507,  508,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  432,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,  442,  499,  500,  501,  502,  503,   -1,  505,
  273,  507,  508,  453,  277,   -1,   -1,   -1,   -1,   -1,
  460,  284,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,
   -1,   -1,   -1,   -1,  273,   -1,  339,   -1,  277,   -1,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  339,   -1,   -1,  406,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,  423,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  507,  508,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,
  499,  500,  501,  502,  503,  460,  505,   -1,  507,  508,
   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,  493,  494,
  495,  496,   -1,   -1,  499,  500,  501,  502,  503,  504,
  505,  506,  507,  508,  509,  510,  511,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
  483,  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,
  493,  494,  495,  496,   -1,   -1,  499,  500,  501,  502,
  503,  504,  505,  506,  507,  508,  509,  510,  511,
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
"keyword_or_column : FIRST",
"keyword_or_column : SKIP",
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
"order_item : value order_direction nulls_placement",
"order_direction : ASC",
"order_direction : DESC",
"order_direction :",
"nulls_placement : NULLS FIRST",
"nulls_placement : NULLS LAST",
"nulls_placement :",
"for_update_clause : FOR UPDATE for_update_list lock_clause",
"for_update_clause :",
"for_update_list : OF column_list",
"for_update_list :",
"lock_clause : WITH LOCK",
"lock_clause :",
"select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause",
"ordered_select_expr : SELECT limit_clause distinct_clause select_list from_clause where_clause group_clause having_clause plan_clause order_clause",
"limit_clause : first_clause skip_clause",
"limit_clause : first_clause",
"limit_clause : skip_clause",
"limit_clause :",
"first_clause : FIRST long_integer",
"first_clause : FIRST '(' value ')'",
"first_clause : FIRST parameter",
"skip_clause : SKIP long_integer",
"skip_clause : SKIP '(' value ')'",
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

    line_start = ptr = string;
    end = string + length;
    lines = 1;
    att_charset = character_set;
    line_start_bk = line_start;
    lines_bk = lines;
	param_number = 1;
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

    if (g_file->fil_partitions) {
        if (!g_file->fil_length) {
            yyabandon (-261, isc_log_length_spec);
            /* Total length of a partitioned log must be specified */
        }
        
        if (PARTITION_SIZE (OneK * g_file->fil_length, g_file->fil_partitions) <
            (OneK*MIN_LOG_LENGTH)) {
            yyabandon (-239, isc_partition_too_small);
            /* Log partition size too small */
        }
    }
    else {
        if ((g_file->fil_length) && (g_file->fil_length < MIN_LOG_LENGTH)) {
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

return ptr;
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
node->nod_line = (USHORT) lines_bk;
node->nod_column = (USHORT) (last_token_bk - line_start_bk + 1);
node->nod_count = 1;
node->nod_arg[0] = (DSQL_NOD)param_number++;

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
node->nod_line = (USHORT) lines_bk;
node->nod_column = (USHORT) (last_token_bk - line_start_bk + 1);
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
node->nod_line = (USHORT) lines_bk;
node->nod_column = (USHORT) (last_token_bk - line_start_bk + 1);
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


static int yylex (
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
	yylval = (DSQL_NOD) sym->sym_object;
	return sym->sym_keyword;
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
	gds_arg_number, (SLONG) lines, 
	gds_arg_number, (SLONG) (last_token - line_start + 1), /*CVC: +1*/
	    /* Token unknown - line %d, char %d */
 	gds_arg_gds, gds_random, 
	gds_arg_cstring, (int) (ptr - last_token), last_token,
 	0);
    }
}


static void yyabandon (
    SSHORT      sql_code,
    STATUS      error_symbol)
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
            dsql_debug = yyn - '0';
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
        if (dsql_debug)
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
        if (dsql_debug)
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
                if (dsql_debug)
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
                if (dsql_debug)
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
        if (dsql_debug)
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
    if (dsql_debug)
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
{ prepare_console_debug ((int) yyvsp[0], &dsql_debug);
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
{ g_field->fld_dtype = dtype_blob; }
break;
case 80:
{ 
			g_field->fld_dtype = dtype_cstring; 
			g_field->fld_character_length = (USHORT) yyvsp[-2]; }
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
{ log_defined = FALSE;
			  cache_defined = FALSE;
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
{ if (log_defined)
			    yyabandon (-260, isc_log_redef);  /* Log redefined */
			  log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 174:
{ if (log_defined)
			    yyabandon (-260, isc_log_redef);
			  log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 175:
{ g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 176:
{ g_file->fil_flags |= LOG_serial;
			  if (g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 177:
{ g_file = make_file(); 
			  g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) g_file);}
break;
case 178:
{ g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) g_file); }
break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 181:
{ 
		         check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
                                                (DSQL_NOD) g_file); }
break;
case 182:
{ g_file = make_file();
			  g_file->fil_name = (STR) yyvsp[0]; }
break;
case 185:
{ g_file->fil_length = (SLONG) yyvsp[0]; }
break;
case 186:
{ g_file  = make_file ();}
break;
case 191:
{ g_file->fil_start = (SLONG) yyvsp[0];}
break;
case 192:
{ g_file->fil_length = (SLONG) yyvsp[-1];}
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
			g_field->fld_flags |= FLD_computed;
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
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 219:
{ g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) g_field; }
break;
case 220:
{ yyval = yyvsp[-1]; }
break;
case 221:
{ g_field = make_field (NULL);
			  yyval = (DSQL_NOD) g_field; }
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
                        make_node (nod_list, (int) 1, g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ beginning = lex_position(); }
break;
case 376:
{ yyval = (DSQL_NOD) MAKE_string(beginning, 
			       (lex_position() == end) ?
			       lex_position()-beginning : last_token-beginning);}
break;
case 377:
{ beginning = last_token; }
break;
case 378:
{ yyval = (DSQL_NOD) MAKE_string(beginning, 
					lex_position()-beginning); }
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
case 464:
{ yyval = NULL; }
break;
case 465:
{ yyval = NULL; }
break;
case 466:
{ yyval = NULL; }
break;
case 467:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 468:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 469:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 470:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 471:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 472:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 473:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 474:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 476:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 477:
{ yyval = yyvsp[0]; }
break;
case 478:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 479:
{ yyval = yyvsp[0]; }
break;
case 480:
{ yyval = yyvsp[0]; }
break;
case 482:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 485:
{ yyval = NULL; }
break;
case 487:
{ yyval = NULL; }
break;
case 488:
{ yyval = yyvsp[0]; }
break;
case 489:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 505:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 506:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 508:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 509:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 510:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 516:
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
			g_field->fld_dtype = dtype_int64; 
			g_field->fld_length = sizeof (SINT64); 
			}
break;
case 517:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 518:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 519:
{ 
			*stmt_ambiguous = TRUE;
			if (client_dialect <= SQL_DIALECT_V5)
			    {
			    /* Post warning saying that DATE is equivalent to TIMESTAMP */
		            ERRD_post_warning (isc_sqlwarn, gds_arg_number, (SLONG) 301, 
                                               isc_arg_warning, isc_dtype_renamed, 0);
			    g_field->fld_dtype = dtype_timestamp; 
			    g_field->fld_length = sizeof (GDS_TIMESTAMP);
			    }
			else if (client_dialect == SQL_DIALECT_V6_TRANSITION)
			    yyabandon (-104, isc_transitional_date);
			else
			    {
			    g_field->fld_dtype = dtype_sql_date; 
			    g_field->fld_length = sizeof (ULONG);
			    }
			}
break;
case 520:
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
			g_field->fld_dtype = dtype_sql_time; 
			g_field->fld_length = sizeof (SLONG);
			}
break;
case 521:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 524:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 525:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 526:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 527:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 528:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 529:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 530:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 531:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 532:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 533:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 535:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 536:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 537:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 538:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 539:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 540:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 549:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 550:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 551:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 552:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
		    	}
break;
case 553:
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
				g_field->fld_dtype = dtype_double;
				g_field->fld_length = sizeof (double);
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
				g_field->fld_dtype = dtype_int64;
				g_field->fld_length = sizeof (SINT64);
			        }
			    }
			else 
			    if ((SLONG) yyvsp[-1] < 5)
			    	{
			    	g_field->fld_dtype = dtype_short; 
			    	g_field->fld_length = sizeof (SSHORT); 
			    	}
			    else
			    	{
			    	g_field->fld_dtype = dtype_long; 
			    	g_field->fld_length = sizeof (SLONG); 
			    	}
			g_field->fld_precision = (USHORT) yyvsp[-1];
			}
break;
case 554:
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
				g_field->fld_dtype = dtype_double;
				g_field->fld_length = sizeof (double); 
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
				g_field->fld_dtype = dtype_int64;
				g_field->fld_length = sizeof (SINT64);
			        }
			    }
			else
			    {
			    if ((SLONG) yyvsp[-3] < 5)
			    	{
			    	g_field->fld_dtype = dtype_short; 
			    	g_field->fld_length = sizeof (SSHORT); 
			    	}
			    else
			    	{
			    	g_field->fld_dtype = dtype_long; 
			    	g_field->fld_length = sizeof (SLONG); 
			    	}
			    }
			g_field->fld_precision = (USHORT) yyvsp[-3];
			g_field->fld_scale = - (SSHORT) yyvsp[-1];
			}
break;
case 557:
{ 
			if ((SLONG) yyvsp[0] > 7)
			    {
			    g_field->fld_dtype = dtype_double;
			    g_field->fld_length = sizeof (double); 
			    }
			else
			    {
			    g_field->fld_dtype = dtype_real; 
			    g_field->fld_length = sizeof (float);
			    }
			}
break;
case 558:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 559:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 560:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 561:
{ yyval = yyvsp[-1]; }
break;
case 562:
{ yyval = 0; }
break;
case 566:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 567:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 568:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 569:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 570:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 573:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 574:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 577:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 578:
{ yyval = (DSQL_NOD) NULL; }
break;
case 580:
{ yyval = (DSQL_NOD) NULL; }
break;
case 581:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 583:
{ yyval = (DSQL_NOD) NULL; }
break;
case 585:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 590:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 594:
{ yyval = yyvsp[0];}
break;
case 596:
{ yyval = yyvsp[0];}
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 604:
{ yyval = 0; }
break;
case 605:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 606:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 607:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 608:
{ yyval = (DSQL_NOD) 0; }
break;
case 609:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 610:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 612:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 613:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 614:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 615:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 618:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 620:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 621:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 622:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 623:
{ yyval = make_list (yyvsp[0]); }
break;
case 624:
{ yyval = 0; }
break;
case 626:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 627:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 632:
{ yyval = 0; }
break;
case 633:
{ yyval = 0; }
break;
case 634:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 635:
{ yyval = 0; }
break;
case 636:
{ yyval = yyvsp[0]; }
break;
case 637:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 638:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 639:
{ yyval = 0; }
break;
case 640:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 641:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 642:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 643:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 644:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 645:
{ yyval = 0; }
break;
case 646:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 647:
{ yyval = yyvsp[-1]; }
break;
case 648:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 660:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 661:
{ yyval = make_list (yyvsp[0]); }
break;
case 663:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 667:
{ yyval = yyvsp[-1]; }
break;
case 668:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 669:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 670:
{ yyval = make_list (yyvsp[-1]); }
break;
case 671:
{ yyval = NULL; }
break;
case 673:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 677:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 678:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 679:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 680:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 681:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 682:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 683:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 684:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 685:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 686:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 687:
{ yyval = make_list (yyvsp[0]); }
break;
case 688:
{ yyval = 0; }
break;
case 690:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 695:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 699:
{ yyval = yyvsp[0]; }
break;
case 700:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 706:
{ yyval = 0; }
break;
case 707:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 708:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 709:
{ yyval = 0; }
break;
case 710:
{ yyval = 0; }
break;
case 712:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 713:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 717:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 718:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 719:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 723:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 729:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 733:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 734:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 740:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 741:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 747:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 748:
{ yyval = yyvsp[0]; }
break;
case 750:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
break;
case 759:
{ yyval = make_list (yyvsp[-1]); }
break;
case 761:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 765:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 783:
{ yyval = yyvsp[-1]; }
break;
case 784:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 812:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 828:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 829:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 832:
{ yyval = make_list (yyvsp[-1]); }
break;
case 833:
{ yyval = yyvsp[-1]; }
break;
case 834:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 835:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 846:
{ yyval = yyvsp[0]; }
break;
case 847:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 848:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 849:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 850:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 851:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 852:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 853:
{ yyval = yyvsp[-1]; }
break;
case 854:
{ yyval = yyvsp[-1]; }
break;
case 858:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 859:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 860:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 861:
{ yyval = yyvsp[0]; }
break;
case 862:
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
case 863:
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
case 864:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 865:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 867:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 869:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 870:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 871:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 872:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 873:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 876:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
break;
case 877:
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
case 878:
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
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 880:
{ yyval = make_parameter (); }
break;
case 881:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 882:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 883:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 884:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 885:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 886:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 887:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 889:
{ yyval = yyvsp[0]; }
break;
case 890:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 892:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 893:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 894:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 895:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 896:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 898:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 899:
{ yyval = yyvsp[0];}
break;
case 904:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 905:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 906:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 907:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 908:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 909:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 910:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 911:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 913:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 914:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 915:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 916:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 918:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 919:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 920:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 922:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 925:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 926:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 929:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 930:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 932:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 933:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 934:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 936:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 940:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 941:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 942:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 943:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 950:
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
        if (dsql_debug)
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
            if (dsql_debug)
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
    if (dsql_debug)
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
