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
#define MESSAGE 368
#define MERGE 369
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
  259,  259,  259,  259,  259,  259,  259,  259,  259,  256,
  256,  258,  258,  257,  255,  255,  255,  250,  250,  246,
  247,  247,  260,  260,  260,  260,  261,  261,  245,  262,
  262,  263,  263,    8,  264,  264,  264,  264,  264,  264,
  264,  264,  264,  264,  264,  264,  102,  102,  145,  145,
  267,  267,  270,  270,  272,  272,   54,   54,  269,  269,
  269,  269,  269,  269,  269,  269,  269,  277,  277,  268,
  268,  268,  268,  278,  278,   62,   62,   62,   56,   56,
  274,  274,  274,  271,  271,  271,  282,  282,  282,  281,
  281,  280,  280,  280,  275,  275,  285,  283,  283,  283,
  284,  284,  276,  276,  276,  276,  286,  286,   19,   19,
   19,  288,  288,  288,   16,   17,  292,  292,    4,   15,
  291,  291,  293,  293,  294,  294,  287,  295,  295,  296,
  296,  297,  297,  297,  297,  298,  298,  299,  299,  300,
  300,  302,  302,  302,  303,  303,  303,  304,  304,  304,
  301,  306,  306,  306,  307,  307,  305,  305,  308,  310,
  310,  309,  309,  289,   18,  311,  311,  311,  312,  312,
  315,  315,  316,   66,   66,   66,  317,  317,  317,  313,
  313,  318,  318,  319,  319,  314,  323,  320,  320,  320,
  320,  324,  324,  324,  325,  325,  325,  225,  225,  226,
  226,  327,  327,  328,  328,  328,  321,  331,  331,  332,
  332,  333,  333,  334,  334,  335,  335,  210,  210,  337,
  337,  234,  234,   32,  235,  235,  235,  235,  235,  235,
  235,  235,  229,  229,  338,  338,  339,  339,  339,  339,
  339,  341,  341,  341,  230,  230,  228,  228,  322,  322,
  345,  346,  346,  346,  346,  346,  347,  347,  348,  348,
  349,  349,  350,  350,  350,  351,  351,   10,   10,  353,
  353,    7,    7,  354,  355,  356,   20,   20,  357,  358,
  359,  359,  189,  329,  329,    3,    3,  361,  361,  361,
  363,  363,   62,  362,  362,  364,  364,   35,   35,   86,
   85,   85,  352,  352,  365,  366,  366,  211,  211,  211,
  147,  360,  360,  112,  112,  112,  112,  368,  368,  368,
  368,  368,  368,  368,  368,  368,  368,  368,  368,  369,
  369,  369,  369,  369,  369,  369,  369,  374,  374,  374,
  374,  374,  374,  374,  374,  374,  374,  374,  374,  374,
  374,  374,  374,  381,  381,  370,  370,  371,  371,  371,
  371,  372,  372,  376,  376,  377,  377,  377,  377,  375,
  378,  373,  373,  379,  379,  379,  382,  382,  383,  380,
  385,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,  157,  157,  157,
  386,  384,  384,  152,  152,  390,  390,  390,  390,  390,
  388,  388,  388,  388,  388,  219,  153,  153,  154,  155,
  155,  155,  155,  155,   52,   52,   21,   21,  242,  391,
   55,  124,  273,  273,   96,  387,  387,  387,  387,  392,
  392,  392,  392,  392,  392,  392,  392,  392,  392,  392,
  393,  342,  343,  343,  343,  340,  340,  389,  344,  344,
  395,  395,  396,  396,  397,  397,  400,  400,  398,  398,
  403,  403,  402,  399,  401,  394,  394,  394,  394,  394,
  394,  394,  394,  326,  326,  156,   49,  279,  123,  149,
  367,  166,  212,  266,   64,   61,  216,  113,   67,  330,
   36,   42,  336,  265,   40,   43,  209,   41,  290,  404,
  404,  405,  405,  405,  405,  405,  405,  405,  405,  405,
  405,  405,
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
    2,    2,    2,    1,    1,    1,    0,    2,    2,    0,
    1,    2,    2,    2,    2,    2,    1,    3,    7,    1,
    0,    1,    0,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    3,    2,    2,    2,    1,    1,    1,    1,
    4,    5,    1,    3,    1,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    4,    6,    5,    3,    0,    2,    2,    0,    3,    0,
    4,    1,    5,    4,    1,    4,    1,    2,    2,    1,
    1,    1,    2,    2,    2,    2,    1,    0,    3,    5,
    1,    1,    2,    3,    1,    2,    3,    0,    1,    1,
    1,    5,    5,    6,    2,    5,    1,    0,    3,    2,
    1,    0,    2,    0,    1,    0,    3,    1,    0,    1,
    2,    1,    1,    1,    1,    2,    2,    1,    2,    3,
    1,    1,    3,    3,    1,    2,    3,    1,    2,    0,
    2,    1,    1,    0,    1,    1,    1,    3,    2,    3,
    0,    1,    3,    4,    3,    1,    3,    4,    3,    0,
    1,    3,    3,    1,    1,    0,    2,    2,    0,    4,
    0,    2,    0,    2,    0,    9,   10,    2,    1,    1,
    0,    2,    4,    2,    2,    4,    2,    1,    1,    1,
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
    1,    1,    3,    1,    3,    3,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    1,    1,    5,    6,    3,    4,    5,
    6,    3,    4,    3,    4,    3,    4,    4,    5,    4,
    4,    3,    4,    1,    1,    1,    1,    3,    3,   10,
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
    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  559,
  560,  561,    0,  616,  722,  723,  727,  728,  470,    0,
    0,    0,    0,    0,    0,  407,  571,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  889,    0,   22,  887,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  484,    0,    0,   38,   35,   37,    0,   36,
    0,  970,  972,  981,  974,  978,  973,  982,  979,  975,
  977,  980,  976,    0,    0,    0,    0,   33,   69,   67,
  962,  971,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  640,    0,    0,    0,  672,
    0,    0,  964,    0,    0,    0,  108,  565,  969,    2,
    0,    0,    0,    0,  454,  436,  437,  438,  439,  440,
  441,  442,  443,  444,  445,  446,  447,  448,  449,  450,
  451,  452,  453,  455,  456,  457,  458,  459,    0,  435,
    0,  955,    0,  414,  959,    0,  411,  961,    0,  674,
    0,  410,  965,    0,  569,  885,    0,  151,  104,    0,
  105,    0,  218,  761,  951,    0,  103,  146,  958,    0,
    0,   99,    0,  106,  891,    0,  100,    0,  101,    0,
  102,  968,  147,  107,  624,  625,    0,  890,  888,    0,
   75,    0,  956,    0,  492,  954,  485,    0,  491,  496,
  486,  487,  494,  488,  489,  490,  495,    0,   31,    0,
   40,  748,   39,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  895,    0,
  876,  642,  644,    0,  645,  647,  944,  648,    0,  649,
  638,    0,    0,    0,    0,    0,    0,    0,  588,  577,
    0,  580,  582,  583,  584,  585,  591,  592,    0,  673,
  963,    0,  109,    0,  110,    0,  111,    0,    0,  617,
    0,  615,    0,    0,    0,    0,  471,    0,    0,  377,
    0,  221,    0,  417,  408,  468,  469,    0,    0,    0,
    0,    0,    0,  426,    0,  383,  384,    0,  575,  573,
  886,    0,    0,    0,    0,    0,    0,    0,  154,  133,
    0,   97,    0,    0,  112,    0,  122,  121,    0,    0,
    0,    0,    0,    0,  947,   76,    0,    0,    0,  724,
  725,  493,    0,  946,  868,  867,  866,    0,    0,  869,
  870,    0,    0,  872,  356,  357,  351,    0,  355,  864,
  871,    0,  751,    0,    0,   28,    0,    0,    0,   72,
   70,  966,   68,   34,    0,    0,    0,  753,    0,   53,
    0,    0,    0,    0,    0,  567,    0,    0,    0,    0,
  854,  882,    0,    0,  856,    0,    0,  883,    0,  877,
    0,    0,  858,  859,  860,  878,  879,    0,  880,  881,
    0,    0,    0,  884,    0,    0,    0,    0,    0,    0,
  758,  851,  852,  853,  857,  837,    0,  836,    0,  840,
  898,  899,  839,  833,  834,  835,  838,  896,  897,  919,
  920,  923,  924,    0,    0,  651,    0,  735,    0,    0,
  652,    0,    0,  614,    0,  589,    0,  586,    0,  587,
  612,    0,  607,    0,    0,  581,  762,  731,    0,    0,
    0,    0,    0,    0,    0,    0,  621,  618,    0,  186,
    0,    0,  130,  476,  173,  174,    0,  474,    0,    0,
    0,    0,  477,    0,  472,    0,    0,  421,  423,  422,
    0,  424,    0,  418,  219,    0,    0,  272,    0,    0,
    0,    0,  430,  431,    0,  461,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  388,  387,  480,    0,
    0,  150,    0,    0,  159,    0,  158,    0,  165,  148,
    0,  163,  166,  167,  155,    0,    0,  515,  551,  518,
    0,  552,    0,    0,  519,    0,    0,    0,  542,  555,
  514,  537,  516,  517,  512,  499,  377,  497,  498,  500,
    0,    0,  509,  510,  511,  513,    0,    0,    0,    0,
    0,  113,    0,  114,    0,  125,    0,    0,  202,    0,
    0,  375,    0,  221,    0,    0,   84,    0,    0,  743,
    0,    0,    0,    0,    0,  824,  825,  826,    0,    0,
    0,  764,  768,  769,  770,  771,  772,  773,  774,  775,
  776,  777,  778,  873,  874,  875,  865,    0,    0,    0,
  750,   29,    0,    0,   45,   73,    0,    0,   27,    0,
  756,    0,    0,    0,  719,    0,    0,    0,    0,    0,
  566,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  326,  967,    0,    0,    0,    0,    0,    0,
    0,  643,    0,    0,  646,    0,    0,    0,    0,  655,
  960,  563,    0,  893,  562,    0,  590,    0,  598,  594,
  593,    0,    0,    0,  609,  597,    0,    0,  729,  730,
    0,    0,    0,  375,    0,    0,    0,    0,    0,    0,
  131,    0,    0,    0,    0,    0,    0,  377,  420,    0,
  377,  378,  425,  507,    0,  274,    0,  268,  270,  375,
  241,  952,    0,    0,  377,    0,    0,  210,  375,    0,
    0,    0,    0,  246,  240,  243,  244,  245,  429,  466,
  465,  428,  427,    0,    0,    0,    0,    0,    0,  386,
    0,  377,    0,  156,  160,    0,  164,    0,    0,  538,
  539,  556,    0,  553,    0,    0,  545,  543,  544,    0,
    0,    0,    0,  508,    0,    0,    0,    0,  546,    0,
    0,  375,    0,  201,    0,  204,  206,  207,    0,    0,
    0,    0,    0,  221,   79,    0,    0,   78,  526,  527,
  948,    0,    0,    0,  767,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  353,  359,  360,  358,  752,
    0,    0,    0,    0,    0,    0,    0,    0,   54,   62,
   64,    0,    0,   71,    0,    0,    0,  755,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  936,  937,  938,  939,  940,  941,  942,  943,    0,    0,
    0,    0,    0,    0,  929,    0,    0,    0,  670,    0,
    0,  849,  850,  917,    0,    0,  845,  950,    0,    0,
    0,    0,    0,    0,  855,  760,  759,    0,    0,    0,
    0,  660,  661,    0,    0,  653,  656,  564,  894,  599,
  608,  603,  602,    0,  613,  763,  732,  733,  375,    0,
    0,    0,  623,  622,    0,    0,  630,  182,  176,  183,
    0,  179,    0,    0,  178,    0,  189,  170,  169,  892,
  172,  171,  478,    0,    0,    0,  419,  273,    0,  212,
  377,  214,    0,  378,  209,  215,    0,    0,    0,    0,
  432,  433,  377,  375,  378,    0,    0,    0,  404,    0,
  197,  198,  157,    0,    0,    0,    0,    0,    0,  554,
    0,    0,  377,  378,    0,  503,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  199,
    0,    0,    0,    0,  364,    0,   83,    0,    0,    0,
    0,   88,   85,    0,    0,    0,  726,  953,    0,    0,
  779,    0,    0,    0,  805,  804,    0,    0,    0,  812,
  827,  822,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  766,    0,   66,   58,   59,   65,   60,   61,
    0,    0,   23,    0,    0,    0,   43,    0,    0,    0,
  737,  757,    0,  720,    0,  736,    0,    0,    0,    0,
    0,    0,  221,    0,  900,    0,    0,  915,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  925,    0,  935,    0,    0,    0,    0,    0,    0,  916,
  861,    0,    0,    0,    0,    0,    0,  675,    0,    0,
    0,    0,    0,  605,  606,  610,    0,  200,    0,  627,
  628,  634,    0,    0,    0,    0,    0,    0,  190,    0,
    0,  415,  224,  225,  226,  227,  228,  229,    0,    0,
    0,  277,    0,  222,    0,    0,    0,    0,  247,    0,
  462,  463,  434,    0,    0,    0,    0,    0,    0,  377,
  482,  378,  168,  949,    0,    0,  521,    0,  520,  557,
    0,  549,    0,    0,    0,  501,    0,  529,    0,    0,
  531,  534,  536,  375,    0,    0,    0,  120,    0,  205,
  377,    0,    0,  376,    0,   98,    0,  118,    0,   94,
    0,    0,    0,    0,   87,    0,  820,  821,    0,    0,
    0,    0,    0,    0,  823,    0,    0,    0,    0,    0,
    0,    0,  813,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   57,   63,
   55,   56,   24,  741,  742,    0,  739,  746,  747,  744,
    0,    0,  718,    0,    0,  906,  905,    0,    0,  902,
  901,    0,  910,  909,  908,  907,  904,  903,    0,    0,
  931,    0,    0,    0,  930,    0,    0,    0,  668,    0,
    0,  663,    0,  664,    0,  681,  677,  679,    0,    0,
    0,    0,    0,  376,    0,  184,  180,    0,    0,  195,
  191,    0,  378,  280,    0,    0,  378,  278,    0,    0,
    0,  232,    0,    0,  248,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  479,  523,    0,  524,    0,  135,
    0,    0,    0,  139,  141,  142,  143,  504,  506,  502,
  533,    0,    0,  378,    0,    0,    0,    0,  361,    0,
  365,  377,   89,    0,    0,   91,    0,    0,    0,    0,
    0,    0,  829,  828,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   41,    0,    0,  721,  220,  918,  911,  912,
    0,  913,  927,  926,    0,  932,  921,    0,  922,    0,
  666,    0,  547,    0,  688,    0,    0,  685,  689,  690,
  692,  693,  694,    0,    0,  636,  378,  362,    0,  175,
  192,  378,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  285,
  290,  284,    0,    0,    0,  294,  295,  296,  297,  298,
  299,  301,  302,  304,  305,  306,  309,  267,    0,    0,
  208,  233,    0,    0,    0,    0,  234,  237,    0,    0,
  378,  402,  403,  399,  398,  401,  400,    0,  522,  550,
  144,  132,  140,    0,  377,  264,  127,  378,    0,    0,
  379,  366,    0,    0,   92,   93,   80,    0,  791,  799,
    0,  792,  800,  795,  803,    0,    0,  793,  801,  794,
  802,  788,  796,  789,  797,  790,  798,  738,    0,    0,
  928,  669,    0,    0,    0,    0,    0,  702,  704,    0,
  699,    0,  265,  185,  416,    0,  312,    0,    0,  315,
  316,    0,  308,    0,    0,    0,    0,  307,    0,  338,
  339,  293,  300,    0,  303,  286,    0,  291,    0,  342,
  292,  211,  216,  235,    0,    0,  238,    0,  250,  252,
  406,  378,    0,  381,    0,    0,    0,  371,  372,    0,
    0,   77,   95,    0,    0,  914,    0,    0,  691,  686,
  703,    0,    0,  283,    0,  310,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  333,  334,  350,    0,
    0,    0,    0,  345,  343,  239,    0,    0,    0,    0,
    0,  266,  378,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  710,    0,    0,    0,  282,  279,  311,
    0,    0,    0,    0,  314,    0,    0,    0,    0,  322,
    0,  325,    0,  349,  348,  957,  347,    0,    0,    0,
    0,    0,  249,    0,  256,    0,  255,  382,  374,    0,
    0,  367,  119,    0,    0,    0,  712,  701,    0,    0,
  713,    0,  709,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  336,  335,  344,  346,  236,    0,    0,
  260,  259,  258,    0,    0,  637,  831,  708,    0,  715,
  328,    0,    0,  313,  319,    0,    0,    0,    0,  337,
  263,  261,  262,    0,    0,    0,    0,    0,    0,  340,
  317,    0,    0,    0,    0,  714,  331,  318,  320,  830,
  717,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1516,   27,   28, 1517,
   30,   31,   32,   33,   34,   35,   36, 1518,   38, 1519,
  630,  114,  395,  887, 1133,  115,  664,  888,  116,  399,
  669,  140,  117,  118,  251,  959,  130,  889,  890,  191,
  220,  119,  891,  120,  401,   79,  231,  366,  449,  625,
 1070,  384,  847,  596, 1483,  824,  626,  627,  628, 1072,
  232, 1324,   71,  181,   72,  227, 1806, 1276,  212,  217,
  219,  221,  207,  199,  201,  214,  224,  147,  303,  305,
  307,  355,  612,  614,  392,  252,  774,  527,  640, 1007,
  359,  617, 1058, 1268,  563,  724, 1033,  512,  513,  542,
  351,  597, 1044, 1412, 1541, 1212, 1413, 1414, 1415, 1416,
 1417,  641,  208,  200,  347,  570,  348,  349,  571,  572,
  573,  574, 1243, 1001,  515,  516,  991,  989,  517,  995,
  990, 1203, 1376,  996,  997, 1208,  361,  835,  836,  837,
  838,  776, 1014, 1390,  598,  778,  451,  840,  947,  536,
 1339, 1213,  452,  453,  454,  939,  455, 1391, 1392,  545,
 1547, 1699, 1229,  784,  785,  771,  786,  787,  788, 1700,
 1701, 1782,  329,  540, 1220, 1520,  187,  537,  538, 1221,
 1222, 1385, 1675, 1521, 1522, 1523, 1524, 1639, 1525, 1526,
 1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1535, 1536,
 1537, 1722, 1767, 1622, 1686, 1771, 1684,  456,  703, 1358,
  457, 1077, 1640, 1693, 1694, 1735,  387,  388,  458, 1063,
 1274, 1429, 1064, 1065,  279,  479, 1570,  370, 1193, 1372,
 1656, 1657, 1658, 1659, 1191,  338,  559,  802, 1241,  560,
  800,  215,   56,  333,  192,  154,  316,  179,  323,  184,
  762,  759,  324,  334,  792,  335,  556, 1025,  180,  317,
  524,  561, 1242,   93,  190,  235,  599,  600,  764, 1045,
  765, 1046, 1047,  603,  604,  605,  606, 1038,  850,  607,
  608,  609,  817,  610, 1485,  814,   40,   41,   42,  148,
   58,  417,  195,  340,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  730,  492,  974, 1196,  493,  494,  735,
   43,  153,  312,   44,  506,  507,  983,  748,  987,  134,
  717, 1496,  675,  135,  136,  280,  480,  481,  482,  720,
  960,  961,  962,  963, 1185,  459, 1173, 1487, 1488,  460,
 1490,  461,  462,  463, 1714, 1612, 1715, 1716, 1717, 1763,
 1807,  407, 1145,   45,   46,  371,   47,   48,  500,  501,
  896, 1141, 1326, 1330,  408,  672,  204,  642,  643,  644,
  645,  646,  647,  648,  649,  650,  651,  652,  653, 1293,
 1088, 1090, 1091,  946,  702,  464,  465,  466,  467,  391,
  229,  468,  469,  929,  470,  471,  472,  473,  694,  933,
 1174, 1169,  695,  474,  122,
};
static short yysindex[] = {                                   4535,
 1482,  305, 1384,  144, 1194,  503, 3482,  536, 5080,  440,
  554, 3753,  305, 1144,  488, 5601,  -48, 5601,    0,  857,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  451,    0,    0,    0,    0,    0,    0, 6573,
 5601, 5601, 5601, 5601, 5601,    0,    0,  625, 1265, 5601,
 5601, 5601,  785, 5601,  655, 5601, 5601,    0, 5601, 5601,
    0, 1012,    0,  736,    0,    0,  752, 5601,    0, 5601,
 5601, 5601,  863, 5601, 5601, 5601, 5601,  655, 5601, 5601,
 5601, 5601,    0, 5601,  805,    0,    0,    0, 1180,    0,
 1180,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  853,  873,  180, 1242,    0,    0,    0,
    0,    0, 5601, 5601, 5601,  959, 1017, 1047,  182, 1066,
 1000,  143,  220,  764,  967,    0, 5601, 1118, 1421,    0,
 1048, 5601,    0, 5601, 5601, 5601,    0,    0,    0,    0,
 1196,   31, 1184,  558,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  877,    0,
 1265,    0,   82,    0,    0, 1492,    0,    0,  112,    0,
  576,    0,    0, 1161,    0,    0, 1096,    0,    0,  754,
    0, 1304,    0,    0,    0, 1265,    0,    0,    0,  737,
 1492,    0,   57,    0,    0, 1258,    0, 1269,    0, 1180,
    0,    0,    0,    0,    0,    0, 1275,    0,    0, 1165,
    0, 1303,    0, 1220,    0,    0,    0, 1165,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2572,    0, 5601,
    0,    0,    0, 5448, 1253, 5130, 5601, 1348, 1352, 1699,
 1444, 1423, 5448, 1376, 5130, 1400, 1408, 1296,    0, 9737,
    0,    0,    0, 9737,    0,    0,    0,    0, 7526,    0,
    0, 1375, 5601, 1463, 1378,  523, 5601, 1398,    0,    0,
 1421,    0,    0,    0,    0,    0,    0,    0, 5601,    0,
    0, 1492,    0, 1258,    0, 1180,    0, 9737, 1440,    0,
 1424,    0,  812, 1508,  886,  558,    0, 1586,   92,    0,
 5601,    0,  877,    0,    0,    0,    0, 5601, 1469, 4152,
 1417, 4398, 1848,    0, 8597,    0,    0,  586,    0,    0,
    0, 1840, 1840, 1265, 1528, 1265, 1806,  754,    0,    0,
 4119,    0, 5601, 5601,    0, 1469,    0,    0, 1627,  616,
 1881, 5601, 1658, 5601,    0,    0, 1891,  106, 4846,    0,
    0,    0, 1481,    0,    0,    0,    0, 1487, 1497,    0,
    0, 1527, 2440,    0,    0,    0,    0, 1922,    0,    0,
    0, 1521,    0, 5601, 1524,    0, 5601, 5601,  135,    0,
    0,    0,    0,    0, 5601, 5601,  961,    0, 5601,    0,
 1651, 5601, 1929, 5448, 1580,    0, 5601, 1949, 1957, 1961,
    0,    0, 1963, 1967,    0, 1968, 1970,    0, 1974,    0,
    0, 1979,    0,    0,    0,    0,    0, 1981,    0,    0,
 8199, 1985, 1988,    0, 9737, 9737, 8441, 5601, 1990, 2304,
    0,    0,    0,    0,    0,    0, 1899,    0, 1991,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2554,    0, 3547,    0, 1711, 1994,
    0, 3720,  142,    0,  556,    0,  -37,    0,  -37,    0,
    0, 1997,    0,  217, 1606,    0,    0,    0, 1996,  109,
 1983, 1469, 2005, 1783, 2523, 2006,    0,    0, 1672,    0,
  151, 1710,    0,    0,    0,    0, 1265,    0, 1840, 1840,
 1840, 1840,    0, 2012,    0, 1781, 1782,    0,    0,    0,
 1762,    0, 4119,    0,    0, 4119, 1591,    0, 1492, 1800,
 5601,10956,    0,    0,  632,    0, 5601,  318,  112,    0,
    0,    0,    0, 1668, 1632, 1602,    0,    0,    0,   77,
 1677,    0, 1619,  655,    0, 1265,    0, 1802,    0,    0,
 1806,    0,    0,    0,    0,  231, 1633,    0,    0,    0,
 1635,    0, 1682, 2043,    0, 1743, 2054,   50,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2009,  640,    0,    0,    0,    0,  115, 2056, 2057, 2054,
 1492,    0, 1784,    0, 1841,    0, 1265, 1265,    0, 4152,
  576,    0, 1720,    0, 1696, 2069,    0, 6332, 1230,    0,
 1730, 1739, 2084, 5182, 2085,    0,    0,    0, 4943, 7594,
   40,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1818, 2440, 5601,
    0,    0, 5953, 1691,    0,    0, 1667, 5130,    0, 1813,
    0, 1833, 1144, 2092,    0, 1813, 5953, 1814, 1817, 5601,
    0,  890, 8523,  680, 5601, 9737, 1049, 1109, 1112, 3724,
 9737, 5182, 3547, 1687,  533, 9737, 8523,  970,  970, 1144,
 2573, 2100,    0,    0, 8765, 5601, 9737, 9737, 9737, 9737,
 9737,    0, 9737,  727,    0,  347, 1681, 8523, 5601,    0,
    0,    0, 1033,    0,    0,    1,    0, 1695,    0,    0,
    0, 5601,  962, 5601,    0,    0, 5601, 5601,    0,    0,
 8523, 1893, 4152,    0, 1643, 9737, 5601, 1705, 1265, 1265,
    0,  -23, 1619, 1619, 1706,  655,  886,    0,    0, 2125,
    0,    0,    0,    0, 1890,    0, 5601,    0,    0,    0,
    0,    0, 1896, 2128,    0, 1877, 1895,    0,    0, 1830,
 1834, 1180, 1912,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  655, 5601,11021, 1794, 1805, 1807,    0,
  655,    0, 1355,    0,    0, 1776,    0,  114, 1780,    0,
    0,    0,  655,    0, 2043,  174,    0,    0,    0, 1900,
  174, 1785,  174,    0, 2162,  655,  655,  655,    0, 1469,
 5601,    0, 1850,    0, 1853,    0,    0,    0,  586, 1791,
 5601, 1858,  130,    0,    0, 2169, 1938,    0,    0,    0,
    0,  106, 5601, 1796,    0, 1796, 4348,  145, 9737, 9737,
 5858, 2181,    5, 6112, 9737, 6366, 1101, 6450, 6704, 8847,
 6958, 7042, 7296, 5182, 5182,    0,    0,    0,    0,    0,
 5601, 5601, 5601, 5601, 5601, 5601,  136, 2183,    0,    0,
    0, 5953, 1843,    0,   27, 1862, 5601,    0,  764, 8523,
 1862, 2186, 2183, 5953, 2794, 1905, 9737, 9737, 1972, 9737,
 2190, 9737, 2193, 2770, 9737, 9737, 9737, 9737, 9737, 9737,
    0,    0,    0,    0,    0,    0,    0,    0, 1918, 2010,
  433, 9737,  645, 8523,    0, 5182, 2210, 3547,    0, 2201,
  764,    0,    0,    0, 3547, 1874,    0,    0, 1966,  970,
  970, 1173, 1173, 1073,    0,    0,    0,  347, 2208, 2214,
 1257,    0,    0, 5182, 1933,    0,    0,    0,    0,    0,
    0,    0,    0,  340,    0,    0,    0,    0,    0, 1888,
 1791, 1020,    0,    0, 2218, 1760,    0,    0,    0,    0,
 1892,    0, 1840, 2001,    0,  -23,    0,    0,    0,    0,
    0,    0,    0, 2229, 5182, 1916,    0,    0, 1971,    0,
    0,    0, 1916,    0,    0,    0, 1180, 1180, 1771, 2233,
    0,    0,    0,    0,    0,  -21,  443,   76,    0, 2014,
    0,    0,    0, 5601,  144, 1894, 1854, 1890, 2238,    0,
 1619, 1902,    0,    0, 1158,    0, 2223, 5601, 1203,  655,
 2242, 2245, 2247, 2025,  576, 1971, 1840, 1710, 4152,    0,
 1677,  764, 1837, 1856,    0,  398,    0,  655, 1903, 1989,
 6332,    0,    0,  655, 1809, 1998,    0,    0, 2253, 2262,
    0, 2534, 3547, 2265,    0,    0, 3547, 2266, 9089,    0,
    0,    0, 1962, 2277, 3547, 2286, 1306, 2291, 3547, 2295,
 9737, 9737, 2181, 9737, 9171, 2296, 3547, 2297, 2298, 3547,
 2299, 9737, 3547, 2302, 3547, 2310, 2312, 3547, 2315, 2317,
 3547, 2318,    0, 2082,    0,    0,    0,    0,    0,    0,
 2033, 5953,    0, 5953,  136, 2183,    0,  176,  176,  160,
    0,    0, 7526,    0, 1906,    0, 2186, 2183, 2326, 2794,
 2795, 2814,    0, 2867,    0, 3061, 9737,    0, 3085, 3099,
 3273, 3361, 3394, 3520, 9737,  655, 8523, 3547, 1926, 8523,
    0, 9737,    0, 2068,  657, 9737, 8523, 9737, 9737,    0,
    0, 1257, 2321, 8523, 5601,  347, 1995,    0, 1999, 2002,
 2036, 2110, 2055,    0,    0,    0, 1971,    0, 1837,    0,
    0,    0, 1973, 1265, 2015, 1619, 2016, 1619,    0, 5182,
  229,    0,    0,    0,    0,    0,    0,    0, 1939, 2121,
 1971,    0, 9737,    0, 2119, 2003, 1771, 1012,    0, 5182,
    0,    0,    0, 2022, 2023, 2027, 2029, 2030, 2031,    0,
    0,    0,    0,    0, 2373,  144,    0, 1706,    0,    0,
  174,    0, 1916, 2051,  174,    0,  174,    0, 1890, 2380,
    0,    0,    0,    0,  586, 2121, 1619,    0, 1710,    0,
    0, 7526, 2152,    0,   38,    0, 2218,    0, 2391,    0,
 2392, 1265,  -67, 2393,    0, 1265,    0,    0, 9737, 2019,
 2019, 1144, 2403, 1915,    0, 2019, 2019, 9737, 2019, 2019,
 2661, 3547,    0, 2108, 9737, 3547, 2019, 2019, 2019, 2019,
 3547, 2019, 2019, 2019, 2019, 2019, 2019, 2061,    0,    0,
    0,    0,    0,    0,    0, 2021,    0,    0,    0,    0,
 1711, 8523,    0, 2794, 2326,    0,    0, 4119, 2407,    0,
    0, 3570,    0,    0,    0,    0,    0,    0, 3597,  141,
    0, 8523, 2151, 2032,    0, 8523, 3710, 1921,    0, 2028,
 3547,    0, 1934,    0, 1257,    0,    0,    0,  347, 5577,
 5182, 2066, 2121,    0, 1840,    0,    0, 1265, 1355,    0,
    0,  391,    0,    0, 5601,10836,    0,    0, 3547, 2170,
 2119,    0, 1128, 5601,    0, 2126,  485, 2024, 2127, 2034,
 2163, 2131, 2177, 1971,    0,    0, 2430,    0, 2438,    0,
 2134, 2170, 2051,    0,    0,    0,    0,    0,    0,    0,
    0, 1971, 1677,    0, 1355, 2014, 2168, 2098,    0, 1791,
    0,    0,    0, 2118,  -47,    0, 1890, 2129, 3547, 2451,
 2457,  764,    0,    0, 2464, 2466, 3547, 2468, 2469, 9737,
 9737, 3547, 2470, 2471, 2476, 2481, 2482, 2483, 2485, 2486,
 2489, 2490,    0,  176, 1681,    0,    0,    0,    0,    0,
  655,    0,    0,    0, 8523,    0,    0, 8523,    0, 1681,
    0,  944,    0, 2254,    0, 2491, 2492,    0,    0,    0,
    0,    0,    0,   40,  799,    0,    0,    0, 1619,    0,
    0,    0,    0, 4119, 1820,  408, 2480,  476, 2500, 2202,
 9737, 2484, 2505, 2487, 2496, 2498, 2499, 2221, 2503,    0,
    0,    0, 2255,10785, 2508,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2530, 5601,
    0,    0, 2212, 2228, 5601, 1771,    0,    0, 1180, 5601,
    0,    0,    0,    0,    0,    0,    0, 2121,    0,    0,
    0,    0,    0, 2121,    0,    0,    0,    0,  629, 1681,
    0,    0, 9737, 1265,    0,    0,    0, 1265,    0,    0,
 9737,    0,    0,    0,    0, 3547, 3547,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1933, 2532,
    0,    0, 1933, 5182, 5601,  987, 5577,    0,    0, 2205,
    0, 2538,    0,    0,    0,  156,    0, 9413, 5601,    0,
    0, 9737,    0,  581, 2240, 5182, 3375,    0, 5182,    0,
    0,    0,    0, 2282,    0,    0,  724,    0, 2136,    0,
    0,    0,    0,    0, 1771, 1180,    0, 2207,    0,    0,
    0,    0, 2014,    0,  629, 2547, 1257,    0,    0, 1933,
 3547,    0,    0, 2028, 2055,    0, 2055,   40,    0,    0,
    0, 4423, 1916,    0, 2533,    0, 3284, 9495, 1726, 9737,
 2282,  493, 9737, 2539,  649, 1146,    0,    0,    0, 5601,
 5601,  144,  241,    0,    0,    0, 2207,  501, 1771, 2217,
 2226,    0,    0, 1257, 2552,  629, 2263, 2055, 2569, 1681,
 2066, 2066, 5601,    0, 2570, 2577, 1098,    0,    0,    0,
 7624, 2216, 2578, 2282,    0, 1321,  228, 2194, 3547,    0,
 2324,    0, 2282,    0,    0,    0,    0,10836,  724, 1771,
  867,  867,    0, 2329,    0, 2192,    0,    0,    0, 1257,
  629,    0,    0, 1933, 2249, 2249,    0,    0, 4423, 2597,
    0, 5601,    0, 2573, 1936,  467, 2580, 1339, 2282, 2347,
 2338,10836,10836,    0,    0,    0,    0,    0, 2184,  692,
    0,    0,    0, 1472, 2055,    0,    0,    0, 5601,    0,
    0, 2282, 2600,    0,    0,  389, 5601,10836, 2342,    0,
    0,    0,    0, 5182, 2066, 2606, 2610, 1950,10836,    0,
    0,10836,   40, 2249, 5601,    0,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  172,  842,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  139, 5501,    0,    0,    0,    0,    0, 2654,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  830,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1241,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2320,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2272,    0,    0,    0,  153,    0,
  153,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2281,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1260,    0,    0, 7863, 5760,    0,    0,    0, 1278,    0,
    0, 1499,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  369,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   71,    0,    0,    0,    0,
  246,    0,    0, 1283,    0,    0,    0,    0,    0, 1164,
    0, 4726,    0,    0,    0,    0,    0,    0,    0,    0,
   71,    0,  447,    0,    0, 2618,    0,    0,    0, 2396,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1401,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1150,
    0,    0,    0,    0,    0,    0,    0, 5699,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  237,    0,    0,
 1409,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   71,    0, 2618,    0, 2396,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1445,    0, 2389,    0,    0,
    0,    0, 1467,    0,    0,    0,    0,    0, 2415,  -10,
 8921,    0, 1475,    0,    0,    0,    0,  242,    0,    0,
    0, 2213, 2213,    0,    0,    0, 1539, 2139,    0,    0,
    0,    0,    0,    0,    0, 2415,    0,    0, 1334,    0,
    0,    0,    0,    0,    0,    0, 3696, 2301,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1555,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1620,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1634,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1864,    0,    0,    0,    0,    0,    0, 2381,    0,    0,
    0, 2685, 2893,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 3197,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2173,    0,    0,  434,    0,    0, 2364,
    0,  338,    0,    0,    0,    0,  410,    0,  410,    0,
    0,  912,    0,  930,  982,    0,    0,    0,    0, 1395,
    0, 2415,    0,    0,  101,  691,    0,    0,  131,    0,
 3676,  257,    0,    0,    0,    0,    0,    0, 2213, 2213,
 2213, 2213,    0,  644,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2422,    0,
    0,    0,    0,    0, 2389,    0,    0, 1214,    0,  704,
  810,  905, 1045, 2224,    0,    0,    0,    0,    0,    0,
  234,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1652,    0,    0,    0,    0, 1010,  904,    0,    0,    0,
 1563,    0,    0, 5318,    0,    0, 5421,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1264, 2710,    0,    0,    0,    0, 5466, 2241,    0, 5421,
   71,    0,    0,    0,    0,    0,    0,    0,    0,  -10,
  838,    0,    0,    0,    0, 2276,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  126,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  259,
    0,    0, 5501,    0,    0,  259,    0,    0,    0,    0,
    0, 9819,    0, 9819,    0,    0, 9819, 9819, 9819,    0,
    0,    0, 2244,    0,    0,    0,    0, 4044, 4251, 8117,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  425,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  517,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -10,    0, 1198,    0,    0,  384,    0,    0,
    0, 4019,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1240,    0,    0,    0,    0,    0,
    0,    0, 2651,    0,    0, 3372, 1715,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  282,  289,  417,    0,
    0,    0, 2275,    0,    0,    0,    0,    0, 1740,    0,
    0,    0,    0,    0, 5318,    0,    0,    0,    0, 1013,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2415,
    0,    0,  196,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 6924,    0,    0,    0,  269,    0,    0,    0,
    0, 2387,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1662, 1665,    0,    0,
    0,    0,    0,    0,    0, 1666,    0,    0, 7863,    0,
 1666, 1669, 1701,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  888,    0,    0,
 9819,    0,    0,    0,  320,    0,    0,    0, 3405, 4467,
 4674, 3621, 3828,    0,    0,    0,    0,    0, 1609, 2142,
 1636,    0,    0,    0, 1474,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  150,    0,    0,    0,    0,    0,
    0,    0, 2213, 2239,    0, 4099,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2427,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1615,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1703,
    0,    0,    0,    0,    0,    0,    0, 2544,    0,    0,
    0,    0,    0,    0,    0,    0, 1268,    0,    0,    0,
    0,    0,    0,    0,  838, 2427, 2213, 1713,  -10,    0,
 2436, 7863, 1723,  163,    0,    0,    0,    0, 6924,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3038,    0,    0,    0, 5711,    0,    0,    0,
    0,    0,    0,    0, 6447,    0, 7039,    0, 7511,    0,
    0,    0,    0,    0,    0,    0, 8391,    0,    0, 9687,
    0,    0,10007,    0,10079,    0,    0,10090,    0,    0,
10113,    0,    0, 7262,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1662, 1745,    0, 2279,  299,    0,
    0,    0,    0,    0,    0,    0, 1758, 1777, 1779,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2278,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2374, 1106,    0, 3191,    0, 2376,    0, 2377, 2382,
    0,    0,  334,    0,    0,    0, 2427,    0, 1723,    0,
    0,    0, 3266,    0,    0,    0, 2280,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 5797,    0,
 2452,    0,    0,    0, 1159,    0, 1615, 2320,    0,    0,
    0,    0,    0,  656,  714,  776,  811,  999, 1028,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1133,    0,    0,    0,    0, 2805,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1789,    0,
    0,    0,    0,    0,    0,    0, 1797,    0,    0,    0,
    0,    0,  319,    0,    0,    0,    0,    0,    0,    0,
    0, 8117,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10251,    0,10272,    0,10399,    0,    0,    0,    0,
10463,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1839,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1225,    0,    0,    0, 1708,    0,    0,    0,    0,    0,
    0,  210,    0,    0, 2213,    0,    0,    0, 2548,    0,
    0,    0,    0,    0,    0, 2425,    0,    0, 2693, 1707,
 2930,    0, 2389,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2427,    0,    0,    0,    0,    0,    0,
    0, 1844, 1153,    0,    0,    0,    0,    0,    0,    0,
    0, 2427, 2436,    0,  315,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  756,    0,10495,    0,
    0, 9819,    0,    0,    0,    0,10537,    0,    0,    0,
    0,10601,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  299, 1027,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  665,
    0, 2374,    0,  134,    0,    0,  784,    0,    0,    0,
    0,    0,    0, 1063, 2696,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1614, 1639,    0,    0,    0,    0,    0,
    0,    0,    0, 2429,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 3544,    0,    0,  105,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  168,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10633,10678,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  997,    0,
    0,    0,  339,    0,    0,    0,    0,    0,    0, 1990,
    0,    0,    0,    0,    0, 2680,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2689,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2441,    0,
    0,    0,    0,    0, 3544,  171,    0,  118,    0,    0,
    0,    0,    0,    0,    0,  181,  660,    0,    0,  169,
 2693,    0,    0,    0,  394,    0,  331, 3399,    0,    0,
    0, 2696,    0,    0,    0,    0,    0,  129,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  368,    0, 1615,  514,
  720,    0,    0, 2374, 1532,    0,    0,  184,    0,  665,
  173,  238, 1215,    0,    0, 2709,    0,    0,    0,    0,
    0, 2694,  149,    0,    0,    0, 2448,    0, 2697,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 3544,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1262,
    0,    0,    0,  339, 1211, 2714,    0,    0, 2696,    0,
    0,    0,    0, 2718,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2374,  331,    0,    0,    0,    0,    0,
    0,    0, 2699,    0,    0,    0,    0,    0,10692,    0,
    0,    0,    0,    0,  238, 2722,    0,    0,    0,    0,
    0,    0, 1928, 2714,    0,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2765,    0,    0, 2766,
    0,    0,    0,    0,    0,    0,    0,   12,    0, 2773,
    2,  783,   23, -555, 1640,  946, -208,  190, 2758, 2514,
    0,  -38,    0, 2522,  -93,  -31,    0,  390, -254,  -26,
  -54,   -4, -145, 2529, 2130,    0,    0,    0,  529,    0,
    0,  -49, 1722, -524,   35, -897, 2171, 1953, -287, 1731,
 2715, -270,    0,  -57,    0, -458,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -586, -571, 1735, -294,  711, -904,
    0,    0,    0,    0, -320, -113, -637, 1746, -309,  -42,
    0, -510,    0,    0, 1397, -965,    0, 1394,    0,    0,
 2502, -545,  -18,    0,    0,    0,    0, 2475,    0, 2258,
 2513, -190, 1770, -654,    0,    0,    0, -701,    0,    0,
    0,    0,    0,    0, 1847,    0, 2526, 2102, 1788, 2521,
 2535,    0, 1236,    0, -502, 2083,  -55, -627,-1175,    0,
    0,  -83, -922, -911, -910, -189, -907,    0, 1471, -989,
    0, 1166,-1148, 1473,    0, 2322,    0,    0,    0, 1167,
 1172, 1132,   18, -244, -720, -187,    0,    0, 2109,    0,
 1654,    0,    0,   37,    0,    0,    0,    0, -161,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1254, -219,    0,    0,  354,    0,-1130,
  127, 1082, 1246,    0, 1148,    0,    0, 2507, -118, 1910,
 1693, 1520,    0, -988, -812, -836,    0, -479, -159, -659,
    0, -661, 1245,  343, -541, -512, -695, -905,-1191,    0,
    0,    7,    0,    0,    0,    0,    0, 2560,    0,    0,
    0,    0, 2583, 2354,    0,    0,    0,    0,    0, 2591,
    0,    0,    0,    0,  -15,    0,    0,    0, -324, 2090,
 -322, 1660, -453,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2306,    0,    0, 2103,    0,    0,    0, 2510,
 2904,    0,    0,    0,    0,    0, 2628,    0,    0,    0,
    0, 2443,    0, 2442,    0,    0,    0, 2200,    0,    0,
    0, -422,    0,  152,    0, 2187,    0,    0,    0, -576,
-1068, -606,  420,    0, 2807, 1402,    0, 2225, -633, 2237,
    0, -782, 1987,    0,    0, -125, -673,    0, 1341,-1178,
    0,-1118,-1106,-1080, 1464,    0, 1199,    0, 1247,    0,
 1149,    0,    0,    0,    0, 2461,    0,    0,    0, -313,
 2287, 2067, -807,    0,    0,    0, -675,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 4075,
 1583, 1866,    0, -547,    0,    0,    0, -235,    0, 2585,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -286, 1803,    0,   -9,    0,
};
#define YYTABLESIZE 11529
static short yytable[] = {                                     121,
  142,  183,  121,  206,  203,   75,  143,  253,  149,  198,
   76,   37,  390,  273,  276,  189,  300,  202,  272,  275,
  739,  186,  564,  940,  237,  531,  601,  216,  602,  725,
  763,  775,  211,  766,  533,  241,  246,  569,  957,  777,
  218,  182,  185,  188,  143,  193,  745, 1224,  992,  909,
  205,  182,  209, 1363,  188,  242,  143,  193,  386,  222,
  121,  976,  248,  245,  142,  223,  240,  259,  233,  261,
  143,  236,  182,  244,  233,  209,  185,  188, 1395,  143,
  193,  222,  121, 1214,  188,  260, 1143,  247,  855,  478,
 1214,  306,  671,  858, 1215, 1216,  899,  631, 1218,  213,
  626, 1215, 1216,  848,  749, 1218,  304,  978,  839, 1225,
  400,  615,  302,  205,  143,  205,  981,  257,  282,  400,
 1233,  902,  243,  941,  523,  697,  363,  209, 1178,  389,
  633,  325,  301,  687,  188,  143,  222,  498,  572, 1254,
 1249,  626, 1009, 1061,  626,  749,  931,  390,  749,  632,
   74, 1016,  738, 1036,  826, 1271,  352, 1035,  257,  626,
  985,  257,  363,  749,  385,  954,  697,  698,  684, 1069,
  749,  572,  700,  499,  687, 1182,  257,  687,  668, 1132,
  368, 1472,  270,  696,  697, 1081,  723,  329,   74,  633,
  750, 1489,  687,  386,  393,  126,  749,  572,  753,  754,
  755,  756,  751,  678, 1056,  271, 1435,  327,  632,  700,
 1019,  749,  504,  700,  749,  396, 1673,  601, 1041,  602,
   74,  363,  271,  257,  396,  257,  698,  684,  356,  749,
  572,  700,  484,  405, 1568, 1393,  595,  965,  271,  368,
  205,  481,  696,  497,  143,  385,  402,  121,  491, 1272,
  783, 1491,  666,  143,  126,  402,  473,  742,  740,  274,
  734,  569, 1465, 1492,  389,  532, 1144,  242,  700, 1383,
  808, 1733,  535,  185,  203,   74,  548,  143,  700,  554,
  499,  391,  271,  487, 1739,  411, 1431, 1410,  389, 1493,
  309, 1480,  405, 1023,  565,  595,  567, 1430,  528,  385,
  481,  874,  820,  310,  385,  809, 1331, 1575,  242,   86,
  619,  205,   86,  623,  196,  473, 1234,  740,  205,  502,
  205,  611,  205,  621,  357,  205,  818,  613, 1123, 1124,
 1214, 1327,  993,  696,  269, 1266, 1135, 1405,  326,  728,
  391, 1215, 1216,  188,  193, 1218, 1138,  389, 1147, 1149,
  497, 1197,  143,  144,  185,  662, 1092,  528,  141,   90,
  862, 1420, 1042,  862, 1643,  665,  670,  257,  631,  330,
  676,  696,  331,  196,   76,  396, 1238,  797,  529,  684,
  665,  654, 1093,  635,  143,  145,  958,  188,  402,  242,
 1175,  530,  696,  696,  819,  143, 1232, 1647, 1436,  143,
  146, 1393,  188, 1365,  143,  994,  874,  149,  257,  600,
  729,  257,  862, 1239,  798,  327,  390,  332,  626,  749,
 1235,  358,  234,  390,  698,  875,  257,  631, 1489, 1669,
  242, 1502, 1733,  734,  696, 1427,  679,  250,  704,  489,
  626, 1572,  635,  697, 1576, 1226, 1227,   14,  749,  803,
  749,  687,  696,  697, 1062, 1013,  697,  749, 1471, 1211,
  508, 1703, 1139, 1004,  687,  697, 1006,  752,  600,  878,
  633,  906,  721,  687,  734,  390, 1373,  734, 1503, 1277,
 1012,  269, 1538,  698, 1539, 1328,  779,  749, 1491,  632,
  874, 1770,  734,  478, 1278,  698, 1696,  405,  698,  684,
 1492,  265, 1377, 1359,  385,  481, 1792, 1030,  368,  385,
 1359,  368,  697,  253,  473,  385,  805, 1565,  799, 1566,
  687,  697,  400, 1068,  448, 1551, 1493,  700,  478,  687,
  875,  772,  205, 1728,  733,  749,  629,  772,  329, 1577,
  879, 1294, 1265,  126, 1738,  391,  848, 1723,  749,  700,
 1743,  478,  389,  749,  253, 1071,  769,  253,  327,  700,
  369,  825,  473,   73,  203,  697, 1003,  833,  834, 1423,
  749, 1000,  253,  687,  572,  877,  977,  749,  595,  697,
  572, 1076,   76, 1142,  633, 1054, 1482,  687,  667, 1131,
 1765, 1778, 1613, 1408, 1335, 1710,  700, 1615,  804,  269,
  269,   73,  626,  632,  880,  749,  629,  698,  684,  969,
  205,  749,  499,  595,  875,  256,  363, 1000,  257,  851,
  368,  698,  684,  696,  700,  740,  722,   90,  830, 1581,
  849,  269,  368,   73,  368,   76, 1422,  696,  481,  998,
  999,  595,  385,  475,  595,  257, 1650,  257,  665,  700,
  205,  696,  874,  402,  257,  396, 1598,  654,  402,  369,
  595,  629,  196, 1654, 1382,  528,  913,  402, 1655,  684,
  188,  749, 1206,  696,  473,  209, 1500,  269,  391,   86,
  390,  773,  497, 1558, 1397,  389,  595,  203,   73, 1731,
  619,  393, 1809,  491,  874,  975,  948,  734, 1466,  988,
  988, 1564,  475,  369,  205,  698,  188, 1718,  631,  721,
  478,  535,  123,  397,  396, 1442, 1223,  696,  369,  254,
  696,  911,  143,  635,  143,  684,  696,  205, 1231,  696,
  823,  619,  123,  205,  684, 1125, 1267,  205, 1128, 1022,
 1024, 1501,  698, 1236, 1194,  257,  874, 1702, 1253,  619,
 1214,  600,  698,  734,  874,  698, 1709,  205,  367,   57,
  254, 1215, 1216,  254,  698, 1218,  372,  257,  956, 1396,
  390,  499,  397,  696,  257,  393,  875,  390,  254,  124,
  696, 1071,  790,  683,  791,  205,  205, 1567, 1624,  696,
 1002,  253, 1055,  253,  127, 1195,  530, 1409, 1748,  530,
  253, 1741, 1066, 1419, 1602,  487,  102, 1029,  257, 1619,
  392,  698,  137,  390,  600,  313, 1217,  600,  875, 1039,
  698,  143,   80, 1217,  683, 1494,  125, 1467, 1021,  620,
 1129,  143,  336,  600,  393,  103,  104,  151,  105,  934,
  935,  497,  683, 1078,  557,  106,  107,  108,  109,  110,
 1126,  111, 1620,  112,  113,  558, 1127,  102,   76,  600,
 1051, 1052, 1053,  314,  698, 1338,  903, 1167,  257,  392,
  875,  402,  188,  193,  402,  222,  121, 1320,  875, 1322,
 1351, 1130,  402, 1353, 1237,  734,  103,  104,  620,  105,
  152,  253,   14,  734,  402,  402,  106,  107,  108,  109,
  110,  475,  111,  540,  112,  113,  488,  123,  123,  337,
  874,  601,  266,  253, 1621,  150,  822,  138,  874,  396,
  253,  604,  734,  734,  139,  734,  102,  969,  671,  611,
  957,  671,  734,  734,  734,  734,  734,   94,  734,  277,
  734,  734, 1742,  540,  540, 1404,  671,  540,  188,  475,
  780, 1170, 1171,  478,  253,  103,  104,  128,  105,  751,
  726,  489,  540,  618,  540,  106,  107,  108,  109,  110,
  601,  111,  604,  112,  113,  315, 1426,  397,  490,  288,
  450,  596,  910,  936,  475, 1599, 1689,  369,  611,  477,
  369, 1802,  698, 1704,  540,  698,  684,  254,  395,  254,
 1603, 1616,  682,  203, 1359, 1711,  254, 1712,  619,  528,
  393,  710,  136,  601,  253,  602,  711,  390,  505, 1325,
 1325, 1329,  955,  277, 1244, 1620,  698,  394,  956,  530,
  619,  781, 1387,  194,  875, 1690, 1245,  684, 1244,  393,
  596,   76,  875, 1803, 1750,  210, 1691, 1359, 1752,  205,
  528,  698,  396,  528, 1499,  684,  205,  395, 1668, 1364,
  698,  475,  695, 1217,  530, 1473,  278,  698,  528, 1476,
  528,  136,  782,  230,  392,  267,  196,  197, 1424, 1320,
 1682, 1136,  478, 1685, 1260,  698,  394, 1621,  102, 1784,
 1660, 1356, 1379, 1148, 1381, 1172,  385,  254,  783,  369,
  970,  683, 1280,  695, 1755, 1756,  116,  385, 1284,  342,
  397,  369,   73,  369,  683, 1707, 1179,  103,  104,  254,
  105,  695,  402,  683,  402, 1805,  254,  106,  107,  108,
  109,  110,  137,  111,  318,  112,  113, 1573,  353,  454,
  402, 1608,  478,  116,   76,   76,  343,  620,  344,  277,
 1692,  693,  138, 1425,  988,  698,  699,  701,  230,  510,
  254,  540, 1707,  152,  519, 1181,  530, 1609,  511,  620,
  683,  345,  393,  970,  354,  301,  188,  540,  116,  683,
  540,  540,  319,  540,  238, 1497,  102,  540, 1601, 1733,
  540,  137,  907,  228,  671,  671,  346,  629, 1814,  230,
 1350, 1255,  230,  540, 1732,  249,  972,  392, 1707,  540,
  620,  138,  540,  467,  520,  103,  104,  230,  105,  250,
  254,  530,  152,  683, 1610,  106,  107,  108,  109,  110,
 1754,  111, 1434,  112,  113,  254, 1438,  683,  629,  530,
  574,  629, 1707, 1779, 1486,  455, 1255, 1407,  521,  706,
 1256,  620,   76,  601,  707,  255,  629,  467, 1813,  570,
  499,  370,  395,  507, 1187,  863,  522,  528,  863,  620,
 1653,  611,  467, 1079,   74, 1080,  225,  579, 1188,  972,
  530,  540,  576,  530, 1780,  258,  528,  528,  601,  528,
  136,  394,  136,  528,  320, 1259,  528,  671,  530,  574,
  530,  981, 1189,  540,  507,  370,  611,  507,  277,  528,
  540,  505,  321,  226,  540,  528,  601,  863,  570,  601,
  370,  540,  507,  596,  402,   95, 1604,  684,  988,  203,
  497, 1781, 1786, 1787,  611,  601,  579,  611,  671,  540,
  456,  576, 1504,  262,  540, 1325,  322,  710,  708,  857,
  709,  915,  711,  611,  698, 1549, 1190,  698,  596,  188,
  505,  601,  710,  708,  972,  709,   96,  711,  277,  540,
 1651,  277, 1101,  540,  981,  205, 1652,  673,   97,  611,
  695,  973, 1733,  684,  143, 1614,  596,  528, 1102,  596,
  136, 1820,  684,  477,  698,  395,  914, 1795,  499,  263,
  354,  930,  695,   98,  674,  596,  937,  938,  578,  528,
  377,  917,  137,  698,  919,  945,  528,  949,  950,  951,
  952,  953,  698,  945,  394,  528,  660,  528,  477,  264,
  377,  596,  138, 1103, 1760,  268,  242,  390,  230, 1665,
  660,  974,  152, 1667,  412,  528,  102, 1618,  660,  695,
  528,  477,  706,  698,  283, 1648,  505,  707,  695,  354,
  133, 1727, 1104,  152,  660,  299,  413,  578,  497,  308,
   76, 1761,   99,  684,  409,  103,  104, 1600,  105,  528,
  457, 1486,  100, 1217, 1762,  106,  107,  108,  109,  110,
  269,  111,  152,  112,  113,  182, 1484,  530,  674, 1649,
 1708,  311,  695,  412, 1768, 1543, 1646,  101, 1486,   77,
   78,  152, 1200,  530,  974,  629,  695,  968,  660, 1625,
  152, 1319, 1201, 1321, 1662,  413,  152, 1544, 1663, 1105,
  948,  328,  684,  409, 1545,  143,  242,  629,  161,  530,
  185,  507,  674,  507,  152,  530, 1793,  507,  530, 1796,
  507,  711, 1697,  142,  352, 1486,  341,  674,  242,  143,
 1638,  661,  541,  507,  660,  242,  754,  350, 1546, 1082,
 1083, 1087, 1808,  360, 1095, 1097, 1099, 1187, 1107, 1110,
 1113, 1115, 1118, 1121,  339,  706,  362, 1678,  711,  370,
  707, 1188,  370,  754, 1785,  948,  205,  161, 1486,  242,
  706,  711,  541,  541,  682,  707,  541, 1486,  667,  188,
  477,  364,  499,  352,  251, 1189, 1298, 1151, 1152,   44,
 1154,  541, 1156,  541,  365, 1159, 1160, 1161, 1162, 1163,
 1164,  768, 1734,   52,  767,  658,  132,  133,  368,  142,
 1486,  507, 1168, 1486,  938,  143,  499,  499,   96,  667,
  530,  162,  667,  541,  397,  251, 1713,  530,  251,  972,
 1769,   42,  143,  507,   25,  745, 1486,  667,   48, 1190,
  507,  369,  499,  251,  972,  530,  658,   59,   44,  658,
  182, 1736,  497,  499,  976,   98,  499,   73,   60,  102,
  142,  405,   52, 1737,  658,   61,  143, 1713,   76,  976,
   50,  370,  483,  143,  507,  530,  217,  659,   62,  530,
  162, 1790,  128,  370,  497,  370,  497,  497,  103,  104,
   42,  105,  380,   25,  745,  196,  197,   48,  106,  107,
  108,  109,  110, 1484,  111,  142,  112,  113,  406,  525,
  410,  143,  497, 1713,   26, 1031, 1032,  217,  659,  143,
  217,  659,  185,  497,   99,  497,  497,   49,  497,   50,
 1688,  483,  284,  409,  100,  217,  659,  710,  708,   63,
  709,  128,  711,  497, 1776,   49,   51,  412,   46,  185,
  525,  380,  414,  525, 1725,   64,   50, 1078,  129,  101,
  415,  684, 1187,   51,  124,  124,  117,  285,  525,  945,
  525,  416,   65,   26,  684,  185, 1188, 1688, 1799, 1800,
  483, 1301, 1302,  684, 1304, 1306,   49,   66,   52,  674,
  541,   67, 1311,  485,   68,  286,  674,  486,  287,  674,
 1189,  495,   69,  674, 1811,   51,  541,   46,   47,  541,
  541,  674,  541,  217,  288, 1818,  541,  129, 1819,  541,
 1688,   70,  371,  477, 1804,  117,   14,  674,  876, 1775,
  684,  659,  541,  970,  518,  509,  371, 1342,  541,  684,
  289,  541,  526,  898,  371, 1349,  897,  938, 1617,  539,
  938,  674, 1168,   53, 1190,  546, 1357,  938, 1360, 1361,
  371,  549, 1688, 1060,  938, 1688, 1059,   47, 1067,  566,
  562,  844,  217,  947,  970,  970,  970,  970,  970,  970,
  970,  674,  616,  684, 1180,   54,  674, 1179, 1688,   55,
  620,  622,  970,  970,  970,  970,  667,  373, 1198,  667,
  624, 1059, 1205, 1389, 1247, 1204,  667, 1246,  674,  667,
  541,  654, 1252,  667,  371, 1251, 1333,  655,  667, 1332,
  674,  667,  674,  658,  970, 1444,  970,  656, 1179,  663,
  382, 1479,  541,  658, 1478,  659,  658,  667,  373,  541,
  677,  373,  668,  541, 1481,  658, 1791, 1478,  682, 1478,
  541,  680,  477,  375,  376,  377,  373, 1687,  682,  713,
 1817,  667,  497, 1733,  497,  667,  683,  525,  541, 1439,
  684,  497,  685,  541,  667,  706,  686,  687, 1447,  688,
  707,  380,  381,  689,  497, 1452,  525,  525,  690,  525,
  691,  667,  658,  525,  696,  659,  525,  697,  541,  705,
  716,  658,  541,  736, 1687,  659,  714,  718,  659,  525,
  732,  737,  477,  741,  743,  525,  744,  659,  667,  746,
  682,  710,  708,  747,  709,  757,  711,  510,  758,  760,
  667,  761,  938,  770,  794, 1724,  938,  795,  667,  710,
  708,  796,  709,  801,  711,  658,  269, 1687,  806,  810,
  812,  811,  813,  908,  519,  912, 1774,  658,  916,  918,
  920,  815,  497,  816,  659,  827,  828,  667,  667,  821,
  667,  831,  841,  659,  832,  568,  843,  667,  667,  667,
  667,  667,  844,  667,  497,  667,  667,  525,  852, 1687,
  853,  497, 1687,  854,  856,  970,  892,  970,  970,  895,
  893,  900,  964,  904,  520,  970,  905,  932,  153,  525,
  943,  657,  970,  970,  982, 1687,  525,  659,  970,  710,
  708,  970,  709,  510,  711,  497,  979,  525,  986,  659,
 1586, 1587,  511, 1000, 1005,  970,  822, 1011,  521, 1010,
  970,  970,  951, 1017,  970,  525, 1013, 1018,  773, 1026,
  525,  970,  657,  970,  970,  938,  522,  970,  938, 1020,
 1027,  970, 1028, 1034,  970, 1037,  970,  153,  970, 1043,
  657, 1050, 1048,  970,  970, 1057,  970, 1062, 1074,  525,
  373, 1075,  673,  951,  951,  951,  951,  951,  963,  951,
 1089, 1627,  970,  970, 1150,  970, 1134, 1137, 1140, 1132,
 1155,  951,  951,  951,  951, 1153, 1157, 1165,  422,  970,
  535,  970,  970,  970, 1177,  706,  970, 1184,  373,  970,
  970,  710,  708, 1176,  709,  373,  711, 1186,  373,  970,
 1192,  660,  373,  951, 1202,  951, 1207,  374, 1210, 1219,
  373, 1228, 1230,  970,  196, 1248,  970, 1240, 1250,  102,
 1257,  535, 1261, 1661,  535, 1262,  373, 1263, 1264,  706,
 1273, 1664,  970, 1287,  707, 1275, 1068, 1282,  970,  535,
 1285,  535, 1288,  970, 1290, 1291, 1286,  706,  103,  104,
  373,  105,  707, 1295,  970,  970, 1296,  970,  106,  107,
  108,  109,  110,  970,  111, 1297,  112,  113, 1677, 1166,
 1299,  535, 1679,  196, 1300, 1307, 1308, 1309, 1310,  448,
  373, 1312,  428,  874,  712,  710,  708,  716,  709, 1313,
  711, 1314,  970,  970, 1315,  970, 1316, 1317,  430, 1318,
 1352, 1362,  970,  970,  970,  970,  970,  373,  970, 1334,
  970,  970,  375,  376,  377, 1355,  196,  197, 1369,  373,
  981,  373, 1366, 1370, 1384, 1371, 1367,  706,  938, 1368,
 1726, 1386,  707, 1729, 1375,  378,  379,  433,  434,  435,
  380,  381,  436,  437, 1378,  541, 1380, 1398, 1399, 1394,
  439,  440, 1400, 1406, 1401, 1402, 1403,  153, 1451,  444,
 1421,  981,  981,  981,  981,  981,  981,  981, 1411, 1428,
 1432, 1764, 1433, 1437,  951, 1292,  951,  951,  153,  981,
  981,  981,  981, 1443,  951, 1463, 1096, 1468, 1100, 1540,
 1108, 1111,  951, 1116, 1119, 1122, 1464,  951, 1474,  657,
  951, 1495, 1550, 1555, 1553, 1552, 1475,  153, 1556,  657,
 1559,  981,  657,  981,  951, 1554,  951, 1557, 1560,  951,
  951,  657, 1571,  951,  382, 1561,  153, 1569, 1574,  706,
  951, 1579,  951,  951,  707,  153,  951, 1580,  535, 1578,
  951,  153,  271,  951, 1582,  951, 1583,  951, 1584, 1585,
 1588, 1589,  951,  951,  535,  951, 1590,  535,  535,  153,
  535, 1591, 1592, 1593,  535, 1594, 1595,  535,  657, 1596,
 1597,  951,  951, 1605,  951, 1607, 1606,  657, 1623, 1626,
  535,  124, 1628,  530, 1629, 1630,  535,  196,  951,  535,
  951,  951,  951,  196, 1631,  951, 1632, 1633,  951,  951,
 1634, 1635, 1636, 1644,  710,  708, 1641,  709,  951,  711,
 1642, 1645, 1666, 1671,  196,  710,  708, 1672,  709, 1681,
  711,  657,  951,  706,  530,  951, 1637,  530,  707, 1698,
 1706, 1719, 1749,  657,  715,  710,  708, 1730,  709, 1744,
  711,  951,  530,  196,  530, 1751,  196,  951, 1746, 1753,
 1758,  383,  951,  942,  710,  708,  382,  709,  535,  711,
 1759, 1478,  196,  951,  951, 1766,  951, 1773, 1772, 1741,
  196,  196,  951, 1742,  271,  151, 1789,  196, 1794, 1797,
  535, 1798,  981, 1733,  981,  981, 1801,  535, 1812, 1815,
 1816,  535,  981,    1,   30,  196,  626,  203,  535,  749,
  981,  951,  951,   32,  951,  981,  377,  196,  981,  196,
  149,  951,  951,  951,  951,  951,  535,  951,  271,  951,
  951,  535,  981,  650,  978,  269,   82,  981,  981,  528,
  213,  981,  196,  464,  934,  528,  193,  276,  981,  405,
  981,  981,  710,  708,  981,  709,  535,  711,  981,  530,
  535,  981,  933,  981,  528,  981,  682,  196,  680,  676,
  981,  981,  275,  981,  678,  978,  978,  978,  978,  978,
  978,  978,  289,  378,  373,  706,  287,  194,  223,  981,
  981,  102,  981,  978,  978,  978,  978,  324,  288,  707,
  530,  341,  332,  530,  620,  323,  981,  330,  981,  981,
  981,  671,  716,  981,   26,   29,  981,  981,  530,  129,
  103,  104,   39,  105, 1323,  978,  981,  978,  413,  404,
  106,  107,  108,  109,  110,  403,  111,  225,  112,  113,
  981,  374, 1283,  981,  842, 1289, 1073,  894,  239, 1281,
 1279,  530,  706, 1269,  530,  196, 1563,  707, 1562,  981,
 1158,  710,  708,  706,  709,  981,  711, 1258,  707,  528,
  981,  530,  575,  530,  226,  514,  196,  530,  807,  503,
  530,  981,  981,  706,  981, 1336,  710,  708,  707,  709,
  981,  711, 1209,  530,  980,  530, 1270,  196,  530,  530,
  543, 1674,  706,  196, 1337,  710,  708,  707,  709, 1015,
  711, 1542, 1740,  530,  544, 1548,  373, 1747,  789,  981,
  981, 1745,  981, 1783, 1388, 1008,  196, 1680, 1810,  981,
  981,  981,  981,  981, 1695,  981, 1777,  981,  981,  658,
 1199, 1374,  973, 1498,  555,  196,  375,  376,  377, 1705,
  196,  197,  793,  196,  196,  534,  525, 1340,  710,  708,
  196,  709, 1049,  711, 1418,  829,  131, 1040,  496,  378,
  379,  530, 1450,  374,  380,  381,  681,  727,  196,  231,
  731,  971,  984,  973,  973,  973,  973,  973,  973,  973,
  706,  281,  966,  530, 1183,  707,  978, 1670,  978,  978,
  530,  973,  973,  973,  973,  967,  978, 1788, 1611, 1757,
  740,  530,  901, 1821,  978,  196,  657, 1146, 1303,  978,
  231,    0,  978,  231, 1354,    0,  196,    0,    0,  530,
    0,    0,    0,  973,  530,  973,  978,  530,  231,  530,
    0,  978,  978,  530,    0,  978,  530,    0,    0,    0,
    0,    0,  978,    0,  978,  978,    0,    0,  978,  530,
    0,    0,  978,  530,    0,  978,    0,  978,    0,  978,
    0,    0,    0,    0,  978,  978,    0,  978,  375,  376,
  377,    0,  196,  197,    0,    0,    0,  814,    0,    0,
    0,    0,    0,  978,  978,    0,  978,    0,    0,  706,
    0,  378,  379,    0,  707,    0,  380,  381,    0,    0,
  978,    0,  978,  978,  978,    0,    0,  978,    0,    0,
  978,  978,    0,    0,  706,    0,    0,    0,  814,  707,
  978,  814,  530,    0,  530,    0,    0,  530,    0,    0,
    0,  530,    0,  706,  978,    0,  814,  978,  707,    0,
    0, 1341,  710,  708,  530,  709,    0,  711,    0,  530,
    0,    0,    0,  978,    0,    0,  530,    0,    0,  978,
    0,  881,    0,    0,  978, 1343,  710,  708,    0,  709,
    0,  711,    0,    0,    0,  978,  978,    0,  978, 1344,
  710,  708,    0,  709,  978,  711,  706,    0,    0,    0,
  530,  707,    0,    0,  973,    0,  973,  973,    0,    0,
    0,    0,    0,    0,  973,    0,    0,    0,    0,    0,
    0,    0,  973,  978,  978,    0,  978,  973,    0,    0,
  973,    0,  530,  978,  978,  978,  978,  978,    0,  978,
  665,  978,  978,    0,  973,    0,  832,    0,    0,  973,
  973,    0,    0,  973,  530,    0,    0,  242,    0,  231,
  973,  530,  973,  973,    0,    0,  973,    0,    0,    0,
  973,    0,    0,  973,    0,  973,    0,  973,    0,    0,
    0,  665,  973,  973,  665,  973,  884,  832,  832,  832,
  832,  832,    0,  832,    0,  530,    0,    0,    0,  665,
    0,  973,  973,  102,  973,  832,  832,  832,  832,    0,
    0,    0,    0,    0,    0,  181,    0,    0,  973,    0,
  973,  973,  973,    0,    0,  973,    0,    0,  973,  973,
    0,    0,  103,  104,    0,  105,    0,    0,  973,  832,
    0,    0,  106,  107,  108,  109,  110,    0,  111,  814,
  112,  113,  973,    0,    0,  973,  181,  242,    0,  181,
    0,    0,    0, 1345,  710,  708,    0,  709,    0,  711,
    0,  973,    0,    0,  181,  710,  708,  973,  709,  242,
  711,    0,  973,    0,    0,    0,  242,    0,    0,    0,
  706,    0, 1720,  973,  973,  707,  973,    0,    0,    0,
    0,    0,  973,    0,    0,  814,    0,    0,  814,    0,
    0,    0,    0,    0,  706,  814,    0,    0,  814,  707,
  242,  223,  814,    0,    0,    0,    0,  814,  706,    0,
  814,  973,  973,  707,  973,    0,    0,    0,    0,    0,
    0,  973,  973,  973,  973,  973,  814,  973,  662,  973,
  973, 1346,  710,  708,  844,  709,    0,  711,    0,    0,
    0,    0,  223,    0,    0,  223,  710,  708, 1683,  709,
  814,  711,    0,  814,  814,    0,    0,    0,    0,    0,
  223,    0,    0,  814, 1347,  710,  708,    0,  709,  662,
  711,    0,  662,    0,    0,  844,  844,  844,  844,  844,
  814,  844,    0,    0,    0,    0,    0,  662,  832,    0,
  832,  832,    0,  844,  844,  844,  844,    0,  832,    0,
    0,    0,  814,    0,    0,    0,  832,  814,    0,    0,
    0,  832,    0,    0,  832,    0,    0,    0,    0,  814,
    0,  814,    0,    0,    0,    0,    0,  844,  832,    0,
    0,    0,    0,  832,  832,    0,    0,  832,  665,    0,
    0,  665,    0,    0,  832,    0,  832,  832,  665,    0,
  832,  665,    0,  181,  832,  665,    0,  832,    0,  832,
  665,  832,    0,  665,    0,    0,  832,  832,    0,  832,
    0,    0,    0,  251,  181,    0,    0,    0,    0,  665,
    0,    0,  706,    0,    0,  832,  832,  707,  832,    0,
 1348,  710,  708,  706,  709,  181,  711,    0,  707,    0,
    0,  181,  832,  665,  832,  832,  832,  665,    0,  832,
    0,    0,  832,  832,  251,    0,  665,  251,  710,  708,
    0,  709,  832,  711,  181,    0,    0,    0,    0,    0,
    0,    0,  251,  665,    0,    0,  832,    0,    0,  832,
 1469,  710,  708,  181,  709,    0,  711,    0,    0,    0,
  847,    0,  181,    0,    0,  832,    0,    0,  181,    0,
  665,  832,    0,    0,    0,    0,  832, 1470,  710,  708,
  706,  709,  665,  711,    0,  707,  181,  832,  832,  223,
  832,  223,    0,    0,  706,    0,  832,    0,  223,  707,
    0,  847,  847,  847,  847,  847,  844,  847,  844,  844,
    0,    0,    0,  706,    0,  177,  844,    0,  707,  847,
  847,  847,  847,  181,    0,  832,  832,    0,  832,  844,
    0,    0,  844,    0,    0,  832,  832,  832,  832,  832,
    0,  832,    0,  832,  832,    0,  844,    0,    0,    0,
    0,  844,  844,  847,    0,  844,  662,    0,    0,  662,
    0,    0,  844,    0,  844,  844,  662,    0,  844,  662,
    0,    0,  844,  662,  177,  844,    0,  844,  662,  844,
    0,  662,    0,    0,  844,  844,    0,  844,    0,  223,
 1477,  710,  708,    0,  709,    0,  711,  662,    0,    0,
    0,    0,    0,  844,  844,    0,  844,    0,    0,    0,
    0,  223,    0,    0,    0,    0,    0,    0,  223,    0,
  844,  662,  844,  844,  844,  662,   81,  844,    0,    0,
  844,  844,    0,   82,  662,    0,    0,   83,   84,  706,
  844,    0,    0,    0,  707,    0,   85,    0,    0,    0,
    0,  662,  223,    0,  844,    0,    0,  844,   86,    0,
    0,  251,    0,  251,    0,    0,  706,  848,    0,    0,
  251,  707,    0,  844,    0,    0,    0,    0,  662,  844,
    0,    0,    0,    0,  844,    0,    0,    0,    0,  706,
  662,    0,    0,    0,  707,  844,  844,    0,  844,    0,
    0,    0,    0,    0,  844,    0,    0,    0,  848,  848,
  848,  848,  848,    0,  848,    0,  706,    0,    0,    0,
    0,  707,  847,   87,  847,  847,  848,  848,  848,  848,
    0,    0,  847,  844,  844,    0,  844,    0,    0,    0,
   88,    0,    0,  844,  844,  844,  844,  844,  847,  844,
    0,  844,  844,    0,    0,   89,    0,    0,    0,   90,
  848,  251,  847,    0,    0,    0,    0,  847,  847,    0,
   91,  847,    0,  177,    0,    0,    0,    0,  847,    0,
  847,  847,    0,  251,  847,    0,    0,    0,  847,   92,
  251,  847,    0,  847,  177,  847,    0,    0,    0,    0,
  847,  847,    0,  847,    0,    0,    0,    0,  221,    0,
    0,    0,  221,    0,    0,  177,    0,    0,    0,  847,
  847,  177,  847,  719,  251,    0,  221,    0,    0,  706,
  221,    0,    0,  221,  707,    0,  847,    0,  847,  847,
  847,    0,    0,  847,  177,    0,  847,  847,    0,    0,
    0,    0,   95,    0,    0,    0,  847,    0,  187,    0,
    0,    0,    0,  177,    0,    0,    0,    0,    0,    0,
  847,    0,  177,  847,  221,    0,    0,    0,  177,    0,
  221,  221,  221,  842,  221,  221,  221,    0,  221,  847,
    0,    0,    0,   96,    0,  847,  177,    0,    0,    0,
  847,    0,    0,    0,    0,   97,    0,    0,  221,    0,
  221,  847,  847,    0,  847,    0,    0,  187,    0,  126,
  847,    0,    0,    0,  842,    0,  842,  842,  842,  848,
   98,  848,  848,  177,    0,    0,    0,    0,  188,  848,
    0,  221,  842,  842,  842,  842,   81,    0,    0,  847,
  847,    0,  847,    0,    0,  848,    0,    0,  221,  847,
  847,  847,  847,  847,    0,  847,    0,  847,  847,  848,
    0,    0,    0,    0,  848,  848,  842,    0,  848,    0,
  221,    0,    0,    0,    0,  848,    0,  848,  848,    0,
    0,  848,    0,    0,    0,  848,    0,  188,  848,   99,
  848,    0,  848,    0,    0,    0,    0,  848,  848,  100,
  848,    0,    0,    0,    0,  221,  221,    0,    0,  102,
    0,    0,    0,    0,    0,    0,  848,  848,    0,  848,
    0,    0,  221,    0,  101,  921,  922,  923,  924,  925,
  926,  927,  928,  848,    0,  848,  848,  848,  103,  104,
  848,  105,  102,  848,  848,    0,    0,    0,  106,  107,
  108,  109,  110,  848,  111,    0,  112,  113,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  848,    0,    0,
  848,  103,  104,    0,  105,    0,    0,    0,    0,    0,
  841,  106,  107,  108,  109,  110,  848,  111,    0,  112,
  113,    0,  848,    0,    0,    0,    0,  848,    0,    0,
    0,    0,    0,    0,    0,    0,  187,    0,  848,  848,
    0,  848,    0,    0,    0,    0,    0,  848,    0,    0,
    0,  841,    0,  841,  841,  841,    0,  187,    0,    0,
    0,    0,    0,    0,    0,  842,    0,  842,  842,  841,
  841,  841,  841,    0,    0,  842,  848,  848,  187,  848,
    0,    0,    0,    0,  187,    0,  848,  848,  848,  848,
  848,  842,  848,    0,  848,  848,    0,    0,    0,    0,
    0,    0,    0,  841,    0,  842,    0,  187,    0,    0,
  842,  842,    0,    0,  842,    0,  188,    0,    0,    0,
    0,  842,    0,  842,  842,    0,  187,  842,    0,    0,
    0,  842,    0,    0,  842,  187,  842,  188,  842,    0,
    0,  187,    0,  842,  842,    0,  842,    0,  942,  710,
  708,  576,  709,    0,  711,  577,    0,    0,  188,  187,
    0,    0,  842,  842,  188,  842,    0,  872,  871,  873,
    0,    0,    0,  578,    0,    0,  579,    0,    0,  842,
    0,  842,  842,  842,    0,    0,  842,  188,    0,  842,
  842,    0,    0,    0,    0,    0,  187,    0,  541,  842,
    0,    0,    0,    0,    0,    0,  188,    0,    0,    0,
    0,    0,    0,  842,    0,  188,  842,  580,    0,    0,
    0,  188,    0,  581,  582,  583,  843,  584,  585,  586,
    0,  587,  842,    0,    0,    0,    0,    0,  842,  188,
    0,    0,    0,  842,    0,    0,    0,    0,    0,    0,
    0,  588,    0,  589,  842,  842,    0,  842,    0,    0,
    0,    0,    0,  842,    0,    0,    0,  843,    0,  843,
  843,  843,  841,    0,  841,  841,  188,    0,    0,    0,
    0,    0,  841,    0,  590,  843,  843,  843,  843,    0,
    0,    0,  842,  842,    0,  842,    0,    0,  841,    0,
    0,  591,  842,  842,  842,  842,  842,    0,  842,    0,
  842,  842,  841,    0,    0,    0,    0,  841,  841,  843,
    0,  841,    0,  592,    0,    0,    0,    0,  841,    0,
  841,  841,    0,    0,  841,    0,    0,    0,  841,    0,
    0,  841,    0,  841,    0,  841,    0,    0,    0,    0,
  841,  841,    0,  841,    0,    0,    0,    0,  593,  594,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  841,
  841,  102,  841,    0,    0,  595,    0,    0,    0,  859,
    0,    0,    0,    0,    0,    0,  841,  706,  841,  841,
  841,    0,  707,  841,    0,  860,  841,  841,    0,    0,
  103,  104,    0,  105,    0,    0,  841,    0,    0,    0,
  106,  107,  108,  109,  110,    0,  111,    0,  112,  113,
  841,    0,    0,  841,    0,    0,    0,    0,    0,    0,
    0,  861,    0,  846,    0,    0,    0,    0,    0,  841,
  862,    0,    0,    0,  547,  841,    0,    0,  863,    0,
  841,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  841,  841,    0,  841,    0,    0,  864,    0,  865,
  841,    0,    0,    0,  846,    0,  846,  846,  846,    0,
    0,    0,    0,  866,    0,  867,  868,  869,  843,    0,
  843,  843,  846,  846,  846,  846,    0,    0,  843,  841,
  841,    0,  841,    0,    0,    0,    0,    0,    0,  841,
  841,  841,  841,  841,  843,  841,    0,  841,  841,    0,
    0,    0,    0,    0,    0, 1608,  846,    0,  843,    0,
    0,    0,    0,  843,  843,    0,  870,  843,    0,    0,
    0,    0,    0,    0,  843,    0,  843,  843,    0,    0,
  843, 1609,    0,    0,  843,    1,    0,  843,    0,  843,
    0,  843,    0,    0,    0,    0,  843,  843,    0,  843,
    0,    0,    0,    0,    0,    0,    2,    0,    0,    0,
    0,    0,    0,    0,    3,  843,  843,    0,  843,    0,
    0,    4,    0,    5,    0,    6,    0,    0,    0,    0,
    7,    0,  843,    0,  843,  843,  843,    8, 1610,  843,
    0,    0,  843,  843,    0,    0,    0,  102,    0,    0,
    0,    9,  843,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   10,    0,    0,    0,  843,    0,    0,  843,
    0,    0,  102,    0,    0,  639,  103,  104,  445,  105,
  446,    0,    0,    0,    0,  843,  106,  107,  108,  109,
  110,  843,  111,  448,  112,  113,  843,    0,  271,    0,
    0,  103,  104,    0,  105,    0,    0,  843,  843,    0,
  843,  106,  107,  108,  109,  110,  843,  111,    0,  112,
  113,    0,    0,    0,    0,  846,    0,  846,  846,   11,
    0,    0,    0,    0,    0,  846,   12,    0,    0,   13,
    0,   14,   15,    0,    0,  843,  843,    0,  843,    0,
    0,  846,    0,    0,    0,  843,  843,  843,  843,  843,
    0,  843,    0,  843,  843,  846,   16,    0,    0,    0,
  846,  846,  639,    0,  846,  445,    0,  446,    0,    0,
    0,  846,    0,  846,  846,    0,    0,  846,  134,    0,
  448,  846,  134,    0,  846,  271,  846,    0,  846,    0,
    0,    0,    0,  846,  846,    0,  846,    0,    0,    0,
  134,    0,    0,  134,    0,   17,    0,    0,    0,    0,
    0,    0,  846,  846,    0,  846,    0,    0,    0,    0,
   18,    0,    0,    0,    0,    0,    0,    0,    0,  846,
    0,  846,  846,  846,    0,    0,  846,    0,    0,  846,
  846,    0,    0,    0,  134,    0,    0,    0,    0,  846,
  134,  134,  134,    0,  134,  134,  134,    0,  134,    0,
    0,    0,    0,  846,    0,    0,  846,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  134,    0,
  134,    0,  846,    0,    0,    0,    0,    0,  846,    0,
    0,    0,  418,  846,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,  846,  846,    0,  846,    0,    0,
    0,  134,    0,  846,  420,    0,    0,  632,    0,    0,
  373,  421,    0,    0,    0,    0,    0,    0,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  633,
    0,    0,  846,  846,    0,  846,    0,    0,  422,    0,
  134,  423,  846,  846,  846,  846,  846,    0,  846,    0,
  846,  846,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  424,
  425,    0,    0,    0,    0,  134,  134,    0,    0,  418,
    0,  426,    0,    0,    0,  427,    0,    0,  419,    0,
    0,  639,  134,  634,  445,    0,  446,    0,    0,    0,
    0,  420,    0,    0,    0,    0,    0,  373,  421,  448,
    0,    0,    0,    0,  271,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,    0,    0,    0,  422,  635,    0,  423,    0,
    0,    0,  428,    0,    0,    0,    0,    0,  429,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  430,    0,
    0,    0,    0,    0,    0,    0,  424,  425,    0,    0,
    0,    0,  375,  376,  377,  431,  196,  197,  426,    0,
    0,    0,  427,    0,    0,    0,  432,  558,    0,    0,
  634,    0,    0,    0,    0,  378,  379,  433,  434,  435,
  380,  381,  436,  437,  103,  438,    0,  105,    0,   95,
  439,  440,    0,  441,  442,  443,  108,  109,  110,  444,
  111,    0,  112,  113,  636,  637,  638,    0,  558,  700,
    0,  558,    0,  635, 1440, 1441,    0,    0,    0,  428,
 1445, 1446,    0, 1448, 1449,  429,  558,    0,  558,    0,
   96, 1453, 1454, 1455, 1456,  430, 1457, 1458, 1459, 1460,
 1461, 1462,   97,    0,    0,    0,    0,    0,    0,  375,
  376,  377,  431,  196,  197,    0,    0,    0,  558,    0,
    0,    0,    0,  432,    0,    0,    0,   98,    0,    0,
  548,    0,  378,  379,  433,  434,  435,  380,  381,  436,
  437,  103,  438,    0,  105,    0,    0,  439,  440,    0,
  441,  442,  443,  108,  109,  110,  444,  111,  418,  112,
  113,  636,  637,  638,    0,    0,    0,  419,    0,    0,
    0,  548,    0,    0,  548,  532,    0,    0,    0,    0,
  420,    0,    0,    0,    0,    0,  373,  421,    0,  548,
    0,  548,    0,    0,    0,    0,   99,    0,    0,    0,
    0,    0,    0,    0,    0,  633,  100,    0,    0,    0,
    0,    0,    0,    0,  422,    0,  532,  423,    0,  532,
    0,  548,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  101,    0,    0,  532,    0,  532,    0,    0,    0,
    0,    0,    0,    0,    0,  424,  425,    0,    0,  102,
  641,    0,  641,  641,    0,  641,    0,  426,    0,    0,
    0,  427,    0,    0,    0,    0,  532,    0,  641,  634,
    0,    0,    0,  641,    0,    0,    0,    0,  103,  104,
    0,  105,  398,    0,    0,  558,    0,    0,  106,  107,
  108,  109,  110,    0,  111,    0,  112,  113,    0,  102,
    0,  558,    0,    0,    0,  558,    0,  558,    0,    0,
    0,  558,  635,    0,  558,    0,    0,    0,  428,    0,
    0,    0,    0,    0,  429,    0,    0,  558,  103,  104,
    0,  105,    0,  558,  430,    0,  558,    0,  106,  107,
  108,  109,  110,    0,  111,    0,  112,  113,  375,  376,
  377,  431,  196,  197,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,  378,  379,  433,  434,  435,  380,  381,  436,  437,
  103,  438,    0,  105,    0,    0,  439,  440,  548,  441,
  442,  443,  108,  109,  110,  444,  111,    0,  112,  113,
  636,  637,  638,    0,  548,  558,    0,    0,  548,    0,
  548,    0,    0,    0,  548,    0,    0,  548,    0,    0,
  783,    0,    0,    0,    0,    0,    0,  558,    0,    0,
  548,    0,    0,  532,  558,    0,  548,    0,  558,  548,
    0,    0,    0,    0,    0,  558,    0,    0,    0,  532,
    0,    0,    0,  532,    0,  532,    0,    0,    0,  532,
    0,  783,  532,  558,  783,    0,    0,    0,  558,    0,
  641,    0,    0,    0,    0,  532,    0,  641,    0,  783,
    0,  532,    0,    0,  532,    0,  641,    0,    0,    0,
    0,    0,    0,  558,    0,    0,    0,  558,    0,  641,
    0,    0,    0,    0,    0,  641,  641,    0,  548,  639,
    0,  639,  639,  641,  639,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  639,    0,    0,
  548,    0,  639,  641,    0,    0,  641,  548,    0,    0,
    0,  548,    0,    0,    0,    0,    0,    0,  548,    0,
    0,    0,    0,  532,    0,    0,    0,    0,    0,    0,
    0,    0,  641,    0,  641,  641,  548,    0,    0,    0,
    0,  548,    0,    0,    0,  532,  641,    0,    0,    0,
  641,    0,  532,    0,    0,    0,  532,    0,    0,    0,
    0,  394,    0,  532,    0,    0,  548,    0,    0,    0,
  548,    0,    0,    0,    0,    0,    0,  447,    0,    0,
  445,  532,  446,    0,    0,    0,  532,  102,    0,    0,
    0,    0,    0,    0,    0,  448,    0,    0,    0,    0,
  271,    0,    0,    0,    0,    0,    0,  641,    0,    0,
  424,  532,    0,  641,    0,  532,  103,  104,    0,  105,
    0,    0,    0,  641,    0,    0,  106,  107,  108,  109,
  110,    0,  111,    0,  112,  113,    0,  641,  641,  641,
  641,  641,  641,    0,    0,    0,    0,    0,    0,    0,
    0,  641,  783,    0,    0,    0,    0,    0,    0,    0,
  641,  641,  641,  641,  641,  641,  641,  641,  641,  641,
  641,    0,  641,    0,    0,  641,  641,    0,  641,  641,
  641,  641,  641,  641,  641,  641,    0,  641,  641,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,    0,    0,  639,    0,  783,    0,
    0,  783,    0,    0,   73,  639,  431,    0,  783,    0,
    0,  783,    0,    0,    0,  783,    0,  432,  639,    0,
  783,    0,    0,  783,  639,  639,    0,    0,    0,    0,
  102,    0,  639,    0,    0,  103,  438,    0,  105,  783,
    0,    0,    0,    0,  441,  442,  443,  108,  109,  110,
    0,  111,  639,  112,  113,  639,    0,    0,    0,  103,
  104,    0,  105,  783,    0,    0,  783,  783,    0,  106,
  107,  108,  109,  110,    0,  111,  783,  112,  113,    0,
    0,  639,    0,  639,  639,    0,    0, 1084,    0,    0,
 1085,    0,    0,  783,  418,  639,    0,    0,    0,  639,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  783,  420,    0,    0,    0,
  783,  447,  373,  421,  445,    0,  446,    0,  568,    0,
    0,    0,  783,    0,  783,    0,    0,    0,    0,  448,
    0,    0,    0,    0,  271,    0,    0,    0,    0,    0,
  422,    0,    0,  423,    0,    0,  639,  568,  568,    0,
  568,    0,  639,    0,    0,    0,    0,  568,  568,  568,
  568,  568,  639,  568,    0,  568,  568,    0,    0,    0,
    0,  424,  425,    0,    0,    0,  639,  639,  639,  639,
  639,  639,    0,  426,    0,    0,    0,  427,    0,    0,
  639,    0,    0,    0,    0,    0,    0,    0,    0,  639,
  639,  639,  639,  639,  639,  639,  639,  639,  639,  639,
    0,  639,    0,    0,  639,  639,  281,  639,  639,  639,
  639,  639,  639,  639,  639,    0,  639,  639,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  881,    0, 1086,    0,  428,  281,  281,    0,  281,    0,
  429,    0,    0,    0,    0,  281,  281,  281,  281,  281,
  430,  281,    0,  281,  281,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  375,  376,  377,  431,  196,  197,
    0,    0,    0,    0,    0,    0,    0,    0,  432,    0,
    0,    0,    0,    0,    0,    0,    0,  378,  379,  433,
  434,  435,  380,  381,  436,  437,  103,  438,    0,  105,
    0,    0,  439,  440,  882,  441,  442,  443,  108,  109,
  110,  444,  111,    0,  112,  113,    0,    0,    0,    0,
    0, 1094,    0,    0, 1085,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
  883,    0,    0,    0,    0,  884,    0,    0,    0,    0,
  420,  885,    0,    0,    0,  447,  373,  421,  445,    0,
  446,    0,  102,    0,    0,    0,    0,    0,    0,    0,
  886,    0,    0,  448,    0,    0,    0,    0,  271,    0,
    0,    0,    0,    0,  422,    0,    0,  423,    0,    0,
    0,  103,  104,    0,  105,    0,  784,    0,    0,    0,
    0,  106,  107,  108,  109,  110,    0,  111,    0,  112,
  113,    0,    0,    0,    0,  424,  425,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  426,    0,    0,
    0,  427,    0,    0,    0,    0,    0,  784,    0,  447,
  784,    0,  445,    0,  446,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  784,    0,  448,    0,    0,
    0,    0,  271,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1086,    0,  428,    0,
    0,    0,    0,    0,  429,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  430,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  375,  376,
  377,  431,  196,  197,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,  378,  379,  433,  434,  435,  380,  381,  436,  437,
  103,  438,    0,  105,  845,    0,  439,  440,  577,  441,
  442,  443,  108,  109,  110,  444,  111,    0,  112,  113,
    0,    0,  846,    0,    0, 1098,  578,    0, 1085,  579,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  420,    0,    0,    0,    0,    0,
  373,  421,    0,    0,    0,    0,    0,    0,    0,    0,
  580,    0,    0,    0,    0,    0,  581,  582,  583,    0,
  584,  585,  586,    0,  587,    0,    0,    0,  422,    0,
    0,  423,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  588,    0,  589,    0,  784, 1106,
    0,    0, 1085,    0,    0,    0,  418,    0,    0,  424,
  425,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,  426,    0,    0,    0,  427,    0,  590,  420,    0,
    0,    0,    0,  447,  373,  421,  445,    0,  446,    0,
    0,    0,    0,    0,  591,    0,    0,    0,    0,    0,
    0,  448,    0,    0,  784,    0,  271,  784,    0,    0,
    0,    0,  422,    0,  784,  423,  592,  784,    0,    0,
    0,  784,    0,    0,    0,    0,  784,    0,    0,  784,
 1086,    0,  428,    0,    0,    0,    0,    0,  429,    0,
    0,    0,    0,  424,  425,  784,    0,    0,  430,    0,
    0,  593,  594,    0,    0,  426,    0,    0,    0,  427,
    0,    0,  375,  376,  377,  431,  196,  197,  595,  784,
    0,    0,  784,  784,    0,    0,  432,    0,    0,    0,
    0,    0,  784,    0,    0,  378,  379,  433,  434,  435,
  380,  381,  436,  437,  103,  438,    0,  105,    0,  784,
  439,  440,    0,  441,  442,  443,  108,  109,  110,  444,
  111,    0,  112,  113, 1086,    0,  428,    0,    0,    0,
    0,  784,  429,    0,    0,    0,  784,    0,    0,    0,
    0,    0,  430,    0,    0,    0,    0,    0,  784,    0,
  784,    0,    0,    0,    0,    0,  375,  376,  377,  431,
  196,  197,    0,    0,    0,    0,    0,    0,    0,    0,
  432,    0,    0,    0,    0,    0,    0,    0,    0,  378,
  379,  433,  434,  435,  380,  381,  436,  437,  103,  438,
    0,  105,    0,    0,  439,  440,    0,  441,  442,  443,
  108,  109,  110,  444,  111,    0,  112,  113,    0,    0,
    0,    0,    0, 1109,    0,    0, 1085,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,    0,    0,    0,  447,  373,  421,
  445,    0,  446,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  448,    0,    0,    0,    0,
  271,    0,    0,    0,    0,    0,  422,    0,    0,  423,
    0,    0,  155,    0,    0,    0,    0,    0,  808,    0,
    0,  156,  157,  158,  159,  160,  161,  162,  163,  164,
  165,  166,  167,  168,  169,  170,  171,  424,  425,  172,
  173,  174,  175,    0,  176,    0,    0,  177,  178,  426,
    0,    0,    0,  427,    0,    0,    0,    0,    0,  808,
    0,  447,  808,    0,  445,    0,  446,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  808,    0,  448,
    0,    0,    0,    0,  271,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1086,    0,
  428,    0,    0,    0,    0,    0,  429,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  375,  376,  377,  431,  196,  197,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,    0,    0,  378,  379,  433,  434,  435,  380,  381,
  436,  437,  103,  438,    0,  105,  221,    0,  439,  440,
  221,  441,  442,  443,  108,  109,  110,  444,  111,    0,
  112,  113,    0,    0,  221,    0,    0, 1114,  221,    0,
 1085,  221,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
    0,    0,  373,  421,    0,    0,    0,    0,    0,    0,
    0,  765,  221,    0,    0,    0,    0,    0,  221,  221,
  221,    0,  221,  221,  221,    0,  221,    0,    0,    0,
  422,    0,    0,  423,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,    0,  221,    0,
  808, 1117,  765,    0, 1085,  765,    0,    0,  418,    0,
    0,  424,  425,    0,    0,    0,    0,  419,    0,    0,
  765,    0,    0,  426,    0,    0,    0,  427,    0,  221,
  420,    0,    0,    0,    0,  447,  373,  421,  445,    0,
  446,    0,    0,    0,    0,    0,  221,    0,    0,    0,
    0,    0,    0,  448,    0,    0,  808,    0,  271,  808,
    0,    0,    0,    0,  422,    0,  808,  423,  221,  808,
    0,    0,    0,  808,    0,    0,    0,    0,  808,    0,
    0,  808, 1086,    0,  428,    0,    0,    0,    0,    0,
  429,    0,    0,    0,    0,  424,  425,  808,    0,    0,
  430,    0,    0,  221,  221,    0,    0,  426,    0,    0,
    0,  427,    0,    0,  375,  376,  377,  431,  196,  197,
  221,  808,    0,    0,  808,  808,    0,    0,  432,    0,
    0,    0,    0,    0,  808,    0,    0,  378,  379,  433,
  434,  435,  380,  381,  436,  437,  103,  438,    0,  105,
    0,  808,  439,  440,    0,  441,  442,  443,  108,  109,
  110,  444,  111,    0,  112,  113, 1086,    0,  428,    0,
    0,    0,    0,  808,  429,    0,    0,    0,  808,    0,
    0,    0,    0,    0,  430,    0,    0,    0,    0,    0,
  808,    0,  808,    0,    0,    0,    0,    0,  375,  376,
  377,  431,  196,  197,    0,    0,    0,    0,    0,    0,
  787,    0,  432,    0,    0,    0,    0,    0,    0,    0,
    0,  378,  379,  433,  434,  435,  380,  381,  436,  437,
  103,  438,    0,  105,    0,    0,  439,  440,    0,  441,
  442,  443,  108,  109,  110,  444,  111,    0,  112,  113,
    0,  787,    0,    0,  787, 1120,    0,    0, 1085,    0,
    0,    0,  418,    0,    0,  447,    0,  476,  445,  787,
  446,  419,    0,    0,    0,    0,    0,    0,    0,  765,
    0,    0,  765,  448,  420,    0,    0,    0,  271,  765,
  373,  421,  765,    0,    0,    0,  765,    0,    0,    0,
    0,  765,    0,    0,  765,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  422,    0,
  765,  423,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  710,  708,    0,  709,    0,
  711,    0,    0,    0,  765,    0,    0,  765,  765,  424,
  425,    0,    0,  872,  871,  873,    0,  765,    0,    0,
    0,  426,    0,  447,    0,  427,  445,    0,  446,    0,
    0,    0,    0,    0,  765,    0,    0,    0,    0,    0,
    0,  448,    0,    0,    0,    0,  271,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  765,    0,    0,    0,
    0,  765,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  765,    0,  765,    0,    0,    0,    0,
 1086,    0,  428,    0,    0,    0,    0,    0,  429,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  430,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  375,  376,  377,  431,  196,  197,    0,    0,
    0,    0,    0,    0,    0,    0,  432,    0,    0,    0,
    0,    0,  787,    0,    0,  378,  379,  433,  434,  435,
  380,  381,  436,  437,  103,  438,    0,  105,    0,    0,
  439,  440,  418,  441,  442,  443,  108,  109,  110,  444,
  111,  419,  112,  113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  420,    0,    0,    0,    0,    0,
  373,  421,    0,    0,    0,    0,    0,    0,  787,    0,
    0,  787,    0,    0,    0,    0,    0,    0,  787,    0,
    0,  787,    0,    0,    0,  787,    0,    0,  422,    0,
  787,  423,    0,  787,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  859,    0,    0,    0,  787,
    0,    0,    0,  706,    0,    0,    0,  374,  707,  424,
  425,  860,    0,    0,    0,    0,    0,    0,    0,    0,
  418,  426,    0,  787,    0,  427,  787,  787,    0,  419,
    0,    0,  945,    0,  945,  945,  787,  945,    0,    0,
    0,    0,  420,    0,    0,    0,    0,  861,  373,  421,
  945,    0,    0,  787,    0,  945,  862,    0,    0,    0,
    0,    0,    0,    0,  863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  787,  422,    0,    0,  423,
  787,    0,  428,  864,    0,  865,    0,    0,  429,    0,
    0,    0,  787,    0,  787,    0,    0,    0,  430,  866,
    0,  867,  868,  869,    0,  374,    0,  424,  425,    0,
    0,    0,  375,  376,  377,  431,  196,  197,    0,  426,
    0,    0,    0,  427,    0,    0,  432,    0,    0,    0,
    0,    0,    0,    0,    0,  378,  379,  433,  434,  435,
  380,  381,  436,  437,  103,  438,    0,  105,    0,    0,
  439,  440,  870,  441,  442,  443,  108,  109,  110,  444,
  111,    0,  112,  113,    0,    0,    0,    0,    0,    0,
  700,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,    0,    0,    0,    0,    0,  429,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  375,  376,  377,  431,  196,  197,    0,    0,    0,    0,
    0,    0,    0,    0,  432,    0,    0,    0,    0,    0,
    0,    0,    0,  378,  379,  433,  434,  435,  380,  381,
  436,  437,  103,  438,    0,  105,    0,    0,  439,  440,
    0,  441,  442,  443,  108,  109,  110,  444,  111,  945,
  112,  113,    0,    0,    0,    0,    0,    0,  945,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  945,    0,    0,    0,    0,  641,  945,  945,  641,
    0,  641,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  641,    0,    0,    0,    0,  641,
    0,    0,    0,    0,    0,  945,    0,    0,  945,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  945,    0,  945,  945,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  945,    0,
    0,    0,  945,    0,    0,    0,    0,    0,  447,    0,
    0,  445,    0,  446,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  448,    0,    0,    0,
    0,  271,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  945,
    0,    0,    0,    0,    0,  945,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  945,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  945,
  945,  945,  945,  945,  945,    0,    0,    0,    0,    0,
    0,    0,    0,  945,    0,    0,    0,    0,    0,    0,
    0,    0,  945,  945,  945,  945,  945,  945,  945,  945,
  945,  945,  945,    0,  945,    0,    0,  945,  945,    0,
  945,  945,  945,  945,  945,  945,  945,  945,    0,  945,
  945,    0,    0,    0,    0,    0,  641,    0,    0,    0,
    0,    0,    0,  641,    0,    0,    0,    0,    0,    0,
  785,    0,  641,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  641,    0,    0,    0,    0,
    0,  641,  641,    0,    0,    0,    0,    0,    0,  641,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  785,    0,    0,  785,    0,    0,    0,    0,  641,
    0,    0,  641,    0,    0,    0,    0,    0,    0,  785,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
  641,  641,    0,    0,  419,    0,    0,    0,    0,    0,
  447,    0,  641,  445,    0,  446,  641,  420,    0,    0,
    0,    0,    0,  373,  421,    0,    0,    0,  448,    0,
    0,    0,    0,  271,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  422,    0,    0,  423,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  641,    0,    0,    0,    0,    0,  641,
    0,    0,  424,  425,    0,    0,    0,    0,    0,  641,
    0,    0,  447,    0,  426,  445,    0,  446,  427,    0,
    0,    0,    0,  641,  641,  641,  641,  641,  641,    0,
  448,    0,    0,    0,    0,  271,    0,  641,    0,    0,
    0,    0,    0,    0,    0,    0,  641,  641,  641,  641,
  641,  641,  641,  641,  641,  641,  641,    0,  641,    0,
    0,  641,  641,    0,  641,  641,  641,  641,  641,  641,
  641,  641,    0,  641,  641,  428,    0,    0,    0,    0,
    0,  429,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,    0,    0,    0,    0,    0,    0,  692,
    0,    0,  785,    0,    0,  375,  376,  377,  431,  196,
  197,    0,    0,    0,    0,    0,    0,    0,    0,  432,
    0,    0,    0,    0,    0,    0,    0,    0,  378,  379,
  433,  434,  435,  380,  381,  436,  437,  103,  438,    0,
  105,    0,    0,  439,  440,    0,  441,  442,  443,  108,
  109,  110,  444,  111,    0,  112,  113,  418,  785,    0,
    0,  785,    0,    0,    0,    0,  419,    0,  785,    0,
    0,  785,    0,    0,    0,  785,    0,    0,    0,  420,
  785,    0,    0,  785,    0,  373,  421,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  785,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  422,    0,    0,  423,    0,    0,    0,
    0,    0,    0,  785,    0,    0,  785,  785,    0,    0,
    0,    0,    0,    0,    0,    0,  785,    0,    0,  418,
    0,    0,    0,    0,  424,  425,    0,    0,  419,    0,
    0,    0,    0,  785,  447,  944,  426,  445,    0,  446,
  427,  420,    0,    0,    0,    0,    0,  373,  421,    0,
    0,    0,  448,    0,    0,  785,    0,  271,    0,    0,
  785,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  785,    0,  785,  422,    0,    0,  423,    0,
    0,    0,    0,    0,    0,    0,    0,  700,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  428,    0,    0,
    0,    0,    0,  429,  374,    0,  424,  425,    0,    0,
    0,    0,    0,  430,    0,    0,  447,    0,  426,  445,
    0,  446,  427,    0,    0,    0,    0,  375,  376,  377,
  431,  196,  197,    0,  448,    0,    0,    0,    0,  271,
    0,  432,    0,    0,    0,    0,    0,    0,    0,    0,
  378,  379,  433,  434,  435,  380,  381,  436,  437,  103,
  438,    0,  105,    0,    0,  439,  440,    0,  441,  442,
  443,  108,  109,  110,  444,  111,    0,  112,  113,  428,
    0,    0,    0,    0,    0,  429,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  430,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  375,
  376,  377,  431,  196,  197,    0,    0,    0,    0,    0,
    0,    0,    0,  432,    0,    0,    0,    0,    0,    0,
    0,    0,  378,  379,  433,  434,  435,  380,  381,  436,
  437,  103,  438,    0,  105,    0,    0,  439,  440,    0,
  441,  442,  443,  108,  109,  110,  444,  111,    0,  112,
  113,  418,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,    0,  550,    0,    0,  373,
  421,    0,    0,    0,    0,  156,  157,  158,  159,  160,
  161,  162,  163,  164,  165,  166,  167,  168,  169,  170,
  171,    0,    0,  172,  173,  551,  552,  422,  553,    0,
  423,  177,  178,    0,    0,  106,  107,  108,  109,  110,
    0,  111,    0,  112,  113,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,  424,  425,
    0,    0,  419,    0,    0,    0,    0,    0,  447,    0,
  426,  445,    0,  446,  427,  420,    0,    0,    0,    0,
    0,  373,  421,    0,    0,    0,  448,    0,    0,    0,
    0,  271,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  422,
    0,    0,  423,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  428,    0,    0,    0,    0,    0,  429,    0,    0,
  424,  425,    0,    0,    0,    0,    0,  430,    0,    0,
  447,    0,  426,  445,    0,  446,  427,    0,    0,    0,
    0,  375,  376,  377,  431,  196,  197,    0,  448,    0,
    0,    0,    0,  271,    0,  432,    0,    0,    0,    0,
    0,    0,    0,    0,  378,  379,  433,  434,  435,  380,
  381,  436,  437,  103,  438,    0,  105,    0,    0,  439,
  440,    0,  441,  442,  443,  108,  109,  110,  444,  111,
    0,  112,  113,  428,    0,    0,    0,    0,    0,  429,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  430,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1112,    0,    0,  375,  376,  377,  431,  196,  197,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,  378,  379,  433,  434,
  435,  380,  381,  436,  437,  103,  438,    0,  105,    0,
    0,  439,  440,    0,  441,  442,  443,  108,  109,  110,
  444,  111,    0,  112,  113,  418,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,    0,
  460,    0,    0,  373,  421,    0,    0,    0,    0,    0,
  460,  460,  460,  460,  460,  460,  460,  460,  460,  460,
  460,  460,  460,  460,  460,    0,    0,  460,  460,  460,
  460,  422,  460,    0,  423,  460,  460,    0,    0,  460,
  460,  460,  460,  460,    0,  460,    0,  460,  460,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,  424,  425,    0,    0,  419,    0,    0,    0,
    0,    0,  447,    0,  426,  445,    0,  446,  427,  420,
    0,    0,    0,    0,    0,  373,  421,    0,    0,    0,
  448, 1676,    0,    0,    0,  271,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  422,    0,    0,  423,    0,    0,    0,
    0,    0,    0,    0,    0, 1292,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  428,    0,    0,    0,    0,
    0,  429,    0,    0,  424,  425,    0,    0,    0,    0,
    0,  430,    0,    0, 1721,    0,  426,  445,    0,  446,
  427,    0,    0,    0,    0,  375,  376,  377,  431,  196,
  197,    0,  448,    0,    0,    0,    0,  271,    0,  432,
    0,    0,    0,    0,    0,    0,    0,    0,  378,  379,
  433,  434,  435,  380,  381,  436,  437,  103,  438,    0,
  105,    0,    0,  439,  440,    0,  441,  442,  443,  108,
  109,  110,  444,  111,    0,  112,  113,  428,    0,    0,
    0,    0,    0,  429,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  430,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1305,    0,    0,  375,  376,  377,
  431,  196,  197,    0,    0,    0,    0,    0,    0,    0,
    0,  432,    0,    0,    0,    0,    0,    0,    0,    0,
  378,  379,  433,  434,  435,  380,  381,  436,  437,  103,
  438,    0,  105,    0,    0,  439,  440,    0,  441,  442,
  443,  108,  109,  110,  444,  111,    0,  112,  113,  418,
    0,    0,    0,    0,    0,    0,  786,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,    0,    0,    0,    0,  373,  421,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  786,    0,    0,
  786,    0,    0,    0,    0,  422,    0,    0,  423,    0,
    0,    0,    0,    0,    0,  786,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,  424,  425,    0,    0,
  419,    0,    0,    0,    0,    0,  447,    0,  426,  445,
    0,  446,  427,  420,    0,    0,    0,    0,    0,  373,
  421,    0,    0,    0,  448,    0,    0,    0,    0,  271,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  422,    0,    0,
  423,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  428,
    0,    0,    0,    0,    0,  429,  374,    0,  424,  425,
    0,    0,    0,    0,    0,  430,    0,    0,  945,    0,
  426,  945,    0,  945,  427,    0,    0,    0,    0,  375,
  376,  377,  431,  196,  197,    0,  945,    0,    0,    0,
    0,  945,    0,  432,    0,    0,    0,    0,    0,    0,
    0,    0,  378,  379,  433,  434,  435,  380,  381,  436,
  437,  103,  438,    0,  105,    0,    0,  439,  440,    0,
  441,  442,  443,  108,  109,  110,  444,  111,    0,  112,
  113,  428,    0,    0,    0,    0,    0,  429,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  430,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  786,    0,
    0,  375,  376,  377,  431,  196,  197,    0,    0,    0,
    0,    0,    0,    0,    0,  432,    0,    0,    0,    0,
    0,    0,    0,    0,  378,  379,  433,  434,  435,  380,
  381,  436,  437,  103,  438,    0,  105,    0,    0,  439,
  440,    0,  441,  442,  443,  108,  109,  110,  444,  111,
    0,  112,  113,  418,  786,    0,  816,  786,    0,    0,
    0,    0,  419,    0,  786,    0,    0,  786,    0,    0,
    0,  786,    0,    0,    0,  420,  786,    0,    0,  786,
    0,  373,  421,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  786,    0,  816,    0,    0,
  816,    0,    0,    0,    0,    0,    0,    0,    0,  422,
    0,    0,  423,    0,    0,  816,    0,    0,    0,  786,
    0,    0,  786,  786,    0,    0,    0,    0,  780,    0,
    0,    0,  786,    0,    0,  945,    0,    0,    0,  781,
  424,  425,    0,    0,  945,    0,    0,    0,    0,  786,
    0,    0,  426,    0,    0,    0,  427,  945,    0,    0,
    0,    0,  782,  945,  945,    0,    0,    0,    0,  780,
    0,  786,  780,    0,    0,    0,  786,    0,    0,    0,
  781,    0,    0,  781,    0,    0,    0,  780,  786,    0,
  786,  945,    0,    0,  945,    0,    0,    0,  781,    0,
    0,    0,    0,  782,    0,    0,  782,    0,    0,    0,
    0,    0,    0,  428,    0,    0,    0,    0,    0,  429,
    0,  782,  945,  945,    0,    0,    0,    0,    0,  430,
    0,    0,    0,    0,  945,    0,    0,    0,  945,    0,
    0,    0,    0,  375,  376,  377,  431,  196,  197,    0,
    0,    0,    0,    0,    0,    0,    0,  432,    0,    0,
    0,    0,    0,    0,    0,    0,  378,  379,  433,  434,
  435,  380,  381,  436,  437,  103,  438,    0,  105,    0,
    0,  439,  440,    0,  441,  442,  443,  108,  109,  110,
  444,  111,    0,  112,  113,  945,    0,    0,    0,    0,
  815,  945,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  945,    0,    0,    0,    0,    0,    0,  816,    0,
    0,  809,    0,    0,    0,  945,  945,  945,  945,  945,
  945,    0,    0,    0,    0,    0,    0,    0,    0,  945,
    0,  815,    0,    0,  815,    0,    0,    0,  945,  945,
  945,  945,  945,  945,  945,  945,  945,  945,  945,  815,
  945,    0,  809,  945,  945,  809,  945,  945,  945,  945,
  945,  945,  945,  945,  816,  945,  945,  816,    0,    0,
  809,    0,    0,    0,  816,    0,    0,  816,    0,    0,
  780,  816,    0,    0,    0,    0,  816,    0,    0,  816,
    0,  781,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  816,    0,    0,    0,    0,
    0,    0,    0,    0,  782,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  816,
    0,    0,  816,  816,    0,    0,  780,    0,  817,  780,
    0,    0,  816,    0,    0,    0,  780,  781,    0,  780,
  781,    0,    0,  780,    0,    0,    0,  781,  780,  816,
  781,  780,    0,    0,  781,    0,    0,    0,    0,  781,
  782,    0,  781,  782,    0,    0,    0,  780,    0,  817,
  782,  816,  817,  782,    0,    0,  816,  782,  781,    0,
    0,    0,  782,    0,    0,  782,    0,  817,  816,    0,
  816,  780,  818,    0,  780,  780,    0,    0,    0,    0,
    0,  782,  781,    0,  780,  781,  781,    0,    0,    0,
    0,    0,    0,    0,    0,  781,    0,    0,    0,    0,
    0,  780,    0,    0,  806,  782,    0,    0,  782,  782,
    0,    0,  781,  818,    0,    0,  818,    0,  782,    0,
    0,    0,  815,  780,    0,    0,    0,    0,  780,    0,
    0,  818,    0,    0,  781,  782,    0,    0,    0,  781,
  780,    0,  780,  809,    0,  806,  810,    0,  806,    0,
    0,  781,    0,  781,    0,    0,    0,  782,    0,    0,
    0,    0,  782,  806,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  782,    0,  782,    0,  815,    0,
    0,  815,    0,    0,    0,    0,    0,  810,  815,    0,
  810,  815,    0,    0,    0,  815,    0,    0,    0,  809,
  815,    0,  809,  815,    0,  810,    0,    0,    0,  809,
  819,    0,  809,    0,    0,    0,  809,    0,    0,  815,
    0,  809,    0,    0,  809,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  809,    0,  807,  815,    0,    0,  815,  815,    0,    0,
    0,  819,    0,    0,  819,    0,  815,    0,    0,    0,
    0,    0,    0,    0,  809,    0,    0,  809,  809,  819,
  817,    0,    0,  815,    0,    0,    0,  809,    0,    0,
    0,    0,    0,  807,    0,    0,  807,  811,    0,    0,
    0,    0,    0,    0,  809,  815,    0,    0,    0,    0,
  815,  807,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  815,    0,  815,    0,  809,    0,    0,    0,
    0,  809,    0,    0,    0,    0,  817,    0,  811,  817,
    0,  811,    0,  809,  818,  809,  817,    0,    0,  817,
    0,    0,    0,  817,    0,    0,  811,    0,  817,    0,
    0,  817,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  806,  817,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  818,  817,    0,  818,  817,  817,    0,    0,    0,    0,
  818,    0,    0,  818,  817,    0,    0,  818,  810,    0,
    0,    0,  818,    0,    0,  818,    0,    0,    0,    0,
    0,  817,  806,    0,    0,  806,    0,    0,    0,    0,
    0,  818,  806,    0,    0,  806,    0,    0,    0,  806,
    0,    0,    0,  817,  806,    0,    0,  806,  817,    0,
    0,    0,    0,    0,    0,  818,    0,    0,  818,  818,
  817,    0,  817,  806,  810,    0,    0,  810,  818,    0,
    0,    0,  819,    0,  810,    0,    0,  810,    0,    0,
    0,  810,    0,    0,    0,  818,  810,  806,    0,  810,
  806,  806,    0,    0,    0,    0,    0,    0,    0,    0,
  806,    0,    0,    0,  807,  810,    0,  818,    0,    0,
    0,    0,  818,    0,    0,    0,    0,  806,    0,    0,
    0,    0,    0,    0,  818,    0,  818,    0,  819,  810,
    0,  819,  810,  810,    0,    0,    0,    0,  819,  806,
    0,  819,  810,    0,  806,  819,    0,    0,    0,  811,
  819,    0,    0,  819,    0,    0,  806,    0,  806,  810,
  807,    0,    0,  807,    0,    0,    0,    0,    0,  819,
  807,    0,  321,  807,    0,    0,    0,  807,    0,    0,
    0,  810,  807,    0,    0,  807,  810,    0,    0,    0,
    0,    0,    0,  819,    0,    0,  819,  819,  810,    0,
  810,  807,  321,    0,    0,  811,  819,    0,  811,  321,
    0,    0,    0,  321,  321,  811,  321,    0,  811,  321,
    0,    0,  811,  819,    0,  807,    0,  811,  807,  807,
  811,    0,    0,  321,    0,    0,    0,    0,  807,  321,
    0,    0,    0,    0,    0,  819,  811,    0,    0,    0,
  819,    0,    0,    0,    0,  807,    0,    0,    0,    0,
    0,    0,  819,    0,  819, 1386,    0,    0,    0,    0,
  811,    0,    0,  811,  811,    0,    0,  807,    0,    0,
    0,    0,  807,  811,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  807,    6,  807,    0,    0,  321,
  811,    0,    0,    0,    0,    0, 1505, 1506,    0, 1507,
    0,    0, 1508,    0,    0,    0, 1386,    0,  321,    0,
    0,    0,  811,    0,    0,    0, 1509,  811,    0,    0,
    0,    0, 1510,  321,    0,    0,    0,    0,    0,  811,
    0,  811,    0,  321,    0,    0,    6,    0,    0,    0,
    0,    0,  321,    0,  321,    0,    0, 1505, 1506,    0,
 1507,  321,    0, 1508,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1509,    0,    0,
    0,    0,    0, 1510,    0,    0,    0,    0,    0,    0,
  321,  321, 1511,  321,    0,    0,    0,    0,    0,    0,
  321,  321,  321,  321,  321,    0,  321,    0,  321,  321,
    0,   14,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1512,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,  576,    0,
    0,    0,  577, 1511,    0, 1637,    0, 1513,    0,  773,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
  578,    0,   14,  579,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1512,    0,    0,
    0,    0,    0, 1514,  104,    0,  105,   16,    0,    0,
    0,    0,    0,  106,  107,  108,  109,  110, 1513,  111,
    0,  112, 1515,  576,  580,  102,    0,  577,    0,    0,
  581,  582,  583,    0,  584,  585,  586,    0,  587,    0,
    0,    0,    0,    0,    0,  578,    0,    0,  579,    0,
    0,    0,    0,    0, 1514,  104,    0,  105,  588,    0,
  589,    0,    0,    0,  106,  107,  108,  109,  110,    0,
  111,    0,  112, 1515,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  580,
    0,  590,    0,    0,    0,  581,  582,  583,    0,  584,
  585,  586,    0,  587,    0,    0,    0,    0,  591,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  588,    0,  589,    0,    0,    0,    0,
  592,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  102,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  590,    0,    0,    0,
    0,    0,    0,    0,    0,  593,  594,    0,    0,    0,
    0,    0,    0,  591,  103,  104,    0,  105,    0,    0,
    0,    0,  595,    0,  106,  107,  108,  109,  110,    0,
  111,    0,  112,  113,    0,  592,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  593,  594,    0,    0,    0,    0,    0,    0,    0,  103,
  104,    0,  105,    0,    0,    0,    0,  595,    0,  106,
  107,  108,  109,  110,    0,  111,    0,  112,  113,
};
static short yycheck[] = {                                       9,
   16,   52,   12,   61,   60,    4,   16,  101,   18,   59,
    4,    0,  248,  132,  133,   54,  142,   60,  132,  133,
  500,   53,  343,  697,   82,  320,  351,   66,  351,  483,
  533,  542,   64,  536,  322,   86,   91,  347,  714,  542,
   67,   51,   52,   53,   54,   55,  505, 1013,  750,  683,
   60,   61,   62, 1184,   64,   87,   66,   67,  248,   69,
   70,  737,   94,   90,   80,   70,   85,  123,   78,  125,
   80,   81,   82,   89,   84,   85,   86,   87, 1227,   89,
   90,   91,   92, 1006,   94,  124,  899,   92,  634,  279,
 1013,  146,  406,  639, 1006, 1006,  673,  368, 1006,   65,
    0, 1013, 1013,  628,    0, 1013,  145,  741,  621, 1014,
  256,  356,  144,  123,  124,  125,  744,    0,  137,  265,
 1025,  677,   88,  700,  315,    0,  220,  137,  941,  248,
    0,  181,  142,    0,  144,  145,  146,  299,    0, 1044,
 1038,   41,  770,  839,   44,   41,  692,  383,   44,    0,
   45,  779,   44,  808,   40, 1061,  206,   44,   41,   59,
  747,   44,    0,   59,  248,  713,   41,    0,    0,   40,
    0,    0,    0,  299,   41,  958,   59,   44,   44,   44,
    0,   41,   40,    0,   59,   41,   45,   59,   45,   59,
   40, 1370,   59,  383,  250,    0,   44,   59,  519,  520,
  521,  522,  512,  412,  832,   63,  274,   59,   59,    0,
  782,   41,  306,   41,   44,  254,   61,  542,   45,  542,
   45,   59,   63,   44,  263,   44,   59,   59,  211,   59,
   59,   59,  283,    0, 1426, 1225,    0,  717,   63,   59,
  250,    0,   59,  299,  254,    0,  256,  257,  287, 1062,
  545, 1370,  398,  263,   59,  265,    0,  502,    0,   40,
   44,  571, 1331, 1370,  383,  321,  900,  278,   59,   41,
   40,   44,  328,  283,  330,   45,  332,  287,   41,  335,
  406,    0,   63,  283,   44,  263, 1275, 1253,    0, 1370,
  260, 1360,   59,  796,  344,   59,  346,  260,    0,  383,
   59,  262,  597,  152,   59,  576, 1143,  355,  319,   41,
  360,  321,   44,  364,    0,   59,  338,   59,  328,  302,
  330,  353,  332,  362,  268,  335,  277,  354,  874,  875,
 1253, 1139,  356,    0,  264, 1056,  892, 1242,  257,  377,
   59, 1253, 1253,  353,  354, 1253,  320,   59,  904,  905,
  406,  979,  362,  402,  364,  394,  352,   59,   16,   41,
   41, 1259,  816,   44, 1540,  397,  405,    0,    0,  258,
  409,   41,  261,   59,  368,  414,  301,  301,  287,   41,
  412,   44,  378,    0,  394,  434,   40,  397,  398,  400,
  936,  300,   59,    0,  345,  405, 1024, 1546,  466,  409,
  449, 1391,  412, 1186,  414,  429,  262,  417,   41,    0,
  448,   44,   93,  338,  338,  334,    0,  306,  318,  269,
  442,  365,   80,  659,    0,  386,   59,   59, 1607, 1605,
  441,   41,   44,    0,   41, 1272,  414,   40,  448,  439,
  340, 1430,   59,  318,  492, 1017, 1018,  417,  278,  563,
  280,  318,   59,  328,  417,  300,  331,  287,  318, 1005,
  309, 1653,  436,  758,  331,  340,  761,  517,   59,  659,
  340,  680,  482,  340,   41,   59, 1197,   44, 1383, 1066,
  775,  411, 1387,   59, 1389, 1140,  542,  383, 1607,  340,
  262,  264,   59,  683, 1066,  328, 1645,  264,  331,  331,
 1607,  320, 1204, 1177,  259,  264,   40,  802,  328,  264,
 1184,  331,  387,    0,  258,  270,  566, 1423,  442, 1424,
  387,  396,  668,  394,   58,   41, 1607,  318,  718,  396,
  386,  541,  542,   41,  318,  383,  431,  547,  410, 1437,
  659, 1089, 1055,  348,  304,  264, 1071, 1678,  378,  340,
 1699,  741,  264,  383,   41,  843,  539,   44,  410,  387,
  452,  447,  306,  458,  620,  440,  757,  617,  618, 1265,
  400,  458,   59,  440,  436,  659,  738,  407,  342,  454,
  409,  852,  576,  897,  454,  830, 1369,  454,  454,  454,
 1721, 1740, 1497, 1248, 1150, 1664,  387, 1502,  564,  458,
  458,  458,  502,  454,  660,  501,  431,  440,  440,  723,
  620,  441,  738,  377,  386,  436,  454,  458,  501,  629,
  440,  454,  454,  440,  387,  367,  485,  309,  611, 1442,
  629,  458,  452,  458,  454,  629, 1264,  454,  397,  753,
  754,  405,  397,    0,  408,  278, 1551,  280,  680,  440,
  660,  318,  262,  663,  287,    0, 1464,  320,  668,    0,
  424,  431,  348, 1568, 1210,  367,  685,  677,   40,  331,
  680,  501,  993,  340,  418,  685, 1378,  458,  397,  411,
  264,  284,  738, 1404, 1230,  397,  450,  743,  458,   41,
    0,  747,  304,  732,  262,  734,  706,  264, 1332,  749,
  750, 1422,   59,   44,  714,   41,  716, 1673,  340,  719,
  900,  767,  273,    0,   59, 1292, 1011,  387,   59,    0,
  387,   42,  732,  340,  734,  387,  396,  737, 1023,  396,
   91,   41,  286,  743,  396,  881, 1057,  747,  884,  795,
  796, 1379,  318,  301,  405,  378,  262, 1652, 1043,   59,
 1673,  342,  328,  320,  262,  331, 1661,  767,  230,  455,
   41, 1673, 1673,   44,  340, 1673,  238,  400,   42, 1228,
 1006,  897,   59,  440,  407,    0,  386, 1013,   59,  340,
  387, 1069,  465,    0,  467,  795,  796, 1425,  313,  396,
  756,  278,  831,  280,   12,  456,   41, 1251, 1703,   44,
  287,  301,  841, 1257, 1478,  283,  460,  801,  441,  402,
    0,  387,  325,  397,  405,  258, 1006,  408,  386,  813,
  396,  831,  320, 1013,   41, 1371,  273, 1338,  794,    0,
  885,  841,  257,  424,   59,  489,  490,  387,  492,  307,
  308,  897,   59,  853,  259,  499,  500,  501,  502,  503,
  882,  505,  445,  507,  508,  270,  883,  460,  852,  450,
  826,  827,  828,  306,  440, 1153,  677,  435,  501,   59,
  386,  881,  882,  883,  884,  885,  886, 1132,  386, 1134,
 1167,  886,  892, 1170,  442,  452,  489,  490,   59,  492,
  440,  378,  417,  460,  904,  905,  499,  500,  501,  502,
  503,  258,  505,    0,  507,  508,  384,  461,  462,  334,
  262,    0,  130,  400,  507,   59,  277,  430,  262,  264,
  407,  405,  489,  490,  437,  492,  460, 1041,   41,    0,
 1606,   44,  499,  500,  501,  502,  503,  402,  505,  260,
  507,  508,  442,   40,   41, 1240,   59,   44,  958,  306,
  319,  307,  308, 1143,  441,  489,  490,   12,  492, 1269,
  405,  439,   59,  348,   61,  499,  500,  501,  502,  503,
   59,  505,  456,  507,  508,  418, 1271,  264,  456,  424,
  270,    0,  303,  451,  274, 1465,  263,  328,   59,  279,
  331,  300,  328, 1655,   91,  331,    0,  278,    0,  280,
 1480, 1504,  343, 1059, 1678, 1665,  287, 1667,  318,    0,
 1066,   42,    0, 1338,  501, 1338,   47, 1253,  308, 1138,
 1139, 1140,  296,  260, 1034,  445,    0,    0,   42,  274,
  340,  400, 1220,  409,  386,  312, 1035,   41, 1048,  264,
   59, 1035,  386,  352, 1706,  261,  323, 1721, 1708, 1059,
   41,  387,  397,   44, 1375,   59, 1066,   59, 1604, 1185,
  396,  418,    0, 1253,  309, 1352,  303,   41,   59, 1356,
   61,   59,  441,  322,  264,  130,  461,  462, 1266, 1334,
 1626,  892, 1272, 1629, 1050,   59,   59,  507,  460, 1751,
 1570,  435, 1206,  904, 1208,  451,  259,  378, 1393,  440,
  397,  318, 1068,   41, 1711, 1712,  265,  270, 1074,  356,
  397,  452,  458,  454,  331, 1657,   44,  489,  490,  400,
  492,   59, 1132,  340, 1134, 1785,  407,  499,  500,  501,
  502,  503,    0,  505,  258,  507,  508, 1432,  402,  436,
 1150,  343, 1332,  302, 1138, 1139,  393,  318,  395,  260,
  427,  441,    0, 1267, 1204,  445,  446,  447,    0,  348,
  441,  258, 1704,    0,  279,   93,  411,  369,  357,  340,
  387,  418,  397,  470,  438, 1185, 1186,  274,  337,  396,
  277,  278,  306,  280,  322, 1373,  460,  284, 1475,   44,
  287,   59,  303,  458,  307,  308,  443,    0, 1805,   41,
 1166,   44,   44,  300,   59,  401,  397,  397, 1750,  306,
    0,   59,  309,    0,  329,  489,  490,   59,  492,   40,
  501,  466,   59,  440,  426,  499,  500,  501,  502,  503,
 1710,  505, 1282,  507,  508,  383, 1286,  454,   41,    0,
    0,   44, 1784,  377, 1370,  436,   44, 1246,  363,  280,
   93,   41, 1246,  342,  285,  383,   59,   44, 1804,    0,
 1386,    0,  264,    0,  321,   41,  381,  258,   44,   59,
 1565,  342,   59,  854,   45,  856,  265,    0,  335,  470,
   41,  378,    0,   44,  418,   44,  277,  278,  377,  280,
  278,  264,  280,  284,  418,   93,  287,  410,   59,   59,
   61,  397,  359,  400,   41,   44,  377,   44,  260,  300,
  407,   44,  436,  302,  411,  306,  405,   93,   59,  408,
   59,  418,   59,  342, 1334,  260,  383,  331, 1378, 1385,
 1386,  465, 1755, 1756,  405,  424,   59,  408,  451,  436,
  436,   59, 1385,  385,  441, 1464,  470,   42,   43,  639,
   45,  303,   47,  424,  328, 1394,  413,  331,  377, 1369,
   93,  450,   42,   43,  403,   45,  301,   47,  260,  466,
 1558,  260,  272,  470,  470, 1385, 1564,  417,  313,  450,
  318,  420,   44,  387, 1394, 1499,  405,  378,  288,  408,
  378, 1814,  396,  683,    0,  397,  686,   59, 1524,  383,
    0,  691,  340,  338,  444,  424,  696,  697,    0,  400,
  278,  303,  280,  387,  303,  705,  407,  707,  708,  709,
  710,  711,  396,  713,  397,  416,  321,  418,  718,  383,
  278,  450,  280,  333,  337,  436,  278, 1673,  280, 1599,
  335,  397,  279, 1603,    0,  436,  460, 1505,  343,  387,
  441,  741,  280,   59,  337, 1549,  746,  285,  396,   59,
  494, 1681,  362,  300,  359,  418,    0,   59, 1524,  274,
 1464,  374,  407,    0,    0,  489,  490, 1471,  492,  470,
  436, 1607,  417, 1673,  387,  499,  500,  501,  502,  503,
  458,  505,  329,  507,  508, 1505, 1370,  258,    0, 1550,
 1660,  318,  440,   59, 1724,  378, 1545,  442, 1634,  316,
  317,  348,  493,  274,  470,  318,  454,  485,  413, 1508,
  357, 1132,  503, 1134, 1574,   59,  363,  400, 1578,  429,
 1540,   40,   59,   59,  407, 1545,  378,  340,    0,  300,
 1550,  278,   44,  280,  381,  306, 1766,  284,  309, 1769,
  287,  337, 1646, 1569,    0, 1681,  461,   59,  400, 1569,
 1524,   41,    0,  300,   44,  407,  417,  264,  441,  859,
  860,  861, 1792,  316,  864,  865,  866,  321,  868,  869,
  870,  871,  872,  873,  424,  280,  318, 1619,  374,  328,
  285,  335,  331,  444, 1754, 1605, 1606,   59, 1724,  441,
  280,  387,   40,   41,  343,  285,   44, 1733,    0, 1619,
  900,  337, 1738,   59,    0,  359,  311,  907,  908,    0,
  910,   59,  912,   61,  460,  915,  916,  917,  918,  919,
  920,   41, 1690,    0,   44,    0,  493,  494,  336, 1655,
 1766,  378,  932, 1769,  934, 1655, 1772, 1773,  301,   41,
  411,    0,   44,   91,  402,   41, 1672,  418,   44,   46,
  340,    0, 1672,  400,    0,    0, 1792,   59,    0,  413,
  407,  452, 1798,   59,   61,  436,   41,  294,   59,   44,
 1690, 1691, 1738, 1809,   46,  338, 1812,  458,  305,  460,
 1706,  340,   59, 1692,   59,  312, 1706, 1713, 1692,   61,
    0,  440,    0, 1713,  441,  466,    0,    0,  325,  470,
   59, 1762,    0,  452,    0,  454, 1772, 1773,  489,  490,
   59,  492,    0,   59,   59,  461,  462,   59,  499,  500,
  501,  502,  503, 1607,  505, 1751,  507,  508,   40,    0,
  318, 1751, 1798, 1759,    0,  391,  392,   41,   41, 1759,
   44,   44, 1762, 1809,  407,   41, 1812,    0,   44,   59,
 1634,   59,  342,  320,  417,   59,   59,   42,   43,  386,
   45,   59,   47,   59, 1738,  294,    0,  402,    0, 1789,
   41,   59,  383,   44,   59,  402,  305, 1797,    0,  442,
  383,  318,  321,  312,  461,  462,    0,  377,   59, 1089,
   61,  506,  419,   59,  331, 1815,  335, 1681, 1772, 1773,
  436, 1101, 1102,  340, 1104, 1105,   59,  434,  337,  321,
  258,  438, 1112,  361,  441,  405,  328,  450,  408,  331,
  359,  434,  449,  335, 1798,   59,  274,   59,    0,  277,
  278,  343,  280,    0,  424, 1809,  284,   59, 1812,  287,
 1724,  468,  321, 1143,  383,   59,  417,  359,   41, 1733,
  387,   44,  300,    0,  357,  442,  335, 1157,  306,  396,
  450,  309,  287,   41,  343, 1165,   44, 1167,   59,  411,
 1170,  383, 1172,  402,  413,  469, 1176, 1177, 1178, 1179,
  359,   44, 1766,   41, 1184, 1769,   44,   59,   41,  372,
   61,   44,   59,   40,   41,   42,   43,   44,   45,   46,
   47,  413,  286,  440,   41,  434,  418,   44, 1792,  438,
   40,  264,   59,   60,   61,   62,  318,    0,   41,  321,
   40,   44,   41, 1223,   41,   44,  328,   44,  440,  331,
  378,  461,   41,  335,  413,   44,   41,  461,  340,   44,
  452,  343,  454,  318,   91,   41,   93,  461,   44,  436,
   45,   41,  400,  328,   44,   44,  331,  359,   41,  407,
  320,   44,   44,  411,   41,  340,   41,   44,  343,   44,
  418,  402, 1272,  457,  458,  459,   59, 1634,   40,   91,
   41,  383,  278,   44,  280,  387,   40,  258,  436, 1289,
   40,  287,   40,  441,  396,  280,   40,   40, 1298,   40,
  285,  485,  486,   40,  300, 1305,  277,  278,   40,  280,
   40,  413,  387,  284,   40,  318,  287,   40,  466,   40,
  320,  396,  470,  428, 1681,  328,   46,   44,  331,  300,
   44,   46, 1332,   61,   40,  306,  264,  340,  440,   44,
  343,   42,   43,  382,   45,   44,   47,  348,  278,  278,
  452,  300, 1352,  264,  397,  340, 1356,  436,  460,   42,
   43,  470,   45,  397,   47,  440,  458, 1724,  277,  447,
  399,  447,   40,  682,  279,  684, 1733,  452,  687,  688,
  689,  349,  378,   40,  387,   40,   40,  489,  490,   91,
  492,  318,  383,  396,  264,  300,  411,  499,  500,  501,
  502,  503,   44,  505,  400,  507,  508,  378,  389, 1766,
  382,  407, 1769,   40,   40,  262,  436,  264,  265,  317,
  464,   40,  452,  320,  329,  272,  320,  451,    0,  400,
   41,    0,  448,  280,  502, 1792,  407,  440,  285,   42,
   43,  288,   45,  348,   47,  441,  264,  418,  454,  452,
 1450, 1451,  357,  458,   40,  302,  277,   40,  363,  274,
  307,  308,    0,  344,  311,  436,  300,  344,  284,  386,
  441,  318,   41,  320,  321, 1475,  381,  324, 1478,  278,
  386,  328,  386,  418,  331,  416,  333,   59,  335,  300,
   59,   40,  418,  340,  341,  356,  343,  417,   40,  470,
  295,  274,  417,   41,   42,   43,   44,   45,   46,   47,
   40, 1511,  359,  360,  320,  362,   44,  385,  367,   44,
   41,   59,   60,   61,   62,  264,   44,  320,  323,  376,
    0,  378,  379,  380,   44,  280,  383,   40,  321,  386,
  387,   42,   43,   44,   45,  328,   47,   44,  331,  396,
  328,   44,  335,   91,  505,   93,  266,  352,   40,  299,
  343,  501,   40,  410,    0,  422,  413,  264,   41,  460,
   58,   41,   41, 1573,   44,   41,  359,   41,  264,  280,
  454, 1581,  429,   41,  285,  440,  394,  309,  435,   59,
  492,   61,   41,  440,   40,   40,  309,  280,  489,  490,
  383,  492,  285,  352,  451,  452,   40,  454,  499,  500,
  501,  502,  503,  460,  505,   40,  507,  508, 1618,  320,
   40,   91, 1622,   59,   40,   40,   40,   40,   40,   58,
  413,   40,  427,  262,   41,   42,   43,  320,   45,   40,
   47,   40,  489,  490,   40,  492,   40,   40,  443,  327,
  435,   41,  499,  500,  501,  502,  503,  440,  505,   44,
  507,  508,  457,  458,  459,  308,  461,  462,  343,  452,
    0,  454,  388,  274,  446,  331,  388,  280, 1678,  388,
 1680,  271,  285, 1683,  422,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  390,  287,  391,  386,  386,  407,
  495,  496,  386,   41,  386,  386,  386,  279,  311,  504,
   41,   41,   42,   43,   44,   45,   46,   47,  378,  278,
   40, 1721,   41,   41,  262,  417,  264,  265,  300,   59,
   60,   61,   62,   41,  272,  385,  864,   41,  866,  280,
  868,  869,  280,  871,  872,  873,  436,  285,  308,  318,
  288,  396,  337,  301,  338,  442,  435,  329,  338,  328,
   41,   91,  331,   93,  302,  442,  304,  301,   41,  307,
  308,  340,  385,  311,   45,  352,  348,  320,  371,  280,
  318,   41,  320,  321,  285,  357,  324,   41,  258,  371,
  328,  363,   63,  331,   41,  333,   41,  335,   41,   41,
   41,   41,  340,  341,  274,  343,   41,  277,  278,  381,
  280,   41,   41,   41,  284,   41,   41,  287,  387,   41,
   41,  359,  360,  280,  362,   44,   46,  396,   59,   40,
  300,  340,   59,    0,   40,   59,  306,    0,  376,  309,
  378,  379,  380,  279,   59,  383,   59,   59,  386,  387,
  340,   59,  308,  352,   42,   43,   59,   45,  396,   47,
   41,  344,   41,  369,  300,   42,   43,   40,   45,  340,
   47,  440,  410,  280,   41,  413,  451,   44,  285,  383,
   44,   59,   41,  452,   41,   42,   43,   59,   45,  383,
   47,  429,   59,  329,   61,  343,   59,  435,  383,   41,
   41,   40,  440,   41,   42,   43,   45,   45,  378,   47,
   44,   44,  348,  451,  452,  410,  454,  304,  435,  301,
  356,  357,  460,  442,   63,  387,   40,  363,   59,  293,
  400,  304,  262,   44,  264,  265,  463,  407,  307,   44,
   41,  411,  272,    0,  383,  381,  337,   40,  418,  264,
  280,  489,  490,  383,  492,  285,  278,  393,  288,  395,
  458,  499,  500,  501,  502,  503,  436,  505,  264,  507,
  508,  441,  302,  320,    0,  264,  411,  307,  308,  389,
   40,  311,  418,  470,  451,  309,  458,  271,  318,  264,
  320,  321,   42,   43,  324,   45,  466,   47,  328,    0,
  470,  331,  435,  333,  436,  335,  343,  443,  343,  343,
  340,  341,  271,  343,  343,   41,   42,   43,   44,   45,
   46,   47,  308,   41,  295,   40,  308,  458,   59,  359,
  360,  460,  362,   59,   60,   61,   62,   59,  308,   41,
   41,  304,   59,   44,   41,   59,  376,   59,  378,  379,
  380,   44,   41,  383,    0,    0,  386,  387,   59,   12,
  489,  490,    0,  492, 1135,   91,  396,   93,  265,  258,
  499,  500,  501,  502,  503,  257,  505,  265,  507,  508,
  410,  352, 1071,  413,  624,  262,  844,  668,   84, 1069,
 1066,  258,  280, 1058,    0,  258, 1413,  285, 1412,  429,
   41,   42,   43,  280,   45,  435,   47, 1048,  285,  318,
  440,  278,  348,  280,  302,  313,  279,  284,  571,  304,
  287,  451,  452,  280,  454,   41,   42,   43,  285,   45,
  460,   47,  996,  300,  743,   41, 1059,  300,   44,  306,
  330, 1616,  280,  306,   41,   42,   43,  285,   45,  777,
   47, 1391, 1697,   59,  330, 1393,  295, 1701,  547,  489,
  490, 1700,  492, 1742, 1221,  767,  329, 1624, 1797,  499,
  500,  501,  502,  503, 1639,  505, 1739,  507,  508,  383,
  981, 1199,    0, 1374,  335,  348,  457,  458,  459, 1655,
  461,  462,  549,  356,  357,  323,  316,   41,   42,   43,
  363,   45,  823,   47, 1255,  610,   13,  815,  291,  480,
  481,  378,  262,  352,  485,  486,  417,  485,  381,    0,
  489,  732,  746,   41,   42,   43,   44,   45,   46,   47,
  280,  135,  718,  400,  958,  285,  262, 1607,  264,  265,
  407,   59,   60,   61,   62,  719,  272, 1759, 1495, 1713,
  500,  418,  676, 1815,  280,  418,  382,  901, 1103,  285,
   41,   -1,  288,   44, 1172,   -1,  429,   -1,   -1,  436,
   -1,   -1,   -1,   91,  441,   93,  302,  278,   59,  280,
   -1,  307,  308,  284,   -1,  311,  287,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,  300,
   -1,   -1,  328,  470,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,  457,  458,
  459,   -1,  461,  462,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,  280,
   -1,  480,  481,   -1,  285,   -1,  485,  486,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,  280,   -1,   -1,   -1,   41,  285,
  396,   44,  278,   -1,  280,   -1,   -1,  378,   -1,   -1,
   -1,  287,   -1,  280,  410,   -1,   59,  413,  285,   -1,
   -1,   41,   42,   43,  300,   45,   -1,   47,   -1,  400,
   -1,   -1,   -1,  429,   -1,   -1,  407,   -1,   -1,  435,
   -1,  328,   -1,   -1,  440,   41,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,  451,  452,   -1,  454,   41,
   42,   43,   -1,   45,  460,   47,  280,   -1,   -1,   -1,
  441,  285,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  280,  489,  490,   -1,  492,  285,   -1,   -1,
  288,   -1,  378,  499,  500,  501,  502,  503,   -1,  505,
    0,  507,  508,   -1,  302,   -1,    0,   -1,   -1,  307,
  308,   -1,   -1,  311,  400,   -1,   -1,  278,   -1,  280,
  318,  407,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   41,  340,  341,   44,  343,  443,   41,   42,   43,
   44,   45,   -1,   47,   -1,  441,   -1,   -1,   -1,   59,
   -1,  359,  360,  460,  362,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  396,   93,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,  262,
  507,  508,  410,   -1,   -1,  413,   41,  378,   -1,   44,
   -1,   -1,   -1,   41,   42,   43,   -1,   45,   -1,   47,
   -1,  429,   -1,   -1,   59,   42,   43,  435,   45,  400,
   47,   -1,  440,   -1,   -1,   -1,  407,   -1,   -1,   -1,
  280,   -1,   59,  451,  452,  285,  454,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,   -1,   -1,  280,  328,   -1,   -1,  331,  285,
  441,    0,  335,   -1,   -1,   -1,   -1,  340,  280,   -1,
  343,  489,  490,  285,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  359,  505,    0,  507,
  508,   41,   42,   43,    0,   45,   -1,   47,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   42,   43,   44,   45,
  383,   47,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,  396,   41,   42,   43,   -1,   45,   41,
   47,   -1,   44,   -1,   -1,   41,   42,   43,   44,   45,
  413,   47,   -1,   -1,   -1,   -1,   -1,   59,  262,   -1,
  264,  265,   -1,   59,   60,   61,   62,   -1,  272,   -1,
   -1,   -1,  435,   -1,   -1,   -1,  280,  440,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   93,  302,   -1,
   -1,   -1,   -1,  307,  308,   -1,   -1,  311,  318,   -1,
   -1,  321,   -1,   -1,  318,   -1,  320,  321,  328,   -1,
  324,  331,   -1,  258,  328,  335,   -1,  331,   -1,  333,
  340,  335,   -1,  343,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,    0,  279,   -1,   -1,   -1,   -1,  359,
   -1,   -1,  280,   -1,   -1,  359,  360,  285,  362,   -1,
   41,   42,   43,  280,   45,  300,   47,   -1,  285,   -1,
   -1,  306,  376,  383,  378,  379,  380,  387,   -1,  383,
   -1,   -1,  386,  387,   41,   -1,  396,   44,   42,   43,
   -1,   45,  396,   47,  329,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  413,   -1,   -1,  410,   -1,   -1,  413,
   41,   42,   43,  348,   45,   -1,   47,   -1,   -1,   -1,
    0,   -1,  357,   -1,   -1,  429,   -1,   -1,  363,   -1,
  440,  435,   -1,   -1,   -1,   -1,  440,   41,   42,   43,
  280,   45,  452,   47,   -1,  285,  381,  451,  452,  278,
  454,  280,   -1,   -1,  280,   -1,  460,   -1,  287,  285,
   -1,   41,   42,   43,   44,   45,  262,   47,  264,  265,
   -1,   -1,   -1,  280,   -1,    0,  272,   -1,  285,   59,
   60,   61,   62,  418,   -1,  489,  490,   -1,  492,  285,
   -1,   -1,  288,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  507,  508,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   93,   -1,  311,  318,   -1,   -1,  321,
   -1,   -1,  318,   -1,  320,  321,  328,   -1,  324,  331,
   -1,   -1,  328,  335,   59,  331,   -1,  333,  340,  335,
   -1,  343,   -1,   -1,  340,  341,   -1,  343,   -1,  378,
   41,   42,   43,   -1,   45,   -1,   47,  359,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
  376,  383,  378,  379,  380,  387,  305,  383,   -1,   -1,
  386,  387,   -1,  312,  396,   -1,   -1,  316,  317,  280,
  396,   -1,   -1,   -1,  285,   -1,  325,   -1,   -1,   -1,
   -1,  413,  441,   -1,  410,   -1,   -1,  413,  337,   -1,
   -1,  278,   -1,  280,   -1,   -1,  280,    0,   -1,   -1,
  287,  285,   -1,  429,   -1,   -1,   -1,   -1,  440,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  280,
  452,   -1,   -1,   -1,  285,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,  280,   -1,   -1,   -1,
   -1,  285,  262,  402,  264,  265,   59,   60,   61,   62,
   -1,   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,
  419,   -1,   -1,  499,  500,  501,  502,  503,  288,  505,
   -1,  507,  508,   -1,   -1,  434,   -1,   -1,   -1,  438,
   93,  378,  302,   -1,   -1,   -1,   -1,  307,  308,   -1,
  449,  311,   -1,  258,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,  400,  324,   -1,   -1,   -1,  328,  468,
  407,  331,   -1,  333,  279,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,  300,   -1,   -1,   -1,  359,
  360,  306,  362,  264,  441,   -1,  291,   -1,   -1,  280,
  295,   -1,   -1,  298,  285,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,  329,   -1,  386,  387,   -1,   -1,
   -1,   -1,  260,   -1,   -1,   -1,  396,   -1,    0,   -1,
   -1,   -1,   -1,  348,   -1,   -1,   -1,   -1,   -1,   -1,
  410,   -1,  357,  413,  339,   -1,   -1,   -1,  363,   -1,
  345,  346,  347,    0,  349,  350,  351,   -1,  353,  429,
   -1,   -1,   -1,  301,   -1,  435,  381,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  313,   -1,   -1,  373,   -1,
  375,  451,  452,   -1,  454,   -1,   -1,   59,   -1,  327,
  460,   -1,   -1,   -1,   41,   -1,   43,   44,   45,  262,
  338,  264,  265,  418,   -1,   -1,   -1,   -1,    0,  272,
   -1,  406,   59,   60,   61,   62,  411,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,  288,   -1,   -1,  423,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,  302,
   -1,   -1,   -1,   -1,  307,  308,   93,   -1,  311,   -1,
  445,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   59,  331,  407,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,  417,
  343,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,   -1,  497,   -1,  442,  472,  473,  474,  475,  476,
  477,  478,  479,  376,   -1,  378,  379,  380,  489,  490,
  383,  492,  460,  386,  387,   -1,   -1,   -1,  499,  500,
  501,  502,  503,  396,  505,   -1,  507,  508,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,
  413,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
    0,  499,  500,  501,  502,  503,  429,  505,   -1,  507,
  508,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,   -1,  279,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   59,
   60,   61,   62,   -1,   -1,  272,  489,  490,  300,  492,
   -1,   -1,   -1,   -1,  306,   -1,  499,  500,  501,  502,
  503,  288,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   93,   -1,  302,   -1,  329,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,  258,   -1,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,  348,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,  357,  333,  279,  335,   -1,
   -1,  363,   -1,  340,  341,   -1,  343,   -1,   41,   42,
   43,  273,   45,   -1,   47,  277,   -1,   -1,  300,  381,
   -1,   -1,  359,  360,  306,  362,   -1,   60,   61,   62,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,  329,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,  418,   -1,  287,  396,
   -1,   -1,   -1,   -1,   -1,   -1,  348,   -1,   -1,   -1,
   -1,   -1,   -1,  410,   -1,  357,  413,  339,   -1,   -1,
   -1,  363,   -1,  345,  346,  347,    0,  349,  350,  351,
   -1,  353,  429,   -1,   -1,   -1,   -1,   -1,  435,  381,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  373,   -1,  375,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   41,   -1,   43,
   44,   45,  262,   -1,  264,  265,  418,   -1,   -1,   -1,
   -1,   -1,  272,   -1,  406,   59,   60,   61,   62,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  288,   -1,
   -1,  423,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,  302,   -1,   -1,   -1,   -1,  307,  308,   93,
   -1,  311,   -1,  445,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,  480,  481,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
  360,  460,  362,   -1,   -1,  497,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,  376,  280,  378,  379,
  380,   -1,  285,  383,   -1,  288,  386,  387,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,  396,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  507,  508,
  410,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,    0,   -1,   -1,   -1,   -1,   -1,  429,
  333,   -1,   -1,   -1,  287,  435,   -1,   -1,  341,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,   -1,   -1,  360,   -1,  362,
  460,   -1,   -1,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,  262,   -1,
  264,  265,   59,   60,   61,   62,   -1,   -1,  272,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  288,  505,   -1,  507,  508,   -1,
   -1,   -1,   -1,   -1,   -1,  343,   93,   -1,  302,   -1,
   -1,   -1,   -1,  307,  308,   -1,  429,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,  369,   -1,   -1,  328,  261,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,  359,  360,   -1,  362,   -1,
   -1,  297,   -1,  299,   -1,  301,   -1,   -1,   -1,   -1,
  306,   -1,  376,   -1,  378,  379,  380,  313,  426,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,  460,   -1,   -1,
   -1,  327,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  338,   -1,   -1,   -1,  410,   -1,   -1,  413,
   -1,   -1,  460,   -1,   -1,   40,  489,  490,   43,  492,
   45,   -1,   -1,   -1,   -1,  429,  499,  500,  501,  502,
  503,  435,  505,   58,  507,  508,  440,   -1,   63,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  451,  452,   -1,
  454,  499,  500,  501,  502,  503,  460,  505,   -1,  507,
  508,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,  405,
   -1,   -1,   -1,   -1,   -1,  272,  412,   -1,   -1,  415,
   -1,  417,  418,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  288,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  507,  508,  302,  442,   -1,   -1,   -1,
  307,  308,   40,   -1,  311,   43,   -1,   45,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,  273,   -1,
   58,  328,  277,   -1,  331,   63,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
  295,   -1,   -1,  298,   -1,  491,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
  506,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,  396,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  267,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,  406,   -1,  460,  289,   -1,   -1,  292,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  323,   -1,
  445,  326,  499,  500,  501,  502,  503,   -1,  505,   -1,
  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,  267,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,
   -1,   40,  497,  378,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,  421,   -1,  326,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  471,    0,   -1,   -1,
  378,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,  260,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  507,  508,  509,  510,  511,   -1,   41,  417,
   -1,   44,   -1,  421, 1290, 1291,   -1,   -1,   -1,  427,
 1296, 1297,   -1, 1299, 1300,  433,   59,   -1,   61,   -1,
  301, 1307, 1308, 1309, 1310,  443, 1312, 1313, 1314, 1315,
 1316, 1317,  313,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  338,   -1,   -1,
    0,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,  507,
  508,  509,  510,  511,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   41,   -1,   -1,   44,    0,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  314,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   41,  326,   -1,   44,
   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,  460,
   40,   -1,   42,   43,   -1,   45,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   91,   -1,   58,  378,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,  443,   -1,   -1,  258,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,  460,
   -1,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,
   -1,  284,  421,   -1,  287,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,  300,  489,  490,
   -1,  492,   -1,  306,  443,   -1,  309,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  507,  508,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  258,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  507,  508,
  509,  510,  511,   -1,  274,  378,   -1,   -1,  278,   -1,
  280,   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,   -1,
  300,   -1,   -1,  258,  407,   -1,  306,   -1,  411,  309,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,  274,
   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,
   -1,   41,  287,  436,   44,   -1,   -1,   -1,  441,   -1,
  260,   -1,   -1,   -1,   -1,  300,   -1,  267,   -1,   59,
   -1,  306,   -1,   -1,  309,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,  378,   40,
   -1,   42,   43,  303,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  400,   -1,   63,  323,   -1,   -1,  326,  407,   -1,   -1,
   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,  400,  366,   -1,   -1,   -1,
  370,   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,
   -1,  434,   -1,  418,   -1,   -1,  466,   -1,   -1,   -1,
  470,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,  436,   45,   -1,   -1,   -1,  441,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
  354,  466,   -1,  433,   -1,  470,  489,  490,   -1,  492,
   -1,   -1,   -1,  443,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,  507,  508,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  507,  508,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,  318,   -1,
   -1,  321,   -1,   -1,  458,  276,  460,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,  471,  289,   -1,
  340,   -1,   -1,  343,  295,  296,   -1,   -1,   -1,   -1,
  460,   -1,  303,   -1,   -1,  489,  490,   -1,  492,  359,
   -1,   -1,   -1,   -1,  498,  499,  500,  501,  502,  503,
   -1,  505,  323,  507,  508,  326,   -1,   -1,   -1,  489,
  490,   -1,  492,  383,   -1,   -1,  386,  387,   -1,  499,
  500,  501,  502,  503,   -1,  505,  396,  507,  508,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,  260,   -1,   -1,
  263,   -1,   -1,  413,  267,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,  289,   -1,   -1,   -1,
  440,   40,  295,  296,   43,   -1,   45,   -1,  460,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,  427,  489,  490,   -1,
  492,   -1,  433,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  443,  505,   -1,  507,  508,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,  460,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  507,  508,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,  425,   -1,  427,  489,  490,   -1,  492,   -1,
  433,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
  443,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  402,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
  438,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
  289,  449,   -1,   -1,   -1,   40,  295,  296,   43,   -1,
   45,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  468,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,    0,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  507,
  508,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
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
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,  469,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,  483,  484,  354,  355,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  366,
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
   -1,    0,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
  262,  260,   41,   -1,  263,   44,   -1,   -1,  267,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   59,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,  406,
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
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   58,  289,   -1,   -1,   -1,   63,  328,
  295,  296,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
  359,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  354,
  355,   -1,   -1,   60,   61,   62,   -1,  396,   -1,   -1,
   -1,  366,   -1,   40,   -1,  370,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   40,  295,  296,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,  289,   -1,   -1,  460,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,   -1,   -1,  487,  488,  489,  490,  323,  492,   -1,
  326,  495,  496,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,
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
  460,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,   -1,   -1,  487,  488,  489,
  490,  323,  492,   -1,  326,  495,  496,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,   -1,
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
   -1,   -1,   -1,   41,   -1,   -1,   44,    0,   -1,   -1,
   -1,   -1,   -1,   -1,  413,  435,   -1,   -1,   -1,   -1,
  440,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  318,   -1,   41,  321,
   -1,   44,   -1,  452,  262,  454,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   59,   -1,  340,   -1,
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
   -1,  331,  396,   -1,  440,  335,   -1,   -1,   -1,  262,
  340,   -1,   -1,  343,   -1,   -1,  452,   -1,  454,  413,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,  359,
  328,   -1,  271,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  435,  340,   -1,   -1,  343,  440,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  452,   -1,
  454,  359,  301,   -1,   -1,  318,  396,   -1,  321,  308,
   -1,   -1,   -1,  312,  313,  328,  315,   -1,  331,  318,
   -1,   -1,  335,  413,   -1,  383,   -1,  340,  386,  387,
  343,   -1,   -1,  332,   -1,   -1,   -1,   -1,  396,  338,
   -1,   -1,   -1,   -1,   -1,  435,  359,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  271,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,  301,  454,   -1,   -1,  398,
  413,   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,
   -1,   -1,  318,   -1,   -1,   -1,  271,   -1,  417,   -1,
   -1,   -1,  435,   -1,   -1,   -1,  332,  440,   -1,   -1,
   -1,   -1,  338,  432,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,  442,   -1,   -1,  301,   -1,   -1,   -1,
   -1,   -1,  451,   -1,  453,   -1,   -1,  312,  313,   -1,
  315,  460,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  332,   -1,   -1,
   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,  398,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  507,  508,
   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,  273,   -1,
   -1,   -1,  277,  398,   -1,  451,   -1,  453,   -1,  284,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
  295,   -1,  417,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,  442,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,  453,  505,
   -1,  507,  508,  273,  339,  460,   -1,  277,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  373,   -1,
  375,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  507,  508,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,  406,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,   -1,  423,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,  423,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  507,  508,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,  507,  508,
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
"MANUAL","MAXIMUM","MAX_SEGMENT","MESSAGE","MERGE","MINIMUM","MODULE_NAME",
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
"keyword_or_column : SYMBOL",
"keyword_or_column : KW_BREAK",
"keyword_or_column : SUBSTRING",
"keyword_or_column : KW_DESCRIPTOR",
"keyword_or_column : CURRENT_CONNECTION",
"keyword_or_column : CURRENT_TRANSACTION",
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
"non_reserved_word : COALESCE",
"non_reserved_word : KW_DESCRIPTOR",
"non_reserved_word : LAST",
"non_reserved_word : LEAVE",
"non_reserved_word : LOCK",
"non_reserved_word : NULLIF",
"non_reserved_word : NULLS",
"non_reserved_word : STATEMENT",
"non_reserved_word : SUBSTRING",
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
case 460:
{ yyval = NULL; }
break;
case 461:
{ yyval = NULL; }
break;
case 462:
{ yyval = NULL; }
break;
case 463:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 464:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 465:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 466:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 467:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 468:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 469:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 470:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 472:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 473:
{ yyval = yyvsp[0]; }
break;
case 474:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 475:
{ yyval = yyvsp[0]; }
break;
case 476:
{ yyval = yyvsp[0]; }
break;
case 478:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 481:
{ yyval = NULL; }
break;
case 483:
{ yyval = NULL; }
break;
case 484:
{ yyval = yyvsp[0]; }
break;
case 485:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 501:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 502:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 504:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 505:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 506:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 512:
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
case 513:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 514:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 515:
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
case 516:
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
case 517:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 520:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 521:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 522:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 523:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 524:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 525:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 526:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 527:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 528:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 529:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 531:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 532:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 535:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 536:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 545:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 546:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 547:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 548:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
		    	}
break;
case 549:
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
case 550:
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
case 553:
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
case 554:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 555:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 556:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 557:
{ yyval = yyvsp[-1]; }
break;
case 558:
{ yyval = 0; }
break;
case 562:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 563:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 564:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 565:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 566:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 569:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 570:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 573:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 574:
{ yyval = (DSQL_NOD) NULL; }
break;
case 576:
{ yyval = (DSQL_NOD) NULL; }
break;
case 577:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 579:
{ yyval = (DSQL_NOD) NULL; }
break;
case 581:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 586:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 587:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 588:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 590:
{ yyval = yyvsp[0];}
break;
case 592:
{ yyval = yyvsp[0];}
break;
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 600:
{ yyval = 0; }
break;
case 601:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 602:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 603:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 604:
{ yyval = (DSQL_NOD) 0; }
break;
case 605:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 606:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 608:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 610:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 611:
{ yyval = 0; }
break;
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 614:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 615:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 616:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 618:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 619:
{ yyval = make_list (yyvsp[0]); }
break;
case 620:
{ yyval = 0; }
break;
case 622:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 623:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 624:
{ yyval = 0; }
break;
case 625:
{ yyval = make_node (nod_flag, 0, NULL); }
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
{ yyval = 0; }
break;
case 630:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 639:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 640:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 641:
{ yyval = 0; }
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
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 779:
{ yyval = yyvsp[-1]; }
break;
case 780:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 789:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 790:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 791:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 792:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 808:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 810:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 812:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 824:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 825:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 826:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 828:
{ yyval = make_list (yyvsp[-1]); }
break;
case 829:
{ yyval = yyvsp[-1]; }
break;
case 830:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 831:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 841:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 842:
{ yyval = yyvsp[0]; }
break;
case 843:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 844:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 845:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 846:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 847:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 848:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 849:
{ yyval = yyvsp[-1]; }
break;
case 850:
{ yyval = yyvsp[-1]; }
break;
case 854:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 855:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 856:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 857:
{ yyval = yyvsp[0]; }
break;
case 858:
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
case 859:
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
case 860:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 861:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 863:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 865:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 866:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 867:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 868:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 869:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 870:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 872:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
break;
case 873:
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
case 874:
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
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 876:
{ yyval = make_parameter (); }
break;
case 877:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 878:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 879:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 880:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 881:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 882:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 883:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 884:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 885:
{ yyval = yyvsp[0]; }
break;
case 886:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 888:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 889:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 890:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 891:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 892:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 894:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 895:
{ yyval = yyvsp[0];}
break;
case 900:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 901:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 902:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 903:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 904:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 905:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 906:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 907:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 908:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 909:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 911:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 912:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 913:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 918:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 921:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 922:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 926:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 928:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 929:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 930:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 932:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 936:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 937:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 938:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 939:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 940:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 941:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 942:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 943:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 946:
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
