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
extern "C" TEXT *DLL_EXPORT ERR_string(CONST TEXT*, int);
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

static CONST UCHAR
	INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static CONST UCHAR
	NULL_STRING [] = "";	

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
static SSHORT	log_defined, cache_defined;
static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

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
#define SIZE 423
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
#define FLOAT 458
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
  185,  185,  185,  185,  185,  185,  185,  190,  190,  191,
  192,  193,  202,  202,  194,  195,  195,  196,  205,  205,
  197,  206,  200,  200,  200,  201,  201,  201,  203,  203,
  203,  203,  198,  210,  199,  199,  204,  204,  188,  188,
  213,  214,  214,  215,  215,  215,  215,   11,  217,  217,
  217,  218,  218,  218,  218,  218,  218,   72,   81,  223,
  224,  220,  225,  225,  225,  226,  229,  233,  233,  234,
  234,  235,  235,  148,  222,   88,   90,  221,  221,   71,
   84,  238,  238,  238,  239,  242,  242,  243,  243,  243,
  243,  243,  243,  243,  243,  243,  243,  243,  243,  243,
  243,  243,  240,  240,  241,    2,  245,  245,  245,  245,
  245,  245,  245,  253,  254,  251,  251,  255,  255,  255,
  255,  255,  255,  255,  246,  246,  256,  256,  256,  256,
  256,  256,  256,  250,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  261,  261,  261,  258,  258,
  260,  260,  259,  257,  257,  257,  252,  252,  248,  249,
  249,  262,  262,  262,  262,  263,  263,  247,  264,  264,
  265,  265,    8,  266,  266,  266,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  102,  102,  145,  145,  269,
  269,  272,  272,  274,  274,   54,   54,  271,  271,  271,
  271,  271,  271,  271,  271,  271,  279,  279,  270,  270,
  270,  270,  280,  280,   62,   62,   62,   56,   56,  276,
  276,  276,  273,  273,  273,  284,  284,  284,  283,  283,
  282,  282,  282,  277,  277,  287,  285,  285,  285,  286,
  286,  278,  278,  278,  278,  288,  288,   19,   19,   19,
  290,  290,  290,   16,   17,  294,  294,    4,   15,  293,
  293,  295,  295,  296,  296,  289,  297,  297,  298,  298,
  299,  299,  299,  299,  300,  300,  301,  301,  302,  302,
  304,  304,  304,  305,  305,  305,  306,  306,  306,  303,
  308,  308,  308,  309,  309,  307,  307,  310,  312,  312,
  311,  311,  291,   18,  313,  313,  313,  314,  314,  317,
  317,  318,   66,   66,   66,  319,  319,  319,  315,  315,
  320,  320,  321,  321,  316,  325,  322,  322,  322,  322,
  326,  326,  326,  327,  327,  327,  227,  227,  228,  228,
  329,  329,  330,  330,  330,  323,  333,  333,  334,  334,
  335,  335,  336,  336,  337,  337,  208,  208,  339,  339,
  236,  236,   32,  237,  237,  237,  237,  237,  237,  237,
  237,  231,  231,  340,  340,  341,  341,  341,  341,  341,
  343,  343,  343,  232,  232,  230,  230,  347,  347,  347,
  324,  324,  348,  349,  349,  349,  349,  349,  350,  350,
  351,  351,  352,  352,  353,  353,  353,  354,  354,   10,
   10,  356,  356,    7,    7,  357,  358,  359,   20,   20,
  360,  361,  362,  362,  189,  331,  331,    3,    3,  364,
  364,  364,  366,  366,   62,  365,  365,  367,  367,   35,
   35,   86,   85,   85,  355,  355,  368,  369,  369,  209,
  209,  209,  147,  363,  363,  112,  112,  112,  112,  371,
  371,  371,  371,  371,  371,  371,  371,  371,  371,  371,
  371,  372,  372,  372,  372,  372,  372,  372,  372,  377,
  377,  377,  377,  377,  377,  377,  377,  377,  377,  377,
  377,  377,  377,  377,  377,  384,  384,  373,  373,  374,
  374,  374,  374,  375,  375,  379,  379,  380,  380,  380,
  380,  378,  381,  376,  376,  382,  382,  382,  385,  385,
  386,  383,  388,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,  157,
  157,  157,  389,  387,  387,  152,  152,  393,  393,  393,
  393,  393,  391,  391,  391,  391,  391,  219,  153,  153,
  154,  155,  155,  155,  155,  155,   52,   52,   21,   21,
  244,  394,   55,  124,  275,  275,   96,  390,  390,  390,
  390,  395,  395,  395,  395,  395,  395,  395,  395,  395,
  395,  395,  396,  344,  345,  345,  345,  342,  342,  392,
  346,  346,  398,  398,  399,  399,  400,  400,  403,  403,
  401,  401,  406,  406,  405,  402,  404,  397,  397,  397,
  397,  397,  397,  397,  397,  328,  328,  156,   49,  281,
  123,  149,  370,  166,  212,  268,   64,   61,  216,  113,
   67,  332,  211,   36,   42,  338,  267,   40,   43,  207,
   41,  292,  407,
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
    1,    1,    2,    1,    2,    2,    1,    3,    4,    2,
    6,    4,    1,    1,    7,    8,    6,    4,    2,    0,
    4,    2,    1,    3,    0,    2,    4,    0,    1,    1,
    3,    3,    6,    2,    2,    2,    3,    0,    1,    2,
    4,    1,    3,    2,    2,    2,    1,    4,    1,    3,
    0,    1,    1,    1,    3,    3,    3,    7,    7,    7,
    7,    1,    1,    3,    4,    7,    2,    1,    3,    1,
    1,    6,    3,    0,    0,    0,    0,    3,    0,    9,
    9,    1,    1,    0,    2,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    5,    5,    5,    5,
    5,    5,    2,    0,    4,    2,    3,    3,    2,    2,
    3,    3,    2,    3,    6,    1,    2,    4,    3,    2,
    2,    2,    2,    3,    1,    3,    3,    3,    2,    2,
    4,    4,    5,    1,    1,    1,    1,    1,    1,    1,
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
    5,    5,    6,    2,    5,    1,    0,    3,    2,    1,
    0,    2,    0,    1,    0,    3,    1,    0,    1,    2,
    1,    1,    1,    1,    2,    2,    1,    2,    3,    1,
    1,    3,    3,    1,    2,    3,    1,    2,    0,    2,
    1,    1,    0,    1,    1,    1,    3,    2,    3,    0,
    1,    3,    4,    3,    1,    3,    4,    3,    0,    1,
    3,    4,    1,    1,    0,    2,    2,    0,    4,    0,
    2,    0,    2,    0,    9,   10,    2,    1,    1,    0,
    2,    4,    2,    2,    4,    2,    1,    1,    1,    1,
    1,    3,    1,    2,    3,    2,    1,    3,    1,    1,
    6,    3,    3,    2,    3,    0,    1,    3,    1,    1,
    1,    2,    1,    1,    1,    2,    1,    2,    1,    2,
    0,    3,    0,    1,    3,    1,    1,    1,    1,    3,
    1,    1,    1,    2,    0,    2,    0,    3,    5,    0,
    2,    0,    4,    1,    2,    1,    1,    0,    1,    3,
    2,    1,    1,    2,    1,    4,    2,    1,    3,    8,
    5,    1,    3,    1,    1,    4,    4,    4,    1,    1,
    5,    5,    1,    3,    3,    1,    1,    7,    7,    5,
    3,    0,    1,    1,    1,    2,    0,    1,    1,    1,
    0,    3,    1,    3,    1,    0,    3,    1,    3,    1,
    3,    3,    1,    1,    3,    1,    3,    3,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    1,    1,    5,    6,    3,
    4,    5,    6,    3,    4,    3,    4,    3,    4,    4,
    5,    4,    4,    3,    4,    1,    1,    1,    1,    3,
    3,   10,   10,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    3,    3,    3,    3,    3,    3,
    3,    3,    1,    1,    1,    1,    3,    1,    1,    1,
    1,    1,    4,    1,    3,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    1,    2,
    1,    1,    1,    1,    1,    2,    1,    1,    1,    1,
    1,    4,    5,    5,    5,    5,    5,    5,    5,    5,
    5,    5,    6,    6,    6,    8,    4,    4,    3,    6,
    1,    1,    6,    6,    1,    1,    4,    6,    4,    5,
    3,    5,    4,    5,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    0,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  558,
  559,  560,    0,  615,  724,  725,  729,  730,  469,    0,
    0,    0,    0,    0,    0,  406,  570,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  891,    0,   22,  889,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  483,    0,    0,   38,   35,   37,    0,   36,
    0,  973,    0,    0,    0,    0,   33,   69,   67,  965,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,    0,  671,    0,    0,
  967,    0,    0,    0,  108,  564,  972,    2,    0,    0,
    0,    0,  453,  435,  436,  437,  438,  439,  440,  441,
  442,  443,  444,  445,  446,  447,  448,  449,  450,  451,
  452,  454,  455,  456,  457,  458,    0,  434,    0,  957,
    0,  413,  961,    0,  410,  964,    0,  673,    0,  409,
  968,    0,  568,  887,    0,  151,  104,    0,  105,    0,
  218,  763,  953,    0,  103,  146,  960,    0,    0,   99,
    0,  106,  893,    0,  100,    0,  101,    0,  102,  971,
  147,  107,  623,  624,    0,  892,  890,    0,   75,    0,
  958,    0,  491,  956,  484,    0,  490,  495,  485,  486,
  493,  487,  488,  489,  494,    0,   31,    0,   40,  750,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  897,    0,  878,  641,
  643,    0,  644,  646,  946,  647,    0,  648,  637,    0,
    0,    0,    0,    0,    0,    0,  587,  576,    0,  579,
  581,  582,  583,  584,  590,  591,    0,  672,  966,    0,
  109,    0,  110,    0,  111,    0,    0,  616,    0,  614,
    0,    0,    0,    0,  470,    0,    0,  376,    0,  221,
    0,  416,  407,  467,  468,    0,    0,    0,    0,    0,
    0,  425,    0,  382,  383,    0,  574,  572,  888,    0,
    0,    0,    0,    0,    0,    0,  154,  133,    0,   97,
    0,    0,  112,    0,  122,  121,    0,    0,    0,    0,
    0,    0,   76,    0,  949,    0,    0,  726,  727,  492,
    0,  948,  870,  869,  868,    0,    0,  871,  872,    0,
    0,  874,  353,  354,  348,    0,  352,  866,  873,    0,
  753,    0,    0,   28,    0,    0,    0,   72,   70,  969,
   68,   34,    0,    0,    0,  755,    0,   53,    0,    0,
    0,    0,    0,  566,    0,    0,    0,    0,  856,  884,
    0,    0,  858,    0,    0,  885,    0,  879,    0,  860,
  861,  862,  880,  881,    0,  882,  883,    0,    0,    0,
  886,    0,    0,    0,    0,    0,    0,  760,  853,  854,
  855,  859,  839,    0,  838,    0,  842,  900,  901,  841,
  835,  836,  837,  840,  898,  899,  921,  922,  925,  926,
    0,    0,  650,    0,  737,    0,    0,  651,    0,    0,
  613,    0,  588,    0,  585,    0,  586,  611,    0,  606,
    0,    0,  580,  764,  733,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  620,  617,    0,  186,    0,    0,
  130,  475,  173,  174,    0,  473,    0,    0,    0,    0,
  476,    0,  471,    0,    0,  420,  422,  421,    0,  423,
    0,  417,  219,    0,    0,  272,    0,    0,    0,    0,
  429,  430,    0,  460,    0,    0,    0,    0,    0,    0,
    0,  387,  386,  479,    0,    0,  150,    0,    0,  159,
    0,  158,    0,  165,  148,    0,  163,  166,  167,  155,
    0,    0,  514,  550,  517,    0,  551,    0,    0,  518,
    0,    0,    0,  541,  554,  513,  536,  515,  516,  511,
  498,  376,  496,  497,  499,    0,    0,  508,  509,  510,
  512,    0,    0,    0,    0,    0,  113,    0,  114,    0,
  125,    0,    0,  202,    0,    0,  374,    0,  221,    0,
    0,   84,    0,    0,  745,    0,    0,    0,    0,    0,
  826,  827,  828,    0,    0,    0,  766,  770,  771,  772,
  773,  774,  775,  776,  777,  778,  779,  780,  875,  876,
  877,  867,    0,    0,    0,  752,   29,    0,    0,   45,
   73,    0,    0,   27,    0,  758,    0,    0,    0,  721,
    0,    0,    0,    0,    0,  565,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  322,  970,    0,
    0,    0,    0,    0,    0,    0,  642,    0,    0,  645,
    0,    0,    0,    0,  654,  962,  562,    0,  895,  561,
    0,  589,    0,  597,  593,  592,    0,    0,    0,  608,
  596,    0,    0,  731,  732,    0,    0,    0,  374,    0,
    0,    0,    0,    0,    0,    0,  131,    0,    0,    0,
    0,    0,    0,  376,  419,    0,  376,  377,  424,  506,
    0,  274,    0,  268,  270,  374,  241,  954,    0,    0,
  376,    0,    0,  210,  374,    0,    0,    0,    0,  246,
  240,  243,  244,  245,  428,  465,  464,  427,  426,    0,
    0,    0,    0,    0,    0,  385,    0,  376,    0,  156,
  160,    0,  164,    0,    0,  537,  538,  555,    0,  552,
    0,    0,  544,  542,  543,    0,    0,    0,    0,  507,
    0,    0,    0,    0,  545,    0,    0,  374,    0,  201,
    0,  204,  206,  207,    0,    0,    0,    0,    0,  221,
   79,    0,    0,   78,  525,  526,  950,    0,    0,    0,
  769,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  350,  356,  357,  355,  754,    0,    0,    0,    0,
    0,    0,    0,    0,   54,   62,   64,    0,    0,   71,
    0,    0,    0,  757,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  938,  939,  940,  941,
  942,  943,  944,  945,    0,    0,    0,    0,    0,    0,
  931,    0,    0,    0,  669,    0,    0,  851,  852,  919,
    0,    0,  847,  952,    0,    0,    0,    0,    0,    0,
  857,  762,  761,    0,    0,    0,    0,  659,  660,    0,
    0,  652,  655,  563,  896,  598,  607,  602,  601,    0,
  612,  765,  734,  735,  374,    0,    0,    0,    0,  621,
    0,    0,  629,  182,  176,  183,    0,  179,    0,    0,
  178,    0,  189,  170,  169,  894,  172,  171,  477,    0,
    0,    0,  418,  273,    0,  212,  376,  214,    0,  377,
  209,  215,    0,    0,    0,    0,  431,  432,  376,  374,
  377,    0,    0,    0,  403,    0,  197,  198,  157,    0,
    0,    0,    0,    0,    0,  553,    0,    0,  376,  377,
    0,  502,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  199,    0,    0,    0,    0,
  363,    0,   83,    0,    0,    0,    0,   88,   85,    0,
    0,    0,  728,  955,    0,    0,  781,    0,    0,    0,
  807,  806,    0,    0,    0,  814,  829,  824,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  768,    0,
   66,   58,   59,   65,   60,   61,    0,    0,   23,    0,
    0,    0,   43,    0,    0,    0,  739,  759,    0,  722,
    0,  738,    0,    0,    0,    0,    0,    0,  221,    0,
  902,    0,    0,  917,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  927,    0,  937,    0,
    0,    0,    0,    0,    0,  918,  863,    0,    0,    0,
    0,    0,    0,  674,    0,    0,    0,    0,    0,  604,
  605,  609,    0,  200,    0,    0,  622,  633,    0,    0,
    0,    0,    0,    0,  190,    0,    0,  414,  224,  225,
  226,  227,  228,  229,    0,    0,    0,  277,    0,  222,
    0,    0,    0,    0,  247,    0,  461,  462,  433,    0,
    0,    0,    0,    0,    0,  376,  481,  377,  168,  951,
    0,    0,  520,    0,  519,  556,    0,  548,    0,    0,
    0,  500,    0,  528,    0,    0,  530,  533,  535,  374,
    0,    0,    0,  120,    0,  205,  376,    0,    0,  375,
    0,   98,    0,  118,    0,   94,    0,    0,    0,    0,
   87,    0,  822,  823,    0,    0,    0,    0,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,  815,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   57,   63,   55,   56,   24,  743,
  744,    0,  741,  748,  749,  746,    0,    0,  720,    0,
    0,  908,  907,    0,    0,  904,  903,    0,  912,  911,
  910,  909,  906,  905,    0,    0,  933,    0,    0,    0,
  932,    0,    0,    0,  667,    0,    0,  662,    0,  663,
    0,  680,  676,  678,    0,    0,    0,    0,    0,  375,
  626,  627,    0,  184,  180,    0,    0,  195,  191,    0,
  377,  280,    0,    0,  377,  278,    0,    0,    0,  232,
    0,    0,  248,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  478,  522,    0,  523,    0,  135,    0,    0,
    0,  139,  141,  142,  143,  503,  505,  501,  532,    0,
    0,  377,    0,    0,    0,    0,  358,    0,  364,  376,
   89,    0,    0,   91,    0,    0,    0,    0,    0,    0,
  831,  830,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   41,    0,    0,  723,  220,  920,  913,  914,    0,  915,
  929,  928,    0,  934,  923,    0,  924,    0,  665,    0,
  546,    0,  687,    0,    0,  684,  688,  689,  691,  692,
  693,    0,    0,  635,  377,  359,    0,  175,  192,  377,
  145,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  285,  290,  284,
    0,    0,    0,  294,  295,  296,  297,  298,  299,  301,
  302,  304,  307,  267,    0,    0,  208,  233,    0,    0,
    0,    0,  234,  237,    0,    0,  377,  401,  402,  398,
  397,  400,  399,    0,  521,  549,  144,  132,  140,    0,
  376,  264,  127,  377,    0,    0,  378,  365,    0,    0,
   92,   93,   80,    0,  793,  801,    0,  794,  802,  797,
  805,    0,    0,  795,  803,  796,  804,  790,  798,  791,
  799,  792,  800,  740,    0,    0,  930,  668,    0,    0,
    0,    0,    0,  704,  706,    0,  701,    0,  265,  185,
  415,    0,  310,    0,    0,  313,  314,    0,  306,    0,
    0,    0,  305,    0,  335,  336,  293,  300,    0,  303,
  286,    0,  291,    0,  339,  292,  211,  216,  235,    0,
    0,  238,    0,  250,  252,  405,  377,    0,  380,    0,
    0,    0,  370,  371,    0,    0,   77,   95,    0,    0,
  916,    0,    0,  690,  685,  705,    0,    0,  283,    0,
  308,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  329,  330,  347,    0,    0,    0,    0,  342,  340,  239,
    0,    0,    0,    0,    0,  266,  377,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  712,    0,    0,
    0,  282,  279,  309,    0,    0,    0,  312,    0,    0,
    0,  318,    0,  321,    0,  346,  345,  959,  344,    0,
    0,    0,    0,    0,  249,    0,  256,    0,  255,  381,
  373,    0,    0,  366,  119,    0,    0,    0,  714,  703,
    0,    0,  715,    0,  711,    0,    0,    0,    0,    0,
    0,    0,    0,  332,  331,  341,  343,  236,    0,    0,
  260,  259,  258,    0,    0,  636,  833,  710,    0,  717,
  324,    0,    0,  311,    0,    0,    0,  333,  263,  261,
  262,    0,    0,    0,    0,    0,  337,  315,    0,    0,
    0,    0,  716,  327,  316,  832,  719,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1504,   27,   28, 1505,
   30,   31,   32,   33,   34,   35,   36, 1506,   38, 1507,
  615,  103,  383,  873, 1119,  104,  649,  874,  105,  387,
  654,  128,  106,  107,  239,  945,  118,  875,  876,  179,
  208,  108,  877,  109,  389,   79,  219,  353,  436,  610,
 1056,  372,  833,  581, 1471,  810,  611,  612,  613, 1058,
  220, 1310,   71,  169,   72,  215, 1784, 1262,  200,  205,
  207,  209,  195,  187,  189,  202,  212,  135,  291,  293,
  295,  343,  597,  599,  380,  240,  760,  515,  625,  993,
  347,  602, 1044, 1254,  548,  709, 1019,  500,  501,  530,
  339,  582, 1030, 1400,  731, 1198, 1401, 1402, 1403, 1404,
 1405,  626,  196,  188,  335,  555,  336,  337,  556,  557,
  558,  559, 1229,  987,  503,  504,  977,  975,  505,  981,
  976, 1189, 1364,  982,  983, 1194,  349,  821,  822,  823,
  824,  762, 1000, 1378,  583,  764,  438,  826,  933,  524,
 1325, 1199,  439,  440,  441,  925,  442, 1379, 1380,  533,
 1533, 1683, 1215,  770,  771,  757,  772,  773,  774, 1684,
 1685, 1762,  317,  528, 1206, 1508,  175,  525,  526, 1207,
 1208, 1373, 1660, 1509, 1510, 1511, 1512, 1624, 1513, 1514,
 1515, 1516, 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1706,
 1749, 1608, 1670, 1751, 1668,  443,  688, 1344,  444,    0,
    0, 1063, 1625, 1677, 1678, 1717,  375,  376,  445, 1049,
 1260, 1417,    0,    0, 1050, 1051,  267,  466, 1556,  358,
 1179, 1358, 1641, 1642, 1643, 1644, 1177,  326,  544,  788,
 1227,  545,  786,  203,   56,  321,  180,  142,  304,  167,
  311,  172,  748,  745,  312,  322,  778,  323,  541, 1011,
  168,  305,  512,  546, 1228,   93,  178,  223,  584,  585,
  750, 1031,  751, 1032, 1033,  588,  589,  590,  591, 1024,
  836,  592,  593,  594,  803,  595, 1473,  800,   40,   41,
   42,  136,   58,  405,  183,  328,  278,  279,  280,  281,
  282,  283,  284,  285,  286,  715,  479,  960, 1182,  480,
  481,  720,   43,  141,  300,   44,  494,  495, 1187,  734,
  973,  122,  702, 1484,  660,  123,  124,  268,  467,  468,
  469,  705,  946,  947,  948,  949, 1171,  446, 1159, 1475,
 1476,  447, 1478,  448,  449,  450,    0, 1698, 1598, 1699,
 1700, 1701, 1745, 1785,  395, 1131,   45,   46,  359,   47,
   48,  487,  488,  882, 1127, 1312, 1316,  396,  657,  192,
  627,  628,  629,  630,  631,  632,  633,  634,  635,  636,
  637,  638, 1279, 1074, 1076, 1077,  932,  687,  451,  452,
  453,  454,  379,  217,  455,  456,  915,  457,  458,  459,
  460,  679,  919, 1160, 1155,  680,  461,
};
static short yysindex[] = {                                   4936,
 1323, -122, 3626,  104, 1430,  -11, 4032,  359, 1691,  568,
   97, 2644, -122, 1270,  439,  236, 1530,  236,    0,  735,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -58,    0,    0,    0,    0,    0,    0, 6268,
  236,  236,  236,  236,  236,    0,    0,  405, 1423,  236,
  236,  236,  593,  236,  419,  236,  236,    0,  236,  236,
    0,  879,    0,  482,    0,    0,  605,  236,    0,  236,
  236,  236,  642,  236,  236,  236,  236,  419,  236,  236,
  236,  236,    0,  236,  587,    0,    0,    0,  972,    0,
  972,    0,  745,  821,  211, 1165,    0,    0,    0,    0,
  236,  236,  236,  861,  892,  968,  130, 2453,  919,  187,
  208,  687,  893,    0,  236, 1069, 1243,    0,  953,  236,
    0,  236,  236,  236,    0,    0,    0,    0, 1145,   61,
 1122,  -21,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1029,    0, 1423,    0,
   82,    0,    0, 1418,    0,    0, 1733,    0,  549,    0,
    0, 1070,    0,    0, 1061,    0,    0, 1241,    0, 1269,
    0,    0,    0, 1423,    0,    0,    0,  -95, 1418,    0,
   24,    0,    0, 1230,    0, 1223,    0,  972,    0,    0,
    0,    0,    0,    0, 1252,    0,    0,  236,    0, 1265,
    0, 1197,    0,    0,    0,  236,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1393,    0,  236,    0,    0,
    0,  639, 1242,  898,  236,  694, 1332, 1638, 1343, 1376,
  639, 1305,  898, 1329, 1333, 1266,    0, 7494,    0,    0,
    0, 7494,    0,    0,    0,    0, 5584,    0,    0, 1344,
  236, 1453, 1335,  667,  236, 1361,    0,    0, 1243,    0,
    0,    0,    0,    0,    0,    0,  236,    0,    0, 1418,
    0, 1230,    0,  972,    0, 7494, 1425,    0, 1407,    0,
  499, 1519,  775,  -21,    0, 1602,  911,    0,  236,    0,
 1029,    0,    0,    0,    0,  236, 1485,  -25, 1438,   16,
 1877,    0, 8990,    0,    0,  975,    0,    0,    0, 1869,
 1869, 1423, 1583, 1423, 1229, 1241,    0,    0, 8768,    0,
  236,  236,    0, 1485,    0,    0, 1671,  732, 1924,  236,
 1705,  236,    0, 1938,    0,  297, 3382,    0,    0,    0,
 1524,    0,    0,    0,    0, 1528, 1534,    0,    0, 1541,
 1441,    0,    0,    0,    0, 1967,    0,    0,    0, 1211,
    0,  236, 1586,    0,  236,  236,  118,    0,    0,    0,
    0,    0,  236,  236,  765,    0,  236,    0, 1703,  236,
 1985,  639, 1628,    0,  236, 1993, 1995, 2035,    0,    0,
 2050, 2051,    0, 2065, 2073,    0, 2080,    0, 2082,    0,
    0,    0,    0,    0, 2083,    0,    0, 6181, 2085, 2088,
    0, 7494, 7494, 6407,  236, 2091, 1709,    0,    0,    0,
    0,    0,    0, 2041,    0, 2012,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1758,    0, 2559,    0, 1812, 2099,    0,  -33,  132,
    0,  550,    0,  -75,    0,  -75,    0,    0, 2100,    0,
  148, 1718,    0,    0,    0, 2103,   69, 2089,    0, 1485,
 2111, 1889, 2705, 2112,    0,    0, 1774,    0,  136, 1809,
    0,    0,    0,    0, 1423,    0, 1869, 1869, 1869, 1869,
    0, 2115,    0, 1882, 1883,    0,    0,    0, 1862,    0,
 8768,    0,    0, 8768, 1677,    0, 1418, 1906,  236, 7124,
    0,    0,  976,    0,  236, 1394, 1733,    0, 1773, 1737,
 1704,    0,    0,    0,  796, 1778,    0, 1720,  419,    0,
 1423,    0, 1901,    0,    0, 1229,    0,    0,    0,    0,
  213, 1732,    0,    0,    0, 1735,    0, 1781, 2145,    0,
 1838, 2149,   51,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2106,  156,    0,    0,    0,
    0,  140, 2150, 2154, 2149, 1418,    0, 1879,    0, 1936,
    0, 1423, 1423,    0,  -25,  549,    0, 1821,    0, 1796,
 2166,    0, 4969,  193,    0, 1826, 1830, 2180, 3707, 2182,
    0,    0,    0, 3622, 4910,   17,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1828, 1441,  236,    0,    0, 1537, 1786,    0,
    0, 1760,  898,    0, 1910,    0, 1854, 1270, 2190,    0,
 1910, 1537, 1911, 1912,  236,    0, 1163, 6530,  506,  236,
 7494, 1192, 1217, 1272, 2561, 7494, 3707, 2559, 1782,  603,
 7494, 6530, 1174, 1174, 1270, 1907, 2194,    0,    0, 6653,
  236, 7494, 7494, 7494, 7494, 7494,    0, 7494,  239,    0,
  121, 1783, 6530,  236,    0,    0,    0, 1054,    0,    0,
  -32,    0, 1792,    0,    0,    0,  236,  429,  236,    0,
    0,  236,  236,    0,    0, 6530, 1975,  -25,    0,  236,
  879, 7494,  236, 1788, 1423, 1423,    0,  645, 1720, 1720,
 1789,  419,  775,    0,    0, 2204,    0,    0,    0,    0,
 1970,    0,  236,    0,    0,    0,    0,    0, 1976, 2209,
    0, 1952, 1969,    0,    0, 1926, 1928,  972, 1978,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  419,
  236, 8958, 1870, 1888, 1890,    0,  419,    0, 1611,    0,
    0, 1863,    0,  102, 1866,    0,    0,    0,  419,    0,
 2145,  115,    0,    0,    0, 1983,  115, 1872,  115,    0,
 2245,  419,  419,  419,    0, 1485,  236,    0, 1935,    0,
 1861,    0,    0,    0,  975, 1875,  236, 1902,  233,    0,
    0, 2255, 2022,    0,    0,    0,    0,  297,  236, 1880,
    0, 1880, 4824,  492, 7494, 7494, 4299, 2263, 1042, 4478,
 7494, 4554, 1246, 4797, 5032, 6776, 5114, 5267, 5349, 3707,
 3707,    0,    0,    0,    0,    0,  236,  236,  236,  236,
  236,  236,  180, 2264,    0,    0,    0, 1537, 1913,    0,
  380, 1939,  236,    0,  687, 6530, 1939, 2265, 2264, 1537,
  442, 1989, 7494, 7494, 2049, 7494, 2271, 7494, 2277, 2221,
 7494, 7494, 7494, 7494, 7494, 7494,    0,    0,    0,    0,
    0,    0,    0,    0, 2005, 1447,   63, 7494,  877, 6530,
    0, 3707, 1727, 2559,    0, 2278,  687,    0,    0,    0,
 2559, 1916,    0,    0, 2043, 1174, 1174, 1317, 1317,  708,
    0,    0,    0,  121, 2289, 2286, 1623,    0,    0, 3707,
 2009,    0,    0,    0,    0,    0,    0,    0,    0,  -55,
    0,    0,    0,    0,    0, 2001, 1875,    0, 1833,    0,
 2287, 1835,    0,    0,    0,    0, 2002,    0, 1869, 2076,
    0,  645,    0,    0,    0,    0,    0,    0,    0, 2299,
 3707, 1429,    0,    0, 2045,    0,    0,    0, 1429,    0,
    0,    0,  972,  972, 1841, 2308,    0,    0,    0,    0,
    0,  -42,   -6,   53,    0, 2087,    0,    0,    0,  236,
  104, 2007, 1929, 1970, 2312,    0, 1720, 2011,    0,    0,
  901,    0, 2291,  236, 1247,  419, 2314, 2315, 2317, 2096,
  549, 2045, 1869, 1809,  -25,    0, 1778,  687, 1908, 1921,
    0,  101,    0,  419, 1973, 2054, 4969,    0,    0,  419,
 1878, 2055,    0,    0, 2329, 2331,    0, 1746, 2559, 2341,
    0,    0, 2559, 2344, 6899,    0,    0,    0, 2034, 2349,
 2559, 2350, 1527, 2352, 2559, 2353, 7494, 7494, 2263, 7494,
 7022, 2358, 2559, 2359, 2361, 2559, 2363, 7494, 2559, 2368,
 2559, 2370, 2371, 2559, 2380, 2381, 2559, 2384,    0, 2163,
    0,    0,    0,    0,    0,    0, 2104, 1537,    0, 1537,
  180, 2264,    0,  257,  257,  108,    0,    0, 5584,    0,
 2018,    0, 2265, 2264, 2383,  442, 2259, 2388,    0, 2519,
    0, 2587, 7494,    0, 2698, 2721, 2848, 2895, 2902, 2918,
 7494,  419, 6530, 2559, 2006, 6530,    0, 7494,    0, 2128,
  640, 7494, 6530, 7494, 7494,    0,    0, 1623, 2397, 6530,
  236,  121, 2052,    0, 2060, 2061, 2107, 2169, 2120,    0,
    0,    0, 2045,    0, 1908,  997,    0,    0, 2021, 1423,
 2066, 1720, 2067, 1720,    0, 3707,  764,    0,    0,    0,
    0,    0,    0,    0, 2013, 2191, 2045,    0, 7494,    0,
 2175, 2057, 1841,  879,    0, 3707,    0,    0,    0, 2081,
 2086, 2097, 2098, 2113, 2119,    0,    0,    0,    0,    0,
 2429,  104,    0, 1789,    0,    0,  115,    0, 1429, 2123,
  115,    0,  115,    0, 1970, 2435,    0,    0,    0,    0,
  975, 2191, 1720,    0, 1809,    0,    0, 5584, 2205,    0,
  515,    0, 2287,    0, 2454,    0, 2457, 1423,  -18, 2467,
    0, 1423,    0,    0, 7494, 2092, 2092, 1270, 2468, 2019,
    0, 2092, 2092, 7494, 2092, 2092, 1816, 2559,    0, 1667,
 7494, 2559, 2092, 2092, 2092, 2092, 2559, 2092, 2092, 2092,
 2092, 2092, 2092, 2125,    0,    0,    0,    0,    0,    0,
    0, 2084,    0,    0,    0,    0, 1812, 6530,    0,  442,
 2383,    0,    0, 8768, 2483,    0,    0, 3184,    0,    0,
    0,    0,    0,    0, 3204,  123,    0, 6530, 2216, 2090,
    0, 6530, 3225, 2032,    0, 1656, 2559,    0, 2038,    0,
 1623,    0,    0,    0,  121, 3155, 3707, 2130, 2191,    0,
    0,    0, 1869,    0,    0, 1423, 1611,    0,    0,  766,
    0,    0,  236, 5694,    0,    0, 2559, 2249, 2175,    0,
 1068,  236,    0, 2197,  830, 2101, 2201, 2105, 2228, 2203,
 2241, 2045,    0,    0, 2505,    0, 2510,    0, 2199, 2249,
 2123,    0,    0,    0,    0,    0,    0,    0,    0, 2045,
 1778,    0, 1611, 2087, 2236, 2172,    0, 1875,    0,    0,
    0, 2195,  307,    0, 1970, 2196, 2559, 2522, 2529,  687,
    0,    0, 2530, 2531, 2559, 2532, 2534, 7494, 7494, 2559,
 2537, 2539, 2540, 2542, 2543, 2548, 2550, 2551, 2552, 2553,
    0,  257, 1783,    0,    0,    0,    0,    0,  419,    0,
    0,    0, 6530,    0,    0, 6530,    0, 1783,    0, 1232,
    0, 2297,    0, 2557, 2554,    0,    0,    0,    0,    0,
    0,   17, 1046,    0,    0,    0, 1720,    0,    0,    0,
    0, 8768,  164,  469, 2549, 1425, 2569, 2272, 7494, 2555,
 2576, 2558, 2563, 2564, 2567, 2279, 2568,    0,    0,    0,
 2322, 5587, 2574,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2594,  236,    0,    0, 2284, 2295,
  236, 1841,    0,    0,  972,  236,    0,    0,    0,    0,
    0,    0,    0, 2191,    0,    0,    0,    0,    0, 2191,
    0,    0,    0,    0,  162, 1783,    0,    0, 7494, 1423,
    0,    0,    0, 1423,    0,    0, 7494,    0,    0,    0,
    0, 2559, 2559,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2009, 2600,    0,    0, 2009, 3707,
  236,  126, 3155,    0,    0, 2280,    0, 2602,    0,    0,
    0,  109,    0, 7248,  236,    0,    0, 7494,    0, 2305,
 3707, 2372,    0, 3707,    0,    0,    0,    0,  125,    0,
    0,  655,    0, 2198,    0,    0,    0,    0,    0, 1841,
  972,    0, 2267,    0,    0,    0,    0, 2087,    0,  162,
 2604, 1623,    0,    0, 2009, 2559,    0,    0, 1656, 2120,
    0, 2120,   17,    0,    0,    0,  887, 1429,    0, 2596,
    0, 2273, 7371, 2445,  125,  835, 7494, 2603,  854, 1698,
    0,    0,    0,  236,  236,  104,  182,    0,    0,    0,
 2267,  382, 1841, 2281, 2282,    0,    0, 1623, 2620,  162,
 2319, 2120, 2629, 1783, 2130, 2130,  236,    0, 2631, 2634,
 1466,    0,    0,    0, 5812, 2262, 2636,    0,  197, 2246,
 2559,    0, 2376,    0,  125,    0,    0,    0,    0, 5694,
  655, 1841,  -47,  -47,    0, 2382,    0, 2242,    0,    0,
    0, 1623,  162,    0,    0, 2009, 2303, 2303,    0,    0,
  887, 2653,    0,  236,    0, 1907, 2048,  141, 2635, 2403,
 2394, 5694, 5694,    0,    0,    0,    0,    0, 2238, 1134,
    0,    0,    0, 1571, 2120,    0,    0,    0,  236,    0,
    0,  125, 2656,    0,  236, 5694, 2400,    0,    0,    0,
    0, 3707, 2130, 2665, 2669, 2071,    0,    0, 5694,   17,
 2303,  236,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  260,  628,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134, 4119,    0,    0,    0,    0,    0, 2711,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  964,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  846,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2379,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2334,    0,    0,    0,  196,    0,
  196,    0,    0,    0,    0, 2337,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  981,    0,
    0, 5935, 4201,    0,    0,    0, 1110,    0,    0,  736,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  923,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  103,    0,    0,    0,    0,  750,    0,
    0, 1218,    0,    0,    0,    0,    0, 1837,    0, 9003,
    0,    0,    0,    0,    0,    0,    0,    0,  103,    0,
  671,    0,    0, 2686,    0,    0,    0, 2463,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1220,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1285,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1289,    0,    0,
    0,    0,    0,    0,    0, 2268,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  544,    0,    0, 1312,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  103,
    0, 2686,    0, 2463,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1341,    0, 2450,    0,    0,    0,    0,
 1424,    0,    0,    0,    0,    0, 2466,  881, 6123,    0,
 1434,    0,    0,    0,    0,  413,    0,    0,    0, 2274,
 2274,    0,    0,    0, 1465, 2536,    0,    0,    0,    0,
    0,    0,    0, 2466,    0,    0, 1553,    0,    0,    0,
    0,    0,    0, 6436,    0, 2345,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1561,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1567,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1595,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2025,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1785,    0,    0,   94,    0,    0, 2421,    0,  142,    0,
    0,    0,    0,  865,    0,  865,    0,    0,  565,    0,
  925, 1156,    0,    0,    0,    0, 1220,    0, 1350, 2466,
    0,    0,  175, 1108,    0,    0,   84,    0, 3004,  293,
    0,    0,    0,    0,    0,    0, 2274, 2274, 2274, 2274,
    0,  336,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2487,    0,    0,    0,
    0,    0, 2450,    0,    0, 1248,    0, 1245, 2283,    0,
    0,    0,    0,    0,    0,  198,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1608,    0,    0,    0,    0,
 2168,  896,    0,    0,    0, 1152,    0,    0, 3550,    0,
    0, 3947,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1776, 2039,    0,    0,    0,
    0, 3982, 1278,    0, 3947,  103,    0,    0,    0,    0,
    0,    0,    0,    0,  881, 1383,    0,    0,    0,    0,
 2346,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  839,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  350,    0,    0, 4119,    0,    0,
  350,    0,    0,    0,    0,    0, 7617,    0, 7617,    0,
    0, 7617, 7617, 7617,    0,    0,    0, 2301,    0,    0,
    0,    0, 2782, 2952, 6058,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  921,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  772,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  881,    0,    0,
  128,    0,    0,  959,    0,    0,    0, 3068,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  430,    0,    0,    0,    0,    0,    0,    0, 2716,    0,
    0, 1101, 2211,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  458,  728,  840,    0,    0,    0, 2192,    0,
    0,    0,    0,    0, 1823,    0,    0,    0,    0,    0,
 3550,    0,    0,    0,    0,  668,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2466,    0,    0,  389,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7277,    0,
    0,    0,  247,    0,    0,    0,    0, 2459,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1612, 1624,    0,    0,    0,    0,    0,    0,
    0, 1625,    0,    0, 5935,    0, 1625, 1632, 1633,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1077,    0,    0, 7617,    0,    0,    0,
 1038,    0,    0,    0, 2335, 3112, 3263, 2472, 2645,    0,
    0,    0,    0,    0, 3435, 1187, 1073,    0,    0,    0,
 1034,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  534, 1286,    0,
  155,    0,    0,    0,    0,    0,    0,    0, 2274, 2311,
    0, 3669,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2489,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1777,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1663,    0,    0,    0,    0,
    0,    0,    0, 4028,    0,    0,    0,    0,    0,    0,
    0,    0, 1254,    0,    0,    0,    0,    0,    0,    0,
 1383, 2489, 2274, 1670,  881,    0, 2508, 5935, 1696,  159,
    0,    0,    0,    0, 7277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7752,    0,
    0,    0, 7880,    0,    0,    0,    0,    0,    0,    0,
 7891,    0, 7963,    0, 7974,    0,    0,    0,    0,    0,
    0,    0, 8111,    0,    0, 8131,    0,    0, 8214,    0,
 8297,    0,    0, 8308,    0,    0, 8342,    0,    0, 1084,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1612, 1700,    0, 2336,  357,    0,    0,    0,    0,    0,
    0,    0, 1702, 1719, 1736,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2338,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2433, 1255,    0,
 4779,    0, 2434,    0, 2441, 2442,    0,    0,  620,    0,
    0,    0, 2489,    0, 1696,    0,    0,    0, 2857,    0,
    0,    0, 2330,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2339,    0, 2524,    0,    0,    0,
 2423,    0, 1777, 2379,    0,    0,    0,    0,    0,  851,
  883,  926,  934,  962, 1003,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  786,
    0,    0,    0,    0, 2217,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1775,    0,    0,    0,    0,    0,
    0,    0, 1779,    0,    0,    0,    0,    0,  147,    0,
    0,    0,    0,    0,    0,    0,    0, 6058,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8438,    0, 8470,
    0, 8534,    0,    0,    0,    0, 8576,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1790,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1827,    0,    0,    0,
 1600,    0,    0,    0,    0,    0,    0,  276,    0,    0,
    0,    0, 2274,    0,    0,    0, 3429,    0,    0,    0,
    0,    0,    0, 2494,    0,    0, 2757, 1865, 2479,    0,
 2450,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2489,    0,    0,    0,    0,    0,    0,    0, 1822,
 1071,    0,    0,    0,    0,    0,    0,    0,    0, 2489,
 2508,    0,  400,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  171,    0, 8618,    0,    0, 7617,
    0,    0,    0,    0, 8714,    0,    0,    0,    0, 8746,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  357,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  706,    0, 2433,
    0,  497,    0,    0,  847,    0,    0,    0,    0,    0,
    0,  913, 2767,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2500,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2095,    0,    0,  106,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  166,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 8842, 8933,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1225,    0,    0,    0,  875,    0,
    0,    0,    0,    0,    0, 2770,    0,    0,    0,    0,
    0, 2752,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2753,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2509,    0,    0,    0,    0,    0, 2095,
  205,    0,  163,    0,    0,    0,    0,    0,    0,    0,
  598,  305,    0,    0,  200, 2757,    0,    0,    0,  379,
    0,  306, 5863,    0,    0,    0, 2767,    0,    0,    0,
    0,    0,  270,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  362,    0, 1777,  465,  698,    0,    0, 2433, 1480,    0,
    0,  144,    0,  706,  228,  117, 1495,    0,    0, 2776,
    0,    0,    0,    0,    0, 2760,  304,    0, 2515,    0,
 2763,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2095,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  560,    0,    0,    0,  875, 1404, 2787,    0,    0,
 2767,    0,    0,    0,    0, 2785,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2433,  306,    0,    0,    0,    0,    0,
    0,    0, 2771,    0,    0,    0, 3230,    0,    0,    0,
    0,    0,  117, 2793,    0,    0,    0,    0,    0,  929,
 2787,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2836,    0,    0, 2840,
    0,    0,    0,    0,    0,    0,    0,   11,    0, 2846,
    1,  294,   71, -510, 1726,  300, -146,  170, 2838, 2595,
    0,   73,    0, 2608,  -93,   46,    0,  817, -201,  -26,
  -43,   -4, -135, 2610, 2210,    0,    0,    0, 1512,    0,
    0,  -49, 1805, -527,    9, -742, 2261, 2042, -287, 1813,
 2780, -248,    0,  -57,    0, -677,  -51,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -551, -560, 1824, -216, 1116, -863,
    0,    0,    0,    0, -313, -107, -323, 1834, -311,  -45,
    0, -497,    0,    0,-1025, -949,    0, 1473,    0,    0,
 2571, -523,   26,    0,    0,    0,    0, 2545,    0, 2323,
 2581, -173, 1850, -699,    0,    0,    0, -704,    0,    0,
    0,    0,    0,    0, 1915,    0, 2607, 2160, 1847, 2585,
 2588,    0, 1303,    0, -493, 2148,  -48, -639, -700,    0,
    0,  -15, -899, -894, -880, -180, -870,    0, 1533, -927,
    0, 1233,-1154, 1538,    0, 2385,    0,    0,    0, 1237,
 1231, 1199,  124, -306, -631, -157,    0,    0, 2173,    0,
 1721,    0,    0, -483,    0,    0,    0,    0, -219,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -702,    0,    0, -659,    0,-1136, -917,    0,
    0, 1154, 1306,    0, 1210,    0,    0, 2570,  -94, 1965,
 1750, 1579,    0,    0,    0, -996, -838, -834,    0, -470,
 -290, -253,    0, -476, 1310,  302, -191, -475, -711, -753,
-1184,    0,    0,   -2,    0,    0,    0,    0,    0, 2628,
    0,    0,    0,    0, 2651, 2418,    0,    0,    0,    0,
    0, 2652,    0,    0,    0,    0,  -10,    0,    0,    0,
 -319, 2159, -317, 1728, -441,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2378,    0,    0, 2170,    0,    0,
    0, 2575, 2963,    0,    0,    0,    0,    0, 2703,    0,
    0,    0,    0, 2516,    0, 2514,    0,    0,    0, 2275,
    0,    0,    0,  201,    0,  267,    0, 2266,    0,    0,
    0, -618,-1019, -375, 1142,    0, 2871, 1468,    0, 2296,
 -619, 2298,    0, -812, 2056,    0,    0, -114, -663,    0,
 1410,-1113,    0,-1082, -924, -922,    0, 1526,    0, 1274,
    0, 1313,    0, 1224,    0,    0,    0,    0, 2533,    0,
    0,    0, -358, 2356, 2131, -968,    0,    0,    0, -660,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2854, 2215, 1930,    0, -562,    0,    0,    0,
 -215,    0, 2657,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   19, 1873,    0,   -9,
};
#define YYTABLESIZE 9501
static short yytable[] = {                                     110,
  171,   76,  110,  194,   75,  130,  131,  241,  137,  186,
   37,  191,  260,  263,  190,  288,  724,  549,  926,  586,
  378,  587,  521,  554,  225,  261,  264,  749,  710,  968,
  752,  978,  761, 1349,  229,  656,  763,  600,  943,  885,
  206,  170,  173,  176,  131,  181, 1129,  234,  895, 1210,
  193,  170,  197,  969,  176,  374,  131,  181, 1383,  210,
  110,  962,  247,  233,  249,  211,  927,  485,  221,  130,
  131,  224,  170,  201,  221,  197,  173,  176,  232,  131,
  181,  210,  110,  632,  176,  834,  465,  235, 1164,  967,
  294,  519, 1200,  736, 1022,  841,  231, 1201,  174, 1200,
  844,  193,  131,  193, 1201,  751,  964,  616,  388,  199,
  228, 1202,  723, 1047,  351,  197,  995,  388, 1202,  313,
  289, 1204,  176,  131,  210, 1002,  177,  625, 1204,  511,
  825, 1168,  230,  571,  736,  940, 1211,  736,  204,  236,
  238,  377,  632,  695,  340, 1021,  751, 1219,   74,  751,
  270,  888,  736,  917,  631,  378, 1313,  702,  362, 1027,
  944,  653,  257, 1460,  751,  697, 1240,  942,  625, 1658,
  259,  625,  486,  245,  217,  736,  708,  290, 1042,  812,
 1772,  971,  435,  727,  248,  653,  625,   90,  737,  381,
  374,  719,  571,  739,  740,  741,  742,  404,  435,  683,
  492, 1640,  695,  257,  751,  292,  257, 1005,  355, 1258,
  586,  529,  587,  631,  529,  217,  355,  362,  217,  471,
  373,  257, 1603, 1118,  697, 1721,  258,  702,  193, 1554,
  704,  951,  131,  217,  390,  110,  301,   74,  484,  751,
 1715,  131, 1477,  390,  554,  751,  809,  262,  751,  259,
  651,  474,  794,  663,  245, 1423,  404,   74,  683,  571,
  520,  173,  529,  751, 1419,  131, 1130,  523,  702,  191,
  259,  536, 1055, 1479,  539,  702,  377,  489,  860,  486,
  942, 1235,  550, 1381,  552,  302,  702,   86, 1009, 1398,
   86,  345,  472, 1257, 1317, 1222, 1220, 1453,  604,  193,
  608,   74,  713,  535,  368,  115,  193,  341,  193,   80,
  193,  116,  795,  193,  384,  598,  769,  129,  571,  259,
  297,  399,  344,  384,  860, 1183, 1468,  804,  325,  139,
 1759,  176,  181,   57,  702,  474, 1109, 1110,  314, 1200,
  131,   74,  173,  342, 1201,  484,  695,  478,  368,  742,
 1180,  472, 1527,   76, 1224,  373,  527,  736, 1202, 1351,
 1028,  257,  323,  368, 1393,  806,  269, 1121, 1204,  113,
 1218, 1760,  131,  714, 1548,  176,  390, 1632,  695, 1133,
 1135,  222,  140,  131,  489,  759,  596,  131,  126,  346,
  176, 1225,  131,  697,  474,  137,  805,  303, 1161,  196,
 1221, 1181,  257,  861,  735,  257,  298,  476,  742,  999,
 1252,  254,  480,  490,  736,  527,  315,  255, 1761,  695,
  257, 1558,  606, 1415,  632,  689, 1314,  102,  378,  529,
  650, 1480,  808, 1481,  697,  102, 1223,  695, 1472,  217,
  789, 1459, 1212, 1213,  529,  650,  625,  126, 1424,  861,
  253, 1381,  697, 1687,  647,  738,   90,  390,  196,  706,
 1750,  404,  653,  864,  253,  655,  718, 1197,  625,  661,
  529,  480,  664,  529,  384, 1680,  102,  217,   14, 1477,
  529,  765,  751, 1584,  751, 1365, 1720,  465,  529,  751,
  529, 1264,  751,  217,  697,  631,  686,  697, 1153, 1345,
 1263,  791, 1408,  963,  702,  253, 1345, 1491,  253, 1040,
 1479, 1524, 1280, 1525,  269,  217,  390,  388,  892,  758,
  193,  357,  465,  253, 1128,  758, 1707,  990, 1725,  834,
  992,  683, 1067,  216, 1396,  941, 1384,  686,  632, 1411,
  686, 1057, 1470,  594,  998,  465,  736,  897, 1552,  865,
  472, 1359,  819,  820,  736,  686,  191,  790,   76,  369,
  986,  102,   73,  496,  600, 1251,  986, 1758, 1747,  989,
  571, 1016,  652,  257,  216,  847,  847,  216,  847,  751,
  847,  102,  529,  751,  695,  102,  102,  811,  751, 1062,
  257, 1567,  216,  474,  702,  193,  866,  367,  695,  472,
  955,  102,  594,  369,  837,  751,  697,  751,  486,  631,
 1410,   76,  751,  362,  835,  702,  702,  707,  369,  695,
  697, 1599,  102,  600,  102, 1321, 1601, 1054,  863, 1694,
  625,  984,  985,  368, 1117,  193,  368,  529,  390,  257,
  683,  257,  474,  390,  614,  257,  751,  244,  681,  257,
  755,   73,  390,  102,  683,  176,  367, 1551,   86, 1430,
  197, 1488, 1561,  702,  257, 1192,  257,  136, 1480,  571,
 1481,   73, 1370, 1635,  484, 1472,  480,  217,  695,  191,
  325,  934, 1563, 1723,  381,  974,  974,  529,  614,  193,
 1639,  176, 1385,  695,  706,  899,  102,  254, 1454,  102,
 1124, 1672,  695,  529,  523,  465,  751,  131, 1702,  131,
  650,  472,  193,  489,  323,   73,  702,  742,  193,  816,
  934,  390,  697,  193,  527,  697,  136,  388,  614, 1253,
  529, 1111, 1008, 1010, 1114,  673,  529,  126,  254,  529,
  257,  254,  253,  193,  253,  368,  697, 1672,  196,  384,
  988, 1165,  253,  860,  474,   73,  254,  368, 1200,  368,
 1544,   94,  257, 1201,  125,  265,  695, 1057,  486,  257,
  867,  193,  193, 1686, 1418,  695,  378, 1202, 1550,  673,
 1209,  697, 1693,  378, 1015,  137,  388, 1204, 1007,  478,
  697,  961, 1217,  138,  673, 1397, 1025, 1755,  216, 1562,
 1167, 1407, 1588,  257, 1371,  324, 1490,  131,  384,  896,
  480, 1203, 1239,  847,  182,  686, 1125,  131, 1203,  847,
 1037, 1038, 1039, 1730, 1724, 1628, 1455, 1115,  686, 1064,
 1672,  889,  958, 1482,  484,   76,  216,  686,  696,  389,
  111,  529, 1113,  253,  137,  573,  682,  498,  529,  959,
  395, 1324,  216,  198, 1672,  390,  499,  390,  176,  181,
  390,  210,  110,  257,  599,  253,  529, 1116,  390,  126,
 1537, 1605,  253,  489,  216, 1710,  127,   73,  861,  696,
  390,  390,  396,  325,  686,  870,  594,  682,  369, 1041,
 1654,  369,  116,  686, 1713,  539,  529,  696,  389, 1052,
  529,  860,  102,  681,  573,  682,  253,  600,  112,  395,
  920,  921,  694, 1112, 1606,  683, 1306, 1673, 1308,  955,
  697,  594,  630,  599,  610,  392,  367,  218,  372,  367,
  116,  943, 1048,  391,  176,  539,  539,  686,  695,  539,
  216,  396,  600,  737, 1241,  136,  265,  136,  465,  594,
  474,  686,  594,  694,  539,  711,  539,  123,  634, 1671,
  695,  394, 1709,  619,  226,  116,  253, 1674,  594,  372,
  600,  694,  372,  600,  276,  254, 1607,  254, 1675,  697,
  569,  630, 1585,  610,  392,  254,  539,  372,  237,  600,
  266,  388,  391, 1242,  594,   96,  191, 1589, 1602, 1345,
  369,  979,  393,  381,  586, 1671,  587,  695,  384, 1392,
 1230,  238,  369,  384,  369,  600,  695,  634,   76,  384,
  394, 1231,  619,  378, 1230,  860,  861,  860, 1623, 1311,
 1311, 1315,   98,  683,  697,  193,  216,  697,  367,  569,
 1414, 1345,  193, 1489, 1246, 1773,  136, 1122, 1375, 1487,
  367,  475,  367,  507,  922, 1754, 1350,  673, 1203, 1134,
  695,  393, 1266,  376,  673,  137, 1653,  673, 1270, 1786,
  138,  673,  657,  382,  980, 1342,  254,  465,  864,  673,
  603,  864, 1676,  767, 1367, 1645, 1369, 1666, 1671, 1553,
 1669,  860,  683,  697, 1412,  673,  860,  783,  254,  102,
  223,   99,  697,  389,  508,  254,  476,  618,  390,  578,
  390,  100, 1671,  657,  395,  860,  657,  670, 1306,  673,
  670,   76,   76,  477,  767,  388,  390,  767,  242,  138,
  864,  657,  123,  123,  784,  670,  101,  465,  509,  254,
  974,  223,  767,  213,  223, 1413,  396,  384,  618,  673,
  861,  540,  861,  539,  673,  595,  510,  696,  242,  223,
 1336,  289,  176, 1688,  769,  682,  618,  696,  578,  539,
  696, 1337,  539,  539, 1339,  539,  673,  603,  682,  696,
  539,  214,  658,  539, 1156, 1157,  656,  682,  673,  392,
  673,  540,  540,  184,  185,  540,  539,  391,  517,  254,
  242, 1485,  539, 1559,  243,  539,  683,  599,  246,  659,
  540,  518,  540, 1732,  595,  695,  861,  575, 1422,  697,
  696,  861, 1426, 1736,  683,  394,  696,  656,  603,   76,
 1594,  694, 1395,  542,  682,  696, 1756,  389,  785,  697,
  861, 1474,  540,  682,  543,  656,  250,  466,  395,  697,
  372,  646,  697,  694,  645, 1595, 1764,  372, 1790,  486,
  372,  697,  683,  630,  372,  683,  393,  610, 1777, 1778,
  599,  683,  372,  599,  539,  251,  575,  534,  697,  696,
  396,  242,  619,  683,  351,  628,  306,  682,  372,  599,
 1241,  466, 1788,  696, 1650,  766,  539,  504, 1652,  634,
  694,  682,  610,  539,  619, 1795,  466,  539,  697,  694,
  390,  577,  372, 1596,  539,  599,  974,  697,  534, 1737,
 1738,  534,  242,  392,  191,  484,  628, 1492, 1158,  628,
  610,  391,  539,  610, 1638,  307,  534,  539,  534, 1245,
  411,  386,  372,  351,  628,  176,  504,  102,  376,  610,
  138,  252,  683,  694, 1692,  256, 1461, 1311,  102,  394,
 1464,  697,  539,  193,  489,  683,  539,  694,  534,  372,
  577,  287,  131,  437,  683,  610,  767,  462,  223, 1600,
  223,  372,  464,  372,  670,  670, 1636,  121,  223, 1594,
  953,  657, 1637,  953, 1078,  966, 1695,  486, 1696,  411,
  393,  657,  767,  619,  657,  767,  271, 1791,  953,  540,
  953,  493,  767,  657, 1595,  767,  681,  768,  296,  767,
 1079,  683,  265,  412,  767,  540,  618,  767,  540,  540,
  683,  540,  371,  408, 1780, 1604,  540,  370, 1734,  540,
  299, 1633,  378,  767,  619, 1765, 1529,  308,  618,   76,
 1691,  265,  540,  691, 1535,  259, 1586,  316,  540,  692,
  657,  540,  619,  484,  161,  309,  893,  767, 1530,  657,
  767,  767, 1596,  370,  683, 1531,  265, 1203, 1474,  223,
  767, 1587,  412,  170, 1634,  370, 1781,  670,  695,  693,
 1361,  694,  408,  696,  327,  901, 1691,  767,  595,  310,
 1362,  223,  489,  259, 1474,  656, 1610,  507,  223, 1532,
 1647, 1783,  257,  657, 1648,  656,  934, 1087,  656,  767,
  903,  131,  329,  161,  767,  657,  173,  656,  670,  553,
  540,  265,  338,  595, 1088,  534,  767, 1681,  767,  954,
 1691,  350,  223,  678,  130,  131,  348,  683,  684,  686,
 1474,  534,  540, 1173,  534,  534,  683,  534,  508,  540,
  349,  595,  534,  540,  595,  534,   44, 1174,  695,  693,
  540,  694, 1691,  696,  656,  905,  659,  498,  534, 1089,
  595,  934,  193,  656,  534,  272,  499,  534,  540,  352,
  659, 1175,  509,  540,   52,  176,  691,  330,  659,  658,
 1474,  356,  692, 1631,  628,  486,  595,  162, 1090,  489,
  510,   42,  683,  953,  659, 1590, 1716,   49,  540,  349,
  273,  683,  540,   25,  747,   44,  628,  656,   50,  130,
  131,   48,   50, 1474,  331,   51,  332,  486,  486,  656,
  658,  384,  973,  658,  385, 1176, 1697,  131,  274,  357,
 1663,  275,  384,   52,  953,  489,  534, 1474,  658,  333,
   52,  486,  482,  397,  170, 1718,  162,  276,  659,  128,
   42,  484,  393,   76,  486, 1091, 1719,  394,  534,  130,
  131,  453,   25,  747,  334,  534, 1697,  131,  361,  534,
   48,   50, 1770,  277,  398,  379,  534,  695,  693,   26,
  694,   49,  696,  484,  484,  489,  756,  400,  695,  693,
  489,  694,  402,  696,  534,  973,  403,  754,   51,  534,
  753,  482,  130,  131,  361,   53,  691,  484,  128,  354,
 1697,  131,  692,  756,  173,   46,  361,  360,  489,  843,
  484, 1715,  489,  489,  534,  362,   77,   78,  534,  697,
  695,  693,  410,  694,  379,  696, 1714,   54,   26,  173,
   49,   55,  489,  120,  121, 1064,  489, 1152,  695,  693,
 1162,  694,  404,  696,  129,  506,  251,   51,  117,  489,
  470,  362,  173,  464,  953,  473,  900,  695,  693,   47,
  694,  916,  696,  362,   46,  482,  923,  924,  700,  695,
  693,  370,  694, 1742,  696,  931,  691,  935,  936,  937,
  938,  939,  692,  931,  472,  370,  506,  251,  464,  506,
  251,  217,  524,  370,  949,  953,  953,  953,  953,  953,
  966,  953,  713,  129,  506,  251,  152,  117, 1284,  370,
 1743,  464,   14,  953,  953,  953,  953,  493,   47,  497,
  363,  364,  365, 1744,  184,  185,  416,  695,  693,  776,
  694,  777,  696,  524,  217,  867,  524,  865,  862,  713,
  865,  644,  418,  366,  367,  953,  506,  953,  368,  369,
  217,  524,  713,  524,  184,  185,  363,  364,  365,  514,
  184,  185, 1173,  370,  884,  152,  527,  883,  363,  364,
  365, 1046,  184,  185, 1045,  217, 1174,  534,  217,  366,
  367,  420,  421,  422,  368,  369,  423,  424,  658,  865,
  537,  366,  367,  217,  426,  427,  368,  369,  658,  547,
 1175,  658,  132,  431, 1305,  691, 1307, 1766, 1767,  868,
  658,  692, 1053,  681, 1173,  830,  691,  928,  695,  693,
   95,  694,  692,  696, 1782,  551, 1166,  601, 1174, 1165,
 1068, 1069, 1073,  605,  133, 1081, 1083, 1085,  607, 1093,
 1096, 1099, 1101, 1104, 1107,  869,  701,  609, 1439,  134,
  870, 1065, 1175, 1066, 1176,  639,  871,  658,  691,  640,
  318, 1796,   96,  319,  692,  641,  658,  102,  363,  364,
  365,  464, 1017, 1018,   97,  872,  691, 1275, 1137, 1138,
  644, 1140,  692, 1142,  124,  124, 1145, 1146, 1147, 1148,
 1149, 1150,  648,  662,  834,  691,  368,  369,  653,   98,
  665,  692,  667, 1154,  668,  924, 1176,  691,  529,  320,
  658, 1184, 1191,  692, 1045, 1190,  953, 1233,  953,  953,
 1232, 1238,  658,  506, 1237,  506,  953,  699, 1319, 1432,
  506, 1318, 1165,  506,  953,  834,  834,  834,  834,  834,
  953,  834, 1467,  953,  669, 1466,  506, 1438, 1469,  529,
  524, 1466,  529,  834,  834,  834,  834,  953, 1771,  670,
  671, 1466,  953,  953,  251,  691,  953,  529,   99,  524,
  524,  692,  524,  953,  672,  953,  953,  524,  100,  953,
  524, 1794,  673,  953, 1715,  152,  953,  834,  953,  674,
  953,  675,  676,  524,  681,  953,  953,  682,  953,  524,
  690,  698,  701,  101,  894,  251,  898,  152,  251,  902,
  904,  906,  703,  717,  953,  953,  721,  953,  722,  726,
  728,  102,  729,  251,  506,  732,  733,  498,  743,  744,
  746,  953,  747,  953,  953,  953,  152,  527,  953,  756,
  780,  953,  953,  781,  782,  787,  506,  792,  257,  796,
  798,  953,  797,  506,  799,  152,  691,  801,  802,  813,
  931,  196,  692,  814,  152,  953,  807,  817,  953,  818,
  152,  524, 1287, 1288,  827, 1290, 1292,  829,  527,  830,
  496,  527,  839, 1297,  953,  838,  529,  506,  152,  840,
  953,  842,  878,  524,  879,  953,  527,  881,  527,  886,
  524,  890,  891,  918,  929,  950,  953,  953,  965,  953,
  956,  524,  972,  991,  464,  953,  808,  986,  997,  996,
  196,  496,  999,  759,  496, 1006, 1012,  529, 1328,  524,
  529, 1144,  695,  693,  524,  694, 1335,  696,  924,  496,
 1003,  924, 1004, 1154, 1013,  529, 1014, 1343,  924, 1346,
 1347, 1020, 1023, 1029, 1036,  924,  834,  953,  834,  834,
 1034, 1043, 1048,  524, 1060, 1061,  834,  658, 1123, 1322,
  695,  693, 1075,  694,  834,  696, 1126, 1120, 1118, 1136,
  834, 1141, 1139,  834,  695,  693,  529,  694,  529,  696,
 1143, 1163,  691,  529, 1377, 1151,  529,  834, 1170, 1172,
  645, 1704,  834,  834,  846, 1186,  834, 1178, 1196,  529,
 1188, 1193, 1214,  834, 1205,  834,  834, 1216, 1243,  834,
 1226, 1234, 1236,  834, 1247, 1248,  834, 1249,  834, 1250,
  834, 1261, 1259, 1268, 1272,  834,  834, 1054,  834, 1273,
 1271, 1274,  251,  464,  251,  846,  846,  846,  846,  846,
 1276,  846,  251, 1277,  834,  834, 1281,  834, 1282, 1283,
 1427, 1285, 1286,  846,  846,  846,  846, 1293, 1294, 1435,
 1295,  834, 1296,  834,  834,  834, 1440, 1298,  834, 1299,
 1300,  834,  834,  695,  693, 1667,  694,  529,  696, 1301,
 1302,  834,  230, 1303,  860,  527, 1320,  846, 1323,  695,
  693, 1304,  694,  464,  696,  834, 1341, 1348,  834,  529,
 1352, 1338, 1356, 1363,  527,  527,  529,  527, 1353, 1354,
 1355, 1357,  527,  924,  834,  527, 1366,  924, 1368, 1372,
  834, 1374,  529,  230, 1382,  834,  230, 1386,  527, 1394,
  196,  849, 1387,  251,  527, 1409,  834,  834,  231,  834,
  529,  230, 1416, 1388, 1389,  834,  695,  693,  496,  694,
  496,  696,  196, 1420,  529,  251,  529, 1421,  496, 1390,
  691, 1399,  251, 1708,  529, 1391,  692, 1425, 1431, 1278,
 1451,  496,  849,  849,  849,  849,  849,  529,  849,  231,
 1452,  196,  231, 1456, 1462, 1463, 1483,  834, 1526, 1541,
  849,  849,  849,  849, 1536,  153,  251,  231,  691, 1539,
  196, 1542, 1543, 1538,  692, 1545,  527, 1540,  196,  196,
 1546, 1547,  691, 1572, 1573,  196, 1555, 1557,  692, 1326,
  695,  693, 1565,  694,  849,  696, 1560, 1564,  527, 1566,
 1568, 1569, 1570,  196, 1571,  527, 1591, 1574,  924, 1575,
 1576,  924, 1577, 1578,  527,  196,  527,  196, 1579,  496,
 1580, 1581, 1582, 1583,  153,  529,  846, 1593,  846,  846,
  695,  693, 1592,  694,  527,  696,  846, 1609, 1611,  527,
  196,  496,  112, 1613, 1612, 1614, 1615,  529,  496, 1619,
  846, 1616, 1617,  846,  529, 1618, 1620, 1327,  695,  693,
 1621,  694, 1626,  696, 1627,  196, 1629,  846,  527, 1630,
 1651, 1657,  846,  846,  850, 1665,  846, 1690, 1656, 1622,
 1682,  691,  496,  846, 1703,  846,  846,  692,  529,  846,
 1731, 1712, 1733,  846, 1726, 1728,  846,  691,  846, 1735,
  846, 1740, 1748,  692, 1646,  846,  846, 1741,  846, 1466,
 1753, 1752, 1649, 1723, 1724,  850,  850,  850,  850,  850,
  139,  850, 1769, 1774,  846,  846, 1775,  846, 1776, 1715,
  242, 1779,  230,  850,  850,  850,  850, 1789, 1792, 1793,
    1,  846,   95,  846,  846,  846,  625,   30,  846, 1662,
   32,  846,  846, 1664,  691,  203,  751,  376,  567,  271,
  692,  846,  149,  849,  527,  849,  849,  850, 1329,  695,
  693,  649,  694,  849,  696,  846,  695,  693,  846,  694,
  269,  696,  936,  463,   96,  213,  242,   82,  231,  276,
  849, 1330,  695,  693,  846,  694,   97,  696,  527,  193,
  846,  404,  527,  935,  849,  846,  681,  679,  924,  849,
  849,  844, 1711,  849,  675,  677,  846,  846,  194,  846,
  849,   98,  849,  849,  275,  846,  849,  377,  691,  281,
  849,  242,  289,  849,  692,  849,  708,  849,  287,  707,
  223,  320,  849,  849,  153,  849,  709,  288,  328,  338,
 1746,  319,  844,  242,  844,  844,  844,  619,  670,  326,
  242,  849,  849,  718,  849,   26,  153,  846,  691,   29,
  844,  844,  844,  844,  692,   39, 1309,  401,  849,  117,
  849,  849,  849,  392,  391,  849,  181,  242,  849,  849,
   99, 1269,  880,  227,  242,  153,  691, 1267,  849,  828,
  100, 1059,  692, 1549,  844, 1265,  516, 1255,  793,  242,
  560,  502,  849, 1244,  153,  849,  242,  966, 1331,  695,
  693, 1256,  694,  153,  696,  101, 1195,  181,  491,  153,
  181,  849,  531,   95, 1659,  532,  850,  849,  850,  850,
 1001, 1528,  849, 1722, 1727,  181,  850,  153, 1534,  775,
  242, 1729, 1763,  849,  849,  994,  849, 1376, 1787, 1679,
 1757, 1185,  849,  850, 1360, 1332,  695,  693, 1486,  694,
  643,  696, 1333,  695,  693,   96,  694,  850,  696, 1689,
  540,  843,  850,  850,  779,  513,  850,   97, 1334,  695,
  693,  522,  694,  850,  696,  850,  850, 1035, 1406,  850,
 1026,  114,  815,  850,  849,  119,  850,  691,  850,  666,
  850,  483,   98,  692,  730,  850,  850,  712,  850,  716,
  692,  957,  843,  269,  843,  843,  843,  970,  952, 1169,
  691,  953, 1655,  177,  850,  850,  692,  850, 1597, 1739,
  843,  843,  843,  843, 1768, 1797,  887, 1132, 1289,  725,
    0,  850,    0,  850,  850,  850,  642,    0,  850,    0,
 1340,  850,  850,  907,  908,  909,  910,  911,  912,  913,
  914,  850,    0,  844,  843,  844,  844,    0,    0,    0,
    0,   99,    0,  844,    0,  850,    0,    0,  850,    0,
    0,  100,  177,    0, 1082,    0, 1086,  187, 1094, 1097,
  844, 1102, 1105, 1108,  850,    0,    0,    0,    0,    0,
  850,    0,    0,    0,  844,  850,  101,    0,    0,  844,
  844,    0,    0,  844,    0,    0,  850,  850,    0,  850,
  844,    0,  844,  844,  102,  850,  844,    0,    0,    0,
  844,  845,    0,  844,  181,  844,    0,  844,    0,    0,
    0,    0,  844,  844,    0,  844,  187,  691,    0,    0,
    0,    0,    0,  692,    0,  181,    0,    0,    0,    0,
    0,  844,  844,    0,  844,    0,    0,  850,    0,    0,
    0,    0,  845,    0,  845,  845,  845,  181,  844,    0,
  844,  844,  844,  181,    0,  844,    0,    0,  844,  844,
  845,  845,  845,  845,  691,    0,    0,    0,  844,    0,
  692,  691,    0,    0,    0,    0,  181,  692,    0,    0,
    0,    0,  844,    0,    0,  844,    0,  691,    0,    0,
    0,    0,    0,  692,  845,  181,    0,    0,    0,    0,
    0,  844,    0,  843,  181,  843,  843,  844,    0,    0,
  181,    0,  844,  843, 1457,  695,  693,    0,  694,    0,
  696,    0,    0,  844,  844,    0,  844,    0,  181,    0,
  843,    0,  844,    0, 1458,  695,  693,    0,  694,    0,
  696,    0,    0,    0,  843,    0,    0,    0,    0,  843,
  843,  177,  848,  843,    0, 1465,  695,  693,    0,  694,
  843,  696,  843,  843,    0,  181,  843,    0,    0,    0,
  843,    0,  177,  843,  844,  843,    0,  843,    0,    0,
    0,    0,  843,  843,    0,  843,    0,    0,    0,    0,
    0,    0,    0,  848,  177,  848,  848,  848,    0,    0,
  177,  843,  843,    0,  843,    0,    0,    0,    0,    0,
    0,  848,  848,  848,  848,  187,    0,    0,  843,    0,
  843,  843,  843,  177,    0,  843,    0,    0,  843,  843,
    0,    0,    0,    0,    0,    0,  187,    0,  843,    0,
    0,    0,  177,    0,    0,  848,    0,    0,    0,    0,
    0,  177,  843,    0,    0,  843,    0,  177,  187,    0,
    0,    0,    0,  845,  187,  845,  845,    0,    0,    0,
    0,  843,    0,  845,    0,  177,    0,  843,    0,    0,
    0,    0,  843,    0,    0,    0,    0,  187,    0,    0,
  845,    0,    0,  843,  843,    0,  843,    0,    0,    0,
    0,    0,  843,    0,  845,    0,  187,    0,    0,  845,
  845,  624,  177,  845,  432,  187,  433,    0,  196,    0,
  845,  187,  845,  845,  666,    0,  845,    0,    0,  435,
  845,    0,    0,  845,  259,  845,    0,  845,    0,  187,
    0,    0,  845,  845,  843,  845,    0,    0,    0,    0,
    0,    0,    0,  691,    0,    0,    0,    0,    0,  692,
    0,  845,  845,    0,  845,  666,    0,    0,  666,    0,
    0,    0,    0,  691,    0,    0,  187,  196,  845,  692,
  845,  845,  845,  666,    0,  845,    0,    0,  845,  845,
  317,    0,    0,    0,  691,    0,    0,    0,  845,  412,
  692,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  845,    0,  848,  845,  848,  848,    0,    0,
    0,  317,    0,    0,  848,    0,    0,    0,  317,    0,
    0,  845,  317,  317,    0,  317,    0,  845,  317,  557,
    0,  848,  845,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  317,  845,  845,  848,  845,    0,  317,    0,
  848,  848,  845,    0,  848,    0,    0,    0,    0,    0,
    0,  848,    0,  848,  848,    0,    0,  848,    0,    0,
  557,  848,    0,  557,  848,    0,  848,    0,  848,    0,
    0,    0,    0,  848,  848,    0,  848,    0,  557,    0,
  557,    0,    0,   73,  845,  102,    0,    0,    0,    0,
    0,    0,  848,  848,    0,  848,  419,    0,  317,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  848,
  557,  848,  848,  848,    0,  425,  848,  317,  406,  848,
  848,    0,    0,  428,  429,  430,    0,  407,    0,  848,
    0,  624,  317,    0,  432,    0,  433,    0,  188,    0,
    0,  408,  317,  848,  617,    0,  848,  361,  409,  435,
    0,  317,    0,  317,  259,    0,  196,    0,    0,    0,
  317,    0,  848,    0,    0,    0,  618,    0,  848,    0,
    0,    0,    0,  848,    0,  410,    0,  196,  411,    0,
    0,    0,    0,    0,  848,  848,    0,  848,    0,  317,
    0,    0,    0,  848,    0,    0,    0,  188,    0,  196,
    0,    0,    0,    0,    0,  196,  412,  413,  317,    0,
    0,    0,    0,    0,    0,    0,  624,    0,  414,  432,
    0,  433,  415,  666,    0,    0,  666,    0,  196,    0,
  619,    0,    0,  666,  435,  848,  666,    0,    0,  259,
  666,    0,    0,    0,    0,  666,    0,  196,  666,    0,
    0,    0,    0,    0,    0,  196,  196,    0,    0,    0,
    0,    0,  196,    0,  666,    0,    0,    0,    0,    0,
    0,    0,    0,  620,    0,    0,    0,  557,    0,  416,
  196,    0,    0,    0,    0,  417,    0,    0,  666,    0,
    0,    0,  666,  557,    0,  418,    0,  557,    0,  557,
    0,  666,    0,    0,  557,    0,    0,  557,    0,  363,
  364,  365,  102,  184,  185,    0,    0,  196,  666,    0,
  557,    0,    0,  419,    0,    0,  557,    0,  196,  557,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,  666,    0,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,  666,  406,    0,
    0,  621,  622,  623,    0,  666,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  408,    0,    0,    0,    0,    0,  361,  409,    0,
   59,    0,    0,    0,    0,    0,  188,    0,  557,    0,
    0,   60,    0,    0,    0,    0,  618,    0,   61,    0,
    0,    0,    0,    0,    0,  410,  547,  188,  411,    0,
  557,   62,    0,    0,    0,    0,    0,  557,    0,    0,
    0,  557,    0,    0,    0,    0,    0,    0,  557,  188,
    0,    0,    0,  406,    0,  188,  412,  413,    0,    0,
    0,  531,  407,    0,    0,    0,  557,  547,  414,    0,
  547,  557,  415,    0,    0,    0,  408,    0,  188,    0,
  619,    0,  361,  409,    0,  547,    0,  547,    0,    0,
    0,    0,   63,    0,    0,    0,  557,  188,    0,    0,
  557,  618,  531,    0,    0,  531,  188,  529,   64,    0,
  410,    0,  188,  411,    0,    0,    0,  547,    0,  685,
  531,    0,  531,  620,    0,   65,    0,    0,    0,  416,
  188,    0,    0,    0,    0,  417,    0,    0,    0,    0,
   66,  412,  413,    0,   67,  418,    0,   68,  529,    0,
    0,  529,  531,  414,    0,   69,    0,  415,    0,  363,
  364,  365,  102,  184,  185,  619,  529,  188,  529,    0,
    0,    0,    0,  419,   70,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,    0,  620, 1428,
 1429,  621,  622,  623,  416, 1433, 1434,    0, 1436, 1437,
  417,    0,    0,    0,    0,    0, 1441, 1442, 1443, 1444,
  418, 1445, 1446, 1447, 1448, 1449, 1450,    0,  640,    0,
  640,  640,    0,  640,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,  640,    0,  419,    0,
    0,  640,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,  547,  428,  429,  430,    0,    0,
    0,  431,    0,    0,    0,    0,  621,  622,  623,    0,
  547,    0,    0,    0,  547,    0,  547,    0,    0,    0,
    0,  547,    0,    0,  547,    0,    0,    0,    0,  531,
  638,    0,  638,  638,    0,  638,    0,  547,    0,    0,
    0,    0,    0,  547,    0,  531,  547,    0,  638,  531,
    0,  531,    0,  638,    0,    0,  531,    0,    0,  531,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  531,    0,    0,  529,    0,    0,  531,    0,
    0,  531,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  529,    0,  529,    0,    0,
    0,    0,  529,    0,    0,  529,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  547,    0,    0,  529,    0,
    0,    0,    0,    0,  529,    0,    0,   81,  434,    0,
    0,  432,    0,  433,   82,    0,    0,  547,   83,   84,
    0,    0,    0,    0,  547,    0,  435,   85,  547,    0,
  531,  259,    0,    0,    0,  547,    0,    0,    0,   86,
    0,    0,    0,    0,    0,    0,    0,    0,  640,    0,
    0,    0,  531,  547,    0,  640,    0,    0,  547,  531,
    0,    0,    0,  531,  640,    0,    0,    0,    0,    0,
  531,    0,    0,    0,    0,    0,  529,    0,  640,    0,
    0,    0,    0,  547,  640,  640,    0,  547,  531,    0,
    0,    0,  640,  531,    0,    0,    0,    0,  529,    0,
    0,    0,    0,    0,   87,  529,    0,    0,    0,    0,
    0,    0,  640,    0,    0,  640,  529,    0,  531,    0,
    0,   88,  531,    0,    0,    0,    0,    0,    0,    0,
  638,    0,    0,    0,  529,    0,   89,  638,    0,  529,
   90,  640,    0,  640,  640,    0,  638,    0,    0,    0,
    0,   91,    0,    0,    0,  640,    0,    0,    0,  640,
  638,    0,    0,    0,    0,    0,  638,  638,  529,    0,
   92,    0,    0,    0,  638,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  434,    0,    0,
  432,    0,  433,    0,  638,    0,    0,  638,    0,    0,
    0,    0,    0,    0,    0,  435,    0,    0,    0,    0,
  259,    0,    0,    0,    0,    0,  640,    0,    0,    0,
    0,    0,  640,  638,    0,  638,  638,    0, 1070,    0,
    0, 1071,  640,    0,    0,  406,    0,  638,    0,    0,
    0,  638,    0,    0,  407,    0,  640,  640,  640,  640,
  640,  640,    0,    0,    0,    0,    0,    0,  408,    0,
  640,    0,    0,  434,  361,  409,  432,    0,  433,  640,
  640,  640,  640,  640,  640,  640,  640,  640,    0,  640,
    0,  435,    0,    0,  640,  640,  259,  640,  640,  640,
    0,    0,  410,  640,    0,  411,    0,    0,  638,    0,
    0,    0,    0,    0,  638,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  638,    0,    0,    0,    0,    0,
    0,    0,    0,  412,  413,    0,    0,    0,  638,  638,
  638,  638,  638,  638,    0,  414,    0,    0,    0,  415,
    0,    0,  638,    0,    0,    0,    0,    0,    0,    0,
    0,  638,  638,  638,  638,  638,  638,  638,  638,  638,
    0,  638,    0,    0,    0,    0,  638,  638,    0,  638,
  638,  638,    0,    0,    0,  638,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1072,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0, 1080,    0,    0,
 1071,    0,  418,    0,  406,    0,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,    0,  408,    0,    0,
  419,    0,    0,  361,  409,    0,    0,    0,  664,  366,
  367,  420,  421,  422,  368,  369,  423,  424,    0,  425,
    0,    0,    0,    0,  426,  427,    0,  428,  429,  430,
    0,  410,    0,  431,  411,    0,    0,    0,    0,    0,
    0,    0,    0, 1084,    0,    0, 1071,    0,    0,  664,
  406,    0,  664,    0,    0,    0,    0,    0,    0,  407,
    0,    0,  412,  413,    0,    0,  434,  664,    0,  432,
    0,  433,    0,  408,  414,    0,    0,    0,  415,  361,
  409,    0,    0,    0,  435,    0,    0,    0,    0,  259,
    0,    0,    0,    0,  928,  695,  693,    0,  694,    0,
  696,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,    0,  858,  857,  859,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1072,    0,  416,    0,    0,  412,  413,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
  414,  418,    0,    0,  415,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,  695,  693,    0,  694,    0,  696,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,  858,
  857,  859,    0,  426,  427,    0,  428,  429,  430, 1072,
    0,  416,  431,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0,    0,  426,
  427,    0,  428,  429,  430,    0, 1092,    0,  431, 1071,
    0,    0,    0,  406,    0,    0,    0,    0,    0,    0,
    0,  434,  407,    0,  432,    0,  433,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  408,    0,    0,  435,
    0,    0,  361,  409,  259,  845,    0,  664,    0,    0,
  664,    0,    0,  691,    0,    0,    0,  664,    0,  692,
  664,    0,  846,    0,  664,    0,    0,    0,    0,  664,
  410,    0,  664,  411,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  664,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  847,    0,
    0,  412,  413,  434,    0,    0,  432,  848,  433,    0,
    0,    0,  664,  414,    0,  849,  664,  415,    0,    0,
    0,  435,    0,    0,    0,  664,  259,    0,    0,    0,
    0,  845,    0,    0,  850,    0,  851,    0,    0,  691,
    0,    0,  664,    0,    0,  692,    1,    0,  846,    0,
  852,    0,  853,  854,  855,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    2,  664,
    0,    0, 1072,    0,  416,    0,    3,    0,    0,    0,
  417,  664,    0,    4,  847,    5,    0,    6,    0,    0,
  418,  831,    7,  848,    0,  562,    0,    0,    0,    8,
    0,  849,    0,  856,  363,  364,  365,  102,  184,  185,
  832,    0,    0,    9,  563,    0,    0,  564,  419,    0,
  850,    0,  851,    0,   10,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,  852,  425,  853,  854,
  855, 1095,  426,  427, 1071,  428,  429,  430,  406,    0,
    0,  431,    0,    0,    0,    0,  434,  407,  565,  432,
    0,  433,    0,    0,  566,  567,  568,    0,  569,  570,
  571,  408,  572,    0,  435,    0,    0,  361,  409,  259,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  856,
    0,   11,  573,    0,  574,    0,    0,    0,   12,    0,
    0,   13,    0,   14,   15,  410,    0,    0,  411,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1100,    0,  575, 1071,    0,   16,    0,
  406,    0,    0,    0,    0,    0,  412,  413,  434,  407,
    0,  432,  576,  433,    0,    0,    0,    0,  414,    0,
    0,    0,  415,  408,    0,    0,  435,    0,    0,  361,
  409,  259,    0,    0,  577,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   17,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,   18,    0,    0,    0,    0,    0,    0,  578,
  579,    0,    0,    0,    0,    0,    0, 1072,    0,  416,
    0,    0,    0,    0,    0,  417,  580,    0,  412,  413,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,    0,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0, 1103,  426,  427, 1071,
  428,  429,  430,  406,    0,    0,  431,    0,    0, 1072,
    0,  416,  407,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,  408,  418,    0,    0,
    0,    0,  361,  409,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
  410,    0,    0,  411,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0, 1106,  426,
  427, 1071,  428,  429,  430,  406,    0,    0,  431,    0,
    0,  412,  413,  434,  407,  463,  432,    0,  433,    0,
    0,    0,    0,  414,    0,    0,    0,  415,  408,    0,
    0,  435,    0,    0,  361,  409,  259,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,  411,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1072,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,  412,  413,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,  414,    0,    0,    0,  415,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0, 1072,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,    0,  425,
    0,    0,    0,    0,  426,  427,    0,  428,  429,  430,
  406,  434,    0,  431,  432,    0,  433, 1374,    0,  407,
    0,    0,  661,    0,    0,    0,    0,    0,    0,  435,
    0,    0,    0,  408,  259,    0,    0,    0,    0,  361,
  409,    0,    0,    0,    0,    0,    0,    0,    6,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1493,
 1494,    0, 1495,  661,    0, 1496,  661,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,    0, 1497,
    0,  661,    0,    0,    0, 1498,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  362,    0,  412,  413,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1374,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  947,    0,  947,  947,    0,  947,
    0,    0,    0,    0,    0, 1499,    0,    0,    0,    0,
    0,    0,  947,    0,    0,    6,    0,  947,    0,    0,
    0,    0,    0,    0,   14,    0, 1493, 1494,    0, 1495,
    0,  416, 1496,    0,    0,    0,    0,  417,    0, 1500,
    0,    0,    0,    0,    0,    0, 1497,  418,    0,   16,
    0,    0, 1498,    0,    0,    0,    0,    0, 1622,    0,
 1501,  363,  364,  365,  102,  184,  185,  102,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0, 1502,    0,  406,  426,
  427,    0,  428,  429,  430,    0,    0,  407,  431,    0,
    0,    0, 1499,    0,    0, 1503,    0,  640,    0,    0,
  640,  408,  640,    0,    0,    0,    0,  361,  409,    0,
    0,   14,    0,    0,    0,  640,    0,    0,    0,    0,
  640,    0,    0,    0,    0,    0, 1500,    0,    0,    0,
    0,    0,    0,    0,    0,  410,   16,    0,  411,    0,
    0,    0,    0,    0,    0,    0,    0, 1501,    0,    0,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  362,    0,  412,  413,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  414,    0,
    0,  661,  415, 1502,  661,    0,    0,    0,    0,    0,
    0,  661,    0,    0,  661,    0,    0,    0,  661,    0,
    0,  947, 1503,  661,    0,    0,  661,    0,    0,    0,
  947,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  434,    0,  661,  432,  947,  433,    0,    0,    0,  685,
  947,  947,    0,    0,    0,    0,    0,    0,  435,  416,
    0,    0,    0,  259,    0,  417,  661,    0,    0,    0,
  661,    0,    0,    0,    0,  418,    0,    0,  947,  661,
    0,  947,    0,    0,    0,    0,    0,    0,    0,  363,
  364,  365,  102,  184,  185,    0,  661,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,  947,    0,  947,
  947,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,  947,  425,  661,    0,  947,    0,  426,  427,    0,
  428,  429,  430,    0,    0,  661,  431,  640,    0,    0,
    0,    0,    0,    0,  640,    0,    0,    0,    0,    0,
    0,    0,    0,  640,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  640,    0,    0,
    0,    0,    0,  640,  640,    0,    0,    0,    0,    0,
    0,  640,  947,    0,    0,    0,    0,    0,  947,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  947,    0,
    0,  640,    0,    0,  640,    0,    0,    0,    0,    0,
    0,    0,  947,  947,  947,  947,  947,  947,    0,    0,
    0,    0,    0,    0,    0,    0,  947,    0,    0,    0,
    0,    0,  640,  640,    0,  947,  947,  947,  947,  947,
  947,  947,  947,  947,  640,  947,    0,    0,  640,    0,
  947,  947,    0,  947,  947,  947,    0,    0,    0,  947,
    0,    0,    0,    0,    0,    0,  434,  406,    0,  432,
    0,  433,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,    0,  259,
  408,    0,    0,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,  640,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    0,    0,  410,    0,    0,  411,    0,    0,
    0,    0,    0,    0,    0,  640,  640,  640,  640,  640,
  640,    0,    0,    0,    0,    0,    0,    0,    0,  640,
    0,    0,    0,    0,    0,  412,  413,    0,  640,  640,
  640,  640,  640,  640,  640,  640,  640,  414,  640,    0,
    0,  415,    0,  640,  640,    0,  640,  640,  640,    0,
    0,    0,  640,    0,    0,    0,    0,    0,    0,  434,
    0,    0,  432,    0,  433,    0,    0,    0,    0,    0,
    0,    0,    0,  459,    0,    0,    0,  435,    0,    0,
    0,    0,  259,  459,  459,  459,  459,  459,  459,  459,
  459,  459,  459,  459,  459,  459,  459,  459,  416,    0,
  459,  459,  459,  459,  417,  459,    0,    0,  459,  459,
    0,    0,    0,    0,  418,    0,    0,    0,    0,    0,
    0,    0,  677,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,  406,    0,    0,  426,  427,    0,  428,
  429,  430,  407,    0,    0,  431,    0,    0,    0,    0,
    0,    0,  434,  930,    0,  432,  408,  433,    0,    0,
    0,    0,  361,  409,    0,    0,    0,    0,  221,    0,
  435,    0,  221,    0,    0,  259,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,  143,    0,
  410,  221,    0,  411,  221,    0,    0,  144,  145,  146,
  147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
  157,  158,  159,    0,    0,  160,  161,  162,  163,    0,
  164,  412,  413,  165,  166,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,  221,    0,  415,    0,    0,
    0,  221,  221,  221,    0,  221,  221,  221,    0,  221,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,  407,    0,    0,    0,  221,
    0,  221,    0,    0,    0,  434,    0,    0,  432,  408,
  433,    0,    0,    0,  685,  361,  409,    0,    0,    0,
    0,    0,    0,  435,  416,    0,    0,    0,  259,    0,
  417,    0,  221,    0,    0,    0,    0,   81,    0,    0,
  418,    0,    0,  410,    0,    0,  411,    0,    0,  221,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,  221,  362,    0,  412,  413,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,  414,  425,    0,    0,
  415,    0,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0,    0,    0,  221,  221,    0,  406,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,  221,    0,    0,    0,    0,  434,    0,
    0,  432,  408,  433,    0,    0,    0,    0,  361,  409,
    0,    0,    0,    0,    0,    0,  435,  416,    0,    0,
    0,  259,    0,  417,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,  410,    0,    0,  411,
    0,    0,    0,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,  412,  413,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,  414,
  425,    0,    0,  415,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0,    0,    0,    0,    0,
    0,    0,  406,    0,    0,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  434,    0,    0,  432,  408,  433,    0,    0,    0,
    0,  361,  409,    0,    0,    0,    0,    0,    0,  435,
  416,    0,    0,    0,  259,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,    0,    0,  410,
    0,    0,  411,    0,    0,    0,    0,    0,    0,    0,
  363,  364,  365,  102,  184,  185,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
  412,  413,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,  414,  425,    0,    0,  415,    0,  426,  427,
    0,  428,  429,  430,    0,    0,    0,  431,    0,    0,
    0,    0,    0,    0,    0,  406,    0,    0,    0,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  408,    0,
    0,    0,    0,    0,  361,  409,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,  410,    0,    0,  411,    0,    0,    0,    0,
 1098,    0,    0,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
    0,    0,    0,  412,  413,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,  414,  425,    0,    0,  415,
    0,  426,  427,    0,  428,  429,  430,    0,    0,    0,
  431,    0,    0,    0,    0,    0,    0,  434,  406,    0,
  432,    0,  433,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,  435, 1661,    0,    0,    0,
  259,  408,    0,    0,    0,    0, 1278,  361,  409,    0,
    0,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  418,    0,    0,  410,    0,    0,  411,    0,
    0,    0,    0,    0,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,  412,  413,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,  414,  425,
    0,    0,  415,    0,  426,  427,  561,  428,  429,  430,
  562,    0,    0,  431,    0,    0,    0,    0,  759,    0,
 1705,    0,    0,  432,    0,  433,    0,    0,    0,  563,
    0,    0,  564,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,    0,    0,  565,    0,  418,    0,    0,    0,  566,
  567,  568,    0,  569,  570,  571, 1291,  572,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,  573,    0,  574,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,  406,    0,    0,  426,  427,    0,
  428,  429,  430,  407,    0,    0,  431,    0,    0,    0,
  575,    0,    0,  434,    0,    0,  432,  408,  433,    0,
    0,    0,    0,  361,  409,    0,    0,  576,    0,  221,
    0,  435,    0,  221,    0,    0,  259,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  221,  577,
    0,  410,  221,    0,  411,  221,    0,    0,    0,    0,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  412,  413,  578,  579,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,  221,    0,  415,    0,
    0,  580,  221,  221,  221,    0,  221,  221,  221,    0,
  221,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,    0,
  221,    0,  221,    0,    0,    0,  947,    0,    0,  947,
  408,  947,    0,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,    0,  947,  416,    0,    0,    0,  947,
    0,  417,    0,  221,    0,    0,    0,    0,    0,    0,
    0,  418,    0,    0,  410,    0,    0,  411,    0,    0,
  221,    0,    0,    0,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,  221,  362,    0,  412,  413,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,  414,  425,    0,
    0,  415,    0,  426,  427,    0,  428,  429,  430,    0,
    0,  816,  431,    0,    0,    0,    0,  221,  221,    0,
  406,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,    0,    0,  221,    0,    0,    0,    0,    0,
    0,    0,    0,  408,    0,    0,    0,    0,    0,  361,
  409,    0,  816,    0,    0,  816,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
  816,    0,    0,    0,  418,    0,    0,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,  412,  413,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
  414,  425,    0,    0,  415,    0,  426,  427,    0,  428,
  429,  430,    0,    0,    0,  431,    0,    0,    0,  785,
    0,    0,    0,  947,    0,    0,    0,    0,    0,    0,
  786,    0,  947,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  947,    0,    0,    0,
    0,    0,  947,  947,    0,    0,    0,    0,    0,    0,
  785,  416,    0,  785,    0,    0,    0,  417,    0,    0,
    0,  786,    0,    0,  786,    0,    0,  418,  785,    0,
  947,    0,    0,  947,    0,    0,    0,    0,    0,  786,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,  810,    0,    0,  419,    0,    0,    0,    0,
    0,  947,  947,  789,  366,  367,  420,  421,  422,  368,
  369,  423,  424,  947,  425,    0,    0,  947,    0,  426,
  427,    0,  428,  429,  430,    0,    0,    0,  431,    0,
    0,    0,    0,  810,    0,    0,  810,    0,    0,    0,
    0,    0,    0,  816,  789,    0,    0,  789,    0,    0,
    0,  810,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  789,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  947,    0,    0,    0,    0,    0,
  947,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  947,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,  816,  947,  947,  947,  947,  947,  947,
  816,    0,    0,  816,    0,    0,    0,  816,  947,    0,
    0,    0,  816,    0,    0,  816,    0,  947,  947,  947,
  947,  947,  947,  947,  947,  947,    0,  947,    0,    0,
  787,  816,  947,  947,    0,  947,  947,  947,    0,    0,
    0,  947,    0,    0,    0,    0,    0,    0,    0,    0,
  788,    0,    0,    0,    0,  816,    0,    0,  816,  816,
    0,  785,    0,    0,    0,    0,    0,    0,  816,    0,
    0,  787,  786,    0,  787,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  816,    0,    0,    0,  787,
    0,  788,    0,    0,  788,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  816,    0,  788,
    0,    0,  816,    0,    0,    0,    0,    0,  785,    0,
    0,  785,    0,    0,  816,    0,  816,    0,  785,  786,
    0,  785,  786,  818,    0,  785,    0,    0,    0,  786,
  785,    0,  786,  785,  810,    0,  786,    0,    0,    0,
    0,  786,    0,    0,  786,  789,    0,    0,    0,  785,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  786,    0,    0,    0,  818,    0,    0,  818,    0,    0,
    0,    0,    0,  785,    0,    0,  785,  785,    0,    0,
    0,    0,  818,    0,  786,    0,  785,  786,  786,    0,
    0,  810,    0,    0,  810,    0,    0,  786,    0,    0,
    0,  810,  789,  785,  810,  789,  782,    0,  810,    0,
    0,    0,  789,  810,  786,  789,  810,  783,    0,  789,
    0,    0,    0,    0,  789,  785,    0,  789,    0,    0,
  785,    0,  810,    0,    0,    0,  786,    0,    0,    0,
    0,  786,  785,  789,  785,    0,    0,  782,    0,    0,
  782,  784,    0,  786,    0,  786,  810,    0,  783,  810,
  810,  783,    0,    0,    0,  782,    0,  789,    0,  810,
  789,  789,    0,    0,    0,    0,  783,    0,    0,    0,
  789,    0,  787,    0,    0,    0,  810,    0,    0,    0,
    0,    0,  784,    0,    0,  784,    0,  789,    0,    0,
    0,    0,  788,    0,    0,    0,    0,    0,  810,    0,
  784,    0,    0,  810,    0,    0,    0,    0,    0,  789,
    0,    0,    0,    0,  789,  810,    0,  810,    0,    0,
    0,    0,    0,    0,    0,    0,  789,    0,  789,  787,
    0,    0,  787,    0,    0,    0,    0,  817,    0,  787,
    0,    0,  787,    0,    0,    0,  787,    0,    0,  788,
    0,  787,  788,    0,  787,    0,    0,    0,    0,  788,
    0,    0,  788,    0,    0,    0,  788,    0,    0,  811,
  787,  788,    0,    0,  788,  818,    0,    0,  817,    0,
    0,  817,    0,    0,    0,    0,    0,    0,    0,    0,
  788,    0,    0,    0,  787,    0,  817,  787,  787,    0,
    0,    0,    0,    0,    0,    0,    0,  787,    0,    0,
  811,    0,    0,  811,  788,    0,    0,  788,  788,    0,
    0,    0,    0,    0,  787,    0,    0,  788,  811,    0,
    0,    0,  818,  819,    0,  818,    0,    0,    0,    0,
    0,    0,  818,    0,  788,  818,  787,    0,    0,  818,
    0,  787,    0,    0,  818,    0,    0,  818,  782,    0,
    0,    0,    0,  787,    0,  787,  788,    0,    0,  783,
    0,  788,    0,  818,  819,  820,    0,  819,    0,    0,
    0,    0,    0,  788,    0,  788,    0,    0,    0,    0,
    0,    0,  819,    0,    0,    0,    0,  818,    0,    0,
  818,  818,    0,  784,    0,    0,    0,    0,    0,    0,
  818,    0,    0,    0,    0,  782,  820,  808,  782,  820,
    0,    0,    0,    0,    0,  782,  783,  818,  782,  783,
    0,    0,  782,    0,  820,    0,  783,  782,    0,  783,
  782,    0,    0,  783,    0,    0,    0,    0,  783,  818,
    0,  783,    0,    0,  818,    0,  782,    0,  808,    0,
  784,  808,    0,  784,    0,    0,  818,  783,  818,    0,
  784,    0,    0,  784,    0,    0,  808,  784,    0,    0,
  782,    0,  784,  782,  782,  784,    0,    0,    0,    0,
    0,  783,    0,  782,  783,  783,    0,    0,    0,  817,
    0,  784,    0,    0,  783,    0,    0,    0,    0,    0,
  782,    0,    0,  812,    0,    0,    0,    0,    0,    0,
    0,  783,    0,    0,    0,  784,    0,    0,  784,  784,
    0,  811,  782,    0,    0,    0,    0,  782,  784,    0,
    0,    0,    0,  783,    0,  821,    0,    0,  783,  782,
    0,  782,    0,    0,  812,  784,  817,  812,    0,  817,
  783,    0,  783,    0,    0,    0,  817,    0,    0,  817,
    0,    0,  812,  817,    0,    0,    0,  784,  817,    0,
    0,  817,  784,    0,    0,    0,  821,    0,  811,  821,
    0,  811,    0,    0,  784,  819,  784,  817,  811,    0,
    0,  811,    0,    0,  821,  811,    0,    0,    0,    0,
  811,    0,    0,  811,    0,    0,    0,    0,    0,    0,
    0,  817,    0,    0,  817,  817,    0,    0,    0,  811,
    0,    0,    0,    0,  817,    0,    0,  820,    0,    0,
    0,  809,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  817,  819,  811,    0,  819,  811,  811,    0,    0,
    0,    0,  819,    0,    0,  819,  811,    0,    0,  819,
    0,    0,    0,  817,  819,    0,    0,  819,  817,  808,
    0,    0,  809,  811,    0,  809,    0,    0,    0,    0,
  817,    0,  817,  819,  820,    0,    0,  820,    0,    0,
  809,    0,    0,    0,  820,  811,    0,  820,    0,    0,
  811,  820,    0,    0,    0,    0,  820,  819,    0,  820,
  819,  819,  811,    0,  811,    0,    0,    0,    0,    0,
  819,    0,  813,    0,    0,  820,  808,    0,    0,  808,
    0,    0,    0,    0,    0,    0,  808,  819,    0,  808,
    0,    0,    0,  808,    0,    0,    0,    0,  808,  820,
    0,  808,  820,  820,    0,    0,    0,    0,    0,  819,
    0,    0,  820,  813,  819,  812,  813,  808,    0,    0,
    0,    0,    0,    0,    0,    0,  819,    0,  819,  820,
    0,  813,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  808,    0,    0,  808,  808,    0,  821,    0,    0,
    0,  820,    0,    0,  808,    0,  820,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  820,    0,
  820,  808,  812,    0,    0,  812,    0,    0,    0,    0,
  561,    0,  812,    0,  562,  812,    0,    0,    0,  812,
    0,    0,    0,  808,  812,    0,    0,  812,  808,    0,
    0,    0,    0,  563,  821,    0,  564,  821,    0,    0,
  808,    0,  808,  812,  821,    0,    0,  821,    0,    0,
    0,  821,    0,    0,    0,    0,  821,    0,    0,  821,
    0,    0,    0,    0,    0,    0,    0,  812,    0,    0,
  812,  812,    0,  809,    0,  821,    0,  565,    0,    0,
  812,    0,    0,  566,  567,  568,    0,  569,  570,  571,
    0,  572,    0,    0,    0,    0,    0,  812,    0,  821,
    0,    0,  821,  821,    0,    0,    0,    0,    0,    0,
    0,  573,  821,  574,    0,    0,    0,    0,    0,  812,
    0,    0,    0,    0,  812,    0,    0,    0,    0,  821,
  809,    0,    0,  809,    0,    0,  812,    0,  812,    0,
  809,    0,    0,  809,  575,    0,    0,  809,    0,    0,
    0,  821,  809,    0,    0,  809,  821,    0,    0,    0,
    0,  576,    0,    0,  813,    0,    0,    0,  821,    0,
  821,  809,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  577,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  809,    0,    0,  809,  809,
  561,    0,    0,    0,  562,    0,    0,    0,  809,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  578,  579,
    0,  813,    0,  563,  813,  809,  564,    0,    0,    0,
    0,  813,    0,    0,  813,  580,    0,    0,  813,    0,
    0,    0,    0,  813,    0,  134,  813,  809,    0,  134,
    0,    0,  809,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,    0,  809,    0,  809,  565,  134,    0,
    0,  134,    0,  566,  567,  568,    0,  569,  570,  571,
    0,  572,    0,    0,    0,    0,  813,    0,    0,  813,
  813,    0,    0,    0,    0,    0,    0,    0,    0,  813,
    0,  573,    0,  574,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,    0,    0,  813,    0,  134,  134,
  134,    0,  134,  134,  134,    0,  134,    0,    0,    0,
    0,    0,    0,    0,  575,    0,    0,    0,  813,    0,
    0,    0,    0,  813,    0,    0,  134,    0,  134,    0,
    0,  576,    0,    0,    0,  813,    0,  813,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  577,    0,    0,    0,    0,    0,  134,
    0,    0,    0,    0,    0,    0,    0,    0,  102,    0,
    0,    0,    0,    0,    0,    0,  134,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  578,  579,
    0,    0,    0,    0,    0,    0,    0,    0,  134,    0,
  538,    0,    0,    0,    0,  580,    0,    0,    0,  144,
  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
  155,  156,  157,  158,  159,    0,    0,  160,  161,  162,
  163,    0,  164,  134,  134,  165,  166,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  134,
};
static short yycheck[] = {                                       9,
   52,    4,   12,   61,    4,   16,   16,  101,   18,   59,
    0,   60,  120,  121,   60,  130,  487,  331,  682,  339,
  236,  339,  310,  335,   82,  120,  121,  521,  470,  730,
  524,  736,  530, 1170,   86,  394,  530,  344,  699,  658,
   67,   51,   52,   53,   54,   55,  885,   91,  668,  999,
   60,   61,   62,  731,   64,  236,   66,   67, 1213,   69,
   70,  722,  111,   90,  113,   70,  685,  287,   78,   80,
   80,   81,   82,   65,   84,   85,   86,   87,   89,   89,
   90,   91,   92,    0,   94,  613,  267,   92,  927,  729,
  134,  308,  992,    0,  794,  619,   88,  992,   53,  999,
  624,  111,  112,  113,  999,    0,  726,  356,  244,   64,
   85,  992,   44,  825,  208,  125,  756,  253,  999,  169,
  130,  992,  132,  133,  134,  765,   54,    0,  999,  303,
  606,  944,   87,    0,   41,  698, 1000,   44,   66,   94,
   40,  236,   59,    0,  194,   44,   41, 1011,   45,   44,
  125,  662,   59,  677,    0,  371, 1125,   41,    0,   45,
   40,   44,    0,   41,   59,    0, 1030,   42,   41,   61,
   63,   44,  287,   44,    0,   40,   45,  132,  818,   40,
   40,  733,   58,  490,  112,   44,   59,   41,  500,  238,
  371,   44,   59,  507,  508,  509,  510,    0,   58,    0,
  294,   40,   59,   41,    0,  133,   44,  768,  218, 1048,
  530,   41,  530,   59,   44,   41,  226,   59,   44,  271,
  236,   59,   59,   44,   59,   44,   40,    0,  238, 1414,
  264,  702,  242,   59,  244,  245,  258,   45,  287,   44,
   44,  251, 1356,  253,  556,   41,   91,   40,   44,   63,
  386,  284,   40,  400,   44,  274,   59,   45,   59,    0,
  309,  271,  288,   59, 1261,  275,  886,  316,   41,  318,
   63,  320,   40, 1356,  323,    0,  371,  287,  262,  394,
   42, 1024,  332, 1211,  334,  307,   59,   41,  782, 1239,
   44,  268,    0, 1047, 1129,  302,  339, 1317,  348,  309,
  352,   45,  378,  288,    0,   12,  316,  403,  318,  321,
  320,   12,  561,  323,  242,  342,  533,   16,   59,   63,
  260,  251,  199,  251,  262,  965, 1346,  277,   59,  388,
  378,  341,  342,  456,   59,    0,  860,  861,  257, 1239,
  350,   45,  352,  439, 1239,  394,   41,  275,   44,    0,
  406,   59, 1378,  356,  302,  371,    0,  264, 1239, 1172,
  802,    0,   59,   59, 1228,  582,  264,  878, 1239,  273,
 1010,  419,  382,  449, 1400,  385,  386, 1532,    0,  890,
  891,   80,  441,  393,  394,  285,  341,  397,    0,  366,
  400,  339,  402,    0,   59,  405,  346,  419,  922,    0,
  443,  457,   41,  387,  269,   44,  140,  440,   59,  301,
 1042,  118,    0,  290,  321,   59,  335,  118,  466,   41,
   59, 1418,  350, 1258,  341,  435, 1126,  461,  644,    0,
  385, 1356,  277, 1356,   41,  461,  443,   59, 1356,  265,
  548,  319, 1003, 1004,  274,  400,  319,   59,  467,  387,
  321, 1379,   59, 1638,  382,  505,  310,    0,   59,  469,
  264,  264,  321,  644,    0,  393,  319,  991,  341,  397,
   41,   59,  402,   44,  402, 1630,  461,  303,  418, 1593,
  310,  530,  278, 1452,  280, 1190,  305,  668,   59,  384,
   61, 1052,  288,  319,  329,  341,    0,  332,  436, 1163,
 1052,  551, 1245,  723,  388,   41, 1170, 1371,   44,  816,
 1593, 1375, 1075, 1377,  412,  341,   59,  653,  665,  529,
  530,  453,  703,   59,  883,  535, 1663,  744, 1683, 1057,
  747,  332,   41,    0, 1234,  297, 1214,   41,  455, 1251,
   44,  829, 1355,    0,  761,  726,  453,   42, 1412,  644,
  258, 1183,  602,  603,  461,   59,  605,  549,  561,    0,
  459,  461,  459,  297,    0, 1041,  459, 1722, 1705,  743,
  437,  788,  455,  459,   41,   42,   43,   44,   45,  384,
   47,  461,  412,  379,  441,  461,  461,  448,  384,  838,
  459, 1430,   59,  258,  319,  605,  645,    0,  455,  307,
  708,  461,   59,   44,  614,  401,  441,  502,  723,  455,
 1250,  614,  408,  455,  614,  388,  341,  486,   59,    0,
  455, 1485,  461,   59,  461, 1136, 1490,  395,  644, 1649,
  503,  739,  740,  329,  455,  645,  332,  467,  648,  278,
  441,  280,  307,  653,  432,  459,  442,  437,  344,  288,
  527,  459,  662,  461,  455,  665,   59, 1411,  412, 1278,
  670, 1366,  356,  388,  502,  979,  459,    0, 1593,  410,
 1593,  459, 1196, 1537,  723, 1593,  264,  503,   59,  728,
  411,  691, 1425,  302,  733,  735,  736,  258,  432,  699,
 1554,  701, 1216,  388,  704,  670,  461,    0, 1318,  461,
  321, 1619,  397,  274,  753,  886,  502,  717, 1658,  719,
  665,  419,  722,  723,  411,  459,  441,  368,  728,  596,
  730,  264,  329,  733,  368,  332,   59,    0,  432, 1043,
  301,  867,  781,  782,  870,    0,  307,  349,   41,  310,
  379,   44,  278,  753,  280,  441,   41, 1665,  349,    0,
  742,   44,  288,  262,  419,  459,   59,  453, 1658,  455,
 1392,  403,  401, 1658,  326,  260,  388, 1055,  883,  408,
  329,  781,  782, 1637,  260,  397,  992, 1658, 1410,   44,
  997,  388, 1646,  999,  787,    0,   59, 1658,  780,  717,
  397,  719, 1009,   59,   59, 1237,  799, 1715,  265,  493,
   93, 1243, 1466,  442,   41,  257,   41,  817,   59,  304,
  398,  992, 1029,  280,  410,  319,  437,  827,  999,  286,
  812,  813,  814, 1687,  443, 1526, 1324,  871,  332,  839,
 1748,  662,  404, 1357,  883,  838,  303,  341,    0,    0,
  273,  412,  869,  379,   59,    0,    0,  349,  419,  421,
    0, 1139,  319,  261, 1772,  398,  358,  867,  868,  869,
  870,  871,  872,  502,    0,  401,  437,  872,  878,  431,
   41,  403,  408,  883,  341,   41,  438,  459,  387,   41,
  890,  891,    0,  335,  388,  444,  343,   41,  329,  817,
 1591,  332,  265,  397,   41,    0,  467,   59,   59,  827,
  471,  262,  461,  344,   59,   59,  442,  343,  341,   59,
  308,  309,    0,  868,  446,   41, 1118,  263, 1120, 1027,
    0,  378,    0,   59,    0,    0,  329,  323,    0,  332,
  303, 1592,  418,    0,  944,   40,   41,  441,  319,   44,
  459,   59,  378, 1255,   44,  278,  260,  280, 1129,  406,
  284,  455,  409,   41,   59,  406,   61,  287,    0, 1619,
  341,    0, 1665,    0,  323,  338,  502,  313,  425,   41,
  406,   59,   44,  409,  425,  278,  508,  280,  324,   59,
    0,   59, 1453,   59,   59,  288,   91,   59,  402,  425,
  304,  264,   59,   93,  451,  302, 1045, 1468, 1492, 1663,
  441,  357,    0, 1052, 1324, 1665, 1324,  388,  259, 1226,
 1020,   40,  453,  264,  455,  451,  397,   59, 1021,  270,
   59, 1021,   59, 1239, 1034,  262,  387,  262, 1512, 1124,
 1125, 1126,  339,    0,  329, 1045,  503,  332,  441,   59,
 1257, 1705, 1052, 1367, 1036, 1748,  379,  878, 1206, 1363,
  453,  385,  455,  279,  452, 1715, 1171,  322, 1239,  890,
  441,   59, 1054,  278,  329,  280, 1590,  332, 1060, 1772,
    0,  336,    0,  435,  430,  436,  379, 1258,   41,  344,
  349,   44,  428,    0, 1192, 1556, 1194, 1611, 1748, 1413,
 1614,  262,   59,  388, 1252,  360,  262,  302,  401,  461,
    0,  408,  397,  264,  330,  408,  440,    0, 1118,    0,
 1120,  418, 1772,   41,  264,  262,   44,   41, 1320,  384,
   44, 1124, 1125,  457,   41,  398, 1136,   44,  384,   59,
   93,   59,  462,  463,  339,   59,  443, 1318,  364,  442,
 1190,   41,   59,  265,   44, 1253,  264,  398,   41,  414,
  387,    0,  387,  258,  419,    0,  382,  319,  278,   59,
 1152, 1171, 1172, 1640, 1381,  319,   59,  329,   59,  274,
  332, 1153,  277,  278, 1156,  280,  441,  406,  332,  341,
  285,  303,  418,  288,  308,  309,    0,  341,  453,  264,
  455,   40,   41,  462,  463,   44,  301,  264,  288,  502,
  320, 1359,  307, 1420,  384,  310,  332,  343,   44,  445,
   59,  301,   61, 1690,   59,   42,  387,    0, 1268,    0,
   47,  387, 1272, 1694,    0,  264,  388,   41,  457, 1232,
  344,  319, 1232,  259,  388,  397, 1720,  398,  443,  319,
  387, 1356,   91,  397,  270,   59,  386,    0,  398,  329,
  322,   41,  332,  341,   44,  369, 1733,  329, 1782, 1374,
  332,  341,  388,  341,  336,   41,  264,  343, 1752, 1753,
  406,  397,  344,  409,  379,  384,   59,    0,   59,  441,
  398,  401,  319,   59,    0,    0,  258,  441,  360,  425,
   44,   44, 1776,  455, 1585,  320,  401,   44, 1589,  341,
  388,  455,  378,  408,  341, 1789,   59,  412,  388,  397,
 1320,    0,  384,  427,  419,  451, 1366,  397,   41, 1695,
 1696,   44,  442,  398, 1373, 1374,   41, 1373,  452,   44,
  406,  398,  437,  409, 1551,  307,   59,  442,   61,   93,
    0,  444,  414,   59,   59, 1355,   93,  461,  278,  425,
  280,  384,  319,  441, 1645,  437, 1338, 1452,  461,  398,
 1342,  441,  467, 1373, 1374,  332,  471,  455,   91,  441,
   59,  419, 1382,  258,  341,  451,  401,  262,  278, 1487,
  280,  453,  267,  455,  308,  309, 1544,  495,  288,  344,
   41,  319, 1550,   44,  353,   46, 1650, 1512, 1652,   59,
  398,  329,  319,    0,  332,  322,  338, 1783,   59,  258,
   61,  296,  329,  341,  369,  332,  344,  442,  274,  336,
  379,  388,  260,    0,  341,  274,  319,  344,  277,  278,
  397,  280,   40,    0,  301, 1493,  285,   45, 1692,  288,
  319, 1535, 1658,  360,   41, 1736,  379,  419,  341, 1452,
 1642,  260,  301,  280, 1382,   63, 1459,   40,  307,  286,
  388,  310,   59, 1512,    0,  437,  304,  384,  401,  397,
  387,  388,  427,   45,  441,  408,  260, 1658, 1593,  379,
  397, 1463,   59, 1493, 1536,   45,  353,  411,   42,   43,
  494,   45,   59,   47,  425,  304, 1688,  414,  343,  471,
  504,  401, 1512,   63, 1619,  319, 1496,  279,  408,  442,
 1560, 1765,  459,  441, 1564,  329, 1526,  272,  332,  436,
  304, 1531,  462,   59,  441,  453, 1536,  341,  452,  301,
  379,  260,  264,  378,  289,  258,  453, 1631,  455,  486,
 1732,  319,  442,  428, 1555, 1555,  317,  432,  433,  434,
 1665,  274,  401,  322,  277,  278,  332,  280,  330,  408,
    0,  406,  285,  412,  409,  288,    0,  336,   42,   43,
  419,   45, 1764,   47,  388,  304,  322,  349,  301,  334,
  425, 1591, 1592,  397,  307,  343,  358,  310,  437,  338,
  336,  360,  364,  442,    0, 1605,  280,  357,  344,    0,
 1715,  337,  286, 1531,  319, 1720,  451,    0,  363, 1619,
  382,    0,  388,  264,  360,  384, 1674,  295,  467,   59,
  378,  397,  471,    0,    0,   59,  341,  441,  306, 1640,
 1640,    0,    0, 1748,  394,  313,  396, 1752, 1753,  453,
   41,  259,  398,   44,  403,  414, 1657, 1657,  406,  453,
 1605,  409,  270,   59,  305, 1665,  379, 1772,   59,  419,
  338, 1776,    0,  321, 1674, 1675,   59,  425,  414,    0,
   59, 1720,  341, 1676, 1789,  430, 1676,   40,  401, 1690,
 1690,  437,   59,   59,  444,  408, 1697, 1697,  296,  412,
   59,   59, 1744,  451,  319,    0,  419,   42,   43,    0,
   45,    0,   47, 1752, 1753, 1715,  418,  403,   42,   43,
 1720,   45,  384,   47,  437,  471,  384,   41,    0,  442,
   44,   59, 1733, 1733,  296,  403,  280, 1776,   59,  218,
 1741, 1741,  286,  445, 1744,    0,  296,  226, 1748,  624,
 1789,   44, 1752, 1753,  467,  353,  317,  318,  471,   41,
   42,   43,  324,   45,   59,   47,   59,  435,   59, 1769,
   59,  439, 1772,  494,  495, 1775, 1776,  321,   42,   43,
   44,   45,  507,   47,    0,    0,    0,   59,    0, 1789,
  437,  353, 1792,  668,    0,  451,  671,   42,   43,    0,
   45,  676,   47,  353,   59,  435,  681,  682,   41,   42,
   43,  322,   45,  338,   47,  690,  280,  692,  693,  694,
  695,  696,  286,  698,  362,  336,   41,   41,  703,   44,
   44,    0,    0,  344,   40,   41,   42,   43,   44,   45,
   46,   47,  338,   59,   59,   59,    0,   59,  312,  360,
  375,  726,  418,   59,   60,   61,   62,  732,   59,  443,
  458,  459,  460,  388,  462,  463,  428,   42,   43,  466,
   45,  468,   47,   41,    0,  329,   44,   41,   41,  375,
   44,   44,  444,  481,  482,   91,  358,   93,  486,  487,
   59,   59,  388,   61,  462,  463,  458,  459,  460,  288,
  462,  463,  322,  414,   41,   59,  412,   44,  458,  459,
  460,   41,  462,  463,   44,   41,  336,  470,   44,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  319,   93,
   44,  481,  482,   59,  496,  497,  486,  487,  329,   61,
  360,  332,  403,  505, 1118,  280, 1120, 1737, 1738,  403,
  341,  286,   41,  344,  322,   44,  280,   41,   42,   43,
  260,   45,  286,   47,  384,  373,   41,  287,  336,   44,
  845,  846,  847,   40,  435,  850,  851,  852,  264,  854,
  855,  856,  857,  858,  859,  439,  321,   40,  312,  450,
  444,  840,  360,  842,  414,  462,  450,  388,  280,  462,
  258, 1791,  302,  261,  286,  462,  397,  461,  458,  459,
  460,  886,  392,  393,  314,  469,  280,  262,  893,  894,
   44,  896,  286,  898,  462,  463,  901,  902,  903,  904,
  905,  906,  437,  321,    0,  280,  486,  487,   44,  339,
  403,  286,   40,  918,   40,  920,  414,  280,    0,  307,
  441,   41,   41,  286,   44,   44,  262,   41,  264,  265,
   44,   41,  453,  278,   44,  280,  272,   46,   41,   41,
  285,   44,   44,  288,  280,   41,   42,   43,   44,   45,
  286,   47,   41,  289,   40,   44,  301,  262,   41,   41,
  258,   44,   44,   59,   60,   61,   62,  303,   41,   40,
   40,   44,  308,  309,    0,  280,  312,   59,  408,  277,
  278,  286,  280,  319,   40,  321,  322,  285,  418,  325,
  288,   41,   40,  329,   44,  279,  332,   93,  334,   40,
  336,   40,   40,  301,   40,  341,  342,   40,  344,  307,
   40,   91,  321,  443,  667,   41,  669,  301,   44,  672,
  673,  674,   44,   44,  360,  361,  429,  363,   46,   61,
   40,  461,  264,   59,  379,   44,  383,  349,   44,  278,
  278,  377,  301,  379,  380,  381,  330,    0,  384,  264,
  398,  387,  388,  437,  471,  398,  401,  277,  459,  448,
  400,  397,  448,  408,   40,  349,  280,  350,   40,   40,
 1075,    0,  286,   40,  358,  411,   91,  319,  414,  264,
  364,  379, 1087, 1088,  384, 1090, 1091,  412,   41,   44,
    0,   44,  383, 1098,  430,  390,    0,  442,  382,   40,
  436,   40,  437,  401,  465,  441,   59,  318,   61,   40,
  408,  321,  321,  452,   41,  453,  452,  453,  264,  455,
  449,  419,  455,   40, 1129,  461,  277,  459,   40,  274,
   59,   41,  301,  285,   44,  278,  387,   41, 1143,  437,
   44,   41,   42,   43,  442,   45, 1151,   47, 1153,   59,
  345, 1156,  345, 1158,  387,   59,  387, 1162, 1163, 1164,
 1165,  419,  417,  301,   40, 1170,  262,  503,  264,  265,
  419,  357,  418,  471,   40,  274,  272,  418,  386,   41,
   42,   43,   40,   45,  280,   47,  368,   44,   44,  321,
  286,   41,  264,  289,   42,   43,  278,   45,  280,   47,
   44,   44,  280,  285, 1209,  321,  288,  303,   40,   44,
   44,   59,  308,  309,    0,  503,  312,  329,   40,  301,
  506,  266,  502,  319,  300,  321,  322,   40,   58,  325,
  264,  423,   41,  329,   41,   41,  332,   41,  334,  264,
  336,  441,  455,  310,  310,  341,  342,  395,  344,   41,
  493,   41,  278, 1258,  280,   41,   42,   43,   44,   45,
   40,   47,  288,   40,  360,  361,  353,  363,   40,   40,
 1275,   40,   40,   59,   60,   61,   62,   40,   40, 1284,
   40,  377,   40,  379,  380,  381, 1291,   40,  384,   40,
   40,  387,  388,   42,   43,   44,   45,  379,   47,   40,
   40,  397,    0,   40,  262,  258,   44,   93,   41,   42,
   43,  328,   45, 1318,   47,  411,  309,   41,  414,  401,
  389,  436,  274,  423,  277,  278,  408,  280,  389,  389,
  344,  332,  285, 1338,  430,  288,  391, 1342,  392,  447,
  436,  271,  288,   41,  408,  441,   44,  387,  301,   41,
  279,    0,  387,  379,  307,   41,  452,  453,    0,  455,
  442,   59,  278,  387,  387,  461,   42,   43,  278,   45,
  280,   47,  301,   40,  278,  401,  280,   41,  288,  387,
  280,  379,  408,   59,  288,  387,  286,   41,   41,  418,
  386,  301,   41,   42,   43,   44,   45,  301,   47,   41,
  437,  330,   44,   41,  309,  436,  397,  503,  280,  302,
   59,   60,   61,   62,  338,    0,  442,   59,  280,  339,
  349,  339,  302,  443,  286,   41,  379,  443,  357,  358,
   41,  353,  280, 1438, 1439,  364,  321,  386,  286,   41,
   42,   43,   41,   45,   93,   47,  372,  372,  401,   41,
   41,   41,   41,  382,   41,  408,  280,   41, 1463,   41,
   41, 1466,   41,   41,  417,  394,  419,  396,   41,  379,
   41,   41,   41,   41,   59,  379,  262,   44,  264,  265,
   42,   43,   46,   45,  437,   47,  272,   59,   40,  442,
  419,  401,  341,   59, 1499,   40,   59,  401,  408,  341,
  286,   59,   59,  289,  408,   59,   59,   41,   42,   43,
  309,   45,   59,   47,   41,  444,  353,  303,  471,  345,
   41,   40,  308,  309,    0,  341,  312,   44,  369,  452,
  384,  280,  442,  319,   59,  321,  322,  286,  442,  325,
   41,   59,  344,  329,  384,  384,  332,  280,  334,   41,
  336,   41,  411,  286, 1559,  341,  342,   44,  344,   44,
  305,  436, 1567,  302,  443,   41,   42,   43,   44,   45,
  388,   47,   40,   59,  360,  361,  294,  363,  305,   44,
  278,  464,  280,   59,   60,   61,   62,  308,   44,   41,
    0,  377,  260,  379,  380,  381,  338,  384,  384, 1604,
  384,  387,  388, 1608,  280,   40,  264,  278,  461,  264,
  286,  397,  459,  262,  390,  264,  265,   93,   41,   42,
   43,  321,   45,  272,   47,  411,   42,   43,  414,   45,
  264,   47,  452,  471,  302,   40,  278,  412,  280,  271,
  289,   41,   42,   43,  430,   45,  314,   47,  310,  459,
  436,  264,  437,  436,  303,  441,  344,  344, 1663,  308,
  309,    0, 1667,  312,  344,  344,  452,  453,  459,  455,
  319,  339,  321,  322,  271,  461,  325,   41,  280,  461,
  329,  379,  309,  332,  286,  334,   40,  336,  309,   40,
   59,   59,  341,  342,  279,  344,   41,  309,   59,  305,
 1705,   59,   41,  401,   43,   44,   45,   41,   44,   59,
  408,  360,  361,   41,  363,    0,  301,  503,  280,    0,
   59,   60,   61,   62,  286,    0, 1121,  253,  377,   12,
  379,  380,  381,  246,  245,  384,    0,  379,  387,  388,
  408, 1057,  653,   84,  442,  330,  280, 1055,  397,  609,
  418,  830,  286, 1401,   93, 1052,  306, 1044,  556,  401,
  336,  301,  411, 1034,  349,  414,  408,  728,   41,   42,
   43, 1045,   45,  358,   47,  443,  982,   41,  292,  364,
   44,  430,  318,  260, 1602,  318,  262,  436,  264,  265,
  763, 1379,  441, 1681, 1684,   59,  272,  382, 1381,  535,
  442, 1685, 1724,  452,  453,  753,  455, 1207, 1775, 1624,
 1721,  967,  461,  289, 1185,   41,   42,   43, 1360,   45,
  371,   47,   41,   42,   43,  302,   45,  303,   47, 1640,
  323,    0,  308,  309,  537,  304,  312,  314,   41,   42,
   43,  311,   45,  319,   47,  321,  322,  809, 1241,  325,
  801,  328,  595,  329,  503,   13,  332,  280,  334,  405,
  336,  279,  339,  286,  280,  341,  342,  472,  344,  476,
  286,  717,   41,  123,   43,   44,   45,  732,  703,  944,
  280,  704, 1593,    0,  360,  361,  286,  363, 1483, 1697,
   59,   60,   61,   62, 1741, 1792,  661,  887, 1089,  487,
   -1,  377,   -1,  379,  380,  381,  370,   -1,  384,   -1,
 1158,  387,  388,  473,  474,  475,  476,  477,  478,  479,
  480,  397,   -1,  262,   93,  264,  265,   -1,   -1,   -1,
   -1,  408,   -1,  272,   -1,  411,   -1,   -1,  414,   -1,
   -1,  418,   59,   -1,  850,   -1,  852,    0,  854,  855,
  289,  857,  858,  859,  430,   -1,   -1,   -1,   -1,   -1,
  436,   -1,   -1,   -1,  303,  441,  443,   -1,   -1,  308,
  309,   -1,   -1,  312,   -1,   -1,  452,  453,   -1,  455,
  319,   -1,  321,  322,  461,  461,  325,   -1,   -1,   -1,
  329,    0,   -1,  332,  258,  334,   -1,  336,   -1,   -1,
   -1,   -1,  341,  342,   -1,  344,   59,  280,   -1,   -1,
   -1,   -1,   -1,  286,   -1,  279,   -1,   -1,   -1,   -1,
   -1,  360,  361,   -1,  363,   -1,   -1,  503,   -1,   -1,
   -1,   -1,   41,   -1,   43,   44,   45,  301,  377,   -1,
  379,  380,  381,  307,   -1,  384,   -1,   -1,  387,  388,
   59,   60,   61,   62,  280,   -1,   -1,   -1,  397,   -1,
  286,  280,   -1,   -1,   -1,   -1,  330,  286,   -1,   -1,
   -1,   -1,  411,   -1,   -1,  414,   -1,  280,   -1,   -1,
   -1,   -1,   -1,  286,   93,  349,   -1,   -1,   -1,   -1,
   -1,  430,   -1,  262,  358,  264,  265,  436,   -1,   -1,
  364,   -1,  441,  272,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,  452,  453,   -1,  455,   -1,  382,   -1,
  289,   -1,  461,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,  308,
  309,  258,    0,  312,   -1,   41,   42,   43,   -1,   45,
  319,   47,  321,  322,   -1,  419,  325,   -1,   -1,   -1,
  329,   -1,  279,  332,  503,  334,   -1,  336,   -1,   -1,
   -1,   -1,  341,  342,   -1,  344,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,  301,   43,   44,   45,   -1,   -1,
  307,  360,  361,   -1,  363,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,  258,   -1,   -1,  377,   -1,
  379,  380,  381,  330,   -1,  384,   -1,   -1,  387,  388,
   -1,   -1,   -1,   -1,   -1,   -1,  279,   -1,  397,   -1,
   -1,   -1,  349,   -1,   -1,   93,   -1,   -1,   -1,   -1,
   -1,  358,  411,   -1,   -1,  414,   -1,  364,  301,   -1,
   -1,   -1,   -1,  262,  307,  264,  265,   -1,   -1,   -1,
   -1,  430,   -1,  272,   -1,  382,   -1,  436,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,  330,   -1,   -1,
  289,   -1,   -1,  452,  453,   -1,  455,   -1,   -1,   -1,
   -1,   -1,  461,   -1,  303,   -1,  349,   -1,   -1,  308,
  309,   40,  419,  312,   43,  358,   45,   -1,    0,   -1,
  319,  364,  321,  322,    0,   -1,  325,   -1,   -1,   58,
  329,   -1,   -1,  332,   63,  334,   -1,  336,   -1,  382,
   -1,   -1,  341,  342,  503,  344,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,   -1,  286,
   -1,  360,  361,   -1,  363,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  280,   -1,   -1,  419,   59,  377,  286,
  379,  380,  381,   59,   -1,  384,   -1,   -1,  387,  388,
  271,   -1,   -1,   -1,  280,   -1,   -1,   -1,  397,  355,
  286,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  411,   -1,  262,  414,  264,  265,   -1,   -1,
   -1,  302,   -1,   -1,  272,   -1,   -1,   -1,  309,   -1,
   -1,  430,  313,  314,   -1,  316,   -1,  436,  319,    0,
   -1,  289,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  333,  452,  453,  303,  455,   -1,  339,   -1,
  308,  309,  461,   -1,  312,   -1,   -1,   -1,   -1,   -1,
   -1,  319,   -1,  321,  322,   -1,   -1,  325,   -1,   -1,
   41,  329,   -1,   44,  332,   -1,  334,   -1,  336,   -1,
   -1,   -1,   -1,  341,  342,   -1,  344,   -1,   59,   -1,
   61,   -1,   -1,  459,  503,  461,   -1,   -1,   -1,   -1,
   -1,   -1,  360,  361,   -1,  363,  472,   -1,  399,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  377,
   91,  379,  380,  381,   -1,  491,  384,  418,  267,  387,
  388,   -1,   -1,  499,  500,  501,   -1,  276,   -1,  397,
   -1,   40,  433,   -1,   43,   -1,   45,   -1,    0,   -1,
   -1,  290,  443,  411,  293,   -1,  414,  296,  297,   58,
   -1,  452,   -1,  454,   63,   -1,  258,   -1,   -1,   -1,
  461,   -1,  430,   -1,   -1,   -1,  315,   -1,  436,   -1,
   -1,   -1,   -1,  441,   -1,  324,   -1,  279,  327,   -1,
   -1,   -1,   -1,   -1,  452,  453,   -1,  455,   -1,  490,
   -1,   -1,   -1,  461,   -1,   -1,   -1,   59,   -1,  301,
   -1,   -1,   -1,   -1,   -1,  307,  355,  356,  509,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,  367,   43,
   -1,   45,  371,  319,   -1,   -1,  322,   -1,  330,   -1,
  379,   -1,   -1,  329,   58,  503,  332,   -1,   -1,   63,
  336,   -1,   -1,   -1,   -1,  341,   -1,  349,  344,   -1,
   -1,   -1,   -1,   -1,   -1,  357,  358,   -1,   -1,   -1,
   -1,   -1,  364,   -1,  360,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  422,   -1,   -1,   -1,  258,   -1,  428,
  382,   -1,   -1,   -1,   -1,  434,   -1,   -1,  384,   -1,
   -1,   -1,  388,  274,   -1,  444,   -1,  278,   -1,  280,
   -1,  397,   -1,   -1,  285,   -1,   -1,  288,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,  419,  414,   -1,
  301,   -1,   -1,  472,   -1,   -1,  307,   -1,  430,  310,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,  441,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,  453,  267,   -1,
   -1,  510,  511,  512,   -1,  461,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,
  295,   -1,   -1,   -1,   -1,   -1,  258,   -1,  379,   -1,
   -1,  306,   -1,   -1,   -1,   -1,  315,   -1,  313,   -1,
   -1,   -1,   -1,   -1,   -1,  324,    0,  279,  327,   -1,
  401,  326,   -1,   -1,   -1,   -1,   -1,  408,   -1,   -1,
   -1,  412,   -1,   -1,   -1,   -1,   -1,   -1,  419,  301,
   -1,   -1,   -1,  267,   -1,  307,  355,  356,   -1,   -1,
   -1,    0,  276,   -1,   -1,   -1,  437,   41,  367,   -1,
   44,  442,  371,   -1,   -1,   -1,  290,   -1,  330,   -1,
  379,   -1,  296,  297,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,  387,   -1,   -1,   -1,  467,  349,   -1,   -1,
  471,  315,   41,   -1,   -1,   44,  358,    0,  403,   -1,
  324,   -1,  364,  327,   -1,   -1,   -1,   91,   -1,  418,
   59,   -1,   61,  422,   -1,  420,   -1,   -1,   -1,  428,
  382,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
  435,  355,  356,   -1,  439,  444,   -1,  442,   41,   -1,
   -1,   44,   91,  367,   -1,  450,   -1,  371,   -1,  458,
  459,  460,  461,  462,  463,  379,   59,  419,   61,   -1,
   -1,   -1,   -1,  472,  469,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,  422, 1276,
 1277,  510,  511,  512,  428, 1282, 1283,   -1, 1285, 1286,
  434,   -1,   -1,   -1,   -1,   -1, 1293, 1294, 1295, 1296,
  444, 1298, 1299, 1300, 1301, 1302, 1303,   -1,   40,   -1,
   42,   43,   -1,   45,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  472,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,  258,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,  510,  511,  512,   -1,
  274,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,  258,
   40,   -1,   42,   43,   -1,   45,   -1,  301,   -1,   -1,
   -1,   -1,   -1,  307,   -1,  274,  310,   -1,   58,  278,
   -1,  280,   -1,   63,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  301,   -1,   -1,  258,   -1,   -1,  307,   -1,
   -1,  310,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  278,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  379,   -1,   -1,  301,   -1,
   -1,   -1,   -1,   -1,  307,   -1,   -1,  306,   40,   -1,
   -1,   43,   -1,   45,  313,   -1,   -1,  401,  317,  318,
   -1,   -1,   -1,   -1,  408,   -1,   58,  326,  412,   -1,
  379,   63,   -1,   -1,   -1,  419,   -1,   -1,   -1,  338,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,
   -1,   -1,  401,  437,   -1,  267,   -1,   -1,  442,  408,
   -1,   -1,   -1,  412,  276,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,  379,   -1,  290,   -1,
   -1,   -1,   -1,  467,  296,  297,   -1,  471,  437,   -1,
   -1,   -1,  304,  442,   -1,   -1,   -1,   -1,  401,   -1,
   -1,   -1,   -1,   -1,  403,  408,   -1,   -1,   -1,   -1,
   -1,   -1,  324,   -1,   -1,  327,  419,   -1,  467,   -1,
   -1,  420,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,   -1,   -1,  437,   -1,  435,  267,   -1,  442,
  439,  353,   -1,  355,  356,   -1,  276,   -1,   -1,   -1,
   -1,  450,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
  290,   -1,   -1,   -1,   -1,   -1,  296,  297,  471,   -1,
  469,   -1,   -1,   -1,  304,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  324,   -1,   -1,  327,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,  353,   -1,  355,  356,   -1,  260,   -1,
   -1,  263,  444,   -1,   -1,  267,   -1,  367,   -1,   -1,
   -1,  371,   -1,   -1,  276,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
  472,   -1,   -1,   40,  296,  297,   43,   -1,   45,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   58,   -1,   -1,  496,  497,   63,  499,  500,  501,
   -1,   -1,  324,  505,   -1,  327,   -1,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  355,  356,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,  260,   -1,   -1,
  263,   -1,  444,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,
  472,   -1,   -1,  296,  297,   -1,   -1,   -1,    0,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,
   -1,  324,   -1,  505,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,   41,
  267,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,  355,  356,   -1,   -1,   40,   59,   -1,   43,
   -1,   45,   -1,  290,  367,   -1,   -1,   -1,  371,  296,
  297,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   41,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  426,   -1,  428,   -1,   -1,  355,  356,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,  444,   -1,   -1,  371,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   42,   43,   -1,   45,   -1,   47,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   60,
   61,   62,   -1,  496,  497,   -1,  499,  500,  501,  426,
   -1,  428,  505,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,  496,
  497,   -1,  499,  500,  501,   -1,  260,   -1,  505,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   58,
   -1,   -1,  296,  297,   63,  272,   -1,  319,   -1,   -1,
  322,   -1,   -1,  280,   -1,   -1,   -1,  329,   -1,  286,
  332,   -1,  289,   -1,  336,   -1,   -1,   -1,   -1,  341,
  324,   -1,  344,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  325,   -1,
   -1,  355,  356,   40,   -1,   -1,   43,  334,   45,   -1,
   -1,   -1,  384,  367,   -1,  342,  388,  371,   -1,   -1,
   -1,   58,   -1,   -1,   -1,  397,   63,   -1,   -1,   -1,
   -1,  272,   -1,   -1,  361,   -1,  363,   -1,   -1,  280,
   -1,   -1,  414,   -1,   -1,  286,  261,   -1,  289,   -1,
  377,   -1,  379,  380,  381,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  283,  441,
   -1,   -1,  426,   -1,  428,   -1,  291,   -1,   -1,   -1,
  434,  453,   -1,  298,  325,  300,   -1,  302,   -1,   -1,
  444,  273,  307,  334,   -1,  277,   -1,   -1,   -1,  314,
   -1,  342,   -1,  430,  458,  459,  460,  461,  462,  463,
  292,   -1,   -1,  328,  296,   -1,   -1,  299,  472,   -1,
  361,   -1,  363,   -1,  339,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  377,  491,  379,  380,
  381,  260,  496,  497,  263,  499,  500,  501,  267,   -1,
   -1,  505,   -1,   -1,   -1,   -1,   40,  276,  340,   43,
   -1,   45,   -1,   -1,  346,  347,  348,   -1,  350,  351,
  352,  290,  354,   -1,   58,   -1,   -1,  296,  297,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,
   -1,  406,  374,   -1,  376,   -1,   -1,   -1,  413,   -1,
   -1,  416,   -1,  418,  419,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,  407,  263,   -1,  443,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  355,  356,   40,  276,
   -1,   43,  424,   45,   -1,   -1,   -1,   -1,  367,   -1,
   -1,   -1,  371,  290,   -1,   -1,   58,   -1,   -1,  296,
  297,   63,   -1,   -1,  446,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,  507,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,  498,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,
  499,  500,  501,  267,   -1,   -1,  505,   -1,   -1,  426,
   -1,  428,  276,   -1,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,  444,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  260,  496,
  497,  263,  499,  500,  501,  267,   -1,   -1,  505,   -1,
   -1,  355,  356,   40,  276,   42,   43,   -1,   45,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  290,   -1,
   -1,   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,
  267,   40,   -1,  505,   43,   -1,   45,  271,   -1,  276,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,  290,   63,   -1,   -1,   -1,   -1,  296,
  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  313,
  314,   -1,  316,   41,   -1,  319,   44,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  333,
   -1,   59,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  353,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  399,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,  302,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,  313,  314,   -1,  316,
   -1,  428,  319,   -1,   -1,   -1,   -1,  434,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,  333,  444,   -1,  443,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  452,   -1,
  454,  458,  459,  460,  461,  462,  463,  461,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,  490,   -1,  267,  496,
  497,   -1,  499,  500,  501,   -1,   -1,  276,  505,   -1,
   -1,   -1,  399,   -1,   -1,  509,   -1,   40,   -1,   -1,
   43,  290,   45,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,  418,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,  443,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,  461,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  353,   -1,  355,  356,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,
   -1,  319,  371,  490,  322,   -1,   -1,   -1,   -1,   -1,
   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,
   -1,  267,  509,  341,   -1,   -1,  344,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,  360,   43,  290,   45,   -1,   -1,   -1,  418,
  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   58,  428,
   -1,   -1,   -1,   63,   -1,  434,  384,   -1,   -1,   -1,
  388,   -1,   -1,   -1,   -1,  444,   -1,   -1,  324,  397,
   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,  414,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,  353,   -1,  355,
  356,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  367,  491,  441,   -1,  371,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,  453,  505,  260,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,
   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,
   -1,  304,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,  355,  356,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  367,  491,   -1,   -1,  371,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,   -1,   -1,   -1,   -1,   -1,   40,  267,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  367,  491,   -1,
   -1,  371,   -1,  496,  497,   -1,  499,  500,  501,   -1,
   -1,   -1,  505,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  461,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,  483,  484,  485,  428,   -1,
  488,  489,  490,  491,  434,  493,   -1,   -1,  496,  497,
   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,  267,   -1,   -1,  496,  497,   -1,  499,
  500,  501,  276,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   41,   -1,   43,  290,   45,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,  273,   -1,
   58,   -1,  277,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,  461,   -1,
  324,  296,   -1,  327,  299,   -1,   -1,  470,  471,  472,
  473,  474,  475,  476,  477,  478,  479,  480,  481,  482,
  483,  484,  485,   -1,   -1,  488,  489,  490,  491,   -1,
  493,  355,  356,  496,  497,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,  340,   -1,  371,   -1,   -1,
   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,  374,
   -1,  376,   -1,   -1,   -1,   40,   -1,   -1,   43,  290,
   45,   -1,   -1,   -1,  418,  296,  297,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  428,   -1,   -1,   -1,   63,   -1,
  434,   -1,  407,   -1,   -1,   -1,   -1,  412,   -1,   -1,
  444,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,  424,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,  446,  353,   -1,  355,  356,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  367,  491,   -1,   -1,
  371,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,  481,  482,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,  498,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,  290,   45,   -1,   -1,   -1,   -1,  296,  297,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  428,   -1,   -1,
   -1,   63,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,   -1,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  367,
  491,   -1,   -1,  371,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,  290,   45,   -1,   -1,   -1,
   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   58,
  428,   -1,   -1,   -1,   63,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,
  355,  356,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  367,  491,   -1,   -1,  371,   -1,  496,  497,
   -1,  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,
  455,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,  355,  356,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  367,  491,   -1,   -1,  371,
   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,
  505,   -1,   -1,   -1,   -1,   -1,   -1,   40,  267,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   59,   -1,   -1,   -1,
   63,  290,   -1,   -1,   -1,   -1,  418,  296,  297,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  367,  491,
   -1,   -1,  371,   -1,  496,  497,  273,  499,  500,  501,
  277,   -1,   -1,  505,   -1,   -1,   -1,   -1,  285,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  296,
   -1,   -1,  299,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  340,   -1,  444,   -1,   -1,   -1,  346,
  347,  348,   -1,  350,  351,  352,  455,  354,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,  374,   -1,  376,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,  267,   -1,   -1,  496,  497,   -1,
  499,  500,  501,  276,   -1,   -1,  505,   -1,   -1,   -1,
  407,   -1,   -1,   40,   -1,   -1,   43,  290,   45,   -1,
   -1,   -1,   -1,  296,  297,   -1,   -1,  424,   -1,  273,
   -1,   58,   -1,  277,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,  446,
   -1,  324,  296,   -1,  327,  299,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  461,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  355,  356,  481,  482,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  367,   -1,  340,   -1,  371,   -1,
   -1,  498,  346,  347,  348,   -1,  350,  351,  352,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
  374,   -1,  376,   -1,   -1,   -1,   40,   -1,   -1,   43,
  290,   45,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   58,  428,   -1,   -1,   -1,   63,
   -1,  434,   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,
  424,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   -1,  446,  353,   -1,  355,  356,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  367,  491,   -1,
   -1,  371,   -1,  496,  497,   -1,  499,  500,  501,   -1,
   -1,    0,  505,   -1,   -1,   -1,   -1,  481,  482,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,  498,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,
  297,   -1,   41,   -1,   -1,   44,   -1,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,  444,   -1,   -1,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,  355,  356,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  367,  491,   -1,   -1,  371,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   41,  428,   -1,   44,   -1,   -1,   -1,  434,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,  444,   59,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   59,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,  355,  356,    0,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  367,  491,   -1,   -1,  371,   -1,  496,
  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   41,   -1,   -1,   44,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  319,   -1,   -1,  322,  458,  459,  460,  461,  462,  463,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,  472,   -1,
   -1,   -1,  341,   -1,   -1,  344,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
    0,  360,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   41,  262,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  414,   -1,   -1,   -1,   59,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,   -1,   59,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  319,   -1,
   -1,  322,   -1,   -1,  453,   -1,  455,   -1,  329,  319,
   -1,  332,  322,    0,   -1,  336,   -1,   -1,   -1,  329,
  341,   -1,  332,  344,  262,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   -1,  344,  262,   -1,   -1,   -1,  360,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,
   -1,   -1,   59,   -1,  384,   -1,  397,  387,  388,   -1,
   -1,  319,   -1,   -1,  322,   -1,   -1,  397,   -1,   -1,
   -1,  329,  319,  414,  332,  322,    0,   -1,  336,   -1,
   -1,   -1,  329,  341,  414,  332,  344,    0,   -1,  336,
   -1,   -1,   -1,   -1,  341,  436,   -1,  344,   -1,   -1,
  441,   -1,  360,   -1,   -1,   -1,  436,   -1,   -1,   -1,
   -1,  441,  453,  360,  455,   -1,   -1,   41,   -1,   -1,
   44,    0,   -1,  453,   -1,  455,  384,   -1,   41,  387,
  388,   44,   -1,   -1,   -1,   59,   -1,  384,   -1,  397,
  387,  388,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
  397,   -1,  262,   -1,   -1,   -1,  414,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,  414,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,  436,   -1,
   59,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,  453,   -1,  455,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  453,   -1,  455,  319,
   -1,   -1,  322,   -1,   -1,   -1,   -1,    0,   -1,  329,
   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,  319,
   -1,  341,  322,   -1,  344,   -1,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,    0,
  360,  341,   -1,   -1,  344,  262,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  384,   -1,   59,  387,  388,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   41,   -1,   -1,   44,  384,   -1,   -1,  387,  388,   -1,
   -1,   -1,   -1,   -1,  414,   -1,   -1,  397,   59,   -1,
   -1,   -1,  319,    0,   -1,  322,   -1,   -1,   -1,   -1,
   -1,   -1,  329,   -1,  414,  332,  436,   -1,   -1,  336,
   -1,  441,   -1,   -1,  341,   -1,   -1,  344,  262,   -1,
   -1,   -1,   -1,  453,   -1,  455,  436,   -1,   -1,  262,
   -1,  441,   -1,  360,   41,    0,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
  397,   -1,   -1,   -1,   -1,  319,   41,    0,  322,   44,
   -1,   -1,   -1,   -1,   -1,  329,  319,  414,  332,  322,
   -1,   -1,  336,   -1,   59,   -1,  329,  341,   -1,  332,
  344,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,  436,
   -1,  344,   -1,   -1,  441,   -1,  360,   -1,   41,   -1,
  319,   44,   -1,  322,   -1,   -1,  453,  360,  455,   -1,
  329,   -1,   -1,  332,   -1,   -1,   59,  336,   -1,   -1,
  384,   -1,  341,  387,  388,  344,   -1,   -1,   -1,   -1,
   -1,  384,   -1,  397,  387,  388,   -1,   -1,   -1,  262,
   -1,  360,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
  414,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  414,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,  262,  436,   -1,   -1,   -1,   -1,  441,  397,   -1,
   -1,   -1,   -1,  436,   -1,    0,   -1,   -1,  441,  453,
   -1,  455,   -1,   -1,   41,  414,  319,   44,   -1,  322,
  453,   -1,  455,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   59,  336,   -1,   -1,   -1,  436,  341,   -1,
   -1,  344,  441,   -1,   -1,   -1,   41,   -1,  319,   44,
   -1,  322,   -1,   -1,  453,  262,  455,  360,  329,   -1,
   -1,  332,   -1,   -1,   59,  336,   -1,   -1,   -1,   -1,
  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,  360,
   -1,   -1,   -1,   -1,  397,   -1,   -1,  262,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  414,  319,  384,   -1,  322,  387,  388,   -1,   -1,
   -1,   -1,  329,   -1,   -1,  332,  397,   -1,   -1,  336,
   -1,   -1,   -1,  436,  341,   -1,   -1,  344,  441,  262,
   -1,   -1,   41,  414,   -1,   44,   -1,   -1,   -1,   -1,
  453,   -1,  455,  360,  319,   -1,   -1,  322,   -1,   -1,
   59,   -1,   -1,   -1,  329,  436,   -1,  332,   -1,   -1,
  441,  336,   -1,   -1,   -1,   -1,  341,  384,   -1,  344,
  387,  388,  453,   -1,  455,   -1,   -1,   -1,   -1,   -1,
  397,   -1,    0,   -1,   -1,  360,  319,   -1,   -1,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,  414,   -1,  332,
   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,  384,
   -1,  344,  387,  388,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,  397,   41,  441,  262,   44,  360,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  453,   -1,  455,  414,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,  262,   -1,   -1,
   -1,  436,   -1,   -1,  397,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  453,   -1,
  455,  414,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
  273,   -1,  329,   -1,  277,  332,   -1,   -1,   -1,  336,
   -1,   -1,   -1,  436,  341,   -1,   -1,  344,  441,   -1,
   -1,   -1,   -1,  296,  319,   -1,  299,  322,   -1,   -1,
  453,   -1,  455,  360,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,   -1,  344,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,  262,   -1,  360,   -1,  340,   -1,   -1,
  397,   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,
   -1,  354,   -1,   -1,   -1,   -1,   -1,  414,   -1,  384,
   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  374,  397,  376,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,  414,
  319,   -1,   -1,  322,   -1,   -1,  453,   -1,  455,   -1,
  329,   -1,   -1,  332,  407,   -1,   -1,  336,   -1,   -1,
   -1,  436,  341,   -1,   -1,  344,  441,   -1,   -1,   -1,
   -1,  424,   -1,   -1,  262,   -1,   -1,   -1,  453,   -1,
  455,  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,  319,   -1,  296,  322,  414,  299,   -1,   -1,   -1,
   -1,  329,   -1,   -1,  332,  498,   -1,   -1,  336,   -1,
   -1,   -1,   -1,  341,   -1,  273,  344,  436,   -1,  277,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,  453,   -1,  455,  340,  296,   -1,
   -1,  299,   -1,  346,  347,  348,   -1,  350,  351,  352,
   -1,  354,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,
  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,
   -1,  374,   -1,  376,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  340,   -1,   -1,   -1,  414,   -1,  346,  347,
  348,   -1,  350,  351,  352,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,  436,   -1,
   -1,   -1,   -1,  441,   -1,   -1,  374,   -1,  376,   -1,
   -1,  424,   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  446,   -1,   -1,   -1,   -1,   -1,  407,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  461,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,
  461,   -1,   -1,   -1,   -1,  498,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,  485,   -1,   -1,  488,  489,  490,
  491,   -1,  493,  481,  482,  496,  497,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  498,
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
"SIZE","SMALLINT","SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING",
"STATISTICS","SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION",
"TRIGGER","UNCOMMITTED","UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR",
"VARIABLE","VARYING","VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH",
"WORK","WRITE","FLOAT","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER",
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
"logfile_attr : SIZE equals long_integer",
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
"blob_segsize : SEGMENT SIZE unsigned_short_integer",
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
"u_numeric_constant : FLOAT",
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
static int yydebug;
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

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
    {\
    if ((to - string) >= MAX_TOKEN_LEN)        \
	yyabandon (-104, isc_token_too_long); \
    }
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
static BOOLEAN	long_int (DSQL_NOD, SLONG *);
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
static BOOLEAN	short_int (DSQL_NOD, SLONG *, SSHORT);
static void	stack_nodes (DSQL_NOD, DLLS *);
static int 	swallow_single_line_comment (void);
static int	yylex (USHORT, USHORT, USHORT, BOOLEAN *);
static void	yyabandon (SSHORT, STATUS);
static void	check_log_file_attrs (void);

static TEXT	*ptr, *end, *last_token, *line_start;
static TEXT	*last_token_bk, *line_start_bk;
static SSHORT	lines, att_charset;
static SSHORT	lines_bk;
static BOOLEAN	first_time;
static USHORT   param_number;


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
CONST TOK	*token;

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
    first_time = TRUE;
	param_number = 1;
#ifdef DEV_BUILD
    if (DSQL_debug > 10)
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
UCHAR	*p;

for (p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
    if (!(classes [*p] & CHR_DIGIT))
	return FALSE;

*long_value = atol ((char *)((STR) string)->str_data);

return TRUE;
}


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
    if (level > 100)
        *yydeb = level - 100;
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
UCHAR	*p;
SCHAR	buf [10];    
BOOLEAN return_value;

if (((STR) string)->str_length > 5)
    return FALSE;

for (p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
    if (!(classes [*p] & CHR_DIGIT))
	return FALSE;

/* there are 5 or fewer digits, it's value may still be greater
 * than 32767... */

buf [0] = ((STR) string)->str_data[0];
buf [1] = ((STR) string)->str_data[1];
buf [2] = ((STR) string)->str_data[2];
buf [3] = ((STR) string)->str_data[3];
buf [4] = ((STR) string)->str_data[4];
buf [5] = '\0';

*long_value = atoi (buf);

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


static int swallow_single_line_comment (void)
{
/**************************************
 *
 *	s w a l l o w _ s i n g l e _ l i n e _ c o m m e n t
 *
 **************************************
 *
 * Functional description:
 *  Discard single line comments (SLC) starting with --
 *  and takes care of end of input if necessary.
 *  There may be multiple consecutive SLC, multiple SLC
 *  separated by valid commands or by LF.
 *
 **************************************/
	SSHORT	c;
	
	if (ptr >= end)
		return -1;
	while (ptr + 1 < end) {
		c = *ptr++;
		if (c == '-' && *ptr == '-') {
			ptr++;
			while (ptr < end) {
				if ((c = *ptr++) == '\n') {
					lines++;
					line_start = ptr;
					break;
				}
				if (ptr >= end) {
					return -1;
                }
            }
		}
		else {
			--ptr;
			break;
		}
	}
	return 0;
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
UCHAR	*p, tok_class, string [MAX_TOKEN_LEN], *buffer, *buffer_end, *new_buffer;
SYM	sym;
SSHORT	c;
USHORT	buffer_len;

STR	delimited_id_str;

/* Find end of white space and skip comments */

/* CVC: Experiment to see if -- can be implemented as one-line comment. 
This is almost the same block than below in the loop when \n is detected,
but here we should "unget" the first character if there're not 2 hyphens.

if (first_time) {
	first_time = FALSE;
	if (swallow_single_line_comment() < 0)
		return -1;
}
*/

for (;;)
    {
    if (ptr >= end)
	return -1;
    
    c = *ptr++;

	/* Process comments */

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

    yylval = (DSQL_NOD) (MAKE_string (string, p - string))->str_data;

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
	    new_buffer = (UCHAR *) gds__alloc (2 * buffer_len);
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
	    yylval = (DSQL_NOD) MAKE_string (buffer, p - buffer);
	    delimited_id_str = (STR) yylval;
	    delimited_id_str->str_flags |= STR_delimited_id;
	    if (buffer != string)
		gds__free (buffer);
	    return SYMBOL;
	    }
	}
    yylval = (DSQL_NOD) MAKE_string (buffer, p - buffer);
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
	    yylval = (DSQL_NOD) MAKE_string ((UCHAR *) last_token,
					ptr - last_token);
	    last_token_bk = last_token;
	    line_start_bk = line_start;
	    lines_bk = lines;

	    return FLOAT;
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

		yylval = (DSQL_NOD) MAKE_string ((UCHAR *) last_token,
					    ptr - last_token);

		last_token_bk = last_token;
		line_start_bk = line_start;
		lines_bk = lines;

		if (client_dialect < SQL_DIALECT_V6_TRANSITION)
		    return FLOAT;
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
    yylval = (DSQL_NOD) MAKE_string (string, p - string);
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
            yydebug = yyn - '0';
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
        if (yydebug)
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
        if (yydebug)
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
                if (yydebug)
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
                if (yydebug)
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
        if (yydebug)
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
    if (yydebug)
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
{ prepare_console_debug ((int) yyvsp[0], &yydebug);
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
				  (int) e_cnstr_count, MAKE_string (NULL_STRING, 0), NULL, 
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
				(int) e_cnstr_count, MAKE_string (NULL_STRING, 0), NULL, 
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
case 305:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
break;
case 306:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 308:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
break;
case 309:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
break;
case 311:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 312:
{ yyval = make_node (nod_exec_sql, e_exec_vc_count, yyvsp[-1]); }
break;
case 315:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 316:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 317:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 318:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 319:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_label, e_label_count, yyvsp[-1], NULL); }
break;
case 335:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 336:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 337:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 338:
{ yyval = NULL; }
break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 348:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 349:
{ yyval = make_list (yyvsp[0]); }
break;
case 350:
{ yyval = make_list (yyvsp[-1]); }
break;
case 351:
{ yyval = NULL; }
break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_replace_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_mod_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 362:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 363:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 364:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 365:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 367:
{ yyval = make_list (yyvsp[0]); }
break;
case 369:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 373:
{ yyval = yyvsp[-1]; }
break;
case 374:
{ beginning = lex_position(); }
break;
case 375:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
			       (lex_position() == end) ?
			       lex_position()-beginning : last_token-beginning);}
break;
case 376:
{ beginning = last_token; }
break;
case 377:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
					lex_position()-beginning); }
break;
case 378:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string (NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 379:
{ yyval = 0; }
break;
case 380:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 381:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 382:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 384:
{ yyval = NULL; }
break;
case 385:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 386:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 404:
{ yyval = NULL; }
break;
case 405:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 406:
{ yyval = yyvsp[0]; }
break;
case 407:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 408:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 412:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 413:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string (NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 417:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 418:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = yyvsp[0]; }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 422:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 423:
{ yyval = yyvsp[0]; }
break;
case 424:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 426:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 427:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 429:
{ yyval = yyvsp[0]; }
break;
case 430:
{ yyval = yyvsp[0]; }
break;
case 431:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 432:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 433:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 434:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 459:
{ yyval = NULL; }
break;
case 460:
{ yyval = NULL; }
break;
case 461:
{ yyval = NULL; }
break;
case 462:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 463:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 464:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 465:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 466:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 467:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 468:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 469:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 471:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 472:
{ yyval = yyvsp[0]; }
break;
case 473:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 474:
{ yyval = yyvsp[0]; }
break;
case 475:
{ yyval = yyvsp[0]; }
break;
case 477:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 480:
{ yyval = NULL; }
break;
case 482:
{ yyval = NULL; }
break;
case 483:
{ yyval = yyvsp[0]; }
break;
case 484:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 500:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 501:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 503:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 504:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 505:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 511:
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
case 512:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 513:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 514:
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
case 515:
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
case 516:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 519:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 520:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 521:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 522:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 523:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 524:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 525:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 526:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 527:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 528:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 530:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 531:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 532:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 534:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 535:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 544:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 545:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 546:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 547:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
		    	}
break;
case 548:
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
case 549:
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
case 552:
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
case 553:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 554:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 555:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 556:
{ yyval = yyvsp[-1]; }
break;
case 557:
{ yyval = 0; }
break;
case 561:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 562:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 563:
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
{ yyval = (DSQL_NOD) NULL; }
break;
case 575:
{ yyval = (DSQL_NOD) NULL; }
break;
case 576:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 578:
{ yyval = (DSQL_NOD) NULL; }
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
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
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
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
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
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 630:
{ yyval = 0; }
break;
case 631:
{ yyval = yyvsp[0]; }
break;
case 632:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 633:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 634:
{ yyval = 0; }
break;
case 635:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 636:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 638:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 639:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 640:
{ yyval = 0; }
break;
case 641:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 642:
{ yyval = yyvsp[-1]; }
break;
case 643:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_top, 1, yyvsp[-2]);}
break;
case 699:
{ yyval = make_node (nod_top, 2, yyvsp[-4], yyvsp[-3]);}
break;
case 700:
{ yyval = 0;}
break;
case 701:
{ yyval = yyvsp[0]; }
break;
case 702:
{ yyval = 0; }
break;
case 703:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 704:
{ yyval = 0; }
break;
case 705:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 706:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 707:
{ yyval = 0; }
break;
case 708:
{ yyval = 0; }
break;
case 710:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 711:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 714:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 716:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 717:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 721:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 727:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 731:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 732:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 734:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 738:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 739:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 740:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 741:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 745:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 746:
{ yyval = yyvsp[0]; }
break;
case 748:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
break;
case 757:
{ yyval = make_list (yyvsp[-1]); }
break;
case 759:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 762:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 763:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 781:
{ yyval = yyvsp[-1]; }
break;
case 782:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 791:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 792:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 810:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 812:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 826:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 827:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 828:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 830:
{ yyval = make_list (yyvsp[-1]); }
break;
case 831:
{ yyval = yyvsp[-1]; }
break;
case 832:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 833:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 843:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 844:
{ yyval = yyvsp[0]; }
break;
case 845:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 846:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 847:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 848:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 849:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 850:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 851:
{ yyval = yyvsp[-1]; }
break;
case 852:
{ yyval = yyvsp[-1]; }
break;
case 856:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 857:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 858:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 859:
{ yyval = yyvsp[0]; }
break;
case 860:
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
case 861:
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
case 862:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 863:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 865:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 867:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 868:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 869:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 870:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 871:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 872:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 874:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
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
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DATE);
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
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIME); 
			}
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 878:
{ yyval = make_parameter (); }
break;
case 879:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 880:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 881:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 882:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 883:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 884:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 885:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 886:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 887:
{ yyval = yyvsp[0]; }
break;
case 888:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 890:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 891:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 892:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 893:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 894:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 896:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 897:
{ yyval = yyvsp[0];}
break;
case 902:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 903:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 904:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 905:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 906:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 907:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 908:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 909:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 911:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 913:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 914:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 915:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 917:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 920:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 923:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 924:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 928:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 929:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 930:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 931:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 932:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 933:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 934:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 938:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 939:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 940:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 941:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 942:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 943:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 948:
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
        if (yydebug)
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
            if (yydebug)
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
    if (yydebug)
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
