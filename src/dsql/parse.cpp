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
 */

#if defined(DEV_BUILD) && defined(WIN32) && defined(SUPERSERVER)
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
#if !defined(_JRD_ERR_PROTO_H_)
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
#define LEAVE 362
#define LEVEL 363
#define LIKE 364
#define LOG_BUF_SIZE 365
#define LSS 366
#define MANUAL 367
#define MAXIMUM 368
#define MAX_SEGMENT 369
#define MERGE 370
#define MESSAGE 371
#define MINIMUM 372
#define MODULE_NAME 373
#define NAMES 374
#define NATIONAL 375
#define NATURAL 376
#define NCHAR 377
#define NEQ 378
#define NO 379
#define NOT 380
#define NOT_GTR 381
#define NOT_LSS 382
#define NUM_LOG_BUFS 383
#define OF 384
#define ON 385
#define ONLY 386
#define OPTION 387
#define OR 388
#define ORDER 389
#define OUTER 390
#define OUTPUT_TYPE 391
#define OVERFLOW 392
#define PAGE 393
#define PAGES 394
#define PAGE_SIZE 395
#define PARAMETER 396
#define PASSWORD 397
#define PLAN 398
#define POSITION 399
#define POST_EVENT 400
#define PRECISION 401
#define PRIMARY 402
#define PRIVILEGES 403
#define PROCEDURE 404
#define PROTECTED 405
#define RAW_PARTITIONS 406
#define READ 407
#define REAL 408
#define REFERENCES 409
#define RESERVING 410
#define RETAIN 411
#define RETURNING_VALUES 412
#define RETURNS 413
#define REVOKE 414
#define RIGHT 415
#define RPAREN 416
#define ROLLBACK 417
#define SEGMENT 418
#define SELECT 419
#define SET 420
#define SHADOW 421
#define SHARED 422
#define SINGULAR 423
#define SIZE 424
#define SMALLINT 425
#define SNAPSHOT 426
#define SOME 427
#define SORT 428
#define SQLCODE 429
#define STABILITY 430
#define STARTING 431
#define STATISTICS 432
#define SUB_TYPE 433
#define SUSPEND 434
#define SUM 435
#define TABLE 436
#define THEN 437
#define TO 438
#define TRANSACTION 439
#define TRIGGER 440
#define UNCOMMITTED 441
#define UNION 442
#define UNIQUE 443
#define UPDATE 444
#define USER 445
#define VALUES 446
#define VARCHAR 447
#define VARIABLE 448
#define VARYING 449
#define VERSION 450
#define VIEW 451
#define WAIT 452
#define WHEN 453
#define WHERE 454
#define WHILE 455
#define WITH 456
#define WORK 457
#define WRITE 458
#define FLOAT 459
#define NUMBER 460
#define NUMERIC 461
#define SYMBOL 462
#define STRING 463
#define INTRODUCER 464
#define ACTION 465
#define ADMIN 466
#define CASCADE 467
#define FREE_IT 468
#define RESTRICT 469
#define ROLE 470
#define COLUMN 471
#define TYPE 472
#define EXTRACT 473
#define YEAR 474
#define MONTH 475
#define DAY 476
#define HOUR 477
#define MINUTE 478
#define SECOND 479
#define WEEKDAY 480
#define YEARDAY 481
#define TIME 482
#define TIMESTAMP 483
#define CURRENT_DATE 484
#define CURRENT_TIME 485
#define CURRENT_TIMESTAMP 486
#define NUMBER64BIT 487
#define SCALEDINT 488
#define CURRENT_USER 489
#define CURRENT_ROLE 490
#define KW_BREAK 491
#define SUBSTRING 492
#define RECREATE 493
#define KW_DESCRIPTOR 494
#define FIRST 495
#define SKIP 496
#define CONNECTION_ID 497
#define TRANSACTION_ID 498
#define BIGINT 499
#define CASE 500
#define NULLIF 501
#define COALESCE 502
#define USING 503
#define NULLS 504
#define LAST 505
#define ROWS_AFFECTED 506
#define LOCK 507
#define SAVEPOINT 508
#define STATEMENT 509
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
  192,  193,  201,  201,  194,  195,  195,  196,  204,  199,
  199,  199,  200,  200,  200,  202,  202,  202,  202,  197,
  208,  198,  198,  203,  203,  188,  188,  211,  212,  212,
  213,  213,  213,  213,   11,  215,  215,  215,  216,  216,
  216,  216,  216,  216,   72,   81,  221,  222,  218,  223,
  223,  223,  224,  227,  232,  232,  233,  233,  234,  234,
  148,  220,   88,   90,  219,  219,   71,   84,  237,  237,
  237,  238,  241,  241,  242,  242,  242,  242,  242,  242,
  242,  242,  242,  242,  242,  242,  242,  242,  242,  239,
  239,  240,    2,  244,  244,  244,  244,  244,  244,  244,
  252,  253,  250,  250,  254,  254,  254,  254,  254,  254,
  254,  245,  245,  255,  255,  255,  255,  255,  255,  255,
  249,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  260,  260,  260,  257,  257,  259,  259,  258,
  256,  256,  256,  251,  251,  247,  248,  248,  261,  261,
  261,  261,  262,  262,  246,  263,  263,  264,  264,    8,
  265,  265,  265,  265,  265,  265,  265,  265,  265,  265,
  265,  265,  102,  102,  145,  145,  268,  268,  271,  271,
  273,  273,   54,   54,  270,  270,  270,  270,  270,  270,
  270,  270,  270,  278,  278,  269,  269,  269,  269,  279,
  279,   62,   62,   62,   56,   56,  275,  275,  275,  272,
  272,  272,  283,  283,  283,  282,  282,  281,  281,  281,
  276,  276,  286,  284,  284,  284,  285,  285,  277,  277,
  277,  277,  287,  287,   19,   19,   19,  289,  289,  289,
   16,   17,  293,  293,    4,   15,  292,  292,  294,  294,
  295,  295,  288,  296,  296,  297,  297,  298,  298,  298,
  298,  299,  299,  300,  300,  301,  301,  303,  303,  303,
  304,  304,  304,  305,  305,  305,  302,  307,  307,  307,
  308,  308,  306,  306,  309,  311,  311,  310,  310,  290,
   18,  312,  312,  312,  313,  313,  316,  316,  317,  317,
   66,   66,   66,  318,  318,  318,  314,  314,  319,  319,
  320,  320,  315,  323,  321,  321,  321,  321,  324,  324,
  324,  325,  325,  325,  225,  225,  226,  226,  327,  327,
  328,  328,  328,  322,  331,  331,  332,  332,  333,  333,
  334,  334,  335,  335,  206,  206,  337,  337,  235,  235,
   32,  236,  236,  236,  236,  236,  236,  236,  236,  229,
  229,  338,  338,  339,  339,  339,  339,  339,  341,  341,
  341,  230,  230,  228,  228,  345,  345,  345,  231,  231,
  346,  347,  347,  347,  347,  347,  348,  348,  349,  349,
  350,  350,  351,  351,  351,  352,  352,   10,   10,  354,
  354,    7,    7,  355,  356,  357,   20,   20,  358,  359,
  360,  360,  189,  329,  329,    3,    3,  362,  362,  362,
  364,  364,   62,  363,  363,  365,  365,   35,   35,   86,
   85,   85,  353,  353,  366,  367,  367,  207,  207,  207,
  147,  361,  361,  112,  112,  112,  112,  369,  369,  369,
  369,  369,  369,  369,  369,  369,  369,  369,  370,  370,
  370,  370,  370,  370,  370,  370,  375,  375,  375,  375,
  375,  375,  375,  375,  375,  375,  375,  375,  375,  375,
  375,  375,  381,  381,  371,  371,  372,  372,  372,  372,
  373,  373,  377,  377,  378,  378,  378,  378,  376,  379,
  374,  374,  382,  382,  383,  380,  385,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,  157,  157,  157,  386,  384,  384,
  152,  152,  391,  391,  391,  391,  391,  388,  388,  388,
  388,  388,  217,  153,  153,  154,  155,  155,  390,  390,
  390,   52,   52,   21,   21,  243,  392,   55,  124,  274,
  274,   96,  387,  387,  387,  387,  393,  393,  393,  393,
  393,  393,  393,  393,  393,  393,  393,  394,  342,  343,
  343,  343,  340,  340,  389,  344,  344,  396,  396,  397,
  397,  398,  398,  401,  401,  399,  399,  404,  404,  403,
  400,  402,  395,  395,  395,  395,  395,  395,  395,  395,
  326,  326,  156,   49,  280,  123,  149,  368,  166,  210,
  267,   64,   61,  214,  113,   67,  330,  209,   36,   42,
  336,  266,   40,   43,  205,   41,  291,
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
    3,    1,    2,    1,    2,    2,    1,    3,    4,    2,
    6,    4,    1,    1,    7,    8,    6,    4,    2,    1,
    3,    0,    2,    4,    0,    1,    1,    3,    3,    6,
    2,    2,    2,    3,    0,    1,    2,    4,    1,    3,
    2,    2,    2,    1,    4,    1,    3,    0,    1,    1,
    1,    3,    3,    3,    7,    7,    7,    7,    1,    1,
    3,    4,    8,    2,    1,    3,    1,    1,    6,    3,
    0,    0,    0,    0,    3,    0,    9,    9,    1,    1,
    0,    2,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    5,    5,    5,    5,    5,    5,    2,
    0,    4,    2,    3,    3,    2,    2,    3,    3,    2,
    3,    6,    1,    2,    4,    3,    2,    2,    2,    2,
    3,    1,    3,    3,    3,    2,    2,    4,    4,    5,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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
    2,    5,    1,    0,    3,    2,    1,    0,    2,    0,
    1,    0,    3,    1,    0,    1,    2,    1,    1,    1,
    1,    2,    2,    1,    2,    3,    1,    1,    3,    3,
    1,    2,    3,    1,    2,    0,    2,    1,    1,    0,
    1,    1,    1,    3,    2,    3,    0,    1,    3,    4,
    3,    1,    3,    4,    3,    0,    1,    3,    4,    4,
    1,    1,    0,    2,    2,    0,    4,    0,    2,    0,
    2,    0,    9,   10,    2,    1,    1,    0,    2,    4,
    2,    2,    4,    2,    1,    1,    1,    1,    1,    3,
    1,    2,    3,    2,    1,    3,    1,    1,    6,    3,
    3,    2,    3,    0,    1,    3,    1,    1,    1,    2,
    1,    1,    1,    2,    1,    2,    1,    2,    0,    3,
    0,    1,    3,    1,    1,    1,    1,    3,    1,    1,
    1,    2,    0,    2,    0,    3,    5,    0,    2,    0,
    4,    1,    2,    1,    1,    0,    1,    3,    2,    1,
    1,    2,    1,    4,    2,    1,    3,    8,    5,    1,
    3,    1,    1,    4,    4,    4,    1,    1,    5,    5,
    1,    3,    3,    1,    1,    7,    7,    5,    3,    0,
    1,    1,    1,    2,    0,    1,    1,    1,    0,    3,
    1,    3,    1,    0,    3,    1,    3,    1,    3,    3,
    1,    1,    3,    1,    3,    3,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    1,    1,    5,    6,    3,    4,    5,    6,
    3,    4,    3,    4,    3,    4,    4,    5,    4,    4,
    3,    4,    1,    3,    3,   10,   10,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    2,    3,    3,
    3,    3,    3,    3,    3,    3,    1,    1,    1,    1,
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
    1,    1,    1,    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  555,
  556,  557,    0,  612,  722,  723,  727,  728,  466,    0,
    0,    0,    0,    0,    0,  403,  567,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  886,    0,   22,  884,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  480,    0,    0,   38,   35,   37,    0,   36,
    0,  960,    0,    0,    0,    0,   33,   69,   67,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  637,    0,    0,    0,  962,  669,    0,    0,
    0,    0,    0,  108,  967,  561,    2,    0,    0,    0,
    0,  450,  432,  433,  434,  435,  436,  437,  438,  439,
  440,  441,  442,  443,  444,  445,  446,  447,  448,  449,
  451,  452,  453,  454,  455,    0,  431,  952,    0,  956,
    0,  410,  959,    0,  407,    0,  671,  963,    0,  406,
    0,  565,  882,    0,  151,  104,    0,  948,  105,    0,
  218,  761,    0,  955,  103,  146,    0,    0,   99,    0,
  106,  888,    0,  100,    0,  101,  966,    0,  102,  147,
  107,  621,  622,    0,  887,  885,    0,  953,   75,    0,
    0,  951,  488,  481,    0,  487,  492,  482,  483,  490,
  484,  485,  486,  491,    0,   31,    0,   40,  748,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  892,    0,  873,  639,  641,
    0,  642,  644,  941,  645,    0,  646,  635,    0,    0,
    0,    0,    0,    0,    0,  584,  573,    0,  576,  578,
  579,  580,  581,  587,  588,    0,  961,  670,    0,  109,
    0,  110,    0,  111,    0,    0,  613,    0,  611,    0,
    0,    0,    0,  467,    0,    0,  373,    0,  221,    0,
  413,  404,  464,  465,    0,    0,    0,    0,    0,    0,
  422,    0,  379,  380,    0,  571,  569,  883,    0,    0,
    0,    0,    0,    0,    0,  154,  133,    0,   97,    0,
    0,  112,    0,  122,  121,    0,    0,    0,    0,    0,
    0,  944,   76,    0,    0,    0,  724,  725,  489,    0,
  943,  865,  864,  863,    0,    0,  866,  867,    0,    0,
  869,  350,  351,  345,    0,  349,  861,  868,    0,  751,
    0,    0,   28,    0,    0,  964,    0,   72,   70,   68,
   34,    0,    0,    0,  753,    0,   53,    0,    0,    0,
    0,    0,  563,    0,    0,    0,    0,  851,  879,    0,
    0,  853,    0,    0,  880,    0,  874,    0,    0,  855,
  856,  857,  875,  876,    0,  877,  878,    0,    0,    0,
  881,    0,    0,    0,    0,    0,    0,  758,  847,  848,
  849,  854,  833,    0,  832,    0,  836,  895,  896,  835,
  829,  830,  831,  834,  850,  893,  894,  916,  917,  920,
  921,    0,  648,    0,  735,    0,    0,  649,    0,    0,
  610,    0,  585,    0,  582,    0,  583,  608,    0,  603,
    0,    0,  577,    0,  762,  731,    0,    0,    0,    0,
    0,    0,  543,    0,    0,    0,  617,    0,  614,    0,
  186,    0,    0,  130,  472,  173,  174,    0,  470,    0,
    0,    0,    0,  473,    0,  468,    0,    0,  417,  419,
  418,    0,  420,    0,  414,  219,    0,    0,  272,    0,
    0,    0,    0,  426,  427,    0,  457,    0,    0,    0,
    0,    0,    0,    0,  384,  383,  476,    0,    0,  150,
    0,    0,  159,    0,  158,    0,  165,  148,    0,  163,
  166,  167,  155,    0,    0,  511,  547,  514,    0,  548,
    0,    0,  515,    0,    0,    0,  538,  551,  510,  533,
  512,  513,  508,  495,  373,  493,  494,  496,    0,    0,
  505,  506,  507,  509,    0,    0,    0,    0,    0,  113,
    0,  114,    0,  125,    0,    0,  202,    0,    0,  371,
    0,  221,    0,    0,   84,    0,    0,  743,    0,    0,
    0,    0,    0,    0,    0,    0,  764,  768,  769,  770,
  771,  772,  773,  774,  775,  776,  777,  870,  871,  872,
  862,    0,    0,    0,  750,   29,    0,    0,   45,   73,
    0,    0,   27,    0,  756,    0,    0,    0,  719,    0,
    0,    0,    0,    0,  562,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  965,  319,    0,    0,
    0,    0,    0,    0,    0,  640,    0,    0,  643,    0,
    0,    0,    0,  957,  652,  559,    0,  890,  558,    0,
  586,    0,  594,  590,  589,    0,    0,    0,  605,  593,
    0,    0,  729,  730,    0,    0,    0,  371,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  131,    0,    0,
    0,    0,    0,    0,  373,  416,    0,  373,  374,  421,
  503,    0,  274,    0,  268,  270,  371,  949,  241,    0,
    0,  373,    0,    0,  210,  371,    0,    0,    0,    0,
  246,  240,  243,  244,  245,  425,  462,  461,  424,  423,
    0,    0,    0,    0,    0,    0,  382,    0,  373,    0,
  156,  160,    0,  164,    0,    0,  534,  535,  552,    0,
  549,    0,    0,  541,  539,  540,    0,    0,    0,    0,
  504,    0,    0,    0,    0,  542,    0,    0,  371,    0,
  201,    0,  204,  206,  207,    0,    0,    0,    0,    0,
  221,   79,    0,    0,   78,  945,  522,  523,    0,    0,
    0,  767,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  347,  353,  354,  352,  752,    0,    0,    0,
    0,    0,    0,    0,    0,   54,   62,   64,    0,    0,
   71,    0,    0,    0,  755,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  933,  934,  935,
  936,  937,  938,  939,  940,    0,    0,    0,    0,    0,
    0,  926,    0,    0,    0,  667,    0,    0,  845,  846,
  914,    0,    0,  947,  841,    0,    0,    0,    0,    0,
    0,  852,  760,  759,    0,    0,    0,    0,  657,  658,
    0,    0,  650,  653,  560,  891,  595,  604,  599,  598,
    0,  609,  763,  732,  733,  371,    0,    0,  216,    0,
    0,  618,    0,    0,  627,  182,  176,  183,    0,  179,
    0,    0,  178,    0,  189,  170,  169,  889,  172,  171,
  474,    0,    0,    0,  415,  273,    0,  212,  373,  214,
    0,  374,  209,  215,    0,    0,    0,    0,  428,  429,
  373,  371,  374,    0,    0,    0,  400,    0,  197,  198,
  157,    0,    0,    0,    0,    0,    0,  550,    0,    0,
  373,  374,    0,  499,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  199,    0,    0,
    0,    0,  360,    0,   83,    0,    0,    0,    0,   88,
   85,    0,    0,    0,  950,  726,    0,    0,  778,    0,
    0,    0,  804,  803,    0,    0,    0,  811,  823,  821,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  766,    0,   66,   58,   59,   65,   60,   61,    0,    0,
   23,    0,    0,    0,   43,    0,    0,    0,  737,  757,
    0,  720,    0,  736,    0,    0,    0,    0,    0,    0,
  221,    0,  897,    0,    0,  912,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  922,    0,
  932,    0,    0,    0,    0,    0,    0,  913,  858,    0,
    0,    0,    0,    0,    0,  672,    0,    0,    0,    0,
    0,  601,  602,  606,    0,  200,    0,    0,  619,  620,
  631,    0,    0,    0,    0,    0,    0,  190,    0,    0,
  411,  224,  225,  226,  227,  228,  229,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  458,
  459,  430,    0,    0,    0,    0,    0,    0,  373,  478,
  374,  946,  168,    0,    0,  517,    0,  516,  553,    0,
  545,    0,    0,    0,  497,    0,  525,    0,    0,  527,
  530,  532,  371,    0,    0,    0,  120,    0,  205,  373,
    0,    0,  372,    0,   98,    0,  118,    0,   94,    0,
    0,    0,    0,   87,    0,  819,  820,    0,    0,    0,
    0,    0,    0,  822,    0,    0,    0,    0,    0,    0,
    0,  812,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   57,   63,   55,
   56,   24,  741,  742,    0,  739,  746,  747,  744,    0,
    0,  718,    0,    0,  903,  902,    0,    0,  899,  898,
    0,  907,  906,  905,  904,  901,  900,    0,    0,  928,
    0,    0,    0,  927,    0,    0,    0,  665,    0,    0,
  660,    0,  661,    0,  678,  674,  676,    0,    0,    0,
    0,    0,  372,  624,  625,    0,  184,  180,    0,    0,
  195,  191,    0,  374,  280,    0,    0,  374,  278,    0,
    0,    0,  232,    0,    0,  248,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  475,  519,    0,  520,    0,
  135,    0,    0,    0,  139,  141,  142,  143,  500,  502,
  498,  529,    0,    0,  374,    0,    0,    0,    0,  355,
    0,  361,  373,   89,    0,    0,   91,    0,    0,    0,
    0,    0,    0,  825,  824,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   41,    0,    0,  721,  220,  915,  908,
  909,    0,  910,  924,  923,    0,  929,  918,    0,  919,
    0,  663,    0,    0,  685,    0,  682,  686,  687,  689,
  690,  691,    0,    0,  633,  374,  356,    0,  175,  192,
  374,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  302,  304,  307,  267,    0,  208,  233,    0,    0,    0,
    0,  234,  237,    0,    0,  374,  398,  399,  395,  394,
  397,  396,    0,  518,  546,  144,  132,  140,    0,  373,
  264,  127,  374,    0,    0,  375,  362,    0,    0,   92,
   93,   80,    0,  790,  798,    0,  791,  799,  794,  802,
    0,    0,  792,  800,  793,  801,  787,  795,  788,  796,
  789,  797,  738,    0,    0,  925,  666,    0,    0,    0,
    0,  702,  704,    0,  699,    0,  265,  185,  412,    0,
  310,    0,    0,  313,  314,    0,  306,    0,    0,  333,
    0,  305,    0,  332,  293,  300,    0,  303,  286,    0,
  291,    0,  336,  292,  211,  235,    0,    0,  238,    0,
  250,  252,  402,  374,    0,  377,    0,    0,    0,  367,
  368,    0,    0,   77,   95,    0,    0,  911,    0,    0,
  688,  683,  703,    0,    0,  283,    0,  308,    0,    0,
    0,    0,    0,  301,    0,    0,  326,  327,  344,    0,
    0,    0,    0,  339,  337,  239,    0,    0,    0,    0,
    0,  266,  374,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  710,    0,    0,    0,  282,  279,  309,
    0,    0,    0,  312,    0,    0,    0,  318,    0,  343,
  954,  342,  341,    0,    0,    0,    0,    0,  249,    0,
  256,    0,  255,  378,  370,    0,    0,    0,  119,    0,
    0,    0,  712,  701,    0,    0,  713,    0,  709,    0,
    0,    0,    0,    0,    0,    0,    0,  329,  328,  338,
  340,  236,    0,    0,  260,  259,  258,    0,  363,    0,
  634,  827,  708,    0,  715,  321,    0,    0,  311,    0,
    0,    0,  330,  263,  261,  262,    0,    0,    0,    0,
    0,  334,  315,    0,    0,    0,    0,  714,  324,  316,
  826,  717,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1505,   27,   28, 1506,
   30,   31,   32,   33,   34,   35,   36, 1507,   38, 1508,
  618,  103,  382,  874, 1121,  104,  648,  875,  105,  387,
  653,  128,  106,  107,  238,  946,  117,  876,  877,  179,
  208,  108,  878,  109,  389,   79,  219,  353,  436,  613,
 1058,  371,  834,  584,  493,  811,  614,  615,  616, 1060,
  220, 1313,   71,  169,   72,  214, 1779, 1265,  199,  204,
  206,  209,  195,  186,  189,  201,  211,  134,  290,  292,
  294,  342,  600,  602,  379,  239,  761,  518,  625,  995,
  346,  605, 1046, 1257,  551,  708, 1021,  503,  504,  533,
  338,  585, 1032, 1403,  730, 1201, 1404, 1405, 1406, 1407,
 1408,  626,  196,  187,  334,  558,  335,  336,  559,  560,
  561,  562, 1233,  989,  506,  507,  979,  977,  508,  983,
  978, 1192, 1367,  984,  985, 1197,  348,  822,  823,  824,
  825,  763, 1002, 1381,  586,  765,  438,  827,  935,  527,
 1328, 1202,  439,  440,  441,  926,  442, 1382, 1383,  536,
 1532, 1679, 1218,  771,  772,  759,  773,  774,  775, 1680,
 1681, 1756,  316,  531, 1209, 1509,  175,  528,  529, 1210,
 1211, 1376, 1657, 1510, 1511, 1512, 1513, 1622, 1514, 1515,
 1516, 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1702, 1743,
 1606, 1666, 1745,  443,  688, 1347,  444,    0,    0, 1066,
 1623, 1673, 1674, 1712,  374,  375,  445, 1051, 1263, 1420,
    0,    0, 1052, 1053,  266,  466, 1555,  357, 1181, 1361,
 1485, 1638, 1639, 1640, 1641, 1179,  325,  547,  789, 1230,
  548,  787,  202,   56,  320,  180,  141,  303,  166,  310,
  172,  749,  746,  311,  321,  779,  322,  544, 1013,  167,
  304,  515,  549, 1231,   93,  177,  223,  587,  588,  751,
 1033,  752, 1034, 1035,  591,  592,  593,  594, 1026,  838,
  595,  596,  597,  804,  598,  495,  801,   40,   41,   42,
  136,   58,  404,  182,  327,  277,  278,  279,  280,  281,
  282,  283,  284,  285,  714,  479,  961, 1184,  480,  481,
  719,   43,  140,  299,   44,  496,  497, 1189,  735,  975,
  121,  701,  659,  122,  123,  267,  467,  468,  469,  705,
  947,  948,  949,  950, 1173,  446, 1161, 1476, 1477,  447,
 1479,  448,  449,  450,    0, 1694, 1596, 1695, 1696, 1697,
 1739, 1780,  394, 1133,   45,   46,  358,   47,   48,  488,
  489,  883, 1129, 1315, 1319,  395,  656,  192,  627,  628,
  629,  630,  631,  632,  633,  634,  635,  636,  637, 1282,
 1076, 1078, 1079,  933,  686,  451,  452,  453,  454,  455,
  378,  216,  456,  457,  916,  458,  459,  460,  461,  678,
  920, 1162, 1157,  679,
};
static short yysindex[] = {                                   3960,
  853,    8, 3697,  162, 1523,  368, 3713,  324, 1664,  562,
  467, 1375,    8,  903,  -79,  376, -117,  439,    0,  709,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  369,    0,    0,    0,    0,    0,    0, 8641,
  461,  534,  541,  376,  588,    0,    0,  655, 1456,  624,
  461,  636,  931,  541,  784,  376,  588,    0,  792,  812,
    0,  997,    0,  819,    0,    0,  961,  837,    0,  376,
  908,  461, 1090,  837,  636,  534,  541,  784,  376,  588,
  792,  812,    0,  541, 1017,    0,    0,    0, 1415,    0,
 1415,    0,  930, 1077,  170, 1436,    0,    0,    0,  624,
  376,  624, 1129, 1137, 1171,  106, 1552, 1122,  168,  173,
   58, 1076,    0,  636, 1238, 1063,    0,    0, 1172, 1148,
  541,  376,  792,    0,    0,    0,    0, 1343,  -69, 1306,
  545,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  623,    0,    0, 1456,    0,
   -3,    0,    0, 1624,    0, 1270,    0,    0,   85,    0,
 1203,    0,    0, 1184,    0,    0, 1217,    0,    0, 1417,
    0,    0, 1456,    0,    0,    0, -114, 1624,    0,  601,
    0,    0, 1406,    0, 1418,    0,    0, 1415,    0,    0,
    0,    0,    0, 1468,    0,    0, 1282,    0,    0, 1453,
 1359,    0,    0,    0, 1282,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1948,    0,  624,    0,    0,    0,
 1088, 1419,  933,  812, 1250, 1494, 1818, 1547, 1544, 1088,
 1480,  933, 1503, 1508, 1390,    0, 5647,    0,    0,    0,
 5647,    0,    0,    0,    0, 3937,    0,    0, 1474,  534,
 1543, 1469,  739,  376, 1489,    0,    0, 1063,    0,    0,
    0,    0,    0,    0,    0, 1471,    0,    0, 1624,    0,
 1406,    0, 1415,    0,  869, 1522,    0, 1501,    0, 1189,
 1600, 1084,  545,    0, 1659,  851,    0,  624,    0,  623,
    0,    0,    0,    0,  624, 1551,  -91, 1497,  -63, 1916,
    0, 8711,    0,    0, 1548,    0,    0,    0, 1911, 1911,
 1456, 1603, 1456, 1688, 1217,    0,    0, 8598,    0,  541,
  588,    0, 1551,    0,    0, 1694,  912, 1942,  376, 1722,
  534,    0,    0, 1952,  197, 1708,    0,    0,    0, 1527,
    0,    0,    0,    0, 1536, 1539,    0,    0, 1625, 1322,
    0,    0,    0,    0, 1964,    0,    0,    0,  256,    0,
  376, 1574,    0,  541, 1553,    0,   41,    0,    0,    0,
    0,  376, 1471, -115,    0,  376,    0, 1692,  541, 1984,
 1088, 1627,    0,  439, 1990, 1993, 1996,    0,    0, 1999,
 2001,    0, 2003, 2012,    0, 2015,    0,    0, 2021,    0,
    0,    0,    0,    0, 2028,    0,    0, 4471, 2030, 2034,
    0, 5647, 5647, 4651, 1619, 2042, 1814,    0,    0,    0,
    0,    0,    0, 1998,    0, 2044,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1868,    0, 2331,    0, 1770, 2049,    0,  -66,   89,
    0, 1154,    0,  -81,    0,  -81,    0,    0, 2051,    0,
  400, 1667,    0,    0,    0,    0, 2052,  125, 2038, 1551,
 2062, 1840,    0, 1823, 1823, 2061,    0, 2060,    0, 1723,
    0,  146, 1760,    0,    0,    0,    0, 1456,    0, 1911,
 1911, 1911, 1911,    0, 2066,    0, 1833, 1844,    0,    0,
    0, 1816,    0, 8598,    0,    0, 8598, 1601,    0, 1624,
 1859, 1666, 4960,    0,    0,  476,    0, 1666, 1283, 1270,
    0, 1725, 1691, 1661,    0,    0,    0,  -74, 1735,    0,
 1678,  784,    0, 1456,    0, 1872,    0,    0, 1688,    0,
    0,    0,    0,  193, 1702,    0,    0,    0, 1703,    0,
 1757, 2115,    0, 1810, 2119,   -2,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2071,  192,
    0,    0,    0,    0,  200, 2124, 2125, 2119, 1624,    0,
 1847,    0, 1909,    0, 1456, 1456,    0,  -91,   85,    0,
 1792,    0, 1761, 2132,    0, 8448,  172,    0, 1788, 1794,
 2140, 2357, 2142, 1860, 4303,   24,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1651, 1322,  624,    0,    0, 1014, 1745,    0,    0,
 1719,  933,    0, 1881,    0, 1734,  903, 2162,    0, 1881,
 1014, 1882, 1886,  541,    0,  645, 4554,  702,  636, 5647,
  987,  992, 1048, 1791, 5647, 2357, 2331, 1759,  751, 5647,
 4554,  855,  855,  903, 2073, 2172,    0,    0, 4734, 1755,
 5647, 5647, 5647, 5647, 5647,    0, 5647,  131,    0,  104,
 1765, 4554, 1769,    0,    0,    0, 1018,    0,    0,  -40,
    0, 1771,    0,    0,    0,  376, 1141,  376,    0,    0,
  624, 1471,    0,    0, 4554, 1958,  -91,    0, 1755,  997,
  997,  869,  158,  624, 1767, 1456, 1456,    0,  -84, 1678,
 1678, 1764,  784, 1084,    0,    0, 2193,    0,    0,    0,
    0, 1959,    0,  624,    0,    0,    0,    0,    0, 1961,
 2197,    0, 1937, 1954,    0,    0, 1896, 1897, 1415, 1965,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  784,  624, 8532, 1870, 1873, 1874,    0,  784,    0, 1538,
    0,    0, 1826,    0,   82, 1829,    0,    0,    0,  784,
    0, 2115,  179,    0,    0,    0, 1972,  179, 1856,  179,
    0, 2220,  784,  784,  784,    0, 1551,  376,    0, 1920,
    0, 1798,    0,    0,    0, 1548, 1861,  376, 1809,   88,
    0,    0, 2242, 2010,    0,    0,    0,    0,  197, 1825,
 1869,    0, 1869, 3749,  124, 5647, 5647, 2537, 2251, 1300,
 2620, 5647, 2868, 1005, 3112, 3198, 5005, 3379, 3460, 3709,
 2357, 2357,    0,    0,    0,    0,    0, 1553,  541,  588,
 1553,  792,  812,  134, 2248,    0,    0,    0, 1014, 1907,
    0,  -65, 1930, 1471,    0,   58, 4554, 1930, 2259, 2248,
 1014,  638, 1985, 5647, 5647, 2045, 5647, 2275, 5647, 2269,
 2099, 5647, 5647, 5647, 5647, 5647, 5647,    0,    0,    0,
    0,    0,    0,    0,    0, 2004, 1206,  -11, 5647,  755,
 4554,    0, 2357, 1754, 2331,    0, 2282,   58,    0,    0,
    0, 2331, 1835,    0,    0, 2047,  855,  855, 1197, 1197,
  884,    0,    0,    0,  104, 2289, 2287, 1324,    0,    0,
 2357, 2005,    0,    0,    0,    0,    0,    0,    0,    0,
  682,    0,    0,    0,    0,    0, 1839, 1861,    0, 1828,
 1828,    0, 2295, 1876,    0,    0,    0,    0, 1845,    0,
 1911, 2074,    0,  -84,    0,    0,    0,    0,    0,    0,
    0, 2311, 2357, 1766,    0,    0, 2054,    0,    0,    0,
 1766,    0,    0,    0, 1415, 1415, 1865, 2335,    0,    0,
    0,    0,    0,  322,   27, 1164,    0, 2113,    0,    0,
    0, 1918,  162, 1853, 1957, 1959, 2343,    0, 1678, 1858,
    0,    0,  989,    0, 2330, 1918, 1043,  784, 2350, 2351,
 2352, 2130,   85, 2054, 1911, 1760,  -91,    0, 1735,   58,
 1939, 1962,    0,  150,    0,  784, 2000, 2088, 8448,    0,
    0,  784, 1905, 2095,    0,    0, 2365, 2369,    0, 1589,
 2331, 2373,    0,    0, 2331, 2374, 5102,    0,    0,    0,
 2063, 2377, 2331, 2378, 1662, 2381, 2331, 2382, 5647, 5647,
 2251, 5647, 5282, 2384, 2331, 2387, 2388, 2331, 2389, 5647,
 2331, 2392, 2331, 2394, 2397, 2331, 2398, 2399, 2331, 2401,
    0, 2180,    0,    0,    0,    0,    0,    0, 2116, 1014,
    0, 1014,  134, 2248,    0,  205,  205,  182,    0,    0,
 3937,    0, 1908,    0, 2259, 2248, 2403,  638, 2255, 2411,
    0, 2422,    0, 2438, 5647,    0, 2495, 2542, 2697, 2882,
 2894, 3025, 5647,  784, 4554, 2331, 2008, 4554,    0, 5647,
    0, 2139,  529, 5647, 4554, 5647, 5647,    0,    0, 1324,
 2409, 4554, 1148,  104, 2065,    0, 2067, 2070, 2117, 2192,
 2138,    0,    0,    0, 2054,    0, 1939,  667,    0,    0,
    0, 2048, 1456, 2079, 1678, 2089, 1678,    0, 2357,  381,
    0,    0,    0,    0,    0,    0,    0, 2036, 2215, 2054,
    0, 5647,    0, 2200, 2080, 1865,  997,    0, 2357,    0,
    0,    0, 2102, 2104, 2105, 2106, 2107, 2120,    0,    0,
    0,    0,    0, 2466,  162,    0, 1764,    0,    0,  179,
    0, 1766, 2129,  179,    0,  179,    0, 1959, 2472,    0,
    0,    0,    0, 1548, 2215, 1678,    0, 1760,    0,    0,
 3937, 2239,    0,  -14,    0, 2295,    0, 2478,    0, 2480,
 1456,   50, 2481,    0, 1456,    0,    0, 5647, 2108, 2108,
  903, 2482, 1913,    0, 2108, 2108, 5647, 2108, 2108, 2969,
 2331,    0, 1683, 5647, 2331, 2108, 2108, 2108, 2108, 2331,
 2108, 2108, 2108, 2108, 2108, 2108, 2137,    0,    0,    0,
    0,    0,    0,    0, 2090,    0,    0,    0,    0, 1770,
 4554,    0,  638, 2403,    0,    0, 8598, 2488,    0,    0,
 3053,    0,    0,    0,    0,    0,    0, 3096,   90,    0,
 4554, 2221, 2094,    0, 4554, 3171, 1929,    0, 1295, 2331,
    0, 1935,    0, 1324,    0,    0,    0,  104, 1372, 2357,
 2135, 2215,    0,    0,    0, 1911,    0,    0, 1456, 1538,
    0,    0,  677,    0,    0,  624, 5549,    0,    0, 2331,
 1823, 2200,    0,  964,  376,    0, 2196,  718, 2101, 2204,
 2111, 2249, 2211, 2250, 2054,    0,    0, 2516,    0, 2517,
    0, 2206, 1823, 2129,    0,    0,    0,    0,    0,    0,
    0,    0, 2054, 1735,    0, 1538, 2113, 2240, 2173,    0,
 1861,    0,    0,    0, 2194, -104,    0, 1959, 2198, 2331,
 2524, 2532,   58,    0,    0, 2533, 2538, 2331, 2549, 2550,
 5647, 5647, 2331, 2551, 2556, 2557, 2560, 2561, 2562, 2563,
 2564, 2565, 2567,    0,  205, 1765,    0,    0,    0,    0,
    0,  784,    0,    0,    0, 4554,    0,    0, 4554,    0,
 1765,    0, 1108, 2329,    0, 2566,    0,    0,    0,    0,
    0,    0,   24,  656,    0,    0,    0, 1678,    0,    0,
    0,    0, 8598,  163,  759, 2553, 1522, 2574, 2274, 2559,
 5647, 2571, 2580, 2577, 2583, 2586, 2280, 2587,    0,    0,
    0, 2313, 6395, 2589,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2585,    0,    0, 2297, 2306,  376,
 1865,    0,    0, 1415,  534,    0,    0,    0,    0,    0,
    0,    0, 2215,    0,    0,    0,    0,    0, 2215,    0,
    0,    0,    0,  144, 1765,    0,    0, 5647, 1456,    0,
    0,    0, 1456,    0,    0, 5647,    0,    0,    0,    0,
 2331, 2331,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2005, 2615,    0,    0, 2005, 2357, 1755,
 1372,    0,    0, 2291,    0, 2617,    0,    0,    0,   92,
    0, 5346,  541,    0,    0, 5647,    0, 2318, 2357,    0,
 2596,    0, 2357,    0,    0,    0,  183,    0,    0,  958,
    0, 2209,    0,    0,    0,    0, 1865, 1415,    0, 2279,
    0,    0,    0,    0, 2113,    0,  144, 2622, 1324,    0,
    0, 2005, 2331,    0,    0, 1295, 2138,    0, 2138,   24,
    0,    0,    0,  977, 1766,    0, 2608,    0, 3124, 5583,
 3183,  183,  805,    0,  826,  266,    0,    0,    0,  461,
 2207,  162,  359,    0,    0,    0, 2279,  443, 1865, 2283,
 2285,    0,    0, 1324, 2630,  144, 2336, 2138, 2632, 1765,
 2135, 2135,  376,    0, 2633, 2631,  -54,    0,    0,    0,
 4035, 2267, 2638,    0,  191, 2252, 2380,    0,  183,    0,
    0,    0,    0, 5549,  958, 1865,  856,  856,    0, 2385,
    0, 2246,    0,    0,    0, 1324,  144, 2135,    0, 2005,
 2303, 2303,    0,    0,  977, 2654,    0,  534,    0, 2073,
 1950,  154, 2636, 2404, 2395, 5549, 5549,    0,    0,    0,
    0,    0, 2238,  433,    0,    0,    0, 1568,    0, 2138,
    0,    0,    0,  534,    0,    0,  183, 2655,    0, 1825,
 5549, 2396,    0,    0,    0,    0, 2357, 2135, 2661, 2665,
 1956,    0,    0, 5549,   24, 2303,  534,    0,    0,    0,
    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  115, 1176,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  123, 2014,    0,    0,    0,    0,    0, 2709,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  775,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2372,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2334,    0,    0,    0,  261,    0,
  261,    0,    0,    0,    0, 2337,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  894,    0,    0,
 4118, 2272,    0,    0,    0,  983,    0,    0,    0,  785,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  309,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   -6,    0,    0,    0,    0,  211,    0,
 1034,    0,    0,    0,    0,    0, 1412,    0,    0, 8682,
    0,    0,    0,    0,    0,    0,    0,   -6,    0,  379,
    0,    0, 2674,    0,    0,    0,    0, 2451,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1044,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1078,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1299,    0,    0,    0,
    0,    0,    0,    0, 2254,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   95,    0,    0, 1085,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -6,    0,
 2674,    0, 2451,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1309,    0, 2442,    0,    0,    0,    0, 1349,
    0,    0,    0,    0,    0, 2457,  710, 8742,    0, 1392,
    0,    0,    0,    0,  383,    0,    0,    0, 2263, 2263,
    0,    0,    0, 1422, 2007,    0,    0,    0,    0,    0,
    0,    0, 2457,    0,    0, 1476,    0,    0,    0,    0,
    0,    0,    0, 8416, 2339,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1438,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1452,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1461,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6115,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6298,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  120,    0,    0, 2405,    0,  174,    0,
    0,    0,    0, 1022,    0, 1022,    0,    0,  374,    0,
  454,  993,    0,   99,    0,    0,    0, 1044,    0, 2457,
    0,    0,    0,  111,  111,  735,    0,    0,    0,  112,
    0, 2161,  201,    0,    0,    0,    0,    0,    0, 2263,
 2263, 2263, 2263,    0,  351,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2463,
    0,    0,    0,    0,    0, 2442,    0,    0,  888,    0,
  411, 2256,    0,    0,    0,    0,    0,    0,  832,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1507,    0,
    0,    0,    0,  910, 1116,    0,    0,    0, 1263,    0,
    0, 1458,    0,    0, 1542,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2505, 2593,
    0,    0,    0,    0, 1607, 1320,    0, 1542,   -6,    0,
    0,    0,    0,    0,    0,    0,    0,  710, 1566,    0,
    0,    0,    0, 2333,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  522,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  279,    0,    0, 2014,    0,    0,  279,
    0,    0,    0,    0,    0, 5827,    0, 5827,    0,    0,
 5827, 5827, 5827,    0,    0,    0, 2288,    0,    0,    0,
    0, 7016, 7154, 4373,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1435,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  772,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  710,    0,    0,   48,
   48,    0,    0,    0,  328,    0,    0,    0, 2328,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  761,    0,    0,    0,    0,    0,    0,    0, 2703,
    0,    0, 2572, 2145,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  413,  424,  652,    0,    0,    0, 2006,
    0,    0,    0,    0,    0, 1658,    0,    0,    0,    0,
    0, 1458,    0,    0,    0,    0,  898,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2457,    0,    0,  321,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 8566,
    0,    0,    0,   91,    0,    0,    0,    0, 2437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1509, 1526,    0,    0,    0,    0,    0,
    0,    0, 1532,    0,    0, 4118,    0, 1532, 1569, 1593,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  600,    0,    0, 5827,    0,    0,
    0,  244,    0,    0,    0, 6481, 7310, 7448, 6657, 6840,
    0,    0,    0,    0,    0, 2234,  973, 1169,    0,    0,
    0, 1102,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  341,
  341,    0,  122,    0,    0,    0,    0,    0,    0,    0,
 2263, 2290,    0, 3113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2492,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1565,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1602,    0,    0,
    0,    0,    0,    0,    0, 1649,    0,    0,    0,    0,
    0,    0,    0,    0, 1050,    0,    0,    0,    0,    0,
    0,    0, 1566, 2492, 2263, 1638,  710,    0, 2485, 4118,
 1656,   78,    0,    0,    0,    0, 8566,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2862,    0,    0,    0, 2949,    0,    0,    0,    0,    0,
    0,    0, 3672,    0, 5362,    0, 5881,    0,    0,    0,
    0,    0,    0,    0, 6931,    0,    0, 7529,    0,    0,
 7562,    0, 7604,    0,    0, 7734,    0,    0, 7755,    0,
    0, 8296,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1509, 1682,    0, 2314,  306,    0,    0,    0,
    0,    0,    0,    0, 1696, 1721, 1749,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2316,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2407,
 1235,    0, 8338,    0, 2421,    0, 2423, 2424,    0,    0,
  760,    0,    0,    0, 2492,    0, 1656,    0,    0,    0,
    0, 3149,    0,    0,    0, 2317,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2304,    0, 2500,
    0,    0,    0, 3004,    0, 1565, 2372,    0,    0,    0,
    0,    0,  721,  816,  859,  866,  868,  870,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  830,    0,    0,    0,    0, 2899,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1756,    0,    0,
    0,    0,    0,    0,    0, 1762,    0,    0,    0,    0,
    0,  126,    0,    0,    0,    0,    0,    0,    0,    0,
 4373,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 7776,    0, 7893,    0, 7954,    0,    0,    0,    0, 7965,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1787,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  897,
    0,    0,    0, 1428,    0,    0,    0,    0,    0,    0,
  175,    0,    0,    0,    0, 2263,    0,    0,    0,  915,
    0,    0,    0,    0,    0,    0, 2467,    0,    0, 2738,
 1626, 3032,    0, 2442,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2492,    0,    0,    0,    0,    0,
    0,    0, 1789, 1275,    0,    0,    0,    0,    0,    0,
    0,    0, 2492, 2485,    0,  349,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  188,    0, 7988,
    0,    0, 5827,    0,    0,    0,    0, 8104,    0,    0,
    0,    0, 8137,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  306, 1201,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  563,    0, 2407,  899,    0,  814,    0,    0,    0,    0,
    0,    0,  845, 2742,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2475,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2987,    0,    0,   47,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  397,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 8179, 8221,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  375,    0,    0,    0,  715,    0,    0,
    0,    0,    0, 2747,    0,    0,    0,    0,    0, 2729,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2486,    0,    0,    0,    0, 2987,  570,    0,   80,
    0,    0,    0,    0,    0,    0,    0,  458,  423,    0,
    0,  129, 2738,    0,    0,    0,  289,    0,  282, 8388,
    0,    0,    0, 2742,    0,    0,    0,    0,    0,   68,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  151,    0, 1565,  596,
  668,    0,    0, 2407, 1319,    0,    0,  117,    0,  563,
  295,  100, 1071,    0,    0, 2748,    0,    0,    0,    0,
    0, 2732,   79,    0, 2489,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2987,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  528,    0,  157,    0,  715,
  922, 2755,    0,    0, 2742,    0,    0,    0,    0, 2754,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2407,    0,  282,
    0,    0,    0,    0,    0,    0,    0, 2740,    0,    0,
    0, 5944,    0,    0,    0,    0,    0,  100, 2760,    0,
    0,    0,    0,    0, 1803, 2755,    0,    0,    0,    0,
    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2803,    0,    0, 2805,
    0,    0,    0,    0,    0,    0,    0,    9,    0, 2807,
    2,  701,  -23, -552, 1685,  776, -216, -392, 2797, 2558,
    0,  -35,    0, 2569,  -94,  -41,    0,  671, -859,  -33,
  -47,  -37, -192, 2568, 2159,    0,    0,    0, 1379,    0,
    0,  -51, 1772, -516,   -7, -837, 2203, 1992, -279, 1768,
 2745, -272,    0,  -57,    0, -637,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -562,  113, 1778, -269, -156, -929,
    0,    0,    0,    0, -308, -105, -220, 1780, -316,  -43,
    0, -496,    0,    0, -468, -939,    0, 1420,    0,    0,
 2530, -568,  -42,    0,    0,    0,    0, 2501,    0, 2278,
 2548, -155, 1802, -662,    0,    0,    0, -697,    0,    0,
    0,    0,    0,    0, 1867,    0, 2575, 2126, 1820, 2539,
 2552,    0, 1265,    0, -492, 2110,  -60, -653, -700,    0,
    0,  -55, -935, -931, -930, -224, -922,    0, 1486, -984,
    0, 1193,-1139, 1488,    0, 2341,    0,    0,    0, 1194,
 1204, 1159,  510, -294, -338, -324,    0,    0, 2134,    0,
 1676,    0,    0, -387,    0,    0,    0,    0, -124,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -416,    0, -351,    0,-1104, -285,    0,    0, 1119,
 1268,    0, 1180,    0,    0, 2521,  -99, 1932, 1710, 1535,
    0,    0,    0,-1015, -841, -867,    0, -464, -771, -465,
 -303,    0, -741, 1264,  301, -401, -511, -724, -924,-1198,
    0,    0,    1,    0,    0,    0,    0,    0, 2582,    0,
    0,    0,    0, 2592, 2375,    0,    0,    0,    0,    0,
 2609,    0,    0,    0,    0,  -15,    0,    0,    0, -312,
 2109, -310, 1674, -431,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2322,    0,-1203, 2128,    0,    0,    0,
 2518, 2915,    0,    0,    0,    0,    0, 2656,    0,    0,
    0,    0, 2460,    0, 2462,    0,    0,    0, 2217,    0,
    0,    0,   42,    0,  213,    0, 2210, 1974,    0,    0,
 -588, -522,  988,    0, 2824, 1376,    0, 2253, -606, 2247,
    0, -829, 2009,    0,    0, -127, -665,    0, 1360,-1196,
    0,-1174,-1106,-1085,    0, 1472,    0, 1218,    0, 1266,
    0, 1170,    0,    0,    0,    0, 2473,    0,    0,    0,
 -326, 2300, 2075, -978,    0,    0,    0, 1081,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2611,
 1166, 1871,    0, -567,    0,    0,    0, -222,    0,    0,
 2598,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -86, 1805,    0,
};
#define YYTABLESIZE 9240
static short yytable[] = {                                     191,
  130,  171,  288,  193,   76,   75,  240,  185,   37,  494,
  373,  174,  377,  259,  262,  927,  190,  557,  176,  260,
  263,  552,  198,  723,  224,  589,  731,  590,  969,  524,
  203,  750,  210,  205,  753,  228,  762,  522,  709,  980,
  764,  465,  227,  233, 1131,  229,  749,  623,  603,  246,
  388,  248,  235,  842,  234,  845,  232,  200, 1203,  388,
  896, 1213, 1204, 1205,  130, 1203,  655, 1352,  886, 1204,
 1205, 1207, 1214,  231,  968,  247, 1386,  359, 1207,  257,
  230,  269,  619, 1222,  652,  293, 1166,  749,  623,  289,
  749,  623,  970,  971,  591,  928,  291,  826,  948,  835,
  437, 1049, 1243,  997,  462,  749,  623,  918,  889,  464,
  217,  630, 1004,  350,  568, 1170,  693,  312,  965,  734,
  257,  629,  568,  257, 1260, 1023,  322, 1057,  681,  941,
 1463,   86, 1024,  707,   86,  376,  359,  320,  257,  948,
  700,  339,  948,  945,  961,  373,  514,  377, 1316,  244,
  257,  217, 1655,  591,  217, 1475,  700,  948,  487,  948,
  734,  486, 1478,  734, 1069, 1044,   90,  498,  722,  217,
  630,  973,  943,  568,  700,  693,  380, 1120,  734,  372,
  629,  568,  662, 1637, 1480,  737,  738,  681, 1238,  237,
  296,  257,  650, 1767,  257,  726,  532,  703,  492,  943,
  469,  740,  741,  742,  743,  383,   74,  257, 1261,  257,
  381,  435,  261,  244,  383,  700,   74,  651, 1553,  471,
  589, 1601,  590, 1029,  538,  485,  398,  784,  526, 1384,
  258,  526,  795,  700, 1709,  258,  952,   74,  478,  813,
  435,   74,  557,  474,  258, 1421,  124,  523, 1422,   74,
  861, 1560, 1481,  313,  526, 1126,  191,  269,  539,  469,
 1309,  542, 1311, 1320,  785,  487,  770,  258,  890,  381,
  376,  677,  981, 1482,  805,  682,  683,  685,  740,  553,
 1132,  555,  810, 1736,  859,  861,  131,  859,  693,  340,
 1011,  796, 1111, 1112,  700,  607,  645,  712,  599,  644,
  611, 1396, 1401,  657,  749,  524, 1203,  601,  628, 1709,
 1204, 1205, 1185,  609,  372,  807,  129,  264,  132, 1207,
  126, 1737,  693, 1426, 1708,  341, 1123,  632, 1225,  693,
  658,  314,  485,  133, 1738,  700,  859,  740, 1135, 1137,
  626,  323,  649,  806, 1354,  646,  982,  693,  196,   14,
  471,  297,  125,  700, 1163,   76,  654,  649, 1221,  126,
  660,  265,  948,  948,  524,  383,  623,  628,  713,  786,
  188, 1030, 1127,  597,  681,  217,  862,  663,  948,  126,
  221,  626,  477,  734,  626,  861,  632, 1475,  623, 1561,
  616, 1629, 1001, 1418, 1478,  704,  695, 1384,  188,  626,
  476,  948, 1715,  948, 1050, 1557,  269,  196, 1462,  471,
 1411,  862,  387,  217,  736,  681, 1480,  948,  865,  324,
  377, 1374,  365,  385, 1200, 1155,  252,  942,  257,  217,
  257,  749,  597,  681,  760,   90, 1683,  591,  257,  948,
  734,  477,  465,  718, 1492,  790,  494,  893, 1524,  616,
 1525,  217,  630,  607, 1744,  695,  739,  364,  469,  388,
  681,  526,  629, 1309,   57, 1317,  365,  844,  809,  381,
 1226,  387,  766,  591,  381,  992, 1583,  465,  994,  322,
  381,  365,  385, 1056, 1481, 1551, 1124, 1676,  700, 1550,
  320, 1266, 1000,  700,  651, 1368,  651,  526, 1136, 1348,
  465,  591,  792,   86,  591, 1482, 1348,  469,  499, 1283,
  464,  862,  607,  901,  693,  700,  364, 1427,  917, 1018,
  591,  694, 1042,  924,  925,  568,  681,  366, 1473, 1414,
  257, 1254,  932,  359,  936,  937,  938,  939,  940, 1719,
  932,  988,  835,  866,  791,  464,  591,  191,  256,  749,
 1059,  623,  257,  820,  821, 1703, 1597, 1130,  693,  257,
  568, 1599,  694,  700,   76,  173, 1064,  630,  464,  749,
  681,  366,  693,  734, 1399,  706, 1752,  629,  356, 1387,
  694,  734,  257,  867,  681, 1324,  366,  864,  991, 1119,
 1562, 1566,  188,  257,  487,  253, 1741,  964,  700, 1413,
  526,  956,  948,  695,  498,  127, 1632,  243,  471,  381,
  749,  188,  700,  749,  217,  484,  700,   76,  837,  188,
  469,   73,  649, 1636,  168,  617,  900,  256,  749,  617,
 1373,   73,  256,  836,  986,  987,  253,  617,  256,  253,
  668,  988,  861,  668,  484,  749,  477,  740,  812,  628,
 1388,  386,   73,  257,  253,  526,   73,  471,  668,  626,
 1223,  485,  465, 1714,   73,  123,  191,  254,  632,  126,
  693, 1489, 1195,  380,  524, 1113,  387,  693, 1116,  693,
  478,  626,  962,  700,  976,  976,  693,  385,   80, 1070,
 1071, 1075, 1433,  526, 1083, 1085, 1087,  196, 1095, 1098,
 1101, 1103, 1106, 1109, 1682, 1255,  681,  343,  254,  616,
  386,  254,  114, 1689, 1457, 1698,  597, 1491,  717, 1203,
  392, 1010, 1012, 1204, 1205,  695,  254,   94,  695, 1212,
  464,  616, 1207, 1775,  615,  990, 1256, 1139, 1140,  112,
 1142, 1220, 1144,  898, 1717, 1147, 1148, 1149, 1150, 1151,
 1152,  365,  597, 1724,  365,  681,  487,  138, 1536,  693,
  526, 1242, 1156,  681,  925, 1224,  679,  137,  862, 1206,
  471,  377,  681, 1009,  570,  615, 1206, 1059,  377,  392,
  597,  477, 1043,  597,  671, 1776,  364,  115, 1017,  364,
  861, 1483, 1054,  615,  695,  767,  607, 1456,  490,  597,
 1027,  526,  300, 1587,  526, 1039, 1040, 1041, 1400,  948,
  139,  387, 1647,  680, 1410,  393, 1649,  253,  693,  526,
  365,  526,  385,  485, 1117,  597, 1471, 1114,  671,  137,
 1458,  401,  607,  570,  110, 1118, 1115,  127,  695,   76,
  694,  123,  123,  671,  692, 1706, 1362,  749,  450,  749,
  694,  301,  695,  694,  680,  364,  366,  749,  389,  366,
  607, 1327,  694,  607,  365,  388, 1707,  391,  344,  390,
 1688,  679,  680,  253,  393,  253,  365,  768,  365,  607,
  305, 1007,  948,  253, 1378,  692, 1718,  463,  137, 1651,
  401,  695,  254,  566,  695, 1684,  694,  136,  684,  364,
  135,  695,  111,  692,  264,  607,  465,  668,  668,  524,
  694,  364, 1526,  364,  196,  386,  862,  389,  769,  694,
  932,  616,  168,  956,  388,  366,  391, 1167,  390,  306,
 1415,  463, 1290, 1291, 1547, 1293, 1295,  860,  861,  684,
  860,  738,  684, 1300, 1726,  254,  463,  254,  894,  749,
  524,  695,  566,  524,  749,  254,  136,  684, 1760, 1395,
  695,  264,  616,  694,  302, 1345,  868,  345,  524,  366,
  524,  749,  654,  196,  464,  253, 1169,  694,  749,  861,
  616,  366,  575,  366,  392, 1758,  191,  242, 1331,  860,
 1417, 1584,  592,  380, 1348,  170, 1338,  253,  925, 1592,
 1600,  925,  173, 1156,  253,  897, 1588, 1346,  925, 1349,
 1350,  668,  749,  654,  589,  925,  590, 1206,  526,  377,
 1650,  596,  474,   76, 1234, 1593, 1314, 1314, 1318,  242,
 1249,  654, 1244,  572,  526, 1348,  465, 1486,  253,  756,
 1663,  575,  307,  695, 1665, 1353,  681,  254, 1269,  178,
  386,  592,  668,  615, 1273, 1380, 1543, 1488,  921,  922,
  308,  526, 1158, 1159,  862,  181,  861,  526, 1340,  254,
  526, 1342,  749, 1474, 1549,  615,  254,  348,  693,  393,
  596, 1245,  871, 1594,  574,  188, 1244,  861, 1182, 1370,
 1642, 1372,  572,  501,  309,  401,  465,  194,  253,  386,
  693,  681,  695,  681,  464,  862,  671,  373,  817,  137,
  254,  242,  681,  671,  770,  536,  671, 1215, 1216,  392,
  671, 1430,  389, 1690,  475, 1621,   76,   76,  671,  388,
 1438,  391,  680,  390,  690, 1248,  348, 1443,  520, 1183,
  691,  976,  501,  574,  671,  680, 1339,   49,  693, 1490,
 1416,  521,  242, 1558,  680,  536,  536,  693,   50,  536,
  681, 1364, 1603,  692,  464,   51, 1267,  524,  655,  671,
  254, 1365,  196,  526,  536,  136,  536,  136,  600,  476,
  526, 1691,  671, 1692,  925,  692,  524,  524,  925,  524,
   52,  197,  862,  196,  524, 1552,  477,  524,  526,  671,
  695,  693,  680,  923,  671, 1604,  536, 1160, 1785,  655,
  524,  680,  655,  862,  393,  196,  524,  684, 1633, 1425,
 1669,  196, 1728, 1429, 1634, 1730,  671,  655,  526,  600,
  684,  498,  526,  692, 1753,   76, 1398, 1687,  671,  684,
  671,  695,  692,   73,  196, 1705,  264,  694,  692,  487,
  693,  264,  695,  207, 1464,  680,   53,  389, 1467,  695,
  606,  212,  537,  196,  388, 1667,  391, 1605,  390,  680,
 1670,  196,  196,  102,  138, 1754, 1089,  136,  215,  196,
 1635, 1671, 1687,  217, 1571, 1572,  692,  684,   54,  524,
  902,  654,   55, 1090, 1778,  904,  684,  196,  218,  213,
  692,  654,  537,  537,  654, 1474,  537,  264,  408,  925,
 1667,  524,  925,  654,  241,  191,  485,  976,  524,  531,
 1592,  537, 1755,  537, 1687, 1768, 1750,  524,   73,  524,
  484, 1668, 1493,  138,  196,  592,  694,  692, 1091,  693,
  684,  695,  868, 1528, 1611,  196, 1593,  524,  409, 1534,
 1781,  906,  524,  537,  684, 1314, 1687, 1748, 1772, 1773,
  531,  654,  510,  531,  596, 1529,  369,  408, 1092,  222,
  654,  592, 1530,  536,  183,  184, 1668,  385,  531, 1586,
  531,  524, 1598, 1783,  258,  487, 1672, 1731, 1732,  536,
 1667,  405,  536,  536,  386,  536, 1790,  119,  120,  592,
  536, 1643,  592,  536, 1594,  271, 1531,  409,  711, 1646,
  531,  152,  225,  511,  654, 1667,  536,  869,  592,  236,
  681,  161,  536, 1749, 1759,  536,  654,  656,  596, 1175,
 1206,  596,  377,  681,  695, 1093, 1602,  346,  127, 1630,
  116,  272,  681, 1176,  592, 1659,  711,  596,  512, 1661,
  405,   44,  485,  870,  237,   76, 1668,  554,  871,  711,
   52,  242, 1585,  498,  872, 1227,  513, 1177,  656,  273,
  152,  656,  274,  596, 1786,  386,  690,  256,  116,  245,
  161, 1668,  691,  873, 1631,  690,  656,  655,  275,  498,
  681,  691, 1589,  695, 1628,  536,  346,  655,  554,  681,
  655,  554, 1228,  925,  955, 1608,  162, 1644,   42,  655,
   44, 1645,  679,  116,  276,  249,  554,  536,  554,   52,
  537,  250, 1178,  381,  536,   25, 1154,  317,  536,  695,
  318,  745,  695, 1677,  498,  536,  537,  501,  130,  537,
  537,  544,  537,  681, 1740,  959,  502,  537,  554,  127,
  537,   96,  373,  536,  138,  251,  657,  655,  536,  255,
  710, 1660,  960,  537,  251,  162,  655,   42,   48,  537,
  657,  120,  537,  329,  690,  270,  319,  531,  657,  275,
  691,  498,  544,  536,   25,  544,  487,  536,   98,  695,
  745,  286,   50,  531,  657,  354,  531,  531,  695,  531,
  544,  479,  544,  359,  531,  251,  528,  531,  251,  287,
  655,  330, 1710,  331,  498,  700,  295,  360,  487,  487,
  531,  130,  655,  251,  298,  217,  531,   48,  326,  531,
  694,  692,  544,  693,   95,  695,  332,  128, 1693,  498,
  367,  755,  537,  487,  754, 1175,  328,  528,  526,  657,
  528,   50, 1080,  485,  367,  376,  487,  521,   99, 1176,
  479,  333,  367,  315,  537,  528,  217,  528,  100,  217,
  130,  537,   76, 1713,  361,  537,   96, 1693,  367, 1081,
  337,   26,  537, 1177,  217,  485,  485, 1765,   97,  526,
  152,  863,  526,  101,  643,   49,  128,  528,  521,  531,
  537,  521,  113,  694,  692,  537,  693,  526,  695,  526,
  485,  130,  152,   98,  376,  554,  521,  754,  521, 1693,
   51,  531,  347,  485,  694,  692,  411,  693,  531,  695,
  537,  554,  531,  367,  537,  554,  349,  554, 1178,  531,
   26,  152,  554,  352,  754,  554,  656,  624,   46,  777,
  432,  778,  433,  695,   49,  129,  656,  531,  554,  656,
  152,  117,  531,  695,  554,  435,  695,  554,  656,  152,
  258,  679, 1761, 1762,  885,  695,  152,  884,  944,   51,
  362,  363,  364,   99,  183,  184,   47,  531,  217,  355,
 1308,  531, 1310,  100,  152,  694,  692, 1164,  693,  544,
  695,  963,  369,  365,  366,  351,  545,   46,  367,  368,
  369,   95,  356,  944,  129,  544,  656,  546,  101,  544,
  117,  544,  384,  695,  381,  656,  544, 1791, 1067,  544,
 1068,   73,  695,  418,  392,  381,  102,  554, 1048,   77,
   78, 1047,  544,  369,  419,   47,  369,  217,  544, 1055,
 1278,  544,  831,   96,  696,  694,  692,  393,  693,  554,
  695,  369,  397,  425,  528,   97,  554,  396,  690,  656,
  554,  428,  429,  430,  691, 1168,  695,  554, 1167, 1186,
  528,  656, 1047,  399,  528, 1194,  528,  401, 1193, 1175,
   98,  528,  402, 1236,  528,  554, 1235,  403, 1241,  624,
  554, 1240,  432, 1176,  433,  472,  526,  528,  699,  694,
  692,  470,  693,  528,  695,  521,  528,  435,  183,  184,
  473,  544,  258,   95,  482,  554,  526, 1177,  526,  554,
 1019, 1020,  484,  526,  521,  521,  526,  521,  124,  124,
   14,  690,  521,  544,  500,  521,  517,  691, 1322,  526,
  544, 1321, 1777, 1435,  544,  526, 1167,  509,  521,  540,
   99,  544,  690,  530,  521,   96,  510,  537,  691, 1470,
  100,  550, 1469, 1287,  405, 1472,  554,   97, 1469,  544,
  604,  608, 1178,  406,  544,  610,  528,  370,  556,  638,
 1766,  612,  369, 1469, 1442,  101, 1789,  407,  639, 1709,
  620,  640,   98,  360,  408,  196,  153,  643,  528,  544,
  258,  647,  661,  544,  386,  528, 1084,  511, 1088,  528,
 1096, 1099,  621, 1104, 1107, 1110,  528,  652,  526,  666,
  664,  409,  667,  690,  410,  668,  501,  521,  669,  691,
  670,  895,  671,  899,  528,  502,  903,  905,  907,  528,
  526,  672,  512,  638,  673,  638,  638,  526,  638,  521,
  674,  360,  411,  412,  196,  153,  521,  675,  526,  680,
  513,  638,   99,  681,  528,  413,  638,  521,  528,  414,
  687,  689,  100,  362,  363,  364,  526,  622,  697,  698,
  700,  526,  702,  690,  716,  521,  720,  721,  725,  691,
  521,  727,  729,  728,  732,  733,  734,  101,  501,  744,
  745,  367,  368,  929,  694,  692,  748,  693,  361,  695,
  526,  747,  757,  781,  369,  102,  405,  758,  782,  521,
  623,  369,  783,  788,  369,  406,  415,  256,  369, 1146,
  694,  692,  416,  693,  493,  695,  369,  690,  793,  407,
  797,  798,  417,  691,  800,  360,  408,  799,  803,  802,
  177,  808,  369,  814,  815,  818,  362,  363,  364,  418,
  183,  184,  819,  830,  621,  831,  828,  840,  839,  841,
  419,  843,  879,  409,  880,  493,  410,  369,  493,  365,
  366,  420,  421,  422,  367,  368,  423,  424,  882,  425,
  369,  887,  891,  493,  426,  427,  892,  428,  429,  430,
  417,  919,  930,  431,  411,  412,  934,  369,  951,  177,
  957,  966,  974,  988,  362,  363,  364,  413,  183,  184,
  704,  414,  993,  664,  998,  809,  999, 1001,  760,  622,
 1005, 1006, 1008,  360,  369, 1022, 1025,  365,  366,  420,
  421,  422,  367,  368,  423,  424,  369, 1014,  369, 1038,
 1015, 1016,  426,  427,  908,  909,  910,  911,  912,  913,
  914,  915, 1031,  638,  664, 1036, 1045,  664,  684, 1050,
  638, 1062,  623, 1063,  196,  153, 1065,  657,  415,  638,
 1077, 1122,  664, 1125,  416, 1325,  694,  692, 1128,  693,
  361,  695, 1120,  638,  417, 1138,  196,  153, 1141,  638,
  638,  636, 1145,  636,  636, 1143,  636,  638,  362,  363,
  364,  418,  183,  184, 1153, 1165,  690,  187, 1172,  636,
 1174, 1188,  419, 1180,  636,  196,  153,  638,  644, 1196,
  638,  365,  366,  420,  421,  422,  367,  368,  423,  424,
 1199,  425,  690, 1208,  196,  153,  426,  427,  691,  428,
  429,  430,  196,  196,  153,  431,  638, 1217,  638,  638,
  196,  153,  694,  692, 1219,  693, 1229,  695,  690, 1232,
 1237,  638, 1191, 1239,  691,  638,  187, 1246,  196,  153,
 1250, 1251, 1252, 1253, 1262, 1056,  624, 1271, 1274,  432,
  196,  433,  196, 1264, 1275, 1276,  362,  363,  364, 1277,
  183,  184, 1279, 1280,  435, 1284, 1285, 1286,  177,  258,
 1288, 1289,  493, 1296,  493,  196, 1297, 1298, 1299,  365,
  366, 1301,  493, 1302,  367,  368, 1303, 1304, 1305,  177,
 1306,  861,  638, 1307, 1341,  493, 1323, 1344,  638, 1351,
  196, 1326,  694,  692, 1355,  693, 1356,  695,  638, 1357,
 1358,  177, 1329,  694,  692, 1359,  693,  177,  695, 1360,
 1369, 1366,  638,  638,  638,  638,  638,  638, 1330,  694,
  692, 1371,  693, 1375,  695, 1377,  638,  532, 1385, 1389,
  177, 1390, 1391, 1392, 1393,  638,  638,  638,  638,  638,
  638,  638,  638,  638,  503,  638, 1397, 1394, 1402,  177,
  638,  638, 1412,  638,  638,  638, 1419, 1423,  177,  638,
 1424, 1428, 1434, 1454,  493,  177, 1281, 1455, 1459, 1465,
 1466,  636, 1484, 1535,  690, 1332,  694,  692,  636,  693,
  691,  695, 1538,  177, 1537,  503,  493,  636,  503, 1541,
 1540, 1542,  664,  493, 1539,  664, 1544, 1545, 1546, 1556,
 1554,  636,  664,  503, 1564,  664, 1559,  636,  636,  664,
 1563,  223, 1565, 1567,  664,  636,  434,  664, 1568,  432,
  177,  433, 1333,  694,  692,  187,  693,  493,  695, 1569,
 1570, 1573,  526,  664,  435,  636, 1574, 1575,  636,  258,
 1576, 1577, 1578, 1579, 1580, 1581,  187, 1582, 1590, 1591,
  690, 1607,  223, 1609,  111,  223,  691, 1610,  664, 1613,
 1617, 1619,  664,  405,  636, 1625,  636,  636,  187, 1612,
  223,  664,  406,  526,  187, 1614,  526,  694,  692,  636,
  693, 1615,  695,  636, 1616, 1618,  407, 1624,  664, 1626,
 1627,  526,  360,  408, 1664, 1648, 1654,  187, 1662,  434,
 1653, 1620,  432, 1678,  433, 1686, 1699, 1720, 1711, 1722,
 1725,  621, 1729, 1734, 1735,  664,  187,  435, 1742, 1727,
  409, 1469,  258,  410, 1747,  187, 1717,  664, 1746, 1718,
  690,  138,  187, 1764, 1769,  664,  691, 1770, 1709, 1771,
  636,  690, 1774, 1784, 1787, 1788,  636,  691,    1,  623,
  187,  411,  412,  203,  749,  564,  636,  690,   30,  373,
  271,   32,  149,  691,  413,  647,  269,  460,  414,  524,
  636,  636,  636,  636,  636,  636,  622, 1334,  694,  692,
  931,  693,  213,  695,  636,   82,  524,  187,  401,  193,
  679,  524,  930,  636,  636,  636,  636,  636,  636,  636,
  636,  636,  276,  636,  677,  281,  673,  675,  636,  636,
  275,  636,  636,  636,  690,  289,  194,  636,  374,  623,
  691,  706,  503,  287,  503,  415,  705,  223,  707,  503,
  325,  416,  503,  335,  288,  616, 1072,  668,  323, 1073,
  716,  417,   26,  405,   29,  503,   39, 1312,  116,  400,
  881,  390,  406,  391,  829,  362,  363,  364,  418,  183,
  184,  690, 1061, 1548, 1270, 1258,  407,  691,  226,  419,
 1272, 1268,  360,  408,  519,  563,  794, 1247,  365,  366,
  420,  421,  422,  367,  368,  423,  424,  505,  425,  223,
 1198,  223,  967,  426,  427,  534,  428,  429,  430,  223,
  409,  813,  431,  410, 1656,  491, 1259, 1527,  535, 1716,
  526, 1533,  526, 1003, 1723,  690, 1757,  526,  776, 1082,
  526,  691, 1073, 1721,  503, 1379,  405,  996, 1782, 1675,
  642,  411,  412,  526, 1751,  406, 1363, 1487,  526, 1187,
 1685,  525,  813,  543,  413,  813,  503,  434,  414,  407,
  432,  516,  433,  503,  780,  360,  408, 1409, 1037,  816,
  813,  665, 1335,  694,  692,  435,  693,  118,  695, 1028,
  258,  711,  958,  483, 1336,  694,  692,  715,  693,  526,
  695,  972,  526,  409, 1190,  268,  410,  503,  782,  954,
 1652,  223, 1763, 1171,  953, 1595, 1792,  526, 1733,  888,
  724, 1292, 1134, 1074, 1343,  415,  641,    0,    0,    0,
    0,  416,  526,  223,  411,  412,  690,    0,    0,    0,
  223,  417,  691,    0,    0,    0,  251,  413,    0,  782,
    0,  414,  782,    0,  526,  362,  363,  364,  418,  183,
  184,  526,    0,  230,    0,    0,    0,  782,    0,  419,
  694,  692,    0,  693,  223,  695,    0,    0,  365,  366,
  420,  421,  422,  367,  368,  423,  424,  251,  425,    0,
  251,  231,    0,  426,  427,  526,  428,  429,  430,    0,
    0,    0,  431,    0,  230,  251, 1074,  230,  415,    0,
    0,    0,    0,    0,  416,    0,    0,    0,    0,    0,
    0,    0,  230,    0,  417, 1337,  694,  692,    0,  693,
    0,  695,  231,    0,    0,  231,    0,    0,  362,  363,
  364,  418,  183,  184,    0,    0,    0,    0,    0,    0,
  231,    0,  419, 1460,  694,  692,    0,  693,    0,  695,
    0,  365,  366,  420,  421,  422,  367,  368,  423,  424,
    0,  425,  188,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,  813,    0,  431,    0, 1086,    0,    0,
 1073,    0,    0,    0,  405,    0, 1461,  694,  692,    0,
  693,    0,  695,  406,    0,    0,    0,    0,  181,    0,
    0,  434,    0,    0,  432,    0,  433,  407,    0,    0,
    0,  690,    0,  360,  408,  694,  692,  691,  693,  435,
  695,  188,    0,  690,  258,    0,  526,    0,  526,  691,
  813,    0, 1700,  813,    0,    0,  526,    0,    0,  181,
  813,  409,  181,  813,  410,    0,    0,  813,    0,  526,
    0,    0,  813,    0,    0,  813,    0,  181,    0,    0,
  782, 1468,  694,  692,    0,  693,    0,  695,    0,    0,
    0,  813,  411,  412,  694,  692,    0,  693,    0,  695,
 1441,    0,    0,    0,    0,  413,    0,  434,    0,  414,
  432, 1704,  433,    0,    0,    0,  813,    0,  690,  813,
  813,    0,    0,    0,  691,  435,    0,    0,    0,  813,
  258,    0,    0,    0,  251,    0,  251,  782,    0,    0,
  782,    0,    0,    0,  251,    0,  813,  782,  526,    0,
  782,  242,    0,  230,  782,    0,    0,    0,    0,  782,
    0,    0,  782,    0, 1074,    0,  415,    0,  813,    0,
  526,    0,  416,  813,  690,    0,    0,  526,  782,  242,
  691,  231,  417,    0,    0,  813,    0,  813,    0,    0,
    0,    0,    0,    0,    0,    0,  362,  363,  364,  418,
  183,  184,  690,  782,    0,    0,  782,  782,  691,    0,
  419,  526,    0,    0,    0,    0,  782,    0,    0,  365,
  366,  420,  421,  422,  367,  368,  423,  424,    0,  425,
    0,    0,    0,  782,  426,  427,  251,  428,  429,  430,
  188, 1094,    0,  431, 1073,  690,    0,    0,  405,    0,
    0,  691,    0,  242,    0,  782,    0,  406,  251,    0,
  782,  188,    0,    0,    0,  251,    0,    0,    0,    0,
    0,  407,  782,  690,  782,  242,  181,  360,  408,  691,
    0,  242,  242,  188,    0,    0,    0,    0,  434,  188,
    0,  432,    0,  433,    0,    0,    0,  181,    0,  251,
    0,    0,    0,  242,    0,  409,  435,    0,  410,    0,
  242,  258,  188,    0,    0,    0,  242,    0,    0,  181,
  690,    0,    0,    0,    0,  181,  691, 1097,    0,    0,
 1073,  188,  690,    0,  405,    0,  411,  412,  691,    0,
  188,    0,    0,  406,  242,    0,    0,  188,  181,  413,
    0,    0,    0,  414,    0,    0,    0,  407,    0,    0,
    0,    0,    0,  360,  408,  188,    0,  181,    0,  434,
    0,    0,  432,    0,  433,    0,  181,    0,    0,    0,
    0,    0,    0,  181,    0,    0,    0,  435,    0,    0,
    0,  409,  258,    0,  410,    0,    0,    0,    0,    0,
    0,  181,  188,    0,    0,    0,    0,    0, 1074,    0,
  415,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  411,  412,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,  413,    0,    0,  181,  414,
  362,  363,  364,  418,  183,  184,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,  365,  366,  420,  421,  422,  367,  368,
  423,  424,    0,  425,    0,    0,    0,    0,  426,  427,
    0,  428,  429,  430,    0,    0,    0,  431,    0,    0,
    0,    0,    0,    0, 1074,    0,  415,    0,    0,    0,
    0,    0,  416,    0,    0,    0,    0,    0, 1102,    0,
    0, 1073,  417,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,  362,  363,  364,  418,
  183,  184,    0,    0,    0,    0,    0,    0,  407,    0,
  419,  783,    0,    0,  360,  408,    0,    0,    0,  365,
  366,  420,  421,  422,  367,  368,  423,  424,    0,  425,
    0,    0,    0,    0,  426,  427,    0,  428,  429,  430,
    0,    0,  409,  431,    0,  410,    0,    0,    0,    0,
    0,    0,  783,    0,    0,  783,    0,    0,    0, 1105,
    0,    0, 1073,    0,    0,    0,  405,    0,    0,    0,
  783,    0,    0,  411,  412,  406,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  413,    0,  434,  407,
  414,  432,    0,  433,    0,  360,  408,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  435,    0,    0,    0,
    0,  258,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  409,    0,    0,  410,    0,    0,  929,
  694,  692,    0,  693,    0,  695,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1074,    0,  415,  859,  858,
  860,    0,    0,  416,  411,  412,    0,    0,    0,    0,
    0,    0,    0,  417,    0,    0,    0,  413,    0,    0,
    0,  414,    0,    0,    0,    0,    0,  362,  363,  364,
  418,  183,  184,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  365,  366,  420,  421,  422,  367,  368,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0, 1074,    0,  415, 1431,
 1432,    0,    0,    0,  416, 1436, 1437,    0, 1439, 1440,
    0,    0,    0,    0,  417,    0, 1444, 1445, 1446, 1447,
    0, 1448, 1449, 1450, 1451, 1452, 1453,    0,  362,  363,
  364,  418,  183,  184,    0,    0,    0,    0,    0,    0,
    0,    0,  419,  783,    0,    0,    0,    0,    0,    0,
    0,  365,  366,  420,  421,  422,  367,  368,  423,  424,
    0,  425,    0,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,    0,    0,  431,    0,    0, 1108,    0,
    0, 1073,    0,    0,    0,  405,  434,    0,  463,  432,
    0,  433,    0,    0,  406,    0,    0,    0,    0,    0,
  783,   59,    0,  783,  435,    0,    0,    0,  407,  258,
  783,    0,   60,  783,  360,  408,    0,  783,    0,   61,
    0,    0,  783,    0,    0,  783,    0,    0,   81,    0,
  846,    0,   62,    0,    0,   82,    0,    0,  690,   83,
   84,  783,  409,    0,  691,  410,    0,  847,   85,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   86,    0,    0,    0,    0,    0,  783,    0,    0,  783,
  783,    0,    0,  411,  412,    0,    0,    0,    0,  783,
    0,    0,    0,  848,  434,    0,  413,  432,    0,  433,
  414,    0,  849,    0,   63,    0,  783,    0,    0,    0,
  850,    0,  435,    0,    0,    0,    0,  258,    0,    0,
   64,    0,    0,    0,    0,    0,    0,    0,  783,  851,
    0,    0,  852,  783,    0,    0,   87,   65,    0,    0,
    0,    0,    0,    0,    0,  783,  853,  783,  854,  855,
  856,    0,   66,   88,    0, 1074,   67,  415,    0,   68,
    0,    0,    0,  416,    0,    0,    0,   69,   89,    0,
    0,    0,   90,  417,    0,    0,    0,  942,    0,  942,
  942,    0,  942,   91,    0,    0,   70,  362,  363,  364,
  418,  183,  184,    0,    0,  942,    0,    0,    0,  857,
  942,  419,   92,    0,    0,    0,    0,    0,    0,    0,
  365,  366,  420,  421,  422,  367,  368,  423,  424,    0,
  425,    0,    0,  405,    0,  426,  427,    0,  428,  429,
  430,    0,  406,    0,  431,    0,    0,    0,    0,    0,
    1,    0,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,  360,  408,    0,    0,    0,    0,    0,    0,
    0,    0,    2,    0,    0,    0,    0,    0,    0,    0,
    3,    0,    0,    0,    0,    0,    0,    4,    0,    5,
  409,    6,    0,  410,    0,    0,    7,    0,    0,    0,
    0,    0,    0,    8,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    9,    0,  361,
    0,  411,  412,    0,    0,    0,    0,    0,   10,    0,
    0,  405,    0,    0,  413,    0,    0,    0,  414,    0,
  406,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
  360,  408,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  694,  692,    0,  693,    0,  695,
    0,    0,    0,    0,    0,    0,    0,    0,  409,    0,
    0,  410,  859,  858,  860,  415,   11,    0,    0,    0,
    0,  416,    0,   12,    0,    0,   13,    0,   14,   15,
    0,  417,    0,    0,  942,    0,    0,  361,    0,  411,
  412,    0,    0,  942,    0,  362,  363,  364,  418,  183,
  184,    0,  413,   16,    0,    0,  414,  942,    0,  419,
    0,    0,  638,  942,  942,  638,    0,  638,  365,  366,
  420,  421,  422,  367,  368,  423,  424,    0,  425,    0,
  638,    0,    0,  426,  427,  638,  428,  429,  430,    0,
    0,  942,  431,    0,  942,    0,    0,    0,    0,    0,
    0,    0,   17,  684,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  415,    0,    0,    0,   18,    0,  416,
  942,    0,  942,  942,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,  942,    0,    0,    0,  942,
    0,    0,    0,  362,  363,  364,  418,  183,  184,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
  434,    0,    0,  432,    0,  433,  365,  366,  420,  421,
  422,  367,  368,  423,  424,    0,  425,    0,  435,    0,
    0,  426,  427,  258,  428,  429,  430,    0,    0,    0,
  431,    0,    0,    0,    0,    0,  942,    0,    0,    0,
    0,    0,  942,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  942,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  846,    0,  942,  942,  942,  942,
  942,  942,  690,    0,    0,    0,    0,    0,  691,    0,
  942,  847,    0,  434,    0,    0,  432,    0,  433,  942,
  942,  942,  942,  942,  942,  942,  942,  942,    0,  942,
    0,  435,    0,    0,  942,  942,  258,  942,  942,  942,
    0,    0,    0,  942,    0,    0,    0,  848,    0,    0,
    0,    0,  638,    0,    0,    0,  849,    0,    0,  638,
    0,    0,    0,    0,  850,    0,    0,    0,  638,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  638,  851,    0,    0,  852,    0,  638,  638,
    0,    0,    0,    0,    0,    0,  638,    0,    0,    0,
  853,    0,  854,  855,  856,    0,    0,    0,    0,    0,
  434,    0,    0,  432,    0,  433,  638,    0,    0,  638,
    0,    0,    0,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,  258,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  638,  638,    0,
    0,    0,    0,  857,    0,    0,    0,  405,    0,    0,
  638,    0,    0,    0,  638,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,  360,  408,    0,    0,
    0,    0,    0,  434,  931,    0,  432,    0,  433,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  435,    0,    0,  409,    0,  258,  410,    0,    0,
    0,  638,    0,    0,    0,    0,    0,  638,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  638,    0,    0,
  405,    0,    0,    0,    0,  411,  412,    0,    0,  406,
    0,  638,  638,  638,  638,  638,  638,    0,  413,    0,
    0,    0,  414,  407,    0,  638,    0,    0,    0,  360,
  408,    0,    0,    0,  638,  638,  638,  638,  638,  638,
  638,  638,  638,    0,  638,    0,    0,    0,    0,  638,
  638,    0,  638,  638,  638,    0,    0,  409,  638,    0,
  410,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,    0,    0,  416,  361,    0,  411,  412,
    0,    0,    0,    0,    0,  417,    0,  405,    0,    0,
    0,  413,    0,  676,    0,  414,  406,    0,    0,  362,
  363,  364,  418,  183,  184,    0,    0,    0,    0,    0,
  407,    0,    0,  419,    0,    0,  360,  408,    0,    0,
    0,    0,  365,  366,  420,  421,  422,  367,  368,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,  409,    0,  431,  410,    0,    0,
    0,    0,  415,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  417,    0,
  405,    0,    0,    0,    0,  411,  412,    0,    0,  406,
    0,    0,  362,  363,  364,  418,  183,  184,  413,    0,
    0,    0,  414,  407,    0,    0,  419,    0,    0,  360,
  408,    0,    0,    0,    0,  365,  366,  420,  421,  422,
  367,  368,  423,  424,  434,  425,    0,  432,    0,  433,
  426,  427,    0,  428,  429,  430,    0,  409,    0,  431,
  410,    0,  435,    0,    0,    0,    0,  258,    0,  684,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  415,
    0,    0,    0,    0,    0,  416,    0,    0,  411,  412,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,  413,    0,    0,    0,  414,    0,    0,    0,  362,
  363,  364,  418,  183,  184,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  365,  366,  420,  421,  422,  367,  368,  423,
  424,  434,  425,    0,  432,    0,  433,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,    0,    0,  435,
    0,    0,  415,    0,  258,    0,    0,    0,  416,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  362,  363,  364,  418,  183,  184,    0,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  365,  366,  420,  421,  422,
  367,  368,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,  564,  428,  429,  430,  565,    0,    0,  431,
    0,    0,    0,    0,  760,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  566,    0,    0,  567,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,  568,
  360,  408,    0,    0,    0,  569,  570,  571,    0,  572,
  573,  574,    0,  575,    0,    0,    0,    0,    0,    0,
    0,  434,    0,    0,  432,    0,  433,    0,  409,    0,
    0,  410,    0,    0,  576,    0,  577,    0,    0,  435,
    0,    0,    0,    0,  258,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  411,
  412,  807,    0,    0,    0,    0,    0,  578,  405,    0,
    0,    0,  413,    0,    0,    0,  414,  406,    0,    0,
    0,    0,    0,    0,  579,  434,    0,    0,  432,    0,
  433,  407,    0,    0,    0,    0,    0,  360,  408,    0,
    0,    0,  807,  435, 1658,  807,  580,    0,  258,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  807,  188,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,  415,    0,    0,    0,    0,    0,  416,
    0,  581,  582,    0,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,  411,  412,  583,    0,
 1100,    0,    0,  362,  363,  364,  418,  183,  184,  413,
    0,    0,    0,  414,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  365,  366,  420,  421,
  422,  367,  368,  423,  424,    0,  425,    0,    0,    0,
    0,  426,  427,    0,  428,  429,  430,    0,    0,    0,
  431,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1281,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  417,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
  362,  363,  364,  418,  183,  184,    0,    0,    0,    0,
    0,  407,    0,    0,  419,    0,    0,  360,  408,    0,
    0,    0,    0,  365,  366,  420,  421,  422,  367,  368,
  423,  424,    0,  425,    0,    0,    0,    0,  426,  427,
    0,  428,  429,  430,    0,  409,    0,  431,  410,    0,
    0,    0,  405,    0,    0,    0,    0,    0,    0,    0,
    0,  406, 1701,  807,    0,  432,    0,  433,    0,    0,
    0,    0,    0,    0,    0,  407,  411,  412,    0,    0,
  435,  360,  408,    0,    0,  258,    0,    0,    0,  413,
    0,    0,    0,  414,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  409,
    0,    0,  410,    0,    0,    0,    0,    0,    0,    0,
  807,    0,    0,  807,    0,    0,  434,    0,    0,  432,
  807,  433,    0,  807,    0,    0,    0,  807,    0,    0,
  411,  412,  807,    0,  435,  807,    0,    0,    0,  258,
  415,    0,    0,  413,    0,    0,  416,  414,    0,    0,
    0,  807,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1294,    0,    0,
  362,  363,  364,  418,  183,  184,  807,    0,    0,  807,
  807,    0,    0,    0,  419,    0,    0,    0,    0,  807,
    0,    0,    0,  365,  366,  420,  421,  422,  367,  368,
  423,  424,    0,  425,  415,    0,  807,    0,  426,  427,
  416,  428,  429,  430,    0,    0,    0,  431,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,  807,    0,
    0,    0,    0,  807,  362,  363,  364,  418,  183,  184,
    0,    0,    0,    0,    0,  807,    0,  807,  419, 1377,
    0,    0,    0,    0,    0,    0,    0,  365,  366,  420,
  421,  422,  367,  368,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0,  405,
    6,  431,    0,    0,    0,    0,    0,    0,  406,    0,
    0, 1494, 1495,    0, 1496,    0,  942, 1497,    0,  942,
    0,  942,  407,    0,    0,    0,    0,    0,  360,  408,
  786, 1498,    0,    0,  942,    0,    0, 1499,    0,  942,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  409,    0,    0,  410,
 1500,    0,    0,  405,    0,    0,    0,    0,    0,    0,
    0,  786,  406,    0,  786,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  361,  407,  411,  412,  786,
    0,    0,  360,  408,    0,    0,    0,    0, 1501,    0,
  413,    0,    0,    0,  414,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   14,    0,    0,
  409,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0, 1502,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,    0,
    0,  411,  412, 1503,    0,    0,    0,    0,    0,    0,
  484,  415,    0,    0,  413,    0,    0,  416,  414,    0,
    0,    0,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1504,
    0,  362,  363,  364,  418,  183,  184,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  365,  366,  420,  421,  422,  367,
  368,  423,  424,    0,  425,  415,    0,    0,    0,  426,
  427,  416,  428,  429,  430,    0,    0,    0,  431,    0,
    0,  417,    0,  942,    0,    0,    0,    0,    0,    0,
    0,    0,  942,    0,    0,  362,  363,  364,  418,  183,
  184,    0,    0,    0,  948,    0,  942,    0,    0,  419,
    0,    0,  942,  942,    0,    0,    0,    0,  365,  366,
  420,  421,  422,  367,  368,  423,  424,    0,  425,    0,
    0,    0,  786,  426,  427,    0,  428,  429,  430,    0,
  942,    0,  431,  942,  944,  948,  948,  948,  948,  948,
  961,  948,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  948,  948,  948,  948,    0,    0,    0,
    0,  942,  942,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  942,    0,    0,    0,  942,  786,
    0,    0,  786,    0,    0,  948,    0,  948,    0,  786,
    0,    0,  786,    0,  317,    0,  786,    0,    0,    0,
    0,  786,    0,    0,  786,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  786,    0,    0,    0,    0,  317,    0,    0,    0,    0,
    0,    0,  317,    0,    0,  942,  317,  317,    0,  317,
    0,  942,  317,    0,    0,  786,    0,    0,  786,  786,
    0,  942,    0,    0,    0,    0,  317,    0,  786,    0,
    0,    0,  317,    0,    0,  942,  942,  942,  942,  942,
  942,    0,    0,    0,    0,  786,    0,  828,    0,  942,
    0,    0,    0,    0,    0,  317,    0,    0,  942,  942,
  942,  942,  942,  942,  942,  942,  942,  786,  942,    0,
    0,    0,  786,  942,  942,    0,  942,  942,  942,    0,
    0,    0,  942,    0,  786,    0,  786,    0,  828,  828,
  828,  828,  828,  317,  828,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  828,  828,  828,  828,
    0,    0,  317,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  948,  317,  948,    0,
    0,    0,    0,    0,    0,    0,  948,  317,    0,    0,
  828,    0,    0,    0,  948,    0,  317,    0,  317,    0,
  948,    0,    0,  948,    0,  317,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  948,  948,    0,    0,  948,    0,    0,    0,
    0,    0,    0,  948,  317,  948,  948,    0,    0,  948,
    0,    0,    0,  948,    0,    0,  948,    0,  948,    0,
  948,    0,    0,    0,    0,  948,  948,    0,  948,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  948,  948,    0,    0,  948,    0,
  840,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  948,    0,  948,  948,  948,    0,    0,  948,
    0,    0,  948,  948,    0,    0,    0,    0,    0,    0,
    0,    0,  948,    0,    0,    0,    0,    0,    0,    0,
    0,  840,  840,  840,  840,  840,  948,  840,    0,  948,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  840,
  840,  840,  840,    0,    0,  948,    0,    0,    0,    0,
    0,  948,    0,    0,    0,    0,  948,    0,    0,  828,
    0,  828,    0,    0,    0,    0,    0,  948,  948,  828,
  948,    0,    0,  840,    0,    0,  948,  828,    0,    0,
    0,    0,    0,  828,    0,    0,  828,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  828,  828,    0,    0,  828,
    0,    0,    0,    0,    0,    0,  828,    0,  828,  828,
    0,    0,  828,    0,    0,    0,  828,    0,    0,  828,
    0,  828,    0,  828,    0,    0,    0,    0,  828,  828,
    0,  828,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  843,  828,  828,    0,
    0,  828,    0,    0,    0, 1377,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  828,    0,  828,  828,  828,
    0,    0,  828,    0,    0,  828,  828,    0,    0,    0,
    0,    0,    0,    0,    0,  828,    6,  843,  843,  843,
  843,  843,    0,  843,    0,    0,    0, 1494, 1495,  828,
 1496,    0,  828, 1497,    0,  843,  843,  843,  843,    0,
    0,    0,    0,    0,    0,    0,    0, 1498,  828,    0,
    0,    0,    0, 1499,  828,    0,    0,    0,    0,  828,
    0,    0,  840,    0,  840,    0,    0,    0,    0,  843,
  828,  828,  840,  828,    0,    0, 1500,    0,    0,  828,
    0,    0,    0,    0,    0,    0,  840,    0,    0,  840,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  840,  840,
    0,    0,  840,    0, 1501,    0,    0,    0,    0,  840,
    0,  840,  840,    0,    0,  840,    0,    0,    0,  840,
    0,    0,  840,   14,  840,    0,  840,    0,    0,    0,
    0,  840,  840,    0,  840,    0,    0,    0, 1502,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   16,  844,
  840,  840,    0,    0,  840,    0,    0, 1620,    0, 1503,
    0,    0,    0,    0,    0,    0,  484,    0,  840,    0,
  840,  840,  840,    0,    0,  840,    0,    0,  840,  840,
    0,    0,    0,    0,    0,    0,    0,    0,  840,    0,
  844,  844,  844,  844,  844, 1504,  844,    0,    0,    0,
    0,    0,  840,    0,    0,  840,    0,    0,  844,  844,
  844,  844,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  840,    0,    0,    0,    0,    0,  840,  843,    0,
  843,    0,  840,    0,    0,    0,    0,    0,  843,    0,
  784,    0,  844,  840,  840,    0,  840,    0,    0,    0,
    0,    0,  840,    0,    0,  843,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  843,  843,    0,    0,  843,    0,
    0,  784,    0,    0,  784,  843,    0,  843,  843,    0,
    0,  843,    0,    0,    0,  843,    0,    0,  843,  784,
  843,    0,  843,    0,    0,    0,    0,  843,  843,    0,
  843,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  838,  843,  843,    0,    0,
  843,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  843,    0,  843,  843,  843,    0,
    0,  843,    0,    0,  843,  843,    0,    0,    0,    0,
    0,    0,    0,    0,  843,    0,  838,    0,  838,  838,
  838,    0,    0,    0,    0,    0,    0,    0,  843,    0,
    0,  843,    0,    0,  838,  838,  838,  838,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  843,    0,    0,
    0,    0,    0,  843,    0,    0,    0,    0,  843,    0,
    0,  844,    0,  844,    0,    0,    0,    0,  838,  843,
  843,  844,  843,    0,    0,    0,    0,    0,  843,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  844,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  844,  844,    0,
    0,  844,    0,  837,    0,    0,    0,    0,  844,    0,
  844,  844,    0,    0,  844,    0,    0,    0,  844,    0,
    0,  844,    0,  844,    0,  844,    0,    0,    0,    0,
  844,  844,    0,  844,    0,    0,    0,    0,    0,    0,
    0,    0,  784,    0,  837,    0,  837,  837,  837,  844,
  844,    0,    0,  844,    0,    0,    0,    0,    0,    0,
    0,    0,  837,  837,  837,  837,    0,  844,    0,  844,
  844,  844,    0,    0,  844,    0,    0,  844,  844,    0,
    0,    0,    0,    0,    0,    0,    0,  844,    0,    0,
    0,    0,    0,    0,    0,    0,  837,    0,    0,  784,
    0,  844,  784,    0,  844,    0,    0,    0,    0,  784,
    0,    0,  784,    0,    0,    0,  784,    0,    0,    0,
  844,  784,    0,    0,  784,    0,  844,  838,    0,  838,
    0,  844,    0,    0,    0,    0,    0,  838,    0,    0,
  784,    0,  844,  844,    0,  844,    0,    0,    0,    0,
    0,  844,    0,    0,  838,    0,    0,    0,    0,  839,
    0,    0,    0,    0,    0,  784,    0,    0,  784,  784,
    0,    0,    0,  838,  838,    0,    0,  838,  784,    0,
    0,    0,    0,    0,  838,    0,  838,  838,    0,    0,
  838,    0,    0,    0,  838,  784,    0,  838,    0,  838,
  839,  838,  839,  839,  839,    0,  838,  838,    0,  838,
    0,    0,    0,    0,    0,    0,    0,  784,  839,  839,
  839,  839,  784,    0,    0,  838,  838,    0,    0,  838,
    0,    0,    0,    0,  784,    0,  784,    0,    0,    0,
    0,    0,    0,  838,    0,  838,  838,  838,    0,    0,
  838,    0,  839,  838,  838,    0,    0,    0,    0,    0,
    0,    0,    0,  838,    0,  837,    0,  837,    0,    0,
    0,    0,    0,    0,    0,  837,    0,  838,    0,    0,
  838,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  837,    0,    0,    0,  838,  842,    0,    0,
    0,    0,  838,    0,    0,    0,    0,  838,    0,    0,
    0,  837,  837,    0,    0,  837,    0,    0,  838,  838,
    0,  838,  837,    0,  837,  837,    0,  838,  837,    0,
    0,    0,  837,    0,    0,  837,    0,  837,  842,  837,
  842,  842,  842,    0,  837,  837,    0,  837,    0,    0,
    0,    0,    0,    0,    0,    0,  842,  842,  842,  842,
    0,    0,    0,  837,  837,    0,    0,  837,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  785,    0,
    0,  837,    0,  837,  837,  837,    0,    0,  837,    0,
  842,  837,  837,    0,    0,    0,    0,    0,    0,    0,
    0,  837,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  815,    0,    0,    0,  837,    0,    0,  837,  785,
    0,  839,  785,  839,    0,    0,    0,    0,    0,    0,
    0,  839,    0,    0,  837,    0,    0,  785,    0,    0,
  837,    0,    0,    0,    0,  837,    0,    0,  839,    0,
    0,    0,  815,  779,    0,  815,  837,  837,    0,  837,
    0,    0,    0,    0,    0,  837,    0,  839,  839,    0,
  815,  839,    0,    0,    0,    0,    0,    0,  839,    0,
  839,  839,    0,    0,  839,    0,    0,    0,  839,    0,
    0,  839,    0,  839,  779,  839,    0,  779,    0,    0,
  839,  839,    0,  839,    0,    0,    0,    0,    0,    0,
    0,    0,  779,    0,    0,    0,    0,    0,    0,  839,
  839,    0,    0,  839,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  839,    0,  839,
  839,  839,    0,    0,  839,    0,    0,  839,  839,    0,
    0,    0,    0,    0,    0,    0,    0,  839,    0,  842,
    0,  842,    0,    0,    0,    0,    0,    0,    0,  842,
    0,  839,    0,    0,  839,    0,    0,    0,    0,    0,
    0,    0,    0,  780,    0,    0,  842,    0,    0,    0,
  839,    0,    0,    0,    0,    0,  839,    0,    0,    0,
    0,  839,    0,    0,  781,  842,  842,    0,    0,  842,
    0,    0,  839,  839,    0,  839,  842,    0,  842,  842,
    0,  839,  842,    0,  780,  814,  842,  780,    0,  842,
    0,  842,    0,  842,    0,    0,    0,    0,  842,  842,
  785,  842,  780,    0,    0,  781,    0,    0,  781,    0,
    0,    0,    0,    0,    0,    0,    0,  842,  842,    0,
    0,  842,    0,  781,    0,    0,  814,    0,    0,  814,
    0,    0,    0,  815,    0,  842,    0,  842,  842,  842,
    0,    0,  842,    0,  814,  842,  842,    0,    0,    0,
    0,    0,    0,    0,    0,  842,    0,  785,    0,    0,
  785,    0,    0,    0,    0,    0,    0,  785,    0,  842,
  785,    0,  842,    0,  785,  779,    0,    0,    0,  785,
    0,    0,  785,    0,    0,    0,    0,    0,  842,    0,
  815,    0,    0,  815,  842,    0,    0,    0,  785,  842,
  815,    0,  808,  815,    0,    0,    0,  815,    0,    0,
  842,  842,  815,  842,    0,  815,    0,    0,    0,  842,
    0,    0,    0,  785,    0,    0,  785,  785,    0,    0,
    0,  815,  779,    0,    0,  779,  785,    0,    0,    0,
    0,    0,  779,  808,    0,  779,  808,    0,    0,  779,
    0,    0,    0,  785,  779,    0,  815,  779,    0,  815,
  815,  808,    0,  816,    0,    0,    0,    0,    0,  815,
    0,    0,    0,  779,  817,  785,    0,    0,    0,    0,
  785,    0,    0,    0,    0,    0,  815,    0,    0,    0,
    0,    0,  785,    0,  785,    0,    0,  805,  779,    0,
    0,  779,  779,    0,  816,  780,    0,  816,  815,    0,
    0,  779,    0,  815,    0,  817,    0,    0,  817,    0,
    0,    0,  816,    0,    0,  815,  781,  815,  779,    0,
    0,    0,    0,  817,    0,    0,    0,    0,  805,    0,
    0,  805,    0,    0,    0,    0,    0,  814,    0,    0,
  779,    0,    0,    0,    0,  779,  805,    0,    0,    0,
    0,    0,  780,    0,    0,  780,    0,  779,    0,  779,
    0,    0,  780,    0,    0,  780,    0,    0,    0,  780,
    0,    0,    0,  781,  780,    0,  781,  780,    0,    0,
    0,    0,    0,  781,    0,    0,  781,    0,    0,    0,
  781,    0,    0,  780,  814,  781,    0,  814,  781,    0,
    0,    0,    0,  809,  814,    0,    0,  814,    0,    0,
    0,  814,    0,    0,  781,    0,  814,    0,  780,  814,
    0,  780,  780,    0,    0,    0,    0,    0,    0,    0,
    0,  780,    0,    0,    0,  814,  818,    0,    0,  781,
    0,    0,  781,  781,  809,    0,    0,  809,  780,    0,
    0,    0,  781,    0,  808,    0,    0,    0,    0,    0,
  814,    0,  809,  814,  814,    0,    0,    0,    0,  781,
  780,    0,    0,  814,    0,  780,    0,  818,  806,    0,
  818,    0,    0,    0,    0,    0,    0,  780,    0,  780,
  814,  781,    0,    0,    0,  818,  781,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  781,    0,
  781,  808,  814,    0,  808,  816,    0,  814,    0,  806,
  810,  808,  806,    0,  808,    0,  817,    0,  808,  814,
    0,  814,    0,  808,    0,    0,  808,  806,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  805,
    0,    0,  808,    0,    0,    0,    0,    0,    0,    0,
    0,  810,    0,    0,  810,    0,    0,    0,    0,    0,
    0,    0,  816,    0,    0,  816,    0,  808,    0,  810,
  808,  808,  816,  817,    0,  816,  817,    0,    0,  816,
  808,    0,    0,  817,  816,  765,  817,  816,    0,    0,
  817,    0,    0,    0,    0,  817,  805,  808,  817,  805,
    0,    0,    0,  816,    0,    0,  805,    0,    0,  805,
    0,    0,    0,  805,  817,    0,    0,    0,  805,  808,
    0,  805,    0,    0,  808,    0,  765,  662,  816,  765,
    0,  816,  816,    0,    0,    0,  808,  805,  808,  817,
    0,  816,  817,  817,  765,    0,    0,    0,    0,    0,
    0,    0,  817,    0,    0,  809,    0,    0,  816,    0,
    0,    0,  805,    0,    0,  805,  805,    0,  662,  817,
    0,  662,    0,    0,    0,  805,    0,  659,    0,    0,
  816,    0,    0,    0,    0,  816,  662,    0,  818,    0,
    0,  817,  805,    0,    0,    0,  817,  816,    0,  816,
    0,    0,    0,    0,    0,    0,    0,    0,  817,    0,
  817,    0,  809,    0,  805,  809,    0,    0,  659,  805,
    0,  659,  809,    0,    0,  809,    0,    0,    0,  809,
  806,  805,    0,  805,  809,    0,  659,  809,    0,    0,
    0,    0,    0,    0,    0,  818,    0,    0,  818,    0,
    0,    0,    0,  809,    0,  818,    0,    0,  818,    0,
    0,    0,  818,    0,    0,    0,    0,  818,    0,    0,
  818,    0,  810,    0,    0,    0,    0,    0,  809,    0,
    0,  809,  809,    0,    0,    0,  818,  806,    0,    0,
  806,  809,    0,    0,    0,    0,    0,  806,    0,    0,
  806,    0,    0,    0,  806,    0,    0,    0,  809,  806,
    0,  818,  806,    0,  818,  818,    0,    0,    0,    0,
    0,    0,    0,    0,  818,    0,    0,    0,  806,  810,
  809,    0,  810,    0,    0,  809,    0,    0,    0,  810,
    0,  818,  810,    0,    0,    0,  810,  809,    0,  809,
    0,  810,    0,  806,  810,    0,  806,  806,    0,    0,
    0,    0,    0,  818,    0,    0,  806,    0,  818,    0,
  810,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  818,    0,  818,  806,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  810,    0,    0,  810,  810,
    0,    0,    0,    0,  765,  806,    0,  765,  810,    0,
  806,    0,    0,    0,  765,    0,    0,  765,    0,    0,
    0,  765,  806,    0,  806,  810,  765,    0,    0,  765,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  765,  662,  810,    0,  662,
    0,    0,  810,    0,    0,    0,  662,    0,    0,  662,
    0,    0,    0,  662,  810,    0,  810,    0,  662,    0,
  765,  662,    0,  765,  765,    0,    0,    0,  221,    0,
    0,    0,  221,  765,    0,    0,    0,  662,    0,    0,
    0,    0,    0,    0,    0,    0,  659,  221,    0,  659,
  765,  221,    0,    0,  221,    0,  659,    0,    0,  659,
  832,    0,  662,  659,  565,    0,  662,    0,  659,    0,
    0,  659,  765,    0,    0,  662,    0,  765,    0,  833,
    0,    0,    0,  566,    0,    0,  567,  659,    0,  765,
    0,  765,  662,    0,    0,  221,    0,    0,    0,    0,
    0,  221,  221,  221,    0,  221,  221,  221,    0,  221,
    0,    0,  659,    0,    0,    0,  659,    0,    0,  662,
    0,    0,    0,    0,    0,  659,    0,  568,    0,    0,
  221,  662,  221,  569,  570,  571,    0,  572,  573,  574,
    0,  575,  659,    0,  564,    0,    0,    0,  565,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  576,  221,  577,    0,    0,  566,   81,  659,
  567,    0,    0,    0,    0,    0,    0,    0,  221,    0,
  221,  659,  221,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  578,    0,  221,    0,    0,
    0,  221,  221,    0,  221,    0,    0,    0,    0,    0,
  564,  568,  579,    0,  565,    0,    0,  569,  570,  571,
    0,  572,  573,  574,    0,  575,    0,    0,    0,    0,
    0,    0,    0,  566,  580,    0,  567,  221,  221,    0,
    0,    0,    0,    0,    0,  221,  576,    0,  577,    0,
    0,  221,  221,  221,  221,  221,  221,  221,    0,  221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  581,
  582,    0,    0,    0,    0,    0,    0,  568,    0,  578,
  221,    0,  221,  569,  570,  571,  583,  572,  573,  574,
    0,  575,    0,    0,  134,    0,  579,    0,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  576,  221,  577,    0,    0,  134,  580,    0,
  134,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  221,    0,    0,  188,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  578,    0,    0,    0,    0,
    0,    0,  221,  581,  582,    0,    0,    0,    0,    0,
    0,  134,  579,    0,    0,    0,    0,  134,  134,  134,
  583,  134,  134,  134,    0,  134,    0,    0,    0,    0,
    0,    0,    0,    0,  580,    0,    0,  221,  221,    0,
    0,    0,    0,    0,    0,    0,  134,    0,  134,    0,
    0,    0,    0,    0,  221,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  581,
  582,    0,    0,    0,    0,    0,    0,    0,    0,  134,
    0,    0,    0,    0,    0,    0,  583,    0,    0,    0,
    0,    0,  142,    0,    0,    0,  134,    0,    0,    0,
    0,  143,  144,  145,  146,  147,  148,  149,  150,  151,
  152,  153,  154,  155,  156,  157,  158,    0,  134,  159,
  160,  161,  162,    0,  163,    0,    0,  164,  165,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  134,  134,    0,    0,    0,    0,    0,
    0,    0,  541,    0,    0,    0,    0,    0,    0,    0,
  134,  143,  144,  145,  146,  147,  148,  149,  150,  151,
  152,  153,  154,  155,  156,  157,  158,    0,    0,  159,
  160,  161,  162,  456,  163,    0,    0,  164,  165,    0,
    0,    0,    0,  456,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,    0,    0,
  456,  456,  456,  456,    0,  456,    0,    0,  456,  456,
};
static short yycheck[] = {                                      60,
   16,   52,  130,   61,    4,    4,  101,   59,    0,  295,
  235,   53,  235,  119,  120,  681,   60,  334,   54,  119,
  120,  330,   64,  488,   82,  338,  495,  338,  729,  309,
   66,  524,   70,   67,  527,   86,  533,  307,  470,  737,
  533,  266,   85,   91,  886,   87,    0,    0,  343,  110,
  243,  112,   94,  622,   92,  624,   90,   65,  994,  252,
  667, 1001,  994,  994,   80, 1001,  393, 1172,  657, 1001,
 1001,  994, 1002,   89,  728,  111, 1216,    0, 1001,    0,
   88,  124,  355, 1013,   44,  133,  928,   41,   41,  131,
   44,   44,  730,  731,    0,  684,  132,  609,    0,  616,
  257,  826, 1032,  757,  261,   59,   59,  676,  661,  266,
    0,    0,  766,  208,    0,  945,    0,  169,  725,    0,
   41,    0,    0,   44, 1049,   44,   59,   40,    0,  697,
   41,   41,  795,   45,   44,  235,   59,   59,   59,   41,
   41,  193,   44,   40,   46,  370,  302,  370, 1127,   44,
    0,   41,   61,   59,   44, 1359,    0,   59,  286,   61,
   41,  286, 1359,   44,   41,  819,   41,  295,   44,   59,
   59,  734,   42,   59,    0,   59,  237,   44,   59,  235,
   59,   59,  399,   40, 1359,   40,  503,   59, 1026,   40,
  260,   41,  385,   40,   44,  490,  288,  264,  293,   42,
    0,  510,  511,  512,  513,  241,   45,   40, 1050,   59,
    0,   58,   40,   44,  250,   59,   45,   44, 1417,  270,
  533,   59,  533,   45,  288,  286,  250,  302,   41, 1214,
   63,   44,   40,   59,   44,   63,  701,   45,  274,   40,
   58,   45,  559,  284,   63,  260,  326,  308, 1264,   45,
  262,  356, 1359,  257,  315,  321,  317,  264,  319,   59,
 1120,  322, 1122, 1131,  339,  393,  536,   63,  661,   59,
  370,  428,  357, 1359,  277,  432,  433,  434,    0,  331,
  887,  333,   91,  338,   41,  262,  404,   44,    0,  404,
  783,  564,  861,  862,    0,  347,   41,  379,  340,   44,
  351, 1231, 1242,  419,   44,    0, 1242,  341,    0,   44,
 1242, 1242,  966,  349,  370,  585,   16,  260,  436, 1242,
    0,  376,   41,  274,   59,  440,  879,    0,  302,   41,
  446,  335,  393,  451,  389,   41,   93,   59,  891,  892,
    0,  257,  384,  346, 1174,  381,  431,   59,    0,  419,
    0,  139,  432,   59,  923,  355,  392,  399, 1012,  439,
  396,  304,  264,  265,   59,  401,  319,   59,  450,  444,
  462,  803,  438,    0,    0,  265,  388,  401,  280,   59,
   80,   41,    0,  264,   44,  262,   59, 1591,  341,  494,
    0, 1531,  301, 1261, 1591,  462,    0, 1382,  462,   59,
  441,  303,   44,  305,  419, 1421,  413,   59,  319,   59,
 1248,  388,    0,  303,  269,   41, 1591,  319,  643,  335,
  643,   41,    0,    0,  993,  437,  321,  297,  278,  319,
  280,  385,   59,   59,  285,  310, 1635,  343,  288,  341,
  321,   59,  667,   44, 1374,  551,  732,  664, 1378,   59,
 1380,  341,  341,    0,  264,   59,  508,    0,  258,  652,
  332,  274,  341, 1323,  457, 1128,   44,  624,  277,  259,
  444,   59,  533,  379,  264,  745, 1455,  702,  748,  412,
  270,   59,   59,  396, 1591, 1415,  879, 1627,  389, 1414,
  412, 1054,  762,  319,  321, 1193,  456,  310,  891, 1165,
  725,  407,  554,  413,  410, 1591, 1172,  307,  296, 1077,
  667,  388,   59,  670,  398,  341,   59,  468,  675,  789,
  426,    0,  817,  680,  681,  411,  398,    0, 1358, 1254,
  380, 1043,  689,  456,  691,  692,  693,  694,  695, 1679,
  697,  460, 1059,  643,  552,  702,  452,  608,  460,  503,
  830,  504,  402,  605,  606, 1660, 1486,  884,  442,  409,
  438, 1491,   41,  389,  564,  462,  839,  456,  725,    0,
  442,   44,  456,  454, 1237,  487, 1716,  456,  454, 1217,
   59,  462,  503,  644,  456, 1138,   59,  643,  744,  456,
 1428, 1433,  462,  443,  722,    0, 1701,  722,  442, 1253,
  413,  707,  504,   41,  732,  462, 1536,  438,  258,  399,
   41,  462,  456,   44,  504,  462,  442,  617,  617,  462,
  420,  460,  664, 1553,  462,  433,  669,  460,   59,  433,
 1199,  460,  460,  462,  740,  741,   41,  433,  460,   44,
   41,  460,  262,   44,  462,  385,  264,  369,  449,  341,
 1219,    0,  460,  503,   59,  468,  460,  307,   59,  319,
  339,  722,  887,  305,  460,  287,  727,    0,  341,  349,
  389, 1369,  981,  734,  369,  868,  264,  389,  871,  398,
  716,  341,  718,  389,  736,  737,  398,  264,  321,  846,
  847,  848, 1281,  754,  851,  852,  853,  349,  855,  856,
  857,  858,  859,  860, 1634, 1044,  332,  198,   41,  319,
   59,   44,   12, 1643, 1321, 1655,  343,   41,  319, 1655,
    0,  782,  783, 1655, 1655,  329,   59,  404,  332,  999,
  887,  341, 1655,  301,    0,  743, 1045,  894,  895,  273,
  897, 1011,  899,   42,  302,  902,  903,  904,  905,  906,
  907,  329,  379, 1683,  332,   41,  884,  389,   41,    0,
    0, 1031,  919,  389,  921,  444,  344,   59,  388,  994,
  420,  994,  398,  781,    0,   41, 1001, 1057, 1001,   59,
  407,  399,  818,  410,    0,  353,  329,   12,  788,  332,
  262, 1360,  828,   59,  398,  320,  343, 1320,  289,  426,
  800,   41,  258, 1469,   44,  813,  814,  815, 1240,  399,
  442,  399, 1584,    0, 1246,    0, 1588,  117,   59,   59,
  398,   61,  399,  884,  872,  452, 1349,  869,   44,    0,
 1327,    0,  379,   59,  273,  873,  870,  462,  442,  839,
  319,  463,  464,   59,    0,   41, 1185,  278,  438,  280,
  329,  307,  456,  332,   41,  398,  329,  288,    0,  332,
  407, 1141,  341,  410,  442,    0,   41,    0,  268,    0,
 1642,  344,   59,  278,   59,  280,  454,  402,  456,  426,
  258,  769,  472,  288, 1209,   41,  444,    0,   59, 1590,
   59,  329,  117,    0,  332, 1637,   42,    0,    0,  442,
  462,   47,  341,   59,  260,  452, 1131,  308,  309,    0,
  389,  454, 1381,  456,    0,  264,  388,   59,  443,  398,
 1077,    0,  462, 1029,   59,  398,   59,   44,   59,  307,
 1255,   44, 1089, 1090, 1403, 1092, 1093,   41,  262,   41,
   44, 1258,   44, 1100, 1686,  278,   59,  280,  304,  380,
   41,  389,   59,   44,  385,  288,   59,   59, 1730, 1229,
  398,  260,   41,  442,  420,  437,  329,  367,   59,  442,
   61,  402,    0,   59, 1131,  380,   93,  456,  409,  262,
   59,  454,    0,  456,  264, 1727, 1047,  278, 1145,   93,
 1260, 1456,    0, 1054, 1660,  462, 1153,  402, 1155,  344,
 1493, 1158,  462, 1160,  409,  304, 1471, 1164, 1165, 1166,
 1167,  412,  443,   41, 1327, 1172, 1327, 1242,  258, 1242,
 1589,    0,  284, 1023, 1023,  370, 1126, 1127, 1128,  320,
 1038,   59,   44,    0,  274, 1701, 1261, 1362,  443,  530,
 1609,   59,  420,    0, 1613, 1173,  332,  380, 1056,  462,
  399,   59,  453,  319, 1062, 1212, 1395, 1366,  308,  309,
  438,  301,  308,  309,  388,  411,  262,  307, 1155,  402,
  310, 1158,  503, 1359, 1413,  341,  409,    0,  319,  264,
   59,   93,  445,  428,    0,  462,   44,  262,  407, 1195,
 1555, 1197,   59,   44,  472,  264, 1321,  462,  503,  462,
  341,    0,   59,  389, 1261,  388,  322,  278,  599,  280,
  443,  402,  398,  329, 1384,    0,  332, 1005, 1006,  399,
  336, 1278,  264, 1646,  386, 1513, 1126, 1127,  344,  264,
 1287,  264,  319,  264,  280,   93,   59, 1294,  288,  458,
  286, 1193,   93,   59,  360,  332, 1154,  295,  389, 1370,
 1256,  301,  443, 1423,  341,   40,   41,  398,  306,   44,
   59,  495,  404,  319, 1321,  313, 1054,  258,    0,  385,
  503,  505,  258,  413,   59,  278,   61,  280,  407,  441,
  420, 1647,  398, 1649, 1341,  341,  277,  278, 1345,  280,
  338,  261,  388,  279,  285, 1416,  458,  288,  438,  415,
    0,  442,  389,  453,  420,  447,   91,  453, 1777,   41,
  301,  398,   44,  388,  399,  301,  307,  319, 1543, 1271,
  263,  307, 1688, 1275, 1549, 1690,  442,   59,  468,  458,
  332, 1359,  472,  389,  379, 1235, 1235, 1639,  454,  341,
  456,   41,  398,  460,  330, 1662,  260,   42,   43, 1377,
   45,  260,   47,  462, 1341,  442,  404,  399, 1345,   59,
  349,  265,    0,  349,  399, 1617,  399,  509,  399,  456,
  313,  357,  358,  462,    0,  420,  272,  380,  460,  365,
 1550,  324, 1684,  323, 1441, 1442,  442,  389,  436,  380,
  304,  319,  440,  289, 1760,  304,  398,  383,  462,  303,
  456,  329,   40,   41,  332, 1591,   44,  260,    0, 1466,
 1662,  402, 1469,  341,  385, 1376, 1377, 1369,  409,    0,
  344,   59,  467,   61, 1726, 1742, 1714,  418,  460,  420,
  462, 1617, 1376,   59,  420,  343,   42,   43,  334,   45,
  442,   47,  329,  380, 1501,  431,  370,  438,    0, 1385,
 1767,  304,  443,   91,  456, 1455, 1758, 1709, 1746, 1747,
   41,  389,  279,   44,  343,  402,   45,   59,  364,  462,
  398,  379,  409,  258,  463,  464, 1662,  445,   59, 1466,
   61,  472, 1488, 1771,   63, 1513,  429, 1691, 1692,  274,
 1742,    0,  277,  278,  462,  280, 1784,  495,  496,  407,
  285, 1558,  410,  288,  428,  343,  443,   59,  338, 1566,
   91,    0,  323,  330,  442, 1767,  301,  404,  426,  403,
  319,    0,  307, 1709, 1728,  310,  454,    0,  407,  322,
 1655,  410, 1655,  332,    0,  431, 1494,    0,  462, 1534,
  265,  379,  341,  336,  452, 1602,  376,  426,  365, 1606,
   59,    0, 1513,  440,   40, 1455, 1742,    0,  445,  389,
    0,  385, 1462, 1591,  451,  302,  383,  360,   41,  407,
   59,   44,  410,  452, 1778,  462,  280,  460,  303,   44,
   59, 1767,  286,  470, 1535,  280,   59,  319,  426, 1617,
  389,  286,  385,   59, 1530,  380,   59,  329,   41,  398,
  332,   44,  339, 1660,  487, 1497,    0, 1559,    0,  341,
   59, 1563,  344,  338,  452,  387,   59,  402,   61,   59,
  258,  385,  415,  436,  409,    0,  321,  258,  413,  329,
  261,    0,  332, 1628, 1662,  420,  274,  349, 1554,  277,
  278,    0,  280,  442, 1701,  405,  358,  285,   91,  462,
  288,  302,  278,  438,  280,  385,  322,  389,  443,  438,
  407, 1603,  422,  301,    0,   59,  398,   59,    0,  307,
  336,  496,  310,  357,  280,  338,  307,  258,  344,  426,
  286, 1709,   41,  468,   59,   44, 1714,  472,  339,  389,
   59,  420,    0,  274,  360,  217,  277,  278,  398,  280,
   59,    0,   61,  225,  285,   41,    0,  288,   44,  462,
  442,  395, 1670,  397, 1742,  321,  274,  296, 1746, 1747,
  301, 1637,  454,   59,  319,    0,  307,   59,  426,  310,
   42,   43,   91,   45,  260,   47,  420,    0, 1654, 1767,
  322,   41,  380, 1771,   44,  322,  463,   41,    0,  415,
   44,   59,  353, 1714,  336,    0, 1784,    0,  409,  336,
   59,  445,  344,   40,  402,   59,   41,   61,  419,   44,
 1686,  409, 1672, 1672,  353,  413,  302, 1693,  360,  380,
  264,    0,  420,  360,   59, 1746, 1747, 1738,  314,   41,
  279,   41,   44,  444,   44,    0,   59,   91,   41,  380,
  438,   44,  328,   42,   43,  443,   45,   59,   47,   61,
 1771, 1727,  301,  339,   59,  258,   59,  419,   61, 1735,
    0,  402,  317, 1784,   42,   43,  355,   45,  409,   47,
  468,  274,  413,  415,  472,  278,  319,  280,  415,  420,
   59,  330,  285,  462,  446,  288,  319,   40,    0,  467,
   43,  469,   45,  319,   59,    0,  329,  438,  301,  332,
  349,    0,  443,  329,  307,   58,  332,  310,  341,  358,
   63,  344, 1731, 1732,   41,  341,  365,   44,  698,   59,
  459,  460,  461,  409,  463,  464,    0,  468,    0,  337,
 1120,  472, 1122,  419,  383,   42,   43,   44,   45,  258,
   47,  721,    0,  482,  483,  338,  259,   59,  487,  488,
   45,  260,  454,  733,   59,  274,  389,  270,  444,  278,
   59,  280,  404,  389,  259,  398,  285, 1786,  841,  288,
  843,  460,  398,  462,  341,  270,  462,  380,   41,  317,
  318,   44,  301,   41,  473,   59,   44,   59,  307,   41,
  262,  310,   44,  302,   41,   42,   43,   40,   45,  402,
   47,   59,  319,  492,  258,  314,  409,  321,  280,  442,
  413,  500,  501,  502,  286,   41,  442,  420,   44,   41,
  274,  454,   44,  404,  278,   41,  280,  385,   44,  322,
  339,  285,  385,   41,  288,  438,   44,  508,   41,   40,
  443,   44,   43,  336,   45,  363,  258,  301,   41,   42,
   43,  438,   45,  307,   47,  258,  310,   58,  463,  464,
  452,  380,   63,  260,  436,  468,  278,  360,  280,  472,
  393,  394,  462,  285,  277,  278,  288,  280,  463,  464,
  419,  280,  285,  402,  444,  288,  288,  286,   41,  301,
  409,   44,  385,   41,  413,  307,   44,  358,  301,   44,
  409,  420,  280,  413,  307,  302,  279,  471,  286,   41,
  419,   61,   44,  312,  267,   41,  374,  314,   44,  438,
  287,   40,  415,  276,  443,  264,  380,   40,  301,  463,
   41,   40,   45,   44,  312,  444,   41,  290,  463,   44,
  293,  463,  339,  296,  297,    0,    0,   44,  402,  468,
   63,  438,  321,  472,  462,  409,  851,  330,  853,  413,
  855,  856,  315,  858,  859,  860,  420,   44,  380,   40,
  404,  324,   40,  280,  327,   40,  349,  380,   40,  286,
   40,  666,   40,  668,  438,  358,  671,  672,  673,  443,
  402,   40,  365,   40,   40,   42,   43,  409,   45,  402,
   40,  296,  355,  356,   59,   59,  409,   40,  420,   40,
  383,   58,  409,   40,  468,  368,   63,  420,  472,  372,
  462,   40,  419,  459,  460,  461,  438,  380,   91,   46,
  321,  443,   44,  280,   44,  438,  430,   46,   61,  286,
  443,   40,  280,  264,   44,   46,  384,  444,  349,   44,
  278,  487,  488,   41,   42,   43,  301,   45,  353,   47,
  472,  278,  264,  399,  322,  462,  267,  462,  438,  472,
  423,  329,  472,  399,  332,  276,  429,  460,  336,   41,
   42,   43,  435,   45,    0,   47,  344,  280,  277,  290,
  449,  449,  445,  286,   40,  296,  297,  401,   40,  350,
    0,   91,  360,   40,   40,  319,  459,  460,  461,  462,
  463,  464,  264,  413,  315,   44,  385,  384,  391,   40,
  473,   40,  438,  324,  466,   41,  327,  385,   44,  482,
  483,  484,  485,  486,  487,  488,  489,  490,  318,  492,
  398,   40,  321,   59,  497,  498,  321,  500,  501,  502,
  445,  453,   41,  506,  355,  356,  462,  415,  454,   59,
  450,  264,  456,  460,  459,  460,  461,  368,  463,  464,
  462,  372,   40,    0,  274,  277,   40,  301,  285,  380,
  345,  345,  278,  296,  442,  420,  418,  482,  483,  484,
  485,  486,  487,  488,  489,  490,  454,  388,  456,   40,
  388,  388,  497,  498,  474,  475,  476,  477,  478,  479,
  480,  481,  301,  260,   41,  420,  357,   44,  419,  419,
  267,   40,  423,  274,  279,  279,  462,  419,  429,  276,
   40,   44,   59,  387,  435,   41,   42,   43,  369,   45,
  353,   47,   44,  290,  445,  321,  301,  301,  264,  296,
  297,   40,   44,   42,   43,   41,   45,  304,  459,  460,
  461,  462,  463,  464,  321,   44,  280,    0,   40,   58,
   44,  504,  473,  329,   63,  330,  330,  324,   44,  266,
  327,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   40,  492,  280,  300,  349,  349,  497,  498,  286,  500,
  501,  502,  357,  358,  358,  506,  353,  503,  355,  356,
  365,  365,   42,   43,   40,   45,  264,   47,  280,  462,
  424,  368,  507,   41,  286,  372,   59,   58,  383,  383,
   41,   41,   41,  264,  456,  396,   40,  310,  494,   43,
  395,   45,  397,  442,  310,   41,  459,  460,  461,   41,
  463,  464,   40,   40,   58,  353,   40,   40,  258,   63,
   40,   40,  278,   40,  280,  420,   40,   40,   40,  482,
  483,   40,  288,   40,  487,  488,   40,   40,   40,  279,
   40,  262,  429,  328,  437,  301,   44,  309,  435,   41,
  445,   41,   42,   43,  390,   45,  390,   47,  445,  390,
  344,  301,   41,   42,   43,  274,   45,  307,   47,  332,
  392,  424,  459,  460,  461,  462,  463,  464,   41,   42,
   43,  393,   45,  448,   47,  271,  473,  288,  409,  388,
  330,  388,  388,  388,  388,  482,  483,  484,  485,  486,
  487,  488,  489,  490,    0,  492,   41,  388,  380,  349,
  497,  498,   41,  500,  501,  502,  278,   40,  358,  506,
   41,   41,   41,  387,  380,  365,  419,  438,   41,  309,
  437,  260,  398,  338,  280,   41,   42,   43,  267,   45,
  286,   47,  339,  383,  444,   41,  402,  276,   44,  339,
  302,  302,  319,  409,  444,  322,   41,   41,  353,  387,
  321,  290,  329,   59,   41,  332,  373,  296,  297,  336,
  373,    0,   41,   41,  341,  304,   40,  344,   41,   43,
  420,   45,   41,   42,   43,  258,   45,  443,   47,   41,
   41,   41,    0,  360,   58,  324,   41,   41,  327,   63,
   41,   41,   41,   41,   41,   41,  279,   41,  280,   44,
  280,   59,   41,   40,  341,   44,  286,   59,  385,   40,
  341,  309,  389,  267,  353,   41,  355,  356,  301,   59,
   59,  398,  276,   41,  307,   59,   44,   42,   43,  368,
   45,   59,   47,  372,   59,   59,  290,   59,  415,  353,
  345,   59,  296,  297,   59,   41,   40,  330,  341,   40,
  370,  453,   43,  385,   45,   44,   59,  385,  462,  385,
   41,  315,   41,   41,   44,  442,  349,   58,  412,  344,
  324,   44,   63,  327,  305,  358,  302,  454,  437,  444,
  280,  389,  365,   40,   59,  462,  286,  294,   44,  305,
  429,  280,  465,  308,   44,   41,  435,  286,    0,  338,
  383,  355,  356,   40,  264,  462,  445,  280,  385,  278,
  264,  385,  460,  286,  368,  321,  264,  472,  372,  391,
  459,  460,  461,  462,  463,  464,  380,   41,   42,   43,
  453,   45,   40,   47,  473,  413,  310,  420,  264,  460,
  344,  438,  437,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  271,  492,  344,  462,  344,  344,  497,  498,
  271,  500,  501,  502,  280,  309,  460,  506,   41,  423,
  286,   40,  278,  309,  280,  429,   40,   59,   41,  285,
   59,  435,  288,  305,  309,   41,  260,   44,   59,  263,
   41,  445,    0,  267,    0,  301,    0, 1123,   12,  252,
  652,  244,  276,  245,  612,  459,  460,  461,  462,  463,
  464,  280,  831, 1404, 1057, 1046,  290,  286,   84,  473,
 1059, 1054,  296,  297,  305,  335,  559, 1036,  482,  483,
  484,  485,  486,  487,  488,  489,  490,  300,  492,  278,
  984,  280,  727,  497,  498,  317,  500,  501,  502,  288,
  324,    0,  506,  327, 1600,  291, 1047, 1382,  317, 1677,
  278, 1384,  280,  764, 1681,  280, 1718,  285,  538,  260,
  288,  286,  263, 1680,  380, 1210,  267,  754, 1770, 1622,
  370,  355,  356,  301, 1715,  276, 1187, 1363,    0,  968,
 1637,  310,   41,  322,  368,   44,  402,   40,  372,  290,
   43,  303,   45,  409,  540,  296,  297, 1244,  810,  598,
   59,  404,   41,   42,   43,   58,   45,   13,   47,  802,
   63,  472,  716,  278,   41,   42,   43,  476,   45,   41,
   47,  732,   44,  324,  971,  122,  327,  443,    0,  703,
 1591,  380, 1735,  945,  702, 1484, 1787,   59, 1693,  660,
  488, 1091,  888,  427, 1160,  429,  369,   -1,   -1,   -1,
   -1,  435,  380,  402,  355,  356,  280,   -1,   -1,   -1,
  409,  445,  286,   -1,   -1,   -1,    0,  368,   -1,   41,
   -1,  372,   44,   -1,  402,  459,  460,  461,  462,  463,
  464,  409,   -1,    0,   -1,   -1,   -1,   59,   -1,  473,
   42,   43,   -1,   45,  443,   47,   -1,   -1,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   41,  492,   -1,
   44,    0,   -1,  497,  498,  443,  500,  501,  502,   -1,
   -1,   -1,  506,   -1,   41,   59,  427,   44,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,  445,   41,   42,   43,   -1,   45,
   -1,   47,   41,   -1,   -1,   44,   -1,   -1,  459,  460,
  461,  462,  463,  464,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,  473,   41,   42,   43,   -1,   45,   -1,   47,
   -1,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,    0,   -1,   -1,   -1,  497,  498,   -1,  500,
  501,  502,   -1,  262,   -1,  506,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   41,   42,   43,   -1,
   45,   -1,   47,  276,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  290,   -1,   -1,
   -1,  280,   -1,  296,  297,   42,   43,  286,   45,   58,
   47,   59,   -1,  280,   63,   -1,  278,   -1,  280,  286,
  319,   -1,   59,  322,   -1,   -1,  288,   -1,   -1,   41,
  329,  324,   44,  332,  327,   -1,   -1,  336,   -1,  301,
   -1,   -1,  341,   -1,   -1,  344,   -1,   59,   -1,   -1,
  262,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,  360,  355,  356,   42,   43,   -1,   45,   -1,   47,
  262,   -1,   -1,   -1,   -1,  368,   -1,   40,   -1,  372,
   43,   59,   45,   -1,   -1,   -1,  385,   -1,  280,  388,
  389,   -1,   -1,   -1,  286,   58,   -1,   -1,   -1,  398,
   63,   -1,   -1,   -1,  278,   -1,  280,  319,   -1,   -1,
  322,   -1,   -1,   -1,  288,   -1,  415,  329,  380,   -1,
  332,  278,   -1,  280,  336,   -1,   -1,   -1,   -1,  341,
   -1,   -1,  344,   -1,  427,   -1,  429,   -1,  437,   -1,
  402,   -1,  435,  442,  280,   -1,   -1,  409,  360,  278,
  286,  280,  445,   -1,   -1,  454,   -1,  456,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  459,  460,  461,  462,
  463,  464,  280,  385,   -1,   -1,  388,  389,  286,   -1,
  473,  443,   -1,   -1,   -1,   -1,  398,   -1,   -1,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,   -1,  415,  497,  498,  380,  500,  501,  502,
  258,  260,   -1,  506,  263,  280,   -1,   -1,  267,   -1,
   -1,  286,   -1,  380,   -1,  437,   -1,  276,  402,   -1,
  442,  279,   -1,   -1,   -1,  409,   -1,   -1,   -1,   -1,
   -1,  290,  454,  280,  456,  402,  258,  296,  297,  286,
   -1,  380,  409,  301,   -1,   -1,   -1,   -1,   40,  307,
   -1,   43,   -1,   45,   -1,   -1,   -1,  279,   -1,  443,
   -1,   -1,   -1,  402,   -1,  324,   58,   -1,  327,   -1,
  409,   63,  330,   -1,   -1,   -1,  443,   -1,   -1,  301,
  280,   -1,   -1,   -1,   -1,  307,  286,  260,   -1,   -1,
  263,  349,  280,   -1,  267,   -1,  355,  356,  286,   -1,
  358,   -1,   -1,  276,  443,   -1,   -1,  365,  330,  368,
   -1,   -1,   -1,  372,   -1,   -1,   -1,  290,   -1,   -1,
   -1,   -1,   -1,  296,  297,  383,   -1,  349,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,  358,   -1,   -1,   -1,
   -1,   -1,   -1,  365,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  324,   63,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,  383,  420,   -1,   -1,   -1,   -1,   -1,  427,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,  355,  356,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,  420,  372,
  459,  460,  461,  462,  463,  464,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  473,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,  498,
   -1,  500,  501,  502,   -1,   -1,   -1,  506,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,  260,   -1,
   -1,  263,  445,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,  459,  460,  461,  462,
  463,  464,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
  473,    0,   -1,   -1,  296,  297,   -1,   -1,   -1,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,  498,   -1,  500,  501,  502,
   -1,   -1,  324,  506,   -1,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   59,   -1,   -1,  355,  356,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  368,   -1,   40,  290,
  372,   43,   -1,   45,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,  429,   60,   61,
   62,   -1,   -1,  435,  355,  356,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  445,   -1,   -1,   -1,  368,   -1,   -1,
   -1,  372,   -1,   -1,   -1,   -1,   -1,  459,  460,  461,
  462,  463,  464,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  473,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,  497,  498,   -1,  500,  501,
  502,   -1,   -1,   -1,  506,   -1,  427,   -1,  429, 1279,
 1280,   -1,   -1,   -1,  435, 1285, 1286,   -1, 1288, 1289,
   -1,   -1,   -1,   -1,  445,   -1, 1296, 1297, 1298, 1299,
   -1, 1301, 1302, 1303, 1304, 1305, 1306,   -1,  459,  460,
  461,  462,  463,  464,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  473,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  497,  498,   -1,  500,
  501,  502,   -1,   -1,   -1,  506,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
  319,  295,   -1,  322,   58,   -1,   -1,   -1,  290,   63,
  329,   -1,  306,  332,  296,  297,   -1,  336,   -1,  313,
   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,  306,   -1,
  272,   -1,  326,   -1,   -1,  313,   -1,   -1,  280,  317,
  318,  360,  324,   -1,  286,  327,   -1,  289,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  338,   -1,   -1,   -1,   -1,   -1,  385,   -1,   -1,  388,
  389,   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,  325,   40,   -1,  368,   43,   -1,   45,
  372,   -1,  334,   -1,  388,   -1,  415,   -1,   -1,   -1,
  342,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
  404,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,  361,
   -1,   -1,  364,  442,   -1,   -1,  404,  421,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  454,  378,  456,  380,  381,
  382,   -1,  436,  421,   -1,  427,  440,  429,   -1,  443,
   -1,   -1,   -1,  435,   -1,   -1,   -1,  451,  436,   -1,
   -1,   -1,  440,  445,   -1,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,  451,   -1,   -1,  470,  459,  460,  461,
  462,  463,  464,   -1,   -1,   58,   -1,   -1,   -1,  431,
   63,  473,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  267,   -1,  497,  498,   -1,  500,  501,
  502,   -1,  276,   -1,  506,   -1,   -1,   -1,   -1,   -1,
  261,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  283,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  291,   -1,   -1,   -1,   -1,   -1,   -1,  298,   -1,  300,
  324,  302,   -1,  327,   -1,   -1,  307,   -1,   -1,   -1,
   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  353,
   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,  267,   -1,   -1,  368,   -1,   -1,   -1,  372,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,
  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,
   -1,  327,   60,   61,   62,  429,  407,   -1,   -1,   -1,
   -1,  435,   -1,  414,   -1,   -1,  417,   -1,  419,  420,
   -1,  445,   -1,   -1,  267,   -1,   -1,  353,   -1,  355,
  356,   -1,   -1,  276,   -1,  459,  460,  461,  462,  463,
  464,   -1,  368,  444,   -1,   -1,  372,  290,   -1,  473,
   -1,   -1,   40,  296,  297,   43,   -1,   45,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   58,   -1,   -1,  497,  498,   63,  500,  501,  502,   -1,
   -1,  324,  506,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  493,  419,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,  508,   -1,  435,
  353,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,  372,
   -1,   -1,   -1,  459,  460,  461,  462,  463,  464,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  473,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   58,   -1,
   -1,  497,  498,   63,  500,  501,  502,   -1,   -1,   -1,
  506,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,  459,  460,  461,  462,
  463,  464,  280,   -1,   -1,   -1,   -1,   -1,  286,   -1,
  473,  289,   -1,   40,   -1,   -1,   43,   -1,   45,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   58,   -1,   -1,  497,  498,   63,  500,  501,  502,
   -1,   -1,   -1,  506,   -1,   -1,   -1,  325,   -1,   -1,
   -1,   -1,  260,   -1,   -1,   -1,  334,   -1,   -1,  267,
   -1,   -1,   -1,   -1,  342,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,  361,   -1,   -1,  364,   -1,  296,  297,
   -1,   -1,   -1,   -1,   -1,   -1,  304,   -1,   -1,   -1,
  378,   -1,  380,  381,  382,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,
   -1,   -1,   -1,  431,   -1,   -1,   -1,  267,   -1,   -1,
  368,   -1,   -1,   -1,  372,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  324,   -1,   63,  327,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,
  267,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,  276,
   -1,  459,  460,  461,  462,  463,  464,   -1,  368,   -1,
   -1,   -1,  372,  290,   -1,  473,   -1,   -1,   -1,  296,
  297,   -1,   -1,   -1,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,
  498,   -1,  500,  501,  502,   -1,   -1,  324,  506,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,  353,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,  445,   -1,  267,   -1,   -1,
   -1,  368,   -1,  453,   -1,  372,  276,   -1,   -1,  459,
  460,  461,  462,  463,  464,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,  473,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,   -1,   -1,  497,  498,   -1,
  500,  501,  502,   -1,  324,   -1,  506,  327,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
  267,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,  276,
   -1,   -1,  459,  460,  461,  462,  463,  464,  368,   -1,
   -1,   -1,  372,  290,   -1,   -1,  473,   -1,   -1,  296,
  297,   -1,   -1,   -1,   -1,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   40,  492,   -1,   43,   -1,   45,
  497,  498,   -1,  500,  501,  502,   -1,  324,   -1,  506,
  327,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,  419,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,  372,   -1,   -1,   -1,  459,
  460,  461,  462,  463,  464,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  473,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   40,  492,   -1,   43,   -1,   45,  497,  498,   -1,
  500,  501,  502,   -1,   -1,   -1,  506,   -1,   -1,   58,
   -1,   -1,  429,   -1,   63,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  459,  460,  461,  462,  463,  464,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  473,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
  497,  498,  273,  500,  501,  502,  277,   -1,   -1,  506,
   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  296,   -1,   -1,  299,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,   -1,  340,
  296,  297,   -1,   -1,   -1,  346,  347,  348,   -1,  350,
  351,  352,   -1,  354,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  324,   -1,
   -1,  327,   -1,   -1,  375,   -1,  377,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,
  356,    0,   -1,   -1,   -1,   -1,   -1,  408,  267,   -1,
   -1,   -1,  368,   -1,   -1,   -1,  372,  276,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   40,   -1,   -1,   43,   -1,
   45,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,   -1,   41,   58,   59,   44,  447,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,  462,   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
   -1,  482,  483,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,  355,  356,  499,   -1,
  456,   -1,   -1,  459,  460,  461,  462,  463,  464,  368,
   -1,   -1,   -1,  372,   -1,   -1,   -1,  473,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  497,  498,   -1,  500,  501,  502,   -1,   -1,   -1,
  506,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  419,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
  459,  460,  461,  462,  463,  464,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,  473,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,  498,
   -1,  500,  501,  502,   -1,  324,   -1,  506,  327,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   40,  262,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  290,  355,  356,   -1,   -1,
   58,  296,  297,   -1,   -1,   63,   -1,   -1,   -1,  368,
   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  319,   -1,   -1,  322,   -1,   -1,   40,   -1,   -1,   43,
  329,   45,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
  355,  356,  341,   -1,   58,  344,   -1,   -1,   -1,   63,
  429,   -1,   -1,  368,   -1,   -1,  435,  372,   -1,   -1,
   -1,  360,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  456,   -1,   -1,
  459,  460,  461,  462,  463,  464,  385,   -1,   -1,  388,
  389,   -1,   -1,   -1,  473,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,  429,   -1,  415,   -1,  497,  498,
  435,  500,  501,  502,   -1,   -1,   -1,  506,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   -1,
   -1,   -1,   -1,  442,  459,  460,  461,  462,  463,  464,
   -1,   -1,   -1,   -1,   -1,  454,   -1,  456,  473,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  497,  498,   -1,  500,  501,  502,   -1,  267,
  302,  506,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,  313,  314,   -1,  316,   -1,   40,  319,   -1,   43,
   -1,   45,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,
    0,  333,   -1,   -1,   58,   -1,   -1,  339,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,
  362,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,  276,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  353,  290,  355,  356,   59,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,  400,   -1,
  368,   -1,   -1,   -1,  372,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  355,  356,  455,   -1,   -1,   -1,   -1,   -1,   -1,
  462,  429,   -1,   -1,  368,   -1,   -1,  435,  372,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  491,
   -1,  459,  460,  461,  462,  463,  464,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  473,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,  429,   -1,   -1,   -1,  497,
  498,  435,  500,  501,  502,   -1,   -1,   -1,  506,   -1,
   -1,  445,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,  459,  460,  461,  462,  463,
  464,   -1,   -1,   -1,    0,   -1,  290,   -1,   -1,  473,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,   -1,  262,  497,  498,   -1,  500,  501,  502,   -1,
  324,   -1,  506,  327,   40,   41,   42,   43,   44,   45,
   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  368,   -1,   -1,   -1,  372,  319,
   -1,   -1,  322,   -1,   -1,   91,   -1,   93,   -1,  329,
   -1,   -1,  332,   -1,  271,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,
   -1,   -1,  309,   -1,   -1,  429,  313,  314,   -1,  316,
   -1,  435,  319,   -1,   -1,  385,   -1,   -1,  388,  389,
   -1,  445,   -1,   -1,   -1,   -1,  333,   -1,  398,   -1,
   -1,   -1,  339,   -1,   -1,  459,  460,  461,  462,  463,
  464,   -1,   -1,   -1,   -1,  415,   -1,    0,   -1,  473,
   -1,   -1,   -1,   -1,   -1,  362,   -1,   -1,  482,  483,
  484,  485,  486,  487,  488,  489,  490,  437,  492,   -1,
   -1,   -1,  442,  497,  498,   -1,  500,  501,  502,   -1,
   -1,   -1,  506,   -1,  454,   -1,  456,   -1,   41,   42,
   43,   44,   45,  400,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,  419,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,  434,  264,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  272,  444,   -1,   -1,
   93,   -1,   -1,   -1,  280,   -1,  453,   -1,  455,   -1,
  286,   -1,   -1,  289,   -1,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  308,  309,   -1,   -1,  312,   -1,   -1,   -1,
   -1,   -1,   -1,  319,  491,  321,  322,   -1,   -1,  325,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,  334,   -1,
  336,   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,  361,   -1,   -1,  364,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  378,   -1,  380,  381,  382,   -1,   -1,  385,
   -1,   -1,  388,  389,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,  412,   47,   -1,  415,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  431,   -1,   -1,   -1,   -1,
   -1,  437,   -1,   -1,   -1,   -1,  442,   -1,   -1,  262,
   -1,  264,   -1,   -1,   -1,   -1,   -1,  453,  454,  272,
  456,   -1,   -1,   93,   -1,   -1,  462,  280,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  308,  309,   -1,   -1,  312,
   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,  321,  322,
   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,
   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,  360,  361,   -1,
   -1,  364,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  378,   -1,  380,  381,  382,
   -1,   -1,  385,   -1,   -1,  388,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,  302,   41,   42,   43,
   44,   45,   -1,   47,   -1,   -1,   -1,  313,  314,  412,
  316,   -1,  415,  319,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  333,  431,   -1,
   -1,   -1,   -1,  339,  437,   -1,   -1,   -1,   -1,  442,
   -1,   -1,  262,   -1,  264,   -1,   -1,   -1,   -1,   93,
  453,  454,  272,  456,   -1,   -1,  362,   -1,   -1,  462,
   -1,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  308,  309,
   -1,   -1,  312,   -1,  400,   -1,   -1,   -1,   -1,  319,
   -1,  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,
   -1,   -1,  332,  419,  334,   -1,  336,   -1,   -1,   -1,
   -1,  341,  342,   -1,  344,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,    0,
  360,  361,   -1,   -1,  364,   -1,   -1,  453,   -1,  455,
   -1,   -1,   -1,   -1,   -1,   -1,  462,   -1,  378,   -1,
  380,  381,  382,   -1,   -1,  385,   -1,   -1,  388,  389,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,
   41,   42,   43,   44,   45,  491,   47,   -1,   -1,   -1,
   -1,   -1,  412,   -1,   -1,  415,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  431,   -1,   -1,   -1,   -1,   -1,  437,  262,   -1,
  264,   -1,  442,   -1,   -1,   -1,   -1,   -1,  272,   -1,
    0,   -1,   93,  453,  454,   -1,  456,   -1,   -1,   -1,
   -1,   -1,  462,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  308,  309,   -1,   -1,  312,   -1,
   -1,   41,   -1,   -1,   44,  319,   -1,  321,  322,   -1,
   -1,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,   59,
  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,   -1,
  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,  360,  361,   -1,   -1,
  364,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  378,   -1,  380,  381,  382,   -1,
   -1,  385,   -1,   -1,  388,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  398,   -1,   41,   -1,   43,   44,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  412,   -1,
   -1,  415,   -1,   -1,   59,   60,   61,   62,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  431,   -1,   -1,
   -1,   -1,   -1,  437,   -1,   -1,   -1,   -1,  442,   -1,
   -1,  262,   -1,  264,   -1,   -1,   -1,   -1,   93,  453,
  454,  272,  456,   -1,   -1,   -1,   -1,   -1,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  308,  309,   -1,
   -1,  312,   -1,    0,   -1,   -1,   -1,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,
   -1,  332,   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,
  341,  342,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   41,   -1,   43,   44,   45,  360,
  361,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,  378,   -1,  380,
  381,  382,   -1,   -1,  385,   -1,   -1,  388,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   93,   -1,   -1,  319,
   -1,  412,  322,   -1,  415,   -1,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,
  431,  341,   -1,   -1,  344,   -1,  437,  262,   -1,  264,
   -1,  442,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,
  360,   -1,  453,  454,   -1,  456,   -1,   -1,   -1,   -1,
   -1,  462,   -1,   -1,  289,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,  385,   -1,   -1,  388,  389,
   -1,   -1,   -1,  308,  309,   -1,   -1,  312,  398,   -1,
   -1,   -1,   -1,   -1,  319,   -1,  321,  322,   -1,   -1,
  325,   -1,   -1,   -1,  329,  415,   -1,  332,   -1,  334,
   41,  336,   43,   44,   45,   -1,  341,  342,   -1,  344,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  437,   59,   60,
   61,   62,  442,   -1,   -1,  360,  361,   -1,   -1,  364,
   -1,   -1,   -1,   -1,  454,   -1,  456,   -1,   -1,   -1,
   -1,   -1,   -1,  378,   -1,  380,  381,  382,   -1,   -1,
  385,   -1,   93,  388,  389,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,  262,   -1,  264,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,  412,   -1,   -1,
  415,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,  431,    0,   -1,   -1,
   -1,   -1,  437,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,  308,  309,   -1,   -1,  312,   -1,   -1,  453,  454,
   -1,  456,  319,   -1,  321,  322,   -1,  462,  325,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  334,   41,  336,
   43,   44,   45,   -1,  341,  342,   -1,  344,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,  360,  361,   -1,   -1,  364,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,  378,   -1,  380,  381,  382,   -1,   -1,  385,   -1,
   93,  388,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,  412,   -1,   -1,  415,   41,
   -1,  262,   44,  264,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,  431,   -1,   -1,   59,   -1,   -1,
  437,   -1,   -1,   -1,   -1,  442,   -1,   -1,  289,   -1,
   -1,   -1,   41,    0,   -1,   44,  453,  454,   -1,  456,
   -1,   -1,   -1,   -1,   -1,  462,   -1,  308,  309,   -1,
   59,  312,   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,
   -1,  332,   -1,  334,   41,  336,   -1,   44,   -1,   -1,
  341,  342,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,  360,
  361,   -1,   -1,  364,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,  380,
  381,  382,   -1,   -1,  385,   -1,   -1,  388,  389,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  398,   -1,  262,
   -1,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,  412,   -1,   -1,  415,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,  289,   -1,   -1,   -1,
  431,   -1,   -1,   -1,   -1,   -1,  437,   -1,   -1,   -1,
   -1,  442,   -1,   -1,    0,  308,  309,   -1,   -1,  312,
   -1,   -1,  453,  454,   -1,  456,  319,   -1,  321,  322,
   -1,  462,  325,   -1,   41,    0,  329,   44,   -1,  332,
   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,
  262,  344,   59,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,  361,   -1,
   -1,  364,   -1,   59,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,   -1,  262,   -1,  378,   -1,  380,  381,  382,
   -1,   -1,  385,   -1,   59,  388,  389,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  398,   -1,  319,   -1,   -1,
  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,  412,
  332,   -1,  415,   -1,  336,  262,   -1,   -1,   -1,  341,
   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,  431,   -1,
  319,   -1,   -1,  322,  437,   -1,   -1,   -1,  360,  442,
  329,   -1,    0,  332,   -1,   -1,   -1,  336,   -1,   -1,
  453,  454,  341,  456,   -1,  344,   -1,   -1,   -1,  462,
   -1,   -1,   -1,  385,   -1,   -1,  388,  389,   -1,   -1,
   -1,  360,  319,   -1,   -1,  322,  398,   -1,   -1,   -1,
   -1,   -1,  329,   41,   -1,  332,   44,   -1,   -1,  336,
   -1,   -1,   -1,  415,  341,   -1,  385,  344,   -1,  388,
  389,   59,   -1,    0,   -1,   -1,   -1,   -1,   -1,  398,
   -1,   -1,   -1,  360,    0,  437,   -1,   -1,   -1,   -1,
  442,   -1,   -1,   -1,   -1,   -1,  415,   -1,   -1,   -1,
   -1,   -1,  454,   -1,  456,   -1,   -1,    0,  385,   -1,
   -1,  388,  389,   -1,   41,  262,   -1,   44,  437,   -1,
   -1,  398,   -1,  442,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   59,   -1,   -1,  454,  262,  456,  415,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
  437,   -1,   -1,   -1,   -1,  442,   59,   -1,   -1,   -1,
   -1,   -1,  319,   -1,   -1,  322,   -1,  454,   -1,  456,
   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,
   -1,   -1,   -1,  319,  341,   -1,  322,  344,   -1,   -1,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,
  336,   -1,   -1,  360,  319,  341,   -1,  322,  344,   -1,
   -1,   -1,   -1,    0,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,   -1,   -1,  360,   -1,  341,   -1,  385,  344,
   -1,  388,  389,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,   -1,  360,    0,   -1,   -1,  385,
   -1,   -1,  388,  389,   41,   -1,   -1,   44,  415,   -1,
   -1,   -1,  398,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  385,   -1,   59,  388,  389,   -1,   -1,   -1,   -1,  415,
  437,   -1,   -1,  398,   -1,  442,   -1,   41,    0,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,  456,
  415,  437,   -1,   -1,   -1,   59,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,
  456,  319,  437,   -1,  322,  262,   -1,  442,   -1,   41,
    0,  329,   44,   -1,  332,   -1,  262,   -1,  336,  454,
   -1,  456,   -1,  341,   -1,   -1,  344,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  319,   -1,   -1,  322,   -1,  385,   -1,   59,
  388,  389,  329,  319,   -1,  332,  322,   -1,   -1,  336,
  398,   -1,   -1,  329,  341,    0,  332,  344,   -1,   -1,
  336,   -1,   -1,   -1,   -1,  341,  319,  415,  344,  322,
   -1,   -1,   -1,  360,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   -1,  336,  360,   -1,   -1,   -1,  341,  437,
   -1,  344,   -1,   -1,  442,   -1,   41,    0,  385,   44,
   -1,  388,  389,   -1,   -1,   -1,  454,  360,  456,  385,
   -1,  398,  388,  389,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  398,   -1,   -1,  262,   -1,   -1,  415,   -1,
   -1,   -1,  385,   -1,   -1,  388,  389,   -1,   41,  415,
   -1,   44,   -1,   -1,   -1,  398,   -1,    0,   -1,   -1,
  437,   -1,   -1,   -1,   -1,  442,   59,   -1,  262,   -1,
   -1,  437,  415,   -1,   -1,   -1,  442,  454,   -1,  456,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,
  456,   -1,  319,   -1,  437,  322,   -1,   -1,   41,  442,
   -1,   44,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,
  262,  454,   -1,  456,  341,   -1,   59,  344,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,   -1,  322,   -1,
   -1,   -1,   -1,  360,   -1,  329,   -1,   -1,  332,   -1,
   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,   -1,
  344,   -1,  262,   -1,   -1,   -1,   -1,   -1,  385,   -1,
   -1,  388,  389,   -1,   -1,   -1,  360,  319,   -1,   -1,
  322,  398,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,
  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,  415,  341,
   -1,  385,  344,   -1,  388,  389,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  398,   -1,   -1,   -1,  360,  319,
  437,   -1,  322,   -1,   -1,  442,   -1,   -1,   -1,  329,
   -1,  415,  332,   -1,   -1,   -1,  336,  454,   -1,  456,
   -1,  341,   -1,  385,  344,   -1,  388,  389,   -1,   -1,
   -1,   -1,   -1,  437,   -1,   -1,  398,   -1,  442,   -1,
  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  454,   -1,  456,  415,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  385,   -1,   -1,  388,  389,
   -1,   -1,   -1,   -1,  319,  437,   -1,  322,  398,   -1,
  442,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,  454,   -1,  456,  415,  341,   -1,   -1,  344,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  360,  319,  437,   -1,  322,
   -1,   -1,  442,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   -1,  336,  454,   -1,  456,   -1,  341,   -1,
  385,  344,   -1,  388,  389,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,  398,   -1,   -1,   -1,  360,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  319,  292,   -1,  322,
  415,  296,   -1,   -1,  299,   -1,  329,   -1,   -1,  332,
  273,   -1,  385,  336,  277,   -1,  389,   -1,  341,   -1,
   -1,  344,  437,   -1,   -1,  398,   -1,  442,   -1,  292,
   -1,   -1,   -1,  296,   -1,   -1,  299,  360,   -1,  454,
   -1,  456,  415,   -1,   -1,  340,   -1,   -1,   -1,   -1,
   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,  354,
   -1,   -1,  385,   -1,   -1,   -1,  389,   -1,   -1,  442,
   -1,   -1,   -1,   -1,   -1,  398,   -1,  340,   -1,   -1,
  375,  454,  377,  346,  347,  348,   -1,  350,  351,  352,
   -1,  354,  415,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  375,  408,  377,   -1,   -1,  296,  413,  442,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
  425,  454,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  408,   -1,  292,   -1,   -1,
   -1,  296,  447,   -1,  299,   -1,   -1,   -1,   -1,   -1,
  273,  340,  425,   -1,  277,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  296,  447,   -1,  299,  482,  483,   -1,
   -1,   -1,   -1,   -1,   -1,  340,  375,   -1,  377,   -1,
   -1,  346,  347,  348,  499,  350,  351,  352,   -1,  354,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  482,
  483,   -1,   -1,   -1,   -1,   -1,   -1,  340,   -1,  408,
  375,   -1,  377,  346,  347,  348,  499,  350,  351,  352,
   -1,  354,   -1,   -1,  273,   -1,  425,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  375,  408,  377,   -1,   -1,  296,  447,   -1,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  408,   -1,   -1,   -1,   -1,
   -1,   -1,  447,  482,  483,   -1,   -1,   -1,   -1,   -1,
   -1,  340,  425,   -1,   -1,   -1,   -1,  346,  347,  348,
  499,  350,  351,  352,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  447,   -1,   -1,  482,  483,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  375,   -1,  377,   -1,
   -1,   -1,   -1,   -1,  499,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  482,
  483,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  408,
   -1,   -1,   -1,   -1,   -1,   -1,  499,   -1,   -1,   -1,
   -1,   -1,  462,   -1,   -1,   -1,  425,   -1,   -1,   -1,
   -1,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,  485,  486,   -1,  447,  489,
  490,  491,  492,   -1,  494,   -1,   -1,  497,  498,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  482,  483,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,  485,  486,   -1,   -1,  489,
  490,  491,  492,  462,  494,   -1,   -1,  497,  498,   -1,
   -1,   -1,   -1,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,  485,  486,   -1,   -1,
  489,  490,  491,  492,   -1,  494,   -1,   -1,  497,  498,
};
#define YYFINAL 19
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 509
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
"LEQ","LEAVE","LEVEL","LIKE","LOG_BUF_SIZE","LSS","MANUAL","MAXIMUM",
"MAX_SEGMENT","MERGE","MESSAGE","MINIMUM","MODULE_NAME","NAMES","NATIONAL",
"NATURAL","NCHAR","NEQ","NO","NOT","NOT_GTR","NOT_LSS","NUM_LOG_BUFS","OF","ON",
"ONLY","OPTION","OR","ORDER","OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES",
"PAGE_SIZE","PARAMETER","PASSWORD","PLAN","POSITION","POST_EVENT","PRECISION",
"PRIMARY","PRIVILEGES","PROCEDURE","PROTECTED","RAW_PARTITIONS","READ","REAL",
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
"LOCK","SAVEPOINT","STATEMENT",
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
"proc_statement : POST_EVENT value ';'",
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
"select_view_expr : SELECT distinct_clause select_list from_view_clause where_clause group_clause having_clause plan_clause",
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
"order_item : column_name collate_clause order_direction nulls_placement",
"order_item : ordinal collate_clause order_direction nulls_placement",
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
"value : proc_internal_info",
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
"proc_internal_info : GDSCODE",
"proc_internal_info : SQLCODE",
"proc_internal_info : ROWS_AFFECTED",
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
"symbol_blob_subtype_name : SYMBOL",
"symbol_character_set_name : SYMBOL",
"symbol_collation_name : SYMBOL",
"symbol_column_name : SYMBOL",
"symbol_constraint_name : SYMBOL",
"symbol_cursor_name : SYMBOL",
"symbol_domain_name : SYMBOL",
"symbol_exception_name : SYMBOL",
"symbol_filter_name : SYMBOL",
"symbol_gdscode_name : SYMBOL",
"symbol_generator_name : SYMBOL",
"symbol_index_name : SYMBOL",
"symbol_item_alias_name : SYMBOL",
"symbol_label_name : SYMBOL",
"symbol_procedure_name : SYMBOL",
"symbol_role_name : SYMBOL",
"symbol_table_alias_name : SYMBOL",
"symbol_table_name : SYMBOL",
"symbol_trigger_name : SYMBOL",
"symbol_user_name : SYMBOL",
"symbol_variable_name : SYMBOL",
"symbol_view_name : SYMBOL",
"symbol_savepoint_name : SYMBOL",
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
#if defined(DEV_BUILD) && defined(WIN32) && defined(SUPERSERVER)
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
but here we should "unget" the first character if there're not 2 hyphens. */
if (first_time) {
	first_time = FALSE;
	if (swallow_single_line_comment() < 0)
		return -1;
}

for (;;)
    {
    if (ptr >= end)
	return -1;
    
    c = *ptr++;

    while (c == '\n') {
		lines++;
		line_start = ptr /* + 1*/; /* CVC: +1 left out. */
		/* CVC: Experiment to see if -- can be implemented as one-line comment. */
		if (swallow_single_line_comment() < 0)
			return -1;
		c = *ptr++;
	}


    if ((c == '/') && (*ptr == '*'))
	{
	ptr++;
	while (ptr < end)
	    {
	    if ((c = *ptr++) == '*')
		{
		if (*ptr == '/')
		    break;
		}
	    if (c == '\n')
		{
		lines++;
		line_start = ptr /* + 1*/; /* CVC: +1 left out. */

		}
	    }
	if (ptr >= end)
	    return -1;
	ptr++;
	continue;
	}

#if (! ( defined JPN_SJIS || defined JPN_EUC) )
    tok_class = classes [c];
#else
    c = c & 0xff;
    tok_class = (JPN1_CHAR(c) ? CHR_LETTER : classes[c]);
#endif /*JPN_SJIS || JPN_EUC */

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
#if (! ( defined JPN_SJIS || defined JPN_EUC) )
    p = string;
    CHECK_COPY_INCR(p, UPPER (c));
    for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
        {
	if (ptr >= end)
	    return -1;
	CHECK_COPY_INCR(p, UPPER (*ptr));
        }
#else
    for (; ptr <= end ; c = *ptr++ , c = c & 0xff)
        {
        if (KANJI1(c))
            {
	    CHECK_COPY_INCR(p, UPPER (c));
            if (ptr >= end) 
		break; 
            
	    c = *ptr++;
            c = c & 0xff;
            if (!KANJI2(c))
                {
                ptr--;
                break;
                }
            else
		CHECK_COPY_INCR(p,c);
            }
	else
            {
#ifdef JPN_SJIS
            if ((SJIS_SINGLE(c)) || (classes[c] & CHR_IDENT) )
#else
            if (classes[c] & CHR_IDENT)
#endif 
		{	
		CHECK_COPY_INCR(p, UPPER(c));
		if (ptr >= end)
	    	    break;
		}
            else
		{
		ptr--;
                break;
		}
            }
        }
#endif /* JPN_SJIS || JPN_EUC */

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
case 301:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-1]); }
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
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 319:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 320:
{ yyval = make_list (yyvsp[0]); }
break;
case 321:
{ yyval = make_list (yyvsp[-1]); }
break;
case 322:
{ yyval = NULL; }
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
case 328:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 329:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 330:
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 331:
{ yyval = make_node (nod_label, e_label_count, yyvsp[-1], NULL); }
break;
case 332:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 333:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 334:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 335:
{ yyval = NULL; }
break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 338:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 342:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 345:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 346:
{ yyval = make_list (yyvsp[0]); }
break;
case 347:
{ yyval = make_list (yyvsp[-1]); }
break;
case 348:
{ yyval = NULL; }
break;
case 352:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_replace_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_mod_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 360:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 362:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], NULL, NULL); }
break;
case 364:
{ yyval = make_list (yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 369:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 370:
{ yyval = yyvsp[-1]; }
break;
case 371:
{ beginning = lex_position(); }
break;
case 372:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
			       (lex_position() == end) ?
			       lex_position()-beginning : last_token-beginning);}
break;
case 373:
{ beginning = last_token; }
break;
case 374:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
					lex_position()-beginning); }
break;
case 375:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string (NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 376:
{ yyval = 0; }
break;
case 377:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 378:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 379:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 381:
{ yyval = NULL; }
break;
case 382:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 383:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 401:
{ yyval = NULL; }
break;
case 402:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 403:
{ yyval = yyvsp[0]; }
break;
case 404:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 405:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 406:
{ yyval = yyvsp[0]; }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 409:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 410:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string (NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 414:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 415:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 419:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 423:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 425:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{ yyval = yyvsp[0]; }
break;
case 428:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 429:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 430:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 431:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 456:
{ yyval = NULL; }
break;
case 457:
{ yyval = NULL; }
break;
case 458:
{ yyval = NULL; }
break;
case 459:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 460:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 461:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 462:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 463:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 464:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 465:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 466:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 468:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 469:
{ yyval = yyvsp[0]; }
break;
case 470:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 471:
{ yyval = yyvsp[0]; }
break;
case 472:
{ yyval = yyvsp[0]; }
break;
case 474:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 477:
{ yyval = NULL; }
break;
case 479:
{ yyval = NULL; }
break;
case 480:
{ yyval = yyvsp[0]; }
break;
case 481:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 497:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 498:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 500:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 501:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 502:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 508:
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
case 509:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 510:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 511:
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
case 512:
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
case 513:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 516:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 517:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 518:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 519:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 520:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 521:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 522:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 523:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 524:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 525:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 527:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 528:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 529:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 530:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 531:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 532:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 541:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 542:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 543:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 544:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
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
case 549:
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
case 550:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 551:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 552:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
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
case 561:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 562:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 565:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 566:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 569:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 570:
{ yyval = (DSQL_NOD) NULL; }
break;
case 572:
{ yyval = (DSQL_NOD) NULL; }
break;
case 573:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 575:
{ yyval = (DSQL_NOD) NULL; }
break;
case 577:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 582:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 583:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 584:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 585:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 586:
{ yyval = yyvsp[0];}
break;
case 588:
{ yyval = yyvsp[0];}
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 590:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 591:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 594:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 595:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 596:
{ yyval = 0; }
break;
case 597:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 598:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 599:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 600:
{ yyval = (DSQL_NOD) 0; }
break;
case 601:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 602:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 604:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 605:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 606:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 607:
{ yyval = 0; }
break;
case 609:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 610:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 612:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 613:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 614:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 615:
{ yyval = make_list (yyvsp[0]); }
break;
case 616:
{ yyval = 0; }
break;
case 618:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
break;
case 620:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 625:
{ yyval = 0; }
break;
case 626:
{ yyval = 0; }
break;
case 627:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 628:
{ yyval = 0; }
break;
case 629:
{ yyval = yyvsp[0]; }
break;
case 630:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 631:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 632:
{ yyval = 0; }
break;
case 633:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 634:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 635:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 636:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 638:
{ yyval = 0; }
break;
case 639:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 640:
{ yyval = yyvsp[-1]; }
break;
case 641:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_top, 1, yyvsp[-2]);}
break;
case 697:
{ yyval = make_node (nod_top, 2, yyvsp[-4], yyvsp[-3]);}
break;
case 698:
{ yyval = 0;}
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
case 778:
{ yyval = yyvsp[-1]; }
break;
case 779:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 788:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 789:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 790:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 791:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 792:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 807:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 808:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 809:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 811:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 813:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 815:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 817:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 819:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 824:
{ yyval = make_list (yyvsp[-1]); }
break;
case 825:
{ yyval = yyvsp[-1]; }
break;
case 826:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 827:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 837:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 838:
{ yyval = yyvsp[0]; }
break;
case 839:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 840:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 841:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 842:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 843:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 844:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 845:
{ yyval = yyvsp[-1]; }
break;
case 846:
{ yyval = yyvsp[-1]; }
break;
case 849:
{ yyval = yyvsp[0]; }
break;
case 850:
{ yyval = yyvsp[0]; }
break;
case 851:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 852:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 853:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 854:
{ yyval = yyvsp[0]; }
break;
case 855:
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
case 856:
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
case 857:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 858:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 860:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 862:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 863:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 864:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 865:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 866:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 867:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 869:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
break;
case 870:
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
case 871:
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
case 872:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 873:
{ yyval = make_parameter (); }
break;
case 874:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 875:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 876:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 877:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 878:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 879:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 880:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 881:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 882:
{ yyval = yyvsp[0]; }
break;
case 883:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 885:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 886:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 887:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 888:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 889:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 891:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 892:
{ yyval = yyvsp[0];}
break;
case 897:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 898:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 899:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 900:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 901:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 902:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 903:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 904:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 905:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 906:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 907:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 908:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 909:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 910:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 911:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 912:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 913:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 914:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 915:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 918:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 919:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 922:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 923:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 925:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 926:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 927:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 928:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 929:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 933:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 934:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 935:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 936:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 937:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 938:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 939:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 940:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 943:
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
