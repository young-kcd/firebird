#ifndef lint
/*static char yysccsid[] = "from: @(#)yaccpar	1.9 (Berkeley) 02/21/93";*/
static char yyrcsid[] = "$Id: parse.cpp,v 1.61 2003-04-03 01:36:50 brodsom Exp $";
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

/* TMN: Remove Microsoft introduced defines
*/
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

}	/* extern "C"
*/
static DSQL_FLD	g_field;
static FIL	g_file;
static DSQL_NOD	g_field_name;
static TEXT	*beginning;
static SSHORT	log_defined, cache_defined;
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
#define COLLATION 281
#define COMMA 282
#define COMMIT 283
#define COMMITTED 284
#define COMPUTED 285
#define CONCATENATE 286
#define CONDITIONAL 287
#define CONSTRAINT 288
#define CONTAINING 289
#define COUNT 290
#define CREATE 291
#define CSTRING 292
#define CURRENT 293
#define CURSOR 294
#define DATABASE 295
#define DATE 296
#define DB_KEY 297
#define KW_DEBUG 298
#define DECIMAL 299
#define DECLARE 300
#define DEFAULT 301
#define DELETE 302
#define DESC 303
#define DISTINCT 304
#define DO 305
#define DOMAIN 306
#define DROP 307
#define ELSE 308
#define END 309
#define ENTRY_POINT 310
#define EQL 311
#define ESCAPE 312
#define EXCEPTION 313
#define EXECUTE 314
#define EXISTS 315
#define EXIT 316
#define EXTERNAL 317
#define FILTER 318
#define FOR 319
#define FOREIGN 320
#define FROM 321
#define FULL 322
#define FUNCTION 323
#define GDSCODE 324
#define GEQ 325
#define GENERATOR 326
#define GEN_ID 327
#define GRANT 328
#define GROUP 329
#define GROUP_COMMIT_WAIT 330
#define GTR 331
#define HAVING 332
#define IF 333
#define IN 334
#define INACTIVE 335
#define INNER 336
#define INPUT_TYPE 337
#define INDEX 338
#define INSERT 339
#define INTEGER 340
#define INTO 341
#define IS 342
#define ISOLATION 343
#define JOIN 344
#define KEY 345
#define KW_CHAR 346
#define KW_DEC 347
#define KW_DOUBLE 348
#define KW_FILE 349
#define KW_FLOAT 350
#define KW_INT 351
#define KW_LONG 352
#define KW_NULL 353
#define KW_NUMERIC 354
#define KW_UPPER 355
#define KW_VALUE 356
#define LENGTH 357
#define LOGFILE 358
#define LPAREN 359
#define LEFT 360
#define LEQ 361
#define LEVEL 362
#define LIKE 363
#define LOG_BUF_SIZE 364
#define LSS 365
#define MANUAL 366
#define MAXIMUM 367
#define MAX_SEGMENT 368
#define MERGE 369
#define MESSAGE 370
#define MINIMUM 371
#define MODULE_NAME 372
#define NAMES 373
#define NATIONAL 374
#define NATURAL 375
#define NCHAR 376
#define NEQ 377
#define NO 378
#define NOT 379
#define NOT_GTR 380
#define NOT_LSS 381
#define NUM_LOG_BUFS 382
#define OF 383
#define ON 384
#define ONLY 385
#define OPTION 386
#define OR 387
#define ORDER 388
#define OUTER 389
#define OUTPUT_TYPE 390
#define OVERFLOW 391
#define PAGE 392
#define PAGES 393
#define PAGE_SIZE 394
#define PARAMETER 395
#define PASSWORD 396
#define PLAN 397
#define POSITION 398
#define POST_EVENT 399
#define PRECISION 400
#define PRIMARY 401
#define PRIVILEGES 402
#define PROCEDURE 403
#define PROTECTED 404
#define RAW_PARTITIONS 405
#define READ 406
#define REAL 407
#define REFERENCES 408
#define RESERVING 409
#define RETAIN 410
#define RETURNING_VALUES 411
#define RETURNS 412
#define REVOKE 413
#define RIGHT 414
#define RPAREN 415
#define ROLLBACK 416
#define SEGMENT 417
#define SELECT 418
#define SET 419
#define SHADOW 420
#define SHARED 421
#define SINGULAR 422
#define KW_SIZE 423
#define SMALLINT 424
#define SNAPSHOT 425
#define SOME 426
#define SORT 427
#define SQLCODE 428
#define STABILITY 429
#define STARTING 430
#define STATISTICS 431
#define SUB_TYPE 432
#define SUSPEND 433
#define SUM 434
#define TABLE 435
#define THEN 436
#define TO 437
#define TRANSACTION 438
#define TRIGGER 439
#define UNCOMMITTED 440
#define UNION 441
#define UNIQUE 442
#define UPDATE 443
#define USER 444
#define VALUES 445
#define VARCHAR 446
#define VARIABLE 447
#define VARYING 448
#define VERSION 449
#define VIEW 450
#define WAIT 451
#define WHEN 452
#define WHERE 453
#define WHILE 454
#define WITH 455
#define WORK 456
#define WRITE 457
#define FLOAT_NUMBER 458
#define NUMBER 459
#define NUMERIC 460
#define SYMBOL 461
#define STRING 462
#define INTRODUCER 463
#define ACTION 464
#define ADMIN 465
#define CASCADE 466
#define FREE_IT 467
#define RESTRICT 468
#define ROLE 469
#define COLUMN 470
#define TYPE 471
#define EXTRACT 472
#define YEAR 473
#define MONTH 474
#define DAY 475
#define HOUR 476
#define MINUTE 477
#define SECOND 478
#define WEEKDAY 479
#define YEARDAY 480
#define TIME 481
#define TIMESTAMP 482
#define CURRENT_DATE 483
#define CURRENT_TIME 484
#define CURRENT_TIMESTAMP 485
#define NUMBER64BIT 486
#define SCALEDINT 487
#define CURRENT_USER 488
#define CURRENT_ROLE 489
#define KW_BREAK 490
#define SUBSTRING 491
#define RECREATE 492
#define KW_DESCRIPTOR 493
#define FIRST 494
#define SKIP 495
#define CONNECTION_ID 496
#define TRANSACTION_ID 497
#define BIGINT 498
#define CASE 499
#define NULLIF 500
#define COALESCE 501
#define USING 502
#define NULLS 503
#define LAST 504
#define ROWS_AFFECTED 505
#define LOCK 506
#define SAVEPOINT 507
#define STATEMENT 508
#define LEAVE 509
#define INSERTING 510
#define UPDATING 511
#define DELETING 512
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
  203,  203,  205,  205,  205,  205,  198,  212,  201,  201,
  206,  206,  188,  188,  215,  216,  216,  217,  217,  217,
  217,   11,  219,  219,  219,  220,  220,  220,  220,  220,
  220,   72,   81,  225,  226,  222,  227,  227,  227,  228,
  231,  235,  235,  236,  236,  237,  237,  148,  224,   88,
   90,  223,  223,   71,   84,  240,  240,  240,  241,  244,
  244,  245,  245,  245,  245,  245,  245,  245,  245,  245,
  245,  245,  245,  245,  245,  245,  242,  242,  243,    2,
  247,  247,  247,  247,  247,  247,  247,  255,  256,  253,
  253,  257,  257,  257,  257,  257,  257,  257,  248,  248,
  258,  258,  258,  258,  258,  258,  258,  252,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  260,  260,  262,  262,  261,  259,  259,  259,
  254,  254,  250,  251,  251,  264,  264,  264,  264,  265,
  265,  249,  266,  266,  267,  267,    8,  268,  268,  268,
  268,  268,  268,  268,  268,  268,  268,  268,  268,  102,
  102,  145,  145,  271,  271,  274,  274,  276,  276,   54,
   54,  273,  273,  273,  273,  273,  273,  273,  273,  273,
  281,  281,  272,  272,  272,  272,  282,  282,   62,   62,
   62,   56,   56,  278,  278,  278,  275,  275,  275,  286,
  286,  286,  285,  285,  284,  284,  284,  279,  279,  289,
  287,  287,  287,  288,  288,  280,  280,  280,  280,  290,
  290,   19,   19,   19,  292,  292,  292,   16,   17,  296,
  296,    4,   15,  295,  295,  297,  297,  298,  298,  291,
  299,  299,  300,  300,  301,  301,  301,  301,  302,  302,
  303,  303,  304,  304,  306,  306,  306,  307,  307,  307,
  308,  308,  308,  305,  310,  310,  310,  311,  311,  309,
  309,  312,  314,  314,  313,  313,  293,   18,  315,  315,
  315,  316,  316,  319,  319,  320,   66,   66,   66,  321,
  321,  321,  317,  317,  322,  322,  323,  323,  318,  327,
  324,  324,  324,  324,  328,  328,  328,  329,  329,  329,
  229,  229,  230,  230,  331,  331,  332,  332,  332,  325,
  335,  335,  336,  336,  337,  337,  338,  338,  339,  339,
  210,  210,  341,  341,  238,  238,   32,  239,  239,  239,
  239,  239,  239,  239,  239,  233,  233,  342,  342,  343,
  343,  343,  343,  343,  345,  345,  345,  234,  234,  232,
  232,  349,  349,  349,  326,  326,  350,  351,  351,  351,
  351,  351,  352,  352,  353,  353,  354,  354,  355,  355,
  355,  356,  356,   10,   10,  358,  358,    7,    7,  359,
  360,  361,   20,   20,  362,  363,  364,  364,  189,  333,
  333,    3,    3,  366,  366,  366,  368,  368,   62,  367,
  367,  369,  369,   35,   35,   86,   85,   85,  357,  357,
  370,  371,  371,  211,  211,  211,  147,  365,  365,  112,
  112,  112,  112,  373,  373,  373,  373,  373,  373,  373,
  373,  373,  373,  373,  373,  374,  374,  374,  374,  374,
  374,  374,  374,  379,  379,  379,  379,  379,  379,  379,
  379,  379,  379,  379,  379,  379,  379,  379,  379,  386,
  386,  375,  375,  376,  376,  376,  376,  377,  377,  381,
  381,  382,  382,  382,  382,  380,  383,  378,  378,  384,
  384,  384,  387,  387,  388,  385,  390,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,  157,  157,  157,  391,  389,  389,  152,
  152,  395,  395,  395,  395,  395,  393,  393,  393,  393,
  393,  221,  153,  153,  154,  155,  155,  155,  155,  155,
   52,   52,   21,   21,  246,  396,   55,  124,  277,  277,
   96,  392,  392,  392,  392,  397,  397,  397,  397,  397,
  397,  397,  397,  397,  397,  397,  398,  346,  347,  347,
  347,  344,  344,  394,  348,  348,  400,  400,  401,  401,
  402,  402,  405,  405,  403,  403,  408,  408,  407,  404,
  406,  399,  399,  399,  399,  399,  399,  399,  399,  330,
  330,  156,   49,  283,  123,  149,  372,  166,  214,  270,
   64,   61,  218,  113,   67,  334,  213,   36,   42,  340,
  269,   40,   43,  209,   41,  294,  409,
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
    4,    0,    1,    1,    3,    3,    6,    2,    2,    2,
    3,    0,    1,    2,    4,    1,    3,    2,    2,    2,
    1,    4,    1,    3,    0,    1,    1,    1,    3,    3,
    3,    7,    7,    7,    7,    1,    1,    3,    4,    7,
    2,    1,    3,    1,    1,    6,    3,    0,    0,    0,
    0,    3,    0,    9,    9,    1,    1,    0,    2,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    5,    5,    5,    5,    5,    5,    2,    0,    4,    2,
    3,    3,    2,    2,    3,    3,    2,    3,    6,    1,
    2,    4,    3,    2,    2,    2,    2,    3,    1,    3,
    3,    3,    2,    2,    4,    4,    5,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    0,
    2,    2,    0,    1,    2,    2,    2,    2,    2,    1,
    3,    7,    1,    0,    1,    0,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    3,    2,    2,    2,    1,
    1,    1,    1,    4,    5,    1,    3,    1,    3,    1,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    4,    4,    6,    5,    3,    0,    2,    2,
    0,    3,    0,    4,    1,    5,    4,    1,    4,    1,
    2,    2,    1,    1,    1,    2,    2,    2,    2,    1,
    0,    3,    5,    1,    1,    2,    3,    1,    2,    3,
    0,    1,    1,    1,    5,    5,    6,    2,    5,    1,
    0,    3,    2,    1,    0,    2,    0,    1,    0,    3,
    1,    0,    1,    2,    1,    1,    1,    1,    2,    2,
    1,    2,    3,    1,    1,    3,    3,    1,    2,    3,
    1,    2,    0,    2,    1,    1,    0,    1,    1,    1,
    3,    2,    3,    0,    1,    3,    4,    3,    1,    3,
    4,    3,    0,    1,    3,    4,    1,    1,    0,    2,
    2,    0,    4,    0,    2,    0,    2,    0,    9,   10,
    2,    1,    1,    0,    2,    4,    2,    2,    4,    2,
    1,    1,    1,    1,    1,    3,    1,    2,    3,    2,
    1,    3,    1,    1,    6,    3,    3,    2,    3,    0,
    1,    3,    1,    1,    1,    2,    1,    1,    1,    2,
    1,    2,    1,    2,    0,    3,    0,    1,    3,    1,
    1,    1,    1,    3,    1,    1,    1,    2,    0,    2,
    0,    3,    5,    0,    2,    0,    4,    1,    2,    1,
    1,    0,    1,    3,    2,    1,    1,    2,    1,    4,
    2,    1,    3,    8,    5,    1,    3,    1,    1,    4,
    4,    4,    1,    1,    5,    5,    1,    3,    3,    1,
    1,    7,    7,    5,    3,    0,    1,    1,    1,    2,
    0,    1,    1,    1,    0,    3,    1,    3,    1,    0,
    3,    1,    3,    1,    3,    3,    1,    1,    3,    1,
    3,    3,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    3,    4,    5,    6,    3,    4,    3,
    4,    3,    4,    4,    5,    4,    4,    3,    4,    1,
    1,    1,    1,    3,    3,   10,   10,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    3,    3,
    3,    3,    3,    3,    3,    3,    1,    1,    1,    1,
    3,    1,    1,    1,    1,    1,    4,    1,    3,    1,
    2,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    1,    2,    1,    1,    1,    1,    1,    2,
    1,    1,    1,    1,    1,    4,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    6,    6,    6,    8,
    4,    4,    3,    6,    1,    1,    6,    6,    1,    1,
    4,    6,    4,    5,    3,    5,    4,    5,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  562,
  563,  564,    0,  619,  728,  729,  733,  734,  473,    0,
    0,    0,    0,    0,    0,  410,  574,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  895,    0,   22,  893,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  487,    0,    0,   38,   35,   37,    0,   36,
    0,  977,    0,    0,    0,    0,   33,   69,   67,  969,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  643,    0,    0,    0,  675,    0,    0,
  971,    0,    0,    0,  108,  568,  976,    2,    0,    0,
    0,    0,  457,  439,  440,  441,  442,  443,  444,  445,
  446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
  456,  458,  459,  460,  461,  462,    0,  438,    0,  961,
    0,  417,  965,    0,  414,  968,    0,  677,    0,  413,
  972,    0,  572,  891,    0,  151,  104,    0,  105,    0,
  218,  767,  957,    0,  103,  146,  964,    0,    0,   99,
    0,  106,  897,    0,  100,    0,  101,    0,  102,  975,
  147,  107,  627,  628,    0,  896,  894,    0,   75,    0,
  962,    0,  495,  960,  488,    0,  494,  499,  489,  490,
  497,  491,  492,  493,  498,    0,   31,    0,   40,  754,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  901,    0,  882,  645,
  647,    0,  648,  650,  950,  651,    0,  652,  641,    0,
    0,    0,    0,    0,    0,    0,  591,  580,    0,  583,
  585,  586,  587,  588,  594,  595,    0,  676,  970,    0,
  109,    0,  110,    0,  111,    0,    0,  620,    0,  618,
    0,    0,    0,    0,  474,    0,    0,  380,    0,  221,
    0,  420,  411,  471,  472,    0,    0,    0,    0,    0,
    0,  429,    0,  386,  387,    0,  578,  576,  892,    0,
    0,    0,    0,    0,    0,    0,  154,  133,    0,   97,
    0,    0,  112,    0,  122,  121,    0,    0,    0,    0,
    0,    0,   76,    0,  953,    0,    0,  730,  731,  496,
    0,  952,  874,  873,  872,    0,    0,  875,  876,    0,
    0,  878,  357,  358,  352,    0,  356,  870,  877,    0,
  757,    0,    0,   28,    0,    0,    0,   72,   70,  973,
   68,   34,    0,    0,    0,  759,    0,   53,    0,    0,
    0,    0,    0,  570,    0,    0,    0,    0,  860,  888,
    0,    0,  862,    0,    0,  889,    0,  883,    0,  864,
  865,  866,  884,  885,    0,  886,  887,    0,    0,    0,
  890,    0,    0,    0,    0,    0,    0,  764,  857,  858,
  859,  863,  843,    0,  842,    0,  846,  904,  905,  845,
  839,  840,  841,  844,  902,  903,  925,  926,  929,  930,
    0,    0,  654,    0,  741,    0,    0,  655,    0,    0,
  617,    0,  592,    0,  589,    0,  590,  615,    0,  610,
    0,    0,  584,  768,  737,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  624,  621,    0,  186,    0,    0,
  130,  479,  173,  174,    0,  477,    0,    0,    0,    0,
  480,    0,  475,    0,    0,  424,  426,  425,    0,  427,
    0,  421,  219,    0,    0,  272,    0,    0,    0,    0,
  433,  434,    0,  464,    0,    0,    0,    0,    0,    0,
    0,  391,  390,  483,    0,    0,  150,    0,    0,  159,
    0,  158,    0,  165,  148,    0,  163,  166,  167,  155,
    0,    0,  518,  554,  521,    0,  555,    0,    0,  522,
    0,    0,    0,  545,  558,  517,  540,  519,  520,  515,
  502,  380,  500,  501,  503,    0,    0,  512,  513,  514,
  516,    0,    0,    0,    0,    0,  113,    0,  114,    0,
  125,    0,    0,  202,    0,    0,  378,    0,  221,    0,
    0,   84,    0,    0,  749,    0,    0,    0,    0,    0,
  830,  831,  832,    0,    0,    0,  770,  774,  775,  776,
  777,  778,  779,  780,  781,  782,  783,  784,  879,  880,
  881,  871,    0,    0,    0,  756,   29,    0,    0,   45,
   73,    0,    0,   27,    0,  762,    0,    0,    0,  725,
    0,    0,    0,    0,    0,  569,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  326,  974,    0,
    0,    0,    0,    0,    0,    0,  646,    0,    0,  649,
    0,    0,    0,    0,  658,  966,  566,    0,  899,  565,
    0,  593,    0,  601,  597,  596,    0,    0,    0,  612,
  600,    0,    0,  735,  736,    0,    0,    0,  378,    0,
    0,    0,    0,    0,    0,    0,  131,    0,    0,    0,
    0,    0,    0,  380,  423,    0,  380,  381,  428,  510,
    0,  274,    0,  268,  270,  378,  241,  958,    0,    0,
  380,    0,    0,  210,  378,    0,    0,    0,    0,  246,
  240,  243,  244,  245,  432,  469,  468,  431,  430,    0,
    0,    0,    0,    0,    0,  389,    0,  380,    0,  156,
  160,    0,  164,    0,    0,  541,  542,  559,    0,  556,
    0,    0,  548,  546,  547,    0,    0,    0,    0,  511,
    0,    0,    0,    0,  549,    0,    0,  378,    0,  201,
    0,  204,  206,  207,    0,    0,    0,    0,    0,  221,
   79,    0,    0,   78,  529,  530,  954,    0,    0,    0,
  773,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  354,  360,  361,  359,  758,    0,    0,    0,    0,
    0,    0,    0,    0,   54,   62,   64,    0,    0,   71,
    0,    0,    0,  761,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  942,  943,  944,  945,
  946,  947,  948,  949,    0,    0,    0,    0,    0,    0,
  935,    0,    0,    0,  673,    0,    0,  855,  856,  923,
    0,    0,  851,  956,    0,    0,    0,    0,    0,    0,
  861,  766,  765,    0,    0,    0,    0,  663,  664,    0,
    0,  656,  659,  567,  900,  602,  611,  606,  605,    0,
  616,  769,  738,  739,  378,    0,    0,    0,    0,  625,
    0,    0,  633,  182,  176,  183,    0,  179,    0,    0,
  178,    0,  189,  170,  169,  898,  172,  171,  481,    0,
    0,    0,  422,  273,    0,  212,  380,  214,    0,  381,
  209,  215,    0,    0,    0,    0,  435,  436,  380,  378,
  381,    0,    0,    0,  407,    0,  197,  198,  157,    0,
    0,    0,    0,    0,    0,  557,    0,    0,  380,  381,
    0,  506,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  199,    0,    0,    0,    0,
  367,    0,   83,    0,    0,    0,    0,   88,   85,    0,
    0,    0,  732,  959,    0,    0,  785,    0,    0,    0,
  811,  810,    0,    0,    0,  818,  833,  828,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  772,    0,
   66,   58,   59,   65,   60,   61,    0,    0,   23,    0,
    0,    0,   43,    0,    0,    0,  743,  763,    0,  726,
    0,  742,    0,    0,    0,    0,    0,    0,  221,    0,
  906,    0,    0,  921,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  931,    0,  941,    0,
    0,    0,    0,    0,    0,  922,  867,    0,    0,    0,
    0,    0,    0,  678,    0,    0,    0,    0,    0,  608,
  609,  613,    0,  200,    0,    0,  626,  637,    0,    0,
    0,    0,    0,    0,  190,    0,    0,  418,  224,  225,
  226,  227,  228,  229,    0,    0,    0,  277,    0,  222,
    0,    0,    0,    0,  247,    0,  465,  466,  437,    0,
    0,    0,    0,    0,    0,  380,  485,  381,  168,  955,
    0,    0,  524,    0,  523,  560,    0,  552,    0,    0,
    0,  504,    0,  532,    0,    0,  534,  537,  539,  378,
    0,    0,    0,  120,    0,  205,  380,    0,    0,  379,
    0,   98,    0,  118,    0,   94,    0,    0,    0,    0,
   87,    0,  826,  827,    0,    0,    0,    0,    0,    0,
  829,    0,    0,    0,    0,    0,    0,    0,  819,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   57,   63,   55,   56,   24,  747,
  748,    0,  745,  752,  753,  750,    0,    0,  724,    0,
    0,  912,  911,    0,    0,  908,  907,    0,  916,  915,
  914,  913,  910,  909,    0,    0,  937,    0,    0,    0,
  936,    0,    0,    0,  671,    0,    0,  666,    0,  667,
    0,  684,  680,  682,    0,    0,    0,    0,    0,  379,
  630,  631,    0,  184,  180,    0,    0,  195,  191,    0,
  381,  280,    0,    0,  381,  278,    0,    0,    0,  232,
    0,    0,  248,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  482,  526,    0,  527,    0,  135,    0,    0,
    0,  139,  141,  142,  143,  507,  509,  505,  536,    0,
    0,  381,    0,    0,    0,    0,  362,    0,  368,  380,
   89,    0,    0,   91,    0,    0,    0,    0,    0,    0,
  835,  834,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   41,    0,    0,  727,  220,  924,  917,  918,    0,  919,
  933,  932,    0,  938,  927,    0,  928,    0,  669,    0,
  550,    0,  691,    0,    0,  688,  692,  693,  695,  696,
  697,    0,    0,  639,  381,  363,    0,  175,  192,  381,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  285,  290,  284,
    0,    0,    0,  294,  295,  296,  297,  298,  299,  301,
  302,  304,  305,  306,  309,  267,    0,    0,  208,  233,
    0,    0,    0,    0,  234,  237,    0,    0,  381,  405,
  406,  402,  401,  404,  403,    0,  525,  553,  144,  132,
  140,    0,  380,  264,  127,  381,    0,    0,  382,  369,
    0,    0,   92,   93,   80,    0,  797,  805,    0,  798,
  806,  801,  809,    0,    0,  799,  807,  800,  808,  794,
  802,  795,  803,  796,  804,  744,    0,    0,  934,  672,
    0,    0,    0,    0,    0,  708,  710,    0,  705,    0,
  265,  185,  419,    0,  312,    0,    0,  315,  316,    0,
  308,    0,    0,    0,    0,  307,    0,  339,  340,  293,
  300,    0,  303,  286,    0,  291,    0,  343,  292,  211,
  216,  235,    0,    0,  238,    0,  250,  252,  409,  381,
    0,  384,    0,    0,    0,  374,  375,    0,    0,   77,
   95,    0,    0,  920,    0,    0,  694,  689,  709,    0,
    0,  283,    0,  310,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  333,  334,  351,    0,    0,    0,
    0,  346,  344,  239,    0,    0,    0,    0,    0,  266,
  381,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  716,    0,    0,    0,  282,  279,  311,    0,    0,
    0,    0,  314,    0,    0,    0,    0,  322,    0,  325,
    0,  350,  349,  963,  348,    0,    0,    0,    0,    0,
  249,    0,  256,    0,  255,  385,  377,    0,    0,  370,
  119,    0,    0,    0,  718,  707,    0,    0,  719,    0,
  715,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  336,  335,  345,  347,  236,    0,    0,  260,  259,
  258,    0,    0,  640,  837,  714,    0,  721,  328,    0,
    0,  313,  319,    0,    0,    0,    0,  337,  263,  261,
  262,    0,    0,    0,    0,    0,    0,  341,  317,    0,
    0,    0,    0,  720,  331,  318,  320,  836,  723,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1504,   27,   28, 1505,
   30,   31,   32,   33,   34,   35,   36, 1506,   38, 1507,
  615,  103,  383,  873, 1119,  104,  649,  874,  105,  387,
  654,  128,  106,  107,  239,  945,  118,  875,  876,  179,
  208,  108,  877,  109,  389,   79,  219,  353,  436,  610,
 1056,  372,  833,  581, 1471,  810,  611,  612,  613, 1058,
  220, 1310,   71,  169,   72,  215, 1794, 1262,  200,  205,
  207,  209,  195,  187,  189,  202,  212,  135,  291,  293,
  295,  343,  597,  599,  380,  240,  760,  515,  625,  993,
  347,  602, 1044, 1254,  548,  709, 1019,  500,  501,  530,
  339,  582, 1030, 1400,  731, 1198, 1401, 1402, 1403, 1404,
 1405,  626,  196,  188,  335,  555,  336,  337,  556,  557,
  558,  559, 1229,  987,  503,  504,  977,  975,  505,  981,
  976, 1189, 1364,  982,  983, 1194,  349,  821,  822,  823,
  824,  762, 1000, 1378,  583,  764,  438,  826,  933,  524,
 1325, 1199,  439,  440,  441,  925,  442, 1379, 1380,  533,
 1535, 1687, 1215,  770,  771,  757,  772,  773,  774, 1688,
 1689, 1770,  317,  528, 1206, 1508,  175,  525,  526, 1207,
 1208, 1373, 1663, 1509, 1510, 1511, 1512, 1627, 1513, 1514,
 1515, 1516, 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1524,
 1525, 1710, 1755, 1610, 1674, 1759, 1672,  443,  688, 1344,
  444,    0,    0, 1063, 1628, 1681, 1682, 1723,  375,  376,
  445, 1049, 1260, 1417,    0,    0, 1050, 1051,  267,  466,
 1558,  358, 1179, 1358, 1644, 1645, 1646, 1647, 1177,  326,
  544,  788, 1227,  545,  786,  203,   56,  321,  180,  142,
  304,  167,  311,  172,  748,  745,  312,  322,  778,  323,
  541, 1011,  168,  305,  512,  546, 1228,   93,  178,  223,
  584,  585,  750, 1031,  751, 1032, 1033,  588,  589,  590,
  591, 1024,  836,  592,  593,  594,  803,  595, 1473,  800,
   40,   41,   42,  136,   58,  405,  183,  328,  278,  279,
  280,  281,  282,  283,  284,  285,  286,  715,  479,  960,
 1182,  480,  481,  720,   43,  141,  300,   44,  494,  495,
 1187,  734,  973,  122,  702, 1484,  660,  123,  124,  268,
  467,  468,  469,  705,  946,  947,  948,  949, 1171,  446,
 1159, 1475, 1476,  447, 1478,  448,  449,  450,    0, 1702,
 1600, 1703, 1704, 1705, 1751, 1795,  395, 1131,   45,   46,
  359,   47,   48,  487,  488,  882, 1127, 1312, 1316,  396,
  657,  192,  627,  628,  629,  630,  631,  632,  633,  634,
  635,  636,  637,  638, 1279, 1074, 1076, 1077,  932,  687,
  451,  452,  453,  454,  379,  217,  455,  456,  915,  457,
  458,  459,  460,  679,  919, 1160, 1155,  680,  461,
};
static short yysindex[] = {                                   3418,
 1930, -126, 2555,  153, 1132,  316, 2365,  351, 2155,  659,
  241, 1816, -126,  985,  -28,  504,  839,  504,    0,  667,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  734,    0,    0,    0,    0,    0,    0, 5419,
  504,  504,  504,  504,  504,    0,    0,  480, 1607,  504,
  504,  504,  720,  504,  709,  504,  504,    0,  504,  504,
    0,   94,    0,  742,    0,    0,  928,  504,    0,  504,
  504,  504,  999,  504,  504,  504,  504,  709,  504,  504,
  504,  504,    0,  504,  952,    0,    0,    0, 1374,    0,
 1374,    0, 1009, 1075,   92, 1462,    0,    0,    0,    0,
  504,  504,  504, 1089, 1151, 1219,  167, 1770, 1172,  136,
  183,   82, 1125,    0,  504, 1287, 1492,    0, 1211,  504,
    0,  504,  504,  504,    0,    0,    0,    0, 1364,   20,
 1327,  563,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  862,    0, 1607,    0,
  558,    0,    0, 1666,    0,    0, 1655,    0,  745,    0,
    0, 1285,    0,    0, 1308,    0,    0, 1550,    0, 1520,
    0,    0,    0, 1607,    0,    0,    0,  693, 1666,    0,
   71,    0,    0, 1475,    0, 1531,    0, 1374,    0,    0,
    0,    0,    0,    0, 1466,    0,    0,  504,    0, 1521,
    0, 1415,    0,    0,    0,  504,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1120,    0,  504,    0,    0,
    0,  675, 1470, 1086,  504, 1485, 1561, 1846, 1593, 1603,
  675, 1544, 1086, 1568, 1573, 1417,    0, 7641,    0,    0,
    0, 7641,    0,    0,    0,    0, 5740,    0,    0, 1530,
  504, 1614, 1549,  968,  504, 1546,    0,    0, 1492,    0,
    0,    0,    0,    0,    0,    0,  504,    0,    0, 1666,
    0, 1475,    0, 1374,    0, 7641, 1592,    0, 1585,    0,
  560, 1681, 1195,  563,    0, 1758, 1005,    0,  504,    0,
  862,    0,    0,    0,    0,  504, 1636,  -16, 1582,  -12,
 2010,    0, 6212,    0,    0, 1226,    0,    0,    0, 2003,
 2003, 1607, 1702, 1607, 2541, 1550,    0,    0, 3959,    0,
  504,  504,    0, 1636,    0,    0, 1790,  735, 2039,  504,
 1838,  504,    0, 2075,    0,  198, 3582,    0,    0,    0,
 1665,    0,    0,    0,    0, 1685, 1687,    0,    0, 1647,
 1555,    0,    0,    0,    0, 2109,    0,    0,    0, 1244,
    0,  504, 1721,    0,  504,  504,  119,    0,    0,    0,
    0,    0,  504,  504,  548,    0,  504,    0, 1845,  504,
 2117,  675, 1765,    0,  504, 2139, 2140, 2154,    0,    0,
 2160, 2161,    0, 2168, 2169,    0, 2172,    0, 2176,    0,
    0,    0,    0,    0, 2178,    0,    0, 6294, 2179, 2180,
    0, 7641, 7641, 6522,  504, 2181, 1370,    0,    0,    0,
    0,    0,    0, 2079,    0, 2183,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1801,    0, 1629,    0, 1906, 2186,    0,   42,   68,
    0, 1106,    0,  800,    0,  800,    0,    0, 2188,    0,
  180, 1804,    0,    0,    0, 2191,  206, 2177,    0, 1636,
 2199, 1983, 1913, 2202,    0,    0, 1865,    0,  223, 1903,
    0,    0,    0,    0, 1607,    0, 2003, 2003, 2003, 2003,
    0, 2205,    0, 1975, 1977,    0,    0,    0, 1955,    0,
 3959,    0,    0, 3959, 1317,    0, 1666, 1993,  504, 6253,
    0,    0, 1113,    0,  504,  623, 1655,    0, 1860, 1823,
 1791,    0,    0,    0,  765, 1867,    0, 1807,  709,    0,
 1607,    0, 1992,    0,    0, 2541,    0,    0,    0,    0,
  186, 1822,    0,    0,    0, 1825,    0, 1875, 2239,    0,
 1931, 2240,   76,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2194,  170,    0,    0,    0,
    0,  219, 2253, 2255, 2240, 1666,    0, 1963,    0, 2032,
    0, 1607, 1607,    0,  -16,  745,    0, 1914,    0, 1894,
 2271,    0, 8924,  106,    0, 1923, 1935, 2287, 3860, 2288,
    0,    0,    0, 3681, 5383,   28,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1756, 1555,  504,    0,    0, 1193, 1895,    0,
    0, 1870, 1086,    0, 2013,    0, 1831,  985, 2296,    0,
 2013, 1193, 2016, 2017,  504,    0,   88, 6608,  810,  504,
 7641, 1026, 1158, 1185, 2938, 7641, 3860, 1629, 1916,  426,
 7641, 6608, 1119, 1119,  985, 1814, 2305,    0,    0, 6787,
  504, 7641, 7641, 7641, 7641, 7641,    0, 7641,  124,    0,
   72, 1902, 6608,  504,    0,    0,    0,  965,    0,    0,
  660,    0, 1908,    0,    0,    0,  504,  363,  504,    0,
    0,  504,  504,    0,    0, 6608, 2094,  -16,    0,  504,
   94, 7641,  504, 1915, 1607, 1607,    0,  -52, 1807, 1807,
 1925,  709, 1195,    0,    0, 2342,    0,    0,    0,    0,
 2110,    0,  504,    0,    0,    0,    0,    0, 2116, 2351,
    0, 2091, 2111,    0,    0, 2055, 2056, 1374, 2121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  709,
  504, 7576, 2020, 2022, 2024,    0,  709,    0, 1699,    0,
    0, 1987,    0,  112, 1995,    0,    0,    0,  709,    0,
 2239,  193,    0,    0,    0, 2112,  193, 2006,  193,    0,
 2390,  709,  709,  709,    0, 1636,  504,    0, 2080,    0,
 1927,    0,    0,    0, 1226, 2021,  504, 1936,  304,    0,
    0, 2395, 2162,    0,    0,    0,    0,  198,  504, 2025,
    0, 2025, 4597,  454, 7641, 7641, 4592, 2398,  722, 4675,
 7641, 4772,  523, 4951, 5027, 6851, 5270, 5505, 5587, 3860,
 3860,    0,    0,    0,    0,    0,  504,  504,  504,  504,
  504,  504,  176, 2396,    0,    0,    0, 1193, 2060,    0,
  434, 2081,  504,    0,   82, 6608, 2081, 2411, 2396, 1193,
   -2, 2132, 7641, 7641, 2196, 7641, 2421, 7641, 2420, 2054,
 7641, 7641, 7641, 7641, 7641, 7641,    0,    0,    0,    0,
    0,    0,    0,    0, 2144, 1387,  795, 7641,  872, 6608,
    0, 3860, 1719, 1629,    0, 2423,   82,    0,    0,    0,
 1629, 1952,    0,    0, 2190, 1119, 1119,  979,  979, 1453,
    0,    0,    0,   72, 2428, 2430, 1463,    0,    0, 3860,
 2147,    0,    0,    0,    0,    0,    0,    0,    0,  494,
    0,    0,    0,    0,    0, 1968, 2021,    0, 1972,    0,
 2433, 1981,    0,    0,    0,    0, 1982,    0, 2003, 2217,
    0,  -52,    0,    0,    0,    0,    0,    0,    0, 2460,
 3860, 1654,    0,    0, 2203,    0,    0,    0, 1654,    0,
    0,    0, 1374, 1374, 1986, 2462,    0,    0,    0,    0,
    0,  -19,  581,  639,    0, 2248,    0,    0,    0,  504,
  153, 1988, 2083, 2110, 2472,    0, 1807, 2018,    0,    0,
 1548,    0, 2457,  504, 1649,  709, 2478, 2479, 2480, 2258,
  745, 2203, 2003, 1903,  -16,    0, 1867,   82, 2069, 2087,
    0,   69,    0,  709, 2135, 2227, 8924,    0,    0,  709,
 2046, 2228,    0,    0, 2499, 2509,    0, 1711, 1629, 2505,
    0,    0, 1629, 2511, 7077,    0,    0,    0, 2200, 2512,
 1629, 2516, 1336, 2527, 1629, 2528, 7641, 7641, 2398, 7641,
 7141, 2531, 1629, 2534, 2535, 1629, 2537, 7641, 1629, 2542,
 1629, 2543, 2544, 1629, 2545, 2550, 1629, 2551,    0, 2295,
    0,    0,    0,    0,    0,    0, 2264, 1193,    0, 1193,
  176, 2396,    0,  195,  195,  201,    0,    0, 5740,    0,
 2027,    0, 2411, 2396, 2553,   -2, 2122, 2130,    0, 2247,
    0, 2269, 7641,    0, 2307, 2376, 2409, 2437, 2612, 2796,
 7641,  709, 6608, 1629, 2145, 6608,    0, 7641,    0, 2286,
 1035, 7641, 6608, 7641, 7641,    0,    0, 1463, 2558, 6608,
  504,   72, 2211,    0, 2218, 2221, 2262, 2337, 2281,    0,
    0,    0, 2203,    0, 2069, -133,    0,    0, 2192, 1607,
 2223, 1807, 2225, 1807,    0, 3860,  748,    0,    0,    0,
    0,    0,    0,    0, 2173, 2353, 2203,    0, 7641,    0,
 2338, 2220, 1986,   94,    0, 3860,    0,    0,    0, 2243,
 2244, 2246, 2251, 2283, 2289,    0,    0,    0,    0,    0,
 2599,  153,    0, 1925,    0,    0,  193,    0, 1654, 2266,
  193,    0,  193,    0, 2110, 2610,    0,    0,    0,    0,
 1226, 2353, 1807,    0, 1903,    0,    0, 5740, 2380,    0,
   44,    0, 2433,    0, 2628,    0, 2632, 1607,  502, 2633,
    0, 1607,    0,    0, 7641, 2257, 2257,  985, 2644, 2049,
    0, 2257, 2257, 7641, 2257, 2257, 3000, 1629,    0, 1527,
 7641, 1629, 2257, 2257, 2257, 2257, 1629, 2257, 2257, 2257,
 2257, 2257, 2257, 2304,    0,    0,    0,    0,    0,    0,
    0, 2249,    0,    0,    0,    0, 1906, 6608,    0,   -2,
 2553,    0,    0, 3959, 2651,    0,    0, 2931,    0,    0,
    0,    0,    0,    0, 3026,  150,    0, 6608, 2385, 2260,
    0, 6608, 3048, 2078,    0, 1515, 1629,    0, 2084,    0,
 1463,    0,    0,    0,   72, 1566, 3860, 2308, 2353,    0,
    0,    0, 2003,    0,    0, 1607, 1699,    0,    0,  766,
    0,    0,  504, 4349,    0,    0, 1629, 2426, 2338,    0,
 1332,  504,    0, 2369,  802, 2265, 2370, 2273, 2416, 2383,
 2417, 2203,    0,    0, 2683,    0, 2684,    0, 2373, 2426,
 2266,    0,    0,    0,    0,    0,    0,    0,    0, 2203,
 1867,    0, 1699, 2248, 2406, 2343,    0, 2021,    0,    0,
    0, 2359,  561,    0, 2110, 2360, 1629, 2692, 2694,   82,
    0,    0, 2697, 2698, 1629, 2699, 2702, 7641, 7641, 1629,
 2704, 2705, 2706, 2707, 2708, 2709, 2713, 2717, 2718, 2721,
    0,  195, 1902,    0,    0,    0,    0,    0,  709,    0,
    0,    0, 6608,    0,    0, 6608,    0, 1902,    0, 1293,
    0, 2484,    0, 2719, 2722,    0,    0,    0,    0,    0,
    0,   28, 1292,    0,    0,    0, 1807,    0,    0,    0,
    0, 3959,  129,  399, 2715,  -20, 2736, 2438, 7641, 2724,
 2746, 2729, 2732, 2734, 2735, 2458, 2739,    0,    0,    0,
 2497, 4083, 2751,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2771,  504,    0,    0,
 2461, 2471,  504, 1986,    0,    0, 1374,  504,    0,    0,
    0,    0,    0,    0,    0, 2353,    0,    0,    0,    0,
    0, 2353,    0,    0,    0,    0,  127, 1902,    0,    0,
 7641, 1607,    0,    0,    0, 1607,    0,    0, 7641,    0,
    0,    0,    0, 1629, 1629,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2147, 2776,    0,    0,
 2147, 3860,  504,  138, 1566,    0,    0, 2450,    0, 2781,
    0,    0,    0,  152,    0, 7321,  504,    0,    0, 7641,
    0, -121, 2481, 3860, 1736,    0, 3860,    0,    0,    0,
    0,   64,    0,    0, 1237,    0, 2374,    0,    0,    0,
    0,    0, 1986, 1374,    0, 2447,    0,    0,    0,    0,
 2248,    0,  127, 2802, 1463,    0,    0, 2147, 1629,    0,
    0, 1515, 2281,    0, 2281,   28,    0,    0,    0, 1083,
 1654,    0, 2788,    0, 2501, 7406, 1347, 7641,   64,  831,
 7641, 2790,  856,  955,    0,    0,    0,  504,  504,  153,
  211,    0,    0,    0, 2447,  640, 1986, 2456, 2468,    0,
    0, 1463, 2812,  127, 2519, 2281, 2824, 1902, 2308, 2308,
  504,    0, 2835, 2836,  939,    0,    0,    0, 5824, 2474,
 2838,   64,    0, 1325,  242, 2443, 1629,    0, 2578,    0,
   64,    0,    0,    0,    0, 4349, 1237, 1986,  641,  641,
    0, 2584,    0, 2444,    0,    0,    0, 1463,  127,    0,
    0, 2147, 2500, 2500,    0,    0, 1083, 2853,    0,  504,
    0, 1814, 2085,  121, 2843, 1851,   64, 2601, 2592, 4349,
 4349,    0,    0,    0,    0,    0, 2440,  656,    0,    0,
    0, 1353, 2281,    0,    0,    0,  504,    0,    0,   64,
 2863,    0,    0,  441,  504, 4349, 2602,    0,    0,    0,
    0, 3860, 2308, 2867, 2871, 2104, 4349,    0,    0, 4349,
   28, 2500,  504,    0,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  168,  909,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  182, 4337,    0,    0,    0,    0,    0, 2915,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  994,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  817,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2581,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2532,    0,    0,    0,  133,    0,
  133,    0,    0,    0,    0, 2536,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1147,    0,
    0, 5977, 4413,    0,    0,    0, 1341,    0,    0,  484,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  389,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    4,    0,    0,    0,    0,  411,    0,
    0, 1388,    0,    0,    0,    0,    0, 1207,    0, 8688,
    0,    0,    0,    0,    0,    0,    0,    0,    4,    0,
  680,    0,    0, 2881,    0,    0,    0, 2658,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1396,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1431,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  880,    0,    0,
    0,    0,    0,    0,    0, 2467,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  643,    0,    0, 1435,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    4,
    0, 2881,    0, 2658,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1454,    0, 2653,    0,    0,    0,    0,
 1458,    0,    0,    0,    0,    0, 2665, 1081, 6383,    0,
 1495,    0,    0,    0,    0,  230,    0,    0,    0, 2485,
 2485,    0,    0,    0, 1528, 1338,    0,    0,    0,    0,
    0,    0,    0, 2665,    0,    0, 1694,    0,    0,    0,
    0,    0,    0, 8881,    0, 2548,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1540,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1545,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1588,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2300,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1942,    0,    0,  130,    0,    0, 2619,    0,  429,    0,
    0,    0,    0,  175,    0,  175,    0,    0,  739,    0,
  851, 1017,    0,    0,    0,    0, 1396,    0, 1000, 2665,
    0,    0,  137,  949,    0,    0,   95,    0, 1691,  249,
    0,    0,    0,    0,    0,    0, 2485, 2485, 2485, 2485,
    0,  253,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2679,    0,    0,    0,
    0,    0, 2653,    0,    0,  336,    0,  850, 2482,    0,
    0,    0,    0,    0,    0,  867,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1631,    0,    0,    0,    0,
  915, 1097,    0,    0,    0, 1553,    0,    0, 4135,    0,
    0, 4159,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2019, 2146,    0,    0,    0,
    0, 4210, 1773,    0, 4159,    4,    0,    0,    0,    0,
    0,    0,    0,    0, 1081, 1316,    0,    0,    0,    0,
 2521,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  921,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  287,    0,    0, 4337,    0,    0,
  287,    0,    0,    0,    0,    0, 7705,    0, 7705,    0,
    0, 7705, 7705, 7705,    0,    0,    0, 2493,    0,    0,
    0,    0, 2969, 3120, 6059,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  753,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1081,    0,    0,
  111,    0,    0,  701,    0,    0,    0, 3205,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  972,    0,    0,    0,    0,    0,    0,    0, 2912,    0,
    0, 1532, 2962,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  251,  405,  679,    0,    0,    0, 2669,    0,
    0,    0,    0,    0, 1062,    0,    0,    0,    0,    0,
 4135,    0,    0,    0,    0,  933,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2665,    0,    0,  360,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9026,    0,
    0,    0,  277,    0,    0,    0,    0, 2645,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1635, 1663,    0,    0,    0,    0,    0,    0,
    0, 1680,    0,    0, 5977,    0, 1680, 1734, 1743,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  688,    0,    0, 7705,    0,    0,    0,
 1263,    0,    0,    0, 2448, 3276, 3433, 2605, 2813,    0,
    0,    0,    0,    0, 2044,  895, 1428,    0,    0,    0,
  731,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  322,  904,    0,
  169,    0,    0,    0,    0,    0,    0,    0, 2485, 2498,
    0, 3285,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2685,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1808,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1744,    0,    0,    0,    0,
    0,    0,    0, 2517,    0,    0,    0,    0,    0,    0,
    0,    0, 1705,    0,    0,    0,    0,    0,    0,    0,
 1316, 2685, 2485, 1746, 1081,    0, 2696, 5977, 1786,  114,
    0,    0,    0,    0, 9026,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2182,    0,
    0,    0, 6841,    0,    0,    0,    0,    0,    0,    0,
 7695,    0, 7899,    0, 7910,    0,    0,    0,    0,    0,
    0,    0, 7933,    0,    0, 8049,    0,    0, 8178,    0,
 8212,    0,    0, 8241,    0,    0, 8318,    0,    0, 5252,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1635, 1837,    0, 2524,  323,    0,    0,    0,    0,    0,
    0,    0, 1840, 1847, 1866,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2547,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2620, 1361,    0,
 2045,    0, 2623,    0, 2627, 2635,    0,    0,  225,    0,
    0,    0, 2685,    0, 1786,    0,    0,    0, 1853,    0,
    0,    0, 2518,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2502,    0, 2710,    0,    0,    0,
 2422,    0, 1808, 2581,    0,    0,    0,    0,    0,  694,
  700,  704,  715,  787,  865,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1065,
    0,    0,    0,    0, 2995,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1882,    0,    0,    0,    0,    0,
    0,    0, 1900,    0,    0,    0,    0,    0,  164,    0,
    0,    0,    0,    0,    0,    0,    0, 6059,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8382,    0, 8414,
    0, 8498,    0,    0,    0,    0, 8527,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1905,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1778,    0,    0,    0,
 1477,    0,    0,    0,    0,    0,    0,  814,    0,    0,
    0,    0, 2485,    0,    0,    0, 2414,    0,    0,    0,
    0,    0,    0, 2671,    0,    0, 2946, 1904, 2891,    0,
 2653,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2685,    0,    0,    0,    0,    0,    0,    0, 1920,
 1071,    0,    0,    0,    0,    0,    0,    0,    0, 2685,
 2696,    0,  368,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  238,    0, 8616,    0,    0, 7705,
    0,    0,    0,    0, 8666,    0,    0,    0,    0, 8753,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  323,  929,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  838,    0, 2620,
    0,  439,    0,    0,  534,    0,    0,    0,    0,    0,
    0,  450, 2951,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2687,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 3074,    0,    0,  105,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  210,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8803, 8866,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  887,    0,    0,    0,
  844,    0,    0,    0,    0,    0,    0, 2958,    0,    0,
    0,    0,    0, 2941,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2942,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2695,    0,    0,    0,
    0,    0, 3074,  576,    0,  160,    0,    0,    0,    0,
    0,    0,    0,  265,  100,    0,    0,  131, 2946,    0,
    0,    0,  299,    0,  315, 2807,    0,    0,    0, 2951,
    0,    0,    0,    0,    0,  113,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  505,    0, 1808,  707,  778,    0,
    0, 2620, 1965,    0,    0,  148,    0,  838,  260,   93,
 1668,    0,    0, 2966,    0,    0,    0,    0,    0, 2949,
  311,    0,    0,    0, 2712,    0, 2950,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3074,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  579,    0,    0,
    0,  844,  331, 2970,    0,    0, 2951,    0,    0,    0,
    0, 2971,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2620,  315,    0,    0,    0,    0,    0,    0,    0,
 2961,    0,    0,    0,    0,    0, 3513,    0,    0,    0,
    0,    0,   93, 2981,    0,    0,    0,    0,    0,    0,
  855, 2970,    0,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 3023,    0,    0, 3025,
    0,    0,    0,    0,    0,    0,    0,   11,    0, 3032,
    2,  283,   77, -478, 1917,  291, -194,   83, 3022, 2784,
    0,   75,    0, 2794,  -91,   33,    0,  431, -168,  -27,
  -42,  -22, -134, 2799, 2397,    0,    0,    0,  978,    0,
    0,  -51, 1991, -534,   19, -779, 2446, 2219, -291, 1997,
 2972, -268,    0,  -57,    0, -667,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -287,  267, 2001, -286, 1033, -814,
    0,    0,    0,    0, -315, -107,   89, 2015, -317,  -45,
    0, -501,    0,    0,  264, -968,    0, 1660,    0,    0,
 2758, -587,   13,    0,    0,    0,    0, 2743,    0, 2514,
 2779, -212, 2038, -661,    0,    0,    0, -686,    0,    0,
    0,    0,    0,    0, 2099,    0, 2791, 2356, 2041, 2769,
 2780,    0, 1499,    0, -491, 2341,   29, -603, -704,    0,
    0,  -34, -905, -884, -864, -213, -860,    0, 1726, -936,
    0, 1421,-1151, 1728,    0, 2572,    0,    0,    0, 1422,
 1424, 1380,  407, -276, -154, -215,    0,    0, 2361,    0,
 1910,    0,    0, -234,    0,    0,    0,    0, -188,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1507,  847,    0,    0,  254,    0,-1129,
 -131,    0,    0, 1339, 1496,    0, 1400,    0,    0, 2757,
  -86, 2163, 1946, 1777,    0,    0,    0, -984, -826, -841,
    0, -470, -147, -352,    0, -256, 1497, 1074,   90, -449,
 -719, -734,-1144,    0,    0,    1,    0,    0,    0,    0,
    0, 2818,    0,    0,    0,    0, 2833, 2609,    0,    0,
    0,    0,    0, 2846,    0,    0,    0,    0,  -15,    0,
    0,    0, -318, 2344, -312, 1911, -403,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2561,    0,    0, 2357,
    0,    0,    0, 2754, 3147,    0,    0,    0,    0,    0,
 2883,    0,    0,    0,    0, 2700,    0, 2690,    0,    0,
    0, 2451,    0,    0,    0, -324,    0,  189,    0, 2445,
    0,    0,    0, -592, -911, -549,  876,    0, 3047, 1862,
    0, 2475, -630, 2483,    0, -744, 2231,    0,    0, -102,
 -662,    0, 1589,-1119,    0,-1108,-1102,-1099,    0, 1700,
    0, 1438,    0, 1487,    0, 1383,    0,    0,    0,    0,
 2711,    0,    0,    0, -338, 2538, 2302, -774,    0,    0,
    0, -675,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2504, 2242, 2107,    0, -553,    0,
    0,    0, -224,    0, 2832,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -226, 2048,    0,   -9,
};
#define YYTABLESIZE 9524
static short yytable[] = {                                     110,
  130,  171,  110,  194,   76,   75,  131,  186,  137,  241,
   37,  378,  260,  263,  190,  549,  724,  554,  521,  926,
  586,  519,  374,  943,  225,  968,  587,  288,  761,  749,
 1210,  841,  752,  261,  264,  229,  844,  895,  763,  206,
 1349,  170,  173,  176,  131,  181,  962,  211,  234,  978,
  193,  170,  197,  465,  176,  656,  131,  181, 1129,  210,
  110, 1383,  233,  969,  130,  885,  710,  600,  221,  235,
  131,  224,  170,  232,  221,  197,  173,  176,  834,  131,
  181,  210,  110,  201,  176,  174, 1200,  616,  191,  917,
  511,  294,  927, 1200,  636,  964,  199,  228,  485,  372,
 1164,  193,  131,  193,  755, 1047,  231, 1201,  238,  388,
  629,  944,  708,  366, 1201,  197,  351,  313,  388,  230,
  289,  435,  176,  131,  210,  967,  236, 1202,  177,  740,
  687, 1204, 1022,  706, 1202,  245,  217,  270, 1204,  247,
  204,  249,  340,  372,  940,  755,  378,  699,  755,  377,
   74,  629,  995,  636,  629, 1021,  825,  374,  372,  257,
 1780, 1002,  653,  755,  290,  942, 1643,  575,  635,  629,
  740,  329,  366,  740,  603,  258,  755,  217,  435,  942,
  217,  575,  737,  888,  486, 1211,  248, 1605,  740,  687,
 1460,  739,  740,  741,  742,  217, 1219,   74,  259, 1168,
  257,  373,  492,  257,   90,  663,  699,  292,  355,  701,
  245,  586, 1661,  727, 1042, 1240,  355,  587,  257, 1118,
  471, 1258,  262,  719,  699,  794,  575,  635,  193,  484,
   74,  951,  131,  603,  390,  110, 1477, 1027,  554,   74,
  575,  131,   74,  390, 1235,  259,  769, 1479,  476,  723,
  394,  651,  478, 1480, 1727, 1130, 1481,  259,  812,  706,
  809,  173,  736,  259,  371,  131,  381,  269,  701, 1556,
 1398,  529, 1109, 1110, 1381,  535, 1419,  489,  533,  297,
  550,  533,  552,  699,  377, 1721,  746, 1317,  484,  860,
 1009,  486,  795, 1612,  115,  806,  604,  125,  699,  193,
  706,  608,  116, 1418,  979,  704,  193,  476,  193,  394,
  193,  478, 1257,  193,  598,  484,  384,   86,  706, 1220,
   86,  216,  531,  371, 1608,  384,  867,  399,  298,   57,
  623,  176,  181, 1200, 1161,  470,  373,  520,  345,  699,
  131,  265,  173, 1055,  523,  746,  191,  265,  536,  478,
 1313,  539,  804,  759, 1201,  699,   76,  699,  213,  126,
 1361, 1183,  216,  851,  851,  216,  851,  196,  851,  327,
 1362,  623,  131,  596, 1202,  176,  390,  980, 1204,  470,
  216,  531, 1635,  131,  489,  266, 1609,  131,  634,  623,
  176,  893,  131,  740,  470,  137,  214,   14, 1028, 1121,
  254,  217,  126, 1197,  392, 1453, 1218,  701,  255,  127,
  388, 1133, 1135, 1393,  861,  269, 1415,  650,  126,  378,
  941,  805,  484, 1221,  606,  689,  196, 1351,  372,  629,
  864,  372,  650, 1560, 1468,  636,  346,   14,  690,  217,
  789,  870, 1381,  685,  102,  971,  808,  634,  102,  698,
  740,  629,  999,  738,  465,  217,  647,  990,  102,  706,
  992, 1048,  687,  392, 1314, 1408,  701,  655, 1459,  388,
  892,  661,  657,   90,  998, 1477,  384,  217,  664,  690,
  706, 1684,  690,  677, 1721,  496, 1479,  253,  755,  465,
  698,  735, 1480,  484, 1067, 1481, 1691,  690,  718,  791,
 1345, 1016,  102, 1365,  257, 1758,  476, 1345,  698,  635,
  478,  533,  465,  113,  394, 1726,  755,  603,  388,  758,
  193, 1280,  834,  329,  102,  758,  257,  677,  244,  102,
  989, 1411,  102,  686,  963, 1731, 1711, 1057,  701, 1040,
  372,  701,  677,  699, 1128,  257, 1384,  533,  257,  636,
  819,  820,  372,  707,  372,  476, 1491,  865,  765,  478,
 1526,   76, 1527,  257,   73,  699,  102,  790,  366, 1062,
  986,  687, 1396,  652,  686,  755, 1766,  575,  373, 1753,
  603,  102,  740,  603,  102,  687,  216,  102,  699,  102,
  740, 1251,  686,  371,  257,  193,  371, 1554,  102,  603,
  955,  851,  699, 1569,  837,  344,  755,  851, 1370,  863,
 1470,   73,  699,  629,   76,  835,  755,  614,  575,  755,
  486,  699,  373,  635,  216,  603,  614,  484, 1385,  614,
 1117,  984,  985,  191,  755,  193,   80,  373,  390,  217,
  216,  257,  598,  390,   73, 1565, 1410,  706,  394,  533,
  701,  257,  390,   73,  746,  176,   73, 1321,  357,  986,
  197,  257,  216, 1192,  701,  699,  811,  476,  392,  388,
 1601,  478,  465,  866,  388, 1603, 1553, 1586,  393, 1488,
  388,  934,  899,  974,  974, 1430,  699, 1454,   86,  193,
  531,  176, 1706,  399,  706,  699,  490,  650, 1054,  400,
  638,  598,  699,  396,  533,  371,  253,  131,  126,  131,
 1209,  699,  193,  489,  395,  860,  196,  371,  193,  371,
  934,  327, 1217,  193, 1638,  138,  701, 1253,  674,  634,
  687,  674, 1111,  920,  921, 1114,  701,  393,  604,  701,
 1698, 1642, 1239,  193,  889, 1797,  674,  253,  701,  657,
  253,  484,  399,   94, 1124, 1200,  191,  690,  400,  638,
  988,  381,  396, 1057, 1263,  253,  958,  378,  698, 1482,
  690,  193,  193,  395,  378, 1423, 1201,  254, 1203,  690,
  486,  523,  257,  959,  257, 1203,  398, 1015, 1371,  687,
  698,  478,  257,  961, 1087,  701, 1202,  604, 1007, 1025,
 1204, 1607,  392, 1590,  701,  677, 1490,  131,  388, 1008,
 1010, 1088,  677,  706,  314,  677,  577,  131,  254,  677,
  301,  254, 1455, 1631,  216, 1690,  690,  677, 1115, 1064,
 1037, 1038, 1039, 1397, 1697,  690,  254,  698,   76, 1407,
  861, 1113, 1539,  677, 1608,  398,  698, 1324,  701, 1116,
  614,  897,  686,  755,  376,  755, 1089,  390,  176,  181,
  390,  210,  110,  755,  397,  686,  408,  677,  390,  302,
 1125, 1716,  706,  489,  686,  577, 1736,  922,  701,  690,
  390,  390, 1222,  257,  687, 1090,  687, 1252, 1657,  182,
  698, 1041,  315,  690,  660,  376, 1719,  677,  376, 1180,
 1112, 1052,  677,  632,  698,  257, 1609,  373,  498,  614,
  373,  484,  257,  376,  531,  465, 1563,  499,  943,  955,
  700,  686,  685,  397,  677,  408, 1337,  687,  701, 1339,
  686,  111,  136,  755,  176,  660,  677,  737,  677, 1392,
 1224, 1729,  393,  474,  632,  687,  257,  632,  622, 1306,
 1181, 1308, 1091,  660,  755,  531, 1790,  399,  531,  755,
 1122,  700,  632,  400,  102,  658,  123,  396, 1424,  701,
 1414,  533, 1134,  531,  686,  531,  755, 1225,  395,  700,
  198,  303, 1587,  755,  253,  598,  253,  701,  686,  622,
 1375,  136,  659,  623,  253,  674,  674, 1591, 1721,  112,
 1604,  324,  816, 1345, 1656,  586,  257,  622, 1791,  860,
 1230,  587,  533, 1720,  378,  533,  599,  755, 1767,  373,
  598,   76, 1231, 1223, 1230, 1203, 1670,  860, 1359, 1673,
  533,  373,  533,  373, 1005,  193, 1412, 1311, 1311, 1315,
  957,  638,  193,  957,  465,  970, 1345, 1487,  598,  687,
  398,  598,  623, 1564, 1246,  254,  860,  254,  957, 1768,
  957,  528,  687,  860,  137,  254,  783,  598, 1350,  265,
  138,  687, 1266,  191, 1078,  599,  393,  755, 1270,  325,
  381,  604, 1730,  603, 1367,  253, 1369, 1648,  776,  129,
  777,  399,  860,  598,  769,  341,  543,  400,  674,  476,
 1079,  396,  528,  784,  465,  528, 1769,  253,  390,  382,
  390, 1461,  395,  896,  253, 1464,  604,  860,  687,  306,
  528,  139,  528,  137,   76,   76,  390,  687,  397,  138,
  408,  342,  706, 1561,  861,  102,  543,  543,  974,  674,
  543,  123,  123, 1485,  604, 1413,  573,  604,  253, 1743,
 1744, 1306,  861,  222,  706,  543,  254,  543,  607,  371,
  695,  289,  176,  604,  370,  696,  701,   73,  307,  701,
 1336,  687,  531,  116,  140,  687,  376,  713,  254, 1156,
 1157,  861,  259,  376,  398,  254,  376,  543,  861,  604,
  376,  531,  531,  614,  531,  354,  184,  185,  376,  531,
  216,  706,  531,  360, 1801,  573,  152,  785,  253,  607,
  136,  116,  136,  660,  376,  531, 1422,  861,  687,  254,
 1426,  531,  632,  660, 1472,  701,  660, 1742,  614,  533,
 1153,  687,   76, 1395,  701,  660, 1589, 1546,  376,  700,
  687,  132,  861, 1802,  632,  533,  116,  977,  714,  700,
  218,  474,  700, 1474,  706, 1552,  614,  701,  691,  614,
  701,  700,  397,  957,  692,  152, 1641,  622,  376, 1212,
 1213,  486,  533,  133,  687,  614, 1748, 1626,  533,  254,
  308,  533,  660,  687,  646,  265,  457,  645,  134,  622,
  437,  660,  517,  531,  462,  376,  860,  760,  309,  464,
 1699,  614, 1700,  868,  957,  518,  868,  376,  700,  376,
  390,  136,  623, 1749,  974,  531,  701,  700, 1264,  528,
  977,  226,  531, 1158,  760,  701, 1750, 1492,  493,  901,
 1639,  531,  310,  531,  623,  660, 1640,  153,  528,  528,
  582,  528,  380, 1740,  137,  176,  528,  660,  380,  528,
  138,  531,  475,  237,  543,  868,  531,  754,  242,  599,
  753,  700,  528,  193,  489, 1311,  695,  693,  528,  694,
  543,  696,  131,  543,  543,  700,  543,  695,  693, 1602,
  694,  543,  696,  533,  543,  531, 1692,  579,  695,  693,
  533,  694,  242,  696,  599,  701,  153,  543,  691,  582,
  242,  191,  484,  543,  692, 1713,  543,  476,  533,  486,
  697,  695,  693,  238,  694,  361,  696,  265, 1774, 1775,
 1793,  861,  599,  257,  477,  599, 1596,  661,  695,  693,
  355,  694,  766,  696,  581, 1606,  378, 1738,  533, 1653,
  528,  599,  533, 1655,  265, 1636,  579, 1203,   77,   78,
  954, 1597,   76,  415,  701, 1489, 1537,  416,  243, 1588,
  678,  903,  528, 1472,  683,  684,  686,  599,  661,  528,
 1342,  661,  362,  507,  250,  543,  662, 1808,  120,  121,
  528,  242, 1772,  170,  542,  152,  661, 1637,  905,  355,
 1676, 1764, 1474,  581,  412,  543, 1165,  543,  528, 1677,
 1696, 1555,  489,  528,  543,  246, 1613,  152,  543, 1598,
 1650,  711,  415,  767, 1651,  543,  416,  662,  934, 1474,
  662,  867,  242,  131,  508, 1787, 1788,  161,  173,  386,
  276,  223,  528,  543,  251,  662,  152, 1676,  543,  353,
  484,  130, 1685,  102,   44, 1167,  102,  131, 1305, 1678,
 1307, 1799,  544,  412,  768,  152,  695,  693,  509,  694,
 1679,  696, 1806,  543,  152, 1807, 1474,  543,  695,  693,
  152,  694,  223,  696,  388,  223,  510,  363,  364,  365,
 1676,  184,  185,  934,  193,  388,  161,   52,  152, 1763,
  223, 1241,  544,  544, 1773,  868,  544,  176,  353,  370,
  366,  367,  252,   44,  691,  368,  369, 1634,  256, 1474,
  692,  544,  489,  544, 1173,  691,  153,  259, 1474,  121,
 1722,  692, 1676,  486,  271, 1676,  691,  130, 1174,  287,
  162,  869,  692,  131,   42, 1596,  870,  296,  153, 1666,
 1242, 1529,  871,  544, 1701,  299,   52, 1284, 1676,  691,
  131, 1474, 1175,  102, 1474,  692,  843,  486,  486,  489,
 1597,  872,   25, 1550, 1680, 1757,  691,  153,  170, 1724,
  695,  693,  692,  694, 1173,  696, 1592, 1474,  130,  751,
   76, 1725,  663,  486,  131, 1701,  153, 1712, 1174,  162,
  177,  131, 1241,   42,  486,  153,  663,  486,  370, 1778,
  464,  153,  489,  900,  663,  316, 1176, 1152,  916,  327,
 1531,  489, 1175,  923,  924, 1065,  489, 1066, 1598,  153,
  663,   25,  931,  130,  935,  936,  937,  938,  939,  131,
  931, 1701, 1532,   48, 1695,  464, 1792,  131,  751, 1533,
  173, 1245,   50,  486,  489,  128,  661,  489,  508,  177,
  489,  489,  695,  693,  484,  694,  661,  696,  464,  661,
  695,  693, 1162,  694,  493,  696, 1176,  173,  661,  329,
  489,  685,  538, 1534,  663, 1064,  489,  695,  693, 1671,
  694, 1695,  696,  338, 1173,  383,   96,  489,  484,  484,
  489,  348,   48,  173,  691,  662,  862,  508, 1174,  644,
  692,   50,  486,  352,  128,  662,  691,  251,  662,  223,
  544,  223,  692,  538,  484,  661,  538,  662,  869,  223,
  685,  869, 1175,   98,  661,  484,  544, 1695,  484,  544,
  544,  538,  544,  538,  272,  701,   26,  544, 1439,   49,
  544,  700,  695,  693,  383,  694,   51,  696,  251,  350,
  361,  251,  181,  544,  928,  695,  693,  356,  694,  544,
  696, 1695,  544,  538,  662,   46,  251,  357,  661,  273,
  869,  884,  385,  662,  883, 1675, 1176, 1068, 1069, 1073,
  661,  129, 1081, 1083, 1085,  394, 1093, 1096, 1099, 1101,
 1104, 1107,   99,  181, 1721,   26,  181,  274,   49,  117,
  275,  393,  100,  217,   47,   51,  330,  362,  691, 1783,
  223,  181,  318,  397,  692,  319,  276,  662,  464,  217,
  412,  398, 1675,  404,   46, 1137, 1138,  101, 1140,  662,
 1142,  544,  223, 1145, 1146, 1147, 1148, 1149, 1150,  223,
  129,  957,  277,  331,  217,  332,  400,  217,  177,  361,
 1154,  402,  924,  544,  695,  693,  403,  694,  117,  696,
  544,  320,  217,   47,  544, 1675,  470, 1046,  333,  177,
 1045,  544, 1275,  223, 1762,  472, 1053,  410,  217,  830,
  482,  953,  957,  957,  957,  957,  957,  970,  957,  544,
  691,  177, 1166,  334,  544, 1165,  692,  177,  691,  473,
  957,  957,  957,  957,  692,  717,  362, 1675, 1184,   14,
 1675, 1045,  363,  364,  365,  691,  184,  185,  510,  544,
  177,  692, 1191,  544,   73, 1190,  102,  497, 1233,   95,
  538, 1232,  957, 1675,  957,  366,  367,  419,  506,  177,
  368,  369,  717,  670,  668,  514,  538,  527,  177,  538,
  538,  534,  538,  537,  177,  717,  425,  538, 1238,  510,
  538, 1237,  510,  547,  428,  429,  430, 1319,  184,  185,
 1318,   96,  177,  538,  551,   95,  601,  510,  605,  538,
  691,  416,  538,   97,  670,  668,  692,  670,  668, 1432,
 1017, 1018, 1165,  691, 1144,  695,  693,  418,  694,  692,
  696,  607,  670,  668,  363,  364,  365,  931,   98,  177,
  181,  363,  364,  365,  609,  184,  185,   96, 1467, 1287,
 1288, 1466, 1290, 1292, 1469, 1779,  639, 1466, 1466,   97,
 1297,  181,  368,  369,  366,  367,  420,  421,  422,  368,
  369,  423,  424,  114, 1805,  533,  640, 1721,  641,  426,
  427,  538,  644,  181,   98,  124,  124,  648,  431,  181,
  653,  464, 1322,  695,  693,  662,  694,  665,  696,  698,
 1323,  695,  693,  538,  694, 1328,  696,   99,  667,  668,
  538,  820,  181, 1335,  538,  924,  533,  100,  924,  533,
 1154,  538,  730,  669, 1343,  924, 1346, 1347,  692,  670,
  671,  181,  924,  957,  533,  957,  957,  672,  673,  538,
  181,  674,  101,  957,  538,  675,  181,  676,  681,  682,
  690,  957,  820,   99,   49,  820,  701,  957,  699,  703,
  957,  717,  721,  100,  181,   50,  722,  726,  728,  538,
  820, 1377,   51,  538,  957,  732,  729,  733,  743,  957,
  957,  498,  744,  957,  746,  747,  756,  780,  101,  781,
  957,  782,  957,  957,  787,  257,  957,   52,  792,  796,
  957,  181,  797,  957,  798,  957,  102,  957,  799,  802,
  801,  817,  957,  957,  807,  957,  374, 1326,  695,  693,
  464,  694,  813,  696,  814,  818,  510,  827,  510,  838,
  374,  957,  957,  510,  957,  829,  510, 1427,  374, 1327,
  695,  693,  838,  694,  830,  696, 1435,  839,  957,  510,
  957,  957,  957, 1440,  374,  957,  840,  842,  957,  957,
  881,  878,   53,  691,  879,  886,  890,  891,  957,  692,
  838,  838,  838,  838,  838,  929,  838, 1329,  695,  693,
  464,  694,  957,  696,  950,  957,  956,  965,  838,  838,
  838,  838,  670,  668,   54,  670,  668,  918,   55,  972,
  924,  957,  670,  668,  924,  670,  668,  957,  374,  670,
  668,  991,  957,  986,  670,  668,  808,  670,  668,  996,
  997,  999,  838,  957,  957,  759,  957,  510, 1006, 1003,
 1004,  691,  957,  670,  668, 1020, 1012,  692, 1013,  691,
 1014, 1023, 1029,  196,   95,  692, 1330,  695,  693,  510,
  694,  230,  696,  533, 1034,  533,  510,  670,  668, 1036,
  533,  670,  668,  533, 1060, 1061, 1043, 1075, 1048, 1120,
  670,  668,  658,  820,  957, 1123,  533,  850, 1126, 1331,
  695,  693, 1136,  694, 1118,  696,   96,  670,  668, 1139,
  510, 1141,  230, 1143, 1151,  230, 1163, 1170,   97,  691,
 1574, 1575,  196, 1172, 1186, 1178,  645, 1332,  695,  693,
  230,  694, 1193,  696,  670,  668, 1188, 1214,  850,  850,
  850,  850,  850,   98,  850,  924,  670,  668,  924, 1196,
  820, 1216, 1205,  820,  670, 1234,  850,  850,  850,  850,
  820, 1226, 1236,  820, 1243, 1715,  533,  820, 1247, 1248,
 1249, 1250,  820, 1259,  533,  820,  691, 1261,  894, 1054,
  898, 1615,  692,  902,  904,  906, 1268, 1272, 1271, 1273,
  850,  820,  695,  693, 1276,  694,  533,  696,  691, 1274,
 1277, 1282, 1281,  533,  692, 1283,  860,  533, 1756, 1708,
  533,  838,   99,  838,  838,  820, 1285, 1286,  820,  820,
 1293,  838,  100, 1294, 1295,  533, 1296,  533,  820,  838,
 1338, 1298, 1299, 1300, 1301,  838,  691,  533,  838, 1302,
 1303, 1304,  692, 1649, 1341,  820, 1320,  101, 1348, 1352,
 1781, 1652,  838, 1784,  853, 1355, 1353,  838,  838, 1354,
 1356,  838, 1357, 1366, 1363,  102, 1368,  820,  838, 1372,
  838,  838,  820, 1374,  838,  529, 1796, 1382,  838, 1386,
 1387,  838, 1388,  838,  820,  838,  820, 1389, 1665, 1394,
  838,  838, 1667,  838, 1399,  853,  853,  853,  853,  853,
 1409,  853, 1333,  695,  693,  691,  694, 1416,  696,  838,
  838,  692,  838,  853,  853,  853,  853, 1420,  196, 1390,
   81,  196, 1421, 1425, 1278, 1391,  838,   82,  838,  838,
  838,   83,   84,  838, 1431, 1452,  838,  838,  691, 1451,
   85, 1456,  196, 1462,  692, 1463,  838,  853,  924,  242,
 1714,  230,   86, 1717, 1483, 1528, 1538, 1540, 1541,  850,
  838,  850,  850,  838,  196, 1542,  691, 1543, 1545,  850,
  196, 1544,  692, 1547, 1548, 1549, 1557,  196, 1559,  838,
 1562, 1566, 1567,  850, 1568,  838,  850, 1570, 1571, 1572,
  838, 1752, 1573,  196, 1576, 1577, 1578, 1579, 1580, 1581,
  850,  838,  838, 1582,  838,  850,  850, 1583, 1584,  850,
  838, 1585,  196, 1593, 1594, 1595,  850,   87,  850,  850,
  196,  196,  850, 1611,  533, 1614,  850,  196,  112,  850,
  691,  850, 1616,  850,   88, 1617,  692, 1618,  850,  850,
 1619,  850, 1620, 1621,  533,  196,  533, 1623, 1622,   89,
  242,  533,  838,   90,  533, 1624,  665,  850,  850, 1629,
  850, 1630,  854, 1632,   91, 1633, 1654,  533, 1659,  507,
 1660, 1669,  242,  533,  850, 1625,  850,  850,  850,  242,
 1686,  850,  196,   92,  850,  850, 1334,  695,  693, 1732,
  694,  553,  696,  196,  850, 1694, 1707,  665, 1718,   59,
  665, 1734, 1737,  854,  854,  854,  854,  854,  850,  854,
   60,  850, 1739,  242, 1741,  665,  853,   61,  853,  853,
  508,  854,  854,  854,  854, 1746,  853,  850, 1760, 1747,
   62, 1466, 1761,  850, 1754, 1729, 1730,  139,  850,  498,
  231,  691, 1777,  853, 1785,  533, 1786,  692,  499,  850,
  850, 1782,  850, 1789,  509,  854, 1721,  853,  850, 1800,
 1803, 1804,  853,  853,    1,   30,  853,  533,  629,   32,
  203,  755,  510,  853,  533,  853,  853,  571,  271,  853,
  380,  231,   82,  853,  231,  533,  853,  531,  853,  653,
  853,   63,  269,  149,  940,  853,  853,  196,  853,  231,
  850,  213,  467,  533,  531,  276,  193,   64,  533,  408,
  531,  500,  281,  685,  853,  853,  683,  853,  848,  196,
  679, 1457,  695,  693,   65,  694,  194,  696,  681,  289,
  275,  853,  939,  853,  853,  853,  381,  533,  853,   66,
  712,  853,  853,   67,  533,  287,   68,  711,  196,  223,
  324,  853,  500,  288,   69,  500,  713,  332,  323,  848,
  623,  848,  848,  848,  674,  853,  342,  196,  853,  330,
  500,  722,   26,   70,   29,  196,  196,  848,  848,  848,
  848,   39,  196,  117,  853,  533,  401, 1309,  533,  392,
  853,  695,  693,  391,  694,  853,  696, 1269, 1059,  880,
  196, 1267, 1265,  533,  828,  227,  853,  853, 1255,  853,
 1551,  848,  196,  516,  196,  853, 1458,  695,  693,  793,
  694, 1244,  696,  251,  854,  691,  854,  854,  560,  502,
 1195,  692,  491,  966,  854, 1256,  531,  196, 1465,  695,
  693, 1082,  694, 1086,  696, 1094, 1097,  532, 1102, 1105,
 1108,  854, 1662, 1001, 1530, 1728,  775,  853, 1536, 1771,
 1735, 1733,  196,  994,  251,  854, 1376,  251, 1668,  847,
  854,  854, 1683, 1798,  854,  665, 1765,  643,  665, 1185,
 1360,  854,  251,  854,  854,  665, 1486,  854,  665, 1693,
  540,  854,  665,  522,  854,  779,  854,  665,  854,  513,
  665, 1406, 1035,  854,  854,  815,  854, 1026,  666,  119,
  847,  483,  847,  847,  847,  716,  665,  957,  242,  269,
  231,  712,  854,  854, 1169,  854,  970,  952,  847,  847,
  847,  847, 1599, 1658, 1776, 1809,  953, 1745, 1132,  854,
  665,  854,  854,  854,  665, 1289,  854,  725,  887,  854,
  854,  642,    0,  665,  187, 1340,    0,    0,    0,  854,
  691,    0,  847,    0,    0,    0,  692,    0,    0,    0,
  665,    0,    0,  854,    0,    0,  854,    0,    0,    0,
  848,    0,  848,  848,    0,    0,    0,    0,    0,  500,
  848,  500,  854,    0,    0,    0,    0,  665,  854,  500,
    0,    0,    0,  854,    0,    0,    0,  848,    0,  665,
    0, 1438,  500,  187,  854,  854,    0,  854,    0,  242,
    0,  848,  533,  854,  533,  849,  848,  848,    0,  691,
  848,    0,  533,    0,  188,  692,    0,  848,    0,  848,
  848,  242,    0,  848,    0,  533,    0,  848,  242,    0,
  848,    0,  848,    0,  848,  691,    0,    0,    0,  848,
  848,  692,  848,    0,    0,  854,  849,    0,  849,  849,
  849,    0,    0,    0,    0,    0,    0,  691,  848,  848,
    0,  848,  242,  692,  849,  849,  849,  849,    0,    0,
  500,    0,    0,  188,    0,  848,    0,  848,  848,  848,
    0,  251,  848,  251,    0,  848,  848,    0,    0,    0,
    0,  251,  500,    0,    0,  848,    0,    0,  849,  500,
    0,    0,    0,  533,    0,    0,    0,    0,    0,  848,
    0,  847,  848,  847,  847,    0,    0,    0,    0,    0,
    0,  847,    0,    0,    0,  533,    0,    0,  848,    0,
    0,    0,  533,  500,  848,    0,    0,    0,  847,  848,
  907,  908,  909,  910,  911,  912,  913,  914,    0,    0,
  848,  848,  847,  848,    0,    0,    0,  847,  847,  848,
    0,  847,  852,    0,    0,    0,  533,    0,  847,    0,
  847,  847,    0,    0,  847,    0,    0,    0,  847,    0,
    0,  847,  251,  847,    0,  847,    0,    0,    0,    0,
  847,  847,  187,  847,    0,    0,    0,    0,    0,    0,
    0,  848,    0,  852,  251,  852,  852,  852,    0,  847,
  847,  251,  847,  187,    0,    0,    0,    0,    0,    0,
    0,  852,  852,  852,  852,    0,  847,    0,  847,  847,
  847,    0,    0,  847,    0,  187,  847,  847,    0,    0,
    0,  187,    0,    0,    0,  251,  847,    0,    0,    0,
    0,    0,    0,    0,    0,  852,    0,    0,    0,    0,
  847,    0,    0,  847,  187,    0,    0,  849,    0,  849,
  849,    0,  188,    0,    0,    0,    0,  849,    0,  847,
    0,    0,    0,  187,    0,  847,    0,    0,    0,    0,
  847,    0,  187,  188,  849,    0,    0,    0,  187,    0,
    0,  847,  847,    0,  847,    0,    0,    0,  849,    0,
  847,    0,    0,  849,  849,  188,  187,  849,    0,    0,
    0,  188,    0,    0,  849,    0,  849,  849,    0,    0,
  849,    0,    0,    0,  849,    0,    0,  849,    0,  849,
    0,  849,    0,    0,  188,    0,  849,  849,    0,  849,
    0,  624,  847,  187,  432,    0,  433,    0,    0,    0,
    0,    0,    0,  188,    0,  849,  849,    0,  849,  435,
    0,    0,  188,    0,  259,    0,    0,    0,  188,    0,
    0,    0,  849,    0,  849,  849,  849,    0,    0,  849,
    0,    0,  849,  849,    0,    0,  188,    0,    0,    0,
    0,    0,  849,    0,    0,    0,    0,    0,    1,    0,
    0,    0,    0,    0,    0,    0,  849,    0,    0,  849,
    0,    0,    0,    0,  852,    0,  852,  852,    0,    0,
    2,    0,    0,  188,  852,  849,    0,    0,    3,    0,
    0,  849,    0,    0,    0,    4,  849,    5,    0,    6,
  624,  852,    0,  432,    7,  433,    0,  849,  849,    0,
  849,    8,    0,    0,    0,  852,  849,    0,  435,    0,
  852,  852,    0,  259,  852,    9,    0,    0,    0,    0,
    0,  852,    0,  852,  852,    0,   10,  852,    0,    0,
    0,  852,    0,    0,  852,    0,  852,    0,  852,    0,
    0,    0,    0,  852,  852,    0,  852,    0,  849, 1428,
 1429,    0,    0,  321,    0, 1433, 1434,    0, 1436, 1437,
    0,    0,  852,  852,    0,  852, 1441, 1442, 1443, 1444,
    0, 1445, 1446, 1447, 1448, 1449, 1450,    0,    0,  852,
    0,  852,  852,  852,  321,    0,  852,    0,    0,  852,
  852,  321,    0,   11,    0,  321,  321,    0,  321,  852,
   12,  321,    0,   13,    0,   14,   15,    0,    0,    0,
    0,    0,    0,  852,    0,  321,  852,    0,  406,    0,
    0,  321,    0,    0,    0,    0,    0,  407,    0,    0,
   16,    0,  852,    0,    0,    0,    0,    0,  852,    0,
    0,  408,    0,  852,  617,    0,    0,  361,  409,    0,
    0,    0,    0,    0,  852,  852,    0,  852,    0,    0,
    0,    0,    0,  852,    0,    0,  618,    0,    0,  624,
    0,    0,  432,    0,  433,  410,    0,    0,  411,   17,
    0,  321,    0,    0,    0,    0,    0,  435,    0,    0,
    0,    0,  259,    0,   18,    0,    0,    0,    0,    0,
  321,    0,    0,    0,    0,  852,  412,  413,    0,    0,
    0,    0,    0,    0,    0,  321,    0,  406,  414,    0,
    0,    0,  415,    0,    0,  321,  407,    0,    0,    0,
  619,    0,    0,    0,  321,    0,  321,    0,    0,    0,
  408,    0,    0,  321,    0,    0,  361,  409,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  618,    0,    0,    0,    0,
    0,    0,  321,  620,  410,    0,    0,  411,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,  321,    0,    0,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  412,  413,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,  414,    0,    0,
    0,  415,    0,  419,    0,    0,    0,    0,    0,  619,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,    0,    0,    0,
    0,  621,  622,  623,    0,    0,    0,    0,  685,    0,
    0,    0,  620,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  418,    0,  406,    0,    0,    0,
    0,    0,    0,    0,  561,  407,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,  408,
    0,    0,  419,    0,    0,  361,  409,    0,  551,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,  618,  561,  426,  427,  561,  428,
  429,  430,    0,  410,    0,  431,  411,    0,    0,    0,
  621,  622,  623,  561,    0,  561,    0,    0,    0,  551,
    0,    0,  551,    0,    0,    0,    0,    0,    0,  535,
    0,    0,    0,    0,  412,  413,    0,  551,    0,  551,
    0,    0,    0,    0,    0,  561,  414,    0,    0,    0,
  415,  561,    0,    0,    0,  562,    0,    0,  619,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  551,
  535,    0,    0,  535,  563,    0,    0,  564,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  535,    0,
  535,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  620,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,  565,    0,
  535,    0,    0,  418,  566,  567,  568,    0,  569,  570,
  571,    0,  572,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,  573,    0,  574,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0, 1374,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,  575,    0,    0,    0,  621,
  622,  623,    0,    0,    0,    0,  644,    0,  644,  644,
    0,  644,  576,    0,    6,    0,    0,    0,    0,    0,
    0,    0,  561,    0,  644, 1493, 1494,    0, 1495,  644,
    0, 1496,    0,    0,  577,    0,    0,    0,  561,    0,
    0,    0,  561,    0,  561, 1497,  551,    0,    0,  561,
    0, 1498,  561,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  551,    0,    0,  561,  551,    0,  551,  578,
  579,  561,    0,  551,  561,    0,  551,    0,    0,    0,
    0,    0,  642,    0,  642,  642,  580,  642,    0,  551,
    0,    0,    0,    0,    0,  551,    0,  535,  551,    0,
  642,    0,    0,    0,    0,  642,    0,    0,    0,    0,
    0, 1499,    0,  535,    0,    0,    0,  535,    0,  535,
    0,    0,    0,    0,  535,    0,    0,  535,    0,    0,
   14,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  535,    0,    0,  561,    0, 1500,  535,    0,    0,  535,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1625,  561, 1501,  551,    0,    0,
    0,    0,  561,  102,    0,    0,  561,    0,    0,    0,
    0,    0,    0,  561,    0,    0,    0,    0,    0,  551,
    0,    0,    0,    0,    0,    0,  551,    0,    0,    0,
  551,  561, 1502,    0,    0,    0,  561,  551,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  535,    0,
    0, 1503,    0,    0,    0,  551,  644,    0,    0,    0,
  551,  561,    0,  644,    0,  561,    0,    0,    0,    0,
  535,    0,  644,    0,    0,    0,    0,  535,    0, 1374,
    0,  535,    0,    0,    0,  551,  644,    0,  535,  551,
    0,  434,  644,  644,  432,    0,  433,  928,  695,  693,
  644,  694,    0,  696,    0,    0,  535,    0,    0,  435,
    6,  535,    0,    0,  259,    0,  858,  857,  859,    0,
  644, 1493, 1494,  644, 1495,    0,    0, 1496,    0,    0,
    0,    0,  642,    0,    0,    0,  535,    0,    0,  642,
  535, 1497,    0,    0,    0,    0,    0, 1498,  642,  644,
    0,  644,  644,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  642,  644,    0,    0,    0,  644,  642,  642,
    0,    0,    0,    0,  434,    0,  642,  432,    0,  433,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,  642,  259,    0,  642,
    0,    0,    0,    0,    0,    0,    0, 1499,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  644,  642,   14,  642,  642,    0,
  644,    0,    0,    0,    0,    0,    0,    0,    0,  642,
  644, 1500,    0,  642,    0,    0,    0,    0,    0,    0,
    0,   16,    0,    0,  644,  644,  644,  644,  644,  644,
    0,    0, 1501,    0,    0,    0,    0,    0,  644,  102,
    0,  434,    0,    0,  432,    0,  433,  644,  644,  644,
  644,  644,  644,  644,  644,  644,    0,  644,    0,  435,
    0,    0,  644,  644,  259,  644,  644,  644, 1502,    0,
  642,  644,    0,    0,    0,    0,  642,    0,    0,    0,
    0, 1070,    0,    0, 1071,    0,  642, 1503,  406,    0,
    0,    0,    0,    0,    0,    0,    0,  407,  845,    0,
  642,  642,  642,  642,  642,  642,  691,    0,    0,    0,
    0,  408,  692,    0,  642,  846,    0,  361,  409,    0,
    0,    0,    0,  642,  642,  642,  642,  642,  642,  642,
  642,  642,    0,  642,    0,    0,    0,    0,  642,  642,
    0,  642,  642,  642,    0,  410,    0,  642,  411,    0,
    0,  847,    0,    0,    0,    0,    0,    0,    0,    0,
  848,    0,    0,    0, 1080,    0,    0, 1071,  849,    0,
    0,  406,    0,    0,    0,    0,  412,  413,    0,    0,
  407,    0,    0,    0,    0,    0,    0,  850,  414,  851,
    0,    0,  415,    0,  408,    0,    0,    0,    0,    0,
  361,  409,    0,  852,    0,  853,  854,  855,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  434,    0,    0,  432,    0,  433,    0,    0,  410,    0,
    0,  411,    0,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,  259,    0,    0,    0, 1072,    0,  416,
    0,    0,    0,    0,    0,  417,  856,    0,    0,  412,
  413, 1084,    0,    0, 1071,  418,    0,    0,  406,    0,
    0,  414,    0,    0,    0,  415,    0,  407,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,  408,    0,  419,    0,    0,  434,  361,  409,  432,
    0,  433,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,  435,    0,    0,  426,  427,  259,
  428,  429,  430,    0,    0,  410,  431,    0,  411,    0,
 1072,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,  412,  413,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,  414,    0,
    0,    0,  415,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1072,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
 1092,    0,    0, 1071,    0,  418,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
  408,    0,    0,  419,    0,    0,  361,  409,    0,    0,
    0,  771,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,  410,    0,  431,  411,    0,    0,
    0,    0,    0,    0,    0,    0, 1095,    0,    0, 1071,
    0,    0,  771,  406,    0,  771,    0,    0,    0,    0,
    0,    0,  407,    0,    0,  412,  413,    0,    0,  434,
  771,    0,  432,    0,  433,    0,  408,  414,    0,    0,
    0,  415,  361,  409,    0,    0,    0,  435,    0,    0,
    0,    0,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1072,    0,  416,    0,
    0,  412,  413,    0,  417,    0,    0,    0,    0,    0,
    0,    0,    0,  414,  418,    0,    0,  415,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,  695,  693,    0,  694,    0,  696,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,  858,  857,  859,    0,  426,  427,    0,  428,
  429,  430, 1072,    0,  416,  431,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0, 1100,
    0,  431, 1071,    0,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,  434,  407,    0,  432,    0,  433,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  408,
    0,    0,  435,    0,    0,  361,  409,  259,    0,    0,
  771,    0,    0,  771,    0,    0,    0,    0,    0,    0,
  771,    0,    0,  771,    0,    0,    0,  771,    0,    0,
    0,    0,  771,  410,    0,  771,  411,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  771,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  412,  413,  434,    0,    0,  432,
    0,  433,    0,    0,    0,  771,  414,    0,  771,  771,
  415,    0,    0,    0,  435,    0,    0,    0,  771,  259,
    0,    0,    0,    0,  845,    0,    0,    0,    0,    0,
    0,    0,  691,    0,    0,  771,    0,    0,  692,    0,
    0,  846,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  771,    0,    0,
    0,    0,  771,    0,    0, 1072,    0,  416,    0,    0,
    0,    0,    0,  417,  771,    0,  771,  847,    0,    0,
    0,    0,    0,  418,    0,    0,  848,    0,    0,    0,
    0,    0,    0,    0,  849,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,  850,    0,  851,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,  852,
  425,  853,  854,  855, 1103,  426,  427, 1071,  428,  429,
  430,  406,    0,    0,  431,    0,    0,    0,    0,  434,
  407,  463,  432,    0,  433,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  408,    0,    0,  435,    0,    0,
  361,  409,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  410,    0,
    0,  411,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1106,    0,    0, 1071,
    0,    0,    0,  406,    0,    0,    0,    0,    0,  412,
  413,    0,  407,  434,    0,    0,  432,    0,  433,    0,
    0,  414,    0,    0,    0,  415,  408,    0,    0,  143,
    0,  435,  361,  409,    0,    0,  259,    0,  144,  145,
  146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
  156,  157,  158,  159,    0,    0,  160,  161,  162,  163,
  410,  164,    0,  411,  165,  166,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1072,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,  412,  413,    0,    0,    0,    0,    0,  418,    0,
    0,    0,    0,  414,    0,    0,    0,  415,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,  406,    0,    0,  431,
    0,    0, 1072,    0,  416,  407,  951,    0,  951,  951,
  417,  951,    0,    0,    0,    0,    0,    0,    0,  408,
  418,    0,    0,    0,  951,  361,  409,    0,    0,  951,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,  410,    0,    0,  411,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0,    0,
  406,  431,  362,    0,  412,  413,    0,    0,  644,  407,
    0,  644,    0,  644,    0,    0,  414,    0,    0,    0,
  415,    0,    0,  408,    0,    0,  644,    0,    0,  361,
  409,  644,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,  362,    0,  412,  413,
    0,    0,    0,  418,    0,    0,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,  685,    0,  951,  431,    0,    0,    0,    0,    0,
    0,  416,  951,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,  951,  418,    0,    0,
    0,    0,  951,  951,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
  951,    0,    0,  951,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0,  644,  426,
  427,    0,  428,  429,  430,  644,    0,    0,  431,  951,
    0,  951,  951,  434,  644,    0,  432,    0,  433,    0,
    0,    0,    0,  951,    0,    0,    0,  951,  644,    0,
    0,  435,    0,    0,  644,  644,  259,    0,    0,    0,
    0,    0,  644,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  644,    0,    0,  644,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  951,    0,    0,    0,    0,    0,
  951,    0,    0,  644,  644,    0,    0,    0,    0,    0,
  951,    0,    0,    0,    0,  644,    0,    0,    0,  644,
    0,    0,    0,    0,  951,  951,  951,  951,  951,  951,
    0,    0,    0,    0,    0,    0,    0,    0,  951,    0,
    0,    0,    0,    0,    0,    0,    0,  951,  951,  951,
  951,  951,  951,  951,  951,  951,    0,  951,    0,    0,
    0,    0,  951,  951,    0,  951,  951,  951,    0,    0,
    0,  951,    0,    0,    0,    0,  644,    0,    0,    0,
    0,    0,  644,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  644,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  644,  644,  644,  644,
  644,  644,    0,    0,    0,  561,    0,    0,    0,  562,
  644,    0,    0,    0,    0,    0,    0,  759,    0,  644,
  644,  644,  644,  644,  644,  644,  644,  644,  563,  644,
    0,  564,    0,    0,  644,  644,    0,  644,  644,  644,
  406,  434,    0,  644,  432,    0,  433,    0,    0,  407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  435,
    0,    0,    0,  408,  259,    0,    0,    0,    0,  361,
  409,    0,  565,    0,    0,    0,    0,    0,  566,  567,
  568,    0,  569,  570,  571,    0,  572,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,    0,    0,    0,    0,  573,    0,  574,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  434,  412,  413,
  432,    0,  433,    0,    0,    0,    0,    0,    0,  575,
  414,    0,    0,    0,  415,  435,    0,    0,    0,    0,
  259,    0,  538,    0,    0,    0,  576,    0,    0,    0,
    0,  144,  145,  146,  147,  148,  149,  150,  151,  152,
  153,  154,  155,  156,  157,  158,  159,    0,  577,  160,
  161,  162,  163,    0,  164,    0,    0,  165,  166,    0,
    0,    0,    0,  102,    0,    0,    0,    0,    0,    0,
    0,  416,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,  578,  579,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,  677,    0,    0,    0,    0,
  580,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0,  406,  426,
  427,    0,  428,  429,  430,    0,    0,  407,  431,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,  361,  409,    0,
    0,    0,    0,    0,    0,    0,  434,  930,    0,  432,
    0,  433,    0,    0,    0,    0,    0,    0,    0,    0,
  789,    0,    0,  463,  435,  410,    0,    0,  411,  259,
    0,    0,    0,  463,  463,  463,  463,  463,  463,  463,
  463,  463,  463,  463,  463,  463,  463,  463,    0,    0,
  463,  463,  463,  463,  406,  463,  412,  413,  463,  463,
    0,  789,    0,  407,  789,    0,    0,    0,  414,    0,
  434,    0,  415,  432,    0,  433,    0,  408,    0,  789,
    0,    0,    0,  361,  409,    0,    0,    0,  435,    0,
    0,    0,    0,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  410,    0,    0,  411,    0,    0,    0,    0,  685,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
  362,    0,  412,  413,    0,  418,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,    0,    0,    0,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  418,    0,  406,    0,    0,    0,    0,    0,    0,
    0,    0,  407,    0,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,  408,    0,    0,  419,
    0,    0,  361,  409,    0,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0,  789,  426,  427,    0,  428,  429,  430,    0,
  410,    0,  431,  411,    0,    0,  434,  406,    0,  432,
    0,  433,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,  259,
  408,  412,  413,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,    0,  789,
    0,    0,  789,    0,    0,    0,    0,    0,    0,  789,
    0,    0,  789,    0,  410,    0,  789,  411,    0,    0,
  434,  789,    0,  432,  789,  433,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  435,    0,
  789,    0,    0,  259,    0,  412,  413,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0,  414,    0,    0,
  417,  415,    0,    0,  789,    0,    0,  789,  789,    0,
  418,    0,    0,    0,    0,    0,    0,  789,    0,    0,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,  789,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,  789,  425,  416,    0,
    0,  789,  426,  427,  417,  428,  429,  430,    0,    0,
    0,  431,    0,  789,  418,  789,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1098,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,  406,    0,    0,  426,  427,    0,  428,
  429,  430,  407,    0,    0,  431,    0,    0,    0,    0,
  434,    0,    0,  432,    0,  433,  408,    0,    0,    0,
    0,    0,  361,  409,    0,    0,    0,    0,  435, 1664,
    0,    0,    0,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,  411,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,  412,  413,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,  414,    0, 1709,    0,  415,  432,    0,
  433,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  435,  410,    0,    0,  411,  259,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1278,  412,  413,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0,  414,    0,    0,
  417,  415,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,  416,    0,
    0,    0,  426,  427,  417,  428,  429,  430,    0,    0,
    0,  431,    0,    0,  418,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0, 1291,  407,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
  408,    0,  419,    0,    0,    0,  361,  409,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,    0,  410,  431,    0,  411,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  406,    0,    0,  412,  413,    0,    0,    0,
  434,  407,    0,  432,    0,  433,    0,  414,    0,    0,
    0,  415,    0,    0,  790,  408,    0,    0,  435,    0,
    0,  361,  409,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
    0,    0,  411,    0,    0,  790,    0,    0,  790,    0,
    0,    0,    0,    0,  951,    0,    0,  951,  416,  951,
    0,    0,    0,  790,  417,    0,    0,    0,  362,    0,
  412,  413,  951,    0,  418,    0,    0,  951,    0,    0,
    0,    0,  414,    0,    0,    0,  415,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,    0,    0,  431,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,  561,  418,
    0,    0,  562,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  363,  364,  365,  102,  184,  185,    0,
    0,  563,    0,    0,  564,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,    0,  425,    0,  814,    0,
    0,  426,  427,    0,  428,  429,  430,  406,    0,  793,
  431,    0,    0,    0,    0,  565,  407,    0,    0,    0,
    0,  566,  567,  568,    0,  569,  570,  571,    0,  572,
  408,    0,  791,    0,    0,    0,  361,  409,    0,  814,
    0,    0,  814,    0,    0,    0,    0,    0,    0,  573,
  793,  574,    0,  793,    0,    0,  790,  814,    0,    0,
    0,    0,    0,    0,  410,    0,    0,  411,  793,    0,
    0,  951,    0,  791,    0,    0,  791,    0,    0,    0,
  951,    0,  575,    0,    0,    0,    0,    0,    0,    0,
    0,  791,    0,    0,  951,  412,  413,    0,    0,  576,
  951,  951,    0,    0,    0,    0,    0,  414,    0,    0,
    0,  415,    0,  790,    0,    0,  790,    0,    0,    0,
    0,  577,    0,  790,    0,    0,  790,    0,  951,    0,
  790,  951,    0,    0,    0,  790,  102,    0,  790,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  792,    0,
    0,    0,    0,    0,  790,    0,  578,  579,    0,  951,
  951,    0,    0,    0,    0,    0,    0,    0,  416,    0,
    0,  951,    0,  580,  417,  951,    0,    0,  790,    0,
    0,  790,  790,    0,  418,    0,    0,    0,    0,  792,
    0,  790,  792,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,  792,  790,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
  790,  425,  951,    0,    0,  790,  426,  427,  951,  428,
  429,  430,    0,    0,    0,  431,    0,  790,  951,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  814,    0,  951,  951,  951,  951,  951,  951,    0,    0,
    0,  793,    0,    0,    0,    0,  951,  822,    0,    0,
    0,    0,    0,    0,    0,  951,  951,  951,  951,  951,
  951,  951,  951,  951,  791,  951,    0,    0,    0,    0,
  951,  951,    0,  951,  951,  951,    0,    0,    0,  951,
    0,  786,    0,    0,    0,    0,    0,  814,  822,    0,
  814,  822,    0,    0,    0,    0,    0,  814,  793,    0,
  814,  793,    0,    0,  814,    0,  822,    0,  793,  814,
  787,  793,  814,    0,    0,  793,    0,    0,    0,    0,
  793,  791,  786,  793,  791,  786,    0,    0,  814,    0,
    0,  791,    0,    0,  791,    0,    0,    0,  791,  793,
  786,    0,    0,  791,    0,    0,  791,    0,    0,    0,
    0,  787,  814,    0,  787,  814,  814,    0,    0,    0,
    0,    0,  791,  793,    0,  814,  793,  793,    0,  787,
    0,    0,    0,    0,    0,    0,  793,    0,    0,    0,
  792,    0,  814,    0,    0,    0,  791,  788,    0,  791,
  791,    0,    0,  793,    0,    0,    0,    0,    0,  791,
    0,    0,    0,    0,  814,    0,    0,    0,    0,  814,
    0,    0,    0,    0,    0,  793,  791,    0,    0,    0,
  793,  814,    0,  814,    0,    0,    0,    0,  788,    0,
    0,  788,  793,    0,  793,    0,    0,  792,  791,    0,
  792,    0,    0,  791,    0,    0,  788,  792,    0,    0,
  792,  821,    0,    0,  792,  791,    0,  791,    0,  792,
    0,    0,  792,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  792,    0,
    0,    0,    0,  815,    0,    0,    0,    0,    0,    0,
    0,    0,  821,    0,    0,  821,    0,    0,    0,    0,
    0,    0,  792,    0,    0,  792,  792,    0,    0,  822,
  821,    0,    0,    0,    0,  792,    0,    0,    0,    0,
    0,    0,    0,    0,  815,    0,    0,  815,    0,    0,
    0,    0,  792,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  815,  786,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  792,    0,    0,    0,    0,  792,
    0,    0,    0,    0,    0,    0,  822,  823,    0,  822,
    0,  792,  787,  792,    0,    0,  822,    0,    0,  822,
    0,    0,    0,  822,    0,    0,    0,    0,  822,    0,
    0,  822,    0,    0,    0,    0,  824,    0,    0,    0,
  786,    0,    0,  786,    0,    0,    0,  822,  823,    0,
  786,  823,    0,  786,    0,    0,    0,  786,    0,    0,
    0,    0,  786,    0,    0,  786,  823,    0,    0,  787,
    0,  822,  787,    0,  822,  822,    0,  824,    0,  787,
  824,  786,  787,    0,  822,    0,  787,    0,    0,  788,
    0,  787,    0,    0,  787,  824,    0,    0,    0,    0,
    0,  822,    0,    0,    0,  786,    0,    0,  786,  786,
  787,    0,    0,    0,    0,    0,    0,    0,  786,    0,
    0,    0,    0,  822,    0,  812,    0,    0,  822,    0,
    0,    0,    0,    0,  787,  786,    0,  787,  787,    0,
  822,    0,  822,    0,    0,    0,  788,  787,    0,  788,
    0,    0,    0,  821,    0,    0,  788,  786,    0,  788,
    0,    0,  786,  788,  787,    0,  812,    0,  788,  812,
    0,  788,    0,    0,  786,  816,  786,    0,    0,    0,
    0,    0,    0,    0,  812,  815,  787,  788,    0,    0,
    0,  787,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  787,    0,  787,    0,    0,    0,    0,
  821,  788,    0,  821,  788,  788,  816,    0,    0,  816,
  821,    0,    0,  821,  788,    0,    0,  821,    0,    0,
    0,    0,  821,    0,  816,  821,    0,    0,    0,    0,
    0,  788,  815,    0,    0,  815,    0,    0,    0,    0,
    0,  821,  815,    0,    0,  815,    0,    0,    0,  815,
    0,    0,  825,  788,  815,    0,    0,  815,  788,  823,
    0,    0,    0,    0,    0,  821,    0,    0,  821,  821,
  788,    0,  788,  815,    0,    0,    0,    0,  821,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  824,    0,
    0,    0,    0,  825,    0,  821,  825,  815,    0,    0,
  815,  815,  813,    0,    0,    0,    0,    0,    0,    0,
  815,  825,    0,    0,    0,    0,  823,  821,    0,  823,
    0,    0,  821,    0,    0,    0,  823,  815,    0,  823,
    0,    0,    0,  823,  821,    0,  821,    0,  823,    0,
    0,  823,    0,  813,    0,  824,  813,    0,  824,  815,
    0,    0,    0,    0,  815,  824,    0,  823,  824,    0,
    0,  813,  824,    0,    0,  817,  815,  824,  815,    0,
  824,    0,    0,    0,    0,    0,    0,  812,    0,    0,
    0,  823,    0,    0,  823,  823,  824,    0,    0,    0,
    0,    0,    0,    0,  823,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  817,    0,    0,  817,
  824,  823,    0,  824,  824,    0,    0,    0,    0,    0,
    0,    0,    0,  824,  817,    0,    0,  816,    0,    0,
    0,    0,    0,  823,  812,    0,    0,  812,  823,    0,
  824,    0,    0,    0,  812,    0,    0,  812,    0,    0,
  823,  812,  823,    0,    0,    0,  812,    0,    0,  812,
  134,    0,  824,    0,  134,    0,    0,  824,    0,    0,
    0,    0,    0,    0,    0,  812,    0,    0,    0,  824,
    0,  824,    0,  134,  816,    0,  134,  816,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  816,    0,  812,
    0,  816,  812,  812,    0,    0,  816,    0,    0,  816,
    0,    0,  812,    0,  825,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  816,    0,  134,    0,  812,
    0,    0,    0,  134,  134,  134,    0,  134,  134,  134,
    0,  134,    0,    0,    0,    0,    0,    0,    0,  816,
    0,  812,  816,  816,    0,    0,  812,    0,    0,    0,
    0,  134,  816,  134,  813,    0,    0,    0,  812,    0,
  812,  825,    0,    0,  825,    0,    0,    0,    0,  816,
    0,  825,    0,    0,  825,    0,    0,    0,  825,    0,
    0,    0,    0,  825,  134,    0,  825,    0,    0,    0,
    0,  816,    0,    0,    0,    0,  816,    0,    0,    0,
    0,  134,  825,    0,    0,    0,    0,    0,  816,    0,
  816,  813,    0,    0,  813,    0,    0,  817,    0,    0,
    0,  813,    0,  134,  813,    0,  825,    0,  813,  825,
  825,    0,    0,  813,    0,    0,  813,    0,    0,  825,
    0,    0,    0,  221,    0,    0,    0,  221,    0,    0,
    0,    0,  813,    0,    0,    0,  825,    0,  134,  134,
    0,    0,  221,    0,    0,    0,  221,    0,    0,  221,
    0,    0,    0,    0,  817,  134,  813,  817,  825,  813,
  813,    0,    0,  825,  817,    0,  831,  817,    0,  813,
  562,  817,    0,    0,    0,  825,  817,  825,    0,  817,
    0,    0,    0,    0,    0,  832,  813,    0,    0,  563,
  221,    0,  564,    0,    0,  817,  221,  221,  221,    0,
  221,  221,  221,    0,  221,    0,    0,    0,  813,    0,
    0,    0,    0,  813,    0,    0,    0,    0,    0,  817,
    0,    0,  817,  817,  221,  813,  221,  813,    0,    0,
    0,    0,  817,  565,    0,    0,    0,    0,    0,  566,
  567,  568,    0,  569,  570,  571,    0,  572,    0,  817,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
    0,    0,   81,    0,    0,    0,    0,  573,  221,  574,
    0,  817,  221,    0,  221,    0,  817,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,  817,    0,
  817,  221,    0,    0,  221,    0,  221,    0,    0,    0,
  575,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  576,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,  221,    0,    0,  221,    0,    0,    0,  577,
    0,  221,  221,  221,    0,  221,  221,  221,  221,  221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,  221,    0,    0,  578,  579,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  580,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  221,
};
static short yycheck[] = {                                       9,
   16,   52,   12,   61,    4,    4,   16,   59,   18,  101,
    0,  236,  120,  121,   60,  331,  487,  335,  310,  682,
  339,  308,  236,  699,   82,  730,  339,  130,  530,  521,
  999,  619,  524,  120,  121,   86,  624,  668,  530,   67,
 1170,   51,   52,   53,   54,   55,  722,   70,   91,  736,
   60,   61,   62,  267,   64,  394,   66,   67,  885,   69,
   70, 1213,   90,  731,   80,  658,  470,  344,   78,   92,
   80,   81,   82,   89,   84,   85,   86,   87,  613,   89,
   90,   91,   92,   65,   94,   53,  992,  356,   60,  677,
  303,  134,  685,  999,    0,  726,   64,   85,  287,    0,
  927,  111,  112,  113,    0,  825,   88,  992,   40,  244,
    0,   40,   45,    0,  999,  125,  208,  169,  253,   87,
  130,   58,  132,  133,  134,  729,   94,  992,   54,    0,
    0,  992,  794,   41,  999,   44,    0,  125,  999,  111,
   66,  113,  194,   44,  698,   41,  371,    0,   44,  236,
   45,   41,  756,   59,   44,   44,  606,  371,   59,    0,
   40,  765,   44,   59,  132,   42,   40,    0,    0,   59,
   41,   59,   59,   44,    0,   40,   44,   41,   58,   42,
   44,    0,  500,  662,  287, 1000,  112,   59,   59,   59,
   41,  507,  508,  509,  510,   59, 1011,   45,   63,  944,
   41,  236,  294,   44,   41,  400,   59,  133,  218,    0,
   44,  530,   61,  490,  818, 1030,  226,  530,   59,   44,
  271, 1048,   40,   44,    0,   40,   59,   59,  238,    0,
   45,  702,  242,   59,  244,  245, 1356,   45,  556,   45,
   59,  251,   45,  253, 1024,   63,  533, 1356,    0,   44,
    0,  386,    0, 1356,   44,  886, 1356,   63,   40,    0,
   91,  271,   40,   63,    0,  275,  238,  264,   59, 1414,
 1239,  288,  860,  861, 1211,  288, 1261,  287,   41,  260,
  332,   44,  334,   59,  371,   44,    0, 1129,   59,  262,
  782,  394,  561,  314,   12,  582,  348,  326,    0,  309,
   41,  352,   12,  260,  357,  264,  316,   59,  318,   59,
  320,   59, 1047,  323,  342,  287,  242,   41,   59,  339,
   44,    0,    0,   59,  446,  251,  329,  251,  140,  456,
    0,  341,  342, 1239,  922,    0,  371,  309,  268,   41,
  350,  260,  352,   40,  316,   59,  318,  260,  320,  275,
 1125,  323,  277,  285, 1239,   41,  356,   59,  265,    0,
  494,  965,   41,   42,   43,   44,   45,    0,   47,   59,
  504,   41,  382,  341, 1239,  385,  386,  430, 1239,   44,
   59,   59, 1534,  393,  394,  304,  508,  397,    0,   59,
  400,  304,  402,  264,   59,  405,  303,  418,  802,  878,
  118,  265,  431,  991,    0, 1317, 1010,    0,  118,  438,
    0,  890,  891, 1228,  387,  412, 1258,  385,   59,  644,
  297,  346,  394,  443,  350,  435,   59, 1172,  329,  319,
  644,  332,  400, 1418, 1346,  341,  366,  418,    0,  303,
  548,  444, 1379,  344,  461,  733,  277,   59,  461,    0,
  321,  341,  301,  505,  668,  319,  382,  744,  461,  469,
  747,  418,  332,   59, 1126, 1245,   59,  393,  319,   59,
  665,  397,   44,  310,  761, 1595,  402,  341,  402,   41,
  388, 1633,   44,    0,   44,  297, 1595,  321,  384,  703,
   41,  269, 1595,  264,   41, 1595, 1641,   59,  319,  551,
 1163,  788,  461, 1190,    0,  264,  258, 1170,   59,  341,
  258,  274,  726,  273,  264,  305,  384,  343,  653,  529,
  530, 1075, 1057,  411,  461,  535,  459,   44,  437,  461,
  743, 1251,  461,    0,  723, 1687, 1666,  829,  329,  816,
  441,  332,   59,  319,  883,   41, 1214,  310,   44,  455,
  602,  603,  453,  486,  455,  307, 1371,  644,  530,  307,
 1375,  561, 1377,   59,  459,  341,  461,  549,  455,  838,
  459,  441, 1234,  455,   41,    0, 1728,  410,    0, 1709,
  406,  461,  453,  409,  461,  455,  265,  461,  441,  461,
  461, 1041,   59,  329,  459,  605,  332, 1412,  461,  425,
  708,  280,  455, 1430,  614,  199,  502,  286, 1196,  644,
 1355,  459,  388,  503,  614,  614,   41,  432,  437,   44,
  723,  397,   44,  455,  303,  451,  432,  398, 1216,  432,
  455,  739,  740,  605,   59,  645,  321,   59,  648,  503,
  319,  459,    0,  653,  459, 1425, 1250,  388,  398,  412,
  441,  459,  662,  459,  368,  665,  459, 1136,  453,  459,
  670,  502,  341,  979,  455,  441,  448,  419,  264,  259,
 1485,  419,  886,  645,  264, 1490, 1411, 1452,    0, 1366,
  270,  691,  670,  735,  736, 1278,  388, 1318,  412,  699,
  368,  701, 1661,    0,  704,  397,  290,  665,  395,    0,
    0,   59,  388,    0,  467,  441,    0,  717,  349,  719,
  997,  397,  722,  723,    0,  262,  349,  453,  728,  455,
  730,  411, 1009,  733, 1539,   59,  319, 1043,   41,  341,
    0,   44,  867,  308,  309,  870,  329,   59,    0,  332,
 1652, 1556, 1029,  753,  662,  305,   59,   41,  341,  321,
   44,  723,   59,  403,  321, 1661,  728,  319,   59,   59,
  742,  733,   59, 1055, 1052,   59,  404,  992,  319, 1357,
  332,  781,  782,   59,  999,  274, 1661,    0,  992,  341,
  883,  753,  278,  421,  280,  999,    0,  787,   41,   59,
  341,  717,  288,  719,  272,  388, 1661,   59,  780,  799,
 1661,  403,  398, 1466,  397,  322,   41,  817,  398,  781,
  782,  289,  329,    0,  257,  332,    0,  827,   41,  336,
  258,   44, 1324, 1528,  503, 1640,  388,  344,  871,  839,
  812,  813,  814, 1237, 1649,  397,   59,  388,  838, 1243,
  387,  869,   41,  360,  446,   59,  397, 1139,  441,  872,
    0,   42,  319,  278,    0,  280,  334,  867,  868,  869,
  870,  871,  872,  288,    0,  332,    0,  384,  878,  307,
  437,   41,   59,  883,  341,   59, 1691,  452,   41,  441,
  890,  891,  302,  379,   41,  363,    0, 1042, 1593,  410,
  441,  817,  335,  455,    0,   41,   41,  414,   44,  406,
  868,  827,  419,    0,  455,  401,  508,  329,  349,   59,
  332,  883,  408,   59,    0, 1129,  356,  358, 1594, 1027,
    0,  388,  344,   59,  441,   59, 1153,   41,    0, 1156,
  397,  273,    0,  527,  944,   41,  453, 1255,  455, 1226,
  302,  302,  264,  284,   41,   59,  442,   44,    0, 1118,
  457, 1120,  430,   59,  379,   41,  301,  264,   44,  384,
  878,   41,   59,  264,  461,  418,  287,  264,  467,   41,
 1257,    0,  890,   59,  441,   61,  401,  339,  264,   59,
  261,  419, 1453,  408,  278,  343,  280,   59,  455,   41,
 1206,   59,  445,    0,  288,  308,  309, 1468,   44,  341,
 1492,  257,  596, 1666, 1592, 1324,  502,   59,  353,  262,
 1020, 1324,   41,   59, 1239,   44,    0,  442,  378,  441,
  378, 1021, 1021,  443, 1034, 1239, 1614,  262, 1183, 1617,
   59,  453,   61,  455,  768, 1045, 1252, 1124, 1125, 1126,
   41,  341, 1052,   44, 1258,   46, 1709, 1363,  406,  319,
  264,  409,   59,  493, 1036,  278,  262,  280,   59,  419,
   61,    0,  332,  262,    0,  288,  302,  425, 1171,  260,
    0,  341, 1054, 1045,  353,   59,  398,  502, 1060,  335,
 1052,  343,  443,  349, 1192,  379, 1194, 1558,  466,   16,
  468,  398,  262,  451, 1381,  403,    0,  398,  411,  440,
  379,  398,   41,  339, 1318,   44,  466,  401, 1118,  435,
 1120, 1338,  398,  304,  408, 1342,  378,  262,  388,  258,
   59,  388,   61,   59, 1124, 1125, 1136,  397,  264,   59,
  264,  439,  319, 1420,  387,  461,   40,   41, 1190,  452,
   44,  462,  463, 1359,  406, 1253,    0,  409,  442, 1699,
 1700, 1320,  387,   80,  341,   59,  379,   61,  406,   40,
   42, 1171, 1172,  425,   45,   47,  329,  459,  307,  332,
 1152,  441,  258,  265,  441,  332,  322,  378,  401,  308,
  309,  387,   63,  329,  398,  408,  332,   91,  387,  451,
  336,  277,  278,  343,  280,  218,  462,  463,  344,  285,
  459,  388,  288,  226, 1792,   59,    0,  443,  502,  457,
  278,  303,  280,  319,  360,  301, 1268,  387,  332,  442,
 1272,  307,  319,  329, 1356,  388,  332, 1698,  378,  258,
  436,  388, 1232, 1232,  397,  341, 1463, 1392,  384,  319,
  397,  403,  387, 1793,  341,  274,  338,  398,  449,  329,
  323,  284,  332, 1356,  441, 1410,  406,  329,  280,  409,
  332,  341,  398,  264,  286,   59, 1553,  319,  414, 1003,
 1004, 1374,  301,  435,  388,  425,  338, 1512,  307,  502,
  419,  310,  388,  397,   41,  260,  437,   44,  450,  341,
  258,  397,  288,  379,  262,  441,  262,  418,  437,  267,
 1653,  451, 1655,   41,  305,  301,   44,  453,  388,  455,
 1320,  379,  319,  375, 1366,  401,  388,  397, 1052,  258,
  471,  323,  408,  452,  445,  397,  388, 1373,  296,  304,
 1546,  417,  471,  419,  341,  441, 1552,    0,  277,  278,
    0,  280,  278, 1696,  280, 1355,  285,  453,  278,  288,
  280,  437,  385,  402,  258,   93,  442,   41,  278,  343,
   44,  441,  301, 1373, 1374, 1452,   42,   43,  307,   45,
  274,   47, 1382,  277,  278,  455,  280,   42,   43, 1487,
   45,  285,   47,  412,  288,  471, 1643,    0,   42,   43,
  419,   45,  384,   47,  378,    0,   59,  301,  280,   59,
  320, 1373, 1374,  307,  286,   59,  310,  440,  437, 1512,
   41,   42,   43,   40,   45,  296,   47,  260, 1743, 1744,
 1773,  387,  406,  459,  457,  409,  344,    0,   42,   43,
    0,   45,  320,   47,    0, 1493, 1661, 1694,  467, 1587,
  379,  425,  471, 1591,  260, 1537,   59, 1661,  317,  318,
  486,  369, 1452,    0,   59, 1367, 1382,    0,  384, 1459,
  428,  304,  401, 1595,  432,  433,  434,  451,   41,  408,
  436,   44,  353,  279,  386,  379,    0, 1802,  494,  495,
  419,  401, 1739, 1493,  259,  279,   59, 1538,  304,   59,
 1622, 1726, 1595,   59,    0,  270,   44,  401,  437,  263,
 1648, 1413, 1512,  442,  408,   44, 1496,  301,  412,  427,
 1562,  406,   59,  401, 1566,  419,   59,   41, 1528, 1622,
   44,  329,  442, 1533,  330, 1760, 1761,    0, 1538,  444,
  425,    0,  471,  437,  384,   59,  330, 1669,  442,    0,
 1512, 1557, 1634,  461,    0,   93,  461, 1557, 1118,  313,
 1120, 1786,    0,   59,  442,  349,   42,   43,  364,   45,
  324,   47, 1797,  467,  358, 1800, 1669,  471,   42,   43,
  364,   45,   41,   47,  259,   44,  382,  458,  459,  460,
 1712,  462,  463, 1593, 1594,  270,   59,    0,  382, 1721,
   59,   44,   40,   41, 1742,  403,   44, 1607,   59,   45,
  481,  482,  384,   59,  280,  486,  487, 1533,  437, 1712,
  286,   59, 1622,   61,  322,  280,  279,   63, 1721,  495,
 1678,  286, 1754, 1726,  338, 1757,  280, 1643,  336,  419,
    0,  439,  286, 1643,    0,  344,  444,  274,  301, 1607,
   93, 1378,  450,   91, 1660,  319,   59,  312, 1780,  280,
 1660, 1754,  360,  461, 1757,  286,  624, 1760, 1761, 1669,
  369,  469,    0, 1400,  428,  341,  280,  330, 1678, 1679,
   42,   43,  286,   45,  322,   47,  384, 1780, 1694,    0,
 1680, 1680,  322, 1786, 1694, 1701,  349,  341,  336,   59,
    0, 1701,   44,   59, 1797,  358,  336, 1800,   45, 1750,
  668,  364, 1712,  671,  344,   40,  414,  321,  676,  425,
  379, 1721,  360,  681,  682,  840, 1726,  842,  427,  382,
  360,   59,  690, 1739,  692,  693,  694,  695,  696, 1739,
  698, 1747,  401,    0, 1645,  703,  384, 1747,   59,  408,
 1750,   93,    0,    0, 1754,    0,  319, 1757,   44,   59,
 1760, 1761,   42,   43, 1726,   45,  329,   47,  726,  332,
   42,   43,   44,   45,  732,   47,  414, 1777,  341,  462,
 1780,  344,    0,  442,  414, 1785, 1786,   42,   43,   44,
   45, 1692,   47,  264,  322,    0,  302, 1797, 1760, 1761,
 1800,  317,   59, 1803,  280,  319,   41,   93,  336,   44,
  286,   59,   59,  338,   59,  329,  280,    0,  332,  278,
  258,  280,  286,   41, 1786,  388,   44,  341,   41,  288,
  344,   44,  360,  339,  397, 1797,  274, 1738, 1800,  277,
  278,   59,  280,   61,  343,  321,    0,  285,  312,    0,
  288,   41,   42,   43,   59,   45,    0,   47,   41,  319,
  296,   44,    0,  301,   41,   42,   43,  337,   45,  307,
   47, 1772,  310,   91,  388,    0,   59,  453,  441,  378,
   93,   41,  403,  397,   44, 1622,  414,  845,  846,  847,
  453,    0,  850,  851,  852,   40,  854,  855,  856,  857,
  858,  859,  408,   41,   44,   59,   44,  406,   59,    0,
  409,  341,  418,    0,    0,   59,  357,  353,  280,   59,
  379,   59,  258,  321,  286,  261,  425,  441,  886,    0,
  355,  319, 1669,  507,   59,  893,  894,  443,  896,  453,
  898,  379,  401,  901,  902,  903,  904,  905,  906,  408,
   59,    0,  451,  394,   41,  396,  403,   44,  258,  296,
  918,  384,  920,  401,   42,   43,  384,   45,   59,   47,
  408,  307,   59,   59,  412, 1712,  437,   41,  419,  279,
   44,  419,  262,  442, 1721,  362,   41,  324,   59,   44,
  435,   40,   41,   42,   43,   44,   45,   46,   47,  437,
  280,  301,   41,  444,  442,   44,  286,  307,  280,  451,
   59,   60,   61,   62,  286,  338,  353, 1754,   41,  418,
 1757,   44,  458,  459,  460,  280,  462,  463,    0,  467,
  330,  286,   41,  471,  459,   44,  461,  443,   41,  260,
  258,   44,   91, 1780,   93,  481,  482,  472,  358,  349,
  486,  487,  375,    0,    0,  288,  274,  412,  358,  277,
  278,  470,  280,   44,  364,  388,  491,  285,   41,   41,
  288,   44,   44,   61,  499,  500,  501,   41,  462,  463,
   44,  302,  382,  301,  373,  260,  287,   59,   40,  307,
  280,  428,  310,  314,   41,   41,  286,   44,   44,   41,
  392,  393,   44,  280,   41,   42,   43,  444,   45,  286,
   47,  264,   59,   59,  458,  459,  460, 1075,  339,  419,
  258,  458,  459,  460,   40,  462,  463,  302,   41, 1087,
 1088,   44, 1090, 1091,   41,   41,  462,   44,   44,  314,
 1098,  279,  486,  487,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  328,   41,    0,  462,   44,  462,  496,
  497,  379,   44,  301,  339,  462,  463,  437,  505,  307,
   44, 1129,   41,   42,   43,  321,   45,  403,   47,   91,
   41,   42,   43,  401,   45, 1143,   47,  408,   40,   40,
  408,    0,  330, 1151,  412, 1153,   41,  418, 1156,   44,
 1158,  419,  280,   40, 1162, 1163, 1164, 1165,  286,   40,
   40,  349, 1170,  262,   59,  264,  265,   40,   40,  437,
  358,   40,  443,  272,  442,   40,  364,   40,   40,   40,
   40,  280,   41,  408,  295,   44,  321,  286,   46,   44,
  289,   44,  429,  418,  382,  306,   46,   61,   40,  467,
   59, 1209,  313,  471,  303,   44,  264,  383,   44,  308,
  309,  349,  278,  312,  278,  301,  264,  398,  443,  437,
  319,  471,  321,  322,  398,  459,  325,  338,  277,  448,
  329,  419,  448,  332,  400,  334,  461,  336,   40,   40,
  350,  319,  341,  342,   91,  344,  322,   41,   42,   43,
 1258,   45,   40,   47,   40,  264,  278,  384,  280,    0,
  336,  360,  361,  285,  363,  412,  288, 1275,  344,   41,
   42,   43,  390,   45,   44,   47, 1284,  383,  377,  301,
  379,  380,  381, 1291,  360,  384,   40,   40,  387,  388,
  318,  437,  403,  280,  465,   40,  321,  321,  397,  286,
   41,   42,   43,   44,   45,   41,   47,   41,   42,   43,
 1318,   45,  411,   47,  453,  414,  449,  264,   59,   60,
   61,   62,  319,  319,  435,  322,  322,  452,  439,  455,
 1338,  430,  329,  329, 1342,  332,  332,  436,  414,  336,
  336,   40,  441,  459,  341,  341,  277,  344,  344,  274,
   40,  301,   93,  452,  453,  285,  455,  379,  278,  345,
  345,  280,  461,  360,  360,  419,  387,  286,  387,  280,
  387,  417,  301,    0,  260,  286,   41,   42,   43,  401,
   45,    0,   47,  278,  419,  280,  408,  384,  384,   40,
  285,  388,  388,  288,   40,  274,  357,   40,  418,   44,
  397,  397,  418,  262,  503,  386,  301,    0,  368,   41,
   42,   43,  321,   45,   44,   47,  302,  414,  414,  264,
  442,   41,   41,   44,  321,   44,   44,   40,  314,  280,
 1438, 1439,   59,   44,  503,  329,   44,   41,   42,   43,
   59,   45,  266,   47,  441,  441,  506,  502,   41,   42,
   43,   44,   45,  339,   47, 1463,  453,  453, 1466,   40,
  319,   40,  300,  322,  461,  423,   59,   60,   61,   62,
  329,  264,   41,  332,   58, 1669,    0,  336,   41,   41,
   41,  264,  341,  455,  379,  344,  280,  441,  667,  395,
  669, 1499,  286,  672,  673,  674,  310,  310,  493,   41,
   93,  360,   42,   43,   40,   45,  401,   47,  280,   41,
   40,   40,  353,  408,  286,   40,  262,   41, 1712,   59,
   44,  262,  408,  264,  265,  384,   40,   40,  387,  388,
   40,  272,  418,   40,   40,   59,   40,   61,  397,  280,
  436,   40,   40,   40,   40,  286,  280,  442,  289,   40,
   40,  328,  286, 1561,  309,  414,   44,  443,   41,  389,
 1754, 1569,  303, 1757,    0,  344,  389,  308,  309,  389,
  274,  312,  332,  391,  423,  461,  392,  436,  319,  447,
  321,  322,  441,  271,  325,  288, 1780,  408,  329,  387,
  387,  332,  387,  334,  453,  336,  455,  387, 1606,   41,
  341,  342, 1610,  344,  379,   41,   42,   43,   44,   45,
   41,   47,   41,   42,   43,  280,   45,  278,   47,  360,
  361,  286,  363,   59,   60,   61,   62,   40,    0,  387,
  306,  258,   41,   41,  418,  387,  377,  313,  379,  380,
  381,  317,  318,  384,   41,  437,  387,  388,  280,  386,
  326,   41,  279,  309,  286,  436,  397,   93, 1666,  278,
 1668,  280,  338, 1671,  397,  280,  338,  443,  339,  262,
  411,  264,  265,  414,  301,  443,  280,  302,  302,  272,
  307,  339,  286,   41,   41,  353,  321,   59,  386,  430,
  372,  372,   41,  286,   41,  436,  289,   41,   41,   41,
  441, 1709,   41,  330,   41,   41,   41,   41,   41,   41,
  303,  452,  453,   41,  455,  308,  309,   41,   41,  312,
  461,   41,  349,  280,   46,   44,  319,  403,  321,  322,
  357,  358,  325,   59,  258,   40,  329,  364,  341,  332,
  280,  334,   59,  336,  420,   40,  286,   59,  341,  342,
   59,  344,   59,   59,  278,  382,  280,   59,  341,  435,
  379,  285,  503,  439,  288,  309,    0,  360,  361,   59,
  363,   41,    0,  353,  450,  345,   41,  301,  369,  279,
   40,  341,  401,  307,  377,  452,  379,  380,  381,  408,
  384,  384,  419,  469,  387,  388,   41,   42,   43,  384,
   45,  301,   47,  430,  397,   44,   59,   41,   59,  295,
   44,  384,   41,   41,   42,   43,   44,   45,  411,   47,
  306,  414,  344,  442,   41,   59,  262,  313,  264,  265,
  330,   59,   60,   61,   62,   41,  272,  430,  436,   44,
  326,   44,  305,  436,  411,  302,  443,  388,  441,  349,
    0,  280,   40,  289,  294,  379,  305,  286,  358,  452,
  453,   59,  455,  464,  364,   93,   44,  303,  461,  308,
   44,   41,  308,  309,    0,  384,  312,  401,  338,  384,
   40,  264,  382,  319,  408,  321,  322,  461,  264,  325,
  278,   41,  412,  329,   44,  419,  332,  390,  334,  321,
  336,  387,  264,  459,  452,  341,  342,  279,  344,   59,
  503,   40,  471,  437,  310,  271,  459,  403,  442,  264,
  437,    0,  461,  344,  360,  361,  344,  363,    0,  301,
  344,   41,   42,   43,  420,   45,  459,   47,  344,  309,
  271,  377,  436,  379,  380,  381,   41,  471,  384,  435,
   40,  387,  388,  439,    0,  309,  442,   40,  330,   59,
   59,  397,   41,  309,  450,   44,   41,   59,   59,   41,
   41,   43,   44,   45,   44,  411,  305,  349,  414,   59,
   59,   41,    0,  469,    0,  357,  358,   59,   60,   61,
   62,    0,  364,   12,  430,   41,  253, 1121,   44,  246,
  436,   42,   43,  245,   45,  441,   47, 1057,  830,  653,
  382, 1055, 1052,   59,  609,   84,  452,  453, 1044,  455,
 1401,   93,  394,  306,  396,  461,   41,   42,   43,  556,
   45, 1034,   47,    0,  262,  280,  264,  265,  336,  301,
  982,  286,  292,  728,  272, 1045,  318,  419,   41,   42,
   43,  850,   45,  852,   47,  854,  855,  318,  857,  858,
  859,  289, 1604,  763, 1379, 1685,  535,  503, 1381, 1730,
 1689, 1688,  444,  753,   41,  303, 1207,   44, 1612,    0,
  308,  309, 1627, 1785,  312,  319, 1727,  371,  322,  967,
 1185,  319,   59,  321,  322,  329, 1360,  325,  332, 1643,
  323,  329,  336,  311,  332,  537,  334,  341,  336,  304,
  344, 1241,  809,  341,  342,  595,  344,  801,  405,   13,
   41,  279,   43,   44,   45,  476,  360,  717,  278,  123,
  280,  472,  360,  361,  944,  363,  732,  703,   59,   60,
   61,   62, 1483, 1595, 1747, 1803,  704, 1701,  887,  377,
  384,  379,  380,  381,  388, 1089,  384,  487,  661,  387,
  388,  370,   -1,  397,    0, 1158,   -1,   -1,   -1,  397,
  280,   -1,   93,   -1,   -1,   -1,  286,   -1,   -1,   -1,
  414,   -1,   -1,  411,   -1,   -1,  414,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  278,
  272,  280,  430,   -1,   -1,   -1,   -1,  441,  436,  288,
   -1,   -1,   -1,  441,   -1,   -1,   -1,  289,   -1,  453,
   -1,  262,  301,   59,  452,  453,   -1,  455,   -1,  379,
   -1,  303,  278,  461,  280,    0,  308,  309,   -1,  280,
  312,   -1,  288,   -1,    0,  286,   -1,  319,   -1,  321,
  322,  401,   -1,  325,   -1,  301,   -1,  329,  408,   -1,
  332,   -1,  334,   -1,  336,  280,   -1,   -1,   -1,  341,
  342,  286,  344,   -1,   -1,  503,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,  280,  360,  361,
   -1,  363,  442,  286,   59,   60,   61,   62,   -1,   -1,
  379,   -1,   -1,   59,   -1,  377,   -1,  379,  380,  381,
   -1,  278,  384,  280,   -1,  387,  388,   -1,   -1,   -1,
   -1,  288,  401,   -1,   -1,  397,   -1,   -1,   93,  408,
   -1,   -1,   -1,  379,   -1,   -1,   -1,   -1,   -1,  411,
   -1,  262,  414,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,   -1,  401,   -1,   -1,  430,   -1,
   -1,   -1,  408,  442,  436,   -1,   -1,   -1,  289,  441,
  473,  474,  475,  476,  477,  478,  479,  480,   -1,   -1,
  452,  453,  303,  455,   -1,   -1,   -1,  308,  309,  461,
   -1,  312,    0,   -1,   -1,   -1,  442,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,
   -1,  332,  379,  334,   -1,  336,   -1,   -1,   -1,   -1,
  341,  342,  258,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  503,   -1,   41,  401,   43,   44,   45,   -1,  360,
  361,  408,  363,  279,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,  377,   -1,  379,  380,
  381,   -1,   -1,  384,   -1,  301,  387,  388,   -1,   -1,
   -1,  307,   -1,   -1,   -1,  442,  397,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,
  411,   -1,   -1,  414,  330,   -1,   -1,  262,   -1,  264,
  265,   -1,  258,   -1,   -1,   -1,   -1,  272,   -1,  430,
   -1,   -1,   -1,  349,   -1,  436,   -1,   -1,   -1,   -1,
  441,   -1,  358,  279,  289,   -1,   -1,   -1,  364,   -1,
   -1,  452,  453,   -1,  455,   -1,   -1,   -1,  303,   -1,
  461,   -1,   -1,  308,  309,  301,  382,  312,   -1,   -1,
   -1,  307,   -1,   -1,  319,   -1,  321,  322,   -1,   -1,
  325,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,  334,
   -1,  336,   -1,   -1,  330,   -1,  341,  342,   -1,  344,
   -1,   40,  503,  419,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  349,   -1,  360,  361,   -1,  363,   58,
   -1,   -1,  358,   -1,   63,   -1,   -1,   -1,  364,   -1,
   -1,   -1,  377,   -1,  379,  380,  381,   -1,   -1,  384,
   -1,   -1,  387,  388,   -1,   -1,  382,   -1,   -1,   -1,
   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,  261,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,   -1,  414,
   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  283,   -1,   -1,  419,  272,  430,   -1,   -1,  291,   -1,
   -1,  436,   -1,   -1,   -1,  298,  441,  300,   -1,  302,
   40,  289,   -1,   43,  307,   45,   -1,  452,  453,   -1,
  455,  314,   -1,   -1,   -1,  303,  461,   -1,   58,   -1,
  308,  309,   -1,   63,  312,  328,   -1,   -1,   -1,   -1,
   -1,  319,   -1,  321,  322,   -1,  339,  325,   -1,   -1,
   -1,  329,   -1,   -1,  332,   -1,  334,   -1,  336,   -1,
   -1,   -1,   -1,  341,  342,   -1,  344,   -1,  503, 1276,
 1277,   -1,   -1,  271,   -1, 1282, 1283,   -1, 1285, 1286,
   -1,   -1,  360,  361,   -1,  363, 1293, 1294, 1295, 1296,
   -1, 1298, 1299, 1300, 1301, 1302, 1303,   -1,   -1,  377,
   -1,  379,  380,  381,  302,   -1,  384,   -1,   -1,  387,
  388,  309,   -1,  406,   -1,  313,  314,   -1,  316,  397,
  413,  319,   -1,  416,   -1,  418,  419,   -1,   -1,   -1,
   -1,   -1,   -1,  411,   -1,  333,  414,   -1,  267,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
  443,   -1,  430,   -1,   -1,   -1,   -1,   -1,  436,   -1,
   -1,  290,   -1,  441,  293,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,   -1,  452,  453,   -1,  455,   -1,   -1,
   -1,   -1,   -1,  461,   -1,   -1,  315,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  324,   -1,   -1,  327,  492,
   -1,  399,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,  507,   -1,   -1,   -1,   -1,   -1,
  418,   -1,   -1,   -1,   -1,  503,  355,  356,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  433,   -1,  267,  367,   -1,
   -1,   -1,  371,   -1,   -1,  443,  276,   -1,   -1,   -1,
  379,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
  290,   -1,   -1,  461,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  315,   -1,   -1,   -1,   -1,
   -1,   -1,  490,  422,  324,   -1,   -1,  327,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,  509,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,  367,   -1,   -1,
   -1,  371,   -1,  472,   -1,   -1,   -1,   -1,   -1,  379,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,
   -1,  510,  511,  512,   -1,   -1,   -1,   -1,  418,   -1,
   -1,   -1,  422,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  276,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,  290,
   -1,   -1,  472,   -1,   -1,  296,  297,   -1,    0,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,  315,   41,  496,  497,   44,  499,
  500,  501,   -1,  324,   -1,  505,  327,   -1,   -1,   -1,
  510,  511,  512,   59,   -1,   61,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,  355,  356,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   91,  367,   -1,   -1,   -1,
  371,  273,   -1,   -1,   -1,  277,   -1,   -1,  379,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,
   41,   -1,   -1,   44,  296,   -1,   -1,  299,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  422,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,  340,   -1,
   91,   -1,   -1,  444,  346,  347,  348,   -1,  350,  351,
  352,   -1,  354,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  374,   -1,  376,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,  271,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,  407,   -1,   -1,   -1,  510,
  511,  512,   -1,   -1,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,  424,   -1,  302,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  258,   -1,   58,  313,  314,   -1,  316,   63,
   -1,  319,   -1,   -1,  446,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  278,   -1,  280,  333,  258,   -1,   -1,  285,
   -1,  339,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  274,   -1,   -1,  301,  278,   -1,  280,  481,
  482,  307,   -1,  285,  310,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   42,   43,  498,   45,   -1,  301,
   -1,   -1,   -1,   -1,   -1,  307,   -1,  258,  310,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,  399,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
  418,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  301,   -1,   -1,  379,   -1,  433,  307,   -1,   -1,  310,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,  401,  454,  379,   -1,   -1,
   -1,   -1,  408,  461,   -1,   -1,  412,   -1,   -1,   -1,
   -1,   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,  401,
   -1,   -1,   -1,   -1,   -1,   -1,  408,   -1,   -1,   -1,
  412,  437,  490,   -1,   -1,   -1,  442,  419,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  379,   -1,
   -1,  509,   -1,   -1,   -1,  437,  260,   -1,   -1,   -1,
  442,  467,   -1,  267,   -1,  471,   -1,   -1,   -1,   -1,
  401,   -1,  276,   -1,   -1,   -1,   -1,  408,   -1,  271,
   -1,  412,   -1,   -1,   -1,  467,  290,   -1,  419,  471,
   -1,   40,  296,  297,   43,   -1,   45,   41,   42,   43,
  304,   45,   -1,   47,   -1,   -1,  437,   -1,   -1,   58,
  302,  442,   -1,   -1,   63,   -1,   60,   61,   62,   -1,
  324,  313,  314,  327,  316,   -1,   -1,  319,   -1,   -1,
   -1,   -1,  260,   -1,   -1,   -1,  467,   -1,   -1,  267,
  471,  333,   -1,   -1,   -1,   -1,   -1,  339,  276,  353,
   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,  367,   -1,   -1,   -1,  371,  296,  297,
   -1,   -1,   -1,   -1,   40,   -1,  304,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  324,   63,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,  353,  418,  355,  356,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,
  444,  433,   -1,  371,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,  454,   -1,   -1,   -1,   -1,   -1,  472,  461,
   -1,   40,   -1,   -1,   43,   -1,   45,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   58,
   -1,   -1,  496,  497,   63,  499,  500,  501,  490,   -1,
  428,  505,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,  444,  509,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,  272,   -1,
  458,  459,  460,  461,  462,  463,  280,   -1,   -1,   -1,
   -1,  290,  286,   -1,  472,  289,   -1,  296,  297,   -1,
   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,
   -1,  499,  500,  501,   -1,  324,   -1,  505,  327,   -1,
   -1,  325,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  334,   -1,   -1,   -1,  260,   -1,   -1,  263,  342,   -1,
   -1,  267,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,  361,  367,  363,
   -1,   -1,  371,   -1,  290,   -1,   -1,   -1,   -1,   -1,
  296,  297,   -1,  377,   -1,  379,  380,  381,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,  426,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,  430,   -1,   -1,  355,
  356,  260,   -1,   -1,  263,  444,   -1,   -1,  267,   -1,
   -1,  367,   -1,   -1,   -1,  371,   -1,  276,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,  472,   -1,   -1,   40,  296,  297,   43,
   -1,   45,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   58,   -1,   -1,  496,  497,   63,
  499,  500,  501,   -1,   -1,  324,  505,   -1,  327,   -1,
  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,  367,   -1,
   -1,   -1,  371,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
  260,   -1,   -1,  263,   -1,  444,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,  472,   -1,   -1,  296,  297,   -1,   -1,
   -1,    0,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,  324,   -1,  505,  327,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,   41,  267,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,  355,  356,   -1,   -1,   40,
   59,   -1,   43,   -1,   45,   -1,  290,  367,   -1,   -1,
   -1,  371,  296,  297,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,
   -1,  355,  356,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,  444,   -1,   -1,  371,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   42,   43,   -1,   45,   -1,   47,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   60,   61,   62,   -1,  496,  497,   -1,  499,
  500,  501,  426,   -1,  428,  505,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,  260,
   -1,  505,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,  276,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,
   -1,   -1,   58,   -1,   -1,  296,  297,   63,   -1,   -1,
  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,   -1,   -1,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,  324,   -1,  344,  327,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  355,  356,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,  384,  367,   -1,  387,  388,
  371,   -1,   -1,   -1,   58,   -1,   -1,   -1,  397,   63,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  280,   -1,   -1,  414,   -1,   -1,  286,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,   -1,   -1,
   -1,   -1,  441,   -1,   -1,  426,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,  453,   -1,  455,  325,   -1,   -1,
   -1,   -1,   -1,  444,   -1,   -1,  334,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  342,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,  361,   -1,  363,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  377,
  491,  379,  380,  381,  260,  496,  497,  263,  499,  500,
  501,  267,   -1,   -1,  505,   -1,   -1,   -1,   -1,   40,
  276,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,   -1,   58,   -1,   -1,
  296,  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  430,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,  355,
  356,   -1,  276,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  367,   -1,   -1,   -1,  371,  290,   -1,   -1,  461,
   -1,   58,  296,  297,   -1,   -1,   63,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,  485,   -1,   -1,  488,  489,  490,  491,
  324,  493,   -1,  327,  496,  497,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,  444,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,  267,   -1,   -1,  505,
   -1,   -1,  426,   -1,  428,  276,   40,   -1,   42,   43,
  434,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,
  444,   -1,   -1,   -1,   58,  296,  297,   -1,   -1,   63,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,  324,   -1,   -1,  327,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
  267,  505,  353,   -1,  355,  356,   -1,   -1,   40,  276,
   -1,   43,   -1,   45,   -1,   -1,  367,   -1,   -1,   -1,
  371,   -1,   -1,  290,   -1,   -1,   58,   -1,   -1,  296,
  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,  353,   -1,  355,  356,
   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,  418,   -1,  267,  505,   -1,   -1,   -1,   -1,   -1,
   -1,  428,  276,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,  444,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  260,  496,
  497,   -1,  499,  500,  501,  267,   -1,   -1,  505,  353,
   -1,  355,  356,   40,  276,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  290,   -1,
   -1,   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,
   -1,   -1,  304,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,
  472,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  296,  491,
   -1,  299,   -1,   -1,  496,  497,   -1,  499,  500,  501,
  267,   40,   -1,  505,   43,   -1,   45,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,  290,   63,   -1,   -1,   -1,   -1,  296,
  297,   -1,  340,   -1,   -1,   -1,   -1,   -1,  346,  347,
  348,   -1,  350,  351,  352,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,  374,   -1,  376,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,  355,  356,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  407,
  367,   -1,   -1,   -1,  371,   58,   -1,   -1,   -1,   -1,
   63,   -1,  461,   -1,   -1,   -1,  424,   -1,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,  485,   -1,  446,  488,
  489,  490,  491,   -1,  493,   -1,   -1,  496,  497,   -1,
   -1,   -1,   -1,  461,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,  481,  482,   -1,   -1,  444,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,   -1,
  498,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  267,  496,
  497,   -1,  499,  500,  501,   -1,   -1,  276,  505,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,  461,   58,  324,   -1,   -1,  327,   63,
   -1,   -1,   -1,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,  483,  484,  485,   -1,   -1,
  488,  489,  490,  491,  267,  493,  355,  356,  496,  497,
   -1,   41,   -1,  276,   44,   -1,   -1,   -1,  367,   -1,
   40,   -1,  371,   43,   -1,   45,   -1,  290,   -1,   59,
   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,  418,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
  353,   -1,  355,  356,   -1,  444,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,  472,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,  262,  496,  497,   -1,  499,  500,  501,   -1,
  324,   -1,  505,  327,   -1,   -1,   40,  267,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
  290,  355,  356,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,  319,
   -1,   -1,  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,  324,   -1,  336,  327,   -1,   -1,
   40,  341,   -1,   43,  344,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
  360,   -1,   -1,   63,   -1,  355,  356,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,  367,   -1,   -1,
  434,  371,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,
  444,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,  414,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  436,  491,  428,   -1,
   -1,  441,  496,  497,  434,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,  453,  444,  455,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  455,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,  267,   -1,   -1,  496,  497,   -1,  499,
  500,  501,  276,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  290,   -1,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   58,   59,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,  355,  356,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,  367,   -1,   40,   -1,  371,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  324,   -1,   -1,  327,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  418,  355,  356,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,  367,   -1,   -1,
  434,  371,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,  428,   -1,
   -1,   -1,  496,  497,  434,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,  444,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  455,  276,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,  472,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,  324,  505,   -1,  327,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,  355,  356,   -1,   -1,   -1,
   40,  276,   -1,   43,   -1,   45,   -1,  367,   -1,   -1,
   -1,  371,   -1,   -1,    0,  290,   -1,   -1,   58,   -1,
   -1,  296,  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  428,   45,
   -1,   -1,   -1,   59,  434,   -1,   -1,   -1,  353,   -1,
  355,  356,   58,   -1,  444,   -1,   -1,   63,   -1,   -1,
   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,  444,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,  296,   -1,   -1,  299,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,   -1,    0,   -1,
   -1,  496,  497,   -1,  499,  500,  501,  267,   -1,    0,
  505,   -1,   -1,   -1,   -1,  340,  276,   -1,   -1,   -1,
   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,  354,
  290,   -1,    0,   -1,   -1,   -1,  296,  297,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,  374,
   41,  376,   -1,   44,   -1,   -1,  262,   59,   -1,   -1,
   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,   59,   -1,
   -1,  267,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  276,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,  290,  355,  356,   -1,   -1,  424,
  296,  297,   -1,   -1,   -1,   -1,   -1,  367,   -1,   -1,
   -1,  371,   -1,  319,   -1,   -1,  322,   -1,   -1,   -1,
   -1,  446,   -1,  329,   -1,   -1,  332,   -1,  324,   -1,
  336,  327,   -1,   -1,   -1,  341,  461,   -1,  344,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,  360,   -1,  481,  482,   -1,  355,
  356,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,  367,   -1,  498,  434,  371,   -1,   -1,  384,   -1,
   -1,  387,  388,   -1,  444,   -1,   -1,   -1,   -1,   41,
   -1,  397,   44,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   59,  414,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  436,  491,  428,   -1,   -1,  441,  496,  497,  434,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,  453,  444,  455,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  472,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  262,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,    0,   -1,   -1,   -1,   -1,   -1,  319,   41,   -1,
  322,   44,   -1,   -1,   -1,   -1,   -1,  329,  319,   -1,
  332,  322,   -1,   -1,  336,   -1,   59,   -1,  329,  341,
    0,  332,  344,   -1,   -1,  336,   -1,   -1,   -1,   -1,
  341,  319,   41,  344,  322,   44,   -1,   -1,  360,   -1,
   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,  360,
   59,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,
   -1,   41,  384,   -1,   44,  387,  388,   -1,   -1,   -1,
   -1,   -1,  360,  384,   -1,  397,  387,  388,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,
  262,   -1,  414,   -1,   -1,   -1,  384,    0,   -1,  387,
  388,   -1,   -1,  414,   -1,   -1,   -1,   -1,   -1,  397,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,  436,  414,   -1,   -1,   -1,
  441,  453,   -1,  455,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,  453,   -1,  455,   -1,   -1,  319,  436,   -1,
  322,   -1,   -1,  441,   -1,   -1,   59,  329,   -1,   -1,
  332,    0,   -1,   -1,  336,  453,   -1,  455,   -1,  341,
   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,  262,
   59,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,  319,    0,   -1,  322,
   -1,  453,  262,  455,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  319,   -1,   -1,  322,   -1,   -1,   -1,  360,   41,   -1,
  329,   44,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,   -1,   -1,  344,   59,   -1,   -1,  319,
   -1,  384,  322,   -1,  387,  388,   -1,   41,   -1,  329,
   44,  360,  332,   -1,  397,   -1,  336,   -1,   -1,  262,
   -1,  341,   -1,   -1,  344,   59,   -1,   -1,   -1,   -1,
   -1,  414,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,  436,   -1,    0,   -1,   -1,  441,   -1,
   -1,   -1,   -1,   -1,  384,  414,   -1,  387,  388,   -1,
  453,   -1,  455,   -1,   -1,   -1,  319,  397,   -1,  322,
   -1,   -1,   -1,  262,   -1,   -1,  329,  436,   -1,  332,
   -1,   -1,  441,  336,  414,   -1,   41,   -1,  341,   44,
   -1,  344,   -1,   -1,  453,    0,  455,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   59,  262,  436,  360,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,   -1,   -1,
  319,  384,   -1,  322,  387,  388,   41,   -1,   -1,   44,
  329,   -1,   -1,  332,  397,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,   -1,   59,  344,   -1,   -1,   -1,   -1,
   -1,  414,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
   -1,  360,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,
   -1,   -1,    0,  436,  341,   -1,   -1,  344,  441,  262,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
  453,   -1,  455,  360,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,   -1,   41,   -1,  414,   44,  384,   -1,   -1,
  387,  388,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  397,   59,   -1,   -1,   -1,   -1,  319,  436,   -1,  322,
   -1,   -1,  441,   -1,   -1,   -1,  329,  414,   -1,  332,
   -1,   -1,   -1,  336,  453,   -1,  455,   -1,  341,   -1,
   -1,  344,   -1,   41,   -1,  319,   44,   -1,  322,  436,
   -1,   -1,   -1,   -1,  441,  329,   -1,  360,  332,   -1,
   -1,   59,  336,   -1,   -1,    0,  453,  341,  455,   -1,
  344,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,  360,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
  384,  414,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  397,   59,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,  436,  319,   -1,   -1,  322,  441,   -1,
  414,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,
  453,  336,  455,   -1,   -1,   -1,  341,   -1,   -1,  344,
  273,   -1,  436,   -1,  277,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  453,
   -1,  455,   -1,  296,  319,   -1,  299,  322,   -1,   -1,
   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,  384,
   -1,  336,  387,  388,   -1,   -1,  341,   -1,   -1,  344,
   -1,   -1,  397,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,   -1,  340,   -1,  414,
   -1,   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,
   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,
   -1,  436,  387,  388,   -1,   -1,  441,   -1,   -1,   -1,
   -1,  374,  397,  376,  262,   -1,   -1,   -1,  453,   -1,
  455,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,  414,
   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,
   -1,   -1,   -1,  341,  407,   -1,  344,   -1,   -1,   -1,
   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,
   -1,  424,  360,   -1,   -1,   -1,   -1,   -1,  453,   -1,
  455,  319,   -1,   -1,  322,   -1,   -1,  262,   -1,   -1,
   -1,  329,   -1,  446,  332,   -1,  384,   -1,  336,  387,
  388,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,  397,
   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,  360,   -1,   -1,   -1,  414,   -1,  481,  482,
   -1,   -1,  292,   -1,   -1,   -1,  296,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  319,  498,  384,  322,  436,  387,
  388,   -1,   -1,  441,  329,   -1,  273,  332,   -1,  397,
  277,  336,   -1,   -1,   -1,  453,  341,  455,   -1,  344,
   -1,   -1,   -1,   -1,   -1,  292,  414,   -1,   -1,  296,
  340,   -1,  299,   -1,   -1,  360,  346,  347,  348,   -1,
  350,  351,  352,   -1,  354,   -1,   -1,   -1,  436,   -1,
   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  384,
   -1,   -1,  387,  388,  374,  453,  376,  455,   -1,   -1,
   -1,   -1,  397,  340,   -1,   -1,   -1,   -1,   -1,  346,
  347,  348,   -1,  350,  351,  352,   -1,  354,   -1,  414,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,
   -1,   -1,  412,   -1,   -1,   -1,   -1,  374,  273,  376,
   -1,  436,  277,   -1,  424,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,  453,   -1,
  455,  296,   -1,   -1,  299,   -1,  446,   -1,   -1,   -1,
  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,   -1,   -1,  340,   -1,   -1,   -1,  446,
   -1,  346,  347,  348,   -1,  350,  351,  352,  498,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  374,
   -1,  376,   -1,   -1,  481,  482,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  498,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  498,
};
#define YYFINAL 19
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 512
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
"CACHE","CAST","CHARACTER","CHECK","CHECK_POINT_LEN","COLLATE","COLLATION",
"COMMA","COMMIT","COMMITTED","COMPUTED","CONCATENATE","CONDITIONAL",
"CONSTRAINT","CONTAINING","COUNT","CREATE","CSTRING","CURRENT","CURSOR",
"DATABASE","DATE","DB_KEY","KW_DEBUG","DECIMAL","DECLARE","DEFAULT","DELETE",
"DESC","DISTINCT","DO","DOMAIN","DROP","ELSE","END","ENTRY_POINT","EQL",
"ESCAPE","EXCEPTION","EXECUTE","EXISTS","EXIT","EXTERNAL","FILTER","FOR",
"FOREIGN","FROM","FULL","FUNCTION","GDSCODE","GEQ","GENERATOR","GEN_ID","GRANT",
"GROUP","GROUP_COMMIT_WAIT","GTR","HAVING","IF","IN","INACTIVE","INNER",
"INPUT_TYPE","INDEX","INSERT","INTEGER","INTO","IS","ISOLATION","JOIN","KEY",
"KW_CHAR","KW_DEC","KW_DOUBLE","KW_FILE","KW_FLOAT","KW_INT","KW_LONG",
"KW_NULL","KW_NUMERIC","KW_UPPER","KW_VALUE","LENGTH","LOGFILE","LPAREN","LEFT",
"LEQ","LEVEL","LIKE","LOG_BUF_SIZE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT",
"MERGE","MESSAGE","MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR",
"NEQ","NO","NOT","NOT_GTR","NOT_LSS","NUM_LOG_BUFS","OF","ON","ONLY","OPTION",
"OR","ORDER","OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","PAGE_SIZE",
"PARAMETER","PASSWORD","PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY",
"PRIVILEGES","PROCEDURE","PROTECTED","RAW_PARTITIONS","READ","REAL",
"REFERENCES","RESERVING","RETAIN","RETURNING_VALUES","RETURNS","REVOKE","RIGHT",
"RPAREN","ROLLBACK","SEGMENT","SELECT","SET","SHADOW","SHARED","SINGULAR",
"KW_SIZE","SMALLINT","SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING",
"STATISTICS","SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION",
"TRIGGER","UNCOMMITTED","UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR",
"VARIABLE","VARYING","VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH",
"WORK","WRITE","FLOAT_NUMBER","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER",
"ACTION","ADMIN","CASCADE","FREE_IT","RESTRICT","ROLE","COLUMN","TYPE",
"EXTRACT","YEAR","MONTH","DAY","HOUR","MINUTE","SECOND","WEEKDAY","YEARDAY",
"TIME","TIMESTAMP","CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP",
"NUMBER64BIT","SCALEDINT","CURRENT_USER","CURRENT_ROLE","KW_BREAK","SUBSTRING",
"RECREATE","KW_DESCRIPTOR","FIRST","SKIP","CONNECTION_ID","TRANSACTION_ID",
"BIGINT","CASE","NULLIF","COALESCE","USING","NULLS","LAST","ROWS_AFFECTED",
"LOCK","SAVEPOINT","STATEMENT","LEAVE","INSERTING","UPDATING","DELETING",
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
"label : symbol_label_name ':'",
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
"replace_view_clause : symbol_view_name column_parens_opt AS begin_string union_view check_opt end_string",
"alter_view_clause : symbol_view_name column_parens_opt AS begin_string union_view check_opt end_string",
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
"keyword_or_column : CONNECTION_ID",
"keyword_or_column : TRANSACTION_ID",
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
"order_item : value collate_clause order_direction nulls_placement",
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
"limit_range : '(' long_integer ')'",
"limit_range : '(' long_integer ':' long_integer ')'",
"limit_range :",
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
"internal_info : CONNECTION_ID",
"internal_info : TRANSACTION_ID",
"internal_info : GDSCODE",
"internal_info : SQLCODE",
"internal_info : ROWS_AFFECTED",
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
"symbol_UDF_name : valid_symbol_name",
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
const TOK	*token;

for (token = KEYWORD_getTokens(); token->tok_string; ++token)
    {
    SYM         symbol;
    STR         str_;

    symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) sym;
    symbol->sym_string = (TEXT *) token->tok_string;
    symbol->sym_length = strlen (token->tok_string);
    symbol->sym_type = SYM_keyword;
    symbol->sym_keyword = token->tok_ident;
    symbol->sym_version = token->tok_version;
    str_ = FB_NEW_RPT(*DSQL_permanent_pool, symbol->sym_length) str;
    str_->str_length = symbol->sym_length;
    strncpy ((char*)str_->str_data, (char*)symbol->sym_string, symbol->sym_length);
    symbol->sym_object = (void *) str_;
    HSHD_insert (symbol);
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
#if defined(__STDC__)
dsql_yyparse(USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous)
#else
dsql_yyparse(USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous)
#endif
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
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
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
{ yyval = make_node (nod_label, e_label_count, yyvsp[-1], NULL); }
break;
case 339:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 341:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 342:
{ yyval = NULL; }
break;
case 344:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 352:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 353:
{ yyval = make_list (yyvsp[0]); }
break;
case 354:
{ yyval = make_list (yyvsp[-1]); }
break;
case 355:
{ yyval = NULL; }
break;
case 359:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 362:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 364:
{ yyval = make_node (nod_replace_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 365:
{ yyval = make_node (nod_mod_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 367:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 368:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 369:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 370:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 371:
{ yyval = make_list (yyvsp[0]); }
break;
case 373:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 376:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 377:
{ yyval = yyvsp[-1]; }
break;
case 378:
{ beginning = lex_position(); }
break;
case 379:
{ yyval = (DSQL_NOD) MAKE_string(beginning, 
			       (lex_position() == end) ?
			       lex_position()-beginning : last_token-beginning);}
break;
case 380:
{ beginning = last_token; }
break;
case 381:
{ yyval = (DSQL_NOD) MAKE_string(beginning, 
					lex_position()-beginning); }
break;
case 382:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 383:
{ yyval = 0; }
break;
case 384:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 385:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 386:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 388:
{ yyval = NULL; }
break;
case 389:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 390:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 407:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 408:
{ yyval = NULL; }
break;
case 409:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 412:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 413:
{ yyval = yyvsp[0]; }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 416:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 417:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 422:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = yyvsp[0]; }
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 426:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 427:
{ yyval = yyvsp[0]; }
break;
case 428:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 430:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 431:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 432:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 433:
{ yyval = yyvsp[0]; }
break;
case 434:
{ yyval = yyvsp[0]; }
break;
case 435:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 436:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 437:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 438:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 463:
{ yyval = NULL; }
break;
case 464:
{ yyval = NULL; }
break;
case 465:
{ yyval = NULL; }
break;
case 466:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 467:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 468:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 470:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 471:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 472:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 473:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 475:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 476:
{ yyval = yyvsp[0]; }
break;
case 477:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 478:
{ yyval = yyvsp[0]; }
break;
case 479:
{ yyval = yyvsp[0]; }
break;
case 481:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 484:
{ yyval = NULL; }
break;
case 486:
{ yyval = NULL; }
break;
case 487:
{ yyval = yyvsp[0]; }
break;
case 488:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 504:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 505:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 507:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 508:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 509:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 515:
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
case 516:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 517:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 518:
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
case 519:
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
case 520:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 523:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 524:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 525:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 526:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 527:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 528:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 529:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 530:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 531:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 532:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 534:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 535:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 536:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 537:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 538:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 539:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 548:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 549:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 550:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 551:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
		    	}
break;
case 552:
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
case 553:
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
case 556:
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
case 557:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 558:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 559:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 560:
{ yyval = yyvsp[-1]; }
break;
case 561:
{ yyval = 0; }
break;
case 565:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 566:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 567:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 568:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 569:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 572:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 573:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 576:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 577:
{ yyval = (DSQL_NOD) NULL; }
break;
case 579:
{ yyval = (DSQL_NOD) NULL; }
break;
case 580:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 582:
{ yyval = (DSQL_NOD) NULL; }
break;
case 584:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 589:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 593:
{ yyval = yyvsp[0];}
break;
case 595:
{ yyval = yyvsp[0];}
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 603:
{ yyval = 0; }
break;
case 604:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 605:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 606:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 607:
{ yyval = (DSQL_NOD) 0; }
break;
case 608:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 609:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 611:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 612:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 613:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 614:
{ yyval = 0; }
break;
case 616:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 617:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 620:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 621:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 622:
{ yyval = make_list (yyvsp[0]); }
break;
case 623:
{ yyval = 0; }
break;
case 625:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 626:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
break;
case 627:
{ yyval = 0; }
break;
case 628:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 629:
{ yyval = 0; }
break;
case 630:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 631:
{ yyval = 0; }
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
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 642:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 643:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 644:
{ yyval = 0; }
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
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 649:
{ yyval = yyvsp[-1]; }
break;
case 650:
{ yyval = yyvsp[0]; }
break;
case 651:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 652:
{ yyval = 0; }
break;
case 653:
{ yyval = make_list (yyvsp[0]); }
break;
case 654:
{ yyval = 0; }
break;
case 656:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 658:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 659:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 660:
{ yyval = make_list (yyvsp[0]); }
break;
case 662:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 665:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 666:
{ yyval = yyvsp[-1]; }
break;
case 667:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 668:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 669:
{ yyval = make_list (yyvsp[-1]); }
break;
case 670:
{ yyval = NULL; }
break;
case 672:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 676:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 677:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 678:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 680:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 681:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 682:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 683:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 684:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 685:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 686:
{ yyval = make_list (yyvsp[0]); }
break;
case 687:
{ yyval = 0; }
break;
case 689:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 694:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
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
{ yyval = make_node (nod_top, 1, yyvsp[-2]);}
break;
case 703:
{ yyval = make_node (nod_top, 2, yyvsp[-4], yyvsp[-3]);}
break;
case 704:
{ yyval = 0;}
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
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 785:
{ yyval = yyvsp[-1]; }
break;
case 786:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_list (yyvsp[-1]); }
break;
case 835:
{ yyval = yyvsp[-1]; }
break;
case 836:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 837:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 847:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 848:
{ yyval = yyvsp[0]; }
break;
case 849:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 850:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 851:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 852:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 853:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 854:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 855:
{ yyval = yyvsp[-1]; }
break;
case 856:
{ yyval = yyvsp[-1]; }
break;
case 860:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 861:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 862:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 863:
{ yyval = yyvsp[0]; }
break;
case 864:
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
case 865:
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
case 866:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 867:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 869:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 871:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 872:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 873:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 878:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
break;
case 879:
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
case 880:
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
case 881:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 882:
{ yyval = make_parameter (); }
break;
case 883:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 884:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 885:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 886:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 887:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = yyvsp[0]; }
break;
case 892:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 894:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 895:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 896:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 897:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 898:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 901:
{ yyval = yyvsp[0];}
break;
case 906:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 907:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 908:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 909:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 910:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 911:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 913:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 914:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 915:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 917:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 918:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 920:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 921:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 922:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 924:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 927:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 928:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 932:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 933:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 934:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 935:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 936:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 942:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 943:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 952:
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
