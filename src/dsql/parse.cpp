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
 * 2003.06.13 Nickolay Samofatov: Make INSERTING/UPDATING/DELETING non-reserved keywords
 * 2003.07.01 Blas Rodriguez Somoza: Change DEBUG and IN to avoid conflicts in win32 build/bison
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

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE		2048
#define YYMAXDEPTH		2048

/* Make bison allocate static stack */
#define YYINITDEPTH 2048
/* #define YYSTACK_USE_ALLOCA 1 */

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
#define KW_PAGE_SIZE 393
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
  190,  191,  192,  193,  198,  199,  194,  194,  195,  205,
  196,  206,  201,  201,  201,  202,  202,  202,  203,  203,
  203,  203,  197,  200,  200,  204,  204,  187,  187,  211,
  212,  212,  213,  213,  213,  213,   11,   71,   80,  215,
  218,  218,  218,  219,  222,  226,  226,  227,  227,  228,
  228,  147,  217,   87,   89,  216,  216,   70,   83,  231,
  231,  231,  232,  235,  235,  236,  236,  236,  236,  236,
  236,  236,  236,  236,  236,  236,  236,  236,  236,  236,
  233,  233,  234,    2,  238,  238,  238,  238,  238,  238,
  238,  246,  247,  244,  244,  248,  248,  248,  248,  248,
  248,  248,  239,  239,  249,  249,  249,  249,  249,  249,
  249,  243,  254,  254,  254,  254,  254,  254,  254,  254,
  254,  254,  254,  254,  254,  254,  254,  254,  254,  254,
  254,  254,  254,  254,  254,  251,  251,  253,  253,  252,
  250,  250,  250,  245,  245,  241,  242,  242,  256,  256,
  256,  256,  257,  257,  240,  258,  258,  259,  259,    8,
  260,  260,  260,  260,  260,  260,  260,  260,  260,  260,
  260,  260,  101,  101,  144,  144,  263,  263,  266,  266,
  268,  268,   53,   53,  265,  265,  265,  265,  265,  265,
  265,  265,  265,  273,  273,  264,  264,  264,  264,  274,
  274,   61,   61,   61,   55,   55,  270,  270,  270,  267,
  267,  267,  278,  278,  278,  277,  277,  276,  276,  276,
  271,  271,  281,  279,  279,  279,  280,  280,  272,  272,
  272,  272,  282,  282,   18,   18,   18,  284,  284,  284,
   16,   16,   16,  286,  287,  290,  290,  288,  292,  292,
    4,   15,  291,  291,  293,  293,  294,  294,  283,  295,
  295,  296,  296,  297,  297,  297,  297,  298,  298,  299,
  299,  300,  300,  302,  302,  302,  303,  303,  303,  304,
  304,  304,  301,  306,  306,  306,  307,  307,  305,  305,
  308,  310,  310,  309,  309,  285,   17,  311,  311,  311,
  312,  312,  316,  316,  317,   65,   65,   65,  319,  319,
  318,  318,  313,  313,  322,  322,  314,  314,  315,  326,
  327,  328,  320,  321,  323,  323,  323,  323,  329,  329,
  329,  330,  330,  330,  220,  220,  221,  221,  333,  333,
  334,  334,  334,  324,  337,  337,  338,  338,  339,  339,
  340,  340,  341,  341,  208,  208,  343,  343,  229,  229,
   31,  230,  230,  230,  230,  230,  230,  230,  230,  224,
  224,  344,  344,  345,  345,  345,  345,  345,  347,  347,
  347,  225,  225,  223,  223,  325,  325,  351,  352,  352,
  352,  352,  352,  353,  353,  354,  354,  355,  355,  356,
  356,  356,  357,  357,   10,   10,  359,  359,    7,    7,
  360,  361,  362,   19,   19,  363,  364,  365,  365,  188,
  335,  335,    3,    3,  367,  367,  367,  369,  369,   61,
  368,  368,  370,  370,   34,   34,   85,   84,   84,  358,
  358,  371,  372,  372,  209,  209,  209,  146,  366,  366,
  111,  111,  111,  111,  111,  376,  376,  376,  376,  376,
  376,  375,  375,  375,  378,  378,  378,  378,  378,  378,
  378,  378,  378,  378,  379,  379,  379,  379,  379,  379,
  379,  379,  384,  384,  384,  384,  384,  384,  384,  384,
  384,  384,  384,  384,  384,  384,  384,  384,  390,  390,
  380,  380,  381,  381,  381,  381,  382,  382,  386,  386,
  387,  387,  387,  387,  385,  388,  383,  383,  374,  374,
  374,  377,  377,  377,  391,  391,  392,  389,  394,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,  156,  156,  156,  395,  393,
  393,  151,  151,  399,  399,  399,  399,  399,  397,  397,
  397,  397,  397,  331,  152,  152,  153,  154,  154,  154,
  154,  154,   51,   51,   20,   20,  237,  400,   54,  123,
  269,  269,   95,  396,  396,  396,  396,  401,  401,  401,
  401,  401,  401,  401,  401,  401,  401,  401,  402,  348,
  349,  349,  349,  346,  346,  398,  350,  350,  404,  404,
  405,  405,  406,  406,  409,  409,  407,  407,  412,  412,
  411,  408,  410,  403,  403,  403,  403,  403,  403,  403,
  403,  332,  332,  155,   48,  275,  122,  148,  373,  165,
  210,  262,   63,   60,  214,  112,   66,  336,   35,   41,
  342,  261,   39,   42,  207,   40,  289,  255,  255,  413,
  413,  413,  413,  413,  413,  413,  413,  413,  413,  413,
  413,  413,  413,
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
    2,    6,    4,    7,    8,    6,    8,    6,    4,    0,
    4,    2,    1,    3,    0,    2,    4,    0,    1,    1,
    3,    3,    6,    2,    2,    3,    0,    1,    2,    4,
    1,    3,    2,    2,    2,    1,    4,    7,    7,    1,
    1,    3,    4,    7,    2,    1,    3,    1,    1,    6,
    3,    0,    0,    0,    0,    3,    0,    9,    9,    1,
    1,    0,    2,    1,    1,    1,    1,    1,    3,    3,
    3,    3,    3,    3,    5,    5,    5,    5,    5,    5,
    2,    0,    4,    2,    3,    3,    2,    2,    3,    3,
    2,    3,    6,    1,    2,    4,    3,    2,    2,    2,
    2,    3,    1,    3,    3,    3,    2,    2,    4,    4,
    5,    1,    1,    1,    1,    1,    1,    1,    1,    1,
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
    1,    2,    3,    2,    1,    3,    1,    1,    6,    3,
    3,    2,    3,    0,    1,    3,    1,    1,    1,    2,
    1,    1,    1,    2,    1,    2,    1,    2,    0,    3,
    0,    1,    3,    1,    1,    1,    1,    3,    1,    1,
    1,    2,    0,    2,    0,    2,    0,    4,    1,    2,
    1,    1,    0,    1,    3,    2,    1,    1,    2,    1,
    4,    2,    1,    3,    8,    5,    1,    3,    1,    1,
    4,    4,    4,    1,    1,    5,    5,    1,    3,    3,
    1,    1,    7,    7,    5,    3,    0,    1,    1,    1,
    2,    0,    1,    1,    1,    0,    3,    1,    3,    1,
    0,    3,    1,    3,    1,    3,    3,    1,    1,    3,
    1,    2,    1,    3,    3,    1,    2,    3,    3,    3,
    3,    1,    3,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    3,    3,    3,    3,    3,    3,
    3,    3,    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    1,    1,
    5,    6,    3,    4,    5,    6,    3,    4,    3,    4,
    3,    4,    4,    5,    4,    4,    3,    4,    1,    1,
    1,    1,    1,    1,    1,    3,    3,   10,   10,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    2,    2,
    3,    3,    3,    3,    3,    3,    3,    3,    1,    1,
    1,    1,    3,    1,    1,    1,    1,    1,    4,    1,
    3,    1,    2,    1,    1,    1,    1,    1,    1,    1,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    2,    1,    1,    1,    1,
    1,    2,    1,    1,    1,    1,    1,    4,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    6,    6,
    6,    8,    4,    4,    3,    6,    1,    1,    6,    6,
    1,    1,    4,    6,    4,    5,    3,    5,    4,    5,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    0,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  545,
  546,  547,  551,  552,  553,    0,  608,  719,  720,  724,
  725,  456,    0,    0,    0,    0,    0,    0,  394,  563,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  897,    0,   21,  895,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  470,    0,    0,   37,   34,
   36,    0,   35,    0,  978,  980,  982,  981,  987,  983,
  990,  988,  984,  986,  989,  985,  991,  992,  993,    0,
    0,    0,    0,   32,   68,   66,  970,  979,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  637,    0,    0,    0,  669,    0,  972,    0,    0,
    0,    0,  107,  977,  554,    0,    2,    0,    0,    0,
    0,  424,  425,  426,  427,  428,  429,  430,  431,  432,
  433,  434,  435,  436,  437,  438,  439,  440,  441,  442,
  443,  444,  445,    0,  422,  423,    0,  963,    0,  401,
  967,    0,  398,  969,    0,  671,    0,  397,  973,    0,
  561,  893,    0,  150,  103,    0,  104,    0,  217,  959,
  758,    0,  102,  145,  966,    0,    0,   98,    0,  105,
  899,    0,   99,    0,  100,    0,  101,  976,  146,  106,
  616,  617,    0,  898,  896,    0,   74,    0,  964,    0,
  962,  478,  471,    0,  477,  482,  472,  473,  480,  474,
  475,  476,  481,    0,   30,    0,   39,  745,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  903,    0,  884,  631,  631,  632,
  642,  644,  952,  645,    0,  646,  636,  632,    0,    0,
    0,    0,    0,    0,    0,  580,  569,    0,  572,  574,
  575,  576,  577,  583,  584,    0,  971,  670,    0,  108,
    0,  109,    0,  110,    0,    0,    0,  609,    0,    0,
    0,    0,    0,    0,  457,    0,    0,  364,    0,  220,
    0,  404,  395,  454,  455,    0,    0,    0,    0,    0,
    0,  413,    0,  370,  371,    0,  567,  565,  894,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  955,   75,    0,    0,    0,  721,  722,  479,
    0,    0,    0,    0,  862,  890,    0,  954,    0,  864,
    0,    0,  891,    0,  885,  876,  875,  874,    0,    0,
    0,    0,  866,  867,  868,  877,  878,  886,  887,    0,
  888,  889,    0,    0,    0,  892,    0,    0,    0,    0,
    0,  880,    0,  755,  859,  860,  861,  667,  865,  347,
  845,    0,    0,    0,  844,    0,  665,  848,  906,  907,
  847,  841,  842,  843,  846,  879,  904,  905,  927,  928,
  931,  932,    0,  748,    0,    0,   27,    0,    0,    0,
   71,   69,  974,   67,   33,    0,    0,    0,  750,    0,
   52,    0,    0,    0,    0,    0,  559,    0,    0,    0,
  639,  641,    0,  648,    0,  732,    0,    0,  649,    0,
  635,    0,  606,    0,  581,    0,  578,    0,  579,  604,
    0,  599,    0,    0,  573,  759,  728,    0,    0,    0,
    0,    0,    0,  556,  555,    0,    0,  613,  610,    0,
    0,  607,  185,    0,    0,  129,  462,  172,  173,    0,
  460,    0,    0,    0,    0,  463,    0,  458,    0,    0,
  408,  410,  409,    0,  411,    0,  405,  218,    0,    0,
  271,    0,    0,    0,    0,  417,  418,    0,  447,    0,
    0,    0,    0,    0,    0,    0,  375,  374,  466,    0,
    0,  149,    0,    0,  158,    0,  157,    0,  164,  147,
    0,  162,  165,  166,  154,    0,    0,  501,  537,  504,
    0,  538,    0,    0,  505,    0,    0,    0,  528,  541,
  500,  523,  502,  503,  498,  485,  364,  483,  484,  486,
    0,    0,  495,  496,  497,  499,    0,    0,    0,    0,
    0,  112,    0,  113,    0,  124,    0,    0,  201,    0,
    0,  362,    0,  220,    0,    0,   83,    0,    0,  740,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  761,  763,  772,  775,  776,  777,  778,  779,  780,
  781,  782,  783,  784,    0,    0,    0,  881,    0,    0,
    0,    0,    0,    0,  882,  883,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  322,
  975,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  747,   28,    0,    0,   44,   72,    0,    0,
   26,    0,  753,    0,    0,    0,  716,    0,    0,    0,
    0,    0,  558,    0,  631,    0,    0,    0,    0,    0,
  968,  652,  549,    0,  901,  548,    0,  582,    0,  590,
  586,  585,    0,    0,    0,  601,  589,    0,    0,  726,
  727,    0,    0,    0,  362,    0,    0,    0,  623,  627,
    0,    0,  130,    0,    0,    0,    0,    0,    0,  364,
  407,    0,  364,  365,  412,  493,    0,  273,    0,  267,
  269,  362,  240,  960,    0,    0,  364,    0,    0,  209,
  362,    0,    0,    0,    0,  245,  239,  242,  243,  244,
  416,  452,  451,  415,  414,    0,    0,    0,    0,    0,
    0,  373,    0,  364,    0,  155,  159,    0,  163,    0,
    0,  524,  525,  542,    0,  539,    0,    0,  531,  529,
  530,    0,    0,    0,    0,  494,    0,    0,    0,    0,
  532,    0,    0,  362,    0,  200,    0,  203,  205,  206,
    0,    0,    0,    0,    0,  220,   78,    0,    0,   77,
  512,  956,  513,    0,    0,    0,    0,  762,  774,    0,
    0,  832,  833,  834,    0,  766,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  944,
  945,  946,  947,  948,  949,  950,  951,    0,    0,    0,
    0,    0,    0,  937,    0,    0,    0,    0,  857,  324,
  858,  925,    0,    0,  853,  958,    0,    0,    0,    0,
    0,  666,    0,  863,  757,  756,  749,    0,    0,    0,
    0,    0,    0,    0,    0,   53,   61,   63,    0,    0,
   70,    0,    0,    0,  752,    0,    0,    0,    0,    0,
    0,    0,    0,  640,  643,    0,    0,    0,    0,  657,
  658,    0,    0,  650,  653,  550,  902,  591,  600,  595,
  594,    0,  605,  760,  729,  730,  362,    0,    0,  633,
  615,  614,    0,  181,  175,  182,    0,  178,    0,    0,
  177,    0,  188,  169,  168,  900,  171,  170,  464,    0,
    0,    0,  406,  272,    0,  211,  364,  213,    0,  365,
  208,  214,    0,    0,    0,    0,  419,  420,  364,  362,
  365,    0,    0,    0,  391,    0,  196,  197,  156,    0,
    0,    0,    0,    0,    0,  540,    0,    0,  364,  365,
    0,  489,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  198,    0,    0,    0,    0,
  351,    0,   82,    0,    0,    0,    0,   87,   84,    0,
    0,    0,  723,  961,    0,    0,  767,    0,    0,  773,
    0,    0,    0,    0,    0,  810,  809,    0,    0,    0,
  817,  835,  827,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  765,    0,    0,    0,  220,    0,  908,
    0,    0,  923,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  933,    0,  943,    0,    0,
    0,    0,    0,    0,  924,  869,   65,   57,   58,   64,
   59,   60,    0,    0,   22,    0,    0,    0,   42,    0,
    0,    0,  734,  754,    0,  717,    0,  733,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  672,    0,
    0,    0,    0,    0,  597,  598,  602,    0,  199,    0,
    0,    0,    0,    0,    0,    0,    0,  189,    0,    0,
    0,  402,  223,  224,  225,  226,  227,  228,  872,    0,
    0,    0,  276,    0,  221,    0,    0,    0,    0,  246,
    0,  448,  449,  421,    0,    0,    0,    0,    0,    0,
  364,  468,  365,  167,  957,    0,    0,  507,    0,  506,
  543,    0,  535,    0,    0,    0,  487,    0,  515,    0,
    0,  517,  520,  522,  362,    0,    0,    0,  119,    0,
  204,  364,    0,    0,  363,    0,   97,    0,  117,    0,
   93,    0,    0,    0,    0,   86,    0,  825,  826,  769,
    0,  771,    0,    0,    0,    0,    0,    0,    0,  828,
    0,    0,    0,    0,    0,    0,    0,  818,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  914,  913,    0,    0,  910,  909,    0,  918,
  917,  916,  915,  912,  911,    0,    0,  939,    0,    0,
    0,  938,    0,    0,    0,    0,    0,    0,   56,   62,
   54,   55,   23,  738,  739,    0,  736,  743,  744,  741,
    0,    0,  715,    0,    0,  660,    0,  661,    0,  678,
  674,  676,    0,    0,    0,    0,    0,  363,  619,  620,
  634,    0,  183,  179,    0,    0,  194,  190,    0,  365,
  873,  279,    0,    0,  365,  277,    0,    0,    0,  231,
    0,    0,  247,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  465,  509,    0,  510,    0,  134,    0,    0,
    0,  138,  140,  141,  142,  490,  492,  488,  519,    0,
    0,  365,    0,    0,    0,    0,  348,    0,  352,  364,
   88,    0,    0,   90,    0,    0,    0,    0,    0,    0,
  837,  836,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  219,  926,  919,  920,    0,  921,  935,  934,    0,  940,
  929,  930,    0,   40,    0,    0,  718,  663,    0,  533,
    0,  685,    0,    0,  682,  686,  687,  689,  690,  691,
    0,    0,  629,  365,  349,  621,    0,  174,  191,  365,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  284,  289,  283,
    0,    0,    0,  293,  294,  295,  296,  297,  298,  300,
  301,  303,  304,  305,  308,  266,    0,    0,  207,  232,
    0,    0,    0,    0,  233,  236,    0,    0,  365,  389,
  390,  386,  385,  388,  387,    0,  508,  536,  143,  131,
  139,    0,  364,  263,  126,  365,    0,    0,  366,  353,
    0,    0,   91,   92,   79,    0,  796,  804,    0,  797,
  805,  800,  808,    0,    0,  798,  806,  799,  807,  793,
  801,  794,  802,  795,  803,    0,  936,    0,  735,    0,
    0,    0,    0,    0,  699,  701,    0,  696,    0,  264,
  184,  403,    0,  311,    0,    0,    0,  307,    0,    0,
    0,    0,  306,    0,  334,  335,  292,  299,    0,  302,
  285,    0,  290,    0,  338,  291,  210,  215,  234,    0,
    0,  237,    0,  249,  251,  393,  365,    0,  368,    0,
    0,    0,  358,  359,    0,    0,   76,   94,    0,  922,
    0,    0,    0,  688,  683,  700,    0,    0,  282,    0,
  309,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  329,  330,  346,    0,    0,    0,    0,  341,  339,  238,
    0,    0,    0,    0,    0,  265,  365,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  707,    0,    0,
    0,  281,  278,  310,    0,    0,  313,    0,    0,    0,
  319,    0,  321,    0,  345,  344,  965,  343,    0,    0,
    0,    0,    0,  248,    0,  255,    0,  254,  369,  361,
    0,    0,  354,  118,    0,    0,    0,  709,  698,    0,
    0,  710,    0,  706,    0,    0,    0,    0,    0,    0,
    0,    0,  332,  331,  340,  342,  235,    0,    0,  259,
  258,  257,    0,    0,  839,  630,  705,    0,  712,    0,
    0,  312,  316,    0,    0,    0,    0,  333,  262,  260,
  261,    0,    0,    0,    0,    0,    0,  336,  314,    0,
    0,    0,    0,  711,  327,  315,  317,  838,  714,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1544,   28,   29, 1545,
   31,   32,   33,   34,   35,   36, 1546,   38, 1547,  640,
  120,  456,  954, 1185,  121,  706,  955,  122,  460,  711,
  146,  123,  124,  257,  977,  136,  956,  957,  197,  226,
  125,  958,  126,  462,   82,  237,  374,  421,  635, 1086,
  422,  859,  606, 1510,  836,  636,  637,  638, 1088,  238,
 1374,   74,  187,   75,  233, 1824, 1297,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  622,  624,  453,  258,  786,  540,  650, 1023,  367,
  627, 1074, 1289,  573,  735, 1049,  525,  526,  555,  359,
  607, 1060, 1440, 1569, 1232, 1441, 1442, 1443, 1444, 1445,
  651,  214,  206,  355,  580,  356,  357,  581,  582,  583,
  584, 1264, 1017,  528,  529, 1007, 1005,  530, 1011, 1006,
 1222, 1403, 1012, 1013, 1227,  369,  847,  848,  849,  850,
  788, 1030, 1418,  608,  790,  424,  852,  935,  549, 1346,
 1233,  425,  426,  427,  428,  429, 1419, 1420,  558, 1575,
 1723, 1250,  796,  797,  783,  798,  799,  800, 1724, 1725,
 1801,  337,  553, 1241, 1548,  193,  550,  551, 1242, 1243,
 1413, 1700, 1549, 1550, 1551, 1552, 1664, 1553, 1554, 1555,
 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563, 1564, 1565,
  430, 1786, 1710, 1790, 1708,  431,  690,  432,  433, 1093,
 1665, 1717, 1718, 1756, 1079, 1295, 1457, 1080, 1081,  285,
  487, 1598,  378, 1214, 1396, 1681, 1682, 1683, 1684, 1212,
  346,  569,  814, 1262,  570,  812,  221,   59,  341,  198,
  161,  324,  184,  331,  190,  774,  771,  332,  342,  804,
  343,  565, 1041,  185,  434,  325,  537,  571, 1263,   96,
  196,  242,  609,  610,  776, 1061,  777, 1062, 1063,  613,
  614,  615,  616, 1054,  863,  617,  618,  619,  829,  620,
 1512,  826,   40,   41,   42,   43,   44,   45,  155,  515,
   61,  478,  201,  348,  297,  298,  299,  300,  301,  302,
  303,  304,  305,  741,  501,  992, 1217,  502,  503,  746,
   46,  160,  320,  522,   47,  517,  518, 1001, 1401, 1221,
 1526,  759,  140,  728, 1523,  717,  481,  287,  141,  142,
  435,  286,  488,  489,  490,  732,  978,  979,  980,  981,
 1206,  436, 1168, 1514, 1515,  438, 1517,  439,  440,  441,
 1738, 1639, 1739, 1740, 1741, 1784, 1825,  468, 1197,   48,
   49,  379,   50,   51,  509,  510,  963, 1193, 1376, 1380,
  469,  714,  211,  652,  653,  877,  878,  654,  655,  656,
  657,  658,  659,  660,  661,  662,  663,  664, 1318, 1109,
 1111, 1112,  934,  689,  442,  443,  444,  445,  446,  235,
  447,  448,  918,  449,  450,  451,  452,  680,  922, 1169,
 1164,  681,  128,
};
static short yysindex[] = {                                   7393,
 3162,  -93, 3634,  139, 1264,   66, 2945,   84, 3609,  738,
  386, 3124,  -93, 1253,  631, 5124, 1374, 5124,  218,    0,
  699,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  692,    0,    0,    0,    0,
    0,    0,10222, 5124, 5124, 5124, 5124, 5124,    0,    0,
  403, 1336, 5124, 5124, 5124,  612, 5124,  472, 5124, 5124,
    0, 5124, 5124,    0, 1015,    0,  522,    0,    0,  517,
 5124,    0, 5124, 5124, 5124,  765, 5124, 5124, 5124, 5124,
  472, 5124, 5124, 5124, 5124,    0, 5124,  753,    0,    0,
    0, 1120,    0, 1120,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  889,
  898,  111, 1244,    0,    0,    0,    0,    0, 5124, 5124,
 5124,  958,  969,  980,  182,  653,  955,  143,  170,   95,
  923,    0, 5124, 1087, 1076,    0, 1028,    0, 5124, 5124,
 5124, 5124,    0,    0,    0, 5124,    0, 1216,  -17, 1179,
   -6,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  744,    0,    0, 1336,    0,   58,    0,
    0, 1521,    0,    0,  993,    0,  574,    0,    0, 1217,
    0,    0, 1139,    0,    0, 1214,    0, 1397,    0,    0,
    0, 1336,    0,    0,    0,  -42, 1521,    0,  549,    0,
    0, 1352,    0, 1380,    0, 1120,    0,    0,    0,    0,
    0,    0, 1366,    0,    0, 1248,    0, 1386,    0, 1258,
    0,    0,    0, 1248,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8328,    0, 5124,    0,    0,    0, 4811,
 1362, 4892, 5124, 1325, 1414, 1728, 1471, 1454, 4811, 1408,
 4892, 1429, 1437, 1321,    0,10019,    0,    0,    0,    0,
    0,    0,    0,    0, 7600,    0,    0,    0, 1393, 5124,
 1476, 1396,  670, 5124, 1419,    0,    0, 1076,    0,    0,
    0,    0,    0,    0,    0, 5124,    0,    0, 1521,    0,
 1352,    0, 1120,    0, 1492,10019, 1448,    0, 1440, 1430,
  736, 1535, 1091,   -6,    0, 1615,  688,    0, 5124,    0,
  744,    0,    0,    0,    0, 5124, 1496, 4094, 1459, 4579,
 1886,    0,10222,    0,    0,  108,    0,    0,    0, 1898,
 1898, 1336, 1595, 1336, 1292, 1214,    0,    0, 3517,    0,
 5124, 5124,    0, 1496,    0,    0, 1682,  485, 1929, 5124,
 1712, 5124,    0,    0, 1938,  296, 5188,    0,    0,    0,
 1949, 1954, 1956, 1557,    0,    0, 1960,    0, 1969,    0,
 1979, 1982,    0, 1983,    0,    0,    0,    0,    0, 1984,
 1566, 1567,    0,    0,    0,    0,    0,    0,    0, 1990,
    0,    0, 8425, 1991, 1993,    0,10019,10019, 7698, 5124,
 2005,    0, 2351,    0,    0,    0,    0,    0,    0,    0,
    0, 1940, 1957,    0,    0, 2014,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1219,    0, 5124, 1625,    0, 5124, 5124,  138,
    0,    0,    0,    0,    0, 5124, 5124,  825,    0, 5124,
    0, 1731, 5124, 2023, 4811, 1668,    0, 5124, 8664, 2384,
    0,    0,10019,    0, 2351,    0, 1751, 2024,    0, 3386,
    0,   24,    0,  -41,    0,  431,    0,  431,    0,    0,
 2031,    0,  178, 1648,    0,    0,    0, 2032,  106, 2018,
 1496, 2042, 1820,    0,    0, 2343, 2041,    0,    0, 1704,
 1583,    0,    0,  184, 1742,    0,    0,    0,    0, 1336,
    0, 1898, 1898, 1898, 1898,    0, 2047,    0, 1815, 1816,
    0,    0,    0, 1796,    0, 3517,    0,    0, 3517, 1578,
    0, 1521, 1833, 5124,12115,    0,    0,  639,    0, 5124,
  664,  993, 1701, 1664, 1631,    0,    0,    0,    0,  -32,
 1705,    0, 1654,  472,    0, 1336,    0, 1837,    0,    0,
 1292,    0,    0,    0,    0,  122, 1674,    0,    0,    0,
 1676,    0, 1725, 2085,    0, 1777, 2087,  660,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2045,  188,    0,    0,    0,    0,  149, 2097, 2098, 2087,
 1521,    0, 1821,    0, 1879,    0, 1336, 1336,    0, 4094,
  574,    0, 1762,    0, 1735, 2107,    0, 9396, 1041,    0,
 1763, 1775, 2119, 5598, 2120,    0,    0,    0, 5091, 6844,
   20,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  114, 8903,  632,    0, 5124,10019,
 1155, 1156, 1350, 3104,    0,    0,10019, 5750, 2351, 1710,
  -12,10019, 8903,  927,  927, 1253, 2433, 1736, 2122,    0,
    0, 8983, 5124,10019,10019,10019,10019,10019, 8903,10019,
  856, 5124,    0,    0, 5336, 1730,    0,    0, 1703, 4892,
    0, 1851,    0, 1738, 1253, 2131,    0, 1851, 5336, 1852,
 1854, 5124,    0, 2433,    0, 2527,  802, 1726, 8903, 5124,
    0,    0,    0, 1100,    0,    0,  -11,    0, 1732,    0,
    0,    0, 5124,  593, 5124,    0,    0, 5124, 5124,    0,
    0, 8903, 1917, 4094,    0, 1680,10019, 5124,    0,    0,
 1336, 1336,    0,  395, 1654, 1654, 1727,  472, 1091,    0,
    0, 2144,    0,    0,    0,    0, 1909,    0, 5124,    0,
    0,    0,    0,    0, 1915, 2150,    0, 1893, 1910,    0,
    0, 1858, 1865, 1120, 1923,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  472, 5124,12196, 1812, 1828,
 1830,    0,  472,    0, 1523,    0,    0, 1799,    0,   91,
 1807,    0,    0,    0,  472,    0, 2085,  154,    0,    0,
    0, 1924,  154, 1818,  154,    0, 2185,  472,  472,  472,
    0, 1496, 5124,    0, 1874,    0, 1746,    0,    0,    0,
  108, 1817, 5124, 1778,  206,    0,    0, 2197, 1964,    0,
    0,    0,    0,  296, 5124, 1823, 5989,    0,    0, 1823,
 5598,    0,    0,    0, 4139,    0,  103,   40,10019,10019,
 6086, 2205,  897, 6340,10019, 6492,  953, 6601, 6855, 9222,
 7007, 7116, 7370, 5750, 5750,10019,10019, 1985,10019, 2206,
10019, 2213, 2543,10019,10019,10019,10019,10019,10019,    0,
    0,    0,    0,    0,    0,    0,    0, 1952, 1789,  565,
10019,  641, 8903,    0, 5750, 1963, 2214,   95,    0,    0,
    0,    0, 2351, 1804,    0,    0, 1980,  927,  927, 1323,
 1323,    0,  310,    0,    0,    0,    0, 5124, 5124, 5124,
 5124, 5124, 5124,  168, 2232,    0,    0,    0, 5336, 1892,
    0,  -23, 1916, 5124,    0,   95, 8903, 1916, 2241, 2232,
 5336, 4663, 1966,    0,    0,  802, 2248, 2249,  936,    0,
    0, 5750, 1967,    0,    0,    0,    0,    0,    0,    0,
    0,  725,    0,    0,    0,    0,    0, 1810, 1817,    0,
    0,    0, 2255,    0,    0,    0, 1814,    0, 1898, 2026,
    0,  395,    0,    0,    0,    0,    0,    0,    0, 2260,
 5750, 4189,    0,    0, 2002,    0,    0,    0, 4189,    0,
    0,    0, 1120, 1120, 1802, 2264,    0,    0,    0,    0,
    0,  449,  -27,  683,    0, 2044,    0,    0,    0, 5124,
  139, 1855, 1887, 1909, 2269,    0, 1654, 1856,    0,    0,
 1204,    0, 2256, 5124, 1230,  472, 2274, 2276, 2277, 2056,
  574, 2002, 1898, 1742, 4094,    0, 1705,   95, 1868, 1883,
    0,  394,    0,  472, 1931, 2017, 9396,    0,    0,  472,
 1835, 2020,    0,    0, 2289, 2291,    0, 5750, 5750,    0,
 5750, 5750, 2075, 2351, 2294,    0,    0, 2351, 2295, 9461,
    0,    0,    0, 1986, 2296, 2351, 2302, 1548, 2303, 2351,
 2304,10019,10019, 2205,10019, 9541, 2308, 2351, 2309, 2313,
 2351, 2314,10019, 2351, 2317, 2351, 2318, 2321, 2351, 2328,
 2330, 2351, 2338,    0, 2117, 2634, 2641,    0, 2649,    0,
 2707,10019,    0, 2796, 2908, 3048, 3076, 3100, 3126,10019,
  472, 8903, 2351, 1945, 8903,    0,10019,    0, 2073,  597,
10019, 8903,10019,10019,    0,    0,    0,    0,    0,    0,
    0,    0, 2055, 5336,    0, 5336,  168, 2232,    0,  277,
  277,   83,    0,    0, 7600,    0, 1857,    0, 2241, 2232,
 2339, 4663,  936, 2346, 8903, 5124,  802, 2003,    0, 2009,
 2011, 2059, 2132, 2074,    0,    0,    0, 2002,    0, 1868,
  937, 1987, 1336, 2025, 1654, 2016, 1654,    0, 5750,  271,
 1489,    0,    0,    0,    0,    0,    0,    0,    0, 1962,
 2146, 2002,    0,10019,    0, 2143, 2028, 1802, 1015,    0,
 5750,    0,    0,    0, 2046, 2051, 2052, 2053, 2062, 2065,
    0,    0,    0,    0,    0, 2377,  139,    0, 1727,    0,
    0,  154,    0, 4189, 2063,  154,    0,  154,    0, 1909,
 2416,    0,    0,    0,    0,  108, 2146, 1654,    0, 1742,
    0,    0, 7600, 2180,    0,    3,    0, 2255,    0, 2419,
    0, 2421, 1336,  412, 2422,    0, 1336,    0,    0,    0,
 2117,    0, 2117,10019, 2049, 2049, 1253, 2423, 1919,    0,
 2049, 2049,10019, 2049, 2049, 2830, 2351,    0, 1884,10019,
 2351, 2049, 2049, 2049, 2049, 2351, 2049, 2049, 2049, 2049,
 2049, 2049,    0,    0, 3517, 2428,    0,    0, 3156,    0,
    0,    0,    0,    0,    0, 3163,  152,    0, 8903, 2162,
 2036,    0, 8903, 3289, 1920, 1948, 2351, 2092,    0,    0,
    0,    0,    0,    0,    0, 2050,    0,    0,    0,    0,
 1751, 8903,    0, 4663, 2339,    0, 1941,    0,  936,    0,
    0,    0,  802, 5664, 5750, 2091, 2146,    0,    0,    0,
    0, 1898,    0,    0, 1336, 1523,    0,    0,  443,    0,
    0,    0, 5124,12014,    0,    0, 2351, 2208, 2143,    0,
 1260, 5124,    0, 2153,  464, 2057, 2156, 2061, 2190, 2159,
 2200, 2002,    0,    0, 2463,    0, 2465,    0, 2157, 2208,
 2063,    0,    0,    0,    0,    0,    0,    0,    0, 2002,
 1705,    0, 1523, 2044, 2191, 2125,    0, 1817,    0,    0,
    0, 2141,  -56,    0, 1909, 2152, 2351, 2485, 2488,   95,
    0,    0, 2495, 2496, 2351, 2502, 2504,10019,10019, 2351,
 2505, 2506, 2508, 2509, 2511, 2513, 2515, 2517, 2521, 2524,
    0,    0,    0,    0,  472,    0,    0,    0, 8903,    0,
    0,    0, 1726,    0,  277, 1726,    0,    0, 1201,    0,
 2259,    0, 2525, 2523,    0,    0,    0,    0,    0,    0,
   20,  713,    0,    0,    0,    0, 1654,    0,    0,    0,
    0, 3517, 1944,  -84, 2514,  498, 2535, 2236,10019, 2518,
 2538, 2522, 2528, 2533, 2536, 2254, 2545,    0,    0,    0,
 2297,11940, 2550,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2556, 5124,    0,    0,
 2258, 2268, 5124, 1802,    0,    0, 1120, 5124,    0,    0,
    0,    0,    0,    0,    0, 2146,    0,    0,    0,    0,
    0, 2146,    0,    0,    0,    0,  933, 1726,    0,    0,
10019, 1336,    0,    0,    0, 1336,    0,    0,10019,    0,
    0,    0,    0, 2351, 2351,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2573,    0, 1967,    0, 1967,
 5750, 5124,  977, 5664,    0,    0, 2250,    0, 2577,    0,
    0,    0,  232,    0, 9780, 5124,10019,    0, 2111, 2281,
 5750, 2351,    0, 5750,    0,    0,    0,    0, 1433,    0,
    0,  595,    0, 2173,    0,    0,    0,    0,    0, 1802,
 1120,    0, 2242,    0,    0,    0,    0, 2044,    0,  933,
 2582,  936,    0,    0, 1967, 2351,    0,    0, 1948,    0,
 2074, 2074,   20,    0,    0,    0, 5817, 4189,    0, 2575,
    0, 3117, 8328, 1741,10019, 1433,  578, 2576,  601,  949,
    0,    0,    0, 5124, 5124,  139,  204,    0,    0,    0,
 2242,  405, 1802, 2257, 2261,    0,    0,  936, 2591,  933,
 2298, 2074, 2596, 1726, 2091, 2091, 5124,    0, 2597, 2595,
 1161,    0,    0,    0, 2237, 1433,    0, 1464,  247, 2207,
    0, 2347,    0, 1433,    0,    0,    0,    0,12014,  595,
 1802,  869,  869,    0, 2349,    0, 2210,    0,    0,    0,
  936,  933,    0,    0, 1967, 2266, 2266,    0,    0, 5817,
 2614,    0, 5124,    0,  719, 2598, 1333, 1433, 2366, 2358,
12014,12014,    0,    0,    0,    0,    0, 2202,  567,    0,
    0,    0, 1481, 2074,    0,    0,    0, 5124,    0, 1433,
 2619,    0,    0,  220, 5124,12014, 2359,    0,    0,    0,
    0, 5750, 2091, 2623, 2627, 1976,12014,    0,    0,12014,
   20, 2266, 5124,    0,    0,    0,    0,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  211,  760,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  148, 1554,    0,    0,    0,    0,    0,    0,
 2670,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  127,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  348,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2336,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2306,    0,    0,
    0,  307,    0,  307,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2310,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  857,    0,    0, 7937,
 5446,    0,    0,    0,  979,    0,    0,    0, 1752,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  101,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  510,    0,    0,    0,    0,  617,    0,    0, 1124,
    0,    0,    0,    0,    0, 1412,    0, 7566,    0,    0,
    0,    0,    0,    0,    0,    0,  510,    0,  509,    0,
    0, 2638,    0,    0,    0, 2431,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1277,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1326,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1145,    0,    0,    0,    0,
    0,    0,    0, 6180,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1178,    0,    0, 1351,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  510,    0,
 2638,    0, 2431,    0, 1360,    0,    0,    0,    0,  313,
    0,    0,    0, 1388,    0, 2420,    0,    0,    0,    0,
 1390,    0,    0,    0,    0,    0, 2436,  896,10364,    0,
 1409,    0,    0,    0,    0,  230,    0,    0,    0, 2239,
 2239,    0,    0,    0, 1460, 1556,    0,    0,    0,    0,
    0,    0,    0, 2436,    0,    0, 1478,    0,    0,    0,
    0,    0,    0,    0, 7872, 2312,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1911,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2473,
    0,    0,    0, 2726, 2979,    0,    0,    0,    0,    0,
    0,    0,  381,    0,    0,    0,    0,    0,    0,    0,
    0,  166, 3232, 2220,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1462,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1470,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  611,    0,    0, 2383,    0,  183,
    0,    0,    0,    0,    0,  694,    0,  694,    0,    0,
 1196,    0, 1234, 1285,    0,    0,    0,    0, 1277,    0,
 2436,    0,    0,    0,    0,  112,  245,    0,    0,  116,
    0,    0,    0, 2589,  255,    0,    0,    0,    0,    0,
    0, 2239, 2239, 2239, 2239,    0,  280,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2440,    0,    0,    0,    0,    0, 2420,    0,    0,
 1327,    0, 2235,    0,    0,  579,    0,    0,    0,    0,
  228,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1508,    0,    0,    0,    0, 1096, 5714,    0,    0,    0,
 8227,    0,    0, 8800,    0,    0,10338,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1699, 2072,    0,    0,    0,    0,10359, 5060,    0,10338,
  510,    0,    0,    0,    0,    0,    0,    0,    0,  896,
  109,    0,    0,    0,    0, 2305,    0,    0,    0,    0,
    0,    0,    0,    0,    0,10628,10771,10881,    0,    0,
 1398,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,10099,    0,10099,    0,    0,    0,
10099,10099,10099,    0,    0,    0,    0,    0, 2263,    0,
    0,    0,    0, 4202, 4434, 8089, 2662,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  404,    0,    0, 1554,    0,    0,  404,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  605,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  728,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  896,    0,  137,    0,    0,    0,    0,
    0,    0,    0, 2954,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1692,    0,    0,    0,
    0,    0,    0,    0, 2667,    0,    0, 2215, 1127,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  278,  458,
  661,    0,    0,    0, 2498,    0,    0,    0,    0,    0,
 2089,    0,    0,    0,    0,    0, 8800,    0,    0,    0,
    0,  750,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2436,    0,    0,  421,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 9881,    0,    0,    0,  129,    0,
    0,    0,    0, 2399,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,10099,    0,    0,
    0,    0, 1328,    0,    0,    0, 3464, 4687, 4919, 3717,
 3949,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1539, 1574,    0,    0,    0,    0,    0,
    0,    0, 1575,    0,    0, 7937,    0, 1575, 1635, 1636,
    0,    0,    0,    0,    0,    0,  830, 1801,  807,    0,
    0,    0,  702,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  190,    0,    0,    0,    0,    0, 2239, 2278,
    0, 2989,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2444,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1265,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1643,    0,    0,    0,    0,
    0,    0,    0, 3391,    0,    0,    0,    0,    0,    0,
    0,    0, 1232,    0,    0,    0,    0,    0,    0,    0,
  109, 2444, 2239, 1645,  896,    0, 2460, 7937, 1666,  115,
    0,    0,    0,    0, 9881,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 5965,    0,    0,    0, 6291,    0,    0,
    0,    0,    0,    0,    0, 7386,    0,10953,    0,11025,
    0,    0,    0,    0,    0,    0,    0,11085,    0,    0,
11119,    0,    0,11191,    0,11254,    0,    0,11265,    0,
    0,11328,    0,    0, 9204,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2292,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1539, 1672,    0, 2307,
  409,    0,    0,    0,    0,    0,    0,    0, 1696, 1707,
 1714,    0, 2391, 1402,    0, 4029,    0, 2396,    0, 2398,
 2401,    0,    0, 1176,    0,    0,    0, 2444,    0, 1666,
    0, 1921,    0,    0,    0, 2288,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 6213,
    0, 2476,    0,    0,    0, 2602,    0, 1265, 2336,    0,
    0,    0,    0,    0,  697,  793,  813,  826,  841,  853,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  865,    0,    0,    0,    0, 2658,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1719,
    0,    0,    0,    0,    0,    0,    0, 1748,    0,    0,
    0,    0,    0,  174,    0,    0,    0,    0,    0,    0,
  354,    0,  360,    0,    0,    0, 8089,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11392,    0,11426,    0,
11558,    0,    0,    0,    0,11579,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1411,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1750,    0,    0,    0, 1988,    0,
    0,    0,    0,    0,    0,  325,    0,    0,    0,    0,
    0, 2239,    0,    0,    0, 2542,    0,    0,    0,    0,
    0,    0,    0, 2448,    0,    0, 2716, 1656, 2751,    0,
 2420,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2444,    0,    0,    0,    0,    0,    0,    0, 1759,
  926,    0,    0,    0,    0,    0,    0,    0,    0, 2444,
 2460,    0,  448,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  336,    0,11621,    0,    0,10099,
    0,    0,    0,    0,11642,    0,    0,    0,    0,11758,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  432,    0,  409,  650,    0,    0, 2391,    0,
 2424,    0,    0,  276,    0,    0,    0,    0,    0,    0,
 1010, 2719,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1449, 1467,    0,    0,    0,    0,    0,    0,    0,
    0, 2452,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2333,    0,    0,  117,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  159,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,11790,11864,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  410,    0,  805,
    0,    0,    0,    0,    0,    0, 2722,    0,    0,    0,
    0,    0, 2704,    0,    0,    0,    0,    0,    0,    0,
    0, 2706,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2458,    0,    0,    0,    0,    0, 2333,
  136,    0,  128,    0,    0,    0,    0,    0,    0,    0,
  349,  231,    0,    0,  185, 2716,    0,    0,    0,    0,
  361,  346, 4514,    0,    0,    0, 2719,    0,    0,    0,
    0,    0,  225,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  157,    0, 1265,  582,  590,    0,    0, 2391, 1482,    0,
    0,  214,    0,  432,  150,  316, 1452,    0,    0, 2735,
    0,    0,    0,    0, 2720,    0,    0,    0, 2474,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2333,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  338,    0,    0,    0,  410, 2741, 1141,    0,    0, 2719,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2391,  361,    0,    0,    0,    0,    0,    0,
 2724,    0,    0,    0,    0,    0,11916,    0,    0,    0,
    0,    0,  150, 2748,    0,    0,    0,    0,    0,    0,
 3368, 2741,    0,    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2790,    0,    0, 2799,
    0,    0,    0,    0,    0,    0,   11,    0, 2802,    1,
  253,  426, -648, 1609,  324, -212, -483, 2791, 2534,    0,
  -28,    0, 2551,  -94,  -40,    0,  499, -341,    7,  -62,
  -36, -219, 2546, 2108,    0,    0,    0,  928,    0,    0,
  -24, 1737, -520,   -2, -890, 2188, 1970, -299, 1740, 2742,
 -277,    0,  -60,    0, -474,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -561, -555, 1749, -275,  982, -892,    0,
    0,    0,    0, -330, -119, -400, 1756, -331,  -51,    0,
 -529,    0,    0, 1394, -967,    0, 1395,    0,    0, 2519,
 -627,  -18,    0,    0,    0,    0, 2484,    0, 2282, 2529,
 -204, 1780, -691,    0,    0,    0, -694,    0,    0,    0,
    0,    0,    0, 1845,    0, 2547, 2112, 1787, 2532, 2549,
    0, 1222,    0, -516, 2078,  -35, -652, -750,    0,    0,
    0, -931, -918, -916, -267, -915,    0, 1455, -990,    0,
 1160,-1155, 1463,    0, 2316,    0,    0,    0, 1163, 1170,
 1122,   41, -269, -725, -353,    0,    0, 2113,    0, 1655,
    0,    0, -625,    0,    0,    0,    0, -161,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1193,    0, -359,    0,    0,   59,    0, -384, -231, 1085,
 1237,    0, 1143,    0, 1905, 1686, 1509,    0,-1067, -861,
 -862,    0, -494, -708, -697,    0, -464, 1229, 1002, -450,
 -523, -742, -925, -798,    0,    0,   -3,    0,    0,    0,
    0,    0, 2567,    0,    0,    0,    0, 2580, 2350,    0,
    0,    0,    0,    0,   -9, 2593,    0,    0,    0,    0,
  -10,    0,    0,    0, -346, 2080, -342, 1644, -458,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2311,    0,
    0, 2095,    0,    0,    0,    0,    0,    0,   44,    0,
 2915,    0,    0,    0,    0,    0, 2632,    0,    0,    0,
    0, 2438,    0, 2437,    0,    0,    0, 2196,    0,    0,
    0, -532,    0,    0,   76,    0, 2183,    0,    0,    0,
    0,    0, -589,-1158, -825,  759, -146, 1168,    0, 2801,
 -116, 1245,    0, 2218, -601, 2226,    0, -822, 1968,    0,
    0, -141, -240,    0, 1309,-1137,    0,-1135,-1106,-1084,
 1426,    0, 1177,    0, 1224,    0, 1126,    0,    0,    0,
    0, 2455,    0,    0,    0, -375, 2251, 1998, -971,    0,
    0,    0, -661, -367,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2981, 1319,
 1843,    0, -462,    0,    0,    0, -898,    0, 1755,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -210,
 1826,    0,    0,
};
#define YYTABLESIZE 12708
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  308,  154,  259,
   37,  208,  611,  437,  750,  192,  612,  486,  278,  281,
  574,  279,  282,  579,  243,  787,  217,  209,  195,  775,
  546,  252,  778,  736,  688,  247,  229,  204,  789,  946,
  222,  756,  461,  186,  188,  191,  194,  148,  199,  248,
  920,  461,  544,  210,  188,  215,  254,  194,  253,  148,
  199, 1245,  228,  127,  898,  219, 1173, 1008,  734,  246,
  969,  239,  149,  148,  241,  188,  224,  239,  215,  191,
  194,  250,  148,  199,  228,  127,  994,  194,  249,  313,
 1234,  713, 1423,  265,  625,  267,  928, 1234,  641,  251,
  624,  266,  999, 1235, 1195, 1236, 1238,  851, 1077,  309,
 1235,  618, 1236, 1238,  350,  626,  746,  860,  536,  210,
  148,  210,  311, 1239,  289,  966,  612,  256, 1052, 1025,
 1239,  371,  482,  215, 1051,  746,  622, 1246, 1032,  307,
  194,  148,  228, 1100,  507,  277,  154,  564, 1254,  749,
  996, 1292,  618, 1203,  263,  618,  256,  746,  695,  624,
  746,  820,  333, 1270,  508,  323,   77, 1275,  256,   85,
  618,  256,   85,  350,  626,  746,  746,  622,  437,  746,
  622,  710,  276,   77,  681,  612,  256,  360,  838,  625,
  697, 1072, 1496,  763,  746,  622, 1003,  256, 1057,  315,
  256,  765,  766,  767,  768,  277,  564, 1503,  611,  280,
  564, 1184,  612,  693,   89,  256, 1293,  695,  513, 1377,
  454,  745, 1506,  762,  323,  263,  651,  392, 1459,  467,
  356,  457,  277,  983,  318,  970,  493,  943, 1035,  708,
  457,  753,  317,  681,  611, 1085,  210, 1760,  625,  579,
  148,  321,  463,  127,  459, 1421, 1516,  364, 1518,  148,
  720,  463, 1458, 1754,  133,  500, 1144, 1145,  809,  564,
  506,  496,  693, 1257,  356,  680,  868,  378,  835,  461,
  191,  894,  795,  325,  148,  611,  392, 1519,  467,  356,
 1754, 1039, 1698,  545,  923,  924, 1190, 1170, 1603,  322,
  548, 1101,  209,  611,  561,  810, 1438,  563,  821, 1520,
 1187, 1410,  628,  459,  334,  697,  680, 1646,  633,  210,
  621,   77, 1199, 1201,  697,  508,  210,  575,  210,  577,
  210,  832, 1381,  566,  680,  134,  378,  357,  461,  277,
   77,  631, 1234,  629, 1218,  693, 1287,  566,  355,  511,
  746,  194,  199, 1174,  283, 1235,  697, 1236, 1238,  361,
  148,   60,  191,  737, 1098, 1196,  567,  372,  623, 1058,
 1433,  628,   79,  283,  697, 1239,  516,  568,  372,  516,
  668,  357,  295,  697, 1389,   83,  693, 1253,  272, 1448,
 1600,  335,  519, 1230,  768,  362,  357,  284,  486,   14,
  770,  693, 1176,  737,  693,  895,  566,  355,  514,  811,
  691,  323, 1191,  746, 1258,  746,  896,  707, 1672, 1078,
  125,  668,  746, 1647,  668, 1102,  704,  498, 1421,  618,
 1455,  506,  707,  256,  256, 1604,  256,  712,  925,  668,
  624,  718,  927,  256,  612,  148,  457,  195,  194,  463,
  681,  618,  761,  815,  622,  626,  148,  376,  942,  273,
  148,  486,  737,  194,  834,  148,  612,  514,  154, 1495,
 1310, 1311,  695, 1312, 1313, 1188,  622, 1247, 1248,  125,
  731,  275,   89, 1530,  486,   97,  695, 1200, 1099,  695,
  461,  392, 1397,  467, 1020,  744, 1516, 1022, 1518,  746,
 1378,  271,  651, 1097, 1579,  764,  195, 1759,  733,  973,
 1789, 1028,  459,  746, 1720,  681,  376, 1531,  746,  791,
 1298,  723, 1566, 1827, 1567, 1593, 1299, 1519, 1404,  625,
 1734, 1029,  894, 1629,  256,  746,  697,  461, 1046,   85,
 1016,  378,  746, 1451,  784,  210,  262, 1286, 1016, 1520,
  784,  817,  639, 1385,  624, 1087,  256,  377,  356, 1594,
  459,  356,  611,  256, 1019,  618,  860, 1764,  350,  626,
 1509,  816, 1070,  679, 1605,  323,  746, 1436,  974,   76,
  612,  252,   79,  564,  611,  461, 1092,  995, 1194,  253,
  622,  709,  781,  680,  209,  837,   76,  256,  695, 1084,
  275, 1409,  845,  846,  695, 1797,  680,  508, 1609,  516,
  731,  275,  695,  618,  987,  680,  372,  746, 1750,  564,
  210, 1183,  252, 1425,  681,  252,  467,  275,  256,  862,
  253, 1640, 1450,  253,  325,   79,  746, 1642,  681,  861,
  252, 1752,  697,  625,  516, 1014, 1015, 1319,  253,  695,
  902,  731,  628,  693,  731, 1596,  895,  256,  131,  215,
  377,  842,  680,  695,  697,  357,  947,  693,  357,  731,
  356,  680,  459,  900,  378,  372,  355,  785, 1225,  355,
  679,  707,  356,  936,  356, 1463, 1675,  668,  668,  746,
  695,  210,  210,  592,  472,  463,  383,  461,  611,  486,
  463,  681,  697, 1679,  894, 1762, 1586,  639,  695,  463,
 1528,  697,  194,  506,  500,  680,  993,  194,  209,  377,
  731,  376,  454,  156, 1592,  894,  639, 1470, 1177,  680,
 1742, 1180,  693,  148,   76,  148, 1004, 1004,  210,  768,
  681,  693, 1288,  548,  210,  770,  516,  693,  210,  135,
 1009, 1244,  592,   76, 1237,  383,  693,  157, 1810,  695,
  681, 1237,  695, 1252,  697, 1018, 1234, 1521,  125,  210,
  737, 1038, 1040,  268, 1424,  514,  420,  357,  697, 1235,
 1507, 1236, 1238, 1274, 1726, 1087, 1255, 1365,  355,  357,
  668,  357,  384, 1733,  122,  195,  681,  210,  210, 1239,
  355,  516,  355, 1037,  681,  681,  655,  739,  135, 1045,
 1649,  200,  380, 1437, 1071, 1491,  365, 1668,  695, 1447,
 1387, 1055,  508, 1010, 1082,  379,  894,  695,  895,  664,
  344,  668,  628,  148, 1769, 1067, 1068, 1069,  236,  894,
  382,  976, 1370,  148, 1372,  681, 1763,  655, 1345,  895,
  655,  384,  381,  105,  376, 1094,  562, 1713,  894,  252,
   79,  252,  894,  681,  136,  655, 1820,  253,  252,  253,
  664,  380,  216,  664,  731,  372,  253, 1464,  740, 1727,
  372, 1694,  106,  107,  379,  108,  372, 1415,  664, 1181,
 1256,  283,  109,  110,  111,  112,  113,  945,  114,  382,
  721,  115,  116,  117,  118,  119, 1714,  345, 1178, 1776,
 1777,  381,   98,  366,   14,  562, 1182, 1715, 1821, 1691,
  268, 1692,  695,  136,  377,  137, 1663,  486,  506,   76,
  731,  437,  695, 1452,  899,  695,  830,  987,  463,  194,
  199,  463,  228,  127,  695,  202,  203, 1165, 1166,  463,
  895, 1358,  496,   99, 1360,  143, 1179,  792,  763,  252,
  383,  463,  463,  895,  437,  100,  194,  253,  697,  122,
  122,  946, 1680,  698,  542,  959, 1732,  695,  571,  234,
  695,  252,  895, 1259,  137, 1432,  895,  543,  252,  253,
  101,  695, 1754, 1735, 1736,  990,  253, 1832,  611, 1162,
  695,  326,  612, 1693,  831, 1529, 1237, 1753, 1628,  692,
  129, 1630,  991,  372,  423, 1643, 1454,  147,  945,  681,
 1260, 1716,  252, 1707,  115,  486, 1709,  135,  869,  135,
  253, 1363,  681,  876, 1773,  592,  695,  571,  793,  209,
 1265,  681, 1370, 1524,  695,  695,  454,   79,  959,  327,
  692, 1266, 1595,  497, 1265, 1635,  384,  377,  695,  102,
  144,  115,  731, 1281, 1388,  210, 1804,  145,  692,  103,
  731, 1527,  210, 1375, 1375, 1379,  380,  130,  158,  794,
 1636, 1301,  252,  523,  240,   77,  244, 1305,  681,  379,
  253, 1167,  524,  383,  104,  514,  115,  681,  592,  731,
  731,  592,  731, 1685,  382, 1406, 1823, 1408,  498,  731,
  731,  731,  731,  731,  486,  731,  381,  592,  731,  731,
  731,  731,  731,  568,  655,  499,  483,  135,  802, 1215,
  803,  159,  596, 1795,  655,  681,  514,  655, 1637,  514,
  612,  681,  364,  592,  136,  795,  655,  664, 1497,  679,
  664,  944, 1500,  255,  514,  681,  514,  664, 1357,  256,
  664,  328, 1511,  375,  664, 1817, 1818,  483, 1453,  664,
  483,  380,  664,  241,  463,  693,  463,  587,  105,  329,
 1216,  612,  568,  596, 1601,  483,   79,   79,  664,  384,
 1829,  681,  463,  655, 1831,  593,  307,  194, 1004,  612,
  681, 1836,  655,  364, 1837,  137,  693,  106,  107,  380,
  108,  694,  664,  330,  241, 1728,  664,  109,  110,  111,
  112,  113,  379,  114, 1122,  664,  115,  116,  117,  118,
  119, 1731, 1676,  603,  693,  423,  587,  382, 1677, 1775,
 1123,  715,  664, 1805, 1806, 1798,  655, 1276, 1113,  381,
  338,  869, 1513,  339,  593,  869, 1208,  480,  655,  703,
  655,  105,  702,   79,  250, 1771,  485, 1435,  716,  664,
 1209,  260,  508, 1276, 1114,  491,  695, 1731, 1462,  231,
  261,  664, 1466,  664,  588, 1124, 1799,  264, 1627,  664,
  106,  107,  603,  108, 1210,  241, 1277,  516,  340, 1838,
  109,  110,  111,  112,  113,  250,  114, 1803,  250,  115,
  116,  117,  118,  119, 1125,  105,  232, 1678,  664,  664,
 1731,  664, 1280,  250,  491,  325,  453,  692,  664,  664,
  664,  664,  664, 1800,  664,  695,  241,  664,  664,  664,
  664,  664,  268,  588,  106,  107, 1749,  108, 1211,  692,
  570,  269, 1731,  514,  109,  110,  111,  112,  113,  557,
  114, 1532,  270,  115,  116,  117,  118,  119,  870,  532,
  453,  870,  514,  514,  463,  514, 1754,  209,  506,  514,
 1004, 1126,  514,  194,  325,  453, 1787,  399, 1375,  400,
  274, 1813,  105, 1577,  679,  514,  692,  694,  684,  685,
  687,  514, 1511,  210,  483,  692,  483, 1641,  396,  570,
  508,  151,  148,  483,  283,  283,  139,  291,  557,  533,
  870,  106,  107,  290,  108, 1811,  483, 1712, 1814, 1399,
 1237,  109,  110,  111,  112,  113,  105,  114,  694, 1400,
  115,  116,  117,  118,  119,  306,  399,  483,  400,  692,
 1826,  871,  292,  534,  871,  491,  694,  904,  906,  160,
  724,   43,  437,  692,  726,  106,  107,  396,  108,   51,
  151,  535, 1645,  514, 1712,  109,  110,  111,  112,  113,
  293,  114, 1673,  294,  115,  116,  117,  118,  119,  316,
  420, 1626, 1513,  693,  980,  514,  319, 1781,   76,  295,
  105,   79,  514,  871,  483,  697,  695,  161,  696,  980,
  698,  514,  985,  514, 1712,  693,  506, 1513,  160,  587,
   43, 1208, 1794,  188, 1674,  296,  483,  985,   51,  106,
  107,  514,  108,  483, 1782, 1209,  514,  593,   41,  109,
  110,  111,  112,  113, 1671,  114, 1650, 1783,  115,  116,
  117,  118,  119, 1712,  587,  152, 1712,  275,  936, 1210,
  336,  751,  693,  148, 1513,  514,  161,  483,  191,  350,
  532,  693,  593,   24,  742,  603, 1721, 1687, 1712,   80,
   81, 1688,  587, 1631,  986,  587,  149,  148,  751,  697,
  695,  578,  696,  638,  698,  638,  638,   41,  638,  349,
  593,  587,  693,  593, 1513, 1703,  351,  694,  352,  283,
  603,  638, 1513, 1211,  152,  693,  638,  508,  780,  593,
  533,  779,  936,  210, 1095,   99,  588,  587, 1096,  693,
  875,  353,   24,  742,   47,   49,  194, 1571,  603,  523,
  347,  603,  469, 1513,  127,  593, 1513,  485,  524,  508,
  508,  903,  908, 1755,  534,  216,  354,  603,  919, 1572,
  358,  588,  101,  926,  423,  367, 1573,  368, 1513,  149,
  148,   25,  535,  933,  508,  937,  938,  939,  940,  941,
  423,  933, 1369,  603, 1371,  508, 1737,  148,  508,  588,
  151,  516,  588,   47,   49,   48,  216,  370,  493,  216,
 1574,  469,  372,  127,  188, 1757,   50,  373,  588,  377,
  485,  151,   79,   45,  216,  694, 1758, 1711,  128,  149,
  148,  376,  657,  506,  367,  694, 1737,  148,  694, 1809,
   25,  102,  516,  485,  588,  516,  657,  694,  516,  493,
  151,  103,  493,  693,  657,  138,  139,  116,  694,   46,
  516,  671,  516,  466,   48,  506,  506,  493,  216,  151,
  657,  149,  148,  458, 1711,   50,  104,  467,  151, 1737,
  148,  471,   45,  191,  151,  150,  930,  128,  965,  699,
  506,  964,  697,  695,  694,  696, 1076,  698,  708, 1075,
  470,  506,  151,  694,  506,  671,  202,  203,  191, 1747,
  654, 1208,  358, 1788, 1711, 1094,  116,  151,   46,  473,
  671,  475, 1793,  638,  657, 1209,  358,  216, 1083,  476,
  638,  856,  152,  191,  358,  708,  477,  693,  492,  638,
  697,  695,  694,  696,  152,  698,  494,  694,  708, 1210,
  358,  654,  638, 1711, 1175,  495, 1711, 1174,  638,  638,
 1219,  694,  504, 1075, 1224,  152,  638, 1223, 1323,  654,
 1103, 1104, 1108, 1822,   14, 1116, 1118, 1120, 1711, 1128,
 1131, 1134, 1136, 1139, 1142,  514,  638, 1146, 1147,  638,
 1149,  520, 1151,  521,  152, 1154, 1155, 1156, 1157, 1158,
 1159,  531,  105, 1211,  358, 1268, 1273, 1383, 1267, 1272,
 1382,  539, 1163,  152,  423,  638,  552,  638,  638,  897,
  978,  901,  152, 1047, 1048,  905,  907,  909,  152,  638,
  180,  106,  107,  638,  108,  697,  695,  559,  696,  562,
  698,  109,  110,  111,  112,  113,  152,  114,  123,  123,
  115,  116,  117,  118,  119,  396,  397,  398,  485,  516,
  955,  978,  978,  978,  978,  978,  978,  978,  572, 1472,
 1502,  180, 1174,  699,  180,  516,  576,  626,  630,  978,
  978,  978,  978,  406,  407,  632,  493,  634,  493,  180,
  638, 1508,  493,  699,  699,  493,  638,  656,  665,  697,
  695,  516,  696,  666,  698,  667,  638,  516,  493,  669,
  516,  978, 1644,  978,  697,  695, 1171,  696,  670,  698,
  638,  638,  638,  638,  638,  638, 1835,  668,  671, 1754,
  693,  672,  673,  674,  638,  694,  675,  676,  656,  677,
  682,  656,  683,  638,  638,  638,  638,  638,  638,  638,
  638,  638,  638,  638,  692,  638,  656,  700,  638,  638,
  719,  638,  638,  638,  638,  638,  638,  638,  638,  701,
  705,  638,  638,  638,  638,  638,  710,  729,  693,  722,
  727,  516,  671,  694,  743,  747,  493,  748,  752,  671,
 1746,  754,  671,  755,  757,  758,  671,  760,  511,  523,
  769,  933,  770,  772,  671,  773,  782,  806,  493,  807,
  808,  813,  516, 1326, 1327,  493, 1329, 1331, 1161,  516,
  671,  275,  516,  818, 1336,  516,  697,  695,  654,  696,
  822,  698,  823,  824,  825,  827,  828,  516,  654,  511,
  516,  654,  511, 1349,  671,  833,  839,  840,  843,  493,
  654, 1356,  844,  423,  853,  855,  423,  511, 1163,  511,
  856,  864, 1364,  423, 1366, 1367,  865,  516,  866,  870,
  921,  516,  931,  693,  671,  959,  960,  962,  694,  671,
  967,  971,  978,  972,  978,  978,  485,  982,  180,  988,
  997, 1000,  978, 1021, 1016,  834,  423,  654, 1026, 1027,
  978,  671, 1029,  785, 1479,  978,  654, 1042,  978,  180,
 1036, 1033, 1117,  671, 1121,  671, 1129, 1132, 1034, 1137,
 1140, 1143,  978, 1043,  222, 1044, 1050,  978,  978,  959,
  180,  978, 1053, 1059, 1066, 1417,  180,  693,  978, 1073,
  978,  978,  694, 1078,  978, 1064, 1090, 1091,  978,  715,
  654,  978,  693,  978, 1110,  978, 1150,  694, 1148,  180,
  978,  978,  654,  978,  654,  222, 1152, 1172,  222,  693,
  959,  959,  959,  959,  959,  971,  959,  727,  180,  978,
  978, 1160,  978,  222,  485, 1186, 1189,  180,  959,  959,
  959,  959, 1192,  180, 1184, 1202,  978, 1205,  978,  978,
  978, 1226, 1207,  978, 1213, 1467,  978,  978,  702, 1229,
 1240,  180, 1249, 1251, 1475,  656,  978, 1261, 1269, 1271,
  959, 1480,  959, 1278, 1282,  656, 1283, 1284,  656, 1285,
  978, 1294, 1296,  978, 1084, 1303, 1306,  656, 1307, 1308,
  679, 1309,  250, 1315, 1316, 1321, 1314, 1320,  180,  978,
  423, 1322, 1324, 1325,  423,  978,  511, 1332, 1333,  516,
  978,  516, 1334, 1335,  693,  516, 1337, 1338,  516,  694,
 1339,  978,  978,  485,  978,  511,  511, 1340,  511, 1341,
  978,  516,  511,  250,  656,  511,  250, 1342,  894, 1359,
 1362, 1368, 1384,  656,  697,  695, 1386,  696,  511,  698,
 1390,  250,  697,  695,  511,  696, 1391,  698, 1392,  978,
  978, 1393,  978,  105, 1395, 1394, 1407, 1412, 1402,  978,
  978,  978,  978,  978, 1405,  978, 1414, 1434,  978,  978,
  978,  978,  978,  684,  725,  697,  695,  656,  696,  554,
  698, 1426,  106,  107, 1422,  108, 1427, 1428, 1429,  656,
 1439,  656,  109,  110,  111,  112,  113, 1430,  114,  516,
 1431,  115,  116,  117,  118,  119, 1449, 1456, 1460, 1614,
 1615, 1461, 1465, 1471,  684, 1317,  511,  684, 1492, 1498,
 1499,  516,  982,  929,  697,  695, 1504,  696,  516,  698,
  423,  959,  684,  959,  959, 1505, 1522, 1568,  511, 1578,
 1583,  959,  222, 1581,  222,  511, 1584,  195, 1580,  959,
 1585,  222, 1582, 1587,  959, 1588,  511,  959, 1589, 1599,
 1597, 1602,  516,  982,  982,  982,  982,  982,  982,  982,
 1652,  959, 1606,  959,  511, 1607,  959,  959, 1608,  511,
  959,  982,  982,  982,  982, 1610, 1611,  959, 1632,  959,
  959,  195, 1612,  959, 1613, 1616, 1617,  959, 1618, 1619,
  959, 1620,  959, 1621,  959, 1622,  195, 1623,  511,  959,
  959, 1624,  959,  982, 1625,  982, 1634,  975,  697,  695,
 1633,  696, 1648,  698, 1651,  130, 1653, 1654,  959,  959,
 1655,  959, 1686, 1153,  697,  695, 1656,  696,  176,  698,
 1689, 1657,  222, 1659, 1658,  959, 1667,  959,  959,  959,
  195,  229,  959, 1660, 1661,  959,  959,  231, 1666, 1669,
  250, 1670,  250, 1690,  222,  959, 1697, 1696, 1705,  250,
 1706,  222,  693, 1662, 1722, 1730, 1702,  694, 1704,  959,
  693, 1770,  959, 1743, 1751,  694, 1774, 1779, 1780, 1765,
 1772, 1791,  229, 1767,  232,  229, 1785,  176,  959, 1762,
 1792, 1763,  158, 1808,  959,  222, 1812,  516, 1815,  959,
  229, 1816, 1754,  693, 1819, 1830, 1833, 1834,  694,    1,
  959,  959,  618,  959, 1343,  697,  695,  202,  696,  959,
  698, 1344,  697,  695,  423,  696, 1748,  698,   29, 1347,
  697,  695,   31,  696,  746,  698,  148,  364,  516,  270,
  514,  516,  647,  268,  450,  668,  212,  514,  959,  959,
  250,  959,  693,  942,  275,   81,  516,  694,  959,  959,
  959,  959,  959,  392,  959,  987,  941,  959,  959,  959,
  959,  959,  250,  679,  982,  192,  982,  982,  677,  250,
  673,  684,  514,  675,  982,  193,  274, 1348,  697,  695,
  230,  696,  982,  698,  684,  288,  365,  982,  703,  286,
  982,  702,  222,  684,  320,  287,  987,  987,  987,  987,
  987,  987,  987,  250,  982,  704,  195,  337,  328,  982,
  982,  612,  326,  982,  987,  987,  987,  987,  713,   27,
  982,  230,  982,  982,  230, 1373,  982,  195,   30,  195,
  982,   39,  135,  982,  474,  982,  693,  982,  464,  230,
  684,  694,  982,  982,  465,  982,  987,  961,  987,  684,
  195,  854,  693, 1304, 1302, 1089,  195,  694,  245, 1290,
 1300,  982,  982, 1590,  982, 1591, 1350,  697,  695,  585,
  696,  195,  698, 1279,  541,  195,  176,  195,  982,  527,
  982,  982,  982,  195,  195,  982, 1228,  512,  982,  982,
  195, 1291,  819,  684, 1699,  998, 1031,  176,  982,  556,
  195,  697,  695, 1570,  696,  801,  698,  684,  195,  241,
 1761,  229,  982, 1576, 1802,  982,  557, 1768,  176,  195,
  195, 1024,  195, 1766,  176, 1745, 1416,  195,  195, 1828,
 1719,  982, 1796, 1220,  195, 1398, 1525,  982, 1729,  564,
  547,  805,  982,  693, 1065,  195,  538,  176,  694, 1446,
  693, 1056,  195,  982,  982,  694,  982,  137,  693,  505,
  841,  738,  982,  694,  742,  516,  176,  516,  989, 1002,
  195,  288, 1695, 1204,  516,  176,  984, 1638, 1351,  697,
  695,  176,  696,  186,  698,  985, 1807,  516, 1839,  195,
 1778,  982,  982,  751,  982, 1198, 1328,    0,  968,  176,
  195,  982,  982,  982,  982,  982,    0,  982,  983,  241,
  982,  982,  982,  982,  982, 1411,  693,  987,  187,  987,
  987,  694, 1361,    0,    0,    0,    0,  987,    0,    0,
    0,  241,    0,    0,    0,  987,  176,    0,  241,    0,
  987,    0,  186,  987,    0,    0,    0,    0,    0,  983,
  983,  983,  983,  983,  983,  983,    0,  987,  241,    0,
  230,    0,  987,  987,    0,  516,  987,  983,  983,  983,
  983,    0,  241,  987,    0,  987,  987,  187,    0,  987,
    0,    0,    0,  987,    0,    0,  987,  516,  987,    0,
  987,    0,    0,    0,  516,  987,  987,    0,  987,  983,
    0,  983,    0,    0,    0,  693,    0,    0,    0,    0,
  694,    0,    0,    0,  987,  987,    0,  987, 1352,  697,
  695, 1478,  696,    0,  698,    0,    0,    0,  516,    0,
    0,  987,    0,  987,  987,  987,    0,    0,  987,  693,
    0,  987,  987,    0,  694,    0, 1353,  697,  695,    0,
  696,  987,  698,    0,    0,    0,    0,    0,  241,    0,
    0,    0,    0,    0,    0,  987,    0,    0,  987,    0,
 1354,  697,  695,    0,  696,    0,  698,    0,    0,    0,
  241,    0,    0,    0,  987,    0,    0,  241,  697,  695,
  987,  696,    0,  698,    0,  987, 1355,  697,  695,    0,
  696,    0,  698,    0,    0, 1744,  987,  987,    0,  987,
    0,    0,    0,    0,    0,  987,    0,  693,    0,    0,
    0,  241,  694,    0,    0,    0, 1493,  697,  695,    0,
  696,    0,  698, 1494,  697,  695,    0,  696,    0,  698,
    0,  186,    0,    0,  987,  987,    0,  987,    0,    0,
    0,    0,    0,    0,  987,  987,  987,  987,  987,    0,
  987,  840,  186,  987,  987,  987,  987,  987,    0,    0,
  983,    0,  983,  983,    0,    0,  187,    0,    0,   84,
  983,    0,    0,  186,    0,    0,   85,    0,  983,  186,
   86,   87,    0,  983,    0,    0,  983,  187,    0,   88,
    0,    0,  840,  840,  840,  840,  840,    0,  840,    0,
  983,   89,  186,    0,    0,  983,  983,    0,  187,  983,
  840,  840,  840,  840,  187,    0,  983,    0,  983,  983,
    0,  186,  983,    0,    0,    0,  983,    0,    0,  983,
  186,  983,    0,  983,    0,    0,  186,  187,  983,  983,
    0,  983,    0,    0,  840,    0,    0,  693,    0, 1501,
  697,  695,  694,  696,  186,  698,  187,  983,  983,    0,
  983,    0,    0,    0,    0,  187,   90,    0,    0,    0,
    0,  187,    0,    0,  983,  693,  983,  983,  983,    0,
  694,  983,    0,   91,  983,  983,    0,  360,    0,  187,
    0,  186,    0,    0,  983,    0,    0,    0,   92,  693,
    0,    0,   93,   98,  694,    0,    0,    0,  983,    0,
  516,  983,    0,   94,    0,    0,  693,    0,    0,    0,
    0,  694,    0,    0,    0,  693,  187,  983,  360,    0,
  694,  360,   95,  983,    0,    0,    0,    0,  983,    0,
    0,    0,    0,    0,   99,    0,  360,    0,    0,  983,
  983,  516,  983,    0,  516,  693,  100,    0,  983,    0,
  694,    0,  693,    0,    0,    0,    0,  694,    0,  516,
  132,  516,    0,    0,    0,   52,    0,    0,    0,    0,
    0,  101,    0,  852,    0,    0,   53,  983,  983,    0,
  983,    0,    0,   54,    0,    0,    0,  983,  983,  983,
  983,  983,    0,  983,    0,    0,  983,  983,  983,  983,
  983,    0,    0,  840,    0,  840,  840,    0,   55,    0,
    0,    0,    0,  840,  852,  852,  852,  852,  852,    0,
  852,  840,    0,    0,    0,    0,  840,    0,    0,  840,
    0,    0,  852,  852,  852,  852,    0,    0,    0,    0,
  102,    0,    0,  840,    0,    0,    0,    0,  840,  840,
  103,    0,  840,    0,    0,    0,    0,    0,    0,  840,
    0,  840,  840,    0,    0,  840,  852,    0,    0,  840,
    0,    0,  840,   56,  840,  104,  840,    0,  693,    0,
    0,  840,  840,  694,  840,  910,  911,  912,  913,  914,
  915,  916,  917,  105,    0,    0,    0,    0,    0,    0,
  840,  840,    0,  840,    0,   57,    0,    0,    0,   58,
    0,    0,    0,    0,    0,    0,    0,  840,    0,  840,
  840,  840,  106,  107,  840,  108,    0,  840,  840,    0,
    0,    0,  109,  110,  111,  112,  113,  840,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,  840,    0,    0,  840,    0,    0,    0,  516,  730,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  840,    0,    0,    0,    0,    0,  840,    0,  516,    0,
  516,  840,    0,    0,  516,    0,    0,  516,    0,    0,
    0,    0,  840,  840,    0,  840,    0,    0,  360,    0,
  516,  840,    0,    0,    0,  360,  516,    0,  360,    0,
    0,    0,  360,    0,    0,    0,    0,    0,    0,    0,
  360,    0,    0,    0,    0,    0,  855,    0,    0,    0,
  840,  840,    0,  840,    0,  852,  360,  852,  852,    0,
  840,  840,  840,  840,  840,  852,  840,    0,    0,  840,
  840,  840,  840,  840,    0,    0,    0,    0,  852,    0,
  360,  852,    0,    0,    0,    0,    0,  855,  855,  855,
  855,  855,    0,  855,    0,  852,    0,    0,  516,    0,
  852,  852,    0,    0,  852,  855,  855,  855,  855,    0,
  360,  852,    0,  852,  852,    0,    0,  852,    0,  586,
  516,  852,    0,  587,  852,    0,  852,  516,  852,    0,
    0,    0,    0,  852,  852,    0,  852,  360,  516,  855,
    0,  588,    0,    0,  589,    0,    0,    0,    0,  360,
    0,  360,  852,  852,    0,  852,  516,    0,    0,    0,
    0,  516,    0,    0,    0,    0,    0,    0,    0,  852,
    0,  852,  852,  852,    0,  105,  852,    0,    0,  852,
  852,    0,    0,    0,    0,  590,    0,    0,    0,  852,
  516,  591,  592,  593,    0,  594,  595,  596,   98,  597,
    0,    0,    0,  852,  106,  107,  852,  108,    0,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,  598,
  114,  599,  852,  115,  116,  117,  118,  119,  852,    0,
    0,    0,    0,  852,    0,    0,    0,    0,    0,   99,
    0,    0,    0,    0,  852,  852,    0,  852,    0,    0,
    0,  100,  600,  852,    0,    0,    0,   62,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   63,  601,
    0,    0,    0,    0,    0,   64,  101,    0,  856,    0,
    0,    0,  852,  852,    0,  852,    0,    0,   65,    0,
    0,  602,  852,  852,  852,  852,  852,    0,  852,    0,
    0,  852,  852,  852,  852,  852,    0,    0,  855,    0,
  855,  855,    0,    0,    0,    0,    0,    0,  855,  856,
  856,  856,  856,  856,    0,  856,  603,  604,    0,    0,
    0,    0,    0,    0,  855,    0,    0,  856,  856,  856,
  856,    0,    0,  605,    0,  102,    0,    0,  855,   66,
    0,    0,    0,  855,  855,  103,    0,  855,  662,    0,
    0,    0,    0,    0,  855,   67,  855,  855,    0,    0,
  855,  856,    0,    0,  855,    0,    0,  855,    0,  855,
  104,  855,   68,    0,    0,    0,  855,  855,    0,  855,
    0,    0,    0,    0,    0,    0,    0,   69,  105,  662,
    0,   70,  662,    0,   71,  855,  855,    0,  855,    0,
    0,    0,   72,    0,    0,    0,    0,  662,    0,    0,
    0,    0,  855,    0,  855,  855,  855,  106,  107,  855,
  108,   73,  855,  855,    0,    0,    0,  109,  110,  111,
  112,  113,  855,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  855,    0,    0,  855,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  855,    0,    0,    0,    0,
    0,  855,    0,    0,    0,    0,  855,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  855,  855,    0,
  855,    0,    0,    0,    0,    0,  855,    0,    0,  929,
  697,  695,    0,  696,    0,  698,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  892,  891,
  893,  850,    0,    0,    0,  855,  855,    0,  855,    0,
  856,    0,  856,  856,    0,  855,  855,  855,  855,  855,
  856,  855,    0,    0,  855,  855,  855,  855,  855,    0,
    0,    0,    0, 1231,    0,    0,  856,    0,    0,    0,
    0,    0,  850,    0,  850,  850,  850,    0,    0,    0,
  856,    0,    0,    0,    0,  856,  856,    0,    0,  856,
  850,  850,  850,  850,    0,    0,  856,    0,  856,  856,
    0,    0,  856,    0,    0,    0,  856,    0,    0,  856,
    0,  856,    0,  856,    0,    0,    0,    0,  856,  856,
    0,  856,    0,    0,  850, 1468, 1469,    0,    0,    0,
    0, 1473, 1474,    0, 1476, 1477,    0,  856,  856,    0,
  856,    0, 1481, 1482, 1483, 1484,    0, 1485, 1486, 1487,
 1488, 1489, 1490,    0,  856,    0,  856,  856,  856,    0,
    0,  856,    0,    0,  856,  856,    0,    0,    0,    0,
    0,    0,    0,    0,  856,    0,  662,    0,    0,  662,
    0,    0,    0,    0,    0,    0,  662,    0,  856,  662,
    0,  856,    0,  662,    0,    0,    0,    0,  662,    0,
    0,  662,    0,    0,    0,    0,    0,  856,    0,    0,
  554,    0,    0,  856,    0,    0,    0,  662,  856,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  856,
  856,    0,  856,    0,    0,    0,    0,    0,  856,    0,
  879,  662,    0,    0,    0,  662,    0,    0,  693,    0,
    0,    0,    0,  694,  662,    0,  880,    0,    0,    0,
    0,    0,    0,  849,    0,    0,    0,  856,  856,    0,
  856,  662,    0,    0,    0,    0,    0,  856,  856,  856,
  856,  856,    0,  856,    0,    0,  856,  856,  856,  856,
  856,    0,  881,  850,    0,  850,  850,    0,  662,    0,
    0,  882,    0,  850,  849,    0,  849,  849,  849,  883,
  662,    0,  662,  384,    0,    0,    0,    0,    0,  850,
    0,    0,  849,  849,  849,  849,    0,    0,  884,    0,
  885,    0,    0,  850,    0,    0,    0,    0,  850,  850,
    0,  386,  850,  659,  886,    0,  887,  888,  889,  850,
    0,  850,  850,    0,    0,  850,  849,    0,    0,  850,
    0,    0,  850,    0,  850,    0,  850,    0,    0,    0,
  388,  850,  850,    0,  850,    0,    0,    0,    0,    0,
    0,    0,    0,  105,  659,    0,    0,  659,    0,    0,
  850,  850,    0,  850,    0,    0,    0,  890,    0,    0,
    0,    0,  659,    0,    0,    0,    0,  850,    0,  850,
  850,  850,  106,  107,  850,  108,    0,  850,  850,    0,
    0,    0,  109,  110,  111,  112,  113,  850,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,  850,    0,    0,  850,  393,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  850,  395,    0,    0,    0,    0,  850,    0,    0,    0,
    0,  850,    0,    0,    0,  396,  397,  398,    0,  202,
  203,    0,  850,  850,    0,  850,    0,    0,    0,    0,
    0,  850,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,    0,    0,    0,
    0,    0,    0,  411,  412,    0,  851,    0,    0,    0,
  850,  850,  416,  850,    0,  849,    0,  849,  849,    0,
  850,  850,  850,  850,  850,  849,  850,    0,    0,  850,
  850,  850,  850,  850,    0,    0,    0,    0,    0,    0,
    0,  849,    0,    0,    0,    0,    0,  851,    0,  851,
  851,  851,    0,    0,    0,  849,    0,    0,    0,    0,
  849,  849,    0,    0,  849,  851,  851,  851,  851,    0,
    0,  849,    0,  849,  849,    0,    0,  849,    0,    0,
    0,  849,    0,    0,  849,    0,  849,    0,  849,    0,
    0,    0,    0,  849,  849,    0,  849,    0,    0,  851,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  849,  849,    0,  849,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  849,
    0,  849,  849,  849,    0,    0,  849,    0,    0,  849,
  849,    0,    0,    0,    0,    0,    0,    0,    0,  849,
    0,  659,    0,    0,  659,    0,    0,    0,    0,    0,
    0,  659,    0,  849,  659,    0,  849,    0,  659,    0,
    0,    0,    0,  659,    0,    0,  659,    0,    0,    0,
    0,    0,  849,    0,    0,  560,    0,    0,  849,    0,
    0,    0,  659,  849,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  849,  849,    0,  849,    0,    0,
    0,    0,    0,  849,    0,    0,  659,    0,    0,    0,
  659,    0,    0,    0,    0,    0,    0,    0,    0,  659,
    0,    0,    0,    0,    0,    0,    0,    0,  854,    0,
    0,    0,  849,  849,    0,  849,  659,    0,    0,    0,
    0,    0,  849,  849,  849,  849,  849,    0,  849,    0,
    0,  849,  849,  849,  849,  849,    0,    0,  851,    0,
  851,  851,    0,  659,    0,    0,    0,    0,  851,  854,
    0,  854,  854,  854,    0,  659,    0,  659,    0,    0,
    0,    0,    0,    0,  851,    0,    0,  854,  854,  854,
  854,    0,    0,    0,    0,    0,    0,    0,  851,    0,
  948,    0,    0,  851,  851,    0,    0,  851,    0,    0,
    0,    0,    0,    0,  851,    0,  851,  851,    0,    0,
  851,  854,    0,    0,  851,    0,    0,  851,    0,  851,
    0,  851,    0,    0,    0,    0,  851,  851,    0,  851,
    0,    0,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  851,  851,    0,  851,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  521,
    0,    0,  851,    0,  851,  851,  851,  106,  107,  851,
  108,    0,  851,  851,    0,    0,    0,  109,  110,  111,
  112,  113,  851,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  851,    0,    0,  851,
  521,    0,    0,  521,    0,  951,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  851,    0,    0,  521,    0,
  521,  851,  105,    0,    0,    0,  851,    0,    0,    0,
  649,    0,    0,  417,    0,  418,    0,  851,  851,    0,
  851,    0,    0,    0,    0,    0,  851,    0,  420,    0,
  521,  106,  107,  277,  108,    0,    0,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,  851,  851,    0,  851,    0,
  854,    0,  854,  854,    0,  851,  851,  851,  851,  851,
  854,  851,    0,    0,  851,  851,  851,  851,  851,    0,
    0,    0,    0,    0,    0,    0,  854,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  854,    0,    0,    0,    0,  854,  854,  649,    0,  854,
  417,    0,  418,    0,    0,    0,  854,    0,  854,  854,
    0,    0,  854,    0,  455,  420,  854,    0,    0,  854,
  277,  854,    0,  854,    0,    0,    0,    0,  854,  854,
    0,  854,    0,    0,    0,    0,    0,    0,    0,    0,
  105,    0,    0,    0,    0,    0,    0,  854,  854,    0,
  854,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  854,    0,  854,  854,  854,  106,
  107,  854,  108,    0,  854,  854,    0,    0,    0,  109,
  110,  111,  112,  113,  854,  114,    0,  521,  115,  116,
  117,  118,  119,    0,    0,    0,    0,    0,  854,    0,
    0,  854,    0,  521,  459,    0,  521,  521,    0,  521,
    0,    0,    0,  521,    0,    0,  521,  854,    0,    0,
    0,  105,    0,  854,    0,    0,    0,  381,  854,  521,
    0,    0,    0,    0,    0,  521,  382,    0,  521,  854,
  854,    0,  854,    0,    0,    0,    0,    0,  854,  383,
  106,  107,    0,  108,    0,  384,  385,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
  116,  117,  118,  119,  643,    0,    0,  854,  854,    0,
  854,    0,    0,  386,    0,    0,  387,  854,  854,  854,
  854,  854,    0,  854,    0,    0,  854,  854,  854,  854,
  854,    0,    0,    0,    0,    0,    0,  521,    0,    0,
    0,    0,    0,    0,  389,  390,    0,    0,    0,    0,
    0,    0,    0,    0,  381,    0,  391,    0,    0,  521,
  392,    0,    0,  382,    0,    0,  521,    0,  871,    0,
  521,    0,    0,    0,    0,    0,  383,  521,    0,  642,
    0,    0,  384,  385,    0,  632,    0,  632,  632,    0,
  632,    0,    0,    0,    0,  521,    0,    0,    0,    0,
  521,  643,    0,  632,    0,    0,    0,  686,  632,    0,
  386,  645,    0,  387,    0,    0,    0,  393,    0,    0,
    0,    0,    0,  394,    0,  521,    0,    0,    0,  521,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,  389,  390,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,  391,    0,    0,    0,  392,    0,    0,
    0,  400,    0,    0,    0,  644,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,  105,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,  872,  873,  874,    0,    0,  645,    0,
    0,    0,  106,  107,  393,  108,    0,    0,    0,    0,
  394,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  395,  115,  116,  117,  118,  119,    0,  649,    0,    0,
  417,    0,  418,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,  420,    0,    0,  400,    0,
  277,    0,    0,  948,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  646,  647,  648,
    0,    0,    0,    0,    0,  632,    0,    0,    0,    0,
    0,    0,  632,  526,    0,    0,    0,    0,    0,    0,
    0,  632,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  632,    0,    0,  949,    0,    0,
  632,  632,    0,    0,    0,    0,    0,    0,  632,    0,
    0,    0,    0,  526,  526,    0,    0,  526,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  632,    0,
    0,  632,  526,  950,  526,    0,    0,    0,  951,    0,
    0,    0,    0,    0,  952,    0,    0,    0,    0,  649,
    0,    0,  417,    0,  418,  105,    0,  632,    0,  632,
  632,    0,    0,  953,  526,    0,    0,  420,    0,    0,
    0,  632,  277,    0,    0,  632,    0,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,    0,
    0,    0,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  381,    0,    0,    0,    0,    0,
    0,    0,  632,  382,    0,    0,    0,    0,  632,    0,
    0,    0,    0,    0,    0,    0,  383,    0,  632,    0,
    0,    0,  384,  385,    0,    0,    0,    0,    0,    0,
    0,    0,  632,  632,  632,  632,  632,  632,    0,    0,
    0,  643,    0,    0,    0,    0,  632,    0,    0,    0,
  386,    0,    0,  387,    0,  632,  632,  632,  632,  632,
  632,  632,  632,  632,  632,  632,    0,  632,    0,    0,
  632,  632,    0,  632,  632,  632,  632,  632,  632,  632,
  632,  389,  390,  632,  632,  632,  632,  632,    0,    0,
    0,    0,    0,  391,  819,    0,    0,  392,    0,    0,
    0,  526,    0,    0,    0,  867,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  526,    0,    0,
  526,  526,    0,  526,    0,    0,    0,  526,    0,    0,
  526,    0,    0,    0,    0,  819,    0,    0,  819,    0,
    0,    0,    0,  526,    0,    0,  381,  389,  645,  526,
    0,    0,  526,  819,  393,  382,    0,    0,  649,    0,
  394,  417,    0,  418,    0,    0,    0,    0,  383,    0,
  395,    0,    0,    0,  384,  385,  420,    0,    0,    0,
    0,  277,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,  643,    0,    0,    0,    0,  400,    0,
    0,    0,  386,    0,    0,  387,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,  526,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  389,  390,  115,  116,  646,  647,  648,
    0,    0,    0,  526,    0,  391,    0,    0,    0,  392,
  526,   76,    0,  399,  526,  479,    0,  644,  417,    0,
  418,  526,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,    0,    0,    0,  277,  526,
    0,    0,  106,  410,  526,  108,    0,    0,    0, 1635,
    0,  413,  414,  415,  111,  112,  113,    0,  114,    0,
  645,  115,  116,  117,  118,  119,  393,    0,    0,  526,
    0,    0,  394,  526, 1636,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,    0,    0,  819,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108, 1637,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,    0,  115,  116,  646,
  647,  648,    0,    0,  382,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  105,  383,    0,    0,
    0,    0,  819,  384,  385,  819,    0,    0,    0,    0,
  788,    0,  819,    0,    0,  819,    0,    0,    0,  819,
    0,    0,  643,    0,  819,  106,  107,  819,  108,    0,
    0,  386,    0,    0,  387,  109,  110,  111,  112,  113,
    0,  114,    0,  819,  115,  116,  117,  118,  119,    0,
    0,  788,    0,    0,  788,    0,    0,    0,    0,    0,
    0,    0,  389,  390,    0, 1105,    0,  819, 1106,  788,
  819,  819,  381,    0,  391,    0,    0,    0,  392,    0,
  819,  382,    0,    0,    0,    0,  867,    0,    0,    0,
    0,    0,    0,    0,  383,    0,    0,  819,    0,  479,
  384,  385,  417,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  420,    0,  819,
    0,    0,  277,    0,  819,    0,    0,    0,  386,  645,
    0,  387,    0,    0,    0,  393,  819,    0,  819,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,  389,
  390,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,  391,    0,    0,    0,  392,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1107,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,  479,    0,    0,  417,    0,  418,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,    0,  420,
    0,    0,  788,    0,  277,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0, 1115,
    0,    0, 1106,    0,    0,    0,  381,    0,  788,    0,
    0,  788,    0,    0,    0,  382,    0,    0,  788,    0,
    0,  788,    0,    0,    0,  788,    0,    0,  383,    0,
  788,    0,    0,  788,  384,  385,    0,    0,    0,  560,
  479,    0,    0,  417,    0,  418,    0,    0,    0,  788,
    0,    0,    0,    0,    0,    0,    0,    0,  420,    0,
    0,    0,  386,  277,    0,  387,    0,    0,  560,  560,
    0,  560,  280,  788,    0,    0,  788,  788,  560,  560,
  560,  560,  560,    0,  560,    0,  788,  560,  560,  560,
  560,  560,    0,  389,  390,    0,    0,    0,    0,    0,
    0,  280,  280,  788,  280,  391,    0,    0,    0,  392,
    0,  280,  280,  280,  280,  280,    0,  280,    0,    0,
  280,  280,  280,  280,  280,  788,    0,    0,    0,    0,
  788,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  788,    0,  788,    0,    0,    0,    0,    0,
    0, 1119,    0,    0, 1106,    0,    0,    0,  381,    0,
    0,    0,    0,    0, 1107,    0,  393,  382,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
  383,    0,  395,    0,    0,    0,  384,  385,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,  386,    0,    0,  387,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  389,  390,  115,  116,  117,
  118,  119,    0,    0,    0,    0,    0,  391,    0,    0,
 1127,  392,    0, 1106,    0,    0,    0,  381,    0,    0,
    0,    0,    0,    0,    0,    0,  382,    0,    0,    0,
    0,    0,    0,    0,    0,  697,  695,    0,  696,  383,
  698,    0,    0,    0,  479,  384,  385,  417,    0,  418,
    0,    0,    0,  892,  891,  893,    0,    0,    0,    0,
    0,    0,  420,    0,    0,    0, 1107,  277,  393,    0,
    0,    0,    0,  386,  394,    0,  387,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,  389,  390,    0,    0,    0,    0,
    0,    0,  400,    0,    0,    0,  391,    0,    0,    0,
  392,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,    0,    0,  115,
  116,  117,  118,  119,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1107,    0,  393,    0,    0,
    0,    0,    0,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,  479,    0,    0,  417,
    0,  418,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,  420,    0,    0,    0,    0,  277,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,    0,  115,  116,
  117,  118,  119,    0, 1130,  879,    0, 1106,    0,    0,
    0,  381,    0,  693,    0,    0,    0,    0,  694,    0,
  382,  880,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  383,    0,    0,    0,    0,    0,  384,
  385,    0,    0,    0,    0,  479,    0,    0,  417,    0,
  418,    0,    0,    0,    0,    0,    0,  881,    0,    0,
    0,    0,    0,  420,    0,    0,  882,  386,  277,    0,
  387,    0,    0,    0,  883,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  884,    0,  885,    0,    0,  389,  390,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  886,
  391,  887,  888,  889,  392,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1135,    0,    0, 1106,
    0,    0,  890,  381,    0,    0,    0,    0,    0, 1107,
    0,  393,  382,    0,    0,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,  383,    0,  395,    0,    0,
    0,  384,  385,    0,    0,    0,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,  386,
    0,    0,  387,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
  389,  390,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,  391,    0,    0, 1138,  392,    0, 1106,    0,
    0,    0,  381,    0,    0,  789,    0,    0,    0,    0,
    0,  382,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  383,    0,    0,    0,    0,  479,
  384,  385,  417,    0,  418,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  789,  420,    0,  789,
    0, 1107,  277,  393,    0,    0,    0,    0,  386,  394,
    0,  387,    0,    0,  789,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,  389,
  390,    0,    0,    0,    0,    0,    0,  400,    0,    0,
    0,  391,    0,    0,    0,  392,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1107,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,    0,    0,
    0,    0,    0,    0,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0, 1141,
    0,    0, 1106,    0,    0,    0,  381,    0,    0,  479,
    0,  484,  417,    0,  418,  382,    0,  789,    0,    0,
    0,    0,    0,    1,    0,    0,    0,  420,  383,    0,
    0,    0,  277,    0,  384,  385,    0,    0,    0,    0,
    0,    0,    0,    0,    2,    0,    0,    0,    0,    0,
    0,    0,    3,    0,    0,    0,    0,    0,    0,    4,
    0,    5,  386,    6,    0,  387,    0,    0,    7,    0,
    0,    0,    0,  789,    0,    8,  789,    0,    0,    0,
    0,    0,    0,  789,    0,    0,  789,    0,    0,    9,
  789,    0,    0,  389,  390,  789,    0,    0,  789,    0,
   10,    0,    0,    0,    0,  391,    0,  479,    0,  392,
  417,    0,  418,    0,  789,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  420,    0,    0,    0,    0,
  277,    0,    0,    0,    0,    0,    0,    0,  789,    0,
    0,  789,  789,    0,    0,    0,    0,    0,    0,    0,
    0,  789,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1107,    0,  393,   11,  789,    0,
    0,    0,  394,    0,   12,    0,    0,   13,    0,   14,
   15,    0,  395,    0,    0,    0,    0,    0,    0,    0,
  789,    0,    0,    0,    0,  789,  396,  397,  398,  399,
  202,  203,    0,    0,   16,    0,    0,  789,  133,  789,
  400,    0,  133,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
  133,  108,    0,  133,  411,  412,  381,  413,  414,  415,
  111,  112,  113,  416,  114,  382,    0,  115,  116,  117,
  118,  119,    0,   17,    0,    0,    0,    0,  383,    0,
    0,    0,    0,    0,  384,  385,    0,    0,   18,   19,
    0,    0,    0,    0,  133,    0,    0,    0,    0,    0,
  133,  133,  133,    0,  133,  133,  133,    0,  133,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  133,    0,
  133,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  388,    0,  389,  390,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  381,  391,    0,    0,    0,  392,
    0,  133,    0,  382,    0,    0,  953,    0,  953,  953,
    0,  953,    0,    0,    0,    0,  383,    0,  133,    0,
    0,    0,  384,  385,  953,    0,    0,    0,    0,  953,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  133,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,  133,  133,    0,    0,  388,
    0,  389,  390,    0,    0,    0,  396,  397,  398,  399,
  202,  203,  133,  391,    0,    0,    0,  392,    0,    0,
  400,    0,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,  686,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,    0,  638,    0,
  394,  638,    0,  638,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,  220,    0,  638,    0,  220,    0,
    0,  638,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,  220,    0,    0,    0,  220,    0,  400,  220,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  953,    0,  115,  116,  117,  118,  119,
  220,    0,  953,    0,    0,    0,  220,  220,  220,    0,
  220,  220,  220,    0,  220,  953,  527,    0,    0,    0,
    0,  953,  953,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  220,    0,  220,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  953,
    0,    0,  953,    0,    0,    0,  527,  527,    0,    0,
  527,    0,    0,    0,    0,    0,    0,  220,    0,    0,
    0,    0,   80,    0,    0,  527,    0,  527,  953,    0,
  953,  953,    0,    0,  220,    0,    0,    0,    0,    0,
    0,    0,  953,    0,    0,    0,  953,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,  527,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  638,    0,
    0,  220,  220,    0,    0,  638,    0,    0,    0,    0,
    0,    0,    0,  953,  638,    0,    0,  419,  220,  953,
  417,    0,  418,    0,    0,    0,    0,  638,    0,  953,
    0,    0,    0,  638,  638,  420,    0,    0,    0,    0,
  277,  638,    0,  953,  953,  953,  953,  953,  953,    0,
    0,    0,    0,    0,    0,    0,    0,  953,    0,    0,
    0,  638,    0,    0,  638,    0,  953,  953,  953,  953,
  953,  953,  953,  953,  953,  953,  953,    0,  953,    0,
    0,  953,  953,    0,  953,  953,  953,  953,  953,  953,
  953,  953,  638,  638,  953,  953,  953,  953,  953,    0,
    0,    0,    0,    0,  638,    0,    0,    0,  638,    0,
    0,    0,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  420,    0,  527,    0,    0,  277,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  527,    0,    0,  527,  527,    0,  527,    0,    0,    0,
  527,    0,    0,  527,    0,  638,    0,    0,    0,    0,
    0,  638,    0,    0,    0,    0,  527,    0,    0,    0,
    0,  638,  527,    0,    0,  527,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  638,  638,  638,  638,  638,
  638,    0,    0,    0,    0,    0,    0,    0,    0,  638,
    0,    0,    0,    0,    0,    0,    0,    0,  638,  638,
  638,  638,  638,  638,  638,  638,  638,  638,  638,    0,
  638,    0,    0,  638,  638,    0,  638,  638,  638,  638,
  638,  638,  638,  638,  381,    0,  638,  638,  638,  638,
  638,    0,    0,  382,  527,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,    0,    0,  527,    0,    0,    0,
    0,    0,    0,  527,    0,    0,    0,  527,    0,    0,
    0,    0,    0,    0,  527,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,  527,    0,    0,    0,    0,  527,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  388,
    0,  389,  390,    0,    0,    0,    0,    0,    0,    0,
    0,  381,  527,  391,    0,    0,  527,  392,    0,    0,
  382,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,  383,    0,    0,    0,    0,    0,  384,
  385,  420,    0,    0,    0,    0,  277,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  386,    0,    0,
  387,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,    0,    0,    0,    0,    0,  389,  390,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
  391,    0,    0,    0,  392,    0,    0,    0,  400,  544,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
  544,    0,    0,  544,    0,    0,    0,    0,    0,    0,
    0,  393,    0,    0,    0,    0,    0,  394,  544,    0,
  544,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,  678,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,    0,    0,    0,
  544,    0,    0,    0,    0,  400,    0,    0,    0,    0,
    0,    0,    0,    0,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
  381,    0,  115,  116,  117,  118,  119,    0,    0,  382,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,  383,    0,    0,    0,    0,    0,  384,  385,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,  387,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,  479,  932,    0,  417,    0,  418,    0,  391,
    0,    0,    0,  392,    0,    0,    0,    0,    0,    0,
  420,    0,    0,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  544,    0,    0,    0,  544,    0,  544,
  686,    0,    0,  544,    0,    0,  544,    0,    0,    0,
  393,    0,    0,    0,    0,    0,  394,    0,    0,  544,
    0,    0,    0,    0,    0,  544,  395,    0,  544,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,  381,
    0,  115,  116,  117,  118,  119,    0,  544,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,  544,
    0,    0,    0,  764,    0,    0,  544,    0,    0,    0,
  544,    0,    0,    0,    0,    0,    0,  544,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,  544,    0,    0,    0,    0,
  544,    0,    0,    0,  764,    0,    0,  764,    0,  381,
    0,    0,    0,    0,  388,    0,  389,  390,  382,    0,
    0,  479,  764,    0,  417,  544,  418,    0,  391,  544,
    0,  383,  392,    0,    0,    0,    0,  384,  385,  420,
    0,    0,    0,    0,  277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,    0,    0,    0,  394,  389,  390,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,  391,    0,
    0,    0,  392,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,    0,  393,
  115,  116,  117,  118,  119,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  381,    0,
  115,  116,  117,  118,  119,    0,    0,  382,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,  420,    0,
    0,  764,    0,  277,  764,    0,    0,    0,    0,    0,
    0,  764,    0,    0,  764,    0,    0,    0,  764,    0,
    0,    0,    0,  764,  386,    0,  764,  387,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  764,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  389,  390,    0,    0,    0,
  479,    0,    0,  417,    0,  418,  764,  391,    0,  764,
  764,  392,    0,    0,    0,    0,    0,    0,  420,  764,
    0,    0,    0,  277,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  764,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  764,    0,
    0,    0,    0,  764,    0,    0,    0,    0,  393,    0,
    0,    0,    0,    0,  394,  764,    0,  764,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,  857,    0,
    0,    0,  587,    0,    0, 1133,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,  858,    0,    0,    0,
  588,    0,  400,  589,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,  381,    0,  115,
  116,  117,  118,  119,  590,    0,  382,    0,    0,    0,
  591,  592,  593,    0,  594,  595,  596,    0,  597,  383,
    0,    0,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  598,    0,
  599,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  600,    0,    0,    0,    0,    0,  381,    0,    0,
    0,    0,    0,    0,  389,  390,  382,    0,  601,  479,
    0,    0,  417,    0,  418,    0,  391,    0,    0,  383,
  392,    0,    0,    0,    0,  384,  385,  420, 1701,    0,
  602,    0,  277,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,  603,  604, 1317,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  393,    0,    0,
    0,    0,  605,  394,  389,  390,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,  391,    0,    0,    0,
  392,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,  393,  115,  116,
  117,  118,  119,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1330,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,  381,    0,  115,  116,
  117,  118,  119,    0,    0,  382,    0,    0,  479,    0,
    0,  417,    0,  418,    0,    0,    0,    0,  383,    0,
    0,    0,    0,    0,  384,  385,  420,    0,    0,    0,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  386,    0,    0,  387,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  389,  390,    0,    0,    0,  953,    0,
    0,  953,    0,  953,    0,  391,    0,    0,    0,  392,
    0,    0,    0,  220,    0,    0,  953,  220,    0,    0,
    0,  953,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  220,    0,    0,    0,  220,    0,    0,  220,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,  220,
    0,    0,  395,    0,    0,  220,  220,  220,    0,  220,
  220,  220,    0,  220,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,    0,    0,    0,    0,    0,    0,
  400,    0,    0,  220,    0,  220,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,  220,  115,  116,  117,
  118,  119,    0,    0,  382,    0,    0,    0,    0,    0,
    0,    0,    0,  220,    0,    0,    0,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  220,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  534,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  518,    0,
  220,  220,    0,    0,    0,  953,    0,    0,    0,    0,
    0,    0,  389,  390,  953,    0,    0,  220,  534,    0,
    0,  534,    0,    0,  391,    0,    0,  953,  392,    0,
    0,    0,    0,  953,  953,    0,  534,    0,  534,  518,
    0,    0,  518,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  518,    0,  518,
    0,  953,    0,    0,  953,    0,    0,    0,  534,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,  518,
    0,  394,  953,  953,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  953,    0,    0,    0,  953,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,  953,  115,  116,  117,  118,
  119,  953,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  953,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  953,  953,  953,  953,  953,
  953,    0,    0,    0,    0,    0,    0,    0,    0,  953,
    0,    0,    0,    0,    0,    0,    0,    0,  953,  953,
  953,  953,  953,  953,  953,  953,  953,  953,  953,    0,
  953,    0,    0,  953,  953,  534,  953,  953,  953,  953,
  953,  953,  953,  953,    0,    0,  953,  953,  953,  953,
  953,  534,    0,    0,    0,  534,  518,  534,    0,    0,
    0,  534,    0,    0,  534,    0,    0,  829,    0,    0,
    0,    0,  518,    0,    0,    0,  518,  534,  518,    0,
    0,    0,  518,  534,    0,  518,  534,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  518,    0,
    0,    0,    0,    0,  518,    0,    0,  518,  829,  991,
  991,  829,  991,  991,  991,    0,    0,    0,    0,    0,
    0,  105,    0,    0,    0,    0,  829,  991,  991,  991,
  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
  172,  173,  174,  175,  176,  177,    0,    0,  178,  179,
  106,  107,    0,  108,    0,  534,  180,  181,  991,    0,
  109,  110,  111,  112,  113,  182,  114,  183,    0,  115,
  116,  117,  118,  119,    0,    0,  518,  534,    0,    0,
    0,    0,    0,    0,  534,    0,    0,    0,  534,    0,
    0,    0,    0,    0,    0,  534,    0,    0,  518,    0,
    0,    0,    0,    0,    0,  518,    0,    0,    0,  518,
  830,    0,    0,  534,    0,    0,  518,    0,  534,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  518,    0,    0,    0,    0,  518,
    0,    0,    0,  534,    0,    0,    0,  534,    0,    0,
    0,  830,  992,  992,  830,  992,  992,  992,    0,    0,
    0,    0,    0,  446,  518,    0,    0,    0,  518,  830,
  992,  992,  992,  446,  446,  446,  446,  446,  446,  446,
  446,  446,  446,  446,  446,  446,  446,  446,    0,    0,
  446,  446,  446,  446,    0,  446,    0,    0,  446,  446,
    0,  992,  446,  446,  446,  446,  446,  446,  446,  446,
    0,  446,  446,  446,  446,  446,    0,    0,    0,    0,
  831,    0,    0,    0,    0,    0,    0,    0,    0,  829,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  991,
    0,    0,    0,    0,    0,    0,    0,  991,    0,    0,
    0,    0,  991,    0,    0,  991,    0,    0,    0,    0,
    0,  831,  993,  993,  831,  993,  993,  993,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  831,
  993,  993,  993,    0,    0,  829,    0,    0,  829,    0,
    0,  991,  813,    0,    0,  829,    0,    0,  829,    0,
  991,    0,  829,    0,    0,    0,    0,  829,  991,    0,
  829,  993,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  829,  991,    0,  991,
    0,    0,    0,  813,    0,    0,  813,    0,    0,    0,
    0,    0,    0,  991,    0,  991,  991,  991,    0,    0,
  829,  813,    0,  829,  829,    0,    0,    0,    0,    0,
    0,    0,    0,  829,  792,    0,    0,    0,    0,    0,
    0,    0,  830,    0,    0,    0,    0,    0,    0,    0,
  829,    0,  992,    0,    0,    0,    0,    0,    0,    0,
  992,    0,    0,    0,    0,  992,  991,    0,  992,    0,
    0,    0,  829,    0,    0,  792,    0,  829,  792,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  829,
    0,  829,    0,  792,  790,    0,    0,    0,  830,    0,
    0,  830,    0,    0,  992,    0,    0,    0,  830,    0,
    0,  830,    0,  992,    0,  830,    0,    0,    0,    0,
  830,  992,    0,  830,    0,    0,    0,    0,  791,    0,
    0,    0,    0,    0,    0,  790,    0,    0,  790,  830,
  992,    0,  992,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  831,  790,    0,    0,  992,    0,  992,  992,
  992,    0,  993,  830,    0,    0,  830,  830,    0,  791,
  993,    0,  791,    0,    0,  993,  830,    0,  993,    0,
    0,    0,    0,    0,    0,    0,    0,  791,    0,    0,
    0,    0,    0,  830,    0,    0,    0,    0,    0,    0,
  821,    0,    0,    0,    0,    0,    0,    0,  831,  992,
    0,  831,    0,    0,  993,  830,    0,    0,  831,    0,
  830,  831,    0,  993,  813,  831,    0,    0,    0,    0,
  831,  993,  830,  831,  830,    0,    0,    0,    0,    0,
    0,  821,    0,    0,  821,    0,    0,    0,    0,  831,
  993,    0,  993,    0,    0,    0,    0,    0,    0,  821,
    0,    0,    0,  785,    0,    0,  993,    0,  993,  993,
  993,    0,    0,  831,  786,    0,  831,  831,    0,    0,
  813,    0,    0,  813,    0,    0,  831,    0,    0,    0,
  813,    0,    0,  813,    0,    0,  792,  813,    0,    0,
    0,    0,  813,  831,  785,  813,    0,  785,    0,    0,
    0,    0,    0,    0,    0,  786,    0,    0,  786,  993,
    0,  813,  785,    0,    0,  831,    0,    0,    0,    0,
  831,    0,    0,  786,    0,    0,    0,  787,    0,    0,
    0,    0,  831,    0,  831,  813,    0,    0,  813,  813,
    0,    0,  792,    0,    0,  792,  790,    0,  813,    0,
    0,    0,  792,    0,    0,  792,    0,    0,    0,  792,
    0,    0,    0,    0,  792,  813,    0,  792,  787,    0,
    0,  787,    0,    0,    0,    0,    0,    0,    0,    0,
  791,    0,    0,  792,    0,    0,  787,  813,    0,    0,
    0,  820,  813,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  790,    0,  813,  790,  813,  792,    0,    0,
  792,  792,  790,    0,    0,  790,    0,    0,    0,  790,
  792,    0,    0,    0,  790,  814,    0,  790,    0,    0,
    0,    0,  820,    0,    0,  820,  791,  792,    0,  791,
    0,    0,    0,  790,    0,    0,  791,    0,    0,  791,
  820,    0,  821,  791,    0,    0,    0,    0,  791,  792,
    0,  791,    0,    0,  792,    0,  814,  790,    0,  814,
  790,  790,    0,    0,    0,    0,  792,  791,  792,    0,
  790,    0,    0,    0,  814,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  790,    0,    0,
    0,  791,    0,    0,  791,  791,    0,    0,  821,    0,
    0,  821,    0,    0,  791,  785,    0,    0,  821,  790,
    0,  821,    0,    0,  790,  821,  786,    0,    0,    0,
  821,  791,    0,  821,    0,    0,  790,    0,  790,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  821,
    0,    0,    0,  791,    0,    0,    0,  822,  791,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  791,  785,  791,  821,  785,    0,  821,  821,  823,    0,
    0,  785,  786,    0,  785,  786,  821,    0,  785,  787,
    0,    0,  786,  785,    0,  786,  785,    0,  822,  786,
    0,  822,    0,  821,  786,    0,    0,  786,    0,    0,
    0,    0,  785,    0,    0,    0,  822,    0,    0,  823,
  811,    0,  823,  786,    0,  821,    0,    0,    0,    0,
  821,    0,    0,    0,    0,    0,  785,  823,    0,  785,
  785,  815,  821,    0,  821,  787,    0,  786,  787,  785,
  786,  786,    0,  820,    0,  787,    0,    0,  787,    0,
  786,  811,  787,    0,  811,    0,  785,  787,    0,    0,
  787,    0,    0,    0,    0,    0,    0,  786,    0,  811,
    0,    0,  815,    0,    0,  815,  787,  814,  785,    0,
    0,    0,    0,  785,    0,    0,    0,    0,    0,  786,
  815,    0,    0,    0,  786,  785,    0,  785,    0,  820,
  787,    0,  820,  787,  787,    0,  786,    0,  786,  820,
    0,    0,  820,  787,    0,    0,  820,    0,    0,    0,
    0,  820,    0,    0,  820,    0,    0,    0,    0,    0,
  787,    0,    0,  814,    0,    0,  814,    0,    0,    0,
  820,    0,    0,  814,    0,    0,  814,  824,    0,    0,
  814,    0,  787,    0,    0,  814,    0,  787,  814,    0,
    0,    0,    0,    0,  820,    0,    0,  820,  820,  787,
    0,  787,    0,    0,  814,    0,    0,  820,    0,  812,
    0,    0,    0,    0,    0,    0,    0,    0,  824,    0,
    0,  824,    0,    0,  820,    0,    0,    0,  814,    0,
    0,  814,  814,    0,    0,    0,  824,    0,    0,  822,
    0,  814,    0,    0,    0,    0,  820,    0,    0,    0,
  812,  820,    0,  812,    0,    0,    0,    0,  814,    0,
  823,    0,    0,  820,    0,  820,    0,    0,  812,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  814,    0,    0,  816,    0,  814,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  822,    0,  814,  822,  814,
    0,    0,  811,    0,    0,  822,    0,    0,  822,    0,
    0,    0,  822,    0,    0,    0,  823,  822,    0,  823,
  822,    0,    0,  815,  816,    0,  823,  816,    0,  823,
    0,    0,    0,  823,    0,    0,  822,    0,  823,    0,
    0,  823,  816,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  823,  811,    0,
  822,  811,    0,  822,  822,    0,    0,    0,  811,    0,
    0,  811,    0,  822,    0,  811,    0,    0,    0,  815,
  811,  823,  815,  811,  823,  823,    0,    0,    0,  815,
  822,    0,  815,    0,  823,    0,  815,    0,    0,  811,
    0,  815,    0,    0,  815,    0,    0,    0,    0,    0,
    0,  823,  822,    0,    0,    0,    0,  822,    0,    0,
  815,    0,    0,  811,    0,    0,  811,  811,    0,  822,
    0,  822,    0,  823,    0,    0,  811,    0,  823,  824,
    0,    0,    0,    0,  815,    0,    0,  815,  815,    0,
  823,    0,  823,  811,    0,    0,    0,  815,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  812,    0,    0,  815,  811,    0,    0,    0,    0,
  811,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  811,    0,  811,  824,  815,    0,  824,    0,
    0,  815,    0,    0,    0,  824,    0,    0,  824,    0,
    0,    0,  824,  815,    0,  815,    0,  824,    0,    0,
  824,    0,    0,    0,    0,    0,    0,  812,    0,    0,
  812,    0,    0,    0,    0,    0,  824,  812,    0,    0,
  812,    0,    0,    0,  812,  816,    0,    0,    0,  812,
    0,    0,  812,    0,    0,    0,    0,    0,    0,    0,
  824,    0,    0,  824,  824,    0,    0,    0,  812,    0,
    0,    0,    0,  824,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  824,    0,  812,    0,    0,  812,  812,    0,    0,    0,
    0,  816,    0,    0,  816,  812,  318,    0,    0,    0,
    0,  816,  824,    0,  816,    0,    0,  824,  816,    0,
    0,    0,  812,  816,    0,    0,  816,    0,    0,  824,
 1414,  824,    0,    0,    0,    0,  318,    0,    0,    0,
    0,    0,  816,  318,  812,    0,    0,  318,  318,  812,
  318,    0,    0,  318,    0,    0,    0,    0,    0,    0,
    6,  812,    0,  812,    0,    0,  816,  318,    0,  816,
  816, 1533, 1534,  318, 1535,    0,    0, 1536,    0,  816,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1537,    0,    0,    0,    0,  816, 1538,    0,    0,
    0,    0,    0,    0, 1414,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  816,    0,
    0,    0,    0,  816,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  318,    6,  816,    0,  816,    0,    0,
    0,    0,    0,    0,    0, 1533, 1534,    0, 1535,    0,
    0, 1536,  318,    0,    0,    0,    0, 1539,    0,    0,
    0,    0,    0,    0,    0, 1537,    0,  318,    0,    0,
    0, 1538,    0,    0,    0,    0,   14,  318,    0,    0,
    0,    0,    0,    0,    0,    0,  318,    0,  318,    0,
    0, 1540,    0,    0,    0,  318,    0,    0,    0,    0,
    0,   16,    0,    0,    0,    0,    0,  586,    0,    0,
 1662,  587, 1541,    0,    0,    0,    0,    0,  785,  105,
    0,    0,    0,    0,  318,  318,    0,  318,    0,  588,
    0, 1539,  589,    0,  318,  318,  318,  318,  318,    0,
  318,    0,    0,  318,  318,  318,  318,  318, 1542,  107,
   14,  108,    0,    0,    0,    0,    0,    0,  109,  110,
  111,  112,  113,    0,  114, 1540,    0,  115, 1543,  117,
  118,  119,    0,  590,    0,   16,    0,    0,    0,  591,
  592,  593,    0,  594,  595,  596, 1541,  597,  586,    0,
    0,    0,  587,  105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  598,    0,  599,
  588,    0,    0,  589,    0,    0,    0,    0,    0,    0,
    0,    0, 1542,  107,    0,  108,    0,    0,    0,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
  600,  115, 1543,  117,  118,  119,    0,    0,    0,    0,
    0,    0,    0,    0,  590,    0,    0,  601,    0,    0,
  591,  592,  593,    0,  594,  595,  596,    0,  597,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  602,
    0,    0,    0,    0,    0,    0,    0,    0,  598,    0,
  599,    0,    0,    0,  105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  603,  604,    0,    0,    0,    0,
    0,  600,    0,  106,  107,    0,  108,    0,    0,    0,
    0,  605,    0,  109,  110,  111,  112,  113,  601,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  602,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  603,  604,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,    0,
    0,    0,  605,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  149,   18,  104,
    0,   63,  359,  254,  509,   56,  359,  285,  138,  139,
  351,  138,  139,  355,   85,  555,   67,   63,   57,  546,
  330,   94,  549,  492,  419,   89,   73,   62,  555,  701,
   69,  516,  262,   53,   54,   55,   56,   57,   58,   90,
  678,  271,  328,   63,   64,   65,   97,   67,   95,   69,
   70, 1029,   72,   73,  666,   68,  928,  762,   45,   88,
  719,   81,   83,   83,   84,   85,   70,   87,   88,   89,
   90,   92,   92,   93,   94,   95,  748,   97,   91,  152,
 1022,  467, 1248,  129,  364,  131,  686, 1029,  376,   93,
    0,  130,  755, 1022,  966, 1022, 1022,  631,  851,  150,
 1029,    0, 1029, 1029,    0,    0,    0,  638,  323,  129,
  130,  131,  151, 1022,  143,  715,    0,    0,  820,  782,
 1029,  226,  279,  143,   44,    0,    0, 1030,  791,  149,
  150,  151,  152,   41,  306,   63,  156,    0, 1041,   44,
  752, 1077,   41,  976,   44,   44,    0,   41,    0,   59,
   44,   40,  187, 1054,  306,    0,   45, 1060,   41,   41,
   59,   44,   44,   59,   59,   59,   41,   41,  419,   44,
   44,   44,   40,   45,    0,   59,   59,  212,   40,    0,
   41,  844,   41,  525,   59,   59,  758,   41,   45,  156,
   44,  532,  533,  534,  535,   63,   59, 1366,  555,   40,
    0,   44,  555,    0,   41,   59, 1078,   59,  313, 1191,
  256,   44, 1381,   40,   59,   44,   44,    0, 1296,    0,
    0,  260,   63,  728,  159,  719,  290,  700,  794,  459,
  269,  511,  260,   59,    0,   40,  256,   44,   59,  581,
  260,  258,  262,  263,    0, 1246, 1394,  217, 1394,  269,
  473,  271,  260,   44,   12,  294,  894,  895,  301,   59,
  306,  283,   59,  301,   44,    0,  644,    0,   91,    0,
  290,  262,  558,   59,  294,   41,   59, 1394,   59,   59,
   44,  808,   61,  329,  307,  308,  320,  925,  355,  306,
  336,  262,  338,   59,  340,  338, 1274,  343,  586, 1394,
  959,   41,    0,   59,  257,    0,   41,  402,  372,  329,
  361,   45,  971,  972,    0,  467,  336,  352,  338,  354,
  340,  607, 1195,  343,   59,   12,   59,    0,   59,   63,
   45,  370, 1274,  368,  997,    0, 1072,    0,    0,  309,
   44,  361,  362,   44,  260, 1274,   41, 1274, 1274,  402,
  370,  455,  372,  405,  262,  967,  259,  259,  362,  828,
 1263,   59,  376,  260,   59, 1274,   41,  270,  270,   44,
    0,   44,  424,   59, 1207,  320,   41, 1040,  136, 1280,
 1458,  334,  317, 1021,   41,  438,   59,  303,  666,  417,
   41,   41,   93,    0,   59,  386,   59,   59,    0,  442,
  420,  418,  436,  278,  442,  280,  303,  458, 1574,  417,
    0,   41,  287,  508,   44,  386,  455,  439, 1419,  318,
 1293,  467,  473,   40,  278,  492,  280,  466,  451,   59,
  340,  470,  683,  287,  318,  455,  475,    0,  458,  459,
   41,  340,  269,  573,  318,  340,  466,    0,  699,  136,
  470,  729,   59,  473,  277,  475,  340,   59,  478,  318,
 1098, 1099,   41, 1101, 1102,  959,  340, 1033, 1034,   59,
  490,  458,  309,   41,  752,  402,  328,  971,  386,  331,
  710,  264, 1218,  264,  770,  318, 1634,  773, 1634,  383,
 1192,  320,  320,  871,   41,  530,   59,  304,  485,  722,
  264,  787,  258,  378, 1670,  331,   59, 1410,  383,  555,
 1082,  478, 1415,  304, 1417, 1451, 1082, 1634, 1223,  340,
 1689,  300,  262, 1505,  378,  400,  387,  258,  814,  411,
  458,  264,  407, 1286,  554,  555,  436, 1071,  458, 1634,
  560,  576,  431, 1202,  454,  855,  400,  452,  328, 1452,
  306,  331,  318,  407,  769,  454, 1087, 1723,  454,  454,
 1393,  574,  842,  343, 1465,  410,  441, 1269,  725,  458,
  454,    0,  586,  436,  340,  306,  864,  749,  964,    0,
  454,  454,  552,  318,  630,  447,  458,  441,  440,  394,
  458, 1229,  627,  628,    0, 1761,  331,  749, 1470,  274,
    0,  458,  454,  502,  734,  340,    0,  501,   41,  409,
  630,  454,   41, 1251,  440,   44,  397,  458,  501,  639,
   41, 1524, 1285,   44,  410,  639,  501, 1530,  454,  639,
   59,   41,  318,  454,  309,  765,  766, 1110,   59,    0,
  669,   41,  340,  440,   44, 1454,  386,  501,  273,  669,
    0,  621,  387,   59,  340,  328,  702,  454,  331,   59,
  440,  396,  418,   42,  397,   59,  328,  284, 1009,  331,
  343,  722,  452,  693,  454,  274, 1579,  307,  308,  383,
   41,  701,  702,    0,  269,  705,    0,  418,  454,  967,
  710,    0,  387, 1596,  262,  301, 1432,  431,   59,  719,
 1405,  387,  722,  749,  743,  440,  745,  727,  754,   59,
  730,  264,  758,  506, 1450,  262,  431, 1317,  948,  454,
 1698,  951,  387,  743,  458,  745,  761,  762,  748,  386,
  331,  396, 1073,  779,  754,  386,  411,  387,  758,    0,
  356, 1027,   59,  458, 1022,   59,  396,   59,   40,  328,
   59, 1029,  331, 1039,  440,  768, 1698, 1395,  348,  779,
  367,  807,  808,  264, 1249,  367,   58,  440,  454, 1698,
 1382, 1698, 1698, 1059, 1677, 1085,  338, 1172,  440,  452,
  410,  454,    0, 1686,  286,  348,  387,  807,  808, 1698,
  452,  466,  454,  806,    0,  396,    0,  377,   59,  813,
  313,  409,    0, 1272,  843, 1345,  268, 1568,  387, 1278,
 1205,  825,  964,  429,  853,    0,  262,  396,  386,    0,
  257,  451,  348,  843, 1727,  838,  839,  840,  322,  262,
    0,   40, 1184,  853, 1186,   41,  442,   41, 1148,  386,
   44,   59,    0,  460,  397,  865,    0,  263,  262,  278,
  864,  280,  262,   59,    0,   59,  300,  278,  287,  280,
   41,   59,  261,   44,  264,  259,  287,  466,  448, 1678,
  264, 1632,  489,  490,   59,  492,  270, 1241,   59,  952,
  442,  260,  499,  500,  501,  502,  503,   42,  505,   59,
  475,  508,  509,  510,  511,  512,  312,  334,  949, 1735,
 1736,   59,  260,  365,  417,   59,  953,  323,  352, 1628,
  411, 1630,  318,   59,  264,    0, 1552, 1195,  964,  458,
  320, 1172,  328, 1287,  303,  331,  277, 1057,  948,  949,
  950,  951,  952,  953,  340,  461,  462,  307,  308,  959,
  386, 1162,  283,  301, 1165,  325,  950,  319, 1290,  378,
  264,  971,  972,  386, 1205,  313,  976,  378,   42,  461,
  462, 1633,   40,   47,  287,  397, 1685,  328,    0,  458,
  331,  400,  386,  301,   59, 1261,  386,  300,  407,  400,
  338,  387,   44, 1691, 1692,  403,  407, 1823, 1345,  435,
  396,  258, 1345, 1631,  345, 1406, 1274,   59, 1503,    0,
  273, 1506,  420,  397,  436, 1532, 1292,   16,   42,  318,
  338,  427,  441, 1651,  265, 1293, 1654,  278,  644,  280,
  441,  435,  331,  649, 1732,  342,  387,   59,  400, 1075,
 1050,  340, 1384, 1397,  440,  396, 1082, 1051,  470,  306,
   41, 1051, 1453,  384, 1064,  343,  264,  397,  454,  407,
  430,  302,  452, 1066, 1206, 1075, 1775,  437,   59,  417,
  460, 1402, 1082, 1190, 1191, 1192,  264,  340,  387,  441,
  368, 1084,  501,  348,   83,   45,  322, 1090,  387,  264,
  501,  451,  357,  397,  442,    0,  337,  396,  405,  489,
  490,  408,  492, 1598,  264, 1225, 1804, 1227,  439,  499,
  500,  501,  502,  503, 1382,  505,  264,  424,  508,  509,
  510,  511,  512,    0,  318,  456,    0,  378,  465,  405,
  467,  440,  405, 1759,  328,  331,   41,  331,  426,   44,
    0,  440,  278,  450,  280, 1421,  340,  318, 1359,  343,
  321,  296, 1363,  401,   59,  454,   61,  328, 1161,   40,
  331,  418, 1394,  236,  335, 1791, 1792,   41, 1288,  340,
   44,  244,  343,  278, 1184,    0, 1186,    0,  460,  436,
  456,   41,   59,  456, 1460,   59, 1190, 1191,  359,  397,
 1816,  387, 1202,  387, 1822,    0, 1206, 1207, 1223,   59,
  396, 1827,  396,  278, 1830,  280,  280,  489,  490,  397,
  492,  285,  383,  470,  319, 1680,  387,  499,  500,  501,
  502,  503,  397,  505,  272,  396,  508,  509,  510,  511,
  512, 1682, 1586,    0,   59,  254,   59,  397, 1592, 1734,
  288,  417,  413, 1776, 1777,  377,  440,   44,  352,  397,
  258,  867, 1394,  261,   59,  871,  321,  276,  452,   41,
  454,  460,   44, 1267,    0, 1730,  285, 1267,  444,  440,
  335,  383, 1414,   44,  378,   44,    0, 1728, 1303,  265,
  383,  452, 1307,  454,    0,  333,  418,   44, 1499,  460,
  489,  490,   59,  492,  359,  400,   93,  316,  306, 1832,
  499,  500,  501,  502,  503,   41,  505, 1772,   44,  508,
  509,  510,  511,  512,  362,  460,  302, 1593,  489,  490,
 1771,  492,   93,   59,   93,    0,    0,  318,  499,  500,
  501,  502,  503,  465,  505,   59,  441,  508,  509,  510,
  511,  512,  385,   59,  489,  490, 1706,  492,  413,  340,
    0,  383, 1803,  258,  499,  500,  501,  502,  503,    0,
  505, 1413,  383,  508,  509,  510,  511,  512,   41,  279,
   44,   44,  277,  278, 1384,  280,   44, 1413, 1414,  284,
 1405,  429,  287, 1393,   59,   59, 1746,    0, 1505,    0,
  436,   59,  460, 1422,  413,  300,  387,    0,  417,  418,
  419,  306, 1634, 1413,  278,  396,  280, 1527,    0,   59,
 1552,    0, 1422,  287,  260,  260,  494,  342,   59,  329,
   93,  489,  490,  337,  492, 1785,  300, 1659, 1788,  493,
 1698,  499,  500,  501,  502,  503,  460,  505,   41,  503,
  508,  509,  510,  511,  512,  418,   59,  280,   59,  440,
 1810,   41,  377,  363,   44,  288,   59,  303,  303,    0,
  479,    0, 1703,  454,  483,  489,  490,   59,  492,    0,
   59,  381, 1533,  378, 1706,  499,  500,  501,  502,  503,
  405,  505, 1577,  408,  508,  509,  510,  511,  512,  274,
   58, 1495, 1634,  318,   46,  400,  318,  337,  458,  424,
  460, 1505,  407,   93,  378,   42,   43,    0,   45,   61,
   47,  416,   46,  418, 1746,  340, 1552, 1659,   59,  342,
   59,  321, 1754, 1533, 1578,  450,  400,   61,   59,  489,
  490,  436,  492,  407,  374,  335,  441,  342,    0,  499,
  500,  501,  502,  503, 1573,  505, 1536,  387,  508,  509,
  510,  511,  512, 1785,  377,    0, 1788,  458, 1568,  359,
   40,  417,  387, 1573, 1706,  470,   59,  441, 1578,  356,
  279,  396,  377,    0,    0,  342, 1671, 1602, 1810,  316,
  317, 1606,  405,  383,  485,  408, 1597, 1597,  444,   42,
   43,  300,   45,   40,   47,   42,   43,   59,   45,  461,
  405,  424,  280,  408, 1746, 1646,  393,  285,  395,  260,
  377,   58, 1754,  413,   59,  440,   63, 1759,   41,  424,
  329,   44, 1632, 1633,  866,  301,  342,  450,  870,  454,
  649,  418,   59,   59,    0,    0, 1646,  378,  405,  348,
  424,  408,    0, 1785,    0,  450, 1788,  666,  357, 1791,
 1792,  670,  303, 1714,  363,    0,  443,  424,  677,  400,
  264,  377,  338,  682,  683,    0,  407,  316, 1810, 1680,
 1680,    0,  381,  692, 1816,  694,  695,  696,  697,  698,
  699,  700, 1184,  450, 1186, 1827, 1697, 1697, 1830,  405,
  279,    0,  408,   59,   59,    0,   41,  318,    0,   44,
  441,   59,  337,   59, 1714, 1715,    0,  460,  424,  452,
  729,  300, 1716,    0,   59,  318, 1716, 1659,    0, 1730,
 1730,  336,  321, 1759,   59,  328, 1737, 1737,  331, 1783,
   59,  407,   41,  752,  450,   44,  335,  340,  757,   41,
  329,  417,   44,  280,  343,  493,  494,    0,  285,    0,
   59,    0,   61,  340,   59, 1791, 1792,   59,    0,  348,
  359, 1772, 1772,  402, 1706,   59,  442,   40,  357, 1780,
 1780,  318,   59, 1783,  363,  402,   41,   59,   41,   44,
 1816,   44,   42,   43,  387,   45,   41,   47,  337,   44,
  320, 1827,  381,  396, 1830,   44,  461,  462, 1808,   59,
    0,  321,  321,  340, 1746, 1815,   59,  434,   59,  402,
   59,  383, 1754,  260,  413,  335,  335,   59,   41,  383,
  267,   44,  449, 1833,  343,  374,  506,  280,  436,  276,
   42,   43,  285,   45,  279,   47,  361,  440,  387,  359,
  359,   41,  289, 1785,   41,  450, 1788,   44,  295,  296,
   41,  454,  434,   44,   41,  300,  303,   44,  311,   59,
  879,  880,  881,  383,  417,  884,  885,  886, 1810,  888,
  889,  890,  891,  892,  893,  384,  323,  896,  897,  326,
  899,  442,  901,  454,  329,  904,  905,  906,  907,  908,
  909,  357,  460,  413,  413,   41,   41,   41,   44,   44,
   44,  287,  921,  348,  923,  352,  411,  354,  355,  665,
    0,  667,  357,  391,  392,  671,  672,  673,  363,  366,
    0,  489,  490,  370,  492,   42,   43,  469,   45,   44,
   47,  499,  500,  501,  502,  503,  381,  505,  461,  462,
  508,  509,  510,  511,  512,  457,  458,  459,  967,  258,
   40,   41,   42,   43,   44,   45,   46,   47,   61,   41,
   41,   41,   44,   44,   44,  274,  372,  286,   40,   59,
   60,   61,   62,  485,  486,  264,  278,   40,  280,   59,
  427,   41,  284,   44,   44,  287,  433,    0,   40,   42,
   43,  300,   45,   40,   47,   40,  443,  306,  300,   40,
  309,   91,   59,   93,   42,   43,   44,   45,   40,   47,
  457,  458,  459,  460,  461,  462,   41,  461,   40,   44,
  280,   40,   40,   40,  471,  285,  461,  461,   41,   40,
   40,   44,   40,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   40,  492,   59,   91,  495,  496,
  320,  498,  499,  500,  501,  502,  503,  504,  505,   46,
  436,  508,  509,  510,  511,  512,   44,   44,  280,  402,
  320,    0,  321,  285,   44,  428,  378,   46,   61,  328,
  340,   40,  331,  264,   44,  382,  335,  505,    0,  348,
   44, 1110,  278,  278,  343,  300,  264,  397,  400,  436,
  470,  397,  411, 1122, 1123,  407, 1125, 1126,  320,  418,
  359,  458,   41,  277, 1133,   44,   42,   43,  318,   45,
  447,   47,  447,  399,   40,  349,   40,  436,  328,   41,
   59,  331,   44, 1152,  383,   91,   40,   40,  318,  441,
  340, 1160,  264, 1162,  383,  411, 1165,   59, 1167,   61,
   44,  389, 1171, 1172, 1173, 1174,  382,  466,   40,   40,
  451,  470,   41,  280,  413,  436,  464,  317,  285,  418,
   40,  320,  262,  320,  264,  265, 1195,  452,  258,  448,
  264,  502,  272,   40,  458,  277, 1205,  387,  274,   40,
  280,  440,  300,  284,  311,  285,  396,  386,  288,  279,
  278,  344,  884,  452,  886,  454,  888,  889,  344,  891,
  892,  893,  302,  386,    0,  386,  418,  307,  308,    0,
  300,  311,  416,  300,   40, 1244,  306,  280,  318,  356,
  320,  321,  285,  417,  324,  418,   40,  274,  328,  417,
  440,  331,  280,  333,   40,  335,   41,  285,  264,  329,
  340,  341,  452,  343,  454,   41,   44,   44,   44,  280,
   41,   42,   43,   44,   45,   46,   47,  320,  348,  359,
  360,  320,  362,   59, 1293,   44,  385,  357,   59,   60,
   61,   62,  367,  363,   44,  320,  376,   40,  378,  379,
  380,  266,   44,  383,  328, 1314,  386,  387,   44,   40,
  299,  381,  501,   40, 1323,  318,  396,  264,  422,   41,
   91, 1330,   93,   58,   41,  328,   41,   41,  331,  264,
  410,  454,  440,  413,  394,  309,  492,  340,  309,   41,
  343,   41,    0,   40,   40,   40,  262,  352,  418,  429,
 1359,   40,   40,   40, 1363,  435,  258,   40,   40,  278,
  440,  280,   40,   40,  280,  284,   40,   40,  287,  285,
   40,  451,  452, 1382,  454,  277,  278,   40,  280,   40,
  460,  300,  284,   41,  387,  287,   44,   40,  262,  435,
  308,  327,   44,  396,   42,   43,   41,   45,  300,   47,
  388,   59,   42,   43,  306,   45,  388,   47,  388,  489,
  490,  343,  492,  460,  331,  274,  391,  446,  422,  499,
  500,  501,  502,  503,  390,  505,  271,   41,  508,  509,
  510,  511,  512,    0,   41,   42,   43,  440,   45,  287,
   47,  386,  489,  490,  407,  492,  386,  386,  386,  452,
  378,  454,  499,  500,  501,  502,  503,  386,  505,  378,
  386,  508,  509,  510,  511,  512,   41,  278,   40, 1478,
 1479,   41,   41,   41,   41,  417,  378,   44,   41,  308,
  435,  400,    0,   41,   42,   43,  385,   45,  407,   47,
 1499,  262,   59,  264,  265,  436,  396,  280,  400,  337,
  301,  272,  278,  338,  280,  407,  338,    0,  442,  280,
  301,  287,  442,   41,  285,   41,  418,  288,  352,  385,
  320,  371,  441,   41,   42,   43,   44,   45,   46,   47,
 1539,  302,  371,  304,  436,   41,  307,  308,   41,  441,
  311,   59,   60,   61,   62,   41,   41,  318,  280,  320,
  321,    0,   41,  324,   41,   41,   41,  328,   41,   41,
  331,   41,  333,   41,  335,   41,   59,   41,  470,  340,
  341,   41,  343,   91,   41,   93,   44,   41,   42,   43,
   46,   45,   59,   47,   40,  340,   59,   40,  359,  360,
   59,  362, 1601,   41,   42,   43,   59,   45,    0,   47,
 1609,   59,  378,  340,   59,  376,   41,  378,  379,  380,
   59,    0,  383,   59,  308,  386,  387,  265,   59,  352,
  278,  344,  280,   41,  400,  396,   40,  368,  508,  287,
  340,  407,  280,  451,  383,   44, 1645,  285, 1647,  410,
  280,   41,  413,   59,   59,  285,   41,   41,   44,  383,
  343,  435,   41,  383,  302,   44,  410,   59,  429,  301,
  304,  442,  387,   40,  435,  441,   59,    0,  293,  440,
   59,  304,   44,  280,  463,  307,   44,   41,  285,    0,
  451,  452,  337,  454,   41,   42,   43,   40,   45,  460,
   47,   41,   42,   43, 1703,   45, 1705,   47,  383,   41,
   42,   43,  383,   45,  264,   47,  458,  278,   41,  264,
  389,   44,  320,  264,  470,   44,   40,  309,  489,  490,
  378,  492,  280,  451,  271,  411,   59,  285,  499,  500,
  501,  502,  503,  264,  505,    0,  435,  508,  509,  510,
  511,  512,  400,  343,  262,  458,  264,  265,  343,  407,
  343,  318,  436,  343,  272,  458,  271,   41,   42,   43,
    0,   45,  280,   47,  331,  308,   41,  285,   40,  308,
  288,   40,   59,  340,   59,  308,   41,   42,   43,   44,
   45,   46,   47,  441,  302,   41,  279,  304,   59,  307,
  308,   41,   59,  311,   59,   60,   61,   62,   41,    0,
  318,   41,  320,  321,   44, 1187,  324,  300,    0,  258,
  328,    0,   12,  331,  271,  333,  280,  335,  263,   59,
  387,  285,  340,  341,  264,  343,   91,  710,   93,  396,
  279,  634,  280, 1087, 1085,  856,  329,  285,   87, 1074,
 1082,  359,  360, 1440,  362, 1441,   41,   42,   43,  356,
   45,  300,   47, 1064,  326,  348,  258,  306,  376,  321,
  378,  379,  380,  356,  357,  383, 1012,  311,  386,  387,
  363, 1075,  581,  440, 1643,  754,  789,  279,  396,  338,
  329,   42,   43, 1419,   45,  560,   47,  454,  381,  278,
 1721,  280,  410, 1421, 1763,  413,  338, 1725,  300,  348,
  393,  779,  395, 1724,  306, 1703, 1242,  356,  357, 1815,
 1664,  429, 1760,  999,  363, 1220, 1398,  435, 1680,  343,
  331,  562,  440,  280,  835,  418,  324,  329,  285, 1276,
  280,  827,  381,  451,  452,  285,  454,   13,  280,  298,
  620,  494,  460,  285,  498,  278,  348,  280,  743,  757,
  443,  141, 1634,  976,  287,  357,  729, 1522,   41,   42,
   43,  363,   45,    0,   47,  730, 1780,  300, 1833,  418,
 1737,  489,  490,  509,  492,  968, 1124,   -1,  718,  381,
  429,  499,  500,  501,  502,  503,   -1,  505,    0,  378,
  508,  509,  510,  511,  512, 1231,  280,  262,    0,  264,
  265,  285, 1167,   -1,   -1,   -1,   -1,  272,   -1,   -1,
   -1,  400,   -1,   -1,   -1,  280,  418,   -1,  407,   -1,
  285,   -1,   59,  288,   -1,   -1,   -1,   -1,   -1,   41,
   42,   43,   44,   45,   46,   47,   -1,  302,  278,   -1,
  280,   -1,  307,  308,   -1,  378,  311,   59,   60,   61,
   62,   -1,  441,  318,   -1,  320,  321,   59,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,  400,  333,   -1,
  335,   -1,   -1,   -1,  407,  340,  341,   -1,  343,   91,
   -1,   93,   -1,   -1,   -1,  280,   -1,   -1,   -1,   -1,
  285,   -1,   -1,   -1,  359,  360,   -1,  362,   41,   42,
   43,  262,   45,   -1,   47,   -1,   -1,   -1,  441,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,  280,
   -1,  386,  387,   -1,  285,   -1,   41,   42,   43,   -1,
   45,  396,   47,   -1,   -1,   -1,   -1,   -1,  378,   -1,
   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   -1,
   41,   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,
  400,   -1,   -1,   -1,  429,   -1,   -1,  407,   42,   43,
  435,   45,   -1,   47,   -1,  440,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   59,  451,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,  460,   -1,  280,   -1,   -1,
   -1,  441,  285,   -1,   -1,   -1,   41,   42,   43,   -1,
   45,   -1,   47,   41,   42,   43,   -1,   45,   -1,   47,
   -1,  258,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,    0,  279,  508,  509,  510,  511,  512,   -1,   -1,
  262,   -1,  264,  265,   -1,   -1,  258,   -1,   -1,  305,
  272,   -1,   -1,  300,   -1,   -1,  312,   -1,  280,  306,
  316,  317,   -1,  285,   -1,   -1,  288,  279,   -1,  325,
   -1,   -1,   41,   42,   43,   44,   45,   -1,   47,   -1,
  302,  337,  329,   -1,   -1,  307,  308,   -1,  300,  311,
   59,   60,   61,   62,  306,   -1,  318,   -1,  320,  321,
   -1,  348,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
  357,  333,   -1,  335,   -1,   -1,  363,  329,  340,  341,
   -1,  343,   -1,   -1,   93,   -1,   -1,  280,   -1,   41,
   42,   43,  285,   45,  381,   47,  348,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,  357,  402,   -1,   -1,   -1,
   -1,  363,   -1,   -1,  376,  280,  378,  379,  380,   -1,
  285,  383,   -1,  419,  386,  387,   -1,    0,   -1,  381,
   -1,  418,   -1,   -1,  396,   -1,   -1,   -1,  434,  280,
   -1,   -1,  438,  260,  285,   -1,   -1,   -1,  410,   -1,
    0,  413,   -1,  449,   -1,   -1,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,   -1,  280,  418,  429,   41,   -1,
  285,   44,  468,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,  301,   -1,   59,   -1,   -1,  451,
  452,   41,  454,   -1,   44,  280,  313,   -1,  460,   -1,
  285,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,   59,
  327,   61,   -1,   -1,   -1,  294,   -1,   -1,   -1,   -1,
   -1,  338,   -1,    0,   -1,   -1,  305,  489,  490,   -1,
  492,   -1,   -1,  312,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  262,   -1,  264,  265,   -1,  337,   -1,
   -1,   -1,   -1,  272,   41,   42,   43,   44,   45,   -1,
   47,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
  407,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
  417,   -1,  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   93,   -1,   -1,  328,
   -1,   -1,  331,  402,  333,  442,  335,   -1,  280,   -1,
   -1,  340,  341,  285,  343,  472,  473,  474,  475,  476,
  477,  478,  479,  460,   -1,   -1,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   -1,  434,   -1,   -1,   -1,  438,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,  489,  490,  383,  492,   -1,  386,  387,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  396,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,   -1,   -1,   -1,  258,  264,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,  278,   -1,
  280,  440,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,
   -1,   -1,  451,  452,   -1,  454,   -1,   -1,  321,   -1,
  300,  460,   -1,   -1,   -1,  328,  306,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,  262,  359,  264,  265,   -1,
  499,  500,  501,  502,  503,  272,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,  285,   -1,
  383,  288,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   -1,   47,   -1,  302,   -1,   -1,  378,   -1,
  307,  308,   -1,   -1,  311,   59,   60,   61,   62,   -1,
  413,  318,   -1,  320,  321,   -1,   -1,  324,   -1,  273,
  400,  328,   -1,  277,  331,   -1,  333,  407,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,  440,  418,   93,
   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,  452,
   -1,  454,  359,  360,   -1,  362,  436,   -1,   -1,   -1,
   -1,  441,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,  460,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,  396,
  470,  345,  346,  347,   -1,  349,  350,  351,  260,  353,
   -1,   -1,   -1,  410,  489,  490,  413,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,  373,
  505,  375,  429,  508,  509,  510,  511,  512,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  301,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,  313,  406,  460,   -1,   -1,   -1,  294,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  305,  423,
   -1,   -1,   -1,   -1,   -1,  312,  338,   -1,    0,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  325,   -1,
   -1,  445,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,
  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,  272,   41,
   42,   43,   44,   45,   -1,   47,  480,  481,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,  497,   -1,  407,   -1,   -1,  302,  386,
   -1,   -1,   -1,  307,  308,  417,   -1,  311,    0,   -1,
   -1,   -1,   -1,   -1,  318,  402,  320,  321,   -1,   -1,
  324,   93,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
  442,  335,  419,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  434,  460,   41,
   -1,  438,   44,   -1,  441,  359,  360,   -1,  362,   -1,
   -1,   -1,  449,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,  468,  386,  387,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  396,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   41,
   42,   43,   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,
   62,    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
  262,   -1,  264,  265,   -1,  499,  500,  501,  502,  503,
  272,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   45,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   43,   44,   45,   -1,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,  311,
   59,   60,   61,   62,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,  331,
   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   93, 1315, 1316,   -1,   -1,   -1,
   -1, 1321, 1322,   -1, 1324, 1325,   -1,  359,  360,   -1,
  362,   -1, 1332, 1333, 1334, 1335,   -1, 1337, 1338, 1339,
 1340, 1341, 1342,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  410,  331,
   -1,  413,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
  287,   -1,   -1,  435,   -1,   -1,   -1,  359,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,
  272,  383,   -1,   -1,   -1,  387,   -1,   -1,  280,   -1,
   -1,   -1,   -1,  285,  396,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,  489,  490,   -1,
  492,  413,   -1,   -1,   -1,   -1,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  324,  262,   -1,  264,  265,   -1,  440,   -1,
   -1,  333,   -1,  272,   41,   -1,   43,   44,   45,  341,
  452,   -1,  454,  295,   -1,   -1,   -1,   -1,   -1,  288,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,  360,   -1,
  362,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
   -1,  323,  311,    0,  376,   -1,  378,  379,  380,  318,
   -1,  320,  321,   -1,   -1,  324,   93,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
  352,  340,  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   41,   -1,   -1,   44,   -1,   -1,
  359,  360,   -1,  362,   -1,   -1,   -1,  429,   -1,   -1,
   -1,   -1,   59,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,  489,  490,  383,  492,   -1,  386,  387,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  396,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,  427,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  429,  443,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,  457,  458,  459,   -1,  461,
  462,   -1,  451,  452,   -1,  454,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,   -1,   -1,   -1,
   -1,   -1,   -1,  495,  496,   -1,    0,   -1,   -1,   -1,
  489,  490,  504,  492,   -1,  262,   -1,  264,  265,   -1,
  499,  500,  501,  502,  503,  272,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,
   44,   45,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   -1,  311,   59,   60,   61,   62,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   93,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,  410,  331,   -1,  413,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   -1,  287,   -1,   -1,  435,   -1,
   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,  383,   -1,   -1,   -1,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  262,   -1,
  264,  265,   -1,  440,   -1,   -1,   -1,   -1,  272,   41,
   -1,   43,   44,   45,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,   59,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  302,   -1,
  328,   -1,   -1,  307,  308,   -1,   -1,  311,   -1,   -1,
   -1,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   93,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  396,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,
   41,   -1,   -1,   44,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   59,   -1,
   61,  435,  460,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  451,  452,   -1,
  454,   -1,   -1,   -1,   -1,   -1,  460,   -1,   58,   -1,
   91,  489,  490,   63,  492,   -1,   -1,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  489,  490,   -1,  492,   -1,
  262,   -1,  264,  265,   -1,  499,  500,  501,  502,  503,
  272,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  302,   -1,   -1,   -1,   -1,  307,  308,   40,   -1,  311,
   43,   -1,   45,   -1,   -1,   -1,  318,   -1,  320,  321,
   -1,   -1,  324,   -1,  434,   58,  328,   -1,   -1,  331,
   63,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  460,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,
  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,  489,
  490,  383,  492,   -1,  386,  387,   -1,   -1,   -1,  499,
  500,  501,  502,  503,  396,  505,   -1,  258,  508,  509,
  510,  511,  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,
   -1,  413,   -1,  274,  443,   -1,  277,  278,   -1,  280,
   -1,   -1,   -1,  284,   -1,   -1,  287,  429,   -1,   -1,
   -1,  460,   -1,  435,   -1,   -1,   -1,  267,  440,  300,
   -1,   -1,   -1,   -1,   -1,  306,  276,   -1,  309,  451,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,  289,
  489,  490,   -1,  492,   -1,  295,  296,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,  314,   -1,   -1,  489,  490,   -1,
  492,   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,
  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,  366,   -1,   -1,  400,
  370,   -1,   -1,  276,   -1,   -1,  407,   -1,  378,   -1,
  411,   -1,   -1,   -1,   -1,   -1,  289,  418,   -1,  292,
   -1,   -1,  295,  296,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,  314,   -1,   58,   -1,   -1,   -1,  417,   63,   -1,
  323,  421,   -1,  326,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,  466,   -1,   -1,   -1,  470,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,  460,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,  513,  514,  515,   -1,   -1,  421,   -1,
   -1,   -1,  489,  490,  427,  492,   -1,   -1,   -1,   -1,
  433,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  443,  508,  509,  510,  511,  512,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,  471,   -1,
   63,   -1,   -1,  328,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,
   -1,   -1,  267,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,  402,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,
   -1,   -1,   -1,   40,   41,   -1,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   59,  438,   61,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,  449,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  460,   -1,  352,   -1,  354,
  355,   -1,   -1,  468,   91,   -1,   -1,   58,   -1,   -1,
   -1,  366,   63,   -1,   -1,  370,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,  276,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  366,    0,   -1,   -1,  370,   -1,   -1,
   -1,  258,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,
  277,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,
  287,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  300,   -1,   -1,  267,  354,  421,  306,
   -1,   -1,  309,   59,  427,  276,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
  443,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,  378,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  354,  355,  508,  509,  510,  511,  512,
   -1,   -1,   -1,  400,   -1,  366,   -1,   -1,   -1,  370,
  407,  458,   -1,  460,  411,   40,   -1,  378,   43,   -1,
   45,  418,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,  436,
   -1,   -1,  489,  490,  441,  492,   -1,   -1,   -1,  343,
   -1,  498,  499,  500,  501,  502,  503,   -1,  505,   -1,
  421,  508,  509,  510,  511,  512,  427,   -1,   -1,  466,
   -1,   -1,  433,  470,  368,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,  426,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,
   -1,   -1,  318,  295,  296,  321,   -1,   -1,   -1,   -1,
    0,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  314,   -1,  340,  489,  490,  343,  492,   -1,
   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,  503,
   -1,  505,   -1,  359,  508,  509,  510,  511,  512,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,  260,   -1,  383,  263,   59,
  386,  387,  267,   -1,  366,   -1,   -1,   -1,  370,   -1,
  396,  276,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,  413,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,  435,
   -1,   -1,   63,   -1,  440,   -1,   -1,   -1,  323,  421,
   -1,  326,   -1,   -1,   -1,  427,  452,   -1,  454,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   58,
   -1,   -1,  262,   -1,   63,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,  276,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,  289,   -1,
  340,   -1,   -1,  343,  295,  296,   -1,   -1,   -1,  460,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,  359,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,  323,   63,   -1,  326,   -1,   -1,  489,  490,
   -1,  492,  460,  383,   -1,   -1,  386,  387,  499,  500,
  501,  502,  503,   -1,  505,   -1,  396,  508,  509,  510,
  511,  512,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  489,  490,  413,  492,  366,   -1,   -1,   -1,  370,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,
   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  289,   -1,  443,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  354,  355,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,
  260,  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,  289,
   47,   -1,   -1,   -1,   40,  295,  296,   43,   -1,   45,
   -1,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,   -1,   -1,  425,   63,  427,   -1,
   -1,   -1,   -1,  323,  433,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   58,   -1,   -1,   -1,   -1,   63,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  260,  272,   -1,  263,   -1,   -1,
   -1,  267,   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,
  276,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,  324,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  333,  323,   63,   -1,
  326,   -1,   -1,   -1,  341,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,  362,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
  366,  378,  379,  380,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  263,
   -1,   -1,  429,  267,   -1,   -1,   -1,   -1,   -1,  425,
   -1,  427,  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  354,  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,  260,  370,   -1,  263,   -1,
   -1,   -1,  267,   -1,   -1,    0,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,
  295,  296,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   58,   -1,   44,
   -1,  425,   63,  427,   -1,   -1,   -1,   -1,  323,  433,
   -1,  326,   -1,   -1,   59,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  425,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,  260,
   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,
   -1,   42,   43,   -1,   45,  276,   -1,  262,   -1,   -1,
   -1,   -1,   -1,  261,   -1,   -1,   -1,   58,  289,   -1,
   -1,   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  282,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  290,   -1,   -1,   -1,   -1,   -1,   -1,  297,
   -1,  299,  323,  301,   -1,  326,   -1,   -1,  306,   -1,
   -1,   -1,   -1,  318,   -1,  313,  321,   -1,   -1,   -1,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,  327,
  335,   -1,   -1,  354,  355,  340,   -1,   -1,  343,   -1,
  338,   -1,   -1,   -1,   -1,  366,   -1,   40,   -1,  370,
   43,   -1,   45,   -1,  359,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,  405,  413,   -1,
   -1,   -1,  433,   -1,  412,   -1,   -1,  415,   -1,  417,
  418,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,  457,  458,  459,  460,
  461,  462,   -1,   -1,  442,   -1,   -1,  452,  273,  454,
  471,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
  295,  492,   -1,  298,  495,  496,  267,  498,  499,  500,
  501,  502,  503,  504,  505,  276,   -1,  508,  509,  510,
  511,  512,   -1,  491,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,  506,  507,
   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,  370,
   -1,  406,   -1,  276,   -1,   -1,   40,   -1,   42,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  423,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  480,  481,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  497,  366,   -1,   -1,   -1,  370,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,
  433,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,  273,   -1,   58,   -1,  277,   -1,
   -1,   63,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,  471,  298,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
  339,   -1,  276,   -1,   -1,   -1,  345,  346,  347,   -1,
  349,  350,  351,   -1,  353,  289,    0,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   40,   41,   -1,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,
   -1,   -1,  411,   -1,   -1,   59,   -1,   61,  352,   -1,
  354,  355,   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  445,   91,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,
   -1,  480,  481,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,  276,   -1,   -1,   40,  497,  433,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,  303,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  354,  355,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   58,   -1,  258,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  274,   -1,   -1,  277,  278,   -1,  280,   -1,   -1,   -1,
  284,   -1,   -1,  287,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,
   -1,  443,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,  400,   -1,   -1,   -1,
   -1,   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  267,  466,  366,   -1,   -1,  470,  370,   -1,   -1,
  276,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,
  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   59,   -1,
   61,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   91,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  267,   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   40,   41,   -1,   43,   -1,   45,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,   -1,  280,
  417,   -1,   -1,  284,   -1,   -1,  287,   -1,   -1,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,  300,
   -1,   -1,   -1,   -1,   -1,  306,  443,   -1,  309,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,  378,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,  400,
   -1,   -1,   -1,    0,   -1,   -1,  407,   -1,   -1,   -1,
  411,   -1,   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,
  441,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,  267,
   -1,   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,
   -1,   40,   59,   -1,   43,  466,   45,   -1,  366,  470,
   -1,  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,  318,   -1,   63,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,
   -1,   -1,   -1,  340,  323,   -1,  343,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  383,  366,   -1,  386,
  387,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,  396,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,   -1,  433,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,  291,   -1,   -1,   -1,
  295,   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,  339,   -1,  276,   -1,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,   -1,  354,  355,  276,   -1,  423,   40,
   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,
  370,   -1,   -1,   -1,   -1,  295,  296,   58,   59,   -1,
  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  417,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,  497,  433,  354,  355,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,  427,  508,  509,
  510,  511,  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  276,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,  273,   -1,   -1,   58,  277,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  291,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  339,
   -1,   -1,  443,   -1,   -1,  345,  346,  347,   -1,  349,
  350,  351,   -1,  353,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,  406,  508,  509,  510,
  511,  512,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,
  480,  481,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,  276,   -1,   -1,  497,   41,   -1,
   -1,   44,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   -1,   59,   -1,   61,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   91,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   91,
   -1,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,  258,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  274,   -1,   -1,   -1,  278,  258,  280,   -1,   -1,
   -1,  284,   -1,   -1,  287,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  274,   -1,   -1,   -1,  278,  300,  280,   -1,
   -1,   -1,  284,  306,   -1,  287,  309,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,
   -1,   -1,   -1,   -1,  306,   -1,   -1,  309,   41,   42,
   43,   44,   45,   46,   47,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   59,   60,   61,   62,
  469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,   -1,  492,   -1,  378,  495,  496,   91,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  378,  400,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,
   -1,   -1,   -1,   -1,   -1,  418,   -1,   -1,  400,   -1,
   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,   -1,  411,
    0,   -1,   -1,  436,   -1,   -1,  418,   -1,  441,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,  460,  466,   -1,   -1,   -1,  470,   59,
   60,   61,   62,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,   -1,   -1,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,   91,  499,  500,  501,  502,  503,  504,  505,  506,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  272,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   60,   61,   62,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,  324,    0,   -1,   -1,  328,   -1,   -1,  331,   -1,
  333,   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,
  343,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,   -1,
  383,   59,   -1,  386,  387,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,    0,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  280,   -1,   -1,   -1,   -1,  285,  429,   -1,  288,   -1,
   -1,   -1,  435,   -1,   -1,   41,   -1,  440,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   59,    0,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,   -1,   -1,   -1,   -1,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   59,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,  272,  383,   -1,   -1,  386,  387,   -1,   41,
  280,   -1,   44,   -1,   -1,  285,  396,   -1,  288,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,  429,
   -1,  321,   -1,   -1,  324,  435,   -1,   -1,  328,   -1,
  440,  331,   -1,  333,  262,  335,   -1,   -1,   -1,   -1,
  340,  341,  452,  343,  454,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,  359,
  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,    0,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,  396,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,   -1,  262,  335,   -1,   -1,
   -1,   -1,  340,  413,   41,  343,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  429,
   -1,  359,   59,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   59,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,  452,   -1,  454,  383,   -1,   -1,  386,  387,
   -1,   -1,  318,   -1,   -1,  321,  262,   -1,  396,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,  413,   -1,  343,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  262,   -1,   -1,  359,   -1,   -1,   59,  435,   -1,   -1,
   -1,    0,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  318,   -1,  452,  321,  454,  383,   -1,   -1,
  386,  387,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
  396,   -1,   -1,   -1,  340,    0,   -1,  343,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,  318,  413,   -1,  321,
   -1,   -1,   -1,  359,   -1,   -1,  328,   -1,   -1,  331,
   59,   -1,  262,  335,   -1,   -1,   -1,   -1,  340,  435,
   -1,  343,   -1,   -1,  440,   -1,   41,  383,   -1,   44,
  386,  387,   -1,   -1,   -1,   -1,  452,  359,  454,   -1,
  396,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,  396,  262,   -1,   -1,  328,  435,
   -1,  331,   -1,   -1,  440,  335,  262,   -1,   -1,   -1,
  340,  413,   -1,  343,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,  435,   -1,   -1,   -1,    0,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,  318,  454,  383,  321,   -1,  386,  387,    0,   -1,
   -1,  328,  318,   -1,  331,  321,  396,   -1,  335,  262,
   -1,   -1,  328,  340,   -1,  331,  343,   -1,   41,  335,
   -1,   44,   -1,  413,  340,   -1,   -1,  343,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,   59,   -1,   -1,   41,
    0,   -1,   44,  359,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,  383,   59,   -1,  386,
  387,    0,  452,   -1,  454,  318,   -1,  383,  321,  396,
  386,  387,   -1,  262,   -1,  328,   -1,   -1,  331,   -1,
  396,   41,  335,   -1,   44,   -1,  413,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,  413,   -1,   59,
   -1,   -1,   41,   -1,   -1,   44,  359,  262,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  435,
   59,   -1,   -1,   -1,  440,  452,   -1,  454,   -1,  318,
  383,   -1,  321,  386,  387,   -1,  452,   -1,  454,  328,
   -1,   -1,  331,  396,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,
  359,   -1,   -1,  328,   -1,   -1,  331,    0,   -1,   -1,
  335,   -1,  435,   -1,   -1,  340,   -1,  440,  343,   -1,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,  452,
   -1,  454,   -1,   -1,  359,   -1,   -1,  396,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,   -1,  413,   -1,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   59,   -1,   -1,  262,
   -1,  396,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   41,  440,   -1,   44,   -1,   -1,   -1,   -1,  413,   -1,
  262,   -1,   -1,  452,   -1,  454,   -1,   -1,   59,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,    0,   -1,  440,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  318,   -1,  452,  321,  454,
   -1,   -1,  262,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  318,  340,   -1,  321,
  343,   -1,   -1,  262,   41,   -1,  328,   44,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,  359,   -1,  340,   -1,
   -1,  343,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  318,   -1,
  383,  321,   -1,  386,  387,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,  396,   -1,  335,   -1,   -1,   -1,  318,
  340,  383,  321,  343,  386,  387,   -1,   -1,   -1,  328,
  413,   -1,  331,   -1,  396,   -1,  335,   -1,   -1,  359,
   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,
   -1,  413,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
  359,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,  452,
   -1,  454,   -1,  435,   -1,   -1,  396,   -1,  440,  262,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
  452,   -1,  454,  413,   -1,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  413,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  318,  435,   -1,  321,   -1,
   -1,  440,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,  452,   -1,  454,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,  359,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,  262,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,  359,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,  396,  271,   -1,   -1,   -1,
   -1,  328,  435,   -1,  331,   -1,   -1,  440,  335,   -1,
   -1,   -1,  413,  340,   -1,   -1,  343,   -1,   -1,  452,
  271,  454,   -1,   -1,   -1,   -1,  301,   -1,   -1,   -1,
   -1,   -1,  359,  308,  435,   -1,   -1,  312,  313,  440,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,
  301,  452,   -1,  454,   -1,   -1,  383,  332,   -1,  386,
  387,  312,  313,  338,  315,   -1,   -1,  318,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  332,   -1,   -1,   -1,   -1,  413,  338,   -1,   -1,
   -1,   -1,   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,  301,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,  315,   -1,
   -1,  318,  417,   -1,   -1,   -1,   -1,  398,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  332,   -1,  432,   -1,   -1,
   -1,  338,   -1,   -1,   -1,   -1,  417,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,
   -1,  432,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,   -1,  273,   -1,   -1,
  451,  277,  453,   -1,   -1,   -1,   -1,   -1,  284,  460,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  295,
   -1,  398,  298,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  489,  490,
  417,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,  432,   -1,  508,  509,  510,
  511,  512,   -1,  339,   -1,  442,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,  453,  353,  273,   -1,
   -1,   -1,  277,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
  406,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  339,   -1,   -1,  423,   -1,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,
  375,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,
   -1,  406,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,  497,   -1,  499,  500,  501,  502,  503,  423,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 515
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
"OUTER","OUTPUT_TYPE","OVERFLOW","PAGE","PAGES","KW_PAGE_SIZE","PARAMETER",
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
"KW_UPDATING","KW_DELETING",
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
"db_initial_option : KW_PAGE_SIZE equals pos_short_integer",
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
"exec_sql : EXECUTE STATEMENT value ';'",
"for_select : FOR select INTO variable_list cursor_def DO proc_block",
"for_exec_into : FOR EXECUTE STATEMENT value INTO variable_list DO proc_block",
"exec_into : EXECUTE STATEMENT value INTO variable_list ';'",
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
"non_reserved_word : INSERTING",
"non_reserved_word : UPDATING",
"non_reserved_word : DELETING",
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
    sym = HSHD_lookup (NULL_PTR, last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
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
case 314:
{ yyval = make_node (nod_for_select, e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], NULL); }
break;
case 315:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-4], yyvsp[0], make_list(yyvsp[-2])); }
break;
case 316:
{ 
				yyval = make_node (nod_exec_into, e_exec_into_count, yyvsp[-3], 0, make_list(yyvsp[-1])); }
break;
case 317:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 318:
{ yyval = make_node (nod_if, e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 319:
{ yyval = make_node (nod_post, e_pst_count, yyvsp[-2], yyvsp[-1]); }
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
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 335:
{ yyval = make_node (nod_breakleave, e_breakleave_count, NULL, NULL); }
break;
case 336:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 337:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_on_error, e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 342:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 343:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 344:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 345:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 346:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 347:
{ yyval = make_node (nod_exec_procedure, e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 348:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 349:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 350:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 351:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 353:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 355:
{ yyval = make_list (yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 361:
{ yyval = yyvsp[-1]; }
break;
case 362:
{ lex.beginning = lex_position(); }
break;
case 363:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 364:
{ lex.beginning = lex.last_token; }
break;
case 365:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 366:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 367:
{ yyval = 0; }
break;
case 368:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 369:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 370:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 371:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 372:
{ yyval = NULL; }
break;
case 373:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 374:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 375:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 376:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 377:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 392:
{ yyval = NULL; }
break;
case 393:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 394:
{ yyval = yyvsp[0]; }
break;
case 395:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 396:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 397:
{ yyval = yyvsp[0]; }
break;
case 398:
{ yyval = yyvsp[0]; }
break;
case 399:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 400:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 401:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 402:
{ yyval = yyvsp[0]; }
break;
case 403:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 405:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 406:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 407:
{ yyval = yyvsp[0]; }
break;
case 408:
{ yyval = yyvsp[0]; }
break;
case 409:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 410:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 414:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 415:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 416:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 417:
{ yyval = yyvsp[0]; }
break;
case 418:
{ yyval = yyvsp[0]; }
break;
case 419:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 420:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 421:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 422:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 446:
{ yyval = NULL; }
break;
case 447:
{ yyval = NULL; }
break;
case 448:
{ yyval = NULL; }
break;
case 449:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 450:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 451:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 452:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 453:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 454:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 455:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 456:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 458:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 459:
{ yyval = yyvsp[0]; }
break;
case 460:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 461:
{ yyval = yyvsp[0]; }
break;
case 462:
{ yyval = yyvsp[0]; }
break;
case 464:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 465:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 467:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = yyvsp[0]; }
break;
case 471:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 472:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 473:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 474:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 477:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 487:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 488:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 490:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 491:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 492:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 498:
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
case 499:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 500:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 501:
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
case 502:
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
case 503:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 506:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 507:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 508:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 510:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 511:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 512:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 513:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 514:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 515:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 517:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 518:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 519:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 531:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 532:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 533:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 534:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 535:
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
case 536:
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
case 539:
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
case 540:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 541:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 543:
{ yyval = yyvsp[-1]; }
break;
case 544:
{ yyval = 0; }
break;
case 548:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 549:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 550:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 554:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 555:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 556:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 557:
{ yyval = 0; }
break;
case 558:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 561:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 562:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 565:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 566:
{ yyval = (DSQL_NOD) NULL; }
break;
case 568:
{ yyval = (DSQL_NOD) NULL; }
break;
case 569:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 571:
{ yyval = (DSQL_NOD) NULL; }
break;
case 573:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 578:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 579:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 580:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 581:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 582:
{ yyval = yyvsp[0];}
break;
case 584:
{ yyval = yyvsp[0];}
break;
case 585:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 586:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 587:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 588:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 589:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 590:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 591:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 592:
{ yyval = 0; }
break;
case 593:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 594:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 595:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 596:
{ yyval = (DSQL_NOD) 0; }
break;
case 597:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 598:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 600:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 601:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 602:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT) yyvsp[-1] | (SSHORT) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 603:
{ yyval = 0; }
break;
case 605:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 606:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 607:
{ yyval = make_node (nod_select, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 608:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 609:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 610:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 611:
{ yyval = make_list (yyvsp[0]); }
break;
case 612:
{ yyval = 0; }
break;
case 614:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 615:
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 616:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 618:
{ yyval = 0; }
break;
case 619:
{ yyval = MAKE_constant((STR)NOD_NULLS_FIRST, CONSTANT_SLONG); }
break;
case 620:
{ yyval = MAKE_constant((STR)NOD_NULLS_LAST, CONSTANT_SLONG); }
break;
case 621:
{ yyval = yyvsp[-1]; }
break;
case 622:
{ yyval = 0; }
break;
case 623:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 624:
{ yyval = 0; }
break;
case 625:
{ yyval = yyvsp[0]; }
break;
case 626:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 627:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 628:
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 630:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 631:
{ lex.limit_clause = true; }
break;
case 632:
{ lex.limit_clause = false; }
break;
case 633:
{ lex.first_detection = true; }
break;
case 634:
{ lex.first_detection = false; }
break;
case 635:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 636:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 638:
{ yyval = 0; }
break;
case 639:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 640:
{ yyval = yyvsp[-2]; }
break;
case 641:
{ yyval = yyvsp[-1]; }
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
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 764:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 767:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 773:
{ yyval = yyvsp[-1]; }
break;
case 774:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 785:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 794:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 811:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 813:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 815:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 817:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 818:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 819:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 820:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 821:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 822:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 823:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 824:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 825:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 828:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 829:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 836:
{ yyval = make_list (yyvsp[-1]); }
break;
case 837:
{ yyval = yyvsp[-1]; }
break;
case 838:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 839:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 849:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 850:
{ yyval = yyvsp[0]; }
break;
case 851:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 852:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 853:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 854:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 855:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 856:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 857:
{ yyval = yyvsp[-1]; }
break;
case 858:
{ yyval = yyvsp[-1]; }
break;
case 862:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 863:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 864:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 865:
{ yyval = yyvsp[0]; }
break;
case 866:
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
case 867:
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
case 868:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 869:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 871:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 873:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 874:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 880:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 881:
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
case 882:
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
case 883:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 884:
{ yyval = make_parameter (); }
break;
case 885:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 886:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 887:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 888:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = yyvsp[0]; }
break;
case 894:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 896:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 897:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 898:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 899:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 902:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 903:
{ yyval = yyvsp[0];}
break;
case 908:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 909:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 910:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 911:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 915:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 916:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 919:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 920:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 921:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 922:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 923:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 924:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 926:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 929:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 930:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 933:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 934:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 935:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 936:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 937:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 938:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 939:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 940:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 944:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 954:
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
