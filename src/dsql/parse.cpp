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
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL.
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

static void	yyerror (TEXT *);

/* since UNIX isn't standard, we have to define
   stuff which is in <limits.h> (which isn't available
   on all UNIXes... */

const long SHRT_POS_MAX			= 32767;
const long SHRT_UNSIGNED_MAX	= 65535;
const long SHRT_NEG_MAX			= 32768;
const long LONG_POS_MAX			= 2147483647;
const int POSITIVE	= 0;
const int NEGATIVE	= 1;
const int UNSIGNED	= 2;

const int MIN_CACHE_BUFFERS	= 250;
const int DEF_CACHE_BUFFERS	= 1000;

/* Fix 69th procedure problem - solution from Oleg Loa */
#define YYSTACKSIZE	2048
#define YYMAXDEPTH	2048

#define YYSTYPE		DSQL_NOD
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME [] = "DSQL internal"; /* NTX: placeholder */
static const char NULL_STRING [] = "";	

inline SLONG trigger_type_suffix(const int slot1, const int slot2, const int slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


DSQL_NOD		DSQL_parse;

static void	yyerror (TEXT *);

#define YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM USHORT client_dialect, USHORT db_dialect, USHORT parser_version, BOOLEAN *stmt_ambiguous

#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

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

inline void check_bound(const char* const to, const char* const string)
{
	if ((to - string) >= MAX_TOKEN_LEN)
		yyabandon (-104, isc_token_too_long);
}

inline void check_copy_incr(char*& to, const char ch, const char* const string)
{
	check_bound(to, string); 
	*to++ = ch;
}

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
static LexerState lex;

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
#define OPEN 518
#define CLOSE 519
#define FETCH 520
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
  173,  177,  177,  178,  174,  174,  179,  179,  180,  182,
  182,  183,  181,  181,  185,  185,  184,  188,  188,  175,
  190,  190,  190,  191,  191,  189,  189,  189,  189,  189,
  189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
  189,  189,  189,  189,  194,  194,  195,  196,  197,  209,
  209,  198,  203,  204,  199,  199,  200,  213,  201,  214,
  207,  207,  207,  208,  208,  208,  211,  211,  211,  211,
  202,  210,  210,  205,  205,  205,  212,  212,  192,  192,
  219,  220,  220,  221,  221,  221,  221,  206,  206,  206,
  223,  225,  224,  226,   11,   71,   80,  227,  230,  230,
  230,  231,  234,  238,  238,  239,  239,  240,  240,  147,
  229,   87,   89,  228,  228,   70,   83,  243,  243,  243,
  244,  247,  247,  248,  248,  248,  248,  248,  248,  248,
  248,  248,  248,  248,  248,  248,  248,  248,  245,  245,
  246,    2,  250,  250,  250,  250,  250,  250,  250,  258,
  259,  256,  256,  260,  260,  260,  260,  260,  260,  260,
  251,  251,  261,  261,  261,  261,  261,  261,  261,  255,
  266,  266,  266,  266,  266,  266,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  266,  266,  266,  266,
  266,  266,  266,  266,  266,  266,  263,  263,  265,  265,
  264,  262,  262,  262,  257,  257,  253,  254,  254,  268,
  268,  268,  268,  268,  268,  268,  268,  269,  269,  252,
  270,  270,  271,  271,    8,  272,  272,  272,  272,  272,
  272,  272,  272,  272,  272,  272,  272,  101,  101,  144,
  144,  275,  275,  278,  278,  280,  280,   53,   53,  277,
  277,  277,  277,  277,  277,  277,  277,  277,  285,  285,
  276,  276,  276,  276,  286,  286,   61,   61,   61,   55,
   55,  282,  282,  282,  279,  279,  279,  290,  290,  290,
  289,  289,  288,  288,  288,  283,  283,  291,  291,  291,
  292,  292,  284,  284,  284,  284,  293,  293,   18,   18,
   18,  295,  295,  295,   16,   16,   16,  297,  298,  301,
  301,  299,  303,  303,    4,   15,  302,  302,  304,  304,
  305,  305,  294,  306,  306,  307,  307,  308,  308,  308,
  308,  309,  309,  310,  310,  311,  311,  313,  313,  313,
  314,  314,  314,  315,  315,  315,  312,  317,  317,  317,
  318,  318,  316,  316,  319,  321,  321,  320,  320,  296,
   17,  322,  322,  322,  323,  323,  327,  327,  328,   65,
   65,   65,  330,  330,  329,  329,  324,  324,  333,  333,
  325,  325,  326,  187,  337,  338,  331,  332,  334,  334,
  334,  334,  339,  339,  339,  340,  340,  340,  232,  232,
  233,  233,  343,  343,  344,  344,  346,  346,  335,  348,
  348,  349,  349,  349,  352,  353,  353,  354,  354,  356,
  356,  350,  350,  351,  351,  357,  357,  241,  241,   31,
  242,  242,  242,  242,  242,  242,  242,  242,  236,  236,
  358,  358,  359,  237,  237,  235,  235,  336,  336,  360,
  361,  361,  361,  361,  361,  362,  362,  363,  363,  364,
  364,  365,  365,  365,  366,  366,  367,  367,   10,   10,
    7,    7,  369,  370,  371,   19,   19,  372,  373,  374,
  374,  193,  345,  345,  216,  216,    3,    3,  376,  376,
  376,  378,  378,   61,  377,  377,  379,  379,   34,   34,
   85,   84,   84,  368,  368,  380,  381,  381,  217,  217,
  217,  146,  375,  375,  111,  111,  111,  111,  111,  385,
  385,  385,  385,  385,  385,  384,  384,  384,  387,  387,
  387,  387,  387,  387,  387,  387,  387,  387,  388,  388,
  388,  388,  388,  388,  388,  388,  393,  393,  393,  393,
  393,  393,  393,  393,  393,  393,  393,  393,  393,  393,
  393,  393,  399,  399,  389,  389,  390,  390,  390,  390,
  391,  391,  395,  395,  396,  396,  396,  396,  394,  397,
  392,  392,  383,  383,  383,  386,  386,  386,  400,  400,
  401,  398,  403,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,   88,  156,
  156,  156,  404,  402,  402,  151,  151,  410,  410,  410,
  410,  410,  406,  406,  406,  406,  406,  341,  152,  152,
  153,  154,  154,  154,  154,  154,   51,   51,   20,   20,
  249,  411,   54,  123,  281,  281,   95,  405,  405,  405,
  405,  412,  412,  412,  412,  412,  412,  412,  412,  412,
  412,  412,  413,  414,  416,  415,  415,  418,  419,  419,
  409,  409,  407,  408,  408,  420,  420,  421,  421,  422,
  422,  425,  425,  423,  423,  428,  428,  427,  424,  426,
  417,  417,  417,  417,  417,  417,  417,  417,  342,  342,
  155,   48,  287,  122,  148,  382,  165,  186,  274,   63,
   60,  222,  112,   66,  347,  218,   35,   41,  355,  273,
   39,   42,  215,   40,  300,  267,  267,  429,  429,  429,
  429,  429,  429,  429,  429,  429,  429,  429,  429,  429,
  429,  429,  429,
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
    0,    1,    3,    2,    1,    0,    1,    2,    4,    1,
    1,    3,    1,    0,    2,    1,    6,    1,    1,    3,
    1,    2,    0,    1,    2,    2,    2,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    2,    1,    1,    1,
    2,    2,    1,    2,    3,    4,    2,    6,    4,    1,
    1,    8,    9,    6,    8,    6,    4,    0,    4,    2,
    1,    3,    0,    2,    4,    0,    1,    1,    3,    3,
    7,    2,    0,    2,    2,    3,    3,    0,    1,    2,
    4,    1,    3,    2,    2,    2,    1,    1,    1,    1,
    2,    2,    5,    0,    4,    7,    7,    1,    1,    3,
    4,    7,    2,    1,    3,    1,    1,    6,    3,    0,
    0,    0,    0,    3,    0,    9,    9,    1,    1,    0,
    2,    1,    1,    1,    1,    1,    3,    3,    3,    3,
    3,    3,    5,    5,    5,    5,    5,    5,    2,    0,
    4,    2,    3,    3,    2,    2,    3,    3,    2,    3,
    6,    1,    2,    4,    3,    2,    2,    2,    2,    3,
    1,    3,    3,    3,    2,    2,    4,    4,    5,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    2,    2,
    1,    1,    1,    0,    2,    2,    0,    1,    2,    2,
    2,    2,    2,    4,    3,    2,    2,    1,    3,    7,
    1,    0,    1,    0,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    3,    2,    2,    2,    1,    1,    1,
    1,    4,    5,    1,    3,    1,    3,    1,    2,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    4,    4,    6,    5,    3,    0,    2,    2,    0,    3,
    0,    4,    1,    5,    4,    1,    4,    1,    2,    2,
    1,    1,    1,    2,    2,    2,    2,    0,    3,    5,
    1,    1,    2,    3,    1,    2,    3,    0,    1,    1,
    1,    5,    5,    6,    1,    1,    1,    2,    4,    1,
    0,    5,    1,    0,    3,    2,    1,    0,    2,    0,
    1,    0,    3,    1,    0,    1,    2,    1,    1,    1,
    1,    2,    2,    1,    2,    3,    1,    1,    3,    3,
    1,    2,    3,    1,    2,    0,    2,    1,    1,    0,
    1,    1,    1,    3,    2,    3,    0,    1,    3,    4,
    4,    1,    3,    4,    3,    0,    1,    3,    3,    1,
    1,    0,    1,    1,    4,    0,    3,    0,    2,    0,
    2,    0,    9,   10,    0,    0,    0,    0,    3,    2,
    1,    0,    3,    5,    3,    2,    5,    2,    1,    1,
    1,    1,    1,    3,    1,    3,    1,    0,    2,    1,
    3,    1,    1,    1,    6,    1,    0,    3,    0,    1,
    3,    6,    3,    4,    2,    3,    0,    1,    2,    1,
    1,    1,    2,    1,    2,    1,    2,    0,    3,    0,
    1,    3,    1,    2,    0,    2,    0,    2,    0,    4,
    1,    2,    1,    1,    0,    1,    3,    2,    1,    1,
    2,    1,    4,    3,    1,    3,    4,    0,    8,    5,
    1,    1,    4,    4,    4,    1,    1,    5,    5,    1,
    3,    3,    1,    1,    1,    3,    7,    7,    5,    3,
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
   12,   13,   14,   15,   16,   17,   18,   19,   20,  569,
  570,  571,  575,  576,  577,    0,  632,  741,  742,  746,
  747,  477,    0,    0,    0,    0,    0,    0,  412,  587,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  114,    0,    0,   95,    0,  921,    0,   21,  919,    0,
    0,   73,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  495,    0,    0,   37,   34,
   36,    0,   35,    0, 1006, 1008, 1010, 1009, 1015, 1011,
 1018, 1016, 1012, 1014, 1017, 1013, 1019, 1020, 1021, 1022,
 1023,    0,    0,    0,    0,   32,   68,   66,  998, 1007,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  661,    0,    0,    0,  698,    0, 1000,
    0,    0,    0,    0,  107, 1005,  578,    0,    2,    0,
    0,    0,    0,  442,  443,  444,  445,  446,  447,  448,
  449,  450,  451,  452,  453,  454,  455,  456,  457,  458,
  459,  460,  461,  462,  463,  464,  465,  466,    0,  440,
  441,    0,  990,    0,  419,  994,    0,  416,  997,    0,
  700,    0,  415, 1001,    0,  585,  917,    0,  150,  103,
    0,  104,    0,  218,  986,  782,    0,  102,  145,  993,
    0,    0,   98,    0,  105,  923,    0,   99,    0,  100,
    0,  101, 1004,  146,  106,  640,  641,    0,  922,  920,
    0,   74,    0,  991,    0,  989,  503,  496,    0,  502,
  507,  497,  498,  505,  499,  500,  501,  506,    0,   30,
    0,   39,  769,   38,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  927,
    0,  908,  655,  655,  656,  666,  668,  979,  669,    0,
  670,  660,  656,    0,    0,    0,    0,    0,    0,    0,
  604,  593,    0,  596,  598,  599,  600,  601,  607,  608,
    0,  999,  699,    0,  108,    0,  109,    0,  110,    0,
    0,    0,  633,    0,    0,    0,    0,    0,    0,    0,
    0,  478,    0,    0,  382,    0,  221,    0,  422,  413,
  475,  476,    0,    0,    0,    0,    0,    0,  431,    0,
  388,  389,    0,  591,  589,  918,    0,    0,    0,    0,
    0,    0,    0,  153,  132,    0,   96,    0,    0,  111,
    0,  121,  120,    0,    0,    0,    0,    0,    0,  982,
   75,    0,    0,    0,  743,  744,  504,    0,    0,    0,
    0,  886,  914,    0,  981,    0,  888,    0,    0,  915,
    0,  909,  900,  899,  898,    0,    0,    0,    0,  890,
  891,  892,  901,  902,  910,  911,    0,  912,  913,    0,
    0,    0,  916,    0,    0,    0,    0,    0,  904,    0,
  779,  883,  884,  885,  754,  889,  365,  869,    0,    0,
    0,  868,  755,    0,  865,  866,  867,  870,  871,  872,
  903,  928,  929,  930,  931,  944,  946,  954,  955,  958,
  959,    0,  772,    0,    0,   27,    0,    0,    0,   71,
   69, 1002,   67,   33,    0,    0,    0,  774,    0,   52,
    0,    0,    0,    0,    0,  583,    0,    0,    0,  663,
  665,    0,  672,    0,    0,  673,    0,  659,    0,  630,
    0,  605,    0,  602,    0,  603,  628,    0,  623,    0,
    0,  597,  783,  750,    0,    0,    0,    0,    0,    0,
  580,  579,    0,    0,  637,  634,    0,    0,  631,  186,
    0,    0,    0,  129,  483,  173,  174,    0,  486,  481,
    0,  487,    0,    0,    0,    0,  488,    0,  479,    0,
    0,  426,  428,  427,    0,  429,    0,  423,  219,    0,
    0,  272,    0,    0,    0,    0,  435,  436,    0,  468,
    0,    0,    0,    0,    0,    0,    0,  393,  392,  491,
    0,    0,  149,    0,    0,  158,    0,  157,    0,    0,
  164,  147,    0,  162,  165,  166,  154,    0,    0,  526,
  561,  529,    0,  562,    0,    0,  530,    0,    0,    0,
  553,  565,  525,  548,  527,  528,  523,  510,  382,  508,
  509,  511,    0,    0,  520,  521,  522,  524,    0,    0,
    0,    0,    0,  112,    0,  113,    0,  124,    0,    0,
  202,    0,    0,  380,    0,  221,    0,    0,   83,    0,
    0,  764,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  785,  787,  796,  799,  800,  801,  802,
  803,  804,  805,  806,  807,  808,    0,    0,    0,  905,
    0,    0,    0,    0,    0,    0,  906,  907,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  330, 1003,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  771,   28,    0,    0,   44,   72,
    0,    0,   26,    0,  777,    0,    0,    0,  740,    0,
    0,    0,    0,    0,  582,    0,  655,    0,    0,    0,
    0,  677,    0,  573,    0,  925,  572,    0,  606,    0,
  614,  610,  609,    0,    0,    0,  625,  613,    0,    0,
  748,  749,    0,    0,    0,  380,    0,    0,    0,  647,
  651,    0,    0,    0,  130,    0,  485,    0,    0,    0,
    0,    0,  382,  425,    0,  382,  383,  430,  518,    0,
  274,    0,  268,  270,  380,  241,  987,    0,    0,  382,
    0,    0,  210,  380,    0,    0,    0,    0,  246,  240,
  243,  244,  245,  434,  473,  472,  433,  432,    0,    0,
    0,    0,    0,    0,  391,    0,  382,    0,  155,  159,
    0,    0,  163,    0,    0,  549,  550,  566,    0,  563,
    0,    0,  556,  554,  555,    0,    0,    0,    0,  519,
    0,    0,    0,    0,  557,    0,    0,  380,    0,  201,
    0,  204,  206,  207,    0,    0,    0,    0,    0,  221,
   78,    0,    0,   77,  537,  983,  538,    0,    0,    0,
    0,  786,  798,    0,    0,  856,  857,  858,    0,  790,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  971,  972,  973,  974,  975,  976,  977,
  978,    0,    0,    0,    0,    0,    0,  964,    0,    0,
    0,    0,  881,  332,  882,  952,    0,    0,  877,  985,
    0,    0,    0,    0,    0,  756,    0,  887,  781,  780,
  773,    0,    0,    0,    0,    0,    0,    0,    0,   53,
   61,   63,    0,    0,   70,    0,    0,    0,  776,    0,
    0,    0,    0,    0,    0,    0,    0,  664,  667,    0,
    0,    0,    0,  682,  683,  684,    0,    0,  674,  995,
  676,  574,  926,  615,  624,  619,  618,    0,  629,  784,
  751,  752,  380,    0,    0,  657,  639,  638,    0,  182,
  176,  183,    0,  179,  484,    0,    0,  178,    0,  189,
  170,  169,  924,  172,  171,  489,    0,    0,    0,  424,
  273,    0,  212,  382,  214,    0,  383,  209,  215,    0,
    0,    0,    0,  437,  438,  382,  380,  383,    0,    0,
    0,  409,    0,  197,  198,  156,    0,  168,    0,    0,
    0,    0,    0,  564,    0,    0,  382,  383,    0,  514,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  199,    0,    0,    0,    0,  369,    0,
   82,    0,    0,    0,    0,   87,   84,    0,    0,    0,
  745,  988,    0,    0,  791,    0,    0,  797,    0,    0,
    0,    0,    0,  834,  833,    0,    0,    0,  841,  859,
  851,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  789,    0,    0,    0,  221,    0,  932,    0,    0,
  947,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  960,    0,  970,    0,    0,    0,    0,
    0,    0,  951,  893,   65,   57,   58,   64,   59,   60,
    0,    0,   22,    0,    0,    0,   42,    0,    0,    0,
  758,  778,    0,    0,  757,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  701,    0,    0,    0,
    0,    0,  621,  622,  626,    0,  200,    0,    0,    0,
    0,    0,    0,    0,    0,  190,    0,    0,    0,  420,
  224,  225,  226,  227,  228,  229,  896,    0,    0,    0,
  277,    0,  222,    0,    0,    0,    0,  247,    0,  469,
  470,  439,    0,    0,    0,    0,    0,    0,  382,  493,
  383,  167,  984,    0,    0,  532,    0,  531,  567,    0,
  559,    0,    0,    0,  512,    0,  540,    0,    0,  542,
  545,  547,  380,    0,    0,    0,  119,    0,  205,  382,
    0,    0,  381,    0,   97,    0,  117,    0,   93,    0,
    0,    0,    0,   86,    0,  849,  850,  793,    0,  795,
    0,    0,    0,    0,    0,    0,    0,  852,    0,    0,
    0,    0,    0,    0,    0,  842,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  938,  937,    0,    0,  934,  933,    0,  942,  941,  940,
  939,  936,  935,    0,    0,  966,    0,    0,    0,  965,
    0,    0,    0,    0,    0,    0,   56,   62,   54,   55,
   23,  762,  763,    0,  760,  767,  768,  765,    0,  739,
    0,    0,    0,  693,    0,    0,    0,  707,  703,  705,
    0,    0,    0,    0,    0,  381,  643,  644,  658,    0,
  184,  180,    0,    0,  195,  191,    0,  383,  897,  283,
    0,    0,  383,  278,    0,    0,    0,  232,    0,    0,
  248,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  490,  534,    0,  535,    0,  134,    0,    0,    0,  138,
  140,  141,  142,  515,  517,  513,  544,    0,    0,  383,
    0,    0,    0,    0,  366,    0,  370,  382,   88,    0,
    0,   90,    0,    0,    0,    0,    0,    0,  861,  860,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  220,  953,
  943,  945,    0,    0,  962,  961,    0,  967,  956,  957,
    0,   40,    0,    0,    0,  696,  694,    0,    0,    0,
  711,    0,    0,  653,  383,  367,  645,    0,  175,  192,
  383,  144,    0,    0,  280,  281,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  364,
    0,    0,    0,    0,  289,  294,  288,    0,    0,    0,
  298,  299,  300,  301,  302,  303,  305,  306,  308,  309,
  310,  313,    0,    0,    0,  358,  359,  360,    0,  267,
    0,    0,  208,  233,    0,    0,    0,    0,  234,  237,
    0,    0,  383,  407,  408,  404,  403,  406,  405,    0,
  533,  560,  143,  131,  139,    0,  382,  264,  126,  383,
    0,    0,  384,  371,    0,    0,   91,   92,   79,    0,
  820,  828,    0,  821,  829,  824,  832,    0,    0,  822,
  830,  823,  831,  817,  825,  818,  826,  819,  827,    0,
  948,  963,    0,  759,    0,    0,  686,    0,    0,  721,
  723,    0,  718,    0,  265,  185,  421,    0,  279,    0,
  317,    0,    0,  320,  321,    0,  312,    0,    0,  311,
  344,  345,    0,  996,  361,  362,    0,  297,  304,    0,
  307,  290,    0,  295,    0,  349,  296,  314,    0,    0,
  342,  211,  216,  235,    0,    0,  238,    0,  250,  252,
  411,  383,    0,  386,    0,    0,    0,  376,  377,    0,
    0,   76,   94,    0,    0,    0,    0,  685,    0,  712,
  722,    0,    0,  286,  282,    0,  315,    0,    0,    0,
    0,    0,  346,    0,    0,  337,  338,    0,  357,    0,
    0,    0,    0,  352,  350,    0,    0,    0,  239,    0,
    0,    0,    0,    0,  266,  383,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  690,    0,    0,  729,    0,
    0,    0,  285,    0,  316,    0,    0,  319,    0,  327,
    0,  329,    0,    0,  356,  355,  992,  354,    0,    0,
    0,    0,    0,    0,    0,    0,  249,    0,  256,    0,
  255,  387,  379,    0,    0,  372,  118,    0,    0,    0,
    0,  688,  731,  720,    0,    0,  732,    0,  728,    0,
    0,    0,    0,    0,    0,  340,  339,  351,  353,    0,
    0,    0,  236,    0,    0,  260,  259,  258,    0,    0,
  863,  654,  691,  727,    0,    0,  287,    0,    0,  318,
  324,    0,    0,    0,    0,    0,  263,  261,  262,    0,
    0,    0,    0,    0,  734,    0,    0,    0,    0,    0,
  341,    0,    0,    0,  733,    0,  335,  325,    0,  347,
  322,  862,  736,    0,  323,  737,
};
static short yydgoto[] = {                                      20,
   21,   22,   23,   24,   25,   26, 1561,   28,   29, 1562,
   31,   32,   33,   34,   35,   36, 1563,   38, 1564,  652,
  122,  465,  968, 1203,  123,  718,  969,  124,  469,  723,
  148,  125,  126,  262,  991,  138,  970,  971,  202,  231,
  127,  972,  128,  471,   82,  242,  381,  428,  647, 1104,
  429,  873,  618,  224,  850,  648,  649,  650, 1106,  243,
 1392,   74,  192,   75,  238, 1872, 1315,  223,  228,  230,
  232,  218,  210,  212,  225,  235,  155,  315,  317,  319,
  370,  634,  636,  462,  263,  799,  551,  662, 1040,  374,
  639, 1092, 1307,  584,  746, 1066,  533,  534,  566,  366,
  619, 1078, 1458, 1593, 1250, 1459, 1460, 1461, 1462, 1463,
  663,  219,  211,  362,  592,  363,  364,  593,  594,  595,
  596, 1282, 1034,  536,  537, 1023, 1021,  538, 1028, 1022,
 1240, 1421, 1029, 1030, 1245,  376,  861,  862,  863,  864,
  801, 1047, 1436,  620,  803,  431,  866,  949,  560, 1364,
 1251,  432,  433,  434,  435,  436, 1437, 1438,  569, 1599,
 1762, 1268,  809,  810,  796,  811,  812,  813, 1763, 1764,
 1847,  344,  564, 1259, 1565,  198,  561,  562, 1260, 1261,
 1431, 1544, 1545, 1546, 1735, 1111,  729, 1566, 1567, 1568,
 1569, 1695, 1570, 1571, 1572, 1573, 1574, 1575, 1576, 1577,
 1578, 1579, 1580, 1581, 1582, 1583,  437, 1832, 1676, 1584,
 1745, 1865, 1742,  438,  702,  439,  440, 1585, 1696, 1753,
 1754, 1796, 1586, 1587, 1588, 1687, 1097, 1313, 1475, 1098,
 1099,  290,  494, 1622,  385, 1232, 1414, 1716, 1717, 1718,
 1719, 1230,  353,  580,  827, 1280,  581,  825,   79,   59,
  348,  203,  163,  331,  189,  338,  195,  787,  784,  339,
  349,  817,  350,  576, 1058,  190,  441,  332,  548,  582,
 1281,   96,  201,  247,  621,  622,  789, 1079,  790, 1080,
 1081,  625,  626,  627,  628, 1072,  877,  629,  630,  631,
  843,  632,  840,   40,   41,   42,   43,   44,   45,  157,
  522,   61,  487,  206,  355,  302,  303,  304,  305,  306,
  307,  308,  309,  310,  752,  508, 1008, 1235,  509,  510,
  757,   46,  162,  325,  529,   47,  524,  525, 1017, 1419,
 1239, 1537,  770,  142,  740, 1534,  490,  292,  143,  144,
  442,  291,  495,  496,  443,  743, 1001,  992,  993,  994,
  995,  996, 1656, 1728,  444, 1777, 1224, 1530, 1531, 1779,
 1664, 1780, 1781, 1782, 1829, 1873, 1875,  477,   48,   49,
  386,   50,   51,  516,  517,  977, 1211, 1394, 1398,  478,
  726,  216,  664,  665,  891,  892,  666,  667,  668,  669,
  670,  671,  672,  673,  674,  675,  676, 1336, 1127, 1129,
 1130,  948,  701,  445,  446,  447,  448,  449,  450,  451,
  240,  452,  453,  454,  455,  456,  932,  457, 1514,  458,
  459,  460,  461,  692,  936, 1187, 1182,  693,  130,
};
static short yysindex[] = {                                   6356,
 2551, -102, 3726,   88, 1374,   37, 2784,  -25, 4767,  581,
  124, 9440, -102, 1295,  744, 5357,  897, 5357,  -97,    0,
  393,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  476,    0,    0,    0,    0,
    0,    0, 9663, 5357, 5357, 5357, 5357, 5357,    0,    0,
  245, 1368, 5357, 5357, 5357,  418, 5357,  277, 5357, 5357,
    0, 5357, 5357,    0,   87,    0,  312,    0,    0,  460,
 5357,    0, 5357, 5357, 5357,  494, 5357, 5357, 5357, 5357,
  277, 5357, 5357, 5357, 5357,    0, 5357,  427,    0,    0,
    0,  826,    0,  826,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  541,  629,  119,  962,    0,    0,    0,    0,    0,
 5357, 5357, 5357,  687,  729,  735,  115, 2354,  728,  142,
  183, 1072,  747,    0, 5357,  916, 1445,    0,  811,    0,
 5357, 5357, 5357, 5357,    0,    0,    0, 5357,    0,  987,
   12,  947, 1119,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  562,    0,
    0, 1368,    0,  595,    0,    0, 1309,    0,    0, 1409,
    0,  628,    0,    0,  950,    0,    0, 1049,    0,    0,
 1443,    0, 1255,    0,    0,    0, 1368,    0,    0,    0,
  -59, 1309,    0,  761,    0,    0, 1247,    0, 1273,    0,
  826,    0,    0,    0,    0,    0,    0, 1204,    0,    0,
 1137,    0, 1297,    0, 1203,    0,    0,    0, 1137,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9407,    0,
 5357,    0,    0,    0, 4540, 1246, 5060, 5357, 1159, 1328,
 1697, 1447, 1453, 4540, 1376, 5060, 1399, 1415, 1303,    0,
11098,    0,    0,    0,    0,    0,    0,    0,    0, 8679,
    0,    0,    0, 1406, 5357, 1497, 1398,  908, 5357, 1418,
    0,    0, 1445,    0,    0,    0,    0,    0,    0,    0,
 5357,    0,    0, 1309,    0, 1247,    0,  826,    0, 1470,
11098, 1456,    0, 1420, 1433,  -32, 1400, -115, 1408, 1387,
 1119,    0, 1638, 1158,    0, 5357,    0,  562,    0,    0,
    0,    0, 5357, 1521, 3423, 1467, 3795, 1899,    0, 9663,
    0,    0,  642,    0,    0,    0, 1884, 1884, 1368, 1582,
 1368, 2347, 1443,    0,    0, 8645,    0, 5357, 5357,    0,
 1521,    0,    0, 1674,  -69, 1926, 5357, 1707, 5357,    0,
    0, 1932,  257, 5973,    0,    0,    0, 1937, 1943, 1944,
 1529,    0,    0, 1960,    0, 1961,    0, 1965, 1998,    0,
 2004,    0,    0,    0,    0,    0, 2005, 1554, 1588,    0,
    0,    0,    0,    0,    0,    0, 2015,    0,    0, 9504,
 2016, 2024,    0,11098,11098, 8777, 5357, 2032,    0, 2592,
    0,    0,    0,    0,    0,    0,    0,    0, 2033, 1994,
    0,    0,    0, 2044,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1763,    0, 5357, 1655,    0, 5357, 5357,   85,    0,
    0,    0,    0,    0, 5357, 5357,  567,    0, 5357,    0,
 1782, 5357, 2059, 4540, 1704,    0, 5357, 9743, 1689,    0,
    0,11098,    0, 1788, 2066,    0, 1847,    0,   66,    0,
 1225,    0,  -72,    0,  -72,    0,    0, 2074,    0,  113,
 1691,    0,    0,    0, 2076,   71, 2062, 1521, 2080, 1862,
    0,    0, 1916, 2086,    0,    0, 1751, 1627,    0,    0,
  706, 1791, 1793,    0,    0,    0,    0, 1368,    0,    0,
 1794,    0, 1884, 1884, 1884, 1884,    0, 2105,    0, 1877,
 1881,    0,    0,    0, 1861,    0, 8645,    0,    0, 8645,
 1790,    0, 1309, 1898, 5357,12695,    0,    0,  603,    0,
 5357,  623, 1409, 1775, 1728, 1712,    0,    0,    0,    0,
  -56, 1777,    0, 1729,  277,    0, 1368,    0, 1909, 1835,
    0,    0, 2347,    0,    0,    0,    0,  217, 1741,    0,
    0,    0, 1743,    0, 1792, 2149,    0, 1848, 2159,  516,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2116,  138,    0,    0,    0,    0,  139, 2169,
 2170, 2159, 1309,    0, 1904,    0, 1952,    0, 1368, 1368,
    0, 3423,  628,    0, 1834,    0, 1812, 2180,    0, 6906,
 1936,    0, 1836, 1850, 2189, 6576, 2193,    0,    0,    0,
 5876, 7104,    8,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1143, 9982,  136,    0,
 5357,11098, 1153, 1156, 1217, 3150,    0,    0,11098, 6806,
 2592, 1783,  803,11098, 9982,  924,  924, 1295, 1750, 1840,
 2194,    0,    0,10062, 5357,11098,11098,11098,11098,11098,
 9982,11098, 1703, 5357,    0,    0, 7293, 1800,    0,    0,
 1789, 5060,    0, 1945,    0, 1841, 1295, 2223,    0, 1945,
 7293, 1946, 1949, 5357,    0, 1750,    0, 1798, 1635, 1819,
 9982,    0, 5357,    0,  816,    0,    0,   38,    0, 1816,
    0,    0,    0, 5357,  486, 5357,    0,    0, 5357, 5357,
    0,    0, 9982, 2009, 3423,    0, 1772,11098, 5357,    0,
    0, 1368, 1368, 1368,    0,  -10,    0, 1729, 1729, 1817,
  277, 1387,    0,    0, 2237,    0,    0,    0,    0, 2006,
    0, 5357,    0,    0,    0,    0,    0, 2007, 2249,    0,
 1990, 2010,    0,    0, 1951, 1953,  826, 2018,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  277, 5357,
12776, 1919, 1920, 1921,    0,  277,    0, 1505,    0,    0,
 1900, 1368,    0,   95, 1913,    0,    0,    0,  277,    0,
 2149,  125,    0,    0,    0, 2031,  125, 1915,  125,    0,
 2276,  277,  277,  277,    0, 1521, 5357,    0, 1991,    0,
 1873,    0,    0,    0,  642, 1927, 5357, 1882,   94,    0,
    0, 2296, 2069,    0,    0,    0,    0,  257, 5357, 1931,
 7054,    0,    0, 1931, 6576,    0,    0,    0, 3629,    0,
  362,  424,11098,11098, 7151, 2306, 1283, 7409,11098, 7561,
 1180, 7670, 7929,10301, 8081, 8190, 8449, 6806, 6806,11098,
11098, 2085,11098, 2309,11098, 2317, 1868,11098,11098,11098,
11098,11098,11098,    0,    0,    0,    0,    0,    0,    0,
    0, 2034, 1459,  513,11098,  858, 9982,    0, 6806, 2214,
 2318, 1072,    0,    0,    0,    0, 2592, 1893,    0,    0,
 2088,  924,  924, 1366, 1366,    0, 1150,    0,    0,    0,
    0, 5357, 5357, 5357, 5357, 5357, 5357,  109, 2321,    0,
    0,    0, 7293, 1984,    0,  -26, 2008, 5357,    0, 1072,
 9982, 2008, 2327, 2321, 7293, 4020, 2058,    0,    0, 1069,
 2340, 2337, 1360,    0,    0,    0, 6806, 2054,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  395,    0,    0,
    0,    0,    0, 1924, 1927,    0,    0,    0, 2343,    0,
    0,    0, 1929,    0,    0, 1884, 2122,    0,  -10,    0,
    0,    0,    0,    0,    0,    0, 2349, 6806, 3747,    0,
    0, 2092,    0,    0,    0, 3747,    0,    0,    0,  826,
  826, 1897, 2360,    0,    0,    0,    0,    0,  504,   22,
 1038,    0, 2137,    0,    0,    0, 5357,    0,   88, 1938,
 1981, 2006, 2361,    0, 1729, 1966,    0,    0, 1191,    0,
 2348, 5357, 1192,  277, 2364, 2367, 2369, 2148,  628, 2092,
 1884, 1793, 3423,    0, 1777, 1072, 1959, 1974,    0, 1486,
    0,  277, 2022, 2109, 6906,    0,    0,  277, 1930, 2110,
    0,    0, 2380, 2382,    0, 6806, 6806,    0, 6806, 6806,
 2267, 2592, 2384,    0,    0, 2592, 2387,10540,    0,    0,
    0, 2078, 2402, 2592, 2403, 1422, 2409, 2592, 2410,11098,
11098, 2306,11098,10620, 2414, 2592, 2417, 2418, 2592, 2420,
11098, 2592, 2421, 2592, 2424, 2425, 2592, 2427, 2428, 2592,
 2433,    0, 2212, 2039, 2285,    0, 2310,    0, 2503,11098,
    0, 2513, 2722, 2781, 3099, 3271, 3335,11098,11098, 9982,
 2592, 2040, 9982,    0,11098,    0, 2168,  573,11098, 9982,
11098,11098,    0,    0,    0,    0,    0,    0,    0,    0,
 2150, 7293,    0, 7293,  109, 2321,    0,  236,  236,  161,
    0,    0, 8679, 1973,    0, 2327, 2321, 2434, 4020, 2443,
 1360, 2445, 9982, 1847, 1635, 2099,    0, 2112, 2113, 2155,
 2219, 2171,    0,    0,    0, 2092,    0, 1959,  631, 2082,
 1368, 2117, 1729, 2118, 1729,    0, 6806,  440, 1435,    0,
    0,    0,    0,    0,    0,    0,    0, 2064, 2240, 2092,
    0,11098,    0, 2221, 2121, 1897,   87,    0, 6806,    0,
    0,    0, 2133, 2134, 2138, 2139, 2154, 2163,    0,    0,
    0,    0,    0, 2493,   88,    0, 1817,    0,    0,  125,
    0, 3747, 2158,  125,    0,  125,    0, 2006, 2510,    0,
    0,    0,    0,  642, 2240, 1729,    0, 1793,    0,    0,
 8679, 2275,    0,   36,    0, 2343,    0, 2517,    0, 2518,
 1368,  -66, 2520,    0, 1368,    0,    0,    0, 2212,    0,
 2212,11098, 2146, 2146, 1295, 2531, 1975,    0, 2146, 2146,
11098, 2146, 2146, 2496, 2592,    0, 1642,11098, 2592, 2146,
 2146, 2146, 2146, 2592, 2146, 2146, 2146, 2146, 2146, 2146,
    0,    0, 8645, 2532,    0,    0, 3403,    0,    0,    0,
    0,    0,    0, 3424, 1594,    0, 9982, 2266, 2156,    0,
 9982, 3526, 1987, 1547, 2592, 2196,    0,    0,    0,    0,
    0,    0,    0, 2147,    0,    0,    0,    0, 1788,    0,
 4020, 2434, 1847,    0, 1988, 5357, 1360,    0,    0,    0,
 1635,11098, 6806, 2188, 2240,    0,    0,    0,    0, 1884,
    0,    0, 1368, 1505,    0,    0,  479,    0,    0,    0,
 5357, 7804,    0,    0, 2592, 2312, 2221,    0,  881, 5357,
    0, 2259,  619, 2157, 2264, 2161, 2303, 2274, 2304, 2092,
    0,    0, 2574,    0, 2580,    0, 2270, 2312, 2158,    0,
    0,    0,    0,    0,    0,    0,    0, 2092, 1777,    0,
 1505, 2137, 2307, 2238,    0, 1927,    0,    0,    0, 2257,
  -78,    0, 2006, 2258, 2592, 2595, 2597, 1072,    0,    0,
 2604, 2607, 2592, 2613, 2618,11098,11098, 2592, 2619, 2622,
 2625, 2627, 2628, 2629, 2637, 2638, 2646, 2652,    0,    0,
    0,    0,11098, 2655,    0,    0, 9982,    0,    0,    0,
 1819,    0,  236, 1819, 5357,    0,    0, 1389, 2592, 2654,
    0,    8,  845,    0,    0,    0,    0, 1729,    0,    0,
    0,    0, 8645, 2640,    0,    0, 2408,    0, 2141, 1009,
 2643, 2663, 2365,11098, 2653, 2659, 2172, 5357, 5357,    0,
 2660, 2662, 2374, 2664,    0,    0,    0, 2405,10202, 2672,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2675,  355, 2679,    0,    0,    0,    0,    0,
 2681, 5357,    0,    0, 2386, 2397, 5357, 1897,    0,    0,
  826, 5357,    0,    0,    0,    0,    0,    0,    0, 2240,
    0,    0,    0,    0,    0, 2240,    0,    0,    0,    0,
 1668, 1819,    0,    0,11098, 1368,    0,    0,    0, 1368,
    0,    0,11098,    0,    0,    0,    0, 2592, 2592,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2592,
    0,    0, 2054,    0, 2054, 2710,    0, 6806,11098,    0,
    0, 2385,    0, 2712,    0,    0,    0,  134,    0, 2436,
    0,10859, 5357,    0,    0,11098,    0, 6806, 2592,    0,
    0,    0, 2696,    0,    0,    0, 5357,    0,    0, 2108,
    0,    0,  939,    0, 2315,    0,    0,    0,  -13, 2717,
    0,    0,    0,    0, 1897,  826,    0, 2377,    0,    0,
    0,    0, 2137,    0, 1668, 2726, 1360,    0,    0, 2054,
 2592,    0,    0, 1547, 2171, 2171, 5357,    0,    8,    0,
    0, 6318, 3747,    0,    0, 2732,    0, 2758, 9407, 1579,
  708, 2715,    0, 2437, 1448,    0,    0, 2734,    0, 5357,
 5357,   88,  143,    0,    0,  540, 2444, 6806,    0, 2377,
   57, 1897, 2404, 2407,    0,    0, 1360, 2744, 1668, 2451,
 2171, 2756, 1819, 2188, 2188,    0, 2002, 5357,    0, 2766,
 2764, 1208,    0, 1931,    0, 2399, 2108,    0, 2375,    0,
 2108,    0, 2108, 1738,    0,    0,    0,    0, 7804,  939,
11098, 2108,  711, 1897,  848,  848,    0, 2512,    0, 2376,
    0,    0,    0, 1360, 1668,    0,    0, 2054, 2429, 2429,
 5357,    0,    0,    0, 6318, 2775,    0, 5357,    0, 2779,
 1558, 2762, 1503, 7804, 2785,    0,    0,    0,    0, 1372,
  160, 2521,    0, 2371,  584,    0,    0,    0, 1392, 2171,
    0,    0,    0,    0, 5357, 2495,    0, 2108, 2785,    0,
    0, 2528, 2108, 2543, 2533, 7804,    0,    0,    0, 6806,
 2188, 2794, 2798, 2801,    0, 2021, 7804,  159, 5357, 7804,
    0,    8, 2429, 5357,    0, 5357,    0,    0, 7804,    0,
    0,    0,    0, 2802,    0,    0,
};
static short yyrindex[] = {                                      0,
    0,  216,  946,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  168, 6231,    0,    0,    0,    0,    0,    0,
 2844,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  230,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1168,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2509,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2469,    0,    0,
    0,  343,    0,  343,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2472,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1196,    0,
    0, 9016, 6490,    0,    0,    0, 1325,    0,    0,    0,
 2950,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  131,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    5,    0,    0,    0,
    0,  225,    0,    0, 1361,    0,    0,    0,    0,    0,
  154,    0,10960,    0,    0,    0,    0,    0,    0,    0,
    0,    5,    0,  528,    0,    0, 2819,    0,    0,    0,
 2596,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1369,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1407,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  802,    0,    0,    0,    0,    0,    0,    0, 5909,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  774,
    0,    0, 1487,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    5,    0, 2819,    0, 2596,    0, 1493,
    0,    0,    0,    0,  717,    0,    0,    0,    0,    0,
 1496,    0, 2583,    0,    0,    0,    0, 1528,    0,    0,
    0,    0,    0, 2601,  633,12829,    0, 1556,    0,    0,
    0,    0,  371,    0,    0,    0, 2413, 2413,    0,    0,
    0, 1564,  201,    0,    0,    0,    0,    0,    0,    0,
 2601,    0,    0, 1446,    0,    0,    0,    0,    0,    0,
    0, 5048, 2479,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1980,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 2848,    0,    0,    0,
 3152, 3456,    0,    0,    0,    0,    0,    0,    0, 1439,
    0,    0,    0,    0,    0,    0,    0,    0,  130, 3760,
 2471,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1573,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1584,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2553,    0, 2077,    0,    0,    0,
    0,    0,  261,    0,  261,    0,    0,  844,    0,  855,
 1223,    0,    0,    0,    0, 1369,    0, 2601,    0,    0,
    0,    0,  147,  391,    0,    0,  214,    0,    0,    0,
  221,    0,  657,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2413, 2413, 2413, 2413,    0,  849,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 2610,    0,    0,    0,    0,    0, 2583,    0,
    0, 1293,    0, 2406,    0,    0,  900,    0,    0,    0,
    0,  180,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1599,    0,    0,    0,    0, 2462, 2992,    0,
    0,    0, 4417,    0,    0, 2656,    0,    0, 7412,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1396, 1992,    0,    0,    0,    0,11435, 4937,
    0, 7412,    5,    0,    0,    0,    0,    0,    0,    0,
    0,  633, 1401,    0,    0,    0,    0, 2464,    0,    0,
    0,    0,    0,    0,    0,    0,    0,11704,11836,12011,
    0,    0,  599,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,11178,    0,11178,    0,
    0,    0,11178,11178,11178,    0,    0,    0,    0,    0,
 2431,    0,    0,    0,    0, 4800, 5104, 9168, 2835,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  324,    0,    0, 6231,    0,    0,  324,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1054,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1044,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  633,    0,  158,    0,    0,    0,
    0,    0,    0,    0,    0, 1100,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1993,
    0,    0,    0,    0,    0,    0,    0, 2845,    0,    0,
 3937, 3447,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  401,  766,  772,    0,    0,    0,  326,    0,    0,
    0,    0,    0,    0, 3143,    0,    0,    0,    0,    0,
 2656,    0,    0,    0,    0,  763,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 2601,    0,    0,  347,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 9917,    0,
    0,    0,  267,    0,    0,    0,    0, 2578,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,11178,    0,    0,    0,    0, 1742,    0,    0,    0,
 4064, 5320, 5624, 4280, 4584,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1613, 1629,    0,
    0,    0,    0,    0,    0,    0, 1634,    0,    0, 9016,
    0, 1634, 1640, 1645,    0,    0,    0,    0,    0,    0,
  818,  902,  972,    0,    0,    0,    0, 1055,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  313,    0,
    0,    0,    0,    0,    0, 2413, 2438,    0, 1296,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 2626,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1304,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, 1663,    0,    0,    0,    0,    0,    0,    0,
    0, 3330,    0,    0,    0,    0,    0,    0,    0,    0,
 1343,    0,    0,    0,    0,    0,    0,    0, 1401, 2626,
 2413, 1669,  633,    0, 2635, 9016, 1679,  118,    0,    0,
    0,    0, 9917,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 3262,    0,    0,    0, 5457,    0,    0,    0,    0,
    0,    0,    0, 6178,    0, 6570,    0, 8443,    0,    0,
    0,    0,    0,    0,    0, 8978,    0,    0, 9303,    0,
    0, 9383,    0,11471,    0,    0,11572,    0,    0,11807,
    0,    0, 5468,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2467,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1613, 1698,    0, 2468,  331,    0,
    0,    0,    0,    0,    0, 1725, 1735, 1755,    0,    0,
 2562, 1982,    0, 5704,    0, 2569,    0, 2570, 2572,    0,
    0,  247,    0,    0,    0, 2626,    0, 1679,    0,  720,
    0,    0,    0, 2458,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 7257,    0, 2647,
    0,    0,    0, 2686,    0, 1304, 2509,    0,    0,    0,
    0,    0,  829,  867,  878,  879,  883,  890,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  872,    0,    0,    0,    0, 3785,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 1756,    0,    0,
    0,    0,    0,    0,    0, 1762,    0,    0,    0,    0,
    0,  153,    0,    0,    0,    0,    0,    0,  294,    0,
  379,    0,    0,    0, 9168,    0,    0,    0,    0,    0,
    0,    0,    0,    0,11920,    0,12140,    0,12185,    0,
    0,    0,    0,12268,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2876,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1767,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, 1764, 1022,    0,    0,    0, 3001,    0,    0,    0,
    0,    0,    0,  156,    0,    0,    0,    0,    0, 2413,
    0,    0,    0,  827,    0,    0,    0,    0,    0,    0,
    0,   14,    0,    0, 2878, 1444, 3354,    0, 2583,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2626,
    0,    0,    0,    0,    0,    0,    0, 1765,  892,    0,
    0,    0,    0,    0,    0,    0,    0, 2626, 2635,    0,
  381,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  469,    0,12340,    0,    0,11178,    0,    0,
    0,    0,12369,    0,    0,    0,    0,12429,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  669,    0,  331, 1430, 2685,    0,    0, 2562,  993, 1103,
    0,  274, 2880,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,10475,    0,    0,
    0,    0,    0,    0,    0, 1805, 1872,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  781,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 1906,    0,
    0,    0,    0,    0,    0,    0,    0, 3938,    0,    0,
  192,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  176,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,12501,12573,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 2883,
    0,    0,  587,    0, 1148, 2842,    0,    0,    0,    0,
    0, 2881,    0,    0,    0,    0,    0, 2867,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, 2868,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 2620,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 3938,  493,    0,  107,    0,    0,
    0,    0,    0,    0,    0,  175,  369,    0,    0,  126,
 2878,    0,    0,    0,  260,  250,    0,    0, 4425,    0,
    0, 2880,    0,    0,    0,    0,    0,    0,  108,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  618,
    0, 1304,  850,  960,    0,    0, 2562, 2373,    0,    0,
  105,    0,  669,  128,  207,    0,    0, 1491,    0,    0,
 2888,    0,    0,    0,    0, 2879,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  535,    0,
    0,    0,    0, 3938,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  733,    0,    0,    0,  587, 2894,  882,
    0,    0,    0,    0, 2880,    0,    0,    0,    0,    0,
    0,    0,    0,  535, 2885,    0,    0,    0,    0,    0,
 2633,    0,    0,    0,    0,    0,    0,    0, 2562,  260,
    0,    0,    0,    0,    0, 2035,    0,    0, 2886,    0,
    0,12577,    0,    0,    0,  535,    0,    0,    0,    0,
  128, 2901,    0,    0,    0,    0,  535,    0,    0,  535,
    0, 2665, 2894,    0,    0,    0,    0,    0,  535,    0,
    0,    0,    0,    0,    0,    0,
};
static short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, 2946,    0,    0, 2947,
    0,    0,    0,    0,    0,    0,    2,    0, 2948,    1,
  237,  -43, -612, 1744,  353, -311,   50, 2939, 2676,    0,
   30,    0, 2688,  -96,  -31,    0,  616, -919,  -20,  -53,
  -42, -225, 2687, 2232,    0,    0,    0,  801,    0,    0,
  -19, 1853, -579,  -57, -886, 2313, 2097, -309, 1865, 2882,
 -301,    0,  -58,    0, -466,  -51,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -628, -131, 1875, -239, 1194, -897,    0,
    0,    0,    0, -334, -128,-1073, 1885, -340,  -48,    0,
 -529,    0,    0, 1520, -994,    0, 1525,    0,    0, 2657,
 -671,  -18,    0,    0,    0,    0, 2634,    0, 2398, 2673,
 -240, 1922, -694,    0,    0,    0, -733,    0,    0,    0,
    0,    0,    0, 1969,    0, 2689, 2245, 1918, 2667, 2669,
    0, 1347,    0, -534, 2215,  -33, -658, 1426,    0,    0,
    0, -944, -942, -930, -929, -926,    0, 1585,-1010,    0,
 1259,-1173, 1587,    0, 2450,    0,    0,    0, 1263, 1260,
 1224,  -38, -277, -604, -138,    0,    0, 2239,    0, 1769,
    0,    0,    0,    0,    0,-1266, -851, -674,    0,    0,
    0,    0, -219,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 1298,    0, 1279,    0,
  664,    0,    0,  -82,    0, -391,  363, 1482, 1345,    0,
 1241,    0,    0,    0,    0,    0, 2037, 1808, 1631,    0,
-1088, -877, -894,    0, -500, -816, -679,    0,-1451, 1334,
 1129, -376, -515, -713, -957,-1276,    0,    0,   21,    0,
    0,    0,    0,    0, 2700,    0,    0,    0,    0, 2716,
 2482,    0,    0,    0,    0,    0,   -9, 2727,    0,    0,
    0,    0,  -15,    0,    0,    0, -348, 2208, -346, 1768,
 -460,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 2439,    0, 2218,    0,    0,    0,    0,    0,    0,  149,
    0, 3052,    0,    0,    0,    0,    0, 2767,    0,    0,
    0,    0, 2568,    0, 2571,    0,    0,    0, 2319,    0,
    0,    0,-1475,    0,    0,  645,    0, 2311,    0,    0,
    0,    0,    0, -636, -973, -557, -170,  847,    0, 2932,
 -130, 1601,    0, 2336, -273, -961, -871,    0, -830, 2090,
    0,    0,    0,    0, -137,    0,    0,    0, 1423, 1551,
    0, 1262,    0, 1307,    0, -281,    0,    0,    0,    0,
 2575,    0,    0,    0, -407, 2358, 2120,-1054,    0,    0,
    0, -692, -424,  129,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 2522, 1106, 1948,
    0, -606,    0,    0,    0, -914,    0,    0,    0, 1845,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -201, 1910,    0,    0,
};
#define YYTABLESIZE 13349
static short yytable[] = {                                     129,
  151,   37,  129,  194,   78,  217,  150,  264,  156,  284,
  287,  283,  286,  313,  213,  761,  497,  623,  934,  624,
  960,  591,  788,  585,  197,  791,  248,  557, 1113,  214,
  234,  802, 1114,  254,  700,  222,  800,  252,  747, 1024,
  257,  470,  209,  191,  193,  196,  199,  150,  204,  229,
  470, 1263,  258,  215,  193,  220,  767,  199,  253,  150,
  204,  942,  233,  129, 1191,  259, 1010,  151,  725,  251,
  874,  244,  256,  150,  246,  193,  255,  244,  220,  196,
  199,  653,  150,  204,  233,  129,  200,  199,  226,  547,
  980,  514, 1441,  637, 1252,  555, 1253,  270,  227,  272,
  318, 1252, 1213, 1253,  715,  957,  257, 1015, 1254, 1255,
  745,  226, 1256,  491,  760, 1254, 1255,  368,  983, 1256,
  314,  215,  150,  215, 1257,  710,  294,  865,  722,  331,
  648, 1257,   77, 1103,  378,  220, 1042, 1310, 1069, 1070,
 1019,  312,  199,  150,  233, 1049,  642,  257,  156, 1264,
  257, 1095, 1202,  151, 1395,  719,  756,  646,  268, 1221,
 1272,  271,  268,  715, 1547,  257,  333,  588,  719, 1075,
  732,  648,  340,  515,  373,  717,  368,  914,  852,  410,
 1293,  281,  316,  371,  710, 1288, 1800,  642,  331,  648,
  642,  770,  775,   89, 1733, 1620,  719,  367,  646, 1090,
  152,  646, 1793, 1793,  282,  642,  719, 1481,  778,  779,
  780,  781,  151,  650,  719,  588,  646,  623, 1311,  624,
  177,  520,  285,  282,  390, 1477,  588,  463,  849,  636,
  481,  882,  770,  373,  717,  770, 1162, 1163,  410,  998,
  764,  540,  720,  500,  822,  282,  715,  719,  135,  715,
  770,  215,  591, 1439,  650,  150,  834,  472,  129,  152,
  616,   77, 1406, 1767,  150,  719,  472, 1188,  269,  908,
  636,  322,  650,  714,  588,  518, 1627,  513,  640,  177,
   77,  823, 1388,  390, 1390,  196, 1056,  715,  636,  150,
  715, 1685, 1686, 1208,  466, 1476,  835, 1456,  282, 1756,
  715,   77,  556,  466,  750,  715,  320,   85,  715,  559,
   85,  214,  649,  572,  714,  530,  574, 1814, 1399,  616,
  503,  293, 1275,  761,  531,  196,  215,  645,  507,  808,
  539,  343,  714,  215,  792,  215,  633,  215,  515,  586,
  577,  588,  368, 1851, 1852, 1026,  125, 1252,  635, 1253,
 1540,  236,   60,  649, 1236,  641,   83, 1805,  199,  204,
 1205, 1254, 1255, 1849,  136, 1256, 1248,  150,  374,  196,
  492,  649, 1216, 1218,  277,  751,   97, 1257,  369,  846,
  196, 1076,  761, 1451,  196,  824,  770, 1624,  237,  539,
  635,  207,  208,  909, 1407,  288,  133, 1619, 1271, 1482,
  396,  541, 1118,   14,  912,  125,  643, 1892,  158, 1209,
 1521, 1466,  374, 1628,  848,  269, 1473,  703, 1027,  794,
 1744,  941,  987, 1864, 1707, 1524, 1439,  374,   14,  492,
  755,  635,  151, 1046,  276,  719, 1766,  956,  913,  196,
  733, 1525,  513,  410, 1328, 1329, 1799, 1330, 1331,  635,
  719,  159, 1096,  151,  150,  828,  710,  199,  472,  396,
 1115,   89, 1889, 1276,  642,  150,  343,  497, 1654,  150,
  648, 1316,  199,  719,  150,  646,  505,  156,  177,  152,
 1428, 1388,  151,  390,  532, 1305,  642, 1102,  390, 1012,
  278,  177,  770,  716,  390,  719,  470,  646, 1806,  177,
  152,  151,  373,  717,  724,  373,  717, 1422,  730,  541,
  151, 1617,  541,  466,  719, 1396,  151,  333,  776, 1541,
  177, 1337,  384,  280,  794,  874,  177,  829,  177,  152,
 1542, 1759,  804,  770,  151, 1590,  770, 1591,  721,  331,
 1011, 1036,  719, 1037,  715,   76, 1039,  636,  152,  177,
  744,  770, 1033,  650,  267,  797,  215,  152,  715, 1105,
 1045,  797, 1201,  152,  715,  710,  988,  830,  177,  636,
 1212,  368, 1618, 1304,  770, 1427, 1110,  177, 1088,  710,
 1528,  152,  280,  177,  648,  851,  715, 1063, 1807, 1214,
 1469,  714, 1454,  719,  856,  719, 1629, 1443,  716,  280,
  642,  177,  616,  588,  196,  226, 1402,  257,  214,  719,
 1633,  646, 1890,  714,  373,  717, 1003,  257, 1033,  859,
  860,  390,  515, 1116,  588,  196,  373,  710,  373,  717,
 1843, 1415,  215,  715,  492,  735,  715, 1665,  177,  716,
  280,  876,  715, 1667, 1468,  715,  715,  651,  642, 1031,
 1032,  875,  649,  205,  196,  715,  480,  716,  257, 1603,
  714,  257,  916, 1186,  396,  616,  651,  650,  616,  714,
  151,  220, 1699,  196,   76, 1052,  257,   85,  221,  792,
  961,  196,  196,  636,  616, 1119,  715,  651,  196, 1539,
  761, 1243,  770,   76,  125,  950,  374,  539, 1488,  374,
  715,  908,  719,  215,  215, 1710,  196,  472,  635,  717,
  616,  708,  472,  714,   76,  480,  652,  152,  196,  181,
  196,  472, 1714, 1035,  199,  770,  513,  714,  196,  199,
  635,  214,  375, 1000,   76,  463, 1195,  177, 1783, 1198,
  908, 1532,  541,  196,  150,  773,  150, 1117, 1789,  215,
 1773, 1842, 1020, 1020, 1025,  215, 1306,  652,  559,  215,
  181, 1054,  135,  181,  794,  394,  649,  492,  196,  239,
  770,  395,  770,  611,  908,  652,  375,  541,  181,  770,
  984,  241,  215,  507,  883, 1009, 1055, 1057, 1252,  890,
 1253,  375,  844, 1105, 1085, 1086, 1087,  396, 1383, 1233,
 1442,  226, 1254, 1255, 1262,  323, 1256, 1700,  374, 1120,
  215,  215, 1068,  122, 1765,  249, 1270,  697, 1257,  333,
  374,  135,  374, 1772,  394,  909,  196,  260,  401, 1455,
  395, 1405,  611, 1509,  908, 1465, 1725, 1292, 1726,  226,
  515, 1273,  196,  617,  635, 1610, 1062,  150,  482,  253,
 1234,  341,  343,  131,  627, 1776, 1363,  150,  697, 1073,
  845,  697,  160, 1616,  909,  261,  402,  334, 1812, 1112,
  770,  136,  226,  226,  226,  770,  697,  398,  397,  541,
  908,  636,  400, 1868,  351,  196, 1089,  401, 1006,  399,
  253,  137,  770,  253, 1694,  257, 1100,  257,  909,  770,
  578,  679,  617, 1771,  257, 1007, 1186,  482,  253, 1186,
  242,  579, 1199,  627,  480,  161,  716,  710, 1265, 1266,
  132,  805,  636,  265, 1200,  402,  716,  480,  342,  716,
  136, 1196, 1830,  770,  541, 1869,  398,  397,  716,  497,
  636,  400,  679, 1197,  513, 1274, 1003, 1180,  399, 1853,
  137,  242,  472,  199,  204,  472,  233,  129,  909,  254,
  679,  352,  480,  472,  480,  709,  526,  775, 1317,  908,
  710,  680,  908,  710,  772,  472,  472,  181, 1376,  335,
  199, 1378,  710,  727, 1674,  716, 1729,  343,  122,  122,
  181, 1220,  713,  770,  716,  257,  717,  336,  181,  717,
  254, 1850,  806,  254,  909,  269, 1741, 1381, 1668,  883,
  728,  266,  680,  883,  623,  680,  624,  257,  254,  181,
 1653,  678, 1206, 1655,  257,  181, 1299,  181,  372,  394,
  680,  337,  242,  713, 1217,  395,  713,  497,  716, 1450,
  135,  382,  135,  807, 1319, 1774, 1775, 1675,  181,  387,
 1323,  713,  716,  717,  710,  717,  652, 1283,  257,  214,
  375,  678,  678,  375,  717,  678,  463,  181,  145, 1284,
 1472,  273, 1283,  242,  480,  708,  181, 1393, 1393, 1397,
  678,  697,  181,  215,  196, 1538, 1803,  815,  291,  816,
  215, 1816,  401,  909,  717,  710,  909,  196,  343,  187,
  181,  960,  709, 1186,  226,  196,  482, 1186,  990,  937,
  938,  274,  717,  710, 1424,  611, 1426,  275,  257,  482,
 1433, 1720,  226, 1417, 1838,  373,  196,  253,  226,  253,
  402,  492,  196, 1418,  196,  697,  253,  181,  697,  498,
  135,  398,  397,  709,  149,  697,  400,  710,  697,  382,
  611,  136,  697,  399,  482,  196,  482,  697,  187, 1862,
  697,  709,  394,  279, 1183, 1184, 1470,  590,  395,  382,
 1871,  137,  375,  146,  196, 1515,  697, 1471,  611, 1518,
  147,  611,  196,  196,  375,  617,  375, 1660,  710,  196,
  503, 1881,  472, 1192,  472,  586,  627,  611, 1882,  808,
  697, 1749, 1888,  705,  697, 1891,  710,  196,  706,  472,
  115,  245, 1661,  697, 1895,  199, 1819, 1820,  775,  679,
  617, 1020,  612,  611, 1844,  401,  590,  253,  311,  679,
  697,  627,  679,  343, 1294, 1294,  181,  254, 1625,  254,
  141,  679, 1194, 1186,  196,  775,  254,  115,  617,  253,
 1750,  617,  295,  939,  586,  196,  253,  697, 1595,  627,
  321, 1751,  627,  402,  324, 1845,  482,  617, 1527,  697,
 1662,  697, 1818,  280,  398,  397, 1535,  697,  627,  400,
 1596,  612,  115, 1295, 1298, 1453,  399, 1597,  679,  680,
  253,  504,  474,  617,  515,  188,  986,  679,  152,  680,
 1002, 1480,  680,  251,  627, 1484,  697,  697, 1185,  697,
  713,  680, 1846, 1883,  708, 1652,  697,  697,  697,  697,
  697, 1598,  697,  713,  595,  697,  697,  697,  697,  697,
  153,  288,  713,  697,  697,  441,  474,  254, 1277,  678,
 1770,  679,  678,  196,  251,  154,  505,  251,  343,  678,
  253,  474,  678,  679,  188,  679,  678,  187,  680,  254,
  592,  678,  251,  506,  678, 1752,  254,  680,  717,  986,
  187,  717,  710,  354,  289, 1278,  326, 1713,  187,  713,
  678,  717, 1543,  595,  717,  710,  516, 1657,  713,  327,
 1770,  472, 1393,  717,  710,  518,  312,  214,  513,  187,
  254,  199,  288, 1020,  678,  187,  333,  187,  678, 1666,
 1673,  680,  288,  709,  707,  288,  708,  678,  710,  592,
  709, 1548, 1589,  680,  328,  680,  329,  717,  187,  717,
  150,  515,  713,  709,  678,  516,  518, 1770,  753,  518,
  717,  710,  709,  217,  553,  910,  713,  187,  620,  717,
  710, 1140,  430, 1674,  518,  918,  187,  554,  920,   99,
  254,  678,  187,  709,  707,  333,  708, 1141,  710, 1601,
  717, 1711, 1770,  678,  489,  678,  288, 1712,  710,  753,
  187,  678,  753,  430,  217,   14,  594,  217,  717,  709,
 1672, 1793,  581,  717,  710,  417,  101,  753,  709,  620,
  709,  707,  217,  708, 1708,  710, 1792,  717,  710,  356,
  678,  678, 1142,  678,  523,  312, 1675,  187,  365,  922,
  678,  678,  678,  678,  678,  261,  678,  418,  105,  678,
  678,  678,  678,  678,  710,  513,  330,  678,  678,  193,
  379, 1143,  709,  710, 1826,  594, 1793, 1684, 1112, 1112,
 1709,  581, 1748,  188,  417,  414,  709,  106,  107, 1589,
  108, 1861,  375,  160,  612,  102,  188,  109,  110,  111,
  112,  113,   43,  114,  188,  103,  115,  116,  117,  118,
  119, 1827,  950,   51,  120,  121,  418,  150,  709,  707,
  377,  708,  196,  710, 1828,  188,  380, 1858,  161,  612,
  104,  188, 1893,  188, 1894,  151, 1722, 1746, 1144, 1760,
 1723,  150,   41,  691,  414,  427,  187,  696,  697,  699,
  709,  707,  160,  708,  188,  710, 1706,  612,   24,  748,
  612,   43,  383,  766, 1131,  709,  707, 1788,  708,   47,
  710, 1739,   51,  188,   49,  705,  612,  467,  300, 1748,
  706,  705,  188, 1748,  384, 1748,  706,  161,  188,  390,
 1132,  515,  494,  199, 1748,  543,  345,  475,  127,  346,
  390,   41,  612,  518,  990,  518,  188, 1112,  385,  518,
 1226,  736,  518,  709,  707,  738,  708,   24,  710,   80,
   81, 1795,  766, 1748, 1227,  518,  515,   25,   47,  151,
 1757,  705,  753,   49, 1746,  150,  706, 1715, 1746, 1226,
 1836, 1863, 1226,  188,  347,  544, 1778, 1000, 1228, 1746,
 1748,  494,  150, 1227,   48, 1748, 1227,  127,  515,  737,
  709,  707, 1341,  708,   50,  710,  476,  385,  705,  515,
  193, 1797,  515,  706,  959,  753,  753, 1228, 1746,  545,
 1228,  515, 1798,  151,   45,  128,   25,  717,  753,  150,
  717,  116, 1778,   46,  217,  513,  479,  546,  150,  798,
  480, 1658, 1229,  518, 1870, 1746, 1856,  482, 1179,  959,
 1746,  484,  894,   48,  215,  894,  296,  140,  141, 1589,
  943,  709,  707,   50,  708,  518,  710,  485,  357,  151,
  513, 1229,  518,  715, 1229,  150,  714,  895,  486, 1778,
  895, 1000,  188,   45,  128,  150,  717, 1387,  196, 1389,
  116,  297,   46,  217, 1589,  717,  705,  730,  207,  208,
  793,  706,  513,  792,  894,  358,  518,  359,  989,  709,
  707,  499,  708,  513,  710,  196,  513,  502,  753,  298,
 1008,  511,  299,  521,  889,  513, 1589,  501,  705,  895,
  360,  527, 1008,  706,  730, 1008,  739, 1589,  300, 1112,
 1589,  430,   14,  705,  196,  917,  196,  730,  706, 1589,
  944,  979,  933,  711,  978,  361,  528,  940,  430,  753,
  753,  403,  404,  405,  301, 1064, 1065,  947,  753,  951,
  952,  953,  954,  955,  430,  947,  123,  123, 1171,  709,
  707, 1513,  708, 1094,  710,  539, 1093, 1013, 1787,  413,
  414,  705, 1101,  542,  550,  870,  706,  753,  753, 1013,
  753,  563, 1013, 1193,  430,  570, 1192,  753,  753,  753,
  753,  753,  573,  753,  583,  105,  753,  753,  753,  753,
  753,  999, 1497,  587,  753,  753,  430,  709,  707,  638,
  708,  523,  710,  996, 1237,  642,  986, 1093,  705, 1242,
  644,  646, 1241,  706,  106,  107,  677,  108, 1286, 1006,
   77, 1285,  678,  679,  109,  110,  111,  112,  113,  680,
  114,  541,  541,  115,  116,  117,  118,  119,  958,  681,
  682,  120,  121, 1135,  683, 1139, 1291, 1147, 1150, 1290,
 1155, 1158, 1161, 1400,  687, 1490,  711,  105, 1192,  982,
 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1520, 1526,  705,
  711,  711,  541,  541,  706,  541,  541,  684, 1006, 1006,
 1006, 1006, 1822,  685,  686, 1821,  106,  107,  688,  108,
  541,  541, 1747,  541,  689,  694,  109,  110,  111,  112,
  113, 1887,  114,  695, 1793,  115,  116,  117,  118,  119,
 1006,  704, 1006,  120,  121,  738,  711,  705,  738, 1361,
  709,  707,  706,  708,  712,  710, 1121, 1122, 1126,  713,
  717, 1134, 1136, 1138,  105, 1146, 1149, 1152, 1154, 1157,
 1160,  731,  722, 1164, 1165,  734, 1167,  739, 1169,  741,
  742, 1172, 1173, 1174, 1175, 1176, 1177,  754,  758,  765,
  675,  759,  763,  106,  107,  766,  108,  105, 1181,  768,
  430,  771,  769,  109,  110,  111,  112,  113,  774,  114,
  530,  777,  115,  116,  117,  118,  119,  705,  782, 1747,
  120,  121,  706, 1747,  783, 1837,  106,  107,  785,  108,
  786,  795,  105,  820, 1747,  427,  109,  110,  111,  112,
  113,  819,  114,  826,  430,  115,  116,  117,  118,  119,
  236,  821,  832,  120,  121,  831,  280,  836,  839,  837,
  838,  106,  107, 1747,  108,  705,  841,  105,  842, 1671,
  706,  109,  110,  111,  112,  113,  847,  114,  853,  854,
  115,  116,  117,  118,  119,  858,  867,  237,  120,  121,
 1747,  857,  869,  870,  878, 1747,  106,  107,  880,  108,
 1682,  879,  884,  935,  945,  973,  109,  110,  111,  112,
  113, 1006,  114, 1006, 1006,  115,  116,  117,  118,  119,
  541, 1006,  974,  120,  121,  709,  707, 1189,  708, 1006,
  710,  976,  981, 1004, 1006,  985,  541, 1006,  986,  541,
  997,  541, 1013, 1016, 1033,  541, 1038,  911,  541,  915,
 1043, 1006,  848,  919,  921,  923, 1006, 1006, 1044, 1046,
 1006,  541,  541,  798, 1050, 1053, 1051, 1006,  541, 1006,
 1006,  541,  682, 1006, 1059, 1060, 1061, 1006,  709,  707,
 1006,  708, 1006,  710, 1006, 1084,  682, 1067,  705, 1006,
 1006,  947, 1006,  706,  682, 1362,  709,  707, 1071,  708,
 1077,  710, 1082, 1344, 1345, 1108, 1347, 1349, 1006, 1006,
  682, 1006, 1109, 1096, 1354, 1128, 1091,  727, 1166, 1168,
 1365,  709,  707, 1178,  708, 1006,  710, 1006, 1006, 1006,
 1170, 1190, 1006, 1367, 1204, 1006, 1006,  705, 1207,  541,
 1202, 1374, 1375,  430, 1210, 1006,  430, 1219, 1181, 1223,
 1225, 1231, 1382,  430, 1384, 1385,  714, 1244, 1247, 1006,
 1258,  541, 1006,   76,  682,  105,  675, 1267,  541, 1269,
 1279, 1289, 1287,  541, 1300, 1296,  430, 1301, 1006, 1302,
  541, 1303, 1312, 1314, 1006, 1102,  430, 1321, 1325, 1006,
 1326, 1324, 1327, 1333,  106,  107, 1334,  108,  541, 1338,
 1006, 1006,  541, 1006,  109,  110,  111,  112,  113, 1006,
  114, 1339, 1340,  115,  116,  117,  118,  119, 1342, 1343,
 1833,  120,  121, 1350, 1835, 1435, 1351, 1352,  541, 1353,
 1355,  539,  541, 1356, 1357, 1841, 1358, 1359, 1006, 1006,
  986, 1006, 1360,  908, 1377, 1380, 1386, 1401, 1006, 1006,
 1006, 1006, 1006, 1403, 1006, 1404, 1408, 1006, 1006, 1006,
 1006, 1006, 1412,  705, 1859, 1006, 1006, 1411,  706, 1409,
 1410, 1413,  539, 1420,  430,  539, 1423,  565, 1425, 1430,
 1432,  986,  986,  986,  986,  986,  999,  986, 1444, 1445,
  539, 1876,  539, 1446, 1447, 1485, 1878, 1440, 1332,  986,
  986,  986,  986, 1452, 1493, 1457,  678,  709,  707, 1448,
  708, 1498,  710, 1366,  709,  707,  705,  708, 1449,  710,
 1467,  706, 1474, 1368,  709,  707, 1478,  708, 1479,  710,
 1483,  986, 1335,  986,  705,  678,  678,  105,  678,  706,
  430, 1489, 1510, 1516,  430,  678,  678,  678,  678,  678,
 1522,  678, 1523, 1533,  678,  678,  678,  678,  678,  705,
 1517, 1592,  678,  678,  706, 1602,  106,  107, 1604,  108,
  105, 1605, 1606, 1607, 1609, 1529,  109,  110,  111,  112,
  113, 1608,  114,   98, 1611,  115,  116,  117,  118,  119,
 1612, 1613, 1623,  120,  121,  543, 1621, 1626, 1630,  106,
  107,  105,  108,  709,  707, 1631,  708, 1632,  710,  109,
  110,  111,  112,  113, 1634,  114,  589, 1635,  115,  116,
  117,  118,  119, 1636,   99,  568,  120,  121, 1637, 1640,
  106,  107, 1641,  108,  378, 1642,  100, 1643, 1644, 1645,
  109,  110,  111,  112,  113,  544,  114, 1646, 1647,  115,
  116,  117,  118,  119,  687,  230, 1648,  120,  121, 1638,
 1639,  101, 1649,  376,  530, 1651,  568, 1659, 1669,  568,
 1670, 1677, 1678,  531,  132,  378, 1650,  376,  378,  545,
  430, 1680, 1692, 1690,  568,  376,  568, 1681, 1688,  539,
 1689, 1702, 1691,  378,  687,  687,  230,  546,  687,  230,
 1697,  376,  986, 1698,  986,  986, 1701, 1704,  539,  539,
 1705,  539,  986,  687,  230,  539,  568, 1679,  539, 1727,
  986, 1732, 1731, 1736, 1743,  986, 1758, 1496,  986, 1761,
  102,  539, 1369,  709,  707, 1693,  708,  539,  710, 1769,
  103, 1784,  986, 1790,  986,  705, 1791,  986,  986, 1794,
  706,  986,  705, 1802, 1813,  376, 1808,  706,  986, 1810,
  986,  986,  705, 1815,  986,  104, 1817,  706,  986,  709,
  707,  986,  708,  986,  710,  986, 1824, 1825, 1831, 1834,
  986,  986, 1805,  986, 1855,  160, 1785, 1806, 1721, 1857,
 1860, 1370,  709,  707, 1866,  708, 1724,  710, 1793,  986,
  986, 1874,  986, 1867, 1877, 1879, 1880, 1884, 1885,  539,
 1886,  689, 1896,    1,   52,  642,  986, 1010,  986,  986,
  986,   29, 1529,  986,   31,   53,  986,  986,  203,  770,
  382,  539,   54,  590,  271, 1738,  986,  539,  539, 1740,
  148,  705,  671,  269,   81,  471,  706,  539,  753,  539,
  986,  969,  689,  986,  213,  689,  539,   55, 1010, 1010,
 1010, 1010, 1010, 1010, 1010,  193,  276,  539,  410,  986,
  689,  968,  539,  539,  708,  986, 1010, 1010, 1010, 1010,
  986,  706,  702,  568,  704,  194,  950,  275,  383,  725,
  724,  986,  986,  949,  986,  223,  328,  292,  726,  568,
  986,  539,  430,  568,  636,  568,  348,  336, 1010,  568,
 1010,  735,  568,  363,  334,   27,   30,   39, 1391,  700,
  137,  483,   56,  975,  473,  568,  474, 1322,  868,  986,
  986,  568,  986,  242,  568,  230, 1107, 1320,  250,  986,
  986,  986,  986,  986, 1318,  986, 1308, 1614,  986,  986,
  986,  986,  986, 1615,   57,  378,  986,  986,   58,  552,
  833,  551,  378,  700, 1840,  378,  597, 1246,  535,  378,
  681,  705,  687, 1297,  519,  687,  706,  378,  700, 1014,
 1309,  567,  687,  568, 1734,  687, 1048, 1703, 1804,  687,
  814, 1594, 1809,  378,  687, 1600, 1811,  687, 1434, 1848,
 1041,  551,  551,  568, 1801,  551, 1786,  705, 1683, 1755,
 1839,  681,  706,  687,  681, 1416, 1536,  378, 1768,  575,
  551, 1238,  551,  558,  818,  568, 1083,  549, 1074,  681,
  705, 1464,  568,  242,  139,  706,  568,  687,  749,  512,
  855,  687, 1005,  568,  293,  753,  999,  378, 1018, 1222,
  687, 1730,  551, 1663, 1823,  242, 1854,  982,   84, 1346,
  762,  568,  242, 1429, 1379,   85,  568,  687,    0,   86,
   87, 1215,    0,    0,  378,    0,    0,    0,   88, 1010,
    0, 1010, 1010,    0,    0,    0,  378,    0,  378, 1010,
   89,  568,    0,    0,  687,  568,  242, 1010,    0,    0,
    0,    0, 1010,    0,    0, 1010,  687,    0,  687, 1371,
  709,  707,  536,  708,    0,  710,    0,    0,    0, 1010,
    0, 1015,    0,    0, 1010, 1010,    0,    0, 1010,  689,
    0,    0,  689,    0,    0, 1010,    0, 1010, 1010,  689,
    0, 1010,  689,    0,    0, 1010,  689,    0, 1010,    0,
 1010,  689, 1010,  536,  689,   90,  536, 1010, 1010,    0,
 1010,    0, 1015, 1015, 1015, 1015, 1015, 1015, 1015,    0,
  689,  536,   91,  536,    0,    0, 1010, 1010,    0, 1010,
 1015, 1015, 1015, 1015,    0,    0,    0,   92,    0,    0,
    0,   93,    0, 1010,  689, 1010, 1010, 1010,  689,    0,
 1010,    0,   94, 1010, 1010,    0,    0,  689,    0,    0,
    0,    0, 1015, 1010, 1015,    0,    0,    0,    0,  551,
    0,   95,    0,    0,  689,    0,    0, 1010,    0,    0,
 1010,  843,    0,    0,    0,  551,    0,    0,  551,  551,
  700,  551,    0,    0,    0,  551, 1010,  700,  551,    0,
  700,  689, 1010,    0,  700,    0,    0, 1010,    0,    0,
    0,  551,  700,  689,    0,  689,    0,  551, 1010, 1010,
  551, 1010,  843,    0,    0,  843,    0, 1010,  700,    0,
    0, 1372,  709,  707,    0,  708,    0,  710,  681,    0,
  843,    0,    0,    0,    0,    0,    0,    0,  681,  541,
    0,  681,  700,    0,    0,    0, 1010, 1010,    0, 1010,
  681,    0,    0,  708,    0,    0, 1010, 1010, 1010, 1010,
 1010,    0, 1010,  231,    0, 1010, 1010, 1010, 1010, 1010,
    0,    0,  700, 1010, 1010,    0,    0,  700,    0,  551,
  541,    0,    0,  541,    0, 1373,  709,  707,  705,  708,
    0,  710,    0,  706,    0,    0,    0,  681,  541,  700,
  541,  551,    0,    0,  231,    0,  681,  231,  551,    0,
  536,  700,  551,  700,    0,    0,    0,    0,    0,  551,
    0,    0,  231, 1015,    0, 1015, 1015,    0,    0,  536,
  536,    0,  536, 1015,    0,    0,  536,  551,    0,  536,
    0, 1015,  551,    0,    0,    0, 1015,    0,    0, 1015,
  681,    0,  536, 1511,  709,  707,  508,  708,  536,  710,
    0,    0,  681, 1015,  681, 1011,    0,  551, 1015, 1015,
    0,  551, 1015,    0, 1512,  709,  707,    0,  708, 1015,
  710, 1015, 1015,    0,    0, 1015,    0,    0,    0, 1015,
    0,    0, 1015,    0, 1015,    0, 1015,  508,    0,    0,
  508, 1015, 1015,    0, 1015,    0, 1011, 1011, 1011, 1011,
 1011, 1011, 1011,    0,    0,  508,    0,    0,    0,    0,
 1015, 1015,    0, 1015, 1011, 1011, 1011, 1011,    0,    0,
  536,    0,    0,  843,    0,    0,    0, 1015,    0, 1015,
 1015, 1015,    0,    0, 1015,    0,    0, 1015, 1015,    0,
    0,    0,  536,    0,    0,    0, 1011, 1015, 1011,  536,
  705,    0,    0,    0,    0,  706,    0,    0,    0,    0,
  536, 1015,    0,    0, 1015,    0, 1519,  709,  707,    0,
  708,    0,  710,    0,    0,    0,    0,    0,  536,  843,
 1015,    0,  843,  536,    0,    0, 1015,  541,    0,  843,
    0, 1015,  843,    0,    0,    0,  843,    0,    0,    0,
    0,  843, 1015, 1015,  843, 1015,    0,  541,    0,  541,
    0, 1015,  536,  541,  705,    0,  541,    0,    0,  706,
  843,  924,  925,  926,  927,  928,  929,  930,  931,  541,
    0,  242,    0,  231,    0,  541,    0,    0,    0,    0,
 1015, 1015,    0, 1015,  843,    0,    0,  843,  843,    0,
 1015, 1015, 1015, 1015, 1015,    0, 1015,  843,    0, 1015,
 1015, 1015, 1015, 1015,    0,    0,    0, 1015, 1015,  943,
  709,  707,    0,  708,  843,  710,    0,    0,    0,    0,
    0,    0,  705,    0,    0,    0,    0,  706,  906,  905,
  907,    0,    0,    0,    0,    0,  843,    0,    0,    0,
    0,  843,    0,  705,    0,    0,    0,  541,  706,  565,
    0,    0,    0,  843,    0,  843,    0, 1011,    0, 1011,
 1011,    0,    0,    0,  508,    0,  508, 1011,    0,  541,
    0,  242,    0,  508,    0, 1011,  541,    0,    0,    0,
 1011,    0,    0, 1011,    0,    0,  508,  541,    0,    0,
    0,    0,    0,  242,    0,    0,    0, 1011,    0,  864,
  242,    0, 1011, 1011,    0,  541, 1011,    0,    0,    0,
  541,    0,    0, 1011,    0, 1011, 1011,    0,    0, 1011,
    0,    0,    0, 1011,  541,    0, 1011,    0, 1011,    0,
 1011, 1249,    0,    0,  242, 1011, 1011,    0, 1011,  541,
  864,  864,  864,  864,  864,  705,  864,    0,    0,    0,
  706,    0,    0,    0, 1011, 1011,    0, 1011,  864,  864,
  864,  864,    0,    0,  508,  541,    0,    0,  541,    0,
    0, 1011,    0, 1011, 1011, 1011,    0,    0, 1011,    0,
    0, 1011, 1011,  541,    0,    0,  508,    0,    0,    0,
    0, 1011,  864,  508, 1486, 1487,    0,    0,    0,    0,
 1491, 1492,    0, 1494, 1495, 1011,    0,    0, 1011,    0,
    0, 1499, 1500, 1501, 1502,    0, 1503, 1504, 1505, 1506,
 1507, 1508,  105,    0, 1011,    0,    0,  508,    0,    0,
 1011,    0,    0,    0,    0, 1011,    0,    0,    0,    0,
  893,    0,    0,    0,    0,    0, 1011, 1011,  705, 1011,
    0,  106,  107,  706,  108, 1011,  894,    0,    0,    0,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,  223,  251,  120,  121,
    0,    0,    0,    0, 1011, 1011,    0, 1011,    0,    0,
    0,    0,  895,    0, 1011, 1011, 1011, 1011, 1011,    0,
 1011,  896,    0, 1011, 1011, 1011, 1011, 1011,    0,  897,
    0, 1011, 1011,    0,    0,    0,    0,  223,  251,    0,
  223,  251,    0,    0,    0,    0,    0,    0,  898,    0,
  899,    0,    0,    0,    0,  223,  251,    0,    0,    0,
    0,    0,    0,    0,  900,    0,  901,  902,  903,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   62,
    0,  864,    0,  864,  864,    0,    0,    0,    0,    0,
   63,  864,    0,    0,    0,    0,    0,   64,    0,  864,
    0,  391,    0,    0,  864,    0,    0,  864,    0,    0,
   65,    0,    0,    0,    0,    0,    0,  904,    0,    0,
    0,  864,  541,  876,  541,    0,  864,  864,    0,  393,
  864,  541,    0,    0,    0,    0,    0,  864,    0,  864,
  864,  571,    0,  864,  541,    0,    0,  864,    0,    0,
  864,    0,  864,    0,  864,    0,    0,    0,  395,  864,
  864,    0,  864,    0,  876,  876,  876,  876,  876,    0,
  876,   66,    0,    0,    0,    0,    0,    0,  864,  864,
    0,  864,  876,  876,  876,  876,    0,   67,    0,    0,
    0,    0,    0,    0,    0,  864,    0,  864,  864,  864,
    0,    0,  864,    0,   68,  864,  864,    0,    0,    0,
    0,    0,    0,    0,    0,  864,  876,    0,    0,   69,
    0,    0,  541,   70,    0,    0,   71,    0,    0,  864,
    0,    0,  864,  400,   72,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  541,    0,    0,    0,  864,  402,
    0,  541,    0,   73,  864,    0,    0,    0,    0,  864,
    0,    0,    0,  403,  404,  405,    0,  207,  208,    0,
  864,  864,    0,  864,  223,  251,  223,  251,    0,  864,
    0,    0,    0,  223,  251,  541,  408,  409,  410,  411,
  412,  413,  414,  415,  416,    0,    0,    0,    0,    0,
    0,  418,  419,    0,    0,    0,    0,    0,  864,  864,
  423,  864,    0,    0,  105,    0,    0,    0,  864,  864,
  864,  864,  864,    0,  864,    0,    0,  864,  864,  864,
  864,  864,    0,    0,    0,  864,  864,    0,    0,  879,
    0,    0,    0,  106,  107,    0,  108,    0,    0,    0,
    0,    0,    0,  109,  110,  111,  112,  113,    0,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,  223,  251,    0,    0,    0,    0,
  879,  879,  879,  879,  879,  876,  879,  876,  876,    0,
    0,    0,    0,    0,    0,  876,  223,  251,  879,  879,
  879,  879,    0,  223,  251,    0,    0,  962,  876,    0,
    0,  876,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  876,    0,    0,    0,    0,
  876,  876,  879,    0,  876,    0,    0,  223,  251,    0,
    0,  876,    0,  876,  876,    0,    0,  876,    0,    0,
    0,  876,    0,    0,  876,    0,  876,    0,  876,    0,
    0,    0,    0,  876,  876,    0,  876,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  552,    0,    0,    0,
    0,    0,  876,  876,  692,  876,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  876,
    0,  876,  876,  876,    0,    0,  876,    0,    0,  876,
  876,    0,    0,    0,    0,    0,  552,  552,    0,  876,
  552,    0,  965,    0,    0,  692,    0,    0,  692,    0,
    0,    0,    0,  876,    0,  552,  876,  552,    0,  105,
    0,    0,    0,  692,    0,    0,    0,    0,    0,    0,
    0,    0,  876,    0,    0,    0,    0,    0,  876,    0,
    0,    0,    0,  876,    0,    0,    0,  552,  106,  107,
    0,  108,    0,    0,  876,  876,    0,  876,  109,  110,
  111,  112,  113,  876,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,  879,    0,  879,  879,    0,    0,    0,    0,    0,
    0,  879,  876,  876,    0,  876,    0,    0,    0,    0,
    0,    0,  876,  876,  876,  876,  876,  879,  876,    0,
    0,  876,  876,  876,  876,  876,    0,    0,    0,  876,
  876,  879,    0,  880,    0,    0,  879,  879,    0,    0,
  879,    0,    0,    0,    0,    0,    0,  879,    0,  879,
  879,    0,    0,  879,    0,    0,    0,  879,    0,    0,
  879,    0,  879,    0,  879,    0,    0,    0,    0,  879,
  879,    0,  879,    0,  880,  880,  880,  880,  880,    0,
  880,    0,    0,    0,    0,    0,    0,    0,  879,  879,
    0,  879,  880,  880,  880,  880,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  879,    0,  879,  879,  879,
    0,    0,  879,    0,    0,  879,  879,    0,    0,    0,
    0,    0,    0,    0,  552,  879,  880,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  879,
  552,    0,  879,  552,  552,    0,  552,    0,    0,    0,
  552,    0,    0,  552,    0,    0,    0,    0,  879,    0,
    0,    0,    0,    0,  879,    0,  552,    0,    0,  879,
    0,    0,  552,    0,    0,  552,    0,    0,    0,    0,
  879,  879,    0,  879,    0,    0,    0,    0,    0,  879,
    0,    0,  692,    0,    0,  692,    0,    0,    0,    0,
    0,    0,  692,    0,    0,  692,    0,    0,    0,  692,
    0,    0,    0,    0,  692,    0,    0,  692,  879,  879,
    0,  879,    0,    0,    0,    0,    0,    0,  879,  879,
  879,  879,  879,  692,  879,    0,    0,  879,  879,  879,
  879,  879,    0,    0,  552,  879,  879,    0,    0,  874,
    0,    0,    0,    0,    0,    0,    0,  692,    0,    0,
    0,  692,    0,    0,    0,    0,  552,    0,    0,    0,
  692,    0,    0,  552,    0,    0,    0,  552,    0,    0,
    0,    0,    0,    0,  552,    0,    0,  692,    0,    0,
  874,    0,  874,  874,  874,  880,    0,  880,  880,    0,
    0,    0,  552,    0,    0,  880,    0,  552,  874,  874,
  874,  874,    0,    0,  692,    0,    0,    0,    0,    0,
    0,  880,    0,    0,    0,    0,  692,    0,  692,    0,
    0,    0,  552,    0,    0,  880,  552,    0,    0,    0,
  880,  880,  874,    0,  880,    0,    0,    0,    0,    0,
    0,  880,    0,  880,  880,    0,    0,  880,    0,    0,
    0,  880,    0,    0,  880,    0,  880,    0,  880,    0,
    0,    0,    0,  880,  880,    0,  880,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  546,    0,    0,    0,
    0,    0,  880,  880,    0,  880,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  880,
    0,  880,  880,  880,    0,    0,  880,    0,    0,  880,
  880,    0,    0,  464,    0,    0,    0,  546,    0,  880,
  546,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  880,    0,  546,  880,  546,    0,  105,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  880,    0,    0,    0,    0,    0,  880,    0,
    0,    0,    0,  880,    0,    0,   98,  546,  106,  107,
    0,  108,    0,    0,  880,  880,    0,  880,  109,  110,
  111,  112,  113,  880,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,  874,    0,  874,  874,    0,    0,   99,    0,    0,
    0,  874,  880,  880,    0,  880,    0,    0,    0,  100,
    0,    0,  880,  880,  880,  880,  880,  874,  880,    0,
    0,  880,  880,  880,  880,  880,    0,    0,    0,  880,
  880,  874,    0,  873,  101,    0,  874,  874,    0,    0,
  874,    0,    0,    0,    0,    0,    0,  874,    0,  874,
  874,    0,    0,  874,    0,    0,    0,  874,    0,    0,
  874,    0,  874,    0,  874,    0,    0,    0,    0,  874,
  874,    0,  874,    0,  873,    0,  873,  873,  873,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  874,  874,
    0,  874,  873,  873,  873,  873,    0,    0,    0,    0,
    0,    0,    0,  102,    0,  874,    0,  874,  874,  874,
    0,    0,  874,  103,    0,  874,  874,    0,    0,    0,
    0,    0,    0,    0,  546,  874,  873,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  104,  874,
  546,    0,  874,  546,  546,    0,  546,    0,    0,    0,
  546,    0,    0,  546,    0,    0,  105,    0,  874,    0,
    0,    0,    0,    0,  874,    0,  546,    0,    0,  874,
    0,    0,  546,    0,    0,  546,    0,    0,    0,    0,
  874,  874,    0,  874,    0,  106,  107,    0,  108,  874,
    0,    0,    0,    0,    0,  109,  110,  111,  112,  113,
    0,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,    0,    0,    0,  874,  874,
    0,  874,    0,    0,    0,    0,    0,    0,  874,  874,
  874,  874,  874,    0,  874,    0,    0,  874,  874,  874,
  874,  874,    0,    0,  546,  874,  874,    0,    0,  875,
  221,    0,    0,    0,  221,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  546,    0,  221,    0,
    0,    0,  221,  546,    0,  221,    0,  546,    0,    0,
    0,    0,    0,    0,  546,    0,    0,    0,    0,    0,
  875,    0,  875,  875,  875,  873,    0,  873,  873,    0,
    0,    0,  546,    0,    0,  873,    0,  546,  875,  875,
  875,  875,    0,    0,    0,    0,  221,    0,    0,    0,
    0,  873,  221,  221,  221,    0,  221,  221,  221,    0,
  221,    0,  546,    0,    0,  873,  546,    0,    0,    0,
  873,  873,  875,    0,  873,    0,    0,    0,    0,    0,
  221,  873,  221,  873,  873,    0,    0,  873,    0,    0,
    0,  873,    0,    0,  873,    0,  873,    0,  873,    0,
    0,    0,    0,  873,  873,    0,  873,    0,    0,    0,
    0,    0,    0,  221,    0,    0,  812,    0,   80,    0,
    0,    0,  873,  873,    0,  873,    0,  788,    0,    0,
  221,    0,    0,    0,    0,    0,    0,    0,    0,  873,
    0,  873,  873,  873,    0,    0,  873,    0,    0,  873,
  873,    0,  221,    0,    0,    0,    0,  812,    0,  873,
  812,    0,  468,    0,    0,    0,    0,    0,  788,    0,
    0,  788,    0,  873,    0,  812,  873,    0,    0,  105,
    0,    0,    0,    0,    0,    0,  788,  221,  221,    0,
    0,    0,  873,    0,    0,    0,    0,    0,  873,    0,
    0,    0,    0,  873,  221,    0,    0,    0,  106,  107,
    0,  108,    0,    0,  873,  873,    0,  873,  109,  110,
  111,  112,  113,  873,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0,    0,    0,
    0,  875,    0,  875,  875,    0,    0,    0,    0,    0,
    0,  875,  873,  873,    0,  873,    0,    0,    0,    0,
    0,    0,  873,  873,  873,  873,  873,  875,  873,    0,
    0,  873,  873,  873,  873,  873,    0,    0,    0,  873,
  873,  875,    0,  878,    0,    0,  875,  875,    0,    0,
  875,    0,    0,    0,    0,    0,    0,  875,    0,  875,
  875,    0,    0,  875,    0,    0,    0,  875,    0,    0,
  875,    0,  875,    0,  875,    0,    0,    0,    0,  875,
  875,    0,  875,    0,  878,    0,  878,  878,  878,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  875,  875,
    0,  875,  878,  878,  878,  878,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  875,    0,  875,  875,  875,
    0,    0,  875,  695,    0,  875,  875,    0,    0,    0,
    0,    0,    0,    0,    0,  875,  878,    0,  812,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  875,
    0,    0,  875,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  695,    0,    0,  695,  875,    0,
    0,    0,    0,    0,  875,    0,    0,    0,    0,  875,
    0,    0,  695,    0,    0,    0,    0,    0,    0,    0,
  875,  875,    0,  875,  812,    0,    0,  812,    0,  875,
    0,    0,    0,    0,  812,  788,    0,  812,  788,    0,
    0,  812,    0,    0,    0,  788,  812,    0,  788,  812,
    0,    0,  788,    0,    0,    0,    0,  788,  875,  875,
  788,  875,    0,    0,    0,  812,  105,    0,  875,  875,
  875,  875,  875,    0,  875,    0,  788,  875,  875,  875,
  875,  875,    0,    0,    0,  875,  875,    0,    0,  812,
    0,    0,  812,  812,    0,  106,  107,    0,  108,    0,
  788,    0,  812,  788,  788,  109,  110,  111,  112,  113,
    0,  114,    0,  788,  115,  116,  117,  118,  119,  812,
    0,    0,  120,  121,    0,    0,    0,    0,    0,    0,
  788,    0,    0,    0,    0,  878,    0,  878,  878,    0,
    0,  812,    0,    0,    0,  878,  812,    0,    0,    0,
    0,    0,  788,    0,    0,    0,    0,  788,  812,    0,
  812,  878,    0,    0,    0,  661,    0,    0,  424,  788,
  425,  788,    0,    0,    0,  878,    0,    0,    0,    0,
  878,  878,    0,  427,  878,    0,    0,    0,  282,    0,
    0,  878,    0,  878,  878,    0,    0,  878,    0,    0,
    0,  878,    0,    0,  878,    0,  878,    0,  878,    0,
    0,    0,    0,  878,  878,    0,  878,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  878,  878,    0,  878,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  878,
    0,  878,  878,  878,    0,    0,  878,    0,    0,  878,
  878,    0,  661,    0,    0,  424,    0,  425,    0,  878,
    0,  695,    0,    0,  695,    0,    0,    0,    0,    0,
  427,  695,    0,  878,  695,  282,  878,    0,  695,    0,
    0,    0,    0,  695,    0,    0,  695,    0,    0,    0,
    0,    0,  878,    0,    0,    0,    0,    0,  878,    0,
    0,    0,  695,  878,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  878,  878,    0,  878,    0,    0,
    0,    0,    0,  878,    0,    0,  695,    0,    0,    0,
  695,    0,    0,    0,    0,    0,    0,    0,    0,  695,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  878,  878,    0,  878,  695,    0,    0,    0,
    0,    0,  878,  878,  878,  878,  878,    0,  878,    0,
    0,  878,  878,  878,  878,  878,    0,    0,    0,  878,
  878,    0,  388,  695,    0,    0,    0,    0,    0,    0,
    0,  389,    0,    0,    0,  695,    0,  695,    0,    0,
    0,    0,    0,  678,  390,    0,    0,    0,    0,    0,
  391,  392,    0,    0,    0,    0,    0,  813,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  655,
    0,    0,  678,  678,    0,  678,    0,    0,  393,    0,
    0,  394,  678,  678,  678,  678,  678,    0,  678,    0,
    0,  678,  678,  678,  678,  678,    0,    0,  813,  678,
  678,  813,    0,    0,    0,    0,    0,    0,    0,  396,
  397,    0,    0,    0,    0,    0,  813,    0,    0,  388,
    0,  398,    0,    0,    0,  399,    0,    0,  389,    0,
    0,    0,    0,  885,    0,    0,    0,    0,    0,    0,
    0,  390,    0,    0,  654,    0,    0,  391,  392,    0,
  662,    0,  662,  662,    0,  662,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  655,    0,  662,    0,
    0,    0,  698,  662,    0,  393,  657,    0,  394,    0,
    0,    0,  400,    0,    0,    0,    0,    0,  401,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  402,    0,
    0,    0,    0,    0,    0,    0,  396,  397,    0,    0,
    0,    0,  403,  404,  405,  406,  207,  208,  398,    0,
    0,    0,  399,    0,    0,    0,  407,    0,    0,    0,
  656,    0,    0,    0,    0,  408,  409,  410,  411,  412,
  413,  414,  415,  416,  106,  417,    0,  108,  584,    0,
  418,  419,    0,  420,  421,  422,  111,  112,  113,  423,
  114,    0,    0,  115,  116,  117,  118,  119,  886,  887,
  888,  120,  121,  657,    0,    0,    0,  584,  584,  400,
  584,    0,    0,    0,    0,  401,    0,  584,  584,  584,
  584,  584,    0,  584,    0,  402,  584,  584,  584,  584,
  584,    0,    0,    0,  584,  584,    0,    0,    0,  403,
  404,  405,  406,  207,  208,    0,    0,    0,    0,  813,
    0,    0,    0,  407,    0,    0,    0,    0,    0,    0,
    0,    0,  408,  409,  410,  411,  412,  413,  414,  415,
  416,  106,  417,    0,  108,    0,    0,  418,  419,    0,
  420,  421,  422,  111,  112,  113,  423,  114,    0,    0,
  115,  116,  658,  659,  660,    0,    0,    0,  120,  121,
  662,    0,    0,    0,    0,  813,    0,  662,  813,    0,
    0,    0,    0,    0,    0,  813,  662,    0,  813,    0,
    0,    0,  813,    0,    0,    0,    0,  813,    0,  662,
  813,    0,    0,    0,    0,  662,  662,    0,    0,  656,
    0,  656,  656,  662,  656,    0,  813,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  656,    0,    0,
    0,    0,  656,  662,    0,    0,  662,    0,    0,    0,
  813,    0,    0,  813,  813,    0,    0,    0,    0,  837,
    0,    0,    0,  813,    0,    0,    0,    0,    0,    0,
    0,    0,  662,    0,  662,  662,    0,    0,    0,    0,
  813,    0,    0,    0,    0,    0,  662,    0,    0,    0,
  662,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  837,    0,  813,  837,    0,  661,    1,  813,  424,    0,
  425,    0,    0,    0,    0,    0,    0,    0,  837,  813,
    0,  813,    0,  427,    0,    0,    0,    2,  282,    0,
    0,    0,    0,    0,    0,    3,    0,    0,    0,    0,
    0,    0,    4,    0,    5,    0,    6,  662,    0,    0,
 1660,    7,    0,  662,    0,    0,    0,    0,    8,    0,
    0,    0,    0,  662,    0,    0,    0,    0,    0,    0,
    0,    0,    9,    0,    0, 1661,    0,  662,  662,  662,
  662,  662,  662,   10,    0,    0,    0,    0,    0,    0,
    0,  662,    0,    0,    0,    0,    0,    0,    0,    0,
  662,  662,  662,  662,  662,  662,  662,  662,  662,  662,
  662,    0,  662,    0,    0,  662,  662,    0,  662,  662,
  662,  662,  662,  662,  662,  662,    0,    0,  662,  662,
  662,  662,  662, 1662,    0,    0,  662,  662,    0,  656,
    0,    0,    0,    0,    0,    0,  656,    0,    0,    0,
   11,    0,    0,    0,    0,  656,    0,   12,    0,    0,
   13,    0,   14,   15,    0,    0,    0,  105,  656,    0,
    0,    0,    0,    0,  656,  656,    0,    0,    0,    0,
    0,    0,  656,    0,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0,  106,  107,    0,  108,
    0,    0,  656,    0,    0,  656,  109,  110,  111,  112,
  113,    0,  114,    0,    0,  115,  116,  117,  118,  119,
    0,  837,    0,  120,  121,    0,    0,    0,    0,    0,
    0,  656,  388,  656,  656,  661,   17,    0,  424,    0,
  425,  389,    0,    0,    0,  656,    0,    0,    0,  656,
    0,   18,   19,  427,  390,    0,    0,    0,  282,    0,
  391,  392,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  837,    0,  655,
  837,    0,    0,    0,    0,    0,    0,  837,  393,    0,
  837,  394,    0,    0,  837,    0,    0,    0,    0,  837,
    0,    0,  837,    0,    0,    0,  656,    0,    0,    0,
    0,    0,  656,    0,    0,    0,    0,    0,  837,  396,
  397,    0,  656,    0,    0,    0,    0,    0,    0,    0,
    0,  398,    0,    0,    0,  399,  656,  656,  656,  656,
  656,  656,  837,  881,    0,  837,  837,    0,    0,    0,
  656,    0,    0,    0,    0,  837,    0,    0,    0,  656,
  656,  656,  656,  656,  656,  656,  656,  656,  656,  656,
    0,  656,  837,    0,  656,  656,    0,  656,  656,  656,
  656,  656,  656,  656,  656,    0,  657,  656,  656,  656,
  656,  656,  400,    0,  837,  656,  656,    0,  401,  837,
    0,    0,    0,    0,    0,    0,    0,    0,  402,    0,
    0,  837,    0,  837,    0,    0,    0,    0,    0,    0,
    0,    0,  403,  404,  405,  406,  207,  208,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,    0,    0,
    0,    0,    0,    0,    0,  408,  409,  410,  411,  412,
  413,  414,  415,  416,  106,  417,    0,  108,    0,    0,
  418,  419,  388,  420,  421,  422,  111,  112,  113,  423,
  114,  389,    0,  115,  116,  658,  659,  660,    0,    0,
    0,  120,  121,  661,  390,    0,  424,    0,  425,    0,
  391,  392,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  427,    0,    0,    0,    0,  282,    0,    0,  655,
    0,    0,    0,    0,    0,    0,    0,    0,  393,    0,
    0,  394,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  709,  707,    0,  708,    0,
  710,    0,    0,    0,    0,    0,    0,    0,    0,  396,
  397,    0,    0,  906,  905,  907,    0,    0,    0,    0,
    0,  398,    0,    0,    0,  399,    0,    0,  871,    0,
    0,    0,  599,  656,    0,    0,    0,    0,    0,    0,
  488,    0,    0,  424,    0,  425,  872,    0,    0,    0,
  600,    0,    0,  601,    0,    0,    0,    0,  427,    0,
    0,    0,    0,  282,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  657,    0,    0,    0,
    0,    0,  400,    0,    0,    0,    0,    0,  401,    0,
    0,    0,    0,    0,  602,    0,    0,    0,  402,    0,
  603,  604,  605,    0,  606,  607,  608,    0,  609,    0,
    0,    0,  403,  404,  405,  406,  207,  208,    0,    0,
    0,    0,    0,    0,    0,    0,  407,    0,  610,    0,
  611,    0,    0,    0,    0,  408,  409,  410,  411,  412,
  413,  414,  415,  416,  106,  417,    0,  108,    0,    0,
  418,  419,    0,  420,  421,  422,  111,  112,  113,  423,
  114,  612,    0,  115,  116,  658,  659,  660,    0,    0,
  388,  120,  121,    0,    0,    0,    0,    0,  613,  389,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  390,    0,    0,    0,    0,    0,  391,  392,
  614,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  655,    0,    0,
    0,    0,    0,    0,    0,  893,  393,    0,    0,  394,
    0,    0,    0,  705,    0,  615,  616,    0,  706,    0,
    0,  894,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  617,    0,    0,    0,    0,  396,  397,    0,
 1123,  558,    0, 1124,    0,    0,    0,  388,    0,  398,
    0,    0,    0,  399,    0,    0,  389,  895,    0,    0,
    0,  881,    0,    0,    0,    0,  896,    0,    0,  390,
    0,    0,    0,    0,  897,  391,  392,    0,  488,    0,
    0,  424,  558,  425,    0,  558,    0,    0,    0,    0,
    0,    0,    0,  898,    0,  899,  427,    0,    0,    0,
  558,  282,  558,  393,  657,    0,  394,    0,    0,  900,
  400,  901,  902,  903,    0,    0,  401,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  402,    0,    0,    0,
    0,    0,  558,    0,  396,  397,    0,    0,    0,    0,
  403,  404,  405,  406,  207,  208,  398,    0,    0,    0,
  399,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,  904,  408,  409,  410,  411,  412,  413,  414,
  415,  416,  106,  417,    0,  108,    0,    0,  418,  419,
    0,  420,  421,  422,  111,  112,  113,  423,  114,    0,
    0,  115,  116,  117,  118,  119,    0,    0,    0,  120,
  121,    0,    0,    0,    0, 1125,    0,  400,    0,    0,
    0,    0,    0,  401,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  402,    0,    0,    0,    0,    0,    0,
  488,    0,    0,  424,    0,  425,    0,  403,  404,  405,
  406,  207,  208,    0,    0,    0,    0,    0,  427,    0,
  962,  407,    0,  282,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  412,  413,  414,  415,  416,  106,
  417,    0,  108,    0,    0,  418,  419,    0,  420,  421,
  422,  111,  112,  113,  423,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121, 1133,  558,
    0, 1124,    0,    0,    0,  388,    0,    0,    0,    0,
    0,    0,    0,    0,  389,  558,    0,    0,    0,  558,
    0,  558,    0,    0,  963,  558,    0,  390,  558,    0,
    0,    0,    0,  391,  392,    0,    0,    0,    0,  488,
    0,  558,  424,    0,  425,    0,  284,  558,    0,    0,
  558,    0,    0,    0,    0,    0,    0,  427,    0,    0,
  964,  393,  282,    0,  394,  965,    0,    0,    0,    0,
    0,  966,    0,    0,    0,  284,  284,    0,  284,    0,
    0,    0,  105,    0,    0,  284,  284,  284,  284,  284,
  967,  284,  396,  397,  284,  284,  284,  284,  284,    0,
    0,    0,  284,  284,  398,    0,    0,    0,  399,    0,
    0,  106,  107,    0,  108,    0,    0,    0,    0,  558,
    0,  109,  110,  111,  112,  113,    0,  114,    0,    0,
  115,  116,  117,  118,  119,    0,    0,    0,  120,  121,
    0,  558,    0,    0,    0,    0,    0,    0,  558,    0,
 1137,    0,  558, 1124,    0,    0,    0,  388,    0,  558,
    0,    0,    0, 1125,    0,  400,  389,    0,    0,    0,
    0,  401,    0,    0,    0,    0,    0,  558,    0,  390,
    0,  402,  558,    0,    0,  391,  392,    0,    0,    0,
    0,    0,    0,    0,    0,  403,  404,  405,  406,  207,
  208,    0,    0,    0,    0,    0,    0,  558,    0,  407,
    0,  558,    0,  393,    0,    0,  394,    0,  408,  409,
  410,  411,  412,  413,  414,  415,  416,  106,  417,    0,
  108,    0,    0,  418,  419,    0,  420,  421,  422,  111,
  112,  113,  423,  114,  396,  397,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,  398,    0,    0, 1145,
  399,    0, 1124,    0,    0,    0,  388,    0,    0,    0,
    0,    0,    0,    0,    0,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,    0,    0,    0,  391,  392,    0,    0,  488,    0,
    0,  424,    0,  425,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, 1125,  427,  400,    0,    0,
    0,  282,  393,  401,    0,  394,    0,    0,    0,    0,
    0,    0,    0,  402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  403,  404,  405,
  406,  207,  208,  396,  397,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,  398,    0,    0,    0,  399,
  408,  409,  410,  411,  412,  413,  414,  415,  416,  106,
  417,    0,  108,    0,    0,  418,  419,    0,  420,  421,
  422,  111,  112,  113,  423,  114,    0,    0,  115,  116,
  117,  118,  119,    0, 1432,    0,  120,  121,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1125,    0,  400,    0,    0,    0,
    0,    0,  401,    0,    6,    0,    0,    0,    0,    0,
    0,    0,  402,    0,    0, 1549, 1550,    0, 1551,    0,
  488,    0,    0,  424,    0,  425,  403,  404,  405,  406,
  207,  208,    0,    0,    0, 1552,    0,    0,  427,    0,
  407, 1553,    0,  282,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  412,  413,  414,  415,  416,  106,  417,
    0,  108,    0,    0,  418,  419,    0,  420,  421,  422,
  111,  112,  113,  423,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,    0,  120,  121,    0, 1148,    0,
    0, 1124,    0,    0,    0,  388,    0,    0,    0,    0,
    0, 1554,    0,    0,  389,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  390,    0,    0,
   14,    0,    0,  391,  392,    0,    0,    0,    0,  488,
    0,    0,  424,    0,  425, 1555,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,  427,    0,    0,
    0,  393,  282,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,  105,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,    0,    0,    0,    0,    0,    0,
    0,    0, 1556,  107,  398,  108,    0,    0,  399,    0,
    0,    0,  109,  110,  111,  112,  113,    0,  114,    0,
    0,  115, 1557,  117,  118,  119,    0,    0,    0,  120,
  121, 1558, 1559, 1560,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 1153,    0,    0, 1124,    0,    0,    0,  388,    0,    0,
    0,    0,    0, 1125,    0,  400,  389,    0,    0,    0,
    0,  401,    0,    0,    0,    0,    0,    0,    0,  390,
    0,  402,    0,    0,    0,  391,  392,    0,    0,    0,
    0,    0,    0,    0,    0,  403,  404,  405,  406,  207,
  208,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,    0,  393,    0,    0,  394,    0,  408,  409,
  410,  411,  412,  413,  414,  415,  416,  106,  417,    0,
  108,    0,    0,  418,  419,    0,  420,  421,  422,  111,
  112,  113,  423,  114,  396,  397,  115,  116,  117,  118,
  119,    0,  816,    0,  120,  121,  398,    0,    0, 1156,
  399,    0, 1124,    0,    0,    0,  388,    0,    0,    0,
    0,    0,    0,    0,    0,  389,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  390,    0,
    0,    0,    0,  816,  391,  392,  816,    0,  488,    0,
    0,  424,    0,  425,    0,    0,    0,    0,    0,    0,
    0,  816,    0,    0,    0, 1125,  427,  400,    0,    0,
    0,  282,  393,  401,    0,  394,    0,    0,    0,    0,
    0,    0,    0,  402,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  403,  404,  405,
  406,  207,  208,  396,  397,    0,    0,    0,    0,    0,
    0,  407,    0,    0,    0,  398,    0,    0,    0,  399,
  408,  409,  410,  411,  412,  413,  414,  415,  416,  106,
  417,    0,  108,    0,    0,  418,  419,    0,  420,  421,
  422,  111,  112,  113,  423,  114,    0,    0,  115,  116,
  117,  118,  119,    0,    0,    0,  120,  121,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, 1125,    0,  400,    0,    0,    0,
    0,    0,  401,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  402,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  403,  404,  405,  406,
  207,  208,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  412,  413,  414,  415,  416,  106,  417,
    0,  108,    0,    0,  418,  419,    0,  420,  421,  422,
  111,  112,  113,  423,  114,    0,    0,  115,  116,  117,
  118,  119,    0,    0,  816,  120,  121,    0, 1159,    0,
    0, 1124,    0,    0,    0,  388,    0,    0,  488,    0,
  493,  424,    0,  425,  389,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  427,  390,    0,    0,
    0,  282,    0,  391,  392,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  816,    0,    0,  816,    0,    0,    0,    0,    0,    0,
  816,  393,    0,  816,  394,    0,    0,  816,    0,    0,
    0,    0,  816,    0,    0,  816,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  816,  396,  397,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  398,    0,  488,    0,  399,  424,
    0,  425,    0,    0,    0,  816,    0,    0,  816,  816,
    0,    0,    0,    0,  427,    0,    0,    0,  816,  282,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  816,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1125,    0,  400,    0,  816,    0,    0,
    0,  401,  816,    0,    0,    0,    0,    0,    0,    0,
    0,  402,    0,    0,  816,    0,  816,    0,    0,    0,
    0,    0,    0,    0,    0,  403,  404,  405,  406,  207,
  208,    0,    0,    0,    0,    0,    0,  598,    0,  407,
    0,  599,    0,    0,    0,    0,    0,    0,  408,  409,
  410,  411,  412,  413,  414,  415,  416,  106,  417,  600,
  108,    0,  601,  418,  419,  388,  420,  421,  422,  111,
  112,  113,  423,  114,  389,    0,  115,  116,  117,  118,
  119,    0,    0,    0,  120,  121,    0,  390,    0,    0,
    0,    0,    0,  391,  392,    0,    0,  814,    0,    0,
    0,    0,    0,  602,    0,    0,    0,    0,    0,  603,
  604,  605,    0,  606,  607,  608,    0,  609,    0,    0,
    0,  393,    0,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  610,  814,  611,
    0,  814,    0,    0,    0,    0,    0,    0,    0,    0,
  395,    0,  396,  397,    0,    0,  814,    0,    0,    0,
    0,    0,    0,  388,  398,    0,    0,    0,  399,    0,
  612,    0,  389,    0,    0,  980,    0,  980,  980,    0,
  980,    0,    0,    0,    0,  390,    0,  613,    0,    0,
    0,  391,  392,  980,    0,    0,    0,    0,  980,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  614,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  393,
    0,    0,  394,    0,    0,  400,    0,    0,    0,    0,
    0,  401,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  402,    0,    0,  615,  616,    0,    0,  395,    0,
  396,  397,    0,    0,    0,  403,  404,  405,  406,  207,
  208,  617,  398,    0,    0,    0,  399,    0,    0,  407,
    0,    0,    0,    0,    0,    0,    0,    0,  408,  409,
  410,  411,  412,  413,  414,  415,  416,  106,  417,    0,
  108,    0,    0,  418,  419,    0,  420,  421,  422,  111,
  112,  113,  423,  114,    0,    0,  115,  116,  117,  118,
  119,    0,    0,  698,  120,  121,    0,    0,    0,    0,
    0,    0,    0,  400,    0,    0,    0,  662,    0,  401,
  662,    0,  662,    0,    0,    0,    0,    0,    0,  402,
    0,    0,    0,    0,    0,  662,    0,    0,    0,    0,
  662,    0,    0,  403,  404,  405,  406,  207,  208,  814,
    0,    0,    0,    0,    0,    0,    0,  407,    0,    0,
    0,    0,    0,    0,    0,    0,  408,  409,  410,  411,
  412,  413,  414,  415,  416,  106,  417,    0,  108,    0,
    0,  418,  419,    0,  420,  421,  422,  111,  112,  113,
  423,  114,  980,    0,  115,  116,  117,  118,  119,    0,
    0,  980,  120,  121,    0,  814,    0,    0,  814,    0,
    0,    0,  815,    0,  980,  814,    0,    0,  814,    0,
  980,  980,  814,    0,    0,    0,    0,  814,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  814,    0,  980,    0,
    0,  980,    0,  815,    0,    0,  815,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  814,  815,    0,  814,  814,    0,    0,  980,    0,  980,
  980,    0,    0,  814,    0,    0,    0,    0,    0,    0,
    0,  980,  845,    0,    0,  980,    0,    0,    0,    0,
  814,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  814,    0,    0,    0,    0,  814,    0,    0,
    0,    0,    0,  845,    0,    0,  845,  662,    0,  814,
    0,  814,    0,    0,  662,    0,    0,    0,    0,    0,
    0,  845,  980,  662,    0,    0,  426,    0,  980,  424,
    0,  425,    0,    0,    0,    0,  662,    0,  980,    0,
    0,    0,  662,  662,  427,    0,    0,    0,    0,  282,
  662,    0,  980,  980,  980,  980,  980,  980,    0,    0,
    0,    0,    0,    0,    0,    0,  980,    0,    0,    0,
  662,    0,    0,  662,    0,  980,  980,  980,  980,  980,
  980,  980,  980,  980,  980,  980,    0,  980,    0,    0,
  980,  980,    0,  980,  980,  980,  980,  980,  980,  980,
  980,  662,  662,  980,  980,  980,  980,  980,    0,    0,
    0,  980,  980,  662,    0,    0,    0,  662,    0,    0,
    0,    0,    0,  488,    0,    0,  424,    0,  425,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  427,    0,    0,  815,    0,  282,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  662,    0,    0,    0,    0,    0,
  662,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  662,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  815,    0,    0,  815,  662,  662,  662,  662,  662,  662,
  815,    0,    0,  815,    0,    0,    0,  815,  662,    0,
    0,    0,  815,    0,  845,  815,    0,  662,  662,  662,
  662,  662,  662,  662,  662,  662,  662,  662,    0,  662,
    0,  815,  662,  662,    0,  662,  662,  662,  662,  662,
  662,  662,  662,  388,    0,  662,  662,  662,  662,  662,
    0,    0,  389,  662,  662,  815,    0,    0,  815,  815,
    0,    0,    0,    0,    0,  390,    0,    0,  815,   98,
  845,  391,  392,  845,    0,    0,    0,    0,    0,    0,
  845,    0,    0,  845,    0,  815,    0,  845,    0,    0,
    0,    0,  845,    0,    0,  845,    0,    0,    0,  393,
    0,    0,  394,    0,    0,    0,    0,  815,    0,    0,
   99,  845,  815,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  100,    0,  815,    0,  815,    0,  395,    0,
  396,  397,    0,    0,    0,  845,  134,    0,  845,  845,
  388,    0,  398,    0,    0,    0,  399,  101,  845,  389,
    0,    0,  488,    0,    0,  424,    0,  425,    0,    0,
    0,    0,  390,    0,    0,  845,    0,    0,  391,  392,
  427,    0,    0,    0,    0,  282,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  845,    0,    0,
    0,    0,  845,    0,    0,    0,  393,    0,    0,  394,
    0,    0,    0,  400,  845,    0,  845,    0,    0,  401,
    0,    0,    0,    0,    0,    0,  102,    0,    0,  402,
    0,    0,    0,    0,    0,    0,  103,  396,  397,    0,
    0,    0,    0,  403,  404,  405,  406,  207,  208,  398,
    0,    0,    0,  399,    0,    0,    0,  407,    0,    0,
    0,  104,    0,    0,    0,    0,  408,  409,  410,  411,
  412,  413,  414,  415,  416,  106,  417,    0,  108,  105,
    0,  418,  419,    0,  420,  421,  422,  111,  112,  113,
  423,  114,    0,    0,  115,  116,  117,  118,  119,    0,
    0,    0,  120,  121,    0,    0,    0,    0,  106,  107,
  400,  108,    0,    0,    0,    0,  401,    0,  109,  110,
  111,  112,  113,    0,  114,    0,  402,  115,  116,  117,
  118,  119,    0,    0,  690,  120,  121,    0,    0,    0,
  403,  404,  405,  406,  207,  208,    0,    0,    0,    0,
    0,    0,    0,    0,  407,    0,    0,    0,    0,    0,
    0,    0,    0,  408,  409,  410,  411,  412,  413,  414,
  415,  416,  106,  417,    0,  108,    0,    0,  418,  419,
    0,  420,  421,  422,  111,  112,  113,  423,  114,  388,
    0,  115,  116,  117,  118,  119,    0,    0,  389,  120,
  121,  488,    0,    0,  424,    0,  425,    0,    0,    0,
    0,  390,    0,    0,    0,    0,    0,  391,  392,  427,
    0,    0,    0,    0,  282,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  393,    0,    0,  394,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  396,  397,    0,    0,
    0,  488,  946,    0,  424,    0,  425,    0,  398,    0,
    0,    0,  399,    0,    0,    0,    0,    0,    0,  427,
    0,    0,  105,    0,  282,    0,    0,    0,    0,    0,
    0,  164,  165,  166,  167,  168,  169,  170,  171,  172,
  173,  174,  175,  176,  177,  178,  179,    0,    0,  180,
  181,  106,  107,    0,  108,    0,    0,  182,  183,  698,
    0,  109,  110,  111,  112,  113,  184,  114,  185,  400,
  115,  116,  117,  118,  119,  401,    0,    0,  120,  121,
  186,  187,  188,    0,    0,  402,    0,    0,    0,  221,
    0,    0,    0,  221,    0,    0,    0,    0,    0,  403,
  404,  405,  406,  207,  208,    0,    0,  221,    0,    0,
    0,  221,    0,  407,  221,    0,    0,    0,    0,    0,
    0,    0,  408,  409,  410,  411,  412,  413,  414,  415,
  416,  106,  417,    0,  108,    0,    0,  418,  419,    0,
  420,  421,  422,  111,  112,  113,  423,  114,  388,    0,
  115,  116,  117,  118,  119,  221,    0,  389,  120,  121,
    0,  221,  221,  221,    0,  221,  221,  221,    0,  221,
  390,    0,    0,    0,    0,    0,  391,  392,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  221,
    0,  221,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  221,    0,    0,    0,    0,    0,  388,    0,
    0,    0,    0,  395,    0,  396,  397,  389,    0,  221,
  488,    0,    0,  424,    0,  425,    0,  398,    0,    0,
  390,  399,    0,    0,    0,    0,  391,  392,  427,    0,
    0,  221,    0,  282,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  393,    0,    0,  394,    0,    0,
    0,    0,    0,    0,    0,    0,  221,  221,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  400,    0,
    0,    0,    0,  221,  401,  396,  397,    0,    0,    0,
    0,    0,    0,    0,  402,    0,    0,  398,    0,    0,
    0,  399,    0,    0,    0,    0,    0,    0,  403,  404,
  405,  406,  207,  208,    0,    0,    0,    0,    0,    0,
    0,    0,  407,    0,    0,    0,    0,    0,    0,    0,
    0,  408,  409,  410,  411,  412,  413,  414,  415,  416,
  106,  417, 1432,  108,    0,    0,  418,  419,    0,  420,
  421,  422,  111,  112,  113,  423,  114,    0,  400,  115,
  116,  117,  118,  119,  401,    0,    0,  120,  121,    0,
    0,    0,    6,    0,  402,    0,    0,    0,    0,    0,
    0,    0,    0, 1549, 1550,    0, 1551,    0,  403,  404,
  405,  406,  207,  208,    0,    0,    0,    0,    0,    0,
    0,    0,  407, 1552,    0,    0,    0,    0,    0, 1553,
    0,  408,  409,  410,  411,  412,  413,  414,  415,  416,
  106,  417,    0,  108,    0,    0,  418,  419,    0,  420,
  421,  422,  111,  112,  113,  423,  114,  388,    0,  115,
  116,  117,  118,  119,    0,    0,  389,  120,  121,  488,
    0,    0,  424,    0,  425,    0,    0,    0,    0,  390,
    0,    0,    0,    0,    0,  391,  392,  427,    0, 1554,
    0,    0,  282,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14,    0,
    0,    0,    0,  393,    0,    0,  394,    0,    0,    0,
    0,    0,    0, 1555,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0, 1693,    0,  396,  397,    0,    0,    0,  488,
    0,  105,  424,    0,  425,    0,  398,    0,    0,    0,
  399,    0,    0,    0,    0,    0,    0,  427,    0,    0,
    0,    0,  282,    0,    0,    0,    0,    0,    0,    0,
 1556,  107,    0,  108,    0,    0,    0,    0,    0,    0,
  109,  110,  111,  112,  113,    0,  114,    0,    0,  115,
 1557,  117,  118,  119,    0,    0,    0,  120,  121, 1558,
 1559, 1560,    0,    0,    0,    0,    0,  400,    0,    0,
    0,    0,    0,  401,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  402,    0,    0,    0,  986,    0,    0,
    0,  986,    0,    0, 1151,    0,    0,  403,  404,  405,
  406,  207,  208,    0,    0,    0,    0,  988,    0,  986,
    0,  407,  986,    0,    0,    0,    0,    0,    0,    0,
  408,  409,  410,  411,  412,  413,  414,  415,  416,  106,
  417,    0,  108,    0,    0,  418,  419,    0,  420,  421,
  422,  111,  112,  113,  423,  114,  388,    0,  115,  116,
  117,  118,  119,  986,    0,  389,  120,  121,    0,  986,
  986,  986,    0,  986,  986,  986,    0,  986,  390,    0,
    0,    0,    0,    0,  391,  392,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  986,    0,  986,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  393,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  986,    0,    0,    0,    0,    0,  388,    0,    0,    0,
    0,    0,    0,  396,  397,  389,    0,  986,  488,    0,
    0,  424,    0,  425,    0,  398,    0,    0,  390,  399,
    0,    0,    0,    0,  391,  392,  427, 1737,    0,  986,
    0,  282,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  393,    0,    0,  394,    0,    0,    0,    0,
    0,    0,    0,    0,  986,  986, 1335,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  400,    0,    0,    0,
    0,  986,  401,  396,  397,    0,    0,    0,    0,    0,
    0,    0,  402,    0,    0,  398,    0,    0,    0,  399,
    0,    0,    0,    0,    0,    0,  403,  404,  405,  406,
  207,  208,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  412,  413,  414,  415,  416,  106,  417,
    0,  108,    0,    0,  418,  419,    0,  420,  421,  422,
  111,  112,  113,  423,  114,    0,  400,  115,  116,  117,
  118,  119,  401,    0,    0,  120,  121,    0,    0,    0,
    0,    0,  402,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, 1348,    0,    0,  403,  404,  405,  406,
  207,  208,    0,    0,    0,    0,    0,    0,    0,    0,
  407,    0,    0,    0,    0,    0,    0,    0,    0,  408,
  409,  410,  411,  412,  413,  414,  415,  416,  106,  417,
    0,  108,    0,    0,  418,  419,    0,  420,  421,  422,
  111,  112,  113,  423,  114,  388,    0,  115,  116,  117,
  118,  119,    0,    0,  389,  120,  121,  488,    0,    0,
  424,    0,  425,    0,    0,    0,    0,  390,    0,    0,
    0,    0,    0,  391,  392,  427,    0,    0,    0,    0,
  282,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  393,    0,    0,  394,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  396,  397,    0,    0,    0,  980,    0,    0,
  980,    0,  980,    0,  398,    0,    0,    0,  399,    0,
    0,    0,  133,    0,    0,  980,  133,    0,    0,    0,
  980,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  133,    0,    0,  133,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  400,    0,    0,    0,    0,
    0,  401,    0,    0,    0,    0,    0,    0,  133,    0,
    0,  402,    0,    0,  133,  133,  133,    0,  133,  133,
  133,    0,  133,    0,    0,  403,  404,  405,  406,  207,
  208,    0,    0,    0,    0,    0,    0,    0,    0,  407,
    0,    0,  133,    0,  133,    0,    0,    0,  408,  409,
  410,  411,  412,  413,  414,  415,  416,  106,  417,    0,
  108,    0,    0,  418,  419,    0,  420,  421,  422,  111,
  112,  113,  423,  114,  388,  133,  115,  116,  117,  118,
  119,    0,    0,  389,  120,  121,    0,    0,    0,    0,
    0,    0,  133,    0,    0,    0,  390,    0,    0,    0,
    0,    0,  391,  392,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  133,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  393,    0,    0,  394,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  543,    0,    0,    0,    0,  133,
  133,    0,    0,    0,  980,    0,    0,    0,    0,    0,
    0,  396,  397,  980,    0,    0,  133,    0,    0,    0,
    0,    0,    0,  398,    0,    0,  980,  399,    0,    0,
  809,    0,  980,  980,    0,  543,    0,    0,  543,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  543,    0,  543,    0,    0,    0,    0,
  980,    0,    0,  980,    0,    0,    0,    0,    0,    0,
    0,  809,    0,    0,  809,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  400,  543,    0,    0,    0,  809,
  401,  980,  980,    0,    0,    0,    0,    0,    0,    0,
  402,    0,    0,  980,    0,    0,    0,  980,    0,    0,
    0,    0,    0,    0,  403,  404,  405,  406,  207,  208,
    0,    0,    0,    0,    0,    0,    0,    0,  407,    0,
    0,  810,    0,    0,    0,    0,    0,  408,  409,  410,
  411,  412,  413,  414,  415,  416,  106,  417,    0,  108,
    0,    0,  418,  419,    0,  420,  421,  422,  111,  112,
  113,  423,  114,    0,  980,  115,  116,  117,  118,  119,
  980,    0,  810,  120,  121,  810,    0,    0,    0,    0,
  980,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  810,    0,    0,    0,  980,  980,  980,  980,  980,  980,
    0,    0,    0,    0,    0,    0,    0,    0,  980,    0,
    0,    0,    0,    0,    0,    0,    0,  980,  980,  980,
  980,  980,  980,  980,  980,  980,  980,  980,    0,  980,
    0,    0,  980,  980,    0,  980,  980,  980,  980,  980,
  980,  980,  980,    0,    0,  980,  980,  980,  980,  980,
    0,    0,  543,  980,  980,    0,    0,    0,    0,    0,
    0,    0,    0,  853,    0,    0,    0,    0,  543,    0,
    0,    0,  543,    0,  543,    0,    0,    0,  543,    0,
    0,  543,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  809,    0,  543,    0,    0,    0,    0,    0,
  543,    0,    0,  543,  853, 1019, 1019,  853, 1019, 1019,
 1019,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  853, 1019, 1019, 1019,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  809,    0,
    0,  809,    0,    0, 1019,    0,    0,    0,  809,    0,
    0,  809,    0,    0,    0,  809,  811,    0,    0,    0,
  809,    0,  543,  809,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  809,
    0,    0,    0,  810,  543,  854,    0,    0,    0,    0,
    0,  543,    0,    0,    0,  543,    0,  811,    0,    0,
  811,    0,  543,  809,    0,    0,  809,  809,    0,    0,
    0,    0,    0,    0,    0,  811,  809,    0,    0,    0,
  543,    0,    0,    0,    0,  543,  854, 1020, 1020,  854,
 1020, 1020, 1020,  809,    0,    0,    0,    0,    0,  810,
    0,    0,  810,    0,  854, 1020, 1020, 1020,    0,  810,
  543,    0,  810,    0,  543,  809,  810,    0,    0,    0,
  809,  810,    0,    0,  810,    0,    0,    0,    0,  844,
    0,    0,  809,    0,  809,    0, 1020,    0,    0,    0,
  810,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  810,    0,    0,  810,  810,    0,
  844,    0,    0,  844,    0,  853,    0,  810,    0,    0,
    0,    0,    0,    0,    0, 1019,    0,    0,  844,    0,
    0,    0,    0, 1019,  810,    0,    0,    0, 1019,    0,
    0, 1019,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  810,    0,    0,    0,
  855,  810,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  853,    0,  810,  853,  810,    0, 1019,    0,    0,
    0,  853,    0,    0,  853,    0, 1019,    0,  853,    0,
    0,    0,    0,  853, 1019,    0,  853,    0,    0,    0,
    0,  855, 1021, 1021,  855, 1021, 1021, 1021,    0,    0,
    0,    0,  853, 1019,    0, 1019,    0,    0,  811,  855,
 1021, 1021, 1021,    0,    0,    0,    0,    0,    0, 1019,
    0, 1019, 1019, 1019,    0,    0,  853,    0,    0,  853,
  853,    0,    0,    0,    0,    0,    0,  854,    0,  853,
    0, 1021,    0,    0,    0,    0,    0, 1020,    0,    0,
    0,    0,    0,    0,    0, 1020,  853,    0,    0,    0,
 1020,    0,    0, 1020,  811,    0,    0,  811,    0,    0,
    0,    0, 1019,    0,  811,    0,    0,  811,  853,  838,
    0,  811,    0,  853,    0,    0,  811,    0,    0,  811,
    0,    0,    0,  854,    0,  853,  854,  853,    0, 1020,
    0,    0,    0,  854,    0,  811,  854,    0, 1020,    0,
  854,    0,    0,    0,    0,  854, 1020,    0,  854,    0,
  838,  844,    0,  838,  846,    0,    0,    0,    0,  811,
    0,    0,  811,  811,  854, 1020,    0, 1020,  838,    0,
    0,    0,  811,    0,    0,    0,    0,    0,    0,    0,
    0, 1020,    0, 1020, 1020, 1020,    0,    0,  854,  811,
    0,  854,  854,    0,    0,  846,    0,    0,  846,    0,
    0,  854,    0,    0,    0,    0,    0,  844,    0,    0,
  844,  811,    0,  846,    0,    0,  811,  844,  854,    0,
  844,    0,    0,    0,  844,    0,    0,    0,  811,  844,
  811,    0,  844,    0, 1020,    0,    0,  847,    0,    0,
  854,    0,  855,    0,    0,  854,    0,    0,  844,    0,
    0,    0, 1021,    0,    0,    0,    0,  854,    0,  854,
 1021,    0,    0,    0,    0, 1021,    0,    0, 1021,    0,
    0,    0,  844,    0,    0,  844,  844,    0,  847,    0,
    0,  847,    0,    0,    0,  844,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  847,    0,  855,    0,
    0,  855,  844,    0, 1021,    0,    0,    0,  855,  835,
    0,  855,    0, 1021,    0,  855,    0,    0,    0,    0,
  855, 1021,    0,  855,  844,    0,    0,    0,    0,  844,
    0,    0,    0,    0,    0,    0,    0,    0,  839,  855,
 1021,  844, 1021,  844,    0,    0,    0,    0,    0,    0,
  835,    0,    0,  835,    0,    0, 1021,    0, 1021, 1021,
 1021,    0,    0,  855,    0,    0,  855,  855,  835,    0,
    0,  838,    0,    0,    0,    0,  855,    0,    0,  839,
    0,    0,  839,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  855,    0,    0,    0,  839,  848,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, 1021,
    0,    0,    0,    0,    0,  855,  846,    0,    0,    0,
  855,    0,    0,    0,    0,    0,    0,  838,    0,    0,
  838,    0,  855,    0,  855,    0,    0,  838,    0,  848,
  838,    0,  848,    0,  838,    0,    0,    0,    0,  838,
    0,    0,  838,    0,    0,    0,    0,  848,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  838,    0,
  836,    0,  846,    0,    0,  846,    0,    0,    0,    0,
    0,    0,  846,    0,    0,  846,    0,    0,    0,  846,
    0,    0,  838,    0,  846,  838,  838,  846,    0,  847,
    0,    0,    0,    0,    0,  838,    0,    0,    0,    0,
    0,  836,    0,  846,  836,    0,    0,    0,    0,    0,
    0,    0,  838,    0,    0,    0,    0,    0,    0,  836,
    0,    0,    0,    0,    0,    0,    0,  846,    0,    0,
  846,  846,  840,    0,  838,    0,    0,    0,    0,  838,
  846,    0,    0,    0,    0,  847,    0,    0,  847,    0,
    0,  838,    0,  838,    0,  847,    0,  846,  847,    0,
    0,  835,  847,    0,    0,    0,    0,  847,    0,    0,
  847,    0,    0,  840,    0,    0,  840,    0,    0,  846,
    0,    0,    0,    0,  846,    0,  847,    0,    0,    0,
  839,  840,    0,    0,    0,    0,  846,    0,  846,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  847,    0,    0,  847,  847,    0,    0,  835,    0,    0,
  835,    0,    0,  847,    0,    0,    0,  835,    0,    0,
  835,    0,    0,    0,  835,    0,    0,    0,    0,  835,
  847,    0,  835,    0,    0,    0,  839,    0,    0,  839,
  848,    0,    0,    0,    0,    0,  839,    0,  835,  839,
    0,    0,  847,  839,    0,    0,    0,  847,  839,    0,
    0,  839,    0,    0,    0,    0,    0,    0,    0,  847,
    0,  847,  835,    0,    0,  835,  835,  839,    0,    0,
    0,    0,    0,    0,    0,  835,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  848,    0,    0,  848,
    0,  839,  835,    0,  839,  839,  848,    0,    0,  848,
    0,    0,  836,  848,  839,    0,    0,    0,  848,    0,
    0,  848,    0,    0,  835,    0,    0,    0,    0,  835,
    0,  839,    0,    0,    0,    0,    0,  848,    0,    0,
    0,  835,    0,  835,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  839,    0,    0,    0,    0,  839,    0,
    0,  848,    0,    0,  848,  848,    0,    0,  836,    0,
  839,  836,  839,    0,  848,    0,    0,    0,  836,    0,
    0,  836,    0,    0,  840,  836,    0,    0,    0,    0,
  836,  848,    0,  836,    0,    0,    0,  326,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  836,
    0,    0,    0,  848,    0,    0,    0,    0,  848,    0,
    0,    0,    0,    0,    0,    0,    0,  326,    0,    0,
  848,    0,  848,  836,  326,    0,  836,  836,  326,  326,
  840,  326,    0,  840,  326,    0,  836,    0,    0,    0,
  840,    0,    0,  840,    0,    0,    0,  840,  326,    0,
    0,    0,  840,  836,  326,  840,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  840,    0,    0,    0,  836,    0,    0,    0,    0,
  836,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  836,    0,  836,  840,    0,    0,  840,  840,
    0,    0,    0,    0,    0,    0,    0,  598,  840,    0,
    0,  599,    0,    0,  326,    0,    0,    0,  798,    0,
    0,    0,    0,    0,    0,  840,    0,    0,    0,  600,
    0,    0,  601,  326,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  840,  326,    0,
    0,    0,  840,    0,    0,    0,    0,    0,  326,    0,
    0,    0,    0,    0,  840,    0,  840,  326,    0,  326,
    0,    0,    0,  602,    0,    0,  326,    0,    0,  603,
  604,  605,    0,  606,  607,  608,    0,  609,  598,    0,
    0,    0,  599,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  326,  326,  610,  326,  611,
  600,    0,    0,  601,    0,  326,  326,  326,  326,  326,
    0,  326,    0,    0,  326,  326,  326,  326,  326,    0,
    0,    0,  326,  326,  326,  326,  326,    0,    0,    0,
  612,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  602,    0,    0,  613,    0,    0,
  603,  604,  605,    0,  606,  607,  608,    0,  609,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  614,
    0,    0,    0,    0,    0,    0,    0,    0,  610,    0,
  611,    0,    0,    0,  105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  615,  616,    0,    0,    0,    0,
    0,  612,    0,  106,  107,    0,  108,    0,    0,    0,
    0,  617,    0,  109,  110,  111,  112,  113,  613,  114,
    0,    0,  115,  116,  117,  118,  119,    0,    0,    0,
  120,  121,    0,    0,    0,    0,    0,    0,    0,    0,
  614,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  105,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  615,  616,    0,    0,    0,
    0,    0,    0,    0,  106,  107,    0,  108,    0,    0,
    0,    0,  617,    0,  109,  110,  111,  112,  113,    0,
  114,    0,    0,  115,  116,  117,  118,  119,  467,    0,
    0,  120,  121,    0,    0,    0,    0,    0,  467,  467,
  467,  467,  467,  467,  467,  467,  467,  467,  467,  467,
  467,  467,  467,    0,    0,  467,  467,  467,  467,    0,
  467,    0,    0,  467,  467,    0,    0,  467,  467,  467,
  467,  467,  467,  467,  467,    0,  467,  467,  467,  467,
  467,    0,    0,    0,  467,  467,  467,  467,  467,
};
static short yycheck[] = {                                       9,
   16,    0,   12,   55,    4,   64,   16,  104,   18,  140,
  141,  140,  141,  151,   63,  516,  290,  366,  690,  366,
  713,  362,  557,  358,   56,  560,   85,  337,  880,   63,
   73,  566,  884,   91,  426,   67,  566,   89,  499,  773,
   94,  267,   62,   53,   54,   55,   56,   57,   58,   70,
  276, 1046,   95,   63,   64,   65,  523,   67,   90,   69,
   70,  698,   72,   73,  942,   97,  759,   83,  476,   88,
  650,   81,   93,   83,   84,   85,   92,   87,   88,   89,
   90,  383,   92,   93,   94,   95,   57,   97,   68,  330,
  727,  311, 1266,  371, 1039,  335, 1039,  131,   69,  133,
  154, 1046,  980, 1046,    0,  712,    0,  766, 1039, 1039,
   45,   91, 1039,  284,   44, 1046, 1046,    0,  731, 1046,
  152,  131,  132,  133, 1039,    0,  145,  643,   44,    0,
    0, 1046,   45,   40,  231,  145,  795, 1095,   44,  834,
  769,  151,  152,  153,  154,  804,    0,   41,  158, 1047,
   44,  865,   44,    0, 1209,    0,   44,    0,   44,  990,
 1058,  132,   44,   59, 1431,   59,   59,    0,   41,   45,
  482,   41,  192,  311,    0,    0,   59,   42,   40,    0,
 1078,   40,  153,  222,   59, 1072,   44,   41,   59,   59,
   44,    0,  533,   41,   61, 1472,   41,  217,   41,  858,
    0,   44,   44,   44,   63,   59,    0,  274,  543,  544,
  545,  546,   59,    0,   59,    0,   59,  566, 1096,  566,
    0,  318,   40,   63,    0, 1314,   59,  261,   91,    0,
  274,  656,   41,   59,   59,   44,  908,  909,   59,  740,
  518,  357,  468,  295,  301,   63,    0,   41,   12,    0,
   59,  261,  593, 1264,   41,  265,   40,  267,  268,   59,
    0,   45, 1224, 1715,  274,   59,  276,  939,  264,  262,
   41,  260,   59,    0,   59,  314,  355,  311,  348,   59,
   45,  338, 1202,   59, 1204,  295,  821,   41,   59,  299,
   41, 1558, 1559,  320,  265,  260,  598, 1292,   63,  313,
   41,   45,  336,  274,  377,   59,  158,   41,   59,  343,
   44,  345,    0,  347,   41,  348,  350, 1769, 1213,   59,
  283,  308,  301,    0,  357,    0,  336,  379,  299,  569,
    0,  318,   59,  343,   41,  345,  368,  347,  476,  359,
  350,  361,  402, 1819, 1820,  356,    0, 1292,  369, 1292,
 1424,  265,  455,   41, 1013,  375,  320,  301,  368,  369,
  973, 1292, 1292, 1815,   12, 1292, 1038,  377,    0,  379,
    0,   59,  985,  986,  138,  448,  402, 1292,  438,  619,
    0,  842,   59, 1281,   59,  442,   44, 1476,  302,   59,
    0,  461,  462,  386, 1225,  260,  273, 1471, 1057,  466,
    0,  517,   41,  417,  678,   59,  377, 1883,  506,  436,
 1384, 1298,   44,  492,  277,  411, 1311,  427,  429,   41,
 1687,  695,  734,  264, 1598, 1399, 1437,   59,  417,   59,
  318,   41,  279,  300,  320,  467, 1713,  711,  303,   59,
  484, 1403,  476,  264, 1116, 1117,  304, 1119, 1120,   59,
  482,   59,  417,  300,  464,  584,  331,  467,  468,   59,
  885,  309,  304,  442,  318,  475,  453,  741, 1523,  479,
  340, 1100,  482,  318,  484,  318,  439,  487,  258,  279,
   41, 1401,  329,  259,  517, 1090,  340,  394,  264,  763,
  138,  271,    0,  464,  270,  340,  722,  340,  442,  279,
  300,  348,  328,  328,  475,  331,  331, 1241,  479,   41,
  357, 1469,   44,  484,  387, 1210,  363,  410,  538,   41,
  300, 1128,  452,  458,  563, 1105,  306,  585,  308,  329,
 1428, 1705,  566,   41,  381, 1433,   44, 1435,  454,  410,
  760,  782,  387,  783,  440,  458,  786,  318,  348,  329,
  485,   59,  458,  340,  436,  565,  566,  357,  454,  869,
  800,  571,  454,  363,  318,  440,  737,  587,  348,  340,
  978,  454, 1470, 1089,  383, 1247,  878,  357,  856,  454,
 1411,  381,  458,  363,  454,  447,  340,  827, 1762,  981,
 1304,  318, 1287,  387,  633,  440, 1483, 1269,    0,  458,
  454,  381,  342,  436,  279,  585, 1219,  501,  642,  454,
 1488,  454, 1879,  340,  440,  440,  745,    0,  458,  639,
  640,  397,  760,  262,  409,  300,  452,   41,  454,  454,
 1804, 1236,  642,  387,  264,  487,  387, 1535,  418,   41,
  458,  651,  396, 1541, 1303,  396,  387,  431,  502,  778,
  779,  651,  340,  409,  329,  396,    0,   59,   41,   41,
  387,   44,  681,  937,  264,  405,  431,  454,  408,  396,
  517,  681,  318,  348,  458,  807,   59,  411,  261,  386,
  714,  356,  357,  454,  424,  262,  440,  431,  363, 1423,
  367, 1026,  501,  458,  348,  705,  328,  367, 1335,  331,
  454,  262,  734,  713,  714, 1603,  381,  717,  318,   41,
  450,  343,  722,  440,  458,   59,    0,  517,  393,    0,
  395,  731, 1620,  781,  734,  383,  760,  454,  348,  739,
  340,  765,    0,  743,  458,  769,  962,  517, 1733,  965,
  262, 1413,  274,  418,  754,   40,  756,  386,   41,  759,
 1724,   41,  772,  773,  774,  765, 1091,   41,  792,  769,
   41,  819,    0,   44,  386,    0,  454,  397,  443,  458,
  278,    0,  280,    0,  262,   59,   44,  309,   59,  287,
  731,  322,  792,  754,  656,  756,  820,  821, 1733,  661,
 1733,   59,  277, 1103,  852,  853,  854,  397, 1190,  405,
 1267,  781, 1733, 1733, 1044,  161, 1733,  453,  440,  386,
  820,  821,  832,  286, 1712,  322, 1056,    0, 1733,  258,
  452,   59,  454, 1721,   59,  386,    0,  401,    0, 1290,
   59, 1223,   59, 1363,  262, 1296, 1653, 1077, 1655,  819,
  978,  338,  517,    0,  454, 1450,  826,  857,    0,    0,
  456,  257,  318,  273,    0, 1727, 1166,  867,   41,  839,
  345,   44,  387, 1468,  386,   40,    0,  306, 1766,  879,
  378,    0,  852,  853,  854,  383,   59,    0,    0,  411,
  262,    0,    0,  300,  257,   59,  857,   59,  403,    0,
   41,    0,  400,   44, 1569,  278,  867,  280,  386,  407,
  259,    0,   59, 1720,  287,  420, 1180,   59,   59, 1183,
  278,  270,  966,   59,  258,  440,  318,  331, 1050, 1051,
  340,  319,   41,  383,  967,   59,  328,  271,  334,  331,
   59,  963, 1784,  441,  466,  352,   59,   59,  340, 1213,
   59,   59,   41,  964,  978,  442, 1075,  435,   59, 1821,
   59,  319,  962,  963,  964,  965,  966,  967,  386,    0,
   59,  334,  306,  973,  308,   42,  322, 1308, 1100,  262,
   47,    0,  262,  387,  269,  985,  986,  258, 1180,  418,
  990, 1183,  396,  417,  445,  387, 1658,  453,  461,  462,
  271,  990,    0,  501,  396,  378,  328,  436,  279,  331,
   41, 1818,  400,   44,  386,   44, 1678,  435, 1543,  881,
  444,  383,   41,  885, 1363,   44, 1363,  400,   59,  300,
 1521,    0,  973, 1524,  407,  306, 1084,  308,  268,  264,
   59,  470,  400,   41,  985,  264,   44, 1311,  440, 1279,
  278,  241,  280,  441, 1102, 1725, 1726,  508,  329,  249,
 1108,   59,  454,    0,    0,  387,  340, 1067,  441, 1093,
  328,   40,   41,  331,  396,   44, 1100,  348,  325, 1069,
 1310,  385, 1082,  441,  418,  343,  357, 1208, 1209, 1210,
   59,  264,  363, 1093,  258, 1420, 1758,  465,  308,  467,
 1100, 1771,  264,  386,   41,   41,  386,  271,  318,    0,
  381, 1794,    0, 1377, 1084,  279,  258, 1381,   40,  307,
  308,  383,   59,   59, 1243,  342, 1245,  383,  501,  271,
 1259, 1622, 1102,  493, 1799,  365,  300,  278, 1108,  280,
  264,  285,  306,  503,  308,  318,  287,  418,  321,  293,
  378,  264,  264,   41,   16,  328,  264,    0,  331,  278,
  377,  280,  335,  264,  306,  329,  308,  340,   59, 1834,
  343,   59,  397,  436,  307,  308, 1305,    0,  397,  278,
 1850,  280,  440,  430,  348, 1377,  359, 1306,  405, 1381,
  437,  408,  356,  357,  452,  342,  454,  343,   41,  363,
  283, 1866, 1202,   44, 1204,    0,  342,  424, 1870, 1439,
  383,  263, 1877,  280,  387, 1880,   59,  381,  285, 1219,
  265,   83,  368,  396, 1889, 1225, 1774, 1775,  417,  318,
  377, 1241,    0,  450,  377,  397,   59,  378,  418,  328,
  413,  377,  331,  453,   44,   44,  517,  278, 1478,  280,
  494,  340,   93, 1517,  418,  444,  287,  302,  405,  400,
  312,  408,  337,  451,   59,  429,  407,  440,  378,  405,
  274,  323,  408,  397,  318,  418,  418,  424, 1406,  452,
  426,  454, 1773,  458,  397,  397, 1415,  460,  424,  397,
  400,   59,  337,   93,   93, 1285,  397,  407,  387,  318,
  441,  384,    0,  450, 1432,    0,  397,  396,  402,  328,
  485, 1321,  331,    0,  450, 1325,  489,  490,  451,  492,
  318,  340,  465, 1871,  343, 1517,  499,  500,  501,  502,
  503,  441,  505,  331,    0,  508,  509,  510,  511,  512,
  434,  260,  340,  516,  517,  436,   44,  378,  301,  318,
 1717,  440,  321,  517,   41,  449,  439,   44,   40,  328,
  501,   59,  331,  452,   59,  454,  335,  258,  387,  400,
    0,  340,   59,  456,  343,  427,  407,  396,    0,  470,
  271,  318,  318,  424,  303,  338,  258, 1617,  279,  387,
  359,  328, 1431,   59,  331,  331,   44, 1525,  396,  271,
 1767, 1401, 1523,  340,  340,    0, 1406, 1431, 1432,  300,
  441, 1411,  260, 1423,  383,  306,    0,  308,  387, 1538,
  402,  440,  260,   42,   43,  260,   45,  396,   47,   59,
  318, 1431, 1432,  452,  306,  454,  308,   59,  329,    0,
 1440, 1569,  440,  331,  413,   93,   41, 1814,    0,   44,
  387,  387,  340,    0,  287,  303,  454,  348,  405,  396,
  396,  272,  259,  445,   59,  303,  357,  300,  303,  301,
  501,  440,  363,   42,   43,   59,   45,  288,   47, 1440,
   41, 1610, 1849,  452,  281,  454,  260, 1616,  331,   41,
  381,  460,   44,  290,   41,  417,    0,   44,   59,  387,
 1549,   44,    0,  440,  440,    0,  338,   59,  396,  456,
   42,   43,   59,   45, 1601,   47,   59,  454,  454,  461,
  489,  490,  333,  492,  321, 1525,  508,  418,  264,  303,
  499,  500,  501,  502,  503,   40,  505,    0,  460,  508,
  509,  510,  511,  512,  387, 1569,  418,  516,  517, 1549,
  337,  362,  440,  396,  337,   59,   44, 1557, 1558, 1559,
 1602,   59, 1690,  258,   59,    0,  454,  489,  490, 1569,
  492,   59,  316,    0,  342,  407,  271,  499,  500,  501,
  502,  503,    0,  505,  279,  417,  508,  509,  510,  511,
  512,  374, 1592,    0,  516,  517,   59, 1597,   42,   43,
  318,   45, 1602,   47,  387,  300,  460,   40,    0,  377,
  442,  306, 1884,  308, 1886, 1621, 1626, 1690,  429, 1706,
 1630, 1621,    0,  420,   59,   58,  517,  424,  425,  426,
   42,   43,   59,   45,  329,   47, 1597,  405,    0,  405,
  408,   59,  336,    0,  352,   42,   43,   59,   45,    0,
   47, 1673,   59,  348,    0,  280,  424,  402,  424, 1787,
  285,  280,  357, 1791,  452, 1793,  285,   59,  363,  259,
  378, 1799,    0, 1673, 1802,  279,  258,  340,    0,  261,
  270,   59,  450,  278,   40,  280,  381, 1687,    0,  284,
  321,  488,  287,   42,   43,  492,   45,   59,   47,  316,
  317, 1750,   59, 1831,  335,  300, 1834,    0,   59, 1715,
 1699,  280,  264,   59, 1787, 1715,  285,   40, 1791,  321,
 1793,  340,  321,  418,  306,  329, 1732, 1727,  359, 1802,
 1858,   59, 1732,  335,    0, 1863,  335,   59, 1866,   41,
   42,   43,  311,   45,    0,   47,   40,   59,  280, 1877,
 1750, 1751, 1880,  285,   42,  307,  308,  359, 1831,  363,
  359, 1889, 1752, 1769,    0,    0,   59,  328,  320, 1769,
  331,    0, 1778,    0,    0, 1799,  320,  381, 1778,  284,
  318,  383,  413,  378,  383, 1858, 1828,  402,  320,   42,
 1863,  383,   41,   59, 1794,   44,  342,  493,  494, 1799,
   41,   42,   43,   59,   45,  400,   47,  383,  356, 1815,
 1834,  413,  407,   41,  413, 1815,   44,   41,  506, 1825,
   44, 1821,  517,   59,   59, 1825,  387, 1202, 1828, 1204,
   59,  377,   59,   59, 1834,  396,  280,  337,  461,  462,
   41,  285, 1866,   44,   93,  393,  441,  395,   41,   42,
   43,  436,   45, 1877,   47, 1855, 1880,  450,  410,  405,
   46,  434,  408,  384,  661, 1889, 1866,  361,  280,   93,
  418,  442,   58,  285,  374,   61,  320, 1877,  424, 1879,
 1880,  678,  417,  280, 1884,  682, 1886,  387,  285, 1889,
   41,   41,  689,   44,   44,  443,  454,  694,  695,  451,
  452,  457,  458,  459,  450,  391,  392,  704,  460,  706,
  707,  708,  709,  710,  711,  712,  461,  462,   41,   42,
   43,  318,   45,   41,   47,  516,   44,   46,  340,  485,
  486,  280,   41,  516,  287,   44,  285,  489,  490,   58,
  492,  411,   61,   41,  741,  469,   44,  499,  500,  501,
  502,  503,   44,  505,   61,  460,  508,  509,  510,  511,
  512,   46,  311,  372,  516,  517,  763,   42,   43,  286,
   45,  768,   47,   58,   41,   40,   61,   44,  280,   41,
  264,   40,   44,  285,  489,  490,   40,  492,   41,    0,
   45,   44,   40,   40,  499,  500,  501,  502,  503,  461,
  505,    0,    0,  508,  509,  510,  511,  512,  296,   40,
   40,  516,  517,  898,   40,  900,   41,  902,  903,   44,
  905,  906,  907,   41,  461,   41,   44,  460,   44,   40,
   41,   42,   43,   44,   45,   46,   47,   41,   41,  280,
   44,   44,   41,   41,  285,   44,   44,   40,   59,   60,
   61,   62,   41,   40,   40,   44,  489,  490,  461,  492,
   59,   59, 1690,   61,   40,   40,  499,  500,  501,  502,
  503,   41,  505,   40,   44,  508,  509,  510,  511,  512,
   91,   40,   93,  516,  517,   41,   44,  280,   44,   41,
   42,   43,  285,   45,   91,   47,  893,  894,  895,   46,
  436,  898,  899,  900,  460,  902,  903,  904,  905,  906,
  907,  320,   44,  910,  911,  402,  913,  320,  915,   44,
  264,  918,  919,  920,  921,  922,  923,   44,  428,   40,
   44,   46,   61,  489,  490,  264,  492,  460,  935,   44,
  937,  505,  382,  499,  500,  501,  502,  503,  348,  505,
  348,  348,  508,  509,  510,  511,  512,  280,   44, 1787,
  516,  517,  285, 1791,  278, 1793,  489,  490,  278,  492,
  300,  264,  460,  436, 1802,   58,  499,  500,  501,  502,
  503,  397,  505,  397,  981,  508,  509,  510,  511,  512,
  265,  470,  348,  516,  517,  277,  458,  447,   40,  447,
  399,  489,  490, 1831,  492,  280,  349,  460,   40,   59,
  285,  499,  500,  501,  502,  503,   91,  505,   40,   40,
  508,  509,  510,  511,  512,  264,  383,  302,  516,  517,
 1858,  318,  411,   44,  389, 1863,  489,  490,   40,  492,
   59,  382,   40,  451,   41,  436,  499,  500,  501,  502,
  503,  262,  505,  264,  265,  508,  509,  510,  511,  512,
  258,  272,  464,  516,  517,   42,   43,   44,   45,  280,
   47,  317,   40,  448,  285,  320,  274,  288,  320,  278,
  452,  280,  264,  502,  458,  284,   40,  677,  287,  679,
  274,  302,  277,  683,  684,  685,  307,  308,   40,  300,
  311,  300,  300,  284,  344,  278,  344,  318,  306,  320,
  321,  309,  321,  324,  386,  386,  386,  328,   42,   43,
  331,   45,  333,   47,  335,   40,  335,  418,  280,  340,
  341, 1128,  343,  285,  343,   41,   42,   43,  416,   45,
  300,   47,  418, 1140, 1141,   40, 1143, 1144,  359,  360,
  359,  362,  274,  417, 1151,   40,  356,  417,  264,   41,
   41,   42,   43,  320,   45,  376,   47,  378,  379,  380,
   44,   44,  383, 1170,   44,  386,  387,  280,  385,  378,
   44, 1178, 1179, 1180,  367,  396, 1183,  320, 1185,   40,
   44,  328, 1189, 1190, 1191, 1192,   44,  266,   40,  410,
  299,  400,  413,  458,  413,  460,  320,  501,  407,   40,
  264,   41,  422,  411,   41,   58, 1213,   41,  429,   41,
  418,  264,  454,  440,  435,  394, 1223,  309,  309,  440,
   41,  492,   41,   40,  489,  490,   40,  492,  436,  352,
  451,  452,  441,  454,  499,  500,  501,  502,  503,  460,
  505,   40,   40,  508,  509,  510,  511,  512,   40,   40,
 1787,  516,  517,   40, 1791, 1262,   40,   40,  466,   40,
   40,    0,  470,   40,   40, 1802,   40,   40,  489,  490,
    0,  492,   40,  262,  435,  308,  327,   44,  499,  500,
  501,  502,  503,   41,  505,   41,  388,  508,  509,  510,
  511,  512,  274,  280, 1831,  516,  517,  343,  285,  388,
  388,  331,   41,  422, 1311,   44,  390,  287,  391,  446,
  271,   41,   42,   43,   44,   45,   46,   47,  386,  386,
   59, 1858,   61,  386,  386, 1332, 1863,  407,  262,   59,
   60,   61,   62,   41, 1341,  378,  460,   42,   43,  386,
   45, 1348,   47,   41,   42,   43,  280,   45,  386,   47,
   41,  285,  278,   41,   42,   43,   40,   45,   41,   47,
   41,   91,  417,   93,  280,  489,  490,  460,  492,  285,
 1377,   41,   41,  308, 1381,  499,  500,  501,  502,  503,
  385,  505,  436,  396,  508,  509,  510,  511,  512,  280,
  435,  280,  516,  517,  285,  337,  489,  490,  442,  492,
  460,  338,  442,  301,  301, 1412,  499,  500,  501,  502,
  503,  338,  505,  260,   41,  508,  509,  510,  511,  512,
   41,  352,  385,  516,  517,  279,  320,  371,  371,  489,
  490,  460,  492,   42,   43,   41,   45,   41,   47,  499,
  500,  501,  502,  503,   41,  505,  300,   41,  508,  509,
  510,  511,  512,   41,  301,    0,  516,  517,   41,   41,
  489,  490,   41,  492,    0,   41,  313,   41,   41,   41,
  499,  500,  501,  502,  503,  329,  505,   41,   41,  508,
  509,  510,  511,  512,    0,    0,   41,  516,  517, 1496,
 1497,  338,   41,  321,  348,   41,   41,   44,   59,   44,
  293,   59,   40,  357,  340,   41, 1513,  335,   44,  363,
 1517,   59,  308,  340,   59,  343,   61,   59,   59,  258,
   59,   41,   59,   59,   40,   41,   41,  381,   44,   44,
   59,  359,  262,   59,  264,  265,   58,  352,  277,  278,
  344,  280,  272,   59,   59,  284,   91, 1554,  287,   40,
  280,   40,  368,  318,   59,  285,   40,  262,  288,  383,
  407,  300,   41,   42,   43,  451,   45,  306,   47,   44,
  417,   40,  302,   59,  304,  280,  340,  307,  308,   46,
  285,  311,  280,  340,   41,  413,  383,  285,  318,  383,
  320,  321,  280,  343,  324,  442,   41,  285,  328,   42,
   43,  331,   45,  333,   47,  335,   41,   44,  410,  435,
  340,  341,  301,  343,   40,  387,   59,  442, 1625,   41,
   59,   41,   42,   43,  304,   45, 1633,   47,   44,  359,
  360,  337,  362,  463,  307,  293,  304,   44,   41,  378,
   40,    0,   41,    0,  294,  337,  376,    0,  378,  379,
  380,  383, 1659,  383,  383,  305,  386,  387,   40,  264,
  278,  400,  312,  517,  264, 1672,  396,  389,  407, 1676,
  458,  280,  320,  264,  411,  470,  285,  416,   44,  418,
  410,  451,   41,  413,   40,   44,  309,  337,   41,   42,
   43,   44,   45,   46,   47,  458,  271,  436,  264,  429,
   59,  435,  441,  436,  343,  435,   59,   60,   61,   62,
  440,  343,  343,  258,  343,  458,   41,  271,   41,   40,
   40,  451,  452,   41,  454,   59,   59,  308,   41,  274,
  460,  470, 1739,  278,   41,  280,  304,   59,   91,  284,
   93,   41,  287,   59,   59,    0,    0,    0, 1205,    0,
   12,  276,  402,  722,  268,  300,  269, 1105,  646,  489,
  490,  306,  492,  278,  309,  280,  870, 1103,   87,  499,
  500,  501,  502,  503, 1100,  505, 1092, 1458,  508,  509,
  510,  511,  512, 1459,  434,  321,  516,  517,  438,  333,
  593,    0,  328,   44, 1801,  331,  363, 1029,  326,  335,
    0,  280,  318, 1082,  316,  321,  285,  343,   59,  765,
 1093,  345,  328,  345, 1668,  331,  802, 1592, 1760,  335,
  571, 1437, 1763,  359,  340, 1439, 1764,  343, 1260, 1806,
  792,   40,   41,  378, 1756,   44, 1739,  280, 1557, 1695,
 1800,   41,  285,  359,   44, 1238, 1416,  383, 1715,  350,
   59, 1015,   61,  338,  573,  400,  849,  331,  841,   59,
  280, 1294,  407,  378,   13,  285,  411,  383,  501,  303,
  632,  387,  754,  418,  143,  505,  741,  413,  768,  990,
  396, 1659,   91, 1533, 1778,  400, 1825,  730,  305, 1142,
  516,  436,  407, 1249, 1185,  312,  441,  413,   -1,  316,
  317,  982,   -1,   -1,  440,   -1,   -1,   -1,  325,  262,
   -1,  264,  265,   -1,   -1,   -1,  452,   -1,  454,  272,
  337,  466,   -1,   -1,  440,  470,  441,  280,   -1,   -1,
   -1,   -1,  285,   -1,   -1,  288,  452,   -1,  454,   41,
   42,   43,    0,   45,   -1,   47,   -1,   -1,   -1,  302,
   -1,    0,   -1,   -1,  307,  308,   -1,   -1,  311,  318,
   -1,   -1,  321,   -1,   -1,  318,   -1,  320,  321,  328,
   -1,  324,  331,   -1,   -1,  328,  335,   -1,  331,   -1,
  333,  340,  335,   41,  343,  402,   44,  340,  341,   -1,
  343,   -1,   41,   42,   43,   44,   45,   46,   47,   -1,
  359,   59,  419,   61,   -1,   -1,  359,  360,   -1,  362,
   59,   60,   61,   62,   -1,   -1,   -1,  434,   -1,   -1,
   -1,  438,   -1,  376,  383,  378,  379,  380,  387,   -1,
  383,   -1,  449,  386,  387,   -1,   -1,  396,   -1,   -1,
   -1,   -1,   91,  396,   93,   -1,   -1,   -1,   -1,  258,
   -1,  468,   -1,   -1,  413,   -1,   -1,  410,   -1,   -1,
  413,    0,   -1,   -1,   -1,  274,   -1,   -1,  277,  278,
  321,  280,   -1,   -1,   -1,  284,  429,  328,  287,   -1,
  331,  440,  435,   -1,  335,   -1,   -1,  440,   -1,   -1,
   -1,  300,  343,  452,   -1,  454,   -1,  306,  451,  452,
  309,  454,   41,   -1,   -1,   44,   -1,  460,  359,   -1,
   -1,   41,   42,   43,   -1,   45,   -1,   47,  318,   -1,
   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  328,    0,
   -1,  331,  383,   -1,   -1,   -1,  489,  490,   -1,  492,
  340,   -1,   -1,  343,   -1,   -1,  499,  500,  501,  502,
  503,   -1,  505,    0,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  413,  516,  517,   -1,   -1,  418,   -1,  378,
   41,   -1,   -1,   44,   -1,   41,   42,   43,  280,   45,
   -1,   47,   -1,  285,   -1,   -1,   -1,  387,   59,  440,
   61,  400,   -1,   -1,   41,   -1,  396,   44,  407,   -1,
  258,  452,  411,  454,   -1,   -1,   -1,   -1,   -1,  418,
   -1,   -1,   59,  262,   -1,  264,  265,   -1,   -1,  277,
  278,   -1,  280,  272,   -1,   -1,  284,  436,   -1,  287,
   -1,  280,  441,   -1,   -1,   -1,  285,   -1,   -1,  288,
  440,   -1,  300,   41,   42,   43,    0,   45,  306,   47,
   -1,   -1,  452,  302,  454,    0,   -1,  466,  307,  308,
   -1,  470,  311,   -1,   41,   42,   43,   -1,   45,  318,
   47,  320,  321,   -1,   -1,  324,   -1,   -1,   -1,  328,
   -1,   -1,  331,   -1,  333,   -1,  335,   41,   -1,   -1,
   44,  340,  341,   -1,  343,   -1,   41,   42,   43,   44,
   45,   46,   47,   -1,   -1,   59,   -1,   -1,   -1,   -1,
  359,  360,   -1,  362,   59,   60,   61,   62,   -1,   -1,
  378,   -1,   -1,  262,   -1,   -1,   -1,  376,   -1,  378,
  379,  380,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   -1,   -1,  400,   -1,   -1,   -1,   91,  396,   93,  407,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,   -1,   -1,
  418,  410,   -1,   -1,  413,   -1,   41,   42,   43,   -1,
   45,   -1,   47,   -1,   -1,   -1,   -1,   -1,  436,  318,
  429,   -1,  321,  441,   -1,   -1,  435,  258,   -1,  328,
   -1,  440,  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,
   -1,  340,  451,  452,  343,  454,   -1,  278,   -1,  280,
   -1,  460,  470,  284,  280,   -1,  287,   -1,   -1,  285,
  359,  472,  473,  474,  475,  476,  477,  478,  479,  300,
   -1,  278,   -1,  280,   -1,  306,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,  383,   -1,   -1,  386,  387,   -1,
  499,  500,  501,  502,  503,   -1,  505,  396,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,   41,
   42,   43,   -1,   45,  413,   47,   -1,   -1,   -1,   -1,
   -1,   -1,  280,   -1,   -1,   -1,   -1,  285,   60,   61,
   62,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
   -1,  440,   -1,  280,   -1,   -1,   -1,  378,  285,  287,
   -1,   -1,   -1,  452,   -1,  454,   -1,  262,   -1,  264,
  265,   -1,   -1,   -1,  278,   -1,  280,  272,   -1,  400,
   -1,  378,   -1,  287,   -1,  280,  407,   -1,   -1,   -1,
  285,   -1,   -1,  288,   -1,   -1,  300,  418,   -1,   -1,
   -1,   -1,   -1,  400,   -1,   -1,   -1,  302,   -1,    0,
  407,   -1,  307,  308,   -1,  436,  311,   -1,   -1,   -1,
  441,   -1,   -1,  318,   -1,  320,  321,   -1,   -1,  324,
   -1,   -1,   -1,  328,    0,   -1,  331,   -1,  333,   -1,
  335,   45,   -1,   -1,  441,  340,  341,   -1,  343,  470,
   41,   42,   43,   44,   45,  280,   47,   -1,   -1,   -1,
  285,   -1,   -1,   -1,  359,  360,   -1,  362,   59,   60,
   61,   62,   -1,   -1,  378,   41,   -1,   -1,   44,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,   -1,
   -1,  386,  387,   59,   -1,   -1,  400,   -1,   -1,   -1,
   -1,  396,   93,  407, 1333, 1334,   -1,   -1,   -1,   -1,
 1339, 1340,   -1, 1342, 1343,  410,   -1,   -1,  413,   -1,
   -1, 1350, 1351, 1352, 1353,   -1, 1355, 1356, 1357, 1358,
 1359, 1360,  460,   -1,  429,   -1,   -1,  441,   -1,   -1,
  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,   -1,   -1,
  272,   -1,   -1,   -1,   -1,   -1,  451,  452,  280,  454,
   -1,  489,  490,  285,  492,  460,  288,   -1,   -1,   -1,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,    0,    0,  516,  517,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  324,   -1,  499,  500,  501,  502,  503,   -1,
  505,  333,   -1,  508,  509,  510,  511,  512,   -1,  341,
   -1,  516,  517,   -1,   -1,   -1,   -1,   41,   41,   -1,
   44,   44,   -1,   -1,   -1,   -1,   -1,   -1,  360,   -1,
  362,   -1,   -1,   -1,   -1,   59,   59,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  294,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
  305,  272,   -1,   -1,   -1,   -1,   -1,  312,   -1,  280,
   -1,  295,   -1,   -1,  285,   -1,   -1,  288,   -1,   -1,
  325,   -1,   -1,   -1,   -1,   -1,   -1,  429,   -1,   -1,
   -1,  302,  278,    0,  280,   -1,  307,  308,   -1,  323,
  311,  287,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,  287,   -1,  324,  300,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,  352,  340,
  341,   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,
   47,  386,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,  402,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,  419,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,   -1,  434,
   -1,   -1,  378,  438,   -1,   -1,  441,   -1,   -1,  410,
   -1,   -1,  413,  427,  449,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,  429,  443,
   -1,  407,   -1,  468,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,  457,  458,  459,   -1,  461,  462,   -1,
  451,  452,   -1,  454,  278,  278,  280,  280,   -1,  460,
   -1,   -1,   -1,  287,  287,  441,  480,  481,  482,  483,
  484,  485,  486,  487,  488,   -1,   -1,   -1,   -1,   -1,
   -1,  495,  496,   -1,   -1,   -1,   -1,   -1,  489,  490,
  504,  492,   -1,   -1,  460,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,    0,
   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,   -1,
   -1,   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,
   -1,   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,
  516,  517,   -1,   -1,  378,  378,   -1,   -1,   -1,   -1,
   41,   42,   43,   44,   45,  262,   47,  264,  265,   -1,
   -1,   -1,   -1,   -1,   -1,  272,  400,  400,   59,   60,
   61,   62,   -1,  407,  407,   -1,   -1,  328,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   93,   -1,  311,   -1,   -1,  441,  441,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,  359,  360,    0,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   40,   41,   -1,  396,
   44,   -1,  443,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,  410,   -1,   59,  413,   61,   -1,  460,
   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,   -1,   91,  489,  490,
   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,
  501,  502,  503,  460,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  288,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   41,   42,   43,   44,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
  274,   -1,  413,  277,  278,   -1,  280,   -1,   -1,   -1,
  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,  440,
   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,  460,
   -1,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,   -1,   -1,  340,   -1,   -1,  343,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,  359,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,    0,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
   -1,  387,   -1,   -1,   -1,   -1,  400,   -1,   -1,   -1,
  396,   -1,   -1,  407,   -1,   -1,   -1,  411,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,   -1,  413,   -1,   -1,
   41,   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,
   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   59,   60,
   61,   62,   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,
   -1,  288,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,  466,   -1,   -1,  302,  470,   -1,   -1,   -1,
  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,  434,   -1,   -1,   -1,   41,   -1,  396,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  410,   -1,   59,  413,   61,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,   -1,   -1,  260,   91,  489,  490,
   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,
  501,  502,  503,  460,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,  301,   -1,   -1,
   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,  313,
   -1,   -1,  499,  500,  501,  502,  503,  288,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  302,   -1,    0,  338,   -1,  307,  308,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  407,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,  417,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  258,  396,   93,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  442,  410,
  274,   -1,  413,  277,  278,   -1,  280,   -1,   -1,   -1,
  284,   -1,   -1,  287,   -1,   -1,  460,   -1,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,  300,   -1,   -1,  440,
   -1,   -1,  306,   -1,   -1,  309,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,   -1,  489,  490,   -1,  492,  460,
   -1,   -1,   -1,   -1,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  378,  516,  517,   -1,   -1,    0,
  273,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  400,   -1,  291,   -1,
   -1,   -1,  295,  407,   -1,  298,   -1,  411,   -1,   -1,
   -1,   -1,   -1,   -1,  418,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   43,   44,   45,  262,   -1,  264,  265,   -1,
   -1,   -1,  436,   -1,   -1,  272,   -1,  441,   59,   60,
   61,   62,   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,
   -1,  288,  345,  346,  347,   -1,  349,  350,  351,   -1,
  353,   -1,  466,   -1,   -1,  302,  470,   -1,   -1,   -1,
  307,  308,   93,   -1,  311,   -1,   -1,   -1,   -1,   -1,
  373,  318,  375,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,  406,   -1,   -1,    0,   -1,  411,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,    0,   -1,   -1,
  423,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,  445,   -1,   -1,   -1,   -1,   41,   -1,  396,
   44,   -1,  443,   -1,   -1,   -1,   -1,   -1,   41,   -1,
   -1,   44,   -1,  410,   -1,   59,  413,   -1,   -1,  460,
   -1,   -1,   -1,   -1,   -1,   -1,   59,  480,  481,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,   -1,  440,  497,   -1,   -1,   -1,  489,  490,
   -1,  492,   -1,   -1,  451,  452,   -1,  454,  499,  500,
  501,  502,  503,  460,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,  262,   -1,  264,  265,   -1,   -1,   -1,   -1,   -1,
   -1,  272,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,  288,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  302,   -1,    0,   -1,   -1,  307,  308,   -1,   -1,
  311,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  320,
  321,   -1,   -1,  324,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,  340,
  341,   -1,  343,   -1,   41,   -1,   43,   44,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,  360,
   -1,  362,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  376,   -1,  378,  379,  380,
   -1,   -1,  383,    0,   -1,  386,  387,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   93,   -1,  262,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  410,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   41,   -1,   -1,   44,  429,   -1,
   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   59,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  451,  452,   -1,  454,  318,   -1,   -1,  321,   -1,  460,
   -1,   -1,   -1,   -1,  328,  318,   -1,  331,  321,   -1,
   -1,  335,   -1,   -1,   -1,  328,  340,   -1,  331,  343,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,  489,  490,
  343,  492,   -1,   -1,   -1,  359,  460,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  359,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,  383,
   -1,   -1,  386,  387,   -1,  489,  490,   -1,  492,   -1,
  383,   -1,  396,  386,  387,  499,  500,  501,  502,  503,
   -1,  505,   -1,  396,  508,  509,  510,  511,  512,  413,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,  262,   -1,  264,  265,   -1,
   -1,  435,   -1,   -1,   -1,  272,  440,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,  452,   -1,
  454,  288,   -1,   -1,   -1,   40,   -1,   -1,   43,  452,
   45,  454,   -1,   -1,   -1,  302,   -1,   -1,   -1,   -1,
  307,  308,   -1,   58,  311,   -1,   -1,   -1,   63,   -1,
   -1,  318,   -1,  320,  321,   -1,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,  396,
   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,
   58,  328,   -1,  410,  331,   63,  413,   -1,  335,   -1,
   -1,   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,
   -1,   -1,  429,   -1,   -1,   -1,   -1,   -1,  435,   -1,
   -1,   -1,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  451,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,  383,   -1,   -1,   -1,
  387,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  396,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,   -1,  492,  413,   -1,   -1,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,  267,  440,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  276,   -1,   -1,   -1,  452,   -1,  454,   -1,   -1,
   -1,   -1,   -1,  460,  289,   -1,   -1,   -1,   -1,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,
   -1,   -1,  489,  490,   -1,  492,   -1,   -1,  323,   -1,
   -1,  326,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   41,  516,
  517,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,  267,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  276,   -1,
   -1,   -1,   -1,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  289,   -1,   -1,  292,   -1,   -1,  295,  296,   -1,
   40,   -1,   42,   43,   -1,   45,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   58,   -1,
   -1,   -1,  417,   63,   -1,  323,  421,   -1,  326,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  378,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,  460,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,   -1,   -1,  508,  509,  510,  511,  512,  513,  514,
  515,  516,  517,  421,   -1,   -1,   -1,  489,  490,  427,
  492,   -1,   -1,   -1,   -1,  433,   -1,  499,  500,  501,
  502,  503,   -1,  505,   -1,  443,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,  262,
   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
  260,   -1,   -1,   -1,   -1,  318,   -1,  267,  321,   -1,
   -1,   -1,   -1,   -1,   -1,  328,  276,   -1,  331,   -1,
   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,   -1,  289,
  343,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   40,
   -1,   42,   43,  303,   45,   -1,  359,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,  323,   -1,   -1,  326,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,   -1,   -1,    0,
   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  352,   -1,  354,  355,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,  435,   44,   -1,   40,  261,  440,   43,   -1,
   45,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   59,  452,
   -1,  454,   -1,   58,   -1,   -1,   -1,  282,   63,   -1,
   -1,   -1,   -1,   -1,   -1,  290,   -1,   -1,   -1,   -1,
   -1,   -1,  297,   -1,  299,   -1,  301,  427,   -1,   -1,
  343,  306,   -1,  433,   -1,   -1,   -1,   -1,  313,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  327,   -1,   -1,  368,   -1,  457,  458,  459,
  460,  461,  462,  338,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,  426,   -1,   -1,  516,  517,   -1,  260,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
  405,   -1,   -1,   -1,   -1,  276,   -1,  412,   -1,   -1,
  415,   -1,  417,  418,   -1,   -1,   -1,  460,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,  303,   -1,   -1,   -1,   -1,  442,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  489,  490,   -1,  492,
   -1,   -1,  323,   -1,   -1,  326,  499,  500,  501,  502,
  503,   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,  262,   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,  352,  267,  354,  355,   40,  491,   -1,   43,   -1,
   45,  276,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,  506,  507,   58,  289,   -1,   -1,   -1,   63,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,  314,
  321,   -1,   -1,   -1,   -1,   -1,   -1,  328,  323,   -1,
  331,  326,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,  359,  354,
  355,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,  457,  458,  459,  460,
  461,  462,  383,  378,   -1,  386,  387,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,  413,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  421,  508,  509,  510,
  511,  512,  427,   -1,  435,  516,  517,   -1,  433,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,  267,  498,  499,  500,  501,  502,  503,  504,
  505,  276,   -1,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,   40,  289,   -1,   43,   -1,   45,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,  314,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,   -1,
   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   42,   43,   -1,   45,   -1,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  354,
  355,   -1,   -1,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,  273,   -1,
   -1,   -1,  277,  378,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,  291,   -1,   -1,   -1,
  295,   -1,   -1,  298,   -1,   -1,   -1,   -1,   58,   -1,
   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  421,   -1,   -1,   -1,
   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,  433,   -1,
   -1,   -1,   -1,   -1,  339,   -1,   -1,   -1,  443,   -1,
  345,  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,
   -1,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,  373,   -1,
  375,   -1,   -1,   -1,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  406,   -1,  508,  509,  510,  511,  512,   -1,   -1,
  267,  516,  517,   -1,   -1,   -1,   -1,   -1,  423,  276,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  314,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,  323,   -1,   -1,  326,
   -1,   -1,   -1,  280,   -1,  480,  481,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  497,   -1,   -1,   -1,   -1,  354,  355,   -1,
  260,    0,   -1,  263,   -1,   -1,   -1,  267,   -1,  366,
   -1,   -1,   -1,  370,   -1,   -1,  276,  324,   -1,   -1,
   -1,  378,   -1,   -1,   -1,   -1,  333,   -1,   -1,  289,
   -1,   -1,   -1,   -1,  341,  295,  296,   -1,   40,   -1,
   -1,   43,   41,   45,   -1,   44,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  360,   -1,  362,   58,   -1,   -1,   -1,
   59,   63,   61,  323,  421,   -1,  326,   -1,   -1,  376,
  427,  378,  379,  380,   -1,   -1,  433,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  443,   -1,   -1,   -1,
   -1,   -1,   91,   -1,  354,  355,   -1,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  429,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   40,   -1,   -1,   43,   -1,   45,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   58,   -1,
  328,  471,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,  260,  258,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  276,  274,   -1,   -1,   -1,  278,
   -1,  280,   -1,   -1,  402,  284,   -1,  289,  287,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   40,
   -1,  300,   43,   -1,   45,   -1,  460,  306,   -1,   -1,
  309,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
  438,  323,   63,   -1,  326,  443,   -1,   -1,   -1,   -1,
   -1,  449,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,
   -1,   -1,  460,   -1,   -1,  499,  500,  501,  502,  503,
  468,  505,  354,  355,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  366,   -1,   -1,   -1,  370,   -1,
   -1,  489,  490,   -1,  492,   -1,   -1,   -1,   -1,  378,
   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,
  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,
   -1,  400,   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,
  260,   -1,  411,  263,   -1,   -1,   -1,  267,   -1,  418,
   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,  436,   -1,  289,
   -1,  443,  441,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,  466,   -1,  471,
   -1,  470,   -1,  323,   -1,   -1,  326,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  354,  355,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  366,   -1,   -1,  260,
  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  425,   58,  427,   -1,   -1,
   -1,   63,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,  271,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,  301,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  312,  313,   -1,  315,   -1,
   40,   -1,   -1,   43,   -1,   45,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,  332,   -1,   -1,   58,   -1,
  471,  338,   -1,   63,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,   -1,  516,  517,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
   -1,  398,   -1,   -1,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,
  417,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   40,
   -1,   -1,   43,   -1,   45,  432,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  442,   -1,   58,   -1,   -1,
   -1,  323,   63,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  489,  490,  366,  492,   -1,   -1,  370,   -1,
   -1,   -1,  499,  500,  501,  502,  503,   -1,  505,   -1,
   -1,  508,  509,  510,  511,  512,   -1,   -1,   -1,  516,
  517,  518,  519,  520,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,  276,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,
   -1,  443,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  354,  355,  508,  509,  510,  511,
  512,   -1,    0,   -1,  516,  517,  366,   -1,   -1,  260,
  370,   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  276,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,   -1,   -1,   41,  295,  296,   44,   -1,   40,   -1,
   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   59,   -1,   -1,   -1,  425,   58,  427,   -1,   -1,
   -1,   63,  323,  433,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,
  460,  461,  462,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,  471,   -1,   -1,   -1,  366,   -1,   -1,   -1,  370,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,   -1,   -1,  508,  509,
  510,  511,  512,   -1,   -1,   -1,  516,  517,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  425,   -1,  427,   -1,   -1,   -1,
   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  262,  516,  517,   -1,  260,   -1,
   -1,  263,   -1,   -1,   -1,  267,   -1,   -1,   40,   -1,
   42,   43,   -1,   45,  276,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   58,  289,   -1,   -1,
   -1,   63,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,  323,   -1,  331,  326,   -1,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  366,   -1,   40,   -1,  370,   43,
   -1,   45,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,  396,   63,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  425,   -1,  427,   -1,  435,   -1,   -1,
   -1,  433,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  452,   -1,  454,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,  471,
   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,  295,
  492,   -1,  298,  495,  496,  267,  498,  499,  500,  501,
  502,  503,  504,  505,  276,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   -1,   -1,    0,   -1,   -1,
   -1,   -1,   -1,  339,   -1,   -1,   -1,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   41,  375,
   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  352,   -1,  354,  355,   -1,   -1,   59,   -1,   -1,   -1,
   -1,   -1,   -1,  267,  366,   -1,   -1,   -1,  370,   -1,
  406,   -1,  276,   -1,   -1,   40,   -1,   42,   43,   -1,
   45,   -1,   -1,   -1,   -1,  289,   -1,  423,   -1,   -1,
   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  445,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  443,   -1,   -1,  480,  481,   -1,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,  457,  458,  459,  460,  461,
  462,  497,  366,   -1,   -1,   -1,  370,   -1,   -1,  471,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,   -1,   -1,  508,  509,  510,  511,
  512,   -1,   -1,  417,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  427,   -1,   -1,   -1,   40,   -1,  433,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,  457,  458,  459,  460,  461,  462,  262,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,   -1,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,  267,   -1,  508,  509,  510,  511,  512,   -1,
   -1,  276,  516,  517,   -1,  318,   -1,   -1,  321,   -1,
   -1,   -1,    0,   -1,  289,  328,   -1,   -1,  331,   -1,
  295,  296,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,  323,   -1,
   -1,  326,   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   59,   -1,  386,  387,   -1,   -1,  352,   -1,  354,
  355,   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  366,    0,   -1,   -1,  370,   -1,   -1,   -1,   -1,
  413,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,   -1,
   -1,   -1,   -1,   41,   -1,   -1,   44,  260,   -1,  452,
   -1,  454,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,   59,  427,  276,   -1,   -1,   40,   -1,  433,   43,
   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,  443,   -1,
   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,   63,
  303,   -1,  457,  458,  459,  460,  461,  462,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,  480,  481,  482,  483,  484,
  485,  486,  487,  488,  489,  490,   -1,  492,   -1,   -1,
  495,  496,   -1,  498,  499,  500,  501,  502,  503,  504,
  505,  354,  355,  508,  509,  510,  511,  512,   -1,   -1,
   -1,  516,  517,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   -1,   -1,  262,   -1,   63,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,   -1,
  433,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  318,   -1,   -1,  321,  457,  458,  459,  460,  461,  462,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  471,   -1,
   -1,   -1,  340,   -1,  262,  343,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,  359,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,  267,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  276,  516,  517,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,  289,   -1,   -1,  396,  260,
  318,  295,  296,  321,   -1,   -1,   -1,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,  413,   -1,  335,   -1,   -1,
   -1,   -1,  340,   -1,   -1,  343,   -1,   -1,   -1,  323,
   -1,   -1,  326,   -1,   -1,   -1,   -1,  435,   -1,   -1,
  301,  359,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  313,   -1,  452,   -1,  454,   -1,  352,   -1,
  354,  355,   -1,   -1,   -1,  383,  327,   -1,  386,  387,
  267,   -1,  366,   -1,   -1,   -1,  370,  338,  396,  276,
   -1,   -1,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,
   -1,   -1,  289,   -1,   -1,  413,   -1,   -1,  295,  296,
   58,   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,
   -1,   -1,  440,   -1,   -1,   -1,  323,   -1,   -1,  326,
   -1,   -1,   -1,  427,  452,   -1,  454,   -1,   -1,  433,
   -1,   -1,   -1,   -1,   -1,   -1,  407,   -1,   -1,  443,
   -1,   -1,   -1,   -1,   -1,   -1,  417,  354,  355,   -1,
   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,  366,
   -1,   -1,   -1,  370,   -1,   -1,   -1,  471,   -1,   -1,
   -1,  442,   -1,   -1,   -1,   -1,  480,  481,  482,  483,
  484,  485,  486,  487,  488,  489,  490,   -1,  492,  460,
   -1,  495,  496,   -1,  498,  499,  500,  501,  502,  503,
  504,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,   -1,   -1,   -1,   -1,  489,  490,
  427,  492,   -1,   -1,   -1,   -1,  433,   -1,  499,  500,
  501,  502,  503,   -1,  505,   -1,  443,  508,  509,  510,
  511,  512,   -1,   -1,  451,  516,  517,   -1,   -1,   -1,
  457,  458,  459,  460,  461,  462,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  480,  481,  482,  483,  484,  485,  486,
  487,  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,
   -1,  498,  499,  500,  501,  502,  503,  504,  505,  267,
   -1,  508,  509,  510,  511,  512,   -1,   -1,  276,  516,
  517,   40,   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,
   -1,  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   58,
   -1,   -1,   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  354,  355,   -1,   -1,
   -1,   40,   41,   -1,   43,   -1,   45,   -1,  366,   -1,
   -1,   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,
   -1,   -1,  460,   -1,   63,   -1,   -1,   -1,   -1,   -1,
   -1,  469,  470,  471,  472,  473,  474,  475,  476,  477,
  478,  479,  480,  481,  482,  483,  484,   -1,   -1,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,  417,
   -1,  499,  500,  501,  502,  503,  504,  505,  506,  427,
  508,  509,  510,  511,  512,  433,   -1,   -1,  516,  517,
  518,  519,  520,   -1,   -1,  443,   -1,   -1,   -1,  273,
   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,  457,
  458,  459,  460,  461,  462,   -1,   -1,  291,   -1,   -1,
   -1,  295,   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  480,  481,  482,  483,  484,  485,  486,  487,
  488,  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,
  498,  499,  500,  501,  502,  503,  504,  505,  267,   -1,
  508,  509,  510,  511,  512,  339,   -1,  276,  516,  517,
   -1,  345,  346,  347,   -1,  349,  350,  351,   -1,  353,
  289,   -1,   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,
   -1,  375,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,
   -1,   -1,   -1,  352,   -1,  354,  355,  276,   -1,  423,
   40,   -1,   -1,   43,   -1,   45,   -1,  366,   -1,   -1,
  289,  370,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,
   -1,  445,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,
   -1,   -1,   -1,  497,  433,  354,  355,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,
   -1,  370,   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,  271,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,   -1,  427,  508,
  509,  510,  511,  512,  433,   -1,   -1,  516,  517,   -1,
   -1,   -1,  301,   -1,  443,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  312,  313,   -1,  315,   -1,  457,  458,
  459,  460,  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  471,  332,   -1,   -1,   -1,   -1,   -1,  338,
   -1,  480,  481,  482,  483,  484,  485,  486,  487,  488,
  489,  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,
  499,  500,  501,  502,  503,  504,  505,  267,   -1,  508,
  509,  510,  511,  512,   -1,   -1,  276,  516,  517,   40,
   -1,   -1,   43,   -1,   45,   -1,   -1,   -1,   -1,  289,
   -1,   -1,   -1,   -1,   -1,  295,  296,   58,   -1,  398,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  417,   -1,
   -1,   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,
   -1,   -1,   -1,  432,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  442,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  451,   -1,  354,  355,   -1,   -1,   -1,   40,
   -1,  460,   43,   -1,   45,   -1,  366,   -1,   -1,   -1,
  370,   -1,   -1,   -1,   -1,   -1,   -1,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  489,  490,   -1,  492,   -1,   -1,   -1,   -1,   -1,   -1,
  499,  500,  501,  502,  503,   -1,  505,   -1,   -1,  508,
  509,  510,  511,  512,   -1,   -1,   -1,  516,  517,  518,
  519,  520,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,
   -1,   -1,   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  443,   -1,   -1,   -1,  273,   -1,   -1,
   -1,  277,   -1,   -1,  454,   -1,   -1,  457,  458,  459,
  460,  461,  462,   -1,   -1,   -1,   -1,  293,   -1,  295,
   -1,  471,  298,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  480,  481,  482,  483,  484,  485,  486,  487,  488,  489,
  490,   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,
  500,  501,  502,  503,  504,  505,  267,   -1,  508,  509,
  510,  511,  512,  339,   -1,  276,  516,  517,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,  289,   -1,
   -1,   -1,   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  373,   -1,  375,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  406,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,
   -1,   -1,   -1,  354,  355,  276,   -1,  423,   40,   -1,
   -1,   43,   -1,   45,   -1,  366,   -1,   -1,  289,  370,
   -1,   -1,   -1,   -1,  295,  296,   58,   59,   -1,  445,
   -1,   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  480,  481,  417,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,
   -1,  497,  433,  354,  355,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,  366,   -1,   -1,   -1,  370,
   -1,   -1,   -1,   -1,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,   -1,  427,  508,  509,  510,
  511,  512,  433,   -1,   -1,  516,  517,   -1,   -1,   -1,
   -1,   -1,  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  454,   -1,   -1,  457,  458,  459,  460,
  461,  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  471,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,
  481,  482,  483,  484,  485,  486,  487,  488,  489,  490,
   -1,  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,
  501,  502,  503,  504,  505,  267,   -1,  508,  509,  510,
  511,  512,   -1,   -1,  276,  516,  517,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,  289,   -1,   -1,
   -1,   -1,   -1,  295,  296,   58,   -1,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  354,  355,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,  366,   -1,   -1,   -1,  370,   -1,
   -1,   -1,  273,   -1,   -1,   58,  277,   -1,   -1,   -1,
   63,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  295,   -1,   -1,  298,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  427,   -1,   -1,   -1,   -1,
   -1,  433,   -1,   -1,   -1,   -1,   -1,   -1,  339,   -1,
   -1,  443,   -1,   -1,  345,  346,  347,   -1,  349,  350,
  351,   -1,  353,   -1,   -1,  457,  458,  459,  460,  461,
  462,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,
   -1,   -1,  373,   -1,  375,   -1,   -1,   -1,  480,  481,
  482,  483,  484,  485,  486,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,
  502,  503,  504,  505,  267,  406,  508,  509,  510,  511,
  512,   -1,   -1,  276,  516,  517,   -1,   -1,   -1,   -1,
   -1,   -1,  423,   -1,   -1,   -1,  289,   -1,   -1,   -1,
   -1,   -1,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  445,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  480,
  481,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  354,  355,  276,   -1,   -1,  497,   -1,   -1,   -1,
   -1,   -1,   -1,  366,   -1,   -1,  289,  370,   -1,   -1,
    0,   -1,  295,  296,   -1,   41,   -1,   -1,   44,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   59,   -1,   61,   -1,   -1,   -1,   -1,
  323,   -1,   -1,  326,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  427,   91,   -1,   -1,   -1,   59,
  433,  354,  355,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  443,   -1,   -1,  366,   -1,   -1,   -1,  370,   -1,   -1,
   -1,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,    0,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,  427,  508,  509,  510,  511,  512,
  433,   -1,   41,  516,  517,   44,   -1,   -1,   -1,   -1,
  443,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   59,   -1,   -1,   -1,  457,  458,  459,  460,  461,  462,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  471,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  480,  481,  482,
  483,  484,  485,  486,  487,  488,  489,  490,   -1,  492,
   -1,   -1,  495,  496,   -1,  498,  499,  500,  501,  502,
  503,  504,  505,   -1,   -1,  508,  509,  510,  511,  512,
   -1,   -1,  258,  516,  517,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,    0,   -1,   -1,   -1,   -1,  274,   -1,
   -1,   -1,  278,   -1,  280,   -1,   -1,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  262,   -1,  300,   -1,   -1,   -1,   -1,   -1,
  306,   -1,   -1,  309,   41,   42,   43,   44,   45,   46,
   47,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   60,   61,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,
   -1,  321,   -1,   -1,   91,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,   -1,  335,    0,   -1,   -1,   -1,
  340,   -1,  378,  343,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,  262,  400,    0,   -1,   -1,   -1,   -1,
   -1,  407,   -1,   -1,   -1,  411,   -1,   41,   -1,   -1,
   44,   -1,  418,  383,   -1,   -1,  386,  387,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,  396,   -1,   -1,   -1,
  436,   -1,   -1,   -1,   -1,  441,   41,   42,   43,   44,
   45,   46,   47,  413,   -1,   -1,   -1,   -1,   -1,  318,
   -1,   -1,  321,   -1,   59,   60,   61,   62,   -1,  328,
  466,   -1,  331,   -1,  470,  435,  335,   -1,   -1,   -1,
  440,  340,   -1,   -1,  343,   -1,   -1,   -1,   -1,    0,
   -1,   -1,  452,   -1,  454,   -1,   91,   -1,   -1,   -1,
  359,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  383,   -1,   -1,  386,  387,   -1,
   41,   -1,   -1,   44,   -1,  262,   -1,  396,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,   59,   -1,
   -1,   -1,   -1,  280,  413,   -1,   -1,   -1,  285,   -1,
   -1,  288,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  435,   -1,   -1,   -1,
    0,  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  318,   -1,  452,  321,  454,   -1,  324,   -1,   -1,
   -1,  328,   -1,   -1,  331,   -1,  333,   -1,  335,   -1,
   -1,   -1,   -1,  340,  341,   -1,  343,   -1,   -1,   -1,
   -1,   41,   42,   43,   44,   45,   46,   47,   -1,   -1,
   -1,   -1,  359,  360,   -1,  362,   -1,   -1,  262,   59,
   60,   61,   62,   -1,   -1,   -1,   -1,   -1,   -1,  376,
   -1,  378,  379,  380,   -1,   -1,  383,   -1,   -1,  386,
  387,   -1,   -1,   -1,   -1,   -1,   -1,  262,   -1,  396,
   -1,   91,   -1,   -1,   -1,   -1,   -1,  272,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  280,  413,   -1,   -1,   -1,
  285,   -1,   -1,  288,  318,   -1,   -1,  321,   -1,   -1,
   -1,   -1,  429,   -1,  328,   -1,   -1,  331,  435,    0,
   -1,  335,   -1,  440,   -1,   -1,  340,   -1,   -1,  343,
   -1,   -1,   -1,  318,   -1,  452,  321,  454,   -1,  324,
   -1,   -1,   -1,  328,   -1,  359,  331,   -1,  333,   -1,
  335,   -1,   -1,   -1,   -1,  340,  341,   -1,  343,   -1,
   41,  262,   -1,   44,    0,   -1,   -1,   -1,   -1,  383,
   -1,   -1,  386,  387,  359,  360,   -1,  362,   59,   -1,
   -1,   -1,  396,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  376,   -1,  378,  379,  380,   -1,   -1,  383,  413,
   -1,  386,  387,   -1,   -1,   41,   -1,   -1,   44,   -1,
   -1,  396,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,  435,   -1,   59,   -1,   -1,  440,  328,  413,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,  452,  340,
  454,   -1,  343,   -1,  429,   -1,   -1,    0,   -1,   -1,
  435,   -1,  262,   -1,   -1,  440,   -1,   -1,  359,   -1,
   -1,   -1,  272,   -1,   -1,   -1,   -1,  452,   -1,  454,
  280,   -1,   -1,   -1,   -1,  285,   -1,   -1,  288,   -1,
   -1,   -1,  383,   -1,   -1,  386,  387,   -1,   41,   -1,
   -1,   44,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   59,   -1,  318,   -1,
   -1,  321,  413,   -1,  324,   -1,   -1,   -1,  328,    0,
   -1,  331,   -1,  333,   -1,  335,   -1,   -1,   -1,   -1,
  340,  341,   -1,  343,  435,   -1,   -1,   -1,   -1,  440,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    0,  359,
  360,  452,  362,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   41,   -1,   -1,   44,   -1,   -1,  376,   -1,  378,  379,
  380,   -1,   -1,  383,   -1,   -1,  386,  387,   59,   -1,
   -1,  262,   -1,   -1,   -1,   -1,  396,   -1,   -1,   41,
   -1,   -1,   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  413,   -1,   -1,   -1,   59,    0,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  429,
   -1,   -1,   -1,   -1,   -1,  435,  262,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,
  321,   -1,  452,   -1,  454,   -1,   -1,  328,   -1,   41,
  331,   -1,   44,   -1,  335,   -1,   -1,   -1,   -1,  340,
   -1,   -1,  343,   -1,   -1,   -1,   -1,   59,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,   -1,
    0,   -1,  318,   -1,   -1,  321,   -1,   -1,   -1,   -1,
   -1,   -1,  328,   -1,   -1,  331,   -1,   -1,   -1,  335,
   -1,   -1,  383,   -1,  340,  386,  387,  343,   -1,  262,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   41,   -1,  359,   44,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  413,   -1,   -1,   -1,   -1,   -1,   -1,   59,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  383,   -1,   -1,
  386,  387,    0,   -1,  435,   -1,   -1,   -1,   -1,  440,
  396,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,   -1,
   -1,  452,   -1,  454,   -1,  328,   -1,  413,  331,   -1,
   -1,  262,  335,   -1,   -1,   -1,   -1,  340,   -1,   -1,
  343,   -1,   -1,   41,   -1,   -1,   44,   -1,   -1,  435,
   -1,   -1,   -1,   -1,  440,   -1,  359,   -1,   -1,   -1,
  262,   59,   -1,   -1,   -1,   -1,  452,   -1,  454,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  383,   -1,   -1,  386,  387,   -1,   -1,  318,   -1,   -1,
  321,   -1,   -1,  396,   -1,   -1,   -1,  328,   -1,   -1,
  331,   -1,   -1,   -1,  335,   -1,   -1,   -1,   -1,  340,
  413,   -1,  343,   -1,   -1,   -1,  318,   -1,   -1,  321,
  262,   -1,   -1,   -1,   -1,   -1,  328,   -1,  359,  331,
   -1,   -1,  435,  335,   -1,   -1,   -1,  440,  340,   -1,
   -1,  343,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  452,
   -1,  454,  383,   -1,   -1,  386,  387,  359,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  396,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  318,   -1,   -1,  321,
   -1,  383,  413,   -1,  386,  387,  328,   -1,   -1,  331,
   -1,   -1,  262,  335,  396,   -1,   -1,   -1,  340,   -1,
   -1,  343,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,
   -1,  413,   -1,   -1,   -1,   -1,   -1,  359,   -1,   -1,
   -1,  452,   -1,  454,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,  383,   -1,   -1,  386,  387,   -1,   -1,  318,   -1,
  452,  321,  454,   -1,  396,   -1,   -1,   -1,  328,   -1,
   -1,  331,   -1,   -1,  262,  335,   -1,   -1,   -1,   -1,
  340,  413,   -1,  343,   -1,   -1,   -1,  271,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  359,
   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,  440,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  301,   -1,   -1,
  452,   -1,  454,  383,  308,   -1,  386,  387,  312,  313,
  318,  315,   -1,  321,  318,   -1,  396,   -1,   -1,   -1,
  328,   -1,   -1,  331,   -1,   -1,   -1,  335,  332,   -1,
   -1,   -1,  340,  413,  338,  343,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  359,   -1,   -1,   -1,  435,   -1,   -1,   -1,   -1,
  440,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  452,   -1,  454,  383,   -1,   -1,  386,  387,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,  396,   -1,
   -1,  277,   -1,   -1,  398,   -1,   -1,   -1,  284,   -1,
   -1,   -1,   -1,   -1,   -1,  413,   -1,   -1,   -1,  295,
   -1,   -1,  298,  417,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  435,  432,   -1,
   -1,   -1,  440,   -1,   -1,   -1,   -1,   -1,  442,   -1,
   -1,   -1,   -1,   -1,  452,   -1,  454,  451,   -1,  453,
   -1,   -1,   -1,  339,   -1,   -1,  460,   -1,   -1,  345,
  346,  347,   -1,  349,  350,  351,   -1,  353,  273,   -1,
   -1,   -1,  277,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  489,  490,  373,  492,  375,
  295,   -1,   -1,  298,   -1,  499,  500,  501,  502,  503,
   -1,  505,   -1,   -1,  508,  509,  510,  511,  512,   -1,
   -1,   -1,  516,  517,  518,  519,  520,   -1,   -1,   -1,
  406,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
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
  516,  517,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  445,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  460,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  480,  481,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  489,  490,   -1,  492,   -1,   -1,
   -1,   -1,  497,   -1,  499,  500,  501,  502,  503,   -1,
  505,   -1,   -1,  508,  509,  510,  511,  512,  460,   -1,
   -1,  516,  517,   -1,   -1,   -1,   -1,   -1,  470,  471,
  472,  473,  474,  475,  476,  477,  478,  479,  480,  481,
  482,  483,  484,   -1,   -1,  487,  488,  489,  490,   -1,
  492,   -1,   -1,  495,  496,   -1,   -1,  499,  500,  501,
  502,  503,  504,  505,  506,   -1,  508,  509,  510,  511,
  512,   -1,   -1,   -1,  516,  517,  518,  519,  520,
};
#define YYFINAL 20
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 520
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
"KW_UPDATING","KW_DELETING","BACKUP","KW_DIFFERENCE","OPEN","CLOSE","FETCH",
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
"procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"rprocedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"replace_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"alter_procedure_clause : symbol_procedure_name input_parameters output_parameters AS begin_string local_declaration_list full_proc_block end_trigger",
"input_parameters : '(' proc_parameters ')'",
"input_parameters :",
"output_parameters : RETURNS input_parameters",
"output_parameters :",
"proc_parameters : proc_parameter",
"proc_parameters : proc_parameters ',' proc_parameter",
"proc_parameter : simple_column_def_name non_array_type",
"local_declaration_list : local_declarations",
"local_declaration_list :",
"local_declarations : local_declaration",
"local_declarations : local_declarations local_declaration",
"local_declaration : DECLARE var_decl_opt local_declaration_item ';'",
"local_declaration_item : var_declaration_item",
"local_declaration_item : cursor_declaration_item",
"var_declaration_item : column_def_name non_array_type var_init_opt",
"var_decl_opt : VARIABLE",
"var_decl_opt :",
"var_init_opt : '=' default_value",
"var_init_opt : default_opt",
"cursor_declaration_item : symbol_cursor_name CURSOR FOR '(' ordered_select_expr ')'",
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
"proc_statement : cursor_statement ';'",
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
"proc_inputs : rhs_list",
"proc_inputs : '(' rhs_list ')'",
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
"cursor_statement : open_cursor",
"cursor_statement : fetch_cursor",
"cursor_statement : close_cursor",
"open_cursor : OPEN symbol_cursor_name",
"close_cursor : CLOSE symbol_cursor_name",
"fetch_cursor : FETCH fetch_opt symbol_cursor_name INTO variable_list",
"fetch_opt :",
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
"trigger_action : AS begin_trigger local_declaration_list full_proc_block",
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
"keyword_or_column : OPEN",
"keyword_or_column : CLOSE",
"keyword_or_column : FETCH",
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
"table_proc : symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name",
"table_proc : symbol_procedure_name table_proc_inputs",
"table_proc_inputs : '(' rhs_list ')'",
"table_proc_inputs :",
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
"insert : INSERT INTO simple_table_name ins_column_parens_opt VALUES '(' rhs_list ')'",
"insert : INSERT INTO simple_table_name ins_column_parens_opt ordered_select_expr",
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
"rhs_list : rhs",
"rhs_list : rhs_list ',' rhs",
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
"case_abbreviation : COALESCE '(' rhs ',' rhs_list ')'",
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
"case_result : rhs",
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

int DSQL_yyerrflag;


int DSQL_yychar;

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
		DSQL_SYM symbol = FB_NEW_RPT(*DSQL_permanent_pool, 0) dsql_sym;
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
#ifdef DSQL_DEBUG
	if (DSQL_debug & 32)
		dsql_trace("Source DSQL string:\n%.*s", (int)length, string);
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
#ifdef DSQL_DEBUG
	DSQL_debug = level;
#endif
	if (level >> 8)
		*yydeb = level >> 8;
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
	DSQL_SYM	sym;
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
		check_copy_incr(p, UPPER7(*ptr), string);
		}
		
		check_bound(p, string);
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
		check_copy_incr(p, UPPER (c), string);
		for (; ptr < end && classes [*ptr] & CHR_IDENT; ptr++)
		{
			if (ptr >= end)
				return -1;
			check_copy_incr(p, UPPER (*ptr), string);
		}

		check_bound(p, string);
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
{ yyval = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 238:
{ yyval = make_node (nod_unique, 2, NULL, yyvsp[0]); }
break;
case 239:
{ yyval = make_node (nod_primary, (int) e_pri_count, NULL, yyvsp[0]); }
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
{ yyval = make_node (nod_primary, (int) e_pri_count, yyvsp[-1], yyvsp[0]); }
break;
case 249:
{ yyval = make_node (nod_foreign, (int) e_for_count, yyvsp[-5], yyvsp[-3], 
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
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], NULL);}
break;
case 254:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, yyvsp[0]);}
break;
case 255:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[0], yyvsp[-1]); }
break;
case 256:
{ yyval = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, yyvsp[-1], yyvsp[0]);}
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
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
break;
case 261:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
break;
case 262:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
break;
case 263:
{ yyval = make_flag_node (nod_ref_trig_action, 
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
break;
case 264:
{ yyval = make_node (nod_def_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 265:
{ yyval = make_node (nod_redef_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 266:
{ yyval = make_node (nod_replace_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 267:
{ yyval = make_node (nod_mod_procedure,
						(int) e_prc_count, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
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
{ yyval = yyvsp[-1]; }
break;
case 282:
{ yyval = make_node (nod_def_field, (int) e_dfl_count, 
				yyvsp[-2], yyvsp[0], NULL, NULL, NULL, NULL, NULL); }
break;
case 283:
{ yyval = NULL; }
break;
case 284:
{ yyval = NULL; }
break;
case 285:
{ yyval = yyvsp[0]; }
break;
case 286:
{ yyval = yyvsp[0]; }
break;
case 287:
{ yyval = make_node (nod_cursor, (int) e_cur_count,
				yyvsp[-5], yyvsp[-1], NULL, NULL); }
break;
case 290:
{ yyval = yyvsp[-1]; }
break;
case 291:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[0]), NULL); }
break;
case 292:
{ yyval = make_node (nod_block, (int) e_blk_count, make_list (yyvsp[-1]), make_list (yyvsp[0])); }
break;
case 293:
{ yyval = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
break;
case 295:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 311:
{ yyval = make_node (nod_return, (int) e_rtn_count, NULL); }
break;
case 312:
{ yyval = make_node (nod_exit, 0, NULL); }
break;
case 315:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-1], NULL); }
break;
case 316:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, yyvsp[-2], yyvsp[-1]); }
break;
case 317:
{ yyval = make_node (nod_exception_stmt, (int) e_xcp_count, NULL, NULL); }
break;
case 318:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-3],
					  yyvsp[-2], yyvsp[-1]); }
break;
case 319:
{ yyval = make_node (nod_exec_sql, (int) e_exec_sql_count, yyvsp[-1]); }
break;
case 322:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-5],
					  make_list (yyvsp[-3]), yyvsp[-2], yyvsp[0], yyvsp[-7]); }
break;
case 323:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-4], yyvsp[0], make_list (yyvsp[-2]), yyvsp[-8]); }
break;
case 324:
{ yyval = make_node (nod_exec_into, (int) e_exec_into_count, yyvsp[-3], 0, make_list (yyvsp[-1])); }
break;
case 325:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-5], yyvsp[-2], yyvsp[0]); }
break;
case 326:
{ yyval = make_node (nod_if, (int) e_if_count, yyvsp[-3], yyvsp[0], NULL); }
break;
case 327:
{ yyval = make_node (nod_post, (int) e_pst_count, yyvsp[-2], yyvsp[-1]); }
break;
case 328:
{ yyval = NULL; }
break;
case 329:
{ yyval = make_node (nod_for_select, (int) e_flp_count, yyvsp[-3],
					  make_list (yyvsp[-1]), NULL, NULL); }
break;
case 330:
{ yyval = make_node (nod_var_name, (int) e_vrn_count, 
							yyvsp[0]); }
break;
case 331:
{ yyval = make_list (yyvsp[0]); }
break;
case 332:
{ yyval = make_list (yyvsp[-1]); }
break;
case 333:
{ yyval = NULL; }
break;
case 334:
{ yyval = make_list (yyvsp[0]); }
break;
case 335:
{ yyval = make_list (yyvsp[-1]); }
break;
case 336:
{ yyval = NULL; }
break;
case 339:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 340:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 341:
{ yyval = make_node (nod_while, (int) e_while_count, yyvsp[-3], yyvsp[0], yyvsp[-6]); }
break;
case 342:
{ yyval = make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL); }
break;
case 343:
{ yyval = NULL; }
break;
case 344:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 345:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
break;
case 346:
{ yyval = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, yyvsp[-1], NULL)); }
break;
case 347:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 348:
{ yyval = NULL; }
break;
case 350:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 351:
{ yyval = make_node (nod_on_error, (int) e_err_count,
					make_list (yyvsp[-2]), yyvsp[0]); }
break;
case 353:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 354:
{ yyval = make_node (nod_sqlcode, 1, yyvsp[0]); }
break;
case 355:
{ yyval = make_node (nod_gdscode, 1, yyvsp[0]); }
break;
case 356:
{ yyval = make_node (nod_exception, 1, yyvsp[0]); }
break;
case 357:
{ yyval = make_node (nod_default, 1, NULL); }
break;
case 361:
{ yyval = make_node (nod_cursor_open, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 362:
{ yyval = make_node (nod_cursor_close, (int) e_cur_stmt_count, yyvsp[0], NULL, NULL); }
break;
case 363:
{ yyval = make_node (nod_cursor_fetch, (int) e_cur_stmt_count, yyvsp[-2], yyvsp[-3], make_list (yyvsp[0])); }
break;
case 364:
{ yyval = NULL; }
break;
case 365:
{ yyval = make_node (nod_exec_procedure, (int) e_exe_count, yyvsp[-1],
				  yyvsp[0], make_node (nod_all, (int) 0, NULL)); }
break;
case 366:
{ yyval = make_node (nod_def_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 367:
{ yyval = make_node (nod_redef_view, (int) e_view_count, 
					  yyvsp[-6], yyvsp[-5], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 368:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 369:
{ yyval = make_node (nod_list, (int) 1, yyvsp[0]); }
break;
case 370:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 371:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 372:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 373:
{ yyval = make_list (yyvsp[0]); }
break;
case 375:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 378:
{ yyval = make_node (nod_join, (int) e_join_count,
						yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 379:
{ yyval = yyvsp[-1]; }
break;
case 380:
{ lex.beginning = lex_position(); }
break;
case 381:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
				   (lex_position() == lex.end) ?
				   lex_position()-lex.beginning : lex.last_token-lex.beginning);}
break;
case 382:
{ lex.beginning = lex.last_token; }
break;
case 383:
{ yyval = (DSQL_NOD) MAKE_string(lex.beginning, 
					lex_position()-lex.beginning); }
break;
case 384:
{ yyval = make_node (nod_def_constraint, (int) e_cnstr_count, 
					MAKE_string(NULL_STRING, 0), NULL, NULL, NULL, 
					NULL, NULL, NULL, NULL, NULL); }
break;
case 385:
{ yyval = 0; }
break;
case 386:
{ yyval = make_node (nod_def_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 387:
{ yyval = make_node (nod_replace_trigger, (int) e_trg_count,
				yyvsp[-8], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 388:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 389:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 390:
{ yyval = NULL; }
break;
case 391:
{ yyval = MAKE_trigger_type (yyvsp[-1], yyvsp[0]); }
break;
case 392:
{ yyval = MAKE_constant ((STR) 0, CONSTANT_SLONG); }
break;
case 393:
{ yyval = MAKE_constant ((STR) 1, CONSTANT_SLONG); }
break;
case 394:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 0, 0), CONSTANT_SLONG); }
break;
case 395:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 0, 0), CONSTANT_SLONG); }
break;
case 396:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 0, 0), CONSTANT_SLONG); }
break;
case 397:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 2, 0), CONSTANT_SLONG); }
break;
case 398:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 3, 0), CONSTANT_SLONG); }
break;
case 399:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 1, 0), CONSTANT_SLONG); }
break;
case 400:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 3, 0), CONSTANT_SLONG); }
break;
case 401:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 1, 0), CONSTANT_SLONG); }
break;
case 402:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 2, 0), CONSTANT_SLONG); }
break;
case 403:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 2, 3), CONSTANT_SLONG); }
break;
case 404:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (1, 3, 2), CONSTANT_SLONG); }
break;
case 405:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 1, 3), CONSTANT_SLONG); }
break;
case 406:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (2, 3, 1), CONSTANT_SLONG); }
break;
case 407:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 1, 2), CONSTANT_SLONG); }
break;
case 408:
{ yyval = MAKE_constant ((STR) trigger_type_suffix (3, 2, 1), CONSTANT_SLONG); }
break;
case 409:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 410:
{ yyval = NULL; }
break;
case 411:
{ yyval = make_node (nod_list, (int) e_trg_act_count, yyvsp[-1], yyvsp[0]); }
break;
case 412:
{ yyval = yyvsp[0]; }
break;
case 413:
{ yyval = make_node (nod_mod_exception, (int) e_xcp_count, 
						yyvsp[-1], yyvsp[0]); }
break;
case 414:
{ yyval = make_node (nod_mod_relation, (int) e_alt_count, 
						yyvsp[-1], make_list (yyvsp[0])); }
break;
case 415:
{ yyval = yyvsp[0]; }
break;
case 416:
{ yyval = yyvsp[0]; }
break;
case 417:
{ yyval = make_node (nod_mod_database, (int) e_adb_count,
				make_list (yyvsp[0])); }
break;
case 418:
{ yyval = make_node (nod_mod_domain, (int) e_alt_count,
										  yyvsp[-1], make_list (yyvsp[0])); }
break;
case 419:
{ yyval = make_node (nod_mod_index, 
					 (int) e_mod_idx_count, yyvsp[0]); }
break;
case 420:
{ yyval = yyvsp[0]; }
break;
case 421:
{ yyval = make_node (nod_def_constraint, 
				  (int) e_cnstr_count, MAKE_string(NULL_STRING, 0), NULL, 
				  NULL, NULL, yyvsp[-2], NULL, yyvsp[0], NULL, NULL); }
break;
case 423:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 424:
{ yyval = make_node (nod_def_default, (int) e_dft_count,
						yyvsp[-1], yyvsp[0]); }
break;
case 425:
{ yyval = yyvsp[0]; }
break;
case 426:
{ yyval = yyvsp[0]; }
break;
case 427:
{yyval = make_node (nod_del_default, (int) 0, NULL); }
break;
case 428:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, NULL); }
break;
case 429:
{ yyval = yyvsp[0]; }
break;
case 430:
{ yyval = make_node (nod_mod_domain_type, 2, yyvsp[-1]); }
break;
case 432:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 433:
{ yyval = make_node (nod_del_field, 2, yyvsp[-1], yyvsp[0]); }
break;
case 434:
{ yyval = make_node (nod_delete_rel_constraint, (int) 1, yyvsp[0]);}
break;
case 435:
{ yyval = yyvsp[0]; }
break;
case 436:
{ yyval = yyvsp[0]; }
break;
case 437:
{ yyval = make_node (nod_mod_field_pos, 2, yyvsp[-2],
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 438:
{ yyval = make_node (nod_mod_field_name, 2, yyvsp[-2], yyvsp[0]); }
break;
case 439:
{ yyval = make_node (nod_mod_field_type, 3, yyvsp[-3], yyvsp[0], yyvsp[-1]); }
break;
case 440:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 467:
{ yyval = NULL; }
break;
case 468:
{ yyval = NULL; }
break;
case 469:
{ yyval = NULL; }
break;
case 470:
{ yyval = make_node (nod_def_domain, (int) e_dom_count,
												yyvsp[-1], NULL, NULL, NULL, NULL); }
break;
case 471:
{ lex.g_field_name = yyvsp[0];
			  lex.g_field = make_field (yyvsp[0]);
			  yyval = (DSQL_NOD) lex.g_field; }
break;
case 472:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 473:
{ yyval = make_node (nod_cascade, 0, NULL); }
break;
case 474:
{ yyval = make_node (nod_restrict, 0, NULL); }
break;
case 475:
{ yyval = make_node (nod_idx_active, 1, yyvsp[-1]); }
break;
case 476:
{ yyval = make_node (nod_idx_inactive, 1, yyvsp[-1]); }
break;
case 477:
{ lex.log_defined = FALSE;
			  lex.cache_defined = FALSE;
			  yyval = NULL; }
break;
case 479:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 480:
{ yyval = yyvsp[0]; }
break;
case 481:
{ yyval = make_node (nod_drop_log, (int) 0, NULL); }
break;
case 482:
{ yyval = yyvsp[0]; }
break;
case 483:
{ yyval = yyvsp[0]; }
break;
case 484:
{ yyval = make_node (nod_difference_file, (int) 1, yyvsp[0]); }
break;
case 485:
{ yyval = make_node (nod_drop_difference, (int) 0, NULL); }
break;
case 486:
{ yyval = make_node (nod_begin_backup, (int) 0, NULL); }
break;
case 487:
{ yyval = make_node (nod_end_backup, (int) 0, NULL); }
break;
case 489:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 490:
{ yyval = make_node (nod_mod_trigger, (int) e_trg_count,
				yyvsp[-6], NULL, yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-1], yyvsp[0], NULL); }
break;
case 492:
{ yyval = NULL; }
break;
case 494:
{ yyval = NULL; }
break;
case 495:
{ yyval = yyvsp[0]; }
break;
case 496:
{ yyval = make_node (nod_del_exception, 1, yyvsp[0]); }
break;
case 497:
{ yyval = make_node (nod_del_index, (int) 1, yyvsp[0]); }
break;
case 498:
{ yyval = make_node (nod_del_procedure, (int) 1, yyvsp[0]); }
break;
case 499:
{ yyval = make_node (nod_del_relation, (int) 1, yyvsp[0]); }
break;
case 500:
{ yyval = make_node (nod_del_trigger, (int) 1, yyvsp[0]); }
break;
case 501:
{ yyval = make_node (nod_del_view, (int) 1, yyvsp[0]); }
break;
case 502:
{ yyval = make_node (nod_del_filter, (int) 1, yyvsp[0]); }
break;
case 503:
{ yyval = make_node (nod_del_domain, (int) 1, yyvsp[0]); }
break;
case 504:
{ yyval = make_node (nod_del_udf, (int) 1, yyvsp[0]); }
break;
case 505:
{ yyval = make_node (nod_del_shadow, (int) 1, yyvsp[0]); }
break;
case 506:
{ yyval = make_node (nod_del_role, (int) 1, yyvsp[0]); }
break;
case 507:
{ yyval = make_node (nod_del_generator, (int) 1, yyvsp[0]); }
break;
case 512:
{ lex.g_field->fld_ranges = make_list (yyvsp[-1]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-3]; }
break;
case 513:
{ lex.g_field->fld_ranges = make_list (yyvsp[-2]);
			  lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
			  lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
			  yyval = yyvsp[-4]; }
break;
case 515:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 516:
{ if ((SLONG) yyvsp[0] < 1)
			 		yyval = make_node (nod_list, (int) 2, 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG), 
					MAKE_constant ((STR) 1, CONSTANT_SLONG)); 
				  else
			 		yyval = make_node (nod_list, (int) 2, 
			   		MAKE_constant ((STR) 1, CONSTANT_SLONG), 
					MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG) ); }
break;
case 517:
{ yyval = make_node (nod_list, (int) 2, 
			 	MAKE_constant ((STR) yyvsp[-2], CONSTANT_SLONG),
				MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG)); }
break;
case 523:
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
case 524:
{ 
			lex.g_field->fld_dtype = dtype_long; 
			lex.g_field->fld_length = sizeof (SLONG); 
			}
break;
case 525:
{ 
			lex.g_field->fld_dtype = dtype_short; 
			lex.g_field->fld_length = sizeof (SSHORT); 
			}
break;
case 526:
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
case 527:
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
case 528:
{ 
			lex.g_field->fld_dtype = dtype_timestamp; 
			lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
break;
case 531:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			}
break;
case 532:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-1];
			lex.g_field->fld_sub_type = 0;
			}
break;
case 533:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[-3];
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 534:
{ 
			lex.g_field->fld_dtype = dtype_blob; 
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[-1];
			}
break;
case 535:
{
			lex.g_field->fld_seg_length = (USHORT)(ULONG) yyvsp[0];
		  	}
break;
case 536:
{
			lex.g_field->fld_seg_length = (USHORT) 80;
		  	}
break;
case 537:
{
			lex.g_field->fld_sub_type = (USHORT)(ULONG) yyvsp[0];
			}
break;
case 538:
{
			lex.g_field->fld_sub_type_name = yyvsp[0];
			}
break;
case 539:
{
			lex.g_field->fld_sub_type = (USHORT) 0;
			}
break;
case 540:
{
			lex.g_field->fld_character_set = yyvsp[0];
			}
break;
case 542:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 543:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 544:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			lex.g_field->fld_flags |= FLD_national;
			}
break;
case 545:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 546:
{ 
			lex.g_field->fld_dtype = dtype_text; 
			lex.g_field->fld_character_length = 1; 
			}
break;
case 547:
{ 
			lex.g_field->fld_dtype = dtype_varying; 
			lex.g_field->fld_character_length = (USHORT)(ULONG) yyvsp[-1]; 
			}
break;
case 556:
{ 
			  lex.g_field->fld_sub_type = dsc_num_type_numeric;
			}
break;
case 557:
{  
			   lex.g_field->fld_sub_type = dsc_num_type_decimal;
			   if (lex.g_field->fld_dtype == dtype_short)
				{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				};
			}
break;
case 558:
{
			lex.g_field->fld_dtype = dtype_long; 
				lex.g_field->fld_length = sizeof (SLONG); 
			lex.g_field->fld_precision = 9;
				}
break;
case 559:
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
case 560:
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
case 563:
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
case 564:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 565:
{ 
			lex.g_field->fld_dtype = dtype_real; 
			lex.g_field->fld_length = sizeof (float); 
			}
break;
case 566:
{ 
			lex.g_field->fld_dtype = dtype_double; 
			lex.g_field->fld_length = sizeof (double); 
			}
break;
case 567:
{ yyval = yyvsp[-1]; }
break;
case 568:
{ yyval = 0; }
break;
case 572:
{ 
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
						MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG));
			}
break;
case 573:
{
			  yyval = make_node (nod_set_generator2,e_gen_id_count,yyvsp[-2],
					   MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64));
			}
break;
case 574:
{
			  yyval = make_node (nod_set_generator2, (int) e_gen_id_count, yyvsp[-3],
					  make_node(nod_negate, 1,
							MAKE_constant((STR)yyvsp[0], CONSTANT_SINT64)));
			}
break;
case 578:
{ yyval = make_node (nod_user_savepoint, 1, yyvsp[0]); }
break;
case 579:
{ yyval = make_node (nod_release_savepoint, 2, yyvsp[-1], yyvsp[0]); }
break;
case 580:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 581:
{ yyval = 0; }
break;
case 582:
{ yyval = make_node (nod_undo_savepoint, 1, yyvsp[0]); }
break;
case 585:
{ yyval = make_node (nod_commit, 1, yyvsp[0]); }
break;
case 586:
{ yyval = make_node (nod_rollback, 0, NULL); }
break;
case 589:
{ yyval = make_node (nod_commit_retain, 0, NULL); }
break;
case 590:
{ yyval = NULL; }
break;
case 592:
{ yyval = NULL; }
break;
case 593:
{yyval = make_node (nod_trans, 1, make_list (yyvsp[0])); }
break;
case 595:
{ yyval = NULL; }
break;
case 597:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-1], yyvsp[0]); }
break;
case 602:
{ yyval = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
break;
case 603:
{ yyval = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
break;
case 604:
{ yyval = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
break;
case 605:
{ yyval = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
break;
case 606:
{ yyval = yyvsp[0];}
break;
case 608:
{ yyval = yyvsp[0];}
break;
case 609:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 610:
{ yyval = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, yyvsp[0]); }
break;
case 611:
{ yyval = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
break;
case 612:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 613:
{ yyval = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
break;
case 614:
{ yyval = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
break;
case 615:
{ yyval = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
break;
case 616:
{ yyval = 0; }
break;
case 617:
{ yyval = make_node (nod_reserve, 1, make_list (yyvsp[0])); }
break;
case 618:
{ yyval = (DSQL_NOD) NOD_SHARED; }
break;
case 619:
{ yyval = (DSQL_NOD) NOD_PROTECTED ; }
break;
case 620:
{ yyval = (DSQL_NOD) 0; }
break;
case 621:
{ yyval = (DSQL_NOD) NOD_READ; }
break;
case 622:
{ yyval = (DSQL_NOD) NOD_WRITE; }
break;
case 624:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 625:
{ yyval = make_node (nod_table_lock, (int) 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 626:
{ yyval = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(SLONG) yyvsp[-1] | (SSHORT)(SLONG) yyvsp[0]), (SSHORT) 0, NULL); }
break;
case 627:
{ yyval = 0; }
break;
case 629:
{ yyval = make_node (nod_list, (int) 2, yyvsp[-2], yyvsp[0]); }
break;
case 630:
{yyval = make_node (nod_set_statistics, 
				(int)e_stat_count, yyvsp[0]); }
break;
case 631:
{ yyval = make_node (nod_select, (int) e_select_count, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 632:
{ yyval = make_node (nod_list, 1, yyvsp[0]); }
break;
case 633:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 634:
{ yyval = make_flag_node (nod_list, NOD_UNION_ALL, 2, yyvsp[-3], yyvsp[0]); }
break;
case 635:
{ yyval = make_list (yyvsp[0]); }
break;
case 636:
{ yyval = 0; }
break;
case 638:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 639:
{ yyval = make_node (nod_order, (int) e_order_count, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 640:
{ yyval = 0; }
break;
case 641:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 642:
{ yyval = 0; }
break;
case 643:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 644:
{ yyval = 0; }
break;
case 645:
{ yyval = yyvsp[-1]; }
break;
case 646:
{ yyval = 0; }
break;
case 647:
{ yyval = make_node (nod_for_update, 1, yyvsp[0]); }
break;
case 648:
{ yyval = 0; }
break;
case 649:
{ yyval = yyvsp[0]; }
break;
case 650:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 651:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 652:
{ yyval = 0; }
break;
case 653:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL, NULL); }
break;
case 654:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
					yyvsp[-8], yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 655:
{ lex.limit_clause = true; }
break;
case 656:
{ lex.limit_clause = false; }
break;
case 657:
{ lex.first_detection = true; }
break;
case 658:
{ lex.first_detection = false; }
break;
case 659:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[-1], yyvsp[-2]); }
break;
case 660:
{ yyval = make_node (nod_limit, (int) e_limit_count, NULL, yyvsp[-1]); }
break;
case 661:
{ yyval = make_node (nod_limit, (int) e_limit_count, yyvsp[0], NULL); }
break;
case 662:
{ yyval = 0; }
break;
case 663:
{ yyval = MAKE_constant ((STR) yyvsp[-1], CONSTANT_SLONG); }
break;
case 664:
{ yyval = yyvsp[-2]; }
break;
case 665:
{ yyval = yyvsp[-1]; }
break;
case 666:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 667:
{ yyval = yyvsp[-1]; }
break;
case 668:
{ yyval = yyvsp[0]; }
break;
case 669:
{ yyval = make_node (nod_flag, 0, NULL); }
break;
case 670:
{ yyval = 0; }
break;
case 671:
{ yyval = make_list (yyvsp[0]); }
break;
case 672:
{ yyval = 0; }
break;
case 674:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 676:
{ yyval = make_node (nod_alias, 2, yyvsp[-2], yyvsp[0]); }
break;
case 679:
{ yyval = make_list (yyvsp[0]); }
break;
case 681:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 685:
{ yyval = make_node(nod_derived_table, (int) e_derived_table_count, yyvsp[-4], yyvsp[-1], yyvsp[0]); }
break;
case 687:
{ yyval = NULL; }
break;
case 688:
{ yyval = make_list (yyvsp[-1]); }
break;
case 689:
{ yyval = NULL; }
break;
case 691:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 692:
{ yyval = make_node (nod_join, (int) e_join_count, yyvsp[-5], yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 693:
{ yyval = yyvsp[-1]; }
break;
case 694:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-3], yyvsp[0], yyvsp[-2]); }
break;
case 695:
{ yyval = make_node (nod_rel_proc_name, 
					(int) e_rpn_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 696:
{ yyval = make_list (yyvsp[-1]); }
break;
case 697:
{ yyval = NULL; }
break;
case 699:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[-1], yyvsp[0]); }
break;
case 700:
{ yyval = make_node (nod_relation_name, 
						(int) e_rln_count, yyvsp[0], NULL); }
break;
case 701:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 702:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 703:
{ yyval = make_node (nod_join_left, (int) 0, NULL); }
break;
case 704:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 705:
{ yyval = make_node (nod_join_right, (int) 0, NULL); }
break;
case 706:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 707:
{ yyval = make_node (nod_join_full, (int) 0, NULL); }
break;
case 708:
{ yyval = make_node (nod_join_inner, (int) 0, NULL); }
break;
case 709:
{ yyval = make_list (yyvsp[0]); }
break;
case 710:
{ yyval = 0; }
break;
case 712:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 714:
{ yyval = yyvsp[0]; }
break;
case 715:
{ yyval = 0; }
break;
case 716:
{ yyval = yyvsp[0]; }
break;
case 717:
{ yyval = 0; }
break;
case 718:
{ yyval = yyvsp[0]; }
break;
case 719:
{ yyval = 0; }
break;
case 720:
{ yyval = make_node (nod_plan_expr, 2, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 721:
{ yyval = 0; }
break;
case 722:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 723:
{ yyval = make_node (nod_merge, (int) 0, NULL); }
break;
case 724:
{ yyval = 0; }
break;
case 725:
{ yyval = 0; }
break;
case 727:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 728:
{ yyval = make_node (nod_plan_item, 2, make_list (yyvsp[-1]), yyvsp[0]); }
break;
case 731:
{ yyval = make_node (nod_list, 2, yyvsp[-1], yyvsp[0]); }
break;
case 732:
{ yyval = make_node (nod_natural, (int) 0, NULL); }
break;
case 733:
{ yyval = make_node (nod_index, 1, make_list (yyvsp[-1])); }
break;
case 734:
{ yyval = make_node (nod_index_order, 2, yyvsp[-1], yyvsp[0]); }
break;
case 736:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 737:
{ yyval = make_list (yyvsp[-1]); }
break;
case 738:
{ yyval = 0; }
break;
case 739:
{ yyval = make_node (nod_insert, (int) e_ins_count, 
			  yyvsp[-5], make_list (yyvsp[-4]), make_list (yyvsp[-1]), NULL); }
break;
case 740:
{ yyval = make_node (nod_insert, (int) e_ins_count, yyvsp[-2], yyvsp[-1], NULL, yyvsp[0]); }
break;
case 743:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], yyvsp[0], NULL); }
break;
case 744:
{ yyval = make_node (nod_delete, (int) e_del_count, yyvsp[-1], NULL, yyvsp[0]); }
break;
case 745:
{ yyval = make_node (nod_cursor, (int) e_cur_count, yyvsp[0], NULL, NULL, NULL); }
break;
case 748:
{ yyval = make_node (nod_update, (int) e_upd_count, 
				yyvsp[-3], make_list (yyvsp[-1]), yyvsp[0], NULL); }
break;
case 749:
{ yyval = make_node (nod_update, (int) e_upd_count,
				yyvsp[-3], make_list (yyvsp[-1]), NULL, yyvsp[0]); }
break;
case 751:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 752:
{ yyval = make_node (nod_assign, 2, yyvsp[0], yyvsp[-2]); }
break;
case 756:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 757:
{ yyval = make_node (nod_get_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 758:
{ yyval = make_node (nod_put_segment, (int) e_blb_count, yyvsp[-4], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
break;
case 759:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 760:
{ yyval = make_node (nod_list, 2, NULL, yyvsp[0]); }
break;
case 764:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 765:
{ yyval = yyvsp[0]; }
break;
case 767:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 770:
{ yyval = NULL; }
break;
case 771:
{ yyval = make_list (yyvsp[-1]); }
break;
case 773:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 775:
{ yyval = NULL; }
break;
case 776:
{ yyval = make_list (yyvsp[-1]); }
break;
case 778:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 780:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 781:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], NULL); }
break;
case 782:
{ yyval = make_node (nod_field_name, (int) e_fln_count,
						NULL, yyvsp[0]); }
break;
case 784:
{ yyval = make_node (nod_field_name, (int) e_fln_count, 
							yyvsp[-2], yyvsp[0]); }
break;
case 786:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 788:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 789:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 791:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 792:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 793:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 794:
{ yyval = make_node (nod_or, 2, yyvsp[-2], yyvsp[0]); }
break;
case 795:
{ yyval = make_node (nod_and, 2, yyvsp[-2], yyvsp[0]); }
break;
case 797:
{ yyval = yyvsp[-1]; }
break;
case 798:
{ yyval = make_node (nod_not, 1, yyvsp[0]); }
break;
case 809:
{ yyval = make_node (nod_eql, 2, yyvsp[-2], yyvsp[0]); }
break;
case 810:
{ yyval = make_node (nod_lss, 2, yyvsp[-2], yyvsp[0]); }
break;
case 811:
{ yyval = make_node (nod_gtr, 2, yyvsp[-2], yyvsp[0]); }
break;
case 812:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 813:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 814:
{ yyval = make_node (nod_leq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 815:
{ yyval = make_node (nod_geq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 816:
{ yyval = make_node (nod_neq, 2, yyvsp[-2], yyvsp[0]); }
break;
case 817:
{ yyval = make_node (nod_eql_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 818:
{ yyval = make_node (nod_lss_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 819:
{ yyval = make_node (nod_gtr_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 820:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 821:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 822:
{ yyval = make_node (nod_leq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 823:
{ yyval = make_node (nod_geq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 824:
{ yyval = make_node (nod_neq_all, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 825:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 826:
{ yyval = make_node (nod_lss_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 827:
{ yyval = make_node (nod_gtr_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 828:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 829:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 830:
{ yyval = make_node (nod_leq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 831:
{ yyval = make_node (nod_geq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 832:
{ yyval = make_node (nod_neq_any, 2, yyvsp[-5], yyvsp[-1]); }
break;
case 835:
{ yyval = make_node (nod_between, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 836:
{ yyval = make_node (nod_not, 1, make_node (nod_between, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 837:
{ yyval = make_node (nod_like, 2, yyvsp[-2], yyvsp[0]); }
break;
case 838:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 2, yyvsp[-3], yyvsp[0])); }
break;
case 839:
{ yyval = make_node (nod_like, 3, yyvsp[-4], yyvsp[-2], yyvsp[0]); }
break;
case 840:
{ yyval = make_node (nod_not, 1, make_node (nod_like, 
						3, yyvsp[-5], yyvsp[-2], yyvsp[0])); }
break;
case 841:
{ yyval = make_node (nod_eql_any, 2, yyvsp[-2], yyvsp[0]); }
break;
case 842:
{ yyval = make_node (nod_not, 1, make_node (nod_eql_any, 2, yyvsp[-3], yyvsp[0])); }
break;
case 843:
{ yyval = make_node (nod_containing, 2, yyvsp[-2], yyvsp[0]); }
break;
case 844:
{ yyval = make_node (nod_not, 1, make_node (nod_containing, 2, yyvsp[-3], yyvsp[0])); }
break;
case 845:
{ yyval = make_node (nod_starting, 2, yyvsp[-2], yyvsp[0]); }
break;
case 846:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-3], yyvsp[0])); }
break;
case 847:
{ yyval = make_node (nod_starting, 2, yyvsp[-3], yyvsp[0]); }
break;
case 848:
{ yyval = make_node (nod_not, 1, make_node (nod_starting, 2, yyvsp[-4], yyvsp[0])); }
break;
case 849:
{ yyval = make_node (nod_exists, 1, yyvsp[-1]); }
break;
case 850:
{ yyval = make_node (nod_singular, 1, yyvsp[-1]); }
break;
case 851:
{ yyval = make_node (nod_missing, 1, yyvsp[-2]); }
break;
case 852:
{ yyval = make_node (nod_not, 1, make_node (nod_missing, 1, yyvsp[-3])); }
break;
case 853:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 854:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 855:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 856:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 857:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 2, CONSTANT_SLONG)); }
break;
case 858:
{ yyval = make_node (nod_eql, 2,
					make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_trigger_action, CONSTANT_SLONG)),
						MAKE_constant ((STR) 3, CONSTANT_SLONG)); }
break;
case 860:
{ yyval = make_list (yyvsp[-1]); }
break;
case 861:
{ yyval = yyvsp[-1]; }
break;
case 862:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0], NULL); }
break;
case 863:
{ yyval = make_node (nod_select_expr, (int) e_sel_count, 
				yyvsp[-8], yyvsp[-7], make_list (yyvsp[-6]), yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0],
				MAKE_constant ((STR) 1, CONSTANT_SLONG)); }
break;
case 873:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 874:
{ yyval = yyvsp[0]; }
break;
case 875:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_add2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_add, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 876:
{ yyval = make_node (nod_concatenate, 2, yyvsp[-2], yyvsp[0]); }
break;
case 877:
{ yyval = make_node (nod_collate, (int) e_coll_count, (DSQL_NOD) yyvsp[0], yyvsp[-2]); }
break;
case 878:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_subtract2, 2, yyvsp[-2], yyvsp[0]);
			  else 
				  yyval = make_node (nod_subtract, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 879:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				   yyval = make_node (nod_multiply2, 2, yyvsp[-2], yyvsp[0]);
			  else
				   yyval = make_node (nod_multiply, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 880:
{
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_divide2, 2, yyvsp[-2], yyvsp[0]);
			  else
				  yyval = make_node (nod_divide, 2, yyvsp[-2], yyvsp[0]);
			}
break;
case 881:
{ yyval = yyvsp[-1]; }
break;
case 882:
{ yyval = yyvsp[-1]; }
break;
case 886:
{ yyval = make_node (nod_dbkey, 1, NULL); }
break;
case 887:
{ yyval = make_node (nod_dbkey, 1, yyvsp[-2]); }
break;
case 888:
{ 
			  yyval = make_node (nod_dom_value, 0, NULL);
						}
break;
case 889:
{ yyval = yyvsp[0]; }
break;
case 890:
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
case 891:
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
case 892:
{ yyval = make_node (nod_current_timestamp, 0, NULL); }
break;
case 893:
{ yyval = make_node (nod_array, (int) e_ary_count, yyvsp[-3], make_list (yyvsp[-1])); }
break;
case 895:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 897:
{ yyval = make_node (nod_negate, 1, yyvsp[0]); }
break;
case 898:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_STRING); }
break;
case 899:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SLONG); }
break;
case 900:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_DOUBLE); }
break;
case 901:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 902:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_SINT64); }
break;
case 904:
{ yyval = MAKE_str_constant ((STR) yyvsp[0], lex.att_charset); }
break;
case 905:
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
case 906:
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
case 907:
{ yyval = MAKE_constant ((STR) yyvsp[0], CONSTANT_TIMESTAMP); }
break;
case 908:
{ yyval = make_parameter (); }
break;
case 909:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 910:
{ yyval = make_node (nod_user_name, 0, NULL); }
break;
case 911:
{ yyval = make_node (nod_current_role, 0, NULL); }
break;
case 912:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_connection_id, CONSTANT_SLONG)); }
break;
case 913:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_transaction_id, CONSTANT_SLONG)); }
break;
case 914:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_gdscode, CONSTANT_SLONG)); }
break;
case 915:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_sqlcode, CONSTANT_SLONG)); }
break;
case 916:
{ yyval = make_node (nod_internal_info, (int) e_internal_info_count,
						MAKE_constant ((STR) internal_rows_affected, CONSTANT_SLONG)); }
break;
case 917:
{ yyval = yyvsp[0]; }
break;
case 918:
{ ((STR) yyvsp[0])->str_charset = (TEXT *) yyvsp[-1];
			  yyval = yyvsp[0]; }
break;
case 920:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 921:
{ if ((SLONG) yyvsp[0] > SHRT_POS_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 922:
{ if ((SLONG) yyvsp[0] > SHRT_NEG_MAX)
				yyabandon (-842, isc_expec_short);
				/* Short integer expected */
			  yyval = yyvsp[0];}
break;
case 923:
{ if ((SLONG) yyvsp[0] == 0)
				yyabandon (-842, isc_expec_positive);
				/* Positive number expected */
			  yyval = yyvsp[0];}
break;
case 924:
{ if ((SLONG) yyvsp[0] > SHRT_UNSIGNED_MAX)
				yyabandon (-842, isc_expec_ushort);
				/* Unsigned short integer expected */
			  yyval = yyvsp[0];}
break;
case 926:
{ yyval = (DSQL_NOD) - (SLONG) yyvsp[0]; }
break;
case 927:
{ yyval = yyvsp[0];}
break;
case 932:
{ yyval = make_node (nod_agg_count, 0, NULL); }
break;
case 933:
{ yyval = make_node (nod_agg_count, 1, yyvsp[-1]); }
break;
case 934:
{ yyval = make_flag_node (nod_agg_count,
									   NOD_AGG_DISTINCT, 1, yyvsp[-1]); }
break;
case 935:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_total2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_total, 1, yyvsp[-1]);
			}
break;
case 936:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_total2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_total,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 937:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_agg_average2, 1, yyvsp[-1]);
			  else
				  yyval = make_node (nod_agg_average, 1, yyvsp[-1]);
			}
break;
case 938:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_flag_node (nod_agg_average2,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			  else
				  yyval = make_flag_node (nod_agg_average,
						   NOD_AGG_DISTINCT, 1, yyvsp[-1]);
			}
break;
case 939:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 940:
{ yyval = make_node (nod_agg_min, 1, yyvsp[-1]); }
break;
case 941:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 942:
{ yyval = make_node (nod_agg_max, 1, yyvsp[-1]); }
break;
case 943:
{ 
			  if (client_dialect >= SQL_DIALECT_V6_TRANSITION)
				  yyval = make_node (nod_gen_id2, 2, yyvsp[-3], yyvsp[-1]);
			  else
				  yyval = make_node (nod_gen_id, 2, yyvsp[-3], yyvsp[-1]);
			}
break;
case 945:
{ yyval = make_node (nod_extract, (int) e_extract_count, yyvsp[-3], yyvsp[-1]); }
break;
case 947:
{ yyval = make_node (nod_upcase, 1, yyvsp[-1]); }
break;
case 948:
{ yyval = make_node (nod_substr, (int) e_substr_count, yyvsp[-4],
				make_node (nod_subtract, 2, yyvsp[-2],
					MAKE_constant ((STR) 1, CONSTANT_SLONG)), yyvsp[-1]); }
break;
case 949:
{ yyval = yyvsp[0]; }
break;
case 950:
{ yyval = MAKE_constant ((STR) SHRT_POS_MAX, CONSTANT_SLONG); }
break;
case 951:
{ yyval = make_node (nod_udf, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 952:
{ yyval = make_node (nod_udf, 1, yyvsp[-2]); }
break;
case 953:
{ yyval = make_node (nod_cast, (int) e_cast_count, yyvsp[-1], yyvsp[-3]); }
break;
case 956:
{ yyval = make_node (nod_searched_case, 2, 
				make_node (nod_list, 2, make_node (nod_eql, 2, yyvsp[-3], yyvsp[-1]), 
				make_node (nod_null, 0, NULL)), yyvsp[-3]); }
break;
case 957:
{ yyval = make_node (nod_coalesce, 2, yyvsp[-3], yyvsp[-1]); }
break;
case 960:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-2], make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 961:
{ yyval = make_node (nod_simple_case, 3, yyvsp[-4], make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 962:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 963:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 964:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-1]), make_node (nod_null, 0, NULL)); }
break;
case 965:
{ yyval = make_node (nod_searched_case, 2, make_list(yyvsp[-3]), yyvsp[-1]); }
break;
case 966:
{ yyval = make_node (nod_list, 2, yyvsp[-2], yyvsp[0]); }
break;
case 967:
{ yyval = make_node (nod_list, 2, yyvsp[-4], make_node (nod_list, 2, yyvsp[-2], yyvsp[0])); }
break;
case 971:
{ yyval = MAKE_constant ((STR)blr_extract_year, CONSTANT_SLONG); }
break;
case 972:
{ yyval = MAKE_constant ((STR)blr_extract_month, CONSTANT_SLONG); }
break;
case 973:
{ yyval = MAKE_constant ((STR)blr_extract_day, CONSTANT_SLONG); }
break;
case 974:
{ yyval = MAKE_constant ((STR)blr_extract_hour, CONSTANT_SLONG); }
break;
case 975:
{ yyval = MAKE_constant ((STR)blr_extract_minute, CONSTANT_SLONG); }
break;
case 976:
{ yyval = MAKE_constant ((STR)blr_extract_second, CONSTANT_SLONG); }
break;
case 977:
{ yyval = MAKE_constant ((STR)blr_extract_weekday, CONSTANT_SLONG); }
break;
case 978:
{ yyval = MAKE_constant ((STR)blr_extract_yearday, CONSTANT_SLONG); }
break;
case 981:
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
