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
  225,  229,  229,  230,  230,  230,  231,  231,  147,  220,
   87,   89,  219,  219,   70,   83,  235,  235,  235,  236,
  239,  239,  240,  240,  240,  240,  240,  240,  240,  240,
  240,  240,  240,  240,  240,  240,  240,  237,  237,  238,
    2,  242,  242,  242,  242,  242,  242,  242,  250,  251,
  248,  248,  252,  252,  252,  252,  252,  252,  252,  243,
  243,  253,  253,  253,  253,  253,  253,  253,  247,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  258,  258,  258,  258,  258,  258,  258,  258,
  258,  258,  255,  255,  257,  257,  256,  254,  254,  254,
  249,  249,  245,  246,  246,  260,  260,  260,  260,  260,
  260,  260,  260,  261,  261,  244,  262,  262,  263,  263,
    8,  264,  264,  264,  264,  264,  264,  264,  264,  264,
  264,  264,  264,  101,  101,  144,  144,  267,  267,  270,
  270,  272,  272,   53,   53,  269,  269,  269,  269,  269,
  269,  269,  269,  269,  277,  277,  268,  268,  268,  268,
  278,  278,   61,   61,   61,   55,   55,  274,  274,  274,
  271,  271,  271,  282,  282,  282,  281,  281,  280,  280,
  280,  275,  275,  283,  283,  283,  284,  284,  276,  276,
  276,  276,  285,  285,   18,   18,   18,  287,  287,  287,
   16,   16,   16,  289,  290,  293,  293,  291,  295,  295,
    4,   15,  294,  294,  296,  296,  297,  297,  286,  298,
  298,  299,  299,  300,  300,  300,  300,  301,  301,  302,
  302,  303,  303,  305,  305,  305,  306,  306,  306,  307,
  307,  307,  304,  309,  309,  309,  310,  310,  308,  308,
  311,  313,  313,  312,  312,  288,   17,  314,  314,  314,
  315,  315,  319,  319,  320,   65,   65,   65,  322,  322,
  321,  321,  316,  316,  325,  325,  317,  317,  318,  329,
  330,  331,  323,  324,  326,  326,  326,  326,  332,  332,
  332,  333,  333,  333,  223,  223,  224,  224,  336,  336,
  337,  337,  339,  339,  327,  341,  341,  342,  342,  342,
  233,  345,  345,  346,  346,  348,  348,  343,  343,  344,
  344,  349,  349,  210,  210,  350,  350,  232,  232,   31,
  234,  234,  234,  234,  234,  234,  234,  234,  227,  227,
  351,  351,  352,  228,  228,  226,  226,  328,  328,  353,
  354,  354,  354,  354,  354,  355,  355,  356,  356,  357,
  357,  358,  358,  358,  359,  359,  360,  360,   10,   10,
  362,  362,    7,    7,  363,  364,  365,   19,   19,  366,
  367,  368,  368,  188,  338,  338,    3,    3,  370,  370,
  370,  372,  372,   61,  371,  371,  373,  373,   34,   34,
   85,   84,   84,  361,  361,  374,  375,  375,  211,  211,
  211,  146,  369,  369,  111,  111,  111,  111,  111,  379,
  379,  379,  379,  379,  379,  378,  378,  378,  381,  381,
  381,  381,  381,  381,  381,  381,  381,  381,  382,  382,
  382,  382,  382,  382,  382,  382,  387,  387,  387,  387,
  387,  387,  387,  387,  387,  387,  387,  387,  387,  387,
  387,  387,  393,  393,  383,  383,  384,  384,  384,  384,
  385,  385,  389,  389,  390,  390,  390,  390,  388,  391,
  386,  386,  377,  377,  377,  380,  380,  380,  394,  394,
  395,  392,  397,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,  156,
  156,  156,  398,  396,  396,  151,  151,  404,  404,  404,
  404,  404,  400,  400,  400,  400,  400,  334,  152,  152,
  153,  154,  154,  154,  154,  154,   51,   51,   20,   20,
  241,  405,   54,  123,  273,  273,   95,  399,  399,  399,
  399,  406,  406,  406,  406,  406,  406,  406,  406,  406,
  406,  406,  407,  408,  410,  409,  409,  412,  413,  413,
  403,  403,  401,  402,  402,  414,  414,  415,  415,  416,
  416,  419,  419,  417,  417,  422,  422,  421,  418,  420,
  411,  411,  411,  411,  411,  411,  411,  411,  335,  335,
  155,   48,  279,  122,  148,  376,  165,  213,  266,   63,
   60,  217,  112,   66,  340,  212,   35,   41,  347,  265,
   39,   42,  209,   40,  292,  259,  259,  423,  423,  423,
  423,  423,  423,  423,  423,  423,  423,  423,  423,  423,
  423,  423,  423,
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
    2,    1,    3,    1,    1,    1,    6,    3,    0,    0,
    0,    0,    3,    0,    9,    9,    1,    1,    0,    2,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    5,    5,    5,    5,    5,    5,    2,    0,    4,
    2,    3,    3,    2,    2,    3,    3,    2,    3,    6,
    1,    2,    4,    3,    2,    2,    2,    2,    3,    1,
    3,    3,    3,    2,    2,    4,    4,    5,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    2,    1,    1,    1,    0,
    2,    2,    0,    1,    2,    2,    2,    2,    2,    4,
    3,    2,    2,    1,    3,    7,    1,    0,    1,    0,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    3,
    2,    2,    2,    1,    1,    1,    1,    4,    5,    1,
    3,    1,    3,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    4,    4,    6,    5,
    3,    0,    2,    2,    0,    3,    0,    4,    1,    5,
    4,    1,    4,    1,    2,    2,    1,    1,    1,    2,
    2,    2,    2,    0,    3,    5,    1,    1,    2,    3,
    1,    2,    3,    0,    1,    1,    1,    5,    5,    6,
    1,    1,    1,    2,    4,    1,    0,    5,    1,    0,
    3,    2,    1,    0,    2,    0,    1,    0,    3,    1,
    0,    1,    2,    1,    1,    1,    1,    2,    2,    1,
    2,    3,    1,    1,    3,    3,    1,    2,    3,    1,
    2,    0,    2,    1,    1,    0,    1,    1,    1,    3,
    2,    3,    0,    1,    3,    4,    4,    1,    3,    4,
    3,    0,    1,    3,    3,    1,    1,    0,    1,    1,
    4,    0,    3,    0,    2,    0,    2,    0,    9,   10,
    0,    0,    0,    0,    3,    2,    1,    0,    3,    5,
    3,    2,    5,    2,    1,    1,    1,    1,    1,    3,
    1,    3,    1,    0,    2,    1,    3,    1,    1,    1,
    6,    1,    0,    3,    0,    1,    3,    6,    3,    4,
    2,    3,    0,    1,    3,    1,    1,    1,    2,    1,
    1,    1,    2,    1,    2,    1,    2,    0,    3,    0,
    1,    3,    1,    2,    0,    2,    0,    2,    0,    4,
    1,    2,    1,    1,    0,    1,    3,    2,    1,    1,
    2,    1,    4,    3,    1,    3,    4,    0,    8,    5,
    1,    3,    1,    1,    4,    4,    4,    1,    1,    5,
    5,    1,    3,    3,    1,    1,    7,    7,    5,    3,
    0,    1,    1,    1,    2,    0,    1,    1,    1,    0,
    3,    1,    3,    1,    0,    3,    1,    3,    1,    3,
    3,    1,    1,    3,    1,    2,    1,    3,    3,    1,
    2,    3,    3,    3,    3,    1,    3,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    3,    3,
    3,    3,    3,    3,    3,    3,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    1,    1,    5,    6,    3,    4,    5,    6,
    3,    4,    3,    4,    3,    4,    4,    5,    4,    4,
    3,    4,    1,    1,    1,    1,    1,    1,    1,    3,
    3,   10,   10,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    3,    3,    3,    3,    3,    3,
    3,    3,    1,    1,    1,    1,    3,    1,    1,    1,
    1,    1,    4,    1,    3,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    2,    2,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    1,    2,
    1,    1,    1,    1,    1,    2,    1,    1,    1,    1,
    1,    4,    5,    5,    5,    5,    5,    5,    5,    5,
    5,    5,    6,    1,    6,    1,    4,    7,    2,    0,
    4,    3,    6,    1,    1,    6,    6,    1,    1,    4,
    6,    4,    5,    3,    5,    4,    5,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    0,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  555,
  556,  557,  561,  562,  563,    0,  618,  733,  734,  738,
  739,  463,    0,    0,    0,    0,    0,    0,  401,  573,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  911,    0,   21,  909,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  481,    0,    0,   37,   34,
   36,    0,   35,    0,  996,  998, 1000,  999, 1005, 1001,
 1008, 1006, 1002, 1004, 1007, 1003, 1009, 1010, 1011, 1012,
 1013,    0,    0,    0,    0,   32,   68,   66,  988,  997,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  647,    0,    0,    0,  688,    0,  990,
    0,    0,    0,    0,  107,  995,  564,    0,    2,    0,
    0,    0,    0,  431,  432,  433,  434,  435,  436,  437,
  438,  439,  440,  441,  442,  443,  444,  445,  446,  447,
  448,  449,  450,  451,  452,    0,  429,  430,    0,  980,
    0,  408,  984,    0,  405,  987,    0,  690,    0,  404,
  991,    0,  571,  907,    0,  150,  103,    0,  104,    0,
  218,  976,  772,    0,  102,  145,  983,    0,    0,   98,
    0,  105,  913,    0,   99,    0,  100,    0,  101,  994,
  146,  106,  626,  627,    0,  912,  910,    0,   74,    0,
  981,    0,  979,  489,  482,    0,  488,  493,  483,  484,
  491,  485,  486,  487,  492,    0,   30,    0,   39,  759,
   38,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  917,    0,  898,  641,
  641,  642,  652,  654,  969,  655,    0,  656,  646,  642,
    0,    0,    0,    0,    0,    0,    0,  590,  579,    0,
  582,  584,  585,  586,  587,  593,  594,    0,  989,  689,
    0,  108,    0,  109,    0,  110,    0,    0,    0,  619,
    0,    0,    0,    0,    0,    0,    0,    0,  464,    0,
    0,  371,    0,  221,    0,  411,  402,  461,  462,    0,
    0,    0,    0,    0,    0,  420,    0,  377,  378,    0,
  577,  575,  908,    0,    0,    0,    0,    0,    0,    0,
  153,  132,    0,   96,    0,    0,  111,    0,  121,  120,
    0,    0,    0,    0,    0,    0,  972,   75,    0,    0,
    0,  735,  736,  490,    0,    0,    0,    0,  876,  904,
    0,  971,    0,  878,    0,    0,  905,    0,  899,  890,
  889,  888,    0,    0,    0,    0,  880,  881,  882,  891,
  892,  900,  901,    0,  902,  903,    0,    0,    0,  906,
    0,    0,    0,    0,    0,  894,    0,  769,  873,  874,
  875,  686,  879,  353,  859,    0,    0,    0,  858,    0,
  684,  855,  856,  857,  860,  861,  862,  893,  918,  919,
  920,  921,  934,  936,  944,  945,  948,  949,    0,  762,
    0,    0,   27,    0,    0,    0,   71,   69,  992,   67,
   33,    0,    0,    0,  764,    0,   52,    0,    0,    0,
    0,    0,  569,    0,    0,    0,  649,  651,    0,  658,
    0,  746,    0,    0,  659,    0,  645,    0,  616,    0,
  591,    0,  588,    0,  589,  614,    0,  609,    0,    0,
  583,  773,  742,    0,    0,    0,    0,    0,    0,  566,
  565,    0,    0,  623,  620,    0,    0,  617,  186,    0,
    0,    0,  129,  469,  173,  174,    0,  472,  467,    0,
  473,    0,    0,    0,    0,  474,    0,  465,    0,    0,
  415,  417,  416,    0,  418,    0,  412,  219,    0,    0,
  272,    0,    0,    0,    0,  424,  425,    0,  454,    0,
    0,    0,    0,    0,    0,    0,  382,  381,  477,    0,
    0,  149,    0,    0,  158,    0,  157,    0,    0,  164,
  147,    0,  162,  165,  166,  154,    0,    0,  512,  547,
  515,    0,  548,    0,    0,  516,    0,    0,    0,  539,
  551,  511,  534,  513,  514,  509,  496,  371,  494,  495,
  497,    0,    0,  506,  507,  508,  510,    0,    0,    0,
    0,    0,  112,    0,  113,    0,  124,    0,    0,  202,
    0,    0,  369,    0,  221,    0,    0,   83,    0,    0,
  754,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  775,  777,  786,  789,  790,  791,  792,  793,
  794,  795,  796,  797,  798,    0,    0,    0,  895,    0,
    0,    0,    0,    0,    0,  896,  897,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  325,  993,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  761,   28,    0,    0,   44,   72,    0,
    0,   26,    0,  767,    0,    0,    0,  730,    0,    0,
    0,    0,    0,  568,    0,  641,    0,    0,    0,    0,
  663,    0,  559,    0,  915,  558,    0,  592,    0,  600,
  596,  595,    0,    0,    0,  611,  599,    0,    0,  740,
  741,    0,    0,    0,  369,    0,    0,    0,  633,  637,
    0,    0,    0,  130,    0,  471,    0,    0,    0,    0,
    0,  371,  414,    0,  371,  372,  419,  504,    0,  274,
    0,  268,  270,  369,  241,  977,    0,    0,  371,    0,
    0,  210,  369,    0,    0,    0,    0,  246,  240,  243,
  244,  245,  423,  459,  458,  422,  421,    0,    0,    0,
    0,    0,    0,  380,    0,  371,    0,  155,  159,    0,
    0,  163,    0,    0,  535,  536,  552,    0,  549,    0,
    0,  542,  540,  541,    0,    0,    0,    0,  505,    0,
    0,    0,    0,  543,    0,    0,  369,    0,  201,    0,
  204,  206,  207,    0,    0,    0,    0,    0,  221,   78,
    0,    0,   77,  523,  973,  524,    0,    0,    0,    0,
  776,  788,    0,    0,  846,  847,  848,    0,  780,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  961,  962,  963,  964,  965,  966,  967,  968,
    0,    0,    0,    0,    0,    0,  954,    0,    0,    0,
    0,  871,  327,  872,  942,    0,    0,  867,  975,    0,
    0,    0,    0,    0,  685,    0,  877,  771,  770,  763,
    0,    0,    0,    0,    0,    0,    0,    0,   53,   61,
   63,    0,    0,   70,    0,    0,    0,  766,    0,    0,
    0,    0,    0,    0,    0,    0,  650,  653,    0,    0,
  670,    0,    0,  668,  669,    0,    0,  660,  985,  662,
  560,  916,  601,  610,  605,  604,    0,  615,  774,  743,
  744,  369,    0,    0,  643,  625,  624,    0,  182,  176,
  183,    0,  179,  470,    0,    0,  178,    0,  189,  170,
  169,  914,  172,  171,  475,    0,    0,    0,  413,  273,
    0,  212,  371,  214,    0,  372,  209,  215,    0,    0,
    0,    0,  426,  427,  371,  369,  372,    0,    0,    0,
  398,    0,  197,  198,  156,    0,  168,    0,    0,    0,
    0,    0,  550,    0,    0,  371,  372,    0,  500,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  199,    0,    0,    0,    0,  357,    0,   82,
    0,    0,    0,    0,   87,   84,    0,    0,    0,  737,
  978,    0,    0,  781,    0,    0,  787,    0,    0,    0,
    0,    0,  824,  823,    0,    0,    0,  831,  849,  841,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  779,    0,    0,    0,  221,    0,  922,    0,    0,  937,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  950,    0,  960,    0,    0,    0,    0,    0,
    0,  941,  883,   65,   57,   58,   64,   59,   60,    0,
    0,   22,    0,    0,    0,   42,    0,    0,    0,  748,
  768,    0,  731,    0,  747,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  691,    0,    0,    0,
    0,    0,  607,  608,  612,    0,  200,    0,    0,    0,
    0,    0,    0,    0,    0,  190,    0,    0,    0,  409,
  224,  225,  226,  227,  228,  229,  886,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  455,
  456,  428,    0,    0,    0,    0,    0,    0,  371,  479,
  372,  167,  974,    0,    0,  518,    0,  517,  553,    0,
  545,    0,    0,    0,  498,    0,  526,    0,    0,  528,
  531,  533,  369,    0,    0,    0,  119,    0,  205,  371,
    0,    0,  370,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  839,  840,  783,    0,  785,
    0,    0,    0,    0,    0,    0,    0,  842,    0,    0,
    0,    0,    0,    0,    0,  832,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  928,  927,    0,    0,  924,  923,    0,  932,  931,  930,
  929,  926,  925,    0,    0,  956,    0,    0,    0,  955,
    0,    0,    0,    0,    0,    0,   56,   62,   54,   55,
   23,  752,  753,    0,  750,  757,  758,  755,    0,    0,
  729,    0,    0,    0,  679,    0,    0,    0,  697,  693,
  695,    0,    0,    0,    0,    0,  370,  629,  630,  644,
    0,  184,  180,    0,    0,  195,  191,    0,  372,  887,
  280,    0,    0,  372,  278,    0,    0,    0,  232,    0,
    0,  248,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  476,  520,    0,  521,    0,  134,    0,    0,    0,
  138,  140,  141,  142,  501,  503,  499,  530,    0,    0,
  372,    0,    0,    0,    0,  354,    0,  358,  371,   88,
    0,    0,   90,    0,    0,    0,    0,    0,    0,  851,
  850,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
  943,  933,  935,    0,    0,  952,  951,    0,  957,  946,
  947,    0,   40,    0,    0,  732,    0,  682,  680,    0,
    0,    0,  701,    0,    0,  639,  372,  355,  631,    0,
  175,  192,  372,  144,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  285,  290,
  284,    0,    0,    0,  294,  295,  296,  297,  298,  299,
  301,  302,  304,  305,  306,  309,    0,    0,    0,  267,
    0,    0,  208,  233,    0,    0,    0,    0,  234,  237,
    0,    0,  372,  396,  397,  393,  392,  395,  394,    0,
  519,  546,  143,  131,  139,    0,  371,  264,  126,  372,
    0,    0,  373,  359,    0,    0,   91,   92,   79,    0,
  810,  818,    0,  811,  819,  814,  822,    0,    0,  812,
  820,  813,  821,  807,  815,  808,  816,  809,  817,    0,
  938,  953,    0,  749,    0,    0,  672,    0,    0,  711,
  713,    0,  708,    0,  265,  185,  410,    0,  312,    0,
    0,  315,  316,    0,  308,    0,    0,  307,  339,  340,
    0,  986,  293,  300,    0,  303,  286,    0,  291,    0,
  344,  292,    0,    0,  337,  211,  216,  235,    0,    0,
  238,    0,  250,  252,  400,  372,    0,  375,    0,    0,
    0,  364,  365,  366,    0,    0,   76,   94,    0,    0,
    0,    0,  671,    0,  702,  712,    0,    0,  283,    0,
  310,    0,    0,    0,    0,    0,  341,    0,  332,  333,
    0,  352,    0,    0,    0,    0,  347,  345,    0,    0,
    0,  239,    0,    0,    0,    0,    0,  266,  372,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  676,    0,
    0,  719,    0,    0,    0,  282,  279,  311,    0,    0,
  314,    0,  322,  324,    0,    0,  351,  350,  982,  349,
    0,    0,    0,    0,    0,    0,    0,    0,  249,    0,
  256,    0,  255,  376,  368,    0,    0,  360,  118,    0,
    0,    0,    0,  674,  721,  710,    0,    0,  722,    0,
  718,    0,    0,    0,    0,  335,  334,  346,  348,    0,
    0,    0,  236,    0,    0,  260,  259,  258,    0,    0,
  853,  640,  677,  717,    0,    0,    0,    0,  313,  319,
    0,    0,    0,    0,    0,  263,  261,  262,    0,    0,
    0,    0,    0,  724,    0,    0,    0,    0,    0,  336,
    0,    0,    0,  723,    0,  330,  320,    0,  342,  317,
  852,  726,    0,  318,  727,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1555,   28,   29, 1556,
   31,   32,   33,   34,   35,   36, 1557,   38, 1558,  651,
  122,  462,  967, 1202,  123,  717,  968,  124,  466,  722,
  148,  125,  126,  259,  990,  138,  969,  970,  199,  228,
  127,  971,  128,  468,   82,  239,  378,  425,  646, 1103,
  426,  872,  617,  221,  849,  647,  648,  649, 1105,  240,
 1392,   74,  189,   75,  235, 1851, 1315,  220,  225,  227,
  229,  215,  207,  209,  222,  232,  155,  312,  314,  316,
  367,  633,  635,  459,  260,  798,  550,  661, 1039,  371,
  638, 1091, 1307,  583,  745, 1065,  532,  533,  565,  363,
  618, 1077, 1459, 1583, 1250, 1460, 1461, 1462, 1463, 1464,
  662,  216,  208,  359,  591,  360,  361,  592,  593,  594,
  595, 1282, 1033,  535,  536, 1022, 1020,  537, 1027, 1021,
 1240, 1422, 1028, 1029, 1245,  373,  860,  861,  862,  863,
  800, 1046, 1437,  619,  802,  428,  865,  948,  559, 1364,
 1251,  429,  430,  431,  432,  433, 1438, 1439,  568, 1589,
 1745, 1268,  808,  809,  795,  810,  811,  812, 1746, 1747,
 1827,  341,  563, 1259, 1559,  195,  560,  561, 1260, 1261,
 1432, 1720, 1560, 1561, 1562, 1563, 1680, 1564, 1565, 1566,
 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1575, 1576,
  434, 1813, 1664, 1577, 1728, 1844, 1726,  435,  701,  436,
  437, 1578, 1110, 1681, 1736, 1737, 1778, 1096, 1313, 1476,
 1097, 1098,  287,  493, 1612,  382, 1232, 1415, 1700, 1701,
 1702, 1703,  991, 1230,  350,  579,  826, 1280,  580,  824,
   79,   59,  345,  200,  163,  328,  186,  335,  192,  786,
  783,  336,  346,  816,  347,  575, 1057,  187,  438,  329,
  547,  581, 1281,   96,  198,  244,  620,  621,  788, 1078,
  789, 1079, 1080,  624,  625,  626,  627, 1071,  876,  628,
  629,  630,  842,  631,  839,   40,   41,   42,   43,   44,
   45,  157,  521,   61,  484,  203,  352,  299,  300,  301,
  302,  303,  304,  305,  306,  307,  751,  507, 1007, 1235,
  508,  509,  756,   46,  162,  322,  528,   47,  523,  524,
 1016, 1420, 1239, 1539,  769,  142,  739, 1536,  728,  487,
  289,  143,  144,  439,  288,  494,  495,  496,  742, 1000,
  992,  993,  994,  995, 1646, 1713,  440, 1760, 1224, 1185,
 1532, 1533, 1762, 1654, 1763, 1764, 1765, 1811, 1852, 1854,
  474, 1214,   48,   49,  383,   50,   51,  515,  516,  976,
 1210, 1394, 1398,  475,  725,  213,  663,  664,  890,  891,
  665,  666,  667,  668,  669,  670,  671,  672,  673,  674,
  675, 1336, 1126, 1128, 1129,  947,  700,  442,  443,  444,
  445,  446,  447,  448,  237,  449,  450,  451,  452,  453,
  931,  454, 1515,  455,  456,  457,  458,  691,  935, 1186,
 1181,  692,  130,
};
static short yysindex[] = {                                   5269,
 1175, -196, 3948,  169, 1289,  -29, 4289, -105, 9772,  714,
   48, 8656, -196, 1392,  877, 6332, 1628, 6332,   -1,    0,
  458,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  303,    0,    0,    0,    0,
    0,    0, 9399, 6332, 6332, 6332, 6332, 6332,    0,    0,
   18, 1449, 6332, 6332, 6332,  403, 6332,  272, 6332, 6332,
    0, 6332, 6332,    0,  906,    0,  314,    0,    0,  565,
 6332,    0, 6332, 6332, 6332,  594, 6332, 6332, 6332, 6332,
  272, 6332, 6332, 6332, 6332,    0, 6332,  544,    0,    0,
    0,  926,    0,  926,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  644,  665,  214, 1051,    0,    0,    0,    0,    0,
 6332, 6332, 6332,  772,  798,  854,  141, 1923,  855,  126,
  189,  548,  807,    0, 6332,  998, 1379,    0,  939,    0,
 6332, 6332, 6332, 6332,    0,    0,    0, 6332,    0, 1131,
   16, 1097,  604,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1057,    0,    0, 1449,    0,
   52,    0,    0, 1399,    0,    0, 1556,    0,  713,    0,
    0, 1059,    0,    0, 1024,    0,    0, 1245,    0, 1235,
    0,    0,    0, 1449,    0,    0,    0,  722, 1399,    0,
  625,    0,    0, 1294,    0, 1301,    0,  926,    0,    0,
    0,    0,    0,    0, 1313,    0,    0, 1171,    0, 1337,
    0, 1254,    0,    0,    0, 1171,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 9089,    0, 6332,    0,    0,
    0, 5738, 1293, 6041, 6332, 1037, 1451, 1780, 1480, 1504,
 5738, 1456, 6041, 1477, 1489, 1370,    0,10763,    0,    0,
    0,    0,    0,    0,    0,    0, 8437,    0,    0,    0,
 1488, 6332, 1537, 1454,   -3, 6332, 1492,    0,    0, 1379,
    0,    0,    0,    0,    0,    0,    0, 6332,    0,    0,
 1399,    0, 1294,    0,  926,    0, 1551,10763, 1523,    0,
 1510, 1496,  -75, 1429,  -17, 1440, 1107,  604,    0, 1667,
   82,    0, 6332,    0, 1057,    0,    0,    0,    0, 6332,
 1550, 4379, 1507, 4899, 1941,    0, 9399,    0,    0, 1238,
    0,    0,    0, 1956, 1956, 1449, 1618, 1449, 3322, 1245,
    0,    0,11503,    0, 6332, 6332,    0, 1550,    0,    0,
 1732,  547, 1986, 6332, 1763, 6332,    0,    0, 1993,  202,
 6102,    0,    0,    0, 1995, 1997, 1999, 1567,    0,    0,
 2009,    0, 2015,    0, 2017, 2021,    0, 2023,    0,    0,
    0,    0,    0, 2028, 1615, 1617,    0,    0,    0,    0,
    0,    0,    0, 2052,    0,    0, 9319, 2064, 2066,    0,
10763,10763, 8517, 6332, 2079,    0, 1702,    0,    0,    0,
    0,    0,    0,    0,    0, 2076, 2035,    0,    0, 2082,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1508,    0,
 6332, 1703,    0, 6332, 6332,   97,    0,    0,    0,    0,
    0, 6332, 6332,  751,    0, 6332,    0, 1824, 6332, 2102,
 5738, 1746,    0, 6332, 9567, 2100,    0,    0,10763,    0,
 1702,    0, 1838, 2106,    0, 1901,    0,   87,    0, 1193,
    0,  475,    0,  475,    0,    0, 2126,    0,  156, 1751,
    0,    0,    0, 2134,  123, 2125, 1550, 2148, 1926,    0,
    0, 2022, 2169,    0,    0, 1832, 1712,    0,    0,  157,
 1871, 1873,    0,    0,    0,    0, 1449,    0,    0, 1875,
    0, 1956, 1956, 1956, 1956,    0, 2181,    0, 1954, 1955,
    0,    0,    0, 1934,    0,11503,    0,    0,11503, 1663,
    0, 1399, 1971, 6332,12836,    0,    0,  -83,    0, 6332,
  916, 1556, 1841, 1803, 1773,    0,    0,    0,    0,  456,
 1851,    0, 1792,  272,    0, 1449,    0, 1979, 1910,    0,
    0, 3322,    0,    0,    0,    0,  159, 1812,    0,    0,
    0, 1813,    0, 1865, 2228,    0, 1927, 2232,  640,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2184,  160,    0,    0,    0,    0,  178, 2237, 2238,
 2232, 1399,    0, 1964,    0, 2024,    0, 1449, 1449,    0,
 4379,  713,    0, 1902,    0, 1878, 2250,    0,10618, 2602,
    0, 1906, 1914, 2258, 6396, 2259,    0,    0,    0, 5848,
 6683,   42,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  742, 9647,  816,    0, 6332,
10763,  933,  963, 1135, 1988,    0,    0,10763, 6644, 1702,
 1850,  831,10763, 9647,  927,  927, 1392, 2121, 1758, 2263,
    0,    0, 9886, 6332,10763,10763,10763,10763,10763, 9647,
10763, 2409, 6332,    0,    0, 6872, 1869,    0,    0, 1844,
 6041,    0, 1992,    0, 1800, 1392, 2270,    0, 1992, 6872,
 1991, 2001, 6332,    0, 2121,    0, 2369, 2039, 1860, 9647,
    0, 6332,    0, 1080,    0,    0,   51,    0, 1867,    0,
    0,    0, 6332,  611, 6332,    0,    0, 6332, 6332,    0,
    0, 9647, 2049, 4379,    0, 1815,10763, 6332,    0,    0,
 1449, 1449, 1449,    0,  553,    0, 1792, 1792, 1861,  272,
 1107,    0,    0, 2286,    0,    0,    0,    0, 2051,    0,
 6332,    0,    0,    0,    0,    0, 2055, 2291,    0, 2034,
 2065,    0,    0, 2006, 2012,  926, 2058,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  272, 6332,12882,
 1975, 1976, 1977,    0,  272,    0, 1606,    0,    0, 1939,
 1449,    0,   96, 1950,    0,    0,    0,  272,    0, 2228,
  170,    0,    0,    0, 2067,  170, 1952,  170,    0, 2328,
  272,  272,  272,    0, 1550, 6332,    0, 2016,    0, 1830,
    0,    0,    0, 1238, 1958, 6332, 1929,  142,    0,    0,
 2331, 2099,    0,    0,    0,    0,  202, 6332, 1965, 6730,
    0,    0, 1965, 6396,    0,    0,    0, 6436,    0,  406,
  491,10763,10763, 6988, 2341,  727, 7140,10763, 7249,  979,
 7508, 7660,10125, 7769, 8028, 8180, 6644, 6644,10763,10763,
 2119,10763, 2343,10763, 2347, 2385,10763,10763,10763,10763,
10763,10763,    0,    0,    0,    0,    0,    0,    0,    0,
 2073, 1020,  505,10763,  948, 9647,    0, 6644, 1684, 2350,
  548,    0,    0,    0,    0, 1702, 1931,    0,    0, 2122,
  927,  927,  988,  988,    0, 1073,    0,    0,    0,    0,
 6332, 6332, 6332, 6332, 6332, 6332,  180, 2361,    0,    0,
    0, 6872, 2030,    0,  493, 2040, 6332,    0,  548, 9647,
 2040, 2364, 2361, 6872, 5596, 2097,    0,    0,  956, 2389,
    0, 2365, 1273,    0,    0, 6644, 2090,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -117,    0,    0,    0,
    0,    0, 1933, 1958,    0,    0,    0, 2387,    0,    0,
    0, 1940,    0,    0, 1956, 2167,    0,  553,    0,    0,
    0,    0,    0,    0,    0, 2395, 6644, 3256,    0,    0,
 2137,    0,    0,    0, 3256,    0,    0,    0,  926,  926,
 1944, 2407,    0,    0,    0,    0,    0, -163,  -35, 1361,
    0, 2185,    0,    0,    0, 6332,    0,  169, 2045, 2026,
 2051, 2411,    0, 1792, 2046,    0,    0, 1197,    0, 2398,
 6332, 1383,  272, 2417, 2429, 2437, 2216,  713, 2137, 1956,
 1873, 4379,    0, 1851,  548, 2027, 2042,    0, 1843,    0,
  272, 2091, 2175,10618,    0,    0,  272, 1994, 2182,    0,
    0, 2451, 2452,    0, 6644, 6644,    0, 6644, 6644, 2399,
 1702, 2454,    0,    0, 1702, 2455,10205,    0,    0,    0,
 2144, 2461, 1702, 2463, 1946, 2464, 1702, 2465,10763,10763,
 2341,10763,10444, 2466, 1702, 2472, 2474, 1702, 2481,10763,
 1702, 2482, 1702, 2483, 2486, 1702, 2487, 2492, 1702, 2493,
    0, 2272, 2412, 2548,    0, 2586,    0, 2734,10763,    0,
 2774, 2810, 2886, 2936, 3083, 3110,10763,10763, 9647, 1702,
 2101, 9647,    0,10763,    0, 2227,  606,10763, 9647,10763,
10763,    0,    0,    0,    0,    0,    0,    0,    0, 2218,
 6872,    0, 6872,  180, 2361,    0,  251,  251,  207,    0,
    0, 8437,    0, 2094,    0, 2364, 2361, 2499, 5596,  196,
 1273, 2496, 9647, 1901, 2039, 2164,    0, 2165, 2166, 2203,
 2283, 2230,    0,    0,    0, 2137,    0, 2027,  610, 2136,
 1449, 2174, 1792, 2176, 1792,    0, 6644,  432, 1696,    0,
    0,    0,    0,    0,    0,    0,    0, 2120, 2294, 2137,
    0,10763,    0, 2281, 2170, 1944,  906,    0, 6644,    0,
    0,    0, 2190, 2197, 2198, 2199, 2206, 2208,    0,    0,
    0,    0,    0, 2556,  169,    0, 1861,    0,    0,  170,
    0, 3256, 2220,  170,    0,  170,    0, 2051, 2560,    0,
    0,    0,    0, 1238, 2294, 1792,    0, 1873,    0,    0,
 8437, 2324,    0,   81,    0, 2387,    0, 2565,    0, 2567,
 1449,  -28, 2568,    0, 1449,    0,    0,    0, 2272,    0,
 2272,10763, 2189, 2189, 1392, 2570, 2096,    0, 2189, 2189,
10763, 2189, 2189, 3199, 1702,    0, 2089,10763, 1702, 2189,
 2189, 2189, 2189, 1702, 2189, 2189, 2189, 2189, 2189, 2189,
    0,    0,11503, 2571,    0,    0, 3117,    0,    0,    0,
    0,    0,    0, 3177, 1468,    0, 9647, 2306, 2180,    0,
 9647, 3191, 2116, 2038, 1702, 2233,    0,    0,    0,    0,
    0,    0,    0, 2183,    0,    0,    0,    0, 1838, 9647,
    0, 5596, 2499, 1901,    0, 2158, 6332, 1273,    0,    0,
    0, 2039,10763, 6644, 2225, 2294,    0,    0,    0,    0,
 1956,    0,    0, 1449, 1606,    0,    0,  621,    0,    0,
    0, 6332, 7383,    0,    0, 1702, 2344, 2281,    0, 1438,
 6332,    0, 2288,  635, 2188, 2285, 2192, 2325, 2297, 2335,
 2137,    0,    0, 2591,    0, 2597,    0, 2287, 2344, 2220,
    0,    0,    0,    0,    0,    0,    0,    0, 2137, 1851,
    0, 1606, 2185, 2321, 2260,    0, 1958,    0,    0,    0,
 2273,  -74,    0, 2051, 2275, 1702, 2607, 2611,  548,    0,
    0, 2614, 2615, 1702, 2616, 2618,10763,10763, 1702, 2619,
 2621, 2623, 2626, 2627, 2628, 2630, 2631, 2632, 2633,    0,
    0,    0,    0,10763, 2634,    0,    0, 9647,    0,    0,
    0, 1860,    0,  251, 1860,    0, 6332,    0,    0, 1306,
 1702, 2637,    0,   42,  630,    0,    0,    0,    0, 1792,
    0,    0,    0,    0,11503, 3326,  861, 2617, 2638, 2337,
10763, 2624, 2629, 3635, 2635, 2636, 2351, 2641,    0,    0,
    0, 2382,10345, 2642,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -13, 2645,    0,    0,
 2652, 6332,    0,    0, 2352, 2363, 6332, 1944,    0,    0,
  926, 6332,    0,    0,    0,    0,    0,    0,    0, 2294,
    0,    0,    0,    0,    0, 2294,    0,    0,    0,    0,
 2070, 1860,    0,    0,10763, 1449,    0,    0,    0, 1449,
    0,    0,10763,    0,    0,    0,    0, 1702, 1702,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1702,
    0,    0, 2090,    0, 2090, 2668,    0, 6644,10763,    0,
    0, 2346,    0, 2672,    0,    0,    0,  110,    0,10683,
 6332,    0,    0,10763,    0, 6644, 1702,    0,    0,    0,
 2657,    0,    0,    0, 3022,    0,    0,  687,    0, 2269,
    0,    0,  561, 2682,    0,    0,    0,    0, 1944,  926,
    0, 2340,    0,    0,    0,    0, 2185,    0, 1542, 2680,
 1273,    0,    0,    0, 2090, 1702,    0,    0, 2038, 2230,
 2230, 6332,    0,   42,    0,    0, 5784, 3256,    0, 2667,
    0, 2430, 9089, 1673,  656, 2676,    0, 1142,    0,    0,
 2681,    0, 6332, 6332,  169,  140,    0,    0,  556, 2396,
 6644,    0, 2340,   41, 1944, 2356, 2358,    0,    0, 1273,
 2701, 2070, 2400, 2230, 2703, 1860, 2225, 2225,    0, 2159,
 6332,    0, 2704, 2706, 1022,    0,    0,    0, 2342, 3022,
    0, 2320,    0,    0, 3022, 2445,    0,    0,    0,    0,
 7383,  687,10763, 3022,  658, 1944, 1139, 1139,    0, 2458,
    0, 2318,    0,    0,    0, 1273, 2070,    0,    0, 2090,
 2376, 2376, 6332,    0,    0,    0, 5784, 2727,    0, 6332,
    0, 2008, 2709, 1691, 7383,    0,    0,    0,    0, 1876,
  187, 2467,    0, 2307,  580,    0,    0,    0, 1358, 2230,
    0,    0,    0,    0, 6332, 2432, 3022, 2728,    0,    0,
 2477, 3022, 2480, 2485, 7383,    0,    0,    0, 6644, 2225,
 2742, 2746, 2750,    0, 2160, 7383,  182, 6332, 7383,    0,
   42, 2376, 6332,    0, 6332,    0,    0, 7383,    0,    0,
    0,    0, 2756,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  109,    9,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  106, 1613,    0,    0,    0,    0,    0,    0,
 2799,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  212,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1191,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2469,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2425,    0,    0,
    0,  339,    0,  339,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2428,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1243,    0,
    0, 8756, 6166,    0,    0,    0, 1259,    0,    0,    0,
  481,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  131,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -32,    0,    0,    0,    0,  147,    0,
    0, 1280,    0,    0,    0,    0,    0,  248,    0,13078,
    0,    0,    0,    0,    0,    0,    0,    0,  -32,    0,
  465,    0,    0, 2760,    0,    0,    0, 2550,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1292,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1317,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1149,    0,    0,
    0,    0,    0,    0,    0, 6484,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  429,    0,    0, 1366,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -32,    0, 2760,    0, 2550,    0, 1390,    0,    0,    0,
    0,  366,    0,    0,    0,    0,    0, 1412,    0, 2534,
    0,    0,    0,    0, 1422,    0,    0,    0,    0,    0,
 2558,   -6,12986,    0, 1447,    0,    0,    0,    0,  384,
    0,    0,    0, 2362, 2362,    0,    0,    0, 1462,  567,
    0,    0,    0,    0,    0,    0,    0, 2558,    0,    0,
 1548,    0,    0,    0,    0,    0,    0,    0,10060, 2434,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1887,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2687,    0,    0,    0, 2991, 3295,    0,
    0,    0,    0,    0,    0,    0,  848,    0,    0,    0,
    0,    0,    0,    0,    0,  133, 3599, 2378,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1484,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1531,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  642,    0,    0, 2504,    0, 2293,    0,    0,    0,    0,
    0,  337,    0,  337,    0,    0,  534,    0, 1536, 1683,
    0,    0,    0,    0, 1292,    0, 2558,    0,    0,    0,
    0,  128,  186,    0,    0,  183,    0,    0,    0,  689,
    0,  827,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2362, 2362, 2362, 2362,    0,  905,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2562,    0,    0,    0,    0,    0, 2534,    0,    0,
 1234,    0, 2357,    0,    0,  400,    0,    0,    0,    0,
  216,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1540,    0,    0,    0,    0, 2982, 4256,    0,    0,
    0, 4776,    0,    0, 8309,    0,    0,11270,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1126, 2563,    0,    0,    0,    0,11282, 6991,    0,
11270,  -32,    0,    0,    0,    0,    0,    0,    0,    0,
   -6, 1348,    0,    0,    0,    0, 2420,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11551,11683,11858,    0,
    0, 1437,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,11002,    0,11002,    0,    0,
    0,11002,11002,11002,    0,    0,    0,    0,    0, 2383,
    0,    0,    0,    0, 4639, 4943, 9003, 2791,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  302,    0,    0, 1613,    0,    0,  302,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1575,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -111,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   -6,    0,  219,    0,    0,    0,    0,
    0,    0,    0,    0,  946,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3588,    0,
    0,    0,    0,    0,    0,    0, 2796,    0,    0, 3928,
 2965,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  399,  618,  715,    0,    0,    0,  121,    0,    0,    0,
    0,    0,    0, 3169,    0,    0,    0,    0,    0, 8309,
    0,    0,    0,    0,  819,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2558,    0,    0,  363,    0,    0,
    0,    0,    0,    0,    0,    0,    0,11093,    0,    0,
    0,  245,    0,    0,    0,    0, 2528,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
11002,    0,    0,    0,    0, 1530,    0,    0,    0, 3903,
 5159, 5463, 4119, 4423,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1553, 1586,    0,    0,
    0,    0,    0,    0,    0, 1589,    0,    0, 8756,    0,
 1589, 1592, 1600,    0,    0,    0,    0,    0,    0,  844,
    0, 1228, 1174,    0,    0,    0, 1189,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  198,    0,    0,
    0,    0,    0,    0, 2362, 2381,    0, 1071,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2569,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1491,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1622,    0,    0,    0,    0,    0,    0,    0,    0,
 3286,    0,    0,    0,    0,    0,    0,    0,    0, 1416,
    0,    0,    0,    0,    0,    0,    0, 1348, 2569, 2362,
 1635,   -6,    0, 2578, 8756, 1637,  129,    0,    0,    0,
    0,11093,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 5296,    0,    0,    0, 9083,    0,    0,    0,    0,    0,
    0,    0,11374,    0,11829,    0,11935,    0,    0,    0,
    0,    0,    0,    0,11995,    0,    0,12067,    0,    0,
12151,    0,12180,    0,    0,12214,    0,    0,12300,    0,
    0, 1326,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2410,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1553, 1642,    0, 2408,  318,    0,    0,
    0,    0,    0,    0,    0, 1675, 1738, 1760,    0,    0,
 2503,  989,    0, 5678,    0, 2505,    0, 2506, 2511,    0,
    0, 1224,    0,    0,    0, 2569,    0, 1637,    0,  890,
    0,    0,    0, 2401,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6836,    0, 2585,
    0,    0,    0, 2196,    0, 1491, 2469,    0,    0,    0,
    0,    0,  740,  825,  842,  900,  965,  971,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  843,    0,    0,    0,    0, 3870,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1764,    0,    0,
    0,    0,    0,    0,    0, 1776,    0,    0,    0,    0,
    0,  348,    0,    0,    0,    0,    0,    0,  135,    0,
  289,    0,    0,    0, 9003,    0,    0,    0,    0,    0,
    0,    0,    0,    0,12320,    0,12380,    0,12416,    0,
    0,    0,    0,12500,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2817,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1605,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1789, 5543,    0,    0,    0, 1503,    0,    0,
    0,    0,    0,    0,  360,    0,    0,    0,    0,    0,
 2362,    0,    0,    0,  241,    0,    0,    0,    0,    0,
    0,    0,  563,    0,    0, 2819, 1710, 2210,    0, 2534,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2569,    0,    0,    0,    0,    0,    0,    0, 1793,  849,
    0,    0,    0,    0,    0,    0,    0,    0, 2569, 2578,
    0,  371,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  349,    0,12536,    0,    0,11002,    0,
    0,    0,    0,12630,    0,    0,    0,    0,12702,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  672,    0,  318,  991,    0, 4264,    0,    0, 2503,
  532, 1866,    0, 1441, 2821,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1807, 1859,    0,    0,    0,    0,    0,    0,
    0,    0,  675,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1905,    0,
    0,    0,    0,    0,    0,    0,    0, 4046,    0,    0,
  114,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  154,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12736,12765,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2823,
    0,    0,  435,    0, 1034, 4784,    0,    0,    0,    0,
    0, 2822,    0,    0,    0,    0,    0, 2806,    0,    0,
    0,    0,    0,    0,    0,    0, 2813,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2566,
    0,    0,    0,    0,    0,    0,    0,    0, 4046,  179,
    0,  115,    0,    0,    0,    0,    0,    0,    0,  191,
  353,    0,    0,    0,  234, 2819,    0,    0,    0,  257,
  329,    0,    0, 5307,    0,    0, 2821,    0,    0,    0,
    0,    0,  101,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  332,    0, 1491,  994, 1033,    0,    0, 2503,
 1579,    0,    0,  195,    0,  672,  120,  344,    0,    0,
 1514,    0,    0, 2832,    0,    0,    0,    0, 2816,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   60,    0,    0,    0,    0, 4046,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1442,    0,    0,    0,  435,
 2835, 1173,    0,    0,    0,    0, 2821,    0,    0,    0,
    0,    0,    0,    0,   60,    0,    0,    0,    0,    0,
 2580,    0,    0,    0,    0,    0,    0,    0, 2503,  257,
    0,    0,    0,    0,    0, 2168,    0, 2833,    0,    0,
 9226,    0,    0,    0,   60,    0,    0,    0,    0,  120,
 2841,    0,    0,    0,    0,   60,    0,    0,   60,    0,
 1220, 2835,    0,    0,    0,    0,    0,   60,    0,    0,
    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2891,    0,    0, 2893,
    0,    0,    0,    0,    0,    0,    4,    0, 2896,    1,
  221,  -68, -610, 1693,  368, -329,  237, 2888, 2640,    0,
  -27,    0, 2649,  -93,  -40,    0,  228, -893,  -50,  -61,
  -22, -175, 2639, 2186,    0,    0,    0, 1594,    0,    0,
  -36, 1798, -541,    8, -955, 2261, 2047, -299, 1801, 2836,
 -278,    0,  -56,    0, -483,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -620,  -38, 1823, -291, 1102, -876,    0,
    0,    0,    0, -334, -123, -511, 1833, -327,  -48,    0,
 -528,    0,    0, 1471, -980,    0, 1472,    0,    0, 2606,
 -623,    7,    0,    0,    0,    0, 2579,    0, 2348, 2620,
 -193, 1857, -706,    0,    0,    0, -710,    0,    0,    0,
    0,    0,    0, 1930,    0, 2647, 2178, 1872, 2643, 2644,
    0, 1308,    0, -525, 2172,  -41, -664, 1386,    0,    0,
    0, -942, -941, -938, -275, -928,    0, 1533,-1043,    0,
 1226,-1179, 1534,    0, 2406,    0,    0,    0, 1233, 1241,
 1196,  130, -249, -391, -551,    0,    0, 2201,    0, 1728,
    0,    0, -287,    0,    0,    0,    0, -146,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1267,    0, 1257,    0,  -79,    0,    0,   55,    0, -385,
  100, 1439, 1141, 1320,    0, 1215,    0, 1987, 1765, 1585,
    0, -988, -888, -510,    0, -496, -463, -380,    0, -553,
 1311,  990, -686, -501, -573, -726, -937,-1277,    0,    0,
    3,    0,    0,    0,    0,    0, 2665,    0,    0,    0,
    0, 2678, 2444,    0,    0,    0,    0,    0,   -9, 2689,
    0,    0,    0,    0,  -15,    0,    0,    0, -340, 2173,
 -335, 1731, -473,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2413,    0, 2191,    0,    0,    0,    0,    0,
    0,   44,    0, 3016,    0,    0,    0,    0,    0, 2739,
    0,    0,    0,    0, 2540,    0, 2538,    0,    0,    0,
 2292,    0,    0, -930, -762,    0,    0,   79,    0, 2289,
    0,    0,    0,    0,    0, -615, -968, -580,  978, -168,
 1573,    0, 2905, -127, 1509,    0, 2315, -625, -996,-1451,
    0, -838, 2068,    0,    0,    0, -145,    0,    0, -246,
    0, 1409, 1526,    0, 1261,    0, 1303,    0, -412,    0,
    0,    0,    0,    0, 2554,    0,    0,    0, -406, 2349,
 2095,-1062,    0,    0,    0, -688, -461,  441,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2722, 1272, 1936,    0, -547,    0,    0,    0, -920,
    0,    0,    0, 1822,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -241,
 1888,    0,    0,
};
#define YYTABLESIZE 13575
static short yytable[] = {                                     129,
  151,  191,  129,   37,   78,  310,  150,  214,  156,  441,
  261,  492,  281,  284,  210,  194,  280,  283,  760,  226,
  584,  211,  622,  959,  746,  206,  219,  623,  245,  197,
  787,  590,  254,  790,  556,  249,  799,  699,  766,  801,
  554,  224,  253,  188,  190,  193,  196,  150,  201,  250,
  231,  911, 1190,  212,  190,  217,  256,  196, 1220,  150,
  201, 1023,  230,  129, 1263,  933,  724,  151,  864, 1009,
  223,  241,  255,  150,  243,  190,  252,  241,  217,  193,
  196,  941,  150,  201,  230,  129, 1442,  196,  467,  267,
 1212,  269,  315,  223,  248, 1252, 1253,  467,  251, 1254,
 1014,  652, 1252, 1253,  268,  574, 1254,  873,  574, 1256,
  979,  311,  488,  760,  257, 1288, 1256, 1257,  636,  982,
  196,  212,  150,  212, 1257,  313, 1069,  628,  356, 1041,
  634,  744,  326,  546,  375,  217, 1011, 1094, 1048, 1068,
  721,  309,  196,  150,  230, 1395,  379, 1018,  156,  731,
 1221,  291,  337,  707,  760,  257, 1310,  760,  257,  328,
  709,  513,  514,  956,  574,  278,  759,  574,  628, 1264,
 1718,  628,  760,  257, 1273,  782,  441,  364,  760,  196,
 1272, 1102,  636, 1782,  265,  621,  628,  356,  279,  634,
  361,  326, 1089,  881,  705, 1610,  772,  635,  833,  755,
 1293,  317,  478,   77,  774,  379, 1311,  777,  778,  779,
  780,  622,  707,   77, 1074,  399,  460,  851,  632,  760,
 1440,  519,  760, 1201,  622, 1775,  621, 1407,  282,  623,
 1775,  269,  135,  700,  463,  804, 1404,  760,  499,  320,
  196,  636,  997,  463,  621, 1482,   77,  151,  212,  361,
  848,  279,  150,  705,  469,  129,  635,  265,   60,  632,
 1759,  150,  632,  469,  590, 1275,  512,  763,  506,  279,
  622,  242,  529,  115,  399,  319,  807,  632, 1274,  502,
 1617,  530,  193, 1161, 1162,   85,  150, 1233,   85,  719,
   83,  555,  700,  606, 1055,   77,   97,  705,  558,  196,
  211,  751,  571,  907, 1683,  573,  151, 1388,  338, 1390,
  115, 1457,  242,  279, 1187,  634,  805,  525,  834,  585,
  133,  587,  644,  212,  632, 1478,  845,  514,  705,  784,
  212,  257,  212,  502,  212,  640,  602,  576, 1234,  539,
 1477, 1787, 1467,  709,  606,  115,  642, 1236,  368, 1252,
 1253, 1833,  362, 1254, 1213,  196,  201,  806,  274,  709,
  751, 1204,  125, 1256,  150,  638,  193, 1075,  552,  705,
  196, 1257,  257, 1216, 1218,  257,  525,  338,  269,  136,
  503,  553,  760,  478,  709,  339, 1408,  705,   89,  527,
  257, 1271,  527,  242, 1440,  602,  362,  525,  385,  196,
  709,  492,  709,  986, 1452,  379, 1276, 1527, 1691, 1045,
  379,  362,  732, 1248,  702, 1522,  379, 1618,  709, 1749,
  196,  125, 1114,  718,  638,  771,  202,  908,  597,  196,
 1525,  512,   14,  715,  242,  504,  847, 1483,  718, 1684,
  517,  531,  478, 1781,  723,  628, 1117,  940,  729,  196,
 1843,  150,  505,  463,  196,  469,  760,  385,  760,  827,
  273, 1644,  150,  955,  492,  760,  150,  628,  196,  196,
  634,  150, 1429,  754,  156,  700,  196,  196, 1316,  399,
  690,  707, 1788,  196,  707, 1868,  492,  597, 1614,  504,
 1036, 1328, 1329, 1038, 1330, 1331,  760, 1095,  196,  540,
  775,  196, 1396,  621,  158,  275,  709, 1044, 1388, 1742,
  328,  196,  338,  196, 1304,  196,  159,  574,  361,  196,
  782,  361,  636,  803,  690,  621,  151,  734, 1619,  622,
 1423,  703, 1607,  603, 1062, 1101,  632,  635,  196,  690,
  196,  574,  326,  379,  277,  467,  196,  151,  196,  829,
  720,  622, 1544, 1032,  796,  212,  760, 1580,  632, 1581,
  796,  760,  873,  196,  700, 1789,  152,  987, 1104,  196,
 1211,  743,  703, 1530,  381,  703,  151, 1470,  760, 1337,
 1455,  628,  356,  277,  634,  760,  223, 1035,  196,  650,
  703,  828,  603,  707, 1608,  151,  196,  196, 1109,  211,
 1623,  858,  859,  196,  151, 1087, 1823,  707, 1403,  257,
  151,  257, 1010,  514,  760,  257,   76,  383,  257,  760,
 1002,  196,  527, 1428,  850,  152,   76,  277,  151,  628,
  361,  212,  650, 1200,  705,  161,  636,  196, 1469,  621,
  875,  745,  361,  705,  361, 1444,  277,  478,  705,  264,
  874,  635,  705, 1030, 1031,   85,   89,  527,  196,   76,
 1655, 1543,  385,  218, 1032,  622, 1657, 1115,  751,  196,
  217,  960,  632,  700,  784, 1593,  383,  709,  602,  760,
  362,  650,  745,  362,  525,  745,  915,  700,  177,  160,
 1243,  793,  718,  907,  949,  698, 1772, 1305, 1822,  709,
  745, 1399,  212,  212,  492,  638,  469, 1434,   76,  257,
  125,  469,  707, 1541,  384,  705, 1694,  512,  196, 1489,
  469,  760,  211,  196,  705,  506,  460, 1008,  196,   76,
  709,  257,  999, 1698, 1019, 1019, 1024, 1766,  257,  390,
 1756,  602,  161,  150,  602,  150,  709,  177,  212,  558,
  122, 1262, 1118, 1471,  212, 1306,  821,  196,  212,  527,
  602,  855, 1255, 1270,  151,  700,  907, 1051, 1220, 1255,
  597,  236,  257,  384, 1526, 1252, 1253, 1054, 1056, 1254,
  478,  212,  223, 1443, 1292, 1194,  602, 1034, 1197, 1256,
 1534, 1116,  362,  822, 1067,  385,  976, 1257,  390,  709,
 1474,  690, 1104, 1383,  362,  597,  362,  285,  690,  212,
  212,  690, 1207,  709,  527,  690, 1456,  908,  135, 1748,
  223,  700, 1466,  690,  391, 1053,  466, 1061, 1088, 1755,
  700,  514,  257,  597, 1510,  430,  597, 1406, 1099,  690,
 1072,  387,  136,  683, 1416,  152,  150,  687,  137,  703,
  286,  749,  597,  223,  223,  223,  150,  913, 1084, 1085,
 1086,  323,  703,  690, 1537, 1363,  152,  907, 1111,  976,
  289,  703, 1794, 1739,  324,  603, 1119,  135,  597, 1847,
  338,  383,  907,  391,  683,  466,  238,  683,  687,  181,
  908,  687,  369,  690,  639,  152,  907,  823,  690,  386,
  387,  136,  683, 1198,  468,  745,  687,  137, 1025,  325,
  603,  326, 1196, 1542,  152,  246,  843,  907,  703,  907,
  690, 1195,  750,  152, 1704,  122,  122,  703, 1208,  152,
  181, 1848,  690,  181,  690,  512,  492, 1376,  603, 1179,
 1378,  603,  441, 1199,  257,  187,  177,  152,  181, 1732,
 1002,  469,  196,  201,  469,  230,  129,  603,  386,  177,
 1609,  745,  469,  468,  389,  258,  983,  177,  708,  348,
  388,  703, 1650,  709,  469,  469,  441,   14,  384,  196,
  774, 1026,  287,  603,  844,  703,  131, 1451,  177,  370,
  707,  908,  338,  253,  177,  989,  177, 1651, 1733,  707,
 1662,  285,  707,  390,  187,  149,  908,  204,  205, 1734,
 1265, 1266, 1704, 1005,  383,  338, 1255,  177, 1473, 1658,
  908,  327,  622,  389, 1714, 1643,  262,  623, 1645,  388,
 1006,  707,  254,  700,  253,  492,  177,  253, 1831, 1832,
 1381,  908, 1725,  908,  909,  177,  349,  263, 1695,  707,
  211,  177,  253,  132, 1696, 1652, 1283,  460,  707, 1600,
 1317,  708,  706, 1663,  707, 1704,  709,  707, 1284,  177,
  188, 1283,  242,  254,  700,  285,  254, 1606, 1130, 1393,
 1393, 1397,  212,  152,  466,  223, 1540,  959,  391,  212,
 1299,  254,  700,  745,  266,  882,  135,  466,  135, 1871,
  889,  745, 1418,  223, 1131,  387,  177,  683, 1319,  223,
 1704,  384, 1419, 1735, 1323, 1705, 1191, 1785,  912, 1425,
  371, 1427,  136,  365,  492,  504,  371,  338,  137,  188,
  745,  745,  466,  745,  466, 1516,  390,  936,  937, 1519,
  745,  745,  745,  745,  745, 1750,  745,  181,  807,  745,
  745,  745,  745,  745,  687,  687,  270,  745,  745,  366,
  181,  683,  468,  386,  683, 1193,  504,  726,  181,  504,
  233,  683,  622,  666,  683,  468, 1801, 1802,  683, 1710,
  271, 1711, 1472,  683,  504, 1775,  683, 1615,  700,  181,
  576,  469,  285,  469,  727,  181,  135,  181, 1796, 1753,
 1774,  145,  683,  187, 1019,  177,  704,  234, 1205,  469,
  468,  705,  468,  622,  666,  196,  187,  666,  181,  367,
 1217,  391,  285,  705,  187, 1861,  683,  665,  389,  700,
  683,  622,  666,  460,  388,  917,  272,  181,  387,  683,
 1294, 1754,  572, 1829,  466,  187,  181,  700, 1753,  576,
 1139,  187,  181,  187, 1182, 1183,  683,  687,  581, 1800,
  367, 1529, 1661,  367,  705,  919, 1140,  704,  665, 1862,
  181,  253,  705,  253,  187, 1679, 1642,  460,  367,  578,
  253,  938,  705,  683, 1481, 1454,  665,  514, 1485, 1295,
  276,  707,  460,  187, 1753,  683,  386,  683,  687,  704,
  141,  572,  187,  683,  705, 1662,  146,  181,  187,  668,
  254, 1141,  254,  147,  330, 1697,  328,  581,  707,  254,
  882,  707,  468,  668,  882,  778,  187, 1753,  188, 1757,
 1758,  668,  683,  683,  292,  683, 1830,   99,  578, 1178,
 1142,  188,  683,  683,  683,  683,  683,  668,  683,  188,
  707,  683,  683,  683,  683,  683,  308,  427, 1808,  683,
  683,  389,  331,  187,  700,  580,  778,  388, 1663,  778,
  188,  253,   14, 1798,  101,  328,  188,  707,  188,  486,
  814, 1647,  815, 1545,  778,  542,  707, 1019,  491,  567,
  211,  512,  469,  253,  285, 1809, 1393,  309, 1184,  188,
  253,  668,  196,  504,  318,  504,  181, 1143, 1810,  504,
  254,  406,  504, 1591,  321,  105, 1656,  514,  188,  522,
  700,  407,  212, 1579,  580,  504, 1294,  188, 1387,  700,
 1389,  150,  254,  188,  253,  543,  706,  921,  340,  254,
  704,  363, 1255,  102,  106,  107,  403,  108,  567, 1850,
 1872,  188, 1873,  103,  109,  110,  111,  112,  113,  502,
  114,  160,  187,  115,  116,  117,  118,  119,   52,  544,
  406,  120,  121,  254,  332, 1298,  441,  706,  104,   53,
  407,  704,  351,   43,  353,  363,   54,  545,  188, 1660,
  251,  666,  333, 1818,  253,  706,  577, 1692,  362,  704,
  363,  666,  667,  504,  666,  403,  700,  578,  502,  708,
  706,   55,  707,  666,  709, 1824,  698,  309,  690,  700,
  160,  512,  695,  696,  698,  504,  334, 1841,  700, 1731,
   51,  251,  504,  254,  251,  613,  190,  277, 1693,  161,
  367,  705,   43,  667, 1672,  665,  667,  367,  714,  251,
  367,  713,   41, 1579,  367,  665, 1825, 1860,  665, 1690,
  666,  667,  367,  705, 1001,  765,  504,  665, 1867,  666,
  884, 1870,  949,  884,  707,  700,   56,  150,  367, 1707,
 1874, 1699,  193, 1708,  700,   24,  735,  188,  756,   51,
  737,   47,  765, 1226,  613,  151, 1743,  747,  161,   49,
  354,  150,  367, 1826,   80,   81,  379, 1227,   57,  372,
  705,   41,   58,  666,  665,  707,  297,  379,  374,  705,
 1723,  480,  884,  665, 1731,  666, 1226,  666,  700, 1731,
  377, 1228,  367,  707,  127,  514,  374,  355, 1731,  356,
 1227,   25,  700,  778,   24,  885,  778,  756,  885,  376,
   47,  196,  648,  778,  648,  648,  778,  648,   49,  367,
  778, 1277,  357,  705, 1228,  778, 1731,  665,  778,  514,
  648,  367,  380,  367,   48,  648, 1777,  705, 1226,  665,
  480,  665,  598,  151,  778, 1229, 1740,  358, 1648,  150,
 1814, 1731, 1227,  127,  464,  374, 1731,  885, 1278,  514,
   25, 1761,  999,  792, 1821,  381,  791,  150,  778,  217,
  514,  778,  778,  514,  708,  706, 1228,  707, 1229,  709,
  293,  778,  514,  190, 1779,  708,  706, 1188,  707, 1729,
  709, 1771, 1838,   48, 1775, 1780,  151,   50,  778,  512,
 1849,  598,  150,  708,  706, 1761,  707,  704,  709, 1840,
  217,  150,  705,  217,  706,  294, 1836, 1855,  704,   45,
  778,  888, 1857,  128,  706,  778,  212,  706,  217,  363,
 1229, 1579,  363,  512, 1730,  116,  706,  778,  491,  778,
  704,  151,  916,  295,  698, 1514,  296,  150,   46,  932,
  472, 1761,  217,  999,  939,  427,   50,  150,  943,  476,
  193,  710,  297,  512,  946, 1579,  950,  951,  952,  953,
  954,  427,  946,  342,  512, 1585,  343,  512,   45,  473,
  667,  477,  128,  706, 1729,  193,  512,  704,  298, 1816,
  667,  379,  706,  667,  116, 1579,  704, 1586, 1729,  384,
  978,  491,  667,  977, 1587,  698, 1579,   46, 1111, 1579,
  720,  217,  998,  193,  489,  193, 1112,  479, 1579,  481,
 1113,  344,  497,  491,  998,  699, 1729,  998,  522, 1730,
 1093,  482,  648, 1092, 1817,  483,  706,  613, 1588,  648,
  704,  363,  258, 1730,  140,  141,  996,  720,  648,  667,
  706, 1729,  707,  363,  704,  363, 1729,  500,  667,  364,
  720,  648,  707,  501, 1003,  707,  699,  648,  648,  204,
  205, 1730,  613,  364,  707,  648, 1003,  708,  706, 1003,
  707,  364,  709,  498,  699,  510,  972,  996,  996,  996,
  996,  996,  996,  996,  520,  648, 1730,  364,  648,   14,
  613, 1730,  667,  613,  538,  996,  996,  996,  996,  527,
  989,  526,  704,  549,  667,  541,  667,  705,   14,  613,
  562,  707,  986,  704,  648,  976,  648,  648,  705, 1100,
  707, 1192,  869, 1237, 1191,  569, 1092,  996,  648,  996,
 1242,  704,  648, 1241,  572,  613,  705,  708,  706,  586,
  707,  364,  709, 1120, 1121, 1125, 1063, 1064, 1133, 1135,
 1137,  105, 1145, 1148, 1151, 1153, 1156, 1159,  123,  123,
 1163, 1164, 1770, 1166,  707, 1168,  582,  637, 1171, 1172,
 1173, 1174, 1175, 1176,  598,  641,  643,  679,  707,  152,
  106,  107,  645,  108,  676, 1180,  677,  427,  678,  648,
  109,  110,  111,  112,  113,  648,  114, 1837,  680,  115,
  116,  117,  118,  119,  681,  648,  682,  120,  121,  598,
  683,  153,  684,  708,  706,  424,  707,  685,  709,  648,
  648,  648,  648,  648,  648,  686,  154,  687,  989,  708,
  706,  491,  707,  648,  709, 1286, 1291,  598, 1285, 1290,
  598,  688,  648,  648,  648,  648,  648,  648,  648,  648,
  648,  648,  648,  693,  648,  694,  598,  648,  648, 1699,
  648,  648,  648,  648,  648,  648,  648,  648,  703,  710,
  648,  648,  648,  648,  648,  711,  797,  712,  648,  648,
  708,  706,  598,  707, 1401,  709, 1491, 1400,  716, 1191,
  736,  708,  706,  730,  707,  721,  709,  733,  996,  740,
  996,  996,  400,  401,  402,  704, 1521,  738,  996,  710,
  705,  942,  708,  706,  741,  707,  996,  709, 1134,  753,
 1138,  996, 1146, 1149,  996, 1154, 1157, 1160,  757,  758,
  410,  411,   98,  699,  910,  762,  914,  764,  996,  765,
  918,  920,  922,  996,  996,  230,  699,  996, 1528, 1804,
 1866,  710, 1803, 1775,  996,  699,  996,  996,  728,  231,
  996,  728,  767,  768,  996, 1842,  770,  996,  773,  996,
  529,  996,  776,   99,  781,  704,  996,  996,  946,  996,
  705,  782,  784,  785,  794,  100,  230,  818,  819,  230,
 1344, 1345,  820, 1347, 1349,  996,  996,  825,  996,  277,
  231, 1354,  699,  231,  230,  830, 1341,  831,  835,  836,
  101,  699,  996,  837,  996,  996,  996,  838,  231,  996,
 1367,  841,  996,  996,  846,  840,  852,  853, 1374, 1375,
  427,  856,  996,  427,  866, 1180,  233,  857,  868, 1382,
  427, 1384, 1385,  869,  877,  878,  996,  879,  883,  996,
  934,  704,  105,  944,  972,  699,  705,  973,  975,  980,
  984,  996, 1012,  491, 1003,  996, 1015,  704, 1032,  699,
  985,  996,  705,  234,  427, 1037,  996,  847, 1042,  102,
 1043,  106,  107, 1045,  108, 1052,  661,  996,  996,  103,
  996,  109,  110,  111,  112,  113,  996,  114,  797, 1049,
  115,  116,  117,  118,  119, 1050, 1066,  738,  120,  121,
 1058, 1059, 1060, 1436,  104, 1070, 1076, 1083,  704, 1081,
 1107, 1090, 1108,  705, 1095,  996,  996,  976,  996,  704,
 1127,  726, 1165, 1167,  705,  996,  996,  996,  996,  996,
 1169,  996, 1177, 1189,  996,  996,  996,  996,  996, 1498,
  704,  704,  996,  996, 1203,  705, 1209, 1201, 1225,  988,
  708,  706,  491,  707, 1206,  709, 1219, 1231,  976,  976,
  976,  976,  976,  989,  976, 1170,  708,  706, 1223,  707,
  713,  709, 1244, 1486, 1247, 1258,  976,  976,  976,  976,
  708,  706, 1494,  707, 1267,  709, 1269, 1287, 1279, 1499,
  958, 1289, 1361,  708,  706, 1296,  707, 1300,  709,  923,
  924,  925,  926,  927,  928,  929,  930,  105,  976, 1301,
  976,  708,  706,  242,  707,  230,  709, 1302,  427, 1303,
 1312, 1314,  427, 1321, 1101, 1324,  958,  242, 1768,  231,
 1325, 1326, 1327, 1333, 1334, 1338,  106,  107,  105,  108,
 1339,  491, 1340, 1342, 1343, 1350,  109,  110,  111,  112,
  113, 1351,  114, 1352, 1531,  115,  116,  117,  118,  119,
 1353, 1355, 1356,  120,  121, 1357, 1358,  106,  107,  105,
  108, 1359, 1360,  907, 1380, 1377, 1405,  109,  110,  111,
  112,  113, 1402,  114, 1386, 1412,  115,  116,  117,  118,
  119, 1409, 1410, 1411,  120,  121, 1413, 1421,  106,  107,
 1414,  108,  527, 1424, 1433, 1431, 1426,  564,  109,  110,
  111,  112,  113,  242,  114, 1445, 1441,  115,  116,  117,
  118,  119, 1446, 1447, 1448,  120,  121,  242, 1362,  708,
  706, 1449,  707, 1450,  709,  242, 1453, 1458, 1628, 1629,
 1468, 1475,  242,  527, 1479, 1335,  527, 1480, 1484,  242,
 1490, 1511,  661, 1517, 1518, 1640,  242, 1523, 1524,  427,
 1535,  527, 1595, 1582, 1592, 1597, 1365,  708,  706, 1594,
  707, 1601,  709, 1596, 1598, 1599,  242, 1602, 1603,  976,
 1611,  976,  976, 1616, 1613, 1620,   77, 1621,  704,  976,
  242, 1622, 1667,  705, 1624, 1625, 1626,  976, 1627, 1630,
 1332, 1631,  976, 1632,  704,  976, 1633, 1634, 1635,  705,
 1636, 1637, 1638, 1639, 1641, 1665,  132, 1666,  704,  976,
 1649,  976, 1668,  705,  976,  976, 1000, 1669,  976, 1677,
 1675,  704, 1686, 1673, 1674,  976,  705,  976,  976, 1676,
 1682,  976, 1685, 1688,  957,  976, 1689, 1712,  976,  704,
  976, 1717,  976, 1716,  705, 1727, 1706,  976,  976, 1678,
  976, 1741, 1744, 1752, 1709, 1767, 1776, 1000, 1000, 1000,
 1000, 1000, 1000, 1000, 1773, 1784,  976,  976, 1790,  976,
 1792, 1795, 1797, 1799, 1806, 1000, 1000, 1000, 1000, 1807,
 1531, 1812,  664,  976, 1815,  976,  976,  976, 1787, 1788,
  976, 1722,  160,  976,  976, 1724, 1835, 1839, 1853, 1846,
 1845, 1775, 1858,  976, 1366,  708,  706, 1000,  707, 1000,
  709,  664,  664, 1856,  664, 1863, 1864,  976, 1859, 1865,
  976,  664,  664,  664,  664,  664, 1875,  664,    1,  203,
  664,  664,  664,  664,  664,  628,  976,   29,  664,  664,
   31,  371,  976,  760, 1368,  708,  706,  976,  707,  148,
  709,  271,  525,  657,  427,  269,  457,  704,  976,  976,
   81,  976,  705,  959,  687,  213,  525,  976,  193,  276,
  527,  399,  527,  525,  958,  698,  527,  696,  692,  527,
 1369,  708,  706,  694,  707,  275,  709,  940,  194,  372,
  715,  714,  527,  939,  223,  704,  976,  976,  105,  976,
  705,  323,  716,  288,  331,  622,  976,  976,  976,  976,
  976,  725,  976,  343, 1820,  976,  976,  976,  976,  976,
   27,  329,   30,  976,  976,   39, 1391,  106,  107,  137,
  108, 1322, 1320,  470,  105,  867,  974,  109,  110,  111,
  112,  113,  480,  114,  471, 1106,  115,  116,  117,  118,
  119, 1318,  247, 1308,  120,  121, 1370,  708,  706, 1604,
  707, 1605,  709,  106,  107,  551,  108, 1297,  596,  832,
  527, 1013,  534,  109,  110,  111,  112,  113, 1000,  114,
 1000, 1000,  115,  116,  117,  118,  119, 1246, 1000,  518,
  120,  121,  527, 1309,  494, 1719, 1000, 1687, 1786,  527,
 1584, 1000, 1047, 1590, 1000,  813, 1371,  708,  706, 1793,
  707,  525,  709, 1828,  566,  567, 1791, 1435, 1000, 1769,
 1005, 1040, 1671, 1000, 1000, 1783, 1819, 1000, 1869, 1738,
 1238, 1538, 1417,  527, 1000,  494, 1000, 1000,  494, 1751,
 1000,  574,  557,  704, 1000,  817,  548, 1000,  705, 1000,
 1082, 1000,  525,  494, 1465,  525, 1000, 1000,  139, 1000,
 1073, 1005, 1005, 1005, 1005, 1005, 1005, 1005,  511,  748,
  525,  752,  525,  854, 1004, 1000, 1000,  290, 1000, 1005,
 1005, 1005, 1005,  704,  998, 1017, 1222, 1715,  705,   76,
 1653,  105, 1000, 1805, 1000, 1000, 1000, 1834,  761, 1000,
 1430, 1379, 1000, 1000,    0, 1215, 1346,  981,    0,  424,
    0, 1005, 1000, 1005,    0,    0,    0,    0,    0,  704,
  106,  107,    0,  108,  705,    0, 1000,    0,    0, 1000,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0, 1000,    0,  120,  121,    0,
    0, 1000,    0, 1372,  708,  706, 1000,  707,    0,  709,
    0,    0,    0,    0,    0,    0,    0, 1000, 1000,    0,
 1000,    0,    0,    0,    0,    0, 1000,    0,    0,    0,
 1373,  708,  706,    0,  707,    0,  709, 1512,  708,  706,
    0,  707,    0,  709,    0,  704,    0,    0,  522,    0,
  705,    0,    0,    0,    0, 1000, 1000,    0, 1000,    0,
    0,    0,    0,    0,    0, 1000, 1000, 1000, 1000, 1000,
    0, 1000,    0,    0, 1000, 1000, 1000, 1000, 1000,    0,
    0,    0, 1000, 1000,    0,    0,    0,    0,    0,  522,
    0,    0,  522,    0,    0,  704,    0, 1513,  708,  706,
  705,  707,    0,  709,    0,    0,    0,  522,    0,  522,
    0, 1520,  708,  706,    0,  707,    0,  709,    0,  525,
  708,  706,  494,  707,  494,  709,    0,    0,    0,    0,
    0,  494, 1005,    0, 1005, 1005,    0,    0,  525,  525,
    0,  525, 1005,    0,  494,  525,    0,    0,  525,    0,
 1005,    0,    0,    0,    0, 1005,    0,    0, 1005,    0,
    0,  525,    0,    0,    0,  527,    0,  525,    0,    0,
    0,    0, 1005,    0, 1001,    0,    0, 1005, 1005,    0,
 1249, 1005,    0,    0,    0,    0,    0,    0, 1005,    0,
 1005, 1005,    0,    0, 1005,    0,    0,    0, 1005,    0,
    0, 1005,    0, 1005,    0, 1005,  527,    0,    0,  527,
 1005, 1005,    0, 1005,    0, 1001, 1001, 1001, 1001, 1001,
 1001, 1001,  494,    0,  527,    0,  527,    0,    0, 1005,
 1005,    0, 1005, 1001, 1001, 1001, 1001,    0,    0,  525,
    0,    0,  704,    0,  494,    0, 1005,  705, 1005, 1005,
 1005,  494,    0, 1005,    0,    0, 1005, 1005,    0,    0,
    0,  525,    0,    0, 1659, 1001, 1005, 1001,  525,  704,
    0,    0,    0,    0,  705,    0,  704,  525,    0,  525,
 1005,  705,    0, 1005,    0,  494,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  525,    0, 1005,
    0,    0,  525,    0,    0, 1005,  522,    0,    0,    0,
 1005,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1005, 1005,    0, 1005,  522,  522,    0,  522,    0,
 1005,  525,  522,    0,    0,  522,  704,    0,    0,    0,
 1497,  705,    0,    0,    0,    0,    0,    0,  522,    0,
  704,    0,    0,    0,  522,  705,    0,    0,  704, 1005,
 1005,  105, 1005,  705,    0,    0,    0,    0,    0, 1005,
 1005, 1005, 1005, 1005,    0, 1005,    0,    0, 1005, 1005,
 1005, 1005, 1005,    0,    0,    0, 1005, 1005,    0,    0,
  106,  107,    0,  108,    0,    0,    0,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,  527,    0,    0,  522,    0,    0,    0,
  388,    0,    0,    0,    0,    0, 1001,    0, 1001, 1001,
    0,    0,    0,  527,    0,  527, 1001,    0,  522,  527,
    0,    0,  527,    0, 1001,  522,    0,    0,  390, 1001,
    0,    0, 1001,    0,    0,  527,  522,  527,    0,    0,
    0,  527,    0,    0,    0,    0, 1001,    0,  854,    0,
  542, 1001, 1001,    0,  522, 1001,    0,  392,    0,  522,
    0,    0, 1001,    0, 1001, 1001,    0,    0, 1001,    0,
    0,  588, 1001,    0,    0, 1001,    0, 1001,  527, 1001,
    0,  527,    0,    0, 1001, 1001,    0, 1001,  522,  854,
  854,  854,  854,  854,    0,  854,  527,    0,  527,    0,
  543,    0,    0, 1001, 1001,    0, 1001,  854,  854,  854,
  854,    0,    0,  527,    0,    0,    0,    0,    0,  529,
 1001,    0, 1001, 1001, 1001,    0,    0, 1001,  530,    0,
 1001, 1001,  397,    0,  544,  527,    0,    0,    0,    0,
 1001,  854,  527, 1670,    0,    0,    0,    0,  399,    0,
    0,    0,  545,  527, 1001,    0,    0, 1001,    0,    0,
    0,    0,  400,  401,  402,    0,  204,  205,    0,    0,
    0,  527,    0, 1001,    0,    0,  527,    0,    0, 1001,
    0,    0,    0,    0, 1001,  405,  406,  407,  408,  409,
  410,  411,  412,  413,    0, 1001, 1001,    0, 1001,    0,
  415,  416,    0,    0, 1001,  527,    0,    0,    0,  420,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1001, 1001,  105, 1001,    0,    0,    0,
    0,    0,    0, 1001, 1001, 1001, 1001, 1001,    0, 1001,
    0,    0, 1001, 1001, 1001, 1001, 1001,    0,    0,    0,
 1001, 1001,    0,    0,  106,  107,    0,  108,    0,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,  589,    0,
    0,  120,  121,    0,    0,  527,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  854,  527,  854,  854,    0,    0,    0,    0,    0,  527,
  854,    0,    0,    0,    0,    0,    0,    0,  854,    0,
    0,    0,    0,  854,    0,    0,  854,  527,    0,    0,
    0,    0,    0,  527,    0,    0,  527,    0,    0,    0,
  854,    0,  866,    0,    0,  854,  854,    0,    0,  854,
  527,    0,    0,  527,    0,    0,  854,    0,  854,  854,
    0,    0,  854,    0,    0,    0,  854,  223,  527,  854,
    0,  854,    0,  854,    0,    0,    0,    0,  854,  854,
    0,  854,    0,  866,  866,  866,  866,  866,    0,  866,
    0,    0,    0,    0,    0,    0,    0,  854,  854,    0,
  854,  866,  866,  866,  866,    0,    0,    0,  223,    0,
    0,  223,    0,    0,  854,    0,  854,  854,  854,    0,
    0,  854,    0,    0,  854,  854,  223,    0,    0,    0,
    0,    0,    0,    0,  854,  866,    0,    0,  527,    0,
    0,    0,    0,    0,    0,  527,    0,    0,  854,    0,
    0,  854,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  527,    0,    0,    0,  854,    0,    0,
    0,    0,    0,  854,    0,    0,    0,    0,  854,    0,
    0,    0,    0,    0,    0,  251,    0,    0,    0,  854,
  854,    0,  854,  527, 1487, 1488,    0,  527,  854,    0,
 1492, 1493,    0, 1495, 1496,    0,    0,    0,    0,    0,
    0, 1500, 1501, 1502, 1503,    0, 1504, 1505, 1506, 1507,
 1508, 1509,    0,    0,    0,    0,  251,  854,  854,  251,
  854,    0,    0,    0,  105,    0,    0,  854,  854,  854,
  854,  854,    0,  854,  251,    0,  854,  854,  854,  854,
  854,    0,    0,    0,  854,  854,    0,    0,  869,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,  527,    0,  527,
  120,  121,    0,    0,    0,    0,  527,    0,    0,  869,
  869,  869,  869,  869,  866,  869,  866,  866,    0,  527,
    0,    0,    0,    0,  866,    0,    0,  869,  869,  869,
  869,    0,    0,    0,    0,    0,    0,  866,    0,    0,
  866,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  866,  223,    0,  223,    0,  866,
  866,  869,    0,  866,  223,    0,    0,    0,    0,    0,
  866,    0,  866,  866,    0,    0,  866,    0,    0,    0,
  866,    0,    0,  866,    0,  866,    0,  866,    0,    0,
    0,   62,  866,  866,    0,  866,    0,  527,    0,    0,
    0,    0,   63,    0,    0,  537,    0,    0,    0,   64,
    0,  866,  866,  673,  866,    0,    0,    0,    0,  527,
    0,    0,   65,    0,    0,    0,  527,    0,  866,    0,
  866,  866,  866,    0,    0,  866,    0,    0,  866,  866,
    0,    0,    0,    0,    0,  537,  537,    0,  866,  537,
    0,    0,    0,  673,  673,  223,    0,  673,    0,    0,
  527,    0,  866,    0,  537,  866,  537,    0,    0,    0,
    0,    0,  673,  251,    0,  251,    0,  223,    0,    0,
    0,  866,  251,   66,  223,    0,    0,  866,    0,    0,
    0,    0,  866,    0,    0,    0,  537,    0,    0,   67,
    0,    0,    0,  866,  866,    0,  866,    0,    0,    0,
    0,    0,  866,    0,    0,    0,   68,    0,  223,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  869,   69,  869,  869,    0,   70,    0,    0,   71,    0,
  869,  866,  866,    0,  866,    0,   72,    0,    0,    0,
    0,  866,  866,  866,  866,  866,  869,  866,    0,    0,
  866,  866,  866,  866,  866,   73,    0,    0,  866,  866,
  869,    0,  870,  251,    0,  869,  869,    0,    0,  869,
    0,    0,    0,    0,    0,    0,  869,    0,  869,  869,
    0,    0,  869,    0,    0,  251,  869,    0,    0,  869,
    0,  869,  251,  869,    0,    0,    0,    0,  869,  869,
    0,  869,    0,  870,  870,  870,  870,  870,    0,  870,
    0,    0,    0,    0,    0,    0,    0,  869,  869,    0,
  869,  870,  870,  870,  870,    0,  251,    0,    0,    0,
    0,    0,    0,    0,  869,    0,  869,  869,  869,    0,
    0,  869,    0,    0,  869,  869,    0,    0,    0,    0,
    0,    0,    0,  537,  869,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  869,  537,
    0,  869,  537,  537,    0,  537,    0,    0,    0,  537,
    0,    0,  537,    0,    0,    0,    0,  869,    0,    0,
    0,    0,    0,  869,    0,  537,    0,    0,  869,    0,
    0,  537,    0,    0,  537,    0,    0,    0,    0,  869,
  869,    0,  869,    0,    0,    0,    0,    0,  869,    0,
    0,  673,    0,    0,  673,    0,    0,    0,    0,    0,
    0,  673,    0,   84,  673,    0,    0,    0,  673,    0,
   85,    0,    0,  673,   86,   87,  673,  869,  869,    0,
  869,    0,    0,   88,    0,    0,    0,  869,  869,  869,
  869,  869,  673,  869,    0,   89,  869,  869,  869,  869,
  869,    0,    0,  537,  869,  869,    0,    0,  864,    0,
    0,    0,    0,    0,    0,    0,  673,    0,    0,    0,
  673,    0,    0,    0,    0,  537,    0,    0,    0,  673,
    0,    0,  537,    0,    0,  564,  537,    0,    0,    0,
    0,    0,    0,  537,    0,    0,  673,    0,    0,  864,
    0,  864,  864,  864,  870,    0,  870,  870,    0,    0,
   90,  537,    0,    0,  870,    0,  537,  864,  864,  864,
  864,    0,    0,  673,    0,    0,    0,   91,    0,    0,
  870,    0,    0,    0,    0,  673,    0,  673,    0,    0,
    0,  537,   92,    0,  870,  537,   93,    0,    0,  870,
  870,  864,    0,  870,    0,    0,    0,   94,    0,    0,
  870,    0,  870,  870,    0,    0,  870,    0,    0,    0,
  870,    0,    0,  870,    0,  870,   95,  870,    0,    0,
    0,    0,  870,  870,    0,  870,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  538,    0,    0,    0,    0,
    0,  870,  870,  675,  870,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  870,    0,
  870,  870,  870,    0,    0,  870,    0,    0,  870,  870,
    0,    0,    0,    0,    0,  538,  538,    0,  870,  538,
    0,    0,    0,    0,  675,    0,    0,  675,    0,    0,
    0,    0,  870,    0,  538,  870,  538,    0,  105,    0,
    0,    0,  675,    0,    0,    0,    0,    0,    0,    0,
    0,  870,    0,    0,    0,    0,    0,  870,    0,    0,
    0,    0,  870,    0,    0,    0,  538,  106,  107,    0,
  108,    0,    0,  870,  870,    0,  870,  109,  110,  111,
  112,  113,  870,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0,    0,    0,
  864,    0,  864,  864,    0,    0,    0,    0,    0,    0,
  864,  870,  870,    0,  870,    0,    0,    0,    0,    0,
    0,  870,  870,  870,  870,  870,  864,  870,    0,    0,
  870,  870,  870,  870,  870,    0,    0,    0,  870,  870,
  864,    0,  863,    0,    0,  864,  864,    0,    0,  864,
    0,    0,    0,    0,    0,    0,  864,    0,  864,  864,
    0,    0,  864,    0,    0,    0,  864,    0,    0,  864,
    0,  864,    0,  864,    0,    0,    0,    0,  864,  864,
    0,  864,    0,  863,    0,  863,  863,  863,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  864,  864,    0,
  864,  863,  863,  863,  863,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  864,    0,  864,  864,  864,    0,
    0,  864,    0,    0,  864,  864,    0,    0,    0,    0,
    0,    0,    0,  538,  864,  863,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  864,  538,
    0,  864,  538,  538,    0,  538,    0,    0,    0,  538,
    0,    0,  538,    0,    0,    0,    0,  864,    0,    0,
    0,    0,    0,  864,    0,  538,    0,    0,  864,    0,
    0,  538,    0,    0,  538,    0,    0,    0,    0,  864,
  864,    0,  864,    0,    0,    0,    0,    0,  864,    0,
    0,  675,    0,    0,  675,    0,    0,    0,    0,    0,
    0,  675,    0,    0,  675,    0,    0,    0,  675,    0,
    0,    0,    0,  675,    0,    0,  675,  864,  864,    0,
  864,    0,    0,    0,    0,    0,    0,  864,  864,  864,
  864,  864,  675,  864,    0,    0,  864,  864,  864,  864,
  864,    0,    0,  538,  864,  864,    0,    0,  865,    0,
    0,    0,    0,    0,    0,    0,  675,    0,    0,    0,
  675,    0,    0,    0,    0,  538,    0,    0,    0,  675,
    0,    0,  538,    0,    0,  570,  538,    0,    0,    0,
    0,    0,    0,  538,    0,    0,  675,    0,    0,  865,
    0,  865,  865,  865,  863,    0,  863,  863,    0,    0,
    0,  538,    0,    0,  863,    0,  538,  865,  865,  865,
  865,    0,    0,  675,    0,    0,    0,    0,    0,    0,
  863,    0,    0,    0,    0,  675,    0,  675,    0,    0,
    0,  538,    0,    0,  863,  538,    0,    0,    0,  863,
  863,  865,    0,  863,    0,    0,    0,    0,    0,    0,
  863,    0,  863,  863,    0,    0,  863,    0,    0,    0,
  863,    0,    0,  863,    0,  863,    0,  863,    0,    0,
    0,    0,  863,  863,    0,  863,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  833,    0,    0,    0,    0,
    0,  863,  863,    0,  863,    0,  678,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  863,    0,
  863,  863,  863,    0,    0,  863,    0,    0,  863,  863,
    0,    0,    0,    0,    0,    0,  833,    0,  863,  833,
    0,    0,    0,    0,    0,    0,    0,  678,    0,    0,
  678,    0,  863,    0,  833,  863,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  678,    0,    0,    0,    0,
    0,  863,    0,    0,    0,    0,    0,  863,    0,    0,
    0,    0,  863,    0,    0,    0,    0,  106,  107,    0,
  108,    0,    0,  863,  863,    0,  863,  109,  110,  111,
  112,  113,  863,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,    0,    0,    0,
  865,    0,  865,  865,    0,    0,    0,    0,    0,    0,
  865,  863,  863,    0,  863,    0,    0,    0,    0,    0,
    0,  863,  863,  863,  863,  863,  865,  863,    0,    0,
  863,  863,  863,  863,  863,    0,    0,    0,  863,  863,
  865,    0,  868,    0,    0,  865,  865,    0,    0,  865,
    0,    0,    0,    0,    0,    0,  865,    0,  865,  865,
    0,    0,  865,    0,    0,    0,  865,    0,    0,  865,
    0,  865,    0,  865,    0,    0,    0,    0,  865,  865,
    0,  865,    0,  868,    0,  868,  868,  868,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  865,  865,    0,
  865,  868,  868,  868,  868,    0,    0,    0,    0,    1,
    0,    0,    0,    0,  865,    0,  865,  865,  865,    0,
    0,  865,  664,    0,  865,  865,    0,    0,    0,    0,
    2,    0,    0,    0,  865,  868,    0,  833,    3,    0,
    0,    0,    0,    0,    0,    4,    0,    5,  865,    6,
    0,  865,    0,    0,    7,    0,    0,    0,    0,    0,
    0,    8,  664,  664,    0,    0,  664,  865,    0,    0,
    0,    0,    0,  865,    0,    9,    0,    0,  865,    0,
    0,  664,    0,    0,    0,    0,   10,    0,    0,  865,
  865,    0,  865,  833,    0,    0,  833,    0,  865,    0,
    0,    0,    0,  833,  678,    0,  833,  678,    0,    0,
  833,    0,    0,    0,  678,  833,    0,  678,  833,    0,
    0,  678,    0,    0,    0,    0,  678,  865,  865,  678,
  865,    0,    0,    0,  833,    0,    0,  865,  865,  865,
  865,  865,    0,  865,    0,  678,  865,  865,  865,  865,
  865,    0,    0,   11,  865,  865,    0,  681,  833,    0,
   12,  833,  833,   13,    0,   14,   15,    0,    0,  678,
    0,  833,    0,  678,    0,    0,    0,    0,    0,    0,
    0,    0,  678,    0,    0,    0,    0,    0,  833,    0,
   16,    0,    0,    0,    0,    0,    0,    0,  681,  678,
    0,  681,    0,    0,  868,    0,  868,  868,    0,    0,
  833,    0,    0,    0,  868,  833,  681,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  678,  833,    0,  833,
  868,    0,    0,    0,    0,    0,    0,    0,  678,   17,
  678,    0,    0,    0,  868,    0,    0,    0,    0,  868,
  868,    0,    0,  868,   18,   19,    0,    0,    0,    0,
  868,    0,  868,  868,    0,    0,  868,    0,    0,    0,
  868,    0,    0,  868,    0,  868,    0,  868,    0,    0,
    0,    0,  868,  868,    0,  868,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  868,  868,    0,  868,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  868,    0,
  868,  868,  868,    0,    0,  868,    0,    0,  868,  868,
    0,    0,    0,    0,    0,    0,    0,    0,  868,    0,
  664,    0,    0,  664,    0,    0,    0,    0,    0,    0,
  664,    0,  868,  664,    0,  868,    0,  664,    0,    0,
    0,    0,  664,    0,    0,  664,    0,  660,    0,    0,
  421,  868,  422,    0,    0,    0,    0,  868,    0,    0,
    0,  664,  868,    0,    0,  424,    0,    0,    0,    0,
  279,    0,    0,  868,  868,    0,  868,    0,    0,    0,
    0,    0,  868,  961,    0,  664,    0,    0,    0,  664,
    0,    0,    0,    0,    0,    0,    0,    0,  664,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  868,  868,    0,  868,  664,    0,    0,    0,    0,
    0,  868,  868,  868,  868,  868,    0,  868,    0,    0,
  868,  868,  868,  868,  868,    0,    0,    0,  868,  868,
    0,    0,  664,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  664,  681,  664,    0,  681,    0,
    0,    0,  664,    0,    0,  681,    0,    0,  681,    0,
    0,    0,  681,    0,    0,    0,    0,  681,    0,    0,
  681,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  664,  664,    0,  664,    0,  681,    0,  964,    0,
    0,  664,  664,  664,  664,  664,    0,  664,    0,    0,
  664,  664,  664,  664,  664,  105,    0,    0,  664,  664,
  681,    0,    0,    0,  681,    0,    0,    0,    0,    0,
    0,    0,    0,  681,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,    0,
  681,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,  385,    0,    0,  681,    0,    0,
    0,    0,    0,  386,    0,    0, 1650,    0,    0,  681,
    0,  681,    0,    0,    0,    0,  387,  664,    0,    0,
    0,  660,  388,  389,  421,    0,  422,    0,    0,    0,
    0, 1651,    0,    0,    0,    0,    0,    0,    0,  424,
    0,  654,    0,    0,  279,    0,  664,  664,    0,  664,
  390,  461,    0,  391,    0,    0,  664,  664,  664,  664,
  664,    0,  664,    0,    0,  664,  664,  664,  664,  664,
    0,    0,    0,  664,  664,    0,    0,  105,    0,    0,
    0,  393,  394,    0,    0,  642,    0,  642,  642, 1652,
  642,    0,    0,  395,    0,    0,    0,  396,    0,    0,
    0,    0,    0,  642,    0,  884,  106,  107,  642,  108,
    0,    0,    0,    0,    0,    0,  109,  110,  111,  112,
  113,    0,  114,  105,    0,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  697,    0,    0,    0,  656,    0,
    0,    0,  106,  107,  397,  108,    0,    0,    0,    0,
  398,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  399,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,    0,    0,    0,    0,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,    0,    0,  115,  116,  117,  118,  119,
  885,  886,  887,  120,  121,    0,    0,    0,  385,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  387,    0,    0,  653,    0,    0,  388,  389,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  654,    0,    0,    0,    0,
    0,    0,    0,    0,  390,  642,    0,  391,    0,    0,
    0,    0,  642,    0,    0,  660,    0,    0,  421,    0,
  422,  642,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  424,  642,  393,  394,    0,  279,    0,
  642,  642,    0,    0,    0,    0,    0,  395,  642,    0,
    0,  396,    0,    0,    0,    0,  942,  708,  706,  655,
  707,    0,  709,  465,    0,    0,    0,    0,  642,    0,
    0,  642,    0,    0,    0,  905,  904,  906,    0,    0,
  105,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  642,    0,  642,
  642,    0,  656,    0,    0,    0,    0,    0,  397,  106,
  107,  642,  108,    0,  398,  642,    0,    0,    0,  109,
  110,  111,  112,  113,  399,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,  400,  401,
  402,  403,  204,  205,    0,    0,    0,    0,    0,    0,
    0,    0,  404,    0,    0,    0,    0,    0,    0,    0,
    0,  405,  406,  407,  408,  409,  410,  411,  412,  413,
  106,  414,  642,  108,    0,    0,  415,  416,  642,  417,
  418,  419,  111,  112,  113,  420,  114,    0,  642,  115,
  116,  657,  658,  659,    0,    0,    0,  120,  121,    0,
    0,    0,  642,  642,  642,  642,  642,  642,    0,    0,
    0,    0,    0,    0,    0,    0,  642,    0,    0,    0,
    0,    0,    0,    0,    0,  642,  642,  642,  642,  642,
  642,  642,  642,  642,  642,  642,    0,  642,    0,    0,
  642,  642,  385,  642,  642,  642,  642,  642,  642,  642,
  642,  386,    0,  642,  642,  642,  642,  642,    0,    0,
    0,  642,  642,  660,  387,    0,  421,    0,  422,    0,
  388,  389,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  424,    0,    0,    0,    0,  279,  892,    0,  654,
    0,    0,    0,    0,    0,  704,    0,    0,  390,    0,
  705,  391,    0,  893,  708,  706,    0,  707,    0,  709,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  905,  904,  906,    0,    0,    0,    0,  393,
  394,    0,    0,    0,    0,    0,    0,    0,    0,  894,
    0,  395,    0,    0,    0,  396,    0,    0,  895,  660,
    0,    0,  421,  880,  422,    0,  896,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  424,    0,    0,
    0,  105,  279,    0,    0,  897,    0,  898,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  899,    0,  900,  901,  902,  656,    0,    0,    0,
  106,  107,  397,  108,    0,    0,    0,    0,  398,    0,
  109,  110,  111,  112,  113,    0,  114,    0,  399,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,  400,  401,  402,  403,  204,  205,    0,    0,
    0,    0,    0,    0,  903,    0,  404,    0,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,    0,    0,  115,  116,  657,  658,  659,    0,    0,
  385,  120,  121,    0,    0,    0,    0,    0,    0,  386,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  387,    0,    0,    0,    0,    0,  388,  389,
    0,    0,    0,  570,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  892,    0,    0,  654,    0,    0,
    0,    0,  704,    0,    0,    0,  390,  705,    0,  391,
  893,    0,  570,  570,    0,  570,    0,    0,    0,    0,
    0,    0,  570,  570,  570,  570,  570,    0,  570,    0,
  532,  570,  570,  570,  570,  570,  385,  393,  394,  570,
  570,    0,    0,    0,    0,  386,  894,    0,    0,  395,
    0,    0,    0,  396,    0,  895,    0,    0,  387,    0,
    0,  655,    0,  896,  388,  389,    0,  485,    0,    0,
  421,  532,  422,    0,  532,    0,    0,    0,    0,    0,
    0,    0,  897,  654,  898,  424,    0,    0,    0,  532,
  279,  532,  390,    0,    0,  391,    0,    0,  899,    0,
  900,  901,  902,    0,  656,    0,    0,    0,    0,    0,
  397,    0,    0,    0,    0,    0,  398,    0,    0,    0,
    0,  532,    0,  393,  394,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,  396,
  400,  401,  402,  403,  204,  205,    0,  880,    0,    0,
    0,  903,    0,    0,  404,    0,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,    0,
  656,  115,  116,  657,  658,  659,  397,    0,    0,  120,
  121,    0,  398,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,  485,
    0,    0,  421,    0,  422,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,  424,    0,  961,
  404,    0,  279,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121, 1122,  532,    0,
 1123,    0,    0,    0,  385,    0,    0,    0,    0,    0,
    0,    0,    0,  386,  532,    0,    0,  532,  532,    0,
  532,    0,    0,  962,  532,    0,  387,  532,    0,    0,
    0,    0,  388,  389,    0,    0,    0,    0,  485,    0,
  532,  421,    0,  422,    0,  281,  532,    0,    0,  532,
    0,    0,    0,    0,    0,    0,  424,    0,    0,  963,
  390,  279,    0,  391,  964,    0,    0,    0,    0,    0,
  965,    0,    0,    0,  281,  281,    0,  281,    0,    0,
    0,  105,    0,    0,  281,  281,  281,  281,  281,  966,
  281,  393,  394,  281,  281,  281,  281,  281,    0,    0,
    0,  281,  281,  395,    0,    0,    0,  396,    0,    0,
  106,  107,    0,  108,    0,    0,    0,    0,  532,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
  532,    0,    0,    0,    0,    0,    0,  532,    0, 1132,
    0,  532, 1123,    0,    0,    0,  385,    0,  532,    0,
    0,    0, 1124,    0,  397,  386,    0,    0,    0,    0,
  398,    0,    0,    0,    0,    0,  532,    0,  387,    0,
  399,  532,    0,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,  532,    0,  404,    0,
  532,    0,  390,    0,    0,  391,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  393,  394,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,  395,    0,    0, 1136,  396,
    0, 1123,    0,    0,    0,  385,    0,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  387,    0,    0,
    0,    0,    0,  388,  389,    0,    0,  485,    0,    0,
  421,    0,  422,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1124,  424,  397,    0,    0,    0,
  279,  390,  398,    0,  391,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,  393,  394,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,  395,    0,    0,    0,  396,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,    0, 1433,    0,  120,  121,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1124,    0,  397,    0,    0,    0,    0,
    0,  398,    0,    6,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0, 1546, 1547,    0, 1548,    0,  485,
    0,    0,  421,    0,  422,  400,  401,  402,  403,  204,
  205,    0,    0,    0, 1549,    0,    0,  424,    0,  404,
 1550,    0,  279,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0, 1144,    0,    0,
 1123,    0,    0,    0,  385,    0,    0,    0,    0,    0,
 1551,    0,    0,  386,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  387,    0,    0,   14,
    0,    0,  388,  389,    0,    0,    0,    0,  485,    0,
    0,  421,    0,  422, 1552,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   16,    0,  424,    0,    0,    0,
  390,  279,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,  105,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  393,  394,    0,    0,    0,    0,    0,    0,    0,
    0, 1553,  107,  395,  108,    0,    0,  396,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115, 1554,  117,  118,  119,    0,    0,    0,  120,  121,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1147,
    0,    0, 1123,    0,    0,    0,  385,    0,    0,    0,
    0,    0, 1124,    0,  397,  386,    0,    0,    0,    0,
  398,    0,    0,    0,    0,    0,    0,    0,  387,    0,
  399,    0,    0,    0,  388,  389,    0,    0,    0,    0,
    0,    0,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,    0,    0,    0,    0,  404,    0,
    0,    0,  390,    0,    0,  391,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  393,  394,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,  395,    0,    0, 1152,  396,
    0, 1123,    0,    0,    0,  385,    0,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  387,    0,    0,
    0,    0,    0,  388,  389,    0,    0,  485,    0,    0,
  421,    0,  422,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1124,  424,  397,    0,    0,    0,
  279,  390,  398,    0,  391,    0,    0,    0,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,  393,  394,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,  395,    0,    0,    0,  396,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1124,    0,  397,    0,    0,    0,    0,
    0,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,  485,
    0,    0,  421,    0,  422,  400,  401,  402,  403,  204,
  205,    0,    0,    0,    0,    0,    0,  424,    0,  404,
    0,    0,  279,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0, 1155,    0,    0,
 1123,    0,    0,    0,  385,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,    0,    0,  554,    0,
    0,    0,    0,    0,    0,    0,  387,    0,    0,    0,
    0,    0,  388,  389,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  554,
  390,    0,  554,  391,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  554,    0,  554,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  393,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,  396,    0,  554,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1158,
    0,    0, 1123,    0,    0,    0,  385,    0,    0,    0,
    0,    0, 1124,    0,  397,  386,    0,    0,    0,    0,
  398,    0,    0,    0,    0,    0,    0,    0,  387,    0,
  399,    0,    0,    0,  388,  389,  485,    0,  490,  421,
    0,  422,    0,    0,  400,  401,  402,  403,  204,  205,
    0,    0,    0,    0,  424,    0,    0,    0,  404,  279,
    0,    0,  390,    0,    0,  391,    0,  405,  406,  407,
  408,  409,  410,  411,  412,  413,  106,  414,    0,  108,
    0,    0,  415,  416,    0,  417,  418,  419,  111,  112,
  113,  420,  114,  393,  394,  115,  116,  117,  118,  119,
    0,    0,    0,  120,  121,  395,    0,    0,    0,  396,
    0,    0,    0,    0,    0,    0,  485,    0,    0,  421,
    0,  422,    0,    0,    0,    0,  554,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,  279,
    0,    0,  554,    0,    0,    0,  554,    0,  554,    0,
    0,    0,  554,    0,    0,  554,    0,    0,    0,    0,
    0,    0,    0,    0, 1124,    0,  397,    0,  554,    0,
    0,    0,  398,    0,  554,    0,    0,  554,    0,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,  401,  402,  403,
  204,  205,    0,    0,    0,    0,    0,    0,    0,    0,
  404,    0,    0,    0,    0,    0,    0,    0,    0,  405,
  406,  407,  408,  409,  410,  411,  412,  413,  106,  414,
    0,  108,    0,    0,  415,  416,    0,  417,  418,  419,
  111,  112,  113,  420,  114,    0,  554,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,    0,    0,  385,    0,    0,    0,    0,  554,    0,
    0,    0,  386,    0,    0,  554,    0,    0,    0,  554,
    0,    0,    0,    0,    0,  387,  554,    0,    0,    0,
    0,  388,  389,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  554,    0,    0,    0,    0,  554,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  554,    0,    0,    0,  554,    0,
    0,    0,    0,  385,    0,    0,    0,    0,  392,    0,
  393,  394,  386,    0,    0,  970,    0,  970,  970,    0,
  970,    0,  395,    0,    0,  387,  396,    0,    0,    0,
    0,  388,  389,  970,    0,    0,    0,    0,  970,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  397,    0,    0,    0,    0,  392,  398,
  393,  394,    0,    0,    0,    0,    0,    0,    0,  399,
    0,    0,  395,    0,    0,    0,  396,    0,    0,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,   98,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,  697,  417,  418,  419,  111,  112,  113,
  420,  114,    0,  397,  115,  116,  117,  118,  119,  398,
    0,    0,  120,  121,    0,    0,   99,    0,    0,  399,
    0,    0,    0,    0,    0,    0,    0,    0,  100,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,    0,  134,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,  101,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,  970,    0,  115,  116,  117,  118,  119,    0,
    0,  970,  120,  121,    0,    0,    0,    0,    0,    0,
    0,    0,  648,    0,  970,  648,    0,  648,    0,    0,
  970,  970,    0,    0,    0,    0,    0,    0,    0,    0,
  648,    0,  102,    0,    0,  648,    0,    0,    0,    0,
    0,    0,  103,    0,    0,    0,    0,    0,  970,    0,
    0,  970,  802,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  104,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  970,    0,  970,
  970,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,  970,    0,  802,    0,  970,  802,    0,  423,    0,
    0,  421,    0,  422,    0,    0,    0,    0,    0,    0,
    0,  802,    0,    0,  106,  107,  424,  108,    0,    0,
    0,  279,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,  120,  121,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  970,    0,    0,    0,    0,    0,  970,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  970,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  970,  970,  970,  970,  970,  970,    0,    0,
    0,    0,    0,    0,    0,    0,  970,    0,    0,    0,
    0,    0,    0,    0,    0,  970,  970,  970,  970,  970,
  970,  970,  970,  970,  970,  970,    0,  970,    0,    0,
  970,  970,    0,  970,  970,  970,  970,  970,  970,  970,
  970,    0,  648,  970,  970,  970,  970,  970,    0,  648,
    0,  970,  970,    0,    0,    0,    0,    0,  648,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  648,    0,    0,    0,    0,    0,  648,  648,    0,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  648,    0,    0,  648,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  802,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  385,  648,  648,  485,    0,
    0,  421,    0,  422,  386,    0,    0,    0,  648,    0,
    0,    0,  648,    0,    0,    0,  424,  387,    0,    0,
    0,  279,    0,  388,  389,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  802,    0,    0,  802,    0,    0,    0,    0,    0,    0,
  802,  390,    0,  802,  391,    0,    0,  802,    0,    0,
    0,    0,  802,    0,    0,  802,    0,    0,    0,  648,
    0,    0,    0,    0,    0,  648,    0,    0,    0,    0,
  392,  802,  393,  394,    0,  648,    0,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,  396,  648,
  648,  648,  648,  648,  648,  802,    0,    0,  802,  802,
    0,    0,    0,  648,    0,    0,    0,    0,  802,    0,
    0,    0,  648,  648,  648,  648,  648,  648,  648,  648,
  648,  648,  648,    0,  648,  802,  321,  648,  648,    0,
  648,  648,  648,  648,  648,  648,  648,  648,    0,    0,
  648,  648,  648,  648,  648,  397,    0,  802,  648,  648,
    0,  398,  802,    0,    0,    0,  321,    0,    0,    0,
    0,  399,    0,  321,  802,    0,  802,  321,  321,    0,
  321,    0,    0,  321,    0,  400,  401,  402,  403,  204,
  205,    0,    0,    0,    0,    0,    0,  321,    0,  404,
    0,    0,    0,  321,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,  385,  417,  418,  419,  111,
  112,  113,  420,  114,  386,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,  485,  387,    0,  421,
    0,  422,    0,  388,  389,    0,    0,    0,    0,    0,
    0,    0,    0,  321,  424,    0,    0,    0,    0,  279,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  390,  321,    0,  391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  321,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  321,    0,    0,
    0,    0,  393,  394,    0,    0,  321,    0,  321,    0,
    0,    0,    0,    0,  395,  321,  485,    0,  396,  421,
    0,  422,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  424,    0,    0,    0,    0,  279,
    0,    0,    0,    0,  321,  321,    0,  321,    0,    0,
    0,    0,    0,    0,  321,  321,  321,  321,  321,    0,
  321,    0,    0,  321,  321,  321,  321,  321,    0,    0,
    0,  321,  321,    0,    0,  397,    0,    0,    0,    0,
    0,  398,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  399,    0,    0,    0,    0,    0,    0,    0,  689,
    0,    0,    0,    0,    0,  400,  401,  402,  403,  204,
  205,    0,    0,    0,    0,    0,    0,    0,    0,  404,
    0,    0,    0,    0,    0,    0,    0,    0,  405,  406,
  407,  408,  409,  410,  411,  412,  413,  106,  414,    0,
  108,    0,    0,  415,  416,    0,  417,  418,  419,  111,
  112,  113,  420,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,  385,  120,  121,    0,    0,    0,    0,
    0,    0,  386,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  387,    0,    0,  105,    0,
    0,  388,  389,    0,    0,    0,    0,  164,  165,  166,
  167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
  177,  178,  179,    0,    0,  180,  181,  106,  107,  390,
  108,    0,  391,  182,  183,    0,    0,  109,  110,  111,
  112,  113,  184,  114,  185,    0,  115,  116,  117,  118,
  119,    0,    0,  385,  120,  121,    0,    0,    0,    0,
  393,  394,  386,    0,    0,  485,  945,    0,  421,    0,
  422,    0,  395,    0,    0,  387,  396,    0,    0,    0,
    0,  388,  389,  424,    0,    0,    0,    0,  279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  390,
    0,    0,  391,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  697,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  397,    0,    0,    0,    0,  392,  398,
  393,  394,    0,    0,    0,    0,    0,    0,    0,  399,
    0,    0,  395,    0,    0,    0,  396,    0,    0,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,    0,
    0,   98,    0,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,   99,  397,  115,  116,  117,  118,  119,  398,
    0,    0,  120,  121,  100,    0,    0,    0,    0,  399,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  400,  401,  402,  403,  204,  205,  101,
    0,    0,    0,    0,    0,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,    0,  405,  406,  407,  408,
  409,  410,  411,  412,  413,  106,  414,    0,  108,    0,
    0,  415,  416,    0,  417,  418,  419,  111,  112,  113,
  420,  114,  385,    0,  115,  116,  117,  118,  119,    0,
    0,  386,  120,  121,  485,    0,    0,  421,    0,  422,
    0,    0,    0,    0,  387,    0,    0,    0,  102,    0,
  388,  389,  424,    0,    0,    0,    0,  279,  103,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,  391,    0,  104,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  105,    0,    0,    0,    0,    0,    0,    0,  393,
  394,    0,    0,    0,  485,    0,    0,  421,    0,  422,
    0,  395,    0,    0,    0,  396,    0,    0,    0,    0,
  106,  107,  424,  108,    0,    0,    0,  279,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  397,    0,    0,    0,    0,    0,  398,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  399,    0,
    0,    0,  221,    0,    0,    0,  221,    0,    0,    0,
    0,    0,  400,  401,  402,  403,  204,  205,    0,    0,
  221,    0,    0,    0,  221,    0,  404,  221,    0,    0,
    0,    0,    0,    0,    0,  405,  406,  407,  408,  409,
  410,  411,  412,  413,  106,  414,    0,  108,    0,    0,
  415,  416,    0,  417,  418,  419,  111,  112,  113,  420,
  114,  385,    0,  115,  116,  117,  118,  119,  221,    0,
  386,  120,  121,    0,  221,  221,  221,    0,  221,  221,
  221,    0,  221,  387,    0,    0,    0,    0,    0,  388,
  389,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,  221,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  390,    0,    0,
  391,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  221,    0,    0,    0,    0,
   80,  385,    0,    0,    0,    0,    0,    0,  393,  394,
  386,    0,  221,  485,    0,    0,  421,    0,  422,    0,
  395,    0,    0,  387,  396,    0,    0,    0,    0,  388,
  389,  424,    0,    0,  221,    0,  279,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  390,    0,    0,
  391,    0,    0,    0,    0,    0,    0,    0,    0,  221,
  221,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  397,    0,    0,    0,    0,  221,  398,  393,  394,
    0,    0,    0,    0,    0,    0,    0,  399,    0,    0,
  395,    0,    0,    0,  396,    0,    0,    0, 1150,    0,
    0,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,    0,    0,    0,    0,  404,    0,    0,    0,    0,
    0,    0,    0,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414, 1433,  108,    0,    0,  415,
  416, 1335,  417,  418,  419,  111,  112,  113,  420,  114,
    0,  397,  115,  116,  117,  118,  119,  398,    0,    0,
  120,  121,    0,    0,    0,    6,    0,  399,    0,    0,
    0,    0,    0,    0,    0,    0, 1546, 1547,    0, 1548,
    0,  400,  401,  402,  403,  204,  205,    0,    0,    0,
    0,    0,    0,    0,    0,  404, 1549,    0,    0,    0,
    0,    0, 1550,    0,  405,  406,  407,  408,  409,  410,
  411,  412,  413,  106,  414,    0,  108,    0,    0,  415,
  416,    0,  417,  418,  419,  111,  112,  113,  420,  114,
  385,    0,  115,  116,  117,  118,  119,    0,    0,  386,
  120,  121,  485,    0,    0,  421,    0,  422,    0,    0,
    0,    0,  387,    0,    0,    0,    0,    0,  388,  389,
  424, 1721, 1551,    0,    0,  279,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   14,    0,    0,    0,    0,  390,    0,    0,  391,
    0,    0,    0,    0,    0,    0, 1552,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   16,    0,    0,    0,
    0,    0,    0,    0,    0, 1678,    0,  393,  394,    0,
    0,    0,  485,    0,  105,  421,    0,  422,    0,  395,
    0,    0,    0,  396,    0,    0,    0,    0,    0,    0,
  424,    0,    0,    0,    0,  279,    0,    0,    0,    0,
    0,    0,    0, 1553,  107,    0,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115, 1554,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
  397,    0,    0,    0,    0,    0,  398,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  399,    0,    0,    0,
  870,    0,    0,    0,  598,    0,    0, 1348,    0,    0,
  400,  401,  402,  403,  204,  205,    0,    0,  871,    0,
    0,    0,  599,    0,  404,  600,    0,    0,    0,    0,
    0,    0,    0,  405,  406,  407,  408,  409,  410,  411,
  412,  413,  106,  414,    0,  108,    0,    0,  415,  416,
    0,  417,  418,  419,  111,  112,  113,  420,  114,  385,
    0,  115,  116,  117,  118,  119,  601,    0,  386,  120,
  121,    0,  602,  603,  604,    0,  605,  606,  607,    0,
  608,  387,    0,    0,    0,    0,    0,  388,  389,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  609,    0,  610,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  611,    0,    0,    0,    0,    0,  385,
    0,    0,    0,    0,    0,    0,  393,  394,  386,    0,
  612,  970,    0,    0,  970,    0,  970,    0,  395,    0,
    0,  387,  396,    0,    0,    0,    0,  388,  389,  970,
    0,    0,  613,    0,  970,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  390,    0,    0,  391,    0,
    0,    0,    0,    0,    0,    0,    0,  614,  615,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  397,
    0,    0,    0,    0,  616,  398,  393,  394,    0,    0,
    0,    0,    0,    0,    0,  399,    0,    0,  395,    0,
    0,    0,  396,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,    0,  397,
  115,  116,  117,  118,  119,  398,    0,    0,  120,  121,
    0,    0,    0,    0,    0,  399,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  400,
  401,  402,  403,  204,  205,    0,    0,    0,    0,    0,
    0,    0,    0,  404,    0,    0,    0,    0,    0,    0,
    0,    0,  405,  406,  407,  408,  409,  410,  411,  412,
  413,  106,  414,    0,  108,    0,    0,  415,  416,    0,
  417,  418,  419,  111,  112,  113,  420,  114,  970,  544,
  115,  116,  117,  118,  119,    0,    0,  970,  120,  121,
    0,  529,    0,    0,    0,    0,    0,    0,    0,    0,
  970,    0,    0,    0,    0,    0,  970,  970,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  544,    0,    0,  544,    0,    0,    0,    0,    0,    0,
    0,    0,  529,    0,  970,  529,    0,  970,  544,    0,
  544,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  529,    0,  529,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  970,  970,    0,    0,    0,
  544,    0,    0,    0,    0,  221,    0,  970,    0,  221,
    0,  970,  529,  803,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  221,    0,    0,    0,  221,    0,    0,
  221,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  803,    0,    0,  803,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  970,    0,
    0,  221,  803,    0,  970,    0,    0,  221,  221,  221,
    0,  221,  221,  221,  970,  221,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  970,  970,
  970,  970,  970,  970,    0,  221,    0,  221,    0,    0,
    0,    0,  970,    0,    0,    0,    0,    0,    0,    0,
    0,  970,  970,  970,  970,  970,  970,  970,  970,  970,
  970,  970,    0,  970,    0,    0,  970,  970,  221,  970,
  970,  970,  970,  970,  970,  970,  970,    0,    0,  970,
  970,  970,  970,  970,    0,  221,    0,  970,  970,    0,
    0,    0,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  221,    0,  529,
    0,    0,    0,  544,    0,    0,    0,  544,    0,  544,
  843,    0,    0,  544,    0,  529,  544,    0,    0,  529,
    0,  529,    0,    0,    0,  529,    0,    0,  529,  544,
    0,    0,  221,  221,    0,  544,    0,    0,  544,    0,
    0,  529,    0,    0,    0,    0,    0,  529,    0,  221,
  529,  843, 1009, 1009,  843, 1009, 1009, 1009,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  843,
 1009, 1009, 1009,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  803,    0,    0,    0,    0,
    0, 1009,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  529,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  544,
    0,    0,    0,    0,    0,    0,  544,    0,    0,    0,
  544,  529,  844,    0,    0,    0,    0,  544,  529,    0,
    0,  803,  529,    0,  803,    0,    0,    0,    0,  529,
    0,  803,    0,    0,  803,  544,    0,    0,  803,    0,
  544,    0,    0,  803,    0,    0,  803,  529,    0,    0,
    0,    0,  529,  844, 1010, 1010,  844, 1010, 1010, 1010,
    0,    0,  803,    0,    0,  544,    0,    0,    0,  544,
    0,  844, 1010, 1010, 1010,    0,    0,  529,    0,    0,
    0,  529,    0,    0,    0,    0,  803,    0,    0,  803,
  803,    0,    0,    0,    0,    0,    0,    0,    0,  803,
    0,    0,    0, 1010,    0,  597,    0,    0,    0,  598,
    0,    0,    0,    0,    0,    0,  803,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  599,    0,    0,
  600,    0,    0,    0,    0,    0,    0,    0,  803,    0,
    0,    0,  843,  803,    0,    0,    0,    0,    0,    0,
    0,    0, 1009,    0,    0,  803,    0,  803,  827,    0,
 1009,    0,    0,    0,    0, 1009,    0,    0, 1009,    0,
    0,  601,    0,    0,    0,    0,    0,  602,  603,  604,
    0,  605,  606,  607,    0,  608,    0,  845,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  843,  827,
    0,  843,  827,    0, 1009,  609,    0,  610,  843,    0,
    0,  843,    0, 1009,    0,  843,    0,  827,    0,    0,
  843, 1009,    0,  843,    0,    0,    0,    0,  845, 1011,
 1011,  845, 1011, 1011, 1011,    0,    0,    0,  611,  843,
 1009,    0, 1009,    0,    0,    0,  845, 1011, 1011, 1011,
    0,    0,    0,    0,    0,  612, 1009,    0, 1009, 1009,
 1009,    0,    0,  843,  806,    0,  843,  843,    0,    0,
    0,    0,    0,    0,  844,    0,  843,  613, 1011,    0,
    0,    0,    0,    0, 1010,    0,    0,    0,    0,    0,
    0,    0, 1010,  843,    0,    0,    0, 1010,    0,    0,
 1010,    0,    0,    0,    0,  806,    0,    0,  806, 1009,
    0,    0,  614,  615,    0,  843,    0,    0,    0,    0,
  843,    0,    0,  806,  804,    0,    0,    0,    0,  616,
  844,    0,  843,  844,  843,    0, 1010,    0,    0,    0,
  844,    0,    0,  844,    0, 1010,    0,  844,    0,    0,
    0,    0,  844, 1010,    0,  844,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  804,    0,    0,  804,    0,
    0,  844, 1010,    0, 1010,    0,    0,    0,    0,    0,
    0,    0,    0,  804,    0,    0,    0,    0, 1010,    0,
 1010, 1010, 1010,    0,    0,  844,  805,    0,  844,  844,
    0,    0,    0,    0,    0,    0,    0,    0,  844,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  827,    0,    0,    0,    0,  844,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  805,    0,    0,
  805, 1010,    0,    0,    0,    0,    0,  844,    0,  845,
    0,    0,  844,    0,    0,  805,    0,    0,    0, 1011,
    0,    0,    0,    0,  844,    0,  844, 1011,    0,    0,
    0,    0, 1011,    0,    0, 1011,  827,    0,    0,  827,
  835,    0,    0,    0,    0,    0,  827,    0,    0,  827,
    0,    0,    0,  827,    0,    0,    0,    0,  827,    0,
    0,  827,    0,    0,    0,  845,    0,    0,  845,  799,
    0, 1011,    0,    0,    0,  845,    0,  827,  845,    0,
 1011,  835,  845,    0,  835,    0,  806,  845, 1011,    0,
  845,    0,    0,    0,    0,    0,    0,    0,    0,  835,
    0,  827,    0,  800,  827,  827,  845, 1011,    0, 1011,
  799,    0,    0,  799,  827,    0,    0,    0,    0,    0,
    0,    0,    0, 1011,    0, 1011, 1011, 1011,  799,    0,
  845,  827,    0,  845,  845,    0,    0,    0,    0,    0,
    0,    0,  806,  845,  800,  806,  804,  800,    0,    0,
    0,    0,  806,  827,    0,  806,    0,    0,  827,  806,
  845,    0,  800,    0,  806,    0,    0,  806,    0,    0,
  827,    0,  827,    0,    0,    0, 1011,    0,    0,    0,
    0,    0,  845,  806,    0,    0,    0,  845,    0,  801,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  845,
    0,  845,  804,    0,    0,  804,    0,  806,    0,  834,
  806,  806,  804,    0,    0,  804,    0,    0,  805,  804,
  806,    0,    0,    0,  804,    0,    0,  804,    0,    0,
  801,    0,    0,  801,    0,    0,    0,  806,    0,    0,
    0,    0,    0,  804,    0,    0,    0,    0,  801,    0,
  834,    0,    0,  834,    0,    0,    0,    0,    0,  806,
    0,    0,    0,    0,  806,    0,    0,  804,  834,  828,
  804,  804,    0,    0,  805,    0,  806,  805,  806,    0,
  804,    0,    0,    0,  805,    0,    0,  805,    0,    0,
    0,  805,    0,    0,    0,    0,  805,  804,    0,  805,
    0,    0,  835,    0,    0,  836,    0,    0,    0,    0,
  828,    0,    0,  828,    0,  805,    0,    0,    0,  804,
    0,    0,    0,    0,  804,    0,    0,    0,  828,    0,
    0,  799,    0,    0,    0,    0,  804,    0,  804,  805,
    0,    0,  805,  805,    0,    0,  836,    0,    0,  836,
    0,    0,  805,    0,    0,    0,    0,    0,  835,    0,
    0,  835,    0,    0,  836,  800,    0,    0,  835,  805,
    0,  835,    0,    0,    0,  835,    0,    0,    0,    0,
  835,    0,    0,  835,    0,    0,    0,  799,    0,  837,
  799,  805,    0,    0,    0,    0,  805,  799,    0,  835,
  799,    0,    0,    0,  799,    0,    0,    0,  805,  799,
  805,    0,  799,    0,    0,    0,    0,    0,    0,    0,
    0,  800,    0,  835,  800,  825,  835,  835,  799,    0,
  837,  800,    0,  837,  800,    0,  835,    0,  800,    0,
    0,    0,    0,  800,    0,    0,  800,    0,  837,    0,
    0,  801,  799,  835,    0,  799,  799,    0,    0,    0,
    0,    0,  800,    0,    0,  799,  825,    0,    0,  825,
    0,  834,    0,    0,    0,  835,    0,    0,    0,    0,
  835,    0,  799,    0,  825,    0,  800,    0,    0,  800,
  800,    0,  835,    0,  835,    0,    0,    0,    0,  800,
    0,    0,    0,    0,  799,    0,    0,  801,    0,  799,
  801,    0,    0,    0,    0,    0,  800,  801,    0,  829,
  801,  799,    0,  799,  801,    0,    0,  834,    0,  801,
  834,  828,  801,    0,    0,    0,    0,  834,  800,    0,
  834,    0,    0,  800,  834,    0,    0,    0,  801,  834,
    0,    0,  834,    0,    0,  800,    0,  800,    0,    0,
  829,    0,    0,  829,    0,    0,    0,  836,  834,    0,
    0,    0,  801,    0,    0,  801,  801,    0,  829,    0,
    0,    0,    0,    0,    0,  801,    0,  828,    0,    0,
  828,  838,  834,    0,    0,  834,  834,  828,    0,    0,
  828,    0,  801,    0,  828,  834,    0,    0,    0,  828,
    0,    0,  828,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  834,  836,  801,  826,  836,    0,  828,  801,
    0,    0,  838,  836,    0,  838,  836,    0,    0,    0,
  836,  801,    0,  801,  834,  836,    0,    0,  836,  834,
  838,  837,  828,    0,  830,  828,  828,    0,    0,    0,
    0,  834,    0,  834,  836,  828,  826,    0,    0,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  828,    0,  826,    0,    0,  825,  836,    0,
    0,  836,  836,    0,    0,  830,    0,    0,  830,    0,
    0,  836,    0,    0,  828,    0,    0,  837,    0,  828,
  837,    0,    0,  830,    0,    0,    0,  837,  836,    0,
  837,  828,    0,  828,  837,    0,    0,    0,    0,  837,
    0,    0,  837,    0,    0,    0,    0,    0,    0,    0,
  836,    0,    0,  825,    0,  836,  825,    0,  837,    0,
    0,    0,    0,  825,    0,    0,  825,  836,    0,  836,
  825,    0,    0,    0,    0,  825,    0,    0,  825,    0,
    0,    0,  837,    0,    0,  837,  837,    0,    0,    0,
    0,  829,    0,    0,  825,  837,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  837,    0,    0,    0,    0,    0,  825,    0,
    0,  825,  825,    0,    0,    0,    0,    0,    0,    0,
    0,  825,    0,    0,  837,    0,    0,    0,    0,  837,
    0,    0,    0,    0,    0,    0,    0,  829,  825,    0,
  829,  837,    0,  837,    0,    0,    0,  829,    0,    0,
  829,    0,    0,  838,  829,    0,    0,    0,    0,  829,
  825,    0,  829,    0,    0,  825,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  825,  829,  825,
    0,    0,    0,    0,    0,    0,    0,  826,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  829,    0,    0,  829,  829,    0,    0,  838,
    0,    0,  838,    0,    0,  829,  830,    0,    0,  838,
    0,    0,  838,    0,    0,    0,  838,    0,    0,    0,
    0,  838,  829,    0,  838,    0,    0,    0,    0,    0,
    0,    0,    0,  826,    0,    0,  826,    0,    0,    0,
  838,    0,    0,  826,  829,    0,  826,    0,    0,  829,
  826,    0,    0,    0,    0,  826,    0,    0,  826,    0,
    0,  829,  830,  829,  838,  830,    0,  838,  838,    0,
    0,    0,  830,    0,  826,  830,    0,  838,    0,  830,
    0,    0,    0,    0,  830,    0,    0,  830,  597,    0,
    0,    0,  598,    0,  838,    0,    0,    0,  826,  797,
    0,  826,  826,  830,    0,    0,    0,    0,    0,    0,
  599,  826,    0,  600,    0,    0,  838,    0,    0,    0,
    0,  838,    0,    0,    0,    0,    0,  830,  826,    0,
  830,  830,    0,  838,  597,  838,    0,    0,  598,    0,
  830,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  826,    0,    0,    0,  601,  826,  599,  830,    0,  600,
  602,  603,  604,    0,  605,  606,  607,  826,  608,  826,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  830,
    0,    0,    0,    0,  830,    0,    0,    0,  609,    0,
  610,    0,    0,    0,    0,    0,  830,    0,  830,    0,
  601,    0,    0,    0,    0,    0,  602,  603,  604,    0,
  605,  606,  607,    0,  608,    0,    0,    0,    0,    0,
    0,  611,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  609,    0,  610,    0,  612,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  613,    0,    0,    0,    0,    0,    0,  611,    0,    0,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,  612,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  614,  615,    0,    0,    0,
    0,    0,    0,    0,  106,  107,  613,  108,    0,    0,
    0,    0,  616,    0,  109,  110,  111,  112,  113,    0,
  114,  105,    0,  115,  116,  117,  118,  119,    0,    0,
  133,  120,  121,    0,  133,    0,    0,    0,    0,    0,
    0,  614,  615,    0,    0,    0,    0,    0,    0,    0,
  106,  107,  133,  108,    0,  133,    0,    0,  616,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,  120,  121,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  133,    0,    0,    0,
    0,    0,  133,  133,  133,    0,  133,  133,  133,    0,
  133,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  453,    0,    0,    0,    0,
  133,    0,  133,    0,    0,  453,  453,  453,  453,  453,
  453,  453,  453,  453,  453,  453,  453,  453,  453,  453,
    0,    0,  453,  453,  453,  453,    0,  453,    0,    0,
  453,  453,    0,  133,  453,  453,  453,  453,  453,  453,
  453,  453,    0,  453,  453,  453,  453,  453,    0,    0,
  133,  453,  453,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  133,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  133,  133,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  133,
};
static short yycheck[] = {                                       9,
   16,   55,   12,    0,    4,  151,   16,   64,   18,  256,
  104,  287,  140,  141,   63,   56,  140,  141,  515,   70,
  355,   63,  363,  712,  498,   62,   67,  363,   85,   57,
  556,  359,   94,  559,  334,   89,  565,  423,  522,  565,
  332,   69,   93,   53,   54,   55,   56,   57,   58,   90,
   73,  677,  941,   63,   64,   65,   97,   67,  989,   69,
   70,  772,   72,   73, 1045,  689,  473,   83,  642,  758,
   68,   81,   95,   83,   84,   85,   92,   87,   88,   89,
   90,  697,   92,   93,   94,   95, 1266,   97,  264,  131,
  979,  133,  154,   91,   88, 1038, 1038,  273,   91, 1038,
  765,  380, 1045, 1045,  132,    0, 1045,  649,    0, 1038,
  726,  152,  281,    0,    0, 1071, 1045, 1038,  368,  730,
    0,  131,  132,  133, 1045,  153,  833,    0,    0,  794,
    0,   45,    0,  327,  228,  145,  762,  864,  803,   44,
   44,  151,  152,  153,  154, 1208,    0,  768,  158,  479,
  989,  145,  189,    0,   41,   41, 1094,   44,   44,   59,
   41,  308,  308,  711,   59,   40,   44,   59,   41, 1046,
   61,   44,   59,   59,  338,   41,  423,  214,    0,   59,
 1057,   40,    0,   44,   44,    0,   59,   59,   63,   59,
    0,   59,  857,  655,    0, 1473,   40,    0,   40,   44,
 1077,  158,  271,   45,  532,   59, 1095,  542,  543,  544,
  545,    0,   59,   45,   45,    0,  258,   40,    0,   41,
 1264,  315,   44,   44,  565,   44,   41, 1224,   40,  565,
   44,  264,   12,    0,  262,  319,   41,   59,  292,  161,
    0,   59,  739,  271,   59,  274,   45,    0,  258,   59,
   91,   63,  262,   59,  264,  265,   59,   44,  455,   41,
 1712,  271,   44,  273,  592,  301,  308,  517,  296,   63,
   59,  278,  348,  265,   59,  260,  568,   59,  442,  283,
  355,  357,  292,  907,  908,   41,  296,  405,   44,  465,
  320,  333,   59,  405,  820,   45,  402,   41,  340,   59,
  342,    0,  344,  262,  318,  347,   59, 1201,  257, 1203,
  302, 1292,  319,   63,  938,  366,  400,    0,  597,  356,
  273,  358,  376,  333,  365, 1314,  618,  473,    0,   41,
  340,    0,  342,  283,  344,  372,    0,  347,  456,  357,
  260,  301, 1298,    0,  456,  337,  374, 1012,  219, 1292,
 1292, 1803,    0, 1292,  980,  365,  366,  441,  138,    0,
   59,  972,    0, 1292,  374,    0,  376,  841,  287,   41,
    0, 1292,   41,  984,  985,   44,   59,  318,  411,   12,
  384,  300,   44,    0,   41,  334, 1225,   59,   41,   41,
   59, 1056,   44,  400, 1438,   59,   44,  319,    0,  279,
   41,  677,   59,  733, 1281,  259,  442, 1404, 1588,  300,
  264,   59,  481, 1037,  424, 1384,  270,  492,   59, 1697,
  300,   59,  884,  464,   59,  269,  409,  386,    0,   59,
 1399,  473,  417,  461,  441,  439,  277,  466,  479,  453,
  311,  517,   59,  304,  472,  318,   41,  694,  476,  329,
  264,  461,  456,  481,  464,  465,  278,   59,  280,  583,
  320, 1524,  472,  710,  740,  287,  476,  340,  348,  479,
  340,  481,   41,  318,  484,   41,  356,  357, 1099,  264,
    0,  328,  442,  363,  331,  304,  762,   59, 1477,  439,
  782, 1115, 1116,  785, 1118, 1119,  383,  417,  258,  517,
  537,  381, 1209,  318,  506,  138,  387,  799, 1402, 1689,
  410,  271,  453,  393, 1088,  395,   59,  409,  328,  279,
  386,  331,  340,  565,   44,  340,  279,  484, 1484,  318,
 1241,    0, 1470,    0,  826,  394,  318,  340,  418,   59,
  300,  436,  410,  397,  458,  721,  306,  300,  308,  586,
  454,  340, 1429,  458,  564,  565,  378, 1434,  340, 1436,
  570,  383, 1104,  443,  331, 1745,    0,  736,  868,  329,
  977,  485,   41, 1412,  452,   44,  329, 1304,  400, 1127,
 1287,  454,  454,  458,  454,  407,  584,  781,  348,  431,
   59,  584,   59,  440, 1471,  348,  356,  357,  877,  641,
 1489,  638,  639,  363,  357,  855, 1786,  454, 1219,  278,
  363,  280,  759,  759,  501,  501,  458,    0,  287,  441,
  744,  381,  274, 1247,  447,   59,  458,  458,  381,  502,
  440,  641,  431,  454,  440,  440,  454,  517, 1303,  454,
  650,    0,  452,  387,  454, 1269,  458,  264,  454,  436,
  650,  454,  396,  777,  778,  411,  309,  309,  418,  458,
 1537,   41,  264,  261,  458,  454, 1543,  262,  367,  429,
  680,  713,  454,  440,  386,   41,   59,  318,  342,  501,
  328,  431,   41,  331,  367,   44,  680,  454,    0,  387,
 1025,  562,  733,  262,  704,  343,   41, 1089,   41,  340,
   59, 1212,  712,  713,  980,  340,  716, 1259,  458,  378,
  348,  721,   41, 1424,    0,  387, 1593,  759,  348, 1335,
  730,  383,  764,  733,  396,  753,  768,  755,  738,  458,
  387,  400,  742, 1610,  771,  772,  773, 1718,  407,    0,
 1709,  405,  440,  753,  408,  755,  387,   59,  758,  791,
  286, 1043,  262, 1305,  764, 1090,  301,  517,  768,  411,
  424,  632, 1038, 1055,  517,  331,  262,  806, 1699, 1045,
  342,  458,  441,   59, 1400, 1718, 1718,  819,  820, 1718,
  397,  791,  780, 1267, 1076,  961,  450,  780,  964, 1718,
 1414,  386,  440,  338,  831,  397,  397, 1718,   59,  440,
 1311,  321, 1102, 1189,  452,  377,  454,  260,  328,  819,
  820,  331,  320,  454,  466,  335, 1290,  386,    0, 1696,
  818,  387, 1296,  343,    0,  818,    0,  825,  856, 1706,
  396,  977,  501,  405, 1363,  436,  408, 1223,  866,  359,
  838,    0,    0,    0, 1236,  279,  856,    0,    0,  318,
  303,  377,  424,  851,  852,  853,  866,   42,  851,  852,
  853,  258,  331,  383, 1416, 1165,  300,  262,  878,  470,
  308,  340, 1749,  313,  271,  342,  386,   59,  450,  300,
  318,  264,  262,   59,   41,   59,  322,   44,   41,    0,
  386,   44,  268,  413,  348,  329,  262,  442,  418,    0,
   59,   59,   59,  965,    0,  264,   59,   59,  356,  306,
  377,  308,  963, 1425,  348,  322,  277,  262,  387,  262,
  440,  962,  448,  357, 1611,  461,  462,  396,  436,  363,
   41,  352,  452,   44,  454,  977, 1212, 1179,  405,  435,
 1182,  408, 1189,  966,  401,    0,  258,  381,   59,  263,
 1074,  961,  962,  963,  964,  965,  966,  424,   59,  271,
 1472,  320,  972,   59,    0,   40,  730,  279,   42,  257,
    0,  440,  343,   47,  984,  985, 1223,  417,  264,  989,
 1308,  429,  308,  450,  345,  454,  273, 1279,  300,  365,
    0,  386,  318,    0,  306,   40,  308,  368,  312,  328,
  445,  260,  331,  264,   59,   16,  386,  461,  462,  323,
 1049, 1050, 1699,  403,  397,  453, 1292,  329, 1310, 1545,
  386,  418, 1363,   59, 1648, 1522,  383, 1363, 1525,   59,
  420,   41,    0,    0,   41, 1311,  348,   44, 1801, 1802,
  435,  386, 1666,  386,  303,  357,  334,  383, 1600,   59,
 1092,  363,   59,  340, 1606,  426, 1066, 1099,  387, 1451,
 1099,   42,   43,  508,   45, 1752,   47,  396, 1068,  381,
    0, 1081,   83,   41,   41,  260,   44, 1469,  352, 1207,
 1208, 1209, 1092,  517,  258, 1083, 1421, 1776,  264, 1099,
 1083,   59,   59,  452,   44,  655,  278,  271,  280, 1862,
  660,  460,  493, 1101,  378,  264,  418,  264, 1101, 1107,
 1797,  397,  503,  427, 1107, 1612,   44, 1741,  303, 1243,
  278, 1245,  280,  402, 1400,    0,  278,  453,  280,   59,
  489,  490,  306,  492,  308, 1377,  397,  307,  308, 1381,
  499,  500,  501,  502,  503, 1699,  505,  258, 1440,  508,
  509,  510,  511,  512,  307,  308,  385,  516,  517,  438,
  271,  318,  258,  264,  321,   93,   41,  417,  279,   44,
  265,  328,    0,    0,  331,  271, 1757, 1758,  335, 1643,
  383, 1645, 1306,  340,   59,   44,  343, 1479,    0,  300,
    0, 1201,  260, 1203,  444,  306,  378,  308, 1752, 1701,
   59,  325,  359,  258, 1241,  517,  280,  302,  972, 1219,
  306,  285,  308,   41,   41, 1225,  271,   44,  329,    0,
  984,  397,  260,    0,  279, 1849,  383,    0,  264,   41,
  387,   59,   59,    0,  264,  303,  383,  348,  397,  396,
   44, 1705,    0, 1797,  418,  300,  357,   59, 1750,   59,
  272,  306,  363,  308,  307,  308,  413,  410,    0, 1756,
   41, 1407,  402,   44,   41,  303,  288,  280,   41, 1850,
  381,  278,  285,  280,  329, 1563, 1518,   44,   59,    0,
  287,  451,   59,  440, 1321, 1285,   59, 1433, 1325,   93,
  436,    0,   59,  348, 1796,  452,  397,  454,  451,  280,
  494,   59,  357,  460,  285,  445,  430,  418,  363,  321,
  278,  333,  280,  437,  258, 1607,    0,   59,  328,  287,
  880,  331,  418,  335,  884,    0,  381, 1829,  258, 1710,
 1711,  343,  489,  490,  337,  492, 1800,  301,   59,  320,
  362,  271,  499,  500,  501,  502,  503,  359,  505,  279,
   59,  508,  509,  510,  511,  512,  418,  256,  337,  516,
  517,  397,  306,  418,  331,    0,   41,  397,  508,   44,
  300,  378,  417, 1754,  338,   59,  306,  387,  308,  278,
  465, 1527,  467, 1432,   59,  279,  396, 1424,  287,    0,
 1432, 1433, 1402,  400,  260,  374, 1524, 1407,  451,  329,
  407,  413, 1412,  278,  274,  280,  517,  429,  387,  284,
  378,    0,  287, 1441,  318,  460, 1540, 1563,  348,  318,
  387,    0, 1432, 1433,   59,  300,   44,  357, 1201,  396,
 1203, 1441,  400,  363,  441,  329,    0,  303,   40,  407,
    0,    0, 1718,  407,  489,  490,    0,  492,   59, 1830,
 1863,  381, 1865,  417,  499,  500,  501,  502,  503,   44,
  505,    0,  517,  508,  509,  510,  511,  512,  294,  363,
   59,  516,  517,  441,  418,   93, 1723,   41,  442,  305,
   59,   41,  424,    0,  461,   44,  312,  381,  418, 1546,
    0,  318,  436, 1781,  501,   59,  259, 1591,  264,   59,
   59,  328,    0,  378,  331,   59,  318,  270,   93,   42,
   43,  337,   45,  340,   47,  377,  343, 1527,  417,  331,
   59, 1563,  421,  422,  423,  400,  470, 1815,  340, 1675,
    0,   41,  407,  501,   44,    0, 1546,  458, 1592,    0,
  321,  318,   59,   41, 1554,  318,   44,  328,   41,   59,
  331,   44,    0, 1563,  335,  328,  418, 1845,  331, 1587,
  387,   59,  343,  340,  485,  417,  441,  340, 1856,  396,
   41, 1859, 1582,   44,    0,  387,  402, 1587,  359, 1616,
 1868,   40, 1592, 1620,  396,    0,  485,  517,    0,   59,
  489,    0,  444,  321,   59, 1611, 1690,  405,   59,    0,
  356, 1611,  383,  465,  316,  317,  259,  335,  434,  316,
  387,   59,  438,  440,  387,   41,  424,  270,  318,  396,
 1661,    0,   93,  396, 1770,  452,  321,  454,  440, 1775,
  460,  359,  413,   59,    0, 1781,    0,  393, 1784,  395,
  335,    0,  454,  318,   59,   41,  321,   59,   44,  337,
   59, 1661,   40,  328,   42,   43,  331,   45,   59,  440,
  335,  301,  418,  440,  359,  340, 1812,  440,  343, 1815,
   58,  452,  336,  454,    0,   63, 1733,  454,  321,  452,
   59,  454,    0, 1699,  359,  413, 1683,  443,  383, 1699,
 1770, 1837,  335,   59,  402,   59, 1842,   93,  338, 1845,
   59, 1717, 1712,   41, 1784,  452,   44, 1717,  383,    0,
 1856,  386,  387, 1859,   42,   43,  359,   45,  413,   47,
  342,  396, 1868, 1733, 1734,   42,   43,   44,   45, 1675,
   47,   59, 1812,   59,   44, 1735, 1752,    0,  413, 1781,
  383,   59, 1752,   42,   43, 1761,   45,  280,   47,   59,
   41, 1761,  285,   44,  318,  377, 1810, 1837,  318,    0,
  435,  660, 1842,    0,  328,  440, 1776,  331,   59,  328,
  413, 1781,  331, 1815, 1675,    0,  340,  452,  677,  454,
  340, 1797,  681,  405,  343,  318,  408, 1797,    0,  688,
  340, 1807,    0, 1803,  693,  694,   59, 1807,   41,  320,
 1810,   44,  424, 1845,  703, 1815,  705,  706,  707,  708,
  709,  710,  711,  258, 1856,  378,  261, 1859,   59,   40,
  318,  318,   59,  387, 1770, 1835, 1868,  387,  450, 1775,
  328,  238,  396,  331,   59, 1845,  396,  400, 1784,  246,
   41,  740,  340,   44,  407,  343, 1856,   59, 1858, 1859,
  337,   59,   46, 1863,  282, 1865,  879,  402, 1868,  383,
  883,  306,  290,  762,   58,    0, 1812,   61,  767, 1770,
   41,  383,  260,   44, 1775,  506,  440,  342,  441,  267,
  440,  440,   40, 1784,  493,  494,    0,  374,  276,  387,
  454, 1837,  318,  452,  454,  454, 1842,  361,  396,  321,
  387,  289,  328,  450,   46,  331,   41,  295,  296,  461,
  462, 1812,  377,  335,  340,  303,   58,   42,   43,   61,
   45,  343,   47,  436,   59,  434,   40,   41,   42,   43,
   44,   45,   46,   47,  384,  323, 1837,  359,  326,  417,
  405, 1842,  440,  408,  516,   59,   60,   61,   62,  454,
   46,  442,  280,  287,  452,  516,  454,  285,  417,  424,
  411,  387,   58,  280,  352,   61,  354,  355,  285,   41,
  396,   41,   44,   41,   44,  469,   44,   91,  366,   93,
   41,  280,  370,   44,   44,  450,  285,   42,   43,  372,
   45,  413,   47,  892,  893,  894,  391,  392,  897,  898,
  899,  460,  901,  902,  903,  904,  905,  906,  461,  462,
  909,  910,  340,  912,  440,  914,   61,  286,  917,  918,
  919,  920,  921,  922,  342,   40,  264,  461,  454,  402,
  489,  490,   40,  492,   40,  934,   40,  936,   40,  427,
  499,  500,  501,  502,  503,  433,  505,   40,   40,  508,
  509,  510,  511,  512,   40,  443,   40,  516,  517,  377,
   40,  434,   40,   42,   43,   58,   45,   40,   47,  457,
  458,  459,  460,  461,  462,  461,  449,  461,   40,   42,
   43,  980,   45,  471,   47,   41,   41,  405,   44,   44,
  408,   40,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   40,  492,   40,  424,  495,  496,   40,
  498,  499,  500,  501,  502,  503,  504,  505,   40,   44,
  508,  509,  510,  511,  512,   91,  284,   46,  516,  517,
   42,   43,  450,   45,   41,   47,   41,   44,  436,   44,
   41,   42,   43,  320,   45,   44,   47,  402,  262,   44,
  264,  265,  457,  458,  459,  280,   41,  320,  272,   44,
  285,   41,   42,   43,  264,   45,  280,   47,  897,   44,
  899,  285,  901,  902,  288,  904,  905,  906,  428,   46,
  485,  486,  260,  318,  676,   61,  678,   40,  302,  264,
  682,  683,  684,  307,  308,    0,  331,  311,   41,   41,
   41,   44,   44,   44,  318,  340,  320,  321,   41,    0,
  324,   44,   44,  382,  328,  340,  505,  331,  348,  333,
  348,  335,  348,  301,   44,  280,  340,  341, 1127,  343,
  285,  278,  278,  300,  264,  313,   41,  397,  436,   44,
 1139, 1140,  470, 1142, 1143,  359,  360,  397,  362,  458,
   41, 1150,  387,   44,   59,  277,  311,  348,  447,  447,
  338,  396,  376,  399,  378,  379,  380,   40,   59,  383,
 1169,   40,  386,  387,   91,  349,   40,   40, 1177, 1178,
 1179,  318,  396, 1182,  383, 1184,  265,  264,  411, 1188,
 1189, 1190, 1191,   44,  389,  382,  410,   40,   40,  413,
  451,  280,  460,   41,  436,  440,  285,  464,  317,   40,
  320,  452,  264, 1212,  448,  429,  502,  280,  458,  454,
  320,  435,  285,  302, 1223,   40,  440,  277,  274,  407,
   40,  489,  490,  300,  492,  278,   44,  451,  452,  417,
  454,  499,  500,  501,  502,  503,  460,  505,  284,  344,
  508,  509,  510,  511,  512,  344,  418,  320,  516,  517,
  386,  386,  386, 1262,  442,  416,  300,   40,  280,  418,
   40,  356,  274,  285,  417,  489,  490,    0,  492,  280,
   40,  417,  264,   41,  285,  499,  500,  501,  502,  503,
   44,  505,  320,   44,  508,  509,  510,  511,  512,  311,
  280,  280,  516,  517,   44,  285,  367,   44,   44,   41,
   42,   43, 1311,   45,  385,   47,  320,  328,   41,   42,
   43,   44,   45,   46,   47,   41,   42,   43,   40,   45,
   44,   47,  266, 1332,   40,  299,   59,   60,   61,   62,
   42,   43, 1341,   45,  501,   47,   40,  422,  264, 1348,
   42,   41,   41,   42,   43,   58,   45,   41,   47,  472,
  473,  474,  475,  476,  477,  478,  479,  460,   91,   41,
   93,   42,   43,  278,   45,  280,   47,   41, 1377,  264,
  454,  440, 1381,  309,  394,  492,   42,  278,   59,  280,
  309,   41,   41,   40,   40,  352,  489,  490,  460,  492,
   40, 1400,   40,   40,   40,   40,  499,  500,  501,  502,
  503,   40,  505,   40, 1413,  508,  509,  510,  511,  512,
   40,   40,   40,  516,  517,   40,   40,  489,  490,  460,
  492,   40,   40,  262,  308,  435,   41,  499,  500,  501,
  502,  503,   44,  505,  327,  343,  508,  509,  510,  511,
  512,  388,  388,  388,  516,  517,  274,  422,  489,  490,
  331,  492,    0,  390,  271,  446,  391,  287,  499,  500,
  501,  502,  503,  378,  505,  386,  407,  508,  509,  510,
  511,  512,  386,  386,  386,  516,  517,  378,   41,   42,
   43,  386,   45,  386,   47,  400,   41,  378, 1497, 1498,
   41,  278,  407,   41,   40,  417,   44,   41,   41,  400,
   41,   41,  320,  308,  435, 1514,  407,  385,  436, 1518,
  396,   59,  338,  280,  337,  301,   41,   42,   43,  442,
   45,   41,   47,  442,  338,  301,  441,   41,  352,  262,
  320,  264,  265,  371,  385,  371,   45,   41,  280,  272,
  441,   41, 1551,  285,   41,   41,   41,  280,   41,   41,
  262,   41,  285,   41,  280,  288,   41,   41,   41,  285,
   41,   41,   41,   41,   41,   59,  340,   40,  280,  302,
   44,  304,   59,  285,  307,  308,    0,   59,  311,  308,
  340,  280,   41,   59,   59,  318,  285,  320,  321,   59,
   59,  324,   58,  352,  296,  328,  344,   40,  331,  280,
  333,   40,  335,  368,  285,   59, 1615,  340,  341,  451,
  343,   40,  383,   44, 1623,   59,   46,   41,   42,   43,
   44,   45,   46,   47,   59,  340,  359,  360,  383,  362,
  383,   41,  343,   41,   41,   59,   60,   61,   62,   44,
 1649,  410,  460,  376,  435,  378,  379,  380,  301,  442,
  383, 1660,  387,  386,  387, 1664,   40,   59,  337,  463,
  304,   44,  293,  396,   41,   42,   43,   91,   45,   93,
   47,  489,  490,  307,  492,   44,   41,  410,  304,   40,
  413,  499,  500,  501,  502,  503,   41,  505,    0,   40,
  508,  509,  510,  511,  512,  337,  429,  383,  516,  517,
  383,  278,  435,  264,   41,   42,   43,  440,   45,  458,
   47,  264,  389,  320, 1723,  264,  470,  280,  451,  452,
  411,  454,  285,  451,   44,   40,  309,  460,  458,  271,
  278,  264,  280,  436,  435,  343,  284,  343,  343,  287,
   41,   42,   43,  343,   45,  271,   47,   41,  458,   41,
   40,   40,  300,   41,   59,  280,  489,  490,  460,  492,
  285,   59,   41,  308,   59,   41,  499,  500,  501,  502,
  503,   41,  505,  304, 1783,  508,  509,  510,  511,  512,
    0,   59,    0,  516,  517,    0, 1204,  489,  490,   12,
  492, 1104, 1102,  265,  460,  645,  721,  499,  500,  501,
  502,  503,  273,  505,  266,  869,  508,  509,  510,  511,
  512, 1099,   87, 1091,  516,  517,   41,   42,   43, 1459,
   45, 1460,   47,  489,  490,  330,  492, 1081,  360,  592,
  378,  764,  323,  499,  500,  501,  502,  503,  262,  505,
  264,  265,  508,  509,  510,  511,  512, 1028,  272,  313,
  516,  517,  400, 1092,    0, 1658,  280, 1582, 1743,  407,
 1438,  285,  801, 1440,  288,  570,   41,   42,   43, 1747,
   45,    0,   47, 1788,  342,  342, 1746, 1260,  302, 1723,
    0,  791, 1554,  307,  308, 1739, 1782,  311, 1858, 1680,
 1014, 1417, 1238,  441,  318,   41,  320,  321,   44, 1699,
  324,  347,  335,  280,  328,  572,  328,  331,  285,  333,
  848,  335,   41,   59, 1294,   44,  340,  341,   13,  343,
  840,   41,   42,   43,   44,   45,   46,   47,  300,  500,
   59,  504,   61,  631,  753,  359,  360,  143,  362,   59,
   60,   61,   62,  280,  740,  767,  989, 1649,  285,  458,
 1535,  460,  376, 1761,  378,  379,  380, 1807,  515,  383,
 1249, 1184,  386,  387,   -1,  981, 1141,  729,   -1,   58,
   -1,   91,  396,   93,   -1,   -1,   -1,   -1,   -1,  280,
  489,  490,   -1,  492,  285,   -1,  410,   -1,   -1,  413,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  429,   -1,  516,  517,   -1,
   -1,  435,   -1,   41,   42,   43,  440,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,
   41,   42,   43,   -1,   45,   -1,   47,   41,   42,   43,
   -1,   45,   -1,   47,   -1,  280,   -1,   -1,    0,   -1,
  285,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,  280,   -1,   41,   42,   43,
  285,   45,   -1,   47,   -1,   -1,   -1,   59,   -1,   61,
   -1,   41,   42,   43,   -1,   45,   -1,   47,   -1,  258,
   42,   43,  278,   45,  280,   47,   -1,   -1,   -1,   -1,
   -1,  287,  262,   -1,  264,  265,   -1,   -1,  277,  278,
   -1,  280,  272,   -1,  300,  284,   -1,   -1,  287,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  300,   -1,   -1,   -1,    0,   -1,  306,   -1,   -1,
   -1,   -1,  302,   -1,    0,   -1,   -1,  307,  308,   -1,
   45,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   41,   -1,   -1,   44,
  340,  341,   -1,  343,   -1,   41,   42,   43,   44,   45,
   46,   47,  378,   -1,   59,   -1,   61,   -1,   -1,  359,
  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,  378,
   -1,   -1,  280,   -1,  400,   -1,  376,  285,  378,  379,
  380,  407,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,  400,   -1,   -1,   59,   91,  396,   93,  407,  280,
   -1,   -1,   -1,   -1,  285,   -1,  280,  416,   -1,  418,
  410,  285,   -1,  413,   -1,  441,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  436,   -1,  429,
   -1,   -1,  441,   -1,   -1,  435,  258,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  451,  452,   -1,  454,  277,  278,   -1,  280,   -1,
  460,  470,  284,   -1,   -1,  287,  280,   -1,   -1,   -1,
  262,  285,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,
  280,   -1,   -1,   -1,  306,  285,   -1,   -1,  280,  489,
  490,  460,  492,  285,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,  258,   -1,   -1,  378,   -1,   -1,   -1,
  295,   -1,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,
   -1,   -1,   -1,  278,   -1,  280,  272,   -1,  400,  284,
   -1,   -1,  287,   -1,  280,  407,   -1,   -1,  323,  285,
   -1,   -1,  288,   -1,   -1,  300,  418,    0,   -1,   -1,
   -1,  306,   -1,   -1,   -1,   -1,  302,   -1,    0,   -1,
  279,  307,  308,   -1,  436,  311,   -1,  352,   -1,  441,
   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,
   -1,  300,  328,   -1,   -1,  331,   -1,  333,   41,  335,
   -1,   44,   -1,   -1,  340,  341,   -1,  343,  470,   41,
   42,   43,   44,   45,   -1,   47,   59,   -1,   61,   -1,
  329,   -1,   -1,  359,  360,   -1,  362,   59,   60,   61,
   62,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,  348,
  376,   -1,  378,  379,  380,   -1,   -1,  383,  357,   -1,
  386,  387,  427,   -1,  363,  400,   -1,   -1,   -1,   -1,
  396,   93,  407,   59,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,  381,  418,  410,   -1,   -1,  413,   -1,   -1,
   -1,   -1,  457,  458,  459,   -1,  461,  462,   -1,   -1,
   -1,  436,   -1,  429,   -1,   -1,  441,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,  480,  481,  482,  483,  484,
  485,  486,  487,  488,   -1,  451,  452,   -1,  454,   -1,
  495,  496,   -1,   -1,  460,  470,   -1,   -1,   -1,  504,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,  460,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  517,   -1,
   -1,  516,  517,   -1,   -1,  258,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,  274,  264,  265,   -1,   -1,   -1,   -1,   -1,    0,
  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,   -1,   -1,  288,  300,   -1,   -1,
   -1,   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,
  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,
   41,   -1,   -1,   44,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,    0,   59,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,  410,   -1,
   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,  451,
  452,   -1,  454,  466, 1333, 1334,   -1,  470,  460,   -1,
 1339, 1340,   -1, 1342, 1343,   -1,   -1,   -1,   -1,   -1,
   -1, 1350, 1351, 1352, 1353,   -1, 1355, 1356, 1357, 1358,
 1359, 1360,   -1,   -1,   -1,   -1,   41,  489,  490,   44,
  492,   -1,   -1,   -1,  460,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   59,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,    0,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,  278,   -1,  280,
  516,  517,   -1,   -1,   -1,   -1,  287,   -1,   -1,   41,
   42,   43,   44,   45,  262,   47,  264,  265,   -1,  300,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  302,  278,   -1,  280,   -1,  307,
  308,   93,   -1,  311,  287,   -1,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,  294,  340,  341,   -1,  343,   -1,  378,   -1,   -1,
   -1,   -1,  305,   -1,   -1,    0,   -1,   -1,   -1,  312,
   -1,  359,  360,    0,  362,   -1,   -1,   -1,   -1,  400,
   -1,   -1,  325,   -1,   -1,   -1,  407,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,  396,   44,
   -1,   -1,   -1,   40,   41,  378,   -1,   44,   -1,   -1,
  441,   -1,  410,   -1,   59,  413,   61,   -1,   -1,   -1,
   -1,   -1,   59,  278,   -1,  280,   -1,  400,   -1,   -1,
   -1,  429,  287,  386,  407,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   91,   -1,   -1,  402,
   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,  419,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,  434,  264,  265,   -1,  438,   -1,   -1,  441,   -1,
  272,  489,  490,   -1,  492,   -1,  449,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  288,  505,   -1,   -1,
  508,  509,  510,  511,  512,  468,   -1,   -1,  516,  517,
  302,   -1,    0,  378,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,  400,  328,   -1,   -1,  331,
   -1,  333,  407,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,  441,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,  396,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,  274,
   -1,  413,  277,  278,   -1,  280,   -1,   -1,   -1,  284,
   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,  440,   -1,
   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,  305,  331,   -1,   -1,   -1,  335,   -1,
  312,   -1,   -1,  340,  316,  317,  343,  489,  490,   -1,
  492,   -1,   -1,  325,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  359,  505,   -1,  337,  508,  509,  510,  511,
  512,   -1,   -1,  378,  516,  517,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,   -1,
  387,   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,  396,
   -1,   -1,  407,   -1,   -1,  287,  411,   -1,   -1,   -1,
   -1,   -1,   -1,  418,   -1,   -1,  413,   -1,   -1,   41,
   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,   -1,
  402,  436,   -1,   -1,  272,   -1,  441,   59,   60,   61,
   62,   -1,   -1,  440,   -1,   -1,   -1,  419,   -1,   -1,
  288,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,  466,  434,   -1,  302,  470,  438,   -1,   -1,  307,
  308,   93,   -1,  311,   -1,   -1,   -1,  449,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,  468,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,  359,  360,    0,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,  396,   44,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,  410,   -1,   59,  413,   61,   -1,  460,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   91,  489,  490,   -1,
  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,  501,
  502,  503,  460,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  288,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  258,  396,   93,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,  274,
   -1,  413,  277,  278,   -1,  280,   -1,   -1,   -1,  284,
   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,  440,   -1,
   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,  489,  490,   -1,
  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  359,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  378,  516,  517,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,   -1,
  387,   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,  396,
   -1,   -1,  407,   -1,   -1,  287,  411,   -1,   -1,   -1,
   -1,   -1,   -1,  418,   -1,   -1,  413,   -1,   -1,   41,
   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,   -1,
   -1,  436,   -1,   -1,  272,   -1,  441,   59,   60,   61,
   62,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
  288,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,  466,   -1,   -1,  302,  470,   -1,   -1,   -1,  307,
  308,   93,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,  396,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,   -1,  410,   -1,   59,  413,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,  501,
  502,  503,  460,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,
  272,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,  288,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,  261,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,    0,   -1,  386,  387,   -1,   -1,   -1,   -1,
  282,   -1,   -1,   -1,  396,   93,   -1,  262,  290,   -1,
   -1,   -1,   -1,   -1,   -1,  297,   -1,  299,  410,  301,
   -1,  413,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,
   -1,  313,   40,   41,   -1,   -1,   44,  429,   -1,   -1,
   -1,   -1,   -1,  435,   -1,  327,   -1,   -1,  440,   -1,
   -1,   59,   -1,   -1,   -1,   -1,  338,   -1,   -1,  451,
  452,   -1,  454,  318,   -1,   -1,  321,   -1,  460,   -1,
   -1,   -1,   -1,  328,  318,   -1,  331,  321,   -1,   -1,
  335,   -1,   -1,   -1,  328,  340,   -1,  331,  343,   -1,
   -1,  335,   -1,   -1,   -1,   -1,  340,  489,  490,  343,
  492,   -1,   -1,   -1,  359,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  359,  508,  509,  510,  511,
  512,   -1,   -1,  405,  516,  517,   -1,    0,  383,   -1,
  412,  386,  387,  415,   -1,  417,  418,   -1,   -1,  383,
   -1,  396,   -1,  387,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,  413,   -1,
  442,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,  413,
   -1,   44,   -1,   -1,  262,   -1,  264,  265,   -1,   -1,
  435,   -1,   -1,   -1,  272,  440,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  440,  452,   -1,  454,
  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,  491,
  454,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,
  308,   -1,   -1,  311,  506,  507,   -1,   -1,   -1,   -1,
  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,  410,  331,   -1,  413,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   40,   -1,   -1,
   43,  429,   45,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,  359,  440,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,  460,  328,   -1,  383,   -1,   -1,   -1,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  452,  318,  454,   -1,  321,   -1,
   -1,   -1,  460,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,  359,   -1,  443,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  460,   -1,   -1,  516,  517,
  383,   -1,   -1,   -1,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
  413,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,  267,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,  343,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,  289,  460,   -1,   -1,
   -1,   40,  295,  296,   43,   -1,   45,   -1,   -1,   -1,
   -1,  368,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,  314,   -1,   -1,   63,   -1,  489,  490,   -1,  492,
  323,  434,   -1,  326,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,  460,   -1,   -1,
   -1,  354,  355,   -1,   -1,   40,   -1,   42,   43,  426,
   45,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   58,   -1,  378,  489,  490,   63,  492,
   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,  460,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  417,   -1,   -1,   -1,  421,   -1,
   -1,   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
  513,  514,  515,  516,  517,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,  292,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,  260,   -1,  326,   -1,   -1,
   -1,   -1,  267,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,  289,  354,  355,   -1,   63,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,  366,  303,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   41,   42,   43,  378,
   45,   -1,   47,  443,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   60,   61,   62,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,  421,   -1,   -1,   -1,   -1,   -1,  427,  489,
  490,  366,  492,   -1,  433,  370,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  443,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  427,  492,   -1,   -1,  495,  496,  433,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  443,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   40,  289,   -1,   43,   -1,   45,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,  272,   -1,  314,
   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,  323,   -1,
  285,  326,   -1,  288,   42,   43,   -1,   45,   -1,   47,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  324,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  333,   40,
   -1,   -1,   43,  378,   45,   -1,  341,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,  460,   63,   -1,   -1,  360,   -1,  362,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,  421,   -1,   -1,   -1,
  489,  490,  427,  492,   -1,   -1,   -1,   -1,  433,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  443,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,  429,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  267,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,  314,   -1,   -1,
   -1,   -1,  280,   -1,   -1,   -1,  323,  285,   -1,  326,
  288,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
    0,  508,  509,  510,  511,  512,  267,  354,  355,  516,
  517,   -1,   -1,   -1,   -1,  276,  324,   -1,   -1,  366,
   -1,   -1,   -1,  370,   -1,  333,   -1,   -1,  289,   -1,
   -1,  378,   -1,  341,  295,  296,   -1,   40,   -1,   -1,
   43,   41,   45,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  360,  314,  362,   58,   -1,   -1,   -1,   59,
   63,   61,  323,   -1,   -1,  326,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,  421,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   91,   -1,  354,  355,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
  457,  458,  459,  460,  461,  462,   -1,  378,   -1,   -1,
   -1,  429,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
  421,  508,  509,  510,  511,  512,  427,   -1,   -1,  516,
  517,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   58,   -1,  328,
  471,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,  260,  258,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,  274,   -1,   -1,  277,  278,   -1,
  280,   -1,   -1,  402,  284,   -1,  289,  287,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   40,   -1,
  300,   43,   -1,   45,   -1,  460,  306,   -1,   -1,  309,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  438,
  323,   63,   -1,  326,  443,   -1,   -1,   -1,   -1,   -1,
  449,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,  460,   -1,   -1,  499,  500,  501,  502,  503,  468,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  366,   -1,   -1,   -1,  370,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  378,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,  260,
   -1,  411,  263,   -1,   -1,   -1,  267,   -1,  418,   -1,
   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,  436,   -1,  289,   -1,
  443,  441,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,  471,   -1,
  470,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  366,   -1,   -1,  260,  370,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   58,  427,   -1,   -1,   -1,
   63,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  271,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,  301,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  312,  313,   -1,  315,   -1,   40,
   -1,   -1,   43,   -1,   45,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,  332,   -1,   -1,   58,   -1,  471,
  338,   -1,   63,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
  398,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  417,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,  432,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  442,   -1,   58,   -1,   -1,   -1,
  323,   63,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,  366,  492,   -1,   -1,  370,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
  443,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  366,   -1,   -1,  260,  370,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   58,  427,   -1,   -1,   -1,
   63,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  471,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,
  323,   -1,   44,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   91,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
  443,   -1,   -1,   -1,  295,  296,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  471,   63,
   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  516,  517,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  258,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,   -1,
   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,  300,   -1,
   -1,   -1,  433,   -1,  306,   -1,   -1,  309,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  378,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,  400,   -1,
   -1,   -1,  276,   -1,   -1,  407,   -1,   -1,   -1,  411,
   -1,   -1,   -1,   -1,   -1,  289,  418,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,  352,   -1,
  354,  355,  276,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  352,  433,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  260,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,  417,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,  516,  517,   -1,   -1,  301,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  313,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,  327,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,  338,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   40,   -1,  289,   43,   -1,   45,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,  407,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   41,   -1,  370,   44,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,  489,  490,   58,  492,   -1,   -1,
   -1,   63,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,  260,  508,  509,  510,  511,  512,   -1,  267,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,  354,  355,   40,   -1,
   -1,   43,   -1,   45,  276,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   58,  289,   -1,   -1,
   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,  323,   -1,  331,  326,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
  352,  359,  354,  355,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  457,
  458,  459,  460,  461,  462,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,  413,  271,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,  427,   -1,  435,  516,  517,
   -1,  433,  440,   -1,   -1,   -1,  301,   -1,   -1,   -1,
   -1,  443,   -1,  308,  452,   -1,  454,  312,  313,   -1,
  315,   -1,   -1,  318,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,  332,   -1,  471,
   -1,   -1,   -1,  338,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   40,  289,   -1,   43,
   -1,   45,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,  417,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,  451,   -1,  453,   -1,
   -1,   -1,   -1,   -1,  366,  460,   40,   -1,  370,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  267,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  460,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,  469,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,  323,
  492,   -1,  326,  495,  496,   -1,   -1,  499,  500,  501,
  502,  503,  504,  505,  506,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  267,  516,  517,   -1,   -1,   -1,   -1,
  354,  355,  276,   -1,   -1,   40,   41,   -1,   43,   -1,
   45,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,  352,  433,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,  260,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  301,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,  516,  517,  313,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  338,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,  407,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,  417,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,
  489,  490,   58,  492,   -1,   -1,   -1,   63,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
  291,   -1,   -1,   -1,  295,   -1,  471,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,  339,   -1,
  276,  516,  517,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,
  411,  267,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
  276,   -1,  423,   40,   -1,   -1,   43,   -1,   45,   -1,
  366,   -1,   -1,  289,  370,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,  445,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,  497,  433,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  454,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,  271,  492,   -1,   -1,  495,
  496,  417,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,  427,  508,  509,  510,  511,  512,  433,   -1,   -1,
  516,  517,   -1,   -1,   -1,  301,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,  332,   -1,   -1,   -1,
   -1,   -1,  338,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,
  516,  517,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   58,   59,  398,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  417,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,  432,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  442,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,   -1,  354,  355,   -1,
   -1,   -1,   40,   -1,  460,   43,   -1,   45,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
  273,   -1,   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,  291,   -1,
   -1,   -1,  295,   -1,  471,  298,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,  339,   -1,  276,  516,
  517,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,  276,   -1,
  423,   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,
   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,  497,  433,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,  516,  517,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,    0,
  508,  509,  510,  511,  512,   -1,   -1,  276,  516,  517,
   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,   -1,  323,   44,   -1,  326,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,  273,   -1,  366,   -1,  277,
   -1,  370,   91,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,   -1,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,  339,   59,   -1,  433,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,  443,  353,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,  373,   -1,  375,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  406,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  423,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,  258,
   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
    0,   -1,   -1,  284,   -1,  274,  287,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,  300,
   -1,   -1,  480,  481,   -1,  306,   -1,   -1,  309,   -1,
   -1,  300,   -1,   -1,   -1,   -1,   -1,  306,   -1,  497,
  309,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   91,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  378,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  400,
   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,
  411,  400,    0,   -1,   -1,   -1,   -1,  418,  407,   -1,
   -1,  318,  411,   -1,  321,   -1,   -1,   -1,   -1,  418,
   -1,  328,   -1,   -1,  331,  436,   -1,   -1,  335,   -1,
  441,   -1,   -1,  340,   -1,   -1,  343,  436,   -1,   -1,
   -1,   -1,  441,   41,   42,   43,   44,   45,   46,   47,
   -1,   -1,  359,   -1,   -1,  466,   -1,   -1,   -1,  470,
   -1,   59,   60,   61,   62,   -1,   -1,  466,   -1,   -1,
   -1,  470,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   91,   -1,  273,   -1,   -1,   -1,  277,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  295,   -1,   -1,
  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  262,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  272,   -1,   -1,  452,   -1,  454,    0,   -1,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,
   -1,  349,  350,  351,   -1,  353,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   41,
   -1,  321,   44,   -1,  324,  373,   -1,  375,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   59,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,  406,  359,
  360,   -1,  362,   -1,   -1,   -1,   59,   60,   61,   62,
   -1,   -1,   -1,   -1,   -1,  423,  376,   -1,  378,  379,
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,  262,   -1,  396,  445,   91,   -1,
   -1,   -1,   -1,   -1,  272,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  280,  413,   -1,   -1,   -1,  285,   -1,   -1,
  288,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  429,
   -1,   -1,  480,  481,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   59,    0,   -1,   -1,   -1,   -1,  497,
  318,   -1,  452,  321,  454,   -1,  324,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,
   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  376,   -1,
  378,  379,  380,   -1,   -1,  383,    0,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,
   44,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,  262,
   -1,   -1,  440,   -1,   -1,   59,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,  452,   -1,  454,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,  318,   -1,   -1,  321,
    0,   -1,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,  321,    0,
   -1,  324,   -1,   -1,   -1,  328,   -1,  359,  331,   -1,
  333,   41,  335,   -1,   44,   -1,  262,  340,  341,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,  383,   -1,    0,  386,  387,  359,  360,   -1,  362,
   41,   -1,   -1,   44,  396,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   59,   -1,
  383,  413,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,  396,   41,  321,  262,   44,   -1,   -1,
   -1,   -1,  328,  435,   -1,  331,   -1,   -1,  440,  335,
  413,   -1,   59,   -1,  340,   -1,   -1,  343,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,  429,   -1,   -1,   -1,
   -1,   -1,  435,  359,   -1,   -1,   -1,  440,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  318,   -1,   -1,  321,   -1,  383,   -1,    0,
  386,  387,  328,   -1,   -1,  331,   -1,   -1,  262,  335,
  396,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,   -1,  359,   -1,   -1,   -1,   -1,   59,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,  383,   59,    0,
  386,  387,   -1,   -1,  318,   -1,  452,  321,  454,   -1,
  396,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,
   -1,  335,   -1,   -1,   -1,   -1,  340,  413,   -1,  343,
   -1,   -1,  262,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,  359,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   59,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  452,   -1,  454,  383,
   -1,   -1,  386,  387,   -1,   -1,   41,   -1,   -1,   44,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   59,  262,   -1,   -1,  328,  413,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,    0,
  321,  435,   -1,   -1,   -1,   -1,  440,  328,   -1,  359,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  452,  340,
  454,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  383,  321,    0,  386,  387,  359,   -1,
   41,  328,   -1,   44,  331,   -1,  396,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   59,   -1,
   -1,  262,  383,  413,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,  396,   41,   -1,   -1,   44,
   -1,  262,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,  413,   -1,   59,   -1,  383,   -1,   -1,  386,
  387,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,  435,   -1,   -1,  318,   -1,  440,
  321,   -1,   -1,   -1,   -1,   -1,  413,  328,   -1,    0,
  331,  452,   -1,  454,  335,   -1,   -1,  318,   -1,  340,
  321,  262,  343,   -1,   -1,   -1,   -1,  328,  435,   -1,
  331,   -1,   -1,  440,  335,   -1,   -1,   -1,  359,  340,
   -1,   -1,  343,   -1,   -1,  452,   -1,  454,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,   -1,  262,  359,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,  318,   -1,   -1,
  321,    0,  383,   -1,   -1,  386,  387,  328,   -1,   -1,
  331,   -1,  413,   -1,  335,  396,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,  318,  435,    0,  321,   -1,  359,  440,
   -1,   -1,   41,  328,   -1,   44,  331,   -1,   -1,   -1,
  335,  452,   -1,  454,  435,  340,   -1,   -1,  343,  440,
   59,  262,  383,   -1,    0,  386,  387,   -1,   -1,   -1,
   -1,  452,   -1,  454,  359,  396,   41,   -1,   -1,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   59,   -1,   -1,  262,  383,   -1,
   -1,  386,  387,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  396,   -1,   -1,  435,   -1,   -1,  318,   -1,  440,
  321,   -1,   -1,   59,   -1,   -1,   -1,  328,  413,   -1,
  331,  452,   -1,  454,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,  318,   -1,  440,  321,   -1,  359,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,  452,   -1,  454,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  359,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,  413,   -1,
  321,  452,   -1,  454,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,  262,  335,   -1,   -1,   -1,   -1,  340,
  435,   -1,  343,   -1,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,  359,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,  318,
   -1,   -1,  321,   -1,   -1,  396,  262,   -1,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,  413,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  359,   -1,   -1,  328,  435,   -1,  331,   -1,   -1,  440,
  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,
   -1,  452,  318,  454,  383,  321,   -1,  386,  387,   -1,
   -1,   -1,  328,   -1,  359,  331,   -1,  396,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,  273,   -1,
   -1,   -1,  277,   -1,  413,   -1,   -1,   -1,  383,  284,
   -1,  386,  387,  359,   -1,   -1,   -1,   -1,   -1,   -1,
  295,  396,   -1,  298,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,   -1,  383,  413,   -1,
  386,  387,   -1,  452,  273,  454,   -1,   -1,  277,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,  339,  440,  295,  413,   -1,  298,
  345,  346,  347,   -1,  349,  350,  351,  452,  353,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
  339,   -1,   -1,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,  423,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,  445,  492,   -1,   -1,
   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,
  505,  460,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  273,  516,  517,   -1,  277,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,  295,  492,   -1,  298,   -1,   -1,  497,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,
   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
  373,   -1,  375,   -1,   -1,  470,  471,  472,  473,  474,
  475,  476,  477,  478,  479,  480,  481,  482,  483,  484,
   -1,   -1,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  406,  499,  500,  501,  502,  503,  504,
  505,  506,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  423,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  497,
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
"view_table : derived_table",
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
"derived_table : '(' union_expr ')' as_noise correlation_name derived_column_list",
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
case 367:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 368:
{ yyval = yyvsp[-1]; }
break;
case 369:
{ lex.beginning = lex_position(); }
break;
case 370:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 371:
{ lex.beginning = lex.last_token; }
break;
case 372:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 373:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 374:
{ yyval = 0; }
break;
case 375:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 376:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = NULL; }
break;
case 380:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 381:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 399:
{ yyval = NULL; }
break;
case 400:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 403:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 404:
{ yyval = yyvsp[0]; }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 407:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 408:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 409:
{ yyval = yyvsp[0]; }
break;
case 410:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 412:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 413:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 417:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 421:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 422:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 423:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 424:
{ yyval = yyvsp[0]; }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 427:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 428:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 429:
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
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
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
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
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
case 470:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 471:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 472:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 473:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 478:
{ yyval = NULL; }
break;
case 480:
{ yyval = NULL; }
break;
case 481:
{ yyval = yyvsp[0]; }
break;
case 482:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 486:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 487:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 488:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 489:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 491:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 492:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 493:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 498:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 499:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 501:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 502:
{ if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 503:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 509:
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
case 510:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 512:
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
case 513:
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
case 514:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 521:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 522:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 523:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 524:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 525:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 526:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 528:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 529:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 530:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 542:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 543:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 544:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
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
case 549:
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
case 550:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 551:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 552:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
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
case 564:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 565:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 566:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 567:
{ yyval = 0; }
break;
case 568:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 571:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 572:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 575:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 576:
{ yyval = NULL; }
break;
case 578:
{ yyval = NULL; }
break;
case 579:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 581:
{ yyval = NULL; }
break;
case 583:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 588:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 592:
{ yyval = yyvsp[0];}
break;
case 594:
{ yyval = yyvsp[0];}
break;
case 595:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 596:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 597:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 598:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 599:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 600:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 601:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 602:
{ yyval = 0; }
break;
case 603:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 604:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 605:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 606:
{ yyval = (DSQL_NOD) 0; }
break;
case 607:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 608:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 610:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 611:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 612:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 613:
{ yyval = 0; }
break;
case 615:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 616:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 617:
{ yyval = make_node (nod_select, e_select_count, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 618:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 619:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 620:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 621:
{ yyval = make_list (yyvsp[0]); }
break;
case 622:
{ yyval = 0; }
break;
case 624:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 625:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 630:
{ yyval = 0; }
break;
case 631:
{ yyval = yyvsp[-1]; }
break;
case 632:
{ yyval = 0; }
break;
case 633:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
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
{ lex.limit_clause = true; }
break;
case 642:
{ lex.limit_clause = false; }
break;
case 643:
{ lex.first_detection = true; }
break;
case 644:
{ lex.first_detection = false; }
break;
case 645:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 646:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 647:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 648:
{ yyval = 0; }
break;
case 649:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 650:
{ yyval = yyvsp[-2]; }
break;
case 651:
{ yyval = yyvsp[-1]; }
break;
case 652:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 653:
{ yyval = yyvsp[-1]; }
break;
case 654:
{ yyval = yyvsp[0]; }
break;
case 655:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 656:
{ yyval = 0; }
break;
case 657:
{ yyval = make_list (yyvsp[0]); }
break;
case 658:
{ yyval = 0; }
break;
case 660:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 662:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 665:
{ yyval = make_list (yyvsp[0]); }
break;
case 667:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 671:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 673:
{ yyval = NULL; }
break;
case 674:
{ yyval = make_list (yyvsp[-1]); }
break;
case 675:
{ yyval = NULL; }
break;
case 677:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 678:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 679:
{ yyval = yyvsp[-1]; }
break;
case 680:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 681:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 682:
{ yyval = make_list (yyvsp[-1]); }
break;
case 683:
{ yyval = NULL; }
break;
case 685:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 689:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 690:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 691:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 692:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 693:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 694:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 695:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 696:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 697:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 698:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 699:
{ yyval = make_list (yyvsp[0]); }
break;
case 700:
{ yyval = 0; }
break;
case 702:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 704:
{ yyval = yyvsp[0]; }
break;
case 705:
{ yyval = 0; }
break;
case 706:
{ yyval = yyvsp[0]; }
break;
case 707:
{ yyval = 0; }
break;
case 708:
{ yyval = yyvsp[0]; }
break;
case 709:
{ yyval = 0; }
break;
case 710:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 711:
{ yyval = 0; }
break;
case 712:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 713:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 714:
{ yyval = 0; }
break;
case 715:
{ yyval = 0; }
break;
case 717:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 718:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 721:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 723:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 724:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 726:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 727:
{ yyval = make_list (yyvsp[-1]); }
break;
case 728:
{ yyval = 0; }
break;
case 729:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 730:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 736:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 740:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 741:
{ yyval = make_node (nod_update, e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 744:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 747:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 748:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 749:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 750:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 754:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 755:
{ yyval = yyvsp[0]; }
break;
case 757:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
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
{ yyval = NULL; }
break;
case 766:
{ yyval = make_list (yyvsp[-1]); }
break;
case 768:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 772:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 774:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 776:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 778:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 779:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 782:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 783:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = yyvsp[-1]; }
break;
case 788:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 799:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 800:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 801:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 802:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 803:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 804:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 805:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 806:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 807:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 810:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 813:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 814:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 815:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 816:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 817:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 826:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 827:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 829:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 830:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 831:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 832:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 833:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 834:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 835:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 839:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 840:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 841:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 843:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 844:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 845:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 846:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 847:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 848:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 850:
{ yyval = make_list (yyvsp[-1]); }
break;
case 851:
{ yyval = yyvsp[-1]; }
break;
case 852:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 853:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 863:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 864:
{ yyval = yyvsp[0]; }
break;
case 865:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 866:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 867:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 868:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 869:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 870:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 871:
{ yyval = yyvsp[-1]; }
break;
case 872:
{ yyval = yyvsp[-1]; }
break;
case 876:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 877:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 878:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 879:
{ yyval = yyvsp[0]; }
break;
case 880:
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
case 881:
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
case 882:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 883:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 885:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 887:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 888:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 889:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 890:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 891:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 892:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 894:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 895:
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
case 896:
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
case 897:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 898:
{ yyval = make_parameter (); }
break;
case 899:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 900:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 901:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 902:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 903:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 904:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 905:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 906:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 907:
{ yyval = yyvsp[0]; }
break;
case 908:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 910:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 911:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 912:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 913:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 914:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 916:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 917:
{ yyval = yyvsp[0];}
break;
case 922:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 923:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 924:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 925:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 926:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 927:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 928:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 929:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 930:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 931:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 932:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 933:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 935:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 937:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 938:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 939:
{ yyval = yyvsp[0]; }
break;
case 940:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 941:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 942:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 943:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 946:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 947:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 950:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 951:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 952:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 953:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 954:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 955:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 956:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 957:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 961:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 962:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 963:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 964:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 965:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 966:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 967:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 968:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 971:
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
