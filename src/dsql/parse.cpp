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
  336,  266,   40,   43,  205,   41,  291,  405,
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
    1,    1,    1,    1,    1,    1,    1,    1,
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
    0,  968,    0,    0,    0,    0,   33,   69,   67,  960,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  637,    0,    0,    0,  669,    0,    0,
  962,    0,    0,    0,  108,  561,  967,    2,    0,    0,
    0,    0,  450,  432,  433,  434,  435,  436,  437,  438,
  439,  440,  441,  442,  443,  444,  445,  446,  447,  448,
  449,  451,  452,  453,  454,  455,    0,  431,    0,  952,
    0,  410,  956,    0,  407,  959,    0,  671,    0,  406,
  963,    0,  565,  882,    0,  151,  104,    0,  105,    0,
  218,  761,  948,    0,  103,  146,  955,    0,    0,   99,
    0,  106,  888,    0,  100,    0,  101,    0,  102,  966,
  147,  107,  621,  622,    0,  887,  885,    0,   75,    0,
  953,    0,  488,  951,  481,    0,  487,  492,  482,  483,
  490,  484,  485,  486,  491,    0,   31,    0,   40,  748,
   39,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  892,    0,  873,  639,
  641,    0,  642,  644,  941,  645,    0,  646,  635,    0,
    0,    0,    0,    0,    0,    0,  584,  573,    0,  576,
  578,  579,  580,  581,  587,  588,    0,  670,  961,    0,
  109,    0,  110,    0,  111,    0,    0,  613,    0,  611,
    0,    0,    0,    0,  467,    0,    0,  373,    0,  221,
    0,  413,  404,  464,  465,    0,    0,    0,    0,    0,
    0,  422,    0,  379,  380,    0,  571,  569,  883,    0,
    0,    0,    0,    0,    0,    0,  154,  133,    0,   97,
    0,    0,  112,    0,  122,  121,    0,    0,    0,    0,
    0,    0,   76,    0,  944,    0,    0,  724,  725,  489,
    0,  943,  865,  864,  863,    0,    0,  866,  867,    0,
    0,  869,  350,  351,  345,    0,  349,  861,  868,    0,
  751,    0,    0,   28,    0,    0,    0,   72,   70,  964,
   68,   34,    0,    0,    0,  753,    0,   53,    0,    0,
    0,    0,    0,  563,    0,    0,    0,    0,  851,  879,
    0,    0,  853,    0,    0,  880,    0,  874,    0,  855,
  856,  857,  875,  876,    0,  877,  878,    0,    0,    0,
  881,    0,    0,    0,    0,    0,    0,  758,  847,  848,
  849,  854,  833,    0,  832,    0,  836,  895,  896,  835,
  829,  830,  831,  834,  850,  893,  894,  916,  917,  920,
  921,    0,    0,  648,    0,  735,    0,    0,  649,    0,
    0,  610,    0,  585,    0,  582,    0,  583,  608,    0,
  603,    0,    0,  577,  762,  731,    0,    0,    0,    0,
    0,    0,    0,  543,    0,    0,    0,  617,    0,  614,
    0,  186,    0,    0,  130,  472,  173,  174,    0,  470,
    0,    0,    0,    0,  473,    0,  468,    0,    0,  417,
  419,  418,    0,  420,    0,  414,  219,    0,    0,  272,
    0,    0,    0,    0,  426,  427,    0,  457,    0,    0,
    0,    0,    0,    0,    0,  384,  383,  476,    0,    0,
  150,    0,    0,  159,    0,  158,    0,  165,  148,    0,
  163,  166,  167,  155,    0,    0,  511,  547,  514,    0,
  548,    0,    0,  515,    0,    0,    0,  538,  551,  510,
  533,  512,  513,  508,  495,  373,  493,  494,  496,    0,
    0,  505,  506,  507,  509,    0,    0,    0,    0,    0,
  113,    0,  114,    0,  125,    0,    0,  202,    0,    0,
  371,    0,  221,    0,    0,   84,    0,    0,  743,    0,
    0,    0,    0,    0,    0,    0,    0,  764,  768,  769,
  770,  771,  772,  773,  774,  775,  776,  777,  870,  871,
  872,  862,    0,    0,    0,  750,   29,    0,    0,   45,
   73,    0,    0,   27,    0,  756,    0,    0,    0,  719,
    0,    0,    0,    0,    0,  562,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  319,  965,    0,
    0,    0,    0,    0,    0,    0,  640,    0,    0,  643,
    0,    0,    0,    0,  652,  957,  559,    0,  890,  558,
    0,  586,    0,  594,  590,  589,    0,    0,    0,  605,
  593,    0,    0,  729,  730,    0,    0,    0,  371,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  131,    0,
    0,    0,    0,    0,    0,  373,  416,    0,  373,  374,
  421,  503,    0,  274,    0,  268,  270,  371,  241,  949,
    0,    0,  373,    0,    0,  210,  371,    0,    0,    0,
    0,  246,  240,  243,  244,  245,  425,  462,  461,  424,
  423,    0,    0,    0,    0,    0,    0,  382,    0,  373,
    0,  156,  160,    0,  164,    0,    0,  534,  535,  552,
    0,  549,    0,    0,  541,  539,  540,    0,    0,    0,
    0,  504,    0,    0,    0,    0,  542,    0,    0,  371,
    0,  201,    0,  204,  206,  207,    0,    0,    0,    0,
    0,  221,   79,    0,    0,   78,  522,  523,  945,    0,
    0,    0,  767,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  347,  353,  354,  352,  752,    0,    0,
    0,    0,    0,    0,    0,    0,   54,   62,   64,    0,
    0,   71,    0,    0,    0,  755,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  933,  934,
  935,  936,  937,  938,  939,  940,    0,    0,    0,    0,
    0,    0,  926,    0,    0,    0,  667,    0,    0,  845,
  846,  914,    0,    0,  841,  947,    0,    0,    0,    0,
    0,    0,  852,  760,  759,    0,    0,    0,    0,  657,
  658,    0,    0,  650,  653,  560,  891,  595,  604,  599,
  598,    0,  609,  763,  732,  733,  371,    0,    0,  216,
    0,    0,  618,    0,    0,  627,  182,  176,  183,    0,
  179,    0,    0,  178,    0,  189,  170,  169,  889,  172,
  171,  474,    0,    0,    0,  415,  273,    0,  212,  373,
  214,    0,  374,  209,  215,    0,    0,    0,    0,  428,
  429,  373,  371,  374,    0,    0,    0,  400,    0,  197,
  198,  157,    0,    0,    0,    0,    0,    0,  550,    0,
    0,  373,  374,    0,  499,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  199,    0,
    0,    0,    0,  360,    0,   83,    0,    0,    0,    0,
   88,   85,    0,    0,    0,  726,  950,    0,    0,  778,
    0,    0,    0,  804,  803,    0,    0,    0,  811,  823,
  821,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  766,    0,   66,   58,   59,   65,   60,   61,    0,
    0,   23,    0,    0,    0,   43,    0,    0,    0,  737,
  757,    0,  720,    0,  736,    0,    0,    0,    0,    0,
    0,  221,    0,  897,    0,    0,  912,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  922,
    0,  932,    0,    0,    0,    0,    0,    0,  913,  858,
    0,    0,    0,    0,    0,    0,  672,    0,    0,    0,
    0,    0,  601,  602,  606,    0,  200,    0,    0,  619,
  620,  631,    0,    0,    0,    0,    0,    0,  190,    0,
    0,  411,  224,  225,  226,  227,  228,  229,    0,    0,
    0,  277,    0,  222,    0,    0,    0,    0,  247,    0,
  458,  459,  430,    0,    0,    0,    0,    0,    0,  373,
  478,  374,  168,  946,    0,    0,  517,    0,  516,  553,
    0,  545,    0,    0,    0,  497,    0,  525,    0,    0,
  527,  530,  532,  371,    0,    0,    0,  120,    0,  205,
  373,    0,    0,  372,    0,   98,    0,  118,    0,   94,
    0,    0,    0,    0,   87,    0,  819,  820,    0,    0,
    0,    0,    0,    0,  822,    0,    0,    0,    0,    0,
    0,    0,  812,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   57,   63,
   55,   56,   24,  741,  742,    0,  739,  746,  747,  744,
    0,    0,  718,    0,    0,  903,  902,    0,    0,  899,
  898,    0,  907,  906,  905,  904,  901,  900,    0,    0,
  928,    0,    0,    0,  927,    0,    0,    0,  665,    0,
    0,  660,    0,  661,    0,  678,  674,  676,    0,    0,
    0,    0,    0,  372,  624,  625,    0,  184,  180,    0,
    0,  195,  191,    0,  374,  280,    0,    0,  374,  278,
    0,    0,    0,  232,    0,    0,  248,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  475,  519,    0,  520,
    0,  135,    0,    0,    0,  139,  141,  142,  143,  500,
  502,  498,  529,    0,    0,  374,    0,    0,    0,    0,
  355,    0,  361,  373,   89,    0,    0,   91,    0,    0,
    0,    0,    0,    0,  825,  824,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   41,    0,    0,  721,  220,  915,
  908,  909,    0,  910,  924,  923,    0,  929,  918,    0,
  919,    0,  663,    0,    0,  685,    0,  682,  686,  687,
  689,  690,  691,    0,    0,  633,  374,  356,    0,  175,
  192,  374,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  285,
  290,  284,    0,    0,    0,  294,  295,  296,  297,  298,
  299,  302,  304,  307,  267,    0,  208,  233,    0,    0,
    0,    0,  234,  237,    0,    0,  374,  398,  399,  395,
  394,  397,  396,    0,  518,  546,  144,  132,  140,    0,
  373,  264,  127,  374,    0,    0,  375,  362,    0,    0,
   92,   93,   80,    0,  790,  798,    0,  791,  799,  794,
  802,    0,    0,  792,  800,  793,  801,  787,  795,  788,
  796,  789,  797,  738,    0,    0,  925,  666,    0,    0,
    0,    0,  702,  704,    0,  699,    0,  265,  185,  412,
    0,  310,    0,    0,  313,  314,    0,  306,    0,    0,
    0,  305,    0,  332,  333,  293,  300,    0,  303,  286,
    0,  291,    0,  336,  292,  211,  235,    0,    0,  238,
    0,  250,  252,  402,  374,    0,  377,    0,    0,    0,
  367,  368,    0,    0,   77,   95,    0,    0,  911,    0,
    0,  688,  683,  703,    0,    0,  283,    0,  308,    0,
    0,    0,    0,    0,  301,    0,    0,  326,  327,  344,
    0,    0,    0,    0,  339,  337,  239,    0,    0,    0,
    0,    0,  266,  374,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  710,    0,    0,    0,  282,  279,
  309,    0,    0,    0,  312,    0,    0,    0,  318,    0,
  343,  342,  954,  341,    0,    0,    0,    0,    0,  249,
    0,  256,    0,  255,  378,  370,    0,    0,    0,  119,
    0,    0,    0,  712,  701,    0,    0,  713,    0,  709,
    0,    0,    0,    0,    0,    0,    0,    0,  329,  328,
  338,  340,  236,    0,    0,  260,  259,  258,    0,  363,
    0,  634,  827,  708,    0,  715,  321,    0,    0,  311,
    0,    0,    0,  330,  263,  261,  262,    0,    0,    0,
    0,    0,  334,  315,    0,    0,    0,    0,  714,  324,
  316,  826,  717,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1506,   27,   28, 1507,
   30,   31,   32,   33,   34,   35,   36, 1508,   38, 1509,
  619,  103,  383,  875, 1122,  104,  649,  876,  105,  387,
  654,  128,  106,  107,  239,  947,  118,  877,  878,  179,
  208,  108,  879,  109,  389,   79,  219,  353,  436,  614,
 1059,  372,  835,  585,  494,  812,  615,  616,  617, 1061,
  220, 1314,   71,  169,   72,  215, 1780, 1266,  200,  205,
  207,  209,  195,  187,  189,  202,  212,  135,  291,  293,
  295,  343,  601,  603,  380,  240,  762,  519,  626,  996,
  347,  606, 1047, 1258,  552,  709, 1022,  504,  505,  534,
  339,  586, 1033, 1404,  731, 1202, 1405, 1406, 1407, 1408,
 1409,  627,  196,  188,  335,  559,  336,  337,  560,  561,
  562,  563, 1233,  990,  507,  508,  980,  978,  509,  984,
  979, 1193, 1368,  985,  986, 1198,  349,  823,  824,  825,
  826,  764, 1003, 1382,  587,  766,  438,  828,  935,  528,
 1329, 1203,  439,  440,  441,  927,  442, 1383, 1384,  537,
 1533, 1680, 1219,  772,  773,  759,  774,  775,  776, 1681,
 1682, 1757,  317,  532, 1210, 1510,  175,  529,  530, 1211,
 1212, 1377, 1658, 1511, 1512, 1513, 1514, 1623, 1515, 1516,
 1517, 1518, 1519, 1520, 1521, 1522, 1523, 1524, 1703, 1744,
 1607, 1667, 1746,  443,  688, 1348,  444,    0,    0, 1066,
 1624, 1674, 1675, 1712,  375,  376,  445, 1052, 1264, 1421,
    0,    0, 1053, 1054,  267,  467, 1556,  358, 1182, 1362,
 1486, 1639, 1640, 1641, 1642, 1180,  326,  548,  790, 1231,
  549,  788,  203,   56,  321,  180,  142,  304,  167,  311,
  172,  750,  747,  312,  322,  780,  323,  545, 1014,  168,
  305,  516,  550, 1232,   93,  178,  223,  588,  589,  752,
 1034,  753, 1035, 1036,  592,  593,  594,  595, 1027,  838,
  596,  597,  598,  805,  599,  496,  802,   40,   41,   42,
  136,   58,  405,  183,  328,  278,  279,  280,  281,  282,
  283,  284,  285,  286,  715,  480,  962, 1185,  481,  482,
  720,   43,  141,  300,   44,  497,  498, 1190,  736,  976,
  122,  702,  660,  123,  124,  268,  468,  469,  470,  705,
  948,  949,  950,  951, 1174,  446, 1162, 1477, 1478,  447,
 1480,  448,  449,  450,    0, 1695, 1597, 1696, 1697, 1698,
 1740, 1781,  395, 1134,   45,   46,  359,   47,   48,  488,
  489,  884, 1130, 1316, 1320,  396,  657,  192,  628,  629,
  630,  631,  632,  633,  634,  635,  636,  637,  638, 1283,
 1077, 1079, 1080,  934,  687,  451,  452,  453,  454,  455,
  379,  217,  456,  457,  917,  458,  459,  460,  461,  679,
  921, 1163, 1158,  680,  462,
};
static short yysindex[] = {                                   2517,
 1826,  -91, 3266,  155, 1302,   76, 3583,  290, 2959,  722,
  622, 2751,  -91, 1258,  923,  441, 1468,  441,    0,  859,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -54,    0,    0,    0,    0,    0,    0, 4636,
  441,  441,  441,  441,  441,    0,    0,  580, 1330,  441,
  441,  441,  786,  441,  593,  441,  441,    0,  441,  441,
    0,  935,    0,  696,    0,    0,  844,  441,    0,  441,
  441,  441,  874,  441,  441,  441,  441,  593,  441,  441,
  441,  441,    0,  441,  852,    0,    0,    0, 1277,    0,
 1277,    0,  943,  971,  175, 1251,    0,    0,    0,    0,
  441,  441,  441,  987, 1037, 1073,  184, 1649, 1067,  164,
  221,  104, 1044,    0,  441, 1215, 1306,    0, 1149,  441,
    0,  441,  441,  441,    0,    0,    0,    0, 1340,  506,
 1313,   22,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  808,    0, 1330,    0,
  636,    0,    0, 1607,    0,    0, 1025,    0,  736,    0,
    0, 1217,    0,    0, 1296,    0,    0, 1049,    0, 1400,
    0,    0,    0, 1330,    0,    0,    0,  940, 1607,    0,
   69,    0,    0, 1471,    0, 1489,    0, 1277,    0,    0,
    0,    0,    0,    0, 1498,    0,    0,  441,    0, 1544,
    0, 1432,    0,    0,    0,  441,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  660,    0,  441,    0,    0,
    0,  904, 1511,  -39,  441,  967, 1584, 1883, 1608, 1611,
  904, 1538,  -39, 1563, 1575, 1442,    0, 6160,    0,    0,
    0, 6160,    0,    0,    0,    0, 4233,    0,    0, 1523,
  441, 1599, 1514,  807,  441, 1545,    0,    0, 1306,    0,
    0,    0,    0,    0,    0,    0,  441,    0,    0, 1607,
    0, 1471,    0, 1277,    0,  811, 1571,    0, 1553,    0,
 1383, 1640, 1190,   22,    0, 1712, 1178,    0,  441,    0,
  808,    0,    0,    0,    0,  441, 1600,    9, 1557,  569,
 1997,    0, 5760,    0,    0,  677,    0,    0,    0, 1981,
 1981, 1330, 1675, 1330, 1552, 1049,    0,    0, 6969,    0,
  441,  441,    0, 1600,    0,    0, 1763,  826, 2012,  441,
 1792,  441,    0, 2014,    0,  245, 2123,    0,    0,    0,
 1603,    0,    0,    0,    0, 1610, 1622,    0,    0, 1572,
 2023,    0,    0,    0,    0, 2017,    0,    0,    0, 1485,
    0,  441, 1625,    0,  441,  441,  113,    0,    0,    0,
    0,    0,  441,  441,  981,    0,  441,    0, 1773,  441,
 2051,  904, 1694,    0,  441, 2060, 2069, 2071,    0,    0,
 2073, 2075,    0, 2077, 2079,    0, 2082,    0, 2083,    0,
    0,    0,    0,    0, 2084,    0,    0, 4775, 2085, 2087,
    0, 6160, 6160, 4954,  441, 2089, 1759,    0,    0,    0,
    0,    0,    0, 2039,    0, 2088,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2486,    0, 2840,    0, 1810, 2091,    0,   -5,
   72,    0, 1284,    0,  -83,    0,  -83,    0,    0, 2100,
    0,  178, 1719,    0,    0,    0, 2105,  136, 2092,    0,
 1600, 2112, 1891,    0, 1880, 1880, 2117,    0, 2121,    0,
 1788,    0,  210, 1828,    0,    0,    0,    0, 1330,    0,
 1981, 1981, 1981, 1981,    0, 2131,    0, 1904, 1906,    0,
    0,    0, 1884,    0, 6969,    0,    0, 6969, 1574,    0,
 1607, 1924,  441, 6095,    0,    0,   -3,    0,  441, 1202,
 1025,    0, 1793, 1755, 1722,    0,    0,    0,  838, 1798,
    0, 1736,  593,    0, 1330,    0, 1920,    0,    0, 1552,
    0,    0,    0,    0,  203, 1752,    0,    0,    0, 1753,
    0, 1799, 2163,    0, 1857, 2170,   44,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2122,
  690,    0,    0,    0,    0,  206, 2174, 2181, 2170, 1607,
    0, 1907,    0, 1959,    0, 1330, 1330,    0,    9,  736,
    0, 1852,    0, 1825, 2197,    0, 5567,  204,    0, 1853,
 1859, 2211, 2828, 2215, 2302, 5305,  590,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1656, 2023,  441,    0,    0, 1925, 1821,    0,
    0, 1795,  -39,    0, 1939,    0, 1796, 1258, 2222,    0,
 1939, 1925, 1942, 1946,  441,    0,  615, 5018,  756,  441,
 6160,  763,  947, 1014, 2955, 6160, 2828, 2840, 1817,  592,
 6160, 5018,  749,  749, 1258, 2496, 2232,    0,    0, 5255,
  441, 6160, 6160, 6160, 6160, 6160,    0, 6160,  168,    0,
  124, 1822, 5018,  441,    0,    0,    0, 1016,    0,    0,
   52,    0, 1827,    0,    0,    0,  441, 1160,  441,    0,
    0,  441,  441,    0,    0, 5018, 2010,    9,    0,  441,
  935,  935,  811,  141,  441, 1823, 1330, 1330,    0,  -35,
 1736, 1736, 1830,  593, 1190,    0,    0, 2239,    0,    0,
    0,    0, 2004,    0,  441,    0,    0,    0,    0,    0,
 2009, 2244,    0, 1984, 2001,    0,    0, 1952, 1966, 1277,
 2016,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  593,  441, 6662, 1908, 1917, 1919,    0,  593,    0,
 1483,    0,    0, 1889,    0,  122, 1895,    0,    0,    0,
  593,    0, 2163,  187,    0,    0,    0, 2015,  187, 1896,
  187,    0, 2284,  593,  593,  593,    0, 1600,  441,    0,
 1970,    0, 1871,    0,    0,    0,  677, 1913,  441, 1878,
  211,    0,    0, 2286, 2058,    0,    0,    0,    0,  245,
  441, 1915,    0, 1915, 3968,  419, 6160, 6160, 2912, 2294,
 1286, 3147, 6160, 3382, 1355, 3464, 3617, 5338, 3699, 3934,
 4169, 2828, 2828,    0,    0,    0,    0,    0,  441,  441,
  441,  441,  441,  441,  196, 2291,    0,    0,    0, 1925,
 1958,    0,  -20, 1978,  441,    0,  104, 5018, 1978, 2304,
 2291, 1925,  -37, 2031, 6160, 6160, 2074, 6160, 2309, 6160,
 2310, 2530, 6160, 6160, 6160, 6160, 6160, 6160,    0,    0,
    0,    0,    0,    0,    0,    0, 2040, 1903,   27, 6160,
  824, 5018,    0, 2828, 2920, 2840,    0, 2319,  104,    0,
    0,    0, 2840, 1937,    0,    0, 2093,  749,  749,  841,
  841,  804,    0,    0,    0,  124, 2328, 2336,  747,    0,
    0, 2828, 2043,    0,    0,    0,    0,    0,    0,    0,
    0,  -68,    0,    0,    0,    0,    0, 1949, 1913,    0,
 1879, 1879,    0, 2337, 1881,    0,    0,    0,    0, 1950,
    0, 1981, 2125,    0,  -35,    0,    0,    0,    0,    0,
    0,    0, 2348, 2828, 2997,    0,    0, 2095,    0,    0,
    0, 2997,    0,    0,    0, 1277, 1277, 1887, 2352,    0,
    0,    0,    0,    0,   24,  540,  453,    0, 2132,    0,
    0,    0,  441,  155, 1961, 1974, 2004, 2357,    0, 1736,
 1963,    0,    0, 1101,    0, 2344,  441, 1127,  593, 2365,
 2366, 2368, 2148,  736, 2095, 1981, 1828,    9,    0, 1798,
  104, 1968, 1973,    0,   71,    0,  593, 2032, 2108, 5567,
    0,    0,  593, 1933, 2118,    0,    0, 2390, 2391,    0,
  898, 2840, 2395,    0,    0, 2840, 2396, 5518,    0,    0,
    0, 2086, 2402, 2840, 2403, 1723, 2404, 2840, 2405, 6160,
 6160, 2294, 6160, 5600, 2406, 2840, 2409, 2411, 2840, 2412,
 6160, 2840, 2413, 2840, 2414, 2416, 2840, 2418, 2420, 2840,
 2422,    0, 2202,    0,    0,    0,    0,    0,    0, 2139,
 1925,    0, 1925,  196, 2291,    0,  368,  368,   74,    0,
    0, 4233,    0, 1975,    0, 2304, 2291, 2424,  -37, 2546,
 2625,    0, 2686,    0, 2812, 6160,    0, 2937, 2944, 2951,
 2996, 3072, 3210, 6160,  593, 5018, 2840, 2033, 5018,    0,
 6160,    0, 2161,  571, 6160, 5018, 6160, 6160,    0,    0,
  747, 2431, 5018,  441,  124, 2099,    0, 2102, 2104, 2136,
 2201, 2144,    0,    0,    0, 2095,    0, 1968, -100,    0,
    0,    0, 2061, 1330, 2098, 1736, 2103, 1736,    0, 2828,
  698,    0,    0,    0,    0,    0,    0,    0, 2050, 2228,
 2095,    0, 6160,    0, 2219, 2107, 1887,  935,    0, 2828,
    0,    0,    0, 2116, 2124, 2126, 2133, 2137, 2138,    0,
    0,    0,    0,    0, 2467,  155,    0, 1830,    0,    0,
  187,    0, 2997, 2140,  187,    0,  187,    0, 2004, 2471,
    0,    0,    0,    0,  677, 2228, 1736,    0, 1828,    0,
    0, 4233, 2240,    0,  561,    0, 2337,    0, 2483,    0,
 2485, 1330,  -21, 2489,    0, 1330,    0,    0, 6160, 2130,
 2130, 1258, 2493, 1982,    0, 2130, 2130, 6160, 2130, 2130,
 2278, 2840,    0, 2175, 6160, 2840, 2130, 2130, 2130, 2130,
 2840, 2130, 2130, 2130, 2130, 2130, 2130, 2149,    0,    0,
    0,    0,    0,    0,    0, 2115,    0,    0,    0,    0,
 1810, 5018,    0,  -37, 2424,    0,    0, 6969, 2495,    0,
    0, 3254,    0,    0,    0,    0,    0,    0, 3294,  190,
    0, 5018, 2233, 2119,    0, 5018, 3302, 1999,    0, 2063,
 2840,    0, 2019,    0,  747,    0,    0,    0,  124, 1642,
 2828, 2147, 2228,    0,    0,    0, 1981,    0,    0, 1330,
 1483,    0,    0,  700,    0,    0,  441, 4614,    0,    0,
 2840, 1880, 2219,    0, 1280,  441,    0, 2216,  703, 2110,
 2217, 2120, 2257, 2221, 2259, 2095,    0,    0, 2521,    0,
 2525,    0, 2226, 1880, 2140,    0,    0,    0,    0,    0,
    0,    0,    0, 2095, 1798,    0, 1483, 2132, 2247, 2184,
    0, 1913,    0,    0,    0, 2218,  489,    0, 2004, 2224,
 2840, 2553, 2556,  104,    0,    0, 2559, 2560, 2840, 2561,
 2562, 6160, 6160, 2840, 2580, 2584, 2586, 2589, 2591, 2592,
 2593, 2594, 2598, 2600,    0,  368, 1822,    0,    0,    0,
    0,    0,  593,    0,    0,    0, 5018,    0,    0, 5018,
    0, 1822,    0, 1116, 2362,    0, 2599,    0,    0,    0,
    0,    0,    0,  590, 1003,    0,    0,    0, 1736,    0,
    0,    0,    0, 6969,  188,  -44, 2585, 1571, 2605, 2307,
 6160, 2590, 2611, 2596, 2597, 2612, 2615, 2311, 2617,    0,
    0,    0, 2355, 4038, 2620,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2639,    0,    0, 2300, 2338,
  441, 1887,    0,    0, 1277,  441,    0,    0,    0,    0,
    0,    0,    0, 2228,    0,    0,    0,    0,    0, 2228,
    0,    0,    0,    0,  134, 1822,    0,    0, 6160, 1330,
    0,    0,    0, 1330,    0,    0, 6160,    0,    0,    0,
    0, 2840, 2840,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2043, 2643,    0,    0, 2043, 2828,
  441, 1642,    0,    0, 2316,    0, 2647,    0,    0,    0,
  450,    0, 5835,  441,    0,    0, 6160,    0, 2347, 2828,
 2618,    0, 2828,    0,    0,    0,    0,  194,    0,    0,
  583,    0, 2238,    0,    0,    0,    0, 1887, 1277,    0,
 2312,    0,    0,    0,    0, 2132,    0,  134, 2651,  747,
    0,    0, 2043, 2840,    0,    0, 2063, 2144,    0, 2144,
  590,    0,    0,    0, 1040, 2997,    0, 2634,    0, 2801,
 6063, 3317,  194,  713,    0,  803, 1474,    0,    0,    0,
  441,  441,  155,  392,    0,    0,    0, 2312,  545, 1887,
 2314, 2315,    0,    0,  747, 2659,  134, 2359, 2144, 2660,
 1822, 2147, 2147,  441,    0, 2667, 2665, 1043,    0,    0,
    0, 4412, 2299, 2670,    0,  430, 2280, 2417,    0,  194,
    0,    0,    0,    0, 4614,  583, 1887,  732,  732,    0,
 2415,    0, 2276,    0,    0,    0,  747,  134, 2147,    0,
 2043, 2335, 2335,    0,    0, 1040, 2685,    0,  441,    0,
 2496, 2035,   96, 2680, 2432, 2436, 4614, 4614,    0,    0,
    0,    0,    0, 2279,  579,    0,    0,    0, 1407,    0,
 2144,    0,    0,    0,  441,    0,    0,  194, 2698,    0,
  441, 4614, 2439,    0,    0,    0,    0, 2828, 2147, 2700,
 2707, 2037,    0,    0, 4614,  590, 2335,  441,    0,    0,
    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  319,  921,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  156, 2573,    0,    0,    0,    0,    0, 2749,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  444,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2419,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2371,    0,    0,    0,  280,    0,
  280,    0,    0,    0,    0, 2372,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1370,    0,
    0, 4476, 2649,    0,    0,    0, 1374,    0,    0, 1261,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  342,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   75,    0,    0,    0,    0,  879,    0,
    0, 1394,    0,    0,    0,    0,    0, 1516,    0, 7276,
    0,    0,    0,    0,    0,    0,    0,    0,   75,    0,
  541,    0,    0, 2710,    0,    0,    0, 2488,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1419,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1439,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1078,    0,    0,
    0,    0,    0,    0,    0, 2306,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0, 1484,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   75,
    0, 2710,    0, 2488,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1499,    0, 2491,    0,    0,    0,    0,
 1537,    0,    0,    0,    0,    0, 2511,  861, 5924,    0,
 1591,    0,    0,    0,    0,  234,    0,    0,    0, 2320,
 2320,    0,    0,    0, 1602, 1594,    0,    0,    0,    0,
    0,    0,    0, 2511,    0,    0, 1398,    0,    0,    0,
    0,    0,    0, 5192,    0, 2380,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1628,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1633,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1648,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6710,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 6540,    0,    0,  303,    0,    0, 2456,    0,  201,
    0,    0,    0,    0,  480,    0,  480,    0,    0,  648,
    0,  885,  915,    0,    0,    0,    0, 1419,    0,  224,
 2511,    0,    0,    0,  135,  135, 1070,    0,    0,    0,
  114,    0, 2884,  369,    0,    0,    0,    0,    0,    0,
 2320, 2320, 2320, 2320,    0,  544,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2516,    0,    0,    0,    0,    0, 2491,    0,    0, 1243,
    0,  999, 2321,    0,    0,    0,    0,    0,    0,  745,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1667,
    0,    0,    0,    0,  925,  911,    0,    0,    0, 1113,
    0,    0, 1797,    0,    0, 1992,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1209,
 2433,    0,    0,    0,    0, 2029, 1737,    0, 1992,   75,
    0,    0,    0,    0,    0,    0,    0,    0,  861, 1176,
    0,    0,    0,    0, 2382,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  493,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  316,    0,    0, 2573,    0,    0,
  316,    0,    0,    0,    0,    0, 6242,    0, 6242,    0,
    0, 6242, 6242, 6242,    0,    0,    0, 2343,    0,    0,
    0,    0, 7324, 7461, 4711,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1093,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1038,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  861,    0,    0,
  149,  149,    0,    0,    0,  371,    0,    0,    0, 3392,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  109,    0,    0,    0,    0,    0,    0,    0,
 2756,    0,    0, 1415, 1786,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  657,  784,  797,    0,    0,    0,
 2021,    0,    0,    0,    0,    0, 1170,    0,    0,    0,
    0,    0, 1797,    0,    0,    0,    0, 1021,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2511,    0,    0,
  264,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 8802,    0,    0,    0,  338,    0,    0,    0,    0, 2487,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1683, 1686,    0,    0,    0,    0,
    0,    0,    0, 1725,    0,    0, 4476,    0, 1725, 1728,
 1740,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  828,    0,    0, 6242,    0,
    0,    0, 1849,    0,    0,    0, 6847, 7631, 7768, 7017,
 7154,    0,    0,    0,    0,    0, 1216, 1565, 1567,    0,
    0,    0, 1392,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1045, 1045,    0,  182,    0,    0,    0,    0,    0,    0,
    0, 2320, 2345,    0, 3418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2534,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1515,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1744,    0,
    0,    0,    0,    0,    0,    0, 1811,    0,    0,    0,
    0,    0,    0,    0,    0, 1369,    0,    0,    0,    0,
    0,    0,    0, 1176, 2534, 2320, 1748,  861,    0, 2542,
 4476, 1751,  144,    0,    0,    0,    0, 8802,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1969,    0,    0,    0, 2506,    0,    0,    0,    0,
    0,    0,    0, 4223,    0, 5335,    0, 5811,    0,    0,
    0,    0,    0,    0,    0, 7864,    0,    0, 7927,    0,
    0, 8007,    0, 8091,    0,    0, 8125,    0,    0, 8136,
    0,    0, 2318,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1683, 1754,    0, 2374,  350,    0,    0,
    0,    0,    0,    0,    0, 1783, 1790, 1809,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2373,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2469, 1490,    0, 1503,    0, 2470,    0, 2474, 2476,    0,
    0,  370,    0,    0,    0, 2534,    0, 1751,    0,    0,
    0,    0, 3157,    0,    0,    0, 2363,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2360,    0,
 2552,    0,    0,    0, 2296,    0, 1515, 2419,    0,    0,
    0,    0,    0,  867,  909,  953,  959,  961,  978,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  794,    0,    0,    0,    0, 2457,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1819,    0,
    0,    0,    0,    0,    0,    0, 1820,    0,    0,    0,
    0,    0,  171,    0,    0,    0,    0,    0,    0,    0,
    0, 4711,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 8235,    0, 8325,    0, 8331,    0,    0,    0,    0,
 8408,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1824,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1861,    0,    0,    0, 1705,    0,    0,    0,    0,    0,
    0,  760,    0,    0,    0,    0, 2320,    0,    0,    0,
 3168,    0,    0,    0,    0,    0,    0, 2518,    0,    0,
 2788, 1676, 2473,    0, 2491,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2534,    0,    0,    0,    0,
    0,    0,    0, 1854,  928,    0,    0,    0,    0,    0,
    0,    0,    0, 2534, 2542,    0,  294,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  137,    0,
 8494,    0,    0, 6242,    0,    0,    0,    0, 8514,    0,
    0,    0,    0, 8597,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  350, 1075,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  847,    0, 2469,  601,    0, 1076,    0,    0,    0,
    0,    0,    0,  494, 2790,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2528,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1848,    0,    0,  118,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  302,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 8642, 8729,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1255,    0,    0,    0,  662,    0,
    0,    0,    0,    0, 2799,    0,    0,    0,    0,    0,
 2792,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2555,    0,    0,    0,    0, 1848,  515,    0,
  128,    0,    0,    0,    0,    0,    0,    0,  218,  457,
    0,    0,  167, 2788,    0,    0,    0,  299,    0,  340,
 1818,    0,    0,    0, 2790,    0,    0,    0,    0,    0,
  199,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  538,    0, 1515,
  626,  814,    0,    0, 2469, 1507,    0,    0,  143,    0,
  847,  327,  130, 1582,    0,    0, 2820,    0,    0,    0,
    0,    0, 2803,  214,    0, 2567,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1848,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  709,    0,  132,    0,
  662, 1297, 2826,    0,    0, 2790,    0,    0,    0,    0,
 2832,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2469,    0,
  340,    0,    0,    0,    0,    0,    0,    0, 2819,    0,
    0,    0, 4550,    0,    0,    0,    0,    0,  130, 2838,
    0,    0,    0,    0,    0, 1241, 2826,    0,    0,    0,
    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2880,    0,    0, 2888,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2889,
    1,  980,   35, -567, 1768, 1112, -193,  216, 2883, 2648,
    0,   30,    0, 2650,  -93,   33,    0,  587, -884,  -34,
  -42,   -4, -194, 2657, 2253,    0,    0,    0, 1223,    0,
    0,  -36, 1847, -483,   25, -771, 2295, 2078, -276, 1855,
 2830, -286,    0,  -55,    0, -601,  -48,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -547, -564, 1862, -280, 1079, -904,
    0,    0,    0,    0, -316, -101,-1014, 1865, -305,  -46,
    0, -498,    0,    0, -474, -923,    0, 1513,    0,    0,
 2613, -532,   21,    0,    0,    0,    0, 2588,    0, 2361,
 2621, -155, 1890, -677,    0,    0,    0, -698,    0,    0,
    0,    0,    0,    0, 1947,    0, 2642, 2203, 1893, 2619,
 2630,    0, 1337,    0, -493, 2185,  -49, -607, -699,    0,
    0,  -16, -908, -897, -895, -219, -894,    0, 1566, -347,
    0, 1273,-1180, 1569,    0, 2429,    0,    0,    0, 1274,
 1288, 1239,  -14, -277, -624,   42,    0,    0, 2235,    0,
 1760,    0,    0, -206,    0,    0,    0,    0, -132,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -476,    0, -385,    0,-1110, -283,    0,    0, 1203,
 1354,    0, 1265,    0,    0, 2624,  -96, 2028, 1800, 1619,
    0,    0,    0, -548, -840, -817,    0, -472, -240, -337,
 -210,    0, -365, 1361,  335, -763, -469, -689, -889,-1100,
    0,    0,   -2,    0,    0,    0,    0,    0, 2677,    0,
    0,    0,    0, 2692, 2465,    0,    0,    0,    0,    0,
 2704,    0,    0,    0,    0,  -15,    0,    0,    0, -318,
 2199, -313, 1767, -432,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2423,    0,-1106, 2210,    0,    0,    0,
 2609, 3002,    0,    0,    0,    0,    0, 2739,    0,    0,
    0,    0, 2554,    0, 2563,    0,    0,    0, 2327,    0,
    0,    0,   82,    0,  630,    0, 2313, 2047,    0,    0,
 -546, -868, 1022,    0, 2924, 1299,    0, 2346, -600, 2364,
    0, -741, 2106,    0,    0, -112, -653,    0, 1456,-1097,
    0,-1050, -854, -595,    0, 1578,    0, 1314,    0, 1357,
    0, 1278,    0,    0,    0,    0, 2579,    0,    0,    0,
 -340, 2410, 2186, -942,    0,    0,    0,  900,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2848,
 2169, 1988,    0, -473,    0,    0,    0, -224,    0,    0,
 2706,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -183, 1921,    0,   -9,
};
#define YYTABLESIZE 9300
static short yytable[] = {                                     110,
  130,   76,  110,  171,   75,  194,  131,  241,  137,   37,
  191,  378,  495,  190,  553,  724,  374,  288,  260,  263,
  590,  732,  186,  261,  264,  591,  225,  523,  928,  558,
  970,  751,  206,  525,  754,  763, 1387,  229,  710,  981,
  765,  170,  173,  176,  131,  181, 1132,  466,  234,  388,
  193,  170,  197,  656,  176,  233,  131,  181,  388,  210,
  110,  247, 1353,  249,  130,  211,  604,  897,  221,  620,
  131,  224,  170,  232,  221,  197,  173,  176, 1214,  131,
  181,  210,  110,  177,  176,  174, 1204,  235, 1167,  201,
  843,  294,  846, 1204,  890,  204,  199, 1205, 1215, 1206,
 1208,  193,  131,  193, 1205,  228, 1206, 1208,  526, 1223,
  238,  887,  231,  630,  351,  197,  708,  749, 1025,  230,
  289,  969,  176,  131,  210,  966,  236,  257, 1244,  971,
  972,  700,  313,  836,  217, 1768,  259, 1050,  929,  377,
  827,  248,  693,  359,  919,  270,  378,  515,  623,  526,
  998,  374,  526,  435,  486,  568,  653,  340,  749, 1005,
 1261,  749,  292,  946,  290, 1024,  681,  526,  257,  526,
  700,  257,  630, 1638,  487,  217,  749,  526,  217,  723,
  526,  629,  944,  499,  344, 1317,  257,  974,  381,  623,
  700,  651,  623,  217,  741,  742,  743,  744,  739,   74,
  493,  693,  359,  258, 1171, 1008,  663,  623,  355,  944,
 1262,   90, 1045,  727,  568,  590,  355,  364,  245,  373,
  591,  719,  472,  948,  942,  681,  259,  245,  193,  953,
 1464, 1030,  131,  477,  390,  110, 1310,  485, 1312, 1121,
  629,  131,  796,  390,  651,  814, 1602,   74,   74,  738,
 1058,  435, 1427, 1476,  558, 1239,  771,  322,  704,  524,
  262,  173, 1479,  126,  948,  131,  527,  948,  191,  961,
  540,  384,  320,  543,  377,  491,  364,  490,  797,  301,
  384,  487,  948,  259,  948,  399,  490, 1133,  862,   74,
 1012,  869,  477,  196,  713,  554,  533,  556,  693,  193,
 1127,  695,  734,  612,  479,  808,  193,  602,  193, 1481,
  193,  608, 1124,  193, 1321,  740,  768, 1554,  568, 1402,
  806,  982,  126,  749, 1136, 1138,  700, 1397,  302, 1112,
 1113,  176,  181,  139, 1204,  475,  345, 1183,  269,  693,
  131,  628,  173,  734,  485, 1205,  734, 1206, 1208,  524,
  129, 1630,  196,   76,  373,  761, 1491,  693, 1604, 1186,
  695,  734, 1224,  265,   57,  714,  526,  700,  469,  693,
  632, 1031,  131,  600,  740,  176,  390,  568,   86,  610,
  693,   86,  526,  131,  490,  700,  140,  131, 1184,  807,
  176, 1164,  131, 1365,  983,  137,   80,  769,  591,  217,
  628, 1605, 1553, 1366,  386, 1222,  872,  266,  524,  526,
  526,  647,   74,  863,  222,  526, 1128,  650,  526,  378,
 1256,  102,  655,  102,  866,  689,  661,  469,  693,  632,
  259,  384,  650, 1355,  346, 1716,  664,  217,  770, 1310,
  303, 1216, 1217,  616, 1419, 1428,  526, 1677,  466,  495,
  791, 1318, 1457,  217,  630,  102,  365,  591,  388, 1070,
  706, 1201, 1156, 1606,  943,  993, 1225,  623,  995,  102,
 1493,  894,  740, 1710, 1525,  217, 1526, 1412,  737,  596,
   90, 1472, 1001,  466,  767, 1476,  269,  948,  948,  623,
 1268,  477,  694,  692, 1479, 1369,  718,  477,  681, 1720,
  365,  749,  616,  948,  253, 1482,  466, 1267, 1463, 1019,
 1656, 1552, 1349, 1584,  749,  365,  757,  700,  793, 1349,
  526,  651,  629,  760,  193, 1551,  948,  526,  948,  760,
  257,  102,  989,  694,  692, 1684, 1753,  257,  596,  693,
 1043, 1481,  948,  471, 1131,  526,  364,  867,  526,  364,
 1704,  694,  692, 1065, 1060,  749,  102,  707,  749,  191,
 1400, 1363,   76,  681,  948, 1415,  734,  652,  630,  821,
  822, 1325,  700,  749, 1255,  526,  836,  792,  257,  526,
  989,  257, 1598,  693,  102,  818,  700, 1600,  357,  992,
  965, 1742,  568, 1567,  102,  868,  257,  693,  359,  193,
  684,  102,  471,  526, 1284, 1057,  957,  681,  839,  322,
  487,  244,  126,   73,  364,   76, 1388, 1474,  837,  749,
  499,  681,  257,  734,  320,  253,  469,  865,  102,  257,
  695,  477, 1633,  695,  618,  193,  629,  217,  390,  987,
  988,  684,  196,  390,  684,  257, 1414,  597,  102, 1637,
 1120,  623,  390,  813,  102,  176,  387, 1563,  364,  684,
  197,   73,   73,  749,  102, 1196,  253, 1374,  466,  253,
  364, 1490,  364,  485, 1114,  469,  618, 1117,  191,  257,
  862,  936,  628,  740,  253,  381,  693, 1389,  693,  193,
  901,  176,   94, 1745,  706,  693, 1715,  650,  695,  371,
  977,  977,  681,   73,  370,  527,  597,  131,  366,  131,
  693,  632,  193,  490,  700,  387, 1423,  524,  193, 1213,
  936, 1458,  259,  490,  193,  193,  948,  693,  568, 1257,
 1683, 1221, 1699, 1011, 1013, 1434,  693, 1482, 1375, 1690,
 1492,  591,  695, 1537,  401,  193,  479, 1204,  963,   86,
 1002, 1243,  366, 1707, 1228,  734,  695,  693, 1205,  700,
 1206, 1208,  616,  734, 1483,  297,  693,  366,  991,  298,
  378, 1544,  487,  193,  193, 1207,  591,  378, 1691, 1725,
  811, 1060, 1207,  385,  616,  365, 1018,  469,  365, 1550,
  695, 1229,  749,  137,  749,  696,  386,  899, 1028,  618,
  679,  471,  749,  401,  591,  863, 1010,  591, 1401,  131,
  693,  694,  692,  254, 1411,  257, 1588,  257,  700,  131,
 1422,  694,  596,  591,  694,  257,   73,  123, 1484, 1459,
 1118, 1067,  862,  694,  692,  485, 1116,   76, 1040, 1041,
 1042, 1226,  385, 1708, 1561, 1670, 1718, 1168, 1044,  591,
  471,  862,  137,  365,  254,  386,  539,  254, 1055,  390,
  176,  181,  390,  210,  110, 1328,  392, 1385,  668, 1119,
  390,  668,  254, 1558,  265,  490, 1688,  891,  381, 1776,
  694,  692,  390,  390,  607,  596,  668,  695,  596,  694,
  692, 1652,  314,  749,  113, 1671, 1170,  365,  749,  922,
  923,  102, 1115,  253,  596,  253, 1672, 1527,  393,  365,
  536,  365,  466,  253,  592,  749,  257,  138,  895,  684,
  387, 1688,  749,   14,  524,  392,  500,  138,  957, 1548,
  596, 1777,  684,  694,  692,  546,  176,  381,  257,  695,
  693,  684,  694,  607,  696,  257,  547,  694,  692, 1396,
  536,  536,  389,  739,  536,  361,  749,  863,  388,  862,
  391,  862,  471, 1688,  862,  524,  810,  393,  524,  536,
  315,  536, 1341,  592,  862, 1343,  863,  390, 1051,  257,
 1418, 1562, 1227,  524, 1585,  524,  138, 1719,  684,  182,
  597,  115,  324,  681,  111, 1688, 1483,  684,  191, 1589,
 1601,  536,  123,  123,  253,  381, 1346, 1349,  401,  590,
 1673,  389,  362, 1234,  591,  265,  749,  388,  378,  391,
  136,   76,  265, 1207, 1235,  597,  253, 1234,  691,  102,
 1315, 1315, 1319,  253,  692, 1385,  390,  366,  193,  257,
  366,  684,  466,  924,  626,  193,  198,  385, 1349,  681,
 1489,   73,  679,  597,  387,  684,  597, 1651,  681,  898,
  386, 1354,  112, 1250,  862,  306,  903,  253, 1176,  615,
  325,  373,  597,  137,  695,  680, 1475, 1664,  700,  136,
 1666, 1270, 1177, 1643,  863,  626,  863, 1274,  626,  863,
  475,  254,  695,  254, 1371, 1125, 1373,  254,  597,  863,
  700,  254,  466,  626,  771,  366, 1178, 1137,  570, 1754,
  615,  390,  537,  390,  307,  695,  680,  363,  364,  365,
  691,  184,  185,  116,   76,   76,  692,  253,  615,  390,
  392, 1159, 1160,  695,  680,  668,  668,  381,  242,  785,
  366,  367,  381, 1559, 1245,  368,  369,  700,  381,  366,
 1755,  695,  537,  537,  216, 1417,  537,  977, 1465, 1279,
 1179,  366, 1468,  366,  289,  176,  218,  570,  536,  521,
 1245,  537,  393,  537,  607,  695,  786,  691,  695, 1340,
  242,  385,  524,  692,  536,  116, 1706,  536,  536,  863,
  536,  476,  254, 1246,  386,  536,  226, 1756,  536,  213,
  700,  524,  524,  537,  524,  373,  265,  138,  503,  524,
  521,  536,  524,  521,  254,  664,  389,  536, 1731, 1249,
  536,  254,  388,  116,  391,  524,  308,  607,  521,  255,
  521,  524, 1668,   76,  695, 1426, 1399,  214,  668, 1430,
  369,  390,  463,  695,  309, 1786,  477,  499,  125,  503,
  905, 1379,  503,  237,  681,  254,  664,  592,  116,  664,
  671,  242,  607,  478,  392,  487, 1769,  503,   96,   73,
 1636,  102, 1685,  265,  664, 1161,  381, 1668,  310,  668,
  787,  369,  318, 1587,  369,  319,  463,  184,  185,  536,
  607, 1782,  592,  607,  246,  681,  616, 1416,  136,  369,
  136,  463,  242,  524,  671,   98,  393, 1622, 1475,  607,
 1692,  536, 1693,  681,  390,  254,  238,  907,  536,  671,
  592, 1727,  536,  592, 1749,  524,  242,  191,  485,  536,
 1494,  320,  524,  977, 1669,  607,  437,  616,  382,  592,
  463,  524,  341,  524, 1648,  465, 1593,  536, 1650,  176,
  389, 1729,  536,  126,  243,  616,  388, 1668,  391, 1315,
  127,  524, 1759,  626,  102,  592,  524,  193,  490,  566,
  537, 1594,  250,  575,   99,  390,  131,  536,  342, 1669,
 1737,  536, 1668, 1593,  100,  626,  537, 1599,  615,  537,
  537,  681,  537,  572,  680,  524,  968,  537,  658,  136,
  537,  487, 1689,  695, 1487,  330,  695,  680, 1594,  101,
  615,  695,  501,  537,  223, 1535,  680, 1738,  695,  537,
  251,  695,  537, 1779,  695,  659, 1750,  521,  566, 1595,
 1739,  378,  575,  695,  381,  450, 1207, 1176,  348, 1603,
  354, 1631,  331,  600,  332,  381,  521,  521,  360,  521,
  681, 1177,  572,   76,  521,  223,  252,  521,  223, 1669,
 1586,  501,  695,  680,  485,  521, 1595,  333,  511,  968,
  521,  695,  680,  223,  257, 1178,  521,  695,  522,  499,
  695, 1732, 1733,  574, 1669,  170,  503, 1632,  503,  695,
 1761,  537,  334,  503,  600,  754,  503,  348,  408, 1590,
  102,  956,  662,  256,  490,  499,  678, 1609, 1751,  503,
  683,  684,  686,  537,  251,  152,  680, 1710, 1760,  512,
  537,  131,  754, 1645,  537,  646,  173, 1646,  645, 1179,
  680,  537, 1709,  695,  664, 1678,  409,  664,  121,  130,
 1773, 1774,  574,  662,  664,  131,  662,  664,  521,  537,
  499,  664,  271,  513,  537,  251,  664,  408,  251,  664,
 1629,  662,  369,  960,  654, 1784,  655,  287, 1787,  369,
  521,  514,  369,  251,  152,  664,  369,  521, 1791,  537,
  961,  936,  671,  537,  369, 1634,  681,  503,  521,  671,
  405, 1635,  671,  153,  176,  409,  671,  499,  945,  664,
  369,  161,  487,  664,  671,  654,  521,  655,  490,  503,
  655,  521,  664,  296,  756, 1711,  503,  755,   77,   78,
  671,  964,  130,  654,  369,  655, 1090,  346,  131,  664,
  499,  299,   44,  945,  487,  487, 1661,  369, 1081, 1694,
  521,  327,  681, 1091,  671,  131,  316,   52,  272,  405,
  503,  681,  153,  490,  369,  499,  664,  671, 1529,  487,
  161,  170, 1713,  338, 1082,  485,  162,  778,  664,  779,
   76,  130,  487, 1714,  671,  217,  664,  131, 1694,  671,
 1530,  369,   42,  273,  131,   25,  346, 1531, 1092,  711,
 1766,   44,  223,  369,  223,  369,  864,  485,  485,  644,
  490,  671,  223,  845,  656,  490,   52, 1309,  276, 1311,
  681,  274,  130,  671,  275,  671,  217, 1093,  131,  217,
 1694, 1532,  485,  681,  745,  162,  131,   48, 1176,  173,
  276,  502,  681,  490,  217,  485,  531,  490,  490,   50,
  503,   42, 1177,  479,   25,  656,  465,  128,  656,  902,
  376,  120,  121,   26,  918,  173,  277,  329,  490,  925,
  926, 1067,  490,  656,  695,  693, 1178,  694,  933,  696,
  937,  938,  939,  940,  941,  490,  933,  531,  173,  681,
  531,  465,   49,  745, 1094,  493,   48,  348,  681,   51,
 1778,  184,  185,  223,  152,  531,  554,  531,   50,  697,
  695,  693,  479,  694,  465,  696,  128,  350,   46,  376,
  526,  657,   26, 1762, 1763,  223,  152,  659,  129,  117,
 1179,  662,  223,   47,  662,  657,  493,  531,  367,  493,
  511,  662,  681,  657,  662,  352,  886,  554,  662,  885,
  554,   49,  367,  662,  493,  152,  662,  251,   51,  657,
  367,  526,  557,  217,  526,  554,  223,  554,  659,  124,
  124,  659,  662, 1068,  152, 1069,  367,   46, 1792,  526,
  132,  526,  153,  152, 1020, 1021,  659,  129,  117,  152,
  356,  512,   47,  654,  357,  655,  662,  554,  251,  859,
  662,  251,  859,  654,  153,  655,  654,  152,  655,  662,
  502,  860,  133,  657,  860,  654,  251,  655,   95,  503,
  679, 1049,  217,  385, 1048,  513,  662,  134, 1056,  711,
  367,  832,  394,  153,  393, 1071, 1072, 1076,  397,  398,
 1084, 1086, 1088,  514, 1096, 1099, 1102, 1104, 1107, 1110,
  400,  859,  153,  662,  695,  693,  402,  694,  404,  696,
   96,  153,  654,  860,  655,  662,  711,  153,  403,  471,
  473,  654,   97,  655,  474,  896,  465,  900,  813,  711,
  904,  906,  908, 1140, 1141,  153, 1143, 1169, 1145,  483,
 1168, 1148, 1149, 1150, 1151, 1152, 1153,   98,   14, 1187,
 1195,  544, 1048, 1194,  531,  501,  412,  510, 1157,  518,
  926, 1237,  691, 1242, 1236,  654, 1241,  655,  692,  813,
  531,  531,  813,  531,  531, 1323,  531,  654, 1322,  655,
  196,  531, 1436,  656,  531, 1168,  538,  813,  528,  363,
  364,  365,  544,  656, 1288,  544,  656,  531,  691, 1471,
  541,  551, 1470,  531,  692,  656,  531,  555,  679,  605,
  544,  609,  544,  613,  554,  611,   99,  368,  369, 1473,
  644,  648, 1470,  493,  639,  493,  100,  370,  526,  528,
  554,  640,  528,  493,  554, 1767,  554, 1790, 1470,  196,
 1710,  554,  544,  641,  554,  259,  493,  528,  526,  528,
  526,  101,  656,  662,  653,  526,  665,  554,  526,  667,
   73,  656,  102,  554,  695,  693,  554,  694,  668,  696,
  669,  526,  670,  419,  671,  531,  672,  526,  673,  528,
   49,  674,  675,  676,  681,  251,  682,  251,  690,  698,
  701,   50,  425,  699,  703,  251,  659,  531,   51,  659,
  428,  429,  430,  717,  531,  656,  659,  721,  531,  659,
  722,  728,  726,  659,  729,  531,  933,  656,  659,  730,
  733,  659,  625,   52,  493,  432,  734,  433, 1291, 1292,
  735, 1294, 1296,  531,  745,  554,  502,  659,  531, 1301,
  435,  746,  691,  748,  749,  259,  493,  758,  692,  526,
  782,  783,  784,  493,  257,  789,  794,  554,  800,  798,
  799,  659,  801,  531,  554,  659,  803,  531,  554,  804,
  465,  526,  809,  815,  659,  554,  695,  693,  526,  694,
  816,  696,  820, 1155, 1332,  819,  251,  493,   53,  526,
  813,  659, 1339,  554,  926,  829,  831,  926,  554, 1157,
  832,  841,  840, 1347,  926, 1350, 1351,  526,  251,  544,
  842,  926,  526,  869,  844,  251,  883,  880,  659,  881,
   54,  888,  892,  554,   55,  544,  893,  554,  920,  544,
  659,  544,  931,  967,  952,  958,  544,  975,  994,  544,
  810,  526,  999, 1000, 1002,  761,  528,  813,  989,  251,
  813, 1381,  544, 1009, 1015,  230, 1006,  813,  544,  196,
  813,  544,  528, 1016,  813, 1017,  528, 1023,  528,  813,
 1007, 1026,  813,  528, 1037, 1032,  528,  765,  361,  695,
  693,  196,  694, 1039,  696, 1063, 1046,  870,  813,  528,
 1051, 1064,  658, 1078, 1123,  528,  230, 1142,  528,  230,
  465,  625,  691, 1126,  432, 1129,  433, 1121,  692, 1144,
  196, 1139,  813, 1146,  230,  813,  813, 1431,  765,  435,
 1154,  765, 1166,  871,  259,  813, 1439, 1173,  872,  196,
  544, 1181,  691, 1444,  873,  362,  765,  196,  196, 1175,
  645, 1189,  813,  701,  196,  102, 1192, 1200, 1218,  406,
 1197, 1220,  544,  874, 1209, 1230, 1238, 1240,  407,  544,
  465, 1247,  196,  544,  813, 1251, 1252,  528, 1253,  813,
  544, 1254,  408, 1265,  196,  621,  196, 1272,  361,  409,
  926,  813, 1263,  813,  926, 1275, 1057, 1276,  544,  528,
 1277, 1278,  526,  544, 1280, 1281,  528,  622, 1285,  196,
  528, 1286, 1287, 1289, 1290, 1297,  410,  528, 1298,  411,
 1299, 1300, 1302, 1303,  691, 1304,  526, 1305,  544, 1306,
  692, 1307,  544,  862,  196,  528, 1308, 1324, 1342, 1345,
  528, 1352,  231,  526, 1360, 1361,  526,  412,  413, 1359,
  363,  364,  365, 1367,  184,  185, 1443, 1356, 1370,  414,
 1357,  526, 1358,  415, 1372,  528, 1376,  526, 1378,  528,
  526,  623, 1390,  366,  367,  782,  533, 1398,  368,  369,
 1391, 1413, 1392,  231, 1386,  526,  231, 1420, 1403, 1393,
 1572, 1573, 1424, 1394, 1395, 1425,  700,  695,  693, 1429,
  694,  231,  696, 1435, 1455, 1460,  930,  695,  693, 1442,
  694, 1466,  696, 1485,  624,  926,  782, 1282,  926,  782,
  416, 1456, 1538, 1536, 1467, 1539,  417,  691, 1541, 1542,
 1543, 1545, 1540,  692,  782, 1546,  418, 1555,  406, 1557,
 1147,  695,  693,  242,  694,  230,  696,  407, 1547, 1611,
  363,  364,  365,  102,  184,  185, 1326,  695,  693, 1560,
  694,  408,  696, 1565,  419, 1564, 1566,  361,  409, 1568,
 1569, 1570, 1571,  366,  367,  420,  421,  422,  368,  369,
  423,  424,  638,  425,  638,  638,  622,  638,  426,  427,
 1574,  428,  429,  430, 1575,  410, 1576,  431,  411, 1577,
  638, 1578, 1579, 1580, 1581,  638,  765, 1644, 1582,  765,
 1583, 1591, 1592, 1608, 1610, 1647,  765,  112, 1612,  765,
 1613, 1618, 1627,  765, 1614, 1615,  412,  413,  765,  695,
  693,  765,  694, 1620,  696, 1327,  695,  693,  414,  694,
 1616,  696,  415, 1617,  242, 1619, 1665,  765, 1625, 1626,
  623, 1660, 1628, 1649, 1654, 1662, 1655, 1663,  636, 1621,
  636,  636, 1700,  636, 1687, 1679,  242, 1721, 1723, 1726,
 1730,  765, 1728,  242,  765,  765,  636, 1735, 1736, 1743,
  526,  636,  526, 1470,  765, 1747, 1718,  526, 1719,  685,
  526, 1748,  139,  624, 1765, 1771, 1330,  695,  693,  416,
  694,  765,  696,  526,  526,  417,  526,  242, 1770,  926,
 1772, 1710, 1775, 1788,  526,  418, 1785, 1789,    1,  203,
  242,  749,  231,  765,   30,   32,  623,  526,  765,  363,
  364,  365,  102,  184,  185,  691,  564,  782,  373,  524,
  765,  692,  765,  419,  271,  691,  647,    1,  149,  269,
 1741,  692,  366,  367,  420,  421,  422,  368,  369,  423,
  424,  460,  425,   82,  931,  213,  524,  426,  427,    2,
  428,  429,  430,  193,  276,  401,  431,    3,  930,  691,
  524,  526,  679,  677,    4,  692,    5,  673,    6,  675,
  281,  194,  275,    7,  782,  691,  289,  782,  374,  706,
    8,  692,  638,  526,  782,  526,  287,  782,  705,  638,
  526,  782,  695,  693,    9,  694,  782,  696,  638,  782,
  223,  242, 1331,  695,  693,   10,  694,  526,  696, 1701,
  707,  325,  638,  288,  526,  782,  616,  625,  638,  638,
  432,  335,  433,  242,  526,  668,  638,  323,  716,   26,
  242,  695,  693,  177,  694,  435,  696,   29,   39,  782,
  259, 1313,  782,  782,  117,  392,  638,  691,  526,  638,
  401,  391,  782,  692,  691,  882, 1273,  830,  636, 1062,
  692, 1259, 1271,  227,  242,  636, 1269, 1549,  520,  782,
  795,  506,   11,  564,  636,  638, 1248,  638,  638,   12,
  968, 1199,   13,  492,   14,   15,  535, 1657,  636,  638,
 1260,  782,  177,  638,  636,  636,  782,  536, 1528, 1004,
 1717,  434,  636, 1534,  432, 1724,  433, 1758,  782,   16,
  782,  695,  693, 1165,  694,  691,  696,  777, 1722,  435,
 1380,  692,  636, 1783,  259,  636, 1676, 1333,  695,  693,
 1752,  694, 1488,  696, 1334,  695,  693, 1364,  694,  997,
  696, 1335,  695,  693,  643,  694, 1188,  696, 1686,  544,
  638,  636,  526,  636,  636,  781,  638,  517,   17, 1038,
   95, 1410, 1029,  666,  119,  636,  638,  484, 1191,  636,
 1085,  817, 1089,   18, 1097, 1100,  712, 1105, 1108, 1111,
  638,  638,  638,  638,  638,  638, 1336,  695,  693,  716,
  694,  370,  696,  959,  638,  973,  269, 1653,  954, 1764,
 1734, 1172,   96,  638,  638,  638,  638,  638,  638,  638,
  638,  638, 1596,  638,   97, 1793,  725,  955,  638,  638,
  889,  638,  638,  638, 1135,  642,  636,  638,  114, 1293,
  691, 1344,  636,    0,    0,    0,  692,    0,    0,   98,
    0,  691,  636,    0,  406,    0,    0,  692,    0,    0,
    0,    0,    0,  407,    0,    0,  636,  636,  636,  636,
  636,  636, 1337,  695,  693,    0,  694,  408,  696,  691,
  636,    0,    0,  361,  409,  692,    0,    0,    0,  636,
  636,  636,  636,  636,  636,  636,  636,  636,    0,  636,
    0,  177,  622,    0,  636,  636,    0,  636,  636,  636,
    0,  410,    0,  636,  411,    0,  181,    0,   99,    0,
    0,    0,  177,    0,    0,    0,    0,  196,  100,    0,
    0, 1073,    0,    0, 1074,    0,    0,    0,  406,    0,
    0,    0,  412,  413,  177,    0,  434,  407,    0,  432,
  177,  433,    0,  101,  414,    0,    0,  181,  415,  691,
  181,  408,    0,    0,  435,  692,  623,  361,  409,  259,
    0,  102,    0,  177,    0,  181,  691,    0,   95,    0,
    0,    0,  692,  691,    0,    0,  196,    0,    0,  692,
  691,    0,  177,    0,    0,  410,  692,    0,  411,    0,
    0,  177,    0,    0,    0,    0,    0,  177,    0,  624,
 1338,  695,  693,    0,  694,  416,  696,    0,    0,    0,
   96,  417,    0,    0,    0,  177,  412,  413,    0,    0,
    0,  418,   97,    0,    0,  691,    0,    0,  414,    0,
    0,  692,  415,    0,    0,  363,  364,  365,  102,  184,
  185,    0,  361,    0, 1461,  695,  693,   98,  694,  419,
  696,    0,  177,    0,    0,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0,    0,  426,  427,    0,  428,  429,  430,    0,
    0,    0,  431,    0, 1462,  695,  693, 1075,  694,  416,
  696,    0, 1469,  695,  693,  417,  694,    0,  696,  362,
    0,  691,    0,    0,    0,  418,    0,  692,  695,  693,
    0,  694,    0,  696,    0,    0,   99,    0,    0,  363,
  364,  365,  102,  184,  185, 1705,  100,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,  187,  366,  367,  420,  421,  422,  368,  369,  423,
  424,  101,  425,    0,    0,    0, 1083,  426,  427, 1074,
  428,  429,  430,  406,  181,    0,  431,  188,    0,  102,
    0,  434,  407,    0,  432,  196,  433,  909,  910,  911,
  912,  913,  914,  915,  916,  181,  408,    0,    0,  435,
  418,    0,  361,  409,  259,    0,  196,    0,    0,    0,
  187,    0,    0,    0,  363,  364,  365,  181,  184,  185,
    0,    0,    0,  181,    0,    0,    0,    0,  196,    0,
  410,    0,    0,  411,  196,    0,  188,  366,  367,  420,
  421,  422,  368,  369,  423,  424,  181,    0,    0,  691,
    0,    0,  426,  427,    0,  692,    0,  196,    0,    0,
    0,  412,  413,  434,    0,  181,  432,    0,  433,    0,
    0,    0,    0,  414,  181,    0,  196,  415,    0,    0,
  181,  435,    0,    0,  196,  196,  259,    0,    0,    0,
    0,  196,    0,  691,    0,    0,    0,    0,  181,  692,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  196,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   59,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   60, 1075,  691,  416,  181,    0,    0,   61,  692,
  417,  691,    0,    0,    0,    0,  196,  692,    0,    0,
  418,   62,    0,    0,    0,    0,  691,  196,    0,    0,
    0,    0,  692,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0, 1087,  426,  427, 1074,  428,  429,  430,  406,  187,
    0,  431,   63,    0,    0,    0,  434,  407,    0,  432,
    0,  433,    0,    0,    0,    0,    0,    0,   64,    0,
  187,  408,    0,    0,  435,  188,    0,  361,  409,  259,
    0,    0,    0,    0,    0,   65,    0,    0,    0,    0,
    0,    0,  187,    0,    0,    0,  188,    0,  187,    0,
   66,    0,    0,    0,   67,  410,    0,   68,  411,    0,
    0,    0,    0,    0,    0,   69,    0,    0,  188,    0,
    0,  187,    0, 1095,  188,    0, 1074,    0,    0,    0,
  406,    0,    0,    0,   70,    0,  412,  413,  434,  407,
  187,  432,    0,  433,    0,    0,    0,  188,  414,  187,
    0,    0,  415,  408,    0,  187,  435,    0,    0,  361,
  409,  259,    0,    0,    0,    0,  188,    0,    0,    0,
    0,    0,    0,  187,    0,  188,    0,    0,    0,    0,
    0,  188,    0,    0,    0,    0,    0,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,    0,  188,
    0,    0,    0,    0,    0,    0,    0, 1075,    0,  416,
  187,    0,    0,    0,    0,  417,    0,    0,  412,  413,
    0,    0,    0,    0,    0,  418,    0,    0,    0,    0,
  414,    0,    0,    0,  415,    0,  188,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,    0,    0,    0, 1098,  426,  427, 1074,
  428,  429,  430,  406,    0,    0,  431,    0,   81, 1075,
    0,  416,  407,    0,    0,   82,    0,  417,    0,   83,
   84,    0,    0,    0,    0,    0,  408,  418,   85,    0,
    0,    0,  361,  409,    0,    0,    0,    0,    0,    0,
   86,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
  410,    0,    0,  411,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0, 1103,  426,
  427, 1074,  428,  429,  430,  406,    0,    0,  431,    0,
    0,  412,  413,  434,  407,    0,  432,    0,  433,    0,
    0,    0,    0,  414,    0,   87,    0,  415,  408,    0,
    0,  435,    0,    0,  361,  409,  259,    0,    0,    0,
    0,    0,   88,    0,    0,    0,    0,    0,  930,  695,
  693,    0,  694,    0,  696,    0,    0,   89,    0,    0,
    0,   90,  410,    0,    0,  411,    0,  860,  859,  861,
    0,    0,   91,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1075,    0,  416,    0,    0,    0,    0,    0,
  417,   92,    0,  412,  413,    0,    0,    0,    0,    0,
  418,    0,    0,    0,    0,  414,    0,    0,    0,  415,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,    0,    0,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0, 1075,    0,  416, 1432, 1433,    0,
    0,    0,  417, 1437, 1438,    0, 1440, 1441,    0,    0,
    0,    0,  418,    0, 1445, 1446, 1447, 1448,    0, 1449,
 1450, 1451, 1452, 1453, 1454,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,    0,  425,
    0,    0,    0, 1106,  426,  427, 1074,  428,  429,  430,
  406,    0,    0,  431,    0,    0,    0,    0,  434,  407,
    0,  432,    0,  433,    0,    0,    0,    0,    0,    0,
    0,    0,  783,  408,    0,    0,  435,    0,    0,  361,
  409,  259,    0,    0,    0,    0,    0,    0,    0,  847,
    0,    0,    0,    0,    0,    0,    0,  691,    0,    0,
    0,    0,    0,  692,    0,    0,  848,  410,    0,    0,
  411,    0,    0,  783,    0,    0,  783,    0,    0,    0,
    0,    0,  434,    0,  464,  432,    0,  433,    0,    0,
    0,  783,    0,    0,    0,    0,    0,    0,  412,  413,
  435,    0,  849,    0,    0,  259,    0,    0,    0,    0,
  414,  850,    0,    0,  415,    0,    0,    0, 1378,  851,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  852,    0,
  853,    0,    0,    0,    0,    0,    0,    0,    0,    6,
    0,    0,    0,    0,  854,    0,  855,  856,  857,    0,
 1495, 1496,    0, 1497,    0,    0, 1498,    0,    0, 1075,
    0,  416,    0,    0,    0,    0,    0,  417,    0,    0,
 1499,    0,    0,    0,    0,    0, 1500,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,  858,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,    0,    0,    0, 1109,  426,
  427, 1074,  428,  429,  430,  406, 1501,    0,  431,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,  434,    0,    0,  432,   14,  433,    0,  408,    0,
    0,    0,    0,    0,  361,  409,    0,    0,    0,  435,
 1502,    0,    0,    0,  259,    0,    0,    0,    0,    0,
   16,    0,    0,    0,  783,    0,    0,    0,    0, 1621,
    0, 1503,  410,    0,    0,  411,    0,    0,  102,  406,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,  942,    0,  942,  942,    0,
  942,    0,  408,  412,  413,    0,    0, 1504,  361,  409,
    0,    0,    0,  942,    0,  414,    0,    0,  942,  415,
    0,  783,    0,    0,  783,    0, 1505,    0,    0,    0,
    0,  783,    0,    0,  783,    0,  410,    0,  783,  411,
    0,    0,    0,  783,    0,    0,  783,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  783,    0,    0,  362,    0,  412,  413,    0,
    0,    0,    0,    0, 1075,    0,  416,    0,    0,  414,
    0,    0,  417,  415,    0,    0,  783,    0,    0,  783,
  783,    0,  418,    0,    0,    0,    0,    0,    0,  783,
    0,    0,    0,    0,    0,    0,  363,  364,  365,  102,
  184,  185,    0,    0,    0,    0,  783,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,    0,  366,
  367,  420,  421,  422,  368,  369,  423,  424,  783,  425,
  416,    0,    0,  783,  426,  427,  417,  428,  429,  430,
    0,    0,    0,  431,    0,  783,  418,  783,  406,    0,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
  363,  364,  365,  102,  184,  185,    0,    0,    0,    0,
    0,  408,    0,    0,  419,    0,    0,  361,  409,    0,
    0,    0,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,    0,  425,    0,    0,    0,    0,  426,  427,
    0,  428,  429,  430,    0,  410,    0,  431,  411,    0,
    0,    0,  942,    0,    0,    0,    0,    0,    0,    0,
  638,  942,    0,  638,    0,  638,    0,    0,    0,    0,
    0,    0,    0,    0,  362,  942,  412,  413,  638,    0,
    0,  942,  942,  638,    0,    0,    0,    0,  414,    0,
    0,    0,  415,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  942,
    0,    0,  942,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  434,    0,    0,  432,    0,  433,
  317,    0,    0,    0,    0,    0,    0,    0,  942,  685,
  942,  942,  435,    0,    0,    0,    0,  259,    0,  416,
    0,    0,  942,    0,    0,  417,  942,    0,    0,    0,
    0,  317,    0,    0,    0,  418,    0,    0,  317,    0,
    0,    0,  317,  317,    0,  317,    0,    0,  317,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,  317,  419, 1378,    0,    0,    0,  317,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,    0,  425,  942,    0,    0,    0,  426,  427,  942,
  428,  429,  430,    0,    0,    6,  431,    0,    0,  942,
    0,    0,    0,    0,    0,    0, 1495, 1496,    0, 1497,
    0,    0, 1498,  942,  942,  942,  942,  942,  942,    0,
    0,    0,    0,    0,    0,    0, 1499,  942,  317,    0,
    0,    0, 1500,    0,    0,    0,  942,  942,  942,  942,
  942,  942,  942,  942,  942,    0,  942,  317,    0,    0,
  638,  942,  942,    0,  942,  942,  942,  638,    0,    0,
  942,    0,  317,    0,    0,    0,  638,    0,    0,    0,
    0,    0,  317,  434,    0,    0,  432,    0,  433,    0,
  638,  317,    0,  317,    0,    0,  638,  638,    0,    0,
  317,  435, 1501,    0,  638,    0,  259,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,    0,    0,  638,    0,    0,  638,    0,  317,
    0,  406,    0,    0,    0,    0, 1502,    0,    0,    0,
  407,    0,    0,    0,    0,    0,   16,  434,  317,    0,
  432,    0,  433,    0,  408,  638,  638, 1503,    0,    0,
  361,  409,    0,    0,  102,  435,    0,  638,    0,    0,
  259,  638,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  143,    0,  410,    0,
    0,  411,    0, 1504,    0,  144,  145,  146,  147,  148,
  149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
  159,    0, 1505,  160,  161,  162,  163,    0,  164,  412,
  413,  165,  166,    0,    0,    0,    0,    0,  638,    0,
    0,  414,    0,    0,  638,  415,    0,    0,    0,    0,
    0,    0,    0,    0,  638,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  638,  638,
  638,  638,  638,  638,    0,    0,    0,    0,    0,    0,
    0,    0,  638,    0,    0,    0,    0,    0,    0,    0,
    0,  638,  638,  638,  638,  638,  638,  638,  638,  638,
    0,  638,  416,    0,    0,    0,  638,  638,  417,  638,
  638,  638,    0,    0,    0,  638,    0,    0,  418,    0,
  406,    0,    0,    0,    0,    0,  677,    0,    0,  407,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,    0,    0,  408,    0,    0,  419,    0,    0,  361,
  409,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,    0,  410,    0,  431,
  411,    0,    0,    0,  406,    0,    0,    0,    0,    0,
    0,    0,    0,  407,  434,  932,    0,  432,    0,  433,
    0,    0,    0,    0,    0,    0,    0,  408,  412,  413,
    0,    0,  435,  361,  409,    0,    0,  259,    0,    0,
  414,    0,    0,    0,  415,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  807,    0,    0,    0,    0,    0,
    0,  410,    0,    0,  411,    0,  695,  693,    0,  694,
    0,  696,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  860,  859,  861,    0,    0,    0,
  362,  685,  412,  413,    0,  807,    0,  434,  807,    0,
  432,  416,  433,    0,  414,    0,    0,  417,  415,    0,
    0,    0,    0,  807,    0,  435,    0,  418,    0,    0,
  259,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,    0,    0,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,    0,  425,  416,    0,    0,    0,  426,
  427,  417,  428,  429,  430,    0,    0,    0,  431,    0,
    0,  418,    0,    0,  221,    0,    0,    0,  221,    0,
    0,    0,    0,    0,    0,  363,  364,  365,  102,  184,
  185,    0,    0,  221,    0,    0,    0,  221,    0,  419,
  221,    0,    0,    0,    0,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0,    0,  426,  427,    0,  428,  429,  430,    0,
    0,  406,  431,    0,    0,    0,    0,    0,    0,    0,
  407,  221,    0,    0,    0,    0,    0,  221,  221,  221,
    0,  221,  221,  221,  408,  221,    0,    0,    0,    0,
  361,  409,    0,    0,    0,    0,    0,  434,    0,    0,
  432,    0,  433,    0,    0,  221,    0,  221,    0,    0,
    0,    0,    0,    0,    0,  435,  847,    0,  410,    0,
  259,  411,    0,    0,  691,    0,    0,    0,    0,    0,
  692,    0,    0,  848,    0,    0,  807,    0,  221,    0,
    0,    0,    0,   81,  406,    0,    0,    0,    0,  412,
  413,    0,    0,  407,    0,  221,    0,    0,    0,    0,
    0,  414,    0,    0,    0,  415,    0,  408,    0,  849,
    0,    0,    0,  361,  409,    0,    0,  221,  850,  434,
    0,    0,  432,    0,  433,    0,  851,    0,    0,    0,
    0,    0,    0,  807,    0,    0,  807,  435,    0,    0,
    0,  410,  259,  807,  411,  852,  807,  853,    0,    0,
  807,    0,  221,  221,    0,  807,    0,    0,  807,    0,
    0,  854,  416,  855,  856,  857,    0,    0,  417,  221,
    0,    0,  412,  413,  807,    0,    0,    0,  418,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0,    0,  363,  364,  365,  102,  184,  185,  807,    0,
    0,  807,  807,    0,    0,    0,  419,    0,    0,    0,
    0,  807,    0,    0,  858,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,  807,    0,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
    0,    0,    0,    0,    0,  416,    0,    0,    0,    0,
  807,  417,    0,    0,    0,  807,    0,    0,    0,    0,
    0,  418,    0,    0,  406,    0,    0,  807,    0,  807,
    0,    0, 1101,  407,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,    0,  408,    0,  419,
  786,    0,    0,  361,  409,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0,    0,  426,  427,    0,  428,  429,  430,  833,
    0,  410,  431,  566,  411,    0,    0,    0,    0,    0,
    0,  786,    0,    0,  786,    0,    0,    0,  834,    0,
    0,    0,  567,    0,    0,  568,  406,    0,    0,  786,
    0,    0,  412,  413,  434,  407,    0,  432,    0,  433,
    0,    0,    0,    0,  414,    0,    0,    0,  415,  408,
    0,    0,  435, 1659,    0,  361,  409,  259,    0,    0,
    0,    0,    0,    0,    0,    0,  569,    0,    0,    0,
    0,    0,  570,  571,  572,    0,  573,  574,  575,    0,
  576,    0,    0,  410,    0,    0,  411,    0,    0,    0,
    0,    0,    0,    0,    0, 1282,    0,    0,    0,    0,
  577,    0,  578,    0,    0,  416,    0,    0,    0,    0,
    0,  417,    0,    0,  412,  413,    0,    0,    0,    0,
    0,  418,    0,    0,    0,    0,  414,    0,    0,    0,
  415,    0,    0,  579,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,    0,    0,    0,  419,
  580,    0,    0,    0,    0,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0,  581,  426,  427,    0,  428,  429,  430,    0,
    0,    0,  431,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,    0,  582,  583,    0,
    0,    0,    0,    0, 1295,    0,    0,  363,  364,  365,
  102,  184,  185,    0,  584,    0,    0,    0,    0,    0,
    0,  419,  786,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,  406, 1702,    0,  431,  432,    0,  433,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  435,    0,    0,    0,  408,  259,    0,    0,    0,  786,
  361,  409,  786,    0,    0,    0,    0,    0,    0,  786,
    0,    0,  786,    0,    0,    0,  786,    0,    0,    0,
    0,  786,    0,    0,  786,    0,    0,    0,  410,    0,
    0,  411,    0,    0,    0,    0,    0,    0,    0,    0,
  786,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  412,
  413,    0,    0,    0,  786,    0,    0,  786,  786,  434,
    0,  414,  432,    0,  433,  415,    0,  786,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,    0,
  542,    0,  259,    0,  786,    0,    0,    0,    0,  144,
  145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
  155,  156,  157,  158,  159,    0,  786,  160,  161,  162,
  163,  786,  164,    0,    0,  165,  166,    0,    0,    0,
    0,    0,  416,  786,    0,  786,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  418,    0,
    0,  942,    0,    0,  942,    0,  942,    0,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,  942,
    0,    0,    0,    0,  942,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,  406,
  426,  427,    0,  428,  429,  430,    0,    0,  407,  431,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,    0,    0,    0,    0,    0,  361,  409,
    0,    0,    0,    0,    0,    0,    0,  565,    0,    0,
    0,  566,    0,    0,    0,    0,    0,    0,    0,  761,
    0,    0,    0,    0,  456,    0,  410,    0,    0,  411,
  567,    0,    0,  568,  456,  456,  456,  456,  456,  456,
  456,  456,  456,  456,  456,  456,  456,  456,  456,    0,
    0,  456,  456,  456,  456,  362,  456,  412,  413,  456,
  456,    0,    0,    0,    0,    0,  406,    0,    0,  414,
    0,    0,    0,  415,  569,  407,    0,    0,    0,    0,
  570,  571,  572,    0,  573,  574,  575,    0,  576,  408,
    0,    0,    0,    0,    0,  361,  409,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  577,    0,
  578,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  410,    0,    0,  411,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,  579,    0,    0,    0,    0,  418,    0,  942,    0,
    0,    0,    0,    0,  412,  413,    0,  942,  580,    0,
  363,  364,  365,  102,  184,  185,  414,    0,    0,    0,
  415,  942,    0,    0,  419,    0,    0,  942,  942,  948,
  581,    0,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,    0,  425,    0,  102,    0,    0,  426,  427,
    0,  428,  429,  430,    0,  942,    0,  431,  942,    0,
    0,    0,    0,    0,    0,  582,  583,    0,    0,  944,
  948,  948,  948,  948,  948,  961,  948,  416,    0,    0,
    0,    0,  584,  417,    0,    0,  942,  942,  948,  948,
  948,  948,    0,  418,    0,    0,    0,    0,  942,    0,
    0,    0,  942,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
  948,  419,  948,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0,    0,    0,    0,  942,
    0,    0,    0,    0,    0,  942,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  942,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  942,
  942,  942,  942,  942,  942,    0,    0,    0,    0,  828,
    0,    0,    0,  942,    0,    0,    0,    0,    0,    0,
    0,    0,  942,  942,  942,  942,  942,  942,  942,  942,
  942,    0,  942,    0,    0,    0,    0,  942,  942,    0,
  942,  942,  942,    0,    0,    0,  942,    0,    0,    0,
  828,  828,  828,  828,  828,    0,  828,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  828,  828,
  828,  828,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  948,  828,  948,    0,    0,    0,    0,    0,    0,
    0,  948,    0,    0,    0,    0,    0,    0,    0,  948,
    0,    0,    0,    0,    0,  948,    0,    0,  948,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  840,  948,  948,    0,
    0,  948,    0,    0,    0,    0,    0,    0,  948,    0,
  948,  948,    0,    0,  948,    0,    0,    0,  948,    0,
    0,  948,    0,  948,    0,  948,    0,    0,    0,    0,
  948,  948,    0,  948,    0,    0,    0,  840,  840,  840,
  840,  840,    0,  840,    0,    0,    0,    0,    0,  948,
  948,    0,  948,    0,    0,  840,  840,  840,  840,    0,
    0,    0,    0,    0,    0,    0,  948,    0,  948,  948,
  948,    0,    0,  948,    0,    0,  948,  948,    0,    0,
    0,    0,    0,    0,  565,    0,  948,    0,  566,  840,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  948,    0,    0,  948,    0,    0,    0,  567,    0,    0,
  568,    0,    0,    0,    0,    0,    0,    0,    0,  948,
    0,  828,    0,  828,    0,  948,    0,    0,    0,    0,
  948,  828,    0,    0,    0,    0,    0,    0,    0,  828,
    0,  948,  948,    0,  948,  828,    0,    0,  828,    0,
  948,  569,    0,    0,    0,    0,    0,  570,  571,  572,
    0,  573,  574,  575,    0,  576,  843,  828,  828,    0,
    0,  828,    0,    0,    0,    0,    0,    0,  828,    0,
  828,  828,    0,    0,  828,  577,    0,  578,  828,    0,
    0,  828,    0,  828,    0,  828,    0,    0,    0,    0,
  828,  828,    0,  828,    0,    0,    0,  843,  843,  843,
  843,  843,    0,  843,    0,    0,    0,    0,  579,  828,
  828,    0,  828,    0,    0,  843,  843,  843,  843,    0,
    0,    0,    0,    0,    0,  580,  828,    0,  828,  828,
  828,    0,    0,  828,    0,    0,  828,  828,    0,    0,
    0,    0,    0,    0,    0,    0,  828,  581,  840,  843,
  840,    0,    0,    0,    0,    0,    0,    0,  840,    0,
  828,    0,  102,  828,    0,    0,    0,    0,    0,    0,
    0,    0,  840,    0,    0,  840,    0,    0,    0,  828,
    0,    0,  582,  583,    0,  828,    0,    0,    0,    0,
  828,    0,    0,  844,  840,  840,    0,    0,  840,  584,
    0,  828,  828,    0,  828,  840,    0,  840,  840,    0,
  828,  840,    0,    0,    0,  840,    0,    0,  840,    0,
  840,    0,  840,    0,    0,    0,    0,  840,  840,    0,
  840,    0,    0,    0,  844,  844,  844,  844,  844,    0,
  844,    0,    0,    0,    0,    0,  840,  840,    0,  840,
    0,    0,  844,  844,  844,  844,    0,    0,    0,    0,
    0,    0,    0,  840,    0,  840,  840,  840,    0,    0,
  840,    0,    0,  840,  840,    0,    0,    0,    0,    0,
    0,  565,    0,  840,    0,  566,  844,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  840,    0,    0,
  840,    0,    0,    0,  567,    0,    0,  568,    0,    0,
    0,    0,    0,    0,    0,    0,  840,    0,  843,    0,
  843,    0,  840,    0,    0,    0,    0,  840,  843,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  840,  840,
    0,  840,    0,    0,    0,  843,    0,  840,  569,    0,
    0,    0,    0,    0,  570,  571,  572,    0,  573,  574,
  575,    0,  576,  838,  843,  843,    0,    0,  843,    0,
    0,    0,    0,    0,    0,  843,    0,  843,  843,    0,
    0,  843,  577,    0,  578,  843,    0,    0,  843,    0,
  843,    0,  843,    0,    0,    0,    0,  843,  843,    0,
  843,    0,    0,    0,  838,    0,  838,  838,  838,    0,
    0,    0,    0,    0,    0,  579,  843,  843,    0,  843,
    0,    0,  838,  838,  838,  838,    0,    0,    0,    0,
    0,    0,  580,  843,    0,  843,  843,  843,    0,    0,
  843,    0,    0,  843,  843,    0,    0,    0,    0,    0,
    0,    0,    0,  843,  581,  844,  838,  844,    0,    0,
    0,    0,    0,    0,    0,  844,    0,  843,    0,    0,
  843,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  844,    0,    0,    0,  843,    0,    0,  582,
  583,    0,  843,    0,    0,    0,    0,  843,    0,    0,
  837,  844,  844,    0,    0,  844,  584,    0,  843,  843,
    0,  843,  844,    0,  844,  844,    0,  843,  844,    0,
    0,    0,  844,    0,    0,  844,    0,  844,    0,  844,
    0,    0,    0,    0,  844,  844,    0,  844,    0,    0,
    0,  837,    0,  837,  837,  837,    0,    0,    0,    0,
    0,    0,    0,  844,  844,    0,  844,    0,    0,  837,
  837,  837,  837,    0,    0,    0,    0,    0,    0,    0,
  844,    0,  844,  844,  844,    0,    0,  844,    0,    0,
  844,  844,    0,    0,    0,    0,    0,    0,  134,    0,
  844,    0,  134,  837,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  844,    0,    0,  844,    0,    0,
    0,  134,    0,    0,  134,    0,    0,    0,    0,    0,
    0,    0,    0,  844,    0,  838,    0,  838,    0,  844,
    0,    0,    0,    0,  844,  838,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  844,  844,    0,  844,    0,
    0,    0,  838,    0,  844,  134,    0,    0,    0,    0,
    0,  134,  134,  134,    0,  134,  134,  134,    0,  134,
  839,  838,  838,    0,    0,  838,    0,    0,    0,    0,
    0,    0,  838,    0,  838,  838,    0,    0,  838,  134,
    0,  134,  838,    0,    0,  838,    0,  838,    0,  838,
    0,    0,    0,    0,  838,  838,    0,  838,    0,    0,
    0,  839,    0,  839,  839,  839,    0,    0,    0,    0,
    0,    0,  134,  838,  838,    0,  838,    0,    0,  839,
  839,  839,  839,    0,    0,    0,    0,    0,    0,  134,
  838,    0,  838,  838,  838,    0,    0,  838,    0,    0,
  838,  838,    0,    0,    0,    0,    0,    0,    0,    0,
  838,  134,  837,  839,  837,    0,    0,    0,    0,    0,
    0,    0,  837,    0,  838,    0,    0,  838,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  837,
    0,    0,    0,  838,    0,    0,  134,  134,    0,  838,
    0,    0,    0,    0,  838,    0,    0,  842,  837,  837,
    0,    0,  837,  134,    0,  838,  838,    0,  838,  837,
    0,  837,  837,    0,  838,  837,    0,    0,    0,  837,
    0,    0,  837,    0,  837,    0,  837,    0,    0,    0,
    0,  837,  837,    0,  837,    0,    0,    0,  842,    0,
  842,  842,  842,    0,    0,    0,    0,    0,    0,    0,
  837,  837,    0,  837,    0,    0,  842,  842,  842,  842,
    0,    0,    0,    0,    0,    0,    0,  837,    0,  837,
  837,  837,    0,    0,  837,    0,    0,  837,  837,    0,
    0,    0,    0,    0,    0,    0,    0,  837,    0,    0,
  842,    0,    0,  784,    0,    0,    0,    0,    0,    0,
    0,  837,    0,    0,  837,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  837,    0,  839,    0,  839,    0,  837,    0,    0,    0,
    0,  837,  839,    0,  784,    0,    0,  784,    0,    0,
    0,    0,  837,  837,    0,  837,    0,    0,    0,  839,
    0,  837,  784,    0,    0,    0,  785,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  839,  839,
    0,    0,  839,    0,    0,    0,    0,    0,    0,  839,
    0,  839,  839,    0,    0,  839,    0,    0,    0,  839,
    0,    0,  839,    0,  839,    0,  839,  785,    0,    0,
  785,  839,  839,    0,  839,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  785,    0,    0,    0,    0,
  839,  839,    0,  839,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  815,  839,    0,  839,
  839,  839,    0,    0,  839,    0,    0,  839,  839,    0,
    0,    0,    0,    0,    0,    0,    0,  839,    0,  842,
    0,  842,    0,    0,    0,    0,    0,    0,    0,  842,
    0,  839,    0,    0,  839,    0,    0,  815,    0,    0,
  815,    0,    0,    0,    0,    0,  842,    0,    0,    0,
  839,    0,    0,    0,    0,  815,  839,    0,    0,    0,
    0,  839,    0,    0,    0,  842,  842,    0,    0,  842,
    0,    0,  839,  839,    0,  839,  842,    0,  842,  842,
  779,  839,  842,    0,    0,    0,  842,    0,    0,  842,
    0,  842,    0,  842,    0,    0,    0,    0,  842,  842,
    0,  842,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  780,  784,    0,  842,  842,    0,
  842,  779,    0,    0,  779,  781,    0,    0,    0,    0,
    0,    0,    0,    0,  842,    0,  842,  842,  842,  779,
    0,  842,    0,    0,  842,  842,    0,    0,    0,    0,
    0,    0,    0,    0,  842,  780,    0,    0,  780,    0,
    0,    0,    0,    0,    0,    0,  781,    0,  842,  781,
    0,  842,  784,  780,    0,  784,    0,    0,  785,    0,
    0,    0,  784,    0,  781,  784,    0,  842,    0,  784,
    0,    0,    0,  842,  784,    0,    0,  784,  842,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  842,
  842,    0,  842,  784,    0,    0,    0,    0,  842,    0,
    0,    0,    0,    0,  814,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  785,    0,  784,  785,    0,
  784,  784,    0,    0,    0,  785,    0,    0,  785,    0,
  784,    0,  785,    0,    0,    0,    0,  785,  815,    0,
  785,    0,    0,    0,    0,  814,    0,  784,  814,    0,
    0,    0,    0,    0,    0,    0,  785,    0,    0,    0,
    0,    0,    0,  814,    0,    0,    0,    0,    0,  784,
    0,    0,    0,    0,  784,    0,    0,    0,    0,    0,
  785,    0,    0,  785,  785,    0,  784,    0,  784,    0,
    0,    0,    0,  785,  808,  815,    0,    0,  815,    0,
  816,    0,    0,    0,    0,  815,    0,    0,  815,    0,
  785,    0,  815,    0,    0,    0,    0,  815,    0,    0,
  815,    0,  779,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  785,    0,    0,  808,  815,  785,  808,    0,
    0,  816,    0,    0,  816,    0,    0,    0,    0,  785,
    0,  785,    0,  808,    0,    0,  780,    0,    0,  816,
  815,    0,    0,  815,  815,    0,    0,  781,    0,    0,
    0,    0,    0,  815,    0,    0,    0,  817,    0,  779,
    0,    0,  779,    0,    0,    0,    0,    0,    0,  779,
  815,    0,  779,    0,    0,    0,  779,    0,    0,    0,
    0,  779,    0,    0,  779,    0,    0,    0,    0,    0,
    0,    0,  815,  780,    0,    0,  780,  815,  817,    0,
  779,  817,    0,  780,  781,    0,  780,  781,    0,  815,
  780,  815,    0,    0,  781,  780,  817,  781,  780,    0,
    0,  781,    0,    0,  779,    0,  781,  779,  779,  781,
    0,    0,    0,    0,  780,    0,    0,  779,    0,    0,
    0,    0,    0,  805,    0,  781,  814,    0,    0,    0,
    0,    0,    0,    0,  779,    0,    0,    0,  780,    0,
    0,  780,  780,  809,    0,    0,    0,    0,    0,  781,
    0,  780,  781,  781,    0,    0,  779,    0,    0,    0,
    0,  779,  781,    0,  805,    0,    0,  805,  780,    0,
    0,    0,    0,  779,    0,  779,    0,    0,    0,  781,
    0,    0,  805,  814,  809,    0,  814,  809,    0,    0,
  780,    0,    0,  814,    0,  780,  814,    0,    0,    0,
  814,  781,  809,    0,    0,  814,  781,  780,  814,  780,
    0,    0,    0,    0,    0,    0,  808,    0,  781,    0,
  781,    0,  816,    0,  814,    0,  818,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  814,    0,
    0,  814,  814,    0,    0,    0,    0,    0,    0,    0,
    0,  814,    0,    0,    0,    0,    0,  818,    0,    0,
  818,  806,    0,  808,    0,    0,  808,    0,  814,  816,
    0,    0,  816,  808,    0,  818,  808,    0,    0,  816,
  808,    0,  816,    0,    0,  808,  816,    0,  808,  817,
  814,  816,    0,    0,  816,  814,    0,    0,    0,    0,
    0,    0,  806,    0,  808,  806,    0,  814,    0,  814,
  816,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  806,    0,    0,    0,    0,    0,    0,    0,  808,    0,
    0,  808,  808,    0,  816,    0,    0,  816,  816,    0,
    0,  808,    0,    0,    0,    0,  817,  816,  810,  817,
    0,    0,    0,    0,    0,    0,  817,    0,  808,  817,
    0,    0,    0,  817,  816,    0,    0,    0,  817,    0,
    0,  817,    0,    0,    0,  805,    0,    0,    0,    0,
  808,    0,    0,    0,    0,  808,  816,  817,    0,  810,
    0,  816,  810,    0,    0,  809,    0,  808,    0,  808,
    0,    0,    0,  816,    0,  816,    0,  810,    0,    0,
    0,  817,    0,    0,  817,  817,    0,    0,    0,    0,
    0,    0,    0,    0,  817,    0,    0,    0,    0,    0,
    0,    0,  805,    0,    0,  805,    0,    0,    0,    0,
    0,  817,  805,    0,    0,  805,    0,    0,    0,  805,
    0,    0,  809,    0,  805,  809,    0,  805,    0,    0,
    0,    0,  809,  817,    0,  809,    0,    0,  817,  809,
    0,    0,    0,  805,  809,    0,    0,  809,  818,    0,
  817,    0,  817,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  809,    0,    0,    0,  805,    0,    0,
  805,  805,    0,    0,    0,    0,    0,    0,    0,    0,
  805,    0,    0,    0,    0,    0,    0,  809,    0,    0,
  809,  809,    0,  806,    0,    0,    0,  805,    0,    0,
  809,    0,    0,    0,    0,  818,    0,    0,  818,    0,
    0,    0,    0,    0,    0,  818,    0,  809,  818,  805,
    0,    0,  818,    0,  805,    0,    0,  818,    0,    0,
  818,    0,    0,    0,    0,    0,  805,    0,  805,  809,
    0,    0,    0,    0,  809,    0,  818,    0,    0,    0,
  806,    0,    0,  806,    0,    0,  809,    0,  809,    0,
  806,    0,    0,  806,    0,    0,    0,  806,    0,    0,
  818,    0,  806,  818,  818,  806,    0,    0,    0,    0,
  810,    0,    0,  818,    0,    0,    0,    0,    0,    0,
    0,  806,    0,    0,    0,    0,    0,    0,    0,    0,
  818,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  806,    0,    0,  806,  806,
    0,    0,  818,    0,    0,    0,    0,  818,  806,    0,
    0,    0,    0,    0,    0,    0,    0,  810,    0,  818,
  810,  818,    0,    0,    0,  806,    0,  810,    0,    0,
  810,    0,    0,    0,  810,    0,    0,    0,    0,  810,
    0,    0,  810,    0,  221,    0,    0,  806,  221,    0,
    0,    0,  806,    0,    0,    0,    0,    0,  810,    0,
    0,    0,    0,  221,  806,    0,  806,  221,    0,    0,
  221,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  810,    0,    0,  810,  810,    0,    0,    0,
    0,    0,    0,    0,    0,  810,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,  810,    0,    0,    0,    0,  221,  221,  221,
    0,  221,  221,  221,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,  810,    0,    0,    0,    0,  810,
    0,    0,    0,    0,    0,  221,    0,  221,    0,    0,
    0,  810,    0,  810,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  221,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,  221,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
};
static short yycheck[] = {                                       9,
   16,    4,   12,   52,    4,   61,   16,  101,   18,    0,
   60,  236,  296,   60,  331,  488,  236,  130,  120,  121,
  339,  496,   59,  120,  121,  339,   82,  308,  682,  335,
  730,  525,   67,  310,  528,  534, 1217,   86,  471,  738,
  534,   51,   52,   53,   54,   55,  887,  267,   91,  244,
   60,   61,   62,  394,   64,   90,   66,   67,  253,   69,
   70,  111, 1173,  113,   80,   70,  344,  668,   78,  356,
   80,   81,   82,   89,   84,   85,   86,   87, 1002,   89,
   90,   91,   92,   54,   94,   53,  995,   92,  929,   65,
  623,  134,  625, 1002,  662,   66,   64,  995, 1003,  995,
  995,  111,  112,  113, 1002,   85, 1002, 1002,    0, 1014,
   40,  658,   88,    0,  208,  125,   45,    0,  796,   87,
  130,  729,  132,  133,  134,  726,   94,    0, 1033,  731,
  732,    0,  169,  617,    0,   40,   63,  827,  685,  236,
  610,  112,    0,    0,  677,  125,  371,  303,    0,   41,
  758,  371,   44,   58,  287,    0,   44,  194,   41,  767,
 1050,   44,  133,   40,  132,   44,    0,   59,   41,   61,
   41,   44,   59,   40,  287,   41,   59,   41,   44,   44,
   44,    0,   42,  296,  199, 1128,   59,  735,  238,   41,
   59,  386,   44,   59,  511,  512,  513,  514,  504,   45,
  294,   59,   59,   40,  946,  770,  400,   59,  218,   42,
 1051,   41,  820,  491,   59,  534,  226,    0,   44,  236,
  534,   44,  271,    0,  698,   59,   63,   44,  238,  702,
   41,   45,  242,    0,  244,  245, 1121,  287, 1123,   44,
   59,  251,   40,  253,   44,   40,   59,   45,   45,   40,
   40,   58,  274, 1360,  560, 1027,  537,   59,  264,  309,
   40,  271, 1360,    0,   41,  275,  316,   44,  318,   46,
  320,  242,   59,  323,  371,  290,   59,  287,  565,  258,
  251,  394,   59,   63,   61,  251,  296,  888,  262,   45,
  784,  329,   59,    0,  378,  332,  288,  334,    0,  309,
  321,    0,    0,  352,  275,  586,  316,  342,  318, 1360,
  320,  348,  880,  323, 1132,    0,  320, 1418,    0, 1243,
  277,  357,   59,   44,  892,  893,    0, 1232,  307,  862,
  863,  341,  342,  388, 1243,  284,  268,  406,  264,   41,
  350,    0,  352,   41,  394, 1243,   44, 1243, 1243,    0,
   16, 1532,   59,  356,  371,  285, 1371,   59,  403,  967,
   59,   59,  339,  260,  456,  449,  258,   41,    0,    0,
    0,  804,  382,  341,   59,  385,  386,   59,   41,  350,
   41,   44,  274,  393,  394,   59,  441,  397,  457,  346,
  400,  924,  402,  494,  430,  405,  321,  401,    0,  265,
   59,  446, 1417,  504,  444, 1013,  444,  304,   59,  301,
  274,  382,   45,  387,   80,  307,  437,  385,  310,  644,
 1045,  461,  393,  461,  644,  435,  397,   59,   59,   59,
   63,  402,  400, 1175,  366,   44,  402,  303,  442, 1324,
  419, 1006, 1007,    0, 1262,  467,  310, 1628,  668,  733,
  552, 1129, 1321,  319,  341,  461,    0,   59,  653,   41,
  470,  994,  436,  508,  297,  746,  443,  319,  749,  461,
 1375,  665,  509,   44, 1379,  341, 1381, 1249,  269,    0,
  310, 1350,  763,  703,  534, 1592,  412,  264,  265,  341,
 1055,  440,    0,    0, 1592, 1194,  319,  264,  332, 1680,
   44,  384,   59,  280,  321, 1360,  726, 1055,  319,  790,
   61, 1416, 1166, 1456,    0,   59,  531,  388,  555, 1173,
  412,  321,  341,  533,  534, 1415,  303,  419,  305,  539,
  459,  461,  459,   41,   41, 1636, 1717,    0,   59,  397,
  818, 1592,  319,    0,  885,  437,  329,  644,  412,  332,
 1661,   59,   59,  840,  831,   41,  461,  486,   44,  609,
 1238, 1186,  565,  397,  341, 1255,  264,  455,  455,  606,
  607, 1139,  441,   59, 1044,  467, 1060,  553,   41,  471,
  459,   44, 1487,  441,  461,  600,  455, 1492,  453,  745,
  723, 1702,  437, 1434,  461,  645,   59,  455,  455,  609,
    0,  461,   59,  467, 1078,  395,  708,  441,  618,  411,
  723,  437,  349,  459,  397,  618, 1218, 1359,  618,  502,
  733,  455,  459,  321,  411,    0,  258,  644,  461,  502,
  329,  398, 1537,  332,  432,  645,  455,  503,  648,  741,
  742,   41,  349,  653,   44,  459, 1254,    0,  461, 1554,
  455,  503,  662,  448,  461,  665,    0, 1429,  441,   59,
  670,  459,  459,  384,  461,  982,   41, 1200,  888,   44,
  453, 1370,  455,  723,  869,  307,  432,  872,  728,  459,
  262,  691,  341,  368,   59,  735,  388, 1220,  319,  699,
  670,  701,  403,  264,  704,  397,  305,  665,  397,   40,
  737,  738,   41,  459,   45,  755,   59,  717,    0,  719,
  341,  341,  722,  723,  388,   59, 1265,  368,  728, 1000,
  730, 1322,   63,  733,  734,  735,  503,  388,  410, 1046,
 1635, 1012, 1656,  783,  784, 1282,  397, 1592,   41, 1644,
   41,  343,  441,   41,    0,  755,  717, 1656,  719,  412,
  301, 1032,   44,   41,  302,  453,  455,  388, 1656,    0,
 1656, 1656,  319,  461, 1360,  260,  397,   59,  744,  140,
  995, 1396,  885,  783,  784,  995,  378, 1002, 1647, 1684,
   91, 1058, 1002,    0,  341,  329,  789,  419,  332, 1414,
   42,  339,  278,    0,  280,   47,    0,   42,  801,  432,
  344,  258,  288,   59,  406,  387,  782,  409, 1241,  819,
  441,  319,  319,    0, 1247,  278, 1470,  280,   59,  829,
  260,  329,  343,  425,  332,  288,  459,  287, 1361, 1328,
  873,  841,  262,  341,  341,  885,  871,  840,  814,  815,
  816,  302,   59,   41,  356,  263,  302,   44,  819,  451,
  307,  262,   59,  397,   41,   59,  288,   44,  829,  869,
  870,  871,  872,  873,  874, 1142,    0, 1215,   41,  874,
  880,   44,   59, 1422,  260,  885, 1640,  662,    0,  301,
  388,  388,  892,  893,    0,  406,   59,   41,  409,  397,
  397, 1591,  257,  379,  273,  313,   93,  441,  384,  308,
  309,  461,  870,  278,  425,  280,  324, 1382,    0,  453,
    0,  455, 1132,  288,    0,  401,  379,   59,  304,  319,
  264, 1685,  408,  418,    0,   59,  297,    0, 1030, 1404,
  451,  353,  332,  441,  441,  259,  946,   59,  401,   42,
   43,  341,   45,   59,   47,  408,  270,  455,  455, 1230,
   40,   41,    0, 1259,   44,  296,  442,  387,    0,  262,
    0,  262,  419, 1727,  262,   41,  277,   59,   44,   59,
  335,   61, 1156,   59,  262, 1159,  387,    0,  418,  442,
 1261,  493,  443,   59, 1457,   61,   59,  443,  388,  410,
  343,   12,  257,  332,  273, 1759, 1592,  397, 1048, 1472,
 1494,   91,  462,  463,  379, 1055,  436, 1661,  264, 1328,
  428,   59,  353, 1023, 1328,  260,  502,   59, 1243,   59,
    0, 1024,  260, 1243, 1024,  378,  401, 1037,  280,  461,
 1127, 1128, 1129,  408,  286, 1383,   59,  329, 1048,  502,
  332,  441, 1262,  452,    0, 1055,  261,  264, 1702,  388,
 1367,  459,  344,  406,  398,  455,  409, 1590,  397,  304,
  264, 1174,  341, 1039,  262,  258,  304,  442,  322,    0,
  335,  278,  425,  280,    0,    0, 1360, 1610,  319,   59,
 1613, 1057,  336, 1556,  387,   41,  387, 1063,   44,  387,
  284,  278,    0,  280, 1196,  880, 1198,  118,  451,  387,
  341,  288, 1322,   59, 1385,  397,  360,  892,    0,  378,
   41, 1121,    0, 1123,  307,   41,   41,  458,  459,  460,
  280,  462,  463,   12, 1127, 1128,  286,  502,   59, 1139,
  264,  308,  309,   59,   59,  308,  309,  259,  278,  302,
  481,  482,  264, 1424,   44,  486,  487,  388,  270,  441,
  419,   59,   40,   41,  459, 1257,   44, 1194, 1342,  262,
  414,  453, 1346,  455, 1174, 1175,  323,   59,  258,    0,
   44,   59,  264,   61,  349,  329,  339,  280,  332, 1155,
  320,  398,  258,  286,  274,  265, 1663,  277,  278,  387,
  280,  385,  379,   93,  398,  285,  323,  466,  288,  265,
  441,  277,  278,   91,  280,  278,  260,  280,    0,  285,
   41,  301,  288,   44,  401,    0,  264,  307, 1691,   93,
  310,  408,  264,  303,  264,  301,  419,  343,   59,  118,
   61,  307, 1618, 1236,  388, 1272, 1236,  303,  411, 1276,
    0,  264,    0,  397,  437, 1778,  440, 1360,  326,   41,
  304, 1210,   44,  402,    0,  442,   41,  343,  338,   44,
    0,  401,  378,  457,  398, 1378, 1743,   59,  302,  459,
 1551,  461, 1638,  260,   59,  452,  398, 1663,  471,  452,
  443,   41,  258, 1467,   44,  261,   44,  462,  463,  379,
  406, 1768,  378,  409,   44,   41,    0, 1256,  278,   59,
  280,   59,  442,  379,   44,  339,  398, 1514, 1592,  425,
 1648,  401, 1650,   59, 1324,  502,   40,  304,  408,   59,
  406, 1687,  412,  409, 1710,  401,  384, 1377, 1378,  419,
 1377,  307,  408, 1370, 1618,  451,  258,   41,  435,  425,
  262,  417,  403,  419, 1585,  267,  344,  437, 1589, 1359,
  398, 1689,  442,  431,  384,   59,  398, 1743,  398, 1456,
  438,  437, 1728,  319,  461,  451,  442, 1377, 1378,    0,
  258,  369,  386,    0,  408,  398, 1386,  467,  439, 1663,
  338,  471, 1768,  344,  418,  341,  274, 1489,  319,  277,
  278,    0,  280,    0,  319,  471,  398,  285,  418,  379,
  288, 1514, 1643,  329, 1363,  357,  332,  332,  369,  443,
  341,  319,   44,  301,    0, 1386,  341,  375,    0,  307,
  384,  329,  310, 1761,  332,  445, 1710,  258,   59,  427,
  388, 1656,   59,  341,  259,  437, 1656,  322,    0, 1495,
  218, 1535,  394,  406,  396,  270,  277,  278,  226,  280,
   59,  336,   59, 1456,  285,   41,  384,  288,   44, 1743,
 1463,   93,  388,  388, 1514,  288,  427,  419,  279,  471,
  301,  397,  397,   59,  459,  360,  307,   59,  301, 1592,
  388, 1692, 1693,    0, 1768, 1495,  278, 1536,  280,  397,
 1731,  379,  444,  285,  457,  418,  288,   59,    0,  384,
  461,  486,    0,  437, 1514, 1618,  428, 1498, 1715,  301,
  432,  433,  434,  401,    0,    0,  441,   44, 1729,  330,
  408, 1531,  445, 1560,  412,   41, 1536, 1564,   44,  414,
  455,  419,   59,  441,  319, 1629,    0,  322,  495, 1555,
 1747, 1748,   59,   41,  329, 1555,   44,  332,  379,  437,
 1663,  336,  338,  364,  442,   41,  341,   59,   44,  344,
 1531,   59,  322,  404,    0, 1772,    0,  419, 1779,  329,
  401,  382,  332,   59,   59,  360,  336,  408, 1785,  467,
  421, 1591,  322,  471,  344, 1544,  332,  379,  419,  329,
    0, 1550,  332,    0, 1604,   59,  336, 1710,  699,  384,
  360,    0, 1715,  388,  344,   41,  437,   41, 1618,  401,
   44,  442,  397,  274,   41, 1671,  408,   44,  317,  318,
  360,  722, 1638,   59,  384,   59,  272,    0, 1638,  414,
 1743,  319,    0,  734, 1747, 1748, 1604,  397,  353, 1655,
  471,  425,  388,  289,  384, 1655,   40,    0,  343,   59,
  442,  397,   59, 1663,  414, 1768,  441,  397,  379, 1772,
   59, 1671, 1672,  264,  379, 1715,    0,  466,  453,  468,
 1673, 1687, 1785, 1673,  414,    0,  461, 1687, 1694,  419,
  401,  441,    0,  378, 1694,    0,   59,  408,  334,  406,
 1739,   59,  278,  453,  280,  455,   41, 1747, 1748,   44,
 1710,  441,  288,  625,    0, 1715,   59, 1121,  425, 1123,
  319,  406, 1728,  453,  409,  455,   41,  363, 1728,   44,
 1736,  442, 1772,  332,    0,   59, 1736,    0,  322, 1739,
  425,  349,  341, 1743,   59, 1785,    0, 1747, 1748,    0,
  358,   59,  336,    0,   59,   41,  668,    0,   44,  671,
    0,  494,  495,    0,  676, 1765,  451,  462, 1768,  681,
  682, 1771, 1772,   59,   42,   43,  360,   45,  690,   47,
  692,  693,  694,  695,  696, 1785,  698,   41, 1788,  388,
   44,  703,    0,   59,  430,    0,   59,  317,  397,    0,
  384,  462,  463,  379,  279,   59,    0,   61,   59,   41,
   42,   43,   59,   45,  726,   47,   59,  319,    0,   59,
    0,  322,   59, 1732, 1733,  401,  301,    0,    0,    0,
  414,  319,  408,    0,  322,  336,   41,   91,  322,   44,
  279,  329,  441,  344,  332,  338,   41,   41,  336,   44,
   44,   59,  336,  341,   59,  330,  344,    0,   59,  360,
  344,   41,  301,    0,   44,   59,  442,   61,   41,  462,
  463,   44,  360,  842,  349,  844,  360,   59, 1787,   59,
  403,   61,  279,  358,  392,  393,   59,   59,   59,  364,
  337,  330,   59,  319,  453,  319,  384,   91,   41,   41,
  388,   44,   44,  329,  301,  329,  332,  382,  332,  397,
  349,   41,  435,  414,   44,  341,   59,  341,  260,  358,
  344,   41,   59,  403,   44,  364,  414,  450,   41,  338,
  414,   44,   40,  330,  341,  847,  848,  849,  321,  319,
  852,  853,  854,  382,  856,  857,  858,  859,  860,  861,
  403,   93,  349,  441,   42,   43,  384,   45,  507,   47,
  302,  358,  388,   93,  388,  453,  375,  364,  384,  437,
  362,  397,  314,  397,  451,  667,  888,  669,    0,  388,
  672,  673,  674,  895,  896,  382,  898,   41,  900,  435,
   44,  903,  904,  905,  906,  907,  908,  339,  418,   41,
   41,    0,   44,   44,  258,  443,  355,  358,  920,  288,
  922,   41,  280,   41,   44,  441,   44,  441,  286,   41,
  274,  412,   44,  277,  278,   41,  280,  453,   44,  453,
    0,  285,   41,  319,  288,   44,  470,   59,    0,  458,
  459,  460,   41,  329,  312,   44,  332,  301,  280,   41,
   44,   61,   44,  307,  286,  341,  310,  373,  344,  287,
   59,   40,   61,   40,  258,  264,  408,  486,  487,   41,
   44,  437,   44,  278,  462,  280,  418,   45,  258,   41,
  274,  462,   44,  288,  278,   41,  280,   41,   44,   59,
   44,  285,   91,  462,  288,   63,  301,   59,  278,   61,
  280,  443,  388,  321,   44,  285,  403,  301,  288,   40,
  459,  397,  461,  307,   42,   43,  310,   45,   40,   47,
   40,  301,   40,  472,   40,  379,   40,  307,   40,   91,
  295,   40,   40,   40,   40,  278,   40,  280,   40,   91,
  321,  306,  491,   46,   44,  288,  319,  401,  313,  322,
  499,  500,  501,   44,  408,  441,  329,  429,  412,  332,
   46,   40,   61,  336,  264,  419, 1078,  453,  341,  280,
   44,  344,   40,  338,  379,   43,   46,   45, 1090, 1091,
  383, 1093, 1094,  437,   44,  379,  349,  360,  442, 1101,
   58,  278,  280,  278,  301,   63,  401,  264,  286,  379,
  398,  437,  471,  408,  459,  398,  277,  401,  400,  448,
  448,  384,   40,  467,  408,  388,  350,  471,  412,   40,
 1132,  401,   91,   40,  397,  419,   42,   43,  408,   45,
   40,   47,  264,  321, 1146,  319,  379,  442,  403,  419,
  262,  414, 1154,  437, 1156,  384,  412, 1159,  442, 1161,
   44,  383,  390, 1165, 1166, 1167, 1168,  437,  401,  258,
   40, 1173,  442,  329,   40,  408,  318,  437,  441,  465,
  435,   40,  321,  467,  439,  274,  321,  471,  452,  278,
  453,  280,   41,  264,  453,  449,  285,  455,   40,  288,
  277,  471,  274,   40,  301,  285,  258,  319,  459,  442,
  322, 1213,  301,  278,  387,    0,  345,  329,  307,  279,
  332,  310,  274,  387,  336,  387,  278,  419,  280,  341,
  345,  417,  344,  285,  419,  301,  288,    0,  296,   42,
   43,  301,   45,   40,   47,   40,  357,  403,  360,  301,
  418,  274,  418,   40,   44,  307,   41,  264,  310,   44,
 1262,   40,  280,  386,   43,  368,   45,   44,  286,   41,
  330,  321,  384,   44,   59,  387,  388, 1279,   41,   58,
  321,   44,   44,  439,   63,  397, 1288,   40,  444,  349,
  379,  329,  280, 1295,  450,  353,   59,  357,  358,   44,
   44,  503,  414,  321,  364,  461,  506,   40,  502,  267,
  266,   40,  401,  469,  300,  264,  423,   41,  276,  408,
 1322,   58,  382,  412,  436,   41,   41,  379,   41,  441,
  419,  264,  290,  441,  394,  293,  396,  310,  296,  297,
 1342,  453,  455,  455, 1346,  493,  395,  310,  437,  401,
   41,   41,    0,  442,   40,   40,  408,  315,  353,  419,
  412,   40,   40,   40,   40,   40,  324,  419,   40,  327,
   40,   40,   40,   40,  280,   40,    0,   40,  467,   40,
  286,   40,  471,  262,  444,  437,  328,   44,  436,  309,
  442,   41,    0,   41,  274,  332,   44,  355,  356,  344,
  458,  459,  460,  423,  462,  463,  312,  389,  391,  367,
  389,   59,  389,  371,  392,  467,  447,   41,  271,  471,
   44,  379,  387,  481,  482,    0,  288,   41,  486,  487,
  387,   41,  387,   41,  408,   59,   44,  278,  379,  387,
 1442, 1443,   40,  387,  387,   41,   41,   42,   43,   41,
   45,   59,   47,   41,  386,   41,   41,   42,   43,  262,
   45,  309,   47,  397,  422, 1467,   41,  418, 1470,   44,
  428,  437,  443,  338,  436,  339,  434,  280,  302,  339,
  302,   41,  443,  286,   59,   41,  444,  321,  267,  386,
   41,   42,   43,  278,   45,  280,   47,  276,  353, 1501,
  458,  459,  460,  461,  462,  463,   41,   42,   43,  372,
   45,  290,   47,   41,  472,  372,   41,  296,  297,   41,
   41,   41,   41,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   40,  491,   42,   43,  315,   45,  496,  497,
   41,  499,  500,  501,   41,  324,   41,  505,  327,   41,
   58,   41,   41,   41,   41,   63,  319, 1559,   41,  322,
   41,  280,   44,   59,   40, 1567,  329,  341,   59,  332,
   40,  341,  353,  336,   59,   59,  355,  356,  341,   42,
   43,  344,   45,  309,   47,   41,   42,   43,  367,   45,
   59,   47,  371,   59,  379,   59,   59,  360,   59,   41,
  379, 1603,  345,   41,  369, 1607,   40,  341,   40,  452,
   42,   43,   59,   45,   44,  384,  401,  384,  384,   41,
   41,  384,  344,  408,  387,  388,   58,   41,   44,  411,
  278,   63,  280,   44,  397,  436,  302,  285,  443,  418,
  288,  305,  388,  422,   40,  294,   41,   42,   43,  428,
   45,  414,   47,  301,  278,  434,  280,  442,   59, 1661,
  305,   44,  464,   44,  288,  444,  308,   41,    0,   40,
  278,  264,  280,  436,  384,  384,  338,  301,  441,  458,
  459,  460,  461,  462,  463,  280,  461,  262,  278,  390,
  453,  286,  455,  472,  264,  280,  321,  261,  459,  264,
 1702,  286,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  471,  491,  412,  452,   40,  310,  496,  497,  283,
  499,  500,  501,  459,  271,  264,  505,  291,  436,  280,
  437,  379,  344,  344,  298,  286,  300,  344,  302,  344,
  461,  459,  271,  307,  319,  280,  309,  322,   41,   40,
  314,  286,  260,  401,  329,  379,  309,  332,   40,  267,
  408,  336,   42,   43,  328,   45,  341,   47,  276,  344,
   59,  379,   41,   42,   43,  339,   45,  401,   47,   59,
   41,   59,  290,  309,  408,  360,   41,   40,  296,  297,
   43,  305,   45,  401,  442,   44,  304,   59,   41,    0,
  408,   42,   43,    0,   45,   58,   47,    0,    0,  384,
   63, 1124,  387,  388,   12,  246,  324,  280,  442,  327,
  253,  245,  397,  286,  280,  653, 1060,  613,  260,  832,
  286, 1047, 1058,   84,  442,  267, 1055, 1405,  306,  414,
  560,  301,  406,  336,  276,  353, 1037,  355,  356,  413,
  728,  985,  416,  292,  418,  419,  318, 1601,  290,  367,
 1048,  436,   59,  371,  296,  297,  441,  318, 1383,  765,
 1678,   40,  304, 1385,   43, 1682,   45, 1719,  453,  443,
  455,   42,   43,   44,   45,  280,   47,  539, 1681,   58,
 1211,  286,  324, 1771,   63,  327, 1623,   41,   42,   43,
 1716,   45, 1364,   47,   41,   42,   43, 1188,   45,  755,
   47,   41,   42,   43,  371,   45,  969,   47, 1638,  323,
  428,  353,  311,  355,  356,  541,  434,  304,  492,  811,
  260, 1245,  803,  405,   13,  367,  444,  279,  972,  371,
  852,  599,  854,  507,  856,  857,  473,  859,  860,  861,
  458,  459,  460,  461,  462,  463,   41,   42,   43,  477,
   45,   45,   47,  717,  472,  733,  123, 1592,  703, 1736,
 1694,  946,  302,  481,  482,  483,  484,  485,  486,  487,
  488,  489, 1485,  491,  314, 1788,  488,  704,  496,  497,
  661,  499,  500,  501,  889,  370,  428,  505,  328, 1092,
  280, 1161,  434,   -1,   -1,   -1,  286,   -1,   -1,  339,
   -1,  280,  444,   -1,  267,   -1,   -1,  286,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,  458,  459,  460,  461,
  462,  463,   41,   42,   43,   -1,   45,  290,   47,  280,
  472,   -1,   -1,  296,  297,  286,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,  258,  315,   -1,  496,  497,   -1,  499,  500,  501,
   -1,  324,   -1,  505,  327,   -1,    0,   -1,  408,   -1,
   -1,   -1,  279,   -1,   -1,   -1,   -1,    0,  418,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,  355,  356,  301,   -1,   40,  276,   -1,   43,
  307,   45,   -1,  443,  367,   -1,   -1,   41,  371,  280,
   44,  290,   -1,   -1,   58,  286,  379,  296,  297,   63,
   -1,  461,   -1,  330,   -1,   59,  280,   -1,  260,   -1,
   -1,   -1,  286,  280,   -1,   -1,   59,   -1,   -1,  286,
  280,   -1,  349,   -1,   -1,  324,  286,   -1,  327,   -1,
   -1,  358,   -1,   -1,   -1,   -1,   -1,  364,   -1,  422,
   41,   42,   43,   -1,   45,  428,   47,   -1,   -1,   -1,
  302,  434,   -1,   -1,   -1,  382,  355,  356,   -1,   -1,
   -1,  444,  314,   -1,   -1,  280,   -1,   -1,  367,   -1,
   -1,  286,  371,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,  296,   -1,   41,   42,   43,  339,   45,  472,
   47,   -1,  419,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,
   -1,   -1,  505,   -1,   41,   42,   43,  426,   45,  428,
   47,   -1,   41,   42,   43,  434,   45,   -1,   47,  353,
   -1,  280,   -1,   -1,   -1,  444,   -1,  286,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,  408,   -1,   -1,  458,
  459,  460,  461,  462,  463,   59,  418,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  443,  491,   -1,   -1,   -1,  260,  496,  497,  263,
  499,  500,  501,  267,  258,   -1,  505,    0,   -1,  461,
   -1,   40,  276,   -1,   43,  258,   45,  473,  474,  475,
  476,  477,  478,  479,  480,  279,  290,   -1,   -1,   58,
  444,   -1,  296,  297,   63,   -1,  279,   -1,   -1,   -1,
   59,   -1,   -1,   -1,  458,  459,  460,  301,  462,  463,
   -1,   -1,   -1,  307,   -1,   -1,   -1,   -1,  301,   -1,
  324,   -1,   -1,  327,  307,   -1,   59,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  330,   -1,   -1,  280,
   -1,   -1,  496,  497,   -1,  286,   -1,  330,   -1,   -1,
   -1,  355,  356,   40,   -1,  349,   43,   -1,   45,   -1,
   -1,   -1,   -1,  367,  358,   -1,  349,  371,   -1,   -1,
  364,   58,   -1,   -1,  357,  358,   63,   -1,   -1,   -1,
   -1,  364,   -1,  280,   -1,   -1,   -1,   -1,  382,  286,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  382,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  295,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  306,  426,  280,  428,  419,   -1,   -1,  313,  286,
  434,  280,   -1,   -1,   -1,   -1,  419,  286,   -1,   -1,
  444,  326,   -1,   -1,   -1,   -1,  280,  430,   -1,   -1,
   -1,   -1,  286,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,  260,  496,  497,  263,  499,  500,  501,  267,  258,
   -1,  505,  387,   -1,   -1,   -1,   40,  276,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  403,   -1,
  279,  290,   -1,   -1,   58,  258,   -1,  296,  297,   63,
   -1,   -1,   -1,   -1,   -1,  420,   -1,   -1,   -1,   -1,
   -1,   -1,  301,   -1,   -1,   -1,  279,   -1,  307,   -1,
  435,   -1,   -1,   -1,  439,  324,   -1,  442,  327,   -1,
   -1,   -1,   -1,   -1,   -1,  450,   -1,   -1,  301,   -1,
   -1,  330,   -1,  260,  307,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,  469,   -1,  355,  356,   40,  276,
  349,   43,   -1,   45,   -1,   -1,   -1,  330,  367,  358,
   -1,   -1,  371,  290,   -1,  364,   58,   -1,   -1,  296,
  297,   63,   -1,   -1,   -1,   -1,  349,   -1,   -1,   -1,
   -1,   -1,   -1,  382,   -1,  358,   -1,   -1,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  382,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,
  419,   -1,   -1,   -1,   -1,  434,   -1,   -1,  355,  356,
   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,  419,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,
  499,  500,  501,  267,   -1,   -1,  505,   -1,  306,  426,
   -1,  428,  276,   -1,   -1,  313,   -1,  434,   -1,  317,
  318,   -1,   -1,   -1,   -1,   -1,  290,  444,  326,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
  338,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  260,  496,
  497,  263,  499,  500,  501,  267,   -1,   -1,  505,   -1,
   -1,  355,  356,   40,  276,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  367,   -1,  403,   -1,  371,  290,   -1,
   -1,   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,
   -1,   -1,  420,   -1,   -1,   -1,   -1,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,  435,   -1,   -1,
   -1,  439,  324,   -1,   -1,  327,   -1,   60,   61,   62,
   -1,   -1,  450,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,  469,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,  426,   -1,  428, 1280, 1281,   -1,
   -1,   -1,  434, 1286, 1287,   -1, 1289, 1290,   -1,   -1,
   -1,   -1,  444,   -1, 1297, 1298, 1299, 1300,   -1, 1302,
 1303, 1304, 1305, 1306, 1307,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   -1,   -1,  260,  496,  497,  263,  499,  500,  501,
  267,   -1,   -1,  505,   -1,   -1,   -1,   -1,   40,  276,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,  290,   -1,   -1,   58,   -1,   -1,  296,
  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,
   -1,   -1,   -1,  286,   -1,   -1,  289,  324,   -1,   -1,
  327,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,  355,  356,
   58,   -1,  325,   -1,   -1,   63,   -1,   -1,   -1,   -1,
  367,  334,   -1,   -1,  371,   -1,   -1,   -1,  271,  342,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  361,   -1,
  363,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,
   -1,   -1,   -1,   -1,  377,   -1,  379,  380,  381,   -1,
  313,  314,   -1,  316,   -1,   -1,  319,   -1,   -1,  426,
   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,
  333,   -1,   -1,   -1,   -1,   -1,  339,  444,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,  430,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,   -1,   -1,  260,  496,
  497,  263,  499,  500,  501,  267,  399,   -1,  505,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,  418,   45,   -1,  290,   -1,
   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,   58,
  433,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,  452,
   -1,  454,  324,   -1,   -1,  327,   -1,   -1,  461,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,  290,  355,  356,   -1,   -1,  490,  296,  297,
   -1,   -1,   -1,   58,   -1,  367,   -1,   -1,   63,  371,
   -1,  319,   -1,   -1,  322,   -1,  509,   -1,   -1,   -1,
   -1,  329,   -1,   -1,  332,   -1,  324,   -1,  336,  327,
   -1,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,   -1,   -1,  353,   -1,  355,  356,   -1,
   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,   -1,  367,
   -1,   -1,  434,  371,   -1,   -1,  384,   -1,   -1,  387,
  388,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,  397,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,  414,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  436,  491,
  428,   -1,   -1,  441,  496,  497,  434,  499,  500,  501,
   -1,   -1,   -1,  505,   -1,  453,  444,  455,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,  472,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,
   -1,  499,  500,  501,   -1,  324,   -1,  505,  327,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  353,  290,  355,  356,   58,   -1,
   -1,  296,  297,   63,   -1,   -1,   -1,   -1,  367,   -1,
   -1,   -1,  371,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
  271,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  353,  418,
  355,  356,   58,   -1,   -1,   -1,   -1,   63,   -1,  428,
   -1,   -1,  367,   -1,   -1,  434,  371,   -1,   -1,   -1,
   -1,  302,   -1,   -1,   -1,  444,   -1,   -1,  309,   -1,
   -1,   -1,  313,  314,   -1,  316,   -1,   -1,  319,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  333,  472,  271,   -1,   -1,   -1,  339,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,  428,   -1,   -1,   -1,  496,  497,  434,
  499,  500,  501,   -1,   -1,  302,  505,   -1,   -1,  444,
   -1,   -1,   -1,   -1,   -1,   -1,  313,  314,   -1,  316,
   -1,   -1,  319,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  333,  472,  399,   -1,
   -1,   -1,  339,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,  418,   -1,   -1,
  260,  496,  497,   -1,  499,  500,  501,  267,   -1,   -1,
  505,   -1,  433,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,  443,   40,   -1,   -1,   43,   -1,   45,   -1,
  290,  452,   -1,  454,   -1,   -1,  296,  297,   -1,   -1,
  461,   58,  399,   -1,  304,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  418,   -1,   -1,  324,   -1,   -1,  327,   -1,  490,
   -1,  267,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,  443,   40,  509,   -1,
   43,   -1,   45,   -1,  290,  355,  356,  454,   -1,   -1,
  296,  297,   -1,   -1,  461,   58,   -1,  367,   -1,   -1,
   63,  371,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  461,   -1,  324,   -1,
   -1,  327,   -1,  490,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
  485,   -1,  509,  488,  489,  490,  491,   -1,  493,  355,
  356,  496,  497,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,  367,   -1,   -1,  434,  371,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,  428,   -1,   -1,   -1,  496,  497,  434,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,  444,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,  276,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,  290,   -1,   -1,  472,   -1,   -1,  296,
  297,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,  324,   -1,  505,
  327,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   40,   41,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,  355,  356,
   -1,   -1,   58,  296,  297,   -1,   -1,   63,   -1,   -1,
  367,   -1,   -1,   -1,  371,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,   -1,  327,   -1,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,
  353,  418,  355,  356,   -1,   41,   -1,   40,   44,   -1,
   43,  428,   45,   -1,  367,   -1,   -1,  434,  371,   -1,
   -1,   -1,   -1,   59,   -1,   58,   -1,  444,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,  428,   -1,   -1,   -1,  496,
  497,  434,  499,  500,  501,   -1,   -1,   -1,  505,   -1,
   -1,  444,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,  292,   -1,   -1,   -1,  296,   -1,  472,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,
   -1,  267,  505,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,  340,   -1,   -1,   -1,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,  290,  354,   -1,   -1,   -1,   -1,
  296,  297,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  374,   -1,  376,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  272,   -1,  324,   -1,
   63,  327,   -1,   -1,  280,   -1,   -1,   -1,   -1,   -1,
  286,   -1,   -1,  289,   -1,   -1,  262,   -1,  407,   -1,
   -1,   -1,   -1,  412,  267,   -1,   -1,   -1,   -1,  355,
  356,   -1,   -1,  276,   -1,  424,   -1,   -1,   -1,   -1,
   -1,  367,   -1,   -1,   -1,  371,   -1,  290,   -1,  325,
   -1,   -1,   -1,  296,  297,   -1,   -1,  446,  334,   40,
   -1,   -1,   43,   -1,   45,   -1,  342,   -1,   -1,   -1,
   -1,   -1,   -1,  319,   -1,   -1,  322,   58,   -1,   -1,
   -1,  324,   63,  329,  327,  361,  332,  363,   -1,   -1,
  336,   -1,  481,  482,   -1,  341,   -1,   -1,  344,   -1,
   -1,  377,  428,  379,  380,  381,   -1,   -1,  434,  498,
   -1,   -1,  355,  356,  360,   -1,   -1,   -1,  444,   -1,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,  384,   -1,
   -1,  387,  388,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,  397,   -1,   -1,  430,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,  414,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,
  436,  434,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,
   -1,  444,   -1,   -1,  267,   -1,   -1,  453,   -1,  455,
   -1,   -1,  455,  276,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,  472,
    0,   -1,   -1,  296,  297,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,  273,
   -1,  324,  505,  277,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,  292,   -1,
   -1,   -1,  296,   -1,   -1,  299,  267,   -1,   -1,   59,
   -1,   -1,  355,  356,   40,  276,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,  290,
   -1,   -1,   58,   59,   -1,  296,  297,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  340,   -1,   -1,   -1,
   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,
  354,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,
  374,   -1,  376,   -1,   -1,  428,   -1,   -1,   -1,   -1,
   -1,  434,   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,
   -1,  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,
  371,   -1,   -1,  407,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
  424,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,  446,  496,  497,   -1,  499,  500,  501,   -1,
   -1,   -1,  505,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,   -1,   -1,  481,  482,   -1,
   -1,   -1,   -1,   -1,  455,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,  498,   -1,   -1,   -1,   -1,   -1,
   -1,  472,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,  267,   40,   -1,  505,   43,   -1,   45,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,  290,   63,   -1,   -1,   -1,  319,
  296,  297,  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,
  356,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,   40,
   -1,  367,   43,   -1,   45,  371,   -1,  397,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  461,   -1,   63,   -1,  414,   -1,   -1,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,  485,   -1,  436,  488,  489,  490,
  491,  441,  493,   -1,   -1,  496,  497,   -1,   -1,   -1,
   -1,   -1,  428,  453,   -1,  455,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,  267,
  496,  497,   -1,  499,  500,  501,   -1,   -1,  276,  505,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,
   -1,   -1,   -1,   -1,  461,   -1,  324,   -1,   -1,  327,
  296,   -1,   -1,  299,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,  485,   -1,
   -1,  488,  489,  490,  491,  353,  493,  355,  356,  496,
  497,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,  367,
   -1,   -1,   -1,  371,  340,  276,   -1,   -1,   -1,   -1,
  346,  347,  348,   -1,  350,  351,  352,   -1,  354,  290,
   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  374,   -1,
  376,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,
  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,  407,   -1,   -1,   -1,   -1,  444,   -1,  267,   -1,
   -1,   -1,   -1,   -1,  355,  356,   -1,  276,  424,   -1,
  458,  459,  460,  461,  462,  463,  367,   -1,   -1,   -1,
  371,  290,   -1,   -1,  472,   -1,   -1,  296,  297,    0,
  446,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,  461,   -1,   -1,  496,  497,
   -1,  499,  500,  501,   -1,  324,   -1,  505,  327,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,   -1,   40,
   41,   42,   43,   44,   45,   46,   47,  428,   -1,   -1,
   -1,   -1,  498,  434,   -1,   -1,  355,  356,   59,   60,
   61,   62,   -1,  444,   -1,   -1,   -1,   -1,  367,   -1,
   -1,   -1,  371,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   91,  472,   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,
   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   93,  264,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,
   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,  308,  309,   -1,
   -1,  312,   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,
   -1,  332,   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,
  341,  342,   -1,  344,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,  360,
  361,   -1,  363,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  377,   -1,  379,  380,
  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,
   -1,   -1,   -1,   -1,  273,   -1,  397,   -1,  277,   93,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  411,   -1,   -1,  414,   -1,   -1,   -1,  296,   -1,   -1,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,
   -1,  262,   -1,  264,   -1,  436,   -1,   -1,   -1,   -1,
  441,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,
   -1,  452,  453,   -1,  455,  286,   -1,   -1,  289,   -1,
  461,  340,   -1,   -1,   -1,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,   -1,  354,    0,  308,  309,   -1,
   -1,  312,   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,
  321,  322,   -1,   -1,  325,  374,   -1,  376,  329,   -1,
   -1,  332,   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,
  341,  342,   -1,  344,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,   -1,   -1,   -1,  407,  360,
  361,   -1,  363,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,   -1,   -1,   -1,   -1,  424,  377,   -1,  379,  380,
  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  397,  446,  262,   93,
  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
  411,   -1,  461,  414,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,  289,   -1,   -1,   -1,  430,
   -1,   -1,  481,  482,   -1,  436,   -1,   -1,   -1,   -1,
  441,   -1,   -1,    0,  308,  309,   -1,   -1,  312,  498,
   -1,  452,  453,   -1,  455,  319,   -1,  321,  322,   -1,
  461,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,   -1,
  344,   -1,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,  360,  361,   -1,  363,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  377,   -1,  379,  380,  381,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,
   -1,  273,   -1,  397,   -1,  277,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,   -1,
  414,   -1,   -1,   -1,  296,   -1,   -1,  299,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  430,   -1,  262,   -1,
  264,   -1,  436,   -1,   -1,   -1,   -1,  441,  272,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,  453,
   -1,  455,   -1,   -1,   -1,  289,   -1,  461,  340,   -1,
   -1,   -1,   -1,   -1,  346,  347,  348,   -1,  350,  351,
  352,   -1,  354,    0,  308,  309,   -1,   -1,  312,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,  321,  322,   -1,
   -1,  325,  374,   -1,  376,  329,   -1,   -1,  332,   -1,
  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,   -1,
  344,   -1,   -1,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,   -1,   -1,   -1,  407,  360,  361,   -1,  363,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,  424,  377,   -1,  379,  380,  381,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  397,  446,  262,   93,  264,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,  411,   -1,   -1,
  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,  430,   -1,   -1,  481,
  482,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,
    0,  308,  309,   -1,   -1,  312,  498,   -1,  452,  453,
   -1,  455,  319,   -1,  321,  322,   -1,  461,  325,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  334,   -1,  336,
   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,  361,   -1,  363,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  377,   -1,  379,  380,  381,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
  397,   -1,  277,   93,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  411,   -1,   -1,  414,   -1,   -1,
   -1,  296,   -1,   -1,  299,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  430,   -1,  262,   -1,  264,   -1,  436,
   -1,   -1,   -1,   -1,  441,  272,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,  453,   -1,  455,   -1,
   -1,   -1,  289,   -1,  461,  340,   -1,   -1,   -1,   -1,
   -1,  346,  347,  348,   -1,  350,  351,  352,   -1,  354,
    0,  308,  309,   -1,   -1,  312,   -1,   -1,   -1,   -1,
   -1,   -1,  319,   -1,  321,  322,   -1,   -1,  325,  374,
   -1,  376,  329,   -1,   -1,  332,   -1,  334,   -1,  336,
   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   -1,   -1,
   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,   -1,
   -1,   -1,  407,  360,  361,   -1,  363,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,  424,
  377,   -1,  379,  380,  381,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  397,  446,  262,   93,  264,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,  411,   -1,   -1,  414,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,  430,   -1,   -1,  481,  482,   -1,  436,
   -1,   -1,   -1,   -1,  441,   -1,   -1,    0,  308,  309,
   -1,   -1,  312,  498,   -1,  452,  453,   -1,  455,  319,
   -1,  321,  322,   -1,  461,  325,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,  334,   -1,  336,   -1,   -1,   -1,
   -1,  341,  342,   -1,  344,   -1,   -1,   -1,   41,   -1,
   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,  361,   -1,  363,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  377,   -1,  379,
  380,  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   93,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  411,   -1,   -1,  414,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  430,   -1,  262,   -1,  264,   -1,  436,   -1,   -1,   -1,
   -1,  441,  272,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  452,  453,   -1,  455,   -1,   -1,   -1,  289,
   -1,  461,   59,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  308,  309,
   -1,   -1,  312,   -1,   -1,   -1,   -1,   -1,   -1,  319,
   -1,  321,  322,   -1,   -1,  325,   -1,   -1,   -1,  329,
   -1,   -1,  332,   -1,  334,   -1,  336,   41,   -1,   -1,
   44,  341,  342,   -1,  344,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
  360,  361,   -1,  363,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,  377,   -1,  379,
  380,  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,  262,
   -1,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,  411,   -1,   -1,  414,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
  430,   -1,   -1,   -1,   -1,   59,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,  308,  309,   -1,   -1,  312,
   -1,   -1,  452,  453,   -1,  455,  319,   -1,  321,  322,
    0,  461,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,
   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  262,   -1,  360,  361,   -1,
  363,   41,   -1,   -1,   44,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  377,   -1,  379,  380,  381,   59,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,  411,   44,
   -1,  414,  319,   59,   -1,  322,   -1,   -1,  262,   -1,
   -1,   -1,  329,   -1,   59,  332,   -1,  430,   -1,  336,
   -1,   -1,   -1,  436,  341,   -1,   -1,  344,  441,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
  453,   -1,  455,  360,   -1,   -1,   -1,   -1,  461,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,  384,  322,   -1,
  387,  388,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
  397,   -1,  336,   -1,   -1,   -1,   -1,  341,  262,   -1,
  344,   -1,   -1,   -1,   -1,   41,   -1,  414,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,  453,   -1,  455,   -1,
   -1,   -1,   -1,  397,    0,  319,   -1,   -1,  322,   -1,
    0,   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
  414,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,   -1,
  344,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   41,  360,  441,   44,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  453,
   -1,  455,   -1,   59,   -1,   -1,  262,   -1,   -1,   59,
  384,   -1,   -1,  387,  388,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,  397,   -1,   -1,   -1,    0,   -1,  319,
   -1,   -1,  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,
  414,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,
   -1,  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  436,  319,   -1,   -1,  322,  441,   41,   -1,
  360,   44,   -1,  329,  319,   -1,  332,  322,   -1,  453,
  336,  455,   -1,   -1,  329,  341,   59,  332,  344,   -1,
   -1,  336,   -1,   -1,  384,   -1,  341,  387,  388,  344,
   -1,   -1,   -1,   -1,  360,   -1,   -1,  397,   -1,   -1,
   -1,   -1,   -1,    0,   -1,  360,  262,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  414,   -1,   -1,   -1,  384,   -1,
   -1,  387,  388,    0,   -1,   -1,   -1,   -1,   -1,  384,
   -1,  397,  387,  388,   -1,   -1,  436,   -1,   -1,   -1,
   -1,  441,  397,   -1,   41,   -1,   -1,   44,  414,   -1,
   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,   -1,  414,
   -1,   -1,   59,  319,   41,   -1,  322,   44,   -1,   -1,
  436,   -1,   -1,  329,   -1,  441,  332,   -1,   -1,   -1,
  336,  436,   59,   -1,   -1,  341,  441,  453,  344,  455,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  453,   -1,
  455,   -1,  262,   -1,  360,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,   -1,
   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  397,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,    0,   -1,  319,   -1,   -1,  322,   -1,  414,  319,
   -1,   -1,  322,  329,   -1,   59,  332,   -1,   -1,  329,
  336,   -1,  332,   -1,   -1,  341,  336,   -1,  344,  262,
  436,  341,   -1,   -1,  344,  441,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,  360,   44,   -1,  453,   -1,  455,
  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,   -1,
   -1,  387,  388,   -1,  384,   -1,   -1,  387,  388,   -1,
   -1,  397,   -1,   -1,   -1,   -1,  319,  397,    0,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,  414,  332,
   -1,   -1,   -1,  336,  414,   -1,   -1,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,  436,  360,   -1,   41,
   -1,  441,   44,   -1,   -1,  262,   -1,  453,   -1,  455,
   -1,   -1,   -1,  453,   -1,  455,   -1,   59,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
   -1,  414,  329,   -1,   -1,  332,   -1,   -1,   -1,  336,
   -1,   -1,  319,   -1,  341,  322,   -1,  344,   -1,   -1,
   -1,   -1,  329,  436,   -1,  332,   -1,   -1,  441,  336,
   -1,   -1,   -1,  360,  341,   -1,   -1,  344,  262,   -1,
  453,   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  397,   -1,   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,  262,   -1,   -1,   -1,  414,   -1,   -1,
  397,   -1,   -1,   -1,   -1,  319,   -1,   -1,  322,   -1,
   -1,   -1,   -1,   -1,   -1,  329,   -1,  414,  332,  436,
   -1,   -1,  336,   -1,  441,   -1,   -1,  341,   -1,   -1,
  344,   -1,   -1,   -1,   -1,   -1,  453,   -1,  455,  436,
   -1,   -1,   -1,   -1,  441,   -1,  360,   -1,   -1,   -1,
  319,   -1,   -1,  322,   -1,   -1,  453,   -1,  455,   -1,
  329,   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
  384,   -1,  341,  387,  388,  344,   -1,   -1,   -1,   -1,
  262,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  360,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,  397,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,  453,
  322,  455,   -1,   -1,   -1,  414,   -1,  329,   -1,   -1,
  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,
   -1,   -1,  344,   -1,  273,   -1,   -1,  436,  277,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,   -1,  292,  453,   -1,  455,  296,   -1,   -1,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  340,  414,   -1,   -1,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,  374,   -1,  376,   -1,   -1,
   -1,  453,   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  498,
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
