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
 *			clashes with normal DEBUG macro.
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
 *							exception handling in SPs/triggers,
 *							implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.12.03 Dmitry Yemanov: Implemented ORDER BY clause in subqueries.
 * 2002.12.18 Dmitry Yemanov: Added support for SQL-compliant labels and LEAVE statement
 * 2002.12.28 Dmitry Yemanov: Added support for parametrized events.
 * 2003.01.14 Dmitry Yemanov: Fixed bug with cursors in triggers.
 * 2003.01.15 Dmitry Yemanov: Added support for runtime trigger action checks.
 * 2003.02.10 Mike Nordell  : Undefined Microsoft introduced macros to get a clean compile.
 * 2003.05.24 Nickolay Samofatov: Make SKIP and FIRST non-reserved keywords
 * 2003.06.13 Nickolay Samofatov: Make INSERTING/UPDATING/DELETING non-reserved keywords
 * 2003.07.01 Blas Rodriguez Somoza: Change DEBUG and IN to avoid conflicts in win32 build/bison
 * 2003.08.11 Arno Brinkman: Changed GROUP BY to support all expressions and added "AS" support
 *						   with table alias. Also removed group_by_function and ordinal.
 * 2003.08.14 Arno Brinkman: Added support for derived tables.
 */

#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
#include <windows.h>
#include <stdio.h>
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
#include "../jrd/err_proto.h"

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

#define MIN_CACHE_BUFFERS	   250
#define DEF_CACHE_BUFFERS	   1000

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

#ifndef SHLIB_DEFS
DSQL_NOD		DSQL_parse;
#else
extern DSQL_NOD	DSQL_parse;
#endif

static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

#define MAX_TOKEN_LEN   256
#define CHECK_BOUND(to)\
	{\
	if ((to - string) >= MAX_TOKEN_LEN)		\
	yyabandon (-104, isc_token_too_long); \
	}
#define CHECK_COPY_INCR(to,ch){CHECK_BOUND(to); *to++=ch;}


static TEXT	*lex_position (void);
#ifdef NOT_USED_OR_REPLACED
static bool		long_int(DSQL_NOD, SLONG *);
#endif
static DSQL_FLD	make_field (DSQL_NOD);
static FIL	make_file (void);
static DSQL_NOD	make_list (DSQL_NOD);
static DSQL_NOD	make_node (NOD_TYPE, int, ...);
static DSQL_NOD	make_parameter (void);
static DSQL_NOD	make_flag_node (NOD_TYPE, SSHORT, int, ...);
static void	prepare_console_debug (int, int  *);
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(DSQL_NOD, SLONG *, SSHORT);
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
	int  prev_keyword, prev_prev_keyword;
	USHORT	param_number;
	/* Fields to handle FIRST/SKIP as non-reserved keywords */
	bool limit_clause; /* We are inside of limit clause. Need to detect SKIP after FIRST */
	bool first_detection; /* Detect FIRST unconditionally */
	/* Fields to handle INSERTING/UPDATING/DELETING as non-reserved keywords */
	bool brace_analysis; /* When this is true lexer is informed not to swallow braces around INSERTING/UPDATING/DELETING */
	
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
#define KW_DELETE 301
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
#define KW_IN 333
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
#define KW_SHARED 420
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
#define KW_INSERTING 513
#define KW_UPDATING 514
#define KW_DELETING 515
#define BACKUP 516
#define KW_DIFFERENCE 517
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
  115,  118,  118,  119,  119,  119,  119,  119,  121,  121,
  121,  121,  120,  120,  125,  125,  124,   98,  126,  126,
  127,  130,  131,  131,  132,  128,  129,  129,  133,  133,
  134,  134,  135,  135,  135,   96,   96,   96,   69,   79,
  136,  136,  136,  137,  137,  138,  138,  139,  139,  139,
  145,   86,   86,  141,  141,  104,  104,   99,  149,  150,
   58,  142,  142,  105,  105,  105,  105,  105,  105,  143,
  143,  157,  157,  158,  160,  160,  160,  160,  160,  140,
  159,  159,  164,  164,  164,  164,  166,  167,  168,  162,
  162,  163,  161,  161,  161,  161,  161,  169,  170,  171,
  171,  171,  171,   68,   78,   82,  176,  172,  172,  173,
  173,  177,  177,  178,  174,  174,  179,  179,  180,  181,
  181,  182,  182,  183,  183,  175,  185,  185,  185,  186,
  186,  184,  184,  184,  184,  184,  184,  184,  184,  184,
  184,  184,  184,  184,  184,  184,  184,  184,  184,  189,
  189,  190,  191,  192,  203,  203,  193,  198,  199,  194,
  194,  195,  207,  196,  208,  201,  201,  201,  202,  202,
  202,  205,  205,  205,  205,  197,  204,  204,  200,  200,
  200,  206,  206,  187,  187,  214,  215,  215,  216,  216,
  216,  216,   11,   71,   80,  218,  221,  221,  221,  222,
  225,  229,  229,  230,  230,  231,  231,  147,  220,   87,
   89,  219,  219,   70,   83,  234,  234,  234,  235,  238,
  238,  239,  239,  239,  239,  239,  239,  239,  239,  239,
  239,  239,  239,  239,  239,  239,  236,  236,  237,    2,
  241,  241,  241,  241,  241,  241,  241,  249,  250,  247,
  247,  251,  251,  251,  251,  251,  251,  251,  242,  242,
  252,  252,  252,  252,  252,  252,  252,  246,  257,  257,
  257,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  257,  257,  257,  257,  257,  257,  257,  257,  257,
  257,  254,  254,  256,  256,  255,  253,  253,  253,  248,
  248,  244,  245,  245,  259,  259,  259,  259,  259,  259,
  259,  259,  260,  260,  243,  261,  261,  262,  262,    8,
  263,  263,  263,  263,  263,  263,  263,  263,  263,  263,
  263,  263,  101,  101,  144,  144,  266,  266,  269,  269,
  271,  271,   53,   53,  268,  268,  268,  268,  268,  268,
  268,  268,  268,  276,  276,  267,  267,  267,  267,  277,
  277,   61,   61,   61,   55,   55,  273,  273,  273,  270,
  270,  270,  281,  281,  281,  280,  280,  279,  279,  279,
  274,  274,  282,  282,  282,  283,  283,  275,  275,  275,
  275,  284,  284,   18,   18,   18,  286,  286,  286,   16,
   16,   16,  288,  289,  292,  292,  290,  294,  294,    4,
   15,  293,  293,  295,  295,  296,  296,  285,  297,  297,
  298,  298,  299,  299,  299,  299,  300,  300,  301,  301,
  302,  302,  304,  304,  304,  305,  305,  305,  306,  306,
  306,  303,  308,  308,  308,  309,  309,  307,  307,  310,
  312,  312,  311,  311,  287,   17,  313,  313,  313,  314,
  314,  318,  318,  319,   65,   65,   65,  321,  321,  320,
  320,  315,  315,  324,  324,  316,  316,  317,  328,  329,
  330,  322,  323,  325,  325,  325,  325,  331,  331,  331,
  332,  332,  332,  223,  223,  224,  224,  335,  335,  336,
  336,  338,  338,  326,  340,  340,  341,  341,  341,  344,
  345,  345,  346,  346,  348,  348,  342,  342,  343,  343,
  349,  349,  210,  210,  350,  350,  232,  232,   31,  233,
  233,  233,  233,  233,  233,  233,  233,  227,  227,  351,
  351,  352,  228,  228,  226,  226,  327,  327,  353,  354,
  354,  354,  354,  354,  355,  355,  356,  356,  357,  357,
  358,  358,  358,  359,  359,  360,  360,   10,   10,  362,
  362,    7,    7,  363,  364,  365,   19,   19,  366,  367,
  368,  368,  188,  337,  337,    3,    3,  370,  370,  370,
  372,  372,   61,  371,  371,  373,  373,   34,   34,   85,
   84,   84,  361,  361,  374,  375,  375,  211,  211,  211,
  146,  369,  369,  111,  111,  111,  111,  111,  379,  379,
  379,  379,  379,  379,  378,  378,  378,  381,  381,  381,
  381,  381,  381,  381,  381,  381,  381,  382,  382,  382,
  382,  382,  382,  382,  382,  387,  387,  387,  387,  387,
  387,  387,  387,  387,  387,  387,  387,  387,  387,  387,
  387,  393,  393,  383,  383,  384,  384,  384,  384,  385,
  385,  389,  389,  390,  390,  390,  390,  388,  391,  386,
  386,  377,  377,  377,  380,  380,  380,  394,  394,  395,
  392,  397,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,  156,  156,
  156,  398,  396,  396,  151,  151,  404,  404,  404,  404,
  404,  400,  400,  400,  400,  400,  333,  152,  152,  153,
  154,  154,  154,  154,  154,   51,   51,   20,   20,  240,
  405,   54,  123,  272,  272,   95,  399,  399,  399,  399,
  406,  406,  406,  406,  406,  406,  406,  406,  406,  406,
  406,  407,  408,  410,  409,  409,  412,  413,  413,  403,
  403,  401,  402,  402,  414,  414,  415,  415,  416,  416,
  419,  419,  417,  417,  422,  422,  421,  418,  420,  411,
  411,  411,  411,  411,  411,  411,  411,  334,  334,  155,
   48,  278,  122,  148,  376,  165,  213,  265,   63,   60,
  217,  112,   66,  339,  212,   35,   41,  347,  264,   39,
   42,  209,   40,  291,  258,  258,  423,  423,  423,  423,
  423,  423,  423,  423,  423,  423,  423,  423,  423,  423,
  423,  423,
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
    4,    2,    6,    4,    1,    1,    8,    9,    6,    8,
    6,    4,    0,    4,    2,    1,    3,    0,    2,    4,
    0,    1,    1,    3,    3,    7,    2,    0,    2,    2,
    3,    3,    0,    1,    2,    4,    1,    3,    2,    2,
    2,    1,    4,    7,    7,    1,    1,    3,    4,    7,
    2,    1,    3,    1,    1,    6,    3,    0,    0,    0,
    0,    3,    0,    9,    9,    1,    1,    0,    2,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    5,    5,    5,    5,    5,    5,    2,    0,    4,    2,
    3,    3,    2,    2,    3,    3,    2,    3,    6,    1,
    2,    4,    3,    2,    2,    2,    2,    3,    1,    3,
    3,    3,    2,    2,    4,    4,    5,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    1,    1,    1,    0,    2,
    2,    0,    1,    2,    2,    2,    2,    2,    4,    3,
    2,    2,    1,    3,    7,    1,    0,    1,    0,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    3,    2,
    2,    2,    1,    1,    1,    1,    4,    5,    1,    3,
    1,    3,    1,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    4,    4,    6,    5,    3,
    0,    2,    2,    0,    3,    0,    4,    1,    5,    4,
    1,    4,    1,    2,    2,    1,    1,    1,    2,    2,
    2,    2,    0,    3,    5,    1,    1,    2,    3,    1,
    2,    3,    0,    1,    1,    1,    5,    5,    6,    1,
    1,    1,    2,    4,    1,    0,    5,    1,    0,    3,
    2,    1,    0,    2,    0,    1,    0,    3,    1,    0,
    1,    2,    1,    1,    1,    1,    2,    2,    1,    2,
    3,    1,    1,    3,    3,    1,    2,    3,    1,    2,
    0,    2,    1,    1,    0,    1,    1,    1,    3,    2,
    3,    0,    1,    3,    4,    4,    1,    3,    4,    3,
    0,    1,    3,    3,    1,    1,    0,    1,    1,    4,
    0,    3,    0,    2,    0,    2,    0,    9,   10,    0,
    0,    0,    0,    3,    2,    1,    0,    3,    5,    3,
    2,    5,    2,    1,    1,    1,    1,    1,    3,    1,
    3,    1,    0,    2,    1,    3,    1,    1,    1,    6,
    1,    0,    3,    0,    1,    3,    6,    3,    4,    2,
    3,    0,    1,    3,    1,    1,    1,    2,    1,    1,
    1,    2,    1,    2,    1,    2,    0,    3,    0,    1,
    3,    1,    2,    0,    2,    0,    2,    0,    4,    1,
    2,    1,    1,    0,    1,    3,    2,    1,    1,    2,
    1,    4,    3,    1,    3,    4,    0,    8,    5,    1,
    3,    1,    1,    4,    4,    4,    1,    1,    5,    5,
    1,    3,    3,    1,    1,    7,    7,    5,    3,    0,
    1,    1,    1,    2,    0,    1,    1,    1,    0,    3,
    1,    3,    1,    0,    3,    1,    3,    1,    3,    3,
    1,    1,    3,    1,    2,    1,    3,    3,    1,    2,
    3,    3,    3,    3,    1,    3,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    3,    3,    3,
    3,    3,    3,    3,    3,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    1,    1,    5,    6,    3,    4,    5,    6,    3,
    4,    3,    4,    3,    4,    4,    5,    4,    4,    3,
    4,    1,    1,    1,    1,    1,    1,    1,    3,    3,
   10,   10,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    3,    3,    3,    3,    3,    3,    3,
    3,    1,    1,    1,    1,    3,    1,    1,    1,    1,
    1,    4,    1,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    1,    2,    1,
    1,    1,    1,    1,    2,    1,    1,    1,    1,    1,
    4,    5,    5,    5,    5,    5,    5,    5,    5,    5,
    5,    6,    1,    6,    1,    4,    7,    2,    0,    4,
    3,    6,    1,    1,    6,    6,    1,    1,    4,    6,
    4,    5,    3,    5,    4,    5,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  554,
  555,  556,  560,  561,  562,    0,  617,  732,  733,  737,
  738,  462,    0,    0,    0,    0,    0,    0,  400,  572,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  910,    0,   21,  908,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  480,    0,    0,   37,   34,
   36,    0,   35,    0,  995,  997,  999,  998, 1004, 1000,
 1007, 1005, 1001, 1003, 1006, 1002, 1008, 1009, 1010, 1011,
 1012,    0,    0,    0,    0,   32,   68,   66,  987,  996,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  646,    0,    0,    0,  687,    0,  989,
    0,    0,    0,    0,  107,  994,  563,    0,    2,    0,
    0,    0,    0,  430,  431,  432,  433,  434,  435,  436,
  437,  438,  439,  440,  441,  442,  443,  444,  445,  446,
  447,  448,  449,  450,  451,    0,  428,  429,    0,  979,
    0,  407,  983,    0,  404,  986,    0,  689,    0,  403,
  990,    0,  570,  906,    0,  150,  103,    0,  104,    0,
  218,  975,  771,    0,  102,  145,  982,    0,    0,   98,
    0,  105,  912,    0,   99,    0,  100,    0,  101,  993,
  146,  106,  625,  626,    0,  911,  909,    0,   74,    0,
  980,    0,  978,  488,  481,    0,  487,  492,  482,  483,
  490,  484,  485,  486,  491,    0,   30,    0,   39,  758,
   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  916,    0,  897,  640,
  640,  641,  651,  653,  968,  654,    0,  655,  645,  641,
    0,    0,    0,    0,    0,    0,    0,  589,  578,    0,
  581,  583,  584,  585,  586,  592,  593,    0,  988,  688,
    0,  108,    0,  109,    0,  110,    0,    0,    0,  618,
    0,    0,    0,    0,    0,    0,    0,    0,  463,    0,
    0,  370,    0,  221,    0,  410,  401,  460,  461,    0,
    0,    0,    0,    0,    0,  419,    0,  376,  377,    0,
  576,  574,  907,    0,    0,    0,    0,    0,    0,    0,
  153,  132,    0,   96,    0,    0,  111,    0,  121,  120,
    0,    0,    0,    0,    0,    0,  971,   75,    0,    0,
    0,  734,  735,  489,    0,    0,    0,    0,  875,  903,
    0,  970,    0,  877,    0,    0,  904,    0,  898,  889,
  888,  887,    0,    0,    0,    0,  879,  880,  881,  890,
  891,  899,  900,    0,  901,  902,    0,    0,    0,  905,
    0,    0,    0,    0,    0,  893,    0,  768,  872,  873,
  874,  685,  878,  353,  858,    0,    0,    0,  857,    0,
  683,  854,  855,  856,  859,  860,  861,  892,  917,  918,
  919,  920,  933,  935,  943,  944,  947,  948,    0,  761,
    0,    0,   27,    0,    0,    0,   71,   69,  991,   67,
   33,    0,    0,    0,  763,    0,   52,    0,    0,    0,
    0,    0,  568,    0,    0,    0,  648,  650,    0,  657,
    0,  745,    0,    0,  658,    0,  644,    0,  615,    0,
  590,    0,  587,    0,  588,  613,    0,  608,    0,    0,
  582,  772,  741,    0,    0,    0,    0,    0,    0,  565,
  564,    0,    0,  622,  619,    0,    0,  616,  186,    0,
    0,    0,  129,  468,  173,  174,    0,  471,  466,    0,
  472,    0,    0,    0,    0,  473,    0,  464,    0,    0,
  414,  416,  415,    0,  417,    0,  411,  219,    0,    0,
  272,    0,    0,    0,    0,  423,  424,    0,  453,    0,
    0,    0,    0,    0,    0,    0,  381,  380,  476,    0,
    0,  149,    0,    0,  158,    0,  157,    0,    0,  164,
  147,    0,  162,  165,  166,  154,    0,    0,  511,  546,
  514,    0,  547,    0,    0,  515,    0,    0,    0,  538,
  550,  510,  533,  512,  513,  508,  495,  370,  493,  494,
  496,    0,    0,  505,  506,  507,  509,    0,    0,    0,
    0,    0,  112,    0,  113,    0,  124,    0,    0,  202,
    0,    0,  368,    0,  221,    0,    0,   83,    0,    0,
  753,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  774,  776,  785,  788,  789,  790,  791,  792,
  793,  794,  795,  796,  797,    0,    0,    0,  894,    0,
    0,    0,    0,    0,    0,  895,  896,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  325,  992,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  760,   28,    0,    0,   44,   72,    0,
    0,   26,    0,  766,    0,    0,    0,  729,    0,    0,
    0,    0,    0,  567,    0,  640,    0,    0,    0,    0,
  662,    0,  558,    0,  914,  557,    0,  591,    0,  599,
  595,  594,    0,    0,    0,  610,  598,    0,    0,  739,
  740,    0,    0,    0,  368,    0,    0,    0,  632,  636,
    0,    0,    0,  130,    0,  470,    0,    0,    0,    0,
    0,  370,  413,    0,  370,  371,  418,  503,    0,  274,
    0,  268,  270,  368,  241,  976,    0,    0,  370,    0,
    0,  210,  368,    0,    0,    0,    0,  246,  240,  243,
  244,  245,  422,  458,  457,  421,  420,    0,    0,    0,
    0,    0,    0,  379,    0,  370,    0,  155,  159,    0,
    0,  163,    0,    0,  534,  535,  551,    0,  548,    0,
    0,  541,  539,  540,    0,    0,    0,    0,  504,    0,
    0,    0,    0,  542,    0,    0,  368,    0,  201,    0,
  204,  206,  207,    0,    0,    0,    0,    0,  221,   78,
    0,    0,   77,  522,  972,  523,    0,    0,    0,    0,
  775,  787,    0,    0,  845,  846,  847,    0,  779,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  960,  961,  962,  963,  964,  965,  966,  967,
    0,    0,    0,    0,    0,    0,  953,    0,    0,    0,
    0,  870,  327,  871,  941,    0,    0,  866,  974,    0,
    0,    0,    0,    0,  684,    0,  876,  770,  769,  762,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   61,
   63,    0,    0,   70,    0,    0,    0,  765,    0,    0,
    0,    0,    0,    0,    0,    0,  649,  652,    0,    0,
    0,    0,  667,  668,  669,    0,    0,  659,  984,  661,
  559,  915,  600,  609,  604,  603,    0,  614,  773,  742,
  743,  368,    0,    0,  642,  624,  623,    0,  182,  176,
  183,    0,  179,  469,    0,    0,  178,    0,  189,  170,
  169,  913,  172,  171,  474,    0,    0,    0,  412,  273,
    0,  212,  370,  214,    0,  371,  209,  215,    0,    0,
    0,    0,  425,  426,  370,  368,  371,    0,    0,    0,
  397,    0,  197,  198,  156,    0,  168,    0,    0,    0,
    0,    0,  549,    0,    0,  370,  371,    0,  499,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  199,    0,    0,    0,    0,  357,    0,   82,
    0,    0,    0,    0,   87,   84,    0,    0,    0,  736,
  977,    0,    0,  780,    0,    0,  786,    0,    0,    0,
    0,    0,  823,  822,    0,    0,    0,  830,  848,  840,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  778,    0,    0,    0,  221,    0,  921,    0,    0,  936,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  949,    0,  959,    0,    0,    0,    0,    0,
    0,  940,  882,   65,   57,   58,   64,   59,   60,    0,
    0,   22,    0,    0,    0,   42,    0,    0,    0,  747,
  767,    0,  730,    0,  746,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  690,    0,    0,    0,
    0,    0,  606,  607,  611,    0,  200,    0,    0,    0,
    0,    0,    0,    0,    0,  190,    0,    0,    0,  408,
  224,  225,  226,  227,  228,  229,  885,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  454,
  455,  427,    0,    0,    0,    0,    0,    0,  370,  478,
  371,  167,  973,    0,    0,  517,    0,  516,  552,    0,
  544,    0,    0,    0,  497,    0,  525,    0,    0,  527,
  530,  532,  368,    0,    0,    0,  119,    0,  205,  370,
    0,    0,  369,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  838,  839,  782,    0,  784,
    0,    0,    0,    0,    0,    0,    0,  841,    0,    0,
    0,    0,    0,    0,    0,  831,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  927,  926,    0,    0,  923,  922,    0,  931,  930,  929,
  928,  925,  924,    0,    0,  955,    0,    0,    0,  954,
    0,    0,    0,    0,    0,    0,   56,   62,   54,   55,
   23,  751,  752,    0,  749,  756,  757,  754,    0,    0,
  728,    0,    0,    0,  678,    0,    0,    0,  696,  692,
  694,    0,    0,    0,    0,    0,  369,  628,  629,  643,
    0,  184,  180,    0,    0,  195,  191,    0,  371,  886,
  280,    0,    0,  371,  278,    0,    0,    0,  232,    0,
    0,  248,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  475,  519,    0,  520,    0,  134,    0,    0,    0,
  138,  140,  141,  142,  500,  502,  498,  529,    0,    0,
  371,    0,    0,    0,    0,  354,    0,  358,  370,   88,
    0,    0,   90,    0,    0,    0,    0,    0,    0,  850,
  849,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
  942,  932,  934,    0,    0,  951,  950,    0,  956,  945,
  946,    0,   40,    0,    0,  731,    0,  681,  679,    0,
    0,    0,  700,    0,    0,  638,  371,  355,  630,    0,
  175,  192,  371,  144,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  301,  302,  304,  305,  306,  309,    0,    0,    0,  267,
    0,    0,  208,  233,    0,    0,    0,    0,  234,  237,
    0,    0,  371,  395,  396,  392,  391,  394,  393,    0,
  518,  545,  143,  131,  139,    0,  370,  264,  126,  371,
    0,    0,  372,  359,    0,    0,   91,   92,   79,    0,
  809,  817,    0,  810,  818,  813,  821,    0,    0,  811,
  819,  812,  820,  806,  814,  807,  815,  808,  816,    0,
  937,  952,    0,  748,    0,    0,  671,    0,    0,  710,
  712,    0,  707,    0,  265,  185,  409,    0,  312,    0,
    0,  315,  316,    0,  308,    0,    0,  307,  339,  340,
    0,  985,  293,  300,    0,  303,  286,    0,  291,    0,
  344,  292,    0,    0,  337,  211,  216,  235,    0,    0,
  238,    0,  250,  252,  399,  371,    0,  374,    0,    0,
    0,  364,  365,    0,    0,   76,   94,    0,    0,    0,
    0,  670,    0,  701,  711,    0,    0,  283,    0,  310,
    0,    0,    0,    0,    0,  341,    0,  332,  333,    0,
  352,    0,    0,    0,    0,  347,  345,    0,    0,    0,
  239,    0,    0,    0,    0,    0,  266,  371,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  675,    0,    0,
  718,    0,    0,    0,  282,  279,  311,    0,    0,  314,
    0,  322,  324,    0,    0,  351,  350,  981,  349,    0,
    0,    0,    0,    0,    0,    0,    0,  249,    0,  256,
    0,  255,  375,  367,    0,    0,  360,  118,    0,    0,
    0,    0,  673,  720,  709,    0,    0,  721,    0,  717,
    0,    0,    0,    0,  335,  334,  346,  348,    0,    0,
    0,  236,    0,    0,  260,  259,  258,    0,    0,  852,
  639,  676,  716,    0,    0,    0,    0,  313,  319,    0,
    0,    0,    0,    0,  263,  261,  262,    0,    0,    0,
    0,    0,  723,    0,    0,    0,    0,    0,  336,    0,
    0,    0,  722,    0,  330,  320,    0,  342,  317,  851,
  725,    0,  318,  726,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1555,   28,   29, 1556,
   31,   32,   33,   34,   35,   36, 1557,   38, 1558,  651,
  122,  462,  967, 1202,  123,  717,  968,  124,  466,  722,
  148,  125,  126,  259,  990,  138,  969,  970,  199,  228,
  127,  971,  128,  468,   82,  239,  378,  425,  646, 1103,
  426,  872,  617,  221,  849,  647,  648,  649, 1105,  240,
 1392,   74,  189,   75,  235, 1850, 1315,  220,  225,  227,
  229,  215,  207,  209,  222,  232,  155,  312,  314,  316,
  367,  633,  635,  459,  260,  798,  550,  661, 1039,  371,
  638, 1091, 1307,  583,  745, 1065,  532,  533,  565,  363,
  618, 1077, 1459, 1583, 1250, 1460, 1461, 1462, 1463, 1464,
  662,  216,  208,  359,  591,  360,  361,  592,  593,  594,
  595, 1282, 1033,  535,  536, 1022, 1020,  537, 1027, 1021,
 1240, 1422, 1028, 1029, 1245,  373,  860,  861,  862,  863,
  800, 1046, 1437,  619,  802,  428,  865,  948,  559, 1364,
 1251,  429,  430,  431,  432,  433, 1438, 1439,  568, 1589,
 1744, 1268,  808,  809,  795,  810,  811,  812, 1745, 1746,
 1826,  341,  563, 1259, 1559,  195,  560,  561, 1260, 1261,
 1432, 1719, 1560, 1561, 1562, 1563, 1680, 1564, 1565, 1566,
 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576,
  434, 1812, 1664, 1577, 1727, 1843, 1725,  435,  701,  436,
  437, 1578, 1110, 1681, 1735, 1736, 1777, 1096, 1313, 1476,
 1097, 1098,  287,  493, 1612,  382, 1232, 1415, 1700, 1701,
 1702, 1703, 1230,  350,  579,  826, 1280,  580,  824,   79,
   59,  345,  200,  163,  328,  186,  335,  192,  786,  783,
  336,  346,  816,  347,  575, 1057,  187,  438,  329,  547,
  581, 1281,   96,  198,  244,  620,  621,  788, 1078,  789,
 1079, 1080,  624,  625,  626,  627, 1071,  876,  628,  629,
  630,  842,  631,  839,   40,   41,   42,   43,   44,   45,
  157,  521,   61,  484,  203,  352,  299,  300,  301,  302,
  303,  304,  305,  306,  307,  751,  507, 1007, 1235,  508,
  509,  756,   46,  162,  322,  528,   47,  523,  524, 1016,
 1420, 1239, 1539,  769,  142,  739, 1536,  728,  487,  289,
  143,  144,  439,  288,  494,  495,  496,  742, 1000,  991,
  992,  993,  994,  995, 1646, 1712,  440, 1759, 1224, 1185,
 1532, 1533, 1761, 1654, 1762, 1763, 1764, 1810, 1851, 1853,
  474, 1214,   48,   49,  383,   50,   51,  515,  516,  976,
 1210, 1394, 1398,  475,  725,  213,  663,  664,  890,  891,
  665,  666,  667,  668,  669,  670,  671,  672,  673,  674,
  675, 1336, 1126, 1128, 1129,  947,  700,  442,  443,  444,
  445,  446,  447,  448,  237,  449,  450,  451,  452,  453,
  931,  454, 1515,  455,  456,  457,  458,  691,  935, 1186,
 1181,  692,  130,
};
static short yysindex[] = {                                   6230,
 1360, -157, 4158,  147, 1111,  118, 4108,  267,10061,   73,
  504, 9314, -157,  249,  570, 5231, 1374, 5231,  277,    0,
  835,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  475,    0,    0,    0,    0,
    0,    0, 9537, 5231, 5231, 5231, 5231, 5231,    0,    0,
  560, 1276, 5231, 5231, 5231,  718, 5231,  534, 5231, 5231,
    0, 5231, 5231,    0,  758,    0,  544,    0,    0,  805,
 5231,    0, 5231, 5231, 5231,  814, 5231, 5231, 5231, 5231,
  534, 5231, 5231, 5231, 5231,    0, 5231,  742,    0,    0,
    0, 1116,    0, 1116,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  779,  817,   75, 1219,    0,    0,    0,    0,    0,
 5231, 5231, 5231,  821,  911,  941,  127, 1013,  782,  106,
  119,  875,  839,    0, 5231, 1008, 1419,    0,  931,    0,
 5231, 5231, 5231, 5231,    0,    0,    0, 5231,    0, 1080,
  103, 1047, 1077,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  901,    0,    0, 1276,    0,
   51,    0,    0, 1382,    0,    0, 1642,    0,  462,    0,
    0,  963,    0,    0,  986,    0,    0, 1214,    0, 1170,
    0,    0,    0, 1276,    0,    0,    0, 1127, 1382,    0,
  556,    0,    0, 1134,    0, 1147,    0, 1116,    0,    0,
    0,    0,    0,    0, 1177,    0,    0, 1084,    0, 1215,
    0, 1159,    0,    0,    0, 1084,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 9281,    0, 5231,    0,    0,
    0, 4934, 1221, 5783, 5231,  913, 1290, 1539, 1322, 1337,
 4934, 1271, 5783, 1320, 1329, 1240,    0,10972,    0,    0,
    0,    0,    0,    0,    0,    0, 8553,    0,    0,    0,
 1284, 5231, 1398, 1328,  431, 5231, 1349,    0,    0, 1419,
    0,    0,    0,    0,    0,    0,    0, 5231,    0,    0,
 1382,    0, 1134,    0, 1116,    0, 1417,10972, 1394,    0,
 1389, 1387,  520, 1334, -120, 1350,  860, 1077,    0, 1589,
   83,    0, 5231,    0,  901,    0,    0,    0,    0, 5231,
 1472, 3670, 1450, 4414, 1879,    0, 9537,    0,    0,  579,
    0,    0,    0, 1869, 1869, 1276, 1584, 1276, 1247, 1214,
    0,    0,10834,    0, 5231, 5231,    0, 1472,    0,    0,
 1676,  -73, 1927, 5231, 1710, 5231,    0,    0, 1935,  244,
 5847,    0,    0,    0, 1944, 1964, 1979, 1550,    0,    0,
 1996,    0, 2010,    0, 2017, 2019,    0, 2021,    0,    0,
    0,    0,    0, 2022, 1590, 1603,    0,    0,    0,    0,
    0,    0,    0, 2025,    0,    0, 9378, 2026, 2027,    0,
10972,10972, 8651, 5231, 2046,    0, 2069,    0,    0,    0,
    0,    0,    0,    0,    0, 2030, 1991,    0,    0, 2045,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1512,    0,
 5231, 1652,    0, 5231, 5231,  179,    0,    0,    0,    0,
    0, 5231, 5231, 1125,    0, 5231,    0, 1772, 5231, 2049,
 4934, 1694,    0, 5231, 9617, 1923,    0,    0,10972,    0,
 2069,    0, 1779, 2056,    0, 1837,    0,   96,    0,  -32,
    0,  582,    0,  582,    0,    0, 2059,    0,  180, 1677,
    0,    0,    0, 2060,  149, 2048, 1472, 2075, 1858,    0,
    0, 1795, 2080,    0,    0, 1749, 1628,    0,    0,  164,
 1789, 1794,    0,    0,    0,    0, 1276,    0,    0, 1796,
    0, 1869, 1869, 1869, 1869,    0, 2081,    0, 1868, 1874,
    0,    0,    0, 1836,    0,10834,    0,    0,10834, 1840,
    0, 1382, 1884, 5231,12698,    0,    0,  677,    0, 5231,
 1054, 1642, 1758, 1723, 1690,    0,    0,    0,    0,  507,
 1764,    0, 1706,  534,    0, 1276,    0, 1894, 1824,    0,
    0, 1247,    0,    0,    0,    0,  228, 1727,    0,    0,
    0, 1728,    0, 1785, 2145,    0, 1842, 2156,  -19,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2106,  145,    0,    0,    0,    0,  128, 2158, 2159,
 2156, 1382,    0, 1886,    0, 1945,    0, 1276, 1276,    0,
 3670,  462,    0, 1823,    0, 1804, 2173,    0, 6780, 2453,
    0, 1830, 1838, 2181, 6450, 2184,    0,    0,    0, 5750,
 8579,   98,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1076, 9856,  176,    0, 5231,
10972, 1097, 1193, 1256, 2618,    0,    0,10972, 6680, 2069,
 1778,  547,10972, 9856,  804,  804,  249, 2042, 1870, 2187,
    0,    0, 9936, 5231,10972,10972,10972,10972,10972, 9856,
10972, 2090, 5231,    0,    0, 7167, 1797,    0,    0, 1766,
 5783,    0, 1914,    0, 1890,  249, 2192,    0, 1914, 7167,
 1916, 1917, 5231,    0, 2042,    0, 2098, 1396, 1782, 9856,
    0, 5231,    0, 1179,    0,    0,    3,    0, 1791,    0,
    0,    0, 5231,  828, 5231,    0,    0, 5231, 5231,    0,
    0, 9856, 1976, 3670,    0, 1745,10972, 5231,    0,    0,
 1276, 1276, 1276,    0,  -51,    0, 1706, 1706, 1792,  534,
  860,    0,    0, 2209,    0,    0,    0,    0, 1974,    0,
 5231,    0,    0,    0,    0,    0, 1981, 2213,    0, 1956,
 1970,    0,    0, 1913, 1915, 1116, 1983,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  534, 5231,12872,
 1872, 1876, 1877,    0,  534,    0, 1429,    0,    0, 1846,
 1276,    0,   74, 1850,    0,    0,    0,  534,    0, 2145,
  153,    0,    0,    0, 1967,  153, 1852,  153,    0, 2231,
  534,  534,  534,    0, 1472, 5231,    0, 1918,    0, 1906,
    0,    0,    0,  579, 1855, 5231, 1910,  272,    0,    0,
 2233, 2002,    0,    0,    0,    0,  244, 5231, 1860, 6928,
    0,    0, 1860, 6450,    0,    0,    0, 6978,    0,  165,
  636,10972,10972, 7025, 2239,  774, 7283,10972, 7435,   42,
 7544, 7803,10175, 7955, 8064, 8323, 6680, 6680,10972,10972,
 2016,10972, 2242,10972, 2240, 2288,10972,10972,10972,10972,
10972,10972,    0,    0,    0,    0,    0,    0,    0,    0,
 1973, 1321,  543,10972,  653, 9856,    0, 6680, 2001, 2246,
  875,    0,    0,    0,    0, 2069, 1911,    0,    0, 2014,
  804,  804,  769,  769,    0,  293,    0,    0,    0,    0,
 5231, 5231, 5231, 5231, 5231, 5231,  181, 2251,    0,    0,
    0, 7167, 1912,    0,  530, 1936, 5231,    0,  875, 9856,
 1936, 2255, 2251, 7167, 3894, 1982,    0,    0, 1362, 2265,
 2262, 1347,    0,    0,    0, 6680, 1984,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  514,    0,    0,    0,
    0,    0, 1919, 1855,    0,    0,    0, 2266,    0,    0,
    0, 1932,    0,    0, 1869, 2043,    0,  -51,    0,    0,
    0,    0,    0,    0,    0, 2277, 6680, 2683,    0,    0,
 2029,    0,    0,    0, 2683,    0,    0,    0, 1116, 1116,
 1817, 2284,    0,    0,    0,    0,    0,  -34,   31,  852,
    0, 2061,    0,    0,    0, 5231,    0,  147, 1942, 1904,
 1974, 2291,    0, 1706, 1950,    0,    0,  955,    0, 2278,
 5231, 1022,  534, 2296, 2300, 2302, 2082,  462, 2029, 1869,
 1794, 3670,    0, 1764,  875, 1893, 1908,    0,  766,    0,
  534, 1957, 2035, 6780,    0,    0,  534, 1861, 2041,    0,
    0, 2311, 2316,    0, 6680, 6680,    0, 6680, 6680, 2034,
 2069, 2321,    0,    0, 2069, 2322,10414,    0,    0,    0,
 2013, 2328, 2069, 2329, 1202, 2330, 2069, 2333,10972,10972,
 2239,10972,10494, 2334, 2069, 2337, 2341, 2069, 2344,10972,
 2069, 2348, 2069, 2351, 2352, 2069, 2355, 2358, 2069, 2359,
    0, 2139, 2406, 2572,    0, 2664,    0, 2688,10972,    0,
 2971, 3113, 3147, 3158, 3227, 3403,10972,10972, 9856, 2069,
 1968, 9856,    0,10972,    0, 2094,  635,10972, 9856,10972,
10972,    0,    0,    0,    0,    0,    0,    0,    0, 2079,
 7167,    0, 7167,  181, 2251,    0,  203,  203,  122,    0,
    0, 8553,    0, 1954,    0, 2255, 2251, 2363, 3894, 2367,
 1347, 2369, 9856, 1837, 1396, 2023,    0, 2024, 2031, 2071,
 2143, 2087,    0,    0,    0, 2029,    0, 1893, -113, 1999,
 1276, 2032, 1706, 2033, 1706,    0, 6680,  218, 1531,    0,
    0,    0,    0,    0,    0,    0,    0, 1987, 2154, 2029,
    0,10972,    0, 2140, 2036, 1817,  758,    0, 6680,    0,
    0,    0, 2044, 2052, 2058, 2066, 2068, 2070,    0,    0,
    0,    0,    0, 2387,  147,    0, 1792,    0,    0,  153,
    0, 2683, 2051,  153,    0,  153,    0, 1974, 2401,    0,
    0,    0,    0,  579, 2154, 1706,    0, 1794,    0,    0,
 8553, 2167,    0,  584,    0, 2266,    0, 2415,    0, 2405,
 1276,  207, 2423,    0, 1276,    0,    0,    0, 2139,    0,
 2139,10972, 2057, 2057,  249, 2424, 1955,    0, 2057, 2057,
10972, 2057, 2057, 2743, 2069,    0, 1707,10972, 2069, 2057,
 2057, 2057, 2057, 2069, 2057, 2057, 2057, 2057, 2057, 2057,
    0,    0,10834, 2425,    0,    0, 3452,    0,    0,    0,
    0,    0,    0, 3810, 1363,    0, 9856, 2160, 2038,    0,
 9856, 3818, 1971, 1519, 2069, 2086,    0,    0,    0,    0,
    0,    0,    0, 2050,    0,    0,    0,    0, 1779, 9856,
    0, 3894, 2363, 1837,    0, 1988, 5231, 1347,    0,    0,
    0, 1396,10972, 6680, 2088, 2154,    0,    0,    0,    0,
 1869,    0,    0, 1276, 1429,    0,    0,  421,    0,    0,
    0, 5231, 7678,    0,    0, 2069, 2207, 2140,    0, 1185,
 5231,    0, 2162,  596, 2053, 2155, 2062, 2200, 2164, 2202,
 2029,    0,    0, 2464,    0, 2470,    0, 2163, 2207, 2051,
    0,    0,    0,    0,    0,    0,    0,    0, 2029, 1764,
    0, 1429, 2061, 2196, 2127,    0, 1855,    0,    0,    0,
 2146,  -94,    0, 1974, 2147, 2069, 2478, 2482,  875,    0,
    0, 2483, 2486, 2069, 2488, 2489,10972,10972, 2069, 2490,
 2491, 2492, 2493, 2495, 2497, 2498, 2499, 2501, 2504,    0,
    0,    0,    0,10972, 2506,    0,    0, 9856,    0,    0,
    0, 1782,    0,  203, 1782,    0, 5231,    0,    0, 1406,
 2069, 2505,    0,   98,  802,    0,    0,    0,    0, 1706,
    0,    0,    0,    0,10834, 3057,  666, 2502, 2511, 2212,
10972, 2508, 2513, 3296, 2516, 2517, 2218, 2518,    0,    0,
    0, 2252,10634, 2522,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -44, 2525,    0,    0,
 2543, 5231,    0,    0, 2234, 2241, 5231, 1817,    0,    0,
 1116, 5231,    0,    0,    0,    0,    0,    0,    0, 2154,
    0,    0,    0,    0,    0, 2154,    0,    0,    0,    0,
 1678, 1782,    0,    0,10972, 1276,    0,    0,    0, 1276,
    0,    0,10972,    0,    0,    0,    0, 2069, 2069,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2069,
    0,    0, 1984,    0, 1984, 2547,    0, 6680,10972,    0,
    0, 2220,    0, 2554,    0,    0,    0,  528,    0,10733,
 5231,    0,    0,10972,    0, 6680, 2069,    0,    0,    0,
 2537,    0,    0,    0, 2338,    0,    0,  618,    0, 2152,
    0,    0,  654, 2557,    0,    0,    0,    0, 1817, 1116,
    0, 2221,    0,    0,    0,    0, 2061,    0, 1678, 2561,
 1347,    0,    0, 1984, 2069,    0,    0, 1519, 2087, 2087,
 5231,    0,   98,    0,    0, 6192, 2683,    0, 2549,    0,
 2578, 9281, 1248,  724, 2550,    0, 1636,    0,    0, 2564,
    0, 5231, 5231,  147,  232,    0,    0,  559, 2272, 6680,
    0, 2221,   65, 1817, 2235, 2244,    0,    0, 1347, 2575,
 1678, 2281, 2087, 2587, 1782, 2088, 2088,    0, 1993, 5231,
    0, 2588, 2586, 1052,    0,    0,    0, 2222, 2338,    0,
 2198,    0,    0, 2338, 2151,    0,    0,    0,    0, 7678,
  618,10972, 2338,  728, 1817,  799,  799,    0, 2343,    0,
 2189,    0,    0,    0, 1347, 1678,    0,    0, 1984, 2258,
 2258, 5231,    0,    0,    0, 6192, 2595,    0, 5231,    0,
  969, 2580, 1696, 7678,    0,    0,    0,    0, 1124,  202,
 2353,    0, 2179,   55,    0,    0,    0, 1447, 2087,    0,
    0,    0,    0, 5231, 2310, 2338, 2614,    0,    0, 2357,
 2338, 2372, 2362, 7678,    0,    0,    0, 6680, 2088, 2625,
 2629, 2631,    0, 1994, 7678,  335, 5231, 7678,    0,   98,
 2258, 5231,    0, 5231,    0,    0, 7678,    0,    0,    0,
    0, 2632,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  137, 1074,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  132, 6105,    0,    0,    0,    0,    0,    0,
 2674,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  186,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1075,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2339,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2292,    0,    0,
    0,  285,    0,  285,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2294,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1191,    0,
    0, 8890, 6364,    0,    0,    0, 1198,    0,    0,    0,
 1650,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  138,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -10,    0,    0,    0,    0,  426,    0,
    0, 1299,    0,    0,    0,    0,    0,  140,    0,11494,
    0,    0,    0,    0,    0,    0,    0,    0,  -10,    0,
  -67,    0,    0, 2639,    0,    0,    0, 2418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1302,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1435,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1223,    0,    0,
    0,    0,    0,    0,    0, 7131,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1096,    0,    0, 1466,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -10,    0, 2639,    0, 2418,    0, 1484,    0,    0,    0,
    0,  201,    0,    0,    0,    0,    0, 1534,    0, 2407,
    0,    0,    0,    0, 1594,    0,    0,    0,    0,    0,
 2420, 1174,12780,    0, 1619,    0,    0,    0,    0,  430,
    0,    0,    0, 2236, 2236,    0,    0,    0, 1620,  231,
    0,    0,    0,    0,    0,    0,    0, 2420,    0,    0,
 1448,    0,    0,    0,    0,    0,    0,    0, 9791, 2303,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1980,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2722,    0,    0,    0, 3026, 3330,    0,
    0,    0,    0,    0,    0,    0,  250,    0,    0,    0,
    0,    0,    0,    0,    0,  213, 3634, 2416,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1627,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0, 2375,    0, 2382,    0,    0,    0,    0,
    0,  483,    0,  483,    0,    0, 1163,    0, 1181, 1239,
    0,    0,    0,    0, 1302,    0, 2420,    0,    0,    0,
    0,  111,  280,    0,    0,  174,    0,    0,    0,  794,
    0,  391,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2236, 2236, 2236, 2236,    0,  656,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2433,    0,    0,    0,    0,    0, 2407,    0,    0,
  987,    0, 2228,    0,    0,  695,    0,    0,    0,    0,
  212,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1634,    0,    0,    0,    0, 3017, 4291,    0,    0,
    0, 4811,    0,    0,11309,    0,    0,11321,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1769, 1938,    0,    0,    0,    0,11333, 7286,    0,
11321,  -10,    0,    0,    0,    0,    0,    0,    0,    0,
 1174, 1033,    0,    0,    0,    0, 2289,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11602,11763,11906,    0,
    0,  486,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11052,    0,11052,    0,    0,
    0,11052,11052,11052,    0,    0,    0,    0,    0, 2259,
    0,    0,    0,    0, 4674, 4978, 9042, 2655,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  173,    0,    0, 6105,    0,    0,  173,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1003,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  603,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1174,    0,  318,    0,    0,    0,    0,
    0,    0,    0,    0, 1038,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1969,    0,
    0,    0,    0,    0,    0,    0, 2663,    0,    0, 3777,
 2248,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  446,  474,  619,    0,    0,    0,  234,    0,    0,    0,
    0,    0,    0, 3204,    0,    0,    0,    0,    0,11309,
    0,    0,    0,    0,  762,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2420,    0,    0,  334,    0,    0,
    0,    0,    0,    0,    0,    0,    0,10349,    0,    0,
    0,  237,    0,    0,    0,    0, 2403,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
11052,    0,    0,    0,    0, 1581,    0,    0,    0, 3938,
 5194, 5498, 4154, 4458,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1639, 1656,    0,    0,
    0,    0,    0,    0,    0, 1660,    0,    0, 8890,    0,
 1660, 1685, 1688,    0,    0,    0,    0,    0,    0, 1618,
  781, 1318,    0,    0,    0,    0,  313,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  188,    0,    0,
    0,    0,    0,    0, 2236, 2256,    0, 1154,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2442,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1774,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1708,    0,    0,    0,    0,    0,    0,    0,    0,
 3321,    0,    0,    0,    0,    0,    0,    0,    0, 1136,
    0,    0,    0,    0,    0,    0,    0, 1033, 2442, 2236,
 1721, 1174,    0, 2451, 8890, 1725,  131,    0,    0,    0,
    0,10349,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 5331,    0,    0,    0, 6052,    0,    0,    0,    0,    0,
    0,    0, 6444,    0, 8317,    0, 8852,    0,    0,    0,
    0,    0,    0,    0, 9177,    0,    0, 9257,    0,    0,
11360,    0,11969,    0,    0,12005,    0,    0,12041,    0,
    0, 5342,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2282,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1639, 1743,    0, 2280,  251,    0,    0,
    0,    0,    0,    0,    0, 1748, 1753, 1757,    0,    0,
 2376, 1035,    0, 5578,    0, 2378,    0, 2383, 2389,    0,
    0,  113,    0,    0,    0, 2442,    0, 1725,    0,  879,
    0,    0,    0, 2267,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7643,    0, 2467,
    0,    0,    0, 2938,    0, 1774, 2339,    0,    0,    0,
    0,    0,  681,  731,  741,  775,  932,  935,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  813,    0,    0,    0,    0, 3000,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1775,    0,    0,
    0,    0,    0,    0,    0, 1777,    0,    0,    0,    0,
    0,  159,    0,    0,    0,    0,    0,    0,  301,    0,
  330,    0,    0,    0, 9042,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12077,    0,12113,    0,12185,    0,
    0,    0,    0,12257,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2698,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1751,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1786, 2054,    0,    0,    0, 2182,    0,    0,
    0,    0,    0,    0,  761,    0,    0,    0,    0,    0,
 2236,    0,    0,    0,  924,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0, 2700, 1788, 3659,    0, 2407,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2442,    0,    0,    0,    0,    0,    0,    0, 1809,  866,
    0,    0,    0,    0,    0,    0,    0,    0, 2442, 2451,
    0,  369,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  300,    0,12370,    0,    0,11052,    0,
    0,    0,    0,12402,    0,    0,    0,    0,12486,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  777,    0,  251, 1161,    0, 3628,    0,    0, 2376,
 2649, 2819,    0,  358, 2702,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1254, 1829,    0,    0,    0,    0,    0,    0,
    0,    0,  610,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1831,    0,
    0,    0,    0,    0,    0,    0,    0, 3827,    0,    0,
  116,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  130,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12515,12549,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2705,
    0,    0,  745,    0,  973, 4299,    0,    0,    0,    0,
    0, 2703,    0,    0,    0,    0,    0, 2689,    0,    0,
    0,    0,    0,    0,    0,    0, 2691,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2444,
    0,    0,    0,    0,    0,    0,    0,    0, 3827,  606,
    0,  121,    0,    0,    0,    0,    0,    0,    0,  757,
  997,    0,    0,  252, 2700,    0,    0,    0,  143,  384,
    0,    0, 4819,    0,    0, 2702,    0,    0,    0,    0,
    0,   92,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  238,    0, 1774,  906,  974,    0,    0, 2376, 1559,
    0,    0,   89,    0,  777,  125,  161,    0,    0, 1162,
    0,    0, 2712,    0,    0,    0,    0, 2695,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  366,
    0,    0,    0,    0, 3827,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1160,    0,    0,    0,  745, 2714,
 1372,    0,    0,    0,    0, 2702,    0,    0,    0,    0,
    0,    0,    0,  366,    0,    0,    0,    0,    0, 2454,
    0,    0,    0,    0,    0,    0,    0, 2376,  143,    0,
    0,    0,    0,    0, 2011,    0, 2701,    0,    0,12578,
    0,    0,    0,  366,    0,    0,    0,    0,  125, 2720,
    0,    0,    0,    0,  366,    0,    0,  366,    0, 3410,
 2714,    0,    0,    0,    0,    0,  366,    0,    0,    0,
    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2762,    0,    0, 2770,
    0,    0,    0,    0,    0,    0,    4,    0, 2771,    1,
  258,  -76, -632, 1568,  847,  -63, -549, 2761, 2507,    0,
  -27,    0, 2521,  -93,  -40,    0,  690, -255,  -50,  -61,
  -22, -177, 2509, 2072,    0,    0,    0, 1352,    0,    0,
  -36, 1673, -582,   12, -937, 2134, 1920, -309, 1689, 2710,
 -247,    0,  -56,    0, -470,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -561,  -38, 1701, -291, 1023, -863,    0,
    0,    0,    0, -334, -123, -712, 1713, -327,  -48,    0,
 -527,    0,    0, 1342, -992,    0, 1346,    0,    0, 2477,
 -623,  -17,    0,    0,    0,    0, 2448,    0, 2217, 2487,
 -212, 1730, -694,    0,    0,    0, -735,    0,    0,    0,
    0,    0,    0, 1790,    0, 2503, 2067, 1722, 2475, 2479,
    0, 1164,    0, -525, 2063,  -41, -636, 1238,    0,    0,
    0, -944, -943, -941, -275, -928,    0, 1385, -921,    0,
 1082,-1207, 1392,    0, 2263,    0,    0,    0, 1079, 1089,
 1048,  126, -260, -826, -363,    0,    0, 2074,    0, 1576,
    0,    0, -317,    0,    0,    0,    0, -183,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1122,    0, 1115,    0, -461,    0,    0, -259,    0, -388,
 -166, 1305, 1004, 1182,    0, 1085,    0, 1859, 1631, 1458,
    0,-1074, -879, -905,    0, -496, -765, -178,    0, -741,
 1178,  952, -328, -536, -755, -947,-1286,    0,    0,   23,
    0,    0,    0,    0,    0, 2532,    0,    0,    0,    0,
 2545, 2314,    0,    0,    0,    0,    0,   -9, 2560,    0,
    0,    0,    0,  -15,    0,    0,    0, -339, 2047, -335,
 1595, -459,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2269,    0, 2064,    0,    0,    0,    0,    0,    0,
    6,    0, 2883,    0,    0,    0,    0,    0, 2597,    0,
    0,    0,    0, 2402,    0, 2397,    0,    0,    0, 2150,
    0,    0,    0, -294,    0,    0,  158,    0, 2142,    0,
    0,    0,    0,    0, -627,-1082, -785,  355, -174, 1309,
    0, 2764, -127, 1324,    0, 2170, -612, -983,-1415,    0,
 -813, 1925,    0,    0,    0,    0, -145,    0,    0, -246,
    0, 1263, 1388,    0, 1114,    0, 1169,    0,   -5,    0,
    0,    0,    0,    0, 2419,    0,    0,    0, -391, 2193,
 1949,-1005,    0,    0,    0, -689, -427,  443,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2953, 1252, 1798,    0, -555,    0,    0,    0, -918,
    0,    0,    0, 1682,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -248,
 1752,    0,    0,
};
#define YYTABLESIZE 13389
static short yytable[] = {                                     129,
  151,  191,  129,   37,   78,  310,  150,  214,  156,  441,
  261,  492,  281,  284,  210,  194,  280,  283,  760,  226,
  584,  211,  959,  622,  556,  206,  219,  623,  245,  197,
  787,  590,  254,  790,  699,  249, 1023,  799,  746,  801,
  554,  224,  253,  188,  190,  193,  196,  150,  201,  250,
  231,  766, 1263,  212,  190,  217,  256,  196, 1442,  150,
  201, 1190,  230,  129,  911,  933,  873,  151, 1009,  941,
  248,  241,  255,  150,  243,  190,  252,  241,  217,  193,
  196,  724,  150,  201,  230,  129,  467,  196,  704,  267,
  223,  269,  315, 1252, 1253,  467, 1254,  982,  979, 1212,
 1252, 1253,  251, 1254,  268,  864,  488,  636, 1094, 1256,
  627,  311,  704,  223,  546,  759, 1256, 1068,  265, 1257,
  257,  212,  150,  212,  513,  313, 1257,  291, 1014,  706,
  356,  573,  652, 1288,  375,  217,  573,  633, 1069,  151,
  744,  309,  196,  150,  230,  278, 1310,  704,  156, 1011,
  328,  627,  337,  704,  627,  956,  759, 1041,  282,  759,
  708,  257,  514,  317,  257,  708, 1048,  851,  279,  627,
  265,  704,  750,  635,  759, 1221,  441,  364,  633,  257,
  983,  279, 1264,  704,  279,  621, 1610,  634,  706,  356,
  573,   77,  759, 1272,  478,  573,  633, 1074,  151,   89,
  637,  708, 1395,  772,  774, 1117, 1018,  777,  778,  779,
  780,  398,  326, 1293,  635, 1311,  460,  913,  122,  708,
 1089,  519,  721,  755, 1201,  622,  621,  881,  634,  623,
  152,  750,  635,  196,  463,  848,  539,  257,  499, 1478,
 1407,  637,  997,  463,  621, 1774,  634,   77,  212,  686,
  524,  699,  150,  269,  469,  129,  763,  843, 1429,  637,
 1617,  150, 1305,  469,  590,  279,  512,  833,  506,  135,
  398,  326,   77, 1683,  639, 1781,  807,   85,  257,  620,
   85,  257,  193, 1161, 1162,  502,  150,  719,   77,  152,
  686,  555,  196,  686, 1055, 1758,  257,   60,  558, 1457,
  211, 1522,  571, 1273, 1025,  573, 1399,  338,  686,  524,
  699, 1102,  699, 1139, 1187,  634, 1525,  631,  320,  585,
  620,  587,  644,  212,  632,  844,  845,  514,  759, 1140,
  212, 1275,  212,  125,  212,  640, 1191,  576,  620, 1204,
  526,  781, 1440,  526,  368,  131,  642, 1252, 1253,  834,
 1254, 1216, 1218,  699, 1846,  196,  201,  703,  631,  907,
 1467,  631,  319, 1256,  150, 1786,  193, 1213,  196,  552,
  783,  699,  747, 1257, 1141, 1236,  631, 1026, 1774, 1418,
 1691, 1075,  553,  704,  339, 1193, 1832,  204,  205, 1419,
  465,  297,  125,  122,  122,  274,  540, 1618,  703,  744,
  269,  492, 1614, 1142,  732, 1474, 1847, 1274, 1684, 1416,
 1748, 1408,  132, 1248,  702,  731,  703, 1452,  151, 1271,
 1527,  847, 1205,  718,  704,  378, 1115,  196,  627,  477,
  704,  512,  771,  715, 1217,  285,  517,   83,  718,  151,
  744,  504,  704,  744,  723,  384,  273,  940,  729,  465,
  627,  150,  704,  463,  196,  469, 1114,  706,  744,  827,
  706, 1543,  150,  955,  492, 1842,  150,   89,  151,  196,
 1143,  150, 1276,  382,  156,  398,  525,  633,  912,  907,
 1482, 1741,  601,  908,  378,  705,  492,  151,  477,  734,
 1036, 1328, 1329, 1038, 1330, 1331,  151,  754,  759,  704,
  775,  328,  151,  621,  384, 1423, 1787, 1044,  704,  152,
  264,  708,  196,  635, 1396,  257, 1440,  257, 1644,   14,
  151,  873, 1607,  803,  257,  621,  705,  634,  704,  704,
  152, 1032,  382,  196, 1062, 1780, 1788, 1316,  704,  750,
  637,  601,  704,  467,  705,  573, 1619,  708, 1470,  829,
 1116, 1304,  704,  277,  796,  212,  686,  686, 1104,  152,
  796,  987,  196,  277,  627, 1544,  704,  573, 1035,  706,
 1580, 1337, 1581,  526,  850, 1010,  277, 1822,  152, 1032,
  743,  196,  699,  706,  356, 1211, 1403,  152, 1717,  196,
  196,  633, 1455,  152, 1087,  828,  196,  620, 1530,  211,
  381,  858,  859,  908,   76,  759,  223, 1608,  526, 1623,
  277,  152,  627,  514,  196,  257,  759,  524,  383,  620,
 1002,  257,  326, 1428, 1600, 1755,  196,  635,  196, 1109,
  699,  212,  720,  650, 1200,  631, 1593,  257, 1867,  621,
  875,  634, 1606,  699,  257, 1444,  759,   85,  465,  759,
  874,  196,  699, 1030, 1031,  467,  151,  631,  650,  686,
   76,  465,  915,  744,  759, 1101, 1469,  759,   97,  986,
  217,  960, 1483, 1655,  650,  703,  196,  383,  257, 1657,
  389,  125,  907,  338,  378,   76,  781,  793, 1541,  378,
 1243,  699,  718,  477,  949,  378,  465,  703,  465,  699,
  686,   76,  212,  212,  492,  699,  469, 1489,  699,  384,
  526,  469, 1542,  502,  467,  783,  196,  512,  348,  744,
  469,  289,  211,  196, 1765,  506,  460, 1008,  196, 1694,
  390,  338,  999,  620, 1019, 1019, 1024,  382,  257,  389,
  386,  140,  141,  150,  703,  150, 1698,  152,  212,  558,
  196, 1262,  699,  703,  212, 1306,  361,  855,  212, 1609,
  708,  135, 1255, 1270, 1771,  526,  699, 1051, 1821, 1255,
  704,  631, 1252, 1253,  385, 1254,  133, 1054, 1056,  704,
  664,  212,  158, 1194, 1292,  699, 1197, 1526, 1256,  390,
 1534, 1034, 1104,  177, 1067,  349, 1443,  703, 1257,  386,
 1383,  708,  223,  705,  907,  258,  908,  821,  465,  212,
  212,  703,  136,  705,  503,  361,  705,  706,  338,  708,
  135,  664,  378,  369,  601,  705,  477, 1045, 1088, 1053,
 1456,  514, 1747,  385, 1406, 1510, 1466,  577, 1099,  664,
  223, 1754,  384, 1477,  822,  708,  150, 1061,  578, 1207,
  709,  744,  177,  936,  937, 1363,  150,  907,  136,  744,
 1072,  160, 1084, 1085, 1086,  137,  338,  529, 1111,  504,
  382,  136,  705,  223,  223,  223,  530, 1709,  181, 1710,
 1731,  705,  383,  759, 1793,  759,  505,  601,  744,  744,
  601,  744,  759,  159,  145, 1434,  907, 1118,  744,  744,
  744,  744,  744, 1198,  744,  253,  601,  744,  744,  744,
  744,  744, 1196,  467,  161,  744,  744,  287, 1233,  181,
  370, 1195,  181,  196,  137,  705,  467,  338,  908, 1732,
 1376,  388,  601, 1378,  387,  512,  492,  181, 1753,  705,
 1733, 1471,  441, 1199,  389, 1388,  253, 1390,  823,  253,
 1002,  469,  196,  201,  469,  230,  129, 1749,  749, 1182,
 1183,  467,  469,  467,  253, 1208, 1738,  149,  202, 1234,
 1800, 1801,  699,  254,  469,  469,  441, 1179,  218,  196,
  774,  908,  196,  759,  275,  907,  459, 1451,  759,  907,
  388,   76, 1220,  387,  390,  804,  362,  938, 1294,  146,
 1095,  236,  706, 1662,  386,  759,  147,  605, 1836, 1795,
 1265, 1266,  759,  699,  254,  383, 1255,  254, 1473, 1658,
  908, 1119,  233,  622, 1713, 1643,  424,  623, 1645,  750,
  459,  699,  254, 1829,  242,  492,  531,  187,  385,  135,
  362,  135, 1724,  706, 1734,  459,  759, 1295,  704,  797,
  211,  177, 1537,  705, 1828,  362, 1283,  460,  605,  234,
 1317,  706,  338, 1861,  177, 1294, 1663, 1661, 1284, 1381,
   14, 1283,  177,  467,  575,  699,  805,  389,  708, 1393,
 1393, 1397,  212,  704,  361,  959, 1540,  361,  705,  212,
  370,  975,  136,  177, 1299,  596,  187,  882,  664,  177,
  708,  177,  889, 1184,  706,  223,  759,  706,  664,  908,
 1662,  664, 1319,  908, 1298, 1704, 1784,  806, 1323, 1425,
  664, 1427,  177,  223,  492, 1130,  238,  390, 1516,  223,
  429,  699, 1519,  575,  285,  246,  181,  386,  542,  135,
  699,  177,  257,  370, 1650,  137, 1388,  708,  807,  181,
  177, 1131, 1277,  188,  596,  258,  177,  181,  330,  363,
  706,  262,  602,  706,  975,  708,  706,  664,  707, 1651,
  709,  385,  706, 1663,  177, 1823,  664,  286,  181,  501,
  612,  196, 1472,  253,  181,  253,  181, 1615,  543, 1278,
  571,  469,  253,  469,  196,  388,  361,  580,  387,  263,
  708,  706,  196,  363, 1019,  270,  331,  181,  361,  469,
  361,  177,  188,   99,  708,  196, 1824,  276,  363,  706,
  664,  602,  544,  196, 1860,  105,  181, 1652,  501,  196,
 1005,  196,  664, 1112,  664,  181, 1695, 1113,  597,  612,
  545,  181, 1696,  708,  706, 1679,  707, 1006,  709,  571,
  101,  254,  196,  254,  106,  107,  580,  108, 1799,  181,
  254, 1529,  266, 1825,  109,  110,  111,  112,  113, 1642,
  114,  196,   98,  115,  116,  117,  118,  119,  427,  196,
  196,  120,  121,  253, 1481, 1454,  196,  514, 1485,  708,
  706,  378,  707,  271,  709,  187,  181,  597,  577,  997,
  486,  706,  378,  699,  196,  253, 1770, 1813,  187,  491,
  177,  997,  253,   99,  997, 1697,  187,  665,  332,  102,
  706, 1820,  882,  272,  362,  100,  882,  362,  388,  103,
  706,  387,  141,  706,  323,  285,  333,  187,  115,  697,
  522,  196,  706,  187,  292,  187,  253,  324,  308, 1837,
  101,  254,  196,  318,  104,  667,  285,  577,  665,  699,
  706,  665,  708,  706,  321,  707,  187,  709,  699,  667,
  334,  621, 1752,  254, 1854,  115,  665,  667,  909, 1856,
  254, 1647,  325, 1545,  326,  187,  351, 1019, 1807,  706,
  211,  512,  469,  667,  187,  181, 1393,  309,  706,  917,
  187,  989,  196,  704,  708,  706,  253,  707,  705,  709,
  115,  188,  621, 1591,  254, 1728, 1656,  514,  187,  102,
 1752,  340,  212, 1579,  188, 1808,   80,   81,  105,  103,
  621,  150,  188,  362,  328,  989,  362,  596, 1809,  690,
  196, 1255,  706,  695,  696,  698,  353,  667,  362,  372,
  362,  242,  285,  188,  104,  187,  706,  106,  107,  188,
  108,  188, 1817, 1841,  374,  579, 1752,  109,  110,  111,
  112,  113,  596,  114,  254,  441,  115,  116,  117,  118,
  119,  704,  188,  566,  120,  121,  705,  363,  706, 1660,
  363,  706,  242,  328,  327,  919, 1840, 1692,  719, 1752,
  596,  188,  697,  596,  602, 1830, 1831,  735, 1729, 1728,
  188,  737, 1341,  376, 1815,  285,  188,  309,  814,  596,
  815,  512,  612, 1728,  579,  542, 1859,  704,  365, 1730,
 1756, 1757,  705,  405,  188,  719,  190, 1866, 1693,  602,
 1869,  726,  566,  377, 1672,  596,  588,  706,  719, 1873,
  380, 1728,  714, 1579,  187,  713,  706,  612,  921, 1690,
  708,  706, 1585,  707,  366,  709, 1870,  602,  727,  354,
  602,  188,  949,  242, 1797,  543, 1728,  150,  473, 1706,
  597, 1728,  193, 1707, 1586,  612,  602, 1769,  612,  379,
  489, 1587,  405,  406,  529,  151, 1742,  384,  497,  363,
  704,  150, 1729,  530,  612,  705,  355, 1816,  356,  544,
  381,  363,  602,  363,  242,  597, 1729,  682,  402,  160,
 1722,  883,  464, 1730,  883, 1588,   43,  545, 1730,  472,
  612,  357,   51,  161,  514,  665,  277, 1730,   41,  764,
 1178,  476,  704,  597, 1729,  665,  597,  705,  665,  689,
 1849,  196,  406,   52,  477,   24,  358,  665,  682,  755,
  697,  682,  597, 1001,   53, 1730,  764, 1226,  514, 1729,
  188,   54,  479,  883, 1729, 1776,  682,  402,  160, 1774,
 1514, 1227,  888,  151,   47,   43, 1739,   49,  597,  150,
 1730,   51,  161,  689, 1773, 1730,   55,   41,  514,  491,
 1760,  999,  481,  916,  665, 1228,  150,  479,  689,  514,
  932,  482,  514,  665,   24,  939,  427, 1699,  755,  498,
  127,  514,  190, 1778,  373,  946, 1226,  950,  951,  952,
  953,  954,  427,  946, 1779,  151,  204,  205,  512, 1774,
 1227,  150,   25,   47, 1760,  483,   49,   48,  708,  706,
  150,  707,   50,  709, 1839, 1835,   45,  665,  500, 1229,
  293,   56,  491,  589, 1228,  212,  479, 1226,  503,  665,
 1579,  665,  512,  251,  128,  152,  116,  501,   14,  127,
  151, 1227,  510,  373,  491,   46,  150,  217, 1648,  522,
 1760,  884,  999,   57,  884,  294,  150,   58,  704,  193,
  520,   25,  512,  705, 1579, 1228,   48,  153,  217,  503,
   14,   50,  503,  512,  251,   45,  512,  251, 1229, 1063,
 1064,  105,  154,  295,  193,  512,  296,  503,  217, 1848,
  526,  217,  251,  128, 1579,  116,  708,  706,  738,  707,
  527,  709,  297,  884,   46, 1579,  217, 1111, 1579,  538,
  106,  107,  193,  108,  193,  105, 1871, 1579, 1872, 1229,
  109,  110,  111,  112,  113,  541,  114,  217,  298,  115,
  116,  117,  118,  119, 1002,  549,  988,  120,  121,  364,
  792,  682,  562,  791,  106,  107, 1002,  108,  985, 1002,
 1387,  975, 1389,  364,  109,  110,  111,  112,  113,  342,
  114,  364,  343,  115,  116,  117,  118,  119,  123,  123,
  943,  120,  121,  710, 1120, 1121, 1125,  364,  569, 1133,
 1135, 1137,  572, 1145, 1148, 1151, 1153, 1156, 1159,  582,
  978, 1163, 1164,  977, 1166,  682, 1168,  526,  682, 1171,
 1172, 1173, 1174, 1175, 1176,  682, 1093,  344,  682, 1092,
 1100, 1192,  682,  869, 1191,  586, 1180,  682,  427, 1237,
  682,  637, 1092,  736,  708,  706,  641,  707,  526,  709,
  689,  364, 1242,  643,  645, 1241,  682,  689,  526,  995,
  689,  526, 1286,  676,  689, 1285,  704,  400,  401,  402,
 1291,  705,  689, 1290, 1401, 1491,  526, 1400, 1191,  910,
  682,  914,  491,  677,  682,  918,  920,  922,  689,  526,
  679, 1521,  526,  682,  710,  410,  411, 1498,  678,  971,
  995,  995,  995,  995,  995,  995,  995,  526, 1528,  526,
  682,  710,  689, 1803, 1865,  680, 1802, 1774,  995,  995,
  995,  995,  708,  706, 1188,  707,  503,  709,  503,  681,
  686,  727,  503,  663,  727,  503,  682,  682,  683,  233,
  684,  685,  689,  687,  688,  693,  694,  689,  503,  682,
  995,  682,  995,  710,  704,  708,  706,  682,  707,  705,
  709,  711,  942,  708,  706,  703,  707,  716,  709,  689,
  712,  730,  721,  663,  663,  733,  234,  663,  738,  740,
  741,  689,  753,  689,  757,  758,  682,  682,  762,  682,
  708,  706,  663,  707,  764,  709,  682,  682,  682,  682,
  682,  765,  682,  767,  781,  682,  682,  682,  682,  682,
  768,  958,  770,  682,  682,  785,  773,  105,  988,  708,
  706,  529,  707,  776,  709,  782,  503,  794, 1134,  946,
 1138,  784, 1146, 1149,  818, 1154, 1157, 1160,  819,  820,
  825, 1344, 1345,  277, 1347, 1349,  106,  107,  503,  108,
  830,  831, 1354,  835,  836,  503,  109,  110,  111,  112,
  113,  666,  114,  837,  838,  115,  116,  117,  118,  119,
  840, 1367,  958,  120,  121,  841,  846,  852,  853, 1374,
 1375,  427,  704,  856,  427,  866, 1180,  705,  857,  503,
 1382,  427, 1384, 1385,  868,  526,  869,  526,  877,  878,
  879,  526,  666,  883,  526,  666,  526,  944,  934,  973,
  975,  980,  972,  996,  491,  984,  985,  526, 1003, 1012,
  666,  995,  526,  995,  995,  427, 1015,  493, 1037, 1032,
  847,  995, 1043,  797, 1042, 1045, 1049, 1058, 1050,  995,
 1052, 1059, 1060, 1066,  995, 1070, 1076,  995,  526, 1081,
 1083, 1095, 1107, 1090,  526, 1108,  726,  526, 1127, 1165,
  704,  995, 1167, 1169, 1436,  705,  995,  995,  493, 1189,
  995,  493, 1177,  704, 1203, 1332, 1206,  995, 1201,  995,
  995, 1219, 1209,  995, 1223, 1225,  493,  995, 1244,  713,
  995, 1231,  995,  704,  995,  526, 1247, 1267,  705,  995,
  995,  704,  995, 1269, 1279, 1287,  705, 1258, 1170,  708,
  706, 1289,  707,  491,  709, 1296, 1300,  526,  995,  995,
 1301,  995, 1302, 1321,  526, 1303, 1312, 1314,  704, 1325,
 1101, 1326, 1324,  705, 1486,  995, 1327,  995,  995,  995,
 1333, 1334,  995, 1494, 1338,  995,  995, 1339, 1340, 1342,
 1499,  663, 1343, 1350,  663,  995, 1351,  704,  526,  526,
 1352,  663,  705, 1353,  663,  957,  526, 1355,  663,  995,
 1356, 1357,  995,  663, 1358,  424,  663, 1359, 1360,  427,
  907, 1380, 1377,  427,  526, 1386, 1402, 1404,  995, 1405,
 1409, 1410,  663, 1412,  995,  975, 1413, 1414, 1411,  995,
 1421, 1424,  491, 1426, 1433,  660,  564, 1453, 1458, 1445,
  995,  995, 1431,  995,  526, 1531,  663, 1446,  526,  995,
  663, 1468, 1441, 1447, 1475, 1480, 1361,  708,  706,  663,
  707, 1448,  709, 1449, 1479, 1450,  975,  975,  975,  975,
  975,  988,  975, 1484, 1490, 1511,  663, 1517,  995,  995,
 1523,  995, 1518, 1335,  975,  975,  975,  975,  995,  995,
  995,  995,  995, 1535,  995, 1524, 1582,  995,  995,  995,
  995,  995, 1595,  663, 1594,  995,  995,   77, 1592,  666,
 1597, 1598, 1599, 1596, 1601,  663,  975,  663,  975,  666,
 1602, 1613,  666,  663, 1603, 1611, 1616, 1620, 1621, 1628,
 1629,  666, 1622, 1624,  697,  493, 1625,  493, 1626, 1627,
 1630, 1631, 1632, 1633,  493, 1634, 1640, 1635, 1636, 1637,
  427, 1638,  663,  663, 1639,  663, 1641,  493, 1649,  105,
 1666,  132,  663,  663,  663,  663,  663, 1675,  663, 1677,
 1665,  663,  663,  663,  663,  663, 1668,  704,  666,  663,
  663, 1669,  705, 1667, 1673, 1674, 1676,  666,  106,  107,
 1682,  108, 1685, 1686, 1689, 1688, 1711, 1715,  109,  110,
  111,  112,  113, 1716,  114, 1726, 1740,  115,  116,  117,
  118,  119, 1678, 1743, 1751,  120,  121, 1766, 1772, 1775,
  105, 1783, 1362,  708,  706, 1794,  707, 1789,  709,  708,
  706,  666,  707, 1796,  709,  493, 1791, 1798, 1805, 1806,
 1787, 1811, 1814,  666, 1834,  666, 1767, 1705, 1838,  106,
  107, 1845,  108, 1786,  160, 1708, 1852,  493,  702,  109,
  110,  111,  112,  113,  493,  114, 1844, 1774,  115,  116,
  117,  118,  119, 1855, 1857, 1858,  120,  121, 1862, 1863,
 1864, 1531, 1874,    1,   29,  627,   31,  975,  203,  975,
  975,  759, 1721,  271,  370,  704, 1723,  975,  493,  702,
  705,  524,  702,  148,  656,  975,  269,  456,  686,   81,
  975,  660,  213,  975, 1365,  708,  706,  702,  707,  958,
  709,  524,  276,  193,  398,  524,  957,  975,  697,  975,
  695,  999,  975,  975,  194,  691,  975, 1249, 1366,  708,
  706,  693,  707,  975,  709,  975,  975,  275,  939,  975,
  371,  714,  713,  975,  427,  938,  975,  223,  975,  323,
  975,  288,  715,  331,  621,  975,  975,  343,  975,  329,
  724,   27,  999,  999,  999,  999,  999,  999,  999,   30,
   39, 1391,  137,  470,  975,  975, 1322,  975,  867,  480,
  999,  999,  999,  999,  708,  706,  471,  707, 1106,  709,
 1320,  975,  974,  975,  975,  975,  247,  105,  975, 1318,
 1604,  975,  975, 1308, 1819, 1605,  551,  596,  832,  534,
 1297,  975,  999, 1309,  999,  518,  566, 1246,  698, 1687,
  567, 1718, 1584, 1785, 1792,  975,  106,  107,  975,  108,
 1013, 1590,  813, 1790, 1827, 1435,  109,  110,  111,  112,
  113,  663,  114, 1768,  975,  115,  116,  117,  118,  119,
  975,  704, 1782,  120,  121,  975,  705,  704, 1671,  698,
 1868, 1737,  705, 1047, 1040, 1818,  975,  975, 1417,  975,
  663,  663, 1238,  663, 1538,  975, 1750,  698,  574,  557,
  663,  663,  663,  663,  663,  817,  663,  548, 1465,  663,
  663,  663,  663,  663, 1082,  139,  511,  663,  663,  854,
  752,  748, 1004, 1073,  975,  975,  290,  975, 1017,  998,
   76, 1714,  105, 1222,  975,  975,  975,  975,  975, 1833,
  975,  981, 1653,  975,  975,  975,  975,  975, 1804, 1215,
 1430,  975,  975,  761,    0, 1379,    0,  230, 1346,    0,
    0,  106,  107,  704,  108,    0,    0,    0,  705,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,  702,  704,  120,  121,
    0,    0,  705,    0,    0,    0,    0,  388,  230,  702,
    0,  230,    0,  999,    0,  999,  999,    0,  702,    0,
    0,    0,    0,  999,    0,    0,  230,    0,    0,  526,
    0,  999,    0,    0, 1497,  390,  999,    0,    0,  999,
    0, 1368,  708,  706,    0,  707,  524,  709,    0,    0,
    0,    0,  704,  999,    0, 1004,    0,  705,  999,  999,
    0,    0,  999,    0,  392,  702,    0,    0,    0,  999,
  526,  999,  999,  526,  702,  999,    0,    0,    0,  999,
    0,    0,  999,    0,  999,    0,  999,  524,  526,    0,
  524,  999,  999,    0,  999,    0, 1004, 1004, 1004, 1004,
 1004, 1004, 1004,    0,    0,  524,    0,  524,    0,    0,
  999,  999,    0,  999, 1004, 1004, 1004, 1004,  702,  923,
  924,  925,  926,  927,  928,  929,  930,  999,    0,  999,
  999,  999,  702,    0,  999,    0,    0,  999,  999,  397,
    0,    0,    0,    0,    0, 1659, 1004,  999, 1004,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,  999,    0,    0,  999,    0,  698,    0,    0,  400,
  401,  402,    0,  204,  205,    0,    0,    0,    0,  698,
  999,    0,    0, 1369,  708,  706,  999,  707,  698,  709,
    0,  999,  405,  406,  407,  408,  409,  410,  411,  412,
  413,    0,  999,  999,    0,  999,    0,  415,  416,    0,
    0,  999,    0,    0,    0,    0,  420, 1370,  708,  706,
    0,  707,    0,  709,    0,    0,    0,    0, 1371,  708,
  706,    0,  707,  521,  709,  698,    0,    0,    0,    0,
  999,  999,    0,  999,  698,  242,    0,  230,    0,    0,
  999,  999,  999,  999,  999,    0,  999,    0,    0,  999,
  999,  999,  999,  999,    0,    0,    0,  999,  999,    0,
    0,    0,    0,    0,  521,    0,    0,  521,    0,    0,
  704,    0,    0,    0,    0,  705,    0,    0,  698,    0,
    0,    0,  521,    0,  521,    0,    0, 1372,  708,  706,
    0,  707,  698,  709,  524,    0,    0,  526,    0,  526,
    0,    0,    0,    0,    0,    0,  526, 1004,    0, 1004,
 1004,    0,    0,  524,  524,    0,  524, 1004,    0,  526,
  524,    0,    0,  524,    0, 1004,    0,    0,    0,    0,
 1004,    0,    0, 1004,    0,  242,  524,    0,    0,    0,
  526,    0,  524,    0,    0,    0,    0, 1004,    0, 1000,
    0,    0, 1004, 1004,    0,    0, 1004,  242,    0,    0,
    0,    0,    0, 1004,  242, 1004, 1004,    0,    0, 1004,
    0,    0,    0, 1004, 1670,    0, 1004,    0, 1004,    0,
 1004,  526,    0,    0,  526, 1004, 1004,    0, 1004,    0,
 1000, 1000, 1000, 1000, 1000, 1000, 1000,  526,  242,  526,
    0,  526,    0,    0, 1004, 1004,    0, 1004, 1000, 1000,
 1000, 1000,  704,    0,  524,    0,    0,  705,    0,  526,
    0, 1004,    0, 1004, 1004, 1004,  526,    0, 1004,  366,
    0, 1004, 1004,    0,    0,    0,  524,    0,    0,    0,
 1000, 1004, 1000,  524,    0,    0,  704,    0,    0,    0,
    0,  705,  524,    0,  524, 1004,    0,  704, 1004,    0,
  526,    0,  705, 1373,  708,  706,    0,  707,    0,  709,
  366,    0,  524,  366, 1004,    0,    0,  524,    0,    0,
 1004,  521,    0,    0,    0, 1004,    0,    0,  366,    0,
    0,    0,    0,    0,    0,    0, 1004, 1004,    0, 1004,
  521,  521,    0,  521,    0, 1004,  524,  521,    0,    0,
  521,    0, 1512,  708,  706,    0,  707,    0,  709,    0,
    0,    0,    0,  521,    0,    0,  704,    0,    0,  521,
    0,  705,    0,    0, 1004, 1004,  105, 1004,    0,    0,
    0,    0,    0,    0, 1004, 1004, 1004, 1004, 1004,    0,
 1004,    0,    0, 1004, 1004, 1004, 1004, 1004,    0,    0,
    0, 1004, 1004,    0,    0,  106,  107,    0,  108,    0,
    0,    0,    0,    0,    0,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,    0,    0,    0,  526,    0,
    0,  521,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1000,    0, 1000, 1000,    0,    0,    0,  526,    0,
  526, 1000,    0,  521,  526,    0,    0,  526,    0, 1000,
  521,    0,    0,    0, 1000,    0,    0, 1000,    0,    0,
  526,  521,    0,    0,    0,    0,  526,  672,    0,    0,
    0, 1000,    0,  853,    0,    0, 1000, 1000,    0,  521,
 1000,    0,    0,    0,  521,    0,    0, 1000,    0, 1000,
 1000,    0,    0, 1000,    0,    0,    0, 1000,  231,    0,
 1000,    0, 1000,    0, 1000,    0,    0,  672,  672, 1000,
 1000,  672, 1000,  521,  853,  853,  853,  853,  853,    0,
  853,    0,  704,    0,    0,    0,  672,  705, 1000, 1000,
    0, 1000,  853,  853,  853,  853,    0,    0,  526,  231,
    0,    0,  231,    0,    0, 1000,    0, 1000, 1000, 1000,
    0,    0, 1000,    0,    0, 1000, 1000,  231,    0,    0,
  526,    0,    0,    0,    0, 1000,  853,  526,    0,    0,
  366,  704,    0,    0,    0,    0,  705,  366,  526, 1000,
  366,    0, 1000,    0,  366,    0,    0,    0,    0,    0,
    0,    0,  366,    0,    0,  105,  526,    0, 1000,    0,
    0,  526,    0,    0, 1000,    0,    0,    0,  366, 1000,
    0,    0,    0,    0,    0,    0,  223,    0,    0,    0,
 1000, 1000,    0, 1000,  106,  107,    0,  108,    0, 1000,
  526,    0,  366,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,  223, 1000, 1000,
  223, 1000,  366,    0,    0,    0,  251,    0, 1000, 1000,
 1000, 1000, 1000,    0, 1000,  223,    0, 1000, 1000, 1000,
 1000, 1000,    0,    0,    0, 1000, 1000,    0,    0,  366,
 1513,  708,  706,    0,  707,    0,  709,    0, 1520,  708,
  706,  366,  707,  366,  709,    0,    0,  251,    0,    0,
  251,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  251,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  853,    0,  853,  853,    0,
    0,    0,    0,    0,    0,  853,    0,    0,    0,    0,
    0,    0,    0,  853,    0,    0,    0,    0,  853,    0,
    0,  853,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  853,  242,  865,  231,    0,
  853,  853,    0,    0,  853,  672,    0,    0,  672,    0,
    0,  853,    0,  853,  853,  672,  564,  853,  672,    0,
    0,  853,  672,    0,  853,    0,  853,  672,  853,    0,
  672,    0,    0,  853,  853,    0,  853,    0,  865,  865,
  865,  865,  865,    0,  865,    0,  672,    0,    0,    0,
    0,    0,  853,  853,    0,  853,  865,  865,  865,  865,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  853,
  672,  853,  853,  853,  672,    0,  853,    0,    0,  853,
  853,    0,    0,  672,    0,    0,    0,    0,    0,  853,
  865,    0,    0,    0,    0,    0,  242,    0,    0,    0,
  672,    0,    0,  853,    0,    0,  853,    0,    0,    0,
    0,    0,    0,    0,  223,    0,  223,    0,  242,    0,
    0,    0,  853,  223,    0,  242,    0,  672,  853,    0,
    0,    0,    0,  853,    0,    0,    0,    0,    0,  672,
    0,  672,    0,    0,  853,  853,    0,  853,    0,  704,
    0,    0,    0,  853,  705,    0,    0,  704,    0,  242,
    0,    0,  705,    0,  251,    0,  251,    0,    0,    0,
    0,    0,    0,  251,    0,    0,    0,    0,    0,    0,
    0,    0,  853,  853,    0,  853,    0,    0,    0,  105,
    0,    0,  853,  853,  853,  853,  853,    0,  853,    0,
    0,  853,  853,  853,  853,  853,    0,    0,    0,  853,
  853,    0,    0,  868,  223,    0,    0,    0,  106,  107,
    0,  108,    0,    0,    0,    0,    0,    0,  109,  110,
  111,  112,  113,    0,  114,    0,  223,  115,  116,  117,
  118,  119,    0,  223,    0,  120,  121,    0,    0,    0,
    0,    0,    0,    0,  868,  868,  868,  868,  868,  865,
  868,  865,  865,    0,  251,    0,    0,    0,    0,  865,
    0,    0,  868,  868,  868,  868,    0,  223,    0,    0,
    0,  961,  865,    0,    0,  865,  251,    0,    0,    0,
    0,    0,    0,  251,    0,    0,    0,    0,    0,  865,
    0,    0,    0,    0,  865,  865,  868,    0,  865,    0,
    0,    0,    0,    0,    0,  865,    0,  865,  865,    0,
    0,  865,    0,    0,    0,  865,    0,  251,  865,    0,
  865,    0,  865,    0,    0,    0,    0,  865,  865,    0,
  865,    0,    0,    0,    0, 1487, 1488,    0,    0,    0,
  536, 1492, 1493,    0, 1495, 1496,  865,  865,  674,  865,
    0,    0, 1500, 1501, 1502, 1503,    0, 1504, 1505, 1506,
 1507, 1508, 1509,  865,    0,  865,  865,  865,    0,    0,
  865,    0,    0,  865,  865,    0,    0,    0,    0,    0,
  536,  536,    0,  865,  536,    0,  964,    0,    0,  674,
    0,    0,  674,    0,    0,    0,    0,  865,    0,  536,
  865,  536,    0,  105,    0,    0,    0,  674,    0,    0,
    0,    0,    0,    0,    0,    0,  865,    0,    0,    0,
    0,    0,  865,    0,    0,    0,    0,  865,    0,    0,
    0,  536,  106,  107,    0,  108,    0,    0,  865,  865,
    0,  865,  109,  110,  111,  112,  113,  865,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,   84,    0,    0,  868,    0,  868,  868,   85,
    0,    0,    0,   86,   87,  868,  865,  865,    0,  865,
    0,    0,   88,    0,    0,    0,  865,  865,  865,  865,
  865,  868,  865,    0,   89,  865,  865,  865,  865,  865,
    0,   62,    0,  865,  865,  868,    0,  869,    0,    0,
  868,  868,   63,    0,  868,    0,    0,    0,    0,   64,
    0,  868,    0,  868,  868,    0,    0,  868,    0,    0,
    0,  868,   65,    0,  868,    0,  868,    0,  868,    0,
    0,    0,    0,  868,  868,    0,  868,    0,  869,  869,
  869,  869,  869,    0,  869,    0,    0,    0,    0,   90,
    0,    0,  868,  868,    0,  868,  869,  869,  869,  869,
    0,    0,    0,    0,    0,    0,   91,    0,    0,  868,
    0,  868,  868,  868,    0,    0,  868,    0,    0,  868,
  868,   92,    0,   66,    0,   93,    0,    0,  536,  868,
  869,    0,    0,    0,    0,    0,   94,    0,    0,   67,
    0,    0,    0,  868,  536,    0,  868,  536,  536,    0,
  536,    0,    0,    0,  536,   95,   68,  536,    0,    0,
    0,    0,  868,    0,    0,    0,    0,    0,  868,    0,
  536,   69,    0,  868,    0,   70,  536,    0,   71,  536,
    0,    0,    0,    0,  868,  868,   72,  868,    0,    0,
    0,    0,    0,  868,    0,    0,  674,    0,    0,  674,
    0,    0,    0,    0,    0,   73,  674,    0,    0,  674,
    0,    0,    0,  674,    0,    0,    0,    0,  674,    0,
    0,  674,  868,  868,    0,  868,    0,    0,    0,    0,
    0,    0,  868,  868,  868,  868,  868,  674,  868,    0,
    0,  868,  868,  868,  868,  868,    0,    0,  536,  868,
  868,    0,    0,  863,    0,    0,    0,    0,    0,    0,
    0,  674,    0,    0,    0,  674,    0,    0,    0,    0,
  536,    0,    0,    0,  674,    0,    0,  536,    0,    0,
  570,  536,    0,    0,    0,    0,    0,    0,  536,    0,
    0,  674,    0,    0,  863,    0,  863,  863,  863,  869,
    0,  869,  869,    0,    0,    0,  536,    0,    0,  869,
    0,  536,  863,  863,  863,  863,    0,    0,  674,    0,
    0,    0,    0,    0,    0,  869,    0,    0,    0,    0,
  674,    0,  674,    0,    0,    0,  536,    0,    0,  869,
  536,    0,    0,    0,  869,  869,  863,    0,  869,    0,
    0,    0,    0,    0,    0,  869,    0,  869,  869,    0,
    0,  869,    0,    0,    0,  869,    0,    0,  869,    0,
  869,    0,  869,    0,    0,    0,    0,  869,  869,    0,
  869,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  537,    0,    0,    0,    0,    0,  869,  869,  677,  869,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  869,    0,  869,  869,  869,    0,    0,
  869,    0,    0,  869,  869,    0,    0,    0,    0,    0,
  537,  537,    0,  869,  537,    0,    0,    0,    0,  677,
    0,    0,  677,    0,    0,    0,    0,  869,    0,  537,
  869,  537,    0,  105,    0,    0,    0,  677,    0,    0,
    0,    0,    0,    0,    0,    0,  869,    0,    0,    0,
    0,    0,  869,    0,    0,    0,    0,  869,    0,    0,
    0,  537,  106,  107,    0,  108,    0,    0,  869,  869,
    0,  869,  109,  110,  111,  112,  113,  869,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,  863,    0,  863,  863,    0,
    0,    0,    0,    0,    0,  863,  869,  869,    0,  869,
    0,    0,    0,    0,    0,    0,  869,  869,  869,  869,
  869,  863,  869,    0,    0,  869,  869,  869,  869,  869,
    0,    0,    0,  869,  869,  863,    0,  862,    0,    0,
  863,  863,    0,    0,  863,    0,    0,    0,    0,    0,
    0,  863,    0,  863,  863,    0,    0,  863,    0,    0,
    0,  863,    0,    0,  863,    0,  863,    0,  863,    0,
    0,    0,    0,  863,  863,    0,  863,    0,  862,    0,
  862,  862,  862,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  863,  863,    0,  863,  862,  862,  862,  862,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  863,
    0,  863,  863,  863,    0,    0,  863,    0,    0,  863,
  863,    0,    0,    0,    0,    0,    0,    0,  537,  863,
  862,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  863,  537,    0,  863,  537,  537,    0,
  537,    0,    0,    0,  537,    0,    0,  537,    0,    0,
    0,    0,  863,    0,    0,    0,    0,    0,  863,    0,
  537,    0,    0,  863,    0,    0,  537,    0,    0,  537,
    0,    0,    0,    0,  863,  863,    0,  863,    0,    0,
    0,    0,    0,  863,    0,    0,  677,    0,    0,  677,
    0,    0,    0,    0,    0,    0,  677,    0,    0,  677,
    0,    0,    0,  677,    0,    0,    0,    0,  677,    0,
    0,  677,  863,  863,    0,  863,    0,    0,    0,    0,
    0,    0,  863,  863,  863,  863,  863,  677,  863,    0,
    0,  863,  863,  863,  863,  863,    0,    0,  537,  863,
  863,    0,    0,  864,    0,    0,    0,    0,    0,    0,
    0,  677,    0,    0,    0,  677,    0,    0,    0,    0,
  537,    0,    0,    0,  677,    0,    0,  537,    0,    0,
    0,  537,    0,    0,    0,    0,    0,    0,  537,    0,
    0,  677,    0,    0,  864,    0,  864,  864,  864,  862,
    0,  862,  862,    0,    0,    0,  537,    0,    0,  862,
    0,  537,  864,  864,  864,  864,    0,    0,  677,    0,
    0,    0,    0,    0,    0,  862,    0,    0,    0,    0,
  677,    0,  677,    0,    0,    0,  537,    0,    0,  862,
  537,    0,    0,    0,  862,  862,  864,    0,  862,    0,
    0,    0,    0,    0,    0,  862,    0,  862,  862,    0,
    0,  862,    0,    0,    0,  862,    0,    0,  862,    0,
  862,    0,  862,    0,    0,    0,    0,  862,  862,    0,
  862,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  832,    0,    0,    0,    0,    0,  862,  862,    0,  862,
    0,  777,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  862,    0,  862,  862,  862,    0,    0,
  862,    0,    0,  862,  862,    0,    0,  461,    0,    0,
    0,  832,    0,  862,  832,    0,    0,    0,    0,    0,
    0,    0,  777,    0,    0,  777,    0,  862,    0,  832,
  862,    0,    0,  105,    0,    0,    0,    0,    0,    0,
  777,    0,    0,    0,    0,    0,  862,    0,    0,    0,
    0,    0,  862,    0,    0,    0,    0,  862,    0,    0,
    0,    0,  106,  107,    0,  108,    0,    0,  862,  862,
    0,  862,  109,  110,  111,  112,  113,  862,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,  864,    0,  864,  864,    0,
    0,    0,    0,    0,    0,  864,  862,  862,    0,  862,
    0,    0,    0,    0,    0,    0,  862,  862,  862,  862,
  862,  864,  862,    0,    0,  862,  862,  862,  862,  862,
    0,    0,    0,  862,  862,  864,    0,  867,    0,    0,
  864,  864,    0,    0,  864,    0,    0,    0,    0,    0,
    0,  864,    0,  864,  864,    0,    0,  864,    0,    0,
    0,  864,    0,    0,  864,    0,  864,    0,  864,    0,
    0,    0,    0,  864,  864,    0,  864,    0,  867,    0,
  867,  867,  867,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  864,  864,    0,  864,  867,  867,  867,  867,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  864,
    0,  864,  864,  864,    0,    0,  864,  680,    0,  864,
  864,    0,    0,    0,    0,    0,    0,    0,    0,  864,
  867,    0,  832,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  864,    0,    0,  864,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  680,    0,
    0,  680,  864,    0,    0,    0,    0,    0,  864,    0,
    0,    0,    0,  864,    0,    0,  680,    0,    0,    0,
    0,    0,    0,    0,  864,  864,    0,  864,  832,    0,
    0,  832,    0,  864,    0,    0,    0,    0,  832,  777,
    0,  832,  777,    0,    0,  832,    0,    0,    0,  777,
  832,    0,  777,  832,    0,    0,  777,    0,    0,    0,
    0,  777,  864,  864,  777,  864,    0,    0,    0,  832,
  105,    0,  864,  864,  864,  864,  864,    0,  864,    0,
  777,  864,  864,  864,  864,  864,    0,    0,    0,  864,
  864,    0,    0,  832,    0,    0,  832,  832,    0,  106,
  107,    0,  108,    0,  777,    0,  832,  777,  777,  109,
  110,  111,  112,  113,    0,  114,    0,  777,  115,  116,
  117,  118,  119,  832,    0,    0,  120,  121,    0,    0,
    0,    0,    0,    0,  777,    0,    0,    0,    0,  867,
    0,  867,  867,    0,    0,  832,    0,    0,    0,  867,
  832,    0,    0,    0,    0,    0,  777,    0,    0,    0,
    0,  777,  832,    0,  832,  867,    0,    0,    0,  660,
    0,    0,  421,  777,  422,  777,    0,    0,    0,  867,
    0,    0,    0,    0,  867,  867,    0,  424,  867,    0,
    0,    0,  279,    0,    0,  867,    0,  867,  867,    0,
    0,  867,    0,    0,    0,  867,    0,    0,  867,    0,
  867,    0,  867,    0,    0,    0,    0,  867,  867,    0,
  867,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  867,  867,    0,  867,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  867,    0,  867,  867,  867,    0,    0,
  867,    0,    0,  867,  867,    0,  660,    0,    0,  421,
    0,  422,    0,  867,    0,  680,    0,    0,  680,    0,
    0,    0,    0,    0,  424,  680,    0,  867,  680,  279,
  867,    0,  680,    0,    0,    0,    0,  680,    0,    0,
  680,    0,    0,    0,    0,    0,  867,    0,    0,    0,
    0,    0,  867,    0,    0,    0,  680,  867,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  867,  867,
    0,  867,    0,    0,    0,    0,    0,  867,    0,    0,
  680,    0,    0,    0,  680,    0,    0,    0,    0,    0,
    0,    0,    0,  680,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  867,  867,    0,  867,
  680,    0,    0,    0,    0,    0,  867,  867,  867,  867,
  867,    0,  867,    0,    0,  867,  867,  867,  867,  867,
    0,    0,    0,  867,  867,    0,  385,  680,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,    0,  680,
    0,  680,    0,    0,    0,    0,    0,  663,  387,    0,
    0,    0,    0,    0,  388,  389,    0,    0,    0,    0,
    0,  801,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  654,    0,    0,  663,  663,    0,  663,
    0,    0,  390,    0,    0,  391,  663,  663,  663,  663,
  663,    0,  663,    0,    0,  663,  663,  663,  663,  663,
    0,    0,  801,  663,  663,  801,    0,    0,    0,    0,
    0,    0,    0,  393,  394,    0,    0,    0,    0,    0,
  801,    0,    0,  385,    0,  395,    0,    0,    0,  396,
    0,    0,  386,    0,    0,    0,    0,  884,    0,    0,
    0,    0,    0,    0,    0,  387,    0,    0,  653,    0,
    0,  388,  389,    0,  647,    0,  647,  647,    0,  647,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  654,    0,  647,    0,    0,    0,  697,  647,    0,  390,
  656,    0,  391,    0,    0,    0,  397,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
  393,  394,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,  395,    0,    0,    0,  396,    0,    0,    0,
  404,    0,    0,    0,  655,  465,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,  105,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,  885,  886,  887,  120,  121,  656,    0,    0,
    0,  106,  107,  397,  108,    0,    0,    0,    0,  398,
    0,  109,  110,  111,  112,  113,    0,  114,    0,  399,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,    0,  801,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,    0,    0,  115,  116,  657,  658,  659,    0,
    0,    0,  120,  121,  647,    0,    0,    0,    0,  801,
    0,  647,  801,    0,    0,    0,    0,    0,    0,  801,
  647,    0,  801,    0,    0,    0,  801,    0,    0,    0,
    0,  801,    0,  647,  801,    0,    0,    0,    0,  647,
  647,    0,    0,  641,    0,  641,  641,  647,  641,    0,
  801,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  641,    0,    0,    0,    0,  641,  647,    0,    0,
  647,    0,    0,    0,  801,    0,    0,  801,  801,    0,
    0,    0,    0,  802,    0,    0,    0,  801,    0,    0,
    0,    0,    0,    0,    0,    0,  647,    0,  647,  647,
    0,    0,    0,    0,  801,    0,    0,    0,    0,    0,
  647,    0,    0,    0,  647,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  802,    0,  801,  802,    0,  660,
    1,  801,  421,    0,  422,    0,    0,    0,    0,    0,
    0,    0,  802,  801,    0,  801,    0,  424,    0,    0,
    0,    2,  279,    0,    0,    0,    0,    0,    0,    3,
    0,    0,    0,    0,    0,    0,    4,    0,    5,    0,
    6,  647,    0,    0, 1650,    7,    0,  647,    0,    0,
    0,    0,    8,    0,    0,    0,    0,  647,    0,    0,
    0,    0,    0,    0,    0,    0,    9,    0,    0, 1651,
    0,  647,  647,  647,  647,  647,  647,   10,    0,    0,
    0,    0,    0,    0,    0,  647,    0,    0,    0,    0,
    0,    0,    0,    0,  647,  647,  647,  647,  647,  647,
  647,  647,  647,  647,  647,    0,  647,    0,    0,  647,
  647,    0,  647,  647,  647,  647,  647,  647,  647,  647,
    0,    0,  647,  647,  647,  647,  647, 1652,    0,    0,
  647,  647,    0,  641,    0,    0,    0,    0,    0,    0,
  641,    0,    0,    0,   11,    0,    0,    0,    0,  641,
    0,   12,    0,    0,   13,    0,   14,   15,    0,    0,
    0,  105,  641,    0,    0,    0,    0,    0,  641,  641,
    0,    0,    0,    0,    0,    0,  641,    0,    0,    0,
    0,   16,    0,    0,    0,    0,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,  641,    0,    0,  641,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,  802,    0,  120,  121,    0,
    0,    0,    0,    0,    0,  641,  385,  641,  641,  660,
   17,    0,  421,    0,  422,  386,    0,    0,    0,  641,
    0,    0,    0,  641,    0,   18,   19,  424,  387,    0,
    0,    0,  279,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  802,    0,  654,  802,    0,    0,    0,    0,    0,
    0,  802,  390,    0,  802,  391,    0,    0,  802,    0,
    0,    0,    0,  802,    0,    0,  802,    0,    0,    0,
  641,    0,    0,    0,    0,    0,  641,    0,    0,    0,
    0,    0,  802,  393,  394,    0,  641,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,  396,
  641,  641,  641,  641,  641,  641,  802,  880,    0,  802,
  802,    0,    0,    0,  641,    0,    0,    0,    0,  802,
    0,    0,    0,  641,  641,  641,  641,  641,  641,  641,
  641,  641,  641,  641,    0,  641,  802,    0,  641,  641,
    0,  641,  641,  641,  641,  641,  641,  641,  641,    0,
  656,  641,  641,  641,  641,  641,  397,    0,  802,  641,
  641,    0,  398,  802,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,  802,    0,  802,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,  385,  417,  418,  419,
  111,  112,  113,  420,  114,  386,    0,  115,  116,  657,
  658,  659,    0,    0,    0,  120,  121,  660,  387,    0,
  421,    0,  422,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  424,    0,    0,    0,    0,
  279,    0,    0,  654,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,  391,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  942,  708,
  706,    0,  707,    0,  709,    0,    0,    0,    0,    0,
    0,    0,    0,  393,  394,    0,    0,  905,  904,  906,
    0,    0,    0,    0,    0,  395,    0,    0,    0,  396,
    0,    0,  870,    0,    0,    0,  598,  655,    0,    0,
    0,    0,    0,    0,  485,    0,    0,  421,    0,  422,
  871,    0,    0,    0,  599,    0,    0,  600,    0,    0,
    0,    0,  424,    0,    0,    0,    0,  279,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  656,    0,    0,    0,    0,    0,  397,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,    0,  601,    0,
    0,    0,  399,    0,  602,  603,  604,    0,  605,  606,
  607,    0,  608,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,  609,    0,  610,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,  611,    0,  115,  116,  657,
  658,  659,    0,    0,  385,  120,  121,    0,    0,    0,
    0,    0,  612,  386,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  387,    0,    0,    0,
    0,    0,  388,  389,  613,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  654,    0,    0,    0,    0,    0,    0,    0,  892,
  390,    0,    0,  391,    0,    0,    0,  704,    0,  614,
  615,    0,  705,    0,    0,  893,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  616,    0,    0,    0,
    0,  393,  394,    0, 1122,  531,    0, 1123,    0,    0,
    0,  385,    0,  395,    0,    0,    0,  396,    0,    0,
  386,  894,    0,    0,    0,  880,    0,    0,    0,    0,
  895,    0,    0,  387,    0,    0,    0,    0,  896,  388,
  389,    0,  485,    0,    0,  421,  531,  422,    0,  531,
    0,    0,    0,    0,    0,    0,    0,  897,    0,  898,
  424,    0,    0,    0,  531,  279,  531,  390,  656,    0,
  391,    0,    0,  899,  397,  900,  901,  902,    0,    0,
  398,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,    0,    0,    0,    0,    0,  531,    0,  393,  394,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
  395,    0,    0,    0,  396,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,  903,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0, 1124,
    0,  397,    0,    0,    0,    0,    0,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
    0,    0,    0,    0,  485,    0,    0,  421,    0,  422,
    0,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,    0,  424,    0,  961,  404,    0,  279,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121, 1132,  531,    0, 1123,    0,    0,    0,  385,
    0,    0,    0,    0,    0,    0,    0,    0,  386,  531,
    0,    0,  531,  531,    0,  531,    0,    0,  962,  531,
    0,  387,  531,    0,    0,    0,    0,  388,  389,    0,
    0,    0,    0,  485,    0,  531,  421,    0,  422,    0,
  569,  531,    0,    0,  531,    0,    0,    0,    0,    0,
    0,  424,    0,    0,  963,  390,  279,    0,  391,  964,
    0,    0,    0,    0,    0,  965,    0,    0,    0,  569,
  569,    0,  569,    0,    0,    0,  105,    0,    0,  569,
  569,  569,  569,  569,  966,  569,  393,  394,  569,  569,
  569,  569,  569,    0,    0,    0,  569,  569,  395,    0,
    0,    0,  396,    0,    0,  106,  107,    0,  108,    0,
    0,    0,    0,  531,    0,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,  531,    0,    0,    0,    0,
    0,    0,  531,    0, 1136,    0,  531, 1123,    0,    0,
    0,  385,    0,  531,    0,    0,    0, 1124,    0,  397,
  386,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,  531,    0,  387,    0,  399,  531,    0,    0,  388,
  389,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,  531,    0,  404,    0,  531,    0,  390,    0,    0,
  391,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  393,  394,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
  395,    0,    0, 1144,  396,    0, 1123,    0,    0,    0,
  385,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  387,    0,    0,    0,    0,    0,  388,  389,
    0,    0,  485,    0,    0,  421,    0,  422,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1124,
  424,  397,    0,    0,    0,  279,  390,  398,    0,  391,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  400,  401,  402,  403,  204,  205,  393,  394,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,  395,
    0,    0,    0,  396,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  117,  118,  119,    0, 1433,    0,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1124,    0,
  397,    0,    0,    0,    0,    0,  398,    0,    6,    0,
    0,    0,    0,    0,    0,    0,  399,    0,    0, 1546,
 1547,    0, 1548,    0,  485,    0,    0,  421,    0,  422,
  400,  401,  402,  403,  204,  205,    0,    0,    0, 1549,
    0,    0,  424,    0,  404, 1550,    0,  279,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0, 1147,    0,    0, 1123,    0,    0,    0,  385,
    0,    0,    0,    0,    0, 1551,    0,    0,  386,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  387,    0,    0,   14,    0,    0,  388,  389,    0,
    0,    0,  281,  485,    0,    0,  421,    0,  422, 1552,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   16,
    0,  424,    0,    0,    0,  390,  279,    0,  391,    0,
    0,  281,  281,    0,  281,    0,    0,  105,    0,    0,
    0,  281,  281,  281,  281,  281,    0,  281,    0,    0,
  281,  281,  281,  281,  281,    0,  393,  394,  281,  281,
    0,    0,    0,    0,    0,    0, 1553,  107,  395,  108,
    0,    0,  396,    0,    0,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115, 1554,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1152,    0,    0, 1123,    0,    0,
    0,  385,    0,    0,    0,    0,    0, 1124,    0,  397,
  386,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,    0,    0,  387,    0,  399,    0,    0,    0,  388,
  389,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,  390,    0,    0,
  391,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  393,  394,
  115,  116,  117,  118,  119,    0,  826,    0,  120,  121,
  395,    0,    0, 1155,  396,    0, 1123,    0,    0,    0,
  385,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  387,    0,    0,    0,    0,  826,  388,  389,
  826,    0,  485,    0,    0,  421,    0,  422,    0,    0,
    0,    0,    0,    0,    0,  826,    0,    0,    0, 1124,
  424,  397,    0,    0,    0,  279,  390,  398,    0,  391,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  400,  401,  402,  403,  204,  205,  393,  394,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,  395,
    0,    0,    0,  396,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1124,    0,
  397,    0,    0,    0,    0,    0,  398,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
    0,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,  826,  120,
  121,    0, 1158,    0,    0, 1123,    0,    0,    0,  385,
    0,    0,  485,    0,  490,  421,    0,  422,  386,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  424,  387,    0,    0,    0,  279,    0,  388,  389,    0,
  708,  706,    0,  707,    0,  709,    0,    0,    0,    0,
    0,    0,    0,    0,  826,    0,    0,  826,  905,  904,
  906,    0,    0,    0,  826,  390,    0,  826,  391,    0,
    0,  826,    0,    0,    0,    0,  826,    0,    0,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  826,  393,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
  485,    0,  396,  421,    0,  422,    0,    0,    0,  826,
    0,    0,  826,  826,    0,    0,    0,    0,  424,    0,
    0,    0,  826,  279,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1124,    0,  397,
    0,  826,    0,    0,    0,  398,  826,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,  826,    0,
  826,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,  385,
  417,  418,  419,  111,  112,  113,  420,  114,  386,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,  387,    0,    0,    0,    0,    0,  388,  389,    0,
  892,  805,    0,    0,    0,    0,    0,    0,  704,    0,
    0,    0,    0,  705,    0,    0,  893,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  805,    0,    0,  805,    0,    0,    0,    0,
    0,    0,  894,    0,  392,    0,  393,  394,    0,    0,
  805,  895,    0,    0,    0,    0,    0,  385,  395,  896,
    0,    0,  396,    0,    0,    0,  386,    0,    0,  969,
    0,  969,  969,    0,  969,    0,    0,    0,  897,  387,
  898,    0,    0,    0,    0,  388,  389,  969,    0,    0,
    0,    0,  969,    0,  899,    0,  900,  901,  902,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  390,    0,    0,  391,    0,    0,  397,
    0,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,  392,    0,  393,  394,    0,  903,    0,  400,
  401,  402,  403,  204,  205,    0,  395,    0,    0,    0,
  396,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,  697,  120,  121,
    0,    0,    0,    0,    0,    0,    0,  397,    0,    0,
    0,  647,    0,  398,  647,    0,  647,    0,    0,    0,
    0,    0,    0,  399,    0,    0,    0,    0,    0,  647,
    0,    0,    0,    0,  647,    0,    0,  400,  401,  402,
  403,  204,  205,  805,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0,    0,    0,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,    0,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,  969,    0,  115,  116,
  117,  118,  119,    0,    0,  969,  120,  121,    0,  805,
    0,    0,  805,    0,    0,    0,  803,    0,  969,  805,
    0,    0,  805,    0,  969,  969,  805,    0,    0,    0,
    0,  805,    0,    0,  805,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  805,    0,  969,    0,    0,  969,    0,  803,    0,    0,
  803,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  805,  803,    0,  805,  805,    0,
    0,  969,    0,  969,  969,    0,    0,  805,    0,    0,
    0,    0,    0,    0,    0,  969,  804,    0,    0,  969,
    0,    0,    0,    0,  805,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  805,    0,    0,    0,
    0,  805,    0,    0,    0,    0,    0,  804,    0,    0,
  804,  647,    0,  805,    0,  805,    0,    0,  647,    0,
    0,    0,    0,    0,    0,  804,  969,  647,    0,    0,
  423,    0,  969,  421,    0,  422,    0,    0,    0,    0,
  647,    0,  969,    0,    0,    0,  647,  647,  424,    0,
    0,    0,    0,  279,  647,    0,  969,  969,  969,  969,
  969,  969,    0,    0,    0,    0,    0,    0,    0,    0,
  969,    0,    0,    0,  647,    0,    0,  647,    0,  969,
  969,  969,  969,  969,  969,  969,  969,  969,  969,  969,
    0,  969,    0,    0,  969,  969,    0,  969,  969,  969,
  969,  969,  969,  969,  969,  647,  647,  969,  969,  969,
  969,  969,    0,    0,    0,  969,  969,  647,    0,    0,
    0,  647,    0,    0,    0,    0,    0,  485,    0,    0,
  421,    0,  422,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  424,    0,    0,  803,    0,
  279,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  647,    0,
    0,    0,    0,    0,  647,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  647,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  803,    0,    0,  803,  647,  647,
  647,  647,  647,  647,  803,    0,    0,  803,    0,    0,
    0,  803,  647,    0,    0,    0,  803,    0,  804,  803,
    0,  647,  647,  647,  647,  647,  647,  647,  647,  647,
  647,  647,    0,  647,    0,  803,  647,  647,    0,  647,
  647,  647,  647,  647,  647,  647,  647,  385,    0,  647,
  647,  647,  647,  647,    0,    0,  386,  647,  647,  803,
    0,    0,  803,  803,    0,    0,    0,    0,    0,  387,
    0,    0,  803,   98,  804,  388,  389,  804,    0,    0,
    0,    0,    0,    0,  804,    0,    0,  804,    0,  803,
    0,  804,    0,    0,    0,    0,  804,    0,    0,  804,
    0,    0,    0,  390,    0,    0,  391,    0,    0,    0,
    0,  803,    0,    0,   99,  804,  803,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  100,    0,  803,    0,
  803,    0,  392,    0,  393,  394,    0,    0,    0,  804,
  134,    0,  804,  804,  385,    0,  395,    0,    0,    0,
  396,  101,  804,  386,    0,    0,  485,    0,    0,  421,
    0,  422,    0,    0,    0,    0,  387,    0,    0,  804,
    0,    0,  388,  389,  424,    0,    0,    0,    0,  279,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  804,    0,    0,    0,    0,  804,    0,    0,    0,
  390,    0,    0,  391,    0,    0,    0,  397,  804,    0,
  804,    0,    0,  398,    0,    0,    0,    0,    0,    0,
  102,    0,    0,  399,    0,    0,    0,    0,    0,    0,
  103,  393,  394,    0,    0,    0,    0,  400,  401,  402,
  403,  204,  205,  395,    0,    0,    0,  396,    0,    0,
    0,  404,    0,    0,    0,  104,    0,    0,    0,    0,
  405,  406,  407,  408,  409,  410,  411,  412,  413,  106,
  414,    0,  108,  105,    0,  415,  416,    0,  417,  418,
  419,  111,  112,  113,  420,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,  106,  107,  397,  108,    0,    0,    0,    0,
  398,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  399,  115,  116,  117,  118,  119,    0,    0,  689,  120,
  121,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  385,    0,  115,  116,  117,  118,  119,
    0,    0,  386,  120,  121,  485,    0,    0,  421,    0,
  422,    0,    0,    0,    0,  387,    0,    0,    0,    0,
    0,  388,  389,  424,    0,    0,    0,    0,  279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  393,  394,    0,    0,    0,  485,  945,    0,  421,    0,
  422,    0,  395,    0,    0,    0,  396,    0,    0,    0,
    0,    0,    0,  424,    0,    0,  105,    0,  279,    0,
    0,    0,    0,    0,    0,  164,  165,  166,  167,  168,
  169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
  179,    0,    0,  180,  181,  106,  107,    0,  108,    0,
    0,  182,  183,  697,    0,  109,  110,  111,  112,  113,
  184,  114,  185,  397,  115,  116,  117,  118,  119,  398,
    0,    0,  120,  121,    0,    0,    0,    0,    0,  399,
    0,    0,    0,  221,    0,    0,    0,  221,    0,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,  221,    0,    0,    0,  221,    0,  404,  221,    0,
    0,    0,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,  385,    0,  115,  116,  117,  118,  119,  221,
    0,  386,  120,  121,    0,  221,  221,  221,    0,  221,
  221,  221,    0,  221,  387,    0,    0,    0,    0,    0,
  388,  389,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  221,    0,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,  391,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  221,    0,    0,    0,
    0,   80,  385,    0,    0,    0,    0,  392,    0,  393,
  394,  386,    0,  221,  485,    0,    0,  421,    0,  422,
    0,  395,    0,    0,  387,  396,    0,    0,    0,    0,
  388,  389,  424,    0,    0,  221,    0,  279,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,  391,    0,    0,    0,    0,    0,    0,    0,    0,
  221,  221,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  397,    0,    0,    0,    0,  221,  398,  393,
  394,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,  395,    0,    0,    0,  396,    0,    0,    0,    0,
    0,    0,  400,  401,  402,  403,  204,  205,    0,    0,
   98,    0,    0,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,   99,  397,  115,  116,  117,  118,  119,  398,    0,
    0,  120,  121,  100,    0,    0,    0,    0,  399,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  400,  401,  402,  403,  204,  205,  101,    0,
    0,    0,    0,    0,    0,    0,  404,    0,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,  385,    0,  115,  116,  117,  118,  119,    0,    0,
  386,  120,  121,  485,    0,    0,  421,    0,  422,    0,
    0,    0,    0,  387,    0,    0,    0,  102,    0,  388,
  389,  424,    0,    0,    0,    0,  279,  103,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  390,    0,    0,
  391,    0,  104,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,    0,    0,    0,  393,  394,
    0,    0,    0,  485,    0,    0,  421,    0,  422,    0,
  395,    0,    0,    0,  396,    0,    0,    0,    0,  106,
  107,  424,  108,    0,    0,    0,  279,    0,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  397,    0,    0,    0,    0,    0,  398,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
    0,  221,    0,    0,    0,  221,    0,    0, 1150,    0,
    0,  400,  401,  402,  403,  204,  205,    0,    0,  221,
    0,    0,    0,  221,    0,  404,  221,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
  385,    0,  115,  116,  117,  118,  119,  221,    0,  386,
  120,  121,    0,  221,  221,  221,    0,  221,  221,  221,
    0,  221,  387,    0,    0,    0,    0,    0,  388,  389,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  221,    0,  221,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  221,    0,    0,    0,    0,    0,
  385,    0,    0,    0,    0,    0,    0,  393,  394,  386,
    0,  221,  485,    0,    0,  421,    0,  422,    0,  395,
    0,    0,  387,  396,    0,    0,    0,    0,  388,  389,
  424, 1720,    0,  221,    0,  279,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0,    0,    0,  221,  221,
 1335,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  397,    0,    0,    0,    0,  221,  398,  393,  394,    0,
    0,    0,    0,    0,    0,    0,  399,    0,    0,  395,
    0,    0,    0,  396,    0,    0,    0,    0,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
    0,    0,    0,    0,  404,    0,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414, 1433,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
  397,  115,  116,  117,  118,  119,  398,    0,    0,  120,
  121,    0,    0,    0,    6,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0, 1546, 1547, 1348, 1548,    0,
  400,  401,  402,  403,  204,  205,    0,    0,    0,    0,
    0,    0,    0,    0,  404, 1549,    0,    0,    0,    0,
    0, 1550,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,  385,
    0,  115,  116,  117,  118,  119,    0,    0,  386,  120,
  121,  485,    0,    0,  421,    0,  422,    0,    0,    0,
    0,  387,    0,    0,    0,    0,    0,  388,  389,  424,
    0, 1551,    0,    0,  279,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0, 1552,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0, 1678,    0,  393,  394,    0,    0,
    0,  969,    0,  105,  969,    0,  969,    0,  395,    0,
    0,    0,  396,    0,    0,    0,  597,    0,    0,  969,
  598,    0,    0,    0,  969,    0,    0,    0,    0,    0,
    0,    0, 1553,  107,    0,  108,    0,    0,  599,    0,
    0,  600,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115, 1554,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0,    0,    0,    0,    0,  397,
    0,    0,    0,    0,    0,  398,    0,    0,    0,    0,
    0,    0,  601,    0,    0,  399,    0,    0,  602,  603,
  604,    0,  605,  606,  607,    0,  608,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,  609,    0,  610,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  385,  611,
  115,  116,  117,  118,  119,    0,    0,  386,  120,  121,
    0,    0,    0,    0,    0,    0,  612,    0,    0,    0,
  387,    0,    0,    0,    0,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  613,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  390,    0,    0,  391,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  553,    0,
    0,    0,    0,  614,  615,    0,    0,    0,  969,    0,
  543,    0,    0,    0,    0,  393,  394,  969,    0,    0,
  616,    0,  528,    0,    0,    0,    0,  395,    0,    0,
  969,  396,    0,    0,    0,    0,  969,  969,    0,  553,
    0,    0,  553,    0,    0,    0,    0,    0,    0,  834,
    0,  543,    0,    0,  543,    0,    0,  553,    0,  553,
    0,    0,    0,  528,  969,    0,  528,  969,    0,  543,
    0,  543,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  528,    0,  528,    0,    0,    0,    0,  397,  553,
  834,    0,    0,  834,  398,  969,  969,    0,    0,    0,
    0,  543,    0,    0,  399,    0,    0,  969,  834,    0,
    0,  969,    0,  528,    0,    0,    0,    0,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,    0,  108,    0,    0,  415,  416,    0,  417,
  418,  419,  111,  112,  113,  420,  114,    0,  969,  115,
  116,  117,  118,  119,  969,    0,    0,  120,  121,    0,
    0,    0,    0,    0,  969,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  969,  969,
  969,  969,  969,  969,    0,    0,    0,    0,    0,    0,
    0,    0,  969,    0,    0,    0,    0,    0,    0,    0,
    0,  969,  969,  969,  969,  969,  969,  969,  969,  969,
  969,  969,    0,  969,    0,    0,  969,  969,    0,  969,
  969,  969,  969,  969,  969,  969,  969,    0,    0,  969,
  969,  969,  969,  969,    0,    0,  553,  969,  969,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  543,    0,
    0,    0,  553,    0,    0,    0,  553,    0,  553,    0,
  528,    0,  553,    0,  543,  553,    0,    0,  543,    0,
  543,  842,    0,    0,  543,    0,  528,  543,  553,    0,
  528,    0,  528,    0,  553,    0,  528,  553,    0,  528,
  543,  834,    0,    0,    0,    0,  543,    0,    0,  543,
    0,    0,  528,    0,    0,    0,    0,    0,  528,    0,
    0,  528,  842, 1008, 1008,  842, 1008, 1008, 1008,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  842, 1008, 1008, 1008,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  834,    0,    0,
  834,    0,    0,    0,    0,    0,  553,  834,    0,    0,
  834,    0, 1008,    0,  834,    0,    0,    0,  543,  834,
    0,    0,  834,    0,    0,    0,    0,    0,  553,    0,
  528,    0,    0,    0,    0,  553,    0,    0,  834,  553,
  543,    0,    0,    0,    0,    0,  553,  543,    0,    0,
    0,  543,  528,    0,    0,    0,    0,    0,  543,  528,
    0,    0,  834,  528,  553,  834,  834,    0,    0,  553,
  528,    0,    0,    0,    0,  834,  543,    0,    0,    0,
    0,  543,  843,    0,    0,    0,  133,    0,  528,    0,
  133,    0,  834,  528,  553,    0,    0,    0,  553,    0,
    0,    0,    0,    0,    0,    0,  543,    0,  133,    0,
  543,  133,    0,    0,  834,    0,    0,    0,  528,  834,
    0,    0,  528,  843, 1009, 1009,  843, 1009, 1009, 1009,
    0,  834,    0,  834,    0,    0,    0,    0,    0,    0,
    0,  843, 1009, 1009, 1009,    0,    0,    0,    0,    0,
    0,    0,  133,    0,    0,    0,    0,    0,  133,  133,
  133,    0,  133,  133,  133,    0,  133,    0,    0,    0,
    0,    0,    0, 1009,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  842,    0,    0,  133,    0,  133,    0,
    0,    0,    0, 1008,    0,    0,    0,    0,    0,    0,
    0, 1008,    0,    0,    0,    0, 1008,    0,    0, 1008,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  133,
    0,    0,    0,    0,    0,  844,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  133,    0,    0,  842,
    0,    0,  842,    0,    0, 1008,    0,    0,    0,  842,
    0,    0,  842,    0, 1008,    0,  842,    0,  133,    0,
    0,  842, 1008,    0,  842,    0,  844, 1010, 1010,  844,
 1010, 1010, 1010,    0,    0,    0,    0,    0,    0,    0,
  842, 1008,    0, 1008,  844, 1010, 1010, 1010,  798,    0,
    0,    0,    0,  133,  133,    0,    0, 1008,    0, 1008,
 1008, 1008,    0,    0,  842,    0,    0,  842,  842,    0,
  133,    0,    0,    0,    0,    0, 1010,  842,    0,    0,
    0,    0,    0,    0,  799,    0,    0,    0,    0,  798,
    0,    0,  798,    0,  842,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  843,    0,    0,  798,    0,    0,
 1008,    0,    0,    0, 1009,    0,  842,    0,    0,    0,
  800,  842, 1009,    0,    0,  799,    0, 1009,  799,    0,
 1009,    0,    0,  842,    0,  842,    0,    0,    0,    0,
    0,    0,    0,  799,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  833,    0,    0,    0,
  843,  800,    0,  843,  800,    0, 1009,    0,    0,    0,
  843,    0,    0,  843,    0, 1009,    0,  843,    0,  800,
    0,    0,  843, 1009,    0,  843,    0,    0,    0,    0,
    0,    0,  827,    0,    0,    0,    0,  833,    0,    0,
  833,  843, 1009,    0, 1009,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  833,    0,    0, 1009,    0,
 1009, 1009, 1009,    0,    0,  843,    0,    0,  843,  843,
    0,    0,    0,  827,    0,    0,  827,    0,  843,    0,
    0,    0,    0,    0,    0,    0,    0,  844,    0,    0,
    0,  827,    0,    0,    0,  843,    0, 1010,    0,    0,
    0,    0,    0,    0,  835, 1010,    0,    0,    0,    0,
 1010, 1009,    0, 1010,    0,    0,    0,  843,    0,    0,
    0,    0,  843,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  843,    0,  843,    0,    0,    0,
    0,    0,    0,  844,    0,  835,  844,    0,  835, 1010,
  798,    0,    0,  844,    0,    0,  844,    0, 1010,    0,
  844,    0,    0,  835,    0,  844, 1010,    0,  844,    0,
    0,    0,    0,    0,    0,    0,  836,    0,    0,    0,
    0,    0,    0,    0,  844, 1010,  799, 1010,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1010,    0, 1010, 1010, 1010,  798,    0,  844,  798,
    0,  844,  844,    0,    0,    0,  798,  836,    0,  798,
  836,  844,  800,  798,    0,    0,    0,    0,  798,    0,
    0,  798,    0,    0,    0,  836,    0,    0,  844,    0,
    0,    0,  799,    0,    0,  799,    0,  798,    0,    0,
    0,    0,  799,    0, 1010,  799,    0,    0,  833,  799,
  844,    0,    0,    0,  799,  844,    0,  799,    0,    0,
    0,  798,    0,    0,  798,  798,    0,  844,  800,  844,
    0,  800,    0,  799,  798,    0,    0,    0,  800,  824,
    0,  800,    0,    0,  827,  800,    0,    0,    0,    0,
  800,  798,    0,  800,    0,    0,    0,  799,    0,    0,
  799,  799,    0,    0,  833,    0,    0,  833,    0,  800,
  799,  828,    0,  798,  833,    0,    0,  833,  798,    0,
  824,  833,    0,  824,    0,    0,  833,  799,    0,  833,
  798,    0,  798,  800,    0,    0,  800,  800,  824,    0,
  827,    0,    0,  827,    0,  833,  800,    0,    0,  799,
  827,    0,  828,  827,  799,  828,  835,  827,    0,    0,
    0,    0,  827,  800,    0,  827,  799,    0,  799,  833,
  828,    0,  833,  833,    0,    0,    0,    0,    0,    0,
    0,  827,  833,    0,    0,  800,    0,    0,    0,    0,
  800,    0,    0,    0,    0,  837,    0,    0,    0,  833,
    0,    0,  800,    0,  800,  827,    0,    0,  827,  827,
    0,    0,  835,    0,    0,  835,    0,    0,  827,    0,
    0,  833,  835,    0,  825,  835,  833,    0,  836,  835,
    0,    0,    0,    0,  835,  827,  837,  835,  833,  837,
  833,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  835,  837,    0,    0,  827,  829,    0,
    0,    0,  827,    0,    0,  825,    0,    0,  825,    0,
    0,    0,    0,    0,  827,    0,  827,  835,    0,    0,
  835,  835,    0,  825,  836,    0,    0,  836,    0,    0,
  835,    0,    0,    0,  836,    0,    0,  836,    0,  829,
    0,  836,  829,    0,    0,    0,  836,  835,    0,  836,
    0,    0,    0,    0,    0,    0,    0,  829,    0,    0,
    0,    0,    0,    0,    0,  836,    0,    0,    0,  835,
    0,    0,    0,    0,  835,    0,    0,    0,    0,    0,
    0,  824,    0,    0,    0,    0,  835,    0,  835,  836,
    0,    0,  836,  836,    0,    0,    0,    0,    0,    0,
    0,    0,  836,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  828,    0,    0,    0,    0,    0,  836,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  824,    0,    0,
  824,  836,    0,    0,    0,    0,  836,  824,    0,    0,
  824,    0,    0,    0,  824,    0,    0,    0,  836,  824,
  836,    0,  824,    0,    0,    0,    0,    0,    0,  828,
    0,    0,  828,    0,    0,    0,    0,    0,  824,  828,
    0,    0,  828,    0,    0,    0,  828,    0,    0,    0,
    0,  828,    0,    0,  828,    0,    0,  837,    0,    0,
    0,    0,  824,    0,    0,  824,  824,    0,    0,    0,
  828,    0,    0,    0,    0,  824,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  825,    0,    0,    0,
    0,    0,  824,    0,  828,    0,    0,  828,  828,    0,
    0,    0,    0,    0,    0,    0,    0,  828,    0,    0,
    0,    0,    0,  837,  824,    0,  837,    0,    0,  824,
  829,    0,    0,  837,  828,    0,  837,    0,    0,    0,
  837,  824,    0,  824,    0,  837,    0,    0,  837,    0,
    0,    0,  825,    0,    0,  825,  828,    0,    0,    0,
    0,  828,  825,    0,  837,  825,    0,    0,  321,  825,
    0,    0,    0,  828,  825,  828,    0,  825,    0,    0,
    0,    0,    0,    0,    0,    0,  829,    0,  837,  829,
    0,  837,  837,  825,    0,    0,  829,    0,  321,  829,
    0,  837,    0,  829,    0,  321,    0,    0,  829,  321,
  321,  829,  321,    0,    0,  321,    0,  825,  837,    0,
  825,  825,    0,    0,    0,    0,    0,  829,    0,  321,
  825,    0,    0,    0,    0,  321,    0,    0,    0,    0,
  837,    0,    0,    0,    0,  837,    0,  825,    0,    0,
    0,  829,    0,    0,  829,  829,    0,  837,    0,  837,
    0,    0,    0,    0,  829,    0,    0,    0,    0,  825,
    0,    0,    0,    0,  825,    0,    0,    0,    0,    0,
    0,  829,    0,    0,    0,    0,  825,    0,  825,    0,
  597,    0,    0,    0,  598,  321,    0,    0,    0,    0,
    0,  797,    0,  829,    0,    0,    0,    0,  829,    0,
    0,    0,  599,    0,  321,  600,    0,    0,    0,    0,
  829,    0,  829,    0,    0,    0,    0,    0,    0,  321,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  321,
    0,    0,    0,    0,    0,    0,    0,    0,  321,    0,
  321,    0,    0,    0,    0,    0,  601,  321,    0,    0,
    0,    0,  602,  603,  604,    0,  605,  606,  607,    0,
  608,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  321,  321,    0,  321,
  609,    0,  610,    0,    0,    0,  321,  321,  321,  321,
  321,    0,  321,    0,    0,  321,  321,  321,  321,  321,
    0,    0,    0,  321,  321,    0,    0,    0,    0,    0,
    0,    0,    0,  611,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  612,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  613,    0,  597,    0,    0,    0,  598,    0,
    0,    0,    0,    0,    0,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,    0,  599,    0,    0,  600,
    0,    0,    0,    0,    0,    0,    0,  614,  615,    0,
    0,    0,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,    0,    0,  616,    0,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
  601,    0,    0,  120,  121,    0,  602,  603,  604,    0,
  605,  606,  607,    0,  608,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  452,
    0,    0,    0,    0,  609,    0,  610,    0,    0,  452,
  452,  452,  452,  452,  452,  452,  452,  452,  452,  452,
  452,  452,  452,  452,    0,    0,  452,  452,  452,  452,
    0,  452,    0,    0,  452,  452,    0,  611,  452,  452,
  452,  452,  452,  452,  452,  452,    0,  452,  452,  452,
  452,  452,    0,    0,  612,  452,  452,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  613,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  105,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  614,  615,    0,    0,    0,    0,    0,    0,    0,
  106,  107,    0,  108,    0,    0,    0,    0,  616,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,
};
static short yycheck[] = {                                       9,
   16,   55,   12,    0,    4,  151,   16,   64,   18,  256,
  104,  287,  140,  141,   63,   56,  140,  141,  515,   70,
  355,   63,  712,  363,  334,   62,   67,  363,   85,   57,
  556,  359,   94,  559,  423,   89,  772,  565,  498,  565,
  332,   69,   93,   53,   54,   55,   56,   57,   58,   90,
   73,  522, 1045,   63,   64,   65,   97,   67, 1266,   69,
   70,  941,   72,   73,  677,  689,  649,   83,  758,  697,
   88,   81,   95,   83,   84,   85,   92,   87,   88,   89,
   90,  473,   92,   93,   94,   95,  264,   97,    0,  131,
   68,  133,  154, 1038, 1038,  273, 1038,  730,  726,  979,
 1045, 1045,   91, 1045,  132,  642,  281,  368,  864, 1038,
    0,  152,    0,   91,  327,    0, 1045,   44,   44, 1038,
    0,  131,  132,  133,  308,  153, 1045,  145,  765,    0,
    0,    0,  380, 1071,  228,  145,    0,    0,  833,    0,
   45,  151,  152,  153,  154,   40, 1094,   59,  158,  762,
   59,   41,  189,   41,   44,  711,   41,  794,   40,   44,
    0,   41,  308,  158,   44,   41,  803,   40,   63,   59,
   44,   59,    0,    0,   59,  989,  423,  214,   41,   59,
  730,   63, 1046,   41,   63,    0, 1473,    0,   59,   59,
   59,   45,   44, 1057,  271,   59,   59,   45,   59,   41,
    0,   41, 1208,   40,  532,   41,  768,  542,  543,  544,
  545,    0,    0, 1077,   41, 1095,  258,   42,  286,   59,
  857,  315,   44,   44,   44,  565,   41,  655,   41,  565,
    0,   59,   59,    0,  262,   91,  357,    0,  292, 1314,
 1224,   41,  739,  271,   59,   44,   59,   45,  258,    0,
    0,    0,  262,  264,  264,  265,  517,  277,   41,   59,
  355,  271, 1089,  273,  592,   63,  308,   40,  296,   12,
   59,   59,   45,  318,  348,   44,  568,   41,   41,    0,
   44,   44,  292,  907,  908,  283,  296,  465,   45,   59,
   41,  333,   59,   44,  820, 1711,   59,  455,  340, 1292,
  342, 1384,  344,  338,  356,  347, 1212,  257,   59,   59,
   59,   40,    0,  272,  938,  366, 1399,    0,  161,  356,
   41,  358,  376,  333,  365,  345,  618,  473,   44,  288,
  340,  301,  342,    0,  344,  372,   44,  347,   59,  972,
   41,   41, 1264,   44,  219,  273,  374, 1292, 1292,  597,
 1292,  984,  985,   41,  300,  365,  366,    0,   41,  262,
 1298,   44,  260, 1292,  374,  301,  376,  980,    0,  287,
   41,   59,  405, 1292,  333, 1012,   59,  429,   44,  493,
 1588,  841,  300,    0,  334,   93, 1802,  461,  462,  503,
    0,  424,   59,  461,  462,  138,  517,  492,   41,    0,
  411,  677, 1477,  362,  481, 1311,  352,  442,  453, 1236,
 1697, 1225,  340, 1037,  424,  479,   59, 1281,  279, 1056,
 1404,  277,  972,  464,   41,    0,  262,   59,  318,    0,
  318,  473,  269,  461,  984,  260,  311,  320,  479,  300,
   41,  439,   59,   44,  472,    0,  320,  694,  476,   59,
  340,  461,  340,  481,  464,  465,  884,  328,   59,  583,
  331,   41,  472,  710,  740,  264,  476,  309,  329,  479,
  429,  481,  442,    0,  484,  264,  319,  340,  303,  262,
  274, 1689,    0,  386,   59,    0,  762,  348,   59,  484,
  782, 1115, 1116,  785, 1118, 1119,  357,  318,  383,  387,
  537,  410,  363,  318,   59, 1241,  442,  799,  396,  279,
  436,  387,  279,  340, 1209,  278, 1438,  280, 1524,  417,
  381, 1104, 1470,  565,  287,  340,   41,  340,  440,  387,
  300,  458,   59,  300,  826,  304, 1744, 1099,  396,  367,
  340,   59,  454,  721,   59,  409, 1484,  387, 1304,  586,
  386, 1088,  440,  458,  564,  565,  307,  308,  868,  329,
  570,  736,  329,  458,  454, 1429,  454,  436,  781,  440,
 1434, 1127, 1436,  274,  447,  759,  458, 1785,  348,  458,
  485,  348,  331,  454,  454,  977, 1219,  357,   61,  356,
  357,  454, 1287,  363,  855,  584,  363,  318, 1412,  641,
  452,  638,  639,  386,  458,    0,  584, 1471,  309, 1489,
  458,  381,  502,  759,  381,  378,  501,  367,    0,  340,
  744,  501,  410, 1247, 1451, 1708,  393,  454,  395,  877,
  318,  641,  454,  431,  454,  318,   41,  400,  304,  454,
  650,  454, 1469,  331,  407, 1269,   41,  411,  258,   44,
  650,  418,  340,  777,  778,    0,  517,  340,  431,  410,
  458,  271,  680,  264,   59,  394, 1303,  383,  402,  733,
  680,  713,  466, 1537,  431,  318,  443,   59,  441, 1543,
    0,  348,  262,  318,  259,  458,  386,  562, 1424,  264,
 1025,  440,  733,  264,  704,  270,  306,  340,  308,  387,
  451,  458,  712,  713,  980,  454,  716, 1335,  396,  264,
  411,  721, 1425,  283,   59,  386,  348,  759,  257,  320,
  730,  308,  764,  733, 1717,  753,  768,  755,  738, 1593,
    0,  318,  742,  454,  771,  772,  773,  264,  501,   59,
    0,  493,  494,  753,  387,  755, 1610,  517,  758,  791,
  517, 1043,  440,  396,  764, 1090,    0,  632,  768, 1472,
    0,    0, 1038, 1055,   41,  466,  454,  806,   41, 1045,
  387,  454, 1717, 1717,    0, 1717,  273,  819,  820,  396,
    0,  791,  506,  961, 1076,   41,  964, 1400, 1717,   59,
 1414,  780, 1102,    0,  831,  334, 1267,  440, 1717,   59,
 1189,   41,  780,  318,  262,   40,  386,  301,  418,  819,
  820,  454,    0,  328,  384,   59,  331,   41,  453,   59,
   59,   41,  397,  268,  342,  340,  397,  300,  856,  818,
 1290,  977, 1696,   59, 1223, 1363, 1296,  259,  866,   59,
  818, 1705,  397,  260,  338,   42,  856,  825,  270,  320,
   47,  452,   59,  307,  308, 1165,  866,  262,   12,  460,
  838,  387,  851,  852,  853,    0,  453,  348,  878,  439,
  397,   59,  387,  851,  852,  853,  357, 1643,    0, 1645,
  263,  396,  264,  278, 1748,  280,  456,  405,  489,  490,
  408,  492,  287,   59,  325, 1259,  262,  262,  499,  500,
  501,  502,  503,  965,  505,    0,  424,  508,  509,  510,
  511,  512,  963,  258,  440,  516,  517,  308,  405,   41,
  365,  962,   44,    0,   59,  440,  271,  318,  386,  312,
 1179,    0,  450, 1182,    0,  977, 1212,   59, 1704,  454,
  323, 1305, 1189,  966,  264, 1201,   41, 1203,  442,   44,
 1074,  961,  962,  963,  964,  965,  966, 1699,  377,  307,
  308,  306,  972,  308,   59,  436,  313,   16,  409,  456,
 1756, 1757,    0,    0,  984,  985, 1223,  435,  261,  989,
 1308,  386,   59,  378,  138,  262,    0, 1279,  383,  262,
   59,  458,  989,   59,  264,  319,    0,  451,   44,  430,
  417,  458,    0,  445,  264,  400,  437,  405,   40, 1751,
 1049, 1050,  407,   41,   41,  397, 1292,   44, 1310, 1545,
  386,  386,  265, 1363, 1648, 1522,   58, 1363, 1525,  448,
   44,   59,   59, 1799,   83, 1311,  517,    0,  264,  278,
   44,  280, 1666,   41,  427,   59,  441,   93,  280,  284,
 1092,  258, 1416,  285, 1796,   59, 1066, 1099,  456,  302,
 1099,   59,  453, 1849,  271,   44,  508,  402, 1068,  435,
  417, 1081,  279,  418,    0,  331,  400,  397,  318, 1207,
 1208, 1209, 1092,  280,  328, 1775, 1421,  331,  285, 1099,
  278,  397,  280,  300, 1083,    0,   59,  655,  318,  306,
  340,  308,  660,  451,  328, 1083,  501,  331,  328,  386,
  445,  331, 1101,  386,   93, 1612, 1740,  441, 1107, 1243,
  340, 1245,  329, 1101, 1400,  352,  322,  397, 1377, 1107,
  436,  387, 1381,   59,  260,  322,  258,  397,  279,  378,
  396,  348,  401,  278,  343,  280, 1402,  387, 1440,  271,
  357,  378,  301,    0,   59,   40,  363,  279,  258,    0,
    0,  383,    0,  387,  470,   42,   43,  387,   45,  368,
   47,  397,  396,  508,  381,  377,  396,  303,  300,   44,
    0,  258, 1306,  278,  306,  280,  308, 1479,  329,  338,
    0, 1201,  287, 1203,  271,  264,  440,    0,  264,  383,
  440,   41,  279,   44, 1241,  385,  306,  329,  452, 1219,
  454,  418,   59,  301,  454, 1225,  418,  436,   59,   59,
  440,   59,  363,  300, 1848,  460,  348,  426,   93,  306,
  403,  308,  452,  879,  454,  357, 1600,  883,    0,   59,
  381,  363, 1606,   42,   43, 1563,   45,  420,   47,   59,
  338,  278,  329,  280,  489,  490,   59,  492, 1755,  381,
  287, 1407,   44,  465,  499,  500,  501,  502,  503, 1518,
  505,  348,  260,  508,  509,  510,  511,  512,  256,  356,
  357,  516,  517,  378, 1321, 1285,  363, 1433, 1325,   42,
   43,  259,   45,  383,   47,  258,  418,   59,    0,   46,
  278,    0,  270,  331,  381,  400,   59, 1769,  271,  287,
  517,   58,  407,  301,   61, 1607,  279,    0,  418,  407,
  318, 1783,  880,  383,  328,  313,  884,  331,  397,  417,
  328,  397,  494,  331,  258,  260,  436,  300,  265,  343,
  318,  418,  340,  306,  337,  308,  441,  271,  418, 1811,
  338,  378,  429,  274,  442,  321,  260,   59,   41,  387,
   59,   44,   42,   43,  318,   45,  329,   47,  396,  335,
  470,    0, 1701,  400, 1836,  302,   59,  343,  303, 1841,
  407, 1527,  306, 1432,  308,  348,  424, 1424,  337,  387,
 1432, 1433, 1402,  359,  357,  517, 1524, 1407,  396,  303,
  363,   40, 1412,  280,   42,   43,  501,   45,  285,   47,
  337,  258,   41, 1441,  441, 1675, 1540, 1563,  381,  407,
 1749,   40, 1432, 1433,  271,  374,  316,  317,  460,  417,
   59, 1441,  279,  264,    0,   40,  440,  342,  387,  417,
  517, 1717,  440,  421,  422,  423,  461,  413,  452,  316,
  454,  278,  260,  300,  442,  418,  454,  489,  490,  306,
  492,  308, 1780,  340,  318,    0, 1795,  499,  500,  501,
  502,  503,  377,  505,  501, 1722,  508,  509,  510,  511,
  512,  280,  329,    0,  516,  517,  285,  328,  328, 1546,
  331,  331,  319,   59,  418,  303, 1814, 1591,  337, 1828,
  405,  348,  343,  408,  342, 1800, 1801,  485, 1675, 1769,
  357,  489,  311,  337, 1774,  260,  363, 1527,  465,  424,
  467, 1563,  342, 1783,   59,  279, 1844,  280,  402, 1675,
 1709, 1710,  285,    0,  381,  374, 1546, 1855, 1592,  377,
 1858,  417,   59,  460, 1554,  450,  300,  387,  387, 1867,
  336, 1811,   41, 1563,  517,   44,  396,  377,  303, 1587,
   42,   43,  378,   45,  438,   47, 1861,  405,  444,  356,
  408,  418, 1582,  400, 1753,  329, 1836, 1587,   40, 1616,
  342, 1841, 1592, 1620,  400,  405,  424,  340,  408,  238,
  282,  407,   59,    0,  348, 1611, 1690,  246,  290,  440,
  280, 1611, 1769,  357,  424,  285,  393, 1774,  395,  363,
  452,  452,  450,  454,  441,  377, 1783,    0,    0,    0,
 1661,   41,  402, 1769,   44,  441,    0,  381, 1774,  340,
  450,  418,    0,    0, 1780,  318,  458, 1783,    0,  417,
  320,  320,  280,  405, 1811,  328,  408,  285,  331,    0,
 1829, 1661,   59,  294,  318,    0,  443,  340,   41,    0,
  343,   44,  424,  485,  305, 1811,  444,  321, 1814, 1836,
  517,  312,  402,   93, 1841, 1732,   59,   59,   59,   44,
  318,  335,  660, 1699,    0,   59, 1683,    0,  450, 1699,
 1836,   59,   59,   44,   59, 1841,  337,   59, 1844,  677,
 1716, 1711,  383,  681,  387,  359, 1716,    0,   59, 1855,
  688,  383, 1858,  396,   59,  693,  694,   40,   59,  436,
    0, 1867, 1732, 1733,    0,  703,  321,  705,  706,  707,
  708,  709,  710,  711, 1734, 1751,  461,  462, 1780,   44,
  335, 1751,    0,   59, 1760,  506,   59,    0,   42,   43,
 1760,   45,    0,   47,   59, 1809,    0,  440,  361,  413,
  342,  402,  740,  517,  359, 1775,   59,  321,    0,  452,
 1780,  454, 1814,    0,    0,  402,    0,  450,  417,   59,
 1796,  335,  434,   59,  762,    0, 1796,    0,  383,  767,
 1806,   41, 1802,  434,   44,  377, 1806,  438,  280, 1809,
  384,   59, 1844,  285, 1814,  359,   59,  434,    0,   41,
  417,   59,   44, 1855,   41,   59, 1858,   44,  413,  391,
  392,  460,  449,  405, 1834, 1867,  408,   59,   41,  383,
  442,   44,   59,   59, 1844,   59,   42,   43,  320,   45,
  454,   47,  424,   93,   59, 1855,   59, 1857, 1858,  516,
  489,  490, 1862,  492, 1864,  460, 1862, 1867, 1864,  413,
  499,  500,  501,  502,  503,  516,  505,   59,  450,  508,
  509,  510,  511,  512,   46,  287,   46,  516,  517,  321,
   41,  264,  411,   44,  489,  490,   58,  492,   58,   61,
 1201,   61, 1203,  335,  499,  500,  501,  502,  503,  258,
  505,  343,  261,  508,  509,  510,  511,  512,  461,  462,
   41,  516,  517,   44,  892,  893,  894,  359,  469,  897,
  898,  899,   44,  901,  902,  903,  904,  905,  906,   61,
   41,  909,  910,   44,  912,  318,  914,    0,  321,  917,
  918,  919,  920,  921,  922,  328,   41,  306,  331,   44,
   41,   41,  335,   44,   44,  372,  934,  340,  936,   41,
  343,  286,   44,   41,   42,   43,   40,   45,    0,   47,
  321,  413,   41,  264,   40,   44,  359,  328,   41,    0,
  331,   44,   41,   40,  335,   44,  280,  457,  458,  459,
   41,  285,  343,   44,   41,   41,   59,   44,   44,  676,
  383,  678,  980,   40,  387,  682,  683,  684,  359,   41,
  461,   41,   44,  396,   44,  485,  486,  311,   40,   40,
   41,   42,   43,   44,   45,   46,   47,   59,   41,   61,
  413,   44,  383,   41,   41,   40,   44,   44,   59,   60,
   61,   62,   42,   43,   44,   45,  278,   47,  280,   40,
  461,   41,  284,    0,   44,  287,   40,  440,   40,  265,
   40,   40,  413,  461,   40,   40,   40,  418,  300,  452,
   91,  454,   93,   44,  280,   42,   43,  460,   45,  285,
   47,   91,   41,   42,   43,   40,   45,  436,   47,  440,
   46,  320,   44,   40,   41,  402,  302,   44,  320,   44,
  264,  452,   44,  454,  428,   46,  489,  490,   61,  492,
   42,   43,   59,   45,   40,   47,  499,  500,  501,  502,
  503,  264,  505,   44,   44,  508,  509,  510,  511,  512,
  382,   42,  505,  516,  517,  300,  348,  460,   41,   42,
   43,  348,   45,  348,   47,  278,  378,  264,  897, 1127,
  899,  278,  901,  902,  397,  904,  905,  906,  436,  470,
  397, 1139, 1140,  458, 1142, 1143,  489,  490,  400,  492,
  277,  348, 1150,  447,  447,  407,  499,  500,  501,  502,
  503,    0,  505,  399,   40,  508,  509,  510,  511,  512,
  349, 1169,   42,  516,  517,   40,   91,   40,   40, 1177,
 1178, 1179,  280,  318, 1182,  383, 1184,  285,  264,  441,
 1188, 1189, 1190, 1191,  411,  278,   44,  280,  389,  382,
   40,  284,   41,   40,  287,   44,  258,   41,  451,  464,
  317,   40,  436,  452, 1212,  320,  320,  300,  448,  264,
   59,  262,  274,  264,  265, 1223,  502,    0,   40,  458,
  277,  272,   40,  284,  274,  300,  344,  386,  344,  280,
  278,  386,  386,  418,  285,  416,  300,  288,  300,  418,
   40,  417,   40,  356,  306,  274,  417,  309,   40,  264,
  280,  302,   41,   44, 1262,  285,  307,  308,   41,   44,
  311,   44,  320,  280,   44,  262,  385,  318,   44,  320,
  321,  320,  367,  324,   40,   44,   59,  328,  266,   44,
  331,  328,  333,  280,  335,  378,   40,  501,  285,  340,
  341,  280,  343,   40,  264,  422,  285,  299,   41,   42,
   43,   41,   45, 1311,   47,   58,   41,  400,  359,  360,
   41,  362,   41,  309,  407,  264,  454,  440,  280,  309,
  394,   41,  492,  285, 1332,  376,   41,  378,  379,  380,
   40,   40,  383, 1341,  352,  386,  387,   40,   40,   40,
 1348,  318,   40,   40,  321,  396,   40,  280,  441,  411,
   40,  328,  285,   40,  331,  296,  418,   40,  335,  410,
   40,   40,  413,  340,   40,   58,  343,   40,   40, 1377,
  262,  308,  435, 1381,  436,  327,   44,   41,  429,   41,
  388,  388,  359,  343,  435,    0,  274,  331,  388,  440,
  422,  390, 1400,  391,  271,   44,  287,   41,  378,  386,
  451,  452,  446,  454,  466, 1413,  383,  386,  470,  460,
  387,   41,  407,  386,  278,   41,   41,   42,   43,  396,
   45,  386,   47,  386,   40,  386,   41,   42,   43,   44,
   45,   46,   47,   41,   41,   41,  413,  308,  489,  490,
  385,  492,  435,  417,   59,   60,   61,   62,  499,  500,
  501,  502,  503,  396,  505,  436,  280,  508,  509,  510,
  511,  512,  338,  440,  442,  516,  517,   45,  337,  318,
  301,  338,  301,  442,   41,  452,   91,  454,   93,  328,
   41,  385,  331,  460,  352,  320,  371,  371,   41, 1497,
 1498,  340,   41,   41,  343,  278,   41,  280,   41,   41,
   41,   41,   41,   41,  287,   41, 1514,   41,   41,   41,
 1518,   41,  489,  490,   41,  492,   41,  300,   44,  460,
   40,  340,  499,  500,  501,  502,  503,  340,  505,  308,
   59,  508,  509,  510,  511,  512,   59,  280,  387,  516,
  517,   59,  285, 1551,   59,   59,   59,  396,  489,  490,
   59,  492,   58,   41,  344,  352,   40,  368,  499,  500,
  501,  502,  503,   40,  505,   59,   40,  508,  509,  510,
  511,  512,  451,  383,   44,  516,  517,   59,   59,   46,
  460,  340,   41,   42,   43,   41,   45,  383,   47,   42,
   43,  440,   45,  343,   47,  378,  383,   41,   41,   44,
  442,  410,  435,  452,   40,  454,   59, 1615,   59,  489,
  490,  463,  492,  301,  387, 1623,  337,  400,    0,  499,
  500,  501,  502,  503,  407,  505,  304,   44,  508,  509,
  510,  511,  512,  307,  293,  304,  516,  517,   44,   41,
   40, 1649,   41,    0,  383,  337,  383,  262,   40,  264,
  265,  264, 1660,  264,  278,  280, 1664,  272,  441,   41,
  285,  389,   44,  458,  320,  280,  264,  470,   44,  411,
  285,  320,   40,  288,   41,   42,   43,   59,   45,  451,
   47,  309,  271,  458,  264,  436,  435,  302,  343,  304,
  343,    0,  307,  308,  458,  343,  311,   45,   41,   42,
   43,  343,   45,  318,   47,  320,  321,  271,   41,  324,
   41,   40,   40,  328, 1722,   41,  331,   59,  333,   59,
  335,  308,   41,   59,   41,  340,  341,  304,  343,   59,
   41,    0,   41,   42,   43,   44,   45,   46,   47,    0,
    0, 1204,   12,  265,  359,  360, 1104,  362,  645,  273,
   59,   60,   61,   62,   42,   43,  266,   45,  869,   47,
 1102,  376,  721,  378,  379,  380,   87,  460,  383, 1099,
 1459,  386,  387, 1091, 1782, 1460,  330,  360,  592,  323,
 1081,  396,   91, 1092,   93,  313,  342, 1028,    0, 1582,
  342, 1658, 1438, 1742, 1746,  410,  489,  490,  413,  492,
  764, 1440,  570, 1745, 1787, 1260,  499,  500,  501,  502,
  503,  460,  505, 1722,  429,  508,  509,  510,  511,  512,
  435,  280, 1738,  516,  517,  440,  285,  280, 1554,   41,
 1857, 1680,  285,  801,  791, 1781,  451,  452, 1238,  454,
  489,  490, 1014,  492, 1417,  460, 1699,   59,  347,  335,
  499,  500,  501,  502,  503,  572,  505,  328, 1294,  508,
  509,  510,  511,  512,  848,   13,  300,  516,  517,  631,
  504,  500,  753,  840,  489,  490,  143,  492,  767,  740,
  458, 1649,  460,  989,  499,  500,  501,  502,  503, 1806,
  505,  729, 1535,  508,  509,  510,  511,  512, 1760,  981,
 1249,  516,  517,  515,   -1, 1184,   -1,    0, 1141,   -1,
   -1,  489,  490,  280,  492,   -1,   -1,   -1,  285,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  318,  280,  516,  517,
   -1,   -1,  285,   -1,   -1,   -1,   -1,  295,   41,  331,
   -1,   44,   -1,  262,   -1,  264,  265,   -1,  340,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   59,   -1,   -1,    0,
   -1,  280,   -1,   -1,  262,  323,  285,   -1,   -1,  288,
   -1,   41,   42,   43,   -1,   45,    0,   47,   -1,   -1,
   -1,   -1,  280,  302,   -1,    0,   -1,  285,  307,  308,
   -1,   -1,  311,   -1,  352,  387,   -1,   -1,   -1,  318,
   41,  320,  321,   44,  396,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   41,   59,   -1,
   44,  340,  341,   -1,  343,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   61,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,  440,  472,
  473,  474,  475,  476,  477,  478,  479,  376,   -1,  378,
  379,  380,  454,   -1,  383,   -1,   -1,  386,  387,  427,
   -1,   -1,   -1,   -1,   -1,   59,   91,  396,   93,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,   -1,  318,   -1,   -1,  457,
  458,  459,   -1,  461,  462,   -1,   -1,   -1,   -1,  331,
  429,   -1,   -1,   41,   42,   43,  435,   45,  340,   47,
   -1,  440,  480,  481,  482,  483,  484,  485,  486,  487,
  488,   -1,  451,  452,   -1,  454,   -1,  495,  496,   -1,
   -1,  460,   -1,   -1,   -1,   -1,  504,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   41,   42,
   43,   -1,   45,    0,   47,  387,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,  396,  278,   -1,  280,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  440,   -1,
   -1,   -1,   59,   -1,   61,   -1,   -1,   41,   42,   43,
   -1,   45,  454,   47,  258,   -1,   -1,  278,   -1,  280,
   -1,   -1,   -1,   -1,   -1,   -1,  287,  262,   -1,  264,
  265,   -1,   -1,  277,  278,   -1,  280,  272,   -1,  300,
  284,   -1,   -1,  287,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,  378,  300,   -1,   -1,   -1,
    0,   -1,  306,   -1,   -1,   -1,   -1,  302,   -1,    0,
   -1,   -1,  307,  308,   -1,   -1,  311,  400,   -1,   -1,
   -1,   -1,   -1,  318,  407,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   59,   -1,  331,   -1,  333,   -1,
  335,   41,   -1,   -1,   44,  340,  341,   -1,  343,   -1,
   41,   42,   43,   44,   45,   46,   47,  378,  441,   59,
   -1,   61,   -1,   -1,  359,  360,   -1,  362,   59,   60,
   61,   62,  280,   -1,  378,   -1,   -1,  285,   -1,  400,
   -1,  376,   -1,  378,  379,  380,  407,   -1,  383,    0,
   -1,  386,  387,   -1,   -1,   -1,  400,   -1,   -1,   -1,
   91,  396,   93,  407,   -1,   -1,  280,   -1,   -1,   -1,
   -1,  285,  416,   -1,  418,  410,   -1,  280,  413,   -1,
  441,   -1,  285,   41,   42,   43,   -1,   45,   -1,   47,
   41,   -1,  436,   44,  429,   -1,   -1,  441,   -1,   -1,
  435,  258,   -1,   -1,   -1,  440,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,
  277,  278,   -1,  280,   -1,  460,  470,  284,   -1,   -1,
  287,   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,  300,   -1,   -1,  280,   -1,   -1,  306,
   -1,  285,   -1,   -1,  489,  490,  460,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  258,   -1,
   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,  278,   -1,
  280,  272,   -1,  400,  284,   -1,   -1,  287,   -1,  280,
  407,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
  300,  418,   -1,   -1,   -1,   -1,  306,    0,   -1,   -1,
   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,  436,
  311,   -1,   -1,   -1,  441,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,    0,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   40,   41,  340,
  341,   44,  343,  470,   41,   42,   43,   44,   45,   -1,
   47,   -1,  280,   -1,   -1,   -1,   59,  285,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,   -1,  378,   41,
   -1,   -1,   44,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   59,   -1,   -1,
  400,   -1,   -1,   -1,   -1,  396,   93,  407,   -1,   -1,
  321,  280,   -1,   -1,   -1,   -1,  285,  328,  418,  410,
  331,   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  343,   -1,   -1,  460,  436,   -1,  429,   -1,
   -1,  441,   -1,   -1,  435,   -1,   -1,   -1,  359,  440,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  451,  452,   -1,  454,  489,  490,   -1,  492,   -1,  460,
  470,   -1,  383,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   41,  489,  490,
   44,  492,  413,   -1,   -1,   -1,    0,   -1,  499,  500,
  501,  502,  503,   -1,  505,   59,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  440,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   41,   42,
   43,  452,   45,  454,   47,   -1,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  302,  278,    0,  280,   -1,
  307,  308,   -1,   -1,  311,  318,   -1,   -1,  321,   -1,
   -1,  318,   -1,  320,  321,  328,  287,  324,  331,   -1,
   -1,  328,  335,   -1,  331,   -1,  333,  340,  335,   -1,
  343,   -1,   -1,  340,  341,   -1,  343,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,  359,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  383,  378,  379,  380,  387,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,  396,
   93,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
  413,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  278,   -1,  280,   -1,  400,   -1,
   -1,   -1,  429,  287,   -1,  407,   -1,  440,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,  451,  452,   -1,  454,   -1,  280,
   -1,   -1,   -1,  460,  285,   -1,   -1,  280,   -1,  441,
   -1,   -1,  285,   -1,  278,   -1,  280,   -1,   -1,   -1,
   -1,   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,  460,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,    0,  378,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  400,  508,  509,  510,
  511,  512,   -1,  407,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,  262,
   47,  264,  265,   -1,  378,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   59,   60,   61,   62,   -1,  441,   -1,   -1,
   -1,  328,  285,   -1,   -1,  288,  400,   -1,   -1,   -1,
   -1,   -1,   -1,  407,   -1,   -1,   -1,   -1,   -1,  302,
   -1,   -1,   -1,   -1,  307,  308,   93,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,  441,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1, 1333, 1334,   -1,   -1,   -1,
    0, 1339, 1340,   -1, 1342, 1343,  359,  360,    0,  362,
   -1,   -1, 1350, 1351, 1352, 1353,   -1, 1355, 1356, 1357,
 1358, 1359, 1360,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,  396,   44,   -1,  443,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,  410,   -1,   59,
  413,   61,   -1,  460,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   91,  489,  490,   -1,  492,   -1,   -1,  451,  452,
   -1,  454,  499,  500,  501,  502,  503,  460,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  305,   -1,   -1,  262,   -1,  264,  265,  312,
   -1,   -1,   -1,  316,  317,  272,  489,  490,   -1,  492,
   -1,   -1,  325,   -1,   -1,   -1,  499,  500,  501,  502,
  503,  288,  505,   -1,  337,  508,  509,  510,  511,  512,
   -1,  294,   -1,  516,  517,  302,   -1,    0,   -1,   -1,
  307,  308,  305,   -1,  311,   -1,   -1,   -1,   -1,  312,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,  325,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   41,   42,
   43,   44,   45,   -1,   47,   -1,   -1,   -1,   -1,  402,
   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,  419,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,  434,   -1,  386,   -1,  438,   -1,   -1,  258,  396,
   93,   -1,   -1,   -1,   -1,   -1,  449,   -1,   -1,  402,
   -1,   -1,   -1,  410,  274,   -1,  413,  277,  278,   -1,
  280,   -1,   -1,   -1,  284,  468,  419,  287,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
  300,  434,   -1,  440,   -1,  438,  306,   -1,  441,  309,
   -1,   -1,   -1,   -1,  451,  452,  449,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,  468,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  359,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  378,  516,
  517,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,
  400,   -1,   -1,   -1,  396,   -1,   -1,  407,   -1,   -1,
  287,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,
   -1,  413,   -1,   -1,   41,   -1,   43,   44,   45,  262,
   -1,  264,  265,   -1,   -1,   -1,  436,   -1,   -1,  272,
   -1,  441,   59,   60,   61,   62,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,  466,   -1,   -1,  302,
  470,   -1,   -1,   -1,  307,  308,   93,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  359,  360,    0,  362,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   40,   41,   -1,  396,   44,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,  410,   -1,   59,
  413,   61,   -1,  460,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   91,  489,  490,   -1,  492,   -1,   -1,  451,  452,
   -1,  454,  499,  500,  501,  502,  503,  460,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,  288,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  302,   -1,    0,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   41,   -1,
   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  396,
   93,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  410,  274,   -1,  413,  277,  278,   -1,
  280,   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
  300,   -1,   -1,  440,   -1,   -1,  306,   -1,   -1,  309,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  359,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  378,  516,
  517,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,
  400,   -1,   -1,   -1,  396,   -1,   -1,  407,   -1,   -1,
   -1,  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,
   -1,  413,   -1,   -1,   41,   -1,   43,   44,   45,  262,
   -1,  264,  265,   -1,   -1,   -1,  436,   -1,   -1,  272,
   -1,  441,   59,   60,   61,   62,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,  466,   -1,   -1,  302,
  470,   -1,   -1,   -1,  307,  308,   93,   -1,  311,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,  434,   -1,   -1,
   -1,   41,   -1,  396,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,  410,   -1,   59,
  413,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  451,  452,
   -1,  454,  499,  500,  501,  502,  503,  460,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,  288,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  302,   -1,    0,   -1,   -1,
  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   41,   -1,
   43,   44,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,    0,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   93,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,  318,   -1,
   -1,  321,   -1,  460,   -1,   -1,   -1,   -1,  328,  318,
   -1,  331,  321,   -1,   -1,  335,   -1,   -1,   -1,  328,
  340,   -1,  331,  343,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,  489,  490,  343,  492,   -1,   -1,   -1,  359,
  460,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  359,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,  489,
  490,   -1,  492,   -1,  383,   -1,  396,  386,  387,  499,
  500,  501,  502,  503,   -1,  505,   -1,  396,  508,  509,
  510,  511,  512,  413,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,  262,
   -1,  264,  265,   -1,   -1,  435,   -1,   -1,   -1,  272,
  440,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,  452,   -1,  454,  288,   -1,   -1,   -1,   40,
   -1,   -1,   43,  452,   45,  454,   -1,   -1,   -1,  302,
   -1,   -1,   -1,   -1,  307,  308,   -1,   58,  311,   -1,
   -1,   -1,   63,   -1,   -1,  318,   -1,  320,  321,   -1,
   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,  396,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,   -1,   -1,   58,  328,   -1,  410,  331,   63,
  413,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,  359,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
  413,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,  267,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  314,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   41,  516,  517,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,  267,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  292,   -1,
   -1,  295,  296,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  314,   -1,   58,   -1,   -1,   -1,  417,   63,   -1,  323,
  421,   -1,  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  378,  443,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,  460,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,  513,  514,  515,  516,  517,  421,   -1,   -1,
   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,  433,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,  443,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  260,   -1,   -1,   -1,   -1,  318,
   -1,  267,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,
  276,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,  289,  343,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   40,   -1,   42,   43,  303,   45,   -1,
  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  323,   -1,   -1,
  326,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,  355,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,  435,   44,   -1,   40,
  261,  440,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,  452,   -1,  454,   -1,   58,   -1,   -1,
   -1,  282,   63,   -1,   -1,   -1,   -1,   -1,   -1,  290,
   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,  299,   -1,
  301,  427,   -1,   -1,  343,  306,   -1,  433,   -1,   -1,
   -1,   -1,  313,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  327,   -1,   -1,  368,
   -1,  457,  458,  459,  460,  461,  462,  338,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,  426,   -1,   -1,
  516,  517,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,  405,   -1,   -1,   -1,   -1,  276,
   -1,  412,   -1,   -1,  415,   -1,  417,  418,   -1,   -1,
   -1,  460,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,  323,   -1,   -1,  326,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  262,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,  352,  267,  354,  355,   40,
  491,   -1,   43,   -1,   45,  276,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,  506,  507,   58,  289,   -1,
   -1,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  314,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,  323,   -1,  331,  326,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,  359,  354,  355,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
  457,  458,  459,  460,  461,  462,  383,  378,   -1,  386,
  387,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,  413,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  421,  508,  509,  510,  511,  512,  427,   -1,  435,  516,
  517,   -1,  433,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,  267,  498,  499,  500,
  501,  502,  503,  504,  505,  276,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   40,  289,   -1,
   43,   -1,   45,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,
   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,  273,   -1,   -1,   -1,  277,  378,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
  291,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  421,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,   -1,  443,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  406,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  267,  516,  517,   -1,   -1,   -1,
   -1,   -1,  423,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
  323,   -1,   -1,  326,   -1,   -1,   -1,  280,   -1,  480,
  481,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  497,   -1,   -1,   -1,
   -1,  354,  355,   -1,  260,    0,   -1,  263,   -1,   -1,
   -1,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
  276,  324,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
  333,   -1,   -1,  289,   -1,   -1,   -1,   -1,  341,  295,
  296,   -1,   40,   -1,   -1,   43,   41,   45,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,  362,
   58,   -1,   -1,   -1,   59,   63,   61,  323,  421,   -1,
  326,   -1,   -1,  376,  427,  378,  379,  380,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   91,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  429,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  425,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  328,  471,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,  260,  258,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,  274,
   -1,   -1,  277,  278,   -1,  280,   -1,   -1,  402,  284,
   -1,  289,  287,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   40,   -1,  300,   43,   -1,   45,   -1,
  460,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  438,  323,   63,   -1,  326,  443,
   -1,   -1,   -1,   -1,   -1,  449,   -1,   -1,   -1,  489,
  490,   -1,  492,   -1,   -1,   -1,  460,   -1,   -1,  499,
  500,  501,  502,  503,  468,  505,  354,  355,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,
   -1,   -1,  370,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,  378,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,  400,   -1,   -1,   -1,   -1,
   -1,   -1,  407,   -1,  260,   -1,  411,  263,   -1,   -1,
   -1,  267,   -1,  418,   -1,   -1,   -1,  425,   -1,  427,
  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,  436,   -1,  289,   -1,  443,  441,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,  466,   -1,  471,   -1,  470,   -1,  323,   -1,   -1,
  326,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  354,  355,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  366,   -1,   -1,  260,  370,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,
   58,  427,   -1,   -1,   -1,   63,  323,  433,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  271,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,  301,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  312,
  313,   -1,  315,   -1,   40,   -1,   -1,   43,   -1,   45,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,  332,
   -1,   -1,   58,   -1,  471,  338,   -1,   63,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,  398,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,  417,   -1,   -1,  295,  296,   -1,
   -1,   -1,  460,   40,   -1,   -1,   43,   -1,   45,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,
   -1,   58,   -1,   -1,   -1,  323,   63,   -1,  326,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  460,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,  354,  355,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,  366,  492,
   -1,   -1,  370,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,   -1,   -1,
   -1,  267,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  354,  355,
  508,  509,  510,  511,  512,   -1,    0,   -1,  516,  517,
  366,   -1,   -1,  260,  370,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   41,  295,  296,
   44,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,  425,
   58,  427,   -1,   -1,   -1,   63,  323,  433,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,  516,
  517,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   40,   -1,   42,   43,   -1,   45,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,  289,   -1,   -1,   -1,   63,   -1,  295,  296,   -1,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   60,   61,
   62,   -1,   -1,   -1,  328,  323,   -1,  331,  326,   -1,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   40,   -1,  370,   43,   -1,   45,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  396,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,
   -1,  435,   -1,   -1,   -1,  433,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  267,
  498,  499,  500,  501,  502,  503,  504,  505,  276,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
  272,    0,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,  324,   -1,  352,   -1,  354,  355,   -1,   -1,
   59,  333,   -1,   -1,   -1,   -1,   -1,  267,  366,  341,
   -1,   -1,  370,   -1,   -1,   -1,  276,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,   -1,   -1,   -1,  360,  289,
  362,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,
   -1,   -1,   63,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,  429,   -1,  457,
  458,  459,  460,  461,  462,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  417,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   40,   -1,  433,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,  457,  458,  459,
  460,  461,  462,  262,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  276,  516,  517,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,    0,   -1,  289,  328,
   -1,   -1,  331,   -1,  295,  296,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,   -1,  323,   -1,   -1,  326,   -1,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   59,   -1,  386,  387,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  366,    0,   -1,   -1,  370,
   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,  260,   -1,  452,   -1,  454,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   59,  427,  276,   -1,   -1,
   40,   -1,  433,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,  443,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,  303,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  354,  355,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  262,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,  457,  458,
  459,  460,  461,  462,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,  471,   -1,   -1,   -1,  340,   -1,  262,  343,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,  359,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  276,  516,  517,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,  289,
   -1,   -1,  396,  260,  318,  295,  296,  321,   -1,   -1,
   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  413,
   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,  435,   -1,   -1,  301,  359,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  313,   -1,  452,   -1,
  454,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,  383,
  327,   -1,  386,  387,  267,   -1,  366,   -1,   -1,   -1,
  370,  338,  396,  276,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,  413,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,  427,  452,   -1,
  454,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
  407,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
  417,  354,  355,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,  460,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   -1,  451,  516,
  517,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,   -1,   -1,   40,   41,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  460,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  469,  470,  471,  472,  473,
  474,  475,  476,  477,  478,  479,  480,  481,  482,  483,
  484,   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  417,   -1,  499,  500,  501,  502,  503,
  504,  505,  506,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,  291,   -1,   -1,   -1,  295,   -1,  471,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,  339,
   -1,  276,  516,  517,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,
   -1,  411,  267,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,  276,   -1,  423,   40,   -1,   -1,   43,   -1,   45,
   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,  445,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,  497,  433,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
  260,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  301,  427,  508,  509,  510,  511,  512,  433,   -1,
   -1,  516,  517,  313,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  338,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  276,  516,  517,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,  407,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,  489,
  490,   58,  492,   -1,   -1,   -1,   63,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,  454,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,  291,
   -1,   -1,   -1,  295,   -1,  471,  298,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,  339,   -1,  276,
  516,  517,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,
  267,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  276,
   -1,  423,   40,   -1,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,
   58,   59,   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,  497,  433,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,  271,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  427,  508,  509,  510,  511,  512,  433,   -1,   -1,  516,
  517,   -1,   -1,   -1,  301,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  312,  313,  454,  315,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,  332,   -1,   -1,   -1,   -1,
   -1,  338,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,  516,
  517,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,  398,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  417,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,   -1,  354,  355,   -1,   -1,
   -1,   40,   -1,  460,   43,   -1,   45,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  273,   -1,   -1,   58,
  277,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  295,   -1,
   -1,  298,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,  339,   -1,   -1,  443,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,  406,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,  267,   -1,
    0,   -1,   -1,   -1,   -1,  354,  355,  276,   -1,   -1,
  497,   -1,    0,   -1,   -1,   -1,   -1,  366,   -1,   -1,
  289,  370,   -1,   -1,   -1,   -1,  295,  296,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   41,   -1,   -1,   44,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   41,  323,   -1,   44,  326,   -1,   59,
   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,  427,   91,
   41,   -1,   -1,   44,  433,  354,  355,   -1,   -1,   -1,
   -1,   91,   -1,   -1,  443,   -1,   -1,  366,   59,   -1,
   -1,  370,   -1,   91,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  427,  508,
  509,  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  258,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,
   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,
  258,   -1,  284,   -1,  274,  287,   -1,   -1,  278,   -1,
  280,    0,   -1,   -1,  284,   -1,  274,  287,  300,   -1,
  278,   -1,  280,   -1,  306,   -1,  284,  309,   -1,  287,
  300,  262,   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,
   -1,   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,
   -1,  309,   41,   42,   43,   44,   45,   46,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,  378,  328,   -1,   -1,
  331,   -1,   91,   -1,  335,   -1,   -1,   -1,  378,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  400,   -1,
  378,   -1,   -1,   -1,   -1,  407,   -1,   -1,  359,  411,
  400,   -1,   -1,   -1,   -1,   -1,  418,  407,   -1,   -1,
   -1,  411,  400,   -1,   -1,   -1,   -1,   -1,  418,  407,
   -1,   -1,  383,  411,  436,  386,  387,   -1,   -1,  441,
  418,   -1,   -1,   -1,   -1,  396,  436,   -1,   -1,   -1,
   -1,  441,    0,   -1,   -1,   -1,  273,   -1,  436,   -1,
  277,   -1,  413,  441,  466,   -1,   -1,   -1,  470,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,  295,   -1,
  470,  298,   -1,   -1,  435,   -1,   -1,   -1,  466,  440,
   -1,   -1,  470,   41,   42,   43,   44,   45,   46,   47,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
   -1,   -1,   -1,   91,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  423,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,  445,   -1,
   -1,  340,  341,   -1,  343,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,    0,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
  497,   -1,   -1,   -1,   -1,   -1,   91,  396,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,  413,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   59,   -1,   -1,
  429,   -1,   -1,   -1,  272,   -1,  435,   -1,   -1,   -1,
    0,  440,  280,   -1,   -1,   41,   -1,  285,   44,   -1,
  288,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  318,   41,   -1,  321,   44,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   59,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   59,   -1,   -1,   -1,  413,   -1,  272,   -1,   -1,
   -1,   -1,   -1,   -1,    0,  280,   -1,   -1,   -1,   -1,
  285,  429,   -1,  288,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   41,  321,   -1,   44,  324,
  262,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   59,   -1,  340,  341,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,  360,  262,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,  318,   -1,  383,  321,
   -1,  386,  387,   -1,   -1,   -1,  328,   41,   -1,  331,
   44,  396,  262,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   59,   -1,   -1,  413,   -1,
   -1,   -1,  318,   -1,   -1,  321,   -1,  359,   -1,   -1,
   -1,   -1,  328,   -1,  429,  331,   -1,   -1,  262,  335,
  435,   -1,   -1,   -1,  340,  440,   -1,  343,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,  452,  318,  454,
   -1,  321,   -1,  359,  396,   -1,   -1,   -1,  328,    0,
   -1,  331,   -1,   -1,  262,  335,   -1,   -1,   -1,   -1,
  340,  413,   -1,  343,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,  318,   -1,   -1,  321,   -1,  359,
  396,    0,   -1,  435,  328,   -1,   -1,  331,  440,   -1,
   41,  335,   -1,   44,   -1,   -1,  340,  413,   -1,  343,
  452,   -1,  454,  383,   -1,   -1,  386,  387,   59,   -1,
  318,   -1,   -1,  321,   -1,  359,  396,   -1,   -1,  435,
  328,   -1,   41,  331,  440,   44,  262,  335,   -1,   -1,
   -1,   -1,  340,  413,   -1,  343,  452,   -1,  454,  383,
   59,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  396,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  413,
   -1,   -1,  452,   -1,  454,  383,   -1,   -1,  386,  387,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  396,   -1,
   -1,  435,  328,   -1,    0,  331,  440,   -1,  262,  335,
   -1,   -1,   -1,   -1,  340,  413,   41,  343,  452,   44,
  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,   59,   -1,   -1,  435,    0,   -1,
   -1,   -1,  440,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,  383,   -1,   -1,
  386,  387,   -1,   59,  318,   -1,   -1,  321,   -1,   -1,
  396,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   41,
   -1,  335,   44,   -1,   -1,   -1,  340,  413,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  452,   -1,  454,  383,
   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,  435,   -1,   -1,   -1,   -1,  440,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  452,  340,
  454,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,  359,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,  262,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
  359,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,
   -1,   -1,  413,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,  318,  435,   -1,  321,   -1,   -1,  440,
  262,   -1,   -1,  328,  413,   -1,  331,   -1,   -1,   -1,
  335,  452,   -1,  454,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,  318,   -1,   -1,  321,  435,   -1,   -1,   -1,
   -1,  440,  328,   -1,  359,  331,   -1,   -1,  271,  335,
   -1,   -1,   -1,  452,  340,  454,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  383,  321,
   -1,  386,  387,  359,   -1,   -1,  328,   -1,  301,  331,
   -1,  396,   -1,  335,   -1,  308,   -1,   -1,  340,  312,
  313,  343,  315,   -1,   -1,  318,   -1,  383,  413,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,  359,   -1,  332,
  396,   -1,   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,  413,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,  452,   -1,  454,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
  273,   -1,   -1,   -1,  277,  398,   -1,   -1,   -1,   -1,
   -1,  284,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,  295,   -1,  417,  298,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,  432,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,
  453,   -1,   -1,   -1,   -1,   -1,  339,  460,   -1,   -1,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
  373,   -1,  375,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,  273,   -1,   -1,   -1,  277,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,   -1,   -1,  497,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
  339,   -1,   -1,  516,  517,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,  470,
  471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
  481,  482,  483,  484,   -1,   -1,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  406,  499,  500,
  501,  502,  503,  504,  505,  506,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  423,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 517
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
"KW_DEBUG","DECIMAL","DECLARE","DEFAULT","KW_DELETE","DESC","DISTINCT","DO",
"DOMAIN","DROP","ELSE","END","ENTRY_POINT","EQL","ESCAPE","EXCEPTION","EXECUTE",
"EXISTS","EXIT","EXTERNAL","FILTER","FOR","FOREIGN","FROM","FULL","FUNCTION",
"GDSCODE","GEQ","GENERATOR","GEN_ID","GRANT","GROUP","GROUP_COMMIT_WAIT","GTR",
"HAVING","IF","KW_IN","INACTIVE","INNER","INPUT_TYPE","INDEX","INSERT",
"INTEGER","INTO","IS","ISOLATION","JOIN","KEY","KW_CHAR","KW_DEC","KW_DOUBLE",
"KW_FILE","KW_FLOAT","KW_INT","KW_LONG","KW_NULL","KW_NUMERIC","KW_UPPER",
"KW_VALUE","LENGTH","LOGFILE","LPAREN","LEFT","LEQ","LEVEL","LIKE",
"LOG_BUF_SIZE","LSS","MANUAL","MAXIMUM","MAX_SEGMENT","MERGE","MESSAGE",
"MINIMUM","MODULE_NAME","NAMES","NATIONAL","NATURAL","NCHAR","NEQ","NO","NOT",
"NOT_GTR","NOT_LSS","NUM_LOG_BUFS","OF","ON","ONLY","OPTION","OR","ORDER",
"OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","PAGE_SIZE","PARAMETER",
"PASSWORD","PLAN","POSITION","POST_EVENT","PRECISION","PRIMARY","PRIVILEGES",
"PROCEDURE","PROTECTED","RAW_PARTITIONS","READ","REAL","REFERENCES","RESERVING",
"RETAIN","RETURNING_VALUES","RETURNS","REVOKE","RIGHT","RPAREN","ROLLBACK",
"SEGMENT","SELECT","SET","SHADOW","KW_SHARED","SINGULAR","KW_SIZE","SMALLINT",
"SNAPSHOT","SOME","SORT","SQLCODE","STABILITY","STARTING","STATISTICS",
"SUB_TYPE","SUSPEND","SUM","TABLE","THEN","TO","TRANSACTION","TRIGGER",
"UNCOMMITTED","UNION","UNIQUE","UPDATE","USER","VALUES","VARCHAR","VARIABLE",
"VARYING","VERSION","VIEW","WAIT","WHEN","WHERE","WHILE","WITH","WORK","WRITE",
"FLOAT_NUMBER","NUMBER","NUMERIC","SYMBOL","STRING","INTRODUCER","ACTION",
"ADMIN","CASCADE","FREE_IT","RESTRICT","ROLE","COLUMN","TYPE","EXTRACT","YEAR",
"MONTH","DAY","HOUR","MINUTE","SECOND","WEEKDAY","YEARDAY","TIME","TIMESTAMP",
"CURRENT_DATE","CURRENT_TIME","CURRENT_TIMESTAMP","NUMBER64BIT","SCALEDINT",
"CURRENT_USER","CURRENT_ROLE","KW_BREAK","SUBSTRING","RECREATE","KW_DESCRIPTOR",
"FIRST","SKIP","CURRENT_CONNECTION","CURRENT_TRANSACTION","BIGINT","CASE",
"NULLIF","COALESCE","USING","NULLS","LAST","ROW_COUNT","LOCK","SAVEPOINT",
"RELEASE","STATEMENT","LEAVE","INSERTING","UPDATING","DELETING","KW_INSERTING",
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE",
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
"privilege : KW_DELETE",
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
"db_rem_option : KW_DIFFERENCE KW_FILE sql_string",
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
"delete_rule : ON KW_DELETE referential_action",
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
"for_select : label_opt FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : label_opt FOR EXECUTE varstate value INTO variable_list DO proc_block",
"exec_into : EXECUTE varstate value INTO variable_list ';'",
"if_then_else : IF '(' search_condition ')' THEN proc_block ELSE proc_block",
"if_then_else : IF '(' search_condition ')' THEN proc_block",
"post_event : POST_EVENT value event_argument_opt ';'",
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
"while : label_opt WHILE '(' search_condition ')' DO proc_block",
"label_opt : symbol_label_name ':'",
"label_opt :",
"breakleave : KW_BREAK ';'",
"breakleave : LEAVE ';'",
"breakleave : LEAVE symbol_label_name ';'",
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
"invoke_procedure : EXECUTE PROCEDURE symbol_procedure_name proc_inputs",
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
"trigger_type_suffix : KW_DELETE",
"trigger_type_suffix : INSERT OR UPDATE",
"trigger_type_suffix : INSERT OR KW_DELETE",
"trigger_type_suffix : UPDATE OR INSERT",
"trigger_type_suffix : UPDATE OR KW_DELETE",
"trigger_type_suffix : KW_DELETE OR INSERT",
"trigger_type_suffix : KW_DELETE OR UPDATE",
"trigger_type_suffix : INSERT OR UPDATE OR KW_DELETE",
"trigger_type_suffix : INSERT OR KW_DELETE OR UPDATE",
"trigger_type_suffix : UPDATE OR INSERT OR KW_DELETE",
"trigger_type_suffix : UPDATE OR KW_DELETE OR INSERT",
"trigger_type_suffix : KW_DELETE OR INSERT OR UPDATE",
"trigger_type_suffix : KW_DELETE OR UPDATE OR INSERT",
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
"db_alter_clause : ADD KW_DIFFERENCE KW_FILE sql_string",
"db_alter_clause : DROP KW_DIFFERENCE KW_FILE",
"db_alter_clause : BEGIN BACKUP",
"db_alter_clause : END BACKUP",
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
"release_savepoint : RELEASE SAVEPOINT symbol_savepoint_name release_only_opt",
"release_only_opt : ONLY",
"release_only_opt :",
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
"lock_type : KW_SHARED",
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
"select : union_expr order_clause for_update_clause lock_clause",
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
"for_update_clause : FOR UPDATE for_update_list",
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
"select_item : rhs as_noise symbol_item_alias_name",
"as_noise : AS",
"as_noise :",
"from_clause : FROM from_list",
"from_list : table_reference",
"from_list : from_list ',' table_reference",
"table_reference : joined_table",
"table_reference : table_proc",
"table_reference : derived_table",
"derived_table : '(' select ')' as_noise correlation_name derived_column_list",
"correlation_name : symbol_table_alias_name",
"correlation_name :",
"derived_column_list : '(' alias_list ')'",
"derived_column_list :",
"alias_list : symbol_item_alias_name",
"alias_list : alias_list ',' symbol_item_alias_name",
"joined_table : table_reference join_type JOIN table_reference ON search_condition",
"joined_table : '(' joined_table ')'",
"table_proc : symbol_procedure_name proc_table_inputs as_noise symbol_table_alias_name",
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
"group_clause : GROUP BY group_by_list",
"group_clause :",
"group_by_list : group_by_item",
"group_by_list : group_by_list ',' group_by_item",
"group_by_item : value",
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
"access_type : ORDER symbol_index_name extra_indices_opt",
"index_list : symbol_index_name",
"index_list : symbol_index_name ',' index_list",
"extra_indices_opt : INDEX '(' index_list ')'",
"extra_indices_opt :",
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' insert_value_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt ordered_select_expr",
"insert_value_list : rhs",
"insert_value_list : insert_value_list ',' rhs",
"delete : delete_searched",
"delete : delete_positioned",
"delete_searched : KW_DELETE FROM table_name where_clause",
"delete_positioned : KW_DELETE FROM table_name cursor_clause",
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
"search_condition : trigger_action_predicate",
"search_condition : NOT trigger_action_predicate",
"search_condition : simple_search_condition",
"search_condition : search_condition OR search_condition",
"search_condition : search_condition AND search_condition",
"bracable_search_condition : simple_search_condition",
"bracable_search_condition : NOT trigger_action_predicate",
"bracable_search_condition : bracable_search_condition OR search_condition",
"bracable_search_condition : bracable_search_condition AND search_condition",
"bracable_search_condition : special_trigger_action_predicate OR search_condition",
"bracable_search_condition : special_trigger_action_predicate AND search_condition",
"simple_search_condition : predicate",
"simple_search_condition : '(' bracable_search_condition ')'",
"simple_search_condition : NOT simple_search_condition",
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
"in_predicate : value KW_IN in_predicate_value",
"in_predicate : value NOT KW_IN in_predicate_value",
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
"special_trigger_action_predicate : KW_INSERTING",
"special_trigger_action_predicate : KW_UPDATING",
"special_trigger_action_predicate : KW_DELETING",
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
"numeric_value_function : extract_expression",
"extract_expression : EXTRACT '(' timestamp_part FROM value ')'",
"string_value_function : substring_function",
"string_value_function : KW_UPPER '(' value ')'",
"substring_function : SUBSTRING '(' value FROM value string_length_opt ')'",
"string_length_opt : FOR value",
"string_length_opt :",
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
"non_reserved_word : INSERTING",
"non_reserved_word : UPDATING",
"non_reserved_word : DELETING",
"non_reserved_word : BACKUP",
"non_reserved_word : KW_DIFFERENCE",
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
	lex.prev_keyword = -1;
	lex.prev_prev_keyword = -1;
	lex.limit_clause = false;	
	lex.first_detection = false;
	lex.brace_analysis = false;
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
static bool long_int(DSQL_NOD string,
					 SLONG *long_value)
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
			return false;
		}
	}

	*long_value = atol ((char *)((STR) string)->str_data);

	return true;
}
#endif

static DSQL_FLD make_field (DSQL_NOD field_name)
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
	TSQL	tdsql;

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
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;
		   
	temp_file = FB_NEW(*tdsql->tsql_default) fil;

	return temp_file;
}


static DSQL_NOD make_list (DSQL_NOD node)
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
	TSQL	tdsql;

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
	TSQL	tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
	node->nod_type = nod_parameter;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = 1;
	node->nod_arg[0] = (DSQL_NOD)(ULONG) lex.param_number++;

	return node;
}


static DSQL_NOD make_node (NOD_TYPE	type,
						   int count,
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
	TSQL	tdsql;

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


static DSQL_NOD make_flag_node (NOD_TYPE	type,
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
	TSQL	tdsql;

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
#if defined(DEV_BUILD)
	DSQL_debug = level;
#endif
	if (level >> 8)
		*yydeb = level >> 8;
	/* CVC: I added this code form Mike Nordell to see the output from internal
	   operations that's generated in DEV build when DEBUG <n> is typed into isql.exe.
	   When n>0, the output console is activated; otherwise it's closed. */
#if defined(DEV_BUILD) && defined(WIN_NT) && defined(SUPERSERVER)
	static FILE* redirected_output;
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
static bool short_int(DSQL_NOD string,
					  SLONG *long_value,
					  SSHORT range)
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
		return false;
	}

	for (char* p = ((STR) string)->str_data; classes [*p] & CHR_DIGIT; p++)
	{
		if (!(classes [*p] & CHR_DIGIT)) {
			return false;
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

	bool return_value;

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

static void stack_nodes (DSQL_NOD	node,
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
	DSQL_NOD	 curr_node, next_node, start_chain, end_chain, save_link;

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
	end_chain = NULL;
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
		while (true)
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
	int temp = lex.yylex(client_dialect, db_dialect, parser_version, stmt_ambiguous);
	lex.prev_prev_keyword = lex.prev_keyword;
	lex.prev_keyword = temp;
	return temp;
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

	STR delimited_id_str;

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

		bool have_error	 = false;	/* syntax error or value too large */
		bool have_digit	 = false;	/* we've seen a digit			  */
		bool have_decimal   = false;	/* we've seen a '.'				*/
		bool have_exp	   = false;	/* digit ... [eE]				  */
		bool have_exp_sign  = false; /* digit ... [eE] {+-]			 */
		bool have_exp_digit = false; /* digit ... [eE] ... digit		*/
		UINT64	number		 = 0;
		UINT64	limit_by_10	= MAX_SINT64 / 10;

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
				have_error = true;
				break;
			}
			else if (have_exp)
			{
				/* We've seen e or E, but nothing beyond that. */
				if ( ('-' == c) || ('+' == c) )
					have_exp_sign = true;
				else if ( classes [c]  & CHR_DIGIT )
					/* We have a digit: we haven't seen a sign yet,
					but it's too late now. */
					have_exp_digit = have_exp_sign  = true;
				else
				{
					/* end of the token */
					have_error = true;
					break;
				}
			}
			else if ('.' == c)
			{
				if (!have_decimal)
					have_decimal = true;
				else
				{
					have_error = true;
					break;
				}
			}
			else if (classes [c] & CHR_DIGIT)
			{
				/* Before computing the next value, make sure there will be
				   no overflow.  */

				have_digit = true;

				if (number >= limit_by_10)
				/* possibility of an overflow */
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
					}
				number = number * 10 + (c - '0');
			}
			else if ( (('E' == c) || ('e' == c)) && have_digit )
				have_exp = true;
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
				yylval = (DSQL_NOD) (ULONG) number;
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
		sym = HSHD_lookup (NULL, (TEXT *) string, (SSHORT)(p - string), SYM_keyword, parser_version);
		if (sym)
		{
		/* 13 June 2003. Nickolay Samofatov
		 * Detect INSERTING/UPDATING/DELETING as non-reserved keywords.
		 * We need to help parser from lexer because our grammar is not LARL(1) in this case
		 */
			if (prev_keyword == '(' && !brace_analysis &&
				(sym->sym_keyword == INSERTING ||
				 sym->sym_keyword == UPDATING ||
				 sym->sym_keyword == DELETING
				) &&
				/* Produce special_trigger_action_predicate only where we can handle it -
				  in search conditions */
				(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
				 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
				 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
			{			
				LexerState savedState = lex;
				int nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
				lex = savedState;
				if (nextToken==OR || nextToken==AND) {
					switch(sym->sym_keyword) {
					case INSERTING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_INSERTING;
					case UPDATING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_UPDATING;
					case DELETING:
						yylval = (DSQL_NOD) sym->sym_object;
						return KW_DELETING;
					}
				}
			}
		/* 23 May 2003. Nickolay Samofatov
		 * Detect FIRST/SKIP as non-reserved keywords
		 * 1. We detect FIRST or SKIP as keywords if they appear just after SELECT and
		 *   immediately before parameter mark ('?'), opening brace ('(') or number
		 * 2. We detect SKIP as a part of FIRST/SKIP clause the same way
		 * 3. We detect FIRST if we are explicitly asked for (such as in NULLS FIRST/LAST clause)
		 * 4. In all other cases we return them as SYMBOL
		 */
			if ((sym->sym_keyword == FIRST && !first_detection) || sym->sym_keyword == SKIP) {
				if (prev_keyword == SELECT || limit_clause) {
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
						last_token_bk = last_token;
						line_start_bk = line_start;
						lines_bk = lines;
						return sym->sym_keyword;
					}
				} /* else fall down and return token as SYMBOL */
			} else {
				yylval = (DSQL_NOD) sym->sym_object;
				last_token_bk = last_token;
				line_start_bk = line_start;
				lines_bk = lines;
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
		sym = HSHD_lookup (NULL, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
		if (sym)
		{
			++ptr;
			return sym->sym_keyword;
		}
	}
		
	/* We need to swallow braces around INSERTING/UPDATING/DELETING keywords */
	/* This algorithm is not perfect, but it is ok for now. 
	  It should be dropped when BOOLEAN datatype is introduced in Firebird */
	if ( c == '(' && !brace_analysis && 
		/* 1) We need to swallow braces in all boolean expressions
		   2) We may swallow braces in ordinary expressions 
		   3) We should not swallow braces after special tokens 
			 like IF, FIRST, SKIP, VALUES and 30 more other	   
		*/
		(prev_keyword=='(' || prev_keyword==NOT || prev_keyword==AND || prev_keyword==OR ||
		 prev_keyword==ON || prev_keyword==HAVING || prev_keyword==WHERE || prev_keyword==WHEN) ) 
	{
		LexerState savedState = lex;	
		brace_analysis = true;
		int openCount = 0;
		int nextToken;
		do {
			openCount++;
			nextToken = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
		} while (nextToken == '(');
		DSQL_NOD temp_val = yylval;
		if (nextToken == INSERTING || nextToken == UPDATING || nextToken == DELETING)
		{
			/* Skip closing braces. */
			while ( openCount &&
					yylex(client_dialect,db_dialect,
						  parser_version,stmt_ambiguous) == ')')
			{
				openCount--;
			}
			if (openCount) {
				/* Not enough closing braces. Restore status quo. */
				lex = savedState;
			}
			else {
				/* Cool! We successfully swallowed braces ! */
				brace_analysis = false;
				yylval = temp_val;
				/* Check if we need to handle LR(2) grammar case */
				if (prev_keyword == '(' &&
					/* Produce special_trigger_action_predicate only where we can handle it -
					  in search conditions */
					(prev_prev_keyword=='(' || prev_prev_keyword==NOT || prev_prev_keyword==AND || 
					 prev_prev_keyword==OR || prev_prev_keyword==ON || prev_prev_keyword==HAVING || 
					 prev_prev_keyword==WHERE || prev_prev_keyword==WHEN) ) 
				{			
					savedState = lex;
					int token = yylex(client_dialect,db_dialect,parser_version,stmt_ambiguous);
					lex = savedState;
					if (token==OR || token==AND) {
						switch(nextToken) {
						case INSERTING:
							return KW_INSERTING;
						case UPDATING:
							return KW_UPDATING;
						case DELETING:
							return KW_DELETING;
						}
					}
				}
				return nextToken;
			}
		} else {
			/* Restore status quo. */
			lex = savedState;
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
		gds_arg_gds, gds_command_end_err,	/* Unexpected end of command */
		0);
	else
	{
		ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -104,
		gds_arg_gds, gds_dsql_token_unk_err, 
		gds_arg_number, (SLONG) lex.lines, 
		gds_arg_number, (SLONG) (lex.last_token - lex.line_start + 1), /*CVC: +1*/
			/* Token unknown - line %d, char %d */
		gds_arg_gds, gds_random, 
		gds_arg_cstring, (int) (lex.ptr - lex.last_token), lex.last_token, 0);
	}
}


static void yyabandon (SSHORT		sql_code,
					   ISC_STATUS	error_symbol)
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
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-2]; }
break;
case 80:
{ yyval = NULL; }
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
				NULL, MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));}
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
{ yyval = NULL; }
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
{ yyval = NULL; }
break;
case 136:
{ yyval = NULL; }
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
{yyval = NULL;}
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
{yyval = NULL;}
break;
case 163:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 167:
{ yyval = make_node (nod_dfl_charset, 1, yyvsp[0]);}
break;
case 168:
{ yyval = make_node (nod_difference_file, 1, yyvsp[0]); }
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
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);  /* Log redefined */
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 174:
{ if (lex.log_defined)
				yyabandon (-260, isc_log_redef);
			  lex.log_defined = TRUE;
			  yyval = yyvsp[0]; }
break;
case 175:
{ lex.g_file->fil_flags |= LOG_serial | LOG_overflow; 
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			/* Partitions not supported in series of log file specification */
			 yyval = make_node (nod_list, 2, yyvsp[-3], yyvsp[0]); }
break;
case 176:
{ lex.g_file->fil_flags |= LOG_serial;
			  if (lex.g_file->fil_partitions)
				  yyabandon (-261, isc_partition_not_supp);
			  yyval = yyvsp[0]; }
break;
case 177:
{ lex.g_file = make_file(); 
			  lex.g_file->fil_flags = LOG_serial | LOG_default;
			  yyval = make_node (nod_log_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file);}
break;
case 178:
{ lex.g_file->fil_name = (STR) yyvsp[-1]; 
			  yyval = (DSQL_NOD) make_node (nod_file_desc, (int) 1,
						(DSQL_NOD) lex.g_file); }
break;
case 180:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 181:
{ 
				 check_log_file_attrs(); 
			 yyval = (DSQL_NOD) make_node (nod_log_file_desc, (int) 1,
												(DSQL_NOD) lex.g_file); }
break;
case 182:
{ lex.g_file = make_file();
			  lex.g_file->fil_name = (STR) yyvsp[0]; }
break;
case 185:
{ lex.g_file->fil_length = (SLONG) yyvsp[0]; }
break;
case 186:
{ lex.g_file  = make_file ();}
break;
case 191:
{ lex.g_file->fil_start = (SLONG) yyvsp[0];}
break;
case 192:
{ lex.g_file->fil_length = (SLONG) yyvsp[-1];}
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
{ yyval = NULL; }
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
			lex.g_field->fld_flags |= FLD_computed;
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
{ yyval = NULL; }
break;
case 218:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 219:
{ lex.g_field = make_field (yyvsp[0]);
				  yyval = (DSQL_NOD) lex.g_field; }
break;
case 220:
{ yyval = yyvsp[-1]; }
break;
case 221:
{ lex.g_field = make_field (NULL);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 222:
{ yyval = yyvsp[0]; }
break;
case 223:
{ yyval = NULL; }
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
{ yyval = NULL; }
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
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 318:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 319:
{ yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
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
{ yyval = make_node (nod_while, e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 337:
{ yyval = make_node (nod_label, 1, yyvsp[-1]); }
break;
case 338:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
break;
case 340:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL); }
break;
case 341:
{ yyval = make_node (nod_breakleave, e_breakleave_count,
				make_node (nod_label, 1, yyvsp[-1])); }
break;
case 342:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 345:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 348:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 353:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 354:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_select, e_select_count, yyvsp[0], NULL, NULL, NULL); }
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
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
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
{ lex.beginning = lex_position(); }
break;
case 369:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 370:
{ lex.beginning = lex.last_token; }
break;
case 371:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 372:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 373:
{ yyval = 0; }
break;
case 374:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 375:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
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
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
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
case 452:
{ yyval = NULL; }
break;
case 453:
{ yyval = NULL; }
break;
case 454:
{ yyval = NULL; }
break;
case 455:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 456:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 457:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 458:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 459:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 460:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 461:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 462:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 464:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 465:
{ yyval = yyvsp[0]; }
break;
case 466:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 467:
{ yyval = yyvsp[0]; }
break;
case 468:
{ yyval = yyvsp[0]; }
break;
case 469:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 470:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 471:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 472:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
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
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 498:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
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
			lex.g_field->fld_dtype = dtype_int64; 
			lex.g_field->fld_length = sizeof (SINT64); 
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
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
			lex.g_field->fld_dtype = dtype_sql_time; 
			lex.g_field->fld_length = sizeof (SLONG);
			}
break;
case 513:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 516:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 520:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 521:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 522:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 523:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 524:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 525:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 527:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 541:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 542:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 543:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 544:
{		 
			if ( ((SLONG) yyvsp[-1] < 1) || ((SLONG) yyvsp[-1] > 18) )
				yyabandon (-842, isc_precision_err);
				/* Precision most be between 1 and 18. */ 
			if ((SLONG) yyvsp[-1] > 9)
				{
				if ( ( (client_dialect <= SQL_DIALECT_V5) &&
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
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
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 545:
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
				   (db_dialect	 >  SQL_DIALECT_V5) ) ||
				 ( (client_dialect >  SQL_DIALECT_V5) &&
				   (db_dialect	 <= SQL_DIALECT_V5) ) )
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
			lex.g_field->fld_precision = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_scale = - (SSHORT)(SLONG) yyvsp[-1];
			}
break;
case 548:
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
case 549:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 550:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 552:
{ yyval = yyvsp[-1]; }
break;
case 553:
{ yyval = 0; }
break;
case 557:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 558:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 559:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 563:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 564:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 565:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 566:
{ yyval = 0; }
break;
case 567:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 570:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 571:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 574:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 575:
{ yyval = NULL; }
break;
case 577:
{ yyval = NULL; }
break;
case 578:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 580:
{ yyval = NULL; }
break;
case 582:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 587:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 588:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 591:
{ yyval = yyvsp[0];}
break;
case 593:
{ yyval = yyvsp[0];}
break;
case 594:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 601:
{ yyval = 0; }
break;
case 602:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 603:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 604:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 605:
{ yyval = (DSQL_NOD) 0; }
break;
case 606:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 607:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 609:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 610:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 611:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 612:
{ yyval = 0; }
break;
case 614:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 615:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 616:
{ yyval = make_node (nod_select, e_select_count, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 619:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 620:
{ yyval = make_list (yyvsp[0]); }
break;
case 621:
{ yyval = 0; }
break;
case 623:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 624:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 629:
{ yyval = 0; }
break;
case 630:
{ yyval = yyvsp[-1]; }
break;
case 631:
{ yyval = 0; }
break;
case 632:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 633:
{ yyval = 0; }
break;
case 634:
{ yyval = yyvsp[0]; }
break;
case 635:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 636:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 637:
{ yyval = 0; }
break;
case 638:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 639:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 640:
{ lex.limit_clause = true; }
break;
case 641:
{ lex.limit_clause = false; }
break;
case 642:
{ lex.first_detection = true; }
break;
case 643:
{ lex.first_detection = false; }
break;
case 644:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 645:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 646:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 647:
{ yyval = 0; }
break;
case 648:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 649:
{ yyval = yyvsp[-2]; }
break;
case 650:
{ yyval = yyvsp[-1]; }
break;
case 651:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 652:
{ yyval = yyvsp[-1]; }
break;
case 653:
{ yyval = yyvsp[0]; }
break;
case 654:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 655:
{ yyval = 0; }
break;
case 656:
{ yyval = make_list (yyvsp[0]); }
break;
case 657:
{ yyval = 0; }
break;
case 659:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 661:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 664:
{ yyval = make_list (yyvsp[0]); }
break;
case 666:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 670:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 672:
{ yyval = NULL; }
break;
case 673:
{ yyval = make_list (yyvsp[-1]); }
break;
case 674:
{ yyval = NULL; }
break;
case 676:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 677:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 678:
{ yyval = yyvsp[-1]; }
break;
case 679:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 681:
{ yyval = make_list (yyvsp[-1]); }
break;
case 682:
{ yyval = NULL; }
break;
case 684:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 688:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 689:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 690:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 691:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 698:
{ yyval = make_list (yyvsp[0]); }
break;
case 699:
{ yyval = 0; }
break;
case 701:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 703:
{ yyval = yyvsp[0]; }
break;
case 704:
{ yyval = 0; }
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
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 710:
{ yyval = 0; }
break;
case 711:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 712:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 713:
{ yyval = 0; }
break;
case 714:
{ yyval = 0; }
break;
case 716:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 717:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 720:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 722:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 723:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 725:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 726:
{ yyval = make_list (yyvsp[-1]); }
break;
case 727:
{ yyval = 0; }
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
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 777:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 778:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = yyvsp[-1]; }
break;
case 787:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 798:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 827:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 828:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 830:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 831:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 832:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 833:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 834:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 835:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 836:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 837:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 838:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 839:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 840:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 841:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 842:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 843:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 844:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 847:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 849:
{ yyval = make_list (yyvsp[-1]); }
break;
case 850:
{ yyval = yyvsp[-1]; }
break;
case 851:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 852:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 862:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 863:
{ yyval = yyvsp[0]; }
break;
case 864:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 865:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 866:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 867:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 868:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 869:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 870:
{ yyval = yyvsp[-1]; }
break;
case 871:
{ yyval = yyvsp[-1]; }
break;
case 875:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 876:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 877:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 878:
{ yyval = yyvsp[0]; }
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
			yyval = make_node (nod_current_date, 0, NULL);
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 881:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 882:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 884:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 886:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 887:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 888:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 889:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 890:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 891:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 893:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 894:
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
case 895:
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
case 896:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 897:
{ yyval = make_parameter (); }
break;
case 898:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 899:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 900:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 901:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 902:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 903:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 904:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 905:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 906:
{ yyval = yyvsp[0]; }
break;
case 907:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 909:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 910:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 911:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 912:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 913:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 915:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 916:
{ yyval = yyvsp[0];}
break;
case 921:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 922:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 923:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 924:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 925:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 926:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 927:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 928:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 929:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 930:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 932:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 934:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 938:
{ yyval = yyvsp[0]; }
break;
case 939:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 940:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 942:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 945:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 946:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 949:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 950:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 951:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 952:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 953:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 954:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 955:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 956:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 960:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 961:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 962:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 964:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 965:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 966:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 967:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 970:
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
