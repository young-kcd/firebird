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
#define RELEASE 507
#define STATEMENT 508
#define LEAVE 509
#define INSERTING 510
#define UPDATING 511
#define DELETING 512
#define YYERRCODE 256
static short yylhs[] = {                                        -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    9,    9,    9,    9,    9,   22,   22,   21,   21,
   21,   32,   32,   25,   33,   33,   33,   33,   33,   24,
   24,   30,   30,   26,   14,   14,   14,   14,   14,   14,
   14,   36,   27,   27,   27,   27,   37,   37,   37,   37,
   23,   23,   38,   38,   38,   28,   28,   43,   29,   29,
   44,   44,    6,   45,   45,   47,   52,   52,   52,   49,
   49,   49,   56,   56,   57,   57,   50,   50,   59,   59,
   59,   59,   59,   46,    5,   62,   62,   62,   62,   62,
   62,   62,   62,   62,   62,   62,   12,   77,   77,   77,
   13,   81,   81,   64,   64,   67,   67,   67,   75,   90,
   90,   90,   91,   91,   92,   92,   93,   93,   97,   97,
   74,  100,  100,  102,  102,  103,  103,  106,  106,  107,
  108,  108,  109,  110,   72,   76,   73,   94,   94,  113,
  114,  114,  116,  116,  117,  117,  117,  117,  117,  115,
  115,  118,  118,  119,  119,  119,  119,  121,  121,  121,
  121,  120,  120,  125,  125,  124,   98,  126,  126,  127,
  130,  131,  131,  132,  128,  129,  129,  133,  133,  134,
  134,  135,  135,  135,   96,   96,   96,   69,   79,  136,
  136,  136,  137,  137,  138,  138,  139,  139,  139,  145,
   86,   86,  141,  141,  104,  104,   99,  149,  150,   58,
  142,  142,  105,  105,  105,  105,  105,  105,  143,  143,
  157,  157,  158,  160,  160,  160,  160,  160,  140,  159,
  159,  164,  164,  164,  164,  166,  167,  168,  162,  162,
  163,  161,  161,  161,  161,  161,  169,  170,  171,  171,
  171,  171,   68,   78,   82,  176,  172,  172,  173,  173,
  177,  177,  178,  174,  174,  179,  179,  180,  181,  181,
  182,  182,  183,  183,  175,  185,  185,  185,  186,  186,
  184,  184,  184,  184,  184,  184,  184,  184,  184,  184,
  184,  184,  184,  184,  184,  184,  184,  184,  189,  189,
  190,  191,  192,  203,  203,  193,  198,  199,  194,  194,
  195,  206,  206,  196,  207,  201,  201,  201,  202,  202,
  202,  204,  204,  204,  204,  197,  200,  200,  205,  205,
  187,  187,  212,  213,  213,  214,  214,  214,  214,   11,
  216,  216,  216,  217,  217,  217,  217,  217,  217,   71,
   80,  219,  222,  222,  222,  223,  226,  230,  230,  231,
  231,  232,  232,  147,  221,   87,   89,  220,  220,   70,
   83,  235,  235,  235,  236,  239,  239,  240,  240,  240,
  240,  240,  240,  240,  240,  240,  240,  240,  240,  240,
  240,  240,  237,  237,  238,    2,  242,  242,  242,  242,
  242,  242,  242,  250,  251,  248,  248,  252,  252,  252,
  252,  252,  252,  252,  243,  243,  253,  253,  253,  253,
  253,  253,  253,  247,  258,  258,  258,  258,  258,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  255,  255,  257,  257,  256,  254,  254,  254,  249,  249,
  245,  246,  246,  260,  260,  260,  260,  261,  261,  244,
  262,  262,  263,  263,    8,  264,  264,  264,  264,  264,
  264,  264,  264,  264,  264,  264,  264,  101,  101,  144,
  144,  267,  267,  270,  270,  272,  272,   53,   53,  269,
  269,  269,  269,  269,  269,  269,  269,  269,  277,  277,
  268,  268,  268,  268,  278,  278,   61,   61,   61,   55,
   55,  274,  274,  274,  271,  271,  271,  282,  282,  282,
  281,  281,  280,  280,  280,  275,  275,  285,  283,  283,
  283,  284,  284,  276,  276,  276,  276,  286,  286,   18,
   18,   18,  288,  288,  288,   16,   16,   16,  290,  291,
  292,  295,  295,    4,   15,  294,  294,  296,  296,  297,
  297,  287,  298,  298,  299,  299,  300,  300,  300,  300,
  301,  301,  302,  302,  303,  303,  305,  305,  305,  306,
  306,  306,  307,  307,  307,  304,  309,  309,  309,  310,
  310,  308,  308,  311,  313,  313,  312,  312,  289,   17,
  314,  314,  314,  315,  315,  318,  318,  319,   65,   65,
   65,  321,  321,  320,  320,  316,  316,  324,  324,  325,
  325,  317,  329,  330,  331,  322,  323,  326,  326,  326,
  326,  332,  332,  332,  333,  333,  333,  224,  224,  225,
  225,  335,  335,  336,  336,  336,  327,  339,  339,  340,
  340,  341,  341,  342,  342,  343,  343,  209,  209,  345,
  345,  233,  233,   31,  234,  234,  234,  234,  234,  234,
  234,  234,  228,  228,  346,  346,  347,  347,  347,  347,
  347,  349,  349,  349,  229,  229,  227,  227,  328,  328,
  353,  354,  354,  354,  354,  354,  355,  355,  356,  356,
  357,  357,  358,  358,  358,  359,  359,   10,   10,  361,
  361,    7,    7,  362,  363,  364,   19,   19,  365,  366,
  367,  367,  188,  337,  337,    3,    3,  369,  369,  369,
  371,  371,   61,  370,  370,  372,  372,   34,   34,   85,
   84,   84,  360,  360,  373,  374,  374,  210,  210,  210,
  146,  368,  368,  111,  111,  111,  111,  376,  376,  376,
  376,  376,  376,  376,  376,  376,  376,  376,  376,  377,
  377,  377,  377,  377,  377,  377,  377,  382,  382,  382,
  382,  382,  382,  382,  382,  382,  382,  382,  382,  382,
  382,  382,  382,  389,  389,  378,  378,  379,  379,  379,
  379,  380,  380,  384,  384,  385,  385,  385,  385,  383,
  386,  381,  381,  387,  387,  387,  390,  390,  391,  388,
  393,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,  156,  156,  156,
  394,  392,  392,  151,  151,  398,  398,  398,  398,  398,
  396,  396,  396,  396,  396,  218,  152,  152,  153,  154,
  154,  154,  154,  154,   51,   51,   20,   20,  241,  399,
   54,  123,  273,  273,   95,  395,  395,  395,  395,  400,
  400,  400,  400,  400,  400,  400,  400,  400,  400,  400,
  401,  350,  351,  351,  351,  348,  348,  397,  352,  352,
  403,  403,  404,  404,  405,  405,  408,  408,  406,  406,
  411,  411,  410,  407,  409,  402,  402,  402,  402,  402,
  402,  402,  402,  334,  334,  155,   48,  279,  122,  148,
  375,  165,  211,  266,   63,   60,  215,  112,   66,  338,
   35,   41,  344,  265,   39,   42,  208,   40,  293,  259,
  259,  412,  412,  412,  412,  412,  412,  412,  412,  412,
  412,  412,
};
static short yylen[] = {                                         2,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    2,    7,    8,    6,    7,    5,    1,    2,    1,    2,
    1,    1,    3,    1,    1,    1,    1,    2,    2,    3,
    0,    3,    0,    1,    7,    8,    6,    7,    6,    7,
    4,    3,    1,    3,    3,    3,    2,    2,    2,    2,
    1,    3,    1,    2,    2,    1,    3,    1,    1,    3,
    1,    2,    2,    2,    3,    8,    1,    1,    5,    0,
    1,    3,    1,    3,    2,    4,    1,    3,    2,    3,
    4,    4,    2,    9,    2,    3,    7,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    4,    2,    2,    1,    0,    1,    1,    6,    6,    1,
    1,    0,    0,    1,    0,    4,    0,    1,    1,    2,
    8,    1,    0,    3,    0,    0,    1,    1,    2,    1,
    1,    1,    2,    6,    1,    1,    3,    0,    1,    1,
    0,    1,    1,    2,    3,    4,    2,    2,    3,    0,
    1,    1,    2,    1,    1,    1,    4,    3,    3,    3,
    3,    1,    1,    6,    3,    1,    3,    1,    3,    2,
    1,    0,    2,    3,    1,    0,    1,    1,    2,    3,
    4,    0,    1,    2,    0,    1,    1,    5,    5,    3,
    2,    0,    1,    3,    1,    1,    6,    3,    2,    6,
    2,    1,    2,    2,    2,    0,    1,    1,    2,    0,
    2,    0,    1,    1,    1,    1,    1,    1,    0,    1,
    1,    2,    2,    2,    5,    1,    2,    3,    2,    2,
    0,    1,    1,    1,    1,    3,    4,    8,    4,    0,
    6,    1,    1,    2,    2,    0,    3,    3,    1,    2,
    2,    2,    8,    8,    8,    8,    3,    0,    2,    0,
    1,    3,    2,    1,    0,    1,    2,    6,    1,    0,
    2,    1,    1,    1,    3,    1,    2,    0,    1,    2,
    2,    2,    1,    1,    1,    1,    1,    1,    2,    1,
    1,    2,    1,    1,    1,    2,    2,    1,    3,    4,
    2,    6,    4,    1,    1,    7,    8,    6,    8,    6,
    4,    2,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    6,    2,    2,    3,    0,
    1,    2,    4,    1,    3,    2,    2,    2,    1,    4,
    1,    3,    0,    1,    1,    1,    3,    3,    3,    7,
    7,    1,    1,    3,    4,    7,    2,    1,    3,    1,
    1,    6,    3,    0,    0,    0,    0,    3,    0,    9,
    9,    1,    1,    0,    2,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    5,    5,    5,    5,
    5,    5,    2,    0,    4,    2,    3,    3,    2,    2,
    3,    3,    2,    3,    6,    1,    2,    4,    3,    2,
    2,    2,    2,    3,    1,    3,    3,    3,    2,    2,
    4,    4,    5,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    0,    2,    2,
    0,    1,    2,    2,    2,    2,    2,    1,    3,    7,
    1,    0,    1,    0,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    3,    2,    2,    2,    1,    1,    1,
    1,    4,    5,    1,    3,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    4,    4,    6,    5,    3,    0,    2,    2,    0,    3,
    0,    4,    1,    5,    4,    1,    4,    1,    2,    2,
    1,    1,    1,    2,    2,    2,    2,    1,    0,    3,
    5,    1,    1,    2,    3,    1,    2,    3,    0,    1,
    1,    1,    5,    5,    6,    1,    1,    1,    2,    3,
    5,    1,    0,    3,    2,    1,    0,    2,    0,    1,
    0,    3,    1,    0,    1,    2,    1,    1,    1,    1,
    2,    2,    1,    2,    3,    1,    1,    3,    3,    1,
    2,    3,    1,    2,    0,    2,    1,    1,    0,    1,
    1,    1,    3,    2,    3,    0,    1,    3,    4,    3,
    1,    3,    4,    3,    0,    1,    3,    3,    1,    1,
    0,    1,    1,    4,    0,    4,    0,    2,    0,    2,
    0,    9,   10,    0,    0,    0,    0,    3,    2,    1,
    0,    3,    5,    3,    2,    5,    2,    1,    1,    1,
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
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  560,
  561,  562,  566,  567,  568,    0,  621,  732,  733,  737,
  738,  471,    0,    0,    0,    0,    0,    0,  406,  576,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  899,    0,   21,  897,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  485,    0,    0,   37,   34,
   36,    0,   35,    0,  980,  982,  984,  983,  989,  985,
  992,  990,  986,  988,  991,  987,    0,    0,    0,    0,
   32,   68,   66,  972,  981,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  650,    0,
    0,    0,  682,    0,  974,    0,    0,    0,    0,  107,
  979,  569,    0,    2,    0,    0,    0,    0,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,  453,  454,  455,  456,  457,
  458,  459,  460,    0,  434,  435,    0,  965,    0,  413,
  969,    0,  410,  971,    0,  684,    0,  409,  975,    0,
  574,  895,    0,  150,  103,    0,  104,    0,  217,  961,
  771,    0,  102,  145,  968,    0,    0,   98,    0,  105,
  901,    0,   99,    0,  100,    0,  101,  978,  146,  106,
  629,  630,    0,  900,  898,    0,   74,    0,  966,    0,
  964,  493,  486,    0,  492,  497,  487,  488,  495,  489,
  490,  491,  496,    0,   30,    0,   39,  758,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  905,    0,  886,  644,  644,  645,
  655,  657,  954,  658,    0,  659,  649,  645,    0,    0,
    0,    0,    0,    0,    0,  593,  582,    0,  585,  587,
  588,  589,  590,  596,  597,    0,  973,  683,    0,  108,
    0,  109,    0,  110,  570,    0,    0,  622,    0,  620,
    0,    0,    0,    0,  472,    0,    0,  376,    0,  220,
    0,  416,  407,  469,  470,    0,    0,    0,    0,    0,
    0,  425,    0,  382,  383,    0,  580,  578,  896,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  957,   75,    0,    0,    0,  734,  735,  494,
    0,  956,  878,  877,  876,    0,    0,  879,  880,    0,
    0,  882,  355,  356,  350,    0,  354,  874,  881,    0,
  761,    0,    0,   27,    0,    0,    0,   71,   69,  976,
   67,   33,    0,    0,    0,  763,    0,   52,    0,    0,
    0,    0,    0,  572,    0,    0,    0,    0,  864,  892,
    0,    0,  866,    0,    0,  893,    0,  887,    0,    0,
  868,  869,  870,  888,  889,    0,  890,  891,    0,    0,
    0,  894,    0,    0,    0,    0,    0,    0,  768,  861,
  862,  863,  867,  847,    0,  846,    0,    0,  850,  908,
  909,  849,  843,  844,  845,  848,  906,  907,  929,  930,
  933,  934,  652,  654,    0,  661,    0,  745,    0,    0,
  662,    0,  648,    0,  619,    0,  594,    0,  591,    0,
  592,  617,    0,  612,    0,    0,  586,  772,  741,    0,
    0,    0,    0,    0,    0,    0,    0,  626,  623,    0,
  185,    0,    0,  129,  477,  172,  173,    0,  475,    0,
    0,    0,    0,  478,    0,  473,    0,    0,  420,  422,
  421,    0,  423,    0,  417,  218,    0,    0,  271,    0,
    0,    0,    0,  429,  430,    0,  462,    0,    0,    0,
    0,    0,    0,    0,  387,  386,  481,    0,    0,  149,
    0,    0,  158,    0,  157,    0,  164,  147,    0,  162,
  165,  166,  154,    0,    0,  516,  552,  519,    0,  553,
    0,    0,  520,    0,    0,    0,  543,  556,  515,  538,
  517,  518,  513,  500,  376,  498,  499,  501,    0,    0,
  510,  511,  512,  514,    0,    0,    0,    0,    0,  112,
    0,  113,    0,  124,    0,    0,  201,    0,    0,  374,
    0,  220,    0,    0,   83,    0,    0,  753,    0,    0,
    0,    0,    0,  834,  835,  836,    0,    0,    0,  774,
  778,  779,  780,  781,  782,  783,  784,  785,  786,  787,
  788,  883,  884,  885,  875,    0,    0,    0,  760,   28,
    0,    0,   44,   72,    0,    0,   26,    0,  766,    0,
    0,    0,  729,    0,    0,    0,    0,    0,  571,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  325,  977,    0,    0,    0,    0,    0,    0,    0,  644,
    0,    0,    0,    0,    0,    0,    0,  970,  665,  564,
    0,  903,  563,    0,  595,    0,  603,  599,  598,    0,
    0,    0,  614,  602,    0,    0,  739,  740,    0,    0,
    0,  374,    0,    0,    0,    0,    0,    0,  130,    0,
    0,    0,    0,    0,    0,  376,  419,    0,  376,  377,
  424,  508,    0,  273,    0,  267,  269,  374,  240,  962,
    0,    0,  376,    0,    0,  209,  374,    0,    0,    0,
    0,  245,  239,  242,  243,  244,  428,  467,  466,  427,
  426,    0,    0,    0,    0,    0,    0,  385,    0,  376,
    0,  155,  159,    0,  163,    0,    0,  539,  540,  557,
    0,  554,    0,    0,  546,  544,  545,    0,    0,    0,
    0,  509,    0,    0,    0,    0,  547,    0,    0,  374,
    0,  200,    0,  203,  205,  206,    0,    0,    0,    0,
    0,  220,   78,    0,    0,   77,  527,  958,  528,    0,
    0,    0,  777,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  352,  358,  359,  357,  762,    0,    0,
    0,    0,    0,    0,    0,    0,   53,   61,   63,    0,
    0,   70,    0,    0,    0,  765,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  946,  947,
  948,  949,  950,  951,  952,  953,    0,    0,    0,    0,
    0,    0,  939,    0,    0,    0,  680,    0,    0,  859,
  860,  927,    0,    0,  855,  960,    0,    0,    0,    0,
    0,  653,    0,  865,  770,  769,  656,    0,    0,    0,
    0,  670,  671,    0,    0,  663,  666,  565,  904,  604,
  613,  608,  607,    0,  618,  773,  742,  743,  374,    0,
    0,  646,  628,  627,    0,    0,  636,  181,  175,  182,
    0,  178,    0,    0,  177,    0,  188,  169,  168,  902,
  171,  170,  479,    0,    0,    0,  418,  272,    0,  211,
  376,  213,    0,  377,  208,  214,    0,    0,    0,    0,
  431,  432,  376,  374,  377,    0,    0,    0,  403,    0,
  196,  197,  156,    0,    0,    0,    0,    0,    0,  555,
    0,    0,  376,  377,    0,  504,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  198,
    0,    0,    0,    0,  363,    0,   82,    0,    0,    0,
    0,   87,   84,    0,    0,    0,  736,  963,    0,    0,
  789,    0,    0,    0,  815,  814,    0,    0,    0,  822,
  837,  832,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  776,    0,   65,   57,   58,   64,   59,   60,
    0,    0,   22,    0,    0,    0,   42,    0,    0,    0,
  747,  767,    0,  730,    0,  746,    0,    0,    0,    0,
    0,    0,  220,    0,  910,    0,    0,  925,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  935,    0,  945,    0,    0,    0,    0,    0,    0,  926,
  871,    0,    0,    0,    0,    0,    0,  685,    0,    0,
    0,    0,    0,  610,  611,  615,    0,  199,    0,    0,
  640,    0,    0,    0,    0,    0,    0,  189,    0,    0,
  414,  223,  224,  225,  226,  227,  228,    0,    0,    0,
  276,    0,  221,    0,    0,    0,    0,  246,    0,  463,
  464,  433,    0,    0,    0,    0,    0,    0,  376,  483,
  377,  167,  959,    0,    0,  522,    0,  521,  558,    0,
  550,    0,    0,    0,  502,    0,  530,    0,    0,  532,
  535,  537,  374,    0,    0,    0,  119,    0,  204,  376,
    0,    0,  375,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  830,  831,    0,    0,    0,
    0,    0,    0,  833,    0,    0,    0,    0,    0,    0,
    0,  823,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   56,   62,   54,
   55,   23,  751,  752,    0,  749,  756,  757,  754,    0,
    0,  728,    0,    0,  916,  915,    0,    0,  912,  911,
    0,  920,  919,  918,  917,  914,  913,    0,    0,  941,
    0,    0,    0,  940,    0,    0,    0,  678,    0,    0,
  673,    0,  674,    0,  691,  687,  689,    0,    0,    0,
    0,    0,  375,  632,  633,  647,    0,  183,  179,    0,
    0,  194,  190,    0,  377,  279,    0,    0,  377,  277,
    0,    0,    0,  231,    0,    0,  247,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  480,  524,    0,  525,
    0,  134,    0,    0,    0,  138,  140,  141,  142,  505,
  507,  503,  534,    0,    0,  377,    0,    0,    0,    0,
  360,    0,  364,  376,   88,    0,    0,   90,    0,    0,
    0,    0,    0,    0,  839,  838,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   40,    0,    0,  731,  219,  928,
  921,  922,    0,  923,  937,  936,    0,  942,  931,    0,
  932,    0,  676,    0,  548,    0,  698,    0,    0,  695,
  699,  700,  702,  703,  704,    0,    0,  642,  377,  361,
  634,    0,  174,  191,  377,  144,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  284,  289,  283,    0,    0,    0,  293,  294,
  295,  296,  297,  298,  300,  301,  303,  304,  305,  308,
  266,    0,    0,  207,  232,    0,    0,    0,    0,  233,
  236,    0,    0,  377,  401,  402,  398,  397,  400,  399,
    0,  523,  551,  143,  131,  139,    0,  376,  263,  126,
  377,    0,    0,  378,  365,    0,    0,   91,   92,   79,
    0,  801,  809,    0,  802,  810,  805,  813,    0,    0,
  803,  811,  804,  812,  798,  806,  799,  807,  800,  808,
  748,    0,    0,  938,  679,    0,    0,    0,    0,    0,
  712,  714,    0,  709,    0,  264,  184,  415,    0,  311,
    0,    0,  314,  315,    0,  307,    0,    0,    0,    0,
  306,    0,  337,  338,  292,  299,    0,  302,  285,    0,
  290,    0,  341,  291,  210,  215,  234,    0,    0,  237,
    0,  249,  251,  405,  377,    0,  380,    0,    0,    0,
  370,  371,    0,    0,   76,   94,    0,    0,  924,    0,
    0,  701,  696,  713,    0,    0,  282,    0,  309,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  332,
  333,  349,    0,    0,    0,    0,  344,  342,  238,    0,
    0,    0,    0,    0,  265,  377,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  720,    0,    0,    0,
  281,  278,  310,    0,    0,    0,    0,  313,    0,    0,
    0,    0,  321,    0,  324,    0,  348,  347,  967,  346,
    0,    0,    0,    0,    0,  248,    0,  255,    0,  254,
  381,  373,    0,    0,  366,  118,    0,    0,    0,  722,
  711,    0,    0,  723,    0,  719,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  335,  334,  343,  345,
  235,    0,    0,  259,  258,  257,    0,    0,  643,  841,
  718,    0,  725,  327,    0,    0,  312,  318,    0,    0,
    0,    0,  336,  262,  260,  261,    0,    0,    0,    0,
    0,    0,  339,  316,    0,    0,    0,    0,  724,  330,
  317,  319,  840,  727,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1529,   28,   29, 1530,
   31,   32,   33,   34,   35,   36, 1531,   38, 1532,  638,
  117,  403,  895, 1143,  118,  672,  896,  119,  407,  677,
  143,  120,  121,  257,  969,  133,  897,  898,  197,  226,
  122,  899,  123,  409,   82,  237,  374,  457,  633, 1080,
  392,  855,  604, 1495,  832,  634,  635,  636, 1082,  238,
 1333,   74,  187,   75,  233, 1819, 1285,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  150,  310,  312,  314,
  363,  620,  622,  400,  258,  782,  538,  648, 1017,  367,
  625, 1068, 1277,  571,  732, 1043,  523,  524,  553,  359,
  605, 1054, 1424, 1554, 1221, 1425, 1426, 1427, 1428, 1429,
  649,  214,  206,  355,  578,  356,  357,  579,  580,  581,
  582, 1252, 1011,  526,  527, 1001,  999,  528, 1005, 1000,
 1212, 1388, 1006, 1007, 1217,  369,  843,  844,  845,  846,
  784, 1024, 1402,  606,  786,  459,  848,  955,  547, 1348,
 1222,  460,  461,  462,  947,  463, 1403, 1404,  556, 1560,
 1712, 1238,  792,  793,  779,  794,  795,  796, 1713, 1714,
 1795,  337,  551, 1229, 1533,  193,  548,  549, 1230, 1231,
 1397, 1688, 1534, 1535, 1536, 1537, 1652, 1538, 1539, 1540,
 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548, 1549, 1550,
 1735, 1780, 1635, 1699, 1784, 1697,  464,  711, 1367,  465,
 1087, 1653, 1706, 1707, 1748,  395,  396,  466, 1073, 1283,
 1441, 1074, 1075,  285,  489, 1583,  378, 1203, 1381, 1669,
 1670, 1671, 1672, 1201,  346,  567,  810, 1250,  568,  808,
  221,   59,  341,  198,  158,  324,  184,  331,  190,  770,
  767,  332,  342,  800,  343,  563, 1035,  185,  467,  325,
  535,  569, 1251,   96,  196,  242,  607,  608,  772, 1055,
  773, 1056, 1057,  611,  612,  613,  614, 1048,  859,  615,
  616,  617,  825,  618, 1497,  822,   40,   41,   42,   43,
   44,   45,  152,   61,  425,  201,  348,  297,  298,  299,
  300,  301,  302,  303,  304,  305,  738,  503,  984, 1206,
  504,  505,  743,   46,  157,  320,   47,  517,  518,  993,
 1386, 1210, 1511,  756,  997,  137,  725, 1508,  683,  483,
  287,  138,  139,  286,  490,  491,  492,  729,  970,  971,
  972,  973, 1195,  468, 1183, 1499, 1500,  469, 1502,  470,
  471,  472, 1727, 1625, 1728, 1729, 1730, 1776, 1820,  415,
 1155,   48,   49,  379,   50,   51,  511,  512,  904, 1151,
 1335, 1339,  416,  680,  211,  650,  651,  652,  653,  654,
  655,  656,  657,  658,  659,  660,  661, 1302, 1098, 1100,
 1101,  954,  710,  473,  474,  475,  476,  399,  235,  477,
  478,  937,  479,  480,  481,  482,  702,  941, 1184, 1179,
  703,  125,
};
static short yysindex[] = {                                   4590,
 1618, -205, 5020,   83, 1187,  -10, 4209,  -42, 5618,  682,
   91, 4522, -205, 1354,  793, 4583,  997, 4583,  242,    0,
  673,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  437,    0,    0,    0,    0,
    0,    0, 6377, 4583, 4583, 4583, 4583, 4583,    0,    0,
  363, 1317, 4583, 4583, 4583,  555, 4583,  380, 4583, 4583,
    0, 4583, 4583,    0, 1043,    0,  498,    0,    0,  648,
 4583,    0, 4583, 4583, 4583,  678, 4583, 4583, 4583, 4583,
  380, 4583, 4583, 4583, 4583,    0, 4583,  703,    0,    0,
    0, 1016,    0, 1016,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  737,  802,  142, 1157,
    0,    0,    0,    0,    0, 4583, 4583, 4583,  852,  894,
  926,  164, 1066,  883,  202,  205,   65,  881,    0, 4583,
 1089,  544,    0, 1168,    0, 4583, 4583, 4583, 4583,    0,
    0,    0, 4583,    0, 1362,  -33, 1322,    5,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1078,    0,    0, 1317,    0,   41,    0,
    0, 1454,    0,    0, 1579,    0,   58,    0,    0, 1255,
    0,    0, 1208,    0,    0, 1171,    0, 1331,    0,    0,
    0, 1317,    0,    0,    0,  476, 1454,    0,   55,    0,
    0, 1381,    0, 1417,    0, 1016,    0,    0,    0,    0,
    0,    0, 1407,    0,    0, 1325,    0, 1478,    0, 1387,
    0,    0,    0, 1325,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2935,    0, 4583,    0,    0,    0, 4214,
 1441, 4467, 4583, 1045, 1519, 1823, 1550, 1560, 4214, 1480,
 4467, 1513, 1522, 1403,    0, 9886,    0,    0,    0,    0,
    0,    0,    0,    0, 7686,    0,    0,    0, 1477, 4583,
 1553, 1465,  540, 4583, 1493,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0, 4583,    0,    0, 1454,    0,
 1381,    0, 1016,    0,    0, 9886, 1516,    0, 1496,    0,
 1076, 1585, 1156,    5,    0, 1672,   39,    0, 4583,    0,
 1078,    0,    0,    0,    0, 4583, 1549, 3486, 1495, 3934,
 1924,    0, 6377,    0,    0, 1100,    0,    0,    0, 1917,
 1917, 1317, 1621, 1317, 1488, 1171,    0,    0, 4215,    0,
 4583, 4583,    0, 1549,    0,    0, 1708,  789, 1955, 4583,
 1738, 4583,    0,    0, 1964,  191, 4879,    0,    0,    0,
 1562,    0,    0,    0,    0, 1575, 1576,    0,    0, 1760,
 1176,    0,    0,    0,    0, 1966,    0,    0,    0, 1386,
    0, 4583, 1612,    0, 4583, 4583,  147,    0,    0,    0,
    0,    0, 4583, 4583,  911,    0, 4583,    0, 1729, 4583,
 1977, 4214, 1651,    0, 4583, 2014, 2015, 2018,    0,    0,
 2025, 2026,    0, 2027, 2028,    0, 2031,    0,    0, 2035,
    0,    0,    0,    0,    0, 2037,    0,    0, 8284, 2038,
 2041,    0, 9886, 9886, 8514, 4583, 2048, 1686,    0,    0,
    0,    0,    0,    0, 1998,    0,    0, 2045,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9886,    0, 2084,    0, 1774, 2056,
    0, 2449,    0,  119,    0,  803,    0,  618,    0,  618,
    0,    0, 2057,    0,  145, 1676,    0,    0,    0, 2060,
   81, 2046, 1549, 2068, 1846, 1807, 2067,    0,    0, 1732,
    0,  170, 1764,    0,    0,    0,    0, 1317,    0, 1917,
 1917, 1917, 1917,    0, 2069,    0, 1838, 1839,    0,    0,
    0, 1819,    0, 4215,    0,    0, 4215, 1634,    0, 1454,
 1859, 4583,10924,    0,    0,  872,    0, 4583,  619, 1579,
 1733, 1698, 1681,    0,    0,    0,    0,  792, 1755,    0,
 1696,  380,    0, 1317,    0, 1879,    0,    0, 1488,    0,
    0,    0,    0,  130, 1710,    0,    0,    0, 1716,    0,
 1765, 2121,    0, 1818, 2125,   -8,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2078,  148,
    0,    0,    0,    0,  150, 2130, 2131, 2125, 1454,    0,
 1855,    0, 1912,    0, 1317, 1317,    0, 3486,   58,    0,
 1795,    0, 1776, 2148,    0, 9622, 1372,    0, 1806, 1817,
 2160, 5356, 2165,    0,    0,    0, 5109, 3561,   15,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1677, 1176, 4583,    0,    0,
 4550, 1775,    0,    0, 1757, 4467,    0, 1906,    0, 1739,
 1354, 2184,    0, 1906, 4550, 1905, 1907, 4583,    0,  944,
 8754,  169, 4583, 9886,  988, 1019, 1165, 3715, 9886, 5356,
 2084, 1778,  655, 9886, 8754,  807,  807, 1354, 1753, 2185,
    0,    0, 8818, 4583, 9886, 9886, 9886, 9886, 9886,    0,
 9886,  445, 2138,  990, 1779, 8754, 4583,    0,    0,    0,
 1053,    0,    0,  -37,    0, 1785,    0,    0,    0, 4583,
 1191, 4583,    0,    0, 4583, 4583,    0,    0, 8754, 1972,
 3486,    0, 1745, 9886, 4583, 1794, 1317, 1317,    0,  507,
 1696, 1696, 1798,  380, 1156,    0,    0, 2217,    0,    0,
    0,    0, 1982,    0, 4583,    0,    0,    0,    0,    0,
 1994, 2229,    0, 1975, 1992,    0,    0, 1936, 1940, 1016,
 2005,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  380, 4583,11013, 1899, 1901, 1903,    0,  380,    0,
 1500,    0,    0, 1874,    0,   70, 1877,    0,    0,    0,
  380,    0, 2121,  100,    0,    0,    0, 1995,  100, 1878,
  100,    0, 2259,  380,  380,  380,    0, 1549, 4583,    0,
 1950,    0, 1769,    0,    0,    0, 1100, 1887, 4583, 1816,
  127,    0,    0, 2269, 2040,    0,    0,    0,    0,  191,
 4583, 1895,    0, 1895, 4052,  121, 9886, 9886, 6051, 2275,
   -7, 6298, 9886, 6545, 1245, 6630, 6877, 9058, 7124, 7209,
 7456, 5356, 5356,    0,    0,    0,    0,    0, 4583, 4583,
 4583, 4583, 4583, 4583,  162, 2272,    0,    0,    0, 4550,
 1932,    0,   26, 1954, 4583,    0,   65, 8754, 1954, 2278,
 2272, 4550, 3858, 2006, 9886, 9886, 2061, 9886, 2286, 9886,
 2287, 2318, 9886, 9886, 9886, 9886, 9886, 9886,    0,    0,
    0,    0,    0,    0,    0,    0, 2013, 1220,  676, 9886,
  845, 8754,    0, 5356, 2196, 2084,    0, 2290,   65,    0,
    0,    0, 2084, 1824,    0,    0, 2062,  807,  807, 1313,
 1313,    0, 1077,    0,    0,    0,    0,  990, 2296, 2293,
 1285,    0,    0, 5356, 2016,    0,    0,    0,    0,    0,
    0,    0,    0,  321,    0,    0,    0,    0,    0, 1835,
 1887,    0,    0,    0, 2299, 1849,    0,    0,    0,    0,
 1842,    0, 1917, 2085,    0,  507,    0,    0,    0,    0,
    0,    0,    0, 2312, 5356, 2568,    0,    0, 2059,    0,
    0,    0, 2568,    0,    0,    0, 1016, 1016, 1854, 2327,
    0,    0,    0,    0,    0,  405,  689, 1085,    0, 2106,
    0,    0,    0, 4583,   83, 1880, 1949, 1982, 2331,    0,
 1696, 1891,    0,    0, 1114,    0, 2316, 4583, 1132,  380,
 2334, 2335, 2336, 2115,   58, 2059, 1917, 1764, 3486,    0,
 1755,   65, 1927, 1944,    0,  815,    0,  380, 1991, 2089,
 9622,    0,    0,  380, 1911, 2092,    0,    0, 2356, 2363,
    0, 2473, 2084, 2371,    0,    0, 2084, 2372, 9288,    0,
    0,    0, 2063, 2376, 2084, 2379, 1875, 2388, 2084, 2393,
 9886, 9886, 2275, 9886, 9352, 2394, 2084, 2395, 2397, 2084,
 2401, 9886, 2084, 2402, 2084, 2404, 2414, 2084, 2415, 2416,
 2084, 2423,    0, 2151,    0,    0,    0,    0,    0,    0,
 2137, 4550,    0, 4550,  162, 2272,    0,  219,  219,   94,
    0,    0, 7686,    0, 1935,    0, 2278, 2272, 2421, 3858,
 2384, 2444,    0, 2451,    0, 2785, 9886,    0, 2962, 3320,
 3474, 3518, 3528, 3537, 9886,  380, 8754, 2084, 2032, 8754,
    0, 9886,    0, 2158,  783, 9886, 8754, 9886, 9886,    0,
    0, 1285, 2433, 8754, 4583,  990, 2100,    0, 2102, 2107,
 2154, 2206, 2168,    0,    0,    0, 2059,    0, 1927, -163,
    0, 2080, 1317, 2110, 1696, 2112, 1696,    0, 5356,  611,
    0,    0,    0,    0,    0,    0,    0, 2064, 2235, 2059,
    0, 9886,    0, 2220, 2105, 1854, 1043,    0, 5356,    0,
    0,    0, 2123, 2127, 2133, 2135, 2136, 2144,    0,    0,
    0,    0,    0, 2476,   83,    0, 1798,    0,    0,  100,
    0, 2568, 2155,  100,    0,  100,    0, 1982, 2491,    0,
    0,    0,    0, 1100, 2235, 1696,    0, 1764,    0,    0,
 7686, 2256,    0,  477,    0, 2299,    0, 2495,    0, 2497,
 1317,   43, 2498,    0, 1317,    0,    0, 9886, 2119, 2119,
 1354, 2499, 1939,    0, 2119, 2119, 9886, 2119, 2119, 2591,
 2084,    0, 1909, 9886, 2084, 2119, 2119, 2119, 2119, 2084,
 2119, 2119, 2119, 2119, 2119, 2119, 2161,    0,    0,    0,
    0,    0,    0,    0, 2111,    0,    0,    0,    0, 1774,
 8754,    0, 3858, 2421,    0,    0, 4215, 2508,    0,    0,
 3637,    0,    0,    0,    0,    0,    0, 3811,  141,    0,
 8754, 2242, 2116,    0, 8754, 3885, 1941,    0, 1479, 2084,
    0, 1948,    0, 1285,    0,    0,    0,  990, 5303, 5356,
 2156, 2235,    0,    0,    0,    0, 1917,    0,    0, 1317,
 1500,    0,    0,  631,    0,    0, 4583,10804,    0,    0,
 2084, 2274, 2220,    0, 1158, 4583,    0, 2218,  656, 2114,
 2222, 2120, 2260, 2226, 2264, 2059,    0,    0, 2526,    0,
 2527,    0, 2219, 2274, 2155,    0,    0,    0,    0,    0,
    0,    0,    0, 2059, 1755,    0, 1500, 2106, 2249, 2189,
    0, 1887,    0,    0,    0, 2205,  353,    0, 1982, 2210,
 2084, 2541, 2542,   65,    0,    0, 2545, 2551, 2084, 2553,
 2554, 9886, 9886, 2084, 2559, 2560, 2561, 2563, 2564, 2565,
 2567, 2569, 2573, 2575,    0,  219, 1779,    0,    0,    0,
    0,    0,  380,    0,    0,    0, 8754,    0,    0, 8754,
    0, 1779,    0, 1241,    0, 2337,    0, 2572, 2581,    0,
    0,    0,    0,    0,    0,   15,  -59,    0,    0,    0,
    0, 1696,    0,    0,    0,    0, 4215, 2088,  999, 2570,
   35, 2590, 2291, 9886, 2578, 2599, 2583, 2585, 2586, 2587,
 2300, 2589,    0,    0,    0, 2342,10726, 2600,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2620, 4583,    0,    0, 2310, 2319, 4583, 1854,    0,
    0, 1016, 4583,    0,    0,    0,    0,    0,    0,    0,
 2235,    0,    0,    0,    0,    0, 2235,    0,    0,    0,
    0, 1326, 1779,    0,    0, 9886, 1317,    0,    0,    0,
 1317,    0,    0, 9886,    0,    0,    0,    0, 2084, 2084,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2016, 2625,    0,    0, 2016, 5356, 4583, 1200, 5303,
    0,    0, 2304,    0, 2634,    0,    0,    0,  167,    0,
 9592, 4583,    0,    0, 9886,    0,  569, 2343, 5356, 2579,
    0, 5356,    0,    0,    0,    0, 1498,    0,    0,  534,
    0, 2225,    0,    0,    0,    0,    0, 1854, 1016,    0,
 2294,    0,    0,    0,    0, 2106,    0, 1326, 2637, 1285,
    0,    0, 2016, 2084,    0,    0, 1479, 2168,    0, 2168,
   15,    0,    0,    0, 3566, 2568,    0, 2626,    0, 2859,
 9656, 1799, 9886, 1498,  704, 9886, 2632,  740, 1378,    0,
    0,    0, 4583, 4583,   83,  226,    0,    0,    0, 2294,
  724, 1854, 2301, 2309,    0,    0, 1285, 2652, 1326, 2351,
 2168, 2654, 1779, 2156, 2156, 4583,    0, 2655, 2656, 1091,
    0,    0,    0, 7784, 2289, 2657, 1498,    0, 1711,  179,
 2267, 2084,    0, 2399,    0, 1498,    0,    0,    0,    0,
10804,  534, 1854,  374,  374,    0, 2407,    0, 2263,    0,
    0,    0, 1285, 1326,    0,    0, 2016, 2323, 2323,    0,
    0, 3566, 2671,    0, 4583,    0, 1753, 2000, 1082, 2659,
 1701, 1498, 2426, 2412,10804,10804,    0,    0,    0,    0,
    0, 2258,  574,    0,    0,    0, 1847, 2168,    0,    0,
    0, 4583,    0,    0, 1498, 2684,    0,    0,  588, 4583,
10804, 2425,    0,    0,    0,    0, 5356, 2156, 2693, 2697,
 2002,10804,    0,    0,10804,   15, 2323, 4583,    0,    0,
    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  215, 1001,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115, 5695,    0,    0,    0,    0,    0,    0,
 2739,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  456,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  356,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2403,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2358,    0,    0,
    0,  248,    0,  248,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2359,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  864,    0,    0, 8023, 5954,    0,    0,
    0,  919,    0,    0,    0, 2584,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  398,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -12,    0,    0,    0,    0,  440,    0,    0, 1194,
    0,    0,    0,    0,    0, 1172,    0, 6294,    0,    0,
    0,    0,    0,    0,    0,    0,  -12,    0,  -30,    0,
    0, 2705,    0,    0,    0, 2484,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1243,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1321,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1198,    0,    0,    0,    0,
    0,    0,    0, 4717,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  171,    0,    0, 1330,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -12,    0,
 2705,    0, 2484,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1335,    0, 2474,    0,    0,    0,    0,
 1348,    0,    0,    0,    0,    0, 2492,  738, 8434,    0,
 1357,    0,    0,    0,    0,  390,    0,    0,    0, 2303,
 2303,    0,    0,    0, 1361, 1301,    0,    0,    0,    0,
    0,    0,    0, 2492,    0,    0, 1442,    0,    0,    0,
    0,    0,    0,    0, 9318, 2374,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1377,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1395,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1456,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1762,    0,
    0,    0,    0,    0,    0, 2347,    0,    0,    0, 2482,
 2838,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2973,    0, 1970,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0, 2445,
    0,  176,    0,    0,    0,    0,    0,  749,    0,  749,
    0,    0,  635,    0,  647,  739,    0,    0,    0,    0,
 1243,    0, 2492,    0,    0,  124, 1141,    0,    0,  155,
    0, 2341,  387,    0,    0,    0,    0,    0,    0, 2303,
 2303, 2303, 2303,    0,  438,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2500,
    0,    0,    0,    0,    0, 2474,    0,    0, 1073,    0,
 2298,    0,    0, -108,    0,    0,    0,    0,  216,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1464,    0,
    0,    0,    0, 1134,  932,    0,    0,    0, 5228,    0,
    0, 5472,    0,    0, 5615,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1341, 1866,
    0,    0,    0,    0, 5627, 4700,    0, 5615,  -12,    0,
    0,    0,    0,    0,    0,    0,    0,  738, 1380,    0,
    0,    0,    0, 2355,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  163,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  276,    0,    0,
 5695,    0,    0,  276,    0,    0,    0,    0,    0, 9969,
    0, 9969,    0,    0, 9969, 9969, 9969,    0,    0,    0,
 2320,    0,    0,    0,    0, 3955, 4311, 8175,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1129,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  415,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  738,    0, 1069,    0,    0,  418,    0,    0,    0, 3191,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  940,    0,    0,    0,    0,    0,    0,    0,
 2729,    0,    0,  974, 2373,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  465,  701,  767,    0,    0,    0,
 2907,    0,    0,    0,    0,    0, 1161,    0,    0,    0,
    0,    0, 5472,    0,    0,    0,    0,  933,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2492,    0,    0,
  181,    0,    0,    0,    0,    0,    0,    0,    0,    0,
11101,    0,    0,    0,  246,    0,    0,    0,    0, 2463,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1518, 1546,    0,    0,    0,    0,
    0,    0,    0, 1557,    0,    0, 8023,    0, 1557, 1559,
 1596,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  962,    0,    0, 9969,    0,
    0,    0, 1349,    0,    0,    0, 3329, 4446, 4802, 3464,
 3820,    0,    0,    0,    0,    0,    0,    0,  784, 1797,
 1588,    0,    0,    0,  571,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  166,    0,    0,    0,    0,    0,
    0,    0, 2303, 2317,    0, 3398,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2503,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1730,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1608,
    0,    0,    0,    0,    0,    0,    0, 2279,    0,    0,
    0,    0,    0,    0,    0,    0, 1189,    0,    0,    0,
    0,    0,    0,    0, 1380, 2503, 2303, 1613,  738,    0,
 2513, 8023, 1626,  153,    0,    0,    0,    0,11101,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1622,    0,    0,    0, 3719,    0,    0,    0,
    0,    0,    0,    0, 4210,    0, 6295,    0, 7422,    0,
    0,    0,    0,    0,    0,    0, 7662,    0,    0, 7973,
    0,    0, 8834,    0, 9034,    0,    0,10163,    0,    0,
10174,    0,    0, 2118,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1518, 1627,    0, 2345,  350,    0,
    0,    0,    0,    0,    0,    0, 1665, 1673, 1684,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2344,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2435, 1869,    0, 3215,    0, 2440,    0, 2442, 2443,
    0,    0,  135,    0,    0,    0, 2503,    0, 1626,    0,
    0, 3866,    0,    0,    0, 2330,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 4775,    0, 2521,
    0,    0,    0, 3088,    0, 1730, 2403,    0,    0,    0,
    0,    0,  799,  806,  831,  898,  901,  928,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  980,    0,    0,    0,    0, 2999,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1687,    0,    0,
    0,    0,    0,    0,    0, 1693,    0,    0,    0,    0,
    0,  132,    0,    0,    0,    0,    0,    0,    0,    0,
 8175,    0,    0,    0,    0,    0,    0,    0,    0,    0,
10237,    0,10257,    0,10320,    0,    0,    0,    0,10365,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1713,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1740,
    0,    0,    0, 3116,    0,    0,    0,    0,    0,    0,
  535,    0,    0,    0,    0,    0, 2303,    0,    0,    0,
 2039,    0,    0,    0,    0,    0,    0, 2487,    0,    0,
 2755, 1867, 3490,    0, 2474,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2503,    0,    0,    0,    0,
    0,    0,    0, 1714, 1064,    0,    0,    0,    0,    0,
    0,    0,    0, 2503, 2513,    0,  313,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  152,    0,
10466,    0,    0, 9969,    0,    0,    0,    0,10530,    0,
    0,    0,    0,10593,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  350, 1101,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  657,    0, 2435,    0,  473,    0,    0, 1009,    0,
    0,    0,    0,    0,    0,  160, 2757,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1486, 1731,    0,    0,
    0,    0,    0,    0,    0,    0, 2496,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3039,    0,
    0,  136,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  238,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,10673,10694,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  969,    0,    0,    0,  138,    0,    0,    0,    0,
    0,    0, 2765,    0,    0,    0,    0,    0, 2749,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2752,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2504,    0,    0,    0,    0,    0, 3039,  347,    0,
  143,    0,    0,    0,    0,    0,    0,    0,  234,  229,
    0,    0,  159, 2755,    0,    0,    0,  295,    0,  687,
 3690,    0,    0,    0, 2757,    0,    0,    0,    0,    0,
  126,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  378,
    0, 1730,  386,  564,    0,    0, 2435, 1965,    0,    0,
  133,    0,  657,  376,  259, 1309,    0,    0, 2775,    0,
    0,    0,    0,    0, 2759,  243,    0,    0,    0, 2515,
    0, 2761,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3039,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  255,    0,    0,    0,  138, 1312, 2780,    0,
    0, 2757,    0,    0,    0,    0, 2787,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2435,  687,    0,    0,
    0,    0,    0,    0,    0, 2770,    0,    0,    0,    0,
    0,10753,    0,    0,    0,    0,    0,  259, 2783,    0,
    0,    0,    0,    0,    0, 2750, 2780,    0,    0,    0,
    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2833,    0,    0, 2834,
    0,    0,    0,    0,    0,    0,   10,    0, 2835,    2,
  300,   14, -531, 1695,  337, -187,  256, 2831, 2574,    0,
  -28,    0, 2593,  -96,  -40,    0,   73, -330,   -3,  -57,
  -22, -139, 2588, 2178,    0,    0,    0, 1525,    0,    0,
  -36, 1786, -536,   -2, -813, 2234, 2020, -297, 1791, 2786,
 -280,    0,  -60,    0, -478,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -550,   -1, 1801, -298, 1021, -913,    0,
    0,    0,    0, -333, -121,-1067, 1820, -335,  -50,    0,
 -518,    0,    0, 1450, -984,    0, 1461,    0,    0, 2549,
 -539,   -6,    0,    0,    0,    0, 2531,    0, 2311, 2582,
 -219, 1831, -690,    0,    0,    0, -705,    0,    0,    0,
    0,    0,    0, 1888,    0, 2597, 2142, 1827, 2576, 2592,
    0, 1281,    0, -513, 2128,  -35, -631, -718,    0,    0,
  -85, -929, -921, -911, -233, -907,    0, 1520, -991,    0,
 1206,-1138, 1521,    0, 2366,    0,    0,    0, 1207, 1212,
 1173,  105, -254, -135, -801,    0,    0, 2157,    0, 1705,
    0,    0,  274,    0,    0,    0,    0, -154,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1300,  -14,    0,    0,  240,    0,-1123, -511,
 1130, 1292,    0, 1193,    0,    0, 2555, -112, 1956, 1744,
 1572,    0, -988, -848, -881,    0, -494, -687, -390,    0,
 -580, 1288, 1098, -638, -523, -734, -925, -749,    0,    0,
    1,    0,    0,    0,    0,    0, 2616,    0,    0,    0,
    0, 2629, 2405,    0,    0,    0,    0,    0,   -9, 2638,
    0,    0,    0,    0,  -15,    0,    0,    0, -340, 2132,
 -327, 1697, -451,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2346,    0,    0, 2145,    0,    0,    0,    0,
    0,    0,   54, 2956,    0,    0,    0,    0,    0, 2672,
    0,    0,    0,    0, 2480,    0, 2477,    0,    0,    0,
 2238,    0,    0,    0, -951,    0,   93,    0, 2232,    0,
    0,    0,    0,    0,    0, -611, -974, -923,  681, -129,
 1532,    0, 2841, 1277,    0, 2262, -622, 2265,    0, -839,
 2021,    0,    0, -134, -683,    0, 1373,-1176,    0,-1131,
-1122,-1118, 1487,    0, 1224,    0, 1274,    0, 1178,    0,
    0,    0,    0, 2490,    0,    0,    0, -352, 2324, 2101,
 -971,    0,    0,    0, -680,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 3177, 1571, 1900,
    0, -572,    0,    0,    0, -243,    0, 2622,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -32, 1837,
    0,    0,
};
#define YYTABLESIZE 11598
static short yytable[] = {                                     124,
  146,  189,  124,  212,   79,   78,  145,  259,  151,   37,
  398,  308,  208,  278,  281,  192,  747,  572,  609,  577,
  394,  948,  279,  282,  243,  204,  217,  209,  195,  542,
  771,  610,  544,  774,  783,  247,  252,  753, 1233,  785,
  222,  966,  733,  186,  188,  191,  194,  145,  199,  248,
  229,  488, 1002,  210,  188,  215,  254,  194, 1153,  145,
  199,  679,  228,  124,  986,  219,  224,  146,  917,  907,
 1372,  239,  253,  145,  241,  188,  250,  239,  215,  191,
  194,  246,  145,  199,  228,  124, 1223,  194,  249,  251,
  265,  313,  267, 1223, 1224,  639,  949, 1407,  266,  856,
 1188, 1224,  863,  534, 1225,  847,  309,  866, 1227,  623,
 1234, 1225, 1071, 1045,  577, 1227,  210,  145,  210,  311,
  991, 1242,  408,  631,  746, 1046,  988,   77, 1192,  371,
  215,  408,  706,  289,  706,  759,  307,  194,  145,  228,
 1263,  397,  256,  151, 1051, 1280, 1019,  398,  963,  484,
  333,  509,  362,  910,  639, 1026,  277,  394,  694,  705,
  939, 1091,  707,  731,  631,  638, 1079,  631,  393,  816,
  600,  510,   89,  577,   77,  360,  759, 1336,  694,  759,
  125, 1484,  631,  256,  328,  263,  256,  759,  742,  834,
  676,  706,  531,  706,  759,  531,  761,  762,  763,  764,
  705,  256, 1501,  707,  995, 1142,  315,  263, 1066,  758,
  919,  362,  609,  639,  577,  404,  515,  694,  705,  664,
  401,  707, 1746, 1281,  638,  610,  317, 1686,  368,  600,
  975,  404,  686,  367, 1258,   77,  495,  708,  831,  125,
  404,  276, 1405,  577,  280,  498,  210, 1503,  318,   60,
  145,  268,  410,  124,  369,  122, 1504,  791,  750,  145,
 1505,  410,  321,   77,  277,  502,  674,  277,  826, 1752,
  508, 1340,  368,  577,  404,  750,  882, 1422,  397,  510,
  191,  277,  419, 1621,  145, 1154,   85,  368,  961,   85,
 1033,  759,  367,  543,  706, 1443,  708,  334,  369,  710,
  546,  326,  209,  817,  559,  393,  828,  561, 1622,   83,
  322,  130,  195,  369,  344,  573, 1447,  575,  631,  210,
  619,  364,  365, 1514,  283,  540,  210,  435,  210, 1384,
  210,  627, 1223,  564,  750,  706,  827, 1417,  541, 1385,
 1224,  629, 1133, 1134, 1102, 1148,  759, 1637,  131,  529,
 1225,  194,  199,  706, 1227,  579, 1374, 1207,  621,   97,
  145,  961,  191,  128,  673, 1477, 1623,  284, 1145, 1580,
 1103,  195, 1052,  670,  335,  710,   79,  256,  508,  673,
 1157, 1159,  882,   14,  678,  252,  474,  759,  684,  482,
  759,  345,  145,  404, 1492,  194,  410,  637,  268, 1439,
  883,  500, 1241,  145, 1185,  759,  744,  145,  529,  519,
  194, 1405,  145,  513,  579,  151,  710,  641,  256,  366,
 1660,  256,  323,  398,  830,  531,  252, 1399,  283,  252,
  122,  122,  272,  886,  710,  687,  256,  476,  757,  384,
   89,  631, 1783, 1501,  252,  474,  712,  744,  482,  811,
  744,   14,  706, 1585, 1432,  625,  637,  488, 1483, 1337,
  531, 1149,  741,  631,  390,  744, 1023, 1014,  694,  273,
 1016,  918,  697, 1436,  706, 1220,  641,  705,  689,  404,
  707, 1516,  728,  271, 1022, 1551,  965, 1552, 1503,  694,
  707,  760,  488,  707,  639,  664,  476, 1504,  384,  705,
  914, 1505,  707, 1368, 1611,  638,  883, 1389, 1448, 1578,
 1368, 1040,  600,  697,  625,  488,  697,  787,  759, 1709,
 1078,  706, 1579,  390,  694, 1286, 1303, 1010,  125, 1751,
  706,  697,  377,  694,  710,  328,  408,  813, 1494, 1435,
   76, 1274,  780,  210,  856, 1013,  705,  600,  780,  707,
  577, 1010, 1152, 1081,  887,  705,  368,  275,  707,  368,
  637,  367,  531,  253,  367,  708, 1420, 1736,  708,  812,
  694,  692,  706, 1756,  706,  600,  275,  262,  600, 1086,
 1509,  885,  369, 1064,   79,  369,  706,   76,  841,  842,
  962,  987,  209,  710,  600, 1626,  833,  692,  694,  705,
  675, 1628,  707,  730,  253, 1594,  362,  253,  639,  979,
 1778,  510,  694,  705, 1791, 1141,  707,  531,  210,  638,
  600,  637,  253,  577,  759,  631,  759,  858, 1344,  694,
  759, 1746,  888,  759,  606, 1590,  759,   79,  857, 1008,
 1009, 1434,  750,  256,  474,  710,  616,  673,   76,  637,
 1663, 1395,  326,  482,  777,  256,   85,  256,  210,  275,
  195,  410,  275,  252,  256,  252,  410, 1667,  368, 1215,
  744, 1515,  252,  367,  488,  410,   76,  708,  194, 1394,
  368,  706,  368,  215, 1513,  367,  921,  367, 1581, 1454,
  706,  708,  474,  606,  369,  476, 1564,  708,  384, 1409,
  388, 1731, 1723,  384,  956,  616,  369, 1588,  369,  384,
  508,  502,  210,  985,  194,  209,  529,  728, 1478,  401,
  998,  998, 1232,  838,  759, 1204,  744,  706,  390,  759,
  145,  154,  145, 1276, 1240,  210, 1442,  637,  601,  546,
  964,  210, 1243,  476, 1741,  210,  759,  153,  605, 1135,
 1792, 1715, 1138,  759, 1262,  256, 1223,  641, 1408,  388,
 1722, 1012,  710,  252, 1224,  210,  389, 1032, 1034, 1664,
  510,  200,  398,  625, 1225, 1665, 1205,  256, 1227,  398,
 1744, 1081, 1226,  677,  256,  252,  482,  759, 1029, 1226,
  697, 1793,  252,  210,  210,  625, 1702,  601,  395, 1031,
 1768, 1769, 1761,  697,  474,  396, 1615,  605, 1421, 1039,
 1065, 1329,  697, 1331, 1431,  216, 1799, 1800,  256,  609,
 1076, 1049,  498,  155,  677,  389,  252,  677, 1479,  145,
  392, 1061, 1062, 1063, 1656, 1139,  384,   76, 1794,  145,
 1506,  253,  677,  253, 1589, 1703, 1244,  759,  718, 1136,
  253, 1088,  710,  719,  256,  476, 1704,  395,  744,  697,
   79,  390, 1003,  575,  396, 1347,  744, 1496,  697,  508,
  609, 1140,  882, 1815,  710, 1833,  156,  361,  256,  410,
  194,  199,  410,  228,  124,  291,  252, 1137,  694,  392,
  410, 1822,  882, 1072, 1827,  744,  744,  391,  744, 1682,
  394,  694,  410,  410,  105,  744,  744,  744,  744,  744,
  694,  744,  697,  362,  744,  744, 1716,  882,  584,  488,
  292,  710,  575,  499, 1678, 1816,  697,  393, 1680,  979,
 1275,  541,  135,  106,  107, 1004,  108,  882,  966,  531,
  911,  253,  759,  109,  110,  111,  112,  113,  293,  114,
 1416,  294,  115,  116,  126,  234,  391,  694,  194,  394,
 1705,  942,  943,  253,  388,  882,  694,  295,  694,  236,
  253,  541,  541,  222,  710,  541,  606,  584,  500,  136,
  531, 1438, 1612,  531,  708, 1721,  393,  708,  616, 1245,
  541,  135,  541,  296,  736,  501,  883, 1616,  531,  244,
  531,  882,  681, 1629,  253,  681,  609, 1368,  693,  694,
  694,  606, 1329, 1633,  222,  241,  883,  222,  398,  610,
  681,  127,  541,  616, 1754, 1235, 1236,  694, 1226,  968,
  389, 1720,  222,  209, 1253, 1334, 1334, 1338,  136,  606,
  401,  883,  606,  708,  882,   79, 1254,  488, 1253,  693,
 1368,  616,  708, 1512,  616,  256,  241, 1269,  606,  210,
 1373,  883,  395,  137,  253,  737,  210,  693,  635,  396,
  616, 1382,  468,  706, 1287, 1289, 1634, 1681, 1720, 1798,
  601, 1293,  706,  798,  606,  799,  714, 1717, 1673,  883,
  605,  715,  805, 1391,  392, 1393,  616,  388,  781, 1695,
  708,  677, 1698,  255,  677,  944,  791,  488, 1496,  635,
 1177,  677,  635,  144,  677,  601,  468,  140,  677,  260,
 1189, 1805,  137,  677, 1720,  883,  677,  635,  708,  806,
 1246,  468,  410,  529,  410, 1701,  626,  241, 1763,  456,
  624,  708,  677,  601, 1360, 1586,  601, 1362,   79,   79,
  410, 1180, 1181,  605, 1437, 1146,  605, 1264, 1720,  708,
  526,  391,  601,  389,  394, 1755,  677, 1158,  883, 1191,
  677,  151,  605, 1359,  529, 1264,  998,  529,  241,  677,
  240,  624, 1701, 1797,  261,  307,  194,  708,  601,  541,
  788,  393,  529,  581,  529,  395,  677,  531,  605,  624,
  264,  526,  396,  283,  526,  541, 1265,  734,  541,  541,
  135,  541,  135,  531, 1328,  541, 1330, 1365,  541,  526,
  390,  526,  141,  677, 1268, 1701,  295,  392, 1767,  142,
  151,  541,  506,  807, 1788,  677,  268,  541,  277,  531,
  541,  965,  708,  677, 1498,  531,  915,  283,  531,  202,
  203,  222,  581,  222, 1446,   79, 1419,  376, 1450,  136,
  222,  718,  716,  510,  717,  115,  719, 1701,  681,  681,
 1701,  789,  677,  677,  105,  677,  269, 1826,  283, 1666,
 1571,  506,  677,  677,  677,  677,  677, 1724,  677, 1725,
  923,  677,  677, 1701,  391, 1182,  458,  394, 1577,  694,
  152,  708,  115,  106,  107,  487,  108,  231,  270,  541,
  135,  625,  790,  109,  110,  111,  112,  113,  274,  114,
  353,  925,  115,  116,  393,   98,  693,  681, 1485,  583,
 1765,  541, 1488,  410,  411,  326,  516,  115,  541,  693,
  508,  376,  541,  137,  232,   99, 1517,  412,  693,  541,
  531,  222,  625,  998,  682,  694,  408,  531,  565,  152,
  160,  209,  508, 1334,  694, 1668,   99,  541,  194,  566,
  625,  681,  541,  222,  136,  531,  351, 1562,  100,  353,
  222,  508,  101,  327,  508, 1247,  635,  210,  583,  872,
 1627,  529,  872,  411,   43,  693,  145,  541,  147,  508,
 1632,  541,  510,  101,  693,  531,  412, 1818,  635,  531,
  529,  529,  681,  529,  222,  408,   77,  529,  526,  160,
  529, 1746, 1248,  521,  283,  290,  669, 1773,  708,  668,
  148,  708,  522,  529,  530,  351, 1745,  526,  526,  529,
  526,  872,  398, 1633,  526,  149,  708,  526,  693,  105,
  151,  102, 1226,   43, 1614,   51,  708, 1631,  624,  708,
  526,  103,  693,  161, 1774, 1661,  526,  927,  708,  701,
  381,  151,  102,  706,  707,  709,   79, 1775,  106,  107,
  624,  108,  103, 1613,  531, 1498,  104,  708,  109,  110,
  111,  112,  113,  336,  114,  328,  708,  115,  116,  714,
  151,  508,   80,   81,  715,  723, 1634,  104,  188, 1662,
  275,  529, 1498,  329,   51,  708, 1111,   41,  532,  151,
  718,  716,  161,  717,  708,  719,  350,  382,  151, 1659,
 1638,  982, 1112,  529,  151, 1556,  533,  978,  526, 1176,
  529,  105, 1089,  956, 1090,   24,  982,  330,  145,  529,
 1675,  529,  151,  191, 1676,  456,  755, 1557,   47, 1498,
  526, 1197, 1710,  351, 1558,  352,  146,  526,  708,  529,
  106,  107,  145,  108,  529, 1198,   41, 1113,  526,  152,
  109,  110,  111,  112,  113,  306,  114,  668,  353,  115,
  116, 1691,  714,  982,  358,   49,  526,  715, 1559, 1199,
  152,  526, 1498,  529,   24, 1197, 1114,  484,  956,  210,
  983, 1498,  127,  354,  764,  755,  510,   47,  508, 1198,
  508,  824,  194, 1617,  508,  379,   25,  508,  668,  152,
  526,  668,  383,  384,  385,  316,  202,  203,  384,  319,
  508,  764, 1747, 1199, 1498,  721,  668, 1498,  152,  384,
  510,  510,  146, 1200,   49,  386,  387,  152,  145,  105,
  388,  389,  824,  152,   48,  824,  484,  865,  349, 1726,
 1498,  127,   50, 1115,  776,  145,  510,  775,  347, 1740,
  824,  152,  721,   45,  379,   25,  128,  510,  106,  107,
  510,  108,  116,  188, 1749,  721,  368, 1200,  109,  110,
  111,  112,  113,  146,  114,   79, 1750,  115,  116,  145,
 1726,  487,   46,  216,  922,  508,  145,  884,  508,  938,
  667, 1803, 1781,   48,  945,  946,  720,  718,  716,  250,
  717,   50,  719,  953,  370,  957,  958,  959,  960,  961,
  508,  953,   45,  372, 1746,  128,  487,  508,  146,  508,
  508,  116,  718,  716,  145,  717, 1726,  719,  714, 1808,
  375,  980,  145,  715, 1806,  191,  530, 1809,  380,  487,
  250,   46,  216,  250,  516,  508,  987,  202,  203,  906,
  873,  508,  905,  873,  373,  105,  508,  576,  250,  508,
 1821,  987,  191,  950,  718,  716,  667,  717,  724,  719,
 1088,  957,  980,  980,  980,  980,  980,  980,  980, 1070,
 1651,  485, 1069,  376,  106,  107,  531,  108,  191,  493,
  980,  980,  980,  980,  109,  110,  111,  112,  113,   76,
  114,  105,  873,  115,  116,  521,  338,  667,  377,  339,
  718,  716,  405,  717,  522,  719,  135,  136,  718,  716,
  532,  717,  980,  719,  980,  667, 1077, 1738,  413,  852,
  106,  107,  414,  108, 1190,  531,  216, 1189,  533,  417,
  109,  110,  111,  112,  113, 1208,  114,  418, 1069,  115,
  116,  420, 1214,  824,  340, 1213, 1700, 1092, 1093, 1097,
 1041, 1042, 1105, 1107, 1109,  422, 1117, 1120, 1123, 1125,
 1128, 1131,  123,  123,  423,  668,  531,  216,  424,  531,
  216,   52,  494,  496,  497,  668,  718,  716,  668,  717,
 1256,  719,   53, 1255,  531,  216,  506,  668,  487,   54,
  692, 1261,   14, 1700, 1260, 1161, 1162,  520, 1164,  824,
 1166,  529,  824, 1169, 1170, 1171, 1172, 1173, 1174,  824,
  718,  716,  824,  717,   55,  719,  824,  105,  537,  550,
 1178,  824,  946,  557,  824,  714,  916,  560,  920,  961,
  715,  924,  926,  928,  668, 1342, 1700,  570, 1341, 1456,
  824, 1491, 1189,  668, 1490, 1787,  106,  107, 1493,  108,
  714, 1490,  574,  624,  628,  715,  109,  110,  111,  112,
  113,  630,  114,  632,  824,  115,  116,  824,  824,  667,
  961,  961,  961,  961,  961,  973,  961,  824, 1700,   56,
  676, 1700,  662,  980, 1789,  980,  980,  668,  961,  961,
  961,  961,  714,  980,  824,  663,  664,  715,  195,  668,
 1804,  980, 1830, 1490, 1700, 1746,  980,  671,  685,  980,
 1782,   57,  688,  690,  691,   58,  824,  692, 1812, 1813,
  961,  824,  961,  980,  693,  694,  695,  696,  980,  980,
  697,  231,  980,  824,  698,  824,  699,  704,  714,  980,
  705,  980,  980,  715, 1824,  980,  714,  713,  721,  980,
  722,  715,  980,  724,  980, 1831,  980,  195, 1832,  726,
  740,  980,  980,  744,  980,  745,  749,  751,  232,  752,
  754,  521,  765,  755,  667,  766,  768,  775,  769,  953,
  980,  980,  778,  980,  667,  718,  716,  667,  717,  802,
  719, 1310, 1311,  803, 1313, 1315,  667,  980, 1737,  980,
  980,  980, 1320,  531,  980,  531, 1630,  980,  980,  531,
  804,  809,  531,  275,  714,  814,  818,  980,  775,  715,
  821,  775,  819,  820,  824,  531,  823, 1197,  829,  835,
  836,  980,  839,  487,  980,  840,  775,  849,  967,  718,
  716, 1198,  717,  667,  719, 1307,  851, 1351,  714,  670,
  980,  852,  667,  715,  860, 1358,  980,  946,  861,  862,
  946,  980, 1178,  670,  864, 1199, 1366,  946, 1369, 1370,
  900,  670,  980,  980,  946,  980,  383,  384,  385, 1463,
  901,  980,  903,  908,  912,  951,  913,  670,  940, 1817,
  974,  961,  980,  961,  961,  989,  667,  718,  716, 1186,
  717,  961,  719,  531,  388,  389,  992,  996,  667,  961,
  980,  980, 1401,  980,  961, 1010, 1015,  961,  830, 1200,
  980,  980,  980,  980,  980,  531,  980, 1020, 1021,  980,
  980,  961,  531,  961, 1023,  781,  961,  961,  531, 1027,
  961,  670, 1030, 1028, 1036,  370, 1037,  961, 1038,  961,
  961, 1044, 1047,  961, 1053, 1058,  195,  961, 1060,  370,
  961,  487,  961, 1072,  961, 1067,  531,  370, 1084,  961,
  961,  681,  961, 1085, 1099, 1144, 1147,  195, 1451,  531,
 1150, 1142,  531,  370, 1163, 1160, 1165, 1459,  961,  961,
 1167,  961, 1175, 1187, 1464, 1194, 1196,  531,  195,  531,
  176,  714,  668, 1202,  195,  961,  984,  961,  961,  961,
 1216, 1219,  961, 1211, 1237,  961,  961, 1228, 1168,  718,
  716,  487,  717,  714,  719,  961, 1239,  195,  715, 1249,
 1257, 1259,  498, 1266, 1270, 1271, 1272,  370, 1273,  961,
 1282,  946,  961, 1284, 1078,  946,  195,  984,  984,  984,
  984,  984,  984,  984,  195,  195, 1296, 1291,  961,  176,
 1295,  195, 1294, 1297,  961,  984,  984,  984,  984,  961,
 1299, 1300,  882,  498, 1304, 1305,  498,  714, 1306,  195,
  961,  961,  715,  961, 1345,  718,  716, 1308,  717,  961,
  719,  498, 1309, 1316, 1317,  775, 1318,  984,  775,  984,
 1319, 1321, 1106, 1322, 1110,  775, 1118, 1121,  775, 1126,
 1129, 1132,  775, 1323, 1324, 1325,  195,  775,  961,  961,
  775,  961, 1326, 1327, 1343, 1364, 1361,  195,  961,  961,
  961,  961,  961, 1371,  961,  714,  775,  961,  961, 1379,
  715,  989, 1599, 1600, 1346,  718,  716, 1375,  717, 1376,
  719, 1349,  718,  716, 1377,  717, 1378,  719, 1380, 1390,
  775, 1387, 1392,  775,  775, 1398,  552,  946, 1410, 1396,
  946, 1406, 1411,  775,  718,  716, 1418,  717, 1412,  719,
 1413, 1414,  989,  989,  989,  989,  989,  989,  989, 1415,
  775, 1433, 1423, 1440, 1444, 1301,  531, 1445, 1449, 1455,
  989,  989,  989,  989, 1640, 1475, 1476,  105, 1480, 1486,
 1487, 1507,  775, 1553, 1563, 1565,  531,  775,  531, 1566,
 1568, 1567,  531, 1569, 1570,  531, 1572, 1573, 1582,  775,
 1574,  775,  989, 1584,  989, 1587,  106,  107,  531,  108,
 1591, 1592, 1593,  684,  531, 1595,  109,  110,  111,  112,
  113, 1596,  114, 1597, 1598,  115,  116,  714,  176, 1601,
 1602, 1603,  715, 1604, 1605, 1606, 1674, 1607,  984, 1608,
  984,  984,  390, 1609, 1677, 1610, 1618, 1619,  984,  176,
  718,  716, 1696,  717, 1620,  719,  984,  684, 1636, 1639,
  127,  984,  718,  716,  984,  717, 1641,  719, 1642, 1647,
  176, 1643,  684, 1644, 1645, 1646,  176, 1648,  984, 1649,
  498, 1690,  498,  984,  984, 1692,  531,  984, 1654,  498,
 1655, 1657, 1658,  714,  984, 1679,  984,  984,  715,  176,
  984, 1684,  498, 1685,  984, 1650, 1711,  984,  531,  984,
 1719,  984, 1694, 1757, 1732,  531,  984,  984,  176,  984,
 1743, 1759, 1762, 1764, 1766, 1771,  531,  176, 1779, 1772,
 1490, 1785, 1786,  176, 1755,  984,  984, 1754,  984,  155,
 1802,  946,  727, 1739,  531, 1811, 1742, 1807, 1810,  531,
 1814,  176,  984,  714,  984,  984,  984, 1746,  715,  984,
  714, 1825,  984,  984, 1298,  715, 1828, 1829,    1,  631,
   29,   31,  984,  989,  202,  989,  989,  759,  531,  372,
  498,  376,  714,  989, 1777,  270,  984,  715,  176,  984,
  148,  989,  529,  268,  660,   81,  989,  465,  212,  989,
  944,  529,  498,  275,  192,  984,  404,  692,  943,  498,
  529,  984,  690,  989,  686,  688,  984,  193,  989,  989,
  372,  274,  989,  372,  288,  377,  716,  984,  984,  989,
  984,  989,  989,  286,  715,  989,  984,  222,  372,  989,
  323,  287,  989,  498,  989,  717,  989,  331,  340,  322,
  625,  989,  989,  726,  989, 1350,  718,  716,  329,  717,
  681,  719,   27,   30,   39,  984,  984,  985,  984, 1332,
  989,  989,  132,  989,  421,  984,  984,  984,  984,  984,
  411,  984, 1462,  902,  984,  984,  412,  989,  714,  989,
  989,  989,  381,  715,  989,  850, 1292,  989,  989, 1290,
  714, 1083,  245, 1575,  539,  715, 1288,  989,  985,  985,
  985,  985,  985,  985,  985, 1576,  583, 1278, 1267,  815,
  430,  989,  990, 1218,  989, 1279,  985,  985,  985,  985,
  718,  716,  525,  717,  684,  719,  195,  514,  105, 1687,
  989,  684, 1025,  554,  684, 1753,  989, 1733,  684,  382,
 1760,  989, 1555,  797, 1758, 1561,  684, 1796,  985,  555,
  985, 1018,  989,  989, 1400,  989, 1693,  106,  107, 1823,
  108,  989,  684, 1708, 1790,  666, 1209,  109,  110,  111,
  112,  113, 1383,  114, 1510, 1718,  115,  116,  562,  545,
 1430,  536, 1059,  837,  801,  195,  684, 1050,  134,  507,
  989,  989,  842,  989,  391,  735,  739,  981,  288,  390,
  989,  989,  989,  989,  989,  994,  989,  976, 1193,  989,
  989,  977, 1683, 1624,  436, 1801,  684,  277,  531, 1770,
  748,  684, 1352,  718,  716, 1834,  717,  909,  719, 1156,
  438,  665, 1312,  842,  842,  842,  842,  842, 1363,  842,
    0,    0,    0,  684,  383,  384,  385,    0,  202,  203,
    0,  842,  842,  842,  842,  684,    0,  684,  250,  531,
    0,    0,  531,    0,    0,    0,    0,  386,  387,  441,
  442,  443,  388,  389,  444,  445,    0,  531,    0,    0,
    0,    0,  447,  448,  714,  842,    0,    0,    0,  715,
  372,  452,    0,    0,    0,    0,    0,  372,    0,  250,
  372,    0,  250,    0,  372,    0,    0,  229,    0,    0,
    0,    0,  372,    0,    0,    0,    0,  250,    0,  985,
    0,  985,  985,    0,    0,    0,    0,    0,  372,  985,
    0,    0,    0,    0,    0,  669,    0,  985,    0,    0,
    0,    0,  985,    0,    0,  985,    0,    0,  229,    0,
    0,  229,  372,    0,    0,    0,    0,    0,  714,  985,
    0,    0,    0,  715,  985,  985,  229,    0,  985,    0,
    0,    0,    0,    0,    0,  985,  669,  985,  985,  669,
    0,  985,  372,    0,    0,  985,    0,    0,  985,    0,
  985,    0,  985,    0,  669,    0,    0,  985,  985,    0,
  985,    0,    0,    0,    0,  195,    0,    0,    0,  372,
  186,    0,    0,    0,    0,    0,  985,  985,    0,  985,
    0,  372,    0,  372,    0,    0,  195,    0,    0,    0,
    0,    0,    0,  985,  675,  985,  985,  985,    0,    0,
  985,    0,    0,  985,  985,    0,    0,    0,    0,  381,
    0,    0,    0,  985,  842,  195,  842,  842,    0,    0,
    0,  714,    0,    0,  842,    0,  715,  985,    0,  186,
  985,    0,  842,    0,  195,  675,    0,  842,  675,    0,
  842,    0,  195,  195,    0,    0,  985,    0,    0,  195,
    0,    0,  985,  675,  842,    0,  531,  985,  531,  842,
  842,    0,    0,  842,    0,  531,  382,  195,  985,  985,
  842,  985,  842,  842,    0,    0,  842,  985,  531,  195,
  842,  195,    0,  842,    0,  842,    0,  842,    0,    0,
    0,    0,  842,  842,    0,  842,  250,    0,  250,    0,
    0,    0,    0,    0,  195,  250,  985,  985,  854,  985,
    0,  842,  842,    0,  842,    0,  985,  985,  985,  985,
  985,    0,  985,    0,    0,  985,  985,    0,  842,  195,
  842,  842,  842,    0,    0,  842,    0,    0,  842,  842,
 1353,  718,  716,    0,  717,  241,  719,  229,  842,  854,
  854,  854,  854,  854,    0,  854,  531,    0,    0,    0,
    0,    0,  842,    0,    0,  842,    0,  854,  854,  854,
  854,  383,  384,  385,    0,  202,  203,  187,  531,    0,
    0,  842,    0,    0,    0,  531,    0,  842,    0,    0,
    0,    0,  842,    0,  386,  387,  250,    0,    0,  388,
  389,  854,    0,  842,  842,    0,  842,    0,    0,    0,
    0,    0,  842,  669,    0,    0,    0,    0,  250,  531,
    0,    0,    0,  669,    0,  250,  669,    0,  186,    0,
    0,    0,    0,    0,    0,  669,  187,    0,  692,    0,
    0,  842,  842,  857,  842,  241,    0,    0,    0,  186,
    0,  842,  842,  842,  842,  842,    0,  842,    0,  250,
  842,  842,    0,    0,    0,    0,    0,  241,    0,  230,
  186,    0,    0,    0,  241,    0,  186,    0,    0,    0,
    0,    0,  669,    0,  857,  857,  857,  857,  857,    0,
  857,  669,    0,    0, 1354,  718,  716,    0,  717,  186,
  719,    0,  857,  857,  857,  857,    0,    0,  241,    0,
  230,    0,  675,  230,    0,  675,    0,    0,  186,    0,
    0,    0,  675,    0,    0,  675,    0,  186,  230,  675,
    0,    0,    0,  186,  675,  669,  857,  675, 1355,  718,
  716,    0,  717,    0,  719,    0,    0,  669, 1356,  718,
  716,  186,  717,  675,  719,    0,    0, 1357,  718,  716,
    0,  717,    0,  719,    0,    0,    0,    0,    0,    0,
  854,    0,  854,  854,    0,    0,    0,  675,    0,  714,
  854,  675,  718,  716,  715,  717,    0,  719,  186,    0,
  675,    0,    0,  854,    0,    0,  854,    0,    0,    0,
  880,  879,  881,    0,    0,    0,    0,  675,    0,    0,
  854,    0,    0,    0,    0,  854,  854,    0,    0,  854,
    0,    0,    0,    0,    0,    0,  854,    0,  854,  854,
    0,    0,  854,    0,  675,  187,  854,    0,    0,  854,
    0,  854,    0,  854,    0,    0,  675,    0,  854,  854,
    0,  854,    0,    0,    0,    0,  187, 1481,  718,  716,
    0,  717,    0,  719,    0,    0,    0,  854,  854,  672,
  854,    0,    0,    0,    0,    0,    0,  187,    0,    0,
    0,    0,    0,  187,  854,    0,  854,  854,  854,    0,
    0,  854,    0,    0,  854,  854,    0,    0,  793,    0,
    0,    0,    0,    0,  854,  857,  187,  857,  857,    0,
  672,    0,    0,  672,    0,  857,    0,    0,  854,    0,
    0,  854,    0,    0,    0,  187,    0,    0,  672,    0,
    0,  857,    0,  714,  187,    0,    0,  854,  715,  793,
  187,    0,  793,  854,    0,  857,    0,  241,  854,  230,
  857,  857,  552,    0,  857,    0,    0,  793,  187,  854,
  854,  857,  854,  857,  857,    0,    0,  857,  854,    0,
    0,  857,    0,    0,  857,    0,  857,  714,  857,    0,
    0,    0,  715,  857,  857,    0,  857,  714,    0,    0,
    0,    0,  715,    0,    0,  187,  714,  854,  854,  858,
  854,  715,  857,  857,    0,  857,    0,  854,  854,  854,
  854,  854,  867,  854,    0,    0,  854,  854,    0,  857,
  714,  857,  857,  857,    0,  715,  857,    0,  868,  857,
  857, 1482,  718,  716,    0,  717,    0,  719,    0,  857,
  858,  858,  858,  858,  858,  180,  858,  241,    0,    0,
    0,    0,    0,  857,    0,    0,  857,    0,  858,  858,
  858,  858,    0,    0,  869,    0,    0,    0,    0,  241,
    0,    0,  857,  870,    0,    0,  241,    0,  857,    0,
    0,  871,    0,  857,    0,    0,  180,    0, 1621,  180,
    0,    0,  858,    0,  857,  857,  714,  857,    0,    0,
  872,  715,  873,  857,  180, 1489,  718,  716,    0,  717,
  241,  719,    0, 1622,    0,    0,  874,    0,  875,  876,
  877,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,  857,  857,  852,  857,    0,    0,    0,    0,
    0,    0,  857,  857,  857,  857,  857,    0,  857,    0,
    0,  857,  857,    0,  106,  107,    0,  108,    0,    0,
  793,    0,    0,    0,  109,  110,  111,  112,  113,  878,
  114, 1623,    0,  115,  116,  852,    0,  852,  852,  852,
    0,    0,    0,    0,    0,    0,    0,  672,    0,    0,
  672,    0,    0,  852,  852,  852,  852,  672,    0,    0,
  672,    0,    0,    0,  672,  105,    0,    0,    0,  672,
    0,    0,  672,    0,    0,    0,  793,    0,    0,  793,
    0,    0,    0,    0,    0,    0,  793,  852,  672,  793,
    0,    0,    0,  793,  106,  107,    0,  108,  793,    0,
    0,  793,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,  672,  115,  116,    0,  672,  793,    0,    0,
    0,  858,    0,  858,  858,  672,    0,    0,    0,    0,
  714,  858,  950,  718,  716,  715,  717,    0,  719,    0,
    0,  793,  672,    0,  793,  793,    0,  858,    0,    0,
    0,  880,  879,  881,  793,    0,    0,    0,    0,    0,
    0,  858,    0,  180,    0,    0,  858,  858,    0,  672,
  858,  793,    0,    0,    0,    0,    0,  858,    0,  858,
  858,  672,    0,  858,  180,    0,    0,  858,    0,    0,
  858,    0,  858,  793,  858,    0,    0,    0,  793,  858,
  858,    0,  858,    0,  714,  180,    0,    0,    0,  715,
  793,  180,  793,    0,    0,    0,    0,    0,  858,  858,
    0,  858,    0,    0,    0,  889,  929,  930,  931,  932,
  933,  934,  935,  936,  180,  858,    0,  858,  858,  858,
    0,    0,  858,    0,    0,  858,  858,    0,    0,  794,
    0,    0,    0,  180,    0,  858,  852,    0,  852,  852,
  558,    0,  180,    0,    0,    0,  852,    0,  180,  858,
    0,    0,  858,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  852,    0,    0,    0,  180,    0,  858,    0,
  794,    0,    0,  794,  858,    0,  852,    0,    0,  858,
    0,  852,  852,    0,    0,  852,    0,    0,  794,    0,
  858,  858,  852,  858,  852,  852,    0,    0,  852,  858,
    0,    0,  852,  180,    0,  852,    0,  852,    0,  852,
    0,    0,    0,    0,  852,  852,    0,  852,    0,    0,
  892,    0,    0,    0,    0,    0,    0,    0,  858,  858,
  851,  858,    0,  852,  852,    0,  852,  105,  858,  858,
  858,  858,  858,  867,  858,    0,    0,  858,  858,    0,
  852,  714,  852,  852,  852,    0,  715,  852,    0,  868,
  852,  852,    0,    0,    0,    0,  106,  107,    0,  108,
  852,  851,    0,  851,  851,  851,  109,  110,  111,  112,
  113,    0,  114,    0,  852,  115,  116,  852,    0,  851,
  851,  851,  851,    0,    0,  869,    0,    0,    0,    0,
    0,    0,    0,  852,  870,    0,    0,    0,    0,  852,
    0,    0,  871,  105,  852,    0,    0,    0,    0,    0,
    0,    0,    0,  851,    0,  852,  852,    0,  852,    0,
    0,  872,    0,  873,  852,    0,    0,    0,    0,    0,
    0,    0,  106,  107,    0,  108,    0,  874,    0,  875,
  876,  877,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,  852,  852,  853,  852,    0,    0,    0,
    0,    0,    0,  852,  852,  852,  852,  852,    0,  852,
    0,    0,  852,  852,    0,    0,    0,    0,    0,    0,
    0,  794,    0,    0,    0, 1452, 1453,    0,    0,    0,
  878, 1457, 1458,    0, 1460, 1461,  853,  584,  853,  853,
  853,  585, 1465, 1466, 1467, 1468,    0, 1469, 1470, 1471,
 1472, 1473, 1474,    0,  853,  853,  853,  853,    0,  586,
    0,    0,  587,   84,    0,    0,    0,    0,    0,    0,
   85,    0,    0,    0,   86,   87,    0,  794,    0,    0,
  794,    0,    0,   88,    0,    0,    0,  794,  853,    0,
  794,    0,    0,    0,  794,   89,    0,    0,    0,  794,
    0,    0,  794,  588,    0,    0,    0,    0,    0,  589,
  590,  591,    0,  592,  593,  594,    0,  595,  794,    0,
    0,    0,  851,    0,  851,  851,    0,    0,    0,    0,
    0,    0,  851,    0,    0,    0,    0,  596,    0,  597,
    0,    0,  794,    0,    0,  794,  794,    0,  851,    0,
    0,    0,    0,    0,    0,  794,    0,    0,    0,    0,
   90,    0,  851,    0,    0,    0,    0,  851,  851,    0,
  598,  851,  794,    0,    0,    0,    0,   91,  851,    0,
  851,  851,    0,    0,  851,    0,    0,  599,  851,    0,
    0,  851,   92,  851,  794,  851,   93,  402,    0,  794,
  851,  851,    0,  851,    0,    0,    0,   94,    0,  600,
    0,  794,    0,  794,    0,    0,    0,    0,    0,  851,
  851,    0,  851,  105,    0,    0,   95,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  851,    0,  851,  851,
  851,    0,    0,  851,  601,  602,  851,  851,    0,  536,
    0,    0,  106,  107,    0,  108,  851,  853,    0,  853,
  853,  603,  109,  110,  111,  112,  113,  853,  114,    0,
  851,  115,  116,  851,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  853,    0,    0,    0,    0,    0,  851,
  536,    0,    0,  536,    0,  851,    0,  853,    0,    0,
  851,    0,  853,  853,    0,    0,  853,    0,  536,    0,
  536,  851,  851,  853,  851,  853,  853,    0,    0,  853,
  851,    0,    0,  853,    0,    0,  853,    0,  853,    0,
  853,   98,    0,    0,    0,  853,  853,    0,  853,    0,
  536,    0,    0,    0,    0,    0,    0,    0,    0,  851,
  851,  856,  851,    0,  853,  853,    0,  853,    0,  851,
  851,  851,  851,  851,    0,  851,    0,    0,  851,  851,
    0,  853,   99,  853,  853,  853,    0,    0,  853,    0,
    0,  853,  853,    0,  100,    0,    0,    0,    0,    0,
    0,  853,  856,    0,  856,  856,  856,    0,  129,    0,
    1,    0,    0,    0,    0,  853,    0,    0,  853,  101,
  856,  856,  856,  856,    0,    0,    0,    0,    0,    0,
    0,    2,    0,    0,  853,    0,    0,  889,    0,    3,
  853,    0,    0,    0,    0,  853,    4,    0,    5,    0,
    6,    0,    0,    0,  856,    7,  853,  853,    0,  853,
    0,    0,    8,    0,    0,  853,    0,    0,    0,  406,
    0,    0,    0,    0,    0,    0,    9,    0,  647,    0,
    0,  453,    0,  454,    0,    0,  105,   10,  102,    0,
    0,    0,    0,    0,  853,  853,  456,  853,  103,    0,
    0,  277,    0,    0,  853,  853,  853,  853,  853,    0,
  853,  890,    0,  853,  853,  106,  107,  536,  108,    0,
    0,    0,    0,  104,    0,  109,  110,  111,  112,  113,
    0,  114,    0,  536,  115,  116,  536,  536,    0,  536,
    0,  105,    0,  536,    0,    0,  536,  891,    0,    0,
    0,    0,  892,    0,   11,    0,    0,    0,  893,  536,
    0,   12,    0,    0,   13,  536,   14,   15,  536,  105,
  106,  107,    0,  108,    0,    0,    0,  894,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,   16,    0,    0,    0,    0,    0,    0,  106,  107,
    0,  108,  105,    0,    0,    0,    0,    0,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,    0,
    0,    0,    0,  856,    0,  856,  856,    0,    0,    0,
    0,  106,  107,  856,  108,    0,    0,  536,    0,    0,
   17,  109,  110,  111,  112,  113,    0,  114,    0,  856,
  115,  116,    0,    0,    0,   18,   19,    0,    0,  536,
    0,    0,    0,  856,    0,    0,  536,    0,  856,  856,
  536,    0,  856,    0,    0,    0,    0,  536,    0,  856,
    0,  856,  856,    0,    0,  856,    0,    0,    0,  856,
    0,    0,  856,    0,  856,  536,  856,    0,    0,    0,
  536,  856,  856,    0,  856,  426,    0,    0,  647,    0,
    0,  453,    0,  454,  427,    0,    0,    0,    0,    0,
  856,  856,    0,  856,    0,  536,  456,  428,    0,  536,
  640,  277,    0,  381,  429,    0,  573,  856,    0,  856,
  856,  856,    0,    0,  856,    0,    0,  856,  856,    0,
    0,    0,  641,    0,    0,    0,    0,  856,    0,    0,
    0,  430,    0,    0,  431,  573,  573,    0,  573,    0,
    0,  856,    0,    0,  856,  573,  573,  573,  573,  573,
    0,  573,    0,    0,  573,  573,    0,  542,    0,    0,
  856,    0,  432,  433,  280,    0,  856,    0,    0,    0,
    0,  856,    0,    0,  434,    0,    0,    0,  435,    0,
    0,    0,  856,  856,    0,  856,  642,    0,    0,    0,
    0,  856,    0,  280,  280,    0,  280,  542,  542,    0,
    0,  542,    0,  280,  280,  280,  280,  280,    0,  280,
    0,    0,  280,  280,    0,    0,  542,    0,  542,    0,
  856,  856,    0,  856,    0,    0,    0,    0,    0,  643,
  856,  856,  856,  856,  856,  436,  856,    0,    0,  856,
  856,  437,    0,   62,    0,    0,    0,    0,  542,    0,
    0,  438,    0,    0,   63,    0,    0,    0,    0,    0,
    0,   64,    0,    0,    0,  383,  384,  385,  439,  202,
  203,    0,    0,    0,   65,    0,    0,    0,    0,  440,
    0,    0,    0,    0,    0,    0,    0,    0,  386,  387,
  441,  442,  443,  388,  389,  444,  445,  106,  446,    0,
  108,    0,    0,  447,  448,  426,  449,  450,  451,  111,
  112,  113,  452,  114,  427,    0,  115,  116,  644,  645,
  646,    0,    0,    0,    0,  647,    0,  428,  453,    0,
  454,    0,    0,  381,  429,   66,    0,    0,    0,    0,
    0,    0,    0,  456,    0,    0,    0,    0,  277,    0,
    0,   67,  641,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,    0,  431,    0,    0,    0,   68,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   69,    0,    0,    0,   70,    0,    0,
   71,    0,  432,  433,    0,    0,    0,    0,   72,    0,
    0,  559,    0,    0,  434,    0,    0,    0,  435,    0,
    0,    0,    0,    0,    0,  542,  642,   73,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  542,    0,    0,  542,  542,    0,  542,    0,    0,
    0,  542,  559,    0,  542,  559,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  708,    0,  542,    0,  643,
  559,    0,  559,  542,    0,  436,  542,    0,    0,    0,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  438,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  559,    0,    0,  383,  384,  385,  439,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  440,
    0,    0,    0,    0,    0,    0,    0,    0,  386,  387,
  441,  442,  443,  388,  389,  444,  445,  106,  446,    0,
  108,    0,    0,  447,  448,  542,  449,  450,  451,  111,
  112,  113,  452,  114,  549,    0,  115,  116,  644,  645,
  646,    0,  426,    0,    0,    0,  533,  542,    0,    0,
    0,  427,    0,    0,  542,    0,    0,    0,  542,    0,
    0,    0,    0,    0,  428,  542,    0,    0,    0,    0,
  381,  429,    0,    0,    0,  549,  432,    0,  549,    0,
    0,    0,    0,  542,    0,    0,    0,  533,  542,  641,
  533,    0,    0,  549,    0,  549,    0,    0,  430,    0,
    0,  431,    0,    0,    0,  533,    0,  533,    0,    0,
    0,    0,    0,  542,    0,    0,    0,  542,    0,    0,
    0,    0,    0,    0,    0,  549,    0,    0,    0,  432,
  433,    0,    0,    0,    0,    0,    0,  533,    0,    0,
    0,  434,    0,    0,    0,  435,    0,    0,    0,  559,
    0,    0,    0,  642,  651,    0,  651,  651,    0,  651,
    0,    0,    0,    0,    0,  559,    0,    0,    0,  559,
    0,  559,  651,    0,    0,  559,    0,  651,  559,    0,
   76,    0,  439,    0,    0,    0,    0,    0,    0,    0,
    0,  559,    0,  440,    0,    0,  643,  559,    0,    0,
  559,    0,  436,    0,    0,    0,    0,    0,  437,    0,
    0,  106,  446,    0,  108,    0,    0,    0,  438,    0,
  449,  450,  451,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  383,  384,  385,  439,  202,  203,    0,    0,
    0,    0,    0,    0,    0,    0,  440,    0,    0,    0,
    0,    0,    0,    0,    0,  386,  387,  441,  442,  443,
  388,  389,  444,  445,  106,  446,    0,  108,    0,  559,
  447,  448,    0,  449,  450,  451,  111,  112,  113,  452,
  114,    0,    0,  115,  116,  644,  645,  646,    0,    0,
    0,  559,  549,    0,    0,    0,    0,   98,  559,    0,
    0,    0,  559,    0,  533,    0,    0,    0,  549,  559,
    0,    0,  549,    0,  549,    0,    0,    0,  549,    0,
  533,  549,    0,    0,  533,    0,  533,  559,    0,    0,
  533,    0,  559,  533,  549,    0,    0,    0,   99,    0,
  549,    0,    0,  549,    0,    0,  533,    0,    0,    0,
  100,    0,  533,    0,    0,  533,    0,  559,    0,    0,
    0,  559,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  651,  101,    0,    0,    0,    0,
    0,  651,    0,    0,    0,    0,    0,    0,    0,    0,
  651,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  651,    0,    0,    0,    0,    0,  651,
  651,    0,  549,  645,    0,  645,  645,  651,  645,    0,
    0,    0,    0,    0,  533,    0,    0,    0,    0,    0,
    0,  645,    0,    0,  549,    0,  645,  651,    0,    0,
  651,  549,    0,    0,  102,  549,  533,    0,    0,    0,
    0,    0,  549,  533,  103,    0,    0,  533,    0,    0,
    0,    0,    0,    0,  533,    0,  651,    0,  651,  651,
  549,    0,    0,    0,    0,  549,    0,    0,    0,  104,
  651,    0,  533,    0,  651,    0,    0,  533,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
  549,    0,    0,    0,  549,    0,    0,    0,    0,    0,
  455,    0,  533,  453,    0,  454,  533,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  106,  107,  456,  108,
    0,    0,    0,  277,    0,    0,  109,  110,  111,  112,
  113,  651,  114,    0,    0,  115,  116,  651,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  651,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  651,  651,  651,  651,  651,  651,    0,    0,    0,
    0,    0,    0,    0,    0,  651,    0,    0,    0,    0,
    0,    0,    0,    0,  651,  651,  651,  651,  651,  651,
  651,  651,  651,  651,  651,    0,  651,    0,    0,  651,
  651,    0,  651,  651,  651,  651,  651,  651,  651,  651,
    0,    0,  651,  651,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  645,    0,    0,    0,    0,    0,    0,
  645,    0,    0,    0,    0,    0,    0,    0,    0,  645,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  645,    0,    0,    0,    0,    0,  645,  645,
    0,    0,    0,    0,    0,    0,  645,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  645,    0,    0,  645,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  818,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  645,    0,  645,  645,    0,
 1094,    0,    0, 1095,    0,    0,    0,  426,    0,  645,
    0,    0,    0,  645,    0,    0,  427,    0,    0,    0,
    0,    0,    0,    0,    0,  818,    0,  455,  818,  428,
  453,    0,  454,    0,    0,  381,  429,    0,    0,    0,
    0,    0,    0,  818,    0,  456,    0,    0,    0,    0,
  277,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  430,    0,    0,  431,    0,    0,    0,
  645,    0,    0,    0,    0,    0,  645,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  645,    0,    0,    0,
    0,    0,    0,    0,  432,  433,    0,    0,    0,    0,
  645,  645,  645,  645,  645,  645,  434,    0,    0,    0,
  435,    0,    0,    0,  645,    0,    0,    0,    0,    0,
    0,    0,    0,  645,  645,  645,  645,  645,  645,  645,
  645,  645,  645,  645,    0,  645,    0,    0,  645,  645,
    0,  645,  645,  645,  645,  645,  645,  645,  645,    0,
    0,  645,  645,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1096,    0,  436,    0,    0,
    0,    0,    0,  437,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  438,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  383,  384,  385,
  439,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  440,    0,    0,    0,    0,    0,    0,    0,    0,
  386,  387,  441,  442,  443,  388,  389,  444,  445,  106,
  446,    0,  108,    0,    0,  447,  448,    0,  449,  450,
  451,  111,  112,  113,  452,  114,  818, 1104,  115,  116,
 1095,    0,    0,    0,  426,    0,  133,    0,    0,    0,
  133,    0,    0,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  455,    0,  428,  453,  133,  454,
    0,  133,  381,  429,    0,    0,    0,    0,    0,    0,
    0,    0,  456,    0,    0,    0,    0,  277,    0,    0,
    0,    0,  818,    0,    0,  818,    0,    0,    0,    0,
  430,    0,  818,  431,    0,  818,    0,    0,    0,  818,
    0,    0,  133,    0,  818,    0,    0,  818,  133,  133,
  133,    0,  133,  133,  133,    0,  133,    0,    0,    0,
    0,  432,  433,  818,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  434,    0,    0,  133,  435,  133,  455,
    0,    0,  453,    0,  454,    0,    0,  818,    0,    0,
  818,  818,    0,    0,    0,    0,    0,  456,    0,    0,
  818,    0,  277,    0,    0,    0,    0,    0,    0,  133,
    0,    0,    0,    0,    0,    0,    0,  818,    0,    0,
    0,    0,    0,    0,    0,    0,  133,    0,    0,    0,
    0,    0, 1096,    0,  436,    0,    0,    0,    0,  818,
  437,    0,    0,    0,  818,    0,    0,    0,  133,    0,
  438,    0,    0,    0,    0,    0,  818,    0,  818,    0,
    0,    0,    0,    0,  383,  384,  385,  439,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  440,    0,
    0,    0,    0,  133,  133,    0,    0,  386,  387,  441,
  442,  443,  388,  389,  444,  445,  106,  446,    0,  108,
  133,    0,  447,  448,    0,  449,  450,  451,  111,  112,
  113,  452,  114,    0, 1108,  115,  116, 1095,    0,    0,
    0,  426,    0,    0,    0,    0,    0,    0,    0,    0,
  427,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  428,    0,    0,  105,    0,    0,  381,
  429,    0,    0,    0,    0,  159,  160,  161,  162,  163,
  164,  165,  166,  167,  168,  169,  170,  171,  172,  173,
  174,    0,    0,  175,  176,  106,  107,  430,  108,    0,
  431,  177,  178,    0,    0,  109,  110,  111,  112,  113,
  179,  114,  180,    0,  115,  116,  181,  182,  183, 1116,
    0,    0, 1095,    0,    0,    0,  426,    0,  432,  433,
    0,    0,    0,    0,    0,  427,    0,    0,    0,    0,
  434,    0,    0,    0,  435,    0,  455,    0,  428,  453,
    0,  454,    0,    0,  381,  429,    0,    0,    0,    0,
    0,    0,    0,    0,  456,    0,    0,    0,    0,  277,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  430,    0,    0,  431,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1096,
    0,  436,    0,    0,    0,    0,    0,  437,    0,    0,
    0,    0,    0,  432,  433,    0,    0,  438,    0,    0,
    0,    0,    0,    0,    0,  434,    0,    0,    0,  435,
    0,  383,  384,  385,  439,  202,  203,    0,    0,    0,
    0,    0,    0,    0,    0,  440,    0,    0,    0,    0,
    0,    0,    0,    0,  386,  387,  441,  442,  443,  388,
  389,  444,  445,  106,  446,    0,  108,    0,    0,  447,
  448,    0,  449,  450,  451,  111,  112,  113,  452,  114,
    0,    0,  115,  116, 1096,    0,  436,    0,    0,    0,
    0,    0,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  438,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  383,  384,  385,  439,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  440,    0,    0,    0,    0,    0,    0,    0,    0,  386,
  387,  441,  442,  443,  388,  389,  444,  445,  106,  446,
    0,  108,    0,    0,  447,  448,    0,  449,  450,  451,
  111,  112,  113,  452,  114,    0, 1119,  115,  116, 1095,
    0,    0,    0,  426,    0,    0,    0,    0,    0,    0,
    0,    0,  427,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  455,    0,  428,  453,    0,  454,    0,
    0,  381,  429,    0,    0,    0,    0,    0,    0,    0,
    0,  456,    0,    0,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  430,
    0,    0,  431,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  432,  433,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  434,    0,    0,    0,  435,    0,  455,    0,
    0,  453,    0,  454,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  456,    0,    0,    0,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1096,    0,  436,    0,    0,    0,    0,    0,  437,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  438,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  383,  384,  385,  439,  202,  203,    0,
    0,    0,    0,    0,    0,    0,    0,  440,    0,    0,
    0,    0,    0,    0,    0,    0,  386,  387,  441,  442,
  443,  388,  389,  444,  445,  106,  446,    0,  108,    0,
    0,  447,  448,    0,  449,  450,  451,  111,  112,  113,
  452,  114,    0, 1124,  115,  116, 1095,    0,    0,    0,
  426,    0,    0,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  428,    0,    0,    0,    0,    0,  381,  429,
    0,  797,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  430,    0,    0,  431,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  797,    0,    0,  797,    0,    0, 1127,    0,
    0, 1095,    0,    0,    0,  426,    0,  432,  433,    0,
  797,    0,    0,    0,  427,    0,    0,    0,    0,  434,
    0,    0,    0,  435,    0,  455,    0,  428,  453,    0,
  454,    0,    0,  381,  429,    0,    0,    0,    0,    0,
    0,    0,    0,  456,    0,    0,    0,    0,  277,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  430,    0,    0,  431,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1096,    0,
  436,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,  432,  433,    0,    0,  438,    0,    0,    0,
    0,    0,    0,    0,  434,    0,    0,    0,  435,    0,
  383,  384,  385,  439,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  440,    0,    0,    0,    0,    0,
    0,    0,    0,  386,  387,  441,  442,  443,  388,  389,
  444,  445,  106,  446,    0,  108,    0,    0,  447,  448,
    0,  449,  450,  451,  111,  112,  113,  452,  114,    0,
    0,  115,  116, 1096,    0,  436,    0,    0,    0,    0,
    0,  437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  438,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  795,    0,    0,    0,  383,  384,  385,  439,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  440,
    0,    0,    0,  797,    0,    0,    0,    0,  386,  387,
  441,  442,  443,  388,  389,  444,  445,  106,  446,    0,
  108,    0,  795,  447,  448,  795,  449,  450,  451,  111,
  112,  113,  452,  114,    0, 1130,  115,  116, 1095,    0,
  795,    0,  426,    0,    0,  455,    0,  486,  453,    0,
  454,  427,    0,    0,    0,    0,    0,    0,    0,  797,
    0,    0,  797,  456,  428,    0,    0,    0,  277,  797,
  381,  429,  797,    0,    0,    0,  797,    0,    0,    0,
    0,  797,    0,    0,  797,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  430,    0,
  797,  431,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  797,    0,    0,  797,  797,  432,
  433,    0,    0,    0,    0,    0,    0,  797,    0,    0,
    0,  434,    0,  455,    0,  435,  453,    0,  454,    0,
    0,    0,    0,    0,  797,    0,    0,    0,    0,    0,
    0,  456,    0,    0,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  797,    0,    0,    0,
    0,  797,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  797,    0,  797,    0,    0,    0,    0,
 1096,    0,  436,    0,    0,    0,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  438,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  383,  384,  385,  439,  202,  203,    0,    0,
    0,    0,    0,  795,    0,    0,  440,    0,    0,    0,
    0,    0,    0,    0,    0,  386,  387,  441,  442,  443,
  388,  389,  444,  445,  106,  446,    0,  108,    0,    0,
  447,  448,  426,  449,  450,  451,  111,  112,  113,  452,
  114,  427,    0,  115,  116,    0,    0,    0,    0,    0,
    0,    0,  796,    0,  428,    0,    0,    0,    0,  795,
  381,  429,  795,    0,    0,    0,    0,    0,    0,  795,
    0,    0,  795,    0,    0,    0,  795,    0,    0,    0,
    0,  795,    0,    0,  795,    0,    0,    0,  430,    0,
    0,  431,    0,  796,    0,    0,  796,    0,    0,    0,
  795,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  796,    0,    0,    0,    0,    0,  382,    0,  432,
  433,    0,    0,    0,  795,    0,    0,  795,  795,    0,
  426,  434,    0,    0,    0,  435,    0,  795,    0,  427,
    0,    0,  955,    0,  955,  955,    0,  955,    0,    0,
    0,    0,  428,    0,  795,    0,    0,    0,  381,  429,
  955,    0,    0,    0,    0,  955,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  795,    0,    0,    0,
    0,  795,    0,    0,    0,    0,  430,    0,    0,  431,
    0,    0,  436,  795,    0,  795,    0,    0,  437,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  438,    0,
    0,    0,    0,    0,    0,  382,    0,  432,  433,    0,
    0,    0,  383,  384,  385,  439,  202,  203,    0,  434,
    0,    0,    0,  435,    0,    0,  440,    0,    0,    0,
    0,    0,    0,    0,    0,  386,  387,  441,  442,  443,
  388,  389,  444,  445,  106,  446,    0,  108,    0,    0,
  447,  448,    0,  449,  450,  451,  111,  112,  113,  452,
  114,    0,    0,  115,  116,    0,    0,    0,    0,    0,
  708,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  436,    0,    0,    0,  651,    0,  437,  651,    0,  651,
    0,    0,    0,    0,    0,    0,  438,    0,    0,    0,
    0,    0,  651,    0,  796,    0,    0,  651,    0,    0,
  383,  384,  385,  439,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  440,    0,    0,    0,    0,    0,
    0,    0,    0,  386,  387,  441,  442,  443,  388,  389,
  444,  445,  106,  446,    0,  108,    0,    0,  447,  448,
    0,  449,  450,  451,  111,  112,  113,  452,  114,  955,
  796,  115,  116,  796,    0,    0,    0,    0,  955,    0,
  796,    0,    0,  796,    0,    0,    0,  796,    0,    0,
    0,  955,  796,    0,    0,  796,    0,  955,  955,    0,
    0,    0,    0,  455,    0,    0,  453,    0,  454,    0,
    0,  796,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  456,    0,    0,    0,  955,  277,    0,  955,    0,
    0,    0,    0,    0,    0,  796,    0,    0,  796,  796,
    0,    0,    0,    0,    0,    0,    0,    0,  796,    0,
    0,    0,    0,    0,  955,    0,  955,  955,    0,    0,
    0,    0,    0,    0,    0,  796,    0,    0,  955,    0,
    0,    0,  955,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  796,    0,    0,
    0,    0,  796,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  796,    0,  796,    0,    0,    0,
    0,    0,    0,    0,  651,    0,    0,    0,    0,    0,
    0,  651,    0,    0,    0,    0,    0,    0,    0,  955,
  651,    0,    0,    0,    0,  955,    0,    0,    0,    0,
    0,    0,    0,  651,    0,  955,    0,    0,    0,  651,
  651,    0,    0,    0,    0,    0,    0,  651,    0,  955,
  955,  955,  955,  955,  955,    0,    0,    0,    0,    0,
    0,    0,    0,  955,    0,    0,    0,  651,    0,    0,
  651,    0,  955,  955,  955,  955,  955,  955,  955,  955,
  955,  955,  955,    0,  955,    0,    0,  955,  955,    0,
  955,  955,  955,  955,  955,  955,  955,  955,  651,  651,
  955,  955,    0,    0,    0,    0,    0,    0,    0,    0,
  651,    0,    0,    0,  651,    0,    0,    0,    0,    0,
  426,    0,    0,  455,    0,    0,  453,    0,  454,  427,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  456,  428,    0,    0,    0,  277,    0,  381,  429,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  651,    0,    0,    0,    0,  430,  651,    0,  431,
    0,    0,    0,    0,    0,    0,    0,  651,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  651,  651,  651,  651,  651,  651,  432,  433,    0,
    0,    0,    0,    0,    0,  651,    0,    0,    0,  434,
    0,    0,    0,  435,  651,  651,  651,  651,  651,  651,
  651,  651,  651,  651,  651,    0,  651,    0,    0,  651,
  651,    0,  651,  651,  651,  651,  651,  651,  651,  651,
    0,    0,  651,  651,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  436,    0,    0,    0,    0,    0,  437,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,    0,    0,    0,
    0,    0,    0,    0,  700,    0,    0,    0,    0,    0,
  383,  384,  385,  439,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  440,    0,    0,    0,    0,    0,
    0,    0,    0,  386,  387,  441,  442,  443,  388,  389,
  444,  445,  106,  446,    0,  108,    0,    0,  447,  448,
  426,  449,  450,  451,  111,  112,  113,  452,  114,  427,
    0,  115,  116,  455,    0,    0,  453,    0,  454,    0,
    0,    0,  428,    0,    0,    0,    0,    0,  381,  429,
    0,  456,    0,    0,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  826,    0,    0,  430,    0,    0,  431,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  455,  952,    0,
  453,    0,  454,    0,    0,    0,    0,  432,  433,    0,
    0,    0,    0,    0,  826,  456,    0,  826,    0,  434,
  277,    0,    0,  435,    0,    0,    0,    0,    0,    0,
    0,    0,  826,  461,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  461,  461,  461,  461,  461,  461,  461,
  461,  461,  461,  461,  461,  461,  461,  461,    0,    0,
  461,  461,  461,  461,    0,  461,    0,    0,  461,  461,
  708,    0,  461,  461,  461,  461,  461,  461,  461,  461,
  436,  461,  461,  461,  461,  461,  437,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  438,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  383,  384,  385,  439,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  440,    0,    0,    0,    0,    0,
    0,    0,    0,  386,  387,  441,  442,  443,  388,  389,
  444,  445,  106,  446,    0,  108,    0,    0,  447,  448,
    0,  449,  450,  451,  111,  112,  113,  452,  114,    0,
  426,  115,  116,    0,    0,    0,    0,    0,    0,  427,
    0,    0,    0,  790,    0,    0,    0,    0,    0,    0,
    0,    0,  428,    0,    0,    0,    0,    0,  381,  429,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  790,    0,  430,  790,    0,  431,
    0,    0,    0,    0,  426,    0,    0,    0,    0,    0,
    0,    0,  790,  427,    0,  826,    0,  455,    0,    0,
  453,    0,  454,    0,    0,  382,  428,  432,  433,    0,
    0,    0,  381,  429,    0,  456,    0,    0,    0,  434,
  277,    0,    0,  435,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  430,    0,    0,  431,    0,    0,    0,    0,    0,    0,
    0,  826,    0,    0,  826,    0,    0,    0,    0,    0,
    0,  826,    0,    0,  826,    0,    0,    0,  826,    0,
    0,  432,  433,  826,    0,    0,  826,    0,    0,    0,
  436,    0,    0,  434,    0,    0,  437,  435,    0,    0,
    0,    0,  826,    0,    0,    0,  438,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  383,  384,  385,  439,  202,  203,  826,    0,    0,  826,
  826,    0,    0,    0,  440,    0,    0,    0,    0,  826,
    0,    0,    0,  386,  387,  441,  442,  443,  388,  389,
  444,  445,  106,  446,  436,  108,  826,    0,  447,  448,
  437,  449,  450,  451,  111,  112,  113,  452,  114,    0,
  438,  115,  116,    0,    0,    0,    0,    0,  826,    0,
    0,    0,    0,  826,  383,  384,  385,  439,  202,  203,
    0,    0,    0,    0,    0,  826,    0,  826,  440,    0,
    0,    0,    0,    0,    0,  790,    0,  386,  387,  441,
  442,  443,  388,  389,  444,  445,  106,  446,    0,  108,
    0,    0,  447,  448,    0,  449,  450,  451,  111,  112,
  113,  452,  114,    0,  426,  115,  116,  455,    0,    0,
  453,    0,  454,  427,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  456,  428,    0,    0,    0,
  277,  790,  381,  429,  790,    0,    0,    0,    0,    0,
    0,  790,    0,    0,  790,    0,    0,    0,  790,    0,
    0,    0,    0,  790,    0,    0,  790,    0,    0,    0,
  430,    0,    0,  431,    0,    0,    0,    0,    0,    0,
    0,  455,  790,    0,  453,    0,  454,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  456,
    0,  432,  433,    0,  277,    0,  790,    0,    0,  790,
  790,    0,    0,  434,    0,    0,    0,  435,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  790,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  790,    0,
    0,    0,    0,  790,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  436,  790,    0,  790,    0,    0,
  437,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  438,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1122,    0,    0,  383,  384,  385,  439,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  440,    0,
    0,    0,    0,    0,    0,    0,    0,  386,  387,  441,
  442,  443,  388,  389,  444,  445,  106,  446,    0,  108,
    0,    0,  447,  448,  426,  449,  450,  451,  111,  112,
  113,  452,  114,  427,    0,  115,  116,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  428,    0,    0,    0,
    0,    0,  381,  429,    0,    0,    0,    0,    0,    0,
  220,    0,    0,    0,  220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  220,    0,
  430,    0,  220,  431,    0,  220,    0,    0,  426,    0,
    0,    0,    0,    0,    0,    0,    0,  427,    0,    0,
    0,  455,    0,    0,  453,    0,  454,    0,    0,    0,
  428,  432,  433,    0,    0,    0,  381,  429,    0,  456,
 1689,    0,    0,  434,  277,    0,  220,  435,    0,    0,
    0,    0,  220,  220,  220,    0,  220,  220,  220,    0,
  220,    0,    0,    0,  430,    0,    0,  431,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,    0,  220,    0,    0, 1734,    0,    0,  453,    0,
  454,    0,    0,    0, 1301,  432,  433,    0,    0,    0,
    0,    0,    0,  456,  436,    0,    0,  434,  277,    0,
  437,  435,    0,  220,    0,    0,    0,    0,   80,    0,
  438,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,    0,    0,    0,  383,  384,  385,  439,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  440,    0,
    0,    0,  220,    0,    0,    0,    0,  386,  387,  441,
  442,  443,  388,  389,  444,  445,  106,  446,  436,  108,
    0,    0,  447,  448,  437,  449,  450,  451,  111,  112,
  113,  452,  114,    0,  438,  115,  116,  220,  220,    0,
    0,    0,    0,    0,    0, 1314,    0,    0,  383,  384,
  385,  439,  202,  203,  220,    0,    0,    0,    0,    0,
    0,    0,  440,    0,    0,    0,    0,    0,    0,    0,
    0,  386,  387,  441,  442,  443,  388,  389,  444,  445,
  106,  446,    0,  108,    0,    0,  447,  448,    0,  449,
  450,  451,  111,  112,  113,  452,  114,    0,  426,  115,
  116,    0,    0,    0,    0,    0,    0,  427,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  428,    0,    0,    0,    0,    0,  381,  429,    0,    0,
    0,    0,    0,    0,  853,    0,    0,    0,  585,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  854,    0,  430,    0,  586,  431,    0,  587,
    0,    0,  426,    0,    0,  455,    0,    0,  453,    0,
  454,  427,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  456,  428,  432,  433,    0,  277,    0,
  381,  429,    0,    0,    0,    0,    0,  434,    0,    0,
  588,  435,    0,    0,    0,    0,  589,  590,  591,    0,
  592,  593,  594,    0,  595,    0,    0,    0,  430,    0,
    0,  431,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  596,    0,  597,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  382,  955,  432,
  433,  955,    0,  955,    0,    0,    0,    0,  436,    0,
    0,  434,    0,    0,  437,  435,  955,  598,    0,    0,
    0,  955,    0,    0,  438,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  599,    0,    0,    0,  383,  384,
  385,  439,  202,  203,    0,    0,    0,    0,    0,    0,
    0,    0,  440,    0,    0,    0,  600,    0,    0,    0,
    0,  386,  387,  441,  442,  443,  388,  389,  444,  445,
  106,  446,  436,  108,    0,    0,  447,  448,  437,  449,
  450,  451,  111,  112,  113,  452,  114,    0,  438,  115,
  116,  601,  602,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  383,  384,  385,  439,  202,  203,  603,    0,
    0,    0,    0,    0,    0,    0,  440,    0,    0,    0,
    0,    0,    0,    0,    0,  386,  387,  441,  442,  443,
  388,  389,  444,  445,  106,  446,    0,  108,    0,    0,
  447,  448,  426,  449,  450,  451,  111,  112,  113,  452,
  114,  427,  791,  115,  116,    0,    0,    0,    0,    0,
    0,    0,    0,  792,  428,    0,    0,    0,    0,    0,
  381,  429,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  791,    0,    0,  791,    0,  430,    0,
    0,  431,    0,    0,  792,    0,    0,  792,    0,    0,
    0,  791,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  792,    0,    0,  955,  825,    0,    0,  432,
  433,    0,    0,    0,  955,    0,    0,    0,    0,    0,
    0,  434,    0,    0,    0,  435,  819,  955,    0,    0,
    0,    0,    0,  955,  955,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  825,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  955,    0,    0,  955,  825,    0,  819,    0,    0,
  819,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  436,    0,    0,  819,    0,    0,  437,  827,
    0,    0,  955,  955,    0,    0,    0,    0,  438,    0,
    0,    0,    0,    0,  955,    0,    0,    0,  955,    0,
    0,    0,  383,  384,  385,  439,  202,  203,    0,    0,
    0,    0,    0,    0,    0,    0,  440,    0,    0,    0,
  827,    0,    0,  827,  828,  386,  387,  441,  442,  443,
  388,  389,  444,  445,  106,  446,    0,  108,  827,    0,
  447,  448,    0,  449,  450,  451,  111,  112,  113,  452,
  114,    0,    0,  115,  116,  955,    0,    0,    0,    0,
    0,  955,    0,    0,    0,  828,    0,    0,  828,    0,
    0,  955,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  828,  791,  955,  955,  955,  955,  955,
  955,    0,    0,    0,    0,  792,    0,    0,    0,  955,
    0,    0,    0,    0,    0,    0,    0,    0,  955,  955,
  955,  955,  955,  955,  955,  955,  955,  955,  955,    0,
  955,    0,    0,  955,  955,  816,  955,  955,  955,  955,
  955,  955,  955,  955,    0,    0,  955,  955,    0,    0,
  791,    0,    0,  791,    0,    0,    0,    0,    0,    0,
  791,  792,    0,  791,  792,    0,    0,  791,  825,    0,
    0,  792,  791,    0,  792,  791,  816,    0,  792,  816,
    0,    0,    0,  792,    0,    0,  792,    0,  819,    0,
    0,  791,    0,    0,  816,    0,    0,    0,    0,  820,
    0,    0,  792,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  791,    0,    0,  791,  791,
    0,    0,    0,    0,  825,    0,  792,  825,  791,  792,
  792,    0,    0,    0,  825,    0,    0,  825,    0,  792,
  820,  825,    0,  820,  819,  791,  825,  819,    0,  825,
    0,  827,    0,    0,  819,    0,  792,  819,  820,    0,
    0,  819,  829,    0,    0,  825,  819,  791,    0,  819,
    0,    0,  791,    0,    0,    0,    0,    0,  792,    0,
    0,    0,    0,  792,  791,  819,  791,    0,    0,  825,
    0,    0,  825,  825,    0,  792,  828,  792,    0,    0,
    0,    0,  825,  829,    0,    0,  829,  827,    0,  819,
  827,    0,  819,  819,    0,    0,    0,  827,    0,  825,
  827,  829,  819,    0,  827,    0,    0,    0,    0,  827,
    0,    0,  827,    0,    0,    0,    0,    0,    0,  819,
    0,  825,  817,    0,    0,    0,  825,    0,  827,    0,
    0,    0,  828,    0,    0,  828,    0,    0,  825,    0,
  825,  819,  828,  821,    0,  828,  819,    0,    0,  828,
    0,    0,  827,    0,  828,  827,  827,  828,  819,    0,
  819,    0,    0,  817,    0,  827,  817,    0,    0,    0,
    0,    0,    0,  828,    0,    0,    0,  816,    0,    0,
    0,  817,  827,    0,  821,    0,    0,  821,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  828,    0,    0,
  828,  828,  821,    0,  827,    0,    0,    0,    0,  827,
  828,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  827,    0,  827,    0,    0,    0,  828,    0,    0,
    0,    0,    0,  816,    0,    0,  816,    0,    0,    0,
    0,  820,    0,  816,    0,    0,  816,    0,    0,  828,
  816,    0,    0,    0,  828,  816,    0,    0,  816,    0,
    0,    0,    0,    0,    0,    0,  828,    0,  828,    0,
    0,    0,    0,    0,  816,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  820,  816,    0,
  820,  816,  816,    0,  829,    0,    0,  820,    0,    0,
  820,  816,    0,    0,  820,    0,    0,    0,    0,  820,
    0,    0,  820,    0,    0,    0,    0,    0,  816,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  820,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,    0,    0,  816,    0,    0,    0,    0,
  829,    0,  820,  829,    0,  820,  820,  816,    0,  816,
  829,    0,    0,  829,    0,  820,    0,  829,    0,    0,
    0,    0,  829,    0,  817,  829,    0,    0,    0,    0,
    0,    0,  820,    0,    0,    0,    0,    0,    0,    0,
    0,  829,    0,    0,    0,  821,    0,    0,    0,    0,
    0,    0,    0,    0,  820,    0,    0,    0,    0,  820,
    0,    0,    0,    0,    0,  829,    0,    0,  829,  829,
    0,  820,    0,  820,    0,    0,    0,    0,  829,    0,
  817,    0,    0,  817,    0,    0, 1398,    0,    0,    0,
  817,    0,    0,  817,    0,  829,    0,  817,    0,    0,
    0,  821,  817,    0,  821,  817,    0,    0,    0,    0,
    0,  821,    0,  320,  821,    0,    6,  829,  821,    0,
    0,  817,  829,  821,    0,    0,  821, 1518, 1519,    0,
 1520,    0,    0, 1521,  829,    0,  829,    0,    0,    0,
    0,    0,  821,  320,    0,  817,    0, 1522,  817,  817,
  320,    0,    0, 1523,  320,  320,    0,  320,  817,    0,
  320,    0,    0,    0, 1398,    0,  821,    0,    0,  821,
  821,    0,    0,    0,  320,  817,    0,    0,    0,  821,
  320,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    6,    0,  821,  817,    0,    0,
    0,    0,  817,    0,    0, 1518, 1519,    0, 1520,    0,
    0, 1521,    0, 1524,  817,    0,  817,    0,  821,    0,
    0,    0,    0,  821,    0, 1522,    0,    0,    0,    0,
    0, 1523,   14,    0,    0,  821,    0,  821,    0,    0,
  320,    0,    0,    0,    0,    0,    0, 1525,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,  320,
    0,    0,    0,    0,    0,    0, 1650,    0, 1526,    0,
    0,    0,    0,    0,  320,  105,    0,    0,    0,    0,
    0,    0,    0,    0,  320,    0,  584,    0,    0,    0,
  585, 1524,    0,  320,    0,  320,    0,  781,    0,    0,
    0,    0,  320,    0, 1527,  107,    0,  108,  586,    0,
   14,  587,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115, 1528, 1525,    0,    0,    0,    0,
    0,  320,  320,    0,  320,   16,    0,    0,    0,    0,
    0,  320,  320,  320,  320,  320, 1526,  320,    0,    0,
  320,  320,  588,  105,    0,    0,    0,    0,  589,  590,
  591,    0,  592,  593,  594,    0,  595,    0,    0,    0,
    0,    0,    0,    0,    0,  584,    0,    0,    0,  585,
    0,    0, 1527,  107,    0,  108,  596,    0,  597,    0,
    0,    0,  109,  110,  111,  112,  113,  586,  114,    0,
  587,  115, 1528,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  598,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  599,    0,    0,    0,
    0,  588,    0,    0,    0,    0,    0,  589,  590,  591,
    0,  592,  593,  594,    0,  595,    0,    0,  600,    0,
    0,    0,    0,  220,    0,    0,    0,  220,    0,    0,
    0,    0,    0,  105,    0,  596,    0,  597,    0,    0,
    0,  220,    0,    0,    0,  220,    0,    0,  220,    0,
    0,    0,    0,  601,  602,    0,    0,    0,    0,    0,
    0,    0,  106,  107,    0,  108,    0,    0,  598,    0,
  603,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115,  116,    0,    0,  599,    0,    0,    0,  220,
    0,    0,    0,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,    0,    0,    0,  600,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  105,  220,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  601,  602,    0,    0,    0,    0,    0,    0,
    0,  106,  107,    0,  108,    0,  220,    0,    0,  603,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,    0,  220,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  220,  220,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  220,
};
static short yycheck[] = {                                       9,
   16,   55,   12,   64,    4,    4,   16,  104,   18,    0,
  254,  146,   63,  135,  136,   56,  511,  351,  359,  355,
  254,  705,  135,  136,   85,   62,   67,   63,   57,  328,
  544,  359,  330,  547,  553,   89,   94,  516, 1023,  553,
   69,  722,  494,   53,   54,   55,   56,   57,   58,   90,
   73,  285,  758,   63,   64,   65,   97,   67,  907,   69,
   70,  414,   72,   73,  745,   68,   70,   83,  691,  681,
 1194,   81,   95,   83,   84,   85,   92,   87,   88,   89,
   90,   88,   92,   93,   94,   95, 1016,   97,   91,   93,
  126,  149,  128, 1023, 1016,  376,  708, 1236,  127,  636,
  949, 1023,  642,  323, 1016,  629,  147,  647, 1016,  364,
 1024, 1023,  847,   44,    0, 1023,  126,  127,  128,  148,
  752, 1035,  262,    0,   44,  816,  749,   45,  968,  226,
  140,  271,    0,  140,    0,    0,  146,  147,  148,  149,
 1054,  254,    0,  153,   45, 1071,  778,  391,  721,  279,
  187,  306,    0,  685,    0,  787,   63,  391,    0,    0,
  700,   41,    0,   45,   41,    0,   40,   44,  254,   40,
    0,  306,   41,   59,   45,  212,   41, 1149,   41,   44,
    0,   41,   59,   41,   59,   44,   44,  523,   44,   40,
   44,   59,   41,   59,   59,   44,  530,  531,  532,  533,
   41,   59, 1379,   41,  755,   44,  153,   44,  840,   40,
   42,   59,  553,   59,    0,    0,  313,   59,   59,   44,
  256,   59,   44, 1072,   59,  553,  260,   61,    0,   59,
  725,  260,  420,    0, 1048,   45,  290,    0,   91,   59,
  269,   40, 1234,  579,   40,  283,  256, 1379,  156,  455,
  260,  264,  262,  263,    0,  286, 1379,  556,  513,  269,
 1379,  271,  258,   45,   63,  294,  406,   63,  277,   44,
  306, 1153,   44,   59,   59,    0,  262, 1262,  391,  414,
  290,   63,  269,  343,  294,  908,   41,   59,  397,   44,
  804,   44,   59,  329,    0, 1284,   59,  257,   44,   41,
  336,   59,  338,  584,  340,  391,  605,  343,  368,  320,
  306,   12,    0,   59,  257,  352,  274,  354,  372,  329,
  361,  217,  268, 1391,  260,  287,  336,  436,  338,  493,
  340,  368, 1262,  343,   59,   41,  345, 1251,  300,  503,
 1262,  370,  882,  883,  352,  320,    0,  313,   12,    0,
 1262,  361,  362,   59, 1262,    0, 1196,  989,  362,  402,
  370,  470,  372,  273,  405, 1340,  426,  303,  900, 1437,
  378,   59,  824,  402,  334,    0,  376,    0,  414,  420,
  912,  913,  262,  417,  413,    0,    0,   41,  417,    0,
   44,  334,  402,  422, 1369,  405,  406,    0,  411, 1281,
  386,  439, 1034,  413,  944,   59,    0,  417,   59,  317,
  420, 1403,  422,  309,   59,  425,   41,    0,   41,  365,
 1559,   44,  418,  667,  277,  274,   41, 1229,  260,   44,
  461,  462,  133,  667,   59,  422,   59,    0,  269,    0,
  309,  318,  264, 1620,   59,   59,  456,   41,   59,  571,
   44,  417,  318, 1442, 1268,    0,   59,  691,  318, 1150,
  309,  436,  318,  340,    0,   59,  300,  766,  331,  133,
  769,  303,    0, 1275,  340, 1015,   59,  318,  425,  264,
  318, 1395,  492,  320,  783, 1399,   42, 1401, 1620,  331,
  328,  528,  726,  331,  340,  320,   59, 1620,   59,  340,
  688, 1620,  340, 1187, 1476,  340,  386, 1213,  466, 1435,
 1194,  810,  342,   41,   59,  749,   44,  553,  383, 1658,
  394,  387, 1436,   59,  387, 1076, 1099,  458,  348,  304,
  396,   59,  452,  396,    0,  410,  676,  574, 1378, 1274,
  458, 1065,  552,  553, 1081,  765,  387,  377,  558,  387,
  436,  458,  905,  851,  667,  396,  328,  458,  396,  331,
  431,  328,  411,    0,  331,  328, 1257, 1691,  331,  572,
    0,  343,  440, 1712,  440,  405,  458,  436,  408,  860,
 1382,  667,  328,  838,  584,  331,  454,  458,  625,  626,
  720,  746,  628,   59,  424, 1509,  447,  343,  440,  440,
  454, 1515,  440,  485,   41, 1454,  454,   44,  454,  731,
 1734,  746,  454,  454, 1753,  454,  454,  466,  628,  454,
  450,  431,   59,  409,  278,  502,  280,  637, 1160,   59,
  383,   44,  668,  287,    0, 1449,  501,  637,  637,  761,
  762, 1273,  367,  501,  258,  387,    0,  688,  458,  431,
 1564,   41,  410,  264,  550,  278,  411,  280,  668,  458,
  348,  671,  458,  278,  287,  280,  676, 1581,  440, 1003,
  264,   41,  287,  440,  908,  685,  458,  440,  688, 1219,
  452,  387,  454,  693, 1390,  452,  693,  454, 1438, 1301,
  396,  454,  306,   59,  440,  258,   41,   41,  259, 1239,
    0, 1686, 1677,  264,  714,   59,  452,  355,  454,  270,
  746,  740,  722,  742,  724,  751,  367,  727, 1341,  755,
  757,  758, 1021,  619,  378,  405,  320,   41,  264,  383,
  740,   59,  742, 1067, 1033,  745,  260,  340,    0,  775,
  296,  751,  338,  306,   41,  755,  400,  506,    0,  889,
  377, 1665,  892,  407, 1053,  378, 1686,  340, 1237,   59,
 1674,  764,  387,  378, 1686,  775,    0,  803,  804, 1571,
  905,  409, 1016,  318, 1686, 1577,  456,  400, 1686, 1023,
   41, 1079, 1016,    0,  407,  400,  397,  441,  790, 1023,
  318,  418,  407,  803,  804,  340,  263,   59,    0,  802,
 1724, 1725, 1716,  331,  418,    0, 1490,   59, 1260,  809,
  839, 1142,  340, 1144, 1266,  261, 1768, 1769,  441,  405,
  849,  821,  283,  387,   41,   59,  441,   44, 1347,  839,
    0,  834,  835,  836, 1553,  893,  397,  458,  465,  849,
 1380,  278,   59,  280,  492,  312,  442,  501,   42,  890,
  287,  861,  318,   47,   40,  418,  323,   59,  452,  387,
  860,  397,  356,    0,   59, 1163,  460, 1379,  396,  905,
  456,  894,  262,  300,  340, 1827,  440,  402,  501,  889,
  890,  891,  892,  893,  894,  342,  501,  891,  318,   59,
  900,  304,  262,  417, 1818,  489,  490,    0,  492, 1618,
    0,  331,  912,  913,  460,  499,  500,  501,  502,  503,
  340,  505,  440,  438,  508,  509, 1666,  262,    0, 1153,
  377,  387,   59,  384, 1612,  352,  454,    0, 1616, 1051,
 1066,    0,    0,  489,  490,  429,  492,  262, 1619,    0,
  685,  378, 1278,  499,  500,  501,  502,  503,  405,  505,
 1249,  408,  508,  509,  273,  458,   59,  387,  968,   59,
  427,  307,  308,  400,  264,  262,  396,  424,    0,  322,
  407,   40,   41,    0,  440,   44,  342,   59,  439,    0,
   41, 1280, 1477,   44,  328, 1673,   59,  331,  342,  301,
   59,   59,   61,  450,  377,  456,  386, 1492,   59,  322,
   61,  262,   41, 1517,  441,   44, 1347, 1691,    0,   41,
  440,  377, 1343,  445,   41,  278,  386,   44, 1262, 1347,
   59,  340,   91,  377,  301, 1027, 1028,   59, 1262,   40,
  264, 1670,   59, 1069, 1044, 1148, 1149, 1150,   59,  405,
 1076,  386,  408,  387,  262, 1045, 1045, 1281, 1058,   41,
 1734,  405,  396, 1387,  408,   40,  319, 1060,  424, 1069,
 1195,  386,  264,    0,  501,  448, 1076,   59,    0,  264,
  424, 1207,    0,  387, 1076, 1078,  508, 1617, 1717, 1767,
  342, 1084,  396,  465,  450,  467,  280, 1668, 1583,  386,
  342,  285,  301, 1215,  264, 1217,  450,  397,  284, 1639,
    0,  318, 1642,  401,  321,  451, 1405, 1341, 1620,   41,
  435,  328,   44,   16,  331,  377,   44,  325,  335,  383,
   44,   40,   59,  340, 1763,  386,  343,   59,    0,  338,
  442,   59, 1142,    0, 1144, 1647,  348,  400, 1719,   58,
    0,   41,  359,  405, 1177, 1444,  408, 1180, 1148, 1149,
 1160,  307,  308,  405, 1276,  900,  408,   44, 1797,   59,
    0,  264,  424,  397,  264,  442,  383,  912,  386,   93,
  387,    0,  424, 1176,   41,   44, 1213,   44,  441,  396,
   83,   41, 1694, 1764,  383, 1195, 1196,   59,  450,  258,
  319,  264,   59,    0,   61,  397,  413,  258,  450,   59,
   44,   41,  397,  260,   44,  274,   93,  405,  277,  278,
  278,  280,  280,  274, 1142,  284, 1144,  435,  287,   59,
   45,   61,  430,  440,   93, 1737,  424,  397, 1723,  437,
   59,  300,   44,  442, 1746,  452,  385,  306,   63,  300,
  309,   42,    0,  460, 1379,  306,  303,  260,  309,  461,
  462,  278,   59,  280, 1291, 1255, 1255,  278, 1295,  280,
  287,   42,   43, 1398,   45,  265,   47, 1779,  307,  308,
 1782,  400,  489,  490,  460,  492,  383, 1817,  260, 1578,
 1416,   93,  499,  500,  501,  502,  503, 1678,  505, 1680,
  303,  508,  509, 1805,  397,  451,  276,  397, 1434,  331,
    0,   59,  302,  489,  490,  285,  492,  265,  383,  378,
  378,    0,  441,  499,  500,  501,  502,  503,  436,  505,
    0,  303,  508,  509,  397,  260,  318,  417, 1361,    0,
 1721,  400, 1365, 1343,    0,  258,  316,  337,  407,  331,
    0,  278,  411,  280,  302,  301, 1397,    0,  340,  418,
  411,  378,   41, 1390,  444,  387,    0,  418,  259,   59,
    0, 1397, 1398, 1476,  396,   40,  301,  436, 1378,  270,
   59,  410,  441,  400,  494,  436,    0, 1406,  313,   59,
  407,   41,  338,  306,   44,  301,  318, 1397,   59,   41,
 1512,  258,   44,   59,    0,  387, 1406,  466,  402,   59,
  402,  470, 1537,  338,  396,  466,   59, 1798,  340,  470,
  277,  278,  451,  280,  441,   59,   45,  284,  258,   59,
  287,   44,  338,  348,  260,  337,   41,  337,  328,   44,
  434,  331,  357,  300,  279,   59,   59,  277,  278,  306,
  280,   93, 1686,  445,  284,  449,  318,  287,  440,  460,
  279,  407, 1686,   59, 1487,    0,  328, 1518,  318,  331,
  300,  417,  454,    0,  374, 1562,  306,  303,  340,  449,
  295,  300,  407,  453,  454,  455, 1476,  387,  489,  490,
  340,  492,  417, 1483,  329, 1620,  442,  387,  499,  500,
  501,  502,  503,   40,  505,  418,  396,  508,  509,  280,
  329, 1537,  316,  317,  285,  485,  508,  442, 1518, 1563,
  458,  378, 1647,  436,   59,  387,  272,    0,  363,  348,
   42,   43,   59,   45,  396,   47,  356,  352,  357, 1558,
 1521,   46,  288,  400,  363,  378,  381,  485,  378,  320,
  407,  460,  862, 1553,  864,    0,   61,  470, 1558,  416,
 1587,  418,  381, 1563, 1591,   58,    0,  400,    0, 1694,
  400,  321, 1659,  393,  407,  395, 1582,  407,  440,  436,
  489,  490, 1582,  492,  441,  335,   59,  333,  418,  279,
  499,  500,  501,  502,  503,  418,  505,    0,  418,  508,
  509, 1632,  280,  403,  264,    0,  436,  285,  441,  359,
  300,  441, 1737,  470,   59,  321,  362,    0, 1618, 1619,
  420, 1746,    0,  443,  417,   59, 1751,   59,  278,  335,
  280,    0, 1632,  383,  284,    0,    0,  287,   41,  329,
  470,   44,  457,  458,  459,  274,  461,  462,  259,  318,
  300,  444, 1703,  359, 1779,  337,   59, 1782,  348,  270,
 1785, 1786, 1668,  413,   59,  480,  481,  357, 1668,  460,
  485,  486,   41,  363,    0,   44,   59,  647,  461, 1685,
 1805,   59,    0,  429,   41, 1685, 1811,   44,  424, 1694,
   59,  381,  374,    0,   59,   59,    0, 1822,  489,  490,
 1825,  492,    0, 1703, 1704,  387,  316,  413,  499,  500,
  501,  502,  503, 1719,  505, 1705, 1705,  508,  509, 1719,
 1726,  691,    0,    0,  694, 1751, 1726,   41,  378,  699,
   44, 1775, 1737,   59,  704,  705,   41,   42,   43,    0,
   45,   59,   47,  713,  318,  715,  716,  717,  718,  719,
  400,  721,   59,  337,   44,   59,  726,  407, 1764, 1785,
 1786,   59,   42,   43, 1764,   45, 1772,   47,  280,   59,
  236,    0, 1772,  285, 1779, 1775,  279, 1782,  244,  749,
   41,   59,   59,   44,  754, 1811,   46,  461,  462,   41,
   41,  441,   44,   44,  460,  460, 1822,  300,   59, 1825,
 1805,   61, 1802,   41,   42,   43,    0,   45,  320,   47,
 1810,   40,   41,   42,   43,   44,   45,   46,   47,   41,
 1537,  280,   44,  336,  489,  490,  329,  492, 1828,  288,
   59,   60,   61,   62,  499,  500,  501,  502,  503,  458,
  505,  460,   93,  508,  509,  348,  258,   41,  452,  261,
   42,   43,  402,   45,  357,   47,  493,  494,   42,   43,
  363,   45,   91,   47,   93,   59,   41,   59,  340,   44,
  489,  490,   40,  492,   41,    0,    0,   44,  381,  320,
  499,  500,  501,  502,  503,   41,  505,  318,   44,  508,
  509,  402,   41,  262,  306,   44, 1647,  867,  868,  869,
  391,  392,  872,  873,  874,  383,  876,  877,  878,  879,
  880,  881,  461,  462,  383,  318,   41,   41,  506,   44,
   44,  294,  436,  361,  450,  328,   42,   43,  331,   45,
   41,   47,  305,   44,   59,   59,  434,  340,  908,  312,
  343,   41,  417, 1694,   44,  915,  916,  442,  918,  318,
  920,  357,  321,  923,  924,  925,  926,  927,  928,  328,
   42,   43,  331,   45,  337,   47,  335,  460,  287,  411,
  940,  340,  942,  469,  343,  280,  690,   44,  692,    0,
  285,  695,  696,  697,  387,   41, 1737,   61,   44,   41,
  359,   41,   44,  396,   44, 1746,  489,  490,   41,  492,
  280,   44,  372,  286,   40,  285,  499,  500,  501,  502,
  503,  264,  505,   40,  383,  508,  509,  386,  387,   44,
   41,   42,   43,   44,   45,   46,   47,  396, 1779,  402,
   44, 1782,  461,  262, 1751,  264,  265,  440,   59,   60,
   61,   62,  280,  272,  413,  461,  461,  285,    0,  452,
   41,  280,   41,   44, 1805,   44,  285,  436,  320,  288,
  340,  434,  402,   40,   40,  438,  435,   40, 1785, 1786,
   91,  440,   93,  302,   40,   40,   40,   40,  307,  308,
   40,  265,  311,  452,   40,  454,   40,   40,  280,  318,
   40,  320,  321,  285, 1811,  324,  280,   40,   91,  328,
   46,  285,  331,  320,  333, 1822,  335,   59, 1825,   44,
   44,  340,  341,  428,  343,   46,   61,   40,  302,  264,
   44,  348,   44,  382,  318,  278,  278,    0,  300, 1099,
  359,  360,  264,  362,  328,   42,   43,  331,   45,  397,
   47, 1111, 1112,  436, 1114, 1115,  340,  376,  340,  378,
  379,  380, 1122,  278,  383,  280,   59,  386,  387,  284,
  470,  397,  287,  458,  280,  277,  447,  396,   41,  285,
   40,   44,  447,  399,   40,  300,  349,  321,   91,   40,
   40,  410,  318, 1153,  413,  264,   59,  383,   41,   42,
   43,  335,   45,  387,   47,  311,  411, 1167,  280,  321,
  429,   44,  396,  285,  389, 1175,  435, 1177,  382,   40,
 1180,  440, 1182,  335,   40,  359, 1186, 1187, 1188, 1189,
  436,  343,  451,  452, 1194,  454,  457,  458,  459,  311,
  464,  460,  317,   40,  320,   41,  320,  359,  451,  383,
  452,  262,  448,  264,  265,  264,  440,   42,   43,   44,
   45,  272,   47,  378,  485,  486,  502,  454,  452,  280,
  489,  490, 1232,  492,  285,  458,   40,  288,  277,  413,
  499,  500,  501,  502,  503,  400,  505,  274,   40,  508,
  509,  302,  407,  304,  300,  284,  307,  308,    0,  344,
  311,  413,  278,  344,  386,  321,  386,  318,  386,  320,
  321,  418,  416,  324,  300,  418,  258,  328,   40,  335,
  331, 1281,  333,  417,  335,  356,  441,  343,   40,  340,
  341,  417,  343,  274,   40,   44,  385,  279, 1298,   41,
  367,   44,   44,  359,  264,  320,   41, 1307,  359,  360,
   44,  362,  320,   44, 1314,   40,   44,   59,  300,   61,
    0,  280,   44,  328,  306,  376,    0,  378,  379,  380,
  266,   40,  383,  505,  501,  386,  387,  299,   41,   42,
   43, 1341,   45,  280,   47,  396,   40,  329,  285,  264,
  422,   41,    0,   58,   41,   41,   41,  413,  264,  410,
  454, 1361,  413,  440,  394, 1365,  348,   41,   42,   43,
   44,   45,   46,   47,  356,  357,   41,  309,  429,   59,
  309,  363,  492,   41,  435,   59,   60,   61,   62,  440,
   40,   40,  262,   41,  352,   40,   44,  280,   40,  381,
  451,  452,  285,  454,   41,   42,   43,   40,   45,  460,
   47,   59,   40,   40,   40,  318,   40,   91,  321,   93,
   40,   40,  872,   40,  874,  328,  876,  877,  331,  879,
  880,  881,  335,   40,   40,   40,  418,  340,  489,  490,
  343,  492,   40,  327,   44,  308,  435,  429,  499,  500,
  501,  502,  503,   41,  505,  280,  359,  508,  509,  274,
  285,    0, 1462, 1463,   41,   42,   43,  388,   45,  388,
   47,   41,   42,   43,  388,   45,  343,   47,  331,  390,
  383,  422,  391,  386,  387,  271,  287, 1487,  386,  446,
 1490,  407,  386,  396,   42,   43,   41,   45,  386,   47,
  386,  386,   41,   42,   43,   44,   45,   46,   47,  386,
  413,   41,  378,  278,   40,  417,  258,   41,   41,   41,
   59,   60,   61,   62, 1524,  385,  436,  460,   41,  308,
  435,  396,  435,  280,  337,  442,  278,  440,  280,  338,
  301,  442,  284,  338,  301,  287,   41,   41,  320,  452,
  352,  454,   91,  385,   93,  371,  489,  490,  300,  492,
  371,   41,   41,    0,  306,   41,  499,  500,  501,  502,
  503,   41,  505,   41,   41,  508,  509,  280,  258,   41,
   41,   41,  285,   41,   41,   41, 1586,   41,  262,   41,
  264,  265,   45,   41, 1594,   41,  280,   46,  272,  279,
   42,   43,   44,   45,   44,   47,  280,   44,   59,   40,
  340,  285,   42,   43,  288,   45,   59,   47,   40,  340,
  300,   59,   59,   59,   59,   59,  306,   59,  302,  308,
  278, 1631,  280,  307,  308, 1635,  378,  311,   59,  287,
   41,  352,  344,  280,  318,   41,  320,  321,  285,  329,
  324,  368,  300,   40,  328,  451,  383,  331,  400,  333,
   44,  335,  340,  383,   59,  407,  340,  341,  348,  343,
   59,  383,   41,  343,   41,   41,  418,  357,  410,   44,
   44,  435,  304,  363,  442,  359,  360,  301,  362,  387,
   40, 1691,  264, 1693,  436,  304, 1696,   59,  293,  441,
  463,  381,  376,  280,  378,  379,  380,   44,  285,  383,
  280,  307,  386,  387,  262,  285,   44,   41,    0,  337,
  383,  383,  396,  262,   40,  264,  265,  264,  470,    0,
  378,  278,  280,  272, 1734,  264,  410,  285,  418,  413,
  458,  280,  389,  264,  320,  411,  285,  470,   40,  288,
  451,  309,  400,  271,  458,  429,  264,  343,  435,  407,
  436,  435,  343,  302,  343,  343,  440,  458,  307,  308,
   41,  271,  311,   44,  308,   41,   40,  451,  452,  318,
  454,  320,  321,  308,   40,  324,  460,   59,   59,  328,
   59,  308,  331,  441,  333,   41,  335,   59,  304,   59,
   41,  340,  341,   41,  343,   41,   42,   43,   59,   45,
   44,   47,    0,    0,    0,  489,  490,    0,  492, 1145,
  359,  360,   12,  362,  271,  499,  500,  501,  502,  503,
  263,  505,  262,  676,  508,  509,  264,  376,  280,  378,
  379,  380,  295,  285,  383,  632, 1081,  386,  387, 1079,
  280,  852,   87, 1424,  326,  285, 1076,  396,   41,   42,
   43,   44,   45,   46,   47, 1425,  356, 1068, 1058,  579,
  323,  410,  751, 1006,  413, 1069,   59,   60,   61,   62,
   42,   43,  321,   45,  321,   47,    0,  311,  460, 1629,
  429,  328,  785,  338,  331, 1710,  435,   59,  335,  352,
 1714,  440, 1403,  558, 1713, 1405,  343, 1755,   91,  338,
   93,  775,  451,  452, 1230,  454, 1637,  489,  490, 1810,
  492,  460,  359, 1652, 1752,  391,  991,  499,  500,  501,
  502,  503, 1209,  505, 1383, 1668,  508,  509,  343,  331,
 1264,  324,  831,  618,  560,   59,  383,  823,   13,  298,
  489,  490,    0,  492,   40,  496,  500,  740,  138,   45,
  499,  500,  501,  502,  503,  754,  505,  726,  968,  508,
  509,  727, 1620, 1507,  427, 1772,  413,   63,    0, 1726,
  511,  418,   41,   42,   43, 1828,   45,  684,   47,  909,
  443,  390, 1113,   41,   42,   43,   44,   45, 1182,   47,
   -1,   -1,   -1,  440,  457,  458,  459,   -1,  461,  462,
   -1,   59,   60,   61,   62,  452,   -1,  454,    0,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,   -1,   59,   -1,   -1,
   -1,   -1,  495,  496,  280,   93,   -1,   -1,   -1,  285,
  321,  504,   -1,   -1,   -1,   -1,   -1,  328,   -1,   41,
  331,   -1,   44,   -1,  335,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   59,   -1,  262,
   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,  359,  272,
   -1,   -1,   -1,   -1,   -1,    0,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   41,   -1,
   -1,   44,  383,   -1,   -1,   -1,   -1,   -1,  280,  302,
   -1,   -1,   -1,  285,  307,  308,   59,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   41,  320,  321,   44,
   -1,  324,  413,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   59,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,  279,   -1,   -1,   -1,  440,
    0,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,  452,   -1,  454,   -1,   -1,  300,   -1,   -1,   -1,
   -1,   -1,   -1,  376,    0,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,  295,
   -1,   -1,   -1,  396,  262,  329,  264,  265,   -1,   -1,
   -1,  280,   -1,   -1,  272,   -1,  285,  410,   -1,   59,
  413,   -1,  280,   -1,  348,   41,   -1,  285,   44,   -1,
  288,   -1,  356,  357,   -1,   -1,  429,   -1,   -1,  363,
   -1,   -1,  435,   59,  302,   -1,  278,  440,  280,  307,
  308,   -1,   -1,  311,   -1,  287,  352,  381,  451,  452,
  318,  454,  320,  321,   -1,   -1,  324,  460,  300,  393,
  328,  395,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,  278,   -1,  280,   -1,
   -1,   -1,   -1,   -1,  418,  287,  489,  490,    0,  492,
   -1,  359,  360,   -1,  362,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,   -1,  376,  443,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   41,   42,   43,   -1,   45,  278,   47,  280,  396,   41,
   42,   43,   44,   45,   -1,   47,  378,   -1,   -1,   -1,
   -1,   -1,  410,   -1,   -1,  413,   -1,   59,   60,   61,
   62,  457,  458,  459,   -1,  461,  462,    0,  400,   -1,
   -1,  429,   -1,   -1,   -1,  407,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,  480,  481,  378,   -1,   -1,  485,
  486,   93,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  460,  318,   -1,   -1,   -1,   -1,  400,  441,
   -1,   -1,   -1,  328,   -1,  407,  331,   -1,  258,   -1,
   -1,   -1,   -1,   -1,   -1,  340,   59,   -1,  343,   -1,
   -1,  489,  490,    0,  492,  378,   -1,   -1,   -1,  279,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  441,
  508,  509,   -1,   -1,   -1,   -1,   -1,  400,   -1,    0,
  300,   -1,   -1,   -1,  407,   -1,  306,   -1,   -1,   -1,
   -1,   -1,  387,   -1,   41,   42,   43,   44,   45,   -1,
   47,  396,   -1,   -1,   41,   42,   43,   -1,   45,  329,
   47,   -1,   59,   60,   61,   62,   -1,   -1,  441,   -1,
   41,   -1,  318,   44,   -1,  321,   -1,   -1,  348,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,  357,   59,  335,
   -1,   -1,   -1,  363,  340,  440,   93,  343,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,  452,   41,   42,
   43,  381,   45,  359,   47,   -1,   -1,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,  383,   -1,  280,
  272,  387,   42,   43,  285,   45,   -1,   47,  418,   -1,
  396,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,
   60,   61,   62,   -1,   -1,   -1,   -1,  413,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,  440,  258,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,  452,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,  279,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,  359,  360,    0,
  362,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,
   -1,   -1,   -1,  306,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,  396,  262,  329,  264,  265,   -1,
   41,   -1,   -1,   44,   -1,  272,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,   -1,  348,   -1,   -1,   59,   -1,
   -1,  288,   -1,  280,  357,   -1,   -1,  429,  285,   41,
  363,   -1,   44,  435,   -1,  302,   -1,  278,  440,  280,
  307,  308,  287,   -1,  311,   -1,   -1,   59,  381,  451,
  452,  318,  454,  320,  321,   -1,   -1,  324,  460,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,  280,  335,   -1,
   -1,   -1,  285,  340,  341,   -1,  343,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  418,  280,  489,  490,    0,
  492,  285,  359,  360,   -1,  362,   -1,  499,  500,  501,
  502,  503,  272,  505,   -1,   -1,  508,  509,   -1,  376,
  280,  378,  379,  380,   -1,  285,  383,   -1,  288,  386,
  387,   41,   42,   43,   -1,   45,   -1,   47,   -1,  396,
   41,   42,   43,   44,   45,    0,   47,  378,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,   59,   60,
   61,   62,   -1,   -1,  324,   -1,   -1,   -1,   -1,  400,
   -1,   -1,  429,  333,   -1,   -1,  407,   -1,  435,   -1,
   -1,  341,   -1,  440,   -1,   -1,   41,   -1,  343,   44,
   -1,   -1,   93,   -1,  451,  452,  280,  454,   -1,   -1,
  360,  285,  362,  460,   59,   41,   42,   43,   -1,   45,
  441,   47,   -1,  368,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,    0,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,   -1,  489,  490,   -1,  492,   -1,   -1,
  262,   -1,   -1,   -1,  499,  500,  501,  502,  503,  429,
  505,  426,   -1,  508,  509,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   59,   60,   61,   62,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,  460,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   93,  359,  331,
   -1,   -1,   -1,  335,  489,  490,   -1,  492,  340,   -1,
   -1,  343,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,  383,  508,  509,   -1,  387,  359,   -1,   -1,
   -1,  262,   -1,  264,  265,  396,   -1,   -1,   -1,   -1,
  280,  272,   41,   42,   43,  285,   45,   -1,   47,   -1,
   -1,  383,  413,   -1,  386,  387,   -1,  288,   -1,   -1,
   -1,   60,   61,   62,  396,   -1,   -1,   -1,   -1,   -1,
   -1,  302,   -1,  258,   -1,   -1,  307,  308,   -1,  440,
  311,  413,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,  452,   -1,  324,  279,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,  435,  335,   -1,   -1,   -1,  440,  340,
  341,   -1,  343,   -1,  280,  300,   -1,   -1,   -1,  285,
  452,  306,  454,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   -1,   -1,   -1,  328,  472,  473,  474,  475,
  476,  477,  478,  479,  329,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,    0,
   -1,   -1,   -1,  348,   -1,  396,  262,   -1,  264,  265,
  287,   -1,  357,   -1,   -1,   -1,  272,   -1,  363,  410,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  288,   -1,   -1,   -1,  381,   -1,  429,   -1,
   41,   -1,   -1,   44,  435,   -1,  302,   -1,   -1,  440,
   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   59,   -1,
  451,  452,  318,  454,  320,  321,   -1,   -1,  324,  460,
   -1,   -1,  328,  418,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
    0,  492,   -1,  359,  360,   -1,  362,  460,  499,  500,
  501,  502,  503,  272,  505,   -1,   -1,  508,  509,   -1,
  376,  280,  378,  379,  380,   -1,  285,  383,   -1,  288,
  386,  387,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
  396,   41,   -1,   43,   44,   45,  499,  500,  501,  502,
  503,   -1,  505,   -1,  410,  508,  509,  413,   -1,   59,
   60,   61,   62,   -1,   -1,  324,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,  333,   -1,   -1,   -1,   -1,  435,
   -1,   -1,  341,  460,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   93,   -1,  451,  452,   -1,  454,   -1,
   -1,  360,   -1,  362,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,  376,   -1,  378,
  379,  380,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  489,  490,    0,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1, 1299, 1300,   -1,   -1,   -1,
  429, 1305, 1306,   -1, 1308, 1309,   41,  273,   43,   44,
   45,  277, 1316, 1317, 1318, 1319,   -1, 1321, 1322, 1323,
 1324, 1325, 1326,   -1,   59,   60,   61,   62,   -1,  295,
   -1,   -1,  298,  305,   -1,   -1,   -1,   -1,   -1,   -1,
  312,   -1,   -1,   -1,  316,  317,   -1,  318,   -1,   -1,
  321,   -1,   -1,  325,   -1,   -1,   -1,  328,   93,   -1,
  331,   -1,   -1,   -1,  335,  337,   -1,   -1,   -1,  340,
   -1,   -1,  343,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,  359,   -1,
   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
  402,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   -1,
  406,  311,  413,   -1,   -1,   -1,   -1,  419,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,  423,  328,   -1,
   -1,  331,  434,  333,  435,  335,  438,  434,   -1,  440,
  340,  341,   -1,  343,   -1,   -1,   -1,  449,   -1,  445,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,  460,   -1,   -1,  468,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,  480,  481,  386,  387,   -1,    0,
   -1,   -1,  489,  490,   -1,  492,  396,  262,   -1,  264,
  265,  497,  499,  500,  501,  502,  503,  272,  505,   -1,
  410,  508,  509,  413,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,  429,
   41,   -1,   -1,   44,   -1,  435,   -1,  302,   -1,   -1,
  440,   -1,  307,  308,   -1,   -1,  311,   -1,   59,   -1,
   61,  451,  452,  318,  454,  320,  321,   -1,   -1,  324,
  460,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,  260,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,
  490,    0,  492,   -1,  359,  360,   -1,  362,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
   -1,  376,  301,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,  313,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   41,   -1,   43,   44,   45,   -1,  327,   -1,
  261,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,  338,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  282,   -1,   -1,  429,   -1,   -1,  328,   -1,  290,
  435,   -1,   -1,   -1,   -1,  440,  297,   -1,  299,   -1,
  301,   -1,   -1,   -1,   93,  306,  451,  452,   -1,  454,
   -1,   -1,  313,   -1,   -1,  460,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,  327,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,  460,  338,  407,   -1,
   -1,   -1,   -1,   -1,  489,  490,   58,  492,  417,   -1,
   -1,   63,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,  402,   -1,  508,  509,  489,  490,  258,  492,   -1,
   -1,   -1,   -1,  442,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,  274,  508,  509,  277,  278,   -1,  280,
   -1,  460,   -1,  284,   -1,   -1,  287,  438,   -1,   -1,
   -1,   -1,  443,   -1,  405,   -1,   -1,   -1,  449,  300,
   -1,  412,   -1,   -1,  415,  306,  417,  418,  309,  460,
  489,  490,   -1,  492,   -1,   -1,   -1,  468,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  442,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,  460,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,   -1,
   -1,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,   -1,
   -1,  489,  490,  272,  492,   -1,   -1,  378,   -1,   -1,
  491,  499,  500,  501,  502,  503,   -1,  505,   -1,  288,
  508,  509,   -1,   -1,   -1,  506,  507,   -1,   -1,  400,
   -1,   -1,   -1,  302,   -1,   -1,  407,   -1,  307,  308,
  411,   -1,  311,   -1,   -1,   -1,   -1,  418,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,  436,  335,   -1,   -1,   -1,
  441,  340,  341,   -1,  343,  267,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,  466,   58,  289,   -1,  470,
  292,   63,   -1,  295,  296,   -1,  460,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,  314,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,  323,   -1,   -1,  326,  489,  490,   -1,  492,   -1,
   -1,  410,   -1,   -1,  413,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,   -1,    0,   -1,   -1,
  429,   -1,  354,  355,  460,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,  451,  452,   -1,  454,  378,   -1,   -1,   -1,
   -1,  460,   -1,  489,  490,   -1,  492,   40,   41,   -1,
   -1,   44,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,   -1,   -1,   59,   -1,   61,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,  421,
  499,  500,  501,  502,  503,  427,  505,   -1,   -1,  508,
  509,  433,   -1,  294,   -1,   -1,   -1,   -1,   91,   -1,
   -1,  443,   -1,   -1,  305,   -1,   -1,   -1,   -1,   -1,
   -1,  312,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,  325,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   40,   -1,  289,   43,   -1,
   45,   -1,   -1,  295,  296,  386,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,  402,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,  419,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  434,   -1,   -1,   -1,  438,   -1,   -1,
  441,   -1,  354,  355,   -1,   -1,   -1,   -1,  449,   -1,
   -1,    0,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,  258,  378,  468,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  274,   -1,   -1,  277,  278,   -1,  280,   -1,   -1,
   -1,  284,   41,   -1,  287,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  417,   -1,  300,   -1,  421,
   59,   -1,   61,  306,   -1,  427,  309,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   91,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  378,  498,  499,  500,  501,
  502,  503,  504,  505,    0,   -1,  508,  509,  510,  511,
  512,   -1,  267,   -1,   -1,   -1,    0,  400,   -1,   -1,
   -1,  276,   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,
   -1,   -1,   -1,   -1,  289,  418,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   41,  354,   -1,   44,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   41,  441,  314,
   44,   -1,   -1,   59,   -1,   61,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,
   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  258,
   -1,   -1,   -1,  378,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,
   -1,  280,   58,   -1,   -1,  284,   -1,   63,  287,   -1,
  458,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  300,   -1,  471,   -1,   -1,  421,  306,   -1,   -1,
  309,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,  443,   -1,
  498,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,  378,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  400,  258,   -1,   -1,   -1,   -1,  260,  407,   -1,
   -1,   -1,  411,   -1,  258,   -1,   -1,   -1,  274,  418,
   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,
  274,  287,   -1,   -1,  278,   -1,  280,  436,   -1,   -1,
  284,   -1,  441,  287,  300,   -1,   -1,   -1,  301,   -1,
  306,   -1,   -1,  309,   -1,   -1,  300,   -1,   -1,   -1,
  313,   -1,  306,   -1,   -1,  309,   -1,  466,   -1,   -1,
   -1,  470,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  260,  338,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,  378,   40,   -1,   42,   43,  303,   45,   -1,
   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  400,   -1,   63,  323,   -1,   -1,
  326,  407,   -1,   -1,  407,  411,  400,   -1,   -1,   -1,
   -1,   -1,  418,  407,  417,   -1,   -1,  411,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,  352,   -1,  354,  355,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,  442,
  366,   -1,  436,   -1,  370,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
  466,   -1,   -1,   -1,  470,   -1,   -1,   -1,   -1,   -1,
   40,   -1,  466,   43,   -1,   45,  470,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   58,  492,
   -1,   -1,   -1,   63,   -1,   -1,  499,  500,  501,  502,
  503,  427,  505,   -1,   -1,  508,  509,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   40,   44,  289,
   43,   -1,   45,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  262,  260,  508,  509,
  263,   -1,   -1,   -1,  267,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,  289,   43,  295,   45,
   -1,  298,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
  323,   -1,  328,  326,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  339,   -1,  340,   -1,   -1,  343,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
   -1,  354,  355,  359,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,  373,  370,  375,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  396,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,  435,
  433,   -1,   -1,   -1,  440,   -1,   -1,   -1,  445,   -1,
  443,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
  497,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  260,  508,  509,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,  460,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,   -1,   -1,  487,  488,  489,  490,  323,  492,   -1,
  326,  495,  496,   -1,   -1,  499,  500,  501,  502,  503,
  504,  505,  506,   -1,  508,  509,  510,  511,  512,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   40,   -1,  289,   43,
   -1,   45,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  425,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  260,  508,  509,  263,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   40,   -1,  289,   43,   -1,   45,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  260,  508,  509,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,  354,  355,   -1,
   59,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   40,   -1,  289,   43,   -1,
   45,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   41,  495,  496,   44,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  260,  508,  509,  263,   -1,
   59,   -1,  267,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   58,  289,   -1,   -1,   -1,   63,  328,
  295,  296,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
  359,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
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
   -1,   -1,   -1,  262,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,  289,   -1,   -1,   -1,   -1,  318,
  295,  296,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
  267,  366,   -1,   -1,   -1,  370,   -1,  396,   -1,  276,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,  413,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,  427,  452,   -1,  454,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   40,   -1,  433,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  262,   -1,   -1,   63,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
  318,  508,  509,  321,   -1,   -1,   -1,   -1,  276,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,
   -1,  289,  340,   -1,   -1,  343,   -1,  295,  296,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,  323,   63,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  354,  355,
  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  323,  433,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  267,  498,  499,  500,  501,  502,  503,  504,  505,  276,
   -1,  508,  509,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,   -1,   41,   58,   -1,   44,   -1,  366,
   63,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,   -1,   -1,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
  417,   -1,  499,  500,  501,  502,  503,  504,  505,  506,
  427,  508,  509,  510,  511,  512,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  267,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  323,   44,   -1,  326,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  276,   -1,  262,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,  352,  289,  354,  355,   -1,
   -1,   -1,  295,  296,   -1,   58,   -1,   -1,   -1,  366,
   63,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,  354,  355,  340,   -1,   -1,  343,   -1,   -1,   -1,
  427,   -1,   -1,  366,   -1,   -1,  433,  370,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  427,  492,  413,   -1,  495,  496,
  433,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  443,  508,  509,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  471,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  267,  508,  509,   40,   -1,   -1,
   43,   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,   -1,   -1,
   63,  318,  295,  296,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   40,  359,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  354,  355,   -1,   63,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,   -1,
  323,   -1,  295,  326,   -1,  298,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
  289,  354,  355,   -1,   -1,   -1,  295,  296,   -1,   58,
   59,   -1,   -1,  366,   63,   -1,  339,  370,   -1,   -1,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,  417,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  427,   -1,   -1,  366,   63,   -1,
  433,  370,   -1,  406,   -1,   -1,   -1,   -1,  411,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  423,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,  427,  492,
   -1,   -1,  495,  496,  433,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  443,  508,  509,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,
  459,  460,  461,  462,  497,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  267,  508,
  509,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  291,   -1,  323,   -1,  295,  326,   -1,  298,
   -1,   -1,  267,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  289,  354,  355,   -1,   63,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
  339,  370,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   40,  354,
  355,   43,   -1,   45,   -1,   -1,   -1,   -1,  427,   -1,
   -1,  366,   -1,   -1,  433,  370,   58,  406,   -1,   -1,
   -1,   63,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  427,  492,   -1,   -1,  495,  496,  433,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  443,  508,
  509,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  497,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,    0,  508,  509,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  323,   -1,
   -1,  326,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,  267,    0,   -1,   -1,  354,
  355,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,    0,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   59,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   59,   -1,   -1,  433,    0,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   41,   -1,   -1,   44,    0,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   59,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,  262,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,    0,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,  318,   -1,  331,  321,   -1,   -1,  335,  262,   -1,
   -1,  328,  340,   -1,  331,  343,   41,   -1,  335,   44,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,  262,   -1,
   -1,  359,   -1,   -1,   59,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,  318,   -1,  383,  321,  396,  386,
  387,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  396,
   41,  335,   -1,   44,  318,  413,  340,  321,   -1,  343,
   -1,  262,   -1,   -1,  328,   -1,  413,  331,   59,   -1,
   -1,  335,    0,   -1,   -1,  359,  340,  435,   -1,  343,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  452,  359,  454,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,  452,  262,  454,   -1,   -1,
   -1,   -1,  396,   41,   -1,   -1,   44,  318,   -1,  383,
  321,   -1,  386,  387,   -1,   -1,   -1,  328,   -1,  413,
  331,   59,  396,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,  435,    0,   -1,   -1,   -1,  440,   -1,  359,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  452,   -1,
  454,  435,  328,    0,   -1,  331,  440,   -1,   -1,  335,
   -1,   -1,  383,   -1,  340,  386,  387,  343,  452,   -1,
  454,   -1,   -1,   41,   -1,  396,   44,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   59,  413,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   59,   -1,  435,   -1,   -1,   -1,   -1,  440,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
   -1,  262,   -1,  328,   -1,   -1,  331,   -1,   -1,  435,
  335,   -1,   -1,   -1,  440,  340,   -1,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,  383,   -1,
  321,  386,  387,   -1,  262,   -1,   -1,  328,   -1,   -1,
  331,  396,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  413,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
  318,   -1,  383,  321,   -1,  386,  387,  452,   -1,  454,
  328,   -1,   -1,  331,   -1,  396,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,  262,  343,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,  396,   -1,
  318,   -1,   -1,  321,   -1,   -1,  271,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  413,   -1,  335,   -1,   -1,
   -1,  318,  340,   -1,  321,  343,   -1,   -1,   -1,   -1,
   -1,  328,   -1,  271,  331,   -1,  301,  435,  335,   -1,
   -1,  359,  440,  340,   -1,   -1,  343,  312,  313,   -1,
  315,   -1,   -1,  318,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  359,  301,   -1,  383,   -1,  332,  386,  387,
  308,   -1,   -1,  338,  312,  313,   -1,  315,  396,   -1,
  318,   -1,   -1,   -1,  271,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,  332,  413,   -1,   -1,   -1,  396,
  338,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  301,   -1,  413,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,  318,   -1,  398,  452,   -1,  454,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,  332,   -1,   -1,   -1,   -1,
   -1,  338,  417,   -1,   -1,  452,   -1,  454,   -1,   -1,
  398,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,  417,
   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,
   -1,   -1,   -1,   -1,  432,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  442,   -1,  273,   -1,   -1,   -1,
  277,  398,   -1,  451,   -1,  453,   -1,  284,   -1,   -1,
   -1,   -1,  460,   -1,  489,  490,   -1,  492,  295,   -1,
  417,  298,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  432,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  442,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  453,  505,   -1,   -1,
  508,  509,  339,  460,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,  489,  490,   -1,  492,  373,   -1,  375,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  295,  505,   -1,
  298,  508,  509,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,   -1,  445,   -1,
   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,  460,   -1,  373,   -1,  375,   -1,   -1,
   -1,  291,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  406,   -1,
  497,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,   -1,   -1,  423,   -1,   -1,   -1,  339,
   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,   -1,  445,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,  406,   -1,   -1,  497,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  497,
};
#define YYFINAL 20
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
"RELEASE","STATEMENT","LEAVE","INSERTING","UPDATING","DELETING",
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
"statement : savepoint",
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
"savepoint : set_savepoint",
"savepoint : release_savepoint",
"savepoint : undo_savepoint",
"set_savepoint : SAVEPOINT symbol_savepoint_name",
"release_savepoint : RELEASE SAVEPOINT symbol_savepoint_name",
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
case 21:
{ prepare_console_debug ((int) yyvsp[0], &lex.dsql_debug);
			  yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 22:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 23:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-6], yyvsp[-3], make_list(yyvsp[-1]), yyvsp[0]); }
break;
case 24:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-4], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 25:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					yyvsp[-5], yyvsp[-2], make_list(yyvsp[0]), NULL); }
break;
case 26:
{ yyval = make_node (nod_grant, (int) e_grant_count, 
					make_list(yyvsp[-3]), make_list(yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 28:
{ yyval = yyvsp[0]; }
break;
case 29:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 30:
{ yyval = make_node (nod_all, (int) 0, NULL); }
break;
case 31:
{ yyval = make_list (yyvsp[0]); }
break;
case 33:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 34:
{ yyval = make_list (make_node (nod_execute, (int) 0, NULL)); }
break;
case 35:
{ yyval = make_node (nod_select, (int) 0, NULL); }
break;
case 36:
{ yyval = make_node (nod_insert, (int) 0, NULL); }
break;
case 37:
{ yyval = make_node (nod_delete, (int) 0, NULL); }
break;
case 38:
{ yyval = make_node (nod_update, (int) 1, yyvsp[0]); }
break;
case 39:
{ yyval = make_node (nod_references, (int) 1, yyvsp[0]); }
break;
case 40:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 41:
{ yyval = 0; }
break;
case 42:
{ yyval = make_node (nod_grant_admin, (int) 0, NULL); }
break;
case 43:
{ yyval = 0; }
break;
case 44:
{ yyval = make_node (nod_procedure_name, (int) 1, yyvsp[0]); }
break;
case 45:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-5]); }
break;
case 46:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), yyvsp[-6]); }
break;
case 47:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 48:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 49:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-4], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 50:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, yyvsp[-5], yyvsp[-2],
				make_list(yyvsp[0]), NULL); }
break;
case 51:
{ yyval = make_node (nod_revoke, 
				(int) e_grant_count, make_list(yyvsp[-2]), make_list(yyvsp[0]),
				NULL, NULL); }
break;
case 52:
{ yyval = make_node (nod_grant, (int) 0, NULL); }
break;
case 54:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 55:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 56:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 57:
{ yyval = make_node (nod_proc_obj, (int) 1, yyvsp[0]); }
break;
case 58:
{ yyval = make_node (nod_trig_obj, (int) 1, yyvsp[0]); }
break;
case 59:
{ yyval = make_node (nod_view_obj, (int) 1, yyvsp[0]); }
break;
case 60:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 62:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 63:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 64:
{ yyval = make_node (nod_user_name, (int) 2, yyvsp[0], NULL); }
break;
case 65:
{ yyval = make_node (nod_user_group, (int) 1, yyvsp[0]); }
break;
case 67:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 68:
{ yyval = make_node (nod_role_name, (int) 1, yyvsp[0]); }
break;
case 70:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 71:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 72:
{ yyval = make_node (nod_user_name, (int) 1, yyvsp[0]); }
break;
case 73:
{ yyval = yyvsp[0];}
break;
case 74:
{ yyval = yyvsp[0]; }
break;
case 75:
{ yyval = yyvsp[0]; }
break;
case 76:
{ yyval = make_node (nod_def_udf, (int) e_udf_count, 
				yyvsp[-7], yyvsp[-2], yyvsp[0], make_list (yyvsp[-6]), yyvsp[-4]); }
break;
case 78:
{ lex.g_field->fld_dtype = dtype_blob; }
break;
case 79:
{ 
			lex.g_field->fld_dtype = dtype_cstring; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-2]; }
break;
case 80:
{ yyval = (DSQL_NOD) NULL; }
break;
case 82:
{ yyval = yyvsp[-1]; }
break;
case 84:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 85:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				              yyvsp[-1], NULL); }
break;
case 86:
{ yyval = make_node (nod_udf_param, (int) e_udf_param_count,
				yyvsp[-3], MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG)); }
break;
case 88:
{ yyval = yyvsp[-1]; }
break;
case 89:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-1], 
				MAKE_constant ((STR) FUN_reference, CONSTANT_SLONG));}
break;
case 90:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-2], 
				MAKE_constant ((STR) (-1 * FUN_reference), CONSTANT_SLONG));}
break;
case 91:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3], 
				MAKE_constant ((STR) FUN_value, CONSTANT_SLONG));}
break;
case 92:
{ yyval = make_node (nod_udf_return_value, (int) 2, yyvsp[-3],
				MAKE_constant ((STR) FUN_descriptor, CONSTANT_SLONG));}
break;
case 93:
{ yyval = make_node (nod_udf_return_value, (int) 2, 
		  		(DSQL_NOD) NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
break;
case 94:
{ yyval = make_node (nod_def_filter, (int) e_filter_count, 
						yyvsp[-8], yyvsp[-6], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 95:
{ yyval = yyvsp[0]; }
break;
case 96:
{ yyval = make_node (nod_def_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 97:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					yyvsp[-6], yyvsp[-5], yyvsp[-3], yyvsp[-1], yyvsp[0]); }
break;
case 98:
{ yyval = yyvsp[0]; }
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
{ yyval = make_node (nod_unique, 0, NULL); }
break;
case 115:
{ yyval = NULL; }
break;
case 116:
{ yyval = make_list (yyvsp[0]); }
break;
case 118:
{ yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 119:
{ yyval = make_node (nod_def_shadow, (int) e_shadow_count,
			     yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], make_list (yyvsp[0])); }
break;
case 120:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 121:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 122:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 123:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 124:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 125:
{ yyval = (DSQL_NOD) 0;}
break;
case 126:
{ yyval = yyvsp[-1]; }
break;
case 127:
{ yyval = (DSQL_NOD) NULL; }
break;
case 130:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 131:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                          yyvsp[-7], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 132:
{ yyval = NULL; }
break;
case 133:
{ yyval = NULL; }
break;
case 134:
{ yyval = yyvsp[0]; }
break;
case 135:
{ yyval = (DSQL_NOD) NULL; }
break;
case 136:
{ yyval = (DSQL_NOD) NULL; }
break;
case 139:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 140:
{ yyval = make_node (nod_rel_constraint, (int) 2, NULL, yyvsp[0]);}
break;
case 143:
{ yyval = make_node (nod_null, (int) 0, NULL); }
break;
case 144:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 145:
{ yyval = make_node (nod_def_generator, 
						(int) e_gen_count, yyvsp[0]); }
break;
case 146:
{ yyval = make_node (nod_def_role, 
						(int) 1, yyvsp[0]); }
break;
case 147:
{ yyval = make_node (nod_def_database, (int) e_cdb_count,
				 yyvsp[-2], make_list(yyvsp[-1]), make_list (yyvsp[0]));}
break;
case 150:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) yyvsp[0]; }
break;
case 151:
{yyval = (DSQL_NOD) NULL;}
break;
case 154:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 155:
{ yyval = make_node (nod_page_size, 1, yyvsp[0]);}
break;
case 156:
{ yyval = make_node (nod_file_length, 1, yyvsp[-1]);}
break;
case 157:
{ yyval = make_node (nod_user_name, 1, yyvsp[0]);}
break;
case 158:
{ yyval = make_node (nod_password, 1, yyvsp[0]);}
break;
case 159:
{ yyval = make_node (nod_lc_ctype, 1, yyvsp[0]);}
break;
case 160:
{yyval = (DSQL_NOD) NULL;}
break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 168:
{ yyval = make_node (nod_group_commit_wait, 1, yyvsp[0]);}
break;
case 169:
{ yyval = make_node (nod_check_point_len, 1, yyvsp[0]);}
break;
case 170:
{ yyval = make_node (nod_num_log_buffers, 1, yyvsp[0]);}
break;
case 171:
{ yyval = make_node (nod_log_buffer_size, 1, yyvsp[0]);}
break;
case 172:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 173:
{ if (lex.log_defined)
			    yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 174:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
			      yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 176:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
break;
case 177:
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
break;
case 179:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 180:
{ 
		         check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
                                                (DSQL_NOD) lex.g_file); }
break;
case 181:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
break;
case 184:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
break;
case 185:
{ lex.g_file  = make_file ();}
break;
case 190:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
break;
case 191:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
break;
case 198:
{ yyval = make_node (nod_def_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 199:
{ yyval = make_node (nod_redef_relation, 
				(int) e_drl_count, yyvsp[-4], make_list (yyvsp[-1]), yyvsp[-3]); }
break;
case 200:
{ yyval = yyvsp[0]; }
break;
case 201:
{ yyval = yyvsp[0]; }
break;
case 202:
{ yyval = (DSQL_NOD) NULL; }
break;
case 204:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 207:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
					yyvsp[-5], yyvsp[-3], yyvsp[-2], make_list (yyvsp[-1]), yyvsp[0], yyvsp[-4], NULL); }
break;
case 208:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-2], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 209:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				    yyvsp[-1], NULL, NULL, NULL, NULL, NULL, yyvsp[0]); }
break;
case 210:
{ 
			lex.g_field->fld_flags |= FLD_computed;
			yyval = make_node (nod_def_computed, 2, yyvsp[-2], yyvsp[-1]); }
break;
case 213:
{ yyval = NULL; }
break;
case 214:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                            yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 215:
{ yyval = yyvsp[0]; }
break;
case 216:
{ yyval = (DSQL_NOD) NULL; }
break;
case 217:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 218:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
break;
case 219:
{ yyval = yyvsp[-1]; }
break;
case 220:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 221:
{ yyval = yyvsp[0]; }
break;
case 222:
{ yyval = (DSQL_NOD) NULL; }
break;
case 226:
{ yyval = yyvsp[0]; }
break;
case 227:
{ yyval = yyvsp[0]; }
break;
case 228:
{ yyval = yyvsp[0]; }
break;
case 229:
{ yyval = (DSQL_NOD) NULL; }
break;
case 232:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 233:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 234:
{ yyval = make_node (nod_null, (int) 1, NULL); }
break;
case 235:
{ yyval = make_node (nod_foreign, e_for_count,
                        make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 237:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_primary, e_pri_count, NULL, yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_rel_constraint, (int) 2, yyvsp[-1], yyvsp[0]);}
break;
case 240:
{ yyval = yyvsp[0]; }
break;
case 241:
{ yyval = NULL ;}
break;
case 246:
{ yyval = make_node (nod_unique, 2, yyvsp[-1], yyvsp[0]); }
break;
case 247:
{ yyval = make_node (nod_primary, e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 248:
{ yyval = make_node (nod_foreign, e_for_count, yyvsp[-5], yyvsp[-3], 
			         yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, yyvsp[-2], yyvsp[0], NULL, NULL); }
break;
case 250:
{ yyval = make_node (nod_def_index, (int) e_idx_count, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 251:
{ yyval = make_node (nod_def_constraint, 
				(int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 252:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 253:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
break;
case 256:
{ yyval = NULL;}
break;
case 257:
{ yyval = yyvsp[0];}
break;
case 258:
{ yyval = yyvsp[0];}
break;
case 259:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_CASCADE, e_ref_trig_action_count, NULL);}
break;
case 260:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_node (nod_def_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 264:
{ yyval = make_node (nod_redef_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 265:
{ yyval = make_node (nod_replace_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 266:
{ yyval = make_node (nod_mod_procedure,
						  (int) e_prc_count, 
					          yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 267:
{ yyval = make_list (yyvsp[-1]); }
break;
case 268:
{ yyval = NULL; }
break;
case 269:
{ yyval = yyvsp[0]; }
break;
case 270:
{ yyval = NULL; }
break;
case 272:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 273:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-1], NULL, NULL, NULL, NULL, NULL, NULL); }
break;
case 274:
{ yyval = make_list (yyvsp[0]); }
break;
case 275:
{ yyval = NULL; }
break;
case 277:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 278:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-3], yyvsp[-1], NULL, NULL, NULL, NULL, NULL); }
break;
case 279:
{ yyval = NULL; }
break;
case 280:
{ yyval = NULL; }
break;
case 281:
{ yyval = yyvsp[0]; }
break;
case 282:
{ yyval = yyvsp[0]; }
break;
case 285:
{ yyval = yyvsp[-1]; }
break;
case 286:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 287:
{ yyval = make_node (nod_block, e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 288:
{ yyval = make_node (nod_block, e_blk_count, NULL, NULL);}
break;
case 290:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 306:
{ yyval = make_node (nod_return, e_rtn_count, NULL); }
break;
case 307:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 309:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-1], NULL); }
break;
case 310:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 311:
{ yyval = make_node (nod_exception_stmt, e_xcp_count, NULL, NULL); }
break;
case 312:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 313:
{ yyval = make_node (nod_exec_sql, e_exec_sql_count, yyvsp[-1]); }
break;
case 316:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 317:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
break;
case 318:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
break;
case 319:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 320:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 321:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 322:
{ yyval = yyvsp[0]; }
break;
case 323:
{ yyval = NULL; }
break;
case 324:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 325:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
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
case 329:
{ yyval = make_list (yyvsp[0]); }
break;
case 330:
{ yyval = make_list (yyvsp[-1]); }
break;
case 331:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 335:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 336:
{ yyval = make_node (nod_while, e_while_count,
					  yyvsp[-3], yyvsp[0], NULL); }
break;
case 337:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 338:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 339:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 340:
{ yyval = NULL; }
break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 347:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 350:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 351:
{ yyval = make_list (yyvsp[0]); }
break;
case 352:
{ yyval = make_list (yyvsp[-1]); }
break;
case 353:
{ yyval = NULL; }
break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 358:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 359:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 361:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
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
{ lex.beginning = lex_position(); }
break;
case 375:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 376:
{ lex.beginning = lex.last_token; }
break;
case 377:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 378:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
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
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
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
case 461:
{ yyval = NULL; }
break;
case 462:
{ yyval = NULL; }
break;
case 463:
{ yyval = NULL; }
break;
case 464:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 465:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 466:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 467:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 468:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 469:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 470:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 471:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 473:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 474:
{ yyval = yyvsp[0]; }
break;
case 475:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 476:
{ yyval = yyvsp[0]; }
break;
case 477:
{ yyval = yyvsp[0]; }
break;
case 479:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 482:
{ yyval = NULL; }
break;
case 484:
{ yyval = NULL; }
break;
case 485:
{ yyval = yyvsp[0]; }
break;
case 486:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 494:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 495:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 496:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 502:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 503:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 505:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 506:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 507:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 513:
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
case 514:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 515:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 516:
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
case 517:
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
case 518:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 525:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 526:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 527:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 528:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 529:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 530:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 535:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 536:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 537:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 546:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 547:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 548:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 549:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 550:
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
case 551:
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
case 554:
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
case 555:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 556:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 557:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 558:
{ yyval = yyvsp[-1]; }
break;
case 559:
{ yyval = 0; }
break;
case 563:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 564:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 565:
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
{ yyval = make_node (nod_release_savepoint, 1, yyvsp[0]); }
break;
case 571:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 574:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 575:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 578:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 579:
{ yyval = (DSQL_NOD) NULL; }
break;
case 581:
{ yyval = (DSQL_NOD) NULL; }
break;
case 582:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 584:
{ yyval = (DSQL_NOD) NULL; }
break;
case 586:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 591:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 592:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 593:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 594:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 595:
{ yyval = yyvsp[0];}
break;
case 597:
{ yyval = yyvsp[0];}
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 600:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 602:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 604:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 605:
{ yyval = 0; }
break;
case 606:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 607:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 608:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 609:
{ yyval = (DSQL_NOD) 0; }
break;
case 610:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 611:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 613:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 614:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 615:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 616:
{ yyval = 0; }
break;
case 618:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 619:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 620:
{ yyval = make_node (nod_select, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 621:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 622:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 623:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 624:
{ yyval = make_list (yyvsp[0]); }
break;
case 625:
{ yyval = 0; }
break;
case 627:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 628:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 633:
{ yyval = 0; }
break;
case 634:
{ yyval = yyvsp[-1]; }
break;
case 635:
{ yyval = 0; }
break;
case 636:
{ yyval = make_node (nod_for_update, 2, yyvsp[-1], yyvsp[0]); }
break;
case 637:
{ yyval = 0; }
break;
case 638:
{ yyval = yyvsp[0]; }
break;
case 639:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 640:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 641:
{ yyval = 0; }
break;
case 642:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 643:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 644:
{ lex.limit_clause = true; }
break;
case 645:
{ lex.limit_clause = false; }
break;
case 646:
{ lex.first_detection = true; }
break;
case 647:
{ lex.first_detection = false; }
break;
case 648:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 649:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 650:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 651:
{ yyval = 0; }
break;
case 652:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 653:
{ yyval = yyvsp[-2]; }
break;
case 654:
{ yyval = yyvsp[-1]; }
break;
case 655:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 656:
{ yyval = yyvsp[-1]; }
break;
case 657:
{ yyval = yyvsp[0]; }
break;
case 658:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 659:
{ yyval = 0; }
break;
case 660:
{ yyval = make_list (yyvsp[0]); }
break;
case 661:
{ yyval = 0; }
break;
case 663:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 665:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 667:
{ yyval = make_list (yyvsp[0]); }
break;
case 669:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 672:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 673:
{ yyval = yyvsp[-1]; }
break;
case 674:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 675:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 676:
{ yyval = make_list (yyvsp[-1]); }
break;
case 677:
{ yyval = NULL; }
break;
case 679:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 683:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 684:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 685:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 686:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 687:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 688:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 689:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 690:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 693:
{ yyval = make_list (yyvsp[0]); }
break;
case 694:
{ yyval = 0; }
break;
case 696:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 701:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 705:
{ yyval = yyvsp[0]; }
break;
case 706:
{ yyval = 0; }
break;
case 707:
{ yyval = yyvsp[0]; }
break;
case 708:
{ yyval = 0; }
break;
case 709:
{ yyval = yyvsp[0]; }
break;
case 710:
{ yyval = 0; }
break;
case 711:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 712:
{ yyval = 0; }
break;
case 713:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 714:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 715:
{ yyval = 0; }
break;
case 716:
{ yyval = 0; }
break;
case 718:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 723:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 724:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 725:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 729:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 734:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 735:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 739:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 740:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 742:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 746:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 747:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 749:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 753:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 754:
{ yyval = yyvsp[0]; }
break;
case 756:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 759:
{ yyval = NULL; }
break;
case 760:
{ yyval = make_list (yyvsp[-1]); }
break;
case 762:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 764:
{ yyval = NULL; }
break;
case 765:
{ yyval = make_list (yyvsp[-1]); }
break;
case 767:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 771:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 773:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 775:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 789:
{ yyval = yyvsp[-1]; }
break;
case 790:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 796:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 828:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 830:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 835:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 836:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 838:
{ yyval = make_list (yyvsp[-1]); }
break;
case 839:
{ yyval = yyvsp[-1]; }
break;
case 840:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 841:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 851:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 852:
{ yyval = yyvsp[0]; }
break;
case 853:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 854:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 855:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 856:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 857:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 858:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 859:
{ yyval = yyvsp[-1]; }
break;
case 860:
{ yyval = yyvsp[-1]; }
break;
case 864:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 865:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 866:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 867:
{ yyval = yyvsp[0]; }
break;
case 868:
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
case 869:
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
case 870:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 871:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 873:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 875:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 880:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 882:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 883:
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
case 884:
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
case 885:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 886:
{ yyval = make_parameter (); }
break;
case 887:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 888:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 889:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 894:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 895:
{ yyval = yyvsp[0]; }
break;
case 896:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 898:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 899:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 901:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 902:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 904:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 905:
{ yyval = yyvsp[0];}
break;
case 910:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 911:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 912:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 915:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 916:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 917:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 920:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 921:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 922:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 923:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 924:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 925:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 926:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 927:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 928:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 931:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 932:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 936:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 938:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 939:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 940:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 942:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 953:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 956:
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
