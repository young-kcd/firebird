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
  192,  193,  200,  200,  194,  195,  195,  196,  203,  198,
  198,  198,  199,  199,  199,  201,  201,  201,  201,  197,
  202,  202,  188,  188,  208,  209,  209,  210,  210,  210,
  210,   11,  212,  212,  212,  213,  213,  213,  213,  213,
  213,   72,   81,  218,  219,  215,  220,  220,  220,  221,
  224,  229,  229,  230,  230,  231,  231,  148,  217,   88,
   90,  216,  216,   71,   84,  234,  234,  234,  235,  238,
  238,  239,  239,  239,  239,  239,  239,  239,  239,  239,
  239,  239,  239,  239,  239,  239,  236,  236,  237,    2,
  241,  241,  241,  241,  241,  241,  241,  249,  250,  247,
  247,  251,  251,  251,  251,  251,  251,  251,  242,  242,
  252,  252,  252,  252,  252,  252,  252,  246,  257,  257,
  257,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  257,  254,  254,  256,  256,  255,  253,  253,  253,
  248,  248,  244,  245,  245,  258,  258,  258,  258,  259,
  259,  243,  260,  260,  261,  261,    8,  262,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  262,  262,  102,
  102,  145,  145,  265,  265,  268,  268,  270,  270,   54,
   54,  267,  267,  267,  267,  267,  267,  267,  267,  267,
  275,  275,  266,  266,  266,  266,  276,  276,   62,   62,
   62,   56,   56,  272,  272,  272,  269,  269,  269,  280,
  280,  280,  279,  279,  278,  278,  278,  273,  273,  283,
  281,  281,  281,  282,  282,  274,  274,  274,  274,  284,
  284,   19,   19,   19,  286,  286,  286,   16,   17,  290,
  290,    4,   15,  289,  289,  291,  291,  292,  292,  285,
  293,  293,  294,  294,  295,  295,  295,  295,  296,  296,
  297,  297,  298,  298,  300,  300,  300,  301,  301,  301,
  302,  302,  302,  299,  304,  304,  304,  305,  305,  303,
  303,  306,  308,  308,  307,  307,  287,   18,  309,  309,
  309,  310,  310,  313,  313,  314,  314,   66,   66,   66,
  315,  315,  315,  311,  311,  316,  316,  317,  317,  312,
  320,  318,  318,  318,  318,  321,  321,  321,  322,  322,
  322,  222,  222,  223,  223,  324,  324,  325,  325,  325,
  319,  328,  328,  329,  329,  330,  330,  331,  331,  332,
  332,  205,  205,  334,  334,  232,  232,   32,  233,  233,
  233,  233,  233,  233,  233,  233,  226,  226,  335,  335,
  336,  336,  336,  336,  336,  338,  338,  338,  227,  227,
  225,  225,  342,  342,  342,  228,  228,  343,  344,  344,
  344,  344,  344,  345,  345,  346,  346,  347,  347,  348,
  348,  348,  349,  349,   10,   10,  351,  351,    7,    7,
  352,  353,  354,   20,   20,  355,  356,  357,  357,  189,
  326,  326,    3,    3,  359,  359,  359,  361,  361,   62,
  360,  360,  362,  362,   35,   35,   86,   85,   85,  350,
  350,  363,  364,  364,  206,  206,  206,  147,  358,  358,
  112,  112,  112,  112,  366,  366,  366,  366,  366,  366,
  366,  366,  366,  366,  366,  367,  367,  367,  367,  367,
  367,  367,  367,  372,  372,  372,  372,  372,  372,  372,
  372,  372,  372,  372,  372,  372,  372,  372,  372,  378,
  378,  368,  368,  369,  369,  369,  369,  370,  370,  374,
  374,  375,  375,  375,  375,  373,  376,  371,  371,  379,
  379,  380,  377,  382,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,   89,   89,   89,   89,   89,   89,   89,   89,   89,
   89,  157,  157,  157,  383,  381,  381,  152,  152,  388,
  388,  388,  388,  388,  385,  385,  385,  385,  385,  214,
  153,  153,  154,  155,  155,  387,  387,  387,   52,   52,
   21,   21,  240,  389,   55,  124,  271,  271,   96,  384,
  384,  384,  384,  390,  390,  390,  390,  390,  390,  390,
  390,  390,  390,  390,  391,  339,  340,  340,  340,  337,
  337,  386,  341,  341,  393,  393,  394,  394,  395,  395,
  398,  398,  396,  396,  401,  401,  400,  397,  399,  392,
  392,  392,  392,  392,  392,  392,  392,  323,  323,  156,
   49,  277,  123,  149,  365,  166,  207,  264,   64,   61,
  211,  113,   67,  327,   36,   42,  333,  263,   40,   43,
  204,   41,  288,
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
    3,    1,    2,    1,    2,    2,    2,    3,    4,    2,
    6,    4,    1,    1,    7,    8,    6,    4,    2,    1,
    3,    0,    2,    4,    0,    1,    1,    3,    3,    6,
    3,    0,    1,    2,    4,    1,    3,    2,    2,    2,
    1,    4,    1,    3,    0,    1,    1,    1,    3,    3,
    3,    7,    7,    7,    7,    1,    1,    3,    4,    8,
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
    4,    3,    0,    1,    3,    4,    4,    1,    1,    0,
    2,    2,    0,    4,    0,    2,    0,    2,    0,    9,
   10,    2,    1,    1,    0,    2,    4,    2,    2,    4,
    2,    1,    1,    1,    1,    1,    3,    1,    2,    3,
    2,    1,    3,    1,    1,    6,    3,    3,    2,    3,
    0,    1,    3,    1,    1,    1,    2,    1,    1,    1,
    2,    1,    2,    1,    2,    0,    3,    0,    1,    3,
    1,    1,    1,    1,    3,    1,    1,    1,    2,    0,
    2,    0,    3,    5,    0,    2,    0,    4,    1,    2,
    1,    1,    0,    1,    3,    2,    1,    1,    2,    1,
    4,    2,    1,    3,    8,    5,    1,    3,    1,    1,
    4,    4,    4,    1,    1,    5,    5,    1,    3,    3,
    1,    1,    7,    7,    5,    3,    0,    1,    1,    1,
    2,    0,    1,    1,    1,    0,    3,    1,    3,    1,
    0,    3,    1,    3,    1,    3,    3,    1,    1,    3,
    1,    3,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    3,    4,    5,    6,    3,    4,    3,
    4,    3,    4,    4,    5,    4,    4,    3,    4,    1,
    3,    3,   10,   10,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    3,    3,    3,    3,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    3,    1,
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
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,  552,
  553,  554,    0,  609,  719,  720,  724,  725,  463,    0,
    0,    0,    0,    0,    0,  400,  564,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  115,    0,    0,
   96,    0,  883,    0,   22,  881,    0,    0,   74,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  477,    0,    0,   38,   35,   37,    0,   36,
    0,  956,    0,    0,    0,    0,   33,   69,   67,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  634,    0,    0,    0,  958,  666,    0,    0,
    0,    0,    0,  108,  963,  558,    2,    0,    0,    0,
    0,  447,  429,  430,  431,  432,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
  448,  449,  450,  451,  452,    0,  428,  949,    0,  953,
    0,  407,  955,    0,  404,    0,  668,  959,    0,  403,
    0,  562,  879,    0,  151,  104,    0,  945,  105,    0,
  218,  758,    0,  952,  103,  146,    0,    0,   99,    0,
  106,  885,    0,  100,    0,  101,  962,    0,  102,  147,
  107,  618,  619,    0,  884,  882,    0,  950,   75,    0,
    0,  948,  485,  478,    0,  484,  489,  479,  480,  487,
  481,  482,  483,  488,    0,   31,    0,   40,  745,   39,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  889,    0,  870,  636,  638,
    0,  639,  641,  938,  642,    0,  643,  632,    0,    0,
    0,    0,    0,    0,    0,  581,  570,    0,  573,  575,
  576,  577,  578,  584,  585,    0,  957,  667,    0,  109,
    0,  110,    0,  111,    0,    0,  610,    0,  608,    0,
    0,    0,    0,  464,    0,    0,  370,    0,  221,    0,
  410,  401,  461,  462,    0,    0,    0,    0,    0,    0,
  419,    0,  376,  377,    0,  568,  566,  880,    0,    0,
    0,    0,    0,    0,    0,  154,  133,    0,   97,    0,
    0,  112,    0,  122,  121,    0,    0,    0,    0,    0,
    0,  941,   76,    0,    0,    0,  721,  722,  486,    0,
  940,  862,  861,  860,    0,    0,  863,  864,    0,    0,
  866,  347,  348,  342,    0,  346,  858,  865,    0,  748,
    0,    0,   28,    0,    0,  960,    0,   72,   70,   68,
   34,    0,    0,    0,  750,    0,   53,    0,    0,    0,
    0,    0,  560,    0,    0,    0,    0,  848,  876,    0,
    0,  850,    0,    0,  877,    0,  871,    0,    0,  852,
  853,  854,  872,  873,    0,  874,  875,    0,    0,    0,
  878,    0,    0,    0,    0,    0,    0,  755,  844,  845,
  846,  851,  830,    0,  829,    0,  833,  892,  893,  832,
  826,  827,  828,  831,  847,  890,  891,  913,  914,  917,
  918,    0,  645,    0,  732,    0,    0,  646,    0,    0,
  607,    0,  582,    0,  579,    0,  580,  605,    0,  600,
    0,    0,  574,    0,  759,  728,    0,    0,    0,    0,
    0,    0,  540,    0,    0,    0,  614,    0,  611,    0,
  186,    0,    0,  130,  469,  173,  174,    0,  467,    0,
    0,    0,    0,  470,    0,  465,    0,    0,  414,  416,
  415,    0,  417,    0,  411,  219,    0,    0,  272,    0,
    0,    0,    0,  423,  424,    0,  454,    0,    0,    0,
    0,    0,    0,    0,  381,  380,  473,    0,    0,  150,
    0,    0,  159,    0,  158,    0,  165,  148,    0,  163,
  166,  167,  155,    0,    0,  508,  544,  511,    0,  545,
    0,    0,  512,    0,    0,    0,  535,  548,  507,  530,
  509,  510,  505,  492,  370,  490,  491,  493,    0,    0,
  502,  503,  504,  506,    0,    0,    0,    0,    0,  113,
    0,  114,    0,  125,    0,    0,  202,    0,    0,  368,
    0,  221,    0,    0,   84,    0,    0,  740,    0,    0,
    0,    0,    0,    0,    0,    0,  761,  765,  766,  767,
  768,  769,  770,  771,  772,  773,  774,  867,  868,  869,
  859,    0,    0,    0,  747,   29,    0,    0,   45,   73,
    0,    0,   27,    0,  753,    0,    0,    0,  716,    0,
    0,    0,    0,    0,  559,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  961,  319,    0,    0,
    0,    0,    0,    0,    0,  637,    0,    0,  640,    0,
    0,    0,    0,  954,  649,  556,    0,  887,  555,    0,
  583,    0,  591,  587,  586,    0,    0,    0,  602,  590,
    0,    0,  726,  727,    0,    0,    0,  368,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  131,    0,    0,
    0,    0,    0,    0,  370,  413,    0,  370,  371,  418,
  500,    0,  274,    0,  268,  270,  368,  946,  241,    0,
    0,  370,    0,    0,  210,  368,    0,    0,    0,    0,
  246,  240,  243,  244,  245,  422,  459,  458,  421,  420,
    0,    0,    0,    0,    0,    0,  379,    0,  370,    0,
  156,  160,    0,  164,    0,    0,  531,  532,  549,    0,
  546,    0,    0,  538,  536,  537,    0,    0,    0,    0,
  501,    0,    0,    0,    0,  539,    0,    0,  368,    0,
  201,    0,  204,  206,  207,    0,    0,    0,    0,    0,
  221,   79,    0,    0,   78,  942,  519,  520,    0,    0,
    0,  764,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  344,  350,  351,  349,  749,    0,    0,    0,
    0,    0,    0,    0,    0,   54,   62,   64,    0,    0,
   71,    0,    0,    0,  752,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  930,  931,  932,
  933,  934,  935,  936,  937,    0,    0,    0,    0,    0,
    0,  923,    0,    0,    0,  664,    0,    0,  842,  843,
  911,    0,    0,  944,  838,    0,    0,    0,    0,    0,
    0,  849,  757,  756,    0,    0,    0,    0,  654,  655,
    0,    0,  647,  650,  557,  888,  592,  601,  596,  595,
    0,  606,  760,  729,  730,  368,    0,    0,  216,    0,
    0,  615,    0,    0,  624,  182,  176,  183,    0,  179,
    0,    0,  178,    0,  189,  170,  169,  886,  172,  171,
  471,    0,    0,    0,  412,  273,    0,  212,  370,  214,
    0,  371,  209,  215,    0,    0,    0,    0,  425,  426,
  370,  368,  371,    0,    0,    0,  397,    0,  197,  198,
  157,    0,    0,    0,    0,    0,    0,  547,    0,    0,
  370,  371,    0,  496,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  199,    0,    0,
    0,    0,  357,    0,   83,    0,    0,    0,    0,   88,
   85,    0,    0,    0,  947,  723,    0,    0,  775,    0,
    0,    0,  801,  800,    0,    0,    0,  808,  820,  818,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  763,    0,   66,   58,   59,   65,   60,   61,    0,    0,
   23,    0,    0,    0,   43,    0,    0,    0,  734,  754,
    0,  717,    0,  733,    0,    0,    0,    0,    0,    0,
  221,    0,  894,    0,    0,  909,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  919,    0,
  929,    0,    0,    0,    0,    0,    0,  910,  855,    0,
    0,    0,    0,    0,    0,  669,    0,    0,    0,    0,
    0,  598,  599,  603,    0,  200,    0,    0,  616,  617,
  628,    0,    0,    0,    0,    0,    0,  190,    0,    0,
  408,  224,  225,  226,  227,  228,  229,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  455,
  456,  427,    0,    0,    0,    0,    0,    0,  370,  475,
  371,  943,  168,    0,    0,  514,    0,  513,  550,    0,
  542,    0,    0,    0,  494,    0,  522,    0,    0,  524,
  527,  529,  368,    0,    0,    0,  120,    0,  205,  370,
    0,    0,  369,    0,   98,    0,  118,    0,   94,    0,
    0,    0,    0,   87,    0,  816,  817,    0,    0,    0,
    0,    0,    0,  819,    0,    0,    0,    0,    0,    0,
    0,  809,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   57,   63,   55,
   56,   24,  738,  739,    0,  736,  743,  744,  741,    0,
    0,  715,    0,    0,  900,  899,    0,    0,  896,  895,
    0,  904,  903,  902,  901,  898,  897,    0,    0,  925,
    0,    0,    0,  924,    0,    0,    0,  662,    0,    0,
  657,    0,  658,    0,  675,  671,  673,    0,    0,    0,
    0,    0,  369,  621,  622,    0,  184,  180,    0,    0,
  195,  191,    0,  371,  280,    0,    0,  371,  278,    0,
    0,    0,  232,    0,    0,  248,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  472,  516,    0,  517,    0,
  135,    0,    0,    0,  139,  141,  142,  143,  497,  499,
  495,  526,    0,    0,  371,    0,    0,    0,    0,  352,
    0,  358,  370,   89,    0,    0,   91,    0,    0,    0,
    0,    0,    0,  822,  821,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   41,    0,    0,  718,  220,  912,  905,
  906,    0,  907,  921,  920,    0,  926,  915,    0,  916,
    0,  660,    0,    0,  682,    0,  679,  683,  684,  686,
  687,  688,    0,    0,  630,  371,  353,    0,  175,  192,
  371,  145,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  285,  290,  284,
    0,    0,    0,  294,  295,  296,  297,  298,  299,  302,
  304,  267,    0,  208,  233,    0,    0,    0,    0,  234,
  237,    0,    0,  371,  395,  396,  392,  391,  394,  393,
    0,  515,  543,  144,  132,  140,    0,  370,  264,  127,
  371,    0,    0,  372,  359,    0,    0,   92,   93,   80,
    0,  787,  795,    0,  788,  796,  791,  799,    0,    0,
  789,  797,  790,  798,  784,  792,  785,  793,  786,  794,
  735,    0,    0,  922,  663,    0,    0,    0,    0,  699,
  701,    0,  696,    0,  265,  185,  409,    0,  310,    0,
    0,  313,  314,    0,  306,    0,    0,    0,  305,    0,
  307,  293,  300,    0,  303,  286,    0,  291,    0,  333,
  292,  211,  235,    0,    0,  238,    0,  250,  252,  399,
  371,    0,  374,    0,    0,    0,  364,  365,    0,    0,
   77,   95,    0,    0,  908,    0,    0,  685,  680,  700,
    0,    0,  283,    0,  308,    0,    0,    0,    0,    0,
  301,    0,    0,  326,  327,  341,    0,    0,    0,    0,
  336,  334,  239,    0,    0,    0,    0,    0,  266,  371,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  707,    0,    0,    0,  282,  279,  309,    0,    0,    0,
  312,    0,    0,    0,  318,    0,  340,  951,  339,  338,
    0,    0,    0,    0,    0,  249,    0,  256,    0,  255,
  375,  367,    0,    0,    0,  119,    0,    0,    0,  709,
  698,    0,    0,  710,    0,  706,    0,    0,    0,    0,
    0,    0,    0,    0,  329,  328,  335,  337,  236,    0,
    0,  260,  259,  258,    0,  360,    0,  631,  824,  705,
    0,  712,  321,    0,    0,  311,    0,    0,    0,  330,
  263,  261,  262,    0,    0,    0,    0,    0,  331,  315,
    0,    0,    0,    0,  711,  324,  316,  823,  714,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1504,   27,   28, 1505,
   30,   31,   32,   33,   34,   35,   36, 1506,   38, 1507,
  618,  103,  382,  874, 1121,  104,  648,  875,  105,  387,
  653,  128,  106,  107,  238,  946,  117,  876,  877,  179,
  208,  108,  878,  109,  389,   79,  219,  353,  436,  613,
 1058,  371,  834,  584,  493,  811,  614,  615,  616, 1060,
  220, 1313,   71,  169,   72,  214, 1776, 1265,  199,  204,
  206,  209,  195,  186,  189,  201,  211,  134,  290,  292,
  294,  342,  600,  602,  379,  239,  761,  518,  625,  995,
  346,  605, 1046, 1257,  551,  708, 1021,  503,  504,  533,
  338,  585, 1032, 1403,  730, 1201, 1404, 1405, 1406, 1407,
 1408,  626,  196,  187,  334,  558,  335,  336,  559,  560,
  561,  562, 1233,  989,  506,  507,  979,  977,  508,  983,
  978, 1192, 1367,  984,  985, 1197,  348,  822,  823,  824,
  825,  763, 1002, 1381,  586,  765,  438,  827,  935,  527,
 1328, 1202,  439,  440,  441,  926,  442, 1382, 1383,  536,
 1530, 1676, 1218,  771,  772,  759,  773,  774,  775, 1677,
 1678, 1753,  316,  531, 1209, 1508,  175,  528,  529, 1210,
 1211, 1376, 1654, 1509, 1510, 1511, 1512, 1619, 1513, 1514,
 1515, 1516, 1517, 1518, 1519, 1520, 1521, 1699, 1740, 1604,
 1663, 1742,  443,  688, 1347,  444, 1066, 1620, 1670, 1671,
 1709,  374,  375,  445, 1051, 1263, 1420,    0,    0, 1052,
 1053,  266,  466, 1553,  357, 1181, 1361, 1485, 1635, 1636,
 1637, 1638, 1179,  325,  547,  789, 1230,  548,  787,  202,
   56,  320,  180,  141,  303,  166,  310,  172,  749,  746,
  311,  321,  779,  322,  544, 1013,  167,  304,  515,  549,
 1231,   93,  177,  223,  587,  588,  751, 1033,  752, 1034,
 1035,  591,  592,  593,  594, 1026,  838,  595,  596,  597,
  804,  598,  495,  801,   40,   41,   42,  136,   58,  404,
  182,  327,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  714,  479,  961, 1184,  480,  481,  719,   43,  140,
  299,   44,  496,  497, 1189,  735,  975,  121,  701,  659,
  122,  123,  267,  467,  468,  469,  705,  947,  948,  949,
  950, 1173,  446, 1161, 1476, 1477,  447, 1479,  448,  449,
  450,    0, 1691, 1594, 1692, 1693, 1694, 1736, 1777,  394,
 1133,   45,   46,  358,   47,   48,  488,  489,  883, 1129,
 1315, 1319,  395,  656,  192,  627,  628,  629,  630,  631,
  632,  633,  634,  635,  636,  637, 1282, 1076, 1078, 1079,
  933,  686,  451,  452,  453,  454,  455,  378,  216,  456,
  457,  916,  458,  459,  460,  461,  678,  920, 1162, 1157,
  679,
};
static short yysindex[] = {                                   4293,
 1560, -152, 3272,  116,   66,    6, 2805,  363, 1467,  -21,
  140, 2814, -152, 1459,  449,  338, 1062,  361,    0,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  792,    0,    0,    0,    0,    0,    0, 5760,
  489,  608,  651,  338,  787,    0,    0,  573, 1635,  794,
  489,  906, 1034,  651,  887,  338,  787,    0,  943, 1019,
    0,  907,    0,  924,    0,    0, 1126, 1039,    0,  338,
 1067,  489, 1235, 1039,  906,  608,  651,  887,  338,  787,
  943, 1019,    0,  651, 1167,    0,    0,    0, 1599,    0,
 1599,    0, 1212, 1216,   73, 1620,    0,    0,    0,  794,
  338,  794, 1295, 1302, 1306,  123, 1500, 1287,   46,  149,
   28, 1253,    0,  906, 1415, 1000,    0,    0, 1338, 1301,
  651,  338,  943,    0,    0,    0,    0, 1493,   63, 1453,
  520,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1258,    0,    0, 1635,    0,
   43,    0,    0, 1746,    0, 1153,    0,    0,   81,    0,
 1365,    0,    0, 1351,    0,    0, 1521,    0,    0, 1555,
    0,    0, 1635,    0,    0,    0,  556, 1746,    0,   30,
    0,    0, 1507,    0, 1518,    0,    0, 1599,    0,    0,
    0,    0,    0, 1522,    0,    0, 1392,    0,    0, 1533,
 1433,    0,    0,    0, 1392,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  810,    0,  794,    0,    0,    0,
  901, 1485,  577, 1019, 1032, 1558, 1869, 1586, 1611,  901,
 1529,  577, 1552, 1571, 1461,    0, 5617,    0,    0,    0,
 5617,    0,    0,    0,    0, 3690,    0,    0, 1523,  608,
 1612, 1527,  -49,  338, 1554,    0,    0, 1000,    0,    0,
    0,    0,    0,    0,    0, 1537,    0,    0, 1746,    0,
 1507,    0, 1599,    0, 1164, 1610,    0, 1563,    0,  991,
 1650, 1097,  520,    0, 1724, 1391,    0,  794,    0, 1258,
    0,    0,    0,    0,  794, 1618,  -92, 1569,  -74, 2021,
    0, 8241,    0,    0,  820,    0,    0,    0, 2007, 2007,
 1635, 1704, 1635, 1223, 1521,    0,    0, 6426,    0,  651,
  787,    0, 1618,    0,    0, 1793,  517, 2044,  338, 1823,
  608,    0,    0, 2049,  206, 1700,    0,    0,    0, 1633,
    0,    0,    0,    0, 1637, 1645,    0,    0, 1551, 1008,
    0,    0,    0,    0, 2048,    0,    0,    0, 1147,    0,
  338, 1671,    0,  651, 1648,    0,  118,    0,    0,    0,
    0,  338, 1537,  460,    0,  338,    0, 1791,  651, 2070,
  901, 1712,    0,  361, 2087, 2092, 2093,    0,    0, 2095,
 2099,    0, 2103, 2105,    0, 2107,    0,    0, 2114,    0,
    0,    0,    0,    0, 2126,    0,    0, 4232, 2127, 2128,
    0, 5617, 5617, 4411, 1709, 2137, 1556,    0,    0,    0,
    0,    0,    0, 2045,    0, 2125,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1930,    0, 2980,    0, 1858, 2136,    0,  -80,  153,
    0,  -84,    0,  541,    0,  541,    0,    0, 2146,    0,
   91, 1763,    0,    0,    0,    0, 2147,  164, 2133, 1618,
 2155, 1934,    0, 1922, 1922, 2159,    0, 2160,    0, 1824,
    0,  132, 1860,    0,    0,    0,    0, 1635,    0, 2007,
 2007, 2007, 2007,    0, 2167,    0, 1935, 1937,    0,    0,
    0, 1911,    0, 6426,    0,    0, 6426, 1594,    0, 1746,
 1957, 1761, 5552,    0,    0,  420,    0, 1761, 1238, 1153,
    0, 1825, 1788, 1756,    0,    0,    0,  480, 1830,    0,
 1772,  887,    0, 1635,    0, 1955,    0,    0, 1223,    0,
    0,    0,    0,  218, 1787,    0,    0,    0, 1789,    0,
 1836, 2199,    0, 1890, 2202,    7,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2152,  126,
    0,    0,    0,    0,  184, 2210, 2211, 2202, 1746,    0,
 1933,    0, 1989,    0, 1635, 1635,    0,  -92,   81,    0,
 1870,    0, 1843, 2215,    0, 5024,  138,    0, 1871, 1879,
 2223, 2285, 2224, 1786, 5354,   40,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1741, 1008,  794,    0,    0, 2174, 1828,    0,    0,
 1811,  577,    0, 1961,    0, 1747, 1459, 2238,    0, 1961,
 2174, 1967, 1971,  651,    0,  549, 4475,  128,  906, 5617,
  746,  934, 1131, 2671, 5617, 2285, 2980, 1841,  551, 5617,
 4475,  893,  893, 1459, 1938, 2239,    0,    0, 4712, 1834,
 5617, 5617, 5617, 5617, 5617,    0, 5617,   45,    0,   71,
 1831, 4475, 1835,    0,    0,    0,  696,    0,    0,   -3,
    0, 1852,    0,    0,    0,  338,  677,  338,    0,    0,
  794, 1537,    0,    0, 4475, 2039,  -92,    0, 1834,  907,
  907, 1164,  113,  794, 1850, 1635, 1635,    0,  -96, 1772,
 1772, 1848,  887, 1097,    0,    0, 2269,    0,    0,    0,
    0, 2035,    0,  794,    0,    0,    0,    0,    0, 2040,
 2275,    0, 2015, 2033,    0,    0, 1974, 1976, 1599, 2046,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  887,  794, 6119, 1942, 1944, 1946,    0,  887,    0, 1630,
    0,    0, 1903,    0,  119, 1918,    0,    0,    0,  887,
    0, 2199,  162,    0,    0,    0, 2041,  162, 1917,  162,
    0, 2300,  887,  887,  887,    0, 1618,  338,    0, 1988,
    0, 1818,    0,    0,    0,  820, 1928,  338, 1859,   92,
    0,    0, 2309, 2077,    0,    0,    0,    0,  206, 1892,
 1941,    0, 1941, 4762,  687, 5617, 5617, 2369, 2315, 1161,
 2604, 5617, 2839, 1140, 2921, 3074, 4795, 3156, 3391, 3626,
 2285, 2285,    0,    0,    0,    0,    0, 1648,  651,  787,
 1648,  943, 1019,  189, 2312,    0,    0,    0, 2174, 1983,
    0,  -65, 1994, 1537,    0,   28, 4475, 1994, 2331, 2312,
 2174,  440, 2056, 5617, 5617, 2115, 5617, 2340, 5617, 2343,
 2078, 5617, 5617, 5617, 5617, 5617, 5617,    0,    0,    0,
    0,    0,    0,    0,    0, 2067, 1379,  -19, 5617,  659,
 4475,    0, 2285, 2392, 2980,    0, 2346,   28,    0,    0,
    0, 2980, 1881,    0,    0, 2111,  893,  893,   65,   65,
 1024,    0,    0,    0,   71, 2352, 2350, 1336,    0,    0,
 2285, 2069,    0,    0,    0,    0,    0,    0,    0,    0,
  327,    0,    0,    0,    0,    0, 1905, 1928,    0, 1896,
 1896,    0, 2356, 1899,    0,    0,    0,    0, 1906,    0,
 2007, 2140,    0,  -96,    0,    0,    0,    0,    0,    0,
    0, 2367, 2285, 1960,    0,    0, 2108,    0,    0,    0,
 1960,    0,    0,    0, 1599, 1599, 1913, 2376,    0,    0,
    0,    0,    0,  -89,  -53, 1009,    0, 2157,    0,    0,
    0, 1964,  116, 1963, 2001, 2035, 2385,    0, 1772, 1972,
    0,    0, 1142,    0, 2370, 1964, 1151,  887, 2388, 2390,
 2399, 2186,   81, 2108, 2007, 1860,  -92,    0, 1830,   28,
 1996, 2011,    0,  101,    0,  887, 2058, 2145, 5024,    0,
    0,  887, 1970, 2156,    0,    0, 2424, 2427,    0, 2131,
 2980, 2429,    0,    0, 2980, 2430, 4975,    0,    0,    0,
 2118, 2432, 2980, 2441, 1754, 2443, 2980, 2445, 5617, 5617,
 2315, 5617, 5057, 2446, 2980, 2447, 2459, 2980, 2466, 5617,
 2980, 2467, 2980, 2468, 2469, 2980, 2470, 2480, 2980, 2482,
    0, 2261,    0,    0,    0,    0,    0,    0, 2197, 2174,
    0, 2174,  189, 2312,    0,  228,  228,   53,    0,    0,
 3690,    0, 1991,    0, 2331, 2312, 2484,  440, 2257, 2437,
    0, 2453,    0, 2529, 5617,    0, 2628, 2657, 2667, 2713,
 2751, 2797, 5617,  887, 4475, 2980, 2100, 4475,    0, 5617,
    0, 2232,  399, 5617, 4475, 5617, 5617,    0,    0, 1336,
 2491, 4475, 1301,   71, 2153,    0, 2158, 2162, 2200, 2271,
 2214,    0,    0,    0, 2108,    0, 1996, 1221,    0,    0,
    0, 2130, 1635, 2163, 1772, 2164, 1772,    0, 2285,  712,
    0,    0,    0,    0,    0,    0,    0, 2101, 2278, 2108,
    0, 5617,    0, 2267, 2151, 1913,  907,    0, 2285,    0,
    0,    0, 2173, 2176, 2192, 2193, 2196, 2198,    0,    0,
    0,    0,    0, 2521,  116,    0, 1848,    0,    0,  162,
    0, 1960, 2194,  162,    0,  162,    0, 2035, 2543,    0,
    0,    0,    0,  820, 2278, 1772,    0, 1860,    0,    0,
 3690, 2308,    0,  484,    0, 2356,    0, 2556,    0, 2560,
 1635, -100, 2563,    0, 1635,    0,    0, 5617, 2181, 2181,
 1459, 2567, 2002,    0, 2181, 2181, 5617, 2181, 2181, 2643,
 2980,    0, 1762, 5617, 2980, 2181, 2181, 2181, 2181, 2980,
 2181, 2181, 2181, 2181, 2181, 2181, 2228,    0,    0,    0,
    0,    0,    0,    0, 2178,    0,    0,    0,    0, 1858,
 4475,    0,  440, 2484,    0,    0, 6426, 2569,    0,    0,
 2844,    0,    0,    0,    0,    0,    0, 2963,   78,    0,
 4475, 2307, 2183,    0, 4475, 2972, 2006,    0, 1535, 2980,
    0, 2008,    0, 1336,    0,    0,    0,   71, 1420, 2285,
 2225, 2278,    0,    0,    0, 2007,    0,    0, 1635, 1630,
    0,    0,  790,    0,    0,  794, 3529,    0,    0, 2980,
 1922, 2267,    0, 1184,  338,    0, 2282,  805, 2180, 2287,
 2184, 2319, 2289, 2328, 2108,    0,    0, 2590,    0, 2592,
    0, 2281, 1922, 2194,    0,    0,    0,    0,    0,    0,
    0,    0, 2108, 1830,    0, 1630, 2157, 2316, 2252,    0,
 1928,    0,    0,    0, 2274,  396,    0, 2035, 2276, 2980,
 2609, 2610,   28,    0,    0, 2612, 2613, 2980, 2614, 2617,
 5617, 5617, 2980, 2619, 2620, 2622, 2627, 2633, 2635, 2636,
 2638, 2639, 2640,    0,  228, 1831,    0,    0,    0,    0,
    0,  887,    0,    0,    0, 4475,    0,    0, 4475,    0,
 1831,    0, 1071, 2402,    0, 2645,    0,    0,    0,    0,
    0,    0,   40,  718,    0,    0,    0, 1772,    0,    0,
    0,    0, 6426,  182,  780, 2624, 1610, 2647, 2351, 5617,
 2642, 2654, 2644, 2646, 2652, 2354, 2656,    0,    0,    0,
 2407, 5427, 2659,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2665,    0,    0, 2344, 2375,  338, 1913,    0,
    0, 1599,  608,    0,    0,    0,    0,    0,    0,    0,
 2278,    0,    0,    0,    0,    0, 2278,    0,    0,    0,
    0,  107, 1831,    0,    0, 5617, 1635,    0,    0,    0,
 1635,    0,    0, 5617,    0,    0,    0,    0, 2980, 2980,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2069, 2680,    0,    0, 2069, 2285, 1834, 1420,    0,
    0, 2353,    0, 2686,    0,    0,    0,  353,    0, 5292,
  651,    0,    0, 5617,    0, 2386, 2285, 1805,    0, 2285,
    0,    0,    0,  130,    0,    0,   26,    0, 2279,    0,
    0,    0,    0, 1913, 1599,    0, 2348,    0,    0,    0,
    0, 2157,    0,  107, 2684, 1336,    0,    0, 2069, 2980,
    0,    0, 1535, 2214,    0, 2214,   40,    0,    0,    0,
 1103, 1960,    0, 2675,    0, 2774, 5520, 3000,  130,  816,
    0,  834, 1452,    0,    0,    0,  489, 2277,  116,  421,
    0,    0,    0, 2348,  -48, 1913, 2357, 2358,    0,    0,
 1336, 2689,  107, 2391, 2214, 2696, 1831, 2225, 2225,  338,
    0, 2708, 2707, 1549,    0,    0,    0, 3869, 2341, 2709,
    0,  141, 2323, 2456,    0,  130,    0,    0,    0,    0,
 3529,   26, 1913,  730,  730,    0, 2460,    0, 2320,    0,
    0,    0, 1336,  107, 2225,    0, 2069, 2377, 2377,    0,
    0, 1103, 2724,    0,  608,    0, 1938, 2017,  124, 2716,
 2483, 2473, 3529, 3529,    0,    0,    0,    0,    0, 2324,
  -36,    0,    0,    0, 1498,    0, 2214,    0,    0,    0,
  608,    0,    0,  130, 2735,    0, 1892, 3529, 2472,    0,
    0,    0,    0, 2285, 2225, 2739, 2746, 2019,    0,    0,
 3529,   40, 2377,  608,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  236,  523,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  106, 2030,    0,    0,    0,    0,    0, 2789,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  326,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2461,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2416,    0,    0,    0,  105,    0,
  105,    0,    0,    0,    0, 2417,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  795,    0,    0,
 3933, 2106,    0,    0,    0,  856,    0,    0,    0, 1697,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  318,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -20,    0,    0,    0,    0,  813,    0,
 1035,    0,    0,    0,    0,    0, 2031,    0,    0, 6733,
    0,    0,    0,    0,    0,    0,    0,  -20,    0,  -87,
    0,    0, 2762,    0,    0,    0,    0, 2527,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1064,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1101,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1130,    0,    0,    0,
    0,    0,    0,    0, 2345,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  522,    0,    0, 1219,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -20,    0,
 2762,    0, 2527,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1337,    0, 2526,    0,    0,    0,    0, 1359,
    0,    0,    0,    0,    0, 2541,  482, 4089,    0, 1395,
    0,    0,    0,    0,  400,    0,    0,    0, 2349, 2349,
    0,    0,    0, 1410, 2175,    0,    0,    0,    0,    0,
    0,    0, 2541,    0,    0, 1642,    0,    0,    0,    0,
    0,    0,    0, 4649, 2420,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1423,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1484,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1534,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 5997,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 6167,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  187,    0,    0, 2486,    0,  129,    0,
    0,    0,    0,  547,    0,  547,    0,    0,  634,    0,
  988,  992,    0,   99,    0,    0,    0, 1064,    0, 2541,
    0,    0,    0,   79,   79, 1061,    0,    0,    0,  121,
    0, 2625,  216,    0,    0,    0,    0,    0,    0, 2349,
 2349, 2349, 2349,    0,  613,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2547,
    0,    0,    0,    0,    0, 2526,    0,    0,  759,    0,
  908, 2347,    0,    0,    0,    0,    0,    0,  238,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1561,    0,
    0,    0,    0, 1375,  756,    0,    0,    0,  852,    0,
    0, 1261,    0,    0, 1464,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2059, 2877,
    0,    0,    0,    0, 1515, 1080,    0, 1464,  -20,    0,
    0,    0,    0,    0,    0,    0,    0,  482, 1250,    0,
    0,    0,    0, 2400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  269,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  340,    0,    0, 2030,    0,    0,  340,
    0,    0,    0,    0,    0, 5699,    0, 5699,    0,    0,
 5699, 5699, 5699,    0,    0,    0, 2362,    0,    0,    0,
    0, 6781, 6918, 4168,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1382,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  869,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  482,    0,    0,   93,
   93,    0,    0,    0,  348,    0,    0,    0, 2691,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1477,    0,    0,    0,    0,    0,    0,    0, 2780,
    0,    0, 2096, 1432,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  441,  836,  845,    0,    0,    0, 2558,
    0,    0,    0,    0,    0, 1550,    0,    0,    0,    0,
    0, 1261,    0,    0,    0,    0,  838,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2541,    0,    0,  335,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 8332,
    0,    0,    0,   84,    0,    0,    0,    0, 2512,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1588, 1589,    0,    0,    0,    0,    0,
    0,    0, 1608,    0,    0, 3933,    0, 1608, 1615, 1702,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  835,    0,    0, 5699,    0,    0,
    0,  885,    0,    0,    0, 6304, 7088, 7225, 6474, 6611,
    0,    0,    0,    0,    0,  868, 1225,  910,    0,    0,
    0,  393,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  922,
  922,    0,  122,    0,    0,    0,    0,    0,    0,    0,
 2349, 2364,    0, 3448,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2553,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1590,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1705,    0,    0,
    0,    0,    0,    0,    0, 1583,    0,    0,    0,    0,
    0,    0,    0,    0, 1232,    0,    0,    0,    0,    0,
    0,    0, 1250, 2553, 2349, 1707,  482,    0, 2561, 3933,
 1718,   95,    0,    0,    0,    0, 8332,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 3680,    0,    0,    0, 4792,    0,    0,    0,    0,    0,
    0,    0, 5268,    0, 7321,    0, 7384,    0,    0,    0,
    0,    0,    0,    0, 7464,    0,    0, 7548,    0,    0,
 7582,    0, 7593,    0,    0, 7692,    0,    0, 7782,    0,
    0, 2598,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1588, 1739,    0, 2389,  352,    0,    0,    0,
    0,    0,    0,    0, 1759, 1810, 1832,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2398,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2492,
 1285,    0, 3209,    0, 2499,    0, 2501, 2502,    0,    0,
  773,    0,    0,    0, 2553,    0, 1718,    0,    0,    0,
    0, 1959,    0,    0,    0, 2403,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2374,    0, 2576,
    0,    0,    0, 1125,    0, 1590, 2461,    0,    0,    0,
    0,    0,  849,  882,  903,  917, 1048, 1060,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  898,    0,    0,    0,    0, 2950,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1845,    0,    0,
    0,    0,    0,    0,    0, 1854,    0,    0,    0,    0,
    0,  135,    0,    0,    0,    0,    0,    0,    0,    0,
 4168,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 7788,    0, 7865,    0, 7951,    0,    0,    0,    0, 7971,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1867,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1675,
    0,    0,    0, 1045,    0,    0,    0,    0,    0,    0,
  270,    0,    0,    0,    0, 2349,    0,    0,    0, 1687,
    0,    0,    0,    0,    0,    0, 2539,    0,    0, 2808,
 1602, 1249,    0, 2526,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2553,    0,    0,    0,    0,    0,
    0,    0, 1880, 1075,    0,    0,    0,    0,    0,    0,
    0,    0, 2553, 2561,    0,  365,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  691,    0, 8054,
    0,    0, 5699,    0,    0,    0,    0, 8099,    0,    0,
    0,    0, 8186,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  352, 1573,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  945,    0, 2492,  426,    0,  629,    0,    0,    0,    0,
    0,    0,  330, 2821,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2554,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3097,    0,
    0,   85,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  186,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 8207, 8313,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1545,    0,    0,    0,   83,    0,    0,    0,    0,
    0, 2832,    0,    0,    0,    0,    0, 2816,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2564,    0,
    0,    0,    0, 3097,  150,    0,  112,    0,    0,    0,
    0,    0,    0,    0,  175,  804,    0,    0,  436, 2808,
    0,    0,    0,  423,    0,   89, 3444,    0,    0,    0,
 2821,    0,    0,    0,    0,    0,   72,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  151,    0, 1590,  178,  563,    0,    0,
 2492, 1742,    0,    0,  227,    0,  945,  290,  321, 1626,
    0,    0, 2835,    0,    0,    0,    0,    0, 2819,   80,
    0, 2585,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3097,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  886,    0,  201,    0,   83, 1240, 2840,    0,
    0, 2821,    0,    0,    0,    0, 2848,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2492,    0,   89,    0,    0,    0,
    0,    0,    0,    0, 2834,    0,    0,    0, 4007,    0,
    0,    0,    0,    0,  321, 2854,    0,    0,    0,    0,
    0, 1897, 2840,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2896,    0,    0, 2898,
    0,    0,    0,    0,    0,    0,    0,   10,    0, 2899,
    2,  695,  -27, -567, 1780,  752, -242,  197, 2894, 2658,
    0,  -35,    0, 2664,  -94,  -41,    0,  614, -835,  -33,
  -42,  -31, -192, 2668, 2259,    0,    0,    0, 1530,    0,
    0,  -51, 1865, -543,   -7, -911, 2301, 2088, -277, 1868,
 2849, -259,    0,  -57,    0, -647,  -50,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -487,  145, 1884, -269, -156, -925,
    0,    0,    0,    0, -308, -104,   -1, 1889, -306,  -46,
    0, -498,    0,    0, -465, -933,    0, 1540,    0,    0,
 2641, -568,  -44,    0,    0,    0,    0, 2606,    0, 2404,
 2648, -204, 1909, -695,    0,    0,    0, -694,    0,    0,
    0,    0,    0,    0, 1973,    0, 2660, 2229, 1915, 2650,
 2651,    0, 1367,    0, -500, 2205,  -60, -653, -700,    0,
    0,  -10, -939, -935, -931, -224, -923,    0, 1591, -940,
    0, 1298,-1149, 1592,    0, 2440,    0,    0,    0, 1303,
 1310, 1265,  117, -261, -239,  190,    0,    0, 2234,    0,
 1790,    0,    0,  -55,    0,    0,    0,    0, -238,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -865,    0, -392,    0,-1108, -286, 1229, 1371,    0, 1289,
    0,    0, 2663,  -99, 2029, 1816, 1653,    0,    0,    0,
-1074, -839, -891,    0, -471, -795, -662,    5,    0, -728,
 1377,  942, -423, -517, -723, -922, -762,    0,    0,    1,
    0,    0,    0,    0,    0, 2698,    0,    0,    0,    0,
 2714, 2478,    0,    0,    0,    0,    0, 2723,    0,    0,
    0,    0,  -15,    0,    0,    0, -320, 2218, -312, 1785,
 -430,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2438,    0,-1180, 2233,    0,    0,    0, 2637, 3033,    0,
    0,    0,    0,    0, 2772,    0,    0,    0,    0, 2580,
    0, 2581,    0,    0,    0, 2342,    0,    0,    0, -521,
    0,  622,    0, 2329, 2085,    0,    0, -612,-1028, 1052,
    0, 2946, 1458,    0, 2368, -623, 2366,    0, -713, 2134,
    0,    0, -127, -644,    0, 1482,-1141,    0,-1117,-1091,
-1080,    0, 1596,    0, 1340,    0, 1385,    0, 1294,    0,
    0,    0,    0, 2593,    0,    0,    0, -324, 2422, 2206,
 -958,    0,    0,    0,  897,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2397, 1512, 1993,    0,
 -564,    0,    0,    0, -222,    0,    0, 2727,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   47, 1947,
    0,
};
#define YYTABLESIZE 8830
static short yytable[] = {                                     191,
  130,  171,  288,  193,   76,   75,  240,  185,  494,   37,
  373,  174,  377,  190,  259,  262,  723,  589,  176,  260,
  263,  552,  198,  750,  224,  590,  753,  557,  969,  731,
  203,  524,  764,  205,  762,  228,  927,  522,  210,  709,
  227,  465,  980,  896,  886,  229, 1131,  486,  233,  246,
  388,  248,  235,  842, 1203,  845,  232,  200, 1204,  388,
  234, 1203, 1205, 1352,  130, 1204, 1386, 1213,  655, 1205,
 1207,  928,  835,  231,  968,  247, 1214, 1207,  217,  269,
  230,  603,  970,  971,  746,  257,  943, 1222, 1166,  289,
  293,  826,  620,  889,  356,  619,  291,  514,  945, 1024,
  437,  965, 1049,  997,  462,  565, 1243,  918,  258,  464,
  945,  257, 1004,  350, 1238,  258,  244,  312, 1463,  217,
  627,  626,  217,  678,   86,  746, 1260,   86,  746,  690,
  322, 1057,  941,  620,  718,  376,  620,  217,  320,  945,
  237,  339,  945,  746,  957,  373, 1634,  377,  746,  746,
  257,  620,  257,  356,  943,  257,  662,  945,  487,  945,
   74,  652, 1023, 1764,  565, 1044,  244,  498, 1316,  898,
  257,  737,  648, 1426,  361,   90,  380,  253, 1475,  627,
  626,  435,   74,  703, 1706,  692,  731,  435,  261, 1422,
  746,  257,  650,  746,  257,  532,  738,  707,  492,  123,
  697,  740,  741,  742,  743,  383, 1029,  722,  746,  257,
 1261,  258,  589,  538,  383,  466,  810, 1478,  253,  471,
  590,  253,  398,  813,  372,  485,  690,  731,  726,  952,
  731, 1170, 1120,  361,  474,  565,  253,  398,  478, 1320,
 1599, 1480,  861,  269,  692,  731,  973,  523, 1225, 1223,
   74,  110,  557, 1714,  526, 1126,  191,  795,  539,  697,
  981,  542,   74, 1132, 1772,  487,  770, 1481,  691,  697,
  376,  677,   74, 1384,  466,  682,  683,  685, 1482,  553,
  474,  555, 1011,  805, 1309,  690, 1311,  264, 1666,  697,
  258, 1456, 1111, 1112,  565,  607,  398,  344,  599,  313,
  611,  861, 1203,   57,  796, 1396, 1204,  601, 1401,  691,
 1205, 1123, 1185,  609,  343,  807, 1773,  625, 1207,  111,
 1471,  710,  296, 1135, 1137,  567,   80,  691,  697,  689,
  697,  265,  485,  982,  126,  475, 1411,  323, 1667,  737,
  275,  942,  649,  217,  690,  646, 1555,  629,  697, 1668,
  691,  521,  806, 1224, 1163,   76,  654,  649, 1221,  372,
  660,  697,  945,  945,  196,  383, 1427,  862,  188, 1418,
  689, 1127, 1030,  663,  123,  123,  625,  314,  945, 1626,
  704,  217,   77,   78,  567,  760,  188,  264,  689, 1226,
  476,  269,  678,  126, 1715,  345, 1462,  217,  737,  474,
  736,  945,  809,  945, 1741,  490,  629,  477, 1475,  717,
  521,  620,  112, 1652,  678,  324, 1155,  945,  865,  217,
  377,  893,  690,  196, 1200,  681,  862,  746,  257,  746,
  257,  897, 1317,  620,  613,  678,  476,  746,  257,  945,
  384, 1384,  465,  252,   90,  494,  790, 1478, 1492,  648,
  731,  678, 1522, 1669, 1523,  253,  739,  253,  474,  388,
 1354,  627,  626,  690, 1712,  253,  681,  844,  746,  681,
  678, 1480,  766,  466, 1673,  992,  690,  465,  994,  678,
   14,  690,  322,  964,  681,  690, 1056, 1309,  746, 1549,
  320, 1548, 1000,  613,  678,   86, 1581, 1481, 1368,  384,
  465,  398,  792,  361,  256,  188,  361,  731, 1482,  243,
  464,  988, 1283,  901,  692,  835, 1560,  692,  917, 1018,
 1348,  588,  466,  924,  925, 1254, 1716, 1348,  746,  257,
 1414,  173,  932,  746,  936,  937,  938,  939,  940,  991,
  932, 1399,  565,  866,  791,  464,  593,  191, 1700,  356,
  746,  257, 1059,  820,  821, 1042,  253,  746,  257, 1130,
 1595,  188,  254, 1749,   76, 1597, 1266,  127,  464, 1387,
 1324,  361,  651,  188,   73,  627,  626,  988,  253, 1064,
  588,  217,  692,  867,  484,  253,  746,  691,  697, 1738,
  484,  746,  257, 1564,  487,  620,   73,  691,  836, 1413,
  691,  945,  956,  254,  498,  593,  254,  256, 1629,  691,
  697,  256,  468,  257, 1687,  361,  356,   76,  837,  253,
  256,  254,  649,  690,  900, 1633,  692,  361,  677,  361,
 1373,  812,  864,  594,  466,  986,  987,  617,  706,  731,
  692,  697,  168, 1119, 1473,  565,  756,  731,  689,  617,
 1388,  746,  257, 1001, 1551,  697,  691,  697,  625,  617,
  861,  485,  465,  474,   73,  691,  191,  690, 1433,  677,
  689,  468, 1195,  380, 1489, 1113,   73,  697, 1116,  253,
  478,  690,  962,  126,  976,  976,   73,  677,  629, 1070,
 1071, 1075,  594,  526, 1083, 1085, 1087, 1457, 1095, 1098,
 1101, 1103, 1106, 1109,  384, 1679,  114,  737,  697,  691,
  697,  678, 1203,  196, 1686,  817, 1204,  689, 1695,  521,
 1205, 1010, 1012,  691,  678, 1711,  689, 1069, 1207, 1212,
  464,  523, 1182,  678,  523,  990, 1256, 1139, 1140,  767,
 1142, 1220, 1144, 1421,  681, 1147, 1148, 1149, 1150, 1151,
 1152, 1558, 1374,  613, 1721,  533,  487,  681,  460,  242,
  297, 1242, 1156,  115,  925,   94,  681,  678,  868, 1206,
  689,  377,  690, 1009,  124,  613, 1206,  300,  377, 1059,
  678,  784, 1043, 1183,  689,  862, 1644,  116, 1017,  678,
 1646, 1483, 1054, 1702,  563,  533,  533,  474,  127,  533,
 1027,  242,  460,  362, 1255, 1039, 1040, 1041,  264, 1400,
  690,  253,  378,  681,  533, 1410,  533,  460,  785,  690,
  768,  135,  681,  485, 1585,  116,  301, 1114, 1458, 1117,
 1491,  690,  678,  678, 1345,  382, 1115,  136,  384,   76,
  254, 1118,  254, 1685,  383, 1534,  533,  362,  389,  370,
  254,  534,  894,  563,  369,  572, 1703,  890,  921,  922,
  116,  769,  362, 1327,  588,  606,  681,  661,  254, 1680,
  468,  378,  258, 1765, 1704,  665,  678,  657,  665,  125,
  681,  390,  242,  871,  137,  363,  126, 1648, 1559,  593,
  678,  534,  534,  665,  382,  534,  136,  137, 1778,  588,
  386, 1050,  386,  383,  658, 1681,  465,  389,  661,  652,
  534,  661,  534, 1007,  572, 1524,  385,  499,  712,  468,
  932,  623,  786,  242,  956,  856,  661,  588,  856,  363,
  588, 1757, 1290, 1291,  694, 1293, 1295, 1545,  302,  695,
  390,  254,  534, 1300,  363, 1362,  588,  677,  861,  168,
  652,  738,  593,  652, 1723,  593,  137,  129,  340, 1395,
  677,  386,  623,  254,  523,  623, 1158, 1159,  652,  677,
  254,  593,  588,  861,  464,  385,  594,  856,  183,  184,
  623, 1688,  181, 1689, 1582,  692,  191,  604, 1331,  713,
 1417,  589, 1598,  380,  341, 1755, 1338,  593,  925, 1586,
  523,  925,  923, 1156,  254,  264,  589, 1346,  925, 1349,
 1350,  594, 1348,  533,  590,  925,  677, 1206, 1647,  377,
  385,  221, 1725,   76, 1234,  677, 1314, 1314, 1318,  533,
 1249,  468,  533,  533,  569,  533,  465,  386, 1660,  594,
  533, 1662,  594,  533,  653, 1353,  604,  388, 1269,  902,
  589,  861,  369, 1348, 1273, 1380,  533, 1488,  594,  387,
  612, 1590,  533,  692,  254,  533,  861, 1167,  170,  677,
  258,  378, 1474,  862,  138, 1124,  378,  861,  545,  528,
  959, 1639,  378,  677,  594,  653, 1591, 1136,  653,  546,
 1370,  690, 1372,  569, 1775,  861,  465,  960,  862,  382,
  345,  612,  523,  653,  464,  360,  388, 1750,  383,  534,
 1160,  173,  389,  690,  770,  136, 1169,  136,  387,  612,
  528, 1430,  692,  528,  230,  534,   76,   76,  534,  534,
 1438,  534,  362,  138,  533,  362,  534, 1443,  528,  534,
  528,  976,  665,  665, 1592,  390, 1339,  676, 1751, 1215,
 1216, 1416,  534, 1556,  256, 1541,  533,  523,  534,  345,
  690,  534,  361,  533,  464,  230,  386,  533,  230,  690,
  528,  212,  690, 1547,  533,  370,  862,  137,  691,  138,
  385,  955, 1601,  230,  925, 1244,  661,  645,  925,  661,
  644,  862,  533,  264, 1244, 1752,  661,  533, 1267,  661,
  362, 1340,  862,  661, 1342, 1782, 1758, 1759,  661,  213,
  378,  661, 1684,  690,  363, 1727,  136,  363,  571, 1425,
  862, 1664,  533, 1429,  651, 1602,  533,  661,  652,  676,
  534,  498,  139,  382, 1245,   76, 1398,  904,  652,  613,
  623,  652,  383, 1248,  362,  665,  389,  178,  231,  487,
  652,  661,  534,  676,  188,  661,  362, 1684,  362,  534,
  551, 1788,  623,  534,  661,  651, 1664,  362,  363,  364,
  534,  183,  184,  692,  597,  498,  692,  571, 1632,  390,
  613,  661,  363,  651, 1569, 1570,  665, 1603,  534,  231,
  365,  366,  231,  534,  197,  367,  368,  652,  613, 1684,
  386,  551, 1474,  360,  551,  945,  652,  231,  661,  925,
 1227,  388,  925, 1745,  385,  191,  485,  976,  534,  551,
  661,  551,  534,  387,  498,  597,  363, 1665,  661, 1493,
  604, 1684,  692,   96,  589,  381,  405,  528,  363,  501,
  363,  692,  271, 1608,  447,   73, 1664, 1228,  502, 1532,
  652,  551,  370,  528,  138, 1314,  528,  528,  406,  528,
  361,  127,  652,  653,  528,  604,  194,  528, 1490,  589,
   98, 1664, 1665,  653,  521,  510,  653,  272,  945,  612,
  528,  692,  215, 1596,  487,  653,  528, 1464,  676,  528,
  264, 1467, 1175,  604,  402,  405,  604,  589, 1378, 1640,
  589,  612,  242,  207,  230,  273, 1176, 1643,  274,  161,
  317, 1089,  604,  318, 1550,  521,  589,  406,  521, 1746,
  694,  692,  343,  693,  275,  695,  511, 1206, 1090,  377,
 1177,  490,  653,  521,  906,  521, 1600, 1627,  604,   99,
  692,  653,  589, 1656, 1415,  388, 1590, 1658,  217,  100,
  276,  485, 1665,  402, 1587,   76, 1618,  387,  528,  319,
  512,  498, 1583,  541,  131,  362,  363,  364,  161,  183,
  184, 1591,  490, 1091,  101,  490,  523, 1665,  513,  102,
  528,  343, 1628,   44, 1178,  653,  498,  528,  365,  366,
  490,  528, 1625,  367,  368, 1706,  132,  653,  528,  218,
  925,  510, 1092,  242,  541, 1641, 1606,  541,  378, 1642,
 1705,  133, 1584, 1080,  525,  305,  528,  523,  551,  378,
  523,  528,  541,  556,  541,  242,  242,  222,  231, 1592,
 1674,  498,  242,   52,  551,  523,  130,  523,  551, 1081,
  551, 1737,   44,  651,  678,  551,  528,  751,  551,  518,
  528, 1486,  511,  651,  541,  525,  651,  225,  525, 1657,
  162,  551, 1526,  127,  306,  651,  242,  551,  236, 1093,
  551,  501,  692,  525,  751,  525,  694,  692,  498,  693,
  502,  695,  523,  487, 1527,  678,  512,   42,   25,  251,
  518, 1528,   52,  518,  944,  241,  696,  694,  692,  242,
  693,  217,  695,  678,  513,  525,  654,  742,  518, 1707,
  518,  498,  651,  692,   48,  487,  487,  963,  130,  162,
  654,  651,   73,  523,  484, 1529,  523,  242,  654,  944,
  251,  692,  521,  251,  755, 1690,  498,  754,  237,  551,
  487,  523,  217,  523,  654,  217,   42,   25,  251,  242,
  485,  521,  521,  487,  521, 1747,  242, 1175,  690,  521,
  217,  551,  521,  245,  691,  651,  742,  130,  551,   76,
 1710, 1176,  551,   48, 1690,  521,  307,  651,  520,  551,
  249,  521,  485,  485, 1762,  250,  196, 1769, 1770,  251,
  242,  521, 1728, 1729,  308, 1177,  668,  551,  654, 1154,
  692,   50,  551,  777,  476,  778,  128,  485,  130,  490,
  692,  490, 1780,  692, 1364,  857, 1690,  373,  857,  490,
  485,  541,  692,  255, 1365, 1787,   95,  551,  309, 1756,
 1630,  551,  490, 1308,  523, 1310, 1631,  541,   26,  624,
  668,  541,  432,  541,  433,  196,  354,  120,  541, 1178,
  523,  541,  270,  521,  359,  668,  286,  435,   49,   95,
   50,  287,  258,  476,  541,  128,  295,  857,   96,  692,
  541,  298,  525,  541,  411,  521,  373,  523,  692, 1783,
   97,  863,  521,  523,  643,  315,  523,  885,  525,  326,
  884,  521,  525,  521,  525,  694,  692,   26,  693,  525,
  695,   96,  525,  694,  692,   98,  693,  518,  695,   51,
  490,  521,  328,   97,  690,  525,  521,   49,  337, 1175,
  691,  525,  692,  347,  525,  624,  518,  518,  432,  518,
  433,   46,  490, 1176,  518,  690,  349,  518,   98,  490,
  523,  691,  541,  435,  129,  521,  694,  692,  258,  693,
  518,  695,  352,  117,   49,  700,  518, 1177, 1048,  351,
  523, 1047,  523, 1661,  541,   50,   47,  523,   51,  355,
  523,  541,   51,  490,   99,  541,  678,  329,   73,  217,
  418, 1774,  541,  523,  100,  356, 1733,  384,  523,  523,
   46,  419, 1067,  525, 1068,  523,  366,   52,  392, 1055,
  541,  692,  831,  129,  692,  541,  396,   99,  393,  101,
  425, 1178,  117,  523,  330,  525,  331,  100,  428,  429,
  430, 1168,  525, 1734, 1167,   47,  525,  102,  518,  397,
  541,  399,  678,  525,  541,  401, 1735,  366,  217,  332,
  366,  678,  101,  523,  196, 1186, 1194,  523, 1047, 1193,
  518,  525,  119,  120,  402,  366,  525,  518,  181,  470,
  692,  523,   53,  708,  333,  196,  405,  403,  518,  692,
  699,  694,  692,  472,  693,  406,  695,  473,  929,  694,
  692,  525,  693,  523,  695,  525,  518,  196,  482,  407,
  523,  518,  620,  196,   54,  360,  408,  484,   55,  181,
  708,  523,  181, 1236,  369,  500, 1235,  509,  362,  363,
  364,  517, 1241,  708,  621, 1240,  196,  181,  668,  523,
  518, 1019, 1020,  409,  523,  668,  410,   14,  668,  530,
  152, 1322,  668,  690, 1321,  196,  367,  368,  537,  691,
  668,  690, 1435,  196,  196, 1167, 1470,  691, 1472, 1469,
  196, 1469,  405,  523,  411,  412,  668, 1763,  500, 1786,
 1469,  406, 1706,  364,  540, 1287,  413,  550,  196,  635,
  414,  635,  635, 1442,  635,  407,  554,  364,  622,  604,
  668,  360,  408,  608,  690,  364,  610,  635,  612,  152,
  691,  643,  635,  668,  638,  223,  183,  184,  639,  500,
  621,  364,  500,  124,  124,  196,  640,  647,  386,  409,
  668,  661,  410,  652,  664,  668,  196,  500, 1146,  694,
  692,  623,  693,  895,  695,  899,  666,  415,  903,  905,
  907,  667,  668,  416,  669,  697,  223,  668,  670,  223,
  411,  412,  671,  417,  672,  633,  673,  633,  633,  668,
  633,  668,  413,  674,  223,  364,  414,  362,  363,  364,
  418,  183,  184,  633,  622,  675,  680,  681,  633,  687,
  698,  419,  694,  692,  153,  693,  689,  695,  700,  702,
  365,  366,  420,  421,  422,  367,  368,  423,  424,  716,
  425,  720,  721,  725,  727,  426,  427,  728,  428,  429,
  430,  729,  732,  684,  431,  733,  734,  623,  501,  690,
  744,  748,  745,  415,  747,  691,  181,  690,  366,  416,
  757,  758,  781,  691,  782,  366,  783,  788,  366,  417,
  256,  793,  366,  153,  797,  799,  798,  181,  800,  802,
  366,  803,  808,  362,  363,  364,  418,  183,  184,  814,
  815,  818,  819,  828,  830,  360,  366,  419,  831,  181,
  839,  840,  841,  843,  879,  181,  365,  366,  420,  421,
  422,  367,  368,  423,  424,  880,  425,  887,  882,  930,
  366,  426,  427,  951,  428,  429,  430,  891,  181,  635,
  431,  892,  919,  366,  934,  704,  635, 1325,  694,  692,
  957,  693,  966,  695,  974,  635,  988,  181,  993,  152,
  366,  809,  361,  998,  999, 1001,  181,  760, 1005,  635,
 1006, 1022,  181, 1008,  624,  635,  635,  432, 1014,  433,
 1015,  152, 1016,  635, 1025, 1036,  500,  366,  500, 1038,
  181, 1031,  435,  500, 1045, 1050,  500,  258, 1062,  366,
 1063,  366, 1065,  635, 1077, 1122,  635,  690,  657,  500,
  152, 1128, 1084,  691, 1088,  633, 1096, 1099, 1125, 1104,
 1107, 1110,  633,  223, 1120,  223, 1138,  181, 1141,  152,
 1143,  633,  635,  223,  635,  635, 1145, 1153,  152, 1165,
  690, 1172, 1278, 1174,  152,  633,  635, 1180, 1188,  644,
  635,  633,  633,  417, 1191, 1196, 1199, 1208,  434,  633,
  690,  432,  152,  433, 1217, 1219,  691,  362,  363,  364,
 1229,  183,  184, 1237, 1232, 1239,  435, 1246, 1250,  633,
 1251,  258,  633,  694,  692, 1164,  693,  500,  695, 1252,
  365,  366,  420,  421,  422,  367,  368,  423,  424, 1253,
 1262, 1264, 1056,  153, 1271,  426,  427,  635,  633,  500,
  633,  633, 1274,  635, 1276, 1275,  500, 1277, 1279, 1280,
 1284, 1285,  633,  635,  223,  153,  633, 1326,  694,  692,
 1286,  693, 1288,  695, 1289, 1296, 1297,  635,  635,  635,
  635,  635,  635, 1329,  694,  692,  223,  693, 1298,  695,
  500,  635,  868,  223,  153, 1299, 1301, 1302, 1303, 1304,
  635,  635,  635,  635,  635,  635,  635,  635,  635, 1305,
  635, 1306,  861,  153, 1307,  635,  635, 1323,  635,  635,
  635, 1351,  153,  633,  635, 1341,  690,  223,  153,  633,
 1344, 1355,  691, 1358, 1359, 1360, 1356, 1375, 1377,  633,
 1357,  405, 1366, 1369,  532, 1371,  153,  196, 1385, 1389,
  406, 1397, 1390,  633,  633,  633,  633,  633,  633, 1330,
  694,  692, 1402,  693,  407,  695,  869,  633, 1391, 1392,
  360,  408, 1393, 1412, 1394, 1419,  633,  633,  633,  633,
  633,  633,  633,  633,  633, 1423,  633,  762, 1281,  621,
 1424,  633,  633, 1428,  633,  633,  633, 1434,  409, 1459,
  633,  410,  870, 1454, 1455, 1465,  196,  871, 1466, 1533,
 1538, 1484, 1535,  872,  177, 1536, 1537, 1539, 1072, 1540,
 1542, 1073, 1543, 1544,  386,  405, 1552, 1554,  762,  411,
  412,  762,  873,  434,  406, 1557,  432, 1561,  433, 1562,
 1563,  413, 1565, 1566, 1567,  414,  762, 1568,  407, 1571,
 1572,  435, 1573,  622,  360,  408,  258, 1574, 1332,  694,
  692,  690,  693, 1575,  695, 1576, 1577,  691, 1578, 1579,
 1580, 1588, 1605,  177,  694,  692, 1607,  693, 1589,  695,
  187,  111,  409, 1610, 1614,  410, 1623, 1333,  694,  692,
 1609,  693, 1611,  695, 1612, 1622,  623, 1334,  694,  692,
 1613,  693,  415,  695, 1615, 1616,  690, 1621,  416, 1624,
 1645, 1650,  691,  411,  412, 1651, 1659, 1683,  417, 1722,
 1617, 1675,  690, 1696, 1724,  413, 1726, 1708,  691,  414,
 1717, 1719,  362,  363,  364,  418,  183,  184, 1731,  187,
 1732, 1739, 1469, 1335,  694,  692,  419,  693, 1743,  695,
 1744, 1714, 1715, 1761,  138,  365,  366,  420,  421,  422,
  367,  368,  423,  424, 1766,  425, 1767, 1768, 1706, 1781,
  426,  427, 1784,  428,  429,  430, 1785, 1771,    1,  431,
  746, 1336,  694,  692, 1074,  693,  415,  695,  620,   30,
   32,  203,  416,  370,  271,  561,  644,  149,  690,  521,
  269,   82,  417,  928,  691,  694,  692,  457,  693,  213,
  695,  521,  193,  276,  398,  521,  362,  363,  364,  418,
  183,  184, 1697,  927,  281,  676,  196, 1337,  694,  692,
  419,  693,  674,  695,  670,  672,  275,  289,  371,  365,
  366,  420,  421,  422,  367,  368,  423,  424,  196,  425,
  703,  194,  287, 1082,  426,  427, 1073,  428,  429,  430,
  405,  702,  288,  431,  223,  704,  523,  325,  434,  406,
  613,  432,  177,  433, 1460,  694,  692,  196,  693,  332,
  695,  665,  323,  407,  713,   26,  435,   29,   39,  360,
  408,  258, 1312,  177, 1441,  116,  196,  690,  391,  400,
  881,  390,  829,  691,  196,  196,  762,  523, 1061,  762,
  523,  196,  690, 1272, 1270,  177,  762,  409,  691,  762,
  410,  177,  226,  762, 1258,  523,  690, 1268,  762,  196,
  563,  762,  691, 1546, 1247,  519,  690,  505,  187,  523,
  491,  196,  691,  196,  177,  967, 1198,  762,  411,  412,
  434, 1259,  794,  432, 1653,  433,  534,  535, 1003,  187,
  413, 1713, 1525,  177,  414, 1531,  196,  776,  435, 1754,
 1720,  762,  177,  258,  762,  762, 1718,  996,  177, 1672,
  523,  187,  690,  523,  762, 1779, 1187,  187,  691, 1379,
 1748,  196, 1363, 1461,  694,  692,  177,  693,  523,  695,
 1682,  762, 1468,  694,  692, 1487,  693,  780,  695,  543,
  187,  694,  692,  525,  693,  516,  695, 1037, 1409, 1074,
  690,  415,  642,  762, 1028,  816,  691,  416,  762,  187,
  665,  694,  692,  177,  693,  118,  695,  417,  187,  483,
  762,  711,  762,  690,  187, 1190,  715,  958, 1701,  691,
  972,  362,  363,  364,  418,  183,  184,  268,  954,  953,
 1649, 1760,  187,   95, 1730,  419,  690, 1789, 1171, 1593,
  724,  888,  691, 1292,  365,  366,  420,  421,  422,  367,
  368,  423,  424, 1134,  425,  641,  251,    0, 1086,  426,
  427, 1073,  428,  429,  430,  405, 1343,    0,  431,  187,
   81,    0,    0,  434,  406,   96,  432,   82,  433,    0,
    0,   83,   84,  690,    0,    0,    0,   97,  407,  691,
   85,  435,    0,    0,  360,  408,  258,  251,    0,    0,
  251,  113,   86,  908,  909,  910,  911,  912,  913,  914,
  915,    0,   98,    0,  523,  251,  523,    0,    0,    0,
    0,  523,  409,    0,  523,  410,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  523,    0,    0,
 1094,    0,    0, 1073,    0,    0,    0,  405,    0,    0,
    0,    0,    0,  411,  412,  434,  406,    0,  432,    0,
  433,    0,    0,    0,    0,  413,    0,   87,  659,  414,
  407,    0,    0,  435,    0,    0,  360,  408,  258,    0,
    0,   99,    0,    0,   88,    0,    0,  523,    0,  523,
    0,  100,    0,    0,    0,    0,    0,  523,    0,   89,
    0,    0,  690,   90,  409,    0,    0,  410,  691,  659,
  523,  690,  659,    0,   91,  523,  101,  691,    0,  690,
    0,    0,    0,    0, 1074,  691,  415,  659,    0,    0,
    0,    0,  416,   92,  102,  411,  412,  523,    0,  690,
    0,    0,  417,    0,  523,  691,    0,  413,    0,    0,
    0,  414,    0,    0,    0,    0,  362,  363,  364,  418,
  183,  184,    0,    0,    0,    0,    0,    0,    0,    0,
  419,    0,    0,    0,    0,    0,    0,    0,  523,  365,
  366,  420,  421,  422,  367,  368,  423,  424,  523,  425,
    0,    0,    0, 1097,  426,  427, 1073,  428,  429,  430,
  405,    0,    0,  431,    0,    0, 1074,    0,  415,  406,
  523,    0,    0,    0,  416,    0,    0,  523,    0,    0,
    0,    0,    0,  407,  417,    0,    0,    0,    0,  360,
  408,    0,    0,    0,  251,    0,  251,    0,  362,  363,
  364,  418,  183,  184,  251,    0,    0,    0,    0,    0,
    0,  523,  419,    0,    0,    0,    0,  409,    0,    0,
  410,  365,  366,  420,  421,  422,  367,  368,  423,  424,
    0,  425,    0,    0,    0, 1102,  426,  427, 1073,  428,
  429,  430,  405,    0,    0,  431,    0,    0,  411,  412,
  434,  406,    0,  432,    0,  433,    0,    0,    0,    0,
  413,    0,    0,  656,  414,  407,    0,  188,  435,    0,
    0,  360,  408,  258,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  251,    0,    0,    0,  409,
    0,    0,  410,    0,  656,    0,    0,  656,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  251,    0, 1074,
    0,  415,  656,    0,  251,    0,  188,  416,    0,    0,
  411,  412,    0,    0,    0,    0,    0,  417,    0,    0,
    0,    0,  413,    0,    0,    0,  414,  659,    0,    0,
  659,  362,  363,  364,  418,  183,  184,  659,  251,    0,
  659,    0,    0,    0,  659,  419,    0,    0,    0,  659,
    0,    0,  659,    0,  365,  366,  420,  421,  422,  367,
  368,  423,  424,    0,  425,    0,   59,    0,  659,  426,
  427,    0,  428,  429,  430,    0,    0,   60,  431,    0,
    0, 1074,    0,  415,   61,    0,    0,    0,    0,  416,
    0,    0,  659,    0,    0,    0,  659,   62,    0,  417,
    0,    0,    0,    0,    0,  659,    0,    0,    0,    0,
    0,    0,    0,  362,  363,  364,  418,  183,  184,    0,
    0,    0,  659,    0,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  365,  366,  420,  421,
  422,  367,  368,  423,  424,    0,  425,    0,    0,  659,
 1105,  426,  427, 1073,  428,  429,  430,  405,   63,    0,
  431,  659,    0,    0,    0,  434,  406,    0,  432,    0,
  433,    0,    0,    0,   64, 1431, 1432,    0,    0,  810,
  407, 1436, 1437,  435, 1439, 1440,  360,  408,  258,    0,
    0,   65, 1444, 1445, 1446, 1447,    0, 1448, 1449, 1450,
 1451, 1452, 1453,    0,    0,  188,   66,    0,    0,    0,
   67,    0,    0,   68,  409,    0,    0,  410,    0,    0,
  810,   69,    0,  810,    0,    0,  188,    0,    0,  434,
    0,  463,  432,    0,  433,    0,    0,    0,  810,    0,
   70,    0,    0,    0,    0,  411,  412,  435,  188,    0,
    0,    0,  258,    0,  188,    0,    0,  413,    0,    0,
    0,  414,  656,    0,    0,  656,    0,    0,    0,    0,
    0,    0,  656,    0,    0,  656,    0,  188,    0,  656,
    0,    0,    0,    0,  656,    0,    0,  656,    0,    0,
    0,    0,    0,    0,    0,    0,  188,    0,    0, 1377,
    0,    0,    0,  656,    0,  188,    0,    0,    0,    0,
    0,  188,    0,    0,    0,    0, 1074,    0,  415,    0,
    0,    0,    0,    0,  416,    0,    0,  656,    0,  188,
    6,  656,    0,    0,  417,    0,    0,    0,    0,    0,
  656, 1494, 1495,    0, 1496,    0,    0, 1497,  362,  363,
  364,  418,  183,  184,    0,    0,    0,  656,    0,    0,
    0, 1498,  419,    0,    0,    0,  188, 1499,    0,    0,
    0,  365,  366,  420,  421,  422,  367,  368,  423,  424,
    0,  425,    0,    0,  656, 1108,  426,  427, 1073,  428,
  429,  430,  405,    0,    0,  431,  656,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,    0,  434,    0,
    0,  432,    0,  433,    0,  407,    0,    0,    0,    0,
    0,  360,  408,    0,    0,    0,  435, 1500,    0,    0,
    0,  258,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  810,    0,    0,    0,    0,   14,    0,    0,  409,
    0,    0,  410,    0,    0,    0,  405,    0,    0,    0,
    0, 1501,    0,    0,    0,  406,    0,    0,    0,    0,
    0,   16,  939,    0,  939,  939,    0,  939,    0,  407,
  411,  412, 1502,    0,    0,  360,  408,    0,    0,  484,
  939,    0,  413,    0,    0,  939,  414,    0,  810,    0,
    0,  810,    0,    0,    0,    0,    0,    0,  810,    0,
    0,  810,    0,  409,    0,  810,  410,    0, 1503,    0,
  810,    0,    0,  810,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  810,
    0,    0,  361,    0,  411,  412,    0,    0,    0,    0,
    0, 1074,    0,  415,    0,    0,  413,    0,    0,  416,
  414,    0,    0,  810,    0,    0,  810,  810,    0,  417,
    0,    0,    0,    0,    0,    0,  810,    0,    0,    0,
    0,    0,    0,  362,  363,  364,  418,  183,  184,    0,
    0,    0,    0,  810,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  365,  366,  420,  421,
  422,  367,  368,  423,  424,  810,  425,  415,    0,    0,
  810,  426,  427,  416,  428,  429,  430,    0,    0,    0,
  431,    0,  810,  417,  810,  405,    0,    0,    0,    0,
    0,    0,    0,    0,  406,    0,    0,  362,  363,  364,
  418,  183,  184,    0,    0,    0,    0,    0,  407,    0,
    0,  419,    0,    0,  360,  408,    0,    0,    0,    0,
  365,  366,  420,  421,  422,  367,  368,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,    0,  428,  429,
  430,    0,  409,    0,  431,  410,    0,    0,    0,  939,
    0,    0,    0,    0,    0,    0,    0,  635,  939,    0,
  635,    0,  635,    0,    0,    0,    0,    0,    0,    0,
    0,  361,  939,  411,  412,  635,    0,    0,  939,  939,
  635,    0,    0,    0,    0,  413,    0,    0,    0,  414,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  939,    0,    0,  939,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  434,    0,    0,  432,    0,  433,  317,    0,    0,
    0,    0,    0,    0,    0,  939,  684,  939,  939,  435,
    0,    0,    0,    0,  258,    0,  415,    0,    0,  939,
    0,    0,  416,  939,    0,    0,    0,    0,  317,    0,
    0,    0,  417,    0,    0,  317,    0,    0,    0,  317,
  317,    0,  317,    0,    0,  317,  362,  363,  364,  418,
  183,  184,    0,    0,    0,    0,    0,    0,    0,  317,
  419,    0,    0,    0,    0,  317,    0,    0,    0,  365,
  366,  420,  421,  422,  367,  368,  423,  424,    0,  425,
  939,    0,    0,    0,  426,  427,  939,  428,  429,  430,
    0,    0,    0,  431,    0,    0,  939,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  939,  939,  939,  939,  939,  939,    0,    0,    0,    0,
    0,    0,    0,    0,  939,  317,    0,    0,    0,    0,
    0,    0,    0,  939,  939,  939,  939,  939,  939,  939,
  939,  939,    0,  939,  317,    0,    0,  635,  939,  939,
    0,  939,  939,  939,  635,    0,    0,  939,    0,  317,
    0,    0,    0,  635,    0,    0,    0,    0,    0,  317,
  434,    0,    0,  432,    0,  433,    0,  635,  317,    0,
  317,    0,    0,  635,  635,    0,    0,  317,  435,    0,
    0,  635,    0,  258,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  635,    0,    0,  635,    0,  317,    0,  405,    0,
    0,    0,    0,    0,    0,    0,    0,  406,    0,    0,
    0,    0,    0,    0,  434,    0,    0,  432,    0,  433,
    0,  407,  635,  635,    0,    0,    0,  360,  408,    0,
    0,    0,  435,    0,  635,    0,    0,  258,  635,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  453,
    0,    0,    0,    1,    0,  409,    0,    0,  410,  453,
  453,  453,  453,  453,  453,  453,  453,  453,  453,  453,
  453,  453,  453,  453,    0,    2,  453,  453,  453,  453,
    0,  453,    0,    3,  453,  453,  411,  412,    0,    0,
    4,    0,    5,    0,    6,  635,    0,    0,  413,    7,
    0,  635,  414,    0,    0,    0,    8,    0,    0,    0,
    0,  635,    0,    0,    0,    0,    0,    0,    0,    0,
    9,    0,    0,    0,    0,  635,  635,  635,  635,  635,
  635,   10,    0,    0,    0,    0,    0,    0,    0,  635,
    0,    0,    0,    0,    0,    0,    0,    0,  635,  635,
  635,  635,  635,  635,  635,  635,  635,    0,  635,  415,
    0,    0,    0,  635,  635,  416,  635,  635,  635,    0,
    0,    0,  635,    0,    0,  417,    0,  405,    0,    0,
    0,    0,    0,  676,    0,    0,  406,    0,    0,  362,
  363,  364,  418,  183,  184,    0,    0,    0,   11,    0,
  407,    0,    0,  419,    0,   12,  360,  408,   13,    0,
   14,   15,  365,  366,  420,  421,  422,  367,  368,  423,
  424,    0,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,    0,  409,   16,  431,  410,    0,    0,
    0,  405,    0,    0,    0,    0,    0,    0,    0,    0,
  406,  434,  931,    0,  432,    0,  433,    0,    0,    0,
    0,    0,    0,    0,  407,  411,  412,    0,    0,  435,
  360,  408,    0,    0,  258,    0,    0,  413,    0,    0,
    0,  414,    0,    0,   17,    0,    0,    0,    0,    0,
    0,  779,    0,    0,    0,    0,    0,    0,  409,   18,
    0,  410,  929,  694,  692,    0,  693,    0,  695,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  859,  858,  860,    0,    0,    0,  361,  684,  411,
  412,    0,  779,    0,  434,  779,    0,  432,  415,  433,
    0,  413,    0,    0,  416,  414,    0,    0,    0,    0,
  779,    0,  435,    0,  417,    0,    0,  258,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  362,  363,
  364,  418,  183,  184,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  365,  366,  420,  421,  422,  367,  368,  423,  424,
    0,  425,  415,    0,    0,    0,  426,  427,  416,  428,
  429,  430,    0,    0,    0,  431,    0,    0,  417,    0,
    0,  221,    0,    0,    0,  221,    0,    0,    0,    0,
    0,    0,  362,  363,  364,  418,  183,  184,    0,    0,
  221,    0,    0,    0,  221,    0,  419,  221,    0,    0,
    0,    0,    0,    0,    0,  365,  366,  420,  421,  422,
  367,  368,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,    0,    0,  405,  431,
    0,    0,    0,    0,    0,    0,    0,  406,  221,    0,
    0,    0,    0,    0,  221,  221,  221,    0,  221,  221,
  221,  407,  221,    0,    0,    0,    0,  360,  408,    0,
    0,    0,    0,    0,  434,    0,    0,  432,    0,  433,
    0,    0,  221,    0,  221,    0,    0,    0,    0,    0,
    0,    0,  435,  846,    0,  409,    0,  258,  410,    0,
    0,  690,    0,    0,    0,    0,    0,  691,    0,    0,
  847,    0,    0,  779,    0,  221,    0,    0,    0,    0,
   81,  405,    0,    0,    0,    0,  411,  412,    0,    0,
  406,    0,  221,    0,    0,    0,    0,    0,  413,    0,
    0,    0,  414,    0,  407,    0,  848,    0,    0,    0,
  360,  408,    0,    0,  221,  849,  434,    0,    0,  432,
    0,  433,    0,  850,    0,    0,    0,    0,    0,    0,
  779,    0,    0,  779,  435,    0,    0,    0,  409,  258,
  779,  410,  851,  779,  852,    0,    0,  779,    0,  221,
  221,    0,  779,    0,    0,  779,    0,    0,  853,  415,
  854,  855,  856,    0,    0,  416,  221,    0,    0,  411,
  412,  779,    0,    0,    0,  417,    0,    0,    0,    0,
    0,  413,    0,    0,    0,  414,    0,    0,    0,  362,
  363,  364,  418,  183,  184,  779,    0,    0,  779,  779,
    0,    0,    0,  419,    0,    0,    0,    0,  779,    0,
    0,  857,  365,  366,  420,  421,  422,  367,  368,  423,
  424,    0,  425,    0,    0,  779,    0,  426,  427,    0,
  428,  429,  430,    0,    0,    0,  431,    0,    0,    0,
    0,    0,  415,    0,    0,    0,    0,  779,  416,    0,
    0,    0,  779,    0,    0,    0,    0,    0,  417,    0,
    0,  405,    0,    0,  779,    0,  779,    0,    0, 1100,
  406,    0,  362,  363,  364,  418,  183,  184,    0,    0,
    0,    0,    0,    0,  407,    0,  419,  780,    0,    0,
  360,  408,    0,    0,    0,  365,  366,  420,  421,  422,
  367,  368,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,  832,    0,  409,  431,
  565,  410,    0,    0,    0,    0,    0,    0,  780,    0,
    0,  780,    0,    0,    0,  833,    0,    0,    0,  566,
    0,    0,  567,  405,    0,    0,  780,    0,    0,  411,
  412,  434,  406,    0,  432,    0,  433,    0,    0,    0,
    0,  413,    0,    0,    0,  414,  407,    0,    0,  435,
 1655,    0,  360,  408,  258,    0,    0,    0,    0,    0,
    0,    0,    0,  568,    0,    0,    0,    0,    0,  569,
  570,  571,    0,  572,  573,  574,    0,  575,    0,    0,
  409,    0,    0,  410,    0,    0,    0,    0,    0,    0,
    0,    0, 1281,    0,    0,  694,  692,  576,  693,  577,
  695,    0,  415,    0,    0,    0,    0,    0,  416,    0,
    0,  411,  412,  859,  858,  860,    0,    0,  417,    0,
    0,    0,    0,  413,    0,    0,    0,  414,    0,    0,
  578,    0,  362,  363,  364,  418,  183,  184,    0,    0,
    0,    0,    0,    0,    0,    0,  419,  579,    0,    0,
    0,    0,    0,    0,    0,  365,  366,  420,  421,  422,
  367,  368,  423,  424,    0,  425,    0,    0,    0,  580,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
    0,    0,    0,    0,  415,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  417,    0,    0,    0,  581,  582,    0,    0,    0,    0,
    0, 1294,    0,    0,  362,  363,  364,  418,  183,  184,
    0,  583,    0,    0,    0,    0,    0,    0,  419,  780,
    0,    0,    0,    0,    0,    0,    0,  365,  366,  420,
  421,  422,  367,  368,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,  405, 1698,
    0,  431,  432,    0,  433,    0,    0,  406,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  435,    0,    0,
    0,  407,  258,    0,    0,    0,  780,  360,  408,  780,
    0,    0,    0,    0,    0,    0,  780,    0,    0,  780,
    0,    0,    0,  780,    0,    0,    0,    0,  780,    0,
    0,  780,    0,    0,    0,  409,    0,    0,  410,    0,
    0,    0,    0,    0,    0,  846,    0,  780,    0,    0,
    0,    0,    0,  690,    0,    0,    0,    0,    0,  691,
    0,    0,  847,    0,    0,    0,  411,  412,    0,    0,
    0,  780,    0,    0,  780,  780,  434,    0,  413,  432,
    0,  433,  414,    0,  780,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  435,    0,    0,    0,  848,  258,
    0,  780,    0,    0,    0,    0,    0,  849,    0,    0,
    0,    0,    0,    0,    0,  850,    0, 1377,    0,    0,
    0,    0,    0,  780,    0,    0,    0,    0,  780,    0,
    0,    0,    0,    0,  851,    0,  852,    0,    0,  415,
  780,    0,  780,    0,    0,  416,    0,    0,    6,    0,
  853,    0,  854,  855,  856,  417,    0,    0,  939, 1494,
 1495,  939, 1496,  939,    0, 1497,    0,    0,    0,  362,
  363,  364,  418,  183,  184,    0,  939,    0,    0, 1498,
    0,  939,    0,  419,    0, 1499,    0,    0,    0,    0,
    0,    0,  365,  366,  420,  421,  422,  367,  368,  423,
  424,    0,  425,  857,    0,    0,  405,  426,  427,    0,
  428,  429,  430,    0,    0,  406,  431,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,    0,    0,    0,  360,  408,    0,    0,    0,
    0,    0,    0,    0,  564, 1500,    0,    0,  565,    0,
    0,    0,    0,    0,    0,    0,  760,    0,    0,    0,
    0,    0,    0,  409,   14,    0,  410,  566,    0,    0,
  567,    0,    0,    0,    0,    0,    0,    0,    0, 1501,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   16,
    0,    0,  361,    0,  411,  412,    0,    0, 1617,    0,
 1502,    0,    0,  405,    0,    0,  413,  484,    0,    0,
  414,  568,  406,    0,    0,    0,    0,  569,  570,  571,
    0,  572,  573,  574,    0,  575,  407,    0,    0,    0,
    0,    0,  360,  408,    0,    0, 1503,    0,    0,    0,
    0,    0,    0,    0,    0,  576,    0,  577,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  409,    0,    0,  410,    0,    0,    0,  415,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,  578,    0,
    0,    0,    0,  417,    0,  939,    0,    0,    0,    0,
    0,  411,  412,    0,  939,  579,    0,  362,  363,  364,
  418,  183,  184,  413,    0,    0,    0,  414,  939,    0,
    0,  419,    0,    0,  939,  939,  945,  580,    0,    0,
  365,  366,  420,  421,  422,  367,  368,  423,  424,    0,
  425,    0,  188,    0,    0,  426,  427,    0,  428,  429,
  430,    0,  939,    0,  431,  939,    0,    0,    0,    0,
    0,    0,  581,  582,    0,    0,  941,  945,  945,  945,
  945,  945,  957,  945,  415,    0,    0,    0,    0,  583,
  416,    0,    0,  939,  939,  945,  945,  945,  945,    0,
  417,    0,    0,    0,    0,  939,    0,    0,    0,  939,
    0,    0,    0,    0,  362,  363,  364,  418,  183,  184,
    0,    0,    0,    0,    0,    0,    0,  945,  419,  945,
    0,    0,    0,    0,    0,    0,    0,  365,  366,  420,
  421,  422,  367,  368,  423,  424,    0,  425,    0,    0,
    0,    0,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0,    0,    0,  939,    0,    0,    0,
    0,    0,  939,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  939,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  939,  939,  939,  939,
  939,  939,    0,    0,    0,    0,  825,    0,    0,    0,
  939,    0,    0,    0,    0,    0,    0,    0,    0,  939,
  939,  939,  939,  939,  939,  939,  939,  939,    0,  939,
    0,    0,    0,    0,  939,  939,    0,  939,  939,  939,
    0,    0,    0,  939,    0,    0,    0,  825,  825,  825,
  825,  825,    0,  825,    0,    0,    0,    0,    0,    0,
  142,    0,    0,    0,    0,  825,  825,  825,  825,  143,
  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
  154,  155,  156,  157,  158,    0,    0,  159,  160,  161,
  162,    0,  163,    0,    0,  164,  165,    0,  945,  825,
  945,    0,    0,    0,    0,    0,    0,    0,  945,    0,
    0,    0,    0,    0,    0,    0,  945,    0,    0,    0,
    0,    0,  945,    0,    0,  945,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  837,  945,  945,    0,    0,  945,    0,
    0,    0,    0,    0,    0,  945,    0,  945,  945,    0,
    0,  945,    0,    0,    0,  945,    0,    0,  945,    0,
  945,    0,  945,    0,    0,    0,    0,  945,  945,    0,
  945,    0,    0,    0,  837,  837,  837,  837,  837,    0,
  837,    0,    0,    0,    0,    0,  945,  945,    0,  945,
    0,    0,  837,  837,  837,  837,    0,    0,    0,    0,
    0,    0,    0,  945,    0,  945,  945,  945,    0,    0,
  945,    0,    0,  945,  945,    0,    0,    0,    0,    0,
    0,  564,    0,  945,    0,  565,  837,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  945,    0,    0,
  945,    0,    0,    0,  566,    0,    0,  567,    0,    0,
    0,    0,    0,    0,    0,    0,  945,    0,  825,    0,
  825,    0,  945,    0,    0,    0,    0,  945,  825,    0,
    0,    0,    0,    0,    0,    0,  825,    0,  945,  945,
    0,  945,  825,    0,    0,  825,    0,  945,  568,    0,
    0,    0,    0,    0,  569,  570,  571,    0,  572,  573,
  574,    0,  575,  840,  825,  825,    0,    0,  825,    0,
    0,    0,    0,    0,    0,  825,    0,  825,  825,    0,
    0,  825,  576,    0,  577,  825,    0,    0,  825,    0,
  825,    0,  825,    0,    0,    0,    0,  825,  825,    0,
  825,    0,    0,    0,  840,  840,  840,  840,  840,    0,
  840,    0,    0,    0,    0,  578,  825,  825,    0,  825,
    0,    0,  840,  840,  840,  840,    0,    0,    0,    0,
    0,    0,  579,  825,    0,  825,  825,  825,    0,    0,
  825,    0,    0,  825,  825,    0,    0,    0,    0,    0,
    0,    0,    0,  825,  580,  837,  840,  837,    0,    0,
    0,    0,    0,    0,    0,  837,    0,  825,    0,  188,
  825,    0,    0,    0,    0,    0,    0,    0,    0,  837,
    0,    0,  837,    0,    0,    0,  825,    0,    0,  581,
  582,    0,  825,    0,    0,    0,    0,  825,    0,    0,
  841,  837,  837,    0,    0,  837,  583,    0,  825,  825,
    0,  825,  837,    0,  837,  837,    0,  825,  837,    0,
    0,    0,  837,    0,    0,  837,    0,  837,    0,  837,
    0,    0,    0,    0,  837,  837,    0,  837,    0,    0,
    0,  841,  841,  841,  841,  841,    0,  841,    0,    0,
    0,    0,    0,  837,  837,    0,  837,    0,    0,  841,
  841,  841,  841,    0,    0,    0,    0,    0,    0,    0,
  837,    0,  837,  837,  837,    0,    0,  837,    0,    0,
  837,  837,    0,    0,    0,    0,    0,    0,  564,    0,
  837,    0,  565,  841,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  837,    0,    0,  837,    0,    0,
    0,  566,    0,    0,  567,    0,    0,    0,    0,    0,
    0,    0,    0,  837,    0,  840,    0,  840,    0,  837,
    0,    0,    0,    0,  837,  840,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  837,  837,    0,  837,    0,
    0,    0,  840,    0,  837,  568,    0,    0,    0,    0,
    0,  569,  570,  571,    0,  572,  573,  574,    0,  575,
  835,  840,  840,    0,    0,  840,    0,    0,    0,    0,
    0,    0,  840,    0,  840,  840,    0,    0,  840,  576,
    0,  577,  840,    0,    0,  840,    0,  840,    0,  840,
    0,    0,    0,    0,  840,  840,    0,  840,    0,    0,
    0,  835,    0,  835,  835,  835,    0,    0,    0,    0,
    0,    0,  578,  840,  840,    0,  840,    0,    0,  835,
  835,  835,  835,    0,    0,    0,    0,    0,    0,  579,
  840,    0,  840,  840,  840,    0,    0,  840,    0,    0,
  840,  840,    0,    0,    0,    0,    0,    0,    0,    0,
  840,  580,  841,  835,  841,    0,    0,    0,    0,    0,
    0,    0,  841,    0,  840,    0,    0,  840,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  841,
    0,    0,    0,  840,    0,    0,  581,  582,    0,  840,
    0,    0,    0,    0,  840,    0,    0,  834,  841,  841,
    0,    0,  841,  583,    0,  840,  840,    0,  840,  841,
    0,  841,  841,    0,  840,  841,    0,    0,    0,  841,
    0,    0,  841,    0,  841,    0,  841,    0,    0,    0,
    0,  841,  841,    0,  841,    0,    0,    0,  834,    0,
  834,  834,  834,    0,    0,    0,    0,    0,    0,    0,
  841,  841,    0,  841,    0,    0,  834,  834,  834,  834,
    0,    0,    0,    0,    0,    0,    0,  841,    0,  841,
  841,  841,    0,    0,  841,    0,    0,  841,  841,    0,
    0,    0,    0,    0,    0,  134,    0,  841,    0,  134,
  834,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  841,    0,    0,  841,    0,    0,    0,  134,    0,
    0,  134,    0,    0,    0,    0,    0,    0,    0,    0,
  841,    0,  835,    0,  835,    0,  841,    0,    0,    0,
    0,  841,  835,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  841,  841,    0,  841,    0,    0,    0,  835,
    0,  841,  134,    0,    0,    0,    0,    0,  134,  134,
  134,    0,  134,  134,  134,    0,  134,  836,  835,  835,
    0,    0,  835,    0,    0,    0,    0,    0,    0,  835,
    0,  835,  835,    0,    0,  835,  134,    0,  134,  835,
    0,    0,  835,    0,  835,    0,  835,    0,    0,    0,
    0,  835,  835,    0,  835,    0,    0,    0,  836,    0,
  836,  836,  836,    0,    0,    0,    0,    0,    0,  134,
  835,  835,    0,  835,    0,    0,  836,  836,  836,  836,
    0,    0,    0,    0,    0,    0,  134,  835,    0,  835,
  835,  835,    0,    0,  835,    0,    0,  835,  835,    0,
    0,    0,    0,    0,    0,    0,    0,  835,  134,  834,
  836,  834,    0,    0,    0,    0,    0,    0,    0,  834,
    0,  835,    0,    0,  835,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  834,    0,    0,    0,
  835,    0,    0,  134,  134,    0,  835,    0,    0,    0,
    0,  835,    0,    0,  839,  834,  834,    0,    0,  834,
  134,    0,  835,  835,    0,  835,  834,    0,  834,  834,
    0,  835,  834,    0,    0,    0,  834,    0,    0,  834,
    0,  834,    0,  834,    0,    0,    0,    0,  834,  834,
    0,  834,    0,    0,    0,  839,    0,  839,  839,  839,
    0,    0,    0,    0,    0,    0,    0,  834,  834,    0,
  834,    0,    0,  839,  839,  839,  839,    0,    0,    0,
    0,    0,    0,    0,  834,    0,  834,  834,  834,    0,
    0,  834,    0,    0,  834,  834,    0,    0,    0,    0,
    0,    0,    0,    0,  834,    0,    0,  839,    0,    0,
  804,    0,    0,    0,    0,    0,    0,    0,  834,    0,
    0,  834,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  834,    0,  836,
    0,  836,    0,  834,    0,    0,    0,    0,  834,  836,
    0,  804,    0,    0,  804,    0,    0,    0,    0,  834,
  834,    0,  834,    0,    0,    0,  836,    0,  834,  804,
    0,    0,    0,  783,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  836,  836,    0,    0,  836,
    0,    0,    0,    0,    0,    0,  836,    0,  836,  836,
    0,    0,  836,    0,    0,    0,  836,    0,    0,  836,
    0,  836,    0,  836,  783,    0,    0,  783,  836,  836,
    0,  836,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  783,    0,    0,    0,    0,  836,  836,    0,
  836,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  781,  836,    0,  836,  836,  836,    0,
    0,  836,    0,    0,  836,  836,    0,    0,    0,    0,
    0,    0,    0,    0,  836,    0,  839,    0,  839,    0,
    0,    0,    0,    0,    0,    0,  839,    0,  836,    0,
    0,  836,    0,    0,  781,    0,    0,  781,    0,    0,
    0,    0,    0,  839,    0,    0,    0,  836,    0,    0,
    0,    0,  781,  836,    0,    0,    0,    0,  836,    0,
    0,    0,  839,  839,    0,    0,  839,    0,    0,  836,
  836,    0,  836,  839,    0,  839,  839,  782,  836,  839,
    0,    0,    0,  839,    0,    0,  839,    0,  839,    0,
  839,    0,    0,    0,    0,  839,  839,    0,  839,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  812,  804,    0,  839,  839,    0,  839,  782,    0,
    0,  782,  776,    0,    0,    0,    0,    0,    0,    0,
    0,  839,    0,  839,  839,  839,  782,    0,  839,    0,
    0,  839,  839,    0,    0,    0,    0,    0,    0,    0,
    0,  839,  812,    0,    0,  812,    0,    0,    0,    0,
    0,    0,    0,  776,    0,  839,  776,    0,  839,  804,
  812,    0,  804,    0,    0,  783,    0,    0,    0,  804,
    0,  776,  804,    0,  839,    0,  804,    0,    0,    0,
  839,  804,    0,    0,  804,  839,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  839,  839,    0,  839,
  804,    0,    0,    0,    0,  839,    0,    0,    0,    0,
    0,  777,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  783,    0,  804,  783,    0,  804,  804,    0,
    0,    0,  783,    0,    0,  783,    0,  804,    0,  783,
    0,    0,    0,    0,  783,  781,    0,  783,    0,    0,
    0,    0,  777,    0,  804,  777,    0,    0,    0,    0,
    0,    0,    0,  783,    0,    0,    0,    0,    0,    0,
  777,    0,    0,    0,    0,    0,  804,    0,    0,    0,
    0,  804,    0,    0,    0,    0,    0,  783,    0,    0,
  783,  783,    0,  804,    0,  804,    0,    0,    0,    0,
  783,  778,  781,    0,    0,  781,    0,  811,    0,    0,
    0,    0,  781,    0,    0,  781,    0,  783,    0,  781,
    0,    0,    0,    0,  781,    0,    0,  781,    0,  782,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  783,
    0,    0,  778,  781,  783,  778,    0,    0,  811,    0,
    0,  811,    0,    0,    0,    0,  783,    0,  783,    0,
  778,    0,    0,  812,    0,    0,  811,  781,    0,    0,
  781,  781,    0,    0,  776,    0,    0,    0,    0,    0,
  781,    0,    0,    0,  805,    0,  782,    0,    0,  782,
    0,    0,    0,    0,    0,    0,  782,  781,    0,  782,
    0,    0,    0,  782,    0,    0,    0,    0,  782,    0,
    0,  782,    0,    0,    0,    0,    0,    0,    0,  781,
  812,    0,    0,  812,  781,  805,    0,  782,  805,    0,
  812,  776,    0,  812,  776,    0,  781,  812,  781,    0,
    0,  776,  812,  805,  776,  812,    0,    0,  776,    0,
    0,  782,    0,  776,  782,  782,  776,    0,    0,    0,
    0,  812,    0,    0,  782,    0,    0,    0,    0,    0,
  813,    0,  776,  777,    0,    0,    0,    0,    0,    0,
    0,  782,    0,    0,    0,  812,    0,    0,  812,  812,
  814,    0,    0,    0,    0,    0,  776,    0,  812,  776,
  776,    0,    0,  782,    0,    0,    0,    0,  782,  776,
    0,  813,    0,    0,  813,  812,    0,    0,    0,    0,
  782,    0,  782,    0,    0,    0,  776,    0,    0,  813,
  777,  814,    0,  777,  814,    0,    0,  812,    0,    0,
  777,    0,  812,  777,    0,    0,    0,  777,  776,  814,
    0,    0,  777,  776,  812,  777,  812,    0,    0,    0,
    0,    0,    0,  778,    0,  776,    0,  776,    0,  811,
    0,  777,    0,  802,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  777,    0,    0,  777,  777,
    0,    0,    0,    0,    0,    0,    0,    0,  777,    0,
    0,    0,    0,    0,  802,    0,    0,  802,  806,    0,
  778,    0,    0,  778,    0,  777,  811,    0,    0,  811,
  778,    0,  802,  778,    0,    0,  811,  778,    0,  811,
    0,    0,  778,  811,    0,  778,  805,  777,  811,    0,
    0,  811,  777,    0,    0,    0,    0,    0,    0,  806,
    0,  778,  806,    0,  777,    0,  777,  811,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  806,    0,    0,
    0,    0,    0,    0,    0,  778,    0,    0,  778,  778,
    0,  811,    0,    0,  811,  811,    0,    0,  778,    0,
    0,    0,    0,  805,  811,  815,  805,    0,    0,    0,
    0,    0,    0,  805,    0,  778,  805,    0,    0,    0,
  805,  811,    0,    0,    0,  805,  803,    0,  805,    0,
    0,    0,  813,    0,    0,    0,    0,  778,    0,    0,
    0,    0,  778,  811,  805,    0,  815,    0,  811,  815,
    0,    0,  814,    0,  778,    0,  778,    0,    0,    0,
  811,    0,  811,    0,  815,    0,    0,  803,  805,    0,
  803,  805,  805,    0,    0,    0,    0,    0,    0,    0,
    0,  805,    0,    0,    0,  803,    0,    0,    0,  813,
    0,    0,  813,    0,    0,    0,    0,    0,  805,  813,
    0,    0,  813,    0,    0,    0,  813,    0,    0,  814,
    0,  813,  814,    0,  813,    0,    0,    0,    0,  814,
  805,    0,  814,    0,    0,  805,  814,    0,    0,    0,
  813,  814,  807,    0,  814,  802,    0,  805,    0,  805,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  814,    0,    0,    0,  813,    0,    0,  813,  813,    0,
    0,    0,    0,    0,    0,    0,    0,  813,    0,    0,
    0,    0,    0,  807,  814,    0,  807,  814,  814,    0,
  806,    0,    0,    0,  813,    0,    0,  814,    0,    0,
    0,  807,  802,    0,    0,  802,    0,    0,    0,    0,
    0,    0,  802,    0,  814,  802,  813,    0,    0,  802,
    0,  813,    0,    0,  802,    0,    0,  802,    0,    0,
    0,    0,    0,  813,    0,  813,  814,    0,    0,    0,
    0,  814,    0,  802,    0,    0,    0,  806,    0,    0,
  806,    0,    0,  814,    0,  814,    0,  806,    0,    0,
  806,    0,    0,    0,  806,    0,    0,  802,    0,  806,
  802,  802,  806,    0,    0,    0,    0,  815,    0,    0,
  802,    0,    0,    0,    0,    0,    0,    0,  806,    0,
    0,    0,    0,    0,    0,    0,    0,  802,  803,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  806,    0,    0,  806,  806,    0,    0,  802,
    0,    0,    0,    0,  802,  806,    0,    0,    0,    0,
    0,    0,    0,    0,  815,    0,  802,  815,  802,    0,
    0,    0,  806,    0,  815,    0,    0,  815,    0,    0,
    0,  815,    0,    0,    0,  803,  815,    0,  803,  815,
    0,    0,    0,    0,  806,  803,    0,    0,  803,  806,
    0,    0,  803,    0,    0,  815,    0,  803,    0,    0,
  803,  806,    0,  806,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  803,    0,    0,  815,
    0,    0,  815,  815,  807,    0,    0,    0,    0,    0,
    0,    0,  815,    0,    0,    0,    0,    0,    0,    0,
  803,    0,    0,  803,  803,    0,    0,    0,    0,  815,
    0,    0,    0,  803,  221,    0,    0,    0,  221,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  803,  815,    0,  221,    0,    0,  815,  221,    0,    0,
  221,  807,    0,    0,  807,    0,    0,    0,  815,    0,
  815,  807,  803,    0,  807,    0,    0,  803,  807,    0,
    0,    0,    0,  807,    0,    0,  807,    0,    0,  803,
    0,  803,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,  807,    0,    0,    0,    0,  221,  221,  221,
    0,  221,  221,  221,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  807,    0,    0,  807,
  807,  541,    0,    0,    0,  221,    0,  221,    0,  807,
  143,  144,  145,  146,  147,  148,  149,  150,  151,  152,
  153,  154,  155,  156,  157,  158,  807,    0,  159,  160,
  161,  162,    0,  163,    0,    0,  164,  165,  221,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  807,    0,
    0,    0,    0,  807,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  807,    0,  807,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,  221,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
};
static short yycheck[] = {                                      60,
   16,   52,  130,   61,    4,    4,  101,   59,  295,    0,
  235,   53,  235,   60,  119,  120,  488,  338,   54,  119,
  120,  330,   64,  524,   82,  338,  527,  334,  729,  495,
   66,  309,  533,   67,  533,   86,  681,  307,   70,  470,
   85,  266,  737,  667,  657,   87,  886,  286,   91,  110,
  243,  112,   94,  622,  994,  624,   90,   65,  994,  252,
   92, 1001,  994, 1172,   80, 1001, 1216, 1001,  393, 1001,
  994,  684,  616,   89,  728,  111, 1002, 1001,    0,  124,
   88,  343,  730,  731,    0,   40,   42, 1013,  928,  131,
  133,  609,    0,  661,    0,  355,  132,  302,    0,  795,
  257,  725,  826,  757,  261,    0, 1032,  676,   63,  266,
   40,    0,  766,  208, 1026,   63,   44,  169,   41,   41,
    0,    0,   44,   41,   41,   41, 1049,   44,   44,   41,
   59,   40,  697,   41,   44,  235,   44,   59,   59,   41,
   40,  193,   44,   59,   46,  370,   40,  370,   44,    0,
    0,   59,   41,   59,   42,   44,  399,   59,  286,   61,
   45,   44,   44,   40,   59,  819,   44,  295, 1127,   42,
   59,   40,   44,  274,    0,   41,  237,    0, 1359,   59,
   59,   58,   45,  264,   44,    0,    0,   58,   40, 1264,
   41,   41,  385,   44,   44,  288,  503,   45,  293,  287,
    0,  510,  511,  512,  513,  241,   45,   44,   59,   59,
 1050,   63,  533,  288,  250,    0,   91, 1359,   41,  270,
  533,   44,  250,   40,  235,  286,    0,   41,  490,  701,
   44,  945,   44,   59,  284,    0,   59,    0,  274, 1131,
   59, 1359,  262,  264,   59,   59,  734,  308,  302,  339,
   45,  273,  559,  302,  315,  321,  317,   40,  319,   59,
  357,  322,   45,  887,  301,  393,  536, 1359,    0,    0,
  370,  428,   45, 1214,   59,  432,  433,  434, 1359,  331,
  284,  333,  783,  277, 1120,   59, 1122,  260,  263,    0,
   63, 1320,  861,  862,   59,  347,   59,  268,  340,  257,
  351,  262, 1242,  456,  564, 1231, 1242,  341, 1242,   41,
 1242,  879,  966,  349,  198,  585,  353,    0, 1242,  341,
 1349,  406,  260,  891,  892,    0,  321,   59,   59,    0,
   41,  304,  393,  430,    0,  385, 1248,  257,  313,    0,
  425,  297,  384,  265,  280,  381, 1421,    0,   59,  324,
  286,    0,  346,  443,  923,  355,  392,  399, 1012,  370,
  396,   41,  264,  265,    0,  401,  467,  387,  461, 1261,
   41,  437,  803,  401,  462,  463,   59,  335,  280, 1529,
  461,  303,  317,  318,   59,  285,  461,  260,   59,  443,
  440,  412,    0,   59,  443,  366,  319,  319,   59,    0,
  269,  303,  277,  305,  264,  289,   59,  457, 1589,  319,
   59,  319,  273,   61,  332,  335,  436,  319,  643,  341,
  643,  664,    0,   59,  993,    0,  387,  278,  278,  280,
  280,  304, 1128,  341,    0,    0,  440,  288,  288,  341,
    0, 1382,  667,  321,  310,  732,  551, 1589, 1374,  321,
  264,   59, 1378,  428, 1380,  278,  508,  280,   59,  652,
 1174,  341,  341,   41,   44,  288,   41,  624,  384,   44,
  388, 1589,  533,  258, 1624,  745,  388,  702,  748,  397,
  418,   59,  411,  722,   59,  397,  395, 1323,  384, 1415,
  411, 1414,  762,   59,   59,  412, 1455, 1589, 1193,   59,
  725,  264,  554,  329,  459,  461,  332,  321, 1589,  437,
  667,  459, 1077,  670,  329, 1059, 1428,  332,  675,  789,
 1165,    0,  307,  680,  681, 1043, 1676, 1172,  379,  379,
 1254,  461,  689,  384,  691,  692,  693,  694,  695,  744,
  697, 1237,  437,  643,  552,  702,    0,  608, 1657,  455,
  401,  401,  830,  605,  606,  817,  379,  408,  408,  884,
 1486,  461,    0, 1713,  564, 1491, 1054,  461,  725, 1217,
 1138,  397,  455,  461,  459,  455,  455,  459,  401,  839,
   59,  503,  397,  644,  461,  408,  502,  319,  319, 1698,
  461,  442,  442, 1433,  722,  503,  459,  329,  461, 1253,
  332,  503,  707,   41,  732,   59,   44,  459, 1534,  341,
  341,  459,    0,  502, 1643,  441,  453,  617,  617,  442,
  459,   59,  664,  397,  669, 1551,  441,  453,    0,  455,
 1199,  448,  643,    0,  419,  740,  741,  432,  486,  453,
  455,  441,  461,  455, 1358,  410,  530,  461,  319,  432,
 1219,  502,  502,  301, 1417,  455,  388,  388,  341,  432,
  262,  722,  887,  264,  459,  397,  727,  441, 1281,   41,
  341,   59,  981,  734, 1369,  868,  459,  388,  871,  502,
  716,  455,  718,  349,  736,  737,  459,   59,  341,  846,
  847,  848,   59,  754,  851,  852,  853, 1321,  855,  856,
  857,  858,  859,  860,  264, 1631,   12,  368,  388,  441,
  441,  319, 1652,  349, 1640,  599, 1652,  388, 1652,  368,
 1652,  782,  783,  455,  332,  305,  397,   41, 1652,  999,
  887,   41,  406,  341,   44,  743, 1045,  894,  895,  320,
  897, 1011,  899,  260,  319,  902,  903,  904,  905,  906,
  907,  356,   41,  319, 1680,    0,  884,  332,    0,  278,
  139, 1031,  919,   12,  921,  403,  341,  332,  329,  994,
  441,  994,    0,  781,  326,  341, 1001,  258, 1001, 1057,
  388,  302,  818,  457,  455,  387, 1582,  265,  788,  397,
 1586, 1360,  828, 1659,    0,   40,   41,  398,  461,   44,
  800,  320,   44,    0, 1044,  813,  814,  815,  260, 1240,
  388,  117,    0,  388,   59, 1246,   61,   59,  339,  397,
  401,  461,  397,  884, 1469,  303,  307,  869, 1327,  872,
   41,   59,  397,  441,  436,    0,  870,    0,  398,  839,
  278,  873,  280, 1639,    0,   41,   91,   44,    0,   40,
  288,    0,  304,   59,   45,    0,   41,  661,  308,  309,
  338,  442,   59, 1141,  343,  349,  441,    0,  117, 1632,
  258,   59,   63, 1739,   41,   41,  441,  418,   44,  431,
  455,    0,  401,  444,   59,    0,  438, 1588,  493,  343,
  455,   40,   41,   59,   59,   44,   59,    0, 1764,  378,
  461,  418,    0,   59,  445, 1634, 1131,   59,   41,    0,
   59,   44,   61,  769,   59, 1381,    0,  296,  378,  307,
 1077,    0,  443,  442, 1029,   41,   59,  406,   44,   44,
  409, 1727, 1089, 1090,   42, 1092, 1093, 1403,  419,   47,
   59,  379,   91, 1100,   59, 1185,  425,  319,  262,  461,
   41, 1258,  406,   44, 1683,  409,   59,   16,  403, 1229,
  332,   59,   41,  401,  274,   44,  308,  309,   59,  341,
  408,  425,  451,  262, 1131,   59,  343,   93,  462,  463,
   59, 1644,  410, 1646, 1456,   41, 1047,    0, 1145,  449,
 1260,    0, 1493, 1054,  439, 1724, 1153,  451, 1155, 1471,
  310, 1158,  452, 1160,  442,  260, 1327, 1164, 1165, 1166,
 1167,  378, 1657,  258, 1327, 1172,  388, 1242, 1587, 1242,
  444,   80, 1685, 1023, 1023,  397, 1126, 1127, 1128,  274,
 1038,  419,  277,  278,    0,  280, 1261,  461, 1607,  406,
  285, 1610,  409,  288,    0, 1173,   59,    0, 1056,  304,
   59,  262,   45, 1698, 1062, 1212,  301, 1366,  425,    0,
    0,  344,  307,    0,  502,  310,  262,   44,  461,  441,
   63,  259, 1359,  387,    0,  879,  264,  262,  259,    0,
  404, 1553,  270,  455,  451,   41,  369,  891,   44,  270,
 1195,  319, 1197,   59, 1757,  262, 1321,  421,  387,  264,
    0,   41,  412,   59, 1261,  296,   59,  378,  264,  258,
  452,  461,  264,  341, 1384,  278,   93,  280,   59,   59,
   41, 1278,   59,   44,    0,  274, 1126, 1127,  277,  278,
 1287,  280,  329,   59,  379,  332,  285, 1294,   59,  288,
   61, 1193,  308,  309,  427,  264, 1154,  344,  419, 1005,
 1006, 1256,  301, 1423,  459, 1395,  401,  467,  307,   59,
  388,  310,  353,  408, 1321,   41,  264,  412,   44,  397,
   91,  265,  280, 1413,  419,  278,  387,  280,  286,  388,
  264,  486,  403,   59, 1341,   44,  319,   41, 1345,  322,
   44,  387,  437,  260,   44,  466,  329,  442, 1054,  332,
  397, 1155,  387,  336, 1158, 1774, 1728, 1729,  341,  303,
  398,  344, 1636,  441,  329, 1687,  379,  332,    0, 1271,
  387, 1614,  467, 1275,    0,  446,  471,  360,  319,  344,
  379, 1359,  441,  398,   93, 1235, 1235,  304,  329,    0,
  319,  332,  398,   93,  441,  411,  398,  461,    0, 1377,
  341,  384,  401,  344,  461,  388,  453, 1681,  455,  408,
    0, 1783,  341,  412,  397,   41, 1659,  458,  459,  460,
  419,  462,  463,  329,  406,   44,  332,   59, 1548,  398,
   41,  414,  397,   59, 1441, 1442,  452,  508,  437,   41,
  481,  482,   44,  442,  261,  486,  487,  388,   59, 1723,
  398,   41, 1589,  296,   44,  398,  397,   59,  441, 1466,
  302,  264, 1469, 1706,  398, 1376, 1377, 1369,  467,   59,
  453,   61,  471,  264,   93,  457,  441, 1614,  461, 1376,
  343, 1755,  388,  302,  343,  435,    0,  258,  453,  349,
  455,  397,  343, 1500,  437,  459, 1739,  339,  358, 1385,
  441,   91,  278,  274,  280, 1455,  277,  278,    0,  280,
  353,  461,  453,  319,  285,  378,  461,  288, 1370,  378,
  339, 1764, 1659,  329,    0,  279,  332,  378,  471,  319,
  301,    0,  459, 1488, 1512,  341,  307, 1341,  344,  310,
  260, 1345,  322,  406,    0,   59,  409,  406, 1209, 1556,
  409,  341,  278,  461,  280,  406,  336, 1564,  409,    0,
  258,  272,  425,  261, 1416,   41,  425,   59,   44, 1706,
   42,   43,    0,   45,  425,   47,  330, 1652,  289, 1652,
  360,    0,  388,   59,  304,   61, 1494, 1532,  451,  408,
   59,  397,  451, 1600, 1255,  398,  344, 1604,  323,  418,
  451, 1512, 1739,   59,  384, 1455, 1512,  398,  379,  307,
  364, 1589, 1462,    0,  403,  458,  459,  460,   59,  462,
  463,  369,   41,  334,  443,   44,    0, 1764,  382,  461,
  401,   59, 1533,    0,  414,  441, 1614,  408,  481,  482,
   59,  412, 1528,  486,  487,   44,  435,  453,  419,  461,
 1657,  279,  363,  379,   41, 1557, 1497,   44,  259, 1561,
   59,  450, 1466,  353,    0,  258,  437,   41,  258,  270,
   44,  442,   59,  301,   61,  401,  278,  461,  280,  427,
 1625, 1659,  408,    0,  274,   59, 1552,   61,  278,  379,
  280, 1698,   59,  319,    0,  285,  467,  418,  288,    0,
  471, 1362,  330,  329,   91,   41,  332,  323,   44, 1601,
    0,  301,  379,  461,  307,  341,  442,  307,  402,  430,
  310,  349,    0,   59,  445,   61,   42,   43, 1706,   45,
  358,   47,    0, 1711,  401,   41,  364,    0,    0,    0,
   41,  408,   59,   44,  698,  384,   41,   42,   43,  384,
   45,    0,   47,   59,  382,   91,  322,    0,   59, 1667,
   61, 1739,  388,   41,    0, 1743, 1744,  721, 1634,   59,
  336,  397,  459,   41,  461,  442,   44,  379,  344,  733,
   41,   59,  258,   44,   41, 1651, 1764,   44,   40,  379,
 1768,   59,   41,   61,  360,   44,   59,   59,   59,  401,
 1711,  277,  278, 1781,  280, 1711,  408,  322,  280,  285,
   59,  401,  288,   44,  286,  441,   59, 1683,  408, 1669,
 1669,  336,  412,   59, 1690,  301,  419,  453,  288,  419,
  386,  307, 1743, 1744, 1735,  384,    0, 1743, 1744,  384,
  442,  301, 1688, 1689,  437,  360,    0,  437,  414,  321,
  319,    0,  442,  466,    0,  468,    0, 1768, 1724,  278,
  329,  280, 1768,  332,  494,   41, 1732,    0,   44,  288,
 1781,  258,  341,  437,  504, 1781,  260,  467,  471, 1725,
 1541,  471,  301, 1120,  258, 1122, 1547,  274,    0,   40,
   44,  278,   43,  280,   45,   59,  217,  495,  285,  414,
  274,  288,  338,  379,  225,   59,  419,   58,    0,  260,
   59,  461,   63,   59,  301,   59,  274,   93,  302,  388,
  307,  319,  258,  310,  355,  401,   59,  301,  397, 1775,
  314,   41,  408,  307,   44,   40,  310,   41,  274,  425,
   44,  417,  278,  419,  280,   42,   43,   59,   45,  285,
   47,  302,  288,   42,   43,  339,   45,  258,   47,    0,
  379,  437,  462,  314,  280,  301,  442,   59,  264,  322,
  286,  307,  441,  317,  310,   40,  277,  278,   43,  280,
   45,    0,  401,  336,  285,  280,  319,  288,  339,  408,
  258,  286,  379,   58,    0,  471,   42,   43,   63,   45,
  301,   47,  461,    0,  295,  321,  307,  360,   41,  338,
  278,   44,  280,   59,  401,  306,    0,  285,   59,  337,
  288,  408,  313,  442,  408,  412,  332,  357,  459,    0,
  461,  384,  419,  301,  418,  453,  338,  403,  412,  307,
   59,  472,  841,  379,  843,  419,    0,  338,  341,   41,
  437,  329,   44,   59,  332,  442,  321,  408,   40,  443,
  491,  414,   59,  437,  394,  401,  396,  418,  499,  500,
  501,   41,  408,  375,   44,   59,  412,  461,  379,  319,
  467,  403,  388,  419,  471,  384,  388,   41,   59,  419,
   44,  397,  443,  467,  258,   41,   41,  471,   44,   44,
  401,  437,  494,  495,  384,   59,  442,  408,    0,  437,
  388,  379,  403,  338,  444,  279,  267,  507,  419,  397,
   41,   42,   43,  362,   45,  276,   47,  451,   41,   42,
   43,  467,   45,  401,   47,  471,  437,  301,  435,  290,
  408,  442,  293,  307,  435,  296,  297,  461,  439,   41,
  375,  419,   44,   41,   45,  443,   44,  358,  458,  459,
  460,  288,   41,  388,  315,   44,  330,   59,  322,  437,
  471,  392,  393,  324,  442,  329,  327,  418,  332,  412,
    0,   41,  336,  280,   44,  349,  486,  487,  470,  286,
  344,  280,   41,  357,  358,   44,   41,  286,   41,   44,
  364,   44,  267,  471,  355,  356,  360,   41,    0,   41,
   44,  276,   44,  322,   44,  312,  367,   61,  382,   40,
  371,   42,   43,  312,   45,  290,  373,  336,  379,  287,
  384,  296,  297,   40,  280,  344,  264,   58,   40,   59,
  286,   44,   63,  397,  462,    0,  462,  463,  462,   41,
  315,  360,   44,  462,  463,  419,  462,  437,  461,  324,
  414,  321,  327,   44,  403,  419,  430,   59,   41,   42,
   43,  422,   45,  666,   47,  668,   40,  428,  671,  672,
  673,   40,   40,  434,   40,   91,   41,  441,   40,   44,
  355,  356,   40,  444,   40,   40,   40,   42,   43,  453,
   45,  455,  367,   40,   59,  414,  371,  458,  459,  460,
  461,  462,  463,   58,  379,   40,   40,   40,   63,  461,
   46,  472,   42,   43,    0,   45,   40,   47,  321,   44,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   44,
  491,  429,   46,   61,   40,  496,  497,  264,  499,  500,
  501,  280,   44,  418,  505,   46,  383,  422,  349,  280,
   44,  301,  278,  428,  278,  286,  258,  280,  322,  434,
  264,  461,  398,  286,  437,  329,  471,  398,  332,  444,
  459,  277,  336,   59,  448,  400,  448,  279,   40,  350,
  344,   40,   91,  458,  459,  460,  461,  462,  463,   40,
   40,  319,  264,  384,  412,  296,  360,  472,   44,  301,
  390,  383,   40,   40,  437,  307,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  465,  491,   40,  318,   41,
  384,  496,  497,  453,  499,  500,  501,  321,  330,  260,
  505,  321,  452,  397,  461,  461,  267,   41,   42,   43,
  449,   45,  264,   47,  455,  276,  459,  349,   40,  279,
  414,  277,  353,  274,   40,  301,  358,  285,  345,  290,
  345,  419,  364,  278,   40,  296,  297,   43,  387,   45,
  387,  301,  387,  304,  417,  419,  278,  441,  280,   40,
  382,  301,   58,  285,  357,  418,  288,   63,   40,  453,
  274,  455,  461,  324,   40,   44,  327,  280,  418,  301,
  330,  368,  851,  286,  853,  260,  855,  856,  386,  858,
  859,  860,  267,  278,   44,  280,  321,  419,  264,  349,
   41,  276,  353,  288,  355,  356,   44,  321,  358,   44,
  280,   40,  262,   44,  364,  290,  367,  329,  503,   44,
  371,  296,  297,  444,  506,  266,   40,  300,   40,  304,
  280,   43,  382,   45,  502,   40,  286,  458,  459,  460,
  264,  462,  463,  423,  461,   41,   58,   58,   41,  324,
   41,   63,  327,   42,   43,   44,   45,  379,   47,   41,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  264,
  455,  441,  395,  279,  310,  496,  497,  428,  353,  401,
  355,  356,  493,  434,   41,  310,  408,   41,   40,   40,
  353,   40,  367,  444,  379,  301,  371,   41,   42,   43,
   40,   45,   40,   47,   40,   40,   40,  458,  459,  460,
  461,  462,  463,   41,   42,   43,  401,   45,   40,   47,
  442,  472,  329,  408,  330,   40,   40,   40,   40,   40,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   40,
  491,   40,  262,  349,  328,  496,  497,   44,  499,  500,
  501,   41,  358,  428,  505,  436,  280,  442,  364,  434,
  309,  389,  286,  344,  274,  332,  389,  447,  271,  444,
  389,  267,  423,  391,  288,  392,  382,    0,  408,  387,
  276,   41,  387,  458,  459,  460,  461,  462,  463,   41,
   42,   43,  379,   45,  290,   47,  403,  472,  387,  387,
  296,  297,  387,   41,  387,  278,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   40,  491,    0,  418,  315,
   41,  496,  497,   41,  499,  500,  501,   41,  324,   41,
  505,  327,  439,  386,  437,  309,   59,  444,  436,  338,
  302,  397,  443,  450,    0,  339,  443,  339,  260,  302,
   41,  263,   41,  353,  461,  267,  321,  386,   41,  355,
  356,   44,  469,   40,  276,  372,   43,  372,   45,   41,
   41,  367,   41,   41,   41,  371,   59,   41,  290,   41,
   41,   58,   41,  379,  296,  297,   63,   41,   41,   42,
   43,  280,   45,   41,   47,   41,   41,  286,   41,   41,
   41,  280,   59,   59,   42,   43,   40,   45,   44,   47,
    0,  341,  324,   40,  341,  327,  353,   41,   42,   43,
   59,   45,   59,   47,   59,   41,  422,   41,   42,   43,
   59,   45,  428,   47,   59,  309,  280,   59,  434,  345,
   41,  369,  286,  355,  356,   40,  341,   44,  444,   41,
  452,  384,  280,   59,  344,  367,   41,  461,  286,  371,
  384,  384,  458,  459,  460,  461,  462,  463,   41,   59,
   44,  411,   44,   41,   42,   43,  472,   45,  436,   47,
  305,  302,  443,   40,  388,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   59,  491,  294,  305,   44,  308,
  496,  497,   44,  499,  500,  501,   41,  464,    0,  505,
  264,   41,   42,   43,  426,   45,  428,   47,  338,  384,
  384,   40,  434,  278,  264,  461,  321,  459,  280,  390,
  264,  412,  444,  452,  286,   42,   43,  471,   45,   40,
   47,  310,  459,  271,  264,  437,  458,  459,  460,  461,
  462,  463,   59,  436,  461,  344,  279,   41,   42,   43,
  472,   45,  344,   47,  344,  344,  271,  309,   41,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  301,  491,
   40,  459,  309,  260,  496,  497,  263,  499,  500,  501,
  267,   40,  309,  505,   59,   41,    0,   59,   40,  276,
   41,   43,  258,   45,   41,   42,   43,  330,   45,  305,
   47,   44,   59,  290,   41,    0,   58,    0,    0,  296,
  297,   63, 1123,  279,  262,   12,  349,  280,  245,  252,
  652,  244,  612,  286,  357,  358,  319,   41,  831,  322,
   44,  364,  280, 1059, 1057,  301,  329,  324,  286,  332,
  327,  307,   84,  336, 1046,   59,  280, 1054,  341,  382,
  335,  344,  286, 1404, 1036,  305,  280,  300,  258,    0,
  291,  394,  286,  396,  330,  727,  984,  360,  355,  356,
   40, 1047,  559,   43, 1598,   45,  317,  317,  764,  279,
  367, 1674, 1382,  349,  371, 1384,  419,  538,   58, 1715,
 1678,  384,  358,   63,  387,  388, 1677,  754,  364, 1619,
   41,  301,  280,   44,  397, 1767,  968,  307,  286, 1210,
 1712,  444, 1187,   41,   42,   43,  382,   45,   59,   47,
 1634,  414,   41,   42,   43, 1363,   45,  540,   47,  322,
  330,   42,   43,  310,   45,  303,   47,  810, 1244,  426,
  280,  428,  370,  436,  802,  598,  286,  434,  441,  349,
  404,   42,   43,  419,   45,   13,   47,  444,  358,  278,
  453,  472,  455,  280,  364,  971,  476,  716,   59,  286,
  732,  458,  459,  460,  461,  462,  463,  122,  703,  702,
 1589, 1732,  382,  260, 1690,  472,  280, 1784,  945, 1484,
  488,  660,  286, 1091,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  888,  491,  369,    0,   -1,  260,  496,
  497,  263,  499,  500,  501,  267, 1160,   -1,  505,  419,
  306,   -1,   -1,   40,  276,  302,   43,  313,   45,   -1,
   -1,  317,  318,  280,   -1,   -1,   -1,  314,  290,  286,
  326,   58,   -1,   -1,  296,  297,   63,   41,   -1,   -1,
   44,  328,  338,  473,  474,  475,  476,  477,  478,  479,
  480,   -1,  339,   -1,  278,   59,  280,   -1,   -1,   -1,
   -1,  285,  324,   -1,  288,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  301,   -1,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  355,  356,   40,  276,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,  367,   -1,  403,    0,  371,
  290,   -1,   -1,   58,   -1,   -1,  296,  297,   63,   -1,
   -1,  408,   -1,   -1,  420,   -1,   -1,  278,   -1,  280,
   -1,  418,   -1,   -1,   -1,   -1,   -1,  288,   -1,  435,
   -1,   -1,  280,  439,  324,   -1,   -1,  327,  286,   41,
  301,  280,   44,   -1,  450,  379,  443,  286,   -1,  280,
   -1,   -1,   -1,   -1,  426,  286,  428,   59,   -1,   -1,
   -1,   -1,  434,  469,  461,  355,  356,  401,   -1,  280,
   -1,   -1,  444,   -1,  408,  286,   -1,  367,   -1,   -1,
   -1,  371,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  379,  491,
   -1,   -1,   -1,  260,  496,  497,  263,  499,  500,  501,
  267,   -1,   -1,  505,   -1,   -1,  426,   -1,  428,  276,
  401,   -1,   -1,   -1,  434,   -1,   -1,  408,   -1,   -1,
   -1,   -1,   -1,  290,  444,   -1,   -1,   -1,   -1,  296,
  297,   -1,   -1,   -1,  278,   -1,  280,   -1,  458,  459,
  460,  461,  462,  463,  288,   -1,   -1,   -1,   -1,   -1,
   -1,  442,  472,   -1,   -1,   -1,   -1,  324,   -1,   -1,
  327,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,  499,
  500,  501,  267,   -1,   -1,  505,   -1,   -1,  355,  356,
   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  367,   -1,   -1,    0,  371,  290,   -1,    0,   58,   -1,
   -1,  296,  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  379,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  401,   -1,  426,
   -1,  428,   59,   -1,  408,   -1,   59,  434,   -1,   -1,
  355,  356,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,
   -1,   -1,  367,   -1,   -1,   -1,  371,  319,   -1,   -1,
  322,  458,  459,  460,  461,  462,  463,  329,  442,   -1,
  332,   -1,   -1,   -1,  336,  472,   -1,   -1,   -1,  341,
   -1,   -1,  344,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,   -1,  491,   -1,  295,   -1,  360,  496,
  497,   -1,  499,  500,  501,   -1,   -1,  306,  505,   -1,
   -1,  426,   -1,  428,  313,   -1,   -1,   -1,   -1,  434,
   -1,   -1,  384,   -1,   -1,   -1,  388,  326,   -1,  444,
   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,  414,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,  441,
  260,  496,  497,  263,  499,  500,  501,  267,  387,   -1,
  505,  453,   -1,   -1,   -1,   40,  276,   -1,   43,   -1,
   45,   -1,   -1,   -1,  403, 1279, 1280,   -1,   -1,    0,
  290, 1285, 1286,   58, 1288, 1289,  296,  297,   63,   -1,
   -1,  420, 1296, 1297, 1298, 1299,   -1, 1301, 1302, 1303,
 1304, 1305, 1306,   -1,   -1,  258,  435,   -1,   -1,   -1,
  439,   -1,   -1,  442,  324,   -1,   -1,  327,   -1,   -1,
   41,  450,   -1,   44,   -1,   -1,  279,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   59,   -1,
  469,   -1,   -1,   -1,   -1,  355,  356,   58,  301,   -1,
   -1,   -1,   63,   -1,  307,   -1,   -1,  367,   -1,   -1,
   -1,  371,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  330,   -1,  336,
   -1,   -1,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  349,   -1,   -1,  271,
   -1,   -1,   -1,  360,   -1,  358,   -1,   -1,   -1,   -1,
   -1,  364,   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,  384,   -1,  382,
  302,  388,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,
  397,  313,  314,   -1,  316,   -1,   -1,  319,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,  414,   -1,   -1,
   -1,  333,  472,   -1,   -1,   -1,  419,  339,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,  441,  260,  496,  497,  263,  499,
  500,  501,  267,   -1,   -1,  505,  453,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,  290,   -1,   -1,   -1,   -1,
   -1,  296,  297,   -1,   -1,   -1,   58,  399,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  418,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,  443,   40,   -1,   42,   43,   -1,   45,   -1,  290,
  355,  356,  454,   -1,   -1,  296,  297,   -1,   -1,  461,
   58,   -1,  367,   -1,   -1,   63,  371,   -1,  319,   -1,
   -1,  322,   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,
   -1,  332,   -1,  324,   -1,  336,  327,   -1,  490,   -1,
  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,
   -1,   -1,  353,   -1,  355,  356,   -1,   -1,   -1,   -1,
   -1,  426,   -1,  428,   -1,   -1,  367,   -1,   -1,  434,
  371,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,  444,
   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,  414,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  436,  491,  428,   -1,   -1,
  441,  496,  497,  434,  499,  500,  501,   -1,   -1,   -1,
  505,   -1,  453,  444,  455,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,  290,   -1,
   -1,  472,   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,  324,   -1,  505,  327,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,  276,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  353,  290,  355,  356,   58,   -1,   -1,  296,  297,
   63,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,  271,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  353,  418,  355,  356,   58,
   -1,   -1,   -1,   -1,   63,   -1,  428,   -1,   -1,  367,
   -1,   -1,  434,  371,   -1,   -1,   -1,   -1,  302,   -1,
   -1,   -1,  444,   -1,   -1,  309,   -1,   -1,   -1,  313,
  314,   -1,  316,   -1,   -1,  319,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  333,
  472,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
  428,   -1,   -1,   -1,  496,  497,  434,  499,  500,  501,
   -1,   -1,   -1,  505,   -1,   -1,  444,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,  399,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,  418,   -1,   -1,  260,  496,  497,
   -1,  499,  500,  501,  267,   -1,   -1,  505,   -1,  433,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,  443,
   40,   -1,   -1,   43,   -1,   45,   -1,  290,  452,   -1,
  454,   -1,   -1,  296,  297,   -1,   -1,  461,   58,   -1,
   -1,  304,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,   -1,  327,   -1,  490,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  290,  355,  356,   -1,   -1,   -1,  296,  297,   -1,
   -1,   -1,   58,   -1,  367,   -1,   -1,   63,  371,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  461,
   -1,   -1,   -1,  261,   -1,  324,   -1,   -1,  327,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,  485,   -1,  283,  488,  489,  490,  491,
   -1,  493,   -1,  291,  496,  497,  355,  356,   -1,   -1,
  298,   -1,  300,   -1,  302,  428,   -1,   -1,  367,  307,
   -1,  434,  371,   -1,   -1,   -1,  314,   -1,   -1,   -1,
   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,  339,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,  428,
   -1,   -1,   -1,  496,  497,  434,  499,  500,  501,   -1,
   -1,   -1,  505,   -1,   -1,  444,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  452,   -1,   -1,  276,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,  406,   -1,
  290,   -1,   -1,  472,   -1,  413,  296,  297,  416,   -1,
  418,  419,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,   -1,  324,  443,  505,  327,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   40,   41,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  290,  355,  356,   -1,   -1,   58,
  296,  297,   -1,   -1,   63,   -1,   -1,  367,   -1,   -1,
   -1,  371,   -1,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,  324,  507,
   -1,  327,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   60,   61,   62,   -1,   -1,   -1,  353,  418,  355,
  356,   -1,   41,   -1,   40,   44,   -1,   43,  428,   45,
   -1,  367,   -1,   -1,  434,  371,   -1,   -1,   -1,   -1,
   59,   -1,   58,   -1,  444,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,  428,   -1,   -1,   -1,  496,  497,  434,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,  444,   -1,
   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
  292,   -1,   -1,   -1,  296,   -1,  472,  299,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,  267,  505,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,  340,   -1,
   -1,   -1,   -1,   -1,  346,  347,  348,   -1,  350,  351,
  352,  290,  354,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,  374,   -1,  376,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  272,   -1,  324,   -1,   63,  327,   -1,
   -1,  280,   -1,   -1,   -1,   -1,   -1,  286,   -1,   -1,
  289,   -1,   -1,  262,   -1,  407,   -1,   -1,   -1,   -1,
  412,  267,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,
  276,   -1,  424,   -1,   -1,   -1,   -1,   -1,  367,   -1,
   -1,   -1,  371,   -1,  290,   -1,  325,   -1,   -1,   -1,
  296,  297,   -1,   -1,  446,  334,   40,   -1,   -1,   43,
   -1,   45,   -1,  342,   -1,   -1,   -1,   -1,   -1,   -1,
  319,   -1,   -1,  322,   58,   -1,   -1,   -1,  324,   63,
  329,  327,  361,  332,  363,   -1,   -1,  336,   -1,  481,
  482,   -1,  341,   -1,   -1,  344,   -1,   -1,  377,  428,
  379,  380,  381,   -1,   -1,  434,  498,   -1,   -1,  355,
  356,  360,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,
   -1,  367,   -1,   -1,   -1,  371,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,  384,   -1,   -1,  387,  388,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,  397,   -1,
   -1,  430,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,  414,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,
   -1,   -1,  428,   -1,   -1,   -1,   -1,  436,  434,   -1,
   -1,   -1,  441,   -1,   -1,   -1,   -1,   -1,  444,   -1,
   -1,  267,   -1,   -1,  453,   -1,  455,   -1,   -1,  455,
  276,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,  290,   -1,  472,    0,   -1,   -1,
  296,  297,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,  273,   -1,  324,  505,
  277,  327,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,  292,   -1,   -1,   -1,  296,
   -1,   -1,  299,  267,   -1,   -1,   59,   -1,   -1,  355,
  356,   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  367,   -1,   -1,   -1,  371,  290,   -1,   -1,   58,
   59,   -1,  296,  297,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  340,   -1,   -1,   -1,   -1,   -1,  346,
  347,  348,   -1,  350,  351,  352,   -1,  354,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  418,   -1,   -1,   42,   43,  374,   45,  376,
   47,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,  355,  356,   60,   61,   62,   -1,   -1,  444,   -1,
   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,   -1,
  407,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,  424,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,  446,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,   -1,  481,  482,   -1,   -1,   -1,   -1,
   -1,  455,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,  498,   -1,   -1,   -1,   -1,   -1,   -1,  472,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,  267,   40,
   -1,  505,   43,   -1,   45,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  290,   63,   -1,   -1,   -1,  319,  296,  297,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,  360,   -1,   -1,
   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,  289,   -1,   -1,   -1,  355,  356,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   40,   -1,  367,   43,
   -1,   45,  371,   -1,  397,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  325,   63,
   -1,  414,   -1,   -1,   -1,   -1,   -1,  334,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  342,   -1,  271,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,   -1,   -1,  361,   -1,  363,   -1,   -1,  428,
  453,   -1,  455,   -1,   -1,  434,   -1,   -1,  302,   -1,
  377,   -1,  379,  380,  381,  444,   -1,   -1,   40,  313,
  314,   43,  316,   45,   -1,  319,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   58,   -1,   -1,  333,
   -1,   63,   -1,  472,   -1,  339,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,  430,   -1,   -1,  267,  496,  497,   -1,
  499,  500,  501,   -1,   -1,  276,  505,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,
   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  273,  399,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,
   -1,   -1,   -1,  324,  418,   -1,  327,  296,   -1,   -1,
  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  353,   -1,  355,  356,   -1,   -1,  452,   -1,
  454,   -1,   -1,  267,   -1,   -1,  367,  461,   -1,   -1,
  371,  340,  276,   -1,   -1,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,   -1,  354,  290,   -1,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,  490,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  374,   -1,  376,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,  407,   -1,
   -1,   -1,   -1,  444,   -1,  267,   -1,   -1,   -1,   -1,
   -1,  355,  356,   -1,  276,  424,   -1,  458,  459,  460,
  461,  462,  463,  367,   -1,   -1,   -1,  371,  290,   -1,
   -1,  472,   -1,   -1,  296,  297,    0,  446,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,  461,   -1,   -1,  496,  497,   -1,  499,  500,
  501,   -1,  324,   -1,  505,  327,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,   -1,   -1,   40,   41,   42,   43,
   44,   45,   46,   47,  428,   -1,   -1,   -1,   -1,  498,
  434,   -1,   -1,  355,  356,   59,   60,   61,   62,   -1,
  444,   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   91,  472,   93,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,   -1,  491,
   -1,   -1,   -1,   -1,  496,  497,   -1,  499,  500,  501,
   -1,   -1,   -1,  505,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
  461,   -1,   -1,   -1,   -1,   59,   60,   61,   62,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,  485,   -1,   -1,  488,  489,  490,
  491,   -1,  493,   -1,   -1,  496,  497,   -1,  262,   93,
  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,
   -1,   -1,  286,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  308,  309,   -1,   -1,  312,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,  321,  322,   -1,
   -1,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,  452,  453,
   -1,  455,  286,   -1,   -1,  289,   -1,  461,  340,   -1,
   -1,   -1,   -1,   -1,  346,  347,  348,   -1,  350,  351,
  352,   -1,  354,    0,  308,  309,   -1,   -1,  312,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,  321,  322,   -1,
   -1,  325,  374,   -1,  376,  329,   -1,   -1,  332,   -1,
  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,   -1,
  344,   -1,   -1,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,  407,  360,  361,   -1,  363,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,  424,  377,   -1,  379,  380,  381,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  397,  446,  262,   93,  264,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,  411,   -1,  461,
  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  286,
   -1,   -1,  289,   -1,   -1,   -1,  430,   -1,   -1,  481,
  482,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,
    0,  308,  309,   -1,   -1,  312,  498,   -1,  452,  453,
   -1,  455,  319,   -1,  321,  322,   -1,  461,  325,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  334,   -1,  336,
   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,  273,   -1,  397,   -1,  277,
   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  411,   -1,   -1,  414,   -1,   -1,   -1,  296,   -1,
   -1,  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  430,   -1,  262,   -1,  264,   -1,  436,   -1,   -1,   -1,
   -1,  441,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,  453,   -1,  455,   -1,   -1,   -1,  289,
   -1,  461,  340,   -1,   -1,   -1,   -1,   -1,  346,  347,
  348,   -1,  350,  351,  352,   -1,  354,    0,  308,  309,
   -1,   -1,  312,   -1,   -1,   -1,   -1,   -1,   -1,  319,
   -1,  321,  322,   -1,   -1,  325,  374,   -1,  376,  329,
   -1,   -1,  332,   -1,  334,   -1,  336,   -1,   -1,   -1,
   -1,  341,  342,   -1,  344,   -1,   -1,   -1,   41,   -1,
   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,  407,
  360,  361,   -1,  363,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,  424,  377,   -1,  379,
  380,  381,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,  446,  262,
   93,  264,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,  411,   -1,   -1,  414,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
  430,   -1,   -1,  481,  482,   -1,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,    0,  308,  309,   -1,   -1,  312,
  498,   -1,  452,  453,   -1,  455,  319,   -1,  321,  322,
   -1,  461,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,  334,   -1,  336,   -1,   -1,   -1,   -1,  341,  342,
   -1,  344,   -1,   -1,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,  361,   -1,
  363,   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  377,   -1,  379,  380,  381,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   93,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  411,   -1,
   -1,  414,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  430,   -1,  262,
   -1,  264,   -1,  436,   -1,   -1,   -1,   -1,  441,  272,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  452,
  453,   -1,  455,   -1,   -1,   -1,  289,   -1,  461,   59,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  308,  309,   -1,   -1,  312,
   -1,   -1,   -1,   -1,   -1,   -1,  319,   -1,  321,  322,
   -1,   -1,  325,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,  334,   -1,  336,   41,   -1,   -1,   44,  341,  342,
   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,  360,  361,   -1,
  363,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  377,   -1,  379,  380,  381,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   -1,  262,   -1,  264,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,  411,   -1,
   -1,  414,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,  430,   -1,   -1,
   -1,   -1,   59,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,  308,  309,   -1,   -1,  312,   -1,   -1,  452,
  453,   -1,  455,  319,   -1,  321,  322,    0,  461,  325,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,  334,   -1,
  336,   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,  262,   -1,  360,  361,   -1,  363,   41,   -1,
   -1,   44,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  377,   -1,  379,  380,  381,   59,   -1,  384,   -1,
   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  397,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,  411,   44,   -1,  414,  319,
   59,   -1,  322,   -1,   -1,  262,   -1,   -1,   -1,  329,
   -1,   59,  332,   -1,  430,   -1,  336,   -1,   -1,   -1,
  436,  341,   -1,   -1,  344,  441,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,  453,   -1,  455,
  360,   -1,   -1,   -1,   -1,  461,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  319,   -1,  384,  322,   -1,  387,  388,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  397,   -1,  336,
   -1,   -1,   -1,   -1,  341,  262,   -1,  344,   -1,   -1,
   -1,   -1,   41,   -1,  414,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,  453,   -1,  455,   -1,   -1,   -1,   -1,
  397,    0,  319,   -1,   -1,  322,   -1,    0,   -1,   -1,
   -1,   -1,  329,   -1,   -1,  332,   -1,  414,   -1,  336,
   -1,   -1,   -1,   -1,  341,   -1,   -1,  344,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,
   -1,   -1,   41,  360,  441,   44,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,  453,   -1,  455,   -1,
   59,   -1,   -1,  262,   -1,   -1,   59,  384,   -1,   -1,
  387,  388,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
  397,   -1,   -1,   -1,    0,   -1,  319,   -1,   -1,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,  414,   -1,  332,
   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,
  319,   -1,   -1,  322,  441,   41,   -1,  360,   44,   -1,
  329,  319,   -1,  332,  322,   -1,  453,  336,  455,   -1,
   -1,  329,  341,   59,  332,  344,   -1,   -1,  336,   -1,
   -1,  384,   -1,  341,  387,  388,  344,   -1,   -1,   -1,
   -1,  360,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
    0,   -1,  360,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  414,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
    0,   -1,   -1,   -1,   -1,   -1,  384,   -1,  397,  387,
  388,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,  397,
   -1,   41,   -1,   -1,   44,  414,   -1,   -1,   -1,   -1,
  453,   -1,  455,   -1,   -1,   -1,  414,   -1,   -1,   59,
  319,   41,   -1,  322,   44,   -1,   -1,  436,   -1,   -1,
  329,   -1,  441,  332,   -1,   -1,   -1,  336,  436,   59,
   -1,   -1,  341,  441,  453,  344,  455,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,  453,   -1,  455,   -1,  262,
   -1,  360,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,    0,   -1,
  319,   -1,   -1,  322,   -1,  414,  319,   -1,   -1,  322,
  329,   -1,   59,  332,   -1,   -1,  329,  336,   -1,  332,
   -1,   -1,  341,  336,   -1,  344,  262,  436,  341,   -1,
   -1,  344,  441,   -1,   -1,   -1,   -1,   -1,   -1,   41,
   -1,  360,   44,   -1,  453,   -1,  455,  360,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,  384,   -1,   -1,  387,  388,   -1,   -1,  397,   -1,
   -1,   -1,   -1,  319,  397,    0,  322,   -1,   -1,   -1,
   -1,   -1,   -1,  329,   -1,  414,  332,   -1,   -1,   -1,
  336,  414,   -1,   -1,   -1,  341,    0,   -1,  344,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,  436,   -1,   -1,
   -1,   -1,  441,  436,  360,   -1,   41,   -1,  441,   44,
   -1,   -1,  262,   -1,  453,   -1,  455,   -1,   -1,   -1,
  453,   -1,  455,   -1,   59,   -1,   -1,   41,  384,   -1,
   44,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  397,   -1,   -1,   -1,   59,   -1,   -1,   -1,  319,
   -1,   -1,  322,   -1,   -1,   -1,   -1,   -1,  414,  329,
   -1,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,  319,
   -1,  341,  322,   -1,  344,   -1,   -1,   -1,   -1,  329,
  436,   -1,  332,   -1,   -1,  441,  336,   -1,   -1,   -1,
  360,  341,    0,   -1,  344,  262,   -1,  453,   -1,  455,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  360,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,
   -1,   -1,   -1,   41,  384,   -1,   44,  387,  388,   -1,
  262,   -1,   -1,   -1,  414,   -1,   -1,  397,   -1,   -1,
   -1,   59,  319,   -1,   -1,  322,   -1,   -1,   -1,   -1,
   -1,   -1,  329,   -1,  414,  332,  436,   -1,   -1,  336,
   -1,  441,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,
   -1,   -1,   -1,  453,   -1,  455,  436,   -1,   -1,   -1,
   -1,  441,   -1,  360,   -1,   -1,   -1,  319,   -1,   -1,
  322,   -1,   -1,  453,   -1,  455,   -1,  329,   -1,   -1,
  332,   -1,   -1,   -1,  336,   -1,   -1,  384,   -1,  341,
  387,  388,  344,   -1,   -1,   -1,   -1,  262,   -1,   -1,
  397,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  414,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,  436,
   -1,   -1,   -1,   -1,  441,  397,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  319,   -1,  453,  322,  455,   -1,
   -1,   -1,  414,   -1,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,   -1,   -1,   -1,  319,  341,   -1,  322,  344,
   -1,   -1,   -1,   -1,  436,  329,   -1,   -1,  332,  441,
   -1,   -1,  336,   -1,   -1,  360,   -1,  341,   -1,   -1,
  344,  453,   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,   -1,  384,
   -1,   -1,  387,  388,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,  414,
   -1,   -1,   -1,  397,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  414,  436,   -1,  292,   -1,   -1,  441,  296,   -1,   -1,
  299,  319,   -1,   -1,  322,   -1,   -1,   -1,  453,   -1,
  455,  329,  436,   -1,  332,   -1,   -1,  441,  336,   -1,
   -1,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,  453,
   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  340,  360,   -1,   -1,   -1,   -1,  346,  347,  348,
   -1,  350,  351,  352,   -1,  354,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,
  388,  461,   -1,   -1,   -1,  374,   -1,  376,   -1,  397,
  470,  471,  472,  473,  474,  475,  476,  477,  478,  479,
  480,  481,  482,  483,  484,  485,  414,   -1,  488,  489,
  490,  491,   -1,  493,   -1,   -1,  496,  497,  407,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,   -1,
   -1,   -1,   -1,  441,   -1,  424,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  453,   -1,  455,   -1,   -1,
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
#define YYMAXTOKEN 508
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
"proc_statement : KW_BREAK ';'",
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
{ yyval = make_node (nod_unique, (int) 0, NULL); }
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
case 307:
{ yyval = make_node (nod_breakleave, e_break_count, NULL); }
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
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 332:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 337:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 338:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 339:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 342:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 343:
{ yyval = make_list (yyvsp[0]); }
break;
case 344:
{ yyval = make_list (yyvsp[-1]); }
break;
case 345:
{ yyval = NULL; }
break;
case 349:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_replace_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_mod_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 357:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], NULL, NULL); }
break;
case 361:
{ yyval = make_list (yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 366:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 367:
{ yyval = yyvsp[-1]; }
break;
case 368:
{ beginning = lex_position(); }
break;
case 369:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
			       (lex_position() == end) ?
			       lex_position()-beginning : last_token-beginning);}
break;
case 370:
{ beginning = last_token; }
break;
case 371:
{ yyval = (DSQL_NOD) MAKE_string ((UCHAR *) beginning, 
					lex_position()-beginning); }
break;
case 372:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string (NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 373:
{ yyval = 0; }
break;
case 374:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 375:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 376:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 378:
{ yyval = NULL; }
break;
case 379:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 380:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 398:
{ yyval = NULL; }
break;
case 399:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 400:
{ yyval = yyvsp[0]; }
break;
case 401:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 402:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 403:
{ yyval = yyvsp[0]; }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 406:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 407:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string (NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 411:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 412:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 413:
{ yyval = yyvsp[0]; }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 416:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 420:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 421:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 422:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 423:
{ yyval = yyvsp[0]; }
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 426:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 427:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 428:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 453:
{ yyval = NULL; }
break;
case 454:
{ yyval = NULL; }
break;
case 455:
{ yyval = NULL; }
break;
case 456:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 457:
{ g_field_name = yyvsp[0];
			  g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) g_field; }
break;
case 458:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 459:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 460:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 461:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 462:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 463:
{ log_defined = FALSE;
			  cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 465:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 466:
{ yyval = yyvsp[0]; }
break;
case 467:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 468:
{ yyval = yyvsp[0]; }
break;
case 469:
{ yyval = yyvsp[0]; }
break;
case 471:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 472:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 474:
{ yyval = NULL; }
break;
case 476:
{ yyval = NULL; }
break;
case 477:
{ yyval = yyvsp[0]; }
break;
case 478:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 494:
{ g_field->fld_ranges = make_list (yyvsp[-1]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 495:
{ g_field->fld_ranges = make_list (yyvsp[-2]);
		      g_field->fld_dimensions = g_field->fld_ranges->nod_count / 2;
		      g_field->fld_element_dtype = g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 497:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 498:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 499:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 505:
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
case 506:
{ 
			g_field->fld_dtype = dtype_long; 
			g_field->fld_length = sizeof (SLONG); 
			}
break;
case 507:
{ 
			g_field->fld_dtype = dtype_short; 
			g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 508:
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
case 509:
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
case 510:
{ 
			g_field->fld_dtype = dtype_timestamp; 
			g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 513:
{ 
			g_field->fld_dtype = dtype_blob; 
			}
break;
case 514:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-1];
			g_field->fld_sub_type = 0;
			}
break;
case 515:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = (USHORT) yyvsp[-3];
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 516:
{ 
			g_field->fld_dtype = dtype_blob; 
			g_field->fld_seg_length = 80;
			g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 517:
{
			g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 518:
{
			g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 519:
{
			g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 520:
{
			g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 521:
{
			g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 522:
{
			g_field->fld_character_set = yyvsp[0];
			}
break;
case 524:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 525:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 526:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			g_field->fld_flags |= FLD_national;
			}
break;
case 527:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 528:
{ 
			g_field->fld_dtype = dtype_text; 
			g_field->fld_character_length = 1; 
			}
break;
case 529:
{ 
			g_field->fld_dtype = dtype_varying; 
			g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 538:
{ 
			  g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 539:
{  
			   g_field->fld_sub_type = dsc_num_type_decimal;
			   if (g_field->fld_dtype == dtype_short)
				{
				g_field->fld_dtype = dtype_long;
				g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 540:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 541:
{
			g_field->fld_dtype = dtype_long; 
		    	g_field->fld_length = sizeof (SLONG); 
			g_field->fld_precision = 9;
		    	}
break;
case 542:
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
case 543:
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
case 546:
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
case 547:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 548:
{ 
			g_field->fld_dtype = dtype_real; 
			g_field->fld_length = sizeof (float); 
			}
break;
case 549:
{ 
			g_field->fld_dtype = dtype_double; 
			g_field->fld_length = sizeof (double); 
			}
break;
case 550:
{ yyval = yyvsp[-1]; }
break;
case 551:
{ yyval = 0; }
break;
case 555:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 556:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 557:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 558:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 559:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 562:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 563:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 566:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 567:
{ yyval = (DSQL_NOD) NULL; }
break;
case 569:
{ yyval = (DSQL_NOD) NULL; }
break;
case 570:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 572:
{ yyval = (DSQL_NOD) NULL; }
break;
case 574:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 579:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 580:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 581:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 582:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 583:
{ yyval = yyvsp[0];}
break;
case 585:
{ yyval = yyvsp[0];}
break;
case 586:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 587:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 593:
{ yyval = 0; }
break;
case 594:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 595:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 596:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 597:
{ yyval = (DSQL_NOD) 0; }
break;
case 598:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 599:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 601:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 602:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 603:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 604:
{ yyval = 0; }
break;
case 606:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 607:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 608:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 610:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 611:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 612:
{ yyval = make_list (yyvsp[0]); }
break;
case 613:
{ yyval = 0; }
break;
case 615:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 616:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-3], yyvsp[-1], yyvsp[-2], yyvsp[0]); }
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
{ yyval = 0; }
break;
case 624:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 625:
{ yyval = 0; }
break;
case 626:
{ yyval = yyvsp[0]; }
break;
case 627:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 628:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 629:
{ yyval = 0; }
break;
case 630:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 631:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 632:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], yyvsp[-1]); }
break;
case 633:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[0]); }
break;
case 634:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 635:
{ yyval = 0; }
break;
case 636:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 637:
{ yyval = yyvsp[-1]; }
break;
case 638:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 643:
{ yyval = 0; }
break;
case 644:
{ yyval = make_list (yyvsp[0]); }
break;
case 645:
{ yyval = 0; }
break;
case 647:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 649:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 650:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 651:
{ yyval = make_list (yyvsp[0]); }
break;
case 653:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 656:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 657:
{ yyval = yyvsp[-1]; }
break;
case 658:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 659:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 660:
{ yyval = make_list (yyvsp[-1]); }
break;
case 661:
{ yyval = NULL; }
break;
case 663:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 667:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 668:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 669:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 670:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 671:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 672:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 673:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 674:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 677:
{ yyval = make_list (yyvsp[0]); }
break;
case 678:
{ yyval = 0; }
break;
case 680:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 685:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 689:
{ yyval = yyvsp[0]; }
break;
case 690:
{ yyval = 0; }
break;
case 691:
{ yyval = yyvsp[0]; }
break;
case 692:
{ yyval = 0; }
break;
case 693:
{ yyval = make_node (nod_top, 1, yyvsp[-2]);}
break;
case 694:
{ yyval = make_node (nod_top, 2, yyvsp[-4], yyvsp[-3]);}
break;
case 695:
{ yyval = 0;}
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
case 740:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 741:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 775:
{ yyval = yyvsp[-1]; }
break;
case 776:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 778:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 785:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 786:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 787:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 788:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 789:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 790:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 791:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 792:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 793:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 804:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 806:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 808:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 810:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 812:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 821:
{ yyval = make_list (yyvsp[-1]); }
break;
case 822:
{ yyval = yyvsp[-1]; }
break;
case 823:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 824:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 835:
{ yyval = yyvsp[0]; }
break;
case 836:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 837:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 839:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 840:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 841:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 842:
{ yyval = yyvsp[-1]; }
break;
case 843:
{ yyval = yyvsp[-1]; }
break;
case 846:
{ yyval = yyvsp[0]; }
break;
case 847:
{ yyval = yyvsp[0]; }
break;
case 848:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 849:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 850:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 851:
{ yyval = yyvsp[0]; }
break;
case 852:
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
case 853:
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
case 854:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 855:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 857:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 859:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 860:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 861:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 862:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 863:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 864:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 866:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], att_charset); }
break;
case 867:
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
case 868:
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
case 869:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 870:
{ yyval = make_parameter (); }
break;
case 871:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 872:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 873:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 874:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 875:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 876:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 877:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 878:
{ yyval = make_node (nod_proc_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 879:
{ yyval = yyvsp[0]; }
break;
case 880:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 882:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 883:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 884:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 885:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 886:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 888:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 889:
{ yyval = yyvsp[0];}
break;
case 894:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 895:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 896:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 897:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 898:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 899:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 900:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 901:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 902:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 903:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 904:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 905:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 906:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 907:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 908:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 909:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 911:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 912:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 915:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 916:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 920:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 922:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 923:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 924:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 926:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 930:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 931:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 932:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 933:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 934:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 935:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 936:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 937:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 940:
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
