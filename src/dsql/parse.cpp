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
   18,  312,  312,  312,  313,  313,  316,  316,  317,   66,
   66,   66,  318,  318,  318,  314,  314,  319,  319,  320,
  320,  315,  323,  321,  321,  321,  321,  324,  324,  324,
  325,  325,  325,  225,  225,  226,  226,  327,  327,  328,
  328,  328,  322,  331,  331,  332,  332,  333,  333,  334,
  334,  335,  335,  206,  206,  337,  337,  235,  235,   32,
  236,  236,  236,  236,  236,  236,  236,  236,  229,  229,
  338,  338,  339,  339,  339,  339,  339,  341,  341,  341,
  230,  230,  228,  228,  345,  345,  345,  231,  231,  346,
  347,  347,  347,  347,  347,  348,  348,  349,  349,  350,
  350,  351,  351,  351,  352,  352,   10,   10,  354,  354,
    7,    7,  355,  356,  357,   20,   20,  358,  359,  360,
  360,  189,  329,  329,    3,    3,  362,  362,  362,  364,
  364,   62,  363,  363,  365,  365,   35,   35,   86,   85,
   85,  353,  353,  366,  367,  367,  207,  207,  207,  147,
  361,  361,  112,  112,  112,  112,  369,  369,  369,  369,
  369,  369,  369,  369,  369,  369,  369,  370,  370,  370,
  370,  370,  370,  370,  370,  375,  375,  375,  375,  375,
  375,  375,  375,  375,  375,  375,  375,  375,  375,  375,
  375,  381,  381,  371,  371,  372,  372,  372,  372,  373,
  373,  377,  377,  378,  378,  378,  378,  376,  379,  374,
  374,  382,  382,  383,  380,  385,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,  157,  157,  157,  386,  384,  384,  152,
  152,  391,  391,  391,  391,  391,  388,  388,  388,  388,
  388,  217,  153,  153,  154,  155,  155,  390,  390,  390,
   52,   52,   21,   21,  243,  392,   55,  124,  274,  274,
   96,  387,  387,  387,  387,  393,  393,  393,  393,  393,
  393,  393,  393,  393,  393,  393,  394,  342,  343,  343,
  343,  340,  340,  389,  344,  344,  396,  396,  397,  397,
  398,  398,  401,  401,  399,  399,  404,  404,  403,  400,
  402,  395,  395,  395,  395,  395,  395,  395,  395,  326,
  326,  156,   49,  280,  123,  149,  368,  166,  210,  267,
   64,   61,  214,  113,   67,  330,  209,   36,   42,  336,
  266,   40,   43,  205,   41,  291,  405,
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
    3,    1,    3,    4,    3,    0,    1,    3,    4,    1,
    1,    0,    2,    2,    0,    4,    0,    2,    0,    2,
    0,    9,   10,    2,    1,    1,    0,    2,    4,    2,
    2,    4,    2,    1,    1,    1,    1,    1,    3,    1,
    2,    3,    2,    1,    3,    1,    1,    6,    3,    3,
    2,    3,    0,    1,    3,    1,    1,    1,    2,    1,
    1,    1,    2,    1,    2,    1,    2,    0,    3,    0,
    1,    3,    1,    1,    1,    1,    3,    1,    1,    1,
    2,    0,    2,    0,    3,    5,    0,    2,    0,    4,
    1,    2,    1,    1,    0,    1,    3,    2,    1,    1,
    2,    1,    4,    2,    1,    3,    8,    5,    1,    3,
    1,    1,    4,    4,    4,    1,    1,    5,    5,    1,
    3,    3,    1,    1,    7,    7,    5,    3,    0,    1,
    1,    1,    2,    0,    1,    1,    1,    0,    3,    1,
    3,    1,    0,    3,    1,    3,    1,    3,    3,    1,
    1,    3,    1,    3,    3,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    1,    1,    5,    6,    3,    4,    5,    6,    3,
    4,    3,    4,    3,    4,    4,    5,    4,    4,    3,
    4,    1,    3,    3,   10,   10,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    2,    2,    3,    3,    3,
    3,    3,    3,    3,    3,    1,    1,    1,    1,    1,
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
   13,   14,   15,   16,   17,   18,   19,   20,   21,  555,
  556,  557,    0,  612,  721,  722,  726,  727,  466,    0,
    0,    0,    0,    0,    0,  403,  567,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  885,    0,   22,  883,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  480,    0,    0,   38,   35,   37,    0,   36,
    0,  967,    0,    0,    0,    0,   33,   69,   67,  959,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  636,    0,    0,    0,  668,    0,    0,
  961,    0,    0,    0,  108,  561,  966,    2,    0,    0,
    0,    0,  450,  432,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  451,  452,  453,  454,  455,    0,  431,    0,  951,
    0,  410,  955,    0,  407,  958,    0,  670,    0,  406,
  962,    0,  565,  881,    0,  151,  104,    0,  105,    0,
  218,  760,  947,    0,  103,  146,  954,    0,    0,   99,
    0,  106,  887,    0,  100,    0,  101,    0,  102,  965,
  147,  107,  620,  621,    0,  886,  884,    0,   75,    0,
  952,    0,  488,  950,  481,    0,  487,  492,  482,  483,
  490,  484,  485,  486,  491,    0,   31,    0,   40,  747,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  891,    0,  872,  638,
  640,    0,  641,  643,  940,  644,    0,  645,  634,    0,
    0,    0,    0,    0,    0,    0,  584,  573,    0,  576,
  578,  579,  580,  581,  587,  588,    0,  669,  960,    0,
  109,    0,  110,    0,  111,    0,    0,  613,    0,  611,
    0,    0,    0,    0,  467,    0,    0,  373,    0,  221,
    0,  413,  404,  464,  465,    0,    0,    0,    0,    0,
    0,  422,    0,  379,  380,    0,  571,  569,  882,    0,
    0,    0,    0,    0,    0,    0,  154,  133,    0,   97,
    0,    0,  112,    0,  122,  121,    0,    0,    0,    0,
    0,    0,   76,    0,  943,    0,    0,  723,  724,  489,
    0,  942,  864,  863,  862,    0,    0,  865,  866,    0,
    0,  868,  350,  351,  345,    0,  349,  860,  867,    0,
  750,    0,    0,   28,    0,    0,    0,   72,   70,  963,
   68,   34,    0,    0,    0,  752,    0,   53,    0,    0,
    0,    0,    0,  563,    0,    0,    0,    0,  850,  878,
    0,    0,  852,    0,    0,  879,    0,  873,    0,  854,
  855,  856,  874,  875,    0,  876,  877,    0,    0,    0,
  880,    0,    0,    0,    0,    0,    0,  757,  846,  847,
  848,  853,  832,    0,  831,    0,  835,  894,  895,  834,
  828,  829,  830,  833,  849,  892,  893,  915,  916,  919,
  920,    0,    0,  647,    0,  734,    0,    0,  648,    0,
    0,  610,    0,  585,    0,  582,    0,  583,  608,    0,
  603,    0,    0,  577,  761,  730,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  617,  614,    0,  186,    0,
    0,  130,  472,  173,  174,    0,  470,    0,    0,    0,
    0,  473,    0,  468,    0,    0,  417,  419,  418,    0,
  420,    0,  414,  219,    0,    0,  272,    0,    0,    0,
    0,  426,  427,    0,  457,    0,    0,    0,    0,    0,
    0,    0,  384,  383,  476,    0,    0,  150,    0,    0,
  159,    0,  158,    0,  165,  148,    0,  163,  166,  167,
  155,    0,    0,  511,  547,  514,    0,  548,    0,    0,
  515,    0,    0,    0,  538,  551,  510,  533,  512,  513,
  508,  495,  373,  493,  494,  496,    0,    0,  505,  506,
  507,  509,    0,    0,    0,    0,    0,  113,    0,  114,
    0,  125,    0,    0,  202,    0,    0,  371,    0,  221,
    0,    0,   84,    0,    0,  742,    0,    0,    0,    0,
    0,    0,    0,    0,  763,  767,  768,  769,  770,  771,
  772,  773,  774,  775,  776,  869,  870,  871,  861,    0,
    0,    0,  749,   29,    0,    0,   45,   73,    0,    0,
   27,    0,  755,    0,    0,    0,  718,    0,    0,    0,
    0,    0,  562,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  319,  964,    0,    0,    0,    0,
    0,    0,    0,  639,    0,    0,  642,    0,    0,    0,
    0,  651,  956,  559,    0,  889,  558,    0,  586,    0,
  594,  590,  589,    0,    0,    0,  605,  593,    0,    0,
  728,  729,    0,    0,    0,  371,    0,    0,    0,    0,
    0,    0,    0,  131,    0,    0,    0,    0,    0,    0,
  373,  416,    0,  373,  374,  421,  503,    0,  274,    0,
  268,  270,  371,  241,  948,    0,    0,  373,    0,    0,
  210,  371,    0,    0,    0,    0,  246,  240,  243,  244,
  245,  425,  462,  461,  424,  423,    0,    0,    0,    0,
    0,    0,  382,    0,  373,    0,  156,  160,    0,  164,
    0,    0,  534,  535,  552,    0,  549,    0,    0,  541,
  539,  540,    0,    0,    0,    0,  504,    0,    0,    0,
    0,  542,    0,    0,  371,    0,  201,    0,  204,  206,
  207,    0,    0,    0,    0,    0,  221,   79,    0,    0,
   78,  522,  523,  944,    0,    0,    0,  766,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  347,  353,
  354,  352,  751,    0,    0,    0,    0,    0,    0,    0,
    0,   54,   62,   64,    0,    0,   71,    0,    0,    0,
  754,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  932,  933,  934,  935,  936,  937,  938,
  939,    0,    0,    0,    0,    0,    0,  925,    0,    0,
    0,  666,    0,    0,  844,  845,  913,    0,    0,  840,
  946,    0,    0,    0,    0,    0,    0,  851,  759,  758,
    0,    0,    0,    0,  656,  657,    0,    0,  649,  652,
  560,  890,  595,  604,  599,  598,    0,  609,  762,  731,
  732,  371,    0,    0,    0,    0,  618,    0,    0,  626,
  182,  176,  183,    0,  179,    0,    0,  178,    0,  189,
  170,  169,  888,  172,  171,  474,    0,    0,    0,  415,
  273,    0,  212,  373,  214,    0,  374,  209,  215,    0,
    0,    0,    0,  428,  429,  373,  371,  374,    0,    0,
    0,  400,    0,  197,  198,  157,    0,    0,    0,    0,
    0,    0,  550,    0,    0,  373,  374,    0,  499,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  199,    0,    0,    0,    0,  360,    0,   83,
    0,    0,    0,    0,   88,   85,    0,    0,    0,  725,
  949,    0,    0,  777,    0,    0,    0,  803,  802,    0,
    0,    0,  810,  822,  820,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  765,    0,   66,   58,   59,
   65,   60,   61,    0,    0,   23,    0,    0,    0,   43,
    0,    0,    0,  736,  756,    0,  719,    0,  735,    0,
    0,    0,    0,    0,    0,  221,    0,  896,    0,    0,
  911,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  921,    0,  931,    0,    0,    0,    0,
    0,    0,  912,  857,    0,    0,    0,    0,    0,    0,
  671,    0,    0,    0,    0,    0,  601,  602,  606,    0,
  200,    0,    0,  619,  630,    0,    0,    0,    0,    0,
    0,  190,    0,    0,  411,  224,  225,  226,  227,  228,
  229,    0,    0,    0,  277,    0,  222,    0,    0,    0,
    0,  247,    0,  458,  459,  430,    0,    0,    0,    0,
    0,    0,  373,  478,  374,  168,  945,    0,    0,  517,
    0,  516,  553,    0,  545,    0,    0,    0,  497,    0,
  525,    0,    0,  527,  530,  532,  371,    0,    0,    0,
  120,    0,  205,  373,    0,    0,  372,    0,   98,    0,
  118,    0,   94,    0,    0,    0,    0,   87,    0,  818,
  819,    0,    0,    0,    0,    0,    0,  821,    0,    0,
    0,    0,    0,    0,    0,  811,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   57,   63,   55,   56,   24,  740,  741,    0,  738,
  745,  746,  743,    0,    0,  717,    0,    0,  902,  901,
    0,    0,  898,  897,    0,  906,  905,  904,  903,  900,
  899,    0,    0,  927,    0,    0,    0,  926,    0,    0,
    0,  664,    0,    0,  659,    0,  660,    0,  677,  673,
  675,    0,    0,    0,    0,    0,  372,  623,  624,    0,
  184,  180,    0,    0,  195,  191,    0,  374,  280,    0,
    0,  374,  278,    0,    0,    0,  232,    0,    0,  248,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  475,
  519,    0,  520,    0,  135,    0,    0,    0,  139,  141,
  142,  143,  500,  502,  498,  529,    0,    0,  374,    0,
    0,    0,    0,  355,    0,  361,  373,   89,    0,    0,
   91,    0,    0,    0,    0,    0,    0,  824,  823,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   41,    0,    0,
  720,  220,  914,  907,  908,    0,  909,  923,  922,    0,
  928,  917,    0,  918,    0,  662,    0,  543,    0,  684,
    0,    0,  681,  685,  686,  688,  689,  690,    0,    0,
  632,  374,  356,    0,  175,  192,  374,  145,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  285,  290,  284,    0,    0,    0,
  294,  295,  296,  297,  298,  299,  302,  304,  307,  267,
    0,    0,  208,  233,    0,    0,    0,    0,  234,  237,
    0,    0,  374,  398,  399,  395,  394,  397,  396,    0,
  518,  546,  144,  132,  140,    0,  373,  264,  127,  374,
    0,    0,  375,  362,    0,    0,   92,   93,   80,    0,
  789,  797,    0,  790,  798,  793,  801,    0,    0,  791,
  799,  792,  800,  786,  794,  787,  795,  788,  796,  737,
    0,    0,  924,  665,    0,    0,    0,    0,    0,  701,
  703,    0,  698,    0,  265,  185,  412,    0,  310,    0,
    0,  313,  314,    0,  306,    0,    0,    0,  305,    0,
  332,  333,  293,  300,    0,  303,  286,    0,  291,    0,
  336,  292,  211,  216,  235,    0,    0,  238,    0,  250,
  252,  402,  374,    0,  377,    0,    0,    0,  367,  368,
    0,    0,   77,   95,    0,    0,  910,    0,    0,  687,
  682,  702,    0,    0,  283,    0,  308,    0,    0,    0,
    0,    0,  301,    0,    0,  326,  327,  344,    0,    0,
    0,    0,  339,  337,  239,    0,    0,    0,    0,    0,
  266,  374,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  709,    0,    0,    0,  282,  279,  309,    0,
    0,    0,  312,    0,    0,    0,  318,    0,  343,  342,
  953,  341,    0,    0,    0,    0,    0,  249,    0,  256,
    0,  255,  378,  370,    0,    0,    0,  119,    0,    0,
    0,  711,  700,    0,    0,  712,    0,  708,    0,    0,
    0,    0,    0,    0,    0,    0,  329,  328,  338,  340,
  236,    0,    0,  260,  259,  258,    0,  363,    0,  633,
  826,  707,    0,  714,  321,    0,    0,  311,    0,    0,
    0,  330,  263,  261,  262,    0,    0,    0,    0,    0,
  334,  315,    0,    0,    0,    0,  713,  324,  316,  825,
  716,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1501,   27,   28, 1502,
   30,   31,   32,   33,   34,   35,   36, 1503,   38, 1504,
  616,  103,  383,  870, 1116,  104,  646,  871,  105,  387,
  651,  128,  106,  107,  239,  942,  118,  872,  873,  179,
  208,  108,  874,  109,  389,   79,  219,  353,  436,  611,
 1053,  372,  830,  582, 1468,  807,  612,  613,  614, 1055,
  220, 1307,   71,  169,   72,  215, 1778, 1259,  200,  205,
  207,  209,  195,  187,  189,  202,  212,  135,  291,  293,
  295,  343,  598,  600,  380,  240,  757,  516,  623,  990,
  347,  603, 1041, 1251,  549,  706, 1016,  501,  502,  531,
  339,  583, 1027, 1397,  728, 1195, 1398, 1399, 1400, 1401,
 1402,  624,  196,  188,  335,  556,  336,  337,  557,  558,
  559,  560, 1226,  984,  504,  505,  974,  972,  506,  978,
  973, 1186, 1361,  979,  980, 1191,  349,  818,  819,  820,
  821,  759,  997, 1375,  584,  761,  438,  823,  930,  525,
 1322, 1196,  439,  440,  441,  922,  442, 1376, 1377,  534,
 1529, 1678, 1212,  767,  768,  754,  769,  770,  771, 1679,
 1680, 1755,  317,  529, 1203, 1505,  175,  526,  527, 1204,
 1205, 1370, 1656, 1506, 1507, 1508, 1509, 1620, 1510, 1511,
 1512, 1513, 1514, 1515, 1516, 1517, 1518, 1519, 1701, 1742,
 1604, 1665, 1744,  443,  685, 1341,  444,    0,    0, 1060,
 1621, 1672, 1673, 1710,  375,  376,  445, 1046, 1257, 1414,
    0,    0, 1047, 1048,  267,  467, 1552,  358, 1176, 1355,
 1481, 1637, 1638, 1639, 1640, 1174,  326,  545,  785, 1224,
  546,  783,  203,   56,  321,  180,  142,  304,  167,  311,
  172,  745,  742,  312,  322,  775,  323,  542, 1008,  168,
  305,  513,  547, 1225,   93,  178,  223,  585,  586,  747,
 1028,  748, 1029, 1030,  589,  590,  591,  592, 1021,  833,
  593,  594,  595,  800,  596, 1470,  797,   40,   41,   42,
  136,   58,  405,  183,  328,  278,  279,  280,  281,  282,
  283,  284,  285,  286,  712,  480,  957, 1179,  481,  482,
  717,   43,  141,  300,   44,  495,  496, 1184,  731,  970,
  122,  699,  657,  123,  124,  268,  468,  469,  470,  702,
  943,  944,  945,  946, 1168,  446, 1156, 1472, 1473,  447,
 1475,  448,  449,  450,    0, 1693, 1594, 1694, 1695, 1696,
 1738, 1779,  395, 1128,   45,   46,  359,   47,   48,  488,
  489,  879, 1124, 1309, 1313,  396,  654,  192,  625,  626,
  627,  628,  629,  630,  631,  632,  633,  634,  635, 1276,
 1071, 1073, 1074,  929,  684,  451,  452,  453,  454,  455,
  379,  217,  456,  457,  912,  458,  459,  460,  461,  676,
  916, 1157, 1152,  677,  462,
};
static short yysindex[] = {                                   4081,
 1417,   27, 3584,  142, 1377,  115, 3739,   94, 3293,   46,
  478, 3236,   27, 1219,  596,  313, 1420,  313,    0,  750,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  538,    0,    0,    0,    0,    0,    0, 5435,
  313,  313,  313,  313,  313,    0,    0,  496, 1280,  313,
  313,  313,  764,  313,  632,  313,  313,    0,  313,  313,
    0, 1317,    0,  650,    0,    0,  822,  313,    0,  313,
  313,  313,  883,  313,  313,  313,  313,  632,  313,  313,
  313,  313,    0,  313,  798,    0,    0,    0, 1171,    0,
 1171,    0,  891,  972,  269, 1335,    0,    0,    0,    0,
  313,  313,  313, 1011, 1027, 1111,  137, 1450, 1103,  188,
  224,  959, 1130,    0,  313, 1245, 1233,    0, 1181,  313,
    0,  313,  313,  313,    0,    0,    0,    0, 1366,   26,
 1328,   37,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  966,    0, 1280,    0,
  757,    0,    0, 1611,    0,    0, 1307,    0,  779,    0,
    0, 1244,    0,    0, 1209,    0,    0, 1574,    0, 1414,
    0,    0,    0, 1280,    0,    0,    0, -102, 1611,    0,
   56,    0,    0, 1385,    0, 1388,    0, 1171,    0,    0,
    0,    0,    0,    0, 1382,    0,    0,  313,    0, 1396,
    0, 1332,    0,    0,    0,  313,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1826,    0,  313,    0,    0,
    0,  -99, 1384,  736,  313, 1076, 1458, 1767, 1498, 1506,
  -99, 1445,  736, 1475, 1490, 1374,    0, 7710,    0,    0,
    0, 7710,    0,    0,    0,    0, 5809,    0,    0, 1453,
  313, 1532, 1466,  898,  313, 1461,    0,    0, 1233,    0,
    0,    0,    0,    0,    0,    0,  313,    0,    0, 1611,
    0, 1385,    0, 1171,    0, 7710, 1548,    0, 1482,    0,
  -29, 1572, 1114,   37,    0, 1655,  706,    0,  313,    0,
  966,    0,    0,    0,    0,  313, 1538,  -51, 1508,  -43,
 1939,    0, 5641,    0,    0,  627,    0,    0,    0, 1931,
 1931, 1280, 1626, 1280, 1633, 1574,    0,    0, 8757,    0,
  313,  313,    0, 1538,    0,    0, 1714,  445, 1981,  313,
 1749,  313,    0, 1987,    0,  198, 3650,    0,    0,    0,
 1570,    0,    0,    0,    0, 1576, 1582,    0,    0, 1650,
 1684,    0,    0,    0,    0, 2023,    0,    0,    0, 1624,
    0,  313, 1631,    0,  313,  313,  121,    0,    0,    0,
    0,    0,  313,  313,  -90,    0,  313,    0, 1748,  313,
 2032,  -99, 1678,    0,  313, 2048, 2057, 2061,    0,    0,
 2062, 2066,    0, 2079, 2089,    0, 2090,    0, 2100,    0,
    0,    0,    0,    0, 2105,    0,    0, 6363, 2109, 2120,
    0, 7710, 7710, 6591,  313, 2121, 1761,    0,    0,    0,
    0,    0,    0, 1991,    0, 2078,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1895,    0, 2214,    0, 1841, 2119,    0,  -60,
   85,    0, 1256,    0,  434,    0,  434,    0,    0, 2123,
    0,  183, 1735,    0,    0,    0, 2134,  108, 2108,    0,
 1538, 2143, 1920, 2840, 2144,    0,    0, 1804,    0,  235,
 1845,    0,    0,    0,    0, 1280,    0, 1931, 1931, 1931,
 1931,    0, 2147,    0, 1917, 1918,    0,    0,    0, 1896,
    0, 8757,    0,    0, 8757, 1721,    0, 1611, 1935,  313,
 4138,    0,    0,    3,    0,  313,  523, 1307,    0, 1800,
 1765, 1733,    0,    0,    0,  594, 1807,    0, 1747,  632,
    0, 1280,    0, 1932,    0,    0, 1633,    0,    0,    0,
    0,  216, 1760,    0,    0,    0, 1770,    0, 1813, 2175,
    0, 1866, 2179,  503,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2130,  270,    0,    0,
    0,    0,  180, 2182, 2187, 2179, 1611,    0, 1909,    0,
 1965,    0, 1280, 1280,    0,  -51,  779,    0, 1846,    0,
 1825, 2197,    0, 4887,  161,    0, 1852, 1869, 2208, 4290,
 2223, 3735, 4475,   22,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1842,
 1684,  313,    0,    0, 1434, 1806,    0,    0, 1803,  736,
    0, 1952,    0, 1860, 1219, 2237,    0, 1952, 1434, 1957,
 1958,  313,    0, 1028, 6677,  768,  313, 7710, 1169, 1291,
 1309, 3054, 7710, 4290, 2214, 1829,    7, 7710, 6677,  928,
  928, 1219, 1934, 2241,    0,    0, 6856,  313, 7710, 7710,
 7710, 7710, 7710,    0, 7710,  146,    0,  135, 1834, 6677,
  313,    0,    0,    0,  975,    0,    0,  483,    0, 1843,
    0,    0,    0,  313,   50,  313,    0,    0,  313,  313,
    0,    0, 6677, 2026,  -51,    0,  313, 1317, 7710,  313,
 1839, 1280, 1280,    0,  507, 1747, 1747, 1836,  632, 1114,
    0,    0, 2253,    0,    0,    0,    0, 2019,    0,  313,
    0,    0,    0,    0,    0, 2024, 2259,    0, 2001, 2021,
    0,    0, 1959, 1970, 1171, 2031,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  632,  313, 7645, 1923,
 1929, 1930,    0,  632,    0, 1581,    0,    0, 1899,    0,
  179, 1904,    0,    0,    0,  632,    0, 2175,  204,    0,
    0,    0, 2033,  204, 1905,  204,    0, 2295,  632,  632,
  632,    0, 1538,  313,    0, 1980,    0, 1946,    0,    0,
    0,  627, 1921,  313, 1973,  134,    0,    0, 2298, 2067,
    0,    0,    0,    0,  198,  313, 1924,    0, 1924, 4821,
  431, 7710, 7710, 4470, 2304,   15, 4552, 7710, 4787, 1263,
 5022, 5104, 6920, 5257, 5339, 5574, 4290, 4290,    0,    0,
    0,    0,    0,  313,  313,  313,  313,  313,  313,  190,
 2306,    0,    0,    0, 1434, 1962,    0,  -39, 1983,  313,
    0,  959, 6677, 1983, 2313, 2306, 1434,  495, 2044, 7710,
 7710, 2102, 7710, 2326, 7710, 2324, 2131, 7710, 7710, 7710,
 7710, 7710, 7710,    0,    0,    0,    0,    0,    0,    0,
    0, 2049, 1556,  667, 7710,  583, 6677,    0, 4290, 2042,
 2214,    0, 2325,  959,    0,    0,    0, 2214, 1985,    0,
    0, 2093,  928,  928,   73,   73, 1019,    0,    0,    0,
  135, 2339, 2350, 1703,    0,    0, 4290, 2055,    0,    0,
    0,    0,    0,    0,    0,    0,  378,    0,    0,    0,
    0,    0, 1990, 1921,    0, 1893,    0, 2353, 1894,    0,
    0,    0,    0, 2004,    0, 1931, 2138,    0,  507,    0,
    0,    0,    0,    0,    0,    0, 2369, 4290, 3470,    0,
    0, 2110,    0,    0,    0, 3470,    0,    0,    0, 1171,
 1171, 1910, 2373,    0,    0,    0,    0,    0,  530,  509,
  882,    0, 2150,    0,    0,    0,  313,  142, 2005, 1993,
 2019, 2377,    0, 1747, 2034,    0,    0, 1056,    0, 2361,
  313, 1267,  632, 2380, 2382, 2383, 2162,  779, 2110, 1931,
 1845,  -51,    0, 1807,  959, 1972, 1988,    0,  117,    0,
  632, 2040, 2128, 4887,    0,    0,  632, 1944, 2129,    0,
    0, 2401, 2402,    0, 2333, 2214, 2405,    0,    0, 2214,
 2406, 7146,    0,    0,    0, 2101, 2413, 2214, 2418, 1643,
 2420, 2214, 2421, 7710, 7710, 2304, 7710, 7210, 2422, 2214,
 2427, 2429, 2214, 2430, 7710, 2214, 2431, 2214, 2435, 2437,
 2214, 2438, 2440, 2214, 2441,    0, 2221,    0,    0,    0,
    0,    0,    0, 2158, 1434,    0, 1434,  190, 2306,    0,
  358,  358,  255,    0,    0, 5809,    0, 2054,    0, 2313,
 2306, 2411,  495, 2193, 2284,    0, 2340,    0, 2360, 7710,
    0, 2567, 2629, 2765, 2927, 2939, 3018, 7710,  632, 6677,
 2214, 2053, 6677,    0, 7710,    0, 2184,  864, 7710, 6677,
 7710, 7710,    0,    0, 1703, 2461, 6677,  313,  135, 2114,
    0, 2115, 2116, 2163, 2232, 2176,    0,    0,    0, 2110,
    0, 1972,  589,    0,    0, 2087, 1280, 2124, 1747, 2126,
 1747,    0, 4290,  718,    0,    0,    0,    0,    0,    0,
    0, 2069, 2248, 2110,    0, 7710,    0, 2233, 2122, 1910,
 1317,    0, 4290,    0,    0,    0, 2140, 2142, 2151, 2155,
 2156, 2168,    0,    0,    0,    0,    0, 2490,  142,    0,
 1836,    0,    0,  204,    0, 3470, 2170,  204,    0,  204,
    0, 2019, 2493,    0,    0,    0,    0,  627, 2248, 1747,
    0, 1845,    0,    0, 5809, 2282,    0,   31,    0, 2353,
    0, 2521,    0, 2525, 1280,  -34, 2526,    0, 1280,    0,
    0, 7710, 2154, 2154, 1219, 2528, 2070,    0, 2154, 2154,
 7710, 2154, 2154, 2481, 2214,    0, 1779, 7710, 2214, 2154,
 2154, 2154, 2154, 2214, 2154, 2154, 2154, 2154, 2154, 2154,
 2188,    0,    0,    0,    0,    0,    0,    0, 2136,    0,
    0,    0,    0, 1841, 6677,    0,  495, 2411,    0,    0,
 8757, 2534,    0,    0, 3070,    0,    0,    0,    0,    0,
    0, 3195,  333,    0, 6677, 2267, 2141,    0, 6677, 3340,
 2084,    0, 1612, 2214,    0, 2097,    0, 1703,    0,    0,
    0,  135, 2732, 4290, 2186, 2248,    0,    0,    0, 1931,
    0,    0, 1280, 1581,    0,    0,  748,    0,    0,  313,
 6324,    0,    0, 2214, 2308, 2233,    0,  912,  313,    0,
 2247,  751, 2149, 2254, 2159, 2292, 2264, 2302, 2110,    0,
    0, 2545,    0, 2564,    0, 2262, 2308, 2170,    0,    0,
    0,    0,    0,    0,    0,    0, 2110, 1807,    0, 1581,
 2150, 2285, 2230,    0, 1921,    0,    0,    0, 2235,  419,
    0, 2019, 2249, 2214, 2582, 2583,  959,    0,    0, 2584,
 2587, 2214, 2590, 2591, 7710, 7710, 2214, 2592, 2594, 2595,
 2597, 2601, 2602, 2604, 2607, 2609, 2614,    0,  358, 1834,
    0,    0,    0,    0,    0,  632,    0,    0,    0, 6677,
    0,    0, 6677,    0, 1834,    0, 1083,    0, 2379,    0,
 2611, 2618,    0,    0,    0,    0,    0,    0,   22,  524,
    0,    0,    0, 1747,    0,    0,    0,    0, 8757,  163,
  695, 2605, 1548, 2623, 2328, 7710, 2608, 2626, 2620, 2621,
 2624, 2625, 2332, 2627,    0,    0,    0, 2376, 5709, 2628,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2641,  313,    0,    0, 2335, 2345,  313, 1910,    0,    0,
 1171,  313,    0,    0,    0,    0,    0,    0,    0, 2248,
    0,    0,    0,    0,    0, 2248,    0,    0,    0,    0,
  165, 1834,    0,    0, 7710, 1280,    0,    0,    0, 1280,
    0,    0, 7710,    0,    0,    0,    0, 2214, 2214,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2055, 2650,    0,    0, 2055, 4290,  313,  131, 2732,    0,
    0, 2327,    0, 2659,    0,    0,    0,  193,    0, 7390,
  313,    0,    0, 7710,    0, 2363, 4290, 2454,    0, 4290,
    0,    0,    0,    0,  195,    0,    0,   62,    0, 2255,
    0,    0,    0,    0,    0, 1910, 1171,    0, 2322,    0,
    0,    0,    0, 2150,    0,  165, 2668, 1703,    0,    0,
 2055, 2214,    0,    0, 1612, 2176,    0, 2176,   22,    0,
    0,    0, 1279, 3470,    0, 2654,    0, 2503, 7475, 2818,
  195,  878,    0,  902, 1281,    0,    0,    0,  313,  313,
  142,  186,    0,    0,    0, 2322,  520, 1910, 2331, 2336,
    0,    0, 1703, 2676,  165, 2378, 2176, 2683, 1834, 2186,
 2186,  313,    0, 2685, 2684, 1370,    0,    0,    0, 5893,
 2316, 2688,    0,  136, 2293, 2439,    0,  195,    0,    0,
    0,    0, 6324,   62, 1910, 1045, 1045,    0, 2443,    0,
 2299,    0,    0,    0, 1703,  165, 2186,    0, 2055, 2359,
 2359,    0,    0, 1279, 2708,    0,  313,    0, 1934, 2112,
  160, 2692, 2463, 2457, 6324, 6324,    0,    0,    0,    0,
    0, 2301,  616,    0,    0,    0, 1605,    0, 2176,    0,
    0,    0,  313,    0,    0,  195, 2715,    0,  313, 6324,
 2455,    0,    0,    0,    0, 4290, 2186, 2727, 2731, 2113,
    0,    0, 6324,   22, 2359,  313,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  280, 1110,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  132, 3970,    0,    0,    0,    0,    0, 2774,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  996,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1078,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2442,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2391,    0,    0,    0,  123,    0,
  123,    0,    0,    0,    0, 2394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1166,    0,
    0, 6046, 4205,    0,    0,    0, 1176,    0,    0, 1680,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  441,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   -7,    0,    0,    0,    0,  861,    0,
    0, 1341,    0,    0,    0,    0,    0, 1292,    0, 8847,
    0,    0,    0,    0,    0,    0,    0,    0,   -7,    0,
  587,    0,    0, 2741,    0,    0,    0, 2518,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1345,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1426,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1071,    0,    0,
    0,    0,    0,    0,    0, 2323,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  489,    0,    0, 1431,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   -7,
    0, 2741,    0, 2518,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1440,    0, 2507,    0,    0,    0,    0,
 1463,    0,    0,    0,    0,    0, 2524, 1038, 4644,    0,
 1505,    0,    0,    0,    0,  439,    0,    0,    0, 2334,
 2334,    0,    0,    0, 1525, 2127,    0,    0,    0,    0,
    0,    0,    0, 2524,    0,    0, 1599,    0,    0,    0,
    0,    0,    0, 4724,    0, 2407,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1560,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1616,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1632,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2011,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1771,    0,    0,  166,    0,    0, 2470,    0,  164,
    0,    0,    0,    0,  851,    0,  851,    0,    0,  660,
    0,  793,  925,    0,    0,    0,    0, 1345,    0, 1657,
 2524,    0,    0,  105,  437,    0,    0,  139,    0,  156,
  347,    0,    0,    0,    0,    0,    0, 2334, 2334, 2334,
 2334,    0,  379,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2531,    0,    0,
    0,    0,    0, 2507,    0,    0, 1412,    0,  405, 2342,
    0,    0,    0,    0,    0,    0,  899,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1646,    0,    0,    0,
    0,  965,  927,    0,    0,    0, 2210,    0,    0, 3418,
    0,    0, 3464,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1838, 2415,    0,    0,
    0,    0, 3526, 1016,    0, 3464,   -7,    0,    0,    0,
    0,    0,    0,    0,    0, 1038,  939,    0,    0,    0,
    0, 2384,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  258,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  330,    0,    0, 3970,    0,    0,  330,    0,    0,
    0,    0,    0, 7774,    0, 7774,    0,    0, 7774, 7774,
 7774,    0,    0,    0, 2346,    0,    0,    0,    0, 2867,
 2950, 6128,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1248,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  659,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1038,    0,    0,  110,    0,    0,
  821,    0,    0,    0, 2779,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  843,    0,    0,
    0,    0,    0,    0,    0, 2764,    0,    0,  877, 1017,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  467,
  691,  788,    0,    0,    0, 1550,    0,    0,    0,    0,
    0, 1330,    0,    0,    0,    0,    0, 3418,    0,    0,
    0,    0,  841,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2524,    0,    0,  366,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 6454,    0,    0,    0,  375,
    0,    0,    0,    0, 2505,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1663,
 1750,    0,    0,    0,    0,    0,    0,    0, 1751,    0,
    0, 6046,    0, 1751, 1768, 1780,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1040,    0,    0, 7774,    0,    0,    0, 1512,    0,    0,
    0, 2389, 3107, 3280, 2537, 2694,    0,    0,    0,    0,
    0, 1133, 1113,  829,    0,    0,    0,  945,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  625, 1240,    0,  140,    0,    0,
    0,    0,    0,    0,    0, 2334, 2358,    0, 3423,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2547,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1502,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1781,    0,    0,    0,    0,    0,    0,    0,
 1468,    0,    0,    0,    0,    0,    0,    0,    0, 1383,
    0,    0,    0,    0,    0,    0,    0,  939, 2547, 2334,
 1782, 1038,    0, 2556, 6046, 1786,  155,    0,    0,    0,
    0, 6454,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1531,    0,    0,    0, 2203,
    0,    0,    0,    0,    0,    0,    0, 3920,    0, 6910,
    0, 7764,    0,    0,    0,    0,    0,    0,    0, 7968,
    0,    0, 7979,    0,    0, 8002,    0, 8118,    0,    0,
 8247,    0,    0, 8281,    0,    0, 3983,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1663, 1790,    0,
 2385,  352,    0,    0,    0,    0,    0,    0,    0, 1854,
 1857, 1861,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2393,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2483, 1706,    0, 2492,    0, 2487,
    0, 2488, 2491,    0,    0,  791,    0,    0,    0, 2547,
    0, 1786,    0,    0,    0, 3430,    0,    0,    0, 2375,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2387,    0, 2566,    0,    0,    0, 2512,    0, 1502,
 2442,    0,    0,    0,    0,    0,  931,  982,  983,  985,
 1023, 1088,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  889,    0,    0,    0,
    0, 2894,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1926,    0,    0,    0,    0,    0,    0,    0, 1951,
    0,    0,    0,    0,    0,  191,    0,    0,    0,    0,
    0,    0,    0,    0, 6128,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 8310,    0, 8387,    0, 8451,    0,
    0,    0,    0, 8483,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1964,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1589,    0,    0,    0, 1447,    0,    0,
    0,    0,    0,    0,  623,    0,    0,    0,    0, 2334,
    0,    0,    0, 4086,    0,    0,    0,    0,    0,    0,
 2530,    0,    0, 2802, 1865, 3077,    0, 2507,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2547,    0,
    0,    0,    0,    0,    0,    0, 1971,  913,    0,    0,
    0,    0,    0,    0,    0,    0, 2547, 2556,    0,  383,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  118,    0, 8567,    0,    0, 7774,    0,    0,    0,
    0, 8596,    0,    0,    0,    0, 8685,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  352, 1514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  980,    0, 2483,    0,  755,    0,
    0,  407,    0,    0,    0,    0,    0,    0,  143, 2811,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2546,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3005,    0,    0,
  127,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  283,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8735, 8822,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1347,    0,    0,    0,  144,    0,    0,    0,    0,    0,
    0, 2814,    0,    0,    0,    0,    0, 2805,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2559,
    0,    0,    0,    0,    0, 3005,  172,    0,  138,    0,
    0,    0,    0,    0,    0,    0,  649,  618,    0,    0,
  268, 2802,    0,    0,    0,  297,    0,  340, 5392,    0,
    0,    0, 2811,    0,    0,    0,    0,    0,  289,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  474,    0, 1502,  493,  545,
    0,    0, 2483, 1889,    0,    0,  247,    0,  980,  372,
  151, 1547,    0,    0, 2829,    0,    0,    0,    0,    0,
 2813,  338,    0, 2569,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3005,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  786,    0,  119,    0,  144, 1700,
 2834,    0,    0, 2811,    0,    0,    0,    0, 2842,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2483,    0,  340,    0,
    0,    0,    0,    0,    0,    0, 2825,    0,    0,    0,
 5560,    0,    0,    0,    0,    0,  151, 2847,    0,    0,
    0,    0,    0, 2305, 2834,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2890,    0,    0, 2893,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2895,
    2,  264,  -12, -524, 1778,  816, -107,  300, 2887, 2648,
    0,   30,    0, 2656,  -93,  -31,    0,  305, -852,  -20,
  -42,   16, -131, 2658, 2257,    0,    0,    0, 1043,    0,
    0,  -36, 1850, -488,   29, -700, 2309, 2095, -273, 1853,
 2839, -289,    0,  -57,    0, -639,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -506,  189, 1881, -249, 1105, -899,
    0,    0,    0,    0, -315,  -94, -461, 1890, -311,  -46,
    0, -501,    0,    0,  -10, -942,    0, 1534,    0,    0,
 2630, -554,    6,    0,    0,    0,    0, 2603,    0, 2386,
 2636, -158, 1911, -752,    0,    0,    0, -699,    0,    0,
    0,    0,    0,    0, 1961,    0, 2649, 2219, 1907, 2634,
 2637,    0, 1348,    0, -490, 2201,  -49, -612, -706,    0,
    0,  -83, -901, -896, -890, -188, -878,    0, 1571, -956,
    0, 1286,-1113, 1579,    0, 2428,    0,    0,    0, 1283,
 1296, 1254,  150, -232, -672, -288,    0,    0, 2215,    0,
 1772,    0,    0, -238,    0,    0,    0,    0, -150,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -325,    0, -262,    0,-1136,  -74,    0,    0, 1208,
 1359,    0, 1271,    0,    0, 2616,  -92, 2035, 1814, 1644,
    0,    0,    0, -968, -795, -856,    0, -473, -171,  -44,
 -142,    0, -277, 1352, 1054, -544, -447, -686, -883,-1170,
    0,    0,    1,    0,    0,    0,    0,    0, 2677,    0,
    0,    0,    0, 2693, 2469,    0,    0,    0,    0,    0,
 2704,    0,    0,    0,    0,  -15,    0,    0,    0, -320,
 2211, -319, 1776, -421,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2424,    0,    0, 2220,    0,    0,    0,
 2617, 3008,    0,    0,    0,    0,    0, 2745,    0,    0,
    0,    0, 2552,    0, 2550,    0,    0,    0, 2315,    0,
    0,    0, -168,    0,  220,    0, 2303,    0,    0,    0,
 -548,-1095,  689,    0, 2908, 1888,    0, 2341, -575, 2338,
    0, -610, 2092,    0,    0, -117, -661,    0, 1455,-1061,
    0, -885, -592, -546,    0, 1554,    0, 1308,    0, 1355,
    0, 1262,    0,    0,    0,    0, 2562,    0,    0,    0,
 -356, 2395, 2172, -946,    0,    0,    0, -679,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2571,
 1507, 1966,    0, -518,    0,    0,    0, -224,    0,    0,
 2696,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -193, 1912,    0,   -9,
};
#define YYTABLESIZE 9345
static short yytable[] = {                                     110,
  130,  171,  110,  194,   76,   75,  131,  241,  137,   37,
  191,  378,  288,  190,  721,  550,  940,  923,  587,  588,
  965,  174,  186,  555,  225,  260,  263,  261,  264,  758,
 1346,  746,  199,  975,  749,  229,  522,  653, 1019,  959,
  760,  170,  173,  176,  131,  181,  206,  374,  234,  707,
  193,  170,  197, 1207,  176,  230,  131,  181,  520,  210,
  110,  247,  236,  249,  130,  838,  617,  841,  221,  233,
  131,  224,  170,  232,  221,  197,  173,  176,  466,  131,
  181,  210,  110,  177,  176,  211, 1126, 1197,  966,  892,
  228,  294, 1198,  201, 1197,  204, 1380, 1208, 1199, 1198,
  290,  193,  131,  193,  217, 1199,  882,  235, 1216,  622,
 1201,  601,  388,  964,  351,  197,  231, 1201,  699,  914,
  289,  388,  176,  131,  210,  831,  748, 1237, 1161,  705,
  270,  568,  313,  924,  885, 1044,  486,  257,  629,  628,
  992,  248,  691,  377,  512,  217,  378,  961,  217,  999,
  622,  720,  373,  622,  359,  177,  238,  340,  526,  822,
 1254,  526,  292,  217,  650,  733,  748,  748,  622,  487,
  748,  748,  939, 1052,  941, 1310,  937,  699,  257, 1708,
  245,  257,  374,  691,  680,  748,   74,  939,  381,  734,
  568,  699,  736,  737,  738,  739,  257,  629,  628, 1766,
  493,  691, 1039,  701, 1636,   74,  733,  650,  355,  733,
  587,  588,  748,  359,  177,  748,  355,  435, 1450,  809,
  472, 1599, 1018,  968,  733,  948,  716,  258,  193, 1714,
  748,   90,  131, 1115,  390,  110,  530,  485,  399, 1420,
 1550,  131,   74,  390,  536,  555,  692, 1465, 1024, 1255,
  259, 1378,  435, 1654,  648,  791,  269,  693,  724,  521,
   74,  173, 1303,  262, 1305,  131,  524,  680,  191, 1314,
  537,  384,  792,  540,  733,  115,  487,  490,  377,  568,
  384, 1121,  694,  857,  766,  297,  259,  373, 1006, 1416,
 1415, 1474,  660, 1395,  301,  551,  692,  553,  693,  193,
  341,  609, 1106, 1107,  479,  692,  193, 1127,  193,  597,
  193,  605,  245,  193,  917,  918,  693,  259,  111,  499,
 1232,  599,  763,  345, 1668, 1390,  680,  655,  500,  739,
 1165,  176,  181,  803, 1197,  382,  342,  692,  568, 1198,
  131,  694,  173,  302,  485, 1199,  469,  322,  344, 1180,
 1118,  524,  688,  647,  656,  692,   76, 1201,  689,  298,
  806,  102, 1130, 1132, 1158,  126, 1249, 1075,  647,  217,
 1311,  699,  131, 1457, 1669,  176,  390, 1025,  471,  607,
  692,  254,  196,  131,  490, 1670,  112,  131,  739,  661,
  176,  526,  131, 1076, 1215,  137,  320, 1122, 1412, 1743,
  102,  756,   74,  764,  269,  469,  679,  217,  858,  102,
  524,  644,  699,  177, 1628,   86,  378,  102,   86, 1378,
  259,  346,  652,  217,  126,  686,  658,  526,  622,  733,
  699,  384, 1421, 1194,  177,   80,  615,  471,  477,  491,
  627,  196,  938,   14,  765,  217, 1554,  679, 1045,  748,
  622,  748,  861,  955,  786,  303,  177,  253,  919,  748,
  703,  691,  177, 1682, 1303,  679,  387, 1476, 1488,  735,
  956, 1064, 1520,  257, 1521,  680,  466,  615, 1393,  629,
  628,  762,   57,  691,  650,  177,  733, 1362,  591, 1671,
 1713,  987,  253,  996,  989,  615,   94,  477, 1342,  627,
   90,  715, 1580,  732,  177, 1342,  748, 1356,  995, 1548,
  748,  466, 1675,  177,  257,  788,  497,  257,  388,  177,
  755,  193, 1702, 1125, 1547,  387,  755, 1474, 1051,  526,
  691,  680,  257,  253,  466, 1013,  253,  177,  699,  691,
  680, 1405, 1260,  257,  254, 1059,  805,  591,  862, 1689,
  748,  253, 1054, 1277,  889,  748,  191,  860, 1348,  699,
  357, 1408,   76, 1740, 1718,  831,  816,  817,  568,  960,
  704, 1381,  748,  699,  177,  649,  693,  102,  787,  748,
 1037,  986, 1595,  691,  526,  254,  693, 1597,  254,  693,
 1248,  102,  863,  629,  628,  102,  193,  691,  693,  680,
   73, 1751,  487,  254,  469,  834,  102,  217, 1318,  359,
  952,  694,  622,  748,  694,   76,  832,  365,  733,   73,
  102,  102,  699,  102,  216,  102,  733,  808,  748,  615,
  647, 1563,  193, 1631, 1407,  390,  471,  983, 1367,  257,
  390,  981,  982,  692, 1114,  693,  257,  615,  364,  390,
 1635, 1456,  176,  469,  693,  102,   73,  197, 1382,  597,
 1189,  365,  257, 1485,  680,  216,  840,  840,  216,  840,
  485,  840,  896,  748,   73,  191,  365,  752,  931,  694,
  381,  699,  257,  216,  692,  471,  193,  692,  176,  568,
  385,  703,  857,  692,  466,  971,  971,  739,  693,  322,
  524,  692,  477, 1476,  131,  244,  131,  364,  680,  193,
  490, 1697,  693,  983,  126,  193, 1540,  931,  597,  524,
  193, 1559,  680,  694, 1250,  679, 1427,  692, 1005, 1007,
  387,  196, 1108, 1681, 1546, 1111,  692,  694,  679, 1451,
  193, 1467, 1688,  479, 1206,  958,  813,  679,  320,  385,
  113,  257, 1197,  257,  683,  615, 1214, 1198, 1368,  699,
 1477,  257,  487, 1199,  378,  469,  475,  985,  193,  193,
  253,  378,  253,  102, 1557, 1201, 1236,  615, 1054,  801,
  253,  627, 1723, 1177, 1012,  366,   86,  386, 1487,  615,
  692, 1533,  607,  604,  679,  683, 1022,  471,  683, 1479,
 1200, 1584,  967,  679,  131, 1004, 1478, 1200,  138,  894,
 1219,  710, 1394,  683,  131, 1624,   73,  858, 1404, 1452,
  631, 1716,  254,  864,  254, 1112, 1061,  116,  654,  366,
  485,  591,  254, 1109, 1178,   76,  477, 1034, 1035, 1036,
  136,  450,  526, 1038,  366, 1110,  386,  679,  802,  692,
  596,  607,  257, 1049,  390,  176,  181,  390,  210,  110,
  381,  679, 1321,  976,  387,  390,  591, 1590, 1217,  654,
  490,  253,  654,  123,  257,  967,  223,  390,  390,  631,
 1650,  257,  711,  526, 1113,  543,  526,  654,  137,  216,
 1153, 1154, 1591,  253,  591,  780,  544,  591,  401,  136,
  253,  526, 1486,  526,  840,  182,  184,  185,  940,  596,
  840, 1558,  138,  591, 1372,  257, 1774,  223, 1705,  381,
  223,  125,  477,  254,  592,  139,  536,  216,  857,  952,
  392,  176,  781,  255,  253,  223,  977,  466,  867,  591,
  734,  699, 1706,  216,  680,  254,  365,  137, 1549,  365,
 1592, 1220,  254, 1002,  385,  102, 1334,  401,  886, 1336,
 1409,  678, 1717,  699,  524,  216,  536,  536, 1775,  692,
  536,  138, 1218, 1389,  693,  257, 1581,  364,  140,  857,
  364,  393,  389,  592,  388,  536,  254,  536,  773,  392,
  774, 1585,  191,  518,  253,  616, 1477, 1342, 1598,  381,
  587,  588,  597,  680, 1411,  524,  519, 1227,  524,  857,
  699,  378,  857,  314,  365,  531,  493,  536,   76, 1228,
  694, 1227,  391,  524,  198,  524,  126,  265, 1308, 1308,
 1312, 1649,  193,  127, 1155,  324,  782,  597, 1342,  193,
  393,  389, 1478,  388, 1484,  364,  254, 1200,  123,  123,
 1347,  386, 1662,  858,  616, 1664,  531,  493,  365,  531,
  493, 1243, 1162,  699,  600,  597,  466, 1482,  597,  129,
  365,  893,  365,  683,  531,  493,  531,  570, 1641, 1263,
  667,  391, 1358,  667,  597, 1267,  683,  390,  385,  364,
   73,  315, 1359, 1686, 1364,  683, 1366, 1601,  667, 1238,
  526,  364, 1150,  364,  858,  390,  531,  390,  216,  692,
  597, 1164,  653,  325,  366,  600,  526,  366,  136,  381,
  136,   76,   76,  390,  381,  857,  466,  216,  766,  678,
  381,  692,  663,  222,  858,  607,  570,  858, 1686,  857,
 1602, 1458,  683,  526,  218, 1461,  390,  654, 1239,  526,
  971,  683,  526,  653,  223, 1410,  223,  654,  289,  176,
  654,  631,  401,  857,  223,  566,  373, 1555,  137,  654,
  607,  653,  678,  663, 1119,  575,  663, 1333,  692,  386,
 1686,  475,  366, 1221,  536,  386, 1131,  692, 1209, 1210,
  373,  663,  138,  596,  392,  683,  102,  381,  607,  237,
  536,  607, 1603,  536,  536,  226,  536,  688,  381,  683,
  238,  536, 1686,  689,  536, 1729,  654,  607,  265,  136,
 1222, 1784,  524,  306,  566,  654,  366,  536, 1419,   76,
 1392,  692, 1423,  536,  575, 1471,  536, 1261,  366,  625,
  366,  524,  524,  607,  524,  393,  389,  694,  388,  524,
  858, 1632,  524,  487,  526,  223,  596, 1633,  381,  596,
  354,  526,  266,  680,  858,  524, 1583,  592,  360,  654,
 1619,  524,  307,  531,  242,  596,  680,  223, 1469,  526,
  625,  654,  476,  625,  223,  680,  391,  265,  858,  531,
 1525,  152,  531,  531,  493,  531,  493, 1634,  625, 1339,
  531,  596,  592,  531,  493,  536,  694,  390,  694,  526,
 1238,  694, 1526,  526,  616,  242,  531,  493,  223, 1527,
  191,  485,  531, 1489, 1708,  531,  971,  536,  392,  521,
  592,  890,  680,  592,  536, 1704,  616,  477,  536, 1707,
  572,  680,  176,  524,  694,  536,  680,  667,  667,  592,
  152,  390, 1666, 1528,  478,  243, 1308,  242, 1683, 1242,
  193,  490,  437,  536, 1523,  524,  463,  694,  536,  131,
  521,  465,  524,  521,  116,  592,  694,   96,  246,  393,
  389,  524,  388,  524,  308,  680, 1544,  680,  521, 1596,
  521,  487,  508,  536,  531,  493,  250,  536, 1666,  572,
  494,  524,  309,  694, 1170,  680,  524, 1725, 1531, 1646,
  251,  463,  116, 1648,   98, 1767,  531,  493, 1171, 1302,
  391, 1304, 1752,  531,  493,  348,  501,  531,  265,  378,
  574,  653, 1600,  257,  531,  524,  310, 1629,  242,  408,
 1780,  653, 1172,  509,  653, 1747,  655,  116, 1757,   76,
  667,  663,  531,  653,  663,  463, 1582,  531,  493,  485,
  951,  663,  409, 1753,  663, 1200, 1586,  526,  663, 1687,
  463, 1471,  898,  663, 1749,  501,  663,  510, 1666,  242,
  170, 1630,  531,   99,  348,  390,  531,  655,  753,  574,
  655,  667,  663,  100,  252,  511, 1173, 1471,  408,  490,
  653,  251, 1606, 1666,  405,  655, 1771, 1772,  526,  653,
 1754,  526,  931,  694, 1469,  753,  663,  131,  101, 1643,
  663,  409,  173, 1644,  161, 1062,  526, 1063,  526,  663,
  812, 1782,  675, 1676, 1084,  130,  680,  681,  683,  256,
 1667,  131,  251, 1471, 1789,  251,  663, 1730, 1731,  196,
  265, 1085,  858,  653,  694,  858, 1627, 1759,  625,  346,
  251, 1760, 1761,  405,  318,  653,  694,  319,  265, 1659,
  152,  812,  694,  663,  812,  272,  694,  931,  193,  694,
  625,  213,  271,  161, 1758,  663, 1667,  521,  694,  812,
 1471,  176,  152,  663,  900,  487, 1086,  692,  690,  287,
  691, 1690,  693, 1691,  858,  490,  521,  521,  196,  521,
  273, 1709,  902,  320,  521,   44, 1790,  521,  346,  214,
  130,  152, 1590, 1471,  121, 1087,  131,  487,  487,  859,
  521,   52,  859, 1748, 1785,  694,  521, 1692,  274,  296,
  152,  275, 1727,  131,  694,  162,  299, 1591, 1471,  152,
  316,  490,  487,  692,  690,  152,  691,  276,  693,  170,
 1711,  708,   42,  485,  643,  487, 1667,  642,  327,  130,
  329,   76, 1712,  152,   44,  131, 1692,  338,  680,  670,
  276,  859,  131,  277,  692,  690, 1764,  691,  694,  693,
   52, 1667, 1088,   77,   78,  485,  485,  947,  490,  616,
  947,  348,  960,  490,  162, 1592,  350, 1735,  521,   95,
  130,   49,  120,  121, 1777,  947,  131,  947, 1692,  352,
  485,   42,   50,  670,  131,  526,  840,  173,  370,   51,
  521,  490,  356,  485,  680,  490,  490,  521,  670,  102,
  616,  184,  185,  680, 1736,  526,  259,  526,  521,   25,
  744,   96,  526,  173,   52,  526,  490, 1737,  616, 1061,
  490,  751,  864,   97,  750,  655,  521,   48,  526,  465,
  947,  521,  897,  490,  526,  655,  173,  913,  655,   50,
  479,  128,  920,  921,  357,  376,  385,  655,   98,   26,
  678,  928,  812,  932,  933,  934,  935,  936,  393,  928,
  521,  694,  692,  690,  465,  691,  394,  693,   25,  744,
  943,  947,  947,  947,  947,  947,  960,  947,  397,   53,
  692,  690,  132,  691,  398,  693,   48,  465,  196,  947,
  947,  947,  947,  494,  655,  688,  865,  503,   50,  479,
  128,  689,  694,  655,  376,  694,  526,  400,   26,  812,
  196,   54,  812,   49,  133,   55,   51,   99,  402,  812,
   46,  947,  812,  947,  217,  371,  812,  100,  526,  134,
  370,  812,  866,  403,  812,  526, 1149,  867,  503,  196,
  404,  503,  859,  868,  710,  641,  526,  655,  259,  471,
  812,  688,  101,  473,  102,  483,  503,  689,  196,  655,
  881,  694,  869,  880,  526,  217,  196,  196,  217,  526,
  694,  508,   49,  196,  812,   51,  474,  812,  812,   46,
  947,  710,  688,  217,  498,  129, 1170,  812,  689,  507,
  330,  196,  698,  554,  710,  697,  692,  690,  526,  691,
 1171,  693,  515,  196,  812,  196, 1065, 1066, 1070,  528,
  117, 1078, 1080, 1082, 1281, 1090, 1093, 1096, 1098, 1101,
 1104,  947,  509,   47, 1172,   14,  812,  331,  196,  332,
  217,  812, 1014, 1015,  925,  692,  690,  535,  691,  361,
  693,  499,  538,  812,  129,  812, 1043,  465, 1776, 1042,
  500,  548,  333,  196, 1134, 1135,  510, 1137,  552, 1139,
  602,  670, 1142, 1143, 1144, 1145, 1146, 1147,  670,  117,
  827,  670,  608, 1050,  511,  670,  827,  334, 1173, 1151,
  606,  921,   47,  670, 1170, 1163,  610,  656, 1162,  217,
 1181,  636,  947, 1042,  947,  947,  362,  637, 1171,  670,
  688,  656,  947,  638, 1188, 1230,  689, 1187, 1229,  656,
  947,  827,  827,  827,  827,  827,  947,  827,  688,  947,
  124,  124, 1172,  670,  689,  656,  641,  645,  659,  827,
  827,  827,  827,  947, 1235,  650,  670, 1234,  947,  947,
  662,  695,  947,  692,  690, 1159,  691,  664,  693,  947,
 1436,  947,  947,  670, 1316,  947,  665, 1315,  670,  947,
  666,  667,  947,  827,  947,  668,  947,  363,  364,  365,
 1429,  947,  947, 1162,  947,  503, 1173,  503,  669,  656,
  670,  361,  503,  696, 1464,  503,  153, 1463,  670,  671,
  947,  947,  670,  947,  670,  368,  369, 1466,  503,  672,
 1463,  363,  364,  365,  673,  184,  185,  947,  678,  947,
  947,  947, 1765, 1788,  947, 1463, 1708,  947,  947,  679,
  687,  698,  700,  718,  366,  367,  714,  947,  723,  368,
  369, 1141,  692,  690,  688,  691,  928,  693,  362,  719,
  689,  947,  725,  726,  947,  153,  730,  729, 1284, 1285,
  740, 1287, 1289,  499,  741,  743,  744,  777,  753, 1294,
  947,  778,  781,  779,  784,  257,  947,  793,  789,  537,
  367,  947,  795,  688,  796,  798,  503,  794,  799,  689,
  804,  810,  947,  947,  367,  947,  811,  814,  815,  824,
  465,  947,  367, 1319,  692,  690,  826,  691,  503,  693,
  827,  835,  875,  781, 1325,  503,  781,  837,  367,  537,
  537,  836, 1332,  537,  921,  692,  690,  921,  691, 1151,
  693,  781,  839, 1340,  921, 1343, 1344,  876,  537,  878,
  537,  921,  827,  947,  827,  827,  883,  887,  888,  503,
  915,  926,  827,  363,  364,  365,  947,  184,  185,  962,
  827,  953,  988,  969,  983,  805,  827,  993,  994,  827,
  537,  996,  367, 1000,  369,  756,  366,  367, 1003, 1009,
 1374,  368,  369,  827, 1001, 1010, 1011, 1017,  827,  827,
 1020,  688,  827, 1031, 1320,  692,  690,  689,  691,  827,
  693,  827,  827, 1026, 1033,  827, 1040, 1057, 1045,  827,
 1058,  655,  827, 1072,  827,  369,  827, 1120,  369, 1117,
 1123,  827,  827, 1079,  827, 1083, 1115, 1091, 1094,  465,
 1099, 1102, 1105,  369, 1133, 1136, 1138, 1140, 1160, 1148,
  827,  827,  688,  827,  692,  690, 1424,  691, 1167,  693,
 1323,  692,  690, 1175,  691, 1432,  693,  827,  839,  827,
  827,  827, 1437, 1169,  827, 1183,  642,  827,  827, 1185,
 1324,  692,  690, 1190,  691,  153,  693,  827, 1193, 1202,
  688, 1211, 1213, 1223,  526, 1231,  689, 1233, 1240,  465,
 1244,  827, 1245, 1246,  827, 1247, 1256,  153, 1258,  839,
  839,  839,  839,  839, 1051,  839, 1268, 1265, 1269,  921,
  827, 1270, 1271,  921, 1273, 1274,  827,  839,  839,  839,
  839,  827, 1279, 1278, 1317,  526,  153, 1280,  526, 1282,
 1283, 1290,  827,  827,  781,  827, 1291,  537, 1292, 1293,
 1295,  827,  688,  526, 1296,  153, 1297, 1298,  689, 1299,
 1300,  839,  857,  537,  153, 1301,  537,  537, 1335,  537,
  153,  661, 1338,  688,  537,  692,  690,  537,  691,  689,
  693, 1345, 1349, 1350, 1351, 1353, 1352, 1354,  153, 1360,
  537,  230, 1663,  827, 1363, 1369,  537, 1365, 1371,  537,
  530,  781,  692,  690,  781,  691, 1383,  693, 1384, 1379,
 1391,  781,  661, 1406,  781,  661,  842, 1385,  781, 1568,
 1569, 1386, 1387,  781,  692,  690,  781,  691, 1396,  693,
  661,  891,  230,  895, 1388,  230,  899,  901,  903, 1413,
 1417, 1699,  781,  688,  921, 1418, 1422,  921, 1428,  689,
  230, 1275, 1449, 1448, 1453, 1459, 1460,  842,  842,  842,
  842,  842, 1480,  842, 1532, 1541,  781, 1522,  537,  781,
  781, 1534, 1535, 1537, 1272,  842,  842,  842,  842,  781,
 1608, 1536, 1538, 1539, 1542, 1551, 1556, 1326,  692,  690,
  537,  691,  688,  693, 1543, 1553,  781,  537,  689,  688,
 1560,  537, 1561, 1562, 1564,  689,  369, 1565,  537,  842,
 1566, 1567, 1570,  369, 1571, 1572,  369, 1573,  781,  688,
  369, 1574, 1575,  781, 1576,  689,  537, 1577,  369, 1578,
  839,  537,  839,  839, 1579,  781, 1588,  781, 1587, 1642,
  839, 1589, 1607, 1605,  369, 1610, 1609, 1645,  112, 1327,
  692,  690, 1615,  691,  839,  693,  537,  839, 1611, 1612,
  537, 1623, 1613, 1614, 1617, 1616, 1622, 1625,  369, 1626,
 1647,  839,  526,  843,  526, 1652,  839,  839, 1653,  526,
  839,  369,  526, 1661, 1658, 1677, 1618,  839, 1660,  839,
  839, 1685, 1698,  839, 1719,  526, 1724,  839,  369, 1721,
  839, 1726,  839, 1728,  839, 1733, 1741, 1734, 1745,  839,
  839, 1463,  839,  688,  843,  843,  843,  843,  843,  689,
  843, 1717, 1435, 1746, 1716,  369,  139, 1763,  839,  839,
 1768,  839,  843,  843,  843,  843, 1769,  369, 1708,  369,
  688, 1770, 1783,  921, 1773,  839,  689,  839,  839,  839,
 1786, 1787,  839,    1,   30,  839,  839,   32,  187,  622,
  203,  748,  688,  564,  373,  839,  843,  271,  689,  242,
  646,  230,  149,  526,  269,   82,  524,  930,  842,  839,
  842,  842,  839,  213, 1739, 1328,  692,  690,  842,  691,
  661,  693,  460,  661,  524,  526,  193,  276,  839,  401,
  661,  524,  526,  661,  839,  842,  678,  661,  929,  839,
  676,  672,  661,  194,  674,  661,  275,  187,  289,  842,
  839,  839,  374,  839,  842,  842,  688,  281,  842,  839,
  705,  661,  689,  704,  287,  842,  526,  842,  842,  692,
  690,  842,  691,  223,  693,  842,  837,  288,  842,  706,
  842,  325,  842,  335,  616,  661, 1703,  842,  842,  661,
  842,  692,  690,  323,  691,  667,  693,  715,  661,   26,
  242,  839,   29,  526,   39, 1306,  842,  842,  117,  842,
  401,  392,  391, 1266, 1264,  661,  877,  837,  688,  837,
  837,  837,  242,  842,  689,  842,  842,  842,  825,  242,
  842, 1056,  227,  842,  842,  837,  837,  837,  837, 1262,
 1252, 1545,  661,  842,  526,  517,  503,  526,  561, 1192,
  492, 1241,  790,  963,  661, 1655, 1524,  842, 1253,  836,
  842,  532,  526,  242,  533,  843, 1530,  843,  843,  837,
  998, 1715, 1722,  772,  991,  843,  842, 1329,  692,  690,
 1756,  691,  842,  693, 1720, 1373, 1781,  842, 1674, 1330,
  692,  690,  843,  691, 1750,  693,  640, 1684,  842,  842,
  836,  842,  836,  836,  836, 1357,  843,  842, 1182,  541,
 1483,  843,  843,  523,  251,  843,  776,  514,  836,  836,
  836,  836,  843, 1403,  843,  843, 1032, 1023,  843,  812,
  119,  663,  843,  484,  709,  843,  713,  843,  954,  843,
  269,  967, 1166, 1593,  843,  843,  187,  843,  950,  842,
  949, 1762,  836, 1651,  688,  251, 1732, 1791,  251,  722,
  689, 1286,  884,  843,  843, 1129,  843,  187, 1331,  692,
  690,    0,  691,  251,  693,  639, 1337,    0,    0,    0,
  843,    0,  843,  843,  843,    0,  231,  843,    0,  187,
  843,  843,    0,    0,    0,  187,  412,    0,    0,    0,
  843,    0,    0,    0,    0,    0,    0,  688,    0,    0,
    0,    0,    0,  689,  843,    0,  838,  843,  187,    0,
 1454,  692,  690,    0,  691,    0,  693,  231,    0,  727,
  231,    0,    0,  843,    0,  689,    0,  187,  837,  843,
  837,  837,    0,    0,  843,  231,  187,    0,  837,    0,
    0,    0,  187,    0,    0,  843,  843,  838,  843,  838,
  838,  838,    0,    0,  843,  837,    0,    0,    0,    0,
  187,    0,    0,    0,    0,  838,  838,  838,  838,  837,
    0,  526,    0,  526,  837,  837,    0,    0,  837,    0,
    0,  526,    0,    0,    0,  837,    0,  837,  837,    0,
   73,  837,  102,    0,  526,  837,  843,  187,  837,  838,
  837,    0,  837,  419,    0,    0,  688,  837,  837,    0,
  837,  836,  689,  836,  836,    0,    0,    0,  688,    0,
    0,  836,  425,    0,  689,    0,  837,  837,    0,  837,
  428,  429,  430,    0,    0, 1455,  692,  690,  836,  691,
    0,  693,    0,  837,    0,  837,  837,  837,    0,    0,
  837,    0,  836,  837,  837,    0,    0,  836,  836,    0,
    0,  836,    0,  837,    0,    0,    0,    0,  836,    0,
  836,  836,  526,    0,  836,    0,    0,  837,  836,  841,
  837,  836,  251,  836,  251,  836,    0,    0,    0,    0,
  836,  836,  251,  836,  526,    0,  837,  688,    0,    0,
    0,  526,  837,  689,    0,    0,    0,  837,    0,  836,
  836,    0,  836,    0,    0,    0,    0,    0,  837,  837,
  841,  837,  841,  841,  841,    0,  836,  837,  836,  836,
  836,    0,    0,  836,    0,  526,  836,  836,  841,  841,
  841,  841,    0,    0,    0,    0,  836,    0,    0,  688,
    0,    0,    0,    0,  242,  689,  231,    0,    0,    0,
  836,    0,    0,  836,    0,    0,    0,    0,  838,  837,
  838,  838,  841,    0,    0,    0,    0,    0,  838,  836,
 1462,  692,  690,  251,  691,  836,  693,    0,    0,    0,
  836,    0,    0,    0,    0,  838,    0,    0,    0,    0,
    0,  836,  836,    0,  836,  251,    0,    0,    0,  838,
  836,    0,  251,    0,  838,  838,    0,  554,  838,    0,
    0,    0,  188,    0,    0,  838,    0,  838,  838,  181,
    0,  838,    0,    0,    0,  838,    0,    0,  838,    0,
  838,    0,  838,    0,    0,    0,  251,  838,  838,    0,
  838,    0,  836,    0,    0,  242,    0,    0,  554,    0,
    0,  554,    0,  544,    0,    0,  838,  838,    0,  838,
  181,    0,    0,  181,  688,    0,  554,  242,  554,    0,
  689,  188,    0,  838,  242,  838,  838,  838,  181,    0,
  838,    0,    0,  838,  838,   95,    0,    0,    0,    0,
    0,    0,    0,  838,  544,    0,    0,  544,  554,    0,
    0,    0,    0,    0,  370,    0,    0,  838,  242,    0,
  838,    0,  544,    0,  544,  528,  904,  905,  906,  907,
  908,  909,  910,  911,    0,    0,  838,   96,    0,    0,
    0,  841,  838,  841,  841,    0,    0,  838,    0,   97,
    0,  841,   95,    0,  544,    0,    0,    0,  838,  838,
    0,  838,    0,  114,    0,    0,  528,  838,  841,  528,
    0,    0,    0,    0,   98,    0,    0,    0,    0,    0,
    0,    0,  841,    0,  528,    0,  528,  841,  841,    0,
    0,  841,    0,    0,   96,    0,    0,    0,  841,    0,
  841,  841,    0,    0,  841,    0,   97,    0,  841,  838,
    0,  841,    0,  841,    0,  841,  528,    0,    0,  688,
  841,  841,    0,  841,    0,  689,    0,    0,    0,    0,
    0,   98,    0,    0,    0,    0,    0,    0,    0,  841,
  841,    0,  841,   99,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  100,    0,    0,  841,    0,  841,  841,
  841,    0,    0,  841,    0,    0,  841,  841,    0,    0,
    0,    0,    0,    0,    0,  554,  841,    0,  101,    0,
  188,    0,    0,    0,    0,    0,    0,  181,    0,  622,
  841,  554,  432,  841,  433,  554,  102,  554,    0,    0,
   99,  188,  554,    0,    0,  554,    0,  435,  181,  841,
  100,    0,  259,    0,    0,  841,    0,    0,  554,    0,
  841,  544,    0,  188,  554,    0,    0,  554,    0,  188,
  181,  841,  841,    0,  841,  101,  181,  544,    0,    0,
  841,  544,    0,  544,    0,    0,    0,    0,  544,    0,
    0,  544,  188,  102,    0,    0,    0,    0,    0,  181,
    0,    0,    0,    0,  544,  361,    0,    0,    0,    0,
  544,  188,    0,  544,  622,    0,    0,  432,  181,  433,
  188,    0,  841,  528,    0,    0,  188,  181,    0,    0,
    0,    0,  435,  181,    0,    0,  554,  259,    0,  528,
    0,    0,    0,  528,  188,  528,    0,    0,    0,    0,
  528,  181,    0,  528,    0,    0,    0,    0,  554,    0,
    0,    0,  362,    0,    0,  554,  528,    0,    0,  554,
    0,    0,  528,    0,    0,  528,  554,    0,    0,    0,
    0,  188,  544, 1425, 1426,    0,    0,    0,  181, 1430,
 1431,    0, 1433, 1434,  554,    0,    0,    0,    0,  554,
 1438, 1439, 1440, 1441,  544, 1442, 1443, 1444, 1445, 1446,
 1447,  544,    0,    0,    0,  544,    0,    0,   59,    0,
    0,    0,  544,    0,  554,    0,    0,    0,  554,   60,
    0,    0,    0,    0,    0,    0,   61,    0,    0,    0,
  544,    0,    0,    0,  528,  544,    0,    0,    0,   62,
    0,    0,    0,  418,    0,    0,  406,    0,    0,  782,
    0,    0,    0,    0,    0,  407,  528,  363,  364,  365,
  544,  184,  185,  528,  544,    0,    0,  528,    0,  408,
    0,    0,  618,    0,  528,  361,  409,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  782,    0,  528,  782,  619,  426,  427,  528,    0,    0,
   63,    0,    0,  410,    0,    0,  411,    0,  782,    0,
    0,    0,  764,    0,    0,    0,   64,    0,    0,    0,
    0,    0,  528,    0,    0,    0,  528,    0,    0,    0,
    0,  406,    0,   65,  412,  413,    0,    0,    0,  637,
  407,  637,  637,    0,  637,    0,  414,    0,   66,    0,
  415,    0,   67,  764,  408,   68,  764,  637,  620,    0,
  361,  409,  637,   69,    0,    0,    0,    0,    0,    0,
    0,  764,    0,    0,   81,    0,    0,    0,    0,  619,
    0,   82,   70,    0,    0,   83,   84,    0,  410,    0,
    0,  411,    0,    0,   85,    0,    0,    0,    0,    0,
    0,  621,    0,    0,    0,    0,   86,  416,    0,    0,
    0,    0,    0,  417,    0,  196,    0,    0,    0,  412,
  413,    0,    0,  418,    0,    0,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,  363,  364,  365,
  102,  184,  185,  620,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,   87,    0,    0,  196,  426,  427,    0,  428,  429,
  430,    0,  682,    0,  431,    0,  621,    0,   88,    0,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,   89,    0,    0,    0,   90,  418,    0,
    0,  782,    0,    0,    0,    0,    0,    0,   91,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,    0,    0,    0,    0,    0,  419,   92,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,  637,
  426,  427,    0,  428,  429,  430,  637,    0,  782,  431,
    0,  782,    0,    0,  635,  637,  635,  635,  782,  635,
    0,  782,    0,    0,    0,  782,    0,    0,    0,  637,
  782,    0,  635,  782,    0,  637,  637,  635,    0,    0,
    0,    0,    0,  637,    0,    0,    0,    0,    0,  782,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  637,    0,    0,  637,    0,    0,    0,
    0,  764,    0,  782,  764,    0,  782,  782,    0,    0,
    0,  764,    0,    0,  764,    0,  782,    0,  764,    0,
    0,    0,  637,  764,  637,  637,  764,    0,    0,  622,
    0,    0,  432,  782,  433,    0,  637,    0,    0,    0,
  637,    1,  764,  196,    0,    0,    0,  435,    0,    0,
    0,    0,  259,    0,    0,  782,    0,    0,    0,    0,
  782,    0,    0,    2,  196,    0,  764,    0,    0,  764,
  764,    3,  782,    0,  782,    0,    0,    0,    4,  764,
    5,    0,    6,    0,    0,    0,  196,    7,    0,    0,
    0,    0,  196,    0,    8,    0,  764,  637,    0,    0,
    0,    0,    0,  637,    0,    0,    0,    0,    9,    0,
  562,    0,    0,  637,  563,  196,    0,    0,  764,   10,
    0,    0,  756,  764,    0,    0,    0,  637,  637,  637,
  637,  637,  637,  564,  196,  764,  565,  764,    0,    0,
    0,  637,  196,  196,    0,    0,    0,    0,    0,  196,
  637,  637,  637,  637,  637,  637,  637,  637,  637,    0,
  637,    0,    0,    0,  635,  637,  637,  196,  637,  637,
  637,  635,    0,    0,  637,    0,    0,  566,    0,    0,
  635,    0,    0,  567,  568,  569,   11,  570,  571,  572,
    0,  573,    0,   12,  635,    0,   13,    0,   14,   15,
  635,  635,    0,    0,  196,    0,    0,    0,  635,  434,
    0,  574,  432,  575,  433,  196,  692,  690,    0,  691,
    0,  693,    0,   16,    0,    0,    0,  435,  635,    0,
    0,  635,  259,    0,  855,  854,  856,    0,    0,    0,
    0,    0,    0,    0,  576,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  406,  635,    0,  635,
  635,  577,    0,    0,    0,  407,    0,    0,    0,    0,
    0,  635,   17,    0,    0,  635,    0,    0,    0,  408,
    0,    0,    0,  578,    0,  361,  409,   18,    0,    0,
    0,  434,    0,    0,  432,    0,  433,    0,  102,    0,
    0,    0,    0,    0,  619,    0,    0,    0,    0,  435,
    0,    0,    0,  410,  259,    0,  411,    0,  579,  580,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  635,    0,    0,  581,    0,    0,  635,    0,
    0,    0,    0,    0,  412,  413,    0,    0,  635,    0,
    0,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,  635,  635,  635,  635,  635,  635,  620,    0,
    0,    0,    0,    0,    0,    0,  635,    0,    0,    0,
    0,    0,    0,    0,    0,  635,  635,  635,  635,  635,
  635,  635,  635,  635,    0,  635,    0,    0,    0,    0,
  635,  635,    0,  635,  635,  635,    0,    0,    0,  635,
    0,  621,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0, 1067,
    0,    0, 1068,  418,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,  407,  842,  363,  364,  365,
  102,  184,  185,    0,  688,    0,    0,    0,    0,  408,
  689,  419,    0,  843,    0,  361,  409,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,  410,  431,    0,  411,    0,    0,  844,
    0,    0,    0,    0,    0,    0,    0,    0,  845,    0,
    0, 1077,    0,    0, 1068,    0,  846,    0,  406,    0,
    0,    0,    0,    0,  412,  413,  434,  407,    0,  432,
    0,  433,    0,    0,    0,  847,  414,  848,    0,    0,
  415,  408,    0,    0,  435,    0,    0,  361,  409,  259,
    0,  849,    0,  850,  851,  852,    0,    0,    0,    0,
    0,  925,  692,  690,    0,  691,    0,  693,    0,    0,
    0,    0,    0,    0,    0,  410,    0,    0,  411,    0,
  855,  854,  856,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1069,    0,  416,    0,    0,
    0,    0,    0,  417,  853,    0,  412,  413,    0,    0,
    0,    0,    0,  418,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0,    0, 1069,    0,  416,
    0,    0,    0,    0,    0,  417,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  418,  221,    0,    0,    0,
  221,    0,    0,    0,    0,    0,    0,    0,    0,  363,
  364,  365,  102,  184,  185,  221,    0,    0,    0,  221,
    0,    0,  221,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0, 1081,  426,  427, 1068,
  428,  429,  430,  406,    0,    0,  431,    0,    0,    0,
    0,  434,  407,  221,  432,    0,  433,    0,    0,  221,
  221,  221,    0,  221,  221,  221,  408,  221,    0,  435,
    0,    0,  361,  409,  259,    0,    0,    0,    0,    0,
    0,    0,  842,    0,    0,    0,    0,  221,    0,  221,
  688,    0,    0,    0,  456,    0,  689,    0,    0,  843,
  410,    0,    0,  411,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,    0,
  221,  456,  456,  456,  456,   81,  456,    0,    0,  456,
  456,  412,  413,  434,    0,  844,  432,  221,  433,    0,
    0,    0,    0,  414,  845,    0,    0,  415,    0,  828,
    0,  435,  846,  563,    0,    0,  259,    0,    0,  221,
    0,    0,    0,    0,    0,    0,    0,    0,  829,    0,
    0,  847,  564,  848,    0,  565,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  849,    0,  850,
  851,  852,    0,    0,  221,  221,    0,    0,    0,    0,
    0,    0, 1069,    0,  416,    0,    0,    0,    0,    0,
  417,  221,    0,    0,    0,    0,  566,    0,    0,    0,
  418,    0,  567,  568,  569,    0,  570,  571,  572,    0,
  573,    0,    0,    0,  363,  364,  365,  102,  184,  185,
  853,    0,    0,    0,    0,    0,    0,    0,  419,    0,
  574,    0,  575,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0, 1089,  426,  427, 1068,  428,  429,  430,  406,    0,
    0,  431,    0,  576,    0,    0,  434,  407,    0,  432,
    0,  433,    0,    0,    0,    0,    0,    0,    0,    0,
  577,  408,    0,    0,  435,    0,    0,  361,  409,  259,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  578,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  410,    0,    0,  411,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1092,    0,    0, 1068,  579,  580,    0,
  406,    0,    0,    0,    0,    0,  412,  413,  434,  407,
    0,  432,    0,  433,  581,    0,    0,    0,  414,    0,
    0,  658,  415,  408,    0,    0,  435,    0,    0,  361,
  409,  259,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,  658,    0,    0,  658,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1069,    0,  416,
  658,    0,    0,    0,    0,  417,    0,    0,  412,  413,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,    0,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0, 1097,  426,  427, 1068,
  428,  429,  430,  406,    0,    0,  431,    0,    0, 1069,
    0,  416,  407,    0,    0,    0,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,  408,  418,    0,    0,
    0,    0,  361,  409,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
  410,    0,    0,  411,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0, 1100,  426,
  427, 1068,  428,  429,  430,  406,    0,    0,  431,    0,
    0,  412,  413,  434,  407,    0,  432,    0,  433,    0,
    0,    0,    0,  414,    0,    0,    0,  415,  408,    0,
    0,  435,    0,    0,  361,  409,  259,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  410,    0,    0,  411,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1069,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,  412,  413,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,  414,    0,    0,    0,  415,
  658,    0,    0,  658,  363,  364,  365,  102,  184,  185,
  658,    0,    0,  658,    0,    0,    0,  658,  419,    0,
    0,    0,  658,    0,    0,  658,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,  658,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0, 1069,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,  658,    0,    0,    0,  658,
    0,    0,  418,    0,    0,    0,    0,    0,  658,    0,
    0,    0,    0,    0,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,  658,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,    0,  425,
  317,    0,  658, 1103,  426,  427, 1068,  428,  429,  430,
  406,    0,    0,  431,  658,    0,    0,    0,  434,  407,
  464,  432,    0,  433,    0,    0,    0,    0,    0,    0,
    0,  317,    0,  408,    0,    0,  435,    0,  317,  361,
  409,  259,  317,  317,    0,  317,    0,    0,  317,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  317,    0,    0,  143,    0,  410,  317,    0,
  411,    0,    0,    0,  144,  145,  146,  147,  148,  149,
  150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
    0,    0,  160,  161,  162,  163,    0,  164,  412,  413,
  165,  166,  434,    0,    0,  432,    0,  433,    0,    0,
  414,    0,    0,    0,  415,    0,    0,    0,    0,    0,
  435,    0,    0,    0,    0,  259,    0,    0,  317,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  317,    0, 1371,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  317,    0,    0,    0,    0,    0,    0, 1069,
    0,  416,  317,    0,    0,    0,    0,  417,    0,    0,
    6,  317,    0,  317,    0,    0,    0,  418,    0,    0,
  317, 1490, 1491,    0, 1492,    0,    0, 1493,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0, 1494,    0,    0,    0,  419,    0, 1495,    0,  317,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0,  317,  426,
  427,    0,  428,  429,  430,  406,    0,    0,  431,    0,
    0,    0,    0,    0,  407,  941,    0,  941,  941,    0,
  941,    0,    0,    0,    0,    0,    0,    0,  408,    0,
    0,  539,    0,  941,  361,  409,    0, 1496,  941,    0,
  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
  154,  155,  156,  157,  158,  159,   14,    0,  160,  161,
  162,  163,  410,  164,    0,  411,  165,  166,    0,    0,
    0, 1497,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   16,    0,    0,    0,    0,    0,    0,    0,  406,
 1618,  362, 1498,  412,  413,    0,    0,  637,  407,  102,
  637,    0,  637,    0,    0,  414,    0,    0,    0,  415,
    0,    0,  408,    0,    0,  637,    0,    0,  361,  409,
  637,    0,    0,    0,    0,    0,    0,    0, 1499,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  410, 1500,    0,  411,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  416,    0,    0,    0,
    0,    0,  417,    0,    0,  362,    0,  412,  413,    0,
    0,    0,  418,    0,    0,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,    0,  425,
    0,    0,    0,    0,  426,  427,    0,  428,  429,  430,
  682,    0,  941,  431,    0,    0,    0,    0,    0,    0,
  416,  941,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,  941,  418,    0,    0,    0,
    0,  941,  941,    0,    0,    0,    0,    0,    0,    0,
  363,  364,  365,  102,  184,  185,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,  941,
    0,    0,  941,  366,  367,  420,  421,  422,  368,  369,
  423,  424,    0,  425,    0,    0,    0,  637,  426,  427,
    0,  428,  429,  430,  637,    0,    0,  431,  941,    0,
  941,  941,  434,  637,    0,  432,    0,  433,    0,    0,
    0,    0,  941,    0,    0,    0,  941,  637,    0,    0,
  435,    0,    0,  637,  637,  259,    0,    0,    0,    0,
    0,  637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  637,    0,    0,  637,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  941,    0,    0,    0,    0,    0,  941,
    0,    0,  637,  637,    0,    0,    0,    0,    0,  941,
    0,    0,    0,    0,  637,    0,    0,    0,  637,    0,
    0,    0,    0,  941,  941,  941,  941,  941,  941,    0,
    0,    0,    0,    0,    0,    0,    0,  941,    0,    0,
    0,    0,    0,    0,    0,    0,  941,  941,  941,  941,
  941,  941,  941,  941,  941,    0,  941,    0,    0,    0,
    0,  941,  941,    0,  941,  941,  941,    0,    0,    0,
  941,    0,    0,    0,    0,  637,    0,    0,    0,    0,
    0,  637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  637,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  637,  637,  637,  637,  637,
  637,    0,    0,    0, 1371,    0,    0,    0,    0,  637,
    0,    0,    0,    0,    0,    0,    0,    0,  637,  637,
  637,  637,  637,  637,  637,  637,  637,    0,  637,    0,
    0,    0,    0,  637,  637,    6,  637,  637,  637,  406,
  434,    0,  637,  432,    0,  433, 1490, 1491,  407, 1492,
    0,    0, 1493,    0,    0,    0,    0,    0,  435,    0,
    0,    0,  408,  259,    0,    0, 1494,    0,  361,  409,
    0,    0, 1495,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  410,    0,    0,  411,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  434,  412,  413,  432,
    0,  433, 1496,    0,    0,    0,  221,    0,    0,  414,
  221,    0,    0,  415,  435,    0,    0,    0,    0,  259,
    0,   14,    0,    0,    0,  221,    0,    0,    0,  221,
    0,    0,  221,    0,    0,    0, 1497,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1498,    0,    0,
    0,    0,    0,    0,  102,    0,    0,    0,    0,    0,
  416,    0,    0,  221,    0,    0,  417,    0,    0,  221,
  221,  221,    0,  221,  221,  221,  418,  221,    0,    0,
    0,    0,    0, 1499,  674,    0,    0,    0,    0,    0,
  363,  364,  365,  102,  184,  185,    0,  221,    0,  221,
    0,    0, 1500,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,    0,  425,    0,    0,    0,  406,  426,  427,
  221,  428,  429,  430,    0,    0,  407,  431,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
  408,    0,    0,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,    0,    0,  434,  927,    0,  432,  221,
  433,    0,    0,    0,    0,    0,    0,    0,    0,  806,
    0,    0,    0,  435,  410,    0,    0,  411,  259,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  221,  221,    0,    0,    0,    0,
    0,    0,    0,  406,    0,  412,  413,    0,    0,    0,
  806,  221,  407,  806,    0,    0,    0,  414,    0,  434,
    0,  415,  432,    0,  433,    0,  408,    0,  806,    0,
    0,    0,  361,  409,    0,    0,    0,  435,    0,    0,
    0,    0,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  410,    0,    0,  411,    0,    0,    0,    0,  682,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,  362,
    0,  412,  413,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,  414,    0,    0,    0,  415,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,    0,    0,  431,    0,    0,    0,    0,
    0,    0,    0,    0,  416,    0,    0,    0,    0,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,  406,    0,    0,    0,    0,    0,    0,    0,
    0,  407,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,  408,    0,    0,  419,    0,
    0,  361,  409,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,  806,  426,  427,    0,  428,  429,  430,    0,  410,
    0,  431,  411,    0,    0,  434,  406,    0,  432,    0,
  433,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,  435,    0,    0,    0,    0,  259,  408,
  412,  413,    0,    0,    0,  361,  409,    0,    0,    0,
    0,    0,  414,    0,    0,    0,  415,    0,  806,    0,
    0,  806,    0,    0,    0,    0,    0,    0,  806,    0,
    0,  806,    0,  410,    0,  806,  411,    0,    0,  434,
  806,    0,  432,  806,  433,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,  806,
    0,    0,  259,    0,  412,  413,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,  414,    0,    0,  417,
  415,    0,    0,  806,    0,    0,  806,  806,    0,  418,
    0,    0,    0,    0,    0,    0,  806,    0,    0,    0,
    0,    0,    0,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0,  806,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,  806,  425,  416,    0,    0,
  806,  426,  427,  417,  428,  429,  430,    0,    0,    0,
  431,    0,  806,  418,  806,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1095,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,  406,    0,    0,  426,  427,    0,  428,  429,
  430,  407,    0,    0,  431,    0,    0,    0,    0,  434,
    0,    0,  432,    0,  433,  408,    0,    0,    0,    0,
    0,  361,  409,    0,    0,    0,    0,  435, 1657,    0,
    0,    0,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
    0,    0,  411,    0,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  412,  413,    0,    0,    0,  361,  409,    0,    0,    0,
    0,    0,  414,    0, 1700,    0,  415,  432,    0,  433,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  435,  410,    0,    0,  411,  259,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1275,  412,  413,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,  414,    0,    0,  417,
  415,    0,    0,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,    0,  425,  416,    0,    0,
    0,  426,  427,  417,  428,  429,  430,    0,    0,    0,
  431,    0,    0,  418,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0, 1288,  407,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,  408,
    0,  419,    0,    0,    0,  361,  409,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,  410,  431,    0,  411,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  406,    0,    0,  412,  413,    0,    0,    0,  434,
  407,    0,  432,    0,  433,    0,  414,    0,    0,    0,
  415,    0,    0,  785,  408,    0,    0,  435,    0,    0,
  361,  409,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  410,    0,
    0,  411,    0,    0,  785,    0,    0,  785,    0,    0,
    0,    0,    0,  941,    0,    0,  941,  416,  941,    0,
    0,    0,  785,  417,    0,    0,    0,  362,    0,  412,
  413,  941,    0,  418,    0,    0,  941,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0,    0,    0,    0,    0,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,  562,  418,    0,
    0,  563,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
  564,    0,    0,  565,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,  783,    0,    0,
  426,  427,    0,  428,  429,  430,  406,    0,  784,  431,
    0,    0,    0,    0,  566,  407,    0,    0,    0,    0,
  567,  568,  569,    0,  570,  571,  572,    0,  573,  408,
    0,  814,    0,    0,    0,  361,  409,    0,  783,    0,
    0,  783,    0,    0,    0,    0,    0,    0,  574,  784,
  575,    0,  784,    0,    0,  785,  783,    0,    0,    0,
    0,    0,    0,  410,    0,    0,  411,  784,    0,    0,
  941,    0,  814,    0,    0,  814,    0,    0,    0,  941,
    0,  576,    0,    0,    0,    0,    0,    0,    0,    0,
  814,    0,    0,  941,  412,  413,    0,    0,  577,  941,
  941,    0,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,  785,    0,    0,  785,    0,    0,    0,    0,
  578,    0,  785,    0,    0,  785,    0,  941,    0,  785,
  941,    0,    0,    0,  785,  102,    0,  785,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  778,    0,    0,
    0,    0,    0,  785,    0,  579,  580,    0,  941,  941,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
  941,    0,  581,  417,  941,    0,    0,  785,    0,    0,
  785,  785,    0,  418,    0,    0,    0,    0,  778,    0,
  785,  778,    0,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,  778,  785,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,  785,
  425,  941,    0,    0,  785,  426,  427,  941,  428,  429,
  430,    0,    0,    0,  431,    0,  785,  941,  785,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  783,
    0,  941,  941,  941,  941,  941,  941,    0,    0,    0,
  784,    0,    0,    0,    0,  941,  779,    0,    0,    0,
    0,    0,    0,    0,  941,  941,  941,  941,  941,  941,
  941,  941,  941,  814,  941,    0,    0,    0,    0,  941,
  941,    0,  941,  941,  941,    0,    0,    0,  941,    0,
  780,    0,    0,    0,    0,    0,  783,  779,    0,  783,
  779,    0,    0,    0,    0,    0,  783,  784,    0,  783,
  784,    0,    0,  783,    0,  779,    0,  784,  783,  813,
  784,  783,    0,    0,  784,    0,    0,    0,    0,  784,
  814,  780,  784,  814,  780,    0,    0,  783,    0,    0,
  814,    0,    0,  814,    0,    0,    0,  814,  784,  780,
    0,    0,  814,    0,    0,  814,    0,    0,    0,    0,
  813,  783,    0,  813,  783,  783,    0,    0,    0,    0,
    0,  814,  784,    0,  783,  784,  784,    0,  813,    0,
    0,    0,    0,    0,    0,  784,    0,    0,    0,  778,
    0,  783,    0,    0,    0,  814,  807,    0,  814,  814,
    0,    0,  784,    0,    0,    0,    0,    0,  814,    0,
    0,    0,    0,  783,    0,    0,    0,    0,  783,    0,
    0,    0,    0,    0,  784,  814,    0,    0,    0,  784,
  783,    0,  783,    0,    0,    0,    0,  807,    0,    0,
  807,  784,    0,  784,    0,    0,  778,  814,    0,  778,
    0,    0,  814,    0,    0,  807,  778,    0,    0,  778,
  815,    0,    0,  778,  814,    0,  814,    0,  778,    0,
    0,  778,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  778,    0,    0,
    0,    0,  816,    0,    0,    0,    0,    0,    0,    0,
    0,  815,    0,    0,  815,    0,    0,    0,    0,    0,
    0,  778,    0,    0,  778,  778,    0,    0,  779,  815,
    0,    0,    0,    0,  778,    0,    0,    0,    0,    0,
    0,    0,    0,  816,    0,    0,  816,    0,    0,    0,
    0,  778,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  816,  780,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  778,    0,    0,    0,    0,  778,    0,
    0,    0,    0,    0,    0,  779,  804,    0,  779,    0,
  778,  813,  778,    0,    0,  779,    0,    0,  779,    0,
    0,    0,  779,    0,    0,    0,    0,  779,    0,    0,
  779,    0,    0,    0,    0,  808,    0,    0,    0,  780,
    0,    0,  780,    0,    0,    0,  779,  804,    0,  780,
  804,    0,  780,    0,    0,    0,  780,    0,    0,    0,
    0,  780,    0,    0,  780,  804,    0,    0,  813,    0,
  779,  813,    0,  779,  779,    0,  808,    0,  813,  808,
  780,  813,    0,  779,    0,  813,    0,    0,  807,    0,
  813,    0,    0,  813,  808,    0,    0,    0,    0,    0,
  779,    0,    0,    0,  780,    0,    0,  780,  780,  813,
    0,    0,    0,    0,    0,    0,    0,  780,    0,    0,
    0,    0,  779,    0,  817,    0,    0,  779,    0,    0,
    0,    0,    0,  813,  780,    0,  813,  813,    0,  779,
    0,  779,    0,    0,    0,  807,  813,    0,  807,    0,
    0,    0,  815,    0,    0,  807,  780,    0,  807,    0,
    0,  780,  807,  813,    0,  817,    0,  807,  817,    0,
  807,    0,    0,  780,  805,  780,    0,    0,    0,    0,
    0,    0,    0,  817,  816,  813,  807,    0,    0,    0,
  813,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,    0,  813,    0,    0,    0,    0,  815,
  807,    0,  815,  807,  807,  805,    0,    0,  805,  815,
    0,    0,  815,  807,    0,    0,  815,    0,    0,    0,
    0,  815,    0,  805,  815,    0,    0,    0,    0,    0,
  807,  816,    0,    0,  816,    0,    0,    0,    0,    0,
  815,  816,    0,    0,  816,    0,    0,    0,  816,    0,
    0,  809,  807,  816,    0,    0,  816,  807,  804,    0,
    0,    0,    0,    0,  815,    0,    0,  815,  815,  807,
    0,  807,  816,    0,    0,    0,    0,  815,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  808,    0,    0,
    0,    0,  809,    0,  815,  809,  816,    0,    0,  816,
  816,    0,    0,    0,    0,    0,    0,    0,    0,  816,
  809,    0,    0,    0,    0,  804,  815,    0,  804,    0,
    0,  815,    0,    0,    0,  804,  816,    0,  804,    0,
    0,    0,  804,  815,    0,  815,    0,  804,    0,    0,
  804,    0,    0,    0,  808,    0,    0,  808,  816,    0,
    0,    0,    0,  816,  808,    0,  804,  808,    0,    0,
    0,  808,    0,    0,    0,  816,  808,  816,    0,  808,
    0,    0,    0,    0,    0,    0,  817,    0,    0,    0,
  804,    0,    0,  804,  804,  808,    0,    0,    0,    0,
    0,    0,    0,  804,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  808,
  804,    0,  808,  808,    0,    0,    0,    0,    0,    0,
    0,    0,  808,    0,    0,    0,  805,    0,    0,    0,
    0,    0,  804,  817,    0,    0,  817,  804,    0,  808,
    0,    0,    0,  817,    0,    0,  817,    0,    0,  804,
  817,  804,    0,    0,    0,  817,    0,    0,  817,  562,
    0,  808,    0,  563,    0,    0,  808,    0,    0,    0,
    0,    0,    0,    0,  817,    0,    0,    0,  808,    0,
  808,    0,  564,  805,    0,  565,  805,    0,    0,    0,
    0,    0,    0,  805,    0,    0,  805,    0,  817,    0,
  805,  817,  817,    0,    0,  805,    0,    0,  805,    0,
    0,  817,    0,  809,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  805,    0,  566,    0,  817,    0,
    0,    0,  567,  568,  569,    0,  570,  571,  572,    0,
  573,    0,    0,    0,    0,    0,    0,    0,  805,  134,
  817,  805,  805,  134,    0,  817,    0,    0,    0,    0,
  574,  805,  575,    0,    0,    0,    0,  817,    0,  817,
  809,    0,  134,  809,    0,  134,    0,    0,  805,    0,
  809,    0,    0,  809,    0,    0,    0,  809,    0,    0,
    0,    0,  809,  576,    0,  809,    0,    0,    0,    0,
  805,    0,    0,    0,    0,  805,    0,    0,    0,    0,
  577,  809,    0,    0,    0,    0,  134,  805,    0,  805,
    0,    0,  134,  134,  134,    0,  134,  134,  134,    0,
  134,    0,  578,    0,    0,  809,    0,    0,  809,  809,
    0,    0,    0,    0,    0,    0,    0,    0,  809,    0,
  134,    0,  134,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  809,    0,  579,  580,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  134,  581,    0,    0,  809,    0,    0,
    0,    0,  809,    0,    0,    0,    0,    0,    0,    0,
  134,    0,    0,    0,  809,    0,  809,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  134,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  134,  134,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  134,
};
static short yycheck[] = {                                       9,
   16,   52,   12,   61,    4,    4,   16,  101,   18,    0,
   60,  236,  130,   60,  488,  331,  696,  679,  339,  339,
  727,   53,   59,  335,   82,  120,  121,  120,  121,  531,
 1167,  522,   64,  733,  525,   86,  310,  394,  791,  719,
  531,   51,   52,   53,   54,   55,   67,  236,   91,  471,
   60,   61,   62,  996,   64,   87,   66,   67,  308,   69,
   70,  111,   94,  113,   80,  620,  356,  622,   78,   90,
   80,   81,   82,   89,   84,   85,   86,   87,  267,   89,
   90,   91,   92,   54,   94,   70,  882,  989,  728,  665,
   85,  134,  989,   65,  996,   66, 1210,  997,  989,  996,
  132,  111,  112,  113,    0,  996,  655,   92, 1008,    0,
  989,  344,  244,  726,  208,  125,   88,  996,    0,  674,
  130,  253,  132,  133,  134,  614,    0, 1027,  924,   45,
  125,    0,  169,  682,  659,  822,  287,    0,    0,    0,
  753,  112,    0,  236,  303,   41,  371,  723,   44,  762,
   41,   44,  236,   44,    0,    0,   40,  194,   41,  607,
 1044,   44,  133,   59,   44,    0,   44,   41,   59,  287,
   44,    0,   42,   40,   40, 1122,  695,   59,   41,   44,
   44,   44,  371,   41,   41,   59,   45,   42,  238,  501,
   59,   41,  508,  509,  510,  511,   59,   59,   59,   40,
  294,   59,  815,  264,   40,   45,   41,   44,  218,   44,
  531,  531,   41,   59,   59,   44,  226,   58, 1314,   40,
  271,   59,   44,  730,   59,  699,   44,   40,  238,   44,
   59,   41,  242,   44,  244,  245,  288,  287,  251,  274,
 1411,  251,   45,  253,  288,  557,    0, 1343,   45, 1045,
   63, 1208,   58,   61,  386,   40,  264,    0,  491,  309,
   45,  271, 1115,   40, 1117,  275,  316,    0,  318, 1126,
  320,  242,  562,  323,   40,   12,  394,  287,  371,    0,
  251,  321,    0,  262,  534,  260,   63,  371,  779, 1258,
  260, 1353,  400, 1236,  258,  332,    0,  334,   41,  309,
  403,  352,  857,  858,  275,   59,  316,  883,  318,  341,
  320,  348,   44,  323,  308,  309,   59,   63,  273,  349,
 1021,  342,  320,  268,  263, 1225,   59,  418,  358,    0,
  941,  341,  342,  583, 1236,  435,  439,   41,   59, 1236,
  350,   59,  352,  307,  394, 1236,    0,   59,  199,  962,
  875,    0,  280,  385,  445,   59,  356, 1236,  286,  140,
   91,  461,  887,  888,  919,    0, 1039,  353,  400,  265,
 1123,    0,  382,   41,  313,  385,  386,  799,    0,  350,
   41,  118,    0,  393,  394,  324,  341,  397,   59,  402,
  400,  274,  402,  379, 1007,  405,   59,  437, 1255,  264,
  461,  285,   45,  401,  412,   59,    0,  303,  387,  461,
   59,  382,   41,  258, 1528,   41,  641,  461,   44, 1376,
   63,  366,  393,  319,   59,  435,  397,  310,  319,  264,
   59,  402,  467,  988,  279,  321,    0,   59,    0,  290,
    0,   59,  297,  418,  442,  341, 1415,   41,  418,  278,
  341,  280,  641,  404,  549,  419,  301,  321,  452,  288,
  470,  319,  307, 1634, 1317,   59,    0, 1353, 1368,  506,
  421,   41, 1372,    0, 1374,  332,  665,   41, 1231,  341,
  341,  531,  456,  341,  321,  330,  321, 1187,    0,  428,
  305,  741,    0,  301,  744,   59,  403,   59, 1160,   59,
  310,  319, 1449,  269,  349, 1167,  384, 1180,  758, 1409,
  384,  700, 1626,  358,   41,  552,  297,   44,  650,  364,
  530,  531, 1659,  880, 1408,   59,  536, 1589,  395,  412,
  388,  388,   59,   41,  723,  785,   44,  382,  388,  397,
  397, 1242, 1049,  459,    0,  835,  277,   59,  641, 1645,
  379,   59,  826, 1072,  662,  384,  606,  641, 1169,  441,
  453, 1248,  562, 1700, 1678, 1054,  603,  604,  437,  720,
  486, 1211,  401,  455,  419,  455,  319,  461,  550,  408,
  813,  740, 1482,  441,  467,   41,  329, 1487,   44,  332,
 1038,  461,  642,  455,  455,  461,  606,  455,  341,  332,
  459, 1715,  720,   59,  258,  615,  461,  503, 1133,  455,
  705,  329,  503,  442,  332,  615,  615,    0,  453,  459,
  461,  461,    0,  461,    0,  461,  461,  448,  502,  432,
  662, 1427,  642, 1533, 1247,  645,  258,  459, 1193,  502,
  650,  736,  737,  397,  455,  388,  459,  432,    0,  659,
 1550,  319,  662,  307,  397,  461,  459,  667, 1213,    0,
  976,   44,  459, 1363,  397,   41,   42,   43,   44,   45,
  720,   47,  667,  502,  459,  725,   59,  528,  688,  397,
  730,   59,  459,   59,  388,  307,  696,  441,  698,  410,
    0,  701,  262,  397,  883,  732,  733,  368,  441,  411,
  750,  455,  264, 1589,  714,  437,  716,   59,  441,  719,
  720, 1654,  455,  459,  349,  725, 1389,  727,   59,  368,
  730, 1422,  455,  441, 1040,  319, 1275,  388,  778,  779,
  264,  349,  864, 1633, 1407,  867,  397,  455,  332, 1315,
  750, 1352, 1642,  714,  994,  716,  597,  341,  411,   59,
  273,  278, 1654,  280,    0,  319, 1006, 1654,   41,  388,
 1353,  288,  880, 1654,  989,  419,  284,  739,  778,  779,
  278,  996,  280,  461,  356, 1654, 1026,  341, 1052,  277,
  288,  341, 1682,  406,  784,    0,  412,    0,   41,  432,
    0,   41,    0,  349,  388,   41,  796,  419,   44, 1354,
  989, 1463,  398,  397,  814,  777, 1353,  996,   59,   42,
  302,  378, 1234,   59,  824, 1522,  459,  387, 1240, 1321,
    0,  302,  278,  329,  280,  868,  836,   12,    0,   44,
  880,  343,  288,  865,  457,  835,  398,  809,  810,  811,
    0,  437,    0,  814,   59,  866,   59,  441,  346,   59,
    0,   59,  379,  824,  864,  865,  866,  867,  868,  869,
    0,  455, 1136,  357,  398,  875,  378,  344,  339,   41,
  880,  379,   44,  287,  401,  471,    0,  887,  888,   59,
 1587,  408,  449,   41,  869,  259,   44,   59,    0,  265,
  308,  309,  369,  401,  406,  302,  270,  409,    0,   59,
  408,   59, 1364,   61,  280,  410,  462,  463, 1588,   59,
  286,  493,    0,  425, 1203,  442,  301,   41,   41,   59,
   44,  326,  440,  379,    0,  388,    0,  303,  262, 1024,
    0,  941,  339,  118,  442,   59,  430, 1126,  444,  451,
 1252,  319,   41,  319,    0,  401,  329,   59, 1410,  332,
  427,  443,  408,  765,  264,  461, 1150,   59,  659, 1153,
 1249,  344,  443,  341,    0,  341,   40,   41,  353,   42,
   44,   59,  443, 1223,   47,  502, 1450,  329,  441,  262,
  332,    0,    0,   59,    0,   59,  442,   61,  466,   59,
  468, 1465, 1042,  288,  502,    0, 1589, 1659, 1489, 1049,
 1321, 1321,  343,   59, 1254,   41,  301, 1017,   44,  262,
  388, 1236,  262,  257,  397,    0,    0,   91, 1018, 1018,
   41, 1031,    0,   59,  261,   61,  431,  260, 1121, 1122,
 1123, 1586, 1042,  438,  452,  257,  443,  378, 1700, 1049,
   59,   59, 1589,   59, 1360,  397,  502, 1236,  462,  463,
 1168,  264, 1607,  387,   59, 1610,   41,   41,  441,   44,
   44, 1033,   44,  441,  406,  406, 1255, 1356,  409,   16,
  453,  304,  455,  319,   59,   59,   61,    0, 1552, 1051,
   41,   59,  494,   44,  425, 1057,  332,    0,  398,  441,
  459,  335,  504, 1638, 1189,  341, 1191,  403,   59,   44,
  258,  453,  436,  455,  387, 1115,   91, 1117,  459,  319,
  451,   93,    0,  335,  329,  457,  274,  332,  278,  259,
  280, 1121, 1122, 1133,  264,  262, 1315,  503, 1378,  344,
  270,  341,    0,   80,  387,  343,   59,  387, 1683,  262,
  446, 1335,  388,  301,  323, 1339,   59,  319,   93,  307,
 1187,  397,  310,   41,  278, 1250,  280,  329, 1168, 1169,
  332,  341,  264,  262,  288,    0,  278, 1417,  280,  341,
  378,   59,  344,   41,  875,    0,   44, 1149,  388,  444,
 1725,  284,  397,  302,  258,  398,  887,  397, 1000, 1001,
  278,   59,  280,  343,  264,  441,  461,  259,  406,  402,
  274,  409,  508,  277,  278,  323,  280,  280,  270,  455,
   40,  285, 1757,  286,  288, 1689,  388,  425,  260,  379,
  339, 1776,  258,  258,   59,  397,  441,  301, 1265, 1229,
 1229,  441, 1269,  307,   59, 1353,  310, 1049,  453,    0,
  455,  277,  278,  451,  280,  264,  264,    0,  264,  285,
  387, 1540,  288, 1371,  412,  379,  406, 1546,  398,  409,
  218,  419,  304,  319,  387,  301, 1460,  343,  226,  441,
 1509,  307,  307,  258,  384,  425,  332,  401, 1353,  437,
   41,  453,  385,   44,  408,  341,  264,  260,  387,  274,
  379,    0,  277,  278,  278,  280,  280, 1547,   59,  436,
  285,  451,  378,  288,  288,  379,   59, 1317,  329,  467,
   44,  332,  401,  471,  319,  278,  301,  301,  442,  408,
 1370, 1371,  307, 1370,   44,  310, 1363,  401,  398,    0,
  406,  304,  388,  409,  408, 1661,  341,  440,  412,   59,
    0,  397, 1352,  379,    0,  419,    0,  308,  309,  425,
   59,  264, 1615,  442,  457,  384, 1449,  320, 1636,   93,
 1370, 1371,  258,  437, 1375,  401,  262,  388,  442, 1379,
   41,  267,  408,   44,  265,  451,  397,  302,   44,  398,
  398,  417,  398,  419,  419,  441, 1397,   41,   59, 1484,
   61, 1509,  279,  467,  379,  379,  386,  471, 1661,   59,
  296,  437,  437,   59,  322,   59,  442, 1685, 1379, 1581,
  384,    0,  303, 1585,  339, 1741,  401,  401,  336, 1115,
  398, 1117,  378,  408,  408,    0,   44,  412,  260, 1654,
    0,  319, 1490,  459,  419,  471,  471, 1531,  401,    0,
 1766,  329,  360,  330,  332, 1708,    0,  338, 1726, 1449,
  411,  319,  437,  341,  322,   44, 1456,  442,  442, 1509,
  486,  329,    0,  419,  332, 1654,  384,    0,  336, 1641,
   59, 1589,  304,  341, 1713,   93,  344,  364, 1741,  442,
 1490, 1532,  467,  408,   59,  398,  471,   41,  418,   59,
   44,  452,  360,  418,  384,  382,  414, 1615,   59, 1509,
  388,    0, 1493, 1766,    0,   59, 1745, 1746,   41,  397,
  466,   44, 1522,    0, 1589,  445,  384, 1527,  443, 1556,
  388,   59, 1532, 1560,    0,  837,   59,  839,   61,  397,
    0, 1770,  428, 1627,  272, 1551,  432,  433,  434,  437,
 1615, 1551,   41, 1661, 1783,   44,  414, 1690, 1691,    0,
  260,  289,   41,  441,   41,   44, 1527, 1729,  319,    0,
   59, 1730, 1731,   59,  258,  453,  319,  261,  260, 1601,
  279,   41,   59,  441,   44,  343,  329, 1587, 1588,  332,
  341,  265,  338,   59, 1727,  453, 1661,  258,  341,   59,
 1708, 1601,  301,  461,  304, 1713,  334,   42,   43,  419,
   45, 1646,   47, 1648,   93, 1615,  277,  278,   59,  280,
  378, 1669,  304,  307,  285,    0, 1785,  288,   59,  303,
 1636,  330,  344, 1741,  495,  363, 1636, 1745, 1746,   41,
  301,    0,   44, 1708, 1777,  388,  307, 1653,  406,  274,
  349,  409, 1687, 1653,  397,    0,  319,  369, 1766,  358,
   40, 1661, 1770,   42,   43,  364,   45,  425,   47, 1669,
 1670,  406,    0, 1713,   41, 1783, 1741,   44,  425, 1685,
  462, 1671, 1671,  382,   59, 1685, 1692,  264,  332,    0,
  425,   93, 1692,  451,   42,   43, 1737,   45,  441,   47,
   59, 1766,  430,  317,  318, 1745, 1746,   41, 1708,    0,
   44,  317,   46, 1713,   59,  427,  319,  338,  379,  260,
 1726,  295,  494,  495, 1759,   59, 1726,   61, 1734,  338,
 1770,   59,  306,   44, 1734,  258,  622, 1737,   45,  313,
  401, 1741,  337, 1783,  388, 1745, 1746,  408,   59,  461,
   41,  462,  463,  397,  375,  278,   63,  280,  419,    0,
    0,  302,  285, 1763,  338,  288, 1766,  388,   59, 1769,
 1770,   41,  329,  314,   44,  319,  437,    0,  301,  665,
    0,  442,  668, 1783,  307,  329, 1786,  673,  332,    0,
    0,    0,  678,  679,  453,    0,  403,  341,  339,    0,
  344,  687,  262,  689,  690,  691,  692,  693,  341,  695,
  471,   41,   42,   43,  700,   45,   40,   47,   59,   59,
   40,   41,   42,   43,   44,   45,   46,   47,  321,  403,
   42,   43,  403,   45,  319,   47,   59,  723,  279,   59,
   60,   61,   62,  729,  388,  280,  403,    0,   59,   59,
   59,  286,  329,  397,   59,  332,  379,  403,   59,  319,
  301,  435,  322,    0,  435,  439,    0,  408,  384,  329,
    0,   91,  332,   93,    0,   40,  336,  418,  401,  450,
   45,  341,  439,  384,  344,  408,  321,  444,   41,  330,
  507,   44,   41,  450,  338,   44,  419,  441,   63,  437,
  360,  280,  443,  362,  461,  435,   59,  286,  349,  453,
   41,  388,  469,   44,  437,   41,  357,  358,   44,  442,
  397,  279,   59,  364,  384,   59,  451,  387,  388,   59,
  264,  375,  280,   59,  443,    0,  322,  397,  286,  358,
  357,  382,  321,  301,  388,   41,   42,   43,  471,   45,
  336,   47,  288,  394,  414,  396,  842,  843,  844,  412,
    0,  847,  848,  849,  312,  851,  852,  853,  854,  855,
  856,  305,  330,    0,  360,  418,  436,  394,  419,  396,
    0,  441,  392,  393,   41,   42,   43,  470,   45,  296,
   47,  349,   44,  453,   59,  455,   41,  883,  384,   44,
  358,   61,  419,  444,  890,  891,  364,  893,  373,  895,
  287,  322,  898,  899,  900,  901,  902,  903,  329,   59,
    0,  332,  264,   41,  382,  336,   44,  444,  414,  915,
   40,  917,   59,  344,  322,   41,   40,  322,   44,   59,
   41,  462,  262,   44,  264,  265,  353,  462,  336,  360,
  280,  336,  272,  462,   41,   41,  286,   44,   44,  344,
  280,   41,   42,   43,   44,   45,  286,   47,  280,  289,
  462,  463,  360,  384,  286,  360,   44,  437,  321,   59,
   60,   61,   62,  303,   41,   44,  397,   44,  308,  309,
  403,   91,  312,   42,   43,   44,   45,   40,   47,  319,
  312,  321,  322,  414,   41,  325,   40,   44,  419,  329,
   40,   40,  332,   93,  334,   40,  336,  458,  459,  460,
   41,  341,  342,   44,  344,  278,  414,  280,   40,  414,
  441,  296,  285,   46,   41,  288,    0,   44,   40,   40,
  360,  361,  453,  363,  455,  486,  487,   41,  301,   40,
   44,  458,  459,  460,   40,  462,  463,  377,   40,  379,
  380,  381,   41,   41,  384,   44,   44,  387,  388,   40,
   40,  321,   44,  429,  481,  482,   44,  397,   61,  486,
  487,   41,   42,   43,  280,   45, 1072,   47,  353,   46,
  286,  411,   40,  264,  414,   59,  383,   44, 1084, 1085,
   44, 1087, 1088,  349,  278,  278,  301,  398,  264, 1095,
  430,  437,    0,  471,  398,  459,  436,  448,  277,    0,
  322,  441,  400,  280,   40,  350,  379,  448,   40,  286,
   91,   40,  452,  453,  336,  455,   40,  319,  264,  384,
 1126,  461,  344,   41,   42,   43,  412,   45,  401,   47,
   44,  390,  437,   41, 1140,  408,   44,   40,  360,   40,
   41,  383, 1148,   44, 1150,   42,   43, 1153,   45, 1155,
   47,   59,   40, 1159, 1160, 1161, 1162,  465,   59,  318,
   61, 1167,  262,  503,  264,  265,   40,  321,  321,  442,
  452,   41,  272,  458,  459,  460,  453,  462,  463,  264,
  280,  449,   40,  455,  459,  277,  286,  274,   40,  289,
   91,  301,  414,  345,    0,  285,  481,  482,  278,  387,
 1206,  486,  487,  303,  345,  387,  387,  419,  308,  309,
  417,  280,  312,  419,   41,   42,   43,  286,   45,  319,
   47,  321,  322,  301,   40,  325,  357,   40,  418,  329,
  274,  418,  332,   40,  334,   41,  336,  386,   44,   44,
  368,  341,  342,  847,  344,  849,   44,  851,  852, 1255,
  854,  855,  856,   59,  321,  264,   41,   44,   44,  321,
  360,  361,  280,  363,   42,   43, 1272,   45,   40,   47,
   41,   42,   43,  329,   45, 1281,   47,  377,    0,  379,
  380,  381, 1288,   44,  384,  503,   44,  387,  388,  506,
   41,   42,   43,  266,   45,  279,   47,  397,   40,  300,
  280,  502,   40,  264,    0,  423,  286,   41,   58, 1315,
   41,  411,   41,   41,  414,  264,  455,  301,  441,   41,
   42,   43,   44,   45,  395,   47,  493,  310,  310, 1335,
  430,   41,   41, 1339,   40,   40,  436,   59,   60,   61,
   62,  441,   40,  353,   44,   41,  330,   40,   44,   40,
   40,   40,  452,  453,  262,  455,   40,  258,   40,   40,
   40,  461,  280,   59,   40,  349,   40,   40,  286,   40,
   40,   93,  262,  274,  358,  328,  277,  278,  436,  280,
  364,    0,  309,  280,  285,   42,   43,  288,   45,  286,
   47,   41,  389,  389,  389,  274,  344,  332,  382,  423,
  301,    0,   59,  503,  391,  447,  307,  392,  271,  310,
  288,  319,   42,   43,  322,   45,  387,   47,  387,  408,
   41,  329,   41,   41,  332,   44,    0,  387,  336, 1435,
 1436,  387,  387,  341,   42,   43,  344,   45,  379,   47,
   59,  664,   41,  666,  387,   44,  669,  670,  671,  278,
   40,   59,  360,  280, 1460,   41,   41, 1463,   41,  286,
   59,  418,  437,  386,   41,  309,  436,   41,   42,   43,
   44,   45,  397,   47,  338,   41,  384,  280,  379,  387,
  388,  443,  339,  302,  262,   59,   60,   61,   62,  397,
 1496,  443,  339,  302,   41,  321,  372,   41,   42,   43,
  401,   45,  280,   47,  353,  386,  414,  408,  286,  280,
  372,  412,   41,   41,   41,  286,  322,   41,  419,   93,
   41,   41,   41,  329,   41,   41,  332,   41,  436,  280,
  336,   41,   41,  441,   41,  286,  437,   41,  344,   41,
  262,  442,  264,  265,   41,  453,   46,  455,  280, 1555,
  272,   44,   40,   59,  360,   40,   59, 1563,  341,   41,
   42,   43,  341,   45,  286,   47,  467,  289,   59,   59,
  471,   41,   59,   59,  309,   59,   59,  353,  384,  345,
   41,  303,  278,    0,  280,  369,  308,  309,   40,  285,
  312,  397,  288,  341, 1600,  384,  452,  319, 1604,  321,
  322,   44,   59,  325,  384,  301,   41,  329,  414,  384,
  332,  344,  334,   41,  336,   41,  411,   44,  436,  341,
  342,   44,  344,  280,   41,   42,   43,   44,   45,  286,
   47,  443,  262,  305,  302,  441,  388,   40,  360,  361,
   59,  363,   59,   60,   61,   62,  294,  453,   44,  455,
  280,  305,  308, 1659,  464,  377,  286,  379,  380,  381,
   44,   41,  384,    0,  384,  387,  388,  384,    0,  338,
   40,  264,  280,  461,  278,  397,   93,  264,  286,  278,
  321,  280,  459,  379,  264,  412,  390,  452,  262,  411,
  264,  265,  414,   40, 1700,   41,   42,   43,  272,   45,
  319,   47,  471,  322,  310,  401,  459,  271,  430,  264,
  329,  437,  408,  332,  436,  289,  344,  336,  436,  441,
  344,  344,  341,  459,  344,  344,  271,   59,  309,  303,
  452,  453,   41,  455,  308,  309,  280,  461,  312,  461,
   40,  360,  286,   40,  309,  319,  442,  321,  322,   42,
   43,  325,   45,   59,   47,  329,    0,  309,  332,   41,
  334,   59,  336,  305,   41,  384,   59,  341,  342,  388,
  344,   42,   43,   59,   45,   44,   47,   41,  397,    0,
  379,  503,    0,    0,    0, 1118,  360,  361,   12,  363,
  253,  246,  245, 1054, 1052,  414,  650,   41,  280,   43,
   44,   45,  401,  377,  286,  379,  380,  381,  610,  408,
  384,  827,   84,  387,  388,   59,   60,   61,   62, 1049,
 1041, 1398,  441,  397,   41,  306,  301,   44,  336,  979,
  292, 1031,  557,  725,  453, 1598, 1376,  411, 1042,    0,
  414,  318,   59,  442,  318,  262, 1378,  264,  265,   93,
  760, 1676, 1680,  536,  750,  272,  430,   41,   42,   43,
 1717,   45,  436,   47, 1679, 1204, 1769,  441, 1620,   41,
   42,   43,  289,   45, 1714,   47,  371, 1636,  452,  453,
   41,  455,   43,   44,   45, 1182,  303,  461,  964,  323,
 1357,  308,  309,  311,    0,  312,  538,  304,   59,   60,
   61,   62,  319, 1238,  321,  322,  806,  798,  325,  596,
   13,  405,  329,  279,  473,  332,  477,  334,  714,  336,
  123,  729,  941, 1480,  341,  342,  258,  344,  701,  503,
  700, 1734,   93, 1589,  280,   41, 1692, 1786,   44,  488,
  286, 1086,  658,  360,  361,  884,  363,  279,   41,   42,
   43,   -1,   45,   59,   47,  370, 1155,   -1,   -1,   -1,
  377,   -1,  379,  380,  381,   -1,    0,  384,   -1,  301,
  387,  388,   -1,   -1,   -1,  307,  355,   -1,   -1,   -1,
  397,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,
   -1,   -1,   -1,  286,  411,   -1,    0,  414,  330,   -1,
   41,   42,   43,   -1,   45,   -1,   47,   41,   -1,  280,
   44,   -1,   -1,  430,   -1,  286,   -1,  349,  262,  436,
  264,  265,   -1,   -1,  441,   59,  358,   -1,  272,   -1,
   -1,   -1,  364,   -1,   -1,  452,  453,   41,  455,   43,
   44,   45,   -1,   -1,  461,  289,   -1,   -1,   -1,   -1,
  382,   -1,   -1,   -1,   -1,   59,   60,   61,   62,  303,
   -1,  278,   -1,  280,  308,  309,   -1,   -1,  312,   -1,
   -1,  288,   -1,   -1,   -1,  319,   -1,  321,  322,   -1,
  459,  325,  461,   -1,  301,  329,  503,  419,  332,   93,
  334,   -1,  336,  472,   -1,   -1,  280,  341,  342,   -1,
  344,  262,  286,  264,  265,   -1,   -1,   -1,  280,   -1,
   -1,  272,  491,   -1,  286,   -1,  360,  361,   -1,  363,
  499,  500,  501,   -1,   -1,   41,   42,   43,  289,   45,
   -1,   47,   -1,  377,   -1,  379,  380,  381,   -1,   -1,
  384,   -1,  303,  387,  388,   -1,   -1,  308,  309,   -1,
   -1,  312,   -1,  397,   -1,   -1,   -1,   -1,  319,   -1,
  321,  322,  379,   -1,  325,   -1,   -1,  411,  329,    0,
  414,  332,  278,  334,  280,  336,   -1,   -1,   -1,   -1,
  341,  342,  288,  344,  401,   -1,  430,  280,   -1,   -1,
   -1,  408,  436,  286,   -1,   -1,   -1,  441,   -1,  360,
  361,   -1,  363,   -1,   -1,   -1,   -1,   -1,  452,  453,
   41,  455,   43,   44,   45,   -1,  377,  461,  379,  380,
  381,   -1,   -1,  384,   -1,  442,  387,  388,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  397,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  278,  286,  280,   -1,   -1,   -1,
  411,   -1,   -1,  414,   -1,   -1,   -1,   -1,  262,  503,
  264,  265,   93,   -1,   -1,   -1,   -1,   -1,  272,  430,
   41,   42,   43,  379,   45,  436,   47,   -1,   -1,   -1,
  441,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  452,  453,   -1,  455,  401,   -1,   -1,   -1,  303,
  461,   -1,  408,   -1,  308,  309,   -1,    0,  312,   -1,
   -1,   -1,    0,   -1,   -1,  319,   -1,  321,  322,    0,
   -1,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
  334,   -1,  336,   -1,   -1,   -1,  442,  341,  342,   -1,
  344,   -1,  503,   -1,   -1,  379,   -1,   -1,   41,   -1,
   -1,   44,   -1,    0,   -1,   -1,  360,  361,   -1,  363,
   41,   -1,   -1,   44,  280,   -1,   59,  401,   61,   -1,
  286,   59,   -1,  377,  408,  379,  380,  381,   59,   -1,
  384,   -1,   -1,  387,  388,  260,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  397,   41,   -1,   -1,   44,   91,   -1,
   -1,   -1,   -1,   -1,   45,   -1,   -1,  411,  442,   -1,
  414,   -1,   59,   -1,   61,    0,  473,  474,  475,  476,
  477,  478,  479,  480,   -1,   -1,  430,  302,   -1,   -1,
   -1,  262,  436,  264,  265,   -1,   -1,  441,   -1,  314,
   -1,  272,  260,   -1,   91,   -1,   -1,   -1,  452,  453,
   -1,  455,   -1,  328,   -1,   -1,   41,  461,  289,   44,
   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   59,   -1,   61,  308,  309,   -1,
   -1,  312,   -1,   -1,  302,   -1,   -1,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,   -1,  314,   -1,  329,  503,
   -1,  332,   -1,  334,   -1,  336,   91,   -1,   -1,  280,
  341,  342,   -1,  344,   -1,  286,   -1,   -1,   -1,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
  361,   -1,  363,  408,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  418,   -1,   -1,  377,   -1,  379,  380,
  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  397,   -1,  443,   -1,
  258,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   40,
  411,  274,   43,  414,   45,  278,  461,  280,   -1,   -1,
  408,  279,  285,   -1,   -1,  288,   -1,   58,  279,  430,
  418,   -1,   63,   -1,   -1,  436,   -1,   -1,  301,   -1,
  441,  258,   -1,  301,  307,   -1,   -1,  310,   -1,  307,
  301,  452,  453,   -1,  455,  443,  307,  274,   -1,   -1,
  461,  278,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,  330,  461,   -1,   -1,   -1,   -1,   -1,  330,
   -1,   -1,   -1,   -1,  301,  296,   -1,   -1,   -1,   -1,
  307,  349,   -1,  310,   40,   -1,   -1,   43,  349,   45,
  358,   -1,  503,  258,   -1,   -1,  364,  358,   -1,   -1,
   -1,   -1,   58,  364,   -1,   -1,  379,   63,   -1,  274,
   -1,   -1,   -1,  278,  382,  280,   -1,   -1,   -1,   -1,
  285,  382,   -1,  288,   -1,   -1,   -1,   -1,  401,   -1,
   -1,   -1,  353,   -1,   -1,  408,  301,   -1,   -1,  412,
   -1,   -1,  307,   -1,   -1,  310,  419,   -1,   -1,   -1,
   -1,  419,  379, 1273, 1274,   -1,   -1,   -1,  419, 1279,
 1280,   -1, 1282, 1283,  437,   -1,   -1,   -1,   -1,  442,
 1290, 1291, 1292, 1293,  401, 1295, 1296, 1297, 1298, 1299,
 1300,  408,   -1,   -1,   -1,  412,   -1,   -1,  295,   -1,
   -1,   -1,  419,   -1,  467,   -1,   -1,   -1,  471,  306,
   -1,   -1,   -1,   -1,   -1,   -1,  313,   -1,   -1,   -1,
  437,   -1,   -1,   -1,  379,  442,   -1,   -1,   -1,  326,
   -1,   -1,   -1,  444,   -1,   -1,  267,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,  276,  401,  458,  459,  460,
  467,  462,  463,  408,  471,   -1,   -1,  412,   -1,  290,
   -1,   -1,  293,   -1,  419,  296,  297,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
   41,   -1,  437,   44,  315,  496,  497,  442,   -1,   -1,
  387,   -1,   -1,  324,   -1,   -1,  327,   -1,   59,   -1,
   -1,   -1,    0,   -1,   -1,   -1,  403,   -1,   -1,   -1,
   -1,   -1,  467,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,  267,   -1,  420,  355,  356,   -1,   -1,   -1,   40,
  276,   42,   43,   -1,   45,   -1,  367,   -1,  435,   -1,
  371,   -1,  439,   41,  290,  442,   44,   58,  379,   -1,
  296,  297,   63,  450,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,  306,   -1,   -1,   -1,   -1,  315,
   -1,  313,  469,   -1,   -1,  317,  318,   -1,  324,   -1,
   -1,  327,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,  422,   -1,   -1,   -1,   -1,  338,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,    0,   -1,   -1,   -1,  355,
  356,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  367,   -1,   -1,   -1,  371,   -1,  458,  459,  460,
  461,  462,  463,  379,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,  403,   -1,   -1,   59,  496,  497,   -1,  499,  500,
  501,   -1,  418,   -1,  505,   -1,  422,   -1,  420,   -1,
   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,  439,  444,   -1,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  450,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  469,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,  260,
  496,  497,   -1,  499,  500,  501,  267,   -1,  319,  505,
   -1,  322,   -1,   -1,   40,  276,   42,   43,  329,   45,
   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,  290,
  341,   -1,   58,  344,   -1,  296,  297,   63,   -1,   -1,
   -1,   -1,   -1,  304,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,
   -1,  319,   -1,  384,  322,   -1,  387,  388,   -1,   -1,
   -1,  329,   -1,   -1,  332,   -1,  397,   -1,  336,   -1,
   -1,   -1,  353,  341,  355,  356,  344,   -1,   -1,   40,
   -1,   -1,   43,  414,   45,   -1,  367,   -1,   -1,   -1,
  371,  261,  360,  258,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,   -1,   -1,  283,  279,   -1,  384,   -1,   -1,  387,
  388,  291,  453,   -1,  455,   -1,   -1,   -1,  298,  397,
  300,   -1,  302,   -1,   -1,   -1,  301,  307,   -1,   -1,
   -1,   -1,  307,   -1,  314,   -1,  414,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,  328,   -1,
  273,   -1,   -1,  444,  277,  330,   -1,   -1,  436,  339,
   -1,   -1,  285,  441,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,  296,  349,  453,  299,  455,   -1,   -1,
   -1,  472,  357,  358,   -1,   -1,   -1,   -1,   -1,  364,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,  260,  496,  497,  382,  499,  500,
  501,  267,   -1,   -1,  505,   -1,   -1,  340,   -1,   -1,
  276,   -1,   -1,  346,  347,  348,  406,  350,  351,  352,
   -1,  354,   -1,  413,  290,   -1,  416,   -1,  418,  419,
  296,  297,   -1,   -1,  419,   -1,   -1,   -1,  304,   40,
   -1,  374,   43,  376,   45,  430,   42,   43,   -1,   45,
   -1,   47,   -1,  443,   -1,   -1,   -1,   58,  324,   -1,
   -1,  327,   63,   -1,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,  353,   -1,  355,
  356,  424,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,  367,  492,   -1,   -1,  371,   -1,   -1,   -1,  290,
   -1,   -1,   -1,  446,   -1,  296,  297,  507,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  461,   -1,
   -1,   -1,   -1,   -1,  315,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,  324,   63,   -1,  327,   -1,  481,  482,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,   -1,  498,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,  444,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,
  371,   -1,  458,  459,  460,  461,  462,  463,  379,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,  422,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,  263,  444,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,  272,  458,  459,  460,
  461,  462,  463,   -1,  280,   -1,   -1,   -1,   -1,  290,
  286,  472,   -1,  289,   -1,  296,  297,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,  324,  505,   -1,  327,   -1,   -1,  325,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  334,   -1,
   -1,  260,   -1,   -1,  263,   -1,  342,   -1,  267,   -1,
   -1,   -1,   -1,   -1,  355,  356,   40,  276,   -1,   43,
   -1,   45,   -1,   -1,   -1,  361,  367,  363,   -1,   -1,
  371,  290,   -1,   -1,   58,   -1,   -1,  296,  297,   63,
   -1,  377,   -1,  379,  380,  381,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,  430,   -1,  355,  356,   -1,   -1,
   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,  367,   -1,
   -1,   -1,  371,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,  426,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,  273,   -1,   -1,   -1,
  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,  292,   -1,   -1,   -1,  296,
   -1,   -1,  299,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,
  499,  500,  501,  267,   -1,   -1,  505,   -1,   -1,   -1,
   -1,   40,  276,  340,   43,   -1,   45,   -1,   -1,  346,
  347,  348,   -1,  350,  351,  352,  290,  354,   -1,   58,
   -1,   -1,  296,  297,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   -1,   -1,  374,   -1,  376,
  280,   -1,   -1,   -1,  461,   -1,  286,   -1,   -1,  289,
  324,   -1,   -1,  327,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,  485,   -1,
  407,  488,  489,  490,  491,  412,  493,   -1,   -1,  496,
  497,  355,  356,   40,   -1,  325,   43,  424,   45,   -1,
   -1,   -1,   -1,  367,  334,   -1,   -1,  371,   -1,  273,
   -1,   58,  342,  277,   -1,   -1,   63,   -1,   -1,  446,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  292,   -1,
   -1,  361,  296,  363,   -1,  299,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  377,   -1,  379,
  380,  381,   -1,   -1,  481,  482,   -1,   -1,   -1,   -1,
   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,  498,   -1,   -1,   -1,   -1,  340,   -1,   -1,   -1,
  444,   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,
  354,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
  430,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
  374,   -1,  376,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,  260,  496,  497,  263,  499,  500,  501,  267,   -1,
   -1,  505,   -1,  407,   -1,   -1,   40,  276,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  424,  290,   -1,   -1,   58,   -1,   -1,  296,  297,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,  263,  481,  482,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  355,  356,   40,  276,
   -1,   43,   -1,   45,  498,   -1,   -1,   -1,  367,   -1,
   -1,    0,  371,  290,   -1,   -1,   58,   -1,   -1,  296,
  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,
   59,   -1,   -1,   -1,   -1,  434,   -1,   -1,  355,  356,
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
   -1,  355,  356,   40,  276,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  290,   -1,
   -1,   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
  319,   -1,   -1,  322,  458,  459,  460,  461,  462,  463,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,  472,   -1,
   -1,   -1,  341,   -1,   -1,  344,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,  360,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,  384,   -1,   -1,   -1,  388,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,  414,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
  271,   -1,  441,  260,  496,  497,  263,  499,  500,  501,
  267,   -1,   -1,  505,  453,   -1,   -1,   -1,   40,  276,
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  302,   -1,  290,   -1,   -1,   58,   -1,  309,  296,
  297,   63,  313,  314,   -1,  316,   -1,   -1,  319,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  333,   -1,   -1,  461,   -1,  324,  339,   -1,
  327,   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,
  476,  477,  478,  479,  480,  481,  482,  483,  484,  485,
   -1,   -1,  488,  489,  490,  491,   -1,  493,  355,  356,
  496,  497,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  399,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,  271,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  426,
   -1,  428,  443,   -1,   -1,   -1,   -1,  434,   -1,   -1,
  302,  452,   -1,  454,   -1,   -1,   -1,  444,   -1,   -1,
  461,  313,  314,   -1,  316,   -1,   -1,  319,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,  333,   -1,   -1,   -1,  472,   -1,  339,   -1,  490,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  509,  496,
  497,   -1,  499,  500,  501,  267,   -1,   -1,  505,   -1,
   -1,   -1,   -1,   -1,  276,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,
   -1,  461,   -1,   58,  296,  297,   -1,  399,   63,   -1,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,  485,  418,   -1,  488,  489,
  490,  491,  324,  493,   -1,  327,  496,  497,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,
  452,  353,  454,  355,  356,   -1,   -1,   40,  276,  461,
   43,   -1,   45,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,  290,   -1,   -1,   58,   -1,   -1,  296,  297,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  490,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,  509,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,  353,   -1,  355,  356,   -1,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,  367,
   -1,   -1,   -1,  371,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,
  418,   -1,  267,  505,   -1,   -1,   -1,   -1,   -1,   -1,
  428,  276,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  290,  444,   -1,   -1,   -1,
   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,   -1,   -1,  260,  496,  497,
   -1,  499,  500,  501,  267,   -1,   -1,  505,  353,   -1,
  355,  356,   40,  276,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  367,   -1,   -1,   -1,  371,  290,   -1,   -1,
   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,   -1,
   -1,  304,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,  444,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,
   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,
  505,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,  472,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,   -1,  496,  497,  302,  499,  500,  501,  267,
   40,   -1,  505,   43,   -1,   45,  313,  314,  276,  316,
   -1,   -1,  319,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  290,   63,   -1,   -1,  333,   -1,  296,  297,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,  355,  356,   43,
   -1,   45,  399,   -1,   -1,   -1,  273,   -1,   -1,  367,
  277,   -1,   -1,  371,   58,   -1,   -1,   -1,   -1,   63,
   -1,  418,   -1,   -1,   -1,  292,   -1,   -1,   -1,  296,
   -1,   -1,  299,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,  461,   -1,   -1,   -1,   -1,   -1,
  428,   -1,   -1,  340,   -1,   -1,  434,   -1,   -1,  346,
  347,  348,   -1,  350,  351,  352,  444,  354,   -1,   -1,
   -1,   -1,   -1,  490,  452,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,  374,   -1,  376,
   -1,   -1,  509,   -1,  472,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,   -1,   -1,  267,  496,  497,
  407,  499,  500,  501,   -1,   -1,  276,  505,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,
  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   43,  446,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   58,  324,   -1,   -1,  327,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,  355,  356,   -1,   -1,   -1,
   41,  498,  276,   44,   -1,   -1,   -1,  367,   -1,   40,
   -1,  371,   43,   -1,   45,   -1,  290,   -1,   59,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,  418,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,  353,
   -1,  355,  356,   -1,  444,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,  472,   -1,
   -1,  296,  297,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,  262,  496,  497,   -1,  499,  500,  501,   -1,  324,
   -1,  505,  327,   -1,   -1,   40,  267,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  290,
  355,  356,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,  319,   -1,
   -1,  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,
   -1,  332,   -1,  324,   -1,  336,  327,   -1,   -1,   40,
  341,   -1,   43,  344,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  360,
   -1,   -1,   63,   -1,  355,  356,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,  367,   -1,   -1,  434,
  371,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,  444,
   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,  414,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  436,  491,  428,   -1,   -1,
  441,  496,  497,  434,  499,  500,  501,   -1,   -1,   -1,
  505,   -1,  453,  444,  455,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  455,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,  267,   -1,   -1,  496,  497,   -1,  499,  500,
  501,  276,   -1,   -1,  505,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  290,   -1,   -1,   -1,   -1,
   -1,  296,  297,   -1,   -1,   -1,   -1,   58,   59,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,
  355,  356,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,  367,   -1,   40,   -1,  371,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  324,   -1,   -1,  327,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  418,  355,  356,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,  367,   -1,   -1,  434,
  371,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,  428,   -1,   -1,
   -1,  496,  497,  434,  499,  500,  501,   -1,   -1,   -1,
  505,   -1,   -1,  444,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  455,  276,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,  290,
   -1,  472,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,  324,  505,   -1,  327,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,  355,  356,   -1,   -1,   -1,   40,
  276,   -1,   43,   -1,   45,   -1,  367,   -1,   -1,   -1,
  371,   -1,   -1,    0,  290,   -1,   -1,   58,   -1,   -1,
  296,  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,  428,   45,   -1,
   -1,   -1,   59,  434,   -1,   -1,   -1,  353,   -1,  355,
  356,   58,   -1,  444,   -1,   -1,   63,   -1,   -1,   -1,
   -1,  367,   -1,   -1,   -1,  371,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,  444,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
  296,   -1,   -1,  299,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,    0,   -1,   -1,
  496,  497,   -1,  499,  500,  501,  267,   -1,    0,  505,
   -1,   -1,   -1,   -1,  340,  276,   -1,   -1,   -1,   -1,
  346,  347,  348,   -1,  350,  351,  352,   -1,  354,  290,
   -1,    0,   -1,   -1,   -1,  296,  297,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,  374,   41,
  376,   -1,   44,   -1,   -1,  262,   59,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,  327,   59,   -1,   -1,
  267,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  276,
   -1,  407,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,  290,  355,  356,   -1,   -1,  424,  296,
  297,   -1,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,
  371,   -1,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
  446,   -1,  329,   -1,   -1,  332,   -1,  324,   -1,  336,
  327,   -1,   -1,   -1,  341,  461,   -1,  344,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  360,   -1,  481,  482,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
  367,   -1,  498,  434,  371,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,  444,   -1,   -1,   -1,   -1,   41,   -1,
  397,   44,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   59,  414,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  436,
  491,  428,   -1,   -1,  441,  496,  497,  434,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,  453,  444,  455,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,  472,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  262,  491,   -1,   -1,   -1,   -1,  496,
  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  319,   41,   -1,  322,
   44,   -1,   -1,   -1,   -1,   -1,  329,  319,   -1,  332,
  322,   -1,   -1,  336,   -1,   59,   -1,  329,  341,    0,
  332,  344,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,
  319,   41,  344,  322,   44,   -1,   -1,  360,   -1,   -1,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,  360,   59,
   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,
   41,  384,   -1,   44,  387,  388,   -1,   -1,   -1,   -1,
   -1,  360,  384,   -1,  397,  387,  388,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,  262,
   -1,  414,   -1,   -1,   -1,  384,    0,   -1,  387,  388,
   -1,   -1,  414,   -1,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,   -1,   -1,  436,  414,   -1,   -1,   -1,  441,
  453,   -1,  455,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,  453,   -1,  455,   -1,   -1,  319,  436,   -1,  322,
   -1,   -1,  441,   -1,   -1,   59,  329,   -1,   -1,  332,
    0,   -1,   -1,  336,  453,   -1,  455,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,  262,   59,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,  319,    0,   -1,  322,   -1,
  453,  262,  455,   -1,   -1,  329,   -1,   -1,  332,   -1,
   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,   -1,
  344,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  319,
   -1,   -1,  322,   -1,   -1,   -1,  360,   41,   -1,  329,
   44,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   -1,  344,   59,   -1,   -1,  319,   -1,
  384,  322,   -1,  387,  388,   -1,   41,   -1,  329,   44,
  360,  332,   -1,  397,   -1,  336,   -1,   -1,  262,   -1,
  341,   -1,   -1,  344,   59,   -1,   -1,   -1,   -1,   -1,
  414,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,  360,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   -1,   -1,  436,   -1,    0,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,  384,  414,   -1,  387,  388,   -1,  453,
   -1,  455,   -1,   -1,   -1,  319,  397,   -1,  322,   -1,
   -1,   -1,  262,   -1,   -1,  329,  436,   -1,  332,   -1,
   -1,  441,  336,  414,   -1,   41,   -1,  341,   44,   -1,
  344,   -1,   -1,  453,    0,  455,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  262,  436,  360,   -1,   -1,   -1,
  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  453,   -1,  455,   -1,   -1,   -1,   -1,  319,
  384,   -1,  322,  387,  388,   41,   -1,   -1,   44,  329,
   -1,   -1,  332,  397,   -1,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   59,  344,   -1,   -1,   -1,   -1,   -1,
  414,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,   -1,
  360,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,
   -1,    0,  436,  341,   -1,   -1,  344,  441,  262,   -1,
   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,  453,
   -1,  455,  360,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   41,   -1,  414,   44,  384,   -1,   -1,  387,
  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,
   59,   -1,   -1,   -1,   -1,  319,  436,   -1,  322,   -1,
   -1,  441,   -1,   -1,   -1,  329,  414,   -1,  332,   -1,
   -1,   -1,  336,  453,   -1,  455,   -1,  341,   -1,   -1,
  344,   -1,   -1,   -1,  319,   -1,   -1,  322,  436,   -1,
   -1,   -1,   -1,  441,  329,   -1,  360,  332,   -1,   -1,
   -1,  336,   -1,   -1,   -1,  453,  341,  455,   -1,  344,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
  384,   -1,   -1,  387,  388,  360,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,
  414,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  397,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  436,  319,   -1,   -1,  322,  441,   -1,  414,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,  453,
  336,  455,   -1,   -1,   -1,  341,   -1,   -1,  344,  273,
   -1,  436,   -1,  277,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  453,   -1,
  455,   -1,  296,  319,   -1,  299,  322,   -1,   -1,   -1,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,  384,   -1,
  336,  387,  388,   -1,   -1,  341,   -1,   -1,  344,   -1,
   -1,  397,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,  340,   -1,  414,   -1,
   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,
  354,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,  273,
  436,  387,  388,  277,   -1,  441,   -1,   -1,   -1,   -1,
  374,  397,  376,   -1,   -1,   -1,   -1,  453,   -1,  455,
  319,   -1,  296,  322,   -1,  299,   -1,   -1,  414,   -1,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,  407,   -1,  344,   -1,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,
  424,  360,   -1,   -1,   -1,   -1,  340,  453,   -1,  455,
   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,
  354,   -1,  446,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,
  374,   -1,  376,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  414,   -1,  481,  482,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  407,  498,   -1,   -1,  436,   -1,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  424,   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  498,
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
"LOCK","SAVEPOINT","STATEMENT","LEAVE",
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
{ yyval = 0; }
break;
case 621:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 622:
{ yyval = 0; }
break;
case 623:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 624:
{ yyval = 0; }
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
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 635:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 636:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 637:
{ yyval = 0; }
break;
case 638:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 639:
{ yyval = yyvsp[-1]; }
break;
case 640:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 645:
{ yyval = 0; }
break;
case 646:
{ yyval = make_list (yyvsp[0]); }
break;
case 647:
{ yyval = 0; }
break;
case 649:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 651:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 652:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 653:
{ yyval = make_list (yyvsp[0]); }
break;
case 655:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 658:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 659:
{ yyval = yyvsp[-1]; }
break;
case 660:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 661:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 662:
{ yyval = make_list (yyvsp[-1]); }
break;
case 663:
{ yyval = NULL; }
break;
case 665:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 669:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 670:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 671:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 672:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 673:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 674:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 678:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 679:
{ yyval = make_list (yyvsp[0]); }
break;
case 680:
{ yyval = 0; }
break;
case 682:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 687:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 691:
{ yyval = yyvsp[0]; }
break;
case 692:
{ yyval = 0; }
break;
case 693:
{ yyval = yyvsp[0]; }
break;
case 694:
{ yyval = 0; }
break;
case 695:
{ yyval = make_node (nod_top, 1, yyvsp[-2]);}
break;
case 696:
{ yyval = make_node (nod_top, 2, yyvsp[-4], yyvsp[-3]);}
break;
case 697:
{ yyval = 0;}
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
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 777:
{ yyval = yyvsp[-1]; }
break;
case 778:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 787:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 788:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 789:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 790:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 791:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 792:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 806:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 808:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 810:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 812:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 823:
{ yyval = make_list (yyvsp[-1]); }
break;
case 824:
{ yyval = yyvsp[-1]; }
break;
case 825:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 826:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 836:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 837:
{ yyval = yyvsp[0]; }
break;
case 838:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 839:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 841:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 842:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 843:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 844:
{ yyval = yyvsp[-1]; }
break;
case 845:
{ yyval = yyvsp[-1]; }
break;
case 848:
{ yyval = yyvsp[0]; }
break;
case 849:
{ yyval = yyvsp[0]; }
break;
case 850:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 851:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 852:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 853:
{ yyval = yyvsp[0]; }
break;
case 854:
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
case 855:
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
case 856:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 857:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 859:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 861:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 862:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 863:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 864:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 865:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 866:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 868:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
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
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DATE);
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
			yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIME); 
			}
break;
case 871:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 872:
{ yyval = make_parameter (); }
break;
case 873:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 874:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 875:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 876:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 877:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 878:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 879:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 880:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 881:
{ yyval = yyvsp[0]; }
break;
case 882:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 884:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 885:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 886:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 887:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 888:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 890:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 891:
{ yyval = yyvsp[0];}
break;
case 896:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 897:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 898:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 899:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 900:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 901:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 902:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 903:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 904:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 905:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 906:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 907:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 908:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 909:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 910:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 911:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 913:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 914:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 917:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 918:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 922:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 924:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 925:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 926:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 928:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 932:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 933:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 934:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 935:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 936:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 937:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 938:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 939:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 942:
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
