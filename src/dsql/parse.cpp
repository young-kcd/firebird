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
  224,  220,  225,  225,  225,  226,  229,  234,  234,  235,
  235,  236,  236,  148,  222,   88,   90,  221,  221,   71,
   84,  239,  239,  239,  240,  243,  243,  244,  244,  244,
  244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
  244,  244,  241,  241,  242,    2,  246,  246,  246,  246,
  246,  246,  246,  254,  255,  252,  252,  256,  256,  256,
  256,  256,  256,  256,  247,  247,  257,  257,  257,  257,
  257,  257,  257,  251,  262,  262,  262,  262,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  262,  262,  262,
  262,  262,  262,  262,  262,  262,  262,  262,  259,  259,
  261,  261,  260,  258,  258,  258,  253,  253,  249,  250,
  250,  263,  263,  263,  263,  264,  264,  248,  265,  265,
  266,  266,    8,  267,  267,  267,  267,  267,  267,  267,
  267,  267,  267,  267,  267,  102,  102,  145,  145,  270,
  270,  273,  273,  275,  275,   54,   54,  272,  272,  272,
  272,  272,  272,  272,  272,  272,  280,  280,  271,  271,
  271,  271,  281,  281,   62,   62,   62,   56,   56,  277,
  277,  277,  274,  274,  274,  285,  285,  285,  284,  284,
  283,  283,  283,  278,  278,  288,  286,  286,  286,  287,
  287,  279,  279,  279,  279,  289,  289,   19,   19,   19,
  291,  291,  291,   16,   17,  295,  295,    4,   15,  294,
  294,  296,  296,  297,  297,  290,  298,  298,  299,  299,
  300,  300,  300,  300,  301,  301,  302,  302,  303,  303,
  305,  305,  305,  306,  306,  306,  307,  307,  307,  304,
  309,  309,  309,  310,  310,  308,  308,  311,  313,  313,
  312,  312,  292,   18,  314,  314,  314,  315,  315,  318,
  318,  319,   66,   66,   66,  320,  320,  320,  316,  316,
  321,  321,  322,  322,  317,  325,  323,  323,  323,  323,
  326,  326,  326,  327,  327,  327,  227,  227,  228,  228,
  329,  329,  330,  330,  330,  324,  333,  333,  334,  334,
  335,  335,  336,  336,  337,  337,  208,  208,  339,  339,
  237,  237,   32,  238,  238,  238,  238,  238,  238,  238,
  238,  231,  231,  340,  340,  341,  341,  341,  341,  341,
  343,  343,  343,  232,  232,  230,  230,  347,  347,  347,
  233,  233,  348,  349,  349,  349,  349,  349,  350,  350,
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
  245,  394,   55,  124,  276,  276,   96,  390,  390,  390,
  390,  395,  395,  395,  395,  395,  395,  395,  395,  395,
  395,  395,  396,  344,  345,  345,  345,  342,  342,  392,
  346,  346,  398,  398,  399,  399,  400,  400,  403,  403,
  401,  401,  406,  406,  405,  402,  404,  397,  397,  397,
  397,  397,  397,  397,  397,  328,  328,  156,   49,  282,
  123,  149,  370,  166,  212,  269,   64,   61,  216,  113,
   67,  332,  211,   36,   42,  338,  268,   40,   43,  207,
   41,  293,  407,
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
    7,    1,    1,    3,    4,    8,    2,    1,    3,    1,
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
  373,    0,    0,    0,  119,    0,    0,    0,  714,  703,
    0,    0,  715,    0,  711,    0,    0,    0,    0,    0,
    0,    0,    0,  332,  331,  341,  343,  236,    0,    0,
  260,  259,  258,    0,  366,    0,  636,  833,  710,    0,
  717,  324,    0,    0,  311,    0,    0,    0,  333,  263,
  261,  262,    0,    0,    0,    0,    0,  337,  315,    0,
    0,    0,    0,  716,  327,  316,  832,  719,
};
static short yydgoto[] = {                                      19,
   20,   21,   22,   23,   24,   25, 1504,   27,   28, 1505,
   30,   31,   32,   33,   34,   35,   36, 1506,   38, 1507,
  615,  103,  383,  873, 1119,  104,  649,  874,  105,  387,
  654,  128,  106,  107,  239,  945,  118,  875,  876,  179,
  208,  108,  877,  109,  389,   79,  219,  353,  436,  610,
 1056,  372,  833,  581, 1471,  810,  611,  612,  613, 1058,
  220, 1310,   71,  169,   72,  215, 1785, 1262,  200,  205,
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
 1179, 1358, 1484, 1641, 1642, 1643, 1644, 1177,  326,  544,
  788, 1227,  545,  786,  203,   56,  321,  180,  142,  304,
  167,  311,  172,  748,  745,  312,  322,  778,  323,  541,
 1011,  168,  305,  512,  546, 1228,   93,  178,  223,  584,
  585,  750, 1031,  751, 1032, 1033,  588,  589,  590,  591,
 1024,  836,  592,  593,  594,  803,  595, 1473,  800,   40,
   41,   42,  136,   58,  405,  183,  328,  278,  279,  280,
  281,  282,  283,  284,  285,  286,  715,  479,  960, 1182,
  480,  481,  720,   43,  141,  300,   44,  494,  495, 1187,
  734,  973,  122,  702,  660,  123,  124,  268,  467,  468,
  469,  705,  946,  947,  948,  949, 1171,  446, 1159, 1475,
 1476,  447, 1478,  448,  449,  450,    0, 1698, 1598, 1699,
 1700, 1701, 1745, 1786,  395, 1131,   45,   46,  359,   47,
   48,  487,  488,  882, 1127, 1312, 1316,  396,  657,  192,
  627,  628,  629,  630,  631,  632,  633,  634,  635,  636,
  637,  638, 1279, 1074, 1076, 1077,  932,  687,  451,  452,
  453,  454,  379,  217,  455,  456,  915,  457,  458,  459,
  460,  679,  919, 1160, 1155,  680,  461,
};
static short yysindex[] = {                                   4032,
 2150, -159, 3227,   84, 1598,   30, 3464,   17, 2829,   74,
  522, 2663, -159, 1463,  659,  456, 1376,  456,    0,  900,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -78,    0,    0,    0,    0,    0,    0, 6262,
  456,  456,  456,  456,  456,    0,    0,  675, 1518,  456,
  456,  456,  828,  456,  730,  456,  456,    0,  456,  456,
    0, 1017,    0,  746,    0,    0,  954,  456,    0,  456,
  456,  456,  983,  456,  456,  456,  456,  730,  456,  456,
  456,  456,    0,  456,  815,    0,    0,    0, 1290,    0,
 1290,    0,  975,  984,   94, 1370,    0,    0,    0,    0,
  456,  456,  456, 1097, 1108, 1191,  136, 1582, 1156,  250,
  263,  144, 1104,    0,  456, 1271, 1243,    0, 1194,  456,
    0,  456,  456,  456,    0,    0,    0,    0, 1402,  -35,
 1372,  630,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1091,    0, 1518,    0,
  104,    0,    0, 1643,    0,    0, 1525,    0,  145,    0,
    0, 1301,    0,    0, 1284,    0,    0, 1095,    0, 1531,
    0,    0,    0, 1518,    0,    0,    0, 1058, 1643,    0,
  472,    0,    0, 1423,    0, 1446,    0, 1290,    0,    0,
    0,    0,    0,    0, 1451,    0,    0,  456,    0, 1480,
    0, 1374,    0,    0,    0,  456,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1311,    0,  456,    0,    0,
    0, 1401, 1447, 1420,  456, 1080, 1517, 1823, 1552, 1560,
 1401, 1486, 1420, 1523, 1536, 1395,    0, 7488,    0,    0,
    0, 7488,    0,    0,    0,    0, 5578,    0,    0, 1495,
  456, 1578, 1494,  536,  456, 1527,    0,    0, 1243,    0,
    0,    0,    0,    0,    0,    0,  456,    0,    0, 1643,
    0, 1423,    0, 1290,    0, 7488, 1535,    0, 1493,    0,
  980, 1617, 1031,  630,    0, 1709, 1128,    0,  456,    0,
 1091,    0,    0,    0,    0,  456, 1575,   -6, 1529,  479,
 1949,    0, 9037,    0,    0, 1634,    0,    0,    0, 1947,
 1947, 1518, 1658, 1518, 1565, 1095,    0,    0, 8996,    0,
  456,  456,    0, 1575,    0,    0, 1736,  657, 1995,  456,
 1788,  456,    0, 2017,    0,  291, 3375,    0,    0,    0,
 1608,    0,    0,    0,    0, 1610, 1613,    0,    0, 1596,
  821,    0,    0,    0,    0, 2044,    0,    0,    0, 1562,
    0,  456, 1652,    0,  456,  456,  168,    0,    0,    0,
    0,    0,  456,  456, 1364,    0,  456,    0, 1771,  456,
 2051, 1401, 1695,    0,  456, 2062, 2065, 2066,    0,    0,
 2068, 2069,    0, 2075, 2076,    0, 2078,    0, 2081,    0,
    0,    0,    0,    0, 2088,    0,    0, 6175, 2089, 2090,
    0, 7488, 7488, 6401,  456, 2093, 1901,    0,    0,    0,
    0,    0,    0, 2045,    0, 2091,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1941,    0, 2552,    0, 1819, 2098,    0,  -26,   87,
    0,  795,    0,  402,    0,  402,    0,    0, 2099,    0,
  154, 1717,    0,    0,    0, 2104,  180, 2084,    0, 1575,
 2111, 1888, 2701, 2119,    0,    0, 1781,    0,  685, 1818,
    0,    0,    0,    0, 1518,    0, 1947, 1947, 1947, 1947,
    0, 2124,    0, 1891, 1895,    0,    0,    0, 1869,    0,
 8996,    0,    0, 8996, 1574,    0, 1643, 1910,  456, 4891,
    0,    0,  826,    0,  456, 1383, 1525,    0, 1779, 1739,
 1712,    0,    0,    0,  718, 1782,    0, 1727,  730,    0,
 1518,    0, 1911,    0,    0, 1565,    0,    0,    0,    0,
   97, 1742,    0,    0,    0, 1745,    0, 1798, 2154,    0,
 1856, 2169,   16,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2120,  238,    0,    0,    0,
    0,   79, 2175, 2178, 2169, 1643,    0, 1904,    0, 1964,
    0, 1518, 1518,    0,   -6,  145,    0, 1845,    0, 1820,
 2192,    0, 6423,  270,    0, 1859, 1854, 2206, 3700, 2210,
    0,    0,    0, 3615, 4825,  644,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1715,  821,  456,    0,    0, 1710, 1814,    0,
    0, 1799, 1420,    0, 1945,    0, 1857, 1463, 2219,    0,
 1945, 1710, 1954, 1956,  456,    0,  880, 6524,  620,  456,
 7488,  918, 1332, 1468, 2554, 7488, 3700, 2552, 1826,  843,
 7488, 6524,  780,  780, 1463, 2006, 2225,    0,    0, 6647,
  456, 7488, 7488, 7488, 7488, 7488,    0, 7488,  121,    0,
  106, 1831, 6524,  456,    0,    0,    0, 1347,    0,    0,
  -11,    0, 1836,    0,    0,    0,  456, 1482,  456,    0,
    0,  456,  456,    0,    0, 6524, 2029,   -6,    0,  456,
 1017, 7488,  456, 1840, 1518, 1518,    0,  485, 1727, 1727,
 1832,  730, 1031,    0,    0, 2256,    0,    0,    0,    0,
 2020,    0,  456,    0,    0,    0,    0,    0, 2028, 2259,
    0, 2012, 2034,    0,    0, 1972, 1977, 1290, 2046,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  730,
  456, 7118, 1936, 1938, 1942,    0,  730,    0, 1692,    0,
    0, 1913,    0,  163, 1917,    0,    0,    0,  730,    0,
 2154,  115,    0,    0,    0, 2035,  115, 1916,  115,    0,
 2298,  730,  730,  730,    0, 1575,  456,    0, 1984,    0,
 1908,    0,    0,    0, 1634, 1924,  456, 1929,  141,    0,
    0, 2304, 2071,    0,    0,    0,    0,  291,  456, 1928,
    0, 1928, 4260,  409, 7488, 7488, 4321, 2308, 1476, 4472,
 7488, 4556, 1046, 4791, 5026, 6770, 5108, 5261, 5343, 3700,
 3700,    0,    0,    0,    0,    0,  456,  456,  456,  456,
  456,  456,  171, 2305,    0,    0,    0, 1710, 1965,    0,
  -66, 1985,  456,    0,  144, 6524, 1985, 2311, 2305, 1710,
  610, 2036, 7488, 7488, 2094, 7488, 2320, 7488, 2330, 2381,
 7488, 7488, 7488, 7488, 7488, 7488,    0,    0,    0,    0,
    0,    0,    0,    0, 2055, 1375,   27, 7488, 1006, 6524,
    0, 3700, 2321, 2552,    0, 2333,  144,    0,    0,    0,
 2552, 1933,    0,    0, 2100,  780,  780,  702,  702,  894,
    0,    0,    0,  106, 2342, 2339, 1307,    0,    0, 3700,
 2056,    0,    0,    0,    0,    0,    0,    0,    0,  -34,
    0,    0,    0,    0,    0, 1935, 1924,    0, 1883,    0,
 2357, 1897,    0,    0,    0,    0, 1948,    0, 1947, 2138,
    0,  485,    0,    0,    0,    0,    0,    0,    0, 2373,
 3700,  804,    0,    0, 2114,    0,    0,    0,  804,    0,
    0,    0, 1290, 1290, 1915, 2378,    0,    0,    0,    0,
    0,  733,  -18, 1449,    0, 2156,    0,    0,    0,  456,
   84, 1960, 2002, 2020, 2389,    0, 1727, 1975,    0,    0,
 1298,    0, 2376,  456, 1485,  730, 2390, 2394, 2395, 2173,
  145, 2114, 1947, 1818,   -6,    0, 1782,  144, 1987, 2003,
    0,  139,    0,  730, 2057, 2140, 6423,    0,    0,  730,
 1962, 2143,    0,    0, 2417, 2420,    0, 1210, 2552, 2426,
    0,    0, 2552, 2427, 6893,    0,    0,    0, 2116, 2436,
 2552, 2437, 1892, 2454, 2552, 2459, 7488, 7488, 2308, 7488,
 7016, 2461, 2552, 2463, 2464, 2552, 2471, 7488, 2552, 2474,
 2552, 2477, 2478, 2552, 2479, 2482, 2552, 2483,    0, 2218,
    0,    0,    0,    0,    0,    0, 2177, 1710,    0, 1710,
  171, 2305,    0,  232,  232,  187,    0,    0, 5578,    0,
 1976,    0, 2311, 2305, 2485,  610, 2440, 2448,    0, 2455,
    0, 2577, 7488,    0, 2601, 2714, 2841, 2889, 2897, 2911,
 7488,  730, 6524, 2552, 2103, 6524,    0, 7488,    0, 2232,
  648, 7488, 6524, 7488, 7488,    0,    0, 1307, 2504, 6524,
  456,  106, 2157,    0, 2162, 2163, 2211, 2276, 2222,    0,
    0,    0, 2114,    0, 1987,  381,    0,    0, 2133, 1518,
 2166, 1727, 2167, 1727,    0, 3700,  481,    0,    0,    0,
    0,    0,    0,    0, 2113, 2292, 2114,    0, 7488,    0,
 2277, 2158, 1915, 1017,    0, 3700,    0,    0,    0, 2181,
 2184, 2186, 2187, 2189, 2190,    0,    0,    0,    0,    0,
 2540,   84,    0, 1832,    0,    0,  115,    0,  804, 2203,
  115,    0,  115,    0, 2020, 2542,    0,    0,    0,    0,
 1634, 2292, 1727,    0, 1818,    0,    0, 5578, 2306,    0,
   36,    0, 2357,    0, 2546,    0, 2547, 1518,  -87, 2550,
    0, 1518,    0,    0, 7488, 2188, 2188, 1463, 2555, 1980,
    0, 2188, 2188, 7488, 2188, 2188, 2349, 2552,    0, 2258,
 7488, 2552, 2188, 2188, 2188, 2188, 2552, 2188, 2188, 2188,
 2188, 2188, 2188, 2201,    0,    0,    0,    0,    0,    0,
    0, 2165,    0,    0,    0,    0, 1819, 6524,    0,  610,
 2485,    0,    0, 8996, 2564,    0,    0, 3177,    0,    0,
    0,    0,    0,    0, 3197,  124,    0, 6524, 2300, 2174,
    0, 6524, 3294, 1989,    0, 1705, 2552,    0, 2000,    0,
 1307,    0,    0,    0,  106, 1537, 3700, 2215, 2292,    0,
    0,    0, 1947,    0,    0, 1518, 1692,    0,    0,  772,
    0,    0,  456, 5688,    0,    0, 2552, 2336, 2277,    0,
 1294,  456,    0, 2275,  796, 2180, 2282, 2182, 2324, 2288,
 2326, 2114,    0,    0, 2589,    0, 2592,    0, 2281, 2336,
 2203,    0,    0,    0,    0,    0,    0,    0,    0, 2114,
 1782,    0, 1692, 2156, 2318, 2255,    0, 1924,    0,    0,
    0, 2280,  -85,    0, 2020, 2283, 2552, 2613, 2615,  144,
    0,    0, 2617, 2618, 2552, 2622, 2624, 7488, 7488, 2552,
 2625, 2630, 2632, 2633, 2634, 2636, 2637, 2643, 2645, 2646,
    0,  232, 1831,    0,    0,    0,    0,    0,  730,    0,
    0,    0, 6524,    0,    0, 6524,    0, 1831,    0, 1134,
    0, 2410,    0, 2647, 2648,    0,    0,    0,    0,    0,
    0,  644, 1207,    0,    0,    0, 1727,    0,    0,    0,
    0, 8996,  181, 1000, 2642, 1535, 2655, 2361, 7488, 2644,
 2666, 2651, 2652, 2659, 2660, 2380, 2664,    0,    0,    0,
 2405, 4691, 2665,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2681,  456,    0,    0, 2379, 2388,
  456, 1915,    0,    0, 1290,  456,    0,    0,    0,    0,
    0,    0,    0, 2292,    0,    0,    0,    0,    0, 2292,
    0,    0,    0,    0,  152, 1831,    0,    0, 7488, 1518,
    0,    0,    0, 1518,    0,    0, 7488,    0,    0,    0,
    0, 2552, 2552,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2056, 2695,    0,    0, 2056, 3700,
  456,  146, 1537,    0,    0, 2369,    0, 2700,    0,    0,
    0,  185,    0, 7242,  456,    0,    0, 7488,    0, 2404,
 3700, 2365,    0, 3700,    0,    0,    0,    0,  173,    0,
    0,  678,    0, 2295,    0,    0,    0,    0,    0, 1915,
 1290,    0, 2367,    0,    0,    0,    0, 2156,    0,  152,
 2705, 1307,    0,    0, 2056, 2552,    0,    0, 1705, 2222,
    0, 2222,  644,    0,    0,    0,  671,  804,    0, 2694,
    0, 2198, 7365, 2768,  173,  799, 7488, 2703,  848, 1141,
    0,    0,    0,  456,  456,   84,  156,    0,    0,    0,
 2367,  513, 1915, 2382, 2383,    0,    0, 1307, 2722,  152,
 2421, 2222, 2729, 1831, 2215, 2215,  456,    0, 2730, 2734,
 1490,    0,    0,    0, 5806, 2368, 2738,    0,  588, 2352,
 2552,    0, 2480,    0,  173,    0,    0,    0,    0, 5688,
  678, 1915,  524,  524,    0, 2489,    0, 2350,    0,    0,
    0, 1307,  152, 2215,    0, 2056, 2408, 2408,    0,    0,
  671, 2752,    0,  456,    0, 2006, 2025,   91, 2743, 2509,
 2499, 5688, 5688,    0,    0,    0,    0,    0, 2341,   29,
    0,    0,    0, 1184,    0, 2222,    0,    0,    0,  456,
    0,    0,  173, 2778,    0,  456, 5688, 2515,    0,    0,
    0,    0, 3700, 2215, 2785, 2771, 2027,    0,    0, 5688,
  644, 2408,  456,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  259,  700,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  169, 4002,    0,    0,    0,    0,    0, 2830,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  645,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1009,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2495,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2456,    0,    0,    0,  130,    0,
  130,    0,    0,    0,    0, 2457,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1149,    0,
    0, 5929, 4237,    0,    0,    0, 1150,    0,    0, 1018,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  430,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   85,    0,    0,    0,    0,  817,    0,
    0, 1169,    0,    0,    0,    0,    0, 1569,    0, 9060,
    0,    0,    0,    0,    0,    0,    0,    0,   85,    0,
  -30,    0,    0, 2799,    0,    0,    0, 2579,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1202,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1273,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1416,    0,    0,
    0,    0,    0,    0,    0, 2386,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  544,    0,    0, 1304,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   85,
    0, 2799,    0, 2579,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1394,    0, 2570,    0,    0,    0,    0,
 1408,    0,    0,    0,    0,    0, 2590,  622, 6117,    0,
 1440,    0,    0,    0,    0,  260,    0,    0,    0, 2396,
 2396,    0,    0,    0, 1466, 1852,    0,    0,    0,    0,
    0,    0,    0, 2590,    0,    0, 1583,    0,    0,    0,
    0,    0,    0, 4567,    0, 2466,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1481,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1500,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1501,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2018,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1778,    0,    0,  353,    0,    0, 2538,    0,  155,    0,
    0,    0,    0,  845,    0,  845,    0,    0,  601,    0,
  696, 1096,    0,    0,    0,    0, 1202,    0,  911, 2590,
    0,    0,  175, 1190,    0,    0,  193,    0, 2997,  320,
    0,    0,    0,    0,    0,    0, 2396, 2396, 2396, 2396,
    0,  459,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2596,    0,    0,    0,
    0,    0, 2570,    0,    0, 1384,    0,  800, 2393,    0,
    0,    0,    0,    0,    0,  386,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1555,    0,    0,    0,    0,
 2161,  857,    0,    0,    0,  933,    0,    0, 1278,    0,
    0, 3543,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1475, 1847,    0,    0,    0,
    0, 3940, 1153,    0, 3543,   85,    0,    0,    0,    0,
    0,    0,    0,    0,  622, 1649,    0,    0,    0,    0,
 2449,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  751,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  358,    0,    0, 4002,    0,    0,
  358,    0,    0,    0,    0,    0, 7611,    0, 7611,    0,
    0, 7611, 7611, 7611,    0,    0,    0, 2413,    0,    0,
    0,    0, 2775, 2945, 6052,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1313,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1178,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  622,    0,    0,
  164,    0,    0,  824,    0,    0,    0, 3061,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  886,    0,    0,    0,    0,    0,    0,    0, 2826,    0,
    0, 2416, 2032,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  360,  406,  643,    0,    0,    0, 1912,    0,
    0,    0,    0,    0, 1816,    0,    0,    0,    0,    0,
 1278,    0,    0,    0,    0, 1107,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2590,    0,    0,  352,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7271,    0,
    0,    0,  326,    0,    0,    0,    0, 2557,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1564, 1580,    0,    0,    0,    0,    0,    0,
    0, 1587,    0,    0, 5929,    0, 1587, 1641, 1644,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  937,    0,    0, 7611,    0,    0,    0,
 1355,    0,    0,    0, 2328, 3105, 3256, 2465, 2638,    0,
    0,    0,    0,    0, 3428, 1815, 1553,    0,    0,    0,
  784,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  280, 1366,    0,
  331,    0,    0,    0,    0,    0,    0,    0, 2396, 2411,
    0, 3174,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2598,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1567,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1650,    0,    0,    0,    0,
    0,    0,    0, 3964,    0,    0,    0,    0,    0,    0,
    0,    0, 1645,    0,    0,    0,    0,    0,    0,    0,
 1649, 2598, 2396, 1653,  622,    0, 2607, 5929, 1659,  117,
    0,    0,    0,    0, 7271,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 7746,    0,
    0,    0, 7874,    0,    0,    0,    0,    0,    0,    0,
 7885,    0, 7957,    0, 7968,    0,    0,    0,    0,    0,
    0,    0, 8105,    0,    0, 8125,    0,    0, 8208,    0,
 8291,    0,    0, 8302,    0,    0, 8336,    0,    0, 8938,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1564, 1671,    0, 2435,  392,    0,    0,    0,    0,    0,
    0,    0, 1682, 1698, 1699,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2438,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2531, 1213,    0,
 3605,    0, 2533,    0, 2534, 2541,    0,    0,  189,    0,
    0,    0, 2598,    0, 1659,    0,    0,    0, 2850,    0,
    0,    0, 2430,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2429,    0, 2621,    0,    0,    0,
 1530,    0, 1567, 2495,    0,    0,    0,    0,    0,  703,
  735,  782,  805,  844,  962,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  683,
    0,    0,    0,    0, 2520,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1716,    0,    0,    0,    0,    0,
    0,    0, 1737,    0,    0,    0,    0,    0,  142,    0,
    0,    0,    0,    0,    0,    0,    0, 6052,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 8432,    0, 8464,
    0, 8528,    0,    0,    0,    0, 8570,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1794,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1673,    0,    0,    0,
 1581,    0,    0,    0,    0,    0,    0,  375,    0,    0,
    0,    0, 2396,    0,    0,    0, 2185,    0,    0,    0,
    0,    0,    0, 2584,    0,    0, 2855, 1619, 1846,    0,
 2570,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2598,    0,    0,    0,    0,    0,    0,    0, 1800,
  846,    0,    0,    0,    0,    0,    0,    0,    0, 2598,
 2607,    0,  378,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  126,    0, 8612,    0,    0, 7611,
    0,    0,    0,    0, 8708,    0,    0,    0,    0, 8740,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  392,  850,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1004,    0, 2531,
    0,  357,    0,    0, 1136,    0,    0,    0,    0,    0,
    0,  222, 2857,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2595,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2472,    0,    0,  112,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  109,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 8836, 8927,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  989,    0,    0,    0,  563,    0,
    0,    0,    0,    0,    0, 2858,    0,    0,    0,    0,
    0, 2846,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2848,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2599,    0,    0,    0,    0,    0, 2472,
  186,    0,  118,    0,    0,    0,    0,    0,    0,    0,
  368, 1272,    0,    0,  458, 2855,    0,    0,    0,  403,
    0,  431, 5857,    0,    0,    0, 2857,    0,    0,    0,
    0,    0,   98,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  210,    0, 1567,  369,  469,    0,    0, 2531, 1348,    0,
    0,  143,    0, 1004,  348,  267, 1492,    0,    0, 2870,
    0,    0,    0,    0,    0, 2853,  114,    0, 2608,    0,
 2856,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2472,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1390,    0,  147,    0,  563, 1406, 2875,    0,    0,
 2857,    0,    0,    0,    0, 2878,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2531,    0,  431,    0,    0,    0,    0,
    0,    0,    0, 2860,    0,    0,    0, 5581,    0,    0,
    0,    0,    0,  267, 2883,    0,    0,    0,    0,    0,
  715, 2875,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2925,    0,    0, 2929,
    0,    0,    0,    0,    0,    0,    0,   11,    0, 2933,
    1,  896,   51, -574, 1822,  914, -218, -397, 2923, 2684,
    0,  -38,    0, 2702,  -93,   46,    0,  806, -778,   23,
  -42,  -33,  -92, 2704, 2302,    0,    0,    0, 1261,    0,
    0,  -49, 1894, -524,   19, -760, 2355, 2131, -274, 1914,
 2882, -263,    0,  -57,    0, -677,  -51,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -548,  -84, 1919, -277, 1109, -900,
    0,    0,    0,    0, -313,  -99, -350, 1931, -309,  -46,
    0, -498,    0,    0, -506, -951,    0, 1572,    0,    0,
 2670, -523,  -19,    0,    0,    0,    0, 2649,    0, 2422,
 2682, -167, 1950, -639,    0,    0,    0, -698,    0,    0,
    0,    0,    0,    0, 2010,    0, 2711, 2265, 1951, 2677,
 2690,    0, 1407,    0, -491, 2247,  -48, -615, -701,    0,
    0,  -15, -952, -905, -894, -145, -865,    0, 1632, -970,
    0, 1331,-1151, 1633,    0, 2481,    0,    0,    0, 1328,
 1336, 1297,  603, -247, -891, -294,    0,    0, 2270,    0,
 1817,    0,    0, -308,    0,    0,    0,    0, -220,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -390,    0,    0, -322,    0,-1114, -740,    0,
    0, 1260, 1417,    0, 1321,    0,    0, 2672,  -97, 2077,
 1860, 1686,    0,    0,    0, -405, -799, -853,    0, -470,
 -507, -271, -272,    0, -767, 1410,  851, -162, -462, -659,
 -863,-1147,    0,    0,   -2,    0,    0,    0,    0,    0,
 2728,    0,    0,    0,    0, 2742, 2518,    0,    0,    0,
    0,    0, 2753,    0,    0,    0,    0,  -10,    0,    0,
    0, -319, 2249, -312, 1821, -436,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2468,    0,    0, 2264,    0,
    0,    0, 2654, 3047,    0,    0,    0,    0,    0, 2787,
    0,    0,    0,    0, 2597,    0, 2591,    0,    0,    0,
 2353,    0,    0,    0,   76,    0,  756,    0, 2340,    0,
    0,    0, -550,-1011, 1088,    0, 2950, 1642,    0, 2372,
 -600, 2384,    0, -730, 2132,    0,    0, -115, -663,    0,
 1484,-1098,    0, -934, -805, -657,    0, 1597,    0, 1341,
    0, 1388,    0, 1293,    0,    0,    0,    0, 2605,    0,
    0,    0, -344, 2434, 2214, -872,    0,    0,    0, -658,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2847, 1678, 2009,    0, -540,    0,    0,    0,
 -223,    0, 2732,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -224, 1952,    0,   -9,
};
#define YYTABLESIZE 9558
static short yytable[] = {                                     110,
  171,   76,  110,  194,   75,  130,  131,  241,  137,  186,
   37,  191,  378,  190,  288,  177,  724,  549,  926,  586,
  260,  263,  261,  264,  225,  554,  587,  204,  968,  749,
  519,  761,  752,  710,  229,  521,  211,  978,  763, 1200,
  943,  170,  173,  176,  131,  181, 1200, 1210,  234,  656,
  193,  170,  197,  969,  176, 1349,  131,  181,  235,  210,
  110, 1383,  247,  962,  249,  228,  485,  895,  221,  130,
  131,  224,  170,  248,  221,  197,  173,  176,  232,  131,
  181,  210,  110,  201,  176, 1129, 1201,  888,  834,  206,
  374,  294,  616, 1201,  292,  841,  600, 1202,  174, 1211,
  844,  193,  131,  193, 1202,  270,  231,  885,  697,  199,
 1219,  751,  233,  967,  351,  197,  362,  257,  812,  313,
  289,  465,  176,  131,  210,  964, 1204, 1164,   74, 1240,
 1773,  708,  230, 1204,  927,  511,  794,  245,  377,  236,
  995,   74,  695,  825,  340,  944,  702,  378,  435, 1002,
 1252,  388,  751,  917, 1022,  751,  325,  940,  257, 1027,
  388,  257,  942,  625, 1460, 1047,  529,  697,  571,  529,
  751,  486,  323,  751,  217,  362,  257,  290,  238,  245,
 1055,  663,   90, 1257,  971,  751, 1423,  942,  695,  381,
  737, 1640,  632,  739,  740,  741,  742,  719,  653, 1721,
  492,  695, 1042,  384,  625,  702, 1021,  625,  355,  257,
  586,  653,  384, 1168, 1118,  217,  355,  587,  217,  471,
  373,  694,  625,  723,  297,  374,  751,  571,  193,  751,
  435,  951,  131,  217,  390,  110,  478,  704,  484, 1603,
 1381,  131,  727,  390,  751, 1658,  554,  695, 1258,  259,
  257,  632, 1313,  257, 1124,  769,  123, 1477,  571,  480,
  520,  173,  694, 1235,  889,  131, 1554,  523,  257,  191,
 1561,  536,  474,  377,  539, 1317,   74,  489,  486,  216,
  694,  529,  550, 1222,  552, 1130, 1200, 1398,  860,  258,
 1009, 1359,  804,  651,  259, 1418,   57,  795,  604,  193,
  608,  399,  262, 1121,  806, 1453,  193,  702,  193,  139,
  193,  606,  259,  193,   74, 1133, 1135,  571,  480,  472,
  216,  847,  847,  216,  847,  259,  847, 1393,  809, 1781,
  631,  176,  181, 1201, 1468,   74, 1109, 1110,  216, 1306,
  131, 1308,  173,  647, 1202,  484,  111,  702,  269, 1183,
   80,  126,  736,   76,  655,  373,  686,  742,  661,  390,
  314,  805,  140,  384,  598, 1028,   86,  367,  253,   86,
 1125, 1180,  131, 1204,  702,  176,  390,  196,  472, 1424,
 1632, 1782,   14,  131,  489,  404,  596,  131,  702,  631,
  176,  527,  131,  736, 1218,  137,  736,  686, 1161,  529,
  686,  324,  695,  265, 1415,  388,  702, 1562, 1381,  253,
  126,  736,  253,  861,  112,  686,  742,  941,  390,   94,
  378, 1479, 1181,  759, 1223,  689,  367,  253,  476,  630,
  650,  123,  123,  702,  102,  529,  196,  697,  315,  217,
  697, 1351, 1459,  695,  404,  650,  892,  266,  789, 1067,
  527,   90,  664, 1048,  102,  738,  253,  683,  474,  706,
 1720,  695, 1153,  751,  388,  751,  990, 1197,  254,  992,
 1491,  695,  718,  751, 1524,  653, 1525,  217, 1680,  325,
 1122,  765,  625,  998, 1408,  999, 1314,  257,  630,  257,
 1687, 1365, 1134,  217, 1477,  751,  269,  257,  864, 1345,
 1544,  791,  963, 1263,  625,  697, 1345,  695,  325,  254,
 1016, 1552,  254,  751,  808,  217,  683,  474, 1550,  758,
  193, 1371,  465,  480,  323,  758,  811,  254,  614,  695,
  244, 1725,  834,  632, 1280, 1054, 1384,  529, 1128,  695,
  694, 1306,   73,  594,  216,  257,  865, 1551, 1707,  697,
 1480,  102,  819,  820, 1057,   73,  191,  465,   76,  847,
  388, 1321,  694,  697,  751,  847,  102,  790, 1040,  751,
 1758,  362,  707,  257, 1062,  989,  695,  472, 1251, 1584,
  465,  102,  216,  695, 1599,  695,  751,  702,  257, 1601,
 1747, 1411,  529,  751, 1396,  193,  866,  695,  216,  102,
  600,  702,  594,  683,  837,  571,  102,  486,  955,  694,
  257,   76,  102,  751,  835, 1472,  736,  257,  694,  257,
  216,  986,  652,  390, 1470, 1117,  472,  751,  863,  695,
 1567, 1715,  357,  102, 1410,  193, 1635, 1694,  390,  984,
  985,  102,  389,  390,  619,  986,  253,  632,  253,  404,
  899,  257,  390, 1639,  702,  176,  253,  480, 1479,  600,
  197,  897,  694,  614, 1563, 1192,  625, 1488,  571,  388,
  860,  631, 1370,  736,  484,  686,  694,  217,  478,  191,
  961,  934,  137, 1005,  381,  974,  974,  751,  686,  193,
   73,  176, 1385,  702,  706,  610,  367,  686, 1481,  367,
  126,  389,  395,  619,  523, 1200, 1702,  131,  257,  131,
  650,  257,  193,  489,  372,  702,  474, 1454,  193, 1209,
  934,  257,  614,  193,  736,  742,  196, 1430,   73, 1253,
  102, 1217, 1008, 1010,  396,  702, 1686,   86,  472,  345,
  465,  137,  860,  193,  686, 1693,  254,  253,  254,   73,
  696, 1239, 1201,  686,  610,  372,  254,  390,  372,  527,
  988,  395,  702, 1202,  367,  474,  535,  486,  378,  253,
  630,  193,  193,  372, 1111,  378,  253, 1114, 1041,  713,
 1057,  392,  216,  683, 1015,  631, 1730, 1480, 1052,  683,
  695,  696, 1204,  396,  113,  861, 1025,  686, 1007,  695,
 1397,  344, 1588,  388,  391,  736, 1407,  131,  367,  696,
  253,  686, 1490,  736, 1723,  702,  384,  131,  695,  474,
  367,  695,  367,  634, 1628, 1455,  696,  695, 1115, 1064,
 1037, 1038, 1039, 1482,  484,   76, 1537,  346, 1116, 1710,
  392,  979,  683,  394,  599,  138, 1203,  254,  370,  697,
  714, 1750, 1472, 1203,  683, 1419,  539,  390,  176,  181,
  390,  210,  110,  391, 1324,  370,  129,  861,  390,  254,
  253, 1527, 1688,  489, 1361,  384,  254,  474, 1672,  265,
  390,  390,  634,  259, 1362,  529,  594,  301, 1713, 1654,
  697, 1113,  490, 1548,  683,  298,  539,  539,  683,  242,
  539, 1759,  394,  599,  138,  860,  389,  115,  697,  860,
  254, 1375,  683, 1112,  980,  539,  102,  539, 1212, 1213,
  475,  594, 1732,  896, 1672,  116,  529,  955, 1337,  529,
  222, 1339,  540,  943,  176, 1481,  302, 1165,  867,  102,
 1673,  242, 1760,  600,  529,  737,  529,  539, 1392,  594,
  683,  953,  594,  735,  953, 1724,  966, 1412,  138,  683,
  376,  393,  137,  619,  116, 1764,  395, 1264,  594,  953,
  254,  953,  540,  540, 1755,  476,  540,  670,  600, 1414,
  670,  691, 1585,  465,  125,  619, 1167,  692,  683, 1761,
 1674,  540,  477,  540,  594,  670,  191, 1589,  396, 1345,
 1602, 1675,  116,  381,  586,  603,  600, 1672,  573,  600,
 1230,  587, 1558,  254, 1594,  378, 1489,  673,   76,  783,
  393, 1231,  242,  540, 1230,  600, 1311, 1311, 1315,  683,
  861,  255, 1672,  860,  861,  193,  372,  116,  610, 1595,
  389, 1345,  193,  372,  697,  392,  372,  683,  303, 1487,
  372,  600,  496,  870, 1246, 1350,  784,  860,  372,  691,
  860,  673, 1553,  242, 1485,  692, 1653,  573,  391,  696,
  102, 1220, 1266,  610,  372,  384,  673, 1650, 1270,  696,
  384, 1652,  696, 1342,  182, 1645,  384, 1666,  198,  126,
 1669,  696, 1367, 1203, 1369,  595,  127, 1596,  372,  361,
  395,  610,  683,  769,  610, 1676,  136,  394,  390,  860,
  390,  372,  465, 1461,  539,  683,  361, 1464,  184,  185,
  610,   76,   76,  376,  683,  138,  390,  410,  372,  755,
  539,  102,  396,  539,  539,  682,  539, 1692,  696,  265,
  974,  539, 1559,  529,  539,  766,  610,  696,  569,  578,
  920,  921,  534, 1413,  595,  372,  362,  539,  861,  529,
  785,  289,  176,  539,  634,  136,  539,  372,  575,  372,
 1336,  683,  465,  362,  953, 1221,  682,  265,  697,  392,
  683,  697,  861,  893, 1715,  861,  529,  599,   73,  618,
  540,  696,  529,  534,  682,  529,  534,  973,  816, 1714,
  711,  697,  391, 1623,  216,  696,  540,  569,  578,  540,
  540,  534,  540,  534,  384,  953,  237,  540, 1422,  276,
  540,  901, 1426, 1736,  683,  393,  767,  575, 1766,   76,
  618,  416, 1395,  540,  861,  539,  453,  697, 1587,  540,
 1474,  394,  540,  534,  670,  670,  697,  418,  618, 1636,
  599,  695,  693,  599,  694, 1637,  696,  539,  486, 1791,
  697,  363,  364,  365,  539,  184,  185,  768,  539,  599,
  973,  368,  351, 1638, 1709,  539,  218,  557,  363,  364,
  365,  213,  184,  185,  366,  367,  420,  421,  422,  368,
  369,  423,  424,  539,  922,  599, 1671,  529,  539,  426,
  427,  366,  367,  577,  529,  226,  368,  369,  431,  507,
  390,  540,  697, 1156, 1157,  368,  974, 1087,  557,  214,
  683,  557,  529,  539,  191,  484, 1492,  539,  498,  238,
  368,  351,  697,  540, 1088,  697,  557,  499,  557,  673,
  540, 1241, 1671, 1535,  540,  176,  673,  670,  306,  673,
  371,  540,  529,  673, 1311,  370,  529, 1774,  242,  393,
  508,  673,  577,  193,  489,  628,  437,  243,  557,  540,
  462,  697,  131,  259,  540,  464,  683,  673, 1695, 1089,
 1696,   96, 1787,  466,  136,  683,  136, 1600,  670,  369,
 1242,  697, 1754,  411,  509,  864,  486,  307,  864,  540,
  697,  673, 1605,  540,  493,  619,  628,  412, 1090,  628,
  534, 1756,  510,  246,  673,  517,  695,  693,   98,  694,
 1734,  696, 1737, 1738,  628, 1671,  534,  466,  518,  534,
  534,  673,  534,  369,  378, 1604,  673,  534,  595,  408,
  534, 1633,  466, 1778, 1779, 1606,  619,  864,  369,   76,
 1671,  330,  411,  534,  682, 1173, 1586, 1158,  673,  534,
  341, 1765,  534,  484,  619,  161,  412,  682, 1789, 1174,
  673, 1275,  673,  595,  506, 1091,  682, 1474,  354, 1691,
  349, 1796,  250,  170, 1634,  136,  360,   99,  331,  691,
  332,  251, 1631, 1175, 1784,  692,  342,  100,  408,   44,
   52,  595,  489, 1474,  595, 1173, 1610, 1607,  618,  308,
 1647, 1792, 1203,  333, 1648,  506,  934, 1590,  506, 1174,
  595,  131,  101,  682,  161, 1691,  173,  309, 1241,  230,
  618,  534,  682,  506,  659,  557,  678, 1681,  334,  349,
  683,  684,  686, 1175,  130,  131,  595, 1176,  659, 1474,
 1594,  557,  657,  534,  162,  557,  659,  557,   44,   52,
  534,  310,  557,   42,  534,  557,  251, 1783,  152, 1691,
  230,  534,  659,  230,  252, 1595,  682, 1245,  557,   25,
  658,  934,  193,  603,  557,  272,  747,  557,  230,  534,
  682,  265,  256,  657,  534,  176,  657, 1176,  121, 1474,
  368, 1691,  646,  368,  486,  645,  361,  251,  271,  489,
  251,  657,  287,  162,  754,  681, 1716,  753,  217,  534,
  273,  658,   42,  534,  658,  251,  659,  152, 1173,  130,
  131,  697, 1474, 1596,  603,  903,  486,  486,   25,  658,
   48,  697, 1174,   50,  697,  747, 1697,  131,  274,  482,
 1663,  275,  128,  697,  691,  489,  557, 1474,  379,  217,
  692,  486,  217,  362,  170, 1718, 1175,  276,  368,  370,
   26,  484, 1529,   76,  486,  296, 1719,  217,  557,  130,
  131,   49,  316,  370,  628,  557, 1697,  131,  504,  557,
  299,  370, 1771,  277, 1530, 1152,  557,   51,   46,   48,
  697, 1531,   50,  484,  484,  489,  628,  370,  482,  697,
  489,  128,  368,  865,  557,  129,  865,  379,  369,  557,
 1176,  369,  130,  131,  368,  327,  368,  265,  484,   26,
 1697,  131,  843,  681,  173, 1532,  117,  504,  489,  348,
   49,  484,  489,  489,  557,  329,  695,  693,  557,  694,
 1224,  696,  506,  697,  506,  862,   51,   46,  644,  506,
  173,  370,  506,  489,  350,  865, 1064,  489,  363,  364,
  365,  905,  184,  185,  129,  506,  464,  953,  132,  900,
  489,  658,  318,  173,  916,  319,  369, 1225,  352,  923,
  924,  366,  367,   47,  338,  117,  368,  369,  931,  217,
  935,  936,  937,  938,  939,  257,  931,  242,  659,  230,
  133,  464, 1767, 1768,  656,  524,  356,  949,  953,  953,
  953,  953,  953,  966,  953,  134,  357, 1742, 1078,  713,
  369,  320,  954,  756,  464,  382,  953,  953,  953,  953,
  493,   95,  369,  507,  369,  231,  529,  152,  776,  385,
  777,  153,   47,  506, 1079,  656,  524,  393,  217,  524,
  756,  102,  394,  386, 1743,  553,  713, 1797,  953,  152,
  953,  657,  397,  656,  524,  506,  524, 1744,  398,  713,
  102,  657,  506,   96,  657,  958,  231,  529,  400,  231,
  529,  412,  542,  657,  508,   97,  681,  884,  152,  658,
  883,  404,  959,  543,  231,  529,  402,  384,  242,  658,
  153,  196,  658,  498,   77,   78,  506,  152,  384,  403,
   98,  658,  499, 1305,  681, 1307,  152, 1065,  509, 1066,
  242,  470,  152,  695,  693,  497,  694,  242,  696,  472,
  657,  697,  695,  693,  473,  694,  510,  696, 1046,  657,
  152, 1045,   14, 1068, 1069, 1073,  120,  121, 1081, 1083,
 1085,  482, 1093, 1096, 1099, 1101, 1104, 1107,  658, 1053,
  196,  242,  830, 1166,  506, 1184, 1165,  658, 1045,  184,
  185,  700,  695,  693,  691,  694,  527,  696, 1191,   99,
  692, 1190,  537,  657,  464,   73,  514,  102,  534,  100,
 1233, 1137, 1138, 1232, 1140,  657, 1142,  547,  419, 1145,
 1146, 1147, 1148, 1149, 1150, 1238, 1319,  834, 1237, 1318,
 1432,  658,  601, 1165,  101,  701, 1154,  425,  924, 1467,
  551,  496, 1466,  658,  605,  428,  429,  430,  867,  953,
 1469,  953,  953, 1466,  124,  124,  928,  695,  693,  953,
  694,  607,  696,  363,  364,  365,  609,  953,  834,  834,
  834,  834,  834,  953,  834, 1772,  953, 1795, 1466,  639,
 1715,  640,  496,  524,  641,  496,  834,  834,  834,  834,
  953,  368,  369, 1017, 1018,  953,  953,  644,  648,  953,
  496,  662,  524,  524,  653,  524,  953,  665,  953,  953,
  524,  667,  953,  524,  668,  669,  953,  670,  671,  953,
  834,  953,  868,  953,  672,  673,  524,  674,  953,  953,
  675,  953,  524,  242,  529,  231,  529,  676,  681,  682,
  153,  529,  690,  656,  529,  698,  699,  953,  953,  701,
  953,  703,  717,  656,  726,  721,  656,  529,  869,  722,
  728,  729,  153,  870,  953,  656,  953,  953,  953,  871,
  527,  953,  732,  733,  953,  953,  498,  743,  744,  747,
  102,  691,  746,  756,  953,  781,  780,  692,  872,  787,
  691,  153,  782,  931,  196,  257,  692,  792,  953,  796,
  196,  953,  797,  799,  524, 1287, 1288,  798, 1290, 1292,
  153,  527,  656, 1284,  527,  801, 1297,  953,  802,  153,
  807,  656,  196,  953,  813,  153,  524,  814,  953,  527,
  691,  527,  817,  524,  242,  529,  692,  818,  827,  953,
  953,  829,  953,  153,  524,  830,  839,  464,  953,  695,
  693,  196,  694,  196,  696,  840,  242,  529,  838,  842,
  878, 1328,  524,  242,  529,  656, 1704,  524,  886, 1335,
  196,  924,  881,  879,  924,  929, 1154,  656,  196,  196,
 1343,  924, 1346, 1347,  890,  196,  891,  918,  924,  834,
  953,  834,  834,  950,  956,  691,  524,  242,  529,  834,
  986,  692,  965,  196,  972,  991,  808,  834,  997,  695,
  693,  996,  694,  834,  696,  196,  834,  196,  894,  496,
  898,  496,  999,  902,  904,  906, 1003, 1377,  759,  496,
  834, 1004, 1012, 1006, 1013,  834,  834,  846, 1014,  834,
  196, 1020,  496, 1023, 1034, 1029,  834, 1036,  834,  834,
 1043, 1048,  834, 1060, 1061,  658,  834, 1075, 1120,  834,
 1123,  834, 1126,  834, 1118,  196, 1136, 1139,  834,  834,
 1141,  834,  695,  693, 1162,  694,  464,  696,  846,  846,
  846,  846,  846, 1143,  846, 1151, 1163,  834,  834,  691,
  834, 1170, 1172, 1427, 1178, 1186,  846,  846,  846,  846,
  695,  693, 1435,  694,  834,  696,  834,  834,  834, 1440,
  645,  834, 1188, 1193,  834,  834,  695,  693, 1667,  694,
  496,  696, 1196, 1205,  834,  223, 1214, 1216,  527, 1226,
  846, 1144,  695,  693, 1234,  694,  464,  696,  834, 1236,
 1247,  834,  496, 1243, 1248, 1249, 1250,  527,  527,  496,
  527, 1259,  196, 1261,   49,  527,  924,  834,  527, 1268,
  924, 1054, 1272,  834, 1271,   50,  223, 1273,  834,  223,
 1274,  527,   51,  196,  849, 1276, 1277,  527, 1281,  834,
  834,  251,  834,  496,  223, 1282, 1283,  691,  834,  860,
 1322,  695,  693,  692,  694,  196,  696,   52, 1323,  695,
  693,  196,  694, 1285,  696, 1326,  695,  693, 1286,  694,
 1293,  696, 1294, 1295, 1304,  849,  849,  849,  849,  849,
 1296,  849,  251, 1298,  196,  251, 1299, 1300, 1301,  529,
  834, 1302, 1303,  849,  849,  849,  849, 1082, 1320, 1086,
  251, 1094, 1097,  196, 1102, 1105, 1108,  691, 1338,  527,
 1341,  196,  196,  692, 1348, 1352, 1572, 1573,  196, 1356,
 1353, 1354,   53, 1357, 1355, 1363, 1366,  849, 1368, 1372,
  529,  527, 1374,  529,  529, 1382,  196, 1386,  527, 1439,
 1387,  924, 1388, 1389,  924, 1390, 1391,  527,  529,  527,
 1394, 1399, 1409, 1416,   54, 1420, 1451, 1421,   55,  846,
 1425,  846,  846,  695,  693, 1431,  694,  527,  696,  846,
  691, 1452,  527,  196, 1456, 1278,  692, 1612, 1462, 1463,
 1438, 1483, 1536,  846,  196, 1526,  846, 1327,  695,  693,
 1539,  694, 1538,  696, 1540, 1541, 1542, 1543,  691, 1545,
  846,  527, 1546, 1547,  692,  846,  846,  850, 1555,  846,
 1557, 1329,  695,  693,  691,  694,  846,  696,  846,  846,
  692, 1560,  846, 1565, 1564, 1566,  846, 1568, 1569,  846,
  691,  846, 1570,  846, 1571, 1574,  692, 1646,  846,  846,
 1575,  846, 1576, 1577, 1578, 1649, 1579, 1580,  850,  850,
  850,  850,  850, 1581,  850, 1582, 1583,  846,  846, 1591,
  846, 1593, 1592,  223, 1611,  223,  850,  850,  850,  850,
 1609,  112, 1613,  223,  846, 1614,  846,  846,  846, 1615,
 1616,  846, 1662, 1621,  846,  846, 1664, 1617, 1618,  691,
 1619, 1627, 1620, 1626,  846,  692,  849,  691,  849,  849,
  850, 1629, 1630,  692,  691, 1651,  849, 1656,  846, 1657,
  692,  846,  695,  693, 1665,  694, 1622,  696, 1690,  251,
 1682,  251, 1703,  849, 1330,  695,  693,  846,  694,  251,
  696, 1712, 1731,  846, 1733, 1726, 1728,  849,  846, 1735,
 1740,  924,  849,  849,  844, 1711,  849, 1741, 1748,  846,
  846, 1466,  846,  849, 1753,  849,  849, 1752,  846,  849,
 1723, 1770, 1724,  849,  223,  139,  849,  529,  849,  529,
  849, 1775, 1776, 1777, 1780,  849,  849,  529,  849,  695,
  693, 1794,  694, 1746,  696,  844,  223,  844,  844,  844,
  529, 1715, 1790,  223,  849,  849, 1708,  849, 1793,    1,
  846,  691,  625,  844,  844,  844,  844,  692,  203,   30,
   32,  849,  751,  849,  849,  849,  567,  376,  849,  181,
  251,  849,  849,  271,  149,  527,  691,  223,  649,  269,
   82,  849,  692,  463,  936,  213,  527,  844,  276,  193,
  404,  527,  251,  935,  681,  849,  679,  675,  849,  251,
  691, 1331,  695,  693,  677,  694,  692,  696,  194,  281,
  181,  275,  289,  181,  849,  377,  708,  707,  529,  850,
  849,  850,  850,  287,  223,  849,  320,  288,  181,  850,
  709,  328,  338,  251,  319,  619,  849,  849,  326,  849,
  529,  670,   95,  718,   26,  849,  850,  529,   29, 1332,
  695,  693,   39,  694,  117,  696,  401, 1333,  695,  693,
  850,  694, 1309,  696,  843,  850,  850,  392,  391,  850,
 1269, 1334,  695,  693,  880,  694,  850,  696,  850,  850,
 1059,  529,  850,  828,   96,  227,  850,  849, 1267,  850,
 1265,  850, 1549,  850, 1255,  516,   97,  793,  850,  850,
  730,  850,  502, 1244,  560,  843,  692,  843,  843,  843,
  114, 1195,  966,  691,  531, 1256,  177,  850,  850,  692,
  850,   98,  491,  843,  843,  843,  843,  532, 1659, 1001,
 1528, 1722, 1729, 1534,  850,  775,  850,  850,  850, 1727,
 1763,  850,  994, 1376,  850,  850,  907,  908,  909,  910,
  911,  912,  913,  914,  850, 1788,  844,  843,  844,  844,
 1679, 1757,  643, 1185, 1360, 1486,  844,  691,  850, 1689,
  540,  850,  522,  692,  779,  177,  513, 1035,  666,  119,
  187, 1406,  815,  844, 1026,  483,  716,  850,  712,  957,
   99,  970,  269,  850,  952, 1169, 1655,  844,  850, 1597,
  100, 1769,  844,  844, 1739, 1798,  844,  953,   95,  850,
  850,  725,  850,  844,  887,  844,  844, 1289,  850,  844,
 1132,  642,    0,  844,  845,  101,  844,  181,  844, 1340,
  844,    0,    0,    0,    0,  844,  844,    0,  844,  187,
  691,    0,    0,  102,    0,    0,  692,    0,  181,    0,
   96,    0,    0,    0,  844,  844,    0,  844,    0,    0,
  850,    0,   97,    0,    0,  845,    0,  845,  845,  845,
  181,  844,    0,  844,  844,  844,  181,    0,  844,    0,
    0,  844,  844,  845,  845,  845,  845,   98,  691,    0,
    0,  844,    0,  188,  692,    0,  691,    0,    0,  181,
    0,    0,  692,    0,    0,  844,    0,    0,  844,    0,
  691,    0,    0,    0,    0,    0,  692,  845,  181,    0,
    0,    0,    0,    0,  844,    0,  843,  181,  843,  843,
  844,    0,    0,  181,    0,  844,  843, 1457,  695,  693,
    0,  694,    0,  696,    0,    0,  844,  844,    0,  844,
    0,  181,  188,  843,    0,  844,   99, 1458,  695,  693,
    0,  694,    0,  696,    0,    0,  100,  843,    0,    0,
    0,    0,  843,  843,  177,  848,  843,    0,    0,    0,
    0,    0,    0,  843,    0,  843,  843,    0,  181,  843,
    0,  101,    0,  843,    0,  177,  843,  844,  843,    0,
  843,    0,    0,    0,    0,  843,  843,    0,  843,  102,
    0,    0,    0,    0,    0,    0,  848,  177,  848,  848,
  848,    0,    0,  177,  843,  843,    0,  843,    0,    0,
    0,    0,    0,    0,  848,  848,  848,  848,  187,    0,
    0,  843,    0,  843,  843,  843,  177,    0,  843,    0,
    0,  843,  843,    0, 1465,  695,  693,    0,  694,  187,
  696,  843,    0,    0,    0,  177,    0,    0,  848,    0,
    0,    0,    0,    0,  177,  843,    0,    0,  843,    0,
  177,  187,    0,    0,    0,    0,  845,  187,  845,  845,
    0,    0,    0,    0,  843,    0,  845,    0,  177,    0,
  843,    0,    0,    0,    0,  843,    0,    0,    0,    0,
  187,    0,    0,  845,    0,    0,  843,  843,    0,  843,
    0,    0,    0,    0,    0,  843,    0,  845,    0,  187,
    0,    0,  845,  845,  624,  177,  845,  432,  187,  433,
    0,    0,    0,  845,  187,  845,  845,  666,    0,  845,
    0,  188,  435,  845,    0,    0,  845,  259,  845,    0,
  845,    0,  187,    0,    0,  845,  845,  843,  845,    0,
    0,    0,  188,    0,    0,    0,  691,    0,    0,    0,
    0,    0,  692,    0,  845,  845,    0,  845,  666,    0,
    0,  666,    0,    0,  188,    0,  691,    0,    0,  187,
  188,  845,  692,  845,  845,  845,  666,    0,  845,    0,
    0,  845,  845,    0,    0,    0,    0,    0,    0,    0,
    0,  845,    0,  188,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  845,    0,  848,  845,  848,
  848,   59,  188,    0,    0,    0,    0,  848,    0,    0,
    0,  188,   60,    0,  845,    0,    0,  188,    0,   61,
  845,    0,  547,    0,  848,  845,    0,    0,    0,    0,
    0,    0,   62,    0,    0,  188,  845,  845,  848,  845,
    0,    0,    0,  848,  848,  845,    0,  848,    0,    0,
    0,    0,    0,  691,  848,    0,  848,  848,    0,  692,
  848,    0,    0,  547,  848,    0,  547,  848,    0,  848,
    0,  848,  188,    0,    0,    0,  848,  848,    0,  848,
    0,  547,    0,  547,  664,    0,    0,  845,    0,    0,
    0,    0,    0,   63,    0,  848,  848,    0,  848,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   64,
    0,    0,  848,  547,  848,  848,  848,    0,    0,  848,
    0,  406,  848,  848,    0,  664,   65,    0,  664,    0,
  407,    0,  848,    0,  624,    0,    0,  432,    0,  433,
    0,   66,    0,  664,  408,   67,  848,  617,   68,  848,
  361,  409,  435,    0,    0,    0,   69,  259,    0,    0,
    0,    0,    0,    0,    0,  848,    0,    0,    0,  618,
    0,  848,    0,    0,    0,   70,  848,    0,  410,    0,
    0,  411,    0,    0,    0,    0,    0,  848,  848,    0,
  848,    0,    0,    0,    0,    0,  848,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  412,
  413,    0,    0,    0,    0,    0,    0,    0,    0,  624,
    0,  414,  432,    0,  433,  415,  666,    0,    0,  666,
    0,    0,    0,  619,    0,    0,  666,  435,  848,  666,
    0,    0,  259,  666,    0,    0,    0,    0,  666,   81,
    0,  666,    0,    0,    0,    0,   82,    0,    0,    0,
   83,   84,    0,    0,    0,    0,    0,  666,    0,   85,
    0,    0,    0,    0,    0,    0,  620,    0,    0,    0,
  547,   86,  416,    0,    0,    0,    0,    0,  417,    0,
    0,  666,    0,    0,    0,  666,  547,    0,  418,    0,
  547,    0,  547,    0,  666,    0,    0,  547,    0,    0,
  547,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,  666,    0,  547,    0,    0,  419,    0,    0,  547,
    0,    0,  547,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,   87,    0,  666,    0,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
  666,  406,    0,   88,  621,  622,  623,    0,  666,    0,
  407,    0,    0,    0,    0,    0,    0,    0,   89,    0,
    0,    0,   90,    0,  408,    0,    0,    0,    0,    0,
  361,  409,    0,   91,    0,    0,    0,    0,    0,    0,
    0,  547,    0,  664,    0,    0,  664,    0,    0,  618,
    0,    0,   92,  664,    0,    0,  664,    0,  410,  531,
  664,  411,    0,  547,    0,  664,    0,    0,  664,    0,
  547,    0,    0,    0,  547,    0,    0,    0,    0,    0,
    0,  547,    0,  529,  664,    0,  406,    0,    0,  412,
  413,    0,    0,    0,    0,  407,    0,    0,    0,  547,
  531,  414,    0,  531,  547,  415,    0,    0,  664,  408,
    0,    0,  664,  619,    0,  361,  409,    0,  531,    0,
  531,  664,    0,    0,  529,    0,    0,  529,    0,  547,
    0,    0,    0,  547,  618,    0,    0,    0,  664,    0,
    0,    0,  529,  410,  529,    0,  411,    0,    0,    0,
  531,    0,  685,    0,    0,    0,  620,    0,    0,    0,
    0,  640,  416,  640,  640,  664,  640,    0,  417,    0,
    0,    0,    0,    0,  412,  413,    0,  664,  418,  640,
    0,    0,    0,    0,  640,    0,  414,    0,    0,    0,
  415,    0,  363,  364,  365,  102,  184,  185,  619,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,    0,    0,    0,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
    0,  620, 1428, 1429,  621,  622,  623,  416, 1433, 1434,
    0, 1436, 1437,  417,    0,    0,    0,    0,    0, 1441,
 1442, 1443, 1444,  418, 1445, 1446, 1447, 1448, 1449, 1450,
    0,    0,    0,    0,    0,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,    0,    0,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,    0,
  425,    0,    0,    0,    0,  426,  427,  531,  428,  429,
  430,    0,    0,    0,  431,    0,    0,    0,    0,  621,
  622,  623,    0,  531,    0,    0,    0,  531,    0,  531,
    0,  529,    0,    0,  531,    0,    0,  531,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  531,  529,    0,  529,    0,    0,  531,    0,  529,  531,
    0,  529,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    0,    0,  529,    0,    0,    0,  640,    0,
  529,    0,    0,    0,    0,    0,  638,  640,  638,  638,
    0,  638,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    1,    0,  638,    0,    0,  640,  640,  638,
  928,  695,  693,    0,  694,  640,  696,    0,    0,    0,
    0,    0,    0,    0,    2,    0,    0,    0,  531,  858,
  857,  859,    3,    0,    0,  640,    0,    0,  640,    4,
    0,    5,    0,    6,    0,    0,    0,    0,    7,    0,
  531,    0,  529,    0,    0,    8,    0,  531,    0,    0,
    0,  531,    0,    0,  640,    0,  640,  640,  531,    9,
  434,    0,    0,  432,  529,  433,    0,    0,  640,    0,
   10,  529,  640,    0,    0,    0,  531,    0,  435,    0,
    0,  531,  529,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  529,    0,    0,    0,    0,  529,  531,    0,    0,    0,
  531,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  640,
    0,    0,    0,    0,  529,  640,    0,   11,    0,    0,
    0,    0,    0,    0,   12,  640,    0,   13,    0,   14,
   15,    0,    0,    0,    0,    0,    0,    0,    0,  640,
  640,  640,  640,  640,  640,    0,    0,    0,    0,    0,
    0,    0,    0,  640,   16,    0,    0,    0,    0,    0,
    0,    0,  640,  640,  640,  640,  640,  640,  640,  640,
  640,    0,  640,    0,    0,    0,  638,  640,  640,    0,
  640,  640,  640,  638,    0,    0,  640,    0,    0,    0,
    0,  434,  638,    0,  432,    0,  433,    0,    0,    0,
    0,    0,    0,   17,    0,    0,  638,    0,    0,  435,
    0,  845,  638,  638,  259,    0,    0,    0,   18,  691,
  638,    0,    0,    0,    0,  692,    0,    0,  846,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  638,    0,    0,  638,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1070,    0,    0, 1071,  847,    0,    0,  406,    0,  638,
    0,  638,  638,  848,    0,  434,  407,    0,  432,    0,
  433,  849,    0,  638,    0,    0,    0,  638,    0,    0,
  408,    0,    0,  435,    0,    0,  361,  409,  259,    0,
  850,    0,  851,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  852,    0,  853,  854,
  855,    0,    0,    0,  410,    0,    0,  411,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  638,    0,    0,    0,    0,    0,
  638,    0,    0,    0,    0,  412,  413,    0,    0,    0,
  638,    0,    0,    0,    0,    0,    0,  414,    0,  856,
    0,  415,    0,    0,  638,  638,  638,  638,  638,  638,
    0,    0,    0,    0,    0,    0,    0,    0,  638,    0,
    0,    0,    0,    0,    0,    0,    0,  638,  638,  638,
  638,  638,  638,  638,  638,  638,    0,  638,    0,    0,
    0, 1080,  638,  638, 1071,  638,  638,  638,  406,    0,
    0,  638,    0,    0,    0,    0, 1072,  407,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
    0,  408,    0,    0,  418,    0,    0,  361,  409,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,  410,    0,    0,  411,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0, 1084,  426,  427, 1071,  428,
  429,  430,  406,    0,    0,  431,  412,  413,    0,    0,
  434,  407,    0,  432,    0,  433,    0,    0,  414,  221,
    0,    0,  415,  221,    0,  408,    0,    0,  435,    0,
    0,  361,  409,  259,    0,    0,    0,    0,  221,    0,
    0,    0,  221,    0,    0,  221,  695,  693,    0,  694,
    0,  696,    0,    0,    0,    0,    0,    0,    0,  410,
    0,    0,  411,    0,  858,  857,  859,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1072,    0,  416,
    0,    0,    0,    0,    0,  417,  221,    0,    0,    0,
  412,  413,  221,  221,  221,  418,  221,  221,  221,    0,
  221,    0,  414,    0,    0,    0,  415,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
  221,    0,  221,  419,    0,    0,    0,    0,    0,    0,
    0,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424, 1374,  425,    0,    0,    0,    0,  426,  427,    0,
  428,  429,  430,  221,    0,    0,  431,    0,   81,    0,
    0, 1072,    0,  416,    0,    0,    0,    0,    0,  417,
  221,    0,    6,    0,    0,    0,    0,    0,    0,  418,
    0,    0,    0, 1493, 1494,    0, 1495,    0,    0, 1496,
    0,    0,  221,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0, 1497,    0,    0,    0,  419,    0, 1498,
    0,    0,    0,    0,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,    0,  425,  221,  221,    0,
 1092,  426,  427, 1071,  428,  429,  430,  406,    0,    0,
  431,    0,    0,    0,  221,  434,  407,    0,  432,    0,
  433,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  408,    0,    0,  435,    0,    0,  361,  409,  259, 1499,
    0,    0,    0,    0,    0,    0,  845,    0,    0,    0,
    0,    0,    0,    0,  691,    0,    0,    0,   14,    0,
  692,    0,    0,  846,  410,    0,    0,  411,    0,    0,
    0,    0,    0, 1500,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0, 1622,    0, 1501,  412,  413,  434,    0,  847,
  432,  102,  433,    0,    0,    0,    0,  414,  848,    0,
    0,  415,    0,  561,    0,  435,  849,  562,    0,    0,
  259,    0,    0,    0,    0,  759,    0,    0,    0,    0,
 1502,    0,    0,    0,    0,  850,  563,  851,    0,  564,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1503,
    0,  852,    0,  853,  854,  855,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1072,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,    0,    0,    0,
  565,    0,    0,    0,  418,    0,  566,  567,  568,    0,
  569,  570,  571,    0,  572,    0,    0,    0,  363,  364,
  365,  102,  184,  185,  856,    0,    0,    0,    0,    0,
    0,    0,  419,    0,  573,    0,  574,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0, 1095,  426,  427, 1071,  428,
  429,  430,  406,    0,    0,  431,    0,  575,    0,    0,
  434,  407,    0,  432,    0,  433,    0,    0,    0,    0,
    0,    0,    0,    0,  576,  408,    0,    0,  435,    0,
    0,  361,  409,  259,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  577,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
    0,  102,  411,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1100,    0,    0,
 1071,  578,  579,    0,  406,    0,    0,    0,    0,    0,
  412,  413,  434,  407,    0,  432,    0,  433,  580,    0,
    0,    0,  414,    0,    0,    0,  415,  408,    0,    0,
  435,    0,    0,  361,  409,  259,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  410,    0,    0,  411,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1072,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,  412,  413,    0,    0,    0,    0,    0,  418,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0,    0,    0,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
    0,    0,    0,    0,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,    0,  425,    0,    0,    0,
 1103,  426,  427, 1071,  428,  429,  430,  406,    0,    0,
  431,    0,    0, 1072,    0,  416,  407,    0,    0,    0,
    0,  417,    0,    0,    0,    0,    0,    0,    0,    0,
  408,  418,    0,    0,    0,    0,  361,  409,    0,    0,
    0,    0,    0,    0,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,  410,    0,    0,  411,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
    0,    0, 1106,  426,  427, 1071,  428,  429,  430,  406,
    0,    0,  431,    0,    0,  412,  413,  434,  407,  463,
  432,    0,  433,    0,    0,    0,    0,  414,    0,    0,
    0,  415,  408,    0,    0,  435,    0,    0,  361,  409,
  259,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  410,    0,    0,  411,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1072,    0,  416,    0,
    0,    0,    0,    0,  417,    0,    0,  412,  413,    0,
    0,    0,    0,    0,  418,    0,    0,    0,    0,  414,
    0,    0,    0,  415,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,    0,    0,
    0,    0,  419,    0,    0,    0,    0,    0,    0,    0,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
    0,  425,    0,    0,    0,    0,  426,  427,    0,  428,
  429,  430,    0,    0,    0,  431,    0,    0, 1072,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  363,  364,  365,  102,  184,  185,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
    0,    0,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,    0,  425,    0,    0,    0,    0,  426,  427,
    0,  428,  429,  430,  406,  434,    0,  431,  432,    0,
  433,  317,    0,  407,    0,    0,  661,    0,    0,    0,
    0,    0,    0,  435,    0,    0,    0,  408,  259,    0,
    0,    0,    0,  361,  409,    0,    0,    0,    0,    0,
    0,    0,  317,    0,    0,    0,    0,    0,    0,  317,
    0,    0,    0,  317,  317,    0,  317,  661,    0,  317,
  661,  410,    0,    0,  411,    0,    0,    0,    0,    0,
    0,    0,    0,  317,    0,  661,    0,    0,    0,  317,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  362,    0,  412,  413,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  414,    0,    0,    0,  415,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1374,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  947,    0,
  947,  947,    0,  947,    0,    0,    0,    0,    0,  317,
    0,    0,    0,    0,    0,    0,  947,    0,    0,    6,
    0,  947,    0,    0,    0,    0,    0,    0,  317,    0,
 1493, 1494,    0, 1495,    0,  416, 1496,    0,    0,    0,
    0,  417,    0,  317,    0,    0,    0,    0,    0,    0,
 1497,  418,    0,  317,    0,    0, 1498,    0,    0,    0,
    0,    0,  317,    0,  317,  363,  364,  365,  102,  184,
  185,  317,    0,    0,    0,    0,    0,    0,    0,  419,
    0,    0,    0,    0,    0,    0,    0,    0,  366,  367,
  420,  421,  422,  368,  369,  423,  424,    0,  425,    0,
  317,    0,  406,  426,  427,    0,  428,  429,  430,    0,
    0,  407,  431,    0,    0,    0, 1499,    0,    0,  317,
    0,  640,    0,    0,  640,  408,  640,    0,    0,    0,
    0,  361,  409,    0,    0,   14,    0,    0,    0,  640,
    0,    0,    0,    0,  640,    0,    0,    0,    0,    0,
 1500,    0,    0,    0,    0,    0,    0,    0,    0,  410,
   16,    0,  411,    0,    0,    0,    0,    0,    0,    0,
    0, 1501,    0,    0,    0,    0,    0,    0,  102,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  362,    0,
  412,  413,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,  661,  415, 1502,  661,    0,
    0,    0,    0,    0,    0,  661,    0,    0,  661,    0,
    0,    0,  661,    0,    0,  947, 1503,  661,    0,    0,
  661,    0,    0,    0,  947,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  434,    0,  661,  432,  947,  433,
    0,    0,    0,  685,  947,  947,    0,    0,    0,    0,
    0,    0,  435,  416,    0,    0,    0,  259,    0,  417,
  661,    0,    0,    0,  661,    0,    0,    0,    0,  418,
    0,    0,  947,  661,    0,  947,    0,    0,    0,    0,
    0,    0,    0,  363,  364,  365,  102,  184,  185,    0,
  661,    0,    0,    0,    0,    0,    0,  419,    0,    0,
    0,  947,    0,  947,  947,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,  947,  425,  661,    0,  947,
    0,  426,  427,    0,  428,  429,  430,    0,    0,  661,
  431,  640,    0,    0,    0,    0,    0,    0,  640,    0,
    0,    0,    0,    0,    0,    0,    0,  640,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  640,    0,    0,    0,    0,    0,  640,  640,    0,
    0,    0,    0,    0,    0,  640,  947,    0,    0,    0,
    0,    0,  947,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  947,    0,    0,  640,    0,    0,  640,    0,
    0,    0,    0,    0,    0,    0,  947,  947,  947,  947,
  947,  947,    0,    0,    0,    0,    0,    0,    0,    0,
  947,    0,    0,    0,    0,    0,  640,  640,    0,  947,
  947,  947,  947,  947,  947,  947,  947,  947,  640,  947,
    0,    0,  640,    0,  947,  947,    0,  947,  947,  947,
    0,    0,    0,  947,    0,    0,    0,    0,    0,    0,
  434,  406,    0,  432,    0,  433,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,  435,    0,
    0,    0,    0,  259,  408,    0,    0,    0,    0,    0,
  361,  409,    0,    0,    0,    0,    0,    0,    0,  640,
    0,    0,    0,    0,    0,  640,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  640,    0,    0,  410,    0,
    0,  411,    0,    0,    0,    0,    0,    0,    0,  640,
  640,  640,  640,  640,  640,    0,    0,    0,    0,    0,
    0,    0,    0,  640,    0,    0,    0,    0,    0,  412,
  413,    0,  640,  640,  640,  640,  640,  640,  640,  640,
  640,  414,  640,    0,    0,  415,    0,  640,  640,    0,
  640,  640,  640,    0,    0,    0,  640,    0,    0,    0,
    0,    0,    0,  434,    0,    0,  432,    0,  433,    0,
    0,    0,    0,    0,    0,    0,    0,  459,    0,    0,
    0,  435,    0,    0,    0,    0,  259,  459,  459,  459,
  459,  459,  459,  459,  459,  459,  459,  459,  459,  459,
  459,  459,  416,    0,  459,  459,  459,  459,  417,  459,
    0,    0,  459,  459,    0,    0,    0,    0,  418,    0,
    0,    0,    0,    0,    0,    0,  677,    0,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,    0,    0,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,    0,  425,    0,  406,    0,    0,
  426,  427,    0,  428,  429,  430,  407,    0,    0,  431,
    0,    0,    0,    0,    0,    0,  434,  930,    0,  432,
  408,  433,    0,    0,    0,  831,  361,  409,    0,  562,
    0,    0,    0,    0,  435,    0,    0,    0,    0,  259,
    0,    0,    0,    0,  832,    0,    0,    0,  563,    0,
    0,  564,  143,    0,  410,    0,    0,  411,    0,    0,
    0,  144,  145,  146,  147,  148,  149,  150,  151,  152,
  153,  154,  155,  156,  157,  158,  159,    0,    0,  160,
  161,  162,  163,    0,  164,  412,  413,  165,  166,    0,
    0,    0,  565,    0,    0,    0,    0,  414,  566,  567,
  568,  415,  569,  570,  571,    0,  572,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  406,    0,    0,    0,    0,    0,  573,    0,  574,  407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  434,
    0,    0,  432,  408,  433,    0,    0,    0,  685,  361,
  409,    0,    0,    0,    0,    0,    0,  435,  416,  575,
    0,    0,  259,    0,  417,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  418,    0,  576,  410,    0,    0,
  411,    0,    0,    0,    0,    0,    0,    0,  363,  364,
  365,  102,  184,  185,    0,    0,    0,    0,  577,    0,
    0,    0,  419,    0,    0,    0,  362,    0,  412,  413,
    0,  366,  367,  420,  421,  422,  368,  369,  423,  424,
  414,  425,    0,    0,  415,    0,  426,  427,    0,  428,
  429,  430,    0,  578,  579,  431,    0,    0,    0,    0,
    0,    0,    0,  406,    0,    0,    0,    0,    0,    0,
  580,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  434,    0,    0,  432,  408,  433,    0,    0,
    0,    0,  361,  409,    0,    0,    0,    0,    0,    0,
  435,  416,    0,    0,    0,  259,    0,  417,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  418,    0,    0,
  410,    0,    0,  411,    0,    0,    0,    0,    0,    0,
    0,  363,  364,  365,  102,  184,  185,    0,    0,    0,
    0,    0,    0,    0,    0,  419,    0,    0,    0,    0,
    0,  412,  413,    0,  366,  367,  420,  421,  422,  368,
  369,  423,  424,  414,  425,    0,    0,  415,    0,  426,
  427,    0,  428,  429,  430,    0,    0,    0,  431,    0,
    0,    0,    0,    0,    0,    0,  406,    0,    0,    0,
    0,    0,    0,    0,    0,  407,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  434,    0,    0,  432,  408,
  433,    0,    0,    0,    0,  361,  409,    0,    0,    0,
    0,    0,    0,  435,  416,    0,    0,    0,  259,    0,
  417,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  418,    0,    0,  410,    0,    0,  411,    0,    0,    0,
    0,    0,    0,    0,  363,  364,  365,  102,  184,  185,
    0,    0,    0,    0,    0,    0,    0,    0,  419,    0,
    0,    0,    0,    0,  412,  413,    0,  366,  367,  420,
  421,  422,  368,  369,  423,  424,  414,  425,    0,    0,
  415,    0,  426,  427,    0,  428,  429,  430,    0,    0,
    0,  431,    0,    0,    0,    0,    0,    0,    0,  406,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  408,    0,    0,    0,    0,    0,  361,  409,
    0,    0,    0,    0,    0,    0,    0,  416,    0,    0,
    0,    0,    0,  417,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  418,    0,    0,  410,    0,    0,  411,
    0,    0,    0,    0, 1098,    0,    0,  363,  364,  365,
  102,  184,  185,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,    0,  412,  413,    0,
  366,  367,  420,  421,  422,  368,  369,  423,  424,  414,
  425,    0,    0,  415,    0,  426,  427,    0,  428,  429,
  430,    0,    0,    0,  431,    0,    0,    0,    0,    0,
    0,  434,  406,    0,  432,    0,  433,    0,    0,    0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,  435,
 1661,    0,    0,    0,  259,  408,    0,    0,    0,    0,
 1278,  361,  409,    0,    0,    0,    0,    0,    0,    0,
  416,    0,    0,    0,    0,    0,  417,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  418,    0,    0,  410,
    0,    0,  411,    0,    0,    0,    0,    0,    0,    0,
  363,  364,  365,  102,  184,  185,    0,    0,    0,    0,
    0,    0,    0,    0,  419,    0,    0,    0,    0,    0,
  412,  413,    0,  366,  367,  420,  421,  422,  368,  369,
  423,  424,  414,  425,    0,    0,  415,    0,  426,  427,
  561,  428,  429,  430,  562,    0,    0,  431,    0,    0,
    0,    0,    0,    0, 1705,    0,    0,  432,    0,  433,
    0,    0,    0,  563,    0,    0,  564,    0,    0,    0,
    0,    0,  435,    0,    0,    0,    0,  259,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  416,    0,    0,    0,    0,    0,  417,
    0,    0,    0,    0,    0,    0,    0,  565,    0,  418,
    0,    0,    0,  566,  567,  568,    0,  569,  570,  571,
 1291,  572,    0,  363,  364,  365,  102,  184,  185,    0,
    0,    0,    0,    0,    0,    0,    0,  419,    0,    0,
    0,  573,    0,  574,    0,    0,  366,  367,  420,  421,
  422,  368,  369,  423,  424,    0,  425,    0,  406,    0,
    0,  426,  427,    0,  428,  429,  430,  407,    0,    0,
  431,    0,    0,    0,  575,    0,    0,  434,    0,    0,
  432,  408,  433,    0,    0,    0,    0,  361,  409,    0,
    0,  576,    0,  221,    0,  435,    0,  221,    0,    0,
  259,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,  577,    0,  410,  221,    0,  411,  221,
    0,    0,    0,    0,    0,    0,    0,    0,  102,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  412,  413,  578,  579,
    0,    0,    0,    0,    0,    0,    0,    0,  414,    0,
  221,    0,  415,    0,    0,  580,  221,  221,  221,    0,
  221,  221,  221,    0,  221,    0,    0,    0,    0,    0,
    0,  406,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,  221,    0,  221,    0,    0,    0,
  947,    0,    0,  947,  408,  947,    0,    0,    0,    0,
  361,  409,    0,    0,    0,    0,    0,    0,  947,  416,
    0,    0,    0,  947,    0,  417,    0,  221,    0,    0,
    0,    0,    0,    0,    0,  418,    0,    0,  410,    0,
    0,  411,    0,    0,  221,    0,    0,    0,    0,  363,
  364,  365,  102,  184,  185,    0,    0,    0,    0,    0,
    0,    0,    0,  419,    0,    0,  221,  362,    0,  412,
  413,    0,  366,  367,  420,  421,  422,  368,  369,  423,
  424,  414,  425,    0,    0,  415,    0,  426,  427,    0,
  428,  429,  430,    0,    0,  816,  431,    0,    0,    0,
    0,  221,  221,    0,  406,    0,    0,    0,    0,    0,
    0,    0,    0,  407,    0,    0,    0,    0,  221,    0,
    0,    0,    0,    0,    0,    0,    0,  408,    0,    0,
    0,    0,    0,  361,  409,    0,  816,    0,    0,  816,
    0,    0,  416,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,  816,    0,    0,    0,  418,    0,
    0,  410,    0,    0,  411,    0,    0,    0,    0,    0,
    0,    0,  363,  364,  365,  102,  184,  185,    0,    0,
    0,    0,    0,    0,    0,    0,  419,    0,    0,    0,
    0,    0,  412,  413,    0,  366,  367,  420,  421,  422,
  368,  369,  423,  424,  414,  425,    0,    0,  415,    0,
  426,  427,    0,  428,  429,  430,    0,    0,    0,  431,
    0,    0,    0,  785,    0,    0,    0,  947,    0,    0,
    0,    0,    0,    0,  786,    0,  947,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  947,    0,    0,    0,    0,    0,  947,  947,    0,    0,
    0,    0,    0,    0,  785,  416,    0,  785,    0,    0,
    0,  417,    0,    0,    0,  786,    0,    0,  786,    0,
    0,  418,  785,    0,  947,    0,    0,  947,    0,    0,
    0,    0,    0,  786,    0,  363,  364,  365,  102,  184,
  185,    0,    0,    0,    0,    0,  810,    0,    0,  419,
    0,    0,    0,    0,    0,  947,  947,  789,  366,  367,
  420,  421,  422,  368,  369,  423,  424,  947,  425,    0,
    0,  947,    0,  426,  427,    0,  428,  429,  430,    0,
    0,    0,  431,    0,    0,    0,    0,  810,    0,    0,
  810,    0,    0,    0,    0,    0,    0,  816,  789,    0,
    0,  789,    0,    0,    0,  810,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  789,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  947,    0,
    0,    0,    0,    0,  947,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  947,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  816,    0,    0,  816,  947,  947,
  947,  947,  947,  947,  816,    0,    0,  816,    0,    0,
    0,  816,  947,    0,    0,    0,  816,    0,    0,  816,
    0,  947,  947,  947,  947,  947,  947,  947,  947,  947,
    0,  947,    0,    0,  787,  816,  947,  947,    0,  947,
  947,  947,    0,    0,    0,  947,    0,    0,    0,    0,
    0,    0,    0,    0,  788,    0,    0,    0,    0,  816,
    0,    0,  816,  816,    0,  785,    0,    0,    0,    0,
    0,    0,  816,    0,    0,  787,  786,    0,  787,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  816,
    0,    0,    0,  787,    0,  788,    0,    0,  788,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  816,    0,  788,    0,    0,  816,    0,    0,    0,
    0,    0,  785,    0,    0,  785,    0,    0,  816,    0,
  816,    0,  785,  786,    0,  785,  786,  818,    0,  785,
    0,    0,    0,  786,  785,    0,  786,  785,  810,    0,
  786,    0,    0,    0,    0,  786,    0,    0,  786,  789,
    0,    0,    0,  785,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  786,    0,    0,    0,  818,    0,
    0,  818,    0,    0,    0,    0,    0,  785,    0,    0,
  785,  785,    0,    0,    0,    0,  818,    0,  786,    0,
  785,  786,  786,    0,    0,  810,    0,    0,  810,    0,
    0,  786,    0,    0,    0,  810,  789,  785,  810,  789,
  782,    0,  810,    0,    0,    0,  789,  810,  786,  789,
  810,  783,    0,  789,    0,    0,    0,    0,  789,  785,
    0,  789,    0,    0,  785,    0,  810,    0,    0,    0,
  786,    0,    0,    0,    0,  786,  785,  789,  785,    0,
    0,  782,    0,    0,  782,  784,    0,  786,    0,  786,
  810,    0,  783,  810,  810,  783,    0,    0,    0,  782,
    0,  789,    0,  810,  789,  789,    0,    0,    0,    0,
  783,    0,    0,    0,  789,    0,  787,    0,    0,    0,
  810,    0,    0,    0,    0,    0,  784,    0,    0,  784,
    0,  789,    0,    0,    0,    0,  788,    0,    0,    0,
    0,    0,  810,    0,  784,    0,    0,  810,    0,    0,
    0,    0,    0,  789,    0,    0,    0,    0,  789,  810,
    0,  810,    0,    0,    0,    0,    0,    0,    0,    0,
  789,    0,  789,  787,    0,    0,  787,    0,    0,    0,
    0,  817,    0,  787,    0,    0,  787,    0,    0,    0,
  787,    0,    0,  788,    0,  787,  788,    0,  787,    0,
    0,    0,    0,  788,    0,    0,  788,    0,    0,    0,
  788,    0,    0,  811,  787,  788,    0,    0,  788,  818,
    0,    0,  817,    0,    0,  817,    0,    0,    0,    0,
    0,    0,    0,    0,  788,    0,    0,    0,  787,    0,
  817,  787,  787,    0,    0,    0,    0,    0,    0,    0,
    0,  787,    0,    0,  811,    0,    0,  811,  788,    0,
    0,  788,  788,    0,    0,    0,    0,    0,  787,    0,
    0,  788,  811,    0,    0,    0,  818,  819,    0,  818,
    0,    0,    0,    0,    0,    0,  818,    0,  788,  818,
  787,    0,    0,  818,    0,  787,    0,    0,  818,    0,
    0,  818,  782,    0,    0,    0,    0,  787,    0,  787,
  788,    0,    0,  783,    0,  788,    0,  818,  819,  820,
    0,  819,    0,    0,    0,    0,    0,  788,    0,  788,
    0,    0,    0,    0,    0,    0,  819,    0,    0,    0,
    0,  818,    0,    0,  818,  818,    0,  784,    0,    0,
    0,    0,    0,    0,  818,    0,    0,    0,    0,  782,
  820,  808,  782,  820,    0,    0,    0,    0,    0,  782,
  783,  818,  782,  783,    0,    0,  782,    0,  820,    0,
  783,  782,    0,  783,  782,    0,    0,  783,    0,    0,
    0,    0,  783,  818,    0,  783,    0,    0,  818,    0,
  782,    0,  808,    0,  784,  808,    0,  784,    0,    0,
  818,  783,  818,    0,  784,    0,    0,  784,    0,    0,
  808,  784,    0,    0,  782,    0,  784,  782,  782,  784,
    0,    0,    0,    0,    0,  783,    0,  782,  783,  783,
    0,    0,    0,  817,    0,  784,    0,    0,  783,    0,
    0,    0,    0,    0,  782,    0,    0,  812,    0,    0,
    0,    0,    0,    0,    0,  783,    0,    0,    0,  784,
    0,    0,  784,  784,    0,  811,  782,    0,    0,    0,
    0,  782,  784,    0,    0,    0,    0,  783,    0,  821,
    0,    0,  783,  782,    0,  782,    0,    0,  812,  784,
  817,  812,    0,  817,  783,    0,  783,    0,    0,    0,
  817,    0,    0,  817,    0,    0,  812,  817,    0,    0,
    0,  784,  817,    0,    0,  817,  784,    0,    0,    0,
  821,    0,  811,  821,    0,  811,    0,    0,  784,  819,
  784,  817,  811,    0,    0,  811,    0,    0,  821,  811,
    0,    0,    0,    0,  811,    0,    0,  811,    0,    0,
    0,    0,    0,    0,    0,  817,    0,    0,  817,  817,
    0,    0,    0,  811,    0,    0,    0,    0,  817,    0,
    0,  820,    0,    0,    0,  809,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  817,  819,  811,    0,  819,
  811,  811,    0,    0,    0,    0,  819,    0,    0,  819,
  811,    0,    0,  819,    0,    0,    0,  817,  819,    0,
    0,  819,  817,  808,    0,    0,  809,  811,    0,  809,
    0,    0,    0,    0,  817,    0,  817,  819,  820,    0,
    0,  820,    0,    0,  809,    0,    0,    0,  820,  811,
    0,  820,    0,    0,  811,  820,    0,    0,    0,    0,
  820,  819,    0,  820,  819,  819,  811,    0,  811,    0,
    0,    0,    0,    0,  819,    0,  813,    0,    0,  820,
  808,    0,    0,  808,    0,    0,    0,  767,    0,    0,
  808,  819,    0,  808,    0,    0,    0,  808,    0,    0,
    0,    0,  808,  820,    0,  808,  820,  820,    0,    0,
    0,    0,    0,  819,    0,    0,  820,  813,  819,  812,
  813,  808,    0,    0,    0,    0,    0,    0,  767,    0,
  819,  767,  819,  820,    0,  813,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  808,  767,    0,  808,  808,
    0,  821,    0,    0,    0,  820,    0,    0,  808,    0,
  820,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  820,    0,  820,  808,  812,    0,    0,  812,
    0,    0,    0,    0,    0,    0,  812,    0,    0,  812,
    0,    0,    0,  812,    0,    0,    0,  808,  812,    0,
    0,  812,  808,    0,    0,    0,    0,    0,  821,    0,
    0,  821,    0,    0,  808,    0,  808,  812,  821,    0,
    0,  821,    0,    0,    0,  821,    0,    0,    0,    0,
  821,    0,    0,  821,    0,    0,    0,    0,    0,    0,
    0,  812,    0,    0,  812,  812,    0,  809,    0,  821,
    0,    0,    0,    0,  812,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  812,    0,  821,    0,    0,  821,  821,    0,    0,
    0,    0,    0,    0,    0,    0,  821,    0,    0,    0,
    0,    0,    0,  812,    0,    0,    0,    0,  812,    0,
    0,    0,    0,  821,  809,    0,    0,  809,    0,    0,
  812,    0,  812,    0,  809,    0,    0,  809,    0,    0,
    0,  809,    0,    0,    0,  821,  809,    0,    0,  809,
  821,    0,    0,    0,    0,    0,    0,    0,  813,    0,
    0,    0,  821,    0,  821,  809,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  809,
    0,    0,  809,  809,    0,    0,    0,    0,    0,    0,
    0,    0,  809,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  813,    0,    0,  813,  809,
    0,    0,    0,    0,    0,  813,  767,    0,  813,  767,
    0,    0,  813,    0,    0,    0,  767,  813,  561,  767,
  813,  809,  562,  767,    0,    0,  809,    0,  767,    0,
    0,  767,    0,    0,    0,    0,  813,    0,  809,    0,
  809,  563,    0,    0,  564,    0,    0,  767,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  813,    0,    0,  813,  813,    0,    0,    0,    0,    0,
    0,  767,    0,  813,  767,  767,    0,    0,    0,    0,
    0,    0,  134,    0,  767,  565,  134,    0,    0,    0,
  813,  566,  567,  568,    0,  569,  570,  571,    0,  572,
    0,  767,    0,    0,    0,  134,    0,    0,  134,    0,
    0,    0,  813,    0,    0,    0,    0,  813,    0,  573,
    0,  574,    0,  767,    0,    0,    0,    0,  767,  813,
    0,  813,    0,    0,    0,    0,    0,    0,    0,    0,
  767,    0,  767,    0,    0,    0,    0,    0,    0,  134,
    0,    0,  575,    0,    0,  134,  134,  134,    0,  134,
  134,  134,    0,  134,    0,    0,    0,    0,    0,  576,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  134,    0,  134,    0,    0,    0,    0,
    0,  577,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  134,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  578,  579,    0,    0,
    0,    0,    0,  134,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  580,    0,    0,    0,  538,    0,    0,
    0,    0,    0,    0,    0,  134,  144,  145,  146,  147,
  148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
  158,  159,    0,    0,  160,  161,  162,  163,    0,  164,
    0,    0,  165,  166,    0,    0,    0,    0,    0,    0,
  134,  134,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  134,
};
static short yycheck[] = {                                       9,
   52,    4,   12,   61,    4,   16,   16,  101,   18,   59,
    0,   60,  236,   60,  130,   54,  487,  331,  682,  339,
  120,  121,  120,  121,   82,  335,  339,   66,  730,  521,
  308,  530,  524,  470,   86,  310,   70,  736,  530,  992,
  699,   51,   52,   53,   54,   55,  999,  999,   91,  394,
   60,   61,   62,  731,   64, 1170,   66,   67,   92,   69,
   70, 1213,  111,  722,  113,   85,  287,  668,   78,   80,
   80,   81,   82,  112,   84,   85,   86,   87,   89,   89,
   90,   91,   92,   65,   94,  885,  992,  662,  613,   67,
  236,  134,  356,  999,  133,  619,  344,  992,   53, 1000,
  624,  111,  112,  113,  999,  125,   88,  658,    0,   64,
 1011,    0,   90,  729,  208,  125,    0,    0,   40,  169,
  130,  267,  132,  133,  134,  726,  992,  927,   45, 1030,
   40,   45,   87,  999,  685,  303,   40,   44,  236,   94,
  756,   45,    0,  606,  194,   40,    0,  371,   58,  765,
 1042,  244,   41,  677,  794,   44,   59,  698,   41,   45,
  253,   44,   42,    0,   41,  825,   41,   59,    0,   44,
   59,  287,   59,   44,    0,   59,   59,  132,   40,   44,
   40,  400,   41, 1047,  733,    0,  274,   42,    0,  238,
  500,   40,    0,  507,  508,  509,  510,   44,   44,   44,
  294,   59,  818,  242,   41,   59,   44,   44,  218,    0,
  530,   44,  251,  944,   44,   41,  226,  530,   44,  271,
  236,    0,   59,   44,  260,  371,   41,   59,  238,   44,
   58,  702,  242,   59,  244,  245,  275,  264,  287,   59,
 1211,  251,  490,  253,   59,   61,  556,   59, 1048,   63,
   41,   59, 1125,   44,  321,  533,  287, 1356,    0,    0,
  309,  271,   41, 1024,  662,  275, 1414,  316,   59,  318,
  356,  320,  284,  371,  323, 1129,   45,  287,  394,    0,
   59,  288,  332,  302,  334,  886, 1239, 1239,  262,   40,
  782, 1183,  277,  386,   63,  260,  456,  561,  348,  309,
  352,  251,   40,  878,  582, 1317,  316,   41,  318,  388,
  320,  350,   63,  323,   45,  890,  891,   59,   59,    0,
   41,   42,   43,   44,   45,   63,   47, 1228,   91,  301,
    0,  341,  342, 1239, 1346,   45,  860,  861,   59, 1118,
  350, 1120,  352,  382, 1239,  394,  273,    0,  264,  965,
  321,    0,    0,  356,  393,  371,    0,    0,  397,    0,
  257,  346,  441,  402,  342,  802,   41,    0,    0,   44,
  437,  406,  382, 1239,    0,  385,  386,    0,   59,  467,
 1532,  353,  418,  393,  394,    0,  341,  397,   41,   59,
  400,    0,  402,   41, 1010,  405,   44,   41,  922,  274,
   44,  257,    0,  260, 1258,    0,   59,  493, 1379,   41,
   59,   59,   44,  387,  341,   59,   59,  297,   59,  403,
  644, 1356,  457,  285,  443,  435,   59,   59,  440,    0,
  385,  462,  463,   59,  461,  310,   59,  329,  335,  265,
  332, 1172,  319,   41,   59,  400,  665,  304,  548,   41,
   59,  310,  402,  418,  461,  505,  321,    0,    0,  469,
  305,   59,  436,  278,   59,  280,  744,  991,    0,  747,
 1371,   41,  319,  288, 1375,  321, 1377,  303, 1630,  335,
  878,  530,  319,  761, 1245,  301, 1126,  278,   59,  280,
 1638, 1190,  890,  319, 1593,  384,  412,  288,  644, 1163,
 1392,  551,  723, 1052,  341,  397, 1170,  319,  411,   41,
  788, 1412,   44,  384,  277,  341,   59,   59, 1410,  529,
  530,   41,  668,  264,  411,  535,  448,   59,  432,  341,
  437, 1683, 1057,  341, 1075,  395, 1214,  412,  883,  397,
  319, 1320,  459,    0,  265,  459,  644, 1411, 1663,  441,
 1356,  461,  602,  603,  829,  459,  605,  703,  561,  280,
  653, 1136,  341,  455,  379,  286,  461,  549,  816,  384,
 1722,  455,  486,  459,  838,  743,  388,  258, 1041, 1452,
  726,  461,  303,  441, 1485,  397,  401,  441,  379, 1490,
 1705, 1251,  467,  408, 1234,  605,  645,  455,  319,  461,
    0,  455,   59,   41,  614,  437,  461,  723,  708,  388,
  401,  614,  461,  502,  614, 1356,  264,  408,  397,  502,
  341,  459,  455,  264, 1355,  455,  307,  442,  644,  441,
 1430,   44,  453,  461, 1250,  645, 1537, 1649,  648,  739,
  740,  461,    0,  653,    0,  459,  278,  455,  280,  264,
  670,  442,  662, 1554,  388,  665,  288,  398, 1593,   59,
  670,   42,  441,  432, 1425,  979,  503, 1366,  410,  264,
  262,  341, 1196,  321,  723,  319,  455,  503,  717,  728,
  719,  691,    0,  768,  733,  735,  736,  502,  332,  699,
  459,  701, 1216,  319,  704,    0,  329,  341, 1356,  332,
  349,   59,    0,   59,  753, 1658, 1658,  717,  459,  719,
  665,  502,  722,  723,    0,  341,  258, 1318,  728,  997,
  730,  459,  432,  733,   40,  368,  349, 1278,  459, 1043,
  461, 1009,  781,  782,    0,  388, 1637,  412,  419,  268,
  886,   59,  262,  753,  388, 1646,  278,  379,  280,  459,
    0, 1029, 1658,  397,   59,   41,  288,  398,   44,  368,
  742,   59,  388, 1658,  397,  307,  288,  883,  992,  401,
  341,  781,  782,   59,  867,  999,  408,  870,  817,  378,
 1055,    0,  503,    0,  787,  455, 1687, 1593,  827,  332,
  388,   41, 1658,   59,  273,  387,  799,  441,  780,  397,
 1237,  199, 1466,  398,    0,  453, 1243,  817,  441,   59,
  442,  455,   41,  461,  302,  441,    0,  827,  388,  284,
  453,   42,  455,    0, 1526, 1324,   47,  397,  871,  839,
  812,  813,  814, 1357,  883,  838,   41,  366,  872,   41,
   59,  357,   59,    0,    0,    0,  992,  379,   45,    0,
  449,  264, 1593,  999,  397, 1261,    0,  867,  868,  869,
  870,  871,  872,   59, 1139,   45,   16,  387,  878,  401,
  502, 1378, 1640,  883,  494,   59,  408,  419, 1619,  260,
  890,  891,   59,   63,  504,    0,  343,  258,   41, 1591,
   41,  869,  290, 1400,  332,  140,   40,   41,  441,  278,
   44,  378,   59,   59,   59,  262,  264,   12,   59,  262,
  442, 1206,  455,  868,  430,   59,  461,   61, 1003, 1004,
  385,  378, 1690,  304, 1665,   12,   41, 1027, 1153,   44,
   80, 1156,    0, 1592,  944, 1593,  307,   44,  329,  461,
  263,  320,  419,  343,   59, 1255,   61,   91, 1226,  406,
  388,   41,  409,  269,   44,  443,   46, 1252,   59,  397,
  278,    0,  280,  319,  265, 1733,  264, 1052,  425,   59,
  502,   61,   40,   41, 1715,  440,   44,   41,  378, 1257,
   44,  280, 1453, 1129,  326,  341,   93,  286,    0,  466,
  313,   59,  457,   61,  451,   59, 1045, 1468,  264, 1663,
 1492,  324,  303, 1052, 1324,  349,  406, 1748,    0,  409,
 1020, 1324, 1418,  118,  344, 1239, 1367,    0, 1021,  302,
   59, 1021,  401,   91, 1034,  425, 1124, 1125, 1126,   41,
  387,  118, 1773,  262,  387, 1045,  322,  338,  343,  369,
  398, 1705, 1052,  329,   41,  264,  332,   59,  419, 1363,
  336,  451,  297,  444, 1036, 1171,  339,  262,  344,  280,
  262,   44, 1413,  442, 1359,  286, 1590,   59,  264,  319,
  461,  339, 1054,  378,  360,  259,   59, 1585, 1060,  329,
  264, 1589,  332,  436,  410, 1556,  270, 1611,  261,  431,
 1614,  341, 1192, 1239, 1194,    0,  438,  427,  384,  296,
  398,  406,  319, 1381,  409,  428,    0,  264, 1118,  262,
 1120,  397, 1258, 1338,  258,  332,  296, 1342,  462,  463,
  425, 1124, 1125,  278,  341,  280, 1136,  324,  414,  527,
  274,  461,  398,  277,  278,    0,  280, 1645,  388,  260,
 1190,  285, 1420,  258,  288,  320,  451,  397,    0,    0,
  308,  309,    0, 1253,   59,  441,  353,  301,  387,  274,
  443, 1171, 1172,  307,  341,   59,  310,  453,    0,  455,
 1152,  388, 1318,  353,  264,  443,   41,  260,  329,  398,
  397,  332,  387,  304,   44,  387,  301,  343,  459,    0,
  258,  441,  307,   41,   59,  310,   44,  398,  596,   59,
  406,    0,  398, 1512,  459,  455,  274,   59,   59,  277,
  278,   59,  280,   61,  398,  305,  402,  285, 1268,  425,
  288,  304, 1272, 1694,  441,  264,  401,   59, 1736, 1232,
   41,  428, 1232,  301,  387,  379,  437,  388, 1463,  307,
 1356,  398,  310,   91,  308,  309,  397,  444,   59, 1544,
  406,   42,   43,  409,   45, 1550,   47,  401, 1374, 1783,
   59,  458,  459,  460,  408,  462,  463,  442,  412,  425,
  471,    0,    0, 1551, 1665,  419,  323,    0,  458,  459,
  460,  265,  462,  463,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  437,  452,  451, 1619,  412,  442,  496,
  497,  481,  482,    0,  419,  323,  486,  487,  505,  279,
 1320,  379,    0,  308,  309,   44, 1366,  272,   41,  303,
  332,   44,  437,  467, 1373, 1374, 1373,  471,  349,   40,
   59,   59,  329,  401,  289,  332,   59,  358,   61,  322,
  408,   44, 1665, 1382,  412, 1355,  329,  411,  258,  332,
   40,  419,  467,  336, 1452,   45,  471, 1748,  384,  398,
  330,  344,   59, 1373, 1374,    0,  258,  384,   91,  437,
  262,   59, 1382,   63,  442,  267,  388,  360, 1650,  334,
 1652,  302, 1773,    0,  278,  397,  280, 1487,  452,    0,
   93,  388, 1715,    0,  364,   41, 1512,  307,   44,  467,
  397,  384,  403,  471,  296,    0,   41,    0,  363,   44,
  258, 1720,  382,   44,  397,  288,   42,   43,  339,   45,
 1692,   47, 1695, 1696,   59, 1748,  274,   44,  301,  277,
  278,  414,  280,   44, 1658, 1493,  419,  285,  343,    0,
  288, 1535,   59, 1752, 1753,  446,   41,   93,   59, 1452,
 1773,  357,   59,  301,  319,  322, 1459,  452,  441,  307,
  403, 1734,  310, 1512,   59,    0,   59,  332, 1777,  336,
  453,  262,  455,  378,    0,  430,  341, 1593,  218, 1642,
    0, 1790,  386, 1493, 1536,  379,  226,  408,  394,  280,
  396,  384, 1531,  360, 1766,  286,  439,  418,   59,    0,
    0,  406, 1512, 1619,  409,  322, 1496,  508,  319,  419,
 1560, 1784, 1658,  419, 1564,   41, 1526,  384,   44,  336,
  425, 1531,  443,  388,   59, 1688, 1536,  437,   44,    0,
  341,  379,  397,   59,  322,  258,  428, 1631,  444,   59,
  432,  433,  434,  360, 1555, 1555,  451,  414,  336, 1665,
  344,  274,    0,  401,    0,  278,  344,  280,   59,   59,
  408,  471,  285,    0,  412,  288,    0,  384,    0, 1732,
   41,  419,  360,   44,  384,  369,  441,   93,  301,    0,
    0, 1591, 1592,  406,  307,  343,    0,  310,   59,  437,
  455,  260,  437,   41,  442, 1605,   44,  414,  495, 1715,
  329, 1764,   41,  332, 1720,   44,  296,   41,  338, 1619,
   44,   59,  419,   59,   41,  344, 1674,   44,    0,  467,
  378,   41,   59,  471,   44,   59,  414,   59,  322, 1640,
 1640,  319, 1748,  427,  457,  304, 1752, 1753,   59,   59,
    0,  329,  336,    0,  332,   59, 1657, 1657,  406,    0,
 1605,  409,    0,  341,  280, 1665,  379, 1773,    0,   41,
  286, 1777,   44,  353, 1674, 1675,  360,  425,  397,  322,
    0, 1720,  379, 1676, 1790,  274, 1676,   59,  401, 1690,
 1690,    0,   40,  336,  319,  408, 1697, 1697,   44,  412,
  319,  344, 1744,  451,  401,  321,  419,    0,    0,   59,
  388,  408,   59, 1752, 1753, 1715,  341,  360,   59,  397,
 1720,   59,  441,   41,  437,    0,   44,   59,  329,  442,
  414,  332, 1733, 1733,  453,  425,  455,  260, 1777,   59,
 1741, 1741,  624,  344, 1744,  442,    0,   93, 1748,  317,
   59, 1790, 1752, 1753,  467,  462,   42,   43,  471,   45,
  302,   47,  278,  441,  280,   41,   59,   59,   44,  285,
 1770,  414,  288, 1773,  319,   93, 1776, 1777,  458,  459,
  460,  304,  462,  463,   59,  301,  668,    0,  403,  671,
 1790,  418,  258, 1793,  676,  261,  397,  339,  338,  681,
  682,  481,  482,    0,  264,   59,  486,  487,  690,    0,
  692,  693,  694,  695,  696,  459,  698,  278,  445,  280,
  435,  703, 1737, 1738,    0,    0,  337,   40,   41,   42,
   43,   44,   45,   46,   47,  450,  453,  338,  353,  338,
  441,  307,  486,  418,  726,  435,   59,   60,   61,   62,
  732,  260,  453,  279,  455,    0,    0,  279,  466,  403,
  468,    0,   59,  379,  379,   41,   41,  341,   59,   44,
  445,  461,   40,  444,  375,  301,  375, 1792,   91,  301,
   93,  319,  321,   59,   59,  401,   61,  388,  319,  388,
  461,  329,  408,  302,  332,  404,   41,   41,  403,   44,
   44,  355,  259,  341,  330,  314,  344,   41,  330,  319,
   44,  507,  421,  270,   59,   59,  384,  259,  379,  329,
   59,    0,  332,  349,  317,  318,  442,  349,  270,  384,
  339,  341,  358, 1118,  344, 1120,  358,  840,  364,  842,
  401,  437,  364,   42,   43,  443,   45,  408,   47,  362,
  388,   41,   42,   43,  451,   45,  382,   47,   41,  397,
  382,   44,  418,  845,  846,  847,  494,  495,  850,  851,
  852,  435,  854,  855,  856,  857,  858,  859,  388,   41,
   59,  442,   44,   41,  358,   41,   44,  397,   44,  462,
  463,   41,   42,   43,  280,   45,  412,   47,   41,  408,
  286,   44,   44,  441,  886,  459,  288,  461,  470,  418,
   41,  893,  894,   44,  896,  453,  898,   61,  472,  901,
  902,  903,  904,  905,  906,   41,   41,    0,   44,   44,
   41,  441,  287,   44,  443,  321,  918,  491,  920,   41,
  373,    0,   44,  453,   40,  499,  500,  501,  329,  262,
   41,  264,  265,   44,  462,  463,   41,   42,   43,  272,
   45,  264,   47,  458,  459,  460,   40,  280,   41,   42,
   43,   44,   45,  286,   47,   41,  289,   41,   44,  462,
   44,  462,   41,  258,  462,   44,   59,   60,   61,   62,
  303,  486,  487,  392,  393,  308,  309,   44,  437,  312,
   59,  321,  277,  278,   44,  280,  319,  403,  321,  322,
  285,   40,  325,  288,   40,   40,  329,   40,   40,  332,
   93,  334,  403,  336,   40,   40,  301,   40,  341,  342,
   40,  344,  307,  278,  278,  280,  280,   40,   40,   40,
  279,  285,   40,  319,  288,   91,   46,  360,  361,  321,
  363,   44,   44,  329,   61,  429,  332,  301,  439,   46,
   40,  264,  301,  444,  377,  341,  379,  380,  381,  450,
    0,  384,   44,  383,  387,  388,  349,   44,  278,  301,
  461,  280,  278,  264,  397,  437,  398,  286,  469,  398,
  280,  330,  471, 1075,    0,  459,  286,  277,  411,  448,
  279,  414,  448,   40,  379, 1087, 1088,  400, 1090, 1091,
  349,   41,  388,  312,   44,  350, 1098,  430,   40,  358,
   91,  397,  301,  436,   40,  364,  401,   40,  441,   59,
  280,   61,  319,  408,  379,  379,  286,  264,  384,  452,
  453,  412,  455,  382,  419,   44,  383, 1129,  461,   42,
   43,  330,   45,   59,   47,   40,  401,  401,  390,   40,
  437, 1143,  437,  408,  408,  441,   59,  442,   40, 1151,
  349, 1153,  318,  465, 1156,   41, 1158,  453,  357,  358,
 1162, 1163, 1164, 1165,  321,  364,  321,  452, 1170,  262,
  503,  264,  265,  453,  449,  280,  471,  442,  442,  272,
  459,  286,  264,  382,  455,   40,  277,  280,   40,   42,
   43,  274,   45,  286,   47,  394,  289,  396,  667,  278,
  669,  280,  301,  672,  673,  674,  345, 1209,  285,  288,
  303,  345,  387,  278,  387,  308,  309,    0,  387,  312,
  419,  419,  301,  417,  419,  301,  319,   40,  321,  322,
  357,  418,  325,   40,  274,  418,  329,   40,   44,  332,
  386,  334,  368,  336,   44,  444,  321,  264,  341,  342,
   41,  344,   42,   43,   44,   45, 1258,   47,   41,   42,
   43,   44,   45,   44,   47,  321,   44,  360,  361,  280,
  363,   40,   44, 1275,  329,  503,   59,   60,   61,   62,
   42,   43, 1284,   45,  377,   47,  379,  380,  381, 1291,
   44,  384,  506,  266,  387,  388,   42,   43,   44,   45,
  379,   47,   40,  300,  397,    0,  502,   40,  258,  264,
   93,   41,   42,   43,  423,   45, 1318,   47,  411,   41,
   41,  414,  401,   58,   41,   41,  264,  277,  278,  408,
  280,  455,  258,  441,  295,  285, 1338,  430,  288,  310,
 1342,  395,  310,  436,  493,  306,   41,   41,  441,   44,
   41,  301,  313,  279,    0,   40,   40,  307,  353,  452,
  453,    0,  455,  442,   59,   40,   40,  280,  461,  262,
   41,   42,   43,  286,   45,  301,   47,  338,   41,   42,
   43,  307,   45,   40,   47,   41,   42,   43,   40,   45,
   40,   47,   40,   40,  328,   41,   42,   43,   44,   45,
   40,   47,   41,   40,  330,   44,   40,   40,   40,    0,
  503,   40,   40,   59,   60,   61,   62,  850,   44,  852,
   59,  854,  855,  349,  857,  858,  859,  280,  436,  379,
  309,  357,  358,  286,   41,  389, 1438, 1439,  364,  274,
  389,  389,  403,  332,  344,  423,  391,   93,  392,  447,
   41,  401,  271,   44,  288,  408,  382,  387,  408,  312,
  387, 1463,  387,  387, 1466,  387,  387,  417,   59,  419,
   41,  379,   41,  278,  435,   40,  386,   41,  439,  262,
   41,  264,  265,   42,   43,   41,   45,  437,   47,  272,
  280,  437,  442,  419,   41,  418,  286, 1499,  309,  436,
  262,  397,  338,  286,  430,  280,  289,   41,   42,   43,
  339,   45,  443,   47,  443,  302,  339,  302,  280,   41,
  303,  471,   41,  353,  286,  308,  309,    0,  321,  312,
  386,   41,   42,   43,  280,   45,  319,   47,  321,  322,
  286,  372,  325,   41,  372,   41,  329,   41,   41,  332,
  280,  334,   41,  336,   41,   41,  286, 1559,  341,  342,
   41,  344,   41,   41,   41, 1567,   41,   41,   41,   42,
   43,   44,   45,   41,   47,   41,   41,  360,  361,  280,
  363,   44,   46,  278,   40,  280,   59,   60,   61,   62,
   59,  341,   59,  288,  377,   40,  379,  380,  381,   59,
   59,  384, 1604,  309,  387,  388, 1608,   59,   59,  280,
  341,   41,   59,   59,  397,  286,  262,  280,  264,  265,
   93,  353,  345,  286,  280,   41,  272,  369,  411,   40,
  286,  414,   42,   43,  341,   45,  452,   47,   44,  278,
  384,  280,   59,  289,   41,   42,   43,  430,   45,  288,
   47,   59,   41,  436,  344,  384,  384,  303,  441,   41,
   41, 1663,  308,  309,    0, 1667,  312,   44,  411,  452,
  453,   44,  455,  319,  305,  321,  322,  436,  461,  325,
  302,   40,  443,  329,  379,  388,  332,  278,  334,  280,
  336,   59,  294,  305,  464,  341,  342,  288,  344,   42,
   43,   41,   45, 1705,   47,   41,  401,   43,   44,   45,
  301,   44,  308,  408,  360,  361,   59,  363,   44,    0,
  503,  280,  338,   59,   60,   61,   62,  286,   40,  384,
  384,  377,  264,  379,  380,  381,  461,  278,  384,    0,
  379,  387,  388,  264,  459,  390,  280,  442,  321,  264,
  412,  397,  286,  471,  452,   40,  310,   93,  271,  459,
  264,  437,  401,  436,  344,  411,  344,  344,  414,  408,
  280,   41,   42,   43,  344,   45,  286,   47,  459,  461,
   41,  271,  309,   44,  430,   41,   40,   40,  379,  262,
  436,  264,  265,  309,   59,  441,   59,  309,   59,  272,
   41,   59,  305,  442,   59,   41,  452,  453,   59,  455,
  401,   44,  260,   41,    0,  461,  289,  408,    0,   41,
   42,   43,    0,   45,   12,   47,  253,   41,   42,   43,
  303,   45, 1121,   47,    0,  308,  309,  246,  245,  312,
 1057,   41,   42,   43,  653,   45,  319,   47,  321,  322,
  830,  442,  325,  609,  302,   84,  329,  503, 1055,  332,
 1052,  334, 1401,  336, 1044,  306,  314,  556,  341,  342,
  280,  344,  301, 1034,  336,   41,  286,   43,   44,   45,
  328,  982,  728,  280,  318, 1045,    0,  360,  361,  286,
  363,  339,  292,   59,   60,   61,   62,  318, 1602,  763,
 1379, 1681, 1685, 1381,  377,  535,  379,  380,  381, 1684,
 1724,  384,  753, 1207,  387,  388,  473,  474,  475,  476,
  477,  478,  479,  480,  397, 1776,  262,   93,  264,  265,
 1624, 1721,  371,  967, 1185, 1360,  272,  280,  411, 1640,
  323,  414,  311,  286,  537,   59,  304,  809,  405,   13,
    0, 1241,  595,  289,  801,  279,  476,  430,  472,  717,
  408,  732,  123,  436,  703,  944, 1593,  303,  441, 1483,
  418, 1741,  308,  309, 1697, 1793,  312,  704,  260,  452,
  453,  487,  455,  319,  661,  321,  322, 1089,  461,  325,
  887,  370,   -1,  329,    0,  443,  332,  258,  334, 1158,
  336,   -1,   -1,   -1,   -1,  341,  342,   -1,  344,   59,
  280,   -1,   -1,  461,   -1,   -1,  286,   -1,  279,   -1,
  302,   -1,   -1,   -1,  360,  361,   -1,  363,   -1,   -1,
  503,   -1,  314,   -1,   -1,   41,   -1,   43,   44,   45,
  301,  377,   -1,  379,  380,  381,  307,   -1,  384,   -1,
   -1,  387,  388,   59,   60,   61,   62,  339,  280,   -1,
   -1,  397,   -1,    0,  286,   -1,  280,   -1,   -1,  330,
   -1,   -1,  286,   -1,   -1,  411,   -1,   -1,  414,   -1,
  280,   -1,   -1,   -1,   -1,   -1,  286,   93,  349,   -1,
   -1,   -1,   -1,   -1,  430,   -1,  262,  358,  264,  265,
  436,   -1,   -1,  364,   -1,  441,  272,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,  452,  453,   -1,  455,
   -1,  382,   59,  289,   -1,  461,  408,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,  418,  303,   -1,   -1,
   -1,   -1,  308,  309,  258,    0,  312,   -1,   -1,   -1,
   -1,   -1,   -1,  319,   -1,  321,  322,   -1,  419,  325,
   -1,  443,   -1,  329,   -1,  279,  332,  503,  334,   -1,
  336,   -1,   -1,   -1,   -1,  341,  342,   -1,  344,  461,
   -1,   -1,   -1,   -1,   -1,   -1,   41,  301,   43,   44,
   45,   -1,   -1,  307,  360,  361,   -1,  363,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   60,   61,   62,  258,   -1,
   -1,  377,   -1,  379,  380,  381,  330,   -1,  384,   -1,
   -1,  387,  388,   -1,   41,   42,   43,   -1,   45,  279,
   47,  397,   -1,   -1,   -1,  349,   -1,   -1,   93,   -1,
   -1,   -1,   -1,   -1,  358,  411,   -1,   -1,  414,   -1,
  364,  301,   -1,   -1,   -1,   -1,  262,  307,  264,  265,
   -1,   -1,   -1,   -1,  430,   -1,  272,   -1,  382,   -1,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,
  330,   -1,   -1,  289,   -1,   -1,  452,  453,   -1,  455,
   -1,   -1,   -1,   -1,   -1,  461,   -1,  303,   -1,  349,
   -1,   -1,  308,  309,   40,  419,  312,   43,  358,   45,
   -1,   -1,   -1,  319,  364,  321,  322,    0,   -1,  325,
   -1,  258,   58,  329,   -1,   -1,  332,   63,  334,   -1,
  336,   -1,  382,   -1,   -1,  341,  342,  503,  344,   -1,
   -1,   -1,  279,   -1,   -1,   -1,  280,   -1,   -1,   -1,
   -1,   -1,  286,   -1,  360,  361,   -1,  363,   41,   -1,
   -1,   44,   -1,   -1,  301,   -1,  280,   -1,   -1,  419,
  307,  377,  286,  379,  380,  381,   59,   -1,  384,   -1,
   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  397,   -1,  330,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  411,   -1,  262,  414,  264,
  265,  295,  349,   -1,   -1,   -1,   -1,  272,   -1,   -1,
   -1,  358,  306,   -1,  430,   -1,   -1,  364,   -1,  313,
  436,   -1,    0,   -1,  289,  441,   -1,   -1,   -1,   -1,
   -1,   -1,  326,   -1,   -1,  382,  452,  453,  303,  455,
   -1,   -1,   -1,  308,  309,  461,   -1,  312,   -1,   -1,
   -1,   -1,   -1,  280,  319,   -1,  321,  322,   -1,  286,
  325,   -1,   -1,   41,  329,   -1,   44,  332,   -1,  334,
   -1,  336,  419,   -1,   -1,   -1,  341,  342,   -1,  344,
   -1,   59,   -1,   61,    0,   -1,   -1,  503,   -1,   -1,
   -1,   -1,   -1,  387,   -1,  360,  361,   -1,  363,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  403,
   -1,   -1,  377,   91,  379,  380,  381,   -1,   -1,  384,
   -1,  267,  387,  388,   -1,   41,  420,   -1,   44,   -1,
  276,   -1,  397,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  435,   -1,   59,  290,  439,  411,  293,  442,  414,
  296,  297,   58,   -1,   -1,   -1,  450,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  430,   -1,   -1,   -1,  315,
   -1,  436,   -1,   -1,   -1,  469,  441,   -1,  324,   -1,
   -1,  327,   -1,   -1,   -1,   -1,   -1,  452,  453,   -1,
  455,   -1,   -1,   -1,   -1,   -1,  461,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  355,
  356,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,  367,   43,   -1,   45,  371,  319,   -1,   -1,  322,
   -1,   -1,   -1,  379,   -1,   -1,  329,   58,  503,  332,
   -1,   -1,   63,  336,   -1,   -1,   -1,   -1,  341,  306,
   -1,  344,   -1,   -1,   -1,   -1,  313,   -1,   -1,   -1,
  317,  318,   -1,   -1,   -1,   -1,   -1,  360,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,  422,   -1,   -1,   -1,
  258,  338,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,  384,   -1,   -1,   -1,  388,  274,   -1,  444,   -1,
  278,   -1,  280,   -1,  397,   -1,   -1,  285,   -1,   -1,
  288,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,  414,   -1,  301,   -1,   -1,  472,   -1,   -1,  307,
   -1,   -1,  310,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,  403,   -1,  441,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
  453,  267,   -1,  420,  510,  511,  512,   -1,  461,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  439,   -1,  290,   -1,   -1,   -1,   -1,   -1,
  296,  297,   -1,  450,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  379,   -1,  319,   -1,   -1,  322,   -1,   -1,  315,
   -1,   -1,  469,  329,   -1,   -1,  332,   -1,  324,    0,
  336,  327,   -1,  401,   -1,  341,   -1,   -1,  344,   -1,
  408,   -1,   -1,   -1,  412,   -1,   -1,   -1,   -1,   -1,
   -1,  419,   -1,    0,  360,   -1,  267,   -1,   -1,  355,
  356,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,  437,
   41,  367,   -1,   44,  442,  371,   -1,   -1,  384,  290,
   -1,   -1,  388,  379,   -1,  296,  297,   -1,   59,   -1,
   61,  397,   -1,   -1,   41,   -1,   -1,   44,   -1,  467,
   -1,   -1,   -1,  471,  315,   -1,   -1,   -1,  414,   -1,
   -1,   -1,   59,  324,   61,   -1,  327,   -1,   -1,   -1,
   91,   -1,  418,   -1,   -1,   -1,  422,   -1,   -1,   -1,
   -1,   40,  428,   42,   43,  441,   45,   -1,  434,   -1,
   -1,   -1,   -1,   -1,  355,  356,   -1,  453,  444,   58,
   -1,   -1,   -1,   -1,   63,   -1,  367,   -1,   -1,   -1,
  371,   -1,  458,  459,  460,  461,  462,  463,  379,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,  422, 1276, 1277,  510,  511,  512,  428, 1282, 1283,
   -1, 1285, 1286,  434,   -1,   -1,   -1,   -1,   -1, 1293,
 1294, 1295, 1296,  444, 1298, 1299, 1300, 1301, 1302, 1303,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,   -1,
  491,   -1,   -1,   -1,   -1,  496,  497,  258,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,  510,
  511,  512,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
   -1,  258,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  301,  278,   -1,  280,   -1,   -1,  307,   -1,  285,  310,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  301,   -1,   -1,   -1,  267,   -1,
  307,   -1,   -1,   -1,   -1,   -1,   40,  276,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,  261,   -1,   58,   -1,   -1,  296,  297,   63,
   41,   42,   43,   -1,   45,  304,   47,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  283,   -1,   -1,   -1,  379,   60,
   61,   62,  291,   -1,   -1,  324,   -1,   -1,  327,  298,
   -1,  300,   -1,  302,   -1,   -1,   -1,   -1,  307,   -1,
  401,   -1,  379,   -1,   -1,  314,   -1,  408,   -1,   -1,
   -1,  412,   -1,   -1,  353,   -1,  355,  356,  419,  328,
   40,   -1,   -1,   43,  401,   45,   -1,   -1,  367,   -1,
  339,  408,  371,   -1,   -1,   -1,  437,   -1,   58,   -1,
   -1,  442,  419,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  437,   -1,   -1,   -1,   -1,  442,  467,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,  471,  434,   -1,  406,   -1,   -1,
   -1,   -1,   -1,   -1,  413,  444,   -1,  416,   -1,  418,
  419,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,   -1,  491,   -1,   -1,   -1,  260,  496,  497,   -1,
  499,  500,  501,  267,   -1,   -1,  505,   -1,   -1,   -1,
   -1,   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  492,   -1,   -1,  290,   -1,   -1,   58,
   -1,  272,  296,  297,   63,   -1,   -1,   -1,  507,  280,
  304,   -1,   -1,   -1,   -1,  286,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,   -1,  263,  325,   -1,   -1,  267,   -1,  353,
   -1,  355,  356,  334,   -1,   40,  276,   -1,   43,   -1,
   45,  342,   -1,  367,   -1,   -1,   -1,  371,   -1,   -1,
  290,   -1,   -1,   58,   -1,   -1,  296,  297,   63,   -1,
  361,   -1,  363,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  377,   -1,  379,  380,
  381,   -1,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,
  434,   -1,   -1,   -1,   -1,  355,  356,   -1,   -1,   -1,
  444,   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,  430,
   -1,  371,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,
   -1,  260,  496,  497,  263,  499,  500,  501,  267,   -1,
   -1,  505,   -1,   -1,   -1,   -1,  426,  276,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,  444,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,  499,
  500,  501,  267,   -1,   -1,  505,  355,  356,   -1,   -1,
   40,  276,   -1,   43,   -1,   45,   -1,   -1,  367,  273,
   -1,   -1,  371,  277,   -1,  290,   -1,   -1,   58,   -1,
   -1,  296,  297,   63,   -1,   -1,   -1,   -1,  292,   -1,
   -1,   -1,  296,   -1,   -1,  299,   42,   43,   -1,   45,
   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,   -1,  327,   -1,   60,   61,   62,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,  340,   -1,   -1,   -1,
  355,  356,  346,  347,  348,  444,  350,  351,  352,   -1,
  354,   -1,  367,   -1,   -1,   -1,  371,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
  374,   -1,  376,  472,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  271,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,
  499,  500,  501,  407,   -1,   -1,  505,   -1,  412,   -1,
   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
  424,   -1,  302,   -1,   -1,   -1,   -1,   -1,   -1,  444,
   -1,   -1,   -1,  313,  314,   -1,  316,   -1,   -1,  319,
   -1,   -1,  446,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,  333,   -1,   -1,   -1,  472,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,  481,  482,   -1,
  260,  496,  497,  263,  499,  500,  501,  267,   -1,   -1,
  505,   -1,   -1,   -1,  498,   40,  276,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,   58,   -1,   -1,  296,  297,   63,  399,
   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  280,   -1,   -1,   -1,  418,   -1,
  286,   -1,   -1,  289,  324,   -1,   -1,  327,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  355,  356,   40,   -1,  325,
   43,  461,   45,   -1,   -1,   -1,   -1,  367,  334,   -1,
   -1,  371,   -1,  273,   -1,   58,  342,  277,   -1,   -1,
   63,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
  490,   -1,   -1,   -1,   -1,  361,  296,  363,   -1,  299,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  509,
   -1,  377,   -1,  379,  380,  381,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
  340,   -1,   -1,   -1,  444,   -1,  346,  347,  348,   -1,
  350,  351,  352,   -1,  354,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,  430,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,  374,   -1,  376,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,  260,  496,  497,  263,  499,
  500,  501,  267,   -1,   -1,  505,   -1,  407,   -1,   -1,
   40,  276,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  424,  290,   -1,   -1,   58,   -1,
   -1,  296,  297,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  446,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,  461,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,
  263,  481,  482,   -1,  267,   -1,   -1,   -1,   -1,   -1,
  355,  356,   40,  276,   -1,   43,   -1,   45,  498,   -1,
   -1,   -1,  367,   -1,   -1,   -1,  371,  290,   -1,   -1,
   58,   -1,   -1,  296,  297,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  426,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,  444,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,   -1,   -1,   -1,
  260,  496,  497,  263,  499,  500,  501,  267,   -1,   -1,
  505,   -1,   -1,  426,   -1,  428,  276,   -1,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,  444,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
   -1,   -1,  260,  496,  497,  263,  499,  500,  501,  267,
   -1,   -1,  505,   -1,   -1,  355,  356,   40,  276,   42,
   43,   -1,   45,   -1,   -1,   -1,   -1,  367,   -1,   -1,
   -1,  371,  290,   -1,   -1,   58,   -1,   -1,  296,  297,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  426,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,  355,  356,   -1,
   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,  367,
   -1,   -1,   -1,  371,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,   -1,   -1,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,  426,   -1,
  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,   -1,  491,   -1,   -1,   -1,   -1,  496,  497,
   -1,  499,  500,  501,  267,   40,   -1,  505,   43,   -1,
   45,  271,   -1,  276,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,  290,   63,   -1,
   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  302,   -1,   -1,   -1,   -1,   -1,   -1,  309,
   -1,   -1,   -1,  313,  314,   -1,  316,   41,   -1,  319,
   44,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  333,   -1,   59,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  353,   -1,  355,  356,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  367,   -1,   -1,   -1,  371,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,  399,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  302,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,
  313,  314,   -1,  316,   -1,  428,  319,   -1,   -1,   -1,
   -1,  434,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
  333,  444,   -1,  443,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  458,  459,  460,  461,  462,
  463,  461,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,
  483,  484,  485,  486,  487,  488,  489,   -1,  491,   -1,
  490,   -1,  267,  496,  497,   -1,  499,  500,  501,   -1,
   -1,  276,  505,   -1,   -1,   -1,  399,   -1,   -1,  509,
   -1,   40,   -1,   -1,   43,  290,   45,   -1,   -1,   -1,
   -1,  296,  297,   -1,   -1,  418,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
  443,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,  461,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  353,   -1,
  355,  356,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  367,   -1,   -1,  319,  371,  490,  322,   -1,
   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,
   -1,   -1,  336,   -1,   -1,  267,  509,  341,   -1,   -1,
  344,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,  360,   43,  290,   45,
   -1,   -1,   -1,  418,  296,  297,   -1,   -1,   -1,   -1,
   -1,   -1,   58,  428,   -1,   -1,   -1,   63,   -1,  434,
  384,   -1,   -1,   -1,  388,   -1,   -1,   -1,   -1,  444,
   -1,   -1,  324,  397,   -1,  327,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,
  414,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,  353,   -1,  355,  356,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  367,  491,  441,   -1,  371,
   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,  453,
  505,  260,   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,   -1,   -1,   -1,   -1,  304,  428,   -1,   -1,   -1,
   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  444,   -1,   -1,  324,   -1,   -1,  327,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,  460,  461,
  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  472,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  367,  491,
   -1,   -1,  371,   -1,  496,  497,   -1,  499,  500,  501,
   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,   -1,   -1,
   40,  267,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,  290,   -1,   -1,   -1,   -1,   -1,
  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,
   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,  355,
  356,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  367,  491,   -1,   -1,  371,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  461,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,  485,  428,   -1,  488,  489,  490,  491,  434,  493,
   -1,   -1,  496,  497,   -1,   -1,   -1,   -1,  444,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,   -1,  491,   -1,  267,   -1,   -1,
  496,  497,   -1,  499,  500,  501,  276,   -1,   -1,  505,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,   43,
  290,   45,   -1,   -1,   -1,  273,  296,  297,   -1,  277,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,  292,   -1,   -1,   -1,  296,   -1,
   -1,  299,  461,   -1,  324,   -1,   -1,  327,   -1,   -1,
   -1,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,  485,   -1,   -1,  488,
  489,  490,  491,   -1,  493,  355,  356,  496,  497,   -1,
   -1,   -1,  340,   -1,   -1,   -1,   -1,  367,  346,  347,
  348,  371,  350,  351,  352,   -1,  354,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,  374,   -1,  376,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,  290,   45,   -1,   -1,   -1,  418,  296,
  297,   -1,   -1,   -1,   -1,   -1,   -1,   58,  428,  407,
   -1,   -1,   63,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,  424,  324,   -1,   -1,
  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  458,  459,
  460,  461,  462,  463,   -1,   -1,   -1,   -1,  446,   -1,
   -1,   -1,  472,   -1,   -1,   -1,  353,   -1,  355,  356,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  367,  491,   -1,   -1,  371,   -1,  496,  497,   -1,  499,
  500,  501,   -1,  481,  482,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  498,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,  290,   45,   -1,   -1,
   -1,   -1,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  428,   -1,   -1,   -1,   63,   -1,  434,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,
  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,
   -1,  355,  356,   -1,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  367,  491,   -1,   -1,  371,   -1,  496,
  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,  290,
   45,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  428,   -1,   -1,   -1,   63,   -1,
  434,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  444,   -1,   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  458,  459,  460,  461,  462,  463,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,
   -1,   -1,   -1,   -1,  355,  356,   -1,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  367,  491,   -1,   -1,
  371,   -1,  496,  497,   -1,  499,  500,  501,   -1,   -1,
   -1,  505,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,  296,  297,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  444,   -1,   -1,  324,   -1,   -1,  327,
   -1,   -1,   -1,   -1,  455,   -1,   -1,  458,  459,  460,
  461,  462,  463,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  472,   -1,   -1,   -1,   -1,   -1,  355,  356,   -1,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  367,
  491,   -1,   -1,  371,   -1,  496,  497,   -1,  499,  500,
  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,   -1,
   -1,   40,  267,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   59,   -1,   -1,   -1,   63,  290,   -1,   -1,   -1,   -1,
  418,  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,  324,
   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,   -1,   -1,
  355,  356,   -1,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  367,  491,   -1,   -1,  371,   -1,  496,  497,
  273,  499,  500,  501,  277,   -1,   -1,  505,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,  296,   -1,   -1,  299,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  340,   -1,  444,
   -1,   -1,   -1,  346,  347,  348,   -1,  350,  351,  352,
  455,  354,   -1,  458,  459,  460,  461,  462,  463,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,
   -1,  374,   -1,  376,   -1,   -1,  481,  482,  483,  484,
  485,  486,  487,  488,  489,   -1,  491,   -1,  267,   -1,
   -1,  496,  497,   -1,  499,  500,  501,  276,   -1,   -1,
  505,   -1,   -1,   -1,  407,   -1,   -1,   40,   -1,   -1,
   43,  290,   45,   -1,   -1,   -1,   -1,  296,  297,   -1,
   -1,  424,   -1,  273,   -1,   58,   -1,  277,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  292,  446,   -1,  324,  296,   -1,  327,  299,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  461,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  355,  356,  481,  482,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  367,   -1,
  340,   -1,  371,   -1,   -1,  498,  346,  347,  348,   -1,
  350,  351,  352,   -1,  354,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,  374,   -1,  376,   -1,   -1,   -1,
   40,   -1,   -1,   43,  290,   45,   -1,   -1,   -1,   -1,
  296,  297,   -1,   -1,   -1,   -1,   -1,   -1,   58,  428,
   -1,   -1,   -1,   63,   -1,  434,   -1,  407,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  444,   -1,   -1,  324,   -1,
   -1,  327,   -1,   -1,  424,   -1,   -1,   -1,   -1,  458,
  459,  460,  461,  462,  463,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  472,   -1,   -1,  446,  353,   -1,  355,
  356,   -1,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  367,  491,   -1,   -1,  371,   -1,  496,  497,   -1,
  499,  500,  501,   -1,   -1,    0,  505,   -1,   -1,   -1,
   -1,  481,  482,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,  498,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,
   -1,   -1,   -1,  296,  297,   -1,   41,   -1,   -1,   44,
   -1,   -1,  428,   -1,   -1,   -1,   -1,   -1,  434,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  444,   -1,
   -1,  324,   -1,   -1,  327,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  458,  459,  460,  461,  462,  463,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  472,   -1,   -1,   -1,
   -1,   -1,  355,  356,   -1,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  367,  491,   -1,   -1,  371,   -1,
  496,  497,   -1,  499,  500,  501,   -1,   -1,   -1,  505,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,   -1,   -1,   -1,  296,  297,   -1,   -1,
   -1,   -1,   -1,   -1,   41,  428,   -1,   44,   -1,   -1,
   -1,  434,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  444,   59,   -1,  324,   -1,   -1,  327,   -1,   -1,
   -1,   -1,   -1,   59,   -1,  458,  459,  460,  461,  462,
  463,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,  472,
   -1,   -1,   -1,   -1,   -1,  355,  356,    0,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  367,  491,   -1,
   -1,  371,   -1,  496,  497,   -1,  499,  500,  501,   -1,
   -1,   -1,  505,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,  262,   41,   -1,
   -1,   44,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  428,   -1,
   -1,   -1,   -1,   -1,  434,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  444,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  319,   -1,   -1,  322,  458,  459,
  460,  461,  462,  463,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,  472,   -1,   -1,   -1,  341,   -1,   -1,  344,
   -1,  481,  482,  483,  484,  485,  486,  487,  488,  489,
   -1,  491,   -1,   -1,    0,  360,  496,  497,   -1,  499,
  500,  501,   -1,   -1,   -1,  505,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  384,
   -1,   -1,  387,  388,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,  397,   -1,   -1,   41,  262,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  414,
   -1,   -1,   -1,   59,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  436,   -1,   59,   -1,   -1,  441,   -1,   -1,   -1,
   -1,   -1,  319,   -1,   -1,  322,   -1,   -1,  453,   -1,
  455,   -1,  329,  319,   -1,  332,  322,    0,   -1,  336,
   -1,   -1,   -1,  329,  341,   -1,  332,  344,  262,   -1,
  336,   -1,   -1,   -1,   -1,  341,   -1,   -1,  344,  262,
   -1,   -1,   -1,  360,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,
  387,  388,   -1,   -1,   -1,   -1,   59,   -1,  384,   -1,
  397,  387,  388,   -1,   -1,  319,   -1,   -1,  322,   -1,
   -1,  397,   -1,   -1,   -1,  329,  319,  414,  332,  322,
    0,   -1,  336,   -1,   -1,   -1,  329,  341,  414,  332,
  344,    0,   -1,  336,   -1,   -1,   -1,   -1,  341,  436,
   -1,  344,   -1,   -1,  441,   -1,  360,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,  453,  360,  455,   -1,
   -1,   41,   -1,   -1,   44,    0,   -1,  453,   -1,  455,
  384,   -1,   41,  387,  388,   44,   -1,   -1,   -1,   59,
   -1,  384,   -1,  397,  387,  388,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,  397,   -1,  262,   -1,   -1,   -1,
  414,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,  414,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  436,   -1,   59,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,  453,
   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  453,   -1,  455,  319,   -1,   -1,  322,   -1,   -1,   -1,
   -1,    0,   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,
  336,   -1,   -1,  319,   -1,  341,  322,   -1,  344,   -1,
   -1,   -1,   -1,  329,   -1,   -1,  332,   -1,   -1,   -1,
  336,   -1,   -1,    0,  360,  341,   -1,   -1,  344,  262,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  360,   -1,   -1,   -1,  384,   -1,
   59,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  397,   -1,   -1,   41,   -1,   -1,   44,  384,   -1,
   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,  414,   -1,
   -1,  397,   59,   -1,   -1,   -1,  319,    0,   -1,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,  414,  332,
  436,   -1,   -1,  336,   -1,  441,   -1,   -1,  341,   -1,
   -1,  344,  262,   -1,   -1,   -1,   -1,  453,   -1,  455,
  436,   -1,   -1,  262,   -1,  441,   -1,  360,   41,    0,
   -1,   44,   -1,   -1,   -1,   -1,   -1,  453,   -1,  455,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,  319,
   41,    0,  322,   44,   -1,   -1,   -1,   -1,   -1,  329,
  319,  414,  332,  322,   -1,   -1,  336,   -1,   59,   -1,
  329,  341,   -1,  332,  344,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,  436,   -1,  344,   -1,   -1,  441,   -1,
  360,   -1,   41,   -1,  319,   44,   -1,  322,   -1,   -1,
  453,  360,  455,   -1,  329,   -1,   -1,  332,   -1,   -1,
   59,  336,   -1,   -1,  384,   -1,  341,  387,  388,  344,
   -1,   -1,   -1,   -1,   -1,  384,   -1,  397,  387,  388,
   -1,   -1,   -1,  262,   -1,  360,   -1,   -1,  397,   -1,
   -1,   -1,   -1,   -1,  414,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  414,   -1,   -1,   -1,  384,
   -1,   -1,  387,  388,   -1,  262,  436,   -1,   -1,   -1,
   -1,  441,  397,   -1,   -1,   -1,   -1,  436,   -1,    0,
   -1,   -1,  441,  453,   -1,  455,   -1,   -1,   41,  414,
  319,   44,   -1,  322,  453,   -1,  455,   -1,   -1,   -1,
  329,   -1,   -1,  332,   -1,   -1,   59,  336,   -1,   -1,
   -1,  436,  341,   -1,   -1,  344,  441,   -1,   -1,   -1,
   41,   -1,  319,   44,   -1,  322,   -1,   -1,  453,  262,
  455,  360,  329,   -1,   -1,  332,   -1,   -1,   59,  336,
   -1,   -1,   -1,   -1,  341,   -1,   -1,  344,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   -1,   -1,  387,  388,
   -1,   -1,   -1,  360,   -1,   -1,   -1,   -1,  397,   -1,
   -1,  262,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  414,  319,  384,   -1,  322,
  387,  388,   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,
  397,   -1,   -1,  336,   -1,   -1,   -1,  436,  341,   -1,
   -1,  344,  441,  262,   -1,   -1,   41,  414,   -1,   44,
   -1,   -1,   -1,   -1,  453,   -1,  455,  360,  319,   -1,
   -1,  322,   -1,   -1,   59,   -1,   -1,   -1,  329,  436,
   -1,  332,   -1,   -1,  441,  336,   -1,   -1,   -1,   -1,
  341,  384,   -1,  344,  387,  388,  453,   -1,  455,   -1,
   -1,   -1,   -1,   -1,  397,   -1,    0,   -1,   -1,  360,
  319,   -1,   -1,  322,   -1,   -1,   -1,    0,   -1,   -1,
  329,  414,   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,
   -1,   -1,  341,  384,   -1,  344,  387,  388,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,  397,   41,  441,  262,
   44,  360,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
  453,   44,  455,  414,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  384,   59,   -1,  387,  388,
   -1,  262,   -1,   -1,   -1,  436,   -1,   -1,  397,   -1,
  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  453,   -1,  455,  414,  319,   -1,   -1,  322,
   -1,   -1,   -1,   -1,   -1,   -1,  329,   -1,   -1,  332,
   -1,   -1,   -1,  336,   -1,   -1,   -1,  436,  341,   -1,
   -1,  344,  441,   -1,   -1,   -1,   -1,   -1,  319,   -1,
   -1,  322,   -1,   -1,  453,   -1,  455,  360,  329,   -1,
   -1,  332,   -1,   -1,   -1,  336,   -1,   -1,   -1,   -1,
  341,   -1,   -1,  344,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  384,   -1,   -1,  387,  388,   -1,  262,   -1,  360,
   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  414,   -1,  384,   -1,   -1,  387,  388,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  397,   -1,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,
   -1,   -1,   -1,  414,  319,   -1,   -1,  322,   -1,   -1,
  453,   -1,  455,   -1,  329,   -1,   -1,  332,   -1,   -1,
   -1,  336,   -1,   -1,   -1,  436,  341,   -1,   -1,  344,
  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,
   -1,   -1,  453,   -1,  455,  360,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  384,
   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  397,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  319,   -1,   -1,  322,  414,
   -1,   -1,   -1,   -1,   -1,  329,  319,   -1,  332,  322,
   -1,   -1,  336,   -1,   -1,   -1,  329,  341,  273,  332,
  344,  436,  277,  336,   -1,   -1,  441,   -1,  341,   -1,
   -1,  344,   -1,   -1,   -1,   -1,  360,   -1,  453,   -1,
  455,  296,   -1,   -1,  299,   -1,   -1,  360,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  384,   -1,   -1,  387,  388,   -1,   -1,   -1,   -1,   -1,
   -1,  384,   -1,  397,  387,  388,   -1,   -1,   -1,   -1,
   -1,   -1,  273,   -1,  397,  340,  277,   -1,   -1,   -1,
  414,  346,  347,  348,   -1,  350,  351,  352,   -1,  354,
   -1,  414,   -1,   -1,   -1,  296,   -1,   -1,  299,   -1,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,  374,
   -1,  376,   -1,  436,   -1,   -1,   -1,   -1,  441,  453,
   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  453,   -1,  455,   -1,   -1,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  407,   -1,   -1,  346,  347,  348,   -1,  350,
  351,  352,   -1,  354,   -1,   -1,   -1,   -1,   -1,  424,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  374,   -1,  376,   -1,   -1,   -1,   -1,
   -1,  446,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  481,  482,   -1,   -1,
   -1,   -1,   -1,  424,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  498,   -1,   -1,   -1,  461,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  446,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,  485,   -1,   -1,  488,  489,  490,  491,   -1,  493,
   -1,   -1,  496,  497,   -1,   -1,   -1,   -1,   -1,   -1,
  481,  482,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  498,
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
					NULL, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], NULL, NULL); }
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
