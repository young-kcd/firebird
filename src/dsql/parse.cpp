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
	int  prev_keyword;
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
  190,  191,  192,  203,  203,  193,  198,  199,  194,  194,
  195,  206,  206,  196,  207,  201,  201,  201,  202,  202,
  202,  204,  204,  204,  204,  197,  200,  200,  205,  205,
  187,  187,  212,  213,  213,  214,  214,  214,  214,   11,
   71,   80,  216,  219,  219,  219,  220,  223,  227,  227,
  228,  228,  229,  229,  147,  218,   87,   89,  217,  217,
   70,   83,  232,  232,  232,  233,  236,  236,  237,  237,
  237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
  237,  237,  237,  234,  234,  235,    2,  239,  239,  239,
  239,  239,  239,  239,  247,  248,  245,  245,  249,  249,
  249,  249,  249,  249,  249,  240,  240,  250,  250,  250,
  250,  250,  250,  250,  244,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  255,  255,
  255,  255,  255,  255,  255,  255,  255,  255,  252,  252,
  254,  254,  253,  251,  251,  251,  246,  246,  242,  243,
  243,  257,  257,  257,  257,  258,  258,  241,  259,  259,
  260,  260,    8,  261,  261,  261,  261,  261,  261,  261,
  261,  261,  261,  261,  261,  101,  101,  144,  144,  264,
  264,  267,  267,  269,  269,   53,   53,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  274,  274,  265,  265,
  265,  265,  275,  275,   61,   61,   61,   55,   55,  271,
  271,  271,  268,  268,  268,  279,  279,  279,  278,  278,
  277,  277,  277,  272,  272,  282,  280,  280,  280,  281,
  281,  273,  273,  273,  273,  283,  283,   18,   18,   18,
  285,  285,  285,   16,   16,   16,  287,  288,  289,  292,
  292,    4,   15,  291,  291,  293,  293,  294,  294,  284,
  295,  295,  296,  296,  297,  297,  297,  297,  298,  298,
  299,  299,  300,  300,  302,  302,  302,  303,  303,  303,
  304,  304,  304,  301,  306,  306,  306,  307,  307,  305,
  305,  308,  310,  310,  309,  309,  286,   17,  311,  311,
  311,  312,  312,  315,  315,  316,   65,   65,   65,  318,
  318,  317,  317,  313,  313,  321,  321,  322,  322,  314,
  326,  327,  328,  319,  320,  323,  323,  323,  323,  329,
  329,  329,  330,  330,  330,  221,  221,  222,  222,  333,
  333,  334,  334,  334,  324,  337,  337,  338,  338,  339,
  339,  340,  340,  341,  341,  209,  209,  343,  343,  230,
  230,   31,  231,  231,  231,  231,  231,  231,  231,  231,
  225,  225,  344,  344,  345,  345,  345,  345,  345,  347,
  347,  347,  226,  226,  224,  224,  325,  325,  351,  352,
  352,  352,  352,  352,  353,  353,  354,  354,  355,  355,
  356,  356,  356,  357,  357,   10,   10,  359,  359,    7,
    7,  360,  361,  362,   19,   19,  363,  364,  365,  365,
  188,  335,  335,    3,    3,  367,  367,  367,  369,  369,
   61,  368,  368,  370,  370,   34,   34,   85,   84,   84,
  358,  358,  371,  372,  372,  210,  210,  210,  146,  366,
  366,  111,  111,  111,  111,  111,  376,  376,  376,  376,
  376,  376,  375,  375,  375,  378,  378,  378,  378,  378,
  378,  378,  378,  378,  378,  379,  379,  379,  379,  379,
  379,  379,  379,  384,  384,  384,  384,  384,  384,  384,
  384,  384,  384,  384,  384,  384,  384,  384,  384,  390,
  390,  380,  380,  381,  381,  381,  381,  382,  382,  386,
  386,  387,  387,  387,  387,  385,  388,  383,  383,  374,
  374,  374,  377,  377,  377,  391,  391,  392,  389,  394,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,  156,  156,  156,  395,
  393,  393,  151,  151,  399,  399,  399,  399,  399,  397,
  397,  397,  397,  397,  331,  152,  152,  153,  154,  154,
  154,  154,  154,   51,   51,   20,   20,  238,  400,   54,
  123,  270,  270,   95,  396,  396,  396,  396,  401,  401,
  401,  401,  401,  401,  401,  401,  401,  401,  401,  402,
  348,  349,  349,  349,  346,  346,  398,  350,  350,  404,
  404,  405,  405,  406,  406,  409,  409,  407,  407,  412,
  412,  411,  408,  410,  403,  403,  403,  403,  403,  403,
  403,  403,  332,  332,  155,   48,  276,  122,  148,  373,
  165,  211,  263,   63,   60,  215,  112,   66,  336,   35,
   41,  342,  262,   39,   42,  208,   40,  290,  256,  256,
  413,  413,  413,  413,  413,  413,  413,  413,  413,  413,
  413,  413,  413,  413,
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
    7,    7,    1,    1,    3,    4,    7,    2,    1,    3,
    1,    1,    6,    3,    0,    0,    0,    0,    3,    0,
    9,    9,    1,    1,    0,    2,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    5,    5,    5,
    5,    5,    5,    2,    0,    4,    2,    3,    3,    2,
    2,    3,    3,    2,    3,    6,    1,    2,    4,    3,
    2,    2,    2,    2,    3,    1,    3,    3,    3,    2,
    2,    4,    4,    5,    1,    1,    1,    1,    1,    1,
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
    5,    5,    6,    1,    1,    1,    2,    3,    5,    1,
    0,    3,    2,    1,    0,    2,    0,    1,    0,    3,
    1,    0,    1,    2,    1,    1,    1,    1,    2,    2,
    1,    2,    3,    1,    1,    3,    3,    1,    2,    3,
    1,    2,    0,    2,    1,    1,    0,    1,    1,    1,
    3,    2,    3,    0,    1,    3,    4,    3,    1,    3,
    4,    3,    0,    1,    3,    3,    1,    1,    0,    1,
    1,    4,    0,    4,    0,    2,    0,    2,    0,    9,
   10,    0,    0,    0,    0,    3,    2,    1,    0,    3,
    5,    3,    2,    5,    2,    1,    1,    1,    1,    1,
    3,    1,    2,    3,    2,    1,    3,    1,    1,    6,
    3,    3,    2,    3,    0,    1,    3,    1,    1,    1,
    2,    1,    1,    1,    2,    1,    2,    1,    2,    0,
    3,    0,    1,    3,    1,    1,    1,    1,    3,    1,
    1,    1,    2,    0,    2,    0,    2,    0,    4,    1,
    2,    1,    1,    0,    1,    3,    2,    1,    1,    2,
    1,    4,    2,    1,    3,    8,    5,    1,    3,    1,
    1,    4,    4,    4,    1,    1,    5,    5,    1,    3,
    3,    1,    1,    7,    7,    5,    3,    0,    1,    1,
    1,    2,    0,    1,    1,    1,    0,    3,    1,    3,
    1,    0,    3,    1,    3,    1,    3,    3,    1,    1,
    3,    1,    2,    1,    3,    3,    1,    2,    3,    3,
    3,    3,    1,    3,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    3,    3,    3,    3,    3,
    3,    3,    3,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,    6,    1,
    1,    5,    6,    3,    4,    5,    6,    3,    4,    3,
    4,    3,    4,    4,    5,    4,    4,    3,    4,    1,
    1,    1,    1,    1,    1,    1,    3,    3,   10,   10,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    2,
    2,    3,    3,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    3,    1,    1,    1,    1,    1,    4,
    1,    3,    1,    2,    1,    1,    1,    1,    1,    1,
    1,    2,    2,    2,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    2,    1,    2,    1,    1,    1,
    1,    1,    2,    1,    1,    1,    1,    1,    4,    5,
    5,    5,    5,    5,    5,    5,    5,    5,    5,    6,
    6,    6,    8,    4,    4,    3,    6,    1,    1,    6,
    6,    1,    1,    4,    6,    4,    5,    3,    5,    4,
    5,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    0,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,
};
static short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,  548,
  549,  550,  554,  555,  556,    0,  609,  720,  721,  725,
  726,  459,    0,    0,    0,    0,    0,    0,  397,  564,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  898,    0,   21,  896,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  473,    0,    0,   37,   34,
   36,    0,   35,    0,  979,  981,  983,  982,  988,  984,
  991,  989,  985,  987,  990,  986,  992,  993,  994,    0,
    0,    0,    0,   32,   68,   66,  971,  980,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  638,    0,    0,    0,  670,    0,  973,    0,    0,
    0,    0,  107,  978,  557,    0,    2,    0,    0,    0,
    0,  427,  428,  429,  430,  431,  432,  433,  434,  435,
  436,  437,  438,  439,  440,  441,  442,  443,  444,  445,
  446,  447,  448,    0,  425,  426,    0,  964,    0,  404,
  968,    0,  401,  970,    0,  672,    0,  400,  974,    0,
  562,  894,    0,  150,  103,    0,  104,    0,  217,  960,
  759,    0,  102,  145,  967,    0,    0,   98,    0,  105,
  900,    0,   99,    0,  100,    0,  101,  977,  146,  106,
  617,  618,    0,  899,  897,    0,   74,    0,  965,    0,
  963,  481,  474,    0,  480,  485,  475,  476,  483,  477,
  478,  479,  484,    0,   30,    0,   39,  746,   38,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  904,    0,  885,  632,  632,  633,
  643,  645,  953,  646,    0,  647,  637,  633,    0,    0,
    0,    0,    0,    0,    0,  581,  570,    0,  573,  575,
  576,  577,  578,  584,  585,    0,  972,  671,    0,  108,
    0,  109,    0,  110,  558,    0,    0,  610,    0,  608,
    0,    0,    0,    0,  460,    0,    0,  367,    0,  220,
    0,  407,  398,  457,  458,    0,    0,    0,    0,    0,
    0,  416,    0,  373,  374,    0,  568,  566,  895,    0,
    0,    0,    0,    0,    0,    0,  153,  132,    0,   96,
    0,    0,  111,    0,  121,  120,    0,    0,    0,    0,
    0,    0,  956,   75,    0,    0,    0,  722,  723,  482,
    0,    0,    0,    0,  863,  891,    0,  955,    0,  865,
    0,    0,  892,    0,  886,  877,  876,  875,    0,    0,
    0,    0,  867,  868,  869,  878,  879,  887,  888,    0,
  889,  890,    0,    0,    0,  893,    0,    0,    0,    0,
    0,  881,    0,  756,  860,  861,  862,  668,  866,  350,
  846,    0,    0,    0,  845,    0,  666,  849,  907,  908,
  848,  842,  843,  844,  847,  880,  905,  906,  928,  929,
  932,  933,    0,  749,    0,    0,   27,    0,    0,    0,
   71,   69,  975,   67,   33,    0,    0,    0,  751,    0,
   52,    0,    0,    0,    0,    0,  560,    0,    0,    0,
  640,  642,    0,  649,    0,  733,    0,    0,  650,    0,
  636,    0,  607,    0,  582,    0,  579,    0,  580,  605,
    0,  600,    0,    0,  574,  760,  729,    0,    0,    0,
    0,    0,    0,    0,    0,  614,  611,    0,  185,    0,
    0,  129,  465,  172,  173,    0,  463,    0,    0,    0,
    0,  466,    0,  461,    0,    0,  411,  413,  412,    0,
  414,    0,  408,  218,    0,    0,  271,    0,    0,    0,
    0,  420,  421,    0,  450,    0,    0,    0,    0,    0,
    0,    0,  378,  377,  469,    0,    0,  149,    0,    0,
  158,    0,  157,    0,  164,  147,    0,  162,  165,  166,
  154,    0,    0,  504,  540,  507,    0,  541,    0,    0,
  508,    0,    0,    0,  531,  544,  503,  526,  505,  506,
  501,  488,  367,  486,  487,  489,    0,    0,  498,  499,
  500,  502,    0,    0,    0,    0,    0,  112,    0,  113,
    0,  124,    0,    0,  201,    0,    0,  365,    0,  220,
    0,    0,   83,    0,    0,  741,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  762,  764,  773,
  776,  777,  778,  779,  780,  781,  782,  783,  784,  785,
    0,    0,    0,  882,    0,    0,    0,    0,    0,    0,
  883,  884,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  325,  976,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  748,   28,
    0,    0,   44,   72,    0,    0,   26,    0,  754,    0,
    0,    0,  717,    0,    0,    0,    0,    0,  559,    0,
  632,    0,    0,    0,    0,    0,  969,  653,  552,    0,
  902,  551,    0,  583,    0,  591,  587,  586,    0,    0,
    0,  602,  590,    0,    0,  727,  728,    0,    0,    0,
  365,    0,    0,    0,    0,    0,    0,  130,    0,    0,
    0,    0,    0,    0,  367,  410,    0,  367,  368,  415,
  496,    0,  273,    0,  267,  269,  365,  240,  961,    0,
    0,  367,    0,    0,  209,  365,    0,    0,    0,    0,
  245,  239,  242,  243,  244,  419,  455,  454,  418,  417,
    0,    0,    0,    0,    0,    0,  376,    0,  367,    0,
  155,  159,    0,  163,    0,    0,  527,  528,  545,    0,
  542,    0,    0,  534,  532,  533,    0,    0,    0,    0,
  497,    0,    0,    0,    0,  535,    0,    0,  365,    0,
  200,    0,  203,  205,  206,    0,    0,    0,    0,    0,
  220,   78,    0,    0,   77,  515,  957,  516,    0,    0,
    0,    0,  763,  775,    0,    0,  833,  834,  835,    0,
  767,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  945,  946,  947,  948,  949,  950,
  951,  952,    0,    0,    0,    0,    0,    0,  938,    0,
    0,    0,    0,  858,  327,  859,  926,    0,    0,  854,
  959,    0,    0,    0,    0,    0,  667,    0,  864,  758,
  757,  750,    0,    0,    0,    0,    0,    0,    0,    0,
   53,   61,   63,    0,    0,   70,    0,    0,    0,  753,
    0,    0,    0,    0,    0,    0,    0,    0,  641,  644,
    0,    0,    0,    0,  658,  659,    0,    0,  651,  654,
  553,  903,  592,  601,  596,  595,    0,  606,  761,  730,
  731,  365,    0,    0,  634,  616,  615,    0,    0,  624,
  181,  175,  182,    0,  178,    0,    0,  177,    0,  188,
  169,  168,  901,  171,  170,  467,    0,    0,    0,  409,
  272,    0,  211,  367,  213,    0,  368,  208,  214,    0,
    0,    0,    0,  422,  423,  367,  365,  368,    0,    0,
    0,  394,    0,  196,  197,  156,    0,    0,    0,    0,
    0,    0,  543,    0,    0,  367,  368,    0,  492,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  198,    0,    0,    0,    0,  354,    0,   82,
    0,    0,    0,    0,   87,   84,    0,    0,    0,  724,
  962,    0,    0,  768,    0,    0,  774,    0,    0,    0,
    0,    0,  811,  810,    0,    0,    0,  818,  836,  828,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  766,    0,    0,    0,  220,    0,  909,    0,    0,  924,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  934,    0,  944,    0,    0,    0,    0,    0,
    0,  925,  870,   65,   57,   58,   64,   59,   60,    0,
    0,   22,    0,    0,    0,   42,    0,    0,    0,  735,
  755,    0,  718,    0,  734,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  673,    0,    0,    0,    0,
    0,  598,  599,  603,    0,  199,    0,    0,  628,    0,
    0,    0,    0,    0,    0,  189,    0,    0,    0,  405,
  223,  224,  225,  226,  227,  228,  873,    0,    0,    0,
  276,    0,  221,    0,    0,    0,    0,  246,    0,  451,
  452,  424,    0,    0,    0,    0,    0,    0,  367,  471,
  368,  167,  958,    0,    0,  510,    0,  509,  546,    0,
  538,    0,    0,    0,  490,    0,  518,    0,    0,  520,
  523,  525,  365,    0,    0,    0,  119,    0,  204,  367,
    0,    0,  366,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  826,  827,  770,    0,  772,
    0,    0,    0,    0,    0,    0,    0,  829,    0,    0,
    0,    0,    0,    0,    0,  819,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  915,  914,    0,    0,  911,  910,    0,  919,  918,  917,
  916,  913,  912,    0,    0,  940,    0,    0,    0,  939,
    0,    0,    0,    0,    0,    0,   56,   62,   54,   55,
   23,  739,  740,    0,  737,  744,  745,  742,    0,    0,
  716,    0,    0,  661,    0,  662,    0,  679,  675,  677,
    0,    0,    0,    0,    0,  366,  620,  621,  635,    0,
  183,  179,    0,    0,  194,  190,    0,  368,  874,  279,
    0,    0,  368,  277,    0,    0,    0,  231,    0,    0,
  247,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  468,  512,    0,  513,    0,  134,    0,    0,    0,  138,
  140,  141,  142,  493,  495,  491,  522,    0,    0,  368,
    0,    0,    0,    0,  351,    0,  355,  367,   88,    0,
    0,   90,    0,    0,    0,    0,    0,    0,  838,  837,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  219,  927,
  920,  921,    0,  922,  936,  935,    0,  941,  930,  931,
    0,   40,    0,    0,  719,  664,    0,  536,    0,  686,
    0,    0,  683,  687,  688,  690,  691,  692,    0,    0,
  630,  368,  352,  622,    0,  174,  191,  368,  144,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  284,  289,  283,    0,    0,
    0,  293,  294,  295,  296,  297,  298,  300,  301,  303,
  304,  305,  308,  266,    0,    0,  207,  232,    0,    0,
    0,    0,  233,  236,    0,    0,  368,  392,  393,  389,
  388,  391,  390,    0,  511,  539,  143,  131,  139,    0,
  367,  263,  126,  368,    0,    0,  369,  356,    0,    0,
   91,   92,   79,    0,  797,  805,    0,  798,  806,  801,
  809,    0,    0,  799,  807,  800,  808,  794,  802,  795,
  803,  796,  804,    0,  937,    0,  736,    0,    0,    0,
    0,    0,  700,  702,    0,  697,    0,  264,  184,  406,
    0,  311,    0,    0,  314,  315,    0,  307,    0,    0,
    0,    0,  306,    0,  337,  338,  292,  299,    0,  302,
  285,    0,  290,    0,  341,  291,  210,  215,  234,    0,
    0,  237,    0,  249,  251,  396,  368,    0,  371,    0,
    0,    0,  361,  362,    0,    0,   76,   94,    0,  923,
    0,    0,    0,  689,  684,  701,    0,    0,  282,    0,
  309,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  332,  333,  349,    0,    0,    0,    0,  344,  342,
  238,    0,    0,    0,    0,    0,  265,  368,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  708,    0,
    0,    0,  281,  278,  310,    0,    0,  313,    0,    0,
    0,    0,  321,    0,  324,    0,  348,  347,  966,  346,
    0,    0,    0,    0,    0,  248,    0,  255,    0,  254,
  372,  364,    0,    0,  357,  118,    0,    0,    0,  710,
  699,    0,    0,  711,    0,  707,    0,    0,    0,    0,
    0,    0,    0,    0,  335,  334,  343,  345,  235,    0,
    0,  259,  258,  257,    0,    0,  840,  631,  706,    0,
  713,    0,    0,  312,  318,    0,    0,    0,    0,  336,
  262,  260,  261,    0,    0,    0,    0,    0,    0,  339,
  316,    0,    0,    0,    0,  712,  330,  317,  319,  839,
  715,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1542,   28,   29, 1543,
   31,   32,   33,   34,   35,   36, 1544,   38, 1545,  636,
  120,  456,  949, 1182,  121,  702,  950,  122,  460,  707,
  146,  123,  124,  257,  972,  136,  951,  952,  197,  226,
  125,  953,  126,  462,   82,  237,  374,  421,  631, 1083,
  422,  854,  602, 1508,  831,  632,  633,  634, 1085,  238,
 1372,   74,  187,   75,  233, 1826, 1295,  218,  223,  225,
  227,  213,  205,  207,  220,  230,  153,  310,  312,  314,
  363,  618,  620,  453,  258,  781,  536,  646, 1020,  367,
  623, 1071, 1287,  569,  731, 1046,  521,  522,  551,  359,
  603, 1057, 1438, 1567, 1230, 1439, 1440, 1441, 1442, 1443,
  647,  214,  206,  355,  576,  356,  357,  577,  578,  579,
  580, 1262, 1014,  524,  525, 1004, 1002,  526, 1008, 1003,
 1220, 1401, 1009, 1010, 1225,  369,  842,  843,  844,  845,
  783, 1027, 1416,  604,  785,  424,  847,  930,  545, 1344,
 1231,  425,  426,  427,  428,  429, 1417, 1418,  554, 1573,
 1724, 1248,  791,  792,  778,  793,  794,  795, 1725, 1726,
 1803,  337,  549, 1239, 1546,  193,  546,  547, 1240, 1241,
 1411, 1700, 1547, 1548, 1549, 1550, 1664, 1551, 1552, 1553,
 1554, 1555, 1556, 1557, 1558, 1559, 1560, 1561, 1562, 1563,
  430, 1788, 1647, 1711, 1792, 1709,  431,  686,  432,  433,
 1090, 1665, 1718, 1719, 1758, 1076, 1293, 1455, 1077, 1078,
  285,  487, 1596,  378, 1211, 1394, 1681, 1682, 1683, 1684,
 1209,  346,  565,  809, 1260,  566,  807,  221,   59,  341,
  198,  161,  324,  184,  331,  190,  769,  766,  332,  342,
  799,  343,  561, 1038,  185,  434,  325,  533,  567, 1261,
   96,  196,  242,  605,  606,  771, 1058,  772, 1059, 1060,
  609,  610,  611,  612, 1051,  858,  613,  614,  615,  824,
  616, 1510,  821,   40,   41,   42,   43,   44,   45,  155,
   61,  478,  201,  348,  297,  298,  299,  300,  301,  302,
  303,  304,  305,  737,  501,  987, 1214,  502,  503,  742,
   46,  160,  320,   47,  515,  516,  996, 1399, 1218, 1524,
  755, 1000,  140,  724, 1521,  713,  481,  287,  141,  142,
  435,  286,  488,  489,  490,  728,  973,  974,  975,  976,
 1203,  436, 1165, 1512, 1513,  438, 1515,  439,  440,  441,
 1739, 1637, 1740, 1741, 1742, 1786, 1827,  468, 1194,   48,
   49,  379,   50,   51,  509,  510,  958, 1190, 1374, 1378,
  469,  710,  211,  648,  649,  872,  873,  650,  651,  652,
  653,  654,  655,  656,  657,  658,  659,  660, 1316, 1106,
 1108, 1109,  929,  685,  442,  443,  444,  445,  446,  235,
  447,  448,  913,  449,  450,  451,  452,  676,  917, 1166,
 1161,  677,  128,
};
static short yysindex[] = {                                   7601,
 1386,  -90, 3447,  130, 1454,  502, 5115,  548, 3840,  569,
  633, 3079,  -90, 1341,  657, 5332, 1343, 5332,  505,    0,
 1037,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  357,    0,    0,    0,    0,
    0,    0, 8792, 5332, 5332, 5332, 5332, 5332,    0,    0,
  732, 1421, 5332, 5332, 5332,  906, 5332,  715, 5332, 5332,
    0, 5332, 5332,    0,  106,    0,  813,    0,    0,  917,
 5332,    0, 5332, 5332, 5332, 1016, 5332, 5332, 5332, 5332,
  715, 5332, 5332, 5332, 5332,    0, 5332,  947,    0,    0,
    0, 1330,    0, 1330,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1010,
 1085,  112, 1433,    0,    0,    0,    0,    0, 5332, 5332,
 5332,  989, 1135, 1159,  110, 1024, 1057,  183,  195,  991,
 1055,    0, 5332, 1191, 1481,    0, 1170,    0, 5332, 5332,
 5332, 5332,    0,    0,    0, 5332,    0, 1308,  504, 1280,
  529,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1051,    0,    0, 1421,    0,   49,    0,
    0, 1584,    0,    0, 1483,    0,   50,    0,    0, 1229,
    0,    0, 1198,    0,    0, 1576,    0, 1399,    0,    0,
    0, 1421,    0,    0,    0,  835, 1584,    0,   44,    0,
    0, 1351,    0, 1375,    0, 1330,    0,    0,    0,    0,
    0,    0, 1376,    0,    0, 1267,    0, 1401,    0, 1296,
    0,    0,    0, 1267,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 8536,    0, 5332,    0,    0,    0, 5019,
 1379, 5100, 5332, 1313, 1466, 1772, 1502, 1524, 5019, 1448,
 5100, 1472, 1511, 1395,    0,10227,    0,    0,    0,    0,
    0,    0,    0,    0, 7808,    0,    0,    0, 1477, 5332,
 1598, 1523,  521, 5332, 1533,    0,    0, 1481,    0,    0,
    0,    0,    0,    0,    0, 5332,    0,    0, 1584,    0,
 1351,    0, 1330,    0,    0,10227, 1555,    0, 1554,    0,
 1168, 1653, 1003,  529,    0, 1727, 1362,    0, 5332,    0,
 1051,    0,    0,    0,    0, 5332, 1604, 4049, 1557, 4113,
 1986,    0, 8792,    0,    0,  901,    0,    0,    0, 1973,
 1973, 1421, 1673, 1421, 1456, 1576,    0,    0, 7774,    0,
 5332, 5332,    0, 1604,    0,    0, 1766,  719, 2014, 5332,
 1792, 5332,    0,    0, 2022,  128, 5396,    0,    0,    0,
 2024, 2026, 2028, 1609,    0,    0, 2031,    0, 2032,    0,
 2033, 2036,    0, 2039,    0,    0,    0,    0,    0, 2042,
 1622, 1625,    0,    0,    0,    0,    0,    0,    0, 2048,
    0,    0, 8633, 2055, 2056,    0,10227,10227, 7906, 5332,
 2057,    0, 2496,    0,    0,    0,    0,    0,    0,    0,
    0, 2058, 2010,    0,    0, 2059,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1823,    0, 5332, 1671,    0, 5332, 5332,  138,
    0,    0,    0,    0,    0, 5332, 5332,  -57,    0, 5332,
    0, 1788, 5332, 2060, 5019, 1709,    0, 5332, 8872, 2104,
    0,    0,10227,    0, 2496,    0, 1803, 2081,    0, 2347,
    0,   91,    0,  491,    0,  -41,    0,  -41,    0,    0,
 2094,    0,  411, 1720,    0,    0,    0, 2106,   87, 2089,
 1604, 2113, 1891, 1591, 2112,    0,    0, 1776,    0,  147,
 1811,    0,    0,    0,    0, 1421,    0, 1973, 1973, 1973,
 1973,    0, 2116,    0, 1883, 1884,    0,    0,    0, 1863,
    0, 7774,    0,    0, 7774, 1831,    0, 1584, 1901, 5332,
12064,    0,    0,  737,    0, 5332, 1113, 1483, 1771, 1736,
 1705,    0,    0,    0,    0,  655, 1779,    0, 1721,  715,
    0, 1421,    0, 1900,    0,    0, 1456,    0,    0,    0,
    0,  324, 1733,    0,    0,    0, 1734,    0, 1787, 2143,
    0, 1840, 2151,  690,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2101,  248,    0,    0,
    0,    0,  155, 2154, 2155, 2151, 1584,    0, 1878,    0,
 1933,    0, 1421, 1421,    0, 4049,   50,    0, 1815,    0,
 1790, 2158,    0, 9046,  907,    0, 1810, 1824, 2165, 5806,
 2172,    0,    0,    0, 5299, 4832,   15,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1098, 9111,  710,    0, 5332,10227, 1145, 1164, 1266, 2850,
    0,    0,10227, 5958, 2496, 1763,   40,10227, 9111,  781,
  781, 1341, 2174, 1859, 2191,    0,    0, 9191, 5332,10227,
10227,10227,10227,10227, 9111,10227,  708, 5332,    0,    0,
 4598, 1802,    0,    0, 1770, 5100,    0, 1922,    0, 1865,
 1341, 2207,    0, 1922, 4598, 1932, 1938, 5332,    0, 2174,
    0, 2285,  486, 1813, 9111, 5332,    0,    0,    0,  970,
    0,    0,  -33,    0, 1820,    0,    0,    0, 5332,  473,
 5332,    0,    0, 5332, 5332,    0,    0, 9111, 1999, 4049,
    0, 1774,10227, 5332, 1819, 1421, 1421,    0,  636, 1721,
 1721, 1816,  715, 1003,    0,    0, 2243,    0,    0,    0,
    0, 2007,    0, 5332,    0,    0,    0,    0,    0, 2017,
 2255,    0, 1997, 2016,    0,    0, 1958, 1959, 1330, 2035,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  715, 5332,12110, 1918, 1924, 1925,    0,  715,    0, 1534,
    0,    0, 1896,    0,   88, 1903,    0,    0,    0,  715,
    0, 2143,  136,    0,    0,    0, 2009,  136, 1898,  136,
    0, 2284,  715,  715,  715,    0, 1604, 5332,    0, 1961,
    0, 1873,    0,    0,    0,  901, 1916, 5332, 1874,  162,
    0,    0, 2294, 2061,    0,    0,    0,    0,  128, 5332,
 1931, 6197,    0,    0, 1931, 5806,    0,    0,    0, 4347,
    0,  164,   81,10227,10227, 6294, 2300,  664, 6548,10227,
 6700, 1378, 6809, 7063, 9430, 7215, 7324, 7578, 5958, 5958,
10227,10227, 2077,10227, 2309,10227, 2307, 2302,10227,10227,
10227,10227,10227,10227,    0,    0,    0,    0,    0,    0,
    0,    0, 2040,  810,    8,10227,  634, 9111,    0, 5958,
 1494, 2308,  991,    0,    0,    0,    0, 2496, 1879,    0,
    0, 2075,  781,  781,  120,  120,    0,  265,    0,    0,
    0,    0, 5332, 5332, 5332, 5332, 5332, 5332,  157, 2320,
    0,    0,    0, 4598, 1980,    0,  -48, 2001, 5332,    0,
  991, 9111, 2001, 2322, 2320, 4598, 4325, 2049,    0,    0,
  486, 2330, 2327, 1432,    0,    0, 5958, 2044,    0,    0,
    0,    0,    0,    0,    0,    0,  461,    0,    0,    0,
    0,    0, 1892, 1916,    0,    0,    0, 2332, 1869,    0,
    0,    0,    0, 1893,    0, 1973, 2114,    0,  636,    0,
    0,    0,    0,    0,    0,    0, 2338, 5958, 1495,    0,
    0, 2082,    0,    0,    0, 1495,    0,    0,    0, 1330,
 1330, 1881, 2353,    0,    0,    0,    0,    0,  -25,  -44,
 1002,    0, 2119,    0,    0,    0, 5332,  130, 1944, 1975,
 2007, 2354,    0, 1721, 1956,    0,    0, 1025,    0, 2341,
 5332, 1110,  715, 2359, 2365, 2367, 2145,   50, 2082, 1973,
 1811, 4049,    0, 1779,  991, 1971, 1989,    0,  389,    0,
  715, 2038, 2117, 9046,    0,    0,  715, 1941, 2130,    0,
    0, 2399, 2401,    0, 5958, 5958,    0, 5958, 5958, 1923,
 2496, 2403,    0,    0, 2496, 2407, 9669,    0,    0,    0,
 2099, 2412, 2496, 2416, 1599, 2423, 2496, 2430,10227,10227,
 2300,10227, 9749, 2431, 2496, 2433, 2434, 2496, 2435,10227,
 2496, 2436, 2496, 2437, 2438, 2496, 2440, 2441, 2496, 2442,
    0, 2221, 2316, 2506,    0, 2533,    0, 2609,10227,    0,
 2991, 3031, 3055, 3070, 3116, 3168,10227,  715, 9111, 2496,
 2051, 9111,    0,10227,    0, 2176,  516,10227, 9111,10227,
10227,    0,    0,    0,    0,    0,    0,    0,    0, 2160,
 4598,    0, 4598,  157, 2320,    0,  241,  241,   99,    0,
    0, 7808,    0, 1963,    0, 2322, 2320, 2446, 4325, 1432,
 2450, 9111, 5332,  486, 2107,    0, 2109, 2111, 2149, 2226,
 2170,    0,    0,    0, 2082,    0, 1971,  607,    0, 2080,
 1421, 2120, 1721, 2115, 1721,    0, 5958,  372, 1411,    0,
    0,    0,    0,    0,    0,    0,    0, 2070, 2232, 2082,
    0,10227,    0, 2217, 2100, 1881,  106,    0, 5958,    0,
    0,    0, 2127, 2133, 2134, 2135, 2136, 2138,    0,    0,
    0,    0,    0, 2484,  130,    0, 1816,    0,    0,  136,
    0, 1495, 2148,  136,    0,  136,    0, 2007, 2486,    0,
    0,    0,    0,  901, 2232, 1721,    0, 1811,    0,    0,
 7808, 2251,    0,  604,    0, 2332,    0, 2490,    0, 2491,
 1421,  -30, 2492,    0, 1421,    0,    0,    0, 2221,    0,
 2221,10227, 2118, 2118, 1341, 2493, 1964,    0, 2118, 2118,
10227, 2118, 2118, 2182, 2496,    0, 1639,10227, 2496, 2118,
 2118, 2118, 2118, 2496, 2118, 2118, 2118, 2118, 2118, 2118,
    0,    0, 7774, 2495,    0,    0, 3244,    0,    0,    0,
    0,    0,    0, 3366,  413,    0, 9111, 2237, 2122,    0,
 9111, 3560, 1965, 1467, 2496, 2167,    0,    0,    0,    0,
    0,    0,    0, 2124,    0,    0,    0,    0, 1803, 9111,
    0, 4325, 2446,    0, 2002,    0, 1432,    0,    0,    0,
  486, 5533, 5958, 2171, 2232,    0,    0,    0,    0, 1973,
    0,    0, 1421, 1534,    0,    0,  421,    0,    0,    0,
 5332,11890,    0,    0, 2496, 2274, 2217,    0, 1301, 5332,
    0, 2224,  422, 2121, 2233, 2137, 2265, 2234, 2283, 2082,
    0,    0, 2545,    0, 2548,    0, 2238, 2274, 2148,    0,
    0,    0,    0,    0,    0,    0,    0, 2082, 1779,    0,
 1534, 2119, 2271, 2208,    0, 1916,    0,    0,    0, 2223,
 -100,    0, 2007, 2227, 2496, 2551, 2558,  991,    0,    0,
 2559, 2561, 2496, 2563, 2565,10227,10227, 2496, 2571, 2575,
 2584, 2585, 2586, 2588, 2589, 2591, 2592, 2594,    0,    0,
    0,    0,  715,    0,    0,    0, 9111,    0,    0,    0,
 1813,    0,  241, 1813,    0,    0, 1528,    0, 2361,    0,
 2597, 2600,    0,    0,    0,    0,    0,    0,   15,  683,
    0,    0,    0,    0, 1721,    0,    0,    0,    0, 7774,
 1100,  785, 2587,  -38, 2605, 2313,10227, 2598, 2618, 2602,
 2603, 2606, 2610, 2319, 2611,    0,    0,    0, 2363,11866,
 2617,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2637, 5332,    0,    0, 2333, 2340,
 5332, 1881,    0,    0, 1330, 5332,    0,    0,    0,    0,
    0,    0,    0, 2232,    0,    0,    0,    0,    0, 2232,
    0,    0,    0,    0,  787, 1813,    0,    0,10227, 1421,
    0,    0,    0, 1421,    0,    0,10227,    0,    0,    0,
    0, 2496, 2496,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2646,    0, 2044,    0, 2044, 5958, 5332,
  844, 5533,    0,    0, 2323,    0, 2650,    0,    0,    0,
  151,    0, 9988, 5332,    0,    0,10227,    0,  278, 2348,
 5958, 2343,    0, 5958,    0,    0,    0,    0,  973,    0,
    0,  716,    0, 2247,    0,    0,    0,    0,    0, 1881,
 1330,    0, 2311,    0,    0,    0,    0, 2119,    0,  787,
 2656, 1432,    0,    0, 2044, 2496,    0,    0, 1467,    0,
 2170, 2170,   15,    0,    0,    0, 4871, 1495,    0, 2643,
    0, 2857, 8536, 1393,10227,  973,  654,10227, 2645,  692,
 1320,    0,    0,    0, 5332, 5332,  130,  232,    0,    0,
    0, 2311,  530, 1881, 2328, 2329,    0,    0, 1432, 2664,
  787, 2370, 2170, 2673, 1813, 2171, 2171, 5332,    0, 2679,
 2677,  -12,    0,    0,    0, 2335,  973,    0, 1414,  748,
 2295, 2496,    0, 2428,    0,  973,    0,    0,    0,    0,
11890,  716, 1881,  924,  924,    0, 2432,    0, 2296,    0,
    0,    0, 1432,  787,    0,    0, 2044, 2352, 2352,    0,
    0, 4871, 2706,    0, 5332,    0,  733, 2688, 1327,  973,
 2455, 2445,11890,11890,    0,    0,    0,    0,    0, 2287,
   30,    0,    0,    0, 1668, 2170,    0,    0,    0, 5332,
    0,  973, 2708,    0,    0,  436, 5332,11890, 2449,    0,
    0,    0,    0, 5958, 2171, 2713, 2718, 2019,11890,    0,
    0,11890,   15, 2352, 5332,    0,    0,    0,    0,    0,
    0,
};
static short yyrindex[] = {                                      0,
    0,  171,  762,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  127, 1632,    0,    0,    0,    0,    0,    0,
 2760,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  910,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  872,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2426,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2383,    0,    0,
    0,  113,    0,  113,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2384,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1033,    0,    0, 8145,
 5654,    0,    0,    0, 1120,    0,    0,    0, 2110,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  385,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  611,    0,    0,    0,    0,  766,    0,    0, 1202,
    0,    0,    0,    0,    0, 1517,    0,10089,    0,    0,
    0,    0,    0,    0,    0,    0,  611,    0,  -47,    0,
    0, 2728,    0,    0,    0, 2505,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1215,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1246,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1089,    0,    0,    0,    0,
    0,    0,    0, 5557,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  964,    0,    0, 1265,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  611,    0,
 2728,    0, 2505,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1300,    0, 2497,    0,    0,    0,    0,
 1322,    0,    0,    0,    0,    0, 2509,  -22, 5894,    0,
 1331,    0,    0,    0,    0,  209,    0,    0,    0, 2312,
 2312,    0,    0,    0, 1335, 2098,    0,    0,    0,    0,
    0,    0,    0, 2509,    0,    0, 1478,    0,    0,    0,
    0,    0,    0,    0, 8080, 2388,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1902,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2681,
    0,    0,    0, 2934, 3187,    0,    0,    0,    0,    0,
    0,    0,  393,    0,    0,    0,    0,    0,    0,    0,
    0,  210, 3440, 2375,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1361,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1385,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  623,    0,    0, 2458,    0,  145,
    0,    0,    0,    0,    0,  503,    0,  503,    0,    0,
 1081,    0, 1103, 1252,    0,    0,    0,    0, 1215,    0,
 2509,    0,    0,  105,  824,    0,    0,  119,    0, 3770,
  225,    0,    0,    0,    0,    0,    0, 2312, 2312, 2312,
 2312,    0,  380,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2515,    0,    0,
    0,    0,    0, 2497,    0,    0, 1147,    0, 2317,    0,
    0,  832,    0,    0,    0,    0,  208,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1437,    0,    0,    0,
    0, 2866, 1800,    0,    0,    0, 2231,    0,    0, 1339,
    0,    0, 5922,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2514, 2544,    0,    0,
    0,    0, 8435, 5268,    0, 5922,  611,    0,    0,    0,
    0,    0,    0,    0,    0,  -22, 1005,    0,    0,    0,
    0, 2371,    0,    0,    0,    0,    0,    0,    0,    0,
    0,10564,10786,10870,    0,    0, 1520,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
10307,    0,10307,    0,    0,    0,10307,10307,10307,    0,
    0,    0,    0,    0, 2339,    0,    0,    0,    0, 4410,
 4642, 8297, 2739,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  163,    0,    0,
 1632,    0,    0,  163,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1256,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  671,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -22,
    0, 1107,    0,    0,  940,    0,    0,    0, 3787,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  537,    0,    0,    0,    0,    0,    0,    0, 2749,
    0,    0, 3343, 3090,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  274,  290,  412,    0,    0,    0, 1368,
    0,    0,    0,    0,    0, 3599,    0,    0,    0,    0,
    0, 1339,    0,    0,    0,    0,  399,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2509,    0,    0,  322,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 9604,
    0,    0,    0,  239,    0,    0,    0,    0, 2487,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,10307,    0,    0,    0,    0, 1451,    0,    0,
    0, 3672, 4895, 5127, 3925, 4157,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1499, 1513,
    0,    0,    0,    0,    0,    0,    0, 1566,    0,    0,
 8145,    0, 1566, 1571, 1581,    0,    0,    0,    0,    0,
    0,  818, 2228, 2277,    0,    0,    0, 1853,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  168,    0,    0,
    0,    0,    0,    0,    0, 2312, 2337,    0, 3863,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2522,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1460,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1595,    0,    0,    0,    0,    0,    0,    0,
 2648,    0,    0,    0,    0,    0,    0,    0,    0, 1131,
    0,    0,    0,    0,    0,    0,    0, 1005, 2522, 2312,
 1656,  -22,    0, 2535, 8145, 1692,  167,    0,    0,    0,
    0, 9604,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 6173,    0,    0,    0, 6499,    0,    0,    0,    0,    0,
    0,    0, 7594,    0,10638,    0,10954,    0,    0,    0,
    0,    0,    0,    0,11026,    0,    0,11100,    0,    0,
11134,    0,11163,    0,    0,11184,    0,    0,11271,    0,
    0, 1111,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2362,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1499, 1710,    0, 2364,  236,    0,    0,
    0,    0,    0,    0,    0, 1719, 1743, 1762,    0, 2459,
 1272,    0, 2577,    0, 2460,    0, 2463, 2466,    0,    0,
  311,    0,    0,    0, 2522,    0, 1692,    0,    0, 3256,
    0,    0,    0, 2358,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 6025,    0, 2540,
    0,    0,    0, 1488,    0, 1460, 2426,    0,    0,    0,
    0,    0,  649,  661,  753,  779,  789,  815,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  836,    0,    0,    0,    0, 3517,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1784,    0,    0,
    0,    0,    0,    0,    0, 1793,    0,    0,    0,    0,
    0,  166,    0,    0,    0,    0,    0,    0,  118,    0,
  269,    0,    0,    0, 8297,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11304,    0,11441,    0,11470,    0,
    0,    0,    0,11504,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1837,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1798,    0,    0,    0, 2279,    0,    0,    0,
    0,    0,    0,  376,    0,    0,    0,    0,    0, 2312,
    0,    0,    0, 2067,    0,    0,    0,    0,    0,    0,
    0, 2511,    0,    0, 2776, 1795, 2959,    0, 2497,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2522,
    0,    0,    0,    0,    0,    0,    0, 1814, 1034,    0,
    0,    0,    0,    0,    0,    0,    0, 2522, 2535,    0,
  337,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  255,    0,11568,    0,    0,10307,    0,    0,
    0,    0,11642,    0,    0,    0,    0,11671,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  331,    0,  236, 1047,    0,    0, 2459,    0,  174,    0,
    0,  179,    0,    0,    0,    0,    0,    0,  742, 2780,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1522,
 1545,    0,    0,    0,    0,    0,    0,    0,    0, 2513,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 3626,    0,    0,  111,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  204,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,11705,11716,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  471,    0,  873,    0,    0,
    0,    0,    0,    0, 2785,    0,    0,    0,    0,    0,
 2771,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2773,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 2525,    0,    0,    0,    0,    0, 3626,
  117,    0,  107,    0,    0,    0,    0,    0,    0,    0,
  428,  282,    0,    0,  214, 2776,    0,    0,    0,    0,
  316,  885, 2764,    0,    0,    0, 2780,    0,    0,    0,
    0,    0,  259,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  144,    0, 1460,  190,  583,    0,    0, 2459, 1726,
    0,    0,  206,    0,  331,  131,  315,  976,    0,    0,
 2793,    0,    0,    0,    0, 2779,    0,    0,    0, 2537,
    0, 2784,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 3626,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  637,    0,    0,    0,  471, 2810, 1226,    0,
    0, 2780,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2459,  316,    0,    0,    0,    0,
    0,    0, 2795,    0,    0,    0,    0,    0,11792,    0,
    0,    0,    0,    0,  131, 2819,    0,    0,    0,    0,
    0,    0, 2129, 2810,    0,    0,    0,    0,    0,    0,
    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2861,    0,    0, 2863,
    0,    0,    0,    0,    0,    0,   11,    0, 2868,    1,
  305,  -50, -599, 1685,  714, -233,   75, 2858, 2601,    0,
  -28,    0, 2607,  -96,  -40,    0,  555, -894,  -27,  -63,
  -36, -200, 2616, 2175,    0,    0,    0,  570,    0,    0,
  -52, 1789, -519,   -1, -868, 2252, 2037, -298, 1808, 2804,
 -258,    0,  -60,    0, -472,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -570,  -29, 1818, -275, 1137, -929,    0,
    0,    0,    0, -332, -121, -544, 1830, -329,  -51,    0,
 -517,    0,    0, 1465, -974,    0, 1470,    0,    0, 2593,
 -597,  -18,    0,    0,    0,    0, 2555,    0, 2336, 2608,
 -197, 1854, -662,    0,    0,    0, -666,    0,    0,    0,
    0,    0,    0, 1914,    0, 2613, 2180, 1861, 2599, 2604,
    0, 1290,    0, -512, 2156,  -35, -648, -511,    0,    0,
    0, -927, -926, -922, -272, -918,    0, 1521, -904,    0,
 1217,-1181, 1530,    0, 2385,    0,    0,    0, 1221, 1225,
 1187,  306, -250, -755, -336,    0,    0, 2183,    0, 1716,
    0,    0, -259,    0,    0,    0,    0, -161,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1255,    0, 1314,  -78,    0,    0, 1106,    0, -381,  245,
 1148, 1298,    0, 1205,    0, 1974, 1753, 1575,    0,-1070,
 -811, -914,    0, -495, -757, -242,    0, -575, 1292,  965,
 -584, -494, -707, -909,-1258,    0,    0,   -3,    0,    0,
    0,    0,    0, 2631,    0,    0,    0,    0, 2651, 2429,
    0,    0,    0,    0,    0,   -9, 2660,    0,    0,    0,
    0,  -10,    0,    0,    0, -338, 2161, -335, 1712, -457,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2381,
    0,    0, 2184,    0,    0,    0,    0,    0,    0,   55,
 2985,    0,    0,    0,    0,    0, 2709,    0,    0,    0,
    0, 2510,    0, 2512,    0,    0,    0, 2269,    0,    0,
    0,  -74,    0,   86,    0, 2258,    0,    0,    0,    0,
    0,    0, -587,-1136, -896,  394, -210, 1115,    0, 2872,
 -116, 1381,    0, 2290, -611, 2293,    0, -802, 2052,    0,
    0, -129, -239,    0, 1388,-1150,    0,-1104, -622, -618,
 1508,    0, 1248,    0, 1293,    0, 1200,    0,    0,    0,
    0, 2528,    0,    0,    0, -399, 2325, 2079, -988,    0,
    0,    0, -657, -388,  326,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 3191, 1367,
 1926,    0, -503,    0,    0,    0, -913,    0, 1817,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -240,
 1880,    0,    0,
};
#define YYTABLESIZE 12622
static short yytable[] = {                                     127,
   79,  189,  127,  212,   78,  149,  148,  259,  154,  204,
   37,  208,  486,  746,  437,  192,  278,  281,  570,  308,
  607,  279,  282,  608,  243,  575,  217,  209,  195,  770,
  252,  542,  773,  782,  732,  247,  229,  684,  784,  941,
  222,  752,  224,  186,  188,  191,  194,  148,  199,  248,
  893, 1243,  540,  210,  188,  215,  254,  194,  253,  148,
  199,  461,  228,  127, 1421,  251,  219,  709,  482,  246,
  461,  239,  149,  148,  241,  188,  915,  239,  215,  191,
  194,  250,  148,  199,  228,  127,  989,  194,  313,  249,
 1005, 1232, 1233,  265,  923,  267, 1234, 1244, 1232, 1233,
 1236,  266,  994, 1234,  619, 1237,  256, 1236, 1252,  309,
  747, 1170, 1237,  621,  855,  964,  747,  637,  627,  210,
  148,  210,  311,  961,  289,  532,  565, 1273, 1022,  371,
  745, 1048,  846,  215,  333,  730,  991, 1029, 1074,  307,
  194,  148,  228,  256,  507,  619,  154,  256,  619, 1192,
  256,  747, 1049,  263,  747,  263,  747,  747,  769,  360,
  747,  277,  738,  619, 1290,  256,  353,  626, 1200,  747,
  565,  698,   77,  685,   77,  747,  508,  627,  681,  437,
 1054,  706, 1268,  998,  256,  565,  757,  256,  652,  252,
 1069,  758,  938, 1594,  833,  760,  761,  762,  763, 1375,
 1181, 1082,  256,  696, 1097,  694,   89,  395,  470,  326,
  315, 1698,  607,  682,  685,  608,  513,  685,  472,  681,
  454,  738,  276, 1457,  462,  353,  626, 1501,  978,  565,
  252,  457,  685,  252,  280,  517,  493,  681,  122,  716,
  457, 1514, 1504, 1461,  318,  277,  210,  575,  252,  496,
  148,  863,  463,  127, 1601,  241, 1255,  277,  704,  148,
  749,  463,  696, 1291,  694,  500,  395,  470,  326,  889,
  506, 1187,  682,  381, 1649, 1762,  889, 1379,  790,   85,
  191,  359,   85,  462,  148,   77, 1368, 1516, 1370,  379,
 1036, 1141, 1142,  541,  517,  519,  241, 1436,  519,  571,
  544,  573,  209,  277,  557,  334,  344,  559, 1171,  771,
  694,  365, 1253, 1285,  698,  625,  133,  328,  629,  210,
  617,  125, 1167,  816, 1783,  359,  210,  827,  210, 1822,
  210, 1431,  381,  562,  619,  735,  195,  508,  830, 1419,
  359,  627, 1098, 1215, 1232, 1233,  918,  919,  379, 1234,
 1193,  194,  199, 1236, 1184,  698,  694, 1173, 1237,  711,
  148, 1784,  191,  815,   60, 1055, 1196, 1198,   77,  694,
  231,  696,   79,  698, 1785,  698, 1453,  241,   14,  464,
  125, 1823,  335,  345,  625, 1598,  712, 1188, 1251,  486,
 1672, 1602,  669,  890,  747,  195,  747, 1256,  135,  689,
  890, 1387,  517,  747,  690,  498,  736,  232,  366, 1446,
  687,  380, 1408,  122,  122,  756, 1254,  703,  241, 1728,
 1228,  256,  619,  256,  717, 1095,  700,  358,  256,  271,
  256,  506,  703,  669,  698, 1462,  669,  708,  464,  922,
  272,  714, 1159,  625,  619,  148,  457,  810,  194,  463,
 1026,  669,  486, 1494,  741,  937,  148,  135,  627, 1395,
  148, 1528, 1577,  194,  652,  148, 1099,  252,  154,  252,
  380,  395,  470,  759,   89,  486,  252, 1094, 1529, 1756,
  727, 1514,  462, 1564,  968, 1565,  358, 1368, 1721, 1017,
  920,  685, 1019,  747,  747,  747,  681, 1308, 1309,  747,
 1310, 1311,  593,  769,  685,  461, 1025,  626, 1296,  681,
  969,  682, 1419,  685, 1627,  786,  747,  698,  681,  812,
 1592,  256,  364,  747,  829,  971, 1376, 1516,  519,  738,
  462,  696,  719, 1043,  696, 1761,  519,  381,  377, 1591,
  779,  210, 1766,  256,  682, 1013,  779,  262,  275, 1096,
  256, 1084, 1735,  379, 1402, 1081, 1013,  747,  635, 1191,
  685,  593,  565,  519,  855,  681, 1016,  252,  811,  685,
  840,  841,  627, 1284,  681,  729, 1449,  519,   79,  565,
  519, 1799,  253,  990,  256,   76, 1067,   76, 1507,  252,
  209,  705, 1638,  275, 1603,  519,  252,  519, 1640, 1383,
 1089,  832,  517, 1317, 1434,  470,  619,  256,  982,  359,
 1180,  747,  359,  685,  511,  508,  210,  747,  681,  326,
  353,  626,  732,  253,  680,  857,  253,  685,  694, 1407,
  252,   79,  681,  889, 1448,  856,  360,  464, 1011, 1012,
  275,  253,  462,  696,  256,  694,  897, 1675,  386,   85,
  694, 1423,  275,  682,  771,  215, 1607,  696,  696,  694,
  387,  696,  942,  732, 1679,  519,  732,  682,  328,  125,
  381,  635,  780, 1223, 1584,  380,  135,  703,  135,  931,
  360,  732,  889,  889,  195,  464,  379,  210,  210,  486,
  252,  463, 1590,  698, 1751,  360,  463,  694,   76,  669,
  669,  698,  694, 1001, 1001,  463,  694,  386,  194,  506,
  500,  694,  988,  194,  209,  698,  727,  696,  454,  387,
  519,  359, 1645, 1743,  625,  134,  696, 1468,  740,  148,
 1493,  148, 1754,  359,  210,  359, 1526, 1286,  544, 1829,
  210,  693, 1174,  158,  210, 1177, 1235, 1727, 1242,  940,
  694,  895,  383, 1235,  635,  358, 1734,  890,  358, 1032,
 1250, 1015,  698,  317,  210,  375, 1035, 1037, 1505, 1517,
 1232, 1233, 1812, 1518, 1422, 1234,  135,  889,  382, 1236,
 1272,   76,  693, 1084, 1237, 1646,  321, 1363,  385,  965,
  420, 1756,  210,  210,  519, 1519,  159,  464, 1771, 1034,
  693,  682,  669,  496, 1042,  375,  890,  890,  380, 1068,
  519,  383, 1435,  380,  384,  698, 1052,  665, 1445, 1079,
 1385,   83,  693,  612,  375, 1489, 1680,  694,  148,  508,
 1764, 1064, 1065, 1066,  322,  136,  519,  382,  148, 1778,
 1779,  129,  519,  669,  593,  519, 1343,  385,  105,  273,
 1091,  693,  691,  776,  692,   79,  694,  682,  665, 1527,
  253,  665,  253, 1456,  612, 1212,  682,  358, 1691,  253,
 1692,  567,  682,  384,  268,  985,  665,  106,  107,  358,
  108,  358,  612, 1178,  694,  940,  732,  109,  110,  111,
  112,  113,  986,  114,  136,  733,  115,  116,  117,  118,
  119,  890, 1413, 1175,  497,  131, 1593,  593,  130,  613,
  593, 1179,  386,  682,  295,  889, 1213, 1176, 1356,  486,
   14, 1358,  837,  506,  387,  694,  593, 1733, 1834,  437,
  567,  682,  982,  463,  194,  199,  463,  228,  127,  629,
 1162, 1163,  732,  694,  463,  105,  323,  519, 1450,   97,
 1361,   77,  593,  889,  519,  804,  463,  463,  758,  498,
  253,  194,  437,  588,  360,  864,  825,  360,  613,  283,
  871, 1765,  519,  941,  106,  107,  499,  108, 1714,  680,
  147,  143,  253, 1430,  109,  110,  111,  112,  113,  253,
  114, 1006,  805,  115,  116,  117,  118,  119,  629, 1235,
 1245, 1246,  519,  939,  607, 1626,  519,  608, 1628, 1517,
  156, 1791,  894, 1518, 1452, 1110,  383, 1641,  486, 1806,
 1075,  268,  588,  253,  375, 1633,  115, 1715, 1185,  375,
  420, 1693,  563,  137,  826,  375,  209, 1263, 1716,  890,
 1197, 1111,  382,  454,   79,  386,  696,  240, 1264, 1297,
 1634, 1263,  385, 1707, 1668,  787, 1710,  387, 1522,  693,
  689, 1279,  210,  115, 1007,  690,  624, 1525, 1274,  210,
 1373, 1373, 1377, 1386,  732,  597,  360,  890,  384, 1299,
  594,  693,  732,  253, 1164, 1303,  144,  696,  360,  689,
  360,  563,  137,  145,  690,  157,  806, 1732,  115, 1397,
 1685, 1404,  604, 1406, 1729,  696,  623,  486, 1635, 1398,
  765,  732,  732,  367,  732,  136, 1495, 1275, 1694,  572,
 1498,  732,  732,  732,  732,  732,  597,  732,  693, 1158,
  732,  732,  732,  732,  732,  665,  788,  693,  665,  594,
  200,  612, 1717,  790, 1732,  665,  456,  623,  665,  383,
  623,  765,  665, 1274,  765, 1773, 1355,  665, 1642,  563,
  665,  604,  375,  612, 1451,  623,  216,  105, 1001,  765,
  564,  463,   76,  463,  494,  382,  665,  789,  572,  202,
  203,  693, 1599,   79,   79,  385, 1644,  864, 1732,  463,
  456,  864,  105,  307,  194,  693,  106,  107, 1805,  108,
  665,  569, 1278,  682,  665,  456,  109,  110,  111,  112,
  113,  384,  114,  665,  696,  115,  116,  117,  118,  119,
 1732,  106,  107,  494,  108,  613, 1833,  613,  960, 1645,
  665,  109,  110,  111,  112,  113,  361,  114,  236, 1777,
  115,  116,  117,  118,  119,  328,  105, 1676, 1460,  613,
  283,  589, 1464, 1677, 1092,  696, 1625,  665, 1093,  682,
  569,   79, 1511,  375,  571, 1433,  613,  426,  682,  665,
  234,  694,  362,  696,  375,  106,  107,  665,  108,  629,
  694,  528,  508,   98,  613,  109,  110,  111,  112,  113,
 1663,  114, 1646,  284,  115,  116,  117,  118,  119,  402,
 1800,  960, 1257,  105,  328,  588,  665,  665,  326,  665,
  589,  367,  709,  137,  696, 1678,  665,  665,  665,  665,
  665,  403,  665,  571,   99,  665,  665,  665,  665,  665,
  399,  529,  106,  107,  160,  108,  100,  244,  547, 1258,
  588, 1801,  109,  110,  111,  112,  113,  255,  114,  709,
 1001,  115,  116,  117,  118,  119,  327,  283,  402, 1530,
   43,  101,  709, 1756,   76,  530,  105,  195,  588,  256,
 1756,  588,  463,  268,  696,  209,  506,  696, 1755,  547,
  403,  194,  547,  531,   51, 1815, 1373,  588, 1802,  399,
  423, 1575,  260,  160,  483,  106,  107,  547,  108,  547,
  891,  210,  491, 1639,  283,  109,  110,  111,  112,  113,
  148,  114,  480,  588,  115,  116,  117,  118,  119,   43,
  508,  485,  594,  283,  623, 1235,  195,  275,  765,  547,
  102,  765,  105,  696,  693,  691,  161,  692,  765,  694,
  103,  765,  696,   51,  604,  765,  623,  899, 1736, 1737,
  765, 1748,  514,  765,  981,  693,  691,  594,  692,  250,
  694,  106,  107,  437,  108,  104,  901,  261,  328,  765,
 1643,  109,  110,  111,  112,  113,  264,  114, 1673,  604,
  115,  116,  117,  118,  119,  594,  329,  229,  594, 1624,
 1775,  871,  274,  765,  871,  161,  765,  765,   41,   79,
  250, 1797, 1511,  250,  594,  752,  765,  604,  693,  691,
  604,  692,   24,  694,  506,  519,  151,  269,  250,  695,
  330,  188, 1674,  765,  520,  283,  604,  290,  229, 1511,
  594,  229,  752, 1819, 1820,  693,  691, 1168,  692, 1229,
  694,  270, 1671,  871, 1650,  765,  229, 1687,  139,  675,
  765, 1688,  604,  680,  681,  683,  931,   41, 1831,  105,
  695,  148,  765, 1825,  765,  743,  191,  981,  903, 1838,
   47,   24, 1839,  696, 1722,  151, 1511,  797,  695,  798,
   49,  316,  981,  696,  149,  148,  696,  306,  106,  107,
  986,  108,  658,  589,  472,  696,  547,  319,  109,  110,
  111,  112,  113, 1703,  114,  986,  658,  115,  116,  117,
  118,  119,  547,   99,  658,  720,  547, 1511,  547,  722,
  931,  210,  547,  336,  743,  547, 1511, 1750,  589,   47,
  658,  508,  693,  691,  194,  692, 1509,  694,  547,   49,
  693,  691,  696,  692,  547,  694,  195,  547,  538, 1119,
  101,  696,  347,  472, 1757,  127,  589, 1511,  349,  589,
 1511,  539,  358,  508,  508, 1120,  368,  195, 1789,  149,
  148,  639,  689,  639,  639,  589,  639,  690, 1569,   52,
  693,  691, 1511,  692,  658,  694, 1738,  148,  508,  639,
   53,  370,  370,  689,  639,  696,  195,   54,  690,  508,
 1570,  589,  508, 1807, 1808,  188, 1759, 1571, 1813,   25,
 1121, 1816,  372,   79,  127,  195,  547, 1760,   48,  102,
  149,  148,   55,  195,  195,  506,  373, 1738,  148,  103,
  195, 1811, 1747, 1828,  528, 1367,  376, 1369,  547, 1122,
  338, 1572,   50,  339,  150,  547,  689,  377,  195,  547,
  370,  690, 1205, 1790,  104,  574,  547,  506,  506, 1840,
  195,   45,  195,  149,  148,  241, 1206,  229,   25,   80,
   81, 1738,  148,  689,  547,  191,  151,   48,  690,  547,
  458,  870,  506,  128,  529,  195,  723,   56,  340,  384,
 1207,  152,  116,  506,  216,  151,  506,   46,  485,  529,
  191,   50,  898,  519,  547,  466, 1123, 1091,  547,  914,
  195,  467,  520,  216,  921,  423,  151,  386,  530,   57,
   45,  470,  291,   58,  928,  191,  932,  933,  934,  935,
  936,  423,  928,  138,  139,  216,  531,  695,  216,  529,
  529,  471,  128,  529, 1208,  151,  388,  695, 1205,  473,
  695,  116,  682,  216,  475,  231,   46,  292,  529,  695,
  529,  485, 1206,  699,  151,  241,  698,  396,  397,  398,
  689,  775,  216,  151,  774,  690, 1509,  872,  689,  151,
  872,  202,  203,  690,  485,  293, 1207,  241,  294,  514,
  529,  639,  232,  476,  241,  406,  407,  151,  639,  925,
  477,  979,  695, 1713,  295,  960,  695,  639,  959, 1321,
 1629,  682,  492, 1073, 1080,  695, 1072,  851,  689, 1172,
  639,  393, 1171,  690, 1044, 1045,  639,  639,  241,  872,
  296,  350, 1216, 1222,  639, 1072, 1221,  395,  123,  123,
 1208,  956,  979,  979,  979,  979,  979,  979,  979, 1477,
 1713,  396,  397,  398,  639,  202,  203,  639,  494,  695,
  979,  979,  979,  979,  693,  691,  504,  692,  351,  694,
  352,   14,  495,  695,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  639, 1266,  639,  639, 1265, 1205,  411,
  412, 1713,  979,  353,  979,  518, 1271,  639,  416, 1270,
 1796,  639, 1206, 1381, 1470, 1500, 1380, 1171,  695,  527,
 1100, 1101, 1105,  535,  548, 1113, 1115, 1117,  354, 1125,
 1128, 1131, 1133, 1136, 1139,  555, 1207, 1143, 1144,  558,
 1146, 1713, 1148,  568, 1713, 1151, 1152, 1153, 1154, 1155,
 1156,  892, 1506,  896,  572,  695,  361,  900,  902,  904,
 1824,  622, 1160,  626,  423,  628, 1713,  529,  639, 1837,
  361,  630, 1756,  661,  639,  662,  195,  663,  361,  664,
  665,  666,  667,  529,  639,  668,  529,  529,  669,  529,
 1208,  670,  671,  529,  361,  672,  529,  673,  639,  639,
  639,  639,  639,  639,  678,  679,  688,  152,  485,  529,
  696,  695,  639,  706,  697,  529,  701,  715,  529,  672,
  718,  639,  639,  639,  639,  639,  639,  639,  639,  639,
  639,  639,  723,  639,  725,  195,  639,  639,  363,  639,
  639,  639,  639,  639,  639,  639,  639,  739,  361,  639,
  639,  639,  639,  639,  721,  693,  691,  743,  692,  748,
  694,  744,  750,  672,  751,  753,  152,  754,  519,  764,
  765,  767,  768,  979,  777,  979,  979,  801,  672,  363,
  682,  802,  363,  979,  803,  808,  813,  529,  275,  817,
  818,  979,  820,  682, 1312,  819,  979,  363,  822,  979,
  823,  828,  682,  834,  835,  838,  839,  848,  859,  529,
  850,  851,  689,  979,  861,  860,  529,  690,  979,  979,
  529,  865,  979,  916,  924,  693,  691,  529,  692,  979,
  694,  979,  979,  693,  691,  979,  692,  655,  694,  979,
  530,  926,  979,  955,  979,  529,  979,  954,  957,  682,
  529,  979,  979,  928,  979, 1114,  962, 1118,  682, 1126,
 1129,  966, 1134, 1137, 1140, 1324, 1325,  967, 1327, 1329,
  979,  979,  992,  979,  977,  529, 1334,  983,  655,  529,
  530,  530,  999, 1013,  530,  995,  656,  979,  657,  979,
  979,  979, 1018,  829,  979, 1347,  655,  979,  979,  530,
 1023,  530,  682, 1354, 1024,  423, 1026,  979,  423,  780,
 1160, 1030, 1031, 1039, 1362,  423, 1364, 1365, 1056, 1040,
 1041,  979, 1033, 1047,  979, 1061, 1070,  656, 1050,  657,
  656,  530,  657, 1063,  195,  970,  693,  691,  485,  692,
  979,  694, 1075, 1087, 1088,  656,  979,  657,  423, 1107,
 1145,  979, 1150,  693,  691,  195,  692,  711,  694, 1147,
 1149, 1169,  979,  979,  689,  979, 1341,  693,  691, 1157,
  692,  979,  694, 1183, 1186, 1181,  195, 1189, 1199, 1202,
 1204, 1210,  195, 1219,  960,  698,  152, 1227, 1415, 1224,
 1238, 1247, 1259,  689,  693,  691, 1708,  692,  690,  694,
  979,  979, 1249,  979, 1269,  195, 1267,  152, 1276, 1280,
  979,  979,  979,  979,  979, 1281,  979, 1282, 1283,  979,
  979,  979,  979,  979,  195,  960,  960,  960,  960,  960,
  972,  960,  195,  195, 1292, 1301,  152,  485, 1294,  195,
  672, 1081, 1304,  960,  960,  960,  960,  672, 1305, 1306,
  672, 1307, 1313, 1476,  672,  152, 1314,  195, 1465,  363,
 1318, 1319,  672,  689,  152, 1320,  363, 1473,  690,  363,
  152,  689, 1322,  363, 1478,  960,  690,  960,  672, 1323,
 1330,  363, 1331, 1332, 1333, 1335, 1336, 1337,  152, 1338,
 1339, 1340,  889, 1360,  195, 1357, 1366,  363,  530, 1382,
 1384, 1391,  672,  423, 1388,  195, 1389,  423, 1390, 1392,
 1393, 1400, 1412,  550,  530, 1405, 1420,  530,  530, 1403,
  530,  363, 1424,  496,  530, 1410,  485,  530, 1425, 1426,
 1427, 1428,  672, 1429, 1432, 1437, 1447,  672, 1454, 1458,
  530, 1459, 1463, 1469, 1315, 1490,  530,  693,  691,  530,
  692,  363,  694,  519, 1496,  655, 1342,  693,  691,  672,
  692, 1502,  694, 1566,  496,  655, 1497,  496,  655, 1503,
 1576,  672, 1578,  672,  689, 1581, 1520,  655,  363,  690,
 1579, 1582,  496, 1345,  693,  691,  663,  692, 1580,  694,
  363,  689,  363, 1583,  519, 1585,  690,  519, 1586, 1587,
 1595, 1605, 1597, 1600,  656,  689,  657, 1604, 1606, 1608,
  690, 1609,  519, 1610,  656, 1611,  657,  656,  530,  657,
  726, 1614, 1612, 1613,  655, 1615,  656,  663,  657,  680,
  663,  680,  689,  655, 1616, 1617, 1618,  690, 1619, 1620,
  530, 1621, 1622,  423, 1623,  663,  960,  530,  960,  960,
 1630,  530, 1631, 1632, 1651, 1648,  960,  519,  530, 1346,
  693,  691,  130,  692,  960,  694, 1653, 1654, 1659,  960,
 1655, 1656,  960,  656, 1657,  657,  530,  655, 1658, 1660,
 1661,  530,  656, 1652,  657, 1666,  960, 1667,  960,  655,
  983,  960,  960, 1670, 1669,  960, 1690, 1706,  519, 1697,
 1696,  519,  960, 1723,  960,  960,  530, 1662,  960, 1731,
  530, 1744,  960, 1753, 1772,  960,  519,  960,  519,  960,
 1767, 1769, 1774, 1776,  960,  960,  656,  960,  657, 1781,
 1782,  983,  983,  983,  983,  983,  983,  983,  656, 1793,
  657, 1794, 1764,  960,  960, 1686,  960, 1765,  158,  983,
  983,  983,  983, 1689, 1787, 1810, 1814, 1817, 1818, 1821,
  960, 1756,  960,  960,  960, 1832, 1835,  960, 1836,    1,
  960,  960,  619,  660, 1712,   29,   31,  202,  747,  148,
  960,  983,  270,  983,  367,  689,  517,  648,  268, 1702,
  690,   81,  669, 1704,  960,  689,  453,  960,  212,  943,
  690,  496,  275,  496,  192,  517,  942,  496,  395,  517,
  496,  680,  678,  960,  660,  674,  105,  660,  676,  960,
  274, 1712,  689,  496,  960,  193,  368,  690,  288,  704,
  286,  519,  660,  519,  703,  960,  960,  519,  960,  222,
  519,  323,  287,  705,  960,  106,  107,  331,  108,  423,
  340, 1749,  322,  519, 1752,  109,  110,  111,  112,  113,
  613,  114, 1712,  329,  115,  116,  117,  118,  119,  714,
   27, 1795,   30,  960,  960,  517,  960,   39, 1371,  135,
  465,  474, 1302,  960,  960,  960,  960,  960,  464,  960,
  956,  849,  960,  960,  960,  960,  960, 1086,  689, 1300,
  245,  496, 1712,  690,  663, 1712, 1298,  663,  693,  691,
 1288,  692, 1588,  694,  663,  519,  517,  663, 1589,  517,
  581,  663,  814,  496, 1277, 1745,  663, 1712,  537,  663,
  496,  519, 1226,  512,  517,  519,  517,  519,  523,  993,
 1699,  519, 1289,  988,  519,  663,  552, 1568, 1763, 1028,
  796,  553,  983,  519,  983,  983, 1770,  519, 1574, 1768,
  519, 1804,  983,  519,  496, 1414, 1021, 1746,  230,  663,
  983, 1720, 1705,  663, 1830,  983, 1798, 1217,  983, 1396,
 1523, 1730,  663,  560,  988,  988,  988,  988,  988,  988,
  988,  543,  983,  534,  519, 1444,  800,  983,  983,  663,
 1062,  983,  988,  988,  988,  988,  836,  137,  983,  230,
  983,  983,  230,  734,  983, 1053,  505,  984,  983,  738,
  997,  983,  288,  983,  979,  983,  663,  230,  980, 1695,
  983,  983, 1201,  983,  988,  519,  988, 1636,  663, 1809,
 1780, 1348,  693,  691, 1841,  692,  747,  694,  963,  983,
  983, 1195,  983, 1359,    0, 1409, 1326,  519,    0,    0,
    0,    0,    0,    0,  519,    0,  983,    0,  983,  983,
  983,    0,    0,  983,    0,  519,  983,  983,    0,    0,
    0, 1349,  693,  691,    0,  692,  983,  694,    0,    0,
    0,  660,    0,  519,  660,    0,    0,    0,  519,  486,
  983,  660,    0,  983,  660, 1350,  693,  691,  660,  692,
    0,  694,    0,  660,    0,    0,  660,    0,    0,  983,
 1351,  693,  691,    0,  692,  983,  694,  519,    0,    0,
  983,    0,  660,  517,    0,    0,    0,    0,    0,    0,
  486,  983,  983,  486,  983,    0,  689,    0,    0,    0,
  983,  690,  517,  517,    0,  517,  660,    0,  486,  517,
  660,    0,  517,    0,    0,    0, 1352,  693,  691,  660,
  692,    0,  694,    0,    0,  517,    0,    0,    0,  983,
  983,  517,  983,    0,    0,    0,  660,    0,    0,  983,
  983,  983,  983,  983,    0,  983,  984,    0,  983,  983,
  983,  983,  983,    0,    0,  988,    0,  988,  988,    0,
    0,    0,    0,  660,    0,  988,    0,    0, 1353,  693,
  691,    0,  692,  988,  694,  660,    0,    0,  988,    0,
    0,  988,    0,    0,    0,    0,    0,  984,  984,  984,
  984,  984,  984,  984,    0,  988,  241,    0,  230,    0,
  988,  988,    0,  517,  988,  984,  984,  984,  984,    0,
    0,  988,    0,  988,  988,  180,    0,  988,    0,    0,
    0,  988,    0,    0,  988,  517,  988,    0,  988,    0,
  689,    0,  517,  988,  988,  690,  988,  984,    0,  984,
    0,  517,    0,  517, 1491,  693,  691,    0,  692,    0,
  694,    0,  988,  988,    0,  988,  180,    0,    0,  180,
    0,  517,    0,    0,    0,    0,  517,    0,    0,  988,
  689,  988,  988,  988,  180,  690,  988,    0,    0,  988,
  988,  905,  906,  907,  908,  909,  910,  911,  912,  988,
    0,    0,    0,    0,  689,  517,  241,    0,   98,  690,
    0,    0,  222,  988,    0,    0,  988,    0,    0,  689,
    0,    0,    0,    0,  690,    0,    0,    0,  241,    0,
    0,    0,  988,    0,    0,  241,    0,  486,  988,  486,
    0,    0,    0,  988,    0,    0,  486,    0,    0,   99,
    0,    0,    0,  222,  988,  988,  222,  988,    0,  486,
    0,  100,    0,  988,    0,  689,    0,    0,    0,  241,
  690,  222,    0,    0,    0,  132, 1492,  693,  691,    0,
  692,    0,  694,    0,    0,    0,  101,    0,    0,    0,
    0,    0,  988,  988,    0,  988,    0,    0,    0,    0,
    0,    0,  988,  988,  988,  988,  988,    0,  988,  841,
    0,  988,  988,  988,  988,  988,    0,  689,  984,    0,
  984,  984,  690,    0,    0,    0,    0,    0,  984,    0,
    0,    0,    0,    0,    0,    0,  984,  486,    0,    0,
    0,  984,    0,    0,  984,    0,    0,    0,    0,    0,
  841,  841,  841,  841,  841,  102,  841,    0,  984,  486,
    0,    0,    0,  984,  984,  103,  486,  984,  841,  841,
  841,  841,    0,    0,  984,    0,  984,  984,    0,    0,
  984,    0,    0,  180,  984,    0,  519,  984,    0,  984,
  104,  984,    0,  689,    0,    0,  984,  984,  690,  984,
  486,    0,  841,    0,  180,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  984,  984,    0,  984,    0,
    0,    0,    0,    0,    0,  180,    0,  519,    0,    0,
  519,  180,  984,    0,  984,  984,  984,  106,  107,  984,
  108,    0,  984,  984,    0,  519,    0,  109,  110,  111,
  112,  113,  984,  114,  180,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  984,    0,  514,  984,
 1499,  693,  691,  180,  692,    0,  694,    0,    0,    0,
    0,    0,  180,    0,    0,  984,    0,    0,  180,    0,
  222,  984,  222,    0,    0,  250,  984,    0,    0,  222,
    0,    0,    0,    0,    0,    0,  180,  984,  984,  514,
  984,    0,  514,    0,    0,  689,  984,    0,    0,    0,
  690,    0,    0,    0,    0,    0,    0,  514,    0,  514,
    0,    0,    0,    0,    0,    0,  250,    0,    0,  250,
    0,  853,    0,  180,    0,  984,  984,    0,  984,    0,
    0,    0,    0,    0,  250,  984,  984,  984,  984,  984,
    0,  984,    0,    0,  984,  984,  984,  984,  984,    0,
    0,  841,    0,  841,  841,    0,    0,    0,    0,    0,
    0,  841,  853,  853,  853,  853,  853,    0,  853,  841,
  222,    0,    0,    0,  841,    0,    0,  841,    0,    0,
  853,  853,  853,  853,    0,    0,    0,    0,    0,    0,
   62,  841,  222,    0,    0,    0,  841,  841,    0,  222,
  841,   63,    0,    0,    0,    0,    0,  841,   64,  841,
  841,    0,    0,  841,  853,    0,    0,  841,    0,  176,
  841,   65,  841,    0,  841,    0,    0,    0,    0,  841,
  841,    0,  841,  222,    0,    0,  186,    0,    0,    0,
    0,    0,    0,    0,  519,    0,  519,    0,  841,  841,
    0,  841,    0,  519,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  841,  519,  841,  841,  841,
    0,    0,  841,    0,    0,  841,  841,    0,  176,    0,
    0,    0,   66,    0,    0,  841,    0,    0,    0,  689,
    0,    0,    0,    0,  690,  186,    0,    0,   67,  841,
    0,    0,  841,    0,    0,    0,  514,    0,    0,    0,
    0,    0,  187,    0,    0,   68,    0,    0,  841,    0,
    0,    0,    0,    0,  841,  514,  514,    0,  514,  841,
   69,    0,  514,    0,   70,  514,    0,   71,    0,    0,
  841,  841,    0,  841,  519,   72,    0,    0,  514,  841,
    0,    0,    0,  250,  514,  250,    0,    0,    0,    0,
    0,    0,  250,    0,   73,    0,  519,    0,    0,    0,
    0,  187,    0,  519,  856,    0,    0,    0,  841,  841,
    0,  841,    0,  853,    0,  853,  853,    0,  841,  841,
  841,  841,  841,  853,  841,    0,    0,  841,  841,  841,
  841,  841,    0,    0,    0,    0,  853,  519,    0,  853,
    0,    0,    0,    0,    0,  856,  856,  856,  856,  856,
    0,  856,    0,  853,    0,    0,  514,    0,  853,  853,
    0,    0,  853,  856,  856,  856,  856,    0,    0,  853,
    0,  853,  853,    0,    0,  853,    0,    0,  514,  853,
    0,    0,  853,  250,  853,  514,  853,    0,    0,    0,
    0,  853,  853,    0,  853,    0,  514,  856,    0,    0,
    0,    0,    0,    0,    0,  250,    0,  176,    0,    0,
  853,  853,  250,  853,  514,    0,    0,    0,    0,  514,
    0,    0,    0,    0,  186,    0,    0,  853,  176,  853,
  853,  853,    0,    0,  853,    0,    0,  853,  853,    0,
    0,    0,    0,    0,    0,  186,  250,  853,  514,  176,
    0,    0,    0,    0,    0,  176,    0,    0,    0,    0,
    0,  853,    0,    0,  853,    0,  186,    0,    0,    0,
    0,    0,  186,    0,    0,    0,    0,    0,  176,   98,
  853,    0,    0,    0,    0,    0,  853,    0,    0,    0,
    0,  853,    0,    0,    0,  186,    0,  176,    0,    0,
  187,    0,  853,  853,    0,  853,  176,    0,    0,    0,
    0,  853,  176,    0,  186,    0,    0,    0,    0,    0,
   99,  187,    0,  186,    0,    0,    0,    0,    0,  186,
  176,    0,  100,    0,    0,    0,  857,    0,    0,    0,
  853,  853,  187,  853,    0,    0,    0,  186,  187,    0,
  853,  853,  853,  853,  853,    0,  853,  101,    0,  853,
  853,  853,  853,  853,    0,    0,  856,  176,  856,  856,
    0,  187,    0,    0,    0,    0,  856,  857,  857,  857,
  857,  857,    0,  857,  186,    0,    0,    0,    0,    0,
  187,    0,  856,    0,    0,  857,  857,  857,  857,  187,
    0,    0,    0,    0,    0,  187,  856,    0,    0,    0,
    0,  856,  856,    0,    0,  856,    0,    0,    0,    0,
    0,    0,  856,  187,  856,  856,  102,    0,  856,  857,
    0,    0,  856,    0,    0,  856,  103,  856,    0,  856,
    0,    0,    0,    0,  856,  856,    0,  856,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  187,  104,    0,  856,  856,    0,  856,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  105,
  856,    0,  856,  856,  856,    0,    0,  856,    0,    0,
  856,  856,    0,    0,    0,    0,    0,    0,    0,    0,
  856,    0,    0,    0,    0,    0,    0,    0,  106,  107,
    0,  108,    0,    0,  856,  550,    0,  856,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,  117,
  118,  119,    0,  856,    0,    0,    0,    0,    0,  856,
    0,    0,    0,    0,  856,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  856,  856,    0,  856,    0,
    0,    0,    0,    0,  856,    0,    0,  924,  693,  691,
    0,  692,    0,  694,    0,    0,    0,    0,    0,  556,
    0,    0,    0,    0,    0,    0,  887,  886,  888,  851,
    0,    0,    0,  856,  856,    0,  856,    0,  857,    0,
  857,  857,    0,  856,  856,  856,  856,  856,  857,  856,
    0,    0,  856,  856,  856,  856,  856,    0,    0,    0,
    0,    0,    0,    0,  857,    0,    0,    0,    0,    0,
  851,    0,  851,  851,  851,    0,    0,    0,  857,    0,
    0,    0,    0,  857,  857,    0,    0,  857,  851,  851,
  851,  851,    0,    0,  857,    0,  857,  857,    0,    0,
  857,    0,    0,    0,  857,    0,    0,  857,    0,  857,
    0,  857,    0,    0,    0,    0,  857,  857,    0,  857,
    0,    0,  851, 1466, 1467,    0,    0,    0,  105, 1471,
 1472,    0, 1474, 1475,    0,  857,  857,    0,  857,    0,
 1479, 1480, 1481, 1482,    0, 1483, 1484, 1485, 1486, 1487,
 1488,    0,  857,    0,  857,  857,  857,  106,  107,  857,
  108,    0,  857,  857,    0,    0,    0,  109,  110,  111,
  112,  113,  857,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,    0,    0,    0,  857,    0,    0,  857,
    0,    0,  105,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  857,    0,    0,    0,    0,
    0,  857,    0,    0,    0,    0,  857,    0,    0,    0,
    0,  106,  107,    0,  108,    0,    0,  857,  857,    0,
  857,  109,  110,  111,  112,  113,  857,  114,  874,    0,
  115,  116,  117,  118,  119,    0,  689,    0,    0,    0,
    0,  690,    0,    0,  875,    0,    0,    0,    0,    0,
    0,  850,    0,    0,    0,  857,  857,    0,  857,    0,
    0,    0,  943,    0,    0,  857,  857,  857,  857,  857,
    0,  857,    0,    0,  857,  857,  857,  857,  857,    0,
  876,  851,    0,  851,  851,    0,    0,    0,    0,  877,
    0,  851,  850,    0,  850,  850,  850,  878,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  851,    0,    0,
  850,  850,  850,  850,    0,    0,  879,    0,  880,    0,
    0,  851,    0,    0,    0,    0,  851,  851,    0,    0,
  851,    0,  881,    0,  882,  883,  884,  851,    0,  851,
  851,    0,    0,  851,  850,    0,    0,  851,    0,    0,
  851,    0,  851,    0,  851,    0,    0,    0,    0,  851,
  851,    0,  851,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  946,  851,  851,
    0,  851,    0,    0,    0,  885,    0,    0,    0,    0,
    0,    0,    0,    0,  105,  851,    0,  851,  851,  851,
    0,    0,  851,    0,    0,  851,  851,    0,    0,    0,
    0,    0,    0,    0,    0,  851,    0,    0,    0,    0,
    0,    0,    0,  106,  107,    0,  108,    0,    0,  851,
    0,    0,  851,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,  851,    0,
    0,    0,    0,    0,  851,    0,    0,    0,    0,  851,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  851,  851,    0,  851,    0,    0,    0,    0,    0,  851,
    0,    0,    0,  693,  691,    0,  692,    0,  694,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  887,  886,  888,  852,    0,    0,    0,  851,  851,
    0,  851,    0,  850,    0,  850,  850,    0,  851,  851,
  851,  851,  851,  850,  851,    0,    0,  851,  851,  851,
  851,  851,    0,    0,    0,  943,    0,    0,    0,  850,
    0,    0,    0,    0,    0,  852,    0,  852,  852,  852,
    0,    0,    0,  850,    0,    0,    0,    0,  850,  850,
    0,    0,  850,  852,  852,  852,  852,    0,    0,  850,
    0,  850,  850,    0,    0,  850,    0,    0,    0,  850,
    0,    0,  850,    0,  850,    0,  850,    0,    0,    0,
    0,  850,  850,    0,  850,    0,    0,  852,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  944,
  850,  850,    0,  850,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  850,    0,  850,
  850,  850,    0,    0,  850,    0,    0,  850,  850,    0,
    0,    0,    0,    0,    0,  945,    0,  850,    0,    0,
  946,    0,    0,    0,    0,    0,  947,    0,    0,    0,
    0,  850,    0,    0,  850,    0,    0,  105,    0,    0,
    0,    0,    0,    0,    0,  948,    0,    0,    0,    0,
  850,    0,    0,    0,    0,    0,  850,    0,    0,    0,
    0,  850,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,  850,  850,    0,  850,  109,  110,  111,  112,
  113,  850,  114,  874,    0,  115,  116,  117,  118,  119,
    0,  689,    0,    0,    0,    0,  690,    0,    0,  875,
    0,    0,    0,    0,    0,    0,  855,    0,    0,    0,
  850,  850,    0,  850,    0,    0,    0,    0,    0,    0,
  850,  850,  850,  850,  850,    0,  850,    0,    0,  850,
  850,  850,  850,  850,    0,  876,  852,    0,  852,  852,
    0,    0,    0,    0,  877,    0,  852,  855,    0,  855,
  855,  855,  878,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  852,    0,    0,  855,  855,  855,  855,    0,
    0,  879,    0,  880,    0,    0,  852,    0,    0,    0,
    0,  852,  852,    0,    0,  852,    0,  881,    0,  882,
  883,  884,  852, 1633,  852,  852,    0,    0,  852,  855,
    0,    0,  852,    0,    0,  852,    0,  852,    0,  852,
    0,    0,    0,    0,  852,  852,    0,  852, 1634,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  852,  852,    0,  852,    0,    0,    0,
  885,    0,    0,    0,    0,    0,    0,  524,    0,    0,
  852,    0,  852,  852,  852,    0,    0,  852,    0,    0,
  852,  852,    0,    0,    0,    0,    0,    0,    0,    0,
  852,    0,    0,    0,    0,    0, 1635,    0,    0,    0,
    0,    0,    0,    0,  852,    0,    0,  852,  524,    0,
    0,  524,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  852,    0,    0,  524,    0,  524,  852,
  105,    0,    0,    0,  852,    0,    0,    0,  645,    0,
    0,  417,    0,  418,    0,  852,  852,    0,  852,    0,
    0,    0,    0,    0,  852,    0,  420,    0,  524,  106,
  107,  277,  108,    0,    0,    0,    0,    0,    0,  109,
  110,  111,  112,  113,    0,  114,    0,    0,  115,  116,
  117,  118,  119,  852,  852,    0,  852,    0,  855,    0,
  855,  855,    0,  852,  852,  852,  852,  852,  855,  852,
    0,    0,  852,  852,  852,  852,  852,    0,    0,    0,
    0,    0,    0,    0,  855,    0,    0,    0,    0,   84,
    0,    0,    0,    0,    0,    0,   85,    0,  855,    0,
   86,   87,    0,  855,  855,  645,    0,  855,  417,   88,
  418,    0,    0,    0,  855,    0,  855,  855,    0,    0,
  855,   89,  455,  420,  855,    0,    0,  855,  277,  855,
    0,  855,    0,    0,    0,    0,  855,  855,    0,  855,
    0,    0,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,    0,    0,  855,  855,    0,  855,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  855,    0,  855,  855,  855,  106,  107,  855,
  108,    0,  855,  855,    0,    0,   90,  109,  110,  111,
  112,  113,  855,  114,    0,  524,  115,  116,  117,  118,
  119,    0,    0,   91,    0,    0,  855,    0,    0,  855,
    0,  524,  459,    0,  524,  524,    0,  524,   92,    0,
    0,  524,   93,    0,  524,  855,    0,    0,    0,  105,
    0,  855,    0,   94,    0,  381,  855,  524,    0,    0,
    0,    0,    0,  524,  382,    0,  524,  855,  855,    0,
  855,    0,   95,    0,    0,    0,  855,  383,  106,  107,
    0,  108,    0,  384,  385,    0,    0,    0,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,  117,
  118,  119,  639,    0,    0,  855,  855,    0,  855,    0,
    0,  386,    0,    0,  387,  855,  855,  855,  855,  855,
    0,  855,    0,    0,  855,  855,  855,  855,  855,    0,
    0,    0,    0,    0,    0,  524,    0,    0,    0,    0,
    0,    0,  389,  390,    0,    0,    0,    0,    0,    0,
    0,    0,  381,    0,  391,    0,    0,  524,  392,    0,
    0,  382,    0,    0,  524,    0,  866,    0,  524,    0,
    0,    0,    0,    0,  383,  524,    0,  638,    0,    0,
  384,  385,    0,  633,    0,  633,  633,    0,  633,    0,
    0,    0,    0,  524,    0,    0,    0,    0,  524,  639,
    0,  633,    0,    0,    0,  682,  633,    0,  386,  641,
    0,  387,    0,    0,    0,  393,    0,    0,    0,    0,
    0,  394,    0,  524,    0,    0,    0,  524,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,  389,
  390,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,  391,    0,    0,    0,  392,    0,    0,    0,  400,
    0,    0,    0,  640,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,  105,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,  867,  868,  869,    0,    0,  641,    0,    0,    0,
  106,  107,  393,  108,    0,    0,    0,    0,  394,    0,
  109,  110,  111,  112,  113,    0,  114,    0,  395,  115,
  116,  117,  118,  119,    0,  645,    0,    0,  417,    0,
  418,    0,  396,  397,  398,  399,  202,  203,    0,    0,
    0,    0,    0,  420,    0,    0,  400,    0,  277,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,  389,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  642,  643,  644,    0,    0,
    0,    0,    0,  633,    0,    0,    0,    0,    0,    0,
  633,  537,    0,    0,    0,    0,    0,    0,    0,  633,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  633,    0,    0,    0,    0,    0,  633,  633,
    0,    0,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,  537,    0,    0,  537,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  633,    0,    0,  633,
  537,    0,  537,    0,    0,    0,    0,    0,    0,    0,
   76,    0,  399,    0,    0,    0,    0,  645,    0,    0,
  417,    0,  418,  400,    0,  633,    0,  633,  633,    0,
    0,    0,  537,    0,    0,  420,  561,    0,    0,  633,
  277,  106,  410,  633,  108,    0,    0,    0,    0,    0,
  413,  414,  415,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,  561,  561,    0,  561,    0,
    0,    0,    0,    0,    0,  561,  561,  561,  561,  561,
    0,  561,    0,    0,  561,  561,  561,  561,  561,    0,
    0,    0,  381,    0,    0,    0,    0,    0,    0,    0,
  633,  382,    0,    0,    0,    0,  633,    0,    0,    0,
    0,    0,    0,    0,  383,    0,  633,    0,    0,    0,
  384,  385,    0,    0,    0,    0,    0,    0,    0,    0,
  633,  633,  633,  633,  633,  633,    0,    0,    0,  639,
    0,    0,    0,    0,  633,    0,    0,    0,  386,    0,
    0,  387,    0,  633,  633,  633,  633,  633,  633,  633,
  633,  633,  633,  633,    0,  633,    0,    0,  633,  633,
    0,  633,  633,  633,  633,  633,  633,  633,  633,  389,
  390,  633,  633,  633,  633,  633,    0,    0,    0,    0,
    0,  391,  820,    0,    0,  392,    0,    0,    0,  537,
    0,    0,    0,  862,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  537,    0,    0,    0,  537,
    0,  537,    0,    0,    0,  537,    0,    0,  537,    0,
    0,    0,    0,  820,    0,    0,  820,    0,    0,    0,
    0,  537,    0,    0,  381,    0,  641,  537,    0,    0,
  537,  820,  393,  382,    0,    0,  645,    0,  394,  417,
    0,  418,    0,    0,    0,    0,  383,    0,  395,    0,
    0,    0,  384,  385,  420,    0,    0,    0,    0,  277,
    0,    0,  396,  397,  398,  399,  202,  203,    0,    0,
    0,  639,    0,    0,    0,    0,  400,    0,    0,    0,
  386,    0,    0,  387,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,  537,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,  389,  390,  115,  116,  642,  643,  644,    0,    0,
    0,  537,    0,  391,    0,    0,    0,  392,  537,    0,
    0,    0,  537,  479,    0,  640,  417,    0,  418,  537,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,  449,    0,    0,  277,  537,    0,    0,
    0,    0,  537,  449,  449,  449,  449,  449,  449,  449,
  449,  449,  449,  449,  449,  449,  449,  449,  641,    0,
  449,  449,  449,  449,  393,  449,    0,  537,  449,  449,
  394,  537,  449,  449,  449,  449,  449,  449,  449,  449,
  395,  449,  449,  449,  449,  449,    0,    0,    0,    0,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,    0,    0,    0,  820,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  381,    0,  115,  116,  642,  643,  644,
    0,    0,  382,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  280,  383,    0,    0,    0,    0,
  820,  384,  385,  820,    0,    0,    0,    0,  789,    0,
  820,    0,    0,  820,    0,    0,    0,  820,    0,    0,
  639,    0,  820,  280,  280,  820,  280,    0,    0,  386,
    0,    0,  387,  280,  280,  280,  280,  280,    0,  280,
    0,  820,  280,  280,  280,  280,  280,    0,    0,  789,
    0,    0,  789,    0,    0,    0,    0,    0,    0,    0,
  389,  390,    0, 1102,    0,  820, 1103,  789,  820,  820,
  381,    0,  391,    0,    0,    0,  392,    0,  820,  382,
    0,    0,    0,    0,  862,    0,    0,    0,    0,    0,
    0,    0,  383,    0,    0,  820,    0,  479,  384,  385,
  417,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  420,    0,  820,    0,    0,
  277,    0,  820,    0,    0,    0,  386,  641,    0,  387,
    0,    0,    0,  393,  820,    0,  820,    0,    0,  394,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,  389,  390,    0,
    0,    0,    0,  396,  397,  398,  399,  202,  203,  391,
    0,    0,    0,  392,    0,    0,    0,  400,    0,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1104,    0,
  393,    0,    0,    0,    0,    0,  394,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  395,    0,    0,  479,
    0,    0,  417,    0,  418,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,  420,    0,    0,
  789,    0,  277,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,    0,
    0,  115,  116,  117,  118,  119,    0, 1112,    0,    0,
 1103,    0,    0,    0,  381,    0,  789,    0,    0,  789,
    0,    0,    0,  382,    0,    0,  789,    0,    0,  789,
    0,    0,    0,  789,    0,    0,  383,    0,  789,    0,
    0,  789,  384,  385,    0,    0,    0,    0,  479,    0,
    0,  417,    0,  418,    0,    0,    0,  789,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
  386,  277,    0,  387,    0,    0,    0,    0,    0,    0,
    0,  789,    0,    0,  789,  789,    0,    0,    0,    0,
    0,    0,    0,    0,  789,    0,    0,    0,    0,    0,
    0,  389,  390,    0,    0,    0,    0,    0,    0,    0,
    0,  789,    0,  391,    0,    0,    0,  392,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  789,    0,    0,    0,    0,  789,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  789,    0,  789,    0,    0,    0,    0,    0,    0, 1116,
    0,    0, 1103,    0,    0,    0,  381,    0,    0,    0,
    0,    0, 1104,    0,  393,  382,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,  383,    0,
  395,    0,    0,    0,  384,  385,    0,    0,    0,    0,
    0,    0,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,    0,  386,    0,    0,  387,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  389,  390,  115,  116,  117,  118,  119,
    0,    0,    0,    0,    0,  391,    0,    0, 1124,  392,
    0, 1103,    0,    0,    0,  381,    0,    0,    0,    0,
    0,    0,    0,    0,  382,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  383,    0,    0,
    0,    0,  479,  384,  385,  417,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,    0,    0,    0, 1104,  277,  393,    0,    0,    0,
    0,  386,  394,    0,  387,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,  398,  399,
  202,  203,  389,  390,    0,    0,    0,    0,    0,    0,
  400,    0,    0,    0,  391,    0,    0,    0,  392,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1104,    0,  393,    0,    0,    0,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  479,    0,    0,  417,    0,  418,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,  420,    0,    0,    0,    0,  277,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,    0,  115,  116,  117,  118,
  119,    0, 1127,    0,    0, 1103,    0,    0,    0,  381,
    0,    0,    0,    0,    0,    0,    0,    0,  382,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,    0,
    0,    0,    0,  479,    0,    0,  417,    0,  418,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  420,    0,    0,    0,  386,  277,    0,  387,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  389,  390,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  391,    0,
    0,    0,  392,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1132,    0,    0, 1103,    0,    0,
    0,  381,    0,    0,    0,    0,    0, 1104,    0,  393,
  382,    0,    0,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,  383,    0,  395,    0,    0,    0,  384,
  385,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,  386,    0,    0,
  387,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  389,  390,
  115,  116,  117,  118,  119,    0,    0,    0,    0,    0,
  391,    0,    0, 1135,  392,    0, 1103,    0,    0,    0,
  381,    0,    0,  790,    0,    0,    0,    0,    0,  382,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  383,    0,    0,    0,    0,  479,  384,  385,
  417,    0,  418,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  790,  420,    0,  790,    0, 1104,
  277,  393,    0,    0,    0,    0,  386,  394,    0,  387,
    0,    0,  790,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  396,  397,  398,  399,  202,  203,  389,  390,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,  391,
    0,    0,    0,  392,  401,  402,  403,  404,  405,  406,
  407,  408,  409,  106,  410,    0,  108,    0,    0,  411,
  412,    0,  413,  414,  415,  111,  112,  113,  416,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1104,    0,
  393,    0,    0,    0,    0,    0,  394,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  395,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  396,  397,  398,  399,  202,  203,    0,    0,    0,    0,
    0,    0,    0,    0,  400,    0,    0,    0,    0,    0,
    0,    0,    0,  401,  402,  403,  404,  405,  406,  407,
  408,  409,  106,  410,    0,  108,    0,    0,  411,  412,
    0,  413,  414,  415,  111,  112,  113,  416,  114,    0,
    0,  115,  116,  117,  118,  119,    0, 1138,    0,    0,
 1103,    0,    0,    0,  381,    0,    0,  479,    0,  484,
  417,    0,  418,  382,    0,  790,    0,    0,    0,    0,
    0,    1,    0,    0,    0,  420,  383,    0,    0,    0,
  277,    0,  384,  385,    0,    0,    0,    0,    0,    0,
    0,    0,    2,    0,    0,    0,    0,    0,    0,    0,
    3,    0,    0,    0,    0,    0,    0,    4,    0,    5,
  386,    6,    0,  387,    0,    0,    7,    0,    0,    0,
    0,  790,    0,    8,  790,    0,    0,    0,    0,    0,
    0,  790,    0,    0,  790,    0,    0,    9,  790,    0,
    0,  389,  390,  790,    0,    0,  790,    0,   10,    0,
    0,    0,    0,  391,    0,  479,    0,  392,  417,    0,
  418,    0,  790,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  420,    0,    0,    0,    0,  277,    0,
    0,    0,    0,    0,    0,    0,  790,    0,    0,  790,
  790,    0,    0,    0,    0,    0,    0,    0,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1104,    0,  393,   11,  790,    0,    0,    0,
  394,    0,   12,    0,    0,   13,    0,   14,   15,    0,
  395,    0,    0,    0,    0,    0,    0,    0,  790,    0,
    0,    0,    0,  790,  396,  397,  398,  399,  202,  203,
    0,    0,   16,    0,    0,  790,  582,  790,  400,    0,
  583,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,  584,  108,
    0,  585,  411,  412,  381,  413,  414,  415,  111,  112,
  113,  416,  114,  382,    0,  115,  116,  117,  118,  119,
    0,   17,    0,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,    0,    0,   18,   19,    0,    0,
    0,    0,  586,    0,    0,    0,    0,    0,  587,  588,
  589,    0,  590,  591,  592,    0,  593,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  594,    0,  595,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  388,
    0,  389,  390,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  381,  391,    0,    0,    0,  392,    0,  596,
    0,  382,    0,    0,  954,    0,  954,  954,    0,  954,
    0,    0,    0,    0,  383,    0,  597,    0,    0,    0,
  384,  385,  954,    0,    0,    0,    0,  954,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  598,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  386,    0,
    0,  387,    0,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,    0,  599,  600,    0,    0,  388,    0,  389,
  390,    0,    0,    0,  396,  397,  398,  399,  202,  203,
  601,  391,    0,    0,    0,  392,    0,    0,  400,    0,
    0,    0,    0,    0,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,    0,    0,  115,  116,  117,  118,  119,
    0,    0,  682,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  393,    0,    0,    0,  639,    0,  394,  639,
    0,  639,    0,    0,    0,    0,    0,    0,  395,    0,
    0,    0,  220,    0,  639,    0,  220,    0,    0,  639,
    0,    0,  396,  397,  398,  399,  202,  203,    0,    0,
  220,    0,    0,    0,  220,    0,  400,  220,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,  954,    0,  115,  116,  117,  118,  119,  220,    0,
  954,    0,    0,    0,  220,  220,  220,    0,  220,  220,
  220,    0,  220,  954,  521,    0,    0,    0,    0,  954,
  954,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  220,    0,  220,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  954,    0,    0,
  954,    0,    0,    0,    0,  521,    0,    0,  521,    0,
    0,    0,    0,    0,    0,  220,    0,    0,    0,    0,
   80,    0,    0,  521,    0,  521,  954,    0,  954,  954,
    0,    0,  220,    0,    0,    0,    0,    0,    0,    0,
  954,    0,    0,    0,  954,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  220,  521,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  639,    0,    0,  220,
  220,    0,    0,  639,    0,    0,    0,    0,    0,    0,
    0,  954,  639,    0,    0,  419,  220,  954,  417,    0,
  418,    0,    0,    0,    0,  639,    0,  954,    0,    0,
    0,  639,  639,  420,    0,    0,    0,    0,  277,  639,
    0,  954,  954,  954,  954,  954,  954,    0,    0,    0,
    0,    0,    0,    0,    0,  954,    0,    0,    0,  639,
    0,    0,  639,    0,  954,  954,  954,  954,  954,  954,
  954,  954,  954,  954,  954,    0,  954,    0,    0,  954,
  954,    0,  954,  954,  954,  954,  954,  954,  954,  954,
  639,  639,  954,  954,  954,  954,  954,    0,    0,    0,
    0,    0,  639,    0,    0,    0,  639,    0,    0,    0,
    0,    0,  479,    0,    0,  417,    0,  418,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  420,    0,  521,    0,    0,  277,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  521,    0,
    0,    0,  521,    0,  521,    0,    0,    0,  521,    0,
    0,  521,    0,  639,    0,    0,    0,    0,    0,  639,
    0,    0,    0,    0,  521,    0,    0,    0,    0,  639,
  521,    0,    0,  521,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  639,  639,  639,  639,  639,  639,    0,
    0,    0,    0,    0,    0,    0,    0,  639,    0,    0,
    0,    0,    0,    0,    0,    0,  639,  639,  639,  639,
  639,  639,  639,  639,  639,  639,  639,    0,  639,    0,
    0,  639,  639,    0,  639,  639,  639,  639,  639,  639,
  639,  639,  381,    0,  639,  639,  639,  639,  639,    0,
    0,  382,  521,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  383,    0,    0,    0,    0,    0,
  384,  385,    0,    0,  521,    0,    0,    0,    0,    0,
    0,  521,    0,    0,    0,  521,    0,    0,    0,    0,
    0,    0,  521,    0,    0,    0,    0,    0,  386,    0,
    0,  387,    0,    0,    0,    0,    0,    0,    0,    0,
  521,    0,    0,    0,    0,  521,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  388,    0,  389,
  390,    0,    0,    0,    0,    0,    0,    0,    0,  381,
  521,  391,    0,    0,  521,  392,    0,    0,  382,    0,
    0,  479,    0,    0,  417,    0,  418,    0,    0,    0,
    0,  383,    0,    0,    0,    0,    0,  384,  385,  420,
    0,    0,    0,    0,  277,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  386,    0,    0,  387,    0,
    0,    0,  393,    0,    0,    0,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  395,    0,
    0,    0,    0,    0,    0,    0,  389,  390,    0,    0,
    0,    0,  396,  397,  398,  399,  202,  203,  391,    0,
    0,    0,  392,    0,    0,    0,  400,    0,    0,    0,
    0,    0,    0,    0,    0,  401,  402,  403,  404,  405,
  406,  407,  408,  409,  106,  410,    0,  108,    0,    0,
  411,  412,    0,  413,  414,  415,  111,  112,  113,  416,
  114,    0,    0,  115,  116,  117,  118,  119,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,    0,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  395,    0,    0,    0,    0,
    0,    0,    0,  674,    0,    0,    0,    0,    0,  396,
  397,  398,  399,  202,  203,    0,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,    0,    0,    0,
    0,    0,  401,  402,  403,  404,  405,  406,  407,  408,
  409,  106,  410,    0,  108,    0,    0,  411,  412,    0,
  413,  414,  415,  111,  112,  113,  416,  114,  381,    0,
  115,  116,  117,  118,  119,    0,    0,  382,    0,    0,
  479,    0,    0,  417,    0,  418,    0,    0,    0,    0,
  383,    0,    0,    0,    0,    0,  384,  385,  420,    0,
    0,    0,    0,  277,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  386,    0,    0,  387,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  389,  390,    0,    0,    0,
  479,  927,    0,  417,    0,  418,    0,  391,    0,    0,
    0,  392,    0,    0,    0,    0,    0,    0,  420,    0,
    0,  105,    0,  277,    0,    0,    0,    0,    0,    0,
  162,  163,  164,  165,  166,  167,  168,  169,  170,  171,
  172,  173,  174,  175,  176,  177,    0,    0,  178,  179,
  106,  107,    0,  108,    0,    0,  180,  181,  682,    0,
  109,  110,  111,  112,  113,  182,  114,  183,  393,  115,
  116,  117,  118,  119,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  395,    0,    0,    0,  852,    0,
    0,    0,  583,    0,    0,    0,    0,    0,  396,  397,
  398,  399,  202,  203,    0,    0,  853,    0,    0,    0,
  584,    0,  400,  585,    0,    0,    0,    0,    0,    0,
    0,  401,  402,  403,  404,  405,  406,  407,  408,  409,
  106,  410,    0,  108,    0,    0,  411,  412,    0,  413,
  414,  415,  111,  112,  113,  416,  114,  381,    0,  115,
  116,  117,  118,  119,  586,    0,  382,    0,    0,    0,
  587,  588,  589,    0,  590,  591,  592,    0,  593,  383,
    0,    0,    0,    0,    0,  384,  385,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  594,    0,
  595,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  596,    0,    0,    0,    0,    0,  381,    0,    0,
    0,    0,  388,    0,  389,  390,  382,    0,  597,  479,
    0,    0,  417,    0,  418,    0,  391,    0,    0,  383,
  392,    0,    0,    0,    0,  384,  385,  420,    0,    0,
  598,    0,  277,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  386,    0,    0,  387,    0,    0,    0,
    0,    0,    0,    0,    0,  599,  600,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  393,    0,    0,
    0,    0,  601,  394,  389,  390,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,  391,    0,    0,    0,
  392,    0,    0,    0,    0,    0,    0,  396,  397,  398,
  399,  202,  203,    0,    0,    0,    0,    0,    0,    0,
    0,  400,    0,    0,    0,    0,    0,    0,    0,    0,
  401,  402,  403,  404,  405,  406,  407,  408,  409,  106,
  410,    0,  108,    0,    0,  411,  412,    0,  413,  414,
  415,  111,  112,  113,  416,  114,    0,  393,  115,  116,
  117,  118,  119,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  395,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  396,  397,  398,
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
    0,    0,    0,  389,  390,    0,    0,    0,  479,    0,
    0,  417,    0,  418,    0,  391,    0,    0,    0,  392,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
    0,  277,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  393,    0,    0,    0,
    0,    0,  394,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  395,    0,    0,    0,  220,    0,    0,    0,
  220,    0,    0, 1130,    0,    0,  396,  397,  398,  399,
  202,  203,    0,    0,  220,    0,    0,    0,  220,    0,
  400,  220,    0,    0,    0,    0,    0,    0,    0,  401,
  402,  403,  404,  405,  406,  407,  408,  409,  106,  410,
    0,  108,    0,    0,  411,  412,    0,  413,  414,  415,
  111,  112,  113,  416,  114,  381,    0,  115,  116,  117,
  118,  119,  220,    0,  382,    0,    0,    0,  220,  220,
  220,    0,  220,  220,  220,    0,  220,  383,    0,    0,
    0,    0,    0,  384,  385,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  220,    0,  220,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  220,
    0,    0,    0,    0,    0,  381,    0,    0,    0,    0,
    0,    0,  389,  390,  382,    0,  220,  479,    0,    0,
  417,    0,  418,    0,  391,    0,    0,  383,  392,    0,
    0,    0,    0,  384,  385,  420, 1701,    0,  220,    0,
  277,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  386,    0,    0,  387,    0,    0,    0,    0,    0,
    0,    0,    0,  220,  220, 1315,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,    0,    0,
  220,  394,  389,  390,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,  391,    0,    0,    0,  392,    0,
    0,    0,    0,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,    0,  393,  115,  116,  117,  118,
  119,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  395,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1328,    0,    0,  396,  397,  398,  399,  202,
  203,    0,    0,    0,    0,    0,    0,    0,    0,  400,
    0,    0,    0,    0,    0,    0,    0,    0,  401,  402,
  403,  404,  405,  406,  407,  408,  409,  106,  410,    0,
  108,    0,    0,  411,  412,    0,  413,  414,  415,  111,
  112,  113,  416,  114,  381,    0,  115,  116,  117,  118,
  119,    0,    0,  382,    0,    0,  479,    0,    0,  417,
    0,  418,    0,    0,    0,    0,  383,    0,    0,    0,
    0,    0,  384,  385,  420,    0,    0,    0,    0,  277,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  386,    0,    0,  387,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  389,  390,    0,    0,    0,  954,    0,    0,  954,
    0,  954,    0,  391,    0,    0,    0,  392,    0,    0,
    0,  133,    0,    0,  954,  133,    0,    0,    0,  954,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,  133,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,    0,    0,    0,
  394,    0,    0,    0,    0,    0,    0,  133,    0,    0,
  395,    0,    0,  133,  133,  133,    0,  133,  133,  133,
    0,  133,    0,    0,  396,  397,  398,  399,  202,  203,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,  133,    0,  133,    0,    0,    0,  401,  402,  403,
  404,  405,  406,  407,  408,  409,  106,  410,    0,  108,
    0,    0,  411,  412,    0,  413,  414,  415,  111,  112,
  113,  416,  114,  381,  133,  115,  116,  117,  118,  119,
    0,    0,  382,    0,    0,    0,    0,    0,    0,    0,
    0,  133,    0,    0,    0,  383,    0,    0,    0,    0,
    0,  384,  385,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  133,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  386,
    0,    0,  387,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  830,    0,    0,    0,    0,  133,  133,
    0,    0,    0,  954,    0,    0,    0,    0,    0,    0,
  389,  390,  954,    0,    0,  133,    0,    0,    0,    0,
    0,    0,  391,    0,    0,  954,  392,    0,    0,    0,
    0,  954,  954,    0,  830,  992,  992,  830,  992,  992,
  992,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  830,  992,  992,  992,    0,    0,    0,  954,
    0,    0,  954,    0,    0,    0,    0,  814,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,  992,    0,    0,    0,    0,  394,
  954,  954,    0,    0,    0,    0,    0,    0,    0,  395,
    0,    0,  954,    0,    0,    0,  954,    0,  814,    0,
    0,  814,    0,  396,  397,  398,  399,  202,  203,    0,
    0,    0,    0,    0,    0,    0,  814,  400,    0,    0,
    0,    0,    0,    0,    0,    0,  401,  402,  403,  404,
  405,  406,  407,  408,  409,  106,  410,    0,  108,    0,
    0,  411,  412,    0,  413,  414,  415,  111,  112,  113,
  416,  114,    0,  954,  115,  116,  117,  118,  119,  954,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  954,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  954,  954,  954,  954,  954,  954,    0,
    0,    0,    0,    0,    0,    0,    0,  954,    0,    0,
    0,    0,    0,    0,    0,  831,  954,  954,  954,  954,
  954,  954,  954,  954,  954,  954,  954,    0,  954,    0,
    0,  954,  954,    0,  954,  954,  954,  954,  954,  954,
  954,  954,    0,    0,  954,  954,  954,  954,  954,    0,
    0,    0,    0,    0,    0,  830,  831,  993,  993,  831,
  993,  993,  993,    0,    0,  992,    0,    0,    0,    0,
    0,    0,    0,  992,  831,  993,  993,  993,  992,    0,
    0,  992,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  832,
    0,    0,    0,    0,    0,    0,  993,    0,    0,    0,
    0,  830,    0,    0,  830,    0,    0,  992,    0,    0,
    0,  830,    0,    0,  830,    0,  992,    0,  830,  814,
    0,    0,    0,  830,  992,    0,  830,    0,    0,    0,
  832,  994,  994,  832,  994,  994,  994,    0,    0,    0,
    0,    0,  830,  992,    0,  992,    0,    0,  832,  994,
  994,  994,    0,    0,    0,    0,    0,    0,    0,  992,
    0,  992,  992,  992,    0,    0,  830,    0,    0,  830,
  830,    0,    0,  793,    0,  814,    0,    0,  814,  830,
  994,    0,    0,    0,    0,  814,    0,    0,  814,    0,
    0,    0,  814,    0,    0,    0,  830,  814,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  992,    0,  793,    0,  814,  793,  830,    0,
    0,    0,    0,  830,    0,    0,    0,    0,    0,    0,
    0,    0,  793,    0,    0,  830,    0,  830,    0,    0,
  814,    0,    0,  814,  814,  791,    0,    0,    0,    0,
    0,    0,    0,  814,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  831,    0,    0,
  814,    0,    0,    0,    0,    0,    0,  993,    0,    0,
    0,    0,    0,    0,    0,  993,  791,    0,    0,  791,
  993,    0,  814,  993,    0,    0,    0,  814,    0,    0,
    0,    0,    0,    0,  791,    0,    0,    0,    0,  814,
    0,  814,    0,    0,    0,    0,    0,    0,    0,  792,
    0,    0,    0,  831,    0,    0,  831,    0,    0,  993,
    0,    0,    0,  831,    0,    0,  831,    0,  993,    0,
  831,    0,    0,    0,    0,  831,  993,    0,  831,    0,
    0,  832,    0,  822,    0,    0,    0,    0,    0,    0,
  792,  994,    0,  792,  831,  993,    0,  993,    0,  994,
    0,    0,    0,    0,  994,    0,    0,  994,  792,    0,
    0,  993,  786,  993,  993,  993,    0,    0,  831,    0,
    0,  831,  831,    0,  822,    0,    0,  822,    0,    0,
    0,  831,    0,  787,    0,    0,    0,  832,    0,    0,
  832,    0,  822,  994,    0,    0,    0,  832,  831,    0,
  832,    0,  994,  786,  832,    0,  786,    0,    0,  832,
  994,    0,  832,    0,  993,  793,    0,    0,    0,    0,
  831,  786,    0,    0,  787,  831,    0,  787,  832,  994,
    0,  994,    0,    0,    0,    0,    0,  831,    0,  831,
    0,    0,  787,    0,    0,  994,    0,  994,  994,  994,
    0,    0,  832,    0,    0,  832,  832,    0,    0,    0,
    0,    0,    0,    0,    0,  832,    0,    0,    0,    0,
  788,  793,    0,    0,  793,    0,    0,    0,    0,    0,
    0,  793,  832,    0,  793,    0,    0,  791,  793,    0,
    0,    0,    0,  793,    0,    0,  793,    0,  994,    0,
    0,    0,    0,  821,  832,    0,    0,    0,    0,  832,
    0,  788,  793,    0,  788,    0,    0,    0,    0,    0,
    0,  832,    0,  832,    0,    0,    0,    0,    0,  788,
    0,    0,    0,    0,    0,    0,  793,    0,    0,  793,
  793,    0,    0,  791,  821,    0,  791,  821,    0,  793,
    0,    0,    0,  791,    0,    0,  791,    0,    0,    0,
  791,  792,  821,    0,    0,  791,  793,    0,  791,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  791,    0,    0,    0,  793,    0,
    0,    0,    0,  793,    0,  822,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  793,    0,  793,  791,    0,
    0,  791,  791,    0,    0,    0,    0,  792,    0,    0,
  792,  791,    0,    0,  786,    0,    0,  792,    0,    0,
  792,    0,    0,    0,  792,    0,    0,    0,  791,  792,
  815,    0,  792,    0,    0,  787,    0,    0,    0,    0,
    0,  822,    0,    0,  822,    0,    0,    0,  792,    0,
  791,  822,    0,    0,  822,  791,    0,    0,  822,  823,
    0,    0,    0,  822,    0,    0,  822,  791,    0,  791,
  786,  815,  792,  786,  815,  792,  792,    0,    0,    0,
  786,    0,  822,  786,    0,  792,    0,  786,    0,  815,
    0,  787,  786,  824,  787,  786,    0,    0,    0,    0,
  823,  787,  792,  823,  787,    0,  822,    0,  787,  822,
  822,  786,    0,  787,    0,    0,  787,    0,  823,  822,
    0,    0,  788,    0,  792,    0,    0,    0,    0,  792,
    0,    0,  787,    0,  824,  786,  822,  824,  786,  786,
    0,  792,    0,  792,    0,    0,    0,    0,  786,    0,
    0,    0,  824,    0,    0,  821,  787,  812,  822,  787,
  787,    0,    0,  822,    0,  786,    0,    0,    0,  787,
    0,    0,    0,    0,    0,  822,    0,  822,  788,    0,
    0,  788,    0,    0,    0,    0,  787,  786,  788,    0,
    0,  788,  786,    0,    0,  788,    0,    0,  812,    0,
  788,  812,    0,  788,  786,    0,  786,    0,  787,    0,
    0,  821,    0,  787,  821,    0,  812,    0,    0,  788,
    0,  821,    0,    0,  821,  787,    0,  787,  821,    0,
    0,  816,    0,  821,    0,    0,  821,    0,    0,    0,
    0,    0,    0,  788,    0,    0,  788,  788,    0,    0,
    0,    0,  821,    0,    0,    0,  788,    0,    0,    0,
  825,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  816,  788,    0,  816,  821,    0,    0,  821,
  821,    0,    0,    0,    0,    0,    0,    0,    0,  821,
  816,    0,  815,    0,  813,  788,    0,    0,    0,    0,
  788,  825,    0,    0,  825,  817,  821,    0,    0,    0,
    0,    0,  788,    0,  788,    0,    0,    0,    0,  825,
    0,  823,    0,    0,    0,    0,    0,    0,  821,    0,
    0,    0,    0,  821,    0,  813,    0,    0,  813,    0,
    0,    0,    0,    0,    0,  821,  817,  821,  815,  817,
    0,  815,    0,  813,    0,  824,    0,    0,  815,    0,
    0,  815,    0,    0,  817,  815,    0,    0,    0,    0,
  815,    0,    0,  815,    0,    0,    0,  823,    0,    0,
  823,    0,    0,    0,    0,    0,    0,  823,    0,  815,
  823,    0,    0,    0,  823,    0,    0,    0,    0,  823,
    0,    0,  823,    0,    0,    0,    0,    0,    0,    0,
    0,  824,    0,  815,  824,    0,  815,  815,  823,  812,
    0,  824,    0,    0,  824,    0,  815,    0,  824,    0,
    0,    0,    0,  824,    0,    0,  824,    0,    0,    0,
    0,    0,  823,  815,    0,  823,  823,    0,    0,    0,
    0,    0,  824,    0,    0,  823,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  815,    0,    0,    0,    0,
  815,    0,  823,    0,    0,  812,  824,    0,  812,  824,
  824,    0,  815,    0,  815,  812,    0,    0,  812,  824,
    0,    0,  812,  816,  823,    0,    0,  812,    0,  823,
  812,    0,    0,    0,    0,    0,  824,    0,    0,    0,
    0,  823,    0,  823,    0,    0,  812,    0,    0,    0,
    0,    0,  825,    0,    0,    0,    0,    0,  824,    0,
    0,    0,    0,  824,    0,    0,    0,    0,    0,    0,
  812,    0,    0,  812,  812,  824,    0,  824,    0,  816,
    0,    0,  816,  812,    0,    0,  813,    0,    0,  816,
    0,    0,  816,    0,    0,    0,  816,  817,    0,    0,
  812,  816,    0,    0,  816,    0,    0,    0,  825,    0,
    0,  825,    0,    0,    0,    0,    0,    0,  825,    0,
  816,  825,  812,    0,    0,  825,    0,  812,    0,    0,
  825,    0,    0,  825,    0,    0,    0,    0,    0,  812,
    0,  812,  813,    0,  816,  813,    0,  816,  816,  825,
    0,    0,  813,  817,    0,  813,  817,  816,    0,  813,
    0,    0,    0,  817,  813,    0,  817,  813,    0,    0,
  817,    0,    0,  825,  816,  817,  825,  825,  817,    0,
    0,    0,  320,  813,    0,    0,  825,    0,    0,    0,
    0,    0,    0,    0,  817,    0,  816,    0,    0,    0,
    0,  816,    0,  825,    0,    0,    0,  813,    0,    0,
  813,  813,  320,  816,    0,  816,    0,    0,  817,  320,
  813,  817,  817,  320,  320,  825,  320,    0,    0,  320,
  825,  817,    0,    0,    0,    0,    0,  813,    0,    0,
    0,    0,  825,  320,  825,    0,    0,    0,  817,  320,
    0,    0,    0,    0,    0,    0, 1412,    0,    0,  813,
    0,    0,    0,    0,  813,    0,    0,    0,    0,    0,
  817,    0,    0,    0,    0,  817,  813,    0,  813,    0,
 1412,    0,    0,    0,    0,    0,    6,  817,    0,  817,
    0,    0,    0,    0,    0,    0,    0, 1531, 1532,    0,
 1533,    0,    0, 1534,    0,    0,    0,    0,    0,  320,
    6,    0,    0,    0,    0,    0,    0, 1535,    0,    0,
    0, 1531, 1532, 1536, 1533,    0,    0, 1534,  320,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1535,    0,  320,    0,    0,    0, 1536,    0,    0,
    0,    0,    0,  320,    0,    0,    0,    0,    0,    0,
    0,    0,  320,    0,  320,    0,    0,    0,    0,    0,
    0,  320,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1537,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  320,  320,   14,  320,    0,    0,    0, 1537,    0,    0,
  320,  320,  320,  320,  320,    0,  320, 1538,    0,  320,
  320,  320,  320,  320,    0,    0,   14,   16,    0,    0,
    0,    0,    0,    0,    0,    0, 1662,    0, 1539,    0,
    0, 1538,    0,    0,    0,  105,    0,    0,    0,    0,
    0,   16,    0,    0,    0,    0,  582,    0,    0,    0,
  583,    0, 1539,    0,    0,    0,    0,  780,    0,  105,
    0,    0,    0,    0, 1540,  107,    0,  108,  584,    0,
    0,  585,    0,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115, 1541,  117,  118,  119, 1540,  107,
    0,  108,  582,    0,    0,    0,  583,    0,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115, 1541,  117,
  118,  119,  586,    0,  584,    0,    0,  585,  587,  588,
  589,    0,  590,  591,  592,    0,  593,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  594,    0,  595,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  586,    0,
    0,    0,    0,    0,  587,  588,  589,    0,  590,  591,
  592,    0,  593,    0,    0,    0,    0,    0,    0,  596,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  594,    0,  595,    0,  597,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  598,    0,
    0,    0,    0,    0,    0,  596,    0,    0,    0,    0,
    0,    0,    0,  105,    0,    0,    0,    0,    0,    0,
    0,    0,  597,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  599,  600,    0,    0,    0,    0,    0,
    0,    0,  106,  107,  598,  108,    0,    0,    0,    0,
  601,    0,  109,  110,  111,  112,  113,    0,  114,  105,
    0,  115,  116,  117,  118,  119,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  599,
  600,    0,    0,    0,    0,    0,    0,    0,  106,  107,
    0,  108,    0,    0,    0,    0,  601,    0,  109,  110,
  111,  112,  113,    0,  114,    0,    0,  115,  116,  117,
  118,  119,
};
static short yycheck[] = {                                       9,
    4,   55,   12,   64,    4,   16,   16,  104,   18,   62,
    0,   63,  285,  509,  254,   56,  138,  139,  351,  149,
  359,  138,  139,  359,   85,  355,   67,   63,   57,  542,
   94,  330,  545,  551,  492,   89,   73,  419,  551,  697,
   69,  514,   70,   53,   54,   55,   56,   57,   58,   90,
  662, 1026,  328,   63,   64,   65,   97,   67,   95,   69,
   70,  262,   72,   73, 1246,   93,   68,  467,  279,   88,
  271,   81,   83,   83,   84,   85,  674,   87,   88,   89,
   90,   92,   92,   93,   94,   95,  744,   97,  152,   91,
  757, 1019, 1019,  129,  682,  131, 1019, 1027, 1026, 1026,
 1019,  130,  751, 1026,    0, 1019,    0, 1026, 1038,  150,
    0,  923, 1026,  364,  634,  715,    0,  376,    0,  129,
  130,  131,  151,  711,  143,  323,    0, 1057,  777,  226,
   44,   44,  627,  143,  187,   45,  748,  786,  846,  149,
  150,  151,  152,    0,  306,   41,  156,   41,   44,  961,
   44,   41,  815,   44,   44,   44,   44,   41,   41,  212,
   44,   63,    0,   59, 1074,   59,    0,    0,  971,   59,
    0,   41,   45,    0,   45,   59,  306,   59,    0,  419,
   45,   44, 1051,  754,   41,   59,   40,   44,   44,    0,
  839,  521,  696, 1452,   40,  528,  529,  530,  531, 1188,
   44,   40,   59,    0,   41,    0,   41,    0,    0,    0,
  156,   61,  551,    0,   41,  551,  313,   44,  269,   41,
  256,   59,   40, 1294,    0,   59,   59, 1364,  724,   59,
   41,  260,   59,   44,   40,    0,  290,   59,  286,  473,
  269, 1392, 1379,  274,  159,   63,  256,  577,   59,  283,
  260,  640,  262,  263,  355,  278,  301,   63,  459,  269,
  511,  271,   59, 1075,   59,  294,   59,   59,   59,  262,
  306,  320,   59,    0,  313,   44,  262, 1192,  554,   41,
  290,    0,   44,   59,  294,   45, 1181, 1392, 1183,    0,
  803,  889,  890,  329,   59,   41,  319, 1272,   44,  352,
  336,  354,  338,   63,  340,  257,  257,  343,   44,   41,
    0,  268,  338, 1069,    0,  368,   12,   59,  372,  329,
  361,    0,  920,  582,  337,   44,  336,  603,  338,  300,
  340, 1261,   59,  343,  362,  377,    0,  467,   91, 1244,
   59,  370,  262,  992, 1272, 1272,  307,  308,   59, 1272,
  962,  361,  362, 1272,  954,   41,   41,   93, 1272,  417,
  370,  374,  372,   40,  455,  823,  966,  967,   45,   59,
  265,   41,  376,   59,  387,    0, 1291,  400,  417,    0,
   59,  352,  334,  334,    0, 1456,  444,  436, 1037,  662,
 1572,  492,    0,  386,  278,   59,  280,  442,    0,  280,
  386, 1204,  317,  287,  285,  439,  448,  302,  365, 1278,
  420,    0,   41,  461,  462,  269,  442,  458,  441, 1678,
 1018,  278,  318,  280,  475,  262,  455,    0,   40,  320,
  287,  467,  473,   41,   59,  466,   44,  466,   59,  679,
  136,  470,  435,   59,  340,  455,  475,  569,  458,  459,
  300,   59,  725,   41,   44,  695,  466,   59,  340, 1215,
  470,   41,   41,  473,  320,  475,  386,  278,  478,  280,
   59,  264,  264,  526,  309,  748,  287,  866, 1408,   44,
  490, 1632,  258, 1413,  718, 1415,   59, 1382, 1670,  765,
  451,  318,  768,  383,  378,  383,  318, 1095, 1096,  383,
 1098, 1099,    0,  386,  331,  706,  782,  340, 1079,  331,
  721,   41, 1417,  340, 1503,  551,  400,  387,  340,  572,
 1450,  378,  217,  407,  277,   40, 1189, 1632,  274,  367,
  306,  328,  478,  809,  331,  304,    0,  264,  452, 1449,
  550,  551, 1724,  400,  331,  458,  556,  436,  458,  386,
  407,  850, 1689,  264, 1221,  394,  458,  441,  431,  959,
  387,   59,  436,  309, 1084,  387,  764,  378,  570,  396,
  623,  624,  454, 1068,  396,  485, 1284,   41,  582,  409,
   44, 1763,    0,  745,  441,  458,  837,  458, 1391,  400,
  626,  454, 1522,  458, 1463,   59,  407,   61, 1528, 1199,
  859,  447,  367, 1107, 1267,  397,  502,  501,  730,  328,
  454,  501,  331,  440,  309,  745,  626,  501,  440,  410,
  454,  454,    0,   41,  343,  635,   44,  454,  318, 1227,
  441,  635,  454,  262, 1283,  635,    0,  258,  760,  761,
  458,   59,  418,  440,  501,  440,  665, 1577,    0,  411,
  340, 1249,  458,  440,  386,  665, 1468,  454,  328,  454,
    0,  331,  698,   41, 1594,  411,   44,  454,  410,  348,
  397,  431,  284, 1006, 1430,  264,  278,  718,  280,  689,
   44,   59,  262,  262,  348,  306,  397,  697,  698,  962,
  501,  701, 1448,  318,   41,   59,  706,  387,  458,  307,
  308,  387,  387,  756,  757,  715,  396,   59,  718,  745,
  739,  396,  741,  723,  750,  340,  726,  387,  754,   59,
  466,  440,  445, 1698,  340,   12,  396, 1315,  318,  739,
  318,  741,   41,  452,  744,  454, 1403, 1070,  774,  304,
  750,    0,  943,  387,  754,  946, 1019, 1677, 1024,   42,
  440,   42,    0, 1026,  431,  328, 1686,  386,  331,  789,
 1036,  763,  387,  260,  774,    0,  802,  803, 1380, 1392,
 1698, 1698,   40, 1392, 1247, 1698,  378,  262,    0, 1698,
 1056,  458,   41, 1082, 1698,  508,  258, 1169,    0,  715,
   58,   44,  802,  803,  258, 1393,  440,  418, 1728,  801,
   59,  331,  410,  283,  808,  236,  386,  386,  397,  838,
  274,   59, 1270,  244,    0,  440,  820,    0, 1276,  848,
 1202,  320,   42,    0,   59, 1343,   40,   47,  838,  959,
  301,  833,  834,  835,  306,    0,  300,   59,  848, 1736,
 1737,  273,  306,  451,  342,  309, 1145,   59,  460,  136,
  860,   42,   43,  548,   45,  859,   47,  387,   41, 1404,
  278,   44,  280,  260,   41,  405,  396,  440, 1626,  287,
 1628,    0,    0,   59,  264,  403,   59,  489,  490,  452,
  492,  454,   59,  947,    0,   42,  264,  499,  500,  501,
  502,  503,  420,  505,   59,  405,  508,  509,  510,  511,
  512,  386, 1239,  944,  384,  273, 1451,  405,  340,    0,
  408,  948,  264,   41,  424,  262,  456,  945, 1159, 1192,
  417, 1162,  617,  959,  264,   41,  424, 1685, 1825, 1169,
   59,   59, 1054,  943,  944,  945,  946,  947,  948,    0,
  307,  308,  320,   59,  954,  460,  418,  411, 1285,  402,
  435,   45,  450,  262,  418,  301,  966,  967, 1288,  439,
  378,  971, 1202,    0,  328,  640,  277,  331,   59,  260,
  645,  442,  436, 1631,  489,  490,  456,  492,  263,  343,
   16,  325,  400, 1259,  499,  500,  501,  502,  503,  407,
  505,  356,  338,  508,  509,  510,  511,  512,   59, 1272,
 1030, 1031,  466,  296, 1343, 1501,  470, 1343, 1504, 1632,
  506,  264,  303, 1632, 1290,  352,  264, 1530, 1291, 1777,
  417,  411,   59,  441,  259,  343,  265,  312,  954,  264,
   58, 1629,    0,    0,  345,  270, 1072, 1047,  323,  386,
  966,  378,  264, 1079, 1048,  397,    0,   83, 1048, 1079,
  368, 1061,  264, 1651, 1566,  319, 1654,  397, 1395,  318,
  280, 1063, 1072,  302,  429,  285,  348, 1400,   44, 1079,
 1187, 1188, 1189, 1203,  452,  405,  440,  386,  264, 1081,
    0,  340,  460,  501,  451, 1087,  430,   41,  452,  280,
  454,   59,   59,  437,  285,   59,  442, 1682,  337,  493,
 1596, 1223,    0, 1225, 1680,   59,    0, 1380,  426,  503,
    0,  489,  490,  278,  492,  280, 1357,   93, 1630,    0,
 1361,  499,  500,  501,  502,  503,  456,  505,  387,  320,
  508,  509,  510,  511,  512,  318,  400,  396,  321,   59,
  409,  318,  427, 1419, 1729,  328,    0,   41,  331,  397,
   44,   41,  335,   44,   44, 1731, 1158,  340,   59,  259,
  343,   59,  397,  340, 1286,   59,  261,  460, 1221,   59,
  270, 1181,  458, 1183,   44,  397,  359,  441,   59,  461,
  462,  440, 1458, 1187, 1188,  397,  402,  862, 1773, 1199,
   44,  866,  460, 1203, 1204,  454,  489,  490, 1774,  492,
  383,    0,   93,  331,  387,   59,  499,  500,  501,  502,
  503,  397,  505,  396,    0,  508,  509,  510,  511,  512,
 1805,  489,  490,   93,  492,    0, 1824,  318,  397,  445,
  413,  499,  500,  501,  502,  503,  402,  505,  322, 1735,
  508,  509,  510,  511,  512,    0,  460, 1584, 1301,  340,
  260,    0, 1305, 1590,  861,    0, 1497,  440,  865,  387,
   59, 1265, 1392,  259,    0, 1265,   41,  436,  396,  452,
  458,  387,  438,   59,  270,  489,  490,  460,  492,  340,
  396,  279, 1412,  260,   59,  499,  500,  501,  502,  503,
 1550,  505,  508,  303,  508,  509,  510,  511,  512,    0,
  377,  470,  301,  460,   59,  342,  489,  490,  258,  492,
   59,  278,  337,  280,   59, 1591,  499,  500,  501,  502,
  503,    0,  505,   59,  301,  508,  509,  510,  511,  512,
    0,  329,  489,  490,    0,  492,  313,  322,    0,  338,
  377,  418,  499,  500,  501,  502,  503,  401,  505,  374,
 1403,  508,  509,  510,  511,  512,  306,  260,   59, 1411,
    0,  338,  387,   44,  458,  363,  460,    0,  405,   40,
   44,  408, 1382,  385,  328, 1411, 1412,  331,   59,   41,
   59, 1391,   44,  381,    0,   59, 1503,  424,  465,   59,
  254, 1420,  383,   59,  280,  489,  490,   59,  492,   61,
  303, 1411,  288, 1525,  260,  499,  500,  501,  502,  503,
 1420,  505,  276,  450,  508,  509,  510,  511,  512,   59,
 1550,  285,  342,  260,  318, 1698,   59,  458,  318,   91,
  407,  321,  460,  387,   42,   43,    0,   45,  328,   47,
  417,  331,  396,   59,  342,  335,  340,  303, 1691, 1692,
  340,   59,  316,  343,  485,   42,   43,  377,   45,    0,
   47,  489,  490, 1703,  492,  442,  303,  383,  418,  359,
 1531,  499,  500,  501,  502,  503,   44,  505, 1575,  377,
  508,  509,  510,  511,  512,  405,  436,    0,  408, 1493,
 1733,   41,  436,  383,   44,   59,  386,  387,    0, 1503,
   41, 1761, 1632,   44,  424,  417,  396,  405,   42,   43,
  408,   45,    0,   47, 1550,  348,    0,  383,   59,    0,
  470, 1531, 1576,  413,  357,  260,  424,  337,   41, 1659,
  450,   44,  444, 1793, 1794,   42,   43,   44,   45,   45,
   47,  383, 1571,   93, 1534,  435,   59, 1600,  494,  413,
  440, 1604,  450,  417,  418,  419, 1566,   59, 1818,  460,
   41, 1571,  452, 1806,  454,    0, 1576,   46,  303, 1829,
    0,   59, 1832,  318, 1671,   59, 1706,  465,   59,  467,
    0,  274,   61,  328, 1595, 1595,  331,  418,  489,  490,
   46,  492,  321,  342,    0,  340,  258,  318,  499,  500,
  501,  502,  503, 1644,  505,   61,  335,  508,  509,  510,
  511,  512,  274,  301,  343,  479,  278, 1747,  280,  483,
 1630, 1631,  284,   40,   59,  287, 1756, 1706,  377,   59,
  359, 1761,   42,   43, 1644,   45, 1392,   47,  300,   59,
   42,   43,  387,   45,  306,   47,  279,  309,  287,  272,
  338,  396,  424,   59, 1715,    0,  405, 1787,  461,  408,
 1790,  300,  264, 1793, 1794,  288,  316,  300, 1747, 1680,
 1680,   40,  280,   42,   43,  424,   45,  285,  378,  294,
   42,   43, 1812,   45,  413,   47, 1697, 1697, 1818,   58,
  305,    0,  318,  280,   63,  440,  329,  312,  285, 1829,
  400,  450, 1832, 1778, 1779, 1715, 1716,  407, 1787,    0,
  333, 1790,  337, 1717,   59,  348,  378, 1717,    0,  407,
 1731, 1731,  337,  356,  357, 1761,  460, 1738, 1738,  417,
  363, 1785,  340, 1812,  279, 1181,  336, 1183,  400,  362,
  258,  441,    0,  261,  402,  407,  280,  452,  381,  411,
   59,  285,  321,  340,  442,  300,  418, 1793, 1794, 1834,
  393,    0,  395, 1774, 1774,  278,  335,  280,   59,  316,
  317, 1782, 1782,  280,  436, 1785,  434,   59,  285,  441,
  402,  645, 1818,    0,  329,  418,  320,  402,  306,  295,
  359,  449,    0, 1829,    0,  279, 1832,    0,  662,    0,
 1810,   59,  666,  348,  466,  340,  429, 1817,  470,  673,
  443,   40,  357,    0,  678,  679,  300,  323,  363,  434,
   59,  320,  342,  438,  688, 1835,  690,  691,  692,  693,
  694,  695,  696,  493,  494,   41,  381,  318,   44,   40,
   41,  318,   59,   44,  413,  329,  352,  328,  321,  402,
  331,   59,    0,   59,  383,  265,   59,  377,   59,  340,
   61,  725,  335,   41,  348,  378,   44,  457,  458,  459,
  280,   41,   59,  357,   44,  285, 1632,   41,  280,  363,
   44,  461,  462,  285,  748,  405,  359,  400,  408,  753,
   91,  260,  302,  383,  407,  485,  486,  381,  267,   41,
  506,    0,   44, 1659,  424,   41,  387,  276,   44,  311,
  383,   59,  436,   41,   41,  396,   44,   44,  280,   41,
  289,  427,   44,  285,  391,  392,  295,  296,  441,   93,
  450,  356,   41,   41,  303,   44,   44,  443,  461,  462,
  413,   40,   41,   42,   43,   44,   45,   46,   47,  311,
 1706,  457,  458,  459,  323,  461,  462,  326,  361,  440,
   59,   60,   61,   62,   42,   43,  434,   45,  393,   47,
  395,  417,  450,  454,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  352,   41,  354,  355,   44,  321,  495,
  496, 1747,   91,  418,   93,  442,   41,  366,  504,   44,
 1756,  370,  335,   41,   41,   41,   44,   44,   44,  357,
  874,  875,  876,  287,  411,  879,  880,  881,  443,  883,
  884,  885,  886,  887,  888,  469,  359,  891,  892,   44,
  894, 1787,  896,   61, 1790,  899,  900,  901,  902,  903,
  904,  661,   41,  663,  372,   44,  321,  667,  668,  669,
  383,  286,  916,   40,  918,  264, 1812,  258,  427,   41,
  335,   40,   44,   40,  433,   40,    0,   40,  343,  461,
   40,   40,   40,  274,  443,   40,  277,  278,   40,  280,
  413,   40,  461,  284,  359,  461,  287,   40,  457,  458,
  459,  460,  461,  462,   40,   40,   40,    0,  962,  300,
   91,   44,  471,   44,   46,  306,  436,  320,  309,    0,
  402,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  320,  492,   44,   59,  495,  496,    0,  498,
  499,  500,  501,  502,  503,  504,  505,   44,  413,  508,
  509,  510,  511,  512,   41,   42,   43,  428,   45,   61,
   47,   46,   40,   44,  264,   44,   59,  382,  348,   44,
  278,  278,  300,  262,  264,  264,  265,  397,   59,   41,
  318,  436,   44,  272,  470,  397,  277,  378,  458,  447,
  447,  280,   40,  331,  262,  399,  285,   59,  349,  288,
   40,   91,  340,   40,   40,  318,  264,  383,  389,  400,
  411,   44,  280,  302,   40,  382,  407,  285,  307,  308,
  411,   40,  311,  451,   41,   42,   43,  418,   45,  318,
   47,  320,  321,   42,   43,  324,   45,    0,   47,  328,
    0,   41,  331,  464,  333,  436,  335,  436,  317,  387,
  441,  340,  341, 1107,  343,  879,   40,  881,  396,  883,
  884,  320,  886,  887,  888, 1119, 1120,  320, 1122, 1123,
  359,  360,  264,  362,  452,  466, 1130,  448,   41,  470,
   40,   41,  454,  458,   44,  502,    0,  376,    0,  378,
  379,  380,   40,  277,  383, 1149,   59,  386,  387,   59,
  274,   61,  440, 1157,   40, 1159,  300,  396, 1162,  284,
 1164,  344,  344,  386, 1168, 1169, 1170, 1171,  300,  386,
  386,  410,  278,  418,  413,  418,  356,   41,  416,   41,
   44,   91,   44,   40,  258,   41,   42,   43, 1192,   45,
  429,   47,  417,   40,  274,   59,  435,   59, 1202,   40,
  264,  440,   41,   42,   43,  279,   45,  417,   47,   41,
   44,   44,  451,  452,  280,  454,   41,   42,   43,  320,
   45,  460,   47,   44,  385,   44,  300,  367,  320,   40,
   44,  328,  306,  505,    0,   44,  279,   40, 1242,  266,
  299,  501,  264,  280,   42,   43,   44,   45,  285,   47,
  489,  490,   40,  492,   41,  329,  422,  300,   58,   41,
  499,  500,  501,  502,  503,   41,  505,   41,  264,  508,
  509,  510,  511,  512,  348,   41,   42,   43,   44,   45,
   46,   47,  356,  357,  454,  309,  329, 1291,  440,  363,
  321,  394,  492,   59,   60,   61,   62,  328,  309,   41,
  331,   41,   40,  262,  335,  348,   40,  381, 1312,  321,
  352,   40,  343,  280,  357,   40,  328, 1321,  285,  331,
  363,  280,   40,  335, 1328,   91,  285,   93,  359,   40,
   40,  343,   40,   40,   40,   40,   40,   40,  381,   40,
   40,   40,  262,  308,  418,  435,  327,  359,  258,   44,
   41,  343,  383, 1357,  388,  429,  388, 1361,  388,  274,
  331,  422,  271,  287,  274,  391,  407,  277,  278,  390,
  280,  383,  386,    0,  284,  446, 1380,  287,  386,  386,
  386,  386,  413,  386,   41,  378,   41,  418,  278,   40,
  300,   41,   41,   41,  417,   41,  306,   42,   43,  309,
   45,  413,   47,    0,  308,  318,   41,   42,   43,  440,
   45,  385,   47,  280,   41,  328,  435,   44,  331,  436,
  337,  452,  442,  454,  280,  301,  396,  340,  440,  285,
  338,  338,   59,   41,   42,   43,    0,   45,  442,   47,
  452,  280,  454,  301,   41,   41,  285,   44,   41,  352,
  320,   41,  385,  371,  318,  280,  318,  371,   41,   41,
  285,   41,   59,   41,  328,   41,  328,  331,  378,  331,
  264,   41, 1476, 1477,  387,   41,  340,   41,  340,  343,
   44,  343,  280,  396,   41,   41,   41,  285,   41,   41,
  400,   41,   41, 1497,   41,   59,  262,  407,  264,  265,
  280,  411,   46,   44,   40,   59,  272,    0,  418,   41,
   42,   43,  340,   45,  280,   47,   59,   40,  340,  285,
   59,   59,  288,  387,   59,  387,  436,  440,   59,   59,
  308,  441,  396, 1537,  396,   59,  302,   41,  304,  452,
    0,  307,  308,  344,  352,  311,   41,  340,   41,   40,
  368,   44,  318,  383,  320,  321,  466,  451,  324,   44,
  470,   59,  328,   59,   41,  331,   59,  333,   61,  335,
  383,  383,  343,   41,  340,  341,  440,  343,  440,   41,
   44,   41,   42,   43,   44,   45,   46,   47,  452,  435,
  452,  304,  301,  359,  360, 1599,  362,  442,  387,   59,
   60,   61,   62, 1607,  410,   40,   59,  293,  304,  463,
  376,   44,  378,  379,  380,  307,   44,  383,   41,    0,
  386,  387,  337,    0, 1659,  383,  383,   40,  264,  458,
  396,   91,  264,   93,  278,  280,  389,  320,  264, 1643,
  285,  411,   44, 1647,  410,  280,  470,  413,   40,  451,
  285,  278,  271,  280,  458,  309,  435,  284,  264,  436,
  287,  343,  343,  429,   41,  343,  460,   44,  343,  435,
  271, 1706,  280,  300,  440,  458,   41,  285,  308,   40,
  308,  278,   59,  280,   40,  451,  452,  284,  454,   59,
  287,   59,  308,   41,  460,  489,  490,   59,  492, 1703,
  304, 1705,   59,  300, 1708,  499,  500,  501,  502,  503,
   41,  505, 1747,   59,  508,  509,  510,  511,  512,   41,
    0, 1756,    0,  489,  490,    0,  492,    0, 1184,   12,
  264,  271, 1084,  499,  500,  501,  502,  503,  263,  505,
  706,  630,  508,  509,  510,  511,  512,  851,  280, 1082,
   87,  378, 1787,  285,  318, 1790, 1079,  321,   42,   43,
 1071,   45, 1438,   47,  328,  258,   41,  331, 1439,   44,
  356,  335,  577,  400, 1061,   59,  340, 1812,  326,  343,
  407,  378, 1009,  311,   59,  278,   61,  280,  321,  750,
 1641,  284, 1072,    0,  287,  359,  338, 1417, 1722,  784,
  556,  338,  262,  400,  264,  265, 1726,  300, 1419, 1725,
  407, 1765,  272,  306,  441, 1240,  774, 1703,    0,  383,
  280, 1664, 1649,  387, 1817,  285, 1762,  994,  288, 1217,
 1396, 1680,  396,  343,   41,   42,   43,   44,   45,   46,
   47,  331,  302,  324,  441, 1274,  558,  307,  308,  413,
  830,  311,   59,   60,   61,   62,  616,   13,  318,   41,
  320,  321,   44,  494,  324,  822,  298,  739,  328,  498,
  753,  331,  141,  333,  725,  335,  440,   59,  726, 1632,
  340,  341,  971,  343,   91,  378,   93, 1520,  452, 1782,
 1738,   41,   42,   43, 1835,   45,  509,   47,  714,  359,
  360,  963,  362, 1164,   -1, 1229, 1121,  400,   -1,   -1,
   -1,   -1,   -1,   -1,  407,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,  418,  386,  387,   -1,   -1,
   -1,   41,   42,   43,   -1,   45,  396,   47,   -1,   -1,
   -1,  318,   -1,  436,  321,   -1,   -1,   -1,  441,    0,
  410,  328,   -1,  413,  331,   41,   42,   43,  335,   45,
   -1,   47,   -1,  340,   -1,   -1,  343,   -1,   -1,  429,
   41,   42,   43,   -1,   45,  435,   47,  470,   -1,   -1,
  440,   -1,  359,  258,   -1,   -1,   -1,   -1,   -1,   -1,
   41,  451,  452,   44,  454,   -1,  280,   -1,   -1,   -1,
  460,  285,  277,  278,   -1,  280,  383,   -1,   59,  284,
  387,   -1,  287,   -1,   -1,   -1,   41,   42,   43,  396,
   45,   -1,   47,   -1,   -1,  300,   -1,   -1,   -1,  489,
  490,  306,  492,   -1,   -1,   -1,  413,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,    0,   -1,  508,  509,
  510,  511,  512,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,   -1,   -1,  440,   -1,  272,   -1,   -1,   41,   42,
   43,   -1,   45,  280,   47,  452,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   41,   42,   43,
   44,   45,   46,   47,   -1,  302,  278,   -1,  280,   -1,
  307,  308,   -1,  378,  311,   59,   60,   61,   62,   -1,
   -1,  318,   -1,  320,  321,    0,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,  400,  333,   -1,  335,   -1,
  280,   -1,  407,  340,  341,  285,  343,   91,   -1,   93,
   -1,  416,   -1,  418,   41,   42,   43,   -1,   45,   -1,
   47,   -1,  359,  360,   -1,  362,   41,   -1,   -1,   44,
   -1,  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,  376,
  280,  378,  379,  380,   59,  285,  383,   -1,   -1,  386,
  387,  472,  473,  474,  475,  476,  477,  478,  479,  396,
   -1,   -1,   -1,   -1,  280,  470,  378,   -1,  260,  285,
   -1,   -1,    0,  410,   -1,   -1,  413,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,  400,   -1,
   -1,   -1,  429,   -1,   -1,  407,   -1,  278,  435,  280,
   -1,   -1,   -1,  440,   -1,   -1,  287,   -1,   -1,  301,
   -1,   -1,   -1,   41,  451,  452,   44,  454,   -1,  300,
   -1,  313,   -1,  460,   -1,  280,   -1,   -1,   -1,  441,
  285,   59,   -1,   -1,   -1,  327,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,  338,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,    0,
   -1,  508,  509,  510,  511,  512,   -1,  280,  262,   -1,
  264,  265,  285,   -1,   -1,   -1,   -1,   -1,  272,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  280,  378,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,
   41,   42,   43,   44,   45,  407,   47,   -1,  302,  400,
   -1,   -1,   -1,  307,  308,  417,  407,  311,   59,   60,
   61,   62,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,  258,  328,   -1,    0,  331,   -1,  333,
  442,  335,   -1,  280,   -1,   -1,  340,  341,  285,  343,
  441,   -1,   93,   -1,  279,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,  300,   -1,   41,   -1,   -1,
   44,  306,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   59,   -1,  499,  500,  501,
  502,  503,  396,  505,  329,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,    0,  413,
   41,   42,   43,  348,   45,   -1,   47,   -1,   -1,   -1,
   -1,   -1,  357,   -1,   -1,  429,   -1,   -1,  363,   -1,
  278,  435,  280,   -1,   -1,    0,  440,   -1,   -1,  287,
   -1,   -1,   -1,   -1,   -1,   -1,  381,  451,  452,   41,
  454,   -1,   44,   -1,   -1,  280,  460,   -1,   -1,   -1,
  285,   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,   61,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,
   -1,    0,   -1,  418,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   59,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,   41,   42,   43,   44,   45,   -1,   47,  280,
  378,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,
  294,  302,  400,   -1,   -1,   -1,  307,  308,   -1,  407,
  311,  305,   -1,   -1,   -1,   -1,   -1,  318,  312,  320,
  321,   -1,   -1,  324,   93,   -1,   -1,  328,   -1,    0,
  331,  325,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,  441,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  278,   -1,  280,   -1,  359,  360,
   -1,  362,   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,  300,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   59,   -1,
   -1,   -1,  386,   -1,   -1,  396,   -1,   -1,   -1,  280,
   -1,   -1,   -1,   -1,  285,   59,   -1,   -1,  402,  410,
   -1,   -1,  413,   -1,   -1,   -1,  258,   -1,   -1,   -1,
   -1,   -1,    0,   -1,   -1,  419,   -1,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,  277,  278,   -1,  280,  440,
  434,   -1,  284,   -1,  438,  287,   -1,  441,   -1,   -1,
  451,  452,   -1,  454,  378,  449,   -1,   -1,  300,  460,
   -1,   -1,   -1,  278,  306,  280,   -1,   -1,   -1,   -1,
   -1,   -1,  287,   -1,  468,   -1,  400,   -1,   -1,   -1,
   -1,   59,   -1,  407,    0,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,
  501,  502,  503,  272,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,  285,  441,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   41,   42,   43,   44,   45,
   -1,   47,   -1,  302,   -1,   -1,  378,   -1,  307,  308,
   -1,   -1,  311,   59,   60,   61,   62,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,  400,  328,
   -1,   -1,  331,  378,  333,  407,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,  418,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  400,   -1,  258,   -1,   -1,
  359,  360,  407,  362,  436,   -1,   -1,   -1,   -1,  441,
   -1,   -1,   -1,   -1,  258,   -1,   -1,  376,  279,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,  279,  441,  396,  470,  300,
   -1,   -1,   -1,   -1,   -1,  306,   -1,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,   -1,  300,   -1,   -1,   -1,
   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,  329,  260,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,  329,   -1,  348,   -1,   -1,
  258,   -1,  451,  452,   -1,  454,  357,   -1,   -1,   -1,
   -1,  460,  363,   -1,  348,   -1,   -1,   -1,   -1,   -1,
  301,  279,   -1,  357,   -1,   -1,   -1,   -1,   -1,  363,
  381,   -1,  313,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  489,  490,  300,  492,   -1,   -1,   -1,  381,  306,   -1,
  499,  500,  501,  502,  503,   -1,  505,  338,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  262,  418,  264,  265,
   -1,  329,   -1,   -1,   -1,   -1,  272,   41,   42,   43,
   44,   45,   -1,   47,  418,   -1,   -1,   -1,   -1,   -1,
  348,   -1,  288,   -1,   -1,   59,   60,   61,   62,  357,
   -1,   -1,   -1,   -1,   -1,  363,  302,   -1,   -1,   -1,
   -1,  307,  308,   -1,   -1,  311,   -1,   -1,   -1,   -1,
   -1,   -1,  318,  381,  320,  321,  407,   -1,  324,   93,
   -1,   -1,  328,   -1,   -1,  331,  417,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  418,  442,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  410,  287,   -1,  413,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   -1,   41,   42,   43,
   -1,   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,  287,
   -1,   -1,   -1,   -1,   -1,   -1,   60,   61,   62,    0,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,  262,   -1,
  264,  265,   -1,  499,  500,  501,  502,  503,  272,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   43,   44,   45,   -1,   -1,   -1,  302,   -1,
   -1,   -1,   -1,  307,  308,   -1,   -1,  311,   59,   60,
   61,   62,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   93, 1313, 1314,   -1,   -1,   -1,  460, 1319,
 1320,   -1, 1322, 1323,   -1,  359,  360,   -1,  362,   -1,
 1330, 1331, 1332, 1333,   -1, 1335, 1336, 1337, 1338, 1339,
 1340,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   -1,   -1,  499,  500,  501,
  502,  503,  396,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,
   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,  451,  452,   -1,
  454,  499,  500,  501,  502,  503,  460,  505,  272,   -1,
  508,  509,  510,  511,  512,   -1,  280,   -1,   -1,   -1,
   -1,  285,   -1,   -1,  288,   -1,   -1,   -1,   -1,   -1,
   -1,    0,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,  328,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
  324,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,  333,
   -1,  272,   41,   -1,   43,   44,   45,  341,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  288,   -1,   -1,
   59,   60,   61,   62,   -1,   -1,  360,   -1,  362,   -1,
   -1,  302,   -1,   -1,   -1,   -1,  307,  308,   -1,   -1,
  311,   -1,  376,   -1,  378,  379,  380,  318,   -1,  320,
  321,   -1,   -1,  324,   93,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,  359,  360,
   -1,  362,   -1,   -1,   -1,  429,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  460,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  410,
   -1,   -1,  413,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,   -1,   42,   43,   -1,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   60,   61,   62,    0,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,  262,   -1,  264,  265,   -1,  499,  500,
  501,  502,  503,  272,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  328,   -1,   -1,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   41,   -1,   43,   44,   45,
   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,  307,  308,
   -1,   -1,  311,   59,   60,   61,   62,   -1,   -1,  318,
   -1,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,
   -1,  340,  341,   -1,  343,   -1,   -1,   93,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  402,
  359,  360,   -1,  362,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,   -1,   -1,   -1,  438,   -1,  396,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,  449,   -1,   -1,   -1,
   -1,  410,   -1,   -1,  413,   -1,   -1,  460,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  468,   -1,   -1,   -1,   -1,
  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,  451,  452,   -1,  454,  499,  500,  501,  502,
  503,  460,  505,  272,   -1,  508,  509,  510,  511,  512,
   -1,  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,  324,  262,   -1,  264,  265,
   -1,   -1,   -1,   -1,  333,   -1,  272,   41,   -1,   43,
   44,   45,  341,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  288,   -1,   -1,   59,   60,   61,   62,   -1,
   -1,  360,   -1,  362,   -1,   -1,  302,   -1,   -1,   -1,
   -1,  307,  308,   -1,   -1,  311,   -1,  376,   -1,  378,
  379,  380,  318,  343,  320,  321,   -1,   -1,  324,   93,
   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,
   -1,   -1,   -1,   -1,  340,  341,   -1,  343,  368,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,
  429,   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,
  386,  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  396,   -1,   -1,   -1,   -1,   -1,  426,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  410,   -1,   -1,  413,   41,   -1,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  429,   -1,   -1,   59,   -1,   61,  435,
  460,   -1,   -1,   -1,  440,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,  451,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,  460,   -1,   58,   -1,   91,  489,
  490,   63,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,
  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,
  510,  511,  512,  489,  490,   -1,  492,   -1,  262,   -1,
  264,  265,   -1,  499,  500,  501,  502,  503,  272,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,  305,
   -1,   -1,   -1,   -1,   -1,   -1,  312,   -1,  302,   -1,
  316,  317,   -1,  307,  308,   40,   -1,  311,   43,  325,
   45,   -1,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,
  324,  337,  434,   58,  328,   -1,   -1,  331,   63,  333,
   -1,  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  460,   -1,
   -1,   -1,   -1,   -1,   -1,  359,  360,   -1,  362,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  376,   -1,  378,  379,  380,  489,  490,  383,
  492,   -1,  386,  387,   -1,   -1,  402,  499,  500,  501,
  502,  503,  396,  505,   -1,  258,  508,  509,  510,  511,
  512,   -1,   -1,  419,   -1,   -1,  410,   -1,   -1,  413,
   -1,  274,  443,   -1,  277,  278,   -1,  280,  434,   -1,
   -1,  284,  438,   -1,  287,  429,   -1,   -1,   -1,  460,
   -1,  435,   -1,  449,   -1,  267,  440,  300,   -1,   -1,
   -1,   -1,   -1,  306,  276,   -1,  309,  451,  452,   -1,
  454,   -1,  468,   -1,   -1,   -1,  460,  289,  489,  490,
   -1,  492,   -1,  295,  296,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,  314,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,   -1,  366,   -1,   -1,  400,  370,   -1,
   -1,  276,   -1,   -1,  407,   -1,  378,   -1,  411,   -1,
   -1,   -1,   -1,   -1,  289,  418,   -1,  292,   -1,   -1,
  295,  296,   -1,   40,   -1,   42,   43,   -1,   45,   -1,
   -1,   -1,   -1,  436,   -1,   -1,   -1,   -1,  441,  314,
   -1,   58,   -1,   -1,   -1,  417,   63,   -1,  323,  421,
   -1,  326,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,  466,   -1,   -1,   -1,  470,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,  460,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,  513,  514,  515,   -1,   -1,  421,   -1,   -1,   -1,
  489,  490,  427,  492,   -1,   -1,   -1,   -1,  433,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,  443,  508,
  509,  510,  511,  512,   -1,   40,   -1,   -1,   43,   -1,
   45,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,  471,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,  354,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,   -1,   -1,
  267,    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
   -1,   -1,   -1,   -1,   -1,   -1,  303,   -1,   -1,   -1,
   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,
   59,   -1,   61,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  458,   -1,  460,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,  471,   -1,  352,   -1,  354,  355,   -1,
   -1,   -1,   91,   -1,   -1,   58,  460,   -1,   -1,  366,
   63,  489,  490,  370,  492,   -1,   -1,   -1,   -1,   -1,
  498,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,  489,  490,   -1,  492,   -1,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  427,  276,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,  314,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  354,
  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,  366,    0,   -1,   -1,  370,   -1,   -1,   -1,  258,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  274,   -1,   -1,   -1,  278,
   -1,  280,   -1,   -1,   -1,  284,   -1,   -1,  287,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,  300,   -1,   -1,  267,   -1,  421,  306,   -1,   -1,
  309,   59,  427,  276,   -1,   -1,   40,   -1,  433,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,  314,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,  378,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  400,   -1,  366,   -1,   -1,   -1,  370,  407,   -1,
   -1,   -1,  411,   40,   -1,  378,   43,   -1,   45,  418,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,  460,   -1,   -1,   63,  436,   -1,   -1,
   -1,   -1,  441,  470,  471,  472,  473,  474,  475,  476,
  477,  478,  479,  480,  481,  482,  483,  484,  421,   -1,
  487,  488,  489,  490,  427,  492,   -1,  466,  495,  496,
  433,  470,  499,  500,  501,  502,  503,  504,  505,  506,
  443,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,  262,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,   -1,
  318,  295,  296,  321,   -1,   -1,   -1,   -1,    0,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,   -1,   -1,
  314,   -1,  340,  489,  490,  343,  492,   -1,   -1,  323,
   -1,   -1,  326,  499,  500,  501,  502,  503,   -1,  505,
   -1,  359,  508,  509,  510,  511,  512,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,   -1,  260,   -1,  383,  263,   59,  386,  387,
  267,   -1,  366,   -1,   -1,   -1,  370,   -1,  396,  276,
   -1,   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,  413,   -1,   40,  295,  296,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   58,   -1,  435,   -1,   -1,
   63,   -1,  440,   -1,   -1,   -1,  323,  421,   -1,  326,
   -1,   -1,   -1,  427,  452,   -1,  454,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   58,   -1,   -1,
  262,   -1,   63,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,  318,   -1,   -1,  321,
   -1,   -1,   -1,  276,   -1,   -1,  328,   -1,   -1,  331,
   -1,   -1,   -1,  335,   -1,   -1,  289,   -1,  340,   -1,
   -1,  343,  295,  296,   -1,   -1,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
  323,   63,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  413,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,  260,
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
   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,  260,  370,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   40,  295,  296,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,   -1,   -1,  425,   63,  427,   -1,   -1,   -1,
   -1,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,  260,   -1,   -1,  263,   -1,   -1,   -1,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  276,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,  323,   63,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,  260,  370,   -1,  263,   -1,   -1,   -1,
  267,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   40,  295,  296,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   58,   -1,   44,   -1,  425,
   63,  427,   -1,   -1,   -1,   -1,  323,  433,   -1,  326,
   -1,   -1,   59,   -1,   -1,   -1,   -1,  443,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  457,  458,  459,  460,  461,  462,  354,  355,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  366,
   -1,   -1,   -1,  370,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  425,   -1,
  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,  260,   -1,   -1,
  263,   -1,   -1,   -1,  267,   -1,   -1,   40,   -1,   42,
   43,   -1,   45,  276,   -1,  262,   -1,   -1,   -1,   -1,
   -1,  261,   -1,   -1,   -1,   58,  289,   -1,   -1,   -1,
   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  282,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  290,   -1,   -1,   -1,   -1,   -1,   -1,  297,   -1,  299,
  323,  301,   -1,  326,   -1,   -1,  306,   -1,   -1,   -1,
   -1,  318,   -1,  313,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,   -1,  327,  335,   -1,
   -1,  354,  355,  340,   -1,   -1,  343,   -1,  338,   -1,
   -1,   -1,   -1,  366,   -1,   40,   -1,  370,   43,   -1,
   45,   -1,  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  425,   -1,  427,  405,  413,   -1,   -1,   -1,
  433,   -1,  412,   -1,   -1,  415,   -1,  417,  418,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  457,  458,  459,  460,  461,  462,
   -1,   -1,  442,   -1,   -1,  452,  273,  454,  471,   -1,
  277,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,  295,  492,
   -1,  298,  495,  496,  267,  498,  499,  500,  501,  502,
  503,  504,  505,  276,   -1,  508,  509,  510,  511,  512,
   -1,  491,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,  506,  507,   -1,   -1,
   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,
   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  267,  366,   -1,   -1,   -1,  370,   -1,  406,
   -1,  276,   -1,   -1,   40,   -1,   42,   43,   -1,   45,
   -1,   -1,   -1,   -1,  289,   -1,  423,   -1,   -1,   -1,
  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  480,  481,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
  497,  366,   -1,   -1,   -1,  370,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  417,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,  433,   43,
   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,  273,   -1,   58,   -1,  277,   -1,   -1,   63,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
  291,   -1,   -1,   -1,  295,   -1,  471,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  267,   -1,  508,  509,  510,  511,  512,  339,   -1,
  276,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,  289,    0,   -1,   -1,   -1,   -1,  295,
  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,
  326,   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,
  411,   -1,   -1,   59,   -1,   61,  352,   -1,  354,  355,
   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   91,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  260,   -1,   -1,  480,
  481,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  427,  276,   -1,   -1,   40,  497,  433,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,  303,
   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,  485,
  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,
  496,   -1,  498,  499,  500,  501,  502,  503,  504,  505,
  354,  355,  508,  509,  510,  511,  512,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,   -1,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   58,   -1,  258,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,
   -1,  287,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,
   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,   -1,  443,
  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  378,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,  400,   -1,   -1,   -1,   -1,   -1,
   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,   -1,   -1,
   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  267,
  466,  366,   -1,   -1,  470,  370,   -1,   -1,  276,   -1,
   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,
   -1,   -1,   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  451,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,   -1,   -1,  276,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,   -1,
   40,   41,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,
   -1,  460,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
  469,  470,  471,  472,  473,  474,  475,  476,  477,  478,
  479,  480,  481,  482,  483,  484,   -1,   -1,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,  417,   -1,
  499,  500,  501,  502,  503,  504,  505,  506,  427,  508,
  509,  510,  511,  512,  433,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,  457,  458,
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
   -1,   -1,  352,   -1,  354,  355,  276,   -1,  423,   40,
   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,
  370,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,   -1,
  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
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
   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,   -1,
  277,   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,  291,   -1,   -1,   -1,  295,   -1,
  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,  339,   -1,  276,   -1,   -1,   -1,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,  276,   -1,  423,   40,   -1,   -1,
   43,   -1,   45,   -1,  366,   -1,   -1,  289,  370,   -1,
   -1,   -1,   -1,  295,  296,   58,   59,   -1,  445,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  417,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
  497,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,  427,  508,  509,  510,  511,
  512,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  276,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,   -1,   -1,   -1,   40,   -1,   -1,   43,
   -1,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,  273,   -1,   -1,   58,  277,   -1,   -1,   -1,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,   -1,
  443,   -1,   -1,  345,  346,  347,   -1,  349,  350,  351,
   -1,  353,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,  373,   -1,  375,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,  406,  508,  509,  510,  511,  512,
   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,   -1,   -1,
   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  480,  481,
   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,
  354,  355,  276,   -1,   -1,  497,   -1,   -1,   -1,   -1,
   -1,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,   -1,
   -1,  295,  296,   -1,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   91,   -1,   -1,   -1,   -1,  433,
  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   41,   -1,
   -1,   44,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,  427,  508,  509,  510,  511,  512,  433,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,    0,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,   -1,   -1,   -1,  262,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,  272,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  280,   59,   60,   61,   62,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   91,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,  262,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   41,   42,   43,   44,   45,   46,   47,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,    0,   -1,  318,   -1,   -1,  321,  396,
   91,   -1,   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,  413,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   41,   -1,  359,   44,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   -1,  452,   -1,  454,   -1,   -1,
  383,   -1,   -1,  386,  387,    0,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  280,   41,   -1,   -1,   44,
  285,   -1,  435,  288,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,  452,
   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   -1,  262,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   41,  272,   -1,   44,  359,  360,   -1,  362,   -1,  280,
   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   59,   -1,
   -1,  376,    0,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   -1,   41,   -1,   -1,   44,   -1,   -1,
   -1,  396,   -1,    0,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   59,  324,   -1,   -1,   -1,  328,  413,   -1,
  331,   -1,  333,   41,  335,   -1,   44,   -1,   -1,  340,
  341,   -1,  343,   -1,  429,  262,   -1,   -1,   -1,   -1,
  435,   59,   -1,   -1,   41,  440,   -1,   44,  359,  360,
   -1,  362,   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,
   -1,   -1,   59,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
    0,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   -1,  328,  413,   -1,  331,   -1,   -1,  262,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,  429,   -1,
   -1,   -1,   -1,    0,  435,   -1,   -1,   -1,   -1,  440,
   -1,   41,  359,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,  318,   41,   -1,  321,   44,   -1,  396,
   -1,   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,
  335,  262,   59,   -1,   -1,  340,  413,   -1,  343,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,  262,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  383,   -1,
   -1,  386,  387,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,  396,   -1,   -1,  262,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  413,  340,
    0,   -1,  343,   -1,   -1,  262,   -1,   -1,   -1,   -1,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,  359,   -1,
  435,  328,   -1,   -1,  331,  440,   -1,   -1,  335,    0,
   -1,   -1,   -1,  340,   -1,   -1,  343,  452,   -1,  454,
  318,   41,  383,  321,   44,  386,  387,   -1,   -1,   -1,
  328,   -1,  359,  331,   -1,  396,   -1,  335,   -1,   59,
   -1,  318,  340,    0,  321,  343,   -1,   -1,   -1,   -1,
   41,  328,  413,   44,  331,   -1,  383,   -1,  335,  386,
  387,  359,   -1,  340,   -1,   -1,  343,   -1,   59,  396,
   -1,   -1,  262,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,  359,   -1,   41,  383,  413,   44,  386,  387,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,  396,   -1,
   -1,   -1,   59,   -1,   -1,  262,  383,    0,  435,  386,
  387,   -1,   -1,  440,   -1,  413,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,  452,   -1,  454,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,  413,  435,  328,   -1,
   -1,  331,  440,   -1,   -1,  335,   -1,   -1,   41,   -1,
  340,   44,   -1,  343,  452,   -1,  454,   -1,  435,   -1,
   -1,  318,   -1,  440,  321,   -1,   59,   -1,   -1,  359,
   -1,  328,   -1,   -1,  331,  452,   -1,  454,  335,   -1,
   -1,    0,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,  359,   -1,   -1,   -1,  396,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   41,  413,   -1,   44,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   59,   -1,  262,   -1,    0,  435,   -1,   -1,   -1,   -1,
  440,   41,   -1,   -1,   44,    0,  413,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   59,
   -1,  262,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,  452,   41,  454,  318,   44,
   -1,  321,   -1,   59,   -1,  262,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   59,  335,   -1,   -1,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,  359,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  383,  321,   -1,  386,  387,  359,  262,
   -1,  328,   -1,   -1,  331,   -1,  396,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  383,  413,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,  359,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,  413,   -1,   -1,  318,  383,   -1,  321,  386,
  387,   -1,  452,   -1,  454,  328,   -1,   -1,  331,  396,
   -1,   -1,  335,  262,  435,   -1,   -1,  340,   -1,  440,
  343,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,  359,   -1,   -1,   -1,
   -1,   -1,  262,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,  452,   -1,  454,   -1,  318,
   -1,   -1,  321,  396,   -1,   -1,  262,   -1,   -1,  328,
   -1,   -1,  331,   -1,   -1,   -1,  335,  262,   -1,   -1,
  413,  340,   -1,   -1,  343,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,   -1,
  359,  331,  435,   -1,   -1,  335,   -1,  440,   -1,   -1,
  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  318,   -1,  383,  321,   -1,  386,  387,  359,
   -1,   -1,  328,  318,   -1,  331,  321,  396,   -1,  335,
   -1,   -1,   -1,  328,  340,   -1,  331,  343,   -1,   -1,
  335,   -1,   -1,  383,  413,  340,  386,  387,  343,   -1,
   -1,   -1,  271,  359,   -1,   -1,  396,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  359,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,  413,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,  301,  452,   -1,  454,   -1,   -1,  383,  308,
  396,  386,  387,  312,  313,  435,  315,   -1,   -1,  318,
  440,  396,   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,
   -1,   -1,  452,  332,  454,   -1,   -1,   -1,  413,  338,
   -1,   -1,   -1,   -1,   -1,   -1,  271,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,  452,   -1,  454,   -1,
  271,   -1,   -1,   -1,   -1,   -1,  301,  452,   -1,  454,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  312,  313,   -1,
  315,   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,  398,
  301,   -1,   -1,   -1,   -1,   -1,   -1,  332,   -1,   -1,
   -1,  312,  313,  338,  315,   -1,   -1,  318,  417,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  332,   -1,  432,   -1,   -1,   -1,  338,   -1,   -1,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,   -1,  453,   -1,   -1,   -1,   -1,   -1,
   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  398,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,  417,  492,   -1,   -1,   -1,  398,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,  432,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  417,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  451,   -1,  453,   -1,
   -1,  432,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,  273,   -1,   -1,   -1,
  277,   -1,  453,   -1,   -1,   -1,   -1,  284,   -1,  460,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,  295,   -1,
   -1,  298,   -1,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  489,  490,
   -1,  492,  273,   -1,   -1,   -1,  277,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,  339,   -1,  295,   -1,   -1,  298,  345,  346,
  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,   -1,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,   -1,   -1,   -1,   -1,  406,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  373,   -1,  375,   -1,  423,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,   -1,
   -1,   -1,   -1,   -1,   -1,  406,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,  445,  492,   -1,   -1,   -1,   -1,
  497,   -1,  499,  500,  501,  502,  503,   -1,  505,  460,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,  497,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,
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
			)) 
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
					return sym->sym_keyword;
				}
			} /* else fall down and return token as SYMBOL */
		} else {
			yylval = (DSQL_NOD) sym->sym_object;
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
if ( c == '(' && !brace_analysis ) {
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
			if (prev_keyword == '(') {			
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
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 352:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_select, (int) 2, yyvsp[0], NULL); }
break;
case 354:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 356:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_select_expr, e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 358:
{ yyval = make_list (yyvsp[0]); }
break;
case 360:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 363:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 364:
{ yyval = yyvsp[-1]; }
break;
case 365:
{ lex.beginning = lex_position(); }
break;
case 366:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
			       (lex_position() == lex.end) ?
			       lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 367:
{ lex.beginning = lex.last_token; }
break;
case 368:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 369:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 370:
{ yyval = 0; }
break;
case 371:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 372:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 373:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 374:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 375:
{ yyval = NULL; }
break;
case 376:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 377:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 378:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 379:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 0, 0), CONSTANT_SLONG); }
break;
case 380:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 0, 0), CONSTANT_SLONG); }
break;
case 381:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 0, 0), CONSTANT_SLONG); }
break;
case 382:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 0), CONSTANT_SLONG); }
break;
case 383:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 0), CONSTANT_SLONG); }
break;
case 384:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 0), CONSTANT_SLONG); }
break;
case 385:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 0), CONSTANT_SLONG); }
break;
case 386:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 0), CONSTANT_SLONG); }
break;
case 387:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 0), CONSTANT_SLONG); }
break;
case 388:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 2, 3), CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (1, 3, 2), CONSTANT_SLONG); }
break;
case 390:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 1, 3), CONSTANT_SLONG); }
break;
case 391:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (2, 3, 1), CONSTANT_SLONG); }
break;
case 392:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 1, 2), CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) TRIGGER_TYPE_SUFFIX (3, 2, 1), CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 395:
{ yyval = NULL; }
break;
case 396:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 397:
{ yyval = yyvsp[0]; }
break;
case 398:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 399:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 400:
{ yyval = yyvsp[0]; }
break;
case 401:
{ yyval = yyvsp[0]; }
break;
case 402:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 403:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
                                          yyvsp[-1], make_list (yyvsp[0])); }
break;
case 404:
{ yyval = make_node (nod_mod_index, 
				     (int) e_mod_idx_count, yyvsp[0]); }
break;
case 405:
{ yyval = yyvsp[0]; }
break;
case 406:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 408:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 409:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
					    yyvsp[-1], yyvsp[0]); }
break;
case 410:
{ yyval = yyvsp[0]; }
break;
case 411:
{ yyval = yyvsp[0]; }
break;
case 412:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 413:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 414:
{ yyval = yyvsp[0]; }
break;
case 415:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 417:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 418:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 419:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = yyvsp[0]; }
break;
case 422:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 423:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 425:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 449:
{ yyval = NULL; }
break;
case 450:
{ yyval = NULL; }
break;
case 451:
{ yyval = NULL; }
break;
case 452:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
                                        	    yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 453:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 454:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 455:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 456:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 457:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 458:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 459:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = (DSQL_NOD) NULL; }
break;
case 461:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 462:
{ yyval = yyvsp[0]; }
break;
case 463:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 464:
{ yyval = yyvsp[0]; }
break;
case 465:
{ yyval = yyvsp[0]; }
break;
case 467:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 468:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 470:
{ yyval = NULL; }
break;
case 472:
{ yyval = NULL; }
break;
case 473:
{ yyval = yyvsp[0]; }
break;
case 474:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 475:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 476:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 477:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 478:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 479:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 480:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 481:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 482:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 483:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 484:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 490:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-3]; }
break;
case 491:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
		      lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
		      lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
		      yyval = yyvsp[-4]; }
break;
case 493:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 494:
{ if ((SLONG) yyvsp[0] < 1)
		     		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
		          else
		     		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 495:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 501:
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
case 502:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 503:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 504:
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
case 505:
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
case 506:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 509:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 510:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 511:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 512:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) yyvsp[-1];
			}
break;
case 513:
{
			lex.g_field->fld_seg_length = (USHORT) yyvsp[0];
		  	}
break;
case 514:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 515:
{
			lex.g_field->fld_sub_type = (USHORT) yyvsp[0];
			}
break;
case 516:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 517:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 518:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 520:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 521:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 522:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 523:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 524:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT) yyvsp[-1]; 
			}
break;
case 534:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 535:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 536:
{ yyval = make_node (nod_position, 1, yyvsp[0]); }
break;
case 537:
{
			lex.g_field->fld_dtype = dtype_long; 
		    	lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
		    	}
break;
case 538:
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
case 539:
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
case 542:
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
case 543:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 546:
{ yyval = yyvsp[-1]; }
break;
case 547:
{ yyval = 0; }
break;
case 551:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 552:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
				       MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 553:
{
			  yyval = make_node (nod_set_generator2, e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
						    MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 557:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 558:
{ yyval = make_node (nod_release_savepoint, 1, yyvsp[0]); }
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
{ yyval = make_node (nod_order, e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 617:
{ yyval = 0; }
break;
case 618:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 619:
{ yyval = 0; }
break;
case 620:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 621:
{ yyval = 0; }
break;
case 622:
{ yyval = yyvsp[-1]; }
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
{ lex.limit_clause = true; }
break;
case 633:
{ lex.limit_clause = false; }
break;
case 634:
{ lex.first_detection = true; }
break;
case 635:
{ lex.first_detection = false; }
break;
case 636:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 637:
{ yyval = make_node (nod_limit, e_limit_count, NULL, yyvsp[-1]); }
break;
case 638:
{ yyval = make_node (nod_limit, e_limit_count, yyvsp[0], NULL); }
break;
case 639:
{ yyval = 0; }
break;
case 640:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 641:
{ yyval = yyvsp[-2]; }
break;
case 642:
{ yyval = yyvsp[-1]; }
break;
case 643:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 644:
{ yyval = yyvsp[-1]; }
break;
case 645:
{ yyval = yyvsp[0]; }
break;
case 646:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 647:
{ yyval = 0; }
break;
case 648:
{ yyval = make_list (yyvsp[0]); }
break;
case 649:
{ yyval = 0; }
break;
case 651:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 653:
{ yyval = make_node (nod_alias, 2, yyvsp[-1], yyvsp[0]); }
break;
case 654:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 655:
{ yyval = make_list (yyvsp[0]); }
break;
case 657:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 660:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 661:
{ yyval = yyvsp[-1]; }
break;
case 662:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-2], yyvsp[0], yyvsp[-1]); }
break;
case 663:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 664:
{ yyval = make_list (yyvsp[-1]); }
break;
case 665:
{ yyval = NULL; }
break;
case 667:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 671:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 672:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 673:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 674:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 675:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 676:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 677:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 678:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 679:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 680:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 681:
{ yyval = make_list (yyvsp[0]); }
break;
case 682:
{ yyval = 0; }
break;
case 684:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 689:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 693:
{ yyval = yyvsp[0]; }
break;
case 694:
{ yyval = 0; }
break;
case 695:
{ yyval = yyvsp[0]; }
break;
case 696:
{ yyval = 0; }
break;
case 697:
{ yyval = yyvsp[0]; }
break;
case 698:
{ yyval = 0; }
break;
case 699:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 700:
{ yyval = 0; }
break;
case 701:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 702:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 703:
{ yyval = 0; }
break;
case 704:
{ yyval = 0; }
break;
case 706:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 707:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 710:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 711:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 712:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 713:
{ yyval = make_node (nod_index_order, 1, yyvsp[0]); }
break;
case 715:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 716:
{ yyval = make_node (nod_insert, e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 717:
{ yyval = make_node (nod_insert, e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 719:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 722:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 723:
{ yyval = make_node (nod_delete, e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 724:
{ yyval = make_node (nod_cursor, e_cur_count, yyvsp[0], NULL, NULL); }
break;
case 727:
{ yyval = make_node (nod_update, e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 728:
{ yyval = make_node (nod_update, e_upd_count,
			  	yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 730:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 734:
{ yyval = make_node (nod_get_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 735:
{ yyval = make_node (nod_put_segment, e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 737:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 741:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 742:
{ yyval = yyvsp[0]; }
break;
case 744:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 747:
{ yyval = NULL; }
break;
case 748:
{ yyval = make_list (yyvsp[-1]); }
break;
case 750:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 752:
{ yyval = NULL; }
break;
case 753:
{ yyval = make_list (yyvsp[-1]); }
break;
case 755:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 758:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 759:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 761:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 763:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 765:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 766:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 768:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 769:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 770:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 771:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 772:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 774:
{ yyval = yyvsp[-1]; }
break;
case 775:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 787:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 790:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 795:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 796:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 797:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 798:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 799:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 800:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 801:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 802:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 803:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 804:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 805:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 806:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 807:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 808:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 809:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 812:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 814:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 816:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 818:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 819:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 820:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 821:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 822:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 823:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 824:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 825:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 826:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 829:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 830:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 831:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 832:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 833:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 834:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 835:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 837:
{ yyval = make_list (yyvsp[-1]); }
break;
case 838:
{ yyval = yyvsp[-1]; }
break;
case 839:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 840:
{ yyval = make_node (nod_select_expr, e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 850:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 851:
{ yyval = yyvsp[0]; }
break;
case 852:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 853:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 854:
{ yyval = make_node (nod_collate, e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 855:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
			      yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 856:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			       yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
			       yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 857:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
			      yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 858:
{ yyval = yyvsp[-1]; }
break;
case 859:
{ yyval = yyvsp[-1]; }
break;
case 863:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 864:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 865:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
                        }
break;
case 866:
{ yyval = yyvsp[0]; }
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
			yyval = make_node (nod_current_date, 0, NULL);
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
			yyval = make_node (nod_current_time, 0, NULL);
			}
break;
case 869:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 870:
{ yyval = make_node (nod_array, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 872:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 874:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 875:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 876:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 877:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 878:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 879:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 881:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 882:
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
case 883:
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
case 884:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 885:
{ yyval = make_parameter (); }
break;
case 886:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 887:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 888:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 889:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 890:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 891:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 892:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 893:
{ yyval = make_node (nod_internal_info, e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 894:
{ yyval = yyvsp[0]; }
break;
case 895:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 897:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 898:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 899:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
			    yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 900:
{ if ((SLONG) yyvsp[0] == 0)
			    yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 901:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
			    yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 903:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 904:
{ yyval = yyvsp[0];}
break;
case 909:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 910:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 911:
{ yyval = make_flag_node (nod_agg_count,
                                       NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 912:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 913:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 914:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
			      yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 915:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
			      yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
			      yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 916:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 917:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 918:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 919:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 920:
{ 
				  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				      yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
				  else
					  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
				}
break;
case 921:
{ yyval = make_node (nod_extract, e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 922:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-3],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-1]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG)); }
break;
case 923:
{ yyval = make_node (nod_substr, e_substr_count, yyvsp[-5],
					MAKE_constant ((STR) ((SLONG)(yyvsp[-3]) - 1), CONSTANT_SLONG),
					MAKE_constant ((STR) (yyvsp[-1]), CONSTANT_SLONG)); }
break;
case 924:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 925:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 926:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 927:
{ yyval = make_node (nod_cast, e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 930:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 931:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 934:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 935:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 936:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 937:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 938:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 939:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 940:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 941:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 945:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 946:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 947:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 948:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 949:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 950:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 951:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 952:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 955:
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
